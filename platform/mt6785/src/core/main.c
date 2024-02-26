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
#include "download.h"
#include "meta.h"
#include "sec.h"
#include "partition_api.h"
#include "dram_buffer.h"
#include "wdt.h"
#include "timer.h"
#include "log_store_pl.h"
#include <memory_layout.h> /* for PL_ATF_BASE used in ATF_ROM_LOAD_ADDR */
#include "pmic.h"
#include "rpmb_key.h"
//#include "mt_ptp2.h"
#if CFG_ATF_SUPPORT
#include "tz_init.h"
#include "tz_mem.h"
#endif
#if (MTK_TINYSYS_SSPM_SUPPORT || CFG_LOAD_SLT_SSPM)
#include "sspm.h"
#include <emi_mpu_mt.h>
#endif
#include <pal_log.h>
#include <partition_api.h>
#include <partition_active.h>

#if CFG_GZ_SUPPORT
#include <gz_init.h>
#endif

#include <dconfig_env.h>

#if defined(MTK_AB_OTA_UPDATER)
#include "bootctrl.h"
#include "ab_bootcheck.h"
#endif
#include <pal_log.h>
#include <pal_typedefs.h>

#include <plat_debug.h>

/*============================================================================*/
/* CONSTAND DEFINITIONS                                                       */
/*============================================================================*/
#define MOD "[BLDR]"
#define ATF_DRAM_IMG_NAME "atf_dram"

/*============================================================================*/
/* MACROS DEFINITIONS                                                         */
/*============================================================================*/
#define CMD_MATCH(cmd1,cmd2)  \
    (!strncmp((const char*)(cmd1->data), (cmd2), min(strlen(cmd2), cmd1->len)))

/*============================================================================*/
/* GLOBAL VARIABLES                                                           */
/*============================================================================*/
#if CFG_BOOT_ARGUMENT
#define bootarg g_dram_buf->bootarg
#endif

/*============================================================================*/
/* STATIC VARIABLES                                                           */
/*============================================================================*/
bl_param_t *p_bldr_param = NULL;
static bl_param_t bldr_param;

/*============================================================================*/
/* EXTERN                                                                     */
/*============================================================================*/
#if CFG_FAST_META_SUPPORT
extern unsigned char g_is_fast_meta;
#endif

/*============================================================================*/
/* INTERNAL FUNCTIONS                                                         */
/*============================================================================*/
#if CFG_BOOT_TIME_PROFILE
U32 profiling_time = 0;	//declare in main.c
#endif

static void bldr_pre_process(void)
{
    int isLocked = 0;
    uint32_t ret = 0;
    #if CFG_FAST_META_SUPPORT
    u8 meta_boot = 0, meta_log = 0;
    #endif

    #if CFG_USB_AUTO_DETECT
    platform_usbdl_flag_check();
    #endif

    #if CFG_EMERGENCY_DL_SUPPORT
    platform_safe_mode(1, CFG_EMERGENCY_DL_TIMEOUT_MS);
    #endif

    /* essential hardware initialization. e.g. timer, pll, uart... */
    platform_pre_init();

    pal_log_info("\n%s Build Time: %s\n", MOD, BUILD_TIME);

#if (CFG_AEE_SUPPORT == 1)
    mcu_dfd_debug_en();
#endif

    /* init security library */
    sec_lib_init();
    BOOTING_TIME_PROFILING_LOG("Sec lib init");

#if MTK_DOE_CONFIG_ENV_SUPPORT
    dconfig_initenv();
    BOOTING_TIME_PROFILING_LOG("dconfig init");
    #endif

#if !CFG_FPGA_PLATFORM
    drcc_init();
    BOOTING_TIME_PROFILING_LOG("drcc_init");
#endif
//    config_DCC_Calin();
//    dump_dcc_regs();
//    disable_FBB_SW();

    g_boot_mode = NORMAL_BOOT;

#ifdef MTK_SECURITY_SW_SUPPORT
    /* note that if you use cmm file, these parameters are empty. */
	ret |= seclib_set_pl_load_addr(p_bldr_param->bl_loadinfo[0].bl_load_addr);
	ret |= seclib_set_cc_status(p_bldr_param->cc_lcs, p_bldr_param->cc_flags);
	ret |= seclib_set_sctrl_info(p_bldr_param->sctrl_cert_file_addr, p_bldr_param->sctrl_cert_file_len);
	ret |= seclib_set_tool_auth_info(p_bldr_param->tool_auth_file_addr, p_bldr_param->tool_auth_file_len);
	ret |= seclib_set_me_id(p_bldr_param->meid, ME_IDENTITY_LEN);
	ret |= seclib_set_soc_id(p_bldr_param->socid, SOC_ID_LEN);
	ret |= seclib_set_prov_key(p_bldr_param->prov_key, PROVISIONING_KEY_LEN);
	if (ret) {
		pal_log_err("Fail to init chip info.\n");
		ASSERT(0);
	}
#endif

    /* hardware initialization */
    platform_init();

    part_init();
    BOOTING_TIME_PROFILING_LOG("part_init");

#if CFG_GZ_SUPPORT
    gz_pre_init();
    BOOTING_TIME_PROFILING_LOG("gz_pre_init");
#endif

#if CFG_FAST_META_SUPPORT
    read_meta_proinfo(&meta_boot, &meta_log);
    if(check_meta_short_pin() || (meta_boot == 0x31)) {
        g_is_fast_meta = 1;
        g_boot_mode = META_BOOT;
        // let kernel always enumerate ELT port
        g_meta_com_id = CFG_FAST_META_FLAG | 0x00000004;
        g_meta_com_type = META_USB_COM;
        // default disable modem/uart log, and can be enable by proinfo flag or meta api
        if(meta_log == 0x31) {
            g_meta_log_disable = 0;

            #if CFG_UART_DYNAMIC_SWITCH
            set_log_switch(1);
            #endif
        }
        else
            g_meta_log_disable = 1;

        pal_log_err("%s fast_meta = %d, meta_log_disable = %d\n", MOD, g_is_fast_meta, g_meta_log_disable);
    }
    BOOTING_TIME_PROFILING_LOG("read_meta_proinfo");
#endif

    store_switch_to_dram();
    BOOTING_TIME_PROFILING_LOG("store_switch_to_dram");
    part_dump();
    BOOTING_TIME_PROFILING_LOG("part_dump");

    rpmb_program_key();
    BOOTING_TIME_PROFILING_LOG("rpmb");

    /* note: lock state is only valid after sec_lib_init, be careful when moving the following functions */
#ifdef MTK_FACTORY_LOCK_SUPPORT
    seclib_query_factory_lock(&isLocked);
#endif

#if CFG_UART_TOOL_HANDSHAKE
            /* init uart handshake for sending 'ready' to tool and receiving handshake
             * pattern from tool in the background and we'll see the pattern later.
             * this can reduce the handshake time.
             */
    boot_mode_t mode = NORMAL_BOOT;
#ifdef MTK_SECURITY_SW_SUPPORT
    mode = seclib_brom_meta_mode();
#endif
    if (!isLocked && mode == NORMAL_BOOT) {
        pal_log_info("%s Starting tool handshake.\n", MOD);
        uart_handshake_init();
        BOOTING_TIME_PROFILING_LOG("UART handshake init");
        log_buf_ctrl(1); /* switch log buffer to dram */
    }
#endif
}

