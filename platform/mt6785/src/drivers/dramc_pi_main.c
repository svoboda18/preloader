/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/

//=============================================================================
//  Include Files
//=============================================================================
#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
#if __ETT__
#include <common.h>
#include <ett_common.h>
#include <api.h>
#endif
#endif

//#if (FOR_DV_SIMULATION_USED==0)
#include "emi.h"
//#endif
#include "dramc_common.h"
#include "dramc_pi_api.h"
#include "x_hal_io.h"
#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
#ifndef MT6779_FPGA
#include <pmic.h>
#endif
#endif

#if ! __ETT__
#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
#include "pmic.h"
#include <dconfig_env.h>
#endif
#endif

//=============================================================================
//  Definition
//=============================================================================

//=============================================================================
//  Global Variables
//=============================================================================
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
   SAVE_TIME_FOR_CALIBRATION_T SavetimeData;
#endif

#define ENABLE_DRAM_SINGLE_FREQ_SELECT 0xFF // 0xFF=all freq by gFreqTbl. The 0x"X" != 0xFF for single freq by gFreqTbl index, ex: 0x3 for DDR3733

// Darren: fastK (DramcSave_Time_For_Cal_Init) need porting depend on gFreqTbl
DRAM_DFS_FREQUENCY_TABLE_T gFreqTbl[DRAM_DFS_SHUFFLE_MAX] = {
   {LP4_DDR3200 /*0*/, DIV8_MODE, SRAM_SHU1, DUTY_LAST_K,  VREF_CALI_OFF, NORMAL_CLOSE_LOOP},  //francis : can't change this shuffle's k position due to duty K
   {LP4_DDR2400 /*1*/, DIV8_MODE, SRAM_SHU3, DUTY_LAST_K,  VREF_CALI_OFF, NORMAL_CLOSE_LOOP},  //francis : can't change this shuffle's k position due to duty K  
   {LP4_DDR3200 /*2*/, DIV8_MODE, SRAM_SHU2, DUTY_NEED_K,  VREF_CALI_OFF, NORMAL_CLOSE_LOOP},  //francis : can't change this shuffle's k position due to duty K
   {LP4_DDR3733 /*3*/, DIV8_MODE, SRAM_SHU0, DUTY_NEED_K,  VREF_CALI_ON,  NORMAL_CLOSE_LOOP},  // highest freq of term group (3733) must k first.
   {LP4_DDR1600 /*4*/, DIV4_MODE, SRAM_SHU5, DUTY_DEFAULT, VREF_CALI_OFF, NORMAL_CLOSE_LOOP},
   {LP4_DDR1200 /*5*/, DIV4_MODE, SRAM_SHU7, DUTY_DEFAULT, VREF_CALI_OFF, NORMAL_CLOSE_LOOP},
   {LP4_DDR1200 /*6*/, DIV4_MODE, SRAM_SHU8, DUTY_DEFAULT, VREF_CALI_OFF, NORMAL_CLOSE_LOOP},
   {LP4_DDR800  /*7*/, DIV4_MODE, SRAM_SHU9, DUTY_DEFAULT, VREF_CALI_OFF, DDR800_SEMI_LOOP},
   {LP4_DDR2400 /*8*/, DIV8_MODE, SRAM_SHU4, DUTY_NEED_K,  VREF_CALI_ON,  NORMAL_CLOSE_LOOP},// highest freq of unterm group (2400) must k first.
   {LP4_DDR1600 /*9*/, DIV8_MODE, SRAM_SHU6, DUTY_DEFAULT, VREF_CALI_ON,  NORMAL_CLOSE_LOOP},  //Darren: DDR1600 for MRW (DramcModeRegInit_LP4 and CBT)
};

DRAMC_CTX_T *psCurrDramCtx;

#if defined(DDR_INIT_TIME_PROFILING) || (__ETT__ && SUPPORT_SAVE_TIME_FOR_CALIBRATION)
DRAMC_CTX_T gTimeProfilingDramCtx;
U8 gtime_profiling_flag = 0;
#endif

bool gAndroid_DVFS_en = TRUE;
bool gUpdateHighestFreq = FALSE;

DRAMC_CTX_T DramCtx_LPDDR4 =
{
  CHANNEL_DUAL, // Channel number
  CHANNEL_A,          // DRAM_CHANNEL
  RANK_DUAL,        //DRAM_RANK_NUMBER_T
  RANK_0,               //DRAM_RANK_T
  &gFreqTbl[DRAM_DFS_SHUFFLE_MAX-1], // Default is DDR1600 1:8 mode 0.8V
  DRAM_DFS_SHUFFLE_1,
  TYPE_LPDDR4X,        // DRAM_DRAM_TYPE_T
  FSP_0 , //// DRAM Fast switch point type, only for LP4, useless in LP3
  ODT_OFF,
  {CBT_NORMAL_MODE, CBT_NORMAL_MODE},  // bring up LP4X rank0 & rank1 use normal mode
#if ENABLE_READ_DBI
  {DBI_OFF,DBI_ON},  //read DBI
#else
  {DBI_OFF,DBI_OFF}, //read DBI
#endif
#if ENABLE_WRITE_DBI
  {DBI_OFF,DBI_ON},  // write DBI
#else
  {DBI_OFF,DBI_OFF},  // write DBI
#endif
  DATA_WIDTH_16BIT,     // DRAM_DATA_WIDTH_T
  DEFAULT_TEST2_1_CAL,    // test2_1;
  DEFAULT_TEST2_2_CAL,    // test2_2;
  TEST_XTALK_PATTERN,     // test_pattern;
  800,                  // frequency
  800,                  // freqGroup
  0x88, //vendor_id initial value
  REVISION_ID_MAGIC, //revision id
  0xff, //density
  {0,0},
  0,  // ucnum_dlycell_perT;
  0,  // u2DelayCellTimex100;

#if PRINT_CALIBRATION_SUMMARY
   //aru4CalResultFlag[CHANNEL_NUM][RANK_MAX]
   {{0,0,}, {0,0}},
   //aru4CalExecuteFlag[CHANNEL_NUM][RANK_MAX]
   {{0,0,}, {0,0}},
   1,
   0,
#endif
#if WRITE_LEVELING_MOVE_DQS_INSTEAD_OF_CLK
  {{0,0}, {0,0}}, //BOOL arfgWriteLevelingInitShif;
#endif
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
     FALSE, //femmc_Ready
     0,
     0,
     0,
    &SavetimeData,
#endif
    PHYPLL_MODE,
};

U8 gfirst_init_flag = 0;
//=============================================================================
//  External references
//=============================================================================
#if __ETT__
extern int global_which_test;
#endif

#if !__ETT__ && defined(DDR_RESERVE_MODE)
extern u32 g_ddr_reserve_ta_err;
#endif

extern U8 gu1MR23Done;

extern void EMI_Init(DRAMC_CTX_T *p);
extern void EMI_Init2(void);

#if (FOR_DV_SIMULATION_USED==1 || SW_CHANGE_FOR_SIMULATION==1)
int doe_get_config(char* feature)
{return 0;}
#endif

/*
 * LP4 table
 *  [0] 3200
 *  [1] 3200
 *  [2] 2400
 *  [3] 1600
 * LP3 table
 *  [0] 1866
 *  [1] 1600
 *  [2] 1600
 *  [3] 1200
 */
#define STD_LP4_VCORE_KOPP0	800000
#define STD_LP4_VCORE_KOPP1	750000
#define STD_LP4_VCORE_KOPP2	700000
#define STD_LP4_VCORE_KOPP3	700000
#define STD_LP3_VCORE_KOPP0	800000
#define STD_LP3_VCORE_KOPP1	750000
#define STD_LP3_VCORE_KOPP2	750000
#define STD_LP3_VCORE_KOPP3	700000
#ifdef DRAM_HQA
#if __ETT__
extern unsigned int hqa_LP4_vcore_kopp0;
extern unsigned int hqa_LP4_vcore_kopp1;
extern unsigned int hqa_LP4_vcore_kopp2;
extern unsigned int hqa_LP4_vcore_kopp3;
extern unsigned int hqa_LP3_vcore_kopp0;
extern unsigned int hqa_LP3_vcore_kopp1;
extern unsigned int hqa_LP3_vcore_kopp2;
extern unsigned int hqa_LP3_vcore_kopp3;
#define VCORE_KOPP_BY_FREQ(F, D)	hqa_##D##_vcore_kopp##F
#else
#define VCORE_KOPP_BY_FREQ(F, D)	HQA_VCORE_KOPP(F, D)
#endif
#else
#define VCORE_KOPP_BY_FREQ(F, D)	STD_##D##_VCORE_KOPP##F
#endif

U8 gHQA_Test_Freq_Vcore_Level = 0;  // 0: only 1 freq  , others are multi freq  1: low vcore 2: high vcore

void vSetVcoreByFreq(DRAMC_CTX_T *p)
{
#ifndef MT6779_FPGA
#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
#if __FLASH_TOOL_DA__
	dramc_set_vcore_voltage(800000);
#else

	#if 0 //bring-up
	dramc_set_vcore_voltage(800000);
	dramc_set_vdram_voltage(TYPE_LPDDR4, 1125000);
	dramc_set_vddq_voltage(TYPE_LPDDR4, 600000);
	dramc_set_vmddr_voltage(750000);
	print("Read voltage for %d, %d\n", p->frequency, vGet_Current_ShuLevel(p));
	print("Vcore = %d\n", dramc_get_vcore_voltage());
	print("Vdram = %d\n", dramc_get_vdram_voltage(p->dram_type));
	print("Vddq = %d\n", dramc_get_vddq_voltage(p->dram_type));
	print("Vmddr = %d\n", dramc_get_vmddr_voltage());
	return;
	#endif

	unsigned int vio18, vcore, vdram, vddq, vmddr;
	int ret;

	vio18 = vcore = vdram = vddq = vmddr = 0;

#if defined(__ETT__)
	hqa_set_voltage_by_freq(p, &vio18, &vcore, &vdram, &vddq, &vmddr);
#elif defined(VCORE_BIN)
	switch (vGet_Current_ShuLevel(p)) {
	case SRAM_SHU0:
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(p->dram_type, KSHU0);
		if (!vcore)
	#endif
		vcore = get_vcore_uv_table(0);
		break;
	case SRAM_SHU1:
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(p->dram_type, KSHU1);
		if (!vcore)
	#endif
		vcore = (3 * get_vcore_uv_table(0) + get_vcore_uv_table(1)) >> 2;
		break;
	case SRAM_SHU2:
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(p->dram_type, KSHU2);
		if (!vcore)
	#endif
		vcore = (get_vcore_uv_table(0) + 3 * get_vcore_uv_table(1)) >> 2;
		break;
	case SRAM_SHU3:
	case SRAM_SHU5:
	case SRAM_SHU7:
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(p->dram_type, KSHU3);
		if (!vcore)
	#endif
		vcore = (get_vcore_uv_table(0) + get_vcore_uv_table(1)) >> 1;
		break;
	case SRAM_SHU4:
	case SRAM_SHU6:
	case SRAM_SHU8:
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(p->dram_type, KSHU4);
		if (!vcore)
	#endif
		vcore = get_vcore_uv_table(1);
		break;
	case SRAM_SHU9:
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(p->dram_type, KSHU9);
		if (!vcore)
	#endif
		vcore = get_vcore_uv_table(2);
		break;
	}
#else
	switch (vGet_Current_ShuLevel(p)) {
	case SRAM_SHU0: // 3733
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(p->dram_type, KSHU0);
	#else
		vcore = SEL_PREFIX_VCORE(LP4, KSHU0);
	#endif
		break;
	case SRAM_SHU1: // 3200
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(p->dram_type, KSHU1);
	#else
		vcore = SEL_PREFIX_VCORE(LP4, KSHU1);
	#endif
		break;
	case SRAM_SHU2: // 3200
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(p->dram_type, KSHU2);
	#else
		vcore = SEL_PREFIX_VCORE(LP4, KSHU2);
	#endif
		break;
	case SRAM_SHU3: // 2400
	case SRAM_SHU5: // 1600
	case SRAM_SHU7: // 1200
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(p->dram_type, KSHU3);
	#else
		vcore = SEL_PREFIX_VCORE(LP4, KSHU3);
	#endif
		break;
	case SRAM_SHU4: // 2400
	case SRAM_SHU6: // 1600
	case SRAM_SHU8: // 1200
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(p->dram_type, KSHU4);
	#else
		vcore = SEL_PREFIX_VCORE(LP4, KSHU4);
	#endif
		break;
	case SRAM_SHU9: // 800
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(p->dram_type, KSHU9);
	#else
		vcore = SEL_PREFIX_VCORE(LP4, KSHU9);
	#endif
		break;
	default:
		return;
    }
#endif

	if (((doe_get_config("dram_fix_3094_0825")) && (vGet_Current_ShuLevel(p)==SRAM_SHU1)) || (doe_get_config("dram_all_3094_0825")) || (doe_get_config("dram_opp0_3733_others_3094_0825")))
			vcore= 825000;
	else if (((doe_get_config("dram_fix_3094_0725")) && (vGet_Current_ShuLevel(p)==SRAM_SHU2)) || (doe_get_config("dram_all_3094_0725")) || (doe_get_config("dram_all_1534_0725")) ||
		(doe_get_config("dramopp0_3094_others_1534_0725")) || (doe_get_config("dram_opp0_2400_others_1534_0725")))
			vcore= 725000;
	else if ((doe_get_config("dram_fix_2400_0725")) && (vGet_Current_ShuLevel(p)==SRAM_SHU4))
			vcore= 725000;
	else if ((doe_get_config("dram_fix_1534_0725")) && (vGet_Current_ShuLevel(p)==SRAM_SHU6))
			vcore= 725000;
	else if ((doe_get_config("dram_fix_1200_0725")) && (vGet_Current_ShuLevel(p)==SRAM_SHU8))
			vcore= 725000;
	else if ((doe_get_config("dram_fix_1200_065")) && (vGet_Current_ShuLevel(p)==SRAM_SHU8))
			vcore= 650000;
	else if ((doe_get_config("dram_fix_800_065")) && (vGet_Current_ShuLevel(p)==SRAM_SHU9))
			vcore= 650000;

	if (vcore)
		dramc_set_vcore_voltage(vcore);

#if defined(DRAM_HQA)
	if (vio18)
		dramc_set_vio18_voltage(vio18);

	if (vdram)
		dramc_set_vdram_voltage(p->dram_type, vdram);

	if (vddq)
		dramc_set_vddq_voltage(p->dram_type, vddq);

	if (vmddr)
		dramc_set_vmddr_voltage(vmddr);
#endif

#ifdef FOR_HQA_REPORT_USED
    switch (vGet_Current_ShuLevel(p)) {
        case SRAM_SHU0: // 3733
		case SRAM_SHU9: // 800
            gHQA_Test_Freq_Vcore_Level = 0; //only 1 freq
            break;
        case SRAM_SHU1: // 3200
        case SRAM_SHU3: // 2400
        case SRAM_SHU5: // 1600
        case SRAM_SHU7: // 1200
            gHQA_Test_Freq_Vcore_Level = 2; //high vcore
            break;
        case SRAM_SHU2: // 3200    
        case SRAM_SHU4: // 2400
        case SRAM_SHU6: // 1600
        case SRAM_SHU8: // 1200
            gHQA_Test_Freq_Vcore_Level = 1; //low vcore
            break;
        default:
            print("[HQA] undefined shuffle level for Vcore (SHU%d)\r\n", vGet_Current_ShuLevel(p));
#if __ETT__
            while(1);
#endif
            break;
    }
#endif

#ifndef DDR_INIT_TIME_PROFILING
	print("Read voltage for %d, %d\n", p->frequency, vGet_Current_ShuLevel(p));
	print("Vio18 = %d\n", dramc_get_vio18_voltage());
	print("Vcore = %d\n", dramc_get_vcore_voltage());
	print("Vdram = %d\n", dramc_get_vdram_voltage(p->dram_type));
	print("Vddq = %d\n", dramc_get_vddq_voltage(p->dram_type));
	print("Vmddr = %d\n", dramc_get_vmddr_voltage());
#endif
#endif
#endif
#endif
}

U32 vGetVoltage(DRAMC_CTX_T *p, U32 get_voltage_type)
{
#if (defined(DRAM_HQA) || defined(__ETT__)) && (FOR_DV_SIMULATION_USED == 0)
    if (get_voltage_type==0)
        return dramc_get_vcore_voltage();

    if (get_voltage_type==1)
        return dramc_get_vdram_voltage(p->dram_type);

    if (get_voltage_type==2)
        return dramc_get_vddq_voltage(p->dram_type);

    if (get_voltage_type==3)
           return dramc_get_vio18_voltage();
    if (get_voltage_type==4)
             return dramc_get_vmddr_voltage();
    
#endif

    return 0;
}


