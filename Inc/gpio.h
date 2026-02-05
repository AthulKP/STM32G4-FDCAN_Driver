/*
 * gpio.h
 *  -> Created on: 05-Jan-2026
 *  -> Author: athul
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <stdint.h>
#include "utils.h"

#define  REG_GPIOA_MODER	REG32(0x48000000U)
#define  REG_GPIOA_OTYPER 	REG32(0x48000004U)
#define  REG_GPIOA_PUPDR 	REG32(0x4800000CU)
#define  REG_GPIOA_AFRH 	REG32(0x48000024U)

#define  AFMODE_9		    0x9
#define  OUTPUT_MODE		0x2
#define  NO_PUSH_PULL       0x3
#define  PIN_11             11
#define  PIN_12             12
#define  BIT_Pos_2          0x2
#define  BIT_Pos_4          0x4
#define  AFRL_PIN_COUNT     0x8

void init_gpio_fdcan(void);

#endif
