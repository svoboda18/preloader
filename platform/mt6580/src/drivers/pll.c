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

#include "typedefs.h"
#include "platform.h"

#include "pll.h"
#include "timer.h"
#include "spm.h"
#include "spm_mtcmos.h"
#include "dramc.h"
#include "wdt.h"
#include "emi.h"

#include "../security/inc/sec_devinfo.h"

/* re-defined
#define DRAMC_WRITE_REG(val,offset) \
        do { \
            (*(volatile unsigned int *)(DRAMC0_BASE + (offset))) = (unsigned int)(val); \
            (*(volatile unsigned int *)(DDRPHY_BASE + (offset))) = (unsigned int)(val); \
        } while(0)*/


/***********************
 * MEMPLL Configuration
 ***********************/
#define r_bias_en_stb_time            (0x00000000 << 24)  //170[31:24]
#define r_bias_lpf_en_stb_time        (0x00000000 << 16)  //170[23:16]
#define r_mempll_en_stb_time          (0x00000000 << 8)   //170[15:8]
#define r_dmall_ck_en_stb_time        (0x00000000 << 0)   //170[7:0]

#define r_dds_en_stb_time             (0x00000000 << 24)  //171[31:24]
#define r_div_en_stb_time             (0x00000000 << 16)  //171[23:16]
#define r_dmpll2_ck_en_stb_time       (0x00000000 << 8)   //171[15:8]
#define r_iso_en_stb_time             (0x00000000 << 0)   //171[7:0]

#define r_bias_en_stb_dis             (0x00000001 << 28)  //172[28]
#define r_bias_en_src_sel             (0x00000001 << 24)  //172[24]
#define r_bias_lpf_en_stb_dis         (0x00000001 << 20)  //172[20]
#define r_bias_lpf_en_src_sel         (0x00000001 << 16)  //172[16]
#define r_mempll4_en_stb_dis          (0x00000001 << 15)  //172[15]
#define r_mempll3_en_stb_dis          (0x00000001 << 14)  //172[14]
#define r_mempll2_en_stb_dis          (0x00000001 << 13)  //172[13]
#define r_mempll_en_stb_dis           (0x00000001 << 12)  //172[12]
#define r_mempll4_en_src_sel          (0x00000001 << 11)  //172[11]
#define r_mempll3_en_src_sel          (0x00000001 << 10)  //172[10]
#define r_mempll2_en_src_sel          (0x00000001 << 9)   //172[9]
#define r_mempll_en_src_sel           (0x00000001 << 8)   //172[8]
#define r_dmall_ck_en_stb_dis         (0x00000001 << 4)   //172[4]
#define r_dmall_ck_en_src_sel         (0x00000001 << 0)   //172[0]

#define r_dds_en_stb_dis              (0x00000001 << 28)  //173[28]
#define r_dds_en_src_sel              (0x00000001 << 24)  //173[24]
#define r_div_en_stb_dis              (0x00000001 << 20)  //173[20]
#define r_div_en_src_sel              (0x00000001 << 16)  //173[16]
#define r_dmpll2_ck_en_stb_dis        (0x00000001 << 12)  //173[12]
#define r_dmpll2_ck_en_src_sel        (0x00000001 << 8)   //173[8]
#define r_iso_en_stb_dis              (0x00000001 << 4)   //173[4]
#define r_iso_en_src_sel              (0x00000001 << 0)   //173[0]

#define r_dmbyp_pll4                  (0x00000001 << 0)   //190[0]
#define r_dmbyp_pll3                  (0x00000001 << 1)   //190[1]
#define r_dm1pll_sync_mode            (0x00000001 << 2)   //190[2]
#define r_dmall_ck_en                 (0x00000001 << 4)   //190[4]
#define r_dmpll2_clk_en               (0x00000001 << 5)   //190[5]

#define pllc1_postdiv_1_0             (0x00000003 << 14)  //180[15:14]
#define pllc1_blp                     (0x00000001 << 12)  //180[12]
#define pllc1_mempll_n_info_chg       (0x00000001 << 0)   //189[0]
#define pllc1_dmss_pcw_ncpo_30_0      (0x7fffffff << 1)   //189[31:1]
#define pllc1_mempll_div_en           (0x00000001 <<24)   //181[24]
#define pllc1_mempll_div_6_0          (0x0000007f <<25)   //181[31:25]
#define pllc1_mempll_reserve_2        (0x00000001 <<18)   //181[18]
#define pllc1_mempll_top_reserve_2_0  (0x00000000 <<16)   //182[18:16]
#define pllc1_mempll_bias_en          (0x00000001 <<14)   //181[14]
#define pllc1_mempll_bias_lpf_en      (0x00000001 <<15)   //181[15]
#define pllc1_mempll_en               (0x00000001 << 2)   //180[2]
#define pllc1_mempll_sdm_prd_1        (0x00000001 <<11)   //188[11]

#define mempll2_prediv_1_0            (0x00000000 << 0)   //182[1:0]
#define mempll2_vco_div_sel           (0x00000001 <<29)   //183[29]
#define mempll2_m4pdiv_1_0            (0x00000003 <<10)   //183[11:10],P9
#define mempll2_fbdiv_6_0             (0x0000007f << 2)   //182[8:2],P6
#define mempll2_fb_mck_sel            (0x00000001 << 9)   //183[9]
#define mempll2_fbksel_1_0            (0x00000003 <<10)   //182[11:10]
#define mempll2_bp_br                 (0x00000003 <<26)   //183[27:26]
#define mempll2_posdiv_1_0            (0x00000000 <<30)   //183[31:30]
#define mempll2_ref_dl_4_0            (0x00000000 <<27)   //184[31:27]
#define mempll2_fb_dl_4_0             (0x00000000 <<22)   //184[26:22]
#define mempll2_en                    (0x00000001 <<18)   //183[18]

#define mempll3_prediv_1_0            (0x00000000 << 0)   //184[1:0]
#define mempll3_vco_div_sel           (0x00000001 <<29)   //185[29]
#define mempll3_m4pdiv_1_0            (0x00000003 <<10)   //185[11:10]
#define mempll3_fbdiv_6_0             (0x0000007f << 2)   //184[8:2]
#define mempll3_bp_br                 (0x00000003 <<26)   //185[27:26]
#define mempll3_fb_mck_sel            (0x00000001 << 9)   //185[9]
#define mempll3_fbksel_1_0            (0x00000003 <<10)   //184[11:10]
#define mempll3_posdiv_1_0            (0x00000000 <<30)   //185[31:30]
#define mempll3_ref_dl_4_0            (0x00000000 <<27)   //186[31:27]
#define mempll3_fb_dl_4_0             (0x00000000 <<22)   //186[26:22]
#define mempll3_en                    (0x00000001 <<18)   //185[18]

#define mempll4_prediv_1_0            (0x00000000 << 0)   //186[1:0]
#define mempll4_vco_div_sel           (0x00000001 <<29)   //187[29]
#define mempll4_m4pdiv_1_0            (0x00000003 <<10)   //187[11:10]
#define mempll4_fbdiv_6_0             (0x0000007f << 2)   //186[8:2]
#define mempll4_fbksel_1_0            (0x00000003 <<10)   //186[11:10]
#define mempll4_bp_br                 (0x00000003 <<26)   //187[27:26]
#define mempll4_fb_mck_sel            (0x00000001 << 9)   //187[9]
#define mempll4_posdiv_1_0            (0x00000000 <<30)   //187[31:30]
#define mempll4_ref_dl_4_0            (0x00000000 <<27)   //188[31:27]
#define mempll4_fb_dl_4_0             (0x00000000 <<22)   //188[26:22]
#define mempll4_en                    (0x00000001 <<18)   //187[18]

/***********************
 * MEMPLL Calibration
 ***********************/
#define MEMPLL_JMETER_CNT               1024
#define MEMPLL_JMETER_CONFIDENCE_CNT    (MEMPLL_JMETER_CNT/10)
#define MEMPLL_JMETER_WAIT_TIME_BASE    10 // time base
#define MEMPLL_JMETER_WAIT_TIMEOUT      1000/MEMPLL_JMETER_WAIT_TIME_BASE // timeout ~ 1000us

