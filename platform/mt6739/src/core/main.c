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
//#include "mt_ptp2.h"
#if CFG_ATF_SUPPORT
#include "tz_init.h"
#endif
#include "tz_emi_mpu.h"
#include "sec_efuse.h"
#include <sec_boot.h>

#if defined(MTK_AB_OTA_UPDATER)
#include "bootctrl.h"
#include "ab_bootcheck.h"
#endif
#include <partition_api.h>
#include <partition_active.h>
#include <pal_log.h>
#include <pal_typedefs.h>
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

/*============================================================================*/
/* INTERNAL FUNCTIONS                                                         */
/*============================================================================*/
#if defined(PL_PROFILING)
U32 profiling_time = 0;	//declare in main.c
#endif

#if MTK_TINYSYS_SSPM_SUPPORT
#define SSPM_DBG_MODE 0

#if SSPM_DBG_MODE
#define sspm_dbg(f, ...)       print("%s" f, MOD, ##__VA_ARGS__)
#else
#define sspm_dbg
#endif

#define SSPM_MPU_REGION_ID  3
#define SSPM_MEM_SIZE   0x00080000  // 512K
#define SSPM_MEM_ALIGN  0x00010000  // 64K (minimal size for EMI MPU)
#define SSPM_MEM_LIMIT  0xC0000000
#define SSPM_TCM_SZ     0x00028000  // 160K
#define SSPM_RD_SZ      0x00010000  // 64K

#define ROUNDUP(a,b)    (((a) + ((b)-1)) & ~((b)-1))

struct sspm_info_t {
    unsigned int sspm_dm_ofs;
    unsigned int sspm_dm_sz;
    unsigned int rd_ofs;
    unsigned int rd_sz;
};
#endif

unsigned int bldr_load_loader_ext_etc(void);

static void bldr_pre_process(void)
{
    int isLocked = 0;
    u32 ret = 0;
#ifdef MTK_EFUSE_WRITER_RESERVE_CODESIZE
	struct efuse_param param = {0};
#endif

    #if CFG_USB_AUTO_DETECT
    platform_usbdl_flag_check();
    #endif

    #if CFG_EMERGENCY_DL_SUPPORT
    platform_safe_mode(1, CFG_EMERGENCY_DL_TIMEOUT_MS);
    #endif

    /* essential hardware initialization. e.g. timer, pll, uart... */
    platform_pre_init();

    print("\n%s Build Time: %s\n", MOD, BUILD_TIME);
//    config_DCC_Calin();
//    dump_dcc_regs();
//    disable_FBB_SW();

    g_boot_mode = NORMAL_BOOT;

    /* hardware initialization */
    platform_init();

    part_init();
    BOOTING_TIME_PROFILING_LOG("Part Init");
    store_switch_to_dram();
    BOOTING_TIME_PROFILING_LOG("store_switch_to_dram");
    part_dump();
    BOOTING_TIME_PROFILING_LOG("part_dump");

    sec_update_dram_init_status();

#if CFG_PRELOADER_EXTENSION
    ret = bldr_load_loader_ext_etc();
    if (ret) {
       print("load loader_ext_etc fail (%x)\n", ret);
       ASSERT(0);
    }
    BOOTING_TIME_PROFILING_LOG("loader_ext_etc");
#endif

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
    print("%s Efuse status(%x)\n", MOD, ret);
    BOOTING_TIME_PROFILING_LOG("EFUSE Self Blow");
#else
    print("%s MTK_EFUSE_WRITER_RESERVE_CODESIZE off\n", MOD);
#endif

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
        uart_handshake_init();
		BOOTING_TIME_PROFILING_LOG("UART handshake init");
        log_buf_ctrl(1); /* switch log buffer to dram */
	}
#endif
}

static void bldr_post_process(void)
{
    platform_post_init();
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

    print("[BLDR] DISCON...");
    if (ret = comm->recv(discon, HSHK_DISCON_SZ, tmo_ms)) {
    print("timeout\n");
    return ret;
    }

    if (0 == memcmp(discon, HSHK_DISCON, HSHK_DISCON_SZ))
    print("OK\n");
    else
    print("protocol mispatch\n");

    return ret;
}

