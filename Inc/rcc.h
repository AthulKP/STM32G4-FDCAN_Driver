/*
* rcc.h 
*  -> Created on: 21-Jan-2026
*  -> Author: Athul
*  -> © 2026 Athul.
*/

#ifndef RCC_H_
#define RCC_H_

#include <stdint.h>
#include "utils.h"

#define REG_RCC_CR			REG32(0X40021000U)
#define REG_RCC_APB1ENR1    REG32(0x40021058U)  // 0x58  offset   reset = 0x0000_0400
#define REG_RCC_CCIPR		REG32(0x40021088U)
#define REG_RCC_AHB2ENR     REG32(0x4002104CU)
#define REG_RCC_PLLCFGR     REG32(0x4002100CU)  // Reset value: 0x00001000
#define REG_RCC_CFGR        REG32(0x40021008U)  // Reset value: 0x00001000

#define  EN_GPIOA_CLOCK		(uint32_t)0x1<<0
#define  EN_CANFD_CLOCK     (uint32_t)(1<<25)
#define  EN_TIM3_CLOCK      (uint32_t)(1<<1)
#define  EN_TIM2_CLOCK      (uint32_t)(1<<0)
#define  EN_HSE_CLOCK       (uint32_t)(1<<16)
#define  HSE_READY          (uint32_t)(1<<17)
#define  HSI_CLOCK_EN       (uint32_t)(1<<8)
#define  HSI_CLOCK_READY    (uint32_t)(1<<10)
#define  PLLON_EN           (uint32_t)(1<<24)
#define  PLL_READY          (uint32_t)(1<<25)
#define  PLLM_4             (uint32_t)(3<<4)
#define  PLLN_85            (uint32_t)(85<<8)
#define  PLLR_2             (uint32_t)(2<<25)
#define  PLLR_EN            (uint32_t)(1<<24)
#define  PLLSRC_HSI         (uint32_t)(2<<0)
#define  SYSCLK_PLL_EN      (uint32_t)(3<<0)
#define  SYSCLK_PLL_READY   (uint32_t)(3<<2)
#define  FDCAN_CLOCK_PCLK   (uint32_t)(2<<24)

void initialize_clock_max(void);

#endif /* RCC_H_ */
