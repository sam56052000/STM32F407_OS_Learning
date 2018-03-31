#include "stm32f4_gpio.h"

void Gpio_Init(uint32_t group, uint32_t pin, uint32_t mode, uint32_t otype
	, uint32_t speed, uint32_t pupd)
{
	uint32_t *mode_reg, *otype_reg, *speed_reg, *pupd_reg;

	uint32_t pin_mask_2bit = (0x03 << (pin << 1));

	uint32_t pin_mask_1bit = (0x01 << pin);

	mode_reg = (uint32_t *)(group + GPIO_REG_MODE_OFFSET);

	otype_reg = (uint32_t *)(group + GPIO_REG_OTYPE_OFFSET);

	speed_reg = (uint32_t *)(group + GPIO_REG_SPEED_OFFSET);

	pupd_reg = (uint32_t *)(group + GPIO_REG_PUPD_OFFSET);

	*mode_reg &= ~pin_mask_2bit;
	*mode_reg |= mode << (pin << 1);

	*otype_reg &= ~pin_mask_1bit;
	*otype_reg |= otype << pin;

	*speed_reg &= ~pin_mask_2bit;
	*speed_reg |= speed << (pin << 1);

	*pupd_reg &= ~pin_mask_2bit;
	*pupd_reg |= pupd << (pin << 1);
}

void Gpio_Set(uint32_t group, uint32_t pin)
{
	uint32_t *ctrl_reg = (uint32_t *)(group + GPIO_REG_CTRL_OFFSET);

	*ctrl_reg |= (0x01 << pin);
}

void Gpio_Reset(uint32_t group, uint32_t pin)
{
	uint32_t *ctrl_reg = (uint32_t *)(group + GPIO_REG_CTRL_OFFSET);

	*ctrl_reg |= (0x01 << (pin + 15));
}

void Gpio_Toggle(uint32_t group, uint32_t pin)
{
	uint32_t *ctrl_reg = (uint32_t *)(group + GPIO_REG_OUT_DATA_OFFSET);

	*ctrl_reg ^= (0x01 << pin);
}

uint32_t Gpio_Read(uint32_t group, uint32_t pin)
{
	uint32_t *input_reg = (uint32_t *)(group + GPIO_REG_IN_DATA_OFFSET);

	if((*input_reg >> pin) & 0x01)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}