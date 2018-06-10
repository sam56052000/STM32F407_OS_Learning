#ifndef STM32F4_SYS_
#define STM32F4_SYS_

#include "../Core/arm_cortex_m4.h"
#include "../Core/arm_cortex_nvic.h"

//
// Hardware Float Process Uint Address
//
#define HW_FLOAT_UNIT_ADDR		0xE000ED88
#define HW_FLOAT_UNIT_ENABLE	(0xF<<20)

//
// System Configure Macro
//
#define SYS_CLK_HSE				8000000
#define SYS_CLK_HSE_TIMEOUT		0x5000
#define SYS_CLK_PLL_M			8
#define SYS_CLK_PLL_N			336
#define SYS_CLK_PLL_P			2
#define SYS_CLK_PLL_Q			7

#define SYS_FLASH_LATENCY		5

//
// Define System Variable
//
uint32_t SystemClock;
uint32_t AHBClock;
uint32_t APB1Clock;
uint32_t APB2Clock;

//
// Flash Configure Register
//	ICEN : Instruction Cache Enable
//	DCEN : Data Cache Enable
// 
// stm32f407 mcu spec p98
//
#define FLASH_REG_ACR			0x40023C00

#define FLASH_ACR_ICEN          0x00000200
#define FLASH_ACR_DCEN          0x00000400

//
//	Chip Memory Allocation
//
#define STM32F4_SRAM1_START		0x20000000
#define STM32F4_SRAM2_START		0x2001C000

//
// Function
//
void Flash_Register_Init(void);
void HW_Float_Process_Unit_Enable(void);

#endif