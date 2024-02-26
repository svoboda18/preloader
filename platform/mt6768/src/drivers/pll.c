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
/* #include "platform.h" */

#include "pll.h"
#include "sec_devinfo.h"
#include "spm_mtcmos.h"

/****
* colck management internal config setting
****/

/* #define SET_ARMPLL_DIV_EN */
#define SET_ARMPLL_CLK_SRC
/* #define FREQ_MTR_CHECK_CK */ /* old school */
/* #define _FREQ_SCAN_ */
/* #define _RESULT_COMPARE_ */

unsigned int mt_get_abist_freq(unsigned int ID)
{
	int output = 0, i = 0;
	unsigned int temp, clk26cali_0, clk_dbg_cfg, clk_misc_cfg_0, clk26cali_1;

	clk_dbg_cfg = DRV_Reg32(CLK_DBG_CFG);
	DRV_WriteReg32(CLK_DBG_CFG, (clk_dbg_cfg & 0xFFC0FFFC)|(ID << 16)); //sel abist_cksw and enable freq meter sel abist
	clk_misc_cfg_0 = DRV_Reg32(CLK_MISC_CFG_0);
	DRV_WriteReg32(CLK_MISC_CFG_0, (clk_misc_cfg_0 & 0x00FFFFFF) | (0x3 << 24)); // select divider, WAIT CONFIRM
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
	return output * 4;
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

#define Range 1000

const int ckgen_needCheck_array[] =
{
/* 01 - 05*/	1, 0, 1, 1, 1,
/* 06 - 10*/	1, 1, 1, 1, 1,
/* 11 - 15*/	1, 1, 1, 1, 1,
/* 16 - 20*/	1, 1, 1, 1, 1,
/* 21 - 25*/	1, 1, 1, 1, 1,
/* 26 - 30*/	1, 1, 1, 1, 1,
/* 31 - 35*/	1, 1, 0, 0, 0,
/* 36 - 40*/	0, 0, 0, 0, 0,
/* 41 - 45*/	0, 0, 0, 0, 0,
/* 46 - 50*/	0, 0, 0, 0, 0,
/* 51 - 55*/	0, 0, 0, 0, 0,
/* 56 - 58*/	0, 0, 0,
};

const int ckgen_golden_array[] =
{
/*add if need*/
/* 01 - 05*/	156000, 466000, 273000, 26000, 535000,
/* 06 - 10*/	273000, 24000, 24000, 24000, 24000,
/* 11 - 15*/	26000, 109200, 273000, 384000, 192000,
/* 16 - 20*/	26000, 136500, 196608, 24576, 26000,
/* 21 - 25*/	273000, 136500, 62400, 68250, 124800,
/* 26 - 30*/	68250, 208000, 384000, 26000, 208000,
/* 31 - 35*/	273000, 273000, 0, 0, 0,
/* 36 - 40*/	0, 0, 0, 0, 0,
/* 41 - 45*/	0, 0, 0, 0, 0,
/* 46 - 50*/	0, 0, 0, 0, 0,
/* 51 - 55*/	0, 0, 0, 0, 0,
};

#if 0 //remove to save space of log string
const char *ckgen_array[] =
{
/*
01:hd_faxi_ck,
02:hf_fmem_ck,
03:hf_fmm_ck,			//312
04:hf_fscp_ck,
05:hf_fmfg_ck,			//350 --> 460
06:hf_fatb_ck,
07:f_fcamtg_ck,
08:f_fcamtg1_ck,
09:f_fcamtg2_ck,
10:f_fcamtg3_ck,
11:f_fuart_ck,
12:hf_fspi_ck,
13:hf_fmsdc50_0_hclk_ck,
14:hf_fmsdc50_0_ck,		//400 --> 416 --> 384
15:hf_fmsdc30_1_ck,
16:hf_faudio_ck,		//54.6 --> 26
17:hf_faud_intbus_ck,
18:hf_faud_1_ck,		//192.608 --> 196.608
19:hf_faud_engen1_ck,
20:f_fdisp_pwm_ck,		//need calcibration to ULPOSC1 firstly ?
21:hf_fsspm_ck,
22:hf_fdxcc_ck,
23:f_fusb_top_ck,
24:hf_fspm_ck,
25:hf_fi2c_ck,
26:f_fpwm_ck,
27:f_fseninf_ck,
28:hf_faes_fde_ck,			//400 --> 416 --> 384
29:f_fpwrap_ulposc_ck,		//need calcibration to ULPOSC1 firstly ?
30:f_fcamtm_ck,
31,
32,
33,
34,
35,
36,
37,
38,
39,
40,
41,
42,
43,
44,
45,
46,
47,
48:f_ufs_mp_sap_cfg_ck,
49:f_ufs_tick1us_ck,
50:hd_faxi_east_ck,				--> X
51:hd_faxi_west_ck,				--> X
52:hd_faxi_north_ck,			--> X
53:hd_faxi_south_ck,			--> X
54:hg_fmipicfg_tx_ck,			--> X
55:fmem_ck_bfe_dcm_ch0,			--> X
56:fmem_ck_aft_dcm_ch0,			--> X
57:fmem_ck_bfe_dcm_ch1			--> X
58:fmem_ck_aft_dcm_ch1			--> X
};
*/
};
#endif //remove to save space of log string

#if 1
const int abist_needCheck_array[] =
{
/* 01 - 05*/	0, 0, 0, 0, 0,
/* 06 - 10*/	0, 0, 1, 1, 1,
/* 11 - 15*/	1, 1, 1, 1, 1,
/* 16 - 20*/	1, 0, 0, 0, 0,
/* 21 - 25*/	1, 1, 1, 1, 1,
/* 26 - 30*/	1, 1, 1, 0, 0,
/* 31 - 35*/	1, 0, 0, 0, 0,
/* 36 - 40*/	0, 0, 0, 0, 0,
/* 41 - 45*/	0, 0, 0, 0, 0,
/* 46 - 50*/	0, 0, 0, 1, 0,
/* 51 - 55*/	0, 0, 0, 0, 1,
/* 56 - 57*/	0, 0,
};

/* if needCheck_array[x]=1 but golden_array[x]=0: means we only want to read the value*/
const int ckgen_abist_golden_array[] =
{
/* 01 - 05*/	0, 0, 0, 0, 0,
/* 06 - 10*/	0, 0, 546000, 364000, 218400,
/* 11 - 15*/	156000, 624000, 416000, 249600, 178285,
/* 16 - 20*/	26000, 0, 0, 0, 0,
/* 21 - 25*/	1175000 /* ARMPLL-L, LittleCore*/ , 1087000 /*ARMPLL, BigCore*/, 1092000, 1248000, 535000 /*GPUPLL*/,
/* 26 - 30*/	384000  /* MSDCPLL*/, 457000 /*MMPLL*/,   196608, 0, 0,
/* 31 - 35*/	192000, 0, 0, 0, 0,
/* 36 - 40*/	0, 0, 0, 0, 0,
/* 41 - 45*/	0, 0, 0, 0, 0,
/* 46 - 50*/	0, 0, 0, 822000 /* CCIPLL */, 208000,
/* 51 - 55*/	0, 0, 0, 0, 48000,
/* 56 - 57*/	104000, 52000,
};

#if 0 //remove to save space of log string
const char *abist_array[] =
{
/*
1	AD_CSI0_DELAY_TSTCLK,
2	AD_CSI1_DELAY_TSTCLK,
3	UFS_MP_CLK2FREQ,
4	AD_MDBPIPLL_DIV3_CK,
5	AD_MDBPIPLL_DIV7_CK,
6	AD_MDBRPPLL_DIV6_CK,
7	AD_UNIV_624M_CK,
8	AD_MAIN_H546_CK,
9	AD_MAIN_H364M_CK,
10	AD_MAIN_H218P4M_CK,
11	AD_MAIN_H156M_CK,
12	AD_UNIV_624M_CK,
13	AD_UNIV_416M_CK,
14	AD_UNIV_249P6M_CK,
15	AD_UNIV_178P3M_CK,
16	AD_MDPLL_FS26M_CK,
17	AD_CSI1A_DPHY_DELAYCAL_CK,
18	AD_CSI1B_DPHY_DELAYCAL_CK,
19	AD_CSI2A_DPHY_DELAYCAL_CK,
20	AD_CSI2B_DPHY_DELAYCAL_CK,
21	AD_ARMPLL_L_CK,
22	AD_ARMPLL_CK,
23	AD_MAINPLL_1092M_CK,
24	AD_UNIVPLL_1248M_CK,
25	AD_MFGPLL_CK,					//350 --> 460
26	AD_MSDCPLL_384M_CK,				//416 --> 384
27	AD_MMPLL_CK,					//457
28	AD_APLL1_196P608M_CK,
29,
30	AD_APPLLGP_TST_CK,
31	AD_USB20_192M_CK,				//should check
32,
33,
34	AD_VENCPLL_CK,
35	AD_DSI0_MPPLL_TST_CK,
36	AD_DSI0_LNTC_DSICLK,
37	AD_ULPOSC1_CK,
38	AD_ULPOSC2_CK,
39	rtc32k_ck_i,
40	mcusys_arm_clk_out_all,
41	AD_ULPOSC1_SYNC_CK,
42	AD_ULPOSC2_SYNC_CK,
43	msdc01_in_ck,
44	msdc02_in_ck,
45	msdc11_in_ck,
46	msdc12_in_ck,
47,
48,
49	AD_CCIPLL_CK,
50	AD_MPLL_208M_CK,
51	AD_WBG_DIG_CK_416M,
52	AD_WBG_B_DIG_CK_64M,
53	AD_WBG_W_DIG_CK_160M,
54	DA_USB20_48M_DIV_CK,
55	DA_UNIV_48M_DIV_CK,
56	DA_MPLL_104M_DIV_CK,
57	DA_MPLL_52M_DIV_CK,
58	DA_ARMCPU_MON_CK,
59,
60	ckmon1_ck,
61	ckmon2_ck,
62	ckmon3_ck,
63	ckmon4_ck,
*/
};
#endif //remove to save space of log string
#endif

#define CKGEN_CHANNEL_CNT 58
#define ABIST_CHANNEL_CNT 57
unsigned int ret_feq_store[CKGEN_CHANNEL_CNT+ABIST_CHANNEL_CNT];
unsigned int ret_feq_total=0;
unsigned int pll_chk_is_fail = 0;

//after pmic_init
void mt_pll_post_init(void)
{

	unsigned int temp,clk_misc_cfg_0,ret_feq,reg_temp;
	unsigned int isFail = 0;

	/* need dram porting code */
	/* mempll_init_main(); */
	#ifdef _FREQ_SCAN_
	print("Pll post init start...\n");

	print("==============================\n");
	print("==      Parsing Start       ==\n");
	print("==============================\n");
	for(temp=1; temp<=CKGEN_CHANNEL_CNT; temp++)
	{
		if(!ckgen_needCheck_array[temp-1])
			continue;
		else
			print("%d:",temp);
		ret_feq = 0;
		ret_feq = mt_get_ckgen_freq(temp);
		ret_feq_store[ret_feq_total] = ret_feq;
		ret_feq_total++;
		/* print("%s:", ckgen_array[temp-1]); */
		print("%d\n",ret_feq);

	}

	//abist
	for(temp=1; temp<=ABIST_CHANNEL_CNT; temp++)
	{
		if(!abist_needCheck_array[temp-1])
			continue;
		else
			print("%d:",temp);
		ret_feq = mt_get_abist_freq(temp);
		ret_feq_store[ret_feq_total] = ret_feq;
		ret_feq_total++;

		/* print("%s:", ckgen_abist_array[temp-1]); */

		print("%d\n", ret_feq);
	}

#if 0
	// add debug log here
	// for example: print("mt_pll_post_init: AP_PLL_CON3        = 0x%x, GS = 0xFFFCCCCC\n", DRV_Reg32(AP_PLL_CON3));
#endif
	print("Pll post init Done!\n");
#endif // _FREQ_SCAN_

}

void mt_print_pll_check_result(void)
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
	DRV_WriteReg32(CLK_CFG_UPDATE, 0xffffffff);
}

