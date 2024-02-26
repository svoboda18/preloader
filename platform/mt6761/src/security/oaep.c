/* Copyright Statement:
*
* This software/firmware and related documentation ("MediaTek Software") are
* protected under relevant copyright laws. The information contained herein
* is confidential and proprietary to MediaTek Inc. and/or its licensors.
* Without the prior written permission of MediaTek inc. and/or its licensors,
* any reproduction, modification, use or disclosure of MediaTek Software,
* and information contained herein, in whole or in part, shall be strictly prohibited.
*/
/* MediaTek Inc. (C) 2018. All rights reserved.
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

#include <string.h>
#include "pal_typedefs.h"
#include "oaep.h"
#include "rnd.h"
#include "hash_export.h"
#include "rsa2048_export.h"
#include "pal_log.h"
#include "verified_boot_error.h"

/* used OAEP prefix to avoid name collision */
#define OAEP_MIN(a, b) ((a) < (b) ? (a) : (b))

const char sha1_zero_hash[SHA1_HASH_SZ] = {
    0xda, 0x39, 0xa3, 0xee, 0x5e, 0x6b, 0x4b, 0x0d,
    0x32, 0x55, 0xbf, 0xef, 0x95, 0x60, 0x18, 0x90,
    0xaf, 0xd8, 0x07, 0x09
};

const char sha256_zero_hash[SHA256_HASH_SZ] = {
    0xe3, 0xb0, 0xc4, 0x42, 0x98, 0xfc, 0x1c, 0x14,
    0x9a, 0xfb, 0xf4, 0xc8, 0x99, 0x6f, 0xb9, 0x24,
    0x27, 0xae, 0x41, 0xe4, 0x64, 0x9b, 0x93, 0x4c,
    0xa4, 0x95, 0x99, 0x1b, 0x78, 0x52, 0xb8, 0x55
};

/* for debug purpose */
static void dump_buf(unsigned char *buf, unsigned int buf_sz)
{
	unsigned int idx = 0;

	if (NULL == buf)
		return;

	if (buf_sz < 4)
		return;

	for (idx = 0; idx <= buf_sz - 4; idx += 4) {
		pal_log_err("[0x%x] 0x%x 0x%x 0x%x 0x%x\n", idx,
								*(buf + idx),
								*(buf + idx + 1),
								*(buf + idx + 2),
								*(buf + idx + 3));
	}

	return;
}

static unsigned int i2osp(unsigned int integer,
							unsigned char *osp,
							unsigned int osp_sz)
{
	unsigned int ret = OAEP_OK;
	int idx = 0;

	memset(osp, 0x0, osp_sz);
	if (osp_sz < 1)
		return OAEP_I2OSP_FAIL;

	idx = osp_sz - 1;

	while (integer != 0) {
		osp[idx] = integer % 0x100;
		integer /= 0x100;
		idx--;
		if (idx < 0) {
			ret = OAEP_I2OSP_FAIL;
			break;
		}
	}

	return ret;
}

static unsigned int mgf1(unsigned char *seed,
							unsigned int seed_sz,
							unsigned char *mask,
							unsigned int mask_sz,
							struct oaep_content *oaep_info)
{
	unsigned int ret = OAEP_OK;
	unsigned int counter = 0;
	unsigned int num_round = 0;
	unsigned char *input = NULL;
	unsigned int input_sz = 0;
	unsigned char *hash = NULL;

	if (NULL == oaep_info)
		return ERR_VB_NOT_VALID_PTR;

	hash = (unsigned char*)sec_calloc(oaep_info->hash_sz);

	if (NULL == hash)
		return OAEP_MGF1_MALLOC_FAIL;

	if (NULL == seed)
		return OAEP_MGF1_INVALID_SEED_BUF;

	if (0 == seed_sz)
		return OAEP_MGF1_INVALID_SEED_BUF_SZ;

	if (NULL == mask)
		return OAEP_MGF1_INVALID_MASK_BUF;

	if (0 == mask_sz)
		return OAEP_MGF1_INVALID_MASK_BUF_SZ;

	input_sz = seed_sz + MGF1_COUNTER_SZ;
	input = (unsigned char *)malloc(input_sz);
	if (NULL == input) {
		ret = OAEP_MGF1_MALLOC_FAIL;
		return ret;
	}

	memcpy(input, seed, seed_sz * sizeof(char));

	if (oaep_info->hash_sz <= 0) {
		ret = ERR_VB_NOT_VALID_VALUE;
		goto end;
	}

