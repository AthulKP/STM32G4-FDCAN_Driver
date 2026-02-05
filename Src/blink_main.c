/*
*****************************************************
* @file   : blink_main.c
* @brief  :
*  -> Created on : 01-Feb-2026
*  -> Author     : Athul
*  -> LICENSE    : Copyright (c) 2026, Athul
*                  All rights reserved.
*****************************************************
*/


#include <stdint.h>

int main(void) {
  uint32_t *p_gpioc_moder = (uint32_t *)0x48000800;
  uint32_t *p_gpioc_odr   = (uint32_t *)0x48000814;
  uint32_t *p_rcc_AHB2ENR = (uint32_t *)0X4002104C;
  
  *p_rcc_AHB2ENR |= (1 << 2); // GPIO C clock enable
  *p_gpioc_moder &= ~(3 << 12);     // GPIO C clear mode
  *p_gpioc_moder |= (1 << 12);      // GPIO C set to output
  
  /* Loop forever */
  for (;;) {
    *p_gpioc_odr ^= (1 << 6); // Toggle the output 
    for (uint32_t i = 0; i < 500000; i++) // with a delay
      ;
  }
}
