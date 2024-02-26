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
 * The following software/firmware and/or related documentation ("Media Tek
 * Software") have been modified by MediaTek Inc. All revisions are subject to
 * any receiver's applicable license agreements with MediaTek Inc.
 */

#ifndef MT_PLAT_DBG_INFO_H
#define MT_PLAT_DBG_INFO_H

/* here include the header declaring users' data structure */
#ifdef LP4_DRAM_SUPPORT
#include "emi_lp4.h"
#else
#include "emi.h"
#endif

#define PLAT_DBG_INFO_BASE 0x11D800
#define PLAT_DBG_INFO_SIZE 256
#define INFO_TYPE_MAX 3

#define INFO_TAIL_MAGIC 0x72590000

#define INIT_DBG_HEAD(X) \
	dbg_info->head[TYPE_##X]=((KEY_##X<<16)|(sizeof(DEF_##X)&0xFFFF));
#define INFO_ALIGN_CHECK(X) \
	_Static_assert(sizeof(X)%4==0,#X" alignment is violated");

typedef enum {
	TYPE_LAST_DRAMC,
	TYPE_LAST_EMI,
	TYPE_END
} DBG_INFO_TYPE;
_Static_assert(TYPE_END <= INFO_TYPE_MAX, "TYPE_END is violated");

typedef struct {
	unsigned int head[INFO_TYPE_MAX];
#ifdef DEF_LAST_DRAMC
	DEF_LAST_DRAMC last_dramc;
#endif
#ifdef DEF_LAST_EMI
	DEF_LAST_EMI last_emi;
#endif
	unsigned int tail;
} top_dbg_info;
_Static_assert(sizeof(top_dbg_info) <= PLAT_DBG_INFO_SIZE, "PLAT_DBG_INFO_SIZE is violated");

#ifdef DEF_LAST_DRAMC
#define KEY_LAST_DRAMC 0xD8A3
INFO_ALIGN_CHECK(DEF_LAST_DRAMC)
#endif
#ifdef DEF_LAST_EMI
#define KEY_LAST_EMI 0xE31C
INFO_ALIGN_CHECK(DEF_LAST_EMI)
#endif

typedef struct {
	unsigned int key;
	unsigned int base;
	unsigned int size;
} dbg_info_in_bootargs;

unsigned int get_dbg_info_key(DBG_INFO_TYPE info_type);
unsigned int get_dbg_info_base(unsigned int key);
unsigned int get_dbg_info_size(unsigned int key);

#endif //MT_PLAT_DBG_INFO_H

