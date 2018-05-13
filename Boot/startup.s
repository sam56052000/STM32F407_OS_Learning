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
*	
*	1. Fetch Inital SP Value
*	2. Fetch Reset PC Value
*/
.global  Reset_Vector
.section  .isr_vector,"a",%progbits
.type  Reset_Vector, %object
    
Reset_Vector:
  .word  _estack
  .word  _VECTOR_RESET

.size  Reset_Vector, .-Reset_Vector
