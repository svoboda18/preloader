/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */

/* MediaTek Inc. (C) 2017. All rights reserved.
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

/* pal api */
#include <pal_typedefs.h>
#include <pal_assert.h>
#include <pal_log.h>

/* common service api */
#include <storage_error.h>
#include <storage_api.h>

#include <blkdev.h>
#include <boot_device.h>

#define MOD "storage"

/* in order to export the same interface to user, generic physical region is used
   and converted to physical region type for each storage type */
uint32_t storage_get_part_id(uint32_t phys_region)
{
	/* there is no physical region attribute for nand */
	return 0;
}

uint32_t storage_get_block_size(uint32_t *blk_sz)
{
	uint32_t ret = STATUS_OK;
	blkdev_t *blkdev = NULL;

	if (NULL == (blkdev = blkdev_get(CFG_BOOT_DEV))) {
		ret = ERR_STORAGE_GENERAL_GET_BLOCKDEV_FAIL;
		pal_log_err("[%s] can't find boot device(%d)\n", MOD, CFG_BOOT_DEV);
		return ret;
	}

	*blk_sz = blkdev->blksz;

	return ret;
}

ssize_t storage_read(uint32_t phys_part, uint64_t offset, uint8_t *buf,
		     uint32_t size)
{
	ssize_t len = 0;
	uint32_t phys_part_id = 0;
	blkdev_t *blkdev = NULL;

	if (NULL == (blkdev = blkdev_get(CFG_BOOT_DEV))) {
		set_last_error(ERR_STORAGE_GENERAL_GET_BLOCKDEV_FAIL);
		pal_log_err("[%s] can't find boot device(%d)\n", MOD, CFG_BOOT_DEV);
		return -1;
	}

	phys_part_id = storage_get_part_id(phys_part);

	if (0 == blkdev_read(blkdev, offset, size, buf, phys_part_id))
		len = size;
	else {
		set_last_error(ERR_STORAGE_GENERAL_READ_FAIL);
		pal_log_err("[%s] raw_read fail\n", MOD);
		return -1;
	}

	return len;
}

ssize_t storage_write(uint32_t phys_part, uint64_t offset, uint8_t *buf,
		      uint32_t size)
{
	ssize_t len = 0;
	blkdev_t *blkdev = NULL;
	uint32_t phys_part_id = 0;

	if (NULL == (blkdev = blkdev_get(CFG_BOOT_DEV))) {
		set_last_error(ERR_STORAGE_GENERAL_GET_BLOCKDEV_FAIL);
		pal_log_err("[%s] can't find boot device(%d)\n", MOD, CFG_BOOT_DEV);
		return -1;
	}

	phys_part_id = storage_get_part_id(phys_part);

	if (0 == blkdev_write(blkdev, offset, size, buf, phys_part_id))
		len = size;

	else {
		set_last_error(ERR_STORAGE_GENERAL_WRITE_FAIL);
		pal_log_err("[%s] raw_read fail\n", MOD);
		return -1;
	}

	return len;
}

uint32_t storage_switch_boot_part(uint32_t phys_part)
{
	uint32_t ret = STATUS_OK;

	PAL_UNUSED_PARAM(phys_part);

	return ret;
}

