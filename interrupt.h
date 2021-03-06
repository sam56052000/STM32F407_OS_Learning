#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include "./Boot/peripherals.h"
#include "./Boot/print.h"
#include "driver.h"
#include "proc.h"

uint32_t int_flg;
uint32_t usart3_busy;

//
//	Function
//
void Systick_Interrupt_Handler(void);
void USART3_IRQHandler(void);
void UART_Queue_Init(void);

void Syscall_Interrupt_Handler(void);
uint32_t *Task_Handler(void);

#endif