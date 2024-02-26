/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein is
 * confidential and proprietary to MediaTek Inc. and/or its licensors. Without
 * the prior written permission of MediaTek inc. and/or its licensors, any
 * reproduction, modification, use or disclosure of MediaTek Software, and
 * information contained herein, in whole or in part, shall be strictly
 * prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER
 * ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 * INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES
 * TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO
 * OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN MEDIATEK
 * SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE
 * RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S
 * ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE
 * RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE
 * MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE
 * CHARGE PAID BY RECEIVER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek
 * Software") have been modified by MediaTek Inc. All revisions are subject to
 * any receiver's applicable license agreements with MediaTek Inc.
 */

#include "typedefs.h"
#include "platform.h"
#include "boot_device.h"
#include "nand.h"
#include "mmc_common_inter.h"
#include "memory_layout.h"
#if CFG_ENABLE_DCACHE
#include "mmu.h"
#endif

#include "uart.h"
#include "nand_core.h"
#include "pll.h"
#include "i2c.h"
#include "rtc.h"
#include "emi.h"
#include "pmic.h"
#include "wdt.h"
#include "ram_console.h"
#include "cust_sec_ctrl.h"
#include "gpio.h"
#include "pmic_wrap_init.h"
#include "keypad.h"
#include "usbphy.h"
#include "timer.h"
#include "dram_buffer.h"
#include "spm_mtcmos.h"
#include "lastpc.h"
#include <clkbuf_ctl.h>
#include <pmic_drv.h>
#include <sec_efuse.h>
#include "pl_version.h"
#include <mtk_charger.h>
#include <mtk_subpmic.h>
#include <anti_rollback.h>
#include <rpmb_key.h>
#if CFG_GZ_SUPPORT
#include <gz_init.h>
#endif

#ifdef MTK_TRNG_CALIBRATION
#if defined MTK_TRNG_CALIBRATION_1ST_ROUND
#define TRNG_CALIB_BUF_SIZE  (28 + 24 * 1066)
unsigned int __NOBITS_SECTION__(.secbuf) g_trng_buf[TRNG_CALIB_BUF_SIZE / sizeof(unsigned int)];
extern unsigned int trng_calib_1st_round(unsigned int *buf, unsigned int buf_sz);
#endif //MTK_TRNG_CALIBRATION_1ST_ROUND
#if defined  MTK_TRNG_CALIBRATION_2ND_ROUND
#define TRNG_CALIB_BUF_SIZE  (28 + 24 * 546135)
unsigned int *g_trng_buf = 0x42000000;
extern unsigned int trng_calib_2nd_round(unsigned int *buf, unsigned int buf_sz);
#endif //MTK_TRNG_CALIBRATION_2ND_ROUND
#endif //MTK_TRNG_CALIBRATION

/*============================================================================*/
/* CONSTAND DEFINITIONS                                                       */
/*============================================================================*/
#define MOD "[PLFM]"

/*============================================================================*/
/* GLOBAL VARIABLES                                                           */
/*============================================================================*/
unsigned int __SRAM__ sys_stack[CFG_SYS_STACK_SZ >> 2];
const unsigned int sys_stack_sz = CFG_SYS_STACK_SZ;
boot_mode_t g_boot_mode;
boot_dev_t g_boot_dev;
meta_com_t g_meta_com_type = META_UNKNOWN_COM;
u32 g_meta_com_id = 0;
unsigned char g_meta_log_disable = 0;
boot_reason_t g_boot_reason;
ulong g_boot_time;
u32 g_ddr_reserve_enable = 0;
u32 g_ddr_reserve_success = 0;
u32 g_ddr_reserve_ready = 0;
u32 g_ddr_reserve_ta_err = 0;
extern unsigned int part_num;
extern part_hdr_t   part_info[PART_MAX_NUM];
extern bool ram_console_is_abnormal_boot(void);
extern bl_param_t *p_bldr_param;

#if CFG_BOOT_ARGUMENT
#define bootarg g_dram_buf->bootarg
#endif

#if CFG_USB_AUTO_DETECT
bool g_usbdl_flag;
#endif

#if CFG_FAST_META_SUPPORT
unsigned char g_is_fast_meta = 0;
#endif

#if CFG_ENABLE_DCACHE
bool g_is_dcache_enable = false;
#endif
/*============================================================================*/
/* EXTERNAL FUNCTIONS                                                         */
/*============================================================================*/
#define GPIO_BANK       (GPIO_BASE + 0x6F0)
#define TRAP_UFS_FIRST  (1U << 11) /* bit 11 0: UFS, bit 11 1: eMMC */
static u32 boot_device_init(void)
{
#if !defined(BOOTDEV_SDMMC_UFS_COMBO)
    #if (CFG_BOOT_DEV == BOOTDEV_UFS)
    return (u32)ufs_init_device();
    #elif (CFG_BOOT_DEV == BOOTDEV_SDMMC)
    return (u32)mmc_init_device();
    #else
    return (u32)nand_init_device();
    #endif
#else
    #if (CFG_BOOT_DEV == BOOTDEV_SDMMC) || (CFG_BOOT_DEV == BOOTDEV_UFS)
    #if CFG_FPGA_PLATFORM
    if (ufs_init_device())
    	return (u32)mmc_init_device();
    else
    	return 0;
    #else
    if ((DRV_Reg32(GPIO_BANK) & TRAP_UFS_FIRST) == 0)
        return (u32)ufs_init_device();
    else
        return (u32)mmc_init_device();
    #endif
    #else
    return (u32)nand_init_device();
    #endif
#endif
}

int usb_accessory_in(void)
{
#if !CFG_FPGA_PLATFORM
    int exist = 0;

    if (upmu_is_chr_det() == KAL_TRUE) {
        exist = 1;
        #if !CFG_USBIF_COMPLIANCE
        /* enable charging current as early as possible to avoid can't enter
         * following battery charging flow when low battery
         */
        platform_set_chrg_cur(450);
        #endif
    }
    return exist;
#else
    return 1;
#endif
}

extern bool is_uart_cable_inserted(void);

int usb_cable_in(void)
{
#if !CFG_FPGA_PLATFORM
    int exist = 0;
    CHARGER_TYPE ret;

	/* Change MDCIRQ USB signal source (ap2md_ssusb_dev_mix_int)
    from default Full Interrupt 0x00 (ssusb_dev_int)
    to MD partial Interrupt 0x01 (ssusb_dev_int_md) */
	pal_log_info("\n[USB] MDCIRQ 0x10000280 before: %x\n", DRV_Reg32(0x10000280));
	DRV_WriteReg32(0x10000280, (DRV_Reg32(0x10000280) & ~0x3) | 0x1);
	pal_log_info("[USB] MDCIRQ 0x10000280 after: %x\n\n", DRV_Reg32(0x10000280));

    if ((g_boot_reason == BR_USB) || usb_accessory_in()) {
        ret = mt_charger_type_detection();
        if (ret == STANDARD_HOST || ret == CHARGING_HOST) {
            pal_log_info("\n%s USB cable in\n", MOD);
            mt_usb_phy_poweron();
            mt_usb_phy_savecurrent();

            exist = 1;
        } else if (ret == NONSTANDARD_CHARGER || ret == STANDARD_CHARGER) {
            #if CFG_USBIF_COMPLIANCE
            platform_set_chrg_cur(450);
            #endif
        }
    }

    return exist;
#else
    pal_log_info("\n%s USB cable in\n", MOD);
    mt_usb_phy_poweron();
    mt_usb_phy_savecurrent();

    return 1;
#endif
}

#if CFG_FPGA_PLATFORM
void show_tx(void)
{
	UINT8 var;
	USBPHY_I2C_READ8(0x6E, &var);
	UINT8 var2 = (var >> 3) & ~0xFE;
	pal_log_info("[USB]addr: 0x6E (TX), value: %x - %x\n", var, var2);
}

void store_tx(UINT8 value)
{
	UINT8 var;
	UINT8 var2;
	USBPHY_I2C_READ8(0x6E, &var);

	if (value == 0) {
		var2 = var & ~(1 << 3);
	} else {
		var2 = var | (1 << 3);
	}

	USBPHY_I2C_WRITE8(0x6E, var2);
	USBPHY_I2C_READ8(0x6E, &var);
	var2 = (var >> 3) & ~0xFE;

	pal_log_info("[USB]addr: 0x6E TX [AFTER WRITE], value after: %x - %x\n", var, var2);
}

void show_rx(void)
{
	UINT8 var;
	USBPHY_I2C_READ8(0x77, &var);
	UINT8 var2 = (var >> 7) & ~0xFE;
	pal_log_info("[USB]addr: 0x77 (RX) [AFTER WRITE], value after: %x - %x\n", var, var2);
}

void test_uart(void)
{
	int i=0;
	UINT8 val = 0;
	for (i=0; i<1000; i++)
	{
			show_tx();
            mdelay(300);
			if (val) {
				val = 0;
			}
			else {
				val = 1;
			}
			store_tx(val);
			show_rx();
            mdelay(1000);
	}
}
#endif

