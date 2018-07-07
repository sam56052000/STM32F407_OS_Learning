#include "driver.h"

void Disk_Init(void)
{
	disk1.start_pos = DISK1_ADDRESS;
	disk1.sector_size = 128;
	disk1.storage_size = DISK_SIZE;
	disk1.Dout = Disk_Data_Out;
	disk1.Din = Disk_Data_In;
	Register_Storage_Device(&disk1, DISK_1);

	ram1.start_pos = (uint32_t)&usart_FIFO;
	ram1.sector_size = 128;
	ram1.storage_size = UART_FIFO_SIZE;
	ram1.Dout = Ram_Data_Out;
	ram1.Din = Ram_Data_In;
	Register_Storage_Device(&ram1, RAM_1);

	disk2.start_pos = DISK2_ADDRESS;
	disk2.sector_size = SECTOR_SIZE;
	disk2.storage_size = DISK_SIZE;
	disk2.Dout = Disk_Data_Out;
	disk2.Din = Disk_Data_In;

	disk3.start_pos = DISK3_ADDRESS;
	disk3.sector_size = SECTOR_SIZE;
	disk3.storage_size = DISK_SIZE;
	disk3.Dout = Disk_Data_Out;
	disk3.Din = Disk_Data_In;
}

void Disk_Clear(storage_device_t *disk)
{
	switch(disk -> start_pos)
	{
		case DISK1_ADDRESS:
		{
			Flash_Erase_Sector(FLASH_CR_SECTOR5);
			Flash_Erase_Sector(FLASH_CR_SECTOR6);
		}
		break;
		case DISK2_ADDRESS:
		{
			Flash_Erase_Sector(FLASH_CR_SECTOR7);
			Flash_Erase_Sector(FLASH_CR_SECTOR8);
		}
		break;
		case DISK3_ADDRESS:
		{
			Flash_Erase_Sector(FLASH_CR_SECTOR9);
			Flash_Erase_Sector(FLASH_CR_SECTOR10);
		}
		break;
	}

	Flash_Clear_Flag(FLASH_SR_PGSERR | FLASH_SR_PGPERR | FLASH_SR_PGAERR
		 | FLASH_SR_WRPERR | FLASH_SR_OPERR | FLASH_SR_EOP);
}

void Disk_Data_Out(storage_device_t *sd, void *dest, uint32_t bias, uint32_t size)
{
	uint32_t i;

	for(i = 0; i < size; i += 4)
	{
		*(uint32_t *)dest = *(uint32_t *)(sd->start_pos + bias);

		bias += 4;
		dest += 4;
	}
}

void Disk_Data_In(storage_device_t *sd, void *sorc, uint32_t bias, uint32_t size)
{
	uint32_t i;

	Flash_Unlock();

	Disk_Clear(sd);

	for(i = 0; i < size; i += 4)
	{
		Flash_Write_Data(sd->start_pos + bias, *(uint32_t *)sorc);

		bias += 4;
		sorc += 4;
	}

	Flash_Lock();
}

void Ram_Data_Out(storage_device_t *sd, void *dest, uint32_t bias, uint32_t size)
{
	uint32_t i;

	for(i = 0; i < size; i += 4)
	{
		*(uint32_t *)dest = *(uint32_t *)(sd->start_pos + bias);

		bias += 4;
		dest += 4;
	}
}

void Ram_Data_In(storage_device_t *sd, void *sorc, uint32_t bias, uint32_t size)
{
	uint32_t i;

	for(i = 0; i < size; i += 4)
	{
		*(uint32_t *)(sd->start_pos + bias) = *(uint32_t *)sorc;

		bias += 4;
		sorc += 4;
	}
}

int32_t Register_Storage_Device(storage_device_t *sd, uint32_t num)
{
	if(num > MAX_DISK_NUM)
		return -1;

	if(storage[num])
	{
		return -1;
	}
	else
	{
		storage[num] = sd;
	}

	return 0;
}

void Unregister_Storage_Device(uint32_t num)
{
	if(num > MAX_DISK_NUM)
		return;

	storage[num] = NULL;
}