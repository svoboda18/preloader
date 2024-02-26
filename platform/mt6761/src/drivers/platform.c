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
#include "lastpc.h"
#include <clkbuf_ctl.h>
#include <sec_efuse.h>
#include <tz_mem.h>
#include <mtk_subpmic.h>
#include <anti_rollback.h>
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
u32 g_pl_otp_status = 0x0;
extern unsigned int part_num;
extern part_hdr_t   part_info[PART_MAX_NUM];
extern u32 g_max_cpus;
extern bool ram_console_is_abnormal_boot(void);
extern bl_param_t *p_bldr_param;

#if defined (MTK_KERNEL_POWER_OFF_CHARGING)
kal_bool kpoc_flag = false;
#endif

#if CFG_BOOT_ARGUMENT
#define bootarg g_dram_buf->bootarg
#endif

#if CFG_USB_AUTO_DETECT
bool g_usbdl_flag;
#endif

#if CFG_FAST_META_SUPPORT
unsigned char g_is_fast_meta = 0;
#endif

CHARGER_TYPE g_charger_type;
/*============================================================================*/
/* EXTERNAL FUNCTIONS                                                         */
/*============================================================================*/
static u32 boot_device_init(void)
{
    #if (CFG_BOOT_DEV == BOOTDEV_UFS)
    return (u32)ufs_init_device();
    #elif (CFG_BOOT_DEV == BOOTDEV_SDMMC)
    return (u32)mmc_init_device();
    #else
    return (u32)nand_init_device();
    #endif
}

void platform_set_chrg_cur(int ma)
{
    hw_set_cc(ma);
}

