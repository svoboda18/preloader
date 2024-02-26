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

#ifndef PLL1_H
#define PLL1_H

#define APMIXED_BASE      (0x10209000)
#define CKSYS_BASE        (0x10210000)
#define INFRACFG_AO_BASE  (0x10000000)
#define PERICFG_BASE      (0x10002000)
#define AUDIO_BASE        (0x11220000)
#define MFGCFG_BASE       (0x13000000)
#define MMSYS_CONFIG_BASE (0x14000000)
#define IMGSYS_BASE       (0x15000000)
#define VDEC_GCON_BASE    (0x16000000)
#define VENC_GCON_BASE    (0x17000000)

/* MCUSS Register */
#define ACLKEN_DIV              (0x10200640)

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

#define ARMPLL_CON0             (APMIXED_BASE + 0x200)
#define ARMPLL_CON1             (APMIXED_BASE + 0x204)
#define ARMPLL_CON2             (APMIXED_BASE + 0x208)
#define ARMPLL_PWR_CON0         (APMIXED_BASE + 0x20C)

#define MAINPLL_CON0            (APMIXED_BASE + 0x210)
#define MAINPLL_CON1            (APMIXED_BASE + 0x214)
#define MAINPLL_PWR_CON0        (APMIXED_BASE + 0x21C)

#define UNIVPLL_CON0            (APMIXED_BASE + 0x220)
#define UNIVPLL_CON1            (APMIXED_BASE + 0x224)
#define UNIVPLL_PWR_CON0        (APMIXED_BASE + 0x22C)

#define MMPLL_CON0              (APMIXED_BASE + 0x230)
#define MMPLL_CON1              (APMIXED_BASE + 0x234)
#define MMPLL_CON2              (APMIXED_BASE + 0x238)
#define MMPLL_PWR_CON0          (APMIXED_BASE + 0x23C)

#define MSDCPLL_CON0            (APMIXED_BASE + 0x240)
#define MSDCPLL_CON1            (APMIXED_BASE + 0x244)
#define MSDCPLL_PWR_CON0        (APMIXED_BASE + 0x24C)

#define VENCPLL_CON0            (APMIXED_BASE + 0x250)
#define VENCPLL_CON1            (APMIXED_BASE + 0x254)
#define VENCPLL_PWR_CON0        (APMIXED_BASE + 0x25C)

#define TVDPLL_CON0             (APMIXED_BASE + 0x260)
#define TVDPLL_CON1             (APMIXED_BASE + 0x264)
#define TVDPLL_PWR_CON0         (APMIXED_BASE + 0x26C)

//#define MPLL_CON0               (clk_apmixed_base + 0x280)
//#define MPLL_CON1               (clk_apmixed_base + 0x284)
//#define MPLL_PWR_CON0           (clk_apmixed_base + 0x28C)

#define APLL1_CON0              (APMIXED_BASE + 0x270)
#define APLL1_CON1              (APMIXED_BASE + 0x274)
#define APLL1_CON2              (APMIXED_BASE + 0x278)
#define APLL1_CON3              (APMIXED_BASE + 0x27C)
#define APLL1_PWR_CON0          (APMIXED_BASE + 0x280)

