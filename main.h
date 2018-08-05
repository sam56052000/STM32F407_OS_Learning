#ifndef MAIN_H_
#define MAIN_H_

#include "./Boot/peripherals.h"
#include "interrupt.h"
#include "memory.h"
#include "driver.h"
#include "filesystem.h"
#include "exec.h"
#include "systemcall.h"
#include "proc.h"

void OS_Main_Init(void);
void Write_Data_to_Flash(storage_device_t *trg);
void Task_Function(uint32_t args);
void Main_Loop(void);

#define APPLICATION_ADDRESS		0x20018000

#endif