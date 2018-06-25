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
// System Function
//
void HW_Float_Process_Unit_Enable(void);

//
// Flash Register
//
#define FLASH_REG_ACR			0x40023C00
#define FLASH_REG_KEY			0x40023C04
#define FLASH_REG_OPTKEY		0x40023C08
#define FLASH_REG_SR			0x40023C0C
#define FLASH_REG_CR			0x40023C10

//
// Flash Memory Address
// stm32f407 mcu spec p75
//
#define FLASH_ADD_SECTOR0		0x08000000
#define FLASH_ADD_SECTOR1		0x08004000
#define FLASH_ADD_SECTOR2		0x08008000
#define FLASH_ADD_SECTOR3		0x0800C000
#define FLASH_ADD_SECTOR4		0x08010000
#define FLASH_ADD_SECTOR5		0x08020000
#define FLASH_ADD_SECTOR6		0x08040000
#define FLASH_ADD_SECTOR7		0x08060000
#define FLASH_ADD_SECTOR8		0x08080000
#define FLASH_ADD_SECTOR9		0x080A0000
#define FLASH_ADD_SECTOR10		0x080C0000
#define FLASH_ADD_SECTOR11		0x080E0000

//
// Flash Configure Register
//	ICEN : Instruction Cache Enable
//	DCEN : Data Cache Enable
// 
// stm32f407 mcu spec p98
//
#define FLASH_ACR_ICEN          0x00000200
#define FLASH_ACR_DCEN          0x00000400

#define FLASH_KEY1				0x45670123
#define FLASH_KEY2				0xCDEF89AB

//
// Flash Flag
// stm32f407 mcu spec p101
//
// PGSERR : program squence error
// PGPERR : program parallelism error
// PGAERR : program alignment error
// WRPERR : write protection error
// OPERR  : operation error
// EOP    : end of operation
//
#define FLASH_SR_BUSY			0x00010000
#define FLASH_SR_PGSERR			0x00000080
#define FLASH_SR_PGPERR			0x00000040
#define FLASH_SR_PGAERR			0x00000020
#define FLASH_SR_WRPERR			0x00000010
#define FLASH_SR_OPERR			0x00000002
#define FLASH_SR_EOP			0x00000001

//
// Flash Control Register
// stm32f407 mcu spec p104
//
#define FLASH_CR_LOCK			0x80000000
#define FLASH_CR_START			0x00010000
#define FLASH_CR_SER			0x00000002
#define FLASH_CR_PG				0x00000001
#define FLASH_CR_PSIZE_MASK		0x00000300
#define FLASH_CR_SNB_MASK		0x00000078

#define FLASH_CR_SIZE8			0x00000000
#define FLASH_CR_SIZE16			0x00000100
#define FLASH_CR_SIZE32			0x00000200
#define FLASH_CR_SIZE64			0x00000300

//
//	Chip Memory Allocation
//
#define STM32F4_SRAM1_START		0x20000000
#define STM32F4_SRAM2_START		0x2001C000

//
// Flash Function
//
void Flash_Register_Init(void);
void Flash_Unlock(void);
void Flash_Lock(void);
void Flash_Clear_Flag(uint32_t flag);
void Flash_Wait_Operation(void);
void Flash_Erase_Sector(uint32_t sector_number);
void Flash_Write_Data(uint32_t addr, uint32_t data);

#endif