#ifdef FOR_HQA_TEST_USED
VCORE_DELAYCELL_T gVcoreDelayCellTable[51]={ {562500, 1140},
                                                                            {568750, 1125},
                                                                            {575000, 1103},
                                                                            {581250, 1097},
                                                                            {587500, 1080},
                                                                            {593750, 1059},
                                                                            {600000, 1050},
                                                                            {606250, 1046},
                                                                            {612500, 1014},
                                                                            {618750, 1004},
                                                                            {625000, 1002},
                                                                            {631250, 994},
                                                                            {637500, 966},
                                                                            {643750, 959},
                                                                            {650000, 954},
                                                                            {656250, 923},
                                                                            {662500, 920},
                                                                            {668750, 907},
                                                                            {675000, 888},
                                                                            {681250, 892},
                                                                            {687500, 877},
                                                                            {693750, 868},
                                                                            {700000, 865},
                                                                            {706250, 843},
                                                                            {712500, 841},
                                                                            {718750, 830},
                                                                            {725000, 820},
                                                                            {731250, 815},
                                                                            {737500, 810},
                                                                            {743750, 800},
                                                                            {750000, 783},
                                                                            {756250, 780},
                                                                            {762500, 769},
                                                                            {768750, 768},
                                                                            {775000, 756},
                                                                            {781250, 753},
                                                                            {787500, 752},
                                                                            {793750, 747},
                                                                            {800000, 740},
                                                                            {806250, 732},
                                                                            {812500, 725},
                                                                            {818750, 720},
                                                                            {825000, 718},
                                                                            {831250, 717},
                                                                            {837500, 715},
                                                                            {843750, 713},
                                                                            {850000, 708},
                                                                            {856250, 705},
                                                                            {862500, 702},
                                                                            {868750, 700},
                                                                            {875000, 698} };

void GetVcoreDelayCellTimeFromTable(DRAMC_CTX_T *p)
{
    U32 channel_i, i;
    U32 get_vcore = 0;
    U16 u2gdelay_cell_ps = 0;
    U8 u1delay_cell_cnt = 0;
    VCORE_DELAYCELL_T *pVcoreDelayCellTable;

#if (defined(DRAM_HQA) || defined(__ETT__)) && (FOR_DV_SIMULATION_USED == 0)
    get_vcore = dramc_get_vcore_voltage();
#endif

    pVcoreDelayCellTable = (VCORE_DELAYCELL_T *)gVcoreDelayCellTable;
    u1delay_cell_cnt = sizeof(gVcoreDelayCellTable)/sizeof(gVcoreDelayCellTable[0]);

    for(i=0; i<u1delay_cell_cnt; i++)
    {
        if (get_vcore >= pVcoreDelayCellTable[i].u2Vcore) u2gdelay_cell_ps = pVcoreDelayCellTable[i].u2DelayCell;
    }

    mcSHOW_DBG_MSG(("[GetVcoreDelayCellTimeFromTable(%d)] VCore=%d(x100), DelayCell=%d(x100)\n", u1delay_cell_cnt, get_vcore, u2gdelay_cell_ps));

    for(channel_i=CHANNEL_A; channel_i < p->support_channel_num; channel_i++)
    {
        u2gdelay_cell_ps_all[get_shuffleIndex_by_Freq(p)][channel_i] = u2gdelay_cell_ps;
    }
}
#endif

void mem_test_address_calculation(DRAMC_CTX_T * p, U32 uiSrcAddr, U32*pu4Dest)
{
    U32 u4RankSize;

#if __ETT__
    *pu4Dest = uiSrcAddr - RANK0_START_VA + RANK1_START_VA;
#else
    *pu4Dest = uiSrcAddr + p->ranksize[RANK_0];
#endif
}


#if CPU_RW_TEST_AFTER_K
void vDramCPUReadWriteTestAfterCalibration(DRAMC_CTX_T *p)
{
    U8 u1DumpInfo=0, u1RankIdx;
    U32 uiLen, count, uiFixedAddr, uiRankdAddr[RANK_MAX];
    U32 pass_count, err_count;
    uiLen = 0xffff;

#if GATING_ONLY_FOR_DEBUG
    DramcGatingDebugInit(p);
#endif

    uiRankdAddr[0] = DDR_BASE;
    mem_test_address_calculation(p, DDR_BASE, &uiRankdAddr[1]);

    for(u1RankIdx =0; u1RankIdx< p->support_rank_num; u1RankIdx++)
    {
        u1DumpInfo=0;
        err_count=0;
        pass_count=0;

#if !__ETT__
	// scy: not to test rank1 (wrong addr 0x0000_0000)
	if (u1RankIdx >= 1)
		continue;
#endif

        #if GATING_ONLY_FOR_DEBUG
        DramcGatingDebugRankSel(p, u1RankIdx);
        #endif

        uiFixedAddr = uiRankdAddr[u1RankIdx];

        for (count= 0; count<uiLen; count+=4)
        {
            *(volatile unsigned int   *)(count +uiFixedAddr) = count + (0x5a5a <<16);
        }

        for (count=0; count<uiLen; count+=4)
        {
            if (*(volatile unsigned int   *)(count +uiFixedAddr) != count + (0x5a5a <<16))
            {
                //mcSHOW_DBG_MSG(("[Fail] Addr %xh = %xh\n",count, *(volatile unsigned int   *)(count)));
                err_count++;
            }
            else
                pass_count ++;
        }

#if RUNTIME_SHMOO_RELEATED_FUNCTION && SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if (err_count==0)
        {
#if __ETT__
            mcSHOW_ERR_MSG(("CH %c,RANK %d,BYTE %d,VRANGE %d,VREF %d,PI %d,MEM_RESULT PASS\n",
                p->pSavetimeData->Runtime_Shmoo_para.TX_Channel == 0 ? 'A' : 'B',
                p->pSavetimeData->Runtime_Shmoo_para.TX_Rank,
                p->pSavetimeData->Runtime_Shmoo_para.TX_Byte,
                p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Range,
                p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Value,
                p->pSavetimeData->Runtime_Shmoo_para.TX_PI_delay-p->pSavetimeData->Runtime_Shmoo_para.TX_Original_PI_delay));
#else
            print("CH %c,RANK %d,BYTE %d,VRANGE %d,VREF %d,PI %d,MEM_RESULT PASS\n",
                p->pSavetimeData->Runtime_Shmoo_para.TX_Channel == 0 ? 'A' : 'B',
                p->pSavetimeData->Runtime_Shmoo_para.TX_Rank,
                p->pSavetimeData->Runtime_Shmoo_para.TX_Byte,
                p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Range,
                p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Value,
                p->pSavetimeData->Runtime_Shmoo_para.TX_PI_delay-p->pSavetimeData->Runtime_Shmoo_para.TX_Original_PI_delay);
#endif
        }
#else
        if(err_count)
        {
            mcSHOW_DBG_MSG(("[MEM_TEST] Rank %d Fail.", u1RankIdx));
            u1DumpInfo =1;
#if defined(SLT)
            while(1);
#endif
        }
        else
        {
            mcSHOW_DBG_MSG(("[MEM_TEST] Rank %d OK.", u1RankIdx));
        }
        mcSHOW_DBG_MSG(("(uiFixedAddr 0x%X, Pass count =%d, Fail count =%d)\n", uiFixedAddr, pass_count, err_count));
#endif
    }

    if(u1DumpInfo)
    {
        // Read gating error flag
        #if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
        DramcDumpDebugInfo(p);
        #endif
    }

    #if GATING_ONLY_FOR_DEBUG
    DramcGatingDebugExit(p);
    #endif
}
#endif


#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
#if !EMMC_READY
u32 g_dram_save_time_init_done[DRAM_DFS_SHUFFLE_MAX]={0};
SAVE_TIME_FOR_CALIBRATION_T SaveTimeDataByShuffle[DRAM_DFS_SHUFFLE_MAX];
#endif

DRAM_STATUS_T DramcSave_Time_For_Cal_End(DRAMC_CTX_T *p)
{
    if(!u1IsLP4Family(p->dram_type))
        return DRAM_FAIL;

    if(p->femmc_Ready==0)
    {
        #if EMMC_READY
        write_offline_dram_calibration_data(p->shu_type, p->pSavetimeData);
        mcSHOW_DBG_MSG(("[FAST_K] Save calibration result to emmc\n"));
        #else
        g_dram_save_time_init_done[p->shu_type] =1;
        memcpy(&(SaveTimeDataByShuffle[p->shu_type]), p->pSavetimeData, sizeof(SAVE_TIME_FOR_CALIBRATION_T));
        mcSHOW_DBG_MSG(("[FAST_K] Save calibration result to SW memory\n"));
        #endif
    }
    else
    {
        mcSHOW_DBG_MSG(("[FAST_K] Bypass saving calibration result to emmc\n"));
    }

	return DRAM_OK;
}

DRAM_STATUS_T DramcSave_Time_For_Cal_Init(DRAMC_CTX_T *p)
{
    if(!u1IsLP4Family(p->dram_type))
        return DRAM_FAIL;

	if (doe_get_config("fullk"))
		return DRAM_FAIL;

    // Parepare fask k data
    #if EMMC_READY
    // scy: only need to read emmc one time for each boot-up
    //if (g_dram_save_time_init_done == 1)
    //    return DRAM_OK;
    //else
    //    g_dram_save_time_init_done = 1;
    if(read_offline_dram_calibration_data(p->shu_type, p->pSavetimeData)<0)
        {
        p->femmc_Ready=0;
        memset(p->pSavetimeData, 0, sizeof(SAVE_TIME_FOR_CALIBRATION_T));
        }
        else
        {
        p->femmc_Ready=1;
        }

    #else //EMMC is not avaliable, load off-line data

    if(g_dram_save_time_init_done[p->shu_type] ==0)
    {
        p->femmc_Ready=0;    
        memset(p->pSavetimeData, 0, sizeof(SAVE_TIME_FOR_CALIBRATION_T));
    }
    else
    {
        memcpy(p->pSavetimeData, &(SaveTimeDataByShuffle[p->shu_type]), sizeof(SAVE_TIME_FOR_CALIBRATION_T));
        p->femmc_Ready=1;
    }
    #endif

    if(p->femmc_Ready==1)
    {
        if(p->frequency <1600)
        {   // freq < 1600, TX and RX tracking are disable. Therefore, bypass calibration.
            p->Bypass_RDDQC=1;
            p->Bypass_RXWINDOW=1;
            p->Bypass_TXWINDOW=1;
    }
    else
    {
            p->Bypass_RDDQC=1;
            p->Bypass_RXWINDOW=0;
            p->Bypass_TXWINDOW=0;
    }

#if RUNTIME_SHMOO_RELEATED_FUNCTION
        p->Bypass_RDDQC=1;
        p->Bypass_RXWINDOW=1;
        p->Bypass_TXWINDOW=1;
#endif
    }

#if EMMC_READY
    mcSHOW_DBG_MSG(("[FAST_K] DramcSave_Time_For_Cal_Init SHU%d, femmc_Ready=%d\n", p->shu_type, p->femmc_Ready));
#else
    mcSHOW_DBG_MSG(("[FAST_K] DramcSave_Time_For_Cal_Init SHU%d, Init_done=%d, femmc_Ready=%d\n", p->shu_type, g_dram_save_time_init_done[p->shu_type], p->femmc_Ready));
#endif
    mcSHOW_DBG_MSG(("[FAST_K] Bypass_RDDQC %d, Bypass_RXWINDOW=%d, Bypass_TXWINDOW=%d\n", p->Bypass_RDDQC, p->Bypass_RXWINDOW, p->Bypass_TXWINDOW));

    return DRAM_OK;
}
#endif


U8 gGet_MDL_Used_Flag=0;
void Set_MDL_Used_Flag(U8 value)
{
    gGet_MDL_Used_Flag = value;
}

U8 Get_MDL_Used_Flag(void)
{
    return gGet_MDL_Used_Flag;
}

#if TX_K_DQM_WITH_WDBI
#ifdef FOR_HQA_TEST_USED
extern U8 gFinalTXVrefDQ[CHANNEL_NUM][RANK_MAX];
#endif
void vSwitchWriteDBISettings(DRAMC_CTX_T *p, U8 u1OnOff)
{
    S8 u1TXShiftUI;

    u1TXShiftUI = (u1OnOff) ? -8 : 8;
    DramcWriteMinus1MCKForWriteDBI(p, u1TXShiftUI); //Tx DQ/DQM -1 MCK for write DBI ON

    SetDramModeRegForWriteDBIOnOff(p, p->dram_fsp, u1OnOff);
    DramcWriteDBIOnOff(p, u1OnOff);
}
#endif

