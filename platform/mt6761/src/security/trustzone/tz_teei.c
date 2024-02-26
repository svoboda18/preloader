/*
 * Copyright (c) 2015-2016 MICROTRUST Incorporated
 * All rights reserved
 *
 * This file and software is confidential and proprietary to MICROTRUST Inc.
 * Unauthorized copying of this file and software is strictly prohibited.
 * You MUST NOT disclose this file and software unless you get a license
 * agreement from MICROTRUST Incorporated.
 */

#include <dram_buffer.h>
#include <imsg_log.h>
#include <typedefs.h>
#include <tz_mem.h>
#include <tz_teei.h>
#include <mt_uart.h>
#include <utos_version.h>
#include "tz_platform_teei.h"

/* #define TEEI_DEBUG 1 */
#if CFG_BOOT_ARGUMENT_BY_ATAG
extern unsigned int g_uart;
#elif CFG_BOOT_ARGUMENT && !CFG_BOOT_ARGUMENT_BY_ATAG
#define bootarg g_dram_buf->bootarg
#endif

void CONFIG_ISEE_DEVS(tee_dev_t_ptr devs, u32 type, u64 addr, u32 irq, u32 dapc_num, u32 param0)
{
	((tee_dev_t_ptr)devs)->dev_type = type;
	((tee_dev_t_ptr)devs)->base_addr = addr;
	((tee_dev_t_ptr)devs)->intr_num = irq;
	((tee_dev_t_ptr)devs)->apc_num = dapc_num;
	((tee_dev_t_ptr)devs)->param[0] = param0;

}

void CONFIG_ISEE_FLAGS(u64 *flags, u32 set_value, u64 mask, u32 offset)
{
	u64 teei_flags = *flags;
	teei_flags &= ~mask;
#ifdef TEEI_DEBUG
	IMSG_DEBUG("[START] FLAGS[0x%x] VALUE[0x%x] OFFSET[0x%x]\n", (unsigned int)teei_flags, (unsigned int)set_value, offset);
#endif
	teei_flags = teei_flags | (set_value << offset);
#ifdef TEEI_DEBUG
	IMSG_DEBUG("[END] FLAGS[0x%x]\n", (unsigned int)teei_flags);
#endif
	*flags = teei_flags;
}

void CONFIG_ISEE_FLAGS_MULTI_SELECTION(u64 *flags, u32 set_value, u64 mask, u32 offset)
{
	u64 teei_flags = *flags;

	if (!set_value) {
#ifdef TEEI_DEBUG
		IMSG_DEBUG("[START] FLAGS[0x%x] VALUE[0x%x] OFFSET[0x%x]\n", (unsigned int)teei_flags, (unsigned int)set_value, offset);
#endif
		teei_flags &= ~mask;

	}
	teei_flags |= (set_value << offset);
#ifdef TEEI_DEBUG
	IMSG_DEBUG("[END] FLAGS[0x%x]\n", (unsigned int)teei_flags);
#endif
	*flags = teei_flags;
}




#ifdef MTK_SEC_VIDEO_PATH_SUPPORT
#define RESERVED_SEC_MEMORY_SIZE_FOR_SECURE_MEMORY_MGR (3*1024*1024)    /* 3MB */
#define RESERVED_SEC_MEMORY_SIZE_FOR_RES1 (28*1024)                     /* 28KB */
#else
#define RESERVED_SEC_MEMORY_SIZE_FOR_SECURE_MEMORY_MGR (0) /* 0MB */
#define RESERVED_SEC_MEMORY_SIZE_FOR_RES1 (0)                    /* 0KB */
#endif

