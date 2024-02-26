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
 * MediaTek Inc. (C) 2016. All rights reserved.
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

#include <plat_dbg_info.h>

static top_dbg_info *dbg_info;
static int init = 0;

static void init_dbg_info(void)
{
	unsigned int i;
	unsigned int offset, size;

	dbg_info = (top_dbg_info *) PLAT_DBG_INFO_BASE;

	for (i = 0; i < INFO_TYPE_MAX; i++)
		*((unsigned int *)dbg_info + i) = 0;

#ifdef KEY_LAST_DRAMC
	INIT_DBG_HEAD(LAST_DRAMC)
#endif
#ifdef KEY_LAST_EMI
	INIT_DBG_HEAD(LAST_EMI)
#endif
#ifdef KEY_PLAT_SRAM_FLAG
	INIT_DBG_HEAD(PLAT_SRAM_FLAG)
#endif


	offset = 4 * INFO_TYPE_MAX;
	for (i = 0; i < TYPE_END; i++) {
		size = dbg_info->head[i];
		if ((size >> 16) == 0)
			continue;
		dbg_info->head[i] = (dbg_info->head[i] & 0xffff0000) | offset;
		offset += (size & 0xffff);
	}

	dbg_info->tail = INFO_TAIL_MAGIC | sizeof(top_dbg_info);

	init = 1;
}

unsigned int get_dbg_info_key(DBG_INFO_TYPE info_type)
{
	if (init == 0)
		init_dbg_info();

	return (*((unsigned int *)dbg_info + info_type) >> 16);
}

unsigned int get_dbg_info_base(unsigned int key)
{
	unsigned int i;
	unsigned int offset;

	if (key == 0)
		return 0;

	if (init == 0)
		init_dbg_info();

	for (i = 0; i < TYPE_END; i++) {
		offset = *((unsigned int *)dbg_info + i);
		if ((offset >> 16) == key)
			return ((unsigned int)dbg_info + (offset & 0xffff));
	}

	return 0;
}

unsigned int get_dbg_info_size(unsigned int key)
{
	unsigned int offset_start, offset_end;
	unsigned int i;

	if (key == 0)
		return 0;

	if (init == 0)
		init_dbg_info();

	for (i = 0; i < TYPE_END; i++) {
		offset_start = *((unsigned int *)dbg_info + i);
		if ((offset_start >> 16) == key)
			break;
	}
	offset_start &= 0xffff;

	offset_end = dbg_info->tail - 4;
	for (i++; i < TYPE_END; i++) {
		if ((*((unsigned int *)dbg_info + i) >> 16) != 0) {
			offset_end = *((unsigned int *)dbg_info + i);
			break;
		}
	}

	offset_end &= 0xffff;

	return (offset_end - offset_start);
}

