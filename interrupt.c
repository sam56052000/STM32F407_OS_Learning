#include "interrupt.h"

void Systick_Interrupt_Handler(void)
{
	__asm__ __volatile__
	(
		"mrs %[now], PSP\n"
		: [now]"=r"(current->sp)
		: 
		:
	);

	// printk("--------before:%x\n", current->sp);
	// printk("%x\n", *(current->sp));
	// printk("%x\n", *(current->sp+1));
	// printk("%x\n", *(current->sp+2));
	// printk("%x\n", *(current->sp+3));
	// printk("%x\n", *(current->sp+4));
	// printk("%x\n", *(current->sp+5));
	// printk("%x\n", *(current->sp+6));
	// printk("%x\n", *(current->sp+7));

	uint32_t new_sp;

	__asm__ __volatile__
	(
		"bl common_schedule\n"
		"msr PSP,r0\n"
		"mrs %[new_sp], PSP\n"
		"mov lr, #0xFFFFFFFD\n"
		: [new_sp]"=r"(new_sp)
		: 
		: "r0"
	);

	// printk("--------after:%x\n", current->sp);
	// printk("%x\n", *(current->sp));
	// printk("%x\n", *(current->sp+1));
	// printk("%x\n", *(current->sp+2));
	// printk("%x\n", *(current->sp+3));
	// printk("%x\n", *(current->sp+4));
	// printk("%x\n", *(current->sp+5));
	// printk("%x\n", *(current->sp+6));
	// printk("%x\n", *(current->sp+7));

	LED_TOGGLE(PIN_LED5);
}

void USART3_IRQHandler(void)
{
    usart3_busy = 1;

    char Receive_Data = (char)(Usart_Receive_Data(USART3));

    usart_FIFO[usart_front++] = Receive_Data;
    
    usart3_busy = 0;
}

void Syscall_Interrupt_Handler(void)
{
	__asm__ __volatile__
	(
		"ldr r0, [sp]\n"
		"b Syscall_Handler\n"
	);
}

uint32_t *Task_Handler(void)
{
	// uint32_t *now_psp;

	// __asm__ __volatile__
	// (
	// 	"mrs %[psp], PSP\n"
	// 	: [psp]"=r"(now_psp)
	// 	: 
	// 	:
	// );
	// current->sp = now_psp;

	// uint32_t *sp = common_schedule()->sp;

	// printk("%x\n", now_psp);
	// printk("%x\n", sp);

	// printk("---------------------\n");
	// printk("%x\n", now_psp);
	// printk("%x\n", sp);
	// printk("%x\n", Task_Function);
	// printk("---------------------\n");
	// printk("%x, %x\n", *(now_psp), *(sp));
	// printk("%x, %x\n", *(now_psp+1), *(sp+1));
	// printk("%x, %x\n", *(now_psp+2), *(sp+2));
	// printk("%x, %x\n", *(now_psp+3), *(sp+3));
	// printk("%x, %x\n", *(now_psp+4), *(sp+4));
	// printk("%x, %x\n", *(now_psp+5), *(sp+5));
	// printk("%x, %x\n", *(now_psp+6), *(sp+6));
	// printk("%x, %x\n", *(now_psp+7), *(sp+7));

	// printk("%x\n", *(sp+6));
	// printk("%x\n", Task_Function);
	// *(sp+5) = *(now_psp+5);
	// *(sp+6) = (uint32_t)Task_Function;
	// *(sp+7) = *(now_psp+7);
	// *sp = 0xff;
	// *(sp+8) = *(now_psp+8);
	// *(sp+9) = *(now_psp+9);

	return 0;
}

void UART_Queue_Init(void)
{
    usart_front = 0;
}