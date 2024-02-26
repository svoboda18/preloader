OUTPUT_ARCH(arm)

ENTRY(_start)

romBase = 0x00201000;
bss1Base = 0x00102000;
bss2Base = 0x00108C00;
stackBase = 0x00200000;

MEMORY {
    bss1 : ORIGIN = 0x00102000, LENGTH = 0x6C00
    rom : ORIGIN = 0x00201000, LENGTH = 0x3A000
    bss2 : ORIGIN = 0x00108C00, LENGTH = 0x5000
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

    __boot_end = .;

    /* bss1 region */
    . = bss1Base;
    .bss1 ALIGN(16) : {
        _bss1_start = .;
        *print.o (.bss.log_sram_buf)
        *efi.o (.bss.gpt_sram_buf)
        *efi.o (.bss.gpt_sram_crc32_table)
        *efi.o (.bss.gpt_sram_part_info)
        *efi.o (.bss.gpt_sram_part_meta_info)
        *dramc_*.o (.bss .bss.* COMMON)
        /* make _bss1_end as 4 bytes alignment */
        . = ALIGN(4);
        _bss1_end = .;
    }
    ASSERT((SIZEOF(.bss1) < LENGTH(bss1)), "Error: .bss1 over size!")

    /* stack */
    . = stackBase;
    .stack ALIGN(4) : {
        _stack_start = .;
        *(.bss.sys_stack)
        _stack_end = .;
    }

    /* bss2 region */
    . = bss2Base;
    .bss2 ALIGN(16) : {
        _bss2_start = .;
        *(.bss)
        *(.bss.*)
        *(COMMON)
        /* make _bss2_end as 4 bytes alignment */
        . = ALIGN(4);
        _bss2_end = .;
    }
    ASSERT((SIZEOF(.bss2) < LENGTH(bss2)), "Error: .bss2 over size!")

    /* Clang needs .ARM.exidx, but we do not use it for debug. */
    /DISCARD/ : {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    }
}