void mt_disable_mmpll(void)
{
	unsigned int temp;
	//step 1
	temp = DRV_Reg32(MMPLL_CON0);
	DRV_WriteReg32(MMPLL_CON0, temp & 0xFFFFFFFE); // [0]=0 (MMPLL_EN)
	//step 2
	temp = DRV_Reg32(MMPLL_CON3);
	DRV_WriteReg32(MMPLL_CON3, temp | 0x2); // [1]=1 (MMPLL_ISO_EN)
	//step 3
	temp = DRV_Reg32(MMPLL_CON3);
	DRV_WriteReg32(MMPLL_CON3, temp & 0xFFFFFFFE); // [0]=0 (MMPLL_PWR_ON)
}

void mt_disable_apll1(void)
{
	unsigned int temp;
	//step 1
	temp = DRV_Reg32(APLL1_CON0);
	DRV_WriteReg32(APLL1_CON0, temp & 0xFFFFFFFE); // [0]=0 (APLL1_EN)
	//step 2
	temp = DRV_Reg32(APLL1_CON4);
	DRV_WriteReg32(APLL1_CON4, temp | 0x2); // [1]=1 (APLL1_ISO_EN)
	//step 3
	temp = DRV_Reg32(APLL1_CON4);
	DRV_WriteReg32(APLL1_CON4, temp & 0xFFFFFFFE); // [0]=0 (APLL1_PWR_ON)
}

