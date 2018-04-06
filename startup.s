 /*	StartUp Setting
*
*	syntax	: Instruction Syntax
*	cpu		: Target Processor
*	fpu		: Select Floating-Pointer Uinit
*	thumb	: Type of Instruction Generatinig
*/
.syntax unified
.cpu cortex-m4
.fpu softvfp
.thumb

/*
*	Global Define
*/
.global  g_pfnVectors

/*	Linker Script Variable
*
*	_sidata	: start address of data section with initialzation value
*	_sdata	: start address of data section
*	_edata	: end address of data section
*	_sbss	: start address of bss section
*	_ebss	: end address of bss section
*/
.word _sidata
.word _sdata
.word _edata
.word _sbss
.word _ebss

/*
*	Reset_Handler
*
*	When System Reset, run this program.
*/
.section	.text.Reset_Handler
.weak		Reset_Handler
.type		Reset_Handler, %function

Reset_Handler:
	movs	r1, #0
	b		CopyDataLoopInit

/*
*	Golobal Data Initialzation
*/
CopyDataInit:
	ldr		r3, =_sidata
	ldr		r3, [r3, r1]
	str		r3, [r0, r1]
	adds	r1, r1, #4

CopyDataLoopInit:
	ldr		r0, =_sdata
	ldr		r3, =_edata
	adds	r2, r0, r1
	cmp		r2, r3
	bcc		CopyDataInit	/*Branch if C Clear*/
	ldr		r2, =_sbss
	b		FillZeroBssLoop

/*
*	Fill Zero Bss data section
*/
FillZeroBss:
	movs	r3, #0
	str		r3, [r2], #4

FillZeroBssLoop:
	ldr		r3, =_ebss
	cmp		r2, r3
	bcc		FillZeroBss

/*
*	Enable Hardware Float Process Unit
*/
	ldr		r0, =0xE000ED88
	ldr		r1, [r0]
	orr		r1, r1, #(0xF << 20)
	str		r1, [r0]

/*
*	Call the Application entry Point
*/
	bl  	SystemInit
	bl		main
	bx		lr

.size		Reset_Handler, .-Reset_Handler

/*
*	Default_Handler
*
*	When System is unexcept state, run this program.
*/
.section .text.Default_Handler

Default_Handler:

InfiniteLoop:
	b		InfiniteLoop

.size Default_Handler, .-Default_Handler

/*
*	Define Interrupt Vector
*	
*	1. Fetch Inital SP Value
*	2. Fetch Reset PC Value
*/
.section  .isr_vector,"a",%progbits
.type  g_pfnVectors, %object
.size  g_pfnVectors, .-g_pfnVectors
    
g_pfnVectors:
  .word  _estack
  .word  Reset_Handler
 