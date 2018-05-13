#include "peripherals.h"

void UART_Puts(char *s)
{
	while(*s)
	{
        Usart_Send_Data(USART3, *s);
        s++;
    }
}

