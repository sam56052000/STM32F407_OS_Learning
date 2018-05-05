#include "main.h"

void Init_Peripherals(void)
{
	System_Init();
	LED_Init();
	UART_Init();
}

int main(void)
{
	Init_Peripherals();

	//LED_ON(PIN_LED3);
	//LED_ON(PIN_LED4);
	//LED_ON(PIN_LED5);
	//LED_ON(PIN_LED6);

	while(1)
	{
		Usart_Send_Data(USART3,'a');
	}
}