static void bldr_post_process(void)
{
    platform_post_init();

#if CFG_GZ_SUPPORT
    gz_post_init();
    BOOTING_TIME_PROFILING_LOG("gz_post_init");
#endif
}

u32 seclib_get_devinfo_with_index(u32 index)
{
	return internal_seclib_get_devinfo_with_index(index);
}

static bool wait_for_discon(struct comport_ops *comm, u32 tmo_ms)
{
    bool ret;
    u8 discon[HSHK_DISCON_SZ];
    memset(discon, 0x0, HSHK_DISCON_SZ);

    pal_log_info("[BLDR] DISCON...");
    if (ret = comm->recv(discon, HSHK_DISCON_SZ, tmo_ms)) {
    pal_log_info("timeout\n");
    return ret;
    }

    if (0 == memcmp(discon, HSHK_DISCON, HSHK_DISCON_SZ))
    pal_log_info("OK\n");
    else
    pal_log_info("protocol mispatch\n");

    return ret;
}

int bldr_load_part(char *name, blkdev_t *bdev, u32 *addr, u32 *size)
{
    part_t *part = part_get(name);

    if (NULL == part) {
        pal_log_err("%s %s partition not found\n", MOD, name);
        return -1;
    }

    return part_load(bdev, part, addr, 0, size);
}

int bldr_load_part_lk(blkdev_t *bdev, u32 *addr, u32 *size)
{
    unsigned long lk_active = 0, lk2_active = 0;

    // check the active bit of lk partition
    part_t *part_lk = part_get("lk");
    if (NULL == part_lk) {
        pal_log_err("%s lk partition not found\n", MOD);
        return -1;
    } else {
        lk_active = mt_part_get_active_bit(part_lk);
    }

    // check the active bit of lk2 partition
    part_lk = part_get("lk2");
    if (NULL == part_lk) {
        pal_log_err("%s lk2 partition not found\n", MOD);
        //return -1; Comment it since we could load lk partition to boot
    } else {
        lk2_active = mt_part_get_active_bit(part_lk);
    }

    pal_log_info("%s lk active = %x, lk2 active = %x\n", MOD, lk_active, lk2_active);

    // load partition
    if (lk_active == 0 && lk2_active > 0) {
        pal_log_info("%s Loading LK2 Partition...\n", MOD);
        part_lk = part_get("lk2");
        if (NULL == part_lk) {
            pal_log_err("%s lk2 partition not found\n", MOD);
            return -1;
        }
        else {
            return part_load(bdev, part_lk, addr, 0, size);
        }
    } else {
        pal_log_info("%s Loading LK Partition...\n", MOD);
        part_lk = part_get("lk");
        if (NULL == part_lk) {
            pal_log_err("%s lk partition not found\n", MOD);
            return -1;
        }
        else {
            return part_load(bdev, part_lk, addr, 0, size);
        }
    }
}

int bldr_load_tee_part(char *name, blkdev_t *bdev, u32 *addr, u32 offset, u32 *size)
{
    int ret;
    part_t *part = part_get(name);
	u32 next_offset;
	u32 atf_sram_size;
	u32 atf_dram_addr = 0;

    if (NULL == part) {
        pal_log_err("%s %s part. not found\n", MOD, name);
        return -1;
    }

    ret = part_load(bdev, part, addr, offset, size);
    if (ret) {
        pal_log_err("%s %s part. ATF load fail\n", MOD, name);
        return ret;
    }
	pal_log_info("%s %s part. ATF load addr:0x%x, size:0x%x\n", MOD, name, *addr, *size);

	next_offset = sizeof(part_hdr_t) + *size;
	if (if_equal_img_name(bdev, part, next_offset, ATF_DRAM_IMG_NAME))
	{
		atf_sram_size = *size;
		ret = part_load(bdev, part, &atf_dram_addr, next_offset, size);
		if (ret) {
			pal_log_err("%s %s part. ATF load fail\n", MOD, name);
			return ret;
		}
		pal_log_info("%s %s part. ATF dram load addr:0x%x, size:0x%x\n", MOD, name, atf_dram_addr, *size);
		*size = *size + sizeof(part_hdr_t) + atf_sram_size;
	}

    /* header addr will be updated to entry point addr */
#if CFG_TEE_SUPPORT
    {
    u32 tee_addr = 0;
    u32 next_offset = sizeof(part_hdr_t) + *size;

    ret = part_load(bdev, part, &tee_addr, next_offset, size);
    if (ret) {
        pal_log_err("%s %s part. TEE load fail\n", MOD, name);
        return ret;
    }

    /* header addr will be updated to entry point addr */
        ret = tee_verify_image(&tee_addr);

    if (ret)
        pal_log_err("%s %s part. TEE verify fail\n", MOD, name);

    /* set tee entry address */
    tee_set_entry(tee_addr);

    /* set hwuid. note that if you use cmm file, the parameter is empty. */
    tee_set_hwuid((u8*)&p_bldr_param->meid[0], sizeof(p_bldr_param->meid));
    }
#endif

    return ret;
}

#if (MTK_TINYSYS_SSPM_SUPPORT || CFG_LOAD_SLT_SSPM)
static void *sspm_memcpy(void *dest, const void *src, int count)
{
    unsigned int *tmp = dest;
    const unsigned int *s = src;

    while (count > 0) {
        *tmp++ = *s++;
        count -= 4;
    }

    return dest;
}
#endif

#if MTK_TINYSYS_SSPM_SUPPORT
extern u32 g_ddr_reserve_enable;
extern u32 g_ddr_reserve_success;

