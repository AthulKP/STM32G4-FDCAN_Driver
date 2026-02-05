/*
* fdcan_hw.h 
*  -> Created on: 21-Jan-2026
*  -> Author: Athul
*  -> © 2026 Athul.
*/
#ifndef FDCAN_HW_H_
#define FDCAN_HW_H_

#include <stdint.h>
#include "utils.h"

// Base addresses for FDCAN peripherals
#define FDCAN1_BASE   0x40006400UL
#define FDCAN2_BASE   0x40006800UL  
#define FDCAN3_BASE   0x40006C00UL  

// These are POINTERS to the struct at those addresses
#define FDCAN1  ((FDCAN_TypeDef *) FDCAN1_BASE)
#define FDCAN2  ((FDCAN_TypeDef *) FDCAN2_BASE)
#define FDCAN3  ((FDCAN_TypeDef *) FDCAN3_BASE)


#define RAM_FDCAN_BASE		0x4000A400
#define RAM_FDCAN_INSTANCE_END		0x34C

#define RAM_FDCAN1_START		RAM_FDCAN_BASE 
#define RAM_FDCAN2_START		RAM_FDCAN1_START + RAM_FDCAN_INSTANCE_END + 0x4
#define RAM_FDCAN3_START		RAM_FDCAN2_START + RAM_FDCAN_INSTANCE_END + 0x4

// Section offsets (in bytes)
#define FDCAN_RAM_STD_FILTERS_OFFSET    0x000
#define FDCAN_RAM_EXT_FILTERS_OFFSET    0x070
#define FDCAN_RAM_RX_FIFO0_OFFSET       0x0B0
#define FDCAN_RAM_RX_FIFO1_OFFSET       0x188
#define FDCAN_RAM_TX_EVENT_OFFSET       0x260
#define FDCAN_RAM_TX_BUFFERS_OFFSET     0x278

// Element sizes (in bytes)
#define FDCAN_MSG_ELEMENT_SIZE      72  // 18 words
#define FDCAN_STD_FILTER_SIZE       4   // 1 word
#define FDCAN_EXT_FILTER_SIZE       8   // 2 words
#define FDCAN_TX_EVENT_SIZE         8   // 2 words

#define  REG_FDCAN_1_CCCR   REG32(0x40006418U)  // 0x18  offset
#define  REG_FDCAN_1_DBTP   REG32(0x4000640CU)  // 0x0C  offset
#define  REG_FDCAN_1_NBTP   REG32(0x4000641CU)  // 0x1C  offset
#define  REG_FDCAN_1_TEST   REG32(0x40006410U)  // 0x10  offset
#define  REG_FDCAN_1_TXBC   REG32(0x400064C0U)  // 0xC0  offset
#define  REG_FDCAN_1_TXFQS  REG32(0x400064C4U)  // 0xC4  offset
#define  REG_FDCAN_1_TXBRP  REG32(0x400064C8U)  // 0xC8  offset
#define  REG_FDCAN_1_TXBAR  REG32(0x400064CCU)  // 0xCC  offset
#define  REG_FDCAN_1_TXBTO  REG32(0x400064D4U)  // 0xD4  offset
#define  REG_FDCAN_1_RXF0S  REG32(0x40006490U)  // 0x90  offset
#define  REG_FDCAN_1_RXF0A  REG32(0x40006494U)  // 0x94  offset

#define  RAM_FDCAN_1_TXFIFO_1_TBSA 	(uint32_t)0x4000AE78U   // 0x279  offset
#define  RAM_FDCAN_1_RXFIFO_0    	(uint32_t)0x4000AC00U   // 0xB0  offset

#define  EN_CANFD_TX_QUEUE   (uint32_t)(1<<24)
#define  EN_CANFD_TX_FIFO    ~(uint32_t)(1<<24)

#define  FDCAN_FDO_EN					(uint32_t)1<<8
#define  FDCAN_TX_1	 					(uint32_t)1<<0
#define  FDCAN_TX_2	 					(uint32_t)1<<1
#define  FDCAN_TX_3	 					(uint32_t)1<<2
#define  FDCAN_CONFIGURATION_CHANGE_EN	(uint32_t)1<<1
#define  FDCAN_BUS_MONITORING_MODE_EN	(uint32_t)1<<5
#define  FDCAN_TEST_MODE_EN				(uint32_t)1<<7
#define  FDCAN_LOOPBACK_MODE_EN			(uint32_t)1<<4
#define  FDCAN_INITIALIZATION_EN		(uint32_t)1<<0
#define  TIMESTAMP_TIM3                 (uint32_t)0x2

