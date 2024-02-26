OUTPUT_ARCH(arm)

ENTRY(_start)

romBase = 0x00201000;
bss1Base = 0x00100000;
bss2Base = 0x00267000;
stackBase = 0x00200000;

/* log_sram_buf + bss1 should be less than 40 KB.*/
MEMORY {
    bss1 : ORIGIN = 0x00100000, LENGTH = 0xA000
    rom : ORIGIN = 0x00201000, LENGTH = 0x35000
    bss2 : ORIGIN = 0x00267000, LENGTH = 0x14000
    stack : ORIGIN = 0x00200000, LENGTH = 0xC00
}

SECTIONS {
    /* rom region */
    . = romBase;
    .start ALIGN(4) : {
        *(.text.start)
    }

    . = romBase + 0x01FC;
    .rom_info ALIGN(4) : {
        *(.data.rom_info)
    }

    .text ALIGN(4) : {
        *(.text)
        *(.text.*)
    }

    .rodata ALIGN(4) : {
        *(.rodata)
        *(.rodata.*)
    }

    .data ALIGN(4) : {
        *(.data)
        *(.data.*)
    }

    .got ALIGN(4) : {
        *(.got)
        *(.got.*)
    }

	.secbuf ALIGN(4) : {
		_secbuf_start = .;
		*(.secbuf)
		_secbuf_end = .;
	}

    __boot_end = .;

    /* stack region */
    . = stackBase;
    .stack ALIGN(4) : {
        _stack_start = .;
        *(.stack)
        _stack_end = .;
    }

    /* bss2 region */
    . = bss2Base;
    .bss2 ALIGN(16) : {
        _bss2_start = .;
        *(EXCLUDE_FILE(*circbuf.o *dramc_*.o *usb*.o *pcie.o) .bss)
        *(EXCLUDE_FILE(*circbuf.o *dramc_*.o *usb*.o *pcie.o) .bss.*)
        *(EXCLUDE_FILE(*circbuf.o *dramc_*.o *usb*.o *pcie.o) COMMON)
        /* make _bss_end as 4 bytes alignment */
        . = ALIGN(4);
        _bss2_end = .;
    }

    /* bss1 region */
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
    }
}
