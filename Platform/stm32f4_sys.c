#include "stm32f4_sys.h"

void Flash_Register_Init(void)
{
	volatile uint32_t *ACR_Reg = (uint32_t *)(FLASH_REG_ACR);

	*ACR_Reg |= (FLASH_ACR_ICEN | FLASH_ACR_DCEN | SYS_FLASH_LATENCY);
}

void Flash_Unlock(void)
{
	volatile uint32_t *KEY_Reg = (uint32_t *)(FLASH_REG_KEY);

	*KEY_Reg = FLASH_KEY1;
	*KEY_Reg = FLASH_KEY2;
}

void Flash_Lock(void)
{
	volatile uint32_t *CR_Reg = (uint32_t *)(FLASH_REG_KEY);

	*CR_Reg |= FLASH_CR_LOCK;
}

void Flash_Clear_Flag(uint32_t flag)
{
	volatile uint32_t *SR_Reg = (uint32_t *)(FLASH_REG_SR);

	*SR_Reg |= flag;
}

void Flash_Wait_Operation(void)
{
	volatile uint32_t *SR_Reg = (uint32_t *)(FLASH_REG_SR);

	while(*SR_Reg & FLASH_SR_BUSY);
}

void Flash_Erase_Sector(uint32_t sector_number)
{
	volatile uint32_t *CR_Reg = (uint32_t *)(FLASH_REG_CR);

	//
	//	Check Flash Controller is idle
	//
	Flash_Wait_Operation();

	//
	//	Sector Erase Operation
	//
	*CR_Reg &= ~(FLASH_CR_PSIZE_MASK);
	*CR_Reg &= ~(FLASH_CR_SNB_MASK);
	*CR_Reg |= FLASH_CR_SIZE32;
	*CR_Reg |= (sector_number | FLASH_CR_SER);
	*CR_Reg |= FLASH_CR_START;

	//
	//	Wait Erase Operation Finish
	//
	Flash_Wait_Operation();
}

void Flash_Write_Data(uint32_t addr, uint32_t data)
{
	volatile uint32_t *CR_Reg = (uint32_t *)(FLASH_REG_CR);

	*CR_Reg &= ~(FLASH_CR_SNB_MASK);
	*CR_Reg |= FLASH_CR_SIZE32;
	*CR_Reg |= FLASH_CR_PG;

	*(volatile uint32_t *)(addr) = data;

	Flash_Wait_Operation();

	*CR_Reg &= ~(FLASH_CR_PG);
}

void HW_Float_Process_Unit_Enable(void)
{
	volatile uint32_t *Unit_Addr = (uint32_t *)(HW_FLOAT_UNIT_ADDR);

	*Unit_Addr |= HW_FLOAT_UNIT_ENABLE;
}