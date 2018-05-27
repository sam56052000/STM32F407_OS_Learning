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

    if( (Receive_Data > ' ') && (Receive_Data <= '~') )
    {
    	printk("Get Usart3 Data : %c\n", Receive_Data);
    }
    
    usart3_busy = 0;
}