	num_round = (mask_sz + oaep_info->hash_sz - 1) / oaep_info->hash_sz;
	for (counter = 0; counter < num_round; counter++) {
		int hash_ret = 0;
		ret = i2osp(counter, input + seed_sz, MGF1_COUNTER_SZ);
		if (ret)
			goto end;

		if (NULL == oaep_info->hash_func) {
			ret = ERR_VB_NOT_VALID_PTR;
			goto end;
		}

		hash_ret = oaep_info->hash_func(input, input_sz, hash);
		if (hash_ret) {
			ret = OAEP_MGF1_HASH_FAIL;
			goto end;
		}
		memcpy(mask + counter * oaep_info->hash_sz,
				hash,
				OAEP_MIN(mask_sz - counter * oaep_info->hash_sz,
				oaep_info->hash_sz));
	}

end:
	if (NULL != input)
		free(input);

	return ret;
}

unsigned int init_oaep_info(struct oaep_content *oaep_info, unsigned int attr)
{
	unsigned int ret = 0;

	if (NULL == oaep_info)
		return ERR_VB_NOT_VALID_PTR;

	/* rsa_mod_sz is set to 256 by default and can be extended to
	 * 384 or 512 by controlling attr variable in the future.
	 */
	oaep_info->rsa_mod_sz = RSA_MODULUS_SZ;

	if (attr & ATTR_SHA1) {
		oaep_info->hash_sz = SHA1_HASH_SZ;
		oaep_info->seed_sz = oaep_info->hash_sz;
		oaep_info->db_sz = oaep_info->rsa_mod_sz - 1 - oaep_info->hash_sz;
		memcpy(oaep_info->zero_hash, sha1_zero_hash, oaep_info->hash_sz);
		oaep_info->hash_func = sha1;
	} else {
		oaep_info->hash_sz = SHA256_HASH_SZ;
		oaep_info->seed_sz = oaep_info->hash_sz;
		oaep_info->db_sz = oaep_info->rsa_mod_sz - 1 - oaep_info->hash_sz;
		memcpy(oaep_info->zero_hash, sha256_zero_hash, oaep_info->hash_sz);
		oaep_info->hash_func = sha256;
	}

	return ret;
}

unsigned int oaep_encode(unsigned char *msg,
							unsigned long msg_sz,
							unsigned char *encoded_msg,
							unsigned long encoded_msg_sz,
							struct oaep_content *oaep_info)
{
	unsigned int ret = OAEP_OK;
	unsigned char * const masked_seed = encoded_msg + 1;
	unsigned char * const masked_db   = encoded_msg + 1 + oaep_info->seed_sz;
	/* temp data */
	unsigned char *seed_mask = NULL;
	unsigned char *db_mask = NULL;
	unsigned int idx = 0;

	seed_mask = (unsigned char *)sec_calloc(oaep_info->seed_sz);
	db_mask = (unsigned char *)sec_calloc(oaep_info->db_sz);

	if (NULL == seed_mask || NULL == db_mask) {
		ret = ERR_VB_NOT_VALID_PTR;
		goto end;
	}

	if (NULL == msg) {
		ret = OAEP_INVALID_MSG_BUF;
		goto end;
	}

	if (0 == msg_sz) {
		ret =  OAEP_INVALID_MSG_BUF_SZ;
		goto end;
	}

	if (NULL == encoded_msg) {
		ret = OAEP_INVALID_EMSG_BUF;
		goto end;
	}

	if (0 == encoded_msg_sz) {
		ret = OAEP_INVALID_EMSG_BUF_SZ;
		goto end;
	}

	if (oaep_info->rsa_mod_sz != encoded_msg_sz) {
		ret = OAEP_INVALID_EMSG_BUF_SZ;
		goto end;
	}

	if (msg_sz > oaep_info->db_sz - oaep_info->seed_sz - 1) {
		ret = OAEP_MSG_TOO_LONG;
		goto end;
	}

	memset(encoded_msg, 0x0, encoded_msg_sz);

	/* get seed from random number generator */
	if ((oaep_info->seed_sz % sizeof(unsigned int)) != 0) {
		ret = OAEP_INVALID_SEED_SZ;
		goto end;
	}

	for (idx = 0; idx < oaep_info->seed_sz; idx += 4) {
		unsigned int rnd_val = 0;
		ret = plat_get_rnd(&rnd_val);
		if (ret)
			goto end;
		memcpy(masked_seed + idx, &rnd_val, sizeof(unsigned int));
	}

	/* store db in masked_db, mask not applied yet */
	memcpy(masked_db, oaep_info->zero_hash, oaep_info->hash_sz);
	masked_db[oaep_info->db_sz - msg_sz - 1] = 0x1;
	memcpy(masked_db + oaep_info->db_sz - msg_sz, msg, msg_sz);

