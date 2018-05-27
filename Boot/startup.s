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
*	Define Interrupt Vector
*/
.global  Reset_Vector
.section  .isr_vector,"a",%progbits
.type  Reset_Vector, %object
    
Reset_Vector:
/*
* Internal Interrupt Vector
*/
  .word		_estack                        // Main Stack Top Value
  .word		_VECTOR_RESET                  // Reset Handler
  .word		0                              // NMI Handler
  .word		0                              // Hw Fault Handler
  .word		0                              // Memory Manage Fault Handler
  .word		0                              // Bus Fault Handler
  .word		0                              // Usage Fault Handler
  .word		0                              //
  .word		0                              //
  .word		0                              //
  .word		0                              //
  .word		0                              //
  .word		0                              //
  .word		0                              //
  .word		0                              //
  .word		Systick_Interrupt_Handler      // Systick Interrupt Handler

/*
* External Interrupt Vector
*/
  .word   0
  .word   0
  .word   0
  .word   0
  .word   0
  .word   0
  .word   0
  .word   0
  .word   0
  .word   0
  .word   0
  .word   0
  .word   0
  .word   0
  .word   0
  .word   0
  .word   0
  .word   0
  .word   0
  .word   0
  .word   0
  .word   0
  .word   0
  .word   0
  .word   0
  .word   0
  .word   0
  .word   0
  .word   0
  .word   0
  .word   0
  .word   0
  .word   0
  .word   0
  .word   0
  .word   0
  .word   0
  .word   0
  .word   0
  .word   USART3_IRQHandler                 // Usart3 IRQ Handler

.size  Reset_Vector, .-Reset_Vector
