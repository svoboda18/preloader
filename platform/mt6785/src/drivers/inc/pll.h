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

#ifndef PLL_H
#define PLL_H
#include "platform.h"

/* SPM Register */
#define POWERON_CONFIG_EN	(SPM_BASE + 0x0000)
#define DIS_PWR_CON		(SPM_BASE + 0x030C)
#define PWR_STATUS		(SPM_BASE + 0x0160)
#define PWR_STATUS_2ND		(SPM_BASE + 0x0164)

/* INFRACFG Register */
#define INFRA_TOPAXI_SI0_STA	(INFRACFG_BASE + 0x0000)
#define INFRA_TOPAXI_SI2_STA	(INFRACFG_BASE + 0x0028)

/* MCUCFG Register */
#define CPU_PLLDIV_CFG0          (MCUCFG_BASE + 0xA2A0) //ARMPLL_LL
#define CPU_PLLDIV_CFG1          (MCUCFG_BASE + 0xA2A4) //ARMPLL_BL
//#define CPU_PLLDIV_CFG2          (MCUCFG_BASE + 0xA2A8) //ARMPLL_BB
#define BUS_PLLDIV_CFG           (MCUCFG_BASE + 0xA2E0) //CCIPLL

/* APMIXEDSYS Register */
#define AP_PLL_CON0             (APMIXED_BASE + 0x00)
#define AP_PLL_CON1             (APMIXED_BASE + 0x04)
#define AP_PLL_CON2             (APMIXED_BASE + 0x08)
#define AP_PLL_CON3             (APMIXED_BASE + 0x0C)
#define AP_PLL_CON4             (APMIXED_BASE + 0x10)
#define AP_PLL_CON5             (APMIXED_BASE + 0x14)
#define AP_PLL_CON6             (APMIXED_BASE + 0x18)
#define AP_PLL_CON7             (APMIXED_BASE + 0x1C)
#define AP_PLL_CON8             (APMIXED_BASE + 0x20)
#define CLKSQ_STB_CON0          (APMIXED_BASE + 0x24)
#define PLL_PWR_CON0            (APMIXED_BASE + 0x28)
#define PLL_PWR_CON1            (APMIXED_BASE + 0x2C)
#define PLL_ISO_CON0            (APMIXED_BASE + 0x30)
#define PLL_ISO_CON1            (APMIXED_BASE + 0x34)
#define PLL_STB_CON0            (APMIXED_BASE + 0x38)
#define DIV_STB_CON0            (APMIXED_BASE + 0x3C)
#define PLL_CHG_CON0            (APMIXED_BASE + 0x40)
#define PLL_TEST_CON0           (APMIXED_BASE + 0x44)

#define ARMPLL_LL_CON0          (APMIXED_BASE + 0x200)
#define ARMPLL_LL_CON1          (APMIXED_BASE + 0x204)
#define ARMPLL_LL_CON2          (APMIXED_BASE + 0x208)
#define ARMPLL_LL_PWR_CON0      (APMIXED_BASE + 0x20C)

#define ARMPLL_BL_CON0          (APMIXED_BASE + 0x210)
#define ARMPLL_BL_CON1          (APMIXED_BASE + 0x214)
#define ARMPLL_BL_CON2          (APMIXED_BASE + 0x218)
#define ARMPLL_BL_PWR_CON0      (APMIXED_BASE + 0x21C)

#define ARMPLL_BB_CON0          (APMIXED_BASE + 0x220)
#define ARMPLL_BB_CON1          (APMIXED_BASE + 0x224)
#define ARMPLL_BB_CON2          (APMIXED_BASE + 0x228)
#define ARMPLL_BB_PWR_CON0      (APMIXED_BASE + 0x22C)
                                              
#define MAINPLL_CON0            (APMIXED_BASE + 0x230)
#define MAINPLL_CON1            (APMIXED_BASE + 0x234)
#define MAINPLL_PWR_CON0        (APMIXED_BASE + 0x23C)
                                                
#define UNIVPLL_CON0            (APMIXED_BASE + 0x240)
#define UNIVPLL_CON1            (APMIXED_BASE + 0x244)
#define UNIVPLL_PWR_CON0        (APMIXED_BASE + 0x24C)
                                                
#define MFGPLL_CON0              (APMIXED_BASE + 0x250)
#define MFGPLL_CON1              (APMIXED_BASE + 0x254)
#define MFGPLL_CON2              (APMIXED_BASE + 0x258)
#define MFGPLL_PWR_CON0          (APMIXED_BASE + 0x25C)
                                                
