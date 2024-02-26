/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2019. All rights reserved.
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

#ifndef _SEC_POLICY_CONFIG_COMMON_H_
#define _SEC_POLICY_CONFIG_COMMON_H_

/*Policy Mask*/
#define DL_FORBIDDEN_BIT  0
#define VFY_BIT           1
#define HASH_BIND_BIT     2

#define CREATE_POLICY_ENTRY(bind, verify, dl) ((bind << HASH_BIND_BIT) | (verify << VFY_BIT) | (dl << DL_FORBIDDEN_BIT))
#define DL_FORBIDDEN_BIT_SET(policy) ((policy >> DL_FORBIDDEN_BIT) & 0x1)
#define VFY_BIT_SET(policy) ((policy >> VFY_BIT) & 0x1)
#define HASH_BIND_BIT_SET(policy) ((policy >> HASH_BIND_BIT) & 0x1)

#define PL_SW_ID          0
#define LK_SW_ID          0
#define LOGO_SW_ID        0
#define BOOT_SW_ID        0
#define SYSTEM_SW_ID      0
#define TEE1_SW_ID        0
#define TEE2_SW_ID        0
#define OEMKEYSTORE_SW_ID 0
#define KEYSTORE_SW_ID    0
#define USERDATA_SW_ID    0
#define MD1IMG_SW_ID      0
#define MD1DSP_SW_ID      0
#define MD1ARM7_SW_ID     0
#define MD3IMG_SW_ID      0


/*Image Binding Hash*/
#define PL_BINDING_HASH  "fdd62730afd983f367b267037d1668c164ab51568485ba305621cc28d6268d96"
#define LK_BINDING_HASH  0
#define LOGO_BINDING_HASH  0
#define BOOT_BINDING_HASH  0
#define SYSTEM_BINDING_HASH  0
#define TEE1_BINDING_HASH 0
#define TEE2_BINDING_HASH 0
#define OEMKEYSTORE_BINDING_HASH 0
#define KEYSTORE_BINDING_HASH 0
#define USERDATA_BINDING_HASH 0
#define MD1IMG_BINDING_HASH 0
#define MD1DSP_BINDING_HASH 0
#define MD1ARM7_BINDING_HASH 0
#define MD3IMG_BINDING_HASH 0

/*Image Binding Hash*/
#define  CUST1_BINDING_HASH  		0
#define  CUST2_BINDING_HASH 		0

/***********************************
CUSTOM IMG Config
************************************/
#define  CUST1_IMG_NAME        ""
#define  CUST2_IMG_NAME        ""

#endif