void mt_pll_init(void)
{
	int ret = 0;
	unsigned int temp;
	unsigned int temp1;
	int i;

	print("Pll init start...\n");

	//step 1
	temp = DRV_Reg32(AP_PLL_CON0);
	DRV_WriteReg32(AP_PLL_CON0, temp | 0x01);// [0]=1 (CLKSQ_EN)

	//step 2
	gpt_busy_wait_us(100);

	//step 3
	temp = DRV_Reg32(AP_PLL_CON0);
	DRV_WriteReg32(AP_PLL_CON0, temp | 0x2); // [1]=1 (CLKSQ_LPF_EN)
	/*************
	* xPLL PWR ON
	**************/
	//step 4
	temp = DRV_Reg32(ARMPLL_CON3);
	DRV_WriteReg32(ARMPLL_CON3, temp | 0x1); // [0]=1 (ARMPLL_PWR_ON)

	//step 5
	temp = DRV_Reg32(ARMPLL_L_CON3);
	DRV_WriteReg32(ARMPLL_L_CON3, temp | 0x1); // [0]=1 (ARMPLL_LL_PWR_ON)

	//step 6
	temp = DRV_Reg32(CCIPLL_CON3);
	DRV_WriteReg32(CCIPLL_CON3, temp | 0x1); // [0]=1 (CCIPLL_PWR_ON)

	//step 7
	temp = DRV_Reg32(MFGPLL_CON3);
	DRV_WriteReg32(MFGPLL_CON3, temp | 0x1); // [0]=1 (GPUPLL_PWR_ON)

	//step 8
	/* remove mpll operation */
	//temp = DRV_Reg32(MPLL_CON3);
	//DRV_WriteReg32(MPLL_CON3, temp | 0x1); // [0]=1 (MPLL_PWR_ON)

	//step 9
	temp = DRV_Reg32(MAINPLL_CON3);
	DRV_WriteReg32(MAINPLL_CON3, temp | 0x1); // [0]=1 (MAINPLL_PWR_ON)

	//step 10
	temp = DRV_Reg32(UNIVPLL_CON3);
	DRV_WriteReg32(UNIVPLL_CON3, temp | 0x1); // [0]=1 (UNIVPLL_PWR_ON)

	//step 11
	temp = DRV_Reg32(MSDCPLL_CON3);
	DRV_WriteReg32(MSDCPLL_CON3, temp | 0x1); // [0]=1 (MSDCPLL_PWR_ON)

	//step 12
	temp = DRV_Reg32(MMPLL_CON3);
	DRV_WriteReg32(MMPLL_CON3, temp | 0x1); // [0]=1 (MMPLL_PWR_ON)

	//step 13
	temp = DRV_Reg32(APLL1_CON4);
	DRV_WriteReg32(APLL1_CON4, temp | 0x1); // [0]=1 (APLL1_PWR_ON)

	//step 14
	gpt_busy_wait_us(30); // Wait 30us
	/******************
	* xPLL ISO Eisable
	*******************/
	//step 15
	temp = DRV_Reg32(ARMPLL_CON3);
	DRV_WriteReg32(ARMPLL_CON3, temp & 0xFFFFFFFD); // [1]=0 (ARMPLL_ISO_EN)

	//step 16
	temp = DRV_Reg32(ARMPLL_L_CON3);
	DRV_WriteReg32(ARMPLL_L_CON3, temp & 0xFFFFFFFD); // [1]=0 (ARMPLL_L_ISO_EN)

	//step 17
	temp = DRV_Reg32(CCIPLL_CON3);
	DRV_WriteReg32(CCIPLL_CON3, temp & 0xFFFFFFFD); // [1]=0 (CCIPLL_ISO_EN)

	//step 18
	temp = DRV_Reg32(MFGPLL_CON3);
	DRV_WriteReg32(MFGPLL_CON3, temp & 0xFFFFFFFD); // [1]=0 (MFGPLL_ISO_EN)

	//step 19
	/* remove mpll operation */
	//temp = DRV_Reg32(MPLL_CON3);
	//DRV_WriteReg32(MPLL_CON3, temp & 0xFFFFFFFD); // [1]=0 (MPLL_ISO_EN)

	//step 20
	temp = DRV_Reg32(MAINPLL_CON3);
	DRV_WriteReg32(MAINPLL_CON3, temp & 0xFFFFFFFD); // [1]=0 (MAINPLL_ISO_EN)

	//step 21
	temp = DRV_Reg32(UNIVPLL_CON3);
	DRV_WriteReg32(UNIVPLL_CON3, temp & 0xFFFFFFFD); // [1]=0 (UNIVPLL_ISO_EN)

	//step 22
	temp = DRV_Reg32(MSDCPLL_CON3);
	DRV_WriteReg32(MSDCPLL_CON3, temp & 0xFFFFFFFD); // [1]=0 (MSDCPLL_ISO_EN)

	//step 23
	temp = DRV_Reg32(MMPLL_CON3);
	DRV_WriteReg32(MMPLL_CON3, temp & 0xFFFFFFFD); // [1]=0 (MMPLL_ISO_EN)

	//step 24
	temp = DRV_Reg32(APLL1_CON4);
	DRV_WriteReg32(APLL1_CON4, temp & 0xFFFFFFFD); // [1]=0 (APLL1_ISO_EN)

	//step 25
	gpt_busy_wait_us(1); // Wait 1us

	/********************
	* xPLL Frequency Set
	*********************/
	//step 26
	//DRV_WriteReg32(ARMPLL_CON1, 0x81152762); // [25:0] (Big Core, ARMPLL_N_INFO, 1100 MHz)
	DRV_WriteReg32(ARMPLL_CON1, 0x8114E762); // [25:0] (Big Core, ARMPLL_N_INFO, 1087 MHz)

	//step 27
	//DRV_WriteReg32(ARMPLL_L_CON1, 0x811713B1); // [25:0] (Little Core, ARMPLL_LL_N_INFO, 1200 MHz)
	DRV_WriteReg32(ARMPLL_L_CON1, 0x8116989D); // [25:0] (Little Core, ARMPLL_LL_N_INFO, 1175 MHz)

	//step 28
	//DRV_WriteReg32(CCIPLL_CON1, 0x811C589D); // [25:0] (CCIPLL_N_INFO, 1474 MHz, div2=737 Mhz finally)
	DRV_WriteReg32(CCIPLL_CON1, 0x821F9D89); // [25:0] (CCIPLL_N_INFO, 822 MHz)

	//step 29
	//DRV_WriteReg32(MFGPLL_CON1, 0x840F6276); // [21:0] (GPUPLL_N_INFO, 100MHz)
	DRV_WriteReg32(MFGPLL_CON1, 0x821493B1); // [21:0] (GPUPLL_N_INFO, 535Mhz)

	//step 30
	/* remove mpll operation */
	//DRV_WriteReg32(MPLL_CON1, 0x83100000); // [21:0] (MPLL_N_INFO, 208MHz)

	//step 31
	DRV_WriteReg32(MAINPLL_CON1, 0x81150000); // [21:0] (MAINPLL_N_INFO, 1092MHz)

	//step 32
	DRV_WriteReg32(UNIVPLL_CON1, 0x80180000); // [21:0] (UNIVPLL_N_INFO, 1248MHz)

	//step 33
	DRV_WriteReg32(MSDCPLL_CON1, 0x820EC4EC); // [21:0] (MSDCPLL_N_INFO, 384MHz)

	//step 34
	DRV_WriteReg32(MMPLL_CON1, 0x821193B1); // [21:0] (MMPLL_N_INFO, 457MHz)

	//step 35
	DRV_WriteReg32(APLL1_CON2, 0x78FD5265); // [23:14] = MSB of APLL1_N_INFO, 196.608MHz
	DRV_WriteReg32(APLL1_CON1, 0x84000000); // [21:0] = LSB of APLL1_N_INFO, 196.608MHz
	//DRV_WriteReg32(APLL1_TUNER_CON0, 0x3C7EA933); // [23:14] = MSB of APLL1_N_INFO, 196.608MHz

	/***********************
	* xPLL Frequency Enable
	************************/
	//step 36
	temp = DRV_Reg32(ARMPLL_CON0);
	DRV_WriteReg32(ARMPLL_CON0, temp | 0x1); // [0]=1 (ARMPLL_EN)

	//step 37
	temp = DRV_Reg32(ARMPLL_L_CON0);
	DRV_WriteReg32(ARMPLL_L_CON0, temp | 0x1); // [0]=1 (ARMPLL_L_EN)

	//step 38
	temp = DRV_Reg32(CCIPLL_CON0);
	DRV_WriteReg32(CCIPLL_CON0, temp | 0x1); // [0]=1 (CCIPLL_EN)

	//step 39
	temp = DRV_Reg32(MFGPLL_CON0);
	DRV_WriteReg32(MFGPLL_CON0, temp | 0x1); // [0]=1 (GPUPLL_EN)

	//step 40
	/* remove mpll operation */
	//temp = DRV_Reg32(MPLL_CON0);
	//DRV_WriteReg32(MPLL_CON0, temp | 0x1); // [0]=1 (MPLL_EN)

	//step 41
	temp = DRV_Reg32(MAINPLL_CON0);
	DRV_WriteReg32(MAINPLL_CON0, temp | 0x1); // [0]=1 (MAINPLL_EN)

	//step 42
	temp = DRV_Reg32(UNIVPLL_CON0);
	DRV_WriteReg32(UNIVPLL_CON0, temp | 0x1); // [0]=1 (UNIVPLL_EN)

	//step 43
	temp = DRV_Reg32(MSDCPLL_CON0);
	DRV_WriteReg32(MSDCPLL_CON0, temp | 0x1); // [0]=1 (MSDCPLL_EN)

	//step 44
	temp = DRV_Reg32(MMPLL_CON0);
	DRV_WriteReg32(MMPLL_CON0, temp | 0x1); // [0]=1 (MMPLL_EN)

	//step 45
	temp = DRV_Reg32(APLL1_CON0);
	DRV_WriteReg32(APLL1_CON0, temp | 0x1); // [0]=1 (APLL1_EN)

	/***************
	* xPLL DIV Enable
	****************/
	//step 46
	temp = DRV_Reg32(MAINPLL_CON0);
	DRV_WriteReg32(MAINPLL_CON0, temp | 0xFF000000); // [31:24] (MAINPLL_DIV_EN)

	//step 47
	temp = DRV_Reg32(UNIVPLL_CON0);
	DRV_WriteReg32(UNIVPLL_CON0, temp | 0xFF000000); // [31:24] (UNIVPLL_DIV_EN)

	// step 48
	gpt_busy_wait_us(20); // Wait PLL stable (20us)

	/***************
	* xPLL DIV RSTB
	****************/
	//step 49
	temp = DRV_Reg32(MAINPLL_CON0);
	DRV_WriteReg32(MAINPLL_CON0, temp | 0x00800000); // [23]=1 (MAINPLL_DIV_RSTB)

	//step 50
	temp = DRV_Reg32(UNIVPLL_CON0);
	DRV_WriteReg32(UNIVPLL_CON0, temp | 0x00800000); // [23]=1 (UNIVPLL_DIV_RSTB)

	// step 51
	gpt_busy_wait_us(20); // Wait PLL stable (20us)

	/*****************
	* xPLL HW Control
	******************/
	//step 52
	temp = DRV_Reg32(PLLON_CON1);
	DRV_WriteReg32(PLLON_CON1, temp & 0xffbfffff); // [22]=0  use HW delay mode (by_maindiv_dly)

	/**************
    	* INFRA CPU CLKMUX, CLK Div
    	***************/
    	/* TODO: need to ask for infra mux setting? 20171122 */
#ifdef SET_ARMPLL_DIV_EN
	/* 8: div1, A: div2, B: div4, 1D: div6 */
	temp = DRV_Reg32(CPU_PLLDIV_CFG0) ;				// LCores.
	DRV_WriteReg32(CPU_PLLDIV_CFG0, (temp & 0xFFC1FFFF) | (0x08 << 17)); // [21:17] divsel: CPU clock divide by 1

	temp = DRV_Reg32(CPU_PLLDIV_CFG1) ;				// BCores
	DRV_WriteReg32(CPU_PLLDIV_CFG1, (temp & 0xFFC1FFFF) | (0x08 << 17)); // [21:17] divsel: CPU clock divide by 1

	temp = DRV_Reg32(BUS_PLLDIV_CFG) ;				//CCI(DSU)
	DRV_WriteReg32(BUS_PLLDIV_CFG, (temp & 0xFFC1FFFF) | (0x08 << 17)); // [21:17] divsel: CPU clock divide by 1
#endif
#ifdef SET_ARMPLL_CLK_SRC
	/* 0: 26M,  1: armpll, 2:  Mainpll, 3:  Unipll */
	temp = DRV_Reg32(CPU_PLLDIV_CFG0);
	DRV_WriteReg32(CPU_PLLDIV_CFG0, (temp & 0xFFFFF9FF) | (0x01<<9)); // [10:9] muxsel: switch to PLL speed

	temp = DRV_Reg32(CPU_PLLDIV_CFG1);
	DRV_WriteReg32(CPU_PLLDIV_CFG1, (temp & 0xFFFFF9FF) | (0x01<<9)); // [10:9] muxsel: switch to PLL speed

	temp = DRV_Reg32(BUS_PLLDIV_CFG);
	DRV_WriteReg32(BUS_PLLDIV_CFG, (temp & 0xFFFFF9FF) | (0x01<<9)); // [10:9] muxsel: switch to PLL speed
#endif
	/************
  	  * TOP CLKMUX
  	  *************/

	//need to confirm with dcm owner
	// [21] rg_axi_dcm_dis_en
	// [22] rg_pllck_sel_no_spm
	// set this register before AXI clock switch to fast clock (26MHz => 156MHz)
	DRV_WriteReg32(INFRA_BUS_DCM_CTRL, DRV_Reg32(INFRA_BUS_DCM_CTRL) | (1 << 21) | (1 << 22));//spm axi dcm enable

	//DRV_WriteReg32(PWR_RST_CTL, 0x00000001);//disable big cpu protection


	// config AXI clock first
	DRV_WriteReg32(CLK_CFG_0, 0x00000001);      // axi=syspll_d7
	DRV_WriteReg32(CLK_CFG_UPDATE, 0x00000001); //update AXI clock

	/* config other clocks */
	/* scp_sel=top_ap_clk_ctrl_f26m_ck(0), mm_sel=syspll1_d2(2), ddrphycfg_ck=don't touch,  mem_sel=clk26m(0),axi=syspll1_d7(1) */
	DRV_WriteReg32(CLK_CFG_0, 0x00020001);
	/* camtg1_sel=usb20_192m_d8(1), camtg_sel = usb20_192m_d8(1), atb_sel=syspll1_d2(2),mfg_sel=mfgpll_ck(1)*/
	DRV_WriteReg32(CLK_CFG_1, 0x01010201);
	/* spi_sel=syspll3_d2(1),uart_sel=top_ap_clk_ctrl_f26m_ck(0),camtg3_sel=usb20_192m_d8(1),camtg2_sel=usb20_192m_d8(1) */
	DRV_WriteReg32(CLK_CFG_2, 0x01000101);
	/* audio_sel=top_ap_clk_ctrl_f26m_ck(0),msdc30_1_sel=msdcpll_d2(1),msdc50_0_sel=msdcpll_ck(1),msdc50_0_hclk_sel=syspll1_d2(1) */
	DRV_WriteReg32(CLK_CFG_3, 0x00010101);

	/* disp_pwm_sel=clk26m(0),aud_engen1_sel=apll1_d8(3),aud_1_sel=ad_apll1_ck(1),aud_intbus_sel=syspll1_d4(1) */
	DRV_WriteReg32(CLK_CFG_4, 0x00030101);
	/* spm_sel=syspll1_d8(1),usb_top_sel=univpll3_d4(1),dxcc_sel=SYSPLL1_D4(2),sspm_sel=SYSPLL1_D2(1) */
	DRV_WriteReg32(CLK_CFG_5, 0x01010201);
	/* aes_fde_sel=msdcpll_ck(1),seninf_sel=univpll2_d2(3),pwm_sel=syspll1_d8(2),i2c_sel=univpll3_d2(2) */
	DRV_WriteReg32(CLK_CFG_6, 0x01030202);
	/* cam_sel=syspll_d2(1), venc_sel=syspll1_d2(2), camtm_sel=univpll2_d2(3),pwrap_ulposc_sel=clk26m(0) */
	DRV_WriteReg32(CLK_CFG_7, 0x01020300);

	DRV_WriteReg32(CLK_CFG_UPDATE, 0xFFFFFFFC);    //update all clocks except "fmem & axi"(dram)
	//DRV_WriteReg32(MODULE_CLK_SEL, DRV_Reg32(MODULE_CLK_SEL) | 0xF);

#ifdef FREQ_MTR_CHECK_CK
	for(temp = 1; temp < 59; temp++) {
    		if((temp > 32) && (temp < 48)) {
			continue; //1'b0
		} else{
			mt_get_ckgen_freq(temp);
		}
	}

	for(temp = 1; temp < 64; temp++) {
		if((temp==29)||
			(temp==32)||
			(temp==33)||
			(temp==47)||
			(temp==48)||
			(temp==59)) {
			continue; //1'b0
		} else if((temp==40)) {                  // Special case for mcusys_arm_clk_out_all
			/*"0x0 --> 1
			 *1~0xFE --> PLL freq/((1~0xFE) +1 )
			 *0xFF --> 0"
			 */
			temp1 = DRV_Reg32(CPU_PLLDIV_CFG0);
			DRV_WriteReg32(CPU_PLLDIV_CFG0, (temp1 & 0xFFFFFF00) | (0x01)); // [7:0] Meter input from MP0

			temp1 = DRV_Reg32(CPU_PLLDIV_CFG1);
			DRV_WriteReg32(CPU_PLLDIV_CFG1, (temp1 & 0xFFFFFF00) | (0xFF)); // [7:0]

			temp1 = DRV_Reg32(BUS_PLLDIV_CFG);
			DRV_WriteReg32(BUS_PLLDIV_CFG, (temp1 & 0xFFFFFF00) | (0xFF)); // [7:0]

			mt_get_abist_freq(temp);

			temp1 = DRV_Reg32(CPU_PLLDIV_CFG0);
			DRV_WriteReg32(CPU_PLLDIV_CFG0, (temp1 & 0xFFFFFF00) | (0xFF)); // [7:0]

			temp1 = DRV_Reg32(CPU_PLLDIV_CFG1);
			DRV_WriteReg32(CPU_PLLDIV_CFG1, (temp1 & 0xFFFFFF00) | (0x01)); // [7:0] Meter input from MP1

			temp1 = DRV_Reg32(BUS_PLLDIV_CFG);
			DRV_WriteReg32(BUS_PLLDIV_CFG, (temp1 & 0xFFFFFF00) | (0xFF)); // [7:0]

			mt_get_abist_freq(temp);

			temp1 = DRV_Reg32(CPU_PLLDIV_CFG0);
			DRV_WriteReg32(CPU_PLLDIV_CFG0, (temp1 & 0xFFFFFF00) | (0xFF)); // [7:0]

			temp1 = DRV_Reg32(CPU_PLLDIV_CFG1);
			DRV_WriteReg32(CPU_PLLDIV_CFG1, (temp1 & 0xFFFFFF00) | (0xFF)); // [7:0]

			temp1 = DRV_Reg32(BUS_PLLDIV_CFG);
			DRV_WriteReg32(BUS_PLLDIV_CFG, (temp1 & 0xFFFFFF00) | (0x01)); // [7:0]  Meter input from BUS

			mt_get_abist_freq(temp);
		} else {
			mt_get_abist_freq(temp);
		}
	}
#endif

	/* MTCMOS */


	/* for CGs */

	/* INFRA CG */
	DRV_WriteReg32(INFRA_TOPAXI_SI0_CTL, DRV_Reg32(INFRA_TOPAXI_SI0_CTL) | (0x80000000));
	DRV_WriteReg32(PERI_BUS_DCM_CTRL, DRV_Reg32(PERI_BUS_DCM_CTRL) & ~(0x00000004));

	DRV_WriteReg32(MODULE_SW_CG_0_CLR, 0x98ffff7f);
	/* disable trng[9], secure access after devapc in LK, [27][28] DXCC */
	DRV_WriteReg32(MODULE_SW_CG_1_CLR, 0x879C7D96);
	DRV_WriteReg32(MODULE_SW_CG_1_SET, 0x18000200);
	DRV_WriteReg32(MODULE_SW_CG_2_CLR, 0x2ffc87dd);
	DRV_WriteReg32(MODULE_SW_CG_3_CLR, 0x003F87DB);

#if 1
	print("mtcmos Start..\n");
	spm_mtcmos_ctrl_dis(STA_POWER_ON);
	print("mtcmos Done!\n");
	/* DISP CG */
	/* DRV_WriteReg32(MMSYS_CG_CLR0, 0xFFFFFFFF); */
#endif


#if 0
	/* disable mmpll/apll1 since no one need it */
	mt_disable_mmpll();
#endif

	mt_disable_apll1();

	print("Pll init Done!\n");
}


