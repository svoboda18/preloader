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
/*#include "spm_mtcmos.h"*/


#include "../security/inc/sec_devinfo.h"
extern void mempll_init_main(void);
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
    if (ID == 39) {
        DRV_WriteReg32(CLK26CALI_0, 0x100);
        DRV_WriteReg32(CLK26CALI_1, (clk26cali_1 & 0xFC00FFFF) | 0x400000);
        DRV_WriteReg32(CLK26CALI_0, 0x1100);
        DRV_WriteReg32(CLK26CALI_0, 0x1110);
    } else {
        DRV_WriteReg32(CLK26CALI_0, 0x1000);
        DRV_WriteReg32(CLK26CALI_0, 0x1010);
    }
    /* wait frequency meter finish */
    while (DRV_Reg32(CLK26CALI_0) & 0x10)
    {
        udelay(10);
        i++;
        if(i > 200)
            break;
    }
    temp = DRV_Reg32(CLK26CALI_1) & 0xFFFF;
    if (ID == 39)
        output = ((64 * 26000) / temp); // hz
    else
        output = ((temp * 26000) ) / 1024; // Khz

    DRV_WriteReg32(CLK_DBG_CFG, clk_dbg_cfg);
    DRV_WriteReg32(CLK_MISC_CFG_0, clk_misc_cfg_0);
    DRV_WriteReg32(CLK26CALI_0, clk26cali_0);
    DRV_WriteReg32(CLK26CALI_1, clk26cali_1);

    //print("abist meter[%d] = %d Khz\n", ID, output);
    if(i > 200)
        return 0;
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
        udelay(10);
		i++;
        if(i > 100)
        	break;
    }
    temp = DRV_Reg32(CLK26CALI_1) & 0xFFFF;

    output = ((temp * 26000) ) / 1024; // Khz

    DRV_WriteReg32(CLK_DBG_CFG, clk_dbg_cfg);
    DRV_WriteReg32(CLK_MISC_CFG_0, clk_misc_cfg_0);
    DRV_WriteReg32(CLK26CALI_0, clk26cali_0);
    DRV_WriteReg32(CLK26CALI_1, clk26cali_1);

    //print("ckgen meter[%d] = %d Khz\n", ID, output);
    if(i > 100)
        return 0;
    return output;

}


//#define _RESULT_COMPARE_

#define Range 1000

const int ckgen_needCheck_array[] =
{
/* 01 - 05*/	1, 1, 1, 0, 0,
/* 06 - 10*/	0, 1, 1, 1, 1,
/* 11 - 15*/	0, 1, 1, 1, 0,
/* 16 - 20*/	0, 1, 1, 0, 0,
/* 21 - 25*/	0, 0, 1, 1, 1,
/* 26 - 30*/	0, 1, 1, 1, 1,
/* 31 - 35*/	1, 0, 1, 0, 1,
/* 36 - 40*/	1, 1, 1, 0, 0,
/* 41 - 45*/	1, 0, 0, 0, 0,
/* 46 - 50*/	0, 1, 1, 1, 0,
/* 51 - 55*/	0, 0, 0, 1, 0,
/* 56 - 59*/	0, 0, 0, 0,
};

const char *ckgen_golden_array[] =
{
/*add if need*/
};

#if 0 //remove to save space of log string
const char *ckgen_array[] =
{
/*
01:hd_faxi_ck,
02:hf_fddrphycfg_ck,
03:hf_fmm_ck,
04,
05,
06,
07:hf_fmfg_ck,
08:f_fcamtg_ck,
09:f_fuart_ck,
10:hf_fspi_ck,
11,
12:hf_fmsdc50_0_hclk_ck,
13:hf_fmsdc50_0_ck,
14:hf_fmsdc30_1_ck,
15,
16,
17:hf_faudio_ck,
18:hf_faud_intbus_ck,
19,
20,
21,
22,
23:hf_fdbi0_ck,
24:hf_fscam_ck,
25:hf_faud_1_ck,
26,
27:f_fdisp_pwm_ck,
28:hf_fnfi2x_ck,
29:hf_fnfiecc_ck,
30:f_fusb_top_ck,
31:hg_fspm_ck,
32,
33:f_fi2c_ck,
34,
35:f_fseninf_ck,
36:f_fdxcc_ck,
37:hf_faud_engin1_ck,
38:hf_faud_engin2_ck,
39,
40,
41:f_fcamtg2_ck,
42,
43,
44,
45,
46,
47:hf_fnfi1x_ck,
48:f_ufs_mp_sap_cfg_ck,
49:f_ufs_tick1us_ck,
50:hd_faxi_east_ck,
51:hd_faxi_west_ck,
52:hd_faxi_north_ck,
53:hd_faxi_south_ck,
54:hg_fmipicfg_tx_ck,
55:fmem_ck_bfe_dcm_ch0,
56:fmem_ck_aft_dcm_ch0,
57,
58,
59:dramc_pll104m_ck,
};
*/
};
#endif //remove to save space of log string

