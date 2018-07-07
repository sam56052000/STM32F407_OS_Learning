#ifndef PERIPHERALS_H_
#define PERIPHERALS_H_

#include "../Platform/stm32f4_sys.h"
#include "../Platform/stm32f4_rcc.h"
#include "../Platform/stm32f4_gpio.h"
#include "../Platform/stm32f4_usart.h"

//
// LED Pin Assignment
// stm32f407 discovery spec p24
//
// PD12 : Green
// PD13 : Orange
// PD14 : Red
// PD15 : Blue
//
#define PIN_LED3		12
#define PIN_LED4		13
#define PIN_LED5		14
#define PIN_LED6		15
#define PIN_BUTTON		0

//
// UART Pin Assignment
// stm32f407 discovery spec p23
//
// PC10 : USART3_TX
// PC11 : USART3_RX
//
#define PIN_UART3_TX	10
#define PIN_UART3_RX	11

//
// Function
//
void UART_Puts(char *);

//
// Macro
//
#define LED_ON(LED_PIN)		Gpio_Set(GPIOD, LED_PIN)
#define LED_OFF(LED_PIN)	Gpio_Reset(GPIOD, LED_PIN)
#define LED_TOGGLE(LED_PIN)	Gpio_Toggle(GPIOD, LED_PIN)

#define BUTTON_ACTIVE		Gpio_Read(GPIOA, PIN_BUTTON)

#endif