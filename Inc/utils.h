/*
 * gpio.h
 *  -> Created on: 05-Jan-2026
 *  -> Author: athul
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <stdint.h>

#ifdef __cplusplus
  #define   __I     volatile             /*!< Defines 'read only' permissions */
#else
  #define   __I     volatile const       /*!< Defines 'read only' permissions */
#endif
#define     __O     volatile             /*!< Defines 'write only' permissions */
#define     __IO    volatile


#define REG32(address) (*(volatile uint32_t *)address)

static inline uint32_t bitmask_u32(int bit_position) {
  return bit_position >= 32 ? ~0ULL : (1ULL << bit_position) - 1ULL;
}

static inline void reset_bits_u32(volatile uint32_t *reg, uint8_t pos,
                              uint8_t width) {
  *reg &= ~(((1U << width) - 1) << pos);
}

#endif
