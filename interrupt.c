#include "interrupt.h"

void Systick_Interrupt_Handler(void)
{
	int_flg++;

	if(usart3_busy == 1)
	{
		printk("Successful Interrupt Nesting");
	}

	LED_TOGGLE(PIN_LED4);
}

void USART3_IRQHandler(void)
{
    usart3_busy = 1;

    char Receive_Data = (char)(Usart_Receive_Data(USART3));

    usart_FIFO[usart_front++] = Receive_Data;
    
    usart3_busy = 0;
}

void UART_Queue_Init(void)
{
    usart_front = 0;
}