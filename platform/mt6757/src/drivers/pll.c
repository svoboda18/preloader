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


//after pmic_init
//#define _FREQ_SCAN_
//#define _RESULT_COMPARE_
#define Range 1000

unsigned int ret_feq_store[100];
unsigned int ret_feq_total=0;
unsigned int pll_chk_is_fail = 0;


unsigned int mt_get_abist_freq(unsigned int ID)
{
	int output = 0, i = 0;
    unsigned int temp, clk26cali_0, clk_dbg_cfg, clk_misc_cfg_0, clk26cali_1;

    clk_dbg_cfg = DRV_Reg32(CLK_DBG_CFG);
    DRV_WriteReg32(CLK_DBG_CFG, (clk_dbg_cfg & 0xFFC0FFFC)|(ID << 16)); //sel abist_cksw and enable freq meter sel abist

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
    /*DRV_WriteReg32(CLK26CALI_0, clk26cali_0);*/
    /*DRV_WriteReg32(CLK26CALI_1, clk26cali_1);*/

	DRV_WriteReg32(CLK26CALI_0, 0x1010);
	mdelay(10);
	DRV_WriteReg32(CLK26CALI_0, 0x1000);
	DRV_WriteReg32(CLK26CALI_0, 0x0000);

    if(i>10)
        return 0;
    else
        return output;
}

static unsigned int mt_get_pll_freq(unsigned int ID)
{
    int output = 0, i = 0;
    unsigned int temp, clk26cali_0, clk_dbg_cfg, clk_misc_cfg_0, clk26cali_1;

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
        //print("wait for frequency meter finish, CLK26CALI = 0x%x\n", DRV_Reg32(CLK26CALI_0));
        mdelay(10);
		i++;
        if(i > 10)
        	break;
    }

    temp = DRV_Reg32(CLK26CALI_1) & 0xFFFF;

    output = ((temp * 26000) ) / 1024; // Khz

    DRV_WriteReg32(CLK_DBG_CFG, clk_dbg_cfg);
    DRV_WriteReg32(CLK_MISC_CFG_0, clk_misc_cfg_0);
    /*DRV_WriteReg32(CLK26CALI_0, clk26cali_0);*/
    /*DRV_WriteReg32(CLK26CALI_1, clk26cali_1);*/

	DRV_WriteReg32(CLK26CALI_0, 0x1010);
	mdelay(10);
	DRV_WriteReg32(CLK26CALI_0, 0x1000);
	DRV_WriteReg32(CLK26CALI_0, 0x0000);

	if(i>10)
        return 0;
    else
        return output;

}


const char *ckgen_golden_array[] =
{
/* 1 - 5*/    	0, 0, 0, 0, 0,
/* 6 - 10*/		0, 0, 0, 0, 0,
/* 11 - 15*/	0, 0, 0, 0, 0,
/* 16 - 20*/	0, 0, 0, 0, 0,
/* 21 - 25*/	0, 0, 0, 0, 0,
/* 26 - 30*/	0, 0, 0, 0, 0,
/* 31 - 35*/	0, 0, 0, 0, 0,
/* 36 - 39*/	0, 0, 0,
};

const int ckgen_abist_needCheck_array[] =
{
/* 1 - 5*/		1,	1, 	1,	1,	1,
/* 6 - 10*/		1,	1,	1,	1,	1,
/* 11 - 15*/	1,  1,	1,	1,	1,
/* 16 - 20*/	1,  1,	0,	0,	1,
/* 21 - 25*/	0,	1,  1,	1,	1,
/* 26 - 30*/	1,	1, 	1,	1,	1,
/* 31 - 35*/	1,	1,	1,	1,  1,
/* 36 - 40*/	1, 	0,  1,  1,  1,
/* 41 - 45*/	1,  1,	1,	1,	1,
/* 46 - 50*/	1,	1,  1,	1,	1,
/* 51 - 55*/	1,	1,	1,	1,	1,
/* 56 - 60*/	1,	1,	0,  0,	0,
/* 61 - 63*/	0,	0,	0,
};