int usb_accessory_in(void)
{
#if !CFG_FPGA_PLATFORM
    int exist = 0;

    if (PMIC_CHRDET_EXIST == pmic_IsUsbCableIn()) {
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

    if ((g_boot_reason == BR_USB) || usb_accessory_in()) {
        g_charger_type = mt_charger_type_detection();
        if (g_charger_type == STANDARD_HOST || g_charger_type == CHARGING_HOST) {
            pal_log_info("\n%s USB cable in\n", MOD);
            mt_usb_phy_poweron();
            mt_usb_phy_savecurrent();

            exist = 1;
        } else if (g_charger_type == NONSTANDARD_CHARGER || g_charger_type == STANDARD_CHARGER) {
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

void set_to_usb_mode(void)
{
		UINT8 var;
#if !CFG_FPGA_PLATFORM
        #if 0
		/* Turn on USB MCU Bus Clock */
		var = READ_REG(PERI_GLOBALCON_PDN0_SET);
		pal_log_debug("\n[USB]USB bus clock: 0x008, value: %x\n", var);
		USB_CLR_BIT(USB0_PDN, PERI_GLOBALCON_PDN0_SET);
		var = READ_REG(PERI_GLOBALCON_PDN0_SET);
		pal_log_debug("\n[USB]USB bus clock: 0x008, value after: %x\n", var);
        #endif

		/* Switch from BC1.1 mode to USB mode */
		var = USBPHY_READ8(0x1A);
		pal_log_debug("\n[USB]addr: 0x1A, value: %x\n", var);
		USBPHY_WRITE8(0x1A, var & 0x7f);
		pal_log_debug("\n[USB]addr: 0x1A, value after: %x\n", USBPHY_READ8(0x1A));

		/* Set RG_UART_EN to 0 */
		var = USBPHY_READ8(0x6E);
		pal_log_debug("\n[USB]addr: 0x6E, value: %x\n", var);
		USBPHY_WRITE8(0x6E, var & ~0x01);
		pal_log_debug("\n[USB]addr: 0x6E, value after: %x\n", USBPHY_READ8(0x6E));

		/* Set RG_USB20_DM_100K_EN to 0 */
		var = USBPHY_READ8(0x22);
		pal_log_debug("\n[USB]addr: 0x22, value: %x\n", var);
		USBPHY_WRITE8(0x22, var & ~0x02);
		pal_log_debug("\n[USB]addr: 0x22, value after: %x\n", USBPHY_READ8(0x22));
#else
		/* Set RG_UART_EN to 0 */
		USBPHY_I2C_READ8(0x6E, &var);
		pal_log_debug("\n[USB]addr: 0x6E, value: %x\n", var);
		USBPHY_I2C_WRITE8(0x6E, var & ~0x01);
		USBPHY_I2C_READ8(0x6E, &var);
		pal_log_debug("\n[USB]addr: 0x6E, value after: %x\n", var);

		/* Set RG_USB20_DM_100K_EN to 0 */
		USBPHY_I2C_READ8(0x22, &var);
		pal_log_debug("\n[USB]addr: 0x22, value: %x\n", var);
		USBPHY_I2C_WRITE8(0x22, var & ~0x02);
		USBPHY_I2C_READ8(0x22, &var);
		pal_log_debug("\n[USB]addr: 0x22, value after: %x\n", var);
#endif
		var = READ_REG(UART1_BASE + 0x90);
		pal_log_debug("\n[USB]addr: 0x11002090 (UART1), value: %x\n", var);
		WRITE_REG(var & ~0x01, UART1_BASE + 0x90);
		pal_log_debug("\n[USB]addr: 0x11002090 (UART1), value after: %x\n", READ_REG(UART1_BASE + 0x90));
}

void set_to_uart_mode(void)
{
#if !CFG_FPGA_PLATFORM
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
#else
		UINT8 var;

		/* Set ru_uart_mode to 2'b01 */
		USBPHY_I2C_READ8(0x6B, &var);
		pal_log_debug("\n[USB]addr: 0x6B, value: %x\n", var);
		USBPHY_I2C_WRITE8(0x6B, var | 0x7C);
		USBPHY_I2C_READ8(0x6B, &var);
		pal_log_debug("\n[USB]addr: 0x6B, value after: %x\n", var);

		/* Set RG_UART_EN to 1 */
		USBPHY_I2C_READ8(0x6E, &var);
		pal_log_debug("\n[USB]addr: 0x6E, value: %x\n", var);
		USBPHY_I2C_WRITE8(0x6E, var | 0x07);
		USBPHY_I2C_READ8(0x6E, &var);
		pal_log_debug("\n[USB]addr: 0x6E, value after: %x\n", var);

		/* Set RG_USB20_DM_100K_EN to 1 */
		USBPHY_I2C_READ8(0x22, &var);
		pal_log_debug("\n[USB]addr: 0x22, value: %x\n", var);
		USBPHY_I2C_WRITE8(0x22, var | 0x02);
		USBPHY_I2C_READ8(0x22, &var);
		pal_log_debug("\n[USB]addr: 0x22, value after: %x\n", var);
#endif
}

extern U32 PMIC_VUSB_EN(void);

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
void platform_set_boot_args_by_atag(unsigned int *ptr)
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
#if CFG_FAST_META_SUPPORT
    if(g_is_fast_meta)
        tags->u.boot_mode.boot_mode = META_BOOT;
    else
 #endif
        tags->u.boot_mode.boot_mode = g_boot_mode;
    ptr += tags->hdr.size;

    tags = (boot_tag *)ptr;
    tags->hdr.size = boot_tag_size(boot_tag_meta_com);
    tags->hdr.tag = BOOT_TAG_META_COM;
    tags->u.meta_com.meta_com_id = g_meta_com_id;
#if CFG_FAST_META_SUPPORT
    if(g_is_fast_meta) {
        tags->u.meta_com.meta_com_type = META_USB_COM;
        if(!g_meta_log_disable)
            tags->u.meta_com.meta_com_id |= 0x00000004;
        else
            tags->u.meta_com.meta_com_id &= ~0x00000004;
    }
    else
 #endif
   {
        tags->u.meta_com.meta_com_type = (u32)g_meta_com_type;
   }
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
	mblock_set_mem_tag(&ptr);
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
    tags->u.boot_time.boot_time = get_timer(g_boot_time);;
    ptr += tags->hdr.size;

#if FEATURE_DOWNLOAD_SCREEN
    tags = (boot_tag *)ptr;
    tags->hdr.size = boot_tag_size(boot_tag_da_info);
    tags->hdr.tag = BOOT_TAG_DA_INFO;
    tags->u.da_info.da_info.addr = bootarg.da_info.addr;
    tags->u.da_info.da_info.arg1 = bootarg.da_info.arg1;
    tags->u.da_info.da_info.arg2 = bootarg.da_info.arg2;
    tags->u.da_info.da_info.len  = bootarg.da_info.len;
    tags->u.da_info.da_info.sig_len = bootarg.da_info.sig_len;
    ptr += tags->hdr.size;
#endif

    tags = (boot_tag *)ptr;
    tags->hdr.size = boot_tag_size(boot_tag_sec_info);
    tags->hdr.tag = BOOT_TAG_SEC_INFO;
    tags->u.sec_info.sec_limit.magic_num = bootarg.sec_limit.magic_num;
    tags->u.sec_info.sec_limit.forbid_mode = bootarg.sec_limit.forbid_mode;
    ptr += tags->hdr.size;

#if defined(DUMMY_AP) || defined(SLT)
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
#endif

#if CFG_LOAD_DSP_ROM || CFG_LOAD_MD_ROM
    tags = (boot_tag *)ptr;
    tags->hdr.size = boot_tag_size(boot_tag_eflag);
    tags->hdr.tag = BOOT_TAG_EFLAG;
    tags->u.eflag.e_flag = sp_check_platform();
    ptr += tags->hdr.size;
#endif

    tags = (boot_tag *)ptr;
    tags->hdr.size = boot_tag_size(boot_tag_ddr_reserve);
    tags->hdr.tag = BOOT_TAG_DDR_RESERVE;
    tags->u.ddr_reserve.ddr_reserve_enable = g_ddr_reserve_enable;
    tags->u.ddr_reserve.ddr_reserve_success = g_ddr_reserve_success;
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
    tags->u.emi_info.dram_type = (mt_get_dram_type() & 0xF) | ((get_dram_mr(5) << 24) & 0xFF000000);
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
    for (i = 0;i < INFO_TYPE_MAX; i++) {
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

#if 0 //no PTP in MT6761
    tags = (boot_tag *)ptr;
    tags->hdr.size  = boot_tag_size(boot_tag_ptp);
    tags->hdr.tag   = BOOT_TAG_PTP;
    mt_set_ptp_info(&bootarg.ptp_volt_info);
    tags->u.ptp_volt.ptp_volt_info.first_volt = bootarg.ptp_volt_info.first_volt;
    tags->u.ptp_volt.ptp_volt_info.second_volt = bootarg.ptp_volt_info.second_volt;
    tags->u.ptp_volt.ptp_volt_info.third_volt = bootarg.ptp_volt_info.third_volt;
    tags->u.ptp_volt.ptp_volt_info.have_550 = bootarg.ptp_volt_info.have_550;
    ptr += tags->hdr.size;
#endif //no PTP in MT6761

    tags = (boot_tag *)ptr;
    tags->hdr.size = boot_tag_size(boot_tag_imgver_info);
    tags->hdr.tag  = BOOT_TAG_IMGVER_INFO;
    tags->u.imgver_info.pl_imgver_status = g_pl_otp_status;
    ptr += tags->hdr.size;

    tags = (boot_tag *)ptr;
    tags->hdr.size = boot_tag_size(boot_tag_max_cpus);
    tags->hdr.tag = BOOT_TAG_MAX_CPUS;
    tags->u.max_cpus.max_cpus = g_max_cpus;
    ptr += tags->hdr.size;

    BOOTING_TIME_PROFILING_LOG("Set Argu - before CHR_INFO");
    tags = (boot_tag *)ptr;
    tags->hdr.size = boot_tag_size(boot_tag_chr_info);
    tags->hdr.tag  = BOOT_TAG_CHR_INFO;
#if CFG_FAST_META_SUPPORT
    if(g_is_fast_meta)
        tags->u.chr_info.charger_type = STANDARD_HOST;
    else
#endif
#if !CFG_FPGA_PLATFORM
    tags->u.chr_info.charger_type = g_charger_type;
#else
    tags->u.chr_info.charger_type = STANDARD_HOST;
#endif

    ptr += tags->hdr.size;
    BOOTING_TIME_PROFILING_LOG("Set Argu - after CHR_INFO");

	tags = (boot_tag *)ptr;
	tags->hdr.size = boot_tag_size(boot_tag_bat_info);
	tags->hdr.tag  = BOOT_TAG_BAT_INFO;
	tags->u.bat_info.boot_voltage = fg_get_vbat();
#if 0 //reduce code size
	tags->u.bat_info.shutdown_time = fg_get_shutdowntime();
#endif
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
				mblock_dump_mem_tag((unsigned int*)tags);
				break;
			case BOOT_TAG_MD_INFO:
				for(i=0;i<4;i++)
					pal_log_info("MD_INFO: 0x%x\n", tags->u.md_info.md_type[i]);
				break;
			case BOOT_TAG_BOOT_TIME:
				pal_log_err("BOOT_TIME: %d\n", tags->u.boot_time.boot_time);
				break;
#if FEATURE_DOWNLOAD_SCREEN
			case BOOT_TAG_DA_INFO:
				pal_log_info("DA_INFO: 0x%x\n", tags->u.da_info.da_info.addr);
				pal_log_info("DA_INFO: 0x%x\n", tags->u.da_info.da_info.arg1);
				pal_log_info("DA_INFO: 0x%x\n", tags->u.da_info.da_info.arg2);
				pal_log_info("DA_INFO: 0x%x\n", tags->u.da_info.da_info.len);
				pal_log_info("DA_INFO: 0x%x\n", tags->u.da_info.da_info.sig_len);
				break;
#endif
			case BOOT_TAG_SEC_INFO:
				pal_log_info("SEC_INFO: 0x%x\n", tags->u.sec_info.sec_limit.magic_num);
				pal_log_info("SEC_INFO: 0x%x\n", tags->u.sec_info.sec_limit.forbid_mode);
				break;
#if defined(DUMMY_AP) || defined(SLT)
			case BOOT_TAG_PART_NUM:
				pal_log_info("PART_NUM: %d\n", tags->u.part_num.part_num);
				break;
			case BOOT_TAG_PART_INFO:
				pal_log_info("PART_INFO: 0x%x\n", tags->u.part_info.part_info);
				break;
#endif
#if CFG_LOAD_DSP_ROM || CFG_LOAD_MD_ROM
			case BOOT_TAG_EFLAG:
				pal_log_info("EFLAG: %d\n", tags->u.eflag.e_flag);
				break;
#endif
			case BOOT_TAG_DDR_RESERVE:
				pal_log_info("DDR_RESERVE: %d\n", tags->u.ddr_reserve.ddr_reserve_enable);
				pal_log_info("DDR_RESERVE: %d\n", tags->u.ddr_reserve.ddr_reserve_success);
				pal_log_info("DDR_RESERVE: %d\n", tags->u.ddr_reserve.ddr_reserve_ready);
				break;
			case BOOT_TAG_DRAM_BUF:
				pal_log_info("DRAM_BUF: %d\n", tags->u.dram_buf.dram_buf_size);
				break;
			case BOOT_TAG_EMI_INFO:
				pal_log_info("EMI dram_type: 0x%x\n", tags->u.emi_info.dram_type);
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
#if 0 //reduce code size
				pal_log_info("shutdown_time: 0x%x\n", tags->u.bat_info.shutdown_time);
#endif
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
    BOOTING_TIME_PROFILING_LOG("Set Argu");
    platform_dump_boot_atag();
    BOOTING_TIME_PROFILING_LOG("Dump Argu");
    // Switch log port to UART2 while uart meta connected.
    if (g_boot_mode == META_BOOT && g_meta_com_type == META_UART_COM)
    {
        pal_log_info("%s UART META Connected, Switch log port to UART2...\n", MOD);
        mtk_serial_set_current_uart(UART2);
    }
#elif CFG_BOOT_ARGUMENT && !CFG_BOOT_ARGUMENT_BY_ATAG
    bootarg.magic = BOOT_ARGUMENT_MAGIC;
    bootarg.mode  = g_boot_mode;
#if CFG_LOAD_DSP_ROM || CFG_LOAD_MD_ROM
    bootarg.e_flag = sp_check_platform();
#endif
    bootarg.log_port = CFG_UART_LOG;
    bootarg.log_baudrate = CFG_LOG_BAUDRATE;
    bootarg.log_enable = (u8)log_status();
#if !CFG_FPGA_PLATFORM
/*In FPGA phase, dram related function should by pass*/
    bootarg.dram_rank_num = bootarg.mblock_info.mblock_num;
    for (i = 0; i < bootarg.mblock_info.mblock_num; i++) {
	    bootarg.dram_rank_size[i] = bootarg.mblock_info.mblock[i].size;
        pal_log_info("%s, rank[%d].size = 0x%llx\n", __func__, i,
			(unsigned long long) bootarg.dram_rank_size[i]);
    }
#endif
    bootarg.boot_reason = g_boot_reason;
    bootarg.meta_com_type = (u32)g_meta_com_type;
    bootarg.meta_com_id = g_meta_com_id;
    bootarg.meta_uart_port = CFG_UART_META;
    bootarg.boot_time = get_timer(g_boot_time);

#if defined(DUMMY_AP) || defined(SLT)
    bootarg.part_num =  g_dram_buf->part_num;
    bootarg.part_info = g_dram_buf->part_info;
#endif

    bootarg.ddr_reserve_enable = g_ddr_reserve_enable;
    bootarg.ddr_reserve_success= g_ddr_reserve_success;
    bootarg.ddr_reserve_success= g_ddr_reserve_ready;
    bootarg.dram_buf_size =  sizeof(dram_buf_t);

#if 0 //no PTP in MT6761
    mt_set_ptp_info(&bootarg.ptp_volt_info);
#endif //no PTP in MT6761

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
    pal_log_info("%s DDR reserve mode: enable = %d, success = %d\n", MOD, bootarg.ddr_reserve_enable, bootarg.ddr_reserve_success);
    pal_log_info("%s dram_buf_size: 0x%x\n", MOD, bootarg.dram_buf_size);
    pal_log_info("%s tee_reserved_mem: 0x%llx, 0x%llx\n", MOD, bootarg.tee_reserved_mem.start, bootarg.tee_reserved_mem.size);
    // Switch log port to UART2 while uart meta connected.
    if (g_boot_mode == META_BOOT && g_meta_com_type == META_UART_COM) {
        pal_log_info("%s UART META Connected, Switch log port to UART2...\n", MOD);
        bootarg.log_port = UART2;
        mtk_serial_set_current_uart(UART2);
    }
#endif

}

#if FEATURE_DOWNLOAD_SCREEN
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
#endif

void platform_wdt_all_kick(void)
{
    /* kick watchdog to avoid cpu reset */
    mtk_wdt_restart();

#if !CFG_FPGA_PLATFORM
    /* kick PMIC watchdog to keep charging */
    pl_kick_chr_wdt();
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

    memset(id, 0, len);

#if (CFG_BOOT_DEV == BOOTDEV_UFS)
    ret = ufs_get_device_id(id, len, fw_id_len);
#elif (CFG_BOOT_DEV == BOOTDEV_SDMMC)
    ret = mmc_get_device_id(id, len,fw_id_len);
#else
    ret = nand_get_device_id(id, len);
#endif

    return ret;
}

#if (CFG_PMIC_FULL_RESET)
bool platform_pmic_full_reset_check(void)
{
	pal_log_info("[Reg check 1][0x%x]=0x%x\n", 0x166, upmu_get_reg_value(0x166));
	pal_log_info("[Reg check 1][0x%x]=0x%x\n", 0x22, upmu_get_reg_value(0x22));
	if(upmu_get_reg_value(0x22)){
		pal_log_info("[Reg check][0x%x]=0x%x\n", 0x22, upmu_get_reg_value(0x22));
		pmic_config_interface(0x22, 0, 0xff, 0); //clear status at 0x22
		pal_log_info("[Reg check][0x%x]=0x%x\n", 0x22, upmu_get_reg_value(0x22));
		return true;
	}
	else
		return false;
}
#endif

static boot_reason_t platform_boot_status(void)
{
#if !CFG_FPGA_PLATFORM
#if (CFG_PMIC_FULL_RESET)
    bool wdt_flag = platform_pmic_full_reset_check();
#endif

	u32 pmic_reboot = get_pmic_boot_status();
	u32 val;
#if defined (MTK_KERNEL_POWER_OFF_CHARGING)
	ulong begin = get_timer(0);
	do  {
		if (rtc_boot_check()) {
			pal_log_err("%s RTC boot!\n", MOD);
			return BR_RTC;
		}
		if(!kpoc_flag)
			break;
	} while (get_timer(begin) < 1000 && kpoc_flag);
#else
    if (rtc_boot_check()) {
        pal_log_err("%s RTC boot!\n", MOD);
        return BR_RTC;
    }

#endif
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

#if (CFG_PMIC_FULL_RESET)
 if(wdt_flag && g_rgu_status == RE_BOOT_REASON_UNKNOW){
	   pal_log_info("%s WDT reboot bypass power key! PMIC Full Reset.\n", MOD);
	   g_rgu_status = RE_BOOT_BY_WDT_SW;
	   return BR_WDT_BY_PASS_PWK;
   }
#endif

#if !CFG_FPGA_PLATFORM
    /* check power key */
    if ((mtk_detect_key(PL_PMIC_PWR_KEY) && hw_check_battery()) || is_pmic_long_press_reset()) {
        pal_log_err("%s Power key boot!\n", MOD);
        rtc_mark_bypass_pwrkey();
        return BR_POWER_KEY;
    }
    BOOTING_TIME_PROFILING_LOG("check Boot status-PWR key");
#endif



#if !CFG_EVB_PLATFORM
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
#endif

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

#ifdef SLT

#define UID_LADDR  (MCUSYS_CFGREG_BASE+0x6140)
#define UID_HADDR  (MCUSYS_CFGREG_BASE+0x6144)


unsigned int get_chip_uid(void){
  return DRV_Reg32(UID_LADDR);
}
unsigned int get_chip_uid2(void){
  return DRV_Reg32(UID_HADDR);
}
#endif

#if CFG_MICROTRUST_TEE_SUPPORT
u32 platform_chip_hw_code(void)
{
    return DRV_Reg32(APHW_CODE);
}
#endif

unsigned int mt_get_chip_hw_ver(void)
{
	return DRV_Reg32(APHW_VER);
}

CHIP_SW_VER mt_get_chip_sw_ver(void)
{
	CHIP_SW_VER sw_ver;
	unsigned int ver;
	//    unsigned int hw_subcode = DRV_Reg32(APHW_SUBCODE);

	ver = DRV_Reg32(APSW_VER);
	if ( 0x0 == ver )
		sw_ver = CHIP_SW_VER_01;
	else
		sw_ver = CHIP_SW_VER_02;
#ifdef SLT
        pal_log_info("chip uid: 0x%x 0x%x\n", get_chip_uid(), get_chip_uid2());
#endif
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

#define MP1_CACHE_MEM_DELSEL0_ADDR 0x10200224
#define MP1_CACHE_MEM_DELSEL0_MASK (0x7 << 3)

bool platform_is_boot_reason_valid(void)
{
    pal_log_info("Boot reason=%d\n", g_boot_reason);
    if (g_boot_reason == BR_RTC || g_boot_reason == BR_POWER_KEY || g_boot_reason == BR_USB || g_boot_reason == BR_WDT || g_boot_reason == BR_WDT_BY_PASS_PWK || g_boot_reason == BR_2SEC_REBOOT)
        return TRUE;
    else
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

void platform_pre_init(void)
{
    u32 pmic_ret=0;
    u32 pwrap_ret=0,i=0;
    u32 ret;

#ifdef MTK_TEST_TMODE
    trigger_s36_test_even(MTK_TEST_TMODE,false);
#endif

    /* init timer */
    mtk_timer_init();

    /* init boot time */
    g_boot_time = get_timer(0);

    BOOTING_TIME_PROFILING_LOG("Preloader Start");
#if !CFG_FPGA_PLATFORM
    /* init pll */
    mt_pll_init();
    BOOTING_TIME_PROFILING_LOG("PLL");
#endif

    //Duplicate init, remove it.
    /* init uart baudrate when pll on */
    //mtk_uart_init(UART_SRC_CLK_FRQ, CFG_LOG_BAUDRATE);

    /*GPIO init*/
    mt_gpio_init();
    BOOTING_TIME_PROFILING_LOG("GPIO");

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

//#if !CFG_FPGA_PLATFORM
    /* init pll post */
//    mt_pll_post_init();
//	BOOTING_TIME_PROFILING_LOG("PLL post");
//#endif

    //retry 3 times for pmic wrapper init
    pwrap_init_preloader();
    BOOTING_TIME_PROFILING_LOG("PWRAP");

    //i2c hw init
    i2c_hw_init();

    //show ic debug information
    show_devinfo_debug_info();

    //enable_devinfo_dcm
    enable_devinfo_dcm();

#ifdef MTK_MT22_MODE
    devinfo_init_mode(MTK_MT22_MODE);
#endif

    BOOTING_TIME_PROFILING_LOG("I2C");
#if !CFG_FPGA_PLATFORM
    pmic_ret = pmic_init();
    BOOTING_TIME_PROFILING_LOG("PMIC");

    mt_pll_post_init();
    BOOTING_TIME_PROFILING_LOG("PLL POST Init");
#endif

#if CFG_DRAM_CALIB_OPTIMIZATION
    /*
     * Init booting storage device.
     *
     * If CFG_DRAM_CALIB_OPTIMIZATION is enabled, booting storage device
     * can be initialized before DRAM initialization.
     */
    ret = boot_device_init();
    BOOTING_TIME_PROFILING_LOG("Boot dev init");
    pal_log_err("%s Init Boot Device: %s(%d)\n", MOD, ret ? "FAIL" : "OK", ret);

    //call ab_ota_boot_check here to detect ota boot status as earlier as possible when CFG_DRAM_CALIB_OPTIMIZATION is enabled
    #if defined(MTK_AB_OTA_UPDATER)
        ab_ota_boot_check();
    #endif
#endif

    //enable long press reboot function
    PMIC_enable_long_press_reboot();
    BOOTING_TIME_PROFILING_LOG("Long Pressed Reboot");

    pal_log_err("%s Init PWRAP: %s(%d)\n", MOD, pwrap_ret ? "FAIL" : "OK", pwrap_ret);
    pal_log_err("%s Init PMIC: %s(%d)\n", MOD, pmic_ret ? "FAIL" : "OK", pmic_ret);


    pal_log_err("%s chip_hw_ver[%x], chip_sw_ver[%x]\n", MOD, mt_get_chip_hw_ver(), mt_get_chip_sw_ver());

#if 0 //no PTP in MT6761 
    if (!pmic_ret)
    	ptp_init();
#endif //no PTP in MT6761
}



static void peribus_init(void)
{
    /*
     * enable audio bus node with burst mode
     * to improve audio performance
     */
    writel(0x00007F00, PERICFG_BASE+0x220);
}

void platform_init(void)
{
    u32 ret, tmp;
    boot_reason_t reason;
    UINT32 var;
#ifdef MTK_EFUSE_WRITER_RESERVE_CODESIZE
    struct efuse_param param = {0};
#endif

    lastpc_init();

#if !CFG_FPGA_PLATFORM
    if (clk_buf_init())
	    pal_log_err("%s: clk_buf_init fail\n", __func__);
    rtc_init();

    if (mt_get_chip_sw_ver() == CHIP_SW_VER_01) {
#ifndef MTK_DREQ_DIS
        var = readl(0x10200B54);
        writel(var | (0x1), 0x10200B54);
#endif
    }
    pmic_init_setting();
#endif

#if !CFG_BYPASS_EMI
    /* check DDR-reserve mode */
    check_ddr_reserve_status();
    BOOTING_TIME_PROFILING_LOG("chk DDR Reserve status");
#endif

    /* init watch dog, will enable AP watch dog */
    mtk_wdt_init();
    /*init kpd PMIC mode support*/
    set_kpd_pmic_mode();

#if !CFG_FPGA_PLATFORM/* FIXME */
    g_boot_reason = reason = platform_boot_status();
    BOOTING_TIME_PROFILING_LOG("check Boot status");

    if (reason == BR_RTC || reason == BR_POWER_KEY || reason == BR_USB || reason == BR_WDT || reason == BR_WDT_BY_PASS_PWK || reason == BR_2SEC_REBOOT)
        rtc_bbpu_power_on();
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
    mtk_kpd_gpio_set();
    if (mtk_detect_key(KPD_DL_KEY1)) {
        pal_log_info("Vol Up detected. Log Keep on.\n");
    } else {
        pal_log_err("Log Turned Off.\n");
        set_log_switch(0);
    }
    BOOTING_TIME_PROFILING_LOG("UART DYNAMIC SWITCH");
#endif

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

    /* init memory */
#if !CFG_BYPASS_EMI
#if !CFG_FPGA_PLATFORM
    mt_mem_init();
    BOOTING_TIME_PROFILING_LOG("mem_init");
#endif
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

#if !CFG_BYPASS_EMI
	ram_console_init();
	ram_console_reboot_reason_save(g_rgu_status);
	BOOTING_TIME_PROFILING_LOG("Ram console");
#endif

#if !CFG_DRAM_CALIB_OPTIMIZATION
	/*
	 * Init booting storage device.
	 *
	 * If CFG_DRAM_CALIB_OPTIMIZATION is disabled or not defined, booting storage device
	 * needs to be initialized after DRAM initialization.
	 */
    ret = boot_device_init();
    BOOTING_TIME_PROFILING_LOG("Boot dev init");
    pal_log_err("%s Init Boot Device: %s(%d)\n", MOD, ret ? "FAIL" : "OK", ret);
#endif

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


/* make sure usb11 in power saving mode*/
	mt_usb11_phy_savecurrent();


#if CFG_REBOOT_TEST
    mtk_wdt_sw_reset();
    while(1);
#endif

#if (!CFG_FPGA_PLATFORM) && (!CFG_BYPASS_EMI)
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
    bootarg.dram_rank_num = 0x00000001;
    bootarg.dram_rank_size[0] = 0x40000000;
    bootarg.dram_rank_size[1] = 0x0;
    bootarg.dram_rank_size[2] = 0x0;
    bootarg.dram_rank_size[3] = 0x0;
    bootarg.mblock_info.mblock_num = 0x00000001;
    bootarg.mblock_info.mblock[0].start = 0x40000000;
    bootarg.mblock_info.mblock[0].size = 0x40000000;
    bootarg.mblock_info.mblock[0].rank = 0;
    bootarg.mblock_info.mblock[1].start = 0x0;
    bootarg.mblock_info.mblock[1].size = 0x0;
    bootarg.mblock_info.mblock[1].rank = 0;
    bootarg.mblock_info.mblock[2].start = 0;
    bootarg.mblock_info.mblock[2].size = 0;
    bootarg.mblock_info.mblock[2].rank = 0;
    bootarg.mblock_info.mblock[3].start = 0;
    bootarg.mblock_info.mblock[3].size = 0;
    bootarg.mblock_info.mblock[3].rank = 0;
    bootarg.orig_dram_info.rank_num = 1;
    bootarg.orig_dram_info.rank_info[0].start = 0x40000000;
    bootarg.orig_dram_info.rank_info[0].size = 0x40000000;
    bootarg.orig_dram_info.rank_info[1].start = 0x0;
    bootarg.orig_dram_info.rank_info[1].size = 0x0;
    bootarg.orig_dram_info.rank_info[2].start = 0;
    bootarg.orig_dram_info.rank_info[2].size = 0;
    bootarg.orig_dram_info.rank_info[3].start = 0;
    bootarg.orig_dram_info.rank_info[3].size = 0;
    bootarg.lca_reserved_mem.start = 0;
    bootarg.lca_reserved_mem.size = 0;
#endif
    mtk_subpmic_init();
    BOOTING_TIME_PROFILING_LOG("mtk subpmic init");
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
	lastpc_init();
      BOOTING_TIME_PROFILING_LOG("lastpc init");
#ifdef MTK_SECURITY_SW_SUPPORT
	/*Anti rollback update*/
	#ifdef MTK_SECURITY_ANTI_ROLLBACK
	g_pl_otp_status = sec_otp_ver_update();
	#endif
#endif

#if CFG_FUNCTION_PICACHU_SUPPORT
    start_picachu(PL_PICACHU_BASE);
    BOOTING_TIME_PROFILING_LOG("picachu");
#endif
#if CFG_BATTERY_DETECT
    /* normal boot to check battery exists or not */
    if (g_boot_mode == NORMAL_BOOT && !hw_check_battery() && usb_accessory_in()) {
        pal_log_err("%s Wait for battery inserted...\n", MOD);
        mtk_try_unlock_bat(MTK_UNLOCK_BAT_CV_LEVEL);
        mdelay(1000);
        /* disable pmic pre-charging led */
        pl_close_pre_chr_led();
        /* enable force charging mode */
        pl_charging(1);
        do {
            mdelay(300);
            /* check battery exists or not */
            if (hw_check_battery())
                break;
            /* kick all watchdogs */
            platform_wdt_all_kick();
        } while(1);
        /* disable force charging mode */
        pl_charging(0);
    }
#endif

#if !CFG_FPGA_PLATFORM
    pl_battery_init(true);
#endif
    BOOTING_TIME_PROFILING_LOG("Battery detect");

    platform_bootarg_reserve();
    ram_console_mblock_reserve();
    platform_set_boot_args();
    BOOTING_TIME_PROFILING_LOG("Boot Argu");
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
    print("Detect fastmeta pin %d: mode %d, out %d, pull en %d, pull up %d, value %d.\n",
        CFG_FAST_META_GPIO,
        mt_get_gpio_mode(0x80000000 | CFG_FAST_META_GPIO),
        mt_get_gpio_dir(0x80000000 | CFG_FAST_META_GPIO),
        mt_get_gpio_pull_enable(0x80000000 | CFG_FAST_META_GPIO),
        mt_get_gpio_pull_select(0x80000000 | CFG_FAST_META_GPIO),
        val = mt_get_gpio_in(0x80000000 | CFG_FAST_META_GPIO));
    if(!val) { // detect GPIO pull LOW
        print("Enable fastmeta.\n");
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
        part_proinfo_addr = (u64)(part->start_sect) * bootdev->blksz;
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
#endif //CFG_FAST_META_SUPPORT