#define APLL2_CON0              (APMIXED_BASE + 0x284)
#define APLL2_CON1              (APMIXED_BASE + 0x288)
#define APLL2_CON2              (APMIXED_BASE + 0x28C)
#define APLL2_CON3              (APMIXED_BASE + 0x290)
#define APLL2_PWR_CON0          (APMIXED_BASE + 0x294)
/*
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
*/
/* TOPCKGEN Register */
#define CLK_MODE                (CKSYS_BASE + 0x000)
#define TST_SEL_0               (CKSYS_BASE + 0x020)
#define TST_SEL_1               (CKSYS_BASE + 0x024)
#define CLK_CFG_0               (CKSYS_BASE + 0x040)
#define CLK_CFG_1               (CKSYS_BASE + 0x050)
#define CLK_CFG_2               (CKSYS_BASE + 0x060)
#define CLK_CFG_3               (CKSYS_BASE + 0x070)
#define CLK_CFG_4               (CKSYS_BASE + 0x080)
#define CLK_CFG_5               (CKSYS_BASE + 0x090)
#define CLK_CFG_6               (CKSYS_BASE + 0x0A0) 
#define CLK_CFG_7               (CKSYS_BASE + 0x0B0) 
#define CLK_CFG_8               (CKSYS_BASE + 0x100)
#define CLK_CFG_9               (CKSYS_BASE + 0x104)
#define CLK_CFG_10              (CKSYS_BASE + 0x108)
#define CLK_CFG_11              (CKSYS_BASE + 0x10C)
#define CLK_SCP_CFG_0           (CKSYS_BASE + 0x200)
#define CLK_SCP_CFG_1           (CKSYS_BASE + 0x204)
#define CLK_MISC_CFG_0          (CKSYS_BASE + 0x210)
#define CLK_MISC_CFG_1          (CKSYS_BASE + 0x214)
#define CLK_MISC_CFG_2          (CKSYS_BASE + 0x218)
#define CLK26CALI_0             (CKSYS_BASE + 0x220)
#define CLK26CALI_1             (CKSYS_BASE + 0x224)
#define CLK26CALI_2             (CKSYS_BASE + 0x228)
#define CKSTA_REG               (CKSYS_BASE + 0x22C)
#define MBIST_CFG_0             (CKSYS_BASE + 0x308)
#define MBIST_CFG_1             (CKSYS_BASE + 0x30C)

/* INFRASYS Register */
#define TOP_CKMUXSEL            (INFRACFG_AO_BASE + 0x00)
#define TOP_CKDIV1              (INFRACFG_AO_BASE + 0x08)
#define TOP_DCMCTL              (INFRACFG_AO_BASE + 0x10)

#define	INFRA_GLOBALCON_DCMCTL  (INFRACFG_AO_BASE + 0x050) //0x10000050
/** 0x10000050	INFRA_GLOBALCON_DCMCTL
 * 0	0	faxi_dcm_enable
 * 1	1	fmem_dcm_enable
 * 8	8	axi_clock_gated_en
 * 9	9	l2c_sram_infra_dcm_en
 **/
#define INFRA_GLOBALCON_DCMCTL_MASK     (0x00000303)
#define INFRA_GLOBALCON_DCMCTL_ON       (0x00000303)
#define INFRA_GLOBALCON_DCMDBC  (INFRACFG_AO_BASE + 0x054) //0x10000054
/** 0x10000054	INFRA_GLOBALCON_DCMDBC
 * 6	0	dcm_dbc_cnt (default 7'h7F)
 * 8	8	faxi_dcm_dbc_enable
 * 22	16	dcm_dbc_cnt_fmem (default 7'h7F)
 * 24	24	dcm_dbc_enable_fmem
 **/
#define INFRA_GLOBALCON_DCMDBC_MASK  ((0x7f<<0) | (1<<8) | (0x7f<<16) | (1<<24))
#define INFRA_GLOBALCON_DCMDBC_ON      ((0<<0) | (1<<8) | (0<<16) | (1<<24))
#define INFRA_GLOBALCON_DCMFSEL (INFRACFG_AO_BASE + 0x058) //0x10000058
/** 0x10000058	INFRA_GLOBALCON_DCMFSEL
 * 2	0	dcm_qtr_fsel ("1xx: 1/4, 01x: 1/8, 001: 1/16, 000: 1/32")
 * 11	8	dcm_half_fsel ("1xxx:1/2, 01xx: 1/4, 001x: 1/8, 0001: 1/16, 0000: 1/32")
 * 20	16	dcm_full_fsel ("1xxxx:1/1, 01xxx:1/2, 001xx: 1/4, 0001x: 1/8, 00001: 1/16, 00000: 1/32")
 * 28	24	dcm_full_fsel_fmem ("1xxxx:1/1, 01xxx:1/2, 001xx: 1/4, 0001x: 1/8, 00001: 1/16, 00000: 1/32")
 **/
