#ifndef PROC_H_
#define PROC_H_

#include "./Boot/peripherals.h"
#include "./Boot/print.h"

#define TASK_SIZE			0x400
#define TASK_STACK_BASE		0x2001A000

#define TASK_0				0x2001A000
#define TASK_1				0x2001A400
#define TASK_2				0x2001A800
#define TASK_3				0x2001AC00

#define STACK_MASK			0xFFFFFFF8

typedef struct task_info_t task_info_t;

struct task_info_t
{
	uint32_t *sp;
	task_info_t *next;
};

task_info_t *current;
uint32_t task_stack_base;

void Task_Init(void);
uint32_t *common_schedule(void);
task_info_t *Current_Task_Info(void);
task_info_t *Copy_Task_Info(void);
int32_t Do_Fork(void (*f)(void *), uint32_t args);
uint32_t Get_xPSR(void);

extern void Main_Loop(void);
extern void Task_Function(uint32_t args);


#endif