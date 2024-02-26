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
#include <pal_log.h>
#include <error.h>
#include <img_info.h>
#include <verified_boot_error.h>

uint32_t get_img_info(uint8_t *hdr_buf, uint32_t hdr_buf_sz, struct img_info *img_info)
{
	uint32_t ret = STATUS_OK;
	union mkimg_hdr *mkimg_hdr = (union mkimg_hdr *)hdr_buf;
	struct bootimg_hdr *bootimg_hdr = (struct bootimg_hdr *)hdr_buf;

	memset(img_info, 0x0, sizeof(struct img_info));

	if (MKIMG_MAGIC == mkimg_hdr->info.magic) {
		uint32_t align_sz = 0;
		img_info->img_hdr_type = IMG_HDR_TYPE_RAW;
		/* for raw image type, one header always store information for one image */
		if (MKIMG_EXT_MAGIC == mkimg_hdr->info.ext_magic) {
			/* we don't reference image header for its size for now */
			img_info->img_hdr_sz            = MKIMG_HDR_SZ;
			/* we don't reference image header version for parsing for now */
			/* it's fixed to 1 currently */
			img_info->img_hdr_ver        = 1;
			img_info->img_type           = mkimg_hdr->info.img_type;
			img_info->scrambled          = mkimg_hdr->info.scrambled;
			img_info->img_load_mode      = mkimg_hdr->info.mode;
			img_info->img_load_addr      = mkimg_hdr->info.maddr;
			img_info->img_load_addr_high = mkimg_hdr->info.maddr_extend;
			img_info->img_sz_high        = mkimg_hdr->info.dsz_extend;

			/* may cause overflow if img_sz overflow occurs after padding */
			/* image with size 0xffffffff00000000 ~ 0xffffffffffffffff is
			   also not reasonable */
			if (img_info->img_sz_high == 0xffffffff)
				return ERR_VB_NOT_VALID_IMG_SZ;

			align_sz = mkimg_hdr->info.align_sz;
			if (align_sz > MAX_ALIGN_SZ)
				return ERR_VB_NOT_VALID_IMG_ALIGN_SZ;
		} else {
			img_info->img_hdr_sz         = MKIMG_HDR_SZ;
			img_info->img_hdr_ver        = 0;
			img_info->img_type           = IMG_TYPE_IMG_AP_BIN;
			img_info->scrambled          = 0;
			img_info->img_load_mode      = mkimg_hdr->info.mode;
			img_info->img_load_addr      = mkimg_hdr->info.maddr;
			img_info->img_load_addr_high = 0;
			img_info->img_sz_high        = 0;
			align_sz                     = LEGACY_MKIMG_ALIGN_SZ;
	    }

		/* dsz does not include image header nor padding */
		/* overflow handling, it is possible that image size is larger than 4GB */
		if (img_info->img_sz > (0xffffffff - (align_sz - 1)))
			img_info->img_sz_high++;
		img_info->img_dsz = mkimg_hdr->info.dsz;
		img_info->img_sz = ROUND_UP(mkimg_hdr->info.dsz, align_sz);
		memset(img_info->name, 0x0, MAX_NAME_SZ);
		memcpy(img_info->name, mkimg_hdr->info.name, MKIMG_NAME_SZ);

		return ret;
	}

#if LOAD_BOOTIMG_SUPPORT
	if (!memcmp(bootimg_hdr->magic, BOOTIMG_MAGIC, strlen(BOOTIMG_MAGIC))) {
		uint32_t page_sz = bootimg_hdr->page_sz;
		if (page_sz > BOOTIMG_HDR_SZ)
			return ERR_VB_NOT_VALID_PAGE_SZ;

		img_info->img_hdr_type = IMG_HDR_TYPE_BOOT;
		img_info->img_hdr_sz   = page_sz;
		img_info->img_hdr_ver  = 0;
		img_info->img_type     = IMG_TYPE_IMG_AP_BIN;
		img_info->scrambled    = 0;

		/* image load info for kernel relocation */
		img_info->sub_img_load_mode[KENREL_SLOT]      = DEFAULT_LOAD_MODE;
		img_info->sub_img_load_addr[KENREL_SLOT]      = bootimg_hdr->kernel_addr;
		img_info->sub_img_load_addr_high[KENREL_SLOT] = 0;
		img_info->sub_img_sz[KENREL_SLOT]             = ROUND_UP(bootimg_hdr->kernel_sz, page_sz);
		img_info->sub_img_sz_high[KENREL_SLOT]        = 0;

		/* image load info for ramdisk relocation */
		img_info->sub_img_load_mode[RAMDISK_SLOT]      = DEFAULT_LOAD_MODE;
		img_info->sub_img_load_addr[RAMDISK_SLOT]      = bootimg_hdr->ramdisk_addr;
		img_info->sub_img_load_addr_high[RAMDISK_SLOT] = 0;
		img_info->sub_img_sz[RAMDISK_SLOT]             = ROUND_UP(bootimg_hdr->ramdisk_sz, page_sz);
		img_info->sub_img_sz_high[RAMDISK_SLOT]        = 0;

		/* image load info for second stage relocation */
		img_info->sub_img_load_mode[SECOND_STAGE_SLOT]      = DEFAULT_LOAD_MODE;
		img_info->sub_img_load_addr[SECOND_STAGE_SLOT]      = bootimg_hdr->second_addr;
		img_info->sub_img_load_addr_high[SECOND_STAGE_SLOT] = 0;
		img_info->sub_img_sz[SECOND_STAGE_SLOT]             = ROUND_UP(bootimg_hdr->second_sz, page_sz);
		img_info->sub_img_sz_high[SECOND_STAGE_SLOT]        = 0;

		/* image load info for boot image loading */
		img_info->img_load_mode      = DEFAULT_LOAD_MODE;
		img_info->img_load_addr      = EXTERNAL_LOAD_ADDR;
		img_info->img_load_addr_high = 0x0;
		img_info->img_sz = img_info->sub_img_sz[KENREL_SLOT] + \
							img_info->sub_img_sz[RAMDISK_SLOT] + \
							img_info->sub_img_sz[SECOND_STAGE_SLOT];
		img_info->img_sz_high = 0;
		img_info->img_dsz = img_info->img_sz;
		img_info->img_dsz_high = img_info->img_sz_high;

		memset(img_info->img_name, 0x0, MAX_NAME_SZ);
		memcpy(img_info->img_name, bootimg_hdr->name, BOOTIMG_NAME_SZ);

		return ret;
	}
#endif

	/* should not reach here, image type not suppoted */
	/* other fields are already filled with 0 */
	img_info->img_hdr_type = IMG_HDR_TYPE_UNKNOWN;
	ret = ERR_VB_IMG_TYPE_INVALID;

	return ret;
}

