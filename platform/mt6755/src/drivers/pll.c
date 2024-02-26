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

#include "dramc_pi_api.h"
#include "dramc_common.h"
#include "dramc_register.h"
#include "wdt.h"
#include "emi.h"

#include "../security/inc/sec_devinfo.h"



#if 1
unsigned int mt_get_cpu_freq(void)
{
	int output = 0, i = 0;
    unsigned int temp = 0, clk26cali_0 = 0, clk_dbg_cfg = 0, clk_misc_cfg_0 = 0, clk26cali_1 = 0;

    clk_dbg_cfg = DRV_Reg32(CLK_DBG_CFG);
    DRV_WriteReg32(CLK_DBG_CFG, (clk_dbg_cfg & 0xFFFFFFFE)|(20 << 16)); //sel abist_cksw and enable freq meter sel abist
    
    clk_misc_cfg_0 = DRV_Reg32(CLK_MISC_CFG_0);
    DRV_WriteReg32(CLK_MISC_CFG_0, (clk_misc_cfg_0 & 0x00FFFFFF)); // select divider, WAIT CONFIRM

    clk26cali_1 = DRV_Reg32(CLK26CALI_1);
    //DRV_WriteReg32(CLK26CALI_1, 0x00ff0000); // cycle count default 1024,[25:16]=3FF

    //temp = DRV_Reg32(CLK26CALI_0);
    DRV_WriteReg32(CLK26CALI_0, 0x1000);
    DRV_WriteReg32(CLK26CALI_0, 0x1010);

    /* wait frequency meter finish */
    while (DRV_Reg32(CLK26CALI_0) & 0x10)
    {
        mdelay(10);
        i++;
        if(i > 10)
        	break;
    }

    temp = DRV_Reg32(CLK26CALI_1) & 0xFFFF;

    output = ((temp * 26000) ) / 1024; // Khz

    DRV_WriteReg32(CLK_DBG_CFG, clk_dbg_cfg);
    DRV_WriteReg32(CLK_MISC_CFG_0, clk_misc_cfg_0);
    DRV_WriteReg32(CLK26CALI_0, clk26cali_0);
    DRV_WriteReg32(CLK26CALI_1, clk26cali_1);

    /*print("CA15 freq = %d\n", output);*/

    if(i>10)
        return 0;
    else
        return output;
}


unsigned int mt_get_mem_freq(void)
{
    int output = 0, i = 0;
    unsigned int temp = 0, clk26cali_0 = 0, clk_dbg_cfg = 0, clk_misc_cfg_0 = 0, clk26cali_1 = 0;

    clk_dbg_cfg = DRV_Reg32(CLK_DBG_CFG);
    DRV_WriteReg32(CLK_DBG_CFG, (clk_dbg_cfg & 0xFFFFFFFE)|(37 << 16)); //sel abist_cksw and enable abist meter sel abist

    clk_misc_cfg_0 = DRV_Reg32(CLK_MISC_CFG_0);
    DRV_WriteReg32(CLK_MISC_CFG_0, (clk_misc_cfg_0 & 0x00FFFFFF)); // select divider

    clk26cali_1 = DRV_Reg32(CLK26CALI_1);
    //DRV_WriteReg32(CLK26CALI_1, 0x00ff0000); // 

    //temp = DRV_Reg32(CLK26CALI_0);
    DRV_WriteReg32(CLK26CALI_0, 0x1000);
    DRV_WriteReg32(CLK26CALI_0, 0x1010);

    /* wait frequency meter finish */
    while (DRV_Reg32(CLK26CALI_0)& 0x10)
    {
        /*print("wait for frequency meter finish, CLK26CALI = 0x%x\n", DRV_Reg32(CLK26CALI_0));*/
        mdelay(10);
		i++;
        if(i > 10)
        	break;
    }


    temp = DRV_Reg32(CLK26CALI_1) & 0xFFFF;

    output = ((temp * 26000) ) / 1024; // Khz

    DRV_WriteReg32(CLK_DBG_CFG, clk_dbg_cfg);
    DRV_WriteReg32(CLK_MISC_CFG_0, clk_misc_cfg_0);
    DRV_WriteReg32(CLK26CALI_0, clk26cali_0);
    DRV_WriteReg32(CLK26CALI_1, clk26cali_1);

    /*print("CLK26CALI = 0x%x, mem bus frequency = %d Khz\n", temp, output);*/
	if(i>10)
        return 0;
    else
        return output;

}

