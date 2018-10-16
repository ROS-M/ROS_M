
/* Memory Spaces Definitions */
MEMORY
{
  RAM      (RWX) : ORIGIN = 0x20000000+0, LENGTH = 128K-0
  CCMRAM   (RWX) : ORIGIN = 0x10000000+0, LENGTH = 64K-0
  FLASH    (RX)  : ORIGIN = 0x08000000+0, LENGTH = 512K-0
}

/* higher address of the user mode stack */
_estack = ORIGIN(RAM)+LENGTH(RAM);

/* There will be a link error if there is not this amount of RAM free at the end. */
_Minimum_Stack_Size = 0x100 ;

/* Check valid alignment for VTOR */
ASSERT(ORIGIN(FLASH) == ALIGN(ORIGIN(FLASH), 0x80), "Start of memory region flash not aligned for startup vector table");

/* Sections Definitions */

SECTIONS
{
    /* the program code is stored in the .text section, which goes to Flash */
    .text :
    {
        . = ALIGN(0x80); /* PM0056, Rev.1 (4/2009), 4.3.3 */
        _isr_vectorsflash_offs = . - 0x08000000;
        KEEP(*(.isr_vector))
        . = ALIGN(4);
        *(.text)                   /* code */
        *(.text.*)                 /* remaining code (-ffunction-sections)*/
        *(.stub .gnu.linkonce.t.*)
        *(.rodata .rodata.* .gnu.linkonce.r.*) /* read-only data (constants) */
        *(.glue_7 .glue_7t)        /* redundant for thumb2 ?! */
        . = ALIGN(4);
        _etext = . + 0x970;
        /* This is used by the startup in order to initialize the .data secion */
        _sidata = _etext;
    } >FLASH

    /* This is the initialized data section
    The program executes knowing that the data is in the RAM
    but the loader puts the initial values in the FLASH (inidata).
    It is one task of the startup to copy the initial values from FLASH to RAM. */
    .data  : AT ( _sidata )
    {
        . = .;
        . = ALIGN(4);
        /* This is used by the startup in order to initialize the .data section */
        _sdata = . ;

        . = ALIGN(0x80);
        _isr_vectorsram_offs = . - 0x20000000;
        KEEP(*(.isr_vectorsram))
        . = ALIGN(4);

        *(.ramfunc*)
        . = ALIGN(4);
        *(.data .data.* .gnu.linkonce.d.*)
        . = ALIGN(4);
        /* This is used by the startup in order to initialize the .data section */
        _edata = . ;
    } >RAM

    /* This is the uninitialized data section */
    .bss (NOLOAD):
    {
        . = ALIGN(4);
        /* This is used by the startup in order to initialize the .bss section */
        _sbss = .;
        *(.bss .bss.*)
        *(COMMON)
        . = ALIGN(4);
         /* This is used by the startup in order to initialize the .bss section */
         _ebss = . ;
    } >RAM

    PROVIDE ( end = _ebss );
    PROVIDE ( _end = _ebss );
    
    /* This is the user stack section 
    This is just to check that there is enough RAM left for the User mode stack
    It should generate an error if it's full.
     */
    ._usrstack (NOLOAD):
    {
        . = ALIGN(4);
        _susrstack = . ;
        . = . + _Minimum_Stack_Size ;
        . = ALIGN(4);
        _eusrstack = . ;
    } >RAM
}

/* default stack size(s). These are used by the startup in order to allocate stacks for 
the different modes. */
__Stack_Size = 1024 ;

PROVIDE ( _Stack_Size = __Stack_Size ) ;
__Stack_Init = _estack - __Stack_Size ;

/*"PROVIDE" allows to easily override these values from an object file or the commmand line.*/
PROVIDE ( _Stack_Init = __Stack_Init ) ;
