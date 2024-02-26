/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2018. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver\'s
 * applicable license agreements with MediaTek Inc.
 */

/* for general operations */
#include "sec_platform.h"

/* import customer configuration */
#include "sec_cust.h"

/* import sec cfg partition info */
#include "sec_rom_info.h"

/* customer key */
#include "cust_sec_ctrl.h"
#include "oemkey.h"
#include "KEY_IMAGE_AUTH.h"

/* for crypto operations */
#include "sec.h"
#include <verified_boot_error.h>

/* for storage device operations */
#include "cust_bldr.h"
#ifndef MTK_EMMC_SUPPORT
#include "nand.h"
#include "nand_core.h"
#endif
#include "dram_buffer.h"
#include "pal_log.h"
#include "partition_api.h"

/**************************************************************************
*  MACRO
**************************************************************************/
#define MOD                   "SEC"

#define SMSG(f, ...)          pal_log_info(f, ##__VA_ARGS__)
#define DMSG(f, ...)          pal_log_debug(f, ##__VA_ARGS__)

/**************************************************************************
 *  GLOBAL VARIABLES
 **************************************************************************/

SECURE_CFG_INFO                         sec_cfg_info;
unsigned int                            g_sec_cfg_exists;
u8 g_oemkey[OEM_PUBK_SZ] = {OEM_PUBK};

/**************************************************************************
 *  EXTERNAL VARIABLES
 **************************************************************************/
extern AND_ROMINFO_T                    g_ROM_INFO;
extern struct nand_chip                 g_nand_chip;
extern unsigned int heap_start_addr;
extern unsigned int heap_max_size;
extern unsigned int heap_current_alloc;

/**************************************************************************
 *  EXTERNAL FUNCTIONS
 **************************************************************************/
extern u32 get_sec_cfg_cnt_size(void);


U8* sec_cfg_load(void)
{
	U32 i       = 0;
	U8 *buf     = (U8 *)sec_util_get_working_buf();
	U32 seccfg_size = 0;

	blkdev_t    *bootdev = NULL;


	/* --------------------- */
	/* initialize buffer     */
	/* --------------------- */
	seccfg_size = get_sec_cfg_cnt_size();
	memset(buf, 0x0, seccfg_size);

	/* --------------------- */
	/* read sec cfg          */
	/* --------------------- */
	SMSG("\n\n[%s] read '0x%x'\n",MOD,sec_cfg_info.addr);

	if (NULL == (bootdev = blkdev_get(CFG_BOOT_DEV))) {
		SMSG("[%s] can't find boot device(%d)\n", MOD, CFG_BOOT_DEV);
		return NULL;
	}

	if (0 != blkdev_read(bootdev, sec_cfg_info.addr, seccfg_size, (u8*)buf, sec_cfg_info.part_id)) {
		SMSG("[%s] seccfg load fail\n", MOD);
	}

	/* dump first 8 bytes for debugging */
	for (i = 0; i < 8; i++) {
		SMSG("0x%x,",buf[i]);
	}
	SMSG("\n");

	return buf;
}


void sec_cfg_save(U8* src)
{
	U32 i       = 0;
	blkdev_t    *bootdev = NULL;
	/* --------------------- */
	/* write sec cfg          */
	/* --------------------- */

	SMSG("[%s] write '0x%x'\n",MOD,sec_cfg_info.addr);

	if (NULL == (bootdev = blkdev_get(CFG_BOOT_DEV))) {
		SMSG("[%s] can't find boot device(%d)\n", MOD, CFG_BOOT_DEV);
		ASSERT(0);
	}
#ifndef MTK_EMMC_SUPPORT
	nand_erase_data(sec_cfg_info.addr, g_nand_chip.chipsize, get_sec_cfg_cnt_size());
#endif
	if (0 != blkdev_write(bootdev, sec_cfg_info.addr, get_sec_cfg_cnt_size(), (u8*)src, sec_cfg_info.part_id)) {
		SMSG("[%s] seccfg write fail\n", MOD);
	}

	/* dump first 8 bytes for debugging */
	for (i = 0; i < 8; i++) {
		SMSG("0x%x,",src[i]);
	}
	SMSG("\n");

	return;
}



