/* Copyright Statement:
*
* This software/firmware and related documentation ("MediaTek Software") are
* protected under relevant copyright laws. The information contained herein
* is confidential and proprietary to MediaTek Inc. and/or its licensors.
* Without the prior written permission of MediaTek inc. and/or its licensors,
* any reproduction, modification, use or disclosure of MediaTek Software,
* and information contained herein, in whole or in part, shall be strictly prohibited.
*/
/* MediaTek Inc. (C) 2015. All rights reserved.
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

#include "sec_platform.h"
#include "nand_core.h"
#include "boot_device.h"
#include "cust_bldr.h"
#include "cust_sec_ctrl.h"
#include "sec_boot.h"
#include "sec.h"
#include <dram_buffer.h>
#include <pal_typedefs.h>
#include <pal_log.h>
#include <sec_util.h>
#include <partition_api.h>
#include <storage_api.h>
#include <error.h>

/******************************************************************************
 * CONSTANT DEFINITIONS
 ******************************************************************************/
#define MOD                         "LIB"

/******************************************************************************
 *  EXTERNAL VARIABLES
 ******************************************************************************/
extern boot_arg_t                   bootarg;

/******************************************************************************
 *  INTERNAL VARIABLES
 ******************************************************************************/
static BOOL bUsbHandshakeSuccess    = FALSE;

/******************************************************************************
 *  RETURN AVAILABLE BUFFER FOR S-BOOT CHECK
 ******************************************************************************/
uint8_t *sec_util_get_img_buf(void)
{
	return (uint8_t *)(g_dram_buf->sec_img_buf);
}

uint32_t sec_util_get_img_buf_sz(void)
{
	return SEC_IMG_BUFFER_LENGTH;
}

uint8_t *sec_util_get_working_buf(void)
{
	return (uint8_t *)(g_dram_buf->sec_working_buf);
}

uint32_t sec_util_get_working_buf_sz(void)
{
	return SEC_WORKING_BUFFER_LENGTH;
}

uint8_t *sec_util_get_heap_buf(void)
{
	return (uint8_t *)(g_dram_buf->sec_lib_heap_buf);
}

uint32_t sec_util_get_heap_buf_sz(void)
{
	return SEC_LIB_HEAP_LENGTH;
}

uint8_t *sec_util_get_secro_buf(void)
{
	return (uint8_t *)(g_dram_buf->sec_secro_buf);
}

uint32_t sec_util_get_secro_buf_sz(void)
{
	return SEC_SECRO_BUFFER_LENGTH;
}

uint8_t *get_vb_buf(void)
{
	return sec_util_get_img_buf();
}

uint32_t get_vb_buf_sz(void)
{
	return sec_util_get_img_buf_sz();
}

/******************************************************************************
 *  READ IMAGE FOR S-BOOT CHECK (FROM NAND or eMMC DEVICE)
 ******************************************************************************/
uint32_t sec_util_raw_read(uint64_t offset,
			   uint8_t *buf,
			   uint32_t size)
{
	ssize_t len = storage_read(STORAGE_PHYS_PART_USER, offset, buf, size);

	if (len == size)
		return 0;
	else
		return get_last_error();
}

uint32_t sec_util_raw_write(uint64_t offset,
			    uint8_t *buf,
			    uint32_t size)
{
	ssize_t len = storage_write(STORAGE_PHYS_PART_USER, offset, buf, size);

	if (len == size)
		return 0;
	else
		return get_last_error();
}

uint32_t sec_util_read_image(char *part_name,
			     uint8_t *buf,
			     uint64_t offset,
			     uint32_t size)
{
	ssize_t len = partition_read(part_name, offset, buf, size);

	if (len == size)
		return 0;
	else
		return get_last_error();
}

/******************************************************************************
 *  WRITE IMAGE FOR S-BOOT USAGE (FROM NAND or eMMC DEVICE)
 ******************************************************************************/
static uint32_t sec_util_write_image(char *part_name, uint8_t *buf,
				     uint64_t offset, uint32_t size)
{
	ssize_t len = partition_write(part_name, offset, buf, size);

	if (len == size)
		return 0;
	else
		return get_last_error();
}

static BOOL sec_util_force_brom_download_recovery(void)
{
#define SEC_PL_ERASE_SIZE 2048
	uint8_t *sec_buf = sec_util_get_img_buf();

	memset(sec_buf, 0, SEC_PL_ERASE_SIZE);

	if (STATUS_OK != sec_util_write_image(SBOOT_PART_PL, sec_buf, 0,
					      SEC_PL_ERASE_SIZE)) {
		pal_log_err("[%s] Write image fail for seek offset 0x%x\n", MOD, 0);
		return FALSE;
	}

	pal_log_info("[%s] Force brom download recovery success\n", MOD);
	return TRUE;
}

BOOL sec_util_brom_download_recovery_check(void)
{
#ifdef KPD_DL_KEY2
	if (mtk_detect_key(KPD_DL_KEY2) && FALSE == bUsbHandshakeSuccess
	    && is_BR_cmd_disabled()) {
		pal_log_info("[%s] Start checking (1500 ms)\n", MOD);
		mdelay(1500);

		if (false == mtk_detect_key(KPD_DL_KEY2)) {
			pal_log_info("[%s] Key is not detected, wait for 1500ms \n", MOD);
			mdelay(1500);
			if (mtk_detect_key(KPD_DL_KEY2)) {
				pal_log_info("[%s] Key is detected\n", MOD);
				return sec_util_force_brom_download_recovery();
			} else {
				pal_log_info("[%s] Key is not detected\n", MOD);
				return FALSE;
			}
		} else {
			pal_log_info("[%s] Key is detected\n", MOD);
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

void sec_util_force_entering_fastboot_mode(void)
{
	uint32_t addr = CFG_UBOOT_MEMADDR;

	g_boot_mode = FASTBOOT;
	platform_set_boot_args();
	bldr_jump(addr, &bootarg, sizeof(boot_arg_t));

	pal_log_debug("error on jumping to fastboot mode\n");
	pal_assert(0);
}

