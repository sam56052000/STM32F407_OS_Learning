#ifndef BOOT_H_
#define BOOT_H_

#include "peripherals.h"
#include "print.h"

//
// Function
//
void STM32f407_Init_Clock(void);
void STM32f407_Init_LED(void);
void STM32f407_Init_UART(void);
void Print_Boot_Data(void);
void Platform_Boot(void);

//
// Init Function Pointer Structure
//
typedef void (*init_func_t)(void);

static init_func_t init_func[] =
{
	STM32f407_Init_Clock,
	STM32f407_Init_LED,
	STM32f407_Init_UART,
	Print_Boot_Data,
	Test_Printk,
	0
};

#endif