void set_to_uart_mode(void)
{
	UINT32 var;

	/* RG_USB20_BC11_SW_EN 0x11F4_0818[23] = 1'b0 */
	USBPHY_CLR32(0x18, (0x1 << 23));

	/* Set RG_SUSPENDM 0x11F4_0868[3] to 1 */
	USBPHY_SET32(0x68, (0x1 << 3));

	/* force suspendm 0x11F4_0868[18] = 1 */
	USBPHY_SET32(0x68, (0x1 << 18));
	/* Set rg_uart_mode 0x11F4_0868[31:30] to 2'b01 */
	USBPHY_CLR32(0x68, (0x3 << 30));
	USBPHY_SET32(0x68, (0x1 << 30));

	/* force_uart_i 0x11F4_0868[29] = 0 */
	USBPHY_CLR32(0x68, (0x1 << 29));

	/* force_uart_bias_en 0X11f4_0868[28] = 1 */
	USBPHY_SET32(0x68, (0x1 << 28));

	/* force_uart_tx_oe 0x11F4_0868[27] = 1 */
	USBPHY_SET32(0x68, (0x1 << 27));

	/* force_uart_en 0x11F4_0868[26] = 1 */
	USBPHY_SET32(0x68, (0x1 << 26));

	/* RG_UART_BIAS_EN 0x11F4_086C[18] = 1 */
	USBPHY_SET32(0x6C, (0x1 << 18));

	/* RG_UART_TX_OE 0X11f4_086c[17] = 1 */
	USBPHY_SET32(0x6C, (0x1 << 17));

	/* Set RG_UART_EN to 1 */
	USBPHY_SET32(0x6C, (0x1 << 16));

	/* Set RG_USB20_DM_100K_EN to 1 */
	USBPHY_SET32(0x20, (0x1 << 17));

	/* RG_XCVRSEL[1:0], 2'b01. */
	USBPHY_CLR32(0x68, (0x3 << 4));
	USBPHY_SET32(0x68, (0x1 << 4));
	/* RG_TERMSEL, 1'b1 */
	USBPHY_SET32(0x68, (0x1 << 2));

	/* force_dp_pulldown, 1'b1, force_dm_pulldown, 1'b1,
	 * force_xcversel, 1'b1, force_termsel, 1'b1, force_datain, 1'b1
	 */
	USBPHY_SET32(0x68, ((0x1 << 20) | (0x1 << 21) | (0x1 << 19) | (0x1 << 17) | (0x1 << 23)));

	/* GPIO Selection */
	var = readl(GPIO_BASE+RG_GPIO_SELECT);
	writel(var & (~(GPIO_SEL_MASK)), GPIO_BASE+RG_GPIO_SELECT);

	var = readl(GPIO_BASE+RG_GPIO_SELECT);
	writel(var | (GPIO_SEL_UART0), GPIO_BASE+RG_GPIO_SELECT);

	pal_log_info("\n[USB] Switch to UART mode!!\n");
}

void platform_vusb_on(void)
{
#if !CFG_FPGA_PLATFORM
    U32 ret = 0;

    ret = PMIC_VUSB_EN();

    if (ret == 0)
        pal_log_info("[platform_vusb_on] VUSB33 is on\n");
    else
        pal_log_info("[platform_vusb_on] Failed to turn on VUSB33!\n");
#endif
    return;
}

#if CFG_BOOT_ARGUMENT_BY_ATAG
void platform_set_boot_args_by_atag(unsigned *ptr)
{
    int i=0, j=0;
    boot_tag *tags;

    tags = (boot_tag *)ptr;
    tags->hdr.size = boot_tag_size(boot_tag_boot_reason);
    tags->hdr.tag = BOOT_TAG_BOOT_REASON;
    tags->u.boot_reason.boot_reason = g_boot_reason;
    ptr += tags->hdr.size;

    tags = (boot_tag *)ptr;
    tags->hdr.size = boot_tag_size(boot_tag_boot_mode);
    tags->hdr.tag = BOOT_TAG_BOOT_MODE;
    tags->u.boot_mode.boot_mode = g_boot_mode;
    ptr += tags->hdr.size;

    tags = (boot_tag *)ptr;
    tags->hdr.size = boot_tag_size(boot_tag_meta_com);
    tags->hdr.tag = BOOT_TAG_META_COM;
    tags->u.meta_com.meta_com_id = g_meta_com_id;
    tags->u.meta_com.meta_com_type = (u32)g_meta_com_type;
    tags->u.meta_com.meta_uart_port = CFG_UART_META;
    tags->u.meta_com.meta_log_disable = g_meta_log_disable;
    tags->u.meta_com.fast_meta_gpio = CFG_FAST_META_GPIO;
    ptr += tags->hdr.size;

    tags = (boot_tag *)ptr;
    tags->hdr.size = boot_tag_size(boot_tag_log_com);
    tags->hdr.tag = BOOT_TAG_LOG_COM;
    tags->u.log_com.log_port = CFG_UART_LOG;
    // Switch log port to UART2 while uart meta connected.
    if (g_boot_mode == META_BOOT && g_meta_com_type == META_UART_COM)
        tags->u.log_com.log_port = UART2;
    tags->u.log_com.log_baudrate = CFG_LOG_BAUDRATE;
    tags->u.log_com.log_enable = (u8)log_status();
#if CFG_UART_DYNAMIC_SWITCH
    tags->u.log_com.log_dynamic_switch = (u8)get_log_switch();
#endif
    ptr += tags->hdr.size;

  //---- DRAM tags ----
    tags = (boot_tag *)ptr;
	memset(tags,0,sizeof(boot_tag));
    tags->hdr.size = boot_tag_size(boot_tag_mem);
    tags->hdr.tag = BOOT_TAG_MEM;
    tags->u.mem.dram_rank_num = bootarg.orig_dram_info.rank_num;

    for(i=0;i<4;i++){
	tags->u.mem.dram_rank_size[i] = bootarg.dram_rank_size[i];
    }
    tags->u.mem.mblock_info.mblock_num = bootarg.mblock_info.mblock_num;
    for(i=0;i<tags->u.mem.mblock_info.mblock_num;i++) {
	tags->u.mem.mblock_info.mblock[i].start = bootarg.mblock_info.mblock[i].start;
	tags->u.mem.mblock_info.mblock[i].size = bootarg.mblock_info.mblock[i].size;
	tags->u.mem.mblock_info.mblock[i].rank = bootarg.mblock_info.mblock[i].rank;
    }
#if defined(CFG_MBLOCK_LIB) && (MBLOCK_EXPAND(CFG_MBLOCK_LIB) == MBLOCK_EXPAND(2))
	tags->u.mem.mblock_info.mblock_magic = MBLOCK_MAGIC;
	tags->u.mem.mblock_info.mblock_version = MBLOCK_VERSION;
    tags->u.mem.mblock_info.reserved_num = bootarg.mblock_info.reserved_num;
    for(i=0;i<tags->u.mem.mblock_info.reserved_num;i++) {
	tags->u.mem.mblock_info.reserved[i].start = bootarg.mblock_info.reserved[i].start;
	tags->u.mem.mblock_info.reserved[i].size = bootarg.mblock_info.reserved[i].size;
	tags->u.mem.mblock_info.reserved[i].mapping= bootarg.mblock_info.reserved[i].mapping;
	memcpy(&tags->u.mem.mblock_info.reserved[i].name, &bootarg.mblock_info.reserved[i].name, MBLOCK_RESERVED_NAME_SIZE);
    }
#endif
    tags->u.mem.orig_dram_info.rank_num = bootarg.orig_dram_info.rank_num;
    for(i=0;i<4;i++) {
	tags->u.mem.orig_dram_info.rank_info[i].start = bootarg.orig_dram_info.rank_info[i].start; // 64
	tags->u.mem.orig_dram_info.rank_info[i].size = bootarg.orig_dram_info.rank_info[i].size; // 64
    }

    tags->u.mem.lca_reserved_mem.start= bootarg.lca_reserved_mem.start;
    tags->u.mem.lca_reserved_mem.size = bootarg.lca_reserved_mem.size;
    tags->u.mem.tee_reserved_mem.start= bootarg.tee_reserved_mem.start;
    tags->u.mem.tee_reserved_mem.size = bootarg.tee_reserved_mem.size;
    ptr += tags->hdr.size;
  //----------------
    tags = (boot_tag *)ptr;
    tags->hdr.size = boot_tag_size(boot_tag_md_info);
    tags->hdr.tag = BOOT_TAG_MD_INFO;
    for(i=0;i<4;i++)
	tags->u.md_info.md_type[i] = bootarg.md_type[i];
    ptr += tags->hdr.size;

    tags = (boot_tag *)ptr;
    tags->hdr.size = boot_tag_size(boot_tag_boot_time);
    tags->hdr.tag = BOOT_TAG_BOOT_TIME;
    tags->u.boot_time.boot_time = get_timer(g_boot_time);
    ptr += tags->hdr.size;

    tags = (boot_tag *)ptr;
    tags->hdr.size = boot_tag_size(boot_tag_da_info);
    tags->hdr.tag = BOOT_TAG_DA_INFO;
    tags->u.da_info.da_info.addr = bootarg.da_info.addr;
    tags->u.da_info.da_info.arg1 = bootarg.da_info.arg1;
    tags->u.da_info.da_info.arg2 = bootarg.da_info.arg2;
    tags->u.da_info.da_info.len  = bootarg.da_info.len;
    tags->u.da_info.da_info.sig_len = bootarg.da_info.sig_len;
    ptr += tags->hdr.size;

    tags = (boot_tag *)ptr;
    tags->hdr.size = boot_tag_size(boot_tag_sec_info);
    tags->hdr.tag = BOOT_TAG_SEC_INFO;
    tags->u.sec_info.sec_limit.magic_num = bootarg.sec_limit.magic_num;
    tags->u.sec_info.sec_limit.forbid_mode = bootarg.sec_limit.forbid_mode;
    ptr += tags->hdr.size;

    tags = (boot_tag *)ptr;
    tags->hdr.size = boot_tag_size(boot_tag_part_num);
    tags->hdr.tag = BOOT_TAG_PART_NUM;
    tags->u.part_num.part_num = g_dram_buf->part_num;
    ptr += tags->hdr.size;

    tags = (boot_tag *)ptr;
    tags->hdr.size = boot_tag_size(boot_tag_part_info);
    tags->hdr.tag = BOOT_TAG_PART_INFO;
    tags->u.part_info.part_info = g_dram_buf->part_info;
    ptr += tags->hdr.size;

    tags = (boot_tag *)ptr;
    tags->hdr.size = boot_tag_size(boot_tag_eflag);
    tags->hdr.tag = BOOT_TAG_EFLAG;
    tags->u.eflag.e_flag = sp_check_platform();
    ptr += tags->hdr.size;

    tags = (boot_tag *)ptr;
    tags->hdr.size = boot_tag_size(boot_tag_ddr_reserve);
    tags->hdr.tag = BOOT_TAG_DDR_RESERVE;
    tags->u.ddr_reserve.ddr_reserve_enable = g_ddr_reserve_enable;
    tags->u.ddr_reserve.ddr_reserve_success = (g_ddr_reserve_ta_err == 0) ? 1 : 0;
    tags->u.ddr_reserve.ddr_reserve_ready = g_ddr_reserve_ready;
    ptr += tags->hdr.size;

    tags = (boot_tag *)ptr;
    tags->hdr.size  = boot_tag_size(boot_tag_dram_buf);
    tags->hdr.tag   = BOOT_TAG_DRAM_BUF;
    tags->u.dram_buf.dram_buf_size = sizeof(dram_buf_t);
    ptr += tags->hdr.size;

    tags = (boot_tag *)ptr;
    tags->hdr.size  = boot_tag_size(boot_tag_emi_info);
    tags->hdr.tag   = BOOT_TAG_EMI_INFO;
#if !CFG_FPGA_PLATFORM
    tags->u.emi_info.dram_type = mt_get_dram_type();
    tags->u.emi_info.ch_num = get_dram_channel_nr();
    tags->u.emi_info.rk_num = get_dram_rank_nr();
    get_dram_rank_size(tags->u.emi_info.rank_size);
#else
    tags->u.emi_info.dram_type = 0;  // unused info on FPGA
    tags->u.emi_info.ch_num = 0;
    tags->u.emi_info.rk_num = 0;
#endif
    ptr += tags->hdr.size;

    tags = (boot_tag *)ptr;
    tags->hdr.size = boot_tag_size(boot_tag_sram_info);
    tags->hdr.tag  = BOOT_TAG_SRAM_INFO;
    tags->u.sram_info.non_secure_sram_addr= CFG_NON_SECURE_SRAM_ADDR;
    tags->u.sram_info.non_secure_sram_size = CFG_NON_SECURE_SRAM_SIZE;
    ptr += tags->hdr.size;

    tags = (boot_tag *)ptr;
    tags->hdr.size = boot_tag_size(boot_tag_plat_dbg_info);
    tags->hdr.tag  = BOOT_TAG_PLAT_DBG_INFO;
    tags->u.plat_dbg_info.info_max = INFO_TYPE_MAX;
    for (i = 0; i < INFO_TYPE_MAX; i++) {
#ifdef CFG_PLAT_DBG_INFO_LIB
        unsigned int key = get_dbg_info_key(i);
        tags->u.plat_dbg_info.info[i].key = key;
        tags->u.plat_dbg_info.info[i].base = get_dbg_info_base(key);
        tags->u.plat_dbg_info.info[i].size = get_dbg_info_size(key);
#else
        tags->u.plat_dbg_info.info[i].key = 0;
        tags->u.plat_dbg_info.info_base[i] = 0;
        tags->u.plat_dbg_info.info_size[i] = 0;
#endif
    }
    ptr += tags->hdr.size;

    tags = (boot_tag *)ptr;
    tags->hdr.size = boot_tag_size(boot_tag_chr_info);
    tags->hdr.tag  = BOOT_TAG_CHR_INFO;
#if !CFG_FPGA_PLATFORM
    tags->u.chr_info.charger_type = mt_charger_type_detection();
#else
    tags->u.chr_info.charger_type = STANDARD_HOST;
#endif
    ptr += tags->hdr.size;

	tags = (boot_tag *)ptr;
	tags->hdr.size = boot_tag_size(boot_tag_bat_info);
	tags->hdr.tag  = BOOT_TAG_BAT_INFO;
	tags->u.bat_info.boot_voltage = fg_get_vbat();
	tags->u.bat_info.shutdown_time = fg_get_shutdowntime();
	ptr += tags->hdr.size;

    tags = (boot_tag *)ptr;
    tags->hdr.size = boot_tag_size(boot_tag_is_abnormal_boot);
    tags->hdr.tag = BOOT_TAG_IS_ABNORMAL_BOOT;
    tags->u.is_abnormal_boot.is_abnormal_boot = ram_console_is_abnormal_boot();
    ptr += tags->hdr.size;

    {
        u32 ram_console_sram_addr = 0;
        u32 ram_console_sram_size = 0;
        tags = (boot_tag *)ptr;
        tags->hdr.size = boot_tag_size(boot_tag_ram_console_info);
        tags->hdr.tag = BOOT_TAG_RAM_CONSOLE_INFO;
        ram_console_sram_addr_size(&ram_console_sram_addr, &ram_console_sram_size);
        tags->u.ram_console_info.sram_addr = ram_console_sram_addr;
        tags->u.ram_console_info.sram_size = ram_console_sram_size;
        tags->u.ram_console_info.def_type = ram_console_def_memory();
        tags->u.ram_console_info.memory_info_offset = ram_console_memory_info_offset();
        ptr += tags->hdr.size;
    }

    tags = (boot_tag *)ptr;
    tags->hdr.size = boot_tag_size(boot_tag_soc_id);
    tags->hdr.tag  = BOOT_TAG_SOC_ID;
    memcpy(tags->u.socid.id, p_bldr_param->socid, SOC_ID_LEN);
    ptr += tags->hdr.size;

#if CFG_GZ_SUPPORT
    ptr = gz_config_info_atag(ptr);
    ptr = gz_config_boot_atag(ptr);
    ptr = gz_config_platform_atag(ptr);
#endif

#if CFG_GZ_PWRAP_ENABLE
    ptr = gz_config_pwrap_atag(ptr);
#endif

    /* END */
    *ptr++ = 0;
    *ptr++ = 0;
}

