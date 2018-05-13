#include "stm32f4_sys.h"

void Flash_Register_Init(void)
{
	uint32_t *ACR_Reg = (uint32_t *)(FLASH_REG_ACR);

	*ACR_Reg |= (FLASH_ACR_ICEN | FLASH_ACR_DCEN | SYS_FLASH_LATENCY);
}

void HW_Float_Process_Unit_Enable(void)
{
	uint32_t *Unit_Addr = (uint32_t *)(HW_FLOAT_UNIT_ADDR);

	*Unit_Addr |= HW_FLOAT_UNIT_ENABLE;
}