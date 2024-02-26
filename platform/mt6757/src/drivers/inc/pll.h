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
//#include "custom_emi.h"
#define PLL_H

#define APMIXED_BASE      (0x1000C000)
#define CKSYS_BASE        (0x10000000)
#define INFRACFG_AO_BASE  (0x10001000)
#define AUDIO_BASE        (0x11220000)
#define MFGCFG_BASE       (0x13000000)
#define MMSYS_CONFIG_BASE (0x14000000)
#define IMGSYS_BASE       (0x15000000)
#define VDEC_GCON_BASE    (0x16000000)
#define VENC_GCON_BASE    (0x17000000)


/* APMIXEDSYS Register */
#define AP_PLL_CON0             (APMIXED_BASE + 0x00)
#define AP_PLL_CON1             (APMIXED_BASE + 0x04)
#define AP_PLL_CON2             (APMIXED_BASE + 0x08)
#define AP_PLL_CON3             (APMIXED_BASE + 0x0C)
#define AP_PLL_CON4             (APMIXED_BASE + 0x10)
#define AP_PLL_CON5             (APMIXED_BASE + 0x14)
#define AP_PLL_CON6             (APMIXED_BASE + 0x18)
#define AP_PLL_CON7             (APMIXED_BASE + 0x1C)
#define CLKSQ_STB_CON0          (APMIXED_BASE + 0x20)
#define PLL_PWR_CON0            (APMIXED_BASE + 0x24)
#define PLL_PWR_CON1            (APMIXED_BASE + 0x28)
#define PLL_ISO_CON0            (APMIXED_BASE + 0x2C)
#define PLL_ISO_CON1            (APMIXED_BASE + 0x30)
#define PLL_STB_CON0            (APMIXED_BASE + 0x34)
#define DIV_STB_CON0            (APMIXED_BASE + 0x38)
#define PLL_CHG_CON0            (APMIXED_BASE + 0x3C)
#define PLL_TEST_CON0           (APMIXED_BASE + 0x40)

#define ARMPLL_LL_CON0          (APMIXED_BASE + 0x200)
#define ARMPLL_LL_CON1          (APMIXED_BASE + 0x204)
#define ARMPLL_LL_CON2          (APMIXED_BASE + 0x208)
#define ARMPLL_LL_PWR_CON0      (APMIXED_BASE + 0x20C)
#define ARMPLL_L_CON0          (APMIXED_BASE + 0x210)
#define ARMPLL_L_CON1          (APMIXED_BASE + 0x214)
#define ARMPLL_L_CON2          (APMIXED_BASE + 0x218)
#define ARMPLL_L_PWR_CON0      (APMIXED_BASE + 0x21C)
                                                
#define MAINPLL_CON0            (APMIXED_BASE + 0x220)
#define MAINPLL_CON1            (APMIXED_BASE + 0x224)
#define MAINPLL_PWR_CON0        (APMIXED_BASE + 0x22C)
                                                
#define UNIVPLL_CON0            (APMIXED_BASE + 0x230)
#define UNIVPLL_CON1            (APMIXED_BASE + 0x234)
#define UNIVPLL_PWR_CON0        (APMIXED_BASE + 0x23C)
                                                
#define MMPLL_CON0              (APMIXED_BASE + 0x240)
#define MMPLL_CON1              (APMIXED_BASE + 0x244)
#define MMPLL_CON2              (APMIXED_BASE + 0x248)
#define MMPLL_PWR_CON0          (APMIXED_BASE + 0x24C)
                                                
#define MSDCPLL_CON0            (APMIXED_BASE + 0x250)
#define MSDCPLL_CON1            (APMIXED_BASE + 0x254)
#define MSDCPLL_PWR_CON0        (APMIXED_BASE + 0x25C)
                                                
#define VENCPLL_CON0            (APMIXED_BASE + 0x260)
#define VENCPLL_CON1            (APMIXED_BASE + 0x264)
#define VENCPLL_PWR_CON0        (APMIXED_BASE + 0x26C)
                                                
#define TVDPLL_CON0             (APMIXED_BASE + 0x270)
#define TVDPLL_CON1             (APMIXED_BASE + 0x274)
#define TVDPLL_PWR_CON0         (APMIXED_BASE + 0x27C)
                                                
#define MPLL_CON0               (APMIXED_BASE + 0x280)
#define MPLL_CON1               (APMIXED_BASE + 0x284)
#define MPLL_PWR_CON0           (APMIXED_BASE + 0x28C)

#define CCIPLL_CON0          (APMIXED_BASE + 0x290)
#define CCIPLL_CON1          (APMIXED_BASE + 0x294)
#define CCIPLL_PWR_CON0      (APMIXED_BASE + 0x29C)
                                                
#define APLL1_CON0              (APMIXED_BASE + 0x2A0)
#define APLL1_CON1              (APMIXED_BASE + 0x2A4)
#define APLL1_CON2              (APMIXED_BASE + 0x2A8)
#define APLL1_CON3              (APMIXED_BASE + 0x2AC)
#define APLL1_PWR_CON0          (APMIXED_BASE + 0x2B0)
                                                
#define APLL2_CON0              (APMIXED_BASE + 0x2B4)
#define APLL2_CON1              (APMIXED_BASE + 0x2B8)
#define APLL2_CON2              (APMIXED_BASE + 0x2BC)
#define APLL2_CON3              (APMIXED_BASE + 0x2C0)
#define APLL2_PWR_CON0          (APMIXED_BASE + 0x2C4)
                                               
