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
#ifndef MT6765_FPGA
#include <pmic.h>
#endif
#endif

#if ! __ETT__
#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
#include "pmic.h"
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

DRAM_DFS_FREQUENCY_TABLE_T gFreqTbl[DRAM_DFS_SHUFFLE_MAX] = {
    {LP4_HIGHEST_FREQSEL, LP4_HIGHEST_FREQ, DRAM_DFS_SHUFFLE_1},
    {LP4_MIDDLE_FREQSEL, LP4_MIDDLE_FREQ, DRAM_DFS_SHUFFLE_2},
    {LP4_LOWEST_FREQSEL,  LP4_LOWEST_FREQ, DRAM_DFS_SHUFFLE_3},
};

DRAM_DFS_FREQUENCY_TABLE_T gFreqTbl_LP3[DRAM_DFS_SHUFFLE_MAX] = {
    {LP3_DDR1866,  933, DRAM_DFS_SHUFFLE_1},
    {LP3_DDR1600,  800, DRAM_DFS_SHUFFLE_2},
    {LP3_DDR1200,  600, DRAM_DFS_SHUFFLE_3},
};

DRAMC_CTX_T *psCurrDramCtx;

#if defined(DDR_INIT_TIME_PROFILING) || (__ETT__ && SUPPORT_SAVE_TIME_FOR_CALIBRATION)
DRAMC_CTX_T gTimeProfilingDramCtx;
U8 gtime_profiling_flag = 0;
#endif

#if (!__ETT__ && ENABLE_LP3_SW==0)
// Preloader which does not support LP3
// scy: reduce code size by removing unused LPDDR3 structure
#else
DRAMC_CTX_T DramCtx_LPDDR3 =
{
  CHANNEL_SINGLE, // Channel number
  CHANNEL_A,          // DRAM_CHANNEL
  RANK_DUAL,        //DRAM_RANK_NUMBER_T
  RANK_0,               //DRAM_RANK_T
#if DUAL_FREQ_K
  LP3_DDR1200,
#else
#if __FLASH_TOOL_DA__
  LP3_DDR1200,
#else
  //LP3_DDR1333,  //LP3_DDR1866,
  LP3_DDR1866,
#endif
#endif
  DRAM_DFS_SHUFFLE_1,
  TYPE_LPDDR3,        // DRAM_DRAM_TYPE_T
  FSP_0 , //// DRAM Fast switch point type, only for LP4, useless in LP3
  ODT_OFF,
  {CBT_NORMAL_MODE, CBT_NORMAL_MODE}, //only for LP4, useless in LP3
  {DBI_OFF,DBI_OFF},
  {DBI_OFF,DBI_OFF},
  DATA_WIDTH_32BIT,     // DRAM_DATA_WIDTH_T
  DEFAULT_TEST2_1_CAL,    // test2_1;
  DEFAULT_TEST2_2_CAL,    // test2_2;
  TEST_XTALK_PATTERN,     // test_pattern;
  667,                  // frequency
  667,                  // freqGroup
    0x88, //vendor_id initial value
  0, //revision id
  0, //density
  {0,0},
  0,  // ucnum_dlycell_perT;
  0,  // u2DelayCellTimex100;

  DISABLE,  // enable_cbt_scan_vref;
  DISABLE,  // enable_rx_scan_vref;
  DISABLE,   // enable_tx_scan_vref;
#if PRINT_CALIBRATION_SUMMARY
    //aru4CalResultFlag[CHANNEL_NUM][RANK_MAX]
    {{0,0},  {0,0}},
    //aru4CalExecuteFlag[CHANNEL_NUM][RANK_MAX]
    {{0,0},  {0,0}},
#endif
#if WRITE_LEVELING_MOVE_DQS_INSTEAD_OF_CLK
  {{0,0}, {0,0}}, //BOOL arfgWriteLevelingInitShif;
#endif
#if TX_PERBIT_INIT_FLOW_CONTROL
  {{FALSE, FALSE}, {FALSE, FALSE}},//BOOL fgTXPerbifInit;
#endif
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
     FALSE, //femmc_Ready
     0,
     0,
     0,
     &SavetimeData,
#endif
#if (fcFOR_CHIP_ID == fcCervino)
    0,  //bDLP3
#endif
};
#endif