static void vCalibration_Flow_LP4(DRAMC_CTX_T *p)
{
    U8 u1RankMax;
    S8 s1RankIdx;
    //DRAM_STATUS_T VrefStatus;

#ifdef DDR_INIT_TIME_PROFILING
    U32 CPU_Cycle;
    TimeProfileBegin();
#endif

#if ENABLE_PHY_RX_INPUT_OFFSET  // skip when bring up
    ///TODO: no shuffle, only need to do once under highest freq.
    if(p->frequency == u2DFSGetHighestFreq(p))
    DramcRXInputBufferOffsetCal(p);

#ifdef DDR_INIT_TIME_PROFILING
    CPU_Cycle=TimeProfileEnd();
    mcSHOW_TIME_MSG(("\tRX input cal takes %d us\n", CPU_Cycle));
    TimeProfileBegin();
#endif
#endif


#if GATING_ADJUST_TXDLY_FOR_TRACKING
    DramcRxdqsGatingPreProcess(p);
#endif

    if (p->support_rank_num==RANK_DUAL)
        u1RankMax = RANK_MAX;
    else
        u1RankMax = RANK_1;

    //vAutoRefreshSwitch(p, DISABLE); //auto refresh is set as disable in LP4_DramcSetting, so don't need to disable again

#if ENABLE_CA_TRAINING
#if (fcFOR_CHIP_ID == fcKrug)
    if (p->frequency != 400) // for Ei_ger E1 DDR800 no CBT calib
#endif  
    {
        #if ENABLE_CBT_DIV4_WA // for Ei_ger E1 1:4 mode CBT workaround
        if ((vGet_Div_Mode(p) == DIV4_MODE) && (p->frequency <= 800))
            CmdBusTrainingDiv4ModeWA(p, DIV8_MODE, ENABLE);
        #endif
    
        for(s1RankIdx=RANK_0; s1RankIdx<u1RankMax; s1RankIdx++)
        {
            vSetRank(p, s1RankIdx);
            #if PINMUX_AUTO_TEST_PER_BIT_CA
            CheckCAPinMux(p);
            #endif

            if (GetEyeScanEnable(p, 0) == ENABLE) gCBT_EYE_Scan_flag = 2;

            CmdBusTrainingLP4(p);   //CBT normal K

            if (doe_get_config("log_to_storage"))
            {
            #if ENABLE_EYESCAN_GRAPH
            if (gCBT_EYE_Scan_flag==2)
            {
                gCBT_EYE_Scan_flag=1;
                CmdBusTrainingLP4(p);   //K CBT eyescan   
            }
            print_EYESCAN_LOG_message(p, 0); //draw CBT eyescan
            #endif
            }
            #ifdef DDR_INIT_TIME_PROFILING
            CPU_Cycle=TimeProfileEnd();
            mcSHOW_TIME_MSG(("\tRank %d CBT takes %d us\n", s1RankIdx, CPU_Cycle));
            TimeProfileBegin();
            #endif   
        }
        vSetRank(p, RANK_0);

        No_Parking_On_CLRPLL(p);

        #if ENABLE_CBT_DIV4_WA // for Ei_ger E1 1:4 mode CBT workaround
        if ((vGet_Div_Mode(p) == DIV4_MODE) && (p->frequency <= 800))
            CmdBusTrainingDiv4ModeWA(p, DIV4_MODE, DISABLE);
        #endif
    }
#endif //ENABLE_CA_TRAINING

    for(s1RankIdx=RANK_0; s1RankIdx<u1RankMax; s1RankIdx++)
    {
        vSetRank(p, s1RankIdx);

        vAutoRefreshSwitch(p, DISABLE); //When doing WriteLeveling, should make sure that auto refresh is disable

//#if ENABLE_LP4_ZQ_CAL
        //DramcZQCalibration(p); //ZQ calibration should be done before CBT and operated at low frequency, so it is moved to mode register init
//#endif

        #if ENABLE_WRITE_LEVELING
        if (!(u1IsLP4Div4DDR800(p) && (p->rank==RANK_1))) // skip for DDR800 rank1
        {
            #if ENABLE_DDR800_OPEN_LOOP_PHASE_PREDICTION
            if (vGet_DDR800_Mode(p) == DDR800_SEMI_LOOP)
                DramcWriteLevelingDdr800Semi((DRAMC_CTX_T *) p); // for DDR800 semi open loop mode
            else
            #endif
            DramcWriteLeveling((DRAMC_CTX_T *) p);//Dram will be reset when finish write leveling
        }

        #ifdef DDR_INIT_TIME_PROFILING
        CPU_Cycle=TimeProfileEnd();
        mcSHOW_TIME_MSG(("\tRank %d Write leveling takes %d us\n", s1RankIdx, CPU_Cycle));
        TimeProfileBegin();
        #endif
        #endif

        #if LJPLL_FREQ_DEBUG_LOG
        DDRPhyFreqMeter();
        #endif

        vAutoRefreshSwitch(p, ENABLE); //when doing gating, RX and TX calibration, auto refresh should be enable

        DramcRxdqsGatingCal(p);

        #ifdef DDR_INIT_TIME_PROFILING
        CPU_Cycle=TimeProfileEnd();
        mcSHOW_TIME_MSG(("\tRank %d Gating takes %d us\n", s1RankIdx, CPU_Cycle));
        TimeProfileBegin();
        #endif

        #if LJPLL_FREQ_DEBUG_LOG
            DDRPhyFreqMeter();
        #endif

#if ENABLE_RDDQC_DIV4_WA
        DramcRxWinRDDQCWA(p); // RDDQC workaround for 1:4 mode
#endif
        DramcRxWindowPerbitCal((DRAMC_CTX_T *) p, 0, NULL);

        #ifdef DDR_INIT_TIME_PROFILING
        CPU_Cycle=TimeProfileEnd();
        mcSHOW_TIME_MSG(("\tRank %d RX RDDQC takes %d us\n", s1RankIdx, CPU_Cycle));
        TimeProfileBegin();
        #endif

        #if LJPLL_FREQ_DEBUG_LOG
            DDRPhyFreqMeter();
        #endif

#if MRW_CHECK_ONLY
        mcSHOW_MRW_MSG(("\n==[MR Dump] %s==\n", __func__));
#endif        
#if TX_K_DQM_WITH_WDBI
        //DramcWriteDBIOnOff() control both rank, need to recover for rank1 tx calibration
        DramcWriteDBIOnOff(p, 0);
#endif

        // DQM disable for DQ_ONLY calibration
        //DramcMRWriteFldAlign(p, 13, 1, MR13_DMD, TO_MR);

#if !ENABLE_DDR800_PHASE_MODE_INTERSECTION
        if (!(u1IsLP4Div4DDR800(p) && (p->rank==RANK_1) && (vGet_DDR800_Mode(p) == DDR800_SEMI_LOOP))) // skip for DDR800semi rank1, Phase rank0/1 using same RG.
#endif
            DramcTxWindowPerbitCal((DRAMC_CTX_T *) p, TX_DQ_DQS_MOVE_DQ_DQM, FALSE);  //Vref scan disable

        if(u1IsDDR800PhaseMode(p) == FALSE)
        {
            if (Get_Vref_Calibration_OnOff(p)==VREF_CALI_ON)
            {
                DramcTxWindowPerbitCal((DRAMC_CTX_T *) p, TX_DQ_DQS_MOVE_DQ_ONLY, TRUE); 
            }
#if PINMUX_AUTO_TEST_PER_BIT_TX
            CheckTxPinMux(p);         
#endif
            DramcTxWindowPerbitCal((DRAMC_CTX_T *) p, TX_DQ_DQS_MOVE_DQ_ONLY, FALSE);

            // Recover DQM(enalbe) after DQ_ONLY calibration
            //DramcMRWriteFldAlign(p, 13, 0, MR13_DMD, TO_MR);

#if TX_K_DQM_WITH_WDBI
            if ((p->DBI_W_onoff[p->dram_fsp]==DBI_ON))
            {
                // K DQM with DBI_ON, and check DQM window spec.
                //mcSHOW_DBG_MSG(("[TX_K_DQM_WITH_WDBI] Step1: K DQM with DBI_ON, and check DQM window spec.\n\n"));
                vSwitchWriteDBISettings(p, DBI_ON);
                DramcTxWindowPerbitCal((DRAMC_CTX_T *) p, TX_DQ_DQS_MOVE_DQM_ONLY, FALSE);
                vSwitchWriteDBISettings(p, DBI_OFF);
            }
#endif
        }
        if (doe_get_config("log_to_storage"))
        {
#if ENABLE_EYESCAN_GRAPH
        if(u1IsDDR800PhaseMode(p) == FALSE)
        {
            Dramc_K_TX_EyeScan_Log(p);
	    print_EYESCAN_LOG_message(p, 2); //draw TX eyescan
        }
#endif
        }
        #ifdef DDR_INIT_TIME_PROFILING
        CPU_Cycle=TimeProfileEnd();
        mcSHOW_TIME_MSG(("\tRank %d TX calibration takes %d us\n", s1RankIdx, CPU_Cycle));
        TimeProfileBegin();
        #endif

        #if LJPLL_FREQ_DEBUG_LOG
            DDRPhyFreqMeter();
        #endif

        DramcRxdatlatCal((DRAMC_CTX_T *) p);

        #ifdef DDR_INIT_TIME_PROFILING
        CPU_Cycle=TimeProfileEnd();
        mcSHOW_TIME_MSG(("\tRank %d Datlat takes %d us\n", s1RankIdx, CPU_Cycle));
        TimeProfileBegin();
        #endif

        #if LJPLL_FREQ_DEBUG_LOG
            DDRPhyFreqMeter();
        #endif
#if PINMUX_AUTO_TEST_PER_BIT_RX
        CheckRxPinMux(p);         
#endif
        DramcRxWindowPerbitCal((DRAMC_CTX_T *) p, 1, NULL);

        #ifdef DDR_INIT_TIME_PROFILING
        CPU_Cycle=TimeProfileEnd();
        mcSHOW_TIME_MSG(("\tRank %d RX calibration takes %d us\n", s1RankIdx, CPU_Cycle));
        TimeProfileBegin();
        #endif
       // DramcRxdqsGatingCal(p);

        if (doe_get_config("log_to_storage"))
        {
#if ENABLE_EYESCAN_GRAPH
        print_EYESCAN_LOG_message(p, 1); //draw RX eyescan
#endif
        }

#if TX_OE_CALIBATION
        if(p->frequency >=1600)
        {
            DramcTxOECalibration(p);
        }
#endif

        vAutoRefreshSwitch(p, DISABLE); //After gating, Rx and Tx calibration, auto refresh should be disable

        #if ENABLE_TX_TRACKING
        #if 0 /* Starting from Vinson, no need to pre-calculate MR23 for different freqs */
        if(gu1MR23Done==FALSE)
        {
            DramcDQSOSCAuto(p);
        }
        #endif
        DramcDQSOSCAuto(p);
        DramcDQSOSCMR23(p);
        DramcDQSOSCSetMR18MR19(p);
        #endif
    }
    
    #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    if(p->femmc_Ready==0)
    #endif
    {
        if(p->frequency >= RX_VREF_DUAL_RANK_K_FREQ)  // for 3733/4266
        {
            U8 u1ByteIdx, u1HighFreqRXVref[2];
            for(u1ByteIdx =0 ; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
            {
                u1HighFreqRXVref[u1ByteIdx] = (gFinalRXVrefDQ[p->channel][RANK_0][u1ByteIdx] + gFinalRXVrefDQ[p->channel][RANK_1][u1ByteIdx]) >> 1;
                mcSHOW_DBG_MSG(("RX Vref Byte%d (u1HighFreqRXVref) = %d = (%d+ %d)>>1\n", u1ByteIdx, u1HighFreqRXVref[u1ByteIdx], gFinalRXVrefDQ[p->channel][RANK_0][u1ByteIdx], gFinalRXVrefDQ[p->channel][RANK_1][u1ByteIdx]));
            }
            
            for(s1RankIdx=RANK_0; s1RankIdx < u1RankMax; s1RankIdx++)
            {
                vSetRank(p, s1RankIdx);
                DramcRxWindowPerbitCal((DRAMC_CTX_T *) p, 1, u1HighFreqRXVref);
            }
        }
    }
    
    vSetRank(p, RANK_0); // Set p's rank back to 0 (Avoids unexpected auto-rank offset calculation in u4RegBaseAddrTraslate())

    #if ENABLE_TX_TRACKING
    DramcDQSOSCShuSettings(p);
    #endif

#if GATING_ADJUST_TXDLY_FOR_TRACKING
    DramcRxdqsGatingPostProcess(p);
#endif

#if TDQSCK_PRECALCULATION_FOR_DVFS
    DramcDQSPrecalculation_preset(p);
#endif

    DramcDualRankRxdatlatCal(p);

#if RDSEL_TRACKING_EN
    if (p->frequency != 400)
        RDSELRunTimeTracking_preset(p);
#endif

#if XRTWTW_NEW_CROSS_RK_MODE
    if(p->support_rank_num == RANK_DUAL)
    {
        XRTWTW_SHU_Setting(p);
    }
#endif

#if LJPLL_FREQ_DEBUG_LOG
    DDRPhyFreqMeter();
#endif

    #ifdef DDR_INIT_TIME_PROFILING
    CPU_Cycle=TimeProfileEnd();
    mcSHOW_TIME_MSG(("\tMisc takes %d us\n\n", s1RankIdx, CPU_Cycle));
    #endif
}

static void vDramCalibrationSingleChannel(DRAMC_CTX_T *p)
{
#if !__ETT__
    /*
     * Since DRAM calibration will cost much time,
     * kick wdt here to prevent watchdog timeout.
     */
#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
    mtk_wdt_restart();
#endif
#endif

    vCalibration_Flow_LP4(p);
}

static void vDramCalibrationAllChannel(DRAMC_CTX_T *p)
{
    U8 channel_idx, rank_idx;

#ifdef DDR_INIT_TIME_PROFILING
    U32 u4low_tick0, u4high_tick0, u4low_tick1, u4high_tick1;
#if __ETT__
    u4low_tick0 = GPT_GetTickCount(&u4high_tick0);
#else
    u4low_tick0 = get_timer(0);
#endif
#endif

    for(channel_idx=CHANNEL_A; channel_idx<p->support_channel_num; channel_idx++)
    {
        vSetPHY2ChannelMapping(p, channel_idx);// when switching channel, must update PHY to Channel Mapping
        vDramCalibrationSingleChannel(p);
    }

    vSetPHY2ChannelMapping(p, CHANNEL_A);

#if PRINT_CALIBRATION_SUMMARY
    vPrintCalibrationResult(p);
#endif

#ifdef FOR_HQA_TEST_USED
    #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    if(p->femmc_Ready==1)
    {
        mcSHOW_DBG_MSG(("\nCalibration fast K is enable, cannot show HQA measurement information\n"));
    }
    else
    #endif
    print_HQA_measure_message(p);
#endif

    /* Enable/Disable calibrated rank's DBI function accordingly */
#if ENABLE_READ_DBI
	//Read DBI ON
	vSetRank(p, RANK_0);
	vSetPHY2ChannelMapping(p, CHANNEL_A);

	DramcReadDBIOnOff(p, p->DBI_R_onoff[p->dram_fsp]);
#endif

#if ENABLE_WRITE_DBI
    //Write DBI ON
    for(channel_idx=CHANNEL_A; channel_idx<p->support_channel_num; channel_idx++)
    {
        vSetPHY2ChannelMapping(p, channel_idx);

        for(rank_idx=RANK_0; rank_idx<RANK_MAX; rank_idx++)
        {
            vSetRank(p, rank_idx);
            DramcWriteMinus1MCKForWriteDBI(p, -8); //Tx DQ/DQM -1 MCK for write DBI ON
        }
        vSetRank(p, RANK_0);
    }
    vSetPHY2ChannelMapping(p, CHANNEL_A);

    DramcWriteDBIOnOff(p, p->DBI_W_onoff[p->dram_fsp]);

    // Improve Write DBI Power
    ApplyWriteDBIPowerImprove(p, ENABLE);

    #if ENABLE_WRITE_DBI_Protect
    ApplyWriteDBIProtect(p, ENABLE);
    #endif
#endif

#if TX_PICG_NEW_MODE
    TXPICGSetting(p);
#endif

#ifdef DUMMY_READ_FOR_DQS_GATING_RETRY
    DummyReadForDqsGatingRetryShuffle(p, 1);
#else
    DummyReadForDqsGatingRetryShuffle(p, 0);
#endif

#if XRTRTR_NEW_CROSS_RK_MODE
if(p->support_rank_num == RANK_DUAL)
{
	XRTRTR_SHU_Setting(p);
}
#endif

#if (ENABLE_TX_TRACKING || TDQSCK_PRECALCULATION_FOR_DVFS)
    FreqJumpRatioCalculation(p);
#endif
    
#if ENABLE_TX_TRACKING
    U8 backup_channel= p->channel;
    U8 channelIdx;
    	
    for(channelIdx=CHANNEL_A; channelIdx<p->support_channel_num; channelIdx++)
    {
        vSetPHY2ChannelMapping(p, channelIdx);
        DramcHwDQSOSC(p);
    }
    	
    vSetPHY2ChannelMapping(p, backup_channel);
    mcSHOW_DBG_MSG(("TX_TRACKING: ON\n"));
#else
    mcSHOW_DBG_MSG(("TX_TRACKING: OFF\n"));
#endif

    SetMr13VrcgToNormalOperationShuffle(p);//Set VRCG{MR13[3]} to 0 while DVFS, Ready to copy to SDRAM
    
#ifdef DDR_INIT_TIME_PROFILING
#if __ETT__
    u4low_tick1 = GPT_GetTickCount(&u4high_tick1);
    mcSHOW_TIME_MSG(("  (4) vDramCalibrationAllChannel() take %d ms\n\r",((u4low_tick1-u4low_tick0)*76)/1000000));
#else
    u4low_tick1 = get_timer(u4low_tick0);
    mcSHOW_TIME_MSG(("  (4) vDramCalibrationAllChannel() take %d ms\n\r",u4low_tick1));
#endif
#endif

}


static void vCalibration_Flow_For_MDL(DRAMC_CTX_T *p)
{
    U8 u1RankMax;
    S8 s1RankIdx;

#if GATING_ADJUST_TXDLY_FOR_TRACKING
    DramcRxdqsGatingPreProcess(p);
#endif

    if (p->support_rank_num==RANK_DUAL)
        u1RankMax = RANK_MAX;
    else
        u1RankMax = RANK_1;

    for(s1RankIdx=RANK_0; s1RankIdx<u1RankMax; s1RankIdx++)
    {
        vSetRank(p, s1RankIdx);
        
        vAutoRefreshSwitch(p, ENABLE); //when doing gating, RX and TX calibration, auto refresh should be enable
        DramcRxdqsGatingCal(p);
        DramcRxWindowPerbitCal((DRAMC_CTX_T *) p, 0, NULL);

#if MRW_CHECK_ONLY
        mcSHOW_MRW_MSG(("\n==[MR Dump] %s==\n", __func__));
#endif        
        vAutoRefreshSwitch(p, DISABLE); //After gating, Rx and Tx calibration, auto refresh should be disable
    }
    
    vSetRank(p, RANK_0); // Set p's rank back to 0 (Avoids unexpected auto-rank offset calculation in u4RegBaseAddrTraslate())

#if GATING_ADJUST_TXDLY_FOR_TRACKING
    DramcRxdqsGatingPostProcess(p);
#endif
}


int GetDramInforAfterCalByMRR(DRAMC_CTX_T *p, DRAM_INFO_BY_MRR_T *DramInfo)
{
    U8 u1ChannelIdx, u1RankIdx, u1RankMax, u1DieNumber=0;
    U16 u2Density;
    U64 u8Size = 0, u8Size_backup = 0;
    U64 u8ChannelSize;
    U32 u4ChannelNumber=1, u4RankNumber=1;

    if (p->revision_id != REVISION_ID_MAGIC)
        return 0;

    vSetPHY2ChannelMapping(p, CHANNEL_A);

    // Read MR5 for Vendor ID
    DramcModeRegReadByRank(p, RANK_0, 5, &(p->vendor_id));// for byte mode, don't show value of another die.
    p->vendor_id &= 0xFF;
    mcSHOW_DBG_MSG(("[GetDramInforAfterCalByMRR] Vendor %x.\n", p->vendor_id));
    // Read MR6 for Revision ID
    DramcModeRegReadByRank(p, RANK_0, 6, &(p->revision_id));// for byte mode, don't show value of another die.
    mcSHOW_DBG_MSG(("[GetDramInforAfterCalByMRR] Revision %x.\n", p->revision_id));

    if(DramInfo != NULL)
    {
        DramInfo->u2MR5VendorID = p->vendor_id;
        DramInfo->u2MR6RevisionID = p->revision_id;

        for(u1RankIdx =0; u1RankIdx<RANK_MAX; u1RankIdx++)
            DramInfo->u8MR8RankSize[u1RankIdx] =0;
    }

    // Read MR8 for dram density
    for(u1RankIdx =0; u1RankIdx<(p->support_rank_num); u1RankIdx++)
    {
        #if 0//PRINT_CALIBRATION_SUMMARY
        if((p->aru4CalExecuteFlag[u1ChannelIdx][u1RankIdx] !=0)  && \
            (p->aru4CalResultFlag[u1ChannelIdx][u1RankIdx]==0))
        #endif
        {
            DramcModeRegReadByRank(p, u1RankIdx, 0, &(gu2MR0_Value[u1RankIdx]));
            mcSHOW_DBG_MSG(("MR0 0x%x\n", gu2MR0_Value[u1RankIdx]));
            
            DramcModeRegReadByRank(p, u1RankIdx, 8, &u2Density);
            mcSHOW_DBG_MSG(("MR8 0x%x\n", u2Density));

            u1DieNumber =1;
            if(((u2Density >> 6) & 0x3) == 1) //OP[7:6] =0, x16 (normal mode)
                u1DieNumber =2;

            u2Density = (u2Density>>2)&0xf;

            switch(u2Density)
            {
                ///TODO: Darren, please check the value of u8Size.
                case 0x0:
                    u8Size = 0x20000000;  //4Gb = 512MB
                    //mcSHOW_DBG_MSG(("[EMI]DRAM density = 4Gb\n"));
                    break;
                case 0x1:
                    u8Size = 0x30000000;  //6Gb = 768MB
                    //mcSHOW_DBG_MSG(("[EMI]DRAM density = 6Gb\n"));
                    break;
                case 0x2:
                    u8Size = 0x40000000;  //8Gb = 1GB = 2^30 bytes = 0x40000000 bytes
                    //mcSHOW_DBG_MSG(("[EMI]DRAM density = 8Gb\n"));
                    break;
                case 0x3:
                    u8Size = 0x60000000;  //12Gb = 1.5GB = 3^30 bytes = 0x60000000 bytes
                    //mcSHOW_DBG_MSG(("[EMI]DRAM density = 12Gb\n"));
                    break;
                case 0x4:
                    u8Size = 0x80000000;  //16Gb = 2GB = 4^30 bytes = 0x80000000 bytes
                    //mcSHOW_DBG_MSG(("[EMI]DRAM density = 16Gb\n"));
                    break;
                case 0x5:
                    u8Size = 0xc0000000; //24Gb = 3GB = 6^30 bytes = 0xc0000000 bytes
                    //mcSHOW_DBG_MSG(("[EMI]DRAM density = 24Gb\n"));
                    break;                            
                case 0x6:
                    u8Size = 0x100000000L; //32Gb = 4GB = 8^30 bytes = 0x10000000 bytes
                    //mcSHOW_DBG_MSG(("[EMI]DRAM density = 32Gb\n"));
                    break;
                default:
                    u8Size = 0; //reserved
            }

            if (u8Size_backup < u8Size) // find max dram size for vDramcACTimingOptimize
            {
                u8Size_backup = u8Size;
                p->density = u2Density;
            }

            p->ranksize[u1RankIdx] = u8Size * u1DieNumber;  //dram rank size = density * DieNumber
            
            if(DramInfo != NULL)
            {
                DramInfo->u8MR8RankSize[u1RankIdx] = p->ranksize[u1RankIdx];
            }
        }
        // 1GB = 2^30 bytes
        // u8Size * (2^3) / (2^30)  ==>Gb
        mcSHOW_DBG_MSG(("RK%d, DieNum %d, Density %dGb, RKsize %dGb.\n\n", u1RankIdx, u1DieNumber, (U32)(u8Size>>27), (U32)(p->ranksize[u1RankIdx]>>27)));
    }

    return 0;
}


#if ENABLE_RANK_NUMBER_AUTO_DETECTION
void DramRankNumberDetection(DRAMC_CTX_T *p)
{
    U8 u1RankBak;

    u1RankBak = u1GetRank(p);  // backup current rank setting

    vSetPHY2ChannelMapping(p, CHANNEL_A); // when switching channel, must update PHY to Channel Mapping
    vSetRank(p, RANK_1);

    if(DramcWriteLeveling((DRAMC_CTX_T *) p) == DRAM_OK)//Dram will be reset when finish write leveling
    {
        p->support_rank_num = RANK_DUAL;
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RSTMASK), 0, RSTMASK_RSV_DRAM_SUPPORT_RANK_NUM);  //keep support_rank_num to reserved rg
    }
    else
    {
        p->support_rank_num = RANK_SINGLE;
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RSTMASK), 1, RSTMASK_RSV_DRAM_SUPPORT_RANK_NUM);  //keep support_rank_num to reserved rg
    }
    mcSHOW_DBG_MSG(("[RankNumberDetection] %d\n", p->support_rank_num));

    vSetRank(p, u1RankBak);  // restore rank setting
}
#endif