#define MSDCPLL_CON0            (APMIXED_BASE + 0x260)
#define MSDCPLL_CON1            (APMIXED_BASE + 0x264)
#define MSDCPLL_CON2            (APMIXED_BASE + 0x268)
#define MSDCPLL_PWR_CON0        (APMIXED_BASE + 0x26C)

#define TVDPLL_CON0             (APMIXED_BASE + 0x270)
#define TVDPLL_CON1             (APMIXED_BASE + 0x274)
#define TVDPLL_CON2             (APMIXED_BASE + 0x278)
#define TVDPLL_PWR_CON0         (APMIXED_BASE + 0x27C)

#define MMPLL_CON0              (APMIXED_BASE + 0x280)
#define MMPLL_CON1              (APMIXED_BASE + 0x284)
#define MMPLL_CON2              (APMIXED_BASE + 0x288)
#define MMPLL_PWR_CON0          (APMIXED_BASE + 0x28C)

#define MPLL_CON0              (APMIXED_BASE + 0x290)
#define MPLL_CON1              (APMIXED_BASE + 0x294)
#define MPLL_CON2              (APMIXED_BASE + 0x298)
#define MPLL_CON3              (APMIXED_BASE + 0x29C)
#define MPLL_PWR_CON0          (APMIXED_BASE + 0x29C)


#define CCIPLL_CON0          (APMIXED_BASE + 0x2A0)
#define CCIPLL_CON1          (APMIXED_BASE + 0x2A4)
#define CCIPLL_CON2          (APMIXED_BASE + 0x2A8)
#define CCIPLL_PWR_CON0      (APMIXED_BASE + 0x2AC)

#define ADSPPLL_CON0          (APMIXED_BASE + 0x2B0)
#define ADSPPLL_CON1          (APMIXED_BASE + 0x2B4)
#define ADSPPLL_CON2          (APMIXED_BASE + 0x2B8)
#define ADSPPLL_PWR_CON0      (APMIXED_BASE + 0x2BC)

#define APLL1_CON0              (APMIXED_BASE + 0x2C0)
#define APLL1_CON1              (APMIXED_BASE + 0x2C4)
#define APLL1_CON2              (APMIXED_BASE + 0x2C8)
#define APLL1_CON3              (APMIXED_BASE + 0x2CC)
#define APLL1_PWR_CON0          (APMIXED_BASE + 0x2D0)
                                                
#define APLL2_CON0              (APMIXED_BASE + 0x2D4)
#define APLL2_CON1              (APMIXED_BASE + 0x2D8)
#define APLL2_CON2              (APMIXED_BASE + 0x2DC)
#define APLL2_CON3              (APMIXED_BASE + 0x2E0)
#define APLL2_PWR_CON0          (APMIXED_BASE + 0x2E4)

#define APUPLL_CON0             (APMIXED_BASE + 0x2EC)
#define APUPLL_CON1             (APMIXED_BASE + 0x2F0)
#define APUPLL_PWR_CON0         (APMIXED_BASE + 0x2F8)

/* TOPCKGEN Register */
#define CLK_CFG_UPDATE          (TOPCKGEN_BASE + 0x004)
#define CLK_CFG_UPDATE1         (TOPCKGEN_BASE + 0x008)

//#define CLK_CFG_0               (TOPCKGEN_BASE + 0x020)
#define CLK_CFG_0_SET           (TOPCKGEN_BASE + 0x024)
#define CLK_CFG_0_CLR           (TOPCKGEN_BASE + 0x028)

//#define CLK_CFG_1               (TOPCKGEN_BASE + 0x030)
#define CLK_CFG_1_SET           (TOPCKGEN_BASE + 0x034)
#define CLK_CFG_1_CLR           (TOPCKGEN_BASE + 0x038)

//#define CLK_CFG_2               (TOPCKGEN_BASE + 0x040)
#define CLK_CFG_2_SET           (TOPCKGEN_BASE + 0x044)
#define CLK_CFG_2_CLR           (TOPCKGEN_BASE + 0x048)

//#define CLK_CFG_3               (TOPCKGEN_BASE + 0x050)
#define CLK_CFG_3_SET           (TOPCKGEN_BASE + 0x054)
#define CLK_CFG_3_CLR           (TOPCKGEN_BASE + 0x058)

//#define CLK_CFG_4               (TOPCKGEN_BASE + 0x060)
#define CLK_CFG_4_SET           (TOPCKGEN_BASE + 0x064)
#define CLK_CFG_4_CLR           (TOPCKGEN_BASE + 0x068)

