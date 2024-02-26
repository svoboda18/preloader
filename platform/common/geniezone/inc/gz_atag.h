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

#ifndef GZ_ATAG_H
#define GZ_ATAG_H

#include "typedefs.h"

/* gz configs */
#define EL2_BOOT_DISABLE        (1 << 0)
#define EL2_REMAP_ENABLE        (1 << 1)

/* gz info atag */
struct boot_tag_gz_info {
    u32 gz_configs;
    u32 lk_addr;
    u32 build_variant;
};

/* gz boot atag */
#define RPMB_KEY_SIZE           (32)
struct boot_tag_gz_param {
    u8  hwuid[ME_IDENTITY_LEN];
    u64 modemMteeShareMemPA;
    u32 modemMteeShareMemSize;
    u8  rpmb_key[RPMB_KEY_SIZE];
    u32 rpmb_base;
    u32 rpmb_size;
};

/* gz platform atag */
#define GZ_PLAT_FLAGS_REMAP_ENABLE (1 << 0)
#define GZ_PLAT_FLAGS_GIC_V2       (1 << 1)
#define GZ_PLAT_FLAGS_GIC_V3       (1 << 2)
#define GZ_PLAT_FLAGS_PWRAP_EN     (1 << 3)
#define GZ_PLAT_FLAGS_ERASE_ATAG   (1 << 31)
#define GZ_PLAT_FLAGS_NEBULA_ENABLED     (1 << 5)
#define GZ_INVALID_MPU_REGION_ID   (0x4E4F5054)
#define GZ_INVALID_MPU_DOMAIN      (0xFF)

#define GZ_PLAT_TAG_SIZE (256)
struct boot_tag_gz_platform {

    /* GZ platform */
    u64 flags;
    u64 exec_start_offset;
    u32 reserve_mem_size;

    /* GZ remap offset */
    struct {
        u64 offset_ddr;
        u64 offset_io;
        u64 offset_sec_io;
    } __attribute__((packed)) remap;

    /* HW reg base */
    struct {
        u32 uart;
        u32 cpuxgpt;
        u32 gicd;
        u32 gicr;
        u32 pwrap;
        u32 rtc;
        u32 mcucfg;
        u32 res[8];
    } __attribute__((packed)) reg_base;

    /* Hardware version */
    struct {
        u16 hw_code;
        u16 hw_sub_code;
    } __attribute__((packed)) plat_ver;

    /* VM info */
    u32 vm_mem_size;

    /* gz log info */
    u32 log_addr;
    u32 log_size;

    /* total detected DRAM size */
    u32 dram_size_1mb_cnt;

    /* device info area 30 */
    u32 dev_info_area30;

    /*sys timer irq*/
    u32 sys_timer_irq;

    /* RAZ */
    u32 reserved[31];

} __attribute__((packed));

#if CFG_GZ_PWRAP_ENABLE
/* gz pwrap atag */
#define GZ_PWRAP_TAG_SIZE (128)
struct boot_tag_gz_pwrap {
    u64 wacs1_init_done;
    u64 wacs1_cmd;
    u64 wacs1_rdata;
    u64 wacs1_vldclr;
    u64 get_wacs1_init_done1_shift;
    u64 get_sys_idle1_shift;
    u32 reserved[20];
} __attribute__((packed));
#endif

#endif /* end of GZ_ATAG_H */
