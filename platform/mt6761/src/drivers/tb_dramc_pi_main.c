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
#include "tb_emi.h"
//#endif
#include "tb_dramc_common.h"
#include "tb_dramc_pi_api.h"
#include "tb_x_hal_io.h"
#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
#ifndef MT6761_FPGA
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
    {LP4_DDR1600,  800, DRAM_DFS_SHUFFLE_3},
};

DRAM_DFS_FREQUENCY_TABLE_T gFreqTbl_LP3[DRAM_LP3_DFS_SHUFFLE_MAX] = {
    {LP3_DDR1866,  933, DRAM_DFS_SHUFFLE_1},
    {LP3_DDR1600,  800, DRAM_DFS_SHUFFLE_2},
    {LP3_DDR1200,  600, DRAM_DFS_SHUFFLE_3},
};

DRAMC_CTX_T *psCurrDramCtx;

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
  {0},            // shuffle_frequency
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
     &SavetimeData,
#endif
    0,  //bDLP3
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
  LP4_DDR1600,
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
  {0},            // shuffle_frequency
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
    &SavetimeData,
#endif
    0,  //bDLP3
};

U8 gfirst_init_flag = 0;
//=============================================================================
//  External references
//=============================================================================
#ifdef DRAM_CALIB_LOG
extern U32 gDRAM_CALIB_LOG_pointer;
extern SAVE_TO_SRAM_FORMAT_T   gDRAM_CALIB_LOG;
#endif

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
#ifndef MT6761_FPGA
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
    if(u1IsLP4Family(p->dram_type))
    {
    // for 3733, 3200
		if(p->frequency >= 1600)
			#ifdef VCORE_BIN
				vcore = (get_vcore_ptp_volt(p->dram_type, 0) + get_vcore_ptp_volt(p->dram_type, 1)) >> 1;
			#else
				vcore = (SEL_PREFIX_VCORE(LP4, KOPP0) + SEL_PREFIX_VCORE(LP4, KOPP1)) >> 1;
			#endif
		else if (p->frequency == 1200)
			#ifdef VCORE_BIN
				vcore = (get_vcore_ptp_volt(p->dram_type, 1) + get_vcore_ptp_volt(p->dram_type, 2)) >> 1;
			#else
				vcore = (SEL_PREFIX_VCORE(LP4, KOPP1) + SEL_PREFIX_VCORE(LP4, KOPP2)) >> 1;
			#endif
		else if (p->frequency == 800)
			#ifdef VCORE_BIN
				vcore = (get_vcore_ptp_volt(p->dram_type, 2) + get_vcore_ptp_volt(p->dram_type, 4)) >> 1;
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
            vcore = SEL_PREFIX_VCORE(LP3, KOPP0);
        else if(p->frequency == 800) // for 1600
            vcore = (SEL_PREFIX_VCORE(LP3, KOPP0) + SEL_PREFIX_VCORE(LP3, KOPP2)) >> 1;
        else if(p->frequency == 600) // for 1200
            vcore = (SEL_PREFIX_VCORE(LP3, KOPP2) + SEL_PREFIX_VCORE(LP3, KOPP4)) >> 1;
		else
			return ;
    }
#endif

#if defined(DRAM_HQA)
	if (vio18)
		dramc_set_vdd1_voltage(vio18);
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

#if defined(DRAM_HQA)
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
        u2gdelay_cell_ps_all[get_HQA_shuffleIdx(p)][channel_i] = u2gdelay_cell_ps;
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


#if 0
void vDramCPUReadWriteTestAfterCalibration2(DRAMC_CTX_T *p)
{
    U8 u1DumpInfo=0;
    U32 uiLen, count, uiFixedAddr;
    U32 pass_count, err_count;
    uiLen = 0xff00;
    //uiFixedAddr = DDR_BASE+0x10000000;
    uiFixedAddr = DDR_BASE;

    err_count=0;
    pass_count=0;

#if GATING_ONLY_FOR_DEBUG
    DramcGatingDebugInit(p);
    DramcGatingDebugRankSel(p, RANK_0);
#endif
    for (count=uiFixedAddr; count<uiFixedAddr+uiLen; count+=4)
    {
        *(volatile unsigned int   *)(count) = 0xffffffff;
    }

    for (count=uiFixedAddr; count<uiFixedAddr+uiLen; count+=4)
    {
        if (*(volatile unsigned int   *)(count) != 0xffffffff)
        {
            //mcSHOW_DBG_MSG(("[Fail] Addr %xh = %xh\n",count, *(volatile unsigned int   *)(count)));
            err_count++;
        }
        else
            pass_count ++;
    }

    if(err_count)
    {
        mcSHOW_DBG_MSG(("[MEM_TEST 2] Rank0 Fail. (uiFixedAddr 0x%X, Pass count =%d, Fail count =%d)\n", uiFixedAddr, pass_count, err_count));
        u1DumpInfo =1;
    }
    else
    {
        mcSHOW_DBG_MSG(("[MEM_TEST 2] Rank0 OK. (uiFixedAddr 0x%X, Pass count =%d, Fail count =%d)\n", uiFixedAddr, pass_count, err_count));
    }

    #if DUAL_RANK_ENABLE
    if (p->support_rank_num==RANK_DUAL)
    {
        #if GATING_ONLY_FOR_DEBUG
        DramcGatingDebugRankSel(p, RANK_1);
        #endif

        err_count=0;
        pass_count=0;

        if(p->support_channel_num == CHANNEL_SINGLE)
            uiFixedAddr =DDR_BASE+0x40000000;
        else
            uiFixedAddr =DDR_BASE+0x80000000;

        for (count=uiFixedAddr; count<uiFixedAddr+uiLen; count+=4)
        {
            *(volatile unsigned int   *)(count) = 0xffffffff;
        }

        for (count=uiFixedAddr; count<uiFixedAddr+uiLen; count+=4)
        {
            if (*(volatile unsigned int   *)(count) != 0xffffffff)
            {
                //mcSHOW_DBG_MSG(("[Fail] Addr %xh = %xh\n",count, *(volatile unsigned int   *)(count)));
                err_count++;
            }
            else
                pass_count ++;
        }

        if(err_count)
        {
            mcSHOW_DBG_MSG(("[MEM_TEST 2] Rank1 Fail. (uiFixedAddr 0x%X, Pass count =%d, Fail count =%d)\n", uiFixedAddr, pass_count, err_count));
            u1DumpInfo =1;
        }
        else
        {
            mcSHOW_DBG_MSG(("[MEM_TEST 2] Rank1 OK. (uiFixedAddr 0x%X, Pass count =%d, Fail count =%d)\n", uiFixedAddr, pass_count, err_count));
        }
    }
    #endif

    if(u1DumpInfo)
    {
        // Read gating error flag
        DramcDumpDebugInfo(p);
    }
    #if GATING_ONLY_FOR_DEBUG
    DramcGatingDebugExit(p);
    #endif
}
#endif
#if FORLOOP_TEST_SAVE_TIME_FOR_CALIBRATION_ON_ETT
#if 1
#if 1
#define LIGHT_CYAN  ""
#define LIGHT_GREEN ""
#define _NONE       ""
#else
#define LIGHT_CYAN  "\033[1;36m"
#define LIGHT_GREEN "\033[1;32m"
#define _NONE       "\033[m"
#endif

