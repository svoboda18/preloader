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
#include "spm_mtcmos.h"

unsigned int mt_get_abist_freq(unsigned int ID)
{
	int output = 0, i = 0;
    unsigned int temp, clk26cali_0, clk_dbg_cfg, clk_misc_cfg_0, clk26cali_1;

    clk_dbg_cfg = DRV_Reg32(CLK_DBG_CFG);
    DRV_WriteReg32(CLK_DBG_CFG, (clk_dbg_cfg & 0xFFC0FFFC)|(ID << 16)); //sel abist_cksw and enable freq meter sel abist
    clk_misc_cfg_0 = DRV_Reg32(CLK_MISC_CFG_0);
    DRV_WriteReg32(CLK_MISC_CFG_0, (clk_misc_cfg_0 & 0x00FFFFFF)); // select divider, WAIT CONFIRM
    clk26cali_0 = DRV_Reg32(CLK26CALI_0);
    clk26cali_1 = DRV_Reg32(CLK26CALI_1);
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

    //print("abist meter[%d] = %d Khz\n", ID, output);
    return output;
}

static unsigned int mt_get_ckgen_freq(unsigned int ID)
{
    int output = 0, i = 0;
    unsigned int temp, clk26cali_0, clk_dbg_cfg, clk_misc_cfg_0, clk26cali_1;

    clk_dbg_cfg = DRV_Reg32(CLK_DBG_CFG);
    DRV_WriteReg32(CLK_DBG_CFG, (clk_dbg_cfg & 0xFFFFC0FC)|(ID << 8)|(0x1)); //sel ckgen_cksw[22] and enable freq meter sel ckgen[21:16], 01:hd_faxi_ck

    clk_misc_cfg_0 = DRV_Reg32(CLK_MISC_CFG_0);
    DRV_WriteReg32(CLK_MISC_CFG_0, (clk_misc_cfg_0 & 0x00FFFFFF)); // select divider?dvt set zero
    clk26cali_0 = DRV_Reg32(CLK26CALI_0);
    clk26cali_1 = DRV_Reg32(CLK26CALI_1);
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

    //print("ckgen meter[%d] = %d Khz\n", ID, output);
    return output;

}




void mt_pll_post_init(void)
{
}

void mt_set_topck_default(void)
{
	DRV_WriteReg32(CLK_CFG_0, 0x00000000);
	DRV_WriteReg32(CLK_CFG_1, 0x00000000);
	DRV_WriteReg32(CLK_CFG_2, 0x00000000);
	DRV_WriteReg32(CLK_CFG_3, 0x00000000);
	DRV_WriteReg32(CLK_CFG_4, 0x00000000);
	DRV_WriteReg32(CLK_CFG_5, 0x00000000);
	DRV_WriteReg32(CLK_CFG_6, 0x00000000);
	DRV_WriteReg32(CLK_CFG_7, 0x00000000);
	DRV_WriteReg32(CLK_CFG_8, 0x00000000);
	DRV_WriteReg32(CLK_CFG_9, 0x00000000);
	DRV_WriteReg32(CLK_CFG_10, 0x00000000);
	DRV_WriteReg32(CLK_CFG_UPDATE, 0x7FFFFFFE);
        DRV_WriteReg32(CLK_CFG_UPDATE1, 0x000007FF);
}