U8 gPRE_MIOCK_JMETER_HQA_USED_flag=0;
void Set_PRE_MIOCK_JMETER_HQA_USED_flag(U8 value)
{
    gPRE_MIOCK_JMETER_HQA_USED_flag = value;
}
U8 Get_PRE_MIOCK_JMETER_HQA_USED_flag(U8 value)
{
    return gPRE_MIOCK_JMETER_HQA_USED_flag;
}

#ifdef ENABLE_MIOCK_JMETER
extern int hqa_vmddr_class;   
void Get_RX_DelayCell(DRAMC_CTX_T *p)
{
#if defined(FOR_HQA_REPORT_USED) && (FOR_DV_SIMULATION_USED==0) && (SW_CHANGE_FOR_SIMULATION==0)
        if (gHQALOG_RX_delay_cell_ps_075V == 0)
        { 
#if defined(__ETT__)        
            mcSHOW_DBG_MSG(("RX delay cell calibration (%d):\n", hqa_vmddr_class));
        
            switch (hqa_vmddr_class)
            {            
                case 1:
                    dramc_set_vcore_voltage(_SEL_PREFIX(VMDDR, HV, LP4));
                    break;   
                case 2:
                    dramc_set_vcore_voltage(_SEL_PREFIX(VMDDR, NV, LP4));
                    break;
                case 3:
                    dramc_set_vcore_voltage(_SEL_PREFIX(VMDDR, LV, LP4));
                    break;                
            }
#else
            // set vcore to RX used 0.75V
            dramc_set_vcore_voltage(SEL_PREFIX_VMDDR);  //set vmddr voltage to vcore to K RX delay cell
#endif
    
            DramcMiockJmeter(p);
    
            gHQALOG_RX_delay_cell_ps_075V = u2gdelay_cell_ps;
    
            // set vocre back
            vSetVcoreByFreq(p);
        }
#endif
}

void PRE_MIOCK_JMETER_HQA_USED(DRAMC_CTX_T *p)
{
    U32 backup_freq_sel, backup_channel;
    U32 channel_idx, shuffleIdx;

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    if(p->femmc_Ready==1)
    {
        for(channel_idx=CHANNEL_A; channel_idx<p->support_channel_num; channel_idx++)
        {
            //for (shuffleIdx = DRAM_DFS_SHUFFLE_1; shuffleIdx < DRAM_DFS_SHUFFLE_MAX; shuffleIdx++)
            {
                ucg_num_dlycell_perT_all[p->shu_type][channel_idx] = p->pSavetimeData->ucnum_dlycell_perT;
                u2gdelay_cell_ps_all[p->shu_type][channel_idx] = p->pSavetimeData->u2DelayCellTimex100;
            }
        }

        p->ucnum_dlycell_perT = p->pSavetimeData->ucnum_dlycell_perT;
        p->u2DelayCellTimex100 = p->pSavetimeData->u2DelayCellTimex100;
        return;
    }
#endif


    backup_freq_sel = vGet_PLL_FreqSel(p);
    backup_channel = p->channel;

    mcSHOW_DBG_MSG3(("[JMETER_HQA]\n"));
    Set_PRE_MIOCK_JMETER_HQA_USED_flag(1);
    
    vSetPHY2ChannelMapping(p, CHANNEL_A);
   
    DramcMiockJmeterHQA(p);
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    if(p->femmc_Ready==0)
    {
        #if 0
        for(channel_idx=CHANNEL_A; channel_idx<p->support_channel_num; channel_idx++)
        {
            for (shuffleIdx = DRAM_DFS_SHUFFLE_1; shuffleIdx < DRAM_DFS_SHUFFLE_MAX; shuffleIdx++)
            {
                p->pSavetimeData->ucg_num_dlycell_perT_all[channel_idx] = ucg_num_dlycell_perT_all[p->shu_type][channel_idx];
                p->pSavetimeData->u2gdelay_cell_ps_all[channel_idx] = u2gdelay_cell_ps_all[p->shu_type][channel_idx];
            }
        }
        #endif
        p->pSavetimeData->ucnum_dlycell_perT = p->ucnum_dlycell_perT;
        p->pSavetimeData->u2DelayCellTimex100 = p->u2DelayCellTimex100;
    }
#endif
    vSetPHY2ChannelMapping(p, backup_channel);

    Set_PRE_MIOCK_JMETER_HQA_USED_flag(0);

    vSet_PLL_FreqSel(p, backup_freq_sel);
}
#endif

#if RUNTIME_SHMOO_RELEATED_FUNCTION && SUPPORT_SAVE_TIME_FOR_CALIBRATION
#define RUNTIME_SHMOO_ON_GOING  0x00
#define RUNTIME_SHMOO_END       0xff
void RunTime_Shmoo_update_parameters(DRAMC_CTX_T *p)
{
    U8 backup_channel, backup_rank;
    U16 tx_pi_delay, tx_dqm_pi_delay;
    S16 rx_first_delay=0x1fff, rx_first_delay_bit, rx_last_delay=-0x1fff, rx_last_delay_bit;
    U8 u1BitIdx;
    U32 u4value;
    PASS_WIN_DATA_T FinalWinPerBit[DQ_DATA_WIDTH];
    U8 ui_large_value, ui_small_value, pi_value;
    U8 ui_dqm_large_value, ui_dqm_small_value, pi_dqm_value;
#if 0
    U8 ui_oen_large_value, ui_oen_small_value, pi_oen_value;
    U8 ui_dqm_oen_large_value, ui_dqm_oen_small_value, pi_dqm_oen_value;
#endif
    U8 scan_invert_flag=0;

    backup_channel = p->channel;
    backup_rank = p->rank;

    p->channel = RUNTIME_SHMOO_TEST_CHANNEL;
    p->rank = RUNTIME_SHMOO_TEST_RANK;

#if RUNTIME_SHMOO_TX
    if (RUNTIME_SHMOO_TEST_BYTE == 0)
    {
        tx_pi_delay = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0), SHURK0_SELPH_DQ0_TXDLY_DQ0) * 256 +
                      u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ2), SHURK0_SELPH_DQ2_DLY_DQ0) * 32 +
                      u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ7), SHU_R0_B0_DQ7_RK0_ARPI_DQ_B0);

        tx_dqm_pi_delay = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ1), SHURK0_SELPH_DQ1_TXDLY_DQM0) * 256 +
                          u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ3), SHURK0_SELPH_DQ3_DLY_DQM0) * 32 +
                          u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ7), SHU_R0_B0_DQ7_RK0_ARPI_DQM_B0);
    }
    else
    {
        tx_pi_delay = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0), SHURK0_SELPH_DQ0_TXDLY_DQ1) * 256 +
                      u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ2), SHURK0_SELPH_DQ2_DLY_DQ1) * 32 +
                      u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ7), SHU_R0_B1_DQ7_RK0_ARPI_DQ_B1);

        tx_dqm_pi_delay = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ1), SHURK0_SELPH_DQ1_TXDLY_DQM1) * 256 +
                          u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ3), SHURK0_SELPH_DQ3_DLY_DQM1) * 32 +
                          u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ7), SHU_R0_B1_DQ7_RK0_ARPI_DQM_B1);
    }
#endif

#if RUNTIME_SHMOO_RX
//    vAutoRefreshSwitch(p, ENABLE); 

    for (u1BitIdx=0; u1BitIdx<8; u1BitIdx++)
	{
    	FinalWinPerBit[u1BitIdx].first_pass=p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[p->channel][p->rank][u1BitIdx+RUNTIME_SHMOO_TEST_BYTE*8];
    	FinalWinPerBit[u1BitIdx].last_pass=p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[p->channel][p->rank][u1BitIdx+RUNTIME_SHMOO_TEST_BYTE*8];
	}

    //find smallest first and largest last pass
    for (u1BitIdx=0; u1BitIdx<8; u1BitIdx++)
    {
        if (FinalWinPerBit[u1BitIdx].first_pass < rx_first_delay) 
        {
            rx_first_delay = FinalWinPerBit[u1BitIdx].first_pass;
            rx_first_delay_bit = u1BitIdx;
        }
        if (FinalWinPerBit[u1BitIdx].last_pass > rx_last_delay) 
        {
            rx_last_delay = FinalWinPerBit[u1BitIdx].last_pass;        
            rx_last_delay_bit = u1BitIdx;
        }
    }
#endif

#if __ETT__
    mcSHOW_ERR_MSG(("fra dramc_get_rshmoo_pass_cnt = %x\n",dramc_get_rshmoo_pass_cnt()));
    mcSHOW_ERR_MSG(("fra femmc_Ready = %d ==\n",p->femmc_Ready));
#else
    print("fra dramc_get_rshmoo_pass_cnt = %x\n",dramc_get_rshmoo_pass_cnt());
    print("fra femmc_Ready = %d ==\n",p->femmc_Ready);
#endif

    if (p->femmc_Ready==0 ||
 ((p->pSavetimeData->Runtime_Shmoo_para.TX_Channel!=RUNTIME_SHMOO_TEST_CHANNEL) || (p->pSavetimeData->Runtime_Shmoo_para.TX_Rank!=RUNTIME_SHMOO_TEST_RANK) || (p->pSavetimeData->Runtime_Shmoo_para.TX_Byte!=RUNTIME_SHMOO_TEST_BYTE))) //first K
    {
#if RUNTIME_SHMOO_TX    
        p->pSavetimeData->Runtime_Shmoo_para.flag= RUNTIME_SHMOO_ON_GOING; //on going
        p->pSavetimeData->Runtime_Shmoo_para.Scan_Direction=0;
        p->pSavetimeData->Runtime_Shmoo_para.TX_PI_delay = tx_pi_delay-32+RUNTIME_SHMOO_TEST_PI_DELAY_START;
        p->pSavetimeData->Runtime_Shmoo_para.TX_Original_PI_delay = p->pSavetimeData->Runtime_Shmoo_para.TX_PI_delay;
        p->pSavetimeData->Runtime_Shmoo_para.TX_DQM_PI_delay = tx_dqm_pi_delay-32+RUNTIME_SHMOO_TEST_PI_DELAY_START;
        p->pSavetimeData->Runtime_Shmoo_para.TX_Original_DQM_PI_delay = p->pSavetimeData->Runtime_Shmoo_para.TX_DQM_PI_delay;
        if (RUNTIME_SHMOO_TEST_VREF_START<51)
        {
            p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Range = 0;
            p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Value = RUNTIME_SHMOO_TEST_VREF_START;
        }
        else
        {
            p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Range = 1;
            p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Value = RUNTIME_SHMOO_TEST_VREF_START-51+21;
        }
        p->pSavetimeData->Runtime_Shmoo_para.TX_Channel = RUNTIME_SHMOO_TEST_CHANNEL;
        p->pSavetimeData->Runtime_Shmoo_para.TX_Rank = RUNTIME_SHMOO_TEST_RANK;
        p->pSavetimeData->Runtime_Shmoo_para.TX_Byte = RUNTIME_SHMOO_TEST_BYTE;
#endif

#if RUNTIME_SHMOO_RX
        p->pSavetimeData->Runtime_Shmoo_para.flag= RUNTIME_SHMOO_ON_GOING; //on going
        p->pSavetimeData->Runtime_Shmoo_para.Scan_Direction=0;
        p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Range = 0;
        p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Value = 0;
        for (u1BitIdx=0; u1BitIdx<8; u1BitIdx++)
    	{
    	    p->pSavetimeData->Runtime_Shmoo_para.RX_delay[u1BitIdx] = FinalWinPerBit[u1BitIdx].first_pass- RUNTIME_SHMOO_RX_TEST_MARGIN;    
        }
        p->pSavetimeData->Runtime_Shmoo_para.RX_Original_delay = p->pSavetimeData->Runtime_Shmoo_para.RX_delay[0];                
        p->pSavetimeData->Runtime_Shmoo_para.TX_Channel = RUNTIME_SHMOO_TEST_CHANNEL;           
        p->pSavetimeData->Runtime_Shmoo_para.TX_Rank = RUNTIME_SHMOO_TEST_RANK;           
        p->pSavetimeData->Runtime_Shmoo_para.TX_Byte = RUNTIME_SHMOO_TEST_BYTE;             
#endif
    }