void platform_dump_boot_atag(void)
{
    int i=0, j=0;
    boot_tag *tags;
	for (tags = (boot_tag *)&(g_dram_buf->boottag); tags->hdr.size; tags = (boot_tag *)boot_tag_next(tags))
	{
        switch(tags->hdr.tag)
		{
            case BOOT_TAG_BOOT_REASON:
				pal_log_err("BOOT_REASON: %d\n", tags->u.boot_reason.boot_reason);
				break;
			case BOOT_TAG_BOOT_MODE:
				pal_log_err("BOOT_MODE: %d\n", tags->u.boot_mode.boot_mode);
				break;
			case BOOT_TAG_META_COM:
				pal_log_info("META_COM TYPE: %d\n", tags->u.meta_com.meta_com_type);
				pal_log_info("META_COM ID: %d\n", tags->u.meta_com.meta_com_id);
				pal_log_info("META_COM PORT: %d\n", tags->u.meta_com.meta_uart_port);
				pal_log_info("META LOG DISABLE: %d\n", tags->u.meta_com.meta_log_disable);
				pal_log_info("FAST META GPIO: %d\n", tags->u.meta_com.fast_meta_gpio);
				break;
			case BOOT_TAG_LOG_COM:
				pal_log_info("LOG_COM PORT: %d\n", tags->u.log_com.log_port);
				pal_log_info("LOG_COM BAUD: %d\n", tags->u.log_com.log_baudrate);
				pal_log_info("LOG_COM EN: %d\n", tags->u.log_com.log_enable);
#if CFG_UART_DYNAMIC_SWITCH
				pal_log_info("LOG_COM SWITCH: %d\n", tags->u.log_com.log_dynamic_switch);
#endif
				break;
			case BOOT_TAG_MEM:
				pal_log_info("MEM_NUM: %d\n", tags->u.mem.dram_rank_num);
				for (i = 0; i < tags->u.mem.dram_rank_num; i++)
				    pal_log_info("MEM_SIZE: 0x%llx\n", tags->u.mem.dram_rank_size[i]);

				pal_log_info("mblock num: 0x%x\n", tags->u.mem.mblock_info.mblock_num);
				for(i=0;i<tags->u.mem.mblock_info.mblock_num;i++) {
				    pal_log_info("mblock start: 0x%llx\n", tags->u.mem.mblock_info.mblock[i].start);
				    pal_log_info("mblock size: 0x%llx\n", tags->u.mem.mblock_info.mblock[i].size);
				    pal_log_info("mblock rank: 0x%x\n", tags->u.mem.mblock_info.mblock[i].rank);
				}
#if defined(CFG_MBLOCK_LIB) && (MBLOCK_EXPAND(CFG_MBLOCK_LIB) == MBLOCK_EXPAND(2))
				pal_log_info("magic=0x%lx version=0x%lx reserve num: 0x%x\n", \
						tags->u.mem.mblock_info.mblock_magic, \
						tags->u.mem.mblock_info.mblock_version,\
						tags->u.mem.mblock_info.reserved_num);\
				for(i=0;i<tags->u.mem.mblock_info.reserved_num;i++) {
				    pal_log_info("mblock start: 0x%llx\n", tags->u.mem.mblock_info.reserved[i].start);
				    pal_log_info("mblock size: 0x%llx\n", tags->u.mem.mblock_info.reserved[i].size);
				    pal_log_info("mblock mapping: 0x%x\n", tags->u.mem.mblock_info.reserved[i].mapping);
				    pal_log_info("mblock name: %s\n", tags->u.mem.mblock_info.reserved[i].name);
				}
#endif
				pal_log_info("orig_dram num: 0x%x\n", tags->u.mem.orig_dram_info.rank_num);
				for(i=0;i<4;i++) {
				    pal_log_info("orig_dram start: 0x%llx\n", tags->u.mem.orig_dram_info.rank_info[i].start);
					pal_log_info("orig_dram size: 0x%llx\n", tags->u.mem.orig_dram_info.rank_info[i].size);
				}

				pal_log_info("lca start: 0x%llx\n", tags->u.mem.lca_reserved_mem.start);
				pal_log_info("lca size: 0x%llx\n", tags->u.mem.lca_reserved_mem.size);
				pal_log_info("tee start: 0x%llx\n", tags->u.mem.tee_reserved_mem.start);
				pal_log_info("tee size: 0x%llx\n", tags->u.mem.tee_reserved_mem.size);
				break;
			case BOOT_TAG_MD_INFO:
				for(i=0;i<4;i++)
					pal_log_info("MD_INFO: 0x%x\n", tags->u.md_info.md_type[i]);
				break;
			case BOOT_TAG_BOOT_TIME:
				pal_log_err("BOOT_TIME: %d\n", tags->u.boot_time.boot_time);
				break;
			case BOOT_TAG_DA_INFO:
				pal_log_info("DA_INFO: 0x%x\n", tags->u.da_info.da_info.addr);
				pal_log_info("DA_INFO: 0x%x\n", tags->u.da_info.da_info.arg1);
				pal_log_info("DA_INFO: 0x%x\n", tags->u.da_info.da_info.arg2);
				pal_log_info("DA_INFO: 0x%x\n", tags->u.da_info.da_info.len);
				pal_log_info("DA_INFO: 0x%x\n", tags->u.da_info.da_info.sig_len);
				break;
			case BOOT_TAG_SEC_INFO:
				pal_log_info("SEC_INFO: 0x%x\n", tags->u.sec_info.sec_limit.magic_num);
				pal_log_info("SEC_INFO: 0x%x\n", tags->u.sec_info.sec_limit.forbid_mode);
				break;
			case BOOT_TAG_PART_NUM:
				pal_log_info("PART_NUM: %d\n", tags->u.part_num.part_num);
				break;
			case BOOT_TAG_PART_INFO:
				pal_log_info("PART_INFO: 0x%x\n", tags->u.part_info.part_info);
				break;
			case BOOT_TAG_EFLAG:
				pal_log_info("EFLAG: %d\n", tags->u.eflag.e_flag);
				break;
			case BOOT_TAG_DDR_RESERVE:
				pal_log_info("DDR_RESERVE: %d\n", tags->u.ddr_reserve.ddr_reserve_enable);
				pal_log_info("DDR_RESERVE: %d\n", tags->u.ddr_reserve.ddr_reserve_success);
				pal_log_info("DDR_RESERVE: %d\n", tags->u.ddr_reserve.ddr_reserve_ready);
				break;
			case BOOT_TAG_DRAM_BUF:
				pal_log_info("DRAM_BUF: %d\n", tags->u.dram_buf.dram_buf_size);
				break;
			case BOOT_TAG_EMI_INFO:
				pal_log_info("EMI dram_type: %d\n", tags->u.emi_info.dram_type);
				pal_log_info("EMI ch_num: %d\n", tags->u.emi_info.ch_num);
				pal_log_info("EMI rk_num: %d\n", tags->u.emi_info.rk_num);
				for (i = 0; i < tags->u.emi_info.rk_num; i++) {
					pal_log_info("EMI rank%d size: 0x%x\n", i, tags->u.emi_info.rank_size[i]);
				}
				break;
			case BOOT_TAG_SRAM_INFO:
				pal_log_info("SRAM start: 0x%x\n", tags->u.sram_info.non_secure_sram_addr);
				pal_log_info("SRAM size: 0x%x\n", tags->u.sram_info.non_secure_sram_size);
				break;
			case BOOT_TAG_BAT_INFO:
				pal_log_info("boot_voltage: 0x%x\n", tags->u.bat_info.boot_voltage);
				pal_log_info("shutdown_time: 0x%x\n", tags->u.bat_info.shutdown_time);
				break;
			case BOOT_TAG_PLAT_DBG_INFO:
				for (i = 0; i < INFO_TYPE_MAX; i++) {
					pal_log_info("PLAT_DBG_INFO key: 0x%x\n", tags->u.plat_dbg_info.info[i].key);
					pal_log_info("PLAT_DBG_INFO base: 0x%x\n", tags->u.plat_dbg_info.info[i].base);
					pal_log_info("PLAT_DBG_INFO size: 0x%x\n", tags->u.plat_dbg_info.info[i].size);
				}
				break;
			case BOOT_TAG_IS_ABNORMAL_BOOT:
                                pal_log_info("is abnormal boot: %d\n", tags->u.is_abnormal_boot.is_abnormal_boot);
                                break;
			case BOOT_TAG_RAM_CONSOLE_INFO:
                                pal_log_info("ram_console info sram_addr: 0x%x\n", tags->u.ram_console_info.sram_addr);
                                pal_log_info("ram_console info sram_size: 0x%x\n", tags->u.ram_console_info.sram_size);
                                pal_log_info("ram_console info def_type: 0x%x\n", tags->u.ram_console_info.def_type);
                                pal_log_info("ram_console memory_info_offset: 0x%x\n", tags->u.ram_console_info.memory_info_offset);
                                break;
        }
	}
}
#endif

