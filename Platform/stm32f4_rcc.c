#include "stm32f4_rcc.h"

void Enable_RCC_AHB1(uint32_t module)
{
	uint32_t *rcc_reg = (uint32_t *)(RCC_REG + RCC_REG_AHB1_EN_OFFSET);

	*rcc_reg |= module;
}

void Enable_RCC_APB1(uint32_t module)
{
	uint32_t *rcc_reg = (uint32_t *)(RCC_REG + RCC_REG_APB1_EN_OFFSET);

	*rcc_reg |= module;
}