#define INFRA_GLOBALCON_DCMFSEL_MASK ((0x7<<0) | (0xf<<8) | (0x1f<<16) | (0x1f<<24))
#define INFRA_GLOBALCON_DCMFSEL_ON ((0<<0) | (0<<8) | (0x10<<16) | (0x10<<24))

#define INFRA_PDN_SET0          (INFRACFG_AO_BASE + 0x0040)
#define INFRA_PDN_CLR0          (INFRACFG_AO_BASE + 0x0044)
#define INFRA_PDN_STA0          (INFRACFG_AO_BASE + 0x0048)

#define TOPAXI_PROT_EN          (INFRACFG_AO_BASE + 0x0220)
#define TOPAXI_PROT_STA1        (INFRACFG_AO_BASE + 0x0228)

#define PERI_PDN_SET0           (PERICFG_BASE + 0x0008)
#define PERI_PDN_CLR0           (PERICFG_BASE + 0x0010)
#define PERI_PDN_STA0           (PERICFG_BASE + 0x0018)

/* Audio Register*/
#define AUDIO_TOP_CON0          (AUDIO_BASE + 0x0000)
#define AUDIO_TOP_CON1          (AUDIO_BASE + 0x0004)

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

#define MMSYS_DUMMY             (MMSYS_CONFIG_BASE + 0x890)
//#define	SMI_LARB_BWL_EN_REG     (clk_mmsys_config_base + 0x21050)

/* IMGSYS Register */
#define IMG_CG_CON              (IMGSYS_BASE + 0x0000)
#define IMG_CG_SET              (IMGSYS_BASE + 0x0004)
#define IMG_CG_CLR              (IMGSYS_BASE + 0x0008)

/* VDEC Register */                                
#define VDEC_CKEN_SET           (VDEC_GCON_BASE + 0x0000)
#define VDEC_CKEN_CLR           (VDEC_GCON_BASE + 0x0004)
#define LARB_CKEN_SET           (VDEC_GCON_BASE + 0x0008)
#define LARB_CKEN_CLR           (VDEC_GCON_BASE + 0x000C)

/* MJC Register*/
//#define MJC_CG_CON              (clk_mjc_config_base + 0x0000)
//#define MJC_CG_SET              (clk_mjc_config_base + 0x0004)
//#define MJC_CG_CLR              (clk_mjc_config_base + 0x0008)

/* VENC Register*/
#define VENC_CG_CON             (VENC_GCON_BASE + 0x0)
#define VENC_CG_SET             (VENC_GCON_BASE + 0x4)
#define VENC_CG_CLR             (VENC_GCON_BASE + 0x8)

enum {
    PLL_MODE_1  = 1,
    PLL_MODE_2  = 2,
    PLL_MODE_3  = 3,
};
enum {
    DDR533   = 533,
    DDR800   = 800,
    DDR938   = 938,
    DDR1066  = 1066,
    DDR1333  = 1333,
    DDR1466  = 1466,
};

#if 0
#define DRAMC_ASYNC
#endif

#if 0
#define DDRPHY_3PLL_MODE
#endif


#ifdef DDRPHY_3PLL_MODE
#define PHYSYNC_MODE
#if 1
#define DDRPHY_2PLL
#endif
#endif

#if 0
#define DDR_533
#endif
#if 0
#define DDR_800
#endif
#if 1
#define DDR_1333
#endif


/* for MTCMOS */
/*
#define STA_POWER_DOWN  0
#define STA_POWER_ON    1

#define DIS_PWR_STA_MASK    (0x1 << 3)

#define PWR_RST_B           (0x1 << 0)
#define PWR_ISO             (0x1 << 1)
#define PWR_ON              (0x1 << 2)
#define PWR_ON_S            (0x1 << 3)
#define PWR_CLK_DIS         (0x1 << 4)

#define SRAM_PDN            (0xf << 8)
#define DIS_SRAM_ACK        (0x1 << 12)

#define MD1_PROT_MASK     0x04B8//bit 3,4,5,7,10
#define MD_SRAM_PDN         (0x1 << 8)
#define MD1_PWR_STA_MASK    (0x1 << 0)

int spm_mtcmos_ctrl_disp(int state);
*/
#endif