#ifdef TEEI_DEBUG
static void teei_boot_param_dump(tee_arg_t_ptr teearg) {
    IMSG_DEBUG("============================DUMP START=============================\n");
    IMSG_DEBUG("%s teearg : 0x%x\n", __func__, teearg);
    IMSG_DEBUG("%s atf_magic : 0x%x\n", __func__, teearg->magic);
    IMSG_DEBUG("%s length : 0x%x\n", __func__, teearg->length);
    IMSG_DEBUG("%s version : 0x%x\n", __func__, teearg->version);
    IMSG_DEBUG("%s secDRamBase : 0x%x\n", __func__, teearg->secDRamBase);
    IMSG_DEBUG("%s secDRamSize : 0x%x\n", __func__, teearg->secDRamSize);
    IMSG_DEBUG("%s gic_distributor_base : 0x%x\n", __func__, teearg->gic_distributor_base);
    IMSG_DEBUG("%s gic_cpuinterface_base : 0x%x\n", __func__, teearg->gic_cpuinterface_base);
    IMSG_DEBUG("%s gic_version : 0x%x\n", __func__, teearg->gic_version);
    IMSG_DEBUG("%s total_number_spi : 0x%x\n", __func__, teearg->total_number_spi);
    IMSG_DEBUG("%s teearg->ssiq_number[0] : 0x%x\n", __func__, teearg->ssiq_number[0]);
    IMSG_DEBUG("%s teearg->ssiq_number[1] : 0x%x\n", __func__, teearg->ssiq_number[1]);
    IMSG_DEBUG("%s teearg->ssiq_number[2] : 0x%x\n", __func__, teearg->ssiq_number[2]);
    IMSG_DEBUG("%s teearg->ssiq_number[3] : 0x%x\n", __func__, teearg->ssiq_number[3]);
    IMSG_DEBUG("%s teearg->ssiq_number[4] : 0x%x\n", __func__, teearg->ssiq_number[4]);
    IMSG_DEBUG("%s teearg->tee_dev[0].dev_type : 0x%x\n", __func__, (teearg->tee_dev[0]).dev_type);
    IMSG_DEBUG("%s teearg->tee_dev[0].base_addr : 0x%x\n", __func__,(teearg->tee_dev[0]).base_addr);
    IMSG_DEBUG("%s teearg->tee_dev[0].intr_num : 0x%x\n", __func__, (teearg->tee_dev[0]).intr_num);
    IMSG_DEBUG("%s teearg->tee_dev[0].apc_num : 0x%x\n", __func__, (teearg->tee_dev[0]).apc_num);
    IMSG_DEBUG("%s teearg->tee_dev[1].dev_type : 0x%x\n", __func__, teearg->tee_dev[1].dev_type);
    IMSG_DEBUG("%s teearg->tee_dev[1].base_addr : 0x%x\n", __func__, teearg->tee_dev[1].base_addr);
    IMSG_DEBUG("%s teearg->tee_dev[1].intr_num : 0x%x\n", __func__, teearg->tee_dev[1].intr_num);
    IMSG_DEBUG("%s teearg->tee_dev[1].apc_num : 0x%x\n", __func__, teearg->tee_dev[1].apc_num);
    IMSG_DEBUG("%s teearg->tee_dev[2].dev_type : 0x%x\n", __func__, teearg->tee_dev[2].dev_type);
    IMSG_DEBUG("%s teearg->tee_dev[2].base_addr : 0x%x\n", __func__, teearg->tee_dev[2].base_addr);
    IMSG_DEBUG("%s teearg->tee_dev[2].intr_num : 0x%x\n", __func__, teearg->tee_dev[2].intr_num);
    IMSG_DEBUG("%s teearg->tee_dev[2].apc_num : 0x%x\n", __func__, teearg->tee_dev[2].apc_num);
    IMSG_DEBUG("%s teearg->tee_dev[3].dev_type : 0x%x\n", __func__, teearg->tee_dev[3].dev_type);
    IMSG_DEBUG("%s teearg->tee_dev[3].base_addr : 0x%x\n", __func__, teearg->tee_dev[3].base_addr);
    IMSG_DEBUG("%s teearg->tee_dev[3].intr_num : 0x%x\n", __func__, teearg->tee_dev[3].intr_num);
    IMSG_DEBUG("%s teearg->tee_dev[3].apc_num : 0x%x\n", __func__, teearg->tee_dev[3].apc_num);
}
#endif

static int is_tee_64_bit_supported(u32 tee_entry) {
    u32 tee_first_instr;

    tee_first_instr = *(u32*)tee_entry;
#define INSTR_32BIT_NOP 0xE320F000
    if (tee_first_instr == (u32)INSTR_32BIT_NOP) {
        IMSG_DEBUG("tee is 32-bit supported (0x%x)\n", tee_first_instr);
        return 0;
    } else {
        IMSG_DEBUG("tee is 64-bit supported (0x%x)\n", tee_first_instr);
        return 1;
    }
}

