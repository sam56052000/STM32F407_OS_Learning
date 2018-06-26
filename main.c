#include "main.h"

void OS_Main_Init(void)
{
	Init_Page_Map();
	Kmalloc_Init();
	Disk_Init();
}

int main(void)
{
	OS_Main_Init();

	LED_ON(PIN_LED5);
	LED_ON(PIN_LED6);

	uint32_t a[10], b[10], i;

	
	a[0] = 0x12345678;
	a[1] = 0x22345678;
	a[2] = 0x32345678;
	a[3] = 0x42345678;
	a[4] = 0x52345678;
	a[5] = 0x62345678;
	a[6] = 0x72345678;
	a[7] = 0x82345678;
	a[8] = 0x92345678;
	a[9] = 0xA2345678;

	disk1.Din(&disk1, a, 0, sizeof(a));
	disk1.Dout(&disk1, b, 0, sizeof(b));

	for(i = 0; i < 10; i++)
	{
		printk("b[0]=%x\n", b[i]);
	}
	
	printk("Start Run Main Loop");

	while(1)
	{

	}
}