DRAMC_CTX_T DramCtx_LPDDR4 =
{
  CHANNEL_DUAL, // Channel number
  CHANNEL_A,          // DRAM_CHANNEL
  RANK_DUAL,        //DRAM_RANK_NUMBER_T
  RANK_0,               //DRAM_RANK_T

#ifdef MTK_FIXDDR1600_SUPPORT
  LP4_DDR1600,
#else
#if DUAL_FREQ_K
  LP4_LOWEST_FREQSEL, // Darren: it will be overwritten by gFreqTbl[DRAM_DFS_SHUFFLE_3].freq_sel (Init_DRAM)
#else
#if __FLASH_TOOL_DA__
  LP4_DDR1600,
#else
  LP4_DDR1600,
#endif
#endif
#endif
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
  0, //revision id
  0, //density
  {0,0},
  0,  // ucnum_dlycell_perT;
  0,  // u2DelayCellTimex100;

  ENABLE,   // enable_cbt_scan_vref;
  ENABLE,  // enable_rx_scan_vref;
  ENABLE,   // enable_tx_scan_vref;

#if PRINT_CALIBRATION_SUMMARY
   //aru4CalResultFlag[CHANNEL_NUM][RANK_MAX]
   {{0,0,}, {0,0}},
   //aru4CalExecuteFlag[CHANNEL_NUM][RANK_MAX]
   {{0,0,}, {0,0}},
#endif
#if WRITE_LEVELING_MOVE_DQS_INSTEAD_OF_CLK
  {{0,0}, {0,0}}, //BOOL arfgWriteLevelingInitShif;
#endif
#if TX_PERBIT_INIT_FLOW_CONTROL
  {{FALSE, FALSE}, {FALSE, FALSE}},//BOOL fgTXPerbifInit;
#endif
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
     FALSE, //femmc_Ready
     0,
     0,
     0,
     &SavetimeData,
#endif
#if (fcFOR_CHIP_ID == fcCervino)
    0,  //bDLP3
#endif
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
extern U8 ucg_num_dlycell_perT_all[DRAM_DFS_SHUFFLE_MAX][CHANNEL_NUM];
extern U16 u2gdelay_cell_ps_all[DRAM_DFS_SHUFFLE_MAX][CHANNEL_NUM];

extern void EMI_Init(DRAMC_CTX_T *p);
extern void EMI_Init2(void);

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

void vSetVcoreByFreq(DRAMC_CTX_T *p)
{
#ifndef MT6765_FPGA
#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
#if __FLASH_TOOL_DA__
	dramc_set_vcore_voltage(700000);
#else
	unsigned int vio18, vcore, vdram, vddq, tmp;
	int ret;

	vio18 = vcore = vdram = vddq = 0;

#if defined(DRAM_HQA) && defined(__ETT__)
	hqa_set_voltage_by_freq(p, &vio18, &vcore, &vdram, &vddq);
#else
#ifdef DRAM_HQA
	vio18 = HQA_VIO18;
#endif

    if(u1IsLP4Family(p->dram_type))
    {
		if(p->frequency >= 1600)
			#ifdef VCORE_BIN
				vcore = get_vcore_uv_table(0);
			#else
				vcore = (SEL_PREFIX_VCORE(LP4, KOPP0) + SEL_PREFIX_VCORE(LP4, KOPP1)) >> 1;
			#endif
		else if (p->frequency == 1200)
			#ifdef VCORE_BIN
				vcore = (get_vcore_uv_table(0) + get_vcore_uv_table(1)) >> 1;
			#else
				vcore = (SEL_PREFIX_VCORE(LP4, KOPP1) + SEL_PREFIX_VCORE(LP4, KOPP2)) >> 1;
			#endif
		else if (p->frequency == 800)
			#ifdef VCORE_BIN
				vcore = (get_vcore_uv_table(1) + get_vcore_uv_table(3)) >> 1;
			#else
				vcore = (SEL_PREFIX_VCORE(LP4, KOPP2) + SEL_PREFIX_VCORE(LP4, KOPP4)) >> 1;
			#endif
		else
			return ;
    }
    else
    {
        // for 1866
        if(p->frequency >= 933)
			#ifdef VCORE_BIN
				vcore = get_vcore_uv_table(0);
			#else
				vcore = SEL_PREFIX_VCORE(LP3, KOPP0);
			#endif
		else if(p->frequency == 800) // for 1600
			#ifdef VCORE_BIN
				vcore = (get_vcore_uv_table(0) + get_vcore_uv_table(1)) >> 1;
			#else
				vcore = (SEL_PREFIX_VCORE(LP3, KOPP0) + SEL_PREFIX_VCORE(LP3, KOPP2)) >> 1;
			#endif
		else if(p->frequency == 600) // for 1200
			#ifdef VCORE_BIN
				vcore = (get_vcore_uv_table(1) + get_vcore_uv_table(3)) >> 1;
			#else
				vcore = (SEL_PREFIX_VCORE(LP3, KOPP2) + SEL_PREFIX_VCORE(LP3, KOPP4)) >> 1;
			#endif
		else
			return ;
    }
#endif

#ifdef LAST_DRAMC
	update_last_dramc_k_voltage(p, vcore);
#endif

#if defined(DRAM_HQA)
	if (vio18)
		dramc_set_vdd1_voltage(p->dram_type, vio18);
#endif

	if (vcore)
		dramc_set_vcore_voltage(vcore);

#if defined(DRAM_HQA)
	if (vdram)
		dramc_set_vdd2_voltage(p->dram_type, vdram);

	if (u1IsLP4Family(p->dram_type)) {
		if (vddq)
			dramc_set_vddq_voltage(p->dram_type, vddq);
	}
#endif

#ifndef DDR_INIT_TIME_PROFILING
	print("Read voltage for %d\n", p->frequency);
	print("Vio18 = %d\n", dramc_get_vdd1_voltage());
	print("Vcore = %d\n", dramc_get_vcore_voltage());
	print("Vdram = %d\n", dramc_get_vdd2_voltage(p->dram_type));

	if (u1IsLP4Family(p->dram_type))
		print("Vddq = %d\n", dramc_get_vddq_voltage(p->dram_type));
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
        return dramc_get_vdd2_voltage(p->dram_type);

    if (get_voltage_type==2)
        return dramc_get_vddq_voltage(p->dram_type);
#endif

    return 0;
}


#if ENABLE_LP3_SW
#ifdef FOR_HQA_TEST_USED
VCORE_DELAYCELL_T gVcoreDelayCellTable[19]={ {8625, 671},
                                            {8500, 671},
                                            {8375, 688},
                                            {8250, 688},
                                            {8125, 706},
                                            {8000, 716},
                                            {7875, 735},
                                            {7750, 735},
                                            {7625, 745},
                                            {7500, 767},
                                            {7375, 778},
                                            {7250, 789},
                                            {7125, 813},
                                            {7000, 826},
                                            {6875, 839},
                                            {6750, 852},
                                            {6625, 866},
                                            {6500, 895},
                                            {6375, 925} };
void GetVcoreDelayCellTimeFromTable(DRAMC_CTX_T *p)
{
    U32 channel_i, i;
    U32 get_vcore;
    U16 u2gdelay_cell_ps = 0;

#if (defined(DRAM_HQA) || defined(__ETT__)) && (FOR_DV_SIMULATION_USED == 0)
    get_vcore = dramc_get_vcore_voltage()/100;
#endif

    for(i=0; i<19; i++)
    {
        if (get_vcore < gVcoreDelayCellTable[i].u2Vcore) u2gdelay_cell_ps = gVcoreDelayCellTable[i].u2DelayCell;
    }

    mcSHOW_DBG_MSG(("[GetVcoreDelayCellTimeFromTable] VCore=%d(x100), DelayCell=%d(x100)\n",get_vcore, u2gdelay_cell_ps));

    for(channel_i=CHANNEL_A; channel_i < p->support_channel_num; channel_i++)
    {
        u2gdelay_cell_ps_all[get_shuffleIndex_by_Freq(p)][channel_i] = u2gdelay_cell_ps;
    }
}
#endif
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

    if(u1IsLP4Family(p->dram_type) && (p->femmc_Ready==0))
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
            p->Bypass_RDDQC=0;
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
void vSwitchWriteDBISettings(DRAMC_CTX_T *p, U8 u1OnOff)
{
    S8 u1TXShiftUI;

    u1TXShiftUI = (u1OnOff) ? -8 : 8;
    DramcWriteMinus1MCKForWriteDBI(p, u1TXShiftUI); //Tx DQ/DQM -1 MCK for write DBI ON

    SetDramModeRegForWriteDBIOnOff(p, u1OnOff);
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
    for(s1RankIdx=RANK_0; s1RankIdx<u1RankMax; s1RankIdx++)
    {
        vSetRank(p, s1RankIdx);
        CmdBusTrainingLP4(p);

#if EYESCAN_LOG
        print_EYESCAN_LOG_message(p, 0); //draw CBT eyescan
#endif
#ifdef DDR_INIT_TIME_PROFILING
        CPU_Cycle=TimeProfileEnd();
        mcSHOW_TIME_MSG(("\tRank %d CBT takes %d us\n", s1RankIdx, CPU_Cycle));
        TimeProfileBegin();
#endif
    }
    
    vSetRank(p, RANK_0);

#if DUAL_FREQ_K
    No_Parking_On_CLRPLL(p);
#endif
#endif //ENABLE_CA_TRAINING

    for(s1RankIdx=RANK_0; s1RankIdx<u1RankMax; s1RankIdx++)
    {
        vSetRank(p, s1RankIdx);

//#if ENABLE_LP4_ZQ_CAL
        //DramcZQCalibration(p); //ZQ calibration should be done before CBT and operated at low frequency, so it is moved to mode register init
//#endif

        #if ENABLE_WRITE_LEVELING
        DramcWriteLeveling((DRAMC_CTX_T *) p);//Dram will be reset when finish write leveling

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

        DramcRxWindowPerbitCal((DRAMC_CTX_T *) p, 0);

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
        DramcTxWindowPerbitCal((DRAMC_CTX_T *) p, TX_DQ_DQS_MOVE_DQ_DQM, FALSE);  //Vref scan disable
        DramcTxWindowPerbitCal((DRAMC_CTX_T *) p, TX_DQ_DQS_MOVE_DQ_ONLY, p->enable_tx_scan_vref); 
        DramcTxWindowPerbitCal((DRAMC_CTX_T *) p, TX_DQ_DQS_MOVE_DQ_ONLY, FALSE);

#if TX_K_DQM_WITH_WDBI
        #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(p->Bypass_TXWINDOW==0) //if bypass TX K, DQM will be calculate form DQ. (no need to K DQM)
        #endif
        {
            if ((p->DBI_W_onoff[p->dram_fsp]==DBI_ON))
            {
                // K DQM with DBI_ON, and check DQM window spec.
                //mcSHOW_DBG_MSG(("[TX_K_DQM_WITH_WDBI] Step1: K DQM with DBI_ON, and check DQM window spec.\n\n"));
                vSwitchWriteDBISettings(p, DBI_ON);
                DramcTxWindowPerbitCal((DRAMC_CTX_T *) p, TX_DQ_DQS_MOVE_DQM_ONLY, FALSE);
                vSwitchWriteDBISettings(p, DBI_OFF);
            }
        }
#endif

#if EYESCAN_LOG
        Dramc_K_TX_EyeScan_Log(p);
        print_EYESCAN_LOG_message(p, 2); //draw TX eyescan
#endif

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

        DramcRxWindowPerbitCal((DRAMC_CTX_T *) p, 1);

        #ifdef DDR_INIT_TIME_PROFILING
        CPU_Cycle=TimeProfileEnd();
        mcSHOW_TIME_MSG(("\tRank %d RX calibration takes %d us\n", s1RankIdx, CPU_Cycle));
        TimeProfileBegin();
        #endif
       // DramcRxdqsGatingCal(p);

#if EYESCAN_LOG
        print_EYESCAN_LOG_message(p, 1); //draw RX eyescan
#endif

#if TX_OE_CALIBATION
        DramcTxOECalibration(p);
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
    vSetRank(p, RANK_0); // Set p's rank back to 0 (Avoids unexpected auto-rank offset calculation in u4RegBaseAddrTraslate())

    #if ENABLE_TX_TRACKING
    DramcDQSOSCShuSettings(p);
    #endif

#if GATING_ADJUST_TXDLY_FOR_TRACKING
        DramcRxdqsGatingPostProcess(p);
#endif

    if (p->support_rank_num==RANK_DUAL)
    {
        DramcDualRankRxdatlatCal(p);
    }

#if LJPLL_FREQ_DEBUG_LOG
        DDRPhyFreqMeter();
#endif

    #ifdef DDR_INIT_TIME_PROFILING
    CPU_Cycle=TimeProfileEnd();
    mcSHOW_TIME_MSG(("\tMisc takes %d us\n\n", s1RankIdx, CPU_Cycle));
    #endif
}

#if ENABLE_LP3_SW
static void vCalibration_Flow_LP3(DRAMC_CTX_T *p)
{
    U8 u1RankMax;
    S8 s1RankIdx;

#ifdef DDR_INIT_TIME_PROFILING
    U32 CPU_Cycle;
    TimeProfileBegin();
#endif

    //vAutoRefreshSwitch(p, DISABLE); //auto refresh is set as disable in LP3_DramcSetting, so don't need to disable again

#if ENABLE_CA_TRAINING  // skip when bring up
    vSetRank(p, RANK_0);
    CATrainingLP3(p);
    #if CA_TRAINING_K_RANK1_ENABLE //if dual rank K is enable, rank 0 and 1 both should do CA training
    if (p->support_rank_num==RANK_DUAL)
    {
        vSetRank(p, RANK_1);
        CATrainingLP3(p);
        vSetRank(p, RANK_0);
        //CATrainingLP3PostProcess(p); //set final CLK and CA delay as the result of averge delay of rank0 and rank1
    }
    #endif

#if 0//LP3_CATRAING_SHIFT_CLK_PI
    u1PrintModeRegWrite =1;
    DramcModeRegInit_LP3(p, FALSE); // set mode register without reset dram. 
    u1PrintModeRegWrite =0;
#endif
#endif // end of ENABLE_CA_TRAINING

#ifdef DDR_INIT_TIME_PROFILING
    CPU_Cycle=TimeProfileEnd();
    mcSHOW_TIME_MSG(("\tDRAMC CA train takes %d us\n", CPU_Cycle));
    TimeProfileBegin();
#endif

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

            vAutoRefreshSwitch(p, DISABLE); //If auto refresh don't disable at begin, auto refresh will be enable when K rank1 CAtraining and write leveling

            if((p->support_rank_num==RANK_SINGLE) || ((p->support_rank_num==RANK_DUAL) && (s1RankIdx == RANK_0)))
            {
                #if ENABLE_WRITE_LEVELING
                    DramcWriteLeveling((DRAMC_CTX_T *) p);//Dram will be reset when finish write leveling
                #endif

                #ifdef DDR_INIT_TIME_PROFILING
                    CPU_Cycle=TimeProfileEnd();
                    mcSHOW_TIME_MSG(("\tRank %d Write leveling takes %d us\n", s1RankIdx, CPU_Cycle));
                    TimeProfileBegin();
                #endif
            }

        vAutoRefreshSwitch(p, ENABLE); //when doing gating, RX and TX calibration, auto refresh should be enable

        #if LJPLL_FREQ_DEBUG_LOG
            DDRPhyFreqMeter();
        #endif

            DramcRxdqsGatingCal(p);

        #ifdef DDR_INIT_TIME_PROFILING
            CPU_Cycle=TimeProfileEnd();
            mcSHOW_TIME_MSG(("\tRank %d Gating takes %d us\n", s1RankIdx, CPU_Cycle));
            TimeProfileBegin();
        #endif

        #if LJPLL_FREQ_DEBUG_LOG
            DDRPhyFreqMeter();
        #endif

            DramcRxdatlatCal((DRAMC_CTX_T *) p);

        #ifdef DDR_INIT_TIME_PROFILING
            CPU_Cycle=TimeProfileEnd();
            mcSHOW_TIME_MSG(("\tRank %d Datlat takes %d us\n\r", s1RankIdx, CPU_Cycle));
            TimeProfileBegin();
        #endif

        #if LJPLL_FREQ_DEBUG_LOG
            DDRPhyFreqMeter();
        #endif

            if (p->support_rank_num==RANK_DUAL)
        #ifndef LP3_DUAL_RANK_RX_K
            if(s1RankIdx==RANK_0)
        #endif
            {
                DramcRxWindowPerbitCal((DRAMC_CTX_T *) p, 1);

            #ifdef DDR_INIT_TIME_PROFILING
                CPU_Cycle=TimeProfileEnd();
                mcSHOW_TIME_MSG(("\tRank %d RX takes %d us\n", s1RankIdx, CPU_Cycle));
                TimeProfileBegin();
            #endif
            }

            if (p->support_rank_num==RANK_DUAL)
        #ifndef LP3_DUAL_RANK_TX_K
            if(s1RankIdx==RANK_0)
        #endif
            {
                DramcTxWindowPerbitCal((DRAMC_CTX_T *) p, TX_DQ_DQS_MOVE_DQ_DQM, FALSE);
                #ifdef DDR_INIT_TIME_PROFILING
                CPU_Cycle=TimeProfileEnd();
                mcSHOW_TIME_MSG(("\tRank %d TX takes %d us\n", s1RankIdx, CPU_Cycle));
                TimeProfileBegin();
                #endif
            }
        }

        vSetRank(p, RANK_0);

    #if GATING_ADJUST_TXDLY_FOR_TRACKING
            DramcRxdqsGatingPostProcess(p);
    #endif

        if (p->support_rank_num==RANK_DUAL)
        {
            DramcDualRankRxdatlatCal(p);
        }

#if LJPLL_FREQ_DEBUG_LOG
        DDRPhyFreqMeter();
#endif

    vAutoRefreshSwitch(p, DISABLE); //After gating, Rx and Tx calibration, auto refresh should be disable

#ifdef DDR_INIT_TIME_PROFILING
    CPU_Cycle=TimeProfileEnd();
    mcSHOW_TIME_MSG(("\tMisc takes %d us\n\n", s1RankIdx, CPU_Cycle));
#endif
}
#endif


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

    if(u1IsLP4Family(p->dram_type))
        vCalibration_Flow_LP4(p);
#if ENABLE_LP3_SW
    else
        vCalibration_Flow_LP3(p);
#endif
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

    if(u1IsLP4Family(p->dram_type))
    {
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
            DramcRxWindowPerbitCal((DRAMC_CTX_T *) p, 0);

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
    else // LP3
    {
       vDramCalibrationSingleChannel(p);  ///TODO: vCalibration_Flow_For_MDL for LP3
    }
}


int GetDramInforAfterCalByMRR(DRAMC_CTX_T *p, DRAM_INFO_BY_MRR_T *DramInfo)
{
    U8 u1ChannelIdx, u1RankIdx, u1RankMax, u1DieNumber=0;
    U16 u2Density;
    U64 u8Size = 0, u8Size_backup = 0;
    U64 u8ChannelSize;
    U32 u4ChannelNumber=1, u4RankNumber=1;

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

        for(u1ChannelIdx=0; u1ChannelIdx<(p->support_channel_num); u1ChannelIdx++)
            for(u1RankIdx =0; u1RankIdx<RANK_MAX; u1RankIdx++)
                DramInfo->u8MR8Density[u1ChannelIdx][u1RankIdx] =0;
    }

     // Read MR8 for dram density
    for(u1ChannelIdx=0; u1ChannelIdx<(p->support_channel_num); u1ChannelIdx++)
        for(u1RankIdx =0; u1RankIdx<(p->support_rank_num); u1RankIdx++)
        {
            #if 0//PRINT_CALIBRATION_SUMMARY
            if((p->aru4CalExecuteFlag[u1ChannelIdx][u1RankIdx] !=0)  && \
                (p->aru4CalResultFlag[u1ChannelIdx][u1RankIdx]==0))
            #endif
            {
                vSetPHY2ChannelMapping(p, u1ChannelIdx);
                DramcModeRegReadByRank(p, u1RankIdx, 8, &u2Density);
                mcSHOW_DBG_MSG(("MR8 %x\n", u2Density));

                u1DieNumber = 1;
                if(p->dram_type == TYPE_LPDDR3)
                {
                    if(((u2Density >> 6) & 0x3) == 1) { //OP[7:6] =0, x16 (2 die)
                        u1DieNumber = 2;
                    }
                } else if (u1IsLP4Family(p->dram_type)) {
                    if(((u2Density >> 6) & 0x3) == 1) { //OP[7:6] =0, x16 (normal mode)
                        u1DieNumber = 2;
                    }
                }

                if (DramInfo != NULL)
                    DramInfo->u1DieNum[u1ChannelIdx][u1RankIdx] = u1DieNumber;

                u2Density = (u2Density>>2)&0xf;

                if (u1IsLP4Family(p->dram_type))
                {
            	    switch(u2Density)
            	    {
            	        ///TODO: Darren, please check the value of u8Size.
            	        case 0x0:
            	            u8Size = 0x20000000;  //4Gb
            	            //DBG_MSG("[EMI]DRAM density = 4Gb\n");
            	            break;
            	        case 0x1:
            	            u8Size = 0x30000000;  //6Gb
            	            //DBG_MSG("[EMI]DRAM density = 6Gb\n");
            	            break;
            	        case 0x2:
            	            u8Size = 0x40000000;  //8Gb
            	            //DBG_MSG("[EMI]DRAM density = 8Gb\n");
            	            break;
            	        case 0x3:
            	            u8Size = 0x60000000;  //12Gb
            	            //DBG_MSG("[EMI]DRAM density = 12Gb\n");
            	            break;
            	        case 0x4:
            	            u8Size = 0x80000000;  //16Gb
            	            //DBG_MSG("[EMI]DRAM density = 16Gb\n");
            	            break;
            	        case 0x5:
            	            u8Size = 0xc0000000; //24Gb
            	            //DBG_MSG("[EMI]DRAM density = 24Gb\n");
            	            break;                            
            	        case 0x6:
            	            u8Size = 0x100000000L; //32Gb
            	            //DBG_MSG("[EMI]DRAM density = 32Gb\n");
            	            break;
            	        default:
            	            u8Size = 0; //reserved
            	    }
                }
#if ENABLE_LP3_SW
                else
                {
                    switch(u2Density)
                    {
                        case 0x6:
                            u8Size = 0x20000000;  //4Gb
                            //DBG_MSG("[EMI]DRAM density = 4Gb\n");
                            break;
                        case 0xE:
                            u8Size = 0x30000000;  //6Gb
                            //DBG_MSG("[EMI]DRAM density = 6Gb\n");
                            break;
                        case 0x7:
                            u8Size = 0x40000000;  //8Gb
                            //DBG_MSG("[EMI]DRAM density = 8Gb\n");
                            break;
                        case 0xD:
                            u8Size = 0x60000000;  //12Gb
                            //DBG_MSG("[EMI]DRAM density = 12Gb\n");
                            break;
                        case 0x8:
                            u8Size = 0x80000000;  //16Gb
                            //DBG_MSG("[EMI]DRAM density = 16Gb\n");
                            break;
                        //case 0x9:
                            //u8Size = 0x100000000L; //32Gb
                            //DBG_MSG("[EMI]DRAM density = 32Gb\n");
                            //break;
                        default:
                            u8Size = 0; //reserved
                    }
                }
#endif /* ENABLE_LP3_SW */
                if (u8Size_backup < u8Size) // find max dram size for vDramcACTimingOptimize
                {
                    u8Size_backup = u8Size;
                    p->density = u2Density;
                }

                u8Size *= u1DieNumber;
                u8ChannelSize = u8Size;
                u4ChannelNumber = p->support_channel_num;

                if (p->support_rank_num==RANK_DUAL)
                {
                    u4RankNumber = 2;
                }

                p->ranksize[u1RankIdx] = (u8ChannelSize/u4RankNumber)*u4ChannelNumber;

                if(DramInfo != NULL)
                    DramInfo->u8MR8Density[u1ChannelIdx][u1RankIdx] = u8Size;
         }

        mcSHOW_DBG_MSG(("CH%d, RK%d, DieNum %d, Density %llx, RKsize %llx.\n", u1ChannelIdx, u1RankIdx, u1DieNumber, u8Size, p->ranksize[u1RankIdx]));
        }

    vSetPHY2ChannelMapping(p, CHANNEL_A);

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


    backup_freq_sel = p->freq_sel;
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

    #if 0
    p->freq_sel = LP4_DDR2667;
    DDRPhyFreqSel(p, p->freq_sel);
    vSetVcoreByFreq(p);
    DramcInit((DRAMC_CTX_T *) p);
    for(channel_idx=CHANNEL_A; channel_idx<p->support_channel_num; channel_idx++)
    {
        vSetPHY2ChannelMapping(p, channel_idx);
        DramcMiockJmeterHQA(p);
    }
    vSetPHY2ChannelMapping(p, backup_channel);
    #endif

    Set_PRE_MIOCK_JMETER_HQA_USED_flag(0);

    p->freq_sel = backup_freq_sel;
}
#endif