void teei_boot_param_prepare(u32 param_addr, u32 tee_entry, u64 teei_sec_dram_size, u64 dram_base,
                             u64 dram_size, u32 chip_hw_code) {
    tee_arg_t_ptr teearg = (tee_arg_t_ptr)param_addr;
    u32 teei_log_port = 0;

    //IMSG_DEBUG("==================================================================\n");
    IMSG_DEBUG("uTos PL VERSION [%s]\n", UTOS_VERSION);
    //IMSG_DEBUG("==================================================================\n");

    if (teearg == NULL) {
        IMSG_ERROR("teei boot param argument is NULL!\n");
        return;
    }

    memset(teearg, 0, sizeof(tee_arg_t));

    /* Prepare TEE boot parameters */
    teearg->magic = TEEI_BOOTCFG_MAGIC;             /* TEEI magic number */
    teearg->length = sizeof(tee_arg_t);             /* TEEI argument block size */
    teearg->version = TEE_ARGUMENT_VERSION;
    teearg->secDRamBase = tee_entry;                /* Secure DRAM base address */
    teearg->secDRamSize =
        teei_sec_dram_size - RESERVED_SEC_MEMORY_SIZE_FOR_SECURE_MEMORY_MGR; /* Secure DRAM size */
    IMSG_DEBUG("%s secDRamSize: 0x%x\n", __func__, teearg->secDRamSize);

    /* SSI Reserve */
    teearg->total_number_spi = 256;                 /* Platform Support total 256 SPIs and 32 PPIs */
    //teearg->ssiq_number[0] = (32 + 248);            /* Platform reserve SPI ID 248 for TEEI, which is ID 280 */
    teearg->ssiq_number[0] = TEE_FCH_IRQ_NO;            /* Platform reserve SPI ID 248 for TEEI, which is ID 280 */
    teearg->ssiq_number[1] = TEE_DRV_IRQ_NO;            /* Platform reserve SPI ID 248 for TEEI, which is ID 280 */
    teearg->ssiq_number[2] = TEE_REV_IRQ_NO;            /* Platform reserve SPI ID 248 for TEEI, which is ID 280 */
    teearg->ssiq_number[3] = TEE_DUMPER_IRQ_NO;

    /* GIC parameters */
    teearg->gic_distributor_base = GIC_BASE;
#if GIC_VERSION == 0x02
    teearg->gic_cpuinterface_base = GIC_CPU;
#else
    teearg->gic_redistributor_base = GICR_BASE;
#endif
    teearg->gic_version = GIC_VERSION;
    teearg->chip_hw_code = chip_hw_code;
    IMSG_DEBUG("%s teearg->chip_hw_code: 0x%x\n", __func__, teearg->chip_hw_code);

    /* Configure for ISEE flags */
    teearg->flags = ISEE_FLAGS_DEFAULT_VALUE;
    CONFIG_ISEE_FLAGS(&teearg->flags, ISEE_FLAGS_LOG_PATH_BUFFER, ISEE_FLAGS_LOG_PATH_MASK, ISEE_FLAGS_LOG_PATH_OFFSET);
#if defined(TARGET_BUILD_VARIANT_ENG) || defined(TARGET_BUILD_VARIANT_USERDEBUG)
    CONFIG_ISEE_FLAGS(&teearg->flags, ISEE_FLAGS_LOG_LEVEL_INFO, ISEE_FLAGS_LOG_LEVEL_MASK, ISEE_FLAGS_LOG_LEVEL_OFFSET);
#else
    CONFIG_ISEE_FLAGS(&teearg->flags, ISEE_FLAGS_LOG_LEVEL_INFO, ISEE_FLAGS_LOG_LEVEL_MASK, ISEE_FLAGS_LOG_LEVEL_OFFSET);
#endif
    CONFIG_ISEE_FLAGS(&teearg->flags, ISEE_FLAGS_SOTER_TYPE_MINIMAL, ISEE_FLAGS_SOTER_TYPE_MASK, ISEE_FLAGS_SOTER_TYPE_OFFSET);
    CONFIG_ISEE_FLAGS(&teearg->flags, ISEE_FLAGS_VERIFY_MODE_DISABLE_COUNTER, ISEE_FLAGS_VERIFY_MODE_MASK, ISEE_FLAGS_VERIFY_MODE_OFFSET);
#if CFG_RPMB_SET_KEY
    CONFIG_ISEE_FLAGS(&teearg->flags, ISEE_FLAGS_SECURE_STORAGE_RPMB, ISEE_FLAGS_SECURE_STORAGE_MASK, ISEE_FLAGS_SECURE_STORAGE_OFFSET);
#else
    CONFIG_ISEE_FLAGS(&teearg->flags, ISEE_FLAGS_SECURE_STORAGE_FS, ISEE_FLAGS_SECURE_STORAGE_MASK, ISEE_FLAGS_SECURE_STORAGE_OFFSET);
#endif

    CONFIG_ISEE_FLAGS_MULTI_SELECTION(&teearg->flags, ISEE_FLAGS_SOTER_FEATURE_RESET, ISEE_FLAGS_SOTER_FEATURE_MASK, ISEE_FLAGS_SOTER_FEATURE_OFFSET);
#if CFG_MICROTRUST_TUI_SUPPORT
    CONFIG_ISEE_FLAGS_MULTI_SELECTION(&teearg->flags, ISEE_FLAGS_SOTER_FEATURE_TUI, ISEE_FLAGS_SOTER_FEATURE_MASK, ISEE_FLAGS_SOTER_FEATURE_OFFSET);
#endif
#if CFG_MICROTRUST_GP_SUPPORT
    CONFIG_ISEE_FLAGS_MULTI_SELECTION(&teearg->flags, ISEE_FLAGS_SOTER_FEATURE_GP, ISEE_FLAGS_SOTER_FEATURE_MASK, ISEE_FLAGS_SOTER_FEATURE_OFFSET);
#endif
    CONFIG_ISEE_FLAGS_MULTI_SELECTION(&teearg->flags, ISEE_FLAGS_SOTER_FEATURE_DRM, ISEE_FLAGS_SOTER_FEATURE_MASK, ISEE_FLAGS_SOTER_FEATURE_OFFSET);
    if (is_tee_64_bit_supported(tee_entry)) {
     CONFIG_ISEE_FLAGS_MULTI_SELECTION(&teearg->flags, ISEE_FLAGS_SOTER_FEATURE_AARCH64, ISEE_FLAGS_SOTER_FEATURE_MASK, ISEE_FLAGS_SOTER_FEATURE_OFFSET);
    }
    if (!seclib_sbc_enabled()) {
        CONFIG_ISEE_FLAGS_MULTI_SELECTION(&teearg->flags, ISEE_FLAGS_SOTER_FEATURE_EFUSEDISABLE, ISEE_FLAGS_SOTER_FEATURE_MASK, ISEE_FLAGS_SOTER_FEATURE_OFFSET);
    }
	/* Default disabled Multi-Ta */
	/* CONFIG_ISEE_FLAGS(&teearg->flags, ISEE_FLAGS_SOTER_FEATURE_MULTI_TA, ISEE_FLAGS_SOTER_FEATURE_MASK, ISEE_FLAGS_SOTER_FEATURE_OFFSET); */
	CONFIG_ISEE_FLAGS(&teearg->flags, ISEE_FLAGS_SOTER_SYSTEM_DEBUG_OPEN, ISEE_FLAGS_SOTER_SYSTEM_DEBUG_MASK, ISEE_FLAGS_SOTER_SYSTEM_DEBUG_OFFSET);
	CONFIG_ISEE_FLAGS(&teearg->flags, ISEE_FLAGS_BOOT_RELEASE_MODE, ISEE_FLAGS_BOOT_MODE_MASK, ISEE_FLAGS_BOOT_MODE_OFFSET);
    CONFIG_ISEE_FLAGS(&teearg->flags, ISEE_FLAGS_SYSTRACE_DISABLE_MODE, ISEE_FLAGS_SYSTRACE_MODE_MASK, ISEE_FLAGS_SYSTRACE_MODE_OFFSET);
    IMSG_DEBUG("%s teearg->flags :  0x%x\n", __func__, (unsigned int)teearg->flags);

#if CFG_BOOT_ARGUMENT_BY_ATAG
    teei_log_port = g_uart;
    IMSG_DEBUG("%s teearg : CFG_BOOT_ARGUMENT_BY_ATAG 0x%x\n", __func__, g_uart);
#elif CFG_BOOT_ARGUMENT && !CFG_BOOT_ARGUMENT_BY_ATAG
    teei_log_port = bootarg.log_port;
    IMSG_DEBUG("%s teearg : CFG_BOOT_ARGUMENT 0x%x\n", __func__, bootarg.log_port);
#else
    teei_log_port = CFG_UART_LOG;
    IMSG_DEBUG("%s teearg : log port by prj cfg 0x%x\n", __func__, CFG_UART_LOG);
#endif
    /* UART parameters for Log Debug from Uart */
    switch (teei_log_port) {
    case UART0_BASE:
        CONFIG_ISEE_DEVS(&(teearg->tee_dev[0]), MT_UART16550, UART0_BASE, MT_UART0_IRQ,
                         MT_UART0_DAPC, CFG_LOG_BAUDRATE);
        break;
    case UART1_BASE:
        CONFIG_ISEE_DEVS(&(teearg->tee_dev[0]), MT_UART16550, UART1_BASE, MT_UART1_IRQ,
                         MT_UART1_DAPC, CFG_LOG_BAUDRATE);
        break;
    case UART2_BASE:
        CONFIG_ISEE_DEVS(&(teearg->tee_dev[0]), MT_UART16550, UART2_BASE, MT_UART2_IRQ,
                         MT_UART2_DAPC, CFG_LOG_BAUDRATE);
        break;
#ifdef UART3_BASE
    case UART3_BASE:
        CONFIG_ISEE_DEVS(&(teearg->tee_dev[0]), MT_UART16550, UART3_BASE, MT_UART3_IRQ,
                         MT_UART3_DAPC, CFG_LOG_BAUDRATE);
        break;
#endif
    default:
        break;
    }

    /* SEC GPTIMER parameters */
    CONFIG_ISEE_DEVS(&(teearg->tee_dev[1]), MT_SEC_GPT, MT_SEC_GPT_BASE, MT_SEC_GPT_IRQ,
                     MT_SEC_GPT_DAPC, 0);
    /* SEC GPTWDT parameters */
    CONFIG_ISEE_DEVS(&(teearg->tee_dev[2]), MT_SEC_WDT, MT_SEC_WDT_BASE, MT_SEC_WDT_IRQ,
                     MT_SEC_WDT_DAPC, 0);
    /* SEC CRYPTO parameters */
    CONFIG_ISEE_DEVS(&(teearg->tee_dev[3]), MT_SEC_CRYPTO, MT_SEC_CRYPTO_BASE, MT_SEC_CRYPTO_IRQ,
                     MT_SEC_CRYPTO_DAPC, 0);

    /* when  spi  is used, you should set a right SPI_INDEX*/
#ifdef CFG_MICROTRUST_FP_SUPPORT
	CONFIG_ISEE_DEVS(&(teearg->tee_dev[4]),
			MT_SEC_SPI | DEBUG | LOOPBACK, 0, 0, 0, SPI_INDEX);
#endif

#ifdef TEEI_DEBUG
    teei_boot_param_dump(teearg);
#endif
}

