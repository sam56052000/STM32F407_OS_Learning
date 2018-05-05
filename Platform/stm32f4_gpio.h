#ifndef STM32F4_GPIO_H_
#define STM32F4_GPIO_H_

#include "stm32f4_sys.h"

//
// stm32f407 gpio periphreal address
// stm32f407 mcu spec p65
//
#define GPIOA	0x40020000
#define GPIOB	0x40020400
#define GPIOC	0x40020800
#define GPIOD	0x40020C00

//
// stm32f407 gpio registers
// stm32f407 mcu spec p281
//
#define GPIO_REG_MODE_OFFSET		0x00
#define GPIO_REG_OTYPE_OFFSET		0x04
#define GPIO_REG_SPEED_OFFSET		0x08
#define GPIO_REG_PUPD_OFFSET		0x0C
#define GPIO_REG_IN_DATA_OFFSET		0x10
#define GPIO_REG_OUT_DATA_OFFSET	0x14
#define GPIO_REG_CTRL_OFFSET		0x18
#define GPIO_REG_LCKR_OFFEST		0x1C
#define GPIO_REG_AFRL_OFFSET		0x20
#define GPIO_REG_AFRH_OFFSET		0x24

//
// Gpio Mode Register
// stm32f407 mcu spec p281
//
#define GPIO_REG_MODE_INPUT			0x00
#define GPIO_REG_MODE_OUTPUT		0x01
#define GPIO_REG_MODE_FUNCTION		0x02
#define GPIO_REG_MODE_ANALOG		0x03

//
// Gpio Otype Register
// stm32f407 mcu spec p281
//
#define GPIO_REG_OTYPE_PP			0x00
#define GPIO_REG_OTYPE_OD			0x01

//
// Gpio Speed Register
// stm32f407 mcu spec p282
//
#define GPIO_REG_SPEED_LOW			0x00
#define GPIO_REG_SPEED_MEDIUM		0x01
#define GPIO_REG_SPEED_FAST 		0x02
#define GPIO_REG_SPEED_HIGH			0x03

//
// Gpio PuPd Register
// stm32f407 mcu spec p282
//
#define GPIO_REG_PUPD_NONE			0x00
#define GPIO_REG_PUPD_PULLUP		0x01
#define GPIO_REG_PUPD_PULLDOWN		0x02

//
// Gpio Alternate Function Setting
// stm32f407 mcu spec p272
//
#define GPIO_AF_USART3				0x07

//
// Function
//
void Gpio_Init(uint32_t group, uint32_t pin, uint32_t mode, uint32_t otype
	, uint32_t speed, uint32_t pupd);
void Gpio_Alternate_Function_Setting(uint32_t group, uint32_t pin
	, uint16_t function);
void Gpio_Set(uint32_t group, uint32_t pin);
void Gpio_Reset(uint32_t group, uint32_t pin);
void Gpio_Toggle(uint32_t group, uint32_t pin);
uint32_t Gpio_Read(uint32_t group, uint32_t pin);

#endif