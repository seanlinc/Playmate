// clang-format off
// AUTO-GENERATED - DO NOT EDIT
// Generated from project.dbc
#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#ifndef MIN_OF
#define MIN_OF(x,y) ((x) < (y) ? (x) : (y))
#endif
#ifndef MAX_OF
#define MAX_OF(x,y) ((x) > (y) ? (x) : (y))
#endif

/**
 * Extern dependency to use dbc_encode_and_send_*() API
 * This is designed such that the generated code does not depend on your CAN driver
 * @param argument_from_dbc_encode_and_send is a pass through argument from dbc_encode_and_send_*()
 * @returns the value returned from is returned from dbc_encode_and_send_*()
 */
extern bool dbc_send_can_message(void * argument_from_dbc_encode_and_send, uint32_t message_id, const uint8_t bytes[8], uint8_t dlc);

/// Missing in Action (MIA) structure
typedef struct {
  uint32_t mia_counter; ///< Counter used to track MIA
} dbc_mia_info_t;

/**
 * dbc_encode_*() API returns this instance to indicate message ID and DLC that should be used
 * dbc_decode_*() API checks against the provided header to make sure we are decoding the right message
 */
typedef struct {
  uint32_t message_id;  ///< CAN bus message identification
  uint8_t message_dlc;  ///< Data Length Code of the CAN message
} dbc_message_header_t;

// Enumeration for DRIVER_HEARTBEAT_cmd
typedef enum {
  DRIVER_HEARTBEAT_cmd_REBOOT = 2,
  DRIVER_HEARTBEAT_cmd_SYNC = 1,
  DRIVER_HEARTBEAT_cmd_NOOP = 0,
} DRIVER_HEARTBEAT_cmd_e;

// Message headers containing CAN message IDs and their DLCs; @see dbc_message_header_t
static const dbc_message_header_t dbc_header_DRIVER_HEARTBEAT                    = {      100U, 1 };
static const dbc_message_header_t dbc_header_SENSOR_SONARS                       = {      200U, 4 };

/**
 * DRIVER_HEARTBEAT: Sent by 'DRIVER'
 *   - Sync message used to synchronize the controllers
 *   - Expected every 1000 ms
 */
typedef struct {
  dbc_mia_info_t mia_info;

  DRIVER_HEARTBEAT_cmd_e DRIVER_HEARTBEAT_cmd; // Heartbeat command from the driver
} dbc_DRIVER_HEARTBEAT_s;

/**
 * SENSOR_SONARS: Sent by 'SENSOR'
 *   - Expected every 50 ms
 */
typedef struct {
  dbc_mia_info_t mia_info;

  uint16_t SENSOR_SONARS_left; // unit: inch
  uint16_t SENSOR_SONARS_right; // unit: inch
  uint16_t SENSOR_SONARS_middle; // unit: inch
} dbc_SENSOR_SONARS_s;

// -----------------------------------------------------------------------------
// When a message's MIA counter reaches this value
// corresponding MIA replacements occur
// -----------------------------------------------------------------------------
extern const uint32_t dbc_mia_threshold_DRIVER_HEARTBEAT;
extern const uint32_t dbc_mia_threshold_SENSOR_SONARS;

// -----------------------------------------------------------------------------
// User must define these externed instances in their code to use MIA functions
// These are copied during dbc_service_mia_*() when message MIA timeout occurs
// -----------------------------------------------------------------------------
extern const dbc_DRIVER_HEARTBEAT_s     dbc_mia_replacement_DRIVER_HEARTBEAT; // Suggested MIA threshold: (3*1000)
extern const dbc_SENSOR_SONARS_s        dbc_mia_replacement_SENSOR_SONARS; // Suggested MIA threshold: (3*50)


/**
 * Encode to transmit DRIVER_HEARTBEAT:
 *   Transmitter: 'DRIVER' with message ID 100 composed of 1 bytes
 */
static inline dbc_message_header_t dbc_encode_DRIVER_HEARTBEAT(uint8_t bytes[8], const dbc_DRIVER_HEARTBEAT_s *message) {
  uint64_t raw = 0;
  memset(bytes, 0, 8);

  // Encode to raw 8-bit signal with scale=1
  raw = ((uint64_t)((((int64_t)message->DRIVER_HEARTBEAT_cmd)))) & 0xff;
  bytes[0] |= (((uint8_t)(raw >>  0) & 0xff)     ); // 8 bits at B0

  return dbc_header_DRIVER_HEARTBEAT;
}

/// @see dbc_encode_DRIVER_HEARTBEAT(); this is its variant to encode and call dbc_send_can_message() to send the message
static inline bool dbc_encode_and_send_DRIVER_HEARTBEAT(void *argument_for_dbc_send_can_message, const dbc_DRIVER_HEARTBEAT_s *message) {
  uint8_t bytes[8];
  const dbc_message_header_t header = dbc_encode_DRIVER_HEARTBEAT(bytes, message);
  return dbc_send_can_message(argument_for_dbc_send_can_message, header.message_id, bytes, header.message_dlc);
}

/**
 * Encode to transmit SENSOR_SONARS:
 *   Transmitter: 'SENSOR' with message ID 200 composed of 4 bytes
 */
