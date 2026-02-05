/*
 * fdcan_config.h
 *  -> Created on: 05-Jan-2026
 *  -> Author: athul
 */

#ifndef FDCAN_CONFIG_H_
#define FDCAN_CONFIG_H_

#include "fdcan_hw.h"
#include "utils.h"
#include <stdbool.h>
#include <stdint.h>

// CAN contorller states
typedef enum {
  CAN_IDLE = 0,
  CAN_ERROR_ACTIVE,
  CAN_ERROR_PASSIVE,
  CAN_BUS_OFF
} fdcan_state_t;

// Return status
typedef enum {
  FDCAN_OK = 0,
  FDCAN_ERROR,
  FDCAN_BUSY,
  FDCAN_TIMEOUT,
  FDCAN_INVALID_PARAM,
  FDCAN_NOT_INITIALIZED,
  FDCAN_ALREADY_INITIALIZED
} fdcan_status_t;

// Operating modes
typedef enum {
  FDCAN_MODE_NORMAL = 0,
  FDCAN_MODE_LOOPBACK_INTERNAL,
  FDCAN_MODE_LOOPBACK_EXTERNAL,
  FDCAN_MODE_BUS_MONITORING
} fdcan_mode_t;

// Frame types
typedef enum {
  FDCAN_FRAME_CLASSIC = 0,   // Classic CAN (up to 8 bytes)
  FDCAN_FRAME_FD_NO_BRS = 2, // CAN FD without bit rate switching
  FDCAN_FRAME_FD_BRS         // CAN FD with bit rate switching
} fdcan_frame_type_t;

// ID types
typedef enum {
  FDCAN_ID_STANDARD = 0, // 11-bit ID
  FDCAN_ID_EXTENDED      // 29-bit ID
} fdcan_id_type_t;

// Filter actions
typedef enum {
  STORE_TO_FIFO0 = 0,
  STORE_TO_FIFO1,
  REJECT
} fdcan_filter_action_t;

typedef struct {
  uint16_t prescaler;      // Bit rate prescaler
  uint8_t time_seg1;       // Time segment 1
  uint8_t time_seg2;       // Time segment 2
  uint8_t sync_jump_width; // Synchronization jump width
} fdcan_bit_timing_t;

typedef struct {
  fdcan_mode_t mode;                 // Operating mode
  fdcan_bit_timing_t nominal_timing; // Nominal bit timing (arbitration)
  fdcan_bit_timing_t data_timing;    // Data bit timing (CAN FD)
  bool auto_retransmission;          // Enable auto retransmission
  bool tx_pause;                     // Transmit pause feature
  bool protocol_exception;           // Protocol exception handling
  bool fd_transmission_reception;    // Enable CAN FD frames
  bool queue_mode;                   // TX FIFO/Queue mode
  // TODO:  fdcan_global_filter_t global_filter; // Global filter config
} fdcan_config_t;

typedef struct {
  uint16_t id1; // Filter ID 1
  uint16_t id2; // Filter ID 2 (for range/dual)
  // TODO:   fdcan_filter_type_t type;     // Range, Dual ID, Classic, etc.
  fdcan_filter_action_t action; // Store to FIFO0, FIFO1, Reject
} fdcan_std_filter_t;

typedef struct {
  uint32_t id1; // Extended ID 1
  uint32_t id2; // Extended ID 2
                // TODO:   fdcan_filter_type_t type;     // Filter type
  fdcan_filter_action_t action; // Filter action
} fdcan_ext_filter_t;

// TX Message structure
typedef struct {
  uint32_t id;                   // CAN ID (11 or 29 bits)
  fdcan_id_type_t id_type;       // Standard or Extended
  fdcan_frame_type_t frame_type; // Classic, FD, FD+BRS
  uint8_t dlc;                   // Data length code (0-15)
  bool rtr;                      // Remote transmission request
  uint8_t data[64];              // Payload data
} fdcan_tx_msg_t;

// RX Message structure
typedef struct {
  uint32_t id;                   // CAN ID
  fdcan_id_type_t id_type;       // Standard or Extended
  fdcan_frame_type_t frame_type; // Frame type
  uint8_t dlc;                   // Data length code
  bool rtr;                      // Remote frame flag
  uint64_t timestamp;            // Hardware timestamp
  uint8_t filter_idx;            // Matching filter index
  uint8_t data[64];              // Payload data
} fdcan_rx_msg_t;

typedef struct {
  uint32_t magic;          // Hardware instance (FDCAN1/2/3)
  FDCAN_TypeDef *instance; // Hardware instance (FDCAN1/2/3)
  fdcan_config_t config;   // Configuration

  // FDCAN status
  bool initialized;
  fdcan_state_t state; // Driver state

  // Cached RAM pointers (initialized once)
  volatile uint32_t rx_fifo0_base;
  volatile uint32_t rx_fifo1_base;
  volatile uint32_t tx_buffer_base;
  volatile uint32_t std_filter_base;
  volatile uint32_t ext_filter_base;

  // TODO : Configuration
  // uint8_t rx_fifo0_size;
  // uint8_t rx_fifo1_size;
  // uint8_t tx_buffer_count;

  // TODO : Callbacks
  /*
  fdcan_rx_callback_t rx_callback;
  fdcan_tx_callback_t tx_callback;
  fdcan_error_callback_t error_callback;
  */

  // TODO: Synchronization (for RTOS)
  /*
  void *tx_mutex;
  void *rx_mutex;
  */

  // Statistics
  uint32_t tx_count;
  uint32_t rx_count;
  uint32_t error_count;

} fdcan_handle_t;

// Common bit rates at 170MHz FDCAN clock
static const fdcan_bit_timing_t FDCAN_TIMING_500K = {
    .prescaler = 17, .time_seg1 = 16, .time_seg2 = 3, .sync_jump_width = 1};

static const fdcan_bit_timing_t FDCAN_TIMING_1M = {
    .prescaler = 10, .time_seg1 = 14, .time_seg2 = 2, .sync_jump_width = 1};

static const fdcan_bit_timing_t FDCAN_DATA_TIMING_2M = {
    .prescaler = 5, .time_seg1 = 14, .time_seg2 = 2, .sync_jump_width = 1};

static const uint8_t dlc_to_payload_size[16] = {0, 1,  2,  3,  4,  5,  6,  7,
                                                8, 12, 16, 20, 24, 32, 48, 64};

#endif