static int bldr_load_sspm_part(blkdev_t *bdev, u32 *addr, u32 *size)
{
    int ret;
    u32 tmp_addr;
    part_t *part;
    ptimg_hdr_t *hdr;
    unsigned char *img, *pmimg, *dmimg, *cpmimg, *ximg;
    unsigned int pmsize, dmsize, cpmsize, xsize;
    char *parts[] = { "sspm_1", "sspm_2" };
    struct sspm_info_t *info = (struct sspm_info_t *) *addr;

#if SSPM_DBG_MODE
#define SSPM_CFGREG_PC              (SSPM_SW_RSTN + 0x64)
#define SSPM_CFGREG_AHB_STATUS      (SSPM_SW_RSTN + 0x6C)
#define SSPM_CFGREG_AHB_M0_ADDR     (SSPM_SW_RSTN + 0x70)
#define SSPM_CFGREG_AHB_M1_ADDR     (SSPM_SW_RSTN + 0x74)
    sspm_dbg("SSPM_CFGREG_PC: 0x%x\n", DRV_Reg32(SSPM_CFGREG_PC));
    sspm_dbg("SSPM_CFGREG_AHB_STATUS: 0x%x\n", DRV_Reg32(SSPM_CFGREG_AHB_STATUS));
    sspm_dbg("SSPM_CFGREG_AHB_M0_ADDR: 0x%x\n", DRV_Reg32(SSPM_CFGREG_AHB_M0_ADDR));
    sspm_dbg("SSPM_CFGREG_AHB_M1_ADDR: 0x%x\n", DRV_Reg32(SSPM_CFGREG_AHB_M1_ADDR));
#endif

#if defined(MTK_AB_OTA_UPDATER)
    const char *ab_suffix = get_suffix();
    if (ab_suffix == NULL)
        return -1;

    tmp_addr = *addr + ROUNDUP(sizeof(*info), 4);
    if (!memcmp(ab_suffix, BOOTCTRL_SUFFIX_A, 2)) {
        ret = bldr_load_part("sspm_a", bdev, &tmp_addr, &size);
    } else {
        ret = bldr_load_part("sspm_b", bdev, &tmp_addr, &size);
    }
#else
    for (ret = 0;ret < (sizeof(parts) / sizeof(*parts));ret++) {
        part = part_get(parts[ret]);

        if (part && mt_part_get_active_bit(part) > 0)
            break;
    }

    if (ret == (sizeof(parts) / sizeof(*parts)))
        ret = 0;

    /* decide partition by active bit */
    part = part_get(parts[ret]);

    if (NULL == part) {
        pal_log_err("%s SSPM part. not found\n", MOD);
        return -1;
    }

    tmp_addr = *addr + ROUNDUP(sizeof(*info), 4);

    /* load and verify image */
    ret = part_load(bdev, part, &tmp_addr, 0, size);
#endif //MTK_AB_OTA_UPDATER

    if (ret) {
        pal_log_err("%s SSPM part. load fail\n", MOD);
        return ret;
    }
    sspm_dbg("Load SSPM partition to dram 0x%x (%d bytes)\n", tmp_addr, *size);

    /* separate ptimg */
    pmimg = dmimg = cpmimg = ximg = NULL;
    pmsize = dmsize = cpmsize = xsize = 0;
    hdr = (ptimg_hdr_t *) tmp_addr;

    while (hdr->magic == PT_MAGIC) {
        img = ((char *) hdr) + hdr->hdr_size;

        switch (hdr->id) {
            case PT_ID_SSPM_PM:
                pmimg = img;
                pmsize = hdr->img_size;
                break;
            case PT_ID_SSPM_DM:
                dmimg = img;
                dmsize = hdr->img_size;
                break;
            case PT_ID_SSPM_CPM:
                cpmimg = img;
                cpmsize = hdr->img_size;
                break;
            case PT_ID_SSPM_XFILE:
                ximg = img;
                xsize = hdr->img_size;
                break;
            default:
                pal_log_warn("%s Warning: Ignore unknow SSPM image_%d\n", MOD, hdr->id);
                break;
        }

        img += ROUNDUP(hdr->img_size, hdr->align);
        hdr = (ptimg_hdr_t *) img;
    }

    sspm_dbg("pmimg: 0x%x(%d bytes), dmimg: 0x%x(%d bytes), ximg: 0x%x(%d bytes)\n",
        (u32)pmimg, pmsize, (u32)dmimg, dmsize, (u32)ximg, xsize);

    if (!pmimg || !dmimg || !ximg) {
        pal_log_err("%s SSPM partition missing - PM:0x%x, DM:0x%x (@0x%x)\n", MOD, (u32)pmimg, (u32)dmimg, tmp_addr);
        return -1;
    }


    if (cpmsize > SSPM_CPM_SZ) {
        pal_log_err("%s SSPM .cpm (%d bytes) is oversize\n", MOD, cpmsize);
        return -1;
    }

    /* load PM */
    sspm_memcpy((unsigned char *) CFG_SSPM_MEMADDR, pmimg, pmsize);

    /* Setup sspm_info  */
    memset(info, 0, ROUNDUP(sizeof(*info), 4));

    info->sspm_dm_ofs = (unsigned int)dmimg - (unsigned int)info;
    info->sspm_dm_sz = dmsize;

    info->rd_ofs = ROUNDUP(SSPM_IMG_SZ, SSPM_MEM_ALIGN);
    info->rd_sz  = SSPM_TCM_SZ - pmsize;

    info->xfile_addr = (unsigned int)ximg - sizeof(ptimg_hdr_t);
    info->xfile_sz = sizeof(ptimg_hdr_t) + xsize + sizeof(unsigned int); // with postfix 'END'

    sspm_dbg("sspm_info @0x%x: dm offset 0x%x (%d bytes), rd offset 0x%x(%d bytes), xfile addr 0x%x(%d bytes)\n",
        (u32)info, info->sspm_dm_ofs, info->sspm_dm_sz, info->rd_ofs, info->rd_sz, info->xfile_addr, info->xfile_sz);

    if ( (pmsize + dmsize + xsize + 3 * sizeof(ptimg_hdr_t)) > SSPM_IMG_SZ) {
        pal_log_warn("%s Warning: SSPM IMG is over than %dK, the coredump will overwrite it!\n", MOD, SSPM_IMG_SZ/1024);
    }

    /* re-allocate CPM */
    if (cpmimg> 0) {
        tmp_addr = ROUNDUP((*addr + SSPM_IMG_SZ + SSPM_TCM_SZ), SSPM_DDR_ALIGN) + (SSPM_TCM_SZ - SSPM_DDR_SIZE);
        memcpy((void *)tmp_addr, (void *)cpmimg, cpmsize);
        DRV_WriteReg32(SSPM_DDR_RMAP0, tmp_addr >> 18);

        sspm_dbg("Re-allocate CPM to 0x%x; DDR_RMAP is 0x%x\n", tmp_addr, DRV_Reg32(SSPM_DDR_RMAP0));
    }
    mtk_wdt_request_mode_set(MTK_WDT_STATUS_SSPM_RST, WD_REQ_RST_MODE);
    mtk_wdt_request_en_set(MTK_WDT_STATUS_SSPM_RST, WD_REQ_EN);

#if SSPM_DBG_MODE
    /* GPIO pinmux - SSPM UART:4, and JTAG:5 */
    DRV_SetReg32(0x10005440, 0x00455050);
    DRV_SetReg32(0x10005450, 0x00005540);
#endif

    DRV_WriteReg32(SSPM_CFGREG_GPR0, (unsigned int) *addr);
    DRV_WriteReg32(SSPM_CFGREG_GPR1, ram_console_is_abnormal_boot()); // If not normal boot, notify sspm to backup

    DRV_SetReg32(SSPM_SW_RSTN, 0x1);

    pal_log_info("%s SSPM Start %s ddr reserved mode\n", MOD,
         (g_ddr_reserve_enable==1 && g_ddr_reserve_success==1) ? "with" : "without" );

    return 0;
}
#endif

