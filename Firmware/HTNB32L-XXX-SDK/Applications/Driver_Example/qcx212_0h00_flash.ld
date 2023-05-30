
/* Entry Point */
ENTRY(Reset_Handler)

/* Specify the memory areas */
MEMORY
{
  LOAD_IRAM(rwx)                   : ORIGIN = 0x00004000, LENGTH = 32K
  LOAD_DRAM_SHARED(rwx)            : ORIGIN = 0x0000C000, LENGTH = 156K
  LOAD_DRAM_BSP(rwx)               : ORIGIN = 0x00041000, LENGTH = 8K
  FLASH_APP (rx)                   : ORIGIN = 0x00820000, LENGTH = 2560K        /* Use only the first bank */
}


/* Define output sections */
SECTIONS
{
  . = ORIGIN(FLASH_APP);
  Image$$UNLOAD_IROM$$Base = .;
  .isr_vector :
  {
    . = ALIGN(8);
    KEEP(*(.isr_vector)) /* Startup code */
    . = ALIGN(8);
  } >FLASH_APP AT>FLASH_APP

  /* The program code and other data goes into FLASH */
  .text :
  {
    . = ALIGN(8);
    *(EXCLUDE_FILE(*flash_qcx212.o *qspi_qcx212.o *cache_qcx212.o) .text*)
    *(.eh_frame)
    KEEP (*(.init))
    KEEP (*(.fini))
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
  } >FLASH_APP AT>FLASH_APP

  .unload_nocache :
  {
   . = ALIGN(128);
   Image$$UNLOAD_NOCACHE$$Base = .;

   *cache_qcx212.o (.text*)

    . = ALIGN(8);

    _etext = .;        /* define a global symbols at end of code */

  }>FLASH_APP AT>FLASH_APP

  /* used by the startup to initialize data */

  Load$$LOAD_IRAM$$Base = LOADADDR(.load_iram);
  .load_iram ORIGIN(LOAD_IRAM):
  {
    . = ALIGN(4);
    Image$$LOAD_IRAM$$Base = .;
    *(.ramCode)
    *flash_qcx212.o(.text*)
     *qspi_qcx212.o(.text*)

    . = ALIGN(4);
    Image$$LOAD_IRAM$$Limit = .;
  } >LOAD_IRAM AT>FLASH_APP
  Load$$LOAD_DRAM_BSP$$Base = LOADADDR(.load_dram_bsp);
  .load_dram_bsp ORIGIN(LOAD_DRAM_BSP) :
  {
    . = ALIGN(4);
    Image$$LOAD_DRAM_BSP$$Base = .;
    Image$$LOAD_DRAM_BSP$$Limit = .;
  } >LOAD_DRAM_BSP AT>FLASH_APP
  Load$$LOAD_DRAM_SHARED$$Base = LOADADDR(.load_dram_shared);
  .load_dram_shared ORIGIN(LOAD_DRAM_SHARED):
  {
    . = ALIGN(4);
    Image$$LOAD_DRAM_SHARED$$Base = .;
    *(.data*)
    . = ALIGN(4);
    Image$$LOAD_DRAM_SHARED$$Limit = .;

    Image$$LOAD_DRAM_SHARED$$ZI$$Base = .;
    *(.bss*)
    *(.flashbackupdata)
    . = ALIGN(4); /* stack should be 4 byte align */
    Image$$LOAD_DRAM_SHARED$$ZI$$Limit = .;
    __stack_start = .;
    *(.stack)
    __stack_end = .;

    . = ALIGN(4);
    _heap_memory_start = .;
    _heap_memory_end = LENGTH(LOAD_DRAM_SHARED) - _heap_memory_start;

  } >LOAD_DRAM_SHARED AT>FLASH_APP

}

GROUP(
    libgcc.a
    libc.a
    libm.a
 )