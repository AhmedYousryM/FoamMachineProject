#include "Communication.h"

// ---------------- Singleton ----------------
MainArduinoDataHandler& MainArduinoDataHandler::instance() {
    static MainArduinoDataHandler inst;
    return inst;
}

/* This is the constructor of the class MainArduinoDataHandler.
A constructor is a special function that runs automatically when an object of the class is created.
Its job is to set up initial values for variables so the object starts in a clean, predictable state.*/

MainArduinoDataHandler::MainArduinoDataHandler() {
    serial = nullptr;         /*serial is a pointer to the serial port (like Serial1, Serial2, etc.), At first, we don’t know which serial will be used, so we set it to nullptr (means “nothing yet”), Later, another function (like begin()) will attach the real serial object.*/
    inLen = 0;                /*inLen is the length of the current incoming message we are reading, At the start, no message is being read, so it’s 0*/
    lastMalformedHash = 0;    /*This remembers the hash (a kind of ID) of the last badly formed message we got, Example: If a message was broken, cut off, or had wrong format, we mark it here, Starts at 0 (no malformed messages yet).*/
    lastMalformedCount = 0;   /*Counts how many times the same bad message has repeated, Example: if a device keeps sending broken messages, we increment this counter, Starts at 0*/
    awaitingAck = false;      /*This is a flag: it tells if we are waiting for a reply/acknowledgment from another Arduino, At the beginning, we are not waiting, so it’s false.*/
    for (int i = 0; i < OUTGOING_QUEUE_SIZE; ++i) {   /*outQueue is an array that stores messages we want to send, Each message slot has a used flag (true = message stored, false = empty), At the start, we clear the whole queue (all used = false), so it’s empty.*/
        outQueue[i].used = false;
    }
}

/* This is the destructor of the class MainArduinoDataHandler.
A destructor is a special function in C++ that runs automatically when an object is destroyed (for example, goes out of scope, or is deleted if created dynamically with new).
Its main job is usually to release resources (like memory, open files, or hardware connections).

Why is it empty?
In your code, the class doesn’t allocate any dynamic memory (new/malloc) or hold special resources that need manual cleanup.
That’s why the destructor is empty — there’s simply nothing to clean up.
C++ will automatically destroy member variables (like serial, outQueue, etc.) when the object is destroyed.*/

MainArduinoDataHandler::~MainArduinoDataHandler() {
}

// ---------------- Public API ----------------
void MainArduinoDataHandler::begin(HardwareSerial &serialPort, unsigned long baud) {
    serial = &serialPort;
    serial->begin(baud);
    inLen = 0;
    awaitingAck = false;
    lastMalformedCount = 0;
    lastMalformedHash = 0;
    debugPrint("Comm begin\n");
}

void MainArduinoDataHandler::loop() {
    if (!serial) return;

    // Read incoming bytes
    readSerialNonBlocking();

    // Extract frames if any
    extractFramesFromBuffer();

    // Process outgoing queue
    processOutgoingQueue();

    // Check awaiting ack timeout
    if (awaitingAck) {
        unsigned long now = millis();
        if (now >= awaitingAckExpireAt) {
            // expired
            if (awaitingAckRemainingRetries > 0) {
                // retry once: move queued matching item(s) to retransmit priority
                awaitingAckRemainingRetries--;
                awaitingAckExpireAt = now + ANSWER_EXPECTING_TIME_MS;
                // find matching outgoing items and bump priority
                bool found = false;
                for (int i = 0; i < OUTGOING_QUEUE_SIZE; ++i) {
                    if (outQueue[i].used && outQueue[i].destChar == awaitingAckDest && outQueue[i].index == awaitingAckIndex) {
                        outQueue[i].priority = PRI_RETRANSMIT;
                        found = true;
                    }
                }
                // send a warning to Main about retry
                sendWarning(WARNING_RETRY_OCCURRED_INDEX); // user should map index
            } else {
                // no more retries -> escalate to error and clear awaiting ack
                awaitingAck = false;
                sendError(ERROR_NO_REPLY_INDEX); // user must map to actual error macro
            }
        }
    }
}

