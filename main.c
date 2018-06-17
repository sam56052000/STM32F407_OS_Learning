#include "main.h"

void OS_Main_Init(void)
{
	Init_Page_Map();
}

int main(void)
{
	OS_Main_Init();

	LED_ON(PIN_LED5);
	LED_ON(PIN_LED6);

	char *p1, *p2, *p3;

	p1 = (char *)(Get_Free_Pages(0, 3));
	p2 = (char *)(Get_Free_Pages(0, 4));

	printk("p1=%x\n", (uint32_t)p1);
	printk("p2=%x\n", (uint32_t)p2);

	Put_Free_Pages(p1, 3);
	Put_Free_Pages(p2, 4);

	p3 = (char *)(Get_Free_Pages(0, 3));
	printk("p3=%x\n", (uint32_t)p3);

	printk("Run Main Loop");
	
	while(1)
	{

	}
}