int bldr_load_part(char *name, blkdev_t *bdev, u32 *addr, u32 *size)
{
    part_t *part = part_get(name);

    if (NULL == part) {
        print("%s %s partition not found\n", MOD, name);
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
        print("%s lk partition not found\n", MOD);
        return -1;
    } else {
        lk_active = mt_part_get_active_bit(part_lk);
    }

    // check the active bit of lk2 partition
    part_lk = part_get("lk2");
    if (NULL == part_lk) {
        print("%s lk2 partition not found\n", MOD);
        //return -1; Comment it since we could load lk partition to boot
    } else {
        lk2_active = mt_part_get_active_bit(part_lk);
    }

    print("%s lk active = %x, lk2 active = %x\n", MOD, lk_active, lk2_active);

    // load partition
    if (lk_active == 0 && lk2_active > 0) {
        print("%s Loading LK2 Partition...\n", MOD);
        part_lk = part_get("lk2");
        if (part_lk != NULL) {
            return part_load(bdev, part_lk, addr, 0, size);
        } else {
            return -1;
        }
    } else {
        print("%s Loading LK Partition...\n", MOD);
        part_lk = part_get("lk");
        if (part_lk != NULL) {
            return part_load(bdev, part_lk, addr, 0, size);
        } else {
            return -1;
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
        print("%s %s part. not found\n", MOD, name);
        return -1;
    }

    ret = part_load(bdev, part, addr, offset, size);
    if (ret) {
        print("%s %s part. ATF load fail\n", MOD, name);
        return ret;
    }
	print("%s %s part. ATF load addr:0x%x, size:0x%x\n", MOD, name, *addr, *size);

	next_offset = sizeof(part_hdr_t) + *size;
	if (if_equal_img_name(bdev, part, next_offset, ATF_DRAM_IMG_NAME))
	{
		atf_sram_size = *size;
		ret = part_load(bdev, part, &atf_dram_addr, next_offset, size);
		if (ret) {
			print("%s %s part. ATF load fail\n", MOD, name);
			return ret;
		}
		print("%s %s part. ATF dram load addr:0x%x, size:0x%x\n", MOD, name, atf_dram_addr, *size);
		*size = *size + sizeof(part_hdr_t) + atf_sram_size;
	}

    /* header addr will be updated to entry point addr */
#if CFG_TEE_SUPPORT
    {
    u32 tee_addr = 0;
    u32 next_offset = sizeof(part_hdr_t) + *size;

    ret = part_load(bdev, part, &tee_addr, next_offset, size);
    if (ret) {
        print("%s %s part. TEE load fail\n", MOD, name);
        return ret;
    }

    /* header addr will be updated to entry point addr */
        ret = tee_verify_image(&tee_addr);

    if (ret)
        print("%s %s part. TEE verify fail\n", MOD, name);

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
    unsigned char *img, *pmimg, *dmimg;
    unsigned int pmsize, dmsize;
    char *parts[] = { "sspm_1", "sspm_2" };
    struct sspm_info_t *info = (struct sspm_info_t *) *addr;

#define PT_ID_SSPM_DM   0
#define PT_ID_SSPM_PM   1

#define SSPM_SW_RSTN        0x10A40000
#define SSPM_CFGREG_GPR0    SSPM_SW_RSTN + 0x20
#define SSPM_CFGREG_GPR1    SSPM_SW_RSTN + 0x24
#define SSPM_CFGREG_GPR2    SSPM_SW_RSTN + 0x28
#define SSPM_CFGREG_GPR3    SSPM_SW_RSTN + 0x2C
#define SSPM_CFGREG_GPR4    SSPM_SW_RSTN + 0x30
#define SSPM_CFGREG_GPR5    SSPM_SW_RSTN + 0x34

#define ROUNDUP(a,b)    (((a) + ((b)-1)) & ~((b)-1))

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
        print("%s SSPM part. not found\n", MOD);
        return -1;
    }

    tmp_addr = *addr + ROUNDUP(sizeof(*info), 4);

    /* load and verify image */
    ret = part_load(bdev, part, &tmp_addr, 0, size);
    if (ret) {
        print("%s SSPM part. load fail\n", MOD);
        return ret;
    }
    sspm_dbg("Load SSPM partition to dram 0x%x (size: 0x%x)\n", tmp_addr, size);

    /* separate ptimg */
    pmimg = dmimg = NULL;
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
        }

        img += ROUNDUP(hdr->img_size, hdr->align);
        hdr = (ptimg_hdr_t *) img;
    }

    if (!pmimg || !dmimg) {
        print("%s SSPM partition missing - PM:0x%x, DM:0x%x (@0x%x)\n", MOD, (u32)pmimg, (u32)dmimg, tmp_addr);
        return -1;
    }

    sspm_dbg("pmimg: 0x%x(size 0x%x), dmimg: 0x%x(size 0x%x)\n", (u32)pmimg, pmsize, (u32)dmimg, dmsize);

    DRV_WriteReg32(SSPM_SW_RSTN, 0x90000000);

    sspm_memcpy((unsigned char *) CFG_SSPMP_MEMADDR, pmimg, pmsize);

    memset(info, 0, ROUNDUP(sizeof(*info), 4));

    info->sspm_dm_ofs = (unsigned int)dmimg - (unsigned int)info;
    info->sspm_dm_sz = dmsize;

    info->rd_ofs = ROUNDUP(SSPM_TCM_SZ, SSPM_MEM_ALIGN);
    info->rd_sz = SSPM_RD_SZ;

    sspm_dbg("sspm_info @0x%x: dm offset 0x%x (0x%x), rd offset 0x%x(0x%x))\n", (u32)info,
        info->sspm_dm_ofs, info->sspm_dm_sz, info->rd_ofs, info->rd_sz);

    mtk_wdt_request_mode_set(MTK_WDT_STATUS_SSPM_RST, WD_REQ_RST_MODE);
    mtk_wdt_request_en_set(MTK_WDT_STATUS_SSPM_RST, WD_REQ_EN);