// ---------------- Reading ----------------
void MainArduinoDataHandler::readSerialNonBlocking() {
    while (serial->available() && inLen < INBUF_SIZE) {
        inBuf[inLen++] = (uint8_t)serial->read();
    }
}

// Frame extraction: look for a payload (len >= min) followed by 4 hex chars (XOR and SUM).
void MainArduinoDataHandler::extractFramesFromBuffer() {
    // Keep scanning from buffer start. If no possible frame, return.
    // Frame format: payload (variable length) + 4 ascii hex chars for xor and sum.
    const size_t minPayloadLen = 4; // conservative minimal (could be "Er0x" or "F C:Ok" etc.)
    size_t scanPos = 0;
    while (inLen >= (minPayloadLen + 4)) {
        bool foundFrame = false;
        size_t foundPayloadLen = 0;

        // Attempt payloadLen from minPayloadLen up to inLen - 4
        size_t maxCandidate = (inLen >= 4) ? inLen - 4 : 0;
        for (size_t payloadLen = minPayloadLen; payloadLen <= maxCandidate && payloadLen <= MAX_PAYLOAD_LEN; ++payloadLen) {
            // check if next 4 chars are valid hex
            const uint8_t *hexPtr = &inBuf[payloadLen];
            bool okHex = true;
            for (int h = 0; h < 4; ++h) {
                char c = (char)hexPtr[h];
                bool isHex = (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
                if (!isHex) { okHex = false; break; }
            }
            if (!okHex) continue;

            // compute xor and sum over payload bytes
            uint8_t xorv = 0;
            uint8_t sumv = 0;
            for (size_t i = 0; i < payloadLen; ++i) {
                uint8_t b = inBuf[i];
                xorv ^= b;
                sumv += b;
            }
            // parse hex ascii -> bytes
            char hexBuf[3] = { (char)hexPtr[0], (char)hexPtr[1], 0 };
            uint8_t chkX = 0, chkS = 0;
            if (!hexToByte(hexBuf, chkX)) continue;
            hexBuf[0] = (char)hexPtr[2];
            hexBuf[1] = (char)hexPtr[3];
            hexBuf[2] = 0;
            if (!hexToByte(hexBuf, chkS)) continue;

            if (chkX == xorv && chkS == sumv) {
                foundFrame = true;
                foundPayloadLen = payloadLen;
                break;
            }
        }

        if (!foundFrame) {
            // no valid frame starting at buffer start: drop first byte and continue
            memmove(inBuf, inBuf + 1, --inLen);
            continue;
        }

        // Extract payload into null-terminated string
        if (foundPayloadLen > MAX_PAYLOAD_LEN) foundPayloadLen = MAX_PAYLOAD_LEN;
        char payloadBuf[MAX_PAYLOAD_LEN + 1];
        memcpy(payloadBuf, inBuf, foundPayloadLen);
        payloadBuf[foundPayloadLen] = '\0';

        // Process extracted payload
        processReceivedMessage(payloadBuf, foundPayloadLen);

        // Remove frame (payload + 4 hex chars) from inBuf
        size_t removeCount = foundPayloadLen + 4;
        if (removeCount < inLen) {
            memmove(inBuf, inBuf + removeCount, inLen - removeCount);
            inLen -= removeCount;
        } else {
            inLen = 0;
        }
    }
}

// ---------------- Utilities: hex <-> byte ----------------
void MainArduinoDataHandler::byteToHex(uint8_t b, char out[3]) {
    const char hex[] = "0123456789ABCDEF";
    out[0] = hex[(b >> 4) & 0x0F];
    out[1] = hex[b & 0x0F];
    out[2] = '\0';
}

bool MainArduinoDataHandler::hexToByte(const char *hex2, uint8_t &out) {
    auto hexVal = [](char c)->int {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
        if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
        return -1;
    };
    int hi = hexVal(hex2[0]);
    int lo = hexVal(hex2[1]);
    if (hi < 0 || lo < 0) return false;
    out = (uint8_t)((hi << 4) | lo);
    return true;
}

void MainArduinoDataHandler::computeXorSum(const char *payload, size_t payloadLen, uint8_t &xorOut, uint8_t &sumOut) {
    xorOut = 0; sumOut = 0;
    for (size_t i = 0; i < payloadLen; ++i) {
        uint8_t b = (uint8_t)payload[i];
        xorOut ^= b;
        sumOut += b;
    }
}

// ---------------- Building payloads ----------------
void MainArduinoDataHandler::indexTo3Digits(uint8_t index, char out3[4]) {
    out3[0] = '0' + ((index / 100) % 10);
    out3[1] = '0' + ((index / 10) % 10);
    out3[2] = '0' + (index % 10);
    out3[3] = '\0';
}

size_t MainArduinoDataHandler::formatVariableValue(const Variable *var, char *outBuf, size_t outSize) {
    if (!var || outSize == 0) return 0;
    switch (var->type) {
        case VAR_BOOL:
            outBuf[0] = var->value.asBool ? 'T' : 'F';
            outBuf[1] = '\0';
            return 1;
        case VAR_CHAR:
            return (size_t)snprintf(outBuf, outSize, "%d", (int)var->value.asChar);
        case VAR_UCHAR:
            return (size_t)snprintf(outBuf, outSize, "%u", (unsigned int)var->value.asUChar);
        case VAR_INT:
            return (size_t)snprintf(outBuf, outSize, "%d", var->value.asInt);
        case VAR_UINT:
            return (size_t)snprintf(outBuf, outSize, "%u", var->value.asUInt);
        case VAR_LONG:
            return (size_t)snprintf(outBuf, outSize, "%ld", var->value.asLong);
        case VAR_ULONG:
            return (size_t)snprintf(outBuf, outSize, "%lu", var->value.asULong);
        case VAR_FLOAT:
            // note: float formatting depends on libc/avr-libc enabling floats in printf.
            return (size_t)snprintf(outBuf, outSize, "%.3f", var->value.asFloat);
        case VAR_STRING:
            if (var->value.asString == nullptr) return 0;
            strncpy(outBuf, var->value.asString, outSize - 1);
            outBuf[outSize - 1] = '\0';
            return strlen(outBuf);
        default:
            return 0;
    }
}

size_t MainArduinoDataHandler::buildInfoPayload(char *dst, size_t dstSize, char sender, char receiver, uint8_t index, const Variable *var) {
    if (dstSize == 0) return 0;
    char idx3[4];
    indexTo3Digits(index, idx3);
    char valBuf[MAX_PAYLOAD_LEN];
    size_t vlen = formatVariableValue(var, valBuf, sizeof(valBuf));
    int n = snprintf(dst, dstSize, "%c%c%s=%.*s", sender, receiver, idx3, (int)vlen, valBuf);
    return n < 0 ? 0 : (size_t)n;
}

size_t MainArduinoDataHandler::buildQuestionPayload(char *dst, size_t dstSize, char sender, char receiver, uint8_t index) {
    if (dstSize == 0) return 0;
    char idx3[4];
    indexTo3Digits(index, idx3);
    int n = snprintf(dst, dstSize, "%c%c%s?", sender, receiver, idx3);
    return n < 0 ? 0 : (size_t)n;
}

size_t MainArduinoDataHandler::buildReplyOkPayload(char *dst, size_t dstSize, char sender, char receiver) {
    if (dstSize == 0) return 0;
    int n = snprintf(dst, dstSize, "%c%c:Ok", sender, receiver);
    return n < 0 ? 0 : (size_t)n;
}

size_t MainArduinoDataHandler::buildReplyWtPayload(char *dst, size_t dstSize, char sender, char receiver) {
    if (dstSize == 0) return 0;
    int n = snprintf(dst, dstSize, "%c%c:WT", sender, receiver);
    return n < 0 ? 0 : (size_t)n;
}

size_t MainArduinoDataHandler::buildErrorPayload(char *dst, size_t dstSize, uint16_t errIndex) {
    if (dstSize == 0) return 0;
    char idx3[4]; indexTo3Digits((uint8_t)(errIndex & 0xFF), idx3);
    int n = snprintf(dst, dstSize, "Er%s", idx3);
    return n < 0 ? 0 : (size_t)n;
}

size_t MainArduinoDataHandler::buildWarningPayload(char *dst, size_t dstSize, uint16_t warnIndex) {
    if (dstSize == 0) return 0;
    char idx3[4]; indexTo3Digits((uint8_t)(warnIndex & 0xFF), idx3);
    int n = snprintf(dst, dstSize, "Wr%s", idx3);
    return n < 0 ? 0 : (size_t)n;
}

// ---------------- Enqueue ----------------
bool MainArduinoDataHandler::enqueueOutgoing(Priority pri, const char *payload, size_t payloadLen, bool expectAck, char destChar, uint8_t index) {
    if (payloadLen == 0 || payloadLen > MAX_PAYLOAD_LEN) return false;
    // find free slot
    int freeIdx = -1;
    for (int i = 0; i < OUTGOING_QUEUE_SIZE; ++i) {
        if (!outQueue[i].used) { freeIdx = i; break; }
    }
    if (freeIdx < 0) {
        // try to evict a lower priority item
        int worstIdx = -1;
        uint8_t worstPri = 0;
        for (int i = 0; i < OUTGOING_QUEUE_SIZE; ++i) {
            if (outQueue[i].used) {
                if (worstIdx < 0 || outQueue[i].priority > worstPri) {
                    worstPri = outQueue[i].priority;
                    worstIdx = i;
                }
            }
        }
        if (worstIdx >= 0 && worstPri > pri) {
            freeIdx = worstIdx; // overwrite
        } else {
            return false;
        }
    }
    OutgoingItem &it = outQueue[freeIdx];
    it.used = true;
    it.priority = pri;
    it.payloadLen = (uint8_t)payloadLen;
    memcpy(it.payload, payload, it.payloadLen);
    it.payload[it.payloadLen] = '\0';
    it.retriesLeft = MAX_RETRIES;
    it.lastSentAt = 0;
    it.expectAck = expectAck;
    it.destChar = destChar;
    it.index = index;
    return true;
}

// ---------------- Transmit queue processing ----------------
void MainArduinoDataHandler::processOutgoingQueue() {
    // pick highest priority (lowest numeric)
    int bestIdx = -1;
    Priority bestPri = (Priority)(PRI_INFO_QUESTION + 1);
    for (int i = 0; i < OUTGOING_QUEUE_SIZE; ++i) {
        if (!outQueue[i].used) continue;
        if (outQueue[i].priority < bestPri) {
            bestPri = outQueue[i].priority;
            bestIdx = i;
        }
    }
    if (bestIdx < 0) return;

    OutgoingItem &it = outQueue[bestIdx];

    // If the item expects ack and we already have awaitingAck and it's for a different destination/index, we should
    // allow higher priority retransmits; otherwise we avoid sending another expecting-ack message while one outstanding exists.
    if (it.expectAck && awaitingAck) {
        // allow retransmit-priority items to be sent even if awaitingAck is set
        if (it.priority != PRI_RETRANSMIT) return;
    }

    // throttle small re-sends
    unsigned long now = millis();
    if (it.lastSentAt != 0 && (now - it.lastSentAt) < 10) return;

    // transmit
    transmitWithHexChecks(it.payload, it.payloadLen);
    it.lastSentAt = now;

    if (it.expectAck) {
        // set awaiting ack info (single outstanding ack)
        awaitingAck = true;
        awaitingAckDest = it.destChar;
        awaitingAckIndex = it.index;
        awaitingAckExpireAt = now + ANSWER_EXPECTING_TIME_MS;
        awaitingAckRemainingRetries = it.retriesLeft;
        // note: retriesLeft indicates number of retransmits left after the initial send
    } else {
        // no ack expected: done
        it.used = false;
    }

    // If this was a retransmit attempt, decrement retriesLeft
    if (it.priority == PRI_RETRANSMIT && it.retriesLeft > 0) {
        it.retriesLeft--;
    }
}

// ---------------- Transmit payload with ASCII-HEX checks ----------------
void MainArduinoDataHandler::transmitWithHexChecks(const char *payload, size_t payloadLen) {
    if (!serial) return;
    // compute xor and sum
    uint8_t xorv = 0, sumv = 0;
    computeXorSum(payload, payloadLen, xorv, sumv);

    // write payload bytes as ASCII
    serial->write((const uint8_t*)payload, payloadLen);

    // convert xor and sum to two hex ascii each and write
    char hexX[3], hexS[3];
    byteToHex(xorv, hexX);
    byteToHex(sumv, hexS);
    // write the 4 ASCII hex characters
    serial->write((const uint8_t*)hexX, 2);
    serial->write((const uint8_t*)hexS, 2);

    // (Optionally append newline for debug; not added by default to stay protocol-pure)
}

// ---------------- Received payload processing ----------------
void MainArduinoDataHandler::processReceivedMessage(const char *payload, size_t payloadLen) {
    if (payloadLen < 2) return;
    // Error/Warning (these are typically addressed to Main only; Fork typically doesn't act on them)
    if (payloadLen >= 3 && payload[0] == 'E' && payload[1] == 'r') {
        // Received ErNNN - ignore on fork (or log)
        debugPrint("Received Er.. ignoring\n");
        return;
    }
    if (payloadLen >= 3 && payload[0] == 'W' && payload[1] == 'r') {
        debugPrint("Received Wr.. ignoring\n");
        return;
    }

    // Replies with colon format: "XY:Ok" or "XY:WT"
    if (payloadLen >= 5 && payload[2] == ':') {
        char src = payload[0];
        char dst = payload[1];
        // Only process replies addressed to this board
        if (dst != THIS_BOARD_CHAR) return;
        if (payloadLen >= 6 && strncmp(payload + 3, "Ok", 2) == 0) {
            handleReceivedOk(src, dst);
            return;
        } else if (payloadLen >= 6 && strncmp(payload + 3, "WT", 2) == 0) {
            handleReceivedWt(src, dst);
            return;
        } else {
            // malformed reply
            uint16_t h = simpleHash(payload, payloadLen);
            if (h == lastMalformedHash) lastMalformedCount++; else { lastMalformedHash = h; lastMalformedCount = 1; }
            replyWtMessages(src);
            sendWarning(WARNING_INCORRECT_MESSAGE_INDEX);
            if (lastMalformedCount >= MALFORMED_REPEAT_LIMIT) sendError(ERROR_WRONG_MESSAGE_INDEX);
            return;
        }
    }

    // Information or Question formats: need at least 6 chars for XYDDD= or XYDDD?
    if (payloadLen >= 6) {
        char s = payload[0];
        char r = payload[1];
        // index 3 digits payload[2..4]
        char idxStr[4] = { payload[2], payload[3], payload[4], '\0' };
        int idx = atoi(idxStr);
        char op = payload[5];

        // If message is not for this board, ignore (Main will route)
        if (r != THIS_BOARD_CHAR) return;

        // Validate index
        if (idx < 0 || idx > 255) {
            replyWtMessages(s);
            sendWarning(WARNING_INCORRECT_MESSAGE_INDEX);
            return;
        }

        if (op == '=') {
            // info message; value from payload[6..end)
            const char *valuePtr = (payloadLen > 6) ? &payload[6] : "";
            size_t valueLen = (payloadLen > 6) ? payloadLen - 6 : 0;

            bool success = updateVariable(s, r, (uint8_t)idx, valuePtr, valueLen);
            if (success) {
                // update done -> send Ok back to original sender
                replyOkMessages(s);
            } else {
                // failed update -> reply WT and send warning to Main
                replyWtMessages(s);
                sendWarning(WARNING_INCORRECT_MESSAGE_INDEX);
            }
            return;
        } else if (op == '?') {
            // question message: respond with info (immediate)
            Variable *var = resolveVariableForReceived(s, (uint8_t)idx);
            if (!var) {
                replyWtMessages(s);
                sendWarning(WARNING_INCORRECT_MESSAGE_INDEX);
                return;
            }
            // build info payload to reply (sender = THIS_BOARD_CHAR, receiver = original sender s)
            char out[MAX_PAYLOAD_LEN + 1];
            size_t outLen = buildInfoPayload(out, sizeof(out), THIS_BOARD_CHAR, s, (uint8_t)idx, var);
            // enqueue as answer (priority 4)
            enqueueOutgoing(PRI_ANSWER_QUESTION, out, outLen, true, s, (uint8_t)idx);
            return;
        } else {
            // unknown op
            replyWtMessages(s);
            sendWarning(WARNING_INCORRECT_MESSAGE_INDEX);
            return;
        }
    }

    // Malformed fallback
    uint16_t h = simpleHash(payload, payloadLen);
    if (h == lastMalformedHash) lastMalformedCount++; else { lastMalformedHash = h; lastMalformedCount = 1; }
    // try to guess original sender as payload[0] if available
    if (payloadLen >= 1) replyWtMessages(payload[0]);
    sendWarning(WARNING_INCORRECT_MESSAGE_INDEX);
    if (lastMalformedCount >= MALFORMED_REPEAT_LIMIT) sendError(ERROR_WRONG_MESSAGE_INDEX);
}

// ---------------- Update variable ----------------
bool MainArduinoDataHandler::updateVariable(char sender, char receiver, uint8_t index, const char *valueAscii, size_t valueLen) {
    Variable *var = resolveVariableForReceived(sender, index);
    if (!var) return false;

    switch (var->type) {
        case VAR_BOOL: {
            bool b;
            if (!parseBoolAscii(valueAscii, valueLen, b)) return false;
            SET_BOOL((*var), b);
            return true;
        }
        case VAR_CHAR: {
            int v;
            if (!parseIntAscii(valueAscii, valueLen, v)) return false;
            SET_CHAR((*var), (char)v);
            return true;
        }
        case VAR_UCHAR: {
            unsigned int v;
            if (!parseUIntAscii(valueAscii, valueLen, v)) return false;
            SET_UCHAR((*var), (unsigned char)v);
            return true;
        }
        case VAR_INT: {
            int v;
            if (!parseIntAscii(valueAscii, valueLen, v)) return false;
            SET_INT((*var), v);
            return true;
        }
        case VAR_UINT: {
            unsigned int v;
            if (!parseUIntAscii(valueAscii, valueLen, v)) return false;
            SET_UINT((*var), v);
            return true;
        }
        case VAR_LONG: {
            long v;
            if (!parseLongAscii(valueAscii, valueLen, v)) return false;
            SET_LONG((*var), v);
            return true;
        }
        case VAR_ULONG: {
            unsigned long v;
            if (!parseULongAscii(valueAscii, valueLen, v)) return false;
            SET_ULONG((*var), v);
            return true;
        }
        case VAR_FLOAT: {
            float f;
            if (!parseFloatAscii(valueAscii, valueLen, f)) return false;
            SET_FLOAT((*var), f);
            return true;
        }
        case VAR_STRING: {
            // allocate a copy - be careful on AVR (may not have much heap).
            char *copy = (char*)malloc(valueLen + 1);
            if (!copy) return false;
            memcpy(copy, valueAscii, valueLen);
            copy[valueLen] = '\0';
            SET_STRING((*var), (const char*)copy);
            return true;
        }
        default:
            return false;
    }
}

// ---------------- Replies ----------------
void MainArduinoDataHandler::replyOkMessages(char originalSender) {
    char buf[MAX_PAYLOAD_LEN + 1];
    size_t n = buildReplyOkPayload(buf, sizeof(buf), THIS_BOARD_CHAR, originalSender);
    enqueueOutgoing(PRI_OK_WT, buf, n, false, originalSender, 0);
}

void MainArduinoDataHandler::replyWtMessages(char originalSender) {
    char buf[MAX_PAYLOAD_LEN + 1];
    size_t n = buildReplyWtPayload(buf, sizeof(buf), THIS_BOARD_CHAR, originalSender);
    enqueueOutgoing(PRI_OK_WT, buf, n, false, originalSender, 0);
}

// ---------------- Received Ok/WT handling ----------------
void MainArduinoDataHandler::handleReceivedOk(char sender, char receiver) {
    // only care if Ok is for this board
    if (receiver != THIS_BOARD_CHAR) return;
    // if awaiting ack and matches sender & index, clear awaiting ack and remove corresponding queue items
    if (awaitingAck && awaitingAckDest == sender) {
        awaitingAck = false;
        // remove matching outgoing items
        for (int i = 0; i < OUTGOING_QUEUE_SIZE; ++i) {
            if (outQueue[i].used && outQueue[i].destChar == sender && outQueue[i].index == awaitingAckIndex) {
                outQueue[i].used = false;
            }
        }
    }
}

void MainArduinoDataHandler::handleReceivedWt(char sender, char receiver) {
    if (receiver != THIS_BOARD_CHAR) return;
    // find outgoing matching items and mark for retransmit
    bool found = false;
    for (int i = 0; i < OUTGOING_QUEUE_SIZE; ++i) {
        if (outQueue[i].used && outQueue[i].destChar == sender) {
            outQueue[i].priority = PRI_RETRANSMIT;
            found = true;
        }
    }
    if (!found) {
        // nothing to retransmit -> escalate
        sendError(ERROR_MESSAGE_MISSING_INDEX);
    }
}

// ---------------- Sending API ----------------
void MainArduinoDataHandler::sendMessage(char dest, uint8_t index, bool expectAck) {
    // resolve var to read
    Variable *v = resolveVariableForSending(dest, index);
    if (!v) {
        // can't resolve -> warn
        sendWarning(WARNING_INCORRECT_MESSAGE_INDEX);
        return;
    }
    char payload[MAX_PAYLOAD_LEN + 1];
    size_t len = buildInfoPayload(payload, sizeof(payload), THIS_BOARD_CHAR, dest, index, v);
    enqueueOutgoing(PRI_INFO_QUESTION, payload, len, expectAck, dest, index);
}

void MainArduinoDataHandler::sendQuestion(char dest, uint8_t index) {
    char payload[MAX_PAYLOAD_LEN + 1];
    size_t len = buildQuestionPayload(payload, sizeof(payload), THIS_BOARD_CHAR, dest, index);
    enqueueOutgoing(PRI_INFO_QUESTION, payload, len, true, dest, index);
}

void MainArduinoDataHandler::sendError(uint16_t errorIndex) {
    char payload[MAX_PAYLOAD_LEN + 1];
    size_t len = buildErrorPayload(payload, sizeof(payload), errorIndex);
    enqueueOutgoing(PRI_ERROR_WARNING, payload, len, true, ID_MAIN, (uint8_t)(errorIndex & 0xFF));
}

void MainArduinoDataHandler::sendWarning(uint16_t warningIndex) {
    char payload[MAX_PAYLOAD_LEN + 1];
    size_t len = buildWarningPayload(payload, sizeof(payload), warningIndex);
    enqueueOutgoing(PRI_ERROR_WARNING, payload, len, true, ID_MAIN, (uint8_t)(warningIndex & 0xFF));
}

void MainArduinoDataHandler::flushQueue() {
    for (int i = 0; i < OUTGOING_QUEUE_SIZE; ++i) outQueue[i].used = false;
    awaitingAck = false;
}

// ---------------- Resolve variable helpers ----------------
Variable* MainArduinoDataHandler::resolveVariableForReceived(char sender, uint8_t index) {
    // if index in shared range
    if (index <= MAX_SHARED_VARIABLES) {
        extern Variable Shared_Var[]; // must be defined in Shared_Variables.cpp
        return &Shared_Var[index];
    } else {
        // choose array based on sender (which indicates original array)
        if (sender == ID_MAIN) { // message from Main -> variable located in FM_Vars
            extern Variable FM_Vars[];
            return &FM_Vars[index];
        } else if (sender == ID_CNC) {
            extern Variable FC_Vars[];
            return &FC_Vars[index];
        } else if (sender == ID_RPI) {
            extern Variable FR_Vars[];
            return &FR_Vars[index];
        } else {
            return nullptr;
        }
    }
}

Variable* MainArduinoDataHandler::resolveVariableForSending(char dest, uint8_t index) {
    if (index <= MAX_SHARED_VARIABLES) {
        extern Variable Shared_Var[];
        return &Shared_Var[index];
    } else {
        // dest selects which non-shared array the macro belongs to
        if (dest == ID_MAIN) {
            extern Variable FM_Vars[];
            return &FM_Vars[index];
        } else if (dest == ID_CNC) {
            extern Variable FC_Vars[];
            return &FC_Vars[index];
        } else if (dest == ID_RPI) {
            extern Variable FR_Vars[];
            return &FR_Vars[index];
        } else {
            return nullptr;
        }
    }
}

// ---------------- Simple hash ----------------
uint16_t MainArduinoDataHandler::simpleHash(const char *data, size_t len) {
    uint16_t h = 21661;
    for (size_t i = 0; i < len; ++i) {
        h = (uint16_t)((h ^ (uint8_t)data[i]) * 16777619u);
    }
    return h;
}

// ---------------- Parse utilities ----------------
bool MainArduinoDataHandler::parseBoolAscii(const char *s, size_t len, bool &out) {
    if (len == 1 && (s[0] == 'T' || s[0] == 'F')) {
        out = (s[0] == 'T');
        return true;
    }
    if (len == 1 && (s[0] == '1' || s[0] == '0')) {
        out = (s[0] == '1');
        return true;
    }
    return false;
}

bool MainArduinoDataHandler::parseIntAscii(const char *s, size_t len, int &out) {
    if (len == 0) return false;
    char tmp[MAX_PAYLOAD_LEN + 1];
    size_t c = (len < sizeof(tmp) - 1) ? len : sizeof(tmp) - 1;
    memcpy(tmp, s, c); tmp[c] = '\0';
    out = atoi(tmp);
    return true;
}

bool MainArduinoDataHandler::parseUIntAscii(const char *s, size_t len, unsigned int &out) {
    if (len == 0) return false;
    char tmp[MAX_PAYLOAD_LEN + 1];
    size_t c = (len < sizeof(tmp) - 1) ? len : sizeof(tmp) - 1;
    memcpy(tmp, s, c); tmp[c] = '\0';
    out = (unsigned int)strtoul(tmp, NULL, 10);
    return true;
}

bool MainArduinoDataHandler::parseLongAscii(const char *s, size_t len, long &out) {
    if (len == 0) return false;
    char tmp[MAX_PAYLOAD_LEN + 1];
    size_t c = (len < sizeof(tmp) - 1) ? len : sizeof(tmp) - 1;
    memcpy(tmp, s, c); tmp[c] = '\0';
    out = strtol(tmp, NULL, 10);
    return true;
}

bool MainArduinoDataHandler::parseULongAscii(const char *s, size_t len, unsigned long &out) {
    if (len == 0) return false;
    char tmp[MAX_PAYLOAD_LEN + 1];
    size_t c = (len < sizeof(tmp) - 1) ? len : sizeof(tmp) - 1;
    memcpy(tmp, s, c); tmp[c] = '\0';
    out = strtoul(tmp, NULL, 10);
    return true;
}

bool MainArduinoDataHandler::parseFloatAscii(const char *s, size_t len, float &out) {
    if (len == 0) return false;
    char tmp[MAX_PAYLOAD_LEN + 1];
    size_t c = (len < sizeof(tmp) - 1) ? len : sizeof(tmp) - 1;
    memcpy(tmp, s, c); tmp[c] = '\0';
    out = (float)atof(tmp);
    return true;
}

// ---------------- Debug print ----------------
void MainArduinoDataHandler::debugPrint(const char *s) {
#ifdef SERIAL_DEBUG
    Serial.print(s);
#endif
}