#if CFG_LOAD_SLT_SSPM
static int bldr_load_sspm_part_slt(blkdev_t *bdev, u32 *addr, u32 *size)
{
	int ret;
	ptimg_hdr_t *hdr;
	unsigned char *img, *pmimg, *dmimg;
	unsigned int pmsize, dmsize;
	char *parts = "SSPM";

	/* load and verify image */
    ret = bldr_load_part(parts, bdev, addr, size);
	if (ret) {
		pal_log_err("%s SSPM part. load fail\n", MOD);
		return ret;
	}

	/* separate ptimg */
	pmimg = dmimg = NULL;
	hdr = (ptimg_hdr_t *) *addr;

	while (hdr->magic == PT_MAGIC) {
		img = ((char *) hdr) + hdr->hdr_size;

		switch (hdr->id) {
			case PT_ID_SSPM_PM:
				pmimg = img;
				pmsize = hdr->img_size;
				break;
			case PT_ID_SSPM_DM:
				dmimg = img;
				dmsize = hdr->img_size;
				break;
		}

		img += ROUNDUP(hdr->img_size, hdr->align);
		hdr = (ptimg_hdr_t *) img;
	}

	if (!pmimg || !dmimg) {
		pal_log_err("%s SSPM part missing: 0x%x, 0x%x\n", MOD, (u32) pmimg, (u32) dmimg);
		return -1;
	}

	// print("pmimg: 0x%x (0x%x), dmimg: 0x%x (0x%x)\n", (u32) pmimg, (u32) pmsize, (u32) dmimg, (u32) dmsize);

	sspm_memcpy((unsigned char *) CFG_SSPM_MEMADDR, pmimg, pmsize);
	sspm_memcpy((unsigned char *) CFG_SSPM_MEMADDR + pmsize, dmimg, dmsize);

	pal_log_info("%s SSPM finished\n");
	return 0;
}
#endif

static bool bldr_cmd_handler(struct bldr_command_handler *handler,
    struct bldr_command *cmd, struct bldr_comport *comport)
{
    struct comport_ops *comm = comport->ops;
    u32 attr = handler->attr;

#if CFG_DT_MD_DOWNLOAD
    if (CMD_MATCH(cmd, SWITCH_MD_REQ)) {
        /* SWITCHMD */
        if (attr & CMD_HNDL_ATTR_COM_FORBIDDEN)
            goto forbidden;

        comm->send((u8*)SWITCH_MD_ACK, strlen(SWITCH_MD_ACK));
        platform_modem_download();
        return TRUE;
    }
#endif

