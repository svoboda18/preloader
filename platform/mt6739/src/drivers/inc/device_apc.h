/* MediaTek Inc. (C) 2010. All rights reserved.
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
* have been modified by MediaTek Inc. All revisions are subject to any receiver's
* applicable license agreements with MediaTek Inc.
*/


#ifndef __MTK_DEVICE_APC_H__
#define __MTK_DEVICE_APC_H__

#include "typedefs.h"

#define DEVAPC_AO_INFRA_BASE        0x1000E000  // for INFRA/PERI
#define DEVAPC_AO_MD_BASE           0x10019000  // for MD
#define DEVAPC_AO_MM_BASE           0x1001C000  // for MM

/*******************************************************************************
 * REGISTER ADDRESS DEFINATION
 ******************************************************************************/
/* Device APC AO for INFRA/PERI */

#define DEVAPC_AO_INFRA_MAS_DOM_0   ((volatile unsigned int*)(DEVAPC_AO_INFRA_BASE+0x0A00))
#define DEVAPC_AO_INFRA_MAS_DOM_1   ((volatile unsigned int*)(DEVAPC_AO_INFRA_BASE+0x0A04))
#define DEVAPC_AO_INFRA_MAS_DOM_2   ((volatile unsigned int*)(DEVAPC_AO_INFRA_BASE+0x0A08))
#define DEVAPC_AO_INFRA_MAS_DOM_3   ((volatile unsigned int*)(DEVAPC_AO_INFRA_BASE+0x0A0C))

#define DEVAPC_AO_INFRA_MAS_SEC_0   ((volatile unsigned int*)(DEVAPC_AO_INFRA_BASE+0x0B00))

#define DEVAPC_AO_INFRA_DOM_RMP_0   ((volatile unsigned int*)(DEVAPC_AO_INFRA_BASE+0x0D00))
#define DEVAPC_AO_INFRA_DOM_RMP_1   ((volatile unsigned int*)(DEVAPC_AO_INFRA_BASE+0x0D04))

#define DEVAPC_AO_INFRA_APC_CON     ((volatile unsigned int*)(DEVAPC_AO_INFRA_BASE+0x0F00))

/* ---------------------------------------------------------------------------------------- */
/* Device APC AO for MD */

#define DEVAPC_AO_MD_DOM_RMP_0      ((volatile unsigned int*)(DEVAPC_AO_MD_BASE+0x0D00))

#define DEVAPC_AO_MD_APC_CON        ((volatile unsigned int*)(DEVAPC_AO_MD_BASE+0x0F00))

/* ---------------------------------------------------------------------------------------- */
/* Device APC AO for MM */

#define DEVAPC_AO_MM_DOM_RMP_0      ((volatile unsigned int*)(DEVAPC_AO_MM_BASE+0x0D00))

#define DEVAPC_AO_MM_APC_CON        ((volatile unsigned int*)(DEVAPC_AO_MM_BASE+0x0F00))

/* ---------------------------------------------------------------------------------------- */

#define MOD_NO_IN_1_DEVAPC          16
#define DEVAPC_APC_CON_SEN          0x1
#define MASTER_NUM_INFRA            30

// TRANSACTION_ATTRIBUTE
typedef enum {
    NON_SECURE_TRAN = 0,
    SECURE_TRAN
} E_TRANSACTION;

// Master Type
typedef enum {
    MASTER_TYPE_INFRA = 0,
} E_MASTER_TYPE;

///* DOMAIN_SETUP */
#define DOMAIN_0                      0
#define DOMAIN_1                      1
#define DOMAIN_2                      2
#define DOMAIN_3                      3
#define DOMAIN_4                      4
#define DOMAIN_5                      5
#define DOMAIN_6                      6
#define DOMAIN_7                      7
#define DOMAIN_8                      8
#define DOMAIN_9                      9
#define DOMAIN_10                     10
#define DOMAIN_11                     11
#define DOMAIN_12                     12
#define DOMAIN_13                     13
#define DOMAIN_14                     14
#define DOMAIN_15                     15

/* ---------------------------------------------------------------------------------------- */
/* Masks for Domain Control for DEVAPC */
/* Need to be updated */
#define MD1                             (0xF << 0)
#define CONN2AP                         (0xF << 24)
#define MFG                             (0xF << 4)
#define SSPM                            (0xF << 12)

/* ---------------------------------------------------------------------------------------- */
// domain index
typedef enum {
    E_DOMAIN_0 = 0,
    E_DOMAIN_1,
    E_DOMAIN_2,
    E_DOMAIN_3,
    E_DOMAIN_4,
    E_DOMAIN_5,
    E_DOMAIN_6,
    E_DOMAIN_7
} E_MASK_DOM;

