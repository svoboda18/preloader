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

#include <pal_typedefs.h>
#include <pal_assert.h>
#include <storage_api.h>
#include <partition_api.h>
#include <partition_error.h>
#include <partition_active.h>

#include <platform.h>
#include <boot_device.h>
#include <partition.h>

/* not exported to user */
#define PARTITION_OP_READ  (0)
#define PARTITION_OP_WRITE (1)

static ssize_t partition_op(uint32_t op, char *part_name, uint64_t offset,
			    uint8_t *buf, uint32_t size)
{
	ssize_t len = 0;
	uint32_t ret = STATUS_OK;
	uint32_t blk_sz = 0x0;
	uint64_t src = 0x0ULL;
	part_t *part = NULL;

	if (NULL == (part = part_get(part_name))) {
		set_last_error(ERR_PART_GENERAL_NOT_EXIST);
		len = -1;
		goto end;
	}

	ret = storage_get_block_size(&blk_sz);
	if (ret) {
		set_last_error(ret);
		len = -1;
		goto end;
	}

	src = (uint64_t)part->start_sect * blk_sz + offset;

	/* currently we support USER region access only, to access other physical regions,
	   you need partition name to physical region translation logic here */

	if (op == PARTITION_OP_READ) {
		len = storage_read(STORAGE_PHYS_PART_USER, src, buf, size);
		if (len != size)
			goto end;
	} else if (op == PARTITION_OP_WRITE) {
		len = storage_write(STORAGE_PHYS_PART_USER, src, buf, size);
		if (len != size)
			goto end;
	} else {
		/* unknown partition operation */
		set_last_error(ERR_PART_GENERAL_UNKNOWN_OP);
		len = -1;
	}

end:
	return len;
}

ssize_t partition_read(char *part_name, uint64_t offset, uint8_t *buf,
		       uint32_t size)
{
	ssize_t len = 0;
	uint32_t ret = STATUS_OK;
	char active_part_name[PART_NAME_BUF_SZ] = {0};
	uint32_t input_part_name_sz = 0;

	if (NULL == part_name) {
		set_last_error(ERR_PART_GENERAL_INVALID_NAME);
		return -1;
	}

	input_part_name_sz = strlen(part_name);

	if (input_part_name_sz > (PART_NAME_BUF_SZ - 1)) {
		set_last_error(ERR_PART_GENERAL_INVALID_NAME_SZ);
		return -1;
	}

	memcpy(active_part_name, part_name, input_part_name_sz);

	/* name translation logic here to get active partition name */
	/* gpt active and ab system are mutually exclusive */
#if defined (MTK_AB_OTA_UPDATER)
	ret = partition_get_ab(active_part_name, PART_NAME_BUF_SZ, GET_ACTIVE);
	if (ret) {
		set_last_error(ret);
		len = -1;
		goto end;
	}
#else
	ret = partition_get_gpt_active(active_part_name, PART_NAME_BUF_SZ, GET_ACTIVE);
	if (ret) {
		set_last_error(ret);
		len = -1;
		goto end;
	}
#endif

	len = partition_op(PARTITION_OP_READ, active_part_name, offset, buf, size);
	if (len != size)
		goto end;

end:
	return len;
}

ssize_t partition_write(char *part_name, uint64_t offset, uint8_t *buf,
			uint32_t size)
{
	ssize_t len = 0;
	uint32_t ret = STATUS_OK;
	char active_part_name[PART_NAME_BUF_SZ] = {0};
	uint32_t input_part_name_sz = 0;

	if (NULL == part_name) {
		set_last_error(ERR_PART_GENERAL_INVALID_NAME);
		return -1;
	}

	input_part_name_sz = strlen(part_name);

	if (input_part_name_sz > (PART_NAME_BUF_SZ - 1)) {
		set_last_error(ERR_PART_GENERAL_INVALID_NAME_SZ);
		return -1;
	}

	memcpy(active_part_name, part_name, input_part_name_sz);

	/* name translation logic here to get active partition name */
	/* gpt active and ab system are mutually exclusive */
#if defined (MTK_AB_OTA_UPDATER)
	ret = partition_get_ab(active_part_name, PART_NAME_BUF_SZ, GET_ACTIVE);
	if (ret) {
		set_last_error(ret);
		len = -1;
		goto end;
	}
#else
	ret = partition_get_gpt_active(active_part_name, PART_NAME_BUF_SZ, GET_ACTIVE);
	if (ret) {
		set_last_error(ret);
		len = -1;
	}
#endif

	len = partition_op(PARTITION_OP_WRITE, active_part_name, offset, buf, size);
	if (len != size)
		goto end;

end:
	return len;
}

ssize_t partition_read_inactive(char *part_name, uint64_t offset, uint8_t *buf,
				uint32_t size)
{
	ssize_t len = 0;
	uint32_t ret = STATUS_OK;
	char inactive_part_name[PART_NAME_BUF_SZ] = {0};
	uint32_t input_part_name_sz = 0;

	if (NULL == part_name) {
		set_last_error(ERR_PART_GENERAL_INVALID_NAME);
		return -1;
	}

	input_part_name_sz = strlen(part_name);

	if (input_part_name_sz > (PART_NAME_BUF_SZ - 1)) {
		set_last_error(ERR_PART_GENERAL_INVALID_NAME_SZ);
		return -1;
	}

	memcpy(inactive_part_name, part_name, input_part_name_sz);

	/* name translation logic here to get active partition name */
	/* gpt active and ab system are mutually exclusive */
#if defined (MTK_AB_OTA_UPDATER)
	ret = partition_get_ab(inactive_part_name, PART_NAME_BUF_SZ, GET_INACTIVE);
	if (ret) {
		set_last_error(ret);
		len = -1;
		goto end;
	}
#else
	ret = partition_get_gpt_active(inactive_part_name, PART_NAME_BUF_SZ,
				       GET_INACTIVE);
	if (ret) {
		set_last_error(ret);
		len = -1;
		goto end;
	}
#endif

	len = partition_op(PARTITION_OP_READ, inactive_part_name, offset, buf, size);
	if (len != size)
		goto end;

end:
	return len;
}

ssize_t partition_write_inactive(char *part_name, uint64_t offset, uint8_t *buf,
				 uint32_t size)
{
	ssize_t len = 0;
	uint32_t ret = STATUS_OK;
	char inactive_part_name[PART_NAME_BUF_SZ] = {0};
	uint32_t input_part_name_sz = 0;

	if (NULL == part_name) {
		set_last_error(ERR_PART_GENERAL_INVALID_NAME);
		return -1;
	}

	input_part_name_sz = strlen(part_name);

	if (input_part_name_sz > (PART_NAME_BUF_SZ - 1)) {
		set_last_error(ERR_PART_GENERAL_INVALID_NAME_SZ);
		return -1;
	}

	memcpy(inactive_part_name, part_name, input_part_name_sz);

	/* name translation logic here to get active partition name */
	/* gpt active and ab system are mutually exclusive */
#if defined (MTK_AB_OTA_UPDATER)
	ret = partition_get_ab(inactive_part_name, PART_NAME_BUF_SZ, GET_INACTIVE);
	if (ret) {
		set_last_error(ret);
		len = -1;
		goto end;
	}
#else
	ret = partition_get_gpt_active(inactive_part_name, PART_NAME_BUF_SZ,
				       GET_INACTIVE);
	if (ret) {
		set_last_error(ret);
		len = -1;
		goto end;
	}
#endif

	len = partition_op(PARTITION_OP_WRITE, inactive_part_name, offset, buf, size);
	if (len != size)
		goto end;

end:
	return len;
}

