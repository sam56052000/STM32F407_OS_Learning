/* 
*	Reset Handler
*	
*	Memory	: 0x00000004
*/
.syntax unified
.cpu cortex-m4
.fpu softvfp
.thumb

/*
*	Global Define
*/
.global	_VECTOR_RESET

/*	
*	Linker Script Variable
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

.section	.text._VECTOR_RESET
.type		_VECTOR_RESET, %function

_VECTOR_RESET:
/*
*	Golobal Data Initialzation
*/
	movs	r1, #0
	b		CopyDataLoopInit
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
	bcc		CopyDataInit

/*
*	Fill Zero Bss data section
*/
	ldr		r2, =_sbss
	b		FillZeroBssLoop
FillZeroBss:
	movs	r3, #0
	str		r3, [r2], #4
FillZeroBssLoop:
	ldr		r3, =_ebss
	cmp		r2, r3
	bcc		FillZeroBss

/*
*	Call the Application entry Point
*/
	bl		Platform_Boot
	bl		main

.size		_VECTOR_RESET, .-_VECTOR_RESET
