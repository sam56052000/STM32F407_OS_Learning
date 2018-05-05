#include "stm32f4_rcc.h"

void RCC_Set_Default(void)
{
	uint32_t *CR_Reg = (uint32_t *)(RCC_REG + RCC_REG_CR_OFFSET);
	uint32_t *CFGR_Reg = (uint32_t *)(RCC_REG + RCC_REG_CFGR_OFFSET);
	uint32_t *PLLCFGR_Reg = (uint32_t *)(RCC_REG + RCC_REG_PLLCFGR_OFFSET);
	uint32_t *CIR_Reg = (uint32_t *)(RCC_REG + RCC_REG_CIR_OFFSET);

	//
	// Set On HSION Bit
	//
	*CR_Reg |= RCC_REG_CR_HSION;

	//
	// Reset CFGR Register
	//
	*CFGR_Reg = 0x00000000;

	//
	// Reset HSEON, CSSON and PLLON Bits
	//
	*CR_Reg &= ~(RCC_REG_CR_HSEON | RCC_REG_CR_CSSON | RCC_REG_CR_PLLON);

	//
	// Reset PLLCFGR Register
	//
	*PLLCFGR_Reg = RCC_REG_PLLCFGR_DEFAULT;

	//
	// Reset HSEBYP Bit (HSE Bypass)
	//
	*CR_Reg &= ~(RCC_REG_CR_HSEBYP);

	//
	// Disable all Interrupt
	//
	*CIR_Reg = 0x00000000;
}

void RCC_Set_System_Clock(void)
{
	volatile uint32_t *CR_Reg = (volatile uint32_t *)(RCC_REG + RCC_REG_CR_OFFSET);
	volatile uint32_t *CFGR_Reg = (volatile uint32_t *)(RCC_REG + RCC_REG_CFGR_OFFSET);
	volatile uint32_t *PLLCFGR_Reg = (volatile uint32_t *)(RCC_REG + RCC_REG_PLLCFGR_OFFSET);

	uint32_t HSE_Status = 0, HSE_Counter = 0;

	//
	// Enable HSE Clock
	// Wait HSE Ready, or Timeout to do Nothing.
	//
	*CR_Reg |= RCC_REG_CR_HSEON;

	do
	{
		HSE_Status = (*CR_Reg) & RCC_REG_CR_HSERDY;

		HSE_Counter ++;
	}
	while( (HSE_Status == 0) && (HSE_Counter != SYS_CLK_HSE_TIMEOUT) );

	//
	// If HSE Ready, Init System Clock
	//
	if(HSE_Status != RESET)
	{
		//
		// Enable High Performance Mode
		//
		Enable_RCC_APB1(RCC_REG_APB1_PWREN);

		//
		// Set System Clock Prescaler
		// 
		// HCKL	= SYSCLK
		// PCLK1= HCLK / 4
		// PCLK2= HCLK / 2
		//
		*CFGR_Reg |= RCC_REG_CFGR_HPRE_DIV1;
		*CFGR_Reg |= RCC_REG_CFGR_PPRE1_DIV4;
		*CFGR_Reg |= RCC_REG_CFGR_PPRE2_DIV2;

		//
		// Configure Main PLL Clock, and wait PLL Clock Ready
		//
		*PLLCFGR_Reg =	(SYS_CLK_PLL_M)
						| (SYS_CLK_PLL_N << 6)
						| (((SYS_CLK_PLL_P >> 1) - 1) << 16)
						| (SYS_CLK_PLL_Q << 24)
						| (RCC_REG_PLLCFGR_SRC_HSE);

		*CR_Reg |= RCC_REG_CR_PLLON;

		while( (*CR_Reg & RCC_REG_CR_PLLRDY) == 0);

		//
		// Flash fetch Setting
		//
		Flash_Register_Init();

		//
		// Switch PLL as System Clock Source, and wait switch ready
		//
		*CFGR_Reg &= ~(RCC_REG_CFGR_SW);

		*CFGR_Reg |= RCC_REG_CFGR_SW_PLL;

		while( (*CFGR_Reg & RCC_REG_CFGR_SWS) != RCC_REG_CFGR_SWS_PLL);

		SystemClock = 168000000;
		AHBClock = 168000000;
		APB1Clock = 84000000;
		APB2Clock = 42000000;
	}
}

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