#if SSPM_DBG_MODE
    //Setup GPIO pinmux SSPM JTAG
    DRV_SetReg32(0x102d0440, 0x00666660);
    //Setup GPIO pinmux SSPM UART
    DRV_SetReg32(0x102d0370, 0x10000000);
    DRV_SetReg32(0x102d0380, 0x00000001);
#endif

    DRV_WriteReg32(SSPM_CFGREG_GPR0, (unsigned int) *addr);
    DRV_WriteReg32(SSPM_CFGREG_GPR1, ram_console_is_abnormal_boot()); // If not normal boot, notify sspm to backup
    DRV_SetReg32(SSPM_SW_RSTN, 0x1);

    print("%s SSPM Start! (with g_rgu_status 0x%x & %s ddr reserved mode)\n", MOD, g_rgu_status,
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

#define SSPM_IMG_TMP_OFFSET 0x28000 // 160K
#define PT_ID_SSPM_DM   0
#define PT_ID_SSPM_PM   1

#define SSPM_SW_RSTN        0x10A40000

	/* load and verify image */
    ret = bldr_load_part(parts, bdev, addr, size);
	if (ret) {
		print("%s SSPM part. load fail\n", MOD);
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
		print("%s SSPM part missing: 0x%x, 0x%x\n", MOD, (u32) pmimg, (u32) dmimg);
		return -1;
	}

	//    print("pmimg: 0x%x (0x%x), dmimg: 0x%x (0x%x)\n", (u32) pmimg, (u32) pmsize, (u32) dmimg, (u32) dmsize);

	DRV_WriteReg32(SSPM_SW_RSTN, 0x90000000);

	sspm_memcpy((unsigned char *) CFG_SSPMP_MEMADDR, pmimg, pmsize);
	sspm_memcpy((unsigned char *) CFG_SSPMD_MEMADDR, dmimg, dmsize);

	print("%s SSPM finished\n");
	return 0;
}
#endif

#if CFG_PRELOADER_EXTENSION
int bldr_load_loader_ext_dram(void)
{
    blkdev_t *bdev = NULL;
    int ret = 0;
    part_hdr_t hdr;
    u32 sec_policy_idx = 0;
    u32 img_auth_required = 0;
    u32 ms = 0;
    u8 img_name[32] = {0};
    u8 part_name[32] = {0};
    u32 boot_part = 0;
    struct part_info_t loader_ext_info;

    bdev = blkdev_get(CFG_BOOT_DEV);
    if (NULL == bdev) {
        print("%s can't find boot device(%d)\n", MOD, CFG_BOOT_DEV);
        return -1;
    }

    /* Since preloader and loader_ext.img need to be the same build, we need
     * to ensure the followings after OTA
     * 1. boot part 1 loaded with loader_ext1/loader_ext_a
     * 2. boot part 2 loaded with loader_ext2/loader_ext_b
     */
    ret = mmc_get_boot_part(&boot_part);
    if (0 != ret) {
        print("get boot part fail: %d\n", ret);
    } else {
        print("boot_part: %d\n", boot_part);
    }

#if defined(MTK_AB_OTA_UPDATER)
    if (boot_part == EMMC_PART_BOOT1) {
        memcpy(part_name, "loader_ext_a", 12);
    } else {
        memcpy(part_name, "loader_ext_b", 12);
    }
    part_name[11 + 1] = '\0'; //[0 ~ 11] is loader_ext_X
#else
    if (boot_part == EMMC_PART_BOOT1) {
        memcpy(part_name, "loader_ext1", 11);
    } else {
        memcpy(part_name, "loader_ext2", 11);
    }
    part_name[10 + 1] = '\0'; //[0 ~ 10] is loader_extX
#endif //MTK_AB_OTA_UPDATER
    ret = mt_get_part_info_by_name(part_name, &loader_ext_info);
    if (ret) {
        print("get %s_info error\n", part_name);
    }

#ifdef MTK_SECURITY_SW_SUPPORT
    /* get security policy of current partition */
    print("part name=%s\n", part_name);
    sec_policy_idx = get_policy_entry_idx(part_name);
    img_auth_required = get_vfy_policy(sec_policy_idx);
    print("img_auth_required=%x\n", img_auth_required);
    ms = get_timer(0);
    if (img_auth_required) {
        sec_malloc_buf_reset();
        memcpy(img_name, "loader_ext_dram", 16);
        if (sec_img_auth_init(part_name, img_name)) {
            print("cert vfy fail\n");
            ASSERT(0);
        }
#ifdef MTK_SECURITY_ANTI_ROLLBACK
        if (sec_rollback_check(0)) {
            print("ver check fail\n", MOD);
            ASSERT(0);
        }
#endif
    }
    ms = get_timer(ms);
    print("img: %s cert vfy(%d ms)\n", img_name, ms);
#endif

    print("load %s_dram\n", part_name);
    if (blkdev_read(bdev, loader_ext_info.addr, sizeof(part_hdr_t), (u8 *)&hdr, EMMC_PART_USER) != 0) {
        print("read header error\n");
        ASSERT(0);
    } else {
        if (hdr.info.magic != PART_MAGIC) {
            print("img not exist\n");
            ASSERT(0);
        }
    }

    if (blkdev_read(bdev, loader_ext_info.addr + sizeof(part_hdr_t), hdr.info.dsize, (u8 *)CFG_LOADER_EXT_DRAM_ADDR, EMMC_PART_USER) != 0) {
        print("load image error\n");
        ASSERT(0);
    }

#ifdef MTK_SECURITY_SW_SUPPORT
    ms = get_timer(0);
    if (img_auth_required) {
        print("img vfy...");
        ret = sec_img_auth(CFG_LOADER_EXT_DRAM_ADDR, hdr.info.dsize);
        if (ret) {
            print("fail(0x%x)\n", ret);
            ASSERT(0);
        } else {
            print("ok\n");
        }
    }
    ms = get_timer(ms);
    print("img: %s vfy(%d ms)\n", "loader_ext_dram", ms);
#endif

    if (ret) {
        print("load loader_ext_dram fail\n");
    }

    return ret;
}

