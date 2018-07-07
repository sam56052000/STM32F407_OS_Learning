#ifndef DRIVER_H_
#define DRIVER_H_

#include "./Boot/peripherals.h"

#define MAX_DISK_NUM		3
#define DISK_1 				0
#define RAM_1				1

#define SECTOR_SIZE			1024
#define DISK_SIZE			0x00040000
#define DISK1_ADDRESS		0x08020000
#define DISK2_ADDRESS		0x08060000
#define DISK3_ADDRESS		0x080A0000

typedef struct storage_device_t storage_device_t;

struct storage_device_t
{
	uint32_t start_pos;
	uint32_t sector_size;
	uint32_t storage_size;

	void (*Dout)(storage_device_t *sd, void *dest, uint32_t bias, uint32_t size);
	void (*Din)(storage_device_t *sd, void *dest, uint32_t bias, uint32_t size);

};

storage_device_t *storage[MAX_DISK_NUM];
storage_device_t disk1, disk2, disk3, ram1;

//
//	UART Data Buffer
//
#define UART_FIFO_SIZE		1024

char usart_FIFO[UART_FIFO_SIZE];
uint32_t usart_front;
uint32_t usart_rear;

//
// Function
//
void Disk_Init(void);
void Disk_Clear(storage_device_t *disk);
void Disk_Data_Out(storage_device_t *sd, void *dest, uint32_t bias, uint32_t size);
void Disk_Data_In(storage_device_t *sd, void *dest, uint32_t bias, uint32_t size);
int32_t Register_Storage_Device(storage_device_t *sd, uint32_t num);
void Unregister_Storage_Device(uint32_t num);
void Ram_Data_Out(storage_device_t *sd, void *dest, uint32_t bias, uint32_t size);
void Ram_Data_In(storage_device_t *sd, void *sorc, uint32_t bias, uint32_t size);

#endif