    if (CMD_MATCH(cmd, ATCMD_PREFIX)) {
        /* "AT+XXX" */

        if (CMD_MATCH(cmd, ATCMD_NBOOT_REQ)) {
            /* return "AT+OK" to tool */
            comm->send((u8*)ATCMD_OK, strlen(ATCMD_OK));

            g_boot_mode = NORMAL_BOOT;
            g_boot_reason = BR_TOOL_BY_PASS_PWK;

        } else {
            /* return "AT+UNKONWN" to ack tool */
            comm->send((u8*)ATCMD_UNKNOWN, strlen(ATCMD_UNKNOWN));

            return FALSE;
        }
    } else if (CMD_MATCH(cmd, META_STR_REQ)) {
        para_t param;
        para_t_v2 param_v2;
        memset(&param, 0, sizeof(param));  /*init param*/
        memset(&param_v2, 0, sizeof(param_v2));

#if CFG_BOOT_ARGUMENT
        bootarg.md_type[0] = 0;
        bootarg.md_type[1] = 0;
#endif

        /* "METAMETA" */
        if (attr & CMD_HNDL_ATTR_COM_FORBIDDEN)
            goto forbidden;

    /* for backward compatibility */
    comm->recv((u8*)&param.v0001, sizeof(param.v0001), 2000);

    /* meta usb type use for ELT Port */
    pal_log_info("meta_com_id = %d\n", param.v0001.usb_type);
    g_meta_com_id = param.v0001.usb_type;

#if CFG_WORLD_PHONE_SUPPORT
        comm->send((u8*)META_ARG_VER_STR, strlen(META_ARG_VER_STR));

        if (0 == comm->recv((u8*)&param.v0001, sizeof(param.v0001), 5000)) {
            g_meta_com_id = param.v0001.usb_type;
        pal_log_info("meta_com_id = %d\n", param.v0001.usb_type);
        pal_log_info("md_type[0] = %d \n", param.v0001.md0_type);
        pal_log_info("md_type[1] = %d \n", param.v0001.md1_type);
#if CFG_BOOT_ARGUMENT
        bootarg.md_type[0] = param.v0001.md0_type;
        bootarg.md_type[1] = param.v0001.md1_type;
#endif
        }
#endif

        /* V2 param extension */
        comm->send((u8*)META_ARG_VER_STR_V2, strlen(META_ARG_VER_STR_V2));
        if (0 == comm->recv((u8*)&param_v2.v0002, sizeof(param_v2.v0002), 5000)) {
            pal_log_info("meta_com_id = %d\n", param_v2.v0002.usb_type);
            pal_log_info("meta_log_disable = %d\n", param_v2.v0002.meta_log_disable);
            g_meta_com_id = param_v2.v0002.usb_type;
            g_meta_log_disable = param_v2.v0002.meta_log_disable; /* disable uart log to enhance boot time */
        }

        comm->send((u8*)META_STR_ACK, strlen(META_STR_ACK));

#if CFG_WORLD_PHONE_SUPPORT
        wait_for_discon(comm, 1000);
#endif
        g_boot_mode = META_BOOT;
    } else if (CMD_MATCH(cmd, FACTORY_STR_REQ)) {
        para_t param;

        /* "FACTFACT" */
        if (attr & CMD_HNDL_ATTR_COM_FORBIDDEN)
            goto forbidden;

        if (0 == comm->recv((u8*)&param.v0001, sizeof(param.v0001), 5)) {
            g_meta_com_id = param.v0001.usb_type;
        }

        comm->send((u8*)FACTORY_STR_ACK, strlen(FACTORY_STR_ACK));

        g_boot_mode = FACTORY_BOOT;
    } else if (CMD_MATCH(cmd, META_ADV_REQ)) {
        /* "ADVEMETA" */
        if (attr & CMD_HNDL_ATTR_COM_FORBIDDEN)
            goto forbidden;

        comm->send((u8*)META_ADV_ACK, strlen(META_ADV_ACK));

        wait_for_discon(comm, 1000);

        g_boot_mode = ADVMETA_BOOT;
    } else if (CMD_MATCH(cmd, ATE_STR_REQ)) {
        para_t param;

        /* "FACTORYM" */
        if (attr & CMD_HNDL_ATTR_COM_FORBIDDEN)
            goto forbidden;

        if (0 == comm->recv((u8*)&param.v0001, sizeof(param.v0001), 5)) {
            g_meta_com_id = param.v0001.usb_type;
        }

        comm->send((u8*)ATE_STR_ACK, strlen(ATE_STR_ACK));

        g_boot_mode = ATE_FACTORY_BOOT;
    } else if (CMD_MATCH(cmd, FB_STR_REQ)) {

    /* "FASTBOOT" */
    comm->send((u8 *)FB_STR_ACK, strlen(FB_STR_ACK));

    g_boot_mode = FASTBOOT;
    } else {
        pal_log_warn("%s unknown received: \'%s\'\n", MOD, cmd->data);

        return FALSE;
    }

    pal_log_info("%s '%s' received!\n", MOD, cmd->data);

    return TRUE;

forbidden:
    comm->send((u8*)META_FORBIDDEN_ACK, strlen(META_FORBIDDEN_ACK));
    pal_log_warn("%s '%s' is forbidden!\n", MOD, cmd->data);
    return FALSE;
}

static int bldr_handshake(struct bldr_command_handler *handler)
{
    boot_mode_t mode = NORMAL_BOOT;
    bool isSLA = 0;
    int isLocked = 0;

#ifdef MTK_SECURITY_SW_SUPPORT
    /* get mode type */
    mode = seclib_brom_meta_mode();
    isSLA = seclib_sla_enabled();
	BOOTING_TIME_PROFILING_LOG("seclib_brom_meta_mode");
#endif

#ifdef MTK_FACTORY_LOCK_SUPPORT
    seclib_query_factory_lock(&isLocked);
#endif

    switch (mode) {
    case NORMAL_BOOT:
        /* ------------------------- */
        /* security check            */
        /* ------------------------- */
        if (TRUE == isSLA) {
            handler->attr |= CMD_HNDL_ATTR_COM_FORBIDDEN;
            pal_log_info("%s META DIS\n", MOD);
        }

        if (!isLocked) {
            pal_log_info("%s Tool connection is unlocked\n", MOD);
            #if CFG_USB_TOOL_HANDSHAKE
            #if CFG_FAST_META_SUPPORT
            if(g_is_fast_meta)
                pal_log_info("%s In fast meta, bypass usb handshake.\n", MOD);
            else
            #endif
            {
            if (TRUE == usb_handshake(handler))
                g_meta_com_type = META_USB_COM;
		    BOOTING_TIME_PROFILING_LOG("USB handshake");
            }
            #endif
            #if CFG_UART_TOOL_HANDSHAKE
            if (TRUE == uart_handshake(handler))
                g_meta_com_type = META_UART_COM;
		    BOOTING_TIME_PROFILING_LOG("UART handshake");
            #endif
        }
        else {
            pal_log_info("%s Tool connection is locked\n", MOD);
            bootarg.sec_limit.magic_num = SEC_LIMIT_MAGIC;
            bootarg.sec_limit.forbid_mode = F_FACTORY_MODE;
        }

        break;

    case META_BOOT:
        pal_log_info("%s BR META BOOT\n", MOD);

// init md_type for security chip + world phone project in BROM Meta case.
#if CFG_BOOT_ARGUMENT
        bootarg.md_type[0] = 0;
        bootarg.md_type[1] = 0;
#endif
        g_boot_mode = META_BOOT;

        if(!usb_cable_in())
            g_meta_com_type = META_UART_COM;
        else
            g_meta_com_type = META_USB_COM;
        break;


    case FACTORY_BOOT:
        pal_log_info("%s BR FACTORY BOOT\n", MOD);
        g_boot_mode = FACTORY_BOOT;

        if(!usb_cable_in())
            g_meta_com_type = META_UART_COM;
        else
            g_meta_com_type = META_USB_COM;
        break;


    case ADVMETA_BOOT:
        pal_log_info("%s BR ADVMETA BOOT\n", MOD);
        g_boot_mode = ADVMETA_BOOT;

        if(!usb_cable_in())
            g_meta_com_type = META_UART_COM;
        else
            g_meta_com_type = META_USB_COM;
        break;


    case ATE_FACTORY_BOOT:
        pal_log_info("%s BR ATE FACTORY BOOT\n", MOD);
        g_boot_mode = ATE_FACTORY_BOOT;

        if(!usb_cable_in())
            g_meta_com_type = META_UART_COM;
        else
            g_meta_com_type = META_USB_COM;
        break;


    default:
        pal_log_info("%s UNKNOWN MODE\n", MOD);
        break;
    }

    return 0;
}

static void bldr_wait_forever(void)
{
    /* prevent wdt timeout and clear usbdl flag */
    mtk_wdt_disable();
    platform_safe_mode(0, 0);
    pal_log_info("bldr_wait_forever\n");
    while(1);
}