#if 1
const int ckgen_abist_needCheck_array[] =
{
/* 01 - 05*/	0, 0, 0, 0, 0,
/* 06 - 10*/	0, 0, 0, 0, 0,
/* 11 - 15*/	0, 0, 0, 0, 0,
/* 16 - 20*/	1, 0, 0, 0, 0,
/* 21 - 25*/	0, 1, 1, 1, 0,
/* 26 - 30*/	1, 1, 1, 0, 0,
/* 31 - 35*/	1, 1, 0, 1, 0,
/* 36 - 40*/	0, 0, 0, 1, 0,
/* 41 - 45*/	0, 0, 0, 0, 0,
/* 46 - 50*/	0, 0, 0, 0, 0,
/* 51 - 55*/	0, 0, 0, 1, 1,
};

/* if needCheck_array[x]=1 but golden_array[x]=0: means we only want to read the value*/
const int ckgen_abist_golden_array[] =
{
/* 01 - 05*/	0, 0, 0, 0, 0,
/* 06 - 10*/	0, 0, 0, 0, 0,
/* 11 - 15*/	0, 0, 0, 0, 26000,
/* 16 - 20*/	0, 0, 0, 0, 0,
/* 21 - 25*/	0, 884000, 1092000, 1248000, 0,
/* 26 - 30*/	0, 0, 0, 0, 0,
/* 31 - 35*/	192000, 192000, 0, 300000, 0,
/* 36 - 40*/	0, 0, 0, 0, 0,
/* 41 - 45*/	0, 0, 0, 0, 0,
/* 46 - 50*/	0, 0, 0, 0, 208000,
/* 51 - 55*/	0, 0, 0, 48000, 48000,
};

#if 0 //remove to save space of log string
const char *abist_array[] =
{
/*
1	AD_CSI0_DELAY_TSTCLK,
2	AD_CSI1_DELAY_TSTCLK,
3,
4	AD_MDBPIPLL_DIV3_CK,
5	AD_MDBPIPLL_DIV7_CK,
6	AD_MDBRPPLL_DIV6_CK,
7	AD_UNIV_624M_CK,
8	AD_MAIN_H546_CK,
9	AD_MEMPLL_MONCLK,
10	AD_MEMPLL2_MONCLK,
11	AD_MEMPLL3_MONCLK,
12	AD_MEMPLL4_MONCLK,
13,
14,
15,
16	AD_MDPLL_FS26M_CK,
17,
18,
19,
20	AD_ARMPLL_L_CK,
21,
22	AD_ARMPLL_LL_CK,
23	AD_MAINPLL_CK,
24	AD_UNIVPLL_CK,
25,
26	AD_MSDCPLL_CK,
27	AD_MMPLL_CK,
28	AD_APLL1_CK,
29,
30	AD_APPLLGP_TST_CK,
31	AD_USB20_192M_CK,
32	AD_UNIV_192M_CK,
33,
34	AD_VENCPLL_CK,
35	AD_DSI0_MPPLL_TST_CK,
36	AD_DSI0_LNTC_DSICLK,
37,
38,
39	rtc32k_ck_i,
40	mcusys_arm_clk_out_all,
41,
42,
43	msdc01_in_ck,
44	msdc02_in_ck,
45	msdc11_in_ck,
46	msdc12_in_ck,
47,
48,
49,
50	AD_MPLL_208M_CK,
51,
52,
53,
54	DA_USB20_48M_DIV_CK,
55	DA_UNIV_48M_DIV_CK,
*/
};
#endif //remove to save space of log string
#endif