void mt_pll_init(void)
{
    int ret = 0;
    unsigned int temp;
    unsigned int temp1; // FIX BY HK
    unsigned int rdata = 0;
    unsigned int wdata = 0;
    int i;

    print("Pll init start...\n");
#if 1
    temp = DRV_Reg32(AP_PLL_CON2);
    DRV_WriteReg32(AP_PLL_CON2, temp | 0x1FFE0000);//need confirm
#endif
    //step 1
    //DRV_WriteReg32(CLKSQ_STB_CON0, 0x05010501); // reduce CLKSQ disable time
    temp = DRV_Reg32(AP_PLL_CON0);
    DRV_WriteReg32(AP_PLL_CON0, temp | 0x01);// [0]
    
    //step 2
    //DRV_WriteReg32(PLL_ISO_CON0, 0x00080008); // extend PWR/ISO control timing to 1us
    gpt_busy_wait_us(200);
	
    //step 3
    //DRV_WriteReg32(AP_PLL_CON6, 0x00000000); //
    temp = DRV_Reg32(AP_PLL_CON0);
    DRV_WriteReg32(AP_PLL_CON0, temp | 0x2); //[1]?
    /*************
 	   * xPLL PWR ON
 	   **************/
    //step 4
    temp = DRV_Reg32(MFGPLL_PWR_CON0);
    DRV_WriteReg32(MFGPLL_PWR_CON0, temp | 0x1);

    //step 5
    //temp = DRV_Reg32(MPLL_PWR_CON0);
    //DRV_WriteReg32(MPLL_PWR_CON0, temp | 0x1);

    //step 6
    temp = DRV_Reg32(MAINPLL_PWR_CON0);
    DRV_WriteReg32(MAINPLL_PWR_CON0, temp | 0x1);

    //step 7
    temp = DRV_Reg32(UNIVPLL_PWR_CON0);
    DRV_WriteReg32(UNIVPLL_PWR_CON0, temp | 0x1);

    //step 8
    temp = DRV_Reg32(MSDCPLL_PWR_CON0);
    DRV_WriteReg32(MSDCPLL_PWR_CON0, temp | 0x1);

    //step 9
    temp = DRV_Reg32(MMPLL_PWR_CON0);
    DRV_WriteReg32(MMPLL_PWR_CON0, temp | 0x1);

    //step 10
    temp = DRV_Reg32(TVDPLL_PWR_CON0);
    DRV_WriteReg32(TVDPLL_PWR_CON0, temp | 0x1);

    //step 11
    temp = DRV_Reg32(APLL1_PWR_CON0);
    DRV_WriteReg32(APLL1_PWR_CON0, temp | 0x1);
    
    //step 12
    temp = DRV_Reg32(APLL2_PWR_CON0);
    DRV_WriteReg32(APLL2_PWR_CON0, temp | 0x1);
    
    //step 13
    temp = DRV_Reg32(ARMPLL_LL_PWR_CON0);
    DRV_WriteReg32(ARMPLL_LL_PWR_CON0, temp | 0x1);    
    
    //step 14
    temp = DRV_Reg32(ARMPLL_L_PWR_CON0);
    DRV_WriteReg32(ARMPLL_L_PWR_CON0, temp | 0x1);

    //step 15
    temp = DRV_Reg32(CCIPLL_PWR_CON0);
    DRV_WriteReg32(CCIPLL_PWR_CON0, temp | 0x1);

    //step 16
    gpt_busy_wait_us(100);	
    /******************
    	* xPLL ISO Disable
    	*******************/
    //step 17
     temp = DRV_Reg32(MFGPLL_PWR_CON0);
    DRV_WriteReg32(MFGPLL_PWR_CON0, temp & 0xFFFFFFFD);

    //step 18
    //temp = DRV_Reg32(MMPLL_CON0);
    //DRV_WriteReg32(MMPLL_CON0, temp & 0xFFFFFFFD);

    //step 19
    temp = DRV_Reg32(MAINPLL_PWR_CON0);
    DRV_WriteReg32(MAINPLL_PWR_CON0, temp & 0xFFFFFFFD);

    //step 20
    temp = DRV_Reg32(UNIVPLL_PWR_CON0);
    DRV_WriteReg32(UNIVPLL_PWR_CON0, temp & 0xFFFFFFFD);

    //step 21
    temp = DRV_Reg32(MSDCPLL_PWR_CON0);
    DRV_WriteReg32(MSDCPLL_PWR_CON0, temp & 0xFFFFFFFD);

    //step 22
    temp = DRV_Reg32(MMPLL_PWR_CON0);
    DRV_WriteReg32(MMPLL_PWR_CON0, temp & 0xFFFFFFFD);

    //step 23
    temp = DRV_Reg32(TVDPLL_PWR_CON0);
    DRV_WriteReg32(TVDPLL_PWR_CON0, temp & 0xFFFFFFFD);

    //step 24
    temp = DRV_Reg32(APLL1_PWR_CON0);
    DRV_WriteReg32(APLL1_PWR_CON0, temp & 0xFFFFFFFD);

    //step 25
    temp = DRV_Reg32(APLL2_PWR_CON0);
    DRV_WriteReg32(APLL2_PWR_CON0, temp & 0xFFFFFFFD);

    //step 26
    temp = DRV_Reg32(ARMPLL_LL_PWR_CON0);
    DRV_WriteReg32(ARMPLL_LL_PWR_CON0, temp & 0xFFFFFFFD);
    
    //step 27
    temp = DRV_Reg32(ARMPLL_L_PWR_CON0);
    DRV_WriteReg32(ARMPLL_L_PWR_CON0, temp & 0xFFFFFFFD);

    //step 28
    temp = DRV_Reg32(CCIPLL_PWR_CON0);
    DRV_WriteReg32(CCIPLL_PWR_CON0, temp & 0xFFFFFFFD);

    //step 29
    gpt_busy_wait_us(50);
    
    //step 30
    temp = DRV_Reg32(APLL1_CON0);
    DRV_WriteReg32(APLL1_CON0, temp | 0x00000000);

    //step 31
    temp = DRV_Reg32(APLL2_CON0);
    DRV_WriteReg32(APLL2_CON0, temp | 0x00000000);
    /********************
       * xPLL Frequency Set
       *********************/
    //step 32
    //DRV_WriteReg32(MFGPLL_CON1, 0x830f6276); // 200MHz
    DRV_WriteReg32(MFGPLL_CON1, 0x8213B13B); //512MHz
    //DRV_WriteReg32(MFGPLL_CON1, 0x821EC4EC); //800MHz

    //step 34
    DRV_WriteReg32(MAINPLL_CON1, 0x81150000); //1092MHz

    //step 35
    DRV_WriteReg32(UNIVPLL_CON1, 0x80180000); //1248MHz
    
    //step 36
    //DRV_WriteReg32(MSDCPLL_CON1, 0x831EC4EC); //400MHz  FIX BY HK
    //DRV_WriteReg32(MSDCPLL_CON1, 0x831C9D89); //372MHz  FIX BY HK
    //DRV_WriteReg32(MSDCPLL_CON1, 0x83200000); //416MHz
    DRV_WriteReg32(MSDCPLL_CON1, 0x831D89D8); //384MHz

    //step 37
    //DRV_WriteReg32(MMPLL_CON1, 0x831713B1); // 300MHz
    //DRV_WriteReg32(MMPLL_CON1, 0x83204EC4); // 420MHz
    DRV_WriteReg32(MMPLL_CON1, 0x801E49D9); // 3150MHz
    //step 38
    DRV_WriteReg32(TVDPLL_CON1, 0x8216D89E); //594MHz
    //DRV_WriteReg32(TVDPLL_CON1, 0x82114EC4); //450MHz
    
    //step 39
    DRV_WriteReg32(APLL1_CON1, 0x6F28BD4C); //180.6336MHz
    DRV_WriteReg32(APLL1_CON2, 0x6F28BD4D); //180.6336MHz
    //step 40
    DRV_WriteReg32(APLL2_CON1, 0x78FD5264); //196.608MHz
    DRV_WriteReg32(APLL2_CON2, 0x78FD5265); //196.608MHz
    //step 41
    DRV_WriteReg32(APLL1_CON0, 0x80000188);
    
    //step 42
    DRV_WriteReg32(APLL2_CON0, 0x80000188);
     
    //step 43: CPU SPEED*****
    //DRV_WriteReg32(ARMPLL_LL_CON1, 0x81110000); // 884MHz, arm div = 1
    DRV_WriteReg32(ARMPLL_LL_CON1, 0x81152762); // 1100MHz, pos div = 2, arm div = 1
    //step 44
    //DRV_WriteReg32(ARMPLL_L_CON1, 0x8118C4EC); // 1288MHz, arm div = 1
    DRV_WriteReg32(ARMPLL_L_CON1, 0x811713B1); // 1200MHz, pss div = 2, arm div = 1
    
    //step 45
    //DRV_WriteReg32(CCIPLL_CON1, 0x821713B1); // 600MHz, arm div = 2
    //DRV_WriteReg32(CCIPLL_CON1, 0x81152762); // 1100MHz/2, pos div = 2, arm div = 2
    DRV_WriteReg32(CCIPLL_CON1, 0x81170000); // 1196/2MHz, pos div = 2, arm div = 2

    /***********************
      * xPLL Frequency Enable
      ************************/
     //step 46
     temp = DRV_Reg32(MFGPLL_CON0);
     DRV_WriteReg32(MFGPLL_CON0, temp | 0x1);
 
    //step 48
    temp = DRV_Reg32(MAINPLL_CON0);
    DRV_WriteReg32(MAINPLL_CON0, temp | 0x1);

    //step 49
    temp = DRV_Reg32(UNIVPLL_CON0);
    DRV_WriteReg32(UNIVPLL_CON0, temp | 0x1);

    //step 50
    temp = DRV_Reg32(MSDCPLL_CON0);
    DRV_WriteReg32(MSDCPLL_CON0, temp | 0x1);

    //step 51
    temp = DRV_Reg32(MMPLL_CON0);
    DRV_WriteReg32(MMPLL_CON0, temp | 0x1);

    //step 52
    temp = DRV_Reg32(TVDPLL_CON0);
    DRV_WriteReg32(TVDPLL_CON0, temp | 0x1);

    //step 53
    temp = DRV_Reg32(APLL1_CON0);
    DRV_WriteReg32(APLL1_CON0, temp | 0x1);

    //step 54
    temp = DRV_Reg32(APLL2_CON0);
    DRV_WriteReg32(APLL2_CON0, temp | 0x1);

    //step 55
    temp = DRV_Reg32(ARMPLL_LL_CON0);
    DRV_WriteReg32(ARMPLL_LL_CON0, temp | 0x1);

    //step 56
    temp = DRV_Reg32(ARMPLL_L_CON0);
    DRV_WriteReg32(ARMPLL_L_CON0, temp | 0x1);

    //step 57
    temp = DRV_Reg32(CCIPLL_CON0);
    DRV_WriteReg32(CCIPLL_CON0, temp | 0x1);

    // step 58
    gpt_busy_wait_us(50); // wait for PLL stable (min delay is 20us)

    /***************
      * xPLL DIV RSTB
      ****************/
    //step 59
    temp = DRV_Reg32(MAINPLL_CON0);
    DRV_WriteReg32(MAINPLL_CON0, temp | 0x01000000);
	
    //step 60
    temp = DRV_Reg32(UNIVPLL_CON0);
    DRV_WriteReg32(UNIVPLL_CON0, temp | 0x01000000);

    //step 61
    temp = DRV_Reg32(MMPLL_CON0);
    DRV_WriteReg32(MMPLL_CON0, temp | 0x00800000);
    /*****************
	* xPLL HW Control
    	******************/

    /**************
    	* INFRA CPU CLKMUX, CLK Div
    	***************/

//8: div1, A: div2, B: div4, 1D: div6
    temp = DRV_Reg32(MP0_PLL_DIV_CFG) ;
    DRV_WriteReg32(MP0_PLL_DIV_CFG, (temp & 0xFFE1FFFF) | (0x08 << 17)); // [21:17] divsel: CPU clock divide by 1

    //temp = DRV_Reg32(MP1_PLL_DIV_CFG) ;
    //DRV_WriteReg32(MP1_PLL_DIV_CFG, (temp & 0xFFE1FFFF) | (0x08 << 17)); // [21:17] divsel: CPU clock divide by 1

    temp = DRV_Reg32(MP2_PLL_DIV_CFG) ;
    DRV_WriteReg32(MP2_PLL_DIV_CFG, (temp & 0xFFE1FFFF) | (0x08 << 17)); // [21:17] divsel: CPU clock divide by 1

    temp = DRV_Reg32(BUS_PLL_DIV_CFG) ;
    DRV_WriteReg32(BUS_PLL_DIV_CFG, (temp & 0xFFE1FFFF) | (0x0A << 17)); // [21:17] divsel: CPU clock divide by 1

    //0: 26M,  1: armpll, 2:  Mainpll, 3:  Unipll
    temp = DRV_Reg32(MP0_PLL_DIV_CFG);
    DRV_WriteReg32(MP0_PLL_DIV_CFG, (temp & 0xFFFFF9FF) | (0x01<<9)); // [10:9] muxsel: switch to PLL speed

    //temp = DRV_Reg32(MP1_PLL_DIV_CFG);
    //DRV_WriteReg32(MP1_PLL_DIV_CFG, (temp & 0xFFFFF9FF) | (0x01<<9)); // [10:9] muxsel: switch to PLL speed

    temp = DRV_Reg32(MP2_PLL_DIV_CFG);
    DRV_WriteReg32(MP2_PLL_DIV_CFG, (temp & 0xFFFFF9FF) | (0x01<<9)); // [10:9] muxsel: switch to PLL speed

    temp = DRV_Reg32(BUS_PLL_DIV_CFG);
    DRV_WriteReg32(BUS_PLL_DIV_CFG, (temp & 0xFFFFF9FF) | (0x01<<9)); // [10:9] muxsel: switch to PLL speed

    /************
  	  * TOP CLKMUX
  	  *************/
	DRV_WriteReg32(INFRA_BUS_DCM_CTRL, DRV_Reg32(INFRA_BUS_DCM_CTRL) | (1 << 21) | (1 << 22));
	
    //need to confirm with roy
    // [22] set this register before AXI clock switch to fast clock (26MHz => 156MHz) 
    //DRV_WriteReg32(INFRA_BUS_DCM_CTRL, DRV_Reg32(INFRA_BUS_DCM_CTRL) | (1 << 21) | (1 << 22) | (1 << 23));//spm axi dcm enable
    //DRV_WriteReg32(PERI_BUS_DCM_CTRL_0, DRV_Reg32(PERI_BUS_DCM_CTRL_0) | (1 << 21) | (1 << 22) | (1 << 23));
    //DRV_WriteReg32(PWR_RST_CTL, 0x00000001);//disable big cpu protection


    // config AXI clock first
    DRV_WriteReg32(CLK_CFG_0, 0x00000002);      // axi=syspll2_d2
    DRV_WriteReg32(CLK_CFG_UPDATE, 0x00000001); //update AXI clock

    DRV_WriteReg32(CLK_CFG_20_SET, 0x00030000); //set CAM[16] & MM[17] fast clock transition
    // config other clocks
    #if 1 // 0.8v setting
    DRV_WriteReg32(CLK_CFG_0, 0x01010102);
    DRV_WriteReg32(CLK_CFG_1, 0x01010101);
    DRV_WriteReg32(CLK_CFG_2, 0x01010301); // mfgpll_ck
    DRV_WriteReg32(CLK_CFG_3, 0x01000101);
    DRV_WriteReg32(CLK_CFG_4, 0x01040101);
    DRV_WriteReg32(CLK_CFG_5, 0x00000100);
    DRV_WriteReg32(CLK_CFG_6, 0x01010201);
    DRV_WriteReg32(CLK_CFG_7, 0x01030300);
    DRV_WriteReg32(CLK_CFG_8, 0x01020102);
    DRV_WriteReg32(CLK_CFG_9, 0x01030303);
    DRV_WriteReg32(CLK_CFG_10, 0x00000101);
    #else //bianco backup
    DRV_WriteReg32(CLK_CFG_0, 0x01000002);
    DRV_WriteReg32(CLK_CFG_1, 0x01000600);//cam tvdpll
    DRV_WriteReg32(CLK_CFG_2, 0x00010001);//camtg 26Mx, uart 26M
    DRV_WriteReg32(CLK_CFG_3, 0x04010100);//msdc30_1 msdcpll_d2
    DRV_WriteReg32(CLK_CFG_4, 0x01010004);//msdc30_2 msdcpll_d2
    DRV_WriteReg32(CLK_CFG_5, 0x01000000);
    DRV_WriteReg32(CLK_CFG_6, 0x01010001);
    DRV_WriteReg32(CLK_CFG_7, 0x01000000); //disp pwm: 26M
    DRV_WriteReg32(CLK_CFG_8, 0x00010001);//i2c syspll1 d8*
    DRV_WriteReg32(CLK_CFG_9, 0x01020203);
    DRV_WriteReg32(CLK_CFG_10, 0x01030303);//aud eng1/2: apll div 8

    #if 0
    #if (CFG_BOOT_DEV == BOOTDEV_UFS)
    DRV_WriteReg32(CLK_CFG_10, 0x01030303);//aud eng1/2: apll div 8 , hf_faes_ufsfde_ck SYSPLL_D3 364MHz
    #elif (CFG_BOOT_DEV == BOOTDEV_SDMMC)
    DRV_WriteReg32(CLK_CFG_10, 0x01050303);//aud eng1/2: apll div 8 , hf_faes_ufsfde_ck UNIVPLL_D3 416MHz
    #endif
    #endif

    #endif

    DRV_WriteReg32(CLK_CFG_UPDATE, 0x7FFFFFFE);    //update all clocks except "axi"
    DRV_WriteReg32(CLK_CFG_UPDATE1, 0x000007FF);

#if 1
    temp = DRV_Reg32(CLK_MISC_CFG_0);   // FIX BY HK
    //DRV_WriteReg32(CLK_MISC_CFG_0, ( temp | 0x0003000)); // FIX BY HK enable ufs_mp_sap_cfg_ck, ufs_tick1us_ck
    DRV_WriteReg32(CLK_MISC_CFG_0, ( temp | 0x0004000)); // enable [14] dramc_pll104m_ck
#endif
#if 0
    for(temp = 1; temp < 60; temp++)
    {
    	if((temp==6)||               // FIX BY HK
    		 (temp==16)||
    		 (temp==20)||
    		 (temp==28)||
    		 (temp==29)||
    		 (temp==42)||
    		 (temp==43)||
    		 (temp==44)||
    		 (temp==45)||
    		 (temp==46)||
    		 (temp==47)||
    		 (temp==50)||
    		 (temp==51)||
    		 (temp==52)||
    		 (temp==53)||
    		 (temp==55)||
    		 (temp==56)||
    		 (temp==57)||
    		 (temp==58)||
    		 (temp==59)   		 
    	   ) {
        } else{
        mt_get_ckgen_freq(temp);
      }
    }
     
    
    for(temp = 16; temp < 56; temp++)
    {
    	if((temp==17)||
    		 (temp==18)||
    		 (temp==19)||
    		 (temp==21)||
    		 (temp==30)||
    		 (temp==33)||
    		 (temp==35)||
    		 (temp==36)||
    		 (temp==37)||
    		 (temp==38)||
    		 (temp==41)||
    		 (temp==42)||
    		 (temp==43)||
    		 (temp==44)||
    		 (temp==45)||
    		 (temp==46)||
    		 (temp==47)||
    		 (temp==48)||
    		 (temp==50)||
    		 (temp==51)||
    		 (temp==52)||
    		 (temp==53)  		 
    	   ) {
        } else if((temp==40)){                  // FIX BY HK
    		  temp1 = DRV_Reg32(MP0_PLL_DIV_CFG);
          DRV_WriteReg32(MP0_PLL_DIV_CFG, (temp1 & 0xFFFFFF00) | (0x01)); // [7:0] Meter input from MP0
          
          temp1 = DRV_Reg32(MP1_PLL_DIV_CFG);
          DRV_WriteReg32(MP1_PLL_DIV_CFG, (temp1 & 0xFFFFFF00) | (0xFF)); // [7:0]

          temp1 = DRV_Reg32(BUS_PLL_DIV_CFG);
          DRV_WriteReg32(BUS_PLL_DIV_CFG, (temp1 & 0xFFFFFF00) | (0xFF)); // [7:0]   		
        
          mt_get_abist_freq(temp);
          
          temp1 = DRV_Reg32(MP0_PLL_DIV_CFG);
          DRV_WriteReg32(MP0_PLL_DIV_CFG, (temp1 & 0xFFFFFF00) | (0xFF)); // [7:0] 
          
          temp1 = DRV_Reg32(MP1_PLL_DIV_CFG);
          DRV_WriteReg32(MP1_PLL_DIV_CFG, (temp1 & 0xFFFFFF00) | (0x01)); // [7:0] Meter input from MP1

          temp1 = DRV_Reg32(BUS_PLL_DIV_CFG);
          DRV_WriteReg32(BUS_PLL_DIV_CFG, (temp1 & 0xFFFFFF00) | (0xFF)); // [7:0]  		
          
          mt_get_abist_freq(temp);
          
          temp1 = DRV_Reg32(MP0_PLL_DIV_CFG);
          DRV_WriteReg32(MP0_PLL_DIV_CFG, (temp1 & 0xFFFFFF00) | (0xFF)); // [7:0]  
          
          temp1 = DRV_Reg32(MP1_PLL_DIV_CFG);
          DRV_WriteReg32(MP1_PLL_DIV_CFG, (temp1 & 0xFFFFFF00) | (0xFF)); // [7:0]  

          temp1 = DRV_Reg32(BUS_PLL_DIV_CFG);
          DRV_WriteReg32(BUS_PLL_DIV_CFG, (temp1 & 0xFFFFFF00) | (0x01)); // [7:0]  Meter input from BUS 		
          
          mt_get_abist_freq(temp);
      }
      else{
        mt_get_abist_freq(temp);
      }
    }
#endif
    /************
    	* MTCMOS 
    	*************/

    print("mtcmos Start..\n");
    spm_mtcmos_ctrl_dis(STA_POWER_ON);	
#if 0 //simulation only
	spm_mtcmos_ctrl_vde(STA_POWER_ON);
	spm_mtcmos_ctrl_isp(STA_POWER_ON);
	spm_mtcmos_ctrl_ven(STA_POWER_ON);
	spm_mtcmos_ctrl_cam(STA_POWER_ON);
	spm_mtcmos_ctrl_md1(STA_POWER_ON);
	spm_mtcmos_ctrl_c2k(STA_POWER_ON);
	spm_mtcmos_ctrl_mfg_async(STA_POWER_ON);
	spm_mtcmos_ctrl_mfg(STA_POWER_ON);
	spm_mtcmos_ctrl_mfg_core1(STA_POWER_ON);
	spm_mtcmos_ctrl_mfg_core0(STA_POWER_ON);
	spm_mtcmos_ctrl_audio(STA_POWER_ON);
	spm_mtcmos_ctrl_conn(STA_POWER_ON);
#endif

    print("mtcmos Done!\n");
#if 1
    /*for CG*/
    DRV_WriteReg32(INFRA_PDN_CLR0, 0xDBEFFF7F);//7, 20, 26, 29
    DRV_WriteReg32(INFRA_PDN_CLR1, 0xDF9F7FF7);//3, 15, 21, 22, 29
    DRV_WriteReg32(INFRA_PDN_SET1, 0x18000000);//27, 28: dxcc
    DRV_WriteReg32(INFRA_PDN_CLR2, 0x7FFC07DD);//1, 5, 11~17, 31
    DRV_WriteReg32(INFRA_PDN_CLR3, 0x01FF01FF);//9~15, 25~31
#endif
    /*DISP CG Clear and DCM disable*/
    /*DRV_WriteReg32(DISP_CG_CLR0, 0xFFFFFFFF);*/
    /*DRV_WriteReg32(DISP_CG_CLR1, 0x00003FFF);*/

    print("Pll init Done!\n");
}


