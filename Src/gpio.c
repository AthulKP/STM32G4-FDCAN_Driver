/*
 * gpio.h
 *  -> Created on: 05-Jan-2026
 *  -> Author: athul
 */

#include "gpio.h"
#include "utils.h"
#include <stdint.h>

/**
 * @brief GPIO Pins : PA11 - RX and PA12 - TX
 *        GPIO Mode : AF output
 *        GPIO AF : AF9
 *        GPIO Push/Pull : Floating (No push pull)
 *
 */
void init_gpio_fdcan(void) {

  //TODO : Configure with instances 
  // GPIOA Mode  : AF mode for PA11 and PA12
  REG_GPIOA_MODER &= (uint32_t)~(bitmask_u32(BIT_Pos_2) << (PIN_11 * BIT_Pos_2)) &
                     ~(bitmask_u32(BIT_Pos_2) << (PIN_12 * BIT_Pos_2));
  REG_GPIOA_MODER |= (uint32_t)((OUTPUT_MODE << PIN_11 * BIT_Pos_2) |
                                (OUTPUT_MODE << PIN_12 * BIT_Pos_2));

  // GPIOA PUPDR : both pins no pull and push
  REG_GPIOA_PUPDR &= (uint32_t)~((NO_PUSH_PULL << PIN_11 * BIT_Pos_2) |
                                 (NO_PUSH_PULL << PIN_12 * BIT_Pos_2));

  // GPIOA AF Mode : Both pins as AF9
  REG_GPIOA_AFRH |=
      (uint32_t)((AFMODE_9 << (PIN_11 - AFRL_PIN_COUNT) * BIT_Pos_4) |
                 (AFMODE_9 << (PIN_12 - AFRL_PIN_COUNT) * BIT_Pos_4));
}
