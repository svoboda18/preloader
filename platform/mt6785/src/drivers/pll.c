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

#if 0
const int ckgen_needCheck_array[] =
{
/* 01 - 05*/	1, 1, 1, 1, 1,
/* 06 - 10*/	1, 1, 1, 1, 1,
/* 11 - 15*/	1, 1, 1, 1, 1,
/* 16 - 20*/	1, 1, 1, 1, 1,
/* 21 - 25*/	1, 1, 1, 1, 1,
/* 26 - 30*/	1, 1, 1, 1, 1,
/* 31 - 35*/	1, 1, 1, 1, 1,
/* 36 - 40*/	1, 1, 1, 1, 1,
/* 41 - 45*/	1, 1, 1, 1, 1,
/* 46 - 50*/	1, 1, 1, 1, 1,
/* 51 - 55*/	1, 1, 1, 1, 1,
/* 56 - 60*/	0, 0, 0, 0, 0,
};

const char *ckgen_golden_array[] =
{
/*add if need*/
/* 01 - 05*/	156000, 594000, 26000, 26000, 594000,
/* 06 - 10*/	546000, 546000, 560000, 560000, 624000,
/* 11 - 15*/	700000, 700000, 785000, 624000, 100000,
/* 16 - 20*/	52000, 52000, 52000, 52000, 52000,
/* 21 - 25*/	26000, 109200, 273000, 384000, 192000,
/* 26 - 30*/	26000, 136500, 26000, 273000, 273000,
/* 31 - 35*/	297000, 109200, 26000, 124800, 124800,
/* 36 - 40*/	68250, 124800, 525000, 525000, 450000,
/* 41 - 45*/	273000, 22579, 24576, 546000, 136500,
/* 46 - 50*/	180633, 196608, 787500, 364000, 630000,
/* 51 - 55*/	624000, 208000, 78000, 196608, 273000,
/* 56 - 60*/	0, 0, 0, 0, 0,
};

const int abist_needCheck_array[] =
{
/* 01 - 05*/	0, 0, 0, 0, 0,
/* 06 - 10*/	0, 0, 0, 0, 0,
/* 11 - 15*/	0, 0, 1, 0, 0,
/* 16 - 20*/	1, 1, 1, 1, 1,
/* 21 - 25*/	0, 1, 1, 1, 1,
/* 26 - 30*/	1, 1, 1, 1, 0,
/* 31 - 35*/	0, 1, 0, 1, 0,
/* 36 - 40*/	0, 0, 0, 0, 0,
/* 41 - 45*/	0, 0, 0, 0, 0,
/* 46 - 50*/	0, 0, 0, 1, 1,
/* 51 - 55*/	0, 0, 0, 0, 1,
/* 56 - 60*/	0, 0, 0, 0, 0,
};

/* if needCheck_array[x]=1 but golden_array[x]=0: means we only want to read the value*/
const int abist_golden_array[] =
{
/* 01 - 05*/	0, 0, 0, 0, 0,
/* 06 - 10*/	0, 0, 0, 0, 0,
/* 11 - 15*/	0, 0, 2800000, 0, 0,
/* 16 - 20*/	26000, 700000, 560000, 466667, 1175000,
/* 21 - 25*/	0, 1087000, 1092000, 1248000, 100000,
/* 26 - 30*/	384000, 594000, 180633, 196608, 0,
/* 31 - 35*/	0, 192000, 0, 594000, 0,
/* 36 - 40*/	0, 0, 0, 0, 0,
/* 41 - 45*/	0, 0, 0, 0, 0,
/* 46 - 50*/	0, 0, 0, 1204000, 208000,
/* 51 - 55*/	0, 0, 0, 0, 48000,
/* 56 - 60*/	0, 0, 0, 0, 0,
};


#define _FREQ_SCAN_
#define _RESULT_COMPARE_
#define Range 1000
#define CKGEN_CHANNEL_CNT 60
#define ABIST_CHANNEL_CNT 60
unsigned int ret_feq_store[CKGEN_CHANNEL_CNT+ABIST_CHANNEL_CNT];
unsigned int ret_feq_total=0;
unsigned int pll_chk_is_fail = 0;

