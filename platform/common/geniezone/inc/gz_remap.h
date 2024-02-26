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
 * MediaTek Inc. (C) 2016. All rights reserved.
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

#ifndef __GZ_REMAP_H__
#define __GZ_REMAP_H__

#include <stdint.h>
#include <gz_remap_mt.h>

/* GZ remap return code */
enum GZ_REMAP_STATUS {
    GZ_REMAP_SUCCESS,
    GZ_REMAP_ERR_NOT_IMPLEMENTED,
    GZ_REMAP_ERR_INVALID_ARGUMENT,
    GZ_REMAP_ERR_GENERIC
};

/* GZ remap VM IDs, MUST BE platform-common for backward-compatibility */
enum GZ_REMAP_VMID {
    GZ_REMAP_VMID_AP,
    GZ_REMAP_VMID_GZ,
    /* keep increasing ID for new VMs here */
};

/**
 * The listed are the universal interface for GZ remap version 1 & 2.
 * For certain APIs not supported by GZ remap v1, the function return
 * error code -GZ_REMAP_ERR_NOT_IMPLEMENTED is expected.
 * Note that the implementation is platform-dependent.
 */
/* GZ remap v1 & v2 supported */
int gz_remap_ddr_enable(void);
int gz_remap_ddr_disable(void);
uint64_t gz_remap_ddr_get_offset(uint32_t vm_id);
int gz_remap_io_enable(void);
int gz_remap_io_disable(void);
uint64_t gz_remap_io_get_offset(uint32_t vm_id);
int gz_remap_dump_config(void);
int gz_remap_init(void);

/* GZ remap v2 supported only */
int gz_remap_sec_io_enable(uint32_t vm_id);
int gz_remap_sec_io_disable(uint32_t vm_id);
uint64_t gz_remap_sec_io_get_offset(uint32_t vm_id);
int gz_remap_vm_domain_set(uint32_t vm_id, uint32_t map_domain);

#endif /* end of __GZ_REMAP_H__ */
