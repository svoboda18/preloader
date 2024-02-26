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
//#include "typedefs.h"
#include "platform.h"
#include "tz_mem.h"
#include "tz_trusty.h"
//#include "mmc_rpmb.h"

#define MOD "[TZ_TRUSTY]"

#define TEE_DEBUG
#ifdef TEE_DEBUG
#define DBG_MSG(str, ...) do {print(str, ##__VA_ARGS__);} while(0)
#else
#define DBG_MSG(str, ...) do {} while(0)
#endif

//extern unsigned int seclib_get_msg_auth_key(unsigned char *id, unsigned int id_size, unsigned char *key, unsigned int key_size);
//extern unsigned int seclib_scramble_key(unsigned char *key_in, unsigned int key_in_size);
//extern int tee_get_hwuid(u8 *id, u32 size);

/**************************************************************************
 *  EXTERNAL FUNCTIONS
 **************************************************************************/
#if 0
void tbase_secmem_param_prepare(u32 param_addr, u32 tee_entry,
    u32 tbase_sec_dram_size, u32 tee_smem_size)
{
    int ret = 0;
    sec_mem_arg_t sec_mem_arg;
    u8 hwuid[16];

    ret = tee_get_hwuid(hwuid, 16);
    if (ret)
        DBG_MSG("%s hwuid not initialized yet\n", MOD);

    /* Prepare secure memory configuration parameters */
    sec_mem_arg.magic = SEC_MEM_MAGIC;
    sec_mem_arg.version = SEC_MEM_VERSION;
    sec_mem_arg.svp_mem_start = tee_entry + tbase_sec_dram_size;
    sec_mem_arg.tplay_mem_size = SEC_MEM_TPLAY_MEMORY_SIZE;
    sec_mem_arg.tplay_mem_start = tee_entry + (tee_smem_size - SEC_MEM_TPLAY_MEMORY_SIZE);
    sec_mem_arg.tplay_table_size = SEC_MEM_TPLAY_TABLE_SIZE;
    sec_mem_arg.tplay_table_start = sec_mem_arg.tplay_mem_start - SEC_MEM_TPLAY_TABLE_SIZE;
    sec_mem_arg.svp_mem_end = sec_mem_arg.tplay_table_start;
    seclib_get_msg_auth_key((unsigned char *)hwuid, 16, (unsigned char *)sec_mem_arg.msg_auth_key, 32);
    //seclib_scramble_key((unsigned char *)sec_mem_arg.msg_auth_key, 32);
    sec_mem_arg.rpmb_size = mmc_rpmb_get_size();

#if CFG_TEE_SECURE_MEM_PROTECTED
    sec_mem_arg.secmem_obfuscation = 1;
#else
    sec_mem_arg.secmem_obfuscation = 0;
#endif

    DBG_MSG("%s sec_mem_arg.magic: 0x%x\n", MOD, sec_mem_arg.magic);
    DBG_MSG("%s sec_mem_arg.version: 0x%x\n", MOD, sec_mem_arg.version);
    DBG_MSG("%s sec_mem_arg.svp_mem_start: 0x%x\n", MOD, sec_mem_arg.svp_mem_start);
    DBG_MSG("%s sec_mem_arg.svp_mem_end: 0x%x\n", MOD, sec_mem_arg.svp_mem_end);
    DBG_MSG("%s sec_mem_arg.tplay_mem_start: 0x%x\n", MOD, sec_mem_arg.tplay_mem_start);
    DBG_MSG("%s sec_mem_arg.tplay_mem_size: 0x%x\n", MOD, sec_mem_arg.tplay_mem_size);
    DBG_MSG("%s sec_mem_arg.tplay_table_start: 0x%x\n", MOD, sec_mem_arg.tplay_table_start);
    DBG_MSG("%s sec_mem_arg.tplay_table_size: 0x%x\n", MOD, sec_mem_arg.tplay_table_size);
    DBG_MSG("%s sec_mem_arg.secmem_obfuscation: 0x%x\n", MOD, sec_mem_arg.secmem_obfuscation);
    DBG_MSG("%s tee_entry_addr: 0x%x\n", MOD, tee_entry);
    DBG_MSG("%s tee_secmem_size: 0x%x\n", MOD, tee_smem_size);
    DBG_MSG("%s rpmb_size: 0x%x\n", MOD, sec_mem_arg.rpmb_size);

    memcpy((void*)param_addr, &sec_mem_arg, sizeof(sec_mem_arg_t));
}
#endif