//fra    else  if (dramc_get_rshmoo_step())
    else if ((dramc_get_rshmoo_step()) && (p->pSavetimeData->Runtime_Shmoo_para.flag != RUNTIME_SHMOO_END))
    {
        //judge scan direction
        if (RUNTIME_SHMOO_FAST_K == 0)
        {
            p->pSavetimeData->Runtime_Shmoo_para.Scan_Direction=0;
        }
        else
        {
            if (dramc_get_rshmoo_pass_cnt() > (RUNTIME_SHMOO_FAST_K-1))
            {
                if (p->pSavetimeData->Runtime_Shmoo_para.Scan_Direction==0) 
                {
                    p->pSavetimeData->Runtime_Shmoo_para.Scan_Direction=1;
                }
                else 
                {
                    p->pSavetimeData->Runtime_Shmoo_para.Scan_Direction=0;
                }

                scan_invert_flag = 1;
            }
    }

        if (scan_invert_flag == 0)
        {
            if (p->pSavetimeData->Runtime_Shmoo_para.Scan_Direction==0)
            {
#if RUNTIME_SHMOO_TX            
        p->pSavetimeData->Runtime_Shmoo_para.TX_PI_delay += RUNTIME_SHMOO_TEST_PI_DELAY_STEP;
        if (p->pSavetimeData->Runtime_Shmoo_para.TX_PI_delay > p->pSavetimeData->Runtime_Shmoo_para.TX_Original_PI_delay+RUNTIME_SHMOO_TEST_PI_DELAY_END)
        {
            p->pSavetimeData->Runtime_Shmoo_para.TX_PI_delay = p->pSavetimeData->Runtime_Shmoo_para.TX_Original_PI_delay;
            p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Value += RUNTIME_SHMOO_TEST_VREF_STEP;

            if ((p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Value+p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Range*30) > RUNTIME_SHMOO_TEST_VREF_END)
            {
                p->pSavetimeData->Runtime_Shmoo_para.flag= RUNTIME_SHMOO_END; //test finish
            }
            else
            {
                if (p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Range==0)
                {
                    if (p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Value > 50)
                    {
                        p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Range = 1;
                        p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Value -= 30;
                    }
                }
            }
        }

        p->pSavetimeData->Runtime_Shmoo_para.TX_DQM_PI_delay += RUNTIME_SHMOO_TEST_PI_DELAY_STEP;
        if (p->pSavetimeData->Runtime_Shmoo_para.TX_DQM_PI_delay > p->pSavetimeData->Runtime_Shmoo_para.TX_Original_DQM_PI_delay+RUNTIME_SHMOO_TEST_PI_DELAY_END)
        {
            p->pSavetimeData->Runtime_Shmoo_para.TX_DQM_PI_delay = p->pSavetimeData->Runtime_Shmoo_para.TX_Original_DQM_PI_delay;
        }
#endif

#if RUNTIME_SHMOO_RX
                for (u1BitIdx=0; u1BitIdx<8; u1BitIdx++)
                {
                    p->pSavetimeData->Runtime_Shmoo_para.RX_delay[u1BitIdx] += RUNTIME_SHMOO_TEST_PI_DELAY_STEP;
                }
                if (p->pSavetimeData->Runtime_Shmoo_para.RX_delay[rx_last_delay_bit] > rx_last_delay+RUNTIME_SHMOO_RX_TEST_MARGIN)
                {
                    for (u1BitIdx=0; u1BitIdx<8; u1BitIdx++)
                    {
                        p->pSavetimeData->Runtime_Shmoo_para.RX_delay[u1BitIdx] = FinalWinPerBit[u1BitIdx].first_pass - RUNTIME_SHMOO_RX_TEST_MARGIN;
                    }
                    p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Value += RUNTIME_SHMOO_TEST_VREF_STEP;                
                    if ((p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Value) > RUNTIME_SHMOO_RX_VREF_RANGE_END)
                    {
                        p->pSavetimeData->Runtime_Shmoo_para.flag= RUNTIME_SHMOO_END; //test finish
                    }                    
                }
#endif
    }
            else
            {
#if RUNTIME_SHMOO_TX            
                p->pSavetimeData->Runtime_Shmoo_para.TX_PI_delay -= RUNTIME_SHMOO_TEST_PI_DELAY_STEP;
                if (p->pSavetimeData->Runtime_Shmoo_para.TX_PI_delay < p->pSavetimeData->Runtime_Shmoo_para.TX_Original_PI_delay)
                {
                    p->pSavetimeData->Runtime_Shmoo_para.TX_PI_delay = p->pSavetimeData->Runtime_Shmoo_para.TX_Original_PI_delay + (RUNTIME_SHMOO_TEST_PI_DELAY_END+1);
                    p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Value += RUNTIME_SHMOO_TEST_VREF_STEP;

                    if ((p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Value+p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Range*30) > RUNTIME_SHMOO_TEST_VREF_END)
                    {
                        p->pSavetimeData->Runtime_Shmoo_para.flag= RUNTIME_SHMOO_END; //test finish
                    }
                    else
                    {
                        if (p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Range==0)
                        {
                            if (p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Value > 50)
                            {
                                p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Range = 1;
                                p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Value -= 30;
                            }
                        }
                    }           
                }

                p->pSavetimeData->Runtime_Shmoo_para.TX_DQM_PI_delay -= RUNTIME_SHMOO_TEST_PI_DELAY_STEP;
                if (p->pSavetimeData->Runtime_Shmoo_para.TX_DQM_PI_delay < p->pSavetimeData->Runtime_Shmoo_para.TX_Original_DQM_PI_delay)
                {
                    p->pSavetimeData->Runtime_Shmoo_para.TX_DQM_PI_delay = p->pSavetimeData->Runtime_Shmoo_para.TX_Original_DQM_PI_delay + (RUNTIME_SHMOO_TEST_PI_DELAY_END+1);        
                }            
#endif

#if RUNTIME_SHMOO_RX
                for (u1BitIdx=0; u1BitIdx<8; u1BitIdx++)
                {
                    p->pSavetimeData->Runtime_Shmoo_para.RX_delay[u1BitIdx] -= RUNTIME_SHMOO_TEST_PI_DELAY_STEP;
                }
                if (p->pSavetimeData->Runtime_Shmoo_para.RX_delay[rx_first_delay_bit] < rx_first_delay-RUNTIME_SHMOO_RX_TEST_MARGIN)
                {
                    for (u1BitIdx=0; u1BitIdx<8; u1BitIdx++)
                    {
                        p->pSavetimeData->Runtime_Shmoo_para.RX_delay[u1BitIdx] = FinalWinPerBit[u1BitIdx].last_pass + RUNTIME_SHMOO_RX_TEST_MARGIN;
                    }
                    p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Value += RUNTIME_SHMOO_TEST_VREF_STEP;                
                    if ((p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Value) > RUNTIME_SHMOO_RX_VREF_RANGE_END)
                    {
                        p->pSavetimeData->Runtime_Shmoo_para.flag= RUNTIME_SHMOO_END; //test finish
                    }                    
                }
#endif

            }
        }
        else
        {
            if (p->pSavetimeData->Runtime_Shmoo_para.Scan_Direction==0)
            {
#if RUNTIME_SHMOO_TX                    
                p->pSavetimeData->Runtime_Shmoo_para.TX_PI_delay = p->pSavetimeData->Runtime_Shmoo_para.TX_Original_PI_delay;
                p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Value += RUNTIME_SHMOO_TEST_VREF_STEP;

                if ((p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Value+p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Range*30) > RUNTIME_SHMOO_TEST_VREF_END)
                {
                    p->pSavetimeData->Runtime_Shmoo_para.flag= RUNTIME_SHMOO_END; //test finish
                }
                else
                {
                    if (p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Range==0)
                    {
                        if (p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Value > 50)
                        {
                            p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Range = 1;
                            p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Value -= 30;
                        }
                    }
                }    

                p->pSavetimeData->Runtime_Shmoo_para.TX_DQM_PI_delay = p->pSavetimeData->Runtime_Shmoo_para.TX_Original_DQM_PI_delay;        
#endif

#if RUNTIME_SHMOO_RX        
                for (u1BitIdx=0; u1BitIdx<8; u1BitIdx++)
                {
                    p->pSavetimeData->Runtime_Shmoo_para.RX_delay[u1BitIdx] = FinalWinPerBit[u1BitIdx].first_pass - RUNTIME_SHMOO_RX_TEST_MARGIN;
                }
                p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Value += RUNTIME_SHMOO_TEST_VREF_STEP;

                if ((p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Value) > RUNTIME_SHMOO_RX_VREF_RANGE_END)
                {
                    p->pSavetimeData->Runtime_Shmoo_para.flag= RUNTIME_SHMOO_END; //test finish
                } 
#endif
            }
            else
            {
#if RUNTIME_SHMOO_TX            
                p->pSavetimeData->Runtime_Shmoo_para.TX_PI_delay = p->pSavetimeData->Runtime_Shmoo_para.TX_Original_PI_delay + (RUNTIME_SHMOO_TEST_PI_DELAY_END+1);
                //p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Value += RUNTIME_SHMOO_TEST_VREF_STEP;

                p->pSavetimeData->Runtime_Shmoo_para.TX_DQM_PI_delay = p->pSavetimeData->Runtime_Shmoo_para.TX_Original_DQM_PI_delay + (RUNTIME_SHMOO_TEST_PI_DELAY_END+1);        
#endif
            
#if RUNTIME_SHMOO_RX
                for (u1BitIdx=0; u1BitIdx<8; u1BitIdx++)
                {
                    p->pSavetimeData->Runtime_Shmoo_para.RX_delay[u1BitIdx] = FinalWinPerBit[u1BitIdx].last_pass + RUNTIME_SHMOO_RX_TEST_MARGIN;
                }
#endif
            }
        }
    }

#if __ETT__
    mcSHOW_ERR_MSG(("Fra RunTime Shmoo CH%d, Rank%d, Byte%d\n",RUNTIME_SHMOO_TEST_CHANNEL, RUNTIME_SHMOO_TEST_RANK, RUNTIME_SHMOO_TEST_BYTE ));
#else
    print("Fra RunTime Shmoo CH%d, Rank%d, Byte%d\n",RUNTIME_SHMOO_TEST_CHANNEL, RUNTIME_SHMOO_TEST_RANK, RUNTIME_SHMOO_TEST_BYTE );
#endif

//fra    if (p->pSavetimeData->Runtime_Shmoo_para.flag != RUNTIME_SHMOO_END)
    {
#if RUNTIME_SHMOO_TX    
#if __ETT__
        mcSHOW_ERR_MSG(("Fra RunTime Shmoo original K TX Vref = (%d, %d)\n", (u1MR14Value[RUNTIME_SHMOO_TEST_CHANNEL][RUNTIME_SHMOO_TEST_RANK][p->dram_fsp]>>6) & 1, u1MR14Value[RUNTIME_SHMOO_TEST_CHANNEL][RUNTIME_SHMOO_TEST_RANK][p->dram_fsp] & 0x3f));
        mcSHOW_ERR_MSG(("Fra RunTime Shmoo original K TX Byte%d PI Delay = %d\n", RUNTIME_SHMOO_TEST_BYTE, p->pSavetimeData->Runtime_Shmoo_para.TX_Original_PI_delay+32-RUNTIME_SHMOO_TEST_PI_DELAY_START));

        mcSHOW_ERR_MSG(("Fra RunTime Shmoo TX Vref = (%d, %d)\n", p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Range, p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Value));
        mcSHOW_ERR_MSG(("Fra RunTime Shmoo TX Byte%d PI Delay = %d\n", RUNTIME_SHMOO_TEST_BYTE, p->pSavetimeData->Runtime_Shmoo_para.TX_PI_delay-p->pSavetimeData->Runtime_Shmoo_para.TX_Original_PI_delay));
#else
        print("Fra RunTime Shmoo original K TX Vref = (%d, %d)\n", (u1MR14Value[RUNTIME_SHMOO_TEST_CHANNEL][RUNTIME_SHMOO_TEST_RANK][p->dram_fsp]>>6) & 1, u1MR14Value[RUNTIME_SHMOO_TEST_CHANNEL][RUNTIME_SHMOO_TEST_RANK][p->dram_fsp] & 0x3f);
        print("Fra RunTime Shmoo original K TX Byte%d PI Delay = %d\n", RUNTIME_SHMOO_TEST_BYTE, p->pSavetimeData->Runtime_Shmoo_para.TX_Original_PI_delay+32-RUNTIME_SHMOO_TEST_PI_DELAY_START);

        print("Fra RunTime Shmoo TX Vref = (%d, %d)\n", p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Range, p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Value);
        print("Fra RunTime Shmoo TX Byte%d PI Delay = %d\n", RUNTIME_SHMOO_TEST_BYTE, p->pSavetimeData->Runtime_Shmoo_para.TX_PI_delay-p->pSavetimeData->Runtime_Shmoo_para.TX_Original_PI_delay);
#endif
#endif

#if RUNTIME_SHMOO_RX
        mcSHOW_ERR_MSG(("Fra RunTime Shmoo K RX Vref = %d\n", p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Value));
        mcSHOW_ERR_MSG(("Fra RunTime Shmoo RX Byte%d Delay = %d\n", RUNTIME_SHMOO_TEST_BYTE, p->pSavetimeData->Runtime_Shmoo_para.RX_delay[0]));
#endif

#if RUNTIME_SHMOO_TX    
        TxWinTransferDelayToUIPI(p, TX_DQ_DQS_MOVE_DQ_DQM, p->pSavetimeData->Runtime_Shmoo_para.TX_PI_delay, 1, &ui_large_value, &ui_small_value, &pi_value);
        TxWinTransferDelayToUIPI(p, TX_DQ_DQS_MOVE_DQ_DQM, p->pSavetimeData->Runtime_Shmoo_para.TX_DQM_PI_delay, 1, &ui_dqm_large_value, &ui_dqm_small_value, &pi_dqm_value);

        if (RUNTIME_SHMOO_TEST_BYTE == 0)
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0), ui_large_value, SHURK0_SELPH_DQ0_TXDLY_DQ0);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ2), ui_small_value, SHURK0_SELPH_DQ2_DLY_DQ0);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ7), pi_value, SHU_R0_B0_DQ7_RK0_ARPI_DQ_B0);

            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ1), ui_dqm_large_value, SHURK0_SELPH_DQ1_TXDLY_DQM0);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ3), ui_dqm_small_value, SHURK0_SELPH_DQ3_DLY_DQM0);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ7), pi_dqm_value, SHU_R0_B0_DQ7_RK0_ARPI_DQM_B0);
        }
        else
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0), ui_large_value, SHURK0_SELPH_DQ0_TXDLY_DQ1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ2), ui_small_value, SHURK0_SELPH_DQ2_DLY_DQ1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ7), pi_value, SHU_R0_B1_DQ7_RK0_ARPI_DQ_B1);

            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ1), ui_dqm_large_value, SHURK0_SELPH_DQ1_TXDLY_DQM1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ3), ui_dqm_small_value, SHURK0_SELPH_DQ3_DLY_DQM1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ7), pi_dqm_value, SHU_R0_B1_DQ7_RK0_ARPI_DQM_B1);
        }
        DramcTXSetVref(p, p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Range, p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Value);

        //DLL all off from Justin
#if ENABLE_MCK8X_MODE
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_SHU_CA_DLL0), 0x0, SHU_CA_DLL0_RG_ARDLL_PHDET_EN_CA_SHU);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DLL0), 0x0, SHU_B0_DLL0_RG_ARDLL_PHDET_EN_B0_SHU);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_SHU_B1_DLL0), 0x0, SHU_B1_DLL0_RG_ARDLL_PHDET_EN_B1_SHU);
#else
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_CA_DLL_ARPI2), 0x0, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_DLL_ARPI2), 0x0, B0_DLL_ARPI2_RG_ARDLL_PHDET_EN_B0);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_DLL_ARPI2), 0x0, B1_DLL_ARPI2_RG_ARDLL_PHDET_EN_B1);
#endif
#endif

