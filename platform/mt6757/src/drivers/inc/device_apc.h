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


#ifndef _MTK_DEVICE_APC_H
#define _MTK_DEVICE_APC_H

#include "typedefs.h"

#define DEVAPC0_AO_BASE         0x1000E000
#define DEVAPC0_PD_BASE         0x10207000


/*******************************************************************************
 * REGISTER ADDRESS DEFINATION
 ******************************************************************************/
/* Device APC AO */
#define DEVAPC0_D0_APC_0            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0000))
#define DEVAPC0_D1_APC_0            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0100))
#define DEVAPC0_D2_APC_0            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0200))
#define DEVAPC0_D3_APC_0            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0300))
#define DEVAPC0_D4_APC_0            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0400))
#define DEVAPC0_D5_APC_0            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0500))
#define DEVAPC0_D6_APC_0            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0600))
#define DEVAPC0_D7_APC_0            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0700))

#define DEVAPC0_MAS_DOM_0           ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0A00))
#define DEVAPC0_MAS_DOM_1           ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0A04))
#define DEVAPC0_MAS_DOM_2           ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0A08))
#define DEVAPC0_MAS_DOM_3           ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0A0C))

#define DEVAPC0_MAS_SEC_0           ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0B00))

#define DEVAPC0_MD_SEC_0            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0D00))
#define DEVAPC0_MD_SEC_1            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0D04))
#define DEVAPC0_MD_SEC_R_0          ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0D08))
#define DEVAPC0_MD_SEC_R_1          ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0D0C))

#define DEVAPC0_MM_SEC_0            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0E00))
#define DEVAPC0_MM_SEC_1            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0E04))
#define DEVAPC0_MM_SEC_2            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0E08))

#define DEVAPC0_APC_CON             ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0F00))
#define DEVAPC0_APC_LOCK_0          ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0F04))
#define DEVAPC0_APC_LOCK_1          ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0F08))
#define DEVAPC0_APC_LOCK_2          ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0F0C))
#define DEVAPC0_APC_LOCK_3          ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0F10))
#define DEVAPC0_APC_LOCK_4          ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0F14))
#define DEVAPC0_APC_LOCK_5          ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0F18))
#define DEVAPC0_APC_LOCK_6          ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0F1C))


/* Device APC PD */
#define DEVAPC0_D0_VIO_MASK_0       ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x0000))
#define DEVAPC0_D0_VIO_MASK_1       ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x0004))
#define DEVAPC0_D0_VIO_MASK_2       ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x0008))
#define DEVAPC0_D0_VIO_MASK_3       ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x000C))
#define DEVAPC0_D0_VIO_MASK_4       ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x0010))
#define DEVAPC0_D0_VIO_MASK_5       ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x0014))
#define DEVAPC0_D0_VIO_MASK_6       ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x0018))
#define DEVAPC0_D0_VIO_MASK_7       ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x001C))

#define DEVAPC0_D0_VIO_STA_0        ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x0400))
#define DEVAPC0_D0_VIO_STA_1        ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x0404))
#define DEVAPC0_D0_VIO_STA_2        ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x0408))
#define DEVAPC0_D0_VIO_STA_3        ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x040C))
#define DEVAPC0_D0_VIO_STA_4        ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x0410))
#define DEVAPC0_D0_VIO_STA_5        ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x0414))
#define DEVAPC0_D0_VIO_STA_6        ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x0418))
#define DEVAPC0_D0_VIO_STA_7        ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x041C))

#define DEVAPC0_VIO_DBG0            ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x0900))
#define DEVAPC0_VIO_DBG1            ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x0904))

#define DEVAPC0_PD_APC_CON          ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x0F00))

#define DEVAPC0_DEC_ERR_CON         ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x0F80))
#define DEVAPC0_DEC_ERR_ADDR        ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x0F84))
#define DEVAPC0_DEC_ERR_ID          ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x0F88))


// device apc parameter
#define MOD_NO_IN_1_DEVAPC          16
#define DEVAPC_APC_CON_SEN          0x1


// device apc attribute
typedef enum {
	E_L0 = 0,
	E_L1,
	E_L2,
	E_L3,
	E_MAX_APC_ATTR
} APC_ATTR;


// domain index
typedef enum {
	E_DOMAIN_0 = 0,
	E_DOMAIN_1 ,
	E_DOMAIN_2 ,
	E_DOMAIN_3 ,
	E_DOMAIN_4 ,
	E_DOMAIN_5 ,
	E_DOMAIN_6 ,
	E_DOMAIN_7 ,
	E_MAX_MASK_DOM
} E_MASK_DOM;


// transaction attribute
typedef enum {
	NON_SECURE_TRAN = 0,
	SECURE_TRAN
} E_TRANSACTION;


///* DOMAIN_SETUP */
#define DOMAIN_0                      0
#define DOMAIN_1                      1
#define DOMAIN_2                      2
#define DOMAIN_3                      3
#define DOMAIN_4                      4
#define DOMAIN_5                      5
#define DOMAIN_6                      6
#define DOMAIN_7                      7


/* Masks for Domain Control for DEVAPC */
#define MD1_DOMAIN                      (0xf << 0)
#define MD3_DOMAIN                      (0xf << 4)
#define CONN2AP                         (0xf << 24)
#define MFG_M0                          (0xf << 4)


#define MASTER_MSDC0                    5

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
	do {	\
		volatile unsigned int tv = reg_read32(reg); \
		val = ((tv & (field)) >> (uffs((unsigned int)field) - 1)); \
	} while(0)

#endif
