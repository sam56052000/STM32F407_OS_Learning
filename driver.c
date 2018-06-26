#include "driver.h"

void Disk_Init(void)
{
	//
	//	Flash Parameter Init
	//
	Flash_Unlock();

	Flash_Erase_Sector(FLASH_CR_SECTOR5);
	Flash_Erase_Sector(FLASH_CR_SECTOR6);
	// Flash_Erase_Sector(FLASH_CR_SECTOR7);
	// Flash_Erase_Sector(FLASH_CR_SECTOR8);
	// Flash_Erase_Sector(FLASH_CR_SECTOR9);
	// Flash_Erase_Sector(FLASH_CR_SECTOR10);

	Flash_Clear_Flag(FLASH_SR_PGSERR | FLASH_SR_PGPERR | FLASH_SR_PGAERR
		 | FLASH_SR_WRPERR | FLASH_SR_OPERR | FLASH_SR_EOP);

	Flash_Lock();

	//
	//	Disk Driver Init
	//
	disk1.start_pos = DISK1_ADDRESS;
	disk1.sector_size = SECTOR_SIZE;
	disk1.storage_szie = DISK_SIZE;
	disk1.Dout = Disk_Data_Out;
	disk1.Din = Disk_Data_In;

	disk2.start_pos = DISK2_ADDRESS;
	disk2.sector_size = SECTOR_SIZE;
	disk2.storage_szie = DISK_SIZE;
	disk2.Dout = Disk_Data_Out;
	disk2.Din = Disk_Data_In;

	disk3.start_pos = DISK3_ADDRESS;
	disk3.sector_size = SECTOR_SIZE;
	disk3.storage_szie = DISK_SIZE;
	disk3.Dout = Disk_Data_Out;
	disk3.Din = Disk_Data_In;
}

void Disk_Clear(void)
{
	Flash_Unlock();
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

	for(i = 0; i < size; i += 4)
	{
		Flash_Write_Data(sd->start_pos + bias, *(uint32_t *)sorc);

		bias += 4;
		sorc += 4;
	}

	Flash_Lock();
}