// In stm32g4xx.h (STM32 HAL header)

// FDCAN_TypeDef is a STRUCT, not an enum
typedef struct
{
__IO uint32_t CREL ;			// Offset = 0x0
__IO uint32_t ENDN ;			// Offset = 0x4
__I uint32_t RES_0;			// Offset = 0x8
__IO uint32_t DBTP ;			// Offset = 0xC
__IO uint32_t TEST ;			// Offset = 0x10
__IO uint32_t RWD  ;			// Offset = 0x14
__IO uint32_t CCCR ;			// Offset = 0x18
__IO uint32_t NBTP ;			// Offset = 0x1C
__IO uint32_t TSCC ;			// Offset = 0x20
__IO uint32_t TSCV ;			// Offset = 0x24
__IO uint32_t TOCC ;			// Offset = 0x28
__IO uint32_t TOCV ;			// Offset = 0x2C
__I uint32_t RES_1;			// Offset = 0x30
__I uint32_t RES_2;			// Offset = 0x34
__I uint32_t RES_3;			// Offset = 0x38
__I uint32_t RES_4;			// Offset = 0x3C
__IO uint32_t ECR  ;			// Offset = 0x40
__IO uint32_t PSR  ;			// Offset = 0x44
__IO uint32_t TDCR ;			// Offset = 0x48
__I uint32_t RES_5;			// Offset = 0x4C
__IO uint32_t IR   ;			// Offset = 0x50
__IO uint32_t IE   ;			// Offset = 0x54
__IO uint32_t ILS  ;			// Offset = 0x58
__IO uint32_t ILE  ;			// Offset = 0x5C
__I uint32_t RES_6;			// Offset = 0x60
__I uint32_t RES_7;			// Offset = 0x64
__I uint32_t RES_8;			// Offset = 0x68
__I uint32_t RES_9;			// Offset = 0x6C
__I uint32_t RES_10;			// Offset = 0x70
__I uint32_t RES_11;			// Offset = 0x74
__I uint32_t RES_12;			// Offset = 0x78
__I uint32_t RES_13;			// Offset = 0x7C
__IO uint32_t RXGFC;			// Offset = 0x80
__IO uint32_t XIDAM;			// Offset = 0x84
__IO uint32_t HPMS ;			// Offset = 0x88
__I uint32_t RES_14;			// Offset = 0x8C
__IO uint32_t RXF0S;			// Offset = 0x90
__IO uint32_t RXF0A;			// Offset = 0x94
__IO uint32_t RXF1S;			// Offset = 0x98
__IO uint32_t RXF1A;			// Offset = 0x9C
__I uint32_t RES_15;			// Offset = 0xA0
__I uint32_t RES_16;			// Offset = 0xA4
__I uint32_t RES_17;			// Offset = 0xA8
__I uint32_t RES_18;			// Offset = 0xAC
__I uint32_t RES_19;			// Offset = 0xB0
__I uint32_t RES_20;			// Offset = 0xB4
__I uint32_t RES_21;			// Offset = 0xB8
__I uint32_t RES_22;			// Offset = 0xBC
__IO uint32_t TXBC ;			// Offset = 0xC0
__IO uint32_t TXFQS;			// Offset = 0xC4
__IO uint32_t TXBRP;			// Offset = 0xC8
__IO uint32_t TXBAR;			// Offset = 0xCC
__IO uint32_t TXBCR;			// Offset = 0xD0
__IO uint32_t TXBTO;			// Offset = 0xD4
__IO uint32_t TXBCF;			// Offset = 0xD8
__IO uint32_t TXBTIE;			// Offset = 0xDC
__IO uint32_t TXBCIE;			// Offset = 0xE0
__IO uint32_t TXEFS;			// Offset = 0xE4
__IO uint32_t TXEFA;			// Offset = 0xE8
__I uint32_t RES_23;			// Offset = 0xEC
__I uint32_t RES_24;			// Offset = 0xF0
__I uint32_t RES_25;			// Offset = 0xF4
__I uint32_t RES_26;			// Offset = 0xF8
__I uint32_t RES_27;			// Offset = 0xFC
__IO uint32_t CKDIV;			// Offset = 0x100
} FDCAN_TypeDef;

#endif // FDCAN_HW_H_
