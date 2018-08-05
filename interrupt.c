#include "interrupt.h"

void Systick_Interrupt_Handler(void)
{
	//printk("Start thread Changer\n");

	// uint32_t *now_psp;

	// __asm__ __volatile__
	// (
	// 	"mrs %[psp], PSP\n"
	// 	: [psp]"=r"(now_psp)
	// 	: 
	// 	:
	// );

	// printk("******************\n");
	// printk("%x\n", now_psp);
	// printk("------------------\n");
	// printk("%x\n", *now_psp);
	// printk("%x\n", *(now_psp+1));
	// printk("%x\n", *(now_psp+2));
	// printk("%x\n", *(now_psp+3));
	// printk("%x\n", *(now_psp+4));
	// printk("%x\n", *(now_psp+5));
	// printk("%x\n", *(now_psp+6));
	// printk("%x\n", *(now_psp+7));
	// printk("%x\n", *(now_psp+8));
	// printk("------------------\n");

	//uint32_t *psp = (uint32_t *)(current->sp);//(uint32_t *)(common_schedule()->sp);
	// printk("%x\n", psp);
	// printk("%x\n", *psp);
	// printk("%x, %x\n", *(psp+6), Task_Function);

	//*psp = 0xFF;
	// *(psp+1) = *(now_psp+1);
	// *(psp+2) = *(now_psp+2);
	// *(psp+3) = *(now_psp+3);
	// *(psp+4) = *(now_psp+4);
	//printk("%x\n", *(psp+5));
	//*(psp+5) = *(now_psp+5);
	//printk("%x\n", *(now_psp+5));
	//*(psp+6) = Task_Function;
	
	//*(psp+7) = *(now_psp+7);
	
	// *(psp+8) = *(now_psp+8);
	// *(psp+9) = *(now_psp+9);
	// *(psp+10) = *(now_psp+10);

	// printk("------------------\n");
	// printk("%x\n", psp);
	// printk("------------------\n");
	// printk("%x\n", *psp);
	// printk("%x\n", *(psp+1));
	// printk("%x\n", *(psp+2));
	// printk("%x\n", *(psp+3));
	// printk("%x\n", *(psp+4));
	// printk("%x\n", *(psp+5));
	// printk("%x\n", *(psp+6));
	// printk("%x\n", *(psp+7));
	// printk("%x\n", *(psp+8));
	// printk("------------------\n");

	__asm__ __volatile__
	(
		"mrs %[now], PSP\n"
		: [now]"=r"(current->sp)
		: 
		: "r0"
	);

	//printk("before:%x\n", current->sp);

	__asm__ __volatile__
	(
		"bl common_schedule\n"
		"msr PSP,r0\n"
		"mov lr, #0xFFFFFFFD\n"
		:
		: 
		: "r0"
	);

	printk("SP ->%x\n", current->sp);
	
	printk("%x\n", *(current->sp));
	// printk("%x\n", *(current->sp+1));
	// printk("%x\n", *(current->sp+2));
	// printk("%x\n", *(current->sp+3));
	// printk("%x\n", *(current->sp+4));
	// printk("%x\n", *(current->sp+5));
	// printk("%x\n", *(current->sp+6));
	// printk("%x\n", *(current->sp+7));

	// *(current->sp+6) = (uint32_t)Task_Function;

	LED_TOGGLE(PIN_LED4);
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