void platform_set_boot_args(void)
{
    int i;

#if CFG_ATF_SUPPORT
    u64 sec_addr = 0;
    u32 sec_size = 0;
    tee_get_secmem_start(&sec_addr);
    tee_get_secmem_size(&sec_size);
    bootarg.tee_reserved_mem.start = sec_addr;
    bootarg.tee_reserved_mem.size = sec_size;
#else
    bootarg.tee_reserved_mem.start = 0;
    bootarg.tee_reserved_mem.size = 0;
#endif

#if CFG_BOOT_ARGUMENT_BY_ATAG
    pal_log_info("\n%s boot to LK by ATAG reason=%d addr=%x\n", MOD, g_boot_reason,&g_dram_buf->boottag);
    platform_set_boot_args_by_atag((unsigned int *)&(g_dram_buf->boottag)); // set jump addr
    platform_dump_boot_atag();

    // Switch log port to UART2 while uart meta connected.
    if (g_boot_mode == META_BOOT && g_meta_com_type == META_UART_COM)
    {
        pal_log_info("%s UART META Connected, Switch log port to UART2...\n", MOD);
        mtk_serial_set_current_uart(UART2);
    }
#elif CFG_BOOT_ARGUMENT && !CFG_BOOT_ARGUMENT_BY_ATAG
    bootarg.magic = BOOT_ARGUMENT_MAGIC;
    bootarg.mode  = g_boot_mode;
    bootarg.e_flag = sp_check_platform();
    bootarg.log_port = CFG_UART_LOG;
    bootarg.log_baudrate = CFG_LOG_BAUDRATE;
    bootarg.log_enable = (u8)log_status();
#if !CFG_FPGA_PLATFORM
/*In FPGA phase, dram related function should by pass*/
    bootarg.dram_rank_num = bootarg.mblock_info.mblock_num;
    for (i = 0; i < bootarg.mblock_info.mblock_num; i++) {
        pal_log_info("%s, rank[%d].size = 0x%llx\n", __func__, i,
			(unsigned long long) bootarg.dram_rank_size[i]);
    }
#endif
    bootarg.boot_reason = g_boot_reason;
    bootarg.meta_com_type = (u32)g_meta_com_type;
    bootarg.meta_com_id = g_meta_com_id;
    bootarg.meta_uart_port = CFG_UART_META;
    bootarg.boot_time = get_timer(g_boot_time);

    bootarg.part_num =  g_dram_buf->part_num;
    bootarg.part_info = g_dram_buf->part_info;

    bootarg.ddr_reserve_enable = g_ddr_reserve_enable;
    bootarg.ddr_reserve_success = (g_ddr_reserve_ta_err == 0) ? 1 : 0;
    bootarg.ddr_reserve_ready = g_ddr_reserve_ready;
    bootarg.dram_buf_size =  sizeof(dram_buf_t);

    bootarg.non_secure_sram_addr = CFG_NON_SECURE_SRAM_ADDR;
    bootarg.non_secure_sram_size = CFG_NON_SECURE_SRAM_SIZE;

    pal_log_info("%s NON SECURE SRAM ADDR: 0x%x\n", MOD, bootarg.non_secure_sram_addr);
    pal_log_info("%s NON SECURE SRAM SIZE: 0x%x\n", MOD, bootarg.non_secure_sram_size);

#if CFG_WORLD_PHONE_SUPPORT
    pal_log_info("%s md_type[0] = %d \n", MOD, bootarg.md_type[0]);
    pal_log_info("%s md_type[1] = %d \n", MOD, bootarg.md_type[1]);
#endif

    pal_log_info("\n%s boot reason: %d\n", MOD, g_boot_reason);
    pal_log_info("%s boot mode: %d\n", MOD, g_boot_mode);
    pal_log_info("%s META COM%d: %d\n", MOD, bootarg.meta_com_id, bootarg.meta_com_type);
    pal_log_info("%s <0x%x>: 0x%x\n", MOD, &bootarg.e_flag, bootarg.e_flag);
    pal_log_info("%s boot time: %dms\n", MOD, bootarg.boot_time);
    pal_log_info("%s DDR reserve mode: enable = %d, success = %d, ready = %d\n", MOD, bootarg.ddr_reserve_enable, bootarg.ddr_reserve_success, bootarg.ddr_reserve_ready);
    pal_log_info("%s dram_buf_size: 0x%x\n", MOD, bootarg.dram_buf_size);
    pal_log_info("%s tee_reserved_mem: 0x%llx, 0x%llx\n", MOD, bootarg.tee_reserved_mem.start, bootarg.tee_reserved_mem.size);
    // Switch log port to UART2 while uart meta connected.
    if (g_boot_mode == META_BOOT && g_meta_com_type == META_UART_COM)
    {
        pal_log_info("%s UART META Connected, Switch log port to UART2...\n", MOD);
        bootarg.log_port = UART2;
        mtk_serial_set_current_uart(UART2);
    }
#endif

}

void platform_set_dl_boot_args(da_info_t *da_info)
{
#if CFG_BOOT_ARGUMENT
    if (da_info->addr != BA_FIELD_BYPASS_MAGIC)
	bootarg.da_info.addr = da_info->addr;

    if (da_info->arg1 != BA_FIELD_BYPASS_MAGIC)
	bootarg.da_info.arg1 = da_info->arg1;

    if (da_info->arg2 != BA_FIELD_BYPASS_MAGIC)
	bootarg.da_info.arg2 = da_info->arg2;

    if (da_info->len != BA_FIELD_BYPASS_MAGIC)
	bootarg.da_info.len = da_info->len;

    if (da_info->sig_len != BA_FIELD_BYPASS_MAGIC)
	bootarg.da_info.sig_len = da_info->sig_len;
#endif

    return;
}

void platform_wdt_all_kick(void)
{
    /* kick watchdog to avoid cpu reset */
    mtk_wdt_restart();

#if !CFG_FPGA_PLATFORM
    /* kick PMIC watchdog to keep charging */
    //remove empty function to save space pl_kick_chr_wdt();
#endif
}

void platform_wdt_kick(void)
{
    /* kick hardware watchdog */
    mtk_wdt_restart();
}

