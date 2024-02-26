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

#include "sec_platform.h"
#include "sec_region.h"
#include "sec.h"

/******************************************************************************
 * MODULE
 ******************************************************************************/
#define MOD                         "SEC_REGION"

/******************************************************************************
 * DEBUG
 ******************************************************************************/
#define SMSG                        dbg_print

/******************************************************************************
 *  EXTERNAL VARIABLES
 ******************************************************************************/

/******************************************************************************
 *  SECURITY REGION CHECK
 ******************************************************************************/
struct region {
	unsigned int start;
	unsigned int size;
};

static struct region g_read_whitelist[] = {
	{RGU_BASE, 0x1000},
	{USBDL_FLAG, 0x4},
	{DEVINFO_BASE, 0x1000}
};

static struct region g_write_whitelist[] = {
	{RGU_BASE, 0x1000},
	{USBDL_FLAG, 0x4}
};

/* check whether region 1 in in region 2 */
unsigned int is_in_region(struct region *region1, struct region *region2)
{
	unsigned int in = 0;

	if ((region1->start >= region2->start) &&
	    (region1->start + region1->size) <=
	    (region2->start + region2->size))
		in = 1;

	return in;
}

int whitelist_check(U32 addr, U32 len, U32 is_read)
{
	unsigned int i = 0;
	unsigned int whitelist_size = 0;
	struct region region;

	region.start = (unsigned int)addr;
	region.size = (unsigned int)len;

	if (is_read) {
		whitelist_size = sizeof(g_read_whitelist) / sizeof(struct region);
		for (i = 0; i < whitelist_size; i++) {
			if (1 == is_in_region(&region, g_read_whitelist + i))
				return 0;
		}
	} else {
		whitelist_size = sizeof(g_write_whitelist) / sizeof(struct region);
		for (i = 0; i < whitelist_size; i++) {
			if (1 == is_in_region(&region, g_write_whitelist + i))
				return 0;
		}
	}

	return -1;

}

U32 sec_region_check(U32 addr, U32 len, U32 is_read)
{
	U32 ret = SEC_OK;
	U32 tmp = addr + len;

	if (whitelist_check(addr, len, is_read)) {
		SMSG("[%s] Not Allowed\n", MOD);
		ret = 0xffffffff;
	}

	return ret;
}

/******************************************************************************
 *  DA REGION CHECK
 ******************************************************************************/
U32 da_region_check (U32 addr, U32 len)
{
	U32 ret = SEC_OK;

	if (DA_DOWNLOAD_LOC != addr) {
		ret = ERR_DA_INVALID_LOCATION;
		goto _exit;
	}

	if (DA_DOWNLOAD_MAX_SZ < len) {
		ret = ERR_DA_INVALID_LENGTH;
		goto _exit;
	}

_exit:

	return ret;
}
