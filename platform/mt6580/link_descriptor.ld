OUTPUT_ARCH(arm)

ENTRY(_start)

romBase = 0x00201000;
ramBase = 0x00102180;
pll_init_baseaddr = 0x00100100;

MEMORY {
    ram : ORIGIN = ramBase, LENGTH = 0xBA80
    rom : ORIGIN = romBase, LENGTH = 0x1F000
}

SECTIONS {
    /* rom region */
    . = romBase;
    __boot_start = .;
    .start ALIGN(4) : {
        *(.text.start)
    }

    . = romBase + 0x01FC;
    .rom_info ALIGN(4) : {
    	*(.data.rom_info)
    }

    .text ALIGN(4) : {
        *(.text)
/*        *(.text.*)     for get .text.mt_pll_init   */
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

    .pll_text pll_init_baseaddr : AT (ADDR(.got) + SIZEOF(.got))
    {
        Image$$PLL_INIT$$Base = . ;
        *(.text.mt_pll_init)
        Image$$PLL_INIT$$Limit = . ;
    }

    __load_start_pll_text = LOADADDR (.pll_text);
    __load_stop_pll_text = LOADADDR (.pll_text) + SIZEOF (.pll_text);

    __boot_end = .;

    /* ram region */
    . = ramBase;
    .bss ALIGN(16) : {
        _bss_start = .;
        *(.bss)
        *(.bss.*)
        *(COMMON)
        /* make _bss_end as 4 bytes alignment */
        . = ALIGN(4);
        _bss_end = .;
    }

	.secbuf ALIGN(4) : {
		_secbuf_start = .;
		*(.secbuf)
		_secbuf_end = .;
	}

}