#if RUNTIME_SHMOO_RELEATED_FUNCTION && SUPPORT_SAVE_TIME_FOR_CALIBRATION
void RunTime_Shmoo_update_parameters(DRAMC_CTX_T *p)
{

    U8 backup_channel, backup_rank;
    U16 tx_pi_delay, tx_dqm_pi_delay;
    U8 ui_large_value, ui_small_value, pi_value;
    U8 ui_dqm_large_value, ui_dqm_small_value, pi_dqm_value;
#if 0
    U8 ui_oen_large_value, ui_oen_small_value, pi_oen_value;
    U8 ui_dqm_oen_large_value, ui_dqm_oen_small_value, pi_dqm_oen_value;
#endif

    backup_channel = p->channel;
    backup_rank = p->rank;

    p->channel = RUNTIME_SHMOO_TEST_CHANNEL;
    p->rank = RUNTIME_SHMOO_TEST_RANK;

    if (RUNTIME_SHMOO_TEST_BYTE == 0)
    {
        tx_pi_delay = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0), SHURK0_SELPH_DQ0_TXDLY_DQ0) * 256 +
                      u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ2), SHURK0_SELPH_DQ2_DLY_DQ0) * 32 +
                      u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);

        tx_dqm_pi_delay = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ1), SHURK0_SELPH_DQ1_TXDLY_DQM0) * 256 +
                          u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ3), SHURK0_SELPH_DQ3_DLY_DQM0) * 32 +
                          u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0);
    }
    else
    {
        tx_pi_delay = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0), SHURK0_SELPH_DQ0_TXDLY_DQ1) * 256 +
                      u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ2), SHURK0_SELPH_DQ2_DLY_DQ1) * 32 +
                      u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ7), SHU1_R0_B1_DQ7_RK0_ARPI_DQ_B1);

        tx_dqm_pi_delay = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ1), SHURK0_SELPH_DQ1_TXDLY_DQM1) * 256 +
                          u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ3), SHURK0_SELPH_DQ3_DLY_DQM1) * 32 +
                          u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ7), SHU1_R0_B1_DQ7_RK0_ARPI_DQM_B1);
    }

    mcSHOW_ERR_MSG(("fra femmc_Ready = %d ==\n",p->femmc_Ready));

    if (p->femmc_Ready==0 ||
 ((p->pSavetimeData->Runtime_Shmoo_para.TX_Channel!=RUNTIME_SHMOO_TEST_CHANNEL) || (p->pSavetimeData->Runtime_Shmoo_para.TX_Rank!=RUNTIME_SHMOO_TEST_RANK) || (p->pSavetimeData->Runtime_Shmoo_para.TX_Byte!=RUNTIME_SHMOO_TEST_BYTE))) //first K
    {
        p->pSavetimeData->Runtime_Shmoo_para.flag= 0; //on going
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
    }
    else  if (dramc_get_rshmoo_step())
    {
        p->pSavetimeData->Runtime_Shmoo_para.TX_PI_delay += RUNTIME_SHMOO_TEST_PI_DELAY_STEP;
        if (p->pSavetimeData->Runtime_Shmoo_para.TX_PI_delay > p->pSavetimeData->Runtime_Shmoo_para.TX_Original_PI_delay+RUNTIME_SHMOO_TEST_PI_DELAY_END)
        {
            p->pSavetimeData->Runtime_Shmoo_para.TX_PI_delay = p->pSavetimeData->Runtime_Shmoo_para.TX_Original_PI_delay;
            p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Value += RUNTIME_SHMOO_TEST_VREF_STEP;

            if ((p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Value+p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Range*30) > RUNTIME_SHMOO_TEST_VREF_END)
            {
                p->pSavetimeData->Runtime_Shmoo_para.flag= 0xff; //test finish
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
    }

    mcSHOW_ERR_MSG(("Fra RunTime Shmoo CH%d, Rank%d, Byte%d\n",RUNTIME_SHMOO_TEST_CHANNEL, RUNTIME_SHMOO_TEST_RANK, RUNTIME_SHMOO_TEST_BYTE ));

    if (p->pSavetimeData->Runtime_Shmoo_para.flag != 0xff)
    {
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

        TxWinTransferDelayToUIPI(p, p->pSavetimeData->Runtime_Shmoo_para.TX_PI_delay, 1, &ui_large_value, &ui_small_value, &pi_value);
        TxWinTransferDelayToUIPI(p, p->pSavetimeData->Runtime_Shmoo_para.TX_DQM_PI_delay, 1, &ui_dqm_large_value, &ui_dqm_small_value, &pi_dqm_value);
#if 0
        TxWinTransferDelayToUIPI(p, p->pSavetimeData->Runtime_Shmoo_para.TX_PI_delay-TX_DQ_OE_SHIFT*32, 0, 0, &ui_oen_large_value, &ui_oen_small_value, &pi_oen_value);
        TxWinTransferDelayToUIPI(p, p->pSavetimeData->Runtime_Shmoo_para.TX_DQM_PI_delay-TX_DQ_OE_SHIFT*32, 0, 0, &ui_dqm_oen_large_value, &ui_dqm_oen_small_value, &pi_dqm_oen_value);

        if (RUNTIME_SHMOO_TEST_BYTE == 0)
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0), P_Fld(ui_large_value, SHURK0_SELPH_DQ0_TXDLY_DQ0) | \
                                                                            P_Fld(ui_oen_large_value, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ2), P_Fld(ui_small_value, SHURK0_SELPH_DQ2_DLY_DQ0) | \
                                                                            P_Fld(ui_oen_small_value, SHURK0_SELPH_DQ2_DLY_OEN_DQ0));
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), pi_value, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);

