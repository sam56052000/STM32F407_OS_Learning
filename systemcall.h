#ifndef SYSTEMCALL_H_
#define SYSTEMCALL_H_

#include "./Boot/peripherals.h"
#include "./Boot/print.h"

void SVC0(void);
void SVC1(void);
void SVC2(void);

#define INCLUDE_SYSTEMCALL \
	SYSTEMCALL(system0, SVC0), \
	SYSTEMCALL(system1, SVC1), \
	SYSTEMCALL(system2, SVC2), \
	SYSTEMCALL(empty, SVC_Empty)

enum SystemCall_enum
{
	#define SYSTEMCALL(name, func)	SVC##name
		INCLUDE_SYSTEMCALL
	#undef SYSTEMCALL
	,eSystemCall_size
};

void SystemCall(uint32_t SVC_Number);
void Syscall_Handler(uint32_t exe);
void SVC_Empty(void);

#endif