void teei_key_param_prepare(u32 param_addr,u8 *hwuid, u32 hwid_size,u8 *hrid, u32 hrid_size, u8 * rpmb_key)
{
    tee_keys_t_ptr keyarg = (tee_keys_t_ptr)param_addr;
    keyarg->magic = TEEI_BOOTCFG_MAGIC;
    memcpy(keyarg->rpmb_key, rpmb_key, KEY_LEN);
    /* hwuid[0..15] used for huk, hwuid[16..23] used for socid, wechat will use */
    keyarg->hwid_size = hwid_size;
    if (keyarg->hwid_size)
        memcpy(keyarg->hw_id, hwuid, keyarg->hwid_size);
    keyarg->hrid_size =  hrid_size; /* add hrid size atf hrid size */
    if (keyarg->hrid_size)
        memcpy(keyarg->hr_id, hrid, keyarg->hrid_size);/* add hrid copy */
}

extern unsigned long rtcgettime_teei(void);
void teei_sgpt1_init(u32 base_addr);

void teei_rtctime_param_prepare(u32 param_addr)
{
	tee_arg_t_ptr teearg = (tee_arg_t_ptr)param_addr;
	tee_dev_t_ptr teedev1 = &(teearg->tee_dev[1]);
	u32 rtctime = 0;
	rtctime = rtcgettime_teei();
	teedev1->param[0] = rtctime;
	teei_sgpt1_init(MT_SEC_GPT_BASE);
}

#define        SEC_APXGPT1_CON               0x220
#define        SEC_APXGPT1_PRESCALE          0x224
#define        SEC_APXGPT1_COUNTER_L         0x228
#define        SEC_APXGPT1_COUNTER_H         0x22C


void teei_sgpt1_init(u32 base_addr)
{
       unsigned int val;
        val = 0x0;
       __raw_writel(val, base_addr+SEC_APXGPT1_CON);   //disable

       val = 0x2;
        __raw_writel(val, base_addr+SEC_APXGPT1_CON);  //clears counter to 0

       val = 0x0;
        __raw_writel(val, base_addr+SEC_APXGPT1_PRESCALE);   //13M clock, one division

       val = 0x30;
        __raw_writel(val, base_addr+SEC_APXGPT1_CON);   //free run

       val = 0x31;
        __raw_writel(val, base_addr+SEC_APXGPT1_CON);   //enable

}
