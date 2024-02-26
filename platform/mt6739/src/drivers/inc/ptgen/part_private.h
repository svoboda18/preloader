/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2012. All rights reserved.
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
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */
#include "mt_partition.h"

part_t partition_layout[PART_MAX_COUNT];

struct part_name_map g_part_name_map[PART_MAX_COUNT] = {
	{"preloader",	PART_PRELOADER,	"raw data",	0,	0,	0},
	{"pro_info",	PART_PRO_INFO,	"raw data",	1,	0,	0},
	{"nvram",	PART_NVRAM,	"raw data",	2,	0,	0},
	{"protect_f",	PART_PROTECT_F,	"yaffs2",	3,	0,	0},
	{"protect_s",	PART_PROTECT_S,	"yaffs2",	4,	0,	0},
	{"seccfg",	PART_SECURE,	"raw data",	5,	0,	0},
	{"uboot",	PART_UBOOT,	"raw data",	6,	0,	0},
	{"boot",	PART_BOOTIMG,	"raw data",	7,	1,	1},
	{"recovery",	PART_RECOVERY,	"raw data",	8,	0,	0},
	{"sec_ro",	PART_SECSTATIC,	"raw data",	9,	0,	0},
	{"misc",	PART_MISC,	"raw data",	10,	0,	0},
	{"frp",	PART_FRP,	"raw data",	11,	0,	0},
	{"logo",	PART_LOGO,	"raw data",	12,	0,	0},
	{"expdb",	PART_APANIC,	"raw data",	13,	0,	0},
	{"nvdata",	PART_NVDATA,	"ubifs",	14,	0,	0},
	{"system",	PART_ANDROID,	"ubifs",	15,	1,	1},
	{"cache",	PART_CACHE,	"ubifs",	16,	1,	1},
	{"userdata",	PART_USRDATA,	"ubifs",	17,	1,	1},
};

