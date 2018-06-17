#ifndef ARM_CORTEX_M4_H_
#define ARM_CORTEX_M4_H_

//
// define variable type
//
typedef unsigned char 			uint8_t;
typedef unsigned short			uint16_t;
typedef unsigned int 			uint32_t;
typedef unsigned long long		uint64_t;

typedef signed char				int8_t;
typedef short					int16_t;
typedef int						int32_t;
typedef	long long				int64_t;

//
// Global Macro Define
//
#define SET						1
#define RESET					0

#define ENABLE					1
#define DISABLE					0

#define	NULL					((void *)0)

//
//	Systick define in arm-cortex maunal
//
typedef struct SYSTICK_t
{
	volatile uint32_t ctrl;
	volatile uint32_t load;
	volatile uint32_t value;
	volatile uint32_t calibration;

} SYSTICK_t;

#define SYSTICK_ADDR	0xE000E010
#define Systick			((SYSTICK_t *)(SYSTICK_ADDR))

//
//	Systick Ctrl Register
//	
//	ARM Cortex-M4 Spec
//
#define SYSTICK_REG_CTRL_ENABLE			0x1
#define SYSTICK_REG_CTRL_DISABLE		0x0
#define SYSTICK_REG_CTRL_EXTCLK			0x0
#define SYSTICK_REG_CTRL_CORECLK		0x4
#define SYSTICK_REG_CTRL_TICKINT_EN		0x2
#define SYSTICK_REG_CTRL_TICKINT_DIS	0x0
#define SYSTICK_REG_CTRL_CNTFLAG_BIT	0x00010000

//
//	Function
//
void Systick_Config(uint32_t ticks);

#endif