#if 1 //DQM move together
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ1), P_Fld(ui_dqm_large_value, SHURK0_SELPH_DQ1_TXDLY_DQM0) | \
                                                                            P_Fld(ui_dqm_oen_large_value, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ3), P_Fld(ui_dqm_small_value, SHURK0_SELPH_DQ3_DLY_DQM0) | \
                                                                            P_Fld(ui_dqm_oen_small_value, SHURK0_SELPH_DQ3_DLY_OEN_DQM0));
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), pi_dqm_value, SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0);
#endif
        }
        else
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0), P_Fld(ui_large_value, SHURK0_SELPH_DQ0_TXDLY_DQ1) | \
                                                                             P_Fld(ui_oen_large_value, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ2), P_Fld(ui_small_value, SHURK0_SELPH_DQ2_DLY_DQ1) | \
                                                                             P_Fld(ui_oen_small_value, SHURK0_SELPH_DQ2_DLY_OEN_DQ1));
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ7), pi_value, SHU1_R0_B1_DQ7_RK0_ARPI_DQ_B1);

#if 1 //DQM move together
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ1), P_Fld(ui_dqm_large_value, SHURK0_SELPH_DQ1_TXDLY_DQM1) | \
                                                                            P_Fld(ui_dqm_oen_large_value, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ3), P_Fld(ui_dqm_small_value, SHURK0_SELPH_DQ3_DLY_DQM1) | \
                                                                            P_Fld(ui_dqm_oen_small_value, SHURK0_SELPH_DQ3_DLY_OEN_DQM1));
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ7), pi_dqm_value, SHU1_R0_B1_DQ7_RK0_ARPI_DQM_B1);
#endif
        }
