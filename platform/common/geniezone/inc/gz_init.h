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

#ifndef GZ_INIT_H
#define GZ_INIT_H

#include "typedefs.h"
#include "blkdev.h"

#define GZ_KERNEL_LOAD_OFFSET   (0X38000000) /* GZ Code Start */

#if CFG_NEBULA_VM_SUPPORT
#define EL2_DRAM_SIZE           (0xC00000)  /* 12MB */
#define VM_DRAM_SIZE            (0x8000000) /* 128MB */
#else
#define EL2_DRAM_SIZE           (0xC00000)  /* 12MB */
#define VM_DRAM_SIZE            (0x0)       /* 0MB */
#endif

#define GZ_DRAM_SIZE            (EL2_DRAM_SIZE + VM_DRAM_SIZE)

#define GZ_ALIGNMENT            (0x200000)   /* 2MB alignment */
#if CFG_MICROTRUST_TEE_SUPPORT
#define GZ_ADDR_MAX             (0xA0000000) /* Upper limit */
#else
#define GZ_ADDR_MAX             (0x80000000) /* Upper limit */
#endif

#define GZ_MD_SHM_SIZE          (0x10000)    /* 64KB */
#define GZ_MD_SHM_ALIGNMENT     (0x2000000)  /* 64MB alignment */
#define GZ_MD_SHM_ADDR_MAX      (0x80000000) /* Upper limit */

#define GZ_SDSP_FW_SIZE         (0x200000)   /* 2MB */
#define GZ_SDSP_FW_ALIGNMENT    (0x200000)   /* 2MB alignment */
#define GZ_SDSP_FW_ADDR_MAX     (GZ_ADDR_MAX) /* Upper limit */

#define SAPU_MTEE_SHM_SIZE        (0x400000)   /* 4MB alignment */
#define SAPU_MTEE_SHM_ALIGNMENT   (0x200000)   /* 2MB alignment */
#define SAPU_MTEE_SHM_ADDR_MAX    (GZ_ADDR_MAX) /* Upper limit */

#define GZ_TEE_STATIC_SHM_SIZE       (0x200000)   /* 2MB */
#define GZ_TEE_STATIC_SHM_ALIGNMENT  (0x200000)   /* 2MB alignment */
#define GZ_TEE_STATIC_SHM_ADDR_MAX   (GZ_ADDR_MAX) /* Upper limit */

/* 64 + 1 PAGES */
#define GZ_LOG_BUFFER_SIZE (65 * 4096)

#define GZ_PAGE_SIZE (4096)
#define GZ_TEE_STATIC_SHM_HEADER_SIZE	4096
#define MAX_GZ_TEE_STATIC_SHM_ENTRY		GZ_TEE_STATIC_SHM_HEADER_SIZE/sizeof(gz_tee_static_shm_t)
#define MAGIC_NUMBER	0x76616C64
#define MAX_GZ_TEE_STATIC_SHM_NAME		16

typedef struct {
    u32 magic;
    u32 sz;
    u64 pa;
    char name[MAX_GZ_TEE_STATIC_SHM_NAME];
    u8 reserve[32];
} gz_tee_static_shm_t;

#define GZ_RPMB_SIZE            (0x20000)    /* 128KB*/

#define EL2_BOOTING_DISABLED    (0x4e6f475a) /* ascii nogz */

/* extern variables */
extern u32 gz_md_shm_addr;
extern u32 gz_md_shm_size;
extern u32 build_variant;

/* extern APIs */
unsigned int is_booting_el2(void);
int bldr_load_gz_part(blkdev_t *bdev, const char *part_name);
u32 gz_config_info_atag(boot_tag *tags);
u32 gz_config_boot_atag(boot_tag *tags);
u32 gz_config_platform_atag(boot_tag *tags);
#if CFG_GZ_PWRAP_ENABLE
u32 gz_config_pwrap_atag(boot_tag *tags);
#endif
void gz_pre_init(void);
void gz_post_init(void);
u64 gz_get_jump_addr(void);
int gz_de_init(void);
u32 gz_get_load_addr(u32 maddr);

u32 gz_get_sdsp_mem_info(u32 *pa, u32 *size);
u32 gz_get_sapu_shm_info(u32 *pa, u32 *size);
u32 get_gz_tee_static_shm_info(u32 *pa, u32 *size);

struct gz_mblock_info {
    u64 addr;
    u64 size;
    u64 align;
    u64 limit;
    void (*post_process)(u64 reserved_addr, u64 reserved_size);
    char *name;
};

#endif /* end of GZ_INIT_H */