/* ---------------------------------------------------------------------------------------- */
// INFRA DOMAIN REMAP
#define INFRA_DOM_RMP_INIT      (0xFFFFFFFF)

#define INFRA_AP                (0x7 << 0)   //EMI MPU domain 0
#define INFRA_MD1               (0x7 << 3)   //EMI MPU domain 1
#define INFRA_CONN              (0x7 << 6)   //EMI MPU domain 2
#define INFRA_SCP               (0x7 << 9)   //EMI MPU domain 3
#define INFRA_MM                (0x7 << 12)  //EMI MPU domain 4
#define INFRA_MD3               (0x7 << 15)  //EMI MPU domain 5 (RESERVED)
#define INFRA_MFG               (0x7 << 18)  //EMI MPU domain 6
#define INFRA_MD_HW             (0x7 << 21)  //EMI MPU domain 7
#define INFRA_SSPM              (0x7 << 24)  //EMI MPU domain 8

/* ---------------------------------------------------------------------------------------- */
// MM DOMAIN REMAP
#define MM_DOM_RMP_INIT         (0xFFFFFFFF)

#define MM_AP                   (0x3 << 0)   //EMI MPU domain 0
#define MM_MD1                  (0x3 << 2)   //EMI MPU domain 1
#define MM_CONN                 (0x3 << 4)   //EMI MPU domain 2
#define MM_SCP                  (0x3 << 6)   //EMI MPU domain 3
#define MM_MM                   (0x3 << 8)   //EMI MPU domain 4
#define MM_MD3                  (0x3 << 10)  //EMI MPU domain 5 (RESERVED)
#define MM_MFG                  (0x3 << 12)  //EMI MPU domain 6
#define MM_MD_HW                (0x3 << 14)  //EMI MPU domain 7
#define MM_SSPM                 (0x3 << 16)  //EMI MPU domain 8

/* ---------------------------------------------------------------------------------------- */
// MD DOMAIN REMAP
#define MD_DOM_RMP_INIT         (0xFFFFFFFF)

#define MD_INFRA_DOMAIN_0       (0x3 << 0)   //Infra domain 0
#define MD_INFRA_DOMAIN_1       (0x3 << 2)   //Infra domain 1
#define MD_INFRA_DOMAIN_2       (0x3 << 4)   //Infra domain 2
#define MD_INFRA_DOMAIN_3       (0x3 << 6)   //Infra domain 3
#define MD_INFRA_DOMAIN_4       (0x3 << 8)   //Infra domain 4
#define MD_INFRA_DOMAIN_5       (0x3 << 10)  //Infra domain 5
#define MD_INFRA_DOMAIN_6       (0x3 << 12)  //Infra domain 6
#define MD_INFRA_DOMAIN_7       (0x3 << 14)  //Infra domain 7

/* ---------------------------------------------------------------------------------------- */

#define MASTER_MSDC0             5
#define MASTER_UFS              15

/* ---------------------------------------------------------------------------------------- */

static inline unsigned int uffs(unsigned int x)
{
    unsigned int r = 1;

    if (!x)
        return 0;
    if (!(x & 0xffff)) {
        x >>= 16;
        r += 16;
    }
    if (!(x & 0xff)) {
        x >>= 8;
        r += 8;
    }
    if (!(x & 0xf)) {
        x >>= 4;
        r += 4;
    }
    if (!(x & 3)) {
        x >>= 2;
        r += 2;
    }
    if (!(x & 1)) {
        x >>= 1;
        r += 1;
    }
    return r;
}

#define reg_read16(reg)          __raw_readw(reg)
#define reg_read32(reg)          __raw_readl(reg)
#define reg_write16(reg,val)     __raw_writew(val,reg)
#define reg_write32(reg,val)     __raw_writel(val,reg)

#define reg_set_bits(reg,bs)     ((*(volatile u32*)(reg)) |= (u32)(bs))
#define reg_clr_bits(reg,bs)     ((*(volatile u32*)(reg)) &= ~((u32)(bs)))

#define reg_set_field(reg,field,val) \
    do {    \
        volatile unsigned int tv = reg_read32(reg); \
        tv &= ~(field); \
        tv |= ((val) << (uffs((unsigned int)field) - 1)); \
        reg_write32(reg,tv); \
    } while(0)

#define reg_get_field(reg,field,val) \
    do {    \
        volatile unsigned int tv = reg_read32(reg); \
        val = ((tv & (field)) >> (uffs((unsigned int)field) - 1)); \
    } while(0)

#endif //__MTK_DEVICE_APC_H__