void mt_mempll_init(int type, int pll_mode)
{
    unsigned int temp;

    /*********************************
    * (1) Setup DDRPHY operation mode
    **********************************/

    *((V_UINT32P)(DRAMC0_BASE + 0x007c)) |= 0x00000001; //DFREQ_DIV2=1
    *((V_UINT32P)(DDRPHY_BASE + 0x007c)) |= 0x00000001;

    if (pll_mode == PLL_MODE_3)
    {
        //*((V_UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00000020;    //3PLL sync mode, OK
        // [16] R_MEMPLL2_FB_M_CK_CG_EN -> for 2/3 PLL, feedback clock need to be ON
        *((V_UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00010020;    //3PLL sync mode, OK
    }
    else if (pll_mode== PLL_MODE_2)
    {
        //*((V_UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00000022;
        // [16] R_MEMPLL2_FB_M_CK_CG_EN -> for 2/3 PLL, feedback clock need to be ON
        *((V_UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00010022;
    }
    else // 1 PLL mode
    {
        *((V_UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00000007;    //1PLL sync mode, OK.
    }

    /*****************************************************************************************
    * (2) Setup MEMPLL operation case & frequency. May set according to dram type & frequency
    ******************************************************************************************/
    *((V_UINT32P)(DDRPHY_BASE + (0x0170 <<2))) = r_bias_en_stb_time | r_bias_lpf_en_stb_time | r_mempll_en_stb_time | r_dmall_ck_en_stb_time;
    *((V_UINT32P)(DDRPHY_BASE + (0x0171 <<2))) = r_dds_en_stb_time | r_div_en_stb_time | r_dmpll2_ck_en_stb_time | r_iso_en_stb_time;
    *((V_UINT32P)(DDRPHY_BASE + (0x0172 <<2))) = r_bias_en_stb_dis| r_bias_en_src_sel | r_bias_lpf_en_stb_dis| r_bias_lpf_en_src_sel | r_mempll4_en_stb_dis| r_mempll3_en_stb_dis| r_mempll2_en_stb_dis| r_mempll_en_stb_dis| r_mempll4_en_src_sel | r_mempll3_en_src_sel | r_mempll2_en_src_sel | r_mempll_en_src_sel | r_dmall_ck_en_stb_dis | r_dmall_ck_en_src_sel;
    *((V_UINT32P)(DDRPHY_BASE + (0x0173 <<2))) = r_dds_en_stb_dis| r_dds_en_src_sel | r_div_en_stb_dis| r_div_en_src_sel | r_dmpll2_ck_en_stb_dis| r_dmpll2_ck_en_src_sel | r_iso_en_stb_dis| r_iso_en_src_sel;

    // MEMPLL common setting
    *((V_UINT32P)(DDRPHY_BASE + (0x0180 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0180 <<2))) & (~pllc1_postdiv_1_0)) | 0x00000000; //RG_MEMPLL_POSDIV[1:0] = 2'b00;
    *((V_UINT32P)(DDRPHY_BASE + (0x0180 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0180 <<2))) & (~pllc1_blp)) | (0x00000001 << 12); //RG_MEMPLL_BLP = 1'b1;
    *((V_UINT32P)(DDRPHY_BASE + (0x0181 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0181 <<2))) & (~pllc1_mempll_div_6_0)) | (0x00000052 << 25); //RG_MEMPLL_DIV = 7'h52;
    *((V_UINT32P)(DDRPHY_BASE + (0x0181 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0181 <<2))) & (~pllc1_mempll_reserve_2)) | (0x00000001 << 18); //RG_MEMPLL_RESERVE[2] = 1;

    *((V_UINT32P)(DDRPHY_BASE + (0x0182 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0182 <<2))) & (~mempll2_fbksel_1_0)) | 0x00000000; //RG_MEMPLL2_FBKSEL[1:0] = 2'b00;
    *((V_UINT32P)(DDRPHY_BASE + (0x0183 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0183 <<2))) & (~mempll2_bp_br)) | (0x00000003 << 26); //RG_MEMPLL2_BP = 1, RG_MEMPLL2_BR=1;

    if ((pll_mode == PLL_MODE_3) || (pll_mode == PLL_MODE_2))
    {
        *((V_UINT32P)(DDRPHY_BASE + (0x0183 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0183 <<2))) & (~mempll2_fb_mck_sel)) | (0x00000001 << 9); //RG_MEMPLL2_FB_MCK_SEL;
    }

    *((V_UINT32P)(DDRPHY_BASE + (0x0184 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0184 <<2))) & (~mempll3_fbksel_1_0)) | 0x00000000; //RG_MEMPLL3_FBKSEL = 2'b00;
    *((V_UINT32P)(DDRPHY_BASE + (0x0185 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0185 <<2))) & (~mempll3_bp_br)) | (0x00000003 << 26); //RG_MEMPLL3_BP = 1, RG_MEMPLL3_BR=1;

    if ((pll_mode == PLL_MODE_3) || (pll_mode == PLL_MODE_2))
    {
        *((V_UINT32P)(DDRPHY_BASE + (0x0185 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0185 <<2))) & (~mempll3_fb_mck_sel)) | (0x00000001 << 9); //RG_MEMPLL3_FB_MCK_SEL = 1;
    }

    *((V_UINT32P)(DDRPHY_BASE + (0x0186 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0186 <<2))) & (~mempll4_fbksel_1_0)) | 0x00000000; //RG_MEMPLL4_FBKSEL = 2'b00;
    *((V_UINT32P)(DDRPHY_BASE + (0x0187 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0187 <<2))) & (~mempll4_bp_br)) | (0x00000003 << 26); //RG_MEMPLL4_BP = 1, RG_MEMPLL4_BR=1;

    if ((pll_mode == PLL_MODE_3) || (pll_mode == PLL_MODE_2))
    {
        *((V_UINT32P)(DDRPHY_BASE + (0x0187 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0187 <<2))) & (~mempll4_fb_mck_sel)) | (0x00000001 << 9); //RG_MEMPLL4_FB_MCK_SEL = 1;
    }

    //MEMPLL different setting for different frequency begin
    if (type == DDR1333) // real DDR-1280 (sign-off)
    {
        *((V_UINT32P)(DDRPHY_BASE + (0x0183 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0183 <<2))) & (~mempll2_vco_div_sel)) | 0x00000000; //RG_MEMPLL2_VCO_DIV_SEL =0;
        *((V_UINT32P)(DDRPHY_BASE + (0x0185 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0185 <<2))) & (~mempll3_vco_div_sel)) | 0x00000000; //RG_MEMPLL3_VCO_DIV_SEL =0;
        *((V_UINT32P)(DDRPHY_BASE + (0x0187 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0187 <<2))) & (~mempll4_vco_div_sel)) | 0x00000000; //RG_MEMPLL4_VCO_DIV_SEL =0;
        // DDR-1333
        //*((V_UINT32P)(DDRPHY_BASE + (0x0189 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0189 <<2))) & (~pllc1_dmss_pcw_ncpo_30_0)) | (0x50d8fe7c << 1); //RG_DMSS_PCW_NCPO[30:0]
        // DDR-1280
        *((V_UINT32P)(DDRPHY_BASE + (0x0189 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0189 <<2))) & (~pllc1_dmss_pcw_ncpo_30_0)) | (0x4da21535 << 1); //RG_DMSS_PCW_NCPO[30:0]
        // DDR-1280 -8%
        //*((V_UINT32P)(DDRPHY_BASE + (0x0189 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0189 <<2))) & (~pllc1_dmss_pcw_ncpo_30_0)) | (0x4762d719 << 1); //RG_DMSS_PCW_NCPO[30:0]

        if ((pll_mode == PLL_MODE_3) || (pll_mode == PLL_MODE_2))
        {
            *((V_UINT32P)(DDRPHY_BASE + (0x0182 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0182 <<2))) & (~mempll2_fbdiv_6_0)) | (0x0000000d << 2); //RG_MEMPLL2_FBDIV = 7'h0d;
            *((V_UINT32P)(DDRPHY_BASE + (0x0184 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0184 <<2))) & (~mempll3_fbdiv_6_0)) | (0x0000000d << 2); //RG_MEMPLL3_FBDIV = 7'h0d;
            *((V_UINT32P)(DDRPHY_BASE + (0x0186 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0186 <<2))) & (~mempll4_fbdiv_6_0)) | (0x0000000d << 2); //RG_MEMPLL4_FBDIV = 7'h0d;
        }
        else
        {
            *((V_UINT32P)(DDRPHY_BASE + (0x0182 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0182 <<2))) & (~mempll2_fbdiv_6_0)) | (0x00000034 << 2); //RG_MEMPLL2_FBDIV = 7'h34;
            *((V_UINT32P)(DDRPHY_BASE + (0x0184 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0184 <<2))) & (~mempll3_fbdiv_6_0)) | (0x00000034 << 2); //RG_MEMPLL3_FBDIV = 7'h34;
            *((V_UINT32P)(DDRPHY_BASE + (0x0186 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0186 <<2))) & (~mempll4_fbdiv_6_0)) | (0x00000034 << 2); //RG_MEMPLL4_FBDIV = 7'h34;
        }
    }
    else if (type == DDR938) // for DVFS_low (DVS HQA), the same settings as DDR-1333 other than NCPO
    {
        *((V_UINT32P)(DDRPHY_BASE + (0x0183 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0183 <<2))) & (~mempll2_vco_div_sel)) | 0x00000000; //RG_MEMPLL2_VCO_DIV_SEL =0;
        *((V_UINT32P)(DDRPHY_BASE + (0x0185 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0185 <<2))) & (~mempll3_vco_div_sel)) | 0x00000000; //RG_MEMPLL3_VCO_DIV_SEL =0;
        *((V_UINT32P)(DDRPHY_BASE + (0x0187 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0187 <<2))) & (~mempll4_vco_div_sel)) | 0x00000000; //RG_MEMPLL4_VCO_DIV_SEL =0;
    #if 1
        *((V_UINT32P)(DDRPHY_BASE + (0x0189 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0189 <<2))) & (~pllc1_dmss_pcw_ncpo_30_0)) | (0x38e3f9f0 << 1); //RG_DMSS_PCW_NCPO[30:0]
    #else // only for debug (lowest bring up frequency DDR-667)
        *((V_UINT32P)(DDRPHY_BASE + (0x0189 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0189 <<2))) & (~pllc1_dmss_pcw_ncpo_30_0)) | (0x287442a6 << 1); //RG_DMSS_PCW_NCPO[30:0]
    #endif

        if ((pll_mode == PLL_MODE_3) || (pll_mode == PLL_MODE_2))
        {
            *((V_UINT32P)(DDRPHY_BASE + (0x0182 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0182 <<2))) & (~mempll2_fbdiv_6_0)) | (0x0000000d << 2); //RG_MEMPLL2_FBDIV = 7'h0d;
            *((V_UINT32P)(DDRPHY_BASE + (0x0184 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0184 <<2))) & (~mempll3_fbdiv_6_0)) | (0x0000000d << 2); //RG_MEMPLL3_FBDIV = 7'h0d;
            *((V_UINT32P)(DDRPHY_BASE + (0x0186 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0186 <<2))) & (~mempll4_fbdiv_6_0)) | (0x0000000d << 2); //RG_MEMPLL4_FBDIV = 7'h0d;
        }
        else
        {
            *((V_UINT32P)(DDRPHY_BASE + (0x0182 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0182 <<2))) & (~mempll2_fbdiv_6_0)) | (0x00000034 << 2); //RG_MEMPLL2_FBDIV = 7'h34;
            *((V_UINT32P)(DDRPHY_BASE + (0x0184 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0184 <<2))) & (~mempll3_fbdiv_6_0)) | (0x00000034 << 2); //RG_MEMPLL3_FBDIV = 7'h34;
            *((V_UINT32P)(DDRPHY_BASE + (0x0186 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0186 <<2))) & (~mempll4_fbdiv_6_0)) | (0x00000034 << 2); //RG_MEMPLL4_FBDIV = 7'h34;
        }
    }
    else if (type == DDR900) // for DVFS_low, the same settings as DDR-1333 other than NCPO
    {
        *((V_UINT32P)(DDRPHY_BASE + (0x0183 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0183 <<2))) & (~mempll2_vco_div_sel)) | 0x00000000; //RG_MEMPLL2_VCO_DIV_SEL =0;
        *((V_UINT32P)(DDRPHY_BASE + (0x0185 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0185 <<2))) & (~mempll3_vco_div_sel)) | 0x00000000; //RG_MEMPLL3_VCO_DIV_SEL =0;
        *((V_UINT32P)(DDRPHY_BASE + (0x0187 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0187 <<2))) & (~mempll4_vco_div_sel)) | 0x00000000; //RG_MEMPLL4_VCO_DIV_SEL =0;
        *((V_UINT32P)(DDRPHY_BASE + (0x0189 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0189 <<2))) & (~pllc1_dmss_pcw_ncpo_30_0)) | (0x3695f6e9 << 1); //RG_DMSS_PCW_NCPO[30:0]

        if ((pll_mode == PLL_MODE_3) || (pll_mode == PLL_MODE_2))
        {
            *((V_UINT32P)(DDRPHY_BASE + (0x0182 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0182 <<2))) & (~mempll2_fbdiv_6_0)) | (0x0000000d << 2); //RG_MEMPLL2_FBDIV = 7'h0d;
            *((V_UINT32P)(DDRPHY_BASE + (0x0184 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0184 <<2))) & (~mempll3_fbdiv_6_0)) | (0x0000000d << 2); //RG_MEMPLL3_FBDIV = 7'h0d;
            *((V_UINT32P)(DDRPHY_BASE + (0x0186 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0186 <<2))) & (~mempll4_fbdiv_6_0)) | (0x0000000d << 2); //RG_MEMPLL4_FBDIV = 7'h0d;
        }
        else
        {
            *((V_UINT32P)(DDRPHY_BASE + (0x0182 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0182 <<2))) & (~mempll2_fbdiv_6_0)) | (0x00000034 << 2); //RG_MEMPLL2_FBDIV = 7'h34;
            *((V_UINT32P)(DDRPHY_BASE + (0x0184 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0184 <<2))) & (~mempll3_fbdiv_6_0)) | (0x00000034 << 2); //RG_MEMPLL3_FBDIV = 7'h34;
            *((V_UINT32P)(DDRPHY_BASE + (0x0186 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0186 <<2))) & (~mempll4_fbdiv_6_0)) | (0x00000034 << 2); //RG_MEMPLL4_FBDIV = 7'h34;
        }
    }
    else if (type == DDR1466)
    {
        *((V_UINT32P)(DDRPHY_BASE + (0x0183 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0183 <<2))) & (~mempll2_vco_div_sel)) | 0x00000000; //RG_MEMPLL2_VCO_DIV_SEL =0;
        *((V_UINT32P)(DDRPHY_BASE + (0x0185 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0185 <<2))) & (~mempll3_vco_div_sel)) | 0x00000000; //RG_MEMPLL3_VCO_DIV_SEL =0;
        *((V_UINT32P)(DDRPHY_BASE + (0x0187 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0187 <<2))) & (~mempll4_vco_div_sel)) | 0x00000000; //RG_MEMPLL4_VCO_DIV_SEL =0;
        *((V_UINT32P)(DDRPHY_BASE + (0x0189 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0189 <<2))) & (~pllc1_dmss_pcw_ncpo_30_0)) | (0x52902d02 << 1); //RG_DMSS_PCW_NCPO[30:0] = 31'h52902d02;
        if ((pll_mode == PLL_MODE_3) || (pll_mode == PLL_MODE_2))
        {
            *((V_UINT32P)(DDRPHY_BASE + (0x0182 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0182 <<2))) & (~mempll2_fbdiv_6_0)) | (0x0000000e << 2); //RG_MEMPLL2_FBDIV = 7'h0e;
            *((V_UINT32P)(DDRPHY_BASE + (0x0184 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0184 <<2))) & (~mempll3_fbdiv_6_0)) | (0x0000000e << 2); //RG_MEMPLL3_FBDIV = 7'h0e;
            *((V_UINT32P)(DDRPHY_BASE + (0x0186 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0186 <<2))) & (~mempll4_fbdiv_6_0)) | (0x0000000e << 2); //RG_MEMPLL4_FBDIV = 7'h0e;
        }
        else
        {
            *((V_UINT32P)(DDRPHY_BASE + (0x0182 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0182 <<2))) & (~mempll2_fbdiv_6_0)) | (0x00000038 << 2); //RG_MEMPLL2_FBDIV = 7'h38;
            *((V_UINT32P)(DDRPHY_BASE + (0x0184 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0184 <<2))) & (~mempll3_fbdiv_6_0)) | (0x00000038 << 2); //RG_MEMPLL3_FBDIV = 7'h38;
            *((V_UINT32P)(DDRPHY_BASE + (0x0186 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0186 <<2))) & (~mempll4_fbdiv_6_0)) | (0x00000038 << 2); //RG_MEMPLL4_FBDIV = 7'h38;
        }
    }
    else if (type == DDR800)// DDR-800, for DVFS_low, the same settings as DDR-1066 other than NCPO
    {
        *((V_UINT32P)(DDRPHY_BASE + (0x0189 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0189 <<2))) & (~pllc1_dmss_pcw_ncpo_30_0)) | (0x3957b864 << 1); //RG_DMSS_PCW_NCPO[30:0] = 31'h4c68b439;
        if ((pll_mode == PLL_MODE_3) || (pll_mode == PLL_MODE_2))
        {
            *((V_UINT32P)(DDRPHY_BASE + (0x0182 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0182 <<2))) & (~mempll2_fbdiv_6_0)) | (0x0000000b << 2); //RG_MEMPLL2_FBDIV = 7'h0b;
            *((V_UINT32P)(DDRPHY_BASE + (0x0184 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0184 <<2))) & (~mempll3_fbdiv_6_0)) | (0x0000000b << 2); //RG_MEMPLL3_FBDIV = 7'h0b;
            *((V_UINT32P)(DDRPHY_BASE + (0x0186 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0186 <<2))) & (~mempll4_fbdiv_6_0)) | (0x0000000b << 2); //RG_MEMPLL4_FBDIV = 7'h0b;
        }
        else
        {
            *((V_UINT32P)(DDRPHY_BASE + (0x0182 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0182 <<2))) & (~mempll2_fbdiv_6_0)) | (0x0000002c << 2); //RG_MEMPLL2_FBDIV = 7'h2c;
            *((V_UINT32P)(DDRPHY_BASE + (0x0184 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0184 <<2))) & (~mempll3_fbdiv_6_0)) | (0x0000002c << 2); //RG_MEMPLL3_FBDIV = 7'h2c;
            *((V_UINT32P)(DDRPHY_BASE + (0x0186 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0186 <<2))) & (~mempll4_fbdiv_6_0)) | (0x0000002c << 2); //RG_MEMPLL4_FBDIV = 7'h2c;
        }
    }
    else // DDR-1066
    {
        *((V_UINT32P)(DDRPHY_BASE + (0x0189 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0189 <<2))) & (~pllc1_dmss_pcw_ncpo_30_0)) | (0x4c68b439 << 1); //RG_DMSS_PCW_NCPO[30:0] = 31'h4c68b439;
        if ((pll_mode == PLL_MODE_3) || (pll_mode == PLL_MODE_2))
        {
            *((V_UINT32P)(DDRPHY_BASE + (0x0182 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0182 <<2))) & (~mempll2_fbdiv_6_0)) | (0x0000000b << 2); //RG_MEMPLL2_FBDIV = 7'h0b;
            *((V_UINT32P)(DDRPHY_BASE + (0x0184 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0184 <<2))) & (~mempll3_fbdiv_6_0)) | (0x0000000b << 2); //RG_MEMPLL3_FBDIV = 7'h0b;
            *((V_UINT32P)(DDRPHY_BASE + (0x0186 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0186 <<2))) & (~mempll4_fbdiv_6_0)) | (0x0000000b << 2); //RG_MEMPLL4_FBDIV = 7'h0b;
        }
        else
        {
            *((V_UINT32P)(DDRPHY_BASE + (0x0182 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0182 <<2))) & (~mempll2_fbdiv_6_0)) | (0x0000002c << 2); //RG_MEMPLL2_FBDIV = 7'h2c;
            *((V_UINT32P)(DDRPHY_BASE + (0x0184 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0184 <<2))) & (~mempll3_fbdiv_6_0)) | (0x0000002c << 2); //RG_MEMPLL3_FBDIV = 7'h2c;
            *((V_UINT32P)(DDRPHY_BASE + (0x0186 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0186 <<2))) & (~mempll4_fbdiv_6_0)) | (0x0000002c << 2); //RG_MEMPLL4_FBDIV = 7'h2c;
        }
    }
    //MEMPLL different setting for different frequency end

    if (pll_mode == PLL_MODE_2)
    {
        //*((V_UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00000022  | r_dmpll2_clk_en;
        *((V_UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00010022  | r_dmpll2_clk_en;
    }
    else if (pll_mode == PLL_MODE_3)
    {
        //*((V_UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00000020 | r_dmpll2_clk_en;    //3PLL sync mode
        *((V_UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00010020 | r_dmpll2_clk_en;    //3PLL sync mode
    }
    else // 1-PLL mode
    {
        //*((V_UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00000007 | r_dmpll2_clk_en;    //1PLL sync mode
        *((V_UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00000007 ;    //1PLL sync mode
    }

    /***********************************
    * (3) Setup MEMPLL power on sequence
    ************************************/

    gpt_busy_wait_us(2);

    *((V_UINT32P)(DDRPHY_BASE + (0x0181 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0181 <<2))) & (~pllc1_mempll_bias_en)) | (0x00000001 << 14); //RG_MEMPLL_BIAS_EN = 1'b1;

    gpt_busy_wait_us(2);

    *((V_UINT32P)(DDRPHY_BASE + (0x0181 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0181 <<2))) & (~pllc1_mempll_bias_lpf_en)) | (0x00000001 << 15); //RG_MEMPLL_BIAS_LPF_EN = 1'b1;

    gpt_busy_wait_us(1000);

    *((V_UINT32P)(DDRPHY_BASE + (0x0180 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0180 <<2))) & (~pllc1_mempll_en)) | (0x00000001 << 2); //RG_MEMPLL_EN = 1'b1;

    gpt_busy_wait_us(20);

    *((V_UINT32P)(DDRPHY_BASE + (0x0181 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0181 <<2))) & (~pllc1_mempll_div_en)) | (0x00000001 << 24); //RG_MEMPLL_DIV_EN = 1'b1;

    gpt_busy_wait_us(1);

    if ((pll_mode == PLL_MODE_3) || (pll_mode == PLL_MODE_2))
    {
        *((V_UINT32P)(DDRPHY_BASE + (0x0183 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0183 <<2))) & (~mempll2_en)) | (0x00000001 << 18); //RG_MEMPLL2_EN = 1'b1;
        *((V_UINT32P)(DDRPHY_BASE + (0x0185 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0185 <<2))) & (~mempll3_en)) | (0x00000001 << 18); //RG_MEMPLL3_EN = 1'b1;
        *((V_UINT32P)(DDRPHY_BASE + (0x0187 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0187 <<2))) & (~mempll4_en)) | (0x00000001 << 18); //RG_MEMPLL4_EN = 1'b1;
    }
    else
    {
        *((V_UINT32P)(DDRPHY_BASE + (0x0183 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0183 <<2))) & (~mempll2_en)) | (0x00000001 << 18); //RG_MEMPLL2_EN = 1'b1;
        *((V_UINT32P)(DDRPHY_BASE + (0x0185 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0185 <<2))) & (~mempll3_en)) | (0x00000000 << 18); //RG_MEMPLL3_EN = 1'b0;
        *((V_UINT32P)(DDRPHY_BASE + (0x0187 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0187 <<2))) & (~mempll4_en)) | (0x00000000 << 18); //RG_MEMPLL4_EN = 1'b0;
    }

    gpt_busy_wait_us(23);

    if (pll_mode == PLL_MODE_2)
    {
        //*((V_UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00000022  | r_dmpll2_clk_en | r_dmall_ck_en;
        *((V_UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00010022  | r_dmpll2_clk_en | r_dmall_ck_en;
    }
    else if (pll_mode == PLL_MODE_3)
    {
        //*((V_UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00000020 | r_dmpll2_clk_en | r_dmall_ck_en;    //3PLL sync mode
        *((V_UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00010020 | r_dmpll2_clk_en | r_dmall_ck_en;    //3PLL sync mode

    }
    else // 1-PLL mode
    {
        *((V_UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00000007 | r_dmpll2_clk_en | r_dmall_ck_en;    //1PLL sync mode
    }

    /**********************************
    * (4) MEMPLL control switch to SPM
    ***********************************/
#ifdef fcSWITCH_SPM_CONTROL
    if ((pll_mode == PLL_MODE_3) || (pll_mode == PLL_MODE_2))
    {
        *((V_UINT32P)(DDRPHY_BASE + (0x0173 <<2))) = 0x40101000;   //[0]ISO_EN_SRC=0,[22]DIV_EN_SC_SRC=0 (pll2off),[16]DIV_EN_SRC=0,[8]PLL2_CK_EN_SRC=1(1pll),[8]PLL2_CK_EN_SRC=0(3pll)
    }
    else // 1-PLL mode
    {
        *((V_UINT32P)(DDRPHY_BASE + (0x0173 <<2))) = 0x40101000;   //[0]ISO_EN_SRC=0,[22]DIV_EN_SC_SRC=0 (pll2off),[16]DIV_EN_SRC=0,[8]PLL2_CK_EN_SRC=1(1pll),[8]PLL2_CK_EN_SRC=0(3pll)
    }

    if ((pll_mode == PLL_MODE_3) || (pll_mode == PLL_MODE_2))
    {
        *((V_UINT32P)(DDRPHY_BASE + (0x0172 <<2))) = 0x0000F010;   //[24]BIAS_EN_SRC=0,[16]BIAS_LPF_EN_SRC=0,[8]MEMPLL_EN,[9][10][11]MEMPLL2,3,4_EN_SRC,[0]ALL_CK_EN_SRC=0
    }
    else // 1-PLL mode
    {
        *((V_UINT32P)(DDRPHY_BASE + (0x0172 <<2))) = 0x0000FC10;   //1PLL mode, MEMPLL3,4_EN no change to spm controller.eep to 1'b0 for power saving.
        //*((V_UINT32P)(DDRPHY_BASE + (0x0172 <<2))) = 0x0000F010; // sim ok
    }

    //*((V_UINT32P)(DDRPHY_BASE + (0x0170 <<2))) = 0x003C1B96;   //setting for delay time
    *((V_UINT32P)(DDRPHY_BASE + (0x0170 <<2))) = 0x063C0000;   //setting for delay time
#endif

// MEMPLL configuration end
}

#ifdef DDRPHY_3PLL_MODE

#if 0 // For JC debug
int mt_mempll_cali(void)
{
unsigned int temp = 0, ii, addr;
int one_count = 0, zero_count = 0;


    temp = DRV_Reg32(0x102131CC) & 0x0000FFFF;
    DRV_WriteReg32(0x102131CC, (temp | (MEMPLL_JMETER_CNT<<16))); // 0x102131CC[31:16] PLL2 0x400 = 1024 count

    temp = DRV_Reg32(0x102131D0) & 0x0000FFFF;
    DRV_WriteReg32(0x102131D0, (temp | (MEMPLL_JMETER_CNT<<16))); // 0x102131D0[31:16] PLL3 0x400 = 1024 count

    temp = DRV_Reg32(0x102131D4) & 0x0000FFFF;
    DRV_WriteReg32(0x102131D4, (temp | (MEMPLL_JMETER_CNT<<16))); // 0x102131D4[31:16] PLL4 0x400 = 1024 count

    for (ii=0; ii<64; ii++)
    {
        if (ii<32)
        {
        #if 1
            temp = DRV_Reg32(0x10213610) & ~0xF8000000;
            DRV_WriteReg32(0x10213610, (temp | (0x00 << 27 )));

            temp = DRV_Reg32(0x10213610) & ~0x07C00000;
            DRV_WriteReg32(0x10213610, (temp | (ii << 22)));
        #endif
        }
        else
        {
        #if 1
            temp = DRV_Reg32(0x10213610) & ~0xF8000000;
            DRV_WriteReg32(0x10213610, (temp | ((ii-32) << 27 )));

            temp = DRV_Reg32(0x10213610) & ~0x07C00000;
            DRV_WriteReg32(0x10213610, (temp | (0x00 << 22)));
        #endif
        }
        print("loop: %d, 0x10213610=0x%x\n", ii,DRV_Reg32(0x10213610) );
        gpt_busy_wait_us(20); // wait for external loop ready

        temp = DRV_Reg32(0x102131CC);
        DRV_WriteReg32(0x102131CC, temp | 0x1); // 0x102131CC[0]=1 PLL2
        temp = DRV_Reg32(0x102131D0);
        DRV_WriteReg32(0x102131D0, temp | 0x1); // 0x102131D0[0]=1 PLL3
        temp = DRV_Reg32(0x102131D4);
        DRV_WriteReg32(0x102131D4, temp | 0x1); // 0x102131D4[0]=1 PLL4

        gpt_busy_wait_us(80); // wait for jitter meter complete, 1/26MHz*1024

        one_count = DRV_Reg32(0x10213320) >> 16; // 0x10213320[31:16] PLL2 one count
        zero_count = DRV_Reg32(0x10213320) & 0x0000FFFF; // 0x10213320[15:0] PLL2 zero count
        print("PLL2 one_cnt/zero_cnt = %d/%d\n", one_count, zero_count);

        one_count = DRV_Reg32(0x10213324) >> 16; // 0x10213324[31:16] PLL3 one count
        zero_count = DRV_Reg32(0x10213324) & 0x0000FFFF; // 0x10213324[15:0] PLL3 zero count
        print("PLL3 one_cnt/zero_cnt = %d/%d\n", one_count, zero_count);

        one_count = DRV_Reg32(0x10213328) >> 16; // 0x10213328[31:16] PLL4 one count
        zero_count = DRV_Reg32(0x10213328) & 0x0000FFFF; // 0x10213328[15:0] PLL4 zero count
        print("PLL4 one_cnt/zero_cnt = %d/%d\n", one_count, zero_count);

        // disable jitter meter
        temp = DRV_Reg32(0x102131CC);
        DRV_WriteReg32(0x102131CC, temp & ~0x1); // 0x102131CC[0]=0 PLL2
        temp = DRV_Reg32(0x102131D0);
        DRV_WriteReg32(0x102131D0, temp & ~0x1); // 0x102131D0[0]=0 PLL3
        temp = DRV_Reg32(0x102131D4);
        DRV_WriteReg32(0x102131D4, temp & ~0x1); // 0x102131D4[0]=0 PLL4

        print("\n\n");
    }

    must_print("=====================DBG=====================\n");
    for(addr = 0x600; addr < 0x650; addr +=4){
        must_print("addr:%x, value:%x\n",addr, DRAMC_READ_REG(addr));
    }
    must_print("=============================================\n");
}

#else
int mt_mempll_cali(void)
{
    int one_count = 0, zero_count = 0;
    int pll2_done = 0, pll3_done = 0, pll4_done = 0, ret = 0;

    unsigned int temp = 0, pll2_dl = 0, pll3_dl = 0, pll4_dl = 0;
    int pll2_phase=0, pll3_phase=0, pll4_phase=0;
    unsigned int jmeter_wait_count;

    /***********************************************
    * 1. Set jitter meter clock to internal FB path
    ************************************************/
    //temp = DRV_Reg32(0x1021360C);
    //DRV_WriteReg32(0x1021360C, temp & ~0x200); // 0x1021360C[9] = 0 PLL2

    //temp = DRV_Reg32(0x10213614);
    //DRV_WriteReg32(0x10213614, temp & ~0x200); // 0x10213614[9] = 0 PLL3

    //temp = DRV_Reg32(0x1021361C);
    //DRV_WriteReg32(0x1021361C, temp & ~0x200); // 0x1021361C[9] = 0 PLL4

    /***********************************************
    * 2. Set jitter meter count number
    ************************************************/
    // PLL2 0x102131CC  0x1021332C[0]: JMETER_DONE
    // PLL3 0x102131D0  0x1021332C[1]: JMETER_DONE
    // PLL4 0x102131D4  0x1021332C[2]: JMETER_DONE

    temp = DRV_Reg32(0x102131CC) & 0x0000FFFF;
    DRV_WriteReg32(0x102131CC, (temp | (MEMPLL_JMETER_CNT<<16))); // 0x102131CC[31:16] PLL2 0x400 = 1024 count

    temp = DRV_Reg32(0x102131D0) & 0x0000FFFF;
    DRV_WriteReg32(0x102131D0, (temp | (MEMPLL_JMETER_CNT<<16))); // 0x102131D0[31:16] PLL3 0x400 = 1024 count

    temp = DRV_Reg32(0x102131D4) & 0x0000FFFF;
    DRV_WriteReg32(0x102131D4, (temp | (MEMPLL_JMETER_CNT<<16))); // 0x102131D4[31:16] PLL4 0x400 = 1024 count


    while(1)
    {
        /***********************************************
        * 3. Adjust delay chain tap number
        ************************************************/
        if (!pll2_done)
        {
            if (pll2_phase == 0)    // initial phase set to 0 for REF and FBK
            {
                temp = DRV_Reg32(0x10213610) & ~0xF8000000;
                DRV_WriteReg32(0x10213610, (temp | (0x00 << 27 )));

                temp = DRV_Reg32(0x10213610) & ~0x07C00000;
                DRV_WriteReg32(0x10213610, (temp | (0x00 << 22)));
            }
            else if (pll2_phase == 1)   // REF lag FBK, delay FBK
            {
                temp = DRV_Reg32(0x10213610) & ~0xF8000000;
                DRV_WriteReg32(0x10213610, (temp | (0x00 << 27 )));

                temp = DRV_Reg32(0x10213610) & ~0x07C00000;
                DRV_WriteReg32(0x10213610, (temp | (pll2_dl << 22)));
            }
            else   // REF lead FBK, delay REF
            {

                temp = DRV_Reg32(0x10213610) & ~0xF8000000;
                DRV_WriteReg32(0x10213610, (temp | (pll2_dl << 27 )));

                temp = DRV_Reg32(0x10213610) & ~0x07C00000;
                DRV_WriteReg32(0x10213610, (temp | (0x00 << 22)));
            }
        }

        if (!pll3_done)
        {
            if (pll3_phase == 0)    // initial phase set to 0 for REF and FBK
            {
                temp = DRV_Reg32(0x10213618) & ~0xF8000000;
                DRV_WriteReg32(0x10213618, (temp | (0x00 << 27 )));

                temp = DRV_Reg32(0x10213618) & ~0x07C00000;
                DRV_WriteReg32(0x10213618, (temp | (0x00 << 22)));
            }
            else if (pll3_phase == 1)   // REF lag FBK, delay FBK
            {
                temp = DRV_Reg32(0x10213618) & ~0xF8000000;
                DRV_WriteReg32(0x10213618, (temp | (0x00 << 27 )));

                temp = DRV_Reg32(0x10213618) & ~0x07C00000;
                DRV_WriteReg32(0x10213618, (temp | (pll3_dl << 22)));
            }
            else   // REF lead FBK, delay REF
            {
                temp = DRV_Reg32(0x10213618) & ~0xF8000000;
                DRV_WriteReg32(0x10213618, (temp | (pll3_dl << 27 )));

                temp = DRV_Reg32(0x10213618) & ~0x07C00000;
                DRV_WriteReg32(0x10213618, (temp | (0x00 << 22)));
            }
        }

        if (!pll4_done)
        {
            if (pll4_phase == 0)    // initial phase set to 0 for REF and FBK
            {
                temp = DRV_Reg32(0x10213620) & ~0xF8000000;
                DRV_WriteReg32(0x10213620, (temp | (0x00 << 27 )));

                temp = DRV_Reg32(0x10213620) & ~0x07C00000;
                DRV_WriteReg32(0x10213620, (temp | (0x00 << 22)));
            }
            else if (pll4_phase == 1)   // REF lag FBK, delay FBK
            {
                temp = DRV_Reg32(0x10213620) & ~0xF8000000;
                DRV_WriteReg32(0x10213620, (temp | (0x00 << 27 )));

                temp = DRV_Reg32(0x10213620) & ~0x07C00000;
                DRV_WriteReg32(0x10213620, (temp | (pll4_dl << 22)));
            }
            else   // REF lead FBK, delay REF
            {
                temp = DRV_Reg32(0x10213620) & ~0xF8000000;
                DRV_WriteReg32(0x10213620, (temp | (pll4_dl << 27 )));

                temp = DRV_Reg32(0x10213620) & ~0x07C00000;
                DRV_WriteReg32(0x10213620, (temp | (0x00 << 22)));
            }
        }

        gpt_busy_wait_us(20); // wait for external loop ready

        /***********************************************
        * 4. Enable jitter meter
        ************************************************/
        if (!pll2_done)
        {
            temp = DRV_Reg32(0x102131CC);
            DRV_WriteReg32(0x102131CC, temp | 0x1); // 0x102131CC[0]=1 PLL2
        }

        if (!pll3_done)
        {
            temp = DRV_Reg32(0x102131D0);
            DRV_WriteReg32(0x102131D0, temp | 0x1); // 0x102131D0[0]=1 PLL3
        }

        if (!pll4_done)
        {
            temp = DRV_Reg32(0x102131D4);
            DRV_WriteReg32(0x102131D4, temp | 0x1); // 0x102131D4[0]=1 PLL4
        }

        gpt_busy_wait_us(80); // wait for jitter meter complete, 1/26MHz*1024

/*
        jmeter_wait_count = 0;
        if (!pll2_done)
        {
            while (!(DRV_Reg32(0x1021332C)&0x00000001))
            {
                gpt_busy_wait_us(MEMPLL_JMETER_WAIT_TIME_BASE);
                jmeter_wait_count++;
                if (jmeter_wait_count>MEMPLL_JMETER_WAIT_TIMEOUT)
                {
                    mcSHOW_ERR_MSG(("[ERROR] PLL2 Jeter Meter Count Timeout > %d us!!\n", MEMPLL_JMETER_WAIT_TIME_BASE*MEMPLL_JMETER_WAIT_TIMEOUT));
                    break;
                }
            }
        }

        jmeter_wait_count = 0;
        if (!pll3_done)
        {
            while (!(DRV_Reg32(0x1021332C)&0x00000002))
            {
                gpt_busy_wait_us(MEMPLL_JMETER_WAIT_TIME_BASE);
                jmeter_wait_count++;
                if (jmeter_wait_count>MEMPLL_JMETER_WAIT_TIMEOUT)
                {
                    mcSHOW_ERR_MSG(("[ERROR] PLL3 Jeter Meter Count Timeout > %d us!!\n", MEMPLL_JMETER_WAIT_TIME_BASE*MEMPLL_JMETER_WAIT_TIMEOUT));
                    break;
                }
            }
        }

        jmeter_wait_count = 0;
        if (!pll4_done)
        {
            while (!(DRV_Reg32(0x1021332C)&0x00000004))
            {
                gpt_busy_wait_us(MEMPLL_JMETER_WAIT_TIME_BASE);
                jmeter_wait_count++;
                if (jmeter_wait_count>MEMPLL_JMETER_WAIT_TIMEOUT)
                {
                    mcSHOW_ERR_MSG(("[ERROR] PLL4 Jeter Meter Count Timeout > %d us!!\n", MEMPLL_JMETER_WAIT_TIME_BASE*MEMPLL_JMETER_WAIT_TIMEOUT));
                    break;
                }
            }
        }
*/

        /***********************************************
        * 5. Check jitter meter counter value
        ************************************************/
        if (!pll2_done)
        {
            one_count = DRV_Reg32(0x10213320) >> 16; // 0x10213320[31:16] PLL2 one count
            zero_count = DRV_Reg32(0x10213320) & 0x0000FFFF; // 0x10213320[15:0] PLL2 zero count

            if (pll2_phase == 0)
            {
                if (one_count > (zero_count+MEMPLL_JMETER_CONFIDENCE_CNT))
                {
                    // REF lag FBK
                    pll2_phase = 1;
                    pll2_dl++;

                    print("[PLL_Phase_Calib] PLL2 initial phase: REF lag FBK, one_cnt/zero_cnt = %d/%d\n", one_count, zero_count);
                }
                else if (zero_count > (one_count+MEMPLL_JMETER_CONFIDENCE_CNT))
                {
                    // REF lead FBK
                    pll2_phase = 2;
                    pll2_dl++;

                    print("[PLL_Phase_Calib] PLL2 initial phase: REF lead FBK, one_cnt/zero_cnt = %d/%d\n", one_count, zero_count);
                }
                else
                {
                    // in phase at initial
                    pll2_done = 1;

                    print("[PLL_Phase_Calib] PLL2 initial phase: REF in-phase FBK, one_cnt/zero_cnt = %d/%d\n", one_count, zero_count);
                }
            }
            else if (pll2_phase == 1)
            {
                if ((zero_count+MEMPLL_JMETER_CONFIDENCE_CNT) >= one_count)
                {
                    pll2_done = 1;
                    print("[PLL_Phase_Calib] PLL2 REF_DL: 0x0, FBK_DL: 0x%x, one_cnt/zero_cnt = %d/%d\n", pll2_dl, one_count, zero_count);
                }
                else
                {
                    pll2_dl++;
                }
            }
            else
            {
                if ((one_count+MEMPLL_JMETER_CONFIDENCE_CNT) >= zero_count)
                {
                    pll2_done = 1;
                    print("[PLL_Phase_Calib] PLL2 REF_DL: 0x%x, FBK_DL: 0x0, one_cnt/zero_cnt = %d/%d\n", pll2_dl, one_count, zero_count);
                }
                else
                {
                    pll2_dl++;
                }
            }
        }

        if (!pll3_done)
        {
            one_count = DRV_Reg32(0x10213324) >> 16; // 0x10213324[31:16] PLL3 one count
            zero_count = DRV_Reg32(0x10213324) & 0x0000FFFF; // 0x10213324[15:0] PLL3 zero count

            if (pll3_phase == 0)
            {
                if (one_count > (zero_count+MEMPLL_JMETER_CONFIDENCE_CNT))
                {
                    // REF lag FBK
                    pll3_phase = 1;
                    pll3_dl++;

                    print("[PLL_Phase_Calib] PLL3 initial phase: REF lag FBK, one_cnt/zero_cnt = %d/%d\n", one_count, zero_count);
                }
                else if (zero_count > (one_count+MEMPLL_JMETER_CONFIDENCE_CNT))
                {
                    // REF lead FBK
                    pll3_phase = 2;
                    pll3_dl++;

                    print("[PLL_Phase_Calib] PLL3 initial phase: REF lead FBK, one_cnt/zero_cnt = %d/%d\n", one_count, zero_count);
                }
                else
                {
                    // in phase at initial
                    pll3_done = 1;

                    print("[PLL_Phase_Calib] PLL3 initial phase: REF in-phase FBK, one_cnt/zero_cnt = %d/%d\n", one_count, zero_count);
                }
            }
            else if (pll3_phase == 1)
            {
                if ((zero_count+MEMPLL_JMETER_CONFIDENCE_CNT) >= one_count)
                {
                    pll3_done = 1;
                    print("[PLL_Phase_Calib] PLL3 REF_DL: 0x0, FBK_DL: 0x%x, one_cnt/zero_cnt = %d/%d\n", pll3_dl, one_count, zero_count);
                }
                else
                {
                    pll3_dl++;
                }
            }
            else
            {
                if ((one_count+MEMPLL_JMETER_CONFIDENCE_CNT) >= zero_count)
                {
                    pll3_done = 1;
                    print("[PLL_Phase_Calib] PLL3 REF_DL: 0x%x, FBK_DL: 0x0, one_cnt/zero_cnt = %d/%d\n", pll3_dl, one_count, zero_count);
                }
                else
                {
                    pll3_dl++;
                }
            }
        }

        if (!pll4_done)
        {
            one_count = DRV_Reg32(0x10213328) >> 16; // 0x10213328[31:16] PLL4 one count
            zero_count = DRV_Reg32(0x10213328) & 0x0000FFFF; // 0x10213328[15:0] PLL4 zero count

            if (pll4_phase == 0)
            {
                if (one_count > (zero_count+MEMPLL_JMETER_CONFIDENCE_CNT))
                {
                    // REF lag FBK
                    pll4_phase = 1;
                    pll4_dl++;

                    print("[PLL_Phase_Calib] PLL4 initial phase: REF lag FBK, one_cnt/zero_cnt = %d/%d\n", one_count, zero_count);
                }
                else if (zero_count > (one_count+MEMPLL_JMETER_CONFIDENCE_CNT))
                {
                    // REF lead FBK
                    pll4_phase = 2;
                    pll4_dl++;

                    print("[PLL_Phase_Calib] PLL4 initial phase: REF lead FBK, one_cnt/zero_cnt = %d/%d\n", one_count, zero_count);
                }
                else
                {
                    // in phase at initial
                    pll4_done = 1;

                    print("[PLL_Phase_Calib] PLL4 initial phase: REF in-phase FBK, one_cnt/zero_cnt = %d/%d\n", one_count, zero_count);
                }
            }
            else if (pll4_phase == 1)
            {
                if ((zero_count+MEMPLL_JMETER_CONFIDENCE_CNT) >= one_count)
                {
                    pll4_done = 1;
                    print("[PLL_Phase_Calib] PLL4 REF_DL: 0x0, FBK_DL: 0x%x, one_cnt/zero_cnt = %d/%d\n", pll4_dl, one_count, zero_count);
                }
                else
                {
                    pll4_dl++;
                }
            }
            else
            {
                if ((one_count+MEMPLL_JMETER_CONFIDENCE_CNT) >= zero_count)
                {
                    pll4_done = 1;
                    print("[PLL_Phase_Calib] PLL4 REF_DL: 0x%x, FBK_DL: 0x0, one_cnt/zero_cnt = %d/%d\n", pll4_dl, one_count, zero_count);
                }
                else
                {
                    pll4_dl++;
                }
            }
        }

        /***********************************************
        * 6. Reset jitter meter value
        ************************************************/

        if (!pll2_done)
        {
            //pll2_dl++;
            temp = DRV_Reg32(0x102131CC);
            DRV_WriteReg32(0x102131CC, temp & ~0x1); // 0x102131CC[0]=0 PLL2
        }

        if (!pll3_done)
        {
            //pll3_dl++;
            temp = DRV_Reg32(0x102131D0);
            DRV_WriteReg32(0x102131D0, temp & ~0x1); // 0x102131D0[0]=0 PLL3
        }

        if (!pll4_done)
        {
            //pll4_dl++;
            temp = DRV_Reg32(0x102131D4);
            DRV_WriteReg32(0x102131D4, temp & ~0x1); // 0x102131D4[0]=0 PLL4
        }

        /*************************************************************
        * Then return to step 1 to adjust next delay chain tap value.
        * Until we have ~ 50% of one or zero count on jitter meter
        **************************************************************/
        if (pll2_done && pll3_done && pll4_done)
        {
            ret = 0;
            break;
        }

        if (pll2_dl >= 32 || pll3_dl >= 32 || pll4_dl >= 32)
        {
            ret = -1;
            break;
        }
    }


    if (ret != 0)
    {
        print("[PLL_Phase_Calib] MEMPLL 3PLL mode calibration fail\n");
    #if 0 // for FT, enable it...
        while(1); // TBD
    #endif
    }

    /***********************************************
    * 7. Set jitter meter clock to external FB path
    ************************************************/
/*
    temp = DRV_Reg32(0x1021360C);
    DRV_WriteReg32(0x1021360C, temp | 0x200); // 0x1021360C[9] = 1 PLL2

    temp = DRV_Reg32(0x10213614);
    DRV_WriteReg32(0x10213614, temp | 0x200); // 0x10213614[9] = 1 PLL3

    temp = DRV_Reg32(0x1021361C);
    DRV_WriteReg32(0x1021361C, temp | 0x200); // 0x1021361C[9] = 1 PLL4
*/
    return ret;

}
#endif
#endif

//#define DEBUG_FQMTR
#ifdef DEBUG_FQMTR

#ifdef __KERNEL__ // for kernel

#define FQMTR_TAG     "Power/swap"

#define fqmtr_err(fmt, args...)       \
    xlog_printk(ANDROID_LOG_ERROR, FQMTR_TAG, fmt, ##args)
#define fqmtr_warn(fmt, args...)      \
    xlog_printk(ANDROID_LOG_WARN, FQMTR_TAG, fmt, ##args)
#define fqmtr_info(fmt, args...)      \
    xlog_printk(ANDROID_LOG_INFO, FQMTR_TAG, fmt, ##args)
#define fqmtr_dbg(fmt, args...)       \
    xlog_printk(ANDROID_LOG_DEBUG, FQMTR_TAG, fmt, ##args)
#define fqmtr_ver(fmt, args...)       \
    xlog_printk(ANDROID_LOG_VERBOSE, FQMTR_TAG, fmt, ##args)

#define fqmtr_read(addr)            __raw_readl(IOMEM(addr))
#define fqmtr_write(addr, val)      mt_reg_sync_writel(val, addr)

#else // for preloader

#define fqmtr_err(fmt, args...)       \
    print(fmt, ##args)
#define fqmtr_warn(fmt, args...)      \
    print(fmt, ##args)
#define fqmtr_info(fmt, args...)      \
    print(fmt, ##args)
#define fqmtr_dbg(fmt, args...)       \
    print(fmt, ##args)
#define fqmtr_ver(fmt, args...)       \
    print(fmt, ##args)

#define fqmtr_read(addr)            DRV_Reg32(addr)
#define fqmtr_write(addr, val)      DRV_WriteReg32(val, addr)

#endif


#define FREQ_MTR_CTRL_REG       (CKSYS_BASE + 0x10)
#define FREQ_MTR_CTRL_RDATA     (CKSYS_BASE + 0x14)


#define RG_FQMTR_CKDIV_GET(x)           (((x) >> 28) & 0x3)
#define RG_FQMTR_CKDIV_SET(x)           (((x)& 0x3) << 28)
#define RG_FQMTR_FIXCLK_SEL_GET(x)      (((x) >> 24) & 0x3)
#define RG_FQMTR_FIXCLK_SEL_SET(x)      (((x)& 0x3) << 24)
#define RG_FQMTR_MONCLK_SEL_GET(x)      (((x) >> 16) & 0x3f)
#define RG_FQMTR_MONCLK_SEL_SET(x)      (((x)& 0x3f) << 16)
#define RG_FQMTR_MONCLK_EN_GET(x)       (((x) >> 15) & 0x1)
#define RG_FQMTR_MONCLK_EN_SET(x)       (((x)& 0x1) << 15)
#define RG_FQMTR_MONCLK_RST_GET(x)      (((x) >> 14) & 0x1)
#define RG_FQMTR_MONCLK_RST_SET(x)      (((x)& 0x1) << 14)
#define RG_FQMTR_MONCLK_WINDOW_GET(x)   (((x) >> 0) & 0xfff)
#define RG_FQMTR_MONCLK_WINDOW_SET(x)   (((x)& 0xfff) << 0)

#define RG_FQMTR_CKDIV_DIV_2    0
#define RG_FQMTR_CKDIV_DIV_4    1
#define RG_FQMTR_CKDIV_DIV_8    2
#define RG_FQMTR_CKDIV_DIV_16   3

#define RG_FQMTR_FIXCLK_26MHZ   0
#define RG_FQMTR_FIXCLK_32KHZ   2

enum rg_fqmtr_monclk
{
    RG_FQMTR_MONCLK_PDN = 0,
    RG_FQMTR_MONCLK_MAINPLL_DIV8,
    RG_FQMTR_MONCLK_MAINPLL_DIV11,
    RG_FQMTR_MONCLK_MAINPLL_DIV12,
    RG_FQMTR_MONCLK_MAINPLL_DIV20,
    RG_FQMTR_MONCLK_MAINPLL_DIV7,
    RG_FQMTR_MONCLK_MAINPLL_DIV16,
    RG_FQMTR_MONCLK_MAINPLL_DIV24,
    RG_FQMTR_MONCLK_NFI2X,
    RG_FQMTR_MONCLK_WHPLL,
    RG_FQMTR_MONCLK_WPLL,
    RG_FQMTR_MONCLK_26MHZ,
    RG_FQMTR_MONCLK_USB_48MHZ,
    RG_FQMTR_MONCLK_EMI1X ,
    RG_FQMTR_MONCLK_AP_INFRA_FAST_BUS,
    RG_FQMTR_MONCLK_SMI_MMSYS,
    RG_FQMTR_MONCLK_UART0,
    RG_FQMTR_MONCLK_UART1,
    RG_FQMTR_MONCLK_GPU,
    RG_FQMTR_MONCLK_MSDC0,
    RG_FQMTR_MONCLK_MSDC1,
    RG_FQMTR_MONCLK_AD_DSI0_LNTC_DSICLK,
    RG_FQMTR_MONCLK_AD_MPPLL_TST_CK,
    RG_FQMTR_MONCLK_AP_PLLLGP_TST_CK,
    RG_FQMTR_MONCLK_52MHZ,
    RG_FQMTR_MONCLK_ARMPLL,
    RG_FQMTR_MONCLK_32KHZ,
    RG_FQMTR_MONCLK_AD_MEMPLL_MONCLK,
    RG_FQMTR_MONCLK_AD_MEMPLL2_MONCLK,
    RG_FQMTR_MONCLK_AD_MEMPLL3_MONCLK,
    RG_FQMTR_MONCLK_AD_MEMPLL4_MONCLK,
    RG_FQMTR_MONCLK_RESERVED,
    RG_FQMTR_MONCLK_CAM_SENINF,
    RG_FQMTR_MONCLK_SCAM,
    RG_FQMTR_MONCLK_PWM_MMSYS,
    RG_FQMTR_MONCLK_DDRPHYCFG,
    RG_FQMTR_MONCLK_PMIC_SPI,
    RG_FQMTR_MONCLK_SPI,
    RG_FQMTR_MONCLK_104MHZ,
    RG_FQMTR_MONCLK_USB_78MHZ,
    RG_FQMTR_MONCLK_MAX,
};

const char *rg_fqmtr_monclk_name[] =
{
    [RG_FQMTR_MONCLK_PDN]                   = "power done",
    [RG_FQMTR_MONCLK_MAINPLL_DIV8]          = "mpll D8",
    [RG_FQMTR_MONCLK_MAINPLL_DIV11]         = "mpll D11",
    [RG_FQMTR_MONCLK_MAINPLL_DIV12]         = "mpll D12",
    [RG_FQMTR_MONCLK_MAINPLL_DIV20]         = "mpll D20",
    [RG_FQMTR_MONCLK_MAINPLL_DIV7]          = "upll D7",
    [RG_FQMTR_MONCLK_MAINPLL_DIV16]         = "upll D16",
    [RG_FQMTR_MONCLK_MAINPLL_DIV24]         = "upll D24",
    [RG_FQMTR_MONCLK_NFI2X]                 = "nfi2x",
    [RG_FQMTR_MONCLK_WHPLL]                 = "whpll",
    [RG_FQMTR_MONCLK_WPLL]                  = "wpll",
    [RG_FQMTR_MONCLK_26MHZ]                 = "26MHz",
    [RG_FQMTR_MONCLK_USB_48MHZ]             = "USB 48MHz",
    [RG_FQMTR_MONCLK_EMI1X]                 = "emi1x",
    [RG_FQMTR_MONCLK_AP_INFRA_FAST_BUS]     = "infra fast bus",
    [RG_FQMTR_MONCLK_SMI_MMSYS]             = "smi mmsys",
    [RG_FQMTR_MONCLK_UART0]                 = "uart0",
    [RG_FQMTR_MONCLK_UART1]                 = "uart1",
    [RG_FQMTR_MONCLK_GPU]                   = "gpu",
    [RG_FQMTR_MONCLK_MSDC0]                 = "msdc0",
    [RG_FQMTR_MONCLK_MSDC1]                 = "msdc1",
    [RG_FQMTR_MONCLK_AD_DSI0_LNTC_DSICLK]   = "AD_DSI0_LNTC_DSICLK",
    [RG_FQMTR_MONCLK_AD_MPPLL_TST_CK]       = "AD_MPPLL_TST_CK",
    [RG_FQMTR_MONCLK_AP_PLLLGP_TST_CK]      = "AP_PLLLGP_TST_CK",
    [RG_FQMTR_MONCLK_52MHZ]                 = "52MHz",
    [RG_FQMTR_MONCLK_ARMPLL]                = "ARMPLL",
    [RG_FQMTR_MONCLK_32KHZ]                 = "32Khz",
    [RG_FQMTR_MONCLK_AD_MEMPLL_MONCLK]      = "AD_MEMPLL_MON",
    [RG_FQMTR_MONCLK_AD_MEMPLL2_MONCLK]     = "AD_MEMPLL_2MON",
    [RG_FQMTR_MONCLK_AD_MEMPLL3_MONCLK]     = "AD_MEMPLL_3MON",
    [RG_FQMTR_MONCLK_AD_MEMPLL4_MONCLK]     = "AD_MEMPLL_4MON",
    [RG_FQMTR_MONCLK_RESERVED]              = "Reserved",
    [RG_FQMTR_MONCLK_CAM_SENINF]            = "CAM seninf",
    [RG_FQMTR_MONCLK_SCAM]                  = "SCAM",
    [RG_FQMTR_MONCLK_PWM_MMSYS]             = "PWM mmsys",
    [RG_FQMTR_MONCLK_DDRPHYCFG]             = "ddrphycfg",
    [RG_FQMTR_MONCLK_PMIC_SPI]              = "PMIC SPI",
    [RG_FQMTR_MONCLK_SPI]                   = "SPI",
    [RG_FQMTR_MONCLK_104MHZ]                = "104MHz",
    [RG_FQMTR_MONCLK_USB_78MHZ]             = "USB 78MHz",
    [RG_FQMTR_MONCLK_MAX]                   = "MAX",
};

#define RG_FQMTR_EN     1
#define RG_FQMTR_RST    1

#define RG_FRMTR_WINDOW     0x100

unsigned int do_fqmtr_ctrl(int fixclk, int monclk_sel)
{
    u32 value = 0;

    BUG_ON(!((fixclk == RG_FQMTR_FIXCLK_26MHZ) | (fixclk == RG_FQMTR_FIXCLK_32KHZ)));
    // reset
    DRV_WriteReg32(FREQ_MTR_CTRL_REG, RG_FQMTR_MONCLK_RST_SET(RG_FQMTR_RST));
    // reset deassert
    DRV_WriteReg32(FREQ_MTR_CTRL_REG, RG_FQMTR_MONCLK_RST_SET(!RG_FQMTR_RST));
    // set window and target
    DRV_WriteReg32(FREQ_MTR_CTRL_REG, RG_FQMTR_MONCLK_WINDOW_SET(RG_FRMTR_WINDOW) |
                RG_FQMTR_MONCLK_SEL_SET(monclk_sel) |
                RG_FQMTR_FIXCLK_SEL_SET(fixclk)	|
		RG_FQMTR_MONCLK_EN_SET(RG_FQMTR_EN));
    gpt_busy_wait_us(100);
    value = DRV_Reg32(FREQ_MTR_CTRL_RDATA);
    // reset
    DRV_WriteReg32(FREQ_MTR_CTRL_REG, RG_FQMTR_MONCLK_RST_SET(RG_FQMTR_RST));
    // reset deassert
    DRV_WriteReg32(FREQ_MTR_CTRL_REG, RG_FQMTR_MONCLK_RST_SET(!RG_FQMTR_RST));
    if (fixclk == RG_FQMTR_FIXCLK_26MHZ)
        return ((26 * value) / (RG_FRMTR_WINDOW + 1));
    else
        return ((32000 * value) / (RG_FRMTR_WINDOW + 1));

}


void dump_fqmtr(void)
{
    int i = 0;
    unsigned int ret;
/*
    // fixclk = RG_FQMTR_FIXCLK_32KHZ
    for (i = 0; i < RG_FQMTR_MONCLK_MAX; i++)
    {
        if (i == RG_FQMTR_MONCLK_RESERVED)
            continue;
        ret = do_fqmtr_ctrl(RG_FQMTR_FIXCLK_32KHZ, i);
        fqmtr_dbg("%s - %d", rg_fqmtr_monclk_name[i], ret);
    }
*/
    // fixclk = RG_FQMTR_FIXCLK_26MHZ
    for (i = 0; i < RG_FQMTR_MONCLK_MAX; i++)
    {
        if (i == RG_FQMTR_MONCLK_RESERVED)
            continue;
        ret = do_fqmtr_ctrl(RG_FQMTR_FIXCLK_26MHZ, i);
        fqmtr_dbg("%s - %d MHz\n", rg_fqmtr_monclk_name[i], ret);
    }
}


unsigned int mt_get_cpu_freq(void)
{
	unsigned output = 0;
#if CFG_FPGA_PLATFORM
	return output;
#else
	return do_fqmtr_ctrl(RG_FQMTR_FIXCLK_26MHZ, RG_FQMTR_MONCLK_ARMPLL);
#endif
}


unsigned int mt_get_mem_freq(void)
{
	unsigned output = 0;
#if CFG_FPGA_PLATFORM
	return output;
#else
	return do_fqmtr_ctrl(RG_FQMTR_FIXCLK_26MHZ, RG_FQMTR_MONCLK_EMI1X);
#endif
}

unsigned int mt_get_bus_freq(void)
{
	unsigned int bus_clk = 26000;
#if CFG_FPGA_PLATFORM
	return bus_clk; // Khz
#else
	return do_fqmtr_ctrl(RG_FQMTR_FIXCLK_26MHZ, RG_FQMTR_MONCLK_AP_INFRA_FAST_BUS);
#endif
}
#endif /* DEBUG_FQMTR */
//after pmic_init
void mt_pll_post_init(void)
{
    unsigned int temp;
    int ret;

#if LPDDR == 2
    mt_mempll_init(DDR1066, PLL_MODE_1);
#else
    mt_mempll_init(DDR1066, PLL_MODE_1);
#endif
    //mt_mempll_init(DDR1333, PLL_MODE_3); // 3PLL mode
    //mt_mempll_init(DDR938, PLL_MODE_1);
    //mt_mempll_init(DDR900, PLL_MODE_1);
    //mt_mempll_init(DDR1466, PLL_MODE_1);

    /* MEMPLL Calibration */
    #ifdef DDRPHY_3PLL_MODE
        ret = mt_mempll_cali();
    #endif
/*****************
* xPLL HW Control
******************/
    //TBD (mem init)
    DRV_WriteReg32(AP_PLL_CON1, (DRV_Reg32(AP_PLL_CON1) & 0xFCFCEFCC)); // Main, ARM PLL HW Control
    DRV_WriteReg32(AP_PLL_CON2, (DRV_Reg32(AP_PLL_CON2) & 0xFFFFFFFC)); // Main, ARM PLL HW Control

#ifdef DEBUG_FQMTR
    dump_fqmtr();
#endif /* DEBUG_FQMTR */

    print("AP_PLL_CON1= 0x%x\n", DRV_Reg32(AP_PLL_CON1));
    print("AP_PLL_CON2= 0x%x\n", DRV_Reg32(AP_PLL_CON2));
    print("CLKSQ_STB_CON0= 0x%x\n", DRV_Reg32(CLKSQ_STB_CON0));
    print("PLL_ISO_CON0= 0x%x\n", DRV_Reg32(PLL_ISO_CON0));
    print("ARMPLL_CON0= 0x%x\n", DRV_Reg32(ARMPLL_CON0));
    print("ARMPLL_CON1= 0x%x\n", DRV_Reg32(ARMPLL_CON1));
    print("ARMPLL_PWR_CON0= 0x%x\n", DRV_Reg32(ARMPLL_PWR_CON0));
    print("MPLL_CON0= 0x%x\n", DRV_Reg32(MAINPLL_CON0));
    print("MPLL_CON1= 0x%x\n", DRV_Reg32(MAINPLL_CON1));
    print("MPLL_PWR_CON0= 0x%x\n", DRV_Reg32(MAINPLL_PWR_CON0));
    print("UPLL_CON0= 0x%x\n", DRV_Reg32(UNIVPLL_CON0));
    print("UPLL_CON1= 0x%x\n", DRV_Reg32(UNIVPLL_CON1));
    print("UPLL_PWR_CON0= 0x%x", DRV_Reg32(UNIVPLL_PWR_CON0));
    print("DISP_CG_CON0= 0x%x, \n", DRV_Reg32(MMSYS_CG_CON0));
    print("DISP_CG_CON1= 0x%x, \n", DRV_Reg32(MMSYS_CG_CON1));
#if 0
    print("cpu_freq = %dKhz\n", mt_get_cpu_freq());
    print("bus_freq = %dKhz\n", mt_get_bus_freq());
    print("mem_freq = %dKhz\n", mt_get_mem_freq());
#endif
}

#if 0
//after pmic_init
void mt_arm_pll_sel(void)
{
    unsigned int temp;
    temp = DRV_Reg32(TOP_CKMUXSEL);
    DRV_WriteReg32(TOP_CKMUXSEL, temp | 0x1); // switch CA7_ck to ARMCA7PLL
    print("[PLL] mt_arm_pll_sel done\n");
}
#endif

void mt_pll_init(void)
{
  int ret = 0;
  unsigned int temp;
#if !(CFG_FPGA_PLATFORM)
/*************
 * CLKSQ
 * ***********/
	DRV_WriteReg32(AP_PLL_CON0, (DRV_Reg32(AP_PLL_CON0) | 0x1)); //[0] CLKSQ_EN = 1
	gpt_busy_wait_us(100);	//wait 100us
	DRV_WriteReg32(AP_PLL_CON0, (DRV_Reg32(AP_PLL_CON0) | 0x2)); //[1] CLKSQ_LPF_EN =1

/*************
* xPLL PWR ON
**************/
	DRV_WriteReg32(ARMPLL_PWR_CON0, (DRV_Reg32(ARMPLL_PWR_CON0) | 0x1)); 	//[0]ARMPLL_PWR_ON = 1
	DRV_WriteReg32(MAINPLL_PWR_CON0, (DRV_Reg32(MAINPLL_PWR_CON0) | 0x1)); 	//[0]MAINPLL_PWR_ON = 1
	DRV_WriteReg32(UNIVPLL_PWR_CON0, (DRV_Reg32(UNIVPLL_PWR_CON0) | 0x1)); 	//[0]UNIVPLL_PWR_ON = 1

/*************
* Wait PWR ready(30ns)
**************/
	gpt_busy_wait_us(30);

/******************
* xPLL ISO Disable
*******************/
	DRV_WriteReg32(ARMPLL_PWR_CON0,	(DRV_Reg32(ARMPLL_PWR_CON0) & 0xFFFFFFFD));//[2]ARMPLL_ISO_EN = 0
	DRV_WriteReg32(MAINPLL_PWR_CON0, (DRV_Reg32(MAINPLL_PWR_CON0) & 0xFFFFFFFD)); //[2]MAINPLL_ISO_EN = 0
	DRV_WriteReg32(UNIVPLL_PWR_CON0, (DRV_Reg32(UNIVPLL_PWR_CON0) & 0xFFFFFFFD)); //[2]UNIVPLL_ISO_EN = 0

/********************
* xPLL Frequency Set
*********************/
	DRV_WriteReg32(ARMPLL_CON1, 0x8009a000);  // 1000Mhz
	DRV_WriteReg32(MAINPLL_CON1, 0x800e7000);
	DRV_WriteReg32(UNIVPLL_CON1, 0x81000060);

/***********************
* xPLL Frequency Enable
************************/
	DRV_WriteReg32(ARMPLL_CON0, (DRV_Reg32(ARMPLL_CON0) | 0x1)); //[0] ARMPLL_EN = 1
	DRV_WriteReg32(MAINPLL_CON0, (DRV_Reg32(MAINPLL_CON0) | 0x1)); //[0]MAINPLL_EN = 1
	DRV_WriteReg32(UNIVPLL_CON0, (DRV_Reg32(UNIVPLL_CON0) | 0x1)); //[0] UNIVPLL_EN = 1

/*************
* Wait PWR ready(20ns)
**************/
	gpt_busy_wait_us(20); // wait for PLL stable (min delay is 20us)

/***************
* xPLL DIV RSTB
****************/
	DRV_WriteReg32(MAINPLL_CON0, (DRV_Reg32(MAINPLL_CON0) | 0x08000000));//[27]MAINPLL_DIV_RSTB = 1
	DRV_WriteReg32(UNIVPLL_CON0, (DRV_Reg32(UNIVPLL_CON0) | 0x08000000));//]27]UNIVPLL_DIV_RSTB = 1

/*****************
* AXI BUS Init
******************/
	DRV_WriteReg32(INFRABUS_DCMCTL1, 0x80000000);
	DRV_WriteReg32(CLK_MUX_SEL0, 0x0B60b446);

/**************
* INFRA CLKMUX
***************/
	DRV_WriteReg32(ACLKEN_DIV, 0x12); // CPU BUS clock freq is divided by 2
	DRV_WriteReg32(PCLKEN_DIV, 0x15); // CPU debug clock freq is divided by 5
    	temp = DRV_Reg32(TOP_CKMUXSEL) & ~0xC;
    	DRV_WriteReg32(TOP_CKMUXSEL, temp | 0x4); // switch CPU clock to ARMPLL
/************
* TOP CLKMUX
*************/
	DRV_WriteReg32(CLK_MUX_SEL0, 0x27653446);
	DRV_WriteReg32(CLK_MUX_SEL1, 0x01291008);

/*************/
/*for MTCMOS
/*************/
	spm_write(SPM_POWERON_CONFIG_SET, (SPM_PROJECT_CODE << 16) | (1U << 0));
	DRV_WriteReg32(CLK_GATING_CTRL0, 0xBFCFFDFA);
	DRV_WriteReg32(CLK_GATING_CTRL1, (DRV_Reg32(CLK_GATING_CTRL1) | 0x800000));
	DRV_WriteReg32(CLK_GATING_CTRL2, (DRV_Reg32(CLK_GATING_CTRL2) | 0x10));
#ifdef MTK_EMMC_SUPPORT
//	DRV_WriteReg32(CLK_GATING_CTRL2, (DRV_Reg32(CLK_GATING_CTRL2) | 0x4));
//	DRV_WriteReg32(CLK_GATING_CTRL1, (DRV_Reg32(CLK_GATING_CTRL1) | 0x800C0));
#endif
	spm_mtcmos_ctrl_disp(STA_POWER_ON);
//	spm_mtcmos_ctrl_isp(STA_POWER_ON);
//	spm_mtcmos_ctrl_mfg(STA_POWER_ON);
//	spm_mtcmos_ctrl_connsys(STA_POWER_ON);

/*************/
/*for CG
/*************/
	DRV_WriteReg32(MMSYS_CG_CON0,(DRV_Reg32(MMSYS_CG_CON0) & 0xfffffffC)); //enable bit0, 1, for CG
	DRV_WriteReg32(MMSYS_CG_CON1,0x0);
//	DRV_WriteReg32(IMG_CG_CON,0x0);
	DRV_WriteReg32(MFG_CG_CLR,0x1);
	DRV_WriteReg32(AUDIO_TOP_CON0,(DRV_Reg32(AUDIO_TOP_CON0) | 0x3000044));
	DRV_WriteReg32(PMIC_WRAP_SEL,(DRV_Reg32(PMIC_WRAP_SEL) & 0xfffffffd));

#endif
}
