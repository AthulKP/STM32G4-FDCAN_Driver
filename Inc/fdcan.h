/*
* fdcan.h 
*  -> Created on: 21-Jan-2026
*  -> Author: Athul
*  -> © 2026 Athul.
*/


#ifndef FDCAN_H_
#define FDCAN_H_

#include "fdcan_config.h"
#include "fdcan_hw.h"
#include <stdint.h>
#include <string.h>


//TODO: static uint32_t g_init_timestamp = 0;

#define FDCAN_INIT_MAGIC 0xFDCA1234

static fdcan_handle_t g_fdcan1_handle = {0};
#ifdef FDCAN2
static fdcan_handle_t g_fdcan2_handle = {0};
#endif
#ifdef FDCAN3
static fdcan_handle_t g_fdcan3_handle = {0};
#endif

static inline fdcan_handle_t* fdcan_get_handle(FDCAN_TypeDef *instance) {
    if (instance == FDCAN1) return &g_fdcan1_handle;
#ifdef FDCAN2
    if (instance == FDCAN2) return &g_fdcan2_handle;
#endif
#ifdef FDCAN3
    if (instance == FDCAN3) return &g_fdcan3_handle;
#endif
    return NULL;
}

static inline uint32_t fdcan_get_ram_base(FDCAN_TypeDef *instance) {
    if (instance == FDCAN1) return RAM_FDCAN1_START;
#ifdef FDCAN2
    if (instance == FDCAN2) return RAM_FDCAN2_START;
#endif
#ifdef FDCAN3
    if (instance == FDCAN3) return RAM_FDCAN3_START;
#endif
    return (uint32_t)0x0;
}

fdcan_status_t initialize_fdcan(FDCAN_TypeDef *instance, const fdcan_config_t *config);
fdcan_status_t send_can_frame(FDCAN_TypeDef *instance, fdcan_tx_msg_t* can_msg);
fdcan_rx_msg_t* receive_rx_frame(FDCAN_TypeDef *instance, fdcan_rx_msg_t* can_msg);

#endif
