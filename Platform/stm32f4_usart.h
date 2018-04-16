#ifndef STM32F4_USART_
#define STM32F4_USART_

#include "stm32f4_sys.h"

//
// stm32f407 usart periphreal address
// stm32f407 mcu spec p67
//
#define USART3		0x40004800

//
// stm32f407 usart registers
// stm32f407 mcu spec p1010
//
#define USART_REG_STATUS_OFFSET		0x00
#define USART_REG_DATA				0x04
#define USART_REG_BUADRATE_OFFSET	0x08
#define USART_REG_CTRL_1_OFFSET		0x0C
#define USART_REG_CTRL_2_OFFSET		0x10
#define USART_REG_CTRL_3_OFFSET		0x14
#define USART_REG_GT_PR_OFFSET		0x18

//
// usart init structure
//
typedef struct USART_Init_Register_t
{
	
	
} USART_Init_Register_t;

#endif