	/* create db_mask */
	ret = mgf1(masked_seed, oaep_info->seed_sz, db_mask,
				oaep_info->db_sz, oaep_info);
	if (ret)
		goto end;

	/* apply db_mask to create masked_db */
	for (idx = 0; idx < oaep_info->db_sz; idx++)
		masked_db[idx] ^= db_mask[idx];

	/* create seed_mask */
	ret = mgf1(masked_db, oaep_info->db_sz, seed_mask,
				oaep_info->seed_sz, oaep_info);
	if (ret)
		goto end;

	/* apply seed_mask to create masked_seed */
	for (idx = 0; idx < oaep_info->seed_sz; idx++)
		masked_seed[idx] ^= seed_mask[idx];

end:
	if (ret)
		pal_log_err("[oaep] encode fail (0x%x)\n", ret);

	return ret;
}

unsigned int oaep_decode(unsigned char *encoded_msg,
							unsigned long encoded_msg_sz,
							unsigned char *msg,
							unsigned long *msg_sz,
							struct oaep_content *oaep_info)
{
	unsigned int ret = OAEP_OK;
	unsigned char * const masked_seed = encoded_msg + 1;
	unsigned char * const masked_db   = encoded_msg + 1 + oaep_info->seed_sz;
	/* temp data */
	unsigned char *seed_mask = NULL;
	unsigned char *db_mask = NULL;
	unsigned int idx = 0;
	seed_mask = (unsigned char *)sec_calloc(oaep_info->seed_sz);
	db_mask = (unsigned char *)sec_calloc(oaep_info->db_sz);

	if (NULL == seed_mask || NULL == db_mask) {
		ret = ERR_VB_NOT_VALID_PTR;
		goto end;
	}

	if (NULL == msg) {
		ret = OAEP_INVALID_MSG_BUF;
		goto end;
	}

	if (NULL == msg_sz) {
		ret = OAEP_INVALID_MSG_BUF_SZ;
		goto end;
	}

	if (0 == *msg_sz) {
		ret = OAEP_INVALID_MSG_BUF_SZ;
		goto end;
	}

	if (NULL == encoded_msg) {
		ret = OAEP_INVALID_EMSG_BUF;
		goto end;
	}

	if (0 == encoded_msg_sz) {
		ret = OAEP_INVALID_EMSG_BUF_SZ;
		goto end;
	}

	if (RSA_MODULUS_SZ != encoded_msg_sz) {
		ret = OAEP_INVALID_EMSG_BUF_SZ;
		goto end;
	}

	memset(msg, 0x0, *msg_sz);

	/* recover seed_mask */
	ret = mgf1(masked_db, oaep_info->db_sz, seed_mask,
				oaep_info->seed_sz, oaep_info);
	if (ret)
		goto end;

	/* recover seed */
	/* after this step, data in masked_seed is actually seed */
	for (idx = 0; idx < oaep_info->seed_sz; idx++)
		masked_seed[idx] ^= seed_mask[idx];

	/* recover db_mask */
	ret = mgf1(masked_seed, oaep_info->seed_sz, db_mask,
				oaep_info->db_sz, oaep_info);
	if (ret)
		goto end;

	/* recover db */
	/* after this step, data in masked_db is actually db */
	for (idx = 0; idx < oaep_info->db_sz; idx++)
		masked_db[idx] ^= db_mask[idx];

	/* check against zero hash */
	if (0 != memcmp(masked_db, oaep_info->zero_hash, oaep_info->hash_sz)) {
		ret = OAEP_ZERO_HASH_CHK_FAIL;
		pal_log_err("[oaep] zero hash compare fail\n");
		pal_log_err("[oaep] zero hash:\n");
		dump_buf((unsigned char *)oaep_info->zero_hash, oaep_info->hash_sz);

		pal_log_err("[oaep] db:\n");
		dump_buf(masked_db, oaep_info->db_sz);

		goto end;
	}

	for (idx = oaep_info->hash_sz; idx < oaep_info->db_sz; idx++) {
		if (masked_db[idx] == 0)
			continue;
		else if (masked_db[idx] == 1)
			break;
		else {
			ret = OAEP_INVALID_FORMAT;
			goto end;
		}
	}

	/* msg starts at idx + 1 */
	if (oaep_info->db_sz - idx - 1 > *msg_sz) {
		ret = OAEP_INVALID_MSG_BUF_SZ;
		goto end;
	}

	*msg_sz = oaep_info->db_sz - idx - 1;
	memcpy(msg, masked_db + idx + 1, *msg_sz);

end:
	if (ret)
		pal_log_err("[oaep] decode fail (0x%x)\n", ret);

	return ret;

}