#else
        if (RUNTIME_SHMOO_TEST_BYTE == 0)
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0), ui_large_value, SHURK0_SELPH_DQ0_TXDLY_DQ0);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ2), ui_small_value, SHURK0_SELPH_DQ2_DLY_DQ0);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), pi_value, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);

            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ1), ui_dqm_large_value, SHURK0_SELPH_DQ1_TXDLY_DQM0);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ3), ui_dqm_small_value, SHURK0_SELPH_DQ3_DLY_DQM0);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), pi_dqm_value, SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0);
        }
        else
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0), ui_large_value, SHURK0_SELPH_DQ0_TXDLY_DQ1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ2), ui_small_value, SHURK0_SELPH_DQ2_DLY_DQ1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ7), pi_value, SHU1_R0_B1_DQ7_RK0_ARPI_DQ_B1);

            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ1), ui_dqm_large_value, SHURK0_SELPH_DQ1_TXDLY_DQM1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ3), ui_dqm_small_value, SHURK0_SELPH_DQ3_DLY_DQM1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ7), pi_dqm_value, SHU1_R0_B1_DQ7_RK0_ARPI_DQM_B1);
        }
#endif
        DramcTXSetVref(p, p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Range, p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Value);
    }

    //save parameters to eMMC
#if EMMC_READY
    write_offline_dram_calibration_data(p->shu_type, p->pSavetimeData);
#endif
    mcSHOW_ERR_MSG(("Fra Save calibration result to emmc\n"));

    //copy parameters to memory for kernel test script used
    //wait for YiRong's SRAM copy function
    dramc_set_rshmoo_info(p->pSavetimeData->Runtime_Shmoo_para.TX_Rank, p->pSavetimeData->Runtime_Shmoo_para.TX_Channel,
        p->pSavetimeData->Runtime_Shmoo_para.TX_Byte, p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Range, p->pSavetimeData->Runtime_Shmoo_para.TX_Vref_Value,
        p->pSavetimeData->Runtime_Shmoo_para.TX_PI_delay-p->pSavetimeData->Runtime_Shmoo_para.TX_Original_PI_delay, 1, (p->pSavetimeData->Runtime_Shmoo_para.flag == 0xff) ? 1 : 0);


    //DLL all off from Justin
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_CA_DLL_ARPI2), 0x0, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_DLL_ARPI2), 0x0, B0_DLL_ARPI2_RG_ARDLL_PHDET_EN_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_DLL_ARPI2), 0x0, B1_DLL_ARPI2_RG_ARDLL_PHDET_EN_B1);

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

#ifdef DDR_INIT_TIME_PROFILING
    U32 CPU_Cycle;
    TimeProfileBegin();
