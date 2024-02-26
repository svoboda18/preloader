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
#include <partition_api.h>
#include <loader.h>
#include <verified_boot_error.h>

/* load_addr and load_addr_high are used as default load address if load address value
   in image header is "refer to external setting" */
uint32_t loader(struct img_load_config *img_load_config,
		struct img_info *img_info)
{
	uint32_t ret = STATUS_OK;
	uint8_t *buf = (uint8_t *)get_vb_buf();
	uint32_t buf_sz = (uint32_t)get_vb_buf_sz();
	uint32_t found = 0;
	uint64_t offset = 0;
	uint32_t name_compare_len;
	uint32_t max_name_len;
	uint32_t retry = 0;
	ssize_t len = 0;

	if (NULL == buf)
		return ERR_VB_BUF_ADDR_INVALID;

	if (NULL == img_load_config)
		return ERR_VB_BUF_ADDR_INVALID;

	if (img_load_config->num_load_addr > MAX_NUM_IMG_WITH_ONE_HDR)
		return ERR_VB_BUF_OVERFLOW;

	if (MAX_HDR_SZ > buf_sz)
		return ERR_VB_BUF_OVERFLOW;

	memset(buf, 0x0, MAX_HDR_SZ);

	while (0 == found) {
		if (NULL == img_load_config->part_name) {
			ret = ERR_VB_NOT_VALID_STRING;
			break;
		}

		len = partition_read(img_load_config->part_name, offset, buf, MAX_HDR_SZ);
		if (len < 0) {
			ret = (uint32_t)len;
			break;
		}

		ret = get_img_info(buf, MAX_HDR_SZ, img_info);
		if (ret) {
			if (retry == 1) {
				/* retried but in vain */
				break;
			} else {
				/* there might be a rsa2048 signature here,
				   retry with offset + rsa2048 sig size */
				offset += RSA2048_SIG_SZ;
				retry = 1;
				continue;
			}
		}

		/* reset retry flag since there might be multiple subimages in
		   current partition and each of them may include rsa2048 signature */
		retry = 0;

		/* image name is not given, once valid image header is found,
		   continue image loading */
		if (NULL == img_load_config->img_name) {
			found = 1;
			break;
		}

		name_compare_len = strlen(img_load_config->img_name);
		if (img_info->img_hdr_type == IMG_HDR_TYPE_RAW)
			max_name_len = MKIMG_NAME_SZ - 1;
		else if (img_info->img_hdr_type == IMG_HDR_TYPE_BOOT)
			max_name_len = BOOTIMG_NAME_SZ - 1;
		else {
			max_name_len = 0;
			ret = ERR_VB_INVALID_IMG_HDR;
			break;
		}

		if (name_compare_len > max_name_len) {
			ret = ERR_VB_NOT_VALID_STRING;
			break;
		}

		if (!memcmp(img_load_config->img_name, img_info->name, name_compare_len)) {
			found = 1;
			break;
		}

		offset += img_info->img_hdr_sz;
		offset += img_info->img_sz + ((((uint64_t)img_info->img_sz_high & 0xffffffff))
					      << 32);
	}

	if (!found)
		ret = ERR_VB_IMG_NOT_FOUND;

	else {
		uint32_t vfy_en = 0;
		vfy_en = get_vfy_policy_by_name(img_load_config->part_name,
						img_load_config->img_name);
		if (vfy_en) {
			ret = vfy_phase1(img_load_config->part_name, offset, img_info);
			if (ret) {
				pal_log_err("vfy_phase1 failed(0x%x)\n", ret);
				return ret;
			}
			pal_log_err("vfy_phase1 ok\n");
		}

		/* load image */
		/* 64 bit loading address is not supported now  */
		if (EXTERNAL_LOAD_ADDR == img_info->img_load_addr)
			img_info->img_load_addr = img_load_config->load_addr;

		if (TEE_LOAD_MODE == img_info->img_load_mode)
			img_info->img_load_addr = tee_get_load_addr(img_info->img_load_addr);

		/* load address range check  */
		if (img_info->img_load_addr == 0) {
			ret = ERR_VB_INVALID_LOAD_ADDR;
			return ret;
		}

		len = partition_read(img_load_config->part_name, offset + img_info->img_hdr_sz,
				     (uint8_t *)img_info->img_load_addr, img_info->img_dsz);
		if (len < 0) {
			ret = (uint32_t)len;
			return ret;
		}

		/* do padding */
		if (img_info->img_sz != img_info->img_dsz)
			memset((uint8_t *)img_info->img_load_addr + img_info->img_dsz, 0x0,
			       (img_info->img_sz - img_info->img_dsz) * sizeof(uint8_t));

		if (vfy_en) {
			ret = vfy_phase2(img_info->img_load_addr, img_info->img_sz);
			if (ret) {
				pal_log_err("vfy_phase2 failed(0x%x)\n", ret);
				return ret;
			}
			pal_log_err("vfy_phase2 ok\n");
		}

		/* postprocessing */
		if (img_info->scrambled) {
			/* [TODO] restore */
		}

#if LOAD_BOOTIMG_SUPPORT
		if (img_info->img_hdr_type == IMG_HDR_TYPE_BOOT) {
			/* relocate ramdisk and second stage */
			uint32_t relocate_src = 0;
			uint32_t relocate_target = 0;
			uint32_t relocate_sz = 0;

			/* boot image header -> kernel -> ramdisk -> second stage  */

			/* relocate ramdisk */
			relocate_src = img_info->img_load_addr + \
				       img_info->img_hdr_sz + \
				       img_info->sub_img_sz[KENREL_SLOT];
			relocate_target = img_info->sub_img_load_addr[RAMDISK_SLOT];
			relocate_sz = img_info->sub_img_sz[RAMDISK_SLOT];
			if (relocate_sz)
				memcpy(relocate_target, relocate_src, relocate_sz);

			/* relocate second stage */
			relocate_src = img_info->img_load_addr + \
				       img_info->img_hdr_sz + \
				       img_info->sub_img_sz[KENREL_SLOT] + \
				       img_info->sub_img_sz[RAMDISK_SLOT];
			relocate_target = img_info->sub_img_load_addr[SECOND_STAGE_SLOT];
			relocate_sz = img_info->sub_img_sz[SECOND_STAGE_SLOT];
			if (relocate_sz)
				memcpy(relocate_target, relocate_src, relocate_sz);

			/* [TODO] decompress kernel if needed */
			/* [TODO] handle device tree */
		}
#endif
	}

	return ret;
}