unsigned int bldr_load_loader_ext_etc()
{
    blkdev_t *bdev;
    part_t *part_loader_ext;
    u32 addr = CFG_LOADER_EXT_ETC_ADDR;
    u32 size = 0;
    u8 part_name[32] = {0};
    u8 img_name[32] = {0};
    u32 img_size = 0;
    u32 ret = 0;
    u32 boot_part = 0;

    /* Since preloader and loader_ext.img need to be the same build, we need
     * to ensure the followings after OTA
     * 1. boot part 1 loaded with loader_ext1/loader_ext_a
     * 2. boot part 2 loaded with loader_ext2/loader_ext_b
     */
    ret = mmc_get_boot_part(&boot_part);
    if (0 != ret) {
        print("get boot part fail: %d\n", ret);
    } else {
        print("boot_part: %d\n", boot_part);
    }

#if defined(MTK_AB_OTA_UPDATER)
    if (boot_part == EMMC_PART_BOOT1) {
        memcpy(part_name, "loader_ext_a", 12);
    } else {
        memcpy(part_name, "loader_ext_b", 12);
    }
    part_name[11 + 1] = '\0'; //[0 ~ 11] is loader_ext_X
#else
    if (boot_part == EMMC_PART_BOOT1) {
        memcpy(part_name, "loader_ext1", 11);
    } else {
        memcpy(part_name, "loader_ext2", 11);
    }
    part_name[10 + 1] = '\0'; //[0 ~ 10] is loader_extX
#endif //MTK_AB_OTA_UPDATER


    /* loader_ext.img is as follow:
     * -------------------
     * | part_hdr_t      |
     * |-----------------|
     * | loader_ext_dram |
     * |-----------------|
     * | part_hdr_t      |
     * |-----------------|
     * | loader_ext_etc  |
     * -------------------
     *
     * To load loader_ext_etc, we need to bypass the size of part_hdr_t and
     * loader_ext_dram.
     */
    memcpy(img_name, "loader_ext_dram", 16);
    bdev = blkdev_get(CFG_BOOT_DEV);
    if (NULL == bdev) {
        print("%s can't find boot device(%d)\n", MOD, CFG_BOOT_DEV);
        return -1;
    }

    print("load %s_etc\n", part_name);
    part_loader_ext = part_get(part_name);
    if (NULL == part_loader_ext) {
        print("%s not found\n", part_name);
        return -1;
    } else {
        ret = sec_get_img_size(part_name, img_name, &img_size);
        if (ret) {
            print("get %s size fail: %x", part_name, ret);
        }
        /* img_size = image size + signature */
        return part_load(bdev, part_loader_ext, &addr, sizeof(part_hdr_t) + img_size, &size);
    }
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
	memset(&param, 0, sizeof(param));  /*init param*/

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
    print("meta_com_id = %d\n", param.v0001.usb_type);
    g_meta_com_id = param.v0001.usb_type;

#if CFG_WORLD_PHONE_SUPPORT
        comm->send((u8*)META_ARG_VER_STR, strlen(META_ARG_VER_STR));

        if (0 == comm->recv((u8*)&param.v0001, sizeof(param.v0001), 5000)) {
            g_meta_com_id = param.v0001.usb_type;
        print("md_type[0] = %d \n", param.v0001.md0_type);
        print("md_type[1] = %d \n", param.v0001.md1_type);
#if CFG_BOOT_ARGUMENT
        bootarg.md_type[0] = param.v0001.md0_type;
        bootarg.md_type[1] = param.v0001.md1_type;
#endif
        }
#endif
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
        print("%s unknown received: \'%s\'\n", MOD, cmd->data);

        return FALSE;
    }

    print("%s '%s' received!\n", MOD, cmd->data);

    return TRUE;

