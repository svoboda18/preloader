/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2011
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

#ifndef TZ_TBASE_H
#define TZ_TBASE_H

#include "typedefs.h"

/* Tbase Magic For Interface */
#define TBASE_BOOTCFG_MAGIC (0x434d4254) // String TBMC in little-endian

/* TEE version */
#define TEE_ARGUMENT_VERSION            (0x00010000U)

typedef struct {
    u32 magic;        // magic value from information 
    u32 length;       // size of struct in bytes.
    u64 version;      // Version of structure
    u64 dRamBase;     // NonSecure DRAM start address
    u64 dRamSize;     // NonSecure DRAM size
    u64 secDRamBase;  // Secure DRAM start address
    u64 secDRamSize;  // Secure DRAM size
    u64 secIRamBase;  // Secure IRAM base
    u64 secIRamSize;  // Secure IRam size
    u64 conf_mair_el3;// MAIR_EL3 for memory attributes sharing
    u32 RFU1;
    u32 MSMPteCount;  // Number of MMU entries for MSM
    u64 MSMBase;      // MMU entries for MSM
    u64 gic_distributor_base;
    u64 gic_cpuinterface_base;
    u32 gic_version;
    u32 total_number_spi;
    u32 ssiq_number;
    u32 RFU2;
    u64 flags;
}tee_arg_t, *tee_arg_t_ptr;

/**************************************************************************
 * EXPORTED FUNCTIONS
 **************************************************************************/
void tbase_secmem_param_prepare(u32 param_addr, u32 tee_entry, u32 tbase_sec_dram_size, u32 tee_smem_size);
void tbase_boot_param_prepare(u32 param_addr, u32 tee_entry, u64 tbase_sec_dram_size, u64 dram_base, u64 dram_size);

#endif /* TZ_TBASE_H */

