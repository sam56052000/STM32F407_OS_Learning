#include "arm_cortex_m4.h"

void Systick_Config(uint32_t ticks)
{
	//
	//	Set overflow counter value
	//
	Systick -> load = ticks - 1;

	//
	//	Reset the counter
	//
	Systick -> value = 0;

	//
	//	Enable Systick Clock
	//
	Systick -> ctrl |= SYSTICK_REG_CTRL_ENABLE;
	
	//
	//	Enable Systick interrupt
	//
	Systick -> ctrl |= SYSTICK_REG_CTRL_TICKINT_EN;

	//
	//	Use Core clock source
	//
	Systick -> ctrl |= SYSTICK_REG_CTRL_CORECLK;
}