void mt_print_pll_chcek_result(void)
{
	unsigned int temp,clk_misc_cfg_0,ret_feq,reg_temp;
	unsigned int isFail = 0;
	unsigned int total = 0;

	#ifdef _FREQ_SCAN_

	print("==============================\n");
	print("==      Parsing Start       ==\n");
	print("==============================\n");
	for(temp=1; temp<CKGEN_CHANNEL_CNT; temp++)
	{
		if(!ckgen_needCheck_array[temp-1])
		{
		  //print("skip:%d\n",temp);
		 	continue;
		}
		ret_feq = ret_feq_store[total];
		total++;
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
	for(temp=1; temp<ABIST_CHANNEL_CNT; temp++)
	{
		if(!abist_needCheck_array[temp-1])
		{
		  //print("skip:%d\n",temp);
		 	continue;
		}
		ret_feq = ret_feq_store[total];
		total++;

		//print("%s:", ckgen_abist_array[temp-1]);

		print("%d", ret_feq);
#ifdef _RESULT_COMPARE_
		if(abist_golden_array[temp-1] > 0)
		{
			if((ret_feq < (abist_golden_array[temp-1] - Range)) || (ret_feq > (abist_golden_array[temp-1] + Range)))
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
		print("Post Check PLL/CLK Freq Fail..!!!\n");
		//while(1);
	}
	else
	{
		print("Pass\n");
	}
#endif
#endif //_FREQ_SCAN_

}
#endif

void mt_pll_post_init(void)
{
}

void mt_set_topck_default(void)
{
	DRV_WriteReg32(CLK_CFG_0_CLR, 0xfffffffe);
	DRV_WriteReg32(CLK_CFG_1_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_2_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_3_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_4_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_5_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_6_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_7_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_8_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_9_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_10_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_11_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_12_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_13_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_UPDATE, 0x7FFFFFFE);
	DRV_WriteReg32(CLK_CFG_UPDATE1, 0x81FFFFFF);
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

    //step 4
    temp = DRV_Reg32(AP_PLL_CON6);
    DRV_WriteReg32(AP_PLL_CON6, temp & 0xFFFFDFFF);
    /*************
 	   * xPLL PWR ON
 	   **************/
    //step 5
    temp = DRV_Reg32(MFGPLL_PWR_CON0);
    DRV_WriteReg32(MFGPLL_PWR_CON0, temp | 0x1);

    //step 6
    //temp = DRV_Reg32(MPLL_PWR_CON0);
    //DRV_WriteReg32(MPLL_PWR_CON0, temp | 0x1);

    //step 7
    temp = DRV_Reg32(MAINPLL_PWR_CON0);
    DRV_WriteReg32(MAINPLL_PWR_CON0, temp | 0x1);

    //step 8
    temp = DRV_Reg32(UNIVPLL_PWR_CON0);
    DRV_WriteReg32(UNIVPLL_PWR_CON0, temp | 0x1);

    //step 9
    temp = DRV_Reg32(MSDCPLL_PWR_CON0);
    DRV_WriteReg32(MSDCPLL_PWR_CON0, temp | 0x1);

    //step 10
    temp = DRV_Reg32(MMPLL_PWR_CON0);
    DRV_WriteReg32(MMPLL_PWR_CON0, temp | 0x1);

    //step 11
    temp = DRV_Reg32(TVDPLL_PWR_CON0);
    DRV_WriteReg32(TVDPLL_PWR_CON0, temp | 0x1);

    //step 12
    temp = DRV_Reg32(ADSPPLL_PWR_CON0);
    DRV_WriteReg32(ADSPPLL_PWR_CON0, temp | 0x1);

    //step 13
    temp = DRV_Reg32(APLL1_PWR_CON0);
    DRV_WriteReg32(APLL1_PWR_CON0, temp | 0x1);
    
    //step 14
    temp = DRV_Reg32(APLL2_PWR_CON0);
    DRV_WriteReg32(APLL2_PWR_CON0, temp | 0x1);
    
    //step 15
    temp = DRV_Reg32(ARMPLL_LL_PWR_CON0);
    DRV_WriteReg32(ARMPLL_LL_PWR_CON0, temp | 0x1);    
    
    //step 16
    temp = DRV_Reg32(ARMPLL_BL_PWR_CON0);
    DRV_WriteReg32(ARMPLL_BL_PWR_CON0, temp | 0x1);

    //step 17
    //temp = DRV_Reg32(ARMPLL_BB_PWR_CON0);
    //DRV_WriteReg32(ARMPLL_BB_PWR_CON0, temp | 0x1);

    //step 18
    temp = DRV_Reg32(APUPLL_PWR_CON0);
    DRV_WriteReg32(APUPLL_PWR_CON0, temp | 0x1);

    //step 19
    temp = DRV_Reg32(CCIPLL_PWR_CON0);
    DRV_WriteReg32(CCIPLL_PWR_CON0, temp | 0x1);

    //step 20
    gpt_busy_wait_us(100);	
    /******************
    	* xPLL ISO Disable
    	*******************/
    //step 21
    temp = DRV_Reg32(MFGPLL_PWR_CON0);
    DRV_WriteReg32(MFGPLL_PWR_CON0, temp & 0xFFFFFFFD);

    //step 22
    //temp = DRV_Reg32(MPLL_CON0);
    //DRV_WriteReg32(MPLL_CON0, temp & 0xFFFFFFFD);

    //step 23
    temp = DRV_Reg32(MAINPLL_PWR_CON0);
    DRV_WriteReg32(MAINPLL_PWR_CON0, temp & 0xFFFFFFFD);

    //step 24
    temp = DRV_Reg32(UNIVPLL_PWR_CON0);
    DRV_WriteReg32(UNIVPLL_PWR_CON0, temp & 0xFFFFFFFD);

    //step 25
    temp = DRV_Reg32(MSDCPLL_PWR_CON0);
    DRV_WriteReg32(MSDCPLL_PWR_CON0, temp & 0xFFFFFFFD);

    //step 26
    temp = DRV_Reg32(MMPLL_PWR_CON0);
    DRV_WriteReg32(MMPLL_PWR_CON0, temp & 0xFFFFFFFD);

    //step 27
    temp = DRV_Reg32(TVDPLL_PWR_CON0);
    DRV_WriteReg32(TVDPLL_PWR_CON0, temp & 0xFFFFFFFD);

    //step 28
    temp = DRV_Reg32(ADSPPLL_PWR_CON0);
    DRV_WriteReg32(ADSPPLL_PWR_CON0, temp & 0xFFFFFFFD);

    //step 29
    temp = DRV_Reg32(APLL1_PWR_CON0);
    DRV_WriteReg32(APLL1_PWR_CON0, temp & 0xFFFFFFFD);

    //step 30
    temp = DRV_Reg32(APLL2_PWR_CON0);
    DRV_WriteReg32(APLL2_PWR_CON0, temp & 0xFFFFFFFD);

    //step 31
    temp = DRV_Reg32(ARMPLL_LL_PWR_CON0);
    DRV_WriteReg32(ARMPLL_LL_PWR_CON0, temp & 0xFFFFFFFD);
    
    //step 32
    temp = DRV_Reg32(ARMPLL_BL_PWR_CON0);
    DRV_WriteReg32(ARMPLL_BL_PWR_CON0, temp & 0xFFFFFFFD);

    //step 33
    //temp = DRV_Reg32(ARMPLL_BB_PWR_CON0);
    //DRV_WriteReg32(ARMPLL_BB_PWR_CON0, temp & 0xFFFFFFFD);

    //step 34
    temp = DRV_Reg32(APUPLL_PWR_CON0);
    DRV_WriteReg32(APUPLL_PWR_CON0, temp & 0xFFFFFFFD);

    //step 35
    temp = DRV_Reg32(CCIPLL_PWR_CON0);
    DRV_WriteReg32(CCIPLL_PWR_CON0, temp & 0xFFFFFFFD);

    //step 36
    gpt_busy_wait_us(50);
    
    //step 37
    temp = DRV_Reg32(APLL1_CON0);
    DRV_WriteReg32(APLL1_CON0, temp | 0x00000000);

    //step 38
    temp = DRV_Reg32(APLL2_CON0);
    DRV_WriteReg32(APLL2_CON0, temp | 0x00000000);
    /********************
       * xPLL Frequency Set
       *********************/
    //step 39
    //DRV_WriteReg32(MFGPLL_CON1, 0x830f6276); // 200MHz
    //DRV_WriteReg32(MFGPLL_CON1, 0x8213B13B); //512MHz - E1
    //DRV_WriteReg32(MFGPLL_CON1, 0x82189D89); //640MHz
    //DRV_WriteReg32(MFGPLL_CON1, 0x821EC4EC); //800MHz
    //DRV_WriteReg32(MFGPLL_CON1, 0x840F6276); //100MHz
    //DRV_WriteReg32(MFGPLL_CON1, 0x8217D89D); //620MHz
    //DRV_WriteReg32(MFGPLL_CON1, 0x84133B13); //125MHz
    DRV_WriteReg32(MFGPLL_CON1, 0x8314C4EC); //270MHz

    //step 41
    DRV_WriteReg32(MAINPLL_CON1, 0x81150000); //1092MHz

    //step 42
    DRV_WriteReg32(UNIVPLL_CON1, 0x80180000); //1248MHz

    //step 43
    //DRV_WriteReg32(MSDCPLL_CON1, 0x831EC4EC); //400MHz  FIX BY HK
    //DRV_WriteReg32(MSDCPLL_CON1, 0x831C9D89); //372MHz  FIX BY HK
    //DRV_WriteReg32(MSDCPLL_CON1, 0x83200000); //416MHz
    DRV_WriteReg32(MSDCPLL_CON1, 0x831D89D8); //384MHz

    //step 44
    //DRV_WriteReg32(MMPLL_CON1, 0x831713B1); // 300MHz
    //DRV_WriteReg32(MMPLL_CON1, 0x83204EC4); // 420MHz
    DRV_WriteReg32(MMPLL_CON1, 0x801E49D8); // 3150MHz

    //step 45
    //DRV_WriteReg32(TVDPLL_CON1, 0x8216D89E); //594MHz
    //DRV_WriteReg32(TVDPLL_CON1, 0x82174EC4); //606MHz
    //DRV_WriteReg32(TVDPLL_CON1, 0x82114EC4); //450MHz
    DRV_WriteReg32(TVDPLL_CON1, 0x821589D8); //560MHz

    //step 46
    //DRV_WriteReg32(ADSPPLL_CON1, 0x821EC4EC); //800MHz
    //DRV_WriteReg32(ADSPPLL_CON1, 0x82127627); //480MHz
    DRV_WriteReg32(ADSPPLL_CON1, 0x801AEC4E); //2800MHz

    //step 47
    DRV_WriteReg32(APLL1_CON1, 0x6F28BD4C); //180.6336MHz
    DRV_WriteReg32(APLL1_CON2, 0x6F28BD4D); //180.6336MHz
    //step 48
    DRV_WriteReg32(APLL2_CON1, 0x78FD5265); //196.608MHz
    DRV_WriteReg32(APLL2_CON2, 0x78FD5266); //196.608MHz
    //step 49
    DRV_WriteReg32(APLL1_CON0, 0x80000188);
    
    //step 50
    DRV_WriteReg32(APLL2_CON0, 0x80000188);
     
    //step 51: CPU SPEED*****
    //DRV_WriteReg32(ARMPLL_LL_CON1, 0x81110000); // 884MHz, arm div = 1
    //DRV_WriteReg32(ARMPLL_LL_CON1, 0x81152762); // 1100MHz, pos div = 2, arm div = 1
    //DRV_WriteReg32(ARMPLL_LL_CON1, 0x8120B13B); // 1700MHz, pos div = 2, arm div = 1, need confirm opp
    //DRV_WriteReg32(ARMPLL_LL_CON1, 0x811AEC4E); // 1400MHz, pos div = 2, arm div = 1, need confirm opp
    //DRV_WriteReg32(ARMPLL_LL_CON1, 0x81193B13); // 1312MHz, pos div = 2, arm div = 1, need confirm opp
    //DRV_WriteReg32(ARMPLL_LL_CON1, 0x8115DD89); // 1137MHz, pos div = 2, arm div = 1, need confirm opp
    //DRV_WriteReg32(ARMPLL_LL_CON1, 0x811713B1); // 1200MHz, pos div = 2, arm div = 1, need confirm opp
    DRV_WriteReg32(ARMPLL_LL_CON1, 0x8116989D); // 1175MHz, pos div = 2, arm div = 1, need confirm opp
    //step 52
    //DRV_WriteReg32(ARMPLL_L_CON1, 0x8118C4EC); // 1288MHz, arm div = 1
    //DRV_WriteReg32(ARMPLL_L_CON1, 0x811713B1); // 1200MHz, pss div = 2, arm div = 1
    //DRV_WriteReg32(ARMPLL_BL_CON1, 0x80152762); // 2200MHz, pos div = 1, arm div = 1, need confirm opp
    //DRV_WriteReg32(ARMPLL_BL_CON1, 0x80113627); // 1790MHz, pos div = 1, arm div = 1, need confirm opp
    //DRV_WriteReg32(ARMPLL_BL_CON1, 0x80102276); // 1678MHz, pos div = 1, arm div = 1, need confirm opp
    //DRV_WriteReg32(ARMPLL_BL_CON1, 0x811584EC); // 1119MHz, pos div = 2, arm div = 1, need confirm opp
    //DRV_WriteReg32(ARMPLL_BL_CON1, 0x81152762); // 1100MHz, pos div = 2, arm div = 1, need confirm opp
    //DRV_WriteReg32(ARMPLL_BL_CON1, 0x8114E762); // 1087MHz, pos div = 2, arm div = 1, need confirm opp
    DRV_WriteReg32(ARMPLL_BL_CON1, 0x8114DD89); // 1085MHz, pos div = 2, arm div = 1, need confirm opp
    //step 53
    //DRV_WriteReg32(ARMPLL_BB_CON1, 0x80152762); // 2200MHz, pos div = 1, arm div = 1, need confirm opp
    //DRV_WriteReg32(ARMPLL_BB_CON1, 0x80113627); // 1790MHz, pos div = 1, arm div = 1, need confirm opp
    //DRV_WriteReg32(ARMPLL_BB_CON1, 0x80102276); // 1678MHz, pos div = 1, arm div = 1, need confirm opp
    //DRV_WriteReg32(ARMPLL_BB_CON1, 0x811584EC); // 1119MHz, pos div = 2, arm div = 1, need confirm opp
    //step 54
    DRV_WriteReg32(APUPLL_CON1, 0x821CD89D); // 750MHz
    //step 55
    //DRV_WriteReg32(CCIPLL_CON1, 0x821713B1); // 600MHz, arm div = 2
    //DRV_WriteReg32(CCIPLL_CON1, 0x81152762); // 1100MHz/2, pos div = 2, arm div = 2
    //DRV_WriteReg32(CCIPLL_CON1, 0x81170000); // 1196/2MHz, pos div = 2, arm div = 2
    //DRV_WriteReg32(CCIPLL_CON1, 0x8120B13B); // 1700MHz, pos div = 2, arm div = 1, need confirm opp
    //DRV_WriteReg32(CCIPLL_CON1, 0x81133627); // 999MHz, pos div = 2, arm div = 1, need confirm opp
    //DRV_WriteReg32(CCIPLL_CON1, 0x8111C4EC); // 924MHz, pos div = 2, arm div = 1, need confirm opp
    //DRV_WriteReg32(CCIPLL_CON1, 0x821713B1); // 600MHz, pos div = 2, arm div = 1, need confirm opp
    //DRV_WriteReg32(CCIPLL_CON1, 0x81172762); // 602MHz, pos div = 2, arm div = 2, need confirm opp
    DRV_WriteReg32(CCIPLL_CON1, 0x821789D8); // 612MHz, pos div = 4, arm div = 1, need confirm opp
    /***********************
      * xPLL Frequency Enable
      ************************/
     //step 56
     temp = DRV_Reg32(MFGPLL_CON0);
     DRV_WriteReg32(MFGPLL_CON0, temp | 0x1);
 
    //step 58
    temp = DRV_Reg32(MAINPLL_CON0);
    DRV_WriteReg32(MAINPLL_CON0, temp | 0x1);

    //step 59
    temp = DRV_Reg32(UNIVPLL_CON0);
    DRV_WriteReg32(UNIVPLL_CON0, temp | 0x1);

    //step 60
    temp = DRV_Reg32(MSDCPLL_CON0);
    DRV_WriteReg32(MSDCPLL_CON0, temp | 0x1);

    //step 61
    temp = DRV_Reg32(MMPLL_CON0);
    DRV_WriteReg32(MMPLL_CON0, temp | 0x1);

    //step 62
    temp = DRV_Reg32(TVDPLL_CON0);
    DRV_WriteReg32(TVDPLL_CON0, temp | 0x1);

    //step 63
    temp = DRV_Reg32(ADSPPLL_CON0);
    DRV_WriteReg32(ADSPPLL_CON0, temp | 0x1);

    //step 64
    temp = DRV_Reg32(APLL1_CON0);
    DRV_WriteReg32(APLL1_CON0, temp | 0x1);

    //step 65
    temp = DRV_Reg32(APLL2_CON0);
    DRV_WriteReg32(APLL2_CON0, temp | 0x1);

    //step 66
    temp = DRV_Reg32(ARMPLL_LL_CON0);
    DRV_WriteReg32(ARMPLL_LL_CON0, temp | 0x1);

    //step 67
    temp = DRV_Reg32(ARMPLL_BL_CON0);
    DRV_WriteReg32(ARMPLL_BL_CON0, temp | 0x1);

    //step 68
    //temp = DRV_Reg32(ARMPLL_BB_CON0);
    //DRV_WriteReg32(ARMPLL_BB_CON0, temp | 0x1);

    //step 69
    temp = DRV_Reg32(APUPLL_CON0);
    DRV_WriteReg32(APUPLL_CON0, temp | 0x1);

    //step 70
    temp = DRV_Reg32(CCIPLL_CON0);
    DRV_WriteReg32(CCIPLL_CON0, temp | 0x1);

    // step 71
    gpt_busy_wait_us(50); // wait for PLL stable (min delay is 20us)

    /***************
      * xPLL DIV RSTB
      ****************/
    //step 72
    temp = DRV_Reg32(MAINPLL_CON0);
    DRV_WriteReg32(MAINPLL_CON0, temp | 0x01000000);
	
    //step 73
    temp = DRV_Reg32(UNIVPLL_CON0);
    DRV_WriteReg32(UNIVPLL_CON0, temp | 0x01000000);

    //step 74
    temp = DRV_Reg32(MMPLL_CON0);
    DRV_WriteReg32(MMPLL_CON0, temp | 0x00800000);
    
    //step 75
    temp = DRV_Reg32(ADSPPLL_CON0);
    DRV_WriteReg32(ADSPPLL_CON0, temp | 0x00800000);
    /*****************
	* xPLL HW Control
    	******************/

    /**************
    	* INFRA CPU CLKMUX, CLK Div
    	***************/

//8: div1, A: div2, B: div4, 1D: div6
    temp = DRV_Reg32(CPU_PLLDIV_CFG0) ;
    DRV_WriteReg32(CPU_PLLDIV_CFG0, (temp & 0xFFE1FFFF) | (0x08 << 17)); // [21:17] divsel: CPU clock divide by 1

    temp = DRV_Reg32(CPU_PLLDIV_CFG1) ;
    DRV_WriteReg32(CPU_PLLDIV_CFG1, (temp & 0xFFE1FFFF) | (0x08 << 17)); // [21:17] divsel: CPU clock divide by 1

    //temp = DRV_Reg32(CPU_PLLDIV_CFG2) ;
    //DRV_WriteReg32(CPU_PLLDIV_CFG2, (temp & 0xFFE1FFFF) | (0x08 << 17)); // [21:17] divsel: CPU clock divide by 1

    temp = DRV_Reg32(BUS_PLLDIV_CFG) ;
    DRV_WriteReg32(BUS_PLLDIV_CFG, (temp & 0xFFE1FFFF) | (0x08 << 17)); // [21:17] divsel: CPU clock divide by 1

    //0: 26M,  1: armpll, 2:  Mainpll, 3:  Unipll
    temp = DRV_Reg32(CPU_PLLDIV_CFG0);
    DRV_WriteReg32(CPU_PLLDIV_CFG0, (temp & 0xFFFFF9FF) | (0x01 << 9)); // [10:9] muxsel: switch to PLL speed

    temp = DRV_Reg32(CPU_PLLDIV_CFG1);
    DRV_WriteReg32(CPU_PLLDIV_CFG1, (temp & 0xFFFFF9FF) | (0x01 << 9)); // [10:9] muxsel: switch to PLL speed

    //temp = DRV_Reg32(CPU_PLLDIV_CFG2);
    //DRV_WriteReg32(CPU_PLLDIV_CFG2, (temp & 0xFFFFF9FF) | (0x01 << 9)); // [10:9] muxsel: switch to PLL speed

    temp = DRV_Reg32(BUS_PLLDIV_CFG);
    DRV_WriteReg32(BUS_PLLDIV_CFG, (temp & 0xFFFFF9FF) | (0x01 << 9)); // [10:9] muxsel: switch to PLL speed

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
    DRV_WriteReg32(CLK_CFG_0_CLR, 0x00000003);
    DRV_WriteReg32(CLK_CFG_0_SET, 0x00000002);  // axi
    DRV_WriteReg32(CLK_CFG_UPDATE, 0x00000001); //update AXI clock

    //DRV_WriteReg32(CLK_CFG_20_SET, 0x00030000); //set CAM[16] & MM[17] fast clock transition
    // config other clocks
    // 0.8v setting
    DRV_WriteReg32(CLK_CFG_0_CLR, 0xFFFFFFFF);
    //DRV_WriteReg32(CLK_CFG_0_SET, 0x01040102);
    DRV_WriteReg32(CLK_CFG_0_SET, 0x00000502);//scp[18:16]: 26M, cksys_fmem[24]:26M
   
    DRV_WriteReg32(CLK_CFG_1_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_1_SET, 0x08000006);
    
    DRV_WriteReg32(CLK_CFG_2_CLR, 0xFFFFFFFF);
    //DRV_WriteReg32(CLK_CFG_2_SET, 0x0f0f0e08);
    DRV_WriteReg32(CLK_CFG_2_SET, 0x0f0f0d00);
    
    DRV_WriteReg32(CLK_CFG_3_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_3_SET, 0x00010d00);
    
    DRV_WriteReg32(CLK_CFG_4_CLR, 0xFFFFFFFF);
    //DRV_WriteReg32(CLK_CFG_4_SET, 0x02020202);
    DRV_WriteReg32(CLK_CFG_4_SET, 0x01010101);
    
    DRV_WriteReg32(CLK_CFG_5_CLR, 0xFFFFFFFF);
    //DRV_WriteReg32(CLK_CFG_5_SET, 0x01010101);
    DRV_WriteReg32(CLK_CFG_5_SET, 0x01010100);//uart[0]:26M
    
    DRV_WriteReg32(CLK_CFG_6_CLR, 0xFFFFFFFF);
    //DRV_WriteReg32(CLK_CFG_6_SET, 0x00010101);
    //DRV_WriteReg32(CLK_CFG_6_SET, 0x02010004);
    DRV_WriteReg32(CLK_CFG_6_SET, 0x00010004);
    
    DRV_WriteReg32(CLK_CFG_7_CLR, 0xFFFFFFFF);
    //DRV_WriteReg32(CLK_CFG_7_SET, 0x01010401);
    DRV_WriteReg32(CLK_CFG_7_SET, 0x00010201);//sspm_sel[10:8]:273M
    
    DRV_WriteReg32(CLK_CFG_8_CLR, 0xFFFFFFFF);
    //DRV_WriteReg32(CLK_CFG_8_SET, 0x02030302);
    DRV_WriteReg32(CLK_CFG_8_SET, 0x02030300);//disp_pwm_sel[2:0]: 26M
    
    DRV_WriteReg32(CLK_CFG_9_CLR, 0xFFFFFFFF);
    //DRV_WriteReg32(CLK_CFG_9_SET, 0x06070702);
    DRV_WriteReg32(CLK_CFG_9_SET, 0x03030302);//seninf: 312M

    DRV_WriteReg32(CLK_CFG_10_CLR, 0xFFFFFFFF);
    //DRV_WriteReg32(CLK_CFG_10_SET, 0x01030301);
    DRV_WriteReg32(CLK_CFG_10_SET, 0x05030301);//aes: 416M

    DRV_WriteReg32(CLK_CFG_11_CLR, 0xFFFFFFFF);
    //DRV_WriteReg32(CLK_CFG_11_SET, 0x04010101);
    DRV_WriteReg32(CLK_CFG_11_SET, 0x06010101);//adsp: 466M
    
    DRV_WriteReg32(CLK_CFG_12_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_12_SET, 0x02090b02);
    
    DRV_WriteReg32(CLK_CFG_13_CLR, 0xFFFFFFFF);
    //DRV_WriteReg32(CLK_CFG_13_SET, 0x02020301);
    DRV_WriteReg32(CLK_CFG_13_SET, 0x01000300);//camtg5[16:24]: 24M, pwm[0]:26M
   
    DRV_WriteReg32(CLK_CFG_UPDATE, 0x7FFFFFFE);//update all clocks except "axi"
    DRV_WriteReg32(CLK_CFG_UPDATE1, 0x81FFFFFF);
   
#if 1
    temp = DRV_Reg32(CLK_MISC_CFG_0);   // FIX BY HK
    //DRV_WriteReg32(CLK_MISC_CFG_0, ( temp | 0x0003000)); // FIX BY HK enable ufs_mp_sap_cfg_ck, ufs_tick1us_ck
    DRV_WriteReg32(CLK_MISC_CFG_0, ( temp | 0x0004000)); // enable [14] dramc_pll104m_ck
#endif
#if 0
    for(temp = 1; temp < 64; temp++)
    {
        mt_get_ckgen_freq(temp);
    }
  
    for(temp = 1; temp < 64; temp++)
    {
        mt_get_abist_freq(temp);
    }
#endif
    /************
    	* MTCMOS 
    	*************/
#if 1
    /*for CG*/
    DRV_WriteReg32(INFRA_PDN_CLR0, 0xDBEFFF7F);//7, 20, 26, 29
    DRV_WriteReg32(INFRA_PDN_CLR1, 0xDF9F7FF7);//3, 15, 21, 22, 29
    DRV_WriteReg32(INFRA_PDN_SET1, 0x18000000);//27, 28: dxcc
    DRV_WriteReg32(INFRA_PDN_CLR2, 0xFFFFFFDF);//5
    DRV_WriteReg32(INFRA_PDN_CLR3, 0xFDFF01FF);//9~15, 25
    print("Pll init Done!!\n");
#endif

#if 1
    print("mtcmos Start..\n");
    spm_mtcmos_ctrl_dis(STA_POWER_ON);
    print("mtcmos Done!\n");
    /*DISP CG Clear and DCM disable*/
    //DRV_WriteReg32(DISP_CG_CLR0, 0xFFFFFFFF);
    //DRV_WriteReg32(DISP_CG_CLR1, 0x0001FFFF);
#endif
    /* HW Latch INFRA */
    //DRV_WriteReg32(TOPRGU_LATCH, 0x95020001);
    //DRV_WriteReg32(DEBUG_LATCH, 0x00000000);

#if 1
    /* MT6779: infra_ao fmem DCMs. Need to be applied before DRAM init. */
    print("Infra_ao_mem DCM: apply aggresive settings(180913).\n");

    temp = DRV_Reg32(MEM_DCM_CTRL);
    DRV_WriteReg32(MEM_DCM_CTRL, (temp & ~(1 << 27)));		// step0

    temp = DRV_Reg32(INFRA_EMI_DCM_CFG0);
    DRV_WriteReg32(INFRA_EMI_DCM_CFG0, temp | 0x0000147E);	// step1,2 (bit 1,2,3,4,5,6,10,12)

    DRV_WriteReg32(INFRA_EMI_DCM_CFG1, 0x4078C000 /* 0x40788000 */);	// step3(from DE)
    DRV_WriteReg32(INFRA_EMI_DCM_TOP_CK_ANCHOR_CFG, 0x7);	// step4(from DE)
    DRV_WriteReg32(INFRA_EMI_DCM_CFG3, 0xFF);			// step5(from CODA/autogen)
								// (INFRA_EMI_DCM_CFG3, INFRACFG_AO_MEM_DCM_EMI_GROUP_REG2_ON)
#endif

}