forbidden:
    comm->send((u8*)META_FORBIDDEN_ACK, strlen(META_FORBIDDEN_ACK));
    print("%s '%s' is forbidden!\n", MOD, cmd->data);
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
            print("%s META DIS\n", MOD);
        }

        if (!isLocked) {
            print("%s Tool connection is unlocked\n", MOD);
            #if CFG_USB_TOOL_HANDSHAKE
            if (TRUE == usb_handshake(handler))
                g_meta_com_type = META_USB_COM;
		    BOOTING_TIME_PROFILING_LOG("USB handshake");
            #endif
            #if CFG_UART_TOOL_HANDSHAKE
            if (TRUE == uart_handshake(handler))
                g_meta_com_type = META_UART_COM;
		    BOOTING_TIME_PROFILING_LOG("UART handshake");
            #endif
        }
        else {
            print("%s Tool connection is locked\n", MOD);
            bootarg.sec_limit.magic_num = SEC_LIMIT_MAGIC;
            bootarg.sec_limit.forbid_mode = F_FACTORY_MODE;
        }

        break;

    case META_BOOT:
        print("%s BR META BOOT\n", MOD);

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
        print("%s BR FACTORY BOOT\n", MOD);
        g_boot_mode = FACTORY_BOOT;

        if(!usb_cable_in())
            g_meta_com_type = META_UART_COM;
        else
            g_meta_com_type = META_USB_COM;
        break;


    case ADVMETA_BOOT:
        print("%s BR ADVMETA BOOT\n", MOD);
        g_boot_mode = ADVMETA_BOOT;

        if(!usb_cable_in())
            g_meta_com_type = META_UART_COM;
        else
            g_meta_com_type = META_USB_COM;
        break;


    case ATE_FACTORY_BOOT:
        print("%s BR ATE FACTORY BOOT\n", MOD);
        g_boot_mode = ATE_FACTORY_BOOT;

        if(!usb_cable_in())
            g_meta_com_type = META_UART_COM;
        else
            g_meta_com_type = META_USB_COM;
        break;


    default:
        print("%s UNKNOWN MODE\n", MOD);
        break;
    }

    return 0;
}

