#include "main.h"

void OS_Main_Init(void)
{
	Init_Page_Map();
	Kmalloc_Init();
	Disk_Init();
	FileSystem_Init();

	UART_Queue_Init();
}

int main(void)
{
	OS_Main_Init();

	LED_ON(PIN_LED5);
	LED_ON(PIN_LED6);

	uint32_t i;

	uint32_t write_flash_flag = 0;
	
	printk("Start Run Main Loop\n");

	while(1)
	{
		if(BUTTON_ACTIVE)
		{
			LED_TOGGLE(PIN_LED6);

			write_flash_flag = 1;
		}

		if(write_flash_flag == 1)
		{
			Write_Data_to_Flash(&disk1);

			write_flash_flag = 2;
		}

		if(write_flash_flag == 2)
		{
			write_flash_flag = 0;

			inode_t *node;
			char buf[128];
			for(i= 0; i < 128; i++)
			{
				buf[i] = 0;
			}

			node = fs_type[ROMFS]->Namei(fs_type[ROMFS], "number.txt");
			fs_type[ROMFS]->device->Dout(fs_type[ROMFS]->device, buf, fs_type[ROMFS]->Get_daddr(node), node->dsize);
			for(i = 0; i < sizeof(buf); i++) {
				printk("%c ",buf[i]);
			}
			printk("\n");
		}
	}
}

void Write_Data_to_Flash(storage_device_t *trg)
{
	disk1.Din(trg, usart_FIFO, 0, usart_front);

	usart_front = 0;
}