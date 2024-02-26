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
 * MediaTek Inc. (C) 2018. All rights reserved.
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

#include <pal_typedefs.h>
#include <pal_log.h>
#include <hmac.h>
#include <sha256_public.h>
#include <verified_boot_error.h>

static struct hmac_op_metadata hmac_metadata;

/*
 * hmac_sha256: calculate HMAC with sepcified key and msg.
 * @key: key for HMAC
 * @key_sz: key size for HMAC
 * @msg: message for HMAC
 * @msg_len: message len for HMAC
 * @mac: hmac result, 32 bytes long because hash algorithm is fixed to sha256
 * return 0 for success, otherwise negative status code.
 */
uint32_t hmac_sha256(uint8_t *key, uint32_t key_sz,
		    uint8_t *msg, uint32_t msg_len, uint8_t *mac)
{
	uint32_t ret = STATUS_OK;
	int i = 0;

	if (key == NULL)
		return ERR_VB_CRYPTO_HMAC_KEY_PTR_INVALID;

	if (key_sz > RPMB_KEY_SIZE)
		return ERR_VB_CRYPTO_HMAC_KEY_LEN_INVALID;

	if (msg == NULL)
		return ERR_VB_CRYPTO_HMAC_MSG_PTR_INVALID;

	if (msg_len > MSG_MAX_SIZE)
		return ERR_VB_CRYPTO_HMAC_MSG_LEN_INVALID;

	if (msg_len == 0)
		return ERR_VB_CRYPTO_HMAC_MSG_LEN_INVALID;

	if (mac == NULL)
		return ERR_VB_CRYPTO_HMAC_MAC_PTR_INVALID;

	memset(&hmac_metadata, 0x0, sizeof(struct hmac_op_metadata));
	memcpy(hmac_metadata.msg, msg, msg_len);

	memset(hmac_metadata.i_key_pad, 0x36, O_KEY_PAD_SIZE);
	memset(hmac_metadata.o_key_pad, 0x5C, I_KEY_PAD_SIZE);

	/* Xor inner/outer key padding with input key */
	for (i = 0; i < key_sz; i++) {
		hmac_metadata.o_key_pad[i] ^= key[i];
		hmac_metadata.i_key_pad[i] ^= key[i];
	}

	/* hash: first pass */
	ret = sha256(hmac_metadata.i_key_pad,
		     (msg_len + I_KEY_PAD_SIZE),
		     hmac_metadata.hash1);
	if (ret)
		return ERR_VB_CRYPTO_HMAC_HASH_FAIL;

	/* hash: second pass */
	ret = sha256(hmac_metadata.o_key_pad,
		     (O_KEY_PAD_SIZE + SHA256_SIZE),
		     hmac_metadata.hash2);
	if (ret)
		return ERR_VB_CRYPTO_HMAC_HASH_FAIL;

	/* copy final hash to output mac */
	memcpy(mac, hmac_metadata.hash2, SHA256_SIZE);

	return STATUS_OK;
}

