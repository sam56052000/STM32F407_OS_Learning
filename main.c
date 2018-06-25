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

	Flash_Unlock();
	volatile uint32_t *SR_Reg = (uint32_t *)(FLASH_REG_SR);
	printk("status=%x\n", *SR_Reg);

	Flash_Erase_Sector(0x0028);

	printk("status=%x\n", *SR_Reg);
	Flash_Clear_Flag(FLASH_SR_PGSERR | FLASH_SR_PGPERR | FLASH_SR_PGAERR
		 | FLASH_SR_WRPERR | FLASH_SR_OPERR | FLASH_SR_EOP);

	printk("status=%x\n", *SR_Reg);
	
	Flash_Write_Data(DISK1_ADDRESS, 0x12345678);

	printk("status=%x\n", *SR_Reg);

	printk("%x\n", *(volatile uint32_t *)DISK1_ADDRESS);

	// int32_t i;
	// int32_t *p = (int32_t *)0x08020000;

	// for(i = 0; i < 100; i++)
	// {
	// 	*(p++) = i;
	// }

	// for(i = 0; i < 100; i++)
	// {
	// 	printk("add=%x, dat=%d\n", p, *p);
	// 	p++;
	// }
	
	printk("Start Run Main Loop");

	while(1)
	{

	}
}