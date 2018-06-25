#include "driver.h"

void Disk_Init(void)
{
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

int32_t Disk_Data_Out(storage_device_t *sd, void *dest, uint32_t bias, uint32_t size)
{

}

int32_t Disk_Data_In(storage_device_t *sd, void *dest, uint32_t bias, uint32_t size)
{
	
}