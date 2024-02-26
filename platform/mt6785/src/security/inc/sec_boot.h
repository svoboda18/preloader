/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2019. All rights reserved.
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


#ifndef SEC_BOOT_H
#define SEC_BOOT_H

#include "cust_sec_ctrl.h"
#include <pal_typedefs.h>

#define SEC_FEATURE_MASK_ANTIROLLBACK 0x00000001
/**************************************************************************
 * [SECURE PARTITION NAME]
 **************************************************************************/
/* Note : these definitions all sync with download tool. do not modify it */
#define SBOOT_PART_PL                   "preloader"
#define SBOOT_PART_UBOOT                "lk"
#define SBOOT_PART_LOGO                 "logo"
#define SBOOT_PART_BOOTIMG              "boot"
#define SBOOT_PART_ANDSYSIMG            "system"
#define SBOOT_PART_USERDATA             "userdata"
#define SBOOT_PART_CACHE                "cache"
#define SBOOT_PART_RECOVERY             "recovery"
#define SBOOT_PART_SECSTATIC            "secro"
#define SBOOT_PART_SECURE               "seccfg"

/**************************************************************************
 * [SECURE BOOT CHECK]
 **************************************************************************/
/* Note : this structure record all the partitions
          which should be verified by secure boot check */
#define AND_SEC_BOOT_CHECK_PART_SIZE    (90)
typedef struct {
    unsigned char                       name[9][10];
} AND_SECBOOT_CHECK_PART_T;

uint32_t sec_get_img_size(char *part_name, char *img_name,
			  uint32_t *img_size);

#endif /* SEC_BOOT_H */

