#include "stm32f4_sys.h"

void Flash_Register_Init(void)
{
	uint32_t *ACR_Reg = (uint32_t *)(FLASH_REG_ACR);

	*ACR_Reg |= (FLASH_ACR_ICEN | FLASH_ACR_DCEN | SYS_FLASH_LATENCY);
}