#endif

    if(u1IsLP4Family(dram_type))
    {
        psCurrDramCtx = &DramCtx_LPDDR4;
    }
    else
    {
#if (!__ETT__ && ENABLE_LP3_SW==0)
#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
// Preloader which does not support LP3
// scy: reduce code size by removing unused LPDDR3 structure
	mcSHOW_DBG_MSG(("[Init_DRAM] LPDDR3 not supported\n"));
	ASSERT(0);
#endif
#else
        psCurrDramCtx = &DramCtx_LPDDR3;
#endif
    }

#if defined(DDR_INIT_TIME_PROFILING) || (__ETT__ && SUPPORT_SAVE_TIME_FOR_CALIBRATION)
    if (gtime_profiling_flag == 0)
    {
        memcpy(&gTimeProfilingDramCtx, psCurrDramCtx, sizeof(DRAMC_CTX_T));
        gtime_profiling_flag = 1;
    }
    
    p = &gTimeProfilingDramCtx;
    gfirst_init_flag = 0;

    if(u1IsLP4Family(p->dram_type))
        DramcConfInfraReset(p);
#else  
    p = psCurrDramCtx;
#endif

    Set_MDL_Used_Flag(get_mdl_used);

    #if 0//__ETT__
        if(u1IsLP4Family(p->dram_type))        //ETT, LPDDR4 and 4x
        {
            p->dram_type = LP4_DRAM_TYPE_SELECT;
            p->dram_cbt_mode[RANK_0] = LP4_CBT_MODE;
            p->dram_cbt_mode[RANK_1] = LP4_CBT_MODE;
        }
        else  //Preloader & ETTLPDDR3
    #endif
        {
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
        }

        if(u1IsLP4Family(p->dram_type))
        {
            DramcBroadcastOnOff(DRAMC_BROADCAST_ON);   //LP4 broadcast on
        }
        else
        {
            DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);  //LP3 broadcast off
        }

        if (gfirst_init_flag == 0)
        {
            MPLLInit();
            Global_Option_Init(p);
            gfirst_init_flag = 1;
        }
		Global_Option_Init2(p);	// setting according to emi_settings

#ifdef FIRST_BRING_UP
        Test_Broadcast_Feature(p);
#endif

#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
        {
            U8 backup_broadcast;
            backup_broadcast = GetDramcBroadcast();
            EMI_Init(p);
            DramcBroadcastOnOff(backup_broadcast);
        }
#endif


      mcSHOW_DBG_MSG(("\n\n[Bianco] ETT version 0.0.0.1\n dram_type %d, R0 cbt_mode %d, R1 cbt_mode %d VENDOR=%d\n\n", p->dram_type, p->dram_cbt_mode[RANK_0], p->dram_cbt_mode[RANK_1], p->vendor_id));

      vDramcInit_PreSettings(p);

      // DramC & PHY init for all channels
      #if DUAL_FREQ_K
      SPM_Pinmux_Setting(p);
      if(u1IsLP4Family(p->dram_type))
          p->freq_sel = gFreqTbl[DRAM_DFS_SHUFFLE_3].freq_sel;
      #endif
      
      //===  First frequency ======
      DDRPhyFreqSel(p, p->freq_sel);
      vSetVcoreByFreq(p);

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
      DramcSave_Time_For_Cal_Init(p);
#endif

      if (p->dram_type == TYPE_LPDDR4)
      {
          DramcSwImpedanceCal(p,1, 1);  //within term
      }
      else if (p->dram_type == TYPE_LPDDR4X)
      {
          DramcSwImpedanceCal(p,1, 0);  //without term
          DramcSwImpedanceCal(p,1, 1);  //within term
      }
      else
      {
        //TYPE_LPDDR4P, TYPE_LPDDR3
          DramcSwImpedanceCal(p,1, 0);  //without term
      }

      //update ODTP/ODTN of term to unterm
      DramcUpdateImpedanceTerm2UnTerm(p);
#ifdef DDR_INIT_TIME_PROFILING
      CPU_Cycle=TimeProfileEnd();
      mcSHOW_TIME_MSG(("(0)Pre_Init + SwImdepance takes %d ms\n\r", CPU_Cycle/1000));
#endif

#ifdef DUMP_INIT_RG_LOG_TO_DE
        gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag = 1;
#endif

      DFSInitForCalibration(p);

#if defined(ENABLE_DRAM_SLT)
      O1Path_Test(p);
#endif

#ifdef TEST_MODE_MRS
    if(global_which_test  == 0)
      TestModeTestMenu();
#endif

#if ENABLE_RANK_NUMBER_AUTO_DETECTION  // only need to do this when DUAL_RANK_ENABLE is 1
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
            if (p->femmc_Ready==1)
            {
                p->support_rank_num = p->pSavetimeData->support_rank_num;
            }
            else
            {
                DramRankNumberDetection(p);
                DFSInitForCalibration(p);  // Restore setting after rank dection (especially DQ= DQS+16)            
                DramInfo->u4RankNum = p->support_rank_num;
                p->pSavetimeData->support_rank_num = p->support_rank_num;
            }
#else  
            DramRankNumberDetection(p);
            DFSInitForCalibration(p);  // Restore setting after rank dection (especially DQ= DQS+16)
            DramInfo->u4RankNum = p->support_rank_num;
#endif          
#endif

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
            vDramCalibrationAllChannel(p);
            GetDramInforAfterCalByMRR(p, NULL);
            vDramcACTimingOptimize(p);
        }

#if DUAL_FREQ_K
    if(u1IsLP4Family(p->dram_type))
    {
    #ifdef MTK_FIXDDR1600_SUPPORT
        DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_1, DRAM_DFS_SHUFFLE_2);
        DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_1, DRAM_DFS_SHUFFLE_3);
    #else
        DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_1, DRAM_DFS_SHUFFLE_3); //Darren NOTE: Please take care of gFreqTbl table when you update SHUFFLE_X
        #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        DramcSave_Time_For_Cal_End(p);
        #endif
 
    //===  Second frequency ======
        DDRPhyFreqSel(p, gFreqTbl[DRAM_DFS_SHUFFLE_2].freq_sel);
        vSetVcoreByFreq(p);
        #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        DramcSave_Time_For_Cal_Init(p);
        #endif
        DFSInitForCalibration(p);
        vDramCalibrationAllChannel(p);
        vDramcACTimingOptimize(p);
        DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_1, DRAM_DFS_SHUFFLE_2); //Darren NOTE: Please take care of gFreqTbl table when you update SHUFFLE_X
        #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        DramcSave_Time_For_Cal_End(p);
        #endif

        //===  Third frequency ======
        DDRPhyFreqSel(p, gFreqTbl[DRAM_DFS_SHUFFLE_1].freq_sel);
        vSetVcoreByFreq(p);  
        #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        DramcSave_Time_For_Cal_Init(p);
        #endif
        DFSInitForCalibration(p);
        vDramCalibrationAllChannel(p);
        vDramcACTimingOptimize(p);
        #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        DramcSave_Time_For_Cal_End(p);
        #endif     
    #endif
    }
    else
    {
    #if 0
        DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_1, DRAM_DFS_SHUFFLE_3);
        DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_1, DRAM_DFS_SHUFFLE_2);
    #else
        DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_1, DRAM_DFS_SHUFFLE_3);   //save 1333 0.74V shuffle_2

        DDRPhyFreqSel(p, LP3_DDR1600);
        vSetVcoreByFreq(p);
        DFSInitForCalibration(p);
        vDramCalibrationAllChannel(p);
        DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_1, DRAM_DFS_SHUFFLE_2);   //save 1333 0.74V shuffle_3

	#ifndef MTK_LP3_HIGHEST_DDR1600
        DDRPhyFreqSel(p, LP3_DDR1866);
        vSetVcoreByFreq(p);
        DFSInitForCalibration(p);
        vDramCalibrationAllChannel(p);
	#endif
    #endif
    }