/* if needCheck_array[x]=1 but golden_array[x]=0: means we only want to read the value*/
const int ckgen_abist_golden_array[] =
{
/* 1 - 5*/		0,	0,	0,	0,	0,
/* 6 - 10*/		0,	0,	0,	0,	0,
/* 11 - 15*/	0,	0,	0,	0,	0,
/* 16 - 20*/	0,	0,	0,	0,	0,
/* 21 - 25*/	0,	0,	0,	0,	0,
/* 26 - 30*/	0,	0, 	0,	0,	0,
/* 31 - 35*/	0,	0,	0,	0,	0,
/* 36 - 40*/	0,	0,	0,	0,	0,
/* 41 - 45*/	0,	0,	0,	0,	0,
/* 46 - 50*/	0,	0,	0,	0,	0,
/* 51 - 55*/	0,	0,	0,	0,	0,
/* 56 - 60*/	0,	0,	0,  0,	0,
/* 61 - 63*/	0,	0,	0,
};

void mt_pll_post_init(void)
{
	unsigned int temp,clk_misc_cfg_0,ret_feq,reg_temp;
	unsigned int isFail = 0;

	#ifdef _FREQ_SCAN_
	print("Pll post init start...\n");

	print("==============================\n");
	print("==      Parsing Start       ==\n");
	print("==============================\n");
	for(temp=1; temp<=39; temp++) /*fmeter has 39 clock source*/
	{
		ret_feq = 0;
		if(temp==18)
		{
			continue;
		}

		ret_feq = mt_get_pll_freq(temp);
		ret_feq_store[ret_feq_total] = ret_feq;
		ret_feq_total++;
		print("%d",ret_feq);
#ifdef _RESULT_COMPARE_
		if((ret_feq < (ckgen_golden_array[temp-1] - Range)) || (ret_feq > (ckgen_golden_array[temp-1] + Range)))
		{
			print(" : ERROR");
			isFail = 1;
		}
#endif
		printf("\n");

	}

	//abist 
	for(temp=1; temp<=57; temp++) /*abist has 63 clock source*/
	{
		if(!ckgen_abist_needCheck_array[temp-1])
		{
		 	continue;
		}
		ret_feq = mt_get_abist_freq(temp);
		ret_feq_store[ret_feq_total] = ret_feq;
		ret_feq_total++;

		print("%d", ret_feq);
#ifdef _RESULT_COMPARE_
		if(ckgen_abist_golden_array[temp-1] > 0)
		{
			if((ret_feq < (ckgen_abist_golden_array[temp-1] - Range)) || (ret_feq > (ckgen_abist_golden_array[temp-1] + Range)))
			{	
				print(" : ERROR");
				isFail = 1;
			}
		}
#endif
		printf("\n");

	}


#ifdef _RESULT_COMPARE_
	if(isFail)
	{
		print("Check PLL/CLK Freq Fail..!!!\n");
		//while(1);
	}
	else
	{
		print("Pass\n");
	}

	pll_chk_is_fail = isFail;

	#endif

	print("Pll post init Done!\n");
#endif // _FREQ_SCAN_
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
	DRV_WriteReg32(CLK_CFG_UPDATE, 0xFFFFFFFD);    //update all clocks except "fmem & axi"
	DRV_WriteReg32(CLK_CFG_UPDATE1, 0x00000007);
    printf("[PLL] mt_set_topck_default done\n");
}