#if RUNTIME_SHMOO_RX
    // here has a problem, RX dq is perbit but I just can choose one (bit0) to do compare and setting
    if (p->pSavetimeData->Runtime_Shmoo_para.RX_delay[0] <=0)
    {
#if 0    
        // Set DQS delay
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ6 + 0x50*RUNTIME_SHMOO_TEST_BYTE), \
                                P_Fld((-p->pSavetimeData->Runtime_Shmoo_para.RX_delay[0]),SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0) |P_Fld((-p->pSavetimeData->Runtime_Shmoo_para.RX_delay[0]),SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_F_DLY_B0));

        // Set DQM delay to 0
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ6 + 0x50*RUNTIME_SHMOO_TEST_BYTE), \
                                P_Fld(0,SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0) |P_Fld(0,SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_F_DLY_B0));
        
        DramPhyReset(p);
#endif
        // Set DQ delay to 0
        for (u1BitIdx=0; u1BitIdx<4; u1BitIdx++)
        {
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ2 + 0x50*RUNTIME_SHMOO_TEST_BYTE + u1BitIdx*4), 0);//DQ0~DQ7
        }        
    }
    else
    {
#if 0    
        // Set DQS delay to 0
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ6 + 0x50*RUNTIME_SHMOO_TEST_BYTE), \
                                P_Fld(0,SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0) |P_Fld(0,SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_F_DLY_B0));

    
        // Adjust DQM output delay.
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ6 + 0x50*RUNTIME_SHMOO_TEST_BYTE), \
                                P_Fld(p->pSavetimeData->Runtime_Shmoo_para.RX_delay,SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0) |P_Fld(p->pSavetimeData->Runtime_Shmoo_para.RX_delay,SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_F_DLY_B0));

        DramPhyReset(p);
#endif
        // Adjust DQ output delay.
        for (u1BitIdx=0; u1BitIdx<8; u1BitIdx+=2)
        {
             vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ2+ (0x50*RUNTIME_SHMOO_TEST_BYTE) +u1BitIdx*2), \
                                            P_Fld(((U32)p->pSavetimeData->Runtime_Shmoo_para.RX_delay[u1BitIdx]),SHU_R0_B0_DQ2_RK0_RX_ARDQ0_R_DLY_B0) |\
                                            P_Fld(((U32)p->pSavetimeData->Runtime_Shmoo_para.RX_delay[u1BitIdx+1]),SHU_R0_B0_DQ2_RK0_RX_ARDQ1_R_DLY_B0));
        
        }
    }        

    //Set Vref
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ5 + 0x80*RUNTIME_SHMOO_TEST_BYTE), p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Value, SHU_B0_DQ5_RG_RX_ARDQ_VREF_SEL_B0);  // LP4 and LP4x with term: 0xe
#endif
    }

    //save parameters to eMMC
#if EMMC_READY
    write_offline_dram_calibration_data(p->shu_type, p->pSavetimeData);
#endif
    mcSHOW_ERR_MSG(("Fra Save calibration result to emmc\n"));

    //copy parameters to memory for kernel test script used
    //wait for YiRong's SRAM copy function
#if RUNTIME_SHMOO_TX    
    dramc_set_rshmoo_info(p->pSavetimeData->Runtime_Shmoo_para.TX_Rank, p->pSavetimeData->Runtime_Shmoo_para.TX_Channel,
        p->pSavetimeData->Runtime_Shmoo_para.TX_Byte, p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Range, p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Value,
        p->pSavetimeData->Runtime_Shmoo_para.TX_PI_delay-p->pSavetimeData->Runtime_Shmoo_para.TX_Original_PI_delay, 1, (p->pSavetimeData->Runtime_Shmoo_para.flag == RUNTIME_SHMOO_END) ? 1 : 0);
#endif

#if RUNTIME_SHMOO_RX
    dramc_set_rshmoo_info(p->pSavetimeData->Runtime_Shmoo_para.TX_Rank, p->pSavetimeData->Runtime_Shmoo_para.TX_Channel,
        p->pSavetimeData->Runtime_Shmoo_para.TX_Byte, p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Range, p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Value,
        p->pSavetimeData->Runtime_Shmoo_para.RX_delay[0], 1, (p->pSavetimeData->Runtime_Shmoo_para.flag == RUNTIME_SHMOO_END) ? 1 : 0);

//    vAutoRefreshSwitch(p, DISABLE); //After gating, Rx and Tx calibration, auto refresh should be disable 

#endif

    p->channel = backup_channel;
    p->rank = backup_rank;
}
#endif

#ifdef FIRST_BRING_UP
void Test_Broadcast_Feature(DRAMC_CTX_T *p)
{
    U32 u4RegBackupAddress[] =
    {
        (DRAMC_REG_SHURK0_DQSIEN),
        (DRAMC_REG_SHURK0_DQSIEN+SHIFT_TO_CHB_ADDR),

        (DDRPHY_B0_DQ0),
        (DDRPHY_B0_DQ0+SHIFT_TO_CHB_ADDR),        
    };    
    U32 read_value;
    U8 backup_broadcast;
    
    backup_broadcast = GetDramcBroadcast();        

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);   

    DramcBackupRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));    

    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);    

    vIO32Write4B(DRAMC_REG_SHURK0_DQSIEN, 0xA55A00FF);
    vIO32Write4B(DDRPHY_B0_DQ0, 0xA55A00FF);    

    read_value = u4IO32Read4B(DRAMC_REG_SHURK0_DQSIEN+SHIFT_TO_CHB_ADDR);  
    if (read_value != 0xA55A00FF)
    {
        mcSHOW_ERR_MSG(("Check Erro! Broad Cast CHA RG to CHB Fail!!\n"));
        while(1);        
    }

    read_value = u4IO32Read4B(DDRPHY_B0_DQ0+SHIFT_TO_CHB_ADDR);  
    if (read_value != 0xA55A00FF)
    {
        mcSHOW_ERR_MSG(("Check Erro! Broad Cast CHA RG to CHB Fail!!\n"));
        while(1);
    }    

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);

    DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));

    DramcBroadcastOnOff(backup_broadcast);            
}
#endif

U8 u1TestLLL=0;

int Init_DRAM(DRAM_DRAM_TYPE_T dram_type, DRAM_CBT_MODE_EXTERN_T dram_cbt_mode_extern, DRAM_INFO_BY_MRR_T *DramInfo, U8 get_mdl_used)
{
    #if !SW_CHANGE_FOR_SIMULATION

    int mem_start,len, s4value;
    DRAMC_CTX_T * p;
    U8 ucstatus = 0;
    U32 u4value;
    U8 chIdx;
    U8 final_shu;

#ifdef DDR_INIT_TIME_PROFILING
    U32 CPU_Cycle;
    TimeProfileBegin();
#endif

    psCurrDramCtx = &DramCtx_LPDDR4;

#if defined(DDR_INIT_TIME_PROFILING) || (__ETT__ && SUPPORT_SAVE_TIME_FOR_CALIBRATION)
    if (gtime_profiling_flag == 0)
    {
        memcpy(&gTimeProfilingDramCtx, psCurrDramCtx, sizeof(DRAMC_CTX_T));
        gtime_profiling_flag = 1;
    }
    
    p = &gTimeProfilingDramCtx;
    gfirst_init_flag = 0;

    DramcConfInfraReset(p);
#else  
    p = psCurrDramCtx;  
#endif

    Set_MDL_Used_Flag(get_mdl_used);

    p->dram_type = dram_type;

    /* Convert DRAM_CBT_MODE_EXTERN_T to DRAM_CBT_MODE_T */
    switch ((int)dram_cbt_mode_extern)
    {
        case CBT_R0_R1_NORMAL:
            p->dram_cbt_mode[RANK_0] = CBT_NORMAL_MODE;
            p->dram_cbt_mode[RANK_1] = CBT_NORMAL_MODE;
            break;
        case CBT_R0_R1_BYTE:
            p->dram_cbt_mode[RANK_0] = CBT_BYTE_MODE1;
            p->dram_cbt_mode[RANK_1] = CBT_BYTE_MODE1;
            break;
        case CBT_R0_NORMAL_R1_BYTE:
            p->dram_cbt_mode[RANK_0] = CBT_NORMAL_MODE;
            p->dram_cbt_mode[RANK_1] = CBT_BYTE_MODE1;
            break;
        case CBT_R0_BYTE_R1_NORMAL:
            p->dram_cbt_mode[RANK_0] = CBT_BYTE_MODE1;
            p->dram_cbt_mode[RANK_1] = CBT_NORMAL_MODE;
            break;
        default:
            mcSHOW_ERR_MSG(("Error!"));
            break;
    }
    mcSHOW_DBG_MSG2(("dram_cbt_mode_extern: %d\n"
                      "dram_cbt_mode [RK0]: %d, [RK1]: %d\n",
                      (int)dram_cbt_mode_extern, p->dram_cbt_mode[RANK_0], p->dram_cbt_mode[RANK_1]));
    
#if ENABLE_APB_MASK_WRITE
    U32 u4GPTTickCnt;
    TimeProfileBegin();

    EnableDramcAPBMaskWrite(p);
    DramcRegAPBWriteMask(p);

    u4GPTTickCnt = TimeProfileEnd();
    mcSHOW_TIME_MSG(("[DramcRegAPBWriteMask] take %d ms\n", u4GPTTickCnt/1000));

    TestAPBMaskWriteFunc(p);

    while(1);
#endif

    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);   //LP4 broadcast on

    if (gfirst_init_flag == 0)
    {
        MPLLInit();
        Global_Option_Init(p);
        gfirst_init_flag = 1;
    }

#ifdef FIRST_BRING_UP
    Test_Broadcast_Feature(p);
#endif

#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
    {
        U8 backup_broadcast;
        backup_broadcast = GetDramcBroadcast();
        EMI_Init(p);
        TA2_Test_Run_Time_HW_Set_Column_Num(p);
        DramcBroadcastOnOff(backup_broadcast);
    }
#endif

    mcSHOW_DBG_MSG(("\n\n[Bian_co] ETT version 0.0.0.1\n dram_type %d, R0 cbt_mode %d, R1 cbt_mode %d VENDOR=%d\n\n", p->dram_type, p->dram_cbt_mode[RANK_0], p->dram_cbt_mode[RANK_1], p->vendor_id));
  
    vDramcInit_PreSettings(p);
  
    // DramC & PHY init for all channels
    //===  First frequency ======
    
#if defined(DUMP_INIT_RG_LOG_TO_DE)
    vSetDFSFreqSelByTable(p, &gFreqTbl[3]); // 9:DDR1600 1:DDR2400 0:DDR3200 3:DDR3733
#else
    vSetDFSFreqSelByTable(p, &gFreqTbl[DRAM_DFS_SHUFFLE_MAX-1]);
#endif
    #if (ENABLE_DRAM_SINGLE_FREQ_SELECT != 0xFF) || defined(FIRST_BRING_UP) || (__FLASH_TOOL_DA__)
        gAndroid_DVFS_en = FALSE; //skip ETT DVFS stress
    #endif

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    DramcSave_Time_For_Cal_Init(p);
#endif
#ifndef LOOPBACK_TEST
    if (p->dram_type == TYPE_LPDDR4X) // LP4/LP4P need confirm
    {
            // DDR3733 & below
            // DRVP/ODTN Vref setting is 0x35 for 40/60ohm
            // DRVN Vref setting is 0x2A for 40ohm
            // DDR4266 & up
            // DRVP/ODTN Vref setting is 0x3A for 30/60ohm
            // DRVN Vref setting is 0x2A for 30ohm
            DramcSwImpedanceCal(p, 1, IMP_LOW_FREQ); // for DRVN/P and ODTN
            #if FSP1_CLKCA_TERM
            DramcSwImpedanceCal(p, 1, IMP_HIGH_FREQ); // for DRVN/P and ODTN
            #endif
            #if ENABLE_SAMSUNG_NT_ODT
            DramcSwImpedanceCal(p, 1, IMP_NT_ODTN); // for Samsung NT ODTN
            #endif
    }
    else
    {
        mcSHOW_ERR_MSG(("[DramcSwImpedanceCal] Warnning: Need confirm DRAM type for SW IMP Calibration !!!\n"));
        #if __ETT__
        while(1);
        #endif
    }
#endif

#ifdef DDR_INIT_TIME_PROFILING
    CPU_Cycle=TimeProfileEnd();
    mcSHOW_TIME_MSG(("(0)Pre_Init + SwImdepance takes %d ms\n\r", CPU_Cycle/1000));
#endif

#ifdef DUMP_INIT_RG_LOG_TO_DE
    gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag = 1;
    mcSHOW_DUMP_INIT_RG_MSG(("\n\n//=== DDR\033[1;32m%d\033[m\n",p->frequency<<1));
#endif

    //Clk free run
    //EnableDramcPhyDCM(p, 0);

    DFSInitForCalibration(p);

#ifdef TEST_MODE_MRS
    if(global_which_test  == 0)
        TestModeTestMenu();
#endif


#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    if (p->femmc_Ready==1)
    {
        p->support_rank_num = p->pSavetimeData->support_rank_num;
    }
    else
#endif
    {
#if ENABLE_RANK_NUMBER_AUTO_DETECTION  // only need to do this when DUAL_RANK_ENABLE is 1
            if (Get_MDL_Used_Flag()==GET_MDL_USED)
            {
                DramRankNumberDetection(p);
                DFSInitForCalibration(p);  // Restore setting after rank dection (especially DQ= DQS+16)
            }
#endif

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        p->pSavetimeData->support_rank_num = p->support_rank_num;
#endif
    }

    #if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
    U8 backup_broadcast;
    backup_broadcast = GetDramcBroadcast();
    EMI_Init2();
    DramcBroadcastOnOff(backup_broadcast);
    #endif
        
    if (Get_MDL_Used_Flag()==GET_MDL_USED)
    {
        // only K CHA to save time
        vSetPHY2ChannelMapping(p, CHANNEL_A);
        vCalibration_Flow_For_MDL(p); // currently for LP4               
        GetDramInforAfterCalByMRR(p, DramInfo);
        return 0;
    }
    else //NORMAL_USED
    {
        #if (fcFOR_CHIP_ID == fcKrug) // @Darren, new chip need double confirm
        UpdateDFSTbltoDDR4266(p);
        #endif
        vDramCalibrationAllChannel(p);
        GetDramInforAfterCalByMRR(p, DramInfo);
        vDramcACTimingOptimize(p);
    }

#if ((!defined(FIRST_BRING_UP)) || (ENABLE_DRAM_SINGLE_FREQ_SELECT != 0xFF)) && (!__FLASH_TOOL_DA__)
    DramcSaveToShuffleSRAM(p, DRAM_DFS_SHUFFLE_1, gFreqTbl[DRAM_DFS_SHUFFLE_MAX-1].shuffleIdx);
    #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    DramcSave_Time_For_Cal_End(p);
    #endif
    LoadShuffleSRAMtoDramc(p, gFreqTbl[DRAM_DFS_SHUFFLE_MAX-1].shuffleIdx, DRAM_DFS_SHUFFLE_2); //Darren: DDR1600 for MRW (DramcModeRegInit_LP4 and CBT)

    S8 u1ShuIdx;
#if (ENABLE_DRAM_SINGLE_FREQ_SELECT == 0xFF)
    for(u1ShuIdx = DRAM_DFS_SHUFFLE_MAX-2 ; u1ShuIdx >= DRAM_DFS_SHUFFLE_1; u1ShuIdx--)
#else
    u1ShuIdx = ENABLE_DRAM_SINGLE_FREQ_SELECT;
#endif
    {
        vSetDFSFreqSelByTable(p, &gFreqTbl[u1ShuIdx]);
        #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        DramcSave_Time_For_Cal_Init(p);
        #endif
        DFSInitForCalibration(p);
        vDramCalibrationAllChannel(p);      
        vDramcACTimingOptimize(p);

        if (p->frequency == u2DFSGetHighestFreq(p))
        {            
#if RUNTIME_SHMOO_RELEATED_FUNCTION && SUPPORT_SAVE_TIME_FOR_CALIBRATION
            DramcRunTimeShmooRG_BackupRestore(p);
#endif
                
#if RUNTIME_SHMOO_RELEATED_FUNCTION && SUPPORT_SAVE_TIME_FOR_CALIBRATION
            RunTime_Shmoo_update_parameters(p);
#endif   
        }
       
        DramcSaveToShuffleSRAM(p, DRAM_DFS_SHUFFLE_1, gFreqTbl[u1ShuIdx].shuffleIdx);
        #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        DramcSave_Time_For_Cal_End(p);
        #endif
    }