DRAM_STATUS_T vDramfirstCalibrationdump(DRAMC_CTX_T *p)
{
    int ch,rank,byte,freq,DQ,DQS;
    U8 DDRfreq[3] = {LP4_DDR1600,LP4_DDR2667,LP4_DDR3200}; //3 frequency/shuffle

    for (freq=0;freq<3;freq++)
    mcSHOW_DBG_MSG(("==== DDR %s%d%s%s",LIGHT_CYAN,DDRfreq[freq]==LP4_DDR1600?1600:DDRfreq[freq]==LP4_DDR3200?3200:2667,freq==2?"\n":"\t\t\t\t\t",_NONE));
    /* CBT */
    {
        for (freq=0;freq<3;freq++)
        mcSHOW_DBG_MSG(("[%sCBT%s]\tVref CLKDelay CmdDelay FinalDly%s",LIGHT_CYAN,_NONE,freq==2?"\n":"\t\t"));
        for(ch=CHANNEL_A;ch<p->support_channel_num;ch++)
        {
            for(rank=RANK_0;rank<RANK_MAX;rank++)
            {
                for (freq=0;freq<3;freq++)
                mcSHOW_DBG_MSG(("%s%c%s RK%s%d%s\t%d\t%d\t%d\t%d%s",LIGHT_GREEN,'A'+ch,_NONE,LIGHT_CYAN,rank,_NONE,
                        p->pSavetimeData->u1CBTVref_Save[ch][rank][DDRfreq[freq]],
                        p->pSavetimeData->u1CBTClkDelay_Save[ch][rank][DDRfreq[freq]],
                        p->pSavetimeData->u1CBTCmdDelay_Save[ch][rank][DDRfreq[freq]],
                        p->pSavetimeData->u1CBTCsDelay_Save[ch][rank][DDRfreq[freq]],freq==2?"\n":"\t\t"));
            }
        }
        mcSHOW_DBG_MSG(("\n"));
    }

    /* Write Leveling */
    {
        for (freq=0;freq<3;freq++)
        mcSHOW_DBG_MSG(("[%sWLevel%s]byte0\tbyte1%s",LIGHT_CYAN,_NONE,freq==2?"\n":"\t\t\t\t"));
        for(ch=CHANNEL_A;ch<p->support_channel_num;ch++)
        {
            for(rank=RANK_0;rank<RANK_MAX;rank++)
            {
                for (freq=0;freq<3;freq++)
                mcSHOW_DBG_MSG(("%s%c%s RK%s%d%s\t%d\t%d%s",LIGHT_GREEN,'A'+ch,_NONE,LIGHT_CYAN,rank,_NONE,
                        p->pSavetimeData->u1WriteLeveling_bypass_Save[ch][rank][DDRfreq[freq]][0],
                        p->pSavetimeData->u1WriteLeveling_bypass_Save[ch][rank][DDRfreq[freq]][1],freq==2?"\n":"\t\t\t\t"));
            }
        }
        mcSHOW_DBG_MSG(("\n"));
    }

    /* Rx Gating */
    {
        for (freq=0;freq<3;freq++)
        mcSHOW_DBG_MSG(("[%sRx Gating%s]%s",LIGHT_CYAN,_NONE,freq==2?"\n":"\t\t\t\t\t"));
        for (freq=0;freq<3;freq++)
        mcSHOW_DBG_MSG(("\t2T\t05T\tPI\tpass cnt%s",freq==2?"\n":"\t"));
        for(ch=CHANNEL_A;ch<p->support_channel_num;ch++)
        {
            for(rank=RANK_0;rank<RANK_MAX;rank++)
            {
                for (freq=0;freq<3;freq++)
                mcSHOW_DBG_MSG(("%s%c%s RK%s%d%s\t(%d, %d)\t(%d, %d)\t(%d,%d)\t(%d,%d)%s",LIGHT_GREEN,'A'+ch,_NONE,LIGHT_CYAN,rank,_NONE,
                        p->pSavetimeData->u1Gating2T_Save[ch][rank][DDRfreq[freq]][0],
                        p->pSavetimeData->u1Gating2T_Save[ch][rank][DDRfreq[freq]][1],
                        p->pSavetimeData->u1Gating05T_Save[ch][rank][DDRfreq[freq]][0],
                        p->pSavetimeData->u1Gating05T_Save[ch][rank][DDRfreq[freq]][1],
                        p->pSavetimeData->u1Gatingfine_tune_Save[ch][rank][DDRfreq[freq]][0],
                        p->pSavetimeData->u1Gatingfine_tune_Save[ch][rank][DDRfreq[freq]][1],
                        p->pSavetimeData->u1Gatingucpass_count_Save[ch][rank][DDRfreq[freq]][0],
                        p->pSavetimeData->u1Gatingucpass_count_Save[ch][rank][DDRfreq[freq]][1],freq==2?"\n":"\t\t"));

            }
        }
        mcSHOW_DBG_MSG(("\n"));
    }

    /* Tx WinPerbit */
    {
        for (freq=0;freq<3;freq++)
        mcSHOW_DBG_MSG(("[%sTx WinPerbit DQ%s]%s",LIGHT_CYAN,_NONE,freq==2?"\n":"\t\t\t\t"));

        for (freq=0;freq<3;freq++)
        mcSHOW_DBG_MSG(("\tVref\tTx(min)(max)Center%s",freq==2?"\n":"\t\t"));
        for(ch=CHANNEL_A;ch<p->support_channel_num;ch++)
        {
            for(rank=RANK_0;rank<RANK_MAX;rank++)
            {
                for (freq=0;freq<3;freq++)
                mcSHOW_DBG_MSG(("%s%c%s RK%s%d%s\t%d\t(%d,%d)%s",LIGHT_GREEN,'A'+ch,_NONE,LIGHT_CYAN,rank,_NONE,
                        p->pSavetimeData->u1TxWindowPerbitVref_Save[ch][rank][DDRfreq[freq]],
                        p->pSavetimeData->u1TxCenter_min_Save[ch][rank][DDRfreq[freq]][0],
                        p->pSavetimeData->u1TxCenter_max_Save[ch][rank][DDRfreq[freq]][1],freq==2?"\n":"\t\t\t\t"));
            }
        }
        mcSHOW_DBG_MSG(("\n"));

        for(ch=CHANNEL_A;ch<p->support_channel_num;ch++)
        {
            for(rank=RANK_0;rank<RANK_MAX;rank++)
            {
                for (freq=0;freq<3;freq++)
                mcSHOW_DBG_MSG(("%s%c%s RK%s%d%s\tfirst\tlast\tcenter%s",LIGHT_GREEN,'A'+ch,_NONE,LIGHT_CYAN,rank,_NONE,freq==2?"\n":"\t\t\t"));
                for (DQ=0; DQ<16; DQ++)
                {
                    for (freq=0;freq<3;freq++)
                    mcSHOW_DBG_MSG(("DQ %d\t%d\t%d\t%d%s",DQ,
                            p->pSavetimeData->u1Txfirst_pass_Save[ch][rank][DDRfreq[freq]][DQ],
                            p->pSavetimeData->u1Txlast_pass_Save[ch][rank][DDRfreq[freq]][DQ],
                            p->pSavetimeData->u1Txwin_center_Save[ch][rank][DDRfreq[freq]][DQ],freq==2?"\n":"\t\t\t"));
                }
                mcSHOW_DBG_MSG(("\n"));
            }
        }
        mcSHOW_DBG_MSG(("\n"));
    }

    /* Rx WinPerbit */
    {
        for (freq=0;freq<3;freq++)
        mcSHOW_DBG_MSG(("[%sRx WinPerbit DQ%s]%s",LIGHT_CYAN,_NONE,freq==2?"\n":"\t\t\t\t"));

        for (freq=0;freq<3;freq++)
        mcSHOW_DBG_MSG(("\tVref%s",freq==2?"\n":"\t\t\t\t\t"));
        for(ch=CHANNEL_A;ch<p->support_channel_num;ch++)
        {
            for (freq=0;freq<3;freq++)
            mcSHOW_DBG_MSG(("%s%c%s\t%d%s",LIGHT_GREEN,'A'+ch,_NONE, p->pSavetimeData->u1RxWinPerbitVref_Save[ch][DDRfreq[freq]],freq==2?"\n":"\t\t\t\t\t"));
        }
        mcSHOW_DBG_MSG(("\n"));

        for(ch=CHANNEL_A;ch<p->support_channel_num;ch++)
        {
            for(rank=RANK_0;rank<RANK_MAX;rank++)
            {
                for (freq=0;freq<3;freq++)
                mcSHOW_DBG_MSG(("%s%c%s RK%s%d%s\tfirst\tlast%s",LIGHT_GREEN,'A'+ch,_NONE,LIGHT_CYAN,rank,_NONE,freq==2?"\n":"\t\t\t\t"));
                for (DQ=0; DQ<16; DQ++)
                {
                    for (freq=0;freq<3;freq++)
                    mcSHOW_DBG_MSG(("DQ %d\t%d\t%d%s",DQ,
                            p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[ch][rank][DDRfreq[freq]][DQ],
                            p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[ch][rank][DDRfreq[freq]][DQ],freq==2?"\n":"\t\t\t\t"));
                }
                mcSHOW_DBG_MSG(("\n"));
            }
        }
        mcSHOW_DBG_MSG(("\n"));
    }

    /* Rx DatLat */
    {
        for (freq=0;freq<3;freq++)
        mcSHOW_DBG_MSG(("[%sRx DatLat%s]%s",LIGHT_CYAN,_NONE,freq==2?"\n":"\t\t\t\t\t"));
        for(ch=CHANNEL_A;ch<p->support_channel_num;ch++)
        {
            for(rank=RANK_0;rank<RANK_MAX;rank++)
            {
                for (freq=0;freq<3;freq++)
                mcSHOW_DBG_MSG(("%s%c%s RK%s%d%s\t%d%s",LIGHT_GREEN,'A'+ch,_NONE,LIGHT_CYAN,rank,_NONE,
                        p->pSavetimeData->u1RxDatlat_Save[ch][rank][DDRfreq[freq]],freq==2?"\n":"\t\t\t\t\t"));
            }
        }
        mcSHOW_DBG_MSG(("\n"));
    }
}
#else
DRAM_STATUS_T vDramfirstCalibrationdump(DRAMC_CTX_T *p)
{

int ch,rank,byte,freq,DDRfreq,DQ,DQS;
	for (freq=0;freq<3;freq++)
	{
		 if (freq==0)
		 DDRfreq=LP4_DDR800;
		 else if(freq==1)
		 DDRfreq=LP4_DDR2667;
		 else if(freq==2)
		 DDRfreq=LP4_DDR3200;

        mcSHOW_DBG_MSG(("p->pSavetimeData->\n"));
		for(ch=0;ch<4;ch++)
		{
			for(rank=0;rank<2;rank++)
			{
				//mcSHOW_DBG_MSG(("u1WriteLeveling_Save[%d][%d][%d]=%d;\n",ch,rank,DDRfreq,p->pSavetimeData->u1WriteLeveling_Save[ch][rank][DDRfreq]));
				for(byte=0;byte<2;byte++)
				{
                    mcSHOW_DBG_MSG(("u1Gating2T_Save[%d][%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],byte,p->pSavetimeData->u1Gating2T_Save[ch][rank][_MappingFreqArray[DDRfreq]][byte]));
                    mcSHOW_DBG_MSG(("u1Gating05T_Save[%d][%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],byte,p->pSavetimeData->u1Gating05T_Save[ch][rank][_MappingFreqArray[DDRfreq]][byte]));
                    mcSHOW_DBG_MSG(("u1Gatingfine_tune_Save[%d][%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],byte,p->pSavetimeData->u1Gatingfine_tune_Save[ch][rank][_MappingFreqArray[DDRfreq]][byte]));
                    mcSHOW_DBG_MSG(("u1Gatingucpass_count_Save[%d][%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],byte,p->pSavetimeData->u1Gatingucpass_count_Save[ch][rank][_MappingFreqArray[DDRfreq]][byte]));
                    mcSHOW_DBG_MSG(("u1WriteLeveling_bypass_Save[%d][%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],byte,p->pSavetimeData->u1WriteLeveling_bypass_Save[ch][rank][_MappingFreqArray[DDRfreq]][byte]));

                    mcSHOW_DBG_MSG(("u1TxCenter_min_Save[%d][%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],byte,p->pSavetimeData->u1TxCenter_min_Save[ch][rank][_MappingFreqArray[DDRfreq]][byte]));
                    mcSHOW_DBG_MSG(("u1TxCenter_max_Save[%d][%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],byte,p->pSavetimeData->u1TxCenter_max_Save[ch][rank][_MappingFreqArray[DDRfreq]][byte]));
				}
				 for (DQ=0; DQ<16; DQ++)
                {
                    mcSHOW_DBG_MSG(("u1RxWinPerbitDQ_firsbypass_Save[%d][%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],DQ,p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[ch][rank][_MappingFreqArray[DDRfreq]][DQ]));
                    mcSHOW_DBG_MSG(("u1RxWinPerbitDQ_lastbypass_Save[%d][%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],DQ,p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[ch][rank][_MappingFreqArray[DDRfreq]][DQ]));

                    mcSHOW_DBG_MSG(("u1Txwin_center_Save[%d][%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],DQ,p->pSavetimeData->u1Txwin_center_Save[ch][rank][_MappingFreqArray[DDRfreq]][DQ]));
                    mcSHOW_DBG_MSG(("u1Txfirst_pass_Save[%d][%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],DQ,p->pSavetimeData->u1Txfirst_pass_Save[ch][rank][_MappingFreqArray[DDRfreq]][DQ]));
                    mcSHOW_DBG_MSG(("u1Txlast_pass_Save[%d][%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],DQ,p->pSavetimeData->u1Txlast_pass_Save[ch][rank][_MappingFreqArray[DDRfreq]][DQ]));
#if CA_PER_BIT_DELAY_CELL
                    if(DQ<=5)
                    {
                        mcSHOW_DBG_MSG(("u1CBTCA_PerBit_DelayLine_Save[%d][%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],DQ,p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[ch][rank][_MappingFreqArray[DDRfreq]][DQ]));
                    }
#endif
                }

                mcSHOW_DBG_MSG(("u1CBTVref_Save[%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],p->pSavetimeData->u1CBTVref_Save[ch][rank][_MappingFreqArray[DDRfreq]]));
                mcSHOW_DBG_MSG(("u1CBTClkDelay_Save[%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],p->pSavetimeData->u1CBTClkDelay_Save[ch][rank][_MappingFreqArray[DDRfreq]]));
                mcSHOW_DBG_MSG(("u1CBTCmdDelay_Save[%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],p->pSavetimeData->u1CBTCmdDelay_Save[ch][rank][_MappingFreqArray[DDRfreq]]));
                mcSHOW_DBG_MSG(("u1CBTCsDelay_Save[%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],p->pSavetimeData->u1CBTCsDelay_Save[ch][rank][_MappingFreqArray[DDRfreq]]));
				mcSHOW_DBG_MSG(("u1RxDatlat_Save[%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],p->pSavetimeData->u1RxDatlat_Save[ch][rank][_MappingFreqArray[DDRfreq]]));
//              mcSHOW_DBG_MSG(("u1TxWindowPerbit_Min_FirstPass_Save[%d][%d][%d]=%d;\n",ch,rank,DDRfreq,p->pSavetimeData->u1TxWindowPerbit_Min_FirstPass_Save[ch][rank][DDRfreq]));
                mcSHOW_DBG_MSG(("u1TxWindowPerbitVref_Save[%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],p->pSavetimeData->u1TxWindowPerbitVref_Save[ch][rank][_MappingFreqArray[DDRfreq]]));
//              mcSHOW_DBG_MSG(("u1RxWinPerbit_Min_FirstPass_Save[%d][%d][%d]=%d;\n",ch,rank,DDRfreq,p->pSavetimeData->u1RxWinPerbit_Min_FirstPass_Save[ch][rank][DDRfreq]));
//              mcSHOW_DBG_MSG(("u1RxWinPerbit_Min_LastPass_Save[%d][%d][%d]=%d;\n",ch,rank,DDRfreq,p->pSavetimeData->u1RxWinPerbit_Min_LastPass_Save[ch][rank][DDRfreq]));
			}
			mcSHOW_DBG_MSG(("u1RxWinPerbitVref_Save[%d][%d]=%d;\n",ch,_MappingFreqArray[DDRfreq],p->pSavetimeData->u1RxWinPerbitVref_Save[ch][_MappingFreqArray[DDRfreq]]));
		}
	}

}
#endif
#endif
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
u32 g_dram_save_time_init_done;

DRAM_STATUS_T DramcSave_Time_For_Cal_Init(DRAMC_CTX_T *p)
{
    #if EMMC_READY
    U8 shuffleIdx;
    // scy: only need to read emmc one time for each boot-up
    if (g_dram_save_time_init_done == 1)
        return DRAM_OK;
    else
        g_dram_save_time_init_done = 1;

    for (shuffleIdx = DRAM_DFS_SHUFFLE_1; shuffleIdx < DRAM_DFS_SHUFFLE_MAX; shuffleIdx++)
        _MappingFreqArray[gFreqTbl[shuffleIdx].freq_sel] = shuffleIdx;

    if(!u1IsLP4Family(p->dram_type) || read_offline_dram_calibration_data(p->pSavetimeData)<0)
    {
        p->pSavetimeData->femmc_Ready=0;
        memset(p->pSavetimeData, 0, sizeof(SAVE_TIME_FOR_CALIBRATION_T));
    }
    else
    {
        p->pSavetimeData->femmc_Ready=1;
        p->pSavetimeData->Bypass_RDDQC[_MappingFreqArray[LP4_DDR1600]]=1;
        p->pSavetimeData->Bypass_RXWINDOW[_MappingFreqArray[LP4_DDR1600]]=1;
        p->pSavetimeData->Bypass_TXWINDOW[_MappingFreqArray[LP4_DDR1600]]=1;

        if (LP4_MIDDLE_FREQSEL>LP4_DDR3200)
        {
            p->pSavetimeData->Bypass_RDDQC[_MappingFreqArray[LP4_MIDDLE_FREQSEL]]=1;
            p->pSavetimeData->Bypass_RXWINDOW[_MappingFreqArray[LP4_MIDDLE_FREQSEL]]=1;
            p->pSavetimeData->Bypass_TXWINDOW[_MappingFreqArray[LP4_MIDDLE_FREQSEL]]=1;
        }
        else
        {
            p->pSavetimeData->Bypass_RDDQC[_MappingFreqArray[LP4_MIDDLE_FREQSEL]]=0;
            p->pSavetimeData->Bypass_RXWINDOW[_MappingFreqArray[LP4_MIDDLE_FREQSEL]]=0;
            p->pSavetimeData->Bypass_TXWINDOW[_MappingFreqArray[LP4_MIDDLE_FREQSEL]]=0;
        }
#if RUNTIME_SHMOO_RELEATED_FUNCTION
        // force RXDQC, Rx, Tx without K again for highest frequency (for run time Tx eye scan test)
        p->pSavetimeData->Bypass_RDDQC[_MappingFreqArray[LP4_MIDDLE_FREQSEL]]=1;
        p->pSavetimeData->Bypass_RXWINDOW[_MappingFreqArray[LP4_MIDDLE_FREQSEL]]=1;
        p->pSavetimeData->Bypass_TXWINDOW[_MappingFreqArray[LP4_MIDDLE_FREQSEL]]=1;

        p->pSavetimeData->Bypass_RDDQC[_MappingFreqArray[LP4_HIGHEST_FREQSEL]]=1;
        p->pSavetimeData->Bypass_RXWINDOW[_MappingFreqArray[LP4_HIGHEST_FREQSEL]]=1;
        p->pSavetimeData->Bypass_TXWINDOW[_MappingFreqArray[LP4_HIGHEST_FREQSEL]]=1;
#else
        p->pSavetimeData->Bypass_RDDQC[_MappingFreqArray[LP4_HIGHEST_FREQSEL]]=0;
        p->pSavetimeData->Bypass_RXWINDOW[_MappingFreqArray[LP4_HIGHEST_FREQSEL]]=0;
        p->pSavetimeData->Bypass_TXWINDOW[_MappingFreqArray[LP4_HIGHEST_FREQSEL]]=0;
#endif
    }

#if FORLOOP_TEST_SAVE_TIME_FOR_CALIBRATION_ON_ETT
    mcSHOW_DBG_MSG(("[save time for cal] dump calibration data\n"));
    vDramfirstCalibrationdump(p);
#endif


    #else // EMMC_READY==0
    memset(p->pSavetimeData, 0, sizeof(SAVE_TIME_FOR_CALIBRATION_T));
    p->pSavetimeData->femmc_Ready=1;

    p->pSavetimeData->Bypass_RDDQC[_MappingFreqArray[5]]=1;
    p->pSavetimeData->Bypass_RDDQC[_MappingFreqArray[2]]=1;
    p->pSavetimeData->Bypass_RDDQC[_MappingFreqArray[1]]=0;
    p->pSavetimeData->Bypass_RXWINDOW[_MappingFreqArray[5]]=1;
    p->pSavetimeData->Bypass_RXWINDOW[_MappingFreqArray[2]]=1;
    p->pSavetimeData->Bypass_RXWINDOW[_MappingFreqArray[1]]=0;
    p->pSavetimeData->Bypass_TXWINDOW[_MappingFreqArray[5]]=1;
    p->pSavetimeData->Bypass_TXWINDOW[_MappingFreqArray[2]]=1;
    p->pSavetimeData->Bypass_TXWINDOW[_MappingFreqArray[1]]=0;

    #if 1 //offline calibration data
    p->pSavetimeData->u1Gating2T_Save[0][0][2][0]=2;
    p->pSavetimeData->u1Gating05T_Save[0][0][2][0]=0;
    p->pSavetimeData->u1Gatingfine_tune_Save[0][0][2][0]=4;
    p->pSavetimeData->u1Gatingucpass_count_Save[0][0][2][0]=47;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[0][0][2][0]=27;
    p->pSavetimeData->u1TxCenter_min_Save[0][0][2][0]=57;
    p->pSavetimeData->u1TxCenter_max_Save[0][0][2][0]=58;
    p->pSavetimeData->u1Gating2T_Save[0][0][2][1]=2;
    p->pSavetimeData->u1Gating05T_Save[0][0][2][1]=0;
    p->pSavetimeData->u1Gatingfine_tune_Save[0][0][2][1]=4;
    p->pSavetimeData->u1Gatingucpass_count_Save[0][0][2][1]=47;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[0][0][2][1]=27;
    p->pSavetimeData->u1TxCenter_min_Save[0][0][2][1]=56;
    p->pSavetimeData->u1TxCenter_max_Save[0][0][2][1]=57;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][2][0]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][2][0]=30;
    p->pSavetimeData->u1Txwin_center_Save[0][0][2][0]=57;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][2][0]=43;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][2][0]=72;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[0][0][2][0]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][2][1]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][2][1]=30;
    p->pSavetimeData->u1Txwin_center_Save[0][0][2][1]=57;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][2][1]=43;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][2][1]=72;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[0][0][2][1]=76;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][2][2]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][2][2]=28;
    p->pSavetimeData->u1Txwin_center_Save[0][0][2][2]=57;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][2][2]=43;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][2][2]=72;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[0][0][2][2]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][2][3]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][2][3]=28;
    p->pSavetimeData->u1Txwin_center_Save[0][0][2][3]=57;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][2][3]=42;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][2][3]=72;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[0][0][2][3]=128;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][2][4]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][2][4]=30;
    p->pSavetimeData->u1Txwin_center_Save[0][0][2][4]=57;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][2][4]=43;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][2][4]=72;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[0][0][2][4]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][2][5]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][2][5]=30;
    p->pSavetimeData->u1Txwin_center_Save[0][0][2][5]=58;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][2][5]=43;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][2][5]=73;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[0][0][2][5]=4;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][2][6]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][2][6]=28;
    p->pSavetimeData->u1Txwin_center_Save[0][0][2][6]=57;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][2][6]=43;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][2][6]=72;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][2][7]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][2][7]=28;
    p->pSavetimeData->u1Txwin_center_Save[0][0][2][7]=58;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][2][7]=43;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][2][7]=73;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][2][8]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][2][8]=28;
    p->pSavetimeData->u1Txwin_center_Save[0][0][2][8]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][2][8]=42;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][2][8]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][2][9]=-87;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][2][9]=26;
    p->pSavetimeData->u1Txwin_center_Save[0][0][2][9]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][2][9]=42;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][2][9]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][2][10]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][2][10]=28;
    p->pSavetimeData->u1Txwin_center_Save[0][0][2][10]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][2][10]=42;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][2][10]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][2][11]=-87;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][2][11]=28;
    p->pSavetimeData->u1Txwin_center_Save[0][0][2][11]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][2][11]=42;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][2][11]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][2][12]=-87;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][2][12]=28;
    p->pSavetimeData->u1Txwin_center_Save[0][0][2][12]=57;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][2][12]=42;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][2][12]=72;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][2][13]=-87;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][2][13]=28;
    p->pSavetimeData->u1Txwin_center_Save[0][0][2][13]=57;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][2][13]=42;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][2][13]=72;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][2][14]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][2][14]=28;
    p->pSavetimeData->u1Txwin_center_Save[0][0][2][14]=57;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][2][14]=42;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][2][14]=72;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][2][15]=-87;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][2][15]=26;
    p->pSavetimeData->u1Txwin_center_Save[0][0][2][15]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][2][15]=42;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][2][15]=71;
    p->pSavetimeData->u1CBTVref_Save[0][0][2]=33;
    p->pSavetimeData->u1CBTClkDelay_Save[0][0][2]=0;
    p->pSavetimeData->u1CBTCmdDelay_Save[0][0][2]=1;
    p->pSavetimeData->u1CBTCsDelay_Save[0][0][2]=2;
    p->pSavetimeData->u1RxDatlat_Save[0][0][2]=10;
    p->pSavetimeData->u1TxWindowPerbitVref_Save[0][0][2]=30;
    p->pSavetimeData->u1Gating2T_Save[0][1][2][0]=2;
    p->pSavetimeData->u1Gating05T_Save[0][1][2][0]=0;
    p->pSavetimeData->u1Gatingfine_tune_Save[0][1][2][0]=4;
    p->pSavetimeData->u1Gatingucpass_count_Save[0][1][2][0]=48;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[0][1][2][0]=26;
    p->pSavetimeData->u1TxCenter_min_Save[0][1][2][0]=55;
    p->pSavetimeData->u1TxCenter_max_Save[0][1][2][0]=56;
    p->pSavetimeData->u1Gating2T_Save[0][1][2][1]=2;
    p->pSavetimeData->u1Gating05T_Save[0][1][2][1]=0;
    p->pSavetimeData->u1Gatingfine_tune_Save[0][1][2][1]=4;
    p->pSavetimeData->u1Gatingucpass_count_Save[0][1][2][1]=48;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[0][1][2][1]=26;
    p->pSavetimeData->u1TxCenter_min_Save[0][1][2][1]=56;
    p->pSavetimeData->u1TxCenter_max_Save[0][1][2][1]=56;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][2][0]=-95;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][2][0]=30;
    p->pSavetimeData->u1Txwin_center_Save[0][1][2][0]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][2][0]=42;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][2][0]=70;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[0][1][2][0]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][2][1]=-93;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][2][1]=30;
    p->pSavetimeData->u1Txwin_center_Save[0][1][2][1]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][2][1]=42;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][2][1]=70;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[0][1][2][1]=76;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][2][2]=-93;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][2][2]=30;
    p->pSavetimeData->u1Txwin_center_Save[0][1][2][2]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][2][2]=42;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][2][2]=70;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[0][1][2][2]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][2][3]=-95;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][2][3]=30;
    p->pSavetimeData->u1Txwin_center_Save[0][1][2][3]=55;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][2][3]=42;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][2][3]=69;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[0][1][2][3]=128;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][2][4]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][2][4]=30;
    p->pSavetimeData->u1Txwin_center_Save[0][1][2][4]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][2][4]=42;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][2][4]=70;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[0][1][2][4]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][2][5]=-93;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][2][5]=32;
    p->pSavetimeData->u1Txwin_center_Save[0][1][2][5]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][2][5]=42;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][2][5]=70;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[0][1][2][5]=4;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][2][6]=-93;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][2][6]=30;
    p->pSavetimeData->u1Txwin_center_Save[0][1][2][6]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][2][6]=42;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][2][6]=70;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][2][7]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][2][7]=30;
    p->pSavetimeData->u1Txwin_center_Save[0][1][2][7]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][2][7]=42;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][2][7]=70;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][2][8]=-93;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][2][8]=30;
    p->pSavetimeData->u1Txwin_center_Save[0][1][2][8]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][2][8]=41;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][2][8]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][2][9]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][2][9]=28;
    p->pSavetimeData->u1Txwin_center_Save[0][1][2][9]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][2][9]=41;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][2][9]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][2][10]=-93;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][2][10]=30;
    p->pSavetimeData->u1Txwin_center_Save[0][1][2][10]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][2][10]=41;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][2][10]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][2][11]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][2][11]=30;
    p->pSavetimeData->u1Txwin_center_Save[0][1][2][11]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][2][11]=41;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][2][11]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][2][12]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][2][12]=30;
    p->pSavetimeData->u1Txwin_center_Save[0][1][2][12]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][2][12]=41;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][2][12]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][2][13]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][2][13]=30;
    p->pSavetimeData->u1Txwin_center_Save[0][1][2][13]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][2][13]=41;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][2][13]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][2][14]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][2][14]=30;
    p->pSavetimeData->u1Txwin_center_Save[0][1][2][14]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][2][14]=42;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][2][14]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][2][15]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][2][15]=28;
    p->pSavetimeData->u1Txwin_center_Save[0][1][2][15]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][2][15]=41;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][2][15]=71;
    p->pSavetimeData->u1CBTVref_Save[0][1][2]=31;
    p->pSavetimeData->u1CBTClkDelay_Save[0][1][2]=0;
    p->pSavetimeData->u1CBTCmdDelay_Save[0][1][2]=1;
    p->pSavetimeData->u1CBTCsDelay_Save[0][1][2]=1;
    p->pSavetimeData->u1RxDatlat_Save[0][1][2]=10;
    p->pSavetimeData->u1TxWindowPerbitVref_Save[0][1][2]=28;
    p->pSavetimeData->u1RxWinPerbitVref_Save[0][2]=48;
    p->pSavetimeData->u1Gating2T_Save[1][0][2][0]=2;
    p->pSavetimeData->u1Gating05T_Save[1][0][2][0]=0;
    p->pSavetimeData->u1Gatingfine_tune_Save[1][0][2][0]=4;
    p->pSavetimeData->u1Gatingucpass_count_Save[1][0][2][0]=48;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[1][0][2][0]=27;
    p->pSavetimeData->u1TxCenter_min_Save[1][0][2][0]=56;
    p->pSavetimeData->u1TxCenter_max_Save[1][0][2][0]=57;
    p->pSavetimeData->u1Gating2T_Save[1][0][2][1]=2;
    p->pSavetimeData->u1Gating05T_Save[1][0][2][1]=0;
    p->pSavetimeData->u1Gatingfine_tune_Save[1][0][2][1]=4;
    p->pSavetimeData->u1Gatingucpass_count_Save[1][0][2][1]=47;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[1][0][2][1]=26;
    p->pSavetimeData->u1TxCenter_min_Save[1][0][2][1]=55;
    p->pSavetimeData->u1TxCenter_max_Save[1][0][2][1]=56;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][2][0]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][2][0]=28;
    p->pSavetimeData->u1Txwin_center_Save[1][0][2][0]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][2][0]=42;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][2][0]=71;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[1][0][2][0]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][2][1]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][2][1]=28;
    p->pSavetimeData->u1Txwin_center_Save[1][0][2][1]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][2][1]=42;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][2][1]=70;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[1][0][2][1]=76;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][2][2]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][2][2]=28;
    p->pSavetimeData->u1Txwin_center_Save[1][0][2][2]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][2][2]=42;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][2][2]=70;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[1][0][2][2]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][2][3]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][2][3]=28;
    p->pSavetimeData->u1Txwin_center_Save[1][0][2][3]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][2][3]=42;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][2][3]=70;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[1][0][2][3]=128;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][2][4]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][2][4]=28;
    p->pSavetimeData->u1Txwin_center_Save[1][0][2][4]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][2][4]=42;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][2][4]=70;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[1][0][2][4]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][2][5]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][2][5]=28;
    p->pSavetimeData->u1Txwin_center_Save[1][0][2][5]=57;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][2][5]=43;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][2][5]=71;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[1][0][2][5]=4;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][2][6]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][2][6]=28;
    p->pSavetimeData->u1Txwin_center_Save[1][0][2][6]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][2][6]=42;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][2][6]=70;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][2][7]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][2][7]=28;
    p->pSavetimeData->u1Txwin_center_Save[1][0][2][7]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][2][7]=42;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][2][7]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][2][8]=-93;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][2][8]=28;
    p->pSavetimeData->u1Txwin_center_Save[1][0][2][8]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][2][8]=41;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][2][8]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][2][9]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][2][9]=26;
    p->pSavetimeData->u1Txwin_center_Save[1][0][2][9]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][2][9]=41;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][2][9]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][2][10]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][2][10]=28;
    p->pSavetimeData->u1Txwin_center_Save[1][0][2][10]=55;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][2][10]=41;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][2][10]=70;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][2][11]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][2][11]=26;
    p->pSavetimeData->u1Txwin_center_Save[1][0][2][11]=55;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][2][11]=41;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][2][11]=70;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][2][12]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][2][12]=30;
    p->pSavetimeData->u1Txwin_center_Save[1][0][2][12]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][2][12]=41;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][2][12]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][2][13]=-87;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][2][13]=28;
    p->pSavetimeData->u1Txwin_center_Save[1][0][2][13]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][2][13]=41;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][2][13]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][2][14]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][2][14]=28;
    p->pSavetimeData->u1Txwin_center_Save[1][0][2][14]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][2][14]=41;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][2][14]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][2][15]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][2][15]=28;
    p->pSavetimeData->u1Txwin_center_Save[1][0][2][15]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][2][15]=41;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][2][15]=71;
    p->pSavetimeData->u1CBTVref_Save[1][0][2]=31;
    p->pSavetimeData->u1CBTClkDelay_Save[1][0][2]=0;
    p->pSavetimeData->u1CBTCmdDelay_Save[1][0][2]=1;
    p->pSavetimeData->u1CBTCsDelay_Save[1][0][2]=1;
    p->pSavetimeData->u1RxDatlat_Save[1][0][2]=10;
    p->pSavetimeData->u1TxWindowPerbitVref_Save[1][0][2]=30;
    p->pSavetimeData->u1Gating2T_Save[1][1][2][0]=2;
    p->pSavetimeData->u1Gating05T_Save[1][1][2][0]=0;
    p->pSavetimeData->u1Gatingfine_tune_Save[1][1][2][0]=4;
    p->pSavetimeData->u1Gatingucpass_count_Save[1][1][2][0]=48;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[1][1][2][0]=27;
    p->pSavetimeData->u1TxCenter_min_Save[1][1][2][0]=56;
    p->pSavetimeData->u1TxCenter_max_Save[1][1][2][0]=57;
    p->pSavetimeData->u1Gating2T_Save[1][1][2][1]=2;
    p->pSavetimeData->u1Gating05T_Save[1][1][2][1]=0;
    p->pSavetimeData->u1Gatingfine_tune_Save[1][1][2][1]=4;
    p->pSavetimeData->u1Gatingucpass_count_Save[1][1][2][1]=48;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[1][1][2][1]=27;
    p->pSavetimeData->u1TxCenter_min_Save[1][1][2][1]=56;
    p->pSavetimeData->u1TxCenter_max_Save[1][1][2][1]=57;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][2][0]=-93;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][2][0]=30;
    p->pSavetimeData->u1Txwin_center_Save[1][1][2][0]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][2][0]=42;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][2][0]=71;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[1][1][2][0]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][2][1]=-93;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][2][1]=30;
    p->pSavetimeData->u1Txwin_center_Save[1][1][2][1]=57;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][2][1]=43;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][2][1]=71;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[1][1][2][1]=76;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][2][2]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][2][2]=30;
    p->pSavetimeData->u1Txwin_center_Save[1][1][2][2]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][2][2]=42;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][2][2]=71;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[1][1][2][2]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][2][3]=-93;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][2][3]=30;
    p->pSavetimeData->u1Txwin_center_Save[1][1][2][3]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][2][3]=42;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][2][3]=71;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[1][1][2][3]=128;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][2][4]=-93;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][2][4]=30;
    p->pSavetimeData->u1Txwin_center_Save[1][1][2][4]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][2][4]=42;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][2][4]=71;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[1][1][2][4]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][2][5]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][2][5]=32;
    p->pSavetimeData->u1Txwin_center_Save[1][1][2][5]=57;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][2][5]=43;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][2][5]=71;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[1][1][2][5]=4;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][2][6]=-93;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][2][6]=28;
    p->pSavetimeData->u1Txwin_center_Save[1][1][2][6]=57;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][2][6]=43;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][2][6]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][2][7]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][2][7]=30;
    p->pSavetimeData->u1Txwin_center_Save[1][1][2][7]=57;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][2][7]=43;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][2][7]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][2][8]=-93;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][2][8]=30;
    p->pSavetimeData->u1Txwin_center_Save[1][1][2][8]=57;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][2][8]=42;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][2][8]=72;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][2][9]=-93;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][2][9]=30;
    p->pSavetimeData->u1Txwin_center_Save[1][1][2][9]=57;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][2][9]=42;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][2][9]=72;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][2][10]=-93;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][2][10]=30;
    p->pSavetimeData->u1Txwin_center_Save[1][1][2][10]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][2][10]=42;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][2][10]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][2][11]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][2][11]=30;
    p->pSavetimeData->u1Txwin_center_Save[1][1][2][11]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][2][11]=42;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][2][11]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][2][12]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][2][12]=32;
    p->pSavetimeData->u1Txwin_center_Save[1][1][2][12]=57;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][2][12]=42;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][2][12]=72;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][2][13]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][2][13]=30;
    p->pSavetimeData->u1Txwin_center_Save[1][1][2][13]=57;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][2][13]=42;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][2][13]=72;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][2][14]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][2][14]=30;
    p->pSavetimeData->u1Txwin_center_Save[1][1][2][14]=57;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][2][14]=42;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][2][14]=72;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][2][15]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][2][15]=30;
    p->pSavetimeData->u1Txwin_center_Save[1][1][2][15]=57;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][2][15]=42;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][2][15]=72;
    p->pSavetimeData->u1CBTVref_Save[1][1][2]=31;
    p->pSavetimeData->u1CBTClkDelay_Save[1][1][2]=0;
    p->pSavetimeData->u1CBTCmdDelay_Save[1][1][2]=1;
    p->pSavetimeData->u1CBTCsDelay_Save[1][1][2]=1;
    p->pSavetimeData->u1RxDatlat_Save[1][1][2]=10;
    p->pSavetimeData->u1TxWindowPerbitVref_Save[1][1][2]=28;
    p->pSavetimeData->u1RxWinPerbitVref_Save[1][2]=48;
    p->pSavetimeData->u1Gating2T_Save[2][0][2][0]=2;
    p->pSavetimeData->u1Gating05T_Save[2][0][2][0]=0;
    p->pSavetimeData->u1Gatingfine_tune_Save[2][0][2][0]=4;
    p->pSavetimeData->u1Gatingucpass_count_Save[2][0][2][0]=47;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[2][0][2][0]=26;
    p->pSavetimeData->u1TxCenter_min_Save[2][0][2][0]=56;
    p->pSavetimeData->u1TxCenter_max_Save[2][0][2][0]=56;
    p->pSavetimeData->u1Gating2T_Save[2][0][2][1]=2;
    p->pSavetimeData->u1Gating05T_Save[2][0][2][1]=0;
    p->pSavetimeData->u1Gatingfine_tune_Save[2][0][2][1]=4;
    p->pSavetimeData->u1Gatingucpass_count_Save[2][0][2][1]=48;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[2][0][2][1]=26;
    p->pSavetimeData->u1TxCenter_min_Save[2][0][2][1]=56;
    p->pSavetimeData->u1TxCenter_max_Save[2][0][2][1]=57;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][2][0]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][2][0]=26;
    p->pSavetimeData->u1Txwin_center_Save[2][0][2][0]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][2][0]=42;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][2][0]=70;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[2][0][2][0]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][2][1]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][2][1]=28;
    p->pSavetimeData->u1Txwin_center_Save[2][0][2][1]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][2][1]=42;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][2][1]=70;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[2][0][2][1]=76;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][2][2]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][2][2]=26;
    p->pSavetimeData->u1Txwin_center_Save[2][0][2][2]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][2][2]=42;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][2][2]=70;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[2][0][2][2]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][2][3]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][2][3]=28;
    p->pSavetimeData->u1Txwin_center_Save[2][0][2][3]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][2][3]=42;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][2][3]=70;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[2][0][2][3]=128;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][2][4]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][2][4]=30;
    p->pSavetimeData->u1Txwin_center_Save[2][0][2][4]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][2][4]=42;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][2][4]=70;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[2][0][2][4]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][2][5]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][2][5]=26;
    p->pSavetimeData->u1Txwin_center_Save[2][0][2][5]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][2][5]=42;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][2][5]=71;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[2][0][2][5]=4;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][2][6]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][2][6]=24;
    p->pSavetimeData->u1Txwin_center_Save[2][0][2][6]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][2][6]=42;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][2][6]=70;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][2][7]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][2][7]=28;
    p->pSavetimeData->u1Txwin_center_Save[2][0][2][7]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][2][7]=42;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][2][7]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][2][8]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][2][8]=26;
    p->pSavetimeData->u1Txwin_center_Save[2][0][2][8]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][2][8]=41;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][2][8]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][2][9]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][2][9]=28;
    p->pSavetimeData->u1Txwin_center_Save[2][0][2][9]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][2][9]=41;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][2][9]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][2][10]=-93;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][2][10]=28;
    p->pSavetimeData->u1Txwin_center_Save[2][0][2][10]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][2][10]=41;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][2][10]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][2][11]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][2][11]=28;
    p->pSavetimeData->u1Txwin_center_Save[2][0][2][11]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][2][11]=41;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][2][11]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][2][12]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][2][12]=28;
    p->pSavetimeData->u1Txwin_center_Save[2][0][2][12]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][2][12]=42;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][2][12]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][2][13]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][2][13]=28;
    p->pSavetimeData->u1Txwin_center_Save[2][0][2][13]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][2][13]=42;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][2][13]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][2][14]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][2][14]=26;
    p->pSavetimeData->u1Txwin_center_Save[2][0][2][14]=57;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][2][14]=42;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][2][14]=72;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][2][15]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][2][15]=28;
    p->pSavetimeData->u1Txwin_center_Save[2][0][2][15]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][2][15]=41;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][2][15]=71;
    p->pSavetimeData->u1CBTVref_Save[2][0][2]=31;
    p->pSavetimeData->u1CBTClkDelay_Save[2][0][2]=0;
    p->pSavetimeData->u1CBTCmdDelay_Save[2][0][2]=2;
    p->pSavetimeData->u1CBTCsDelay_Save[2][0][2]=1;
    p->pSavetimeData->u1RxDatlat_Save[2][0][2]=10;
    p->pSavetimeData->u1TxWindowPerbitVref_Save[2][0][2]=34;
    p->pSavetimeData->u1Gating2T_Save[2][1][2][0]=2;
    p->pSavetimeData->u1Gating05T_Save[2][1][2][0]=0;
    p->pSavetimeData->u1Gatingfine_tune_Save[2][1][2][0]=4;
    p->pSavetimeData->u1Gatingucpass_count_Save[2][1][2][0]=48;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[2][1][2][0]=27;
    p->pSavetimeData->u1TxCenter_min_Save[2][1][2][0]=58;
    p->pSavetimeData->u1TxCenter_max_Save[2][1][2][0]=58;
    p->pSavetimeData->u1Gating2T_Save[2][1][2][1]=2;
    p->pSavetimeData->u1Gating05T_Save[2][1][2][1]=0;
    p->pSavetimeData->u1Gatingfine_tune_Save[2][1][2][1]=4;
    p->pSavetimeData->u1Gatingucpass_count_Save[2][1][2][1]=47;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[2][1][2][1]=27;
    p->pSavetimeData->u1TxCenter_min_Save[2][1][2][1]=57;
    p->pSavetimeData->u1TxCenter_max_Save[2][1][2][1]=57;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][2][0]=-95;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][2][0]=30;
    p->pSavetimeData->u1Txwin_center_Save[2][1][2][0]=58;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][2][0]=43;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][2][0]=73;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[2][1][2][0]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][2][1]=-95;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][2][1]=30;
    p->pSavetimeData->u1Txwin_center_Save[2][1][2][1]=58;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][2][1]=43;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][2][1]=73;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[2][1][2][1]=76;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][2][2]=-95;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][2][2]=32;
    p->pSavetimeData->u1Txwin_center_Save[2][1][2][2]=58;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][2][2]=43;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][2][2]=73;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[2][1][2][2]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][2][3]=-95;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][2][3]=30;
    p->pSavetimeData->u1Txwin_center_Save[2][1][2][3]=58;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][2][3]=43;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][2][3]=73;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[2][1][2][3]=128;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][2][4]=-93;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][2][4]=32;
    p->pSavetimeData->u1Txwin_center_Save[2][1][2][4]=58;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][2][4]=43;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][2][4]=73;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[2][1][2][4]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][2][5]=-93;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][2][5]=32;
    p->pSavetimeData->u1Txwin_center_Save[2][1][2][5]=58;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][2][5]=43;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][2][5]=73;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[2][1][2][5]=4;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][2][6]=-93;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][2][6]=30;
    p->pSavetimeData->u1Txwin_center_Save[2][1][2][6]=58;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][2][6]=43;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][2][6]=73;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][2][7]=-93;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][2][7]=30;
    p->pSavetimeData->u1Txwin_center_Save[2][1][2][7]=58;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][2][7]=43;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][2][7]=73;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][2][8]=-95;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][2][8]=30;
    p->pSavetimeData->u1Txwin_center_Save[2][1][2][8]=57;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][2][8]=42;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][2][8]=72;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][2][9]=-95;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][2][9]=30;
    p->pSavetimeData->u1Txwin_center_Save[2][1][2][9]=57;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][2][9]=42;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][2][9]=72;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][2][10]=-95;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][2][10]=28;
    p->pSavetimeData->u1Txwin_center_Save[2][1][2][10]=57;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][2][10]=42;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][2][10]=72;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][2][11]=-93;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][2][11]=30;
    p->pSavetimeData->u1Txwin_center_Save[2][1][2][11]=57;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][2][11]=42;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][2][11]=72;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][2][12]=-93;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][2][12]=32;
    p->pSavetimeData->u1Txwin_center_Save[2][1][2][12]=57;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][2][12]=43;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][2][12]=72;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][2][13]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][2][13]=32;
    p->pSavetimeData->u1Txwin_center_Save[2][1][2][13]=57;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][2][13]=43;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][2][13]=72;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][2][14]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][2][14]=30;
    p->pSavetimeData->u1Txwin_center_Save[2][1][2][14]=57;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][2][14]=43;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][2][14]=72;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][2][15]=-93;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][2][15]=32;
    p->pSavetimeData->u1Txwin_center_Save[2][1][2][15]=57;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][2][15]=42;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][2][15]=72;
    p->pSavetimeData->u1CBTVref_Save[2][1][2]=31;
    p->pSavetimeData->u1CBTClkDelay_Save[2][1][2]=0;
    p->pSavetimeData->u1CBTCmdDelay_Save[2][1][2]=1;
    p->pSavetimeData->u1CBTCsDelay_Save[2][1][2]=1;
    p->pSavetimeData->u1RxDatlat_Save[2][1][2]=10;
    p->pSavetimeData->u1TxWindowPerbitVref_Save[2][1][2]=32;
    p->pSavetimeData->u1RxWinPerbitVref_Save[2][2]=48;
    p->pSavetimeData->u1Gating2T_Save[3][0][2][0]=2;
    p->pSavetimeData->u1Gating05T_Save[3][0][2][0]=0;
    p->pSavetimeData->u1Gatingfine_tune_Save[3][0][2][0]=4;
    p->pSavetimeData->u1Gatingucpass_count_Save[3][0][2][0]=48;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[3][0][2][0]=26;
    p->pSavetimeData->u1TxCenter_min_Save[3][0][2][0]=55;
    p->pSavetimeData->u1TxCenter_max_Save[3][0][2][0]=56;
    p->pSavetimeData->u1Gating2T_Save[3][0][2][1]=2;
    p->pSavetimeData->u1Gating05T_Save[3][0][2][1]=0;
    p->pSavetimeData->u1Gatingfine_tune_Save[3][0][2][1]=4;
    p->pSavetimeData->u1Gatingucpass_count_Save[3][0][2][1]=48;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[3][0][2][1]=26;
    p->pSavetimeData->u1TxCenter_min_Save[3][0][2][1]=56;
    p->pSavetimeData->u1TxCenter_max_Save[3][0][2][1]=56;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][2][0]=-93;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][2][0]=28;
    p->pSavetimeData->u1Txwin_center_Save[3][0][2][0]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][2][0]=42;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][2][0]=70;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[3][0][2][0]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][2][1]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][2][1]=28;
    p->pSavetimeData->u1Txwin_center_Save[3][0][2][1]=55;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][2][1]=42;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][2][1]=69;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[3][0][2][1]=76;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][2][2]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][2][2]=28;
    p->pSavetimeData->u1Txwin_center_Save[3][0][2][2]=55;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][2][2]=41;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][2][2]=69;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[3][0][2][2]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][2][3]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][2][3]=28;
    p->pSavetimeData->u1Txwin_center_Save[3][0][2][3]=55;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][2][3]=42;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][2][3]=69;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[3][0][2][3]=128;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][2][4]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][2][4]=28;
    p->pSavetimeData->u1Txwin_center_Save[3][0][2][4]=55;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][2][4]=42;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][2][4]=69;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[3][0][2][4]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][2][5]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][2][5]=28;
    p->pSavetimeData->u1Txwin_center_Save[3][0][2][5]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][2][5]=42;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][2][5]=70;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[3][0][2][5]=4;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][2][6]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][2][6]=26;
    p->pSavetimeData->u1Txwin_center_Save[3][0][2][6]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][2][6]=42;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][2][6]=70;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][2][7]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][2][7]=26;
    p->pSavetimeData->u1Txwin_center_Save[3][0][2][7]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][2][7]=42;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][2][7]=70;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][2][8]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][2][8]=26;
    p->pSavetimeData->u1Txwin_center_Save[3][0][2][8]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][2][8]=41;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][2][8]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][2][9]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][2][9]=26;
    p->pSavetimeData->u1Txwin_center_Save[3][0][2][9]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][2][9]=41;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][2][9]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][2][10]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][2][10]=28;
    p->pSavetimeData->u1Txwin_center_Save[3][0][2][10]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][2][10]=41;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][2][10]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][2][11]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][2][11]=28;
    p->pSavetimeData->u1Txwin_center_Save[3][0][2][11]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][2][11]=41;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][2][11]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][2][12]=-87;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][2][12]=30;
    p->pSavetimeData->u1Txwin_center_Save[3][0][2][12]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][2][12]=41;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][2][12]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][2][13]=-87;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][2][13]=30;
    p->pSavetimeData->u1Txwin_center_Save[3][0][2][13]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][2][13]=42;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][2][13]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][2][14]=-87;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][2][14]=30;
    p->pSavetimeData->u1Txwin_center_Save[3][0][2][14]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][2][14]=42;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][2][14]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][2][15]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][2][15]=28;
    p->pSavetimeData->u1Txwin_center_Save[3][0][2][15]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][2][15]=41;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][2][15]=71;
    p->pSavetimeData->u1CBTVref_Save[3][0][2]=35;
    p->pSavetimeData->u1CBTClkDelay_Save[3][0][2]=0;
    p->pSavetimeData->u1CBTCmdDelay_Save[3][0][2]=1;
    p->pSavetimeData->u1CBTCsDelay_Save[3][0][2]=2;
    p->pSavetimeData->u1RxDatlat_Save[3][0][2]=10;
    p->pSavetimeData->u1TxWindowPerbitVref_Save[3][0][2]=30;
    p->pSavetimeData->u1Gating2T_Save[3][1][2][0]=2;
    p->pSavetimeData->u1Gating05T_Save[3][1][2][0]=0;
    p->pSavetimeData->u1Gatingfine_tune_Save[3][1][2][0]=8;
    p->pSavetimeData->u1Gatingucpass_count_Save[3][1][2][0]=47;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[3][1][2][0]=26;
    p->pSavetimeData->u1TxCenter_min_Save[3][1][2][0]=55;
    p->pSavetimeData->u1TxCenter_max_Save[3][1][2][0]=56;
    p->pSavetimeData->u1Gating2T_Save[3][1][2][1]=2;
    p->pSavetimeData->u1Gating05T_Save[3][1][2][1]=0;
    p->pSavetimeData->u1Gatingfine_tune_Save[3][1][2][1]=8;
    p->pSavetimeData->u1Gatingucpass_count_Save[3][1][2][1]=47;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[3][1][2][1]=26;
    p->pSavetimeData->u1TxCenter_min_Save[3][1][2][1]=55;
    p->pSavetimeData->u1TxCenter_max_Save[3][1][2][1]=56;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][2][0]=-93;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][2][0]=30;
    p->pSavetimeData->u1Txwin_center_Save[3][1][2][0]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][2][0]=42;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][2][0]=70;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[3][1][2][0]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][2][1]=-93;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][2][1]=30;
    p->pSavetimeData->u1Txwin_center_Save[3][1][2][1]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][2][1]=42;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][2][1]=70;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[3][1][2][1]=76;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][2][2]=-93;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][2][2]=30;
    p->pSavetimeData->u1Txwin_center_Save[3][1][2][2]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][2][2]=42;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][2][2]=70;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[3][1][2][2]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][2][3]=-93;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][2][3]=30;
    p->pSavetimeData->u1Txwin_center_Save[3][1][2][3]=55;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][2][3]=42;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][2][3]=69;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[3][1][2][3]=128;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][2][4]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][2][4]=30;
    p->pSavetimeData->u1Txwin_center_Save[3][1][2][4]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][2][4]=42;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][2][4]=70;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[3][1][2][4]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][2][5]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][2][5]=30;
    p->pSavetimeData->u1Txwin_center_Save[3][1][2][5]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][2][5]=42;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][2][5]=70;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[3][1][2][5]=4;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][2][6]=-93;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][2][6]=28;
    p->pSavetimeData->u1Txwin_center_Save[3][1][2][6]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][2][6]=42;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][2][6]=70;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][2][7]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][2][7]=30;
    p->pSavetimeData->u1Txwin_center_Save[3][1][2][7]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][2][7]=42;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][2][7]=70;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][2][8]=-93;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][2][8]=30;
    p->pSavetimeData->u1Txwin_center_Save[3][1][2][8]=55;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][2][8]=41;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][2][8]=70;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][2][9]=-93;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][2][9]=28;
    p->pSavetimeData->u1Txwin_center_Save[3][1][2][9]=55;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][2][9]=41;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][2][9]=70;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][2][10]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][2][10]=30;
    p->pSavetimeData->u1Txwin_center_Save[3][1][2][10]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][2][10]=41;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][2][10]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][2][11]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][2][11]=30;
    p->pSavetimeData->u1Txwin_center_Save[3][1][2][11]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][2][11]=41;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][2][11]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][2][12]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][2][12]=32;
    p->pSavetimeData->u1Txwin_center_Save[3][1][2][12]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][2][12]=42;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][2][12]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][2][13]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][2][13]=32;
    p->pSavetimeData->u1Txwin_center_Save[3][1][2][13]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][2][13]=42;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][2][13]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][2][14]=-89;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][2][14]=30;
    p->pSavetimeData->u1Txwin_center_Save[3][1][2][14]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][2][14]=42;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][2][14]=71;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][2][15]=-91;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][2][15]=32;
    p->pSavetimeData->u1Txwin_center_Save[3][1][2][15]=56;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][2][15]=41;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][2][15]=71;
    p->pSavetimeData->u1CBTVref_Save[3][1][2]=29;
    p->pSavetimeData->u1CBTClkDelay_Save[3][1][2]=0;
    p->pSavetimeData->u1CBTCmdDelay_Save[3][1][2]=1;
    p->pSavetimeData->u1CBTCsDelay_Save[3][1][2]=2;
    p->pSavetimeData->u1RxDatlat_Save[3][1][2]=10;
    p->pSavetimeData->u1TxWindowPerbitVref_Save[3][1][2]=34;
    p->pSavetimeData->u1RxWinPerbitVref_Save[3][2]=48;
    p->pSavetimeData->u1Gating2T_Save[0][0][1][0]=3;
    p->pSavetimeData->u1Gating05T_Save[0][0][1][0]=2;
    p->pSavetimeData->u1Gatingfine_tune_Save[0][0][1][0]=4;
    p->pSavetimeData->u1Gatingucpass_count_Save[0][0][1][0]=37;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[0][0][1][0]=25;
    p->pSavetimeData->u1TxCenter_min_Save[0][0][1][0]=83;
    p->pSavetimeData->u1TxCenter_max_Save[0][0][1][0]=85;
    p->pSavetimeData->u1Gating2T_Save[0][0][1][1]=3;
    p->pSavetimeData->u1Gating05T_Save[0][0][1][1]=0;
    p->pSavetimeData->u1Gatingfine_tune_Save[0][0][1][1]=24;
    p->pSavetimeData->u1Gatingucpass_count_Save[0][0][1][1]=47;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[0][0][1][1]=25;
    p->pSavetimeData->u1TxCenter_min_Save[0][0][1][1]=83;
    p->pSavetimeData->u1TxCenter_max_Save[0][0][1][1]=85;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][1][0]=-2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][1][0]=29;
    p->pSavetimeData->u1Txwin_center_Save[0][0][1][0]=85;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][1][0]=73;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][1][0]=97;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[0][0][1][0]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][1][1]=-2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][1][1]=29;
    p->pSavetimeData->u1Txwin_center_Save[0][0][1][1]=85;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][1][1]=73;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][1][1]=98;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[0][0][1][1]=76;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][1][2]=-2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][1][2]=28;
    p->pSavetimeData->u1Txwin_center_Save[0][0][1][2]=84;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][1][2]=71;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][1][2]=97;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[0][0][1][2]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][1][3]=-2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][1][3]=26;
    p->pSavetimeData->u1Txwin_center_Save[0][0][1][3]=84;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][1][3]=72;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][1][3]=96;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[0][0][1][3]=128;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][1][4]=-2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][1][4]=29;
    p->pSavetimeData->u1Txwin_center_Save[0][0][1][4]=83;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][1][4]=71;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][1][4]=96;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[0][0][1][4]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][1][5]=-2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][1][5]=29;
    p->pSavetimeData->u1Txwin_center_Save[0][0][1][5]=85;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][1][5]=73;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][1][5]=98;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[0][0][1][5]=4;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][1][6]=-2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][1][6]=29;
    p->pSavetimeData->u1Txwin_center_Save[0][0][1][6]=85;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][1][6]=72;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][1][6]=98;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][1][7]=-2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][1][7]=27;
    p->pSavetimeData->u1Txwin_center_Save[0][0][1][7]=85;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][1][7]=73;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][1][7]=98;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][1][8]=-5;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][1][8]=29;
    p->pSavetimeData->u1Txwin_center_Save[0][0][1][8]=84;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][1][8]=72;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][1][8]=97;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][1][9]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][1][9]=27;
    p->pSavetimeData->u1Txwin_center_Save[0][0][1][9]=84;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][1][9]=72;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][1][9]=96;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][1][10]=-5;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][1][10]=28;
    p->pSavetimeData->u1Txwin_center_Save[0][0][1][10]=84;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][1][10]=71;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][1][10]=98;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][1][11]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][1][11]=29;
    p->pSavetimeData->u1Txwin_center_Save[0][0][1][11]=85;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][1][11]=72;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][1][11]=98;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][1][12]=-5;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][1][12]=29;
    p->pSavetimeData->u1Txwin_center_Save[0][0][1][12]=83;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][1][12]=71;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][1][12]=95;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][1][13]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][1][13]=29;
    p->pSavetimeData->u1Txwin_center_Save[0][0][1][13]=85;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][1][13]=73;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][1][13]=98;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][1][14]=-5;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][1][14]=29;
    p->pSavetimeData->u1Txwin_center_Save[0][0][1][14]=84;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][1][14]=71;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][1][14]=97;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][1][15]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][1][15]=25;
    p->pSavetimeData->u1Txwin_center_Save[0][0][1][15]=85;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][1][15]=73;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][1][15]=98;
    p->pSavetimeData->u1CBTVref_Save[0][0][1]=33;
    p->pSavetimeData->u1CBTClkDelay_Save[0][0][1]=0;
    p->pSavetimeData->u1CBTCmdDelay_Save[0][0][1]=1;
    p->pSavetimeData->u1CBTCsDelay_Save[0][0][1]=2;
    p->pSavetimeData->u1RxDatlat_Save[0][0][1]=14;
    p->pSavetimeData->u1TxWindowPerbitVref_Save[0][0][1]=14;
    p->pSavetimeData->u1Gating2T_Save[0][1][1][0]=3;
    p->pSavetimeData->u1Gating05T_Save[0][1][1][0]=1;
    p->pSavetimeData->u1Gatingfine_tune_Save[0][1][1][0]=4;
    p->pSavetimeData->u1Gatingucpass_count_Save[0][1][1][0]=46;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[0][1][1][0]=24;
    p->pSavetimeData->u1TxCenter_min_Save[0][1][1][0]=82;
    p->pSavetimeData->u1TxCenter_max_Save[0][1][1][0]=84;
    p->pSavetimeData->u1Gating2T_Save[0][1][1][1]=3;
    p->pSavetimeData->u1Gating05T_Save[0][1][1][1]=0;
    p->pSavetimeData->u1Gatingfine_tune_Save[0][1][1][1]=28;
    p->pSavetimeData->u1Gatingucpass_count_Save[0][1][1][1]=47;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[0][1][1][1]=22;
    p->pSavetimeData->u1TxCenter_min_Save[0][1][1][1]=80;
    p->pSavetimeData->u1TxCenter_max_Save[0][1][1][1]=82;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][1][0]=-4;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][1][0]=27;
    p->pSavetimeData->u1Txwin_center_Save[0][1][1][0]=83;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][1][0]=70;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][1][0]=96;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[0][1][1][0]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][1][1]=-4;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][1][1]=30;
    p->pSavetimeData->u1Txwin_center_Save[0][1][1][1]=82;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][1][1]=69;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][1][1]=95;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[0][1][1][1]=76;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][1][2]=-4;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][1][2]=29;
    p->pSavetimeData->u1Txwin_center_Save[0][1][1][2]=83;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][1][2]=71;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][1][2]=96;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[0][1][1][2]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][1][3]=-4;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][1][3]=28;
    p->pSavetimeData->u1Txwin_center_Save[0][1][1][3]=83;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][1][3]=70;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][1][3]=96;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[0][1][1][3]=128;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][1][4]=-4;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][1][4]=30;
    p->pSavetimeData->u1Txwin_center_Save[0][1][1][4]=84;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][1][4]=72;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][1][4]=97;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[0][1][1][4]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][1][5]=-4;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][1][5]=30;
    p->pSavetimeData->u1Txwin_center_Save[0][1][1][5]=83;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][1][5]=70;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][1][5]=97;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[0][1][1][5]=4;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][1][6]=-4;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][1][6]=29;
    p->pSavetimeData->u1Txwin_center_Save[0][1][1][6]=83;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][1][6]=70;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][1][6]=96;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][1][7]=-4;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][1][7]=29;
    p->pSavetimeData->u1Txwin_center_Save[0][1][1][7]=83;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][1][7]=70;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][1][7]=97;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][1][8]=-4;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][1][8]=28;
    p->pSavetimeData->u1Txwin_center_Save[0][1][1][8]=82;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][1][8]=69;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][1][8]=95;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][1][9]=-4;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][1][9]=28;
    p->pSavetimeData->u1Txwin_center_Save[0][1][1][9]=80;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][1][9]=67;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][1][9]=94;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][1][10]=-4;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][1][10]=29;
    p->pSavetimeData->u1Txwin_center_Save[0][1][1][10]=81;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][1][10]=68;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][1][10]=94;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][1][11]=-4;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][1][11]=29;
    p->pSavetimeData->u1Txwin_center_Save[0][1][1][11]=80;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][1][11]=67;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][1][11]=94;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][1][12]=-4;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][1][12]=29;
    p->pSavetimeData->u1Txwin_center_Save[0][1][1][12]=81;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][1][12]=68;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][1][12]=94;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][1][13]=-4;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][1][13]=29;
    p->pSavetimeData->u1Txwin_center_Save[0][1][1][13]=81;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][1][13]=68;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][1][13]=94;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][1][14]=-4;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][1][14]=29;
    p->pSavetimeData->u1Txwin_center_Save[0][1][1][14]=82;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][1][14]=69;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][1][14]=95;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][1][15]=-4;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][1][15]=27;
    p->pSavetimeData->u1Txwin_center_Save[0][1][1][15]=81;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][1][15]=69;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][1][15]=94;
    p->pSavetimeData->u1CBTVref_Save[0][1][1]=33;
    p->pSavetimeData->u1CBTClkDelay_Save[0][1][1]=0;
    p->pSavetimeData->u1CBTCmdDelay_Save[0][1][1]=1;
    p->pSavetimeData->u1CBTCsDelay_Save[0][1][1]=2;
    p->pSavetimeData->u1RxDatlat_Save[0][1][1]=14;
    p->pSavetimeData->u1TxWindowPerbitVref_Save[0][1][1]=18;
    p->pSavetimeData->u1RxWinPerbitVref_Save[0][1]=4;
    p->pSavetimeData->u1Gating2T_Save[1][0][1][0]=3;
    p->pSavetimeData->u1Gating05T_Save[1][0][1][0]=2;
    p->pSavetimeData->u1Gatingfine_tune_Save[1][0][1][0]=12;
    p->pSavetimeData->u1Gatingucpass_count_Save[1][0][1][0]=36;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[1][0][1][0]=23;
    p->pSavetimeData->u1TxCenter_min_Save[1][0][1][0]=81;
    p->pSavetimeData->u1TxCenter_max_Save[1][0][1][0]=84;
    p->pSavetimeData->u1Gating2T_Save[1][0][1][1]=3;
    p->pSavetimeData->u1Gating05T_Save[1][0][1][1]=0;
    p->pSavetimeData->u1Gatingfine_tune_Save[1][0][1][1]=28;
    p->pSavetimeData->u1Gatingucpass_count_Save[1][0][1][1]=48;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[1][0][1][1]=25;
    p->pSavetimeData->u1TxCenter_min_Save[1][0][1][1]=84;
    p->pSavetimeData->u1TxCenter_max_Save[1][0][1][1]=86;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][1][0]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][1][0]=27;
    p->pSavetimeData->u1Txwin_center_Save[1][0][1][0]=83;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][1][0]=70;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][1][0]=97;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[1][0][1][0]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][1][1]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][1][1]=27;
    p->pSavetimeData->u1Txwin_center_Save[1][0][1][1]=82;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][1][1]=69;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][1][1]=95;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[1][0][1][1]=76;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][1][2]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][1][2]=27;
    p->pSavetimeData->u1Txwin_center_Save[1][0][1][2]=83;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][1][2]=69;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][1][2]=97;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[1][0][1][2]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][1][3]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][1][3]=26;
    p->pSavetimeData->u1Txwin_center_Save[1][0][1][3]=81;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][1][3]=68;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][1][3]=94;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[1][0][1][3]=128;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][1][4]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][1][4]=27;
    p->pSavetimeData->u1Txwin_center_Save[1][0][1][4]=83;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][1][4]=69;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][1][4]=97;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[1][0][1][4]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][1][5]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][1][5]=27;
    p->pSavetimeData->u1Txwin_center_Save[1][0][1][5]=84;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][1][5]=70;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][1][5]=98;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[1][0][1][5]=4;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][1][6]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][1][6]=27;
    p->pSavetimeData->u1Txwin_center_Save[1][0][1][6]=83;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][1][6]=70;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][1][6]=96;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][1][7]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][1][7]=27;
    p->pSavetimeData->u1Txwin_center_Save[1][0][1][7]=83;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][1][7]=69;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][1][7]=97;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][1][8]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][1][8]=29;
    p->pSavetimeData->u1Txwin_center_Save[1][0][1][8]=85;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][1][8]=72;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][1][8]=99;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][1][9]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][1][9]=26;
    p->pSavetimeData->u1Txwin_center_Save[1][0][1][9]=84;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][1][9]=71;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][1][9]=97;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][1][10]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][1][10]=27;
    p->pSavetimeData->u1Txwin_center_Save[1][0][1][10]=85;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][1][10]=72;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][1][10]=98;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][1][11]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][1][11]=26;
    p->pSavetimeData->u1Txwin_center_Save[1][0][1][11]=84;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][1][11]=70;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][1][11]=98;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][1][12]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][1][12]=29;
    p->pSavetimeData->u1Txwin_center_Save[1][0][1][12]=86;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][1][12]=73;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][1][12]=100;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][1][13]=-1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][1][13]=28;
    p->pSavetimeData->u1Txwin_center_Save[1][0][1][13]=85;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][1][13]=72;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][1][13]=99;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][1][14]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][1][14]=29;
    p->pSavetimeData->u1Txwin_center_Save[1][0][1][14]=86;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][1][14]=73;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][1][14]=100;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][1][15]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][1][15]=26;
    p->pSavetimeData->u1Txwin_center_Save[1][0][1][15]=85;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][1][15]=71;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][1][15]=99;
    p->pSavetimeData->u1CBTVref_Save[1][0][1]=31;
    p->pSavetimeData->u1CBTClkDelay_Save[1][0][1]=0;
    p->pSavetimeData->u1CBTCmdDelay_Save[1][0][1]=1;
    p->pSavetimeData->u1CBTCsDelay_Save[1][0][1]=2;
    p->pSavetimeData->u1RxDatlat_Save[1][0][1]=14;
    p->pSavetimeData->u1TxWindowPerbitVref_Save[1][0][1]=18;
    p->pSavetimeData->u1Gating2T_Save[1][1][1][0]=3;
    p->pSavetimeData->u1Gating05T_Save[1][1][1][0]=1;
    p->pSavetimeData->u1Gatingfine_tune_Save[1][1][1][0]=12;
    p->pSavetimeData->u1Gatingucpass_count_Save[1][1][1][0]=45;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[1][1][1][0]=24;
    p->pSavetimeData->u1TxCenter_min_Save[1][1][1][0]=82;
    p->pSavetimeData->u1TxCenter_max_Save[1][1][1][0]=83;
    p->pSavetimeData->u1Gating2T_Save[1][1][1][1]=3;
    p->pSavetimeData->u1Gating05T_Save[1][1][1][1]=1;
    p->pSavetimeData->u1Gatingfine_tune_Save[1][1][1][1]=4;
    p->pSavetimeData->u1Gatingucpass_count_Save[1][1][1][1]=48;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[1][1][1][1]=24;
    p->pSavetimeData->u1TxCenter_min_Save[1][1][1][1]=82;
    p->pSavetimeData->u1TxCenter_max_Save[1][1][1][1]=84;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][1][0]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][1][0]=31;
    p->pSavetimeData->u1Txwin_center_Save[1][1][1][0]=83;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][1][0]=70;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][1][0]=97;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[1][1][1][0]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][1][1]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][1][1]=30;
    p->pSavetimeData->u1Txwin_center_Save[1][1][1][1]=83;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][1][1]=69;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][1][1]=97;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[1][1][1][1]=76;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][1][2]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][1][2]=28;
    p->pSavetimeData->u1Txwin_center_Save[1][1][1][2]=82;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][1][2]=68;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][1][2]=97;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[1][1][1][2]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][1][3]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][1][3]=29;
    p->pSavetimeData->u1Txwin_center_Save[1][1][1][3]=83;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][1][3]=69;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][1][3]=97;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[1][1][1][3]=128;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][1][4]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][1][4]=30;
    p->pSavetimeData->u1Txwin_center_Save[1][1][1][4]=82;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][1][4]=68;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][1][4]=96;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[1][1][1][4]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][1][5]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][1][5]=31;
    p->pSavetimeData->u1Txwin_center_Save[1][1][1][5]=83;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][1][5]=69;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][1][5]=97;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[1][1][1][5]=4;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][1][6]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][1][6]=29;
    p->pSavetimeData->u1Txwin_center_Save[1][1][1][6]=82;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][1][6]=69;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][1][6]=95;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][1][7]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][1][7]=31;
    p->pSavetimeData->u1Txwin_center_Save[1][1][1][7]=83;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][1][7]=70;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][1][7]=97;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][1][8]=-6;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][1][8]=28;
    p->pSavetimeData->u1Txwin_center_Save[1][1][1][8]=84;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][1][8]=71;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][1][8]=97;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][1][9]=-6;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][1][9]=28;
    p->pSavetimeData->u1Txwin_center_Save[1][1][1][9]=84;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][1][9]=71;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][1][9]=97;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][1][10]=-6;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][1][10]=29;
    p->pSavetimeData->u1Txwin_center_Save[1][1][1][10]=82;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][1][10]=68;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][1][10]=97;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][1][11]=-5;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][1][11]=28;
    p->pSavetimeData->u1Txwin_center_Save[1][1][1][11]=84;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][1][11]=72;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][1][11]=97;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][1][12]=-5;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][1][12]=30;
    p->pSavetimeData->u1Txwin_center_Save[1][1][1][12]=83;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][1][12]=69;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][1][12]=97;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][1][13]=-5;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][1][13]=29;
    p->pSavetimeData->u1Txwin_center_Save[1][1][1][13]=84;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][1][13]=72;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][1][13]=97;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][1][14]=-6;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][1][14]=30;
    p->pSavetimeData->u1Txwin_center_Save[1][1][1][14]=84;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][1][14]=71;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][1][14]=97;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][1][15]=-6;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][1][15]=29;
    p->pSavetimeData->u1Txwin_center_Save[1][1][1][15]=84;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][1][15]=72;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][1][15]=97;
    p->pSavetimeData->u1CBTVref_Save[1][1][1]=31;
    p->pSavetimeData->u1CBTClkDelay_Save[1][1][1]=0;
    p->pSavetimeData->u1CBTCmdDelay_Save[1][1][1]=1;
    p->pSavetimeData->u1CBTCsDelay_Save[1][1][1]=2;
    p->pSavetimeData->u1RxDatlat_Save[1][1][1]=14;
    p->pSavetimeData->u1TxWindowPerbitVref_Save[1][1][1]=14;
    p->pSavetimeData->u1RxWinPerbitVref_Save[1][1]=4;
    p->pSavetimeData->u1Gating2T_Save[2][0][1][0]=3;
    p->pSavetimeData->u1Gating05T_Save[2][0][1][0]=0;
    p->pSavetimeData->u1Gatingfine_tune_Save[2][0][1][0]=24;
    p->pSavetimeData->u1Gatingucpass_count_Save[2][0][1][0]=47;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[2][0][1][0]=27;
    p->pSavetimeData->u1TxCenter_min_Save[2][0][1][0]=86;
    p->pSavetimeData->u1TxCenter_max_Save[2][0][1][0]=88;
    p->pSavetimeData->u1Gating2T_Save[2][0][1][1]=3;
    p->pSavetimeData->u1Gating05T_Save[2][0][1][1]=0;
    p->pSavetimeData->u1Gatingfine_tune_Save[2][0][1][1]=24;
    p->pSavetimeData->u1Gatingucpass_count_Save[2][0][1][1]=48;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[2][0][1][1]=24;
    p->pSavetimeData->u1TxCenter_min_Save[2][0][1][1]=83;
    p->pSavetimeData->u1TxCenter_max_Save[2][0][1][1]=86;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][1][0]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][1][0]=28;
    p->pSavetimeData->u1Txwin_center_Save[2][0][1][0]=88;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][1][0]=76;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][1][0]=101;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[2][0][1][0]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][1][1]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][1][1]=28;
    p->pSavetimeData->u1Txwin_center_Save[2][0][1][1]=88;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][1][1]=75;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][1][1]=101;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[2][0][1][1]=76;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][1][2]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][1][2]=27;
    p->pSavetimeData->u1Txwin_center_Save[2][0][1][2]=87;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][1][2]=74;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][1][2]=101;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[2][0][1][2]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][1][3]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][1][3]=28;
    p->pSavetimeData->u1Txwin_center_Save[2][0][1][3]=86;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][1][3]=74;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][1][3]=99;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[2][0][1][3]=128;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][1][4]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][1][4]=30;
    p->pSavetimeData->u1Txwin_center_Save[2][0][1][4]=86;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][1][4]=74;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][1][4]=98;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[2][0][1][4]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][1][5]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][1][5]=30;
    p->pSavetimeData->u1Txwin_center_Save[2][0][1][5]=88;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][1][5]=75;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][1][5]=101;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[2][0][1][5]=4;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][1][6]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][1][6]=27;
    p->pSavetimeData->u1Txwin_center_Save[2][0][1][6]=87;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][1][6]=74;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][1][6]=101;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][1][7]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][1][7]=29;
    p->pSavetimeData->u1Txwin_center_Save[2][0][1][7]=88;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][1][7]=75;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][1][7]=102;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][1][8]=-2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][1][8]=27;
    p->pSavetimeData->u1Txwin_center_Save[2][0][1][8]=85;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][1][8]=73;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][1][8]=97;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][1][9]=-2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][1][9]=28;
    p->pSavetimeData->u1Txwin_center_Save[2][0][1][9]=83;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][1][9]=71;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][1][9]=96;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][1][10]=-2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][1][10]=28;
    p->pSavetimeData->u1Txwin_center_Save[2][0][1][10]=84;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][1][10]=71;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][1][10]=97;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][1][11]=-2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][1][11]=28;
    p->pSavetimeData->u1Txwin_center_Save[2][0][1][11]=83;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][1][11]=70;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][1][11]=97;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][1][12]=-2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][1][12]=29;
    p->pSavetimeData->u1Txwin_center_Save[2][0][1][12]=86;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][1][12]=73;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][1][12]=99;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][1][13]=-2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][1][13]=30;
    p->pSavetimeData->u1Txwin_center_Save[2][0][1][13]=86;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][1][13]=73;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][1][13]=99;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][1][14]=-2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][1][14]=28;
    p->pSavetimeData->u1Txwin_center_Save[2][0][1][14]=86;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][1][14]=73;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][1][14]=99;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][1][15]=-2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][1][15]=28;
    p->pSavetimeData->u1Txwin_center_Save[2][0][1][15]=85;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][1][15]=72;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][1][15]=98;
    p->pSavetimeData->u1CBTVref_Save[2][0][1]=31;
    p->pSavetimeData->u1CBTClkDelay_Save[2][0][1]=0;
    p->pSavetimeData->u1CBTCmdDelay_Save[2][0][1]=1;
    p->pSavetimeData->u1CBTCsDelay_Save[2][0][1]=2;
    p->pSavetimeData->u1RxDatlat_Save[2][0][1]=14;
    p->pSavetimeData->u1TxWindowPerbitVref_Save[2][0][1]=18;
    p->pSavetimeData->u1Gating2T_Save[2][1][1][0]=3;
    p->pSavetimeData->u1Gating05T_Save[2][1][1][0]=0;
    p->pSavetimeData->u1Gatingfine_tune_Save[2][1][1][0]=28;
    p->pSavetimeData->u1Gatingucpass_count_Save[2][1][1][0]=48;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[2][1][1][0]=26;
    p->pSavetimeData->u1TxCenter_min_Save[2][1][1][0]=86;
    p->pSavetimeData->u1TxCenter_max_Save[2][1][1][0]=88;
    p->pSavetimeData->u1Gating2T_Save[2][1][1][1]=3;
    p->pSavetimeData->u1Gating05T_Save[2][1][1][1]=0;
    p->pSavetimeData->u1Gatingfine_tune_Save[2][1][1][1]=28;
    p->pSavetimeData->u1Gatingucpass_count_Save[2][1][1][1]=48;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[2][1][1][1]=25;
    p->pSavetimeData->u1TxCenter_min_Save[2][1][1][1]=84;
    p->pSavetimeData->u1TxCenter_max_Save[2][1][1][1]=86;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][1][0]=-5;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][1][0]=29;
    p->pSavetimeData->u1Txwin_center_Save[2][1][1][0]=86;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][1][0]=74;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][1][0]=99;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[2][1][1][0]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][1][1]=-5;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][1][1]=29;
    p->pSavetimeData->u1Txwin_center_Save[2][1][1][1]=86;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][1][1]=74;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][1][1]=99;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[2][1][1][1]=76;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][1][2]=-5;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][1][2]=30;
    p->pSavetimeData->u1Txwin_center_Save[2][1][1][2]=86;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][1][2]=73;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][1][2]=100;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[2][1][1][2]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][1][3]=-5;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][1][3]=29;
    p->pSavetimeData->u1Txwin_center_Save[2][1][1][3]=86;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][1][3]=73;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][1][3]=99;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[2][1][1][3]=128;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][1][4]=-5;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][1][4]=31;
    p->pSavetimeData->u1Txwin_center_Save[2][1][1][4]=88;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][1][4]=76;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][1][4]=101;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[2][1][1][4]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][1][5]=-5;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][1][5]=30;
    p->pSavetimeData->u1Txwin_center_Save[2][1][1][5]=86;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][1][5]=73;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][1][5]=99;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[2][1][1][5]=4;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][1][6]=-5;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][1][6]=27;
    p->pSavetimeData->u1Txwin_center_Save[2][1][1][6]=87;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][1][6]=74;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][1][6]=100;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][1][7]=-5;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][1][7]=30;
    p->pSavetimeData->u1Txwin_center_Save[2][1][1][7]=87;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][1][7]=74;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][1][7]=100;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][1][8]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][1][8]=29;
    p->pSavetimeData->u1Txwin_center_Save[2][1][1][8]=85;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][1][8]=73;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][1][8]=97;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][1][9]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][1][9]=29;
    p->pSavetimeData->u1Txwin_center_Save[2][1][1][9]=86;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][1][9]=73;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][1][9]=99;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][1][10]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][1][10]=28;
    p->pSavetimeData->u1Txwin_center_Save[2][1][1][10]=85;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][1][10]=71;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][1][10]=99;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][1][11]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][1][11]=29;
    p->pSavetimeData->u1Txwin_center_Save[2][1][1][11]=86;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][1][11]=73;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][1][11]=100;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][1][12]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][1][12]=30;
    p->pSavetimeData->u1Txwin_center_Save[2][1][1][12]=84;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][1][12]=71;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][1][12]=97;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][1][13]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][1][13]=30;
    p->pSavetimeData->u1Txwin_center_Save[2][1][1][13]=85;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][1][13]=73;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][1][13]=98;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][1][14]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][1][14]=29;
    p->pSavetimeData->u1Txwin_center_Save[2][1][1][14]=85;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][1][14]=71;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][1][14]=99;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][1][15]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][1][15]=30;
    p->pSavetimeData->u1Txwin_center_Save[2][1][1][15]=85;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][1][15]=72;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][1][15]=98;
    p->pSavetimeData->u1CBTVref_Save[2][1][1]=29;
    p->pSavetimeData->u1CBTClkDelay_Save[2][1][1]=0;
    p->pSavetimeData->u1CBTCmdDelay_Save[2][1][1]=1;
    p->pSavetimeData->u1CBTCsDelay_Save[2][1][1]=2;
    p->pSavetimeData->u1RxDatlat_Save[2][1][1]=14;
    p->pSavetimeData->u1TxWindowPerbitVref_Save[2][1][1]=18;
    p->pSavetimeData->u1RxWinPerbitVref_Save[2][1]=4;
    p->pSavetimeData->u1Gating2T_Save[3][0][1][0]=3;
    p->pSavetimeData->u1Gating05T_Save[3][0][1][0]=1;
    p->pSavetimeData->u1Gatingfine_tune_Save[3][0][1][0]=4;
    p->pSavetimeData->u1Gatingucpass_count_Save[3][0][1][0]=47;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[3][0][1][0]=24;
    p->pSavetimeData->u1TxCenter_min_Save[3][0][1][0]=83;
    p->pSavetimeData->u1TxCenter_max_Save[3][0][1][0]=84;
    p->pSavetimeData->u1Gating2T_Save[3][0][1][1]=3;
    p->pSavetimeData->u1Gating05T_Save[3][0][1][1]=1;
    p->pSavetimeData->u1Gatingfine_tune_Save[3][0][1][1]=4;
    p->pSavetimeData->u1Gatingucpass_count_Save[3][0][1][1]=46;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[3][0][1][1]=22;
    p->pSavetimeData->u1TxCenter_min_Save[3][0][1][1]=82;
    p->pSavetimeData->u1TxCenter_max_Save[3][0][1][1]=83;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][1][0]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][1][0]=30;
    p->pSavetimeData->u1Txwin_center_Save[3][0][1][0]=84;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][1][0]=71;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][1][0]=97;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[3][0][1][0]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][1][1]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][1][1]=30;
    p->pSavetimeData->u1Txwin_center_Save[3][0][1][1]=83;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][1][1]=70;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][1][1]=97;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[3][0][1][1]=76;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][1][2]=-1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][1][2]=28;
    p->pSavetimeData->u1Txwin_center_Save[3][0][1][2]=83;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][1][2]=70;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][1][2]=97;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[3][0][1][2]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][1][3]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][1][3]=29;
    p->pSavetimeData->u1Txwin_center_Save[3][0][1][3]=83;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][1][3]=70;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][1][3]=96;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[3][0][1][3]=128;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][1][4]=-1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][1][4]=30;
    p->pSavetimeData->u1Txwin_center_Save[3][0][1][4]=83;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][1][4]=70;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][1][4]=96;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[3][0][1][4]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][1][5]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][1][5]=31;
    p->pSavetimeData->u1Txwin_center_Save[3][0][1][5]=84;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][1][5]=72;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][1][5]=97;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[3][0][1][5]=4;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][1][6]=-2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][1][6]=28;
    p->pSavetimeData->u1Txwin_center_Save[3][0][1][6]=84;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][1][6]=71;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][1][6]=97;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][1][7]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][1][7]=30;
    p->pSavetimeData->u1Txwin_center_Save[3][0][1][7]=84;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][1][7]=72;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][1][7]=97;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][1][8]=-4;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][1][8]=29;
    p->pSavetimeData->u1Txwin_center_Save[3][0][1][8]=82;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][1][8]=70;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][1][8]=95;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][1][9]=-4;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][1][9]=29;
    p->pSavetimeData->u1Txwin_center_Save[3][0][1][9]=82;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][1][9]=70;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][1][9]=95;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][1][10]=-2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][1][10]=29;
    p->pSavetimeData->u1Txwin_center_Save[3][0][1][10]=82;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][1][10]=69;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][1][10]=95;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][1][11]=-4;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][1][11]=30;
    p->pSavetimeData->u1Txwin_center_Save[3][0][1][11]=82;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][1][11]=69;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][1][11]=95;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][1][12]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][1][12]=31;
    p->pSavetimeData->u1Txwin_center_Save[3][0][1][12]=82;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][1][12]=70;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][1][12]=95;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][1][13]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][1][13]=32;
    p->pSavetimeData->u1Txwin_center_Save[3][0][1][13]=83;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][1][13]=71;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][1][13]=96;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][1][14]=-1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][1][14]=32;
    p->pSavetimeData->u1Txwin_center_Save[3][0][1][14]=82;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][1][14]=70;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][1][14]=95;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][1][15]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][1][15]=30;
    p->pSavetimeData->u1Txwin_center_Save[3][0][1][15]=83;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][1][15]=70;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][1][15]=96;
    p->pSavetimeData->u1CBTVref_Save[3][0][1]=29;
    p->pSavetimeData->u1CBTClkDelay_Save[3][0][1]=0;
    p->pSavetimeData->u1CBTCmdDelay_Save[3][0][1]=2;
    p->pSavetimeData->u1CBTCsDelay_Save[3][0][1]=2;
    p->pSavetimeData->u1RxDatlat_Save[3][0][1]=14;
    p->pSavetimeData->u1TxWindowPerbitVref_Save[3][0][1]=14;
    p->pSavetimeData->u1Gating2T_Save[3][1][1][0]=3;
    p->pSavetimeData->u1Gating05T_Save[3][1][1][0]=1;
    p->pSavetimeData->u1Gatingfine_tune_Save[3][1][1][0]=8;
    p->pSavetimeData->u1Gatingucpass_count_Save[3][1][1][0]=48;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[3][1][1][0]=24;
    p->pSavetimeData->u1TxCenter_min_Save[3][1][1][0]=83;
    p->pSavetimeData->u1TxCenter_max_Save[3][1][1][0]=85;
    p->pSavetimeData->u1Gating2T_Save[3][1][1][1]=3;
    p->pSavetimeData->u1Gating05T_Save[3][1][1][1]=1;
    p->pSavetimeData->u1Gatingfine_tune_Save[3][1][1][1]=8;
    p->pSavetimeData->u1Gatingucpass_count_Save[3][1][1][1]=47;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[3][1][1][1]=21;
    p->pSavetimeData->u1TxCenter_min_Save[3][1][1][1]=81;
    p->pSavetimeData->u1TxCenter_max_Save[3][1][1][1]=83;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][1][0]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][1][0]=30;
    p->pSavetimeData->u1Txwin_center_Save[3][1][1][0]=84;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][1][0]=71;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][1][0]=97;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[3][1][1][0]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][1][1]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][1][1]=28;
    p->pSavetimeData->u1Txwin_center_Save[3][1][1][1]=83;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][1][1]=70;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][1][1]=97;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[3][1][1][1]=76;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][1][2]=-2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][1][2]=28;
    p->pSavetimeData->u1Txwin_center_Save[3][1][1][2]=85;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][1][2]=72;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][1][2]=98;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[3][1][1][2]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][1][3]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][1][3]=28;
    p->pSavetimeData->u1Txwin_center_Save[3][1][1][3]=83;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][1][3]=70;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][1][3]=97;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[3][1][1][3]=128;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][1][4]=-2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][1][4]=30;
    p->pSavetimeData->u1Txwin_center_Save[3][1][1][4]=84;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][1][4]=71;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][1][4]=97;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[3][1][1][4]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][1][5]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][1][5]=30;
    p->pSavetimeData->u1Txwin_center_Save[3][1][1][5]=85;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][1][5]=72;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][1][5]=98;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[3][1][1][5]=4;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][1][6]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][1][6]=28;
    p->pSavetimeData->u1Txwin_center_Save[3][1][1][6]=85;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][1][6]=72;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][1][6]=98;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][1][7]=-3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][1][7]=30;
    p->pSavetimeData->u1Txwin_center_Save[3][1][1][7]=84;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][1][7]=71;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][1][7]=98;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][1][8]=-1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][1][8]=31;
    p->pSavetimeData->u1Txwin_center_Save[3][1][1][8]=82;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][1][8]=70;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][1][8]=95;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][1][9]=-1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][1][9]=31;
    p->pSavetimeData->u1Txwin_center_Save[3][1][1][9]=81;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][1][9]=69;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][1][9]=93;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][1][10]=-1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][1][10]=31;
    p->pSavetimeData->u1Txwin_center_Save[3][1][1][10]=82;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][1][10]=69;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][1][10]=95;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][1][11]=-1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][1][11]=32;
    p->pSavetimeData->u1Txwin_center_Save[3][1][1][11]=82;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][1][11]=69;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][1][11]=95;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][1][12]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][1][12]=33;
    p->pSavetimeData->u1Txwin_center_Save[3][1][1][12]=82;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][1][12]=70;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][1][12]=95;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][1][13]=-1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][1][13]=33;
    p->pSavetimeData->u1Txwin_center_Save[3][1][1][13]=82;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][1][13]=70;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][1][13]=95;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][1][14]=-1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][1][14]=32;
    p->pSavetimeData->u1Txwin_center_Save[3][1][1][14]=83;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][1][14]=71;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][1][14]=96;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][1][15]=-1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][1][15]=32;
    p->pSavetimeData->u1Txwin_center_Save[3][1][1][15]=82;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][1][15]=70;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][1][15]=94;
    p->pSavetimeData->u1CBTVref_Save[3][1][1]=29;
    p->pSavetimeData->u1CBTClkDelay_Save[3][1][1]=0;
    p->pSavetimeData->u1CBTCmdDelay_Save[3][1][1]=2;
    p->pSavetimeData->u1CBTCsDelay_Save[3][1][1]=2;
    p->pSavetimeData->u1RxDatlat_Save[3][1][1]=14;
    p->pSavetimeData->u1TxWindowPerbitVref_Save[3][1][1]=18;
    p->pSavetimeData->u1RxWinPerbitVref_Save[3][1]=4;
    p->pSavetimeData->u1Gating2T_Save[0][0][0][0]=3;
    p->pSavetimeData->u1Gating05T_Save[0][0][0][0]=3;
    p->pSavetimeData->u1Gatingfine_tune_Save[0][0][0][0]=24;
    p->pSavetimeData->u1Gatingucpass_count_Save[0][0][0][0]=35;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[0][0][0][0]=27;
    p->pSavetimeData->u1TxCenter_min_Save[0][0][0][0]=93;
    p->pSavetimeData->u1TxCenter_max_Save[0][0][0][0]=96;
    p->pSavetimeData->u1Gating2T_Save[0][0][0][1]=3;
    p->pSavetimeData->u1Gating05T_Save[0][0][0][1]=2;
    p->pSavetimeData->u1Gatingfine_tune_Save[0][0][0][1]=4;
    p->pSavetimeData->u1Gatingucpass_count_Save[0][0][0][1]=48;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[0][0][0][1]=27;
    p->pSavetimeData->u1TxCenter_min_Save[0][0][0][1]=95;
    p->pSavetimeData->u1TxCenter_max_Save[0][0][0][1]=97;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][0][0]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][0][0]=26;
    p->pSavetimeData->u1Txwin_center_Save[0][0][0][0]=95;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][0][0]=85;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][0][0]=106;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[0][0][0][0]=4;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][0][1]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][0][1]=26;
    p->pSavetimeData->u1Txwin_center_Save[0][0][0][1]=95;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][0][1]=82;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][0][1]=108;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[0][0][0][1]=4;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][0][2]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][0][2]=26;
    p->pSavetimeData->u1Txwin_center_Save[0][0][0][2]=94;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][0][2]=82;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][0][2]=107;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[0][0][0][2]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][0][3]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][0][3]=24;
    p->pSavetimeData->u1Txwin_center_Save[0][0][0][3]=93;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][0][3]=81;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][0][3]=105;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[0][0][0][3]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][0][4]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][0][4]=26;
    p->pSavetimeData->u1Txwin_center_Save[0][0][0][4]=93;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][0][4]=81;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][0][4]=105;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[0][0][0][4]=2;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][0][5]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][0][5]=26;
    p->pSavetimeData->u1Txwin_center_Save[0][0][0][5]=95;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][0][5]=83;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][0][5]=108;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[0][0][0][5]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][0][6]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][0][6]=25;
    p->pSavetimeData->u1Txwin_center_Save[0][0][0][6]=96;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][0][6]=83;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][0][6]=109;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][0][7]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][0][7]=24;
    p->pSavetimeData->u1Txwin_center_Save[0][0][0][7]=96;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][0][7]=85;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][0][7]=108;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][0][8]=-1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][0][8]=27;
    p->pSavetimeData->u1Txwin_center_Save[0][0][0][8]=97;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][0][8]=85;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][0][8]=109;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][0][9]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][0][9]=25;
    p->pSavetimeData->u1Txwin_center_Save[0][0][0][9]=95;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][0][9]=83;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][0][9]=107;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][0][10]=-1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][0][10]=26;
    p->pSavetimeData->u1Txwin_center_Save[0][0][0][10]=96;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][0][10]=84;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][0][10]=109;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][0][11]=2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][0][11]=27;
    p->pSavetimeData->u1Txwin_center_Save[0][0][0][11]=96;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][0][11]=83;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][0][11]=109;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][0][12]=-1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][0][12]=26;
    p->pSavetimeData->u1Txwin_center_Save[0][0][0][12]=96;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][0][12]=84;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][0][12]=108;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][0][13]=2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][0][13]=27;
    p->pSavetimeData->u1Txwin_center_Save[0][0][0][13]=96;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][0][13]=85;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][0][13]=108;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][0][14]=-1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][0][14]=27;
    p->pSavetimeData->u1Txwin_center_Save[0][0][0][14]=96;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][0][14]=83;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][0][14]=109;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][0][15]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][0][15]=23;
    p->pSavetimeData->u1Txwin_center_Save[0][0][0][15]=96;
    p->pSavetimeData->u1Txfirst_pass_Save[0][0][0][15]=84;
    p->pSavetimeData->u1Txlast_pass_Save[0][0][0][15]=109;
    p->pSavetimeData->u1CBTVref_Save[0][0][0]=31;
    p->pSavetimeData->u1CBTClkDelay_Save[0][0][0]=0;
    p->pSavetimeData->u1CBTCmdDelay_Save[0][0][0]=1;
    p->pSavetimeData->u1CBTCsDelay_Save[0][0][0]=2;
    p->pSavetimeData->u1RxDatlat_Save[0][0][0]=16;
    p->pSavetimeData->u1TxWindowPerbitVref_Save[0][0][0]=14;
    p->pSavetimeData->u1Gating2T_Save[0][1][0][0]=3;
    p->pSavetimeData->u1Gating05T_Save[0][1][0][0]=3;
    p->pSavetimeData->u1Gatingfine_tune_Save[0][1][0][0]=28;
    p->pSavetimeData->u1Gatingucpass_count_Save[0][1][0][0]=35;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[0][1][0][0]=24;
    p->pSavetimeData->u1TxCenter_min_Save[0][1][0][0]=89;
    p->pSavetimeData->u1TxCenter_max_Save[0][1][0][0]=92;
    p->pSavetimeData->u1Gating2T_Save[0][1][0][1]=3;
    p->pSavetimeData->u1Gating05T_Save[0][1][0][1]=2;
    p->pSavetimeData->u1Gatingfine_tune_Save[0][1][0][1]=12;
    p->pSavetimeData->u1Gatingucpass_count_Save[0][1][0][1]=47;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[0][1][0][1]=25;
    p->pSavetimeData->u1TxCenter_min_Save[0][1][0][1]=93;
    p->pSavetimeData->u1TxCenter_max_Save[0][1][0][1]=95;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][0][0]=2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][0][0]=29;
    p->pSavetimeData->u1Txwin_center_Save[0][1][0][0]=91;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][0][0]=80;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][0][0]=103;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[0][1][0][0]=3;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][0][1]=2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][0][1]=30;
    p->pSavetimeData->u1Txwin_center_Save[0][1][0][1]=89;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][0][1]=78;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][0][1]=101;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[0][1][0][1]=2;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][0][2]=2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][0][2]=27;
    p->pSavetimeData->u1Txwin_center_Save[0][1][0][2]=91;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][0][2]=80;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][0][2]=103;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[0][1][0][2]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][0][3]=2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][0][3]=29;
    p->pSavetimeData->u1Txwin_center_Save[0][1][0][3]=91;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][0][3]=80;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][0][3]=103;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[0][1][0][3]=2;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][0][4]=2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][0][4]=29;
    p->pSavetimeData->u1Txwin_center_Save[0][1][0][4]=92;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][0][4]=81;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][0][4]=104;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[0][1][0][4]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][0][5]=2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][0][5]=30;
    p->pSavetimeData->u1Txwin_center_Save[0][1][0][5]=91;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][0][5]=80;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][0][5]=103;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[0][1][0][5]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][0][6]=2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][0][6]=27;
    p->pSavetimeData->u1Txwin_center_Save[0][1][0][6]=91;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][0][6]=79;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][0][6]=103;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][0][7]=2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][0][7]=29;
    p->pSavetimeData->u1Txwin_center_Save[0][1][0][7]=91;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][0][7]=79;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][0][7]=103;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][0][8]=-1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][0][8]=27;
    p->pSavetimeData->u1Txwin_center_Save[0][1][0][8]=95;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][0][8]=82;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][0][8]=108;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][0][9]=-1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][0][9]=26;
    p->pSavetimeData->u1Txwin_center_Save[0][1][0][9]=94;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][0][9]=81;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][0][9]=108;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][0][10]=-1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][0][10]=27;
    p->pSavetimeData->u1Txwin_center_Save[0][1][0][10]=94;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][0][10]=81;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][0][10]=108;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][0][11]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][0][11]=27;
    p->pSavetimeData->u1Txwin_center_Save[0][1][0][11]=94;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][0][11]=81;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][0][11]=108;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][0][12]=-1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][0][12]=27;
    p->pSavetimeData->u1Txwin_center_Save[0][1][0][12]=93;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][0][12]=81;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][0][12]=106;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][0][13]=-1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][0][13]=27;
    p->pSavetimeData->u1Txwin_center_Save[0][1][0][13]=95;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][0][13]=82;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][0][13]=108;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][0][14]=-1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][0][14]=27;
    p->pSavetimeData->u1Txwin_center_Save[0][1][0][14]=95;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][0][14]=82;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][0][14]=109;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][0][15]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][0][15]=25;
    p->pSavetimeData->u1Txwin_center_Save[0][1][0][15]=95;
    p->pSavetimeData->u1Txfirst_pass_Save[0][1][0][15]=82;
    p->pSavetimeData->u1Txlast_pass_Save[0][1][0][15]=108;
    p->pSavetimeData->u1CBTVref_Save[0][1][0]=31;
    p->pSavetimeData->u1CBTClkDelay_Save[0][1][0]=0;
    p->pSavetimeData->u1CBTCmdDelay_Save[0][1][0]=1;
    p->pSavetimeData->u1CBTCsDelay_Save[0][1][0]=2;
    p->pSavetimeData->u1RxDatlat_Save[0][1][0]=16;
    p->pSavetimeData->u1TxWindowPerbitVref_Save[0][1][0]=16;
    p->pSavetimeData->u1RxWinPerbitVref_Save[0][0]=4;
    p->pSavetimeData->u1Gating2T_Save[1][0][0][0]=3;
    p->pSavetimeData->u1Gating05T_Save[1][0][0][0]=3;
    p->pSavetimeData->u1Gatingfine_tune_Save[1][0][0][0]=24;
    p->pSavetimeData->u1Gatingucpass_count_Save[1][0][0][0]=36;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[1][0][0][0]=24;
    p->pSavetimeData->u1TxCenter_min_Save[1][0][0][0]=89;
    p->pSavetimeData->u1TxCenter_max_Save[1][0][0][0]=92;
    p->pSavetimeData->u1Gating2T_Save[1][0][0][1]=3;
    p->pSavetimeData->u1Gating05T_Save[1][0][0][1]=2;
    p->pSavetimeData->u1Gatingfine_tune_Save[1][0][0][1]=12;
    p->pSavetimeData->u1Gatingucpass_count_Save[1][0][0][1]=47;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[1][0][0][1]=24;
    p->pSavetimeData->u1TxCenter_min_Save[1][0][0][1]=91;
    p->pSavetimeData->u1TxCenter_max_Save[1][0][0][1]=94;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][0][0]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][0][0]=25;
    p->pSavetimeData->u1Txwin_center_Save[1][0][0][0]=92;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][0][0]=81;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][0][0]=104;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[1][0][0][0]=4;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][0][1]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][0][1]=25;
    p->pSavetimeData->u1Txwin_center_Save[1][0][0][1]=91;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][0][1]=79;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][0][1]=103;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[1][0][0][1]=3;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][0][2]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][0][2]=25;
    p->pSavetimeData->u1Txwin_center_Save[1][0][0][2]=91;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][0][2]=80;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][0][2]=103;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[1][0][0][2]=2;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][0][3]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][0][3]=23;
    p->pSavetimeData->u1Txwin_center_Save[1][0][0][3]=89;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][0][3]=77;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][0][3]=102;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[1][0][0][3]=3;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][0][4]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][0][4]=25;
    p->pSavetimeData->u1Txwin_center_Save[1][0][0][4]=92;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][0][4]=80;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][0][4]=104;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[1][0][0][4]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][0][5]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][0][5]=25;
    p->pSavetimeData->u1Txwin_center_Save[1][0][0][5]=92;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][0][5]=80;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][0][5]=105;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[1][0][0][5]=3;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][0][6]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][0][6]=25;
    p->pSavetimeData->u1Txwin_center_Save[1][0][0][6]=91;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][0][6]=80;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][0][6]=103;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][0][7]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][0][7]=25;
    p->pSavetimeData->u1Txwin_center_Save[1][0][0][7]=92;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][0][7]=79;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][0][7]=106;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][0][8]=2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][0][8]=28;
    p->pSavetimeData->u1Txwin_center_Save[1][0][0][8]=93;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][0][8]=81;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][0][8]=105;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][0][9]=2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][0][9]=25;
    p->pSavetimeData->u1Txwin_center_Save[1][0][0][9]=91;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][0][9]=79;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][0][9]=104;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][0][10]=2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][0][10]=26;
    p->pSavetimeData->u1Txwin_center_Save[1][0][0][10]=92;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][0][10]=80;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][0][10]=105;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][0][11]=2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][0][11]=25;
    p->pSavetimeData->u1Txwin_center_Save[1][0][0][11]=92;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][0][11]=79;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][0][11]=105;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][0][12]=2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][0][12]=28;
    p->pSavetimeData->u1Txwin_center_Save[1][0][0][12]=94;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][0][12]=82;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][0][12]=106;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][0][13]=4;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][0][13]=27;
    p->pSavetimeData->u1Txwin_center_Save[1][0][0][13]=93;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][0][13]=81;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][0][13]=106;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][0][14]=2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][0][14]=28;
    p->pSavetimeData->u1Txwin_center_Save[1][0][0][14]=94;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][0][14]=81;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][0][14]=108;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][0][15]=2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][0][15]=26;
    p->pSavetimeData->u1Txwin_center_Save[1][0][0][15]=93;
    p->pSavetimeData->u1Txfirst_pass_Save[1][0][0][15]=79;
    p->pSavetimeData->u1Txlast_pass_Save[1][0][0][15]=107;
    p->pSavetimeData->u1CBTVref_Save[1][0][0]=31;
    p->pSavetimeData->u1CBTClkDelay_Save[1][0][0]=0;
    p->pSavetimeData->u1CBTCmdDelay_Save[1][0][0]=0;
    p->pSavetimeData->u1CBTCsDelay_Save[1][0][0]=1;
    p->pSavetimeData->u1RxDatlat_Save[1][0][0]=16;
    p->pSavetimeData->u1TxWindowPerbitVref_Save[1][0][0]=16;
    p->pSavetimeData->u1Gating2T_Save[1][1][0][0]=3;
    p->pSavetimeData->u1Gating05T_Save[1][1][0][0]=4;
    p->pSavetimeData->u1Gatingfine_tune_Save[1][1][0][0]=0;
    p->pSavetimeData->u1Gatingucpass_count_Save[1][1][0][0]=36;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[1][1][0][0]=23;
    p->pSavetimeData->u1TxCenter_min_Save[1][1][0][0]=89;
    p->pSavetimeData->u1TxCenter_max_Save[1][1][0][0]=90;
    p->pSavetimeData->u1Gating2T_Save[1][1][0][1]=3;
    p->pSavetimeData->u1Gating05T_Save[1][1][0][1]=2;
    p->pSavetimeData->u1Gatingfine_tune_Save[1][1][0][1]=16;
    p->pSavetimeData->u1Gatingucpass_count_Save[1][1][0][1]=48;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[1][1][0][1]=25;
    p->pSavetimeData->u1TxCenter_min_Save[1][1][0][1]=92;
    p->pSavetimeData->u1TxCenter_max_Save[1][1][0][1]=94;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][0][0]=-2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][0][0]=26;
    p->pSavetimeData->u1Txwin_center_Save[1][1][0][0]=89;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][0][0]=77;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][0][0]=102;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[1][1][0][0]=3;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][0][1]=-2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][0][1]=27;
    p->pSavetimeData->u1Txwin_center_Save[1][1][0][1]=89;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][0][1]=76;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][0][1]=103;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[1][1][0][1]=2;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][0][2]=-2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][0][2]=27;
    p->pSavetimeData->u1Txwin_center_Save[1][1][0][2]=89;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][0][2]=77;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][0][2]=102;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[1][1][0][2]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][0][3]=-2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][0][3]=26;
    p->pSavetimeData->u1Txwin_center_Save[1][1][0][3]=90;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][0][3]=77;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][0][3]=103;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[1][1][0][3]=2;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][0][4]=-2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][0][4]=28;
    p->pSavetimeData->u1Txwin_center_Save[1][1][0][4]=89;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][0][4]=76;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][0][4]=102;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[1][1][0][4]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][0][5]=-2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][0][5]=28;
    p->pSavetimeData->u1Txwin_center_Save[1][1][0][5]=90;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][0][5]=77;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][0][5]=103;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[1][1][0][5]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][0][6]=-2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][0][6]=27;
    p->pSavetimeData->u1Txwin_center_Save[1][1][0][6]=89;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][0][6]=76;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][0][6]=102;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][0][7]=-2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][0][7]=29;
    p->pSavetimeData->u1Txwin_center_Save[1][1][0][7]=90;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][0][7]=77;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][0][7]=103;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][0][8]=-2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][0][8]=24;
    p->pSavetimeData->u1Txwin_center_Save[1][1][0][8]=93;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][0][8]=81;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][0][8]=105;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][0][9]=-2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][0][9]=26;
    p->pSavetimeData->u1Txwin_center_Save[1][1][0][9]=94;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][0][9]=81;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][0][9]=107;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][0][10]=-2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][0][10]=26;
    p->pSavetimeData->u1Txwin_center_Save[1][1][0][10]=93;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][0][10]=80;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][0][10]=106;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][0][11]=-2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][0][11]=27;
    p->pSavetimeData->u1Txwin_center_Save[1][1][0][11]=93;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][0][11]=81;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][0][11]=106;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][0][12]=-1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][0][12]=27;
    p->pSavetimeData->u1Txwin_center_Save[1][1][0][12]=92;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][0][12]=80;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][0][12]=104;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][0][13]=-2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][0][13]=27;
    p->pSavetimeData->u1Txwin_center_Save[1][1][0][13]=93;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][0][13]=82;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][0][13]=105;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][0][14]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][0][14]=25;
    p->pSavetimeData->u1Txwin_center_Save[1][1][0][14]=94;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][0][14]=81;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][0][14]=107;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][0][15]=-2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][0][15]=27;
    p->pSavetimeData->u1Txwin_center_Save[1][1][0][15]=93;
    p->pSavetimeData->u1Txfirst_pass_Save[1][1][0][15]=81;
    p->pSavetimeData->u1Txlast_pass_Save[1][1][0][15]=106;
    p->pSavetimeData->u1CBTVref_Save[1][1][0]=31;
    p->pSavetimeData->u1CBTClkDelay_Save[1][1][0]=0;
    p->pSavetimeData->u1CBTCmdDelay_Save[1][1][0]=0;
    p->pSavetimeData->u1CBTCsDelay_Save[1][1][0]=2;
    p->pSavetimeData->u1RxDatlat_Save[1][1][0]=16;
    p->pSavetimeData->u1TxWindowPerbitVref_Save[1][1][0]=14;
    p->pSavetimeData->u1RxWinPerbitVref_Save[1][0]=0;
    p->pSavetimeData->u1Gating2T_Save[2][0][0][0]=3;
    p->pSavetimeData->u1Gating05T_Save[2][0][0][0]=2;
    p->pSavetimeData->u1Gatingfine_tune_Save[2][0][0][0]=4;
    p->pSavetimeData->u1Gatingucpass_count_Save[2][0][0][0]=48;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[2][0][0][0]=28;
    p->pSavetimeData->u1TxCenter_min_Save[2][0][0][0]=95;
    p->pSavetimeData->u1TxCenter_max_Save[2][0][0][0]=99;
    p->pSavetimeData->u1Gating2T_Save[2][0][0][1]=3;
    p->pSavetimeData->u1Gating05T_Save[2][0][0][1]=2;
    p->pSavetimeData->u1Gatingfine_tune_Save[2][0][0][1]=8;
    p->pSavetimeData->u1Gatingucpass_count_Save[2][0][0][1]=47;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[2][0][0][1]=24;
    p->pSavetimeData->u1TxCenter_min_Save[2][0][0][1]=92;
    p->pSavetimeData->u1TxCenter_max_Save[2][0][0][1]=95;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][0][0]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][0][0]=26;
    p->pSavetimeData->u1Txwin_center_Save[2][0][0][0]=98;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][0][0]=86;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][0][0]=111;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[2][0][0][0]=3;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][0][1]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][0][1]=26;
    p->pSavetimeData->u1Txwin_center_Save[2][0][0][1]=98;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][0][1]=85;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][0][1]=111;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[2][0][0][1]=2;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][0][2]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][0][2]=24;
    p->pSavetimeData->u1Txwin_center_Save[2][0][0][2]=96;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][0][2]=83;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][0][2]=110;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[2][0][0][2]=2;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][0][3]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][0][3]=26;
    p->pSavetimeData->u1Txwin_center_Save[2][0][0][3]=96;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][0][3]=83;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][0][3]=109;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[2][0][0][3]=2;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][0][4]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][0][4]=27;
    p->pSavetimeData->u1Txwin_center_Save[2][0][0][4]=95;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][0][4]=83;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][0][4]=107;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[2][0][0][4]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][0][5]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][0][5]=28;
    p->pSavetimeData->u1Txwin_center_Save[2][0][0][5]=98;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][0][5]=86;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][0][5]=110;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[2][0][0][5]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][0][6]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][0][6]=23;
    p->pSavetimeData->u1Txwin_center_Save[2][0][0][6]=96;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][0][6]=82;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][0][6]=111;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][0][7]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][0][7]=26;
    p->pSavetimeData->u1Txwin_center_Save[2][0][0][7]=99;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][0][7]=87;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][0][7]=111;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][0][8]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][0][8]=26;
    p->pSavetimeData->u1Txwin_center_Save[2][0][0][8]=94;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][0][8]=82;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][0][8]=106;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][0][9]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][0][9]=26;
    p->pSavetimeData->u1Txwin_center_Save[2][0][0][9]=93;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][0][9]=80;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][0][9]=106;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][0][10]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][0][10]=26;
    p->pSavetimeData->u1Txwin_center_Save[2][0][0][10]=92;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][0][10]=80;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][0][10]=105;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][0][11]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][0][11]=26;
    p->pSavetimeData->u1Txwin_center_Save[2][0][0][11]=93;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][0][11]=79;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][0][11]=107;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][0][12]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][0][12]=26;
    p->pSavetimeData->u1Txwin_center_Save[2][0][0][12]=94;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][0][12]=82;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][0][12]=107;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][0][13]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][0][13]=28;
    p->pSavetimeData->u1Txwin_center_Save[2][0][0][13]=95;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][0][13]=82;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][0][13]=108;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][0][14]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][0][14]=25;
    p->pSavetimeData->u1Txwin_center_Save[2][0][0][14]=93;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][0][14]=80;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][0][14]=107;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][0][0][15]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][0][0][15]=26;
    p->pSavetimeData->u1Txwin_center_Save[2][0][0][15]=94;
    p->pSavetimeData->u1Txfirst_pass_Save[2][0][0][15]=80;
    p->pSavetimeData->u1Txlast_pass_Save[2][0][0][15]=108;
    p->pSavetimeData->u1CBTVref_Save[2][0][0]=31;
    p->pSavetimeData->u1CBTClkDelay_Save[2][0][0]=0;
    p->pSavetimeData->u1CBTCmdDelay_Save[2][0][0]=1;
    p->pSavetimeData->u1CBTCsDelay_Save[2][0][0]=2;
    p->pSavetimeData->u1RxDatlat_Save[2][0][0]=16;
    p->pSavetimeData->u1TxWindowPerbitVref_Save[2][0][0]=14;
    p->pSavetimeData->u1Gating2T_Save[2][1][0][0]=3;
    p->pSavetimeData->u1Gating05T_Save[2][1][0][0]=2;
    p->pSavetimeData->u1Gatingfine_tune_Save[2][1][0][0]=12;
    p->pSavetimeData->u1Gatingucpass_count_Save[2][1][0][0]=47;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[2][1][0][0]=27;
    p->pSavetimeData->u1TxCenter_min_Save[2][1][0][0]=95;
    p->pSavetimeData->u1TxCenter_max_Save[2][1][0][0]=100;
    p->pSavetimeData->u1Gating2T_Save[2][1][0][1]=3;
    p->pSavetimeData->u1Gating05T_Save[2][1][0][1]=2;
    p->pSavetimeData->u1Gatingfine_tune_Save[2][1][0][1]=12;
    p->pSavetimeData->u1Gatingucpass_count_Save[2][1][0][1]=47;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[2][1][0][1]=26;
    p->pSavetimeData->u1TxCenter_min_Save[2][1][0][1]=94;
    p->pSavetimeData->u1TxCenter_max_Save[2][1][0][1]=95;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][0][0]=-1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][0][0]=27;
    p->pSavetimeData->u1Txwin_center_Save[2][1][0][0]=96;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][0][0]=84;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][0][0]=109;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[2][1][0][0]=3;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][0][1]=-1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][0][1]=27;
    p->pSavetimeData->u1Txwin_center_Save[2][1][0][1]=95;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][0][1]=83;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][0][1]=108;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[2][1][0][1]=4;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][0][2]=-1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][0][2]=27;
    p->pSavetimeData->u1Txwin_center_Save[2][1][0][2]=97;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][0][2]=84;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][0][2]=111;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[2][1][0][2]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][0][3]=-1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][0][3]=26;
    p->pSavetimeData->u1Txwin_center_Save[2][1][0][3]=95;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][0][3]=82;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][0][3]=109;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[2][1][0][3]=2;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][0][4]=-1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][0][4]=29;
    p->pSavetimeData->u1Txwin_center_Save[2][1][0][4]=100;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][0][4]=88;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][0][4]=112;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[2][1][0][4]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][0][5]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][0][5]=29;
    p->pSavetimeData->u1Txwin_center_Save[2][1][0][5]=95;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][0][5]=82;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][0][5]=108;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[2][1][0][5]=2;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][0][6]=-1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][0][6]=24;
    p->pSavetimeData->u1Txwin_center_Save[2][1][0][6]=98;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][0][6]=85;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][0][6]=112;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][0][7]=-1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][0][7]=27;
    p->pSavetimeData->u1Txwin_center_Save[2][1][0][7]=96;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][0][7]=83;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][0][7]=109;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][0][8]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][0][8]=27;
    p->pSavetimeData->u1Txwin_center_Save[2][1][0][8]=95;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][0][8]=84;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][0][8]=107;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][0][9]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][0][9]=27;
    p->pSavetimeData->u1Txwin_center_Save[2][1][0][9]=95;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][0][9]=81;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][0][9]=109;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][0][10]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][0][10]=26;
    p->pSavetimeData->u1Txwin_center_Save[2][1][0][10]=95;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][0][10]=81;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][0][10]=109;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][0][11]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][0][11]=27;
    p->pSavetimeData->u1Txwin_center_Save[2][1][0][11]=95;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][0][11]=81;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][0][11]=109;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][0][12]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][0][12]=27;
    p->pSavetimeData->u1Txwin_center_Save[2][1][0][12]=95;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][0][12]=82;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][0][12]=108;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][0][13]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][0][13]=29;
    p->pSavetimeData->u1Txwin_center_Save[2][1][0][13]=94;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][0][13]=82;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][0][13]=107;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][0][14]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][0][14]=26;
    p->pSavetimeData->u1Txwin_center_Save[2][1][0][14]=95;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][0][14]=82;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][0][14]=109;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[2][1][0][15]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[2][1][0][15]=28;
    p->pSavetimeData->u1Txwin_center_Save[2][1][0][15]=95;
    p->pSavetimeData->u1Txfirst_pass_Save[2][1][0][15]=81;
    p->pSavetimeData->u1Txlast_pass_Save[2][1][0][15]=109;
    p->pSavetimeData->u1CBTVref_Save[2][1][0]=29;
    p->pSavetimeData->u1CBTClkDelay_Save[2][1][0]=0;
    p->pSavetimeData->u1CBTCmdDelay_Save[2][1][0]=1;
    p->pSavetimeData->u1CBTCsDelay_Save[2][1][0]=2;
    p->pSavetimeData->u1RxDatlat_Save[2][1][0]=16;
    p->pSavetimeData->u1TxWindowPerbitVref_Save[2][1][0]=16;
    p->pSavetimeData->u1RxWinPerbitVref_Save[2][0]=4;
    p->pSavetimeData->u1Gating2T_Save[3][0][0][0]=3;
    p->pSavetimeData->u1Gating05T_Save[3][0][0][0]=2;
    p->pSavetimeData->u1Gatingfine_tune_Save[3][0][0][0]=20;
    p->pSavetimeData->u1Gatingucpass_count_Save[3][0][0][0]=47;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[3][0][0][0]=23;
    p->pSavetimeData->u1TxCenter_min_Save[3][0][0][0]=91;
    p->pSavetimeData->u1TxCenter_max_Save[3][0][0][0]=93;
    p->pSavetimeData->u1Gating2T_Save[3][0][0][1]=3;
    p->pSavetimeData->u1Gating05T_Save[3][0][0][1]=2;
    p->pSavetimeData->u1Gatingfine_tune_Save[3][0][0][1]=16;
    p->pSavetimeData->u1Gatingucpass_count_Save[3][0][0][1]=48;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[3][0][0][1]=22;
    p->pSavetimeData->u1TxCenter_min_Save[3][0][0][1]=90;
    p->pSavetimeData->u1TxCenter_max_Save[3][0][0][1]=93;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][0][0]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][0][0]=24;
    p->pSavetimeData->u1Txwin_center_Save[3][0][0][0]=93;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][0][0]=81;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][0][0]=105;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[3][0][0][0]=2;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][0][1]=-1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][0][1]=23;
    p->pSavetimeData->u1Txwin_center_Save[3][0][0][1]=91;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][0][1]=79;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][0][1]=104;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[3][0][0][1]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][0][2]=2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][0][2]=24;
    p->pSavetimeData->u1Txwin_center_Save[3][0][0][2]=92;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][0][2]=80;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][0][2]=104;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[3][0][0][2]=2;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][0][3]=-1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][0][3]=23;
    p->pSavetimeData->u1Txwin_center_Save[3][0][0][3]=91;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][0][3]=79;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][0][3]=103;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[3][0][0][3]=3;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][0][4]=5;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][0][4]=24;
    p->pSavetimeData->u1Txwin_center_Save[3][0][0][4]=92;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][0][4]=80;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][0][4]=104;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[3][0][0][4]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][0][5]=-1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][0][5]=24;
    p->pSavetimeData->u1Txwin_center_Save[3][0][0][5]=93;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][0][5]=81;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][0][5]=105;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[3][0][0][5]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][0][6]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][0][6]=23;
    p->pSavetimeData->u1Txwin_center_Save[3][0][0][6]=92;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][0][6]=80;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][0][6]=105;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][0][7]=-1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][0][7]=24;
    p->pSavetimeData->u1Txwin_center_Save[3][0][0][7]=93;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][0][7]=80;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][0][7]=106;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][0][8]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][0][8]=26;
    p->pSavetimeData->u1Txwin_center_Save[3][0][0][8]=91;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][0][8]=79;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][0][8]=104;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][0][9]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][0][9]=24;
    p->pSavetimeData->u1Txwin_center_Save[3][0][0][9]=90;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][0][9]=78;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][0][9]=103;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][0][10]=2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][0][10]=26;
    p->pSavetimeData->u1Txwin_center_Save[3][0][0][10]=91;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][0][10]=78;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][0][10]=104;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][0][11]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][0][11]=26;
    p->pSavetimeData->u1Txwin_center_Save[3][0][0][11]=91;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][0][11]=78;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][0][11]=104;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][0][12]=6;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][0][12]=28;
    p->pSavetimeData->u1Txwin_center_Save[3][0][0][12]=92;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][0][12]=80;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][0][12]=104;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][0][13]=2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][0][13]=28;
    p->pSavetimeData->u1Txwin_center_Save[3][0][0][13]=93;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][0][13]=81;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][0][13]=105;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][0][14]=3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][0][14]=28;
    p->pSavetimeData->u1Txwin_center_Save[3][0][0][14]=92;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][0][14]=79;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][0][14]=105;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][0][0][15]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][0][0][15]=25;
    p->pSavetimeData->u1Txwin_center_Save[3][0][0][15]=91;
    p->pSavetimeData->u1Txfirst_pass_Save[3][0][0][15]=78;
    p->pSavetimeData->u1Txlast_pass_Save[3][0][0][15]=105;
    p->pSavetimeData->u1CBTVref_Save[3][0][0]=31;
    p->pSavetimeData->u1CBTClkDelay_Save[3][0][0]=0;
    p->pSavetimeData->u1CBTCmdDelay_Save[3][0][0]=2;
    p->pSavetimeData->u1CBTCsDelay_Save[3][0][0]=2;
    p->pSavetimeData->u1RxDatlat_Save[3][0][0]=16;
    p->pSavetimeData->u1TxWindowPerbitVref_Save[3][0][0]=14;
    p->pSavetimeData->u1Gating2T_Save[3][1][0][0]=3;
    p->pSavetimeData->u1Gating05T_Save[3][1][0][0]=2;
    p->pSavetimeData->u1Gatingfine_tune_Save[3][1][0][0]=24;
    p->pSavetimeData->u1Gatingucpass_count_Save[3][1][0][0]=47;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[3][1][0][0]=22;
    p->pSavetimeData->u1TxCenter_min_Save[3][1][0][0]=90;
    p->pSavetimeData->u1TxCenter_max_Save[3][1][0][0]=92;
    p->pSavetimeData->u1Gating2T_Save[3][1][0][1]=3;
    p->pSavetimeData->u1Gating05T_Save[3][1][0][1]=2;
    p->pSavetimeData->u1Gatingfine_tune_Save[3][1][0][1]=24;
    p->pSavetimeData->u1Gatingucpass_count_Save[3][1][0][1]=47;
    p->pSavetimeData->u1WriteLeveling_bypass_Save[3][1][0][1]=18;
    p->pSavetimeData->u1TxCenter_min_Save[3][1][0][1]=86;
    p->pSavetimeData->u1TxCenter_max_Save[3][1][0][1]=88;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][0][0]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][0][0]=28;
    p->pSavetimeData->u1Txwin_center_Save[3][1][0][0]=91;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][0][0]=79;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][0][0]=103;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[3][1][0][0]=2;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][0][1]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][0][1]=27;
    p->pSavetimeData->u1Txwin_center_Save[3][1][0][1]=91;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][0][1]=79;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][0][1]=104;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[3][1][0][1]=2;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][0][2]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][0][2]=26;
    p->pSavetimeData->u1Txwin_center_Save[3][1][0][2]=90;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][0][2]=78;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][0][2]=103;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[3][1][0][2]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][0][3]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][0][3]=27;
    p->pSavetimeData->u1Txwin_center_Save[3][1][0][3]=90;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][0][3]=78;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][0][3]=103;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[3][1][0][3]=3;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][0][4]=2;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][0][4]=28;
    p->pSavetimeData->u1Txwin_center_Save[3][1][0][4]=90;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][0][4]=78;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][0][4]=102;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[3][1][0][4]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][0][5]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][0][5]=28;
    p->pSavetimeData->u1Txwin_center_Save[3][1][0][5]=91;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][0][5]=79;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][0][5]=104;
    p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[3][1][0][5]=1;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][0][6]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][0][6]=26;
    p->pSavetimeData->u1Txwin_center_Save[3][1][0][6]=92;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][0][6]=79;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][0][6]=105;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][0][7]=0;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][0][7]=28;
    p->pSavetimeData->u1Txwin_center_Save[3][1][0][7]=91;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][0][7]=78;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][0][7]=104;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][0][8]=3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][0][8]=29;
    p->pSavetimeData->u1Txwin_center_Save[3][1][0][8]=87;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][0][8]=75;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][0][8]=100;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][0][9]=3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][0][9]=28;
    p->pSavetimeData->u1Txwin_center_Save[3][1][0][9]=87;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][0][9]=74;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][0][9]=100;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][0][10]=3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][0][10]=29;
    p->pSavetimeData->u1Txwin_center_Save[3][1][0][10]=86;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][0][10]=73;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][0][10]=100;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][0][11]=3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][0][11]=30;
    p->pSavetimeData->u1Txwin_center_Save[3][1][0][11]=86;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][0][11]=73;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][0][11]=100;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][0][12]=4;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][0][12]=31;
    p->pSavetimeData->u1Txwin_center_Save[3][1][0][12]=87;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][0][12]=74;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][0][12]=100;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][0][13]=3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][0][13]=31;
    p->pSavetimeData->u1Txwin_center_Save[3][1][0][13]=87;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][0][13]=75;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][0][13]=100;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][0][14]=3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][0][14]=30;
    p->pSavetimeData->u1Txwin_center_Save[3][1][0][14]=88;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][0][14]=75;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][0][14]=101;
    p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[3][1][0][15]=3;
    p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[3][1][0][15]=29;
    p->pSavetimeData->u1Txwin_center_Save[3][1][0][15]=88;
    p->pSavetimeData->u1Txfirst_pass_Save[3][1][0][15]=76;
    p->pSavetimeData->u1Txlast_pass_Save[3][1][0][15]=101;
    p->pSavetimeData->u1CBTVref_Save[3][1][0]=29;
    p->pSavetimeData->u1CBTClkDelay_Save[3][1][0]=0;
    p->pSavetimeData->u1CBTCmdDelay_Save[3][1][0]=1;
    p->pSavetimeData->u1CBTCsDelay_Save[3][1][0]=2;
    p->pSavetimeData->u1RxDatlat_Save[3][1][0]=16;
    p->pSavetimeData->u1TxWindowPerbitVref_Save[3][1][0]=14;
    p->pSavetimeData->u1RxWinPerbitVref_Save[3][0]=4;
    #endif
    #endif
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
extern U8 gFinalTXVrefDQM[CHANNEL_NUM][RANK_MAX];
#endif
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

