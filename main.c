#include "main.h"

void OS_Main_Init(void)
{
	Init_Page_Map();
	Kmalloc_Init();
}

int main(void)
{
	OS_Main_Init();

	LED_ON(PIN_LED5);
	LED_ON(PIN_LED6);

	printk("Run Main Loop");
	
	while(1)
	{

	}
}