/**************************************************************************
 * [SECURE LIBRARY INITIALIZATION]
 **************************************************************************/
void sec_malloc_buf_reset(void)
{
	heap_start_addr    = (U32)sec_util_get_heap_buf();
	heap_max_size      = (U32)sec_util_get_heap_buf_sz();
	heap_current_alloc = (U32)0;

	return;
}

void sec_lib_init (void)
{

#ifdef MTK_SECURITY_SW_SUPPORT
    part_t *part;
    U32 err;
    CUSTOM_SEC_CFG cust_cfg;
    BOOL bAC = g_ROM_INFO.m_SEC_CTRL.m_seccfg_ac_en;
    g_sec_cfg_exists = 0;
    U8* seccfg_buf = NULL;
    blkdev_t    *bootdev = NULL;

    /* ---------------------- */
    /* check status           */
    /* ---------------------- */

    /* check customer configuration data structure */
    COMPILE_ASSERT(CUSTOM_SEC_CFG_SIZE == sizeof(CUSTOM_SEC_CFG));

    if (NULL == (bootdev = blkdev_get(CFG_BOOT_DEV))) {
        SMSG("[%s] can't find boot device(%d)\n", MOD, CFG_BOOT_DEV);
        if(is_BR_cmd_disabled())
            return;
        else {
            ASSERT(0);
            return;
        }
    }


    /* ---------------------- */
    /* initialize variables   */
    /* ---------------------- */
    sec_malloc_buf_reset();

    /* initialize customer configuration buffer */
    memset (&cust_cfg, 0x0, sizeof(cust_cfg));

    /* initialize customer configuration for security library */
    cust_cfg.sec_usb_dl = SEC_USBDL_CFG;
    cust_cfg.sec_boot = SEC_BOOT_CFG;
    /* initialize customer configuration for security library */
    memcpy (cust_cfg.crypto_seed, CUSTOM_CRYPTO_SEED, sizeof(cust_cfg.crypto_seed));

    /* ---------------------- */
    /* check data structure   */
    /* ---------------------- */

    sec_rom_info_init();
    sec_ctrl_init();

    err = sec_func_init(&cust_cfg, TRUE);
    if (STATUS_OK != err) {
        SMSG ("[%s] Basic config not available...\n", MOD);
        ASSERT(0);
    }

    part = part_get("seccfg");
    if (part) {
        sec_cfg_info.addr = part->start_sect * bootdev->blksz;
        sec_cfg_info.len = part->nr_sects * bootdev->blksz;
        sec_cfg_info.part_id = part->part_id;
        seccfg_buf = sec_cfg_load();
        if (seccfg_buf) {
#if !CFG_FPGA_PLATFORM
            /* starting to initialze security library */
            if (STATUS_OK == (err = sec_seccfg_init(seccfg_buf, SEC_CFG_READ_SIZE, TRUE, bAC)))
                g_sec_cfg_exists = 1;
            else
                SMSG("[%s] init fail '0x%x'\n",MOD,err);
#endif
        }
    } else {
        SMSG("[%s] seccfg part not found\n", MOD);
    }

    /* set root public key */
    seclib_set_pubk(g_oemkey, OEM_PUBK_SZ);
#else
    /* ROM_INFO must be linked even though MTK_SECURITY_SW_SUPPORT=0.
     * Therefore, we refer to ROM_INFO to make sure it's linked.
     */
    g_ROM_INFO.m_SEC_CTRL.reserve[0] = 0;
#endif

	return;
}

BOOL is_BR_cmd_disabled(void)
{
    U32 b_disable = 0;

    if ((TRUE == seclib_sec_usbdl_enabled(TRUE))
        && (STATUS_OK == seclib_get_pl_cmd_dis(&b_disable))) {
        if (b_disable) {
            SMSG("[%s] BR cmd is disabled\n", MOD);
            return TRUE;
        }
    }

    return FALSE;
}