//#define CLK_CFG_5               (TOPCKGEN_BASE + 0x070)
#define CLK_CFG_5_SET           (TOPCKGEN_BASE + 0x074)
#define CLK_CFG_5_CLR           (TOPCKGEN_BASE + 0x078)

//#define CLK_CFG_6               (TOPCKGEN_BASE + 0x080) 
#define CLK_CFG_6_SET           (TOPCKGEN_BASE + 0x084)
#define CLK_CFG_6_CLR           (TOPCKGEN_BASE + 0x088)

//#define CLK_CFG_7               (TOPCKGEN_BASE + 0x090) 
#define CLK_CFG_7_SET           (TOPCKGEN_BASE + 0x094)
#define CLK_CFG_7_CLR           (TOPCKGEN_BASE + 0x098)

//#define CLK_CFG_8               (TOPCKGEN_BASE + 0x0A0) 
#define CLK_CFG_8_SET           (TOPCKGEN_BASE + 0x0A4)
#define CLK_CFG_8_CLR           (TOPCKGEN_BASE + 0x0A8)

//#define CLK_CFG_9               (TOPCKGEN_BASE + 0x0B0)
#define CLK_CFG_9_SET           (TOPCKGEN_BASE + 0x0B4)
#define CLK_CFG_9_CLR           (TOPCKGEN_BASE + 0x0B8)

//#define CLK_CFG_10              (TOPCKGEN_BASE + 0x0C0)
#define CLK_CFG_10_SET          (TOPCKGEN_BASE + 0x0C4)
#define CLK_CFG_10_CLR          (TOPCKGEN_BASE + 0x0C8)

//#define CLK_CFG_11              (TOPCKGEN_BASE + 0x0D0)
#define CLK_CFG_11_SET          (TOPCKGEN_BASE + 0x0D4)
#define CLK_CFG_11_CLR          (TOPCKGEN_BASE + 0x0D8)

//#define CLK_CFG_12              (TOPCKGEN_BASE + 0x0E0)
#define CLK_CFG_12_SET          (TOPCKGEN_BASE + 0x0E4)
#define CLK_CFG_12_CLR          (TOPCKGEN_BASE + 0x0E8)

//#define CLK_CFG_13              (TOPCKGEN_BASE + 0x0F0)
#define CLK_CFG_13_SET          (TOPCKGEN_BASE + 0x0F4)
#define CLK_CFG_13_CLR          (TOPCKGEN_BASE + 0x0F8)

#define CLK_MISC_CFG_0          (TOPCKGEN_BASE + 0x110)
#define CLK_MISC_CFG_1          (TOPCKGEN_BASE + 0x120)
#define CLK_DBG_CFG             (TOPCKGEN_BASE + 0x10C)
#define CLK_SCP_CFG_0           (TOPCKGEN_BASE + 0x200)
#define CLK_SCP_CFG_1           (TOPCKGEN_BASE + 0x210)
#define CLK26CALI_0             (TOPCKGEN_BASE + 0x220)
#define CLK26CALI_1             (TOPCKGEN_BASE + 0x224)

/* INFRASYS Register */
#define INFRA_BUS_DCM_CTRL      	(INFRACFG_AO_BASE + 0x70)
#define PERI_BUS_DCM_CTRL       	(INFRACFG_AO_BASE + 0x74)
#define MEM_DCM_CTRL			(INFRACFG_AO_BASE + 0x78)

#define INFRA_TOPAXI_PROTECTEN   	(INFRACFG_AO_BASE + 0x0220)
#define INFRA_TOPAXI_PROTECTEN_STA1 	(INFRACFG_AO_BASE + 0x0228)
#define INFRA_TOPAXI_PROTECTEN_1   	(INFRACFG_AO_BASE + 0x0250)
#define INFRA_TOPAXI_PROTECTEN_STA1_1 	(INFRACFG_AO_BASE + 0x0258)


#define INFRA_TOPAXI_PROTECTEN_SET	(INFRACFG_AO_BASE + 0x02A0)
#define INFRA_TOPAXI_PROTECTEN_CLR	(INFRACFG_AO_BASE + 0x02A4)
#define INFRA_TOPAXI_PROTECTEN_1_SET	(INFRACFG_AO_BASE + 0x02A8)
#define INFRA_TOPAXI_PROTECTEN_1_CLR	(INFRACFG_AO_BASE + 0x02AC)

