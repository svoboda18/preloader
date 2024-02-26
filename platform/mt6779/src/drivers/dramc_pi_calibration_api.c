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
/*-----------------------------------------------------------------------------
 *
 * $Author: jc.wu $
 * $Date: 2012/6/5 $
 * $RCSfile: pi_calibration_api.c,v $
 * $Revision: #5 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_calibration_api.c
 *  Basic DRAMC calibration API implementation
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
//#include "..\Common\pd_common.h"
#include "dramc_common.h"
#include "x_hal_io.h"
#include "dramc_pi_api.h"
#include "dramc_actiming.h"
//#include "DramC_reg.h"
//#include "System_reg.h"

#if 1//REG_ACCESS_PORTING_DGB
U8 RegLogEnable=0;
#endif

#if REG_SHUFFLE_REG_CHECK
U8 ShuffleRegCheck =0;
#endif
//#define CATRAINING_NUM_LP3      10
#define CATRAINING_NUM_LP4      6
#define CATRAINING_NUM          6  // Lafi_te, LP4 only
#define DIE_NUM_MAX         1 //Lafi_te, LP4 only

#define MAX_CA_PI_DELAY         63
#define MAX_CS_PI_DELAY         63
#define MAX_CLK_PI_DELAY        31

#define PASS_RANGE_NA   0x7fff

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
static U8 fgwrlevel_done = 0;

#if defined(RELEASE)
U8 gEye_Scan_color_flag = 0;
U8 gCBT_EYE_Scan_flag = 0;
U8 gCBT_EYE_Scan_only_higheset_freq_flag = 1;
U8 gRX_EYE_Scan_flag = 0;
U8 gRX_EYE_Scan_only_higheset_freq_flag = 1;
U8 gTX_EYE_Scan_flag = 1;
U8 gTX_EYE_Scan_only_higheset_freq_flag = 1;
#elif  (CFG_DRAM_LOG_TO_STORAGE)
U8 gEye_Scan_color_flag = 0;
U8 gCBT_EYE_Scan_flag = 0;
U8 gCBT_EYE_Scan_only_higheset_freq_flag = 1;
U8 gRX_EYE_Scan_flag = 1;
U8 gRX_EYE_Scan_only_higheset_freq_flag = 1;
U8 gTX_EYE_Scan_flag = 1;
U8 gTX_EYE_Scan_only_higheset_freq_flag = 1;
#else
U8 gEye_Scan_color_flag = 1;
U8 gCBT_EYE_Scan_flag = 0;
U8 gCBT_EYE_Scan_only_higheset_freq_flag = 1;
U8 gRX_EYE_Scan_flag = 0;
U8 gRX_EYE_Scan_only_higheset_freq_flag = 1;
U8 gTX_EYE_Scan_flag = 0;
U8 gTX_EYE_Scan_only_higheset_freq_flag = 1;
#endif

#ifdef FOR_HQA_REPORT_USED
#if CFG_DRAM_LOG_TO_STORAGE
U8 gHQALog_flag = 1;
#else
U8 gHQALog_flag = 0;
#endif
U16 gHQALOG_RX_delay_cell_ps_075V = 0;
#endif

#if __ETT__
U8 gETT_WHILE_1_flag = 1;
#endif

U16 gu2MR0_Value[RANK_MAX] = {0xffff, 0xffff};
U8 u1MR01Value[FSP_MAX];
U8 u1MR02Value[FSP_MAX];
U8 u1MR03Value[FSP_MAX];
U8 u1MR13Value;

U8 u1MR12Value[CHANNEL_NUM][RANK_MAX][FSP_MAX];
U8 u1MR14Value[CHANNEL_NUM][RANK_MAX][FSP_MAX];

#if ENABLE_RDDQC_DIV4_WA
// for 1:4 mode RDDQC workaround
U8 u1MR15Value = 0x55;
U8 u1MR20Value = 0x55;
U8 u1MR32Value = 0x5a;
U8 u1MR40Value = 0x3c;
#endif

#if MRW_CHECK_ONLY
U16 u2MRRecord[CHANNEL_NUM][RANK_MAX][FSP_MAX][MR_NUM];
#endif

U8 gCBT_VREF_RANGE_SEL;
U8 gCBT_VREF_RANGE_BEGIN;
U8 gCBT_VREF_RANGE_END;
U8 gCBT_VREF_RANGE_STEP;

U8 gu1DieNum[2]; // 2 rank may have different die number

static S32 CATrain_CmdDelay[CHANNEL_NUM][RANK_MAX];
static U32 CATrain_CsDelay[CHANNEL_NUM][RANK_MAX];
static S32 CATrain_ClkDelay[CHANNEL_NUM][RANK_MAX];
static S32 wrlevel_dqs_final_delay[DQS_NUMBER]; // 3 is channel number
#if ENABLE_DDR800_OPEN_LOOP_PHASE_PREDICTION
S32 wrlevel_ddr800_dqs_delay_prediction[CHANNEL_NUM][DQS_NUMBER]={{0xFF,0xFF},{0xFF,0xFF}};
#endif
static S32 wrlevel_dqs_ddr800_rk0_delay[DQS_NUMBER]; // 3 is channel number
static U8 ucg_num_dlycell_perT = 49; //from 60807
U16 u2gdelay_cell_ps;
U8 ucg_num_dlycell_perT_all[DRAM_DFS_SHUFFLE_MAX][CHANNEL_NUM];///TODO: to be removed by Francis
U16 u2gdelay_cell_ps_all[DRAM_DFS_SHUFFLE_MAX][CHANNEL_NUM];///TODO: to be removed by Francis
U32 u4gVcore[DRAM_DFS_SHUFFLE_MAX];

U32 gDramcSwImpedanceResult[2][4]={{0,0,0,0},{0,0,0,0}};//ODT_ON/OFF x DRVP/DRVN/ODTP/ODTN
#if APPLY_SIGNAL_WAVEFORM_SETTINGS_ADJUST
S8  gDramcSwImpedanceAdjust[2][4]={{0,0,0,0},{0,0,0,0}};//ODT_ON/OFF x DRVP/DRVN/ODTP/ODTN
S8  gDramcDqOdtRZQAdjust=-1;
S8  gDramcMR03PDDSAdjust[FSP_MAX]={-1,-1};
S8  gDramcMR22SoCODTAdjust[FSP_MAX]={-1,-1};
#endif

S8 iFirstCAPass[RANK_MAX][DIE_NUM_MAX][CATRAINING_NUM], iLastCAPass[RANK_MAX][DIE_NUM_MAX][CATRAINING_NUM];  // 2 die.

S16 gu2RX_DQS_Duty_Offset[DQS_NUMBER][2];
#if PINMUX_AUTO_TEST_PER_BIT_RX
U16 gFinalRXPerbitFirstPass[CHANNEL_NUM][DQ_DATA_WIDTH];
#endif
#if PINMUX_AUTO_TEST_PER_BIT_TX
U16 gFinalTXPerbitFirstPass[CHANNEL_NUM][DQ_DATA_WIDTH];
#endif

#ifdef FOR_HQA_TEST_USED
U16 gFinalCBTVrefCA[CHANNEL_NUM][RANK_MAX];
U16 gFinalCBTCA[CHANNEL_NUM][RANK_MAX][10];
U16 gFinalRXPerbitWin[CHANNEL_NUM][RANK_MAX][DQ_DATA_WIDTH];
U16 gFinalTXPerbitWin[CHANNEL_NUM][RANK_MAX][DQ_DATA_WIDTH];
U16 gFinalTXPerbitWin_min_max[CHANNEL_NUM][RANK_MAX];
U16 gFinalTXPerbitWin_min_margin[CHANNEL_NUM][RANK_MAX];
U16 gFinalTXPerbitWin_min_margin_bit[CHANNEL_NUM][RANK_MAX];
S8 gFinalClkDuty[CHANNEL_NUM];
U32 gFinalClkDutyMinMax[CHANNEL_NUM][2];
S8 gFinalDQSDuty[CHANNEL_NUM][DQS_NUMBER];
U32 gFinalDQSDutyMinMax[CHANNEL_NUM][DQS_NUMBER][2];
#endif
U8 gFinalCBTVrefDQ[CHANNEL_NUM][RANK_MAX];
U8 gFinalRXVrefDQ[CHANNEL_NUM][RANK_MAX][2];
U8 gFinalTXVrefDQ[CHANNEL_NUM][RANK_MAX];

U8 gFinalRXVrefDQForSpeedUp[CHANNEL_NUM][RANK_MAX][2/*ODT_onoff*/][2/*2bytes*/]={0};

#define RX_DELAY_PRE_CAL 1
#if RX_DELAY_PRE_CAL
S16 s2RxDelayPreCal=PASS_RANGE_NA;
#endif

extern const U8 uiLPDDR4_MRR_Mapping_POP_TX[CHANNEL_NUM][16];
extern const U8 uiLPDDR4_O1_Mapping_POP[CHANNEL_NUM][16];
#if (CA_PER_BIT_DELAY_CELL || PINMUX_AUTO_TEST_PER_BIT_CA)
extern const U8 uiLPDDR4_CA_Mapping_POP[CHANNEL_NUM][6]; 
#endif

extern DRAM_DFS_FREQUENCY_TABLE_T gFreqTbl[];


void dle_factor_handler(DRAMC_CTX_T *p, U8 curr_val, U8 pip_num);
U8 GetCBTVrefPinMuxValue(DRAMC_CTX_T *p, U8 u1VrefLevel);
U8 get_shuffleIndex_by_Freq(DRAMC_CTX_T *p); //Retrieve LP4's shuffle index from gFreqTbl
void HQA_LOG_Print_Freq_String(DRAMC_CTX_T *p);


#define IN_CBT  (0)
#define OUT_CBT (1)

void vInitGlobalVariablesByCondition(void)
{
    U8 u1CHIdx, u1RankIdx, u1FSPIdx, u1MRIdx;

    u1MR01Value[FSP_0] = 0x26;
    u1MR01Value[FSP_1] = 0x56;

    u1MR03Value[FSP_0] = 0x31; //Set write post-amble as 0.5 tck
#if ENABLE_WRITE_POST_AMBLE_1_POINT_5_TCK
    u1MR03Value[FSP_1] = 0x33; //Set write post-amble as 1.5 tck, support after Eig_er E2
#else
    u1MR03Value[FSP_1] = 0x31; //Set write post-amble as 0.5 tck
#endif

    
    for(u1FSPIdx=0; u1FSPIdx<FSP_MAX; u1FSPIdx++)
    {
        u1MR02Value[u1FSPIdx]=0x1a;
    }

    for(u1CHIdx=0; u1CHIdx<CHANNEL_NUM; u1CHIdx++)
        for(u1RankIdx=0; u1RankIdx< RANK_MAX; u1RankIdx++)
            for(u1FSPIdx=0; u1FSPIdx<FSP_MAX; u1FSPIdx++)
            {
                // MR14 default value, LP4 default 0x4d, LP4X 0x5d
                u1MR14Value[u1CHIdx][u1RankIdx][u1FSPIdx] = (u1FSPIdx==FSP_0) ? 0x5d : 0x18;  //0x18: customize for Eig_er
                #if FSP1_CLKCA_TERM
                u1MR12Value[u1CHIdx][u1RankIdx][u1FSPIdx] = (u1FSPIdx==FSP_0) ? 0x5d : 0x20;
                #else
                u1MR12Value[u1CHIdx][u1RankIdx][u1FSPIdx] = 0x5d;
                #endif
                #if MRW_CHECK_ONLY
                for(u1MRIdx=0; u1MRIdx<MR_NUM; u1MRIdx++)
                    u2MRRecord[u1CHIdx][u1RankIdx][u1FSPIdx][u1MRIdx] =0xffff;
                #endif
            }
            
    memset(gu2RX_DQS_Duty_Offset, 0, sizeof(gu2RX_DQS_Duty_Offset));    
}


#if PRINT_CALIBRATION_SUMMARY
void vSetCalibrationResult(DRAMC_CTX_T *p, U8 ucCalType, U8 ucResult)
{
    p->aru4CalExecuteFlag[p->channel][p->rank] |= (1<<ucCalType); // ececution done
    if (ucResult == DRAM_OK)  // Calibration OK
    {
        p->aru4CalResultFlag[p->channel][p->rank] &= (~(1<<ucCalType));
    }
    else  //Calibration fail
    {
        p->aru4CalResultFlag[p->channel][p->rank] |= (1<<ucCalType);
    }
}

#if 0  //no use now, disable for saving code size.
void vGetCalibrationResult_All(DRAMC_CTX_T *p, U8 u1Channel, U8 u1Rank, U8 u1FreqType, U32 *u4CalExecute, U32 *u4CalResult)
{
    *u4CalExecute = p->aru4CalExecuteFlag[u1Channel][u1Rank];
    *u4CalResult = p->aru4CalResultFlag[u1Channel][u1Rank];
}

void vGetCalibrationResult(DRAMC_CTX_T *p, U8 ucCalType, U8 *ucCalExecute, U8 *ucCalResult)
{
    U32 ucCalResult_All, ucCalExecute_All;

    ucCalExecute_All = p->aru4CalExecuteFlag[p->channel][p->rank];
    ucCalResult_All = p->aru4CalResultFlag[p->channel][p->rank];

    *ucCalExecute = (U8)((ucCalExecute_All >>ucCalType) & 0x1);
    *ucCalResult =  (U8)((ucCalResult_All >>ucCalType) & 0x1);
}
#endif

const char *szCalibStatusName[DRAM_CALIBRATION_MAX]=
{
    "ZQ Calibration",
    "SW Impedance",
    "CA Training",
    "Write leveling",
    "RX DQS gating",
    "RX DATLAT",
    "RX DQ/DQS(RDDQC)",
    "RX DQ/DQS(Engine)",
    "TX DQ/DQS",
};

void vPrintCalibrationResult(DRAMC_CTX_T *p)
{
    U8 ucCHIdx, ucRankIdx, ucCalIdx;
    U32 ucCalResult_All, ucCalExecute_All;
    U8 ucCalResult, ucCalExecute;
    U8 u1CalibrationFail;

    mcSHOW_DBG_MSG(("\n\n[Calibration Summary] Freqency %d\n", p->frequency));

    //for(ucFreqIdx=0; ucFreqIdx<DRAM_DFS_SHUFFLE_MAX; ucFreqIdx++)
    {
        //mcSHOW_DBG_MSG(("==Freqency = %d==\n", get_FreqTbl_by_shuffleIndex(p,ucFreqIdx)->frequency));
        for(ucCHIdx=0; ucCHIdx<p->support_channel_num; ucCHIdx++)
        {
            for(ucRankIdx=0; ucRankIdx<RANK_MAX; ucRankIdx++)
            {
                u1CalibrationFail =0;
                ucCalExecute_All = p->aru4CalExecuteFlag[ucCHIdx][ucRankIdx];
                ucCalResult_All = p->aru4CalResultFlag[ucCHIdx][ucRankIdx];
                mcSHOW_DBG_MSG(("CH %d, Rank %d\n", ucCHIdx, ucRankIdx));
                //mcSHOW_DBG_MSG(("[vPrintCalibrationResult] Channel = %d, Rank= %d, Freq.= %d, (ucCalExecute_All 0x%x, ucCalResult_All 0x%x)\n", ucCHIdx, ucRankIdx, ucFreqIdx, ucCalExecute_All, ucCalResult_All));

                for(ucCalIdx =0; ucCalIdx<DRAM_CALIBRATION_MAX; ucCalIdx++)
                {
                    ucCalExecute = (U8)((ucCalExecute_All >>ucCalIdx) & 0x1);
                    ucCalResult =  (U8)((ucCalResult_All >>ucCalIdx) & 0x1);

                    if(ucCalExecute==1 && ucCalResult ==1) // excuted and fail
                    {
                        u1CalibrationFail =1;
                        mcSHOW_DBG_MSG(("%s: %s\n", szCalibStatusName[ucCalIdx], ((ucCalResult == 0) ? "OK" : "Fail")));
                    }
                }

                if(u1CalibrationFail ==0)
                {
                    mcSHOW_DBG_MSG(("All Pass.\n"));
                }
                mcSHOW_DBG_MSG(("\n"));
            }
        }
    }

    memset(p->aru4CalResultFlag, 0, sizeof(p->aru4CalResultFlag));
    memset(p->aru4CalExecuteFlag, 0, sizeof(p->aru4CalExecuteFlag));
}
#endif

void vResetDelayChainBeforeCalibration(DRAMC_CTX_T *p)
{
    U8 u1RankIdx, u1RankIdxBak;

    u1RankIdxBak = u1GetRank(p);

    for(u1RankIdx=RANK_0; u1RankIdx<RANK_MAX; u1RankIdx++)
    {
        vSetRank(p, u1RankIdx);

        vIO32WriteFldMulti_All(DDRPHY_SHU_R0_CA_CMD0, P_Fld(0x0, SHU_R0_CA_CMD0_RK0_TX_ARCA5_DLY) 
                | P_Fld(0x0, SHU_R0_CA_CMD0_RK0_TX_ARCA4_DLY) 
                | P_Fld(0x0, SHU_R0_CA_CMD0_RK0_TX_ARCA3_DLY) 
                | P_Fld(0x0, SHU_R0_CA_CMD0_RK0_TX_ARCA2_DLY) 
                | P_Fld(0x0, SHU_R0_CA_CMD0_RK0_TX_ARCA1_DLY) 
                | P_Fld(0x0, SHU_R0_CA_CMD0_RK0_TX_ARCA0_DLY));    
        vIO32WriteFldMulti_All(DDRPHY_SHU_R0_B0_DQ0, P_Fld(0x0, SHU_R0_B0_DQ0_RK0_TX_ARDQ7_DLY_B0) 
                | P_Fld(0x0, SHU_R0_B0_DQ0_RK0_TX_ARDQ6_DLY_B0) 
                | P_Fld(0x0, SHU_R0_B0_DQ0_RK0_TX_ARDQ5_DLY_B0) 
                | P_Fld(0x0, SHU_R0_B0_DQ0_RK0_TX_ARDQ4_DLY_B0) 
                | P_Fld(0x0, SHU_R0_B0_DQ0_RK0_TX_ARDQ3_DLY_B0) 
                | P_Fld(0x0, SHU_R0_B0_DQ0_RK0_TX_ARDQ2_DLY_B0) 
                | P_Fld(0x0, SHU_R0_B0_DQ0_RK0_TX_ARDQ1_DLY_B0) 
                | P_Fld(0x0, SHU_R0_B0_DQ0_RK0_TX_ARDQ0_DLY_B0));    
        vIO32WriteFldMulti_All(DDRPHY_SHU_R0_B1_DQ0, P_Fld(0x0, SHU_R0_B1_DQ0_RK0_TX_ARDQ7_DLY_B1) 
                | P_Fld(0x0, SHU_R0_B1_DQ0_RK0_TX_ARDQ6_DLY_B1) 
                | P_Fld(0x0, SHU_R0_B1_DQ0_RK0_TX_ARDQ5_DLY_B1) 
                | P_Fld(0x0, SHU_R0_B1_DQ0_RK0_TX_ARDQ4_DLY_B1) 
                | P_Fld(0x0, SHU_R0_B1_DQ0_RK0_TX_ARDQ3_DLY_B1) 
                | P_Fld(0x0, SHU_R0_B1_DQ0_RK0_TX_ARDQ2_DLY_B1) 
                | P_Fld(0x0, SHU_R0_B1_DQ0_RK0_TX_ARDQ1_DLY_B1) 
                | P_Fld(0x0, SHU_R0_B1_DQ0_RK0_TX_ARDQ0_DLY_B1));
        vIO32WriteFldAlign_All(DDRPHY_SHU_R0_B0_DQ1, 0x0, SHU_R0_B0_DQ1_RK0_TX_ARDQM0_DLY_B0);
        vIO32WriteFldAlign_All(DDRPHY_SHU_R0_B1_DQ1, 0x0, SHU_R0_B1_DQ1_RK0_TX_ARDQM0_DLY_B1);
    }

    vSetRank(p, u1RankIdxBak);
}


//Reset PHY to prevent glitch when change DQS gating delay or RX DQS input delay
// [Lynx] Evere_st : cannot reset single channel. All DramC and All Phy have to reset together.
void DramPhyReset(DRAMC_CTX_T *p)
{
    // Evere_st change reset order : reset DQS before DQ, move PHY reset to final.
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DDRCONF0), 1, DDRCONF0_RDATRST);// read data counter reset
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 1, MISC_CTRL1_R_DMPHYRST);

    //RG_ARCMD_RESETB & RG_ARDQ_RESETB_B0/1 only reset once at init, Justin Chan.
    ///TODO: need to confirm RG_ARCMD_RESETB & RG_ARDQ_RESETB_B0/1 is reset at mem.c
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B0_DQ9), P_Fld(0, B0_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B0) |P_Fld(0, B0_DQ9_RG_RX_ARDQ_STBEN_RESETB_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B1_DQ9), P_Fld(0, B1_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B1) |P_Fld(0, B1_DQ9_RG_RX_ARDQ_STBEN_RESETB_B1));
    #ifdef LOOPBACK_TEST
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_CA_CMD10), P_Fld(0, CA_CMD10_RG_RX_ARCLK_STBEN_RESETB) | P_Fld(0, CA_CMD10_RG_RX_ARCMD_STBEN_RESETB));
    #endif
    mcDELAY_US(1);//delay 10ns
    #ifdef LOOPBACK_TEST
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_CA_CMD10), P_Fld(1, CA_CMD10_RG_RX_ARCLK_STBEN_RESETB) | P_Fld(1, CA_CMD10_RG_RX_ARCMD_STBEN_RESETB));
    #endif
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B1_DQ9), P_Fld(1, B1_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B1) | P_Fld(1, B1_DQ9_RG_RX_ARDQ_STBEN_RESETB_B1));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B0_DQ9), P_Fld(1, B0_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B0) |P_Fld(1, B0_DQ9_RG_RX_ARDQ_STBEN_RESETB_B0));

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 0, MISC_CTRL1_R_DMPHYRST);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DDRCONF0), 0, DDRCONF0_RDATRST);// read data counter reset
}

#if 0
DRAM_STATUS_T DramcRankSwap(DRAMC_CTX_T *p, U8 u1Rank)
{
    U8 u1Multi;

#if 0
    if (p->support_rank_num > 1)
        u1Multi = 1;
    else
        u1Multi = 0;
#else
    //RANK_DUAL or RANK_SINGLE, all set RKMODE to 1
    u1Multi = 1;
#endif

    mcSHOW_DBG_MSG(("[RankSwap] Rank num %d, (Multi %d), Rank %d\n", p->support_rank_num, u1Multi, u1Rank));
    //mcFPRINTF((fp_A60501, "[DramcRankSwap] Rank number %d, (u1Multi %d), Rank %d\n", p->support_rank_num, u1Multi, u1Rank));

    //Set to non-zero for multi-rank
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), u1Multi, RKCFG_RKMODE);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), u1Rank, RKCFG_RKSWAP);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), u1Rank, RKCFG_TXRANK); //use other rank's setting //TXRANK should be set before TXRANKFIX

    if (u1Rank == 0)
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), 0, RKCFG_TXRANKFIX);
    }
    else
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), 1, RKCFG_TXRANKFIX);
    }

    return DRAM_OK;
}
#endif

#if SIMULATION_LP4_ZQ
//-------------------------------------------------------------------------
/** DramcZQCalibration
 *  start Dram ZQ calibration.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL
 */
//-------------------------------------------------------------------------
DRAM_STATUS_T DramcZQCalibration(DRAMC_CTX_T *p)
{
    U32 u4Response;
    U32 u4TimeCnt = TIME_OUT_CNT;
    U32 u4RegBackupAddress[] = {DRAMC_REG_ADDR(DRAMC_REG_MRS), DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), DRAMC_REG_ADDR(DRAMC_REG_CKECTRL)};

    // Backup rank, CKE fix on/off, HW MIOCK control settings
    DramcBackupRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));

    mcSHOW_DBG_MSG3(("[ZQCalibration]\n"));
    mcFPRINTF((fp_A60501, "[ZQCalibration]\n"));

    // Disable HW MIOCK control to make CLK always on
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 1, DRAMC_PD_CTRL_MIOCKCTRLOFF);
    mcDELAY_US(1);

    //if CKE2RANK=1, only need to set CKEFIXON, it will apply to both rank.
    CKEFixOnOff(p, p->rank, CKE_FIXON, CKE_WRITE_TO_ONE_CHANNEL);

    //Use rank swap or MRSRK to select rank
    //DramcRankSwap(p, p->rank);
    //!!R_DMMRSRK(R_DMMPCRKEN=1) specify rank0 or rank1
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1GetRank(p), MRS_MRSRK);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MPC_OPTION), 1, MPC_OPTION_MPCRKEN);

    //ZQCAL Start
    //R_DMZQCEN, 0x1E4[4]=1 for ZQCal Start
    //Wait zqc_response=1 (dramc_conf_nao, 0x3b8[4])
    //R_DMZQCEN, 0x1E4[4]=0
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1, SPCMD_ZQCEN);
    do
    {
        u4Response = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_ZQC_RESPONSE);
        u4TimeCnt --;
        mcDELAY_US(1);  // Wait tZQCAL(min) 1us or wait next polling

        mcSHOW_DBG_MSG3(("%d- ", u4TimeCnt));
        mcFPRINTF((fp_A60501, "%d- ", u4TimeCnt));
    }while((u4Response==0) &&(u4TimeCnt>0));

    if(u4TimeCnt==0)//time out
    {
        vSetCalibrationResult(p, DRAM_CALIBRATION_ZQ, DRAM_FAIL);
        mcSHOW_DBG_MSG(("ZQCAL Start fail (time out)\n"));
        mcFPRINTF((fp_A60501, "ZQCAL Start fail (time out)\n"));
        return DRAM_FAIL;
    }
    else
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_ZQCEN);
    }

    // [JC] delay tZQCAL
    mcDELAY_US(1);
    u4TimeCnt = TIME_OUT_CNT;

    //ZQCAL Latch
    //R_DMZQLATEN, 0x1E4[6]=1 for ZQCal latch
    //Wait zqlat_response=1 (dramc_conf_nao, 0x3b8[28])
    //R_DMZQLATEN, 0x1E4[6]=0
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1, SPCMD_ZQLATEN);
    do
    {
        u4Response = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_ZQLAT_RESPONSE);
        u4TimeCnt --;
        mcDELAY_US(1);// Wait tZQLAT 30ns or wait next polling

        mcSHOW_DBG_MSG3(("%d=", u4TimeCnt));
        mcFPRINTF((fp_A60501, "%d= ", u4TimeCnt));
    }while((u4Response==0) &&(u4TimeCnt>0));

    if(u4TimeCnt==0)//time out
    {
        vSetCalibrationResult(p, DRAM_CALIBRATION_ZQ, DRAM_FAIL);
        mcSHOW_DBG_MSG(("ZQCAL Latch fail (time out)\n"));
        mcFPRINTF((fp_A60501, "ZQCAL Latch fail (time out)\n"));
        return DRAM_FAIL;
    }
    else
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_ZQLATEN);
    }

    // [JC] delay tZQLAT
    mcDELAY_US(1);

    // Restore rank, CKE fix on, HW MIOCK control settings
    DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));

    vSetCalibrationResult(p, DRAM_CALIBRATION_ZQ, DRAM_OK);
    mcSHOW_DBG_MSG3(("\n[DramcZQCalibration] Done\n\n"));
    mcFPRINTF((fp_A60501, "\n[DramcZQCalibration] Done\n\n"));

    return DRAM_OK;
}
#endif

#if SIMULATION_RX_INPUT_BUF
//-------------------------------------------------------------------------
/** DramcSwImpedanceCal
 *  start TX OCD impedance calibration.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param  apply           (U8): 0 don't apply the register we set  1 apply the register we set ,default don't apply.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL
 */
//-------------------------------------------------------------------------
static U8 RXInputBuf_DelayExchange(S8 iOfst)
{
    U8 u1Value;

    if(iOfst <0)
    {
        u1Value = 0x8 | (-iOfst);
    }
    else
    {
        u1Value = iOfst;
    }

    return u1Value;
}

// cannot be simulated in DV or DSim, it's analog feature.
DRAM_STATUS_T DramcRXInputBufferOffsetCal(DRAMC_CTX_T *p)
{
    U32 u4RegBackupAddress[] =
    {
        (DRAMC_REG_ADDR(DDRPHY_B0_DQ5)),
        (DRAMC_REG_ADDR(DDRPHY_B1_DQ5)),
        (DRAMC_REG_ADDR(DDRPHY_B0_DQ6)),
        (DRAMC_REG_ADDR(DDRPHY_B1_DQ6)),
        (DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ5)),
        (DRAMC_REG_ADDR(DDRPHY_SHU_B1_DQ5)),
        (DRAMC_REG_ADDR(DDRPHY_B0_DQ3)),
        (DRAMC_REG_ADDR(DDRPHY_B1_DQ3)),
        (DRAMC_REG_ADDR(DRAMC_REG_PADCTRL)),
    };
    S8 iOffset, iDQFlagChange[16], iDQMFlagChange[2];
    U32 u4Value, u4RestltDQ[2], u4RestltDQM[2];
    U8 u1BitIdx, u1ByteIdx, u1FinishCount, u1DQFinalFlagChange[16], u1DQMFinalFlagChange[2];

    mcSHOW_DBG_MSG(("\n[RXInputBufferOffsetCal]\n"));
    mcFPRINTF((fp_A60501, "\n[RXInputBufferOffsetCal] \n"));

    //Back up dramC register
    DramcBackupRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PADCTRL), 0xf, PADCTRL_FIXDQIEN);

    //Enable BIAS (RG_RX_*RDQ*_RES_BIAS_EN_B), RG_RX_*_BIAS_PS* =    2'b01(RG mode)
    //Select VREF, for LPDDR4 set RG_RX_*DQ_DDR4_SEL_* =1, RG_RX_*DQ_DDR3_SEL_* =0
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B0_DQ6),
                                        P_Fld(1, B0_DQ6_RG_RX_ARDQ_BIAS_PS_B0) |
                                        P_Fld(1, B0_DQ6_RG_RX_ARDQ_RES_BIAS_EN_B0) |
                                        P_Fld(0, B0_DQ6_RG_RX_ARDQ_DDR3_SEL_B0) |
                                        P_Fld(1, B0_DQ6_RG_RX_ARDQ_DDR4_SEL_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B1_DQ6),
                                        P_Fld(1, B1_DQ6_RG_RX_ARDQ_BIAS_PS_B1) |
                                        P_Fld(1, B1_DQ6_RG_RX_ARDQ_RES_BIAS_EN_B1)  |
                                        P_Fld(0, B1_DQ6_RG_TX_ARDQ_DDR3_SEL_B1) |
                                        P_Fld(1, B1_DQ6_RG_TX_ARDQ_DDR4_SEL_B1));

    //Enable VREF, (RG_RX_*DQ_VREF_EN_* =1)
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ5), 1, B0_DQ5_RG_RX_ARDQ_VREF_EN_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ5), 1, B1_DQ5_RG_RX_ARDQ_VREF_EN_B1);

    //Set Vref voltage: LP4 with termination  SEL[4:0] = 01110 (0xe) (refer to Vref default table)
    //Only need to set Vref with term, only K rx input offset at highest freq.
    //Need to set according to ODT on/off.
    if(p->odt_onoff==ODT_ON)
    {
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ5), 0xe, SHU_B0_DQ5_RG_RX_ARDQ_VREF_SEL_B0);  // LP4 and LP4x with term: 0xe
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B1_DQ5), 0xe, SHU_B1_DQ5_RG_RX_ARDQ_VREF_SEL_B1);  // LP4 and LP4x with term: 0xe
    }
    else
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ5), 0x16, SHU_B0_DQ5_RG_RX_ARDQ_VREF_SEL_B0);  // LP4 and LP4x with term: 0xe
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B1_DQ5), 0x16, SHU_B1_DQ5_RG_RX_ARDQ_VREF_SEL_B1);  // LP4 and LP4x with term: 0xe
    }

    // Wait 1us.
    mcDELAY_US(1);

    //Enable RX input buffer (RG_RX_*DQ_IN_BUFF_EN_* =1, DA_RX_*DQ_IN_GATE_EN_* =1)
    //Enable RX input buffer offset calibration (RG_RX_*DQ_OFFC_EN_*=1)
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PADCTRL), 0xf, PADCTRL_FIXDQIEN); // same as DA_RX_*DQ_IN_GATE_EN_*=1, Keep DQ input always ON. ==> Note: Do not enable again.
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B0_DQ3), P_Fld(1, B0_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B0) | P_Fld(1, B0_DQ3_RG_RX_ARDQ_OFFC_EN_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B1_DQ3), P_Fld(1, B1_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B1) | P_Fld(1, B1_DQ3_RG_RX_ARDQ_OFFC_EN_B1));

    // SW parameter initialization
    u1FinishCount =0;
    iDQMFlagChange[0] = 0x7f;
    iDQMFlagChange[1] = 0x7f;

    for(u1BitIdx=0; u1BitIdx< 16; u1BitIdx++)
    {
        iDQFlagChange[u1BitIdx] = 0x7f; //initial as invalid
    }

    //Sweep RX offset calibration code (RG_RX_*DQ*_OFFC<3:0>), the MSB is sign bit, sweep the code from -7(1111) to +7(0111)
    for(iOffset=-7; iOffset<7; iOffset++)
    {
        u4Value = RXInputBuf_DelayExchange(iOffset);

        #ifdef ETT_PRINT_FORMAT
        mcSHOW_DBG_MSG(("iOffset= %d, u4Value=%d,", iOffset, u4Value));
        #else
        mcSHOW_DBG_MSG(("iOffset= %2d, u4Value=%2d,", iOffset, u4Value));
        #endif
        mcFPRINTF((fp_A60501, "iOffset= %2d, u4Value=%2d,", iOffset, u4Value));

         //Delay of DQM0.
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ1), u4Value, B0_DQ1_RG_RX_ARDQM0_OFFC_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ1), u4Value, B1_DQ1_RG_RX_ARDQM0_OFFC_B1);

         //Delay of DQ0~DQ7.
        u4Value = u4Value |(u4Value<<4) |(u4Value<<8) |(u4Value<<12) |(u4Value<<16) |(u4Value<<20) |(u4Value<<24) |(u4Value<<28);
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B0_DQ0), u4Value);
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B1_DQ0), u4Value);

        //For each code sweep, wait 0.1us to check the flag.
        mcDELAY_US(1);

        //Check offset flag of DQ (RGS_*DQ*_OFFSET_FLAG_*), the value will be from 1(-7) to 0(+7). Record the value when the flag becomes "0".
        //Flag bit0 is for DQ0,  Flag bit15 for DQ15
        u4RestltDQ[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_PHY_RGS_DQ), MISC_PHY_RGS_DQ_RGS_ARDQ_OFFSET_FLAG_B0);
        u4RestltDQ[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_PHY_RGS_DQ), MISC_PHY_RGS_DQ_RGS_ARDQ_OFFSET_FLAG_B1);
        u4RestltDQ[0] |= (u4RestltDQ[1] <<8);

        mcSHOW_DBG_MSG(("RestltDQ (B1)0x%x (B0)0x%x, ", u4RestltDQ[1], u4RestltDQ[0]));
        mcFPRINTF((fp_A60501, "RestltDQ (B1)0x%x (B0)0x%x, ", u4RestltDQ[1], u4RestltDQ[0]));

        for(u1BitIdx= 0; u1BitIdx <16; u1BitIdx++)
        {
            if(iDQFlagChange[u1BitIdx] == 0x7f) //invalid
            {
                u4Value = (u4RestltDQ[0] >> u1BitIdx) & 0x1;

                if(u4Value ==0) // 1 -> 0
                {
                    iDQFlagChange[u1BitIdx] = iOffset;
                    u1FinishCount ++;
                }
            }
        }

        //Check offset flag of DQM (RGS_*DQ*_OFFSET_FLAG_*), the value will be from 1(-7) to 0(+7). Record the value when the flag becomes "0".
        u4RestltDQM[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_PHY_RGS_DQ), MISC_PHY_RGS_DQ_RGS_ARDQM0_OFFSET_FLAG_B0);
        u4RestltDQM[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_PHY_RGS_DQ), MISC_PHY_RGS_DQ_RGS_ARDQM0_OFFSET_FLAG_B1);

        mcSHOW_DBG_MSG(("RestltDQM (B1)= 0x%x, (B0)= 0x%x\n", u4RestltDQM[1], u4RestltDQM[0]));
        mcFPRINTF((fp_A60501, "RestltDQM (B1)= 0x%x, (B0)= 0x%x\n", u4RestltDQM[1], u4RestltDQM[0]));

        for(u1ByteIdx= 0; u1ByteIdx <2; u1ByteIdx++)
        {
            if(iDQMFlagChange[u1ByteIdx]== 0x7f) //invalid
            {
                if(u4RestltDQM[u1ByteIdx]==0)// 1 -> 0
                {
                    iDQMFlagChange[u1ByteIdx]= iOffset;
                    u1FinishCount++;
                }
            }
        }

        if(u1FinishCount==18) // (DQ8 bits, DQM 1bit, total 9 bits.) x2 bytes
        {
            break; //all bits done, early break
        }
    }

    mcSHOW_DBG_MSG(("\nResult DQ\n"
                    "\t0\t1\t2\t3\t4\t5\t6\t7\n"
                    "Byte 0\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n"
                    "Byte 1\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n"
                    "Result DQM [0] %d, [1] %d\n",
                        iDQFlagChange[0], iDQFlagChange[1], iDQFlagChange[2], iDQFlagChange[3],
                        iDQFlagChange[4], iDQFlagChange[5], iDQFlagChange[6], iDQFlagChange[7],
                        iDQFlagChange[8], iDQFlagChange[9], iDQFlagChange[10], iDQFlagChange[11],
                        iDQFlagChange[12], iDQFlagChange[13], iDQFlagChange[14],iDQFlagChange[15],
                        iDQMFlagChange[0], iDQMFlagChange[1]));

    mcFPRINTF((fp_A60501,"Result DQ [0]%d [1]%d [2]%d [3]%d [4]%d [5]%d [6]%d [7]%d\n", \
                                        iDQFlagChange[0], iDQFlagChange[1], iDQFlagChange[2], iDQFlagChange[3], \
                                        iDQFlagChange[4], iDQFlagChange[5], iDQFlagChange[6],iDQFlagChange[7]));

    mcFPRINTF((fp_A60501, "Result DQ Byte 1 [0]%d [1]%d [2]%d [3]%d [4]%d [5]%d [6]%d [7]%d\n", \
                                        iDQFlagChange[8], iDQFlagChange[9], iDQFlagChange[10], iDQFlagChange[11], \
                                        iDQFlagChange[12], iDQFlagChange[13], iDQFlagChange[14],iDQFlagChange[15]));

    mcFPRINTF((fp_A60501, "Result DQM[0] %d, Result DQM[1] %d,\n", iDQMFlagChange[0], iDQMFlagChange[1]));

    //Restore setting registers
    DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));

    // Change the offset value according to register format
    for(u1BitIdx= 0; u1BitIdx <16; u1BitIdx++)
    {
        u1DQFinalFlagChange[u1BitIdx] = RXInputBuf_DelayExchange(iDQFlagChange[u1BitIdx]);
    }
    u1DQMFinalFlagChange[0]= RXInputBuf_DelayExchange(iDQMFlagChange[0]);
    u1DQMFinalFlagChange[1]= RXInputBuf_DelayExchange(iDQMFlagChange[1]);

    //Apply the code recorded.
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ1), u1DQMFinalFlagChange[0], B0_DQ1_RG_RX_ARDQM0_OFFC_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ1), u1DQMFinalFlagChange[1], B1_DQ1_RG_RX_ARDQM0_OFFC_B1);

    u4Value = u1DQFinalFlagChange[0] |(u1DQFinalFlagChange[1]<<4) |(u1DQFinalFlagChange[2]<<8) |(u1DQFinalFlagChange[3]<<12) | \
                    (u1DQFinalFlagChange[4]<<16) |(u1DQFinalFlagChange[5]<<20) |(u1DQFinalFlagChange[6]<<24) |(u1DQFinalFlagChange[7]<<28);
    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B0_DQ0), u4Value);

    u4Value = u1DQFinalFlagChange[8] |(u1DQFinalFlagChange[9]<<4) |(u1DQFinalFlagChange[10]<<8) |(u1DQFinalFlagChange[11]<<12) | \
                    (u1DQFinalFlagChange[12]<<16) |(u1DQFinalFlagChange[13]<<20) |(u1DQFinalFlagChange[14]<<24) |(u1DQFinalFlagChange[15]<<28);
    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B1_DQ0), u4Value);

    // Disable RX input buffer offset calibration (RG_RX_*DQ_OFFC_EN_*=0)
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_TXDQ3+u4AddrOfst), 0, TXDQ3_RG_RX_ARDQ_OFFC_EN_B0);

    #if 0
    mcSHOW_DBG_MSG(("Register DQ [0]%d [1]%d [2]%d [3]%d [4]%d [5]%d [6]%d [7]%d\n", \
                                        u1DQFinalFlagChange[0], u1DQFinalFlagChange[1], u1DQFinalFlagChange[2], u1DQFinalFlagChange[3], \
                                        u1DQFinalFlagChange[4], u1DQFinalFlagChange[5], u1DQFinalFlagChange[6],u1DQFinalFlagChange[7]));
    mcFPRINTF((fp_A60501,"Register DQ [0]%d [1]%d [2]%d [3]%d [4]%d [5]%d [6]%d [7]%d\n", \
                                        u1DQFinalFlagChange[0], u1DQFinalFlagChange[1], u1DQFinalFlagChange[2], u1DQFinalFlagChange[3], \
                                        u1DQFinalFlagChange[4], u1DQFinalFlagChange[5], u1DQFinalFlagChange[6],u1DQFinalFlagChange[7]));
    mcSHOW_DBG_MSG(("Register DQM %d\n", u1DQMFinalFlagChange));
    mcFPRINTF((fp_A60501, "Register DQM %d\n", u1DQMFinalFlagChange));
    #endif
    mcSHOW_DBG_MSG3(("[DramcRXInputBufferOffsetCal] Done\n"));
    mcFPRINTF((fp_A60501, "[DramcRXInputBufferOffsetCal] Done\n"));

    return DRAM_OK;
}
#endif

//-------------------------------------------------------------------------
/** DramcSwImpedanceCal
 *  start TX OCD impedance calibration.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param  apply           (U8): 0 don't apply the register we set  1 apply the register we set ,default don't apply.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL
 */
//-------------------------------------------------------------------------
#define HYNIX_IMPX_ADJUST 0
#if HYNIX_IMPX_ADJUST
U32 ImpedanceAdjustment_Hynix(U32 u4OriValue, U8 u1Div)
{
    U32 u4RODT;
    U32 u4AdjustValue;

    if(u1Div >0)
    {
        u4RODT = 240/u1Div;
        u4AdjustValue = 60 *(u4OriValue+2)/u4RODT -2;
    }
    else
    {
        u4RODT =0;
        u4AdjustValue = u4OriValue;
    }

    mcSHOW_DBG_MSG(("ODTN Change by Tool, 240 div %d =%d, After adjustment ODTN=%d\n\n", u1Div, u4RODT, u4AdjustValue));

    return u4AdjustValue;
}
#endif

/* Impedance have a total of 19 steps, but the HW value mapping to hardware is 0~15, 29~31
* This function adjusts passed value u1ImpVal by adjust step count "u1AdjStepCnt"
* After adjustment, if value is 1. Too large (val > 31) -> set to max 31
*                               2. Too small (val < 0) -> set to min 0
*                               3. Value is between 15 & 29, adjust accordingly ( 15 < value < 29 )
* returns: Impedance value after adjustment
*/
static U32 SwImpedanceAdjust(U32 u4ImpVal, S8 s1StepCnt)
{
    S32 S4ImpedanceTemp = (S32)u4ImpVal;

   // Perform impedance value adjustment
    S4ImpedanceTemp += s1StepCnt;
    /* After adjustment, if value is 1. Too large (val > 31) -> set to max 31
     *                               2. Too small (val < 0) -> set to min 0
     *                               3. Value is between 15 & 29, adjust accordingly ( 15 < value < 29 )
     */
    if ((S4ImpedanceTemp > 15) && (S4ImpedanceTemp < 29)) //Value is between 15 & 29 ( 15 < value < 29)
    {
        S4ImpedanceTemp = S4ImpedanceTemp - 16 + 29;
    }

    if (S4ImpedanceTemp > 31) //Value after adjustment too large -> set to max 31
    {
        S4ImpedanceTemp = 31;
    }
    else if (S4ImpedanceTemp < 0) //Value after adjustment too small -> set to min 0
    {
        S4ImpedanceTemp = 0;
    }

    return (U32)S4ImpedanceTemp;
}

#if SIMULATION_SW_IMPED
void DramcSwImpedanceSaveRegister(DRAMC_CTX_T *p, U8 ca_freq_option, U8 dq_freq_option, U8 save_to_where)
{
    U8 backup_broadcast;
    U8 u1Hysteresis;

    backup_broadcast = GetDramcBroadcast();

    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    //DQ
    vIO32WriteFldMulti((DRAMC_REG_SHU_DRVING1 + save_to_where * SHU_GRP_DRAMC_OFFSET), P_Fld(gDramcSwImpedanceResult[dq_freq_option][DRVP], SHU_DRVING1_DQDRVP2) | P_Fld(gDramcSwImpedanceResult[dq_freq_option][DRVN], SHU_DRVING1_DQDRVN2));
    vIO32WriteFldMulti((DRAMC_REG_SHU_DRVING2 + save_to_where * SHU_GRP_DRAMC_OFFSET), P_Fld(gDramcSwImpedanceResult[dq_freq_option][DRVP], SHU_DRVING2_DQDRVP1) | P_Fld(gDramcSwImpedanceResult[dq_freq_option][DRVN], SHU_DRVING2_DQDRVN1));
    vIO32WriteFldMulti((DRAMC_REG_SHU_DRVING3 + save_to_where * SHU_GRP_DRAMC_OFFSET), P_Fld(gDramcSwImpedanceResult[dq_freq_option][ODTP], SHU_DRVING3_DQODTP2) | P_Fld(gDramcSwImpedanceResult[dq_freq_option][ODTN], SHU_DRVING3_DQODTN2));
    vIO32WriteFldMulti((DRAMC_REG_SHU_DRVING4 + save_to_where * SHU_GRP_DRAMC_OFFSET), P_Fld(gDramcSwImpedanceResult[dq_freq_option][ODTP], SHU_DRVING4_DQODTP1) | P_Fld(gDramcSwImpedanceResult[dq_freq_option][ODTN], SHU_DRVING4_DQODTN1));
    
    //DQS
    #if SUPPORT_HYNIX_RX_DQS_WEAK_PULL
    if (p->vendor_id == VENDOR_HYNIX)
    {  U32 temp_value[4];
        int i;
        for(i=0; i<4; i++)
        {
            temp_value[i] = SwImpedanceAdjust(gDramcSwImpedanceResult[dq_freq_option][i], 2);
        }
         vIO32WriteFldMulti((DRAMC_REG_SHU_DRVING1 + save_to_where * SHU_GRP_DRAMC_OFFSET), P_Fld(temp_value[0], SHU_DRVING1_DQSDRVP2) | P_Fld(temp_value[1], SHU_DRVING1_DQSDRVN2));
         vIO32WriteFldMulti((DRAMC_REG_SHU_DRVING1 + save_to_where * SHU_GRP_DRAMC_OFFSET), P_Fld(temp_value[0], SHU_DRVING1_DQSDRVP1) | P_Fld(temp_value[1], SHU_DRVING1_DQSDRVN1));
         vIO32WriteFldMulti((DRAMC_REG_SHU_DRVING3 + save_to_where * SHU_GRP_DRAMC_OFFSET), P_Fld(temp_value[2], SHU_DRVING3_DQSODTP2) | P_Fld(temp_value[3], SHU_DRVING3_DQSODTN2));
         vIO32WriteFldMulti((DRAMC_REG_SHU_DRVING3 + save_to_where * SHU_GRP_DRAMC_OFFSET), P_Fld(temp_value[2], SHU_DRVING3_DQSODTP) | P_Fld(temp_value[3], SHU_DRVING3_DQSODTN));
    }
    else
    #endif
    {
        vIO32WriteFldMulti((DRAMC_REG_SHU_DRVING1 + save_to_where * SHU_GRP_DRAMC_OFFSET), P_Fld(gDramcSwImpedanceResult[dq_freq_option][DRVP], SHU_DRVING1_DQSDRVP2) | P_Fld(gDramcSwImpedanceResult[dq_freq_option][DRVN], SHU_DRVING1_DQSDRVN2));
        vIO32WriteFldMulti((DRAMC_REG_SHU_DRVING1 + save_to_where * SHU_GRP_DRAMC_OFFSET), P_Fld(gDramcSwImpedanceResult[dq_freq_option][DRVP], SHU_DRVING1_DQSDRVP1) | P_Fld(gDramcSwImpedanceResult[dq_freq_option][DRVN], SHU_DRVING1_DQSDRVN1));
        vIO32WriteFldMulti((DRAMC_REG_SHU_DRVING3 + save_to_where * SHU_GRP_DRAMC_OFFSET), P_Fld(gDramcSwImpedanceResult[dq_freq_option][ODTP], SHU_DRVING3_DQSODTP2) | P_Fld(gDramcSwImpedanceResult[dq_freq_option][ODTN], SHU_DRVING3_DQSODTN2));
        vIO32WriteFldMulti((DRAMC_REG_SHU_DRVING3 + save_to_where * SHU_GRP_DRAMC_OFFSET), P_Fld(gDramcSwImpedanceResult[dq_freq_option][ODTP], SHU_DRVING3_DQSODTP) | P_Fld(gDramcSwImpedanceResult[dq_freq_option][ODTN], SHU_DRVING3_DQSODTN));
    }
    
    //CMD & CLK
    vIO32WriteFldMulti((DRAMC_REG_SHU_DRVING2 + save_to_where * SHU_GRP_DRAMC_OFFSET), P_Fld(gDramcSwImpedanceResult[ca_freq_option][DRVP], SHU_DRVING2_CMDDRVP2) | P_Fld(gDramcSwImpedanceResult[ca_freq_option][DRVN], SHU_DRVING2_CMDDRVN2));
    vIO32WriteFldMulti((DRAMC_REG_SHU_DRVING2 + save_to_where * SHU_GRP_DRAMC_OFFSET), P_Fld(gDramcSwImpedanceResult[ca_freq_option][DRVP], SHU_DRVING2_CMDDRVP1) | P_Fld(gDramcSwImpedanceResult[ca_freq_option][DRVN], SHU_DRVING2_CMDDRVN1));
    vIO32WriteFldMulti((DRAMC_REG_SHU_DRVING4 + save_to_where * SHU_GRP_DRAMC_OFFSET), P_Fld(gDramcSwImpedanceResult[ca_freq_option][ODTP], SHU_DRVING4_CMDODTP2) | P_Fld(gDramcSwImpedanceResult[ca_freq_option][ODTN], SHU_DRVING4_CMDODTN2));
    vIO32WriteFldMulti((DRAMC_REG_SHU_DRVING4 + save_to_where * SHU_GRP_DRAMC_OFFSET), P_Fld(gDramcSwImpedanceResult[ca_freq_option][ODTP], SHU_DRVING4_CMDODTP1) | P_Fld(gDramcSwImpedanceResult[ca_freq_option][ODTN], SHU_DRVING4_CMDODTN1));
    
    //RG_TX_*RCKE_DRVP/RG_TX_*RCKE_DRVN doesn't set, so set 0xA first 
    vIO32WriteFldAlign((DDRPHY_SHU_CA_CMD11 + save_to_where * SHU_GRP_DDRPHY_OFFSET), gDramcSwImpedanceResult[ca_freq_option][DRVP], SHU_CA_CMD11_RG_TX_ARCKE_DRVP);
    vIO32WriteFldAlign((DDRPHY_SHU_CA_CMD11 + save_to_where * SHU_GRP_DDRPHY_OFFSET), gDramcSwImpedanceResult[ca_freq_option][DRVN], SHU_CA_CMD11_RG_TX_ARCKE_DRVN);
    
    //CKE
    // CKE is full swing.
    // LP4/LP4X set DRVP/DRVN as LP3's default value
    // DRVP=8
    // DRVN=9
    //DRVP[4:0] = RG_TX_ARCMD_PU_PRE<1:0>, RG_TX_ARCLK_DRVN_PRE<2:0>
    // E2 settings confirm by DE: YY Hsu
    vIO32WriteFldAlign((DDRPHY_SHU_CA_CMD3 + save_to_where * SHU_GRP_DDRPHY_OFFSET), (8>>3)&0x3, SHU_CA_CMD3_RG_TX_ARCMD_PU_PRE); //Darren need confirm
    vIO32WriteFldAlign((DDRPHY_SHU_CA_CMD0 + save_to_where * SHU_GRP_DDRPHY_OFFSET), 8&0x7, SHU_CA_CMD0_RG_TX_ARCLK_DRVN_PRE); //Darren need confirm
    //DRVN[4:0] = RG_ARCMD_REV<12:8>
    #if (fcFOR_CHIP_ID == fcLafite)
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    vIO32WriteFldAlign_All((DDRPHY_SHU_CA_DLL2 + save_to_where * SHU_GRP_DDRPHY_OFFSET), 9, SHU_CA_DLL2_RG_TX_ARCKE_DRVN_B0);
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    #endif

    DramcBroadcastOnOff(backup_broadcast);
}

//-------------------------------------------------------------------------
/** vImpCalVrefSel
 *  Set IMP_VREF_SEL for DRVP, DRVN, Run-time/Tracking
 *  (Refer to "IMPCAL Settings" document register "RG_RIMP_VREF_SEL" settings)
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param  freq_region     (enum): pass freq_region (LOW_FREQ/HIGH_FREQ) for LP4X
 *  @param  u1ImpCalStage   (U8): During DRVP, DRVN, run-time/tracking stages
 *                                some vref_sel values are different
 */
//-------------------------------------------------------------------------
/* Definitions to make IMPCAL_VREF_SEL function more readable */
#define IMPCAL_STAGE_DRVP     0
#define IMPCAL_STAGE_DRVN     1
#define IMPCAL_STAGE_ODTP     2
#define IMPCAL_STAGE_ODTN     3
#define IMPCAL_STAGE_TRACKING 4

/* LP4X IMP_VREF_SEL w/o term ==== */
#define IMP_DRVP_LP4X_LOWFREQ_VREF_SEL   0x35
#define IMP_DRVN_LP4X_LOWFREQ_VREF_SEL   0x2a
#define IMP_ODTN_LP4X_LOWFREQ_VREF_SEL   0x35
#define IMP_TRACK_LP4X_LOWFREQ_VREF_SEL  0x35
#define IMP_DRVP_LP4X_HIGHFREQ_VREF_SEL   0x3a
#define IMP_DRVN_LP4X_HIGHFREQ_VREF_SEL   0x2a
#define IMP_ODTN_LP4X_HIGHFREQ_VREF_SEL   0x3a
#define IMP_TRACK_LP4X_HIGHFREQ_VREF_SEL  0x3a

/* Refer to "IMPCAL Settings" document register "RG_RIMP_VREF_SEL" settings */
static void vImpCalVrefSel(DRAMC_CTX_T *p, DRAMC_IMP_T efreq_region, U8 u1ImpCalStage)
{
    U8 u1RegTmpValue = 0;

    if (p->dram_type == TYPE_LPDDR4X)
    {
        if (u1ImpCalStage == IMPCAL_STAGE_DRVP) // DRVP
        {
            u1RegTmpValue = (efreq_region == LOW_FREQ) ? IMP_DRVP_LP4X_LOWFREQ_VREF_SEL : IMP_DRVP_LP4X_HIGHFREQ_VREF_SEL;
        }
        else if (u1ImpCalStage == IMPCAL_STAGE_DRVN) // DRVN
        {
            u1RegTmpValue = (efreq_region == LOW_FREQ) ? IMP_DRVN_LP4X_LOWFREQ_VREF_SEL : IMP_DRVN_LP4X_HIGHFREQ_VREF_SEL;
        }
        else if (u1ImpCalStage == IMPCAL_STAGE_ODTN) // ODTN
        {
            u1RegTmpValue = (efreq_region == LOW_FREQ) ? IMP_ODTN_LP4X_LOWFREQ_VREF_SEL : IMP_ODTN_LP4X_HIGHFREQ_VREF_SEL;
        }
        else // IMPCAL_STAGE_TRACKING (Tracking) for DRVP/ODTN tracking
        {
            u1RegTmpValue = (efreq_region == LOW_FREQ) ? IMP_TRACK_LP4X_LOWFREQ_VREF_SEL : IMP_TRACK_LP4X_HIGHFREQ_VREF_SEL;
        }
    }
    else
    {
        mcSHOW_ERR_MSG(("[vImpCalVrefSel] Warnning: Need confirm DRAM type for IMP_VREF_SEL !!!\n"));
        #if __ETT__
        while(1);
        #endif
    }

    // dbg msg after vref_sel selection
    mcSHOW_DBG_MSG3(("[vImpCalVrefSel] IMP_VREF_SEL 0x%x, IMPCAL stage:%u, freq_region:%u\n",
                      u1RegTmpValue, u1ImpCalStage, efreq_region));

    /* Set IMP_VREF_SEL register field's value */
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_CA_CMD11), u1RegTmpValue, SHU_CA_CMD11_RG_RIMP_VREF_SEL);

    return;
}

static U32 DramcSwImpCalResult(DRAMC_CTX_T *p, char *drvType, U32 u4Fld)
{
    U32 u4ImpxDrv=0, u4ImpCalResult=0;
    U32 u4CheckImpChange = (u4Fld == SHU_IMPCAL1_IMPDRVP) ? 1 : 0;
    
    for(u4ImpxDrv=0; u4ImpxDrv<32 ; u4ImpxDrv++)
    {
        if(u4ImpxDrv == 16) //0~15, 29~31
            u4ImpxDrv = 29;

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_IMPCAL1), u4ImpxDrv, u4Fld);
        mcDELAY_US(1);
        u4ImpCalResult = u4IO32ReadFldAlign((DDRPHY_MISC_PHY_RGS_CMD), MISC_PHY_RGS_CMD_RGS_RIMPCALOUT);
        mcSHOW_DBG_MSG2(("OCD %s=%d ,CALOUT=%d\n", drvType, u4ImpxDrv, u4ImpCalResult));
        mcFPRINTF((fp_A60501, "OCD %s=%d ,CALOUT=%d\n", drvType, u4ImpxDrv, u4ImpCalResult));

        if(u4ImpCalResult == u4CheckImpChange)//first found
        {
            mcSHOW_DBG_MSG2(("\nOCD %s calibration OK! %s=%d\n\n", drvType, drvType, u4ImpxDrv));
            mcFPRINTF((fp_A60501, "\nOCD %s calibration OK! %s=%d\n\n", drvType, drvType, u4ImpxDrv));
            break;
        }
    }

    if (u4ImpxDrv == 32) // Can't find SwImp drv results
    {
        u4ImpxDrv = 31;
        mcSHOW_DBG_MSG2(("\nOCD %s calibration FAIL! %s=%d\n\n", drvType, drvType, u4ImpxDrv));
        mcFPRINTF((fp_A60501, "\nOCD %s calibration FAIL! %s=%d\n\n", drvType, drvType, u4ImpxDrv));
    }

    return u4ImpxDrv;
}

DRAM_STATUS_T DramcSwImpedanceCal(DRAMC_CTX_T *p, U8 u1Para, DRAMC_IMP_T freq_region)
{
    U32 u4DRVP_Result =0xff,u4ODTN_Result =0xff, u4DRVN_Result =0xff;
    //U32 u4BaklReg_DDRPHY_MISC_IMP_CTRL0, u4BaklReg_DDRPHY_MISC_IMP_CTRL1;
    U32 u4BaklReg_DRAMC_REG_IMPCAL;
    U8 backup_channel;
    U8 backup_broadcast;
    U8 u1DrvType=0, u1CALI_ENP=0, u1CALI_ENN=0;
    U32 u4SwImpCalResult=0, u4DrvFld=0;
    char *drvStr="NULL";

    backup_broadcast = GetDramcBroadcast();
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);

    // Darren
    vIO32WriteFldMulti_All(DDRPHY_MISC_SPM_CTRL1, P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10) | P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_B0)
                                           | P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_B1) | P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_CA));

    vIO32WriteFldAlign_All(DDRPHY_MISC_SPM_CTRL2, 0x0, MISC_SPM_CTRL2_PHY_SPM_CTL2);
    vIO32WriteFldAlign_All(DDRPHY_MISC_SPM_CTRL0, 0x0, MISC_SPM_CTRL0_PHY_SPM_CTL0);

    //Disable IMP HW Tracking
    //Hw Imp tracking disable for all channels Because SwImpCal will be K again when resume from DDR reserved mode
    vIO32WriteFldAlign_All(DRAMC_REG_IMPCAL, 0, IMPCAL_IMPCAL_HW);

    backup_channel = p->channel;
    vSetPHY2ChannelMapping(p, CHANNEL_A);

    //Register backup
    //u4BaklReg_DDRPHY_MISC_IMP_CTRL0 = u4IO32Read4B((DDRPHY_MISC_IMP_CTRL0));
    //u4BaklReg_DDRPHY_MISC_IMP_CTRL1 = u4IO32Read4B((DDRPHY_MISC_IMP_CTRL1));
    u4BaklReg_DRAMC_REG_IMPCAL = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL));

    //RG_IMPCAL_VREF_SEL (now set in vImpCalVrefSel())
    //RG_IMPCAL_LP3_EN=0, RG_IMPCAL_LP4_EN=1
#if 0
    vIO32WriteFldMulti( (DDRPHY_MISC_IMP_CTRL1), P_Fld(1, MISC_IMP_CTRL1_RG_RIMP_BIAS_EN) | \
    					P_Fld(0, MISC_IMP_CTRL1_RG_RIMP_PRE_EN) | P_Fld(1, MISC_IMP_CTRL1_RG_RIMP_VREF_EN));
#else
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MISC_IMP_CTRL1), P_Fld(0, MISC_IMP_CTRL1_RG_RIMP_PRE_EN));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), P_Fld(0, IMPCAL_IMPCAL_CALI_ENN) | P_Fld(1, IMPCAL_IMPCAL_IMPPDP) | \
                        P_Fld(1, IMPCAL_IMPCAL_IMPPDN));    //RG_RIMP_BIAS_EN and RG_RIMP_VREF_EN move to IMPPDP and IMPPDN
#endif
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MISC_IMP_CTRL0), P_Fld(1, MISC_IMP_CTRL0_RG_IMP_EN) | \
                        P_Fld(0, MISC_IMP_CTRL0_RG_RIMP_DDR3_SEL) | P_Fld(1, MISC_IMP_CTRL0_RG_RIMP_DDR4_SEL));
    mcDELAY_US(1);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), 1, IMPCAL_IMPCAL_CALI_EN);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_IMPCAL1), P_Fld(0, SHU_IMPCAL1_IMPDRVN)|P_Fld(0, SHU_IMPCAL1_IMPDRVP));


    //LP4X: ODTN/DRVN/DRVP calibration start
    for (u1DrvType=0; u1DrvType<IMP_MAX; u1DrvType++) // Calibration sequence for DRVP, DRVN and ODTN
    {
        if (u1DrvType == ODTP) // no use, skip ODTP
            continue;

        /* Set IMP_VREF_SEL value for DRVP/DRVN and ODTN */
        vImpCalVrefSel(p, freq_region, u1DrvType);

        switch(u1DrvType)
        {
            case DRVP:
                drvStr = "DRVP";
                u1CALI_ENP = 0x1;
                u1CALI_ENN = 0x0;
                u4DrvFld = SHU_IMPCAL1_IMPDRVP;
                u4DRVP_Result = 0;               
                break;           
            case DRVN:
            case ODTN:
                drvStr = (u1DrvType == DRVN) ? "DRVN" : "ODTN";
                u1CALI_ENP = 0x0;
                u1CALI_ENN = (u1DrvType == DRVN) ? 0x0 : 0x1; // 0x1 change to ODTN path
                u4DrvFld = SHU_IMPCAL1_IMPDRVN;
                break;
            default:
                mcSHOW_ERR_MSG(("[DramcSwImpedanceCal] Warnning: Need confirm u1DrvType for SW IMP Calibration !!!\n"));
                break;
        }

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), u1CALI_ENP, IMPCAL_IMPCAL_CALI_ENP);  //PUCMP_EN move to CALI_ENP
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), u1CALI_ENN, IMPCAL_IMPCAL_CALI_ENN);  //ODT_EN move to CALI_ENN

        mcSHOW_DBG_MSG2(("\n\n\tK %s\n", drvStr));
        mcFPRINTF((fp_A60501, "\n\tK %s\n", drvStr));     
    
        //DRVP=DRVP_FINAL
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_IMPCAL1), u4DRVP_Result, SHU_IMPCAL1_IMPDRVP);  //PUCMP_EN move to CALI_ENP
    
        //LP4P_EN[1] (internal enable), DRV05=1 and LP3, original is 0x3 for LP4P
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_CA_CMD11), 0x1, SHU_CA_CMD11_DRV05);
    
        //If RGS_TX_OCD_IMPCALOUTX=1
        //RG_IMPX_DRVN++;
        //Else save RG_IMPX_DRVN value and assign to DRVN
        u4SwImpCalResult = DramcSwImpCalResult(p, drvStr, u4DrvFld);                  

        switch(u1DrvType)
        {
            case DRVP:
                u4DRVP_Result = u4SwImpCalResult;
                break;
            case DRVN:
                u4DRVN_Result = u4SwImpCalResult;
                break;
            case ODTN:
                u4ODTN_Result = u4SwImpCalResult;
                break;
            default:
                mcSHOW_ERR_MSG(("[DramcSwImpedanceCal] Warnning: Need confirm u4SwImpCalResult for SW IMP Calibration !!!\n"));
                break;
        }
    }
    //Register Restore
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), u4BaklReg_DRAMC_REG_IMPCAL);
    //vIO32Write4B((DDRPHY_MISC_IMP_CTRL0), u4BaklReg_DDRPHY_MISC_IMP_CTRL0);
    //vIO32Write4B((DDRPHY_MISC_IMP_CTRL1), u4BaklReg_DDRPHY_MISC_IMP_CTRL1);


/*** default value if K fail
    LP3:  DRVP=8, DRVN=9
    LP4:  DRVP=6, DRVN=9, ODTN=14
    LP4X(UT): DRVP=12, DRVN=9
    LP4X(T):  DRVP=5, DRVN=9, ODTN=14
    LP4P: DRVP=8, DRVN=10
***/
    mcSHOW_DBG_MSG(("[SwImpedanceCal] DRVP=%d, DRVN=%d, ODTN=%d\n", u4DRVP_Result, u4DRVN_Result, u4ODTN_Result));
    mcFPRINTF((fp_A60501, "[SwImpedanceCal] DRVP=%d, DRVN=%d, ODTN=%d\n", u4DRVP_Result, u4DRVN_Result, u4ODTN_Result));

    #if 0//HYNIX_IMPX_ADJUST
    if(u1Para)
    {
        u4ODTN_Result= ImpedanceAdjustment_Hynix(u4ODTN_Result, u1Para);
    }
    #endif

    gDramcSwImpedanceResult[freq_region][DRVP] = u4DRVP_Result;
    gDramcSwImpedanceResult[freq_region][DRVN] = u4DRVN_Result;
    gDramcSwImpedanceResult[freq_region][ODTP] = 0;
    gDramcSwImpedanceResult[freq_region][ODTN] = u4ODTN_Result;

    /* Set IMP_VREF_SEL value for TRACKING/RUN-TIME */
    vImpCalVrefSel(p, freq_region, IMPCAL_STAGE_TRACKING);

#if RUNTIME_SHMOO_RELEATED_FUNCTION && SUPPORT_SAVE_TIME_FOR_CALIBRATION
    {
        U8 u1drv;
        {
            for (u1drv=0; u1drv<4; u1drv++)
            {
                if(p->femmc_Ready==0)
                    p->pSavetimeData->u1SwImpedanceResule[freq_region][u1drv] = gDramcSwImpedanceResult[freq_region][u1drv];
                else
                    gDramcSwImpedanceResult[freq_region][u1drv] = p->pSavetimeData->u1SwImpedanceResule[freq_region][u1drv];
            }
        }
    }
#endif

    mcSHOW_DBG_MSG(("freq_region=%d, Reg: DRVP=%d, DRVN=%d, ODTN=%d\n", freq_region, gDramcSwImpedanceResult[freq_region][DRVP],
                                    gDramcSwImpedanceResult[freq_region][DRVN], gDramcSwImpedanceResult[freq_region][ODTN]));

#if APPLY_SIGNAL_WAVEFORM_SETTINGS_ADJUST
    if((p->dram_type == TYPE_LPDDR4X) && (freq_region ==0))
    {
        gDramcSwImpedanceResult[freq_region][DRVP] = SwImpedanceAdjust(gDramcSwImpedanceResult[freq_region][DRVP], gDramcSwImpedanceAdjust[freq_region][DRVP]);
        gDramcSwImpedanceResult[freq_region][DRVN] = SwImpedanceAdjust(gDramcSwImpedanceResult[freq_region][DRVN], gDramcSwImpedanceAdjust[freq_region][ODTN]);
    }
    else
    {
        gDramcSwImpedanceResult[freq_region][DRVP] = SwImpedanceAdjust(gDramcSwImpedanceResult[freq_region][DRVP], gDramcSwImpedanceAdjust[freq_region][DRVP]);
        gDramcSwImpedanceResult[freq_region][ODTN] = SwImpedanceAdjust(gDramcSwImpedanceResult[freq_region][ODTN], gDramcSwImpedanceAdjust[freq_region][ODTN]);
    }
    
    mcSHOW_DBG_MSG(("freq_region=%d, Reg: DRVP=%d, DRVN=%d, ODTN=%d (After Adjust)\n", freq_region, gDramcSwImpedanceResult[freq_region][DRVP],
                                        gDramcSwImpedanceResult[freq_region][DRVN], gDramcSwImpedanceResult[freq_region][ODTN]));
#endif

#if defined(SLT)
    //YingYu: only check for freq_region = 0 (un-term, DQ)
    if (gDramcSwImpedanceResult[freq_region][DRVP] >= 31 && (freq_region ==0) ) {
        mcSHOW_DBG_MSG(("SLT_BIN2\n"));
        while(1);
    }
#endif


    vSetCalibrationResult(p, DRAM_CALIBRATION_SW_IMPEDANCE, DRAM_OK);
    mcSHOW_DBG_MSG3(("[DramcSwImpedanceCal] Done\n\n"));
    mcFPRINTF((fp_A60501, "[DramcSwImpedanceCal] Done\n\n"));

    vSetPHY2ChannelMapping(p, backup_channel);
    DramcBroadcastOnOff(backup_broadcast);

    return DRAM_OK;
}
#endif //SIMULATION_SW_IMPED

#if 0 // E2 don't use
void DramcUpdateImpedanceTerm2UnTerm(DRAMC_CTX_T *p)
{
    gDramcSwImpedanceResult[ODT_OFF][ODTP] = gDramcSwImpedanceResult[ODT_ON][ODTP];
    gDramcSwImpedanceResult[ODT_OFF][ODTN] = gDramcSwImpedanceResult[ODT_ON][ODTN];
}
#endif

#ifdef ENABLE_HW_IMPCAL
DRAM_STATUS_T DramcHwImpedanceCal(DRAMC_CTX_T *p, U8 u1Para, U8 term_option)
{
    U32 u4ImpxDrv, u4ImpCalResult;
    U32 u4DRVP_Result =0xff,u4ODTN_Result =0xff, u4DRVN_Result =0xff;
    U32 u4BaklReg_DDRPHY_MISC_IMP_CTRL0, u4BaklReg_DDRPHY_MISC_IMP_CTRL1, u4BaklReg_DRAMC_REG_IMPCAL;
    U32 u4BaklReg_DRAMC_REG_REFCTRL0;
    U8 u1ByteIdx, u1RegTmpValue;
    U8 backup_channel;
    U8 backup_broadcast;

    backup_broadcast = GetDramcBroadcast();

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);

    // Darren
    vIO32WriteFldMulti_All(DDRPHY_MISC_SPM_CTRL1, P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10) | P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_B0)
                                           | P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_B1) | P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_CA));

    vIO32WriteFldAlign_All(DDRPHY_MISC_SPM_CTRL2, 0x0, MISC_SPM_CTRL2_PHY_SPM_CTL2);
    vIO32WriteFldAlign_All(DDRPHY_MISC_SPM_CTRL0, 0x0, MISC_SPM_CTRL0_PHY_SPM_CTL0);

    //Disable IMP HW Tracking
    vIO32WriteFldAlign_All(DRAMC_REG_IMPCAL, 0, IMPCAL_IMPCAL_HW);

    backup_channel = p->channel;
    vSetPHY2ChannelMapping(p, CHANNEL_A);

    //Register backup
    //u4BaklReg_DDRPHY_MISC_IMP_CTRL0 = u4IO32Read4B((DDRPHY_MISC_IMP_CTRL0));
    //u4BaklReg_DDRPHY_MISC_IMP_CTRL1 = u4IO32Read4B((DDRPHY_MISC_IMP_CTRL1));
    u4BaklReg_DRAMC_REG_IMPCAL = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL));
    u4BaklReg_DRAMC_REG_REFCTRL0 = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0));

    //enable autorefresh
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0), 0, REFCTRL0_REFDIS); //REFDIS=0, enable autorefresh

    //Basic setup
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA2), 0x7, SHU_SELPH_CA2_TXDLY_CMD);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_IMP_CTRL1), 0, MISC_IMP_CTRL1_RG_RIMP_PRE_EN);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), P_Fld(0, IMPCAL_IMPCAL_IMPPDP) |
                        P_Fld(0, IMPCAL_IMPCAL_IMPPDN) |
                        P_Fld(0, IMPCAL_IMPCAL_CALI_ENP) |
                        P_Fld(0, IMPCAL_IMPCAL_CALI_ENN) |
                        P_Fld(0, IMPCAL_IMPCAL_CALI_EN) |
                        //P_Fld(0, IMPCAL_IMPCAL_DRVUPOPT) |    // for Cann_on project
                        P_Fld(1, IMPCAL_IMPCAL_NEW_OLD_SL) |    //Impedance tracking form, 0: old, 1:new.
                        P_Fld(1, IMPCAL_IMPCAL_SWVALUE_EN));    //Impedance tracking DRVP, DRVN initial value from SW setting

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_IMPCAL1), P_Fld(0, SHU_IMPCAL1_IMPDRVP) |   //Set SW calibration value & HW initial calibration value for OCDP
                        P_Fld(3, SHU_IMPCAL1_IMPCALCNT) |  //hw impcal interval (unit: the number of refresh)
                        P_Fld(4, SHU_IMPCAL1_IMPCAL_CHKCYCLE)); //hw impcal calibration to check CMPOOUT

    /* Set IMP_VREF_SEL value for DRVP */
    vImpCalVrefSel(p, term_option, IMPCAL_STAGE_DRVP);

#if (fcFOR_CHIP_ID == fcLafite)
    if(p->dram_type == TYPE_LPDDR4)
    {
        u1RegTmpValue = 1;//LP4P_EN=0, DRV05=1
    }
    else  //TYPE_LPDDR4X || TYPE_LPDDR4P
    {
        u1RegTmpValue = 3;//LP4P_EN=1, DRV05=1
    }
#endif
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_CA_CMD11), u1RegTmpValue, SHU_CA_CMD11_DRV05);

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MISC_IMP_CTRL0), P_Fld(0, MISC_IMP_CTRL0_RG_IMP_EN) | \
                        P_Fld(0, MISC_IMP_CTRL0_RG_RIMP_DDR3_SEL) | \
                        P_Fld(1, MISC_IMP_CTRL0_RG_RIMP_DDR4_SEL));
    
    //Enable HW Imp tracking for calibration (SW impcal switch, 0: SW. 1:HW.)
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), 1, IMPCAL_IMPCAL_HW);

    U8 DQ_DRVP,DQ_ODTN;
    int u1Dly_cnt = 0;

    while (2000>(u1Dly_cnt++))
    {
        DQ_DRVP = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS2), IMPCAL_STATUS2_DRVPDQ_SAVE1);
        DQ_ODTN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS2), IMPCAL_STATUS2_DRVNDQ_SAVE1);

        if (DQ_DRVP!=0 && DQ_ODTN!=0)
        {
            u4DRVP_Result = DQ_DRVP;
            u4ODTN_Result = DQ_ODTN;
            break;
        }
        mcDELAY_US(1);
    }

    //Register Restore
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), u4BaklReg_DRAMC_REG_IMPCAL);
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0), u4BaklReg_DRAMC_REG_REFCTRL0);


/*** default value if K fail
    LP3:  DRVP=8, DRVN=9
    LP4:  DRVP=6, DRVN=9, ODTN=14
    LP4X(UT): DRVP=12, DRVN=9
    LP4X(T):  DRVP=5, DRVN=9, ODTN=14
    LP4P: DRVP=8, DRVN=10
***/
#if (fcFOR_CHIP_ID == fcLafite)
    u4DRVN_Result = 9; //fixed value from DE YingMin Liao
#endif

    if(u4DRVP_Result==0xff)
    {
        mcFPRINTF((fp_A60501, "\n[CHIP_SCAN]1. OCD DRVP calibration FAIL! \n\n"));
#if (fcFOR_CHIP_ID == fcLafite)
        u4DRVP_Result = 31;
#endif
    }
    if(u4ODTN_Result==0xff || u4DRVP_Result==0xff)
    {
        mcFPRINTF((fp_A60501, "\n[CHIP_SCAN]3. OCD ODTN calibration FAIL! \n\n"));
#if (fcFOR_CHIP_ID == fcLafite)
        u4ODTN_Result = 31;
#endif
    }

    mcSHOW_DBG_MSG(("[HwImpedanceCal] DRVP=%d, DRVN=%d, ODTN=%d\n", u4DRVP_Result, u4DRVN_Result, u4ODTN_Result));
    mcFPRINTF((fp_A60501, "[HwImpedanceCal] DRVP=%d, DRVN=%d, ODTN=%d\n", u4DRVP_Result, u4DRVN_Result, u4ODTN_Result));

    #if 0//HYNIX_IMPX_ADJUST
    if(u1Para)
    {
        u4ODTN_Result= ImpedanceAdjustment_Hynix(u4ODTN_Result, u1Para);
    }
    #endif

    if((p->dram_type == TYPE_LPDDR4X || p->dram_type == TYPE_LPDDR4P) && (term_option ==0))
    {
        gDramcSwImpedanceResult[term_option][DRVP] = u4DRVP_Result;
        gDramcSwImpedanceResult[term_option][DRVN] = u4ODTN_Result;    //Justin : LP4X unterm DRVN is ODTN * 2
        gDramcSwImpedanceResult[term_option][ODTP] = 0;
        gDramcSwImpedanceResult[term_option][ODTN] = 15;    //Justin : LP4X unterm, ODTN is useless
    }
    else
    {
        gDramcSwImpedanceResult[term_option][DRVP] = (u4DRVP_Result<=3) ? (u4DRVP_Result * 3) : u4DRVP_Result;
        gDramcSwImpedanceResult[term_option][DRVN] = (u4DRVN_Result<=3) ? (u4DRVN_Result * 3) : u4DRVN_Result;
        gDramcSwImpedanceResult[term_option][ODTP] = 0;
        gDramcSwImpedanceResult[term_option][ODTN] = (u4ODTN_Result<=3) ? (u4ODTN_Result * 3) : u4ODTN_Result;
    }

    /* Set IMP_VREF_SEL value for TRACKING/RUN-TIME */
    vImpCalVrefSel(p, term_option, IMPCAL_STAGE_TRACKING);
    
#if RUNTIME_SHMOO_RELEATED_FUNCTION && SUPPORT_SAVE_TIME_FOR_CALIBRATION
    {
        U8 u1drv;
        {
            for (u1drv=0; u1drv<4; u1drv++)
            {
                if(p->femmc_Ready==0)
                    p->pSavetimeData->u1SwImpedanceResule[term_option][u1drv] = gDramcSwImpedanceResult[term_option][u1drv];
                else
                    gDramcSwImpedanceResult[term_option][u1drv] = p->pSavetimeData->u1SwImpedanceResule[term_option][u1drv];
            }
        }
    }
#endif
    mcSHOW_DBG_MSG(("term_option=%d, Reg: DRVP=%d, DRVN=%d, ODTN=%d\n", term_option, gDramcSwImpedanceResult[term_option][0],
                                        gDramcSwImpedanceResult[term_option][1], gDramcSwImpedanceResult[term_option][ODTN]));
    
#if APPLY_SIGNAL_WAVEFORM_SETTINGS_ADJUST
        if((p->dram_type == TYPE_LPDDR4X || p->dram_type == TYPE_LPDDR4P) && (term_option ==0))
        {
            gDramcSwImpedanceResult[term_option][DRVP] = SwImpedanceAdjust(gDramcSwImpedanceResult[term_option][DRVP], gDramcSwImpedanceAdjust[term_option][DRVP]);
            gDramcSwImpedanceResult[term_option][DRVN] = SwImpedanceAdjust(gDramcSwImpedanceResult[term_option][DRVN], gDramcSwImpedanceAdjust[term_option][ODTN]);
        }
        else
        {
            gDramcSwImpedanceResult[term_option][DRVP] = SwImpedanceAdjust(gDramcSwImpedanceResult[term_option][DRVP], gDramcSwImpedanceAdjust[term_option][DRVP]);
            gDramcSwImpedanceResult[term_option][ODTN] = SwImpedanceAdjust(gDramcSwImpedanceResult[term_option][ODTN], gDramcSwImpedanceAdjust[term_option][ODTN]);
        }
        
        mcSHOW_DBG_MSG(("term_option=%d, Reg: DRVP=%d, DRVN=%d, ODTN=%d (After Adjust)\n", term_option, gDramcSwImpedanceResult[term_option][DRVP],
                                            gDramcSwImpedanceResult[term_option][DRVN], gDramcSwImpedanceResult[term_option][ODTN]));
#endif

#if defined(SLT)
        if (gDramcSwImpedanceResult[term_option][DRVP] >= 31 && (term_option ==1) ) {
            mcSHOW_DBG_MSG(("SLT_BIN2\n"));
            while(1);
        }
#endif

    vSetCalibrationResult(p, DRAM_CALIBRATION_SW_IMPEDANCE, DRAM_OK);
    mcSHOW_DBG_MSG3(("[DramcSwImpedanceCal] Done\n\n"));
    mcFPRINTF((fp_A60501, "[DramcSwImpedanceCal] Done\n\n"));

    vSetPHY2ChannelMapping(p, backup_channel);
    DramcBroadcastOnOff(backup_broadcast);

    return DRAM_OK;
}
#endif

#if 0
void Dram_Reset(DRAMC_CTX_T *p)
{
    // This function is implemented based on DE's bring up flow for DRAMC
    #if !SW_CHANGE_FOR_SIMULATION
    if(p->dram_type == TYPE_LPDDR3)
        DramcModeRegInit_LP3(p);
    else
        DramcModeRegInit_LP4(p);
    #endif

    ///TODO: update MR2(WL& RL) from AC timing table
}
#endif


void O1PathOnOff(DRAMC_CTX_T *p, U8 u1OnOff)
{
    U8 u1FixDQIEN = 0;

    u1FixDQIEN = (u1OnOff == ENABLE) ? 3 : 0;
    vIO32WriteFldAlign_All(DRAMC_REG_PADCTRL, u1FixDQIEN, PADCTRL_FIXDQIEN);

    vIO32WriteFldAlign_All(DDRPHY_B0_DQ5, u1OnOff, B0_DQ5_RG_RX_ARDQ_EYE_VREF_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DQ5, u1OnOff, B1_DQ5_RG_RX_ARDQ_EYE_VREF_EN_B1);
    vIO32WriteFldAlign_All(DDRPHY_B0_DQ3, u1OnOff, B0_DQ3_RG_RX_ARDQ_SMT_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DQ3, u1OnOff, B1_DQ3_RG_RX_ARDQ_SMT_EN_B1);

    mcDELAY_US(1);
}

#if SIMUILATION_LP4_CBT

void CATrainingPosCal(DRAMC_CTX_T *p, U8* pu1DieNum, S8* ps1CAMacroDelay, S8 *ps1CACenterDiff)
{
    U8 u1RankIdx, u1DieIdx, u1CAIdx, u1MacroIdx, u1MacroNum;
    S8 s1Intersect_min_byMacro[2], s1Intersect_max_byMacro[2];
    S8 s1Intersect_min_byBit[CATRAINING_NUM], s1Intersect_max_byBit[CATRAINING_NUM], s1CACenter_min[2];//, s1CACenter[CATRAINING_NUM];
    S8 s1TempFirstPass, s1TempLastPass;
    U8 u1PerBitDelayCellEnable =0, u1CABitNum=0;
    S8 s1CACenter[CATRAINING_NUM];

#if CA_PER_BIT_DELAY_CELL
    u1PerBitDelayCellEnable =1;
#endif
    u1CABitNum =6;

    mcSHOW_DBG_MSG(("\n[CATrainingPosCal] consider %d rank data\n", p->rank +1));
    
    for(u1MacroIdx =0; u1MacroIdx <2; u1MacroIdx++)
    {
        s1Intersect_min_byMacro[u1MacroIdx] = -127; // 127
        s1Intersect_max_byMacro[u1MacroIdx] = 127; //-127 
        s1CACenter_min[u1MacroIdx] = 0x7f;
    }
    
    for(u1CAIdx=0; u1CAIdx < u1CABitNum; u1CAIdx++)
    {   
        s1Intersect_min_byBit[u1CAIdx] = -127; // 127
        s1Intersect_max_byBit[u1CAIdx] = 127; //-127

        if (u1IsLP4Family(p->dram_type))
        {
            u1MacroIdx=0;
        }
        else //LP3
        {
            if(u1CAIdx==0 || u1CAIdx==1)
            {
                u1MacroIdx=0;
            }
            else
                u1MacroIdx=1;
        }
        
        for(u1RankIdx=RANK_0; u1RankIdx<(p->rank+1); u1RankIdx++)
        {
            for(u1DieIdx=0; u1DieIdx< pu1DieNum[u1RankIdx]; u1DieIdx++)
            {
                s1TempFirstPass = iFirstCAPass[u1RankIdx][u1DieIdx][u1CAIdx];
                s1TempLastPass = iLastCAPass[u1RankIdx][u1DieIdx][u1CAIdx];
                //mcSHOW_DBG_MSG(("RK%d, Die%d ,CA%d,iFirstCAPass = %d, iLastCAPass=%d \n",u1RankIdx, u1DieIdx, u1CAIdx, s1TempFirstPass, s1TempLastPass));

                // Intersection by Macro
                if(s1TempFirstPass > s1Intersect_min_byMacro[u1MacroIdx])
                    s1Intersect_min_byMacro[u1MacroIdx] = s1TempFirstPass;

                if(s1TempLastPass < s1Intersect_max_byMacro[u1MacroIdx])
                    s1Intersect_max_byMacro[u1MacroIdx] = s1TempLastPass;  

                // intersection by CA bit
                if(s1TempFirstPass > s1Intersect_min_byBit[u1CAIdx])
                    s1Intersect_min_byBit[u1CAIdx] = s1TempFirstPass;

                if(s1TempLastPass < s1Intersect_max_byBit[u1CAIdx])
                    s1Intersect_max_byBit[u1CAIdx] = s1TempLastPass;              
            }
        }

        s1CACenter[u1CAIdx] = (s1Intersect_min_byBit[u1CAIdx] +s1Intersect_max_byBit[u1CAIdx])/2;

        if(s1CACenter[u1CAIdx] < s1CACenter_min[u1MacroIdx])
            s1CACenter_min[u1MacroIdx] = s1CACenter[u1CAIdx];
    }

    // If CA perbit, choose min CA PI of all bits.
    // If CA perbyte, choose middle position of intersenction range of all bits.
    if(u1IsLP4Family(p->dram_type))
    {
        u1MacroNum=1;
    }
    else //LP3
    {
        u1MacroNum=2;
    }

    // CA perbit enable
    if(u1PerBitDelayCellEnable && (p->u2DelayCellTimex100 !=0))  
    {        
        for(u1MacroIdx =0; u1MacroIdx<u1MacroNum; u1MacroIdx++)
        {
            if(s1CACenter_min[u1MacroIdx] <0) //don't move clk
            {
                //mcSHOW_DBG_MSG(("warning : Macro%d minimum CA PI delay is %d(<0) and changed to 0\n", u1MacroIdx, s1CACenter_min[u1MacroIdx]));
                s1CACenter_min[u1MacroIdx] =0;
            }
            
            ps1CAMacroDelay[u1MacroIdx] = s1CACenter_min[u1MacroIdx];
        }
        
        mcSHOW_DBG_MSG(("u2DelayCellTimex100 = %d/100 ps\n", p->u2DelayCellTimex100));
        
        for(u1CAIdx=0; u1CAIdx < u1CABitNum; u1CAIdx++)
        {    
            if(u1IsLP4Family(p->dram_type))
            {
                u1MacroIdx=0;
            }
            else //LP3
            {
                if(u1CAIdx==0 || u1CAIdx==1)
                {
                    u1MacroIdx=0;
                }
                else
                    u1MacroIdx=1;
            }
      
            if(s1CACenter[u1CAIdx] <0) //don't move clk
            {
                s1CACenter[u1CAIdx] =0;
                ps1CACenterDiff[u1CAIdx]=0;
            }
            else
            {
                ps1CACenterDiff[u1CAIdx] = s1CACenter[u1CAIdx] - s1CACenter_min[u1MacroIdx];
            }

            mcSHOW_DBG_MSG(("CA%d delay=%d (%d~%d),", u1CAIdx, s1CACenter[u1CAIdx], s1Intersect_min_byBit[u1CAIdx], s1Intersect_max_byBit[u1CAIdx]));
            mcSHOW_DBG_MSG(("Diff = %d PI ", ps1CACenterDiff[u1CAIdx]));
            ps1CACenterDiff[u1CAIdx] = (ps1CACenterDiff[u1CAIdx]*100000000/(p->frequency<<6))/p->u2DelayCellTimex100; 
            mcSHOW_DBG_MSG(("(%d cell)", ps1CACenterDiff[u1CAIdx]));

            if(ps1CACenterDiff[u1CAIdx]>15)
            {
                mcSHOW_DBG_MSG(("[WARNING] CA%d delay cell %d >15, adjust to 15 cell", u1CAIdx, ps1CACenterDiff[u1CAIdx]));
                ps1CACenterDiff[u1CAIdx] =15;
            }
            
            mcSHOW_DBG_MSG(("\n"));
        }

        for(u1MacroIdx =0; u1MacroIdx < u1MacroNum; u1MacroIdx++)
        {
            mcSHOW_DBG_MSG(("\nCA PerBit enable=%d, Macro%d, CA PI delay=%d\n", u1PerBitDelayCellEnable, u1MacroIdx, ps1CAMacroDelay[u1MacroIdx]));
        }
    }
    else //CA perbyte
    {
        for(u1MacroIdx =0; u1MacroIdx < u1MacroNum; u1MacroIdx++)
        {
            ps1CAMacroDelay[u1MacroIdx] = (s1Intersect_min_byMacro[u1MacroIdx] +s1Intersect_max_byMacro[u1MacroIdx])/2;
            
            if(ps1CAMacroDelay[u1MacroIdx] <0)//don't move clk
            {
                //mcSHOW_DBG_MSG(("warning : CA PI delay is %d(<0) and changed to 0\n", ps1CAMacroDelay[u1MacroIdx]));
                ps1CAMacroDelay[u1MacroIdx] =0;
            }
            mcSHOW_DBG_MSG(("CA PerBit enable=%d, Macro%d, CA PI delay=%d (%d~%d)\n", u1PerBitDelayCellEnable, u1MacroIdx, ps1CAMacroDelay[u1MacroIdx], s1Intersect_min_byMacro[u1MacroIdx], s1Intersect_max_byMacro[u1MacroIdx]));
        }
    }
}

#if CA_PER_BIT_DELAY_CELL
extern const U8 uiLPDDR4_CA_Mapping_POP[CHANNEL_NUM][6];
#endif

void CATrainingSetPerBitDelayCell(DRAMC_CTX_T *p, S8 *ps1CACenterDiff)
{
    U8 uiCA;
    U8 *uiLPDDR_CA_Mapping = NULL;

    // Need to porting for each project according to LP3 pinmux table
    #if (fcFOR_CHIP_ID == fcLafite)
    #if CA_PER_BIT_DELAY_CELL
    uiLPDDR_CA_Mapping = (U8 *)uiLPDDR4_CA_Mapping_POP[p->channel];

    if(u1IsLP4Family(p->dram_type))
    {   
        for (uiCA=0; uiCA<CATRAINING_NUM_LP4; uiCA++)
        {
            //mcSHOW_DBG_MSG(("CA%d dly line = %d cells \n", uiLPDDR_CA_Mapping[uiCA], ps1CACenterDiff[uiCA]));
            #if EYESCAN_LOG
            gEyeScan_DelayCellPI[uiCA] = (S8) ps1CACenterDiff[uiCA];
            #endif
        }
        
        // Set CA perbit delay line calibration results
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_CA_CMD0),  P_Fld(ps1CACenterDiff[uiLPDDR_CA_Mapping[0]], SHU_R0_CA_CMD0_RK0_TX_ARCA0_DLY) | \
                                                                    P_Fld(ps1CACenterDiff[uiLPDDR_CA_Mapping[1]], SHU_R0_CA_CMD0_RK0_TX_ARCA1_DLY) | \
                                                                    P_Fld(ps1CACenterDiff[uiLPDDR_CA_Mapping[2]], SHU_R0_CA_CMD0_RK0_TX_ARCA2_DLY) | \
                                                                    P_Fld(ps1CACenterDiff[uiLPDDR_CA_Mapping[3]], SHU_R0_CA_CMD0_RK0_TX_ARCA3_DLY) | \
                                                                    P_Fld(ps1CACenterDiff[uiLPDDR_CA_Mapping[4]], SHU_R0_CA_CMD0_RK0_TX_ARCA4_DLY) | \
                                                                    P_Fld(ps1CACenterDiff[uiLPDDR_CA_Mapping[5]], SHU_R0_CA_CMD0_RK0_TX_ARCA5_DLY) );
    }
    #endif
    #endif
}
//-------------------------------------------------------------------------
/** CmdBusTrainingLP4
 *  start the calibrate the skew between (1) Clk pin and CAx pins. (2) Clk and CS pin (3)Vref(ca) driving
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL
 */
//-------------------------------------------------------------------------
static void insertionSort(S32 a[], S32 array_size)
{
     S32 i, j, index;
     for (i = 1; i < array_size; ++i)
     {
          index = a[i];
          for (j = i; j > 0 && a[j-1] > index; j--)
               a[j] = a[j-1];

          a[j] = index;
     }
}

void CBT_Switch_Freq(DRAMC_CTX_T *p, U8 freq)
{
    if(freq == CBT_LOW_FREQ)
    {
        DramcMRWriteFldAlign(p, 13, 0, MR13_FSP_OP, JUST_TO_GLOBAL_VALUE);
        DramcMRWriteFldAlign(p, 13, 0, MR13_FSP_WR, JUST_TO_GLOBAL_VALUE);
    }
    else // CBT_HIGH_FREQ
    {
        DramcMRWriteFldAlign(p, 13, 1, MR13_FSP_OP, JUST_TO_GLOBAL_VALUE);
        DramcMRWriteFldAlign(p, 13, 1, MR13_FSP_WR, JUST_TO_GLOBAL_VALUE);
    }
        
#if (FOR_DV_SIMULATION_USED == TRUE)
    return;
#endif

#if (fcFOR_CHIP_ID == fcLafite) 
    static U8 _CurFreq = CBT_UNKNOWN_FREQ;
    if (_CurFreq == freq)
    {
        return; // Do nothing no meter the frequency is.
    }
    _CurFreq = freq;
    
    if(freq == CBT_LOW_FREQ)
    {
        DramcDFSDirectJump_RGMode(p, DRAM_DFS_SHUFFLE_2); // Darren NOTE: Dramc shu1 for MRW (DramcModeRegInit_LP4 and CBT)
    }
    else
    {
        DramcDFSDirectJump_RGMode(p, DRAM_DFS_SHUFFLE_1);
    }
#else
    #error Need check of the DRAM_DFS_SHUFFLE_X for your chip !!!
#endif
    //DDRPhyFreqMeter();
}

static void vSetDramMRCBTOnOff(DRAMC_CTX_T *p, U8 u1OnOff, U8 operating_fsp)
{
    if(u1OnOff)
    {
        // op[7] = !(p->dram_fsp), dram will switch to another FSP_OP automatically
        if(operating_fsp)
        {
            MRWriteFldMulti(p, 13, P_Fld(0, MR13_FSP_OP) |
                                   P_Fld(1, MR13_FSP_WR) |
                                   P_Fld(1, MR13_CBT),
                                   TO_MR);
        }
        else
        {
            MRWriteFldMulti(p, 13, P_Fld(1, MR13_FSP_OP) |
                                   P_Fld(0, MR13_FSP_WR) |
                                   P_Fld(1, MR13_CBT),
                                   TO_MR);
        }

        if (p->dram_cbt_mode[p->rank] == CBT_BYTE_MODE1)
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WRITE_LEV), 1, WRITE_LEV_BYTEMODECBTEN);    //BYTEMODECBTEN=1
        }
    }
    else
    {
        if(operating_fsp)
        {   
            // !! Remain MR13_FSP_OP = 0, because of system is at low frequency now.
            MRWriteFldMulti(p, 13, P_Fld(1, MR13_FSP_WR) |
                                   P_Fld(0, MR13_CBT),
                                   TO_MR);
        }
        else
        {
            MRWriteFldMulti(p, 13, P_Fld(0, MR13_FSP_OP) |
                                   P_Fld(0, MR13_FSP_WR) |
                                   P_Fld(0, MR13_CBT),
                                   TO_MR);
        }
    }
    
}

#if ENABLE_CBT_DIV4_WA
void CmdBusTrainingDiv4ModeWA(DRAMC_CTX_T *p, DIV_MODE_T eMode, U8 u1OnOff)
{// Darren TODO: action item, remove it to vCalibration_Flow_LP4
    DIV_MODE_T eModeBackup = vGet_Div_Mode(p);
    U32 u4RegBackupAddress[] = 
    {       
        (DDRPHY_B0_DLL_ARPI2),
        (DDRPHY_B1_DLL_ARPI2),
        (DDRPHY_CA_DLL_ARPI2),
        (DDRPHY_B0_DLL_ARPI2 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_B1_DLL_ARPI2 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_CA_DLL_ARPI2 + SHIFT_TO_CHB_ADDR),
    };

    //Back up dramC register
    DramcBackupRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));

    //DLL
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI2, 0x0, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI2, 0x0, B0_DLL_ARPI2_RG_ARDLL_PHDET_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI2, 0x0, B1_DLL_ARPI2_RG_ARDLL_PHDET_EN_B1);
    mcDELAY_US(1);

    vIO32WriteFldMulti_All(DRAMC_REG_SHU_CONF0, P_Fld(u1OnOff, SHU_CONF0_DM64BITEN)
                | P_Fld(!u1OnOff, SHU_CONF0_FDIV2)
                | P_Fld(u1OnOff, SHU_CONF0_FREQDIV4)
                | P_Fld(u1OnOff, SHU_CONF0_DUALSCHEN)
                | P_Fld(!u1OnOff, SHU_CONF0_WDATRGO));
    mcDELAY_US(1);

    vSet_Div_Mode(p, eMode);
    DDRDllModeSetting(p);
    mcDELAY_US(1);

    //DLL
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI2, 0x1, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);
    mcDELAY_US(1);
    // The write all for 4 channels CA, but here you can write CHB CA only
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI2, 0x1, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);
    mcDELAY_US(1);
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI2, 0x1, B0_DLL_ARPI2_RG_ARDLL_PHDET_EN_B0);
    mcDELAY_US(1);
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI2, 0x1, B1_DLL_ARPI2_RG_ARDLL_PHDET_EN_B1);
    mcDELAY_US(1);

    //Restore setting registers
    DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));
    //Restore Div Mode
    vSet_Div_Mode(p, eModeBackup);
}
#endif

void CBTEntry(DRAMC_CTX_T *p, U8 operating_fsp, U16 operation_frequency)
{
    //Write (DRAMC_AO_BASE+ 0xE<<2) [25] = 1b0 // disable dramc DCMEN
    //Write (DRAMC_AO_BASE+ 0xE<<2) [30] = 1b0 // set R_DMPHYCLKDYNGEN=0
    //Write (DRAMC_AO_BASE+ 0x80<<2) [29] = 1b0 // set R_DMDQSIENCG_NORMAL_EN=0
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), P_Fld(0, DRAMC_PD_CTRL_PHYCLKDYNGEN)| P_Fld(0, DRAMC_PD_CTRL_DCMEN));


    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), 0, STBCAL_DQSIENCG_NORMAL_EN);

    //Step 0.0 CKE go high (Release R_DMCKEFIXOFF, R_DMCKEFIXON=1)
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 1, DRAMC_PD_CTRL_MIOCKCTRLOFF);   //MIOCKCTRLOFF=1

    CKEFixOnOff(p, p->rank, CKE_FIXON, CKE_WRITE_TO_ONE_CHANNEL);

    //Step 0: MRW MR13 OP[0]=1 to enable CBT
    vSetDramMRCBTOnOff(p, ENABLE, operating_fsp);

    //Step 0.1: before CKE low, Let DQS=0 by R_DMwrite_level_en=1, spec: DQS_t has to retain a low level during tDQSCKE period
    if (p->dram_cbt_mode[p->rank] == CBT_NORMAL_MODE)
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WRITE_LEV), 1, WRITE_LEV_WRITE_LEVEL_EN);
    }

    mcDELAY_US(1);

    //Step 1.0: let CKE go low
    CKEFixOnOff(p, p->rank, CKE_FIXOFF, CKE_WRITE_TO_ONE_CHANNEL);
    
    // Adjust u1MR13Value
    (operating_fsp == FSP_1)? DramcMRWriteFldAlign(p, 13, 1, MR13_FSP_OP, JUST_TO_GLOBAL_VALUE): DramcMRWriteFldAlign(p, 13, 0, MR13_FSP_OP, JUST_TO_GLOBAL_VALUE);

    // Step 1.1 : let IO to O1 path valid
    if (p->dram_cbt_mode[p->rank] == CBT_NORMAL_MODE)
    {
        // Let R_DMFIXDQIEN1=1 (byte1), 0xd8[13]  ==> Note: Do not enable again.
        //Currently set in O1PathOnOff
        //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PADCTRL), 0x3, PADCTRL_FIXDQIEN);

        // Let DDRPHY RG_RX_ARDQ_SMT_EN_B1=1 (byte1)
        //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ3), 1, B1_DQ3_RG_RX_ARDQ_SMT_EN_B1);
        O1PathOnOff(p, 1);
    }

    if (p->dram_cbt_mode[p->rank] == CBT_BYTE_MODE1)
    {
        // let IO to O1 path valid by DDRPHY RG_RX_ARDQ_SMT_EN_B0=1
        //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ3), 1, B0_DQ3_RG_RX_ARDQ_SMT_EN_B0);
        O1PathOnOff(p, 1);
    }

    // Wait tCAENT
    mcDELAY_US(1);
}

void CBTExit(DRAMC_CTX_T *p, U8 operating_fsp, U8 operation_frequency)
{
    U8 u1MROP;

    if (p->dram_cbt_mode[p->rank] == CBT_NORMAL_MODE || p->dram_cbt_mode[p->rank] == CBT_BYTE_MODE1)
    {
        //Step 1: CKE go high (Release R_DMCKEFIXOFF, R_DMCKEFIXON=1)
        CKEFixOnOff(p, p->rank, CKE_FIXON, CKE_WRITE_TO_ONE_CHANNEL);

        //Step 2:wait tCATX, wait tFC
        mcDELAY_US(1);

        //Step 3: MRW to command bus training exit (MR13 OP[0]=0 to disable CBT)
        vSetDramMRCBTOnOff(p, DISABLE, operating_fsp);
    }

    //Step 4:
    //Disable O1 path output
    if (p->dram_cbt_mode[p->rank] == CBT_NORMAL_MODE)
    {
        //Let DDRPHY RG_RX_ARDQ_SMT_EN_B1=0
        //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ3), 0, B1_DQ3_RG_RX_ARDQ_SMT_EN_B1);
        O1PathOnOff(p, 0);

        //Let FIXDQIEN1=0 ==> Note: Do not enable again.
        //Moved into O1PathOnOff
        //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PADCTRL), 0, PADCTRL_FIXDQIEN);
    }

    if (p->dram_cbt_mode[p->rank] == CBT_BYTE_MODE1)
    {
        //Let DDRPHY RG_RX_ARDQ_SMT_EN_B0=0
        //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ3), 0, B0_DQ3_RG_RX_ARDQ_SMT_EN_B0);
        O1PathOnOff(p, 0);

        //Disable Byte mode CBT enable bit
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WRITE_LEV), 0, WRITE_LEV_BYTEMODECBTEN);    //BYTEMODECBTEN=0
    }

    // Wait tCAENT
    mcDELAY_US(1);    
}

static void CBTSetVref(DRAMC_CTX_T *p, U32 u2VrefLevel, U8 operating_fsp, U8 stateFlag)
{
    U32 u4DbgValue;
    U8 u1VrefValue_pinmux;

    mcFPRINTF((fp_A60501, "\nCBTSetVref = 0x%x\n", u2VrefLevel));

    if ((p->dram_cbt_mode[p->rank] == CBT_NORMAL_MODE) && 
        (stateFlag == IN_CBT))
    {
        u1VrefValue_pinmux = GetCBTVrefPinMuxValue(p, u2VrefLevel);
        
#if !REDUCE_LOG_FOR_PRELOADER
    mcSHOW_DBG_MSG(("\nCH_%d, RK_%d, Range=%d, VrefValue_pinmux = 0x%x\n",p->channel, p->rank, gCBT_VREF_RANGE_SEL, u1VrefValue_pinmux));
#endif
        u1MR12Value[p->channel][p->rank][operating_fsp] = ((gCBT_VREF_RANGE_SEL & 0x1) << 6) | u2VrefLevel;

        //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WRITE_LEV), ((gCBT_VREF_RANGE_SEL&0x1) <<6) | (u2VrefLevel & 0x3f), WRITE_LEV_DMVREFCA);  //MR12, bit[25:20]=OP[5:0]  bit 26=OP[6]
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WRITE_LEV), u1VrefValue_pinmux, WRITE_LEV_DMVREFCA);  //MR12, bit[25:20]=OP[5:0]  bit 26=OP[6]

         //DQS_SEL=1, DQS_B1_G=1, Toggle R_DMDQS_WLEV (1 to 0)
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WRITE_LEV), 1 , WRITE_LEV_DQS_SEL);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WRITE_LEV), 0xa , WRITE_LEV_DQSBX_G);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WRITE_LEV), 1 , WRITE_LEV_DQS_WLEV);
        mcDELAY_US(1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WRITE_LEV), 0, WRITE_LEV_DQS_WLEV);

    }
    else
    {
        if (operating_fsp == FSP_1)
        {
            DramcMRWriteFldAlign(p, 13, 1, MR13_FSP_WR, TO_MR);
        }
            
        u4DbgValue = (((gCBT_VREF_RANGE_SEL & 0x1) << 6) | (u2VrefLevel & 0x3f));
        u1MR12Value[p->channel][p->rank][operating_fsp] = u4DbgValue;
        mcSHOW_DBG_MSG3(("u4DbgValue = 0x%x\n", u4DbgValue));

        DramcModeRegWriteByRank(p, p->rank, 12, u4DbgValue);
    }

    //wait tVREF_LONG
    mcDELAY_US(1);
}


static U32 CBTDelayCACLKCompare(DRAMC_CTX_T *p, S32 iDelay)
{
    U32 u4Result=0, u4Result0=0, u4Ready;
    U32 u4TimeCnt;

    u4TimeCnt = TIME_OUT_CNT;

    if(iDelay < 0)
    {   //Set CLK delay
          vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_CA_CMD9), P_Fld(0, SHU_R0_CA_CMD9_RG_RK0_ARPI_CMD) |
                                                                     P_Fld(-iDelay, SHU_R0_CA_CMD9_RG_RK0_ARPI_CLK) |
                                                                     P_Fld(-iDelay, SHU_R0_CA_CMD9_RG_RK0_ARPI_CS));
    }
    else
    {   //Set CA output delay
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_CA_CMD9), P_Fld(iDelay, SHU_R0_CA_CMD9_RG_RK0_ARPI_CMD) |
                                                                   P_Fld(0, SHU_R0_CA_CMD9_RG_RK0_ARPI_CLK) |
                                                                   P_Fld(0, SHU_R0_CA_CMD9_RG_RK0_ARPI_CS));

    }
    //Let R_DMTESTCATRAIN=1 to enable HW CAPAT Generator
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CATRAINING1), 1, CATRAINING1_TESTCATRAIN);

    //Check CA training compare ready (dramc_conf_nao 0x3fc , CATRAIN_CMP_CPT)
    do
    {
        u4Ready = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TCMDO1LAT), TCMDO1LAT_CATRAIN_CMP_CPT);
        u4TimeCnt --;
        mcDELAY_US(1);
    }while((u4Ready==0) &&(u4TimeCnt>0));

    if(u4TimeCnt==0)//time out
    {
        mcSHOW_DBG_MSG(("[CBTDelayCACLKCompare] Resp fail (time out)\n"));
        mcFPRINTF((fp_A60501, "[CBTDelayCACLKCompare] Resp fail (time out)\n"));
        //return DRAM_FAIL;
    }

    //Get CA training compare result (dramc_conf_nao 0x3fc , CATRAIN_CMP_ERR)
        if (p->dram_cbt_mode[p->rank] == CBT_NORMAL_MODE)
        {
            u4Result = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TCMDO1LAT), TCMDO1LAT_CATRAIN_CMP_ERR);
        }
        else
        {
            u4Result0 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TCMDO1LAT), TCMDO1LAT_CATRAIN_CMP_ERR0);
//            mcSHOW_DBG_MSG(("[Francis] TCMDO1LAT_CATRAIN_CMP_ERR0=0x%x\n", u4Result0));
            u4Result = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TCMDO1LAT), TCMDO1LAT_CATRAIN_CMP_ERR);
//            mcSHOW_DBG_MSG(("[Francis] TCMDO1LAT_CATRAIN_CMP_ERR=0x%x\n", u4Result));
        }

    //Let R_DMTESTCATRAIN=0 to disable HW CAPAT Generator
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CATRAINING1), 0, CATRAINING1_TESTCATRAIN);

    return (u4Result|u4Result0); // return pattern compre result
}


static U32 CBTDelayCSCompare(DRAMC_CTX_T *p, U32 uiDelay)
{
    U8 *uiLPDDR_O1_Mapping = NULL;
    U32 u4Result, u4Ready;
    U32 u4TimeCnt;
    U32 u4dq_o1;
    U32 u4byte_index;

    u4TimeCnt = TIME_OUT_CNT;

    uiLPDDR_O1_Mapping = (U8 *)uiLPDDR4_O1_Mapping_POP[p->channel];

    //Set CS output delay
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_CA_CMD9), uiDelay, SHU_R0_CA_CMD9_RG_RK0_ARPI_CS);

    //Step 5: toggle CS/CA for CS training by R_DMTCMDEN (wait dramc_nao tcmd_response=1, disable R_DMTCMDEN), 0x1e4[5]
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1, SPCMD_TCMDEN);
    do
    {
        u4Ready = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_TCMD_RESPONSE);
        u4TimeCnt --;
        mcDELAY_US(1);
    }while((u4Ready==0) &&(u4TimeCnt>0));


    if(u4TimeCnt==0)//time out
    {
        mcSHOW_DBG_MSG(("[CBTDelayCSCompare] Resp fail (time out)\n"));
        mcFPRINTF((fp_A60501, "[CBTDelayCSCompare] Resp fail (time out)\n"));
        //return DRAM_FAIL;
    }

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_TCMDEN);

    //Step 6: check CS training result on DQ[13:8] by O1, DDRPHYCFG 0xF80
    //Expected CA value is h2a (CA pulse width is 6UI, CS pulse is 1UI)
    u4dq_o1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_DQO1), MISC_DQO1_DQO1_RO);
    u4Result = 0;
    for(u4byte_index=8; u4byte_index<=13; u4byte_index++)
    {
        u4Result |= (((u4dq_o1 & (1<<uiLPDDR_O1_Mapping[u4byte_index])) >> (uiLPDDR_O1_Mapping[u4byte_index])) << (u4byte_index-8));
    }
    mcSHOW_DBG_MSG3(("CS Dly = %d, Result=0x%x\n", uiDelay, u4Result));

    return u4Result; // return pattern compre result
}

#if 0  //for CBT CS test
static U32 CBTDelayCSCompare2(DRAMC_CTX_T *p)
{
    U32 u4err_value, uiDelay;

    for (uiDelay=0; uiDelay<=MAX_CS_PI_DELAY; uiDelay++)
    {
        // Set CS output delay
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_ARPI_CMD), uiDelay, ARPI_CMD_DA_ARPI_CS);
        u4err_value= TestEngineCompare(p);
        mcSHOW_DBG_MSG(("CBTDelayCSCompare2= %3d, u4err_value=0x%2x\n", uiDelay, u4err_value));
    }

    return DRAM_OK; // return pattern compre result
}
#endif

#if 0
void DramcCmdBusTrainingPostProcess(DRAMC_CTX_T *p)
{
    S32 iCSFinalClkDelay, iCSFinalCmdDelay, iCSFinalCSDelay;
    U8 backup_rank, irank;

    // CBT Rank0/1 must set Clk/CA/CS the same from Wei-Jen

    mcSHOW_DBG_MSG(("[DramcCmdBusTrainingPostProcess] p->frequency=%d\n", p->frequency));

    backup_rank = u1GetRank(p);

    iCSFinalClkDelay= (CATrain_ClkDelay[p->channel][RANK_0] + CATrain_ClkDelay[p->channel][RANK_1])/2;
    CATrain_ClkDelay[p->channel][RANK_0] = iCSFinalClkDelay;
    CATrain_ClkDelay[p->channel][RANK_1] = iCSFinalClkDelay;

    iCSFinalCmdDelay= (CATrain_CmdDelay[p->channel][RANK_0] + CATrain_CmdDelay[p->channel][RANK_1])/2;
    CATrain_CmdDelay[p->channel][RANK_0] = iCSFinalCmdDelay;
    CATrain_CmdDelay[p->channel][RANK_1] = iCSFinalCmdDelay;

    iCSFinalCSDelay= (CATrain_CsDelay[p->channel][RANK_0] + CATrain_CsDelay[p->channel][RANK_1])/2;
    CATrain_CsDelay[p->channel][RANK_0] = iCSFinalCSDelay;
    CATrain_CsDelay[p->channel][RANK_1] = iCSFinalCSDelay;

    for(irank=RANK_0; irank<RANK_MAX; irank++)
    {
        vSetRank(p, irank);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_CA_CMD9), P_Fld(iCSFinalClkDelay, SHU_R0_CA_CMD9_RG_RK0_ARPI_CLK) |
                                                                    P_Fld(iCSFinalCmdDelay, SHU_R0_CA_CMD9_RG_RK0_ARPI_CMD) |
                                                                    P_Fld(iCSFinalCSDelay, SHU_R0_CA_CMD9_RG_RK0_ARPI_CS));
    }

    mcSHOW_DBG_MSG(("Clk Dly = %d\nCmd Dly = %d\nCS Dly = %d\n", iCSFinalClkDelay, iCSFinalCmdDelay, iCSFinalCSDelay));


#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    if(p->femmc_Ready==0)
    {
        p->pSavetimeData->u1CBTClkDelay_Save[p->channel][RANK_0]= iCSFinalClkDelay;
        p->pSavetimeData->u1CBTClkDelay_Save[p->channel][RANK_1]= iCSFinalClkDelay;

        p->pSavetimeData->u1CBTCmdDelay_Save[p->channel][RANK_0]= iCSFinalCmdDelay;
        p->pSavetimeData->u1CBTCmdDelay_Save[p->channel][RANK_1]= iCSFinalCmdDelay;

        p->pSavetimeData->u1CBTCsDelay_Save[p->channel][RANK_0]= iCSFinalCSDelay;
        p->pSavetimeData->u1CBTCsDelay_Save[p->channel][RANK_1]= iCSFinalCSDelay;
    }
#endif

    vSetRank(p, backup_rank);

}
#endif

static void CBTAdjustCS(DRAMC_CTX_T *p)
{
    S32 iFirstCSPass=0, iLastCSPass=0, iCSFinalDelay;//iCSCenter
    U32 uiDelay, u4ValueReadBack, u4CSWinSize;
    U8 backup_rank, rank_i;

    backup_rank = u1GetRank(p);

#if (SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_CBT)
	if(p->femmc_Ready==1)
  	{
        iCSFinalDelay=p->pSavetimeData->u1CBTCsDelay_Save[p->channel][p->rank];
	}
	else
#endif
    {
        iFirstCSPass = -1;
        iLastCSPass = -1;

        for (uiDelay=0; uiDelay<=MAX_CS_PI_DELAY; uiDelay++)
        {
            u4ValueReadBack = CBTDelayCSCompare(p, uiDelay);

            if(iFirstCSPass == -1)
            {
                if(u4ValueReadBack== 0x2a)  // compare pass
                {
                    iFirstCSPass = uiDelay;
                }
            }
            else if(iLastCSPass == -1)
            {
                if(u4ValueReadBack != 0x2a)  // compare fail
                {
                    iLastCSPass = uiDelay-1;
                }
                else if (uiDelay ==MAX_CS_PI_DELAY)
                {
                    iLastCSPass = uiDelay;
                }
            }

            // Wait time before output CS pattern to DDR again.. (Review this if need to save time)
            mcDELAY_US(1);
        }

        u4CSWinSize = iLastCSPass - iFirstCSPass + (iLastCSPass==iFirstCSPass?0:1);

        if(u4CSWinSize > ((MAX_CS_PI_DELAY+1)>>1))  // if winSize >32, CS delay= winSize -32.
        {
            iCSFinalDelay = u4CSWinSize -((MAX_CS_PI_DELAY+1)>>1);
        }
        else  ///TODO: need to delay CLK?  A60817 and A60501 cannot move CLK PI due to multi_phase problem.
        {
            iCSFinalDelay =0;
        }

        CATrain_CsDelay[p->channel][p->rank] = iCSFinalDelay;
        mcSHOW_DBG_MSG(("\nCS Dly= %d (%d-%d-32)\n", iCSFinalDelay, iLastCSPass, iFirstCSPass));
        mcFPRINTF((fp_A60501, "\nCS Dly= %d (%d-%d-32)\n", iCSFinalDelay, iLastCSPass, iFirstCSPass));
        
        // if dual rank, use average position of both rank
        if(backup_rank == RANK_1)
        {
            iCSFinalDelay = (CATrain_CsDelay[p->channel][RANK_0] + CATrain_CsDelay[p->channel][RANK_1])/2;
        }        
    }
        //Set CS output delay after training
        /* p->rank = RANK_0, save to Reg Rank0 and Rank1, p->rank = RANK_1, save to Reg Rank1 */
    for(rank_i=RANK_0; rank_i<backup_rank+1; rank_i++)
    {
        vSetRank(p, rank_i);

        #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(p->femmc_Ready==0)
        {
            p->pSavetimeData->u1CBTCsDelay_Save[p->channel][p->rank]= iCSFinalDelay;
        }
        #endif
        
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_CA_CMD9), iCSFinalDelay, SHU_R0_CA_CMD9_RG_RK0_ARPI_CS);
        CATrain_CsDelay[p->channel][rank_i]= iCSFinalDelay;
        //mcSHOW_DBG_MSG(("\n[CBTAdjustCS] RK%d ,CS Dly: %d\n",rank_i, iCSFinalDelay));
    }
    vSetRank(p, backup_rank);
}

static void CBTSetCACLKResult(DRAMC_CTX_T *p, S8 s1FinalCACLK, S8 *ps1CACenterDiff)
{
    U8 backup_rank, rank_i, uiCA;

    backup_rank = u1GetRank(p);

    /* p->rank = RANK_0, save to Reg Rank0 and Rank1, p->rank = RANK_1, save to Reg Rank1 */
    for(rank_i=RANK_0; rank_i<backup_rank+1; rank_i++)
    {
        vSetRank(p, rank_i);
        
        #if (SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_CBT)
        if(p->femmc_Ready==1)
        {
            CATrain_ClkDelay[p->channel][p->rank]=p->pSavetimeData->u1CBTClkDelay_Save[p->channel][p->rank];
            CATrain_CmdDelay[p->channel][p->rank]=p->pSavetimeData->u1CBTCmdDelay_Save[p->channel][p->rank];

            #if CA_PER_BIT_DELAY_CELL
            for (uiCA = 0; uiCA < CATRAINING_NUM_LP4; uiCA++)
            {
                ps1CACenterDiff[uiCA] = p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[p->channel][p->rank][uiCA];
            }
            #endif

            vSetCalibrationResult(p, DRAM_CALIBRATION_CA_TRAIN, DRAM_OK); // set default result OK, udpate status when per bit fail
        }
        else
        #endif
        {
            CATrain_ClkDelay[p->channel][p->rank] = 0;
            
            if(s1FinalCACLK<0)
            {
                CATrain_CmdDelay[p->channel][p->rank] = 0;
            }
            else
            {
                CATrain_CmdDelay[p->channel][p->rank] = s1FinalCACLK;
            }
        }

        //mcSHOW_DBG_MSG(("[CBTSetCACLKResult]Rank%d, Clk dly= %d, CA dly= %d\n", rank_i,  CATrain_ClkDelay[p->channel][p->rank], CATrain_CmdDelay[p->channel][p->rank]));
        //mcFPRINTF((fp_A60501, "[CBTSetCACLKResult]Rank%d, Clk dly= %d, CA dly= %d\n", rank_i,  CATrain_ClkDelay[p->channel][p->rank], CATrain_CmdDelay[p->channel][p->rank]));

        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_CA_CMD9), P_Fld(CATrain_CmdDelay[p->channel][p->rank], SHU_R0_CA_CMD9_RG_RK0_ARPI_CMD) |
                                                                    P_Fld(CATrain_ClkDelay[p->channel][p->rank], SHU_R0_CA_CMD9_RG_RK0_ARPI_CLK));
        
        #if CA_PER_BIT_DELAY_CELL
        CATrainingSetPerBitDelayCell(p, ps1CACenterDiff);
        #endif

        
        #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(p->femmc_Ready==0)
        {
            p->pSavetimeData->u1CBTClkDelay_Save[p->channel][p->rank]= CATrain_ClkDelay[p->channel][p->rank];
            p->pSavetimeData->u1CBTCmdDelay_Save[p->channel][p->rank]= CATrain_CmdDelay[p->channel][p->rank];
            
            #if CA_PER_BIT_DELAY_CELL
            for (uiCA = 0; uiCA < CATRAINING_NUM_LP4; uiCA++)
            {
                p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[p->channel][p->rank][uiCA] = ps1CACenterDiff[uiCA];
            }
            
#endif
        }
        #endif
    }
    
    vSetRank(p, backup_rank);
    
#if EYESCAN_LOG
    gEyeScan_CaliDelay[0] = CATrain_CmdDelay[p->channel][p->rank] + MAX_CLK_PI_DELAY;
#endif
}


U8 GetCBTVrefPinMuxValue(DRAMC_CTX_T *p, U8 u1VrefLevel)
{
    U8 u2VrefBit, u2Vref_new, u2Vref_org;

    if (p->dram_cbt_mode[p->rank] == CBT_BYTE_MODE1) 
        return ((gCBT_VREF_RANGE_SEL&0x1) << 6) | (u1VrefLevel & 0x3f);

    u2Vref_new = 0;
    u2Vref_org = ((gCBT_VREF_RANGE_SEL&0x1) << 6) | (u1VrefLevel & 0x3f);
    for (u2VrefBit = 0; u2VrefBit < 8; u2VrefBit++)
    {
//        mcSHOW_DBG_MSG(("=== u2VrefBit: %d, %d\n",u2VrefBit,uiLPDDR4_O1_Mapping_POP[p->channel][u2VrefBit]));
        if (u2Vref_org & (1 << u2VrefBit))
        {
            u2Vref_new |=  (1 << uiLPDDR4_O1_Mapping_POP[p->channel][u2VrefBit]);
//            mcSHOW_DBG_MSG(("=== u2VrefBit: %d, %d, u2Vref_org: %x, u2Vref_new: 0x%x\n",u2VrefBit,uiLPDDR4_O1_Mapping_POP[p->channel][u2VrefBit],u2Vref_org,u2Vref_new));
        }
    }

    mcSHOW_DBG_MSG3(("=== u2Vref_new: 0x%x --> 0x%x\n",u2Vref_org,u2Vref_new));

    return u2Vref_new;
}

U8 GetCBTVrefPinMuxRevertValue(DRAMC_CTX_T *p, U8 u1VrefLevel)
{
    U8 u2VrefBit, u2Vref_new, u2Vref_org;

    if (p->dram_cbt_mode[p->rank] == CBT_BYTE_MODE1) 
        return u1VrefLevel;

    u2Vref_new = 0;
    u2Vref_org = u1VrefLevel;
    for (u2VrefBit = 0; u2VrefBit < 8; u2VrefBit++)
    {
        u2Vref_new |= ((u2Vref_org>>uiLPDDR4_O1_Mapping_POP[p->channel][u2VrefBit])&1) << u2VrefBit;
    }

    mcSHOW_DBG_MSG3(("=== Revert u2Vref_new: 0x%x --> 0x%x\n",u2Vref_org,u2Vref_new));

    return u2Vref_new;
}

#if PINMUX_AUTO_TEST_PER_BIT_CA
void CheckCAPinMux(DRAMC_CTX_T *p)
{
    U8 u1CAIndex[CATRAINING_NUM_LP4] = {0};
    U8 u1CAdelay[CATRAINING_NUM_LP4] = {0};
    int i = 0;
    int j = 0;
    U8 u1Min = 0xff;
    U8 u1MinIdx = 0;
    U8 u1RankIdx = 0;

    for(i = 0; i<CATRAINING_NUM_LP4; i++)
    {
        memset(u1CAdelay, 0, sizeof(u1CAdelay));
        u1CAdelay[i] = 0xf;

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), p->rank, RKCFG_TXRANK);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), 1, RKCFG_TXRANKFIX);

        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_CA_CMD0), P_Fld(u1CAdelay[5], SHU_R0_CA_CMD0_RK0_TX_ARCA5_DLY) 
                | P_Fld(u1CAdelay[4], SHU_R0_CA_CMD0_RK0_TX_ARCA4_DLY) 
                | P_Fld(u1CAdelay[3], SHU_R0_CA_CMD0_RK0_TX_ARCA3_DLY) 
                | P_Fld(u1CAdelay[2], SHU_R0_CA_CMD0_RK0_TX_ARCA2_DLY) 
                | P_Fld(u1CAdelay[1], SHU_R0_CA_CMD0_RK0_TX_ARCA1_DLY) 
                | P_Fld(u1CAdelay[0], SHU_R0_CA_CMD0_RK0_TX_ARCA0_DLY));  
        CmdBusTrainingLP4(p);

        u1Min = 0xff;
        for(j = 0; j<CATRAINING_NUM_LP4; j++)
        {   
            if(gFinalCBTCA[p->channel][p->rank][j] <= u1Min)
            {
                u1Min = gFinalCBTCA[p->channel][p->rank][j]; 
                u1MinIdx = j;
            }
        }
        u1CAIndex[i] = u1MinIdx;
    }
    for(i = 0; i<CATRAINING_NUM_LP4; i++)
    {
        mcSHOW_DBG_MSG(("CH[%d] Rank[%d] APHY_CA[%d]-->DRAM(->MRR->DRAMC)[%d]\n", p->channel, p->rank, i, u1CAIndex[i]));
        if(u1CAIndex[i] != uiLPDDR4_CA_Mapping_POP[p->channel][i])
        {
            mcSHOW_DBG_MSG(("!Not mapping with CA mapping table\n"));
            while(1);        
        }
    }
    memset(u1CAdelay, 0, sizeof(u1CAdelay));
    vIO32WriteFldMulti_All(DDRPHY_SHU_R0_CA_CMD0, P_Fld(u1CAdelay[5], SHU_R0_CA_CMD0_RK0_TX_ARCA5_DLY)    
                | P_Fld(u1CAdelay[4], SHU_R0_CA_CMD0_RK0_TX_ARCA4_DLY)
                | P_Fld(u1CAdelay[3], SHU_R0_CA_CMD0_RK0_TX_ARCA3_DLY)
                | P_Fld(u1CAdelay[2], SHU_R0_CA_CMD0_RK0_TX_ARCA2_DLY)
                | P_Fld(u1CAdelay[1], SHU_R0_CA_CMD0_RK0_TX_ARCA1_DLY)
                | P_Fld(u1CAdelay[0], SHU_R0_CA_CMD0_RK0_TX_ARCA0_DLY));
    return;
}

void CheckCADelayCell(DRAMC_CTX_T *p)
{
    DRAM_CHANNEL_T channel_bak = p->channel;
    DRAM_RANK_T rank_bak = p->rank; 

    vSetPHY2ChannelMapping(p, CHANNEL_A);
    vSetRank(p, RANK_0);
    mcSHOW_DBG_MSG(("CA_delay_cell CHA R0[%X] CHB R0[%X] CHA R1[%X] CHB R1[%X]\n", 
    u4IO32Read4B(DDRPHY_SHU_R0_CA_CMD0), 
    u4IO32Read4B(DDRPHY_SHU_R0_CA_CMD0 + SHIFT_TO_CHB_ADDR),
    u4IO32Read4B(DDRPHY_SHU_R0_CA_CMD0 + 0x100), 
    u4IO32Read4B(DDRPHY_SHU_R0_CA_CMD0 + SHIFT_TO_CHB_ADDR + 0x100)));
    vSetPHY2ChannelMapping(p, channel_bak);
    vSetRank(p, rank_bak);

    return;
}
#endif

DRAM_STATUS_T CmdBusTrainingLP4(DRAMC_CTX_T *p)
{
    U8 u1VrefLevel, uiFinalVref;
    U32 u1vrefidx, u1BitIdx, ii;
    U32 u4CompareResult;
    PASS_WIN_DATA_T FinalWinPerCA[CATRAINING_NUM_LP4]={{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}};
    U32 uiCA, uiFinishCount, uiTemp;
    S16 iDelay;
    
    S32 iFirstPass_temp[CATRAINING_NUM_LP4], iLastPass_temp[CATRAINING_NUM_LP4];
    U32 uiCAWinSum, uiCAWinSumMax;
    S32 iCACenter[CATRAINING_NUM_LP4], iCACenterSum;//, iCAFinalCenter[CATRAINING_NUM_LP4] = {0};
    U8 operating_fsp;
    U16 operation_frequency;
    U8 irange, irange_start, irange_end;
    U8 uiFinalRange=0;
    S32 iCK_MIN = 1000, iCA_PerBit_DelayLine[CATRAINING_NUM_LP4] = {0};
    U8 u1EnableDelayCell; 
    U8 u1CBTEyeScanEnable;
    
    S8 iFinalCACLK;
    S8 s1CACenterDiff[CATRAINING_NUM]={0}; //for CA_PER_BIT
    
#if CA_PER_BIT_DELAY_CELL    
    U32 u1DelayCellOfst[CATRAINING_NUM_LP4];
#endif
#if ENABLE_EYESCAN_GRAPH
    U8 EyeScan_index[CATRAINING_NUM_LP4];
#endif
    U32 u4RegBackupAddress[] =
    {
        (DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL)),
        (DRAMC_REG_ADDR(DRAMC_REG_STBCAL)),
        (DRAMC_REG_ADDR(DRAMC_REG_CKECTRL)),
        (DRAMC_REG_ADDR(DRAMC_REG_WRITE_LEV)),
        (DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0)),
        (DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL)),
    };

#if MRW_CHECK_ONLY
    mcSHOW_MRW_MSG(("\n==[MR Dump] %s==\n", __func__));
#endif

    u1CBTEyeScanEnable = (gCBT_EYE_Scan_flag==1 && ((gCBT_EYE_Scan_only_higheset_freq_flag==1 && p->frequency == u2DFSGetHighestFreq(p)) || gCBT_EYE_Scan_only_higheset_freq_flag==0));

#if ENABLE_EYESCAN_GRAPH
    if(u1IsLP4Family(p->dram_type))
    {
        for(u1vrefidx=0; u1vrefidx<VREF_TOTAL_NUM_WITH_RANGE;u1vrefidx++)
        {
            for(uiCA = 0; uiCA < CATRAINING_NUM_LP4; uiCA++)
            {
                for(ii=0; ii<EYESCAN_BROKEN_NUM; ii++)
                {
                    gEyeScan_Min[u1vrefidx][uiCA][ii] = EYESCAN_DATA_INVALID;
                    gEyeScan_Max[u1vrefidx][uiCA][ii] = EYESCAN_DATA_INVALID;
                }
            }
        }
    }
#endif

    //Back up dramC register
    DramcBackupRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));

    vAutoRefreshSwitch(p, DISABLE); //When doing CA training, should make sure that auto refresh is disable

    //tx_rank_sel is selected by SW //Lewis@20180509: tx_rank_sel is selected by SW in CBT if TMRRI design has changed.
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), p->rank, RKCFG_TXRANK); 
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), 1, RKCFG_TXRANKFIX); //TXRANKFIX should be write after TXRANK or the rank will be fix at rank 1

    //SW variable initialization
    uiCAWinSumMax = 0;
    uiFinalVref = u1MR12Value[p->channel][p->rank][p->dram_fsp] & 0x3f;
    iFinalCACLK = 0;
    operating_fsp = p->dram_fsp;
    operation_frequency = p->frequency;
    #if CA_PER_BIT_DELAY_CELL
    CATrainingSetPerBitDelayCell(p, s1CACenterDiff);
    #endif


    //Note : Assume that there is a default CS value that can apply for CA.
    if (p->dram_cbt_mode[p->rank] == CBT_NORMAL_MODE)
    {
        (p->dram_fsp == FSP_1)? CBT_Switch_Freq(p, CBT_LOW_FREQ): NULL;
        (p->dram_fsp == FSP_1)? DramcModeRegInit_CATerm(p, 1): NULL;
        CBTEntry(p, operating_fsp, operation_frequency);  
        (p->dram_fsp == FSP_1)? CBT_Switch_Freq(p, CBT_HIGH_FREQ): NULL;
    }

#if PINMUX_AUTO_TEST_PER_BIT_CA
    CheckCADelayCell(p);
#endif

#if VENDER_JV_LOG
    vPrintCalibrationBasicInfo_ForJV(p);
#else
    vPrintCalibrationBasicInfo(p);
#endif

    //Step 3: set CBT range, verify range and setp
#if (SW_CHANGE_FOR_SIMULATION || FOR_DV_SIMULATION_USED)
    gCBT_VREF_RANGE_SEL = 0; //MR12,OP[6]
    irange_start=irange_end=0;
    gCBT_VREF_RANGE_BEGIN = 0;
    gCBT_VREF_RANGE_END = 2; //binary 110010
    gCBT_VREF_RANGE_STEP = 2;
#else
    #if FSP1_CLKCA_TERM
    if(p->dram_fsp == FSP_0)//un-term
    {
        gCBT_VREF_RANGE_SEL = 1; //MR12,OP[6]
        irange_start=irange_end=1;    
    }
    else//term
    {
        gCBT_VREF_RANGE_SEL = 0; //MR12,OP[6]
        irange_start=irange_end=0;
    }
    #else//un-term
    gCBT_VREF_RANGE_SEL = 1; //MR12,OP[6]
    irange_start=irange_end=1;
    #endif
    gCBT_VREF_RANGE_STEP = 2;

    if(Get_Vref_Calibration_OnOff(p) == DISABLE)
    {
        gCBT_VREF_RANGE_BEGIN = (u1MR12Value[p->channel][p->rank][p->dram_fsp] & 0x3f);
        gCBT_VREF_RANGE_END = gCBT_VREF_RANGE_BEGIN;
    }
    else
    {
        if (p->dram_type == TYPE_LPDDR4)
        {
            //range 1
            gCBT_VREF_RANGE_BEGIN = 13 - 5; // 300/1100(VDDQ) = 27.2%
            gCBT_VREF_RANGE_END = 13 + 5;
        }
        else
        {
            //range 1
            #if FSP1_CLKCA_TERM
            if(p->dram_fsp == FSP_0)//un-term
            {
                gCBT_VREF_RANGE_BEGIN = 27 - 5; // 290/600(VDDQ)=48.3%
                gCBT_VREF_RANGE_END = 27 + 5;    
            }
            else//term
            {
                gCBT_VREF_RANGE_BEGIN = 25 - 5;  // 180/600 = 30%
                gCBT_VREF_RANGE_END = 25 + 5;
            }
            #else//un-term
            gCBT_VREF_RANGE_BEGIN = 27 - 5; // 290/600(VDDQ)=48.3%
            gCBT_VREF_RANGE_END = 27 + 5;
            #endif
        }
    }
#endif

    if (u1CBTEyeScanEnable)
    {
        irange_start = 0;
        irange_end = 1;
    }

    for(irange=irange_start; irange<=irange_end; irange++)
    {
        if (u1CBTEyeScanEnable)
        {
            gCBT_VREF_RANGE_SEL = irange;
            gCBT_VREF_RANGE_BEGIN = 0;
            gCBT_VREF_RANGE_END = 50;
            gCBT_VREF_RANGE_STEP = EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV;
            if (gCBT_VREF_RANGE_SEL == 1)
            {
                gCBT_VREF_RANGE_BEGIN = 21+(EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV-1);
            }
        }

#if (SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_VREF_CAL)
        if(p->femmc_Ready==1)
        {
            mcSHOW_DBG_MSG(("\n[FAST_K] BYPASS_VREF_CAL CBT\n"));
        }
        else
#endif
        {
            for(u1VrefLevel = gCBT_VREF_RANGE_BEGIN; u1VrefLevel<=gCBT_VREF_RANGE_END; u1VrefLevel+=gCBT_VREF_RANGE_STEP)
            {
#if ENABLE_EYESCAN_GRAPH
                for (uiCA = 0; uiCA < CATRAINING_NUM_LP4; uiCA++)
                {
                    gEyeScan_DelayCellPI[uiCA] = 0;
                    EyeScan_index[uiCA] = 0;
                }
#endif
     
                // BYTE MODE: Set Vref & CBTEntry
                if (p->dram_cbt_mode[p->rank] == CBT_BYTE_MODE1)
                {
                    (p->dram_fsp == FSP_1)? CBT_Switch_Freq(p, CBT_LOW_FREQ): NULL;
                    (p->dram_fsp == FSP_1)? DramcModeRegInit_CATerm(p, 1): NULL;
                    CBTSetVref(p, u1VrefLevel, operating_fsp, IN_CBT);                 
                    CBTEntry(p, operating_fsp, operation_frequency);
                    (p->dram_fsp == FSP_1)? CBT_Switch_Freq(p, CBT_HIGH_FREQ): NULL;
                }
                else // NORMAL MODE: Just Set Vref
                {
                    CBTSetVref(p, u1VrefLevel, operating_fsp, IN_CBT);
                }
				#if CALIBRATION_SPEED_UP_DEBUG
                mcSHOW_DBG_MSG(("\n\tYulia CBT VrefRange %d, VrefLevel=%d\n", gCBT_VREF_RANGE_SEL, u1VrefLevel));
                #endif

#if VENDER_JV_LOG
                mcSHOW_DBG_MSG5(("\n\tLP4 CBT VrefRange %d, VrefLevel=%d\n", gCBT_VREF_RANGE_SEL, u1VrefLevel));
#endif
                
                // Delay CA output delay to do CA training in order to get the pass window.
                // moving CA relative to CK and repeating until CA is centered on the latching edge of CK
                // Note  !!!!!!!!!!!!!!!!!!!!!!!
                // Assume : Leave clk as the init value and adjust CA delay only can find out each CA window including of the left boundary.
                // If NOT, we may need to off-line adjust 0x404 SELPH2_TXDLY_CMD

                // SW variable initialization
                uiFinishCount = 0;
                uiCAWinSum = 0;
                iCACenterSum = 0;
                for (uiCA = 0; uiCA<CATRAINING_NUM_LP4; uiCA++)
                {
                     iLastPass_temp[uiCA] = PASS_RANGE_NA;
                     iFirstPass_temp[uiCA] = PASS_RANGE_NA;
                     iCA_PerBit_DelayLine[uiCA] = 0;
                     iCACenter[uiCA] = 0;
                }

#if CBT_MOVE_CA_INSTEAD_OF_CLK
                for (iDelay=0; iDelay<=MAX_CA_PI_DELAY; iDelay++)
#else
                for (iDelay=(-MAX_CLK_PI_DELAY); iDelay<=MAX_CA_PI_DELAY; iDelay++)
#endif
                {
                    u4CompareResult = CBTDelayCACLKCompare(p, iDelay);

                    //wait 1us between each CA pattern
                    mcDELAY_US(1);

                    mcSHOW_DBG_MSG3(("CBTDelayCACLK Delay= %d, CompareResult 0x%x\n", iDelay, u4CompareResult));
                    mcFPRINTF((fp_A60501, "CBTDelayCACLK Delay = %d, CompareResult 0x%x\n", iDelay, u4CompareResult));

                    for (uiCA = 0; uiCA<CATRAINING_NUM_LP4; uiCA++)
                    {
                        if((iFirstPass_temp[uiCA] != PASS_RANGE_NA) && (iLastPass_temp[uiCA] != PASS_RANGE_NA))
                        {
                            continue;
                        }

                        uiTemp = (u4CompareResult >>uiCA) & 0x1; //Get Each bit of CA result

                        if(iFirstPass_temp[uiCA] == PASS_RANGE_NA)
                        {
                            if(uiTemp==0) //compare correct: pass
                            {
                                iFirstPass_temp[uiCA] = iDelay;
                            }
                        }
                        else if(iLastPass_temp[uiCA] == PASS_RANGE_NA)
                        {
                            if(uiTemp==1) //compare error : fail
                            {
                                if ((iDelay-iFirstPass_temp[uiCA]) < 5) //prevent glitch
                                {
                                    iFirstPass_temp[uiCA] = PASS_RANGE_NA;
                                    continue;
                                }

                                iLastPass_temp[uiCA] = (iDelay-1);
                            }
                            else if (iDelay == MAX_CA_PI_DELAY)
                            {
                                iLastPass_temp[uiCA] = iDelay;
                            }

                            if(iLastPass_temp[uiCA] != PASS_RANGE_NA)
                            {
                                uiFinishCount++;
                                uiCAWinSum += (iLastPass_temp[uiCA] - iFirstPass_temp[uiCA]);  //Sum of CA Windows for vref selection
                                //iCACenter[uiCA] = (iLastPass_temp[uiCA] + iFirstPass_temp[uiCA])>>1;     //window center of each CA bit
                                //iCACenterSum += iCACenter[uiCA];

#if !REDUCE_LOG_FOR_PRELOADER
                                mcSHOW_DBG_MSG(("\n[CA %d] Center %d (%d~%d)\n", uiCA, iCACenter[uiCA] , iFirstPass_temp[uiCA], iLastPass_temp[uiCA] ));
#endif
#if ENABLE_EYESCAN_GRAPH
                                if (EyeScan_index[uiCA] < EYESCAN_BROKEN_NUM)
                                {
                                    gEyeScan_Min[(u1VrefLevel+irange*30)/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][uiCA][EyeScan_index[uiCA]] = iFirstPass_temp[uiCA]+MAX_CLK_PI_DELAY-CBT_MOVE_CA_INSTEAD_OF_CLK*32;
                                    gEyeScan_Max[(u1VrefLevel+irange*30)/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][uiCA][EyeScan_index[uiCA]] = iLastPass_temp[uiCA]+MAX_CLK_PI_DELAY-CBT_MOVE_CA_INSTEAD_OF_CLK*32;
                                    mcSHOW_DBG_MSG3(("u2VrefLevel=%d, u2VrefRange=%d, %d, uiCA=%d, index=%d (%d, %d)==\n",u1VrefLevel, irange, u1VrefLevel+irange*30, uiCA, EyeScan_index[uiCA], gEyeScan_Min[(u1VrefLevel+irange*30)/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][uiCA][EyeScan_index[uiCA]], gEyeScan_Max[(u1VrefLevel+irange*30)/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][uiCA][EyeScan_index[uiCA]]));
                                    EyeScan_index[uiCA]=EyeScan_index[uiCA]+1;
                                }
#endif
                            }
                        }
                    }

                    //Wait tCACD(22clk) before output CA pattern to DDR again..
                    mcDELAY_US(1); 

                    if(uiFinishCount == CATRAINING_NUM_LP4)
                        break;
                }

#if VENDER_JV_LOG
                for (uiCA = 0; uiCA<CATRAINING_NUM_LP4; uiCA++)
                {
                    mcSHOW_DBG_MSG5(("CBT Bit%d, CA window %d ps\n", uiCA, (iLastPass_temp[uiCA]-iFirstPass_temp[uiCA]+1)*1000000/p->frequency/64));
                }
#endif
                //set CK/CS pi delay to 0 and set CA pi delay to center
#if (ENABLE_DDR800_OPEN_LOOP_MODE_OPTION==1) || (ENABLE_DDR800_OPEN_LOOP_PHASE_PREDICTION==1)
                if (u1IsDDR800PhaseMode(p) == TRUE)
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_CA_CMD9), 0, SHU_CA_CMD9_RG_DDR400_DQ_PS_CA);
                else
#endif
                {
                    #if CBT_MOVE_CA_INSTEAD_OF_CLK
                    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_CA_CMD9), P_Fld(0x20, SHU_R0_CA_CMD9_RG_RK0_ARPI_CMD) | P_Fld(0, SHU_R0_CA_CMD9_RG_RK0_ARPI_CLK) | P_Fld(0, SHU_R0_CA_CMD9_RG_RK0_ARPI_CS));
                    #else
                    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_CA_CMD9), P_Fld(0, SHU_R0_CA_CMD9_RG_RK0_ARPI_CMD) | P_Fld(0, SHU_R0_CA_CMD9_RG_RK0_ARPI_CLK) | P_Fld(0, SHU_R0_CA_CMD9_RG_RK0_ARPI_CS));
                    #endif
                }

#if !REDUCE_LOG_FOR_PRELOADER
                mcSHOW_DBG_MSG(("\n[CmdBusTrainingLP4] CAWinSum: %d\n", uiCAWinSum));
#endif

                if(uiCAWinSum > uiCAWinSumMax)
                {
                    uiCAWinSumMax = uiCAWinSum;
                    uiFinalVref = u1VrefLevel;
                    if (u1CBTEyeScanEnable) uiFinalRange = gCBT_VREF_RANGE_SEL;

                    for (uiCA=0; uiCA<CATRAINING_NUM_LP4; uiCA++)
                    {
                        FinalWinPerCA[uiCA].first_pass = iFirstPass_temp[uiCA];
                        FinalWinPerCA[uiCA].last_pass = iLastPass_temp[uiCA];                                                
                    }
                }

#if ENABLE_EYESCAN_GRAPH
                for (uiCA=0; uiCA<CATRAINING_NUM_LP4; uiCA++)
                {
                    gEyeScan_WinSize[(u1VrefLevel+irange*30)/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][uiCA] = (iLastPass_temp[uiCA] - iFirstPass_temp[uiCA])+(iLastPass_temp[uiCA]==iFirstPass_temp[uiCA]?0:1);
                }
#endif
                if (p->dram_cbt_mode[p->rank] == CBT_BYTE_MODE1)
                {
                    (p->dram_fsp == FSP_1)? CBT_Switch_Freq(p, CBT_LOW_FREQ): NULL;
                    CBTExit(p, operating_fsp, operation_frequency); 
                }

                if (u1CBTEyeScanEnable == 0)
                {
                    if(uiCAWinSum < (uiCAWinSumMax*95/100))
                    {
                        mcSHOW_DBG_MSG(("\nCBT Vref found, early break!\n"));
                        break;//max vref found, early break;
                    }
                }
            }
        }
    }
    if (u1CBTEyeScanEnable) gCBT_VREF_RANGE_SEL = uiFinalRange;

#if (SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_VREF_CAL)
    if(p->femmc_Ready==0)
#endif
    {
        for (uiCA=0; uiCA<CATRAINING_NUM_LP4; uiCA++)
        {
            FinalWinPerCA[uiCA].win_center = (FinalWinPerCA[uiCA].first_pass +  FinalWinPerCA[uiCA].last_pass)/2;
            FinalWinPerCA[uiCA].win_size = (FinalWinPerCA[uiCA].last_pass - FinalWinPerCA[uiCA].first_pass)+(FinalWinPerCA[uiCA].last_pass==FinalWinPerCA[uiCA].first_pass?0:1);

            #if 1//CA_PER_BIT_DELAY_CELL
            iFirstCAPass[p->rank][0][uiCA] = FinalWinPerCA[uiCA].first_pass;
            iLastCAPass[p->rank][0][uiCA] = FinalWinPerCA[uiCA].last_pass;
            #endif
            
#ifdef FOR_HQA_TEST_USED
            gFinalCBTCA[p->channel][p->rank][uiCA] = FinalWinPerCA[uiCA].win_size;
#endif
            mcSHOW_DBG_MSG(("[CA %d] Center %d (%d~%d) winsize %d\n", uiCA, FinalWinPerCA[uiCA].win_center , FinalWinPerCA[uiCA].first_pass, FinalWinPerCA[uiCA].last_pass, FinalWinPerCA[uiCA].win_size ));
        }

#ifdef FOR_HQA_REPORT_USED
        if (gHQALog_flag==1)
        {
            mcSHOW_DBG_MSG(("\n"));
            for (uiCA=0; uiCA<CATRAINING_NUM_LP4; uiCA++)
            {
                HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT0_2, "CA", "_Center", uiCA, FinalWinPerCA[uiCA].win_center, NULL);
                HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT0_2, "CA", "_Perbit_Window(%)", uiCA, (FinalWinPerCA[uiCA].win_size*100 + 63)/64, NULL);
            }
        }
#endif

        #if 1//CA_PER_BIT_DELAY_CELL
        // LP4 has already consider two dies. No more position calculation of 2 dies.
        gu1DieNum[p->rank] =1;
        CATrainingPosCal(p, gu1DieNum, &iFinalCACLK, s1CACenterDiff);
        #else
        #if SW_CHANGE_FOR_SIMULATION
            iFinalCACLK =(int)((float) iCACenterSum/(float)CATRAINING_NUM_LP4);
            #else
            iFinalCACLK = iCACenterSum/CATRAINING_NUM_LP4;
            #endif
        #endif
    }

#if (SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_VREF_CAL)
    if(p->femmc_Ready==1)
    {
        uiFinalVref = p->pSavetimeData->u1CBTVref_Save[p->channel][p->rank];
    }
#endif

    mcSHOW_DBG_MSG(("\n[CmdBusTrainingLP4] Vref(ca) range %d: %d\n", gCBT_VREF_RANGE_SEL, uiFinalVref));
    mcFPRINTF((fp_A60501, "\n[CmdBusTrainingLP4] Vref(ca) is Range %d: %d\n", gCBT_VREF_RANGE_SEL, uiFinalVref));
#if VENDER_JV_LOG
    mcSHOW_DBG_MSG5(("\n[CmdBusTrainingLP4] Vref(ca) range %d: %d\n", gCBT_VREF_RANGE_SEL, uiFinalVref));
#endif

#ifdef FOR_HQA_TEST_USED
    gFinalCBTVrefCA[p->channel][p->rank] = uiFinalVref & 0x3f;
#endif

    //Set Vref after trainging
    // BYTE MODE: Set Vref & CBTEntry
    if (p->dram_cbt_mode[p->rank] == CBT_BYTE_MODE1)
    {
        (p->dram_fsp == FSP_1)? CBT_Switch_Freq(p, CBT_LOW_FREQ): NULL;
        CBTSetVref(p, uiFinalVref, operating_fsp, IN_CBT);                 
        CBTEntry(p, operating_fsp, operation_frequency); 
        (p->dram_fsp == FSP_1)? CBT_Switch_Freq(p, CBT_HIGH_FREQ): NULL;
    }
    else // NORMAL MODE: Just Set Vref
    {
        CBTSetVref(p, uiFinalVref, operating_fsp, IN_CBT);
    }

    //Set CA_PI_Delay after trainging
#if (SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_CBT)
    if(p->femmc_Ready==1)
    {
        CATrain_ClkDelay[p->channel][p->rank]=p->pSavetimeData->u1CBTClkDelay_Save[p->channel][p->rank];
        CATrain_CmdDelay[p->channel][p->rank]=p->pSavetimeData->u1CBTCmdDelay_Save[p->channel][p->rank];
        CATrain_CsDelay[p->channel][p->rank]=p->pSavetimeData->u1CBTCsDelay_Save[p->channel][p->rank];
        vSetCalibrationResult(p, DRAM_CALIBRATION_CA_TRAIN, DRAM_OK); // set default result OK, udpate status when per bit fail
    }
    else
#endif
    {
        if(iFinalCACLK <0)
        {   //Set CLK delay
#ifdef DONT_MOVE_CLK_DELAY
            CATrain_ClkDelay[p->channel][p->rank] = 0;
            mcSHOW_ERR_MSG(("Warning!! Clk Dly = %d, adjust to 0 !!\n",-iFinalCACLK));
#else
            CATrain_ClkDelay[p->channel][p->rank] = -iFinalCACLK;
#endif
            CATrain_CmdDelay[p->channel][p->rank] = 0;
        }
        else
        {   //Set CA output delay
            CATrain_ClkDelay[p->channel][p->rank] = 0;
            CATrain_CmdDelay[p->channel][p->rank] = iFinalCACLK;
        }
    }

    CBTSetCACLKResult(p, iFinalCACLK, s1CACenterDiff);
    
#if ENABLE_EYESCAN_GRAPH
    gEyeScan_CaliDelay[0] = CATrain_CmdDelay[p->channel][p->rank] + MAX_CLK_PI_DELAY -CBT_MOVE_CA_INSTEAD_OF_CLK*32;
#endif

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    if(p->femmc_Ready==0)
    {
        p->pSavetimeData->u1CBTVref_Save[p->channel][p->rank]= uiFinalVref;
        p->pSavetimeData->u1CBTClkDelay_Save[p->channel][p->rank]= CATrain_ClkDelay[p->channel][p->rank];
        p->pSavetimeData->u1CBTCmdDelay_Save[p->channel][p->rank]= CATrain_CmdDelay[p->channel][p->rank];
        p->pSavetimeData->u1CBTCsDelay_Save[p->channel][p->rank]= CATrain_CsDelay[p->channel][p->rank];
    }
#endif

    //mcSHOW_DBG_MSG(("\nAverage CA Dly: %d\n", iFinalCACLK));
    //mcFPRINTF((fp_A60501, "\nAverage CA Dly: %d\n", iFinalCACLK));
#if 0//VENDER_JV_LOG
        mcSHOW_DBG_MSG5(("\nAverage CA Dly %d\n", iFinalCACLK));
#endif
    //wait tVREF_LONG
    mcDELAY_US(1);

    //-------------  CS and CLK ----------
    CBTAdjustCS(p);

//-------  Going to exit Command bus training(CBT) mode.-------------
    (p->dram_fsp == FSP_1)? CBT_Switch_Freq(p, CBT_LOW_FREQ): NULL;
    CBTExit(p, operating_fsp, operation_frequency);
    CBTSetVref(p, uiFinalVref, operating_fsp, OUT_CBT);
    (p->dram_fsp == FSP_1)? DramcModeRegInit_CATerm(p, 0): NULL;
    (p->dram_fsp == FSP_1)? CBT_Switch_Freq(p, CBT_HIGH_FREQ): NULL;

#if EYESCAN_LOG || defined(FOR_HQA_TEST_USED)
    gFinalCBTVrefDQ[p->channel][p->rank] = uiFinalVref;
#endif

    mcSHOW_DBG_MSG3(("\n[CmdBusTrainingLP4] Done\n"));
    mcFPRINTF((fp_A60501, "\n[CmdBusTrainingLP4] Done\n"));

    //tx_rank_sel is selected by HW //Lewis@20180509: tx_rank_sel is selected by SW in CBT if TMRRI design has changed.
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), 0, RKCFG_TXRANK); 
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), 0, RKCFG_TXRANKFIX); //TXRANKFIX should be write after TXRANK or the rank will be fix at rank 1

    //Restore setting registers
    DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));

    return DRAM_OK;
}
#endif //SIMUILATION_LP4_CBT

//-------------------------------------------------------------------------
/** DramcWriteLeveling
 *  start Write Leveling Calibration.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param  apply           (U8): 0 don't apply the register we set  1 apply the register we set ,default don't apply.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL
 */
//-------------------------------------------------------------------------
#define WRITE_LEVELING_MOVD_DQS 1//UI

U8 u1MCK2UI_DivShift(DRAMC_CTX_T *p)
{
    //in LP4 1:8 mode, 8 small UI =  1 large UI
    //in LP3 & LP4 1:8 mode, 4 small UI =  1 large UI

    if (vGet_Div_Mode(p) == DIV8_MODE)
    {
        return 3;
    }
    else
    {
        return 2;
    }
}

// NOT suitable for Gating delay
static DRAM_STATUS_T ExecuteMoveDramCDelay(DRAMC_CTX_T *p, REG_TRANSFER_T regs[], S8 iShiftUI)
{
    S32 s4HighLevelDelay, s4DelaySum;
    U32 u4Tmp0p5T, u4Tmp2T;
    U8 ucDataRateDivShift = 0;
    DRAM_STATUS_T MoveResult;

    ucDataRateDivShift = u1MCK2UI_DivShift(p);

    u4Tmp0p5T = u4IO32ReadFldAlign(DRAMC_REG_ADDR(regs[0].u4Addr), regs[0].u4Fld) & (~(1<<ucDataRateDivShift));
    u4Tmp2T = u4IO32ReadFldAlign(DRAMC_REG_ADDR(regs[1].u4Addr), regs[1].u4Fld);
    //mcSHOW_DBG_MSG(("\n[MoveDramC_Orz]  u4Tmp2T:%d,  u4Tmp0p5T: %d,\n",  u4Tmp2T, u4Tmp0p5T));
    //mcFPRINTF((fp_A60501, "\n[MoveDramC_Orz]  u4Tmp2T:%d,  u4Tmp0p5T: %d,\n",  u4Tmp2T, u4Tmp0p5T));

    s4HighLevelDelay = (u4Tmp2T <<ucDataRateDivShift) + u4Tmp0p5T;
    s4DelaySum = (s4HighLevelDelay + iShiftUI);
    //mcSHOW_DBG_MSG(("\n[MoveDramC_Orz]  s4HighLevelDealy(%d) +  iShiftUI(%d) = %d\n",  s4HighLevelDelay, iShiftUI, s4DelaySum));

    if(s4DelaySum < 0)
    {
        u4Tmp0p5T =0;
        u4Tmp2T=0;
        MoveResult =  DRAM_FAIL;
        //mcSHOW_ERR_MSG(("\n[MoveDramC_Orz]  s4HighLevelDealy(%d) +  iShiftUI(%d) is small than 0!!\n",  s4HighLevelDelay, iShiftUI));
    }
    else
    {
        u4Tmp2T = s4DelaySum >> ucDataRateDivShift;
        u4Tmp0p5T = s4DelaySum - (u4Tmp2T <<ucDataRateDivShift);
        MoveResult = DRAM_OK;
    }

    vIO32WriteFldAlign(DRAMC_REG_ADDR(regs[0].u4Addr), u4Tmp0p5T, regs[0].u4Fld);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(regs[1].u4Addr), u4Tmp2T, regs[1].u4Fld);
    //mcSHOW_DBG_MSG(("\n[MoveDramC_Orz]  Final ==> u4Tmp2T:%d,  u4Tmp0p5T: %d,\n",  u4Tmp2T, u4Tmp0p5T));
    //mcFPRINTF((fp_A60501, "\n[MoveDramC_Orz]  Final ==> u4Tmp2T:%d,  u4Tmp0p5T: %d,\n",  u4Tmp2T, u4Tmp0p5T));

    return MoveResult;
}

void MoveDramC_TX_DQS(DRAMC_CTX_T *p, U8 u1ByteIdx, S8 iShiftUI)
{
    REG_TRANSFER_T TransferReg[2];

    //mcSHOW_DBG_MSG(("\n[MoveDramC_TX_DQS] Byte %d, iShiftUI %d\n", u1ByteIdx, iShiftUI));

    switch(u1ByteIdx)
    {
        case 0:
            // DQS0
            TransferReg[0].u4Addr = DRAMC_REG_SHU_SELPH_DQS1;
            TransferReg[0].u4Fld =SHU_SELPH_DQS1_DLY_DQS0;
            TransferReg[1].u4Addr = DRAMC_REG_SHU_SELPH_DQS0;
            TransferReg[1].u4Fld =SHU_SELPH_DQS0_TXDLY_DQS0;
            ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
            break;

        case 1:
            // DQS1
            TransferReg[0].u4Addr = DRAMC_REG_SHU_SELPH_DQS1;
            TransferReg[0].u4Fld =SHU_SELPH_DQS1_DLY_DQS1;
            TransferReg[1].u4Addr = DRAMC_REG_SHU_SELPH_DQS0;
            TransferReg[1].u4Fld =SHU_SELPH_DQS0_TXDLY_DQS1;
            ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
            break;

        case 2:
            // DQS2
            TransferReg[0].u4Addr = DRAMC_REG_SHU_SELPH_DQS1;
            TransferReg[0].u4Fld =SHU_SELPH_DQS1_DLY_DQS2;
            TransferReg[1].u4Addr = DRAMC_REG_SHU_SELPH_DQS0;
            TransferReg[1].u4Fld =SHU_SELPH_DQS0_TXDLY_DQS2;
            ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
            break;

        case 3:
            // DQS3
            TransferReg[0].u4Addr = DRAMC_REG_SHU_SELPH_DQS1;
            TransferReg[0].u4Fld =SHU_SELPH_DQS1_DLY_DQS3;
            TransferReg[1].u4Addr = DRAMC_REG_SHU_SELPH_DQS0;
            TransferReg[1].u4Fld =SHU_SELPH_DQS0_TXDLY_DQS3;
            ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
            break;

            default:
                break;
    }
}

void MoveDramC_TX_DQS_OEN(DRAMC_CTX_T *p, U8 u1ByteIdx, S8 iShiftUI)
{
    REG_TRANSFER_T TransferReg[2];

    //mcSHOW_DBG_MSG(("\n[MoveDramC_TX_DQS_OEN] Byte %d, iShiftUI %d\n", u1ByteIdx, iShiftUI));

    switch(u1ByteIdx)
    {
        case 0:
            // DQS_OEN_0
            TransferReg[0].u4Addr = DRAMC_REG_SHU_SELPH_DQS1;
            TransferReg[0].u4Fld =SHU_SELPH_DQS1_DLY_OEN_DQS0;
            TransferReg[1].u4Addr = DRAMC_REG_SHU_SELPH_DQS0;
            TransferReg[1].u4Fld =SHU_SELPH_DQS0_TXDLY_OEN_DQS0;
            ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
            break;

        case 1:
            // DQS_OEN_1
            TransferReg[0].u4Addr = DRAMC_REG_SHU_SELPH_DQS1;
            TransferReg[0].u4Fld =SHU_SELPH_DQS1_DLY_OEN_DQS1;
            TransferReg[1].u4Addr = DRAMC_REG_SHU_SELPH_DQS0;
            TransferReg[1].u4Fld =SHU_SELPH_DQS0_TXDLY_OEN_DQS1;
            ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
            break;

        case 2:
            // DQS_OEN_2
            TransferReg[0].u4Addr = DRAMC_REG_SHU_SELPH_DQS1;
            TransferReg[0].u4Fld =SHU_SELPH_DQS1_DLY_OEN_DQS2;
            TransferReg[1].u4Addr = DRAMC_REG_SHU_SELPH_DQS0;
            TransferReg[1].u4Fld =SHU_SELPH_DQS0_TXDLY_OEN_DQS2;
            ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
            break;

        case 3:
            // DQS_OEN_3
            TransferReg[0].u4Addr = DRAMC_REG_SHU_SELPH_DQS1;
            TransferReg[0].u4Fld =SHU_SELPH_DQS1_DLY_OEN_DQS3;
            TransferReg[1].u4Addr = DRAMC_REG_SHU_SELPH_DQS0;
            TransferReg[1].u4Fld =SHU_SELPH_DQS0_TXDLY_OEN_DQS3;
            ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
            break;

            default:
                break;
    }
}


void MoveDramC_TX_DQ(DRAMC_CTX_T *p, U8 u1ByteIdx, S8 iShiftUI)
{
    REG_TRANSFER_T TransferReg[2];

    //mcSHOW_DBG_MSG(("\n[MoveDramC_TX_DQ] Byte %d, iShiftUI %d\n", u1ByteIdx, iShiftUI));

    switch(u1ByteIdx)
    {
        case 0:
            // DQM0
            TransferReg[0].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ3;
            TransferReg[0].u4Fld =SHURK0_SELPH_DQ3_DLY_DQM0;
            TransferReg[1].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ1;
            TransferReg[1].u4Fld =SHURK0_SELPH_DQ1_TXDLY_DQM0;
            ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);

            // DQ0
            TransferReg[0].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ2;
            TransferReg[0].u4Fld =SHURK0_SELPH_DQ2_DLY_DQ0;
            TransferReg[1].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ0;
            TransferReg[1].u4Fld =SHURK0_SELPH_DQ0_TXDLY_DQ0;
            ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
        break;

        case 1:
            // DQM1
            TransferReg[0].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ3;
            TransferReg[0].u4Fld =SHURK0_SELPH_DQ3_DLY_DQM1;
            TransferReg[1].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ1;
            TransferReg[1].u4Fld =SHURK0_SELPH_DQ1_TXDLY_DQM1;
            ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
            // DQ1
            TransferReg[0].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ2;
            TransferReg[0].u4Fld =SHURK0_SELPH_DQ2_DLY_DQ1;
            TransferReg[1].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ0;
            TransferReg[1].u4Fld =SHURK0_SELPH_DQ0_TXDLY_DQ1;
            ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
        break;

        case 2:
            // DQM2
            TransferReg[0].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ3;
            TransferReg[0].u4Fld =SHURK0_SELPH_DQ3_DLY_DQM2;
            TransferReg[1].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ1;
            TransferReg[1].u4Fld =SHURK0_SELPH_DQ1_TXDLY_DQM2;
            ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
            // DQ2
            TransferReg[0].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ2;
            TransferReg[0].u4Fld =SHURK0_SELPH_DQ2_DLY_DQ2;
            TransferReg[1].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ0;
            TransferReg[1].u4Fld =SHURK0_SELPH_DQ0_TXDLY_DQ2;
            ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
        break;

        case 3:
            // DQM3
            TransferReg[0].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ3;
            TransferReg[0].u4Fld =SHURK0_SELPH_DQ3_DLY_DQM3;
            TransferReg[1].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ1;
            TransferReg[1].u4Fld =SHURK0_SELPH_DQ1_TXDLY_DQM3;
            ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
            // DQ3
            TransferReg[0].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ2;
            TransferReg[0].u4Fld =SHURK0_SELPH_DQ2_DLY_DQ3;
            TransferReg[1].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ0;
            TransferReg[1].u4Fld =SHURK0_SELPH_DQ0_TXDLY_DQ3;
            ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
        break;
    }
}

void MoveDramC_TX_DQ_OEN(DRAMC_CTX_T *p, U8 u1ByteIdx, S8 iShiftUI)
{
    REG_TRANSFER_T TransferReg[2];

    //mcSHOW_DBG_MSG(("\n[MoveDramC_TX_DQ_OEN] Byte %d, iShiftUI %d\n", u1ByteIdx, iShiftUI));

    switch(u1ByteIdx)
    {
        case 0:
            // DQM_OEN_0
            TransferReg[0].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ3;
            TransferReg[0].u4Fld =SHURK0_SELPH_DQ3_DLY_OEN_DQM0;
            TransferReg[1].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ1;
            TransferReg[1].u4Fld =SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0;
            ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
            // DQ_OEN_0
            TransferReg[0].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ2;
            TransferReg[0].u4Fld =SHURK0_SELPH_DQ2_DLY_OEN_DQ0;
            TransferReg[1].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ0;
            TransferReg[1].u4Fld =SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0;
            ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
        break;

        case 1:
            // DQM_OEN_1
            TransferReg[0].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ3;
            TransferReg[0].u4Fld =SHURK0_SELPH_DQ3_DLY_OEN_DQM1;
            TransferReg[1].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ1;
            TransferReg[1].u4Fld =SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1;
            ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
             // DQ_OEN_1
            TransferReg[0].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ2;
            TransferReg[0].u4Fld =SHURK0_SELPH_DQ2_DLY_OEN_DQ1;
            TransferReg[1].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ0;
            TransferReg[1].u4Fld =SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1;
             ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
        break;

        case 2:
            // DQM_OEN_2
            TransferReg[0].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ3;
            TransferReg[0].u4Fld =SHURK0_SELPH_DQ3_DLY_OEN_DQM2;
            TransferReg[1].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ1;
            TransferReg[1].u4Fld =SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2;
            ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
            // DQ_OEN_2
            TransferReg[0].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ2;
            TransferReg[0].u4Fld =SHURK0_SELPH_DQ2_DLY_OEN_DQ2;
            TransferReg[1].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ0;
            TransferReg[1].u4Fld =SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2;
            ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
        break;

        case 3:
            // DQM_OEN_3
            TransferReg[0].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ3;
            TransferReg[0].u4Fld =SHURK0_SELPH_DQ3_DLY_OEN_DQM3;
            TransferReg[1].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ1;
            TransferReg[1].u4Fld =SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3;
            ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
            // DQ_OEN_3
            TransferReg[0].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ2;
            TransferReg[0].u4Fld =SHURK0_SELPH_DQ2_DLY_OEN_DQ3;
            TransferReg[1].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ0;
            TransferReg[1].u4Fld =SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3;
            ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
        break;
    }
}

#if SIMULATION_WRITE_LEVELING
#if WRITE_LEVELING_MOVE_DQS_INSTEAD_OF_CLK
void WriteLevelingMoveDQSInsteadOfCLK(DRAMC_CTX_T *p, S8 iShiftUI)
{
    U8 u1ByteIdx;
    U8 backup_rank, ii;

    backup_rank = u1GetRank(p);

    for(u1ByteIdx =0 ; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
    {
        MoveDramC_TX_DQS(p, u1ByteIdx, iShiftUI);
        MoveDramC_TX_DQS_OEN(p, u1ByteIdx, iShiftUI);

        for(ii=RANK_0; ii<RANK_MAX; ii++)
        {
            vSetRank(p, ii);
            MoveDramC_TX_DQ(p, u1ByteIdx, iShiftUI);
            MoveDramC_TX_DQ_OEN(p, u1ByteIdx, iShiftUI);
        }
        vSetRank(p, backup_rank);
    }
}
#endif


U8 u1IsLP4Div4DDR800(DRAMC_CTX_T *p)
{
    if((vGet_Div_Mode(p) == DIV4_MODE) && (p->frequency == 400))
        return TRUE;
    else
        return FALSE;
}

//static void vSetDramMRWriteLevelingOnOff(DRAMC_CTX_T *p, U8 u1OnOff)
void vSetDramMRWriteLevelingOnOff(DRAMC_CTX_T *p, U8 u1OnOff)
{
    // MR2 OP[7] to enable/disable write leveling
    if(u1OnOff)
        u1MR02Value[p->dram_fsp] |= 0x80;  // OP[7] WR LEV =1
    else
        u1MR02Value[p->dram_fsp] &= 0x7f;  // OP[7] WR LEV =0

    DramcModeRegWriteByRank(p, p->rank, 2, u1MR02Value[p->dram_fsp]);
}

#define DQS_DUTY_MEASURE_WITH_WRITE_LEVELING 0
#define DDR800_DIV4_1UI_IS_64PI_FLOW !ENABLE_DDR800_OPEN_LOOP_MODE_OPTION

U8 u1IsDDR800PhaseMode(DRAMC_CTX_T *p)
{
    if((vGet_DDR800_Mode(p) == DDR800_OPEN_LOOP) || (vGet_DDR800_Mode(p) == DDR800_SEMI_LOOP))
        return TRUE;
    else // DDR800_CLOSE_LOOP and NORMAL_CLOSE_LOOP
        return FALSE;
}

static void WriteLevelingScanRange_PI(DRAMC_CTX_T *p, S32 *ps4OutBegin, S32 *ps4OutEnd, U8 *pu1OverUI, S8 *ps1ShiftUICnt)
{
    S32 s4PIBegin = 0, s4PIEnd;
    U8 u1OverUI;
    S8 s1ShiftUICnt;

    // scan region
    #if ENABLE_DDR800_OPEN_LOOP_MODE_OPTION || DDR800_DIV4_1UI_IS_64PI_FLOW
    if (vGet_DDR800_Mode(p) == DDR800_CLOSE_LOOP)
        s4PIEnd = s4PIBegin + 128; // For Eiger 1:4 mode DDR800 (UI = 64*PI), 1*UI
    else if (vGet_DDR800_Mode(p) == DDR800_OPEN_LOOP)
        s4PIEnd = s4PIBegin + 96;
    else
    #endif
        s4PIEnd = s4PIBegin + 64; // scan 1T for 2UI* DQS_32PI, 0~63

    #if ENABLE_DDR800_OPEN_LOOP_MODE_OPTION || DDR800_DIV4_1UI_IS_64PI_FLOW
    if (u1IsLP4Div4DDR800(p))
    {
        s1ShiftUICnt = 1; // shift 1*UI
        if (u1IsDDR800PhaseMode(p) == TRUE)
            u1OverUI = 32; // shift 1*UI
        else
            u1OverUI = 64; // shift 1*UI
    }
    else
    #endif
    {
        s1ShiftUICnt = 2; // PI scan over 1*T
        u1OverUI = 64; // shift 1*T
    }

    #if (fcFOR_CHIP_ID == fcLafite)
    s4PIBegin += 8; // for sync ddr800 phase calib
    #endif

    *ps4OutBegin = s4PIBegin;
    *ps4OutEnd = s4PIEnd;
    *pu1OverUI = u1OverUI;
    *ps1ShiftUICnt = s1ShiftUICnt;
}

#if ENABLE_DDR800_OPEN_LOOP_PHASE_PREDICTION
static S8 s1WriteLevelingDDR800PhaseClosest(DRAMC_CTX_T *p, S32 s4DDR800PiDelay)
{
    S8 s1DDR800Phase=0;

    s1DDR800Phase = s4DDR800PiDelay/8;   
    if((s4DDR800PiDelay%8) >= 4)
        s1DDR800Phase++;

    return (s1DDR800Phase << 3);
}

DRAM_STATUS_T DramcWriteLevelingDdr800Semi(DRAMC_CTX_T *p)
{
    U8 byte_i=0, u1OverUI=32;
    S8 s1ShiftUICnt=1;

    mcSHOW_DBG_MSG(("[Write Leveling]\n"));

    #if WRITE_LEVELING_MOVE_DQS_INSTEAD_OF_CLK
    if(p->arfgWriteLevelingInitShif[p->channel][p->rank] ==FALSE)
    {
        WriteLevelingMoveDQSInsteadOfCLK(p, -s1ShiftUICnt);// shift -1*UI
        //p->arfgWriteLevelingInitShif[p->channel][p->rank] =TRUE;
        p->arfgWriteLevelingInitShif[p->channel][RANK_0] =TRUE;
        p->arfgWriteLevelingInitShif[p->channel][RANK_1] =TRUE;

        mcSHOW_DBG_MSG3(("WriteLevelingMoveDQSInsteadOfCLK -1U\n"));
        mcFPRINTF((fp_A60501, "WriteLevelingMoveDQSInsteadOfCLK -1U\n"));
    }
    #endif

    for (byte_i = 0; byte_i < (p->data_width/DQS_BIT_NUMBER);  byte_i++)
    {
        wrlevel_dqs_final_delay[byte_i] = s1WriteLevelingDDR800PhaseClosest(p, wrlevel_ddr800_dqs_delay_prediction[p->channel][byte_i]);
        //mcSHOW_DBG_MSG(("Phase CH%d B%d => %d (PI:%d)\n", p->channel, byte_i, u1DDR800Phase[byte_i], wrlevel_ddr800_dqs_delay_prediction[p->channel][byte_i]));
        #if defined(FOR_HQA_TEST_USED) && defined(FOR_HQA_REPORT_USED)
        if (gHQALog_flag==1)
        {
                HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT1, "", "WriteLeveling_DQS", byte_i, wrlevel_dqs_final_delay[byte_i], NULL);
        }
        #endif  

        mcSHOW_DBG_MSG(("Write leveling (Byte %d): %d", byte_i, wrlevel_dqs_final_delay[byte_i]));

        if(wrlevel_dqs_final_delay[byte_i] >= u1OverUI) //ARPI_DQ_B* is 6 bits, max 0x40
        {
            wrlevel_dqs_final_delay[byte_i] -= u1OverUI;
            MoveDramC_TX_DQS(p, byte_i, s1ShiftUICnt);
            MoveDramC_TX_DQS_OEN(p, byte_i, s1ShiftUICnt);
            mcSHOW_DBG_MSG((" => %d", wrlevel_dqs_final_delay[byte_i]));
        }
        mcSHOW_DBG_MSG(("\n"));
    }

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ9), (wrlevel_dqs_final_delay[0]>>3), SHU_B0_DQ9_RG_DDR400_DQS_PS_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B1_DQ9), (wrlevel_dqs_final_delay[1]>>3), SHU_B1_DQ9_RG_DDR400_DQS_PS_B1);

    return DRAM_OK;
}
#endif

DRAM_STATUS_T DramcWriteLeveling(DRAMC_CTX_T *p)
{
// Note that below procedure is based on "ODT off"
    DRAM_STATUS_T KResult= DRAM_FAIL;

    U8 *uiLPDDR_O1_Mapping = NULL;
    U32 u4value=0, u4value1=0, u4dq_o1=0, u4dq_o1_tmp[DQS_NUMBER];
    U8 byte_i, ucsample_count;
    S32 ii, ClockDelayMax;
    U8 ucsample_status[DQS_NUMBER], ucdq_o1_perbyte[DQS_NUMBER], ucdq_o1_index[DQS_NUMBER];
    DRAM_RANK_T backup_rank;

    S32 wrlevel_dq_delay[DQS_NUMBER]; // 3 is channel number
    S32 wrlevel_dqs_delay[DQS_NUMBER]; // 3 is channel number

    #if WRITE_LEVELING_MOVE_DQS_INSTEAD_OF_CLK
    S32 i4PIBegin, i4PIEnd;
    U8 u1PIStep;
    #endif
    U8 u1WLOverUICnt=0, u1PIDelay=0, u1WLPassUICnt[DQS_NUMBER]={0};
    S32 s4PI;
    #if ENABLE_DDR800_OPEN_LOOP_MODE_OPTION || DDR800_DIV4_1UI_IS_64PI_FLOW
    U8 u1OverUI=0;
    S8 s1ShiftUICnt=0;
    #endif


    #if DQS_DUTY_MEASURE_WITH_WRITE_LEVELING
    U16 u2TriggerCnt;
    #endif

    // error handling
    if (!p)
    {
        mcSHOW_ERR_MSG(("context NULL\n"));
        return DRAM_FAIL;
    }

    U32 u4RegBackupAddress[] =
    {
        (DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0)),
        (DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL)),
        (DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL)),
        (DRAMC_REG_ADDR(DRAMC_REG_WRITE_LEV)),
        (DRAMC_REG_ADDR(DRAMC_REG_CKECTRL)),
    };

#if MRW_CHECK_ONLY
    mcSHOW_MRW_MSG(("\n==[MR Dump] %s==\n", __func__));
#endif

    fgwrlevel_done = 0;
    backup_rank = u1GetRank(p);

    //DramcRankSwap(p, p->rank);
    //tx_rank_sel is selected by SW //Lewis@20180604: tx_rank_sel is selected by SW in WL if TMRRI design has changed.
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), p->rank, RKCFG_TXRANK); 
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), 1, RKCFG_TXRANKFIX); //TXRANKFIX should be write after TXRANK     

    uiLPDDR_O1_Mapping = (U8 *)uiLPDDR4_O1_Mapping_POP[p->channel];

    // DQ mapping
    ///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    /// Note : uiLPDDR_O1_Mapping_POP, need to take care mapping in real chip, but not in test chip.
    /// Evere_st : there is bit swap inside single byte. PHY & DRAM is 1-1 byte mapping, no swap.
    for (byte_i=0; byte_i<(p->data_width/DQS_BIT_NUMBER); byte_i++)
    {
        ucdq_o1_index[byte_i] = byte_i*8;
    }

#if REG_ACCESS_PORTING_DGB
    RegLogEnable =1;
    mcSHOW_DBG_MSG(("\n[REG_ACCESS_PORTING_FUNC] DramcWriteLeveling\n"));
    mcFPRINTF((fp_A60501, "\n[REG_ACCESS_PORTING_FUNC] DramcWriteLeveling\n"));
#endif

    // backup mode settings
    DramcBackupRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));

    #if WRITE_LEVELING_MOVE_DQS_INSTEAD_OF_CLK
    if(p->arfgWriteLevelingInitShif[p->channel][p->rank] ==FALSE)
    {
        WriteLevelingMoveDQSInsteadOfCLK(p, -1);// shift -1*UI
        //p->arfgWriteLevelingInitShif[p->channel][p->rank] =TRUE;
        p->arfgWriteLevelingInitShif[p->channel][RANK_0] =TRUE;
        p->arfgWriteLevelingInitShif[p->channel][RANK_1] =TRUE;

        mcSHOW_DBG_MSG3(("WriteLevelingMoveDQSInsteadOfCLK -1U\n"));
        mcFPRINTF((fp_A60501, "WriteLevelingMoveDQSInsteadOfCLK -1U\n"));
    }
    #endif

    // decide algorithm parameters according to freq.(PI mode/ phase mode)
    WriteLevelingScanRange_PI(p, &i4PIBegin, &i4PIEnd, &u1OverUI, &s1ShiftUICnt);

 #if (SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_WRITELEVELING)
    if(p->femmc_Ready==1)
    {
        wrlevel_dqs_final_delay[0] =p->pSavetimeData->u1WriteLeveling_bypass_Save[p->channel][p->rank][0];
        wrlevel_dqs_final_delay[1] =p->pSavetimeData->u1WriteLeveling_bypass_Save[p->channel][p->rank][1];

        u1WLPassUICnt[0] = wrlevel_dqs_final_delay[0]/u1OverUI;
        u1WLPassUICnt[1] = wrlevel_dqs_final_delay[1]/u1OverUI;

        ucsample_count = 0xff;
        KResult = DRAM_OK;
        vSetCalibrationResult(p, DRAM_CALIBRATION_WRITE_LEVEL, DRAM_OK);
    }
    else
#endif
    {
        //write leveling mode initialization
        //REFCNT_FR_CLK = 0 (0x1dc[23:16]), ADVREFEN = 0 (0x44[30]), (CONF2_REFCNT =0)
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 1, DRAMC_PD_CTRL_MIOCKCTRLOFF);   //MIOCKCTRLOFF=1
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 0, DRAMC_PD_CTRL_PHYCLKDYNGEN);   //PHYCLKDYNGEN=0

        //Make CKE fixed at 1 (Don't enter power down, Put this before issuing MRS): CKEFIXON = 1
        CKEFixOnOff(p, p->rank, CKE_FIXON, CKE_WRITE_TO_ONE_CHANNEL);

        //PHY RX Setting for Write Leveling
        //Let IO toO1 path valid, Enable SMT_EN
        O1PathOnOff(p, 1);

        // enable DDR write leveling mode:  issue MR2[7] to enable write leveling (refer to DEFAULT MR2 value)
        vSetDramMRWriteLevelingOnOff(p, ENABLE);

        //wait tWLDQSEN (25 nCK / 25ns) after enabling write leveling mode (DDR3 / LPDDDR3)
        mcDELAY_US(1);

        //Set {R_DQS_B3_G R_DQS_B2_G R_DQS_B1_G R_DQS_B0_G}=1010: 0x13c[4:1] (this depends on sel_ph setting)
        //Enable Write leveling: 0x13c[0]
        //vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_WRITE_LEVELING), P_Fld(0xa, WRITE_LEVELING_DQSBX_G)|P_Fld(1, WRITE_LEVELING_WRITE_LEVEL_EN));
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WRITE_LEV), 0xa, WRITE_LEV_DQSBX_G);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WRITE_LEV), 1, WRITE_LEV_WRITE_LEVEL_EN);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WRITE_LEV), 1, WRITE_LEV_CBTMASKDQSOE);

        // select DQS
        u4value = 0x3;//select byte 0.1
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WRITE_LEV), u4value, WRITE_LEV_DQS_SEL);

        // wait tWLMRD (40 nCL / 40 ns) before DQS pulse (DDR3 / LPDDR3)
        mcDELAY_US(1);

        //Proceed write leveling...
        //Initilize sw parameters
        ClockDelayMax = MAX_TX_DQSDLY_TAPS;
        for (ii=0; ii < (S32)(p->data_width/DQS_BIT_NUMBER); ii++)
        {
            ucsample_status[ii] = 0;
            wrlevel_dqs_final_delay[ii] = 0;
        }

        //used for WL done status
        // each bit of sample_cnt represents one-byte WL status
        // 1: done or N/A. 0: NOK
        if ((p->data_width == DATA_WIDTH_16BIT))
        {
            ucsample_count = 0xfc;
        }
        else
        {
            ucsample_count = 0xf0;
        }

        mcSHOW_DBG_MSG(("[Write Leveling]\n"));
        mcSHOW_DBG_MSG(("delay  byte0  byte1  byte2  byte3\n\n"));

        mcFPRINTF((fp_A60501, "\n\tdramc_write_leveling_swcal\n"));
        mcFPRINTF((fp_A60501, "delay  byte0  byte1  byte2  byte3\n\n"));

        // Set DQS output delay to 0
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ7), 0, SHU_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);  //rank0, byte0, DQS delay
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ7), 0, SHU_R0_B1_DQ7_RK0_ARPI_PBYTE_B1);  //rank0, byte1, DQS delay

        #if DQS_DUTY_MEASURE_WITH_WRITE_LEVELING
        if(p->channel==CHANNEL_A && p->frequency == u2DFSGetHighestFreq(p))
            u1PIStep =2;
        else
        #endif
        {
            #if ENABLE_DDR800_OPEN_LOOP_MODE_OPTION
            if (u1IsDDR800PhaseMode(p) == TRUE)
                u1PIStep =8;
            else
            #endif
                u1PIStep =1;
        }

        for (s4PI=i4PIBegin; s4PI<i4PIEnd; s4PI+=u1PIStep)
        {
            if ((s4PI/u1OverUI)==(u1WLOverUICnt+1)) // Scan next 2*UI or 1*UI
            {
                u1WLOverUICnt++; // over 1*UI shift
                for (byte_i = 0; byte_i < (p->data_width/DQS_BIT_NUMBER);  byte_i++)
                {
                    MoveDramC_TX_DQS(p, byte_i, s1ShiftUICnt);
                    MoveDramC_TX_DQS_OEN(p, byte_i, s1ShiftUICnt);
                }
            }
            #if ENABLE_DDR800_OPEN_LOOP_MODE_OPTION
            else
            {
                //OPEN_LOOP_MODE phase max is 90 degree (=16), skip 135 degree
                if ((vGet_DDR800_Mode(p) == DDR800_OPEN_LOOP) && (s4PI > (16+u1OverUI*u1WLOverUICnt)))
                    continue;
            }
            #endif

            #if DQS_DUTY_MEASURE_WITH_WRITE_LEVELING
            if(u1PIStep==2)
                mcDELAY_MS(10000);
            #endif

            if (s4PI <= 0)
            {
                // Adjust Clk output delay.
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_CA_CMD9), -s4PI, SHU_R0_CA_CMD9_RG_RK0_ARPI_CLK);
            }
            else
            {
                // Adjust DQS output delay.
                // PI (TX DQ/DQS adjust at the same time)
                if (s4PI >= (u1OverUI*u1WLOverUICnt)) // For E1 1:4 mode DDR800 (UI = 64*PI) & Phase mode (45 degree = 8*PI)
                    u1PIDelay = s4PI - (u1OverUI*u1WLOverUICnt); // shift 1*UI
                else
                    u1PIDelay = s4PI;

                #if ENABLE_DDR800_OPEN_LOOP_MODE_OPTION
                if (u1IsDDR800PhaseMode(p) == TRUE)
                {
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ9), (u1PIDelay>>3), SHU_B0_DQ9_RG_DDR400_DQS_PS_B0); //Divide by 8 (45 degree = 8*PI)
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B1_DQ9), (u1PIDelay>>3), SHU_B0_DQ9_RG_DDR400_DQS_PS_B0); //Divide by 8 (45 degree = 8*PI)
                }
                else
                #endif
                {
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ7), u1PIDelay, SHU_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);  //rank0, byte0, DQS delay
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ7), u1PIDelay, SHU_R0_B1_DQ7_RK0_ARPI_PBYTE_B1);  //rank0, byte1, DQS delay
                }
            }

            //Trigger DQS pulse, R_DQS_WLEV: 0x13c[8] from 1 to 0
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WRITE_LEV), 1, WRITE_LEV_DQS_WLEV);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WRITE_LEV), 0, WRITE_LEV_DQS_WLEV);

            #if DQS_DUTY_MEASURE_WITH_WRITE_LEVELING
            if(u1PIStep==2)
            {
                for(u2TriggerCnt=0; u2TriggerCnt<10000; u2TriggerCnt++)
                {
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WRITE_LEV), 1, WRITE_LEV_DQS_WLEV);
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WRITE_LEV), 0, WRITE_LEV_DQS_WLEV);
                }
            }
            #endif

            //wait tWLO (7.5ns / 20ns) before output (DDR3 / LPDDR3)
            mcDELAY_US(1);

            //Read DQ_O1 from register
            u4dq_o1 = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_DQO1));

            #ifdef ETT_PRINT_FORMAT
            mcSHOW_DBG_MSG2(("%d    ", s4PI));
            #else
            mcSHOW_DBG_MSG2(("%2d    ", s4PI));
            #endif
            mcFPRINTF((fp_A60501, "%2d    ", s4PI));

            //mcSHOW_DBG_MSG(("0x%x    ", u4dq_o1));
            //mcFPRINTF((fp_A60501, "0x%x    ", u4dq_o1));

            for (byte_i = 0; byte_i < (p->data_width/DQS_BIT_NUMBER);  byte_i++)
            {
                ucdq_o1_perbyte[byte_i] = (U8)((u4dq_o1>>ucdq_o1_index[byte_i]) & 0xff);         // ==> TOBEREVIEW

                mcSHOW_DBG_MSG2(("%x   ", ucdq_o1_perbyte[byte_i]));
                mcFPRINTF((fp_A60501, "%x    ", ucdq_o1_perbyte[byte_i]));

                if ((ucsample_status[byte_i]==0) && (ucdq_o1_perbyte[byte_i]==0))
                {
                    ucsample_status[byte_i] = 1;
                }
                else if ((ucsample_status[byte_i]>=1) && (ucdq_o1_perbyte[byte_i] ==0))
                {
                    ucsample_status[byte_i] = 1;
                }
                else if ((ucsample_status[byte_i]>=1) && (ucdq_o1_perbyte[byte_i] == 0xff))
                {
                    ucsample_status[byte_i]++;
                }
                //mcSHOW_DBG_MSG(("(%x) ", ucsample_status[byte_i]));

                if((ucsample_count &(0x01 << byte_i))==0)// result not found of byte yet
                {
                    #if ENABLE_DDR800_OPEN_LOOP_MODE_OPTION
                    if ((u1IsDDR800PhaseMode(p) == TRUE) && (ucsample_status[byte_i] == 2))
                    {
                        wrlevel_dqs_final_delay[byte_i] = s4PI; //45 degree = 8*PI
                        ucsample_count |= (0x01 << byte_i);
                        u1WLPassUICnt[byte_i] = u1WLOverUICnt;
                    }
                    else
                    #endif
                    {
                        if((ucsample_status[byte_i] == 8) || ((s4PI==i4PIEnd-1) && (ucsample_status[byte_i] > 1)))
                        {
                            wrlevel_dqs_final_delay[byte_i] = s4PI - ucsample_status[byte_i] + 2;
                            ucsample_count |= (0x01 << byte_i);
                            u1WLPassUICnt[byte_i] = u1WLOverUICnt;
                            //mcSHOW_DBG_MSG(("(record %d) ", wrlevel_dqs_final_delay[byte_i]));
                            #if ENABLE_DDR800_OPEN_LOOP_PHASE_PREDICTION
                            if ((p->frequency == 800) && (vGet_Div_Mode(p) == DIV8_MODE) && (p->rank == RANK_0))
                                wrlevel_ddr800_dqs_delay_prediction[p->channel][byte_i] = ((800*(wrlevel_dqs_final_delay[byte_i]-32)/1600))+32;
                            #endif
                        }
                    }
                }
            }
            mcSHOW_DBG_MSG2(("\n"));
            mcFPRINTF((fp_A60501, "\n"));

            #if !DQS_DUTY_MEASURE_WITH_WRITE_LEVELING
            if (ucsample_count == 0xff)
                break;  // all byte found, early break.
            #endif
        }
    }

    if (u1WLOverUICnt>=1)
    {
        for (byte_i = 0; byte_i < (p->data_width/DQS_BIT_NUMBER);  byte_i++)
        {
            MoveDramC_TX_DQS(p, byte_i, -s1ShiftUICnt*u1WLOverUICnt);
            MoveDramC_TX_DQS_OEN(p, byte_i, -s1ShiftUICnt*u1WLOverUICnt);
        }
    }

    if (ucsample_count == 0xff)
    {
        // all bytes are done
        fgwrlevel_done= 1;
        KResult = DRAM_OK;
    }
    else
    {
        KResult = DRAM_FAIL;
    }

    vSetCalibrationResult(p, DRAM_CALIBRATION_WRITE_LEVEL, KResult);

    mcSHOW_DBG_MSG2(("pass bytecount = 0x%x (0xff: all bytes pass) \n\n", ucsample_count));
    mcFPRINTF((fp_A60501, "pass bytecount = 0x%x (0xff: all bytes pass)\n\n", ucsample_count));

#if 0
    for (byte_i = 0; byte_i < (p->data_width/DQS_BIT_NUMBER);  byte_i++)
    {
        if (ClockDelayMax > wrlevel_dqs_final_delay[byte_i])
        {
            ClockDelayMax = wrlevel_dqs_final_delay[byte_i];
        }
    }

    if (ClockDelayMax > 0)
    {
        ClockDelayMax = 0;
    }
    else
    {
        ClockDelayMax = -ClockDelayMax;
    }

    vPrintCalibrationBasicInfo(p);

    mcSHOW_DBG_MSG(("WL Clk dly = %d, CA clk dly = %d\n", ClockDelayMax, CATrain_ClkDelay[p->channel][p->rank]));
    mcFPRINTF((fp_A60501, "WL Clk dly = %d, CA clk dly = %d\n", ClockDelayMax, CATrain_ClkDelay[p->channel][p->rank]));

    // Adjust Clk & CA if needed
    if (CATrain_ClkDelay[p->channel][p->rank] < ClockDelayMax)
    {
        S32 Diff = ClockDelayMax - CATrain_ClkDelay[p->channel][p->rank];
        mcSHOW_DBG_MSG(("CA adjust %d taps\n", Diff));

        // Write shift value into CA output delay.
        u4value = CATrain_CmdDelay[p->channel][p->rank];
        u4value += Diff;
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_CA_CMD9), u4value, SHU_R0_CA_CMD9_RG_RK0_ARPI_CMD);

        mcSHOW_DBG_MSG(("Update CA PI Dly Macro0 = %d, Macro1 = %d\n", u4value, u4value1));
        mcFPRINTF((fp_A60501, "Update CA PI Dly Macro0 = %d, Macro1 = %d\n", u4value, u4value1));

        // Write shift value into CS output delay.
        //u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_ARPI_CMD), ARPI_CMD_DA_ARPI_CS);
        u4value = CATrain_CsDelay[p->channel][p->rank];
        u4value += Diff;
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_CA_CMD9), u4value, SHU_R0_CA_CMD9_RG_RK0_ARPI_CS);
        mcSHOW_DBG_MSG(("Update CS Dly = %d\n", u4value));
        mcFPRINTF((fp_A60501, "Update CS Dly = %d\n", u4value));
    }
    else
    {
        mcSHOW_DBG_MSG(("No need to update CA/CS dly (CLK dly smaller than CA training)\n"));
        ClockDelayMax = CATrain_ClkDelay[p->channel][p->rank];
    }

    //DramcEnterSelfRefresh(p, 1);  //enter self refresh mode when changing CLK
    // Write max center value into Clk output delay.
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_CA_CMD9), ClockDelayMax, SHU_R0_CA_CMD9_RG_RK0_ARPI_CLK);
    //DramcEnterSelfRefresh(p, 0);

    mcFPRINTF((fp_A60501, "Final Clk output dly = %d\n", ClockDelayMax));
    mcSHOW_DBG_MSG(("Final Clk output dly = %d\n", ClockDelayMax));
    //mcSHOW_DBG_MSG(("After adjustment...\n"));

    for (byte_i = 0; byte_i < (p->data_width/DQS_BIT_NUMBER);  byte_i++)
    {
        wrlevel_dqs_final_delay[byte_i] += (ClockDelayMax);

        mcSHOW_DBG_MSG(("DQS%d dly: %d\n", byte_i, wrlevel_dqs_final_delay[byte_i]));
        mcFPRINTF((fp_A60501, "DQS%d dly: %d\n", byte_i, wrlevel_dqs_final_delay[byte_i]));

#if defined(FOR_HQA_TEST_USED) && defined(FOR_HQA_REPORT_USED)
        HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT1, "", "WriteLeveling_DQS", byte_i, wrlevel_dqs_final_delay[byte_i], NULL);
#endif
    }
#endif

#if defined(FOR_HQA_TEST_USED) && defined(FOR_HQA_REPORT_USED)
    if (gHQALog_flag==1)
    {
        for (byte_i = 0; byte_i < (p->data_width/DQS_BIT_NUMBER);  byte_i++)
        {
            HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT1, "", "WriteLeveling_DQS", byte_i, wrlevel_dqs_final_delay[byte_i], NULL);
        }
    }
#endif    


	#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
	if(p->femmc_Ready==0)
	{
   		p->pSavetimeData->u1WriteLeveling_bypass_Save[p->channel][p->rank][0]=wrlevel_dqs_final_delay[0];
   		p->pSavetimeData->u1WriteLeveling_bypass_Save[p->channel][p->rank][1]=wrlevel_dqs_final_delay[1];
	}
	#endif
    // write leveling done, mode settings recovery if necessary
    // recover mode registers : issue MR2[7] to disable write leveling (refer to DEFAULT MR2 value)
    vSetDramMRWriteLevelingOnOff(p, DISABLE);

    // restore registers.
    DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));

    //Disable DQ_O1, SELO1ASO=0 for power saving
    O1PathOnOff(p, 0);

    #if REG_SHUFFLE_REG_CHECK
    ShuffleRegCheck =1;
    #endif

    // Calculate DQS PI delay  
    for(byte_i = 0; byte_i < (p->data_width/DQS_BIT_NUMBER);  byte_i++)
    {
        mcSHOW_DBG_MSG(("Write leveling (Byte %d): %d", byte_i, wrlevel_dqs_final_delay[byte_i]));
        if(wrlevel_dqs_final_delay[byte_i] >= (u1OverUI*u1WLPassUICnt[byte_i])) //ARPI_DQ_B* is 6 bits, max 0x40
        {
            wrlevel_dqs_final_delay[byte_i] -= u1OverUI*u1WLPassUICnt[byte_i];
            MoveDramC_TX_DQS(p, byte_i, s1ShiftUICnt*u1WLPassUICnt[byte_i]);
            MoveDramC_TX_DQS_OEN(p, byte_i, s1ShiftUICnt*u1WLPassUICnt[byte_i]);
            mcSHOW_DBG_MSG((" => %d", wrlevel_dqs_final_delay[byte_i]));
        }

        wrlevel_dqs_delay[byte_i] = wrlevel_dqs_final_delay[byte_i];
        mcSHOW_DBG_MSG(("\n"));
    }

    for(ii=p->rank; ii<RANK_MAX; ii++)
    {
        vSetRank(p,ii);
        // set to best values for  DQS
        #if ENABLE_DDR800_OPEN_LOOP_MODE_OPTION
        if (u1IsDDR800PhaseMode(p) == TRUE)
        {
            //if (p->rank == RANK_1)
            //    continue; // TODO: dual ranks for DQS delay average
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ9), (wrlevel_dqs_delay[0]>>3), SHU_B0_DQ9_RG_DDR400_DQS_PS_B0); //Divide by 8 (45 degree)
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B1_DQ9), (wrlevel_dqs_delay[1]>>3), SHU_B0_DQ9_RG_DDR400_DQS_PS_B0); //Divide by 8 (45 degree)
        }
        else
        #endif
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ7), wrlevel_dqs_delay[0], SHU_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ7), wrlevel_dqs_delay[1], SHU_R0_B1_DQ7_RK0_ARPI_PBYTE_B1);
        }
    }
    vSetRank(p,backup_rank);

    #if REG_SHUFFLE_REG_CHECK
    ShuffleRegCheck =0;
    #endif

    //DramcRankSwap(p, RANK_0);
    //tx_rank_sel is selected by HW 
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), 0, RKCFG_TXRANK); 
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), 0, RKCFG_TXRANKFIX); //TXRANKFIX should be write after TXRANK 

#if REG_ACCESS_PORTING_DGB
    RegLogEnable =0;
#endif
    mcSHOW_DBG_MSG3(("[DramcWriteLeveling] Done\n\n"));
    mcFPRINTF((fp_A60501, "[DramcWriteLeveling] Done\n\n"));

    return KResult;
}
#endif //SIMULATION_WRITE_LEVELING


#if SIMULATION_GATING
//LP3 RODT is not enable, don't need to set the RODT settings.
//LP4 RODT range is very large(5ns),  no need to adjust with gating position
//0x860	SHU_ODTCTRL	32	ODT CONTROL REGISTER
//                         31	31	RODTE	RW	PUBLIC	1'b1
// 	 	 	 	30	30	RODTE2	RW	PUBLIC	1'b1
// 	 	 	 	7	4	RODT	RW	PUBLIC	4'bx= DQSINCTL or -1
#if ENABLE_RODT_TRACKING
#define GATING_RODT_LATANCY_EN 0  //disable when RODT tracking enable
#else
#define GATING_RODT_LATANCY_EN 1  //Need to enable when RODT enable
#endif

#define GATING_PATTERN_NUM_LP4 0x23
#define GATING_GOLDEND_DQSCNT_LP4 0x4646

#define GATING_PATTERN_NUM_LP3 0x46
#define GATING_GOLDEND_DQSCNT_LP3 0x2323

#define GATING_TXDLY_CHNAGE  1 //Gating txdly chcange & RANKINCTL setting

#if SW_CHANGE_FOR_SIMULATION
#define ucRX_DLY_DQSIENSTB_LOOP 32
#define ucRX_DQS_CTL_LOOP 8
#endif

#if GATING_ADJUST_TXDLY_FOR_TRACKING
U8 u1TXDLY_Cal_min =0xff, u1TXDLY_Cal_max=0;
U8 ucbest_coarse_tune2T_backup[RANK_MAX][DQS_NUMBER];
U8 ucbest_coarse_tune0p5T_backup[RANK_MAX][DQS_NUMBER];
U8 ucbest_coarse_tune2T_P1_backup[RANK_MAX][DQS_NUMBER];
U8 ucbest_coarse_tune0p5T_P1_backup[RANK_MAX][DQS_NUMBER];
#endif


/* Current function is for LP4 only
 * u1Mode decides old or new length modes (7UI, 8UI) should be used
 * 0: OLD 8UI mode (not extend 2T RD preamble)
 * 1: NEW 7UI mode (extend 2T RD preamble) (DQS_GW_7UI defined)
 *    NEW 8UI mode (extend 2T RD preamble) (DQS_GW_7UI not defined)
 */
void DramcGatingMode(DRAMC_CTX_T *p, U8 u1Mode)
{
    // mode 0:  old burst mode
    // mode 1:  7UI or 8UI gating window length mode (depends on if DQS_GW_7UI is defined)
#if (fcFOR_CHIP_ID == fcLafite)
    /* There are currently 2 ways to set GatingMode (sets different registers)
     * 1. Alas_ka
     * 2. Bian_co, Whitn_ey, Ki_bo+(Olymp_us)
     */
    U8 u1VrefSel = 0, u1BurstE2 = 0;

    //mcSHOW_DBG_MSG3(("[GatingMode] "));
    if (u1Mode == 0) /* old mode */
    {
        u1VrefSel = 0;
        u1BurstE2 = 0;
        //mcSHOW_DBG_MSG3(("old 8UI\n"));
    }
    else /* 7UI or 8UI gating window length mode */
    {
#ifdef DQS_GW_7UI
        u1VrefSel = 2;
        u1BurstE2 = 1;
        //mcSHOW_DBG_MSG3(("new 7UI\n"));
#else
        u1VrefSel = 1;
        u1BurstE2 = 0;
        //mcSHOW_DBG_MSG3(("new 8UI\n"));
#endif
    }

    /* BIAS_VREF_SEL is used as switch for old, new burst modes */
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), u1VrefSel, B0_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), u1VrefSel, B1_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B1);
#endif /* fcFOR_CHIP_ID */

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ9), 1, B0_DQ9_RG_RX_ARDQS0_DQSIENMODE_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ9), 1, B1_DQ9_RG_RX_ARDQS0_DQSIENMODE_B1);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_STBCAL1), u1BurstE2, STBCAL1_DQSIEN_7UI_EN);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), 1, STBCAL_DQSIENMODE_SELPH);

    /* Perform reset (makes sure PHY's behavior works as the above setting) */
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B0_DQ9), P_Fld(0, B0_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B0) |P_Fld(0, B0_DQ9_RG_RX_ARDQ_STBEN_RESETB_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B1_DQ9), P_Fld(0, B1_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B1) |P_Fld(0, B1_DQ9_RG_RX_ARDQ_STBEN_RESETB_B1));
    mcDELAY_US(1);//delay 10ns
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B1_DQ9), P_Fld(1, B1_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B1) |P_Fld(1, B1_DQ9_RG_RX_ARDQ_STBEN_RESETB_B1));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B0_DQ9), P_Fld(1, B0_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B0) |P_Fld(1, B0_DQ9_RG_RX_ARDQ_STBEN_RESETB_B0));
    //DramPhyReset(p);
#if __ETT__
    if (u1IsLP4Family(p->dram_type) == FALSE)
    {  // GatingMode() is designed for LP4 only (only resets current channel PHY)
        mcSHOW_ERR_MSG(("GatingMode err!(LP4 only)\n"));
    }
#endif
}

U8 u1GetGatingStartPos(DRAMC_CTX_T *p)
{
    const U8 au1MR2MappingToRL[2][8] = {{6, 10, 14, 20, 24, 28, 32, 36},   //normal mode
                                        {6, 10, 16, 22, 26, 32, 36, 40}};  //byte mode

    U8 u1MR0_LatencyMode;
    U8 u1MR2RLValue = u1MR02Value[p->dram_fsp] & 0x7; //MR2 Op[2:0]
    U8 u1RX_Path_delay_UI, u1RealRL,u1StartUI, u1ExtraMCKfor1_4mode;
    U8 u1MCK2CK_UI, u1ReadDQSINCTL, u1DQSINCTL_UI;
    U8 u4TDQSCK_UI_min = 1500 * p->frequency *2/ 1000000;
 // 1500ps = ? UI

    if(gu2MR0_Value[p->rank] == 0xffff)  //MR0 is not ready
    {
        u1MR0_LatencyMode = CBT_NORMAL_MODE;
    }
    else
    {
        u1MR0_LatencyMode = (gu2MR0_Value[p->rank]>>1) & 0x1; //MR0 OP[1],  0:normal mode,  1:byte mode
    }

    u1RealRL = au1MR2MappingToRL[u1MR0_LatencyMode][u1MR2RLValue];
 
    if(vGet_Div_Mode(p) == DIV4_MODE)
    {
        u1MCK2CK_UI = 4;
        u1ExtraMCKfor1_4mode = 1;
    }
    else
    {
        u1MCK2CK_UI = 8;
        u1ExtraMCKfor1_4mode = 0;
    }

    // RX_Path_delay_UI = RL*2 + tDQSCK_UI<1500~3500ps> - PHY_interanl<skip 30ps> - GatingAheadDQS<2UI> + if(1:4 mod)+1MCK 
    u1RX_Path_delay_UI = (u1RealRL<<1) + u4TDQSCK_UI_min - 2 + (u1MCK2CK_UI*u1ExtraMCKfor1_4mode);

    u1ReadDQSINCTL = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQSCTL), SHURK0_DQSCTL_DQSINCTL);
    u1DQSINCTL_UI = u1ReadDQSINCTL * u1MCK2CK_UI;
    
    if((u1RX_Path_delay_UI-3) >= u1DQSINCTL_UI)  
        u1StartUI = (u1RX_Path_delay_UI -3) - u1DQSINCTL_UI; // gating start calibration position = gating min position(1500ns) -3UI
    else
    {
        u1StartUI =0;
        mcSHOW_ERR_MSG(("GatingStartPos err! Need to fine-tune default DQSINCTL value.\n(RX_Path_delay_UI %d) < DQSINCTL_UI %d)\n", u1RX_Path_delay_UI, u1DQSINCTL_UI));
        #if __ETT__
        while(1);
        #endif
    }

    mcSHOW_DBG_MSG(("[GatingStartPos] MR0_LatencyMode %d, u1RealRL %d , u4TDQSCK_UI_min %d, 1:4ExtraMCK %d\n", u1MR0_LatencyMode, u1RealRL, u4TDQSCK_UI_min, u1ExtraMCKfor1_4mode));
    mcSHOW_DBG_MSG(("RX_Path_delay_UI(%d) -3 - DQSINCTL_UI(%d) = u1StartUI(%d)\n", u1RX_Path_delay_UI, u1DQSINCTL_UI, u1StartUI));

    return u1StartUI;        
}


// Use gating old burst mode to find gating window boundary
// Set the begining of window as new burst mode gating window center.
#define LP4_GATING_LEAD_LAG_FLAG_JUDGE 1

DRAM_STATUS_T DramcRxdqsGatingCal(DRAMC_CTX_T *p)
{
    #if !SW_CHANGE_FOR_SIMULATION
    U8 ucRX_DLY_DQSIENSTB_LOOP,ucRX_DQS_CTL_LOOP;
    #endif
    U32 u4value, u4all_result_R=0, u4all_result_F=0, u4err_value;
    U8 ucpass_begin[DQS_NUMBER] = {0}, ucpass_count[DQS_NUMBER] = {0}, ucCurrentPass;
    U8 ucmin_coarse_tune2T[DQS_NUMBER] = {0}, ucmin_coarse_tune0p5T[DQS_NUMBER] = {0}, ucmin_fine_tune[DQS_NUMBER] = {0};
    U8 ucpass_count_1[DQS_NUMBER] = {0}, ucmin_coarse_tune2T_1[DQS_NUMBER] = {0}, ucmin_coarse_tune0p5T_1[DQS_NUMBER] = {0}, ucmin_fine_tune_1[DQS_NUMBER] = {0};
    U8 dqs_i=0,  ucdly_coarse_large, ucdly_coarse_0p5T, ucdly_fine_xT, ucDQS_GW_FINE_STEP=0;
    U8 ucdqs_result_R, ucdqs_result_F, uctmp_offset, uctmp_value;
    U8 ucbest_fine_tune[DQS_NUMBER] = {0}, ucbest_coarse_tune0p5T[DQS_NUMBER] = {0}, ucbest_coarse_tune2T[DQS_NUMBER] = {0};
    U8 ucbest_fine_tune_P1[DQS_NUMBER] = {0}, ucbest_coarse_tune0p5T_P1[DQS_NUMBER] = {0}, ucbest_coarse_tune2T_P1[DQS_NUMBER] = {0};

    U8 ucFreqDiv = 0;
    U8 ucdly_coarse_large_P1, ucdly_coarse_0p5T_P1;

#if GATING_ADJUST_TXDLY_FOR_TRACKING
    U8 u1TX_dly_DQSgated = 0;
#endif

#if GATING_RODT_LATANCY_EN
    U8 ucdly_coarse_large_RODT, ucdly_coarse_0p5T_RODT;
    U8 ucdly_coarse_large_RODT_P1 = 0, ucdly_coarse_0p5T_RODT_P1 = 0;
    U8 ucbest_coarse_large_RODT[DQS_NUMBER] = {0}, ucbest_coarse_0p5T_RODT[DQS_NUMBER] = {0};
    U8 ucbest_coarse_large_RODT_P1[DQS_NUMBER] = {0}, ucbest_coarse_0p5T_RODT_P1[DQS_NUMBER] = {0};
#endif
    U8 ucCoarseTune=0, ucCoarseStart=8, ucCoarseEnd=8;
    U32 LP3_DataPerByte[DQS_NUMBER] = {0};
    U32 u4DebugCnt[DQS_NUMBER] = {0};
    U16 u2DebugCntPerByte;

    #if LP4_GATING_LEAD_LAG_FLAG_JUDGE
    U8 u1DQS_lead[DQS_NUMBER] = {0}, u1DQS_lag[DQS_NUMBER] = {0}, u1DQS_high[DQS_NUMBER] = {0}, u1DQS_transition[DQS_NUMBER] = {0}, u1DQSGatingFound[DQS_NUMBER] = {0};
    U8 ucdly_coarse_large_leadLag[DQS_NUMBER] = {0}, ucdly_coarse_0p5T_leadLag[DQS_NUMBER] = {0}, ucdly_fine_tune_leadLag[DQS_NUMBER] = {0};
    #endif

    U8 u1PassByteCount=0;

   #if 0///SUPPORT_SAVE_TIME_FOR_CALIBRATION
  U8 ucpass_offset=0;
     U8 ucpass_count_0_temp=0;
     U8 ucpass_count_1_temp=0;
     U8 ucCoarseTune_offset=0;
     U8 minpasscount=0;
   #endif

    // error handling
    if (!p)
    {
        mcSHOW_ERR_MSG(("context NULL\n"));
        return DRAM_FAIL;
    }
    
    U32 u4RegBackupAddress[] =
    {
        (DRAMC_REG_ADDR(DRAMC_REG_STBCAL)),
        (DRAMC_REG_ADDR(DRAMC_REG_STBCAL1)),
        (DRAMC_REG_ADDR(DRAMC_REG_DDRCONF0)),
        (DRAMC_REG_ADDR(DRAMC_REG_SPCMD)),
        (DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0)),
    };

    
    //Register backup
    DramcBackupRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));

    //Justin: DQ_REV_B*[5] =1, select RX gating mode to prevent 0.5T fake gating window behind real window.
    /* LP4: Disable(set to 0) "RX DQS ISI pulse CG function" during gating window calibration (must set to 1 when done) */
    RxDQSIsiPulseCG(p, DISABLE);
    DramcGatingMode(p, 1);
    // Set Gating PI delay to 0
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQSIEN), 0x0);

    //Disable perbank refresh, use all bank refresh, currently DUT problem
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0),  0, REFCTRL0_PBREFEN);

    // Disable HW gating first, 0x1c0[31], need to disable both UI and PI tracking or the gating delay reg won't be valid.
    DramcHWGatingOnOff(p, 0);

    //If DQS ring counter is different as our expectation, error flag is asserted and the status is in ddrphycfg 0xFC0 ~ 0xFCC
    //Enable this function by R_DMSTBENCMPEN=1 (0x348[18])
    //Set R_DMSTBCNT_LATCH_EN=1, 0x348[11]
    //Set R_DM4TO1MODE=0, 0x54[11]
    //Clear error flag by ddrphycfg 0x5c0[1] R_DMPHYRST
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_STBCAL1), 1, STBCAL1_STBENCMPEN);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_STBCAL1), 1, STBCAL1_STBCNT_LATCH_EN);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DDRCONF0), 0, DDRCONF0_DM4TO1MODE);

    //enable &reset DQS counter
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1, SPCMD_DQSGCNTEN);
    mcDELAY_US(4);//wait 1 auto refresh after DQS Counter enable

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1, SPCMD_DQSGCNTRST);
    mcDELAY_US(1);//delay 2T
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_DQSGCNTRST);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), u1GetRank(p), MISC_CTRL1_R_DMSTBENCMP_RK_OPT);
    DramcEngine2Init(p, 0x55000000, 0xaa000000 |GATING_PATTERN_NUM_LP4, TEST_AUDIO_PATTERN, 0);

    #if 0 // Array initialization are now performed during declaration
    //Initialize variables
    for (dqs_i=0; dqs_i<(p->data_width/DQS_BIT_NUMBER); dqs_i++)
    {
        ucpass_begin[dqs_i] = 0;
        ucpass_count[dqs_i] = 0;
        #if LP4_GATING_LEAD_LAG_FLAG_JUDGE
        u1DQS_lead[dqs_i] = 0;
        u1DQS_lag[dqs_i]   = 0;
        u1DQS_high[dqs_i] = 0;
        u1DQS_transition[dqs_i] =0;

        ucdly_coarse_large_leadLag[dqs_i] = 0;
        ucdly_coarse_0p5T_leadLag[dqs_i] = 0;
        ucdly_fine_tune_leadLag[dqs_i] = 0;
        #endif
    }
    #endif
    
    #if !SW_CHANGE_FOR_SIMULATION
    ucRX_DLY_DQSIENSTB_LOOP= 32;// PI fine tune 0->31
    ucRX_DQS_CTL_LOOP = 8; // Since Evere_st, no matter LP3 or LP4. ucRX_DQS_CTL_LOOP is 8.
    #endif

    mcSHOW_DBG_MSG2(("[Gating]\n"));
    vPrintCalibrationBasicInfo(p);

    if (vGet_Div_Mode(p) == DIV4_MODE)
        ucFreqDiv= 2;
    else
        ucFreqDiv= 4;
    
    #if ENABLE_DDR800_OPEN_LOOP_MODE_OPTION
    if (u1IsDDR800PhaseMode(p) == TRUE)
        ucDQS_GW_FINE_STEP = (0x1 << 4); // Divide by 16 (90 degree)
    else
    #endif 
        ucDQS_GW_FINE_STEP = DQS_GW_FINE_STEP;

    /* ucCoarseStart
     * 1. Depends on current freq's DQSINCTL setting
     * 2. Preserves ~4UI before actual DQS delay value
     */
    ucCoarseStart= u1GetGatingStartPos(p);
    ucCoarseEnd = ucCoarseStart + 20;

    #if GATING_RODT_LATANCY_EN  //LP3 RODT is not enable, don't need to set the RODT settings.
    // Fix build warning, initialize variables.
    ucdly_coarse_large_RODT = 0;
    ucdly_coarse_0p5T_RODT = 0;

    ucdly_coarse_large_RODT_P1 = 4;
    ucdly_coarse_0p5T_RODT_P1 = 4;

    // 1.   DQSG latency =
    // (1)   R_DMR*DQSINCTL[3:0] (MCK) +
    // (2)   selph_TX_DLY[2:0] (MCK) +
    // (3)   selph_dly[2:0] (UI)

    // 2.   RODT latency =
    // (1)   R_DMTRODT[3:0] (MCK) +
    // (2)   selph_TX_DLY[2:0] (MCK) +
    // (3)   selph_dly[2:0] (UI)

    #if 0
    //R_DMTRODT[3:0] (MCK)  = R_DMR*DQSINCTL[3:0] (MCK)
    //if(p->rank == RANK_0)
    {
        u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQSCTL), SHURK0_DQSCTL_DQSINCTL);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_ODTCTRL), u4value, SHU_ODTCTRL_RODT);
    }
    #if 0 //R1DQSINCTL and R2DQSINCTL is useless on A-PHY. Only need to set RODT once.
    else //rank1
    {
        u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQSCTL2), DQSCTL2_R1DQSINCTL);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DDR2CTL), u4value & 0x7, DDR2CTL_RODT);//R_DMTRODT[2:0]
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DDR2CTL), (u4value >>3), DDR2CTL_RODT3);//R_DMTRODT[3]
    }
    #endif
    #endif
    #endif

    #if (SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_GatingCal)
    if(p->femmc_Ready==1)
    {
          mcSHOW_DBG_MSG2(("[bypass Gating]\n"));
    }
    else
    #endif
    {
        for (ucCoarseTune = ucCoarseStart; ucCoarseTune < ucCoarseEnd; ucCoarseTune += DQS_GW_COARSE_STEP)
        {
            ucdly_coarse_large      = ucCoarseTune / ucRX_DQS_CTL_LOOP;
            ucdly_coarse_0p5T      = ucCoarseTune % ucRX_DQS_CTL_LOOP;

            ucdly_coarse_large_P1 = (ucCoarseTune + ucFreqDiv) / ucRX_DQS_CTL_LOOP;
            ucdly_coarse_0p5T_P1 =(ucCoarseTune + ucFreqDiv) % ucRX_DQS_CTL_LOOP;

            #if GATING_RODT_LATANCY_EN  //LP3 RODT is not enable, don't need to set the RODT settings.
            u4value = (ucdly_coarse_large <<3)+ucdly_coarse_0p5T;

            if(u4value>=11)
            {
                u4value -=11;
                ucdly_coarse_large_RODT     = u4value>>3;
                ucdly_coarse_0p5T_RODT     = u4value -(ucdly_coarse_large_RODT<<3);

                u4value = (ucdly_coarse_large <<3)+ucdly_coarse_0p5T -11;
                ucdly_coarse_large_RODT_P1     = u4value>>3;
                ucdly_coarse_0p5T_RODT_P1     = u4value -(ucdly_coarse_large_RODT_P1<<3);
            }
            else
            {
                ucdly_coarse_large_RODT = 0;
                ucdly_coarse_0p5T_RODT = 0;

                ucdly_coarse_large_RODT_P1 = 4;
                ucdly_coarse_0p5T_RODT_P1 = 4;

                mcSHOW_ERR_MSG(("[RxdqsGatingCal] Error: ucdly_coarse_large_RODT[%d] is already 0. RODT cannot be -11 UI\n", dqs_i));
                mcFPRINTF((fp_A60501, "[RxdqsGatingCal] Error: ucdly_coarse_large_RODT[%d] is already 0. RODT cannot be -11 UI\n", dqs_i));
            }
            #endif

            //DramPhyCGReset(p, 1);// need to reset when UI update or PI change >=2

            // 4T or 2T coarse tune
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG0), \
                                        P_Fld((U32) ucdly_coarse_large, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED)| \
                                        P_Fld((U32) ucdly_coarse_large, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)| \
                                        P_Fld((U32) ucdly_coarse_large, SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED)| \
                                        P_Fld((U32) ucdly_coarse_large, SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED)| \
                                        P_Fld((U32) ucdly_coarse_large_P1, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)| \
                                        P_Fld((U32) ucdly_coarse_large_P1, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)| \
                                        P_Fld((U32) ucdly_coarse_large_P1, SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED_P1)| \
                                        P_Fld((U32) ucdly_coarse_large_P1, SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED_P1));

            // 0.5T coarse tune
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG1), \
                                        P_Fld((U32) ucdly_coarse_0p5T, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED)| \
                                        P_Fld((U32) ucdly_coarse_0p5T, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)| \
                                        P_Fld((U32) ucdly_coarse_0p5T, SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED)| \
                                        P_Fld((U32) ucdly_coarse_0p5T, SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED)| \
                                        P_Fld((U32) ucdly_coarse_0p5T_P1, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)| \
                                        P_Fld((U32) ucdly_coarse_0p5T_P1, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)| \
                                        P_Fld((U32) ucdly_coarse_0p5T_P1, SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED_P1)| \
                                        P_Fld((U32) ucdly_coarse_0p5T_P1, SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED_P1));

            #if GATING_RODT_LATANCY_EN  //LP3 RODT is not enable, don't need to set the RODT settings.
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_ODTEN0), \
                                        P_Fld((U32) ucdly_coarse_large_RODT, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN)| \
                                        P_Fld((U32) ucdly_coarse_large_RODT, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN)| \
                                        P_Fld((U32) ucdly_coarse_large_RODT, SHURK0_SELPH_ODTEN0_TXDLY_B2_RODTEN)| \
                                        P_Fld((U32) ucdly_coarse_large_RODT, SHURK0_SELPH_ODTEN0_TXDLY_B3_RODTEN)| \
                                        P_Fld((U32) ucdly_coarse_large_RODT_P1, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN_P1)| \
                                        P_Fld((U32) ucdly_coarse_large_RODT_P1, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN_P1)| \
                                        P_Fld((U32) ucdly_coarse_large_RODT_P1, SHURK0_SELPH_ODTEN0_TXDLY_B2_RODTEN_P1)| \
                                        P_Fld((U32) ucdly_coarse_large_RODT_P1, SHURK0_SELPH_ODTEN0_TXDLY_B3_RODTEN_P1));

            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_ODTEN1), \
                                        P_Fld((U32) ucdly_coarse_0p5T_RODT, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN)| \
                                        P_Fld((U32) ucdly_coarse_0p5T_RODT, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN)| \
                                        P_Fld((U32) ucdly_coarse_0p5T_RODT, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN)| \
                                        P_Fld((U32) ucdly_coarse_0p5T_RODT, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN)| \
                                        P_Fld((U32) ucdly_coarse_0p5T_RODT_P1, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN_P1)| \
                                        P_Fld((U32) ucdly_coarse_0p5T_RODT_P1, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN_P1)| \
                                        P_Fld((U32) ucdly_coarse_0p5T_RODT_P1, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN_P1)| \
                                        P_Fld((U32) ucdly_coarse_0p5T_RODT_P1, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN_P1));

            //mcSHOW_DBG_MSG2(("RODT delay(2T, 0.5T) = (%d, %d)\n", ucdly_coarse_large_RODT, ucdly_coarse_0p5T_RODT));
            //mcFPRINTF((fp_A60501,"RODT delay(2T, 0.5T) = (%d, %d)\n", ucdly_coarse_large_RODT, ucdly_coarse_0p5T_RODT));
            #endif

            for (ucdly_fine_xT=DQS_GW_FINE_START; ucdly_fine_xT<DQS_GW_FINE_END; ucdly_fine_xT+=ucDQS_GW_FINE_STEP)
            {
                #if ENABLE_DDR800_OPEN_LOOP_MODE_OPTION
                if (u1IsDDR800PhaseMode(p) == TRUE)
                {
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ7), ucdly_fine_xT>>4, SHU_R0_B0_DQ7_DA_ARPI_DDR400_0D5UI_RK0_B0); // Divide by 16 (90 degree)
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ7), ucdly_fine_xT>>4, SHU_R0_B1_DQ7_DA_ARPI_DDR400_0D5UI_RK0_B1); // Divide by 16 (90 degree)
                }
                else
                #endif
                {
                    //ok we set a coarse/fine tune value already
                    u4value = ucdly_fine_xT | (ucdly_fine_xT<<8) | (ucdly_fine_xT<<16) | (ucdly_fine_xT<<24);
                    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQSIEN), u4value);
                }
                //reset phy, reset read data counter
                DramPhyReset(p);

                //reset DQS counter
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1, SPCMD_DQSGCNTRST);
                mcDELAY_US(1);//delay 2T
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_DQSGCNTRST);

                // enable TE2, audio pattern
                DramcEngine2Run(p, TE_OP_READ_CHECK, TEST_AUDIO_PATTERN);

                u4all_result_R = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_STBERR_RK0_R), MISC_STBERR_RK0_R_STBERR_RK0_R);
                u4all_result_F = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_STBERR_RK0_F), MISC_STBERR_RK0_F_STBERR_RK0_F);

                //read DQS counter
                u4DebugCnt[0] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DQSGNWCNT0));
                u4DebugCnt[1] = (u4DebugCnt[0] >> 16) & 0xffff;
                u4DebugCnt[0] &= 0xffff;
                u4err_value =0;

                #if LP4_GATING_LEAD_LAG_FLAG_JUDGE
                for (dqs_i=0; dqs_i<(p->data_width/DQS_BIT_NUMBER); dqs_i++)
                {
                    if(dqs_i==0)
                    {
                        u1DQS_lead[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_PHY_RGS_STBEN_B0), MISC_PHY_RGS_STBEN_B0_AD_RX_ARDQS0_STBEN_LEAD_B0);
                        u1DQS_lag[0]   = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_PHY_RGS_STBEN_B0), MISC_PHY_RGS_STBEN_B0_AD_RX_ARDQS0_STBEN_LAG_B0);
                    }
                    else //dqs1
                    {
                        u1DQS_lead[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_PHY_RGS_STBEN_B1), MISC_PHY_RGS_STBEN_B1_AD_RX_ARDQS0_STBEN_LEAD_B1);
                        u1DQS_lag[1]   = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_PHY_RGS_STBEN_B1), MISC_PHY_RGS_STBEN_B1_AD_RX_ARDQS0_STBEN_LAG_B1);
                    }
                }
                #endif

                 /*TINFO="%2d  %2d  %2d |(B3->B0) 0x%4x, 0x%4x, 0x%4x, 0x%4x | %2x %2x  %2x %2x  %2x %2x  %2x %2x | 0x%8x\n", ucdly_coarse_large, ucdly_coarse_0p5T, ucdly_fine_xT, \
                                                      u4DebugCnt[3], u4DebugCnt[2], u4DebugCnt[1], u4DebugCnt[0], \
                                                       (u4all_result_F>>24)&0xff, (u4all_result_R>>24)&0xff, \
                                                       (u4all_result_F>>16)&0xff, (u4all_result_R>>16)&0xff, \
                                                       (u4all_result_F>>8)&0xff,   (u4all_result_R>>8)&0xff, \
                                                       (u4all_result_F)&0xff,         (u4all_result_R)&0xff,    u4err_value*/

                #ifdef ETT_PRINT_FORMAT
                #if 0
                mcSHOW_DBG_MSG(("%d  %d  %d |(B3->B0) 0x%H, 0x%H, 0x%H, 0x%H | %B %B  %B %B  %B %B  %B %B | B0(%d, %d) B1(%d, %d) |0x%X\n",
                                                       ucdly_coarse_large, ucdly_coarse_0p5T, ucdly_fine_xT, \
                                                       u4DebugCnt[3], u4DebugCnt[2], u4DebugCnt[1], u4DebugCnt[0], \
                                                       (u4all_result_F>>24)&0xff, (u4all_result_R>>24)&0xff, \
                                                       (u4all_result_F>>16)&0xff, (u4all_result_R>>16)&0xff, \
                                                       (u4all_result_F>>8)&0xff,   (u4all_result_R>>8)&0xff, \
                                                       (u4all_result_F)&0xff,         (u4all_result_R)&0xff,
                                                       #if LP4_GATING_LEAD_LAG_FLAG_JUDGE
                                                       u1DQS_lead[0], u1DQS_lag[0], u1DQS_lead[1], u1DQS_lag[1],
                                                       #else
                                                       0,0,0,0,
                                                       #endif
                                                        u4err_value));
                #else
                mcSHOW_DBG_MSG(("%d %d %d |", ucdly_coarse_large, ucdly_coarse_0p5T, ucdly_fine_xT ));

                for (dqs_i=0; dqs_i<(p->data_width/DQS_BIT_NUMBER); dqs_i++)
                {
                    mcSHOW_DBG_MSG(("%H ", u4DebugCnt[dqs_i]));
                }
                mcSHOW_DBG_MSG((" |"));
                for (dqs_i=0; dqs_i<(p->data_width/DQS_BIT_NUMBER); dqs_i++)
                {
                    mcSHOW_DBG_MSG(("(%B %B)", (u4all_result_F>>(DQS_BIT_NUMBER*dqs_i)) & 0xff, (u4all_result_R>>(DQS_BIT_NUMBER*dqs_i)) & 0xff));
                }

                #if LP4_GATING_LEAD_LAG_FLAG_JUDGE
                mcSHOW_DBG_MSG((" |"));
                for (dqs_i=0; dqs_i<(p->data_width/DQS_BIT_NUMBER); dqs_i++)
                {
                    mcSHOW_DBG_MSG(("(%d %d)", u1DQS_lead[dqs_i], u1DQS_lag[dqs_i]));
                }
                #endif

                mcSHOW_DBG_MSG2(("| %X", u4err_value));
                //mcSHOW_DBG_MSG(("\n"));
                #endif

                #else
                mcSHOW_DBG_MSG(("%2d  %2d  %2d |(B3->B0) 0x%4x, 0x%4x, 0x%4x, 0x%4x | %2x %2x  %2x %2x  %2x %2x  %2x %2x | 0x%8x\n", ucdly_coarse_large, ucdly_coarse_0p5T, ucdly_fine_xT, \
                                                      u4DebugCnt[3], u4DebugCnt[2], u4DebugCnt[1], u4DebugCnt[0], \
                                                       (u4all_result_F>>24)&0xff, (u4all_result_R>>24)&0xff, \
                                                       (u4all_result_F>>16)&0xff, (u4all_result_R>>16)&0xff, \
                                                       (u4all_result_F>>8)&0xff,   (u4all_result_R>>8)&0xff, \
                                                       (u4all_result_F)&0xff,         (u4all_result_R)&0xff,    u4err_value));

                #endif

                 mcFPRINTF((fp_A60501,"%2d  %2d  %2d |(B3->B0) 0x%4x, 0x%4x, 0x%4x, 0x%4x | (B3->B0) %2x %2x  %2x %2x  %2x %2x  %2x %2x | 0x%8x\n", ucdly_coarse_large, ucdly_coarse_0p5T, ucdly_fine_xT, \
                                                    u4DebugCnt[3], u4DebugCnt[2], u4DebugCnt[1], u4DebugCnt[0], \
                                                     (u4all_result_F>>24)&0xff, (u4all_result_R>>24)&0xff, \
                                                     (u4all_result_F>>16)&0xff, (u4all_result_R>>16)&0xff, \
                                                     (u4all_result_F>>8)&0xff,   (u4all_result_R>>8)&0xff, \
                                                     (u4all_result_F)&0xff,         (u4all_result_R)&0xff,    u4err_value));

                //find gating window pass range per DQS separately
                for (dqs_i=0; dqs_i<(p->data_width/DQS_BIT_NUMBER); dqs_i++)
                {
                    if(u1PassByteCount  & (1<<dqs_i))
                    {
                        // real window found, break to prevent finding fake window.
                        continue;
                    }
                
                    //get dqs error result
                    ucdqs_result_R = (U8)((u4all_result_R>>(8*dqs_i))&0xff);
                    ucdqs_result_F = (U8)((u4all_result_F>>(8*dqs_i))&0xff);
                    u2DebugCntPerByte =(U16) u4DebugCnt[dqs_i];

                    // check if current tap is pass
                    ucCurrentPass =0;
                    if((ucdqs_result_R==0) && (ucdqs_result_F==0) && (u2DebugCntPerByte==GATING_GOLDEND_DQSCNT_LP4))
                         ucCurrentPass =1;

                    //if current tap is pass
                    if (ucCurrentPass)
                    {
                        if (ucpass_begin[dqs_i]==0)
                        {
                            //no pass tap before , so it is the begining of pass range
                            ucpass_begin[dqs_i] = 1;
                            ucpass_count_1[dqs_i] = 0;
                            ucmin_coarse_tune2T_1[dqs_i] = ucdly_coarse_large;
                            ucmin_coarse_tune0p5T_1[dqs_i] = ucdly_coarse_0p5T;
                            ucmin_fine_tune_1[dqs_i] = ucdly_fine_xT;

                            /*TINFO="[Byte %d]First pass (%d, %d, %d)\n", dqs_i,ucdly_coarse_large, ucdly_coarse_0p5T, ucdly_fine_xT*/
                            mcSHOW_DBG_MSG2(("  [Byte %d]First pass", dqs_i));

#if 0
                            if (ucCoarseTune == ucCoarseStart && ucdly_fine_xT==DQS_GW_FINE_START)
                            {
                                if((((u4all_result_F>>(DQS_BIT_NUMBER*dqs_i)) & 0xff)==0) && (((u4all_result_R>>(DQS_BIT_NUMBER*dqs_i)) & 0xff)==0))
                                {
                                    mcSHOW_ERR_MSG((" is from Coarse Start Position!!! Warning!!!\n"));
#if __ETT__                                
                                    while(1);
#endif                                                               
                                }
                            }
#endif
                        }

                        if (ucpass_begin[dqs_i]==1)
                        {
                            //incr pass tap number
                            ucpass_count_1[dqs_i]++;
                        }

#if LP4_GATING_LEAD_LAG_FLAG_JUDGE
                        // Francis add to prevent lead/lag transition miss judgement
                        if((ucpass_begin[dqs_i] == 1) && (ucpass_count_1[dqs_i]*ucDQS_GW_FINE_STEP > 32)) //Gating window found > 1UI
                        {
                            if(u1DQS_high[dqs_i]==0)
                            {
                                mcSHOW_DBG_MSG(("  [Byte %d] Window > 1UI", dqs_i));
                            }

                            if((u1DQS_lead[dqs_i]==1) && (u1DQS_lag[dqs_i]==1))
                            {
                                u1DQS_high[dqs_i] ++;
                                //mcSHOW_DBG_MSG2(("[Byte %d] Lead/lag falling high (%d)\n", dqs_i,u1DQS_high[dqs_i]));
                            }

                            if(u1DQS_high[dqs_i] *ucDQS_GW_FINE_STEP >16)//>16 PI prevent glitch
                            {
                                if((u1DQS_lead[dqs_i]==1) && (u1DQS_lag[dqs_i]==1))
                                {
                                    ucdly_coarse_large_leadLag[dqs_i] = ucdly_coarse_large;
                                    ucdly_coarse_0p5T_leadLag[dqs_i] = ucdly_coarse_0p5T;
                                    ucdly_fine_tune_leadLag[dqs_i] = ucdly_fine_xT;
                                    u1DQS_transition[dqs_i] =1;
                                }
                                else if(((u1DQS_lead[dqs_i]==1) && (u1DQS_lag[dqs_i]==0)) ||((u1DQS_lead[dqs_i]==0) && (u1DQS_lag[dqs_i]==1)))
                                {
                                    #if 0
                                    if(u1DQS_transition[dqs_i] ==1)
                                    {
                                        mcSHOW_DBG_MSG(("[Byte %d] Lead/lag falling Transition (%d, %d, %d)\n", dqs_i,ucdly_coarse_large_leadLag[dqs_i], ucdly_coarse_0p5T_leadLag[dqs_i], ucdly_fine_tune_leadLag[dqs_i]));
                                    }
                                    #endif
                                    u1DQS_transition[dqs_i] ++;
                                }
                                else if((u1DQS_lead[dqs_i]==0) && (u1DQS_lag[dqs_i]==0))
                                {
                                    mcSHOW_DBG_MSG(("  [Byte %d] Lead/lag tap(%d)", dqs_i, u1DQS_transition[dqs_i]));
                                    //u1DQSGatingFound[dqs_i] =1;
                                    u1PassByteCount  |= (1<<dqs_i);
                                    u1DQS_high[dqs_i] =0;
                                }
                            }
                        } 
#endif
                    }
                    else // current tap is fail
                    {
                        if (ucpass_begin[dqs_i]==1)
                        {
                            //at the end of pass range
                            ucpass_begin[dqs_i] = 0;
                            #if 0
                            if(p->dram_type==TYPE_LPDDR3) 
                            {
                                //save the max range settings, to avoid glitch
                                if (ucpass_count_1[dqs_i] > ucpass_count[dqs_i])
                                {
                                    ucmin_coarse_tune2T[dqs_i] = ucmin_coarse_tune2T_1[dqs_i];
                                    ucmin_coarse_tune0p5T[dqs_i] = ucmin_coarse_tune0p5T_1[dqs_i];
                                    ucmin_fine_tune[dqs_i] = ucmin_fine_tune_1[dqs_i];
                                    ucpass_count[dqs_i] = ucpass_count_1[dqs_i];

                                    /*TINFO="[Byte %d]Bigger pass win(%d, %d, %d)  Pass tap=%d\n", \
                                        dqs_i, ucmin_coarse_tune2T_1[dqs_i], ucmin_coarse_tune0p5T_1[dqs_i], ucmin_fine_tune_1[dqs_i], ucpass_count_1[dqs_i]*/
                                    mcSHOW_DBG_MSG(("  [Byte %d]Bigger pass win(%d, %d, %d)  Pass tap=%d", \
                                        dqs_i, ucmin_coarse_tune2T_1[dqs_i], ucmin_coarse_tune0p5T_1[dqs_i], ucmin_fine_tune_1[dqs_i], ucpass_count_1[dqs_i]));

                                    // LP3 pass window around 2 UI(pause mode), set 1~3 UI is pass
                                    if((ucpass_count_1[dqs_i]*ucDQS_GW_FINE_STEP > 32) && (ucpass_count_1[dqs_i]*ucDQS_GW_FINE_STEP < 96))
                                    {
                                        u1PassByteCount  |= (1<<dqs_i);
                                    }
                                }
                            }
                            #endif
                        }
                    }

                    if(u1PassByteCount==0x3)
                    {
                        mcSHOW_DBG_MSG2(("\nAll bytes gating window pass Done, Early break!\n"));
                        mcFPRINTF((fp_A60501, "\nAll bytes gating window pass Done, Early break!\n"));
                        ucdly_fine_xT = DQS_GW_FINE_END;//break loop
                        ucCoarseTune = ucCoarseEnd;      //break loop
                    }
                }
            
                mcSHOW_DBG_MSG(("\n"));
            }
        }

        DramcEngine2End(p);

        #if !SW_CHANGE_FOR_SIMULATION
        vSetCalibrationResult(p, DRAM_CALIBRATION_GATING, DRAM_OK);
        #endif

        //check if there is no pass taps for each DQS
        for (dqs_i=0; dqs_i<(p->data_width/DQS_BIT_NUMBER); dqs_i++)
        {
            if (ucpass_count[dqs_i]==0)
            {
                #if LP4_GATING_LEAD_LAG_FLAG_JUDGE==0
                /*TINFO="error, no pass taps in DQS_%d !!!\n", dqs_i*/
                mcSHOW_ERR_MSG(("error, no pass taps in DQS_%d!\n", dqs_i));
                mcFPRINTF((fp_A60501, "error, no pass taps in DQS_%d!\n", dqs_i));
                #if !SW_CHANGE_FOR_SIMULATION
                vSetCalibrationResult(p, DRAM_CALIBRATION_GATING, DRAM_FAIL);
                #endif
                #endif
            }
        }
    }

    
    //find center of each byte
    for (dqs_i=0; dqs_i<(p->data_width/DQS_BIT_NUMBER); dqs_i++)
    {
#if SW_CHANGE_FOR_SIMULATION  // simulation cannot support %
        // -- PI for Phase0 & Phase1 --
        uctmp_offset = ucpass_count[dqs_i]*ucDQS_GW_FINE_STEP/2;
        uctmp_value = ucmin_fine_tune[dqs_i]+uctmp_offset;
        ucbest_fine_tune[dqs_i] = uctmp_value - (uctmp_value/ucRX_DLY_DQSIENSTB_LOOP) * ucRX_DLY_DQSIENSTB_LOOP;
        ucbest_fine_tune_P1[dqs_i] = ucbest_fine_tune[dqs_i];

        // coarse tune 0.5T for Phase 0
        uctmp_offset = uctmp_value / ucRX_DLY_DQSIENSTB_LOOP;
        uctmp_value = ucmin_coarse_tune0p5T[dqs_i]+uctmp_offset;
        ucbest_coarse_tune0p5T[dqs_i] = uctmp_value - (uctmp_value/ucRX_DQS_CTL_LOOP) * ucRX_DQS_CTL_LOOP;

        // coarse tune 2T for Phase 0
        uctmp_offset = uctmp_value/ucRX_DQS_CTL_LOOP;
        ucbest_coarse_tune2T[dqs_i] = ucmin_coarse_tune2T[dqs_i]+uctmp_offset;

        // coarse tune 0.5T for Phase 1
        uctmp_value = ucbest_coarse_tune0p5T[dqs_i]+ ucFreqDiv;
        ucbest_coarse_tune0p5T_P1[dqs_i] = uctmp_value - (uctmp_value/ ucRX_DQS_CTL_LOOP) *ucRX_DQS_CTL_LOOP;

        // coarse tune 2T for Phase 1
        uctmp_offset = uctmp_value/ucRX_DQS_CTL_LOOP;
        ucbest_coarse_tune2T_P1[dqs_i] = ucbest_coarse_tune2T[dqs_i]+uctmp_offset;
#else

        #if (SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_GatingCal)
        if(p->femmc_Ready==1)
        {
            //@Darren, Fixed fastK WA for gating (temp)
            DramcEngine2Init(p, 0x55000000, 0xaa000000 |GATING_PATTERN_NUM_LP4, TEST_AUDIO_PATTERN, 0);
            // enable TE2, audio pattern
            DramcEngine2Run(p, TE_OP_READ_CHECK, TEST_AUDIO_PATTERN);
            DramcEngine2End(p);

            ucmin_coarse_tune2T[dqs_i]=p->pSavetimeData->u1Gating2T_Save[p->channel][p->rank][dqs_i];
            ucmin_coarse_tune0p5T[dqs_i]=p->pSavetimeData->u1Gating05T_Save[p->channel][p->rank][dqs_i];
            ucmin_fine_tune[dqs_i]=p->pSavetimeData->u1Gatingfine_tune_Save[p->channel][p->rank][dqs_i];
            ucpass_count[dqs_i]=p->pSavetimeData->u1Gatingucpass_count_Save[p->channel][p->rank][dqs_i];
            vSetCalibrationResult(p, DRAM_CALIBRATION_GATING, DRAM_OK);
        }
        else
        #endif
        {
            #if LP4_GATING_LEAD_LAG_FLAG_JUDGE
            ucpass_count[dqs_i] = u1DQS_transition[dqs_i];
            ucmin_fine_tune[dqs_i] = ucdly_fine_tune_leadLag[dqs_i];
            ucmin_coarse_tune0p5T[dqs_i] = ucdly_coarse_0p5T_leadLag[dqs_i];
            ucmin_coarse_tune2T[dqs_i] = ucdly_coarse_large_leadLag[dqs_i];
            #endif
        }

        #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(p->femmc_Ready==0)
        {
            p->pSavetimeData->u1Gating2T_Save[p->channel][p->rank][dqs_i]=ucmin_coarse_tune2T[dqs_i];
            p->pSavetimeData->u1Gating05T_Save[p->channel][p->rank][dqs_i]=ucmin_coarse_tune0p5T[dqs_i];
            p->pSavetimeData->u1Gatingfine_tune_Save[p->channel][p->rank][dqs_i]=ucmin_fine_tune[dqs_i];
            p->pSavetimeData->u1Gatingucpass_count_Save[p->channel][p->rank][dqs_i]=ucpass_count[dqs_i];
            // mcSHOW_DBG_MSG(("save Gating\n"));
        }
        #endif

        // -- PI for Phase0 & Phase1 --
        #if ENABLE_DDR800_OPEN_LOOP_MODE_OPTION
        if (u1IsDDR800PhaseMode(p) == TRUE)
            uctmp_offset = ucDQS_GW_FINE_STEP;
        else
        #endif
            uctmp_offset = ucpass_count[dqs_i]*ucDQS_GW_FINE_STEP/2;

        uctmp_value = ucmin_fine_tune[dqs_i]+uctmp_offset;
        ucbest_fine_tune[dqs_i] = uctmp_value% ucRX_DLY_DQSIENSTB_LOOP;
        ucbest_fine_tune_P1[dqs_i] = ucbest_fine_tune[dqs_i];

        // coarse tune 0.5T for Phase 0
        uctmp_offset = uctmp_value / ucRX_DLY_DQSIENSTB_LOOP;
        uctmp_value = ucmin_coarse_tune0p5T[dqs_i]+uctmp_offset;
        ucbest_coarse_tune0p5T[dqs_i] = uctmp_value% ucRX_DQS_CTL_LOOP;

        // coarse tune 2T for Phase 0
        uctmp_offset = uctmp_value/ucRX_DQS_CTL_LOOP;
        ucbest_coarse_tune2T[dqs_i] = ucmin_coarse_tune2T[dqs_i]+uctmp_offset;

        // coarse tune 0.5T for Phase 1
        uctmp_value = ucbest_coarse_tune0p5T[dqs_i]+ ucFreqDiv;
        ucbest_coarse_tune0p5T_P1[dqs_i] = uctmp_value% ucRX_DQS_CTL_LOOP;

        // coarse tune 2T for Phase 1
        uctmp_offset = uctmp_value/ucRX_DQS_CTL_LOOP;
        ucbest_coarse_tune2T_P1[dqs_i] = ucbest_coarse_tune2T[dqs_i]+uctmp_offset;
#endif
    }

    mcSHOW_DBG_MSG3(("\n\tdqs input gating window, final dly value\n\n"));
    mcFPRINTF((fp_A60501, "\n\tdqs input gating window, final dly value\n\n"));

    for (dqs_i=2; dqs_i<4; dqs_i++)  // LP4, DQ byte 2 and byte 3 are useless, set gating result as 0.
    {
        ucbest_coarse_tune2T[dqs_i] = ucbest_coarse_tune0p5T[dqs_i] = ucbest_fine_tune[dqs_i]= 0;
        ucbest_coarse_tune2T_P1[dqs_i] = ucbest_coarse_tune0p5T_P1[dqs_i] = ucbest_fine_tune_P1[dqs_i]= 0;

        #if GATING_ADJUST_TXDLY_FOR_TRACKING
        ucbest_coarse_tune2T_backup[p->rank][dqs_i] = ucbest_coarse_tune0p5T_backup[p->rank][dqs_i] = 0;
        ucbest_coarse_tune2T_P1_backup[p->rank][dqs_i] = ucbest_coarse_tune0p5T_P1_backup[p->rank][dqs_i] = 0;
        #endif
    }

    for (dqs_i=0; dqs_i<(p->data_width/DQS_BIT_NUMBER); dqs_i++)
    {
        #ifdef FOR_HQA_REPORT_USED
        HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT6, "DQSINCTL ", "", 0, u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQSCTL), SHURK0_DQSCTL_DQSINCTL), NULL);
        HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT0, "Gating_Center_", "2T", dqs_i, ucbest_coarse_tune2T[dqs_i], NULL);
        HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT0, "Gating_Center_", "05T", dqs_i, ucbest_coarse_tune0p5T[dqs_i], NULL);
        HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT0, "Gating_Center_", "PI", dqs_i, ucbest_fine_tune[dqs_i], NULL);
        #endif

        /*TINFO="best DQS%d delay(2T, 0.5T, PI) = (%d, %d, %d)\n", dqs_i, ucbest_coarse_tune2T[dqs_i], ucbest_coarse_tune0p5T[dqs_i], ucbest_fine_tune[dqs_i]*/
        mcSHOW_DBG_MSG(("best DQS%d dly(2T, 0.5T, PI) = (%d, %d, %d)\n", dqs_i, ucbest_coarse_tune2T[dqs_i], ucbest_coarse_tune0p5T[dqs_i], ucbest_fine_tune[dqs_i]));
        mcFPRINTF((fp_A60501,"best DQS%d dly(2T, 0.5T, PI) = (%d, %d, %d)\n", dqs_i, ucbest_coarse_tune2T[dqs_i], ucbest_coarse_tune0p5T[dqs_i], ucbest_fine_tune[dqs_i]));
        
        #if GATING_ADJUST_TXDLY_FOR_TRACKING
        // find min gating TXDLY (should be in P0)
        if (vGet_Div_Mode(p) == DIV8_MODE)
        {
            u1TX_dly_DQSgated =ucbest_coarse_tune2T[dqs_i];
        }
        else // LPDDR4 for 1:4 mode and LPDDR3
        {
            u1TX_dly_DQSgated  = ((ucbest_coarse_tune2T[dqs_i] <<1)|((ucbest_coarse_tune0p5T[dqs_i] >>2)&0x1));
        }

        if(u1TX_dly_DQSgated < u1TXDLY_Cal_min)
            u1TXDLY_Cal_min = u1TX_dly_DQSgated;

        ucbest_coarse_tune0p5T_backup[p->rank][dqs_i] = ucbest_coarse_tune0p5T[dqs_i];
        ucbest_coarse_tune2T_backup[p->rank][dqs_i] = ucbest_coarse_tune2T[dqs_i];
        #endif
    }

    mcSHOW_DBG_MSG(("\n"));
    mcFPRINTF((fp_A60501,"\n"));

    for (dqs_i=0; dqs_i<(p->data_width/DQS_BIT_NUMBER); dqs_i++)
    {
        /*TINFO="best DQS%d P1 delay(2T, 0.5T, PI) = (%d, %d, %d)\n", dqs_i, ucbest_coarse_tune2T_P1[dqs_i], ucbest_coarse_tune0p5T_P1[dqs_i], ucbest_fine_tune[dqs_i]*/
        mcSHOW_DBG_MSG(("best DQS%d P1 dly(2T, 0.5T, PI) = (%d, %d, %d)\n", dqs_i, ucbest_coarse_tune2T_P1[dqs_i], ucbest_coarse_tune0p5T_P1[dqs_i], ucbest_fine_tune[dqs_i]));
        mcFPRINTF((fp_A60501,"best DQS%d P1 dly(2T, 0.5T, PI) = (%d, %d, %d)\n", dqs_i, ucbest_coarse_tune2T_P1[dqs_i], ucbest_coarse_tune0p5T_P1[dqs_i], ucbest_fine_tune[dqs_i]));

        #if GATING_ADJUST_TXDLY_FOR_TRACKING
        // find max gating TXDLY (should be in P1)

        if (vGet_Div_Mode(p) == DIV8_MODE)
        {
            u1TX_dly_DQSgated  = ucbest_coarse_tune2T_P1[dqs_i];
        }
        else // LPDDR4 for 1:4 mode and LPDDR3
        {
            u1TX_dly_DQSgated  = ((ucbest_coarse_tune2T_P1[dqs_i] <<1)|((ucbest_coarse_tune0p5T_P1[dqs_i] >>2)&0x1));
        }

        if(u1TX_dly_DQSgated > u1TXDLY_Cal_max)
            u1TXDLY_Cal_max = u1TX_dly_DQSgated;

        ucbest_coarse_tune0p5T_P1_backup[p->rank][dqs_i] = ucbest_coarse_tune0p5T_P1[dqs_i];
        ucbest_coarse_tune2T_P1_backup[p->rank][dqs_i] = ucbest_coarse_tune2T_P1[dqs_i];
        #endif
    }

    mcSHOW_DBG_MSG(("\n"));
    mcFPRINTF((fp_A60501,"\n"));

    //Restore registers
    DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));

    /* LP3/4: Set ARDQ_RPRE_TOG_EN must be 1 after gating window calibration */
    RxDQSIsiPulseCG(p, ENABLE);

    #if REG_SHUFFLE_REG_CHECK
    ShuffleRegCheck =1;
    #endif

    // 4T or 2T coarse tune
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG0), \
                                P_Fld((U32) ucbest_coarse_tune2T[0], SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED)| \
                                P_Fld((U32) ucbest_coarse_tune2T[1], SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)| \
                                P_Fld((U32) ucbest_coarse_tune2T[2], SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED)| \
                                P_Fld((U32) ucbest_coarse_tune2T[3], SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED)| \
                                P_Fld((U32) ucbest_coarse_tune2T_P1[0], SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)| \
                                P_Fld((U32) ucbest_coarse_tune2T_P1[1], SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)| \
                                P_Fld((U32) ucbest_coarse_tune2T_P1[2], SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED_P1)| \
                                P_Fld((U32) ucbest_coarse_tune2T_P1[3], SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED_P1));

    // 0.5T coarse tune
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG1), \
                                P_Fld((U32) ucbest_coarse_tune0p5T[0], SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED)| \
                                P_Fld((U32) ucbest_coarse_tune0p5T[1], SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)| \
                                P_Fld((U32) ucbest_coarse_tune0p5T[2], SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED)| \
                                P_Fld((U32) ucbest_coarse_tune0p5T[3], SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED)| \
                                P_Fld((U32) ucbest_coarse_tune0p5T_P1[0], SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)| \
                                P_Fld((U32) ucbest_coarse_tune0p5T_P1[1], SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)| \
                                P_Fld((U32) ucbest_coarse_tune0p5T_P1[2], SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED_P1)| \
                                P_Fld((U32) ucbest_coarse_tune0p5T_P1[3], SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED_P1));


#if GATING_RODT_LATANCY_EN    //LP3 RODT is not enable, don't need to set the RODT settings.
    // RODT = Gating - 11UI,
    for (dqs_i=0; dqs_i<(p->data_width/DQS_BIT_NUMBER); dqs_i++)
    {
        uctmp_value = (ucbest_coarse_tune2T[dqs_i] <<3)+ucbest_coarse_tune0p5T[dqs_i];

        if(uctmp_value>=11)
        {
            //P0
            uctmp_value -=11;
            ucbest_coarse_large_RODT[dqs_i] = uctmp_value >>3;
            ucbest_coarse_0p5T_RODT[dqs_i]  = uctmp_value - (ucbest_coarse_large_RODT[dqs_i] <<3);

            //P1
            uctmp_value = (ucbest_coarse_tune2T_P1[dqs_i] <<3)+ucbest_coarse_tune0p5T_P1[dqs_i] -11;
            ucbest_coarse_large_RODT_P1[dqs_i] = uctmp_value >>3;
            ucbest_coarse_0p5T_RODT_P1[dqs_i]  = uctmp_value - (ucbest_coarse_large_RODT_P1[dqs_i] <<3);

            mcSHOW_DBG_MSG2(("\nbest RODT dly(2T, 0.5T) = (%d, %d)\n", ucbest_coarse_large_RODT[dqs_i], ucbest_coarse_0p5T_RODT[dqs_i]));
            mcFPRINTF((fp_A60501,"best RODT dly(2T, 0.5T) = (%d, %d)\n", ucbest_coarse_large_RODT[dqs_i], ucbest_coarse_0p5T_RODT[dqs_i]));
        }
        else //if(ucbest_coarse_tune2T[0] ==0)  //shouble not happen,  just only protect this happen
        {
            //P0
            ucbest_coarse_large_RODT[dqs_i] =0;
            ucbest_coarse_0p5T_RODT[dqs_i] = 0;
            //P1
            ucbest_coarse_large_RODT_P1[dqs_i] =4;
            ucbest_coarse_0p5T_RODT_P1[dqs_i] = 4;

            mcSHOW_ERR_MSG(("[RxdqsGatingCal] Error: ucbest_coarse_tune2T[%d] is already 0. RODT cannot be -1 UI\n", dqs_i));
            mcFPRINTF((fp_A60501, "[RxdqsGatingCal] Error: ucbest_coarse_tune2T[%d] is already 0. RODT cannot be -1 UI\n", dqs_i));
        }
    }

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_ODTEN0), \
                                P_Fld((U32) ucbest_coarse_large_RODT[0], SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN)| \
                                P_Fld((U32) ucbest_coarse_large_RODT[1], SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN)| \
                                P_Fld((U32) ucbest_coarse_large_RODT[2], SHURK0_SELPH_ODTEN0_TXDLY_B2_RODTEN)| \
                                P_Fld((U32) ucbest_coarse_large_RODT[3], SHURK0_SELPH_ODTEN0_TXDLY_B3_RODTEN)| \
                                P_Fld((U32) ucbest_coarse_large_RODT_P1[0], SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN_P1)| \
                                P_Fld((U32) ucbest_coarse_large_RODT_P1[1], SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN_P1)| \
                                P_Fld((U32) ucbest_coarse_large_RODT_P1[2], SHURK0_SELPH_ODTEN0_TXDLY_B2_RODTEN_P1)| \
                                P_Fld((U32) ucbest_coarse_large_RODT_P1[3], SHURK0_SELPH_ODTEN0_TXDLY_B3_RODTEN_P1));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_ODTEN1), \
                                P_Fld((U32) ucbest_coarse_0p5T_RODT[0], SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN)| \
                                P_Fld((U32) ucbest_coarse_0p5T_RODT[1], SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN)| \
                                P_Fld((U32) ucbest_coarse_0p5T_RODT[2], SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN)| \
                                P_Fld((U32) ucbest_coarse_0p5T_RODT[3], SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN)| \
                                P_Fld((U32) ucbest_coarse_0p5T_RODT_P1[0], SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN_P1)| \
                                P_Fld((U32) ucbest_coarse_0p5T_RODT_P1[1], SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN_P1)| \
                                P_Fld((U32) ucbest_coarse_0p5T_RODT_P1[2], SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN_P1)| \
                                P_Fld((U32) ucbest_coarse_0p5T_RODT_P1[3], SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN_P1));
#endif

    // Set Fine Tune Value to registers
    #if ENABLE_DDR800_OPEN_LOOP_MODE_OPTION
    if (u1IsDDR800PhaseMode(p) == TRUE)
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ7), ucbest_fine_tune[0]>>4, SHU_R0_B0_DQ7_DA_ARPI_DDR400_0D5UI_RK0_B0); // Divide by 16 (90 degree)
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ7), ucbest_fine_tune[1]>>4, SHU_R0_B1_DQ7_DA_ARPI_DDR400_0D5UI_RK0_B1); // Divide by 16 (90 degree)
    }
    else
    #endif
    {
        u4value = ucbest_fine_tune[0] | (ucbest_fine_tune[1]<<8) | (ucbest_fine_tune[2]<<16) | (ucbest_fine_tune[3]<<24);
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQSIEN), u4value);
    }

#if RDSEL_TRACKING_EN				
	//Byte 0
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_INI_UIPI), (ucbest_coarse_tune2T_backup[p->rank][0] << 3) | (ucbest_coarse_tune0p5T_backup[p->rank][0]), SHURK0_INI_UIPI_RG_INI_UI_B0R0);//UI
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_INI_UIPI), ucbest_fine_tune[0], SHURK0_INI_UIPI_RG_INI_PI_B0R0); //PI
	//Byte 1
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_INI_UIPI), (ucbest_coarse_tune2T_backup[p->rank][1] << 3) | (ucbest_coarse_tune0p5T_backup[p->rank][1]), SHURK0_INI_UIPI_RG_INI_UI_B1R0);//UI
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_INI_UIPI), ucbest_fine_tune[1], SHURK0_INI_UIPI_RG_INI_PI_B1R0); //PI
#endif

#if REG_SHUFFLE_REG_CHECK
    ShuffleRegCheck =0;
#endif

    //mcDELAY_US(1);//delay 2T
    //DramPhyCGReset(p, 0);
    DramPhyReset(p);   //reset phy, reset read data counter

    /*TINFO="[DramcRxdqsGatingCal] Done\n"*/
    mcSHOW_DBG_MSG3(("[DramcRxdqsGatingCal] Done\n\n"));
    mcFPRINTF((fp_A60501, "[DramcRxdqsGatingCal] Done\n\n"));

    return DRAM_OK;
// log example
/*
0   1  12 |(B3->B0) 0x   0, 0x1211, 0x1212, 0x1211 | (B3->B0) 11 11  11 11  11 11  11 11 | 0xffffffff
0   1  16 |(B3->B0) 0x   0, 0x1211, 0x1212, 0x1211 | (B3->B0) 11 11  11 11  11 11  11 11 | 0xffffffff
0   1  20 |(B3->B0) 0x   0, 0x1211, 0x1212, 0x1211 | (B3->B0) 11 11  11 11  11 11  11 11 | 0xffffffff
0   1  24 |(B3->B0) 0x   0, 0x1211, 0x1212, 0x1211 | (B3->B0) 11 11  11 11  11 11  11 11 | 0xffffffff
0   1  28 |(B3->B0) 0x   0, 0x1211, 0x1212, 0x1211 | (B3->B0) 11 11  11 11  11 11  11 11 | 0xffffffff
0   2   0 |(B3->B0) 0x   0, 0x1d1c, 0x1212, 0x1211 | (B3->B0) 11 11  11 11  11 11  11 11 | 0xffffffff
0   2   4 |(B3->B0) 0x   0, 0x2324, 0x1212, 0x1413 | (B3->B0) 11 11   0  0  11 11  11 11 | 0xffffffff
0   2   8 |(B3->B0) 0x   0, 0x2323, 0x1212, 0x2222 | (B3->B0)  0  0   0  0  11 11  11 11 | 0xff00ffff
0   2  12 |(B3->B0) 0x   0, 0x2323, 0x1211, 0x2323 | (B3->B0)  0  0   0  0  11 11   0  0 | 0x    ffff
0   2  16 |(B3->B0) 0x   0, 0x2323, 0x 504, 0x2324 | (B3->B0)  0  0   0  0  11 11   0  0 | 0x    ff00
0   2  20 |(B3->B0) 0x   0, 0x2323, 0x 303, 0x2323 | (B3->B0)  0  0   0  0   1  1   0  0 | 0x    ff00
0   2  24 |(B3->B0) 0x   0, 0x2323, 0x2324, 0x2323 | (B3->B0)  0  0   0  0   0  0   0  0 | 0x       0
0   2  28 |(B3->B0) 0x   0, 0x2323, 0x2323, 0x2323 | (B3->B0)  0  0   0  0   0  0   0  0 | 0x       0
0   3   0 |(B3->B0) 0x   0, 0x2323, 0x2323, 0x2323 | (B3->B0)  0  0   0  0   0  0   0  0 | 0x       0
0   3   4 |(B3->B0) 0x   0, 0x2323, 0x2323, 0x2323 | (B3->B0)  0  0   0  0   0  0   0  0 | 0x       0
0   3   8 |(B3->B0) 0x   0, 0x2323, 0x2323, 0x2323 | (B3->B0)  0  0   0  0   0  0   0  0 | 0x       0
0   3  12 |(B3->B0) 0x   0, 0x2323, 0x2323, 0x2323 | (B3->B0)  0  0   0  0   0  0   0  0 | 0x       0
0   3  16 |(B3->B0) 0x   0, 0x2323, 0x2323, 0x2323 | (B3->B0)  0  0   0  0   0  0   0  0 | 0x       0
0   3  20 |(B3->B0) 0x   0, 0x2323, 0x2323, 0x2323 | (B3->B0)  0  0   0  0   0  0   0  0 | 0x       0
0   3  24 |(B3->B0) 0x   0, 0x2323, 0x2323, 0x2323 | (B3->B0)  0  0   0  0   0  0   0  0 | 0x       0
0   3  28 |(B3->B0) 0x   0, 0x2323, 0x2323, 0x2323 | (B3->B0)  0  0   0  0   0  0   0  0 | 0x       0
1   0   0 |(B3->B0) 0x   0, 0x2120, 0x2323, 0x2323 | (B3->B0) 11 11  11 11   0  0   0  0 | 0xffff0000
1   0   4 |(B3->B0) 0x   0, 0x2323, 0x2323, 0x1212 | (B3->B0) 11 11   0  0   0  0  11 11 | 0xffff00ff
1   0   8 |(B3->B0) 0x   0, 0x2324, 0x2323, 0x2323 | (B3->B0)  0  0   0  0   0  0   0  0 | 0xffff00ff
1   0  12 |(B3->B0) 0x   0, 0x2323, 0x2323, 0x2323 | (B3->B0)  0  0   0  0   0  0   0  0 | 0xffff00ff
1   0  16 |(B3->B0) 0x   0, 0x2323, 0x1f1f, 0x2323 | (B3->B0)  0  0   0  0  11 11   0  0 | 0xffffffff
1   0  20 |(B3->B0) 0x   0, 0x2323, 0x2324, 0x2323 | (B3->B0)  0  0   0  0   0  0   0  0 | 0xffffffff
1   0  24 |(B3->B0) 0x   0, 0x2323, 0x2323, 0x2323 | (B3->B0)  0  0   0  0   0  0   0  0 | 0xffffffff
1   0  28 |(B3->B0) 0x   0, 0x2322, 0x2324, 0x2323 | (B3->B0) 11 11  11 11   0  0   0  0 | 0x    ffff
1   1   0 |(B3->B0) 0x   0, 0x2322, 0x2324, 0x2323 | (B3->B0) 11 11  11 11   0  0   0  0 | 0xffffffff
1   1   4 |(B3->B0) 0x   0, 0x2323, 0x2324, 0x2322 | (B3->B0) 11 11  11 11   0  0  11 11 | 0xffffff00
1   1   8 |(B3->B0) 0x   0, 0x2323, 0x2324, 0x2322 | (B3->B0) 11 11  11 11  11 11  11 11 | 0xffffffff
1   1  12 |(B3->B0) 0x   0, 0x2323, 0x2323, 0x2323 | (B3->B0) 11 11  11 11  11 11  11 11 | 0xffffffff
1   1  16 |(B3->B0) 0x   0, 0x2323, 0x2322, 0x2323 | (B3->B0) 11 11  11 11  11 11  11 11 | 0xffffffff
1   1  20 |(B3->B0) 0x   0, 0x2323, 0x2323, 0x2323 | (B3->B0) 11 11  11 11  11 11  11 11 | 0xffffffff
1   1  24 |(B3->B0) 0x   0, 0x2323, 0x2323, 0x2323 | (B3->B0) 11 11  11 11  11 11  11 11 | 0xffffffff
1   1  28 |(B3->B0) 0x   0, 0x2323, 0x2323, 0x2323 | (B3->B0) 11 11  11 11  11 11  11 11 | 0xffffffff
1   2   0 |(B3->B0) 0x   0, 0x1a1b, 0x2323, 0x2323 | (B3->B0) 11 11  11 11  11 11  11 11 | 0xffffffff
1   2   4 |(B3->B0) 0x   0, 0x1a1b, 0x2323, 0x1e1f | (B3->B0)  0  0  11 11  11 11   0  0 | 0xffffffff

===============================================================================
   dqs input gating widnow, final delay value
   channel=2(2:cha, 3:chb)
===============================================================================
test2_1: 0x55000000, test2_2: 0xaa000400, test pattern: 5
dqs input gating widnow, best delay value
===============================================================================
best DQS0 delay(2T, 0.5T, PI) = (0, 3, 12)
best DQS1 delay(2T, 0.5T, PI) = (0, 3, 22)
best DQS2 delay(2T, 0.5T, PI) = (0, 3, 4)
best DQS3 delay(2T, 0.5T, PI) = (0, 3, 4)
===============================================================================
best DQS0 P1 delay(2T, 0.5T, PI) = (1, 1, 12)
best DQS1 P1 delay(2T, 0.5T, PI) = (1, 1, 22)
best DQS2 P1 delay(2T, 0.5T, PI) = (1, 1, 4)
best DQS3 P1 delay(2T, 0.5T, PI) = (1, 1, 4)
===============================================================================
[DramcRxdqsGatingCal] ====Done====

*/
}

#if GATING_ADJUST_TXDLY_FOR_TRACKING
void DramcRxdqsGatingPostProcess(DRAMC_CTX_T *p)
{
    U8 dqs_i, u1RankRxDVS=0;
    U8 u1RankIdx, u1RankMax, u1RankBak;
    S8 s1ChangeDQSINCTL;
#if XRTRTR_NEW_CROSS_RK_MODE
    U16 u2PHSINCTL = 0;
    U32 u4Rank_Sel_MCK_P0[2], u4Rank_Sel_MCK_P1[2], u4RANKINCTL_STB;
#endif
#if RDSEL_TRACKING_EN
    U32 u4PI_value[2]={0};
#endif
    U32 backup_rank;
    U32 u4ReadDQSINCTL, u4ReadRODT, u4ReadTXDLY[RANK_MAX][DQS_NUMBER], u4ReadTXDLY_P1[RANK_MAX][DQS_NUMBER], u4RankINCTL_ROOT, u4XRTR2R, reg_TX_dly_DQSgated_min = 0;

    backup_rank = u1GetRank(p);

#ifdef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
    if (vGet_Div_Mode(p) == DIV8_MODE)
    {
        // wei-jen: DQSgated_min should be 2 when freq >= 1333, 1 when freq < 1333
        if (p->frequency >= 1333)
        {
            reg_TX_dly_DQSgated_min = 2;
        }
        else
        {
            reg_TX_dly_DQSgated_min = 1;
        }
    }
    else // for LPDDR4 1:4 mode
    {
        // 1866,1600,1333,1200  : reg_TX_dly_DQSgated (min) =2
        reg_TX_dly_DQSgated_min = 2;
    }
#else
    // wei-jen: DQSgated_min should be 3 when freq >= 1333, 2 when freq < 1333
    if (p->frequency >= 1333)
    {
        reg_TX_dly_DQSgated_min = 3;
    }
    else
    {
        reg_TX_dly_DQSgated_min = 2;
    }
#endif

    //Sylv_ia MP setting is switched to new mode, so RANKRXDVS can be set as 0 (review by HJ Huang)
#if 0
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ7), u1RankRxDVS, SHU_B0_DQ7_R_DMRANKRXDVS_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B1_DQ7), u1RankRxDVS, SHU_B1_DQ7_R_DMRANKRXDVS_B1);
#endif
    // === End of DVS setting =====

    s1ChangeDQSINCTL = reg_TX_dly_DQSgated_min- u1TXDLY_Cal_min;

    mcSHOW_DBG_MSG(("[RxdqsGatingPostProcess] freq %d\n"
                    "ChangeDQSINCTL %d, reg_TX_dly_DQSgated_min %d, u1TXDLY_Cal_min %d\n",
                        p->frequency,
                        s1ChangeDQSINCTL, reg_TX_dly_DQSgated_min, u1TXDLY_Cal_min));

    if(s1ChangeDQSINCTL!=0)  // need to change DQSINCTL and TXDLY of each byte
    {
        u1TXDLY_Cal_min += s1ChangeDQSINCTL;
        u1TXDLY_Cal_max += s1ChangeDQSINCTL;

        if (p->support_rank_num==RANK_DUAL)
            u1RankMax = RANK_MAX;
        else
             u1RankMax =RANK_1;

        for(u1RankIdx=0; u1RankIdx<u1RankMax; u1RankIdx++)
        {
            mcSHOW_DBG_MSG2(("Rank: %d\n", u1RankIdx));

            for (dqs_i=0; dqs_i<(p->data_width/DQS_BIT_NUMBER); dqs_i++)
            {
                if(vGet_Div_Mode(p) == DIV8_MODE)
                {
                    u4ReadTXDLY[u1RankIdx][dqs_i]= ucbest_coarse_tune2T_backup[u1RankIdx][dqs_i];
                    u4ReadTXDLY_P1[u1RankIdx][dqs_i]= ucbest_coarse_tune2T_P1_backup[u1RankIdx][dqs_i];

                    u4ReadTXDLY[u1RankIdx][dqs_i] += s1ChangeDQSINCTL;
                    u4ReadTXDLY_P1[u1RankIdx][dqs_i] += s1ChangeDQSINCTL;

                    ucbest_coarse_tune2T_backup[u1RankIdx][dqs_i] = u4ReadTXDLY[u1RankIdx][dqs_i];
                    ucbest_coarse_tune2T_P1_backup[u1RankIdx][dqs_i] = u4ReadTXDLY_P1[u1RankIdx][dqs_i];
                }
                else // LP3 or LP4 1:4 mode
                {
                    u4ReadTXDLY[u1RankIdx][dqs_i]= ((ucbest_coarse_tune2T_backup[u1RankIdx][dqs_i]<<1) + ((ucbest_coarse_tune0p5T_backup[u1RankIdx][dqs_i]>>2) & 0x1));
                    u4ReadTXDLY_P1[u1RankIdx][dqs_i]= ((ucbest_coarse_tune2T_P1_backup[u1RankIdx][dqs_i]<<1) + ((ucbest_coarse_tune0p5T_P1_backup[u1RankIdx][dqs_i]>>2) & 0x1));

                    u4ReadTXDLY[u1RankIdx][dqs_i] += s1ChangeDQSINCTL;
                    u4ReadTXDLY_P1[u1RankIdx][dqs_i] += s1ChangeDQSINCTL;

                    ucbest_coarse_tune2T_backup[u1RankIdx][dqs_i] = (u4ReadTXDLY[u1RankIdx][dqs_i] >>1);
                    ucbest_coarse_tune0p5T_backup[u1RankIdx][dqs_i] = ((u4ReadTXDLY[u1RankIdx][dqs_i] & 0x1) <<2)+(ucbest_coarse_tune0p5T_backup[u1RankIdx][dqs_i] & 0x3);

                    ucbest_coarse_tune2T_P1_backup[u1RankIdx][dqs_i] = (u4ReadTXDLY_P1[u1RankIdx][dqs_i] >>1);
                    ucbest_coarse_tune0p5T_P1_backup[u1RankIdx][dqs_i] = ((u4ReadTXDLY_P1[u1RankIdx][dqs_i] & 0x1)<<2) +(ucbest_coarse_tune0p5T_P1_backup[u1RankIdx][dqs_i] & 0x3);
                }
                mcSHOW_DBG_MSG(("best DQS%d dly(2T, 0.5T) = (%d, %d)\n", dqs_i, ucbest_coarse_tune2T_backup[u1RankIdx][dqs_i], ucbest_coarse_tune0p5T_backup[u1RankIdx][dqs_i]));
            }

            for (dqs_i=0; dqs_i<(p->data_width/DQS_BIT_NUMBER); dqs_i++)
            {
                mcSHOW_DBG_MSG(("best DQS%d P1 dly(2T, 0.5T) = (%d, %d)\n", dqs_i, ucbest_coarse_tune2T_P1_backup[u1RankIdx][dqs_i], ucbest_coarse_tune0p5T_P1_backup[u1RankIdx][dqs_i]));
            }
        }

        for(u1RankIdx=0; u1RankIdx<u1RankMax; u1RankIdx++)
        {
            vSetRank(p, u1RankIdx);
            // 4T or 2T coarse tune
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG0), \
                                    P_Fld((U32) ucbest_coarse_tune2T_backup[u1RankIdx][0], SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED)| \
                                    P_Fld((U32) ucbest_coarse_tune2T_backup[u1RankIdx][1], SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)| \
                                    P_Fld((U32) ucbest_coarse_tune2T_backup[u1RankIdx][2], SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED)| \
                                    P_Fld((U32) ucbest_coarse_tune2T_backup[u1RankIdx][3], SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED)| \
                                    P_Fld((U32) ucbest_coarse_tune2T_P1_backup[u1RankIdx][0], SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)| \
                                    P_Fld((U32) ucbest_coarse_tune2T_P1_backup[u1RankIdx][1], SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)| \
                                    P_Fld((U32) ucbest_coarse_tune2T_P1_backup[u1RankIdx][2], SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED_P1)| \
                                    P_Fld((U32) ucbest_coarse_tune2T_P1_backup[u1RankIdx][3], SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED_P1));

                // 0.5T coarse tune
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG1), \
                                    P_Fld((U32) ucbest_coarse_tune0p5T_backup[u1RankIdx][0], SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED)| \
                                    P_Fld((U32) ucbest_coarse_tune0p5T_backup[u1RankIdx][1], SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)| \
                                    P_Fld((U32) ucbest_coarse_tune0p5T_backup[u1RankIdx][2], SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED)| \
                                    P_Fld((U32) ucbest_coarse_tune0p5T_backup[u1RankIdx][3], SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED)| \
                                    P_Fld((U32) ucbest_coarse_tune0p5T_P1_backup[u1RankIdx][0], SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)| \
                                    P_Fld((U32) ucbest_coarse_tune0p5T_P1_backup[u1RankIdx][1], SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)| \
                                    P_Fld((U32) ucbest_coarse_tune0p5T_P1_backup[u1RankIdx][2], SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED_P1)| \
                                    P_Fld((U32) ucbest_coarse_tune0p5T_P1_backup[u1RankIdx][3], SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED_P1));
        #if RDSEL_TRACKING_EN				
            //Byte 0
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_INI_UIPI), (ucbest_coarse_tune2T_backup[u1RankIdx][0] << 3) | (ucbest_coarse_tune0p5T_backup[u1RankIdx][0]), SHURK0_INI_UIPI_RG_INI_UI_B0R0);//UI
            //Byte 1
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_INI_UIPI), (ucbest_coarse_tune2T_backup[u1RankIdx][1] << 3) | (ucbest_coarse_tune0p5T_backup[u1RankIdx][1]), SHURK0_INI_UIPI_RG_INI_UI_B1R0);//UI
        #endif
        }
    }
    vSetRank(p, backup_rank);

    u4ReadDQSINCTL = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQSCTL), SHURK0_DQSCTL_DQSINCTL);
    mcSHOW_DBG_MSG(("u4ReadDQSINCTL=%d\n", u4ReadDQSINCTL)); // for OO validation tools
    u4ReadDQSINCTL -= s1ChangeDQSINCTL;

    #if ENABLE_READ_DBI
    if(p->DBI_R_onoff[p->dram_fsp])
    {
      u4ReadDQSINCTL++;
      u4ReadRODT = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_ODTCTRL), SHU_ODTCTRL_RODT);
      vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_ODTCTRL), u4ReadRODT+1, SHU_ODTCTRL_RODT); //update RODT value when READ_DBI is on
    }
    #endif

#if XRTRTR_NEW_CROSS_RK_MODE
		for (dqs_i=0; dqs_i<(p->data_width/DQS_BIT_NUMBER); dqs_i++)
		{
			if(ucbest_coarse_tune2T_backup[RANK_0][dqs_i] > ucbest_coarse_tune2T_backup[RANK_1][dqs_i])
			{
				u4Rank_Sel_MCK_P0[dqs_i] = (ucbest_coarse_tune2T_backup[RANK_0][dqs_i] > 0)? (ucbest_coarse_tune2T_backup[RANK_0][dqs_i] - 1) : 0;
				u4Rank_Sel_MCK_P1[dqs_i] = (ucbest_coarse_tune2T_P1_backup[RANK_0][dqs_i] > 0)? (ucbest_coarse_tune2T_P1_backup[RANK_0][dqs_i] - 1) : 0;
			}
			else
			{
				u4Rank_Sel_MCK_P0[dqs_i] = (ucbest_coarse_tune2T_backup[RANK_1][dqs_i] > 0)? (ucbest_coarse_tune2T_backup[RANK_1][dqs_i] - 1) : 0;
				u4Rank_Sel_MCK_P1[dqs_i] = (ucbest_coarse_tune2T_P1_backup[RANK_1][dqs_i] > 0)? (ucbest_coarse_tune2T_P1_backup[RANK_1][dqs_i] - 1) : 0;
			}
		}
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_RANKSEL_MCK), P_Fld(u4Rank_Sel_MCK_P0[0], SHU_SELPH_RANKSEL_MCK_TX_DLY_B0_RANKSEL_P0) 
																| P_Fld(u4Rank_Sel_MCK_P1[0], SHU_SELPH_RANKSEL_MCK_TX_DLY_B0_RANKSEL_P1) 
																| P_Fld(u4Rank_Sel_MCK_P0[1], SHU_SELPH_RANKSEL_MCK_TX_DLY_B1_RANKSEL_P0) 
																| P_Fld(u4Rank_Sel_MCK_P1[1], SHU_SELPH_RANKSEL_MCK_TX_DLY_B1_RANKSEL_P1));  
		
		u4RANKINCTL_STB = (u4ReadDQSINCTL > 2)? (u4ReadDQSINCTL-2) : 0;
		u2PHSINCTL = (u4ReadDQSINCTL == 0) ? 0 : (u4ReadDQSINCTL - 1); 
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_RANKCTL), u4RANKINCTL_STB, SHU_RANKCTL_RANKINCTL_STB); 
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_RANK_SEL_STB), u2PHSINCTL, SHU_RANK_SEL_STB_RANK_SEL_PHSINCTL);
#endif

#ifdef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
    // Wei-Jen: RANKINCTL_RXDLY = RANKINCTL = RankINCTL_ROOT = u4ReadDQSINCTL-2, if XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY enable
    // Wei-Jen: New algorithm : u4ReadDQSINCTL-2 >= 0
    if(u4ReadDQSINCTL>=2)
    {
        u4RankINCTL_ROOT = u4ReadDQSINCTL-2;
    }
    else
    {
        u4RankINCTL_ROOT=0;
        mcSHOW_ERR_MSG(("u4RankINCTL_ROOT <2, Please check\n"));
#if (__ETT__)
        while(1);
#endif
    }
#else
    //Modify for corner IC failed at HQA test XTLV
    if(u4ReadDQSINCTL>=3)
    {
        u4RankINCTL_ROOT = u4ReadDQSINCTL-3;
    }
    else
    {
        u4RankINCTL_ROOT=0;
        mcSHOW_ERR_MSG(("u4RankINCTL_ROOT <3, Risk for supporting 1066/RL8\n"));
    }
#endif

    //DQSINCTL
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQSCTL), u4ReadDQSINCTL, SHURK0_DQSCTL_DQSINCTL);  //Rank0 DQSINCTL
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK1_DQSCTL), u4ReadDQSINCTL, SHURK1_DQSCTL_R1DQSINCTL); //Rank1 DQSINCTL

    //No need to update RODT. If we update RODT, also need to update SELPH_ODTEN0_TXDLY
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_ODTCTRL), u4ReadDQSINCTL, SHU_ODTCTRL_RODT);           //RODT = DQSINCTL

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_RANKCTL), u4ReadDQSINCTL, SHU_RANKCTL_RANKINCTL_PHY);  //RANKINCTL_PHY = DQSINCTL
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_RANKCTL), u4RankINCTL_ROOT, SHU_RANKCTL_RANKINCTL);  //RANKINCTL= DQSINCTL -3
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_RANKCTL), u4RankINCTL_ROOT, SHU_RANKCTL_RANKINCTL_ROOT1);  //RANKINCTL_ROOT1= DQSINCTL -3

#ifdef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_RANKCTL), u4RankINCTL_ROOT, SHU_RANKCTL_RANKINCTL_RXDLY);

    u4XRTR2R= u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM_XRT), SHU_ACTIM_XRT_XRTR2R);

    mcSHOW_DBG_MSG2(("TX_dly_DQSgated check: min %d  max %d, ChangeDQSINCTL=%d\n", u1TXDLY_Cal_min, u1TXDLY_Cal_max, s1ChangeDQSINCTL));
    mcSHOW_DBG_MSG2(("DQSINCTL=%d, RANKINCTL=%d, u4XRTR2R=%d\n", u4ReadDQSINCTL, u4RankINCTL_ROOT, u4XRTR2R));
#else
    //XRTR2R=A-phy forbidden margin(6T) + reg_TX_dly_DQSgated (max) +Roundup(tDQSCKdiff/MCK+0.25MCK)+1(05T sel_ph margin)-1(forbidden margin overlap part)
    //Roundup(tDQSCKdiff/MCK+1UI) =1~2 all LP3 and LP4 timing
    //u4XRTR2R= 8 + u1TXDLY_Cal_max;  // 6+ u1TXDLY_Cal_max +2

    //Modify for corner IC failed at HQA test XTLV @ 3200MHz
    u4XRTR2R= 8 + u1TXDLY_Cal_max + 1;  // 6+ u1TXDLY_Cal_max +2
    if (u4XRTR2R > 12)
    {
        u4XRTR2R= 12;
        mcSHOW_ERR_MSG(("XRTR2R > 12, Max value is 12\n"));
    }
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM_XRT), u4XRTR2R, SHU_ACTIM_XRT_XRTR2R);

    mcSHOW_DBG_MSG2(("TX_dly_DQSgated check: min %d  max %d, ChangeDQSINCTL=%d\n", u1TXDLY_Cal_min, u1TXDLY_Cal_max, s1ChangeDQSINCTL));
    mcSHOW_DBG_MSG2(("DQSINCTL=%d, RANKINCTL=%d, u4XRTR2R=%d\n", u4ReadDQSINCTL, u4RankINCTL_ROOT, u4XRTR2R));
#endif

#if 0//ENABLE_RODT_TRACKING
    //Because Kibo+,WE2,Bianco,Vinson...or behind project support WDQS, they need to apply the correct new setting
    //The following 2 items are indepentent
    //1. if TX_WDQS on(by vendor_id) or p->odt_onoff = 1, ROEN/RODTE/RODTE2 = 1
    //2. if ENABLE_RODT_TRACKING on, apply new setting and RODTENSTB_TRACK_EN = ROEN
    // LP4 support only
    U8 u1ReadROEN;
    u1ReadROEN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_ODTCTRL), SHU_ODTCTRL_ROEN);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_RODTENSTB), P_Fld(0xff, SHU_RODTENSTB_RODTENSTB_EXT)|\
                                                            P_Fld(u1ReadROEN, SHU_RODTENSTB_RODTENSTB_TRACK_EN));
#endif

#ifdef XRTR2W_PERFORM_ENHANCE_RODTEN
    // LP4 support only
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_RODTENSTB), P_Fld(0x0fff, SHU_RODTENSTB_RODTENSTB_EXT)|\
                                                            P_Fld(1, SHU_RODTENSTB_RODTEN_P1_ENABLE)|\
                                                            P_Fld(1, SHU_RODTENSTB_RODTENSTB_TRACK_EN));
#endif


    vSetRank(p, backup_rank);


}
#endif


#if GATING_ADJUST_TXDLY_FOR_TRACKING
void DramcRxdqsGatingPreProcess(DRAMC_CTX_T *p)
{
    u1TXDLY_Cal_min =0xff;
    u1TXDLY_Cal_max=0;
}
#endif
#endif //SIMULATION_GATING

#if ENABLE_RDDQC_DIV4_WA
void DramcRxWinRDDQCWA(DRAMC_CTX_T *p)
{
    if (vGet_Div_Mode(p) == DIV4_MODE)
    {
        // for DIV4 MODE
        u1MR15Value = 0x0;
        u1MR20Value = 0x0;
        u1MR32Value = 0xaa;
        u1MR40Value = 0xaa;
    }
    else
    {
        // rollback to defaults for 1:8 mode
        u1MR15Value = 0x55;
        u1MR20Value = 0x55;
        u1MR32Value = 0x5a;
        u1MR40Value = 0x3c;
    }

    DramcModeRegWriteByRank(p, p->rank, 15, u1MR15Value);
    DramcModeRegWriteByRank(p, p->rank, 20, u1MR20Value);
    DramcModeRegWriteByRank(p, p->rank, 32, u1MR32Value);
    DramcModeRegWriteByRank(p, p->rank, 40, u1MR40Value);
    mcSHOW_DBG_MSG(("[DramcRxWinRDDQCWA] MR15=0x%x, MR20=0x%x, MR32=0x%x, MR40=0x%x\n", u1MR15Value, u1MR20Value, u1MR32Value, u1MR40Value));
}
#endif


//-------------------------------------------------------------------------
/** DramcRxWindowPerbitCal (v2 version)
 *  start the rx dqs perbit sw calibration.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL
 */
//-------------------------------------------------------------------------
// default RX vref is 0xe=14
static U32 DramcRxWinRDDQCInit(DRAMC_CTX_T *p)
{
    U8 *uiLPDDR_O1_Mapping;
    U16 temp_value=0;
    U8 MR_GOLDEN_MR15_GOLDEN_value=0, MR_GOLDEN_MR20_GOLDEN_value=0;
    int i;

    //U8 u1ReadDBIbak[2];
    //U32 u4MRS_reg_bak;

    // Disable Read DBI
    //u1ReadDBIbak[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ7), SHU_B0_DQ7_R_DMDQMDBI_SHU_B0);
    //u1ReadDBIbak[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B1_DQ7), SHU_B1_DQ7_R_DMDQMDBI_SHU_B1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ7),  0, SHU_B0_DQ7_R_DMDQMDBI_SHU_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B1_DQ7),  0, SHU_B1_DQ7_R_DMDQMDBI_SHU_B1);
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), 1, SPCMDCTRL_RDDQCDIS); Moved to "DramcRxWinRDDQCRun()"

    //u4MRS_reg_bak = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_MRS));
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1GetRank(p), MRS_MRSRK);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MPC_OPTION), 1, MPC_OPTION_MPCRKEN);

#if ENABLE_RDDQC_DIV4_WA
    if (vGet_Div_Mode(p) == DIV4_MODE)
    {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_MR_GOLDEN), 
            P_Fld(u1MR15Value, MR_GOLDEN_MR15_GOLDEN) | 
            P_Fld(u1MR20Value, MR_GOLDEN_MR20_GOLDEN) |
            P_Fld(u1MR32Value, MR_GOLDEN_MR32_GOLDEN) |
            P_Fld(u1MR40Value, MR_GOLDEN_MR40_GOLDEN));
    }
    else
#endif
    {
        uiLPDDR_O1_Mapping = (U8 *)uiLPDDR4_O1_Mapping_POP[p->channel];
    
        for(i=0; i<16; i++)
        {
            temp_value |= ((0x5555 >> i) & 0x1) << uiLPDDR_O1_Mapping[i];
        }
        MR_GOLDEN_MR15_GOLDEN_value = (U8) temp_value & 0xff;
        MR_GOLDEN_MR20_GOLDEN_value = (U8) (temp_value>>8) & 0xff;
    
        //Set golden pattern from Shih-hsiu's suggestion. 2016/3/25 04:43pm, RE: [Oly_mpus] RX per bit calibration.
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_MR_GOLDEN), 
                  P_Fld(MR_GOLDEN_MR15_GOLDEN_value, MR_GOLDEN_MR15_GOLDEN) 
                | P_Fld(MR_GOLDEN_MR20_GOLDEN_value, MR_GOLDEN_MR20_GOLDEN)
    #if ENABLE_RDDQC_DIV4_WA
                | P_Fld(u1MR32Value, MR_GOLDEN_MR32_GOLDEN)
                | P_Fld(u1MR40Value, MR_GOLDEN_MR40_GOLDEN)
    #endif
                );
    }

    //MPC_RDDQC_SET_MRS
    //Write MR32 DQ calibration pattern A //skip it if you want default 5Ah
    //Write MR40 DQ calibration pattern B //skip it if you want default 3Ch
    //Write MR15 low byte inverter for DQ calibration (byte 0) //skip it if you want default 55h
    //Write MR20 uper byte inverter for DQ calibration (byte 1) //skip it if you want default aah
    //DramcModeRegWrite(p, 32, 0x86);
    //DramcModeRegWrite(p, 40, 0xC9);
    //DramcModeRegWrite(p, 32, 0xaa);
    //DramcModeRegWrite(p, 32, 0xbb);
}

/* Issue "RD DQ Calibration"
 * 1. RDDQCEN = 1 for RDDQC
 * 2. RDDQCDIS = 1 to stop RDDQC burst
 * 3. Wait rddqc_response = 1
 * 4. Read compare result
 * 5. RDDQCEN = 0
 * 6. RDDQCDIS = 0 (Stops RDDQC request)
 */
static U32 DramcRxWinRDDQCRun(DRAMC_CTX_T *p)
{
    U32 u4Result, u4Response;
    U32 u4TimeCnt= TIME_OUT_CNT;

    //Issue RD DQ calibration
    //R_DMRDDQCEN, 0x1E4[7]=1 for RDDQC,  R_DMRDDQCDIS, 0x1EC[26]=1 to stop RDDQC burst
    //Wait rddqc_response=1, (dramc_conf_nao, 0x3b8[31])
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), 1, SPCMDCTRL_RDDQCDIS); // Fixed 1:4 mode RDDQC no window (SPCMDCTRL_RDDQCDIS=1) before SPCMD_RDDQCEN=1
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1, SPCMD_RDDQCEN);
    do
    {
        u4Response = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_RDDQC_RESPONSE);
        u4TimeCnt --;
        mcDELAY_US(1);
    }while((u4Response ==0) &&(u4TimeCnt>0));

    if(u4TimeCnt==0)//time out
    {
        mcSHOW_DBG_MSG(("[RxWinRDDQC] Resp fail (time out)\n"));
        mcFPRINTF((fp_A60501, "[RxWinRDDQC] Resp fail (time out)\n"));
        //return DRAM_FAIL;
    }

    //Then read RDDQC compare result (dramc_conf_nao, 0x36c)
    u4Result = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_RDQC_CMP));
    u4Result = (u4Result >> DQ_DATA_WIDTH_LP4) | (u4Result & 0xffff);
    //R_DMRDDQCEN, 0x1E4[7]=0
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_RDDQCEN);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), 0, SPCMDCTRL_RDDQCDIS);

    return u4Result;
}

static U32 DramcRxWinRDDQCEnd(DRAMC_CTX_T *p)
{
    //Recover Read DBI
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ7),  p->DBI_R_onoff, SHU_B0_DQ7_R_DMDQMDBI_SHU_B0);
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B1_DQ7),  p->DBI_R_onoff, SHU_B1_DQ7_R_DMDQMDBI_SHU_B1);

    // Recover MPC Rank
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), 0, MRS_MRSRK);
}

static void SetRxDqDelay(DRAMC_CTX_T *p, U8 ii, S16 iDelay)
{
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ2+ ii*4), \
                            P_Fld(iDelay,SHU_R0_B0_DQ2_RK0_RX_ARDQ0_R_DLY_B0) |P_Fld(iDelay,SHU_R0_B0_DQ2_RK0_RX_ARDQ1_R_DLY_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ2+ ii*4), \
                            P_Fld(iDelay,SHU_R0_B1_DQ2_RK0_RX_ARDQ0_R_DLY_B1) |P_Fld(iDelay,SHU_R0_B1_DQ2_RK0_RX_ARDQ1_R_DLY_B1));
}

static void SetRxDqDqsDelay(DRAMC_CTX_T *p, S16 iDelay)
{
    U8 ii, u1ByteIdx;
    U32 u4value;
    U8 dl_value[8];

    if (iDelay <=0)
    {
        // Set DQS delay
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ6), (-iDelay+gu2RX_DQS_Duty_Offset[0][0]), SHU_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ6), (-iDelay+gu2RX_DQS_Duty_Offset[1][0]), SHU_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1);
        DramPhyReset(p);
    }
    else
    {
        // Adjust DQM output delay.
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ6), iDelay, SHU_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ6), iDelay, SHU_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1);
        DramPhyReset(p);

        // Adjust DQ output delay.
        //u4value = ((U32) iDelay) | (((U32)iDelay)<<8) | (((U32)iDelay)<<16) | (((U32)iDelay)<<24);
        for (ii=0; ii<4; ii++)
        {
            SetRxDqDelay(p, ii, iDelay);
        }
    }
}



#if SIMULATION_RX_PERBIT
DRAM_STATUS_T DramcRxWindowPerbitCal(DRAMC_CTX_T *p, U8 u1UseTestEngine, U8 *u1AssignedVref)
{
    U8 ii, u1BitIdx, u1ByteIdx;
    U32 u1vrefidx;
    U8 ucbit_first, ucbit_last;
    S16 iDelay=0, u4DelayBegin=0, u4DelayEnd, u4DelayStep=1;
    S16 iDutyOffset=0, u4DutyOffsetBegin, u4DutyOffsetEnd, u4DutyOffsetStep=4;
    U32 uiFinishCount;
    U32 u4value, u4err_value, u4fail_bit;
    PASS_WIN_DATA_T WinPerBit[DQ_DATA_WIDTH], FinalWinPerBit[DQ_DATA_WIDTH];
    S32 iDQSDlyPerbyte[DQS_NUMBER], iDQMDlyPerbyte[DQS_NUMBER];//, iFinalDQSDly[DQS_NUMBER];
    U8 u1VrefScanEnable =0;
    U16 u2TempWinSum[2], u2rx_window_sum[2]={0}, u2TmpDQMSum;
    U16 u2VrefLevel, u2FinalVref[2]= {0xe, 0xe};
    U16 u2VrefBegin, u2VrefEnd, u2VrefStep;
    U32 u4fail_bit_R, u4fail_bit_F;
    U8  u1RXEyeScanEnable, u1PrintCalibrationProc;
    U8 u1min_bit_by_vref[2], u1min_winsize_by_vref[2];    
    U8 u1min_bit[2], u1min_winsize[2]={0};

    U8 dl_value[8]={0,0,0,0,0,0,0,0};
    U8 backup_rank, u1KnownVref[2]={0xff, 0xff};

#if 0//PPORT_SAVE_TIME_FOR_CALIBRATION
   S16 u1minfirst_pass=0xff,u1minlast_pass=0xff,u4Delayoffset;
   U8  u1AllBitPassCount;
#endif

#if ENABLE_EYESCAN_GRAPH
    U8 EyeScan_index[DQ_DATA_WIDTH_LP4] = {0};
    U8 u1pass_in_this_vref_flag[DQ_DATA_WIDTH_LP4];
#endif


#if REG_ACCESS_PORTING_DGB
    RegLogEnable =1;
    mcSHOW_DBG_MSG(("\n[REG_ACCESS_PORTING_FUNC] DramcRxWindowPerbitCal\n"));
    mcFPRINTF((fp_A60501, "\n[REG_ACCESS_PORTING_FUNC] DramcRxWindowPerbitCal\n"));
#endif

    // error handling
    if (!p)
    {
        mcSHOW_ERR_MSG(("context NULL\n"));
        return DRAM_FAIL;
    }

    backup_rank = u1GetRank(p);

    u1RXEyeScanEnable = (gRX_EYE_Scan_flag==1 && ((gRX_EYE_Scan_only_higheset_freq_flag==1 && p->frequency == u2DFSGetHighestFreq(p)) || gRX_EYE_Scan_only_higheset_freq_flag==0));

#if ENABLE_EYESCAN_GRAPH
    if (u1IsLP4Family(p->dram_type))
    {
        for(u1vrefidx=0; u1vrefidx<VREF_TOTAL_NUM_WITH_RANGE;u1vrefidx++)
        {
            for (u1BitIdx = 0; u1BitIdx < DQ_DATA_WIDTH_LP4; u1BitIdx++)
            {
                for(ii=0; ii<EYESCAN_BROKEN_NUM; ii++)
                {
                    gEyeScan_Min[u1vrefidx][u1BitIdx][ii] = EYESCAN_DATA_INVALID;
                    gEyeScan_Max[u1vrefidx][u1BitIdx][ii] = EYESCAN_DATA_INVALID;

                    gEyeScan_ContinueVrefHeight[u1BitIdx] = 0;
                    gEyeScan_TotalPassCount[u1BitIdx] = 0;                    
                }
            }
        }
    }
#endif


    //defult set result fail. When window found, update the result as oK
    if(u1UseTestEngine)
    {
        vSetCalibrationResult(p, DRAM_CALIBRATION_RX_PERBIT, DRAM_FAIL);
        DramcEngine2Init(p, p->test2_1, p->test2_2, p->test_pattern, 0);
    }
    else
    {
        vSetCalibrationResult(p, DRAM_CALIBRATION_RX_RDDQC, DRAM_FAIL);
        DramcRxWinRDDQCInit(p);
    }

    if(u1UseTestEngine==1)
    {
    #if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
        if ((p->rank==RANK_0) || (p->frequency >= RX_VREF_DUAL_RANK_K_FREQ) || (u1RXEyeScanEnable==1))
            u1VrefScanEnable =1;
    #else
            u1VrefScanEnable =0;
    #endif
    }

    u1PrintCalibrationProc = ((u1VrefScanEnable==0) || (u1RXEyeScanEnable==1) || (u1AssignedVref!=NULL));

    #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    if(p->femmc_Ready==1 && ((p->Bypass_RDDQC && u1UseTestEngine==0) || (p->Bypass_RXWINDOW && u1UseTestEngine==1)))
    {
        mcSHOW_DBG_MSG(("[FAST_K] Bypass RX Calibration\n"));
    }
    else
    #endif
    {
#if VENDER_JV_LOG
#if 0 //BU don't want customer knows our RX's ability
        if(u1UseTestEngine ==1)
            vPrintCalibrationBasicInfo_ForJV(p);
#endif
#else
        vPrintCalibrationBasicInfo(p);
#endif
        mcSHOW_DBG_MSG2(("Start DQ dly to find pass range UseTestEngine =%d\n", u1UseTestEngine));
        mcFPRINTF((fp_A60501, "Start RX DQ/DQS calibration UseTestEngine =%d\n", u1UseTestEngine));
    }
    
    mcSHOW_DBG_MSG(("RX Vref Scan = %d\n", u1VrefScanEnable));
    mcFPRINTF((fp_A60501, "RX Vref Scan= %d\n", u1VrefScanEnable));

    if(u1VrefScanEnable)
    {
        if(Get_Vref_Calibration_OnOff(p)==VREF_CALI_OFF)
        {
            u2VrefBegin = 0;
            u2VrefEnd = 0;
            u1KnownVref[0] =gFinalRXVrefDQForSpeedUp[p->channel][p->rank][p->odt_onoff][0];// byte 0
            u1KnownVref[1] =gFinalRXVrefDQForSpeedUp[p->channel][p->rank][p->odt_onoff][1];// byte 1
    
            if(u1UseTestEngine==1 && ((u1KnownVref[0] ==0) ||(u1KnownVref[1] ==0)))
            {
                mcSHOW_ERR_MSG(("\nWrong frequency K order=%d\n"));
                #if __ETT__
                while(1);
                #endif
            }
        }
        else if(u1AssignedVref!=NULL)  // need to specify RX Vref and don't scan RX Vref.
        {
            u2VrefBegin = 0;
            u2VrefEnd = 0;
            u1KnownVref[0] =u1AssignedVref[0];  // byte 0
            u1KnownVref[1] =u1AssignedVref[1];  // byte 1
        }
        else
        {
            #if (SW_CHANGE_FOR_SIMULATION ||FOR_DV_SIMULATION_USED)
            u2VrefBegin =RX_VREF_RANGE_BEGIN;
            #else
            if (u1RXEyeScanEnable==0)
            {
                if(p->odt_onoff)
                {
                    u2VrefBegin = RX_VREF_RANGE_BEGIN_ODT_ON;
                }
                else
                {
                    u2VrefBegin = RX_VREF_RANGE_BEGIN_ODT_OFF;
                }
            }
            else        
                u2VrefBegin = 0;//Lewis@20160817: Enlarge RX Vref range for eye scan
            #endif

            u2VrefEnd =RX_VREF_RANGE_END;
        }

        if (u1RXEyeScanEnable==0)
        {
            u2VrefStep=RX_VREF_RANGE_STEP;
        }
        else
        {
            u2VrefStep = EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV;
        }

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ5), 1, B0_DQ5_RG_RX_ARDQ_VREF_EN_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ5), 1, B1_DQ5_RG_RX_ARDQ_VREF_EN_B1);
    }
    else //LPDDR3 or diable RX Vref
    {
        u2VrefBegin = 0;
        u2VrefEnd = 0; // SEL[4:0] = 01110
        u2VrefStep =1; //don't care, just make for loop break;
    }

    #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    if(p->femmc_Ready==1 && ((p->Bypass_RDDQC && u1UseTestEngine==0) || (p->Bypass_RXWINDOW && u1UseTestEngine==1)))
    {        
        // load RX DQS and DQM delay from eMMC
        for (u1ByteIdx=0; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
        {     
            if(u1VrefScanEnable)
            {
                // load RX Vref from eMMC
                #if ( SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_VREF_CAL)
                u2FinalVref[u1ByteIdx] = p->pSavetimeData->u1RxWinPerbitVref_Save[p->channel][u1ByteIdx];
                #endif
            }
            
            iDQSDlyPerbyte[u1ByteIdx]= p->pSavetimeData->u1RxWinPerbit_DQS[p->channel][p->rank][u1ByteIdx];
            iDQMDlyPerbyte[u1ByteIdx]= p->pSavetimeData->u1RxWinPerbit_DQM[p->channel][p->rank][u1ByteIdx];
        }

        // load RX DQ delay from eMMC
        for (u1BitIdx=0; u1BitIdx<16; u1BitIdx++)
        {
            FinalWinPerBit[u1BitIdx].best_dqdly= p->pSavetimeData->u1RxWinPerbit_DQ[p->channel][p->rank][u1BitIdx];
        }
        
        if(u1UseTestEngine)
            vSetCalibrationResult(p, DRAM_CALIBRATION_RX_PERBIT, DRAM_OK);
        else
            vSetCalibrationResult(p, DRAM_CALIBRATION_RX_RDDQC, DRAM_OK);
    }
    else
    #endif
    {
        #if RX_DELAY_PRE_CAL && SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(u1UseTestEngine==0 || p->Bypass_RDDQC)
        #elif RX_DELAY_PRE_CAL
        if(u1UseTestEngine==0)
        #endif
        {
            if(p->frequency >=1600)
            {
                u4DelayBegin= -26;
            }
            else if(p->frequency >= 1140)
            {
                u4DelayBegin= -30;
            }
            else if(p->frequency >=800)
            {
                u4DelayBegin= -48;
            }
            else
            {
            if (vGet_Div_Mode(p) == DIV4_MODE)
                if (p->frequency <= 400)
                    u4DelayBegin= -MAX_RX_DQSDLY_TAPS; // for DDR800
                else
                    u4DelayBegin= -63; // for DDR1200
            else
                u4DelayBegin= -MAX_RX_DQSDLY_TAPS;
            }
            
            s2RxDelayPreCal =PASS_RANGE_NA; 
        }           
        #if RX_DELAY_PRE_CAL
        else
        {
            u4DelayBegin = s2RxDelayPreCal -15;  // for test engine
            if (u4DelayBegin < -MAX_RX_DQSDLY_TAPS)
                u4DelayBegin = -MAX_RX_DQSDLY_TAPS;
        }
        #endif

        u4DelayEnd = MAX_RX_DQDLY_TAPS;

        if(p->frequency <= 400)
            u4DelayStep = 4;  // 800
        else if(p->frequency <= 800)
            u4DelayStep = 2;  //1600, 1200
        else
            u4DelayStep = 1; //2667, 3200

        if(u1UseTestEngine==0) //if RDDQD, roughly calibration
            u4DelayStep <<= 1;

        u4DutyOffsetBegin = 0;
        u4DutyOffsetEnd = 0;
        u4DutyOffsetStep = 1;

        #if !REDUCE_LOG_FOR_PRELOADER
        mcSHOW_DBG_MSG(("\nRX DQS R/F Scan, iDutyOffset= %d\n", iDutyOffset));
        mcFPRINTF((fp_A60501, "\nRX DQS R/F Scan, iDutyOffset= %d\n", iDutyOffset));
        #endif

        for(u2VrefLevel = u2VrefBegin; u2VrefLevel <= u2VrefEnd; u2VrefLevel += u2VrefStep)
        {
            if(u1VrefScanEnable ==1)
            {
                #if ((!REDUCE_LOG_FOR_PRELOADER) || CALIBRATION_SPEED_UP_DEBUG)
                mcSHOW_DBG_MSG(("\n\tYulia RX VrefLevel=%d\n", u2VrefLevel));
                mcFPRINTF((fp_A60501, "\n\tRX VrefLevel=%d\n", u2VrefLevel));
                #endif
#if 0
                #if VENDER_JV_LOG
                if(u1UseTestEngine ==1)
                mcSHOW_DBG_MSG5(("\n\tRX VrefLevel=%d\n", u2VrefLevel));
                #endif
#endif
                //Set RX Vref Here
                if(u1KnownVref[0] != 0xff && u1KnownVref[1]!=0xff)
                {                    
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ5), u1KnownVref[0], SHU_B0_DQ5_RG_RX_ARDQ_VREF_SEL_B0);  // LP4 and LP4x with term: 0xe
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B1_DQ5), u1KnownVref[1], SHU_B1_DQ5_RG_RX_ARDQ_VREF_SEL_B1);  // LP4 and LP4x with term: 0xe
                }
                else  //normal vref calibration
                {
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ5), u2VrefLevel, SHU_B0_DQ5_RG_RX_ARDQ_VREF_SEL_B0);  // LP4 and LP4x with term: 0xe
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B1_DQ5), u2VrefLevel, SHU_B1_DQ5_RG_RX_ARDQ_VREF_SEL_B1);  // LP4 and LP4x with term: 0xe
                }
            }

            // 1.delay DQ ,find the pass widnow (left boundary).
            // 2.delay DQS find the pass window (right boundary).
            // 3.Find the best DQ / DQS to satify the middle value of the overall pass window per bit
            // 4.Set DQS delay to the max per byte, delay DQ to de-skew

            for (iDutyOffset=u4DutyOffsetBegin; iDutyOffset<=u4DutyOffsetEnd; iDutyOffset+=u4DutyOffsetStep)
            {
                // initialize parameters
                uiFinishCount =0;

                for (u1ByteIdx=0; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
                {
                    u2TempWinSum[u1ByteIdx] =0;
                    u1min_bit_by_vref[u1ByteIdx]=0xff;
                    u1min_winsize_by_vref[u1ByteIdx]=0xff;   
                }

                for (u1BitIdx = 0; u1BitIdx < p->data_width; u1BitIdx++)
                {
                    WinPerBit[u1BitIdx].first_pass = (S16)PASS_RANGE_NA;
                    WinPerBit[u1BitIdx].last_pass = (S16)PASS_RANGE_NA;
                    FinalWinPerBit[u1BitIdx].first_pass = (S16)PASS_RANGE_NA;
                    FinalWinPerBit[u1BitIdx].last_pass = (S16)PASS_RANGE_NA;

                    #if ENABLE_EYESCAN_GRAPH
                    if (u1IsLP4Family(p->dram_type))
                    {
                        gEyeScan_CaliDelay[u1BitIdx/8] = 0;
                        gEyeScan_DelayCellPI[u1BitIdx] = 0;
                        EyeScan_index[u1BitIdx] = 0;
                        u1pass_in_this_vref_flag[u1BitIdx] = 0;
                    }
                    #endif
                }

                // Adjust DQM output delay to 0
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ6), 0, SHU_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ6), 0, SHU_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1);
                
                // Adjust DQ output delay to 0
                //every 2bit dq have the same delay register address
                for (ii=0; ii<4; ii++)
                {
#if PINMUX_AUTO_TEST_PER_BIT_RX
                    if(gRX_check_per_bit_flag == 1)
                    {
                        //not reset delay cell
                    }
                    else
#endif
                    {
                        SetRxDqDelay(p, ii, 0);
                    }                    
                }

                for (iDelay=u4DelayBegin; iDelay<=u4DelayEnd; iDelay+= u4DelayStep)
                {
                    SetRxDqDqsDelay(p, iDelay);

                    if(u1UseTestEngine)
                    {
                        u4err_value = DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, p->test_pattern);
                    }
                    else
                    {
                        u4err_value = DramcRxWinRDDQCRun(p);
                    }

                    if(u1PrintCalibrationProc)
                    {
                        #ifdef ETT_PRINT_FORMAT
                        if(u4err_value !=0)
                        {
                            mcSHOW_DBG_MSG2(("%d, [0]", iDelay));
                        }
                        #else
                        mcSHOW_DBG_MSG2(("iDelay= %4d, [0]", iDelay));
                        #endif
                        mcFPRINTF((fp_A60501, "iDelay= %4d, [0]", iDelay));
                        //mcSHOW_DBG_MSG2(("u4err_value %x, u1MRRValue %x\n", u4err_value, u1MRRValue));
                        //mcFPRINTF((fp_A60501, "u4err_value!!  %x", u4err_value));
                    }

                    // check fail bit ,0 ok ,others fail
                    for (u1BitIdx = 0; u1BitIdx < p->data_width; u1BitIdx++)
                    {
                        u4fail_bit = u4err_value&((U32)1<<u1BitIdx);

                        if(WinPerBit[u1BitIdx].first_pass== PASS_RANGE_NA)
                        {
                            if(u4fail_bit==0) //compare correct: pass
                            {
                                WinPerBit[u1BitIdx].first_pass = iDelay;

                                #if RX_DELAY_PRE_CAL
                                if(u1UseTestEngine==0 && (s2RxDelayPreCal ==PASS_RANGE_NA))
                                {
                                    s2RxDelayPreCal = iDelay;
                                }

                                if(u1UseTestEngine==1 && iDelay==u4DelayBegin)
                                {
                                    mcSHOW_ERR_MSG(("RX_DELAY_PRE_CAL: Warning, possible miss RX window boundary\n"));
                                    #if __ETT__
                                    //DDR800, RX window too big, will pass at DQS dealy 127 (-127). Therefore, don't stop.
                                    if(p->frequency >= 600)
                                        while(1);
                                    #endif
                                }

                                #endif
                                
                                #if ENABLE_EYESCAN_GRAPH                            
                                if (u1IsLP4Family(p->dram_type))
                                {
                                    u1pass_in_this_vref_flag[u1BitIdx]=1;                           
                                }
                                #endif
                            }                   
                        }
                        else if(WinPerBit[u1BitIdx].last_pass == PASS_RANGE_NA)
                        {
                            //mcSHOW_DBG_MSG(("fb%d \n", u4fail_bit));

                            if(u4fail_bit !=0) //compare error : fail
                            {
                                WinPerBit[u1BitIdx].last_pass  = (iDelay-1);
                            }
                            else if (iDelay > (u4DelayEnd-u4DelayStep))
                            {
                                WinPerBit[u1BitIdx].last_pass = iDelay;
                            }

                            if(WinPerBit[u1BitIdx].last_pass  !=PASS_RANGE_NA)
                            {
                                if((WinPerBit[u1BitIdx].last_pass -WinPerBit[u1BitIdx].first_pass) >= (FinalWinPerBit[u1BitIdx].last_pass -FinalWinPerBit[u1BitIdx].first_pass))
                                {
                                    #if 0 //for debug
                                    if(FinalWinPerBit[u1BitIdx].last_pass != PASS_RANGE_NA)
                                    {
                                        mcSHOW_DBG_MSG2(("Bit[%d] Bigger window update %d > %d\n", u1BitIdx, \
                                            (WinPerBit[u1BitIdx].last_pass -WinPerBit[u1BitIdx].first_pass), (FinalWinPerBit[u1BitIdx].last_pass -FinalWinPerBit[u1BitIdx].first_pass)));
                                        mcFPRINTF((fp_A60501,"Bit[%d] Bigger window update %d > %d\n", u1BitIdx, \
                                            (WinPerBit[u1BitIdx].last_pass -WinPerBit[u1BitIdx].first_pass), (FinalWinPerBit[u1BitIdx].last_pass -FinalWinPerBit[u1BitIdx].first_pass)));
                                    }
                                    #endif

                                    //if window size bigger than 7, consider as real pass window. If not, don't update finish counte and won't do early break;
                                    if((WinPerBit[u1BitIdx].last_pass -WinPerBit[u1BitIdx].first_pass) >7)
                                        uiFinishCount |= (1<<u1BitIdx);

                                    //update bigger window size
                                    FinalWinPerBit[u1BitIdx].first_pass = WinPerBit[u1BitIdx].first_pass;
#if PINMUX_AUTO_TEST_PER_BIT_RX
                                    gFinalRXPerbitFirstPass[p->channel][u1BitIdx] = WinPerBit[u1BitIdx].first_pass;                                    
#endif
                                    FinalWinPerBit[u1BitIdx].last_pass = WinPerBit[u1BitIdx].last_pass;
                                }

                                #if ENABLE_EYESCAN_GRAPH
                                if(u1UseTestEngine)
                                {
                                    if (EyeScan_index[u1BitIdx] < EYESCAN_BROKEN_NUM)
                                    {
                                        gEyeScan_Min[u2VrefLevel/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx][EyeScan_index[u1BitIdx]] = WinPerBit[u1BitIdx].first_pass;
                                        gEyeScan_Max[u2VrefLevel/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx][EyeScan_index[u1BitIdx]] = WinPerBit[u1BitIdx].last_pass;
                                        mcSHOW_DBG_MSG3(("u2VrefLevel=%d, u1BitIdx=%d, index=%d (%d, %d)==\n",u2VrefLevel, u1BitIdx, EyeScan_index[u1BitIdx], gEyeScan_Min[u2VrefLevel/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx][EyeScan_index[u1BitIdx]], gEyeScan_Max[u2VrefLevel/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx][EyeScan_index[u1BitIdx]]));
                                        EyeScan_index[u1BitIdx]=EyeScan_index[u1BitIdx]+1;
                                    }
                                }
                                #endif

                                //reset tmp window
                                WinPerBit[u1BitIdx].first_pass = PASS_RANGE_NA;
                                WinPerBit[u1BitIdx].last_pass = PASS_RANGE_NA;
                            }
                        }

                        if(u1PrintCalibrationProc)
                        {
                            #ifdef ETT_PRINT_FORMAT
                            if(u4err_value !=0)
                            #endif
                            {
                                if(u1BitIdx%DQS_BIT_NUMBER ==0)
                                {
                                    mcSHOW_DBG_MSG2((" "));
                                    mcFPRINTF((fp_A60501, " "));
                                }

                                if (u4fail_bit == 0)
                                {
                                    mcSHOW_DBG_MSG2(("o"));
                                    mcFPRINTF((fp_A60501, "o"));
#if ENABLE_EYESCAN_GRAPH                            
                                    if (u1IsLP4Family(p->dram_type))
                                    {
                                         gEyeScan_TotalPassCount[u1BitIdx]+=(u4DelayStep*EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV);
                                    }
#endif     
                                }
                                else
                                {                                   
                                    mcSHOW_DBG_MSG2(("x"));
                                    mcFPRINTF((fp_A60501, "x"));                                 
                                }
                            }
#if ENABLE_EYESCAN_GRAPH // fixed build error when FOR_DV_SIMULATION_USED=1
                            else
                            {
                                if (u1IsLP4Family(p->dram_type))
                                {
                                    gEyeScan_TotalPassCount[u1BitIdx]+=(u4DelayStep*EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV);
                                }
                            }
#endif                  
                        }
                    }

                    if(u1PrintCalibrationProc)
                    {
                        #ifdef ETT_PRINT_FORMAT
                        if(u4err_value !=0)
                        #endif
                        {
                            mcSHOW_DBG_MSG2((" [MSB]\n"));
                            mcFPRINTF((fp_A60501, " [MSB]\n"));
                        }
                    }

                    //if all bits widnow found and all bits turns to fail again, early break;
                    if(uiFinishCount == 0xffff)
                    {
                        if(u1UseTestEngine)
                            vSetCalibrationResult(p, DRAM_CALIBRATION_RX_PERBIT, DRAM_OK);
                        else
                            vSetCalibrationResult(p, DRAM_CALIBRATION_RX_RDDQC, DRAM_OK);
                                
                        if((u1VrefScanEnable==0)  || u1RXEyeScanEnable)
                        {
                            if((u4err_value&0xffff) == 0xffff)
                            {
                                    #if !REDUCE_LOG_FOR_PRELOADER
                                    mcSHOW_DBG_MSG(("\nRX all bits window found, early break!\n"));
                                    #endif
                                    break;  //early break
                            }
                        }
                    }
                }

                for (u1BitIdx = 0; u1BitIdx < p->data_width; u1BitIdx++)
                {
                    u1ByteIdx = u1BitIdx/DQS_BIT_NUMBER;
                    
                    //FinalWinPerBit[u1BitIdx].win_size = FinalWinPerBit[u1BitIdx].last_pass - FinalWinPerBit[u1BitIdx].first_pass + (FinalWinPerBit[u1BitIdx].last_pass==FinalWinPerBit[u1BitIdx].first_pass?0:1);

                    if(FinalWinPerBit[u1BitIdx].first_pass == PASS_RANGE_NA)
                        FinalWinPerBit[u1BitIdx].win_size = 0;
                    else 
                        FinalWinPerBit[u1BitIdx].win_size= FinalWinPerBit[u1BitIdx].last_pass- FinalWinPerBit[u1BitIdx].first_pass + u4DelayStep;

                    if (FinalWinPerBit[u1BitIdx].win_size < u1min_winsize_by_vref[u1ByteIdx])
                    {
                        u1min_bit_by_vref[u1ByteIdx]= u1BitIdx;
                        u1min_winsize_by_vref[u1ByteIdx] = FinalWinPerBit[u1BitIdx].win_size;
                    }
                    
                    u2TempWinSum[u1ByteIdx] += FinalWinPerBit[u1BitIdx].win_size;  //Sum of CA Windows for vref selection

#if 0 //BU don't want customer knows our RX's ability
                    #if VENDER_JV_LOG
                    if(u1UseTestEngine ==1)
                    {
                        U8 shuffleIdx;
                        shuffleIdx = get_shuffleIndex_by_Freq(p);
                        mcSHOW_DBG_MSG5(("RX Bit%d, %d%%\n", u1BitIdx,  ((FinalWinPerBit[u1BitIdx].win_size*gHQALOG_RX_delay_cell_ps_075V*p->frequency*2)+(1000000-1))/1000000));
                    }
                    #endif
#endif
#if ENABLE_EYESCAN_GRAPH
                    if (u1IsLP4Family(p->dram_type))
                    {
                        gEyeScan_WinSize[u2VrefLevel/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx] = FinalWinPerBit[u1BitIdx].win_size;
                    }
#endif
                }
   

                for (u1ByteIdx=0; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
                {                   
                    if((u1min_winsize_by_vref[u1ByteIdx] > u1min_winsize[u1ByteIdx]) ||
                        ((u1min_winsize_by_vref[u1ByteIdx] == u1min_winsize[u1ByteIdx]) && (u2TempWinSum[u1ByteIdx] > u2rx_window_sum[u1ByteIdx])))
                    {
                        u2rx_window_sum[u1ByteIdx] =u2TempWinSum[u1ByteIdx];
                        u1min_winsize[u1ByteIdx] = u1min_winsize_by_vref[u1ByteIdx];
                        u1min_bit[u1ByteIdx] = u1min_bit_by_vref[u1ByteIdx];
                        
                        if(u1KnownVref[u1ByteIdx] != 0xff)
                        {
                            u2FinalVref[u1ByteIdx] = u1KnownVref[u1ByteIdx];
                        }
                        else
                        {
                            u2FinalVref[u1ByteIdx] = u2VrefLevel;
                        }
                        
                        mcSHOW_DBG_MSG2(("RX Vref B%d= %d, ", u1ByteIdx, u2FinalVref[u1ByteIdx]));
                        mcSHOW_DBG_MSG2(("Window Sum %d, worse bit %d, min window %d\n", u2TempWinSum[u1ByteIdx], u1min_bit_by_vref[u1ByteIdx], u1min_winsize_by_vref[u1ByteIdx]));

                        for (u1BitIdx=(u1ByteIdx*DQS_BIT_NUMBER); u1BitIdx<(u1ByteIdx*DQS_BIT_NUMBER)+DQS_BIT_NUMBER; u1BitIdx++)
                        {
                            FinalWinPerBit[u1BitIdx].win_center = (FinalWinPerBit[u1BitIdx].last_pass + FinalWinPerBit[u1BitIdx].first_pass)>>1;     // window center of each DQ bit

                            if(u1PrintCalibrationProc)
                            {
                                #ifdef ETT_PRINT_FORMAT
                                mcSHOW_DBG_MSG(("iDelay=%d, Bit %d, Center %d (%d ~ %d) %d\n", iDelay, u1BitIdx, FinalWinPerBit[u1BitIdx].win_center, FinalWinPerBit[u1BitIdx].first_pass, FinalWinPerBit[u1BitIdx].last_pass, FinalWinPerBit[u1BitIdx].win_size));
                                #else
                                mcSHOW_DBG_MSG(("iDelay=%d, Bit %2d, Center %3d (%4d ~ %4d) %d\n", iDelay, u1BitIdx, FinalWinPerBit[u1BitIdx].win_center, FinalWinPerBit[u1BitIdx].first_pass, FinalWinPerBit[u1BitIdx].last_pass, FinalWinPerBit[u1BitIdx].win_size));
                                #endif
                                mcFPRINTF((fp_A60501, "iDelay=%d, Bit %2d, Center %3d (%4d ~ %4d)\n", iDelay, u1BitIdx, FinalWinPerBit[u1BitIdx].win_center, FinalWinPerBit[u1BitIdx].first_pass, FinalWinPerBit[u1BitIdx].last_pass, FinalWinPerBit[u1BitIdx].win_size));
                            }

#ifdef FOR_HQA_TEST_USED
                            if(u1UseTestEngine ==1)
                            {
                                gFinalRXPerbitWin[p->channel][p->rank][u1BitIdx] = FinalWinPerBit[u1BitIdx].win_size;
                            }
#endif
                        }
                    }
                }
            }

#if ENABLE_EYESCAN_GRAPH
            if (u1IsLP4Family(p->dram_type))
            {
                for (u1BitIdx = 0; u1BitIdx < p->data_width; u1BitIdx++)
                {
                    if (u1pass_in_this_vref_flag[u1BitIdx]) 
                    {
                        U8 continuevrefheightfirstpass, continuevrefheightlastpass;

                        continuevrefheightfirstpass = gEyeScan_ContinueVrefHeight[u1BitIdx] & 0xff;
                        if (continuevrefheightfirstpass == 0)
                        {
                            continuevrefheightfirstpass = u2VrefLevel;
                        }
                        continuevrefheightlastpass = u2VrefLevel;
                        gEyeScan_ContinueVrefHeight[u1BitIdx] = (continuevrefheightlastpass<<8) | continuevrefheightfirstpass;
                    
                    }
                }
            }
#endif        

            if((u2TempWinSum[0] < (u2rx_window_sum[0]*95/100)) && (u2TempWinSum[1] < (u2rx_window_sum[1]*95/100))&& u1RXEyeScanEnable == 0)
            {
                //mcSHOW_DBG_MSG(("\nRX Vref found, early break!\n"));
                u2VrefLevel = u2VrefEnd;
                break;//max vref found, early break;
            }
        }

        if(u1UseTestEngine)
        {
            DramcEngine2End(p);
        }
        else
        {
            DramcRxWinRDDQCEnd(p);
        }

        // 3
        //As per byte, check max DQS delay in 8-bit. Except for the bit of max DQS delay, delay DQ to fulfill setup time = hold time
        for (u1ByteIdx = 0; u1ByteIdx < (p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
        {
            u2TmpDQMSum =0;
    
            ucbit_first =DQS_BIT_NUMBER*u1ByteIdx;
            ucbit_last = DQS_BIT_NUMBER*u1ByteIdx+DQS_BIT_NUMBER-1;
            iDQSDlyPerbyte[u1ByteIdx] = MAX_RX_DQSDLY_TAPS;
    
            for (u1BitIdx = ucbit_first; u1BitIdx <= ucbit_last; u1BitIdx++)
            {
                // find out max Center value
                if(FinalWinPerBit[u1BitIdx].win_center < iDQSDlyPerbyte[u1ByteIdx])
                {
                    iDQSDlyPerbyte[u1ByteIdx] = FinalWinPerBit[u1BitIdx].win_center;
                }
    
                //mcSHOW_DBG_MSG(("bit#%2d : center=(%2d)\n", u1BitIdx, FinalWinPerBit[u1BitIdx].win_center));
                //mcFPRINTF((fp_A60501, "bit#%2d : center=(%2d)\n", u1BitIdx, FinalWinPerBit[u1BitIdx].win_center));
            }
    
            //mcSHOW_DBG_MSG(("----seperate line----\n"));
            //mcFPRINTF((fp_A60501, "----seperate line----\n"));
    
            if (iDQSDlyPerbyte[u1ByteIdx]  > 0)  // Delay DQS=0, Delay DQ only
            {
                iDQSDlyPerbyte[u1ByteIdx]  = 0;
            }
            else  //Need to delay DQS
            {
                iDQSDlyPerbyte[u1ByteIdx]  = -iDQSDlyPerbyte[u1ByteIdx] ;
            }
    
            // we delay DQ or DQS to let DQS sample the middle of rx pass window for all the 8 bits,
            for (u1BitIdx = ucbit_first; u1BitIdx <= ucbit_last; u1BitIdx++)
            {
                FinalWinPerBit[u1BitIdx].best_dqdly = iDQSDlyPerbyte[u1ByteIdx] + FinalWinPerBit[u1BitIdx].win_center;
                u2TmpDQMSum += FinalWinPerBit[u1BitIdx].best_dqdly;
#if ENABLE_EYESCAN_GRAPH
                if (u1IsLP4Family(p->dram_type))
                {
                    gEyeScan_DelayCellPI[u1BitIdx] = FinalWinPerBit[u1BitIdx].best_dqdly;
                }
#endif
            }
    
            // calculate DQM as average of 8 DQ delay
            iDQMDlyPerbyte[u1ByteIdx] = u2TmpDQMSum/DQS_BIT_NUMBER;
    
#ifdef FOR_HQA_REPORT_USED
            HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT0, "RX_Window_Center_", "DQS", u1ByteIdx, iDQSDlyPerbyte[u1ByteIdx], NULL);
            HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT0, "RX_Window_Center_", "DQM", u1ByteIdx, iDQMDlyPerbyte[u1ByteIdx], NULL);
            for (u1BitIdx = ucbit_first; u1BitIdx <= ucbit_last; u1BitIdx++)
            {
                HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT1, "RX_Window_Center_", "DQ", u1BitIdx, FinalWinPerBit[u1BitIdx].win_center, NULL);
            }
#endif
        }
    }
    

    #if REG_SHUFFLE_REG_CHECK
    ShuffleRegCheck =1;
    #endif

    gu2RX_DQS_Duty_Offset[0][0]=gu2RX_DQS_Duty_Offset[0][1]=gu2RX_DQS_Duty_Offset[1][0]=gu2RX_DQS_Duty_Offset[1][1] = 0;

    if(u1VrefScanEnable==1)
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ5), u2FinalVref[0], SHU_B0_DQ5_RG_RX_ARDQ_VREF_SEL_B0);  // LP4 and LP4x with term: 0xe
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B1_DQ5), u2FinalVref[1], SHU_B1_DQ5_RG_RX_ARDQ_VREF_SEL_B1);  // LP4 and LP4x with term: 0xe
    }
    
    // set dqs delay, (dqm delay)
    for (u1ByteIdx = 0; u1ByteIdx < (p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
    {
        //Set RX Final Vref Here
        if(u1VrefScanEnable==1)
        {                
            #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
            if(p->femmc_Ready==0)
            {
                p->pSavetimeData->u1RxWinPerbitVref_Save[p->channel][u1ByteIdx]=u2FinalVref[u1ByteIdx];
            }
            #endif
    
            mcSHOW_DBG_MSG(("\nFinal RX Vref B%d = %d", u1ByteIdx, u2FinalVref[u1ByteIdx]));
            mcFPRINTF((fp_A60501, "\nFinal RX Vref B%d = %d", u1ByteIdx, u2FinalVref[u1ByteIdx]));
    
            // When only calibrate RX Vref for Rank 0, apply the same value for Rank 1.
            if(p->rank==RANK_0)
            {
                gFinalRXVrefDQ[p->channel][RANK_0][u1ByteIdx] = (U8) u2FinalVref[u1ByteIdx];
                gFinalRXVrefDQForSpeedUp[p->channel][RANK_0][p->odt_onoff][u1ByteIdx] = (U8) u2FinalVref[u1ByteIdx];
                mcSHOW_DBG_MSG((" to rank0"));
                mcFPRINTF((fp_A60501, " to rank0"));
            }
            
            gFinalRXVrefDQ[p->channel][RANK_1][u1ByteIdx] = (U8) u2FinalVref[u1ByteIdx];
            gFinalRXVrefDQForSpeedUp[p->channel][RANK_1][p->odt_onoff][u1ByteIdx] = (U8) u2FinalVref[u1ByteIdx];
            mcSHOW_DBG_MSG((" to rank1\n"));
            mcFPRINTF((fp_A60501, "to rank1\n"));
        }

    
        // Set DQS & DQM delay
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ6 + u1ByteIdx*DDRPHY_AO_B0_B1_OFFSET_0X50), \
            P_Fld(((U32)iDQSDlyPerbyte[u1ByteIdx]+gu2RX_DQS_Duty_Offset[u1ByteIdx][0]),SHU_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0) |
            P_Fld(((U32)iDQMDlyPerbyte[u1ByteIdx]),SHU_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0));

        #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(p->femmc_Ready==0)
        {
            p->pSavetimeData->u1RxWinPerbit_DQS[p->channel][p->rank][u1ByteIdx]=(U32)iDQSDlyPerbyte[u1ByteIdx];
            p->pSavetimeData->u1RxWinPerbit_DQM[p->channel][p->rank][u1ByteIdx]=(U32)iDQMDlyPerbyte[u1ByteIdx];
        }
        #endif
    }

    #if REG_SHUFFLE_REG_CHECK
    ShuffleRegCheck =0;
    #endif

    #if REG_SHUFFLE_REG_CHECK
    ShuffleRegCheck =1;
    #endif

    // set dq delay
    for (u1BitIdx=0; u1BitIdx<DQS_BIT_NUMBER; u1BitIdx+=2)
    {
         vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ2+u1BitIdx*2), \
                                        P_Fld(((U32)FinalWinPerBit[u1BitIdx].best_dqdly),SHU_R0_B0_DQ2_RK0_RX_ARDQ0_R_DLY_B0) |\
                                        P_Fld(((U32)FinalWinPerBit[u1BitIdx+1].best_dqdly),SHU_R0_B0_DQ2_RK0_RX_ARDQ1_R_DLY_B0));

         vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ2+u1BitIdx*2), \
                                        P_Fld((U32)FinalWinPerBit[u1BitIdx+8].best_dqdly,SHU_R0_B1_DQ2_RK0_RX_ARDQ0_R_DLY_B1)| \
                                        P_Fld((U32)FinalWinPerBit[u1BitIdx+9].best_dqdly,SHU_R0_B1_DQ2_RK0_RX_ARDQ1_R_DLY_B1));

        //mcSHOW_DBG_MSG(("u1BitId %d  Addr 0x%2x = %2d %2d %2d %2d \n", u1BitIdx, DRAMC_REG_ADDR(DDRPHY_RXDQ1+u1BitIdx*2), \
        //                FinalWinPerBit[u1BitIdx].best_dqdly, FinalWinPerBit[u1BitIdx+1].best_dqdly,  FinalWinPerBit[u1BitIdx+8].best_dqdly, FinalWinPerBit[u1BitIdx+9].best_dqdly));
    }
    
    #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    if(p->femmc_Ready==0)
	{
        for (u1BitIdx=0; u1BitIdx<16; u1BitIdx++)
    	{
         	p->pSavetimeData->u1RxWinPerbit_DQ[p->channel][p->rank][u1BitIdx]=(U32)FinalWinPerBit[u1BitIdx].best_dqdly;
#if RUNTIME_SHMOO_RELEATED_FUNCTION
         	p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[p->channel][p->rank][u1BitIdx]=(U32)FinalWinPerBit[u1BitIdx].first_pass;
        	p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[p->channel][p->rank][u1BitIdx]=(U32)FinalWinPerBit[u1BitIdx].last_pass;
#endif
      	}
	}
    #endif

    DramPhyReset(p);

    vPrintCalibrationBasicInfo(p);

#ifdef ETT_PRINT_FORMAT
    mcSHOW_DBG_MSG(("DQS Delay:\nDQS0 = %d, DQS1 = %d\n"
                    "DQM Delay:\nDQM0 = %d, DQM1 = %d\n",
                        iDQSDlyPerbyte[0], iDQSDlyPerbyte[1],
                        iDQMDlyPerbyte[0], iDQMDlyPerbyte[1]));
#else
    mcSHOW_DBG_MSG(("DQS Delay:\nDQS0 = %2d, DQS1 = %2d\n"
                    "DQM Delay:\nDQM0 = %2d, DQM1 = %2d\n",
                        iDQSDlyPerbyte[0], iDQSDlyPerbyte[1],
                        iDQMDlyPerbyte[0], iDQMDlyPerbyte[1]));
#endif
    mcSHOW_DBG_MSG(("DQ Delay:\n"));

    mcFPRINTF((fp_A60501, "\tdramc_rxdqs_perbit_swcal\n"));
    mcFPRINTF((fp_A60501, "\tchannel=%d(1:cha) \n", p->channel));
    mcFPRINTF((fp_A60501, "\tbus width=%d\n\n", p->data_width));

    mcFPRINTF((fp_A60501, "DQS Delay:\n DQS0 = %2d DQS1 = %2d\n", iDQSDlyPerbyte[0], iDQSDlyPerbyte[1]));
    mcFPRINTF((fp_A60501, "DQM Delay:\n DQM0 = %2d DQM1 = %2d\n", iDQMDlyPerbyte[0], iDQMDlyPerbyte[1]));
    mcFPRINTF((fp_A60501, "DQ Delay:\n"));

    for (u1BitIdx = 0; u1BitIdx < p->data_width; u1BitIdx=u1BitIdx+4)
    {
    #ifdef ETT_PRINT_FORMAT
        mcSHOW_DBG_MSG(("DQ%d =%d, DQ%d =%d, DQ%d =%d, DQ%d =%d\n", u1BitIdx, FinalWinPerBit[u1BitIdx].best_dqdly, u1BitIdx+1, FinalWinPerBit[u1BitIdx+1].best_dqdly, u1BitIdx+2, FinalWinPerBit[u1BitIdx+2].best_dqdly, u1BitIdx+3, FinalWinPerBit[u1BitIdx+3].best_dqdly));
    #else
        mcSHOW_DBG_MSG(("DQ%2d =%2d, DQ%2d =%2d, DQ%2d =%2d, DQ%2d =%2d\n", u1BitIdx, FinalWinPerBit[u1BitIdx].best_dqdly, u1BitIdx+1, FinalWinPerBit[u1BitIdx+1].best_dqdly, u1BitIdx+2, FinalWinPerBit[u1BitIdx+2].best_dqdly, u1BitIdx+3, FinalWinPerBit[u1BitIdx+3].best_dqdly));
    #endif
        mcFPRINTF((fp_A60501, "DQ%2d =%2d, DQ%2d =%2d, DQ%2d =%2d, DQ%2d=%2d\n", u1BitIdx, FinalWinPerBit[u1BitIdx].best_dqdly, u1BitIdx+1, FinalWinPerBit[u1BitIdx+1].best_dqdly, u1BitIdx+2, FinalWinPerBit[u1BitIdx+2].best_dqdly, u1BitIdx+3, FinalWinPerBit[u1BitIdx+3].best_dqdly));
    }
    mcSHOW_DBG_MSG(("\n\n"));
    mcFPRINTF((fp_A60501, "\n\n"));

    // BU request RX & TX window size log.
#if 0//def RELEASE  // for parsing tool
    if(u1UseTestEngine==1)
    {
        mcSHOW_DBG_MSG4(("RX CH%d R%d ,Freq %d\n", p->channel, p->rank, p->frequency));
        for (u1BitIdx = 0; u1BitIdx < p->data_width; u1BitIdx++)
        {
            mcSHOW_DBG_MSG4(("%d: %d\n", u1BitIdx, gFinalRXPerbitWin[p->channel][p->rank][u1BitIdx]));
        }
    }
#endif

    #if REG_SHUFFLE_REG_CHECK
    ShuffleRegCheck =0;
    #endif
    mcSHOW_DBG_MSG3(("[DramcRxWindowPerbitCal] Done\n"));
    mcFPRINTF((fp_A60501, "[DramcRxWindowPerbitCal] Done\n"));

#if REG_ACCESS_PORTING_DGB
    RegLogEnable =0;
#endif

return DRAM_OK;

    // Log example  ==> Neec to update
    /*
------------------------------------------------------
Start calculate dq time and dqs time /
Find max DQS delay per byte / Adjust DQ delay to align DQS...
------------------------------------------------------
bit# 0 : dq time=11 dqs time= 8
bit# 1 : dq time=11 dqs time= 8
bit# 2 : dq time=11 dqs time= 6
bit# 3 : dq time=10 dqs time= 8
bit# 4 : dq time=11 dqs time= 8
bit# 5 : dq time=10 dqs time= 8
bit# 6 : dq time=11 dqs time= 8
bit# 7 : dq time= 9 dqs time= 6
----seperate line----
bit# 8 : dq time=12 dqs time= 7
bit# 9 : dq time=10 dqs time= 8
bit#10 : dq time=11 dqs time= 8
bit#11 : dq time=10 dqs time= 8
bit#12 : dq time=11 dqs time= 8
bit#13 : dq time=11 dqs time= 8
bit#14 : dq time=11 dqs time= 8
bit#15 : dq time=12 dqs time= 8
----seperate line----
bit#16 : dq time=11 dqs time= 7
bit#17 : dq time=10 dqs time= 8
bit#18 : dq time=11 dqs time= 7
bit#19 : dq time=11 dqs time= 6
bit#20 : dq time=10 dqs time= 9
bit#21 : dq time=11 dqs time=10
bit#22 : dq time=11 dqs time=10
bit#23 : dq time= 9 dqs time= 9
----seperate line----
bit#24 : dq time=12 dqs time= 6
bit#25 : dq time=13 dqs time= 6
bit#26 : dq time=13 dqs time= 7
bit#27 : dq time=11 dqs time= 7
bit#28 : dq time=12 dqs time= 8
bit#29 : dq time=10 dqs time= 8
bit#30 : dq time=13 dqs time= 7
bit#31 : dq time=11 dqs time= 8
----seperate line----
==================================================
    dramc_rxdqs_perbit_swcal_v2
    channel=2(2:cha, 3:chb) apply = 1
==================================================
DQS Delay :
 DQS0 = 0 DQS1 = 0 DQS2 = 0 DQS3 = 0
DQ Delay :
DQ 0 =  1 DQ 1 =  1 DQ 2 =  2 DQ 3 =  1
DQ 4 =  1 DQ 5 =  1 DQ 6 =  1 DQ 7 =  1
DQ 8 =  2 DQ 9 =  1 DQ10 =  1 DQ11 =  1
DQ12 =  1 DQ13 =  1 DQ14 =  1 DQ15 =  2
DQ16 =  2 DQ17 =  1 DQ18 =  2 DQ19 =  2
DQ20 =  0 DQ21 =  0 DQ22 =  0 DQ23 =  0
DQ24 =  3 DQ25 =  3 DQ26 =  3 DQ27 =  2
DQ28 =  2 DQ29 =  1 DQ30 =  3 DQ31 =  1
_______________________________________________________________
   */
}
#endif //SIMULATION_RX_PERBIT

#if SIMULATION_DATLAT
void dle_factor_handler(DRAMC_CTX_T *p, U8 curr_val, U8 pip_num)
{
    U8 u1DATLAT_DSEL=0;
    U8 u1DLECG_OptionEXT1 =0;
    U8 u1DLECG_OptionEXT2 =0;
    U8 u1DLECG_OptionEXT3 =0;

#if REG_ACCESS_PORTING_DGB
    RegLogEnable =1;
    mcSHOW_DBG_MSG(("\n[REG_ACCESS_PORTING_FUNC] dle_factor_handler\n"));
    mcFPRINTF((fp_A60501, "\n[REG_ACCESS_PORTING_FUNC] dle_factor_handler\n"));
#endif

    #if REG_SHUFFLE_REG_CHECK
    ShuffleRegCheck =1;
    #endif

    if(curr_val<2)
        curr_val =2;

#if RX_PIPE_BYPASS_EN
    u1DATLAT_DSEL = curr_val - 1;
#else
    u1DATLAT_DSEL = curr_val - 2;
#endif

    // Datlat_dsel = datlat -1, only 1 TX pipe
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_CONF1),
                                        P_Fld(curr_val, SHU_CONF1_DATLAT) |
                                        P_Fld(u1DATLAT_DSEL, SHU_CONF1_DATLAT_DSEL) |
                                        P_Fld(u1DATLAT_DSEL, SHU_CONF1_DATLAT_DSEL_PHY));

    //Lewis@20180418 Change the fomula of rd_period_en extend of DATLAT, since the moidfy CLKAR_RDYCKAR of golden setting could set to 0.
    //(>=8 & <14) set EXT1 =1, EXT2=0, EXT3=0
    //(>= 14 & <19) set EXT1=1, EXT2=1, EXT3=0
    //(>=19) set EXT1=1, EXT2=1, EXT3=1
    u1DLECG_OptionEXT1 = (curr_val>=8) ? (1) : (0);
    u1DLECG_OptionEXT2 = (curr_val>=14) ? (1) : (0);
    u1DLECG_OptionEXT3 = (curr_val>=19) ? (1) : (0);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_PIPE), P_Fld(u1DLECG_OptionEXT1, SHU_PIPE_READ_START_EXTEND1)
                | P_Fld(u1DLECG_OptionEXT1, SHU_PIPE_DLE_LAST_EXTEND1)
                | P_Fld((u1DLECG_OptionEXT2), SHU_PIPE_READ_START_EXTEND2)
                | P_Fld((u1DLECG_OptionEXT2), SHU_PIPE_DLE_LAST_EXTEND2)
                | P_Fld((u1DLECG_OptionEXT3), SHU_PIPE_READ_START_EXTEND3)
                | P_Fld((u1DLECG_OptionEXT3), SHU_PIPE_DLE_LAST_EXTEND3));

    #if REG_SHUFFLE_REG_CHECK
    ShuffleRegCheck =0;
    #endif

    DramPhyReset(p);

#if REG_ACCESS_PORTING_DGB
    RegLogEnable =0;
#endif
}

//-------------------------------------------------------------------------
/** Dramc_ta2_rx_scan
 */
//-------------------------------------------------------------------------
static U32 Dramc_ta2_rx_scan(DRAMC_CTX_T *p, U8 u1UseTestEngine)
{
    U32 ii, u4err_value;
    S16 iDelay;
    U8 u1ByteIdx;
    U32 u4RegBak_DDRPHY_RXDQ_RK0[DQS_NUMBER][5];  // rank 0
    U8 dl_value[8]={0,0,0,0,0,0,0,0};

    // reg backup
#if 0
        for (ii=0; ii<5; ii++)
        {
            u4RegBak_DDRPHY_RXDQ_RK0[0][ii] =(u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ2+ii*4), PHY_FLD_FULL)); // rank 0, byte 0
            u4RegBak_DDRPHY_RXDQ_RK0[1][ii] =(u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ2+ii*4), PHY_FLD_FULL)); // rank 0, byte 1
        }
#endif

        // Adjust DQM output delay to 0
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ6), 0, SHU_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ6), 0, SHU_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1);

        // Adjust DQ output delay to 0
        //every 2bit dq have the same delay register address
        for (ii=0; ii<4; ii++)
        {
            SetRxDqDelay(p, ii, 0);
        }

        if(u1UseTestEngine)
            DramcEngine2Init(p, p->test2_1, p->test2_2, p->test_pattern, 0);

        // quick rx dqs search
        //mcSHOW_DBG_MSG(("quick rx dqs search\n"));
        //mcFPRINTF((fp_A60817, "quick rx dqs search\n", iDelay));
        for (iDelay=-32; iDelay<=32; iDelay+=4)
        {
            //mcSHOW_DBG_MSG(("%2d, ", iDelay));
            //mcFPRINTF((fp_A60817,"%2d, ", iDelay));
            SetRxDqDqsDelay(p, iDelay);

            if(u1UseTestEngine)
            {
               u4err_value = DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, p->test_pattern);
            }
            else
            {
               u4err_value = DramcRxWinRDDQCRun(p);
            }

            if(u4err_value ==0)// rx dqs found.
                break;
        }

        if(u1UseTestEngine)
            DramcEngine2End(p);

        #ifdef ETT_PRINT_FORMAT
        mcSHOW_DBG_MSG(("RX DQS dly = %d, ", iDelay));
        #else
        mcSHOW_DBG_MSG(("RX DQS dly = %2d, ", iDelay));
        #endif
        mcFPRINTF((fp_A60501, "RX DQS dly = %2d, ", iDelay));

#if 0
        // restore registers
        for (ii=0; ii<5; ii++)
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ2+ ii*4), u4RegBak_DDRPHY_RXDQ_RK0[0][ii] , PHY_FLD_FULL);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ2+ ii*4), u4RegBak_DDRPHY_RXDQ_RK0[1][ii] , PHY_FLD_FULL);
        }
#endif

        return u4err_value;
}


static U8 aru1RxDatlatResult[CHANNEL_NUM][RANK_MAX];

U8 DramcRxdatlatScan(DRAMC_CTX_T *p, DRAM_DATLAT_CALIBRATION_TYTE_T use_rxtx_scan)
{
    U8 ii, ucStartCalVal=0;
    U32 u4prv_register_080;
    U32 u4err_value= 0xffffffff;
    U8 ucfirst, ucbegin, ucsum, ucbest_step, ucpipe_num =0;
    U16 u2DatlatBegin;

    // error handling
    if (!p)
    {
        mcSHOW_ERR_MSG(("context NULL\n"));
        return DRAM_FAIL;
    }
#if REG_ACCESS_PORTING_DGB
    RegLogEnable =1;
    mcSHOW_DBG_MSG(("\n[REG_ACCESS_PORTING_FUNC]   DramcRxdatlatCal\n"));
    mcFPRINTF((fp_A60501, "\n[REG_ACCESS_PORTING_FUNC]   DramcRxdatlatCal\n"));
#endif

    mcSHOW_DBG_MSG(("\n[DATLAT]\n"
                    "Freq=%d, CH%d RK%d, use_rxtx_scan=%d\n\n",
                         p->frequency, p->channel, p->rank, use_rxtx_scan));

    mcFPRINTF((fp_A60501, "\n\tDATLAT calibration\n"));
    mcFPRINTF((fp_A60501, "\tch=%d(1:cha), rank=%d, use_rxtx_scan=%d\n\n",
                             p->channel, p->rank, use_rxtx_scan));

    // [11:10] DQIENQKEND 01 -> 00 for DATLAT calibration issue, DQS input enable will refer to DATLAT
    // if need to enable this (for power saving), do it after all calibration done
    //u4prv_register_0d8 = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_MCKDLY));
    //vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_PADCTRL), P_Fld(0, PADCTRL_DQIENQKEND) | P_Fld(0, PADCTRL_DQIENLATEBEGIN));

    // pre-save
    // 0x07c[6:4]   DATLAT bit2-bit0
    u4prv_register_080 = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SHU_CONF1));

    // init best_step to default
    ucbest_step = (U8) u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_CONF1), SHU_CONF1_DATLAT);
    mcSHOW_DBG_MSG(("DATLAT Default: 0x%x\n", ucbest_step));
    mcFPRINTF((fp_A60501, "DATLAT Default: 0x%x\n", ucbest_step));

    // 1.set DATLAT 0-15 (0-21 for MT6595)
    // 2.enable engine1 or engine2
    // 3.check result  ,3~4 taps pass
    // 4.set DATLAT 2nd value for optimal

    // Initialize
    ucfirst = 0xff;
    ucbegin = 0;
    ucsum = 0;

    DramcEngine2Init(p, p->test2_1, p->test2_2, p->test_pattern, 0);
#if (FOR_DV_SIMULATION_USED==0)
		u2DatlatBegin=7;
#else
		u2DatlatBegin=0;
#endif

    #if (SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_DATLAT)
	if(p->femmc_Ready==1)
	{
        ucbest_step = p->pSavetimeData->u1RxDatlat_Save[p->channel][p->rank];
    }
    else
    #endif
    {
        for (ii = u2DatlatBegin; ii < DATLAT_TAP_NUMBER; ii++)
        {
            // 1
            dle_factor_handler(p, ii, ucpipe_num);

            // 2
            if(use_rxtx_scan == fcDATLAT_USE_DEFAULT)
            {
                u4err_value = DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, p->test_pattern);
            }

            // 3
            if (u4err_value == 0)
            {
                if (ucbegin == 0)
                {
                    // first tap which is pass
                    ucfirst = ii;
                    ucbegin = 1;
                }
                if (ucbegin == 1)
                {
                    ucsum++;

                    if(ucsum >4)
                        break;  //early break.
                }
            }
            else
            {
                if (ucbegin == 1)
                {
                    // pass range end
                    ucbegin = 0xff;
                }
            }

            #ifdef ETT_PRINT_FORMAT
            mcSHOW_DBG_MSG(("%d, 0x%X, sum=%d\n", ii, u4err_value, ucsum));
            #else
            mcSHOW_DBG_MSG(("TAP=%2d, err_value=0x%8x,  sum=%d\n", ii, u4err_value, ucsum));
            #endif
            mcFPRINTF((fp_A60501, "TAP=%2d, err_value=0x%8x, begin=%d, first=%3d, sum=%d\n", ii, u4err_value, ucbegin, ucfirst, ucsum));
        }

        DramcEngine2End(p);

        // 4
        if (ucsum == 0)
        {
            mcSHOW_ERR_MSG(("no DATLAT taps pass, DATLAT calibration fail!\n"));
        }
        else if (ucsum <= 3)
        {
            ucbest_step = ucfirst + (ucsum>>1);
        }
        else // window is larger htan 3
        {
            ucbest_step = ucfirst + 1;
        }
    }
    aru1RxDatlatResult[p->channel][p->rank] = ucbest_step;

   #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
	if(p->femmc_Ready==0)
    {
        p->pSavetimeData->u1RxDatlat_Save[p->channel][p->rank] = ucbest_step;
    }
   #endif
    
    mcSHOW_DBG_MSG(("best_step=%d\n\n", ucbest_step));
    mcFPRINTF((fp_A60501, "best_step=%d\n", ucbest_step));

#if __ETT__
    U8 _init_Datlat_value = vDramcACTimingGetDatLat(p);
    if ((_init_Datlat_value > (ucbest_step + 1))&&(_init_Datlat_value < (ucbest_step - 1)))
    {
        mcSHOW_DBG_MSG(("[WARNING!!] Datlat initial value(%d) = best_step(%d) %c %d, out of range!\n\n",
                           _init_Datlat_value,
                           ucbest_step,
                           (ucbest_step > _init_Datlat_value)? '-': '+',
                           abs(ucbest_step - _init_Datlat_value)));
        while(1);
    }
#endif

#if defined(FOR_HQA_TEST_USED) && defined(FOR_HQA_REPORT_USED)
    HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT2, "DATLAT", "", 0, ucbest_step, NULL);
#endif

    #if (SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_DATLAT)
	if(p->femmc_Ready==1)
	{
        dle_factor_handler(p, ucbest_step, ucpipe_num);
        vSetCalibrationResult(p, DRAM_CALIBRATION_DATLAT, DRAM_OK);
	}
    else
    #endif
    {
        if(ucsum <4)
        {
            mcSHOW_DBG_MSG2(("[NOTICE] CH%d, DatlatSum %d\n", p->channel, ucsum));
            mcFPRINTF((fp_A60501, "[NOTICE] CH%d, DatlatSum  %d\n", p->channel, ucsum));
        }

        if (ucsum == 0)
        {
            mcSHOW_ERR_MSG(("DATLAT calibration fail, write back to default values!\n"));
            vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU_CONF1), u4prv_register_080);
            vSetCalibrationResult(p, DRAM_CALIBRATION_DATLAT, DRAM_FAIL);
        }
        else
        {
            dle_factor_handler(p, ucbest_step, ucpipe_num);
            vSetCalibrationResult(p, DRAM_CALIBRATION_DATLAT, DRAM_OK);
        }
    }
    // [11:10] DQIENQKEND 01 -> 00 for DATLAT calibration issue, DQS input enable will refer to DATLAT
    // if need to enable this (for power saving), do it after all calibration done
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_PADCTRL), P_Fld(1, PADCTRL_DQIENQKEND) | P_Fld(1, PADCTRL_DQIENLATEBEGIN));

    mcSHOW_DBG_MSG3(("[DramcRxdatlatCal] Done\n"));
    mcFPRINTF((fp_A60501, "[DramcRxdatlatCal] Done\n"));

#if REG_ACCESS_PORTING_DGB
    RegLogEnable =0;
#endif

    return ucsum;
}

void DramcRxdatlatCal(DRAMC_CTX_T *p)
{
    U8 u1DatlatWindowSum;

    u1DatlatWindowSum = DramcRxdatlatScan(p, fcDATLAT_USE_DEFAULT);

    if((p->dram_type == TYPE_LPDDR3) &&(u1DatlatWindowSum <5))
    {
        mcSHOW_DBG_MSG(("\nDatlatWindowSum %d too small(<5), Start RX + Datlat scan\n", u1DatlatWindowSum));
        DramcRxdatlatScan(p, fcDATLAT_USE_RX_SCAN);
    }
}

DRAM_STATUS_T DramcDualRankRxdatlatCal(DRAMC_CTX_T *p)
{
    U8 u1FinalDatlat, u1Datlat0, u1Datlat1;

    u1Datlat0 = aru1RxDatlatResult[p->channel][0];
    u1Datlat1 = aru1RxDatlatResult[p->channel][1];

    if(p->support_rank_num==RANK_DUAL)
    {
        if(u1Datlat0> u1Datlat1)
        {
            u1FinalDatlat= u1Datlat0;
        }
        else
        {
            u1FinalDatlat= u1Datlat1;
        }
    }
    else
    {
        u1FinalDatlat= u1Datlat0;
    }

    #if ENABLE_READ_DBI
    if(p->DBI_R_onoff[p->dram_fsp])
    {
      u1FinalDatlat++;
    }
    #endif
		
    dle_factor_handler(p, u1FinalDatlat, 3);

    #if RDSEL_TRACKING_EN
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_RDSEL_TRACK), u1FinalDatlat, SHU_RDSEL_TRACK_R_DMDATLAT_I);
    #endif

    mcSHOW_DBG_MSG(("[DualRankRxdatlatCal] RK0: %d, RK1: %d, Final_Datlat %d\n", u1Datlat0, u1Datlat1, u1FinalDatlat));
    mcFPRINTF((fp_A60501, "[DualRankRxdatlatCal] RK0: %d, RK1: %d, Final_Datlat %d\n", u1Datlat0, u1Datlat1, u1FinalDatlat));

    return DRAM_OK;

}
#endif //SIMULATION_DATLAT

#if SIMULATION_TX_PERBIT

#if TX_OE_CALIBATION
#define TX_OE_PATTERN_USE_TA2 1
#define TX_OE_SCAN_FULL_RANGE 0

void DramcTxOECalibration(DRAMC_CTX_T *p)
{
    U8 u1ByteIdx, ucBegin[2]={0}, ucEnd[2]={0xff, 0xff}, ucbest_step[2];
    //U8 ucbegin=0xff, , ucfirst, ucsum, ucbest_step;
    U32 u4RegValue_TXDLY, u4RegValue_dly, u4err_value;
    U16 u2Delay, u2TempVirtualDelay, u2SmallestVirtualDelay=0xffff;
    U16 u2DQOEN_DelayBegin, u2DQEN_DelayEnd;
    U8 ucdq_ui_large_bak[DQS_NUMBER], ucdq_ui_small_bak[DQS_NUMBER];
    U8 ucdq_oen_ui_large[2], ucdq_oen_ui_small[2];
    U8 ucdq_current_ui_large, ucdq_current_ui_small;
    //U8 ucdq_ui_large_reg_value=0xff, ucdq_ui_small_reg_value=0xff;
    U8 ucdq_final_dqm_oen_ui_large[DQS_NUMBER] = {0}, ucdq_final_dqm_oen_ui_small[DQS_NUMBER] = {0};


    #if TX_OE_PATTERN_USE_TA2
    mcSHOW_DBG_MSG(("\n[DramC_TX_OE_Calibration] TA2\n"));
    #else
    mcSHOW_DBG_MSG(("\n[DramC_TX_OE_Calibration] DMA\n"));
    #endif

    
#if (SUPPORT_SAVE_TIME_FOR_CALIBRATION)
    if(p->femmc_Ready==1)
    {
        for(u1ByteIdx=0; u1ByteIdx<DQS_NUMBER_LP4; u1ByteIdx++)
        {
            ucdq_oen_ui_large[u1ByteIdx]= p->pSavetimeData->u1TX_OE_DQ_MCK[p->channel][p->rank][u1ByteIdx];
            ucdq_oen_ui_small[u1ByteIdx]= p->pSavetimeData->u1TX_OE_DQ_UI[p->channel][p->rank][u1ByteIdx];
        }
    }
    else
#endif
    {
        u4RegValue_TXDLY= u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0));
        u4RegValue_dly= u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ2));

        // find smallest DQ byte delay
        for(u1ByteIdx=0; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
        {
            ucdq_ui_large_bak[u1ByteIdx] = (u4RegValue_TXDLY >> (u1ByteIdx*4)) &0x7;
            ucdq_ui_small_bak[u1ByteIdx] = (u4RegValue_dly >> (u1ByteIdx*4)) &0x7;

            u2TempVirtualDelay = (ucdq_ui_large_bak[u1ByteIdx] <<3) + ucdq_ui_small_bak[u1ByteIdx];
            if(u2TempVirtualDelay < u2SmallestVirtualDelay)
            {
                u2SmallestVirtualDelay = u2TempVirtualDelay;
            }

            mcSHOW_DBG_MSG(("Original DQ_B%d (%d %d) =%d, OEN = %d\n", u1ByteIdx, ucdq_ui_large_bak[u1ByteIdx], ucdq_ui_small_bak[u1ByteIdx], u2TempVirtualDelay, u2TempVirtualDelay-TX_DQ_OE_SHIFT_LP4));
        }

        #if TX_OE_PATTERN_USE_TA2
        DramcEngine2Init(p, p->test2_1, p->test2_2, TEST_AUDIO_PATTERN, 0);
        #else
        DramcDmaEngine((DRAMC_CTX_T *)p, 0x50000000, 0x60000000, 0xff00, 8, DMA_PREPARE_DATA_ONLY, p->support_channel_num);
        #endif

        #if TX_OE_SCAN_FULL_RANGE
        // -17~+8 UI
        if(u2SmallestVirtualDelay >= 17)
            u2DQOEN_DelayBegin = u2SmallestVirtualDelay -17;
        else
            u2DQOEN_DelayBegin =0;

        u2DQEN_DelayEnd = u2DQOEN_DelayBegin +25;

        #else // reduce range to speed up
        if(u2SmallestVirtualDelay >= 7)
            u2DQOEN_DelayBegin = u2SmallestVirtualDelay -7;
        else
            u2DQOEN_DelayBegin =0;

        u2DQEN_DelayEnd = u2DQOEN_DelayBegin +10;
        #endif

        for (u2Delay = u2DQOEN_DelayBegin; u2Delay <= u2DQEN_DelayEnd; u2Delay++)
        {
            ucdq_current_ui_large= (u2Delay >>3);
            ucdq_current_ui_small = u2Delay & 0x7;
            //mcSHOW_DBG_MSG(("\nucdq_oen_ui %d %d ", ucdq_oen_ui_large, ucdq_oen_ui_small));

            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0), \
                                            P_Fld(ucdq_current_ui_large, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0) | \
                                            P_Fld(ucdq_current_ui_large, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1) );

            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ1), \
                                            P_Fld(ucdq_current_ui_large, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0) | \
                                            P_Fld(ucdq_current_ui_large, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1));

            // DLY_DQ[2:0]
           vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ2), \
                                            P_Fld(ucdq_current_ui_small, SHURK0_SELPH_DQ2_DLY_OEN_DQ0) | \
                                            P_Fld(ucdq_current_ui_small, SHURK0_SELPH_DQ2_DLY_OEN_DQ1) );

             // DLY_DQM[2:0]
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ3), \
                                             P_Fld(ucdq_current_ui_small, SHURK0_SELPH_DQ3_DLY_OEN_DQM0) | \
                                             P_Fld(ucdq_current_ui_small, SHURK0_SELPH_DQ3_DLY_OEN_DQM1));

            #if TX_OE_PATTERN_USE_TA2
            u4err_value = DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, TEST_AUDIO_PATTERN);
            #else
            u4err_value= DramcDmaEngine((DRAMC_CTX_T *)p, 0x50000000, 0x60000000, 0xff00, 8, DMA_CHECK_DATA_ACCESS_AND_COMPARE, p->support_channel_num);
            #endif

            // 3
            for(u1ByteIdx=0; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
            {
                if (((u4err_value >> (u1ByteIdx<<3)) & 0xff) == 0)
                {
                    if(ucBegin[u1ByteIdx]==0)
                        ucBegin[u1ByteIdx]=1;

                    ucEnd[u1ByteIdx] = u2Delay;
                }
            }

#ifdef ETT_PRINT_FORMAT
            mcSHOW_DBG_MSG(("%d, 0x%X, End_B0=%d End_B1=%d\n", u2Delay, u4err_value, ucEnd[0], ucEnd[1]));
#else
            mcSHOW_DBG_MSG(("TAP=%2d, err_value=0x%8x, End_B0=%d End_B1=%d\n", u2Delay, u4err_value, ucEnd[0], ucEnd[1]));
#endif
            mcFPRINTF((fp_A60501, "TAP=%2d, err_value=0x%8x, End_B0=%d End_B1=%d\n", u2Delay, u4err_value, ucEnd[0], ucEnd[1]));

            if((u4err_value & 0xffff !=0) && ucBegin[0]==1 && ucBegin[1]==1)
                break; // early break;
        }

        #if TX_OE_PATTERN_USE_TA2
        DramcEngine2End(p);
        #endif

        // 4
        for(u1ByteIdx=0; u1ByteIdx<DQS_NUMBER_LP4; u1ByteIdx++)
        {
            if (ucEnd[u1ByteIdx] == 0xff)
            {
                ucbest_step[u1ByteIdx] = u2SmallestVirtualDelay - TX_DQ_OE_SHIFT_LP4;  //bakcup original delay, will be uesed if Pass window not found.
                mcSHOW_ERR_MSG(("Byte %d no TX OE taps pass, calibration fail!\n", u1ByteIdx));
            }
            else // window is larger htan 3
            {
                ucbest_step[u1ByteIdx] = ucEnd[u1ByteIdx] - 3;
            }
            mcSHOW_DBG_MSG(("Byte%d end_step=%d  best_step=%d\n", u1ByteIdx, ucEnd[u1ByteIdx], ucbest_step[u1ByteIdx]));
            mcFPRINTF((fp_A60501, "Byte%d first_step=%d best_step=%d\n", u1ByteIdx, ucEnd[u1ByteIdx], ucbest_step[u1ByteIdx]));

            ucdq_oen_ui_large[u1ByteIdx]= (ucbest_step[u1ByteIdx] >>3);
            ucdq_oen_ui_small[u1ByteIdx] = ucbest_step[u1ByteIdx] & 0x7;
        }
    }

    for(u1ByteIdx=0; u1ByteIdx<DQS_NUMBER_LP4; u1ByteIdx++)
    {
        mcSHOW_DBG_MSG(("Byte%d TX OE(2T, 0.5T) = (%d, %d)\n", u1ByteIdx, ucdq_oen_ui_large[u1ByteIdx], ucdq_oen_ui_small[u1ByteIdx]));
    }
    mcSHOW_DBG_MSG(("\n\n"));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0), \
                                    P_Fld(ucdq_oen_ui_large[0], SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0) | \
                                    P_Fld(ucdq_oen_ui_large[1], SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ1), \
                                    P_Fld(ucdq_oen_ui_large[0], SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0) | \
                                    P_Fld(ucdq_oen_ui_large[1], SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1));
    // DLY_DQ[2:0]
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ2), \
                                    P_Fld(ucdq_oen_ui_small[0], SHURK0_SELPH_DQ2_DLY_OEN_DQ0) | \
                                    P_Fld(ucdq_oen_ui_small[1], SHURK0_SELPH_DQ2_DLY_OEN_DQ1) );
     // DLY_DQM[2:0]
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ3), \
                                     P_Fld(ucdq_oen_ui_small[0], SHURK0_SELPH_DQ3_DLY_OEN_DQM0) | \
                                     P_Fld(ucdq_oen_ui_small[1], SHURK0_SELPH_DQ3_DLY_OEN_DQM1));

    #if (SUPPORT_SAVE_TIME_FOR_CALIBRATION)
    if(p->femmc_Ready==0)
    {
        for(u1ByteIdx=0; u1ByteIdx<DQS_NUMBER_LP4; u1ByteIdx++)
        {
            p->pSavetimeData->u1TX_OE_DQ_MCK[p->channel][p->rank][u1ByteIdx] = ucdq_oen_ui_large[u1ByteIdx];
            p->pSavetimeData->u1TX_OE_DQ_UI[p->channel][p->rank][u1ByteIdx] = ucdq_oen_ui_small[u1ByteIdx];
        }
    }
    #endif

}
#endif



//=============================================================
///// DramC TX perbi calibration ----------Begin--------------
//=============================================================
//-------------------------------------------------------------------------
/** DramcTxWindowPerbitCal (v2)
 *  TX DQS per bit SW calibration.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param  apply           (U8): 0 don't apply the register we set  1 apply the register we set ,default don't apply.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL
 */
//-------------------------------------------------------------------------
#if (SW_CHANGE_FOR_SIMULATION ||FOR_DV_SIMULATION_USED)
#define TX_VREF_RANGE_BEGIN       0
#define TX_VREF_RANGE_END           2 // binary 110010
#define TX_VREF_RANGE_STEP         2
#else
#define TX_VREF_RANGE_BEGIN       16
#define TX_VREF_RANGE_END           50 // binary 110010
#define TX_VREF_RANGE_STEP         2
#endif

#define TX_DQ_UI_TO_PI_TAP         64 // 1 PI = tCK/64, total 128 PI, 1UI = 32 PI
#define TX_PHASE_DQ_UI_TO_PI_TAP         32 // 1 PI = tCK/64, total 128 PI, 1UI = 32 PI for DDR800 semi open loop mode
#define LP4_TX_VREF_DATA_NUM 50
#define LP4_TX_VREF_PASS_CONDITION 0
#define LP4_TX_VREF_BOUNDARY_NOT_READY 0xff

typedef struct _PASS_WIN_DATA_BY_VREF_T
{
    U16 u2VrefUsed;
    U16 u2WinSum_byVref;
    U8 u1WorseBitWinSize_byVref;
    U8 u1WorseBitIdx_byVref;
} PASS_WIN_DATA_BY_VREF_T;


void TxWinTransferDelayToUIPI(DRAMC_CTX_T *p, DRAM_TX_PER_BIT_CALIBRATION_TYTE_T calType, U16 uiDelay, U8 u1AdjustPIToCenter, U8* pu1UILarge_DQ, U8* pu1UISmall_DQ, U8* pu1PI, U8* pu1UILarge_DQOE, U8* pu1UISmall_DQOE)
{
    U8 u1Small_ui_to_large, u1PI = 0, u164PIto1UI;
    U16 u2TmpValue, u2DQOE_shift;;
    DDR800_MODE_T eDdr800Mode = vGet_DDR800_Mode(p);
    U8 u1PiTap = (u1IsDDR800PhaseMode(p) == TRUE) ? TX_PHASE_DQ_UI_TO_PI_TAP : TX_DQ_UI_TO_PI_TAP;


    u1Small_ui_to_large = u1MCK2UI_DivShift(p);

    if(pu1PI != NULL)
    {
        u1PI = uiDelay & (u1PiTap-1);
        *pu1PI =u1PI;
    }

    if (u1IsLP4Div4DDR800(p)) // All DDR800 mode for 1UI carry
        u164PIto1UI = 0;
    else
        u164PIto1UI = 1;

    u2TmpValue = (uiDelay /u1PiTap)<<u164PIto1UI; // 1:8 mode for 2UI carry, DDR800 1:4 mode for 1UI carry

    if(u1AdjustPIToCenter && (pu1PI!=NULL) && (eDdr800Mode == NORMAL_CLOSE_LOOP))
    {
        if(u1PI<10)
        {
            u1PI += (u1PiTap)>>1;
            u2TmpValue --;
        }
        else if(u1PI>u1PiTap-10)
        {
            u1PI -= (u1PiTap)>>1;
            u2TmpValue ++;
        }

        *pu1PI =u1PI;
    }

    #if 0
    *pu1UISmall_DQ = u2TmpValue % u1Small_ui_to_large;
    *pu1UILarge_DQ = u2TmpValue / u1Small_ui_to_large;
    #else
    *pu1UISmall_DQ = u2TmpValue - ((u2TmpValue >> u1Small_ui_to_large) <<u1Small_ui_to_large);
    *pu1UILarge_DQ = (u2TmpValue >> u1Small_ui_to_large);
    #endif
	
    // calculate DQ OE according to DQ UI
    #if TX_K_DQM_WITH_WDBI
    if(calType==TX_DQ_DQS_MOVE_DQM_ONLY)
    {
        if (vGet_Div_Mode(p) == DIV4_MODE)
            u2DQOE_shift = 1; //OE_shift = OE_shift - 3(original OE position) + 4 (MCK)
        else
            u2DQOE_shift = 5; //OE_shift = OE_shift - 3(original OE position) + 8 (MCK)
		   
        u2TmpValue += u2DQOE_shift;
    }
    else
    #endif 
    {
        u2TmpValue -= TX_DQ_OE_SHIFT_LP4;
    }

    *pu1UISmall_DQOE = u2TmpValue - ((u2TmpValue >> u1Small_ui_to_large) <<u1Small_ui_to_large);
    *pu1UILarge_DQOE = (u2TmpValue >> u1Small_ui_to_large);
}

static void TxPrintWidnowInfo(DRAMC_CTX_T *p, PASS_WIN_DATA_T WinPerBitData[])
{
    U8 u1BitIdx;

    for (u1BitIdx = 0; u1BitIdx < DQS_BIT_NUMBER; u1BitIdx++)
    {
    #ifdef ETT_PRINT_FORMAT
        mcSHOW_DBG_MSG(("TX Bit%d (%d~%d) %d %d,   Bit%d (%d~%d) %d %d,", \
            u1BitIdx, WinPerBitData[u1BitIdx].first_pass, WinPerBitData[u1BitIdx].last_pass, WinPerBitData[u1BitIdx].win_size, WinPerBitData[u1BitIdx].win_center, \
            u1BitIdx+8, WinPerBitData[u1BitIdx+8].first_pass, WinPerBitData[u1BitIdx+8].last_pass, WinPerBitData[u1BitIdx+8].win_size, WinPerBitData[u1BitIdx+8].win_center));
    #else
        mcSHOW_DBG_MSG(("TX Bit%2d (%2d~%2d) %2d %2d,   Bit%2d (%2d~%2d) %2d %2d,", \
            u1BitIdx, WinPerBitData[u1BitIdx].first_pass, WinPerBitData[u1BitIdx].last_pass, WinPerBitData[u1BitIdx].win_size, WinPerBitData[u1BitIdx].win_center, \
            u1BitIdx+8, WinPerBitData[u1BitIdx+8].first_pass, WinPerBitData[u1BitIdx+8].last_pass, WinPerBitData[u1BitIdx+8].win_size, WinPerBitData[u1BitIdx+8].win_center));
    #endif
        mcFPRINTF((fp_A60501,"TX Bit%2d (%2d~%2d) %2d %2d,   Bit%2d (%2d~%2d) %2d %2d,", \
            u1BitIdx, WinPerBitData[u1BitIdx].first_pass, WinPerBitData[u1BitIdx].last_pass, WinPerBitData[u1BitIdx].win_size, WinPerBitData[u1BitIdx].win_center, \
            u1BitIdx+8, WinPerBitData[u1BitIdx+8].first_pass, WinPerBitData[u1BitIdx+8].last_pass, WinPerBitData[u1BitIdx+8].win_size, WinPerBitData[u1BitIdx+8].win_center));

        mcSHOW_DBG_MSG(("\n"));
        mcFPRINTF((fp_A60501,"\n"));
    }
    mcSHOW_DBG_MSG(("\n"));
    mcFPRINTF((fp_A60501,"\n"));
}


static void TXPerbitCalibrationInit(DRAMC_CTX_T *p, U8 calType)
{
    //Set TX delay chain to 0
    if(calType !=TX_DQ_DQS_MOVE_DQM_ONLY)
    {
    #if 1    
    #if PINMUX_AUTO_TEST_PER_BIT_TX
        if(gTX_check_per_bit_flag == 1)
        {
            //not reset delay cell
        }
        else
    #endif
        {
            vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ0), 0);
            vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ0), 0);
        }
    #else
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ0), P_Fld(0x0, SHU_R0_B0_DQ0_RK0_TX_ARDQ7_DLY_B0) 
            | P_Fld(0x0, SHU_R0_B0_DQ0_RK0_TX_ARDQ6_DLY_B0) 
            | P_Fld(0x0, SHU_R0_B0_DQ0_RK0_TX_ARDQ5_DLY_B0) 
            | P_Fld(0x0, SHU_R0_B0_DQ0_RK0_TX_ARDQ4_DLY_B0) 
            | P_Fld(0x0, SHU_R0_B0_DQ0_RK0_TX_ARDQ3_DLY_B0) 
            | P_Fld(0x0, SHU_R0_B0_DQ0_RK0_TX_ARDQ2_DLY_B0) 
            | P_Fld(0x0, SHU_R0_B0_DQ0_RK0_TX_ARDQ1_DLY_B0) 
            | P_Fld(0x0, SHU_R0_B0_DQ0_RK0_TX_ARDQ0_DLY_B0));    
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ0), P_Fld(0x0, SHU_R0_B1_DQ0_RK0_TX_ARDQ7_DLY_B1) 
            | P_Fld(0x0, SHU_R0_B1_DQ0_RK0_TX_ARDQ6_DLY_B1) 
            | P_Fld(0x0, SHU_R0_B1_DQ0_RK0_TX_ARDQ5_DLY_B1) 
            | P_Fld(0x0, SHU_R0_B1_DQ0_RK0_TX_ARDQ4_DLY_B1) 
            | P_Fld(0x0, SHU_R0_B1_DQ0_RK0_TX_ARDQ3_DLY_B1) 
            | P_Fld(0x0, SHU_R0_B1_DQ0_RK0_TX_ARDQ2_DLY_B1) 
            | P_Fld(0x0, SHU_R0_B1_DQ0_RK0_TX_ARDQ1_DLY_B1) 
            | P_Fld(0x0, SHU_R0_B1_DQ0_RK0_TX_ARDQ0_DLY_B1));
    #endif
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ1), 0x0, SHU_R0_B0_DQ1_RK0_TX_ARDQM0_DLY_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ1), 0x0, SHU_R0_B1_DQ1_RK0_TX_ARDQM0_DLY_B1);
    }


    //Use HW TX tracking value
    //R_DMARPIDQ_SW :drphy_conf (0x170[7])(default set 1)
    //   0: DQS2DQ PI setting controlled by HW
    //R_DMARUIDQ_SW : Dramc_conf(0x156[15])(default set 1)
    //    0: DQS2DQ UI setting controlled by HW
    ///TODO: need backup original setting?
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 1, MISC_CTRL1_R_DMARPIDQ_SW);
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR), 1, DQSOSCR_ARUIDQ_SW);

}

#define TX_TDQS2DQ_PRE_CAL 1
#if TX_TDQS2DQ_PRE_CAL
//  (1) DDR800 1:4 mode
//  (2) DDR1200/1600 1:4 mode
//  (3) 1:8 mode
// The 3 condition have different MCK2UI/UI2PI. Therefore, TX DQS2DQ should be record separately.
// Here, we record (2) and (3).  DDR800 1:4 skip recording DQS2DQ.
U16 u2DQS2DQ_Pre_Cal[CHANNEL_NUM][RANK_MAX][2/*DIV_Mode*/]={0};
#endif

static void TXScanRange_PI(DRAMC_CTX_T *p, DRAM_TX_PER_BIT_CALIBRATION_TYTE_T calType, U16 *pu2Begin, U16 *pu2End)
{
    U8 u1MCK2UI, u1UI2PI, u1ByteIdx;
    U32 u4RegValue_TXDLY, u4RegValue_dly;
    U8 ucdq_ui_large_bak[DQS_NUMBER], ucdq_ui_small_bak[DQS_NUMBER];
    U16 u2TempVirtualDelay, u2SmallestVirtualDelay=0xffff; 
    U16 u2DQDelayBegin=0, u2DQDelayEnd=0;

    u4RegValue_TXDLY= u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_DQS0));
    u4RegValue_dly= u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_DQS1));

    u1MCK2UI = u1MCK2UI_DivShift(p);

    if (vGet_DDR800_Mode(p) == DDR800_CLOSE_LOOP)
        u1UI2PI = 6;
    else
        u1UI2PI = 5;


    // find smallest DQS delay
    for(u1ByteIdx=0; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
    {
        ucdq_ui_large_bak[u1ByteIdx] = (u4RegValue_TXDLY >> (u1ByteIdx<<2)) &0x7;// MCK
        ucdq_ui_small_bak[u1ByteIdx] = (u4RegValue_dly >> (u1ByteIdx<<2)) &0x7;// UI
        //wrlevel_dqs_final_delay[u1ByteIdx]  ==> PI

        //LP4 : Virtual Delay = 256 * MCK + 32*UI + PI;
        //LP3 : Virtual Delay = 128 * MCK + 32*UI + PI;
        u2TempVirtualDelay = (((ucdq_ui_large_bak[u1ByteIdx] <<u1MCK2UI) + ucdq_ui_small_bak[u1ByteIdx])<<u1UI2PI) + wrlevel_dqs_final_delay[u1ByteIdx];

        if(u2TempVirtualDelay < u2SmallestVirtualDelay)
        {
            u2SmallestVirtualDelay = u2TempVirtualDelay;
        }
        
        //mcSHOW_DBG_MSG2(("Original DQS_B%d VirtualDelay %d = (%d %d %d)\n", u1ByteIdx, u2TempVirtualDelay,\
        //                ucdq_ui_large_bak[u1ByteIdx], ucdq_ui_small_bak[u1ByteIdx], wrlevel_dqs_final_delay[u1ByteIdx]));
    }
  
    u2DQDelayBegin = u2SmallestVirtualDelay;

    #if TX_TDQS2DQ_PRE_CAL
    if (u1IsLP4Div4DDR800(p) == FALSE)
    {
        if(u2DQS2DQ_Pre_Cal[p->channel][p->rank][vGet_Div_Mode(p)]> 0)
        {
            U16 u2TmpShift;
            mcSHOW_DBG_MSG(("TX_TDQS2DQ_PRE_CAL : change DQ begin %d -->", u2DQDelayBegin));
            
            u2TmpShift = (u2DQS2DQ_Pre_Cal[p->channel][p->rank][vGet_Div_Mode(p)]* p->frequency) /1000;
            if(u2TmpShift>=15)
                u2TmpShift -=15;
            else
                u2TmpShift =0;
            
            u2DQDelayBegin += u2TmpShift;                   
            mcSHOW_DBG_MSG(("%d (+%d)\n", u2DQDelayBegin, u2TmpShift));
        }
    }
    #endif

    #if TX_K_DQM_WITH_WDBI
    if(calType==TX_DQ_DQS_MOVE_DQM_ONLY)
    {
        // DBI on, calibration range -1MCK
        u2DQDelayBegin -=(1<<(u1MCK2UI+5));
    }
    #endif                
    u2DQDelayEnd = u2DQDelayBegin + 256;             

    *pu2Begin = u2DQDelayBegin;
    *pu2End = u2DQDelayEnd;

    #if 0//TX_TDQS2DQ_PRE_CAL
    mcSHOW_DBG_MSG(("TXScanRange_PI %d~%d\n", u2DQDelayBegin,u2DQDelayEnd));
    #endif
}


static void TXScanRange_Vref(DRAMC_CTX_T *p, U8 u1VrefScanEnable, U16* pu2Range, U16 *pu2Begin, U16 *pu2End, U16 *pu2Setp)
{
    U16 u2VrefBegin, u2FinalRange, u2VrefEnd;

    if(u1VrefScanEnable)
    {
    #if (SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_VREF_CAL)
        if(p->femmc_Ready==1)
        {
            // if fast K, use TX Vref that saved.
            u2VrefBegin = p->pSavetimeData->u1TxWindowPerbitVref_Save[p->channel][p->rank]; 
            u2VrefEnd = u2VrefBegin+1;
        }
        else
    #endif
        {
            if (p->odt_onoff == ODT_OFF)
            {      
                if (p->dram_type == TYPE_LPDDR4)            
                {
                    //range 1          
                    u2VrefBegin = 13 - 5; // 300/1100(VDDQ) = 27.2%
                    u2VrefEnd = 13 + 5; 
                }
                else
                {
                    //range 1          
                    u2VrefBegin = 27 - 5; // 290/600(VDDQ)=48.3%
                    u2VrefEnd = 27 + 5;                 
                }
            }
            else
            {
                // range 0
                u2VrefBegin = TX_VREF_RANGE_BEGIN;
                u2VrefEnd = TX_VREF_RANGE_END;
            }
        }
    }
    else //LPDDR3, the for loop will only excute u2VrefLevel=TX_VREF_RANGE_END/2.
    {
        u2VrefBegin = 0;
        u2VrefEnd = 0;                
    }
    
    *pu2Range = (!p->odt_onoff);
    *pu2Begin = u2VrefBegin;
    *pu2End = u2VrefEnd;
    *pu2Setp = TX_VREF_RANGE_STEP;

}

static U16 TxChooseVref(DRAMC_CTX_T *p, PASS_WIN_DATA_BY_VREF_T pVrefInfo[], U8 u1VrefNum)
{
    U8 u1VrefIdx, u1WorseBitIdx=0, u1WinSizeOfWorseBit=0;
    U8 u1VrefPassBegin=LP4_TX_VREF_BOUNDARY_NOT_READY, u1VrefPassEnd=LP4_TX_VREF_BOUNDARY_NOT_READY, u1TempPassNum=0, u1MaxVerfPassNum=0;
    U8 u1VrefPassBegin_Final=LP4_TX_VREF_BOUNDARY_NOT_READY, u1VrefPassEnd_Final=LP4_TX_VREF_BOUNDARY_NOT_READY;
    U16 u2MaxWinSum=0;
    U16 u2FinalVref=0;

    for(u1VrefIdx=0; u1VrefIdx < u1VrefNum; u1VrefIdx++)
    {
        mcSHOW_DBG_MSG(("TX Vref=%d, minBit %d, minWin=%d, winSum=%d\n", 
            pVrefInfo[u1VrefIdx].u2VrefUsed,
            pVrefInfo[u1VrefIdx].u1WorseBitIdx_byVref,
            pVrefInfo[u1VrefIdx].u1WorseBitWinSize_byVref, 
            pVrefInfo[u1VrefIdx].u2WinSum_byVref));
        
        #if LP4_TX_VREF_PASS_CONDITION
        if((pVrefInfo[u1VrefIdx].u1WorseBitWinSize_byVref > LP4_TX_VREF_PASS_CONDITION))
        {
            if(u1VrefPassBegin ==LP4_TX_VREF_BOUNDARY_NOT_READY)
            {
                u1VrefPassBegin = pVrefInfo[u1VrefIdx].u2VrefUsed;
                u1TempPassNum =1;
            }
            else
                u1TempPassNum ++;

            if(u1VrefIdx==u1VrefNum-1)
            {
                u1VrefPassEnd = pVrefInfo[u1VrefIdx].u2VrefUsed;
                if(u1TempPassNum > u1MaxVerfPassNum)
                {
                    u1VrefPassBegin_Final= u1VrefPassBegin;
                    u1VrefPassEnd_Final = u1VrefPassEnd;
                    u1MaxVerfPassNum= u1TempPassNum;
                }
            }
        }
        else
        {
            if((u1VrefPassBegin != LP4_TX_VREF_BOUNDARY_NOT_READY) && (u1VrefPassEnd==LP4_TX_VREF_BOUNDARY_NOT_READY))
            {
                u1VrefPassEnd = pVrefInfo[u1VrefIdx].u2VrefUsed-TX_VREF_RANGE_STEP;
                if(u1TempPassNum > u1MaxVerfPassNum)
                {
                    u1VrefPassBegin_Final= u1VrefPassBegin;
                    u1VrefPassEnd_Final = u1VrefPassEnd;
                    u1MaxVerfPassNum= u1TempPassNum;
                }
                u1VrefPassBegin=0xff;
                u1VrefPassEnd=0xff;
                u1TempPassNum =0;
            }
        }
        #endif
    }

    #if LP4_TX_VREF_PASS_CONDITION
    //if((u1VrefPassBegin_Final !=LP4_TX_VREF_BOUNDARY_NOT_READY) && (u1VrefPassEnd_Final!=LP4_TX_VREF_BOUNDARY_NOT_READY))  
    if(u1MaxVerfPassNum>0)
    {   
        // vref pass window found
        u2FinalVref = (u1VrefPassBegin_Final + u1VrefPassEnd_Final) >>1;
        mcSHOW_DBG_MSG(("[TxChooseVref] Window > %d, Vref (%d~%d), Final Vref %d\n",LP4_TX_VREF_PASS_CONDITION, u1VrefPassBegin_Final, u1VrefPassEnd_Final, u2FinalVref));
    }
    else
    #endif
    {
        // not vref found
        for(u1VrefIdx=0; u1VrefIdx < u1VrefNum; u1VrefIdx++)
        {
            if((pVrefInfo[u1VrefIdx].u1WorseBitWinSize_byVref > u1WinSizeOfWorseBit) ||
                ((pVrefInfo[u1VrefIdx].u1WorseBitWinSize_byVref == u1WinSizeOfWorseBit) && (pVrefInfo[u1VrefIdx].u2WinSum_byVref > u2MaxWinSum)))
            {
                u1WinSizeOfWorseBit = pVrefInfo[u1VrefIdx].u1WorseBitWinSize_byVref;
                u1WorseBitIdx = pVrefInfo[u1VrefIdx].u1WorseBitIdx_byVref;
                u2MaxWinSum = pVrefInfo[u1VrefIdx].u2WinSum_byVref;
                u2FinalVref = pVrefInfo[u1VrefIdx].u2VrefUsed;
            }
        }

        mcSHOW_DBG_MSG(("[TxChooseVref] Worse bit %d, Min win %d, Win sum %d, Final Vref %d\n", u1WorseBitIdx, u1WinSizeOfWorseBit, u2MaxWinSum, u2FinalVref));
    }
   
    return u2FinalVref;
}


void DramcTXSetVref(DRAMC_CTX_T *p, U8 u1VrefRange, U8 u1VrefValue)
{
    U8 u1TempOPValue = ((u1VrefValue & 0x3f) | (u1VrefRange<<6));
    
    u1MR14Value[p->channel][p->rank][p->dram_fsp] = u1TempOPValue;
    DramcModeRegWriteByRank(p, p->rank, 14, u1TempOPValue);  

    #if CALIBRATION_SPEED_UP_DEBUG
    mcSHOW_DBG_MSG(("Yulia TX Vref : CH%d Rank%d, TX Range %d Vref %d\n\n",p->channel, p->rank, u1VrefRange, (u1VrefValue & 0x3f)));
    #endif
}


static void TXSetFinalVref(DRAMC_CTX_T *p, U16 u2FinalRange, U16 u2FinalVref)
{  
    DramcTXSetVref(p, u2FinalRange, u2FinalVref);
    
#ifdef FOR_HQA_TEST_USED
    gFinalTXVrefDQ[p->channel][p->rank] = (U8) u2FinalVref;
#endif

#if VENDER_JV_LOG
    mcSHOW_DBG_MSG5(("\nFinal TX Range %d Vref %d\n\n", u2FinalRange, u2FinalVref));
#else
    mcSHOW_DBG_MSG(("\nFinal TX Range %d Vref %d\n\n", u2FinalRange, u2FinalVref));
    mcFPRINTF((fp_A60501, "\nFinal TX Range %d Vref %d\n\n", u2FinalRange, u2FinalVref));
#endif

    #if CALIBRATION_SPEED_UP_DEBUG
    mcSHOW_DBG_MSG(("Yulia TX Vref Final: CH%d Rank%d, TX Range %d Vref %d\n\n",p->channel, p->rank, u2FinalRange, u2FinalVref));
    #endif
}


#if ENABLE_TX_TRACKING
static void TXUpdateTXTracking(DRAMC_CTX_T *p, DRAM_TX_PER_BIT_CALIBRATION_TYTE_T calType, U8 ucdq_pi[], U8 ucdqm_pi[])
{
     if(calType == TX_DQ_DQS_MOVE_DQ_ONLY || calType ==TX_DQ_DQS_MOVE_DQM_ONLY)
     {
         //make a copy to dramc reg for TX DQ tracking used
         if(calType ==TX_DQ_DQS_MOVE_DQ_ONLY)
         {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_PI),
                            P_Fld(ucdq_pi[0], SHURK0_PI_RK0_ARPI_DQ_B0) | P_Fld(ucdq_pi[1], SHURK0_PI_RK0_ARPI_DQ_B1));
            
            // Source DQ
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQS2DQ_CAL1),
                                                    P_Fld(ucdq_pi[1], SHURK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ1) |
                                                    P_Fld(ucdq_pi[0], SHURK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ0));
            // Target DQ
             vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQS2DQ_CAL2),
                                                     P_Fld(ucdq_pi[1], SHURK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ1) |
                                                     P_Fld(ucdq_pi[0], SHURK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ0));
         }
    
         //if(calType ==TX_DQ_DQS_MOVE_DQM_ONLY || (calType ==TX_DQ_DQS_MOVE_DQ_ONLY))
         {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_PI),
                            P_Fld(ucdqm_pi[0], SHURK0_PI_RK0_ARPI_DQM_B0) | P_Fld(ucdqm_pi[1], SHURK0_PI_RK0_ARPI_DQM_B1));
           
            // Target DQM
             vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQS2DQ_CAL5),
                                                 P_Fld(ucdqm_pi[1], SHURK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM1) |
                                                 P_Fld(ucdqm_pi[0], SHURK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM0));                     
         }
     }

     
#if 0// for LP3 , TX tracking will be disable, don't need to set DQ delay in DramC.
     ///TODO: check LP3 byte mapping of dramC
     vIO32WriteFldMulti(DRAMC_REG_SHURK0_PI+(CHANNEL_A<< POS_BANK_NUM), \
                              P_Fld(ucdq_final_pi[0], SHURK0_PI_RK0_ARPI_DQ_B0) | P_Fld(ucdq_final_pi[1], SHURK0_PI_RK0_ARPI_DQ_B1));
     
     vIO32WriteFldMulti(DRAMC_REG_SHURK0_PI+SHIFT_TO_CHB_ADDR, \
                              P_Fld(ucdq_final_pi[2], SHURK0_PI_RK0_ARPI_DQ_B0) | P_Fld(ucdq_final_pi[3], SHURK0_PI_RK0_ARPI_DQ_B1));
#endif

}
#endif //End ENABLE_TX_TRACKING


static void TXSetDelayReg_DQ(DRAMC_CTX_T *p, U8 u1UpdateRegUI, U8 ucdq_ui_large[], U8 ucdq_oen_ui_large[], U8 ucdq_ui_small[], U8 ucdq_oen_ui_small[], U8 ucdql_pi[])
{
    if(u1UpdateRegUI)
    {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0), \
                                     P_Fld(ucdq_ui_large[0], SHURK0_SELPH_DQ0_TXDLY_DQ0) |
                                     P_Fld(ucdq_ui_large[1], SHURK0_SELPH_DQ0_TXDLY_DQ1) |
                                     P_Fld(ucdq_ui_large[2], SHURK0_SELPH_DQ0_TXDLY_DQ2) |
                                     P_Fld(ucdq_ui_large[3], SHURK0_SELPH_DQ0_TXDLY_DQ3) |
                                     P_Fld(ucdq_oen_ui_large[0], SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0) |
                                     P_Fld(ucdq_oen_ui_large[1], SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1) |
                                     P_Fld(ucdq_oen_ui_large[2], SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2) |
                                     P_Fld(ucdq_oen_ui_large[3], SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3));

        // DLY_DQ[2:0]
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ2), \
                                     P_Fld(ucdq_ui_small[0], SHURK0_SELPH_DQ2_DLY_DQ0) |
                                     P_Fld(ucdq_ui_small[1], SHURK0_SELPH_DQ2_DLY_DQ1) |
                                     P_Fld(ucdq_ui_small[2], SHURK0_SELPH_DQ2_DLY_DQ2) |
                                     P_Fld(ucdq_ui_small[3], SHURK0_SELPH_DQ2_DLY_DQ3) |
                                     P_Fld(ucdq_oen_ui_small[0], SHURK0_SELPH_DQ2_DLY_OEN_DQ0) |
                                     P_Fld(ucdq_oen_ui_small[1], SHURK0_SELPH_DQ2_DLY_OEN_DQ1) |
                                     P_Fld(ucdq_oen_ui_small[2], SHURK0_SELPH_DQ2_DLY_OEN_DQ2) |
                                     P_Fld(ucdq_oen_ui_small[3], SHURK0_SELPH_DQ2_DLY_OEN_DQ3));
    }
    
    #if ENABLE_DDR800_OPEN_LOOP_MODE_OPTION
    if (u1IsDDR800PhaseMode(p) == TRUE)
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ9), (ucdql_pi[0]>>3), SHU_B0_DQ9_RG_DDR400_DQ_PS_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B1_DQ9), (ucdql_pi[1]>>3), SHU_B1_DQ9_RG_DDR400_DQ_PS_B1);
    }
    else
    #endif
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ7), ucdql_pi[0], SHU_R0_B0_DQ7_RK0_ARPI_DQ_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ7), ucdql_pi[1], SHU_R0_B1_DQ7_RK0_ARPI_DQ_B1);    
    }
}


static void TXSetDelayReg_DQM(DRAMC_CTX_T *p, U8 u1UpdateRegUI, U8 ucdqm_ui_large[], U8 ucdqm_oen_ui_large[], U8 ucdqm_ui_small[], U8 ucdqm_oen_ui_small[], U8 ucdqm_pi[])
{
    if(u1UpdateRegUI)
    {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ1),
                                     P_Fld(ucdqm_ui_large[0], SHURK0_SELPH_DQ1_TXDLY_DQM0) |
                                     P_Fld(ucdqm_ui_large[1], SHURK0_SELPH_DQ1_TXDLY_DQM1) |
                                     P_Fld(ucdqm_ui_large[2], SHURK0_SELPH_DQ1_TXDLY_DQM2) |
                                     P_Fld(ucdqm_ui_large[3], SHURK0_SELPH_DQ1_TXDLY_DQM3) |
                                     P_Fld(ucdqm_oen_ui_large[0], SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0) |
                                     P_Fld(ucdqm_oen_ui_large[1], SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1) |
                                     P_Fld(ucdqm_oen_ui_large[2], SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2) |
                                     P_Fld(ucdqm_oen_ui_large[3], SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3));
        
         // DLY_DQM[2:0]
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ3),
                                     P_Fld(ucdqm_ui_small[0], SHURK0_SELPH_DQ3_DLY_DQM0) |
                                     P_Fld(ucdqm_ui_small[1], SHURK0_SELPH_DQ3_DLY_DQM1) |
                                     P_Fld(ucdqm_ui_small[2], SHURK0_SELPH_DQ3_DLY_DQM2) |
                                     P_Fld(ucdqm_ui_small[3], SHURK0_SELPH_DQ3_DLY_DQM3) |
                                     P_Fld(ucdqm_oen_ui_small[0], SHURK0_SELPH_DQ3_DLY_OEN_DQM0) |
                                     P_Fld(ucdqm_oen_ui_small[1], SHURK0_SELPH_DQ3_DLY_OEN_DQM1) |
                                     P_Fld(ucdqm_oen_ui_small[2], SHURK0_SELPH_DQ3_DLY_OEN_DQM2) |
                                     P_Fld(ucdqm_oen_ui_small[3], SHURK0_SELPH_DQ3_DLY_OEN_DQM3));
    }
    
    if(u1IsDDR800PhaseMode(p) == FALSE)
    {   
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ7), ucdqm_pi[0], SHU_R0_B0_DQ7_RK0_ARPI_DQM_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ7), ucdqm_pi[1], SHU_R0_B1_DQ7_RK0_ARPI_DQM_B1);
    }
}

DRAM_STATUS_T DramcTxWindowPerbitCal(DRAMC_CTX_T *p, DRAM_TX_PER_BIT_CALIBRATION_TYTE_T calType, U8 u1VrefScanEnable)
{
    U8 u1BitTemp, u1BitIdx, u1ByteIdx, u1RankIdx, backup_rank;
    U32 uiFinishCount;
    PASS_WIN_DATA_T WinPerBit[DQ_DATA_WIDTH], VrefWinPerBit[DQ_DATA_WIDTH], FinalWinPerBit[DQ_DATA_WIDTH];
    
    U16 uiDelay, u2DQDelayBegin, u2DQDelayEnd, u2DQDelayStep;
    
    U8 ucdq_pi, ucdq_ui_small, ucdq_ui_large,ucdq_oen_ui_small, ucdq_oen_ui_large;
    U8 ucdq_ui_small_reg_value, u1UpdateRegUI;  // for UI and TXDLY change check, if different , set reg.
    
    U8 ucdq_reg_pi[DQS_NUMBER], ucdq_reg_ui_large[DQS_NUMBER], ucdq_reg_ui_small[DQS_NUMBER];
    U8 ucdq_reg_oen_ui_large[DQS_NUMBER], ucdq_reg_oen_ui_small[DQS_NUMBER];

    U8 ucdq_reg_dqm_pi[DQS_NUMBER] = {0}, ucdq_reg_dqm_ui_large[DQS_NUMBER] = {0}, ucdq_reg_dqm_ui_small[DQS_NUMBER] = {0};
    U8 ucdq_reg_dqm_oen_ui_large[DQS_NUMBER] = {0}, ucdq_reg_dqm_oen_ui_small[DQS_NUMBER] = {0};

    #if 1//TX_DQM_CALC_MAX_MIN_CENTER
    U16 u2DQM_Delay;  // LP4 only
    U16 u2Center_min[DQS_NUMBER]={0},u2Center_max[DQS_NUMBER]={0};
    #endif
    U8 u1EnableDelayCell=0, u1DelayCellOfst[DQ_DATA_WIDTH]={0};
    U32 u4err_value, u4fail_bit;
    U16 u2FinalRange=0, u2FinalVref;
    U16 u2VrefLevel, u2VrefBegin, u2VrefEnd, u2VrefStep;
    U16 u2TempWinSum, u2MaxWindowSum=0;//, u2tx_window_sum[LP4_TX_VREF_DATA_NUM]={0};
    U32 u4TempRegValue;
    U8 u1min_bit, u1min_winsize=0;
    U8 u1VrefIdx =0;
    U8 u1PIDiff;
    PASS_WIN_DATA_BY_VREF_T VrefInfo[LP4_TX_VREF_DATA_NUM];
    
    if (!p)
    {
        mcSHOW_ERR_MSG(("context NULL\n"));
        return DRAM_FAIL;
    }

#if VENDER_JV_LOG
    if(calType == TX_DQ_DQS_MOVE_DQ_ONLY)
        vPrintCalibrationBasicInfo_ForJV(p);
#else
        vPrintCalibrationBasicInfo(p);
#endif

    backup_rank = u1GetRank(p);

    TXPerbitCalibrationInit(p, calType);
    TXScanRange_PI(p, calType, &u2DQDelayBegin, &u2DQDelayEnd);
    TXScanRange_Vref(p, u1VrefScanEnable, &u2FinalRange, &u2VrefBegin, &u2VrefEnd, &u2VrefStep);

    if (u1IsDDR800PhaseMode(p) == TRUE)
        u2DQDelayStep = (1 << 3);
    else if(calType ==TX_DQ_DQS_MOVE_DQ_DQM)
        u2DQDelayStep= 2;
    else
        u2DQDelayStep = 1;
    
    vSetCalibrationResult(p, DRAM_CALIBRATION_TX_PERBIT, DRAM_FAIL);

#if 0
    mcSHOW_DBG_MSG(("[TxWindowPerbitCal] calType=%d, VrefScanEnable %d (Range %d,  VrefBegin %d, u2VrefEnd %d)\n"
                    "\nBegin, DQ Scan Range %d~%d\n",
                    calType, u1VrefScanEnable, u2FinalRange, u2VrefBegin, u2VrefEnd, u2DQDelayBegin, u2DQDelayEnd));

    mcFPRINTF((fp_A60501, "[TxWindowPerbitCal] calType=%d, VrefScanEnable %d\n"
                    "\nBegin, DQ Scan Range %d~%d\n",
                    calType, u1VrefScanEnable, u2DQDelayBegin, u2DQDelayEnd));
#endif
                
    #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    if(p->femmc_Ready==1 && (p->Bypass_TXWINDOW))
    {
        for (u1ByteIdx=0; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
        {
            u2Center_min[u1ByteIdx]=p->pSavetimeData->u1TxCenter_min_Save[p->channel][p->rank][u1ByteIdx];
            u2Center_max[u1ByteIdx]=p->pSavetimeData->u1TxCenter_max_Save[p->channel][p->rank][u1ByteIdx];
            
            for (u1BitIdx=0; u1BitIdx<DQS_BIT_NUMBER; u1BitIdx++)
            {
                u1BitTemp = u1ByteIdx*DQS_BIT_NUMBER+u1BitIdx;
                FinalWinPerBit[u1BitTemp].win_center= p->pSavetimeData->u1Txwin_center_Save[p->channel][p->rank][u1BitTemp];
            }
        }
        
        vSetCalibrationResult(p, DRAM_CALIBRATION_TX_PERBIT, DRAM_OK);
    }
    else
    #endif
    {
        DramcEngine2Init(p, p->test2_1, p->test2_2, p->test_pattern, 0);

        for(u2VrefLevel = u2VrefBegin; u2VrefLevel <= u2VrefEnd; u2VrefLevel += u2VrefStep)
        {
            // SET tx Vref (DQ) here, LP3 no need to set this.
            if(u1VrefScanEnable)
            {
                #if (!REDUCE_LOG_FOR_PRELOADER)
                mcSHOW_DBG_MSG(("\n\n\tLP4 TX VrefRange %d, VrefLevel=%d\n", u2FinalRange, u2VrefLevel));
                mcFPRINTF((fp_A60501, "\n\n\tLP4 TX VrefRange %d,VrefLevel=%d\n", u2FinalRange, u2VrefLevel));
                #endif

                #if VENDER_JV_LOG
                if(calType ==TX_DQ_DQS_MOVE_DQ_ONLY)
                {
                    mcSHOW_DBG_MSG5(("\n\tLP4 TX VrefRange %d, VrefLevel=%d\n", u2FinalRange, u2VrefLevel));
                }
                #endif

                DramcTXSetVref(p, u2FinalRange, u2VrefLevel);
            }
            else
            {
                mcSHOW_DBG_MSG(("\n\n\tTX Vref Scan disable\n"));
                mcFPRINTF((fp_A60501, "\n\n\tTX Vref Scan disable\n"));
            }

            // initialize parameters
            uiFinishCount = 0;
            u2TempWinSum =0;
            ucdq_ui_small_reg_value = 0xff;

            for (u1BitIdx = 0; u1BitIdx < p->data_width; u1BitIdx++)
            {
                WinPerBit[u1BitIdx].first_pass = (S16)PASS_RANGE_NA;
                WinPerBit[u1BitIdx].last_pass = (S16)PASS_RANGE_NA;
                VrefWinPerBit[u1BitIdx].first_pass = (S16)PASS_RANGE_NA;
                VrefWinPerBit[u1BitIdx].last_pass = (S16)PASS_RANGE_NA;
            }

            //Move DQ delay ,  1 PI = tCK/64, total 128 PI, 1UI = 32 PI
            //For data rate 3200, max tDQS2DQ is 2.56UI (82 PI)
            //For data rate 4266, max tDQS2DQ is 3.41UI (109 PI)
            for (uiDelay = u2DQDelayBegin; uiDelay <u2DQDelayEnd; uiDelay+= u2DQDelayStep)
            {
                TxWinTransferDelayToUIPI(p, calType, uiDelay, 0, &ucdq_ui_large, &ucdq_ui_small, &ucdq_pi, &ucdq_oen_ui_large, &ucdq_oen_ui_small);

                // Check if TX UI changed, if not change , don't need to set reg again
                if(ucdq_ui_small_reg_value != ucdq_ui_small)
                {
                    u1UpdateRegUI=1;
                    ucdq_ui_small_reg_value = ucdq_ui_small;
                }
                else
                    u1UpdateRegUI=0;
                
                for(u1ByteIdx=0; u1ByteIdx < DQS_NUMBER; u1ByteIdx++)
                {
                    if(u1UpdateRegUI)
                    {
                        ucdq_reg_ui_large[u1ByteIdx] = ucdq_ui_large;
                        ucdq_reg_ui_small[u1ByteIdx] = ucdq_ui_small;
                        ucdq_reg_oen_ui_large[u1ByteIdx] = ucdq_oen_ui_large;
                        ucdq_reg_oen_ui_small[u1ByteIdx] = ucdq_oen_ui_small;    
                        
                        ucdq_reg_dqm_ui_large[u1ByteIdx] = ucdq_ui_large;
                        ucdq_reg_dqm_ui_small[u1ByteIdx] = ucdq_ui_small;
                        ucdq_reg_dqm_oen_ui_large[u1ByteIdx] = ucdq_oen_ui_large;
                        ucdq_reg_dqm_oen_ui_small[u1ByteIdx] = ucdq_oen_ui_small;                   
                    }
                    
                    ucdq_reg_pi[u1ByteIdx] = ucdq_pi;
                    ucdq_reg_dqm_pi[u1ByteIdx] = ucdq_pi;
                }

                if(calType ==TX_DQ_DQS_MOVE_DQ_ONLY || calType== TX_DQ_DQS_MOVE_DQ_DQM)
                {
                    TXSetDelayReg_DQ(p, u1UpdateRegUI, ucdq_reg_ui_large, ucdq_reg_oen_ui_large, ucdq_reg_ui_small, ucdq_reg_oen_ui_small, ucdq_reg_pi);
                }

                if(calType ==TX_DQ_DQS_MOVE_DQM_ONLY || calType== TX_DQ_DQS_MOVE_DQ_DQM)
                {
                    TXSetDelayReg_DQM(p, u1UpdateRegUI, ucdq_reg_dqm_ui_large, ucdq_reg_dqm_oen_ui_large, ucdq_reg_dqm_ui_small, ucdq_reg_dqm_oen_ui_small, ucdq_reg_dqm_pi);
                }

                // audio +xtalk pattern
                u4err_value=0;
                DramcEngine2SetPat(p,TEST_AUDIO_PATTERN, 0,0);
                u4err_value = DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, TEST_AUDIO_PATTERN);
                DramcEngine2SetPat(p,TEST_XTALK_PATTERN, 0,1);
                u4err_value |= DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, TEST_XTALK_PATTERN);

                if(u1VrefScanEnable==0 && (calType != TX_DQ_DQS_MOVE_DQM_ONLY))
                {
                    //mcSHOW_DBG_MSG(("Delay=%3d |%2d %2d %3d| %2d %2d| 0x%8x [0]",uiDelay, ucdq_ui_large,ucdq_ui_small, ucdq_pi, ucdq_oen_ui_large,ucdq_oen_ui_small, u4err_value));
                    #ifdef ETT_PRINT_FORMAT
                    if(u4err_value !=0)
                    {
                        mcSHOW_DBG_MSG2(("%d |%d %d %d|[0]", uiDelay, ucdq_ui_large, ucdq_ui_small, ucdq_pi));
                    }
                    #else
                    mcSHOW_DBG_MSG2(("Delay=%3d |%2d %2d %3d| 0x%8x [0]", uiDelay, ucdq_ui_large, ucdq_ui_small, ucdq_pi, u4err_value));
                    #endif
                    mcFPRINTF((fp_A60501, "Delay=%3d | %2d %2d %3d| 0x%8x [0]", uiDelay, ucdq_ui_large,ucdq_ui_small, ucdq_pi, u4err_value));
                }

                // check fail bit ,0 ok ,others fail
                for (u1BitIdx = 0; u1BitIdx < p->data_width; u1BitIdx++)
                {
                    u4fail_bit = u4err_value&((U32)1<<u1BitIdx);

                    if(u1VrefScanEnable==0 && (calType != TX_DQ_DQS_MOVE_DQM_ONLY))
                    {
                        if(u4err_value != 0)
                        {
                            if(u1BitIdx%DQS_BIT_NUMBER ==0)
                            {
                                mcSHOW_DBG_MSG2((" "));
                                mcFPRINTF((fp_A60501, " "));
                            }

                            if (u4fail_bit == 0)
                            {
                                mcSHOW_DBG_MSG2(("o"));
                                mcFPRINTF((fp_A60501, "o"));
                            }
                            else
                            {
                                mcSHOW_DBG_MSG2(("x"));
                                mcFPRINTF((fp_A60501, "x"));
                            }
                        }
                    }

                    if(u1IsDDR800PhaseMode(p) == TRUE)
                    {
                        u4fail_bit = u4err_value&((U32)0xFF<<((u1BitIdx>>3)<<3));
                    }

                    if(WinPerBit[u1BitIdx].first_pass== PASS_RANGE_NA)
                    {
                        if(u4fail_bit==0) //compare correct: pass
                        {
                            WinPerBit[u1BitIdx].first_pass = uiDelay;
                            
                            #if TX_TDQS2DQ_PRE_CAL
                            if ((u1IsLP4Div4DDR800(p) == FALSE) && (calType ==TX_DQ_DQS_MOVE_DQ_ONLY) && (u1VrefScanEnable==FALSE))
                            {
                                if(u2DQS2DQ_Pre_Cal[p->channel][p->rank][vGet_Div_Mode(p)]==0)
                                {
                                    u2DQS2DQ_Pre_Cal[p->channel][p->rank][vGet_Div_Mode(p)] = ((uiDelay - u2DQDelayBegin)* 1000) / p->frequency;
                                }
                                
                                if(uiDelay==u2DQDelayBegin)
                                {
                                    mcSHOW_ERR_MSG(("TX_TDQS2DQ_PRE_CAL: Warning, possible miss TX window boundary\n"));
                                    #if __ETT__
                                    while(1);
                                    #endif
                                }
                            }
                            #endif
                        }
                    }
                    else if(WinPerBit[u1BitIdx].last_pass == PASS_RANGE_NA)
                    {
                        if(u4fail_bit !=0) //compare error : fail
                        {
                            WinPerBit[u1BitIdx].last_pass  = uiDelay-u2DQDelayStep;
                        }
                        else if (uiDelay > (u2DQDelayEnd - u2DQDelayStep))
                        {
                            WinPerBit[u1BitIdx].last_pass = uiDelay;
                        }

                        if(WinPerBit[u1BitIdx].last_pass  !=PASS_RANGE_NA)
                        {
                            if((WinPerBit[u1BitIdx].last_pass -WinPerBit[u1BitIdx].first_pass) >= (VrefWinPerBit[u1BitIdx].last_pass -VrefWinPerBit[u1BitIdx].first_pass))
                            {
                                if((VrefWinPerBit[u1BitIdx].last_pass != PASS_RANGE_NA) && (VrefWinPerBit[u1BitIdx].last_pass -VrefWinPerBit[u1BitIdx].first_pass)>0)
                                {
                                    mcSHOW_DBG_MSG2(("Bit[%d] Bigger window update %d > %d, window broken?\n", u1BitIdx, \
                                        (WinPerBit[u1BitIdx].last_pass -WinPerBit[u1BitIdx].first_pass), (VrefWinPerBit[u1BitIdx].last_pass -VrefWinPerBit[u1BitIdx].first_pass)));
                                    mcFPRINTF((fp_A60501,"Bit[%d] Bigger window update %d > %d\n", u1BitIdx, \
                                        (WinPerBit[u1BitIdx].last_pass -WinPerBit[u1BitIdx].first_pass), (VrefWinPerBit[u1BitIdx].last_pass -VrefWinPerBit[u1BitIdx].first_pass)));

                                }
                                
                                //if window size bigger than 7, consider as real pass window. If not, don't update finish counte and won't do early break;
                                if((WinPerBit[u1BitIdx].last_pass -WinPerBit[u1BitIdx].first_pass) >7)
                                    uiFinishCount |= (1<<u1BitIdx);

                                //update bigger window size
                                VrefWinPerBit[u1BitIdx].first_pass = WinPerBit[u1BitIdx].first_pass;
                                VrefWinPerBit[u1BitIdx].last_pass = WinPerBit[u1BitIdx].last_pass;
                            }

                            //reset tmp window
                            WinPerBit[u1BitIdx].first_pass = PASS_RANGE_NA;
                            WinPerBit[u1BitIdx].last_pass = PASS_RANGE_NA;
                        }
                     }
                }

                if(u1VrefScanEnable==0 && (calType != TX_DQ_DQS_MOVE_DQM_ONLY))
                {
                    if(u4err_value != 0)
                    {
                        mcSHOW_DBG_MSG2((" [MSB]\n"));
                        mcFPRINTF((fp_A60501, " [MSB]\n"));
                    }
                }

                //if all bits widnow found and all bits turns to fail again, early break;
                if(uiFinishCount == 0xffff)
                {
                    vSetCalibrationResult(p, DRAM_CALIBRATION_TX_PERBIT, DRAM_OK);

                    #if !REDUCE_LOG_FOR_PRELOADER
                    #ifdef ETT_PRINT_FORMAT
                    mcSHOW_DBG_MSG2(("TX calibration finding left boundary early break. PI DQ delay=0x%B\n", uiDelay));
                    #else
                    mcSHOW_DBG_MSG2(("TX calibration finding left boundary early break. PI DQ delay=0x%2x\n", uiDelay));
                    #endif
                    #endif
                    break;  //early break
                }
            }


            // (1) calculate per bit window size 
            // (2) find out min win of all DQ bits
            // (3) calculate perbit window center
            u1min_winsize = 0xff;
            u1min_bit = 0xff;
            for (u1BitIdx = 0; u1BitIdx < p->data_width; u1BitIdx++)
            {
                //if(VrefWinPerBit[u1BitIdx].last_pass == VrefWinPerBit[u1BitIdx].first_pass)
                if(VrefWinPerBit[u1BitIdx].first_pass == PASS_RANGE_NA)
                    VrefWinPerBit[u1BitIdx].win_size = 0;
                else 
                    VrefWinPerBit[u1BitIdx].win_size= VrefWinPerBit[u1BitIdx].last_pass- VrefWinPerBit[u1BitIdx].first_pass + u2DQDelayStep;

                if (VrefWinPerBit[u1BitIdx].win_size < u1min_winsize)
                {
                    u1min_bit = u1BitIdx;
                    u1min_winsize = VrefWinPerBit[u1BitIdx].win_size;
                }

                u2TempWinSum += VrefWinPerBit[u1BitIdx].win_size;  //Sum of CA Windows for vref selection

                #if VENDER_JV_LOG
                if(calType == TX_DQ_DQS_MOVE_DQ_ONLY)
                {
                    mcSHOW_DBG_MSG5(("TX Bit%d, %d%%\n", u1BitIdx,  (VrefWinPerBit[u1BitIdx].win_size*100+31)/32));
                }
                #endif


                // calculate per bit window position and print
                VrefWinPerBit[u1BitIdx].win_center= (VrefWinPerBit[u1BitIdx].first_pass + VrefWinPerBit[u1BitIdx].last_pass) >> 1;
#if PINMUX_AUTO_TEST_PER_BIT_TX
                gFinalTXPerbitFirstPass[p->channel][u1BitIdx] = VrefWinPerBit[u1BitIdx].first_pass;
#endif
            }
            

            #if __ETT__
            if(u1VrefScanEnable==0)
            {
                //mcSHOW_DBG_MSG(("\n\tCH=%d, VrefRange= %d, VrefLevel = %d\n", p->channel, u2FinalRange, u2VrefLevel));
                //mcFPRINTF((fp_A60501,"\n\tchannel=%d(2:cha, 3:chb)  u2VrefLevel = %d\n", p->channel, u2VrefLevel));
                TxPrintWidnowInfo(p, VrefWinPerBit);
            }
            #endif
            
            if(u1VrefScanEnable==1)
            {     
                if(u2TempWinSum > u2MaxWindowSum)
                    u2MaxWindowSum= u2TempWinSum;
                
                VrefInfo[u1VrefIdx].u2VrefUsed = u2VrefLevel;
                VrefInfo[u1VrefIdx].u1WorseBitWinSize_byVref= u1min_winsize;
                VrefInfo[u1VrefIdx].u1WorseBitIdx_byVref = u1min_bit; 
                VrefInfo[u1VrefIdx].u2WinSum_byVref = u2TempWinSum;
                u1VrefIdx ++;
            }
            
            #if LP4_TX_VREF_PASS_CONDITION
            if(u1VrefScanEnable && (u2TempWinSum < (u2MaxWindowSum*95/100)) &&(u1min_winsize < LP4_TX_VREF_PASS_CONDITION))
            #else
            if(u1VrefScanEnable && (u2TempWinSum < (u2MaxWindowSum*95/100)))
            #endif
            { 
                mcSHOW_DBG_MSG(("\nTX Vref early break, caculate TX vref\n"));
                break;
            }
        }

        DramcEngine2End(p);

        if(u1VrefScanEnable==0)// ..if time domain (not vref scan) , calculate window center of all bits.
        {
            // Calculate the center of DQ pass window
            // Record center sum of each byte
            for (u1ByteIdx=0; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
            {
                #if 1//TX_DQM_CALC_MAX_MIN_CENTER
                u2Center_min[u1ByteIdx] = 0xffff;
                u2Center_max[u1ByteIdx] = 0;
                #endif

                for (u1BitIdx=0; u1BitIdx<DQS_BIT_NUMBER; u1BitIdx++)
                {
                    u1BitTemp = u1ByteIdx * DQS_BIT_NUMBER + u1BitIdx;
                    memcpy(FinalWinPerBit, VrefWinPerBit, sizeof(PASS_WIN_DATA_T)*DQ_DATA_WIDTH);                     

                    if(FinalWinPerBit[u1BitTemp].win_center < u2Center_min[u1ByteIdx])
                        u2Center_min[u1ByteIdx] = FinalWinPerBit[u1BitTemp].win_center;

                    if(FinalWinPerBit[u1BitTemp].win_center > u2Center_max[u1ByteIdx])
                        u2Center_max[u1ByteIdx] = FinalWinPerBit[u1BitTemp].win_center;

                    #ifdef FOR_HQA_TEST_USED
                    if(((calType == TX_DQ_DQS_MOVE_DQ_ONLY) && (u1VrefScanEnable==0)) || (u1IsDDR800PhaseMode(p)==TRUE))
                    {
                        gFinalTXPerbitWin[p->channel][p->rank][u1BitTemp] = FinalWinPerBit[u1BitTemp].win_size;
                        if ((u1IsDDR800PhaseMode(p)==TRUE) && (p->support_rank_num==RANK_DUAL)) // @Darren, DDR800semi calib rank0 only for HQA report
                            gFinalTXPerbitWin[p->channel][RANK_1][u1BitTemp] = FinalWinPerBit[u1BitTemp].win_size;
                    }
                    #endif
                }
            }
            
            #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
            if(p->femmc_Ready==0)//save firtst run pass value
            {
                for (u1ByteIdx=0; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
                {
                    if((calType == TX_DQ_DQS_MOVE_DQ_ONLY) || (u1IsDDR800PhaseMode(p)==TRUE)) // && u1VrefScanEnable==0
                    {
                        p->pSavetimeData->u1TxCenter_min_Save[p->channel][p->rank][u1ByteIdx]=u2Center_min[u1ByteIdx];
                        p->pSavetimeData->u1TxCenter_max_Save[p->channel][p->rank][u1ByteIdx]=u2Center_max[u1ByteIdx];
                        
                        for (u1BitIdx=0; u1BitIdx<DQS_BIT_NUMBER; u1BitIdx++)
                        {
                            u1BitTemp = u1ByteIdx*DQS_BIT_NUMBER+u1BitIdx;
                            p->pSavetimeData->u1Txwin_center_Save[p->channel][p->rank][u1BitTemp]=FinalWinPerBit[u1BitTemp].win_center;
                        }
                    }
                }
            }
            #endif
        }
    }

    // SET tx Vref (DQ) = u2FinalVref, LP3 no need to set this.
    if(u1VrefScanEnable)
    {
        #if SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_VREF_CAL
        if(p->femmc_Ready==1 && (p->Bypass_TXWINDOW))
        {
            u2FinalVref = p->pSavetimeData->u1TxWindowPerbitVref_Save[p->channel][p->rank];                     
        }  
        else
        #endif
        {
            u2FinalVref = TxChooseVref(p, VrefInfo, u1VrefIdx);
            #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
            if(p->femmc_Ready==0)////save firtst run Vref value
            {
                p->pSavetimeData->u1TxWindowPerbitVref_Save[p->channel][p->rank]=u2FinalVref;
            }
            #endif
        }
        
        TXSetFinalVref(p, u2FinalRange, u2FinalVref);      
        return DRAM_OK;
    }
    else
    {  
        #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(p->femmc_Ready==0)////save firtst run Vref value
        {
            p->pSavetimeData->u1TxWindowPerbitVref_Save[p->channel][p->rank]=u1MR14Value[p->channel][p->rank][p->dram_fsp] & 0x3f;
        }
        #endif
    }
    

#ifdef FOR_HQA_TEST_USED
    // LP4 DQ time domain || LP3 DQ_DQM time domain
    if((calType == TX_DQ_DQS_MOVE_DQ_ONLY) || (u1IsDDR800PhaseMode(p)==TRUE))
    {
        gFinalTXPerbitWin_min_max[p->channel][p->rank] = u1min_winsize;
    }
#endif

    // LP3 only use "TX_DQ_DQS_MOVE_DQ_DQM" scan
    // first freq 800(LP4-1600) doesn't support jitter meter(data < 1T), therefore, don't use delay cell
    if((calType == TX_DQ_DQS_MOVE_DQ_ONLY) && (p->frequency >= 1333) && (p->u2DelayCellTimex100!=0))
    {
        u1EnableDelayCell =1;
        mcSHOW_DBG_MSG(("[TX_PER_BIT_DELAY_CELL] DelayCellTimex100 =%d/100 ps\n", p->u2DelayCellTimex100));
    }

    //Calculate the center of DQ pass window
    //average the center delay
    for (u1ByteIdx=0; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
    {
        mcSHOW_DBG_MSG((" == TX Byte %d ==\n", u1ByteIdx));
        u2DQM_Delay = ((u2Center_min[u1ByteIdx] + u2Center_max[u1ByteIdx])>>1); //(max +min)/2
        
        if(u1EnableDelayCell==0)  
        {   
            uiDelay = u2DQM_Delay;
        }
        else// if(calType == TX_DQ_DQS_MOVE_DQ_ONLY)
        {
            uiDelay = u2Center_min[u1ByteIdx];  // for DQ PI delay , will adjust with delay cell
            
            // calculate delay cell perbit
            for (u1BitIdx = 0; u1BitIdx < DQS_BIT_NUMBER; u1BitIdx++)
            {
                u1BitTemp = u1ByteIdx*DQS_BIT_NUMBER+u1BitIdx;
                u1PIDiff = FinalWinPerBit[u1BitTemp].win_center - u2Center_min[u1ByteIdx];
                if(p->u2DelayCellTimex100 !=0)
                {
                    u1DelayCellOfst[u1BitTemp] = (u1PIDiff*100000000/(p->frequency<<6))/p->u2DelayCellTimex100;                    
                    mcSHOW_DBG_MSG(("u1DelayCellOfst[%d]=%d cells (%d PI)\n",  u1BitTemp, u1DelayCellOfst[u1BitTemp], u1PIDiff));
                }
                else
                {
                    mcSHOW_ERR_MSG(("Error: Cell time (p->u2DelayCellTimex100) is 0 \n"));
                    break;
                }
            }
            
        }   

        TxWinTransferDelayToUIPI(p, calType, uiDelay, 1, &ucdq_reg_ui_large[u1ByteIdx], &ucdq_reg_ui_small[u1ByteIdx], &ucdq_reg_pi[u1ByteIdx],\
                                &ucdq_reg_oen_ui_large[u1ByteIdx], &ucdq_reg_oen_ui_small[u1ByteIdx]);

        TxWinTransferDelayToUIPI(p, calType, u2DQM_Delay, 1, &ucdq_reg_dqm_ui_large[u1ByteIdx], &ucdq_reg_dqm_ui_small[u1ByteIdx], &ucdq_reg_dqm_pi[u1ByteIdx],\
                                &ucdq_reg_dqm_oen_ui_large[u1ByteIdx], &ucdq_reg_dqm_oen_ui_small[u1ByteIdx]);

        if(calType ==TX_DQ_DQS_MOVE_DQ_ONLY || calType== TX_DQ_DQS_MOVE_DQ_DQM)
        {
            mcSHOW_DBG_MSG(("Update DQ  dly =%d (%d ,%d, %d)  DQ  OEN =(%d ,%d)\n", 
                            uiDelay, ucdq_reg_ui_large[u1ByteIdx], ucdq_reg_ui_small[u1ByteIdx], ucdq_reg_pi[u1ByteIdx],\
                            ucdq_reg_oen_ui_large[u1ByteIdx], ucdq_reg_oen_ui_small[u1ByteIdx]));

            mcFPRINTF((fp_A60501,"Byte%d, PI DQ dly %d\n",  u1ByteIdx, uiDelay));
            mcFPRINTF((fp_A60501,"Final DQ PI dly(LargeUI, SmallUI, PI) =(%d ,%d, %d)\n", ucdq_reg_ui_large[u1ByteIdx], ucdq_reg_ui_small[u1ByteIdx], ucdq_reg_pi[u1ByteIdx]));
            mcFPRINTF((fp_A60501,"OEN DQ PI dly(LargeUI, SmallUI, PI) =(%d ,%d, %d)\n\n", ucdq_reg_oen_ui_large[u1ByteIdx], ucdq_reg_oen_ui_small[u1ByteIdx], ucdq_reg_pi[u1ByteIdx]));
        }
        
        //if(calType ==TX_DQ_DQS_MOVE_DQM_ONLY || calType== TX_DQ_DQS_MOVE_DQ_DQM)
        {
            mcSHOW_DBG_MSG(("Update DQM dly =%d (%d ,%d, %d)  DQM OEN =(%d ,%d)",  
                    u2DQM_Delay, ucdq_reg_dqm_ui_large[u1ByteIdx], ucdq_reg_dqm_ui_small[u1ByteIdx], ucdq_reg_dqm_pi[u1ByteIdx], \
                    ucdq_reg_dqm_oen_ui_large[u1ByteIdx], ucdq_reg_dqm_oen_ui_small[u1ByteIdx]));       
        }
        mcSHOW_DBG_MSG(("\n", u1ByteIdx));
        
#ifdef FOR_HQA_REPORT_USED
        if(calType == TX_DQ_DQS_MOVE_DQ_ONLY || (u1IsDDR800PhaseMode(p)==TRUE))
        {
            for (u1BitIdx = 0; u1BitIdx < p->data_width; u1BitIdx++)
            {
                HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT1, "TX_Window_Center_", "DQ", u1BitIdx, FinalWinPerBit[u1BitIdx].win_center, NULL);
            }
        }

        if(calType == TX_DQ_DQS_MOVE_DQM_ONLY ||(u1IsDDR800PhaseMode(p)==TRUE))
        {
            HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT0, "TX_Window_Center_", "DQM", u1ByteIdx, u2DQM_Delay, NULL);
        }
#if 0
        HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT1, "TX_Window_Center_", "LargeUI", u1ByteIdx, ucdq_reg_ui_large[u1ByteIdx], NULL);
        HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT0, "TX_Window_Center_", "SmallUI", u1ByteIdx, ucdq_reg_ui_small[u1ByteIdx], NULL);
        HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT0, "TX_Window_Center_", "PI", u1ByteIdx, ucdq_reg_pi[u1ByteIdx], NULL);            
#endif        
#endif

    }


#if REG_ACCESS_PORTING_DGB
    RegLogEnable =1;
#endif

        /* p->rank = RANK_0, save to Reg Rank0 and Rank1, p->rank = RANK_1, save to Reg Rank1 */
        for(u1RankIdx=p->rank; u1RankIdx<RANK_MAX; u1RankIdx++)
        {
            vSetRank(p,u1RankIdx);

            if(calType ==TX_DQ_DQS_MOVE_DQ_ONLY || calType== TX_DQ_DQS_MOVE_DQ_DQM)
            {
                TXSetDelayReg_DQ(p, TRUE, ucdq_reg_ui_large, ucdq_reg_oen_ui_large, ucdq_reg_ui_small, ucdq_reg_oen_ui_small, ucdq_reg_pi);
            }

            TXSetDelayReg_DQM(p, TRUE, ucdq_reg_dqm_ui_large, ucdq_reg_dqm_oen_ui_large, ucdq_reg_dqm_ui_small, ucdq_reg_dqm_oen_ui_small, ucdq_reg_dqm_pi);

             
             if(u1EnableDelayCell)
             {
                 vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ0),
                            P_Fld(u1DelayCellOfst[7] , SHU_R0_B0_DQ0_RK0_TX_ARDQ7_DLY_B0)
                         | P_Fld(u1DelayCellOfst[6] , SHU_R0_B0_DQ0_RK0_TX_ARDQ6_DLY_B0)
                         | P_Fld(u1DelayCellOfst[5] , SHU_R0_B0_DQ0_RK0_TX_ARDQ5_DLY_B0)
                         | P_Fld(u1DelayCellOfst[4] , SHU_R0_B0_DQ0_RK0_TX_ARDQ4_DLY_B0)
                         | P_Fld(u1DelayCellOfst[3] , SHU_R0_B0_DQ0_RK0_TX_ARDQ3_DLY_B0)
                         | P_Fld(u1DelayCellOfst[2] , SHU_R0_B0_DQ0_RK0_TX_ARDQ2_DLY_B0)
                         | P_Fld(u1DelayCellOfst[1] , SHU_R0_B0_DQ0_RK0_TX_ARDQ1_DLY_B0)
                         | P_Fld(u1DelayCellOfst[0] , SHU_R0_B0_DQ0_RK0_TX_ARDQ0_DLY_B0));
                 vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ0),
                            P_Fld(u1DelayCellOfst[15] , SHU_R0_B1_DQ0_RK0_TX_ARDQ7_DLY_B1)
                         | P_Fld(u1DelayCellOfst[14] , SHU_R0_B1_DQ0_RK0_TX_ARDQ6_DLY_B1)
                         | P_Fld(u1DelayCellOfst[13] , SHU_R0_B1_DQ0_RK0_TX_ARDQ5_DLY_B1)
                         | P_Fld(u1DelayCellOfst[12] , SHU_R0_B1_DQ0_RK0_TX_ARDQ4_DLY_B1)
                         | P_Fld(u1DelayCellOfst[11] , SHU_R0_B1_DQ0_RK0_TX_ARDQ3_DLY_B1)
                         | P_Fld(u1DelayCellOfst[10] , SHU_R0_B1_DQ0_RK0_TX_ARDQ2_DLY_B1)
                         | P_Fld(u1DelayCellOfst[9] , SHU_R0_B1_DQ0_RK0_TX_ARDQ1_DLY_B1)
                         | P_Fld(u1DelayCellOfst[8] , SHU_R0_B1_DQ0_RK0_TX_ARDQ0_DLY_B1));
             }
           
            #if ENABLE_TX_TRACKING
            TXUpdateTXTracking(p, calType, ucdq_reg_pi, ucdq_reg_dqm_pi);
            #endif
        }
        
        vSetRank(p, backup_rank);

#if REG_ACCESS_PORTING_DGB
    RegLogEnable =0;
#endif

    mcSHOW_DBG_MSG3(("[TxWindowPerbitCal] Done\n\n"));
    mcFPRINTF((fp_A60501, "[TxWindowPerbitCal] Done\n\n"));

    #if 0
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_PADCTL4), 1, PADCTL4_CKEFIXON);  // test only
    #endif

    return DRAM_OK;
}

#endif //SIMULATION_TX_PERBIT

#if ENABLE_EYESCAN_GRAPH
void Dramc_K_TX_EyeScan_Log(DRAMC_CTX_T *p)
{
    U8 ucindex, u1BitIdx, u1ByteIdx;
    U8 ii, backup_rank, u1PrintWinData, u1vrefidx;
    PASS_WIN_DATA_T WinPerBit[DQ_DATA_WIDTH], VrefWinPerBit[DQ_DATA_WIDTH], FinalWinPerBit[DQ_DATA_WIDTH];
    U16 tx_pi_delay[4], tx_dqm_pi_delay[4];
    U16 u2DQDelayBegin, uiDelay;
    U16 u2VrefLevel, u2VrefBegin, u2VrefEnd, u2VrefStep, u2VrefRange;
    U8 ucdq_pi, ucdq_ui_small, ucdq_ui_large,ucdq_oen_ui_small, ucdq_oen_ui_large;
    U32 uiFinishCount;
    U16 u2TempWinSum, u2tx_window_sum=0;
    U32 u4err_value, u4fail_bit;
    #if 1//TX_DQM_CALC_MAX_MIN_CENTER
    U16 u2Center_min[DQS_NUMBER] = {0}, u2Center_max[DQS_NUMBER] = {0};
    #endif

    U16 TXPerbitWin_min_max = 0;
    U32 min_bit, min_winsize;

    U16 u2FinalVref=0xd;
    U16 u2FinalRange=0;

    U8 EyeScan_index[DQ_DATA_WIDTH];

    U8 backup_u1MR14Value;
    U8 u1pass_in_this_vref_flag[DQ_DATA_WIDTH];

    U32 u4RegBackupAddress[] =
    {
        (DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0)),
        (DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ2)),
        (DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ1)),
        (DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ3)),
        (DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ7)),
        (DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ7)),
    };

    if(gTX_EYE_Scan_flag==0) return;

    if (gTX_EYE_Scan_only_higheset_freq_flag==1 && p->frequency != u2DFSGetHighestFreq(p)) return;

    //backup register value
    DramcBackupRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));

    backup_u1MR14Value = u1MR14Value[p->channel][p->rank][p->dram_fsp];

    //set initial values
    for(u1vrefidx=0; u1vrefidx<VREF_TOTAL_NUM_WITH_RANGE;u1vrefidx++)
    {
        for (u1BitIdx = 0; u1BitIdx < p->data_width; u1BitIdx++)
        {
            for(ii=0; ii<EYESCAN_BROKEN_NUM; ii++)
            {
                gEyeScan_Min[u1vrefidx][u1BitIdx][ii] = EYESCAN_DATA_INVALID;
                gEyeScan_Max[u1vrefidx][u1BitIdx][ii] = EYESCAN_DATA_INVALID;
            }
            gEyeScan_ContinueVrefHeight[u1BitIdx] = 0;
            gEyeScan_TotalPassCount[u1BitIdx] = 0;
        }
    }

    for(u1ByteIdx=0; u1ByteIdx < p->data_width/DQS_BIT_NUMBER; u1ByteIdx++)
    {
        if (u1ByteIdx == 0)
        {
            tx_pi_delay[u1ByteIdx] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0), SHURK0_SELPH_DQ0_TXDLY_DQ0) * 256 +
                          u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ2), SHURK0_SELPH_DQ2_DLY_DQ0) * 32 +
                          u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ7), SHU_R0_B0_DQ7_RK0_ARPI_DQ_B0);

            tx_dqm_pi_delay[u1ByteIdx] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ1), SHURK0_SELPH_DQ1_TXDLY_DQM0) * 256 +
                              u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ3), SHURK0_SELPH_DQ3_DLY_DQM0) * 32 +
                              u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ7), SHU_R0_B0_DQ7_RK0_ARPI_DQM_B0);        
        }
        else
        {
            tx_pi_delay[u1ByteIdx] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0), SHURK0_SELPH_DQ0_TXDLY_DQ1) * 256 +
                          u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ2), SHURK0_SELPH_DQ2_DLY_DQ1) * 32 +
                          u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ7), SHU_R0_B1_DQ7_RK0_ARPI_DQ_B1);    

            tx_dqm_pi_delay[u1ByteIdx] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ1), SHURK0_SELPH_DQ1_TXDLY_DQM1) * 256 +
                              u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ3), SHURK0_SELPH_DQ3_DLY_DQM1) * 32 +
                              u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ7), SHU_R0_B1_DQ7_RK0_ARPI_DQM_B1);                
        }
    }

    if (tx_pi_delay[0] < tx_pi_delay[1])
    {
        u2DQDelayBegin = tx_pi_delay[0]-32;
    }
    else
    {
        u2DQDelayBegin = tx_pi_delay[1]-32;
    }

    u2VrefRange = 0;
    u2VrefBegin = 0;
    u2VrefEnd = 50;
    u2VrefStep = EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV;

    DramcEngine2Init(p, p->test2_1, p->test2_2, p->test_pattern, 0);

    for(u2VrefLevel = u2VrefBegin; u2VrefLevel <= u2VrefEnd; u2VrefLevel += u2VrefStep)
    {
        //set vref
//fra        u1MR14Value[p->channel][p->rank][p->dram_fsp] = (u2VrefLevel | (u2VrefRange<<6));
        DramcModeRegWriteByRank(p, p->rank, 14, u2VrefLevel | (u2VrefRange<<6));

        // initialize parameters
        uiFinishCount = 0;
        u2TempWinSum =0;

        for (u1BitIdx = 0; u1BitIdx < p->data_width; u1BitIdx++)
        {
            WinPerBit[u1BitIdx].first_pass = (S16)PASS_RANGE_NA;
            WinPerBit[u1BitIdx].last_pass = (S16)PASS_RANGE_NA;
            VrefWinPerBit[u1BitIdx].first_pass = (S16)PASS_RANGE_NA;
            VrefWinPerBit[u1BitIdx].last_pass = (S16)PASS_RANGE_NA;

            gEyeScan_DelayCellPI[u1BitIdx] = 0;

            EyeScan_index[u1BitIdx] = 0;
            u1pass_in_this_vref_flag[u1BitIdx] = 0;            
        }

        for (uiDelay=0; uiDelay<64; uiDelay++)
        {
            TxWinTransferDelayToUIPI(p, TX_DQ_DQS_MOVE_DQ_DQM, tx_pi_delay[0]+uiDelay-32, 0, &ucdq_ui_large, &ucdq_ui_small, &ucdq_pi, &ucdq_oen_ui_large, &ucdq_oen_ui_small);
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0), \
                                             P_Fld(ucdq_ui_large, SHURK0_SELPH_DQ0_TXDLY_DQ0) | \
                                             P_Fld(ucdq_oen_ui_large, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ2), \
                                             P_Fld(ucdq_ui_small, SHURK0_SELPH_DQ2_DLY_DQ0) | \
                                             P_Fld(ucdq_oen_ui_small, SHURK0_SELPH_DQ2_DLY_OEN_DQ0));
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ7), ucdq_pi, SHU_R0_B0_DQ7_RK0_ARPI_DQ_B0);

            TxWinTransferDelayToUIPI(p, TX_DQ_DQS_MOVE_DQ_DQM, tx_pi_delay[1]+uiDelay-32, 0, &ucdq_ui_large, &ucdq_ui_small, &ucdq_pi, &ucdq_oen_ui_large, &ucdq_oen_ui_small);
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0), \
                                             P_Fld(ucdq_ui_large, SHURK0_SELPH_DQ0_TXDLY_DQ1) | \
                                             P_Fld(ucdq_oen_ui_large, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ2), \
                                             P_Fld(ucdq_ui_small, SHURK0_SELPH_DQ2_DLY_DQ1) | \
                                             P_Fld(ucdq_oen_ui_small, SHURK0_SELPH_DQ2_DLY_OEN_DQ1));
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ7), ucdq_pi, SHU_R0_B1_DQ7_RK0_ARPI_DQ_B1);

            TxWinTransferDelayToUIPI(p, TX_DQ_DQS_MOVE_DQ_DQM, tx_dqm_pi_delay[0]+uiDelay-32, 0, &ucdq_ui_large, &ucdq_ui_small, &ucdq_pi, &ucdq_oen_ui_large, &ucdq_oen_ui_small);
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ1), \
                                            P_Fld(ucdq_ui_large, SHURK0_SELPH_DQ1_TXDLY_DQM0) | \
                                            P_Fld(ucdq_oen_ui_large, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0));
           vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ3), \
                                            P_Fld(ucdq_ui_small, SHURK0_SELPH_DQ3_DLY_DQM0) | \
                                            P_Fld(ucdq_oen_ui_small, SHURK0_SELPH_DQ3_DLY_OEN_DQM0));
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ7), ucdq_pi, SHU_R0_B0_DQ7_RK0_ARPI_DQM_B0);

            TxWinTransferDelayToUIPI(p, TX_DQ_DQS_MOVE_DQ_DQM, tx_dqm_pi_delay[1]+uiDelay-32, 0, &ucdq_ui_large, &ucdq_ui_small, &ucdq_pi, &ucdq_oen_ui_large, &ucdq_oen_ui_small);
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ1), \
                                             P_Fld(ucdq_ui_large, SHURK0_SELPH_DQ1_TXDLY_DQM1) | \
                                             P_Fld(ucdq_oen_ui_large, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ3), \
                                             P_Fld(ucdq_ui_small, SHURK0_SELPH_DQ3_DLY_DQM1) | \
                                             P_Fld(ucdq_oen_ui_small, SHURK0_SELPH_DQ3_DLY_OEN_DQM1));
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ7), ucdq_pi, SHU_R0_B1_DQ7_RK0_ARPI_DQM_B1);           


             // audio +xtalk pattern
            u4err_value=0;
            DramcEngine2SetPat(p,TEST_AUDIO_PATTERN, 0,0);
            u4err_value = DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, TEST_AUDIO_PATTERN);
            DramcEngine2SetPat(p,TEST_XTALK_PATTERN, 0,1);
            u4err_value |= DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, TEST_XTALK_PATTERN);

            // check fail bit ,0 ok ,others fail
            for (u1BitIdx = 0; u1BitIdx < p->data_width; u1BitIdx++)
            {
                u4fail_bit = u4err_value&((U32)1<<u1BitIdx);

                if (u4fail_bit == 0)
                {
                    gEyeScan_TotalPassCount[u1BitIdx]+=EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV;
                }

                if(WinPerBit[u1BitIdx].first_pass== PASS_RANGE_NA)
                {
                    if(u4fail_bit==0) //compare correct: pass
                    {
                        WinPerBit[u1BitIdx].first_pass = uiDelay;
                        u1pass_in_this_vref_flag[u1BitIdx] = 1;
                    }
                }
                else if(WinPerBit[u1BitIdx].last_pass == PASS_RANGE_NA)
                {
                    if(u4fail_bit !=0) //compare error : fail
                    {
                        WinPerBit[u1BitIdx].last_pass  = (uiDelay-1);
                    }
                    else if (uiDelay>=63)
                    {
                        WinPerBit[u1BitIdx].last_pass  = 63;
                    }

                    if(WinPerBit[u1BitIdx].last_pass  !=PASS_RANGE_NA)
                    {
                        if((WinPerBit[u1BitIdx].last_pass -WinPerBit[u1BitIdx].first_pass) >= (VrefWinPerBit[u1BitIdx].last_pass -VrefWinPerBit[u1BitIdx].first_pass))
                        {
                            //if window size bigger than 7, consider as real pass window. If not, don't update finish counte and won't do early break;
                            if((WinPerBit[u1BitIdx].last_pass -WinPerBit[u1BitIdx].first_pass) >7)
                                uiFinishCount |= (1<<u1BitIdx);

                            //update bigger window size
                            VrefWinPerBit[u1BitIdx].first_pass = WinPerBit[u1BitIdx].first_pass;
                            VrefWinPerBit[u1BitIdx].last_pass = WinPerBit[u1BitIdx].last_pass;
                        }

#if ENABLE_EYESCAN_GRAPH
                            if (EyeScan_index[u1BitIdx] < EYESCAN_BROKEN_NUM)
                            {
#if VENDER_JV_LOG || defined(RELEASE)
                                gEyeScan_Min[(u2VrefLevel+u2VrefRange*30)/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx][EyeScan_index[u1BitIdx]] = WinPerBit[u1BitIdx].first_pass;
                                gEyeScan_Max[(u2VrefLevel+u2VrefRange*30)/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx][EyeScan_index[u1BitIdx]] = WinPerBit[u1BitIdx].last_pass;
#else
//fra                                gEyeScan_Min[(u2VrefLevel+u2VrefRange*30)/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx][EyeScan_index[u1BitIdx]] = WinPerBit[u1BitIdx].first_pass + tx_pi_delay[u1BitIdx/8]-32;
//fra                                gEyeScan_Max[(u2VrefLevel+u2VrefRange*30)/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx][EyeScan_index[u1BitIdx]] = WinPerBit[u1BitIdx].last_pass + tx_pi_delay[u1BitIdx/8]-32;
                                gEyeScan_Min[(u2VrefLevel+u2VrefRange*30)/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx][EyeScan_index[u1BitIdx]] = (S8) WinPerBit[u1BitIdx].first_pass;
                                gEyeScan_Max[(u2VrefLevel+u2VrefRange*30)/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx][EyeScan_index[u1BitIdx]] = (S8) WinPerBit[u1BitIdx].last_pass;
                                gEyeScan_MinMax_store_delay[u1BitIdx/8] =  tx_pi_delay[u1BitIdx/8]-32; /* save this information for HQA pass/fail judgement used */
#endif
                                EyeScan_index[u1BitIdx]=EyeScan_index[u1BitIdx]+1;
                            }
#endif

                        //reset tmp window
                        WinPerBit[u1BitIdx].first_pass = PASS_RANGE_NA;
                        WinPerBit[u1BitIdx].last_pass = PASS_RANGE_NA;
                    }
                 }
               }
        }

        min_winsize = 0xffff;
        min_bit = 0xff;
        for (u1BitIdx = 0; u1BitIdx < p->data_width; u1BitIdx++)
        {
            VrefWinPerBit[u1BitIdx].win_size = VrefWinPerBit[u1BitIdx].last_pass- VrefWinPerBit[u1BitIdx].first_pass +(VrefWinPerBit[u1BitIdx].last_pass==VrefWinPerBit[u1BitIdx].first_pass?0:1);

            if (VrefWinPerBit[u1BitIdx].win_size < min_winsize)
            {
                min_bit = u1BitIdx;
                min_winsize = VrefWinPerBit[u1BitIdx].win_size;
            }

            u2TempWinSum += VrefWinPerBit[u1BitIdx].win_size;  //Sum of CA Windows for vref selection

#if ENABLE_EYESCAN_GRAPH
            gEyeScan_WinSize[(u2VrefLevel+u2VrefRange*30)/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx] = VrefWinPerBit[u1BitIdx].win_size;
#endif

#ifdef FOR_HQA_TEST_USED
            if((((u1MR14Value[p->channel][p->rank][p->dram_fsp]>>6)&1) == u2VrefRange) && ((u1MR14Value[p->channel][p->rank][p->dram_fsp]&0x3f)==u2VrefLevel))
            {
                gFinalTXPerbitWin[p->channel][p->rank][u1BitIdx] = VrefWinPerBit[u1BitIdx].win_size;
            }
#endif

        }

        if ((min_winsize > TXPerbitWin_min_max) || ((min_winsize == TXPerbitWin_min_max) && (u2TempWinSum >u2tx_window_sum)))
        {
            TXPerbitWin_min_max = min_winsize;
            u2tx_window_sum =u2TempWinSum;
            u2FinalRange = u2VrefRange;
            u2FinalVref = u2VrefLevel;

            //Calculate the center of DQ pass window
            // Record center sum of each byte
            for (u1ByteIdx=0; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
            {
        #if 1//TX_DQM_CALC_MAX_MIN_CENTER
                u2Center_min[u1ByteIdx] = 0xffff;
                u2Center_max[u1ByteIdx] = 0;
        #endif

                for (u1BitIdx=0; u1BitIdx<DQS_BIT_NUMBER; u1BitIdx++)
                {
                    ucindex = u1ByteIdx * DQS_BIT_NUMBER + u1BitIdx;
                    FinalWinPerBit[ucindex].first_pass = VrefWinPerBit[ucindex].first_pass;
                    FinalWinPerBit[ucindex].last_pass =  VrefWinPerBit[ucindex].last_pass;
                    FinalWinPerBit[ucindex].win_size = VrefWinPerBit[ucindex].win_size;
                    FinalWinPerBit[ucindex].win_center = (FinalWinPerBit[ucindex].first_pass + FinalWinPerBit[ucindex].last_pass) >> 1;

                    if(FinalWinPerBit[ucindex].win_center < u2Center_min[u1ByteIdx])
                        u2Center_min[u1ByteIdx] = FinalWinPerBit[ucindex].win_center;

                    if(FinalWinPerBit[ucindex].win_center > u2Center_max[u1ByteIdx])
                        u2Center_max[u1ByteIdx] = FinalWinPerBit[ucindex].win_center;
                }
            }
        }


        if(u2VrefRange==0 && u2VrefLevel ==50)
        {
            u2VrefRange = 1;
            u2VrefLevel = 20;
        }

        for (u1BitIdx = 0; u1BitIdx < p->data_width; u1BitIdx++)
        {
            if (u1pass_in_this_vref_flag[u1BitIdx]) gEyeScan_ContinueVrefHeight[u1BitIdx]+=EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV;  //count pass number of continue vref
        }
    }

    DramcEngine2End(p);

    //Calculate the center of DQ pass window
    //average the center delay
    for (u1ByteIdx=0; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
    {
        uiDelay = ((u2Center_min[u1ByteIdx] + u2Center_max[u1ByteIdx])>>1); //(max +min)/2

#if ENABLE_EYESCAN_GRAPH
#if VENDER_JV_LOG || defined(RELEASE)
        gEyeScan_CaliDelay[u1ByteIdx] = uiDelay;
#else
        gEyeScan_CaliDelay[u1ByteIdx] = uiDelay + tx_pi_delay[u1ByteIdx]-32;
#endif
#endif
    }


    //restore to orignal value
    DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));

    DramcModeRegWriteByRank(p, p->rank, 14, backup_u1MR14Value);
    u1MR14Value[p->channel][p->rank][p->dram_fsp] = backup_u1MR14Value;

}
#endif











//-------------------------------------------------------------------------
/** DramcMiockJmeter
 *  start MIOCK jitter meter.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param block_no         (U8): block 0 or 1.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL
 */
//-------------------------------------------------------------------------

#ifdef ENABLE_MIOCK_JMETER
DRAM_STATUS_T DramcMiockJmeter(DRAMC_CTX_T *p)
{
    U8 u1ByteIdx;
    U32 u4RevBX[DQS_NUMBER];
    U8 ucsearch_state, ucdqs_dly, fgcurrent_value, fginitial_value, ucstart_period=0, ucmiddle_period=0, ucend_period=0;
    U32 u4sample_cnt, u4ones_cnt[DQS_NUMBER], u4MPDIV_IN_SEL;
    U16 u2real_freq, u2real_period;

    U8 u1ShuLevel;
    U32 u4PLL5_ADDR;
    U32 u4PLL8_ADDR;
    U32 u4CA_CMD6;
    U32 u4SDM_PCW;
    U32 u4PREDIV;
    U32 u4POSDIV;
    U32 u4CKDIV4;
    U32 u4VCOFreq;
    U32 u4DataRate;
    U8 u1RxGatingPI=0, u1RxGatingPI_start=0, u1RxGatingPI_end=63;

#if (fcFOR_CHIP_ID == fcLafite)    
     u1RxGatingPI = 0x0;
#endif

    u2gdelay_cell_ps=0;

    // error handling
    if (!p)
    {
        mcSHOW_ERR_MSG(("context NULL\n"));
        return DRAM_FAIL;
    }

    U32 u4RegBackupAddress[] =
    {
        (DRAMC_REG_ADDR(DRAMC_REG_EYESCAN)),
        (DRAMC_REG_ADDR(DRAMC_REG_STBCAL1)),
        (DRAMC_REG_ADDR(DDRPHY_B0_DQ6)),
        (DRAMC_REG_ADDR(DDRPHY_B1_DQ6)),
        (DRAMC_REG_ADDR(DDRPHY_B0_DQ5)),
        (DRAMC_REG_ADDR(DDRPHY_B1_DQ5)),
        (DRAMC_REG_ADDR(DDRPHY_B0_DQ3)),
        (DRAMC_REG_ADDR(DDRPHY_B1_DQ3)),
        (DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ7)),
        (DRAMC_REG_ADDR(DDRPHY_SHU_B1_DQ7)),        
        (DRAMC_REG_ADDR(DDRPHY_B0_DQ4)),
        (DRAMC_REG_ADDR(DDRPHY_B1_DQ4)),
        (DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1)),
        (DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQSIEN)),
        (DRAMC_REG_ADDR(DRAMC_REG_SHURK1_DQSIEN)),
        ((DDRPHY_CA_DLL_ARPI2)),
        ((DDRPHY_B0_DLL_ARPI2)),
        ((DDRPHY_B1_DLL_ARPI2)),        
        ((DDRPHY_CA_DLL_ARPI2)+SHIFT_TO_CHB_ADDR),
        ((DDRPHY_B0_DLL_ARPI2)+SHIFT_TO_CHB_ADDR),
        ((DDRPHY_B1_DLL_ARPI2)+SHIFT_TO_CHB_ADDR),                
    };

    //backup register value
    DramcBackupRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));

    //DLL off to fix middle transion from high to low or low to high at high vcore
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_CA_DLL_ARPI2), 0x0, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_DLL_ARPI2), 0x0, B0_DLL_ARPI2_RG_ARDLL_PHDET_EN_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_DLL_ARPI2), 0x0, B1_DLL_ARPI2_RG_ARDLL_PHDET_EN_B1);

    //MCK4X CG
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 0, MISC_CTRL1_R_DMDQSIENCG_EN);

    // Bypass DQS glitch-free mode
    // RG_RX_*RDQ_EYE_DLY_DQS_BYPASS_B**
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), 1, B0_DQ6_RG_RX_ARDQ_EYE_DLY_DQS_BYPASS_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), 1, B1_DQ6_RG_RX_ARDQ_EYE_DLY_DQS_BYPASS_B1);

    //Enable DQ eye scan
    //RG_??_RX_EYE_SCAN_EN
    //RG_??_RX_VREF_EN
    //RG_??_RX_SMT_EN
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 1, EYESCAN_RG_RX_EYE_SCAN_EN);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), P_Fld(0x1, EYESCAN_EYESCAN_DQS_SYNC_EN)    
                                        | P_Fld(0x1, EYESCAN_EYESCAN_NEW_DQ_SYNC_EN)    
                                        | P_Fld(0x1, EYESCAN_EYESCAN_DQ_SYNC_EN)); 
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ5), 1, B0_DQ5_RG_RX_ARDQ_EYE_EN_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ5), 1, B1_DQ5_RG_RX_ARDQ_EYE_EN_B1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ5), 1, B0_DQ5_RG_RX_ARDQ_VREF_EN_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ5), 1, B1_DQ5_RG_RX_ARDQ_VREF_EN_B1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ3), 1, B0_DQ3_RG_RX_ARDQ_SMT_EN_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ3), 1, B1_DQ3_RG_RX_ARDQ_SMT_EN_B1);

    //JM_SEL
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), 1, B0_DQ6_RG_RX_ARDQ_JM_SEL_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), 1, B1_DQ6_RG_RX_ARDQ_JM_SEL_B1);

    //Enable MIOCK jitter meter mode ( RG_RX_MIOCK_JIT_EN=1)
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 1, EYESCAN_RG_RX_MIOCK_JIT_EN);

    //Disable DQ eye scan (b'1), for counter clear
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 0, EYESCAN_RG_RX_EYE_SCAN_EN);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_STBCAL1), 0, STBCAL1_DQSERRCNT_DIS);
	
    for (u1RxGatingPI=u1RxGatingPI_start; u1RxGatingPI<u1RxGatingPI_end; u1RxGatingPI++)
    {
        mcSHOW_DBG_MSG(("\n[DramcMiockJmeter] u1RxGatingPI = %d\n", u1RxGatingPI));

        ucsearch_state = 0;

    //to see 1T(H,L) or 1T(L,H) from delaycell=0 to 127
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQSIEN), P_Fld(u1RxGatingPI, SHURK0_DQSIEN_R0DQS0IEN) | P_Fld(u1RxGatingPI, SHURK0_DQSIEN_R0DQS1IEN)
                | P_Fld(u1RxGatingPI, SHURK0_DQSIEN_R0DQS2IEN) | P_Fld(u1RxGatingPI, SHURK0_DQSIEN_R0DQS3IEN));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK1_DQSIEN), P_Fld(u1RxGatingPI, SHURK1_DQSIEN_R1DQS0IEN) | P_Fld(u1RxGatingPI, SHURK1_DQSIEN_R1DQS1IEN)
                | P_Fld(u1RxGatingPI, SHURK1_DQSIEN_R1DQS2IEN) | P_Fld(u1RxGatingPI, SHURK1_DQSIEN_R1DQS3IEN));

    for (ucdqs_dly=0; ucdqs_dly<128; ucdqs_dly++)
    {

        //Set DQS delay (RG_??_RX_DQS_EYE_DLY)
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ4), ucdqs_dly, B0_DQ4_RG_RX_ARDQS_EYE_R_DLY_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ4), ucdqs_dly, B0_DQ4_RG_RX_ARDQS_EYE_F_DLY_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ4), ucdqs_dly, B1_DQ4_RG_RX_ARDQS_EYE_R_DLY_B1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ4), ucdqs_dly, B1_DQ4_RG_RX_ARDQS_EYE_F_DLY_B1);
        DramPhyReset(p);

        //Reset eye scan counters (reg_sw_rst): 1 to 0
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 1, EYESCAN_REG_SW_RST);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 0, EYESCAN_REG_SW_RST);

        //Enable DQ eye scan (b'1)
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 1, EYESCAN_RG_RX_EYE_SCAN_EN);

        //2ns/sample, here we delay 1ms about 500 samples
        mcDELAY_US(10);

        //Disable DQ eye scan (b'1), for counter latch
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 0, EYESCAN_RG_RX_EYE_SCAN_EN);

        //Read the counter values from registers (toggle_cnt*, dqs_err_cnt*);
        u4sample_cnt = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TOGGLE_CNT), TOGGLE_CNT_TOGGLE_CNT);
        u4ones_cnt[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQS0_ERR_CNT), DQS0_ERR_CNT_DQS0_ERR_CNT);
        u4ones_cnt[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQS1_ERR_CNT), DQS1_ERR_CNT_DQS1_ERR_CNT);
        u4ones_cnt[2] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQS2_ERR_CNT), DQS2_ERR_CNT_DQS2_ERR_CNT);
        u4ones_cnt[3] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQS3_ERR_CNT), DQS3_ERR_CNT_DQS3_ERR_CNT);
        #ifdef ETT_PRINT_FORMAT
        mcSHOW_DBG_MSG(("%d : %d, %d, %d, %d, %d\n", ucdqs_dly, u4sample_cnt, u4ones_cnt[0],u4ones_cnt[1],u4ones_cnt[2],u4ones_cnt[3]));
        #else
        mcSHOW_DBG_MSG(("%3d : %8d, %8d, %8d, %8d, %8d\n", ucdqs_dly, u4sample_cnt, u4ones_cnt[0],u4ones_cnt[2],u4ones_cnt[3]));
        #endif

        /*
        //Disable DQ eye scan (RG_RX_EYE_SCAN_EN=0, RG_RX_*RDQ_VREF_EN_B*=0, RG_RX_*RDQ_EYE_VREF_EN_B*=0, RG_RX_*RDQ_SMT_EN_B*=0)
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_STBCAL_F), 0, STBCAL_F_RG_EX_EYE_SCAN_EN);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_EYE2), 0, EYE2_RG_RX_ARDQ_VREF_EN_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_EYEB1_2), 0, EYEB1_2_RG_RX_ARDQ_VREF_EN_B1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_EYE2), 0, EYE2_RG_RX_ARDQ_EYE_VREF_EN_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_EYEB1_2), 0, EYEB1_2_RG_RX_ARDQ_EYE_VREF_EN_B1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_TXDQ3), 0, TXDQ3_RG_RX_ARDQ_SMT_EN_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_RXDQ13),0, RXDQ13_RG_RX_ARDQ_SMT_EN_B1);
        */

        //change to boolean value
        if (u4ones_cnt[0] < (u4sample_cnt/2))
        {
            fgcurrent_value = 0;
        }
        else
        {
            fgcurrent_value = 1;
        }

        #if 1//more than 1T data
        {
            if (ucsearch_state==0)
            {
                //record initial value at the beginning
                fginitial_value = fgcurrent_value;
                ucsearch_state = 1;
            }
            else if (ucsearch_state==1)
            {
                // check if change value
                if (fgcurrent_value != fginitial_value)
                {
                    // start of the period
                    fginitial_value = fgcurrent_value;
                    ucstart_period = ucdqs_dly;
                    ucsearch_state = 2;
                }
            }
            else if (ucsearch_state==2)
            {
                // check if change value
                if (fgcurrent_value != fginitial_value)
                {
                    fginitial_value = fgcurrent_value;
                    ucmiddle_period = ucdqs_dly;
                    ucsearch_state = 3;
                }
            }
            else if (ucsearch_state==3)
            {
                // check if change value
                if (fgcurrent_value != fginitial_value)
                {
                    // end of the period, break the loop
                    ucend_period = ucdqs_dly;
                    ucsearch_state = 4;
                    break;
                }
            }
            else
            {
                //nothing
            }
        }
        #else //only 0.5T data
        {
            if (ucsearch_state==0)
            {
                //record initial value at the beginning
                fginitial_value = fgcurrent_value;
                ucsearch_state = 1;
            }
            else if (ucsearch_state==1)
            {
                // check if change value
                if (fgcurrent_value != fginitial_value)
                {
                    // start of the period
                    fginitial_value = fgcurrent_value;
                    ucstart_period = ucdqs_dly;
                    ucsearch_state = 2;
                }
            }
            else if (ucsearch_state==2)
            {
                // check if change value
                if (fgcurrent_value != fginitial_value)
                {
                    // end of the period, break the loop
                    ucend_period = ucdqs_dly;
                    ucsearch_state = 4;
                   break;
                }
            }
        }
        #endif
    }
        if((ucsearch_state==4) || (ucsearch_state==3))
            break;
    }

    //restore to orignal value
    DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));

    if(ucsearch_state!=4)
    {
        if (ucsearch_state!=3)
        {
            mcSHOW_DBG_MSG(("\n\tMIOCK jitter meter - ch=%d\n", p->channel));
            mcSHOW_DBG_MSG(("\tLess than 0.5T data. Cannot calculate delay cell time\n\n"));

            ucg_num_dlycell_perT = 0;   //for LP3 and LP4 lookup table used

            return DRAM_FAIL;
        }
        else
        {
            //Calculate 1 delay cell = ? ps
            // 1T = ? delay cell
            ucg_num_dlycell_perT = (ucmiddle_period - ucstart_period)*2;
            // 1T = ? ps
        }
    }
    else
    {
        //Calculate 1 delay cell = ? ps
        // 1T = ? delay cell
        ucg_num_dlycell_perT = (ucend_period - ucstart_period);
        // 1T = ? ps
    }

    u1ShuLevel = u4IO32ReadFldAlign(DRAMC_REG_SHUSTATUS, SHUSTATUS_SHUFFLE_LEVEL);
    u4PLL5_ADDR = DDRPHY_SHU_PLL5 + SHU_GRP_DDRPHY_OFFSET * u1ShuLevel;
    u4PLL8_ADDR = DDRPHY_SHU_PLL8 + SHU_GRP_DDRPHY_OFFSET * u1ShuLevel;
    u4CA_CMD6 = DDRPHY_SHU_CA_CMD6 + SHU_GRP_DDRPHY_OFFSET * u1ShuLevel;
    u4SDM_PCW = u4IO32ReadFldAlign(u4PLL5_ADDR, SHU_PLL5_RG_RPHYPLL_SDM_PCW);
    u4PREDIV = u4IO32ReadFldAlign(u4PLL8_ADDR, SHU_PLL8_RG_RPHYPLL_PREDIV);
    u4POSDIV = u4IO32ReadFldAlign(u4PLL8_ADDR, SHU_PLL8_RG_RPHYPLL_POSDIV);
    u4CKDIV4 = u4IO32ReadFldAlign(u4CA_CMD6, SHU_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA);
    u4VCOFreq = ((52>>u4PREDIV)*(u4SDM_PCW>>8))>>u4POSDIV;
    u4DataRate = u4VCOFreq>>u4CKDIV4;
    u2real_freq = u4DataRate>>1;

    u2real_period = (U16) (1000000/u2real_freq);
    //calculate delay cell time

    u2gdelay_cell_ps = u2real_period*100 / ucg_num_dlycell_perT;

    if (ucsearch_state==4)
    {
        mcSHOW_DBG_MSG(("\n\tMIOCK jitter meter\tch=%d\n\n"
                        "1T = (%d-%d) = %d dly cells\n"
                        "Clock freq = %d MHz, period = %d ps, 1 dly cell = %d/100 ps\n",
                            p->channel,
                            ucend_period, ucstart_period, ucg_num_dlycell_perT,
                            u2real_freq, u2real_period, u2gdelay_cell_ps));
    }
    else
    {
        mcSHOW_DBG_MSG(("\n\tMIOCK jitter meter\tch=%d\n\n"
                        "1T = (%d-%d)*2 = %d dly cells\n"
                        "Clock freq = %d MHz, period = %d ps, 1 dly cell = %d/100 ps\n",
                            p->channel,
                            ucmiddle_period, ucstart_period, ucg_num_dlycell_perT,
                            u2real_freq, u2real_period, u2gdelay_cell_ps));
    }

    return DRAM_OK;

// log example
/* dly: sample_cnt   DQS0_cnt  DQS1_cnt
    0 : 10962054,        0,        0
    1 : 10958229,        0,        0
    2 : 10961109,        0,        0
    3 : 10946916,        0,        0
    4 : 10955421,        0,        0
    5 : 10967274,        0,        0
    6 : 10893582,        0,        0
    7 : 10974762,        0,        0
    8 : 10990278,        0,        0
    9 : 10972026,        0,        0
   10 :  7421004,        0,        0
   11 : 10943883,        0,        0
   12 : 10984275,        0,        0
   13 : 10955268,        0,        0
   14 : 10960326,        0,        0
   15 : 10952451,        0,        0
   16 : 10956906,        0,        0
   17 : 10960803,        0,        0
   18 : 10944108,        0,        0
   19 : 10959939,        0,        0
   20 : 10959246,        0,        0
   21 : 11002212,        0,        0
   22 : 10919700,        0,        0
   23 : 10977489,        0,        0
   24 : 11009853,        0,        0
   25 : 10991133,        0,        0
   26 : 10990431,        0,        0
   27 : 10970703,    11161,        0
   28 : 10970775,   257118,        0
   29 : 10934442,  9450467,        0
   30 : 10970622, 10968475,        0
   31 : 10968831, 10968831,        0
   32 : 10956123, 10956123,        0
   33 : 10950273, 10950273,        0
   34 : 10975770, 10975770,        0
   35 : 10983024, 10983024,        0
   36 : 10981701, 10981701,        0
   37 : 10936782, 10936782,        0
   38 : 10889523, 10889523,        0
   39 : 10985913, 10985913,    55562
   40 : 10970235, 10970235,   272294
   41 : 10996056, 10996056,  9322868
   42 : 10972350, 10972350, 10969738
   43 : 10963917, 10963917, 10963917
   44 : 10967895, 10967895, 10967895
   45 : 10961739, 10961739, 10961739
   46 : 10937097, 10937097, 10937097
   47 : 10937952, 10937952, 10937952
   48 : 10926018, 10926018, 10926018
   49 : 10943793, 10943793, 10943793
   50 : 10954638, 10954638, 10954638
   51 : 10968048, 10968048, 10968048
   52 : 10944036, 10944036, 10944036
   53 : 11012112, 11012112, 11012112
   54 : 10969137, 10969137, 10969137
   55 : 10968516, 10968516, 10968516
   56 : 10952532, 10952532, 10952532
   57 : 10985832, 10985832, 10985832
   58 : 11002527, 11002527, 11002527
   59 : 10950660, 10873571, 10950660
   60 : 10949022, 10781797, 10949022
   61 : 10974366, 10700617, 10974366
   62 : 10972422,  1331974, 10972422
   63 : 10926567,        0, 10926567
   64 : 10961658,        0, 10961658
   65 : 10978893,        0, 10978893
   66 : 10962828,        0, 10962828
   67 : 10957599,        0, 10957599
   68 : 10969227,        0, 10969227
   69 : 10960722,        0, 10960722
   70 : 10970937,        0, 10963180
   71 : 10962054,        0, 10711639
   72 : 10954719,        0, 10612707
   73 : 10958778,        0,   479589
   74 : 10973898,        0,        0
   75 : 11004156,        0,        0
   76 : 10944261,        0,        0
   77 : 10955340,        0,        0
   78 : 10998153,        0,        0
   79 : 10998774,        0,        0
   80 : 10953234,        0,        0
   81 : 10960020,        0,        0
   82 : 10923831,        0,        0
   83 : 10951362,        0,        0
   84 : 10965249,        0,        0
   85 : 10949103,        0,        0
   86 : 10948707,        0,        0
   87 : 10941147,        0,        0
   88 : 10966572,        0,        0
   89 : 10971333,        0,        0
   90 : 10943721,        0,        0
   91 : 10949337,        0,        0
   92 : 10965942,        0,        0
   93 : 10970397,        0,        0
   94 : 10956429,        0,        0
   95 : 10939896,        0,        0
   96 : 10967112,        0,        0
   97 : 10951911,        0,        0
   98 : 10953702,        0,        0
   99 : 10971090,        0,        0
  100 : 10939590,        0,        0
  101 : 10993392,        0,        0
  102 : 10975932,        0,        0
  103 : 10949499,    40748,        0
  104 : 10962522,   258638,        0
  105 : 10951524,   275292,        0
  106 : 10982475,   417642,        0
  107 : 10966887, 10564347,        0
  ===============================================================================
      MIOCK jitter meter - channel=0
  ===============================================================================
  1T = (107-29) = 78 delay cells
  Clock frequency = 936 MHz, Clock period = 1068 ps, 1 delay cell = 13 ps
*/
}


/* "picoseconds per delay cell" depends on Vcore only (frequency doesn't matter)
 * 1. Retrieve current freq's vcore voltage using pmic API
 * 2. Perform delay cell time calculation (Bypass if shuffle vcore value is the same as before)
 */
static void GetVcoreDelayCellTime(DRAMC_CTX_T *p, U8 shuffleIdx)
{
    static U32 u4previousVcore = 0;
    U32 channel_i;

#if __ETT__
#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
    u4gVcore[shuffleIdx] = pmic_vcore_voltage_read();
#endif

    /* delay cell calculation is skipped if vcore is same as previous shuffle's */
    if (u4gVcore[shuffleIdx] != u4previousVcore)
    {
        u4previousVcore = u4gVcore[shuffleIdx];
        DramcMiockJmeter(p);
    }
#else
        DramcMiockJmeter(p);
#endif

    for(channel_i=CHANNEL_A; channel_i < p->support_channel_num; channel_i++)
    {
        ucg_num_dlycell_perT_all[shuffleIdx][channel_i] = ucg_num_dlycell_perT;
        u2gdelay_cell_ps_all[shuffleIdx][channel_i] = u2gdelay_cell_ps;
    }
#if __ETT__
    mcSHOW_DBG_MSG(("Freq=%d, CH_%d, VCORE=%d, cell=%d\n", p->frequency, p->channel, u4gVcore[shuffleIdx], u2gdelay_cell_ps_all[shuffleIdx][p->channel]));
#endif

    return;
}


void DramcMiockJmeterHQA(DRAMC_CTX_T *p)
{
    //do MiockJitterMeter@DDR2667
    U8 shuffleIdx;

    mcSHOW_DBG_MSG(("[MiockJmeterHQA]\n"));

    shuffleIdx = get_shuffleIndex_by_Freq(p);

    if(p->channel == CHANNEL_A)
    {
        if (p->frequency <= 600)
            ucg_num_dlycell_perT_all[shuffleIdx][p->channel] = 0; // always lookup table
        else
        GetVcoreDelayCellTime(p, shuffleIdx);
    }

    u2gdelay_cell_ps_all[shuffleIdx][CHANNEL_B] = u2gdelay_cell_ps_all[shuffleIdx][CHANNEL_A];

#ifdef FOR_HQA_TEST_USED
        if (ucg_num_dlycell_perT_all[shuffleIdx][p->channel] == 0) GetVcoreDelayCellTimeFromTable(p); //lookup table
#endif          

    /* Use highest freq's delay cell time measurement results as reference */
    p->ucnum_dlycell_perT = ucg_num_dlycell_perT_all[shuffleIdx][p->channel];
    p->u2DelayCellTimex100 = u2gdelay_cell_ps_all[shuffleIdx][p->channel];    
    mcSHOW_DBG_MSG3(("DelayCellTimex100 CH_%d, (VCORE=%d, cell=%d)\n",p->channel, u4gVcore[shuffleIdx], p->u2DelayCellTimex100));     
}
#endif //#ifdef ENABLE_MIOCK_JMETER




#if ENABLE_DUTY_SCAN_V2

#define DutyPrintAllLog         0
#define DutyPrintCalibrationLog 0
#define ONLY_K_DUTY_NO_SET_VALUE 0

#define DUTY_OFFSET_START -8
#define DUTY_OFFSET_END 8

#define CLOCK_PI_START 0
#define CLOCK_PI_END 63
#define CLOCK_PI_STEP 2

#define ClockDutyFailLowerBound 4500    // 45%
#define ClockDutyFailUpperBound 5500    // 55%
#define ClockDutyMiddleBound    5000    // 50%

typedef struct _DUTY_DELAY_RG_SETTING_T
{
    //for CLK/DQS used
    U8  ucDelay;
    U8  ucDelayB;
    U8  ucRev_Bit0;
    U8  ucRev_Bit1;
    //for DQ/DQM used
    U8  ucDQDQMDelay;
    U8  ucRev_DQDQM_Bit0;
    U8  ucRev_DQDQM_Bit1;
} DUTY_DELAY_RG_SETTING_T;

void DramcDutyDelayRGSettingConvert(DRAMC_CTX_T *p, S8 scDutyDelay, DUTY_DELAY_RG_SETTING_T *tDelay)
{
    if (scDutyDelay<0)
    {
        tDelay->ucDelay = -scDutyDelay;
        tDelay->ucDelayB = 0;    
        tDelay->ucRev_Bit0 = 1;
        tDelay->ucRev_Bit1 = 0;

        tDelay->ucDQDQMDelay = (-scDutyDelay) | (1<<3); //sign bit
        tDelay->ucRev_DQDQM_Bit0 = 0;
        tDelay->ucRev_DQDQM_Bit1 = 1;        
    }
    else if (scDutyDelay>0)
    {
        tDelay->ucDelay = 0;
        tDelay->ucDelayB= scDutyDelay;    
        tDelay->ucRev_Bit0 = 0;
        tDelay->ucRev_Bit1 = 1;       

        tDelay->ucDQDQMDelay = scDutyDelay;
        tDelay->ucRev_DQDQM_Bit0 = 1;
        tDelay->ucRev_DQDQM_Bit1 = 0;        
    }
    else
    {
        tDelay->ucDelay = 0;
        tDelay->ucDelayB= 0;    
        tDelay->ucRev_Bit0 = 0;
        tDelay->ucRev_Bit1 = 0;

        tDelay->ucDQDQMDelay = 0;
        tDelay->ucRev_DQDQM_Bit0 = 0;
        tDelay->ucRev_DQDQM_Bit1 = 0;        
    }            
    }

void DramcClockDutySetClkDelayCell(DRAMC_CTX_T *p, S8 *scDutyDelay)
{
    U8 u1ShuffleIdx = 0, u1RankIdx = 0;
    U32 save_offset;
    DUTY_DELAY_RG_SETTING_T tDelay;

//    mcSHOW_DBG_MSG(("CH%d, Final CLK duty delay cell = %d\n", p->channel, scDutyDelay[0]));

    DramcDutyDelayRGSettingConvert(p, scDutyDelay[0], &tDelay);
    
#if DUTY_SCAN_V2_ONLY_K_HIGHEST_FREQ
    for(u1ShuffleIdx = 0; u1ShuffleIdx<DRAM_DFS_SHUFFLE_MAX; u1ShuffleIdx++)
#endif
    {
        for(u1RankIdx = 0; u1RankIdx<RANK_MAX; u1RankIdx++)
        {
            save_offset = u1ShuffleIdx * SHU_GRP_DDRPHY_OFFSET + u1RankIdx*DDRPHY_AO_RANK_OFFSET_0x100;       
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_CA_CMD1) + save_offset, P_Fld(tDelay.ucDelay, SHU_R0_CA_CMD1_RK0_TX_ARCLK_DLY) | P_Fld(tDelay.ucDelay, SHU_R0_CA_CMD1_RK0_TX_ARCLKB_DLY));                                                
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_CA_CMD0) + save_offset, P_Fld(tDelay.ucDelayB, SHU_R0_CA_CMD0_RK0_TX_ARCLK_DLYB) | P_Fld(tDelay.ucDelayB, SHU_R0_CA_CMD0_RK0_TX_ARCLKB_DLYB));                                                            

            save_offset = u1ShuffleIdx * SHU_GRP_DDRPHY_OFFSET;
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_CA_CMD3), P_Fld(tDelay.ucRev_Bit0, SHU_CA_CMD3_RG_TX_ARCMD_PU_BIT0) | P_Fld(tDelay.ucRev_Bit1, SHU_CA_CMD3_RG_TX_ARCMD_PU_BIT1));                             
        }
    }
}

void DQSDutyScan_SetDqsDelayCell(DRAMC_CTX_T *p, S8 *scDutyDelay)
{
    U8 u1ShuffleIdx = 0, u1DQSIdx, u1RankIdx = 0;
    U32 save_offset;
    DUTY_DELAY_RG_SETTING_T tDelay[2];

//    mcSHOW_DBG_MSG(("CH%d, Final DQS0 duty delay cell = %d\n", p->channel, scDutyDelay[0]));
//    mcSHOW_DBG_MSG(("CH%d, Final DQS1 duty delay cell = %d\n", p->channel, scDutyDelay[1]));

    for(u1DQSIdx=0; u1DQSIdx<2; u1DQSIdx++)
    {
        DramcDutyDelayRGSettingConvert(p, scDutyDelay[u1DQSIdx], &(tDelay[u1DQSIdx]));
    }

#if DUTY_SCAN_V2_ONLY_K_HIGHEST_FREQ
    for(u1ShuffleIdx = 0; u1ShuffleIdx<DRAM_DFS_SHUFFLE_MAX; u1ShuffleIdx++)
#endif        
    {
        for(u1RankIdx = 0; u1RankIdx<RANK_MAX; u1RankIdx++)
        {
            for(u1DQSIdx = 0; u1DQSIdx<2; u1DQSIdx++)
        {
                save_offset = u1ShuffleIdx * SHU_GRP_DDRPHY_OFFSET + u1RankIdx*DDRPHY_AO_RANK_OFFSET_0x100 + u1DQSIdx*DDRPHY_AO_B0_B1_OFFSET_0X50;                
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ1) + save_offset, P_Fld(tDelay[u1DQSIdx].ucDelay, SHU_R0_B0_DQ1_RK0_TX_ARDQS0_DLY_B0) | P_Fld(tDelay[u1DQSIdx].ucDelay, SHU_R0_B0_DQ1_RK0_TX_ARDQS0B_DLY_B0));
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ1) + save_offset, P_Fld(tDelay[u1DQSIdx].ucDelayB, SHU_R0_B0_DQ1_RK0_TX_ARDQS0_DLYB_B0) | P_Fld(tDelay[u1DQSIdx].ucDelayB, SHU_R0_B0_DQ1_RK0_TX_ARDQS0B_DLYB_B0));

                    save_offset = u1ShuffleIdx * SHU_GRP_DDRPHY_OFFSET + u1DQSIdx*DDRPHY_AO_B0_B1_OFFSET_0X80;
                    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DLL1) + save_offset, P_Fld(tDelay[u1DQSIdx].ucRev_Bit0, SHU_B0_DLL1_DQS_MCK4X_DLY_EN_B0) | P_Fld(tDelay[u1DQSIdx].ucRev_Bit1, SHU_B0_DLL1_DQS_MCK4XB_DLY_EN_B0));
            }
        }
    }
        }

#if APPLY_DQDQM_DUTY_CALIBRATION
void DQDQMDutyScan_SetDQDQMDelayCell(DRAMC_CTX_T *p, U8 u1ChannelIdx, S8 *scDutyDelay, U8 k_type)
{
    U8 u1ShuffleIdx = 0, u1DQSIdx, u1RankIdx = 0;
    U32 save_offset;
    DUTY_DELAY_RG_SETTING_T tDelay[2];

#if 0
    if (k_type == DutyScan_Calibration_K_DQ)
    {
        mcSHOW_DBG_MSG(("CH%d, Final DQ0 duty delay cell = %d\n", p->channel, scDutyDelay[0]));
        mcSHOW_DBG_MSG(("CH%d, Final DQ1 duty delay cell = %d\n", p->channel, scDutyDelay[1]));
    }
    else
    {
        mcSHOW_DBG_MSG(("CH%d, Final DQM0 duty delay cell = %d\n", p->channel, scDutyDelay[0]));
        mcSHOW_DBG_MSG(("CH%d, Final DQM1 duty delay cell = %d\n", p->channel, scDutyDelay[1]));
    }
#endif    

    for(u1DQSIdx=0; u1DQSIdx<2; u1DQSIdx++)
    {    
        DramcDutyDelayRGSettingConvert(p, scDutyDelay[u1DQSIdx], &(tDelay[u1DQSIdx]));
    }

#if DUTY_SCAN_V2_ONLY_K_HIGHEST_FREQ
    for(u1ShuffleIdx = 0; u1ShuffleIdx<DRAM_DFS_SHUFFLE_MAX; u1ShuffleIdx++)
#endif        
    {
        for(u1DQSIdx = 0; u1DQSIdx<2; u1DQSIdx++)
        {
            save_offset = u1ShuffleIdx * SHU_GRP_DDRPHY_OFFSET + u1DQSIdx*DDRPHY_AO_B0_B1_OFFSET_0X80;                
            
            if (k_type == DutyScan_Calibration_K_DQM)
        {
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ3) + save_offset, P_Fld(tDelay[u1DQSIdx].ucDQDQMDelay, SHU_B0_DQ3_RG_ARDQ_DUTYREV_B0_DQM_DLY)
                                                                        | P_Fld(tDelay[u1DQSIdx].ucRev_DQDQM_Bit0, SHU_B0_DQ3_RG_TX_ARDQS0_PU_B0_BIT0)
                                                                        | P_Fld(tDelay[u1DQSIdx].ucRev_DQDQM_Bit1, SHU_B0_DQ3_RG_TX_ARDQS0_PU_B0_BIT1)); 
            }            

            if ((k_type == DutyScan_Calibration_K_DQ) || (k_type == DutyScan_Calibration_K_DQM)) //DQM is special case
            {
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ3) + save_offset, P_Fld(tDelay[u1DQSIdx].ucDQDQMDelay, SHU_B0_DQ3_RG_ARDQ_DUTYREV_B0_DQ_DLY)
                                                                        | P_Fld(tDelay[u1DQSIdx].ucRev_DQDQM_Bit0, SHU_B0_DQ3_RG_TX_ARDQS0_PU_PRE_B0_BIT0)
                                                                        | P_Fld(tDelay[u1DQSIdx].ucRev_DQDQM_Bit1, SHU_B0_DQ3_RG_TX_ARDQS0_PU_PRE_B0_BIT1)); 
            }
        }
    }
}

#if 0
void DQDQMDutyScan_CopyDQRG2DQMRG(DRAMC_CTX_T *p)
{
    U8 u1ShuffleIdx = 0, u1DQSIdx, u1RankIdx = 0;
    U32 save_offset;
    U8 ucDQDQMDelay;
    U8 ucRev_DQDQM_Bit0, ucRev_DQDQM_Bit1;    

#if DUTY_SCAN_V2_ONLY_K_HIGHEST_FREQ
    for(u1ShuffleIdx = 0; u1ShuffleIdx<DRAM_DFS_SHUFFLE_MAX; u1ShuffleIdx++)
#endif
    {
            for(u1DQSIdx = 0; u1DQSIdx<2; u1DQSIdx++)
            {
            save_offset = u1ShuffleIdx * SHU_GRP_DDRPHY_OFFSET + u1DQSIdx*DDRPHY_AO_B0_B1_OFFSET_0X80;                

            ucDQDQMDelay = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ3) + save_offset, SHU_B0_DQ3_RG_ARDQ_DUTYREV_B0_DQ_DLY);
            ucRev_DQDQM_Bit0 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ3) + save_offset, SHU_B0_DQ3_RG_TX_ARDQS0_PU_PRE_B0_BIT0);
            ucRev_DQDQM_Bit1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ3) + save_offset, SHU_B0_DQ3_RG_TX_ARDQS0_PU_PRE_B0_BIT0);            

            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ3) + save_offset, P_Fld(ucDQDQMDelay, SHU_B0_DQ3_RG_ARDQ_DUTYREV_B0_DQM_DLY)
                                                                    | P_Fld(ucRev_DQDQM_Bit0, SHU_B0_DQ3_RG_TX_ARDQS0_PU_B0_BIT0)
                                                                    | P_Fld(ucRev_DQDQM_Bit1, SHU_B0_DQ3_RG_TX_ARDQS0_PU_B0_BIT1)); 
            }
        }
    }
#endif
#endif


// offset is not related to DQ/DQM/DQS
// we have a circuit to measure duty, But this circuit is not very accurate
// so we need to K offset of this circuit first
// After we got this offset, then we can use it to measure duty
// this offset can measure DQ/DQS/DQM, and every byte has this circuit, too.
// B0/B1/CA all have one circuit.
// CA's circuit can measure CLK duty
// B0/B1's can measure DQ/DQM/DQS duty
S8 DutyScan_Offset_Convert(U8 val)
{
    U8 calibration_sequence[15]={0xf, 0xe, 0xd, 0xc, 0xb, 0xa, 0x9, 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7};

    return ((S8)(calibration_sequence[val]>8 ? 0-(calibration_sequence[val]&0x7) : calibration_sequence[val]));

}
void DutyScan_Offset_Calibration(DRAMC_CTX_T *p)
{
    U8 calibration_sequence[15]={0xf, 0xe, 0xd, 0xc, 0xb, 0xa, 0x9, 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7};
    U8 i, read_val_b0, read_val_b1, read_val_ca;
    U8 cal_i_b0=0xff, cal_i_b1=0xff, cal_i_ca=0xff;

#if VENDER_JV_LOG
    vPrintCalibrationBasicInfo_ForJV(p);
#else
    vPrintCalibrationBasicInfo(p);
#endif

    mcSHOW_DBG_MSG(("[Duty_Offset_Calibration]\n"));

    //B0/B1/CA RG setting
    for(i=0; i<3; i++)
    {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B0_DQ6) + i*DDRPHY_AO_B0_B1_OFFSET_0X80, P_Fld(1, B0_DQ6_RG_RX_ARDQ_LPBK_EN_B0) | P_Fld(0, B0_DQ6_RG_RX_ARDQ_DDR4_SEL_B0) | P_Fld(1, B0_DQ6_RG_RX_ARDQ_DDR3_SEL_B0));

        if (i<2)
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DLL1) + i*DDRPHY_AO_B0_B1_OFFSET_0X80, P_Fld(1, SHU_B0_DLL1_DATA_SWAP_EN_B0) | P_Fld(2, SHU_B0_DLL1_DATA_SWAP_B0));
        }
        else
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_CA_DLL2), P_Fld(1, SHU_CA_DLL2_DATA_SWAP_EN) | P_Fld(2, SHU_CA_DLL2_DATA_SWAP));        
        }
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ5) + i*DDRPHY_AO_B0_B1_OFFSET_0X80, P_Fld(0, SHU_B0_DQ5_RG_RX_ARDQ_VREF_BYPASS_B0) | P_Fld(0x14, SHU_B0_DQ5_RG_RX_ARDQ_VREF_SEL_B0));        
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ0) + i*DDRPHY_AO_B0_B1_OFFSET_0X80, 1, SHU_B0_DQ0_RG_TX_ARDQS0_DRVP_PRE_B0_BIT1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ0) + i*DDRPHY_AO_B0_B1_OFFSET_0X80, 0, B0_DQ0_RG_RX_ARDQ2_OFFC_B0);    
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B0_DQ5) + i*DDRPHY_AO_B0_B1_OFFSET_0X80, P_Fld(1, B0_DQ5_RG_RX_ARDQ_VREF_EN_B0) | P_Fld(0x1, B0_DQ5_RG_RX_ARDQ_EYE_VREF_EN_B0));    
        mcDELAY_US(1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ0) + i*DDRPHY_AO_B0_B1_OFFSET_0X80, 1, SHU_B0_DQ0_RG_TX_ARDQS0_DRVP_PRE_B0_BIT2);

        mcDELAY_US(1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ0) + i*DDRPHY_AO_B0_B1_OFFSET_0X80, 1, SHU_B0_DQ0_RG_TX_ARDQS0_DRVP_PRE_B0_BIT0);        
    }

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_CA_CMD6), P_Fld(1, CA_CMD6_RG_RX_ARCMD_DDR3_SEL) | P_Fld(0, CA_CMD6_RG_RX_ARCMD_DDR4_SEL));

#if DutyPrintCalibrationLog
    mcSHOW_DBG_MSG(("\tB0\tB1\tCA\n"));
    mcSHOW_DBG_MSG(("===========================\n"));
#endif

    for(i=0; i<15; i++)
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ0), calibration_sequence[i], B0_DQ0_RG_RX_ARDQ2_OFFC_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ0), calibration_sequence[i], B1_DQ0_RG_RX_ARDQ2_OFFC_B1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_CMD0), calibration_sequence[i], CA_CMD0_RG_RX_ARCA2_OFFC);

        mcDELAY_US(1);

        read_val_b0 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_AD_RX_DQ_O1), MISC_AD_RX_DQ_O1_AD_RX_ARDQ_O1_B0_BIT2);
        read_val_b1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_AD_RX_DQ_O1), MISC_AD_RX_DQ_O1_AD_RX_ARDQ_O1_B1_BIT2);
        read_val_ca = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_AD_RX_CMD_O1), MISC_AD_RX_CMD_O1_AD_RX_ARCA2_O1);

#if DutyPrintCalibrationLog
        mcSHOW_DBG_MSG(("%d\t%d\t%d\t%d\n", DutyScan_Offset_Convert(i), read_val_b0, read_val_b1, read_val_ca));
#endif

        if (read_val_b0 == 0 && cal_i_b0==0xff)
        {
            cal_i_b0 = i;
        }

        if (read_val_b1 == 0 && cal_i_b1==0xff)
        {
            cal_i_b1 = i;
        }

        if (read_val_ca == 0 && cal_i_ca==0xff)
        {
            cal_i_ca = i;
        }
    }

    if (cal_i_b0==0 || cal_i_b1==0 || cal_i_ca==0)
    {
        mcSHOW_ERR_MSG(("offset calibration i=-7 and AD_RX_*RDQ_O1_B*<2>/AD_RX_*RCA2_O1 ==0 !!\n"));
    }
    else if ((read_val_b0==1 && cal_i_b0==0xff) || (read_val_b1==1 && cal_i_b1==0xff) || (read_val_ca==1 && cal_i_ca==0xff))
    {
        mcSHOW_ERR_MSG(("offset calibration i=7 and AD_RX_*RDQ_O1_B*<2>/AD_RX_*RCA2_O1 ==1 !!\n"));
        if (read_val_b0==1 && cal_i_b0==0xff)
            cal_i_b0 = (i-1); //max
        if (read_val_b1==1 && cal_i_b1==0xff)
            cal_i_b1 = (i-1); //max
        if (read_val_ca==1 && cal_i_ca==0xff)
            cal_i_ca = (i-1); //max
    }

#if DutyPrintCalibrationLog
    mcSHOW_DBG_MSG(("===========================\n"));
#endif
    mcSHOW_DBG_MSG(("\tB0:%d\tB1:%d\tCA:%d\n",DutyScan_Offset_Convert(cal_i_b0),DutyScan_Offset_Convert(cal_i_b1),DutyScan_Offset_Convert(cal_i_ca)));

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ0), 0, SHU_B0_DQ0_RG_TX_ARDQS0_DRVP_PRE_B0_BIT0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B1_DQ0), 0, SHU_B1_DQ0_RG_TX_ARDQS0_DRVP_PRE_B1_BIT0);    
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_CA_CMD0), 0, SHU_CA_CMD0_RG_TX_ARCLK_DRVP_PRE_BIT0);     

    if (cal_i_b0!=0xff) vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ0), calibration_sequence[cal_i_b0], B0_DQ0_RG_RX_ARDQ2_OFFC_B0);
    if (cal_i_b1!=0xff) vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ0), calibration_sequence[cal_i_b1], B1_DQ0_RG_RX_ARDQ2_OFFC_B1);
    if (cal_i_ca!=0xff) vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_CMD0), calibration_sequence[cal_i_ca], CA_CMD0_RG_RX_ARCA2_OFFC);

    return;
}

#if defined(YH_SWEEP_IC)
typedef struct _YH_SWEEP_IC_T
{
    U32 maxduty;
    U32 minduty;
    U32 dutydiff;
    U32 avgduty;
} YH_SWEEP_IC_T;

YH_SWEEP_IC_T gYH_Sweep_IC_test_result[4][CHANNEL_NUM][DQS_NUMBER];

#define YH_SWEEP_IC_PASS_CRITERIO 1 // 0: FT  1: SLT
void YH_Sweep_IC_Print_Result(DRAMC_CTX_T *p)
{
    U8 u1ChannelIdx, u1ByteIdx, k_type;
    U8 u1ByteIdxNum;

    //  SLT:
    //        CHB CLK duty max-min j5.3%: FAIL0
    //        NDQS duty max-min j5.8%: FAIL1
    //        NDQDQM maxduty j54.5% or min_duty<45.5% or max-min j5.8%: FAIL2

    mcSHOW_ERR_MSG(("\n\n YH Sweep IC Print Result =========\n"));

    for(k_type=0; k_type<4; k_type++)
    {

        if (k_type == DutyScan_Calibration_K_CLK) u1ByteIdxNum = 1;
        else u1ByteIdxNum = 2;

        for(u1ChannelIdx=0; u1ChannelIdx<CHANNEL_NUM; u1ChannelIdx++)
                for(u1ByteIdx=0; u1ByteIdx<u1ByteIdxNum; u1ByteIdx++)
                {
                    if (k_type == DutyScan_Calibration_K_CLK && u1ChannelIdx == CHANNEL_B)
                    {
                        mcSHOW_ERR_MSG(("CH%d CLK max-min Duty %d%% : ",u1ChannelIdx, gYH_Sweep_IC_test_result[k_type][u1ChannelIdx][u1ByteIdx].dutydiff));
#if YH_SWEEP_IC_PASS_CRITERIO                        
                        if (gYH_Sweep_IC_test_result[k_type][u1ChannelIdx][u1ByteIdx].dutydiff > 550)
#else
                        if (gYH_Sweep_IC_test_result[k_type][u1ChannelIdx][u1ByteIdx].dutydiff > 510)
#endif
                        {
                            mcSHOW_ERR_MSG(("FAIL0\n"));
                        }
                        else
                        {
                            mcSHOW_ERR_MSG(("PASS\n"));
                        }
                    }
                    if (k_type == DutyScan_Calibration_K_DQS)
                    {
                        mcSHOW_ERR_MSG(("CH%d DQS Byte %d max-min Duty %d%% : ",u1ChannelIdx, u1ByteIdx, gYH_Sweep_IC_test_result[k_type][u1ChannelIdx][u1ByteIdx].dutydiff));
#if YH_SWEEP_IC_PASS_CRITERIO                        
                        if (gYH_Sweep_IC_test_result[k_type][u1ChannelIdx][u1ByteIdx].dutydiff > 600)
#else
                        if (gYH_Sweep_IC_test_result[k_type][u1ChannelIdx][u1ByteIdx].dutydiff > 560)
#endif
                        {
                            mcSHOW_ERR_MSG(("FAIL1\n"));
                        }
                        else
                        {
                            mcSHOW_ERR_MSG(("PASS\n"));
                        }
                    }
                    if (k_type == DutyScan_Calibration_K_DQ || k_type == DutyScan_Calibration_K_DQM)
                    {
                        mcSHOW_ERR_MSG(("CH%d %s Byte %d max Duty %d%%, min Duty %d%% : ",u1ChannelIdx, k_type == DutyScan_Calibration_K_DQ ? "DQ" : "DQM", u1ByteIdx, gYH_Sweep_IC_test_result[k_type][u1ChannelIdx][u1ByteIdx].maxduty, gYH_Sweep_IC_test_result[k_type][u1ChannelIdx][u1ByteIdx].minduty));
#if YH_SWEEP_IC_PASS_CRITERIO                        
                        if  (gYH_Sweep_IC_test_result[k_type][u1ChannelIdx][u1ByteIdx].minduty < 4550 || gYH_Sweep_IC_test_result[k_type][u1ChannelIdx][u1ByteIdx].maxduty > 5450 || gYH_Sweep_IC_test_result[k_type][u1ChannelIdx][u1ByteIdx].dutydiff > 580)
#else
                        if  (gYH_Sweep_IC_test_result[k_type][u1ChannelIdx][u1ByteIdx].minduty < 4600 || gYH_Sweep_IC_test_result[k_type][u1ChannelIdx][u1ByteIdx].maxduty > 5400 || gYH_Sweep_IC_test_result[k_type][u1ChannelIdx][u1ByteIdx].dutydiff > 500)
#endif
                        {
                            mcSHOW_ERR_MSG(("FAIL2\n"));
                        }
                        else
                        {
                            mcSHOW_ERR_MSG(("PASS\n"));
                        }
                    }
                }
    }
}
#endif

S8 gcFinal_K_CLK_delay_cell[CHANNEL_NUM][DQS_NUMBER];
S8 gcFinal_K_DQS_delay_cell[CHANNEL_NUM][DQS_NUMBER];
#if APPLY_DQDQM_DUTY_CALIBRATION
S8 gcFinal_K_DQ_delay_cell[CHANNEL_NUM][DQS_NUMBER];
S8 gcFinal_K_DQM_delay_cell[CHANNEL_NUM][DQS_NUMBER];
#endif
DRAM_STATUS_T DutyScan_Calibration_Flow(DRAMC_CTX_T *p, U8 k_type, U8 ft_test_type)
{
    S8 scinner_duty_ofst, scFinal_clk_delay_cell[DQS_NUMBER]={0,0}, scinner_duty_ofst_step;
    S8 scinner_duty_ofst_start = 0, scinner_duty_ofst_end = 0;
    S32 scdqs_dly, s4PICnt, s4PIBegin, s4PIEnd, s4PICnt_mod64;
    S8 i, swap_idx, ucdqs_i, ucdqs_i_count=2;
    U8 u1ByteIdx;
    U8 ucDelay, ucDelayB, ucDQDQMDelay;
    U8 ucRev_Bit0, ucRev_Bit1, ucRev_DQDQM_Bit0, ucRev_DQDQM_Bit1;
    U32 u4DutyDiff, u4DutyDiff_Limit=900;

    U8 vref_sel_value[2], cal_out_value;
    S32 duty_value[2];
    S32 final_duty;

    U32 ucperiod_duty_max=0, ucperiod_duty_min=0xffffffff, ucperiod_duty_max_clk_dly=0, ucperiod_duty_min_clk_dly=0;
    U32 ucperiod_duty_averige=0, ucFinal_period_duty_averige[DQS_NUMBER]={0,0}, ucmost_approach_50_percent=0xffffffff;
    U32 ucFinal_period_duty_max[DQS_NUMBER] = {0,0}, ucFinal_period_duty_min[DQS_NUMBER] = {0,0};
    U32 ucFinal_duty_max_clk_dly[DQS_NUMBER]={0},ucFinal_duty_min_clk_dly[DQS_NUMBER]={0};
    U8 early_break_count=0;
    U8 str_clk_duty[]="CLK", str_dqs_duty[]="DQS", str_dq_duty[]="DQ", str_dqm_duty[]="DQM";
    U8 *str_who_am_I=str_clk_duty;
#if FT_DSIM_USED    
    S32 s4FT_Test_Final_duty[2][3]={0};
#endif

    DUTY_DELAY_RG_SETTING_T tDelay;

    mcSHOW_ERR_MSG(("\n[DutyScan_Calibration_Flow] k_type=%d\n", k_type));
    /*TINFO="\n[DutyScan_Calibration_Flow] k_type=%d\n", k_type */


    if (k_type == DutyScan_Calibration_K_CLK)
    {
#if 0
        // DQS duty test 3
        //mcSHOW_ERR_MSG(("\n[*PHDET_EN*=0]\n"));
        mcSHOW_ERR_MSG(("\n[*PI*RESETB*=0  *PHDET_EN*=0  *PI_RESETB*=1]\n"));
        /*TINFO="\n[*PI*RESETB*=0  *PHDET_EN*=0  *PI_RESETB*=1]\n" */
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_DLL_ARPI0), 0x0, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_DLL_ARPI2), 0x0, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_DLL_ARPI0), 0x1, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);
#else
#if DutyPrintCalibrationLog
        mcSHOW_ERR_MSG(("\n[  *PHDET_EN*=0  \n"));
#endif
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_DLL_ARPI2), 0x0, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);
#endif
    }
    else
    {
        // DQS duty test 3
        //mcSHOW_ERR_MSG(("\n[*PHDET_EN*=0]\n"));
#if DutyPrintCalibrationLog        
        mcSHOW_ERR_MSG(("[*PI*RESETB*=0  *PHDET_EN*=0  *PI_RESETB*=1]\n"));
#endif
        /*TINFO="[*PI*RESETB*=0  *PHDET_EN*=0  *PI_RESETB*=1]\n" */
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DLL_ARPI0), 0x0, B0_DLL_ARPI0_RG_ARPI_RESETB_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DLL_ARPI0), 0x0, B1_DLL_ARPI0_RG_ARPI_RESETB_B1);

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DLL_ARPI2), 0x0, B0_DLL_ARPI2_RG_ARDLL_PHDET_EN_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DLL_ARPI2), 0x0, B1_DLL_ARPI2_RG_ARDLL_PHDET_EN_B1);

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DLL_ARPI0), 0x1, B0_DLL_ARPI0_RG_ARPI_RESETB_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DLL_ARPI0), 0x1, B1_DLL_ARPI0_RG_ARPI_RESETB_B1);
    }

    //CLK Source Select (DQ/DQM/DQS/CLK)
    if (k_type == DutyScan_Calibration_K_DQ) // K DQ
    {
#if APPLY_DQDQM_DUTY_CALIBRATION
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(p->femmc_Ready==1)
        {
            DQDQMDutyScan_SetDQDQMDelayCell(p, p->channel, p->pSavetimeData->s1DQDuty_clk_delay_cell[p->channel], DutyScan_Calibration_K_DQ);
            return DRAM_OK;
        }
#endif    
#endif    
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ8), 0, B0_DQ8_RG_TX_ARDQ_CAP_DET_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DLL1), 0, SHU_B0_DLL1_RG_MCK4X_SEL_B0);            

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ8), 0, B1_DQ8_RG_TX_ARDQ_CAP_DET_B1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B1_DLL1), 0, SHU_B1_DLL1_RG_MCK4X_SEL_B1); 

        ucdqs_i_count = 2;
        str_who_am_I = (U8*)str_dq_duty;

#if APPLY_DQDQM_DUTY_CALIBRATION
        scinner_duty_ofst_start = -7;
        scinner_duty_ofst_end = 7;
#else
        scinner_duty_ofst_start = 0;
        scinner_duty_ofst_end = 0;
#endif
    }
    else if (k_type == DutyScan_Calibration_K_DQM) // K DQM
    {
#if APPLY_DQDQM_DUTY_CALIBRATION
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(p->femmc_Ready==1)
        {
            DQDQMDutyScan_SetDQDQMDelayCell(p, p->channel, p->pSavetimeData->s1DQMDuty_clk_delay_cell[p->channel], DutyScan_Calibration_K_DQM);
            return DRAM_OK;
        }
#endif    
#endif
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ8), 0, B0_DQ8_RG_TX_ARDQ_CAP_DET_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DLL1), 1, SHU_B0_DLL1_RG_MCK4X_SEL_B0);

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ8), 0, B1_DQ8_RG_TX_ARDQ_CAP_DET_B1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B1_DLL1), 1, SHU_B1_DLL1_RG_MCK4X_SEL_B1);

        ucdqs_i_count = 2;
        str_who_am_I = (U8*)str_dqm_duty;

#if APPLY_DQDQM_DUTY_CALIBRATION
        scinner_duty_ofst_start = -7;
        scinner_duty_ofst_end = 7;
#else
        scinner_duty_ofst_start = 0;
        scinner_duty_ofst_end = 0;
#endif
    }
    else if (k_type == DutyScan_Calibration_K_DQS) // K DQS
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ8), 1, B0_DQ8_RG_TX_ARDQ_CAP_DET_B0);

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ8), 1, B1_DQ8_RG_TX_ARDQ_CAP_DET_B1);

        ucdqs_i_count = 2;
        str_who_am_I = (U8*)str_dqs_duty;

        scinner_duty_ofst_start = DUTY_OFFSET_START;
        scinner_duty_ofst_end = DUTY_OFFSET_END;
    }
    else if (k_type == DutyScan_Calibration_K_CLK) // K CLK
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_CMD9), 1, CA_CMD9_RG_TX_ARCMD_CAP_DET);

        ucdqs_i_count = 1;
        str_who_am_I = (U8*)str_clk_duty;

        scinner_duty_ofst_start = DUTY_OFFSET_START;
        scinner_duty_ofst_end = DUTY_OFFSET_END;
    }
    scinner_duty_ofst_step = 1;

#if defined(YH_SWEEP_IC) || FT_DSIM_USED
    if (ft_test_type == 0)
    {
        scinner_duty_ofst_start=0;
        scinner_duty_ofst_end=0;
    }
    else
    {
        scinner_duty_ofst_start = DUTY_OFFSET_START;
        scinner_duty_ofst_end = DUTY_OFFSET_END;            
        scinner_duty_ofst_step = 8;
    }
#endif

#if (fcFOR_CHIP_ID == fcLafite)
    if (k_type == DutyScan_Calibration_K_CLK)
    {
        u4DutyDiff_Limit = 530;
    }
    else
    {
        u4DutyDiff_Limit = 580;
    }
#endif

#if (fcFOR_CHIP_ID == fcLafite)   
#if !defined(YH_SWEEP_IC)
    if (k_type == DutyScan_Calibration_K_CLK && p->channel == CHANNEL_A)
    {
        s4PIBegin = 0;
        s4PIEnd = 0;
    }
    else
#endif
#endif
    {
        s4PIBegin = CLOCK_PI_START;
        s4PIEnd = CLOCK_PI_END;
    }

    for(ucdqs_i=0; ucdqs_i<ucdqs_i_count; ucdqs_i++)
    {
#if DutyPrintCalibrationLog    
        if (k_type == DutyScan_Calibration_K_CLK)
        {
            mcSHOW_ERR_MSG(("\n[CLK Duty scan]\n", ucdqs_i));
            /*TINFO="\n[CLK Duty scan]\n", ucdqs_i */
        }
        else
        {
            mcSHOW_ERR_MSG(("\n[%s B%d Duty scan]\n", str_who_am_I, ucdqs_i));
            /*TINFO="\n[%s B%d Duty scan]\n", str_who_am_I, ucdqs_i */
        }
#endif        

        ucmost_approach_50_percent=0xffffffff;
        early_break_count=0;

        for(scinner_duty_ofst=scinner_duty_ofst_start; scinner_duty_ofst<=scinner_duty_ofst_end; scinner_duty_ofst+=scinner_duty_ofst_step)
        {
            ucperiod_duty_max = 0;
            ucperiod_duty_min = 100000;

            DramcDutyDelayRGSettingConvert(p, scinner_duty_ofst, &tDelay);

            if (k_type == DutyScan_Calibration_K_DQS)
            {
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ1) + ucdqs_i*DDRPHY_AO_B0_B1_OFFSET_0X50, P_Fld(tDelay.ucDelay, SHU_R0_B0_DQ1_RK0_TX_ARDQS0_DLY_B0) 
                                                                            | P_Fld(tDelay.ucDelay, SHU_R0_B0_DQ1_RK0_TX_ARDQS0B_DLY_B0) 
                                                                            | P_Fld(tDelay.ucDelayB, SHU_R0_B0_DQ1_RK0_TX_ARDQS0_DLYB_B0) 
                                                                            | P_Fld(tDelay.ucDelayB, SHU_R0_B0_DQ1_RK0_TX_ARDQS0B_DLYB_B0));

                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DLL1) + ucdqs_i*DDRPHY_AO_B0_B1_OFFSET_0X80, P_Fld(tDelay.ucRev_Bit0, SHU_B0_DLL1_DQS_MCK4X_DLY_EN_B0)
                                                                        | P_Fld(tDelay.ucRev_Bit1, SHU_B0_DLL1_DQS_MCK4XB_DLY_EN_B0)); 
            }

            if (k_type == DutyScan_Calibration_K_CLK)
            {
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_CA_CMD1), P_Fld(tDelay.ucDelay, SHU_R0_CA_CMD1_RK0_TX_ARCLK_DLY) 
                                                                            | P_Fld(tDelay.ucDelay, SHU_R0_CA_CMD1_RK0_TX_ARCLKB_DLY));
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_CA_CMD0), P_Fld(tDelay.ucDelayB, SHU_R0_CA_CMD0_RK0_TX_ARCLK_DLYB) 
                                                                            | P_Fld(tDelay.ucDelayB, SHU_R0_CA_CMD0_RK0_TX_ARCLKB_DLYB));

                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_CA_CMD3), P_Fld(tDelay.ucRev_Bit0, SHU_CA_CMD3_RG_TX_ARCMD_PU_BIT0)
                                                                        | P_Fld(tDelay.ucRev_Bit1, SHU_CA_CMD3_RG_TX_ARCMD_PU_BIT1));                     
            }


#if APPLY_DQDQM_DUTY_CALIBRATION
            if (k_type == DutyScan_Calibration_K_DQ || k_type == DutyScan_Calibration_K_DQM)
            {
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ3) + ucdqs_i*DDRPHY_AO_B0_B1_OFFSET_0X80, P_Fld(tDelay.ucDQDQMDelay, SHU_B0_DQ3_RG_ARDQ_DUTYREV_B0_DQ_DLY)
                                                                        | P_Fld(tDelay.ucRev_DQDQM_Bit0, SHU_B0_DQ3_RG_TX_ARDQS0_PU_PRE_B0_BIT0)
                                                                        | P_Fld(tDelay.ucRev_DQDQM_Bit1, SHU_B0_DQ3_RG_TX_ARDQS0_PU_PRE_B0_BIT1)); 
            }

#if 0
            if (k_type == DutyScan_Calibration_K_DQM)
            {
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ3) + ucdqs_i*DDRPHY_AO_B0_B1_OFFSET_0X80, P_Fld(tDelay.ucDQDQMDelay, SHU_B0_DQ3_RG_ARDQ_DUTYREV_B0_DQM_DLY)
                                                                        | P_Fld(tDelay.ucRev_DQDQM_Bit0, SHU_B0_DQ3_RG_TX_ARDQS0_PU_B0_BIT0)
                                                                        | P_Fld(tDelay.ucRev_DQDQM_Bit1, SHU_B0_DQ3_RG_TX_ARDQS0_PU_B0_BIT1)); 
            }
#endif            
#endif

            for(s4PICnt=s4PIBegin; s4PICnt<=s4PIEnd; s4PICnt+=CLOCK_PI_STEP)
            {
                s4PICnt_mod64 = (s4PICnt+64)&0x3f;//s4PICnt_mod64 = (s4PICnt+64)%64;
#if DutyPrintAllLog                
                //if(scinner_duty_ofst!=DUTY_OFFSET_START)
                mcSHOW_ERR_MSG(("PI= %d\n", s4PICnt_mod64));
#endif                

                if (k_type == DutyScan_Calibration_K_DQS)
                {
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ7) + ucdqs_i*DDRPHY_AO_B0_B1_OFFSET_0X50, s4PICnt_mod64, SHU_R0_B0_DQ7_RK0_ARPI_PBYTE_B0); 
                    }
                else
                if (k_type == DutyScan_Calibration_K_CLK)
                {
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_CA_CMD9), s4PICnt_mod64, SHU_R0_CA_CMD9_RG_RK0_ARPI_CLK); 
                }
                else
                if (k_type == DutyScan_Calibration_K_DQ)
                {
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ7) + ucdqs_i*DDRPHY_AO_B0_B1_OFFSET_0X50, s4PICnt_mod64, SHU_R0_B0_DQ7_RK0_ARPI_DQ_B0); 
                }
                else
                if (k_type == DutyScan_Calibration_K_DQM)
                {
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ7) + ucdqs_i*DDRPHY_AO_B0_B1_OFFSET_0X50, s4PICnt_mod64, SHU_R0_B0_DQ7_RK0_ARPI_DQM_B0); 
                }

                for(swap_idx=0; swap_idx<2; swap_idx++)
                {
                    if (k_type == DutyScan_Calibration_K_CLK)
                    {
                        if (swap_idx==0)
                        {
                            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_CA_DLL2), 2, SHU_CA_DLL2_DATA_SWAP);
                        }
                        else
                        {
                            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_CA_DLL2), 3, SHU_CA_DLL2_DATA_SWAP);                    
                        }

                        vref_sel_value[swap_idx]= 0;
                        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_CA_CMD5), vref_sel_value[swap_idx]>>1, SHU_CA_CMD5_RG_RX_ARCMD_VREF_SEL);        
                        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_CA_CMD0), (vref_sel_value[swap_idx]&1)==1?0:1, SHU_CA_CMD0_RG_TX_ARCLK_DRVP_PRE_BIT1);                 
                    }
                    else
                    {
                        if (swap_idx==0)
                        {
                            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DLL1) + ucdqs_i*DDRPHY_AO_B0_B1_OFFSET_0X80, 2, SHU_B0_DLL1_DATA_SWAP_B0);
                        }
                        else
                        {
                            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DLL1) + ucdqs_i*DDRPHY_AO_B0_B1_OFFSET_0X80, 3, SHU_B0_DLL1_DATA_SWAP_B0);
                        }

                        vref_sel_value[swap_idx]= 0;
                        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ5) + ucdqs_i*DDRPHY_AO_B0_B1_OFFSET_0X80, vref_sel_value[swap_idx]>>1, SHU_B0_DQ5_RG_RX_ARDQ_VREF_SEL_B0);
                        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ0) + ucdqs_i*DDRPHY_AO_B0_B1_OFFSET_0X80, (vref_sel_value[swap_idx]&1)==1?0:1, SHU_B0_DQ0_RG_TX_ARDQS0_DRVP_PRE_B0_BIT1);
                    }

                    for(i=5; i>=0; i--)
                    {
                        if (k_type == DutyScan_Calibration_K_CLK)
                        {
                            vref_sel_value[swap_idx] |= (1<<i);
                            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_CA_CMD5), vref_sel_value[swap_idx]>>1, SHU_CA_CMD5_RG_RX_ARCMD_VREF_SEL);        
                            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_CA_CMD0), (vref_sel_value[swap_idx]&1)==1?0:1, SHU_CA_CMD0_RG_TX_ARCLK_DRVP_PRE_BIT1);                                 

                            mcDELAY_US(1);

                            cal_out_value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_AD_RX_CMD_O1), MISC_AD_RX_CMD_O1_AD_RX_ARCA2_O1);

                            if (cal_out_value == 0)
                            {
                                vref_sel_value[swap_idx] &= ~(1<<i);
                            }
                        }
                        else
                        {
                                vref_sel_value[swap_idx] |= (1<<i);
                            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ5) + ucdqs_i*DDRPHY_AO_B0_B1_OFFSET_0X80, vref_sel_value[swap_idx]>>1, SHU_B0_DQ5_RG_RX_ARDQ_VREF_SEL_B0);
                            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ0) + ucdqs_i*DDRPHY_AO_B0_B1_OFFSET_0X80, (vref_sel_value[swap_idx]&1)==1?0:1, SHU_B0_DQ0_RG_TX_ARDQS0_DRVP_PRE_B0_BIT1);

                                mcDELAY_US(1);

                            if (ucdqs_i==0)
                            {
                                cal_out_value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_AD_RX_DQ_O1), MISC_AD_RX_DQ_O1_AD_RX_ARDQ_O1_B0_BIT2);
                            }
                            else
                            {
                                cal_out_value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_AD_RX_DQ_O1), MISC_AD_RX_DQ_O1_AD_RX_ARDQ_O1_B1_BIT2);
                            }

#if DutyPrintAllLog
                            mcSHOW_DBG_MSG(("Fra i=%d vref_sel_value[swap_idx]=%x, cal_out=%d\n",i, vref_sel_value[swap_idx], cal_out_value));
#endif

                            if (cal_out_value == 0)
                            {
                                vref_sel_value[swap_idx] &= ~(1<<i);
                            }
                        }
                    }
                }


                for(swap_idx=0; swap_idx<2; swap_idx++)
                {
                    if (vref_sel_value[swap_idx]<=31)
                    {
                        duty_value[swap_idx] = ((vref_sel_value[swap_idx]-21)*726)/10+5017;
                    }
                    else
                    {
                        duty_value[swap_idx] = ((vref_sel_value[swap_idx]-32)*475)/10+5786;
                    }                   
                }

#if DutyPrintAllLog
                mcSHOW_ERR_MSG(("\t[%d][%d] B%d : Vref_Sel=0x%x, Swap Vref_Sel=0x%x\n", scinner_duty_ofst, s4PICnt, ucdqs_i, vref_sel_value[0], vref_sel_value[1]));
                mcSHOW_ERR_MSG(("\t[%d][%d] B%d : duty_value=%d, Swap duty_value=%d\n", scinner_duty_ofst, s4PICnt, ucdqs_i, duty_value[0], duty_value[1]));
#endif                

                final_duty = 5000+((duty_value[0]-duty_value[1])/2);

                if (final_duty > (S32) ucperiod_duty_max)
                {
                    ucperiod_duty_max = final_duty;
                    ucperiod_duty_max_clk_dly = s4PICnt;
                }
                if (final_duty < (S32) ucperiod_duty_min)
                {
                    ucperiod_duty_min = final_duty;
                    ucperiod_duty_min_clk_dly = s4PICnt;
                }

#if DutyPrintAllLog
                mcSHOW_ERR_MSG(("\t[%d][%d] B%d : Final_Duty=%d\n", scinner_duty_ofst, s4PICnt, ucdqs_i, final_duty));
#endif
            }



            ucperiod_duty_averige = (ucperiod_duty_max + ucperiod_duty_min)>>1;

#if FT_DSIM_USED
            if (ft_test_type==1)
            {
                if (scinner_duty_ofst==DUTY_OFFSET_START)
                {
                    s4FT_Test_Final_duty[ucdqs_i][0] = ucperiod_duty_averige;
                }
                else 
                if (scinner_duty_ofst==0)
                {
                    s4FT_Test_Final_duty[ucdqs_i][1] = ucperiod_duty_averige;
                }
                else
                if (scinner_duty_ofst==DUTY_OFFSET_END)
                {
                    s4FT_Test_Final_duty[ucdqs_i][2] = ucperiod_duty_averige;
                }                        
            }
#endif

#if DutyPrintCalibrationLog
            if (k_type==DutyScan_Calibration_K_CLK)
            {
                mcSHOW_ERR_MSG(("[%d] CLK\n",scinner_duty_ofst));
                /*TINFO="[%d] CLK\n",scinner_duty_ofst */
            }
            else
            {
                mcSHOW_ERR_MSG(("[%d] %s%d\n",scinner_duty_ofst, str_who_am_I, ucdqs_i));
                /*TINFO="[%d] %s%d\n",scinner_duty_ofst, str_who_am_I, ucdqs_i */
            }
#endif            

#if DutyPrintCalibrationLog
            mcSHOW_ERR_MSG(("\tMAX Duty = %d%%(X100), CLK PI=%d\n",ucperiod_duty_max, ucperiod_duty_max_clk_dly));
            /*TINFO="\tMAX Duty = %d%%(X100), CLK PI=%d\n",ucperiod_duty_max, ucperiod_duty_max_clk_dly */
            mcSHOW_ERR_MSG(("\tMIN Duty = %d%%(X100), CLK PI=%d\n",ucperiod_duty_min, ucperiod_duty_min_clk_dly));
            /*TINFO="\tMIN Duty = %d%%(X100), CLK PI=%d\n",ucperiod_duty_min, ucperiod_duty_min_clk_dly */
            mcSHOW_ERR_MSG(("\tAVG Duty = %d%%(X100)\n", ucperiod_duty_averige));
            /*TINFO="\tAVG Duty = %d%%(X100)\n", ucperiod_duty_averige */
#endif            

            if (ucperiod_duty_averige >= ClockDutyMiddleBound)
            {
                if ((ucperiod_duty_averige-ClockDutyMiddleBound) <= ucmost_approach_50_percent)
                {
                    ucmost_approach_50_percent = ucperiod_duty_averige-ClockDutyMiddleBound;
                    scFinal_clk_delay_cell[ucdqs_i] = scinner_duty_ofst;
                    ucFinal_period_duty_averige[ucdqs_i] = ucperiod_duty_averige;
                    ucFinal_period_duty_max[ucdqs_i] = ucperiod_duty_max;
                    ucFinal_period_duty_min[ucdqs_i] = ucperiod_duty_min;
                    ucFinal_duty_max_clk_dly[ucdqs_i] = ucperiod_duty_max_clk_dly;
                    ucFinal_duty_min_clk_dly[ucdqs_i] = ucperiod_duty_min_clk_dly;
#if DutyPrintCalibrationLog                    
                    mcSHOW_ERR_MSG(("\t!!! ucmost_approach_50_percent = %d%%(X100) !!!\n",ucmost_approach_50_percent));
                    /*TINFO="!!! ucmost_approach_50_percent = %d%%(X100) !!!\n",ucmost_approach_50_percent */
#endif                    
                    early_break_count = 0;
                }
                else
                {
                    early_break_count ++;
                    if (early_break_count>=2) break; //early break;
                }
            }
            else
            {
                if ((ClockDutyMiddleBound-ucperiod_duty_averige) <= ucmost_approach_50_percent)
                {
                    ucmost_approach_50_percent = ClockDutyMiddleBound-ucperiod_duty_averige;
                    scFinal_clk_delay_cell[ucdqs_i] = scinner_duty_ofst;
                    ucFinal_period_duty_averige[ucdqs_i] = ucperiod_duty_averige;
                    ucFinal_period_duty_max[ucdqs_i] = ucperiod_duty_max;
                    ucFinal_period_duty_min[ucdqs_i] = ucperiod_duty_min;
                    ucFinal_duty_max_clk_dly[ucdqs_i] = ucperiod_duty_max_clk_dly;
                    ucFinal_duty_min_clk_dly[ucdqs_i] = ucperiod_duty_min_clk_dly;
#if DutyPrintCalibrationLog                    
                    mcSHOW_ERR_MSG(("\t!!! ucmost_approach_50_percent = %d%%(X100) !!!\n",ucmost_approach_50_percent));
                    /*TINFO="!!! ucmost_approach_50_percent = %d%%(X100) !!!\n",ucmost_approach_50_percent */
#endif                    
                    early_break_count = 0;
                }
                else
                {
                    early_break_count ++;
#if DutyPrintAllLog==0
                    if (early_break_count>=2) break; //early break;
#endif                    
                }
            }

#if DutyPrintCalibrationLog 
            mcSHOW_ERR_MSG(("\n"));
            /*TINFO="\n" */
#endif            
        }
    }

    for(ucdqs_i=0; ucdqs_i<ucdqs_i_count; ucdqs_i++)
    {
        //for SLT, use ERR_MSG to force print log
        if (k_type == DutyScan_Calibration_K_CLK)
        {
            mcSHOW_ERR_MSG(("\n==%s ==\n", str_who_am_I, ucdqs_i));
            /*TINFO="\n==%s ==\n", str_who_am_I */
        }
        else
        {
            mcSHOW_ERR_MSG(("\n==%s %d ==\n", str_who_am_I, ucdqs_i));
            /*TINFO="\n==%s %d ==\n", str_who_am_I, ucdqs_i */
        }
        mcSHOW_ERR_MSG(("Final %s duty delay cell = %d\n", str_who_am_I, scFinal_clk_delay_cell[ucdqs_i]));
        /*TINFO="Final %s duty delay cell = %d\n", str_who_am_I, scFinal_clk_delay_cell[ucdqs_i] */
        mcSHOW_ERR_MSG(("[%d] MAX Duty = %d%%(X100), DQS PI = %d\n",scFinal_clk_delay_cell[ucdqs_i], ucFinal_period_duty_max[ucdqs_i], ucFinal_duty_max_clk_dly[ucdqs_i]));
        /*TINFO="[%d] MAX Duty = %d%%(X100), DQS PI = %d\n",scFinal_clk_delay_cell[ucdqs_i], ucFinal_period_duty_max[ucdqs_i], ucFinal_duty_max_clk_dly[ucdqs_i] */
        mcSHOW_ERR_MSG(("[%d] MIN Duty = %d%%(X100), DQS PI = %d\n",scFinal_clk_delay_cell[ucdqs_i], ucFinal_period_duty_min[ucdqs_i], ucFinal_duty_min_clk_dly[ucdqs_i]));
        /*TINFO="[%d] MIN Duty = %d%%(X100), DQS PI = %d\n",scFinal_clk_delay_cell[ucdqs_i], ucFinal_period_duty_min[ucdqs_i], ucFinal_duty_min_clk_dly[ucdqs_i] */
        mcSHOW_ERR_MSG(("[%d] AVG Duty = %d%%(X100)\n", scFinal_clk_delay_cell[ucdqs_i], ucFinal_period_duty_averige[ucdqs_i]));
        /*TINFO="[%d] AVG Duty = %d%%(X100)\n", scFinal_clk_delay_cell[ucdqs_i], ucFinal_period_duty_averige[ucdqs_i] */
    }

#if FT_DSIM_USED
    if (ft_test_type==0)
    {
        FT_Duty_Compare_PassFail(p->channel, k_type, ucFinal_period_duty_max[0] , ucFinal_period_duty_min[0],ucFinal_period_duty_max[1] , ucFinal_period_duty_min[1]);
    }
    else
    {
        FT_Duty_Ofst_Diff_PassFail(p->channel, k_type, s4FT_Test_Final_duty[0][0], s4FT_Test_Final_duty[0][1], s4FT_Test_Final_duty[0][2], s4FT_Test_Final_duty[1][0], s4FT_Test_Final_duty[1][1], s4FT_Test_Final_duty[1][2]);
    }
    *GPIO_GPIO_DOUT0  |= 0x04000000; //PAD_PWM_0,      // Lafi_te / round done
#else
    for(ucdqs_i=0; ucdqs_i<ucdqs_i_count; ucdqs_i++)
    {
        u4DutyDiff = ucFinal_period_duty_max[ucdqs_i] - ucFinal_period_duty_min[ucdqs_i];

#if DQS_DUTY_SLT_CONDITION_TEST
        if (k_type == DutyScan_Calibration_K_CLK || (k_type == DutyScan_Calibration_K_DQS))
        {
            u4DQSDutyDiff_Rec[p->channel][ucdqs_i][u1GlobalTestCnt]=u4DutyDiff;

            u4DQSDutyDutyDly[p->channel][ucdqs_i] = scFinal_clk_delay_cell[ucdqs_i];

            if(u4DutyDiff > u4DQSDutyDiff_Max[p->channel][ucdqs_i])
                u4DQSDutyDiff_Max[p->channel][ucdqs_i] = u4DutyDiff;

            if(u4DutyDiff < u4DQSDutyDiff_Min[p->channel][ucdqs_i])
                u4DQSDutyDiff_Min[p->channel][ucdqs_i] = u4DutyDiff;

            u4DQSDutyDiff_Avrg[p->channel][ucdqs_i]  += u4DutyDiff;
        }
#endif

#if defined(YH_SWEEP_IC)
        gYH_Sweep_IC_test_result[k_type][p->channel][ucdqs_i].maxduty = ucFinal_period_duty_max[ucdqs_i];
        gYH_Sweep_IC_test_result[k_type][p->channel][ucdqs_i].minduty = ucFinal_period_duty_min[ucdqs_i];
        gYH_Sweep_IC_test_result[k_type][p->channel][ucdqs_i].dutydiff = u4DutyDiff;
        gYH_Sweep_IC_test_result[k_type][p->channel][ucdqs_i].avgduty = ucFinal_period_duty_averige[ucdqs_i];
#else
        if ((((k_type == DutyScan_Calibration_K_CLK) || (k_type == DutyScan_Calibration_K_DQS)) && (u4DutyDiff < u4DutyDiff_Limit)) ||
#if APPLY_DQDQM_DUTY_CALIBRATION            
           (((k_type == DutyScan_Calibration_K_DQ) || (k_type == DutyScan_Calibration_K_DQM)) && (u4DutyDiff < u4DutyDiff_Limit)))
#else
           (((k_type == DutyScan_Calibration_K_DQ) || (k_type == DutyScan_Calibration_K_DQM)) && ((u4DutyDiff < u4DutyDiff_Limit) && (ucFinal_period_duty_averige[ucdqs_i] >= 4550 && ucFinal_period_duty_averige[ucdqs_i] <= 5450))))
#endif           
        {
            if (k_type == DutyScan_Calibration_K_CLK)
            {
                mcSHOW_ERR_MSG(("\nCH%d %s Duty spec in!! Max-Min= %d%%\n",p->channel, str_who_am_I, u4DutyDiff));
                /*TINFO="\nCH%d %s Duty spec in!! Max-Min= %d%%\n",p->channel, str_who_am_I, u4DutyDiff */
            }
            else
            {
                mcSHOW_ERR_MSG(("\nCH%d %s %d Duty spec in!! Max-Min= %d%%\n",p->channel, str_who_am_I, ucdqs_i, u4DutyDiff));
                /*TINFO="\nCH%d %s %d Duty spec in!! Max-Min= %d%%\n",p->channel, str_who_am_I, ucdqs_i, u4DutyDiff */
            }
        }
        else
        {
            if (k_type == DutyScan_Calibration_K_CLK)
            {
                mcSHOW_ERR_MSG(("\nCH%d %s Duty spec out!! Max-Min= %d%% >%d%%\n", p->channel, str_who_am_I, u4DutyDiff, u4DutyDiff_Limit));
                /*TINFO="\nCH%d %s Duty spec out!! Max-Min= %d%% >8%%\n", p->channel, str_who_am_I, u4DutyDiff */
            }
            else
            {
                mcSHOW_ERR_MSG(("\nCH%d %s %d Duty spec out!! Max-Min= %d%% >%d%%\n", p->channel, str_who_am_I, ucdqs_i, u4DutyDiff, u4DutyDiff_Limit));
                /*TINFO="\nCH%d %s %d Duty spec out!! Max-Min= %d%% >8%%\n", p->channel, str_who_am_I, ucdqs_i, u4DutyDiff */
            }

            #if defined(SLT)
                    while(1); //stop here
            #endif

            #if __ETT__

                #if DQS_DUTY_SLT_CONDITION_TEST
                retStatus = DRAM_FAIL;
                #else
                    #if ONLY_K_DUTY_NO_SET_VALUE==0
                    if (gETT_WHILE_1_flag)
                    {
                        while(1); //stop here
                    }
                #endif
                #endif

            #endif
        }
#endif
    }

#endif

#if ONLY_K_DUTY_NO_SET_VALUE
    scFinal_clk_delay_cell[0] = 0;
    scFinal_clk_delay_cell[1] = 0;
#endif

    if (k_type == DutyScan_Calibration_K_DQS)
    {
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(p->femmc_Ready==0)
        {
            p->pSavetimeData->s1DQSDuty_clk_delay_cell[p->channel][0] = scFinal_clk_delay_cell[0];
            p->pSavetimeData->s1DQSDuty_clk_delay_cell[p->channel][1] = scFinal_clk_delay_cell[1];
        }
#endif

        // backup final values
        gcFinal_K_DQS_delay_cell[p->channel][0] = scFinal_clk_delay_cell[0];
        gcFinal_K_DQS_delay_cell[p->channel][1] = scFinal_clk_delay_cell[1];      

        DQSDutyScan_SetDqsDelayCell(p, scFinal_clk_delay_cell);

#ifdef FOR_HQA_TEST_USED
        gFinalDQSDuty[p->channel][0] = scFinal_clk_delay_cell[0];
        gFinalDQSDuty[p->channel][1] = scFinal_clk_delay_cell[1];
        gFinalDQSDutyMinMax[p->channel][0][0] = ucFinal_period_duty_min[0];
        gFinalDQSDutyMinMax[p->channel][0][1] = ucFinal_period_duty_max[0];
        gFinalDQSDutyMinMax[p->channel][1][0] = ucFinal_period_duty_min[1];
        gFinalDQSDutyMinMax[p->channel][1][1] = ucFinal_period_duty_max[1];
#endif
    }

    if (k_type == DutyScan_Calibration_K_CLK)
    {
        DramcClockDutySetClkDelayCell(p, scFinal_clk_delay_cell);

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(p->femmc_Ready==0)
        {
            p->pSavetimeData->s1ClockDuty_clk_delay_cell[p->channel][RANK_0] = scFinal_clk_delay_cell[0];
            p->pSavetimeData->s1ClockDuty_clk_delay_cell[p->channel][RANK_1] = scFinal_clk_delay_cell[0];
        }
#endif

        // backup final values
        gcFinal_K_CLK_delay_cell[p->channel][0] = scFinal_clk_delay_cell[0];
        gcFinal_K_CLK_delay_cell[p->channel][1] = scFinal_clk_delay_cell[1];

#ifdef FOR_HQA_TEST_USED
        gFinalClkDuty[p->channel] = scFinal_clk_delay_cell[0];
        gFinalClkDutyMinMax[p->channel][0] = ucFinal_period_duty_min[0];
        gFinalClkDutyMinMax[p->channel][1] = ucFinal_period_duty_max[0];
#endif
    }

#if APPLY_DQDQM_DUTY_CALIBRATION
    if (k_type == DutyScan_Calibration_K_DQ)
    {
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(p->femmc_Ready==0)
        {
            p->pSavetimeData->s1DQDuty_clk_delay_cell[p->channel][0] = scFinal_clk_delay_cell[0];
            p->pSavetimeData->s1DQDuty_clk_delay_cell[p->channel][1] = scFinal_clk_delay_cell[1];        
        }
#endif 

        // backup final values
        gcFinal_K_DQ_delay_cell[p->channel][0] = scFinal_clk_delay_cell[0];
        gcFinal_K_DQ_delay_cell[p->channel][1] = scFinal_clk_delay_cell[1];
        
        DQDQMDutyScan_SetDQDQMDelayCell(p, p->channel, scFinal_clk_delay_cell, DutyScan_Calibration_K_DQ);          
    }

    if (k_type == DutyScan_Calibration_K_DQM)
    {
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(p->femmc_Ready==0)
        {
            p->pSavetimeData->s1DQMDuty_clk_delay_cell[p->channel][0] = scFinal_clk_delay_cell[0];
            p->pSavetimeData->s1DQMDuty_clk_delay_cell[p->channel][1] = scFinal_clk_delay_cell[1];        
        }
#endif 

        // backup final values
        gcFinal_K_DQM_delay_cell[p->channel][0] = scFinal_clk_delay_cell[0];
        gcFinal_K_DQM_delay_cell[p->channel][1] = scFinal_clk_delay_cell[1];
        
        DQDQMDutyScan_SetDQDQMDelayCell(p, p->channel, scFinal_clk_delay_cell, DutyScan_Calibration_K_DQM);
    }    
#endif

    DramPhyReset(p);

    mcSHOW_ERR_MSG(("[DutyScan_Calibration_Flow] ====Done====\n"));
    /*TINFO="[DutyScan_Calibration_Flow] ====Done====\n" */

    return DRAM_OK;
}

void DramcNewDutyCalibration(DRAMC_CTX_T *p)
{
    U8 u1ChannelIdx, u1backup_channel;

#if(DQS_DUTY_SLT_CONDITION_TEST)
        U16 u2TestCnt, u2FailCnt=0, u2TestCntTotal =20; //fra 400;
        U8 u1ByteIdx, u1PI_FB;
        U32 u4Variance;
#endif

    #if APOLLO_SPECIFIC
    DRAM_STATUS_T u2FailStatusByCh[2]={DRAM_OK,DRAM_OK};
    #else
    DRAM_STATUS_T u2FailStatusByCh[CHANNEL_NUM]={DRAM_OK,DRAM_OK};
    #endif

    //backup register value
#if FT_DSIM_USED==0
    U32 u4RegBackupAddress[] =
    {
        (DDRPHY_B0_DQ6),
        (DDRPHY_SHU_B0_DLL1),
        (DDRPHY_SHU_B0_DQ5),
        (DDRPHY_SHU_B0_DQ0),
        (DDRPHY_B0_DQ0),
        (DDRPHY_B0_DQ5),
        (DDRPHY_B0_DQ8),
        (DDRPHY_SHU_R0_B0_DQ7),
        (DDRPHY_B0_DLL_ARPI0),
        (DDRPHY_B0_DLL_ARPI2),

        (DDRPHY_B1_DQ6),
        (DDRPHY_SHU_B1_DLL1),
        (DDRPHY_SHU_B1_DQ5),
        (DDRPHY_SHU_B1_DQ0),
        (DDRPHY_B1_DQ0),
        (DDRPHY_B1_DQ5),
        (DDRPHY_B1_DQ8),
        (DDRPHY_SHU_R0_B1_DQ7),
        (DDRPHY_B1_DLL_ARPI0),
        (DDRPHY_B1_DLL_ARPI2),


        (DDRPHY_CA_CMD6),
        (DDRPHY_SHU_CA_DLL2),
        (DDRPHY_SHU_CA_CMD5),
        (DDRPHY_SHU_CA_CMD0),
        (DDRPHY_CA_CMD0),
        (DDRPHY_CA_CMD5),
        (DDRPHY_CA_CMD9),
//        (DDRPHY_SHU_CA_CMD3),
        (DDRPHY_SHU_R0_CA_CMD9),      
        (DDRPHY_CA_DLL_ARPI0),
        (DDRPHY_CA_DLL_ARPI2),



        (DDRPHY_B0_DQ6 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_SHU_B0_DLL1 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_SHU_B0_DQ5 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_SHU_B0_DQ0 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_B0_DQ0 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_B0_DQ5 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_B0_DQ8 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_SHU_R0_B0_DQ7 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_B0_DLL_ARPI0 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_B0_DLL_ARPI2 + SHIFT_TO_CHB_ADDR),

        (DDRPHY_B1_DQ6 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_SHU_B1_DLL1 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_SHU_B1_DQ5 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_SHU_B1_DQ0 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_B1_DQ0 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_B1_DQ5 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_B1_DQ8 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_SHU_R0_B1_DQ7 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_B1_DLL_ARPI0 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_B1_DLL_ARPI2 + SHIFT_TO_CHB_ADDR),


        (DDRPHY_CA_CMD6 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_SHU_CA_DLL2 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_SHU_CA_CMD5 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_SHU_CA_CMD0 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_CA_CMD0 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_CA_CMD5 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_CA_CMD9 + SHIFT_TO_CHB_ADDR),
//        (DDRPHY_SHU_CA_CMD3 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_SHU_R0_CA_CMD9 + SHIFT_TO_CHB_ADDR),              
        (DDRPHY_CA_DLL_ARPI0 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_CA_DLL_ARPI2 + SHIFT_TO_CHB_ADDR)
    };
#endif

#if !FT_DSIM_USED
#if DUTY_SCAN_V2_ONLY_K_HIGHEST_FREQ
    if((p->frequency == u2DFSGetHighestFreq(p)) && (Get_PRE_MIOCK_JMETER_HQA_USED_flag()==0))
#else
    if(Get_PRE_MIOCK_JMETER_HQA_USED_flag()==0)
#endif
#endif
    {
        U8 u1ChannelIdx;
        u1backup_channel = vGetPHY2ChannelMapping(p);

        #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(p->femmc_Ready==1)
        {
            for(u1ChannelIdx=CHANNEL_A; u1ChannelIdx<p->support_channel_num; u1ChannelIdx++)
            {
                vSetPHY2ChannelMapping(p, u1ChannelIdx);
                DramcClockDutySetClkDelayCell(p, p->pSavetimeData->s1ClockDuty_clk_delay_cell[p->channel]);
                DQSDutyScan_SetDqsDelayCell(p, p->pSavetimeData->s1DQSDuty_clk_delay_cell[p->channel]);
                #if APPLY_DQDQM_DUTY_CALIBRATION                    
                DQDQMDutyScan_SetDQDQMDelayCell(p, p->channel, p->pSavetimeData->s1DQMDuty_clk_delay_cell[p->channel], DutyScan_Calibration_K_DQM);
                DQDQMDutyScan_SetDQDQMDelayCell(p, p->channel, p->pSavetimeData->s1DQDuty_clk_delay_cell[p->channel], DutyScan_Calibration_K_DQ);                    
                #endif
            }
            vSetPHY2ChannelMapping(p, u1backup_channel);                
            return;
        }
        else
        #endif
        {  
#if !FT_DSIM_USED        
            if (vGet_Duty_Calibration_Mode(p) == DUTY_DEFAULT)
            {
                //clear global variables to 0 delay
                for(u1ChannelIdx=CHANNEL_A; u1ChannelIdx<p->support_channel_num; u1ChannelIdx++)
                {
                    gcFinal_K_CLK_delay_cell[u1ChannelIdx][0] = 0;
                    gcFinal_K_DQS_delay_cell[u1ChannelIdx][0] = 0;
                    gcFinal_K_DQS_delay_cell[u1ChannelIdx][1] = 0;
                    gcFinal_K_DQM_delay_cell[u1ChannelIdx][0] = 0;
                    gcFinal_K_DQM_delay_cell[u1ChannelIdx][1] = 0;
                    gcFinal_K_DQ_delay_cell[u1ChannelIdx][0] = 0;
                    gcFinal_K_DQ_delay_cell[u1ChannelIdx][1] = 0;                        
                }
            }
#endif            
                
#if !FT_DSIM_USED
            if ((vGet_Duty_Calibration_Mode(p) == DUTY_DEFAULT) || (vGet_Duty_Calibration_Mode(p) == DUTY_LAST_K))
            {
                for(u1ChannelIdx=CHANNEL_A; u1ChannelIdx<p->support_channel_num; u1ChannelIdx++)
                {
                    vSetPHY2ChannelMapping(p, u1ChannelIdx);  
                    DramcClockDutySetClkDelayCell(p, gcFinal_K_CLK_delay_cell[p->channel]);
                    DQSDutyScan_SetDqsDelayCell(p, gcFinal_K_DQS_delay_cell[p->channel]);

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
                    if(p->femmc_Ready==0)
                    {
                        p->pSavetimeData->s1ClockDuty_clk_delay_cell[p->channel][RANK_0] = gcFinal_K_CLK_delay_cell[p->channel][RANK_0];
                        p->pSavetimeData->s1ClockDuty_clk_delay_cell[p->channel][RANK_1] = gcFinal_K_CLK_delay_cell[p->channel][RANK_1];
                        p->pSavetimeData->s1DQSDuty_clk_delay_cell[p->channel][0] = gcFinal_K_DQS_delay_cell[p->channel][0];
                        p->pSavetimeData->s1DQSDuty_clk_delay_cell[p->channel][1] = gcFinal_K_DQS_delay_cell[p->channel][1];
                    }
#endif

                    #if APPLY_DQDQM_DUTY_CALIBRATION
                    DQDQMDutyScan_SetDQDQMDelayCell(p, p->channel, gcFinal_K_DQM_delay_cell[p->channel], DutyScan_Calibration_K_DQM);
                    DQDQMDutyScan_SetDQDQMDelayCell(p, p->channel, gcFinal_K_DQ_delay_cell[p->channel], DutyScan_Calibration_K_DQ);

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
                    if(p->femmc_Ready==0)
                    {
                        p->pSavetimeData->s1DQMDuty_clk_delay_cell[p->channel][0] = gcFinal_K_DQS_delay_cell[p->channel][0];
                        p->pSavetimeData->s1DQMDuty_clk_delay_cell[p->channel][1] = gcFinal_K_DQM_delay_cell[p->channel][1];
                        p->pSavetimeData->s1DQDuty_clk_delay_cell[p->channel][0] = gcFinal_K_DQ_delay_cell[p->channel][0];
                        p->pSavetimeData->s1DQDuty_clk_delay_cell[p->channel][1] = gcFinal_K_DQ_delay_cell[p->channel][1];
                    }
#endif                         
                    #endif
                }

                vSetPHY2ChannelMapping(p, u1backup_channel);
                return;
            }                    
#endif            
        
            //Clk free run
            EnableDramcPhyDCM(p, 0);

        //Fix rank to rank0
        vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0, RKCFG_TXRANK);
        vIO32WriteFldAlign(DRAMC_REG_RKCFG, 1, RKCFG_TXRANKFIX);

            for(u1ChannelIdx=CHANNEL_A; u1ChannelIdx<p->support_channel_num; u1ChannelIdx++)
            {
                vSetPHY2ChannelMapping(p, u1ChannelIdx);

                //backup register value
                #if FT_DSIM_USED==0
                DramcBackupRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));
                #endif

                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_CA_DLL2), 0, SHU_CA_DLL2_RG_VREF_UT_EN);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ0), 0, SHU_B0_DQ0_RG_TX_ARDQ_PRE_EN_B0);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B1_DQ0), 0, SHU_B1_DQ0_RG_TX_ARDQ_PRE_EN_B1);                    
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_CA_CMD0), 1, SHU_CA_CMD0_RG_TX_ARCLK_DRVP_PRE_BIT2);                                        

                DutyScan_Offset_Calibration(p);

                        #if defined(YH_SWEEP_IC)
                            if (p->channel == CHANNEL_B)
                            {
                                u2FailStatusByCh[u1ChannelIdx]= DutyScan_Calibration_Flow(p, DutyScan_Calibration_K_CLK, 1);
                            }
                        #else
                            u2FailStatusByCh[u1ChannelIdx]= DutyScan_Calibration_Flow(p, DutyScan_Calibration_K_CLK, 1);
                        #endif

                            u2FailStatusByCh[u1ChannelIdx]= DutyScan_Calibration_Flow(p, DutyScan_Calibration_K_DQS, 1);

                    #if defined(YH_SWEEP_IC) || APPLY_DQDQM_DUTY_CALIBRATION
                        u2FailStatusByCh[u1ChannelIdx]|= DutyScan_Calibration_Flow(p, DutyScan_Calibration_K_DQM, 1);

#if 0
#if APPLY_DQDQM_DUTY_CALIBRATION
                        //copy DQ RG to DQM
                        DQDQMDutyScan_CopyDQRG2DQMRG(p);
#endif
                    #endif
                            u2FailStatusByCh[u1ChannelIdx]|= DutyScan_Calibration_Flow(p, DutyScan_Calibration_K_DQ, 1);
                        #endif
                #if FT_DSIM_USED==0
                //restore to orignal value
                DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));
                #endif

                //Set K DQS MCK4X_DLY_EN and MCK4XB_DLY_EN again, this is especially for K DQS because other bit fields need to be restored.
                DQSDutyScan_SetDqsDelayCell(p, gcFinal_K_DQS_delay_cell[p->channel]);
            }
        }
        

        vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0, RKCFG_TXRANK);
        vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0, RKCFG_TXRANKFIX);

        vSetPHY2ChannelMapping(p, u1backup_channel);
        
#if !FT_DSIM_USED
#if defined(YH_SWEEP_IC)
        YH_Sweep_IC_Print_Result(p);
        while(1); //stop here
#endif
#endif
    }
}
#endif


#if ENABLE_WRITE_DBI
void DramcWriteMinus1MCKForWriteDBI(DRAMC_CTX_T *p, S8 iShiftUI)
{
    U8 ucdq_ui_large_dqs0, ucdq_ui_large_dqs1;
    U8 ucdq_final_dqm_ui_large_dqs0, ucdq_final_dqm_ui_large_dqs1;
    REG_TRANSFER_T TransferReg[2];
    
    if(p->DBI_W_onoff[p->dram_fsp])
    {
        ucdq_ui_large_dqs0 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0), SHURK0_SELPH_DQ0_TXDLY_DQ0);
        ucdq_ui_large_dqs1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0), SHURK0_SELPH_DQ0_TXDLY_DQ1);
        //mcSHOW_DBG_MSG(("Before -1MCK, ucdq_final_ui_large_dqs0 = %d, ucdq_final_ui_large_dqs1 = %d\n", ucdq_ui_large_dqs0, ucdq_ui_large_dqs1));
        // DQ0
        TransferReg[0].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ2;
        TransferReg[0].u4Fld =SHURK0_SELPH_DQ2_DLY_DQ0;
        TransferReg[1].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ0;
        TransferReg[1].u4Fld =SHURK0_SELPH_DQ0_TXDLY_DQ0;
        ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
        
        // DQ1
        TransferReg[0].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ2;
        TransferReg[0].u4Fld =SHURK0_SELPH_DQ2_DLY_DQ1;
        TransferReg[1].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ0;
        TransferReg[1].u4Fld =SHURK0_SELPH_DQ0_TXDLY_DQ1;
        ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
        
        ucdq_ui_large_dqs0 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0), SHURK0_SELPH_DQ0_TXDLY_DQ0);
        ucdq_ui_large_dqs1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0), SHURK0_SELPH_DQ0_TXDLY_DQ1);
        //mcSHOW_DBG_MSG(("After  -1MCK, ucdq_final_ui_large_dqs0 = %d, ucdq_final_ui_large_dqs1 = %d\n", ucdq_ui_large_dqs0, ucdq_ui_large_dqs1));
        
        
        ucdq_final_dqm_ui_large_dqs0 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ1), SHURK0_SELPH_DQ1_TXDLY_DQM0);
        ucdq_final_dqm_ui_large_dqs1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ1), SHURK0_SELPH_DQ1_TXDLY_DQM1);
        //mcSHOW_DBG_MSG(("Before -1MCK, ucdq_final_dqm_ui_large_dqs0 = %d, ucdq_final_dqm_ui_large_dqs1 = %d\n", ucdq_final_dqm_ui_large_dqs0, ucdq_final_dqm_ui_large_dqs1));
        // DQM0
        TransferReg[0].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ3;
        TransferReg[0].u4Fld =SHURK0_SELPH_DQ3_DLY_DQM0;
        TransferReg[1].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ1;
        TransferReg[1].u4Fld =SHURK0_SELPH_DQ1_TXDLY_DQM0;
        ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
        
        // DQM1
        TransferReg[0].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ3;
        TransferReg[0].u4Fld =SHURK0_SELPH_DQ3_DLY_DQM1;
        TransferReg[1].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ1;
        TransferReg[1].u4Fld =SHURK0_SELPH_DQ1_TXDLY_DQM1;
        ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
        ucdq_final_dqm_ui_large_dqs0 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ1), SHURK0_SELPH_DQ1_TXDLY_DQM0);
        ucdq_final_dqm_ui_large_dqs1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ1), SHURK0_SELPH_DQ1_TXDLY_DQM1);
        //mcSHOW_DBG_MSG(("After  -1MCK, ucdq_final_dqm_ui_large_dqs0 = %d, ucdq_final_dqm_ui_large_dqs1 = %d\n", ucdq_final_dqm_ui_large_dqs0, ucdq_final_dqm_ui_large_dqs1));
    }
}
#endif

