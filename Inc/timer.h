/*
*****************************************************
* @file   : timer.h
* @brief  :
*  -> Created on : 29-Jan-2026
*  -> Author     : Athul
*  -> LICENSE    : Copyright (c) 2026, Athul
*                  All rights reserved.
*****************************************************
*/

#ifndef TIMER_H_
#define TIMER_H_

#include "gpio.h"
#include "utils.h"

#define TIM3_BASE_ADRESSS 0x40000400
#define TIM2_BASE_ADRESSS 0x40000000

#define MMS_UPDATE_MODE             (uint32_t)0x2<<4
#define UPDATE_GENERATION_EN        (uint32_t)0x1<<0
#define TS_TIM_ITR2                 (uint32_t)0x2<<4
#define SMS_EXTERNAL_CLOCK_MODE_1   (uint32_t)0x7<<0

// #define TIMER_INIT_MAGIC  0xCAFE850A

typedef struct __attribute__((__packed__)) {
  __IO uint32_t CR1;             // Offset = 0x0
  __IO uint32_t CR2;             // Offset = 0x4
  __IO uint32_t SMCR;            // Offset = 0x8
  __IO uint32_t DIER;            // Offset = 0xC
  __IO uint32_t SR;              // Offset = 0x10
  __IO uint32_t EGR;             // Offset = 0x14
  __IO uint32_t CCMR1;           // Offset = 0x18
  // __IO uint32_t CCMR1_Alternate; // Offset = 0x18
  __IO uint32_t CCMR2;           // Offset = 0x1C
  // __IO uint32_t CCMR2_Alternate; // Offset = 0x1C
  __IO uint32_t CCER;            // Offset = 0x20
  __IO uint32_t CNT;             // Offset = 0x24
  __IO uint32_t PSC;             // Offset = 0x28
  __IO uint32_t ARR;             // Offset = 0x2C
  __IO uint32_t CCR1;            // Offset = 0x34
  __IO uint32_t CCR2;            // Offset = 0x38
  __IO uint32_t CCR3;            // Offset = 0x3C
  __IO uint32_t CCR4;            // Offset = 0x40
  __I uint32_t RES_0[3];         // Offset = 0x48
  __IO uint32_t ECR;             // Offset = 0x58
  __IO uint32_t TISEL;           // Offset = 0x5C
  __IO uint32_t AF1;             // Offset = 0x60
  __IO uint32_t AF2;             // Offset = 0x64
  __I uint32_t RES_1[220];       // Offset = 0x68
  __IO uint32_t DCR;             // Offset = 0x3DC
  __IO uint32_t DMAR;            // Offset = 0x3E0
} TIMER_TypeDef;

// typedef struct {
//   uint32_t magic;          // Unique Hardware instance 
//   TIMER_TypeDef *instance; // Hardware instance 
// } timer_handle_t;

// static timer_handle_t g_timer3_handle = {0};
// #define TIMER3  ((TIMER_TypeDef *) TIM3_BASE_ADRESSS)

// Return status
typedef enum {
  TIMER_RUNNING = 0,
  TIMER_ERROR,
  TIMER_INVALID_PARAM,
  TIMER_NOT_INITIALIZED,
  TIMER_ALREADY_INITIALIZED
} timer_status_t;

timer_status_t init_timer_fdcan(void);
uint64_t timer_get_fdcan_timestamp(uint16_t rx_timestamp);

#endif // TIMER_H_