static void bldr_wait_forever(void)
{
    /* prevent wdt timeout and clear usbdl flag */
    mtk_wdt_disable();
    platform_safe_mode(0, 0);
    print("bldr_wait_forever\n");
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
    char active_part_name[PART_NAME_BUF_SZ] = {0};


    if (NULL == (bootdev = blkdev_get(CFG_BOOT_DEV))) {
        print("%s can't find boot device(%d)\n", MOD, CFG_BOOT_DEV);
        /* FIXME, should change to global error code */
        return -1;
    }
#if CFG_LOAD_MD_ROM
if (1 == aarch64_slt_done())
{
    /* do not check the correctness */
    addr = CFG_MD1_ROM_MEMADDR;
    //bldr_load_part(PART_MD1_ROM, bootdev, &addr, &size);
    bldr_load_part("MD1_ROM", bootdev, &addr, &size);

    addr = CFG_MD2_ROM_MEMADDR;
    //bldr_load_part(PART_MD2_ROM, bootdev, &addr, &size);
    bldr_load_part("MD2_ROM", bootdev, &addr, &size);
}
#endif
#if CFG_LOAD_MD_RAMDISK
if (1 == aarch64_slt_done())
{
    /* do not check the correctness */
    addr = CFG_MD1_RAMDISK_MEMADDR;
    bldr_load_part("MD1_RAMDISK", bootdev, &addr, &size);

    addr = CFG_MD2_RAMDISK_MEMADDR;
    bldr_load_part("MD2_RAMDISK", bootdev, &addr, &size);
}
#endif

#if CFG_LOAD_MD_DSP
if (1 == aarch64_slt_done())
{
    addr = CFG_MD_DSP_MEMADDR;
    bldr_load_part("MD_DSP",bootdev, &addr, &size);
}
#endif

#if CFG_LOAD_SLT_MD_RAMDISK
if (1 == aarch64_slt_done())
{
    /* do not check the correctness */
    addr = CFG_MD1_RAMDISK_MEMADDR;
    bldr_load_part("FDD_MD_RAMDISK", bootdev, &addr, &size);

    //addr = CFG_TDD_ONLY_MD_RAMDISK_MEMADDR;
    bldr_load_part("TDD_ONLY_MD_RAMDISK", bootdev, &addr, &size);

    //addr = CFG_MD2_RAMDISK_MEMADDR;
    bldr_load_part("MD2_RAMDISK", bootdev, &addr, &size);
}
#endif

#if CFG_LOAD_SLT_MD_DSP
if (1 == aarch64_slt_done())
{
    addr = CFG_MD_DSP_MEMADDR;
    bldr_load_part("FDD_MD_DSP",bootdev, &addr, &size);

    //addr = CFG_TDD_ONLY_MD_DSP_MEMADDR;
    bldr_load_part("TDD_ONLY_MD_DSP",bootdev, &addr, &size);

    //addr = CFG_MD2_DSP_MEMADDR;
    bldr_load_part("MD2_DSP",bootdev, &addr, &size);
}
#endif

#if CFG_LOAD_CONN_SYS
if (1 == aarch64_slt_done())
{
    addr = CFG_CONN_SYS_MEMADDR;
    //bldr_load_part(PART_CONN_SYS,bootdev, &addr, &size);
    bldr_load_part("CONN_SYS",bootdev, &addr, &size);
}
#endif

#if CFG_LOAD_SLT_MD
if (1 == aarch64_slt_done())
{
    int *pass_size = NULL;

    addr = CFG_FDD_MD_ROM_MEMADDR; // work around
    /* HVT_MD_ROM */
    //addr = CFG_HVT_MD_ROM_MEMADDR;
    bldr_load_part("HVT_MD_ROM", bootdev, &addr, &size);

    /* MD_DVT_ROM */
    //addr = CFG_MD_DVT_ROM_MEMADDR;
    bldr_load_part("MD_DVT_ROM", bootdev, &addr, &size);

    /* FDD_MD_ROM */
    //addr = CFG_FDD_MD_ROM_MEMADDR;
    bldr_load_part("FDD_MD_ROM", bootdev, &addr, &size);
    pass_size = addr - 4;
    *pass_size = size;
    print("assign 0x%X value %d\n", pass_size, size);

    /* TDD_MD_ROM */
    //addr = CFG_TDD_ONLY_ROM_MEMADDR;
    bldr_load_part("TDD_ONLY_ROM", bootdev, &addr, &size);
    pass_size = addr - 4;
    *pass_size = size;
    print("assign 0x%X value %d\n", pass_size, size);

    /* MD2_ROM */
    //addr = CFG_MD2_ROM_MEMADDR;
    bldr_load_part("MD2_ROM", bootdev, &addr, &size);
    pass_size = addr - 4;
    *pass_size = size;
    print("assign 0x%X value %d\n", pass_size, size);
}
#endif

#if CFG_LOAD_SLT_SCP
if (1 == aarch64_slt_done())
{
        DRV_WriteReg32(0x100A402C, 0);
        addr = 0;

    bldr_load_part("SCP",bootdev, &addr, &size);
}
#endif


#if CFG_LOAD_SLT_MD32
#define ReadREGMD32(_addr, _value) ((_value) = *(volatile unsigned int *)(_addr) )
#define WriteREGMD32(_addr, _value) (*(volatile unsigned int *)(_addr) = (_value))
if (1 == aarch64_slt_done())
{
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
     print("MD32 loadPM, srcaddr check1:%x, value:%x\n", p_addr, value);
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
    print("MD32 loadPM, srcaddr check2:%x, value:%x\n", p_addr, value);
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
    print("MD32 loadDM, srcaddr:%x, value:%x\n", d_addr, value);
    //memcpy((void *)0x10b38000, (void *)d_addr, sizeof(char)*size);

    DRV_WriteReg32(0x10B00330, 0x00000000);
}

#endif

#if MTK_TINYSYS_SSPM_SUPPORT
    addr = (u32)mblock_reserve(&bootarg.mblock_info,
            (u64)SSPM_MEM_SIZE, (u64)SSPM_MEM_ALIGN,
            SSPM_MEM_LIMIT, RANKMAX);

    if (!addr)
        return -1;

    ret = bldr_load_sspm_part(bootdev, &addr, &size);

    if (ret)
        return ret;

#if (!SSPM_DBG_MODE)
	emi_mpu_set_region_protection((u64)addr, (u64)addr + ROUNDUP(SSPM_TCM_SZ, SSPM_MEM_ALIGN) - 1, SSPM_MPU_REGION_ID,
        SET_ACCESS_PERMISSON(UNLOCK, FORBIDDEN, FORBIDDEN, FORBIDDEN, FORBIDDEN, SEC_RW, FORBIDDEN, FORBIDDEN, FORBIDDEN));
#endif

#endif

#if CFG_LOAD_SLT_SSPM
#define SSPM_MEM_SIZE	0x00080000  // 512K
#define SSPM_MEM_ALIGN  0x00010000  // 64K (minimal size for EMI MPU)
#define SSPM_MEM_LIMIT  0xC0000000
if (1 == aarch64_slt_done())
{
	addr = (u32)mblock_reserve(&bootarg.mblock_info,
			(u64)SSPM_MEM_SIZE, (u64)SSPM_MEM_ALIGN,
			SSPM_MEM_LIMIT, RANKMAX);

	if (!addr)
		return -1;

	ret = bldr_load_sspm_part_slt(bootdev, &addr, &size);

	if (ret)
		return ret;
}
#endif

#if CFG_LOAD_AP_ROM
if (1 == aarch64_slt_done())
{
    u32 p_addr;
    addr = CFG_AP_ROM_MEMADDR;
    //ret = bldr_load_part(PART_AP_ROM, bootdev, &addr, &size);
    ret = bldr_load_part("AP_ROM", bootdev, &addr, &size);
    if (ret)
    return ret;
    *jump_addr = addr;
}
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
    ret = bldr_load_part_lk(bootdev, &addr, &size);

    print("LK addr: 0x%x, size: 0x%x\n", addr, size);
    if (addr < CFG_UBOOT_MEMADDR || (addr + size) > (CFG_UBOOT_MEMADDR + 0x900000))
        print("Warning: LK out of boundary.\n");

    if (ret)
       return ret;
    *jump_addr = addr;
#endif //MTK_AB_OTA_UPDATER
#endif

#if CFG_LOAD_SLT_AARCH64_KERNEL
if (0 == aarch64_slt_done())
{
    addr = CFG_BOOTA64_MEMADDR;
    ret = bldr_load_part("boota64", bootdev, &addr, &size);
    addr = CFG_DTB_MEMADDR;
    ret = bldr_load_part("DTB", bootdev, &addr, &size);
    addr = CFG_IMAGE_AARCH64_MEMADDR;
    ret = bldr_load_part("Image_aarch64", bootdev, &addr, &size);
}
#endif

    BOOTING_TIME_PROFILING_LOG("before load TEE");
#if CFG_ATF_SUPPORT
#if defined(MTK_AB_OTA_UPDATER)
    addr = CFG_ATF_ROM_MEMADDR;

    if (!memcmp(ab_suffix, BOOTCTRL_SUFFIX_A, 2)) {
        ret = bldr_load_tee_part("tee_a", bootdev, &addr, 0, &size);
    } else {
        ret = bldr_load_tee_part("tee_b", bootdev, &addr, 0, &size);
    }

    print("%s bldr load tee part ret=0x%x, addr=0x%x\n", MOD, ret, addr);
#else
    addr = CFG_ATF_ROM_MEMADDR;
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

    print("%s bldr load tee part ret=0x%x, addr=0x%x\n", MOD, ret, addr);
#endif //MTK_AB_OTA_UPDATER
#endif
    BOOTING_TIME_PROFILING_LOG("after load TEE");

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

    print("\n%s jump to 0x%x\n", MOD, addr);
    print("%s <0x%x>=0x%x\n", MOD, addr, *(u32*)addr);
    print("%s <0x%x>=0x%x\n", MOD, addr + 4, *(u32*)(addr + 4));

    jump(addr, arg1, arg2);
}

