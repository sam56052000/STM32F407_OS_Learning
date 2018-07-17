#include "systemcall.h"

static void (*SystemCall_FuncArray[eSystemCall_size])(void) = 
{
	#define SYSTEMCALL(name, func)	(void *)func
		INCLUDE_SYSTEMCALL
	#undef SYSTEMCALL
};

void SVC_Empty(void)
{

}

void SVC0(void)
{
	printk("Execute the SVC0\n");
}

void SVC1(void)
{
	printk("Execute the SVC1\n");
}

void SVC2(void)
{
	printk("Execute the SVC2\n");
}

void SystemCall(uint32_t SVC_Number)
{
	__asm__ __volatile__
	(
		"mov r0, %[exe]\n"
		"svc 0\n"
		:
		:[exe]"r"(SVC_Number)
		:"r0"
	);
}

void Syscall_Handler(uint32_t exe)
{
	if(exe > eSystemCall_size - 1)
	{
		printk("No Function to Run\n");
		return;
	}

	if(SystemCall_FuncArray[exe] != SVC_Empty)
	{
		printk("Run the svc function: %d\n", exe);

		SystemCall_FuncArray[exe]();
	}
	else
	{
		printk("SYSCALL Function is Empty\n");
	}
}