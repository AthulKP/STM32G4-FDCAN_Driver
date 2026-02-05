/*
 * fdcan.c
 *  -> Created on: 21-Jan-2026
 *  -> Author: Athul
 *  -> © 2026 Athul.
 */

#include "fdcan.h"        // API Implementation
#include "fdcan_config.h" // enums and config params
#include "fdcan_hw.h"     // HW specfic values
#include "timer.h"
#include "utils.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <sys/_intsup.h>
#include <sys/select.h>

fdcan_status_t initialize_fdcan(FDCAN_TypeDef *instance,
                                const fdcan_config_t *config) {
  fdcan_handle_t *handle = fdcan_get_handle(instance);
  uint32_t ram_base = fdcan_get_ram_base(instance);

  if (!handle) {
    return FDCAN_INVALID_PARAM;
  }

  // Check if already initialized
  if (handle->magic == FDCAN_INIT_MAGIC && handle->initialized) {
    return FDCAN_ALREADY_INITIALIZED;
  }
  // Reset and set init bit
  instance->CCCR = 0x00000001; // CCCR reset value
  while (!(instance->CCCR & FDCAN_INITIALIZATION_EN))
    ;

  // Set Configuration change CCE = 1
  instance->CCCR |= FDCAN_CONFIGURATION_CHANGE_EN;

  // Enable FD operation
  if (config->fd_transmission_reception == true) {
    instance->CCCR |= FDCAN_FDO_EN;
  }

  // TODO : handle other modes
  if (config->mode == FDCAN_MODE_LOOPBACK_INTERNAL) {
    // Set test mode enable
    instance->CCCR |= FDCAN_BUS_MONITORING_MODE_EN | FDCAN_TEST_MODE_EN;
    // Enable Loop-back mode
    instance->TEST |= FDCAN_LOOPBACK_MODE_EN;
  }

  // Set bit timing considering clock at 170MHz
  instance->NBTP = (uint32_t)((config->nominal_timing.sync_jump_width << 25) |
                              (config->nominal_timing.prescaler << 16) |
                              (config->nominal_timing.time_seg1 << 8) |
                              (config->nominal_timing.time_seg2 << 0));

  if (config->fd_transmission_reception == true) {
    instance->DBTP = (uint32_t)((config->nominal_timing.prescaler << 16) |
                                (config->nominal_timing.time_seg1 << 8) |
                                (config->nominal_timing.time_seg2 << 4) |
                                (config->data_timing.sync_jump_width << 0));
  }
  
  // During FD mode the internal time counter is not reliable so fixing to timer 3.
  instance->TSCC = TIMESTAMP_TIM3;

  // The TX is programmed as FIFO or Queue
  if (config->queue_mode == true) {
    instance->TXBC |= EN_CANFD_TX_QUEUE;
  } else {
    instance->TXBC &= EN_CANFD_TX_FIFO;
  }

  // Clear Init and CCE bit to complete the initialization
  instance->CCCR &= ~(FDCAN_INITIALIZATION_EN | FDCAN_CONFIGURATION_CHANGE_EN);

  // Wait until INIT
  while ((instance->CCCR & FDCAN_INITIALIZATION_EN))
    ;

  // Initialize RAM pointers
  handle->rx_fifo0_base =
      (volatile uint32_t)(ram_base + FDCAN_RAM_RX_FIFO0_OFFSET);
  handle->rx_fifo1_base =
      (volatile uint32_t)(ram_base + FDCAN_RAM_RX_FIFO1_OFFSET);
  handle->tx_buffer_base =
      (volatile uint32_t)(ram_base + FDCAN_RAM_TX_BUFFERS_OFFSET);
  handle->std_filter_base =
      (volatile uint32_t)(ram_base + FDCAN_RAM_STD_FILTERS_OFFSET);
  handle->ext_filter_base =
      (volatile uint32_t)(ram_base + FDCAN_RAM_EXT_FILTERS_OFFSET);

  handle->magic = FDCAN_INIT_MAGIC;
  handle->initialized = true;

  return FDCAN_OK;
}

fdcan_status_t send_can_frame(FDCAN_TypeDef *instance,
                              fdcan_tx_msg_t *can_msg) {

  fdcan_handle_t *handle = fdcan_get_handle(instance);
  uint8_t tx_put_index = (uint8_t)((instance->TXFQS >> 16) & 0x3);
  uint32_t *tx_buffer = (uint32_t *)(handle->tx_buffer_base +
                                     (FDCAN_MSG_ELEMENT_SIZE * tx_put_index));
  if (can_msg->id_type == FDCAN_ID_EXTENDED) {
    *tx_buffer++ = ((can_msg->id_type << 30) | can_msg->id);
  } else {
    *tx_buffer++ = (uint32_t)(can_msg->id << 18);
  }
  *tx_buffer++ = (can_msg->frame_type << 20 | can_msg->dlc << 16);

  uint32_t w = 0;
  uint8_t b = 0;

  for (uint8_t i = 0; i < dlc_to_payload_size[can_msg->dlc & 0xF]; i++) {
    w |= can_msg->data[i] << (8 * b);
    b++;

    if (b == 4 || i == dlc_to_payload_size[can_msg->dlc & 0xF] - 1) {
      *tx_buffer++ = w;
      w = 0;
      b = 0;
    }
  }

  instance->TXBAR |= (1 << tx_put_index);
  while ((instance->TXBRP & (1 << tx_put_index)))
    ;

  return FDCAN_OK;
}

fdcan_rx_msg_t *receive_rx_frame(FDCAN_TypeDef *instance,
                                 fdcan_rx_msg_t *can_msg) {

  // handle to instance
  fdcan_handle_t *handle = fdcan_get_handle(instance);
  uint8_t rx_get_index = (uint8_t)((instance->RXF0S >> 8) & 0x3);

  // RAM pointer
  volatile uint32_t *rx =
      (volatile uint32_t *)(handle->rx_fifo0_base +
                            (rx_get_index * FDCAN_MSG_ELEMENT_SIZE));

  can_msg->id_type = ((rx[0] >> 30) == 0x1);
  if (can_msg->id_type == FDCAN_ID_EXTENDED) {
    can_msg->id = (uint32_t)rx[0] & ((1 << 29) - 1);
  } else {
    can_msg->id = (uint32_t)rx[0] >> 18 & ((1 << 11) - 1);
  }
  can_msg->rtr = (bool)(rx[0] >> 29 & 0x01);
  can_msg->dlc = (uint8_t)((rx[1] >> 16) & 0xF);
  can_msg->frame_type = ((rx[1] >> 20) & 0x3);
  can_msg->timestamp = timer_get_fdcan_timestamp((rx[1] & bitmask_u32(15)));

  volatile uint8_t *data =
      (volatile uint8_t *)rx +
      8; // casting to 8bit but actual 32bit + 2 => 8bit + 8

  for (int i = 0; i < dlc_to_payload_size[can_msg->dlc]; i++) {
    can_msg->data[i] = data[i];
  }

  instance->RXF0A = rx_get_index;
  rx_get_index = (uint8_t)((instance->RXF0S >> 8) & 0x3);
  return can_msg;
}
