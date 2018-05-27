#ifndef STM32F4_USART_
#define STM32F4_USART_

#include "stm32f4_sys.h"

#define USART_CLK						42000000

//
// stm32f407 Usart periphreal address
// stm32f407 mcu spec p67
//
#define USART1							0x40011000
#define USART2							0x40004400
#define USART3							0x40004800
#define UART4							0x40004C00
#define UART5							0x40005000
#define USART6							0x40011400
#define UART7							0x40007800
#define UART8							0x40007C00

//
// stm32f407 usart registers
// stm32f407 mcu spec p1010
//
#define USART_REG_STATUS_OFFSET			0x00
#define USART_REG_DATA_OFFSET			0x04
#define USART_REG_BUADRATE_OFFSET		0x08
#define USART_REG_CTRL_1_OFFSET			0x0C
#define USART_REG_CTRL_2_OFFSET			0x10
#define USART_REG_CTRL_3_OFFSET			0x14
#define USART_REG_GT_PR_OFFSET			0x18

//
// Usart Init Basic Structure Define
//
#define USART_REG_INIT_WORD_LENGTH_8	0x0000
#define USART_REG_INIT_WORD_LENGTH_9	0x1000
#define USART_REG_INIT_MODE_DISABLE		0x0000
#define USART_REG_INIT_MODE_TX			0x0008
#define USART_REG_INIT_MODE_RX			0x0004
#define USART_REG_INIT_PAIRTY_ODD		0x0600
#define USART_REG_INIT_PAIRTY_EVEN		0x0400
#define USART_REG_INIT_PAIRTY_NO		0x0000
#define USART_REG_INIT_STOP_0_5			0x1000
#define USART_REG_INIT_STOP_1_0			0x0000
#define USART_REG_INIT_STOP_1_5			0x3000
#define USART_REG_INIT_STOP_2_0			0x2000
#define USART_REG_HW_FLOW_CTRL_NONE		0x0000
#define USART_REG_HW_FLOW_CTRL_RTS		0x0100
#define USART_REG_HW_FLOW_CTRL_CTS		0x0200
#define USART_REG_HW_FLOW_CTRL_RCTS		0x0300
#define USART_REG_UE					0x2000
#define USART_ENABLE					0x1
#define USART_DISABLE					0x0

//
// Interrupt Setting Flag
//
#define USART_REG_INT_RXNE				0x00000020
#define USART_REG_INT_IDLE				0x00000010
#define USART_REG_INT_TXE				0x00000080

//
// Usart Status Flag
//
#define USART_FLAG_RXNE					0x0020
#define USART_FLAG_TXE					0x0080

//
// Usart ctrl 1 register structure
// stm32f407 mcu spec p1013
//
typedef struct USART_Ctrl_1_Register_t
{
	uint32_t SBK	: 1;	// bit0  : Send Break
	uint32_t RWU	: 1;	// bit1  : Receiver Wakeup
	uint32_t RE		: 1;	// bit2  : Receiver Enable
	uint32_t TE		: 1;	// bit3  : Transmitter Enable
	uint32_t IDLEIE	: 1;	// bit4  : IDLE Interrupt Enable
	uint32_t RXNEIE	: 1;	// bit5  : RXNE Interrupt Enable
	uint32_t TCIE	: 1;	// bit6  : Transmission Complete Interrupt Enable
	uint32_t TXEIE	: 1;	// bit7  : TXE Enable
	uint32_t PEIE	: 1;	// bit8  : PE Interrupt Enable 
	uint32_t PS		: 1;	// bit9  : Pairty Selection
	uint32_t PCE	: 1;	// bit10 : Pairty Control Enable
	uint32_t WAKE	: 1;	// bit11 : Wakeup Method
	uint32_t M		: 1;	// bit12 : Word Length
	uint32_t UE		: 1;	// bit13 : Usart Enable
	uint32_t BIT14	: 1;	// bit14 : Reserved
	uint32_t OVER8	: 1;	// bit15 : Over Sampling Mode
	uint32_t RESV	:16;	// bit16 - 31 Reserved
} USART_Ctrl_1_Register_t;

//
// Usart ctrl 2 register structure
// stm32f407 mcu spec p1016
//
typedef struct USART_Ctrl_2_Register_t
{
	uint32_t ADD	: 4;	// bit0 - 3 Address of Usart Node
	uint32_t BIT4	: 1;	// bit4  : Reserved
	uint32_t LBDL	: 1;	// bit5  : LIN Break Detection Length 
	uint32_t LBDIE	: 1;	// bit6  : LIN Break Detection Interrupt Enable
	uint32_t BIT7	: 1;	// bit7  : Reserved
	uint32_t LBCL	: 1;	// bit8  : Last Bit Clock Pulse
	uint32_t CPHA	: 1;	// bit9  : Clock Phase
	uint32_t CPOL	: 1;	// bit10 : Clock Polarity
	uint32_t CLKEN	: 1;	// bit11 : Clock Enable
	uint32_t STOP	: 2;	// bit12 - 13 STOP bits
	uint32_t LINEN	: 1;	// bit14 : LIN Mode Enable
	uint32_t RESV	:17;	// bit15 - 31 Reserved
} USART_Ctrl_2_Register_t;

//
// Usart ctrl 3 register structure
// stm32f407 mcu spec p1017
//
typedef struct USART_Ctrl_3_Register_t
{
	uint32_t EIE	: 1;	// bit0  : Error Interrupt Enable
	uint32_t IREN	: 1;	// bit1  : IrDA Mode Enable
	uint32_t IRLP	: 1;	// bit2  : IrDA Low-Power
	uint32_t HDSEL	: 1;	// bit3  : Half Duplex Section
	uint32_t NACK	: 1;	// bit4  : Smartcard NACK Enable
	uint32_t SCEN	: 1;	// bit5  : Smartcard mode Enable
	uint32_t DMAR	: 1;	// bit6  : DMA Enable Receiver
	uint32_t DMAT	: 1;	// bit7  : DMA Enable Transmitter
	uint32_t RTSE	: 1;	// bit8  : RTS Enable
	uint32_t CTSE	: 1;	// bit9  : CTS Enable
	uint32_t CTSIE	: 1;	// bit10 : CTS Interrupt Enable
	uint32_t ONEBIT	: 1;	// bit11 : One Sample bit Method Enable
	uint32_t RESV	:20;	// bit12 - 31 Reserved
} USART_Ctrl_3_Register_t;

//
// Usart Init Basic Structure
//
typedef struct USART_BAISC_INIT_t
{
	uint32_t Baudrate;
	uint16_t WordLength;
	uint16_t StopBits;
	uint16_t Pairty;
	uint16_t Hw_Ctrl;
	uint16_t Mode;

} USART_BAISC_INIT_t;

//
// Function
//
void Usart_Init_Register_Setting(uint32_t group, USART_BAISC_INIT_t *Init_Reg);
void Usart_Init_Interrupt(uint32_t group, uint32_t cmd, uint32_t type);
void Usart_CMD(uint32_t group, uint32_t cmd);
void Usart_Send_Data(uint32_t group, char data);
uint16_t Usart_Receive_Data(uint32_t group);
uint32_t Usart_StatusFlag(uint32_t group, uint32_t flag);

#endif