#ifndef STM32F4_SYS_
#define STM32F4_SYS_

#define SYSCLK				168000000

//
// define variable type
//
typedef unsigned char 		uint8_t;
typedef unsigned short		uint16_t;
typedef unsigned int 		uint32_t;
typedef unsigned long long	uint64_t;

typedef signed char			int8_t;
typedef short				int16_t;
typedef int					int32_t;
typedef	long long			int64_t;

//
// define system variable
//
uint32_t SystemCoreClock;

//
// Function
//
void SystemInit(void);

#endif