#define CKGEN_CHANNEL_CNT 59
#define ABIST_CHANNEL_CNT 55
unsigned int ret_feq_store[CKGEN_CHANNEL_CNT+ABIST_CHANNEL_CNT];
unsigned int ret_feq_total=0;
unsigned int pll_chk_is_fail = 0;

//after pmic_init
/*#define _FREQ_SCAN_*/
void mt_pll_post_init(void)
{

	unsigned int temp,clk_misc_cfg_0,ret_feq,reg_temp;
	unsigned int isFail = 0;

	mempll_init_main();
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
		if(!ckgen_abist_needCheck_array[temp-1])
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
		if(!ckgen_abist_needCheck_array[temp-1])
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

#if 0
//after pmic_init
void mt_arm_pll_sel(void)
{
	unsigned int temp;

	temp = DRV_Reg32(TOP_CKMUXSEL);
	//DRV_WriteReg32(TOP_CKMUXSEL, temp | 0x5);
	// switch CA7_ck to ARMCA7PLL, and CA15_ck to ARMCA15PLL
	DRV_WriteReg32(TOP_CKMUXSEL, temp | 0x1); // switch CA7_ck to ARMCA7PLL

	print("[PLL] mt_arm_pll_sel done\n");
}
#endif
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
	DRV_WriteReg32(CLK_CFG_UPDATE, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_UPDATE1, 0x000003FF);
}

