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

#define DEVAPC0_AO_BASE         0x10007000      // for AP
#define DEVAPC0_PD_BASE         0x10207000      // for AP


/*******************************************************************************
 * REGISTER ADDRESS DEFINATION
 ******************************************************************************/
#define DEVAPC0_D0_APC_0            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0000))
#define DEVAPC0_D0_APC_1            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0004))
#define DEVAPC0_D0_APC_2            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0008))
#define DEVAPC0_D0_APC_3            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x000C))
#define DEVAPC0_D0_APC_4            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0010))
#define DEVAPC0_D0_APC_5            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0014))
#define DEVAPC0_D0_APC_6            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0018))
#define DEVAPC0_D0_APC_7            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x001C))
#define DEVAPC0_D0_APC_8            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0020))

#define DEVAPC0_D1_APC_0            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0100))
#define DEVAPC0_D1_APC_1            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0104))
#define DEVAPC0_D1_APC_2            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0108))
#define DEVAPC0_D1_APC_3            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x010C))
#define DEVAPC0_D1_APC_4            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0110))
#define DEVAPC0_D1_APC_5            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0114))
#define DEVAPC0_D1_APC_6            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0118))
#define DEVAPC0_D1_APC_7            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x011C))
#define DEVAPC0_D1_APC_8            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0120))

#define DEVAPC0_D2_APC_0            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0200))
#define DEVAPC0_D2_APC_1            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0204))
#define DEVAPC0_D2_APC_2            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0208))
#define DEVAPC0_D2_APC_3            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x020C))
#define DEVAPC0_D2_APC_4            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0210))
#define DEVAPC0_D2_APC_5            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0214))
#define DEVAPC0_D2_APC_6            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0218))
#define DEVAPC0_D2_APC_7            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x021C))
#define DEVAPC0_D2_APC_8            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0220))

#define DEVAPC0_D3_APC_0            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0300))
#define DEVAPC0_D3_APC_1            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0304))
#define DEVAPC0_D3_APC_2            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0308))
#define DEVAPC0_D3_APC_3            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x030C))
#define DEVAPC0_D3_APC_4            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0310))
#define DEVAPC0_D3_APC_5            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0314))
#define DEVAPC0_D3_APC_6            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0318))
#define DEVAPC0_D3_APC_7            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x031C))
#define DEVAPC0_D3_APC_8            ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0320))

#if defined(MACH_TYPE_MT6735) || defined(MACH_TYPE_MT6737T)

#define DEVAPC0_MAS_DOM_0           ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0A00))
#define DEVAPC0_MAS_DOM_1           ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0A04))
#define DEVAPC0_MAS_DOM_2           ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0A08))
#define DEVAPC0_MAS_SEC             ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0B00))

#elif defined(MACH_TYPE_MT6735M) || defined(MACH_TYPE_MT6737M)

#define DEVAPC0_MAS_DOM_0           ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0400))
#define DEVAPC0_MAS_DOM_1           ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0404))
#define DEVAPC0_MAS_DOM_2           ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0408))
#define DEVAPC0_MAS_SEC             ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0500))

#elif defined(MACH_TYPE_MT6753)

#define DEVAPC0_MAS_DOM_0           ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0A00))
#define DEVAPC0_MAS_DOM_1           ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0A04))
#define DEVAPC0_MAS_DOM_2           ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0A08))
#define DEVAPC0_MAS_SEC             ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0B00))

#else

#error "Wrong MACH type"

#endif

#define DEVAPC0_APC_CON             ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0F00))
#define DEVAPC0_APC_LOCK_0          ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0F04))
#define DEVAPC0_APC_LOCK_1          ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0F08))
#define DEVAPC0_APC_LOCK_2          ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0F0C))
#define DEVAPC0_APC_LOCK_3          ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0F10))
#define DEVAPC0_APC_LOCK_4          ((volatile unsigned int*)(DEVAPC0_AO_BASE+0x0F14))

#define DEVAPC0_PD_APC_CON          ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x0F00))
#define DEVAPC0_D0_VIO_MASK_0       ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x0000))
#define DEVAPC0_D0_VIO_MASK_1       ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x0004))
#define DEVAPC0_D0_VIO_MASK_2       ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x0008))
#define DEVAPC0_D0_VIO_MASK_3       ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x000C))
#define DEVAPC0_D0_VIO_MASK_4       ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x0010))
#define DEVAPC0_D0_VIO_STA_0        ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x0400))
#define DEVAPC0_D0_VIO_STA_1        ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x0404))
#define DEVAPC0_D0_VIO_STA_2        ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x0408))
#define DEVAPC0_D0_VIO_STA_3        ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x040C))
#define DEVAPC0_D0_VIO_STA_4        ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x0410))
#define DEVAPC0_VIO_DBG0            ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x0900))
#define DEVAPC0_VIO_DBG1            ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x0904))

#define DEVAPC0_DEC_ERR_CON         ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x0F80))
#define DEVAPC0_DEC_ERR_ADDR        ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x0F84))
#define DEVAPC0_DEC_ERR_ID          ((volatile unsigned int*)(DEVAPC0_PD_BASE+0x0F88))


#define DEVAPC_APC_CON_CTRL         (0x1 << 0)
#define DEVAPC_APC_CON_EN           0x1
#define MASTER_MSDC0                4
#define MASTER_SPI0                 7

typedef enum {
    NON_SECURE_TRAN = 0,
    SECURE_TRAN,
} E_TRANSACTION;


///* DOMAIN_SETUP */


#define DOMAIN_0                      0
#define DOMAIN_1                      1
#define DOMAIN_2                      2
#define DOMAIN_3                      3

#if defined(MACH_TYPE_MT6735) || defined(MACH_TYPE_MT6737T)

#define DOMAIN_4                      4
#define DOMAIN_5                      5
#define DOMAIN_6                      6

#define CONN2AP                      (0xf << 16)//index12   DEVAPC0_MAS_DOM_1
#define MD1_DOMAIN                   (0xf << 24)//index14   DEVAPC0_MAS_DOM_1
#define MD3_DOMAIN                   (0xf <<  8)//index18   DEVAPC0_MAS_DOM_2
#define GPU                          (0xf << 20)//index21   DEVAPC0_MAS_DOM_2

#elif defined(MACH_TYPE_MT6735M) || defined(MACH_TYPE_MT6737M)
// Notice: Compare to Denali-2: (1)Denali-2 does not have Master "MD3"  (2)Denali-2 does not have Domain 4, 5, 6, 7.

#define CONN2AP                      (0xf << 24)//index12   DEVAPC0_MAS_DOM_0
#define MD1_DOMAIN                   (0xf << 28)//index14   DEVAPC0_MAS_DOM_0
#define GPU                          (0xf << 10)//index21   DEVAPC0_MAS_DOM_1

#elif defined(MACH_TYPE_MT6753)

#define DOMAIN_4                      4
#define DOMAIN_5                      5
#define DOMAIN_6                      6

#define CONN2AP                      (0xf << 16)//index12   DEVAPC0_MAS_DOM_1
#define MD1_DOMAIN                   (0xf << 24)//index14   DEVAPC0_MAS_DOM_1
#define MD3_DOMAIN                   (0xf <<  8)//index18   DEVAPC0_MAS_DOM_2
#define GPU                          (0xf << 20)//index21   DEVAPC0_MAS_DOM_2

#else

#error "Wrong MACH type"

#endif

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

#endif
