#ifndef STM32F4_SYS_
#define STM32F4_SYS_

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
// Function
//
void Flash_Register_Init(void);

#endif