static int bldr_load_images(u32 *jump_addr)
{
    int ret = 0;
    blkdev_t *bootdev;
    u32 addr = 0;
    char *name;
    u32 size = 0;
    u32 spare0 = 0;
    u32 spare1 = 0;
    u32 type = 0;
    char active_part_name[PART_NAME_BUF_SZ] = {0};

    if (NULL == (bootdev = blkdev_get(CFG_BOOT_DEV))) {
        pal_log_err("%s can't find boot device(%d)\n", MOD, CFG_BOOT_DEV);
        /* FIXME, should change to global error code */
        return -1;
    }

#if CFG_LOAD_MD_ROM
    /* do not check the correctness */
    addr = CFG_MD1_ROM_MEMADDR;
    //bldr_load_part(PART_MD1_ROM, bootdev, &addr, &size);
    bldr_load_part("MD1_ROM", bootdev, &addr, &size);

    addr = CFG_MD2_ROM_MEMADDR;
    //bldr_load_part(PART_MD2_ROM, bootdev, &addr, &size);
    bldr_load_part("MD2_ROM", bootdev, &addr, &size);
#endif
#if CFG_LOAD_MD_RAMDISK
    /* do not check the correctness */
    addr = CFG_MD1_RAMDISK_MEMADDR;
    bldr_load_part("MD1_RAMDISK", bootdev, &addr, &size);

    addr = CFG_MD2_RAMDISK_MEMADDR;
    bldr_load_part("MD2_RAMDISK", bootdev, &addr, &size);
#endif

#if CFG_LOAD_MD_DSP
    addr = CFG_MD_DSP_MEMADDR;
    bldr_load_part("MD_DSP",bootdev, &addr, &size);
#endif

#if CFG_LOAD_SLT_MD_RAMDISK
    /* do not check the correctness */
    addr = CFG_MD1_RAMDISK_MEMADDR;
    //bldr_load_part(PART_FDD_MD_RAMDISK, bootdev, &addr, &size);
    bldr_load_part("FDD_MD_RAMDISK", bootdev, &addr, &size);

    addr = CFG_MD1_RAMDISK_MEMADDR;
    //bldr_load_part(PART_TDD_ONLY_MD_RAMDISK, bootdev, &addr, &size);
    bldr_load_part("TDD_ONLY_MD_RAMDISK", bootdev, &addr, &size);

    addr = CFG_MD1_RAMDISK_MEMADDR;
    //bldr_load_part(PART_TDD2G_MD_RAMDISK, bootdev, &addr, &size);
    bldr_load_part("TDD2G_MD_RAMDISK", bootdev, &addr, &size);
#endif

#if CFG_LOAD_SLT_MD_DSP
    addr = CFG_MD_DSP_MEMADDR;
    //bldr_load_part(PART_FDD_MD_DSP,bootdev, &addr, &size);
    bldr_load_part("FDD_MD_DSP",bootdev, &addr, &size);

    addr = CFG_MD_DSP_MEMADDR;
    //bldr_load_part(PART_TDD_ONLY_MD_DSP,bootdev, &addr, &size);
    bldr_load_part("TDD_ONLY_MD_DSP",bootdev, &addr, &size);

    addr = CFG_MD_DSP_MEMADDR;
    //bldr_load_part(PART_C2K_ONLY_MD_DSP,bootdev, &addr, &size);
    bldr_load_part("C2K_ONLY_MD_DSP",bootdev, &addr, &size);

    addr = CFG_MD_DSP_MEMADDR;
    //bldr_load_part(PART_TDD2G_MD_DSP,bootdev, &addr, &size);
    bldr_load_part("TDD2G_MD_DSP",bootdev, &addr, &size);
#endif

#if CFG_LOAD_CONN_SYS
    addr = CFG_CONN_SYS_MEMADDR;
    //bldr_load_part(PART_CONN_SYS,bootdev, &addr, &size);
    bldr_load_part("CONN_SYS",bootdev, &addr, &size);
#endif

#if CFG_LOAD_SLT_MD
    int *pass_size = NULL;
    addr = CFG_FDD_MD_ROM_MEMADDR;
    //bldr_load_part(PART_HVT_MD_ROM, bootdev, &addr, &size);
    bldr_load_part("HVT_MD_ROM", bootdev, &addr, &size);

    addr = CFG_FDD_MD_ROM_MEMADDR;
    //bldr_load_part(PART_LHIF_MD_ROM, bootdev, &addr, &size);
    bldr_load_part("LHIF_MD_ROM", bootdev, &addr, &size);

    addr = CFG_FDD_MD_ROM_MEMADDR;
    //bldr_load_part(PART_FDD_MD_ROM, bootdev, &addr, &size);
    bldr_load_part("FDD_MD_ROM", bootdev, &addr, &size);
    pass_size = addr - 4;
    *pass_size = size;
    pal_log_info("assign 0x%X value %d\n", pass_size, size);

    addr = CFG_FDD_MD_ROM_MEMADDR;
    //bldr_load_part(PART_TDD_ONLY_ROM, bootdev, &addr, &size);
    bldr_load_part("TDD_ONLY_ROM", bootdev, &addr, &size);
    pass_size = addr - 4;
    *pass_size = size;
    pal_log_info("assign 0x%X value %d\n", pass_size, size);
    addr = CFG_FDD_MD_ROM_MEMADDR;
    //bldr_load_part(PART_C2K_ONLY_ROM, bootdev, &addr, &size);
    bldr_load_part("C2K_ONLY_ROM", bootdev, &addr, &size);
    pass_size = addr - 4;
    *pass_size = size;
    pal_log_info("assign 0x%X value %d\n", pass_size, size);

    addr = CFG_FDD_MD_ROM_MEMADDR;
    //bldr_load_part(PART_TDD2G_MD_ROM, bootdev, &addr, &size);
    bldr_load_part("TDD2G_MD_ROM", bootdev, &addr, &size);
#endif

#if CFG_LOAD_SLT_SCP
        DRV_WriteReg32(0x100A402C, 0);
        addr = 0;

    bldr_load_part("SCP",bootdev, &addr, &size);
#endif


#if CFG_LOAD_SLT_MD32
#define ReadREGMD32(_addr, _value) ((_value) = *(volatile unsigned int *)(_addr) )
#define WriteREGMD32(_addr, _value) (*(volatile unsigned int *)(_addr) = (_value))
    u32 p_addr, d_addr;
    u32 src_addr, dest_addr;
    u32 count, value;

    //SPM power on MD32 and MD32 SRAM
    DRV_WriteReg32(0x10B00000, 0x0b160001);
    DRV_WriteReg32(0x10B00330, 0x0000000d);
    DRV_WriteReg32(0x10B00390, 0x00000032);
    //DRV_WriteReg32(0x100062c8, 0xfffffff0);

    p_addr = CFG_MD32P_ROM_MEMADDR;
    //bldr_load_part(PART_MD32_P,bootdev, &p_addr, &size);
    bldr_load_part("MD32_P",bootdev, &p_addr, &size);
    src_addr = p_addr;
    dest_addr = 0x10b30000;
    count = size;

    while (count > 0) {
        ReadREGMD32(src_addr,value);
        WriteREGMD32(dest_addr,value);
        count -= 4;
        src_addr += 4;
        dest_addr += 4;
    }
     pal_log_info("MD32 loadPM, srcaddr check1:%x, value:%x\n", p_addr, value);
    src_addr = p_addr;
    dest_addr = 0x10b30000;
    count = size;
    while (count > 0) {
        ReadREGMD32(src_addr,value);
        WriteREGMD32(dest_addr,value);
        count -= 4;
        src_addr += 4;
        dest_addr += 4;
    }
    pal_log_info("MD32 loadPM, srcaddr check2:%x, value:%x\n", p_addr, value);
    //memcpy((void *)0x10b30000, (void *)p_addr, sizeof(char)*size);

    d_addr = CFG_MD32D_ROM_MEMADDR;
    //bldr_load_part(PART_MD32_D,bootdev, &d_addr, &size);
    bldr_load_part("MD32_D",bootdev, &d_addr, &size);
    src_addr = d_addr;
    dest_addr = 0x10b38000;
    count = size;
    while (count > 0) {
        ReadREGMD32(src_addr,value);
        WriteREGMD32(dest_addr,value);
        count -= 4;
        src_addr += 4;
        dest_addr += 4;
    }
    pal_log_info("MD32 loadDM, srcaddr:%x, value:%x\n", d_addr, value);
    //memcpy((void *)0x10b38000, (void *)d_addr, sizeof(char)*size);

    DRV_WriteReg32(0x10B00330, 0x00000000);
#endif

#if MTK_TINYSYS_SSPM_SUPPORT && !CFG_LOAD_SLT_SSPM
    addr = (u32)mblock_reserve_ext(&bootarg.mblock_info,
            (u64)SSPM_MEM_SIZE, (u64)SSPM_MEM_ALIGN,
            SSPM_MEM_LIMIT, 0, "SSPM-reserved");

    if (!addr)
        return -1;

    ret = bldr_load_sspm_part(bootdev, &addr, &size);

    if (ret)
        return ret;

#if ENABLE_MPU && (!SSPM_DBG_MODE)
    if (ram_console_is_abnormal_boot()) {
        /* If ABNORMAL BOOT, SSPM_MPU_REGION_ID should be accessible for LKDump */
        type = SEC_RW_NSEC_R;
    } else {
        type = FORBIDDEN;
    }

    struct emi_region_info_t region_info;
    region_info.start = (u64)addr;
    region_info.end = (u64)addr + ROUNDUP(SSPM_MEM_SIZE, SSPM_MEM_ALIGN) - 1;
    region_info.region = SSPM_MPU_REGION_ID;
    SET_ACCESS_PERMISSION(region_info.apc, UNLOCK,
        FORBIDDEN, FORBIDDEN, FORBIDDEN, FORBIDDEN,
        FORBIDDEN, FORBIDDEN, FORBIDDEN, SEC_RW,
        FORBIDDEN, FORBIDDEN, FORBIDDEN, FORBIDDEN,
        FORBIDDEN, FORBIDDEN, FORBIDDEN, type);

    emi_mpu_set_protection(&region_info);
#endif

#elif MTK_TINYSYS_SSPM_SUPPORT && CFG_LOAD_SLT_SSPM

#define SSPM_MEM_SIZE   0x00100000  // 1M (Should be large than SSPM_IMG_SZ + SSPM_TCM_SZ + SSPM_CPM_SZ )
#define SSPM_MEM_ALIGN  0x00010000  // 64K (minimal size for EMI MPU)
#define SSPM_MEM_LIMIT  0x80000000

	addr = (u32)mblock_reserve_ext(&bootarg.mblock_info,
			(u64)SSPM_MEM_SIZE, (u64)SSPM_MEM_ALIGN,
			SSPM_MEM_LIMIT, 0, "SSPM-reserved");

	if (!addr)
		return -1;

	ret = bldr_load_sspm_part_slt(bootdev, &addr, &size);

	if (ret)
		return ret;
#endif // MTK_TINYSYS_SSPM_SUPPORT && !CFG_LOAD_SLT_SSPM

#if CFG_LOAD_AP_ROM
    addr = CFG_AP_ROM_MEMADDR;
    //ret = bldr_load_part(PART_AP_ROM, bootdev, &addr, &size);
    ret = bldr_load_part("AP_ROM", bootdev, &addr, &size);
    if (ret)
    return ret;
    *jump_addr = addr;
#elif CFG_LOAD_UBOOT
#if defined(MTK_AB_OTA_UPDATER)
    addr = CFG_UBOOT_MEMADDR;
    const char *ab_suffix = get_suffix();
    if (ab_suffix == NULL)
        return -1;
    if (!memcmp(ab_suffix, BOOTCTRL_SUFFIX_A, 2)) {
        ret = bldr_load_part("lk_a", bootdev, &addr, &size);
    } else {
        ret = bldr_load_part("lk_b", bootdev, &addr, &size);
    }

    if (ret)
        return ret;
    *jump_addr = addr;
#else
    addr = CFG_UBOOT_MEMADDR;
#if !CFG_BYPASS_EMI
    ret = bldr_load_part_lk(bootdev, &addr, &size);

    pal_log_info("LK addr: 0x%x, size: 0x%x\n", addr, size);
    if (addr < CFG_UBOOT_MEMADDR || (addr + size) > (CFG_UBOOT_MEMADDR + 0x900000))
        pal_log_warn("Warning: LK out of boundary.\n");

    if (ret)
       return ret;
#endif
    *jump_addr = addr;
#endif //MTK_AB_OTA_UPDATER
#endif

#if CFG_ATF_SUPPORT
#if defined(MTK_AB_OTA_UPDATER)
    addr = ATF_ROM_LOAD_ADDR;

    if (!memcmp(ab_suffix, BOOTCTRL_SUFFIX_A, 2)) {
        ret = bldr_load_tee_part("tee_a", bootdev, &addr, 0, &size);
    } else {
        ret = bldr_load_tee_part("tee_b", bootdev, &addr, 0, &size);
    }

    pal_log_info("%s bldr load tee part ret=0x%x, addr=0x%x\n", MOD, ret, addr);
#else
    addr = ATF_ROM_LOAD_ADDR;

    if (strlen("tee") >= PART_NAME_BUF_SZ)
        return -1;

    memset(active_part_name, 0x0, PART_NAME_BUF_SZ);
    memcpy(active_part_name, "tee", strlen("tee"));

    ret = partition_get_gpt_active(active_part_name, PART_NAME_BUF_SZ, GET_ACTIVE);
    if (ret)
        return ret;

    ret = bldr_load_tee_part(active_part_name, bootdev, &addr, 0, &size);
    if (ret)
        return ret;

    pal_log_info("%s bldr load tee part ret=0x%x, addr=0x%x\n", MOD, ret, addr);
#endif //MTK_AB_OTA_UPDATER
#endif

#if CFG_GZ_SUPPORT
    if (!gz_de_init()) {
        if (strlen("gz") >= PART_NAME_BUF_SZ)
            return -1;

        memset(active_part_name, 0x0, PART_NAME_BUF_SZ);
        memcpy(active_part_name, "gz", strlen("gz"));

        ret = partition_get_active(active_part_name, PART_NAME_BUF_SZ, GET_ACTIVE);
        if (ret)
            return ret;

        ret = bldr_load_gz_part(bootdev, active_part_name);
        if (ret)
            return ret;
    }
#endif

    return ret;
}
void mt_usb_phy_recover(void);
void mu3d_hal_rst_dev(void);
/*============================================================================*/
/* GLOBAL FUNCTIONS                                                           */
/*============================================================================*/
void bldr_jump(u32 addr, u32 arg1, u32 arg2)
{
    platform_wdt_kick();

    /* disable preloader safe mode */
    platform_safe_mode(0, 0);

    pal_log_err("\n%s jump to 0x%x\n", MOD, addr);
    pal_log_err("%s <0x%x>=0x%x\n", MOD, addr, *(u32*)addr);
    pal_log_err("%s <0x%x>=0x%x\n", MOD, addr + 4, *(u32*)(addr + 4));

    jump(addr, arg1, arg2);
}

