#include "stm32f4_usart.h"

void Usart_Init_Register_Setting(uint32_t group, USART_BAISC_INIT_t *Init_Reg)
{
    uint32_t *Reg1 = (uint32_t *)(group + USART_REG_CTRL_1_OFFSET);
    uint32_t *Reg2 = (uint32_t *)(group + USART_REG_CTRL_2_OFFSET);
    uint32_t *Reg3 = (uint32_t *)(group + USART_REG_CTRL_3_OFFSET);
    uint32_t *baud = (uint32_t *)(group + USART_REG_BUADRATE_OFFSET);

    //
    // Clear Register Data
    // 
    // Ctrl Register 1 : M, PCE, PS, TE, RE
    // Ctrl Register 2 : Stop
    // Ctrl Register 3 : RTSE, CTSE
    //
    *Reg1 &= 0xFFFFF9F3;
    *Reg2 &= 0xFFFFCFFF;
    *Reg3 &= 0xFFFFFCFF;

    //
    // Init Register Data
    //
    *Reg1 |= Init_Reg -> WordLength;
    *Reg1 |= Init_Reg -> Pairty;
    *Reg1 |= Init_Reg -> Mode;
    *Reg2 |= Init_Reg -> StopBits;
    *Reg3 |= Init_Reg -> Hw_Ctrl;

    //
    // Calculate Baudrate Register
    //
    uint32_t CLK = 0, Baud_I, Baud_F;

    if( (group == USART1) || (group == USART6) )
    {
        CLK = APB1Clock;
    }
    else
    {
        CLK = APB2Clock;
    }

    Baud_I = CLK / (Init_Reg -> Baudrate * 16);
    Baud_F = CLK - ( Baud_I *(Init_Reg -> Baudrate * 16));
    Baud_F /= Init_Reg -> Baudrate;

    *baud = (Baud_I << 4) | Baud_F;
}

void Usart_Init_Interrupt(uint32_t group, uint32_t cmd, uint32_t type)
{
    uint32_t *Reg1 = (uint32_t *)(group + USART_REG_CTRL_1_OFFSET);

    *Reg1 &= ~type;

    if(cmd == ENABLE)
    {
        *Reg1 |= type;
    }
}

void Usart_CMD(uint32_t group, uint32_t cmd)
{
    uint32_t *Reg1 = (uint32_t *)(group + USART_REG_CTRL_1_OFFSET);

    if(cmd == USART_ENABLE)
    {
        *Reg1 |= USART_REG_UE;
    }
    else
    {
        *Reg1 &= ~USART_REG_UE;
    }
}

uint32_t Usart_StatusFlag(uint32_t group, uint32_t flag)
{
    volatile uint32_t *status = (volatile uint32_t *)(group + USART_REG_STATUS_OFFSET);

    if( (*status & flag) != RESET)
    {
        return SET; 
    }
    else
    {
        return RESET;
    }
}

void Usart_Send_Data(uint32_t group, char data)
{
    uint16_t *usart_data = (uint16_t *)(group + USART_REG_DATA_OFFSET);

    while(Usart_StatusFlag(group, USART_FLAG_TXE) == RESET);

    *usart_data = (data & (uint16_t)0x01FF);
}

uint16_t Usart_Receive_Data(uint32_t group)
{
    uint16_t *usart_data = (uint16_t *)(group + USART_REG_DATA_OFFSET);

    while(Usart_StatusFlag(group, USART_FLAG_RXNE) == RESET);

    return *usart_data;
}