#define INFRA_TOPAXI_PROTECTEN_MM_SET	(INFRACFG_AO_BASE + 0x02D4)
#define INFRA_TOPAXI_PROTECTEN_MM_CLR	(INFRACFG_AO_BASE + 0x02D8)
#define INFRA_TOPAXI_PROTECTEN_MM_STA1	(INFRACFG_AO_BASE + 0x02EC)
#define INFRA_TOPAXI_SI0_CTL_SET	(INFRACFG_AO_BASE + 0x03B8)
#define INFRA_TOPAXI_SI0_CTL_CLR	(INFRACFG_AO_BASE + 0x03BC)
#define INFRA_TOPAXI_SI2_CTL		(INFRACFG_AO_BASE + 0x0234)

/* infra_ao_mem */
#define INFRA_EMI_DCM_CFG0		(INFRACFG_AO_BASE + 0x1028)
#define INFRA_EMI_DCM_CFG1		(INFRACFG_AO_BASE + 0x102C)
#define INFRA_EMI_DCM_CFG3		(INFRACFG_AO_BASE + 0x1034)
#define INFRA_EMI_DCM_TOP_CK_ANCHOR_CFG	(INFRACFG_AO_BASE + 0x1038)

/* SMI_COMMON */
#define SMI_COMMON_SMI_CLAMP		(SMI_COMMON_BASE + 0x03C0)
#define SMI_COMMON_SMI_CLAMP_SET   	(SMI_COMMON_BASE + 0x03C4)
#define SMI_COMMON_SMI_CLAMP_CLR   	(SMI_COMMON_BASE + 0x03C8)

/* MCUSS Register */
//#define ACLKEN_DIV              (0x10200640)

#define INFRA_PDN_SET0          (INFRACFG_AO_BASE + 0x0080)
#define INFRA_PDN_CLR0          (INFRACFG_AO_BASE + 0x0084)
#define INFRA_PDN_SET1          (INFRACFG_AO_BASE + 0x0088)
#define INFRA_PDN_CLR1          (INFRACFG_AO_BASE + 0x008C)
#define INFRA_PDN_STA0          (INFRACFG_AO_BASE + 0x0090)
#define INFRA_PDN_STA1          (INFRACFG_AO_BASE + 0x0094)
#define INFRA_PDN_SET2          (INFRACFG_AO_BASE + 0x00A4)
#define INFRA_PDN_CLR2          (INFRACFG_AO_BASE + 0x00A8)
#define INFRA_PDN_STA2          (INFRACFG_AO_BASE + 0x00AC)
#define INFRA_PDN_SET3          (INFRACFG_AO_BASE + 0x00C0)
#define INFRA_PDN_CLR3          (INFRACFG_AO_BASE + 0x00C4)
#define INFRA_PDN_STA3          (INFRACFG_AO_BASE + 0x00C8)
/* Audio Register*/
//#define AUDIO_TOP_CON0          (AUDIO_BASE + 0x0000)
//#define AUDIO_TOP_CON1          (AUDIO_BASE + 0x0004)
/* MFGCFG Register*/            
//#define MFG_CG_CON              (MFGCFG_BASE + 0)
//#define MFG_CG_SET              (MFGCFG_BASE + 4)
//#define MFG_CG_CLR              (MFGCFG_BASE + 8)

/* MMSYS Register*/
#define DISP_CG_CON0            (MMSYS_CONFIG_BASE + 0x100)
#define DISP_CG_SET0            (MMSYS_CONFIG_BASE + 0x104)
#define DISP_CG_CLR0            (MMSYS_CONFIG_BASE + 0x108)
#define DISP_CG_CON1            (MMSYS_CONFIG_BASE + 0x110)
#define DISP_CG_SET1            (MMSYS_CONFIG_BASE + 0x114)
#define DISP_CG_CLR1            (MMSYS_CONFIG_BASE + 0x118)

/* IMGSYS Register */
//#define IMG_CG_CON              (IMGSYS_BASE + 0x0000)
//#define IMG_CG_SET              (IMGSYS_BASE + 0x0004)
//#define IMG_CG_CLR              (IMGSYS_BASE + 0x0008)

/* VENC Register*/
//#define VENC_CG_CON             (VENC_GCON_BASE + 0x0)
//#define VENC_CG_SET             (VENC_GCON_BASE + 0x4)
//#define VENC_CG_CLR             (VENC_GCON_BASE + 0x8)
#define TOPRGU_LATCH	(0x10007044)
#define DEBUG_LATCH	(0x0D01A100)


#endif
