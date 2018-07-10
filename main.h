#ifndef MAIN_H_
#define MAIN_H_

#include "./Boot/peripherals.h"
#include "interrupt.h"
#include "memory.h"
#include "driver.h"
#include "filesystem.h"
#include "exec.h"

void OS_Main_Init(void);
void Write_Data_to_Flash(storage_device_t *trg);

#define APPLICATION_ADDRESS		0x20018000

#endif