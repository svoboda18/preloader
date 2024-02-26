OUTPUT_ARCH(arm)

ENTRY(_start)

romBase = 0x00201000;
bss1Base = 0x00100000;
bss2Base = 0x00114C00;
stackBase = 0x0011A400;

/* log_sram_buf + bss1 should be less than 40 KB.*/
MEMORY {
    bss1 : ORIGIN = 0x00100000, LENGTH = 0xA000
    rom : ORIGIN = 0x00201000, LENGTH = 0x3A000
    bss2 : ORIGIN = 0x00114C00, LENGTH = 0x5800
    stack : ORIGIN = 0x0011A400, LENGTH = 0xC00
}

SECTIONS {
    . = romBase;
    .start ALIGN(4) : {
        *(.text.start)
    } >rom

    . = romBase + 0x01FC;
    .rom_info ALIGN(4) : {
        *(.data.rom_info)
    } >rom

    .text ALIGN(4) : {
        *(.text)
        *(.text.*)
    } >rom

    .rodata ALIGN(4) : {
        *(.rodata)
        *(.rodata.*)
    } >rom

    .data ALIGN(4) : {
        *(.data)
        *(.data.*)
    } >rom

    .got ALIGN(4) : {
        *(.got)
        *(.got.*)
    } >rom

	.secbuf ALIGN(4) : {
		_secbuf_start = .;
		*(.secbuf)
		_secbuf_end = .;
	} >rom

    __boot_end = .;

    . = stackBase;
    .stack ALIGN(4) : {
        _stack_start = .;
        *(.stack)
        _stack_end = .;
    } >stack

    . = bss2Base;
    .bss2 ALIGN(16) : {
        _bss2_start = .;
        *(EXCLUDE_FILE(*circbuf.o *dramc_*.o *usb*.o *pcie.o) .bss)
        *(EXCLUDE_FILE(*circbuf.o *dramc_*.o *usb*.o *pcie.o) .bss.*)
        *(EXCLUDE_FILE(*circbuf.o *dramc_*.o *usb*.o *pcie.o) COMMON)
        /* make _bss2_end as 4 bytes alignment */
        . = ALIGN(4);
        _bss2_end = .;
    } >bss2

    . = bss1Base;
    .bss1 ALIGN(16) : {
        _bss1_start = .;
        *print.o (.log_sram_buf)
        *(*circbuf.o *dramc_*.o *usb*.o *pcie.o .bss)
        *(*circbuf.o *dramc_*.o *usb*.o *pcie.o .bss.*)
        *(*circbuf.o *dramc_*.o *usb*.o *pcie.o COMMON)
        /* make _bss_end as 4 bytes alignment */
        . = ALIGN(4);
        _bss1_end = .;
    } >bss1
}
