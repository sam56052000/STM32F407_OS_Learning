#include "proc.h"

void Task_Init(void)
{
	task_stack_base = TASK_STACK_BASE;

	task_info_t *tsk = (task_info_t *)task_stack_base;

	tsk->sp = (uint32_t *)(task_stack_base + TASK_SIZE);

	// *((uint32_t *)(tsk->sp - 1)) = Get_xPSR();;
	// *((uint32_t *)(tsk->sp - 2)) = (uint32_t)Main_Loop;
	// *((uint32_t *)(tsk->sp - 3)) = (uint32_t)Main_Loop;
	// *((uint32_t *)(tsk->sp - 4)) = 0;
	// *((uint32_t *)(tsk->sp - 5)) = 0;
	// *((uint32_t *)(tsk->sp - 6)) = 0;
	// *((uint32_t *)(tsk->sp - 7)) = 0;
	// *((uint32_t *)(tsk->sp - 8)) = 0x1234;

	// tsk->sp -= 8;

	tsk->next = tsk;

	current = tsk;

	task_stack_base += TASK_SIZE;

	Do_Fork((void (*)(void *))Task_Function, 0x1234);
	Do_Fork((void (*)(void *))Task_Function, 0x5678);

	Systick_Config(2050000000);

	__asm__ __volatile__
	(
		"msr PSP, %[sp]\n"
		"mov pc, %[func]\n"
		"mov r0, #3\n"
		"msr control, r0\n"
		: 
		: [sp]"r"(tsk->sp), [func]"r"(Main_Loop)
		: 
	);
}

uint32_t *common_schedule(void)
{
	current = current->next;

	return current->sp;
}

task_info_t *Current_Task_Info(void)
{
	register uint32_t sp asm ("sp");

	return (task_info_t *)(sp &~ (TASK_SIZE-1));
}

task_info_t *Copy_Task_Info(void)
{
	task_info_t *tmp = (task_info_t *)task_stack_base;

	task_stack_base += TASK_SIZE;

	return tmp;
}

uint32_t Get_xPSR(void)
{
	uint32_t p;

	__asm__ __volatile__
	(
		"mrs %[p], xPSR\n"
		:[p]"=r"(p)
		:
		:
	);
	return p;
}

int32_t Do_Fork(void (*f)(void *), uint32_t args)
{
	task_info_t *tsk, *tmp;

	if( (tsk = Copy_Task_Info() ) == (void *)0)
		return -1;

	tsk->sp = (uint32_t *)((uint32_t)(tsk) + TASK_SIZE);
	
	*((uint32_t *)(tsk->sp - 1)) = Get_xPSR();;
	*((uint32_t *)(tsk->sp - 2)) = (uint32_t)f;
	*((uint32_t *)(tsk->sp - 3)) = 0;
	*((uint32_t *)(tsk->sp - 4)) = 0;
	*((uint32_t *)(tsk->sp - 5)) = 0;
	*((uint32_t *)(tsk->sp - 6)) = 0;
	*((uint32_t *)(tsk->sp - 7)) = 0;
	*((uint32_t *)(tsk->sp - 8)) = args;

	tsk->sp -= 8;

	//Systick_Disable();

	tmp = current->next;
	current->next = tsk;
	tsk->next = tmp;

	//Systick_Enable();

	return 0;
}