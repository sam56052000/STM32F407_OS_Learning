#ifndef DRIVER_H_
#define DRIVER_H_

#include "./Boot/peripherals.h"

#define MAX_DISK_NUM		3
#define SECTOR_SIZE			0x00020000
#define DISK_SIZE			0x00040000
#define DISK1_ADDRESS		0x08020000
#define DISK2_ADDRESS		0x08060000
#define DISK3_ADDRESS		0x080A0000

typedef struct storage_device_t storage_device_t;

struct storage_device_t
{
	uint32_t start_pos;
	uint32_t sector_size;
	uint32_t storage_szie;

	int32_t (*Dout)(storage_device_t *sd, void *dest, uint32_t bias, uint32_t size);
	int32_t (*Din)(storage_device_t *sd, void *dest, uint32_t bias, uint32_t size);

};

storage_device_t *storage[MAX_DISK_NUM];
storage_device_t disk1, disk2, disk3;

//
// Function
//
void Disk_Init(void);
void Disk_Clear(void);
int32_t Disk_Data_Out(storage_device_t *sd, void *dest, uint32_t bias, uint32_t size);
int32_t Disk_Data_In(storage_device_t *sd, void *dest, uint32_t bias, uint32_t size);

#endif