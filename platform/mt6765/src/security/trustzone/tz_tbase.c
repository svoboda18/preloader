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
 * MediaTek Inc. (C) 2010. All rights reserved.
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

/* Include header files */
#include "typedefs.h"
#include "tz_mem.h"
#include "tz_tbase.h"
#include "rpmb_key.h"
#include "mmc_rpmb.h"
#include "ufs_aio_interface.h"
#include "key_derive.h"

#define MOD "[TZ_TBASE]"

#define TEE_DEBUG
#ifdef TEE_DEBUG
#define DBG_MSG(str, ...) do {print(str, ##__VA_ARGS__);} while(0)
#else
#define DBG_MSG(str, ...) do {} while(0)
#endif

#define RPMB_KEY_SIZE     (32)

extern int tee_get_hwuid(u8 *id, u32 size);

void tbase_boot_param_prepare(u32 param_addr, u32 tee_entry,
    u64 tbase_sec_dram_size, u64 dram_base, u64 dram_size)
{
    tee_arg_t_ptr teearg = (tee_arg_t_ptr)param_addr;

    /* Prepare TEE boot parameters */
    teearg->magic                 = TBASE_BOOTCFG_MAGIC;             /* Trustonic's TEE magic number */
    teearg->length                = sizeof(tee_arg_t);               /* Trustonic's TEE argument block size */
    //teearg->version               = TBASE_MONITOR_INTERFACE_VERSION; /* Trustonic's TEE argument block version */
    teearg->dRamBase              = dram_base;                       /* DRAM base address */
    teearg->dRamSize              = dram_size;                       /* Full DRAM size */
    teearg->secDRamBase           = tee_entry;                       /* Secure DRAM base address */
    teearg->secDRamSize           = tbase_sec_dram_size;             /* Secure DRAM size */
    teearg->secIRamBase           = TEE_SECURE_ISRAM_ADDR;           /* Secure SRAM base address */
    teearg->secIRamSize           = TEE_SECURE_ISRAM_SIZE;           /* Secure SRAM size */
    //teearg->conf_mair_el3         = read_mair_el3();
    //teearg->MSMPteCount           = totalPages;
    //teearg->MSMBase               = (u64)registerFileL2;
    //teearg->gic_distributor_base  = TBASE_GIC_DIST_BASE;
    //teearg->gic_cpuinterface_base = TBASE_GIC_CPU_BASE;
    //teearg->gic_version           = TBASE_GIC_VERSION;
    teearg->total_number_spi      = 352;                             /* Support total 256 SPIs and 32 PPIs */
    teearg->ssiq_number           = (32 + 271);                      /* reserve SPI ID 271 for <t-base, which is ID 303 */
    //teearg->flags                 = TBASE_MONITOR_FLAGS;

    DBG_MSG("%s teearg.magic: 0x%x\n", MOD, teearg->magic);
    DBG_MSG("%s teearg.length: 0x%x\n", MOD, teearg->version);
    DBG_MSG("%s teearg.dRamBase: 0x%x\n", MOD, teearg->dRamBase);
    DBG_MSG("%s teearg.dRamSize: 0x%x\n", MOD, teearg->dRamSize);
    DBG_MSG("%s teearg.secDRamBase: 0x%x\n", MOD, teearg->secDRamBase);
    DBG_MSG("%s teearg.secDRamSize: 0x%x\n", MOD, teearg->secDRamSize);
    DBG_MSG("%s teearg.secIRamBase: 0x%x\n", MOD, teearg->secIRamBase);
    DBG_MSG("%s teearg.secIRamSize: 0x%x\n", MOD, teearg->secIRamSize);
    DBG_MSG("%s teearg.total_number_spi: %d\n", MOD, teearg->total_number_spi);
    DBG_MSG("%s teearg.ssiq_number: %d\n", MOD, teearg->ssiq_number);
}