#define AP_AUXADC_CON0          (APMIXED_BASE + 0x400)
#define AP_AUXADC_CON1          (APMIXED_BASE + 0x404)
#define TS_CON0                 (APMIXED_BASE + 0x600)
#define TS_CON1                 (APMIXED_BASE + 0x604)
#define AP_ABIST_MON_CON0       (APMIXED_BASE + 0x800)
#define AP_ABIST_MON_CON1       (APMIXED_BASE + 0x804)
#define AP_ABIST_MON_CON2       (APMIXED_BASE + 0x808)
#define AP_ABIST_MON_CON3       (APMIXED_BASE + 0x80C)
#define OCCSCAN_CON             (APMIXED_BASE + 0x810)
#define CLKDIV_CON0             (APMIXED_BASE + 0x814)

/* MCUCFG Register */

/* TOPCKGEN Register */
#define CLK_MODE                (CKSYS_BASE + 0x000)
#define CLK_CFG_UPDATE          (CKSYS_BASE + 0x004)
#define CLK_CFG_UPDATE1         (CKSYS_BASE + 0x008)
#define CLK_CFG_0               (CKSYS_BASE + 0x040)
#define CLK_CFG_1               (CKSYS_BASE + 0x050)
#define CLK_CFG_2               (CKSYS_BASE + 0x060)
#define CLK_CFG_3               (CKSYS_BASE + 0x070)
#define CLK_CFG_4               (CKSYS_BASE + 0x080)
#define CLK_CFG_5               (CKSYS_BASE + 0x090)
#define CLK_CFG_6               (CKSYS_BASE + 0x0A0) 
#define CLK_CFG_7               (CKSYS_BASE + 0x0B0) 
#define CLK_CFG_8               (CKSYS_BASE + 0x0C0) 
#define CLK_CFG_9               (CKSYS_BASE + 0x0D0)
#define CLK_MISC_CFG_0          (CKSYS_BASE + 0x104)
#define CLK_DBG_CFG             (CKSYS_BASE + 0x10C)
#define CLK_SCP_CFG_0           (CKSYS_BASE + 0x200)
#define CLK_SCP_CFG_1           (CKSYS_BASE + 0x204)
#define CLK26CALI_0             (CKSYS_BASE + 0x220)
#define CLK26CALI_1             (CKSYS_BASE + 0x224)


/* INFRASYS Register */
#define TOP_CKMUXSEL            (INFRACFG_AO_BASE + 0x00)
#define TOP_CKDIV1              (INFRACFG_AO_BASE + 0x08)
#define TOP_DCMCTL              (INFRACFG_AO_BASE + 0x10)
#define TOP_CKDIV1_BIG          (INFRACFG_AO_BASE + 0x24)
#define TOP_CKDIV1_SML          (INFRACFG_AO_BASE + 0x28)
#define TOP_CKDIV1_BUS          (INFRACFG_AO_BASE + 0x2C)

#define INFRA_BUS_DCM_CTRL      (INFRACFG_AO_BASE + 0x70)
#define PERI_BUS_DCM_CTRL       (INFRACFG_AO_BASE + 0x74)

#define INFRA_TOPAXI_PROTECTEN   (INFRACFG_AO_BASE + 0x0220)
#define INFRA_TOPAXI_PROTECTSTA1 (INFRACFG_AO_BASE + 0x0228) 
#define INFRA_TOPAXI_PROTECTEN_1   (INFRACFG_AO_BASE + 0x0250)
#define INFRA_TOPAXI_PROTECTSTA1_1 (INFRACFG_AO_BASE + 0x0258)
#define INFRA_PLL_ULPOSC_CON0          (INFRACFG_AO_BASE + 0xB00)
#define INFRA_PLL_ULPOSC_CON1          (INFRACFG_AO_BASE + 0xB04)

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
/* Audio Register*/
#define AUDIO_TOP_CON0          (AUDIO_BASE + 0x0000)
                                
/* MFGCFG Register*/            
#define MFG_CG_CON              (MFGCFG_BASE + 0)
#define MFG_CG_SET              (MFGCFG_BASE + 4)
#define MFG_CG_CLR              (MFGCFG_BASE + 8)

/* MMSYS Register*/
#define DISP_CG_CON0            (MMSYS_CONFIG_BASE + 0x100)
#define DISP_CG_SET0            (MMSYS_CONFIG_BASE + 0x104)
#define DISP_CG_CLR0            (MMSYS_CONFIG_BASE + 0x108)
#define DISP_CG_CON1            (MMSYS_CONFIG_BASE + 0x110)
#define DISP_CG_SET1            (MMSYS_CONFIG_BASE + 0x114)
#define DISP_CG_CLR1            (MMSYS_CONFIG_BASE + 0x118)

/* IMGSYS Register */
#define IMG_CG_CON              (IMGSYS_BASE + 0x0000)
#define IMG_CG_SET              (IMGSYS_BASE + 0x0004)
#define IMG_CG_CLR              (IMGSYS_BASE + 0x0008)

/* VDEC Register */                                
#define VDEC_CKEN_SET           (VDEC_GCON_BASE + 0x0000)
#define VDEC_CKEN_CLR           (VDEC_GCON_BASE + 0x0004)
#define LARB_CKEN_SET           (VDEC_GCON_BASE + 0x0008)
#define LARB_CKEN_CLR           (VDEC_GCON_BASE + 0x000C)

/* VENC Register*/
#define VENC_CG_CON             (VENC_GCON_BASE + 0x0)
#define VENC_CG_SET             (VENC_GCON_BASE + 0x4)
#define VENC_CG_CLR             (VENC_GCON_BASE + 0x8)

#endif
