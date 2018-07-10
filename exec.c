#include "exec.h"

uint32_t Execute_Application(void *start)
{
	asm volatile(
		"mov pc, r0\n\t");
	
	return (uint32_t)start;
}