#include "boot.h"

void STM32f407_Init_Clock(void)
{
	HW_Float_Process_Unit_Enable();

	RCC_Set_Default();

	RCC_Set_System_Clock();
}

void STM32f407_Init_LED(void)
{
	Enable_RCC_AHB1(RCC_REG_AHB1_GPIOD);

	Gpio_Init(GPIOD, PIN_LED3, GPIO_REG_MODE_OUTPUT, GPIO_REG_OTYPE_PP
		, GPIO_REG_SPEED_FAST, GPIO_REG_PUPD_PULLUP);
	Gpio_Init(GPIOD, PIN_LED4, GPIO_REG_MODE_OUTPUT, GPIO_REG_OTYPE_PP
		, GPIO_REG_SPEED_FAST, GPIO_REG_PUPD_PULLUP);
	Gpio_Init(GPIOD, PIN_LED5, GPIO_REG_MODE_OUTPUT, GPIO_REG_OTYPE_PP
		, GPIO_REG_SPEED_FAST, GPIO_REG_PUPD_PULLUP);
	Gpio_Init(GPIOD, PIN_LED6, GPIO_REG_MODE_OUTPUT, GPIO_REG_OTYPE_PP
		, GPIO_REG_SPEED_FAST, GPIO_REG_PUPD_PULLUP);
}

void STM32f407_Init_UART(void)
{
	Enable_RCC_AHB1(RCC_REG_AHB1_GPIOC);
	Enable_RCC_APB1(RCC_REG_APB1_USART3);

	Gpio_Init(GPIOC, PIN_UART3_TX, GPIO_REG_MODE_FUNCTION, GPIO_REG_OTYPE_PP
		, GPIO_REG_SPEED_FAST, GPIO_REG_PUPD_NONE);
	Gpio_Init(GPIOC, PIN_UART3_RX, GPIO_REG_MODE_FUNCTION, GPIO_REG_OTYPE_PP
		, GPIO_REG_SPEED_FAST, GPIO_REG_PUPD_NONE);

	Gpio_Alternate_Function_Setting(GPIOC, PIN_UART3_TX, GPIO_AF_USART3);
	Gpio_Alternate_Function_Setting(GPIOC, PIN_UART3_RX, GPIO_AF_USART3);

    USART_BAISC_INIT_t UsartInitStruct;

    UsartInitStruct.Baudrate = 57600;
    UsartInitStruct.WordLength = USART_REG_INIT_WORD_LENGTH_8;
    UsartInitStruct.StopBits = USART_REG_INIT_STOP_1_0;
    UsartInitStruct.Pairty = USART_REG_INIT_PAIRTY_NO;
    UsartInitStruct.Hw_Ctrl = USART_REG_HW_FLOW_CTRL_NONE;
    UsartInitStruct.Mode = USART_REG_INIT_MODE_TX | USART_REG_INIT_MODE_RX;

    Usart_Init_Register_Setting(USART3, &UsartInitStruct);
    Usart_CMD(USART3, USART_ENABLE);
}

void Test_Printk(void)
{
	char *s = "this is %s test";
	char c = 'C';
	int32_t d = -256;
	int32_t x = 0x12345678;

	printk("\nTesting printk %%:\n");
	printk("Print Char: %c\n", c);
	printk("Print Str : %s\n", s);
	printk("Print Dec : %d\n", d);
	printk("Print Hex : %x\n", x);
	printk("Print Ptr : %p\n", &x);
}

void Platform_Boot(void)
{
	int i;

	for(i = 0; init_func[i]; i++)
	{
		init_func[i]();
	}
}

void Print_Boot_Data(void)
{
	UART_Puts("\n");
	UART_Puts("====================================================================\n");
	UART_Puts("                     STM32F407 OS Test Learning                     \n");
	UART_Puts("====================================================================\n");
	UART_Puts("Platform Boot Complete\n");

	int32_t x = 0x12345678;
	printk("Print Hex : %x%x\n", x,x);
}