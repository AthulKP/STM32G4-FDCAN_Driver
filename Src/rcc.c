/*
 * rcc.c
 *  -> Created on: 05-Jan-2026
 *  -> Author: athul
 */

#include "Inc/rcc.h"

/**
 * @brief By utilizing the PLL running all peripherals at max frequency AHB and AHP at 170MHz
 * 
 */
 __attribute__((used))
void initialize_clock_max(void) {
	// use HSI clock to set PLL for max clock 170MHz
	REG_RCC_CR |= HSI_CLOCK_EN;

	// Clear the PLL clock enable bits
	REG_RCC_CR &= ~PLLON_EN;
	while((REG_RCC_CR & PLL_READY));

	//Set PLLM = /4 , PLLN = x85 , PLLR=/2  outputs 170MHz
	REG_RCC_PLLCFGR = ( PLLR_2 | PLLN_85 | PLLM_4 | PLLSRC_HSI);
	
	// Turn ON the PLL clock
	REG_RCC_CR |= PLLON_EN;
	while(!(REG_RCC_CR & PLL_READY));
	REG_RCC_PLLCFGR |= PLLR_EN;

	// Select system clock as PLL clock
	REG_RCC_CFGR |= SYSCLK_PLL_EN;
	while((REG_RCC_CFGR & SYSCLK_PLL_READY) != SYSCLK_PLL_READY);

	// Turn ON GPIO port A clock
	REG_RCC_AHB2ENR |= EN_GPIOA_CLOCK;

	//Turn ON CANFD clock
	REG_RCC_APB1ENR1 |= EN_CANFD_CLOCK;
	
	// Turn On clock for TIMER 3
	REG_RCC_APB1ENR1 |= EN_TIM3_CLOCK;
	
	// Turn On clock for TIMER 2
	REG_RCC_APB1ENR1 |= EN_TIM2_CLOCK;

	// Select HSE clock for CAN FD kernel
	REG_RCC_CR |= EN_HSE_CLOCK;
	while(!(REG_RCC_CR & HSE_READY));
	REG_RCC_CCIPR |= FDCAN_CLOCK_PCLK;
}