void bldr_jump64(u32 addr, u32 arg1, u32 arg2)
{
    platform_wdt_kick();

    /* disable preloader safe mode */
    platform_safe_mode(0, 0);

    print("\n%s jump to 0x%x\n", MOD, addr);
#if !CFG_BYPASS_EMI
    print("%s <0x%x>=0x%x\n", MOD, addr, *(u32*)addr);
    print("%s <0x%x>=0x%x\n", MOD, addr + 4, *(u32*)(addr + 4));
#endif

#if CFG_ATF_SUPPORT
    trustzone_jump(addr, arg1, arg2);
#else
    print("%s trustzone is not supported!\n", MOD);

    #if CFG_LOAD_SLT_AARCH64_KERNEL
	print("%s jump to 64 bit SLT kernel!\n", MOD);
	jumparch64_slt();
    #endif

#endif
}

void main(u32 *arg)
{
    struct bldr_command_handler handler;
    u32 jump_addr, jump_arg;
    uint32_t ret = 0;

    /* get the bldr argument */
    p_bldr_param = &bldr_param;
    memcpy((void *)p_bldr_param, (void *)*arg, sizeof(bl_param_t));

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

#if CFG_CANCEL_BWDT_TIMEOUT
    /* Cancel BWDT timeout, otherwise it would reset in 2 second. */
    DRV_WriteReg32(0x100070A4, DRV_Reg32(0x100070A4) | 0x66000001);
#endif

    mtk_uart_init(UART_SRC_CLK_FRQ, CFG_LOG_BAUDRATE);
    bldr_pre_process();

    #ifdef HW_INIT_ONLY
#if !CFG_FPGA_PLATFORM
    /*
     * The following is requested by MD: Ying Hsu and Jim Chou
     * Set VCORE and VMODEM as 1.19375V respectively.
     * Read the settings back and print the results.
     */
    pmic_config_interface(0x152A, 0x6C, 0x7F,0);
    pmic_config_interface(0x15AA, 0x6F, 0x7F,0);

    unsigned int val = 0;
    pmic_read_interface(0x152A, &val, 0x7F,0);
    print("VCORE: %d\n", val);
    pmic_read_interface(0x15AA, &val, 0x7F,0);
    print("VMODEM: %d\n", val);
#endif

    bldr_wait_forever();
    #endif

    handler.priv = NULL;
    handler.attr = 0;
    handler.cb   = bldr_cmd_handler;

    BOOTING_TIME_PROFILING_LOG("before bldr_handshake");
    bldr_handshake(&handler);
    BOOTING_TIME_PROFILING_LOG("bldr_handshake");

#if !CFG_FPGA_PLATFORM
    /* security check */
    device_APC_dom_setup();
#endif
    BOOTING_TIME_PROFILING_LOG("sec_boot_check");

#if CFG_ATF_SUPPORT
    trustzone_pre_init();
#endif

    BOOTING_TIME_PROFILING_LOG("before load image");
#if !(CFG_BYPASS_LOAD_IMG_FORCE_ATF)
    /* Do not load ATF, lk, load by JTAG */
    if (0 != bldr_load_images(&jump_addr)) {
        print("%s Second Bootloader Load Failed\n", MOD);
        goto error;
    }
#else
    jump_addr = CFG_UBOOT_MEMADDR;
#endif
    BOOTING_TIME_PROFILING_LOG("load image");

    bldr_post_process();
#ifdef SLT
    mt_usb_phy_recover();
    //mu3d_hal_rst_dev();
#endif

#if CFG_ATF_SUPPORT
    trustzone_post_init();
#endif

#if CFG_LOAD_SLT_AARCH64_KERNEL
if (0 == aarch64_slt_done())
{
    *(unsigned int*) AARCH64_SLT_DONE_ADDRESS = AARCH64_SLT_DONE_MAGIC;
    jump_addr = CFG_BOOTA64_MEMADDR;
    //set up slave cpu reset address
    *(unsigned int*) 0x10200040 = CFG_BOOTA64_MEMADDR; //cpu1
    *(unsigned int*) 0x10200048 = CFG_BOOTA64_MEMADDR; //cpu2
    *(unsigned int*) 0x10200050 = CFG_BOOTA64_MEMADDR; //cpu3
    *(unsigned int*) 0x10200238 = CFG_BOOTA64_MEMADDR; //cpu4
    *(unsigned int*) 0x10200240 = CFG_BOOTA64_MEMADDR; //cpu5
    *(unsigned int*) 0x10200248 = CFG_BOOTA64_MEMADDR; //cpu6
    *(unsigned int*) 0x10200250 = CFG_BOOTA64_MEMADDR; //cpu7
    print("%s Aarch64 Kernel SLT , jump to 64 bit kernel, address: 0x%x\n", MOD,jump_addr);
    bldr_jump64(jump_addr, (u32)&bootarg, sizeof(boot_arg_t));
}
#endif

#if CFG_BOOT_ARGUMENT_BY_ATAG
    jump_arg = (u32)&(g_dram_buf->boottag);
#else
    jump_arg = (u32)&bootarg;
#endif


    /* 64S3,32S1,32S1 (MTK_ATF_BOOT_OPTION = 0)
	 * re-loader jump to LK directly and then LK jump to kernel directly */
#if CFG_ATF_SUPPORT
    print("%s Others, jump to ATF\n", MOD);
    bldr_jump64(jump_addr, jump_arg, sizeof(boot_arg_t));
#else
    bldr_jump(jump_addr, jump_arg, sizeof(boot_arg_t));
#endif

error:
    platform_error_handler();
}

