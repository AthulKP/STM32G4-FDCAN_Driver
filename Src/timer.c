/*
*****************************************************
* @file   : timer.c
* @brief  :
*  -> Created on : 01-Feb-2026
*  -> Author     : Athul
*  -> LICENSE    : Copyright (c) 2026, Athul
*                  All rights reserved.
*****************************************************
*/
#include "timer.h"
#include "utils.h"
#include <stdint.h>

/**
 * @brief This funtion initialize the timer3 for micro second capture and tim2
 * for tracking the tim3 over fow creating a 48bit timer 2^48
 * = 28.14.74.97.67.10.656 us = 8.9 years
 *
 * @return timer_status_t
 */
timer_status_t init_timer_fdcan(void) {
  volatile TIMER_TypeDef *p_timer3 = ((TIMER_TypeDef *)TIM3_BASE_ADRESSS);
  volatile TIMER_TypeDef *p_timer2 = ((TIMER_TypeDef *)TIM2_BASE_ADRESSS);
  // Reset
  p_timer3->CR1 = 0x0;
  p_timer2->CR1 = 0x0;

  // set prescale to 170 - 1 for tim3 and 0 for tim2
  p_timer3->PSC &= 0x0;
  p_timer3->PSC |= (uint16_t)169;
  p_timer2->PSC = 0;

  // Set maximum auto relaod value for both the timer
  p_timer3->ARR = (uint16_t)bitmask_u32(16);
  p_timer2->ARR = (uint32_t)bitmask_u32(32);

  // Set the TIM2 as master creating trigger on overflow
  p_timer3->CR2 = (uint32_t)0x0;    // reset value
  p_timer3->CR2 |= MMS_UPDATE_MODE; // reset value
  p_timer3->EGR = (uint32_t)0x0;
  p_timer3->EGR |= UPDATE_GENERATION_EN;
  p_timer3->SR &= ~UPDATE_GENERATION_EN;

  // Set the source for tim2 trigger as tim3 overflows
  p_timer2->SMCR = (uint32_t)0x0;
  p_timer2->SMCR |= TS_TIM_ITR2;
  p_timer2->SMCR |= SMS_EXTERNAL_CLOCK_MODE_1;

  // TODO : Implement TIM2 overflow trigger
  p_timer2->CR1 |= (uint32_t)0x1;
  p_timer3->CR1 |= (uint32_t)0x1; // enable
  return TIMER_RUNNING;
}

/**
 * @brief Atomic read of TIM2->CNT value
 *
 * @return uint32_t
 */
uint64_t timer_get_fdcan_timestamp(uint16_t rx_timestamp) {
  volatile TIMER_TypeDef *p_timer3 = ((TIMER_TypeDef *)TIM3_BASE_ADRESSS);
  volatile TIMER_TypeDef *p_timer2 = ((TIMER_TypeDef *)TIM2_BASE_ADRESSS);
  uint32_t tim2_value = p_timer2->CNT;
  if (rx_timestamp > p_timer3->CNT) {
    tim2_value--;
  }
  return (uint64_t)(tim2_value << 16 | rx_timestamp);
}
