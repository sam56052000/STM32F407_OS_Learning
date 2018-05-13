/*
*   Entry Point
*/
ENTRY(_VECTOR_RESET)

/*
*   Highest address of the user mode stack
*
*   End of 128K RAM on AHB Bus
*/
_estack = 0x20020000;

/*
*   User Mamory Size
*/
min_user_stack_size = 0x400;
min_user_heap_size = 0x0;

/*
*   Specify the memory areas
*/
MEMORY
{
    FLASH(rx)   : ORIGIN = 0x08000000, LENGTH = 1024K
    RAM(xrw)    : ORIGIN = 0x20000000, LENGTH = 192K
}

/*
*   Define Output Section
*/
SECTIONS
{
    /*
    *   Startup Section
    */
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } >FLASH

    /*
    *   Text Section
    */
    .text :
    {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
    } >FLASH

    /*
    *   _exit for compiler
    */
    _exit = .;

    /*
    *   Use by the startup to initialize data
    *
    *   _sdata, _edata for startup code
    */
    _sidata = .;

    .data : AT ( _sidata )
    {
        . = ALIGN(4);
        _sdata = .;
        *(.data)
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } >RAM

    /*
    *   Uninitialized data section
    *
    *   __bss_start__, __bss_end__ for compiler
    *
    *   _sbss, _ebss for startup code
    */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        __bss_start__ = _sbss;
        *(.bss)
        *(.bss*)
        . = ALIGN(4);
        _ebss = .;
        __bss_end__ = _ebss;
    } >RAM

    /*
    *   User Heap and User Stack
    */
    ._user_heap_stack :
    {
        . = ALIGN(4);
        PROVIDE ( end = . );
        . = . + min_user_stack_size;
        . = . + min_user_heap_size;
        . = ALIGN(4);
    } >RAM

    /*
    *   Remove information from the standard libraries
    */
    /DISCARD/ :
    {
        libc.a ( * )
        libm.a ( * )
        libgcc.a ( * )
    }
}