static unsigned int mt_get_pll_freq(unsigned int ID)
{
    int output = 0;
    unsigned int temp = 0, clk26cali_0 = 0, clk_dbg_cfg = 0, clk_misc_cfg_0 = 0, clk26cali_1 = 0;

    clk_dbg_cfg = DRV_Reg32(CLK_DBG_CFG);
    DRV_WriteReg32(CLK_DBG_CFG, (ID << 8) | 0x01); //sel ckgen_cksw[0] and enable freq meter sel ckgen[13:8], 01:hd_faxi_ck

    clk_misc_cfg_0 = DRV_Reg32(CLK_MISC_CFG_0);
    //DRV_WriteReg32(CLK_MISC_CFG_0, (clk_misc_cfg_0 & 0x00FFFFFF) | (0x07 << 24)); // select divider?dvt set zero
    DRV_WriteReg32(CLK_MISC_CFG_0, (clk_misc_cfg_0 & 0x00FFFFFF)); // select divider?dvt set zero

    clk26cali_1 = DRV_Reg32(CLK26CALI_1);
    //DRV_WriteReg32(CLK26CALI_1, 0x00ff0000); // 

    //temp = DRV_Reg32(CLK26CALI_0);
    DRV_WriteReg32(CLK26CALI_0, 0x1000);
    DRV_WriteReg32(CLK26CALI_0, 0x1010);

    /* wait frequency meter finish */
    while (DRV_Reg32(CLK26CALI_0) & 0x10)
    {
        /*print("wait for frequency meter finish, CLK26CALI = 0x%x\n", DRV_Reg32(CLK26CALI_0));*/
        mdelay(10);
    }

    temp = DRV_Reg32(CLK26CALI_1) & 0xFFFF;

    output = ((temp * 26000) ) / 1024; // Khz

    DRV_WriteReg32(CLK_DBG_CFG, clk_dbg_cfg);
    DRV_WriteReg32(CLK_MISC_CFG_0, clk_misc_cfg_0);
    DRV_WriteReg32(CLK26CALI_0, clk26cali_0);
    DRV_WriteReg32(CLK26CALI_1, clk26cali_1);

    /*print("CLK26CALI = 0x%x, frequency meter[%d] = %d Khz\n", temp, ID, output);*/

    return output;
}

unsigned int mt_get_bus_freq(void)
{
    int output = 0;
    unsigned int temp = 0, clk26cali_0 = 0, clk_dbg_cfg = 0, clk_misc_cfg_0 = 0, clk26cali_1 = 0;

    clk_dbg_cfg = DRV_Reg32(CLK_DBG_CFG);
    DRV_WriteReg32(CLK_DBG_CFG, (1 << 8) | 0x01); //sel abist_cksw and enable freq meter sel abist

    clk_misc_cfg_0 = DRV_Reg32(CLK_MISC_CFG_0);
    DRV_WriteReg32(CLK_MISC_CFG_0, (clk_misc_cfg_0 & 0x00FFFFFF)); // select divider

    clk26cali_1 = DRV_Reg32(CLK26CALI_1);
    //DRV_WriteReg32(CLK26CALI_1, 0x00ff0000); // 

    //temp = DRV_Reg32(CLK26CALI_0);
    DRV_WriteReg32(CLK26CALI_0, 0x1000);
    DRV_WriteReg32(CLK26CALI_0, 0x1010);

    /* wait frequency meter finish */
    while (DRV_Reg32(CLK26CALI_0) & 0x10)
    {
        /*print("wait for frequency meter finish, CLK26CALI = 0x%x\n", DRV_Reg32(CLK26CALI_0));*/
        mdelay(10);
    }

    temp = DRV_Reg32(CLK26CALI_1) & 0xFFFF;

    output = ((temp * 26000) ) / 1024; // Khz

    DRV_WriteReg32(CLK_DBG_CFG, clk_dbg_cfg);
    DRV_WriteReg32(CLK_MISC_CFG_0, clk_misc_cfg_0);
    DRV_WriteReg32(CLK26CALI_0, clk26cali_0);
    DRV_WriteReg32(CLK26CALI_1, clk26cali_1);

    /*print("CLK26CALI = 0x%x, axi bus frequency = %d Khz\n", temp, output);*/

    return output;
}
#endif

