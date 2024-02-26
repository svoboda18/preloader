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

#ifndef __IMG_INFO_H__
#define __IMG_INFO_H__

#include <pal_typedefs.h>
#include <bootimg.h>
#include <mkimg.h>

/* image header types */
#define IMG_HDR_TYPE_UNKNOWN     (0)
#define IMG_HDR_TYPE_RAW         (1)
#define IMG_HDR_TYPE_BOOT        (2)

#define MAX_NAME_SZ (MKIMG_NAME_SZ > BOOTIMG_NAME_SZ ? MKIMG_NAME_SZ : BOOTIMG_NAME_SZ)
#define MAX_HDR_SZ  (MKIMG_HDR_SZ > BOOTIMG_HDR_SZ ? MKIMG_HDR_SZ : BOOTIMG_HDR_SZ)

#define MAX_ALIGN_SZ             (LEGACY_MKIMG_ALIGN_SZ)

#define KENREL_SLOT              (0)
#define RAMDISK_SLOT             (1)
#define SECOND_STAGE_SLOT        (2)
#define MAX_NUM_IMG_WITH_ONE_HDR (3)

#define RSA2048_SIG_SZ           (256)

#define ROUND_UP(x, align)       (((x + align - 1) / align) * align)
#define ROUND_DOWN(x, align)     ((x / align) * align)

#define DEFAULT_LOAD_ADDR        (0xffffffff)
#define EXTERNAL_LOAD_ADDR       DEFAULT_LOAD_ADDR

#define DEFAULT_LOAD_MODE        (0xffffffff)
#define TEE_LOAD_MODE            (0x00000000)

/* for boot image, load address of kernel/ramdisk are different, hence we have different
   load address and image size for them */

struct img_info {
	uint32_t img_hdr_type;
	uint32_t img_hdr_sz;
	uint32_t img_hdr_ver;
	uint32_t img_type;
	uint32_t img_load_mode;
	uint32_t img_load_addr;
	uint32_t img_load_addr_high;
	uint32_t img_dsz;      /* image size without padding */
	uint32_t img_dsz_high; /* image size without padding */
	uint32_t img_sz;       /* image size with padding */
	uint32_t img_sz_high;  /* image size with padding */
	uint32_t scrambled;
	/* the following information are used for relocation */
	uint32_t sub_img_load_mode[MAX_NUM_IMG_WITH_ONE_HDR];
	uint32_t sub_img_load_addr[MAX_NUM_IMG_WITH_ONE_HDR];
	uint32_t sub_img_load_addr_high[MAX_NUM_IMG_WITH_ONE_HDR];
	uint32_t sub_img_sz[MAX_NUM_IMG_WITH_ONE_HDR];
	uint32_t sub_img_sz_high[MAX_NUM_IMG_WITH_ONE_HDR];
	char     name[MAX_NAME_SZ];
};

uint32_t get_img_info(uint8_t *hdr_buf, uint32_t hdr_buf_sz, struct img_info *img_info);

#endif /* __IMG_INFO_H__ */

