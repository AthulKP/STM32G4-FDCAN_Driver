
/*
*****************************************************
* @file   : main.c
* @brief  :
*  -> Created on : 22-Jan-2026
*  -> Author     : Athul
*  -> LICENSE    : Copyright (c) 2026, Athul
*                  All rights reserved.
*****************************************************
*/

#include "fdcan.h"
#include "fdcan_config.h"
#include "fdcan_hw.h"
#include "gpio.h"
#include "rcc.h"
#include "timer.h"
#include "utils.h"
#include <stdbool.h>

int main(void) {
  /* Loop forever */
  // Configuration
  // Clock configuration - Run at 170MHz APB1 bus clock speed 170MHz, FD CAN
  // controller clock speed in 170MHz
  initialize_clock_max();

  // Start the timer for can msg tracking
  init_timer_fdcan();
  
  // Configure GPIO pins for FDCAN
  init_gpio_fdcan();

  fdcan_config_t fdcan1_config = {.mode = FDCAN_MODE_LOOPBACK_INTERNAL,
                                  .queue_mode = false,
                                  .fd_transmission_reception = false,
                                  .nominal_timing = FDCAN_TIMING_500K};

  initialize_fdcan(FDCAN1,&fdcan1_config);
  initialize_fdcan(FDCAN2,&fdcan1_config);
  fdcan1_config.fd_transmission_reception = true;
  fdcan1_config.data_timing = FDCAN_DATA_TIMING_2M;
  initialize_fdcan(FDCAN3,&fdcan1_config); 

  fdcan_rx_msg_t rx_msg;
  fdcan_tx_msg_t tx_msg = {
      .id = 0x123456,
      .id_type = FDCAN_ID_EXTENDED,
      .dlc = 6,
      .frame_type = FDCAN_FRAME_CLASSIC,
      .data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 
        0xa, 0xb, 0xc, 0xd, 0xe, 0xf}};

  send_can_frame(FDCAN1,&tx_msg);
  receive_rx_frame(FDCAN1,&rx_msg);
  tx_msg.id = 0x123;
  tx_msg.id_type = FDCAN_ID_STANDARD;
  send_can_frame(FDCAN2,&tx_msg);
  receive_rx_frame(FDCAN2,&rx_msg);
  tx_msg.id = 0xABCD;
  tx_msg.id_type = FDCAN_ID_EXTENDED;
  tx_msg.frame_type = FDCAN_FRAME_FD_BRS;
  tx_msg.dlc = 15;
  send_can_frame(FDCAN3,&tx_msg);
  receive_rx_frame(FDCAN3,&rx_msg);
  tx_msg.id = 0xBEEF;
  tx_msg.id_type = FDCAN_ID_EXTENDED;
  send_can_frame(FDCAN1,&tx_msg);
  receive_rx_frame(FDCAN1,&rx_msg);
  send_can_frame(FDCAN1,&tx_msg);
  /* Loop forever */
  for (;;)
    ;
}
