#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <Arduino.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

// Your variable definitions + getters/setters
#include "Variables_Structure.h"
#include "Shared_Functions.h"

// Your arrays headers (must provide extern Variable <Name>[])
// Make sure these headers exist and provide the arrays
#include "Shared_Variables.h"
#include "FM_Variables.h"
#include "FC_Variables.h"
#include "FR_Variables.h"
#include "Error_Variables.h"
#include "Warning_Variables.h"

// ---------------- CONFIG (edit to match your project) ----------------

// Board ID chars (single characters)
#define ID_MAIN  'M'
#define ID_FORK  'F'
#define ID_CNC   'C'
#define ID_RPI   'R'

// The board this code runs on:
#ifndef THIS_BOARD_CHAR
#define THIS_BOARD_CHAR ID_FORK
#endif

// Max shared variables (must match your actual definition)
#ifndef MAX_SHARED_VARIABLES
#define MAX_SHARED_VARIABLES 55
#endif

// Max payload length (without 4 ASCII hex check chars)
#ifndef MAX_PAYLOAD_LEN
#define MAX_PAYLOAD_LEN 80
#endif

// Max buffer for incoming raw bytes
#ifndef INBUF_SIZE
#define INBUF_SIZE 256
#endif

// Answer expecting time in ms before retrying
#ifndef ANSWER_EXPECTING_TIME_MS
#define ANSWER_EXPECTING_TIME_MS 300
#endif

// Max number of retries (total attempts = 1 initial + MAX_RETRIES)
#ifndef MAX_RETRIES
#define MAX_RETRIES 1
#endif

// How many times the same malformed frame may appear before escalate to error
#ifndef MALFORMED_REPEAT_LIMIT
#define MALFORMED_REPEAT_LIMIT 3
#endif

// Outgoing queue capacity
#ifndef OUTGOING_QUEUE_SIZE
#define OUTGOING_QUEUE_SIZE 24
#endif

// ----------------------------------------------------------------------

class MainArduinoDataHandler {
public:
    // Singleton access
    static MainArduinoDataHandler& instance();

    // Initialize with serial port (e.g., Serial1) and baud rate
    void begin(HardwareSerial &serialPort, unsigned long baud = 115200);

    // Call frequently in loop()
    void loop();

    // API to send a message: dest is 'M'/'C'/'R', index is the macros from your headers.
    // If index <= MAX_SHARED_VARIABLES -> Shared_Vars used.
    // If index > MAX_SHARED_VARIABLES -> array selection based on dest char (FM/FC/FR).
    // expectAck: true when you expect an Ok (for info) or info (for question)
    void sendMessage(char dest, uint8_t index, bool expectAck = true);

    // Send a question (asks the remote board for the value)
    void sendQuestion(char dest, uint8_t index);

    // Send an Error/Warning (er/wr) by index (index corresponds to your Error_Vars/Warning_Vars).
    void sendError(uint16_t errorIndex);
    void sendWarning(uint16_t warningIndex);

    // Force clear queue (debug only)
    void flushQueue();

private:
    MainArduinoDataHandler();
    ~MainArduinoDataHandler();

    MainArduinoDataHandler(const MainArduinoDataHandler&) = delete;
    MainArduinoDataHandler& operator=(const MainArduinoDataHandler&) = delete;

    // Serial
    HardwareSerial *serial = nullptr;

    // Incoming raw buffer
    uint8_t inBuf[INBUF_SIZE];
    size_t inLen = 0;

    // Malformed repeated detection
    uint16_t lastMalformedHash = 0;
    uint8_t lastMalformedCount = 0;

    // Outgoing queue priority enumeration (lower number = higher priority)
    enum Priority : uint8_t {
        PRI_ERROR_WARNING    = 1, // 1: Error or Warning messages
        PRI_OK_WT            = 2, // 2: OK or WT replies
        PRI_RETRANSMIT       = 3, // 3: retransmit attempts
        PRI_ANSWER_QUESTION  = 4, // 4: answers to question messages (immediate reply)
        PRI_INFO_QUESTION    = 5  // 5: information or new question messages (lowest)
    };