#if CFG_DT_MD_DOWNLOAD
void platform_modem_download(void)
{
    pal_log_info("[%s] modem download...\n", MOD);

    /* Switch to MT6261 USB:
     * GPIO_USB_SW1(USB_SW1)=1		//GPIO115, GPIO48
     * GPIO_USB_SW2(USB_SW2)=0		//GPIO116, GPIO196
     * phone
    #define GPIO_UART_URTS0_PIN         (GPIO115 | 0x80000000)
    #define GPIO_UART_UCTS0_PIN         (GPIO116 | 0x80000000)
     * EVB
    #define GPIO_EXT_USB_SW1         (GPIO48 | 0x80000000)
    #define GPIO_EXT_USB_SW2         (GPIO196 | 0x80000000)
     */
#if defined(GPIO_EXT_USB_SW1)
    mt_set_gpio_mode(GPIO_EXT_USB_SW1, GPIO_EXT_USB_SW1_M_GPIO);
    mt_set_gpio_dir(GPIO_EXT_USB_SW1, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_EXT_USB_SW1, GPIO_OUT_ONE);
#endif

#if defined(GPIO_EXT_USB_SW2)
	mt_set_gpio_mode(GPIO_EXT_USB_SW2, GPIO_EXT_USB_SW2_M_GPIO);
    mt_set_gpio_dir(GPIO_EXT_USB_SW2, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_EXT_USB_SW2, GPIO_OUT_ZERO);
#endif

    /* Press MT6261 DL key to enter download mode
     * GPIO_KPD_KROW1_PIN(GPIO_KCOL0)=0 //GPIO120, GPIO105
     * phone
    #define GPIO_KPD_KROW1_PIN         (GPIO120 | 0x80000000)
     * evb
    #define GPIO_EXT_MD_DL_KEY         (GPIO105 | 0x80000000)
     *
     */
#if defined(GPIO_EXT_MD_DL_KEY)
    mt_set_gpio_mode(GPIO_EXT_MD_DL_KEY, GPIO_EXT_MD_DL_KEY_M_GPIO);
    mt_set_gpio_dir(GPIO_EXT_MD_DL_KEY, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_EXT_MD_DL_KEY, GPIO_OUT_ZERO);
#endif

    /* Bring-up MT6261:
     * GPIO_OTG_DRVVBUS_PIN(GPIO_USB_DRVVBUS)=0,
     * GPIO_52_RST(GPIO_RESETB)=INPUT/NOPULL, GPIO118, GPIO166
     #define GPIO_UART_UTXD3_PIN         (GPIO118 | 0x80000000)
     #define GPIO_EXT_MD_RST         (GPIO166 | 0x80000000)
     * GPIO_RST_KEY(GPIO_PWRKEY)=0->1->0
     #define GPIO_UART_URXD3_PIN         (GPIO117 | 0x80000000)
     * GPIO_PWR_KEY(GPIO_PWRKEY)=1
     #define GPIO_EXT_MD_PWR_KEY         (GPIO167 | 0x80000000)
     */
#if defined(GPIO_EXT_MD_RST)
//    mt_set_gpio_mode(GPIO_OTG_DRVVBUS_PIN, GPIO_OTG_DRVVBUS_PIN_M_GPIO);
    mt_set_gpio_mode(GPIO_EXT_MD_RST, GPIO_EXT_MD_RST_M_GPIO);
#endif

    /* MD DRVVBUS to low */
//    mt_set_gpio_dir(GPIO_OTG_DRVVBUS_PIN, GPIO_DIR_OUT);
//    mt_set_gpio_out(GPIO_OTG_DRVVBUS_PIN, GPIO_OUT_ZERO);
#if defined(GPIO_EXT_MD_PWR_KEY)
    mt_set_gpio_mode(GPIO_EXT_MD_PWR_KEY, GPIO_EXT_MD_PWR_KEY_M_GPIO);
    mt_set_gpio_dir(GPIO_EXT_MD_PWR_KEY, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_EXT_MD_PWR_KEY, GPIO_OUT_ONE); /* default @ reset state */
#endif

#if defined(GPIO_EXT_MD_RST)
    /* MD reset pin: hold to zero */
    mt_set_gpio_pull_enable(GPIO_EXT_MD_RST, GPIO_PULL_DISABLE);
    mt_set_gpio_dir(GPIO_EXT_MD_RST, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_EXT_MD_RST, GPIO_OUT_ZERO); /* default @ reset state */
#endif
    mdelay(200);


    /* MD reset pin: released */
#if defined(GPIO_EXT_MD_RST)
    mt_set_gpio_out(GPIO_EXT_MD_RST, GPIO_OUT_ONE);
    mdelay(200);
    mt_set_gpio_dir(GPIO_EXT_MD_RST, GPIO_DIR_IN);
#endif

    pal_log_info("[%s] AP modem download done\n", MOD);
	//keep kick WDT to avoid HW WDT reset
    while (1) {
        platform_wdt_all_kick();
		mdelay(1000);
    }
}
#endif

#if CFG_USB_AUTO_DETECT
void platform_usbdl_flag_check()
{
    U32 usbdlreg = 0;
    usbdlreg = DRV_Reg32(USBDL_FLAG);
     /*Set global variable to record the usbdl flag*/
    if(usbdlreg & USBDL_BIT_EN)
        g_usbdl_flag = 1;
    else
        g_usbdl_flag = 0;
}

void platform_usb_auto_detect_flow()
{

    pal_log_info("USB DL Flag is %d when enter preloader  \n",g_usbdl_flag);

    /*usb download flag haven't set */
	if(g_usbdl_flag == 0 && g_boot_reason != BR_RTC){
        /*set up usbdl flag*/
        platform_safe_mode(1,CFG_USB_AUTO_DETECT_TIMEOUT_MS);
        pal_log_info("Preloader going reset and trigger BROM usb auto detectiton!!\n");

        /*WDT by pass powerkey reboot*/
        //keep the previous status, pass it into reset function
        if(WDT_BY_PASS_PWK_REBOOT == mtk_wdt_boot_check())
            mtk_arch_reset(1);
        else
            mtk_arch_reset(0);

	}else{
    /*usb download flag have been set*/
    }

}
#endif


void platform_safe_mode(int en, u32 timeout)
{
#if !CFG_FPGA_PLATFORM

    U32 usbdlreg = 0;

    /* if anything is wrong and caused wdt reset, enter bootrom download mode */
    timeout = !timeout ? USBDL_TIMEOUT_MAX : timeout / 1000;
    timeout <<= 2;
    timeout &= USBDL_TIMEOUT_MASK; /* usbdl timeout cannot exceed max value */

    usbdlreg |= timeout;
    if (en)
	    usbdlreg |= USBDL_BIT_EN;
    else
	    usbdlreg &= ~USBDL_BIT_EN;

    usbdlreg &= ~USBDL_BROM ;
    /*Add magic number for MT6582*/
    usbdlreg |= USBDL_MAGIC;

    // set BOOT_MISC0 as watchdog resettable
    DRV_WriteReg32(MISC_LOCK_KEY,MISC_LOCK_KEY_MAGIC);
    DRV_SetReg32(RST_CON,1);
    DRV_WriteReg32(MISC_LOCK_KEY,0);

    DRV_WriteReg32(USBDL_FLAG,usbdlreg);

    return;
#endif

}

#if CFG_EMERGENCY_DL_SUPPORT
void platform_emergency_download(u32 timeout)
{
    /* enter download mode */
    pal_log_info("%s emergency download mode(timeout: %ds).\n", MOD, timeout / 1000);
    platform_safe_mode(1, timeout);

#if !CFG_FPGA_PLATFORM
    mtk_arch_reset(0); /* don't bypass power key */
#endif

    while(1);
}
#endif

int platform_get_mcp_id(u8 *id, u32 len, u32 *fw_id_len)
{
    int ret = -1;
    blkdev_t *bootdev = NULL;

    memset(id, 0, len);
    bootdev = blkdev_get(CFG_BOOT_DEV);
    if (!bootdev)
        return -1;

#if !defined(BOOTDEV_SDMMC_UFS_COMBO)
#if (CFG_BOOT_DEV == BOOTDEV_UFS)
    ret = ufs_get_device_id(id, len, fw_id_len);
#elif (CFG_BOOT_DEV == BOOTDEV_SDMMC)
    ret = mmc_get_device_id(id, len,fw_id_len);
#else
    ret = nand_get_device_id(id, len);
#endif
#else
    #if (CFG_BOOT_DEV == BOOTDEV_SDMMC) || (CFG_BOOT_DEV == BOOTDEV_UFS)
    if (bootdev->type == BOOTDEV_UFS)
        ret = ufs_get_device_id(id, len, fw_id_len);
    else
        ret = mmc_get_device_id(id, len, fw_id_len);
    #else
    ret = nand_get_device_id(id, len);
    #endif
#endif
    return ret;
}

void platform_set_chrg_cur(int ma)
{
    //remove empty function to save space hw_set_cc(ma);
}

static boot_reason_t platform_boot_status(void)
{
#if !CFG_FPGA_PLATFORM

	u32 pmic_reboot = get_pmic_boot_status();
	u32 val;

    if (rtc_boot_check()) {
        pal_log_err("%s RTC boot!\n", MOD);
        return BR_RTC;
    }
    BOOTING_TIME_PROFILING_LOG("check Boot status-RTC");

	if (pmic_reboot == 1) {
		pal_log_err("%s pmic: power exception(OC/PG)!\n", MOD);
		rtc_mark_bypass_pwrkey();
		return BR_POWER_EXC;
	} else if (pmic_reboot == 2) {
		pal_log_err("%s pmic: long power key press reboot!\n", MOD);
		rtc_mark_bypass_pwrkey();
		return BR_LONG_POWKEY;
	} else if (pmic_reboot == 3) {
		val = pmic_get_auxadc_value(AUXADC_LIST_BATADC);
		if (val < 3400) {
			pal_log_err("%s pmic: battery vol is %d!\n", MOD, val);
		} else if(rtc_2sec_reboot_check()) {
			pal_log_err("%s pmic: 2sec reboot!\n", MOD);
			return BR_2SEC_REBOOT;
		} else {
			pal_log_err("%s pmic: power miss, battery vol is %d!\n", MOD, val);
			rtc_mark_bypass_pwrkey();
			return BR_POWER_LOSS;
		}
	}

    BOOTING_TIME_PROFILING_LOG("check Boot status-PMIC");
#endif

    if (mtk_wdt_boot_check() == WDT_NORMAL_REBOOT) {
        pal_log_err("%s WDT normal boot!\n", MOD);
        return BR_WDT;
    } else if (mtk_wdt_boot_check() == WDT_BY_PASS_PWK_REBOOT){
        pal_log_err("%s WDT reboot bypass power key!\n", MOD);
        return BR_WDT_BY_PASS_PWK;
    }
    BOOTING_TIME_PROFILING_LOG("check Boot status-WDT");

#if !CFG_FPGA_PLATFORM
    /* check power key */
    if ((mtk_detect_key(PL_PMIC_PWR_KEY) && !is_pwrkey_short_press() && hw_check_battery())
        || is_pmic_long_press_reset()) {
        pal_log_err("%s Power key boot!\n", MOD);
        rtc_mark_bypass_pwrkey();
        return BR_POWER_KEY;
    }
    BOOTING_TIME_PROFILING_LOG("check Boot status-PWR key");
#endif



    if(drdi_get_hw_ver() == HW_VER_V1) {
    if (usb_accessory_in()) {
        pal_log_err("%s USB/charger boot!\n", MOD);
        BOOTING_TIME_PROFILING_LOG("check Boot status-usb_accessory_in");
        return BR_USB;
    }
    //need to unlock rtc PROT
    //check after rtc_boot_check() initial finish.
	if (rtc_2sec_reboot_check()) {
		pal_log_err("%s 2sec reboot!\n", MOD);
		BOOTING_TIME_PROFILING_LOG("check Boot status-rtc_2sec_reboot_check");
		return BR_2SEC_REBOOT;
	}

    pal_log_err("%s Unknown boot!\n", MOD);
    pl_power_off();
    /* should nerver be reached */
    }

    pal_log_err("%s Power key boot!\n", MOD);

    return BR_POWER_KEY;
}

