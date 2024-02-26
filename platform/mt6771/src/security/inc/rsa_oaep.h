/* Copyright Statement:
*
* This software/firmware and related documentation ("MediaTek Software") are
* protected under relevant copyright laws. The information contained herein
* is confidential and proprietary to MediaTek Inc. and/or its licensors.
* Without the prior written permission of MediaTek inc. and/or its licensors,
* any reproduction, modification, use or disclosure of MediaTek Software,
* and information contained herein, in whole or in part, shall be strictly prohibited.
*/
/* MediaTek Inc. (C) 2015. All rights reserved.
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
*/

#ifndef __RSA_OAEP_H__
#define __RSA_OAEP_H__

#define RSA_OAEP_OK                   (0x0000)
#define RSA_OAEP_INVALID_MSG_BUF      (0x0001)
#define RSA_OAEP_INVALID_MSG_BUF_SZ   (0x0002)
#define RSA_OAEP_INVALID_EMSG_BUF     (0x0003)
#define RSA_OAEP_INVALID_EMSG_BUF_SZ  (0x0004)
#define RSA_OAEP_INVALID_KEY_E_BUF    (0x0005)
#define RSA_OAEP_INVALID_KEY_E_BUF_SZ (0x0006)
#define RSA_OAEP_INVALID_KEY_N_BUF    (0x0007)
#define RSA_OAEP_INVALID_KEY_N_BUF_SZ (0x0008)
#define RSA_OAEP_INVALID_KEY_D_BUF    (0x0009)
#define RSA_OAEP_INVALID_KEY_D_BUF_SZ (0x000a)
#define RSA_OAEP_EXPTMOD_FAIL         (0x000b)
#define RSA_OAEP_INVALID_MSG_SZ_BUF   (0x000c)


unsigned int rsa_oaep_encrypt(unsigned char *msg,
			      unsigned long msg_sz,
			      unsigned char *key_e,
			      unsigned long key_e_sz,
			      unsigned char *key_n,
			      unsigned long key_n_sz,
			      unsigned char *emsg,
			      unsigned long *emsg_sz,
			      unsigned int attr);

unsigned int rsa_oaep_decrypt(unsigned char *emsg,
			      unsigned long emsg_sz,
			      unsigned char *key_d,
			      unsigned long key_d_sz,
			      unsigned char *key_n,
			      unsigned long key_n_sz,
			      unsigned char *msg,
			      unsigned long *msg_sz,
			      unsigned int attr);
#endif /* __RSA_OAEP_H__ */