void trusty_boot_param_prepare(u32 param_addr, u32 tee_entry,
    u64 trusty_sec_dram_size, u64 dram_base, u64 dram_size,
    u8 *rpmb_key, u32 rpmb_key_size)
{
    tee_v8_arg_t *teearg = (tee_v8_arg_t *) param_addr;
    u32 instr;

    /* Prepare TEE boot parameters */
    teearg->magic                 = TEE_ARGUMENT_MAGIC;        /* Trusty's TEE magic number */
    teearg->version               = TEE_ARGUMENT_VERSION;      /* Trusty's TEE argument block version */
    teearg->dRamBase              = dram_base;                 /* DRAM base address */
    teearg->dRamSize              = dram_size;                 /* Full DRAM size */
    teearg->secDRamBase           = tee_entry;                 /* Secure DRAM base address */
    teearg->secDRamSize           = trusty_sec_dram_size;      /* Secure DRAM size */
    teearg->sRamBase              = TEE_SECURE_ISRAM_ADDR;     /* Secure SRAM base address */
    teearg->sRamSize              = TEE_SECURE_ISRAM_SIZE;     /* Secure SRAM size */
    teearg->secSRamBase           = 0x00000000;                /* Secure SRAM base address */
    teearg->secSRamSize           = 0x00000000;                /* Secure SRAM size */
    teearg->log_port              = CFG_UART_LOG;              /* UART base regsiter address */
    teearg->log_baudrate          = CFG_LOG_BAUDRATE;          /* UART baudrate */
    tee_get_hwuid(teearg->hwuid, 16);
    teearg->gicd_base             = GICD_BASE;
    teearg->gicr_base             = GICR_BASE;
    teearg->gic_ver               = 0x3;
    memcpy(teearg->rpmb_key, rpmb_key, rpmb_key_size);

    instr = *(u32 *)tee_entry;

    if (instr >> 24 == 0xea) {
        teearg->aarch32 = 1;
    } else if (instr >> 8 == 0xd53810) {
        teearg->aarch32 = 0;
    } else {
        teearg->aarch32 = 1;
    }

    DBG_MSG("%s teearg->magic: 0x%x\n", MOD, teearg->magic);
    DBG_MSG("%s teearg->version: 0x%x\n", MOD, teearg->version);
    DBG_MSG("%s teearg->dRamBase: 0x%x\n", MOD, teearg->dRamBase);
    DBG_MSG("%s teearg->dRamSize: 0x%x\n", MOD, teearg->dRamSize);
    DBG_MSG("%s teearg->secDRamBase: 0x%x\n", MOD, teearg->secDRamBase);
    DBG_MSG("%s teearg->secDRamSize: 0x%x\n", MOD, teearg->secDRamSize);
    DBG_MSG("%s teearg->sRamBase: 0x%x\n", MOD, teearg->sRamBase);
    DBG_MSG("%s teearg->sRamSize: 0x%x\n", MOD, teearg->sRamSize);
    DBG_MSG("%s teearg->secSRamBase: 0x%x\n", MOD, teearg->secSRamBase);
    DBG_MSG("%s teearg->secSRamSize: 0x%x\n", MOD, teearg->secSRamSize);
    DBG_MSG("%s teearg->log_port: 0x%x\n", MOD, teearg->log_port);
    DBG_MSG("%s teearg->log_baudrate: 0x%x\n", MOD, teearg->log_baudrate);
    DBG_MSG("%s teearg->gicd_base: 0x%x\n", MOD, teearg->gicd_base);
    DBG_MSG("%s teearg->gicr_base: 0x%x\n", MOD, teearg->gicr_base);
    DBG_MSG("%s teearg->gic_ver: 0x%x\n", MOD, teearg->gic_ver);
    DBG_MSG("%s teearg->aarch32: 0x%x\n", MOD, teearg->aarch32);

#if 0
    {
        DBG_MSG("%s teearg->hwuid[0-3] : 0x%x 0x%x 0x%x 0x%x\n", MOD,
                teearg->hwuid[0], teearg->hwuid[1], teearg->hwuid[2], teearg->hwuid[3]);
    }
#endif

#if 0 /* RPMB KEY DUMP */
    {
	    int i;
	    u8 *_rpmb_key = teearg->rpmb_key;
	    for (i = 0; i < RPMB_KEY_SIZE; i+=8) {
		DBG_MSG("%s teearg->rpmb_key[%d:%d] : 0x%x 0x%x 0x%x 0x%x  0x%x 0x%x 0x%x 0x%x\n", MOD, i, i+7,
			_rpmb_key[i + 0], _rpmb_key[i + 1], _rpmb_key[i + 2], _rpmb_key[i + 3],
			_rpmb_key[i + 4], _rpmb_key[i + 5], _rpmb_key[i + 6], _rpmb_key[i + 7]);
	    }
    }
#endif /* RPMB KEY DUMP */
}