#endif //FIRST_BRING_UP

    if ((doe_get_config("dram_all_3094_0825")) || (doe_get_config("dram_all_3094_0725")) || 
        (doe_get_config("dram_all_1534_0725")) || (doe_get_config("dram_opp0_3733_others_3094_0825")) ||
        (doe_get_config("dram_opp0_3094_others_1534_0725")) || (doe_get_config("dram_opp0_2400_others_1534_0725")))
            DramcSaveToShuffleSRAM(p, DRAM_DFS_SHUFFLE_1, gFreqTbl[9].shuffleIdx);

#ifdef DDR_INIT_TIME_PROFILING
    TimeProfileBegin();
#endif

    vApplyConfigAfterCalibration(p);

#ifdef ENABLE_POST_PACKAGE_REPAIR
{
    DRAM_CHANNEL_T eOriChannel = vGetPHY2ChannelMapping(p);
    DRAM_RANK_T eOriRank = u1GetRank(p);
    vSetRank(p, RANK_1);//Fail IC 1-13 setting
    vSetPHY2ChannelMapping(p, CHANNEL_B);//Fail IC 1-13 setting

    DramcPostPackageRepair(p);
    vSetRank(p, eOriRank);
    vSetPHY2ChannelMapping(p, eOriChannel);
}
#endif

#if 0//TX_OE_CALIBATION, for DMA test
    U8 u1ChannelIdx, u1RankIdx;
    for(u1ChannelIdx=0; u1ChannelIdx<(p->support_channel_num); u1ChannelIdx++)
        for(u1RankIdx =0; u1RankIdx<(p->support_rank_num); u1RankIdx++)
        {
            vSetPHY2ChannelMapping(p, u1ChannelIdx);
            vSetRank(p, u1RankIdx);
            DramcTxOECalibration(p);
        }

    vSetPHY2ChannelMapping(p, CHANNEL_A);
    vSetRank(p,RANK_0);

    U32 u4err_value;
    DramcDmaEngine((DRAMC_CTX_T *)p, 0x50000000, 0x60000000, 0xff00, 8, DMA_PREPARE_DATA_ONLY, p->support_channel_num);
    u4err_value= DramcDmaEngine((DRAMC_CTX_T *)p, 0x50000000, 0x60000000, 0xff00, 8, DMA_CHECK_DATA_ACCESS_AND_COMPARE, p->support_channel_num);
    mcSHOW_DBG_MSG(("DramC_TX_OE_Calibration  0x%X\n", u4err_value));
#endif

#if !LCPLL_IC_SCAN
#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
    print_DBG_info(p);
    Dump_EMIRegisters(p);
#endif
#endif

#if 0
    DramcRegDump(p, SRAM_SHU0);
#endif

// ETT_NO_DRAM #endif

#if ETT_NO_DRAM
    //NoDramDramcRegDump(p);
    NoDramRegFill();
#endif

    #if DRAMC_MODEREG_CHECK
    DramcModeReg_Check(p);
    #endif
        
    #if CPU_RW_TEST_AFTER_K
    mcSHOW_DBG_MSG(("\n[MEM_TEST] 02: After DFS, before run time config\n"));
    vDramCPUReadWriteTestAfterCalibration(p);
    #endif

    #if TA2_RW_TEST_AFTER_K
    mcSHOW_DBG_MSG(("\n[TA2_TEST]\n"));
    TA2_Test_Run_Time_HW(p);
    #endif

#if __ETT__
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    if(!(p->femmc_Ready==0))
#elif defined(DDR_INIT_TIME_PROFILING)
if(u2TimeProfileCnt == (DDR_INIT_TIME_PROFILING_TEST_CNT-1)) //last time of loop
#endif
#endif
    {
        #if (DRAMC_DFS_MODE == 1)
        TransferPLLToSPMControl(p);

        if (gAndroid_DVFS_en == TRUE) // shuffle to DDR3733 boot
        {               
#if defined(SLT)
            U8 slt_shu;  // DDR2400 0.65V for SLT
            final_shu = SRAM_SHU3;  // DDR2400 0.7V for SRAM shu0/1/2
            slt_shu = SRAM_SHU4;  // DDR2400 0.65V for SLT
#else
            if (doe_get_config("dram_fix_3094_0825"))
                final_shu = SRAM_SHU1;
            else if (doe_get_config("dram_fix_3094_0725"))
                final_shu = SRAM_SHU2;
            else if (doe_get_config("dram_fix_2400_0725"))
                final_shu = SRAM_SHU4;
            else if (doe_get_config("dram_fix_1534_0725"))
                final_shu = SRAM_SHU6;
            else if ((doe_get_config("dram_fix_1200_0725")) || (doe_get_config("dram_fix_1200_065")))
                final_shu = SRAM_SHU8;
            else if (doe_get_config("dram_fix_800_065"))
                final_shu = SRAM_SHU9;
            else
                final_shu = SRAM_SHU0; // switch back to PHY PLL, DDR3733
#endif
            vSetDFSFreqSelByTable(p, get_FreqTbl_by_shuffleIndex(p, final_shu));
            DramcDFSDirectJump_SPMMode(p, SRAM_SHU2);
            DramcDFSDirectJump_SPMMode(p, final_shu);

#if defined(SLT)
            UpdateSltDFSTbltoDDR2400(p, final_shu, slt_shu);
#endif
        }

        #if (DVT_TEST_DUMMY_RD_SIDEBAND_FROM_SPM && defined(DUMMY_READ_FOR_TRACKING))
        DramcDummyReadForSPMSideBand(p); // SPM dummy read 1us <-> 4us for DVT only (it must call after TransferPLLToSPMControl)
        #endif

        EnableDramcTrackingBySPMControl(p);
        #endif // end of DRAMC_DFS_MODE

        mcSHOW_DBG_MSG(("\n\nSettings after calibration\n\n"));
        mcDUMP_REG_MSG(("\n\nSettings after calibration\n\n"));

        #if DUMP_ALLSUH_RG
        mcDUMP_REG_MSG(("\n[dumpRG]BeforeRunTimeConfig\n"));
        DumpShuRG(p);
        #endif

        DramcRunTimeConfig(p);

        #if DUMP_ALLSUH_RG
        mcDUMP_REG_MSG(("\n[dumpRG]AfterRunTimeConfig\n"));
        DumpShuRG(p);
        #endif

    }

    #if CPU_RW_TEST_AFTER_K
    mcSHOW_DBG_MSG(("\n[MEM_TEST] 03: After run time config\n"));
    vDramCPUReadWriteTestAfterCalibration(p);
    #endif

    #if TA2_RW_TEST_AFTER_K
    mcSHOW_DBG_MSG(("\n[TA2_TEST]\n"));
    TA2_Test_Run_Time_HW(p);
    #endif


#if (__ETT__ && CPU_RW_TEST_AFTER_K)
    /* 0x46000000 is LK base addr */
//while(1)
{
    //if ((s4value = complex_mem_test (0x46000000, 0x2000)) == 0)
    if ((s4value = complex_mem_test (0x40024000, 0x20000)) == 0)
    {
        mcSHOW_DBG_MSG(("1st complex R/W mem test pass\n"));
    }
    else
    {
        mcSHOW_DBG_MSG(("1st complex R/W mem test fail :-%d\n", -s4value));
#if defined(SLT)
        while(1);
#endif
    }
}
#endif

#if MRW_CHECK_ONLY
    vPrintFinalModeRegisterSetting(p);
#endif

#ifdef DDR_INIT_TIME_PROFILING
    CPU_Cycle=TimeProfileEnd();
    mcSHOW_TIME_MSG(("(5) After calibration takes %d ms\n\r", CPU_Cycle/1000));
#endif  // end of DDR_INIT_TIME_PROFILING

#if defined(SLT)
    {   U8 ii;
        for(ii=0; ii<3; ii++)
        {
            //DFSTestProgram(p, 0);
            vDramCPUReadWriteTestAfterCalibration(p);    
        }

        //DramcDFS(p, 0);
    }
#endif

#endif//SW_CHANGE_FOR_SIMULATION
    return 0;
}


#if (FOR_DV_SIMULATION_USED!=0)
void DPI_main(void)
{
    DRAMC_CTX_T DramConfig;

    DramConfig.channel = CHANNEL_A;
    DramConfig.support_rank_num = RANK_DUAL;
    // DramRank
    DramConfig.rank = RANK_0;
    // DRAM type
    DramConfig.dram_type = TYPE_LPDDR3;
    // DRAM Fast switch point type, only for LP4, useless in LP3
    DramConfig.dram_fsp = FSP_0;
    // IC and DRAM read DBI
    DramConfig.DBI_R_onoff[FSP_0] = DBI_OFF;             // only for LP4, uesless in LP3
    DramConfig.DBI_R_onoff[FSP_1] = DBI_ON;              // only for LP4, uesless in LP3
    // IC and DRAM write DBI
    DramConfig.DBI_W_onoff[FSP_0] = DBI_OFF;             // only for LP4, uesless in LP3
    DramConfig.DBI_W_onoff[FSP_1] = DBI_ON;              // only for LP4, uesless in LP3
    // bus width
    DramConfig.data_width = DATA_WIDTH_32BIT;
    // DRAMC internal test engine-2 parameters in calibration
    DramConfig.test2_1 = DEFAULT_TEST2_1_CAL;
    DramConfig.test2_2 = DEFAULT_TEST2_2_CAL;
    // DRAMC test pattern in calibration
    DramConfig.test_pattern = TEST_XTALK_PATTERN;
    // DRAMC operation clock frequency in MHz
    DramConfig.frequency = 800;
    // Switch to ENABLE or DISABLE low frequency write and high frequency read
    DramConfig.u2DelayCellTimex100 = 0;

    //DramConfig.enable_rx_scan_vref =DISABLE;
    //DramConfig.enable_tx_scan_vref =DISABLE;
    //DramConfig.dynamicODT = DISABLE;

    printf("main functino start!\n");

    Init_DRAM(DramConfig.dram_type, CBT_R0_R1_NORMAL, NULL, NORMAL_USED);
}

#define DV_SIMULATION_INIT_C    1
#define DV_SIMULATION_BEFORE_K  1
#define DV_SIMULATION_MIOCKJMETER  0
#define DV_SIMULATION_SW_IMPED 0
#define DV_SIMULATION_LP4_ZQ 0
#define DV_SIMULATION_CA_TRAINING 0
#define DV_SIMULATION_WRITE_LEVELING  0 // Please enable with Tx Calib
#define DV_SIMULATION_GATING 0
#define DV_SIMULATION_DATLAT 0
#define DV_SIMULATION_RX_PERBIT    0
#define DV_SIMULATION_TX_PERBIT    0 // Please enable with write leveling
#define DV_SIMULATION_AFTER_K   1
#define DV_SIMULATION_DBI_ON   1 // please enable with DDR2667 up
#define DV_SIMULATION_SPM_CONTROL 1
#define DV_SIMULATION_RUNTIME_CONFIG 1

#define DV_SIMULATION_RUN_TIME_MRW 0
#define DV_SIMULATION_RUN_TIME_MRR 0
#define DV_SIMULATION_RUN_FREQ_SEL LP4_DDR1600
#define DV_SIMULATION_RUN_FREQ 800
#define DV_SIMULATION_RUN_DIV_MODE DIV8_MODE
#define DV_SIMULATION_DFS 0 // Init settings only when enable DFS. Don't enable any calibration
#define DV_SIMULATION_DFS_SHU_MAX 4

#if DV_SIMULATION_DFS
DRAM_DFS_FREQUENCY_TABLE_T gDVDFSTbl[DV_SIMULATION_DFS_SHU_MAX] = {
    {LP4_DDR3733 /*3733*/, DIV8_MODE, SRAM_SHU0, DUTY_NEED_K,  VREF_CALI_OFF, NORMAL_CLOSE_LOOP},  // highest freq of term group (3733) must k first.
    {LP4_DDR1600 /*1600*/, DIV4_MODE, SRAM_SHU5, DUTY_DEFAULT, VREF_CALI_OFF, NORMAL_CLOSE_LOOP},
    {LP4_DDR800 /* 800*/,  DIV4_MODE, SRAM_SHU9, DUTY_DEFAULT, VREF_CALI_OFF, DDR800_CLOSE_LOOP},
    {DV_SIMULATION_RUN_FREQ_SEL, DV_SIMULATION_RUN_DIV_MODE, SRAM_SHU6, DUTY_NEED_K, VREF_CALI_OFF, NORMAL_CLOSE_LOOP}, //DDR800 must update to DDR800_CLOSE_LOOP
};
#else
DRAM_DFS_FREQUENCY_TABLE_T gDVDFSTbl[1] = {
    {DV_SIMULATION_RUN_FREQ_SEL, DV_SIMULATION_RUN_DIV_MODE, SRAM_SHU6, DUTY_NEED_K, VREF_CALI_OFF,
      NORMAL_CLOSE_LOOP}
};
#endif

U8 gu1BroadcastIsLP4 = TRUE;

void RG_dummy_write(DRAMC_CTX_T *p, U32 pattern)
{
    unsigned int ii;
    for (ii=0; ii<20; ii++)
        vIO32WriteFldAlign(DDRPHY_RFU_0X1D4, pattern, RFU_0X1D4_RESERVED_0X1D4);
}

void EnablePLLtoSPMControl(DRAMC_CTX_T *p)
{
    vIO32WriteFldAlign_All(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_SPM_DVFS_CONTROL_SEL); // DFS SPM mode for calibration
}

void DPI_vDramCalibrationSingleChannel(DRAMC_CTX_T *DramConfig)
{
    U8 ii;

    #if DV_SIMULATION_GATING
    DramcRxdqsGatingPreProcess(DramConfig);
    #endif

    vAutoRefreshSwitch(DramConfig, DISABLE);

    #if DV_SIMULATION_CA_TRAINING
    for(ii=RANK_0; ii<DramConfig->support_rank_num; ii++)
    {
        vSetRank(DramConfig, ii);
        CmdBusTrainingLP4(DramConfig);
    }
    vSetRank(DramConfig, RANK_0);

    No_Parking_On_CLRPLL(DramConfig);
    #endif

    for(ii=RANK_0; ii<DramConfig->support_rank_num; ii++)
    {
        vSetRank(DramConfig, ii);

        vAutoRefreshSwitch(DramConfig, DISABLE); //When doing WriteLeveling, should make sure that auto refresh is disable

        #if DV_SIMULATION_WRITE_LEVELING
        if (!(u1IsLP4Div4DDR800(DramConfig) && (DramConfig->rank==RANK_1))) // skip for DDR800 rank1
            DramcWriteLeveling(DramConfig);
        #endif

        vAutoRefreshSwitch(DramConfig, ENABLE);

        #if DV_SIMULATION_GATING
        // Gating calibration of single rank
        DramcRxdqsGatingCal(DramConfig);
        #endif

        #if DV_SIMULATION_RX_PERBIT
        DramcRxWindowPerbitCal(DramConfig, 0, NULL);
        #endif

        #if DV_SIMULATION_TX_PERBIT
        DramcTxWindowPerbitCal(DramConfig, TX_DQ_DQS_MOVE_DQ_DQM, FALSE);
        if(Get_Vref_Calibration_OnOff(DramConfig)==VREF_CALI_ON)
        {
            DramcTxWindowPerbitCal((DRAMC_CTX_T *) DramConfig, TX_DQ_DQS_MOVE_DQ_ONLY, TRUE);
        }
        DramcTxWindowPerbitCal((DRAMC_CTX_T *) DramConfig, TX_DQ_DQS_MOVE_DQ_ONLY, FALSE);
        #endif

        #if DV_SIMULATION_DATLAT
        // RX Datlat calibration of single rank
        DramcRxdatlatCal(DramConfig);
        #endif

        #if DV_SIMULATION_RX_PERBIT
        DramcRxWindowPerbitCal(DramConfig, 1, RX_VREF_NOT_SPECIFY);
        #endif

        #if 0//DV_SIMULATION_TX_PERBIT
        DramcTxOECalibration(DramConfig);
        #endif

        #if DV_SIMULATION_DBI_ON
            #if ENABLE_WRITE_DBI
            //Write DBI ON
            DramcWriteMinus1MCKForWriteDBI(DramConfig, -8); //Tx DQ/DQM -1 MCK for write DBI ON
            #endif
        #endif
    }

    vSetRank(DramConfig, RANK_0);

    #if DV_SIMULATION_GATING
    DramcRxdqsGatingPostProcess(DramConfig);
    #endif

    #if TDQSCK_PRECALCULATION_FOR_DVFS
    DramcDQSPrecalculation_preset(DramConfig);
    #endif

    #if XRTWTW_NEW_CROSS_RK_MODE
    if(DramConfig->support_rank_num == RANK_DUAL)
    {
        XRTWTW_SHU_Setting(DramConfig);
    }
    #endif

    #if DV_SIMULATION_DATLAT
    DramcDualRankRxdatlatCal(DramConfig);
    #endif
}