    struct OutgoingItem {
        bool used = false;
        Priority priority;
        char payload[MAX_PAYLOAD_LEN + 1]; // ASCII payload (no check hex)
        uint8_t payloadLen = 0;
        uint8_t retriesLeft = 0;    // number of retries remaining
        unsigned long lastSentAt = 0;
        bool expectAck = false;     // does this item expect an Ack/Ok?
        char destChar = 0;          // destination 'M'/'C'/'R'
        uint8_t index = 0;          // associated variable index for ack matching
    };

    OutgoingItem outQueue[OUTGOING_QUEUE_SIZE];

    // awaiting ack tracking (single outstanding ack tracked)
    bool awaitingAck = false;
    char awaitingAckDest = 0;
    uint8_t awaitingAckIndex = 0;
    unsigned long awaitingAckExpireAt = 0;
    uint8_t awaitingAckRemainingRetries = 0;

    // ---------------- internal helpers ----------------

    // Read serial data into inBuf
    void readSerialNonBlocking();

    // Try to detect frames in inBuf and extract them (frame = payload + 4 hex ascii check chars)
    void extractFramesFromBuffer();

    // Handle a single payload (payload is ASCII, without trailing hex)
    void processReceivedMessage(const char *payload, size_t payloadLen);

    // Update variable based on sender and index; returns true if success
    bool updateVariable(char sender, char receiver, uint8_t index, const char *valueAscii, size_t valueLen);

    // Replies
    void replyOkMessages(char originalSender);
    void replyWtMessages(char originalSender);

    // Building payloads (no check hex appended yet)
    size_t buildInfoPayload(char *dst, size_t dstSize, char sender, char receiver, uint8_t index, const Variable *var);
    size_t buildQuestionPayload(char *dst, size_t dstSize, char sender, char receiver, uint8_t index);
    size_t buildReplyOkPayload(char *dst, size_t dstSize, char sender, char receiver);
    size_t buildReplyWtPayload(char *dst, size_t dstSize, char sender, char receiver);
    size_t buildErrorPayload(char *dst, size_t dstSize, uint16_t errIndex);
    size_t buildWarningPayload(char *dst, size_t dstSize, uint16_t warnIndex);

    // Enqueue
    bool enqueueOutgoing(Priority pri, const char *payload, size_t payloadLen, bool expectAck, char destChar, uint8_t index);

    // Process outgoing queue (pick highest priority and transmit)
    void processOutgoingQueue();

    // Transmit payload (appends ASCII hex check bytes)
    void transmitWithHexChecks(const char *payload, size_t payloadLen);

    // Check bytes helpers: compute XOR and SUM of payload bytes
    void computeXorSum(const char *payload, size_t payloadLen, uint8_t &xorOut, uint8_t &sumOut);

    // Convert byte to two ASCII hex chars (uppercase)
    void byteToHex(uint8_t b, char out[3]);

    // Convert two ASCII hex chars to byte; returns false if invalid hex
    bool hexToByte(const char *hex2, uint8_t &out);

    // Utility: index -> three-digit ascii
    void indexTo3Digits(uint8_t index, char out3[4]);

    // Format variable value into ascii (returns length)
    size_t formatVariableValue(const Variable *var, char *outBuf, size_t outSize);

    // Resolve variable pointer for a received message (receiver == THIS_BOARD_CHAR)
    Variable* resolveVariableForReceived(char sender, uint8_t index);

    // Resolve variable pointer for sending (dest indicated)
    Variable* resolveVariableForSending(char dest, uint8_t index);

    // Simple hash of payload for malformed detection
    uint16_t simpleHash(const char *data, size_t len);

    // Utilities to parse ascii values
    bool parseBoolAscii(const char *s, size_t len, bool &out);
    bool parseIntAscii(const char *s, size_t len, int &out);
    bool parseUIntAscii(const char *s, size_t len, unsigned int &out);
    bool parseLongAscii(const char *s, size_t len, long &out);
    bool parseULongAscii(const char *s, size_t len, unsigned long &out);
    bool parseFloatAscii(const char *s, size_t len, float &out);

    // ACK/WT handlers
    void handleReceivedOk(char sender, char receiver);
    void handleReceivedWt(char sender, char receiver);

    // debug print helper (enabled if SERIAL_DEBUG defined)
    void debugPrint(const char *s);
};

#endif // COMMUNICATION_H