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

#ifndef __MKIMG_H__
#define __MKIMG_H__

#include <pal_typedefs.h>

#define MKIMG_MAGIC               (0x58881688)
#define MKIMG_EXT_MAGIC           (0x58891689)

#define MKIMG_HDR_SZ              (0x200)
#define MKIMG_NAME_SZ             (32)

#define LEGACY_MKIMG_ALIGN_SZ     (4096)

/* image types */
#define IMG_TYPE_ID_OFFSET        (0)
#define IMG_TYPE_RESERVED0_OFFSET (8)
#define IMG_TYPE_RESERVED1_OFFSET (16)
#define IMG_TYPE_GROUP_OFFSET     (24)

#define IMG_TYPE_ID_MASK          (0xffU << IMG_TYPE_ID_OFFSET)
#define IMG_TYPE_RESERVED0_MASK   (0xffU << IMG_TYPE_RESERVED0_OFFSET)
#define IMG_TYPE_RESERVED1_MASK   (0xffU << IMG_TYPE_RESERVED1_OFFSET)
#define IMG_TYPE_GROUP_MASK       (0xffU << IMG_TYPE_GROUP_OFFSET)

#define IMG_TYPE_GROUP_AP         (0x00U << IMG_TYPE_GROUP_OFFSET)
#define IMG_TYPE_GROUP_MD         (0x01U << IMG_TYPE_GROUP_OFFSET)
#define IMG_TYPE_GROUP_CERT       (0x02U << IMG_TYPE_GROUP_OFFSET)

/* AP group */
#define IMG_TYPE_IMG_AP_BIN       (0x00 | IMG_TYPE_GROUP_AP)
#define IMG_TYPE_AND_VFY_BOOT_SIG (0x01 | IMG_TYPE_GROUP_AP)

/* MD group */
#define IMG_TYPE_IMG_MD_LTE       (0x00 | IMG_TYPE_GROUP_MD)
#define IMG_TYPE_IMG_MD_C2K       (0x01 | IMG_TYPE_GROUP_MD)

/* CERT group */
#define IMG_TYPE_CERT1            (0x00 | IMG_TYPE_GROUP_CERT)
#define IMG_TYPE_CERT1_MD         (0x01 | IMG_TYPE_GROUP_CERT)
#define IMG_TYPE_CERT2            (0x02 | IMG_TYPE_GROUP_CERT)

union mkimg_hdr {
	struct {
		uint32_t magic;
		uint32_t dsz;
		char name[MKIMG_NAME_SZ];
		uint32_t maddr;
		uint32_t mode;
		/* extension */
		uint32_t ext_magic;
		uint32_t hdr_sz;
		uint32_t hdr_ver;
		uint32_t img_type;
		uint32_t img_list_end;
		uint32_t align_sz;
		uint32_t dsz_extend;
		uint32_t maddr_extend;
		uint32_t scrambled;
	} info;
	uint8_t data[MKIMG_HDR_SZ];
};

#endif /* __MKIMG_H__ */