#if CFG_LOAD_DSP_ROM || CFG_LOAD_MD_ROM
int platform_is_three_g(void)
{
    u32 tmp = sp_check_platform();

    return (tmp & 0x1) ? 0 : 1;
}
#endif

chip_ver_t mt_get_chip_sw_ver(void)
{
    chip_ver_t sw_ver;
    unsigned int ver;

    ver = DRV_Reg32(APSW_VER);
    if (0 == ver)
        sw_ver = CHIP_VER_E1;
    else  //0x1
        sw_ver = CHIP_VER_E2;

    return sw_ver;
}

// ------------------------------------------------
// detect download mode
// ------------------------------------------------

bool platform_com_wait_forever_check(void)
{
#ifdef USBDL_DETECT_VIA_KEY
    /* check download key */
    if (TRUE == mtk_detect_key(COM_WAIT_KEY)) {
        pal_log_info("%s COM handshake timeout force disable: Key\n", MOD);
        return TRUE;
    }
#endif

#ifdef USBDL_DETECT_VIA_AT_COMMAND
    pal_log_info("platform_com_wait_forever_check\n");
    /* check SRAMROM_USBDL_TO_DIS */
    if (USBDL_TO_DIS == (INREG32(SRAMROM_USBDL_TO_DIS) & USBDL_TO_DIS)) {
	pal_log_info("%s COM handshake timeout force disable: AT Cmd\n", MOD);
	CLRREG32(SRAMROM_USBDL_TO_DIS, USBDL_TO_DIS);
	return TRUE;
    }
#endif

    return FALSE;
}

u64 platform_memory_size(void)
{
    static u64 mem_size = 0;
    int nr_bank;
    int i;
    u64 rank_size[4], *rksize = &rank_size[0];
    u64 size = 0;

    if (!mem_size) {
        nr_bank = get_dram_rank_nr();

        get_dram_rank_size(rank_size);

        for (i = 0; i < nr_bank; i++)
            size += *rksize++;
        mem_size = size;
    }

    return mem_size;
}

void bandgap_filter_init(void)
{
    pal_log_info("bandgap filter: 0x1000C600 = 0x%x\n", DRV_Reg32(0x1000C600));
    DRV_WriteReg32(0x1000C600, (DRV_Reg32(0x1000C600) & ~(0xF))); // 0x1000C600[3:0] = 0
    pal_log_info("bandgap filter: 0x1000C600 = 0x%x\n", DRV_Reg32(0x1000C600));
    return;
}

