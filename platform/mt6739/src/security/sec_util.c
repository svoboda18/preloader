/*****************************************************************************
 *  Copyright Statement:
 *  --------------------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of MediaTek Inc. (C) 2011
 *
 *  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 *  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 *  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
 *  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 *  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 *  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 *  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
 *  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
 *  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *
 *  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
 *  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 *  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 *  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
 *  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 *  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
 *  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
 *  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
 *  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
 *  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
 *
 *****************************************************************************/

#include "sec_platform.h"
#include "nand_core.h"
#include "boot_device.h"
#include "cust_bldr.h"
#include "cust_sec_ctrl.h"
#include "sec_boot.h"
#include "sec.h"
#include <dram_buffer.h>
#include <sec_util.h>
#include <partition_api.h>
#include <storage_api.h>
#include <pal_log.h>
#include <verified_boot_error.h>

/******************************************************************************
 * CONSTANT DEFINITIONS
 ******************************************************************************/
#define MOD                   "LIB"

#define SMSG(f, ...)          pal_log_info(f, ##__VA_ARGS__)
#define DMSG(f, ...)          pal_log_debug(f, ##__VA_ARGS__)

/******************************************************************************
 *  EXTERNAL VARIABLES
 ******************************************************************************/
extern struct nand_chip       g_nand_chip;
extern boot_arg_t             bootarg;
U32 sec_dram_init = 0;

/******************************************************************************
 *  INTERNAL VARIABLES
 ******************************************************************************/
BOOL bDumpPartInfo                  = FALSE;
static BOOL bUsbHandshakeSuccess    = FALSE;

/******************************************************************************
 *  RETURN AVAILABLE BUFFER FOR S-BOOT CHECK
 ******************************************************************************/
U8* sec_util_get_img_buf (void)
{
	return (U8*) SEC_SRAM_IMG_BUF;
}

U8* sec_util_get_working_buf (void)
{
	return (U8*) SEC_SRAM_WORKING_BUF;
}

U8 *sec_util_get_heap_buf(void)
{
	return (U8 *)SEC_SRAM_HEAP_BUF;
}

U32 sec_util_get_heap_buf_sz(void)
{
	return SEC_SRAM_HEAP_LENGTH;
}


/******************************************************************************
 *  READ IMAGE FOR S-BOOT CHECK (FROM NAND or eMMC DEVICE)
 ******************************************************************************/
