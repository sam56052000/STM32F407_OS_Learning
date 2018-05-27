#ifndef ARM_CORTEX_NVIC_H_
#define ARM_CORTEX_NVIC_H_

#include "arm_cortex_m4.h"

//
//	External Interrupt Vector enum
//
enum eEXT_NVIC
{
	eEXT_NVIC_0 = 0,
	eEXT_NVIC_1,
	eEXT_NVIC_2,
	eEXT_NVIC_3,
	eEXT_NVIC_4,
	eEXT_NVIC_5,
	eEXT_NVIC_6,
	eEXT_NVIC_7,
	eEXT_NVIC_8,
	eEXT_NVIC_9,
	eEXT_NVIC_10,
	eEXT_NVIC_11,
	eEXT_NVIC_12,
	eEXT_NVIC_13,
	eEXT_NVIC_14,
	eEXT_NVIC_15,
	eEXT_NVIC_16,
	eEXT_NVIC_17,
	eEXT_NVIC_18,
	eEXT_NVIC_19,
	eEXT_NVIC_20,
	eEXT_NVIC_21,
	eEXT_NVIC_22,
	eEXT_NVIC_23,
	eEXT_NVIC_24,
	eEXT_NVIC_25,
	eEXT_NVIC_26,
	eEXT_NVIC_27,
	eEXT_NVIC_28,
	eEXT_NVIC_29,
	eEXT_NVIC_30,
	eEXT_NVIC_31,
	eEXT_NVIC_32,
	eEXT_NVIC_33,
	eEXT_NVIC_34,
	eEXT_NVIC_35,
	eEXT_NVIC_36,
	eEXT_NVIC_37,
	eEXT_NVIC_38,
	eEXT_NVIC_USART3,
	eEXT_NVIC_40,

	eEXT_NVIC_SIZE
} eEXT_NVIC;

//
//	NVIC Register
//
#define NVIC_REG_SETENA		0xE000E100
#define NVIC_REG_CLRENA		0xE000E180
#define NVIC_REG_SETPEND	0xE000E200
#define NVIC_REG_PRIORITY	0xE000E400

//
//	Function
//
void NVIC_Init(uint32_t channel, uint32_t priority, uint32_t subpriority,
	uint32_t cmd);

#endif