void DPI_vDramCalibrationAllChannel(DRAMC_CTX_T *DramConfig)
{
    U8 channel_idx, rank_idx;

    for(channel_idx=CHANNEL_A; channel_idx<DramConfig->support_channel_num; channel_idx++)
    {
        vSetPHY2ChannelMapping(DramConfig, channel_idx);// when switching channel, must update PHY to Channel Mapping
        DPI_vDramCalibrationSingleChannel(DramConfig);
    }

    vSetPHY2ChannelMapping(DramConfig, CHANNEL_A);

    #if DV_SIMULATION_DBI_ON
        #if ENABLE_READ_DBI
        DramcReadDBIOnOff(DramConfig, DramConfig->DBI_R_onoff[DramConfig->dram_fsp]);
        #endif

        #if ENABLE_WRITE_DBI
        DramcWriteDBIOnOff(DramConfig, DramConfig->DBI_W_onoff[DramConfig->dram_fsp]);
        #endif
    #endif

    #if XRTRTR_NEW_CROSS_RK_MODE
    if(DramConfig->support_rank_num == RANK_DUAL)
    {
        XRTRTR_SHU_Setting(DramConfig);
    }
    #endif

    #if (ENABLE_TX_TRACKING || TDQSCK_PRECALCULATION_FOR_DVFS)
    FreqJumpRatioCalculation(DramConfig);
    #endif
}

void DPI_SW_main_LP4(DRAMC_CTX_T *DramConfig)
{
    DRAM_STATUS_T VrefStatus;
#if DV_SIMULATION_RUN_TIME_MRR
    U16 u2val1, u2val2, u2val3;
#endif
#if DV_SIMULATION_DFS
    S8 s1ShuIdx;
#endif

    gu1BroadcastIsLP4 = TRUE;
    DramConfig->support_channel_num = CHANNEL_SINGLE;
    DramConfig->channel = CHANNEL_A;
    #if APOLLO_SPECIFIC
    DramConfig->support_rank_num = RANK_SINGLE;
    #else
    DramConfig->support_rank_num = RANK_DUAL;
    #endif
    // DramRank
    DramConfig->rank = RANK_0;
    // DRAMC operation clock frequency in MHz
#if DV_SIMULATION_DFS
    DramConfig->pDFSTable = &gDVDFSTbl[DV_SIMULATION_DFS_SHU_MAX-1];
#else
    DramConfig->pDFSTable = &gDVDFSTbl[0];
#endif
    DramConfig->frequency = DV_SIMULATION_RUN_FREQ;
    DramConfig->shu_type = DRAM_DFS_SHUFFLE_1;
    // DRAM type
    DramConfig->dram_type = TYPE_LPDDR4X;
    // DRAM Fast switch point type, only for LP4, useless in LP3
    DramConfig->dram_fsp = FSP_0;
    // DRAM CBT mode, only for LP4, useless in LP3
    DramConfig->dram_cbt_mode[RANK_0] = CBT_NORMAL_MODE;
    //DramConfig->dram_cbt_mode[RANK_1] = CBT_BYTE_MODE1;
    DramConfig->dram_cbt_mode[RANK_1] = CBT_NORMAL_MODE;
    // IC and DRAM read DBI
    DramConfig->DBI_R_onoff[FSP_0] = DBI_OFF;        // only for LP4, uesless in LP3
    DramConfig->DBI_R_onoff[FSP_1] = DBI_OFF;         // only for LP4, uesless in LP3
    #if ENABLE_READ_DBI
    DramConfig->DBI_R_onoff[FSP_1] = DBI_ON;         // only for LP4, uesless in LP3
    #else
    DramConfig->DBI_R_onoff[FSP_1] = DBI_OFF;        // only for LP4, uesless in LP3
    #endif
    // IC and DRAM write DBI
    DramConfig->DBI_W_onoff[FSP_0] = DBI_OFF;        // only for LP4, uesless in LP3
    DramConfig->DBI_W_onoff[FSP_1] = DBI_OFF;         // only for LP4, uesless in LP3
    #if ENABLE_WRITE_DBI
    DramConfig->DBI_W_onoff[FSP_1] = DBI_ON;         // only for LP4, uesless in LP3
    #else
    DramConfig->DBI_W_onoff[FSP_1] = DBI_OFF;         // only for LP4, uesless in LP3
    #endif
    // bus width
    DramConfig->data_width = DATA_WIDTH_16BIT;
    // DRAMC internal test engine-2 parameters in calibration
    DramConfig->test2_1 = DEFAULT_TEST2_1_CAL;
    DramConfig->test2_2 = DEFAULT_TEST2_2_CAL;
    // DRAMC test pattern in calibration
    DramConfig->test_pattern = TEST_XTALK_PATTERN;
    // u2DelayCellTimex100
    DramConfig->u2DelayCellTimex100 = 0;
    DramConfig->vendor_id = 0x1;
    DramConfig->density = 0;
    DramConfig->u1PLLMode = PHYPLL_MODE;
    //DramConfig->ranksize = {0,0};

    //DramConfig->enable_cbt_scan_vref = DISABLE;
    //DramConfig->enable_rx_scan_vref =DISABLE;
    //DramConfig->enable_tx_scan_vref =DISABLE;
    
#ifdef DUMP_INIT_RG_LOG_TO_DE
    gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag = 1;
#endif

    RG_dummy_write(DramConfig, 0x12345678);

    DramcBroadcastOnOff(DRAMC_BROADCAST_ON); //LP4 broadcast on

    Global_Option_Init(DramConfig);

    vDramcInit_PreSettings(DramConfig);

    DDRPhyFreqSel(DramConfig, DramConfig->pDFSTable->freq_sel);

    vSetPHY2ChannelMapping(DramConfig, DramConfig->channel);

#if DV_SIMULATION_SW_IMPED
    DramcSwImpedanceCal(DramConfig, 1, IMP_LOW_FREQ);  //for DRVN/P and ODTN 
    //DramcSwImpedanceCal(DramConfig, 1, IMP_HIGH_FREQ);  //for DRVN/P and ODTN 
#endif

#if DV_SIMULATION_INIT_C
    DramcInit(DramConfig);

    #if DV_SIMULATION_BEFORE_K
    vApplyConfigBeforeCalibration(DramConfig);
    //vMR2InitForSimulationTest(DramConfig);
    #endif

#ifdef DUMP_INIT_RG_LOG_TO_DE
    #if 0 //Dump RG to other shuffle for FT used, don't delete
        mcSHOW_DUMP_INIT_RG_MSG(("\n\n\n\n\n\n===== Save to Shuffle RG ======\n"));
        DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_1, DRAM_DFS_SHUFFLE_3);
    #endif
        while(1);
#endif

    #if DV_SIMULATION_MIOCKJMETER
    #ifdef ENABLE_MIOCK_JMETER
    DramcMiockJmeter(DramConfig);
    #endif
    #endif
#endif


#if DV_SIMULATION_DFS
    DPI_vDramCalibrationAllChannel(DramConfig); // for DDR1600 1:8 mode

    DramcSaveToShuffleSRAM(DramConfig, DRAM_DFS_SHUFFLE_1, gDVDFSTbl[DV_SIMULATION_DFS_SHU_MAX-1].shuffleIdx);
    LoadShuffleSRAMtoDramc(DramConfig, gDVDFSTbl[DV_SIMULATION_DFS_SHU_MAX-1].shuffleIdx, DRAM_DFS_SHUFFLE_2); //Darren: DDR1600 for MRW (DramcModeRegInit_LP4 and CBT)

    for(s1ShuIdx = DV_SIMULATION_DFS_SHU_MAX-2; s1ShuIdx >= DRAM_DFS_SHUFFLE_1; s1ShuIdx--)
    {
        vSetDFSFreqSelByTable(DramConfig, &gDVDFSTbl[s1ShuIdx]);
        DramcInit(DramConfig);

        #if DV_SIMULATION_BEFORE_K
        vApplyConfigBeforeCalibration(DramConfig);
        #endif

        #if DV_SIMULATION_MIOCKJMETER
        #ifdef ENABLE_MIOCK_JMETER
        DramcMiockJmeter(DramConfig);
        #endif
        #endif
#endif

        DPI_vDramCalibrationAllChannel(DramConfig); // for gDVDFSTbl

#if DV_SIMULATION_DFS
        DramcSaveToShuffleSRAM(DramConfig, DRAM_DFS_SHUFFLE_1, gDVDFSTbl[s1ShuIdx].shuffleIdx);
    }
#endif

#if DV_SIMULATION_AFTER_K
    vApplyConfigAfterCalibration(DramConfig);
#endif

#if DV_SIMULATION_RUN_TIME_MRW
    enter_pasr_dpd_config(0, 0xFF);
#endif

#if DV_SIMULATION_RUN_TIME_MRR
    DramcModeRegReadByRank(DramConfig, RANK_0, 4, &u2val1);
    DramcModeRegReadByRank(DramConfig, RANK_0, 5, &u2val2);
    DramcModeRegReadByRank(DramConfig, RANK_0, 8, &u2val3);
    mcSHOW_DBG_MSG(("[Runtime time MRR] MR4 = 0x%x, MR5 = 0x%x, MR8 = 0x%x\n", u2val1, u2val2, u2val3));
#endif

#if DV_SIMULATION_RUNTIME_CONFIG
    DramcRunTimeConfig(DramConfig);
#endif

#if DV_SIMULATION_DFS // NOTE: Don't use DramcDFSDirectJump_SPMMode. it will cause NULL object access.
    // high freq -> low freq
    for(s1ShuIdx = 0; s1ShuIdx < DV_SIMULATION_DFS_SHU_MAX; s1ShuIdx++)
            DramcDFSDirectJump_SRAMShuRGMode(DramConfig, gDVDFSTbl[s1ShuIdx].shuffleIdx);
    // low freq -> high freq
    for(s1ShuIdx = DV_SIMULATION_DFS_SHU_MAX-1; s1ShuIdx >= DRAM_DFS_SHUFFLE_1; s1ShuIdx--)
            DramcDFSDirectJump_SRAMShuRGMode(DramConfig, gDVDFSTbl[s1ShuIdx].shuffleIdx);
#endif

#if DV_SIMULATION_SPM_CONTROL
    EnablePLLtoSPMControl(DramConfig);
#endif

    RG_dummy_write(DramConfig, 0xAAAAAAAA);
}
#endif


#if SW_CHANGE_FOR_SIMULATION
void main(void)
{

    DRAMC_CTX_T DramConfig;
    DramConfig.channel = CHANNEL_A;
    DramConfig.support_rank_num = RANK_DUAL;
    // DramRank
    DramConfig.rank = RANK_0;
    // DRAM type
    DramConfig.dram_type = TYPE_LPDDR4X;
    // DRAM Fast switch point type, only for LP4, useless in LP3
    DramConfig.dram_fsp = FSP_0;
    // DRAM CBT mode, only for LP4, useless in LP3
    DramConfig.dram_cbt_mode[RANK_0] = CBT_NORMAL_MODE;
    DramConfig.dram_cbt_mode[RANK_1] = CBT_NORMAL_MODE;
    // IC and DRAM read DBI
    DramConfig.DBI_R_onoff[FSP_0] = DBI_OFF;             // only for LP4, uesless in LP3
    #if ENABLE_READ_DBI
    DramConfig.DBI_R_onoff[FSP_1] = DBI_ON;              // only for LP4, uesless in LP3
    #else
    DramConfig.DBI_R_onoff[FSP_1] = DBI_OFF;        // only for LP4, uesless in LP3
    #endif
    // IC and DRAM write DBI
    DramConfig.DBI_W_onoff[FSP_0] = DBI_OFF;             // only for LP4, uesless in LP3
    #if ENABLE_WRITE_DBI
    DramConfig.DBI_W_onoff[FSP_1] = DBI_ON;              // only for LP4, uesless in LP3
    #else
    DramConfig.DBI_W_onoff[FSP_1] = DBI_OFF;         // only for LP4, uesless in LP3
    #endif
    // bus width
    DramConfig.data_width = DATA_WIDTH_32BIT;
    // DRAMC internal test engine-2 parameters in calibration
    DramConfig.test2_1 = DEFAULT_TEST2_1_CAL;
    DramConfig.test2_2 = DEFAULT_TEST2_2_CAL;
    // DRAMC test pattern in calibration
    DramConfig.test_pattern = TEST_XTALK_PATTERN;
    // DRAMC operation clock frequency in MHz
    DramConfig.frequency = 800;

    //DramConfig.enable_rx_scan_vref =DISABLE;
    //DramConfig.enable_tx_scan_vref =DISABLE;
    //DramConfig.dynamicODT = DISABLE;

    MPLLInit();

    Global_Option_Init(&DramConfig);

    // DramC & PHY init for all channels
    DDRPhyFreqSel(&DramConfig, LP4_DDR1600);


#if WRITE_LEVELING_MOVE_DQS_INSTEAD_OF_CLK
    memset(DramConfig.arfgWriteLevelingInitShif, FALSE, sizeof(DramConfig.arfgWriteLevelingInitShif));
    //>fgWriteLevelingInitShif= FALSE;
#endif

    DramcInit(&DramConfig);

    vApplyConfigBeforeCalibration(&DramConfig);
    vMR2InitForSimulationTest(&DramConfig);

    vSetPHY2ChannelMapping(&DramConfig, DramConfig.channel);

    #if SIMULATION_SW_IMPED
    DramcSwImpedanceCal(&DramConfig, 1, IMP_LOW_FREQ);  //for DRVN/P and ODTN
    //DramcSwImpedanceCal(&DramConfig, 1, IMP_HIGH_FREQ);  //for DRVN/P and ODTN
    #endif


#if SIMULATION_LP4_ZQ
     if (DramConfig.dram_type == TYPE_LPDDR4 || DramConfig.dram_type == TYPE_LPDDR4X || DramConfig.dram_type == TYPE_LPDDR4P)
     {
         DramcZQCalibration(&DramConfig);
     }
#endif

    #if SIMUILATION_LP4_CBT
    CmdBusTrainingLP4(&DramConfig);
    #endif

#if SIMULATION_WRITE_LEVELING
    DramcWriteLeveling(&DramConfig);
#endif

    #if SIMULATION_GATING
    // Gating calibration of single rank
    DramcRxdqsGatingCal(&DramConfig);

    // Gating calibration of both rank
    //DualRankDramcRxdqsGatingCal(&DramConfig);
    #endif

#if SIMUILATION_LP4_RDDQC
    DramcRxWindowPerbitCal(&DramConfig, 0, NULL);
#endif

    #if SIMULATION_DATLAT
    // RX Datlat calibration of single rank
    DramcRxdatlatCal(&DramConfig);

    // RX Datlat calibration of two rank
    //DramcDualRankRxdatlatCal(&DramConfig);
    #endif

    #if SIMULATION_RX_PERBIT
    DramcRxWindowPerbitCal(&DramConfig, 1, NULL);
    #endif

    #if SIMULATION_TX_PERBIT
    DramcTxWindowPerbitCal(&DramConfig, TX_DQ_DQS_MOVE_DQ_DQM);
    DramcTxWindowPerbitCal(&DramConfig, TX_DQ_DQS_MOVE_DQ_ONLY);
    #endif

    #if ENABLE_READ_DBI
    //Read DBI ON
    SetDramModeRegForReadDBIOnOff(&DramConfig, DramConfig.dram_fsp, DramConfig.DBI_R_onoff[DramConfig.dram_fsp]);
    #endif

    #if ENABLE_WRITE_DBI
    //Write DBI ON
    DramcWriteMinus1MCKForWriteDBI(&DramConfig, -8); //Tx DQ/DQM -1 MCK for write DBI ON
    SetDramModeRegForWriteDBIOnOff(&DramConfig, DramConfig.dram_fsp, DramConfig.DBI_W_onoff[DramConfig.dram_fsp]);
    #endif

    #if ENABLE_READ_DBI
    DramcReadDBIOnOff(&DramConfig, DramConfig.DBI_R_onoff[DramConfig.dram_fsp]);
    #endif

    #if ENABLE_WRITE_DBI
    DramcWriteDBIOnOff(&DramConfig, DramConfig.DBI_W_onoff[DramConfig.dram_fsp]);
    #endif
}
#endif //SW_CHANGE_FOR_SIMULATION