void bldr_jump64(u32 addr, u32 arg1, u32 arg2)
{
    u64 addr64 = addr;

    platform_wdt_kick();

    /* disable preloader safe mode */
    platform_safe_mode(0, 0);

    pal_log_err("\n%s jump to 0x%x\n", MOD, addr);
#if !CFG_BYPASS_EMI
    pal_log_err("%s <0x%x>=0x%x\n", MOD, addr, *(u32*)addr);
    pal_log_err("%s <0x%x>=0x%x\n", MOD, addr + 4, *(u32*)(addr + 4));
#endif

#if CFG_GZ_SUPPORT
    if (is_booting_el2()) {
        addr64 = gz_get_jump_addr();
        pal_log_info("%s next is GZ at 0x%llx\n", MOD, addr64);
    }
#endif

#if CFG_ATF_SUPPORT
    trustzone_jump(addr64, arg1, arg2);
#else
    pal_log_err("%s trustzone is not supported!\n", MOD);
#endif
}

void main(u32 *arg)
{
    struct bldr_command_handler handler;
    u32 jump_addr, jump_arg;

    /* get the bldr argument */
    p_bldr_param = &bldr_param;
    memcpy((void *)p_bldr_param, (void *)*arg, sizeof(bl_param_t));

    mtk_uart_init(UART_SRC_CLK_FRQ, CFG_LOG_BAUDRATE);
    bldr_pre_process();

#if !CFG_FPGA_PLATFORM
    wk_vcore_check();
#endif
    #ifdef HW_INIT_ONLY
    bldr_wait_forever();
    #endif

    handler.priv = NULL;
    handler.attr = 0;
    handler.cb   = bldr_cmd_handler;

    BOOTING_TIME_PROFILING_LOG("before bldr_handshake");
    bldr_handshake(&handler);
    BOOTING_TIME_PROFILING_LOG("bldr_handshake");

#if BOOTROM_INTEGRITY_CHECK
	/* if rom integrity check fails, device halts, so don't put it before bootloader
	   handshake, this could make device bricked */
	rom_integrity_check();
#endif

#if !CFG_FPGA_PLATFORM
    /* security check */
    device_APC_dom_setup();
#endif
    BOOTING_TIME_PROFILING_LOG("sec_boot_check");

#if CFG_ATF_SUPPORT
    trustzone_pre_init();
    BOOTING_TIME_PROFILING_LOG("trustzone pre init");
#endif

#if (defined(MTK_AB_OTA_UPDATER) && !defined(EARLY_PARTITION_ACCESS))
    ab_ota_boot_check();
#endif

#if !(CFG_BYPASS_LOAD_IMG_FORCE_ATF)
    /* Do not load ATF, lk, load by JTAG */
    if (0 != bldr_load_images(&jump_addr)) {
        pal_log_err("%s Second Bootloader Load Failed\n", MOD);
#if !CFG_BYPASS_EMI
        goto error;
#endif
    }
#else
    jump_addr = CFG_UBOOT_MEMADDR;
#endif
    BOOTING_TIME_PROFILING_LOG("load images");

    bldr_post_process();
#ifdef SLT
    mt_usb_phy_recover();
    //mu3d_hal_rst_dev();
#endif

#if CFG_ATF_SUPPORT
    trustzone_post_init();
    BOOTING_TIME_PROFILING_LOG("trustzone post init");
#endif

#if CFG_BOOT_ARGUMENT_BY_ATAG
    jump_arg = (u32)&(g_dram_buf->boottag);
#else
    jump_arg = (u32)&bootarg;
#endif

    /* 64S3,32S1,32S1 (MTK_ATF_BOOT_OPTION = 0)
	 * re-loader jump to LK directly and then LK jump to kernel directly */
#if CFG_ATF_SUPPORT
    pal_log_info("%s Others, jump to ATF\n", MOD);
    bldr_jump64(jump_addr, jump_arg, sizeof(boot_arg_t));
#else
    bldr_jump(jump_addr, jump_arg, sizeof(boot_arg_t));
#endif

error:
    platform_error_handler();
}

