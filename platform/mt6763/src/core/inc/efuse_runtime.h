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

#ifndef __EFUSE_RUNTIME_H__
#define __EFUSE_RUNTIME_H__

/******************************************************************************
 * EFUSE RUN-TIME BLOWING INDEX DEFINITIONS
 ******************************************************************************/
/*
 * NOTE: C_DATx is Multi-Bit ECC field and can only be blown once. If you blow
 *       it more than one time. Error code S_EFUSE_MULTI_ECC_FIELD_BLOWN_TWICE
 *       will be returned.
 */
#define EFUSE_IDX_C_DAT0            0
#define EFUSE_IDX_C_DAT1            1
#define EFUSE_IDX_C_DAT2            2
#define EFUSE_IDX_C_DAT3            3
#define EFUSE_IDX_C_DAT4            4
#define EFUSE_IDX_C_DAT5            5
#define EFUSE_IDX_C_CTRL0           6
#define EFUSE_IDX_C_CTRL1           7

/******************************************************************************
 * EFUSE RUN-TIME BLOWING ERROR CODE
 ******************************************************************************/
#define S_EFUSE_DONE                        0
#define S_EFUSE_BLOW_ERROR                  0x00100000
#define S_EFUSE_PMIC_ERR                    0x00020000
#define S_EFUSE_FIELD_NOT_SUPPORTED         0x00001000
#define S_EFUSE_INPUT_DATA_NOT_VALID        0x00002000
#define S_EFUSE_MULTI_ECC_FIELD_BLOWN_TWICE 0x00004000

/******************************************************************************
 * EFUSE RUN-TIME BLOWING FUNCTIONS
 ******************************************************************************/
extern u32 efuse_runtime_read(u32 index, u32 *data);
extern u32 efuse_runtime_blow(u32 index, u32 data);

#endif /* __EFUSE_RUNTIME_H__ */