#if DUAL_RANK_ENABLE
    if (p->support_rank_num==RANK_DUAL)
        u1RankMax = RANK_MAX;
    else
#endif
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

    if (p->support_rank_num==RANK_DUAL)
    {
        // make Rank_0/1 CBT Clk/CA/CS dly has same value
        DramcCmdBusTrainingPostProcess(p);
    }

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

        U8 u1TXVrefDQ;
#if TX_K_DQM_WITH_WDBI
        U8 u1TXVrefDQM, u1TXFinalVref=0xff, u1DQMSpecResult, u1TempVref;

        //DramcWriteDBIOnOff() control both rank, need to recover for rank1 tx calibration
        DramcWriteDBIOnOff(p, 0);
#endif
        DramcTxWindowPerbitCal((DRAMC_CTX_T *) p, TX_DQ_DQS_MOVE_DQ_DQM, FALSE);  //Vref scan disable
        u1TXVrefDQ = DramcTxWindowPerbitCal((DRAMC_CTX_T *) p, TX_DQ_DQS_MOVE_DQ_ONLY, p->enable_tx_scan_vref);

#if TX_K_DQM_WITH_WDBI
        if ((p->DBI_W_onoff[p->dram_fsp]==DBI_ON))
        {
            // K DQM with DBI_ON, and check DQM window spec.
            mcSHOW_DBG_MSG(("[TX_K_DQM_WITH_WDBI] Step1: K DQM with DBI_ON, and check DQM window spec.\n\n"));
            vSwitchWriteDBISettings(p, DBI_ON);
            u1DQMSpecResult = DramcTxWindowPerbitCal((DRAMC_CTX_T *) p, TX_DQ_DQS_MOVE_DQM_ONLY, FALSE);

            #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
            if((u1DQMSpecResult==TX_DQM_WINDOW_SPEC_IN) || (p->pSavetimeData->femmc_Ready==1))
            #else
            if(u1DQMSpecResult==TX_DQM_WINDOW_SPEC_IN)
            #endif
            {
                vSwitchWriteDBISettings(p, DBI_OFF);
            }
            else // DQM window out of spec
            {
                //Scan DQM + Vref
                mcSHOW_DBG_MSG(("[TX_K_DQM_WITH_WDBI] Step2: K DQM and Vref with DBI_ON\n\n"));
                u1TXVrefDQM = DramcTxWindowPerbitCal((DRAMC_CTX_T *) p, TX_DQ_DQS_MOVE_DQM_ONLY, TRUE);

                // Set Fincal TX Vref to MR14
                u1TXFinalVref = (u1TXVrefDQ+u1TXVrefDQM)>>1;
                #ifdef FOR_HQA_TEST_USED
                gFinalTXVrefDQ[p->channel][p->rank] = u1TXFinalVref;
                gFinalTXVrefDQM[p->channel][p->rank] = u1TXFinalVref;
                #endif

                #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
                p->pSavetimeData->u1TxWindowPerbitVref_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]]=u1TXFinalVref;
                #endif
                DramcTXSetVref(p, 0, u1TXFinalVref);
                mcSHOW_DBG_MSG(("[TX_K_DQM_WITH_WDBI] Step3: u1TXVrefDQ=%d, u1TXVrefDQM %d, Set MR14=%d\n\n", u1TXVrefDQ, u1TXVrefDQM, u1TXFinalVref));

                mcSHOW_DBG_MSG(("[TX_K_DQM_WITH_WDBI] Step4: Scan DQM\n\n"));
                DramcTxWindowPerbitCal((DRAMC_CTX_T *) p, TX_DQ_DQS_MOVE_DQM_ONLY, FALSE);

                //Write DBI off + Scan DQ
                //Calculate DQM, Save Data for Fast K, update TX tracking settings.
                mcSHOW_DBG_MSG(("[TX_K_DQM_WITH_WDBI] Step5: Write DBI off + Scan DQ\n\n"));
                vSwitchWriteDBISettings(p, DBI_OFF);
                DramcTxWindowPerbitCal((DRAMC_CTX_T *) p, TX_DQ_DQS_MOVE_DQ_ONLY, FALSE);
            }

            #if 0
            u1DQMSpecResult = DramcTxWindowPerbitCal((DRAMC_CTX_T *) p, TX_DQ_DQS_MOVE_DQM_ONLY, FALSE);

            if(u1DQMSpecResult==TX_DQM_WINDOW_SPEC_OUT) // DQM window out of spec
            {
                //Scan DQM + Vref
                mcSHOW_DBG_MSG(("[TX_K_DQM_WITH_WDBI] Step2: K DQM and Vref with DBI_ON\n\n"));
                for(u1TempVref = u1TXVrefDQ-2; u1TempVref>10; u1TempVref-=2)
                {
                    DramcTXSetVref(p, 0, u1TempVref);
                    u1DQMSpecResult = DramcTxWindowPerbitCal((DRAMC_CTX_T *) p, TX_DQ_DQS_MOVE_DQM_ONLY, FALSE);

                    if(u1DQMSpecResult==TX_DQM_WINDOW_SPEC_IN)
                    {
                        u1TXFinalVref = u1TempVref;
                        break;
                    }
                }

                if(u1TXFinalVref== 0xff)
                {
                    mcSHOW_ERR_MSG(("[TX_K_DQM_WITH_WDBI] Step3: Cannot find valid DQM Vref\n\n"));
                    #if __ETT__
                    while(1);
                    #endif
                }
                else
                {
                    mcSHOW_DBG_MSG(("[TX_K_DQM_WITH_WDBI] Step3: u1TXVrefDQ=%d, Set MR14=%d\n\n", u1TXVrefDQ, u1TXFinalVref));
                }

                #ifdef FOR_HQA_TEST_USED
                gFinalTXVrefDQ[p->channel][p->rank] = u1TXFinalVref;
                #endif
            }
            #endif

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

