#include "arm_cortex_nvic.h"

void NVIC_Init(uint32_t channel, uint32_t priority, uint32_t subpriority,
	uint32_t cmd)
{
	if(cmd != DISABLE)
	{
		uint32_t *en_reg = (uint32_t *)(NVIC_REG_SETENA + (channel >> 5) * 4);
		uint8_t *priority_reg = (uint8_t *)(NVIC_REG_PRIORITY + channel);

		*en_reg |= 0x1 << (channel % 32);

		*priority_reg = priority << 4 | subpriority;
	}
	else
	{
		uint32_t *dis_reg = (uint32_t *)(NVIC_REG_CLRENA + (channel >> 5) * 4);

		*dis_reg |= 0x1 << channel;
	}
}