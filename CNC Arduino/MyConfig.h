#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// Communication timing parameters
constexpr uint16_t ACK_TIMEOUT_MS = 300;      // Wait 300ms for acknowledgment
constexpr uint8_t MAX_RETRY_COUNT = 3;        // Maximum 3 retry attempts

// Queue configuration
constexpr uint8_t QUEUE_SIZE_PER_DEVICE = 10; // Messages per device queue
constexpr uint8_t ERROR_QUEUE_SIZE = 5;       // Error message queue size

// Message configuration
constexpr uint16_t MAX_MESSAGE_LENGTH = 64;   // Maximum message size
constexpr uint8_t MAX_VARIABLE_VALUE_SIZE = 50; // Maximum variable value size

// System parameters
constexpr uint32_t SERIAL_BAUD_RATE = 57600; // Communication baud rate

#endif