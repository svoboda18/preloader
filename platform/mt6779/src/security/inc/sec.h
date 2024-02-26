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


#ifndef SEC_H
#define SEC_H

#include "sec_cust.h"
#include "sec_platform.h"
#include <verified_boot_error.h>

/******************************************************************************
 * SECURE CFG READ SIZE
 ******************************************************************************/
#define SEC_CFG_READ_SIZE              SEC_WORKING_BUFFER_LENGTH

/******************************************************************************
 * SECURE CFG PARTITION INFO
 ******************************************************************************/
typedef struct _SECURE_CFG_INFO {
    U32                                 addr;
    U32                                 len;
    U32                                 part_id;
} SECURE_CFG_INFO;

/**************************************************************************
 * EXPORTED FUNCTIONS
 **************************************************************************/

/* configuration for PL download DA feature */
BOOL seclib_sbc_enabled(void);
BOOL seclib_daa_enabled(void);
BOOL seclib_sla_enabled(void);

/* secure boot */
void sec_boot_check (void);
BOOL seclib_sec_boot_enabled (BOOL bMsg);
U32 seclib_image_check (U8* image_name, BOOL bMsg);
BOOL is_BR_cmd_disabled(void);

 /* sec_cfg related */
U8* sec_util_image_read (U32 offset, U32 size);
U8* sec_util_get_img_buf (void);
U8* sec_util_get_working_buf (void);
void seclib_image_hash_compute (U8 *buf, U32 size);
void seclib_cfg_print_status(void);
boot_mode_t seclib_brom_meta_mode(void);

/* library initialization */
void sec_malloc_buf_reset(void);
U32 seclib_init ( CUSTOM_SEC_CFG *cust_cfg, U8* sec_cfg_cipher_data, U32 sec_cfg_read_size, BOOL bMsg, BOOL bAC);

/* region check */
U32 seclib_region_check (U32 addr, U32 len);

#endif /* SEC_H */

