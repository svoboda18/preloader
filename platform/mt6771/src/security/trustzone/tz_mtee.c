/*
 * Copyright (c) 2019 Mediatek Inc.
 * All rights reserved
 *
 */

#include <dram_buffer.h>
#include <typedefs.h>
#include <tz_mem.h>
#include <mt_uart.h>
#include "tz_init.h"
#include "tz_mtee.h"
#include "tz_platform_mtee.h"

#if CFG_BOOT_ARGUMENT_BY_ATAG
extern unsigned int g_uart;
#endif

#define TEE_MEM_ALIGNMENT (0x100000)    // 256MB Alignment
#define TEE_MEM_LIMITATION (0xC0000000)   // 2GB

#if defined(MTK_SEC_VIDEO_PATH_SUPPORT) || defined(CFG_GZ_SUPPORT)
#define MTEE_RESERVED_SEC_MEM_SIZE (3*1024*1024) /* 3MB */
#else
#define MTEE_RESERVED_SEC_MEM_SIZE (0) /* 0MB */
#endif

void mtee_boot_param_prepare(u32 param_addr, u32 tee_entry, u32 mtee_sec_dram_size, u64 dram_base,
                             u64 dram_size) {
    tee_arg_t_ptr teearg = (tee_arg_t_ptr)param_addr;
    u32 mtee_log_port = 0;

    if (teearg == NULL) {
        print("mtee boot param argument is NULL!\n");
        return;
    }

    memset(teearg, 0, sizeof(tee_arg_t));

    /* Prepare TEE boot parameters */
    teearg->magic = MTEE_BOOTCFG_MAGIC;             /* MTEE magic number */
    teearg->length = sizeof(tee_arg_t);             /* MTEE argument block size */
    teearg->version = TEE_ARGUMENT_VERSION;
    teearg->secDRamBase = tee_entry;                /* Secure DRAM base address */
    teearg->secDRamSize = (mtee_sec_dram_size - MTEE_RESERVED_SEC_MEM_SIZE);       /* Secure DRAM size */
    print("%s secDRamSize: 0x%x\n", __func__, teearg->secDRamSize);

    /* SSI Reserve */
    teearg->total_number_spi = 256;                 /* Platform Support total 256 SPIs and 32 PPIs */

    /* GIC parameters */
    teearg->gic_distributor_base = GIC_BASE;
#if GIC_VERSION == 0x02
    teearg->gic_cpuinterface_base = GIC_CPU;
#else
    teearg->gic_redistributor_base = GICR_BASE;
#endif
    teearg->gic_version = GIC_VERSION;

#if CFG_BOOT_ARGUMENT_BY_ATAG
    mtee_log_port = g_uart;
    print("%s teearg : CFG_BOOT_ARGUMENT_BY_ATAG 0x%x\n", __func__, g_uart);
#elif CFG_BOOT_ARGUMENT && !CFG_BOOT_ARGUMENT_BY_ATAG
    mtee_log_port = bootarg.log_port;
    print("%s teearg : CFG_BOOT_ARGUMENT 0x%x\n", __func__, bootarg.log_port);
#else
    mtee_log_port = CFG_UART_LOG;
    print("%s teearg : log port by prj cfg 0x%x\n", __func__, CFG_UART_LOG);
#endif
}

void mtee_key_param_prepare(u32 param_addr,u8 *hwuid, u32 hwid_size,u8 *hrid, u32 hrid_size, u8 * rpmb_key)
{
}

/**************************************************************************
 * TEE FUNCTIONS
 **************************************************************************/
u32 mtee_get_reserved_tee_address(u32 tee_size)
{
    u32 tee_reserved_address = 0;

    print("Align_tee_addr: 0x%lx, limit_addr: 0x%lx\n", TEE_MEM_ALIGNMENT, TEE_MEM_LIMITATION);
    tee_reserved_address = (u32)mblock_reserve_ext(&bootarg.mblock_info,
            (u64)(tee_size), (u64)TEE_MEM_ALIGNMENT, (u64)TEE_MEM_LIMITATION, 0, TEE_RESERVED_NAME);
    print("Reserved_tee_addr: 0x%x, reserved_size: 0x%x\n", tee_reserved_address, tee_size);
/*
    if (tee_reserved_address != (TEE_MEM_LIMITATION - TEE_MEM_ALIGNMENT)) {
        print("Fail to allocate tee address: 0x%x\n", (u32)(TEE_MEM_LIMITATION - TEE_MEM_ALIGNMENT));
        BUG_ON(1);
    }

    print("Reserved_tee_addr: 0x%x, reserved_size: 0x%x\n", tee_reserved_address, tee_size);
*/
    return tee_reserved_address;
}

u32 mtee_get_reserved_secmem_address(u32 tee_enty, u32 tee_size)
{
    if ((tee_enty + tee_size) >= MTEE_RESERVED_SEC_MEM_SIZE)
        return tee_enty + tee_size - MTEE_RESERVED_SEC_MEM_SIZE;
    else {
        print("Fail to get secure memory address: 0x%x\n",
            (u32)(tee_enty + tee_size));
        return 0;
    }
}

DECLARE_TEE_MODULE("MTEE", mtee_get_reserved_tee_address, mtee_get_reserved_secmem_address, mtee_boot_param_prepare, mtee_key_param_prepare);

