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

#ifndef __VERIFIED_BOOT_H__
#define __VERIFIED_BOOT_H__

#include <pal_typedefs.h>
#include <img_info.h>

#define SHA256_HASH_SZ (32)

uint8_t *get_vb_buf(void);
uint32_t get_vb_buf_sz(void);
uint32_t get_vfy_policy_by_name(char *part_name, char *img_name);
uint32_t vfy_phase1(char *part_name, uint64_t offset, struct img_info *img_info);
uint32_t vfy_phase2(uint32_t load_addr, uint32_t sz);
uint32_t tee_get_load_addr(uint32_t load_addr);
void set_allow_rollback_count(uint32_t count);
uint32_t seclib_get_pl_pubk_hash(uint8_t *hash, uint32_t hash_sz);
void seclib_AES256CBC_dec(uint8_t *puc_key, uint8_t *puc_init_vec, uint8_t *puc_inp, uint8_t *puc_out, int sz);
uint32_t seclib_get_prov_key(uint8_t *prov_key, uint32_t len);
uint32_t seclib_RSA_2048_dec(uint32_t *in, uint32_t *modulus, uint32_t *d, uint32_t *result);
void sec_malloc_buf_reset(void);
uint32_t calc_pubk_hash(uint8_t *buf_n, uint32_t n_sz, uint8_t *hash);
uint32_t get_preloader_hash(uint8_t *pl_hash, uint32_t debug_en);
uint32_t sec_get_image_hash(uint8_t *buf, uint32_t buf_sz,
					uint8_t *img_hash, uint32_t debug_en);

#endif /* __VERIFIED_BOOT_H__ */