#endif

#ifdef DDR_INIT_TIME_PROFILING
    TimeProfileBegin();
#endif

    vApplyConfigAfterCalibration(p);

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

#if RUNTIME_SHMOO_RELEATED_FUNCTION && SUPPORT_SAVE_TIME_FOR_CALIBRATION
    DramcRunTimeShmooRG_BackupRestore(p);
#endif

#if RUNTIME_SHMOO_RELEATED_FUNCTION && SUPPORT_SAVE_TIME_FOR_CALIBRATION
    RunTime_Shmoo_update_parameters(p);
#endif

#if !LCPLL_IC_SCAN
#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
    print_DBG_info(p);
    Dump_EMIRegisters(p);
#endif
#endif

#if 0
    DramcRegDump(p);
#endif

// ETT_NO_DRAM #endif

#if ETT_NO_DRAM
    //NoDramDramcRegDump(p);
    NoDramRegFill();
#endif

#if (DVT_TEST_DUMMY_RD_SIDEBAND_FROM_SPM && defined(DUMMY_READ_FOR_TRACKING))
    DramcDummyReadForTrackingEnable(p);
    mcSHOW_DBG_MSG(("DUMMY_READ_FOR_TRACKING: ON\n"));

    //Disable auto refresh: set R_DMREFDIS=1
    vAutoRefreshSwitch(p, DISABLE);

    while(1)
    {
        mcSHOW_DBG_MSG(("\ndummy read is 1us ===\n"));
        DVS_DMY_RD_ENTR(p);
        mcDELAY_MS(5000);
        mcSHOW_DBG_MSG(("\ndummy read is 4us ===\n"));
        DVS_DMY_RD_EXIT(p);
        mcDELAY_MS(5000);
    }
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

    #if (__ETT__ && SUPPORT_SAVE_TIME_FOR_CALIBRATION)
    if(!(u1IsLP4Family(p->dram_type) && (p->femmc_Ready==0)))
    #endif
    {
        mcSHOW_DBG_MSG(("\n\nSettings after calibration\n\n"));
        DramcRunTimeConfig(p);
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
            DFSTestProgram(p, 0);
            vDramCPUReadWriteTestAfterCalibration(p);    
        }

        DDRPhyFreqSel(p, gFreqTbl[DRAM_DFS_SHUFFLE_2].freq_sel);
        vSetVcoreByFreq(p);
        DramcDFS(p, 0);
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

    DramConfig.enable_rx_scan_vref =DISABLE;
    DramConfig.enable_tx_scan_vref =DISABLE;
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
#define DV_SIMULATION_WRITE_LEVELING  0
#define DV_SIMULATION_GATING 0
#define DV_SIMULATION_DATLAT 0
#define DV_SIMULATION_RX_PERBIT    0
#define DV_SIMULATION_TX_PERBIT    0 // Please enable with write leveling
#define DV_SIMULATION_AFTER_K   1
#define DV_SIMULATION_DBI_ON   0
#define DV_SIMULATION_RUNTIME_CONFIG 0
#define DV_SIMULATION_RUN_TIME_MRW 0
U8 gu1BroadcastIsLP4 = TRUE;

#if ENABLE_LP3_SW
void DPI_SW_main_LP3(DRAMC_CTX_T *DramConfig)
{
    int ii;
	
    U8 gu1BroadcastIsLP4 = FALSE;
    DramConfig->channel = CHANNEL_A;
    DramConfig->support_rank_num = RANK_DUAL;
    // DramRank
    DramConfig->rank = RANK_0;
    DramConfig->freq_sel = LP3_DDR1866;
    // DRAM type
    DramConfig->dram_type = TYPE_LPDDR3;
    // DRAM Fast switch point type, only for LP4, useless in LP3
    DramConfig->dram_fsp = FSP_0;
    // DRAM CBT mode, only for LP4, useless in LP3
    DramConfig->dram_cbt_mode[RANK_0] = CBT_NORMAL_MODE;
    DramConfig->dram_cbt_mode[RANK_1] = CBT_NORMAL_MODE;
    // IC and DRAM read DBI
    DramConfig->DBI_R_onoff[FSP_0] = DBI_OFF;   // only for LP4, uesless in LP3
    DramConfig->DBI_R_onoff[FSP_1] = DBI_OFF;   // only for LP4, uesless in LP3
    // IC and DRAM write DBI
    DramConfig->DBI_W_onoff[FSP_0] = DBI_OFF;   // only for LP4, uesless in LP3
    DramConfig->DBI_W_onoff[FSP_1] = DBI_OFF;   // only for LP4, uesless in LP3
    // bus width
    DramConfig->data_width = DATA_WIDTH_32BIT;
    // DRAMC internal test engine-2 parameters in calibration
    DramConfig->test2_1 = DEFAULT_TEST2_1_CAL;
    DramConfig->test2_2 = DEFAULT_TEST2_2_CAL;
    // DRAMC test pattern in calibration
    DramConfig->test_pattern = TEST_XTALK_PATTERN;
    // DRAMC operation clock frequency in MHz
    DramConfig->frequency = 800;
    // Switch to ENABLE or DISABLE low frequency write and high frequency read
    DramConfig->u2DelayCellTimex100 = 0;

    DramConfig->enable_rx_scan_vref =DISABLE;
    DramConfig->enable_tx_scan_vref =DISABLE;
    //Cervino and Merlot have DLP3, need to give it initial value
    DramConfig->bDLP3 = 0;

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);  //LP3 broadcast off

    Global_Option_Init(DramConfig);
    Global_Option_Init2(DramConfig);

    vDramcInit_PreSettings(DramConfig);

    DDRPhyFreqSel(DramConfig, DramConfig->freq_sel);

    vSetPHY2ChannelMapping(DramConfig, DramConfig->channel);

    #if DV_SIMULATION_SW_IMPED
    //TYPE_LPDDR4P, TYPE_LPDDR3
    DramcSwImpedanceCal(DramConfig, 1, 0);  //without term
    //update ODTP/ODTN of term to unterm
    DramcUpdateImpedanceTerm2UnTerm(DramConfig);
    #endif


#if DV_SIMULATION_INIT_C
    DramcInit(DramConfig);
    #if DV_SIMULATION_MIOCKJMETER
    #ifdef ENABLE_MIOCK_JMETER
    DramcMiockJmeter(DramConfig);
    #endif
    #endif
#endif

#if WRITE_LEVELING_MOVE_DQS_INSTEAD_OF_CLK
    memset(DramConfig->arfgWriteLevelingInitShif, FALSE, sizeof(DramConfig->arfgWriteLevelingInitShif));
    //>fgWriteLevelingInitShif= FALSE;
#endif
#if TX_PERBIT_INIT_FLOW_CONTROL
    memset(DramConfig->fgTXPerbifInit, FALSE, sizeof(DramConfig->fgTXPerbifInit));
#endif

#if DV_SIMULATION_BEFORE_K
    vApplyConfigBeforeCalibration(DramConfig);
    //vMR2InitForSimulationTest(DramConfig);
#endif

    vAutoRefreshSwitch(DramConfig, DISABLE);

#if DV_SIMULATION_CA_TRAINING
    vSetRank(DramConfig, RANK_0);
    CATrainingLP3(DramConfig);
#endif

#if DV_SIMULATION_GATING
    DramcRxdqsGatingPreProcess(DramConfig);
#endif

    for(ii=RANK_0; ii<RANK_MAX; ii++)
    {
        vSetRank(DramConfig, ii);

        vAutoRefreshSwitch(DramConfig, DISABLE);

        #if DV_SIMULATION_WRITE_LEVELING
        if(ii==RANK_0)
            DramcWriteLeveling(DramConfig);
        #endif

        vAutoRefreshSwitch(DramConfig, ENABLE);


        #if DV_SIMULATION_GATING
            DramcRxdqsGatingCal(DramConfig);
        #endif

        #if DV_SIMULATION_DATLAT
            // RX Datlat calibration of single rank
            DramcRxdatlatCal(DramConfig);
        #endif

        #if DV_SIMULATION_RX_PERBIT
            DramcRxWindowPerbitCal(DramConfig, 1);
        #endif

        #if DV_SIMULATION_TX_PERBIT
            DramcTxWindowPerbitCal(DramConfig, TX_DQ_DQS_MOVE_DQ_DQM, FALSE);
        #endif
    }

    vSetRank(DramConfig, RANK_0);
