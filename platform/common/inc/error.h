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

#ifndef __ERROR_H__
#define __ERROR_H__

#include <pal_typedefs.h>

/* definitions */
#define STATUS_OK        (0x00000000)

#define ERR_MOD_MAX      (0x7ff)
#define ERR_MOD_LV1_MAX  (0xff)
#define ERR_MOD_LV2_MAX  (0xfff)

#define ERR_MOD_SHIFT    (20)
#define ERR_LV1_SHIFT    (12)
#define ERR_LV2_SHIFT    (0)

#define ERR_MOD_MASK     (ERR_MOD_MAX << ERR_MOD_SHIFT)       /* 0x7ff00000 */
#define ERR_MOD_LV1_MASK (ERR_MOD_LV1_MAX << ERR_LV1_SHIFT)   /* 0x000ff000 */
#define ERR_MOD_LV2_MASK (ERR_MOD_LV2_MAX << ERR_LV2_SHIFT)   /* 0x00000fff */

/* modules */
#define ERR_MOD_SECURITY_ID  (0x00000001)
#define ERR_MOD_PARTITION_ID (0x00000002)
#define ERR_MOD_STORAGE_ID   (0x00000003)

/* level 1 and level 2 error code are defined in module error headers */

/* macros */
#define MAKE_ERR_MOD_BASE(mod) (mod << ERR_MOD_SHIFT)
#define MAKE_ERR_MOD_LV1_BASE(mod, lv1_err) ((mod << ERR_MOD_SHIFT) | \
						(lv1_err << ERR_LV1_SHIFT))

uint32_t get_last_error(void);
void set_last_error(uint32_t error);

#endif /* __ERROR_H__ */