void platform_pre_init(void)
{
    u32 pmic_ret=0;
    u32 i=0;
#ifdef EARLY_PARTITION_ACCESS
    int ret = 0;
#endif
    struct rtc_time tm;


#ifdef MTK_TEST_TMODE
	trigger_s36_test_even(MTK_TEST_TMODE,false);
#endif

    /* init timer */
    mtk_timer_init();

    /* init boot time */
    g_boot_time = get_timer(0);


#if !CFG_FPGA_PLATFORM
    /* init pll */
    mt_pll_init();
    BOOTING_TIME_PROFILING_LOG("PLL");
#endif

	/* ALPS04154002: Bandgap Filter Setting init, MT6779 only*/
	bandgap_filter_init();

    /*GPIO init*/
    mt_gpio_init();
    BOOTING_TIME_PROFILING_LOG("GPIO");

    mtk_wdt_pre_init();
    BOOTING_TIME_PROFILING_LOG("WDT_PRE");

#if !CFG_FPGA_PLATFORM

    #if (MTK_UART_USB_SWITCH)
	if (is_uart_cable_inserted()) {
		pal_log_info("\n%s Switch to UART Mode\n", MOD);
		set_to_uart_mode();
	} else {
		pal_log_info("\n%s Keep stay in USB Mode\n", MOD);
	}
    BOOTING_TIME_PROFILING_LOG("USB SWITCH to UART");
    #endif
#endif

    //retry 3 times for pmic wrapper init
    pwrap_init_preloader();
    BOOTING_TIME_PROFILING_LOG("PWRAP");

    //print rtc time
    rtc_get_time(&tm);
    pal_log_info("Current RTC time:[%d/%d/%d %d:%d:%d]\n", tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    //i2c hw init
    i2c_hw_init();
    BOOTING_TIME_PROFILING_LOG("I2C");

    //enable_devinfo_dcm
    enable_devinfo_dcm();

    //show ic debug information
    show_devinfo_debug_info();
    BOOTING_TIME_PROFILING_LOG("DEVINFO");

#if !CFG_FPGA_PLATFORM
    pmic_ret = pmic_init();
    BOOTING_TIME_PROFILING_LOG("PMIC");

#if CFG_CPU_FAST_FREQ
	/* adjust vproc/vsram */
	buck_adjust_voltage(VSRAM_PROC, PMIC_VOLT_01_200000_V);
	buck_adjust_voltage(VPROC, PMIC_VOLT_01_000000_V);
#else
	pal_log_info("vproc/vsram run as hw default\n");
#endif
#endif

#ifdef EARLY_PARTITION_ACCESS
    /*
     * Init booting storage device.
     *
     * If CFG_DRAM_CALIB_OPTIMIZATION is enabled, booting storage device
     * can be initialized before DRAM initialization.
     */
    ret = boot_device_init();
    BOOTING_TIME_PROFILING_LOG("Boot dev init");
    pal_log_err("%s Init Boot Device: %s(%d)\n", MOD, ret ? "FAIL" : "OK", ret);

    //call ab_ota_boot_check here to detect boot status as earlier as possible when CFG_DRAM_CALIB_OPTIMIZATION is enabled
#if defined(MTK_AB_OTA_UPDATER)
    ab_ota_boot_check();
#endif // if defined(MTK_AB_OTA_UPDATER)
#endif // EARLY_PARTITION_ACCESS

    //enable long press reboot function
    PMIC_enable_long_press_reboot();
    BOOTING_TIME_PROFILING_LOG("Long Pressed Reboot");

    pal_log_info("%s Init PMIC: %s(%d)\n", MOD, pmic_ret ? "FAIL" : "OK", pmic_ret);
    pal_log_info("%s chip_ver[%x]\n", MOD, mt_get_chip_sw_ver());
    pal_log_info("%s, additional INFO<%x>\n", MOD, seclib_get_devinfo_with_index(28));
#ifdef SLT
    pal_log_info("chip segment code: 0x%x\n",
        seclib_get_devinfo_with_index(30));
    pal_log_info("chip huid: 0x%x 0x%x 0x%x 0x%x\n",
        seclib_get_devinfo_with_index(12),
        seclib_get_devinfo_with_index(13),
        seclib_get_devinfo_with_index(14),
        seclib_get_devinfo_with_index(15));
#endif
}


#ifdef MTK_MT8193_SUPPORT
extern int mt8193_init(void);
#endif

static void cache_delsel_init(void)
{
	writel(0x20, MCUCFG_BASE+0xcd20);
}

static void peribus_init(void)
{
    /*
     * enable audio bus node with burst mode
     * to improve audio performance
     */
    writel(0x0000007F, PERICFG_BASE+0x220);
}

void platform_init(void)
{
    u32 ret;
    boot_reason_t reason;

#ifdef MTK_EFUSE_WRITER_RESERVE_CODESIZE
    struct efuse_param param = {0};
#endif

    lastpc_init();
    BOOTING_TIME_PROFILING_LOG("lastpc");

#if !CFG_FPGA_PLATFORM
    if (clk_buf_init())
	    pal_log_err("%s: clk_buf_init fail\n", __func__);
    rtc_init();
    BOOTING_TIME_PROFILING_LOG("rtc");

    pmic_init_setting();
    BOOTING_TIME_PROFILING_LOG("pmic");
#endif

    /* init vcore opp table */
    dvfsrc_opp_level_mapping();
    BOOTING_TIME_PROFILING_LOG("dvfsrc_opp_level_mapping");

#if !CFG_BYPASS_EMI
    /* check DDR-reserve mode */
    check_ddr_reserve_status();
    BOOTING_TIME_PROFILING_LOG("chk DDR Reserve status");
#endif

    /* init watch dog, will enable AP watch dog */
    mtk_wdt_init();
    /*init kpd PMIC mode support*/
    set_kpd_pmic_mode();


#if CFG_MDWDT_DISABLE
    /* no need to disable MD WDT, the code here is for backup reason */

    /* disable MD0 watch dog. */
    DRV_WriteReg32(0x20050000, 0x2200);

    /* disable MD1 watch dog. */
    DRV_WriteReg32(0x30050020, 0x2200);
#endif


#if !CFG_FPGA_PLATFORM/* FIXME */
    g_boot_reason = reason = platform_boot_status();
    BOOTING_TIME_PROFILING_LOG("check Boot status");

    if (reason == BR_RTC || reason == BR_POWER_KEY || reason == BR_USB || reason == BR_WDT || reason == BR_WDT_BY_PASS_PWK || reason == BR_2SEC_REBOOT)
        rtc_bbpu_power_on();

    pmic_dbg_status(0);
#else

    g_boot_reason = BR_POWER_KEY;

#endif
    BOOTING_TIME_PROFILING_LOG("rtc_bbpu_power_on");

#if !CFG_FPGA_PLATFORM
    pl_battery_init(false);
#endif

    enable_PMIC_kpd_clock();
    BOOTING_TIME_PROFILING_LOG("Enable PMIC Kpd clk");

#if CFG_UART_DYNAMIC_SWITCH
#ifdef KPD_DL_KEY1
    mtk_kpd_gpio_set();
    if (mtk_detect_key(KPD_DL_KEY1)) {
        pal_log_info("Vol Up detected. Log Keep on.\n");
    } else {
        pal_log_err("Log Turned Off.\n");
        set_log_switch(0);
    }
    BOOTING_TIME_PROFILING_LOG("UART DYNAMIC SWITCH");
#else
    pal_log_err("KPD_DL_KEY1 not define\n");
#endif
#endif //CFG_UART_DYNAMIC_SWITCH

#ifdef MTK_TRNG_CALIBRATION
#if defined MTK_TRNG_CALIBRATION_1ST_ROUND
	ret = trng_calib_1st_round(g_trng_buf, TRNG_CALIB_BUF_SIZE);
	if (ret) {
		pal_log_err("trng calibration failed\n");
	}
	/* trng calibration terminates here */
	while(1);
#endif //MTK_TRNG_CALIBRATION_1ST_ROUND
#endif //MTK_TRNG_CALIBRATION
#if CFG_ENABLE_DCACHE
    /* initialize page table, MMU, and data cache */
    paging_init();
    plat_set_dcache_status(true);
#endif
    /* init memory */
#if !CFG_BYPASS_EMI
#if !CFG_FPGA_PLATFORM
    mt_mem_init();
    BOOTING_TIME_PROFILING_LOG("mem_init");
#endif
#endif

#if CFG_ENABLE_DCACHE
    /* disable data cache and MMU before leaving preloader */
    paging_uninit();
    plat_set_dcache_status(false);
#endif

    /*init dram buffer*/
    init_dram_buffer();
    BOOTING_TIME_PROFILING_LOG("Init Dram buf");

    /* switch log buffer to dram */
    //log_buf_ctrl(1);
#if 0 /* FIXME */
    /* enable CA9 share bits for USB(30)/NFI(29)/MSDC(28) modules to access ISRAM */
    tmp = DRV_Reg32(0xC1000200);
    tmp |= ((1<<30)|(1<<29)|(1<<28));
    DRV_WriteReg32 (0xC1000200, tmp);
#endif

#ifdef MTK_MT8193_SUPPORT
	mt8193_init();
#endif

#if !CFG_BYPASS_EMI
#if CFG_RAM_CONSOLE
	ram_console_init();
	ram_console_reboot_reason_save(g_rgu_status);
	BOOTING_TIME_PROFILING_LOG("Ram console");
#endif
#endif

#ifndef EARLY_PARTITION_ACCESS
	/*
	 * Init booting storage device.
	 *
	 * If CFG_DRAM_CALIB_OPTIMIZATION is disabled or not defined, booting storage device
	 * needs to be initialized after DRAM initialization.
	 */
    ret = boot_device_init();
    BOOTING_TIME_PROFILING_LOG("Boot dev init");
    pal_log_err("%s Init Boot Device: %s(%d)\n", MOD, ret ? "FAIL" : "OK", ret);
#endif // EARLY_PARTITION_ACCESS

#if CFG_EMERGENCY_DL_SUPPORT
    /* check if to enter emergency download mode */
    /* Move after dram_inital and boot_device_init.
      Use excetution time to remove delay time in mtk_kpd_gpio_set()*/
    if (mtk_detect_dl_keys()) {
        platform_emergency_download(CFG_EMERGENCY_DL_TIMEOUT_MS);
    }
#endif

#ifdef MTK_TRNG_CALIBRATION
#if defined MTK_TRNG_CALIBRATION_2ND_ROUND
	ret = trng_calib_2nd_round(g_trng_buf, TRNG_CALIB_BUF_SIZE);
	if (ret) {
		pal_log_err("trng calibration failed\n");
	}
	/* trng calibration terminates here */
	while(1);
#endif //MTK_TRNG_CALIBRATION_2ND_ROUND
#endif //MTK_TRNG_CALIBRATION

#ifdef MTK_EFUSE_WRITER_RESERVE_CODESIZE
#ifdef MTK_EFUSE_WRITER_SUPPORT
    efuse_wdt_restart();
    param.magic_key1 = EFUSE_BLOW_KEY1;
    param.magic_key2 = EFUSE_BLOW_KEY2;
    param.enable_self_blow = TRUE;
#else
    param.magic_key1 = 0xFEEEEEEF;
    param.magic_key2 = 0xFEEEEEEF;
    param.enable_self_blow = FALSE;
#endif
    param.clear_part_dis = FALSE;
    ret = efuse_write_all(param);
    pal_log_info("%s Efuse status(%x)\n", MOD, ret);
    BOOTING_TIME_PROFILING_LOG("EFUSE Self Blow");
#else
    pal_log_info("%s MTK_EFUSE_WRITER_RESERVE_CODESIZE off\n", MOD);
#endif

#if CFG_REBOOT_TEST
    mtk_wdt_sw_reset();
    while(1);
#endif

	/* make sure usb11 in power saving mode*/
	mt_usb11_phy_savecurrent();

#if !CFG_FPGA_PLATFORM
/*In FPGA phase, dram related function should by pass*/
    bootarg.dram_rank_num = get_dram_rank_nr();
    get_dram_rank_size(bootarg.dram_rank_size);
    get_orig_dram_rank_info(&bootarg.orig_dram_info);
    setup_mblock_info(&bootarg.mblock_info, &bootarg.orig_dram_info, &bootarg.lca_reserved_mem);
    reserve_dramc_dummy_read();
#if CFG_LAST_EMI_BW_DUMP
    reserve_emi_mbw_buf();
#endif
#else
    /*pass a defaut dram info to LK*/ //512 + 512MB
    bootarg.dram_rank_num = 0x00000002;
    bootarg.dram_rank_size[0] = 0x20000000;
    bootarg.dram_rank_size[1] = 0x20000000;
    bootarg.dram_rank_size[2] = 0xdfffffff;
    bootarg.dram_rank_size[3] = 0xfffffdff;
    bootarg.mblock_info.mblock_num = 0x00000002;
    bootarg.mblock_info.mblock[0].start = 0x40000000;
    bootarg.mblock_info.mblock[0].size = 0x20000000;
    bootarg.mblock_info.mblock[0].rank = 0;
    bootarg.mblock_info.mblock[1].start = 0x60000000;
    bootarg.mblock_info.mblock[1].size = 0x1fe00000;
    bootarg.mblock_info.mblock[1].rank = 1;
    bootarg.mblock_info.mblock[2].start = 0;
    bootarg.mblock_info.mblock[2].size = 0;
    bootarg.mblock_info.mblock[2].rank = 0;
    bootarg.mblock_info.mblock[3].start = 0;
    bootarg.mblock_info.mblock[3].size = 0;
    bootarg.mblock_info.mblock[3].rank = 0;
    bootarg.orig_dram_info.rank_num = 2;
    bootarg.orig_dram_info.rank_info[0].start = 0x40000000;
    bootarg.orig_dram_info.rank_info[0].size = 0x20000000;
    bootarg.orig_dram_info.rank_info[1].start = 0x60000000;
    bootarg.orig_dram_info.rank_info[1].size = 0x20000000;
    bootarg.orig_dram_info.rank_info[2].start = 0;
    bootarg.orig_dram_info.rank_info[2].size = 0;
    bootarg.orig_dram_info.rank_info[3].start = 0;
    bootarg.orig_dram_info.rank_info[3].size = 0;
    bootarg.lca_reserved_mem.start = 0;
    bootarg.lca_reserved_mem.size = 0;
#endif
    BOOTING_TIME_PROFILING_LOG("Get DRAM Info");

    cache_delsel_init();

    peribus_init();
}

void platform_bootarg_reserve(void)
{
	u32 pl_bootarg_res = 0;
	u64 limit_addr = PL_BOOTARG_BASE + PL_BOOTARG_MAX_SIZE;
	u64 align_addr = (u64)0x1000;

	/* Reserve dram buffer */
	pl_bootarg_res = (u32)mblock_reserve_ext(&bootarg.mblock_info,
		(u64) PL_BOOTARG_MAX_SIZE, (u64)align_addr, limit_addr, 1, "pl-bootarg");
	if (pl_bootarg_res != PL_BOOTARG_BASE) {
		pal_log_info("pl_bootarg_res:0x%x, should:0x%x, assert!\n", pl_bootarg_res, PL_BOOTARG_BASE);
		ASSERT(0);
	}
}

void platform_post_init(void)
{
#ifdef MTK_SECURITY_SW_SUPPORT
	/*Anti rollback update*/
#ifdef MTK_SECURITY_ANTI_ROLLBACK
	u32 g_pl_otp_status = 0x0;

	g_pl_otp_status = sec_otp_ver_update();
	if (g_pl_otp_status)
		pal_log_info("update fail 0x%x\n", g_pl_otp_status);
#endif
#endif

#if CFG_FUNCTION_PICACHU_SUPPORT
    start_picachu();
    BOOTING_TIME_PROFILING_LOG("picachu");
#endif

	mtk_subpmic_init();
	BOOTING_TIME_PROFILING_LOG("mtk_subpmic");

#if CFG_BATTERY_DETECT
    /* normal boot to check battery exists or not */
    if (g_boot_mode == NORMAL_BOOT && !hw_check_battery() && usb_accessory_in()) {
        pal_log_err("%s Wait for battery inserted...\n", MOD);
        mtk_try_unlock_bat(MTK_UNLOCK_BAT_CV_LEVEL);
        mdelay(1000);
        /* disable pmic pre-charging led */
        //remove empty function to save space pl_close_pre_chr_led();
        /* enable force charging mode */
        //remove empty function to save space pl_charging(1);
        do {
            mdelay(300);
            /* check battery exists or not */
            if (hw_check_battery())
                break;
            /* kick all watchdogs */
            platform_wdt_all_kick();
        } while(1);
        /* disable force charging mode */
        //remove empty function to save space pl_charging(0);
    }
#endif

#if !CFG_FPGA_PLATFORM
    pl_battery_init(true);
#endif
    BOOTING_TIME_PROFILING_LOG("Battery detect");

#if CFG_MDJTAG_SWITCH
    unsigned int md_pwr_con;

    /* md0 default power on and clock on */
    /* md1 default power on and clock off */

    /* ungate md1 */
    /* rst_b = 0 */
    md_pwr_con = DRV_Reg32(0x10006280);
    md_pwr_con &= ~0x1;
    DRV_WriteReg32(0x10006280, md_pwr_con);

    /* enable clksq2 for md1 */
    DRV_WriteReg32(0x10209000, 0x00001137);
    udelay(200);
    DRV_WriteReg32(0x10209000, 0x0000113f);

    /* rst_b = 1 */
    md_pwr_con = DRV_Reg32(0x10006280);
    md_pwr_con |= 0x1;
    DRV_WriteReg32(0x10006280, md_pwr_con);

    /* switch to MD legacy JTAG */
    /* this step is not essentially required */
    BOOTING_TIME_PROFILING_LOG("MTJTAG switch");
#endif

#if CFG_MDMETA_DETECT
    if (g_boot_mode == META_BOOT || g_boot_mode == ADVMETA_BOOT) {
	/* trigger md0 to enter meta mode */
        DRV_WriteReg32(0x20000010, 0x1);
	/* trigger md1 to enter meta mode */
        DRV_WriteReg32(0x30000010, 0x1);
    } else {
	/* md0 does not enter meta mode */
        DRV_WriteReg32(0x20000010, 0x0);
	/* md1 does not enter meta mode */
        DRV_WriteReg32(0x30000010, 0x0);
    }
    BOOTING_TIME_PROFILING_LOG("MTMETA Detect");
#endif

    platform_bootarg_reserve();
    ram_console_mblock_reserve();
    platform_set_boot_args();
    BOOTING_TIME_PROFILING_LOG("Boot Argu");

#if CFG_GZ_PWRAP_ENABLE
    /* enable pwrap mpu protection */
    pwrap_enable_mpu(TRUE);
    BOOTING_TIME_PROFILING_LOG("pwrap mpu");
#endif

}

void platform_error_handler(void)
{
    int i = 0;
    /* if log is disabled, re-init log port and enable it */
    if (log_status() == 0) {
        mtk_uart_init(UART_SRC_CLK_FRQ, CFG_LOG_BAUDRATE);
        log_ctrl(1);
    }
    pal_log_err("PL fatal error...\n");

    #if !CFG_FPGA_PLATFORM
    sec_util_brom_download_recovery_check();
    #endif

#if defined(ONEKEY_REBOOT_NORMAL_MODE_PL) || defined(TWOKEY_REBOOT_NORMAL_MODE_PL)
    /* add delay for Long Preessed Reboot count down
       only pressed power key will have this delay */
    pal_log_info("PL delay for Long Press Reboot\n");
    for ( i=3; i > 0;i-- ) {
        if (mtk_detect_key(PL_PMIC_PWR_KEY)) {
            platform_wdt_kick();
            mdelay(5000);   //delay 5s/per kick,
        } else {
            break; //Power Key Release,
        }
    }
#endif

    /* enter emergency download mode */
    #if CFG_EMERGENCY_DL_SUPPORT
    platform_emergency_download(CFG_EMERGENCY_DL_TIMEOUT_MS);
    #endif

    while(1);
}

void platform_assert(char *file, int line, char *expr)
{
    pal_log_err("<ASSERT> %s:line %d %s\n", file, line, expr);
    platform_error_handler();
}

/******************************************************************************
* NOTE: This function is for the internal projects of MT6785.
*        -----------------------------------------
*       | GPIO_111    GPIO_175    PCB_ID          |
*       |   Low          Low      EVB   (MTK0754) |
*       |   Low          High     Phone (MTK0719) |
*       |   High         Low      Phone (MTK0966) |
*       |   High         High     TBD             |
*        -----------------------------------------
******************************************************************************/
hw_ver_t drdi_get_hw_ver(void)
{
#if CFG_DRDI_EVB_PHONE
    if(!mt_get_gpio_in(GPIO111) && !mt_get_gpio_in(GPIO175)) {
        pal_log_info("Detect GPIO111 Low & GPIO175 Low: EVB\n");
        return HW_VER_V0;
    }
    else if(!mt_get_gpio_in(GPIO111) && mt_get_gpio_in(GPIO175)){
        pal_log_info("Detect GPIO111 Low & GPIO175 High: Phone\n");
        return HW_VER_V1;
    }
    else if(mt_get_gpio_in(GPIO111) && !mt_get_gpio_in(GPIO175)){
        pal_log_info("Detect GPIO111 High & GPIO175 Low: Phone\n");
        return HW_VER_V1;
    }
    else {
        pal_log_info("Undefined Set: Phone\n");
        return HW_VER_V1;
    }
#elif CFG_EVB_PLATFORM
    return HW_VER_V0; // EVB
#else
    return HW_VER_V1; // Phone
#endif
}

#if CFG_FAST_META_SUPPORT
void fast_meta_gpio_init(void)
{
#if CFG_FAST_META_GPIO_CHECK
	/* For meta mode detection:
	* Setup GPIOx as GPIO mode, Input, Pullup
	*/
	mt_set_gpio_mode(0x80000000 | CFG_FAST_META_GPIO, GPIO_MODE_GPIO);
	mt_set_gpio_dir(0x80000000 | CFG_FAST_META_GPIO, GPIO_DIR_IN);
	mt_set_gpio_pull_enable(0x80000000 | CFG_FAST_META_GPIO, GPIO_PULL_ENABLE);
	mt_set_gpio_pull_select(0x80000000 | CFG_FAST_META_GPIO, GPIO_PULL_UP);
#endif
}

bool check_meta_short_pin(void)
{
#if CFG_FAST_META_GPIO_CHECK
    S32 val;
    pal_log_err("Detect fastmeta pin %d: mode %d, out %d, pull en %d, pull up %d, value %d.\n",
        CFG_FAST_META_GPIO,
        mt_get_gpio_mode(0x80000000 | CFG_FAST_META_GPIO),
        mt_get_gpio_dir(0x80000000 | CFG_FAST_META_GPIO),
        mt_get_gpio_pull_enable(0x80000000 | CFG_FAST_META_GPIO),
        mt_get_gpio_pull_select(0x80000000 | CFG_FAST_META_GPIO),
        val = mt_get_gpio_in(0x80000000 | CFG_FAST_META_GPIO));
    if(!val) { // detect GPIO pull LOW
        pal_log_err("Enable fastmeta.\n");
        return TRUE;
    }
    else
#endif
        return FALSE;
}

unsigned char HEX_FACTORY_PATTERN[8] = {0x46, 0x41, 0x43, 0x54, 0x4f, 0x52, 0x59, 0x21}; // FACTORY!
#define PROINFO_PATTERN_INDEX    338
#define PROINFO_META_BOOT_OFFSET    346
#define PROINFO_META_LOG_OFFSET    349
void read_meta_proinfo(u8 *meta_boot, u8*meta_log)
{
    blkdev_t *bootdev = NULL;
    part_t *part        = NULL;
    u64 part_proinfo_addr = 0;
    int ret = 0;
    int i = 0, is_pattern_match = 0;

    #if CFG_UART_DYNAMIC_SWITCH
    u8 proinfo_debug = 0;
    if(!get_log_switch()) {
        proinfo_debug = 1;
        set_log_switch(1);
    }
    #endif

    if (NULL == (bootdev = blkdev_get(CFG_BOOT_DEV))) {
        pal_log_err("[proinfo] can't find boot device(%d)\n", CFG_BOOT_DEV);
        goto exit;
    }

    //get part addr
    if(NULL == (part = get_part("proinfo"))) {
        pal_log_err("[proinfo] get_part fail\n");
        goto exit;
    }
    else {
        part_proinfo_addr = (u64)(part->start_sect)*bootdev->blksz;
    }

    if (part_proinfo_addr != 0x0)
        pal_log_info("[proinfo] part addr is 0x%llx\n", part_proinfo_addr);
    else {
        pal_log_err("[proinfo] part addr is incorrect !\n");
    }

    ret = blkdev_read(bootdev, part_proinfo_addr, sizeof(g_dram_buf->proinfo), g_dram_buf->proinfo, part->part_id);
    if (ret != 0) {
        pal_log_err("[proinfo] blkdev_read failed\n");
    }
    else
    {
        for(i=0;i<=7;i++) {
            if(g_dram_buf->proinfo[PROINFO_PATTERN_INDEX+i] == HEX_FACTORY_PATTERN[i]) {
                if(i==7)
                    is_pattern_match = 1;
            }
            else
                break;
        }

        if(is_pattern_match) {
            pal_log_err("[proinfo] FACTORY! matched.\n");

            pal_log_err("[proinfo] meta mode[%d] = 0x%x\n", PROINFO_META_BOOT_OFFSET, g_dram_buf->proinfo[PROINFO_META_BOOT_OFFSET]);
            *meta_boot = g_dram_buf->proinfo[PROINFO_META_BOOT_OFFSET];

            pal_log_err("[proinfo] meta log[%d] = 0x%x\n", PROINFO_META_LOG_OFFSET, g_dram_buf->proinfo[PROINFO_META_LOG_OFFSET]);
            *meta_log = g_dram_buf->proinfo[PROINFO_META_LOG_OFFSET];
        }
        else {
            pal_log_info("[proinfo] FACTORY! NOT matched.\n");

            *meta_boot = 0;
            #if defined(TARGET_BUILD_VARIANT_ENG) || defined(TARGET_BUILD_VARIANT_USERDEBUG)
                *meta_log = 0;
            #else
                *meta_log = 1;
            #endif
        }
     }

exit:
    #if CFG_UART_DYNAMIC_SWITCH
    if(proinfo_debug) {
        proinfo_debug = 0;
        set_log_switch(0);
    }
    #endif
    return;
}
#endif

#if CFG_MICROTRUST_TEE_SUPPORT
u32 platform_chip_hw_code(void)
{
    return DRV_Reg32(APHW_CODE);
}
#endif

#if CFG_ENABLE_DCACHE
void plat_clean_invalidate_dcache(void)
{
    if(plat_get_dcache_status())
        clean_invalidate_dcache();
}

bool plat_get_dcache_status(void)
{
    return g_is_dcache_enable;
}

void plat_set_dcache_status(bool bEnable)
{
    g_is_dcache_enable = bEnable;
}
#endif