void mt_pll_init(void)
{
    int ret = 0;
    unsigned int temp;
    unsigned int rdata = 0;
    unsigned int wdata = 0;
    int i;

    print("Pll init start...\n");

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
    temp = DRV_Reg32(ARMPLL_L_PWR_CON0);
    DRV_WriteReg32(ARMPLL_L_PWR_CON0, temp | 0x1);

	//step 5
    temp = DRV_Reg32(ARMPLL_LL_PWR_CON0);
    DRV_WriteReg32(ARMPLL_LL_PWR_CON0, temp | 0x1);

    //step 6
    temp = DRV_Reg32(MAINPLL_PWR_CON0);
    DRV_WriteReg32(MAINPLL_PWR_CON0, temp | 0x1);

    //step 7
    temp = DRV_Reg32(UNIVPLL_PWR_CON0);
    DRV_WriteReg32(UNIVPLL_PWR_CON0, temp | 0x1);

    //step 8
    temp = DRV_Reg32(MMPLL_PWR_CON0);
    DRV_WriteReg32(MMPLL_PWR_CON0, temp | 0x1);

    //step 9
    temp = DRV_Reg32(MSDCPLL_PWR_CON0);
    DRV_WriteReg32(MSDCPLL_PWR_CON0, temp | 0x1);

    //step 10
    temp = DRV_Reg32(VENCPLL_PWR_CON0);
    DRV_WriteReg32(VENCPLL_PWR_CON0, temp | 0x1);

    //step 11
    temp = DRV_Reg32(TVDPLL_PWR_CON0);
    DRV_WriteReg32(TVDPLL_PWR_CON0, temp | 0x1);

    //step 12
    //temp = DRV_Reg32(MPLL_PWR_CON0);
    //DRV_WriteReg32(MPLL_PWR_CON0, temp | 0x1);

    //step 13
    temp = DRV_Reg32(CCIPLL_PWR_CON0);
    DRV_WriteReg32(CCIPLL_PWR_CON0, temp | 0x1);

    //step 14
    temp = DRV_Reg32(APLL1_PWR_CON0);
    DRV_WriteReg32(APLL1_PWR_CON0, temp | 0x1);

    //step 15
    temp = DRV_Reg32(APLL2_PWR_CON0);
    DRV_WriteReg32(APLL2_PWR_CON0, temp | 0x1);

    /******************
    	* xPLL ISO Disable
    	*******************/
    //step 16
    temp = DRV_Reg32(ARMPLL_L_PWR_CON0);
    DRV_WriteReg32(ARMPLL_L_PWR_CON0, temp & 0xFFFFFFFD);

	//step 17
    temp = DRV_Reg32(ARMPLL_LL_PWR_CON0);
    DRV_WriteReg32(ARMPLL_LL_PWR_CON0, temp & 0xFFFFFFFD);

	//step 18
    temp = DRV_Reg32(MAINPLL_PWR_CON0);
    DRV_WriteReg32(MAINPLL_PWR_CON0, temp & 0xFFFFFFFD);

    //step 19
    temp = DRV_Reg32(UNIVPLL_PWR_CON0);
    DRV_WriteReg32(UNIVPLL_PWR_CON0, temp & 0xFFFFFFFD);

    //step 20
    temp = DRV_Reg32(MMPLL_CON0);
    DRV_WriteReg32(MMPLL_CON0, temp & 0xFFFFFFFD);

    //step 21
    temp = DRV_Reg32(MSDCPLL_PWR_CON0);
    DRV_WriteReg32(MSDCPLL_PWR_CON0, temp & 0xFFFFFFFD);

    //step 22
    temp = DRV_Reg32(VENCPLL_PWR_CON0);
    DRV_WriteReg32(VENCPLL_PWR_CON0, temp & 0xFFFFFFFD);

	//step 23
    temp = DRV_Reg32(TVDPLL_PWR_CON0);
    DRV_WriteReg32(TVDPLL_PWR_CON0, temp & 0xFFFFFFFD);

    //step 24
    //temp = DRV_Reg32(MPLL_PWR_CON0);
    //DRV_WriteReg32(MPLL_PWR_CON0, temp & 0xFFFFFFFD);

    //step 25
    temp = DRV_Reg32(CCIPLL_PWR_CON0);
    DRV_WriteReg32(CCIPLL_PWR_CON0, temp & 0xFFFFFFFD);

    //step 26
    temp = DRV_Reg32(APLL1_PWR_CON0);
    DRV_WriteReg32(APLL1_PWR_CON0, temp & 0xFFFFFFFD);

    //step 27
    temp = DRV_Reg32(APLL2_PWR_CON0);
    DRV_WriteReg32(APLL2_PWR_CON0, temp & 0xFFFFFFFD);

    /********************
       * xPLL Frequency Set
       *********************/

    //step 28: CPU SPEED*****
    DRV_WriteReg32(ARMPLL_L_CON1, 0x800F24EC); // 1575MHz

    //step 29: CPU SPEED*****
    //DRV_WriteReg32(ARMPLL_LL_CON1, 0x8113B627); // 1025MHz
    DRV_WriteReg32(ARMPLL_LL_CON1, 0x81190000); // 1300MHz:0x81190000

    //step 30
    DRV_WriteReg32(MAINPLL_CON1, 0x80150000); //1092MHz

    //step 31
    DRV_WriteReg32(MMPLL_CON1, 0x821BB13B); // 720MHz

    //step 32
    DRV_WriteReg32(MSDCPLL_CON1, 0x801C9D89); //372Mhz:0x801C9D89 ,400Mhz:0x801EC4EC ,old 416MHz : 0x80200000

    //step 33
    //DRV_WriteReg32(VENCPLL_CON1, 0x80229D8A); //450MHz
    DRV_WriteReg32(VENCPLL_CON1, 0x801EC4EC); //400MHz

    //step 34
    DRV_WriteReg32(TVDPLL_CON1, 0x8016D89E); //594MHz

    //step 35
    //DRV_WriteReg32(MMPLL_CON1, 0x80100000); //208MHz

    //step 36
    //DRV_WriteReg32(MMPLL_CON0, 0x00010130); //MPLL POSTDIV

    //step 37
    DRV_WriteReg32(CCIPLL_CON1, 0x821809D8); //625MHz

    /***********************
      * xPLL Frequency Enable
      ************************/
    //step 38
    temp = DRV_Reg32(ARMPLL_L_CON0);
    DRV_WriteReg32(ARMPLL_L_CON0, temp | 0x1);

	//step 39
    temp = DRV_Reg32(ARMPLL_LL_CON0);
    DRV_WriteReg32(ARMPLL_LL_CON0, temp | 0x1);

    //step 40
    temp = DRV_Reg32(MAINPLL_CON0);
    DRV_WriteReg32(MAINPLL_CON0, temp | 0x1);

    //step 41
    temp = DRV_Reg32(UNIVPLL_CON0);
    DRV_WriteReg32(UNIVPLL_CON0, temp | 0x1);

    //step 42
    temp = DRV_Reg32(MMPLL_CON0);
    DRV_WriteReg32(MMPLL_CON0, temp | 0x1);

    //step 43
    temp = DRV_Reg32(MSDCPLL_CON0);
    DRV_WriteReg32(MSDCPLL_CON0, temp | 0x1);

    //step 44
    temp = DRV_Reg32(VENCPLL_CON0);
    DRV_WriteReg32(VENCPLL_CON0, temp | 0x1);

	//step 45
	temp = DRV_Reg32(TVDPLL_CON0);
	DRV_WriteReg32(TVDPLL_CON0, temp | 0x1);

	//step 46
	//temp = DRV_Reg32(MPLL_CON0);
	//DRV_WriteReg32(MPLL_CON0, temp | 0x1);

    //step 47
    temp = DRV_Reg32(CCIPLL_CON0);
    DRV_WriteReg32(CCIPLL_CON0, temp | 0x1);

    //step 48
    temp = DRV_Reg32(APLL1_CON0);
    DRV_WriteReg32(APLL1_CON0, temp | 0x1);

    //step 49
    temp = DRV_Reg32(APLL2_CON0);
    DRV_WriteReg32(APLL2_CON0, temp | 0x1);

	// step 50
    gpt_busy_wait_us(20); // wait for PLL stable (min delay is 20us)

    /***************
      * xPLL DIV RSTB
      ****************/

	//step 51
	temp = DRV_Reg32(ARMPLL_LL_CON0);
	DRV_WriteReg32(ARMPLL_LL_CON0, temp | 0x01000000);

	//step 52
	temp = DRV_Reg32(MAINPLL_CON0);
	DRV_WriteReg32(MAINPLL_CON0, temp | 0x01000000);

	//step 53
	temp = DRV_Reg32(UNIVPLL_CON0);
	DRV_WriteReg32(UNIVPLL_CON0, temp | 0x01000000);


    /*****************
	* xPLL HW Control
    	******************/
	#if 0 //move to kernel
	//switch to CLKSQ & PLL to HW Control
    //step 54
    DRV_WriteReg32(AP_PLL_CON3, 0x00044440);

    //step 55
    DRV_WriteReg32(AP_PLL_CON4, temp & 0xC);
	#endif
	
	/*************
	   * set armpll HW mode
	**************/

	 /*************
    	* ULPOSC Init
   	 **************/
	/*240Mhz*/
	DRV_WriteReg32(INFRA_PLL_ULPOSC_CON0, 0x00940E28);
	DRV_WriteReg32(INFRA_PLL_ULPOSC_CON1, 0x00000004);

	/* enable */
	/*DRV_WriteReg32(ULPOSC_CON, 0x00000001);
	gpt_busy_wait_us(30);
	DRV_WriteReg32(ULPOSC_CON, 0x00000005);*/


    /*************
    	* MEMPLL Init
   	 **************/

	//mt_mempll_pre();

    /**************
    	* INFRA CPU CLKMUX, CLK Div
    	***************/

#if 0 //need if vproc < 0.7v 
	temp = DRV_Reg32(TOP_CKDIV1_BIG) ;
	//DRV_WriteReg32(TOP_CKDIV1_BIG, (temp & 0xFFFFFFE0) | 0xA); // BIG CPU clock divide by 2
	DRV_WriteReg32(TOP_CKDIV1_BIG, temp & 0xFFFFFFE0); // BIG CPU clock divide by 1
	
	temp = DRV_Reg32(TOP_CKDIV1_SML);
	DRV_WriteReg32(TOP_CKDIV1_SML, temp & 0xFFFFFFE0); // SML CPU clock divide by 1
	
	temp = DRV_Reg32(TOP_CKDIV1_BUS);
	//DRV_WriteReg32(TOP_CKDIV1_BUS, temp & 0xFFFFFFE0); // BUS CPU clock divide by 1
	DRV_WriteReg32(TOP_CKDIV1_BUS, temp & (temp & 0xFFFFFFE0) | 0xA ); // BUS CPU clock divide by 1, CCI = ARMSPLL, div = 2
#endif	
	temp = DRV_Reg32(TOP_CKMUXSEL);
	DRV_WriteReg32(TOP_CKMUXSEL, temp | 0x1111); // switch CA7&CA15 CLKSQ to ARMPLL, BIG CPU clock to 1700M ; SML CPU to 1000M ; BUS CPU to 1700M (or 1000M to write temp | 0x2110)


    /************
  	  * TOP CLKMUX
  	  *************/
	DRV_WriteReg32(INFRA_BUS_DCM_CTRL, DRV_Reg32(INFRA_BUS_DCM_CTRL) | (1 << 21) | (1 << 22));

    // config AXI clock first
    DRV_WriteReg32(CLK_CFG_0, 0x00000002);      // axi=syspll2_d2
    DRV_WriteReg32(CLK_CFG_UPDATE, 0x00000001); //update AXI clock

    // config other clocks
    DRV_WriteReg32(CLK_CFG_0, 0x02010102);// mm_ck=vencpll_ck, ddrphycfg_ck=syspll1_d8, mem_ck=dmpllL, axi=syspll2_d2,
    DRV_WriteReg32(CLK_CFG_1, 0x01010301);// mfg_ck=mmpll, venc_ck=mm_ck, vdec_ck=syspll1_d2, pwm_ck=x
    DRV_WriteReg32(CLK_CFG_2, 0x01010001);// usb20_ck=x, spi_ck=syspll3_d2, uart=26M, camtg=univpll_d26
    DRV_WriteReg32(CLK_CFG_3, 0x07010101);// msdc30_1=msdcpll_d2, msdc50_0=msdcpll_ck, msdc50_0_hclk=syspll1_d2, img_ck=mm_ck
    DRV_WriteReg32(CLK_CFG_4, 0x01000707);// aud_intbus=syspll1_d4, aud=26m, msdc30_3=syspll3_d4, msdc30_2=msdcpll_d2
    DRV_WriteReg32(CLK_CFG_5, 0x01020100);// mjc_ck= x, atb_ck=syspll_d5, scp_ck=x, pmicspi=26MHz
    DRV_WriteReg32(CLK_CFG_6, 0x01010101);// aud2_ck=apll2 , aud1_ck=apll1, scam=syspll2_d2, dpi0_ck=tvdpll_d2
    DRV_WriteReg32(CLK_CFG_7, 0x01010100);// usb_top=univpll3_d4, ssudb_top_xhci=univpll3_d2, ssusb_top_sys= univpll3_d2, disp_pwm=osc_d8
    DRV_WriteReg32(CLK_CFG_8, 0x01010201);// dvfsp_ck= syspll1_d8, i2c_ck=syspll1_d8, bsi_spi=syspll1_d4, hg_fspm=syspll1_d8
	DRV_WriteReg32(CLK_CFG_9, 0x00000003);// null, null,null,f52m_mfg=univpll2_d8

    DRV_WriteReg32(CLK_CFG_UPDATE, 0xFFFFFFFC);    //update all clocks except "fmem & axi"
	DRV_WriteReg32(CLK_CFG_UPDATE1, 0x00000007);

    DRV_WriteReg32(CLK_SCP_CFG_0, 0x703EF);    //enable scpsys clock off control
    DRV_WriteReg32(CLK_SCP_CFG_1,  0xD);    //enable scpsys clock off control


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
    /*for CG*/
    DRV_WriteReg32(INFRA_PDN_CLR0, 0xFFFFFFFF);
    DRV_WriteReg32(INFRA_PDN_CLR1, 0xFFFFFFFF);
    DRV_WriteReg32(INFRA_PDN_CLR2, 0xFFFFFFFF);

    /*DISP CG Clear and DCM disable*/
    DRV_WriteReg32(DISP_CG_CLR0, 0xFFFFFFFF);
    DRV_WriteReg32(DISP_CG_CLR1, 0x0003FFFF);

    print("Pll init Done!\n");
}

