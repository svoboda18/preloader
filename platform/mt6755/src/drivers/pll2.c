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

/** macro **/
#define aor(v, a, o) (((v) & (a)) | (o))

extern u32 seclib_get_devinfo_with_index(u32 index);

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

    *((UINT32P)(DRAMC0_BASE + 0x007c)) |= 0x00000001; //DFREQ_DIV2=1
    *((UINT32P)(DDRPHY_BASE + 0x007c)) |= 0x00000001;

    if (pll_mode == PLL_MODE_3)
    {
        *((UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00000020;    //3PLL sync mode, OK
    }
    else if (pll_mode== PLL_MODE_2)
    {
        *((UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00000022;
    }
    else // 1 PLL mode
    {
        *((UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00000007;    //1PLL sync mode, OK.
    }

    /*****************************************************************************************
    * (2) Setup MEMPLL operation case & frequency. May set according to dram type & frequency
    ******************************************************************************************/
    *((UINT32P)(DDRPHY_BASE + (0x0170 <<2))) = r_bias_en_stb_time | r_bias_lpf_en_stb_time | r_mempll_en_stb_time | r_dmall_ck_en_stb_time;
    *((UINT32P)(DDRPHY_BASE + (0x0171 <<2))) = r_dds_en_stb_time | r_div_en_stb_time | r_dmpll2_ck_en_stb_time | r_iso_en_stb_time;
    *((UINT32P)(DDRPHY_BASE + (0x0172 <<2))) = r_bias_en_stb_dis| r_bias_en_src_sel | r_bias_lpf_en_stb_dis| r_bias_lpf_en_src_sel | r_mempll4_en_stb_dis| r_mempll3_en_stb_dis| r_mempll2_en_stb_dis| r_mempll_en_stb_dis| r_mempll4_en_src_sel | r_mempll3_en_src_sel | r_mempll2_en_src_sel | r_mempll_en_src_sel | r_dmall_ck_en_stb_dis | r_dmall_ck_en_src_sel;
    *((UINT32P)(DDRPHY_BASE + (0x0173 <<2))) = r_dds_en_stb_dis| r_dds_en_src_sel | r_div_en_stb_dis| r_div_en_src_sel | r_dmpll2_ck_en_stb_dis| r_dmpll2_ck_en_src_sel | r_iso_en_stb_dis| r_iso_en_src_sel;

    // MEMPLL common setting
    *((UINT32P)(DDRPHY_BASE + (0x0180 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0180 <<2))) & (~pllc1_postdiv_1_0)) | 0x00000000; //RG_MEMPLL_POSDIV[1:0] = 2'b00;
    *((UINT32P)(DDRPHY_BASE + (0x0180 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0180 <<2))) & (~pllc1_blp)) | (0x00000001 << 12); //RG_MEMPLL_BLP = 1'b1;
    *((UINT32P)(DDRPHY_BASE + (0x0181 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0181 <<2))) & (~pllc1_mempll_div_6_0)) | (0x00000052 << 25); //RG_MEMPLL_DIV = 7'h52;
    *((UINT32P)(DDRPHY_BASE + (0x0181 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0181 <<2))) & (~pllc1_mempll_reserve_2)) | (0x00000001 << 18); //RG_MEMPLL_RESERVE[2] = 1;
    
    *((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) & (~mempll2_fbksel_1_0)) | 0x00000000; //RG_MEMPLL2_FBKSEL[1:0] = 2'b00;
    *((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) & (~mempll2_bp_br)) | (0x00000003 << 26); //RG_MEMPLL2_BP = 1, RG_MEMPLL2_BR=1;

    if ((pll_mode == PLL_MODE_3) || (pll_mode == PLL_MODE_2))
    {
        *((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) & (~mempll2_fb_mck_sel)) | (0x00000001 << 9); //RG_MEMPLL2_FB_MCK_SEL;
    }
    
    *((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) & (~mempll3_fbksel_1_0)) | 0x00000000; //RG_MEMPLL3_FBKSEL = 2'b00;
    *((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) & (~mempll3_bp_br)) | (0x00000003 << 26); //RG_MEMPLL3_BP = 1, RG_MEMPLL3_BR=1;

    if ((pll_mode == PLL_MODE_3) || (pll_mode == PLL_MODE_2))
    {
        *((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) & (~mempll3_fb_mck_sel)) | (0x00000001 << 9); //RG_MEMPLL3_FB_MCK_SEL = 1;
    }
    
    *((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) & (~mempll4_fbksel_1_0)) | 0x00000000; //RG_MEMPLL4_FBKSEL = 2'b00;
    *((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) & (~mempll4_bp_br)) | (0x00000003 << 26); //RG_MEMPLL4_BP = 1, RG_MEMPLL4_BR=1;

    if ((pll_mode == PLL_MODE_3) || (pll_mode == PLL_MODE_2))
    {
        *((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) & (~mempll4_fb_mck_sel)) | (0x00000001 << 9); //RG_MEMPLL4_FB_MCK_SEL = 1;
    }

    //MEMPLL different setting for different frequency begin
    if (type == DDR1333) // real DDR-1280 (sign-off)
    {
        *((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) & (~mempll2_vco_div_sel)) | 0x00000000; //RG_MEMPLL2_VCO_DIV_SEL =0;
        *((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) & (~mempll3_vco_div_sel)) | 0x00000000; //RG_MEMPLL3_VCO_DIV_SEL =0;
        *((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) & (~mempll4_vco_div_sel)) | 0x00000000; //RG_MEMPLL4_VCO_DIV_SEL =0;
        // DDR-1333
        //*((UINT32P)(DDRPHY_BASE + (0x0189 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0189 <<2))) & (~pllc1_dmss_pcw_ncpo_30_0)) | (0x50d8fe7c << 1); //RG_DMSS_PCW_NCPO[30:0]
        // DDR-1280
        *((UINT32P)(DDRPHY_BASE + (0x0189 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0189 <<2))) & (~pllc1_dmss_pcw_ncpo_30_0)) | (0x4da21535 << 1); //RG_DMSS_PCW_NCPO[30:0]
        
        if ((pll_mode == PLL_MODE_3) || (pll_mode == PLL_MODE_2))
        {
            *((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) & (~mempll2_fbdiv_6_0)) | (0x0000000d << 2); //RG_MEMPLL2_FBDIV = 7'h0d;
            *((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) & (~mempll3_fbdiv_6_0)) | (0x0000000d << 2); //RG_MEMPLL3_FBDIV = 7'h0d;
            *((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) & (~mempll4_fbdiv_6_0)) | (0x0000000d << 2); //RG_MEMPLL4_FBDIV = 7'h0d;
        }
        else
        {
            *((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) & (~mempll2_fbdiv_6_0)) | (0x00000034 << 2); //RG_MEMPLL2_FBDIV = 7'h34;
            *((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) & (~mempll3_fbdiv_6_0)) | (0x00000034 << 2); //RG_MEMPLL3_FBDIV = 7'h34;
            *((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) & (~mempll4_fbdiv_6_0)) | (0x00000034 << 2); //RG_MEMPLL4_FBDIV = 7'h34;
        }
    }
    else if (type == DDR938) // for DVFS_low (DVS HQA), the same settings as DDR-1333 other than NCPO
    {
        *((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) & (~mempll2_vco_div_sel)) | 0x00000000; //RG_MEMPLL2_VCO_DIV_SEL =0;
        *((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) & (~mempll3_vco_div_sel)) | 0x00000000; //RG_MEMPLL3_VCO_DIV_SEL =0;
        *((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) & (~mempll4_vco_div_sel)) | 0x00000000; //RG_MEMPLL4_VCO_DIV_SEL =0;
        
    #if 1
        *((UINT32P)(DDRPHY_BASE + (0x0189 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0189 <<2))) & (~pllc1_dmss_pcw_ncpo_30_0)) | (0x38e3f9f0 << 1); //RG_DMSS_PCW_NCPO[30:0]
    #else // only for debug (lowest bring up frequency DDR-667)
        *((UINT32P)(DDRPHY_BASE + (0x0189 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0189 <<2))) & (~pllc1_dmss_pcw_ncpo_30_0)) | (0x287442a6 << 1); //RG_DMSS_PCW_NCPO[30:0]
    #endif
    
        if ((pll_mode == PLL_MODE_3) || (pll_mode == PLL_MODE_2))
        {
            *((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) & (~mempll2_fbdiv_6_0)) | (0x0000000d << 2); //RG_MEMPLL2_FBDIV = 7'h0d;
            *((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) & (~mempll3_fbdiv_6_0)) | (0x0000000d << 2); //RG_MEMPLL3_FBDIV = 7'h0d;
            *((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) & (~mempll4_fbdiv_6_0)) | (0x0000000d << 2); //RG_MEMPLL4_FBDIV = 7'h0d;
        }
        else
        {
            *((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) & (~mempll2_fbdiv_6_0)) | (0x00000034 << 2); //RG_MEMPLL2_FBDIV = 7'h34;
            *((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) & (~mempll3_fbdiv_6_0)) | (0x00000034 << 2); //RG_MEMPLL3_FBDIV = 7'h34;
            *((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) & (~mempll4_fbdiv_6_0)) | (0x00000034 << 2); //RG_MEMPLL4_FBDIV = 7'h34;
        }
    }
    else if (type == DDR1466)
    {
        *((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) & (~mempll2_vco_div_sel)) | 0x00000000; //RG_MEMPLL2_VCO_DIV_SEL =0;
        *((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) & (~mempll3_vco_div_sel)) | 0x00000000; //RG_MEMPLL3_VCO_DIV_SEL =0;
        *((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) & (~mempll4_vco_div_sel)) | 0x00000000; //RG_MEMPLL4_VCO_DIV_SEL =0;
        *((UINT32P)(DDRPHY_BASE + (0x0189 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0189 <<2))) & (~pllc1_dmss_pcw_ncpo_30_0)) | (0x52902d02 << 1); //RG_DMSS_PCW_NCPO[30:0] = 31'h52902d02;
        if ((pll_mode == PLL_MODE_3) || (pll_mode == PLL_MODE_2))
        {
            *((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) & (~mempll2_fbdiv_6_0)) | (0x0000000e << 2); //RG_MEMPLL2_FBDIV = 7'h0e;
            *((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) & (~mempll3_fbdiv_6_0)) | (0x0000000e << 2); //RG_MEMPLL3_FBDIV = 7'h0e;
            *((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) & (~mempll4_fbdiv_6_0)) | (0x0000000e << 2); //RG_MEMPLL4_FBDIV = 7'h0e;
        }
        else
        {
            *((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) & (~mempll2_fbdiv_6_0)) | (0x00000038 << 2); //RG_MEMPLL2_FBDIV = 7'h38;
            *((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) & (~mempll3_fbdiv_6_0)) | (0x00000038 << 2); //RG_MEMPLL3_FBDIV = 7'h38;
            *((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) & (~mempll4_fbdiv_6_0)) | (0x00000038 << 2); //RG_MEMPLL4_FBDIV = 7'h38;
        }
    }
    else // DDR-1066
    {
        *((UINT32P)(DDRPHY_BASE + (0x0189 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0189 <<2))) & (~pllc1_dmss_pcw_ncpo_30_0)) | (0x4c68b439 << 1); //RG_DMSS_PCW_NCPO[30:0] = 31'h4c68b439;
        if ((pll_mode == PLL_MODE_3) || (pll_mode == PLL_MODE_2))
        {
            *((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) & (~mempll2_fbdiv_6_0)) | (0x0000000b << 2); //RG_MEMPLL2_FBDIV = 7'h0b;
            *((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) & (~mempll3_fbdiv_6_0)) | (0x0000000b << 2); //RG_MEMPLL3_FBDIV = 7'h0b;
            *((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) & (~mempll4_fbdiv_6_0)) | (0x0000000b << 2); //RG_MEMPLL4_FBDIV = 7'h0b;
        }
        else
        {    
            *((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0182 <<2))) & (~mempll2_fbdiv_6_0)) | (0x0000002c << 2); //RG_MEMPLL2_FBDIV = 7'h2c;
            *((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0184 <<2))) & (~mempll3_fbdiv_6_0)) | (0x0000002c << 2); //RG_MEMPLL3_FBDIV = 7'h2c;
            *((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0186 <<2))) & (~mempll4_fbdiv_6_0)) | (0x0000002c << 2); //RG_MEMPLL4_FBDIV = 7'h2c;
        }
    }    
    //MEMPLL different setting for different frequency end

    if (pll_mode == PLL_MODE_2)
    {
        *((UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00000022  | r_dmpll2_clk_en;
    }
    else if (pll_mode == PLL_MODE_3)
    {
        *((UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00000020 | r_dmpll2_clk_en;    //3PLL sync mode
    }
    else // 1-PLL mode
    {    
        //*((UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00000007 | r_dmpll2_clk_en;    //1PLL sync mode
        *((UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00000007 ;    //1PLL sync mode
    }

    /***********************************
    * (3) Setup MEMPLL power on sequence
    ************************************/

    gpt_busy_wait_us(2);

    *((UINT32P)(DDRPHY_BASE + (0x0181 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0181 <<2))) & (~pllc1_mempll_bias_en)) | (0x00000001 << 14); //RG_MEMPLL_BIAS_EN = 1'b1;

    gpt_busy_wait_us(2);

    *((UINT32P)(DDRPHY_BASE + (0x0181 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0181 <<2))) & (~pllc1_mempll_bias_lpf_en)) | (0x00000001 << 15); //RG_MEMPLL_BIAS_LPF_EN = 1'b1;

    gpt_busy_wait_us(1000);

    *((UINT32P)(DDRPHY_BASE + (0x0180 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0180 <<2))) & (~pllc1_mempll_en)) | (0x00000001 << 2); //RG_MEMPLL_EN = 1'b1;

    gpt_busy_wait_us(20);

    *((UINT32P)(DDRPHY_BASE + (0x0181 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0181 <<2))) & (~pllc1_mempll_div_en)) | (0x00000001 << 24); //RG_MEMPLL_DIV_EN = 1'b1;

    gpt_busy_wait_us(1);

    if ((pll_mode == PLL_MODE_3) || (pll_mode == PLL_MODE_2))
    {
        *((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) & (~mempll2_en)) | (0x00000001 << 18); //RG_MEMPLL2_EN = 1'b1;
        *((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) & (~mempll3_en)) | (0x00000001 << 18); //RG_MEMPLL3_EN = 1'b1;
        *((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) & (~mempll4_en)) | (0x00000001 << 18); //RG_MEMPLL4_EN = 1'b1;
    }
    else
    {
        *((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) & (~mempll2_en)) | (0x00000001 << 18); //RG_MEMPLL2_EN = 1'b1;
        *((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) & (~mempll3_en)) | (0x00000000 << 18); //RG_MEMPLL3_EN = 1'b0;
        *((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) & (~mempll4_en)) | (0x00000000 << 18); //RG_MEMPLL4_EN = 1'b0;
    }

    gpt_busy_wait_us(23);

    if (pll_mode == PLL_MODE_2)
    {
        *((UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00000022  | r_dmpll2_clk_en | r_dmall_ck_en;
    }
    else if (pll_mode == PLL_MODE_3)
    {
        *((UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00000020 | r_dmpll2_clk_en | r_dmall_ck_en;    //3PLL sync mode
    }
    else // 1-PLL mode
    {
        *((UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00000007 | r_dmpll2_clk_en | r_dmall_ck_en;    //1PLL sync mode
    }
   
    /**********************************
    * (4) MEMPLL control switch to SPM
    ***********************************/
#ifdef fcSWITCH_SPM_CONTROL
    if ((pll_mode == PLL_MODE_3) || (pll_mode == PLL_MODE_2))
    {
        *((UINT32P)(DDRPHY_BASE + (0x0173 <<2))) = 0x40101000;   //[0]ISO_EN_SRC=0,[22]DIV_EN_SC_SRC=0 (pll2off),[16]DIV_EN_SRC=0,[8]PLL2_CK_EN_SRC=1(1pll),[8]PLL2_CK_EN_SRC=0(3pll)
    }
    else // 1-PLL mode
    {
        *((UINT32P)(DDRPHY_BASE + (0x0173 <<2))) = 0x40101000;   //[0]ISO_EN_SRC=0,[22]DIV_EN_SC_SRC=0 (pll2off),[16]DIV_EN_SRC=0,[8]PLL2_CK_EN_SRC=1(1pll),[8]PLL2_CK_EN_SRC=0(3pll)
    }

    if ((pll_mode == PLL_MODE_3) || (pll_mode == PLL_MODE_2))
    {
        *((UINT32P)(DDRPHY_BASE + (0x0172 <<2))) = 0x0000F010;   //[24]BIAS_EN_SRC=0,[16]BIAS_LPF_EN_SRC=0,[8]MEMPLL_EN,[9][10][11]MEMPLL2,3,4_EN_SRC,[0]ALL_CK_EN_SRC=0
    }
    else // 1-PLL mode
    {
        *((UINT32P)(DDRPHY_BASE + (0x0172 <<2))) = 0x0000FC10;   //1PLL mode, MEMPLL3,4_EN no change to spm controller.eep to 1'b0 for power saving.
        //*((UINT32P)(DDRPHY_BASE + (0x0172 <<2))) = 0x0000F010; // sim ok
    }

    //*((UINT32P)(DDRPHY_BASE + (0x0170 <<2))) = 0x003C1B96;   //setting for delay time
    *((UINT32P)(DDRPHY_BASE + (0x0170 <<2))) = 0x063C0000;   //setting for delay time
#endif 

// MEMPLL configuration end
}

#ifdef DDRPHY_3PLL_MODE
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
    //temp = DRV_Reg32(0x1000F60C);
    //DRV_WriteReg32(0x1000F60C, temp & ~0x200); // 0x1000F60C[9] = 0 PLL2

    //temp = DRV_Reg32(0x1000F614);
    //DRV_WriteReg32(0x1000F614, temp & ~0x200); // 0x1000F614[9] = 0 PLL3

    //temp = DRV_Reg32(0x1000F61C);
    //DRV_WriteReg32(0x1000F61C, temp & ~0x200); // 0x1000F61C[9] = 0 PLL4

    /***********************************************
    * 2. Set jitter meter count number
    ************************************************/
    // PLL2 0x1000F1CC  0x1000F32C[0]: JMETER_DONE 
    // PLL3 0x1000F1D0  0x1000F32C[1]: JMETER_DONE 
    // PLL4 0x1000F1D4  0x1000F32C[2]: JMETER_DONE

    temp = DRV_Reg32(0x1000F1CC) & 0x0000FFFF;
    DRV_WriteReg32(0x1000F1CC, (temp | (MEMPLL_JMETER_CNT<<16))); // 0x1000F1CC[31:16] PLL2 0x400 = 1024 count

    temp = DRV_Reg32(0x1000F1D0) & 0x0000FFFF;
    DRV_WriteReg32(0x1000F1D0, (temp | (MEMPLL_JMETER_CNT<<16))); // 0x1000F1D0[31:16] PLL3 0x400 = 1024 count

    temp = DRV_Reg32(0x1000F1D4) & 0x0000FFFF;
    DRV_WriteReg32(0x1000F1D4, (temp | (MEMPLL_JMETER_CNT<<16))); // 0x1000F1D4[31:16] PLL4 0x400 = 1024 count

    while(1)
    { 
        /***********************************************
        * 3. Adjust delay chain tap number
        ************************************************/
        if (!pll2_done)
        {
            if (pll2_phase == 0)    // initial phase set to 0 for REF and FBK
            {
                temp = DRV_Reg32(0x1000F610) & ~0xF8000000;
                DRV_WriteReg32(0x1000F610, (temp | (0x00 << 27 )));
                
                temp = DRV_Reg32(0x1000F610) & ~0x07C00000;
                DRV_WriteReg32(0x1000F610, (temp | (0x00 << 22)));
            }
            else if (pll2_phase == 1)   // REF lag FBK, delay FBK
            {
                temp = DRV_Reg32(0x1000F610) & ~0xF8000000;
                DRV_WriteReg32(0x1000F610, (temp | (0x00 << 27 )));
                
                temp = DRV_Reg32(0x1000F610) & ~0x07C00000;
                DRV_WriteReg32(0x1000F610, (temp | (pll2_dl << 22)));
            }
            else   // REF lead FBK, delay REF
            {
                temp = DRV_Reg32(0x1000F610) & ~0xF8000000;
                DRV_WriteReg32(0x1000F610, (temp | (pll2_dl << 27 )));
                
                temp = DRV_Reg32(0x1000F610) & ~0x07C00000;
                DRV_WriteReg32(0x1000F610, (temp | (0x00 << 22)));
            }   
        }

        if (!pll3_done)
        {
            if (pll3_phase == 0)    // initial phase set to 0 for REF and FBK
            {
                temp = DRV_Reg32(0x1000F618) & ~0xF8000000;
                DRV_WriteReg32(0x1000F618, (temp | (0x00 << 27 )));
                
                temp = DRV_Reg32(0x1000F618) & ~0x07C00000;
                DRV_WriteReg32(0x1000F618, (temp | (0x00 << 22)));
            }
            else if (pll3_phase == 1)   // REF lag FBK, delay FBK
            {
                temp = DRV_Reg32(0x1000F618) & ~0xF8000000;
                DRV_WriteReg32(0x1000F618, (temp | (0x00 << 27 )));
                
                temp = DRV_Reg32(0x1000F618) & ~0x07C00000;
                DRV_WriteReg32(0x1000F618, (temp | (pll3_dl << 22)));
            }
            else   // REF lead FBK, delay REF
            {
                temp = DRV_Reg32(0x1000F618) & ~0xF8000000;
                DRV_WriteReg32(0x1000F618, (temp | (pll3_dl << 27 )));
                
                temp = DRV_Reg32(0x1000F618) & ~0x07C00000;
                DRV_WriteReg32(0x1000F618, (temp | (0x00 << 22)));
            }
        }

        if (!pll4_done)
        {
            if (pll4_phase == 0)    // initial phase set to 0 for REF and FBK
            {
                temp = DRV_Reg32(0x1000F620) & ~0xF8000000;
                DRV_WriteReg32(0x1000F620, (temp | (0x00 << 27 )));
                
                temp = DRV_Reg32(0x1000F620) & ~0x07C00000;
                DRV_WriteReg32(0x1000F620, (temp | (0x00 << 22)));
            }
            else if (pll4_phase == 1)   // REF lag FBK, delay FBK
            {
                temp = DRV_Reg32(0x1000F620) & ~0xF8000000;
                DRV_WriteReg32(0x1000F620, (temp | (0x00 << 27 )));
                
                temp = DRV_Reg32(0x1000F620) & ~0x07C00000;
                DRV_WriteReg32(0x1000F620, (temp | (pll4_dl << 22)));
            }
            else   // REF lead FBK, delay REF
            {
                temp = DRV_Reg32(0x1000F620) & ~0xF8000000;
                DRV_WriteReg32(0x1000F620, (temp | (pll4_dl << 27 )));
                
                temp = DRV_Reg32(0x1000F620) & ~0x07C00000;
                DRV_WriteReg32(0x1000F620, (temp | (0x00 << 22)));
            }
        }

        gpt_busy_wait_us(20); // wait for external loop ready

        /***********************************************
        * 4. Enable jitter meter
        ************************************************/

        if (!pll2_done)
        {
            temp = DRV_Reg32(0x1000F1CC);
            DRV_WriteReg32(0x1000F1CC, temp | 0x1); // 0x1000F1CC[0]=1 PLL2
        }

        if (!pll3_done)
        {
            temp = DRV_Reg32(0x1000F1D0);
            DRV_WriteReg32(0x1000F1D0, temp | 0x1); // 0x1000F1D0[0]=1 PLL3
        }

        if (!pll4_done)
        {
            temp = DRV_Reg32(0x1000F1D4);
            DRV_WriteReg32(0x1000F1D4, temp | 0x1); // 0x1000F1D4[0]=1 PLL4
        }

        gpt_busy_wait_us(80); // wait for jitter meter complete, 1/26MHz*1024

/*
        jmeter_wait_count = 0;
        if (!pll2_done) 
        {
            while (!(DRV_Reg32(0x1000F32C)&0x00000001))
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
            while (!(DRV_Reg32(0x1000F32C)&0x00000002))
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
            while (!(DRV_Reg32(0x1000F32C)&0x00000004))
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
            one_count = DRV_Reg32(0x1000F320) >> 16; // 0x1000F320[31:16] PLL2 one count
            zero_count = DRV_Reg32(0x1000F320) & 0x0000FFFF; // 0x1000F320[15:0] PLL2 zero count
            
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
            one_count = DRV_Reg32(0x1000F324) >> 16; // 0x1000F324[31:16] PLL3 one count
            zero_count = DRV_Reg32(0x1000F324) & 0x0000FFFF; // 0x1000F324[15:0] PLL3 zero count

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
            one_count = DRV_Reg32(0x1000F328) >> 16; // 0x1000F328[31:16] PLL4 one count
            zero_count = DRV_Reg32(0x1000F328) & 0x0000FFFF; // 0x1000F328[15:0] PLL4 zero count

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
            pll2_dl++;
            temp = DRV_Reg32(0x1000F1CC);
            DRV_WriteReg32(0x1000F1CC, temp & ~0x1); // 0x1000F1CC[0]=0 PLL2
        }

        if (!pll3_done)
        {
            pll3_dl++;
            temp = DRV_Reg32(0x1000F1D0);
            DRV_WriteReg32(0x1000F1D0, temp & ~0x1); // 0x1000F1D0[0]=0 PLL3
        }

        if (!pll4_done)
        {
            pll4_dl++;
            temp = DRV_Reg32(0x1000F1D4);
            DRV_WriteReg32(0x1000F1D4, temp & ~0x1); // 0x1000F1D4[0]=0 PLL4
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
    temp = DRV_Reg32(0x1000F60C);
    DRV_WriteReg32(0x1000F60C, temp | 0x200); // 0x1000F60C[9] = 1 PLL2

    temp = DRV_Reg32(0x1000F614);
    DRV_WriteReg32(0x1000F614, temp | 0x200); // 0x1000F614[9] = 1 PLL3

    temp = DRV_Reg32(0x1000F61C);
    DRV_WriteReg32(0x1000F61C, temp | 0x200); // 0x1000F61C[9] = 1 PLL4
*/
    return ret;
  
}
#endif 


static unsigned int mt_get_cpu_freq(void)
{
    int output = 0;
    int i =0;
    unsigned int temp, clk26cali_0, clk_cfg_8, clk_misc_cfg_1;

    clk26cali_0 = DRV_Reg32(CLK26CALI_0);
    DRV_WriteReg32(CLK26CALI_0, clk26cali_0 | 0x80); // enable fmeter_en

    clk_misc_cfg_1 = DRV_Reg32(CLK_MISC_CFG_1);
    DRV_WriteReg32(CLK_MISC_CFG_1, 0xFFFF0300); // select divider

    clk_cfg_8 = DRV_Reg32(CLK_CFG_8);
    DRV_WriteReg32(CLK_CFG_8, (39 << 8)); // select abist_cksw

    temp = DRV_Reg32(CLK26CALI_0);
    DRV_WriteReg32(CLK26CALI_0, temp | 0x1); // start fmeter

    /* wait frequency meter finish */
    while (DRV_Reg32(CLK26CALI_0) & 0x1)
    {
        mdelay(10);
        i++;
        if(i > 10)
        	break;
    }

    temp = DRV_Reg32(CLK26CALI_1) & 0xFFFF;

    output = ((temp * 26000) / 1024) * 4; // Khz

    DRV_WriteReg32(CLK_CFG_8, clk_cfg_8);
    DRV_WriteReg32(CLK_MISC_CFG_1, clk_misc_cfg_1);
    DRV_WriteReg32(CLK26CALI_0, clk26cali_0);

    if(i>10)
        return 0;
    else
        return output;
}


static unsigned int mt_get_pll_freq(unsigned int ID)
{
    int output = 0;
    int i =0;
    unsigned int temp, clk26cali_0, clk_cfg_9, clk_misc_cfg_1, clk26cali_2;

    clk26cali_0 = DRV_Reg32(CLK26CALI_0);
    DRV_WriteReg32(CLK26CALI_0, clk26cali_0 | 0x80); // enable fmeter_en

    clk_misc_cfg_1 = DRV_Reg32(CLK_MISC_CFG_1);
    DRV_WriteReg32(CLK_MISC_CFG_1, 0x00FFFFFF); // select divider

    clk_cfg_9 = DRV_Reg32(CLK_CFG_9);
    DRV_WriteReg32(CLK_CFG_9, (ID << 16)); // select ckgen_cksw

    temp = DRV_Reg32(CLK26CALI_0);
    DRV_WriteReg32(CLK26CALI_0, temp | 0x10); // start fmeter

    /* wait frequency meter finish */
    while (DRV_Reg32(CLK26CALI_0) & 0x10)
    {
        mdelay(10);
        i++;
        if(i > 10)
        	break;
    }

    temp = DRV_Reg32(CLK26CALI_2) & 0xFFFF;

    output = (temp * 26000) / 1024; // Khz

    DRV_WriteReg32(CLK_CFG_9, clk_cfg_9);
    DRV_WriteReg32(CLK_MISC_CFG_1, clk_misc_cfg_1);
    DRV_WriteReg32(CLK26CALI_0, clk26cali_0);

    if(i>10)
        return 0;
    else
        return output;
}

static unsigned int mt_get_mem_freq(void)
{
    int output = 0;
    int i =0;
    unsigned int temp, clk26cali_0, clk_cfg_8, clk_misc_cfg_1;

    clk26cali_0 = DRV_Reg32(CLK26CALI_0);
    DRV_WriteReg32(CLK26CALI_0, clk26cali_0 | 0x80); // enable fmeter_en

    clk_misc_cfg_1 = DRV_Reg32(CLK_MISC_CFG_1);
    DRV_WriteReg32(CLK_MISC_CFG_1, 0xFFFFFF00); // select divider

    clk_cfg_8 = DRV_Reg32(CLK_CFG_8);
    DRV_WriteReg32(CLK_CFG_8, (14 << 8)); // select abist_cksw

    temp = DRV_Reg32(CLK26CALI_0);
    DRV_WriteReg32(CLK26CALI_0, temp | 0x1); // start fmeter

    /* wait frequency meter finish */
    while (DRV_Reg32(CLK26CALI_0) & 0x1)
    {
        mdelay(10);
        i++;
        if(i > 10)
            break;
    }

    temp = DRV_Reg32(CLK26CALI_1) & 0xFFFF;

    output = (temp * 26000) / 1024; // Khz

    DRV_WriteReg32(CLK_CFG_8, clk_cfg_8);
    DRV_WriteReg32(CLK_MISC_CFG_1, clk_misc_cfg_1);
    DRV_WriteReg32(CLK26CALI_0, clk26cali_0);

    if(i>10)
        return 0;
    else
        return output;
}

static unsigned int mt_get_bus_freq(void)
{
    int output = 0;
    int i=0;
    unsigned int temp, clk26cali_0, clk_cfg_9, clk_misc_cfg_1;

    clk26cali_0 = DRV_Reg32(CLK26CALI_0);
    DRV_WriteReg32(CLK26CALI_0, clk26cali_0 | 0x80); // enable fmeter_en

    clk_misc_cfg_1 = DRV_Reg32(CLK_MISC_CFG_1);
    DRV_WriteReg32(CLK_MISC_CFG_1, 0x00FFFFFF); // select divider

    clk_cfg_9 = DRV_Reg32(CLK_CFG_9);
    DRV_WriteReg32(CLK_CFG_9, (1 << 16)); // select ckgen_cksw

    temp = DRV_Reg32(CLK26CALI_0);
    DRV_WriteReg32(CLK26CALI_0, temp | 0x10); // start fmeter

    /* wait frequency meter finish */
    while (DRV_Reg32(CLK26CALI_0) & 0x10)
    {
        mdelay(10);
        i++;
        if(i > 10)
            break;
    }

    temp = DRV_Reg32(CLK26CALI_2) & 0xFFFF;

    output = (temp * 26000) / 1024; // Khz

    DRV_WriteReg32(CLK_CFG_9, clk_cfg_9);
    DRV_WriteReg32(CLK_MISC_CFG_1, clk_misc_cfg_1);
    DRV_WriteReg32(CLK26CALI_0, clk26cali_0);

    if(i>10)
        return 0;
    else
        return output;
}

const char *ckgen_array[] = 
{
    "hf_faxi_ck", "hd_faxi_ck", "hf_fdpi0_ck", "hf_fddrphycfg_ck", "hf_fmm_ck", 
    "f_fpwm_ck", "hf_fvdec_ck", "hf_fmfg_ck", "hf_fcamtg_ck", "f_fuart_ck", 
    "hf_fspi_ck", "f_fusb20_ck", "hf_fmsdc30_0_ck", "hf_fmsdc30_1_ck", "hf_fmsdc30_2_ck",
    "hf_faudio_ck", "hf_faud_intbus_ck", "hf_fpmicspi_ck", "f_frtc_ck", "f_f26m_ck", 
    "f_f32k_md1_ck", "f_frtc_conn_ck", "f_fusb20p1_ck", "hg_fmipicfg_ck", "NULL", 
    "hd_qaxidcm_ck", "NULL", "hf_fscam_ck", "f_fckbus_scan", " f_fckrtc_scan",
    "hf_fatb_ck", "hf_faud_1_ck", "hf_faud_2_ck", "hf_fmsdc50_0_ck", "hf_firda_ck",
    " hf_firtx_ck", "hf_fdisppwm_ck", "hs_fmfg13m_ck"
};

//extern U32 pmic_config_interface (U32 RegNum, U32 val, U32 MASK, U32 SHIFT);
//after pmic_init
void mt_pll_post_init(void)
{
    unsigned int temp;
#ifdef DDR_RESERVE_MODE  
    unsigned int wdt_mode;
    unsigned int wdt_dbg_ctrl;
#endif

    mt_mempll_init(DDR1066, PLL_MODE_1);

#ifdef DDR_RESERVE_MODE  
    wdt_mode = DRV_Reg32(MTK_WDT_MODE);
    wdt_dbg_ctrl = DRV_Reg32(MTK_WDT_DEBUG_CTL);

    print("before mt_mempll_cali, wdt_mode = 0x%x, wdt_dbg_ctrl = 0x%x\n", wdt_mode, wdt_dbg_ctrl);     
    if(((wdt_mode & MTK_WDT_MODE_DDR_RESERVE) !=0) && ((wdt_dbg_ctrl & MTK_DDR_RESERVE_RTA) != 0)) {
        // toggle INFO_CHG for reserve mode
        *((V_UINT32P)(DDRPHY_BASE + (0x0189 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0189 <<2))) & (~pllc1_mempll_n_info_chg));
        *((V_UINT32P)(DDRPHY_BASE + (0x0189 <<2))) = (*((V_UINT32P)(DDRPHY_BASE + (0x0189 <<2))) | (pllc1_mempll_n_info_chg));
        print("[DDR Reserved Mode] toggle mempll freq for sync to high freq!!!\n");
    }
#endif

    /* MEMPLL Calibration */
    #ifdef DDRPHY_3PLL_MODE
        ret = mt_mempll_cali();
    #endif

    //set mem_clk
    DRV_WriteReg32(CLK_CFG_0, 0x01000102); //mem_ck = mempll, bit8=1
    //DRV_WriteReg32(CLK_CFG_0, 0x01000101); //mem_ck = mempll, bit8=1

#if 0
    pmic_config_interface(0x48A,0x68,0x7F,0); // [6:0]: VPROC_VOSEL_ON; VSRAM tracking,Fandy
    pmic_config_interface(0x48C,0x68,0x7F,0); // [6:0]: VPROC_VOSEL_ON; VSRAM tracking,Fandy

    //CA7: INFRA_TOPCKGEN_CKDIV1[4:0](0x10000008)
    temp = DRV_Reg32(TOP_CKDIV1);
    DRV_WriteReg32(TOP_CKDIV1, temp & 0xFFFFFFE0); // CPU clock divide by 1

    //CA7: INFRA_TOPCKGEN_CKMUXSEL[3:2] (0x10000000) =4
    temp = DRV_Reg32(TOP_CKMUXSEL);
    DRV_WriteReg32(TOP_CKMUXSEL, temp | 0x4); // switch CA7_ck to ARMCA7PLL
#endif
    //step 48
    temp = DRV_Reg32(AP_PLL_CON3);
    DRV_WriteReg32(AP_PLL_CON3, temp & 0xFFF44440); // Only UNIVPLL SW Control

    //step 49
    temp = DRV_Reg32(AP_PLL_CON4);
    DRV_WriteReg32(AP_PLL_CON4, temp & 0xFFFFFFF4); // Only UNIVPLL SW Control

    print("mt_pll_post_init: mt_get_mem_freq = %dKhz\n", mt_get_mem_freq());

#if 0
    print("mt_pll_post_init: mt_get_cpu_freq = %dKhz\n", mt_get_cpu_freq());
    print("mt_pll_post_init: mt_get_bus_freq = %dKhz\n", mt_get_bus_freq());

    for(temp=1; temp<39; temp++)
        print("%s: %d\n", ckgen_array[temp-1], mt_get_pll_freq(temp));
#endif

#if 0 /* DCM debug */
    print("mt_pll_post_init: INFRA_GLOBALCON_DCMCTL = 0x%x\n",
	  DRV_Reg32(INFRA_GLOBALCON_DCMCTL));
#endif

    #if 0
    print("mt_pll_post_init: AP_PLL_CON3        = 0x%x, GS = 0x00000000\n", DRV_Reg32(AP_PLL_CON3));
    print("mt_pll_post_init: AP_PLL_CON4        = 0x%x, GS = 0x00000000\n", DRV_Reg32(AP_PLL_CON4));
    print("mt_pll_post_init: AP_PLL_CON6        = 0x%x, GS = 0x00000000\n", DRV_Reg32(AP_PLL_CON6));
    print("mt_pll_post_init: CLKSQ_STB_CON0     = 0x%x, GS = 0x05010501\n", DRV_Reg32(CLKSQ_STB_CON0));
    print("mt_pll_post_init: PLL_ISO_CON0       = 0x%x, GS = 0x00080008\n", DRV_Reg32(PLL_ISO_CON0));
    print("mt_pll_post_init: MAINPLL_CON0       = 0x%x, GS = 0xF1000101\n", DRV_Reg32(MAINPLL_CON0));
    print("mt_pll_post_init: MAINPLL_CON1       = 0x%x, GS = 0x800A8000\n", DRV_Reg32(MAINPLL_CON1));
    print("mt_pll_post_init: MAINPLL_PWR_CON0   = 0x%x, GS = 0x00000001\n", DRV_Reg32(MAINPLL_PWR_CON0));
    print("mt_pll_post_init: UNIVPLL_CON0       = 0x%x, GS = 0xFF000011\n", DRV_Reg32(UNIVPLL_CON0));
    print("mt_pll_post_init: UNIVPLL_CON1       = 0x%x, GS = 0x80180000\n", DRV_Reg32(UNIVPLL_CON1));
    print("mt_pll_post_init: UNIVPLL_PWR_CON0   = 0x%x, GS = 0x00000001\n", DRV_Reg32(UNIVPLL_PWR_CON0));
    print("mt_pll_post_init: MMPLL_CON0         = 0x%x, GS = 0x00000101\n", DRV_Reg32(MMPLL_CON0));
    print("mt_pll_post_init: MMPLL_CON1         = 0x%x, GS = 0x820D8000\n", DRV_Reg32(MMPLL_CON1));
    print("mt_pll_post_init: MMPLL_PWR_CON0     = 0x%x, GS = 0x00000001\n", DRV_Reg32(MMPLL_PWR_CON0));
    print("mt_pll_post_init: MSDCPLL_CON0       = 0x%x, GS = 0x00000111\n", DRV_Reg32(MSDCPLL_CON0));
    print("mt_pll_post_init: MSDCPLL_CON1       = 0x%x, GS = 0x800F6276\n", DRV_Reg32(MSDCPLL_CON1));
    print("mt_pll_post_init: MSDCPLL_PWR_CON0   = 0x%x, GS = 0x00000001\n", DRV_Reg32(MSDCPLL_PWR_CON0));
    print("mt_pll_post_init: TVDPLL_CON0        = 0x%x, GS = 0x00000101\n", DRV_Reg32(TVDPLL_CON0));
    print("mt_pll_post_init: TVDPLL_CON1        = 0x%x, GS = 0x80112276\n", DRV_Reg32(TVDPLL_CON1));
    print("mt_pll_post_init: TVDPLL_PWR_CON0    = 0x%x, GS = 0x00000001\n", DRV_Reg32(TVDPLL_PWR_CON0));
    print("mt_pll_post_init: VENCPLL_CON0       = 0x%x, GS = 0x00000111\n", DRV_Reg32(VENCPLL_CON0));
    print("mt_pll_post_init: VENCPLL_CON1       = 0x%x, GS = 0x800E989E\n", DRV_Reg32(VENCPLL_CON1));
    print("mt_pll_post_init: VENCPLL_PWR_CON0   = 0x%x, GS = 0x00000001\n", DRV_Reg32(VENCPLL_PWR_CON0));
    print("mt_pll_post_init: APLL1_CON0         = 0x%x, GS = 0xF0000131\n", DRV_Reg32(APLL1_CON0));
    print("mt_pll_post_init: APLL1_CON1         = 0x%x, GS = 0xB7945EA6\n", DRV_Reg32(APLL1_CON1));
    print("mt_pll_post_init: APLL1_PWR_CON0     = 0x%x, GS = 0x00000001\n", DRV_Reg32(APLL1_PWR_CON0));
    print("mt_pll_post_init: APLL2_CON0         = 0x%x, GS = 0x00000131\n", DRV_Reg32(APLL2_CON0));
    print("mt_pll_post_init: APLL2_CON1         = 0x%x, GS = 0xBC7EA932\n", DRV_Reg32(APLL2_CON1));
    print("mt_pll_post_init: APLL2_PWR_CON0     = 0x%x, GS = 0x00000001\n", DRV_Reg32(APLL2_PWR_CON0));
    
    print("mt_pll_post_init: SPM_PWR_STATUS     = 0x%x, \n", DRV_Reg32(SPM_PWR_STATUS));
    print("mt_pll_post_init: DISP_CG_CON0       = 0x%x, \n", DRV_Reg32(DISP_CG_CON0));
    print("mt_pll_post_init: DISP_CG_CON1       = 0x%x, \n", DRV_Reg32(DISP_CG_CON1));
    #endif    


}

#if 0
//after pmic_init
void mt_arm_pll_sel(void)
{
    unsigned int temp;
    
    temp = DRV_Reg32(TOP_CKMUXSEL);
    //DRV_WriteReg32(TOP_CKMUXSEL, temp | 0x5); // switch CA7_ck to ARMCA7PLL, and CA15_ck to ARMCA15PLL
    DRV_WriteReg32(TOP_CKMUXSEL, temp | 0x1); // switch CA7_ck to ARMCA7PLL

    print("[PLL] mt_arm_pll_sel done\n");
}
#endif

void mt_pll_init(void)
{
    int ret = 0;
    unsigned int temp;

    DRV_WriteReg32(ACLKEN_DIV, 0x12); // MCU Bus DIV2

    //step 1
    DRV_WriteReg32(CLKSQ_STB_CON0, 0x98940501); // reduce CLKSQ disable time
    
    //step 2
    DRV_WriteReg32(PLL_ISO_CON0, 0x00080008); // extend PWR/ISO control timing to 1us
    
    //step 3
    DRV_WriteReg32(AP_PLL_CON6, 0x00000000); //

    /*************
    * xPLL PWR ON 
    **************/
    //step 4
    temp = DRV_Reg32(ARMPLL_PWR_CON0);
    DRV_WriteReg32(ARMPLL_PWR_CON0, temp | 0x1);

    //step 5
    temp = DRV_Reg32(MAINPLL_PWR_CON0);
    DRV_WriteReg32(MAINPLL_PWR_CON0, temp | 0x1);
    
    //step 6
    temp = DRV_Reg32(UNIVPLL_PWR_CON0);
    DRV_WriteReg32(UNIVPLL_PWR_CON0, temp | 0x1);
    
    //step 7
    temp = DRV_Reg32(MMPLL_PWR_CON0);
    DRV_WriteReg32(MMPLL_PWR_CON0, temp | 0x1);
    
    //step 8
    temp = DRV_Reg32(MSDCPLL_PWR_CON0);
    DRV_WriteReg32(MSDCPLL_PWR_CON0, temp | 0x1);
    
    //step 9
    temp = DRV_Reg32(VENCPLL_PWR_CON0);
    DRV_WriteReg32(VENCPLL_PWR_CON0, temp | 0x1);
    
    //step 10
    temp = DRV_Reg32(TVDPLL_PWR_CON0);
    DRV_WriteReg32(TVDPLL_PWR_CON0, temp | 0x1);

    //step 11
//    temp = DRV_Reg32(MPLL_PWR_CON0);
//    DRV_WriteReg32(MPLL_PWR_CON0, temp | 0x1);
    
    //step 12
    //temp = DRV_Reg32(VCODECPLL_PWR_CON0);
    //DRV_WriteReg32(VCODECPLL_PWR_CON0, temp | 0x1);

    //step 13
    temp = DRV_Reg32(APLL1_PWR_CON0);
    DRV_WriteReg32(APLL1_PWR_CON0, temp | 0x1);
    
    //step 14
    temp = DRV_Reg32(APLL2_PWR_CON0);
    DRV_WriteReg32(APLL2_PWR_CON0, temp | 0x1);

    gpt_busy_wait_us(5); // wait for xPLL_PWR_ON ready (min delay is 1us)

    /******************
    * xPLL ISO Disable
    *******************/
    //step 15
    temp = DRV_Reg32(ARMPLL_PWR_CON0);
    DRV_WriteReg32(ARMPLL_PWR_CON0, temp & 0xFFFFFFFD);
    
    //step 16
    temp = DRV_Reg32(MAINPLL_PWR_CON0);
    DRV_WriteReg32(MAINPLL_PWR_CON0, temp & 0xFFFFFFFD);
    
    //step 17
    temp = DRV_Reg32(UNIVPLL_PWR_CON0);
    DRV_WriteReg32(UNIVPLL_PWR_CON0, temp & 0xFFFFFFFD);
    
    //step 18
    temp = DRV_Reg32(MMPLL_PWR_CON0);
    DRV_WriteReg32(MMPLL_PWR_CON0, temp & 0xFFFFFFFD);
    
    //step 19
    temp = DRV_Reg32(MSDCPLL_PWR_CON0);
    DRV_WriteReg32(MSDCPLL_PWR_CON0, temp & 0xFFFFFFFD);
    
    //step 20
    temp = DRV_Reg32(VENCPLL_PWR_CON0);
    DRV_WriteReg32(VENCPLL_PWR_CON0, temp & 0xFFFFFFFD);
    
    //step 21
    temp = DRV_Reg32(TVDPLL_PWR_CON0);
    DRV_WriteReg32(TVDPLL_PWR_CON0, temp & 0xFFFFFFFD);
    
    //step 22
//    temp = DRV_Reg32(MPLL_PWR_CON0);
//    DRV_WriteReg32(MPLL_PWR_CON0, temp & 0xFFFFFFFD);
    
    //step 23
    //temp = DRV_Reg32(VCODECPLL_PWR_CON0);
    //DRV_WriteReg32(VCODECPLL_PWR_CON0, temp & 0xFFFFFFFD);
    
    //step 24
    temp = DRV_Reg32(APLL1_PWR_CON0);
    DRV_WriteReg32(APLL1_PWR_CON0, temp & 0xFFFFFFFD);
    
    //step 25
    temp = DRV_Reg32(APLL2_PWR_CON0);
    DRV_WriteReg32(APLL2_PWR_CON0, temp & 0xFFFFFFFD);

    /********************
    * xPLL Frequency Set
    *********************/
    //step 26        
    DRV_WriteReg32(ARMPLL_CON1, 0x810F4000); // 793MHz
    
    //step 27
    DRV_WriteReg32(MAINPLL_CON1, 0x800A8000); //1092MHz

    //20150126, Austin
    DRV_WriteReg32(UNIVPLL_CON1, 0x81180000);
	
    //step 28
    DRV_WriteReg32(MMPLL_CON1, 0x82114EC4); //450MHz
    
    //step 29
    DRV_WriteReg32(MSDCPLL_CON1, 0x810F6276); //800MHz
    
    //step 30
    //DRV_WriteReg32(VENCPLL_CON1, 0x820B89D8); //300MHz, /4
    //DRV_WriteReg32(VENCPLL_CON1, 0x831713B1); //300MHz, /8 for VCORE-DVFS
    DRV_WriteReg32(VENCPLL_CON1, 0x830E0000); //182MHz, /8 for VCORE-DVFS
    
    //step 31
    DRV_WriteReg32(TVDPLL_CON1, 0x830D89D8); // 176MHz

    //step 32
    //FIXME, change to 208MHz
    //DRV_WriteReg32(MPLL_CON1, 0x801C0000);
    //DRV_WriteReg32(MPLL_CON0, 0x00010110); //52MHz
    //DRV_WriteReg32(MPLL_CON1, 0x80100000);
    //DRV_WriteReg32(MPLL_CON0, 0x00010130); //208MHz
    
    //step 33
//#if 1
//    DRV_WriteReg32(VCODECPLL_CON1, 0x80130000); // 494MHz
//#else
//    DRV_WriteReg32(VCODECPLL_CON1, 0x80150000); // 546MHz
//#endif

    //APLL1 and APLL2 use the default setting
//    DRV_WriteReg32(APLL1_CON1, 0xBC7EA932); // 98.304MHz
    DRV_WriteReg32(APLL2_CON1, 0xB7945EA6); // 90.3168MHz    
    /***********************
    * xPLL Frequency Enable
    ************************/
    //step 34
    temp = DRV_Reg32(ARMPLL_CON0);
    DRV_WriteReg32(ARMPLL_CON0, temp | 0x1);
    
    //step 35
    temp = DRV_Reg32(MAINPLL_CON0);
    DRV_WriteReg32(MAINPLL_CON0, temp | 0x1);
    
    //step 36
    temp = DRV_Reg32(UNIVPLL_CON0);
    DRV_WriteReg32(UNIVPLL_CON0, temp | 0x1);
    
    //step 37
    temp = DRV_Reg32(MMPLL_CON0);
    DRV_WriteReg32(MMPLL_CON0, temp | 0x1);
    
    //step 38
    temp = DRV_Reg32(MSDCPLL_CON0);
    DRV_WriteReg32(MSDCPLL_CON0, temp | 0x1);
    
    //step 39
    temp = DRV_Reg32(VENCPLL_CON0);
    DRV_WriteReg32(VENCPLL_CON0, temp | 0x1);
    
    //step 40
    temp = DRV_Reg32(TVDPLL_CON0);
    DRV_WriteReg32(TVDPLL_CON0, temp | 0x1); 

    //step 41
    //temp = DRV_Reg32(MPLL_CON0);
    //DRV_WriteReg32(MPLL_CON0, temp | 0x1); 
    
    //step 42
    //temp = DRV_Reg32(VCODECPLL_CON0);
    //DRV_WriteReg32(VCODECPLL_CON0, temp | 0x1); 
    
    //step 43
    temp = DRV_Reg32(APLL1_CON0);
    DRV_WriteReg32(APLL1_CON0, temp | 0x1); 
    
    //step 44
    temp = DRV_Reg32(APLL2_CON0);
    DRV_WriteReg32(APLL2_CON0, temp | 0x1); 
    
    gpt_busy_wait_us(40); // wait for PLL stable (min delay is 20us)

    /***************
    * xPLL DIV RSTB
    ****************/
    //step 45
    //temp = DRV_Reg32(ARMPLL_CON0);
    //DRV_WriteReg32(ARMPLL_CON0, temp | 0x01000000);
    
    //step 46
    temp = DRV_Reg32(MAINPLL_CON0);
    DRV_WriteReg32(MAINPLL_CON0, temp | 0x01000000);
    
    //step 47
    temp = DRV_Reg32(UNIVPLL_CON0);
    DRV_WriteReg32(UNIVPLL_CON0, temp | 0x01000000);

    /*****************
    * xPLL HW Control
    ******************/
#if 0
    //default is SW mode, set HW mode after MEMPLL caribration
    //step 48
    temp = DRV_Reg32(AP_PLL_CON3);
    DRV_WriteReg32(AP_PLL_CON3, temp & 0xFFF44440); // Only UNIVPLL SW Control

    //step 49
    temp = DRV_Reg32(AP_PLL_CON4);
    DRV_WriteReg32(AP_PLL_CON4, temp & 0xFFFFFFF4); // Only UNIVPLL SW Control
#endif
    /*************
    * MEMPLL Init
    **************/

//    mt_mempll_pre();

    /**************
    * INFRA CLKMUX
    ***************/

    temp = DRV_Reg32(TOP_DCMCTL);
    DRV_WriteReg32(TOP_DCMCTL, temp | 0x1); // Enable INFRA Bus Divider

    /**************
    * Enable Infra DCM
    ***************/
    DRV_WriteReg32(INFRA_GLOBALCON_DCMDBC,
              aor(DRV_Reg32(INFRA_GLOBALCON_DCMDBC),
		  ~INFRA_GLOBALCON_DCMDBC_MASK, INFRA_GLOBALCON_DCMDBC_ON));
    DRV_WriteReg32(INFRA_GLOBALCON_DCMFSEL,
              aor(DRV_Reg32(INFRA_GLOBALCON_DCMFSEL),
		  ~INFRA_GLOBALCON_DCMFSEL_MASK, INFRA_GLOBALCON_DCMFSEL_ON));
    DRV_WriteReg32(INFRA_GLOBALCON_DCMCTL,
              aor(DRV_Reg32(INFRA_GLOBALCON_DCMCTL),
		  ~INFRA_GLOBALCON_DCMCTL_MASK, INFRA_GLOBALCON_DCMCTL_ON));

#if 1    
    //CA7: INFRA_TOPCKGEN_CKDIV1[4:0](0x10000008)
    temp = DRV_Reg32(TOP_CKDIV1);
    DRV_WriteReg32(TOP_CKDIV1, temp & 0xFFFFFFE0); // CPU clock divide by 1

    //CA7: INFRA_TOPCKGEN_CKMUXSEL[3:2] (0x10000000) =4
    temp = DRV_Reg32(TOP_CKMUXSEL);
    DRV_WriteReg32(TOP_CKMUXSEL, temp | 0x4); // switch CA7_ck to ARMCA7PLL
#endif
    /************
    * TOP CLKMUX
    *************/

    DRV_WriteReg32(CLK_CFG_0, 0x01000002);//mm_ck=vencpll, ddrphycfg_ck=26M, mem_ck=26M, axi=syspll_d5
    //DRV_WriteReg32(CLK_CFG_0, 0x01000001);//mm_ck=vencpll, ddrphycfg_ck=26M, mem_ck=26M, axi=syspll1_d2

    //DRV_WriteReg32(CLK_CFG_1, 0x01010180);//camtg=univpll_d26 , mfg_ck=mmpll_ck, vdec_ck=syspll_d2, pwm_ck= gating
    DRV_WriteReg32(CLK_CFG_1, 0x01010480);//camtg=univpll_d26 , mfg_ck=mmpll_ck, vdec_ck=syspll1_d2, pwm_ck= gating

    DRV_WriteReg32(CLK_CFG_2, 0x01010100);//msdc50_0=syspll1_d2, usb20=univpll1_d8, spi_ck=syspll3_d2, uart=26M

    DRV_WriteReg32(CLK_CFG_3, 0x07020202);//msdc30_3=msdcpll_d16, msdc30_2=msdcpll_d4, msdc30_1=msdcpll_d4, msdc30_0=msdcpll_d2, 

    DRV_WriteReg32(CLK_CFG_4, 0x01000100);//scp_ck=syspll1_d8, pmicspi=26MHz, aud_intbus=syspll1_d4, aud_ck=26M,

    DRV_WriteReg32(CLK_CFG_5, 0x01010101);//mfg13m=ad_sys_26M_d2, scam_ck=syspll3_d2, dpi0_ck=tvdpll, atb_ck=syspll1_d2, 

    DRV_WriteReg32(CLK_CFG_6, 0x01010001);//irtx_ck=ad_sys_26M_d2 , irda_ck=univpll2_d4, aud2_ck=26M , aud1_ck=apll1, 
    
    DRV_WriteReg32(CLK_CFG_7, 0x00000000);//NULL, NULL, NULL, disppwm=26M,

    DRV_WriteReg32(CLK_SCP_CFG_0, 0x3FF); // enable scpsys clock off control
    DRV_WriteReg32(CLK_SCP_CFG_1, 0x11); // enable scpsys clock off control

    /*for MTCMOS*/
    spm_write(SPM_POWERON_CONFIG_SET, (SPM_PROJECT_CODE << 16) | (1U << 0));
    spm_mtcmos_ctrl_disp(STA_POWER_ON);
#if 0
    temp = seclib_get_devinfo_with_index(4) & (1<<15);
    if(temp == 0)
        spm_mtcmos_ctrl_mdsys2(STA_POWER_ON);
#endif	
#if 0
    spm_mtcmos_ctrl_vdec(STA_POWER_ON);
    //spm_mtcmos_ctrl_venc(STA_POWER_ON);
    spm_mtcmos_ctrl_isp(STA_POWER_ON);
    spm_mtcmos_ctrl_mfg(STA_POWER_ON);
    spm_mtcmos_ctrl_connsys(STA_POWER_ON);
#endif
    /*for CG*/
    DRV_WriteReg32(INFRA_PDN_CLR0, 0xFFFFFFFF);
    DRV_WriteReg32(PERI_PDN_CLR0, 0xFFFFFFFF);
    /*DISP CG*/
    //DRV_WriteReg32(DISP_CG_CLR0, 0xFFFFFFFF);
    //DRV_WriteReg32(DISP_CG_CLR1, 0x3F);
#if 0
    //AUDIO
    DRV_WriteReg32(AUDIO_TOP_CON0, 0);
    //MFG
    DRV_WriteReg32(MFG_CG_CLR, 0x00000001);
    //ISP
    DRV_WriteReg32(IMG_CG_CLR, 0x000003F1);
    //VDE
    DRV_WriteReg32(VDEC_CKEN_SET, 0x00000001);
    DRV_WriteReg32(LARB_CKEN_SET, 0x00000001);
    //VENC
    //DRV_WriteReg32(VENC_CG_SET, 0x00001111);
#endif

}

#if 0
int spm_mtcmos_ctrl_disp(int state)
{
    int err = 0;
    volatile unsigned int val;
    unsigned long flags;

    spm_write(SPM_POWERON_CONFIG_SET, (SPM_PROJECT_CODE << 16) | (1U << 0));

    if (state == STA_POWER_DOWN) {
        
        spm_write(SPM_DIS_PWR_CON, spm_read(SPM_DIS_PWR_CON) | SRAM_PDN);
#if 1
        while ((spm_read(SPM_DIS_PWR_CON) & DIS_SRAM_ACK) != DIS_SRAM_ACK) {
        }
#endif
        spm_write(SPM_DIS_PWR_CON, spm_read(SPM_DIS_PWR_CON) | PWR_ISO);

        val = spm_read(SPM_DIS_PWR_CON);
        val = (val & ~PWR_RST_B) | PWR_CLK_DIS;
        spm_write(SPM_DIS_PWR_CON, val);

        spm_write(SPM_DIS_PWR_CON, spm_read(SPM_DIS_PWR_CON) & ~(PWR_ON | PWR_ON_S));

        while ((spm_read(SPM_PWR_STATUS) & DIS_PWR_STA_MASK)
                || (spm_read(SPM_PWR_STATUS_2ND) & DIS_PWR_STA_MASK)) {
        }
    } else {    /* STA_POWER_ON */
        spm_write(SPM_DIS_PWR_CON, spm_read(SPM_DIS_PWR_CON) | PWR_ON);
        spm_write(SPM_DIS_PWR_CON, spm_read(SPM_DIS_PWR_CON) | PWR_ON_S);

        while (!(spm_read(SPM_PWR_STATUS) & DIS_PWR_STA_MASK) 
                || !(spm_read(SPM_PWR_STATUS_2ND) & DIS_PWR_STA_MASK)) {
        }

        spm_write(SPM_DIS_PWR_CON, spm_read(SPM_DIS_PWR_CON) & ~PWR_CLK_DIS);
        spm_write(SPM_DIS_PWR_CON, spm_read(SPM_DIS_PWR_CON) & ~PWR_ISO);
        spm_write(SPM_DIS_PWR_CON, spm_read(SPM_DIS_PWR_CON) | PWR_RST_B);

        spm_write(SPM_DIS_PWR_CON, spm_read(SPM_DIS_PWR_CON) & ~SRAM_PDN);

#if 1
        while ((spm_read(SPM_DIS_PWR_CON) & DIS_SRAM_ACK)) {
        }
#endif
    }

    return err;
}
#endif