#if DUAL_RANK_ENABLE
    if (p->support_rank_num==RANK_DUAL)
    {
        DramcDualRankRxdatlatCal(p);
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
        vSetRank(p, RANK_1);
        CATrainingLP3(p);
        vSetRank(p, RANK_0);
        if (p->support_rank_num==RANK_DUAL)
        {
            CATrainingLP3PostProcess(p); //set final CLK and CA delay as the result of averge delay of rank0 and rank1
        }
    #endif
#endif

#ifdef DDR_INIT_TIME_PROFILING
    CPU_Cycle=TimeProfileEnd();
    mcSHOW_TIME_MSG(("\tDRAMC CA train takes %d us\n", CPU_Cycle));
    TimeProfileBegin();
#endif

#if GATING_ADJUST_TXDLY_FOR_TRACKING
    DramcRxdqsGatingPreProcess(p);
#endif

#if DUAL_RANK_ENABLE
    if (p->support_rank_num==RANK_DUAL)
        u1RankMax = RANK_MAX;
    else
#endif
        u1RankMax = RANK_1;

        for(s1RankIdx=RANK_0; s1RankIdx<u1RankMax; s1RankIdx++)
        {
            vSetRank(p, s1RankIdx);

            vAutoRefreshSwitch(p, DISABLE); //If auto refresh don't disable at begin, auto refresh will be enable when K rank1 CAtraining and write leveling

#if DUAL_RANK_ENABLE
            if((p->support_rank_num==RANK_SINGLE) || ((p->support_rank_num==RANK_DUAL) && (s1RankIdx == RANK_0)))
#endif
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

        #if DUAL_RANK_ENABLE
        #ifndef LP3_DUAL_RANK_RX_K
            if(s1RankIdx==RANK_0)
        #endif
        #endif
            {
                DramcRxWindowPerbitCal((DRAMC_CTX_T *) p, 1);

            #ifdef DDR_INIT_TIME_PROFILING
                CPU_Cycle=TimeProfileEnd();
                mcSHOW_TIME_MSG(("\tRank %d RX takes %d us\n", s1RankIdx, CPU_Cycle));
                TimeProfileBegin();
            #endif
            }

        #if DUAL_RANK_ENABLE
        #ifndef LP3_DUAL_RANK_TX_K
            if(s1RankIdx==RANK_0)
        #endif
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

    #if DUAL_RANK_ENABLE
        if (p->support_rank_num==RANK_DUAL)
        {
            DramcDualRankRxdatlatCal(p);
        }
    #endif

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
    u4low_tick0 = GPT_GetTickCount(&u4high_tick0);
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
    print_HQA_measure_message(p);
#endif

#ifdef DRAM_CALIB_LOG
    i4WriteSramLog(gDRAM_CALIB_LOG_pointer, (unsigned int*) &gDRAM_CALIB_LOG, sizeof(gDRAM_CALIB_LOG)/4);
    gDRAM_CALIB_LOG_pointer += sizeof(gDRAM_CALIB_LOG)/4;
#endif

    /* Enable/Disable calibrated rank's DBI function accordingly */
#if ENABLE_READ_DBI
	//Read DBI ON
	for(channel_idx=CHANNEL_A; channel_idx<p->support_channel_num; channel_idx++)
	{
	    vSetPHY2ChannelMapping(p, channel_idx);

		for(rank_idx=RANK_0; rank_idx<RANK_MAX; rank_idx++)
        {
            vSetRank(p, rank_idx);

            SetDramModeRegForReadDBIOnOff(p, p->DBI_R_onoff[p->dram_fsp]);
        }
		vSetRank(p, RANK_0);
	}
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

	        SetDramModeRegForWriteDBIOnOff(p, p->DBI_W_onoff[p->dram_fsp]);
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
    u4low_tick1 = GPT_GetTickCount(&u4high_tick1);
    mcSHOW_TIME_MSG(("(3) Freq %d, vDramCalibrationAllChannel() take %d ms\n\r", p->frequency,((u4low_tick1-u4low_tick0)*76)/1000000));
#endif

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

                u1DieNumber =1;
                if(p->dram_type == TYPE_LPDDR3)
                {
                    if(((u2Density >> 6) & 0x3)==1) //OP[7:6] =0, x16 (2 die)
                        u1DieNumber =2;
                } else if (u1IsLP4Family(p->dram_type)) {
                    if(((u2Density >> 6) & 0x3) == 1) //OP[7:6] =0, x16 (normal mode)
                        u1DieNumber =2;
                }

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
            	        //case 0x5:
            	            //u8Size = 0x100000000L; //32Gb
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

#if DUAL_RANK_ENABLE
                u4RankNumber = 2;
#endif
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
    if(p->pSavetimeData->femmc_Ready==1)
    {
        for(channel_idx=CHANNEL_A; channel_idx<p->support_channel_num; channel_idx++)
        {
            for (shuffleIdx = DRAM_DFS_SHUFFLE_1; shuffleIdx < DRAM_DFS_SHUFFLE_MAX; shuffleIdx++)
            {
                ucg_num_dlycell_perT_all[shuffleIdx][channel_idx] = p->pSavetimeData->ucg_num_dlycell_perT_all[shuffleIdx][channel_idx];
                u2gdelay_cell_ps_all[shuffleIdx][channel_idx] = p->pSavetimeData->u2gdelay_cell_ps_all[shuffleIdx][channel_idx];
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
    if(p->pSavetimeData->femmc_Ready==0)
    {
        for(channel_idx=CHANNEL_A; channel_idx<p->support_channel_num; channel_idx++)
        {
            for (shuffleIdx = DRAM_DFS_SHUFFLE_1; shuffleIdx < DRAM_DFS_SHUFFLE_MAX; shuffleIdx++)
            {
                p->pSavetimeData->ucg_num_dlycell_perT_all[shuffleIdx][channel_idx] = ucg_num_dlycell_perT_all[shuffleIdx][channel_idx];
                p->pSavetimeData->u2gdelay_cell_ps_all[shuffleIdx][channel_idx] = u2gdelay_cell_ps_all[shuffleIdx][channel_idx];
            }
        }
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

    mcSHOW_ERR_MSG(("fra femmc_Ready = %d ==\n",p->pSavetimeData->femmc_Ready));

    if (p->pSavetimeData->femmc_Ready==0 ||
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
    write_offline_dram_calibration_data(p->pSavetimeData);
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
    U32 u4low_tick0, u4high_tick0, u4low_tick1, u4high_tick1;
    u4low_tick0 = GPT_GetTickCount(&u4high_tick0);

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
    p = psCurrDramCtx;

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

#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
        {
            U8 backup_broadcast;
            backup_broadcast = GetDramcBroadcast();
            EMI_Init(p);
            DramcBroadcastOnOff(backup_broadcast);
        }
#endif


    mcSHOW_DBG_MSG(("\n\n[Bianco] ETT version 0.0.0.1\n dram_type %d, R0 cbt_mode %d, R1 cbt_mode %d VENDOR=%d\n\n", p->dram_type, p->dram_cbt_mode[RANK_0], p->dram_cbt_mode[RANK_1], p->vendor_id));

    #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    	DramcSave_Time_For_Cal_Init(p);
    #endif

    vDramcInit_PreSettings(p);

#ifdef DDR_INIT_TIME_PROFILING
        CPU_Cycle=TimeProfileEnd();
        mcSHOW_TIME_MSG(("(0) Pre_Init takes %d ms\n\r", CPU_Cycle/1000));
        TimeProfileBegin();
#endif


    #if DUAL_FREQ_K
    SPM_Pinmux_Setting(p);
    #endif

   // DramC & PHY init for all channels
      DDRPhyFreqSel(p, p->freq_sel);
      vSetVcoreByFreq(p);

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
      mcSHOW_TIME_MSG(("SwImdepance takes %d us\n\r", CPU_Cycle));
#endif

      DFSInitForCalibration(p);


#ifdef DDR_INIT_TIME_PROFILING
      CPU_Cycle=TimeProfileEnd();    
      TimeProfileBegin();
#endif 

      PRE_MIOCK_JMETER_HQA_USED(p);

#ifdef DDR_INIT_TIME_PROFILING
      CPU_Cycle=TimeProfileEnd();    
      mcSHOW_TIME_MSG(("(1) JMeter takes %d ms\n\r", CPU_Cycle/1000));      
      TimeProfileBegin();
#endif 

#if defined(ENABLE_DRAM_SLT)
      O1Path_Test(p);
#endif

#ifdef TEST_MODE_MRS
    if(global_which_test  == 0)
      TestModeTestMenu();
#endif

#if ENABLE_RANK_NUMBER_AUTO_DETECTION  // only need to do this when DUAL_RANK_ENABLE is 1
        DramRankNumberDetection(p);
        DFSInitForCalibration(p);  // Restore setting after rank dection (especially DQ= DQS+16)
#endif

        #if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
        U8 backup_broadcast;
        backup_broadcast = GetDramcBroadcast();
        EMI_Init2();
        DramcBroadcastOnOff(backup_broadcast);
        #endif

#ifdef DDR_INIT_TIME_PROFILING
        CPU_Cycle=TimeProfileEnd();
        mcSHOW_TIME_MSG(("Init take %d us\n\r", CPU_Cycle));
#endif

        if (Get_MDL_Used_Flag()==GET_MDL_USED)
        {
            vSetPHY2ChannelMapping(p, CHANNEL_A);
            vDramCalibrationSingleChannel(p);  // only K CHA to save time
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
	#if defined(ENABLE_DRAM_SLT)
        DDRPhyFreqSel(p, LP4_HIGHEST_FREQSEL);
	#else
        DDRPhyFreqSel(p, LP4_MIDDLE_FREQSEL);
	#endif
        vSetVcoreByFreq(p);
        DFSInitForCalibration(p);
        vDramCalibrationAllChannel(p);
        vDramcACTimingOptimize(p);
        DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_1, DRAM_DFS_SHUFFLE_2); //Darren NOTE: Please take care of gFreqTbl table when you update SHUFFLE_X

	#if defined(ENABLE_DRAM_SLT)
        DDRPhyFreqSel(p, LP4_DDR2400);
	#else
        DDRPhyFreqSel(p, LP4_HIGHEST_FREQSEL);
	#endif
        vSetVcoreByFreq(p);
        DFSInitForCalibration(p);
        vDramCalibrationAllChannel(p);
        vDramcACTimingOptimize(p);

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

        DDRPhyFreqSel(p, LP3_DDR1866);
        vSetVcoreByFreq(p);
        DFSInitForCalibration(p);
        vDramCalibrationAllChannel(p);
    #endif
    }
#endif

#ifdef DDR_INIT_TIME_PROFILING
    TimeProfileBegin();
#endif

    vApplyConfigAfterCalibration(p);
#if FORLOOP_TEST_SAVE_TIME_FOR_CALIBRATION_ON_ETT
	vDramfirstCalibrationdump(p);
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

#if RUNTIME_SHMOO_RELEATED_FUNCTION && SUPPORT_SAVE_TIME_FOR_CALIBRATION
    DramcRunTimeShmooRG_BackupRestore(p);
#endif

#if RUNTIME_SHMOO_RELEATED_FUNCTION && SUPPORT_SAVE_TIME_FOR_CALIBRATION
    RunTime_Shmoo_update_parameters(p);
#endif

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    if(u1IsLP4Family(p->dram_type) && (p->pSavetimeData->femmc_Ready==0))
    {
        #if EMMC_READY
        write_offline_dram_calibration_data(p->pSavetimeData);
        #endif
        mcSHOW_DBG_MSG(("Save calibration result to emmc\n"));
    }
    else
    {
        mcSHOW_DBG_MSG(("Bypass saving calibration result to emmc\n"));
    }
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
    //vDramCPUReadWriteTestAfterCalibration2(p);
    #endif

    #if TA2_RW_TEST_AFTER_K
    mcSHOW_DBG_MSG(("\n[TA2_TEST]\n"));
    TA2_Test_Run_Time_HW(p);
    #endif

    mcSHOW_DBG_MSG(("\n\nSettings after calibration\n\n"));
    DramcRunTimeConfig(p);

    #if CPU_RW_TEST_AFTER_K
    mcSHOW_DBG_MSG(("\n[MEM_TEST] 03: After run time config\n"));
    vDramCPUReadWriteTestAfterCalibration(p);
    //vDramCPUReadWriteTestAfterCalibration2(p);
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
        //while(1);
    }
}
#endif

#ifdef DDR_INIT_TIME_PROFILING
    CPU_Cycle=TimeProfileEnd();
    mcSHOW_TIME_MSG(("(4) After calibration takes %d ms\n\r", CPU_Cycle/1000));

    u4low_tick1 = GPT_GetTickCount(&u4high_tick1);
    mcSHOW_TIME_MSG(("Total Init_Dram() take %d ms\n\n", ((u4low_tick1-u4low_tick0)*76)/1000000));
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

#if ENABLE_LP3_SW
void DPI_SW_main_LP3(DRAMC_CTX_T *DramConfig)
{
    int ii;

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

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);  //LP3 broadcast off

    Global_Option_Init(DramConfig);

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
        if (DramConfig->support_rank_num==RANK_DUAL)
        {
            // make Rank_0/1 CBT Clk/CA/CS dly has same value
            DramcCmdBusTrainingPostProcess(DramConfig);
        }
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