U32 sec_util_read_image (char* img_name, U8 *buf, U64 offset, U32 size)
{
	U32 ret            = STATUS_OK;
	U32 i               = 0;
	U32 cnt             = 0;

	U32 total_pages     = 0;
	blkdev_t *bootdev   = NULL;
	part_t *part        = NULL;
	U64 src;
	U64 img_addr = 0;
#if CFG_PRELOADER_EXTENSION && CFG_DRAM_CALIB_OPTIMIZATION
	struct part_info_t part_info;
#endif


	if (NULL == (bootdev = blkdev_get(CFG_BOOT_DEV))) {
		SMSG("no boot device(%d)\n",  CFG_BOOT_DEV);
		return ERR_VB_NOT_VALID_PTR;
	}

#if CFG_PRELOADER_EXTENSION && CFG_DRAM_CALIB_OPTIMIZATION
	if (!sec_dram_init) {
		ret = mt_get_part_info_by_name(img_name, &part_info);
		if (ret) {
			SMSG("part_load fail\n");
			ASSERT(0);
		}
		DMSG("img_addr: %llx\n", part_info.addr);
		ret = blkdev_read(bootdev, part_info.addr + offset, size, buf, EMMC_PART_USER);
		if (ret) {
			SMSG("part_load fail\n");
			ASSERT(0);
		}
		goto end;
	}
#endif

	/* ======================== */
	/* get part info            */
	/* ======================== */
	/* part_get should be device abstraction function */
	if(NULL == (part = part_get(img_name))) {
		SMSG("part_get fail\n");
		return ERR_VB_NOT_VALID_PTR;
	}
	/* ======================== */
	/* read part data           */
	/* ======================== */
	/* part_load should be device abstraction function */
	if(TRUE == bDumpPartInfo) {
#if ((CFG_BOOT_DEV == BOOTDEV_SDMMC) || (CFG_BOOT_DEV == BOOTDEV_UFS))
		SMSG("part load'0x%x'\n", part->start_sect * bootdev->blksz);
#else
		SMSG("part load'0x%x'\n", mt_part_get_start_addr(part));
#endif
		bDumpPartInfo = FALSE;
	}
#if ((CFG_BOOT_DEV == BOOTDEV_SDMMC) || (CFG_BOOT_DEV == BOOTDEV_UFS))
	src = (U64)part->start_sect * (U64)bootdev->blksz + offset;
#else
	src = mt_part_get_start_addr(part) + offset;
#endif

#if ((CFG_BOOT_DEV == BOOTDEV_SDMMC) || (CFG_BOOT_DEV == BOOTDEV_UFS))
	if (0 != blkdev_read(bootdev, src, size, buf, part->part_id)) {
#else
	if (0 != blkdev_read(bootdev, src, size, buf, mt_part_get_part_id(part))) {
#endif
		SMSG("part_load fail\n");
		ASSERT(0);
	}

end:
	return ret;
}

/******************************************************************************
 *  WRITE IMAGE FOR S-BOOT USAGE (FROM NAND or eMMC DEVICE)
 ******************************************************************************/
static U32 sec_util_write_image (U8* img_name, U8 *buf, U64 offset, U32 size)
{
	U32 ret            = STATUS_OK;
	U32 i               = 0;
	U32 cnt             = 0;

	U32 total_pages     = 0;
	blkdev_t *bootdev   = NULL;
	part_t *part        = NULL;
	U64 dest;


	if (NULL == (bootdev = blkdev_get(CFG_BOOT_DEV))) {
		SMSG("no boot device(%d)\n", CFG_BOOT_DEV);
		return ERR_VB_NOT_VALID_PTR;
	}

	/* ======================== */
	/* get part info            */
	/* ======================== */
	/* part_get should be device abstraction function */
	if(NULL == (part = part_get (img_name))) {
		SMSG("part_get fail\n");
		return ERR_VB_NOT_VALID_PTR;
	}

	/* ======================== */
	/* write part data           */
	/* ======================== */
	/* part_load should be device abstraction function */
	if(TRUE == bDumpPartInfo) {
#if ((CFG_BOOT_DEV == BOOTDEV_SDMMC) || (CFG_BOOT_DEV == BOOTDEV_UFS))
		SMSG("part load'0x%x'\n",  part->start_sect * bootdev->blksz);
#else
		SMSG("part load'0x%x'\n",  mt_part_get_start_addr(part));
#endif
		bDumpPartInfo = FALSE;
	}
#if ((CFG_BOOT_DEV == BOOTDEV_SDMMC) || (CFG_BOOT_DEV == BOOTDEV_UFS))
	dest = (U64)part->start_sect * (U64)bootdev->blksz + offset;
#else
	dest = mt_part_get_start_addr(part) + offset;
#endif

#if ((CFG_BOOT_DEV == BOOTDEV_SDMMC) || (CFG_BOOT_DEV == BOOTDEV_UFS))
	if (0 != blkdev_write(bootdev, dest, size, buf, part->part_id)) {
#else
	if (0 != blkdev_write(bootdev, dest, size, buf, mt_part_get_part_id(part))) {
#endif
		SMSG("part_store fail\n");
		ASSERT(0);
	}

	return ret;
}

static BOOL sec_util_force_brom_download_recovery(void)
{
#define SEC_PL_ERASE_SIZE 2048
	u8 *sec_buf = sec_util_get_img_buf();

	memset(sec_buf,0,SEC_PL_ERASE_SIZE);

	if (STATUS_OK != sec_util_write_image(SBOOT_PART_PL, sec_buf, 0, SEC_PL_ERASE_SIZE)) {
		SMSG("Write image fail for seek offset\n");
		return FALSE;
	}

	SMSG("Force brom dl recovery success\n");
	return TRUE;
}

BOOL sec_util_brom_download_recovery_check(void)
{
#ifdef KPD_DL_KEY2
	if (mtk_detect_key (KPD_DL_KEY2) && FALSE==bUsbHandshakeSuccess
			&& is_BR_cmd_disabled()) {
		SMSG("[%s] Start checking (1500 ms)\n", MOD);
		mdelay(1500);

		if (false == mtk_detect_key (KPD_DL_KEY2)) {
			SMSG("[%s] Key is not detected, wait for 1500ms \n", MOD);
			mdelay(1500);
			if(mtk_detect_key (KPD_DL_KEY2)) {
				SMSG("[%s] Key is detected\n", MOD);
				return sec_util_force_brom_download_recovery();
			} else {
				SMSG("[%s] Key is not detected\n", MOD);
				return FALSE;
			}
		} else {
			SMSG("[%s] Key is detected\n", MOD);
			return FALSE;
		}
	}
#endif
	return FALSE;
}

void sec_set_usb_handshake_status(BOOL status_ok)
{
	bUsbHandshakeSuccess = status_ok;
}

void sec_util_force_entering_fastboot_mode(void){
	u32 addr = CFG_UBOOT_MEMADDR;

	g_boot_mode = FASTBOOT;
	platform_set_boot_args();
	bldr_jump(addr, &bootarg, sizeof(boot_arg_t));

	DMSG("err jumping to fb mode\n");
	while(1);
}



