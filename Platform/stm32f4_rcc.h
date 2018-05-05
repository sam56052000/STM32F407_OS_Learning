#ifndef STM32F4_RCC_H_
#define STM32F4_RCC_H_

#include "stm32f4_sys.h"

//
// stm32f407 rcc periphreal address
// stm32f407 mcu spec p65
//
#define RCC_REG							0x40023800

//
// stm32f407 rcc registers
// stm32f407 mcu spec p224
//
#define RCC_REG_CR_OFFSET				0x00
#define RCC_REG_PLLCFGR_OFFSET			0x04
#define RCC_REG_CFGR_OFFSET				0x08
#define RCC_REG_CIR_OFFSET				0x0C
#define RCC_REG_AHB1_EN_OFFSET			0x30
#define RCC_REG_APB1_EN_OFFSET			0x40

//
// stm32f407 rcc AHB1 enable registers
// stm32f407 mcu spec p242
//
#define RCC_REG_AHB1_GPIOC				(0x1<<2)
#define RCC_REG_AHB1_GPIOD				(0x1<<3)

//
// stm32f407 rcc APB1 enable registers
// stm32f407 mcu spec p245
//
#define RCC_REG_APB1_USART3				(0x1<<18)
#define RCC_REG_APB1_PWREN				(0x1<<28)

//
// stm32f407 CR Register
// stm32f407 mcu spec p224
//
#define RCC_REG_CR_HSION				0x00000001
#define RCC_REG_CR_HSIRDY				0x00000002
#define RCC_REG_CR_HSEON				0x00010000
#define RCC_REG_CR_HSERDY				0x00020000
#define RCC_REG_CR_HSEBYP				0x00040000
#define RCC_REG_CR_CSSON				0x00080000
#define RCC_REG_CR_PLLON				0x01000000
#define RCC_REG_CR_PLLRDY				0x02000000

//
// stm32f407 PLLCFGR Register
// stm32f407 mcu spec p226
//
#define RCC_REG_PLLCFGR_DEFAULT			0x24003010
#define RCC_REG_PLLCFGR_SRC_HSE			0x00400000
#define RCC_REG_PLLCFGR_SRC_HSI			0x00000000

//
// stm32f407 CFGR Register
// stm32f407 mcu spec p228
//
#define RCC_REG_CFGR_HPRE_DIV1			0x00000000
#define RCC_REG_CFGR_HPRE_DIV2			0x00000080
#define RCC_REG_CFGR_HPRE_DIV4			0x00000090
#define RCC_REG_CFGR_HPRE_DIV8			0x000000A0
#define RCC_REG_CFGR_HPRE_DIV16			0x000000B0
#define RCC_REG_CFGR_HPRE_DIV64			0x000000C0
#define RCC_REG_CFGR_HPRE_DIV128		0x000000D0
#define RCC_REG_CFGR_HPRE_DIV256		0x000000E0
#define RCC_REG_CFGR_HPRE_DIV512		0x000000F0

#define RCC_REG_CFGR_PPRE1_DIV1			0x00000000
#define RCC_REG_CFGR_PPRE1_DIV2			0x00001000
#define RCC_REG_CFGR_PPRE1_DIV4			0x00001400
#define RCC_REG_CFGR_PPRE1_DIV8			0x00001800
#define RCC_REG_CFGR_PPRE1_DIV16		0x00001C00

#define RCC_REG_CFGR_PPRE2_DIV1			0x00000000
#define RCC_REG_CFGR_PPRE2_DIV2			0x00008000
#define RCC_REG_CFGR_PPRE2_DIV4			0x0000A000
#define RCC_REG_CFGR_PPRE2_DIV8			0x0000C000
#define RCC_REG_CFGR_PPRE2_DIV16		0x0000E000

#define RCC_REG_CFGR_SW					0x00000003
#define RCC_REG_CFGR_SW_HSI				0x00000000
#define RCC_REG_CFGR_SW_HSE				0x00000001
#define RCC_REG_CFGR_SW_PLL				0x00000002

#define RCC_REG_CFGR_SWS				0x0000000C
#define RCC_REG_CFGR_SWS_HSI			0x00000000
#define RCC_REG_CFGR_SWS_HSE			0x00000004
#define RCC_REG_CFGR_SWS_PLL			0x00000008


//
// Function
//
void Enable_RCC_AHB1(uint32_t module);
void Enable_RCC_APB1(uint32_t module);
void RCC_Set_Default(void);
void RCC_Set_System_Clock(void);

#endif