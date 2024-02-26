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
#include "oaep.h"
#include "rsa2048_export.h"
#include "rsa_oaep.h"
#include "pal_log.h"
#include "verified_boot.h"
#include "verified_boot_error.h"

/* for debug */
#define DUMMY_OAEP (0)
#define DMMMY_RSA  (0)

static void rev_buf(unsigned char *buf, unsigned int buf_size)
{
	unsigned int i;
	unsigned int halfsz;
	unsigned char tmp;

	if ((NULL == buf) || (buf_size <= 1))
		return;

	halfsz = buf_size >> 1;
	for (i = 0; i < halfsz; i++) {
		tmp = buf[i];
		buf[i] = buf[buf_size - 1 - i];
		buf[buf_size - 1 - i] = tmp;
	}

	return;
}

static void dump_buf(unsigned char *buf, unsigned int buf_sz)
{
	unsigned int idx = 0;

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

/* encrypt with public key */
unsigned int rsa_oaep_encrypt(unsigned char *msg,
			      unsigned long msg_sz,
			      unsigned char *key_e,
			      unsigned long key_e_sz,
			      unsigned char *key_n,
			      unsigned long key_n_sz,
			      unsigned char *emsg,
			      unsigned long *emsg_sz,
			      unsigned int attr)
{
	unsigned int ret = RSA_OAEP_OK;
	int rsa_ret = 0;
	unsigned char *tmp_sig = NULL;
	struct oaep_content *oaep_info = NULL;

	oaep_info = (struct oaep_content *)sec_calloc(sizeof(struct oaep_content));
	if (NULL == oaep_info) {
		ret = ERR_VB_NOT_VALID_PTR;
		goto end;
	}

	ret = init_oaep_info(oaep_info, attr);
	if (ret)
		goto end;

	tmp_sig = (unsigned char *)sec_calloc(oaep_info->rsa_mod_sz);

	if (NULL == tmp_sig) {
		ret = ERR_VB_NOT_VALID_PTR;
		goto end;
	}

	if (NULL == msg) {
		ret = RSA_OAEP_INVALID_MSG_BUF;
		goto end;
	}

	if (0 == msg_sz) {
		ret = RSA_OAEP_INVALID_MSG_BUF_SZ;
		goto end;
	}

	if (NULL == key_e) {
		ret = RSA_OAEP_INVALID_KEY_E_BUF;
		goto end;
	}

	if (0 == key_e_sz) {
		ret = RSA_OAEP_INVALID_KEY_E_BUF_SZ;
		goto end;
	}

	if (NULL == key_n) {
		ret = RSA_OAEP_INVALID_KEY_N_BUF;
		goto end;
	}

	if (0 == key_n_sz) {
		ret = RSA_OAEP_INVALID_KEY_N_BUF_SZ;
		goto end;
	}

	if (NULL == emsg) {
		ret = RSA_OAEP_INVALID_EMSG_BUF;
		goto end;
	}

	if (NULL == emsg_sz) {
		ret = RSA_OAEP_INVALID_EMSG_BUF_SZ;
		goto end;
	}

	if (0 == *emsg_sz) {
		ret = RSA_OAEP_INVALID_EMSG_BUF_SZ;
		goto end;
	}

#if DUMMY_OAEP
	memset(tmp_sig, 0x0, oaep_info->rsa_mod_sz);
	memcpy(tmp_sig, msg, msg_sz);
#else
	ret = oaep_encode(msg, msg_sz, tmp_sig, oaep_info->rsa_mod_sz, oaep_info);
	if (ret)
		goto end;
#endif

#if DMMMY_RSA
	memset(emsg, 0x0, oaep_info->rsa_mod_sz);
	memcpy(emsg, tmp_sig, oaep_info->rsa_mod_sz);
#else
	rsa_ret = rsa_exptmod(tmp_sig, oaep_info->rsa_mod_sz,
						      emsg, emsg_sz,
							  key_e, key_e_sz,
							  key_n, key_n_sz);
	if (rsa_ret) {
		pal_log_err("[rsa_oeap] rsa_exptmod fail (0x%x)\n", rsa_ret);
		ret = RSA_OAEP_EXPTMOD_FAIL;
		goto end;
	}

#endif

end:
	sec_malloc_buf_reset();
	if (ret)
		pal_log_err("[rsa_oeap] encrypt fail (0x%x)\n", ret);

	return ret;
}

/* decrypt with private key */
unsigned int rsa_oaep_decrypt(unsigned char *emsg,
			      unsigned long emsg_sz,
			      unsigned char *key_d,
			      unsigned long key_d_sz,
			      unsigned char *key_n,
			      unsigned long key_n_sz,
			      unsigned char *msg,
			      unsigned long *msg_sz,
			      unsigned int attr)
{
	unsigned int ret = RSA_OAEP_OK;
	unsigned int rsa_ret = 0;
	unsigned char *tmp_sig = NULL;
	unsigned long final_msg_sz = 0;
	struct oaep_content *oaep_info = NULL;

	oaep_info = (struct oaep_content *)sec_calloc(sizeof(struct oaep_content));
	if (NULL == oaep_info) {
		ret = ERR_VB_NOT_VALID_PTR;
		goto end;
	}

	ret = init_oaep_info(oaep_info, attr);
	if (ret)
		goto end;

	tmp_sig = (unsigned char *)sec_calloc(oaep_info->rsa_mod_sz);

	if (NULL == tmp_sig) {
		ret = ERR_VB_NOT_VALID_PTR;
		goto end;
	}

	if (NULL == emsg) {
		ret = RSA_OAEP_INVALID_EMSG_BUF;
		goto end;
	}

	if (0 == emsg_sz) {
		ret = RSA_OAEP_INVALID_EMSG_BUF_SZ;
		goto end;
	}

	if (NULL == key_d) {
		ret = RSA_OAEP_INVALID_KEY_D_BUF;
		goto end;
	}

	if (0 == key_d_sz) {
		ret = RSA_OAEP_INVALID_KEY_D_BUF_SZ;
		goto end;
	}

	if (NULL == key_n) {
		ret = RSA_OAEP_INVALID_KEY_N_BUF;
		goto end;
	}

	if (0 == key_n_sz) {
		ret = RSA_OAEP_INVALID_KEY_N_BUF_SZ;
		goto end;
	}

	if (NULL == msg) {
		ret = RSA_OAEP_INVALID_MSG_BUF;
		goto end;
	}

	if (NULL == msg_sz) {
		ret = RSA_OAEP_INVALID_MSG_SZ_BUF;
		goto end;
	}
	final_msg_sz = *msg_sz;

#if DMMMY_RSA
	memset(tmp_sig, 0x0, oaep_info->rsa_mod_sz);
	memcpy(tmp_sig, emsg, emsg_sz);
#else
	rev_buf(emsg, emsg_sz);
	rev_buf(key_n, key_n_sz);
	rev_buf(key_d, key_d_sz);
	rsa_ret = seclib_RSA_2048_dec((uint32_t *)emsg, (uint32_t *)key_n,
					(uint32_t *)key_d, (uint32_t *)tmp_sig);
	if (rsa_ret) {
		pal_log_err("[rsa_oeap] rsa_exptmod fail (0x%x)\n", rsa_ret);
		ret = RSA_OAEP_EXPTMOD_FAIL;
		goto end;
	}
	rev_buf(tmp_sig, oaep_info->rsa_mod_sz);
#endif

#if DUMMY_OAEP
	memset(msg, 0x0, *msg_sz);
	memcpy(msg, tmp_sig, *msg_sz);
#else
	ret = oaep_decode(tmp_sig, oaep_info->rsa_mod_sz, msg, &final_msg_sz, oaep_info);
	if (ret)
		goto end;
#endif

	*msg_sz = final_msg_sz;
end:
	sec_malloc_buf_reset();

	if (ret)
		pal_log_err("[rsa_oeap] decrypt fail (0x%x)\n", ret);

	return ret;
}

