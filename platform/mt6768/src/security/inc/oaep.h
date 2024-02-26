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

#ifndef __OAEP_H__
#define __OAEP_H__

#include "pal_typedefs.h"

#define OAEP_OK                       (0x0000)
#define OAEP_I2OSP_FAIL               (0x0001)
#define OAEP_MGF1_INVALID_SEED_BUF    (0x0002)
#define OAEP_MGF1_INVALID_SEED_BUF_SZ (0x0003)
#define OAEP_MGF1_INVALID_MASK_BUF    (0x0004)
#define OAEP_MGF1_INVALID_MASK_BUF_SZ (0x0005)
#define OAEP_MGF1_MALLOC_FAIL         (0x0006)
#define OAEP_MGF1_HASH_FAIL           (0x0007)
#define OAEP_INVALID_MSG_BUF          (0x0008)
#define OAEP_INVALID_MSG_BUF_SZ       (0x0009)
#define OAEP_INVALID_EMSG_BUF         (0x000a)
#define OAEP_INVALID_EMSG_BUF_SZ      (0x000b)
#define OAEP_MSG_TOO_LONG             (0x000c)
#define OAEP_INVALID_SEED_SZ          (0x000d)
#define OAEP_ZERO_HASH_CHK_FAIL       (0x000e)
#define OAEP_INVALID_FORMAT           (0x000f)

#define MGF1_COUNTER_SZ (4)

#define ATTR_SHA1       0x00000001
#define ATTR_SHA256     0x00000002

struct oaep_content {
	uint32_t rsa_mod_sz;
	uint32_t hash_sz;
	uint32_t seed_sz;
	uint32_t db_sz;
	uint32_t zero_hash[512];
	int (*hash_func)(const unsigned char *, unsigned long, unsigned char *);
};

unsigned int oaep_encode(unsigned char *msg,
							unsigned long msg_sz,
							unsigned char *encoded_msg,
							unsigned long encoded_msg_sz,
							struct oaep_content *oaep_info);

unsigned int oaep_decode(unsigned char *encoded_msg,
							unsigned long encoded_msg_sz,
							unsigned char *msg,
							unsigned long *msg_sz,
							struct oaep_content *oaep_info);

void *sec_calloc(unsigned int size);
unsigned int init_oaep_info(struct oaep_content *oaep_info, unsigned int attr);

#endif /* __OAEP_H__ */