void mt_pll_init(void)
{
	unsigned int temp;

	print("Pll init start...\n");
	//step 0
	DRV_WriteReg32(ACLKEN_DIV, 0x12); // MCU Bus DIV2

	//step 1
	temp = DRV_Reg32(AP_PLL_CON0);
	DRV_WriteReg32(AP_PLL_CON0, temp | 0x01);// [0]=1 (CLKSQ_EN)

	//step 2
	gpt_busy_wait_us(100);

	//step 3
	temp = DRV_Reg32(AP_PLL_CON0);
	DRV_WriteReg32(AP_PLL_CON0, temp | 0x2); //[1]=1 (CLKSQ_LPF_EN)

	/*************
	* xPLL PWR ON
	**************/
	//step 4
	temp = DRV_Reg32(MFGPLL_PWR_CON0);
	DRV_WriteReg32(MFGPLL_PWR_CON0, temp | 0x1);

	//step 5
	temp = DRV_Reg32(MAINPLL_PWR_CON0);
	DRV_WriteReg32(MAINPLL_PWR_CON0, temp | 0x1);

	//step 6
	temp = DRV_Reg32(UNIVPLL_PWR_CON0);
	DRV_WriteReg32(UNIVPLL_PWR_CON0, temp | 0x1);

	//step 7
	temp = DRV_Reg32(MSDCPLL_PWR_CON0);
	DRV_WriteReg32(MSDCPLL_PWR_CON0, temp | 0x1);

	//step 8
	temp = DRV_Reg32(MMPLL_PWR_CON0);
	DRV_WriteReg32(MMPLL_PWR_CON0, temp | 0x1);

	//step 9
	temp = DRV_Reg32(APLL1_PWR_CON0);
	DRV_WriteReg32(APLL1_PWR_CON0, temp | 0x1);

	//step 10
	temp = DRV_Reg32(ARMPLL_LL_PWR_CON0);
	DRV_WriteReg32(ARMPLL_LL_PWR_CON0, temp | 0x1);

	//step 11
	gpt_busy_wait_us(30);

	/******************
	* xPLL ISO Disable
	*******************/
	//step 21
	temp = DRV_Reg32(MFGPLL_PWR_CON0);
	DRV_WriteReg32(MFGPLL_PWR_CON0, temp & 0xFFFFFFFD);

	//step 22
	temp = DRV_Reg32(MAINPLL_PWR_CON0);
	DRV_WriteReg32(MAINPLL_PWR_CON0, temp & 0xFFFFFFFD);

	//step 23
	temp = DRV_Reg32(UNIVPLL_PWR_CON0);
	DRV_WriteReg32(UNIVPLL_PWR_CON0, temp & 0xFFFFFFFD);

	//step 24
	temp = DRV_Reg32(MSDCPLL_PWR_CON0);
	DRV_WriteReg32(MSDCPLL_PWR_CON0, temp & 0xFFFFFFFD);

	//step 25
	temp = DRV_Reg32(MMPLL_PWR_CON0);
	DRV_WriteReg32(MMPLL_PWR_CON0, temp & 0xFFFFFFFD);

	//step 26
	temp = DRV_Reg32(APLL1_PWR_CON0);
	DRV_WriteReg32(APLL1_PWR_CON0, temp & 0xFFFFFFFD);

	//step 28
	temp = DRV_Reg32(ARMPLL_LL_PWR_CON0);
	DRV_WriteReg32(ARMPLL_LL_PWR_CON0, temp & 0xFFFFFFFD);

	//step 29
	gpt_busy_wait_us(1);

	/********************
	* xPLL Frequency Set
	*********************/
	//step 30
	DRV_WriteReg32(MFGPLL_CON1, 0x8215EC4E);//	[21:0] (MFGPLL_N_INFO, 570MHz)

	//step 31
	DRV_WriteReg32(MAINPLL_CON1, 0x800A8000);	//	[21:0] (MAINPLL_N_INFO, 1092MHz)

	//step 32
	DRV_WriteReg32(UNIVPLL_CON1, 0x800C0000);	//	[21:0] (UNIVPLL_N_INFO, 1248MHz)

	//step 33
	DRV_WriteReg32(MSDCPLL_CON1, 0x820EC4EC);	//	[21:0] (MSDCPLL_N_INFO, 384MHz)

	//step 34
	DRV_WriteReg32(MMPLL_CON1, 0x831713B1);	//	[21:0] (MMPLL_N_INFO, 300MHz)

	//step 35
	DRV_WriteReg32(APLL1_CON1, 0x3C7EA932);	//	[31:0] =  APLL1_N_INFO, 336MHz

	//step 36
	DRV_WriteReg32(APLL1_CON0, 0x80000187);	//	[31:0] = APLL1_N_INFO, 336MHz

	//step 37
	DRV_WriteReg32(ARMPLL_LL_CON1, 0x811244EC);	//	[25:0] (ARMPLL_LL_N_INFO, 950MHz)


    /***********************
     * xPLL Frequency Enable
     ************************/
	//step 41
	temp = DRV_Reg32(MFGPLL_CON0);
	DRV_WriteReg32(MFGPLL_CON0, temp | 0x1);

	//step 42
	temp = DRV_Reg32(MAINPLL_CON0);
	DRV_WriteReg32(MAINPLL_CON0, temp | 0x1);

	//step 43
	temp = DRV_Reg32(UNIVPLL_CON0);
	DRV_WriteReg32(UNIVPLL_CON0, temp | 0x1);

	//step 44
	temp = DRV_Reg32(MSDCPLL_CON0);
	DRV_WriteReg32(MSDCPLL_CON0, temp | 0x1);

	//step 45
	temp = DRV_Reg32(MMPLL_CON0);
	DRV_WriteReg32(MMPLL_CON0, temp | 0x1);

	//step 46
	temp = DRV_Reg32(APLL1_CON0);
	DRV_WriteReg32(APLL1_CON0, temp | 0x1);

	//step 47
	temp = DRV_Reg32(ARMPLL_LL_CON0);
	DRV_WriteReg32(ARMPLL_LL_CON0, temp | 0x1);

	//step 51
	temp = DRV_Reg32(MFGPLL_CON0);
	DRV_WriteReg32(MFGPLL_CON0, temp | 0x100);

	//step 52
	temp = DRV_Reg32(MAINPLL_CON0);
	DRV_WriteReg32(MAINPLL_CON0, temp | 0x100);

	//step 53
	temp = DRV_Reg32(UNIVPLL_CON0);
	DRV_WriteReg32(UNIVPLL_CON0, temp | 0x100);

	//step 54
	temp = DRV_Reg32(MSDCPLL_CON0);
	DRV_WriteReg32(MSDCPLL_CON0, temp | 0x100);

	//step 55
	temp = DRV_Reg32(MMPLL_CON0);
	DRV_WriteReg32(MMPLL_CON0, temp | 0x100);

	//step 56
	temp = DRV_Reg32(APLL1_CON0);
	DRV_WriteReg32(APLL1_CON0, temp | 0x100);

	//step 57
	temp = DRV_Reg32(ARMPLL_LL_CON0);
	DRV_WriteReg32(ARMPLL_LL_CON0, temp | 0x100);

	/***************
	* xPLL DIV EN
	****************/
	//step 61
	temp = DRV_Reg32(MAINPLL_CON0);
	DRV_WriteReg32(MAINPLL_CON0, temp | 0xFE000000);

	//step 62
	temp = DRV_Reg32(UNIVPLL_CON0);
	DRV_WriteReg32(UNIVPLL_CON0, temp | 0xFE000000);

	//step 63
	temp = DRV_Reg32(MMPLL_CON0);
	DRV_WriteReg32(MMPLL_CON0, temp | 0xFC000000);

	//step 64
	temp = DRV_Reg32(MFGPLL_CON0);
	DRV_WriteReg32(MFGPLL_CON0, temp | 0x00010000);

	//step 65
	temp = DRV_Reg32(MFGPLL_CON0);
	DRV_WriteReg32(MFGPLL_CON0, temp | 0x00D00000);

	//step 66
	temp = DRV_Reg32(MAINPLL_CON0);
	DRV_WriteReg32(MAINPLL_CON0, temp | 0x00D00000);

	//step 67
	temp = DRV_Reg32(UNIVPLL_CON0);
	DRV_WriteReg32(UNIVPLL_CON0, temp | 0x00D00000);

	//step 68
	gpt_busy_wait_us(20); // wait for PLL stable (min delay is 20us)

	/***************
	* xPLL DIV RSTB
	****************/
	//step 71
	temp = DRV_Reg32(MFGPLL_CON0);
	DRV_WriteReg32(MFGPLL_CON0, temp | 0x01000000);

	//step 72
	temp = DRV_Reg32(MAINPLL_CON0);
	DRV_WriteReg32(MAINPLL_CON0, temp | 0x01000000);

	//step 73
	temp = DRV_Reg32(UNIVPLL_CON0);
	DRV_WriteReg32(UNIVPLL_CON0, temp | 0x01000000);

	/*****************
	* xPLL HW Control
	Set 0 to switch to SPM control
	*AP_PLL_CON3	Set 0 to switch to SPM control
		22 ARMPLL_LL_RSTB_SEL
		21 ARMPLL_LL_EN_SEL
		20 ARMPLL_LL_ISO_SEL
		19 ARMPLL_LL_PWR_SEL
		17 MAINDIV_RSTB_SEL
		13 MAINPLL_EN_SEL
		9 MAINPLL_ISO_SEL
		5 MAINPLL_PWR_SEL
		1 CLKSQ1_LPF_EN_SEL
		0 CLKSQ1_EN_SEL
	*AP_PLL_CON4	Set 0 to switch to SPM control
		13 ARMPLL_LL_OUT_OFF_SEL
		12 ARMPLL_LL_OUT_OFF
		9 MAINPLL_OUT_OFF
		1 MAINPLL_OUT_OFF_SEL

	******************/
	//step 81
	temp = DRV_Reg32(AP_PLL_CON3);
	DRV_WriteReg32(AP_PLL_CON3, temp & 0xff85dddc);

	//step 82
	temp = DRV_Reg32(AP_PLL_CON4);
	DRV_WriteReg32(AP_PLL_CON4, temp & 0xffffcdfd);

	/*************
	* MEMPLL Init
	**************/
//    mt_mempll_pre();

	/**************
	* TOPCKGEN_AO CLKMUX
	***************/
	//0: 26M,  1: armpll, 2:  Mainpll, 3:  Unipll
	temp = DRV_Reg32(INFRA_TOPCKGEN_CKMUXSEL);
	DRV_WriteReg32(INFRA_TOPCKGEN_CKMUXSEL, (temp & 0xFFFFF0FF) | (0x01 << 8));

	/************
	* TOP CLKMUX
	* CLK_CFG_0  = 0x01010102;  // mm, ddrphycfg, mem, axi
	*	axi = syspll1_d4 = 1092/8 = 136.5MHz
	*	mem =26M
	*	ddrphycfg_ck = syspll1_d8 = 1092/2/8 = 68.25MHz
	*	mm_ck = vencpll_ck = 300MHz
	* CLK_CFG_1  = 0x01000000;  // mfg, ----, ----, ---- 
	*	mfg_ck = mmpll_ck = 570MHz
	* CLK_CFG_2  = 0x00010001;  // ----, spi, uart, camtg
	*	spi_ck = syspll3_d2 = 1092/5/2 = 109.2MHz
	*	camtg_ck = univpll_48m_d2 = 1248/3/2 = 208MHz
	*	uart = univpll2_d8 =52MHz
	* CLK_CFG_3  = 0x02010100;  // msdc30_1, msdc50_0, msdc50_0_hclk, ---- 
	* 	msdc50_0_hclk = syspll1_d2 = 1092/2/2 = 273MHz
	* 	msdc50_0 = msdcpll_ck = 400MHz
	* 	msdc30_1 = univpll2_d2 = 208 MHz
	* CLK_CFG_4  = 0x01010000;  // aud_intbus, audio, ----, ----
	* 	audio = syspll3_d4 =546MHz
	* 	aud_intbus = syspll1_d4 = 136.5MHz
	* CLK_CFG_5  = 0x00000000;  // ----,  ----, ----, ----
	* CLK_CFG_6  = 0x00010101;  // ----, aud_1, scam, dbi0
	* 	dpi0 = univpll3_d2 = 594/4=124.8MHz
	* 	scam = syspll3_d2 = 594/4=109.2MHz
	* 	aud_1 = apll1_ck = 336MHz
	* CLK_CFG_7  = 0x01000001;  // usb_top, nfiecc, nfi2x, dsip_pwm
	* 	usb_top = univpll3_d4 = 594/4=148.5MHz
	* 	nfiecc = univpll1_d2 = 312MHz
	* 	nfi2x = syspll_d3 = 364MHz
	* 	dsip_pwm = univpll2_d4 = 104MHz
	* CLK_CFG_8  = 0x00020001;  // ----, i2c, ----, spm
	* 	spm = syspll_d8 = 68MHz
	* 	i2c = univpll3_d4 = 62.4MHz
	* CLK_CFG_9  = 0x01020200;  // camtg2, dxcc, seninf, ----
	* 	camtg2 = univpll_48m_d2 = 24MHz
	* 	dxcc = syspll1_d2 = 546/2=273MHz
	* 	seninf = univpll2_d2 = 416/2=208MHz
	* CLK_CFG_10 = 0x00000003;  // ----, ----, ----, aud_engen1
	* 	aud_engen1 = apll1_d8 = 336/8=42MHz
	*************/

    // [22] set this register before AXI clock switch to fast clock (26MHz => 156MHz)
    //DRV_WriteReg32(INFRA_BUS_DCM_CTRL, DRV_Reg32(INFRA_BUS_DCM_CTRL) | (1 << 21) | (1 << 22) | (1 << 23));//spm axi dcm enable
    //DRV_WriteReg32(PERI_BUS_DCM_CTRL, DRV_Reg32(PERI_BUS_DCM_CTRL) | (1 << 21) | (1 << 22) | (1 << 23));

    // config AXI clock first
    print("MUX Start..\n");
    DRV_WriteReg32(CLK_CFG_0, 0x00000002);      //axi=syspll_d5
    DRV_WriteReg32(CLK_CFG_UPDATE, 0x00000001); //update AXI clock

    // config other clocks
    DRV_WriteReg32(CLK_CFG_0, 0x01000002);
    DRV_WriteReg32(CLK_CFG_1, 0x01000000);
    DRV_WriteReg32(CLK_CFG_2, 0x00010001);
    DRV_WriteReg32(CLK_CFG_3, 0x01010100);
    DRV_WriteReg32(CLK_CFG_4, 0x01010000);
    DRV_WriteReg32(CLK_CFG_5, 0x00000000);
    DRV_WriteReg32(CLK_CFG_6, 0x00010101);
    DRV_WriteReg32(CLK_CFG_7, 0x01040301);
    DRV_WriteReg32(CLK_CFG_8, 0x00020001);
    DRV_WriteReg32(CLK_CFG_9, 0x01010200);
    DRV_WriteReg32(CLK_CFG_10, 0x00000003);
    DRV_WriteReg32(CLK_CFG_UPDATE, 0xFFFFFFFC);	//update all clocks except "fmem & axi"
    DRV_WriteReg32(CLK_CFG_UPDATE1, 0x3FF);			//update all clocks

	DRV_WriteReg32(CLK_SCP_CFG_0, 0x3FF);
	DRV_WriteReg32(CLK_SCP_CFG_1, 0x11);

    /*for MTCMOS*/
    //spm_write(POWERON_CONFIG_EN, (SPM_PROJECT_CODE << 16) | (1U << 0));
    /*print("mtcmos Start..\n");*/
	/*spm_mtcmos_ctrl_dis(STA_POWER_ON);*/
#if 0
	spm_mtcmos_ctrl_mm0(STA_POWER_ON);
	spm_mtcmos_ctrl_md1( STA_POWER_ON);
	spm_mtcmos_ctrl_isp( STA_POWER_ON);
	spm_mtcmos_ctrl_vde( STA_POWER_ON);
	spm_mtcmos_ctrl_mfg_async( STA_POWER_ON);
	spm_mtcmos_ctrl_mfg( STA_POWER_ON);
	spm_mtcmos_ctrl_mfg_core3( STA_POWER_ON);
	spm_mtcmos_ctrl_mfg_core2( STA_POWER_ON);
	spm_mtcmos_ctrl_mfg_core1( STA_POWER_ON);
	spm_mtcmos_ctrl_mfg_core0( STA_POWER_ON);
	spm_mtcmos_ctrl_mjc( STA_POWER_ON);
	spm_mtcmos_ctrl_ven( STA_POWER_ON);
	spm_mtcmos_ctrl_audio( STA_POWER_ON);
	spm_mtcmos_ctrl_c2k( STA_POWER_ON);
#endif

    /*print("mtcmos Done!\n");*/
#if 0
    /*for CG*/
    //infra default disable CG but auxadc default on
    DRV_WriteReg32(INFRA_SW_CG0_CLR, 0xFBEFFFFF); //bit 20,26,: no use
    DRV_WriteReg32(INFRA_SW_CG1_CLR, 0xFFFF3FFF); //bit 14, 15: no use , bit 11 is auxadc
    DRV_WriteReg32(INFRA_SW_CG2_CLR, 0xFFFFFDFF); //bit 9: no use, bit 14 dvfs_spm0
    /*dvfs cpu, gpu setting*/
    temp = DRV_Reg32(PERI_DVFS0_TX_APB_ASYNC_STA);
    DRV_WriteReg32(PERI_DVFS0_TX_APB_ASYNC_STA, temp | 0x1 );

    temp = DRV_Reg32(PERI_DVFS1_TX_APB_ASYNC_STA);
    DRV_WriteReg32(PERI_DVFS1_TX_APB_ASYNC_STA, temp | 0x1);

#endif
    /*INFRA CG*/
    DRV_WriteReg32(MODULE_SW_CG_0_CLR, 0xFFFFFFFF); //
    DRV_WriteReg32(MODULE_SW_CG_1_CLR, 0xFFFFFDFF); //
    DRV_WriteReg32(MODULE_SW_CG_1_SET, 0x18000200); //disable trng[9], secure access after devapc in LK, [27][28] DXCC
    DRV_WriteReg32(MODULE_SW_CG_2_CLR, 0xFFFFFFFF); //
    DRV_WriteReg32(MODULE_SW_CG_3_CLR, 0xFFFFFFFF); //

    /*DISP CG*/
    //DRV_WriteReg32(MMSYS_CG_CLR0, 0xFFFFFFFF);
    //DRV_WriteReg32(MMSYS_CG_CLR1, 0x3FFF);

    /*AUDIO CG*/
    //temp = DRV_Reg32(AUDIO_TOP_CON0);
    //DRV_WriteReg32(AUDIO_TOP_CON0, temp & 0xF0F7FEFB);
    //temp = DRV_Reg32(AUDIO_TOP_CON1);
    //DRV_WriteReg32(AUDIO_TOP_CON1, temp & 0xFFFFFF0F );

    print("Pll init Done!\n");
}