static inline dbc_message_header_t dbc_encode_SENSOR_SONARS(uint8_t bytes[8], const dbc_SENSOR_SONARS_s *message) {
  uint64_t raw = 0;
  memset(bytes, 0, 8);

  // Encode to raw 10-bit signal with scale=1 within range of [0 -> 800]
  raw = ((uint64_t)(((MAX_OF(MIN_OF((int64_t)message->SENSOR_SONARS_left,800),0))))) & 0x3ff;
  bytes[0] |= (((uint8_t)(raw >>  0) & 0xff)     ); // 8 bits at B0
  bytes[1] |= (((uint8_t)(raw >>  8) & 0x03)     ); // 2 bits at B8

  // Encode to raw 10-bit signal with scale=1
  raw = ((uint64_t)((((int64_t)message->SENSOR_SONARS_right)))) & 0x3ff;
  bytes[1] |= (((uint8_t)(raw >>  0) & 0x3f) << 2); // 6 bits at B10
  bytes[2] |= (((uint8_t)(raw >>  6) & 0x0f)     ); // 4 bits at B16

  // Encode to raw 10-bit signal with scale=1
  raw = ((uint64_t)((((int64_t)message->SENSOR_SONARS_middle)))) & 0x3ff;
  bytes[2] |= (((uint8_t)(raw >>  0) & 0x0f) << 4); // 4 bits at B20
  bytes[3] |= (((uint8_t)(raw >>  4) & 0x3f)     ); // 6 bits at B24

  return dbc_header_SENSOR_SONARS;
}

/// @see dbc_encode_SENSOR_SONARS(); this is its variant to encode and call dbc_send_can_message() to send the message
static inline bool dbc_encode_and_send_SENSOR_SONARS(void *argument_for_dbc_send_can_message, const dbc_SENSOR_SONARS_s *message) {
  uint8_t bytes[8];
  const dbc_message_header_t header = dbc_encode_SENSOR_SONARS(bytes, message);
  return dbc_send_can_message(argument_for_dbc_send_can_message, header.message_id, bytes, header.message_dlc);
}

/**
 * Decode received message DRIVER_HEARTBEAT: Sent by DRIVER
 *   Provided a dbc_message_header_t, this will attempt to decode the received message and return true upon success
 */
static inline bool dbc_decode_DRIVER_HEARTBEAT(dbc_DRIVER_HEARTBEAT_s *message, const dbc_message_header_t header, const uint8_t bytes[8]) {
  const bool success = true;

  if ((header.message_id != dbc_header_DRIVER_HEARTBEAT.message_id) || (header.message_dlc != dbc_header_DRIVER_HEARTBEAT.message_dlc)) {
    return !success;
  }

  uint64_t raw = 0;
  raw  = ((uint64_t)((bytes[0]))); // 8 bits from B0
  message->DRIVER_HEARTBEAT_cmd = (DRIVER_HEARTBEAT_cmd_e)((raw));

  message->mia_info.mia_counter = 0;
  return success;
}

/**
 * Decode received message SENSOR_SONARS: Sent by SENSOR
 *   Provided a dbc_message_header_t, this will attempt to decode the received message and return true upon success
 */
static inline bool dbc_decode_SENSOR_SONARS(dbc_SENSOR_SONARS_s *message, const dbc_message_header_t header, const uint8_t bytes[8]) {
  const bool success = true;

  if ((header.message_id != dbc_header_SENSOR_SONARS.message_id) || (header.message_dlc != dbc_header_SENSOR_SONARS.message_dlc)) {
    return !success;
  }

  uint64_t raw = 0;
  raw  = ((uint64_t)((bytes[0]))); // 8 bits from B0
  raw |= ((uint64_t)((bytes[1]) & 0x03)) << 8; // 2 bits from B8
  message->SENSOR_SONARS_left = ((raw));

  raw  = ((uint64_t)((bytes[1] >> 2) & 0x3f)); // 6 bits from B10
  raw |= ((uint64_t)((bytes[2]) & 0x0f)) << 6; // 4 bits from B16
  message->SENSOR_SONARS_right = ((raw));

  raw  = ((uint64_t)((bytes[2] >> 4) & 0x0f)); // 4 bits from B20
  raw |= ((uint64_t)((bytes[3]) & 0x3f)) << 4; // 6 bits from B24
  message->SENSOR_SONARS_middle = ((raw));

  message->mia_info.mia_counter = 0;
  return success;
}

// Do not use this function
static inline bool dbc_service_mia_for(dbc_mia_info_t *mia_info, const uint32_t increment_mia_by, const uint32_t threshold) {
  bool message_just_entered_mia = false;

  if (mia_info->mia_counter >= threshold) {
    // Message is already MIA
  } else {
    mia_info->mia_counter += increment_mia_by;
    message_just_entered_mia = (mia_info->mia_counter >= threshold);
  }

  return message_just_entered_mia;
}

static inline bool dbc_service_mia_DRIVER_HEARTBEAT(dbc_DRIVER_HEARTBEAT_s *message, const uint32_t increment_mia_by) {
  const bool message_just_entered_mia = dbc_service_mia_for(&(message->mia_info), increment_mia_by, dbc_mia_threshold_DRIVER_HEARTBEAT);

  if (message_just_entered_mia) {
    const dbc_mia_info_t previous_mia = message->mia_info;
    *message = dbc_mia_replacement_DRIVER_HEARTBEAT;
    message->mia_info = previous_mia;
  }

  return message_just_entered_mia;
}

static inline bool dbc_service_mia_SENSOR_SONARS(dbc_SENSOR_SONARS_s *message, const uint32_t increment_mia_by) {
  const bool message_just_entered_mia = dbc_service_mia_for(&(message->mia_info), increment_mia_by, dbc_mia_threshold_SENSOR_SONARS);

  if (message_just_entered_mia) {
    const dbc_mia_info_t previous_mia = message->mia_info;
    *message = dbc_mia_replacement_SENSOR_SONARS;
    message->mia_info = previous_mia;
  }

  return message_just_entered_mia;
}

// clang-format off