const char *ckgen_array[] = 
{
    "hd_faxi_ck", "hf_fddrphycfg_ck", "hf_fmm_ck", "f_fpwm_ck", "hf_fvdec_ck", 
    "hf_fmfg_ck", "hf_fcamtg_ck", "f_fuart_ck", "hf_fspi_ck", "hf_fmsdc50_0_h_ck", 
    "hf_fmsdc50_0_ck", "hf_fmsdc30_1_ck", "hf_fmsdc30_2_ck", "hf_fmsdc30_3_ck",
    "hf_faudio_ck", "hf_faud_intbus_ck", "hf_fpmicspi_ck", "NULL", "hf_fatb_ck", 
    "hf_fmjc_ck", "hf_fdpi0_ck", "hf_fscam_ck", "hf_faud_1_ck", "hf_faud_2_ck", 
    "eco_ao12_mem_mux_ck", "eco_ao12_mem_dcm_ck", "f_fdisp_pwm_ck", "f_fssusb_top_sys_ck", "f_fssusb_top_xhci_ck",
    "f_fusb_top_ck", "hg_fspm_ck", "hf_fbsi_spi_ck", "f_fi2c_ck", "hg_fdvfsp_c"
};

//after pmic_init
void mt_pll_post_init(void)
{
    unsigned int temp;
    /*print("Pll post init start...\n");*/
    //step 48
    temp = DRV_Reg32(AP_PLL_CON3);
    DRV_WriteReg32(AP_PLL_CON3, temp & 0xFFFCCCCC); // Only CKSQ0 & MAINPLL & ARMCA7 SPM Control

    //step 49
    temp = DRV_Reg32(AP_PLL_CON4);
    DRV_WriteReg32(AP_PLL_CON4, temp & 0xFFFFFCFC); // Only MAINPLL & ARMCA7 SPM Control

#if 0
	print("bus_freq = %dKhz\n", mt_get_bus_freq());
    print("cpu_freq = %dKhz\n", mt_get_cpu_freq());    
    print("mem_freq = %dKhz\n", mt_get_mem_freq());
    
    for(temp=1; temp<35; temp++)
    {
    	  if((temp==18)||(temp==25)){}
    	  else
        	print("%s: %d\n", ckgen_array[temp-1], mt_get_pll_freq(temp)); 
    }


    print("mt_pll_post_init: AP_PLL_CON3        = 0x%x, GS = 0xFFFCCCCC\n", DRV_Reg32(AP_PLL_CON3));
    print("mt_pll_post_init: AP_PLL_CON4        = 0x%x, GS = 0xFFFFFCFC\n", DRV_Reg32(AP_PLL_CON4));
    print("mt_pll_post_init: AP_PLL_CON6        = 0x%x, GS = 0x00000000\n", DRV_Reg32(AP_PLL_CON6));
    
    print("mt_pll_post_init: CLKSQ_STB_CON0     = 0x%x, GS = 0x05010501\n", DRV_Reg32(CLKSQ_STB_CON0));//Step.1
    print("mt_pll_post_init: PLL_ISO_CON0       = 0x%x, GS = 0x00080008\n", DRV_Reg32(PLL_ISO_CON0));//2
    //un-list set 0
    print("mt_pll_post_init: ARMCA15PLL_CON0     = 0x%x, GS = 0x00000101\n", DRV_Reg32(ARMCA15PLL_CON0));
    print("mt_pll_post_init: ARMCA15PLL_CON1     = 0x%x, GS = 0x810B0000\n", DRV_Reg32(ARMCA15PLL_CON1));
    print("mt_pll_post_init: ARMCA15PLL_PWR_CON0 = 0x%x, GS = 0x00000001\n", DRV_Reg32(ARMCA15PLL_PWR_CON0));
    
    print("mt_pll_post_init: ARMCA7PLL_CON0     = 0x%x, GS = 0xF1000101\n", DRV_Reg32(ARMCA7PLL_CON0));
    print("mt_pll_post_init: ARMCA7PLL_CON1     = 0x%x, GS = 0x810D4000\n", DRV_Reg32(ARMCA7PLL_CON1));
    print("mt_pll_post_init: ARMCA7PLL_PWR_CON0 = 0x%x, GS = 0x00000001\n", DRV_Reg32(ARMCA7PLL_PWR_CON0));
    
    print("mt_pll_post_init: MAINPLL_CON0       = 0x%x, GS = 0xF1000101\n", DRV_Reg32(MAINPLL_CON0));
    print("mt_pll_post_init: MAINPLL_CON1       = 0x%x, GS = 0x800A8000\n", DRV_Reg32(MAINPLL_CON1));
    print("mt_pll_post_init: MAINPLL_PWR_CON0   = 0x%x, GS = 0x00000001\n", DRV_Reg32(MAINPLL_PWR_CON0));
    print("mt_pll_post_init: UNIVPLL_CON0       = 0x%x, GS = 0xFF000011\n", DRV_Reg32(UNIVPLL_CON0));
    print("mt_pll_post_init: UNIVPLL_CON1       = 0x%x, GS = 0x80180000\n", DRV_Reg32(UNIVPLL_CON1));//Jade PLL frequency mapping
    print("mt_pll_post_init: UNIVPLL_PWR_CON0   = 0x%x, GS = 0x00000001\n", DRV_Reg32(UNIVPLL_PWR_CON0));
    print("mt_pll_post_init: MMPLL_CON0         = 0x%x, GS = 0x00000101\n", DRV_Reg32(MMPLL_CON0));
    print("mt_pll_post_init: MMPLL_CON1         = 0x%x, GS = 0x82140000\n", DRV_Reg32(MMPLL_CON1));
    print("mt_pll_post_init: MMPLL_PWR_CON0     = 0x%x, GS = 0x00000001\n", DRV_Reg32(MMPLL_PWR_CON0));
    print("mt_pll_post_init: MSDCPLL_CON0       = 0x%x, GS = 0x00000121\n", DRV_Reg32(MSDCPLL_CON0));//1600, posdiv=4, out 400
    print("mt_pll_post_init: MSDCPLL_CON1       = 0x%x, GS = 0x800F6276\n", DRV_Reg32(MSDCPLL_CON1));
    print("mt_pll_post_init: MSDCPLL_PWR_CON0   = 0x%x, GS = 0x00000001\n", DRV_Reg32(MSDCPLL_PWR_CON0));
    print("mt_pll_post_init: TVDPLL_CON0        = 0x%x, GS = 0xC0000121\n", DRV_Reg32(TVDPLL_CON0));
    print("mt_pll_post_init: TVDPLL_CON1        = 0x%x, GS = 0x8016D89E\n", DRV_Reg32(TVDPLL_CON1));//freq choice:445/594
    print("mt_pll_post_init: TVDPLL_PWR_CON0    = 0x%x, GS = 0x00000001\n", DRV_Reg32(TVDPLL_PWR_CON0));
    print("mt_pll_post_init: VENCPLL_CON0       = 0x%x, GS = 0x00000121\n", DRV_Reg32(VENCPLL_CON0));
    print("mt_pll_post_init: VENCPLL_CON1       = 0x%x, GS = 0x800B0000\n", DRV_Reg32(VENCPLL_CON1));
    print("mt_pll_post_init: VENCPLL_PWR_CON0   = 0x%x, GS = 0x00000001\n", DRV_Reg32(VENCPLL_PWR_CON0));
    //print("mt_pll_post_init: MPLL_CON0          = 0x%x, GS = 0x00010111\n", DRV_Reg32(MPLL_CON0));
    //print("mt_pll_post_init: MPLL_CON1          = 0x%x, GS = 0x801C0000\n", DRV_Reg32(MPLL_CON1));
    //print("mt_pll_post_init: MPLL_PWR_CON0      = 0x%x, GS = 0x00000001\n", DRV_Reg32(MPLL_PWR_CON0));
    print("mt_pll_post_init: APLL1_CON0         = 0x%x, GS = 0x00000131\n", DRV_Reg32(APLL1_CON0));
    print("mt_pll_post_init: APLL1_CON1         = 0x%x, GS = 0xB7945EA6\n", DRV_Reg32(APLL1_CON1));//Jade PLL frequency mapping
    print("mt_pll_post_init: APLL1_PWR_CON0     = 0x%x, GS = 0x00000001\n", DRV_Reg32(APLL1_PWR_CON0));
    print("mt_pll_post_init: APLL2_CON0         = 0x%x, GS = 0x00000131\n", DRV_Reg32(APLL2_CON0));
    print("mt_pll_post_init: APLL2_CON1         = 0x%x, GS = 0xBC7EA932\n", DRV_Reg32(APLL2_CON1));
    print("mt_pll_post_init: APLL2_PWR_CON0     = 0x%x, GS = 0x00000001\n", DRV_Reg32(APLL2_PWR_CON0));
    
    print("mt_pll_post_init:  SPM_PWR_STATUS    = 0x%x, \n", DRV_Reg32(PWR_STATUS));
    print("mt_pll_post_init:  DISP_CG_CON0    = 0x%x, \n", DRV_Reg32(DISP_CG_CON0));
    print("mt_pll_post_init:  DISP_CG_CON1    = 0x%x, \n", DRV_Reg32(DISP_CG_CON1));
#endif
    /*print("Pll post init Done!\n");*/
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
void mt_set_topck_default(void)
{
	DRV_WriteReg32(CLK_CFG_0, 0x00002000);
	DRV_WriteReg32(CLK_CFG_1, 0x00000000);
	DRV_WriteReg32(CLK_CFG_2, 0x00000000);
	DRV_WriteReg32(CLK_CFG_3, 0x00000000);
	DRV_WriteReg32(CLK_CFG_4, 0x00000000);
	DRV_WriteReg32(CLK_CFG_5, 0x00000000);
	DRV_WriteReg32(CLK_CFG_6, 0x00000000);
	DRV_WriteReg32(CLK_CFG_7, 0x00000000);
	DRV_WriteReg32(CLK_CFG_8, 0x00000000);
	DRV_WriteReg32(CLK_CFG_UPDATE, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_UPDATE1, 0x00000007);
	print("[PLL] mt_set_topck_default done\n");
}

void mt_pll_init(void)
{
    int ret = 0;
    unsigned int temp;
    /*print("Pll init start...\n");*/
    DRV_WriteReg32(ACLKEN_DIV, 0x12); // MCU Bus DIV2

    //step 1
    DRV_WriteReg32(CLKSQ_STB_CON0, 0x05010501); // reduce CLKSQ disable time
    
    //step 2
    DRV_WriteReg32(PLL_ISO_CON0, 0x00080008); // extend PWR/ISO control timing to 1us
    
    //step 3
    DRV_WriteReg32(AP_PLL_CON6, 0x00000000); //

    /*************
    * xPLL PWR ON 
    **************/
    //step 4
    temp = DRV_Reg32(ARMCA15PLL_PWR_CON0);
    DRV_WriteReg32(ARMCA15PLL_PWR_CON0, temp | 0x1);

    temp = DRV_Reg32(ARMCA7PLL_PWR_CON0);
    DRV_WriteReg32(ARMCA7PLL_PWR_CON0, temp | 0x1);

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

    //step 11 No MPLL in Jade
    //temp = DRV_Reg32(MPLL_PWR_CON0);
    //DRV_WriteReg32(MPLL_PWR_CON0, temp | 0x1);
    
    //step 12
    temp = DRV_Reg32(VCODECPLL_PWR_CON0);
    DRV_WriteReg32(VCODECPLL_PWR_CON0, temp | 0x1);

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
    temp = DRV_Reg32(ARMCA15PLL_PWR_CON0);
    DRV_WriteReg32(ARMCA15PLL_PWR_CON0, temp & 0xFFFFFFFD);
    
    temp = DRV_Reg32(ARMCA7PLL_PWR_CON0);
    DRV_WriteReg32(ARMCA7PLL_PWR_CON0, temp & 0xFFFFFFFD);
    
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
    
    //step 22 No MPLL in jade
    //temp = DRV_Reg32(MPLL_PWR_CON0);
    //DRV_WriteReg32(MPLL_PWR_CON0, temp & 0xFFFFFFFD);
    
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
    //step 26: CPU SPEED*****
    //DRV_WriteReg32(ARMCA15PLL_CON1, 0x810B0000); // 572MHz------------CA15 Freq
    //DRV_WriteReg32(ARMCA15PLL_CON1, 0x8009A000); // 1001MHz------------CA15 Freq
    //DRV_WriteReg32(ARMCA15PLL_CON1, 0x810F8000); // 806MHz------------CA15 Freq
    DRV_WriteReg32(ARMCA15PLL_CON1, 0x8110C000); // 871MHz------------CA15 Freq
    //DRV_WriteReg32(ARMCA7PLL_CON1, 0x800B0EC4); // 1150MHz
    DRV_WriteReg32(ARMCA7PLL_CON1, 0x810D4000); // 689MHz
    
    //step 27
    DRV_WriteReg32(MAINPLL_CON1, 0x800A8000); //1092MHz
    
    //step 28
    DRV_WriteReg32(MMPLL_CON1, 0x82140000); //520MHz
    
    //step 29
	DRV_WriteReg32(MSDCPLL_CON1, 0x800EC000); /*767MHz*/
    
    //step 30
    //FIXME, change to 410MHz
    //DRV_WriteReg32(VENCPLL_CON1, 0x800F6276); //800MHz
    //`define DDR1333
    DRV_WriteReg32(VENCPLL_CON1, 0x800B0000); //286MHz
    DRV_WriteReg32(VENCPLL_CON0, 0x00000120); //POSDIV = 4
    //`define DDR1866
    //DRV_WriteReg32(VENCPLL_CON1, 0x800E0000); //364MHz
    //DRV_WriteReg32(VENCPLL_CON0, 0x00000120); //POSDIV = 4
    
    //step 31
    //FIXME, change to 594MHz
    //DRV_WriteReg32(TVDPLL_CON1, 0x80112276); // 445.5MHz
    DRV_WriteReg32(TVDPLL_CON1, 0x8016D89E); // 594MHz

    //step 32 //No MPLL in jade
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
    /***********************
    * xPLL Frequency Enable
    ************************/
    //step 34
    temp = DRV_Reg32(ARMCA15PLL_CON0);
    DRV_WriteReg32(ARMCA15PLL_CON0, temp | 0x1);
    
    temp = DRV_Reg32(ARMCA7PLL_CON0);
    DRV_WriteReg32(ARMCA7PLL_CON0, temp | 0x1);
    
    //step 35
    //temp = DRV_Reg32(MAINPLL_CON0) & (~ 0x70);
/* CC: modify for testing */
#if 1
    /* CCI400 @ 500MHz */
    /* not divide by 2 */
#else
    /* CCI400 @ 250MHz */
    /* divide by 2 */
    //temp |= 0x10;
#endif
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

    //step 41 //No MPLL in Jade
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
    temp = DRV_Reg32(ARMCA7PLL_CON0);
    DRV_WriteReg32(ARMCA7PLL_CON0, temp | 0x01000000);
    
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
    //temp = DRV_Reg32(AP_PLL_CON3);
    //DRV_WriteReg32(AP_PLL_CON3, temp & 0xFFF4CCC0); // UNIVPLL SW Control

    //step 49
    //temp = DRV_Reg32(AP_PLL_CON4);
    //DRV_WriteReg32(AP_PLL_CON4, temp & 0xFFFFFFFC); // UNIVPLL,  SW Control
#endif
    /*************
    * MEMPLL Init
    **************/

//    mt_mempll_pre();

    /**************
    * INFRA CLKMUX
    ***************/
 
    temp = DRV_Reg32(TOP_DCMCTL);
    DRV_WriteReg32(TOP_DCMCTL, temp | 0x1); // enable infrasys DCM
   
    temp = DRV_Reg32(TOP_CKDIV1_BIG) ;
    //DRV_WriteReg32(TOP_CKDIV1_BIG, (temp & 0xFFFFFFE0) | 0xA); // BIG CPU clock divide by 2
    DRV_WriteReg32(TOP_CKDIV1_BIG, temp & 0xFFFFFFE0); // BIG CPU clock divide by 1

    temp = DRV_Reg32(TOP_CKDIV1_SML);
    DRV_WriteReg32(TOP_CKDIV1_SML, temp & 0xFFFFFFE0); // SML CPU clock divide by 1

    temp = DRV_Reg32(TOP_CKDIV1_BUS);
    //DRV_WriteReg32(TOP_CKDIV1_BUS, temp & 0xFFFFFFE0); // BUS CPU clock divide by 1
    DRV_WriteReg32(TOP_CKDIV1_BUS, temp & (temp & 0xFFFFFFE0) | 0xA ); // BUS CPU clock divide by 1, CCI = ARMSPLL, div = 2

    temp = DRV_Reg32(TOP_CKMUXSEL);
    DRV_WriteReg32(TOP_CKMUXSEL, temp | 0x2115); // switch CA7&CA15 CLKSQ to ARMPLL, BIG CPU clock to 1700M ; SML CPU to 1000M ; BUS CPU to 1700M (or 1000M to write temp | 0x2110)



    /************
    * TOP CLKMUX
    *************/
		DRV_WriteReg32(INFRA_BUS_DCM_CTRL, DRV_Reg32(INFRA_BUS_DCM_CTRL) | (1 << 21));
    //DRV_WriteReg32(CLK_CFG_0, 0x01000101); //ddrphycfg_ck = 26MHz
    //DRV_WriteReg32(CLK_CFG_0, 0x02010202);//mm_ck=vencpll_ck, ddrphycfg_ck=syspll1_d8, mem_ck=ddr_x1_ck_buf, axi=syspll2_d2
	DRV_WriteReg32(CLK_CFG_0, 0x02002201);
    DRV_WriteReg32(CLK_CFG_1, 0x01000300);//mfg_ck=mmpll_ck, Null, vdec_ck=univpll_d5, pwm_ck=26Mhz 
    DRV_WriteReg32(CLK_CFG_2, 0x00010001);//Null, spi_ck=syspll3_d2, uart=26M, camtg=univpll_d26
    
    //DRV_WriteReg32(CLK_CFG_3, 0x01010100);//msdc30_1=univpll2_d2, msdc50_0=msdcpll_ck, msdc50_0_hclk=syspll1_d2, Null
    DRV_WriteReg32(CLK_CFG_3, 0x07010100);//msdc30_1=msdcpll_d2, msdc50_0=msdcpll_ck, msdc50_0_hclk=syspll1_d2, Null----------------MSDC MUX
    //DRV_WriteReg32(CLK_CFG_4, 0x01010701);//aud_intbus=syspll1_d4, aud=syspll3_d4, msdc30_3=syspll3_d4, msdc30_2=univpll2_d2
	DRV_WriteReg32(CLK_CFG_4, 0x01000107);
    
    DRV_WriteReg32(CLK_CFG_5, 0x01020000);//mjc_ck=syspll_d5, atb_ck=SYSPLL_D5, Null, pmicspi=26MHz
    DRV_WriteReg32(CLK_CFG_6, 0x01010102);//aud2_ck=apll2 , aud1_ck=apll1, scam_ck=syspll3_d2, dpi0_ck=tvdpll_d4
    DRV_WriteReg32(CLK_CFG_7, 0x01000101);//usb_top= univpll3_d4, Null, ssusb_top_sys= univpll3_d2, disp_pwm=univpll2_d4
    //DRV_WriteReg32(CLK_CFG_8, 0x01020201);//dvfsp= syspll1_d8, i2c_ck =univpll3_d4 ,bsi_spi=syspll1_d4, hg_fspm=syspll1_d8
    DRV_WriteReg32(CLK_CFG_8, 0x01010201);//dvfsp= syspll1_d8, i2c_ck =syspll1_d8 ,bsi_spi=syspll1_d4, hg_fspm=syspll1_d: change i2c clkmux

    DRV_WriteReg32(CLK_CFG_UPDATE, 0xffffffff);
    DRV_WriteReg32(CLK_CFG_UPDATE1, 0x00000007);
    //DRV_WriteReg32(CLK_SCP_CFG_0, 0x3FF); // enable scpsys clock off control
    DRV_WriteReg32(CLK_SCP_CFG_0, 0x703FF); // enable scpsys clock off control, -----[2:0][6:5][18:16]SPM sideband signal initial setting check
    DRV_WriteReg32(CLK_SCP_CFG_1, 0x7); // enable scpsys clock off control


    /*for MTCMOS*/
    spm_write(POWERON_CONFIG_EN, (SPM_PROJECT_CODE << 16) | (1U << 0));
    spm_mtcmos_ctrl_disp(STA_POWER_ON);
#if 0
    spm_mtcmos_ctrl_vdec(STA_POWER_ON);
    spm_mtcmos_ctrl_venc(STA_POWER_ON);
    spm_mtcmos_ctrl_isp(STA_POWER_ON);
    spm_mtcmos_ctrl_aud(STA_POWER_ON);
#endif
    /*for CG*/
    DRV_WriteReg32(INFRA_PDN_CLR0, 0xFFFFFFFF);
    DRV_WriteReg32(INFRA_PDN_CLR1, 0xFFFFFFFF);
    DRV_WriteReg32(INFRA_PDN_CLR2, 0xFFFFFFFF);
    /*DISP CG*/
    DRV_WriteReg32(DISP_CG_CLR0, 0xFFFFFFFF);
    DRV_WriteReg32(DISP_CG_CLR1, 0x3F);
#if 0
    //AUDIO
    DRV_WriteReg32(AUDIO_TOP_CON0, 0);
    //MFG
    DRV_WriteReg32(MFG_CG_CLR, 0x00000001);
    //ISP
    DRV_WriteReg32(IMG_CG_CLR, 0x00000FE1);
    //VDE
    DRV_WriteReg32(VDEC_CKEN_SET, 0x00000001);
    DRV_WriteReg32(LARB_CKEN_SET, 0x00000001);
    //VENC
    DRV_WriteReg32(VENC_CG_SET, 0x00001111);
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

#if 0
int spm_mtcmos_ctrl_mdsys1(int state)
{
    int err = 0;
    volatile unsigned int val;
    unsigned long flags;
    int count = 0;

    if (state == STA_POWER_DOWN) {
        spm_write(TOPAXI_PROT_EN, spm_read(TOPAXI_PROT_EN) | MD1_PROT_MASK);
        while ((spm_read(TOPAXI_PROT_STA1) & MD1_PROT_MASK) != MD1_PROT_MASK) {
            count++;
            if(count>1000)
                break;
        }

        spm_write(SPM_MD_PWR_CON, spm_read(SPM_MD_PWR_CON) | MD_SRAM_PDN);

        spm_write(SPM_MD_PWR_CON, spm_read(SPM_MD_PWR_CON) | PWR_ISO);

        val = spm_read(SPM_MD_PWR_CON);
        val = (val & ~PWR_RST_B) | PWR_CLK_DIS;
        spm_write(SPM_MD_PWR_CON, val);

        spm_write(SPM_MD_PWR_CON, spm_read(SPM_MD_PWR_CON) & ~(PWR_ON | PWR_ON_S));

        while ((spm_read(SPM_PWR_STATUS) & MD1_PWR_STA_MASK)
                || (spm_read(SPM_PWR_STATUS_2ND) & MD1_PWR_STA_MASK)) {
        }

        spm_write(AP_PLL_CON7, (spm_read(AP_PLL_CON7) | 0xF)); //force off LTE
        spm_write(0x10007054, 0x80000000);

    } else {    /* STA_POWER_ON */
        
        spm_write(AP_PLL_CON7, (spm_read(AP_PLL_CON7) & (~0x4))); //turn on LTE, clk
        spm_write(AP_PLL_CON7, (spm_read(AP_PLL_CON7) & (~0x1))); //turn on LTE, mtcmos
        spm_write(AP_PLL_CON7, (spm_read(AP_PLL_CON7) & (~0x8))); //turn on LTE, mtcmos + iso
        spm_write(AP_PLL_CON7, (spm_read(AP_PLL_CON7) & (~0x2))); //turn on LTE, memory
    
        spm_write(SPM_MD_PWR_CON, spm_read(SPM_MD_PWR_CON) | PWR_ON);
        spm_write(SPM_MD_PWR_CON, spm_read(SPM_MD_PWR_CON) | PWR_ON_S);

        while (!(spm_read(SPM_PWR_STATUS) & MD1_PWR_STA_MASK) 
                || !(spm_read(SPM_PWR_STATUS_2ND) & MD1_PWR_STA_MASK)) {
        }

        spm_write(SPM_MD_PWR_CON, spm_read(SPM_MD_PWR_CON) & ~PWR_CLK_DIS);
        spm_write(SPM_MD_PWR_CON, spm_read(SPM_MD_PWR_CON) & ~PWR_ISO);
        spm_write(SPM_MD_PWR_CON, spm_read(SPM_MD_PWR_CON) | PWR_RST_B);

        spm_write(SPM_MD_PWR_CON, spm_read(SPM_MD_PWR_CON) & ~MD_SRAM_PDN);

        spm_write(TOPAXI_PROT_EN, spm_read(TOPAXI_PROT_EN) & ~MD1_PROT_MASK);
        while (spm_read(TOPAXI_PROT_STA1) & MD1_PROT_MASK) {
        }
    }

    return err;
}
#endif
