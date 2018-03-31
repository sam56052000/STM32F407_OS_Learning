#ifndef STM32F4_RCC_H_
#define STM32F4_RCC_H_

#include "stm32f4_sys.h"

//
// stm32f407 rcc periphreal address
// stm32f407 mcu spec p65
//
#define RCC_REG	0x40023800

//
// stm32f407 rcc registers
// stm32f407 mcu spec p224
//
#define RCC_REG_AHB1_EN_OFFSET	0x30
#define RCC_REG_APB1_EN_OFFSET	0x40

//
// stm32f407 rcc AHB1 enable registers
// stm32f407 mcu spec p242
//
#define RCC_REG_AHB1_GPIOC		(0x1<<2)
#define RCC_REG_AHB1_GPIOD		(0x1<<3)

//
// stm32f407 rcc APB1 enable registers
// stm32f407 mcu spec p245
//
#define RCC_REG_APB1_USART3		(0x1<<18)


//
// Function
//
void Enable_RCC_AHB1(uint32_t module);

#endif