#if DV_SIMULATION_GATING
    DramcRxdqsGatingPostProcess(DramConfig);
#endif

#if DV_SIMULATION_DATLAT
    DramcDualRankRxdatlatCal(DramConfig);
#endif

#if DV_SIMULATION_AFTER_K
    vApplyConfigAfterCalibration(DramConfig);
#endif

}
#endif

void DPI_SW_main_LP4(DRAMC_CTX_T *DramConfig)
{
    int ii;
    DRAM_STATUS_T VrefStatus;

    U8 gu1BroadcastIsLP4 = TRUE;
    DramConfig->support_channel_num = CHANNEL_SINGLE;
    DramConfig->channel = CHANNEL_A;
    DramConfig->support_rank_num = RANK_DUAL;
    // DramRank
    DramConfig->rank = RANK_0;
    DramConfig->freq_sel = LP4_DDR1600;
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
    // DRAMC operation clock frequency in MHz
    DramConfig->frequency = 800;
    // u2DelayCellTimex100
    DramConfig->u2DelayCellTimex100 = 0;
    DramConfig->vendor_id = 0x1;
    DramConfig->density = 0;
    //DramConfig->ranksize = {0,0};

    DramConfig->enable_cbt_scan_vref = DISABLE;
    DramConfig->enable_rx_scan_vref =DISABLE;
    DramConfig->enable_tx_scan_vref =DISABLE;

    DramcBroadcastOnOff(DRAMC_BROADCAST_ON); //LP4 broadcast on

    Global_Option_Init(DramConfig);
    Global_Option_Init2(DramConfig);

    vDramcInit_PreSettings(DramConfig);

    DDRPhyFreqSel(DramConfig, DramConfig->freq_sel);

    vSetPHY2ChannelMapping(DramConfig, DramConfig->channel);

#if DV_SIMULATION_SW_IMPED
    if (DramConfig->dram_type == TYPE_LPDDR4X)
    {
        DramcSwImpedanceCal(DramConfig,1, 0);  //without term
    }
    DramcSwImpedanceCal(DramConfig,1, 1);  //within term
    //update ODTP/ODTN of term to unterm
    DramcUpdateImpedanceTerm2UnTerm(DramConfig);
#endif


#if DV_SIMULATION_INIT_C
    DramcInit(DramConfig);
    #if DV_SIMULATION_MIOCKJMETER
    #ifdef ENABLE_MIOCK_JMETER
    DramcMiockJmeter(DramConfig);
    #endif
    #endif
    #if ENABLE_RX_TRACKING_LP4
    DramcRxInputDelayTrackingInit_byFreq(DramConfig);
    DramcRxInputDelayTrackingInit_Common(DramConfig);
    DramcRxInputDelayTrackingHW(DramConfig);
    #endif
#endif


#if WRITE_LEVELING_MOVE_DQS_INSTEAD_OF_CLK
    memset(DramConfig->arfgWriteLevelingInitShif, FALSE, sizeof(DramConfig->arfgWriteLevelingInitShif));
    //>fgWriteLevelingInitShif= FALSE;
#endif
#if TX_PERBIT_INIT_FLOW_CONTROL
    memset(DramConfig->fgTXPerbifInit, FALSE, sizeof(DramConfig->fgTXPerbifInit));
#endif

#if DV_SIMULATION_BEFORE_K
    vApplyConfigBeforeCalibration(DramConfig);
    //vMR2InitForSimulationTest(DramConfig);
#endif

#if DV_SIMULATION_GATING
DramcRxdqsGatingPreProcess(DramConfig);
#endif

#if 0//DV_SIMULATION_LP4_ZQ
    DramcZQCalibration(DramConfig);
#endif

#if 0//DV_SIMULATION_CA_TRAINING
    vSetRank(DramConfig, RANK_0);
    CmdBusTrainingLP4(DramConfig);
#endif

    vAutoRefreshSwitch(DramConfig, DISABLE);

    #if DV_SIMULATION_CA_TRAINING
        for(ii=RANK_0; ii<RANK_MAX; ii++)
        {
            vSetRank(DramConfig, ii);
            CmdBusTrainingLP4(DramConfig);


        }
        vSetRank(DramConfig, RANK_0);

        #if DUAL_FREQ_K
        No_Parking_On_CLRPLL(DramConfig);
        #endif
    #endif

    for(ii=RANK_0; ii<RANK_MAX; ii++)
    {
        vSetRank(DramConfig, ii);

        #if DV_SIMULATION_WRITE_LEVELING
        //if (ii==RANK_1)
        DramcWriteLeveling(DramConfig);
        #endif

        vAutoRefreshSwitch(DramConfig, ENABLE);

        #if DV_SIMULATION_GATING
        // Gating calibration of single rank
        DramcRxdqsGatingCal(DramConfig);
        #endif

        #if DV_SIMULATION_RX_PERBIT
        DramcRxWindowPerbitCal(DramConfig, 0);
        #endif

        #if DV_SIMULATION_TX_PERBIT
        DramcTxWindowPerbitCal(DramConfig, TX_DQ_DQS_MOVE_DQ_DQM, FALSE);
        VrefStatus = DramcTxWindowPerbitCal((DRAMC_CTX_T *) DramConfig, TX_DQ_DQS_MOVE_DQ_ONLY,DramConfig->enable_tx_scan_vref);
        #endif

        #if DV_SIMULATION_DATLAT
        // RX Datlat calibration of single rank
        DramcRxdatlatCal(DramConfig);
        #endif

        #if DV_SIMULATION_RX_PERBIT
        DramcRxWindowPerbitCal(DramConfig, 1);
        #endif

        #if 0//DV_SIMULATION_TX_PERBIT
        DramcTxOECalibration(DramConfig);
        #endif

        #if DV_SIMULATION_DBI_ON
            #if ENABLE_READ_DBI
            //Read DBI ON
            SetDramModeRegForReadDBIOnOff(DramConfig, DramConfig->DBI_R_onoff[DramConfig->dram_fsp]);
            #endif

            #if ENABLE_WRITE_DBI
            //Write DBI ON
            DramcWriteMinus1MCKForWriteDBI(DramConfig, -8); //Tx DQ/DQM -1 MCK for write DBI ON
            SetDramModeRegForWriteDBIOnOff(DramConfig, DramConfig->DBI_W_onoff[DramConfig->dram_fsp]);
            #endif
        #endif
    }

    vSetRank(DramConfig, RANK_0);
#if DV_SIMULATION_GATING
    DramcRxdqsGatingPostProcess(DramConfig);
#endif

#if DV_SIMULATION_DATLAT
    DramcDualRankRxdatlatCal(DramConfig);
#endif

#if DV_SIMULATION_AFTER_K
    vApplyConfigAfterCalibration(DramConfig);
#endif

    #if DV_SIMULATION_DBI_ON
        #if ENABLE_READ_DBI
        DramcReadDBIOnOff(DramConfig, DramConfig->DBI_R_onoff[DramConfig->dram_fsp]);
        #endif

        #if ENABLE_WRITE_DBI
        DramcWriteDBIOnOff(DramConfig, DramConfig->DBI_W_onoff[DramConfig->dram_fsp]);
        #endif
    #endif

#if DV_SIMULATION_RUN_TIME_MRW
    enter_pasr_dpd_config(0, 0xFF);
#endif

#if DV_SIMULATION_RUNTIME_CONFIG
    DramcRunTimeConfig(DramConfig);
#endif

}

#endif


