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
//#include "DramC_reg.h"
//#include "System_reg.h"

#if 1//REG_ACCESS_PORTING_DGB
U8 RegLogEnable=0;
#endif

#if REG_SHUFFLE_REG_CHECK
U8 ShuffleRegCheck =0;
#endif

//#define fcFOR_DQSGDUALP_ENABLE   // Dual-phase DQS clock gating control enabling (new gating, not define as MT6589)
#define CBT_SEL_MEDIAN 0

#define CATRAINING_NUM_LP3      10
#define CATRAINING_NUM_LP4      6
#define MAX_CA_PI_DELAY         63
#define MAX_CS_PI_DELAY         63
#define MAX_CLK_PI_DELAY        31


#define PASS_RANGE_NA   0x7fff



//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
static U8 fgwrlevel_done = 0;

U8 u1MR01Value[FSP_MAX];
U8 u1MR02Value[FSP_MAX];
U8 u1MR03Value[FSP_MAX];
U8 u1MR13Value[FSP_MAX];

U8 u1MR12Value[CHANNEL_NUM][RANK_MAX][FSP_MAX];
U8 u1MR14Value[CHANNEL_NUM][RANK_MAX][FSP_MAX];
U8 u1MR14Value_05VDDQ[CHANNEL_NUM][RANK_MAX][FSP_MAX];
U8 u1MR14Value_06VDDQ[CHANNEL_NUM][RANK_MAX][FSP_MAX];

U8 gCBT_VREF_RANGE_SEL;
U8 gCBT_VREF_RANGE_BEGIN;
U8 gCBT_VREF_RANGE_END;
U8 gCBT_VREF_RANGE_STEP;
#if SUPPORT_SAVE_OFF_LINE_CALIBRATION
U8 gCBT_Vref_Scan_Check[CHANNEL_NUM][RANK_MAX][FSP_MAX];
U8 gRX_Vref_Scan_Check[CHANNEL_NUM][RANK_MAX][FSP_MAX];
U8 gTX_Vref_Scan_Check[CHANNEL_NUM][RANK_MAX][FSP_MAX];
#endif

U8 gu1MR23Done = FALSE;
U8 gu1MR23[CHANNEL_NUM][RANK_MAX];
U8 gu1DQSOSCTHRD_INC[CHANNEL_NUM][RANK_MAX];
U8 gu1DQSOSCTHRD_DEC[CHANNEL_NUM][RANK_MAX];
U16 gu2MR18[CHANNEL_NUM][RANK_MAX];
U16 gu2MR19[CHANNEL_NUM][RANK_MAX];
U16 gu2DQSOSC[CHANNEL_NUM][RANK_MAX];
U16 gu2DQSOscCnt[CHANNEL_NUM][RANK_MAX][2];


#ifdef TX_EYE_SCAN
U16 aru2TXCaliDelay[CHANNEL_NUM][DQS_NUMBER];
U16 aru2TXCaliDelay_OEN[CHANNEL_NUM][DQS_NUMBER];
#endif

#define CBT_WORKAROUND_O1_SWAP 0 // set default clk and ca value
#define HW_WORKAROUND_RX_EYE_LP3_BYTE3_DELAY_READY 0

static S32 CATrain_CmdDelay[CHANNEL_NUM][RANK_MAX];
static U32 CATrain_CsDelay[CHANNEL_NUM][RANK_MAX];
static S32 CATrain_ClkDelay[CHANNEL_NUM][RANK_MAX];
static S32 wrlevel_dqs_final_delay[CHANNEL_NUM][DQS_NUMBER]; // 3 is channel number
static U16 u2rx_window_sum;
static U8 ucg_num_dlycell_perT = 49; //from 60807
U8 ucg_num_dlycell_perT_all[DRAM_DFS_SHUFFLE_MAX][CHANNEL_NUM];
U16 u2gdelay_cell_ps;
U16 u2gdelay_cell_ps_all[DRAM_DFS_SHUFFLE_MAX][CHANNEL_NUM];
U32 u4gVcore[DRAM_DFS_SHUFFLE_MAX];

U32 gDramcSwImpedanceResule[2][4];

S16 gu2RX_DQS_Duty_Offset[DQS_NUMBER][2];

#ifdef FOR_HQA_TEST_USED
U16 gFinalCBTVrefCA[CHANNEL_NUM][RANK_MAX];
U16 gFinalCBTCA[CHANNEL_NUM][RANK_MAX][10];
U16 gFinalGatingWin[CHANNEL_NUM][RANK_MAX][4];
U16 gFinalRXPerbitWin[CHANNEL_NUM][RANK_MAX][DQ_DATA_WIDTH];
U16 gFinalTXVrefDQ[CHANNEL_NUM][RANK_MAX];
U16 gFinalTXPerbitWin[CHANNEL_NUM][RANK_MAX][DQ_DATA_WIDTH];
U16 gFinalTXPerbitWin_min_max[CHANNEL_NUM][RANK_MAX];
#if(TX_PER_BIT_DELAY_CELL==0)
U16 gFinalTXPerbitWin_min_margin[CHANNEL_NUM][RANK_MAX];
U16 gFinalTXPerbitWin_min_margin_bit[CHANNEL_NUM][RANK_MAX];
#endif
#endif
U16 gFinalRXVrefDQ[CHANNEL_NUM];

#ifdef DRAM_CALIB_LOG
U32 gDRAM_CALIB_LOG_pointer=0;
SAVE_TO_SRAM_FORMAT_T   gDRAM_CALIB_LOG __attribute__ ((aligned(4)));
#endif
extern const U8 uiLPDDR4_MRR_Mapping_POP_TX[CHANNEL_NUM][16];

static void dle_factor_handler(DRAMC_CTX_T *p, U8 curr_val, U8 pip_num);
U8 GetCmdBusTrainingVrefPinMuxRevertValue(DRAMC_CTX_T *p, U8 u1VrefLevel);


void vInitGlobalVariablesByCondition(void)
{
    U8 u1CHIdx, u1RankIdx, u1FSPIdx;

    u1MR01Value[FSP_0] = 0x26;
    u1MR01Value[FSP_1] = 0x56;
    
    memset(u1MR12Value, 0x4d, sizeof(u1MR12Value));
    
    u1MR13Value[FSP_0] = 0 | (MR13_RRO<<4) | (1<<3);
    u1MR13Value[FSP_1] = 0xc0 | (MR13_RRO<<4) | (1<<3);
    
    for(u1FSPIdx=0; u1FSPIdx<FSP_MAX; u1FSPIdx++)
    {
        u1MR02Value[u1FSPIdx]=0x1a;
        u1MR03Value[u1FSPIdx]=0x31;
    }
    
    for(u1CHIdx=0; u1CHIdx<CHANNEL_NUM; u1CHIdx++)
        for(u1RankIdx=0; u1RankIdx< RANK_MAX; u1RankIdx++)
            for(u1FSPIdx=0; u1FSPIdx<FSP_MAX; u1FSPIdx++)
            {
                // MR14 default value
                u1MR14Value[u1CHIdx][u1RankIdx][u1FSPIdx] = (u1FSPIdx==FSP_0) ? 0x4d : 0x10;
                u1MR14Value_05VDDQ[u1CHIdx][u1RankIdx][u1FSPIdx] = (u1FSPIdx==FSP_0) ? 0x5a : 0x0c;
                u1MR14Value_06VDDQ[u1CHIdx][u1RankIdx][u1FSPIdx] = (u1FSPIdx==FSP_0) ? 0x4d : 0x1e;    
            }

    memset(gu1MR23, 0x30, sizeof(gu1MR23));
    memset(gu1DQSOSCTHRD_INC, 0x6, sizeof(gu1DQSOSCTHRD_INC));
    memset(gu1DQSOSCTHRD_DEC, 0x4, sizeof(gu1DQSOSCTHRD_DEC));
            
#if SUPPORT_SAVE_OFF_LINE_CALIBRATION
    memset(gCBT_Vref_Scan_Check, 0, sizeof(gCBT_Vref_Scan_Check));
    memset(gRX_Vref_Scan_Check, 0, sizeof(gRX_Vref_Scan_Check));
    memset(gTX_Vref_Scan_Check, 0, sizeof(gTX_Vref_Scan_Check));
#endif

    memset(gu2RX_DQS_Duty_Offset, 0, sizeof(gu2RX_DQS_Duty_Offset));

}

void vSetChannelNumber(DRAMC_CTX_T *p)
{
    if(u1IsLP4Family(p->dram_type))
    {
        p->support_channel_num = CHANNEL_NUM; // 4 channel
    }
#if ENABLE_LP3_SW
    else //LP3
        p->support_channel_num = CHANNEL_SINGLE;
#endif /* ENABLE_LP3_SW */
}

void vSetRankNumber(DRAMC_CTX_T *p)
{
#if 1
    if (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RSTMASK), RSTMASK_RSV_DRAM_SUPPORT_RANK_NUM) == 0)
    {
        p->support_rank_num =RANK_DUAL;
    }
    else
    {
        p->support_rank_num =RANK_SINGLE;
    }
#else
    #if DUAL_RANK_ENABLE
    p->support_rank_num =RANK_DUAL;
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RSTMASK), 0, RSTMASK_RSV_DRAM_SUPPORT_RANK_NUM);
    #else
    p->support_rank_num =RANK_SINGLE;
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RSTMASK), 1, RSTMASK_RSV_DRAM_SUPPORT_RANK_NUM);
    #endif
#endif
}

void vSetPHY2ChannelMapping(DRAMC_CTX_T *p, U8 u1Channel)
{
    p->channel =u1Channel;
}


U8 vGetPHY2ChannelMapping(DRAMC_CTX_T *p)
{
    return p->channel;
}

void vSetRank(DRAMC_CTX_T *p, U8 ucRank)
{
    p->rank = ucRank;
}

U8 u1GetRank(DRAMC_CTX_T *p)
{
    return p->rank;
}

/* vGet_Dram_CBT_Mode
 * Due to current HW design (both ranks share the same set of ACTiming regs), mixed
 * mode LP4 now uses byte mode ACTiming settings. This means most calibration steps
 * should use byte mode code flow.
 * Note: The below items must have per-rank settings (Don't use this function)
 * 1. CBT training 2. TX tracking
 */
DRAM_CBT_MODE_T vGet_Dram_CBT_Mode(DRAMC_CTX_T *p)
{
    if(p->dram_cbt_mode[RANK_0] == CBT_NORMAL_MODE && p->dram_cbt_mode[RANK_1] == CBT_NORMAL_MODE)
    {
        return CBT_NORMAL_MODE;
    }
    else // For Mixed mode & Byte mode LP4
    {
        return CBT_BYTE_MODE1;
    }
}

#if !SW_CHANGE_FOR_SIMULATION
void vSetCalibrationResult(DRAMC_CTX_T *p, U8 ucCalType, U8 ucResult)
{
    p->aru4CalExecuteFlag[p->channel][p->rank][p->shu_type] |= (1<<ucCalType); // ececution done
    if (ucResult == DRAM_OK)  // Calibration OK
    {
        p->aru4CalResultFlag[p->channel][p->rank][p->shu_type] &= (~(1<<ucCalType));
    }
    else  //Calibration fail
    {
        p->aru4CalResultFlag[p->channel][p->rank][p->shu_type] |= (1<<ucCalType);
    }
}

void vGetCalibrationResult_All(DRAMC_CTX_T *p, U8 u1Channel, U8 u1Rank, U8 u1FreqType, U32 *u4CalExecute, U32 *u4CalResult)
{
    *u4CalExecute = p->aru4CalExecuteFlag[u1Channel][u1Rank][u1FreqType];
    *u4CalResult = p->aru4CalResultFlag[u1Channel][u1Rank][u1FreqType];
}

#if 0  //no use now, disable for saving code size.
void vGetCalibrationResult(DRAMC_CTX_T *p, U8 ucCalType, U8 *ucCalExecute, U8 *ucCalResult)
{
    U32 ucCalResult_All, ucCalExecute_All;

    ucCalExecute_All = p->aru4CalExecuteFlag[p->channel][p->rank][p->shu_type];
    ucCalResult_All = p->aru4CalResultFlag[p->channel][p->rank][p->shu_type];

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

#if __ETT__ //remove unused function to save space of log string
void vPrintCalibrationResult(DRAMC_CTX_T *p)
{
    U8 ucCHIdx, ucRankIdx, ucFreqIdx, ucCalIdx;
    U32 ucCalResult_All, ucCalExecute_All;
    U8 ucCalResult, ucCalExecute;

    for(ucCHIdx=0; ucCHIdx<p->support_channel_num; ucCHIdx++)
    {
        for(ucRankIdx=0; ucRankIdx<RANK_MAX; ucRankIdx++)
        {
            for(ucFreqIdx=0; ucFreqIdx<DRAM_DFS_SHUFFLE_MAX; ucFreqIdx++)
            {
                ucCalExecute_All = p->aru4CalExecuteFlag[ucCHIdx][ucRankIdx][ucFreqIdx];
                ucCalResult_All = p->aru4CalResultFlag[ucCHIdx][ucRankIdx][ucFreqIdx];
                mcSHOW_DBG_MSG(("[vPrintCalibrationResult] Channel = %d, Rank= %d, Freq.= %d, (ucCalExecute_All 0x%x, ucCalResult_All 0x%x)\n", ucCHIdx, ucRankIdx, ucFreqIdx, ucCalExecute_All, ucCalResult_All));

                for(ucCalIdx =0; ucCalIdx<DRAM_CALIBRATION_MAX; ucCalIdx++)
                {
                    ucCalExecute = (U8)((ucCalExecute_All >>ucCalIdx) & 0x1);
                    ucCalResult =  (U8)((ucCalResult_All >>ucCalIdx) & 0x1);
                    #ifdef ETT_PRINT_FORMAT
                    mcSHOW_DBG_MSG(("\t%s: Execute= %d, Result= %s\n", szCalibStatusName[ucCalIdx], ucCalExecute, ((ucCalResult == 0) ? "OK" : "Fail")));
                    #else
                    mcSHOW_DBG_MSG(("\t%20s: Execute= %d, Result= %s\n", szCalibStatusName[ucCalIdx], ucCalExecute, ((ucCalResult == 0) ? "OK" : "Fail")));
                    #endif
                }
            }
        }
    }
}
#endif
#endif

void vPrintCalibrationBasicInfo(DRAMC_CTX_T *p)
{
    /* TODO: Read/Write DBI status doesn't affect both DramC & dram DBI status
     *       Status will be incorrect since a certain rank's DBI may be enabled after K
     */
    mcSHOW_DBG_MSG(("===============================================================================\n"
                    "Dram Type= %d, Freq= %u, FreqGroup= %u, CH_%d, rank %d\n"
                    "fsp= %d, odt_onoff= %d, Byte mode= %d, Read DBI= %u, Write DBI= %u\n"
                    "===============================================================================\n",
                        p->dram_type, p->frequency, p->freqGroup, p->channel, p->rank,
                        p->dram_fsp, p->odt_onoff, p->dram_cbt_mode[p->rank], ReadDbiStat(p), WriteDbiStat(p)));
}

#if VENDER_JV_LOG
void vPrintCalibrationBasicInfo_ForJV(DRAMC_CTX_T *p)
{
    mcSHOW_DBG_MSG5(("\n\nDram type:"));

    switch(p->dram_type)
    {
#if ENABLE_LP3_SW
        case TYPE_LPDDR3:
            mcSHOW_DBG_MSG5(("LPDDR3\t"));
            break;
#endif /* ENABLE_LP3_SW */
        case TYPE_LPDDR4:
            mcSHOW_DBG_MSG5(("LPDDR4\t"));
            break;

        case TYPE_LPDDR4X:
            mcSHOW_DBG_MSG5(("LPDDR4X\t"));
            break;

        case TYPE_LPDDR4P:
            mcSHOW_DBG_MSG5(("LPDDR4P\t"));
            break;
    }

    /* TODO: Read/Write DBI status doesn't affect both DramC & dram DBI status
     *       Status will be incorrect since a certain rank's DBI may be enabled after K
     */
    mcSHOW_DBG_MSG5(("Freq: %d, FreqGroup %u, channel %d, rank %d\n"
                     "dram_fsp= %d, odt_onoff= %d, Byte mode= %d, Read DBI= %u, Write DBI= %u, UI= %d(ps)\n\n",
                                        p->frequency, p->freqGroup, p->channel, p->rank,
                                        p->dram_fsp, p->odt_onoff, p->dram_cbt_mode[p->rank], ReadDbiStat(p), WriteDbiStat(p), 1000000/p->frequency/64));

    return;
}
#endif

#if (__ETT__ || ENABLE_LP3_SW)
//for LP3 read register in different byte
U32 u4IO32ReadFldAlign_Phy_Byte(U8 ucByteIdx, U32 reg32, U32 fld)
{
       U32 offset=0;
       U32 except_offset=0;

    if(reg32<Channel_A_PHY_AO_BASE_VIRTUAL)
    {
       mcSHOW_DBG_MSG(("\n[ReadFldAlign_Phy_Byte] wrong addr 0x%x\n", reg32));
       return;
    }

    reg32 &= 0xffff;

    if (reg32 >= (DDRPHY_SHU1_R0_B0_DQ0-DDRPHY_AO_BASE_ADDR) && reg32 < (DDRPHY_SHU1_R0_B1_DQ0-DDRPHY_AO_BASE_ADDR))
    {
        offset = 0x50;
        if (reg32 >= (DDRPHY_SHU1_R0_B0_DQ0-DDRPHY_AO_BASE_ADDR) && reg32 < (DDRPHY_SHU1_R0_B0_DQ6-DDRPHY_AO_BASE_ADDR))
        {
            except_offset = 0x0;
        }
        else
        {
            except_offset = 0x8;
        }
    }
    else if (reg32 >= (DDRPHY_R0_B0_RXDVS0-DDRPHY_AO_BASE_ADDR) && reg32 < (DDRPHY_R0_B1_RXDVS0-DDRPHY_AO_BASE_ADDR))
    {
        offset = 0x80;
        except_offset = 0;
    }
    else if (reg32 >= (DDRPHY_SHU1_B0_DQ0-DDRPHY_AO_BASE_ADDR) && reg32 < (DDRPHY_SHU1_B1_DQ0-DDRPHY_AO_BASE_ADDR))
    {
        offset = 0x80;
        except_offset = 0;
    }
    else if (reg32 >= (DDRPHY_B0_DLL_ARPI0-DDRPHY_AO_BASE_ADDR) && reg32 < (DDRPHY_B1_DLL_ARPI0-DDRPHY_AO_BASE_ADDR))
    {
        offset = 0x80;
        except_offset = 0;
    }
    else
    {
        offset = 0x0;
        except_offset = 0;
    }

      switch(ucByteIdx)
      {
        case 0:
            return u4IO32ReadFldAlign(reg32+Channel_A_PHY_AO_BASE_VIRTUAL+offset, fld);   //CHA_B1
            break;
#if (FOR_DV_SIMULATION_USED==0)
        case 1:
            return u4IO32ReadFldAlign(reg32+Channel_B_PHY_AO_BASE_VIRTUAL, fld); //CHB_B0
            break;
        case 2:
                return u4IO32ReadFldAlign(reg32+Channel_B_PHY_AO_BASE_VIRTUAL+(offset<<1)+except_offset, fld); //CHB_CA
            break;
#else
        case 2:
            return u4IO32ReadFldAlign(reg32+Channel_B_PHY_AO_BASE_VIRTUAL, fld); //CHB_B0
            break;
        case 1:
                return u4IO32ReadFldAlign(reg32+Channel_B_PHY_AO_BASE_VIRTUAL+(offset<<1)+except_offset, fld); //CHB_CA
            break;
#endif
        case 3:
            return u4IO32ReadFldAlign(reg32+Channel_B_PHY_AO_BASE_VIRTUAL+offset, fld); //CHB_B1
            break;
        }
}
#endif

#if ENABLE_LP3_SW
//for LP3 write register in different byte
void vIO32WriteFldAlign_Phy_Byte(U8 ucByteIdx, U32 reg32, U32 val32, U32 fld)
{
    U32 offset=0;
    U32 except_offset=0;

    if(reg32<Channel_A_PHY_AO_BASE_VIRTUAL)
    {
        mcSHOW_DBG_MSG(("\n[WriteFldAlign_Phy_Byte] wrong addr 0x%x\n", reg32));
        return;
    }

    reg32 &= 0xffff;

    if (reg32 >= (DDRPHY_SHU1_R0_B0_DQ0-DDRPHY_AO_BASE_ADDR) && reg32 < (DDRPHY_SHU1_R0_B1_DQ0-DDRPHY_AO_BASE_ADDR))
    {
        offset = 0x50;
        if (reg32 >= (DDRPHY_SHU1_R0_B0_DQ0-DDRPHY_AO_BASE_ADDR) && reg32 < (DDRPHY_SHU1_R0_B0_DQ6-DDRPHY_AO_BASE_ADDR))
        {
            except_offset = 0x0;
        }
        else
        {
        except_offset = 0x8;
        }
    }
    else if (reg32 >= (DDRPHY_R0_B0_RXDVS0-DDRPHY_AO_BASE_ADDR) && reg32 < (DDRPHY_R0_B1_RXDVS0-DDRPHY_AO_BASE_ADDR))
    {
        offset = 0x80;
        except_offset = 0x0;
    }
    else if (reg32 >= (DDRPHY_SHU1_B0_DQ0-DDRPHY_AO_BASE_ADDR) && reg32 < (DDRPHY_SHU1_B1_DQ0-DDRPHY_AO_BASE_ADDR))
    {
        offset = 0x80;
        except_offset = 0;
    }
    else if (reg32 >= (DDRPHY_B0_DLL_ARPI0-DDRPHY_AO_BASE_ADDR) && reg32 < (DDRPHY_B1_DLL_ARPI0-DDRPHY_AO_BASE_ADDR))
    {
        offset = 0x80;
        except_offset = 0;
    }
    else
    {
        offset = 0x0;
        except_offset = 0;
    }

    switch(ucByteIdx)
    {
        case 0:
            vIO32WriteFldAlign(reg32+Channel_A_PHY_AO_BASE_VIRTUAL+offset, val32, fld); //CHA_B1
            break;
#if (FOR_DV_SIMULATION_USED==0)
        case 1:
            vIO32WriteFldAlign(reg32+Channel_B_PHY_AO_BASE_VIRTUAL, val32, fld); //CHB_B0
            break;
        case 2:
                vIO32WriteFldAlign(reg32+Channel_B_PHY_AO_BASE_VIRTUAL+(offset<<1)+except_offset, val32, fld); //CHB_CA
            break;
#else
    case 2:
        vIO32WriteFldAlign(reg32+Channel_B_PHY_AO_BASE_VIRTUAL, val32, fld); //CHB_B0
        break;
    case 1:
            vIO32WriteFldAlign(reg32+Channel_B_PHY_AO_BASE_VIRTUAL+(offset<<1)+except_offset, val32, fld); //CHB_CA
        break;
#endif
        case 3:
            vIO32WriteFldAlign(reg32+Channel_B_PHY_AO_BASE_VIRTUAL+offset, val32, fld); //CHB_B1
            break;
    }
}
#endif /* ENABLE_LP3_SW */


// for LP3 to control all PHY of single channel
void vIO32WriteFldAlign_Phy_All(U32 reg32, U32 val32, U32 fld)
{
    U8 ucCHIdx;
    
    if(reg32<Channel_A_PHY_AO_BASE_VIRTUAL)
    {
        mcSHOW_DBG_MSG(("\n[WriteFldAlign_Phy_All] wrong addr 0x%x\n", reg32));
        return;
    }

    reg32 &= 0xffff;

    for(ucCHIdx=0; ucCHIdx<CHANNEL_NUM; ucCHIdx++)
    {
        vIO32WriteFldAlign(reg32+Channel_A_PHY_AO_BASE_VIRTUAL+0x10000*ucCHIdx, val32, fld);
    }
}

void vApplyConfigAfterCalibration(DRAMC_CTX_T *p)
{
    U32 backup_channel;

    backup_channel = vGetPHY2ChannelMapping(p);

/*================================
    PHY RX Settings
==================================*/

    if(u1IsLP4Family(p->dram_type))
    {
        vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL4, 0x11400000, MISC_CG_CTRL4_R_PHY_MCK_CG_CTRL);
        vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL1, 0x0, REFCTRL1_SREF_CG_OPT);
        vIO32WriteFldAlign_All(DRAMC_REG_SHUCTRL, 0x0, SHUCTRL_DVFS_CG_OPT); 
        
        /* Burst mode settings are removed from here due to
         *  1. Set in UpdateInitialSettings_LP4
         *  2. DQS Gating ensures new burst mode is switched when to done
         *     (or doesn't switch gatingMode at all, depending on "LP4_GATING_OLD_BURST_MODE")
         */

        vIO32WriteFldAlign_All(DDRPHY_CA_CMD6, 0x0, CA_CMD6_RG_RX_ARCMD_RES_BIAS_EN);

        vIO32WriteFldAlign_All(DDRPHY_B0_DQ6, 0x0, B0_DQ6_RG_TX_ARDQ_OE_EXT_DIS_B0);
        vIO32WriteFldAlign_All(DDRPHY_B1_DQ6, 0x0, B1_DQ6_RG_TX_ARDQ_OE_EXT_DIS_B1);
        vIO32WriteFldAlign_All(DDRPHY_CA_CMD6, 0x0, CA_CMD6_RG_TX_ARCMD_OE_EXT_DIS);
    }
#if ENABLE_LP3_SW
    else
    {   //pulse mode
        vIO32WriteFldAlign_All(DDRPHY_B0_DQ9, 0x0, B0_DQ9_RG_RX_ARDQS_DQSIENMODE_B0);
        vIO32WriteFldAlign_All(DDRPHY_B1_DQ9, 0x0, B1_DQ9_RG_RX_ARDQS_DQSIENMODE_B1);
        vIO32WriteFldAlign_All(DDRPHY_CA_CMD10, 0x0, CA_CMD10_RG_RX_ARCLK_DQSIENMODE);

        vIO32WriteFldAlign_All(DDRPHY_B0_DQ6, 0x0, B0_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B0);
        vIO32WriteFldAlign_All(DDRPHY_B1_DQ6, 0x0, B1_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B1);
        vIO32WriteFldAlign_All(DDRPHY_CA_CMD6, 0x0, CA_CMD6_RG_RX_ARCMD_BIAS_VREF_SEL);

#if (fcFOR_CHIP_ID == fcBianco)
        vIO32WriteFldAlign(DDRPHY_CA_CMD6, 0x0, CA_CMD6_RG_RX_ARCMD_RES_BIAS_EN);
        vIO32WriteFldAlign(DDRPHY_CA_CMD6+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, CA_CMD6_RG_RX_ARCMD_RES_BIAS_EN);
#endif

        vIO32WriteFldAlign_All(DDRPHY_B0_DQ6, 0x1, B0_DQ6_RG_TX_ARDQ_OE_EXT_DIS_B0);
        vIO32WriteFldAlign_All(DDRPHY_B1_DQ6, 0x1, B1_DQ6_RG_TX_ARDQ_OE_EXT_DIS_B1);
        vIO32WriteFldAlign_All(DDRPHY_CA_CMD6, 0x1, CA_CMD6_RG_TX_ARCMD_OE_EXT_DIS);

    }
#endif /* ENABLE_LP3_SW */

        //DA mode
        vIO32WriteFldAlign_All(DDRPHY_B0_DQ6, 0x0, B0_DQ6_RG_RX_ARDQ_BIAS_PS_B0);
        vIO32WriteFldAlign_All(DDRPHY_B1_DQ6, 0x0, B1_DQ6_RG_RX_ARDQ_BIAS_PS_B1);
        vIO32WriteFldAlign_All(DDRPHY_CA_CMD6, 0x0, CA_CMD6_RG_RX_ARCMD_BIAS_PS);

        vIO32WriteFldAlign_All(DDRPHY_B0_DQ6, 0x1, B0_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B0);
        vIO32WriteFldAlign_All(DDRPHY_B1_DQ6, 0x1, B1_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B1);
        vIO32WriteFldAlign_All(DDRPHY_CA_CMD6, 0x1, CA_CMD6_RG_RX_ARCMD_RPRE_TOG_EN);

        ///TODO: Jeremy : move to ??
        vIO32WriteFldMulti_All(DDRPHY_SHU1_B0_DQ3, P_Fld(0x0, SHU1_B0_DQ3_DQ_REV_B0_BIT_03) | P_Fld(0x0, SHU1_B0_DQ3_DQ_REV_B0_BIT_04) | P_Fld(0x1, SHU1_B0_DQ3_DQ_REV_B0_BIT_05));
        vIO32WriteFldMulti_All(DDRPHY_SHU1_B1_DQ3, P_Fld(0x0, SHU1_B1_DQ3_DQ_REV_B1_BIT_03) | P_Fld(0x0, SHU1_B1_DQ3_DQ_REV_B1_BIT_04) | P_Fld(0x1, SHU1_B1_DQ3_DQ_REV_B1_BIT_05));        
        vIO32WriteFldMulti_All(DDRPHY_SHU1_CA_CMD3, P_Fld(0x0, SHU1_CA_CMD3_ARCMD_REV_BIT_03) | P_Fld(0x0, SHU1_CA_CMD3_ARCMD_REV_BIT_04) | P_Fld(0x1, SHU1_CA_CMD3_ARCMD_REV_BIT_05));        
        vIO32WriteFldMulti_All(DDRPHY_SHU2_B0_DQ3, P_Fld(0x0, SHU1_B0_DQ3_DQ_REV_B0_BIT_03) | P_Fld(0x0, SHU1_B0_DQ3_DQ_REV_B0_BIT_04) | P_Fld(0x1, SHU1_B0_DQ3_DQ_REV_B0_BIT_05));
        vIO32WriteFldMulti_All(DDRPHY_SHU2_B1_DQ3, P_Fld(0x0, SHU1_B1_DQ3_DQ_REV_B1_BIT_03) | P_Fld(0x0, SHU1_B1_DQ3_DQ_REV_B1_BIT_04) | P_Fld(0x1, SHU1_B1_DQ3_DQ_REV_B1_BIT_05));        
        vIO32WriteFldMulti_All(DDRPHY_SHU2_CA_CMD3, P_Fld(0x0, SHU1_CA_CMD3_ARCMD_REV_BIT_03) | P_Fld(0x0, SHU1_CA_CMD3_ARCMD_REV_BIT_04) | P_Fld(0x1, SHU1_CA_CMD3_ARCMD_REV_BIT_05));        
        vIO32WriteFldMulti_All(DDRPHY_SHU3_B0_DQ3, P_Fld(0x0, SHU1_B0_DQ3_DQ_REV_B0_BIT_03) | P_Fld(0x0, SHU1_B0_DQ3_DQ_REV_B0_BIT_04) | P_Fld(0x1, SHU1_B0_DQ3_DQ_REV_B0_BIT_05));
        vIO32WriteFldMulti_All(DDRPHY_SHU3_B1_DQ3, P_Fld(0x0, SHU1_B1_DQ3_DQ_REV_B1_BIT_03) | P_Fld(0x0, SHU1_B1_DQ3_DQ_REV_B1_BIT_04) | P_Fld(0x1, SHU1_B1_DQ3_DQ_REV_B1_BIT_05));        
        vIO32WriteFldMulti_All(DDRPHY_SHU3_CA_CMD3, P_Fld(0x0, SHU1_CA_CMD3_ARCMD_REV_BIT_03) | P_Fld(0x0, SHU1_CA_CMD3_ARCMD_REV_BIT_04) | P_Fld(0x1, SHU1_CA_CMD3_ARCMD_REV_BIT_05));        

        ///TODO: Jeremy : move to ??
        if(p->dram_type == TYPE_LPDDR3)
        {
            #if DramcHWDQSGatingTracking_DVT_JADE_TRACKING_MODE
            vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ3, 0, SHU1_B1_DQ3_DQ_REV_B1_BIT_05);            
            vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ3+((U32)CHANNEL_B<<POS_BANK_NUM), 0, SHU1_B0_DQ3_DQ_REV_B0_BIT_05);
            vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD3+((U32)CHANNEL_B<<POS_BANK_NUM), 0, SHU1_CA_CMD3_ARCMD_REV_BIT_05);
            vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ3+((U32)CHANNEL_B<<POS_BANK_NUM), 0, SHU1_B1_DQ3_DQ_REV_B1_BIT_05);
            vIO32WriteFldAlign(DDRPHY_SHU2_B1_DQ3, 0, SHU1_B1_DQ3_DQ_REV_B1_BIT_05);
            vIO32WriteFldAlign(DDRPHY_SHU2_B0_DQ3+((U32)CHANNEL_B<<POS_BANK_NUM), 0, SHU1_B0_DQ3_DQ_REV_B0_BIT_05);
            vIO32WriteFldAlign(DDRPHY_SHU2_CA_CMD3+((U32)CHANNEL_B<<POS_BANK_NUM), 0, SHU1_CA_CMD3_ARCMD_REV_BIT_05);
            vIO32WriteFldAlign(DDRPHY_SHU2_B1_DQ3+((U32)CHANNEL_B<<POS_BANK_NUM), 0, SHU1_B1_DQ3_DQ_REV_B1_BIT_05); 
            vIO32WriteFldAlign(DDRPHY_SHU3_B1_DQ3, 0, SHU1_B1_DQ3_DQ_REV_B1_BIT_05);
            vIO32WriteFldAlign(DDRPHY_SHU3_B0_DQ3+((U32)CHANNEL_B<<POS_BANK_NUM), 0, SHU1_B0_DQ3_DQ_REV_B0_BIT_05);
            vIO32WriteFldAlign(DDRPHY_SHU3_CA_CMD3+((U32)CHANNEL_B<<POS_BANK_NUM), 0, SHU1_CA_CMD3_ARCMD_REV_BIT_05);
            vIO32WriteFldAlign(DDRPHY_SHU3_B1_DQ3+((U32)CHANNEL_B<<POS_BANK_NUM), 0, SHU1_B1_DQ3_DQ_REV_B1_BIT_05);
            #endif
        }



/*================================
    IMPCAL Settings
==================================*/
    vIO32WriteFldMulti_All(DRAMC_REG_IMPCAL, P_Fld(0, IMPCAL_IMPCAL_IMPPDP) | P_Fld(0, IMPCAL_IMPCAL_IMPPDN));    //RG_RIMP_BIAS_EN and RG_RIMP_VREF_EN move to IMPPDP and IMPPDN
    vIO32WriteFldAlign_All(DDRPHY_MISC_IMP_CTRL0, 0, MISC_IMP_CTRL0_RG_RIMP_EN);

/*================================
    MR1
==================================*/

    //MR1 op[7]=0 already be setted at end of gating calibration, no need to set here again
/*
    u1MR01Value[p->dram_fsp] &= 0x7f;
    DramcModeRegWrite(p, 0x1, u1MR01Value[p->dram_fsp]);
*/
    //Prevent M_CK OFF because of hardware auto-sync
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0, Fld(4,0,AC_MSKB0));

    //DFS- fix Gating Tracking settings
    vIO32WriteFldAlign_All(DDRPHY_MISC_CTRL0, 0, MISC_CTRL0_R_STBENCMP_DIV4CK_EN);
    vIO32WriteFldAlign_All(DDRPHY_MISC_CTRL1, 0, MISC_CTRL1_R_DMSTBENCMP_RK_OPT);

    ///TODO: Disable MR4 MR18/MR19, TxHWTracking, Dummy RD - for DFS workaround
    vIO32WriteFldAlign_All(DRAMC_REG_SPCMDCTRL, 0x1, SPCMDCTRL_REFRDIS);    //MR4 Disable
    vIO32WriteFldAlign_All(DRAMC_REG_DQSOSCR, 0x1, DQSOSCR_DQSOSCRDIS);  //MR18, MR19 Disable
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_SCINTV, 0x1, SHU_SCINTV_DQSOSCENDIS);
    vIO32WriteFldAlign_All(DRAMC_REG_SHU2_SCINTV, 0x1, SHU2_SCINTV_DQSOSCENDIS);
    vIO32WriteFldAlign_All(DRAMC_REG_SHU3_SCINTV, 0x1, SHU3_SCINTV_DQSOSCENDIS);
    vIO32WriteFldMulti_All(DRAMC_REG_DUMMY_RD, P_Fld(0x0, DUMMY_RD_DUMMY_RD_EN)
                                            | P_Fld(0x0, DUMMY_RD_SREF_DMYRD_EN)
                                            | P_Fld(0x0, DUMMY_RD_DQSG_DMYRD_EN)
                                            | P_Fld(0x0, DUMMY_RD_DMY_RD_DBG));

#if APPLY_LP4_POWER_INIT_SEQUENCE
    //CKE dynamic
    CKEFixOnOff(p, CKE_DYNAMIC, CKE_WRITE_TO_ALL_CHANNEL);
    //// Enable  HW MIOCK control to make CLK dynamic
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 0, DRAMC_PD_CTRL_MIOCKCTRLOFF);
#endif

    //close eyescan to save power
    vIO32WriteFldMulti_All(DRAMC_REG_EYESCAN, P_Fld(0x0, EYESCAN_EYESCAN_DQS_SYNC_EN)    
                                        | P_Fld(0x0, EYESCAN_EYESCAN_NEW_DQ_SYNC_EN)    
                                        | P_Fld(0x0, EYESCAN_EYESCAN_DQ_SYNC_EN));

    /* TESTAGENT2 */
    vIO32WriteFldAlign_All(DRAMC_REG_TEST2_4, 4, TEST2_4_TESTAGENTRKSEL); // Rank selection is controlled by Test Agent

    vSetPHY2ChannelMapping(p, backup_channel);
}

#ifdef LOOPBACK_TEST
void DramcLoopbackTest_settings(DRAMC_CTX_T *p, U8 u1Type)
{

    //close DCM, or DQ no clock
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 0, MISC_CTRL1_R_DMDQSIENCG_EN);

    if (u1Type==1) //external loop back
    {
        //IOBIAS settings
        vIO32WriteFldAlign_All(DDRPHY_B0_DQ6, 1, B0_DQ6_RG_RX_ARDQ_BIAS_PS_B0);
        vIO32WriteFldAlign_All(DDRPHY_B1_DQ6, 1, B1_DQ6_RG_RX_ARDQ_BIAS_PS_B1);
        vIO32WriteFldAlign_All(DDRPHY_CA_CMD6, 1, CA_CMD6_RG_RX_ARCMD_BIAS_PS);

        //after initial, must set 0 of PHY registers
        vIO32WriteFldMulti_All(DDRPHY_B0_DQ3, P_Fld(0, B0_DQ3_RG_RX_ARDQS0_SWAP_EN_B0) | P_Fld(0, B0_DQ3_RG_RX_ARDQ_OFFC_EN_B0) | P_Fld(0, B0_DQ3_RG_RX_ARDQ_SMT_EN_B0));
        vIO32WriteFldMulti_All(DDRPHY_B1_DQ3, P_Fld(0, B1_DQ3_RG_RX_ARDQS0_SWAP_EN_B1) | P_Fld(0, B1_DQ3_RG_RX_ARDQ_OFFC_EN_B1) | P_Fld(0, B1_DQ3_RG_RX_ARDQ_SMT_EN_B1));
        vIO32WriteFldMulti_All(DDRPHY_CA_CMD3, P_Fld(0, CA_CMD3_RG_RX_ARCLK_SWAP_EN) | P_Fld(0, CA_CMD3_RG_RX_ARCMD_OFFC_EN) | P_Fld(0, CA_CMD3_RG_RX_ARCMD_SMT_EN));

        //after initial, must set 1 of PHY registers
        vIO32WriteFldMulti_All(DDRPHY_B0_DQ3, P_Fld(1, B0_DQ3_RG_RX_ARDQ_STBENCMP_EN_B0) | P_Fld(1, B0_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B0) | P_Fld(1, B0_DQ3_RG_RX_ARDQS_IN_BUFF_EN_B0) | P_Fld(1, B0_DQ3_RG_ARDQ_RESETB_B0));
        vIO32WriteFldAlign_All(DDRPHY_B0_DQ9, 1, B0_DQ9_RG_RX_ARDQ_STBEN_RESETB_B0);        
        vIO32WriteFldAlign_All(DDRPHY_B0_DQ5, 1, B0_DQ5_RG_RX_ARDQ_VREF_EN_B0);
        vIO32WriteFldMulti_All(DDRPHY_B0_DQ6, P_Fld(1, B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0) | P_Fld(1, B0_DQ6_RG_RX_ARDQ_RES_BIAS_EN_B0));

        vIO32WriteFldMulti_All(DDRPHY_B1_DQ3, P_Fld(1, B1_DQ3_RG_RX_ARDQ_STBENCMP_EN_B1) | P_Fld(1, B1_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B1) | P_Fld(1, B1_DQ3_RG_RX_ARDQS_IN_BUFF_EN_B1) | P_Fld(1, B1_DQ3_RG_ARDQ_RESETB_B1));
        vIO32WriteFldAlign_All(DDRPHY_B1_DQ9, 1, B1_DQ9_RG_RX_ARDQ_STBEN_RESETB_B1);                
        vIO32WriteFldAlign_All(DDRPHY_B1_DQ5, 1, B1_DQ5_RG_RX_ARDQ_VREF_EN_B1);
        vIO32WriteFldMulti_All(DDRPHY_B1_DQ6, P_Fld(1, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1) | P_Fld(1, B1_DQ6_RG_RX_ARDQ_RES_BIAS_EN_B1));

        vIO32WriteFldMulti_All(DDRPHY_CA_CMD3, P_Fld(1, CA_CMD3_RG_RX_ARCMD_STBENCMP_EN) | P_Fld(1, CA_CMD3_RG_RX_ARCMD_IN_BUFF_EN) | P_Fld(1, CA_CMD3_RG_RX_ARCLK_IN_BUFF_EN) | P_Fld(1, CA_CMD3_RG_ARCMD_RESETB));
        vIO32WriteFldAlign_All(DDRPHY_CA_CMD10, 1, CA_CMD10_RG_RX_ARCMD_STBEN_RESETB);                        
        vIO32WriteFldAlign_All(DDRPHY_CA_CMD5, 1, CA_CMD5_RG_RX_ARCMD_VREF_EN);
        vIO32WriteFldMulti_All(DDRPHY_CA_CMD6, P_Fld(1, CA_CMD6_RG_RX_ARCMD_BIAS_EN) | P_Fld(1, CA_CMD6_RG_RX_ARCMD_RES_BIAS_EN));
    }
}
#endif


void vResetDelayChainBeforeCalibration(DRAMC_CTX_T *p)
{
    U8 u1RankIdx, u1RankIdxBak;

    u1RankIdxBak = u1GetRank(p);

    for(u1RankIdx=RANK_0; u1RankIdx<RANK_MAX; u1RankIdx++)
    {
        vSetRank(p, u1RankIdx);

        vIO32WriteFldMulti_All(DDRPHY_SHU1_R0_CA_CMD0, P_Fld(0x0, SHU1_R0_CA_CMD0_RK0_TX_ARCA5_DLY) 
                | P_Fld(0x0, SHU1_R0_CA_CMD0_RK0_TX_ARCA4_DLY) 
                | P_Fld(0x0, SHU1_R0_CA_CMD0_RK0_TX_ARCA3_DLY) 
                | P_Fld(0x0, SHU1_R0_CA_CMD0_RK0_TX_ARCA2_DLY) 
                | P_Fld(0x0, SHU1_R0_CA_CMD0_RK0_TX_ARCA1_DLY) 
                | P_Fld(0x0, SHU1_R0_CA_CMD0_RK0_TX_ARCA0_DLY));    
        vIO32WriteFldMulti_All(DDRPHY_SHU1_R0_B0_DQ0, P_Fld(0x0, SHU1_R0_B0_DQ0_RK0_TX_ARDQ7_DLY_B0) 
                | P_Fld(0x0, SHU1_R0_B0_DQ0_RK0_TX_ARDQ6_DLY_B0) 
                | P_Fld(0x0, SHU1_R0_B0_DQ0_RK0_TX_ARDQ5_DLY_B0) 
                | P_Fld(0x0, SHU1_R0_B0_DQ0_RK0_TX_ARDQ4_DLY_B0) 
                | P_Fld(0x0, SHU1_R0_B0_DQ0_RK0_TX_ARDQ3_DLY_B0) 
                | P_Fld(0x0, SHU1_R0_B0_DQ0_RK0_TX_ARDQ2_DLY_B0) 
                | P_Fld(0x0, SHU1_R0_B0_DQ0_RK0_TX_ARDQ1_DLY_B0) 
                | P_Fld(0x0, SHU1_R0_B0_DQ0_RK0_TX_ARDQ0_DLY_B0));    
        vIO32WriteFldMulti_All(DDRPHY_SHU1_R0_B1_DQ0, P_Fld(0x0, SHU1_R0_B1_DQ0_RK0_TX_ARDQ7_DLY_B1) 
                | P_Fld(0x0, SHU1_R0_B1_DQ0_RK0_TX_ARDQ6_DLY_B1) 
                | P_Fld(0x0, SHU1_R0_B1_DQ0_RK0_TX_ARDQ5_DLY_B1) 
                | P_Fld(0x0, SHU1_R0_B1_DQ0_RK0_TX_ARDQ4_DLY_B1) 
                | P_Fld(0x0, SHU1_R0_B1_DQ0_RK0_TX_ARDQ3_DLY_B1) 
                | P_Fld(0x0, SHU1_R0_B1_DQ0_RK0_TX_ARDQ2_DLY_B1) 
                | P_Fld(0x0, SHU1_R0_B1_DQ0_RK0_TX_ARDQ1_DLY_B1) 
                | P_Fld(0x0, SHU1_R0_B1_DQ0_RK0_TX_ARDQ0_DLY_B1));
        vIO32WriteFldAlign_All(DDRPHY_SHU1_R0_B0_DQ1, 0x0, SHU1_R0_B0_DQ1_RK0_TX_ARDQM0_DLY_B0);
        vIO32WriteFldAlign_All(DDRPHY_SHU1_R0_B1_DQ1, 0x0, SHU1_R0_B1_DQ1_RK0_TX_ARDQM0_DLY_B1);
    }

    vSetRank(p, u1RankIdxBak);
}


void vApplyConfigBeforeCalibration(DRAMC_CTX_T *p)
{
    U8 read_xrtw2w;
    U8 u1RankIdx, u1RankIdxBak;
    u1RankIdxBak = u1GetRank(p);

#if ENABLE_LP3_SW
        if(p->dram_type == TYPE_LPDDR3)
        {
            vIO32WriteFldAlign(DDRPHY_SHU1_B1_DLL0, 0x6, SHU1_B1_DLL0_RG_ARDLL_GAIN_B1);
            vIO32WriteFldAlign(DDRPHY_SHU1_CA_DLL0+(((U32)CHANNEL_B<<POS_BANK_NUM)), 0x6, SHU1_CA_DLL0_RG_ARDLL_GAIN_CA);
            vIO32WriteFldAlign(DDRPHY_SHU1_B0_DLL0+(((U32)CHANNEL_B<<POS_BANK_NUM)), 0x6, SHU1_B0_DLL0_RG_ARDLL_GAIN_B0);
            vIO32WriteFldAlign(DDRPHY_SHU1_B1_DLL0+(((U32)CHANNEL_B<<POS_BANK_NUM)), 0x6, SHU1_B1_DLL0_RG_ARDLL_GAIN_B1);
        }
#endif

    //Clk free run
#if (SW_CHANGE_FOR_SIMULATION==0)
    EnableDramcPhyDCM(p, 0);
#endif

    //Set LP3/LP4 Rank0/1 CA/TX delay chain to 0
#if (FOR_DV_SIMULATION_USED==0)
    //CA0~9 per bit delay line -> CHA_CA0 CHA_CA3 CHA_B0_DQ6 CHA_B0_DQ7 CHA_B0_DQ2 CHA_B0_DQ5 CHA_B0_DQ4 CHA_B0_DQ1 CHA_B0_DQ0 CHA_B0_DQ3
    vResetDelayChainBeforeCalibration(p);
#endif

    //MR4 refresh cnt set to 0x1ff (2ms update)
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_CONF3, 0x1ff, SHU_CONF3_REFRCNT);

    //The counter for Read MR4 cannot be reset after SREF if DRAMC no power down.
    vIO32WriteFldAlign_All(DRAMC_REG_SPCMDCTRL, 1, SPCMDCTRL_SRFMR4_CNTKEEP_B);

    //---- ZQ CS init --------
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_SCINTV, 0x1f, SHU_SCINTV_TZQLAT); //ZQ Calibration Time, unit: 38.46ns, tZQCAL min is 1 us. need to set larger than 0x1b
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_CONF3, 0x1ff, SHU_CONF3_ZQCSCNT); //Every refresh number to issue ZQCS commands, only for DDR3/LPDDR2/LPDDR3/LPDDR4
    vIO32WriteFldAlign_All(DRAMC_REG_SHU2_CONF3, 0x1ff, SHU_CONF3_ZQCSCNT); //Every refresh number to issue ZQCS commands, only for DDR3/LPDDR2/LPDDR3/LPDDR4
    vIO32WriteFldAlign_All(DRAMC_REG_SHU3_CONF3, 0x1ff, SHU_CONF3_ZQCSCNT); //Every refresh number to issue ZQCS commands, only for DDR3/LPDDR2/LPDDR3/LPDDR4
    vIO32WriteFldAlign_All(DRAMC_REG_DRAMCTRL, 0, DRAMCTRL_ZQCALL);  // HW send ZQ command for both rank, disable it due to some dram only have 1 ZQ pin for two rank.

    //Dual channel ZQCS interlace,  0: disable, 1: enable
    if(p->support_channel_num==CHANNEL_SINGLE)
    {
        //single channel, ZQCSDUAL=0, ZQCSMASK=0
        vIO32WriteFldMulti(DRAMC_REG_ZQCS, P_Fld(0, ZQCS_ZQCSDUAL)| P_Fld(0x0, ZQCS_ZQCSMASK));
    }
    else if(p->support_channel_num==CHANNEL_DUAL)
    {
        // HW ZQ command is channel interleaving since 2 channel share the same ZQ pin.
        #ifdef ZQCS_ENABLE_LP4
        // dual channel, ZQCSDUAL =1, and CHA ZQCSMASK=0, CHB ZQCSMASK=1
        vIO32WriteFldMulti_All(DRAMC_REG_ZQCS, P_Fld(1, ZQCS_ZQCSDUAL) | \
                                               P_Fld(0, ZQCS_ZQCSMASK_OPT) | \
                                               P_Fld(0, ZQCS_ZQMASK_CGAR) | \
                                               P_Fld(0, ZQCS_ZQCS_MASK_SEL_CGAR));
       
        // DRAMC CHA(CHN0):ZQCSMASK=1, DRAMC CHB(CHN1):ZQCSMASK=0.
        // ZQCSMASK setting: (Ch A, Ch B) = (1,0) or (0,1)
        // if CHA.ZQCSMASK=1, and then set CHA.ZQCALDISB=1 first, else set CHB.ZQCALDISB=1 first
        vIO32WriteFldAlign(DRAMC_REG_ZQCS + (CHANNEL_A<< POS_BANK_NUM), 1, ZQCS_ZQCSMASK);
        vIO32WriteFldAlign(DRAMC_REG_ZQCS + (CHANNEL_B<< POS_BANK_NUM), 0, ZQCS_ZQCSMASK);

        // DRAMC CHA(CHN0):ZQCS_ZQCS_MASK_SEL=0, DRAMC CHB(CHN1):ZQCS_ZQCS_MASK_SEL=0.
        vIO32WriteFldAlign_All(DRAMC_REG_ZQCS, 0, ZQCS_ZQCS_MASK_SEL);
        #endif
    }
    
    // Disable LP3 HW ZQ
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), 0, SPCMDCTRL_ZQCSDISB);   //LP3 ZQCSDISB=0
    // Disable LP4 HW ZQ
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), 0, SPCMDCTRL_ZQCALDISB);  //LP4 ZQCALDISB=0
    // ---- End of ZQ CS init -----

    #if ENABLE_TX_TRACKING
    vIO32WriteFldAlign_All(DRAMC_REG_DQSOSCR, p->dram_cbt_mode[RANK_0],DQSOSCR_RK0_BYTE_MODE);
    vIO32WriteFldAlign_All(DRAMC_REG_DQSOSCR, p->dram_cbt_mode[RANK_1],DQSOSCR_RK1_BYTE_MODE);
    #endif

    //Disable write-DBI of DRAMC (Avoids pre-defined data pattern being modified)
    DramcWriteDBIOnOff(p, DBI_OFF);
    //Disable read-DBI of DRAMC (Avoids pre-defined data pattern being modified)
    DramcReadDBIOnOff(p, DBI_OFF);

    //disable MR4 read, REFRDIS=1
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), 1, SPCMDCTRL_REFRDIS);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR), 0x1, DQSOSCR_DQSOSCRDIS);  //MR18, MR19 Disable
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SHU_SCINTV), 0x1, SHU_SCINTV_DQSOSCENDIS);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SHU2_SCINTV), 0x1, SHU2_SCINTV_DQSOSCENDIS);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SHU3_SCINTV), 0x1, SHU3_SCINTV_DQSOSCENDIS);
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD), P_Fld(0x0, DUMMY_RD_DUMMY_RD_EN)
                                            | P_Fld(0x0, DUMMY_RD_SREF_DMYRD_EN)
                                            | P_Fld(0x0, DUMMY_RD_DQSG_DMYRD_EN)
                                            | P_Fld(0x0, DUMMY_RD_DMY_RD_DBG));

    // Disable HW gating tracking first, 0x1c0[31], need to disable both UI and PI tracking or the gating delay reg won't be valid.
    DramcHWGatingOnOff(p, 0);

    // Disable gating debug
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL2), 0, STBCAL2_STB_GERRSTOP);

    for(u1RankIdx=RANK_0; u1RankIdx<RANK_MAX; u1RankIdx++)
    {
        vSetRank(p, u1RankIdx);

        // Disable RX delay tracking
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS2), 0x0, R0_B0_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B0);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2), 0x0, R0_B1_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B1);

        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS2), 0x0, R0_B0_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B0);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2), 0x0, R0_B1_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B1);

        //RX delay mux, delay vlaue from reg.
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS2), 0x0, R0_B0_RXDVS2_R_RK0_DVS_MODE_B0);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2), 0x0, R0_B1_RXDVS2_R_RK0_DVS_MODE_B1);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_CA_RXDVS2), 0x0, R0_CA_RXDVS2_R_RK0_DVS_MODE_CA);
    }
    vSetRank(p, u1RankIdxBak);

    // ARPI_DQ SW mode mux, TX DQ use 1: PHY Reg 0: DRAMC Reg
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 1, MISC_CTRL1_R_DMARPIDQ_SW);

    // Set to all-bank refresh
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0),  0, REFCTRL0_PBREFEN);

    // set MPCRK to 0, MPCRKEN alwasys set 1 (Derping)
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_MRS), 0, MRS_MPCRK);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_MPC_OPTION), 1, MPC_OPTION_MPCRKEN);

    //RG mode
    vIO32WriteFldAlign_All(DDRPHY_B0_DQ6, 0x1, B0_DQ6_RG_RX_ARDQ_BIAS_PS_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DQ6, 0x1, B1_DQ6_RG_RX_ARDQ_BIAS_PS_B1);
    vIO32WriteFldAlign_All(DDRPHY_CA_CMD6, 0x1, CA_CMD6_RG_RX_ARCMD_BIAS_PS);

#if ENABLE_RX_TRACKING_LP4
      if(u1IsLP4Family(p->dram_type))
      {
            DramcRxInputDelayTrackingInit_byFreq(p);
       }
#endif

#ifdef LOOPBACK_TEST
#ifdef LPBK_INTERNAL_EN
    DramcLoopbackTest_settings(p, 0);   //0: internal loopback test 1: external loopback test
#else
    DramcLoopbackTest_settings(p, 1);   //0: internal loopback test 1: external loopback test
#endif
#endif

#if SET_CKE_2_RANK_INDEPENDENT
    SetCKE2RankIndependent(p);
#endif

#ifdef DUMMY_READ_FOR_TRACKING
    if (u1IsLP4Family(p->dram_type))
    {
        vIO32WriteFldAlign_All(DRAMC_REG_DUMMY_RD, 1, DUMMY_RD_DMY_RD_RX_TRACK);
    }
#endif

    vIO32WriteFldAlign_All(DRAMC_REG_DRSCTRL, 1, DRSCTRL_DRSDIS);
}


//Reset PHY to prevent glitch when change DQS gating delay or RX DQS input delay
// [Lynx] Everest : cannot reset single channel. All DramC and All Phy have to reset together.
void DramPhyReset(DRAMC_CTX_T *p)
{
    // Everest change reset order : reset DQS before DQ, move PHY reset to final.
    if(u1IsLP4Family(p->dram_type))
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DDRCONF0), 1, DDRCONF0_RDATRST);// read data counter reset
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 1, MISC_CTRL1_R_DMPHYRST);

        //RG_ARCMD_RESETB & RG_ARDQ_RESETB_B0/1 only reset once at init, Justin Chan.
        ///TODO: need to confirm RG_ARCMD_RESETB & RG_ARDQ_RESETB_B0/1 is reset at mem.c
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B0_DQ9), P_Fld(0, B0_DQ9_RG_RX_ARDQS_STBEN_RESETB_B0) |P_Fld(0, B0_DQ9_RG_RX_ARDQ_STBEN_RESETB_B0));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B1_DQ9), P_Fld(0, B1_DQ9_RG_RX_ARDQS_STBEN_RESETB_B1) |P_Fld(0, B1_DQ9_RG_RX_ARDQ_STBEN_RESETB_B1));
        #ifdef LOOPBACK_TEST
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_CA_CMD10), P_Fld(0, CA_CMD10_RG_RX_ARCLK_STBEN_RESETB) | P_Fld(0, CA_CMD10_RG_RX_ARCMD_STBEN_RESETB));
        #endif
        mcDELAY_US(1);//delay 10ns
        #ifdef LOOPBACK_TEST
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_CA_CMD10), P_Fld(1, CA_CMD10_RG_RX_ARCLK_STBEN_RESETB) | P_Fld(1, CA_CMD10_RG_RX_ARCMD_STBEN_RESETB));
        #endif
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B1_DQ9), P_Fld(1, B1_DQ9_RG_RX_ARDQS_STBEN_RESETB_B1) | P_Fld(1, B1_DQ9_RG_RX_ARDQ_STBEN_RESETB_B1));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B0_DQ9), P_Fld(1, B0_DQ9_RG_RX_ARDQS_STBEN_RESETB_B0) |P_Fld(1, B0_DQ9_RG_RX_ARDQ_STBEN_RESETB_B0));

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 0, MISC_CTRL1_R_DMPHYRST);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DDRCONF0), 0, DDRCONF0_RDATRST);// read data counter reset
    }
#if ENABLE_LP3_SW
    else //LPDDR3
    {
        // Everest change : must reset all dramC and PHY together.
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_DDRCONF0), 1, DDRCONF0_RDATRST);// read data counter reset
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 1, MISC_CTRL1_R_DMPHYRST);

        //RG_ARCMD_RESETB & RG_ARDQ_RESETB_B0/1 only reset once at init, Justin Chan.
        ///TODO: need to confirm RG_ARCMD_RESETB & RG_ARDQ_RESETB_B0/1 is reset at mem.c
        //only in LP3 due to DQ pinmux to CA
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_CA_CMD10), P_Fld(0, CA_CMD10_RG_RX_ARCLK_STBEN_RESETB) |P_Fld(0, CA_CMD10_RG_RX_ARCMD_STBEN_RESETB));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ9), P_Fld(0, B0_DQ9_RG_RX_ARDQS_STBEN_RESETB_B0) |P_Fld(0, B0_DQ9_RG_RX_ARDQ_STBEN_RESETB_B0));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ9), P_Fld(0, B1_DQ9_RG_RX_ARDQS_STBEN_RESETB_B1) |P_Fld(0, B1_DQ9_RG_RX_ARDQ_STBEN_RESETB_B1));
        mcDELAY_US(1);//delay 10ns
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ9), P_Fld(1, B1_DQ9_RG_RX_ARDQS_STBEN_RESETB_B1) |P_Fld(1, B1_DQ9_RG_RX_ARDQ_STBEN_RESETB_B1));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ9), P_Fld(1, B0_DQ9_RG_RX_ARDQS_STBEN_RESETB_B0) |P_Fld(1, B0_DQ9_RG_RX_ARDQ_STBEN_RESETB_B0));
        //only in LP3 due to DQ pinmux to CA
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_CA_CMD10), P_Fld(1, CA_CMD10_RG_RX_ARCLK_STBEN_RESETB) | P_Fld(1, CA_CMD10_RG_RX_ARCMD_STBEN_RESETB));
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 0, MISC_CTRL1_R_DMPHYRST);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_DDRCONF0), 0, DDRCONF0_RDATRST);// read data counter reset
    }
#endif

}


void DramEyeStbenReset(DRAMC_CTX_T *p)
{
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1), 1, GDDR3CTL1_RDATRST);// read data counter reset

    if(u1IsLP4Family(p->dram_type))
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ5), 0, B0_DQ5_RG_RX_ARDQ_EYE_STBEN_RESETB_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ5), 0, B1_DQ5_RG_RX_ARDQ_EYE_STBEN_RESETB_B1);

        mcDELAY_US(1);//delay 10ns

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ5), 1, B0_DQ5_RG_RX_ARDQ_EYE_STBEN_RESETB_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ5), 1, B1_DQ5_RG_RX_ARDQ_EYE_STBEN_RESETB_B1);
    }
#if ENABLE_LP3_SW
    else //LPDDR3
    {
        vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ5), 0, B0_DQ5_RG_RX_ARDQ_EYE_STBEN_RESETB_B0);
        vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ5), 0, B1_DQ5_RG_RX_ARDQ_EYE_STBEN_RESETB_B1);
        vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ5), 0, CA_CMD5_RG_RX_ARCMD_EYE_STBEN_RESETB); //only in LP3 due to DQ pinmux to CA

        mcDELAY_US(1);//delay 10ns

        vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ5), 1, B0_DQ5_RG_RX_ARDQ_EYE_STBEN_RESETB_B0);
        vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ5), 1, B1_DQ5_RG_RX_ARDQ_EYE_STBEN_RESETB_B1);
        vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ5), 1, CA_CMD5_RG_RX_ARCMD_EYE_STBEN_RESETB);//only in LP3 due to DQ pinmux to CA
    }
#endif
}


DRAM_STATUS_T DramcRankSwap(DRAMC_CTX_T *p, U8 u1Rank)
{
    U8 u1Multi;

    #if 0
    if (p->support_rank_num > 1)
        u1Multi = 1;
    else
        u1Multi = 0;
    #else
    u1Multi = 1;
    #endif


    mcSHOW_DBG_MSG(("[RankSwap] Rank num %d, (Multi %d), Rank %d\n", p->support_rank_num, u1Multi, u1Rank));
    //mcFPRINTF((fp_A60501, "[DramcRankSwap] Rank number %d, (u1Multi %d), Rank %d\n", p->support_rank_num, u1Multi, u1Rank));

    //Set to non-zero for multi-rank
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), u1Multi, RKCFG_RKMODE);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), u1Rank, RKCFG_RKSWAP);

    if (u1Rank == 0)
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), 0, RKCFG_TXRANKFIX);
    }
    else
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), 1, RKCFG_TXRANKFIX);
    }
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), u1Rank, RKCFG_TXRANK); //use other rank's setting

    return DRAM_OK;
}


DRAM_STATUS_T DramcStartDQSOSC(DRAMC_CTX_T *p)
{
    U32 u4Response;
    U32 u4TimeCnt;

    u4TimeCnt = TIME_OUT_CNT;
    mcSHOW_DBG_MSG(("[DQSOSC]\n"));

    //R_DMDQSOSCENEN, 0x1E4[10]=1 for DQSOSC Start
    //Wait dqsoscen_response=1 (dramc_conf_nao, 0x3b8[29])
    //R_DMDQSOSCENEN, 0x1E4[10]=0
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1, SPCMD_DQSOSCENEN);
    do
    {
        u4Response = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_DQSOSCEN_RESPONSE);
        u4TimeCnt --;
        mcDELAY_US(1);
    }while((u4Response==0) &&(u4TimeCnt>0));

    if(u4TimeCnt==0)//time out
    {
        mcSHOW_DBG_MSG(("Start fail (time out)\n"));
        //mcFPRINTF((fp_A60501, "[DramcStartDQSOSC]  Start fail (time out)\n"));
        return DRAM_FAIL;
    }
    else
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_DQSOSCENEN);
    }

    return DRAM_OK;
}


DRAM_STATUS_T DramcDQSOSCAuto(DRAMC_CTX_T *p)
{
    U8 u1MR23 = gu1MR23[p->channel][p->rank];
    U16 u2MR18, u2MR19;
    U16 u2DQSCnt;
    U16 u2DQSOsc[2];
    U32 u4RegBak[3];

    u4RegBak[0] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_MRS));
    u4RegBak[1] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL));
    u4RegBak[2] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL));

    //!!R_DMMPCRK(R_DMMPCRKEN=1) specify rank0 or rank1
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), 0, RKCFG_DQSOSC2RK);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1GetRank(p), MRS_MPCRK);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MPC_OPTION), 1, MPC_OPTION_MPCRKEN);

    //LPDDR4-3200,     PI resolution = tCK/64 =9.76ps
    //Only if MR23>=16, then error < PI resolution.
    //Set MR23 == 0x3f, stop after 63*16 clock
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1GetRank(p), MRS_MRSRK);
    DramcModeRegWrite(p, 23, u1MR23);

    //SW mode
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SCINTV), 1, SHU_SCINTV_DQSOSCENDIS);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2_SCINTV), 1, SHU2_SCINTV_DQSOSCENDIS);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3_SCINTV), 1, SHU3_SCINTV_DQSOSCENDIS);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 1, DRAMC_PD_CTRL_MIOCKCTRLOFF);   //MIOCKCTRLOFF=1
    
    CKEFixOnOff(p, CKE_FIXON, CKE_WRITE_TO_ONE_CHANNEL);

    DramcStartDQSOSC(p);
    mcDELAY_US(1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1GetRank(p), MRS_MRRRK);
    DramcModeRegRead(p, 18, &u2MR18);
    DramcModeRegRead(p, 19, &u2MR19);


#if (SW_CHANGE_FOR_SIMULATION==0)
    //B0
    u2DQSCnt = (u2MR18 & 0x00FF) | ((u2MR19 & 0x00FF) <<8);
    if(u2DQSCnt !=0)
        u2DQSOsc[0] = u1MR23*16*1000000/(2 * u2DQSCnt * p->frequency); //tDQSOSC = 16*MR23*tCK/2*count
    else
        u2DQSOsc[0] = 0;

    //B1
    u2DQSCnt = (u2MR18 >> 8) | ((u2MR19 & 0xFF00));
    if(u2DQSCnt !=0)
        u2DQSOsc[1] = u1MR23*16*1000000/(2 * u2DQSCnt * p->frequency); //tDQSOSC = 16*MR23*tCK/2*count
    else
        u2DQSOsc[1] = 0;
    mcSHOW_DBG_MSG(("[DQSOSCAuto] RK%d, (LSB)MR18= 0x%x, (MSB)MR19= 0x%x, tDQSOscB0 = %d ps tDQSOscB1 = %d ps\n", u1GetRank(p), u2MR18, u2MR19, u2DQSOsc[0], u2DQSOsc[1]));
#endif

    gu2MR18[p->channel][p->rank] = u2MR18;
    gu2MR19[p->channel][p->rank] = u2MR19;
    gu2DQSOSC[p->channel][p->rank] = u2DQSOsc[0];

    if(u2DQSOsc[1]!=0 && u2DQSOsc[1]<u2DQSOsc[0])
        gu2DQSOSC[p->channel][p->rank] = u2DQSOsc[1];

    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_MRS), u4RegBak[0]);
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), u4RegBak[1]);
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), u4RegBak[2]);

    if(gu1MR23Done==FALSE)
    {
        DRAM_CHANNEL_T chIdx = CHANNEL_A;
        DRAM_RANK_T rankIdx = RANK_0;
        if(gu2DQSOSC[p->channel][p->rank]>500)
        {
            u1MR23 = 0x30;
        }
        else if(gu2DQSOSC[p->channel][p->rank]>350)
        {
            u1MR23 = 0x20;
        }
        else
        {
            u1MR23 = 0x10;
        }
        for(chIdx=CHANNEL_A; chIdx<p->support_channel_num; chIdx++)
        {
            for(rankIdx=RANK_0; rankIdx<RANK_MAX; rankIdx++)
            {
                gu1MR23[chIdx][rankIdx] = u1MR23;
            }
        }
        mcSHOW_DBG_MSG(("Update MR23=%d\n", u1MR23));
        gu1MR23Done = TRUE;
    }

    return DRAM_OK;
}

DRAM_STATUS_T DramcDQSOSCMR23(DRAMC_CTX_T *p)
{
#if (SW_CHANGE_FOR_SIMULATION==0)
    U16 u2DQSOSC = gu2DQSOSC[p->channel][p->rank];
    U32 u4tCK = 1000000/p->frequency;
    U32 u4RunTime = (32*4*5*(u2DQSOSC*u2DQSOSC)/(u4tCK*u4tCK))+1;
    //U32 u4MR23 = (u4RunTime/16)+1;
    U32 u4MR23 = gu1MR23[p->channel][p->rank];
    
    gu1DQSOSCTHRD_INC[p->channel][p->rank] = (6*u4MR23*16)/(u4RunTime);
    gu1DQSOSCTHRD_DEC[p->channel][p->rank] = (4*u4MR23*16)/(u4RunTime);
    mcSHOW_DBG_MSG(("CH%d_RK%d: MR19=%X, MR18=%X, DQSOSC=%d, Runtime=%d, MR23=%d, INC=%d, DEC=%d\n", p->channel, p->rank, gu2MR19[p->channel][p->rank], gu2MR18[p->channel][p->rank], gu2DQSOSC[p->channel][p->rank], u4RunTime,u4MR23,gu1DQSOSCTHRD_INC[p->channel][p->rank],gu1DQSOSCTHRD_DEC[p->channel][p->rank]));
#endif
    return DRAM_OK;
}

DRAM_STATUS_T DramcDQSOSCSetMR18MR19(DRAMC_CTX_T *p)
{
    U16 u2DQSOscCnt[2];
    DramcDQSOSCAuto(p);

    //B0
    gu2DQSOscCnt[p->channel][p->rank][0] = u2DQSOscCnt[0] = (gu2MR18[p->channel][p->rank] & 0x00FF) | ((gu2MR19[p->channel][p->rank] & 0x00FF) <<8);
    //B1
    gu2DQSOscCnt[p->channel][p->rank][1] = u2DQSOscCnt[1] = (gu2MR18[p->channel][p->rank] >> 8) | ((gu2MR19[p->channel][p->rank] & 0xFF00));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU1RK0_DQSOSC), P_Fld(u2DQSOscCnt[0], SHU1RK0_DQSOSC_DQSOSC_BASE_RK0)| P_Fld(u2DQSOscCnt[1], SHU1RK0_DQSOSC_DQSOSC_BASE_RK0_B1));

    mcSHOW_DBG_MSG(("CH%d RK%d: MR19=%X, MR18=%X\n", p->channel, p->rank, gu2MR19[p->channel][p->rank], gu2MR18[p->channel][p->rank]));
    return DRAM_OK;
}

DRAM_STATUS_T DramcDQSOSCShuSettings(DRAMC_CTX_T *p)
{
    U8 u1MR23 = 0x3F;
    U16 u2PRDCNT = 0x3FF;
    U16 u2DQSOSCENCNT = 0x1FF;
    U8 u1Thrd_inc, u1Thrd_dec;

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SCINTV),  0x0, SHU_SCINTV_DQS2DQ_SHU_PITHRD);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_DQSOSC),  0x0, RK0_DQSOSC_R_DMDQS2DQ_FILT_OPT);
    if(p->frequency<=400)
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SCINTV),  0x2, SHU_SCINTV_DQS2DQ_FILT_PITHRD);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU1_WODT),  P_Fld(0x5, SHU1_WODT_TXUPD_W2R_SEL) | P_Fld(0x0, SHU1_WODT_TXUPD_SEL));
    }
    else if(p->frequency<=800)
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SCINTV),  0x5, SHU_SCINTV_DQS2DQ_FILT_PITHRD);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU1_WODT),  P_Fld(0x5, SHU1_WODT_TXUPD_W2R_SEL) | P_Fld(0x0, SHU1_WODT_TXUPD_SEL));
    }
    else if(p->frequency<=1333)
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SCINTV),  0x8, SHU_SCINTV_DQS2DQ_FILT_PITHRD);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU1_WODT),  P_Fld(0x2, SHU1_WODT_TXUPD_W2R_SEL) | P_Fld(0x0, SHU1_WODT_TXUPD_SEL));
    }
    else if(p->frequency<=1600)
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SCINTV),  0xA, SHU_SCINTV_DQS2DQ_FILT_PITHRD);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU1_WODT),  P_Fld(0x2, SHU1_WODT_TXUPD_W2R_SEL) | P_Fld(0x0, SHU1_WODT_TXUPD_SEL));
    }
    else if(p->frequency<=1866)
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SCINTV),  0xB, SHU_SCINTV_DQS2DQ_FILT_PITHRD);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU1_WODT),  P_Fld(0x2, SHU1_WODT_TXUPD_W2R_SEL) | P_Fld(0x0, SHU1_WODT_TXUPD_SEL));
    }
    
    u2PRDCNT = (gu1MR23[p->channel][RANK_0]/4)+3;
    
    if (p->support_rank_num==RANK_DUAL)
    {
        if(gu1MR23[p->channel][RANK_0]>gu1MR23[p->channel][RANK_1])
            u2PRDCNT = (gu1MR23[p->channel][RANK_0]/4)+3;
        else
            u2PRDCNT = (gu1MR23[p->channel][RANK_1]/4)+3;
    }

    //Don't power down dram during DQS interval timer run time, (MR23[7:0] /4) + (tOSCO/MCK unit/16)
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DQSOSC_PRD), u2PRDCNT, SHU1_DQSOSC_PRD_DQSOSC_PRDCNT);

    //set tOSCO constraint to read MR18/MR19
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DQSOSCR), 0x40, SHU_DQSOSCR_DQSOSCRCNT);//unit: MCK to meet spec. tOSCO


    u1Thrd_inc = gu1DQSOSCTHRD_INC[p->channel][0];
    u1Thrd_dec = gu1DQSOSCTHRD_DEC[p->channel][0];

    if(gu1DQSOSCTHRD_INC[p->channel][1] > gu1DQSOSCTHRD_INC[p->channel][0])
    {
        u1Thrd_inc = gu1DQSOSCTHRD_INC[p->channel][1];
        u1Thrd_dec = gu1DQSOSCTHRD_DEC[p->channel][1];
    }

    /*
    if(p->frequency==1600)
    {

        u1Thrd_inc=6;
        u1Thrd_dec=4;
    }
    else if(p->frequency==1333)
    {
        u1Thrd_inc=14;
        u1Thrd_dec=9;
    }
    else //if(p->frequency=800)
    {
        u1Thrd_inc=24;
        u1Thrd_dec=16;
    }
    */


    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DQSOSC_PRD), u1Thrd_inc, SHU1_DQSOSC_PRD_DQSOSCTHRD_INC);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DQSOSC_PRD), u1Thrd_dec, SHU1_DQSOSC_PRD_DQSOSCTHRD_DEC);

    //set interval to do MPC(start DQSOSC) command, and dramc send DQSOSC start to rank0/1/2 at the same time
    //TX tracking period unit: 3.9us
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DQSOSCR2), u2DQSOSCENCNT, SHU_DQSOSCR2_DQSOSCENCNT);

    return DRAM_OK;
}

void DramcHwDQSOSCSetFreqRatio(DRAMC_CTX_T *p)
{
    //for SHUFFLE_1
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RK2_DQSOSC), P_Fld((U16)(gFreqTbl[1].frequency*8/gFreqTbl[0].frequency), RK2_DQSOSC_FREQ_RATIO_TX_0)
                                                            | P_Fld((U16)(gFreqTbl[2].frequency*8/gFreqTbl[0].frequency), RK2_DQSOSC_FREQ_RATIO_TX_1)
                                                            | P_Fld((U16)(gFreqTbl[3].frequency*8/gFreqTbl[0].frequency), RK2_DQSOSC_FREQ_RATIO_TX_2));
    //for SHUFFLE_2
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RK2_DQSOSC), P_Fld((U16)(gFreqTbl[0].frequency*8/gFreqTbl[1].frequency), RK2_DQSOSC_FREQ_RATIO_TX_3)
                                                            | P_Fld((U16)(gFreqTbl[2].frequency*8/gFreqTbl[1].frequency), RK2_DQSOSC_FREQ_RATIO_TX_4)
                                                            | P_Fld((U16)(gFreqTbl[3].frequency*8/gFreqTbl[1].frequency), RK2_DQSOSC_FREQ_RATIO_TX_5));
    //for SHUFFLE_3
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RK2_DUMMY_RD_BK), P_Fld((U16)(gFreqTbl[0].frequency*8/gFreqTbl[2].frequency), RK2_DUMMY_RD_BK_FREQ_RATIO_TX_6)
                                                            | P_Fld((U16)(gFreqTbl[1].frequency*8/gFreqTbl[2].frequency), RK2_DUMMY_RD_BK_FREQ_RATIO_TX_7)
                                                            | P_Fld((U16)(gFreqTbl[3].frequency*8/gFreqTbl[2].frequency), RK2_DUMMY_RD_BK_FREQ_RATIO_TX_8));
    //for SHUFFLE_4
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_PRE_TDQSCK1), P_Fld((U16)(gFreqTbl[0].frequency*8/gFreqTbl[3].frequency), PRE_TDQSCK1_FREQ_RATIO_TX_9)
                                                            | P_Fld((U16)(gFreqTbl[1].frequency*8/gFreqTbl[3].frequency), PRE_TDQSCK1_FREQ_RATIO_TX_10)
                                                            | P_Fld((U16)(gFreqTbl[2].frequency*8/gFreqTbl[3].frequency), PRE_TDQSCK1_FREQ_RATIO_TX_11)
                                                            | P_Fld(1, PRE_TDQSCK1_SHU_PRELOAD_TX_HW)
                                                            | P_Fld(0, PRE_TDQSCK1_SHU_PRELOAD_TX_START)
                                                            | P_Fld(0, PRE_TDQSCK1_SW_UP_TX_NOW_CASE));

    mcSHOW_DBG_MSG3(("TX_FREQ_RATIO_0=%d\n", u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK2_DQSOSC), RK2_DQSOSC_FREQ_RATIO_TX_0)));
    mcSHOW_DBG_MSG3(("TX_FREQ_RATIO_1=%d\n", u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK2_DQSOSC), RK2_DQSOSC_FREQ_RATIO_TX_1)));
    mcSHOW_DBG_MSG3(("TX_FREQ_RATIO_2=%d\n", u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK2_DQSOSC), RK2_DQSOSC_FREQ_RATIO_TX_2)));
    mcSHOW_DBG_MSG3(("TX_FREQ_RATIO_3=%d\n", u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK2_DQSOSC), RK2_DQSOSC_FREQ_RATIO_TX_3)));
    mcSHOW_DBG_MSG3(("TX_FREQ_RATIO_4=%d\n", u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK2_DQSOSC), RK2_DQSOSC_FREQ_RATIO_TX_4)));
    mcSHOW_DBG_MSG3(("TX_FREQ_RATIO_5=%d\n", u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK2_DQSOSC), RK2_DQSOSC_FREQ_RATIO_TX_5)));
    mcSHOW_DBG_MSG3(("TX_FREQ_RATIO_6=%d\n", u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK2_DUMMY_RD_BK), RK2_DUMMY_RD_BK_FREQ_RATIO_TX_6)));
    mcSHOW_DBG_MSG3(("TX_FREQ_RATIO_7=%d\n", u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK2_DUMMY_RD_BK), RK2_DUMMY_RD_BK_FREQ_RATIO_TX_7)));
    mcSHOW_DBG_MSG3(("TX_FREQ_RATIO_8=%d\n", u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK2_DUMMY_RD_BK), RK2_DUMMY_RD_BK_FREQ_RATIO_TX_8)));
    mcSHOW_DBG_MSG3(("TX_FREQ_RATIO_9=%d\n", u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK2_DQSOSC), RK2_DQSOSC_FREQ_RATIO_TX_0)));
    mcSHOW_DBG_MSG3(("TX_FREQ_RATIO_9=%d\n", u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PRE_TDQSCK1), PRE_TDQSCK1_FREQ_RATIO_TX_9)));
    mcSHOW_DBG_MSG3(("TX_FREQ_RATIO_10=%d\n", u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PRE_TDQSCK1), PRE_TDQSCK1_FREQ_RATIO_TX_10)));
    mcSHOW_DBG_MSG3(("TX_FREQ_RATIO_11=%d\n", u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PRE_TDQSCK1), PRE_TDQSCK1_FREQ_RATIO_TX_11)));
}

void DramcHwDQSOSC(DRAMC_CTX_T *p)
{
    U8 u1MR23 = 0x3F;
    U16 u2DQSOSCENCNT = 0x1FF;
    DRAM_RANK_T rank_bak = u1GetRank(p);
    DRAM_CHANNEL_T ch_bak = p->channel;

    DramcHwDQSOSCSetFreqRatio(p);

    //DQSOSC MPC command violation
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MPC_OPTION), 0, MPC_OPTION_MPC_BLOCKALE_OPT);

    //DQS2DQ UI/PI setting controlled by HW
    #if ENABLE_SW_TX_TRACKING
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 1, MISC_CTRL1_R_DMARPIDQ_SW);
    #else
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 0, MISC_CTRL1_R_DMARPIDQ_SW);
    #endif
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR), 1, DQSOSCR_ARUIDQ_SW);

    //Set dqsosc oscillator run time by MRW
    //write RK0 MR23
    #if 0
    vSetRank(p, RANK_0);
    vSetPHY2ChannelMapping(p, CHANNEL_A);
    DramcModeRegWrite(p, 23, u1MR23);
    vSetPHY2ChannelMapping(p, CHANNEL_B);
    DramcModeRegWrite(p, 23, u1MR23);
    //write RK1 MR23
    vSetRank(p, RANK_1);
    vSetPHY2ChannelMapping(p, CHANNEL_A);
    DramcModeRegWrite(p, 23, u1MR23);
    vSetPHY2ChannelMapping(p, CHANNEL_B);
    DramcModeRegWrite(p, 23, u1MR23);
    #endif

    //Enable HW read MR18/MR19 for each rank
    #if ENABLE_SW_TX_TRACKING
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR), 1, DQSOSCR_DQSOSCRDIS);
    #else
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR), 0, DQSOSCR_DQSOSCRDIS);
    #endif

    vSetRank(p, RANK_0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_DQSOSC), 1, RK0_DQSOSC_DQSOSCR_RK0EN);
    if (p->support_rank_num == RANK_DUAL)
    {
        vSetRank(p, RANK_1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK1_DQSOSC), 1, RK1_DQSOSC_DQSOSCR_RK1EN);
    }
    
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR), 1, DQSOSCR_DQSOSC_CALEN);

    //enable DQSOSC HW mode
    #if ENABLE_SW_TX_TRACKING
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SCINTV), 1, SHU_SCINTV_DQSOSCENDIS);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2_SCINTV), 1, SHU2_SCINTV_DQSOSCENDIS);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3_SCINTV), 1, SHU3_SCINTV_DQSOSCENDIS);
    #else
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SCINTV), 0, SHU_SCINTV_DQSOSCENDIS);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2_SCINTV), 0, SHU2_SCINTV_DQSOSCENDIS);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3_SCINTV), 0, SHU3_SCINTV_DQSOSCENDIS);
    #endif

    vSetRank(p, rank_bak);
    vSetPHY2ChannelMapping(p, ch_bak);
}

#if ENABLE_SW_TX_TRACKING & __ETT__
void DramcSWTxTracking(DRAMC_CTX_T *p)
{
    DRAM_RANK_T rank_bak = u1GetRank(p);
    U8 u1MR4OnOff = 1;
    U8 rankIdx = RANK_0;
    U8 u1ShuLevel = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHUSTATUS), SHUSTATUS_SHUFFLE_LEVEL);
    U16 u2DramcOffset = 0x600*u1ShuLevel;
    U16 u2DdrphyOffset = 0x500*u1ShuLevel;
    U16 u2MR1819_Base[CHANNEL_NUM][RANK_MAX][2];
    U16 u2MR1819_Runtime[CHANNEL_NUM][RANK_MAX][2];
    U8 u1DQSOSC_INC=6,u1DQSOSC_DEC=4;
    U8 u1AdjPI[RANK_MAX][2];
    U8 u1OriginalPI_DQ[DRAM_DFS_SHUFFLE_MAX][RANK_MAX][2];
    U8 u1UpdatedPI_DQ[DRAM_DFS_SHUFFLE_MAX][RANK_MAX][2];
    U8 u1OriginalPI_DQM[DRAM_DFS_SHUFFLE_MAX][RANK_MAX][2];
    U8 u1UpdatedPI_DQM[DRAM_DFS_SHUFFLE_MAX][RANK_MAX][2];
    U8 u1FreqRatioTX[DRAM_DFS_SHUFFLE_MAX];
    U8 shuIdx=0;

    u1FreqRatioTX[0] = (gFreqTbl[0].frequency*8/gFreqTbl[u1ShuLevel].frequency);
    u1FreqRatioTX[1] = (gFreqTbl[1].frequency*8/gFreqTbl[u1ShuLevel].frequency);
    u1FreqRatioTX[2] = (gFreqTbl[2].frequency*8/gFreqTbl[u1ShuLevel].frequency);
    u1FreqRatioTX[3] = (gFreqTbl[3].frequency*8/gFreqTbl[u1ShuLevel].frequency);

    mcSHOW_DBG_MSG(("[SWTxTracking] ShuLevel=%d, Ratio[0]=%d [1]=%d [2]=%d [3]=%d", u1ShuLevel,
        u1FreqRatioTX[0], u1FreqRatioTX[1], u1FreqRatioTX[2], u1FreqRatioTX[3]));
 
    vSetRank(p, RANK_0);
    u1DQSOSC_INC = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DQSOSC_PRD)+u2DramcOffset, SHU1_DQSOSC_PRD_DQSOSCTHRD_INC);
    u1DQSOSC_DEC = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DQSOSC_PRD)+u2DramcOffset, SHU1_DQSOSC_PRD_DQSOSCTHRD_DEC);

    for(shuIdx=0; shuIdx<=DRAM_DFS_SHUFFLE_4; shuIdx++)
    {
        u1OriginalPI_DQ[shuIdx][RANK_0][0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1RK0_PI)+(0x600*shuIdx), SHU1RK0_PI_RK0_ARPI_DQ_B0);
        u1OriginalPI_DQ[shuIdx][RANK_0][1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1RK0_PI)+(0x600*shuIdx), SHU1RK0_PI_RK0_ARPI_DQ_B1);
        u1OriginalPI_DQ[shuIdx][RANK_1][0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1RK1_PI)+(0x600*shuIdx), SHU1RK1_PI_RK1_ARPI_DQ_B0);
        u1OriginalPI_DQ[shuIdx][RANK_1][1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1RK1_PI)+(0x600*shuIdx), SHU1RK1_PI_RK1_ARPI_DQ_B1);

        u1OriginalPI_DQM[shuIdx][RANK_0][0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1RK0_PI)+(0x600*shuIdx), SHU1RK0_PI_RK0_ARPI_DQM_B0);
        u1OriginalPI_DQM[shuIdx][RANK_0][1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1RK0_PI)+(0x600*shuIdx), SHU1RK0_PI_RK0_ARPI_DQM_B1);
        u1OriginalPI_DQM[shuIdx][RANK_1][0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1RK1_PI)+(0x600*shuIdx), SHU1RK1_PI_RK1_ARPI_DQM_B0);
        u1OriginalPI_DQM[shuIdx][RANK_1][1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1RK1_PI)+(0x600*shuIdx), SHU1RK1_PI_RK1_ARPI_DQM_B1);
    }

    u2MR1819_Base[p->channel][RANK_0][0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1RK0_DQSOSC)+u2DramcOffset, SHU1RK0_DQSOSC_DQSOSC_BASE_RK0);
    u2MR1819_Base[p->channel][RANK_0][1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1RK0_DQSOSC)+u2DramcOffset, SHU1RK0_DQSOSC_DQSOSC_BASE_RK0_B1);
    u2MR1819_Base[p->channel][RANK_1][0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1RK1_DQSOSC)+u2DramcOffset, SHU1RK1_DQSOSC_DQSOSC_BASE_RK1);
    u2MR1819_Base[p->channel][RANK_1][1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1RK1_DQSOSC)+u2DramcOffset, SHU1RK1_DQSOSC_DQSOSC_BASE_RK1_B1);

    u1MR4OnOff = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), SPCMDCTRL_REFRDIS);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), 1, SPCMDCTRL_REFRDIS);
    for(rankIdx=RANK_0; rankIdx<RANK_MAX; rankIdx++)
    {
        U8 byteIdx=0;

        vSetRank(p, rankIdx);
        DramcDQSOSCAuto(p);
        u2MR1819_Runtime[p->channel][p->rank][0] = (gu2MR18[p->channel][p->rank] & 0x00FF) | ((gu2MR19[p->channel][p->rank] & 0x00FF) <<8);
        if(p->dram_cbt_mode[p->rank]==CBT_BYTE_MODE1)
        {
            u2MR1819_Runtime[p->channel][p->rank][1] = (gu2MR18[p->channel][p->rank] >> 8) | ((gu2MR19[p->channel][p->rank] & 0xFF00));
        }
        else
        {
            u2MR1819_Runtime[p->channel][p->rank][1] = u2MR1819_Runtime[p->channel][p->rank][0];
        }
        //INC : MR1819>base. PI-
        //DEC : MR1819<base. PI+
        for(byteIdx=0; byteIdx<2; byteIdx++)
        {
            U16 deltaMR1819=0;

            if(u2MR1819_Runtime[p->channel][p->rank][byteIdx] >= u2MR1819_Base[p->channel][p->rank][byteIdx])
            {
                deltaMR1819  = u2MR1819_Runtime[p->channel][p->rank][byteIdx] - u2MR1819_Base[p->channel][p->rank][byteIdx];
                u1AdjPI[rankIdx][byteIdx] = deltaMR1819/u1DQSOSC_INC;
                for(shuIdx=0; shuIdx<=DRAM_DFS_SHUFFLE_4; shuIdx++)
                {
                    u1UpdatedPI_DQ[shuIdx][rankIdx][byteIdx] = u1OriginalPI_DQ[shuIdx][rankIdx][byteIdx] - (u1AdjPI[rankIdx][byteIdx]*u1FreqRatioTX[shuIdx]/u1FreqRatioTX[u1ShuLevel]);
                    u1UpdatedPI_DQM[shuIdx][rankIdx][byteIdx] = u1OriginalPI_DQM[shuIdx][rankIdx][byteIdx] - (u1AdjPI[rankIdx][byteIdx]*u1FreqRatioTX[shuIdx]/u1FreqRatioTX[u1ShuLevel]);
                    mcSHOW_DBG_MSG(("CH%d RK%d B%d, Base=%X Runtime=%X delta=%d INC=%d PI=0x%B Adj=%d newPI=0x%B\n", p->channel, u1GetRank(p), byteIdx
                            , u2MR1819_Base[p->channel][p->rank][byteIdx], u2MR1819_Runtime[p->channel][p->rank][byteIdx], deltaMR1819, u1DQSOSC_INC
                            , u1OriginalPI_DQ[shuIdx][rankIdx][byteIdx], (u1AdjPI[rankIdx][byteIdx]*u1FreqRatioTX[shuIdx]/u1FreqRatioTX[u1ShuLevel]), u1UpdatedPI_DQ[shuIdx][rankIdx][byteIdx]));
                }
            }
            else
            {
                deltaMR1819  = u2MR1819_Base[p->channel][p->rank][byteIdx] - u2MR1819_Runtime[p->channel][p->rank][byteIdx];
                u1AdjPI[rankIdx][byteIdx] = deltaMR1819/u1DQSOSC_DEC;
                for(shuIdx=0; shuIdx<=DRAM_DFS_SHUFFLE_4; shuIdx++)
                {
                    u1UpdatedPI_DQ[shuIdx][rankIdx][byteIdx] = u1OriginalPI_DQ[shuIdx][rankIdx][byteIdx] + (u1AdjPI[rankIdx][byteIdx]*u1FreqRatioTX[shuIdx]/u1FreqRatioTX[u1ShuLevel]);
                    u1UpdatedPI_DQM[shuIdx][rankIdx][byteIdx] = u1OriginalPI_DQM[shuIdx][rankIdx][byteIdx] + (u1AdjPI[rankIdx][byteIdx]*u1FreqRatioTX[shuIdx]/u1FreqRatioTX[u1ShuLevel]);
                    mcSHOW_DBG_MSG(("CH%d RK%d B%d, Base=%X Runtime=%X delta=%d DEC=%d PI=0x%B Adj=%d newPI=0x%B\n", p->channel, u1GetRank(p), byteIdx
                            , u2MR1819_Base[p->channel][p->rank][byteIdx], u2MR1819_Runtime[p->channel][p->rank][byteIdx], deltaMR1819, u1DQSOSC_DEC
                            , u1OriginalPI_DQ[shuIdx][rankIdx][byteIdx], (u1AdjPI[rankIdx][byteIdx]*u1FreqRatioTX[shuIdx]/u1FreqRatioTX[u1ShuLevel]), u1UpdatedPI_DQ[shuIdx][rankIdx][byteIdx]));
                }
            }
        }
    }

    vSetRank(p, RANK_0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR), 1, DQSOSCR_TXUPDMODE);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR), 1, DQSOSCR_MANUTXUPD);

    for(shuIdx=0; shuIdx<=DRAM_DFS_SHUFFLE_4; shuIdx++)
    {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7)+(0x500*shuIdx), P_Fld(u1UpdatedPI_DQ[shuIdx][RANK_0][0], SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0)
                                                                 | P_Fld(u1UpdatedPI_DQM[shuIdx][RANK_0][0], SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0));

        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ7)+(0x500*shuIdx), P_Fld(u1UpdatedPI_DQ[shuIdx][RANK_0][1], SHU1_R0_B1_DQ7_RK0_ARPI_DQ_B1)
                                                                 | P_Fld(u1UpdatedPI_DQM[shuIdx][RANK_0][1], SHU1_R0_B1_DQ7_RK0_ARPI_DQM_B1));

        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R1_B0_DQ7)+(0x500*shuIdx), P_Fld(u1UpdatedPI_DQ[shuIdx][RANK_1][0], SHU1_R1_B0_DQ7_RK1_ARPI_DQ_B0)
                                                                 | P_Fld(u1UpdatedPI_DQM[shuIdx][RANK_1][0], SHU1_R1_B0_DQ7_RK1_ARPI_DQM_B0));

        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R1_B1_DQ7)+(0x500*shuIdx), P_Fld(u1UpdatedPI_DQ[shuIdx][RANK_1][1], SHU1_R1_B1_DQ7_RK1_ARPI_DQ_B1)
                                                                 | P_Fld(u1UpdatedPI_DQM[shuIdx][RANK_1][1], SHU1_R1_B1_DQ7_RK1_ARPI_DQM_B1));
    }
    while(u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MISC_STATUSA),MISC_STATUSA_MANUTXUPD_DONE)!=1)
    {
        mcDELAY_US(1);
    }
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR), 0, DQSOSCR_TXUPDMODE);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR), 0, DQSOSCR_MANUTXUPD);


    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), u1MR4OnOff, SPCMDCTRL_REFRDIS);
}
#endif

#if ENABLE_RX_TRACKING_LP4
void DramcRxInputDelayTrackingInit_Common(DRAMC_CTX_T *p)
{
    U8 ii, backup_rank;

    backup_rank = u1GetRank(p);

    //Enable RX_FIFO macro DIV4 clock CG
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_MISC_CG_CTRL1), 0xffffffff, MISC_CG_CTRL1_R_DVS_DIV4_CG_CTRL);

    //DVS mode to RG mode
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS2),0x0, R0_B0_RXDVS2_R_RK0_DVS_MODE_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2),0x0, R0_B1_RXDVS2_R_RK0_DVS_MODE_B1);

    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R1_B0_RXDVS2),0x0, R1_B0_RXDVS2_R_RK1_DVS_MODE_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R1_B1_RXDVS2),0x0, R1_B1_RXDVS2_R_RK1_DVS_MODE_B1);

    //Tracking lead/lag counter >> Rx DLY adjustment fixed to 1
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_RXDVS0),0x0, B0_RXDVS0_R_DMRXDVS_CNTCMP_OPT_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_RXDVS0),0x0, B1_RXDVS0_R_DMRXDVS_CNTCMP_OPT_B1);

    //DQIEN pre-state option to block update for RX ASVA  1-2
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_RXDVS0),0x1, B0_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_RXDVS0),0x1, B1_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_B1);

    //Turn off F_DLY individual calibration option (CTO_AGENT_RDAT cannot separate DR/DF error)
#if SUPPORT_HYNIX_RX_DQS_WEAK_PULL
    if (u1IsLP4Family(p->dram_type) && (p->vendor_id == VENDOR_HYNIX))
    {
        //tracking rising and update rising/falling individual
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS2),0x1, R0_B0_RXDVS2_R_RK0_DVS_FDLY_MODE_B0);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2),0x1, R0_B1_RXDVS2_R_RK0_DVS_FDLY_MODE_B1);

        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R1_B0_RXDVS2),0x1, R1_B0_RXDVS2_R_RK1_DVS_FDLY_MODE_B0);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R1_B1_RXDVS2),0x1, R1_B1_RXDVS2_R_RK1_DVS_FDLY_MODE_B1);
    }
    else
#endif
    {
        //tracking rising and update rising/falling together
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS2),0x0, R0_B0_RXDVS2_R_RK0_DVS_FDLY_MODE_B0);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2),0x0, R0_B1_RXDVS2_R_RK0_DVS_FDLY_MODE_B1);

        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R1_B0_RXDVS2),0x0, R1_B0_RXDVS2_R_RK1_DVS_FDLY_MODE_B0);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R1_B1_RXDVS2),0x0, R1_B1_RXDVS2_R_RK1_DVS_FDLY_MODE_B1);
    }

    for(ii=RANK_0; ii<RANK_MAX; ii++)
    {
        vSetRank(p, ii);

        //DQ/DQM/DQS DLY MAX/MIN value under Tracking mode
        /* Byte 0 */
#if (fcFOR_CHIP_ID == fcBianco)
        /* DQS, DQ, DQM (DQ, DQM are tied together now) -> controlled using DQM MAX_MIN */
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS7), P_Fld(0x0, R0_B0_RXDVS7_RG_RK0_ARDQM0_MIN_DLY_B0) | P_Fld(0x3f, R0_B0_RXDVS7_RG_RK0_ARDQM0_MAX_DLY_B0)
                                                                  | P_Fld(0x0, R0_B0_RXDVS7_RG_RK0_ARDQS0_MIN_DLY_B0) | P_Fld(0x7f, R0_B0_RXDVS7_RG_RK0_ARDQS0_MAX_DLY_B0));
#else
        /* Previous design DQ, DQM MAX/MIN are controlled using different register fields */
        /* DQM, DQS */
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS7), P_Fld(0x0, R0_B0_RXDVS7_RG_RK0_ARDQM0_MIN_DLY_B0) | P_Fld(0x3f, R0_B0_RXDVS7_RG_RK0_ARDQM0_MAX_DLY_B0)
                                                                  | P_Fld(0x0, R0_B0_RXDVS7_RG_RK0_ARDQS0_MIN_DLY_B0) | P_Fld(0x7f, R0_B0_RXDVS7_RG_RK0_ARDQS0_MAX_DLY_B0));
        /* DQ */
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS3), P_Fld(0x0, R0_B0_RXDVS3_RG_RK0_ARDQ0_MIN_DLY_B0) | P_Fld(0x3f, R0_B0_RXDVS3_RG_RK0_ARDQ0_MAX_DLY_B0));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS3), P_Fld(0x0, R0_B0_RXDVS3_RG_RK0_ARDQ1_MIN_DLY_B0) | P_Fld(0x3f, R0_B0_RXDVS3_RG_RK0_ARDQ1_MAX_DLY_B0));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS4), P_Fld(0x0, R0_B0_RXDVS4_RG_RK0_ARDQ2_MIN_DLY_B0) | P_Fld(0x3f, R0_B0_RXDVS4_RG_RK0_ARDQ2_MAX_DLY_B0));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS4), P_Fld(0x0, R0_B0_RXDVS4_RG_RK0_ARDQ3_MIN_DLY_B0) | P_Fld(0x3f, R0_B0_RXDVS4_RG_RK0_ARDQ3_MAX_DLY_B0));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS5), P_Fld(0x0, R0_B0_RXDVS5_RG_RK0_ARDQ4_MIN_DLY_B0) | P_Fld(0x3f, R0_B0_RXDVS5_RG_RK0_ARDQ4_MAX_DLY_B0));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS5), P_Fld(0x0, R0_B0_RXDVS5_RG_RK0_ARDQ5_MIN_DLY_B0) | P_Fld(0x3f, R0_B0_RXDVS5_RG_RK0_ARDQ5_MAX_DLY_B0));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS6), P_Fld(0x0, R0_B0_RXDVS6_RG_RK0_ARDQ6_MIN_DLY_B0) | P_Fld(0x3f, R0_B0_RXDVS6_RG_RK0_ARDQ6_MAX_DLY_B0));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS6), P_Fld(0x0, R0_B0_RXDVS6_RG_RK0_ARDQ7_MIN_DLY_B0) | P_Fld(0x3f, R0_B0_RXDVS6_RG_RK0_ARDQ7_MAX_DLY_B0));
#endif

        /* Byte 1 */
#if (fcFOR_CHIP_ID == fcBianco)
        /* DQS, DQ, DQM (DQ, DQM are tied together now) -> controlled using DQM MAX_MIN */
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS7), P_Fld(0x0, R0_B1_RXDVS7_RG_RK0_ARDQM0_MIN_DLY_B1) | P_Fld(0x3f, R0_B1_RXDVS7_RG_RK0_ARDQM0_MAX_DLY_B1)
                                                                  | P_Fld(0x0, R0_B1_RXDVS7_RG_RK0_ARDQS0_MIN_DLY_B1) | P_Fld(0x7f, R0_B1_RXDVS7_RG_RK0_ARDQS0_MAX_DLY_B1));
#else
        /* Previous design DQ, DQM MAX/MIN are controlled using different register fields */
        /* DQM, DQS */
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS7), P_Fld(0x0, R0_B1_RXDVS7_RG_RK0_ARDQM0_MIN_DLY_B1) | P_Fld(0x3f, R0_B1_RXDVS7_RG_RK0_ARDQM0_MAX_DLY_B1) 
                                                                  | P_Fld(0x0, R0_B1_RXDVS7_RG_RK0_ARDQS0_MIN_DLY_B1) | P_Fld(0x7f, R0_B1_RXDVS7_RG_RK0_ARDQS0_MAX_DLY_B1));
        /* DQ */
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS3), P_Fld(0x0, R0_B1_RXDVS3_RG_RK0_ARDQ0_MIN_DLY_B1) | P_Fld(0x3f, R0_B1_RXDVS3_RG_RK0_ARDQ0_MAX_DLY_B1));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS3), P_Fld(0x0, R0_B1_RXDVS3_RG_RK0_ARDQ1_MIN_DLY_B1) | P_Fld(0x3f, R0_B1_RXDVS3_RG_RK0_ARDQ1_MAX_DLY_B1));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS4), P_Fld(0x0, R0_B1_RXDVS4_RG_RK0_ARDQ2_MIN_DLY_B1) | P_Fld(0x3f, R0_B1_RXDVS4_RG_RK0_ARDQ2_MAX_DLY_B1));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS4), P_Fld(0x0, R0_B1_RXDVS4_RG_RK0_ARDQ3_MIN_DLY_B1) | P_Fld(0x3f, R0_B1_RXDVS4_RG_RK0_ARDQ3_MAX_DLY_B1));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS5), P_Fld(0x0, R0_B1_RXDVS5_RG_RK0_ARDQ4_MIN_DLY_B1) | P_Fld(0x3f, R0_B1_RXDVS5_RG_RK0_ARDQ4_MAX_DLY_B1));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS5), P_Fld(0x0, R0_B1_RXDVS5_RG_RK0_ARDQ5_MIN_DLY_B1) | P_Fld(0x3f, R0_B1_RXDVS5_RG_RK0_ARDQ5_MAX_DLY_B1));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS6), P_Fld(0x0, R0_B1_RXDVS6_RG_RK0_ARDQ6_MIN_DLY_B1) | P_Fld(0x3f, R0_B1_RXDVS6_RG_RK0_ARDQ6_MAX_DLY_B1));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS6), P_Fld(0x0, R0_B1_RXDVS6_RG_RK0_ARDQ7_MIN_DLY_B1) | P_Fld(0x3f, R0_B1_RXDVS6_RG_RK0_ARDQ7_MAX_DLY_B1));
#endif

        //Threshold for LEAD/LAG filter
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS1), P_Fld(0x2, R0_B0_RXDVS1_R_RK0_B0_DVS_TH_LEAD) | P_Fld(0x2, R0_B0_RXDVS1_R_RK0_B0_DVS_TH_LAG));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS1), P_Fld(0x2, R0_B1_RXDVS1_R_RK0_B1_DVS_TH_LEAD) | P_Fld(0x2, R0_B1_RXDVS1_R_RK0_B1_DVS_TH_LAG));

        //DQ/DQS Rx DLY adjustment for tracking mode
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS2), P_Fld(0x0, R0_B0_RXDVS2_R_RK0_RX_DLY_RIS_DQ_SCALE_B0) | P_Fld(0x0, R0_B0_RXDVS2_R_RK0_RX_DLY_RIS_DQS_SCALE_B0));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2), P_Fld(0x0, R0_B1_RXDVS2_R_RK0_RX_DLY_RIS_DQ_SCALE_B1) | P_Fld(0x0, R0_B1_RXDVS2_R_RK0_RX_DLY_RIS_DQS_SCALE_B1));

        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS2), P_Fld(0x0, R0_B0_RXDVS2_R_RK0_RX_DLY_FAL_DQ_SCALE_B0) | P_Fld(0x0, R0_B0_RXDVS2_R_RK0_RX_DLY_FAL_DQS_SCALE_B0));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2), P_Fld(0x0, R0_B1_RXDVS2_R_RK0_RX_DLY_FAL_DQ_SCALE_B1) | P_Fld(0x0, R0_B1_RXDVS2_R_RK0_RX_DLY_FAL_DQS_SCALE_B1));
    }

    vSetRank(p, backup_rank);
}
#endif

void DramcRxInputDelayTrackingInit_byFreq(DRAMC_CTX_T *p)
{
    U8 u1DVS_Delay;
    //Monitor window size setting
    //DDRPHY.SHU*_B*_DQ5.RG_RX_ARDQS0_DVS_DLY_B* (suggested value from A-PHY owner)
//WHITNEY_TO_BE_PORTING
#if (fcFOR_CHIP_ID == fcBianco)
    //          Speed   Voltage     DVS_DLY
    //======================================
    //SHU1      3200    0.8V        3
    //SHU2      2667    0.8V-0.7V   4
    //SHU3      1600    0.7V-0.65V  5
    if(p->freqGroup == 1600)
    {
        u1DVS_Delay =3;
    }
    else if(p->freqGroup == 1333 || p->freqGroup == 1200)
    {
        u1DVS_Delay =4;
    }
    else// if(p->freqGroup == 800)
    {
        u1DVS_Delay =5;
    }
#endif

    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ5), u1DVS_Delay, SHU1_B0_DQ5_RG_RX_ARDQS_DVS_DLY_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DQ5), u1DVS_Delay, SHU1_B1_DQ5_RG_RX_ARDQS_DVS_DLY_B1);

    /* Bianco HW design issue: run-time PBYTE flag will lose it's function and become per-bit -> set to 0 */
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ7), P_Fld(0x0, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0)
                                                            | P_Fld(0x0, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0));
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DQ7), P_Fld(0x0, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1)
                                                            | P_Fld(0x0, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1));
}

#if ENABLE_RX_TRACKING_LP4
void DramcRxInputDelayTrackingHW(DRAMC_CTX_T *p)
{
    DRAM_CHANNEL_T channel_bak = p->channel;
    UINT8 updateDone=0;
    U16 u2DVS_TH=0x0;
    U16 u2MinDly=0x14;
    U16 u2MaxDly=0x30;
    U8 ii, backup_rank;

    vSetPHY2ChannelMapping(p, CHANNEL_A);
    backup_rank = u1GetRank(p);

    //Rx DLY tracking setting (Static)
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_B0_RXDVS0),
                                           P_Fld(1, B0_RXDVS0_R_RX_DLY_TRACK_SPM_CTRL_B0) |
                                           P_Fld(0, B0_RXDVS0_R_RX_RANKINCTL_B0)|
                                           P_Fld(1, B0_RXDVS0_R_RX_RANKINSEL_B0));

    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_B1_RXDVS0),
                                           P_Fld(1, B1_RXDVS0_R_RX_DLY_TRACK_SPM_CTRL_B1) |
                                           P_Fld(0, B1_RXDVS0_R_RX_RANKINCTL_B1)|
                                           P_Fld(1, B1_RXDVS0_R_RX_RANKINSEL_B1));

#if (fcFOR_CHIP_ID == fcBianco)
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ9), P_Fld(0x1, B0_DQ9_R_DMRXDVS_RDSEL_LAT_B0 | P_Fld(0, B0_DQ9_R_DMRXDVS_VALID_LAT_B0)));
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ9), P_Fld(0x1, B1_DQ9_R_DMRXDVS_RDSEL_LAT_B1) | P_Fld(0, B1_DQ9_R_DMRXDVS_VALID_LAT_B1));
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_CA_CMD10), P_Fld(0,CA_CMD10_R_DMRXDVS_RDSEL_LAT_CA) | P_Fld(0, CA_CMD10_R_DMRXDVS_VALID_LAT_CA)); 

    /* DMRXTRACK_DQM_B* (rxdly_track SM DQM enable) -> need to be set to 1 if R_DBI is on
     *  They are shuffle regs -> move setting to DramcSetting_Olympus_LP4_ByteMode()
     */

    //Enable A-PHY DVS LEAD/LAG
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ5), 0x1, B0_DQ5_RG_RX_ARDQS_DVS_EN_B0);                                                              
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ5), 0x1, B1_DQ5_RG_RX_ARDQS_DVS_EN_B1);
#else
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_MISC_RXDVS2), P_Fld(0x1, MISC_RXDVS2_R_RXDVS_RDSEL_TOG_LAT)
                                                            | P_Fld(0x2, MISC_RXDVS2_R_RXDVS_RDSEL_BUS_LAT));
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 0x0, MISC_CTRL1_R_DMDQMDBI);

    //Enable A-PHY DVS LEAD/LAG
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ5), 0x1, B0_DQ5_RG_RX_ARDQS0_DVS_EN_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ5), 0x1, B1_DQ5_RG_RX_ARDQS0_DVS_EN_B1);
#endif    

    //Rx DLY tracking function CG enable
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_RXDVS0), 0x1, B0_RXDVS0_R_RX_DLY_TRACK_CG_EN_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_RXDVS0), 0x1, B1_RXDVS0_R_RX_DLY_TRACK_CG_EN_B1);

    //Rx DLY tracking lead/lag counter enable
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_RXDVS0), 0x1, B0_RXDVS0_R_RX_DLY_TRACK_ENA_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_RXDVS0), 0x1, B1_RXDVS0_R_RX_DLY_TRACK_ENA_B1);

    for(ii=RANK_0; ii<RANK_MAX; ii++)
    {
        vSetRank(p, ii);

        //Rx DLY tracking update enable (HW mode)
        #if 0
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS2), 0x2, R0_B0_RXDVS2_R_RK0_DVS_MODE_B0);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2), 0x2, R0_B1_RXDVS2_R_RK0_DVS_MODE_B1);

        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS2), 0x1, R0_B0_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B0);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2), 0x1, R0_B1_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B1);

        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS2), 0x1, R0_B0_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B0);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2), 0x1, R0_B1_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B1);
        #else
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS2),
                                               P_Fld(2, R0_B0_RXDVS2_R_RK0_DVS_MODE_B0) |
                                               P_Fld(1, R0_B0_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B0)|
                                               P_Fld(1, R0_B0_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B0));

        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2),
                                               P_Fld(2, R0_B1_RXDVS2_R_RK0_DVS_MODE_B1) |
                                               P_Fld(1, R0_B1_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B1)|
                                               P_Fld(1, R0_B1_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B1));
        #endif
    }

    vSetRank(p, backup_rank);

#if 0
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_RXDVS0),0x1, B0_RXDVS0_R_RX_DLY_RK_OPT_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_RXDVS0),0x1, B1_RXDVS0_R_RX_DLY_RK_OPT_B1);

    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 1, MISC_CTRL1_R_HWSAVE_MODE_ENA);
#endif

    vSetPHY2ChannelMapping(p, channel_bak);
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
    U32 u4TimeCnt;

    mcSHOW_DBG_MSG3(("[ZQCalibration]\n"));
    mcFPRINTF((fp_A60501, "[ZQCalibration]\n"));
    //!!R_DMMPCRK(R_DMMPCRKEN=1) specify rank0 or rank1

    u4TimeCnt = TIME_OUT_CNT;

    //Use rank swap or MPCRK to select rank
    //DramcRankSwap(p, p->rank);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1GetRank(p), MRS_MPCRK);
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

    // recover rank swap to "no swap"
    //DramcRankSwap(p, RANK_0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), 0, MRS_MPCRK);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MPC_OPTION), 1, MPC_OPTION_MPCRKEN);

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
    U32 u4BakReg[9];
    S8 iOffset, iDQFlagChange[16], iDQMFlagChange[2];
    U32 u4Value, u4RestltDQ[2], u4RestltDQM[2];
    U8 u1BitIdx, u1ByteIdx, u1FinishCount, u1DQFinalFlagChange[16], u1DQMFinalFlagChange[2];

    mcSHOW_DBG_MSG(("\n[RXInputBufferOffsetCal]\n"));
    mcFPRINTF((fp_A60501, "\n[RXInputBufferOffsetCal] \n"));

    if(!u1IsLP4Family(p->dram_type))
    {
        //RX offset calibration(LP4 only)
        mcSHOW_ERR_MSG(("LP3 no need\n"));
        return DRAM_FAIL;
    }

    //Back up dramC register
    u4BakReg[0] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_B0_DQ5));
    u4BakReg[1] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_B1_DQ5));
    u4BakReg[2] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_B0_DQ6));
    u4BakReg[3] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_B1_DQ6));
    u4BakReg[4] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ5));
    u4BakReg[5] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DQ5));
    u4BakReg[6] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_B0_DQ3));
    u4BakReg[7] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_B1_DQ3));
    u4BakReg[8] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_PADCTRL));

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
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ5), 0xe, SHU1_B0_DQ5_RG_RX_ARDQ_VREF_SEL_B0);  // LP4 and LP4x with term: 0xe
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DQ5), 0xe, SHU1_B1_DQ5_RG_RX_ARDQ_VREF_SEL_B1);  // LP4 and LP4x with term: 0xe
    }
    else
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ5), 0x16, SHU1_B0_DQ5_RG_RX_ARDQ_VREF_SEL_B0);  // LP4 and LP4x with term: 0xe
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DQ5), 0x16, SHU1_B1_DQ5_RG_RX_ARDQ_VREF_SEL_B1);  // LP4 and LP4x with term: 0xe
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
        u4RestltDQ[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_PHY_RGS0), MISC_PHY_RGS0_RGS_ARDQ_OFFSET_FLAG_B0);
        u4RestltDQ[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_PHY_RGS0), MISC_PHY_RGS0_RGS_ARDQ_OFFSET_FLAG_B1);
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
        u4RestltDQM[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_PHY_RGS0), MISC_PHY_RGS0_RGS_ARDQM0_OFFSET_FLAG_B0);
        u4RestltDQM[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_PHY_RGS0), MISC_PHY_RGS0_RGS_ARDQM0_OFFSET_FLAG_B1);

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
    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B0_DQ5), u4BakReg[0]);
    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B1_DQ5), u4BakReg[1]);
    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), u4BakReg[2]);
    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), u4BakReg[3]);
    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ5), u4BakReg[4]);
    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DQ5), u4BakReg[5]);
    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B0_DQ3), u4BakReg[6]);
    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B1_DQ3), u4BakReg[7]);
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_PADCTRL), u4BakReg[8]);

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

#if SIMULATION_SW_IMPED
void DramcSwImpedanceSaveRegister(DRAMC_CTX_T *p, U8 ca_term_option, U8 dq_term_option, U8 save_to_where)
{
    U8 u1Hysteresis;

    if(u1IsLP4Family(p->dram_type))
    {
         //DQ
         vIO32WriteFldAlign_All((DDRPHY_SHU1_B0_DQ1 + save_to_where*0x500), gDramcSwImpedanceResule[dq_term_option][0], SHU1_B0_DQ1_RG_TX_ARDQ_DRVP_B0);
         vIO32WriteFldAlign_All((DDRPHY_SHU1_B0_DQ1 + save_to_where*0x500), gDramcSwImpedanceResule[dq_term_option][1], SHU1_B0_DQ1_RG_TX_ARDQ_DRVN_B0);
         vIO32WriteFldAlign_All((DDRPHY_SHU1_B0_DQ1 + save_to_where*0x500), gDramcSwImpedanceResule[dq_term_option][3], SHU1_B0_DQ1_RG_TX_ARDQ_ODTN_B0);
         vIO32WriteFldAlign_All((DDRPHY_SHU1_B1_DQ1 + save_to_where*0x500), gDramcSwImpedanceResule[dq_term_option][0], SHU1_B1_DQ1_RG_TX_ARDQ_DRVP_B1);
         vIO32WriteFldAlign_All((DDRPHY_SHU1_B1_DQ1 + save_to_where*0x500), gDramcSwImpedanceResule[dq_term_option][1], SHU1_B1_DQ1_RG_TX_ARDQ_DRVN_B1);
         vIO32WriteFldAlign_All((DDRPHY_SHU1_B1_DQ1 + save_to_where*0x500), gDramcSwImpedanceResule[dq_term_option][3], SHU1_B1_DQ1_RG_TX_ARDQ_ODTN_B1);

         vIO32WriteFldMulti_All((DRAMC_REG_SHU1_DRVING1 + save_to_where*0x600), P_Fld(gDramcSwImpedanceResule[dq_term_option][0], SHU1_DRVING1_DQDRVP2) | P_Fld(gDramcSwImpedanceResule[dq_term_option][1], SHU1_DRVING1_DQDRVN2));
         vIO32WriteFldMulti_All((DRAMC_REG_SHU1_DRVING2 + save_to_where*0x600), P_Fld(gDramcSwImpedanceResule[dq_term_option][0], SHU1_DRVING2_DQDRVP1) | P_Fld(gDramcSwImpedanceResule[dq_term_option][1], SHU1_DRVING2_DQDRVN1));
         vIO32WriteFldMulti_All((DRAMC_REG_SHU1_DRVING3 + save_to_where*0x600), P_Fld(gDramcSwImpedanceResule[dq_term_option][2], SHU1_DRVING3_DQODTP2) | P_Fld(gDramcSwImpedanceResule[dq_term_option][3], SHU1_DRVING3_DQODTN2));
         vIO32WriteFldMulti_All((DRAMC_REG_SHU1_DRVING4 + save_to_where*0x600), P_Fld(gDramcSwImpedanceResule[dq_term_option][2], SHU1_DRVING4_DQODTP1) | P_Fld(gDramcSwImpedanceResule[dq_term_option][3], SHU1_DRVING4_DQODTN1));

         //DQS
         vIO32WriteFldAlign_All((DDRPHY_SHU1_B0_DQ2 + save_to_where*0x500), gDramcSwImpedanceResule[dq_term_option][0], SHU1_B0_DQ2_RG_TX_ARDQS_DRVP_B0);
         vIO32WriteFldAlign_All((DDRPHY_SHU1_B0_DQ2 + save_to_where*0x500), gDramcSwImpedanceResule[dq_term_option][1], SHU1_B0_DQ2_RG_TX_ARDQS_DRVN_B0);
         vIO32WriteFldAlign_All((DDRPHY_SHU1_B0_DQ2 + save_to_where*0x500), gDramcSwImpedanceResule[dq_term_option][3], SHU1_B0_DQ2_RG_TX_ARDQS_ODTN_B0);
         vIO32WriteFldAlign_All((DDRPHY_SHU1_B1_DQ2 + save_to_where*0x500), gDramcSwImpedanceResule[dq_term_option][0], SHU1_B1_DQ2_RG_TX_ARDQS_DRVP_B1);
         vIO32WriteFldAlign_All((DDRPHY_SHU1_B1_DQ2 + save_to_where*0x500), gDramcSwImpedanceResule[dq_term_option][1], SHU1_B1_DQ2_RG_TX_ARDQS_DRVN_B1);
         vIO32WriteFldAlign_All((DDRPHY_SHU1_B1_DQ2 + save_to_where*0x500), gDramcSwImpedanceResule[dq_term_option][3], SHU1_B1_DQ2_RG_TX_ARDQS_ODTN_B1);

#if SUPPORT_HYNIX_RX_DQS_WEAK_PULL
         if (u1IsLP4Family(p->dram_type) && (p->vendor_id == VENDOR_HYNIX))
         {  U32 temp_value[4];
            int i;
            for(i=0; i<4; i++)
            {
                if ((gDramcSwImpedanceResule[dq_term_option][i] + 2) > 15) temp_value[i] = (gDramcSwImpedanceResule[dq_term_option][i] + 2) - 16 + 29;
                else temp_value[i] = gDramcSwImpedanceResule[dq_term_option][i] + 2;
            }
             vIO32WriteFldMulti_All((DRAMC_REG_SHU1_DRVING1 + save_to_where*0x600), P_Fld(temp_value[0], SHU1_DRVING1_DQSDRVP2) | P_Fld(temp_value[1], SHU1_DRVING1_DQSDRVN2));
             vIO32WriteFldMulti_All((DRAMC_REG_SHU1_DRVING1 + save_to_where*0x600), P_Fld(temp_value[0], SHU1_DRVING1_DQSDRVP1) | P_Fld(temp_value[1], SHU1_DRVING1_DQSDRVN1));
             vIO32WriteFldMulti_All((DRAMC_REG_SHU1_DRVING3 + save_to_where*0x600), P_Fld(temp_value[2], SHU1_DRVING3_DQSODTP2) | P_Fld(temp_value[3], SHU1_DRVING3_DQSODTN2));
             vIO32WriteFldMulti_All((DRAMC_REG_SHU1_DRVING3 + save_to_where*0x600), P_Fld(temp_value[2], SHU1_DRVING3_DQSODTP) | P_Fld(temp_value[3], SHU1_DRVING3_DQSODTN));
         }
         else
#endif
         {
             vIO32WriteFldMulti_All((DRAMC_REG_SHU1_DRVING1 + save_to_where*0x600), P_Fld(gDramcSwImpedanceResule[dq_term_option][0], SHU1_DRVING1_DQSDRVP2) | P_Fld(gDramcSwImpedanceResule[dq_term_option][1], SHU1_DRVING1_DQSDRVN2));
             vIO32WriteFldMulti_All((DRAMC_REG_SHU1_DRVING1 + save_to_where*0x600), P_Fld(gDramcSwImpedanceResule[dq_term_option][0], SHU1_DRVING1_DQSDRVP1) | P_Fld(gDramcSwImpedanceResule[dq_term_option][1], SHU1_DRVING1_DQSDRVN1));
             vIO32WriteFldMulti_All((DRAMC_REG_SHU1_DRVING3 + save_to_where*0x600), P_Fld(gDramcSwImpedanceResule[dq_term_option][2], SHU1_DRVING3_DQSODTP2) | P_Fld(gDramcSwImpedanceResule[dq_term_option][3], SHU1_DRVING3_DQSODTN2));
             vIO32WriteFldMulti_All((DRAMC_REG_SHU1_DRVING3 + save_to_where*0x600), P_Fld(gDramcSwImpedanceResule[dq_term_option][2], SHU1_DRVING3_DQSODTP) | P_Fld(gDramcSwImpedanceResule[dq_term_option][3], SHU1_DRVING3_DQSODTN));
         }

         //CMD
         vIO32WriteFldAlign_All((DDRPHY_SHU1_CA_CMD1 + save_to_where*0x500), gDramcSwImpedanceResule[ca_term_option][0], SHU1_CA_CMD1_RG_TX_ARCMD_DRVP);
         vIO32WriteFldAlign_All((DDRPHY_SHU1_CA_CMD1 + save_to_where*0x500), gDramcSwImpedanceResule[ca_term_option][1], SHU1_CA_CMD1_RG_TX_ARCMD_DRVN);

         vIO32WriteFldMulti_All((DRAMC_REG_SHU1_DRVING2 + save_to_where*0x600), P_Fld(gDramcSwImpedanceResule[ca_term_option][0], SHU1_DRVING2_CMDDRVP2) | P_Fld(gDramcSwImpedanceResule[ca_term_option][1], SHU1_DRVING2_CMDDRVN2));
         vIO32WriteFldMulti_All((DRAMC_REG_SHU1_DRVING2 + save_to_where*0x600), P_Fld(gDramcSwImpedanceResule[ca_term_option][0], SHU1_DRVING2_CMDDRVP1) | P_Fld(gDramcSwImpedanceResule[ca_term_option][1], SHU1_DRVING2_CMDDRVN1));
         vIO32WriteFldMulti_All((DRAMC_REG_SHU1_DRVING4 + save_to_where*0x600), P_Fld(gDramcSwImpedanceResule[ca_term_option][2], SHU1_DRVING4_CMDODTP2) | P_Fld(gDramcSwImpedanceResule[ca_term_option][3], SHU1_DRVING4_CMDODTN2));
         vIO32WriteFldMulti_All((DRAMC_REG_SHU1_DRVING4 + save_to_where*0x600), P_Fld(gDramcSwImpedanceResule[ca_term_option][2], SHU1_DRVING4_CMDODTP1) | P_Fld(gDramcSwImpedanceResule[ca_term_option][3], SHU1_DRVING4_CMDODTN1));

         //CLK
         vIO32WriteFldAlign_All((DDRPHY_SHU1_CA_CMD2 + save_to_where*0x500), gDramcSwImpedanceResule[ca_term_option][0], SHU1_CA_CMD2_RG_TX_ARCLK_DRVP);
         vIO32WriteFldAlign_All((DDRPHY_SHU1_CA_CMD2 + save_to_where*0x500), gDramcSwImpedanceResule[ca_term_option][1], SHU1_CA_CMD2_RG_TX_ARCLK_DRVN);

         //RG_TX_*RCKE_DRVP/RG_TX_*RCKE_DRVN doesn't set, so set 0xA first 
         vIO32WriteFldAlign_All((DDRPHY_SHU1_CA_CMD11 + save_to_where*0x500), gDramcSwImpedanceResule[ca_term_option][0], SHU1_CA_CMD11_RG_TX_ARCKE_DRVP);
         vIO32WriteFldAlign_All((DDRPHY_SHU1_CA_CMD11 + save_to_where*0x500), gDramcSwImpedanceResule[ca_term_option][1], SHU1_CA_CMD11_RG_TX_ARCKE_DRVN);

         //CKE
         // CKE is full swing.
         // LP4/LP4X set DRVP/DRVN as LP3's default value
         if(u1GetICVersion_Bianco()>=IC_VERSION_E3)
         {     
             u1Hysteresis = (p->frequency > 933) ? 0x7 : 0x18;
         }
         else
         {
             // DRVP=8
             u1Hysteresis = 8;
         }
         //DRVP[4:0] = RG_TX_ARCMD_PU_PRE<1:0>, RG_TX_ARCLK_DRVN_PRE<2:0>
         vIO32WriteFldAlign((DDRPHY_SHU1_CA_CMD3 + save_to_where*0x500), (u1Hysteresis>>3)&0x3, SHU1_CA_CMD3_RG_TX_ARCMD_PU_PRE);
         vIO32WriteFldAlign((DDRPHY_SHU1_CA_CMD0 + save_to_where*0x500), u1Hysteresis&0x7, SHU1_CA_CMD0_RG_TX_ARCLK_DRVN_PRE);
         mcSHOW_DBG_MSG(("CKE DRVP=%d\n", u1Hysteresis));

         // DRVN=9
         //DRVN[4:0] = RG_ARCMD_REV<12:8>
         vIO32WriteFldAlign((DDRPHY_SHU1_CA_CMD3 + save_to_where*0x500), 9, SHU1_CA_CMD3_RG_ARCMD_REV_CKE_DRVN);
    }
#if ENABLE_LP3_SW
    else
    {
         //DQ
         vIO32WriteFldAlign_All((DDRPHY_SHU1_B0_DQ1 + save_to_where*0x500), gDramcSwImpedanceResule[dq_term_option][0], SHU1_B0_DQ1_RG_TX_ARDQ_DRVP_B0);
         vIO32WriteFldAlign_All((DDRPHY_SHU1_B0_DQ1 + save_to_where*0x500), gDramcSwImpedanceResule[dq_term_option][1], SHU1_B0_DQ1_RG_TX_ARDQ_DRVN_B0);
         vIO32WriteFldAlign_All((DDRPHY_SHU1_B1_DQ1 + save_to_where*0x500), gDramcSwImpedanceResule[dq_term_option][0], SHU1_B1_DQ1_RG_TX_ARDQ_DRVP_B1);
         vIO32WriteFldAlign_All((DDRPHY_SHU1_B1_DQ1 + save_to_where*0x500), gDramcSwImpedanceResule[dq_term_option][1], SHU1_B1_DQ1_RG_TX_ARDQ_DRVN_B1);

         vIO32WriteFldMulti_All((DRAMC_REG_SHU1_DRVING1 + save_to_where*0x600), P_Fld(gDramcSwImpedanceResule[dq_term_option][0], SHU1_DRVING1_DQDRVP2) | P_Fld(gDramcSwImpedanceResule[dq_term_option][1], SHU1_DRVING1_DQDRVN2));
         vIO32WriteFldMulti_All((DRAMC_REG_SHU1_DRVING2 + save_to_where*0x600), P_Fld(gDramcSwImpedanceResule[dq_term_option][0], SHU1_DRVING2_DQDRVP1) | P_Fld(gDramcSwImpedanceResule[dq_term_option][1], SHU1_DRVING2_DQDRVN1));

         //DQS
         vIO32WriteFldAlign_All((DDRPHY_SHU1_B0_DQ2 + save_to_where*0x500), gDramcSwImpedanceResule[dq_term_option][0], SHU1_B0_DQ2_RG_TX_ARDQS_DRVP_B0);
         vIO32WriteFldAlign_All((DDRPHY_SHU1_B0_DQ2 + save_to_where*0x500), gDramcSwImpedanceResule[dq_term_option][1], SHU1_B0_DQ2_RG_TX_ARDQS_DRVN_B0);
         vIO32WriteFldAlign_All((DDRPHY_SHU1_B1_DQ2 + save_to_where*0x500), gDramcSwImpedanceResule[dq_term_option][0], SHU1_B1_DQ2_RG_TX_ARDQS_DRVP_B1);
         vIO32WriteFldAlign_All((DDRPHY_SHU1_B1_DQ2 + save_to_where*0x500), gDramcSwImpedanceResule[dq_term_option][1], SHU1_B1_DQ2_RG_TX_ARDQS_DRVN_B1);

         vIO32WriteFldMulti_All((DRAMC_REG_SHU1_DRVING1 + save_to_where*0x600), P_Fld(gDramcSwImpedanceResule[dq_term_option][0], SHU1_DRVING1_DQSDRVP2) | P_Fld(gDramcSwImpedanceResule[dq_term_option][1], SHU1_DRVING1_DQSDRVN2));
         vIO32WriteFldMulti_All((DRAMC_REG_SHU1_DRVING1 + save_to_where*0x600), P_Fld(gDramcSwImpedanceResule[dq_term_option][0], SHU1_DRVING1_DQSDRVP1) | P_Fld(gDramcSwImpedanceResule[dq_term_option][1], SHU1_DRVING1_DQSDRVN1));

        //CMD
         vIO32WriteFldAlign_All((DDRPHY_SHU1_CA_CMD1 + save_to_where*0x500), gDramcSwImpedanceResule[ca_term_option][0], SHU1_CA_CMD1_RG_TX_ARCMD_DRVP);
         vIO32WriteFldAlign_All((DDRPHY_SHU1_CA_CMD1 + save_to_where*0x500), gDramcSwImpedanceResule[ca_term_option][1], SHU1_CA_CMD1_RG_TX_ARCMD_DRVN);

         vIO32WriteFldMulti_All((DRAMC_REG_SHU1_DRVING2 + save_to_where*0x600), P_Fld(gDramcSwImpedanceResule[ca_term_option][0], SHU1_DRVING2_CMDDRVP2) | P_Fld(gDramcSwImpedanceResule[ca_term_option][1], SHU1_DRVING2_CMDDRVN2));
         vIO32WriteFldMulti_All((DRAMC_REG_SHU1_DRVING2 + save_to_where*0x600), P_Fld(gDramcSwImpedanceResule[ca_term_option][0], SHU1_DRVING2_CMDDRVP1) | P_Fld(gDramcSwImpedanceResule[ca_term_option][1], SHU1_DRVING2_CMDDRVN1));

         //CLK
         vIO32WriteFldAlign_All((DDRPHY_SHU1_CA_CMD2 + save_to_where*0x500), gDramcSwImpedanceResule[dq_term_option][0], SHU1_CA_CMD2_RG_TX_ARCLK_DRVP);
         vIO32WriteFldAlign_All((DDRPHY_SHU1_CA_CMD2 + save_to_where*0x500), gDramcSwImpedanceResule[dq_term_option][1], SHU1_CA_CMD2_RG_TX_ARCLK_DRVN);

         //CKE
         // CKE is full swing.
         // LP3 CKE DRVP/DRVN is same above
         //DRVP[4:0] = RG_TX_ARCMD_PU_PRE<1:0>, RG_TX_ARCLK_DRVN_PRE<2:0>
         if(u1GetICVersion_Bianco()>=IC_VERSION_E3)
         {     
             u1Hysteresis = 0x18;
         }
         else
         {
             u1Hysteresis = gDramcSwImpedanceResule[dq_term_option][0];
         }
         //DRVP[4:0] = RG_TX_ARCMD_PU_PRE<1:0>, RG_TX_ARCLK_DRVN_PRE<2:0>
         vIO32WriteFldAlign_All((DDRPHY_SHU1_CA_CMD3 + save_to_where*0x500), (u1Hysteresis>>3)&0x3, SHU1_CA_CMD3_RG_TX_ARCMD_PU_PRE);
         vIO32WriteFldAlign_All((DDRPHY_SHU1_CA_CMD0 + save_to_where*0x500), u1Hysteresis&0x7, SHU1_CA_CMD0_RG_TX_ARCLK_DRVN_PRE);
         mcSHOW_DBG_MSG(("CKE DRVP=%d\n", u1Hysteresis));

         //DRVN[4:0] = RG_ARCMD_REV<12:8>
         vIO32WriteFldAlign_All((DDRPHY_SHU1_CA_CMD3 + save_to_where*0x500), gDramcSwImpedanceResule[dq_term_option][1], SHU1_CA_CMD3_RG_ARCMD_REV_CKE_DRVN);
    }
#endif /* ENABLE_LP3_SW */
}

//-------------------------------------------------------------------------
/** vImpCalVrefSel
 *  Set IMP_VREF_SEL for DRVP, DRVN, Run-time/Tracking
 *  (Refer to "IMPCAL Settings" document register "RG_RIMP_VREF_SEL" settings)
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param  term_option     (U8): pass term_option (odt_on/off) for LP4X
 *  @param  u1ImpCalStage   (U8): During DRVP, DRVN, run-time/tracking stages
 *                                some vref_sel values are different
 */
//-------------------------------------------------------------------------
/* Definitions to make IMPCAL_VREF_SEL function more readable */
#define IMPCAL_STAGE_DRVP     1
#define IMPCAL_STAGE_DRVN     2
#define IMPCAL_STAGE_TRACKING 3

/* LP3 IMP_VREF_SEL ============================== */
#define IMP_LP3_VREF_SEL               0x2b

/* LP4 IMP_VREF_SEL ============================== */
#define IMP_LP4_VREF_SEL               0x1b

/* LP4X IMP_VREF_SEL ============================== */
/* LP4X IMP_VREF_SEL w/i term ==== */
#define IMP_LP4X_TERM_05VDDQ_VREF_SEL     0x17
#define IMP_LP4X_TERM_06VDDQ_VREF_SEL     0x1b

/* Depending on LP4X 0.5VDDQ(VDDQ * 0.5) or 0.6VDDQ(VDDQ * 0.6), vref_sel settings are different */
#if PU_CAL_05VDDQ
    #define IMP_LP4X_TERM_VREF_SEL     IMP_LP4X_TERM_05VDDQ_VREF_SEL
#else // VDDQ * 0.6
    #define IMP_LP4X_TERM_VREF_SEL     IMP_LP4X_TERM_06VDDQ_VREF_SEL
#endif

/* LP4X IMP_VREF_SEL w/o term ==== */
#if (ENABLE_DQ3200_UNTERM == 1)
    #define IMP_DRVP_LP4X_UNTERM_VREF_SEL   0x3a
    #define IMP_DRVN_LP4X_UNTERM_VREF_SEL   0x2a
    #define IMP_TRACK_LP4X_UNTERM_VREF_SEL  0x3a
#else
    #define IMP_DRVP_LP4X_UNTERM_VREF_SEL   0x1a
    #define IMP_DRVN_LP4X_UNTERM_VREF_SEL   0x16
    #define IMP_TRACK_LP4X_UNTERM_VREF_SEL  0x1a
#endif

/* LP4P IMP_VREF_SEL ============================== */
#define IMP_DRVP_LP4P_VREF_SEL        0x13
#define IMP_DRVN_LP4P_VREF_SEL        0xf
#define IMP_TRACK_LP4P_VREF_SEL       0x13

/* Refer to "IMPCAL Settings" document register "RG_RIMP_VREF_SEL" settings */
static void vImpCalVrefSel(DRAMC_CTX_T *p, U8 term_option, U8 u1ImpCalStage)
{
    U8 u1RegTmpValue = 0;

    if (p->dram_type == TYPE_LPDDR4)
    {
        u1RegTmpValue = IMP_LP4_VREF_SEL;
    }
    else if (p->dram_type == TYPE_LPDDR4X)
    {
        if (term_option == 1) // w/i term
        {
            /* 
             * SoC's TX for (0.5 * VDDQ) can't achieve sufficient performance (eye-size),
             * so TX vref is still set to (0.6 * VDDQ) target vref setting
             */
            if(p->vendor_id == VENDOR_HYNIX)
            {
                u1RegTmpValue = IMP_LP4X_TERM_06VDDQ_VREF_SEL;
            }
            else
            {
                u1RegTmpValue = IMP_LP4X_TERM_VREF_SEL;
            }
        }
        else // w/o term
        {
            if (u1ImpCalStage == IMPCAL_STAGE_DRVP) // OCDP
            {
                u1RegTmpValue = IMP_DRVP_LP4X_UNTERM_VREF_SEL;
            }
            else if (u1ImpCalStage == IMPCAL_STAGE_DRVN) // ODTN
            {
                u1RegTmpValue = IMP_DRVN_LP4X_UNTERM_VREF_SEL;
            }
            else // IMPCAL_STAGE_TRACKING (Tracking)
            {
                u1RegTmpValue = IMP_TRACK_LP4X_UNTERM_VREF_SEL;
            }
        }
    }
    else if (p->dram_type == TYPE_LPDDR4P)
    {
        if (u1ImpCalStage == IMPCAL_STAGE_DRVP) // OCDP
        {
            u1RegTmpValue = IMP_DRVP_LP4P_VREF_SEL;
        }
        else if (u1ImpCalStage == IMPCAL_STAGE_DRVN) // OCDN
        {
            u1RegTmpValue = IMP_DRVN_LP4P_VREF_SEL;
        }
        else // IMPCAL_STAGE_TRACKING (Tracking)
        {
            u1RegTmpValue = IMP_TRACK_LP4P_VREF_SEL;
        }
    }
    else // TYPE_LPDDR3
    {
        u1RegTmpValue = IMP_LP3_VREF_SEL;
    }

    // dbg msg after vref_sel selection
    mcSHOW_DBG_MSG3(("[vImpCalVrefSel] IMP_VREF_SEL 0x%x, IMPCAL stage:%u, term_option:%u\n",
                      u1RegTmpValue, u1ImpCalStage, term_option));

    /* Set IMP_VREF_SEL register field's value */
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_CA_CMD11), u1RegTmpValue, SHU1_CA_CMD11_RG_RIMP_VREF_SEL);

    return;
}

DRAM_STATUS_T DramcSwImpedanceCal(DRAMC_CTX_T *p, U8 u1Para, U8 term_option)
{
    U32 u4ImpxDrv, u4ImpCalResult;
    U32 u4DRVP_Result =0xff,u4ODTN_Result =0xff, u4DRVN_Result =0xff;
    U32 u4BaklReg_DDRPHY_MISC_IMP_CTRL0, u4BaklReg_DDRPHY_MISC_IMP_CTRL1, u4BaklReg_DRAMC_REG_IMPCAL;
    U8 u1ByteIdx, u1RegTmpValue;
    U8 backup_channel;

    // Darren
    vIO32WriteFldMulti_All(DDRPHY_MISC_SPM_CTRL1, P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10) | P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_B0)
                                           | P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_B1) | P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_CA));
    
    vIO32WriteFldAlign_All(DDRPHY_MISC_SPM_CTRL2, 0x0, MISC_SPM_CTRL2_PHY_SPM_CTL2);
    vIO32WriteFldAlign_All(DDRPHY_MISC_SPM_CTRL0, 0x0, MISC_SPM_CTRL0_PHY_SPM_CTL0);

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);    

    backup_channel = p->channel;
//    vSetPHY2ChannelMapping(p, CHANNEL_A);
#if (FOR_DV_SIMULATION_USED==0)
    vIO32Write4B_All(DRAMC_REG_ADDR(DDRPHY_MISC_SPM_CTRL3), 0);
#endif

    //Disable IMP HW Tracking
    vIO32WriteFldAlign_All(DRAMC_REG_IMPCAL, 0, IMPCAL_IMPCAL_HW);

    //Register backup
    //u4BaklReg_DDRPHY_MISC_IMP_CTRL0 = u4IO32Read4B((DDRPHY_MISC_IMP_CTRL0));
    //u4BaklReg_DDRPHY_MISC_IMP_CTRL1 = u4IO32Read4B((DDRPHY_MISC_IMP_CTRL1));
    u4BaklReg_DRAMC_REG_IMPCAL = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL));

    /* Set IMP_VREF_SEL value for DRVP */
    vImpCalVrefSel(p, term_option, IMPCAL_STAGE_DRVP);

    if(u1IsLP4Family(p->dram_type))
    {
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
    	vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MISC_IMP_CTRL0), P_Fld(1, MISC_IMP_CTRL0_RG_RIMP_EN) | \
    						P_Fld(0, MISC_IMP_CTRL0_RG_RIMP_DDR3_SEL) | P_Fld(1, MISC_IMP_CTRL0_RG_RIMP_DDR4_SEL));
    }
#if ENABLE_LP3_SW
    else //LPDDR3
    {
    	//RG_IMPCAL_VREF_SEL (now set in vImpCalVrefSel())
    	//RG_IMPCAL_LP3_EN=0, RG_IMPCAL_LP4_EN=1
        //RG_IMPCAL_ODT_EN=0
#if 0
        vIO32WriteFldMulti((DDRPHY_MISC_IMP_CTRL1), P_Fld(0, MISC_IMP_CTRL1_RG_RIMP_ODT_EN)| P_Fld(1, MISC_IMP_CTRL1_RG_RIMP_BIAS_EN) | \
        					P_Fld(0, MISC_IMP_CTRL1_RG_RIMP_PRE_EN) | P_Fld(1, MISC_IMP_CTRL1_RG_RIMP_VREF_EN));
#else
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MISC_IMP_CTRL1), P_Fld(0, MISC_IMP_CTRL1_RG_RIMP_PRE_EN));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), P_Fld(0, IMPCAL_IMPCAL_CALI_ENN) | P_Fld(1, IMPCAL_IMPCAL_IMPPDP) | \
                            P_Fld(1, IMPCAL_IMPCAL_IMPPDN));    //RG_RIMP_BIAS_EN and RG_RIMP_VREF_EN move to IMPPDP and IMPPDN, ODT_EN move to CALI_ENN
#endif
    	vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MISC_IMP_CTRL0), P_Fld(1, MISC_IMP_CTRL0_RG_RIMP_EN) | \
    						P_Fld(1, MISC_IMP_CTRL0_RG_RIMP_DDR3_SEL) | P_Fld(0, MISC_IMP_CTRL0_RG_RIMP_DDR4_SEL));

        #ifdef ETT_PRINT_FORMAT
    	mcSHOW_DBG_MSG2(("0x%X=0x%X\n", DRAMC_REG_ADDR(DDRPHY_MISC_IMP_CTRL1), u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_IMP_CTRL1))));
    	mcSHOW_DBG_MSG2(("0x%X=0x%X\n", DRAMC_REG_ADDR(DDRPHY_MISC_IMP_CTRL0), u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_IMP_CTRL0))));
        #else
    	mcSHOW_DBG_MSG2(("0x%8x=0x%8x\n", DRAMC_REG_ADDR(DDRPHY_MISC_IMP_CTRL1), u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_IMP_CTRL1))));
    	mcSHOW_DBG_MSG2(("0x%8x=0x%8x\n", DRAMC_REG_ADDR(DDRPHY_MISC_IMP_CTRL0), u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_IMP_CTRL0))));
        #endif
    }
#endif
	mcDELAY_US(1);

    // K pull up
    mcSHOW_DBG_MSG2(("\n\tK DRVP\n"));
    mcFPRINTF((fp_A60501, "\n\tK DRVP\n"));
    //PUCMP_EN=1
    //ODT_EN=0
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), 1, IMPCAL_IMPCAL_CALI_EN);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), 1, IMPCAL_IMPCAL_CALI_ENP);  //PUCMP_EN move to CALI_ENP
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), 0, IMPCAL_IMPCAL_CALI_ENN);  //ODT_EN move to CALI_ENN
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_IMPCAL1), P_Fld(0, SHU_IMPCAL1_IMPDRVN)|P_Fld(0, SHU_IMPCAL1_IMPDRVP));

    //DRVP=0
    //DRV05=1
#if (fcFOR_CHIP_ID == fcBianco)
    if(p->dram_type == TYPE_LPDDR4)
    {
        u1RegTmpValue = 1;//LP4P_EN=0, DRV05=1
    }
    else  //TYPE_LPDDR4X || TYPE_LPDDR4P
    {
        u1RegTmpValue = 3;//LP4P_EN=1, DRV05=1
    }
#endif
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_CA_CMD11), u1RegTmpValue, SHU1_CA_CMD11_RG_RIMP_REV);

    //OCDP Flow
    //If RGS_TX_OCD_IMPCALOUTX=0
    //RG_IMPX_DRVP++;
    //Else save and keep RG_IMPX_DRVP value, and assign to DRVP
    for(u4ImpxDrv=0; u4ImpxDrv<32; u4ImpxDrv++)
    {
        if(u4ImpxDrv==16) //0~15, 29~31
            u4ImpxDrv = 29;
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_IMPCAL1), u4ImpxDrv, SHU_IMPCAL1_IMPDRVP);
        mcDELAY_US(1);
        u4ImpCalResult = u4IO32ReadFldAlign((DDRPHY_MISC_PHY_RGS_CMD), MISC_PHY_RGS_CMD_RGS_RIMPCALOUT);
        mcSHOW_DBG_MSG2(("1. OCD DRVP=%d CALOUT=%d\n", u4ImpxDrv, u4ImpCalResult));
        mcFPRINTF((fp_A60501, "1. OCD DRVP=%d CALOUT=%d\n", u4ImpxDrv, u4ImpCalResult));

        if((u4ImpCalResult ==1) && (u4DRVP_Result == 0xff))//first found
        {
            u4DRVP_Result = u4ImpxDrv;
            mcSHOW_DBG_MSG2(("\n1. OCD DRVP calibration OK! DRVP=%d\n\n", u4DRVP_Result));
            mcFPRINTF((fp_A60501, "\n1. OCD DRVP calibration OK! DRVP=%d\n\n", u4DRVP_Result));
            break;
        }
    }

    //LP4: ODTN calibration, LP3: DRVN calibration
    mcSHOW_DBG_MSG2(("\n\n\tK ODTN\n"));
    mcFPRINTF((fp_A60501, "\n\tK ODTN\n"));


    /* Set IMP_VREF_SEL value for DRVN */
    vImpCalVrefSel(p, term_option, IMPCAL_STAGE_DRVN);

    //PUCMP_EN=0
    //LPDDR4 : ODT_EN=1
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), 0, IMPCAL_IMPCAL_CALI_ENP);  //PUCMP_EN move to CALI_ENP

    if(p->dram_type == TYPE_LPDDR4 || p->dram_type == TYPE_LPDDR4X)
    {    	
        if (term_option == 1)
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), 1, IMPCAL_IMPCAL_CALI_ENN);  //ODT_EN move to CALI_ENN
        }
    }

    //DRVP=DRVP_FINAL
    //DRVN=0
    //DRV05=1
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_IMPCAL1), P_Fld(u4DRVP_Result, SHU_IMPCAL1_IMPDRVP) | P_Fld(0, SHU_IMPCAL1_IMPDRVN));

#if (fcFOR_CHIP_ID == fcBianco)
    if(p->dram_type == TYPE_LPDDR4)
    {
        u1RegTmpValue = 1;//LP4P_EN=0, DRV05=1
    }
    else  //TYPE_LPDDR4X || TYPE_LPDDR4P
    {
        u1RegTmpValue = 3;//LP4P_EN=1, DRV05=3
    }
#endif
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_CA_CMD11), u1RegTmpValue, SHU1_CA_CMD11_RG_RIMP_REV);


    //If RGS_TX_OCD_IMPCALOUTX=1
    //RG_IMPX_DRVN++;
    //Else save RG_IMPX_DRVN value and assign to DRVN
    for(u4ImpxDrv=0; u4ImpxDrv<32 ; u4ImpxDrv++)
    {
        if(u4ImpxDrv==16) //0~15, 29~31
            u4ImpxDrv = 29;

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_IMPCAL1), u4ImpxDrv, SHU_IMPCAL1_IMPDRVN);
        mcDELAY_US(1);
        u4ImpCalResult = u4IO32ReadFldAlign((DDRPHY_MISC_PHY_RGS_CMD), MISC_PHY_RGS_CMD_RGS_RIMPCALOUT);
        mcSHOW_DBG_MSG2(("3. OCD ODTN=%d ,CALOUT=%d\n", u4ImpxDrv, u4ImpCalResult));
        mcFPRINTF((fp_A60501, "3. OCD ODTN=%d ,CALOUT=%d\n", u4ImpxDrv, u4ImpCalResult));

        if((u4ImpCalResult ==0) &&(u4ODTN_Result == 0xff))//first found
        {
            u4ODTN_Result = u4ImpxDrv;
            mcSHOW_DBG_MSG2(("\n3. OCD ODTN calibration OK! ODTN=%d\n\n", u4ODTN_Result));
            mcFPRINTF((fp_A60501, "\n3. OCD ODTN calibration OK! ODTN=%d\n\n", u4ODTN_Result));
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
    if(u1IsLP4Family(p->dram_type))
    {
#if (fcFOR_CHIP_ID == fcBianco)
        u4DRVN_Result = 9; //fixed value from DE YingMin Liao
#endif

        if(u4DRVP_Result==0xff)
        {
            mcFPRINTF((fp_A60501, "\n[CHIP_SCAN]1. OCD DRVP calibration FAIL! \n\n"));
#if (fcFOR_CHIP_ID == fcBianco)
            u4DRVP_Result = 31;
#endif
        }
        if(u4ODTN_Result==0xff || u4DRVP_Result==0xff)
        {
            mcFPRINTF((fp_A60501, "\n[CHIP_SCAN]3. OCD ODTN calibration FAIL! \n\n"));
#if (fcFOR_CHIP_ID == fcBianco)
            u4ODTN_Result = 31;
#endif
        }

        mcSHOW_DBG_MSG(("[SwImpedanceCal] DRVP=%d, DRVN=%d, ODTN=%d\n", u4DRVP_Result, u4DRVN_Result, u4ODTN_Result));
        mcFPRINTF((fp_A60501, "[SwImpedanceCal] DRVP=%d, DRVN=%d, ODTN=%d\n", u4DRVP_Result, u4DRVN_Result, u4ODTN_Result));

#ifdef DRAM_CALIB_LOG
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].SwImpedanceCal.DRVP = u4DRVP_Result;
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].SwImpedanceCal.DRVN = u4DRVN_Result;
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].SwImpedanceCal.ODTN = u4ODTN_Result;
#endif

        #if 0//HYNIX_IMPX_ADJUST
        if(u1Para)
        {
            u4ODTN_Result= ImpedanceAdjustment_Hynix(u4ODTN_Result, u1Para);
        }
        #endif

        if((p->dram_type == TYPE_LPDDR4X || p->dram_type == TYPE_LPDDR4P) && (term_option ==0))
        {
                gDramcSwImpedanceResule[term_option][0] = u4DRVP_Result;
                gDramcSwImpedanceResule[term_option][1] = u4ODTN_Result;    //Justin : LP4X unterm DRVN is ODTN * 2
                gDramcSwImpedanceResule[term_option][2] = 0;
                gDramcSwImpedanceResule[term_option][3] = 15;    //Justin : LP4X unterm, ODTN is useless
        }
        else
        {
            gDramcSwImpedanceResule[term_option][0] = (u4DRVP_Result<=3) ? (u4DRVP_Result * 3) : u4DRVP_Result;
            gDramcSwImpedanceResule[term_option][1] = (u4DRVN_Result<=3) ? (u4DRVN_Result * 3) : u4DRVN_Result;
            gDramcSwImpedanceResule[term_option][2] = 0;
            gDramcSwImpedanceResule[term_option][3] = (u4ODTN_Result<=3) ? (u4ODTN_Result * 3) : u4ODTN_Result;
        }
    }
#if ENABLE_LP3_SW
    else  //LPDDR3
    {
        u4DRVN_Result = u4ODTN_Result;
        if(u4DRVN_Result==0xff || u4DRVP_Result==0xff)
        {
            u4DRVP_Result = 8;
        	u4DRVN_Result = 9;
		}

        gDramcSwImpedanceResule[term_option][0] = (u4DRVP_Result<=3) ? (u4DRVP_Result * 3) : u4DRVP_Result;
        gDramcSwImpedanceResule[term_option][1] = (u4DRVN_Result<=3) ? (u4DRVN_Result * 3) : u4DRVN_Result;
        gDramcSwImpedanceResule[term_option][2] = 0;
        gDramcSwImpedanceResule[term_option][3] = (u4ODTN_Result<=3) ? (u4ODTN_Result * 3) : u4ODTN_Result;

        ///TODO: wait Jeremy delete it
#if SIMULATION_SW_IMPED
        DramcSwImpedanceSaveRegister(p, term_option, term_option, DRAM_DFS_SHUFFLE_1);
#endif
    }
#endif

    /* Set IMP_VREF_SEL value for TRACKING/RUN-TIME */
    vImpCalVrefSel(p, term_option, IMPCAL_STAGE_TRACKING);

    mcSHOW_DBG_MSG(("term_option=%d, Reg: DRVP=%d, DRVN=%d, ODTN=%d\n", term_option, gDramcSwImpedanceResule[term_option][0],
                                        gDramcSwImpedanceResule[term_option][1], gDramcSwImpedanceResule[term_option][3]));

#if defined(SLT)
    if (gDramcSwImpedanceResule[term_option][0] >= 31 && (term_option ==1) ) {
        mcSHOW_DBG_MSG(("DRVP >= 31\n"));
        if (!((seclib_get_devinfo_with_index(25) & 0x200) >> 9))
        	while(1);
    } 
#endif    
    

    vSetCalibrationResult(p, DRAM_CALIBRATION_SW_IMPEDANCE, DRAM_OK);
    mcSHOW_DBG_MSG3(("[DramcSwImpedanceCal] Done\n\n"));
    mcFPRINTF((fp_A60501, "[DramcSwImpedanceCal] Done\n\n"));

    vSetPHY2ChannelMapping(p, backup_channel);

    return DRAM_OK;
}
#endif //SIMULATION_SW_IMPED

void DramcUpdateImpedanceTerm2UnTerm(DRAMC_CTX_T *p)
{
    gDramcSwImpedanceResule[ODT_OFF][2] = gDramcSwImpedanceResule[ODT_ON][2];
    gDramcSwImpedanceResule[ODT_OFF][3] = gDramcSwImpedanceResule[ODT_ON][3];
}

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


static void O1PathOnOff(DRAMC_CTX_T *p, U8 u1OnOff)
{
    if(u1OnOff)
    {
        if(u1IsLP4Family(p->dram_type))
        {
            vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ5), 1, B0_DQ5_RG_RX_ARDQ_EYE_VREF_EN_B0);
            vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ5), 1, B1_DQ5_RG_RX_ARDQ_EYE_VREF_EN_B1);
            vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ3), 1, B0_DQ3_RG_RX_ARDQ_SMT_EN_B0);
            vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ3), 1, B1_DQ3_RG_RX_ARDQ_SMT_EN_B1);
        }
        #if ENABLE_LP3_SW
        else
        {
            vIO32WriteFldAlign(DDRPHY_B1_DQ5, 1, B1_DQ5_RG_RX_ARDQ_EYE_VREF_EN_B1);                     //CHA B1
            vIO32WriteFldAlign(DDRPHY_B0_DQ5+((U32)CHANNEL_B<<POS_BANK_NUM), 1, B0_DQ5_RG_RX_ARDQ_EYE_VREF_EN_B0);   //CHB B0
            vIO32WriteFldAlign(DDRPHY_CA_CMD5+((U32)CHANNEL_B<<POS_BANK_NUM), 1, CA_CMD5_RG_RX_ARCMD_EYE_VREF_EN);   //CHB CA
            vIO32WriteFldAlign(DDRPHY_B1_DQ5+((U32)CHANNEL_B<<POS_BANK_NUM), 1, B1_DQ5_RG_RX_ARDQ_EYE_VREF_EN_B1);   //CHB B1

            vIO32WriteFldAlign(DDRPHY_B1_DQ3, 1, B1_DQ3_RG_RX_ARDQ_SMT_EN_B1);                      //CHA B1
            vIO32WriteFldAlign(DDRPHY_B0_DQ3+((U32)CHANNEL_B<<POS_BANK_NUM), 1, B0_DQ3_RG_RX_ARDQ_SMT_EN_B0);    //CHB B0
            vIO32WriteFldAlign(DDRPHY_CA_CMD3+((U32)CHANNEL_B<<POS_BANK_NUM), 1, CA_CMD3_RG_RX_ARCMD_SMT_EN);    //CHB CA
            vIO32WriteFldAlign(DDRPHY_B1_DQ3+((U32)CHANNEL_B<<POS_BANK_NUM), 1, B1_DQ3_RG_RX_ARDQ_SMT_EN_B1);    //CHB B1
        }
        #endif
        mcDELAY_US(1);
    }
    else
    {
        if(u1IsLP4Family(p->dram_type))
        {
            vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ3), 0, B0_DQ3_RG_RX_ARDQ_SMT_EN_B0);
            vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ3), 0, B1_DQ3_RG_RX_ARDQ_SMT_EN_B1);
            vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ5), 0, B0_DQ5_RG_RX_ARDQ_EYE_VREF_EN_B0);
            vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ5), 0, B1_DQ5_RG_RX_ARDQ_EYE_VREF_EN_B1);
        }
        #if ENABLE_LP3_SW
        else
        {
            vIO32WriteFldAlign(DDRPHY_B1_DQ5, 0, B1_DQ5_RG_RX_ARDQ_EYE_VREF_EN_B1);                     //CHA B1
            vIO32WriteFldAlign(DDRPHY_B0_DQ5+((U32)CHANNEL_B<<POS_BANK_NUM), 0, B0_DQ5_RG_RX_ARDQ_EYE_VREF_EN_B0);   //CHB B0
            vIO32WriteFldAlign(DDRPHY_CA_CMD5+((U32)CHANNEL_B<<POS_BANK_NUM), 0, CA_CMD5_RG_RX_ARCMD_EYE_VREF_EN);   //CHB CA
            vIO32WriteFldAlign(DDRPHY_B1_DQ5+((U32)CHANNEL_B<<POS_BANK_NUM), 0, B1_DQ5_RG_RX_ARDQ_EYE_VREF_EN_B1);   //CHB B1

            vIO32WriteFldAlign(DDRPHY_B1_DQ3, 0, B1_DQ3_RG_RX_ARDQ_SMT_EN_B1);                      //CHA B1
            vIO32WriteFldAlign(DDRPHY_B0_DQ3+((U32)CHANNEL_B<<POS_BANK_NUM), 0, B0_DQ3_RG_RX_ARDQ_SMT_EN_B0);    //CHB B0
            vIO32WriteFldAlign(DDRPHY_CA_CMD3+((U32)CHANNEL_B<<POS_BANK_NUM), 0, CA_CMD3_RG_RX_ARCMD_SMT_EN);    //CHB CA
            vIO32WriteFldAlign(DDRPHY_B1_DQ3+((U32)CHANNEL_B<<POS_BANK_NUM), 0, B1_DQ3_RG_RX_ARDQ_SMT_EN_B1);    //CHB B1
        }
        #endif
    }
}

// LPDDR DQ -> PHY DQ mapping
#if 0//(FOR_DV_SIMULATION_USED!=0)
#if ENABLE_LP3_SW
const U8 uiLPDDR3_PHY_Mapping_POP[32] =
{
    20, 22, 21, 23, 19, 17, 16, 18,
    26, 24, 29, 30, 31, 27, 28, 25,
    10, 8, 15, 14, 11, 9, 12, 13,
    0, 2, 5, 6, 4, 1, 3, 7
};
#endif /* ENABLE_LP3_SW */

const U8 uiLPDDR4_PHY_Mapping_POP[CHANNEL_NUM][16] =
{
    //CH-A
    {
        0, 1, 7, 6, 3, 5, 4, 2,
        9, 8, 13, 12, 11, 15, 14, 10
    },
    //CH-B
    {
        0, 1, 5, 6, 3, 7, 4, 2,
        9, 8, 12, 13, 11, 15, 14, 10
    }
};
#endif

#if 0 //ENABLE_LP3_SW //Move to Gloable variable in main.c and gloable init
const U8 uiLPDDR3_PHY_Mapping_POP[32] =
{
    0, 4, 2, 3, 1, 5, 6, 7,
    11, 9, 10, 8, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23,
    27, 24, 26, 28, 25, 29, 30, 31
};
#endif /* ENABLE_LP3_SW */

const U8 uiLPDDR4_PHY_Mapping_POP[CHANNEL_NUM][16] =
{
    //CH-A
    {
        0, 1, 7, 6, 3, 5, 4, 2,
        9, 8, 13, 12, 11, 15, 14, 10
    },
    //CH-B
    {
        0, 1, 5, 6, 3, 7, 4, 2,
        9, 8, 12, 13, 11, 15, 14, 10
    }
};

#if ENABLE_LP3_SW
#if SIMULATION_LP3_CA_TRAINING
//-------------------------------------------------------------------------
/** DramcCATraining
 *  start the calibrate the skew between Clk pin and CAx pins.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL
 */
//-------------------------------------------------------------------------
#define MAX_CLKO_DELAY         31
#define CATRAINING_NUM        10

static DRAM_STATUS_T CATrainingEntry(DRAMC_CTX_T *p, U32 uiMR41, U32 u4GoldenPattern)
{
    //CA_TRAINING_BEGIN:

    // CS extent enable (need DRAM to support)
    // for testing
#if CA_TRAINING_K_RANK1_ENABLE
    if(p->rank == RANK_1)
    {
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), p->rank, MRS_MRSRK);

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), P_Fld(0, RKCFG_CS0FORCE)|P_Fld(0, RKCFG_CS2RANK)); //cannot enable before any MRW command
    }
#endif

#if (FOR_DV_SIMULATION_USED==0)
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CATRAINING1), 1, CATRAINING1_CATRAINCSEXT);
#endif

    // CKE high, CKE must be driven HIGH prior to issuance of the MRW41 and MRW48 command
    CKEFixOnOff(p, CKE_FIXON, CKE_WRITE_TO_ONE_CHANNEL);

    // Enter MR 41/MR48
    // Set MA & OP.
    if (uiMR41)
    {
    	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_MRS), P_Fld(0xa4, MRS_MRSOP)|P_Fld(0,MRS_MRSBA)|P_Fld(41, MRS_MRSMA));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_MRS), P_Fld(0xc0, MRS_MRSOP)|P_Fld(0,MRS_MRSBA)|P_Fld(48, MRS_MRSMA));
    }
    // Hold the CA bus stable for at least one cycle.
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CATRAINING1), 1, CATRAINING1_CATRAINMRS);

    // MRW
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1, SPCMD_MRWEN);
    mcDELAY_US(1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_MRWEN);

    // Disable CA bus stable.
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CATRAINING1), 0, CATRAINING1_CATRAINMRS);

    // Wait tCACKEL(10 tck) before CKE low
    mcDELAY_US(1);

    // CKE low
    CKEFixOnOff(p, CKE_FIXOFF, CKE_WRITE_TO_ONE_CHANNEL);

    // Set CA0~CA3, CA5~CA8 rising/falling golden value.
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_CATRAINING2), u4GoldenPattern);

#if CA_TRAINING_K_RANK1_ENABLE
    if(p->rank == RANK_1)
    {
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), P_Fld(1, RKCFG_CS0FORCE)| P_Fld(1, RKCFG_CS2RANK)); //enable to force CS switch to rank1
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), 1, RKCFG_TXRANK); //TXRANKFIX should be write after TXRANK or the rank will be fix at rank 1
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), 1, RKCFG_TXRANKFIX); //enable to switch to rank1
    }
#endif


    // Wait tCAENT(10 tck) before pattern output
    mcDELAY_US(1);

    return DRAM_OK;
}


static S32 CheckCATrainingTransition(U32 uiCA, U32 capattern, U32 uiRisingEdge, U32 uiFallingEdge)
{
    S32 iPass=0;
    U32 iii;

    if (capattern == 0x55555555)
    {
         if ((uiRisingEdge!=0) && (uiFallingEdge==0))
         {
             iPass = 1;
         }
         else
         {
             iPass = 0;
         }
    }
    else if (capattern == 0xaaaaaaaa)
    {
        if ((uiRisingEdge==0) && (uiFallingEdge!=0))
        {
            iPass = 1;
        }
        else
        {
            iPass = 0;
        }
    }
    else if (capattern == 0x99999999)
    {
        iii = uiCA;
        if (iii>=5) iii-=5;

        if ((iii & 1) == 0)
        {
            if ((uiRisingEdge!=0) && (uiFallingEdge==0))
            {
                iPass = 1;
            }
            else
            {
                iPass = 0;
            }
        }
        else
        {
            if ((uiRisingEdge==0) && (uiFallingEdge!=0))
            {
                iPass = 1;
            }
            else
            {
                iPass = 0;
            }
        }
    }
    else if (capattern == 0x66666666)
    {
        iii = uiCA;
        if (iii>=5) iii-=5;

        if ((iii & 1) == 0)
        {
            if ((uiRisingEdge==0) && (uiFallingEdge!=0))
            {
                iPass = 1;
            }
            else
            {
                iPass = 0;
            }
        }
        else
        {
            if ((uiRisingEdge!=0) && (uiFallingEdge==0))
            {
                iPass = 1;
            }
            else
            {
                iPass = 0;
            }
        }
    }

    return iPass;
}


static void CATrainingExit(DRAMC_CTX_T *p)
{
#if CA_TRAINING_K_RANK1_ENABLE
    if(p->rank == RANK_1) //disable setting of CS and TX rank sel switch to rank1 before leaving CA training
    {
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), P_Fld(0, RKCFG_CS0FORCE)| P_Fld(0, RKCFG_CS2RANK));
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), 0, RKCFG_TXRANK); //TXRANKFIX should be write after TXRANK or the rank will be fix at rank 1
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), 0, RKCFG_TXRANKFIX); //enable to switch to rank1
    }
#endif

    CKEFixOnOff(p, CKE_FIXON, CKE_WRITE_TO_ONE_CHANNEL);
    mcDELAY_US(1);

    // CS extent enable
    // for testing
#if (FOR_DV_SIMULATION_USED==0)
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CATRAINING1), 1, CATRAINING1_CATRAINCSEXT);
#endif

    // MR42 to leave CA training.
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_MRS), P_Fld(0xa8, MRS_MRSOP)|P_Fld(0,MRS_MRSBA)|P_Fld(0x2a, MRS_MRSMA));

    // Hold the CA bus stable for at least one cycle.
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CATRAINING1), 1, CATRAINING1_CATRAINMRS);

    // MRW
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1, SPCMD_MRWEN);
    mcDELAY_US(1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_MRWEN);

    // Disable the hold the CA bus stable for at least one cycle.
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CATRAINING1), 0, CATRAINING1_CATRAINMRS);

    // CS extent disable
    // for testing
#if (FOR_DV_SIMULATION_USED==0)
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CATRAINING1), 0, CATRAINING1_CATRAINCSEXT);
#endif
}

#if CA_TRAINING_K_RANK1_ENABLE 
    static S32 s4CATrainingResult0[CHANNEL_NUM][RANK_MAX];
    static S32 s4CATrainingResult1[CHANNEL_NUM][RANK_MAX];
#endif

void CATrainingDelayCompare(DRAMC_CTX_T *p, U32 uiMR41, U32 u4GoldenPattern, S32 *iCenterSum, S32 *iFirstCAPass , S32 *iLastCAPass, S32 *iCenter, U32 delayinecompare_flag)
{
    U8 *uiLPDDR_PHY_Mapping = NULL;
//    U32 u4O1Data[DQS_NUMBER];
    U32 u4dq_o1_tmp[DQS_NUMBER];
    U32 uiTemp, uiCA, uiRisingEdge, uiFallingEdge, uiFinishCount;
    S32 iPass, iDelay, iDelay_Start, iDelay_End;

    uiLPDDR_PHY_Mapping = (U8 *)uiLPDDR3_PHY_Mapping_POP;

    // Calculate the middle range & max middle.
    mcSHOW_DBG_MSG(("\n[CA Training]\n"));
    vPrintCalibrationBasicInfo(p);

    mcFPRINTF((fp_A60501, "\n1. CA training window before adjustment.\n"));
    mcFPRINTF((fp_A60501, "x=Pass window CA(max~min) Clk(min~max) center. \n"));
    if(uiMR41)
    {
        mcSHOW_DBG_MSG(("y=CA0~CA3, CA5~8\n\n"));
        mcFPRINTF((fp_A60501, "y=CA0~CA3, CA5~8\n\n"));
    }
    else//MR48
    {
        mcSHOW_DBG_MSG(("y=CA4 CA9\n\n"));
        mcFPRINTF((fp_A60501, "y=CA4 CA9\n\n"));
    }

    uiFinishCount = 0;

    if (delayinecompare_flag == 0)
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), 0, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CMD);    //CA0~1 CA_PI_Delay
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), 0, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);       //CA2~9 CA_PI_Delay
    }

    if (delayinecompare_flag == 0)
    {
        iDelay_Start = -(MAX_CLK_PI_DELAY/2);
        iDelay_End = MAX_CA_PI_DELAY;
    }
    else
    {
        iDelay_Start = 0;
        iDelay_End = MAX_CLK_PI_DELAY;
    }

    // Delay clock output delay to do CA training in order to get the pass window.
    for (iDelay= iDelay_Start; iDelay <= iDelay_End; iDelay++)
    {
        if (delayinecompare_flag == 0)
        {
            if(iDelay <=0)
            {    //Set CLK delay
                  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), -iDelay, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CLK); //clock_PI_Delay
            }
            else
            {    // Set CA output delay
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), iDelay, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CMD);    //CA0~1 CA_PI_Delay
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), iDelay, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);       //CA2~9 CA_PI_Delay
            }
        }
        else
        {
             // set CLK PI value
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), iDelay, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CLK);    //clock_PI_Delay
        }

        // CA training pattern output enable
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CATRAINING1), 1, CATRAINING1_CATRAINEN);
        // delay 2 DRAM clock cycle
        mcDELAY_US(1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CATRAINING1), 0, CATRAINING1_CATRAINEN);


        // Wait tADR(20ns) before CA sampled values available in DQ.
        mcDELAY_US(1);

        // Get DQ value.
        uiTemp = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_DQO1));

        #ifdef ETT_PRINT_FORMAT
        mcSHOW_DBG_MSG2(("%d, | ", iDelay));
        #else
        mcSHOW_DBG_MSG2(("delay %3d, DQ_O1 0x%8x| Pass ", iDelay, uiTemp));
        #endif
        mcFPRINTF((fp_A60501, "delay %3d, DQ_O1 0x%8x| Pass ", iDelay, uiTemp));

        // Compare with golden value.
        for (uiCA=0; uiCA<CATRAINING_NUM; uiCA++)
        {
            if(uiMR41 && ((uiCA==4) || (uiCA==9)))  // MR41 is for CA0~3, CA5~8
            {
                continue;
            }
            else if((uiMR41==0) && ((uiCA!=4) && (uiCA!=9)))// MR48 is for CA4, CA8
            {
                continue;
            }

           // if ( (iFirstCAPass[uiCA]==PASS_RANGE_NA) || (iLastCAPass[uiCA]==PASS_RANGE_NA)) //marked fo debug
            {
                if (uiCA<4)   //CA0~3
                {
                    uiRisingEdge = uiTemp & (0x01 << uiLPDDR_PHY_Mapping[uiCA<<1]);
                    uiFallingEdge = uiTemp & (0x01 << uiLPDDR_PHY_Mapping[(uiCA<<1)+1]);
                }
                else if((uiCA==4) || (uiCA==9))
                {
                    uiRisingEdge = uiTemp & (0x01 << uiLPDDR_PHY_Mapping[(uiCA==4) ? 0 : 8]);
                    uiFallingEdge = uiTemp & (0x01 << uiLPDDR_PHY_Mapping[(uiCA==4) ? 1 : 9]);
                }
                else//CA5~8
                {
                    uiRisingEdge = uiTemp & (0x01 << uiLPDDR_PHY_Mapping[(uiCA-1)<<1]);
                    uiFallingEdge = uiTemp & (0x01 << uiLPDDR_PHY_Mapping[((uiCA-1)<<1)+1]);
                }

                iPass = CheckCATrainingTransition(uiCA, u4GoldenPattern, uiRisingEdge, uiFallingEdge);
                mcSHOW_DBG_MSG2(("%d ", iPass));
                mcFPRINTF((fp_A60501, "%d ", iPass));

                if (iFirstCAPass[uiCA]==PASS_RANGE_NA)
                {
                    if (iPass == 1)
                    {
                        iFirstCAPass[uiCA] = iDelay;
                    }
                }
                else
                {
                    if (iLastCAPass[uiCA]==PASS_RANGE_NA)
                    {
                        if (iPass == 0)
                        {
                            if((iDelay-iFirstCAPass[uiCA]) <5)  // prevent glitch
                            {
                                iFirstCAPass[uiCA]=PASS_RANGE_NA;
                                continue;
                            }

                            uiFinishCount++;
                            iLastCAPass[uiCA] = iDelay-1;

                            iCenter[uiCA] = (iLastCAPass[uiCA] + iFirstCAPass[uiCA]) >>1;
                            if (uiCA==0 || uiCA==1)
                            {
                                iCenterSum[0] += iCenter[uiCA];
                            }
                            else
                            {
                                iCenterSum[1] += iCenter[uiCA];
                            }
                        }
                        else
                        {
                            if (iDelay==MAX_CA_PI_DELAY)
                            {
                                uiFinishCount++;
                                iLastCAPass[uiCA] = iDelay;

                                iCenter[uiCA] = (iLastCAPass[uiCA] + iFirstCAPass[uiCA]) >>1;
                                if (uiCA==0 || uiCA==1)
                                {
                                    iCenterSum[0] += iCenter[uiCA];
                                }
                                else
                                {
                                    iCenterSum[1] += iCenter[uiCA];
                                }

                            }
                        }
                    }
                }
            }
        }

        // Wait tCACD(22clk) before output CA pattern to DDR again..
        mcDELAY_US(1);

        mcSHOW_DBG_MSG2(("\n"));
        mcFPRINTF((fp_A60501, "\n"));

        if((uiMR41 && (uiFinishCount==8)) || ((uiMR41==0) && (uiFinishCount==2)))
        {
            mcSHOW_DBG_MSG(("Early break, MR41=%d FinishCount=%d\n", uiMR41, uiFinishCount));
            mcFPRINTF((fp_A60501, "Early break, uiMR41=%d\n", uiMR41));
            break;
        }
    }


    vSetCalibrationResult(p, DRAM_CALIBRATION_CA_TRAIN, DRAM_OK); // set default result OK, udpate status when per bit fail

    if (delayinecompare_flag == 1)
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), 0, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CLK);
    }

    for (uiCA=0; uiCA<CATRAINING_NUM; uiCA++)
    {
        if(uiMR41 && ((uiCA==4) || (uiCA==9)))  // MR41 is for CA0~3, CA5~8
        {
            continue;
        }
        else if((uiMR41==0) && ((uiCA!=4) && (uiCA!=9)))// MR48 is for CA4, CA8
        {
            continue;
        }

#ifdef FOR_HQA_TEST_USED
        gFinalCBTCA[p->channel][p->rank][uiCA] = (iLastCAPass[uiCA] - iFirstCAPass[uiCA])+1;
#endif

        #ifdef ETT_PRINT_FORMAT
        mcSHOW_DBG_MSG(("CA%d  (%d~%d) %d,\n", uiCA, iFirstCAPass[uiCA], iLastCAPass[uiCA], iCenter[uiCA]));
        #else
        mcSHOW_DBG_MSG(("CA%3d   Pass(%3d~%3d)  Center %3d,\n", uiCA, iFirstCAPass[uiCA], iLastCAPass[uiCA], iCenter[uiCA]));
        #endif
        mcFPRINTF((fp_A60501, "CA%4d Pass(%3d~%3d) Center %3d,\n", uiCA, iFirstCAPass[uiCA], iLastCAPass[uiCA], iCenter[uiCA]));

        if(iLastCAPass[uiCA]==PASS_RANGE_NA)  // no CA window found
        {
            vSetCalibrationResult(p, DRAM_CALIBRATION_CA_TRAIN, DRAM_FAIL);
        }
    }

    // CS extent disable
    // for testing
#if (FOR_DV_SIMULATION_USED==0)
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CATRAINING1), 0, CATRAINING1_CATRAINCSEXT);
#endif

    // Wait tCACKEN (10ck)
    mcDELAY_US(1);

    if((uiMR41 && (uiFinishCount<8)) || ((uiMR41==0) && (uiFinishCount<2)))
    {
        mcSHOW_DBG_MSG(("Error: some bits have abnormal window, uiMR41=%d, FinishCount=%d\n", uiMR41, uiFinishCount));
        mcFPRINTF((fp_A60501, "Error: some bits have abnormal window, uiMR41=%d, FinishCount=%d\n", uiMR41, uiFinishCount));
    }
}


DRAM_STATUS_T CATrainingLP3(DRAMC_CTX_T *p)
{
    U32 uiMR41;
    U32 uiReg024h;
    U32 uiReg038h;
    U32 uiReg04ch;
    U32 uiReg810h;
    U32 uiReg064h;
    U32 uiMRSRK;
    S32 iFinalCACLK[2];
    S32 iCenterSum[2]={0,0};
    U32 ii, module_i;
    S32 iFirstCAPass[CATRAINING_NUM], iLastCAPass[CATRAINING_NUM], iCenter[CATRAINING_NUM];
    U8  backup_rank, rank_i;

    //  01010101b -> 10101010b : Golden value = 1001100110011001b=0x9999
    //  11111111b -> 00000000b : Golden value = 0101010101010101b=0x5555
    U32 u4GoldenPattern =0x55555555;
    //U32 u4GoldenPattern =0xaaaaaaaa;

    backup_rank = u1GetRank(p);

    mcSHOW_DBG_MSG(("\n[CATrainingLP3]\n"));
    mcFPRINTF((fp_A60501, "\n[CATrainingLP3]\n"));

    // Fix ODT off. A60501 disable ODT in the init code. So no need to do the following code.
    //uiReg54h=u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WODT), WODT_WODTFIXOFF);
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WODT), 1, WODT_WODTFIXOFF);// According to Derping, should set to 1 to disable ODT.

    // Let MIO_CK always ON.
    uiReg038h=u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL));
    uiReg04ch=u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0)); //backup REFCTRL0_REFDIS register that will be changed at below vAutoRefreshSwitch function
    uiReg064h=u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL));
    uiReg024h=u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL));
    uiMRSRK=u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), MRS_MRSRK);

    vAutoRefreshSwitch(p, DISABLE); //When doing CA training, should make sure that auto refresh is disable

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 1, DRAMC_PD_CTRL_MIOCKCTRLOFF);   //MIOCKCTRLOFF=1
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 0, DRAMC_PD_CTRL_PHYCLKDYNGEN); // set R_DMPHYCLKDYNGEN=0


#ifdef CA_LAG_CK
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA7), u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA7), SHU_SELPH_CA7_DLY_RA2) - 1, SHU_SELPH_CA7_DLY_RA2);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA7), u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA7), SHU_SELPH_CA7_DLY_RA3) - 1, SHU_SELPH_CA7_DLY_RA3);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA7), u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA7), SHU_SELPH_CA7_DLY_RA4) - 1, SHU_SELPH_CA7_DLY_RA4);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA7), u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA7), SHU_SELPH_CA7_DLY_RA5) - 1, SHU_SELPH_CA7_DLY_RA5);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA7), u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA7), SHU_SELPH_CA7_DLY_RA6) - 1, SHU_SELPH_CA7_DLY_RA6);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA7), u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA7), SHU_SELPH_CA7_DLY_RA7) - 1, SHU_SELPH_CA7_DLY_RA7);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA8), u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA8), SHU_SELPH_CA8_DLY_RA8) - 1, SHU_SELPH_CA8_DLY_RA8);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA8), u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA8), SHU_SELPH_CA8_DLY_RA9) - 1, SHU_SELPH_CA8_DLY_RA9);
#endif


    // Step 1.1 : let IO to O1 path valid
    O1PathOnOff(p, 1);

    for (ii=0; ii<CATRAINING_NUM; ii++)
    {
         iLastCAPass[ii] = PASS_RANGE_NA;
         iFirstCAPass[ii] = PASS_RANGE_NA;
    }


    // ----- MR41, CA0~3, CA5~8 -------
    uiMR41 = 1;
    CATrainingEntry(p, uiMR41, u4GoldenPattern);  //MR41
    CATrainingDelayCompare(p, uiMR41, u4GoldenPattern, iCenterSum, iFirstCAPass, iLastCAPass, iCenter, 0);

    // ----- MR48, CA4 and 9 -------
    uiMR41 = 0;
    CATrainingEntry(p, uiMR41, u4GoldenPattern);  //MR48
    CATrainingDelayCompare(p, uiMR41, u4GoldenPattern, iCenterSum, iFirstCAPass, iLastCAPass, iCenter, 0);

    iFinalCACLK[0] =iCenterSum[0]/2;
    iFinalCACLK[1] =iCenterSum[1]/8;

    #if CA_TRAINING_K_RANK1_ENABLE 
        s4CATrainingResult0[p->channel][p->rank] = iFinalCACLK[0];
        s4CATrainingResult1[p->channel][p->rank] = iFinalCACLK[1];
    #endif

    #ifdef ETT_PRINT_FORMAT
    mcSHOW_DBG_MSG(("\n\tGoldenPattern 0x%X, FinalCACLKMacro0 = %d, Macro1 = %d\n\n", u4GoldenPattern, iFinalCACLK[0], iFinalCACLK[1]));
    #else
    mcSHOW_DBG_MSG(("\n\tGoldenPattern 0x%x, FinalCACLKMacro0 = %d, Macro1 = %d\n\n", u4GoldenPattern, iFinalCACLK[0], iFinalCACLK[1]));
    #endif

    mcFPRINTF((fp_A60501, "FinalCACLKMacro0 = %d, Macro1 = %d\n\n", iFinalCACLK[0], iFinalCACLK[1]));

    #if REG_SHUFFLE_REG_CHECK
    ShuffleRegCheck =1;
    #endif


    /* p->rank = RANK_0, save to Reg Rank0 and Rank1, p->rank = RANK_1, save to Reg Rank1 */
    for(rank_i=RANK_0; rank_i<RANK_MAX; rank_i++)
    {
        vSetRank(p, rank_i);
        for(module_i=0; module_i<2; module_i++)
        {
            if(iFinalCACLK[module_i] <0)
            {    //SEt CLK delay
#if CA_TRAIN_RESULT_DO_NOT_MOVE_CLK
                  CATrain_ClkDelay[p->channel][p->rank] =0;  /// should not happen.
                if (module_i==0)
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), 0x10, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CMD);
                else
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), 0x10, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);

                  vSetCalibrationResult(p, DRAM_CALIBRATION_CA_TRAIN, DRAM_FAIL);
                  mcSHOW_ERR_MSG(("Error: CLK dly is not 0 (%d), Set as 0. CA set as 0x10\n\n", -iFinalCACLK[module_i]));
                  mcFPRINTF((fp_A60501, "Error: CLK dly is not 0 (%d), Set as 0. CA set as 0x10\n\n", -iFinalCACLK[module_i]));
                  while(1){};
#else
                if (module_i==0) //choose the smaller CLK delay as final delay, and compensate the other rank of bigger CLK delay by adjusting CA delay 
                {
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), 0, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CMD);
                    CATrain_ClkDelay[p->channel][p->rank] = -iFinalCACLK[module_i];
                }
                else
                {
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), 0, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);
                    if (iFinalCACLK[module_i] < iFinalCACLK[module_i-1])
                    {
                        CATrain_ClkDelay[p->channel][p->rank] = -iFinalCACLK[module_i];
                        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), iFinalCACLK[module_i-1]-iFinalCACLK[module_i], SHU1_R0_CA_CMD9_RG_RK0_ARPI_CMD);
                    }
                    else
                        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), iFinalCACLK[module_i]-iFinalCACLK[module_i-1], SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);
                }


                mcSHOW_DBG_MSG(("Clk dly: %d, CA dly: 0\n\n", -iFinalCACLK[module_i]));
                mcFPRINTF((fp_A60501, "Clk dly: %d, CA dly: 0\n\n", -iFinalCACLK[module_i]));
#endif
            }
            else
            {    // Set CA output delay
                  CATrain_ClkDelay[p->channel][p->rank] = 0;

                if (module_i==0)
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), iFinalCACLK[0], SHU1_R0_CA_CMD9_RG_RK0_ARPI_CMD);
                else
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), iFinalCACLK[1], SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);

                  mcSHOW_DBG_MSG(("Macro%d Clk dly: 0, CA dly: %d\n\n", module_i, iFinalCACLK[module_i]));
                  mcFPRINTF((fp_A60501, "Macro%d Clk dly: 0, CA dly: %d\n\n", module_i, iFinalCACLK[module_i]));
            }
        }

       // no need to enter self refresh before setting CLK under CA training mode
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), CATrain_ClkDelay[p->channel][p->rank] , SHU1_R0_CA_CMD9_RG_RK0_ARPI_CLK);
    }
    vSetRank(p, backup_rank);

    #if REG_SHUFFLE_REG_CHECK
    ShuffleRegCheck =0;
    #endif

    CATrainingExit(p);

    // Disable fix DQ input enable.  Disable IO to O1 path
    O1PathOnOff(p, 0);

    // Disable CKE high, back to dynamic
    CKEFixOnOff(p, CKE_DYNAMIC, CKE_WRITE_TO_ONE_CHANNEL);

    // Restore the registers' values.
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WODT), uiReg54h, WODT_WODTFIXOFF);
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), uiReg038h);
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0), uiReg04ch);
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), uiReg064h);
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), uiReg024h);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), uiMRSRK, MRS_MRSRK);

    return DRAM_OK;
}

DRAM_STATUS_T CATrainingLP3PostProcess(DRAMC_CTX_T *p)
{
    S32 s4FinalMacro[2];
    U8 rank_i, module_i, backup_rank;

    backup_rank = u1GetRank(p);

    s4FinalMacro[0] = (s4CATrainingResult0[p->channel][RANK_0] + s4CATrainingResult0[p->channel][RANK_1])/2;
    s4FinalMacro[1] = (s4CATrainingResult1[p->channel][RANK_0] + s4CATrainingResult1[p->channel][RANK_1])/2;

    /* p->rank = RANK_0, save to Reg Rank0 and Rank1, p->rank = RANK_1, save to Reg Rank1 */
    for(rank_i=RANK_0; rank_i<RANK_MAX; rank_i++)
    {
        vSetRank(p, rank_i);
        for(module_i=0; module_i<2; module_i++)
        {
            if(s4FinalMacro[module_i] <0)
            {    //SEt CLK delay
                 if (module_i==0) //choose the smaller CLK delay as final delay, and compensate the other rank of bigger CLK delay by adjusting CA delay 
                {
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), 0, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CMD);
                    CATrain_ClkDelay[p->channel][p->rank] = -s4FinalMacro[module_i];
                }
                else
                {
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), 0, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);
                    if (s4FinalMacro[module_i] < s4FinalMacro[module_i-1])
                    {
                        CATrain_ClkDelay[p->channel][p->rank] = -s4FinalMacro[module_i];
                        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), s4FinalMacro[module_i-1]-s4FinalMacro[module_i], SHU1_R0_CA_CMD9_RG_RK0_ARPI_CMD);
                    }
                    else
                        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), s4FinalMacro[module_i]-s4FinalMacro[module_i-1], SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);
                }
            }
            else
            {    // Set CA output delay
                CATrain_ClkDelay[p->channel][p->rank] = 0;

                if (module_i==0)
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), s4FinalMacro[0], SHU1_R0_CA_CMD9_RG_RK0_ARPI_CMD);
                else
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), s4FinalMacro[1], SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);

                  mcSHOW_DBG_MSG(("Final Macro%d Clk dly: 0, CA dly: %d\n\n", module_i, s4FinalMacro[module_i]));
                  mcFPRINTF((fp_A60501, "Final Macro%d Clk dly: 0, CA dly: %d\n\n", module_i, s4FinalMacro[module_i]));
            }
        }

       // no need to enter self refresh before setting CLK under CA training mode
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), CATrain_ClkDelay[p->channel][p->rank] , SHU1_R0_CA_CMD9_RG_RK0_ARPI_CLK);
    }

    vSetRank(p, backup_rank);

    return DRAM_OK;
}
#endif
#endif //SIMULATION_LP3_CA_TRAINING


#if SIMUILATION_LP4_CBT
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
#if DUAL_FREQ_K
    if(freq==CBT_LOW_FREQ)
    {
        CBT_DramcDFSDirectJump(p, 1);
    }
    else
    {
        CBT_DramcDFSDirectJump(p, 0);
    }

    //DDRPhyFreqMeter();
#endif
}

static void vSetDramMRCBTOnOff(DRAMC_CTX_T *p, U8 u1OnOff, U8 operating_fsp)
{
    if(u1OnOff)
    {
        u1MR13Value[operating_fsp] |= 0x1; //MR13 OP[0]=1, enable CBT

        // op[7] = !(p->dram_fsp), dram will switch to another FSP_OP automatically
        if(operating_fsp)
            u1MR13Value[operating_fsp] &= 0x7f; // OP[7] =0;
        else
            u1MR13Value[operating_fsp] |= 0x80; // OP[7] =1;

        if (p->dram_cbt_mode[p->rank] == CBT_BYTE_MODE1)
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WRITE_LEV), 1, WRITE_LEV_BYTEMODECBTEN);    //BYTEMODECBTEN=1
        }
    }
    else
    {
        u1MR13Value[operating_fsp] &= 0xfe; //MR13 OP[0]=0, disable CBT

        if(operating_fsp)
            u1MR13Value[operating_fsp] |= 0x80; // OP[7] =1;
        else
            u1MR13Value[operating_fsp] &= 0x7f;  // OP[7] =0;
    }

#if (CBT_K_RANK1_USE_METHOD==1)
        DramcModeRegWriteByRank(p, p->rank, 13, u1MR13Value[operating_fsp]);
#else
        DramcModeRegWriteByRank(p, RANK_0, 13, u1MR13Value[operating_fsp]);
#endif
}


U32 u4CBTRegBak[8];
void CBTEntry(DRAMC_CTX_T *p, U8 operating_fsp, U16 operation_frequency)
{

    u4CBTRegBak[0] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL));
    u4CBTRegBak[1] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_STBCAL));
    u4CBTRegBak[2] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL));
    u4CBTRegBak[3] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_WRITE_LEV));
    u4CBTRegBak[4] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0));
    u4CBTRegBak[5] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL));
        //u4CBTRegBak[6] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM4));

        //Write (DRAMC_AO_BASE+ 0xE<<2) [25] = 1b0 // disable dramc DCMEN
        //Write (DRAMC_AO_BASE+ 0xE<<2) [30] = 1b0 // set R_DMPHYCLKDYNGEN=0
        //Write (DRAMC_AO_BASE+ 0x80<<2) [29] = 1b0 // set R_DMDQSIENCG_NORMAL_EN=0
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), P_Fld(0, DRAMC_PD_CTRL_PHYCLKDYNGEN)| P_Fld(0, DRAMC_PD_CTRL_DCMEN));


        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), 0, STBCAL_DQSIENCG_NORMAL_EN);

        //Step 0.0 CKE go high (Release R_DMCKEFIXOFF, R_DMCKEFIXON=1)
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 1, DRAMC_PD_CTRL_MIOCKCTRLOFF);   //MIOCKCTRLOFF=1

        CKEFixOnOff(p, CKE_FIXON, CKE_WRITE_TO_ONE_CHANNEL);

        //Step 0: MRW MR13 OP[0]=1 to enable CBT
        vSetDramMRCBTOnOff(p, ENABLE, operating_fsp);

        //Step 0.1: before CKE low, Let DQS=0 by R_DMwrite_level_en=1, spec: DQS_t has to retain a low level during tDQSCKE period
        if (p->dram_cbt_mode[p->rank] == CBT_NORMAL_MODE)
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WRITE_LEV), 1, WRITE_LEV_WRITE_LEVEL_EN);
        }

        //Step 1.0: let CKE go low
        CKEFixOnOff(p, CKE_FIXOFF, CKE_WRITE_TO_ONE_CHANNEL);


        // Step 1.1 : let IO to O1 path valid
        if (p->dram_cbt_mode[p->rank] == CBT_NORMAL_MODE)
        {
            // Let R_DMFIXDQIEN1=1 (byte1), 0xd8[13]  ==> Note: Do not enable again.
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PADCTRL), 0x2, PADCTRL_FIXDQIEN);

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
}

void CBTExit(DRAMC_CTX_T *p, U8 operating_fsp, U8 operation_frequency)
{
    U8 u1MROP;

        if (p->dram_cbt_mode[p->rank] == CBT_NORMAL_MODE || p->dram_cbt_mode[p->rank] == CBT_BYTE_MODE1)
        {
            //Step 1: CKE go high (Release R_DMCKEFIXOFF, R_DMCKEFIXON=1)
            CKEFixOnOff(p, CKE_FIXON, CKE_WRITE_TO_ONE_CHANNEL);

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
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PADCTRL), 0, PADCTRL_FIXDQIEN);
        }

        if (p->dram_cbt_mode[p->rank] == CBT_BYTE_MODE1)
        {
            //Let DDRPHY RG_RX_ARDQ_SMT_EN_B0=0
            //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ3), 0, B0_DQ3_RG_RX_ARDQ_SMT_EN_B0);
            O1PathOnOff(p, 0);

            //Disable Byte mode CBT enable bit
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WRITE_LEV), 0, WRITE_LEV_BYTEMODECBTEN);    //BYTEMODECBTEN=0
        }

        // Restore reg settings
        //release R_DMCKEFIXON, Release R_DMREFCNT, R_DMREFCNT_FR_CLK
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), u4CBTRegBak[0] );
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), u4CBTRegBak[1] );
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), u4CBTRegBak[2] );
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_WRITE_LEV), u4CBTRegBak[3] );
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0), u4CBTRegBak[4] );
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), u4CBTRegBak[5] );
    //vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM4), u4CBTRegBak[6] );
}

static void CBTSetFSP(DRAMC_CTX_T *p, U8 operating_fsp, U8 final_set_flag)
{
    if(operating_fsp==FSP_0)
    {
        u1MR13Value[operating_fsp] &= ~(1<<6); //OP[6], fsp_wr=0
        u1MR13Value[operating_fsp] &= 0x7f; // OP[7] =0;        
    }
    else
    {
#if (DUAL_FREQ_K)
        u1MR13Value[operating_fsp] |= (1<<6); //OP[6], fsp_wr=1        
        u1MR13Value[operating_fsp] &= 0x7f;  // OP[7] =0;
#else
        u1MR13Value[operating_fsp] |= (1<<6); //OP[6], fsp_wr=1
        u1MR13Value[operating_fsp] |= 0x80;  // OP[7] =1;
#endif
    }

    DramcModeRegWriteByRank(p, p->rank, 13, u1MR13Value[operating_fsp]);
}

static void CBTSetVref(DRAMC_CTX_T *p, U32 u2VrefLevel, U8 operating_fsp, U8 final_set_flag)
{
    U32 u4DbgValue;
    U8 u1VrefValue_pinmux;

    u1VrefValue_pinmux = (GetCmdBusTrainingVrefPinMuxRevertValue(p, u2VrefLevel) & 0x3f);

#if !REDUCE_LOG_FOR_PRELOADER
    mcSHOW_DBG_MSG(("\nCH_%d, RK_%d, Range=%d, VrefValue_pinmux = 0x%x\n",p->channel, p->rank, gCBT_VREF_RANGE_SEL, u1VrefValue_pinmux));
#endif
    mcFPRINTF((fp_A60501, "\nCBTSetVref = 0x%x\n", u2VrefLevel));


    if (p->dram_cbt_mode[p->rank] == CBT_NORMAL_MODE && final_set_flag==0)
    {
        u1MR12Value[p->channel][p->rank][operating_fsp] = ((gCBT_VREF_RANGE_SEL&0x1) <<6) | u1VrefValue_pinmux;    

        //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WRITE_LEV), ((gCBT_VREF_RANGE_SEL&0x1) <<6) | (u2VrefLevel & 0x3f), WRITE_LEV_DMVREFCA);  //MR12, bit[25:20]=OP[5:0]  bit 26=OP[6]
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WRITE_LEV), u2VrefLevel, WRITE_LEV_DMVREFCA);  //MR12, bit[25:20]=OP[5:0]  bit 26=OP[6]

         //DQS_SEL=1, DQS_B1_G=1, Toggle R_DMDQS_WLEV (1 to 0)
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WRITE_LEV), 1 , WRITE_LEV_DQS_SEL);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WRITE_LEV), 0xa , WRITE_LEV_DQSBX_G);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WRITE_LEV), 1 , WRITE_LEV_DQS_WLEV);
        mcDELAY_US(1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WRITE_LEV), 0, WRITE_LEV_DQS_WLEV);

    }
    else
    {
    u4DbgValue = (((gCBT_VREF_RANGE_SEL&0x1) <<6) | (u2VrefLevel & 0x3f));
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

    if(iDelay <0)
    {    //Set CLK delay
          vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), 0, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CMD);
          vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), -iDelay, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CLK);
          vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), -iDelay, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CS);
    }
    else
    {    // Set CA output delay
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), iDelay, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CMD);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), 0, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CLK);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), 0, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CS);

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
    U8 *uiLPDDR_PHY_Mapping = NULL;
    U32 u4Result, u4Ready;
    U32 u4TimeCnt;
    U32 u4dq_o1;
    U32 u4byte_index;

    u4TimeCnt = TIME_OUT_CNT;

    if (u1IsLP4Family(p->dram_type))
    {
        uiLPDDR_PHY_Mapping = (U8 *)uiLPDDR4_PHY_Mapping_POP[p->channel];
    }
#if ENABLE_LP3_SW
    else
    {
        uiLPDDR_PHY_Mapping = (U8 *)uiLPDDR3_PHY_Mapping_POP;
    }
#endif /* ENABLE_LP3_SW */

    // Set CS output delay
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), uiDelay, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CS);

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
        u4Result |= (((u4dq_o1 & (1<<uiLPDDR_PHY_Mapping[u4byte_index])) >> (uiLPDDR_PHY_Mapping[u4byte_index])) << (u4byte_index-8));
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

    for(irank=RANK_0; irank<=RANK_1; irank++)
    {
        vSetRank(p, irank);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), P_Fld(iCSFinalClkDelay, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CLK) |
                                                                    P_Fld(iCSFinalCmdDelay, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CMD) |
                                                                    P_Fld(iCSFinalCSDelay, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CS));
    }

    mcSHOW_DBG_MSG(("Clk Dly = %d\nCmd Dly = %d\nCS Dly = %d\n", iCSFinalClkDelay, iCSFinalCmdDelay, iCSFinalCSDelay));


#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    if(p->pSavetimeData->femmc_Ready==0 || ((p->pSavetimeData->fcalfail[p->freq_sel] & 0x40)>>6)==1)
    {
        p->pSavetimeData->u1CBTClkDelay_Save[p->channel][RANK_0][_MappingFreqArray[p->freq_sel]]= iCSFinalClkDelay;
        p->pSavetimeData->u1CBTClkDelay_Save[p->channel][RANK_1][_MappingFreqArray[p->freq_sel]]= iCSFinalClkDelay;
        
        p->pSavetimeData->u1CBTCmdDelay_Save[p->channel][RANK_0][_MappingFreqArray[p->freq_sel]]= iCSFinalCmdDelay;
        p->pSavetimeData->u1CBTCmdDelay_Save[p->channel][RANK_1][_MappingFreqArray[p->freq_sel]]= iCSFinalCmdDelay;        

        p->pSavetimeData->u1CBTCsDelay_Save[p->channel][RANK_0][_MappingFreqArray[p->freq_sel]]= iCSFinalCSDelay;
        p->pSavetimeData->u1CBTCsDelay_Save[p->channel][RANK_1][_MappingFreqArray[p->freq_sel]]= iCSFinalCSDelay;                
    }
#endif

    vSetRank(p, backup_rank);

}

static void CBTAdjustCS(DRAMC_CTX_T *p)
{
    S32 iFirstCSPass, iLastCSPass, iCSFinalDelay;//iCSCenter
    U32 uiDelay, u4ValueReadBack, u4CSWinSize;
    U8 backup_rank, ii;

    backup_rank = u1GetRank(p);

 #if (SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_CBT)
	if(p->pSavetimeData->femmc_Ready==1 )
  	{

		iCSFinalDelay=p->pSavetimeData->u1CBTCsDelay_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]];


        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), iCSFinalDelay, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CS);
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

#if 0
    iCSCenter = (iFirstCSPass + iLastCSPass) >>1;

    //Set CS output delay after training
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_ARPI_CMD), iCSCenter, ARPI_CMD_DA_ARPI_CS);
    mcSHOW_DBG_MSG(("\n[CmdBusTrainingLP4] CS Dly: %d\n", iCSCenter));
    mcFPRINTF((fp_A60501, "\n[CmdBusTrainingLP4] CS Dly: %d\n", iCSCenter));
#else
    u4CSWinSize = iLastCSPass - iFirstCSPass + 1;

    if(u4CSWinSize > ((MAX_CS_PI_DELAY+1)>>1))  // if winSize >32, CS delay= winSize -32.
    {
        iCSFinalDelay = u4CSWinSize -((MAX_CS_PI_DELAY+1)>>1);
    }
    else  ///TODO: need to delay CLK?  A60817 and A60501 cannot move CLK PI due to multi_phase problem.
    {
        iCSFinalDelay =0;
    }

    //Set CS output delay after training
    /* p->rank = RANK_0, save to Reg Rank0 and Rank1, p->rank = RANK_1, save to Reg Rank1 */
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), iCSFinalDelay, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CS);

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
 if(p->pSavetimeData->femmc_Ready==0 || ((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x40)>>6)==1)
 {
	p->pSavetimeData->u1CBTCsDelay_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]]= iCSFinalDelay;
 }
#endif

    mcSHOW_DBG_MSG(("\n[CmdBusTrainingLP4] CS Dly: %d, CSPass First: %d, Last: %d\n", iCSFinalDelay, iFirstCSPass,  iLastCSPass));
    mcFPRINTF((fp_A60501, "\n[CmdBusTrainingLP4] CS dly: %d, CSPass First: %d, Last: %d\n", iFirstCSPass, iCSFinalDelay));

    CATrain_CsDelay[p->channel][p->rank] = iCSFinalDelay;

    #if 0//VENDER_JV_LOG
    mcSHOW_DBG_MSG5(("\n[CmdBusTrainingLP4] CS Dly is %d\n", iCSFinalDelay));
    #endif
#endif
}
}

static void CBTSetCACLKResult(DRAMC_CTX_T *p)
{
    U8 backup_rank, ii;

    backup_rank = u1GetRank(p);

    //DramcEnterSelfRefresh(p, 1);  //enter self refresh mode when changing CLK

    // Set CA_PI_Delay after trainging
    /* p->rank = RANK_0, save to Reg Rank0 and Rank1, p->rank = RANK_1, save to Reg Rank1 */
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), CATrain_CmdDelay[p->channel][p->rank], SHU1_R0_CA_CMD9_RG_RK0_ARPI_CMD);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), CATrain_ClkDelay[p->channel][p->rank], SHU1_R0_CA_CMD9_RG_RK0_ARPI_CLK);

    //DramcEnterSelfRefresh(p, 0);
    mcSHOW_DBG_MSG(("\n[CBTSetCACLKResult] CA Dly = %d, CLK dly =%d\n", CATrain_CmdDelay[p->channel][p->rank], CATrain_ClkDelay[p->channel][p->rank]));
    mcFPRINTF((fp_A60501, "\n[CBTSetCACLKResult] CA Dly = %d, CLK dly =%d\n", CATrain_CmdDelay[p->channel][p->rank], CATrain_ClkDelay[p->channel][p->rank]));
}


U8 GetCmdBusTrainingVrefPinMuxValue(DRAMC_CTX_T *p, U8 u1VrefLevel)
{
    U8 u2VrefBit, u2Vref_new, u2Vref_org;

    if (p->dram_cbt_mode[p->rank] == CBT_BYTE_MODE1) return ((gCBT_VREF_RANGE_SEL&0x1) <<6) | (u1VrefLevel & 0x3f);

    
    u2Vref_new = 0;
    u2Vref_org = ((gCBT_VREF_RANGE_SEL&0x1) <<6) | (u1VrefLevel & 0x3f);
    for (u2VrefBit = 0; u2VrefBit < 8; u2VrefBit++)
    {
//        mcSHOW_DBG_MSG(("=== u2VrefBit: %d, %d\n",u2VrefBit,uiLPDDR4_PHY_Mapping_POP[p->channel][u2VrefBit]));
        if (u2Vref_org & (1 << u2VrefBit))
        {
            u2Vref_new |=  (1 << uiLPDDR4_PHY_Mapping_POP[p->channel][u2VrefBit]);
//            mcSHOW_DBG_MSG(("=== u2VrefBit: %d, %d, u2Vref_org: %x, u2Vref_new: 0x%x\n",u2VrefBit,uiLPDDR4_PHY_Mapping_POP[p->channel][u2VrefBit],u2Vref_org,u2Vref_new));
        }
    }

    mcSHOW_DBG_MSG3(("=== u2Vref_new: 0x%x --> 0x%x\n",u2Vref_org,u2Vref_new));

    return u2Vref_new;                  
}    

U8 GetCmdBusTrainingVrefPinMuxRevertValue(DRAMC_CTX_T *p, U8 u1VrefLevel)
{
    U8 u2VrefBit, u2Vref_new, u2Vref_org;

    if (p->dram_cbt_mode[p->rank] == CBT_BYTE_MODE1) return u1VrefLevel;
    
    u2Vref_new = 0;
    u2Vref_org = u1VrefLevel;
    for (u2VrefBit = 0; u2VrefBit < 8; u2VrefBit++)
    {
        u2Vref_new |=  ((u2Vref_org>>uiLPDDR4_PHY_Mapping_POP[p->channel][u2VrefBit])&1) << u2VrefBit;
    }

    mcSHOW_DBG_MSG3(("=== Revert u2Vref_new: 0x%x --> 0x%x\n",u2Vref_org,u2Vref_new));

    return u2Vref_new;                  
}   

DRAM_STATUS_T CmdBusTrainingLP4(DRAMC_CTX_T *p)
{
    U8 u1VrefLevel, uiFinalVref;
    U32 u4CompareResult;
    PASS_WIN_DATA_T FinalWinPerCA[CATRAINING_NUM_LP4];

    U32 uiCA, uiFinishCount, uiTemp;
    S16 iDelay;
    S32 iFirstCAPass[CATRAINING_NUM_LP4], iLastCAPass[CATRAINING_NUM_LP4], iFinalCACLK;
    U32 uiCAWinSum, uiCAWinSumMax;
    U32 uiReg04ch;
    S32 iCACenter[CATRAINING_NUM_LP4], iCACenterSum, iCAFinalCenter[CATRAINING_NUM_LP4] = {0};
        U8 operating_fsp;
        U16 operation_frequency;
#ifdef CBT_VREFCA_EYE_SCAN
    U8  irange;
    U8  uiFinalRange;
#endif
    U8 *uiLPDDR_PHY_Mapping;
    S32 iCK_MIN=1000, iCA_PerBit_DelayLine[CATRAINING_NUM_LP4] = {0};
    U8 u1EnableDelayCell; 
#if CA_PER_BIT_DELAY_CELL    
    U32 u1DelayCellOfst[CATRAINING_NUM_LP4];
#endif

    uiReg04ch = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0)); //backup REFCTRL0_REFDIS register that will be changed at below vAutoRefreshSwitch function

    vAutoRefreshSwitch(p, DISABLE); //When doing CA training, should make sure that auto refresh is disable


    if(u1IsLP4Family(p->dram_type))
    {
        uiLPDDR_PHY_Mapping = (U8 *)uiLPDDR4_PHY_Mapping_POP[p->channel];
    }
    #if ENABLE_LP3_SW
    else
    {
        uiLPDDR_PHY_Mapping = (U8 *)uiLPDDR3_PHY_Mapping_POP;
    }
    #endif

#if (CBT_K_RANK1_USE_METHOD==1)
    if (p->rank == RANK_1)
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), p->rank, MRS_MRSRK);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), p->rank, RKCFG_TXRANK); //use other rank's setting 
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), 1, RKCFG_TXRANKFIX); //TXRANKFIX should be write after TXRANK or the rank will be fix at rank 1                
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MPC_OPTION), 0, MPC_OPTION_MPCRKEN);
    }
#else
    if (p->rank == RANK_1)
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), RANK_0, MRS_MRSRK);

        DramcRankSwap(p, RANK_1);
    }
#endif

    // SW variable initialization
    uiCAWinSumMax=0;
    uiFinalVref=u1MR12Value[p->channel][p->rank][p->dram_fsp] & 0x3f;
    iFinalCACLK=0;

    operating_fsp = p->dram_fsp;
    operation_frequency = p->frequency;

    if (p->dram_cbt_mode[p->rank] == CBT_NORMAL_MODE)
    {
        #if (fcFOR_CHIP_ID == fcBianco)
        uiLPDDR_PHY_Mapping = (U8 *)uiLPDDR4_PHY_Mapping_POP[p->channel];

        //Set MRR pin mux
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_MRR_BIT_MUX1), P_Fld( uiLPDDR_PHY_Mapping[8], MRR_BIT_MUX1_MRR_BIT0_SEL) 
                                                                  | P_Fld( uiLPDDR_PHY_Mapping[9], MRR_BIT_MUX1_MRR_BIT1_SEL) 
                                                                  | P_Fld( uiLPDDR_PHY_Mapping[10], MRR_BIT_MUX1_MRR_BIT2_SEL) 
                                                                  | P_Fld( uiLPDDR_PHY_Mapping[11], MRR_BIT_MUX1_MRR_BIT3_SEL));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_MRR_BIT_MUX2), P_Fld( uiLPDDR_PHY_Mapping[12], MRR_BIT_MUX2_MRR_BIT4_SEL) 
                                                                  | P_Fld( uiLPDDR_PHY_Mapping[13], MRR_BIT_MUX2_MRR_BIT5_SEL));
        #endif
    }

///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Note : (Todo) uiLPDDR_PHY_Mapping_POP, need to take care mapping in real chip, but not in test chip.
///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Note : Assume that there is a default CS value that can apply for CA.

    if (p->dram_cbt_mode[p->rank] == CBT_NORMAL_MODE)
    {
        //switch to low freq
        if (operating_fsp == FSP_1)
        {
                CBT_Switch_Freq(p, CBT_LOW_FREQ);
        }
                
        // Step1: Enter Command Bus Training Mode
        CBTEntry(p, operating_fsp, operation_frequency);
        //Step 2: wait tCAENT
        mcDELAY_US(1);

        //switch to high freq
        if (operating_fsp == FSP_1)
        {
                CBT_Switch_Freq(p, CBT_HIGH_FREQ);
        }        
    }


#if VENDER_JV_LOG
    vPrintCalibrationBasicInfo_ForJV(p);
#else
    vPrintCalibrationBasicInfo(p);
#endif

        //Step 3: set CBT range, verify range and setp
#if (SW_CHANGE_FOR_SIMULATION ||FOR_DV_SIMULATION_USED)
        gCBT_VREF_RANGE_SEL = 0;  //MR12,OP[6]
        gCBT_VREF_RANGE_BEGIN = 0;
        gCBT_VREF_RANGE_END = 2; // binary 110010
        gCBT_VREF_RANGE_STEP = 2;
#else
        gCBT_VREF_RANGE_SEL = 1;  //MR12,OP[6]

        gCBT_VREF_RANGE_STEP = 2;

#if SIMPLIFY_VREF_SCAN
        if (p->skipVrefScan == ENABLE)
        {
            mcSHOW_DBG_MSG(("Skip Vref(ca) scan. Setting: %d\n", u1MR12Value[p->channel][p->rank][p->dram_fsp] & 0x3f));
            mcFPRINTF((fp_A60501, "Skip Vref(ca) scan. Setting: %d\n", u1MR12Value[p->channel][p->rank][p->dram_fsp] & 0x3f));
        }
#endif /* SIMPLIFY_VREF_SCAN */

#if SIMPLIFY_VREF_SCAN
        if ((p->enable_cbt_scan_vref == DISABLE) || (p->skipVrefScan == ENABLE))
#else
        if (p->enable_cbt_scan_vref == DISABLE)
#endif
        {
            gCBT_VREF_RANGE_BEGIN = (u1MR12Value[p->channel][p->rank][p->dram_fsp] & 0x3f);
            gCBT_VREF_RANGE_END = gCBT_VREF_RANGE_BEGIN;
        }
#if SUPPORT_SAVE_OFF_LINE_CALIBRATION
        else if((gCBT_Vref_Scan_Check[p->channel][p->rank][p->dram_fsp]==0) && (p->pOffLineSaveData->u2CAWinSum_Save[p->channel][p->rank][p->dram_fsp]>0))
        {
            gCBT_VREF_RANGE_BEGIN = p->pOffLineSaveData->u1CAVref_Save[p->channel][p->rank][p->dram_fsp];
            gCBT_VREF_RANGE_END = gCBT_VREF_RANGE_BEGIN;
        }
#endif
        else
        {
#if 0 //SUPPORT_SAVE_TIME_FOR_CALIBRATION
		if(p->pSavetimeData->femmc_Ready==1 && ((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x40)>>6)==0)
  		{
             gCBT_VREF_RANGE_BEGIN = p->pSavetimeData->u1CBTVref_Save[p->channel][p->rank][p->freq_sel]-p->pSavetimeData->u1CBTVrefoffset;		
          
  		}
  	else
  #endif
  	{
                if (p->dram_type == TYPE_LPDDR4)            
                {
                    //range 1          
                    gCBT_VREF_RANGE_BEGIN = 13 - 5; // 300/1100(VDDQ) = 27.2%
                    gCBT_VREF_RANGE_END = 13 + 5; 
                }
                else
                {
#if PU_CAL_05VDDQ
                    //range 1          
                    gCBT_VREF_RANGE_BEGIN = 14 - 5; // 250/600(VDDQ)=41.6%
                    gCBT_VREF_RANGE_END = 14 + 5;                 
#else
                    //range 1          
                    gCBT_VREF_RANGE_BEGIN = 27 - 5; // 290/600(VDDQ)=48.3%
                    gCBT_VREF_RANGE_END = 27 + 5;                 
#endif                    
                }
  	}   
        }
#endif

#if (VREF_SPEED_UP_LP4 == 0)
#ifdef CBT_VREFCA_EYE_SCAN
    for(irange=0; irange<=1; irange++)
    {
        gCBT_VREF_RANGE_SEL = irange;
#endif

        gCBT_VREF_RANGE_BEGIN = 0;
        gCBT_VREF_RANGE_END = 50;
        gCBT_VREF_RANGE_STEP = 1;

        if (gCBT_VREF_RANGE_SEL == 1)
        {
            gCBT_VREF_RANGE_BEGIN = 21;
        }
#endif
#if (SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_VREF_CAL)
		if(p->pSavetimeData->femmc_Ready==1 && ((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x40)>>6)==0)
  		{
                        mcSHOW_DBG_MSG(("\nBYPASS_VREF_CAL CBT\n"));
            //gCBT_VREF_RANGE_BEGIN = p->pSavetimeData->u1CBTVref_Save[p->channel][p->rank][p->freq_sel];
            //gCBT_VREF_RANGE_END = gCBT_VREF_RANGE_BEGIN;
  		}
		else
#endif
{
    for(u1VrefLevel =gCBT_VREF_RANGE_BEGIN; u1VrefLevel<=gCBT_VREF_RANGE_END; u1VrefLevel+=gCBT_VREF_RANGE_STEP)
    {
        if (p->dram_cbt_mode[p->rank] == CBT_NORMAL_MODE)
        {
#if(fcFOR_CHIP_ID== fcBianco)            
            if (GetCmdBusTrainingVrefPinMuxValue(p, u1VrefLevel)>0x7f)
            {
                continue;
            }
#endif            
        }
    
        //VREFCA = you want
        {
            if (p->dram_cbt_mode[p->rank] == CBT_BYTE_MODE1)
            {
                #if (FOR_DV_SIMULATION_USED==0)
                //switch to low freq
                if (operating_fsp == FSP_1)
                {
                        CBT_Switch_Freq(p, CBT_LOW_FREQ);
                }
                #endif
                    CBTSetFSP(p, operating_fsp, 0);
            }
            CBTSetVref(p, GetCmdBusTrainingVrefPinMuxValue(p, u1VrefLevel), operating_fsp, 0);
        }
        
#if VENDER_JV_LOG
        mcSHOW_DBG_MSG5(("\n\tLP4 CBT VrefRange %d, VrefLevel=%d\n", gCBT_VREF_RANGE_SEL, u1VrefLevel));
#endif

        if (p->dram_cbt_mode[p->rank] == CBT_BYTE_MODE1)
        {
            // Step1: Enter Command Bus Training Mode
            CBTEntry(p, operating_fsp, operation_frequency);
            //Step 2: wait tCAENT
            mcDELAY_US(1);

            #if (FOR_DV_SIMULATION_USED==0)
            //switch to high freq
            if (operating_fsp == FSP_1)
            {
                CBT_Switch_Freq(p, CBT_HIGH_FREQ);
            }
            #endif
        }

        // Delay CA output delay to do CA training in order to get the pass window.
        // moving CA relative to CK and repeating until CA is centered on the latching edge of CK

        // Note  !!!!!!!!!!!!!!!!!!!!!!!
        // Assume : Leave clk as the init value and adjust CA delay only can find out each CA window including of the left boundary.
        // If NOT, we may need to off-line adjust 0x404 SELPH2_TXDLY_CMD

        // SW variable initialization
        uiFinishCount = 0;
        uiCAWinSum = 0;
        iCACenterSum = 0;
        for (uiCA=0; uiCA<CATRAINING_NUM_LP4; uiCA++)
        {
             iLastCAPass[uiCA] = PASS_RANGE_NA;
             iFirstCAPass[uiCA] = PASS_RANGE_NA;

             iCA_PerBit_DelayLine[uiCA] = 0;
             iCACenter[uiCA] = 0;
        }

        for (iDelay= (-MAX_CLK_PI_DELAY); iDelay<=MAX_CA_PI_DELAY; iDelay++)
        {
            u4CompareResult= CBTDelayCACLKCompare(p, iDelay);

            //wait 1us between each CA pattern
            mcDELAY_US(1);

            mcSHOW_DBG_MSG3(("CBTDelayCACLK Delay= %d, CompareResult 0x%x\n", iDelay, u4CompareResult));
            mcFPRINTF((fp_A60501, "CBTDelayCACLK Delay = %d, CompareResult 0x%x\n", iDelay, u4CompareResult));

            for (uiCA=0; uiCA<CATRAINING_NUM_LP4; uiCA++)
            {
                if((iFirstCAPass[uiCA] != PASS_RANGE_NA) && (iLastCAPass[uiCA] != PASS_RANGE_NA))
                {
                    continue;
                }

                uiTemp = (u4CompareResult >>uiCA) & 0x1; //Get Each bit of CA result

                if(iFirstCAPass[uiCA] == PASS_RANGE_NA)
                {
                    if(uiTemp==0) //compare correct: pass
                    {
                        iFirstCAPass[uiCA] = iDelay;
                    }
                }
                else if(iLastCAPass[uiCA] == PASS_RANGE_NA)
                {
                    if(uiTemp==1) //compare error : fail
                    {
                        if ((iDelay-iFirstCAPass[uiCA]) < 5) // prevent glitch
                        {
                            iFirstCAPass[uiCA] = PASS_RANGE_NA;
                            continue;
                        }

                        iLastCAPass[uiCA] = (iDelay-1);
                    }
                    else if (iDelay==MAX_CA_PI_DELAY)
                    {
                        iLastCAPass[uiCA] = iDelay;
                    }

                    if(iLastCAPass[uiCA] !=PASS_RANGE_NA)
                    {
                        uiFinishCount++;
                        uiCAWinSum += (iLastCAPass[uiCA] -iFirstCAPass[uiCA]);  //Sum of CA Windows for vref selection
                        iCACenter[uiCA] = (iLastCAPass[uiCA] +iFirstCAPass[uiCA])>>1;     // window center of each CA bit
                        iCACenterSum += iCACenter[uiCA];

#if !REDUCE_LOG_FOR_PRELOADER
                        mcSHOW_DBG_MSG(("\n[CA %d] Center %d (%d~%d)\n", uiCA, iCACenter[uiCA] , iFirstCAPass[uiCA], iLastCAPass[uiCA] ));
#endif
#ifdef CBT_VREFCA_EYE_SCAN
                        mcSHOW_DBG_MSG(("\n[CA %d] Center %d (%d~%d)\n", uiCA, ((iLastCAPass[uiCA] +iFirstCAPass[uiCA])>>1) , iFirstCAPass[uiCA], iLastCAPass[uiCA] ));
#endif

                    }
                }
            }

            // Wait tCACD(22clk) before output CA pattern to DDR again..
            mcDELAY_US(1);

            if(uiFinishCount == CATRAINING_NUM_LP4)
                break;
       }

#if VENDER_JV_LOG
        for (uiCA=0; uiCA<CATRAINING_NUM_LP4; uiCA++)
        {
            mcSHOW_DBG_MSG5(("CBT Bit%d, CA window %d ps\n", uiCA, (iLastCAPass[uiCA]-iFirstCAPass[uiCA]+1)*1000000/p->frequency/64));
        }
#endif

        //set CA/CK/CS pi delay to 0
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), 0, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CMD);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), 0, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CLK);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), 0, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CS);

#if !REDUCE_LOG_FOR_PRELOADER
        mcSHOW_DBG_MSG(("\n[CmdBusTrainingLP4] CAWinSum: %d\n", uiCAWinSum));
#endif

        if(uiCAWinSum > uiCAWinSumMax)
        {
            uiCAWinSumMax =uiCAWinSum;
            uiFinalVref = u1VrefLevel;
#ifdef CBT_VREFCA_EYE_SCAN
            uiFinalRange = gCBT_VREF_RANGE_SEL;
#endif
            #if SW_CHANGE_FOR_SIMULATION
            iFinalCACLK =(int)((float) iCACenterSum/(float)CATRAINING_NUM_LP4);
            #else
            if (p->frequency >= 1600)
            {
                iCK_MIN = 1000;
                for (uiCA=0; uiCA<CATRAINING_NUM_LP4; uiCA++) 
                {
                    iCAFinalCenter[uiCA] = iCACenter[uiCA];
                    if (iCAFinalCenter[uiCA] < iCK_MIN) iCK_MIN = iCAFinalCenter[uiCA];
                }

                if (iCK_MIN<0)
                {
                    mcSHOW_ERR_MSG(("Warning!! smallest CA min center = %d < 0, then adjust to 0 !!\n",iCK_MIN));                
                    for (uiCA=0; uiCA<CATRAINING_NUM_LP4; uiCA++) 
                    {
                        if (iCAFinalCenter[uiCA] < 0) iCAFinalCenter[uiCA] = 0;
                    }             
                    iCK_MIN = 0;
                }
                
                iFinalCACLK = iCK_MIN;
            }
            else
            {
                iFinalCACLK = iCACenterSum/CATRAINING_NUM_LP4;
            }
            #endif

            for (uiCA=0; uiCA<CATRAINING_NUM_LP4; uiCA++)
            {
                FinalWinPerCA[uiCA].first_pass = iFirstCAPass[uiCA];
                FinalWinPerCA[uiCA].last_pass = iLastCAPass[uiCA];
                FinalWinPerCA[uiCA].win_center = iCACenter[uiCA];
                FinalWinPerCA[uiCA].win_size = (iLastCAPass[uiCA] - iFirstCAPass[uiCA])+1;
#ifdef FOR_HQA_TEST_USED            
                gFinalCBTCA[p->channel][p->rank][uiCA] = (iLastCAPass[uiCA] - iFirstCAPass[uiCA])+1;
#endif
            }
        }

        if (p->dram_cbt_mode[p->rank] == CBT_BYTE_MODE1)
        {
#if (FOR_DV_SIMULATION_USED==0)
            //switch to low freq
            if (operating_fsp == FSP_1)
            {
                 CBT_Switch_Freq(p, CBT_LOW_FREQ);
            }
#endif
            // Step1: Enter Command Bus Training Mode
            CBTExit(p, operating_fsp, operation_frequency);
            //Step 2: wait tCAENT
            mcDELAY_US(1);
        }

#if VREF_SPEED_UP_LP4
#ifndef CBT_VREFCA_EYE_SCAN
        if(uiCAWinSum < (uiCAWinSumMax*95/100))
        {
            mcSHOW_DBG_MSG(("\nCBT Vref found, early break!\n"));
            break;//max vref found, early break;
        }
#endif        
#endif
    }
    	}
#ifdef CBT_VREFCA_EYE_SCAN
    }
    gCBT_VREF_RANGE_SEL = uiFinalRange;
#endif

#if (SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_VREF_CAL)
    if(p->pSavetimeData->femmc_Ready==0 || ((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x40)>>6)!=0)
#endif
    {
        for (uiCA=0; uiCA<CATRAINING_NUM_LP4; uiCA++)
        {
            mcSHOW_DBG_MSG(("[CA %d] Center %d (%d~%d) winsize %d\n", uiCA, FinalWinPerCA[uiCA].win_center , FinalWinPerCA[uiCA].first_pass, FinalWinPerCA[uiCA].last_pass, FinalWinPerCA[uiCA].win_size ));
        }
    }


    #if SIMPLIFY_VREF_SCAN
    if (p->skipVrefScan == DISABLE)
    #endif
    {
        #if SUPPORT_SAVE_OFF_LINE_CALIBRATION
        // First CBT, need to check window sum
        mcSHOW_DBG_MSG(("\n[CmdBusTrainingLP4] SUPPORT_SAVE_OFF_LINE_CALIBRATION  re-calibrate vref "));
        if(gCBT_Vref_Scan_Check[p->channel][p->rank][p->dram_fsp] ==0)
        {
            if(uiCAWinSumMax < p->pOffLineSaveData->u2CAWinSum_Save[p->channel][p->rank][p->dram_fsp] *9/10)
            {
                // Check Window sum, need to re-calibrate Vref
                gCBT_Vref_Scan_Check[p->channel][p->rank][p->dram_fsp] =1;
                mcSHOW_DBG_MSG(("required\n"));
                return DRAM_K_VREF;
            }
            else
            {
                // Check Window sum, "NO" need to re-calibrate Vref
                mcSHOW_DBG_MSG(("not required\n"));
            }

            mcSHOW_DBG_MSG(("CAWinSumMax %d, CAWinSum_Save*0.9=%d\n",
                                uiCAWinSumMax, p->pOffLineSaveData->u2CAWinSum_Save[p->channel][p->rank][p->dram_fsp] * 9 / 10));
        }
        else
        {
            // re-calibrate Vref done, update new Vref and Window sum
            mcSHOW_DBG_MSG(("done\n"));
            p->pOffLineSaveData->u1CAVref_Save[p->channel][p->rank][p->dram_fsp] = uiFinalVref;
            p->pOffLineSaveData->u2CAWinSum_Save[p->channel][p->rank][p->dram_fsp] = uiCAWinSumMax;
        }
        #endif
    }

 #if (SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_VREF_CAL)
	if(p->pSavetimeData->femmc_Ready==1 )
  	{
	uiFinalVref=p->pSavetimeData->u1CBTVref_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]];
  	}
#endif
    // Set Vref after trainging
#if SIMPLIFY_VREF_SCAN
    if (p->skipVrefScan == DISABLE)
#endif
    {
        if (p->dram_cbt_mode[p->rank] == CBT_BYTE_MODE1)
        {
            #if (FOR_DV_SIMULATION_USED==0)
            //switch to low freq
            if (operating_fsp == FSP_1)
            {
                    CBT_Switch_Freq(p, CBT_LOW_FREQ);
            }
            #endif

            CBTSetFSP(p, operating_fsp, 1);
            CBTSetVref(p, GetCmdBusTrainingVrefPinMuxValue(p,uiFinalVref), operating_fsp, 1);
        }
        else
        {
            CBTSetVref(p, GetCmdBusTrainingVrefPinMuxValue(p,uiFinalVref), operating_fsp, 0);  //Francis, normol mode go DQ pin set vref, don't set final_set_flag here
        }
    }

    mcSHOW_DBG_MSG(("\n[CmdBusTrainingLP4] Vref(ca) range %d: %d\n", gCBT_VREF_RANGE_SEL, uiFinalVref));
    mcFPRINTF((fp_A60501, "\n[CmdBusTrainingLP4] Vref(ca) is Range %d: %d\n", gCBT_VREF_RANGE_SEL, uiFinalVref));
#if VENDER_JV_LOG
    mcSHOW_DBG_MSG5(("\n[CmdBusTrainingLP4] Vref(ca) range %d: %d\n", gCBT_VREF_RANGE_SEL, uiFinalVref));
#endif

#ifdef FOR_HQA_TEST_USED
    gFinalCBTVrefCA[p->channel][p->rank] = uiFinalVref & 0x3f;
#endif

    if (p->dram_cbt_mode[p->rank] == CBT_BYTE_MODE1)
    {
        // Step1: Enter Command Bus Training Mode
        CBTEntry(p, operating_fsp, operation_frequency);
        //Step 2: wait tCAENT
        mcDELAY_US(1);

#if (FOR_DV_SIMULATION_USED==0)
        //switch to high freq
        if (operating_fsp == FSP_1)
        {
            CBT_Switch_Freq(p, CBT_HIGH_FREQ);
        }
#endif
    }

    // Set CA_PI_Delay after trainging
 #if (SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_CBT)
	if(p->pSavetimeData->femmc_Ready==1 )
  	{
		CATrain_ClkDelay[p->channel][p->rank]=p->pSavetimeData->u1CBTClkDelay_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]];
		CATrain_CmdDelay[p->channel][p->rank]=p->pSavetimeData->u1CBTCmdDelay_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]];
		CATrain_CsDelay[p->channel][p->rank]=p->pSavetimeData->u1CBTCmdDelay_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]];
    vSetCalibrationResult(p, DRAM_CALIBRATION_CA_TRAIN, DRAM_OK); // set default result OK, udpate status when per bit fail
  	}
	else
#endif
	{
    if(iFinalCACLK <0)
    {    //Set CLK delay
#ifdef DONT_MOVE_CLK_DELAY
        CATrain_ClkDelay[p->channel][p->rank] = 0;
        mcSHOW_ERR_MSG(("Warning!! Clk Dly = %d, adjust to 0 !!\n",-iFinalCACLK));
#else
        CATrain_ClkDelay[p->channel][p->rank] = -iFinalCACLK;
#endif
        CATrain_CmdDelay[p->channel][p->rank] = 0;
    }
    else
    {    // Set CA output delay
        CATrain_ClkDelay[p->channel][p->rank] = 0;
        CATrain_CmdDelay[p->channel][p->rank] = iFinalCACLK;
    }
		}  
    CBTSetCACLKResult(p);
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
 if(p->pSavetimeData->femmc_Ready==0 || ((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x40)>>6)==1)
 {
	p->pSavetimeData->u1CBTVref_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]]= uiFinalVref;
	p->pSavetimeData->u1CBTClkDelay_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]]= CATrain_ClkDelay[p->channel][p->rank];
	p->pSavetimeData->u1CBTCmdDelay_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]]= CATrain_CmdDelay[p->channel][p->rank];
 }
#endif

    mcSHOW_DBG_MSG(("\nAverage CA Dly: %d\n", iFinalCACLK));
    mcFPRINTF((fp_A60501, "\nAverage CA Dly: %d\n", iFinalCACLK));

#if 0//VENDER_JV_LOG
        mcSHOW_DBG_MSG5(("\nAverage CA Dly %d\n", iFinalCACLK));
#endif
    //wait tVREF_LONG
    mcDELAY_US(1);

//-------------  CS and CLK ----------
    CBTAdjustCS(p);

//-------  Going to exit Command bus training(CBT) mode.-------------
#if (FOR_DV_SIMULATION_USED==0)
        //switch to low freq
        if (operating_fsp == FSP_1)
        {
            CBT_Switch_Freq(p, CBT_LOW_FREQ);
        }
#endif

    CBTExit(p, operating_fsp, operation_frequency);


    if (p->dram_cbt_mode[p->rank] == CBT_NORMAL_MODE)
    {  
        CBTSetFSP(p, operating_fsp, 1);
        CBTSetVref(p, uiFinalVref, operating_fsp, 1);    //francis, normal mode go MR12 set vref again, set final_set_flag to force to MR12 flow
    }


#if (FOR_DV_SIMULATION_USED==0)
        //switch to high freq
        if (operating_fsp == FSP_1)
        {
            CBT_Switch_Freq(p, CBT_HIGH_FREQ);
        }
#endif


#if (SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_CBT && CA_PER_BIT_DELAY_CELL && BYPASS_CA_PER_BIT_DELAY_CELL)
    if(p->pSavetimeData->femmc_Ready==1 && (p->frequency >= 1600))
    {
        iCA_PerBit_DelayLine[0] = p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][0];
        iCA_PerBit_DelayLine[1] = p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][1];
        iCA_PerBit_DelayLine[2] = p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][2];
        iCA_PerBit_DelayLine[3] = p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][3];
        iCA_PerBit_DelayLine[4] = p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][4];
        iCA_PerBit_DelayLine[5] = p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][5];        

        // Set CA perbit delay line calibration results
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD0),  P_Fld(iCA_PerBit_DelayLine[0], SHU1_R0_CA_CMD0_RK0_TX_ARCA0_DLY) | \
                                                                    P_Fld(iCA_PerBit_DelayLine[1], SHU1_R0_CA_CMD0_RK0_TX_ARCA1_DLY) | \
                                                                    P_Fld(iCA_PerBit_DelayLine[2], SHU1_R0_CA_CMD0_RK0_TX_ARCA2_DLY) | \
                                                                    P_Fld(iCA_PerBit_DelayLine[3], SHU1_R0_CA_CMD0_RK0_TX_ARCA3_DLY) | \
                                                                    P_Fld(iCA_PerBit_DelayLine[4], SHU1_R0_CA_CMD0_RK0_TX_ARCA4_DLY) | \
                                                                    P_Fld(iCA_PerBit_DelayLine[5], SHU1_R0_CA_CMD0_RK0_TX_ARCA5_DLY) );        
    }
    else
#endif
#if CA_PER_BIT_DELAY_CELL
    if (p->frequency >= 1600)
    {
        if(p->u2DelayCellTimex100!=0)
        {
            u1EnableDelayCell =1;
            mcSHOW_DBG_MSG(("[CBT_PER_BIT_DELAY_CELL] DelayCellTimex100 =%d/100 ps\n", p->u2DelayCellTimex100));
        }
        else
        {
            u1EnableDelayCell = 0;
        }
       
        if(u1EnableDelayCell==1)
        {
            for (uiCA=0; uiCA<CATRAINING_NUM_LP4; uiCA++) 
            {
                iCA_PerBit_DelayLine[uiCA] = iCAFinalCenter[uiCA] - iCK_MIN;
                if (iCA_PerBit_DelayLine[uiCA] > 15) iCA_PerBit_DelayLine[uiCA] = 15;
                u1DelayCellOfst[uiCA] = (iCA_PerBit_DelayLine[uiCA]*100000000/(p->frequency*64))/p->u2DelayCellTimex100;
                mcSHOW_DBG_MSG(("CA%d perbit dly line = %d cells (%d PI)\n", uiCA, u1DelayCellOfst[uiCA], iCA_PerBit_DelayLine[uiCA]));
            } 

            // Set CA perbit delay line calibration results
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD0),  P_Fld(u1DelayCellOfst[0], SHU1_R0_CA_CMD0_RK0_TX_ARCA0_DLY) | \
                                                                        P_Fld(u1DelayCellOfst[1], SHU1_R0_CA_CMD0_RK0_TX_ARCA1_DLY) | \
                                                                        P_Fld(u1DelayCellOfst[2], SHU1_R0_CA_CMD0_RK0_TX_ARCA2_DLY) | \
                                                                        P_Fld(u1DelayCellOfst[3], SHU1_R0_CA_CMD0_RK0_TX_ARCA3_DLY) | \
                                                                        P_Fld(u1DelayCellOfst[4], SHU1_R0_CA_CMD0_RK0_TX_ARCA4_DLY) | \
                                                                        P_Fld(u1DelayCellOfst[5], SHU1_R0_CA_CMD0_RK0_TX_ARCA5_DLY) );
        }
    }
#endif

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION && CA_PER_BIT_DELAY_CELL
    if((p->pSavetimeData->femmc_Ready==0 || ((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x40)>>6)==1) && (p->frequency >= 1600))
    {
        p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][0] = iCA_PerBit_DelayLine[0];
        p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][1] = iCA_PerBit_DelayLine[1];
        p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][2] = iCA_PerBit_DelayLine[2];
        p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][3] = iCA_PerBit_DelayLine[3];
        p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][4] = iCA_PerBit_DelayLine[4];
        p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][5] = iCA_PerBit_DelayLine[5];        
    }
#endif

    mcSHOW_DBG_MSG3(("\n[CmdBusTrainingLP4] Done\n"));
    mcFPRINTF((fp_A60501, "\n[CmdBusTrainingLP4] Done\n"));

    //restore MRR pin mux
    Set_MRR_Pinmux_Mapping(p);

#ifdef DRAM_CALIB_LOG
    gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].CmdBusTraining.VrefCA = uiFinalVref;
    gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].CmdBusTraining.CA_delay = CATrain_CmdDelay[p->channel][p->rank];
    gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].CmdBusTraining.Clk_delay = CATrain_ClkDelay[p->channel][p->rank];
    gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].CmdBusTraining.CS_delay = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), SHU1_R0_CA_CMD9_RG_RK0_ARPI_CS);;
#endif

    if (p->rank == RANK_1)
    {
#if (CBT_K_RANK1_USE_METHOD==1)
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), 0, MRS_MRSRK);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), 0, RKCFG_TXRANK); //use other rank's setting 
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), 0, RKCFG_TXRANKFIX); //TXRANKFIX should be write after TXRANK or the rank will be fix at rank 1         
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MPC_OPTION), 0x1, MPC_OPTION_MPCRKEN);
#else
        DramcRankSwap(p, RANK_0);
#endif
    }

    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0), uiReg04ch); //restore auto refresh register

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

typedef struct _REG_TRANSFER
{
    U32 u4Addr;
    U32 u4Fld;
} REG_TRANSFER_T;


// NOT suitable for Gating delay
static DRAM_STATUS_T ExecuteMoveDramCDelay(DRAMC_CTX_T *p, REG_TRANSFER_T regs[], S8 iShiftUI)
{
    S32 s4HighLevelDelay, s4DelaySum;
    U32 u4Tmp0p5T, u4Tmp2T;
    U8 ucDataRateDivShift = 0;
    DRAM_STATUS_T MoveResult;

    if (u1IsLP4Family(p->dram_type))
    {
        ucDataRateDivShift = 3;
    }
#if ENABLE_LP3_SW
    else
    {
        ucDataRateDivShift = 2;
    }
#endif /* ENABLE_LP3_SW */

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
            // DQM_OEN_0
            TransferReg[0].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ3;
            TransferReg[0].u4Fld =SHURK0_SELPH_DQ3_DLY_OEN_DQM0;
            TransferReg[1].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ1;
            TransferReg[1].u4Fld =SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0;
            ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
            // DQ0
            TransferReg[0].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ2;
            TransferReg[0].u4Fld =SHURK0_SELPH_DQ2_DLY_DQ0;
            TransferReg[1].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ0;
            TransferReg[1].u4Fld =SHURK0_SELPH_DQ0_TXDLY_DQ0;
            ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
            // DQ_OEN_0
            TransferReg[0].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ2;
            TransferReg[0].u4Fld =SHURK0_SELPH_DQ2_DLY_OEN_DQ0;
            TransferReg[1].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ0;
            TransferReg[1].u4Fld =SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0;
            ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
        break;

        case 1:
            // DQM1
            TransferReg[0].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ3;
            TransferReg[0].u4Fld =SHURK0_SELPH_DQ3_DLY_DQM1;
            TransferReg[1].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ1;
            TransferReg[1].u4Fld =SHURK0_SELPH_DQ1_TXDLY_DQM1;
            ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
            // DQM_OEN_1
            TransferReg[0].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ3;
            TransferReg[0].u4Fld =SHURK0_SELPH_DQ3_DLY_OEN_DQM1;
            TransferReg[1].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ1;
            TransferReg[1].u4Fld =SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1;
            ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
            // DQ1
            TransferReg[0].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ2;
            TransferReg[0].u4Fld =SHURK0_SELPH_DQ2_DLY_DQ1;
            TransferReg[1].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ0;
            TransferReg[1].u4Fld =SHURK0_SELPH_DQ0_TXDLY_DQ1;
            ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
             // DQ_OEN_1
            TransferReg[0].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ2;
            TransferReg[0].u4Fld =SHURK0_SELPH_DQ2_DLY_OEN_DQ1;
            TransferReg[1].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ0;
            TransferReg[1].u4Fld =SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1;
             ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
        break;

        case 2:
            // DQM2
            TransferReg[0].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ3;
            TransferReg[0].u4Fld =SHURK0_SELPH_DQ3_DLY_DQM2;
            TransferReg[1].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ1;
            TransferReg[1].u4Fld =SHURK0_SELPH_DQ1_TXDLY_DQM2;
            ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
            // DQM_OEN_2
            TransferReg[0].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ3;
            TransferReg[0].u4Fld =SHURK0_SELPH_DQ3_DLY_OEN_DQM2;
            TransferReg[1].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ1;
            TransferReg[1].u4Fld =SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2;
            ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
            // DQ2
            TransferReg[0].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ2;
            TransferReg[0].u4Fld =SHURK0_SELPH_DQ2_DLY_DQ2;
            TransferReg[1].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ0;
            TransferReg[1].u4Fld =SHURK0_SELPH_DQ0_TXDLY_DQ2;
            ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
            // DQ_OEN_2
            TransferReg[0].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ2;
            TransferReg[0].u4Fld =SHURK0_SELPH_DQ2_DLY_OEN_DQ2;
            TransferReg[1].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ0;
            TransferReg[1].u4Fld =SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2;
            ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
        break;

        case 3:
            // DQM3
            TransferReg[0].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ3;
            TransferReg[0].u4Fld =SHURK0_SELPH_DQ3_DLY_DQM3;
            TransferReg[1].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ1;
            TransferReg[1].u4Fld =SHURK0_SELPH_DQ1_TXDLY_DQM3;
            ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
            // DQM_OEN_3
            TransferReg[0].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ3;
            TransferReg[0].u4Fld =SHURK0_SELPH_DQ3_DLY_OEN_DQM3;
            TransferReg[1].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ1;
            TransferReg[1].u4Fld =SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3;
            ExecuteMoveDramCDelay(p, TransferReg, iShiftUI);
            // DQ3
            TransferReg[0].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ2;
            TransferReg[0].u4Fld =SHURK0_SELPH_DQ2_DLY_DQ3;
            TransferReg[1].u4Addr = DRAMC_REG_SHURK0_SELPH_DQ0;
            TransferReg[1].u4Fld =SHURK0_SELPH_DQ0_TXDLY_DQ3;
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


#if ENABLE_LP3_SW
//for LPDDR3 DQ delay line used
static void Set_RX_DQ_DelayLine_Phy_Byte(DRAMC_CTX_T *p, U8 u1ByteIdx, S8 value[8])
{
    DRAM_CHANNEL_T backup_channel;

    backup_channel = p->channel;

    switch(u1ByteIdx)
    {
        case 0:
            p->channel = CHANNEL_A;
            //DQ0 -> ARDQ0_B1   //DQ1 -> ARDQ1_B1
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ2), P_Fld(value[0], SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_F_DLY_B1) | P_Fld(value[0], SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_R_DLY_B1) |
                                                                      P_Fld(value[1], SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_F_DLY_B1) | P_Fld(value[1], SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_R_DLY_B1) );
            //DQ3 -> ARDQ3_B1
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ3), P_Fld(value[3], SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_F_DLY_B1) | P_Fld(value[3], SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_R_DLY_B1) );

            //DQ4 -> ARDQ4_B1   //DQ5 -> ARDQ5_B1
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ4), P_Fld(value[4], SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_F_DLY_B1) | P_Fld(value[4], SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_R_DLY_B1) |
                                                                      P_Fld(value[5], SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_F_DLY_B1) | P_Fld(value[5], SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_R_DLY_B1) );
            //DQ6 -> ARDQ6_B1   //DQ7 -> ARDQ7_B1
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ5), P_Fld(value[6], SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_F_DLY_B1) | P_Fld(value[6], SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_R_DLY_B1) |
                                                                      P_Fld(value[7], SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_F_DLY_B1) | P_Fld(value[7], SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_R_DLY_B1) );
            //DQ2 -> ARDQM0_B1
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ6), P_Fld(value[2], SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_F_DLY_B1) | P_Fld(value[2], SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1) );
            break;
        case 1:
            p->channel = CHANNEL_B;
            //DQ8 -> BRDQ0_B0   //DQ9 -> BRDQ1_B0
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ2), P_Fld(value[0], SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_F_DLY_B0) | P_Fld(value[0], SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_R_DLY_B0) |
                                                                      P_Fld(value[1], SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_F_DLY_B0) | P_Fld(value[1], SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_R_DLY_B0) );
            //DQ10 -> BRDQ4_B0  //DQ11 -> BRDQ5_B0
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ3), P_Fld(value[2], SHU1_R0_B0_DQ3_RK0_RX_ARDQ2_F_DLY_B0) | P_Fld(value[2], SHU1_R0_B0_DQ3_RK0_RX_ARDQ2_R_DLY_B0) |
                                                                      P_Fld(value[3], SHU1_R0_B0_DQ3_RK0_RX_ARDQ3_F_DLY_B0) | P_Fld(value[3], SHU1_R0_B0_DQ3_RK0_RX_ARDQ3_R_DLY_B0) );
            //DQ12 -> BRDQ3_B0
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ4), P_Fld(value[4], SHU1_R0_B0_DQ4_RK0_RX_ARDQ4_F_DLY_B0) | P_Fld(value[4], SHU1_R0_B0_DQ4_RK0_RX_ARDQ4_R_DLY_B0) );

            //DQ14 -> BRDQ4_B0  //DQ15 -> BRDQ5_B0
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ5), P_Fld(value[6], SHU1_R0_B0_DQ5_RK0_RX_ARDQ6_F_DLY_B0) | P_Fld(value[6], SHU1_R0_B0_DQ5_RK0_RX_ARDQ6_R_DLY_B0) |
                                                                      P_Fld(value[7], SHU1_R0_B0_DQ5_RK0_RX_ARDQ7_F_DLY_B0) | P_Fld(value[7], SHU1_R0_B0_DQ5_RK0_RX_ARDQ7_R_DLY_B0) );
            //DQ13 -> BRDQM0_B0
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ6), P_Fld(value[5], SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_F_DLY_B0) | P_Fld(value[5], SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0) );
            break;
       case 2:
            p->channel = CHANNEL_B;
            //DQ17 -> BRCA1
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD2), P_Fld(value[4], SHU1_R0_CA_CMD2_RG_RK0_RX_ARCA1_F_DLY) | P_Fld(value[4], SHU1_R0_CA_CMD2_RG_RK0_RX_ARCA1_R_DLY) );

            //DQ18 -> BRCA2     //DQ19 -> BRCA3
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD3), P_Fld(value[0], SHU1_R0_CA_CMD3_RG_RK0_RX_ARCA2_F_DLY) | P_Fld(value[0], SHU1_R0_CA_CMD3_RG_RK0_RX_ARCA2_R_DLY) |
                                                                       P_Fld(value[1], SHU1_R0_CA_CMD3_RG_RK0_RX_ARCA3_F_DLY) | P_Fld(value[1], SHU1_R0_CA_CMD3_RG_RK0_RX_ARCA3_R_DLY) );
            //DQ20 -> BRCA4
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD4), P_Fld(value[4], SHU1_R0_CA_CMD4_RG_RK0_RX_ARCA4_F_DLY) | P_Fld(value[4], SHU1_R0_CA_CMD4_RG_RK0_RX_ARCA4_R_DLY) );

            //DQ22 -> BRCKE0    //DQ23 -> BRCKE1
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD5), P_Fld(value[0], SHU1_R0_CA_CMD5_RG_RK0_RX_ARCKE0_F_DLY) | P_Fld(value[0], SHU1_R0_CA_CMD5_RG_RK0_RX_ARCKE0_R_DLY) |
                                                                       P_Fld(value[1], SHU1_R0_CA_CMD5_RG_RK0_RX_ARCKE1_F_DLY) | P_Fld(value[1], SHU1_R0_CA_CMD5_RG_RK0_RX_ARCKE1_R_DLY) );
            //DQ16 -> BRCS0
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD6), P_Fld(value[4], SHU1_R0_CA_CMD6_RG_RK0_RX_ARCS0_F_DLY) | P_Fld(value[4], SHU1_R0_CA_CMD6_RG_RK0_RX_ARCS0_R_DLY) );

            //DQ21 -> BRCS1
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD7), P_Fld(value[4], SHU1_R0_CA_CMD7_RG_RK0_RX_ARCS1_F_DLY) | P_Fld(value[4], SHU1_R0_CA_CMD7_RG_RK0_RX_ARCS1_R_DLY) );

            break;
       case 3:
           p->channel = CHANNEL_B;
           //DQ24 -> BRDQ0_B1   //DQ25 -> BRDQ1_B1
           vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ2), P_Fld(value[0], SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_F_DLY_B1) | P_Fld(value[0], SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_R_DLY_B1) |
                                                                     P_Fld(value[1], SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_F_DLY_B1) | P_Fld(value[1], SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_R_DLY_B1) );
           //DQ26 -> BRDQ2_B1
           vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ3), P_Fld(value[3], SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_F_DLY_B1) | P_Fld(value[3], SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_R_DLY_B1) );

           //DQ28 -> BRDQ4_B1   //DQ29 -> BRDQ5_B1
           vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ4), P_Fld(value[4], SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_F_DLY_B1) | P_Fld(value[4], SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_R_DLY_B1) |
                                                                     P_Fld(value[5], SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_F_DLY_B1) | P_Fld(value[5], SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_R_DLY_B1) );
           //DQ30 -> BRDQ6_B1   //DQ31 -> BRDQ7_B1
           vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ5), P_Fld(value[6], SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_F_DLY_B1) | P_Fld(value[6], SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_R_DLY_B1) |
                                                                     P_Fld(value[7], SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_F_DLY_B1) | P_Fld(value[7], SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_R_DLY_B1) );
           //DQ27 -> BRDQM0_B1
           vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ6), P_Fld(value[2], SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_F_DLY_B1) | P_Fld(value[2], SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1) );
           break;

    }

    p->channel = backup_channel;
}

//for LPDDR3 DQM delay line used
static void Set_RX_DQM_DelayLine_Phy_Byte(DRAMC_CTX_T *p, U8 u1ByteIdx, S8 value)
{
    DRAM_CHANNEL_T backup_channel;

    backup_channel = p->channel;

    switch(u1ByteIdx)
    {
        case 0:
            p->channel = CHANNEL_A;
            //DQM0 -> ARDQ2_B1
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ3), P_Fld(value, SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_F_DLY_B1) | P_Fld(value, SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_R_DLY_B1) );
            break;
        case 1:
            p->channel = CHANNEL_B;
            //DQM1 -> BRDQ5_B0
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ4), P_Fld(value, SHU1_R0_B0_DQ4_RK0_RX_ARDQ5_F_DLY_B0) | P_Fld(value, SHU1_R0_B0_DQ4_RK0_RX_ARDQ5_R_DLY_B0) );
            break;
       case 2:
            p->channel = CHANNEL_B;
            //DQM2 -> BRCA0
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD2), P_Fld(value, SHU1_R0_CA_CMD2_RG_RK0_RX_ARCA0_F_DLY) | P_Fld(value, SHU1_R0_CA_CMD2_RG_RK0_RX_ARCA0_R_DLY) );
            break;
       case 3:
            p->channel = CHANNEL_B;
            //DQM3 -> BRDQ3_B1
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ3), P_Fld(value, SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_F_DLY_B1) | P_Fld(value, SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_R_DLY_B1) );
            break;

    }

    p->channel = backup_channel;
}
#endif


#if SIMULATION_WRITE_LEVELING
#if WRITE_LEVELING_MOVE_DQS_INSTEAD_OF_CLK
static void WriteLevelingMoveDQSInsteadOfCLK(DRAMC_CTX_T *p)
{
    U8 u1ByteIdx;
    U8 backup_rank, ii;

    backup_rank = u1GetRank(p);

    for(u1ByteIdx =0 ; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
    {
        MoveDramC_TX_DQS(p, u1ByteIdx, -WRITE_LEVELING_MOVD_DQS);

        #if 0//MOVE_TX_OE_LONGER
        if(u1IsLP4Family(p->dram_type))
            MoveDramC_TX_DQS_OEN(p, u1ByteIdx, -WRITE_LEVELING_MOVD_DQS-1);
        else
        #endif
            MoveDramC_TX_DQS_OEN(p, u1ByteIdx, -WRITE_LEVELING_MOVD_DQS);

        for(ii=RANK_0; ii<RANK_MAX; ii++)
        {
            vSetRank(p, ii);
            MoveDramC_TX_DQ(p, u1ByteIdx, -WRITE_LEVELING_MOVD_DQS);
        }
        vSetRank(p, backup_rank);
    }
}
#endif


//static void vSetDramMRWriteLevelingOnOff(DRAMC_CTX_T *p, U8 u1OnOff)
void vSetDramMRWriteLevelingOnOff(DRAMC_CTX_T *p, U8 u1OnOff)
{
    // MR2 OP[7] to enable/disable write leveling
    if(u1OnOff)
        u1MR02Value[p->dram_fsp] |= 0x80;  // OP[7] WR LEV =1
    else
        u1MR02Value[p->dram_fsp] &= 0x7f;  // OP[7] WR LEV =0

    DramcModeRegWrite(p, 2, u1MR02Value[p->dram_fsp]);
}

#define DQS_DUTY_MEASURE_WITH_WRITE_LEVELING 0
DRAM_STATUS_T DramcWriteLeveling(DRAMC_CTX_T *p)
{
// Note that below procedure is based on "ODT off"
    U8 *uiLPDDR_PHY_Mapping = NULL;
    U32 u4value, u4value1=0, u4dq_o1=0, u4dq_o1_tmp[DQS_NUMBER];
    U8 byte_i, ucsample_count;
    S32 ii, ClockDelayMax;
    U8 ucsample_status[DQS_NUMBER], ucdq_o1_perbyte[DQS_NUMBER], ucdq_o1_index[DQS_NUMBER];
    U32 u4prv_register_1dc, u4prv_register_044, u4prv_register_0e4, u4prv_register_13c, u4prv_register_008;
    U32 u4prv_register_04c, u4prv_register_064, u4prv_register_038, u4prv_register_0bc, u4prv_register_024;
    DRAM_RANK_T backup_rank;

    S32 wrlevel_dq_delay[DQS_NUMBER]; // 3 is channel number
    S32 wrlevel_dqs_delay[DQS_NUMBER]={0}; // 3 is channel number

    #if WRITE_LEVELING_MOVE_DQS_INSTEAD_OF_CLK
    S32 i4PIBegin, i4PIEnd;
    U8 u1PIStep;
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

    fgwrlevel_done = 0;
    backup_rank = u1GetRank(p);

    #if CBT_WORKAROUND_O1_SWAP
    if(u1IsLP4Family(p->dram_type))
    {
        CATrain_CmdDelay[p->channel][p->rank] = 0;//u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_ARPI_CMD), ARPI_CMD_DA_ARPI_CMD);
        CATrain_CsDelay[p->channel][p->rank] = 0;//u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_ARPI_CMD), ARPI_CMD_DA_ARPI_CS);
        CATrain_ClkDelay[p->channel][p->rank] = 0;//u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_ARPI_CMD), ARPI_CMD_DA_ARPI_CK);
    }
    #endif

    DramcRankSwap(p, p->rank);

    if (u1IsLP4Family(p->dram_type))
    {
        uiLPDDR_PHY_Mapping = (U8 *)uiLPDDR4_PHY_Mapping_POP[p->channel];
    }
#if ENABLE_LP3_SW
    else
    {
        uiLPDDR_PHY_Mapping = (U8 *)uiLPDDR3_PHY_Mapping_POP;
    }
#endif /* ENABLE_LP3_SW */

    // DQ mapping
    ///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    /// Note : uiLPDDR_PHY_Mapping_POP, need to take care mapping in real chip, but not in test chip.
    /// Everest : there is bit swap inside single byte. PHY & DRAM is 1-1 byte mapping, no swap.
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
    u4prv_register_04c = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0));
    u4prv_register_064 = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL));
    u4prv_register_038 = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL));
    u4prv_register_0bc = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_WRITE_LEV));

    u4prv_register_024 = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL));

    vAutoRefreshSwitch(p, DISABLE); //When doing CA training, should make sure that auto refresh is disable

    #if WRITE_LEVELING_MOVE_DQS_INSTEAD_OF_CLK
    if(p->arfgWriteLevelingInitShif[p->channel][p->rank] ==FALSE)
    {
        WriteLevelingMoveDQSInsteadOfCLK(p);
        //p->arfgWriteLevelingInitShif[p->channel][p->rank] =TRUE;
        p->arfgWriteLevelingInitShif[p->channel][RANK_0] =TRUE;
        p->arfgWriteLevelingInitShif[p->channel][RANK_1] =TRUE;
        #if TX_PERBIT_INIT_FLOW_CONTROL
        // both 2 rank use one write leveling result, TX need to udpate.
        p->fgTXPerbifInit[p->channel][RANK_0]= FALSE;
        p->fgTXPerbifInit[p->channel][RANK_1]= FALSE;
        #endif

        mcSHOW_DBG_MSG3(("WriteLevelingMoveDQSInsteadOfCLK\n"));
        mcFPRINTF((fp_A60501, "WriteLevelingMoveDQSInsteadOfCLK\n"));
    }
    #endif

 #if (SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_WRITELEVELING)
	if(p->pSavetimeData->femmc_Ready==1 && ((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x010)>>4)==0)
  		{
  		wrlevel_dqs_final_delay[p->channel][0] =p->pSavetimeData->u1WriteLeveling_bypass_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][0];
  		wrlevel_dqs_final_delay[p->channel][1] =p->pSavetimeData->u1WriteLeveling_bypass_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][1];

             ucsample_count = 0xff;		
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
    CKEFixOnOff(p, CKE_FIXON, CKE_WRITE_TO_ONE_CHANNEL);

    //ODT, DQIEN fixed at 1; FIXODT = 1 (0xd8[23]), FIXDQIEN = 1111 (0xd8[15:12])
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_WODT), 1, SHU1_WODT_WODTFIXOFF);

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
#if ENABLE_LP3_SW
    if(p->dram_type == TYPE_LPDDR3)
    {
        u4value = 0xf;//select byte 0.1.2.3
    }
    else  //LPDDR4
#endif /* ENABLE_LP3_SW */
    {
        u4value = 0x3;//select byte 0.1
    }
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WRITE_LEV), u4value, WRITE_LEV_DQS_SEL);

    // wait tWLMRD (40 nCL / 40 ns) before DQS pulse (DDR3 / LPDDR3)
    mcDELAY_US(1);

    //Proceed write leveling...
    //Initilize sw parameters
    ClockDelayMax = MAX_TX_DQSDLY_TAPS;
    for (ii=0; ii < (S32)(p->data_width/DQS_BIT_NUMBER); ii++)
    {
        ucsample_status[ii] = 0;
        wrlevel_dqs_final_delay[p->channel][ii] = 0;
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
    if(u1IsLP4Family(p->dram_type))
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), 0, SHU1_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);  //rank0, byte0, DQS delay
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ7), 0, SHU1_R0_B1_DQ7_RK0_ARPI_PBYTE_B1);  //rank0, byte1, DQS delay
    }
#if ENABLE_LP3_SW
    else
    {
        vIO32WriteFldAlign_Phy_Byte(0, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), 0, SHU1_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);
        vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), 0, SHU1_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);
        vIO32WriteFldAlign_Phy_Byte(2, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), 0, SHU1_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);
        vIO32WriteFldAlign_Phy_Byte(3, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), 0, SHU1_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);
    }
#endif
    #if WRITE_LEVELING_MOVE_DQS_INSTEAD_OF_CLK
         #if 0 //SUPPORT_SAVE_TIME_FOR_CALIBRATION
	if(p->pSavetimeData->femmc_Ready==1 && ((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x01)>>0)==0)
	{
		i4PIBegin=p->pSavetimeData->u1WriteLeveling_Save[p->channel][p->rank][p->freq_sel]-p->pSavetimeData->u1WriteLevelingoffset;
      		i4PIEnd = 64;
       }
	else
	#endif
	{
    i4PIBegin = WRITE_LEVELING_MOVD_DQS*32 -MAX_CLK_PI_DELAY-1;
    i4PIEnd = i4PIBegin + 64;
	}

    #endif

    #if DQS_DUTY_MEASURE_WITH_WRITE_LEVELING
    if(p->channel==CHANNEL_A && p->frequency == u2DFSGetHighestFreq(p))
        u1PIStep =2;
    else
    #endif
        u1PIStep =1;

    #if  WRITE_LEVELING_MOVE_DQS_INSTEAD_OF_CLK
    for (ii=i4PIBegin; ii<i4PIEnd; ii+=u1PIStep)
    #else
    for (ii=(-MAX_CLK_PI_DELAY); ii<=MAX_TX_DQSDLY_TAPS; ii++)
    #endif
    {
        #if DQS_DUTY_MEASURE_WITH_WRITE_LEVELING
        if(u1PIStep==2)
            mcDELAY_MS(10000);
        #endif
        
        if (ii <= 0)
        {
            // Adjust Clk output delay.
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), -ii, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CLK);
        }
        else
        {
            // Adjust DQS output delay.
            // PI (TX DQ/DQS adjust at the same time)
            if(u1IsLP4Family(p->dram_type))
            {
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ii, SHU1_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);  //rank0, byte0, DQS delay
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ7), ii, SHU1_R0_B1_DQ7_RK0_ARPI_PBYTE_B1);  //rank0, byte1, DQS delay
            }
#if ENABLE_LP3_SW
            else
            {
                vIO32WriteFldAlign_Phy_Byte(0, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ii, SHU1_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);
                vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ii, SHU1_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);
                vIO32WriteFldAlign_Phy_Byte(2, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ii, SHU1_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);
                vIO32WriteFldAlign_Phy_Byte(3, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ii, SHU1_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);
            }
#endif
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
        if(u1IsLP4Family(p->dram_type))
        {
            u4dq_o1 = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_DQO1));
        }
#if ENABLE_LP3_SW
        else //LPRRR3
        {
            // Get DQ value.
            u4dq_o1 = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_DQO1));
            //mcSHOW_DBG_MSG2(("DQ_O1: 0x%x\n", u4dq_o1));
        }
#endif
        #ifdef ETT_PRINT_FORMAT
        mcSHOW_DBG_MSG2(("%d    ", ii));
        #else
        mcSHOW_DBG_MSG2(("%2d    ", ii));
        #endif
        mcFPRINTF((fp_A60501, "%2d    ", ii));

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
            else if ((ucsample_status[byte_i]>=1) && (ucdq_o1_perbyte[byte_i] !=0))
            {
                ucsample_status[byte_i]++;
                        #if 0 //SUPPORT_SAVE_TIME_FOR_CALIBRATION
	                if(p->pSavetimeData->femmc_Ready==1 &&  ((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x01)>>0)==0)
	                {
                        	wrlevel_dqs_final_delay[p->channel][byte_i] = ii -ucsample_status[byte_i] +2;
                         	ucsample_count |= (0x01 << byte_i);
                        }
                        #endif
            }
            //mcSHOW_DBG_MSG(("(%x) ", ucsample_status[byte_i]));

            if((ucsample_count &(0x01 << byte_i))==0)// result not found of byte yet
            {
                #if  WRITE_LEVELING_MOVE_DQS_INSTEAD_OF_CLK
                if((ucsample_status[byte_i] ==8) || ((ii==i4PIEnd-1) && (ucsample_status[byte_i] >1)))
                #else
                if((ucsample_status[byte_i] ==8) || ((ii==MAX_TX_DQSDLY_TAPS)&& (ucsample_status[byte_i] >1)))
                #endif
                {
                    wrlevel_dqs_final_delay[p->channel][byte_i] = ii -ucsample_status[byte_i] +2;
                    ucsample_count |= (0x01 << byte_i);
                    //mcSHOW_DBG_MSG(("(record %d) ", wrlevel_dqs_final_delay[p->channel][byte_i]));
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
    if (ucsample_count == 0xff)
    {
        // all bytes are done
        fgwrlevel_done= 1;
        vSetCalibrationResult(p, DRAM_CALIBRATION_WRITE_LEVEL, DRAM_OK);
    }
    else
    {
        vSetCalibrationResult(p, DRAM_CALIBRATION_WRITE_LEVEL, DRAM_FAIL);
       #if 0 // SUPPORT_SAVE_TIME_FOR_CALIBRATION
	   if(p->pSavetimeData->femmc_Ready==1 &&  ((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x01)>>0)==0)
	   {
                   p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]]=1<<0; //WL fail
	   }
       #endif
    }

    mcSHOW_DBG_MSG2(("pass bytecount = 0x%x (0xff: all bytes pass) \n\n", ucsample_count));
    mcFPRINTF((fp_A60501, "pass bytecount = 0x%x (0xff: all bytes pass)\n\n", ucsample_count));

    for (byte_i = 0; byte_i < (p->data_width/DQS_BIT_NUMBER);  byte_i++)
    {
        if (ClockDelayMax > wrlevel_dqs_final_delay[p->channel][byte_i])
        {
            ClockDelayMax = wrlevel_dqs_final_delay[p->channel][byte_i];
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

#ifdef DRAM_CALIB_LOG
    gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].Write_Leveling.Clk_delay = ClockDelayMax;
    gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].Write_Leveling.CA_delay = CATrain_ClkDelay[p->channel][p->rank];
#endif

    // Adjust Clk & CA if needed
    if (CATrain_ClkDelay[p->channel][p->rank] < ClockDelayMax)
    {
        S32 Diff = ClockDelayMax - CATrain_ClkDelay[p->channel][p->rank];
        mcSHOW_DBG_MSG(("CA adjust %d taps\n", Diff));

        // Write shift value into CA output delay.
        if(u1IsLP4Family(p->dram_type))
        {
            u4value = CATrain_CmdDelay[p->channel][p->rank];
            u4value += Diff;
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), u4value, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CMD);
        }
#if ENABLE_LP3_SW
        else
        {
            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), SHU1_R0_CA_CMD9_RG_RK0_ARPI_CMD);
            u4value += Diff;

            //default value in init() is 0x10 , vIO32Write4B(mcSET_DDRPHY_REG_ADDR_CHC(0x0458), 0x00100000);   // Partickl
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), u4value, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CMD);

            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);
            u4value1 += Diff;
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), u4value1, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);
        }
#endif
        mcSHOW_DBG_MSG(("Update CA PI Dly Macro0 = %d, Macro1 = %d\n", u4value, u4value1));
        mcFPRINTF((fp_A60501, "Update CA PI Dly Macro0 = %d, Macro1 = %d\n", u4value, u4value1));

        // Write shift value into CS output delay.
        if(u1IsLP4Family(p->dram_type))
        {
            //u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_ARPI_CMD), ARPI_CMD_DA_ARPI_CS);
            u4value = CATrain_CsDelay[p->channel][p->rank];
            u4value += Diff;
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), u4value, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CS);
            mcSHOW_DBG_MSG(("Update CS Dly = %d\n", u4value));
            mcFPRINTF((fp_A60501, "Update CS Dly = %d\n", u4value));
        }
#if ENABLE_LP3_SW
        else
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), ClockDelayMax, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CS);
            mcSHOW_DBG_MSG(("Update CS Dly = %d\n", ClockDelayMax));
            mcFPRINTF((fp_A60501, "Update CS Dly = %d\n", ClockDelayMax));
        }
#endif
    }
    else
    {
        mcSHOW_DBG_MSG(("No need to update CA/CS dly (CLK dly smaller than CA training)\n"));
        ClockDelayMax = CATrain_ClkDelay[p->channel][p->rank];
    }

    //DramcEnterSelfRefresh(p, 1);  //enter self refresh mode when changing CLK
    // Write max center value into Clk output delay.
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), ClockDelayMax, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CLK);
    //DramcEnterSelfRefresh(p, 0);

    mcFPRINTF((fp_A60501, "Final Clk output dly = %d\n", ClockDelayMax));
    mcSHOW_DBG_MSG(("Final Clk output dly = %d\n", ClockDelayMax));
    //mcSHOW_DBG_MSG(("After adjustment...\n"));

    for (byte_i = 0; byte_i < (p->data_width/DQS_BIT_NUMBER);  byte_i++)
    {
        wrlevel_dqs_final_delay[p->channel][byte_i] += (ClockDelayMax);

        mcSHOW_DBG_MSG(("DQS%d dly: %d\n", byte_i, wrlevel_dqs_final_delay[p->channel][byte_i]));
        mcFPRINTF((fp_A60501, "DQS%d dly: %d\n", byte_i, wrlevel_dqs_final_delay[p->channel][byte_i]));
    }

	#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
		if(p->pSavetimeData->femmc_Ready==0 || ((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x01)>>0)==1)
		{	
       		p->pSavetimeData->u1WriteLeveling_bypass_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][0]=wrlevel_dqs_final_delay[p->channel][0];
       		p->pSavetimeData->u1WriteLeveling_bypass_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][1]=wrlevel_dqs_final_delay[p->channel][1];
       		//if(wrlevel_dqs_final_delay[p->channel][0]>=wrlevel_dqs_final_delay[p->channel][1])
       		//{
       		//	p->pSavetimeData->u1WriteLeveling_Save[p->channel][p->rank][p->freq_sel]=wrlevel_dqs_final_delay[p->channel][1]; 
       		//}
       		//else
       		//{
       		//	p->pSavetimeData->u1WriteLeveling_Save[p->channel][p->rank][p->freq_sel]=wrlevel_dqs_final_delay[p->channel][0]; 
       		//}
                //mcSHOW_DBG_MSG(("save writeleveling\n"));
	}
	#endif
    // write leveling done, mode settings recovery if necessary
    // recover mode registers : issue MR2[7] to disable write leveling (refer to DEFAULT MR2 value)
    vSetDramMRWriteLevelingOnOff(p, DISABLE);

    // restore registers.
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), u4prv_register_024);    //restore CKEFIXON value before MIOCKCTRLOFF restore
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0), u4prv_register_04c);
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), u4prv_register_064);
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), u4prv_register_038);
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_WRITE_LEV), u4prv_register_0bc);

    //Disable DQ_O1, SELO1ASO=0 for power saving
    O1PathOnOff(p, 0);

    if(u1IsLP4Family(p->dram_type))
    {
        #if REG_SHUFFLE_REG_CHECK
        ShuffleRegCheck =1;
        #endif

        // set to best values for  DQS
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), wrlevel_dqs_final_delay[p->channel][0], SHU1_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ7), wrlevel_dqs_final_delay[p->channel][1], SHU1_R0_B1_DQ7_RK0_ARPI_PBYTE_B1);

        for(byte_i = 0; byte_i < (p->data_width/DQS_BIT_NUMBER);  byte_i++)
        {
            wrlevel_dq_delay[byte_i] = wrlevel_dqs_final_delay [p->channel][byte_i] + 0x10;
            if(wrlevel_dq_delay[byte_i] >= 0x40) //ARPI_DQ_B* is 6 bits, max 0x40
            {
                wrlevel_dq_delay[byte_i] -= 0x40;
                MoveDramC_TX_DQ(p, byte_i, 2);
            }
        }

        // set to best values for  DQM, DQ
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7),
                                        P_Fld(wrlevel_dq_delay[0], SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0) |
                                        P_Fld(wrlevel_dq_delay[0], SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0));

        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ7),
                                        P_Fld(wrlevel_dq_delay[1], SHU1_R0_B1_DQ7_RK0_ARPI_DQM_B1) |
                                        P_Fld(wrlevel_dq_delay[1], SHU1_R0_B1_DQ7_RK0_ARPI_DQ_B1));

        #if REG_SHUFFLE_REG_CHECK
        ShuffleRegCheck =0;
        #endif
    }
#if ENABLE_LP3_SW
    else //LPDDR3
    {
        #if REG_SHUFFLE_REG_CHECK
        ShuffleRegCheck =1;
        #endif

        for(byte_i=0; byte_i<(p->data_width/DQS_BIT_NUMBER); byte_i++)
        {
            if(wrlevel_dqs_final_delay [p->channel][byte_i] >= 0x40) //ARPI_PBYTE_B* is 6 bits, max 0x40
            {
                wrlevel_dqs_delay[byte_i] = wrlevel_dqs_final_delay [p->channel][byte_i] - 0x40;
                MoveDramC_TX_DQS(p, byte_i, 2);
                MoveDramC_TX_DQS_OEN(p, byte_i, 2);
            }
            else
                wrlevel_dqs_delay[byte_i] = wrlevel_dqs_final_delay [p->channel][byte_i];
        }

#if 1
        /* p->rank = RANK_0, save to Reg Rank0 and Rank1, p->rank = RANK_1, save to Reg Rank1 */
        for(ii=p->rank; ii<RANK_MAX; ii++)
        {
            vSetRank(p,ii);

            // set to best values for  DQS
            vIO32WriteFldAlign_Phy_Byte(0, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), wrlevel_dqs_delay[0], SHU1_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);
            vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), wrlevel_dqs_delay[1], SHU1_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);
            vIO32WriteFldAlign_Phy_Byte(2, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), wrlevel_dqs_delay[2], SHU1_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);
            vIO32WriteFldAlign_Phy_Byte(3, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), wrlevel_dqs_delay[3], SHU1_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);
        }
        vSetRank(p,backup_rank);
#else
        // set to best values for  DQS
        vIO32WriteFldAlign_Phy_Byte(0, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), wrlevel_dqs_delay[0], SHU1_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);
        vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), wrlevel_dqs_delay[1], SHU1_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);
        vIO32WriteFldAlign_Phy_Byte(2, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), wrlevel_dqs_delay[2], SHU1_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);
        vIO32WriteFldAlign_Phy_Byte(3, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), wrlevel_dqs_delay[3], SHU1_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);
#endif


        #if 1//EVEREST_CHANGE_OF_PHY_PBYTE
        //Evereest new change, ARPI_DQ_RK0_ARPI_PBYTE_B* only move DQS, not including of DQM&DQ anymore.
        //Add move DQ, DQ= DQS+0x10, after cali.  take care diff. UI. with DQS
        for(byte_i=0; byte_i<(p->data_width/DQS_BIT_NUMBER); byte_i++)
        {
            wrlevel_dq_delay[byte_i] = wrlevel_dqs_final_delay [p->channel][byte_i] + 0x10;
            if(wrlevel_dq_delay[byte_i] >= 0x40) //ARPI_DQ_B* is 6 bits, max 0x40
            {
                wrlevel_dq_delay[byte_i] -= 0x40;
                MoveDramC_TX_DQ(p, byte_i, 2);
            }
        }

#if 1
        /* p->rank = RANK_0, save to Reg Rank0 and Rank1, p->rank = RANK_1, save to Reg Rank1 */
        for(ii=p->rank; ii<RANK_MAX; ii++)
        {
            vSetRank(p,ii);

            // set to best values for  DQ/DQM/DQ_OEN/DQM_OEN
            vIO32WriteFldAlign_Phy_Byte(0, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), wrlevel_dq_delay[0], SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);
            vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), wrlevel_dq_delay[1], SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);
            vIO32WriteFldAlign_Phy_Byte(2, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), wrlevel_dq_delay[2], SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);
            vIO32WriteFldAlign_Phy_Byte(3, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), wrlevel_dq_delay[3], SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);

            vIO32WriteFldAlign_Phy_Byte(0, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), wrlevel_dq_delay[0], SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0);
#if (FOR_DV_SIMULATION_USED!=0)
            vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), wrlevel_dq_delay[1], SHU1_R0_CA_CMD9_RG_RK0_ARPI_CS);
            vIO32WriteFldAlign_Phy_Byte(2, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), wrlevel_dq_delay[2], SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0);
#else
            vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), wrlevel_dq_delay[1], SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0);
            vIO32WriteFldAlign_Phy_Byte(2, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), wrlevel_dq_delay[2], SHU1_R0_CA_CMD9_RG_RK0_ARPI_CS);
#endif
            vIO32WriteFldAlign_Phy_Byte(3, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), wrlevel_dq_delay[3], SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0);
        }
        vSetRank(p,backup_rank);
#else
        // set to best values for  DQ/DQM/DQ_OEN/DQM_OEN
        vIO32WriteFldAlign_Phy_Byte(0, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), wrlevel_dq_delay[0], SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);
        vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), wrlevel_dq_delay[1], SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);
        vIO32WriteFldAlign_Phy_Byte(2, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), wrlevel_dq_delay[2], SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);
        vIO32WriteFldAlign_Phy_Byte(3, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), wrlevel_dq_delay[3], SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);


        vIO32WriteFldAlign_Phy_Byte(0, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), wrlevel_dq_delay[0], SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0);
#if (FOR_DV_SIMULATION_USED!=0)
        vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), wrlevel_dq_delay[1], SHU1_R0_CA_CMD9_RG_RK0_ARPI_CS);
        vIO32WriteFldAlign_Phy_Byte(2, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), wrlevel_dq_delay[2], SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0);

#else
        vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), wrlevel_dq_delay[1], SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0);
        vIO32WriteFldAlign_Phy_Byte(2, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), wrlevel_dq_delay[2], SHU1_R0_CA_CMD9_RG_RK0_ARPI_CS);
#endif
        vIO32WriteFldAlign_Phy_Byte(3, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), wrlevel_dq_delay[3], SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0);
#endif

        #endif

        #if REG_SHUFFLE_REG_CHECK
        ShuffleRegCheck =0;
        #endif
    }
#endif

    DramcRankSwap(p, RANK_0);

#if REG_ACCESS_PORTING_DGB
    RegLogEnable =0;
#endif
    mcSHOW_DBG_MSG3(("[DramcWriteLeveling] Done\n\n"));
    mcFPRINTF((fp_A60501, "[DramcWriteLeveling] Done\n\n"));

#ifdef DRAM_CALIB_LOG
    gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].Write_Leveling.Final_Clk_delay =  ClockDelayMax;
    gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].Write_Leveling.DQS0_delay = wrlevel_dqs_final_delay[p->channel][0];
    gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].Write_Leveling.DQS1_delay = wrlevel_dqs_final_delay[p->channel][1];
    gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].Write_Leveling.DQS2_delay = wrlevel_dqs_final_delay[p->channel][2];
    gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].Write_Leveling.DQS3_delay = wrlevel_dqs_final_delay[p->channel][3];
#endif

    return DRAM_OK;
}
#endif //SIMULATION_WRITE_LEVELING


#if SIMULATION_GATING
//LP3 RODT is not enable, don't need to set the RODT settings.
//LP4 RODT range is very large(5ns),  no need to adjust with gating position
//0x860	SHU_ODTCTRL	32	ODT CONTROL REGISTER
//                         31	31	RODTE	RW	PUBLIC	1'b1
// 	 	 	 	30	30	RODTE2	RW	PUBLIC	1'b1	
// 	 	 	 	7	4	RODT	RW	PUBLIC	4'bx= DQSINCTL or -1	
#define GATING_RODT_LATANCY_EN 1  //Need to enable when RODT enable

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

// Use gating old burst mode to find gating window boundary
// Set the begining of window as new burst mode gating window center.
#define LP4_GATING_OLD_BURST_MODE 1
#define LP4_GATING_LEAD_LAG_FLAG_JUDGE LP4_GATING_OLD_BURST_MODE

/* 
 * u1Mode decides old or new length modes (7UI, 8UI) should be used
 * 0: OLD 8UI mode (not extend 2T RD preamble)
 * 1: NEW 7UI mode (extend 2T RD preamble) (DQS_GW_7UI defined)
 *    NEW 8UI mode (extend 2T RD preamble) (DQS_GW_7UI not defined)
 */
void DramcGatingMode(DRAMC_CTX_T *p, U8 u1Mode)
{
    // mode 0:  old burst mode
    // mode 1:  7UI or 8UI gating window length mode (depends on if DQS_GW_7UI is defined)
#if (fcFOR_CHIP_ID == fcBianco)
    /* There are currently 2 ways to set GatingMode (sets different registers)
     * 1. Alaska
     * 2. Bianco, Whitney, Kibo+(Olympus)
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

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ9), 1, B0_DQ9_RG_RX_ARDQS_DQSIENMODE_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ9), 1, B1_DQ9_RG_RX_ARDQS_DQSIENMODE_B1);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_STBCAL1), u1BurstE2, STBCAL1_DQSIEN_7UI_EN);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), 1, STBCAL_DQSIENMODE_SELPH);

    /* Perform reset (makes sure PHY's behavior works as the above setting) */
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ9), P_Fld(0, B0_DQ9_RG_RX_ARDQS_STBEN_RESETB_B0) |P_Fld(0, B0_DQ9_RG_RX_ARDQ_STBEN_RESETB_B0));
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ9), P_Fld(0, B1_DQ9_RG_RX_ARDQS_STBEN_RESETB_B1) |P_Fld(0, B1_DQ9_RG_RX_ARDQ_STBEN_RESETB_B1));
    mcDELAY_US(1);//delay 10ns
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ9), P_Fld(1, B1_DQ9_RG_RX_ARDQS_STBEN_RESETB_B1) |P_Fld(1, B1_DQ9_RG_RX_ARDQ_STBEN_RESETB_B1));
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ9), P_Fld(1, B0_DQ9_RG_RX_ARDQS_STBEN_RESETB_B0) |P_Fld(1, B0_DQ9_RG_RX_ARDQ_STBEN_RESETB_B0));
    //DramPhyReset(p);
}

DRAM_STATUS_T DramcRxdqsGatingCal(DRAMC_CTX_T *p)
{
        #if !SW_CHANGE_FOR_SIMULATION
        U8 ucRX_DLY_DQSIENSTB_LOOP,ucRX_DQS_CTL_LOOP;
        #endif
        U32 u4value, u4all_result_R=0, u4all_result_F=0, u4err_value;
        U8 ucpass_begin[DQS_NUMBER] = {0}, ucpass_count[DQS_NUMBER] = {0}, ucCurrentPass;
        U8 ucmin_coarse_tune2T[DQS_NUMBER] = {0}, ucmin_coarse_tune0p5T[DQS_NUMBER] = {0}, ucmin_fine_tune[DQS_NUMBER] = {0};
        U8 ucpass_count_1[DQS_NUMBER] = {0}, ucmin_coarse_tune2T_1[DQS_NUMBER] = {0}, ucmin_coarse_tune0p5T_1[DQS_NUMBER] = {0}, ucmin_fine_tune_1[DQS_NUMBER] = {0};
        U8 dqs_i,  ucdly_coarse_large, ucdly_coarse_0p5T, ucdly_fine_xT, ucDQS_GW_FINE_STEP;
        U8 ucdqs_result_R, ucdqs_result_F, uctmp_offset, uctmp_value;
        U8 ucbest_fine_tune[DQS_NUMBER] = {0}, ucbest_coarse_tune0p5T[DQS_NUMBER] = {0}, ucbest_coarse_tune2T[DQS_NUMBER] = {0};
        U8 ucbest_fine_tune_P1[DQS_NUMBER] = {0}, ucbest_coarse_tune0p5T_P1[DQS_NUMBER] = {0}, ucbest_coarse_tune2T_P1[DQS_NUMBER] = {0};

        U8 ucFreqDiv;
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

        U32 u4BakReg_DRAMC_DQSCAL0, u4BakReg_DRAMC_STBCAL_F;
        U32 u4BakReg_DRAMC_WODT, u4BakReg_DRAMC_SPCMD, u4BakReg_DRAMC_REFCTRL0;
        #if LP4_GATING_OLD_BURST_MODE
        U32 u4BakReg_DDRPHY_B0_DQ6, u4BakReg_DDRPHY_B1_DQ6;
        #else
        U32 u4BakReg_DDRPHY_B0_DQ7, u4BakReg_DDRPHY_B1_DQ7;
        #endif

        #if LP4_GATING_LEAD_LAG_FLAG_JUDGE
        U8 u1DQS_lead[DQS_NUMBER] = {0}, u1DQS_lag[DQS_NUMBER] = {0}, u1DQS_high[DQS_NUMBER] = {0}, u1DQS_transition[DQS_NUMBER] = {0};
        U8 ucdly_coarse_large_leadLag[DQS_NUMBER] = {0}, ucdly_coarse_0p5T_leadLag[DQS_NUMBER] = {0}, ucdly_fine_tune_leadLag[DQS_NUMBER] = {0};
        #endif

        U8 u1PassByteCount=0;

#if ENABLE_CLAIBTAION_WINDOW_LOG_FOR_FT
        U16 u2MinWinSize = 0xffff;
        U8 u1MinWinSizeByteidx = 0xff;
#endif
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

        //Register backup
        u4BakReg_DRAMC_DQSCAL0 = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_STBCAL));
        u4BakReg_DRAMC_STBCAL_F = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_STBCAL1));
        u4BakReg_DRAMC_WODT = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DDRCONF0));
        u4BakReg_DRAMC_SPCMD = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SPCMD));
        u4BakReg_DRAMC_REFCTRL0 = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0));
        #if LP4_GATING_OLD_BURST_MODE
        u4BakReg_DDRPHY_B0_DQ6 = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_B0_DQ6));
        u4BakReg_DDRPHY_B1_DQ6 = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_B1_DQ6));
        #else
        u4BakReg_DDRPHY_B0_DQ7 = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_B0_DQ7));
        u4BakReg_DDRPHY_B1_DQ7 = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_B1_DQ7));
        #endif

        //Justin: DQ_REV_B*[5] =1, select RX gating mode to prevent 0.5T fake gating window behind real window.
        if(u1IsLP4Family(p->dram_type))
        {
            /* LP4: Disable(set to 0) "RX DQS ISI pulse CG function" during gating window calibration (must set to 1 when done) */
            RxDQSIsiPulseCG(p, DISABLE);

            #if LP4_GATING_OLD_BURST_MODE
            #if !LP4_GATING_LEAD_LAG_FLAG_JUDGE
            DramcGatingMode(p, 0);
            #endif
            u1MR01Value[p->dram_fsp] |= 0x80;
            DramcModeRegWriteByRank(p, p->rank, 0x1, u1MR01Value[p->dram_fsp]);
            #else
             vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B0_DQ7), P_Fld( 1, B0_DQ7_RG_TX_ARDQS_PULL_UP_B0) | P_Fld( 1, B0_DQ7_RG_TX_ARDQSB_PULL_DN_B0));
             vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B1_DQ7), P_Fld( 1, B1_DQ7_RG_TX_ARDQS0_PULL_UP_B1) | P_Fld( 1, B1_DQ7_RG_TX_ARDQS0B_PULL_DN_B1));
            #endif
        }
        #if 0 // LP3: Since it is always set to 1 (should be set during initial register settings, comment out here to save code size
        else
        {
            /* LP3: Always enable(set to 1) "RX DQS ISI pulse CG function" */
            RxDQSIsiPulseCG(p, ENABLE);
        }
        #endif
        
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

        if(u1IsLP4Family(p->dram_type))
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), u1GetRank(p), MISC_CTRL1_R_DMSTBENCMP_RK_OPT);
            DramcEngine2Init(p, 0x55000000, 0xaa000000 |GATING_PATTERN_NUM_LP4, TEST_AUDIO_PATTERN, 0);
        }
#if ENABLE_LP3_SW
        else
        {
            vIO32WriteFldAlign_All(DDRPHY_MISC_CTRL1, u1GetRank(p), MISC_CTRL1_R_DMSTBENCMP_RK_OPT);
            DramcEngine2Init(p, 0x55000000, 0xaa000000 |GATING_PATTERN_NUM_LP3, TEST_AUDIO_PATTERN, 0);
        }
#endif

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
        ucRX_DQS_CTL_LOOP = 8; // Since Everest, no matter LP3 or LP4. ucRX_DQS_CTL_LOOP is 8.
        #endif

        mcSHOW_DBG_MSG2(("[Gating]\n"));
        vPrintCalibrationBasicInfo(p);

        if(u1IsLP4Family(p->dram_type))
        {
            ucFreqDiv= 4;
            ucDQS_GW_FINE_STEP = DQS_GW_FINE_STEP;

        	#if 0 //SUPPORT_SAVE_TIME_FOR_CALIBRATION
            if(p->pSavetimeData->femmc_Ready==1 && ((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x02)>>1)==0)
        	{
                for (dqs_i=0; dqs_i<2; dqs_i++)
                {
                    if(ucCoarseStart>p->pSavetimeData->u1Gating2T_Save[p->channel][p->rank][p->freq_sel][dqs_i])//found smallest 2T value
                    {
                        ucCoarseStart =p->pSavetimeData->u1Gating2T_Save[p->channel][p->rank][p->freq_sel][dqs_i];
                        ucdly_coarse_0p5T=p->pSavetimeData->u1Gating05T_Save[p->channel][p->rank][p->freq_sel][dqs_i];
                    }
                }
                if(p->pSavetimeData->u1Gating2T_Save[p->channel][p->rank][p->freq_sel][0]==p->pSavetimeData->u1Gating2T_Save[p->channel][p->rank][p->freq_sel][1])  //2T byte0 = byte1
                {
                    if(p->pSavetimeData->u1Gating05T_Save[p->channel][p->rank][p->freq_sel][0]>p->pSavetimeData->u1Gating05T_Save[p->channel][p->rank][p->freq_sel][1]) //05T byte0 >byte1		              	{
                    {
                        ucdly_coarse_0p5T=p->pSavetimeData->u1Gating05T_Save[p->channel][p->rank][p->freq_sel][1];	//05T=byte1
                    }
                    else if((p->pSavetimeData->u1Gating05T_Save[p->channel][p->rank][p->freq_sel][0])< (p->pSavetimeData->u1Gating05T_Save[p->channel][p->rank][p->freq_sel][1])) //05T byte0 <byte1
                    {
                        ucdly_coarse_0p5T=p->pSavetimeData->u1Gating05T_Save[p->channel][p->rank][p->freq_sel][0];	//05T=byte0
                    }
                }

                if(ucdly_coarse_0p5T>=p->pSavetimeData->u1GatingStartoffset)
                {
                    ucdly_coarse_0p5T=ucdly_coarse_0p5T-p->pSavetimeData->u1GatingStartoffset;
                }
                else
                {
                    ucCoarseStart=ucCoarseStart-1;
                    ucdly_coarse_0p5T=ucdly_coarse_0p5T+(ucRX_DQS_CTL_LOOP-p->pSavetimeData->u1GatingStartoffset);
                }
                ucCoarseStart=ucCoarseStart*ucRX_DQS_CTL_LOOP+ucdly_coarse_0p5T;
                ucCoarseEnd = ucCoarseStart+12;
            }
            else
#endif
            {
                /* ucCoarseStart
                 * 1. Depends on current freq's DQSINCTL setting
                 * 2. Preserves ~4UI before actual DQS delay value
                 */
                if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
                {
                    if (p->freqGroup == 1866)  //3733
                    {
                        ucCoarseStart = 23;
                    }
                    else if (p->freqGroup == 1600)  //3200
                    {
                        ucCoarseStart = 29;
                    }
                    else if (p->freqGroup == 1333) //2667
                    {
                        ucCoarseStart = 22;
                    }
                    else if (p->freqGroup == 1200) //2400, 2280
                    {
                        ucCoarseStart = 22;
                    }
                    else//1600
                    {
                        ucCoarseStart = 16;
                    }
                }
                else // CBT_NORMAL_MODE
                {
                    if (p->freqGroup == 1866)  //3733
                    {   // Fine-tuned on Bianco (CoarseStart: (2, 6, 0), Gating DQS dly: NTV1(3, 1, 8) ~ NTV5(3, 1, 16))
                        ucCoarseStart = 22;
                    }
                    else if (p->freqGroup == 1600)  //3200
                    {   // Fine-tuned on Bianco (CoarseStart: (3, 3, 0), Gating DQS dly: NTV1(3, 7, 12) ~ NTV3(4, 0, 28))
                        ucCoarseStart = 27;
                    }
                    else if (p->freqGroup == 1333) //2667, 2400
                    {   // Fine-tuned on Bianco (CoarseStart: (3, 1, 0))
                        // DDR2667 - Gating DQS dly: NTV1(3, 5, 24) ~ NTV3(3, 7, 2)
                        // DDR2400 - Gating DQS dly: NTV1(3, 5, 0)  ~ NTV3(3, 6, 4)
                        ucCoarseStart = 25;
                    }
                    else if (p->freqGroup == 1200)
                    {
                        ucCoarseStart = 25;
                    }
                    else//1600
                    {  // Fine-tuned on Bianco (CoarseStart: (2, 2, 0), Gating DQS dly: NTV5(2, 6, 22) ~ NTV4(2, 7, 8))
                        ucCoarseStart = 18;
                    }
                }

                ucCoarseEnd = ucCoarseStart + 12;
            }
        }
#if ENABLE_LP3_SW
        else //LPDDR3
        {
            ucFreqDiv= 2;
            ucDQS_GW_FINE_STEP = DQS_GW_FINE_STEP;

            if(p->frequency >= 933)     //1600
                ucCoarseStart = 8;
            else if(p->frequency >= 800)     //1600
                ucCoarseStart = 8;
            else if(p->frequency >= 600)  //1333
                ucCoarseStart = 8;
            else //if(p->frequency >= 533)  //933
                ucCoarseStart = 5;

            #if FOR_DV_SIMULATION_USED
                    if(ucCoarseStart >=5)
                        ucCoarseStart -=5;
                    else
                        ucCoarseStart=0;
            #endif

            ucCoarseEnd = ucCoarseStart+8;
        }
#endif

    #if GATING_RODT_LATANCY_EN  //LP3 RODT is not enable, don't need to set the RODT settings.
        // Fix build warning, initialize variables.
        ucdly_coarse_large_RODT = 0;
        ucdly_coarse_0p5T_RODT = 0;

        if (u1IsLP4Family(p->dram_type))
        {
            ucdly_coarse_large_RODT_P1 = 4;
            ucdly_coarse_0p5T_RODT_P1 = 4;
        }
#if ENABLE_LP3_SW
        else
        {
            ucdly_coarse_large_RODT_P1 = 2;
            ucdly_coarse_0p5T_RODT_P1 = 2;
        }
#endif /* ENABLE_LP3_SW */

        // 1.   DQSG latency =
        // (1)   R_DMR*DQSINCTL[3:0] (MCK) +
        // (2)   selph_TX_DLY[2:0] (MCK) +
        // (3)   selph_dly[2:0] (UI)

        // 2.   RODT latency =
        // (1)   R_DMTRODT[3:0] (MCK) +
        // (2)   selph_TX_DLY[2:0] (MCK) +
        // (3)   selph_dly[2:0] (UI)

        #if 0 
        if(u1IsLP4Family(p->dram_type))
        {
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
        }
    #endif
    #endif

#if (SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_GatingCal)
	if(p->pSavetimeData->femmc_Ready==1 || ((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x02)>>1)==1)
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
            if(u1IsLP4Family(p->dram_type))
            {
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
            if(u1IsLP4Family(p->dram_type))
            {
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
            }
        #endif

            for (ucdly_fine_xT=DQS_GW_FINE_START; ucdly_fine_xT<DQS_GW_FINE_END; ucdly_fine_xT+=ucDQS_GW_FINE_STEP)
            {
                #if LP4_GATING_LEAD_LAG_FLAG_JUDGE
                if(u1IsLP4Family(p->dram_type))
                {
                    DramcGatingMode(p, 0);
                }
                #endif

                //ok we set a coarse/fine tune value already
                u4value = ucdly_fine_xT | (ucdly_fine_xT<<8) | (ucdly_fine_xT<<16) | (ucdly_fine_xT<<24);
                vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQSIEN), u4value);

                //reset phy, reset read data counter
                DramPhyReset(p);

                //reset DQS counter
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1, SPCMD_DQSGCNTRST);
                mcDELAY_US(1);//delay 2T
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_DQSGCNTRST);

                // enable TE2, audio pattern
                DramcEngine2Run(p, TE_OP_READ_CHECK, TEST_AUDIO_PATTERN);

                if(u1IsLP4Family(p->dram_type))
                {
                    u4all_result_R = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_STBERR_RK0_R), MISC_STBERR_RK0_R_STBERR_RK0_R);
                    u4all_result_F = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_STBERR_RK0_F), MISC_STBERR_RK0_F_STBERR_RK0_F);
               }
                #if ENABLE_LP3_SW
                else //LPDDR3
                {
                    LP3_DataPerByte[0] = u4IO32ReadFldAlign(DDRPHY_MISC_STBERR_RK0_R, MISC_STBERR_RK0_R_STBERR_RK0_R);  // CHA for B0 and B1
                    LP3_DataPerByte[1] = u4IO32ReadFldAlign(DDRPHY_MISC_STBERR_RK0_R+(1<< POS_BANK_NUM), MISC_STBERR_RK0_R_STBERR_RK0_R);// CHB for B2 and B3
                    u4all_result_R = LP3_DataPerByte[0] | (LP3_DataPerByte[1] <<16);

                    LP3_DataPerByte[0] = u4IO32ReadFldAlign(DDRPHY_MISC_STBERR_RK0_F, MISC_STBERR_RK0_F_STBERR_RK0_F);  // CHA for B0 and B1
                    LP3_DataPerByte[1] = u4IO32ReadFldAlign(DDRPHY_MISC_STBERR_RK0_F+(1<< POS_BANK_NUM), MISC_STBERR_RK0_F_STBERR_RK0_F);// CHB for B2 and B3
                    u4all_result_F = LP3_DataPerByte[0] | (LP3_DataPerByte[1] <<16);
                }
                #endif

                 //read DQS counter
                u4DebugCnt[0] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DQSGNWCNT0));
                u4DebugCnt[1] = (u4DebugCnt[0] >> 16) & 0xffff;
                u4DebugCnt[0] &= 0xffff;
#if ENABLE_LP3_SW
                if(p->dram_type == TYPE_LPDDR3)
                {
                    u4DebugCnt[2] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DQSGNWCNT1));
                    u4DebugCnt[3] = (u4DebugCnt[2] >> 16) & 0xffff;
                    u4DebugCnt[2] &= 0xffff;
                }
#endif /* ENABLE_LP3_SW */
                 u4err_value =0;

                #if LP4_GATING_LEAD_LAG_FLAG_JUDGE
                if(u1IsLP4Family(p->dram_type))
                {
                    DramcGatingMode(p, 1);
                    DramcEngine2Run(p, TE_OP_READ_CHECK, TEST_AUDIO_PATTERN);// trigger read to make lead/lag flag update.

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
                                if(u1DQS_transition[dqs_i] ==1)
                                {
                                    mcSHOW_DBG_MSG(("[Byte %d] Lead/lag falling Transition (%d, %d, %d)\n", dqs_i,ucdly_coarse_large_leadLag[dqs_i], ucdly_coarse_0p5T_leadLag[dqs_i], ucdly_fine_tune_leadLag[dqs_i]));
                                }
                                u1DQS_transition[dqs_i] ++;
                            }
                            else if((u1DQS_lead[dqs_i]==0) && (u1DQS_lag[dqs_i]==0))
                            {
                                mcSHOW_DBG_MSG(("[Byte %d] Lead/lag Transition tap number (%d)\n", dqs_i, u1DQS_transition[dqs_i]));
                                u1DQS_high[dqs_i] =0;
                             }
                        }
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
                mcSHOW_DBG_MSG(("\n"));
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
                    if(u1IsLP4Family(p->dram_type))
                    {
                       if((ucdqs_result_R==0) && (ucdqs_result_F==0) && (u2DebugCntPerByte==GATING_GOLDEND_DQSCNT_LP4))
                            ucCurrentPass =1;
                    }
                    #if ENABLE_LP3_SW
                    else //LPDDR3
                    {
                        if((ucdqs_result_R==0) && (ucdqs_result_F==0) && (u2DebugCntPerByte==GATING_GOLDEND_DQSCNT_LP3))
                        {
                                ucCurrentPass =1;
                                ucDQS_GW_FINE_STEP = 1;
                        }
                    }
                    #endif

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
                            mcSHOW_DBG_MSG2(("[Byte %d]First pass (%d, %d, %d)\n", dqs_i, ucdly_coarse_large, ucdly_coarse_0p5T, ucdly_fine_xT));
                        }
			#if 0//SUPPORT_SAVE_TIME_FOR_CALIBRATION
			if(p->pSavetimeData->femmc_Ready==1 && ((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x02)>>1)==0)
			{

                       if (ucpass_begin[0]==1 && ucpass_begin[1]==1 && ucpass_offset==0 && ucpass_count_1[0]>=3 && ucpass_count_1[1]>=3)
                        {
				ucCoarseTune_offset=p->pSavetimeData->u1GatingPass_Winoffset;
				ucpass_count_1[0]= ucpass_count_1[0]+(((DQS_GW_FINE_END-DQS_GW_FINE_STEP)-ucdly_fine_xT)/DQS_GW_FINE_STEP)+(ucCoarseTune_offset*DQS_GW_FINE_END/DQS_GW_FINE_STEP);
				ucpass_count_1[1]= ucpass_count_1[1]+(((DQS_GW_FINE_END-DQS_GW_FINE_STEP)-ucdly_fine_xT)/DQS_GW_FINE_STEP)+(ucCoarseTune_offset*DQS_GW_FINE_END/DQS_GW_FINE_STEP);
	                     ucCoarseTune=ucCoarseTune_offset+ucCoarseTune;
				ucpass_offset=1;
                            ucdly_fine_xT=DQS_GW_FINE_END;
                            dqs_i=(p->data_width/DQS_BIT_NUMBER);
                         }
                        else if (ucpass_begin[dqs_i]==1)
                        {
                            //incr pass tap number
                            ucpass_count_1[dqs_i]++;
                        }  

			}
			else
			#endif	
			{
                        if (ucpass_begin[dqs_i]==1)
                        {
                            //incr pass tap number
                            ucpass_count_1[dqs_i]++;
                        }
                    }

#if LP4_GATING_LEAD_LAG_FLAG_JUDGE
                        // Francis add to prevent lead/lag transition miss judgement
                        if (u1IsLP4Family(p->dram_type) && (ucpass_begin[dqs_i] == 1) && (ucpass_count_1[dqs_i]*ucDQS_GW_FINE_STEP > 32))
                        {
                            u1DQS_high[dqs_i] =0;   //no count lead/lag
                        }
                        
                        if (u1IsLP4Family(p->dram_type) && (ucpass_count_1[0]*ucDQS_GW_FINE_STEP > 32) && (ucpass_count_1[1]*ucDQS_GW_FINE_STEP > 32))
                        {
                            // if all bytes gating windows > 1UI, then early break;
                            mcSHOW_DBG_MSG2(("All bytes gating window > 1UI, Early break!\n"));
                            mcFPRINTF((fp_A60501, "All bytes gating window > 1UI, Early break!\n"));
                            ucdly_fine_xT = DQS_GW_FINE_END;//break loop
                            ucCoarseTune = ucCoarseEnd;      //break loop                                    
                        
                        }         
#endif                        
                    }
                    else // current tap is fail
                    {
                        if (ucpass_begin[dqs_i]==1)
                        {
                            //at the end of pass range
                            ucpass_begin[dqs_i] = 0;

                            //save the max range settings, to avoid glitch
                            if (ucpass_count_1[dqs_i] > ucpass_count[dqs_i])
                            {
                                ucmin_coarse_tune2T[dqs_i] = ucmin_coarse_tune2T_1[dqs_i];
                                ucmin_coarse_tune0p5T[dqs_i] = ucmin_coarse_tune0p5T_1[dqs_i];
                                ucmin_fine_tune[dqs_i] = ucmin_fine_tune_1[dqs_i];
                                ucpass_count[dqs_i] = ucpass_count_1[dqs_i];

#if ENABLE_LP3_SW
                                //LP3 CHB_CA didn't has lead/lag RG, use SW workaround
                                if (!u1IsLP4Family(p->dram_type))
                                {
                                    if (ucpass_count_1[dqs_i] >= 70) // if didn't check fail region and pass UI more than 2UI, then workaround back to 2UI
                                    {
                                        ucpass_count_1[dqs_i] = 64; // 2UI
                                        ucpass_count[dqs_i] = ucpass_count_1[dqs_i];
                                    }
                                }
#endif

                                /*TINFO="[Byte %d]Bigger pass win(%d, %d, %d)  Pass tap=%d\n", \
                                    dqs_i, ucmin_coarse_tune2T_1[dqs_i], ucmin_coarse_tune0p5T_1[dqs_i], ucmin_fine_tune_1[dqs_i], ucpass_count_1[dqs_i]*/
                                mcSHOW_DBG_MSG(("[Byte %d]Bigger pass win(%d, %d, %d)  Pass tap=%d\n", \
                                    dqs_i, ucmin_coarse_tune2T_1[dqs_i], ucmin_coarse_tune0p5T_1[dqs_i], ucmin_fine_tune_1[dqs_i], ucpass_count_1[dqs_i]));

                                #ifdef FOR_HQA_TEST_USED
                                gFinalGatingWin[p->channel][p->rank][dqs_i] = ucpass_count_1[dqs_i]*ucDQS_GW_FINE_STEP;
                                #endif

                                // LP4 pass window around 6 UI(burst mode), set 1~3 UI is pass
                                // LP3 pass window around 2 UI(pause mode), set 1~3 UI is pass
                                if((ucpass_count_1[dqs_i]*ucDQS_GW_FINE_STEP > 32) && (ucpass_count_1[dqs_i]*ucDQS_GW_FINE_STEP < 96))
                                {
                                    u1PassByteCount  |= (1<<dqs_i);
                                }

                                if(((u1IsLP4Family(p->dram_type)) && (u1PassByteCount==0x3)) || ((p->dram_type==TYPE_LPDDR3) && (u1PassByteCount==0xf)))
                                {
                                    mcSHOW_DBG_MSG2(("All bytes gating window pass Done, Early break!\n"));
                                    mcFPRINTF((fp_A60501, "All bytes gating window pass Done, Early break!\n"));
                                    ucdly_fine_xT = DQS_GW_FINE_END;//break loop
                                    ucCoarseTune = ucCoarseEnd;      //break loop
                                }
                            }
                        }
                    }
                }
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
                #if 0 //SUPPORT_SAVE_TIME_FOR_CALIBRATION
	   	  if(p->pSavetimeData->femmc_Ready==1 &&  ((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x02)>>1)==0)
	         {
                   p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]]=1<<1; //gating window fail
	   	  }
                #endif
            }

            #if ENABLE_CLAIBTAION_WINDOW_LOG_FOR_FT
            if(ucpass_count[dqs_i] < u2MinWinSize)
            {
                u2MinWinSize = ucpass_count[dqs_i];
                u1MinWinSizeByteidx= dqs_i;
            }
            #endif
        }

        #if LP4_GATING_OLD_BURST_MODE
        if(u1IsLP4Family(p->dram_type))
        {
            //Set the gating window begin as gating position
            //ucpass_count[0]=0;
            //ucpass_count[1]=0;

            //Set the gating window end-2UI as gating position
            ucpass_count[0] <<=1;
            ucpass_count[1] <<=1;
        }
        #else
        if(u1IsLP4Family(p->dram_type))
        {
            // LP4 burst mode, may find 6 UI. The 2 UI of the begining may not work.
            //if gating window > 5UI, skip the first 2 UI.
            if((ucpass_count_1[0]*ucDQS_GW_FINE_STEP > 160) &&(ucpass_count_1[1]*ucDQS_GW_FINE_STEP > 160))
            {
                mcSHOW_DBG_MSG2(("If gating window > 4.5UI, skip the first 2 UI! \n"));
                mcFPRINTF((fp_A60501, "If gating window > 4.5UI, skip the first 2 UI!\n"));

                for (dqs_i=0; dqs_i<2; dqs_i++)
                {
                    ucmin_coarse_tune0p5T[dqs_i] +=2;

                    if(ucmin_coarse_tune0p5T[dqs_i] > ucRX_DQS_CTL_LOOP)
                    {
                        ucmin_coarse_tune0p5T[dqs_i] -= ucRX_DQS_CTL_LOOP;
                        ucmin_coarse_tune2T[dqs_i] +=1;
                    }
                    mcSHOW_DBG_MSG2(("Change Byte%d Window begin to (%d, %d)\n", dqs_i, ucmin_coarse_tune2T[dqs_i], ucmin_coarse_tune0p5T[dqs_i]));
                    mcFPRINTF((fp_A60501, "Change Byte%d Window begin to (%d, %d)\n", dqs_i ucmin_coarse_tune2T[dqs_i], ucmin_coarse_tune0p5T[dqs_i]));
                }
            }
        }
        #endif

        #if ENABLE_CLAIBTAION_WINDOW_LOG_FOR_FT
        mcSHOW_DBG_MSG2(("FT log: Gating min window : byte %d, size %d\n", u1MinWinSizeByteidx, u2MinWinSize*ucDQS_GW_FINE_STEP));
        #endif
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
		if(p->pSavetimeData->femmc_Ready==1 || ((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x02)>>1)==1)
       	{
		ucmin_coarse_tune2T[dqs_i]=p->pSavetimeData->u1Gating2T_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][dqs_i];
		ucmin_coarse_tune0p5T[dqs_i]=p->pSavetimeData->u1Gating05T_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][dqs_i];
		ucmin_fine_tune[dqs_i]=p->pSavetimeData->u1Gatingfine_tune_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][dqs_i];
		ucpass_count[dqs_i]=p->pSavetimeData->u1Gatingucpass_count_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][dqs_i];
            vSetCalibrationResult(p, DRAM_CALIBRATION_GATING, DRAM_OK);
		}
		else
#endif
{			

        #if LP4_GATING_LEAD_LAG_FLAG_JUDGE
        if(u1IsLP4Family(p->dram_type))
            {
                ucpass_count[dqs_i] = u1DQS_transition[dqs_i];
                ucmin_fine_tune[dqs_i] = ucdly_fine_tune_leadLag[dqs_i];
                ucmin_coarse_tune0p5T[dqs_i] = ucdly_coarse_0p5T_leadLag[dqs_i];
                ucmin_coarse_tune2T[dqs_i] = ucdly_coarse_large_leadLag[dqs_i];
            }
    #endif
}
     #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
		if(p->pSavetimeData->femmc_Ready==0 || ((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x02)>>1)==1)
         {
		p->pSavetimeData->u1Gating2T_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][dqs_i]=ucmin_coarse_tune2T[dqs_i];
		p->pSavetimeData->u1Gating05T_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][dqs_i]=ucmin_coarse_tune0p5T[dqs_i];
		p->pSavetimeData->u1Gatingfine_tune_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][dqs_i]=ucmin_fine_tune[dqs_i];
		p->pSavetimeData->u1Gatingucpass_count_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][dqs_i]=ucpass_count[dqs_i];
            // mcSHOW_DBG_MSG(("save Gating\n"));
     	}
     #endif      
        // -- PI for Phase0 & Phase1 --
            #if (LP4_GATING_END_MINUS2UI==1 && LP4_GATING_OLD_BURST_MODE==0)
            mcSHOW_DBG_MSG(("ucpass_count[dqs_i]*ucDQS_GW_FINE_STEP=%d\n", (ucpass_count[dqs_i]*ucDQS_GW_FINE_STEP)));
            if((ucpass_count[dqs_i]*ucDQS_GW_FINE_STEP)<128)//window size < 4UI : select center
            {
                uctmp_offset = ucpass_count[dqs_i]*ucDQS_GW_FINE_STEP/2;
            }
            else // select window_end-2UI
            {
                uctmp_offset = (ucpass_count[dqs_i]*ucDQS_GW_FINE_STEP) - 64;
            }
            #else
            uctmp_offset = ucpass_count[dqs_i]*ucDQS_GW_FINE_STEP/2;
            #endif
            
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

        //mcSHOW_DBG_MSG(("test2_1: 0x%x, test2_2: 0x%x, test pattern: %d\n",  p->test2_1,p->test2_2, p->test_pattern));
        //mcSHOW_DBG_MSG3(("\tdqs input gating window, best dly value\n\n"));


        mcFPRINTF((fp_A60501, "\n\tdqs input gating window, final dly value\n\n"));
        //mcFPRINTF((fp_A60501, "test2_1: 0x%x, test2_2: 0x%x, test pattern: %d\n",  p->test2_1,p->test2_2, p->test_pattern));
        mcFPRINTF((fp_A60501, "\tdqs input gating window, best dly value\n\n"));

        if(u1IsLP4Family(p->dram_type))
        {
            for (dqs_i=2; dqs_i<4; dqs_i++)  // LP4, DQ byte 2 and byte 3 are useless, set gating result as 0.
            {
#if ENABLE_RODT_TRACKING //when RODT tracking on (Bianco), TX DLY of byte2,3 must not be zero
                ucbest_coarse_tune2T[dqs_i] = ucbest_coarse_tune0p5T[dqs_i] = ucbest_fine_tune[dqs_i]= 2;   //must >= 2
                ucbest_coarse_tune2T_P1[dqs_i] = ucbest_coarse_tune0p5T_P1[dqs_i] = ucbest_fine_tune_P1[dqs_i]= 2;  //must >= 2
#else
                ucbest_coarse_tune2T[dqs_i] = ucbest_coarse_tune0p5T[dqs_i] = ucbest_fine_tune[dqs_i]= 0;
                ucbest_coarse_tune2T_P1[dqs_i] = ucbest_coarse_tune0p5T_P1[dqs_i] = ucbest_fine_tune_P1[dqs_i]= 0;
#endif                

                #if GATING_ADJUST_TXDLY_FOR_TRACKING
                #if ENABLE_RODT_TRACKING //if changeDQSINCTL != 0, TX DLY of byte2,3 will be replaced by ucbest_coarse_tune2T_backup in DramcRxdqsGatingPostProcess function, so also set default of backup as 2
                                ucbest_coarse_tune2T_backup[p->rank][dqs_i] = ucbest_coarse_tune0p5T_backup[p->rank][dqs_i] = 2;
                                ucbest_coarse_tune2T_P1_backup[p->rank][dqs_i] = ucbest_coarse_tune0p5T_P1_backup[p->rank][dqs_i] = 2;
                #else
                                ucbest_coarse_tune2T_backup[p->rank][dqs_i] = ucbest_coarse_tune0p5T_backup[p->rank][dqs_i] = 0;
                                ucbest_coarse_tune2T_P1_backup[p->rank][dqs_i] = ucbest_coarse_tune0p5T_P1_backup[p->rank][dqs_i] = 0;
                #endif
                #endif
            }
        }

        for (dqs_i=0; dqs_i<(p->data_width/DQS_BIT_NUMBER); dqs_i++)
        {
            /*TINFO="best DQS%d delay(2T, 0.5T, PI) = (%d, %d, %d)\n", dqs_i, ucbest_coarse_tune2T[dqs_i], ucbest_coarse_tune0p5T[dqs_i], ucbest_fine_tune[dqs_i]*/
            mcSHOW_DBG_MSG(("\nbest DQS%d dly(2T, 0.5T, PI) = (%d, %d, %d)\n", dqs_i, ucbest_coarse_tune2T[dqs_i], ucbest_coarse_tune0p5T[dqs_i], ucbest_fine_tune[dqs_i]));
            mcFPRINTF((fp_A60501,"best DQS%d dly(2T, 0.5T, PI) = (%d, %d, %d)\n", dqs_i, ucbest_coarse_tune2T[dqs_i], ucbest_coarse_tune0p5T[dqs_i], ucbest_fine_tune[dqs_i]));
            #if GATING_ADJUST_TXDLY_FOR_TRACKING
            // find min gating TXDLY (should be in P0)
            if (u1IsLP4Family(p->dram_type))
            {
                u1TX_dly_DQSgated =ucbest_coarse_tune2T[dqs_i];
            }
#if ENABLE_LP3_SW
            else
            {
                u1TX_dly_DQSgated  = ((ucbest_coarse_tune2T[dqs_i] <<1)|((ucbest_coarse_tune0p5T[dqs_i] >>2)&0x1));
            }
#endif /* ENABLE_LP3_SW */

            if(u1TX_dly_DQSgated < u1TXDLY_Cal_min)
                u1TXDLY_Cal_min = u1TX_dly_DQSgated;

            ucbest_coarse_tune0p5T_backup[p->rank][dqs_i] = ucbest_coarse_tune0p5T[dqs_i];
            ucbest_coarse_tune2T_backup[p->rank][dqs_i] = ucbest_coarse_tune2T[dqs_i];
            #endif
        }

        mcSHOW_DBG_MSG(("\n\n"));
        mcFPRINTF((fp_A60501,"\n\n"));

        for (dqs_i=0; dqs_i<(p->data_width/DQS_BIT_NUMBER); dqs_i++)
        {
            /*TINFO="best DQS%d P1 delay(2T, 0.5T, PI) = (%d, %d, %d)\n", dqs_i, ucbest_coarse_tune2T_P1[dqs_i], ucbest_coarse_tune0p5T_P1[dqs_i], ucbest_fine_tune[dqs_i]*/
            mcSHOW_DBG_MSG(("\nbest DQS%d P1 dly(2T, 0.5T, PI) = (%d, %d, %d)\n", dqs_i, ucbest_coarse_tune2T_P1[dqs_i], ucbest_coarse_tune0p5T_P1[dqs_i], ucbest_fine_tune[dqs_i]));
            mcFPRINTF((fp_A60501,"best DQS%d P1 dly(2T, 0.5T, PI) = (%d, %d, %d)\n", dqs_i, ucbest_coarse_tune2T_P1[dqs_i], ucbest_coarse_tune0p5T_P1[dqs_i], ucbest_fine_tune[dqs_i]));

            #if GATING_ADJUST_TXDLY_FOR_TRACKING
            // find max gating TXDLY (should be in P1)

            if (u1IsLP4Family(p->dram_type))
            {
                u1TX_dly_DQSgated  = ucbest_coarse_tune2T_P1[dqs_i];
            }
#if ENABLE_LP3_SW
            else
            {
                u1TX_dly_DQSgated  = ((ucbest_coarse_tune2T_P1[dqs_i] <<1)|((ucbest_coarse_tune0p5T_P1[dqs_i] >>2)&0x1));
            }
#endif /* ENABLE_LP3_SW */

            if(u1TX_dly_DQSgated > u1TXDLY_Cal_max)
                u1TXDLY_Cal_max = u1TX_dly_DQSgated;

            ucbest_coarse_tune0p5T_P1_backup[p->rank][dqs_i] = ucbest_coarse_tune0p5T_P1[dqs_i];
            ucbest_coarse_tune2T_P1_backup[p->rank][dqs_i] = ucbest_coarse_tune2T_P1[dqs_i];
            #endif
        }

        mcSHOW_DBG_MSG(("\n\n"));
        mcFPRINTF((fp_A60501,"\n\n"));

        //Restore registers
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), u4BakReg_DRAMC_DQSCAL0);
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_STBCAL1), u4BakReg_DRAMC_STBCAL_F);
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_DDRCONF0), u4BakReg_DRAMC_WODT);
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), u4BakReg_DRAMC_SPCMD);
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0), u4BakReg_DRAMC_REFCTRL0);
        #if LP4_GATING_OLD_BURST_MODE
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), u4BakReg_DDRPHY_B0_DQ6);
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), u4BakReg_DDRPHY_B1_DQ6);

        if(u1IsLP4Family(p->dram_type))
        {
            #if !LP4_GATING_LEAD_LAG_FLAG_JUDGE
            DramcGatingMode(p, 1);
            #endif
            //MR1 OP[7]=0;
            u1MR01Value[p->dram_fsp] &= 0x7f;
            DramcModeRegWriteByRank(p, p->rank, 0x1, u1MR01Value[p->dram_fsp]);
        }
        #else
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B0_DQ7), u4BakReg_DDRPHY_B0_DQ7);
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B1_DQ7), u4BakReg_DDRPHY_B1_DQ7);
        #endif

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
    if(u1IsLP4Family(p->dram_type))
        {
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

#if ENABLE_RODT_TRACKING
            //Because Kibo+,WE2,Bianco,Vinson...or behind project support WDQS, they need to apply the correct new setting
            //The following 2 items are indepentent
            //1. if TX_WDQS on(by vendor_id) or p->odt_onoff = 1, ROEN/RODTE/RODTE2 = 1
            //2. if ENABLE_RODT_TRACKING on, apply new setting and RODTEN_MCK_MODESEL = ROEN
            U8 u1ReadROEN;
            u1ReadROEN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_ODTCTRL), SHU_ODTCTRL_ROEN);
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_RODTENSTB), P_Fld(0xff, SHU_RODTENSTB_RODTENSTB_EXT)|\
                                                                        P_Fld(9, SHU_RODTENSTB_RODTENSTB_OFFSET)|\
                                                                        P_Fld(u1ReadROEN, SHU_RODTENSTB_RODTEN_MCK_MODESEL));
#endif
#ifdef XRTR2W_PERFORM_ENHANCE_RODTEN
          // LP4 support only
          if (u1IsLP4Family(p->dram_type))
          {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_RODTENSTB), P_Fld(0x0fff, SHU_RODTENSTB_RODTENSTB_EXT)|\
                                                                        P_Fld(1, SHU_RODTENSTB_RODTEN_P1_ENABLE)|\
                                                                        P_Fld(1, SHU_RODTENSTB_RODTEN_MCK_MODESEL));
          }
#endif

        }
    #endif

        // Set Fine Tune Value to registers
        u4value = ucbest_fine_tune[0] | (ucbest_fine_tune[1]<<8) | (ucbest_fine_tune[2]<<16) | (ucbest_fine_tune[3]<<24);
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQSIEN), u4value);

     #if 0 //SUPPORT_SAVE_TIME_FOR_CALIBRATION
		if(p->pSavetimeData->femmc_Ready==0 || ((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x02)>>1)==1)
         {
        	for (dqs_i=0; dqs_i<(p->data_width/DQS_BIT_NUMBER); dqs_i++)
        	{
		p->pSavetimeData->u1Gating2T_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][dqs_i]=ucmin_coarse_tune2T_1[dqs_i];
		p->pSavetimeData->u1Gating05T_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][dqs_i]=ucmin_coarse_tune0p5T_1[dqs_i];
         	}

            // mcSHOW_DBG_MSG(("save Gating\n"));
     	}
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

#ifdef DRAM_CALIB_LOG
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].GatingWindow.DQS0_2T = ucbest_coarse_tune2T_P1[0];
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].GatingWindow.DQS0_05T = ucbest_coarse_tune0p5T_P1[0];
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].GatingWindow.DQS0_PI = ucbest_fine_tune[0];
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].GatingWindow.DQS1_2T = ucbest_coarse_tune2T_P1[1];
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].GatingWindow.DQS1_05T = ucbest_coarse_tune0p5T_P1[1];
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].GatingWindow.DQS1_PI = ucbest_fine_tune[1];
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].GatingWindow.DQS2_2T = ucbest_coarse_tune2T_P1[2];
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].GatingWindow.DQS2_05T = ucbest_coarse_tune0p5T_P1[2];
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].GatingWindow.DQS2_PI = ucbest_fine_tune[2];
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].GatingWindow.DQS3_2T = ucbest_coarse_tune2T_P1[3];
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].GatingWindow.DQS3_05T = ucbest_coarse_tune0p5T_P1[3];
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].GatingWindow.DQS3_PI = ucbest_fine_tune[3];

        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].GatingWindow.DQS0_RODT_2T = ucbest_coarse_large_RODT[0];
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].GatingWindow.DQS0_RODT_05T = ucbest_coarse_0p5T_RODT[0];
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].GatingWindow.DQS1_RODT_2T = ucbest_coarse_large_RODT[1];
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].GatingWindow.DQS1_RODT_05T = ucbest_coarse_0p5T_RODT[1];
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].GatingWindow.DQS2_RODT_2T = ucbest_coarse_large_RODT[2];
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].GatingWindow.DQS2_RODT_05T = ucbest_coarse_0p5T_RODT[2];
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].GatingWindow.DQS3_RODT_2T = ucbest_coarse_large_RODT[3];
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].GatingWindow.DQS3_RODT_05T = ucbest_coarse_0p5T_RODT[3];
#endif

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
    U8 dqs_i, u1RankRxDVS;
    U8 u1RankIdx, u1RankMax, u1RankBak;
    S8 s1ChangeDQSINCTL;
    U32 backup_rank;
    U32 u4ReadDQSINCTL, u4ReadRODT, u4ReadTXDLY[RANK_MAX][DQS_NUMBER], u4ReadTXDLY_P1[RANK_MAX][DQS_NUMBER], u4RankINCTL_ROOT, u4XRTR2R, reg_TX_dly_DQSgated_min = 0;

    backup_rank = u1GetRank(p);

#ifdef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
    if (u1IsLP4Family(p->dram_type))
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
#if ENABLE_LP3_SW
    else
    {
        // 1866,1600,1333,1200  : reg_TX_dly_DQSgated (min) =2
        reg_TX_dly_DQSgated_min = 2;
    }
#endif /* ENABLE_LP3_SW */
#else
    if (u1IsLP4Family(p->dram_type))
    {
        // wei-jen: DQSgated_min should be 3 when freq >= 1333, 2 when freq < 1333
        if (p->frequency >= 1333)
        {
            reg_TX_dly_DQSgated_min = 3;
        }
        else
        {
            reg_TX_dly_DQSgated_min = 2;
        }
    }
#if ENABLE_LP3_SW
    else
    {
        // 800  : reg_TX_dly_DQSgated (min) =2
        // 1066 : reg_TX_dly_DQSgated (min) =2
        // 1270 : reg_TX_dly_DQSgated (min) =2
        // 1600 : reg_TX_dly_DQSgated (min) =3
        // 1866 : reg_TX_dly_DQSgated (min) =3
        if(p->frequency < 700)
        {
            reg_TX_dly_DQSgated_min = 2;
        }
        else
        {
            reg_TX_dly_DQSgated_min = 3;
        }
    }
#endif /* ENABLE_LP3_SW */
#endif

    // === Begin of DVS setting =====
    //RANKRXDVS = reg_TX_dly_DQSgated (min) -1 = Roundup(tDQSCKdiff/MCK)
    if(reg_TX_dly_DQSgated_min>1)
    {
        u1RankRxDVS = reg_TX_dly_DQSgated_min -1;
    }
    else
    {
        u1RankRxDVS=0;
        mcSHOW_ERR_MSG(("[RxdqsGatingPostProcess] u1RankRxDVS <1,  Please check!\n"));
    }

    if(u1IsLP4Family(p->dram_type))
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ7), u1RankRxDVS, SHU1_B0_DQ7_R_DMRANKRXDVS_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DQ7), u1RankRxDVS, SHU1_B1_DQ7_R_DMRANKRXDVS_B1);
    }
#if ENABLE_LP3_SW
    else
    {
        for (dqs_i=0; dqs_i<(p->data_width/DQS_BIT_NUMBER); dqs_i++)
        {
            vIO32WriteFldAlign_Phy_Byte(dqs_i, DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ7), u1RankRxDVS, SHU1_B0_DQ7_R_DMRANKRXDVS_B0);
        }
    }
#endif /* ENABLE_LP3_SW */
    // === End of DVS setting =====

    s1ChangeDQSINCTL = reg_TX_dly_DQSgated_min- u1TXDLY_Cal_min;

    mcSHOW_DBG_MSG(("[RxdqsGatingPostProcess] freq %d\n"
                    "ChangeDQSINCTL %d, reg_TX_dly_DQSgated_min %d, u1TXDLY_Cal_min %d\n",
                        p->frequency,
                        s1ChangeDQSINCTL, reg_TX_dly_DQSgated_min, u1TXDLY_Cal_min));

#ifdef DRAM_CALIB_LOG
    gDRAM_CALIB_LOG.CHANNEL[p->channel].RxdqsGatingPostProcess.s1ChangeDQSINCTL = s1ChangeDQSINCTL;
    gDRAM_CALIB_LOG.CHANNEL[p->channel].RxdqsGatingPostProcess.reg_TX_dly_DQSgated_min = reg_TX_dly_DQSgated_min;
    gDRAM_CALIB_LOG.CHANNEL[p->channel].RxdqsGatingPostProcess.u1TXDLY_Cal_min = u1TXDLY_Cal_min;
#endif

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
                if(u1IsLP4Family(p->dram_type))
                {
                    u4ReadTXDLY[u1RankIdx][dqs_i]= ucbest_coarse_tune2T_backup[u1RankIdx][dqs_i];
                    u4ReadTXDLY_P1[u1RankIdx][dqs_i]= ucbest_coarse_tune2T_P1_backup[u1RankIdx][dqs_i];

                    u4ReadTXDLY[u1RankIdx][dqs_i] += s1ChangeDQSINCTL;
                    u4ReadTXDLY_P1[u1RankIdx][dqs_i] += s1ChangeDQSINCTL;

                    ucbest_coarse_tune2T_backup[u1RankIdx][dqs_i] = u4ReadTXDLY[u1RankIdx][dqs_i];
                    ucbest_coarse_tune2T_P1_backup[u1RankIdx][dqs_i] = u4ReadTXDLY_P1[u1RankIdx][dqs_i];
                }
                else
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
        }
    }
    vSetRank(p, backup_rank);

    u4ReadDQSINCTL = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQSCTL), SHURK0_DQSCTL_DQSINCTL);
    u4ReadDQSINCTL -= s1ChangeDQSINCTL;

    #if ENABLE_READ_DBI
    if(p->DBI_R_onoff[p->dram_fsp])
    {
      u4ReadDQSINCTL++;
      u4ReadRODT = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_ODTCTRL), SHU_ODTCTRL_RODT);
      vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_ODTCTRL), u4ReadRODT+1, SHU_ODTCTRL_RODT); //update RODT value when READ_DBI is on
    }
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

#if ENABLE_RODT_TRACKING
    //Because Kibo+,WE2,Bianco,Vinson...or behind project support WDQS, they need to apply the correct new setting
    //The following 2 items are indepentent
    //1. if TX_WDQS on(by vendor_id) or p->odt_onoff = 1, ROEN/RODTE/RODTE2 = 1
    //2. if ENABLE_RODT_TRACKING on, apply new setting and RODTEN_MCK_MODESEL = ROEN
    U8 u1ReadROEN;
    u1ReadROEN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_ODTCTRL), SHU_ODTCTRL_ROEN);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_RODTENSTB), P_Fld(0xff, SHU_RODTENSTB_RODTENSTB_EXT)|\
                                                                P_Fld(9, SHU_RODTENSTB_RODTENSTB_OFFSET)|\
                                                                P_Fld(u1ReadROEN, SHU_RODTENSTB_RODTEN_MCK_MODESEL));
#endif
#ifdef XRTR2W_PERFORM_ENHANCE_RODTEN
    // LP4 support only
    if (u1IsLP4Family(p->dram_type))
    {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_RODTENSTB), P_Fld(0x0fff, SHU_RODTENSTB_RODTENSTB_EXT)|\
                                                                P_Fld(1, SHU_RODTENSTB_RODTEN_P1_ENABLE)|\
                                                                P_Fld(1, SHU_RODTENSTB_RODTEN_MCK_MODESEL));
    }
#endif


    vSetRank(p, backup_rank);


#ifdef DRAM_CALIB_LOG
    gDRAM_CALIB_LOG.CHANNEL[p->channel].RxdqsGatingPostProcess.TX_dly_DQSgated_check_min = u1TXDLY_Cal_min;
    gDRAM_CALIB_LOG.CHANNEL[p->channel].RxdqsGatingPostProcess.TX_dly_DQSgated_check_max = u1TXDLY_Cal_max;
    gDRAM_CALIB_LOG.CHANNEL[p->channel].RxdqsGatingPostProcess.DQSINCTL = u4ReadDQSINCTL;
    gDRAM_CALIB_LOG.CHANNEL[p->channel].RxdqsGatingPostProcess.RANKINCTL = u4RankINCTL_ROOT;
    gDRAM_CALIB_LOG.CHANNEL[p->channel].RxdqsGatingPostProcess.u4XRTR2R = u4XRTR2R;
#endif
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

//-------------------------------------------------------------------------
/** DramcRxWindowPerbitCal (v2 version)
 *  start the rx dqs perbit sw calibration.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL
 */
//-------------------------------------------------------------------------
// default RX vref is 0xe=14
#if (SW_CHANGE_FOR_SIMULATION ||FOR_DV_SIMULATION_USED)
#define RX_VREF_RANGE_BEGIN       0
#define RX_VREF_RANGE_END          2
#define RX_VREF_RANGE_STEP         2
#else
#define RX_VREF_RANGE_BEGIN      0
#define RX_VREF_RANGE_END          31   //bit5 useless (Justin)   //field is 6 bit, but can only use 0~31
#define RX_VREF_RANGE_STEP         1
#endif


static U32 DramcRxWinRDDQCInit(DRAMC_CTX_T *p)
{
    U8 *uiLPDDR_PHY_Mapping;
    U16 temp_value=0;
    U8 MR_GOLDEN_MR15_GOLDEN_value=0, MR_GOLDEN_MR20_GOLDEN_value=0;
    int i;

    //U8 u1ReadDBIbak[2];
    //U32 u4MRS_reg_bak;

    // Disable Read DBI
    //u1ReadDBIbak[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ7), SHU1_B0_DQ7_R_DMDQMDBI_SHU_B0);
    //u1ReadDBIbak[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DQ7), SHU1_B1_DQ7_R_DMDQMDBI_SHU_B1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ7),  0, SHU1_B0_DQ7_R_DMDQMDBI_SHU_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DQ7),  0, SHU1_B1_DQ7_R_DMDQMDBI_SHU_B1);
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), 1, SPCMDCTRL_RDDQCDIS); Moved to "DramcRxWinRDDQCRun()"

    //u4MRS_reg_bak = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_MRS));
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1GetRank(p), MRS_MPCRK);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MPC_OPTION), 1, MPC_OPTION_MPCRKEN);

    uiLPDDR_PHY_Mapping = (U8 *)uiLPDDR4_PHY_Mapping_POP[p->channel];

    for(i=0; i<16; i++)
    {
        temp_value |= ((0x5555 >> i) & 0x1) << uiLPDDR_PHY_Mapping[i];
    }
    MR_GOLDEN_MR15_GOLDEN_value = (U8) temp_value & 0xff;
    MR_GOLDEN_MR20_GOLDEN_value = (U8) (temp_value>>8) & 0xff;


    //Set golden pattern from Shih-hsiu's suggestion. 2016/3/25 04:43pm, RE: [Olympus] RX per bit calibration. 
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_MR_GOLDEN), P_Fld(MR_GOLDEN_MR15_GOLDEN_value, MR_GOLDEN_MR15_GOLDEN) | P_Fld(MR_GOLDEN_MR20_GOLDEN_value, MR_GOLDEN_MR20_GOLDEN));

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
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1, SPCMD_RDDQCEN);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), 1, SPCMDCTRL_RDDQCDIS);
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
    //R_DMRDDQCEN, 0x1E4[7]=0
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_RDDQCEN);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), 0, SPCMDCTRL_RDDQCDIS);

    return u4Result;
}

static U32 DramcRxWinRDDQCEnd(DRAMC_CTX_T *p)
{
    //Recover Read DBI
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ7),  p->DBI_R_onoff, SHU1_B0_DQ7_R_DMDQMDBI_SHU_B0);
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DQ7),  p->DBI_R_onoff, SHU1_B1_DQ7_R_DMDQMDBI_SHU_B1);

    // Recover MPC Rank
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), 0, MRS_MPCRK);
}

#ifdef MPC_SW_FIFO
void DramcMPC_FIFO(DRAMC_CTX_T *p)
{
    U32 u4Result3,u4Result2,u4Result1,u4Result0;
	U32 u4Result, u4Response;
    U32 u4TimeCnt= TIME_OUT_CNT;
	U32 u4BakReg_DRAMC_REG_LBWDAT3, u4BakReg_DRAMC_REG_LBWDAT2, u4BakReg_DRAMC_REG_LBWDAT1, u4BakReg_DRAMC_REG_LBWDAT0;
	U32 u4BakReg_DRAMC_REG_REFCTRL0, u4BakReg_DRAMC_REG_SPCMDCTRL, u4BakReg_DRAMC_REG_SHU_SCINTV, u4BakReg_DRAMC_REG_PERFCTL0;

    mcSHOW_DBG_MSG(("[DramcMPC_FIFO]\n"));
 	//Register backup
 	u4BakReg_DRAMC_REG_LBWDAT3 = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_LBWDAT3));
 	u4BakReg_DRAMC_REG_LBWDAT2 = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_LBWDAT2));
 	u4BakReg_DRAMC_REG_LBWDAT1 = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_LBWDAT1));
 	u4BakReg_DRAMC_REG_LBWDAT0 = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_LBWDAT0));
 	
 	u4BakReg_DRAMC_REG_REFCTRL0 = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0));
 	u4BakReg_DRAMC_REG_SPCMDCTRL = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL));
 	u4BakReg_DRAMC_REG_SHU_SCINTV = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SHU_SCINTV));
 	u4BakReg_DRAMC_REG_PERFCTL0 = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_PERFCTL0));

	//WRFIFO and RDFIFO's golden data
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_LBWDAT3), 0xAAAAAAAA, LBWDAT3_LBWDATA3); 
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_LBWDAT2), 0x55555555, LBWDAT2_LBWDATA2);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_LBWDAT1), 0xAAAAAAAA, LBWDAT1_LBWDATA1); 
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_LBWDAT0), 0x55555555, LBWDAT0_LBWDATA0); 

	u4Result3 = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_LBWDAT3));
	mcSHOW_DBG_MSG(("DRAMC_REG_LBWDAT3: 0x%x\n", u4Result3));
	mcFPRINTF((fp_A60501, "DRAMC_REG_LBWDAT3: 0x%x\n", u4Result3));
	u4Result2 = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_LBWDAT2));
	mcSHOW_DBG_MSG(("DRAMC_REG_LBWDAT2: 0x%x\n", u4Result2));
	mcFPRINTF((fp_A60501, "DRAMC_REG_LBWDAT2: 0x%x\n", u4Result2));
	u4Result1 = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_LBWDAT1));
	mcSHOW_DBG_MSG(("DRAMC_REG_LBWDAT1: 0x%x\n", u4Result1));
	mcFPRINTF((fp_A60501, "DRAMC_REG_LBWDAT1: 0x%x\n", u4Result1));
	u4Result0 = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_LBWDAT0));
	mcSHOW_DBG_MSG(("DRAMC_REG_LBWDAT0: 0x%x\n", u4Result0));
	mcFPRINTF((fp_A60501, "DRAMC_REG_LBWDAT0: 0x%x\n", u4Result0));


	//Other command is not valid during "WRFIFO and RDFIFO"  period.
	//Disable auto refresh: set R_DMREFDIS=1
    vAutoRefreshSwitch(p, DISABLE);
	//Disable MR4: set R_DMREFRDIS=1
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), 1, SPCMDCTRL_REFRDIS);
	//Disable ZQCAL/ZQLAT command: set R_DMZQCALDISB=0
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), 0, SPCMDCTRL_ZQCALDISB);


	//When R_DMWRFIFOEN=1, MPC WRFIFO can send single request or 5 requests by R_DMRDDQC_INTV[1:0] (0x8C8[12:11])
	//Set R_DMRDDQC_INTV=2'b00 and Set R_DMWRFIO_MODE2 = 1'b0 for single MPC WRFIFO (single mode)
	//Set R_DMRDDQC_INTV=2'b01 and Set R_DMWRFIO_MODE2 = 1'b1 for five MPC WRFIFO (burst mode)
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SCINTV), 0, SHU_SCINTV_RDDQC_INTV);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PERFCTL0), 0, PERFCTL0_WRFIO_MODE2);


    //Issue MPC RD FIFO
    //R_DMWRFIFOEN, 0x0C[31]=1 for WR FIFO
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PERFCTL0), 1, PERFCTL0_WRFIFOEN);

	//Wait wrfifo_response=1 (dramc_conf_nao, 0x88[31])
    do
    {
        u4Response = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_WRFIFO_RESPONSE);
        u4TimeCnt --;
        mcDELAY_US(1);
    }while((u4Response ==0) &&(u4TimeCnt>0));

    if(u4TimeCnt==0)//time out
    {
        mcSHOW_DBG_MSG(("[DramcMPC_FIFO] Resp fail (time out)\n"));
        mcFPRINTF((fp_A60501, "[DramcMPC_FIFO] Resp fail (time out)\n"));
        //return DRAM_FAIL;
    }

    //R_DMWRFIFOEN, 0x0C[31]=0
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PERFCTL0), 0, PERFCTL0_WRFIFOEN);



	//Issue MPC RD FIFO
    //R_DMRDFIFOEN, 0x0C[30]=1 for RD FIFO
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PERFCTL0), 1, PERFCTL0_RDFIFOEN);

	//Wait wrfifo_response=1 (dramc_conf_nao, 0x88[31])
    do
    {
        u4Response = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_RDFIFO_RESPONSE);
        u4TimeCnt --;
        mcDELAY_US(1);
    }while((u4Response ==0) &&(u4TimeCnt>0));

    if(u4TimeCnt==0)//time out
    {
        mcSHOW_DBG_MSG(("[DramcMPC_FIFO] Resp fail (time out)\n"));
        mcFPRINTF((fp_A60501, "[DramcMPC_FIFO] Resp fail (time out)\n"));
        //return DRAM_FAIL;
    }
	
    //Then read RDFIFO compare result (dramc_conf_nao, 0x124)
    //Must do WRFIO first, then do RDFIFO, then compare it.
    u4Result = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CMP_ERR));
	mcSHOW_DBG_MSG(("[DramcMPC_FIFO] Read RDFIFO compare result: 0x%x\n", u4Result));

    //R_DMRDFIFOEN, 0x0C[30]=0
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PERFCTL0), 0, PERFCTL0_RDFIFOEN);

	//Restore registers
	vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_LBWDAT3), u4BakReg_DRAMC_REG_LBWDAT3);
	vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_LBWDAT2), u4BakReg_DRAMC_REG_LBWDAT2);
	vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_LBWDAT1), u4BakReg_DRAMC_REG_LBWDAT1);
	vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_LBWDAT0), u4BakReg_DRAMC_REG_LBWDAT0);

	vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0), u4BakReg_DRAMC_REG_REFCTRL0);
	vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), u4BakReg_DRAMC_REG_SPCMDCTRL);
	vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU_SCINTV), u4BakReg_DRAMC_REG_SHU_SCINTV);
	vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_PERFCTL0), u4BakReg_DRAMC_REG_PERFCTL0);

}
#endif

#ifdef ENABLE_POST_PACKAGE_REPAIR
void DramcPostPackageRepair(DRAMC_CTX_T *p, DRAM_INFO_BY_MRR_T *DramInfo)
{
    U32 u4Response=0;
    U32 u4TimeCnt= TIME_OUT_CNT;
	U16 u2Value=0;
	U16 u2FailRow=0x01ff;
	U8  ucFailBK=0;
	
	mcSHOW_DBG_MSG(("[DramcPostPackageRepair]\n"
                    "\n\tFreq=%d, CH=%d, Rank=%d\n", p->frequency, p->channel, p->rank));
	mcFPRINTF((fp_A60501, "[DramcPostPackageRepair]"
                          "\n\tFreq=%d, CH=%d, Rank=%d\n", p->frequency, p->channel, p->rank));
	
	//MR25 contains one bit of readout per bank indicating that at least one resource is available for Post Package Repair programming.
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1GetRank(p), MRS_MRRRK);
	DramcModeRegRead(p, 25, &u2Value);
	mcSHOW_DBG_MSG(("Before PostPackageRepair, MR25 = 0x%x\n", u2Value & 0xFF));
	mcFPRINTF((fp_A60501, "Before PostPackageRepair, MR25 = 0x%x\n", u2Value & 0xFF));

	//1.DRAMC DCM freerun, 
	//R_DMDCMEN2(dramc conf AO 0x38[1])=0
	vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 0, DRAMC_PD_CTRL_DCMEN2);

	//2.PHY DCM freerun, 
	//R_DMPHYCLKDYNGEN(dramc conf AO 0x38[30])=0
	vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 0, DRAMC_PD_CTRL_PHYCLKDYNGEN);

	//3.Dram clock freerun, 
	//R_DMMIOCKCTRLOFF(dramc	conf AO 0x38[26])=1
	vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 1, DRAMC_PD_CTRL_MIOCKCTRLOFF);
    
	//4.Fix CKE0 and CKE1 be high, 
	//R_DMCKEFIXON(dramc conf AO 0x24[6], CKECTRL.CKEFIXON)=1
	//R_DMCKE1FIXON(dramc conf AO 0x24[4], CKECTRL.CKE1FIXON)=1
	CKEFixOnOff(p, CKE_FIXON, CKE_WRITE_TO_ALL_CHANNEL);

	//Step 0: disable refresh for PPR
	//Let R_DMREFDIS=1
    vAutoRefreshSwitch(p, DISABLE);

	//Step 1: before enter PPR mode, all banks must be precharged 
	//Set R_DMPREAEN=1
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1GetRank(p), MRS_MRSRK);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMCTRL), 1, DRAMCTRL_DPDRK_OPT);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_PREAEN);
	mcDELAY_US(1);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1, SPCMD_PREAEN);

	//wait dramc_conf_nao.prea_response=1
	u4TimeCnt= TIME_OUT_CNT;
	u4Response = 0;
    do
    {
        u4Response = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_PREA_RESPONSE);
        u4TimeCnt --;
        mcDELAY_US(1);
    }while((u4Response ==0) &&(u4TimeCnt>0));
	
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_PREAEN);

    if(u4TimeCnt==0)//time out
    {
        mcSHOW_DBG_MSG(("dramc_conf_nao.prea_response fail (time out)\n"));
        mcFPRINTF((fp_A60501, "dramc_conf_nao.prea_response fail (time out)\n"));
        //return DRAM_FAIL;
    }
	
	//Step 2: enter PPR mode
    if(DramInfo->u2MR5VendorID == 0x6) //Skhynix Manufacturer ID
    {
	//Skhynix DRAM PPR Enable Sequence
	//Assumption: MR3 OP<2> - LOW
	//Skhynix 2z 6Gb/4Gb LPDDR4x PPR Guard key
	//PPR Guard Key, MR9 with following OP codes : B8 - E8 - 98 - BF - EF - 9F - B9 - E9 - 99 - 84 - A2 - 81
	//Skhynix 2z 8Gb LPDDR4x PPR Guard key
	//PPR Guard Key, MR9 with following OP codes : CD - AD -FD -C9 -A9 -F9 -C7 -A7 -F7
	DramcModeRegWrite(p, 0x9, 0xB8);
	DramcModeRegWrite(p, 0x9, 0xE8);
	DramcModeRegWrite(p, 0x9, 0x98);
	DramcModeRegWrite(p, 0x9, 0xBF);
	DramcModeRegWrite(p, 0x9, 0xEF);
	DramcModeRegWrite(p, 0x9, 0x9F);
	DramcModeRegWrite(p, 0x9, 0xB9);
	DramcModeRegWrite(p, 0x9, 0xE9);
	DramcModeRegWrite(p, 0x9, 0x99);
	DramcModeRegWrite(p, 0x9, 0x84);
	DramcModeRegWrite(p, 0x9, 0xA2);
	DramcModeRegWrite(p, 0x9, 0x81);
    }

	//Set MR4[4]=1: PPR entry
	DramcModeRegWrite(p, 0x4, 0x10); 

	//Step 3: wait tMRD 
	//mcDELAY_US(1000);
	mcDELAY_MS(1000);

	//Step 4: issue ACT command with fail row address
	//Set R_DMACTEN_ROW, R_DMACTEN_BK, then set R_DMACTEN, 
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MPC_OPTION), 1, MPC_OPTION_MPCRKEN);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1GetRank(p), MRS_MPCRK);
    
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PPR_CTRL), u2FailRow, PPR_CTRL_ACTEN_ROW);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PPR_CTRL), ucFailBK, PPR_CTRL_ACTEN_BK);
	mcSHOW_DBG_MSG(("PPR, Fail Row = %d,  Fail Bank = %d\n", u2FailRow, ucFailBK));
	mcFPRINTF((fp_A60501, "PPR, Fail Row = %d,  Fail Bank = %d\n", u2FailRow, ucFailBK));

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_ACTEN);
	mcDELAY_US(1);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1, SPCMD_ACTEN);

	//wait dramc_conf_nao.act_response=1
	u4TimeCnt= TIME_OUT_CNT;
	u4Response=0;
    do
    {
        u4Response = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_ACT_RESPONSE);
        u4TimeCnt --;
        mcDELAY_US(1);
    }while((u4Response ==0) &&(u4TimeCnt>0));

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_ACTEN);

    if(u4TimeCnt==0)//time out
    {
        mcSHOW_DBG_MSG(("dramc_conf_nao.act_response fail (time out)\n"));
        mcFPRINTF((fp_A60501, "dramc_conf_nao.act_response fail (time out)\n"));
        //return DRAM_FAIL;
    }

	//Step 5: wait tPGM to allow DRAM repair internally
	mcDELAY_MS(3000);
	
	//Step 6: issue PRE 
	//Set R_DMPREAEN=1
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_PREAEN);
	mcDELAY_US(1);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1, SPCMD_PREAEN);
	
	//wait dramc_conf_nao.prea_response=1
	u4TimeCnt= TIME_OUT_CNT;
	u4Response=0;
    do
    {
        u4Response = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_PREA_RESPONSE);
        u4TimeCnt --;
        mcDELAY_US(1);
    }while((u4Response ==0) &&(u4TimeCnt>0));

    if(u4TimeCnt==0)//time out
    {
        mcSHOW_DBG_MSG(("dramc_conf_nao.prea_response fail (time out)\n"));
        mcFPRINTF((fp_A60501, "dramc_conf_nao.prea_response fail (time out)\n"));
        //return DRAM_FAIL;
    }
	
 	//Step 7: wait tPGM_Exit 
	//mcDELAY_US(1000);
	mcDELAY_MS(1000);

	//Step 8: exit PPR
	//Set MR4[4]=0: PPR exit
	DramcModeRegWrite(p, 0x4, 0x0); 

 	//Step 9: wait tPGMPST, them dram is ready for any valid command
	//mcDELAY_US(1000);
	mcDELAY_MS(1000);

	//MR25 contains one bit of readout per bank indicating that at least one resource is available for Post Package Repair programming.
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1GetRank(p), MRS_MRRRK);
	DramcModeRegRead(p, 25, &u2Value);
	mcSHOW_DBG_MSG(("After PostPackageRepair, MR25 = 0x%x\n", u2Value & 0xFF));

	mcFPRINTF((fp_A60501, "After PostPackageRepair, MR25 = 0x%x\n", u2Value & 0xFF));

}
#endif

static void SetRxDqDqsDelay(DRAMC_CTX_T *p, S16 iDelay)
{
    U8 ii, u1ByteIdx;
    U32 u4value;
    U8 dl_value[8];

    if (iDelay <=0)
    {
        if(u1IsLP4Family(p->dram_type))
        {
            // Set DQS delay
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ6), \
                                    P_Fld((-iDelay+gu2RX_DQS_Duty_Offset[0][0]),SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0) |P_Fld((-iDelay+gu2RX_DQS_Duty_Offset[0][1]),SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_F_DLY_B0));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ6), \
                            P_Fld((-iDelay+gu2RX_DQS_Duty_Offset[1][0]),SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1) |P_Fld((-iDelay+gu2RX_DQS_Duty_Offset[1][1]),SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_F_DLY_B1));

            // Adjust DQM output delay.
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ6), \
                                    P_Fld(0,SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0) |P_Fld(0,SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_F_DLY_B0));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ6), \
                            P_Fld(0,SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1) |P_Fld(0,SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_F_DLY_B1));
        }
#if ENABLE_LP3_SW
        else
        {
            for (u1ByteIdx=0; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
            {
                vIO32WriteFldAlign_Phy_Byte(u1ByteIdx, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ6), -iDelay, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0);
                vIO32WriteFldAlign_Phy_Byte(u1ByteIdx, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ6), -iDelay, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_F_DLY_B0);
            }
        }
#endif

        DramPhyReset(p);
    }
    else
    {
        // Set DQS delay
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ6), \
                                P_Fld((0+gu2RX_DQS_Duty_Offset[0][0]),SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0) |P_Fld((0+gu2RX_DQS_Duty_Offset[0][1]),SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_F_DLY_B0));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ6), \
                        P_Fld((0+gu2RX_DQS_Duty_Offset[1][0]),SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1) |P_Fld((0+gu2RX_DQS_Duty_Offset[1][1]),SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_F_DLY_B1));


        // Adjust DQM output delay.
        if(u1IsLP4Family(p->dram_type))
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ6), \
                                    P_Fld(iDelay,SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0) |P_Fld(iDelay,SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_F_DLY_B0));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ6), \
                            P_Fld(iDelay,SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1) |P_Fld(iDelay,SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_F_DLY_B1));
        }
#if ENABLE_LP3_SW
        else
        {
            for (u1ByteIdx=0; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
            {
                Set_RX_DQM_DelayLine_Phy_Byte(p, u1ByteIdx, iDelay);
            }
        }
#endif

        DramPhyReset(p);


        // Adjust DQ output delay.
        u4value = ((U32) iDelay) | (((U32)iDelay)<<8) | (((U32)iDelay)<<16) | (((U32)iDelay)<<24);

        for (ii=0; ii<4; ii++)
        {
            if(u1IsLP4Family(p->dram_type))
            {
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ2+ ii*4), u4value);//DQ0~DQ7
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ2+ ii*4), u4value);//DQ8~DQ15
            }
#if ENABLE_LP3_SW
            else//LPDDR3
            {

                //every 2bit dq have the same delay register address
                for (u1ByteIdx=0; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
                {
                    dl_value[0] = iDelay;
                    dl_value[1] = iDelay;
                    dl_value[2] = iDelay;
                    dl_value[3] = iDelay;
                    dl_value[4] = iDelay;
                    dl_value[5] = iDelay;
                    dl_value[6] = iDelay;
                    dl_value[7] = iDelay;

                    Set_RX_DQ_DelayLine_Phy_Byte(p, u1ByteIdx, dl_value);
                }
            }
#endif
        }
    }
}



#if SIMULATION_RX_PERBIT
DRAM_STATUS_T DramcRxWindowPerbitCal(DRAMC_CTX_T *p, U8 u1UseTestEngine)
{
    U8 ii, u1BitIdx, u1ByteIdx;
    U8 ucbit_first, ucbit_last;
    S16 iDelay=0, u4DelayBegin=0, u4DelayEnd, u4DelayStep=1;
    S16 iDutyOffset, u4DutyOffsetBegin, u4DutyOffsetEnd, u4DutyOffsetStep=4;
    U32 uiFinishCount;
    U32 u4value, u4err_value, u4fail_bit;
    PASS_WIN_DATA_T WinPerBit[DQ_DATA_WIDTH], FinalWinPerBit[DQ_DATA_WIDTH];
    S32 iDQSDlyPerbyte[DQS_NUMBER], iDQMDlyPerbyte[DQS_NUMBER];//, iFinalDQSDly[DQS_NUMBER];
    U8 u1VrefScanEnable;
    U16 u2VrefLevel, u2TempWinSum, u2TmpDQMSum, u2FinalVref=0xe;
    U16 u2VrefBegin, u2VrefEnd, u2VrefStep;
    U32 u4fail_bit_R, u4fail_bit_F;
    U32 u4AddrOfst = 0x50;

    U16 u2FinalRXPerbitWin_index;
    static U16 u2FinalRXPerbitWin[10][DQ_DATA_WIDTH];
    static S16 s2Final_DQS_Duty_Offset[10][DQ_DATA_WIDTH];
    U16 u2min_rx_value[4], u2min_RX_DQ_bit[4]={0xff,0xff,0xff,0xff}, u2max_rx_value[4];

    U16 u2WinSize;
    #if ENABLE_CLAIBTAION_WINDOW_LOG_FOR_FT
    U16 u2MinWinSize = 0xffff;
    U8 u1MinWinSizeBitidx;
    #endif
    #if SUPPORT_HYNIX_RX_DQS_WEAK_PULL
    U8 u1Hynix_rx_scan_enable=0;
    #endif

    #if SUPPORT_PASS_WINDOW_SPEED_UP
    U8 u1AllBitPassCount, u1SkipThisVref;
    #endif

    U8 dl_value[8]={0,0,0,0,0,0,0,0};
    U8 backup_rank;

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
   S16 u1minfirst_pass=0xff,u1minlast_pass=0xff,u4Delayoffset;
   U8  u1AllBitPassCount;
  // U8  fBypass_RDDQC,fBypass_RXWINDOW;
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

 
    // 1.delay DQ ,find the pass widnow (left boundary).
    // 2.delay DQS find the pass window (right boundary).
    // 3.Find the best DQ / DQS to satify the middle value of the overall pass window per bit
    // 4.Set DQS delay to the max per byte, delay DQ to de-skew
#if VENDER_JV_LOG
    if(u1UseTestEngine ==1)
        vPrintCalibrationBasicInfo_ForJV(p);
#else
    vPrintCalibrationBasicInfo(p);
#endif
    mcSHOW_DBG_MSG2(("Start DQ dly to find pass range UseTestEngine =%d\n", u1UseTestEngine));
    mcSHOW_DBG_MSG2(("x-axis: bit #, y-axis: DQ dly (%d~%d)\n", (-MAX_RX_DQSDLY_TAPS), MAX_RX_DQDLY_TAPS));

    mcFPRINTF((fp_A60501, "Start RX DQ/DQS calibration UseTestEngine =%d\n", u1UseTestEngine));
    mcFPRINTF((fp_A60501, "x-axis is bit #; y-axis is DQ delay\n"));

    // for loop, different Vref,
    u2rx_window_sum = 0;

#if VREF_SPEED_UP_LP4
    if((u1IsLP4Family(p->dram_type) && (u1UseTestEngine==1) && (p->enable_rx_scan_vref==ENABLE) && (p->rank==RANK_0)) //only apply RX Vref Scan for Rank 0 (Rank 0 and 1 use the same Vref reg)
#else
    if((u1IsLP4Family(p->dram_type) && (u1UseTestEngine==1) && (p->enable_rx_scan_vref==ENABLE)) //apply RX Vref Scan for Rank 0 and 1
#endif
     #if SUPPORT_SAVE_OFF_LINE_CALIBRATION
        && (gRX_Vref_Scan_Check[p->channel][p->rank][p->dram_fsp]==1) && (p->pOffLineSaveData->u2RXWinSum_Save[p->channel][p->rank][p->dram_fsp]>0)
    #endif
    )
        u1VrefScanEnable =1;
    else
        u1VrefScanEnable =0;

    #if SIMPLIFY_VREF_SCAN
    if (u1IsLP4Family(p->dram_type) && (p->skipVrefScan == ENABLE) && (u1VrefScanEnable == 1))
    {
        u1VrefScanEnable =0;
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ5), gFinalRXVrefDQ[p->channel], SHU1_B0_DQ5_RG_RX_ARDQ_VREF_SEL_B0);  // LP4 and LP4x with term: 0xe
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DQ5), gFinalRXVrefDQ[p->channel], SHU1_B1_DQ5_RG_RX_ARDQ_VREF_SEL_B1);  // LP4 and LP4x with term: 0xe
        mcSHOW_DBG_MSG(("Skip RX Vref(dq) scan. Setting: %d\n", gFinalRXVrefDQ[p->channel]));
        mcFPRINTF((fp_A60501, "Skip RX Vref(dq) scan. Setting: %d\n", gFinalRXVrefDQ[p->channel]));

        #if VENDER_JV_LOG
        mcSHOW_DBG_MSG5(("Skip RX Vref(dq) scan. Setting: %d\n", gFinalRXVrefDQ[p->channel]));
        #endif
    }
#endif /* SIMPLIFY_VREF_SCAN */

    mcSHOW_DBG_MSG(("RX Vref Scan = %d\n", u1VrefScanEnable));
    mcFPRINTF((fp_A60501, "RX Vref Scan= %d\n", u1VrefScanEnable));

    if(u1VrefScanEnable)
    {
        #if (SW_CHANGE_FOR_SIMULATION ||FOR_DV_SIMULATION_USED)
        u2VrefBegin =RX_VREF_RANGE_BEGIN;
        #else
        if(p->odt_onoff)
        {
            u2VrefBegin =RX_VREF_RANGE_BEGIN;
        }
        else
        {
            #if VREF_SPEED_UP_LP4
            u2VrefBegin = 0x18;
            #else
            u2VrefBegin = 0;//Lewis@20160817: Enlarge RX Vref range for eye scan
            #endif
        }
        #endif
        
        u2VrefEnd =RX_VREF_RANGE_END;
        #if ( SUPPORT_SAVE_TIME_FOR_CALIBRATION &&BYPASS_VREF_CAL)
	if(p->pSavetimeData->femmc_Ready==1 && ((((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x04)>>2)==0 && (u1UseTestEngine ==0)) ||(((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x20)>>5)==0 && (u1UseTestEngine ==1)) ))
	{
       		u2VrefBegin = p->pSavetimeData->u1RxWinPerbitVref_Save[p->channel][_MappingFreqArray[p->freq_sel]];
       		u2VrefEnd=u2VrefBegin+1;
	}
	#endif
        u2VrefStep=RX_VREF_RANGE_STEP;
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ5), 1, B0_DQ5_RG_RX_ARDQ_VREF_EN_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ5), 1, B1_DQ5_RG_RX_ARDQ_VREF_EN_B1);
    }
    else //LPDDR3 or diable RX Vref
    {
        u2VrefBegin = 0;
        u2VrefEnd = 1; // SEL[4:0] = 01110
        u2VrefStep =1; //don't care, just make for loop break;
    }

    if(u1IsLP4Family(p->dram_type))
    {
       #if 0 //SUPPORT_SAVE_TIME_FOR_CALIBRATION
	if(p->pSavetimeData->femmc_Ready==1 && ((((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x04)>>2)==0 && (u1UseTestEngine ==0)) ||(((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x20)>>5)==0 && (u1UseTestEngine ==1)) ))
	{
          u4DelayBegin=p->pSavetimeData->u1RxWinPerbit_Min_FirstPass_Save[p->channel][p->rank][p->freq_sel]-p->pSavetimeData->u1RxWinPerbitoffset;
          u4Delayoffset=p->pSavetimeData->u1RxWinPerbit_Min_LastPass_Save[p->channel][p->rank][p->freq_sel]-p->pSavetimeData->u1RxWinPerbitoffset;

	}
        else
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
            u4DelayBegin= -MAX_RX_DQSDLY_TAPS;
        }
      }
        u4DelayEnd = MAX_RX_DQDLY_TAPS;

        if(p->frequency <850)
            u4DelayStep =2;  //1600
        else
            u4DelayStep =1;//2667, 3200
    }
#if ENABLE_LP3_SW
    else
    {
        if(p->frequency >=933)
        {
            u4DelayBegin= -48;
        }
        else if(p->frequency >=600)
        {
            u4DelayBegin= -70;
        }
        else if(p->frequency >=450)
        {
            u4DelayBegin= -96;
        }
        else
        {
            u4DelayBegin= -MAX_RX_DQSDLY_TAPS;
        }
        u4DelayEnd = MAX_RX_DQDLY_TAPS;

        if(p->frequency <600)
            u4DelayStep =2;  //1066
        else
            u4DelayStep =1;//1600, 1270
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


#if SUPPORT_HYNIX_RX_DQS_WEAK_PULL
    if (u1IsLP4Family(p->dram_type) && (p->vendor_id == VENDOR_HYNIX) && u1UseTestEngine==0)
    {
        u4DutyOffsetBegin = 0;
        u4DutyOffsetEnd = 4;
        u4DutyOffsetStep = 1;

        for(u2FinalRXPerbitWin_index=0; u2FinalRXPerbitWin_index<10; u2FinalRXPerbitWin_index++)
        for(ii=0; ii<DQ_DATA_WIDTH; ii++)
        {
            u2FinalRXPerbitWin[u2FinalRXPerbitWin_index][ii] = 0x7fff;
            s2Final_DQS_Duty_Offset[u2FinalRXPerbitWin_index][ii] = 0;
        }

        u2FinalRXPerbitWin_index = 0;
        u1Hynix_rx_scan_enable =1;

        mcSHOW_DBG_MSG(("HYNIX RX DQS R/F Scan :ON\n"));
        mcFPRINTF((fp_A60501, "HYNIX RX DQS R/F Scan :ON\n"));
    }
#if 0
    else if (u1IsLP4Family(p->dram_type) && (p->vendor_id == VENDOR_HYNIX) && u1UseTestEngine==1)
    {
        u4DutyOffsetBegin = 0;
        u4DutyOffsetEnd = 0;
        u4DutyOffsetStep = 1;

        mcSHOW_DBG_MSG(("HYNIX RX DQS R/F Scan :OFF\n"));
        mcFPRINTF((fp_A60501, "HYNIX RX DQS R/F Scan :OFF\n"));
    }
#endif
    else
    {
        u4DutyOffsetBegin = 0;
        u4DutyOffsetEnd = 0;
        u4DutyOffsetStep = 1;

        mcSHOW_DBG_MSG(("RX DQS R/F Scan : Disable\n"));
        mcFPRINTF((fp_A60501, "RX DQS R/F Scan :Disable\n"));
    }
#else
        u4DutyOffsetBegin = 0;
        u4DutyOffsetEnd = 0;
        u4DutyOffsetStep = 1;
#endif


    #if !REDUCE_LOG_FOR_PRELOADER
    mcSHOW_DBG_MSG(("\nRX DQS R/F Scan, iDutyOffset= %d\n", iDutyOffset));
    mcFPRINTF((fp_A60501, "\nRX DQS R/F Scan, iDutyOffset= %d\n", iDutyOffset));
    #endif

    for(u2VrefLevel = u2VrefBegin; u2VrefLevel < u2VrefEnd; u2VrefLevel += u2VrefStep)
    {
        if(u1VrefScanEnable ==1)
        {
            #if !REDUCE_LOG_FOR_PRELOADER
            mcSHOW_DBG_MSG(("\n\tRX VrefLevel=%d\n", u2VrefLevel));
            mcFPRINTF((fp_A60501, "\n\tRX VrefLevel=%d\n", u2VrefLevel));
            #endif
            #if VENDER_JV_LOG
            if(u1UseTestEngine ==1)
            mcSHOW_DBG_MSG5(("\n\tRX VrefLevel=%d\n", u2VrefLevel));
            #endif

            //Set RX Vref Here
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ5), u2VrefLevel, SHU1_B0_DQ5_RG_RX_ARDQ_VREF_SEL_B0);  // LP4 and LP4x with term: 0xe
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DQ5), u2VrefLevel, SHU1_B1_DQ5_RG_RX_ARDQ_VREF_SEL_B1);  // LP4 and LP4x with term: 0xe
        }


        for (iDutyOffset=u4DutyOffsetBegin; iDutyOffset<=u4DutyOffsetEnd; iDutyOffset+=u4DutyOffsetStep)
        {
            // initialize parameters
            u2TempWinSum =0;
            uiFinishCount =0;

            for (u1BitIdx = 0; u1BitIdx < p->data_width; u1BitIdx++)
            {
                WinPerBit[u1BitIdx].first_pass = (S16)PASS_RANGE_NA;
                WinPerBit[u1BitIdx].last_pass = (S16)PASS_RANGE_NA;
                FinalWinPerBit[u1BitIdx].first_pass = (S16)PASS_RANGE_NA;
                FinalWinPerBit[u1BitIdx].last_pass = (S16)PASS_RANGE_NA;
            }

            // Adjust DQM output delay to 0
            if(u1IsLP4Family(p->dram_type))
            {
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ6), \
                                        P_Fld(0, SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0) |P_Fld(0, SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_F_DLY_B0));
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ6), \
                                P_Fld(0,SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1) |P_Fld(0, SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_F_DLY_B1));
            }
            #if ENABLE_LP3_SW
            else
            {
                for (u1ByteIdx=0; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
                {
                    Set_RX_DQM_DelayLine_Phy_Byte(p, u1ByteIdx, 0);
                }
            }
            #endif

            // Adjust DQ output delay to 0
            //every 2bit dq have the same delay register address
            for (ii=0; ii<4; ii++)
            {
                if(u1IsLP4Family(p->dram_type))
                {
                    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ2+ ii*4), 0);//DQ0~DQ7
                    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ2+ ii*4), 0);//DQ8~DQ15
                }
            #if ENABLE_LP3_SW
                else//LPDDR3
                {
                    for (u1ByteIdx=0; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
                    {
                        Set_RX_DQ_DelayLine_Phy_Byte(p, u1ByteIdx, dl_value);
                    }
                }
            #endif
            }

            #if SUPPORT_PASS_WINDOW_SPEED_UP
            u1AllBitPassCount =0;
            u1SkipThisVref =0;
            #endif

        #if 0 //SUPPORT_SAVE_TIME_FOR_CALIBRATION
	if(p->pSavetimeData->femmc_Ready==1 && ((((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x04)>>2)==0 && (u1UseTestEngine ==0)) ||(((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x20)>>5)==0 && (u1UseTestEngine ==1)) ))
	{
            u1AllBitPassCount =0;
        }
        #endif
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
if(p->pSavetimeData->femmc_Ready==1 && (p->pSavetimeData->Bypass_RDDQC[_MappingFreqArray[p->freq_sel]] || p->pSavetimeData->Bypass_RXWINDOW[_MappingFreqArray[p->freq_sel]] ))
{
        for (u1BitIdx=0; u1BitIdx<16; u1BitIdx++)
        	{
        	FinalWinPerBit[u1BitIdx].first_pass=p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][u1BitIdx];
        	FinalWinPerBit[u1BitIdx].last_pass=p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][u1BitIdx];
        	}
                    if(u1UseTestEngine)
                        vSetCalibrationResult(p, DRAM_CALIBRATION_RX_PERBIT, DRAM_OK);
                    else
                        vSetCalibrationResult(p, DRAM_CALIBRATION_RX_RDDQC, DRAM_OK);
}
else
#endif
{	
            for (iDelay=u4DelayBegin; iDelay<=u4DelayEnd; iDelay+= u4DelayStep)
            {
                    #if SUPPORT_HYNIX_RX_DQS_WEAK_PULL
                    if (u1Hynix_rx_scan_enable)
                    {
                        if (iDutyOffset<0)
                        {
                            gu2RX_DQS_Duty_Offset[0][0]=-iDutyOffset;
                            gu2RX_DQS_Duty_Offset[0][1]=0;
                            gu2RX_DQS_Duty_Offset[1][0]=-iDutyOffset;
                            gu2RX_DQS_Duty_Offset[1][1]=0;
                        }
                        else
                        {
                            gu2RX_DQS_Duty_Offset[0][0]=0;
                            gu2RX_DQS_Duty_Offset[0][1]=iDutyOffset;
                            gu2RX_DQS_Duty_Offset[1][0]=0;
                            gu2RX_DQS_Duty_Offset[1][1]=iDutyOffset;
                        }
                    }
#endif

                SetRxDqDqsDelay(p, iDelay);

                if(u1UseTestEngine)
                {
                    u4err_value = DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, p->test_pattern);
                }
                else
                {
                    u4err_value = DramcRxWinRDDQCRun(p);
                }

                #if VREF_SPEED_UP_LP4
                if(u1VrefScanEnable==0)
                #endif
                {
                    #if SUPPORT_HYNIX_RX_DQS_WEAK_PULL
                    if(u1Hynix_rx_scan_enable==0)
                    #endif
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
                }

                #if SUPPORT_PASS_WINDOW_SPEED_UP
                if(u1IsLP4Family(p->dram_type))
                {
                    if(u1AllBitPassCount == 4 &&  u4err_value!=0)
                    {
                        u1SkipThisVref =1;
                        mcSHOW_DBG_MSG(("SUPPORT_PASS_WINDOW_SPEED_UP RX Overjump\n"));
                        break;
                    }

                    if(u4err_value ==0) // all bit pass
                        u1AllBitPassCount ++;

                    if(u1AllBitPassCount == 3)
                    {
                        mcSHOW_DBG_MSG(("SUPPORT_PASS_WINDOW_SPEED_UP RX Delay %d -> ",iDelay));
                        if(p->freqGroup == 800)
                        {
                            iDelay += 33;
                        }
                        else if (p->freqGroup == 1333 || p->freqGroup == 1200)
                        {
                            iDelay += 18;
                        }
                        else
                        {
                            iDelay += 10;
                        }

                        u1AllBitPassCount ++;
                        mcSHOW_DBG_MSG(("%d\n ",iDelay));
                    }

                }
                #endif

          #if 0 //SUPPORT_SAVE_TIME_FOR_CALIBRATION
	if(p->pSavetimeData->femmc_Ready==1 && ((((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x04)>>2)==0 && (u1UseTestEngine ==0)) ||(((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x20)>>5)==0 && (u1UseTestEngine ==1)) ))
	   {
                if(u1IsLP4Family(p->dram_type))
                {        
                     if(u4err_value ==0) // all bit pass
                        u1AllBitPassCount ++;
                   		if(u1AllBitPassCount == 3 &&  u4err_value==0 )
                   		{
                        		iDelay=u4Delayoffset ;
                        	}

                }
            }
            #endif
                // check fail bit ,0 ok ,others fail
                for (u1BitIdx = 0; u1BitIdx < p->data_width; u1BitIdx++)
                {
                    u4fail_bit = u4err_value&((U32)1<<u1BitIdx);

                    if(WinPerBit[u1BitIdx].first_pass== PASS_RANGE_NA)
                    {
                        if(u4fail_bit==0) //compare correct: pass
                        {
                            WinPerBit[u1BitIdx].first_pass = iDelay;
                        }
                    }
                    else if(WinPerBit[u1BitIdx].last_pass == PASS_RANGE_NA)
                    {
                        //mcSHOW_DBG_MSG(("fb%d \n", u4fail_bit));

                        if(u4fail_bit !=0) //compare error : fail
                        {
                            WinPerBit[u1BitIdx].last_pass  = (iDelay-1);
                        }
                        else if (iDelay==u4DelayEnd)
                        {
                            WinPerBit[u1BitIdx].last_pass  = iDelay;
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
                                FinalWinPerBit[u1BitIdx].last_pass = WinPerBit[u1BitIdx].last_pass;
                            }
                            //reset tmp window
                            WinPerBit[u1BitIdx].first_pass = PASS_RANGE_NA;
                            WinPerBit[u1BitIdx].last_pass = PASS_RANGE_NA;
                        }
                    }

                    #if VREF_SPEED_UP_LP4
                    if(u1VrefScanEnable==0)
                    #endif
                    {
                    #if SUPPORT_HYNIX_RX_DQS_WEAK_PULL
                    if(u1Hynix_rx_scan_enable==0)
                    #endif
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
                            }
                            else
                            {
                                {
                                    mcSHOW_DBG_MSG2(("x"));
                                    mcFPRINTF((fp_A60501, "x"));
                                }
                            }
                        }
                    }
                }
                }

                #if VREF_SPEED_UP_LP4
                if(u1VrefScanEnable==0)
                #endif
                {
                    #ifdef ETT_PRINT_FORMAT
                    if(u4err_value !=0)
                    #endif
                    {
                        #if SUPPORT_HYNIX_RX_DQS_WEAK_PULL
                        if(u1Hynix_rx_scan_enable==0)
                        #endif
                        {
                            mcSHOW_DBG_MSG2((" [MSB]\n"));
                            mcFPRINTF((fp_A60501, " [MSB]\n"));
                        }
                    }
                }

                //if all bits widnow found and all bits turns to fail again, early break;
                if((u1IsLP4Family(p->dram_type) &&(uiFinishCount == 0xffff)) || \
                    ((p->dram_type == TYPE_LPDDR3) &&(uiFinishCount == 0xffffffff)))
                {
                    if(u1UseTestEngine)
                        vSetCalibrationResult(p, DRAM_CALIBRATION_RX_PERBIT, DRAM_OK);
                    else
                        vSetCalibrationResult(p, DRAM_CALIBRATION_RX_RDDQC, DRAM_OK);

                    #if !VREF_SPEED_UP_LP4
                    if(u1VrefScanEnable ==0)
                    #endif
                    {
                        if((u1IsLP4Family(p->dram_type) &&((u4err_value&0xffff) == 0xffff)) ||
                            ((p->dram_type == TYPE_LPDDR3) &&(u4err_value == 0xffffffff)))
                            {
                                    #if !REDUCE_LOG_FOR_PRELOADER
                                    mcSHOW_DBG_MSG(("\nRX all bits window found, early break!\n"));
                                    #endif
                                    break;  //early break
                            }
                    }
                }
            }

}
            for (u1BitIdx = 0; u1BitIdx < p->data_width; u1BitIdx++)
            {
                FinalWinPerBit[u1BitIdx].win_size = FinalWinPerBit[u1BitIdx].last_pass - FinalWinPerBit[u1BitIdx].first_pass + 1;
                u2TempWinSum += FinalWinPerBit[u1BitIdx].win_size;  //Sum of CA Windows for vref selection

                #if ENABLE_CLAIBTAION_WINDOW_LOG_FOR_FT
                if(FinalWinPerBit[u1BitIdx].win_size<u2MinWinSize)
                {
                    u2MinWinSize = FinalWinPerBit[u1BitIdx].win_size;
                    u1MinWinSizeBitidx = u1BitIdx;
                }
                #endif

                #if VENDER_JV_LOG
                if(u1UseTestEngine ==1)
                {
                    U8 shuffleIdx;
                    shuffleIdx = get_HQA_shuffleIdx(p);
                    mcSHOW_DBG_MSG5(("RX Bit%d, %d%%\n", u1BitIdx,  ((FinalWinPerBit[u1BitIdx].win_size*u2gdelay_cell_ps_all[shuffleIdx][0]*p->frequency*2)+(1000000-1))/1000000));
                }
                #endif
            }

            #if ENABLE_CLAIBTAION_WINDOW_LOG_FOR_FT
            mcSHOW_DBG_MSG2(("FT log: RX min window : bit %d, size %d\n", u1MinWinSizeBitidx, u2MinWinSize));
            #endif
            #if !REDUCE_LOG_FOR_PRELOADER
            mcSHOW_DBG_MSG2(("\nRX Vref %d, Window Sum %d > %d\n", u2VrefLevel, u2TempWinSum, u2rx_window_sum));
            mcFPRINTF((fp_A60501, "\nRX Vref %d, Window Sum %d > %d\n", u2VrefLevel, u2TempWinSum, u2rx_window_sum));
            #endif

            #if 0
            if((u1IsLP4Family(p->dram_type) &&(u2TempWinSum <250)) || \
                ((p->dram_type == TYPE_LPDDR3) &&(u2TempWinSum <1100)))
            {
                    mcSHOW_DBG_MSG2(("\n\n[NOTICE] CH %d, TestEngine %d, RX_sum %d\n", p->channel, u1UseTestEngine, u2TempWinSum));
                    mcFPRINTF((fp_A60501, "\n\n[NOTICE] CH %d, TestEngine %d, RX_sum %d\n", p->channel, u1UseTestEngine, u2TempWinSum));
            }
            #endif

#if SUPPORT_HYNIX_RX_DQS_WEAK_PULL
            if((u2TempWinSum >u2rx_window_sum || u1Hynix_rx_scan_enable)
#else
            if((u2TempWinSum >u2rx_window_sum)
#endif
            #if SUPPORT_PASS_WINDOW_SPEED_UP
                && (!u1SkipThisVref)
#endif
            )
            {
                mcSHOW_DBG_MSG3(("\nBetter RX Vref found %d, Window Sum %d > %d\n", u2VrefLevel, u2TempWinSum, u2rx_window_sum));
                mcFPRINTF((fp_A60501, "\nBetter RX Vref found %d, Window Sum %d > %d\n", u2VrefLevel, u2TempWinSum, u2rx_window_sum));

                u2rx_window_sum =u2TempWinSum;
                u2FinalVref = u2VrefLevel;

                for (u1BitIdx=0; u1BitIdx<p->data_width; u1BitIdx++)
                {
                    FinalWinPerBit[u1BitIdx].win_center = (FinalWinPerBit[u1BitIdx].last_pass + FinalWinPerBit[u1BitIdx].first_pass)>>1;     // window center of each DQ bit

                    #if VREF_SPEED_UP_LP4
                    if(u1VrefScanEnable ==0)
                    #endif
                    {
                            #ifdef ETT_PRINT_FORMAT
                                mcSHOW_DBG_MSG(("iDelay=%d, Bit %d, Center %d (%d ~ %d) %d\n", iDelay, u1BitIdx, FinalWinPerBit[u1BitIdx].win_center, FinalWinPerBit[u1BitIdx].first_pass, FinalWinPerBit[u1BitIdx].last_pass, FinalWinPerBit[u1BitIdx].win_size));
                            #else
                                mcSHOW_DBG_MSG(("iDelay=%d, Bit %2d, Center %3d (%4d ~ %4d) %d\n", iDelay, u1BitIdx, FinalWinPerBit[u1BitIdx].win_center, FinalWinPerBit[u1BitIdx].first_pass, FinalWinPerBit[u1BitIdx].last_pass, FinalWinPerBit[u1BitIdx].win_size));
                            #endif
                                mcFPRINTF((fp_A60501, "iDelay=%d, Bit %2d, Center %3d (%4d ~ %4d)\n", iDelay, u1BitIdx, FinalWinPerBit[u1BitIdx].win_center, FinalWinPerBit[u1BitIdx].first_pass, FinalWinPerBit[u1BitIdx].last_pass, FinalWinPerBit[u1BitIdx].win_size));
                    }

			#if 0 //SUPPORT_SAVE_TIME_FOR_CALIBRATION
 			   	 if(p->pSavetimeData->femmc_Ready==1 )
				{
				
				      if((u1VrefScanEnable==0) && (p->freq_sel>2 && FinalWinPerBit[u1BitIdx].win_size< p->pSavetimeData->RXWindowTHLowSpeed) ||(p->freq_sel<3 && FinalWinPerBit[u1BitIdx].win_size< p->pSavetimeData->RXWindowTHHighSpeed))
 			   		 {
				   		if( (!u1UseTestEngine ) &&  ((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x04)>>2)==0)
	   			   		{
                      				p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]]=1<<2; //RDDQC fail                      				
                               			 mcSHOW_DBG_MSG(("RDDQC fail %d win_size =%d\n", p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]],FinalWinPerBit[u1BitIdx].win_size));
	   			   		}
				   		else if( (u1UseTestEngine ) &&  ((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x20)>>5)==0)
	   			   		{
                      				p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]]=1<<5; //RXWindow fail
                                			mcSHOW_DBG_MSG(("RXwindow fail %d win_size =%d\n", p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]],FinalWinPerBit[u1BitIdx].win_size));
	   			   		}
				   	} 			   		 
				
				}
			
                        if(p->pSavetimeData->femmc_Ready==0 || (((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x04)>>2)==1 && (u1UseTestEngine ==0)) ||(((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x20)>>5)==1 && (u1UseTestEngine ==1)) )
	                  {
 			    if(!u1UseTestEngine )
 			    {
				if(u1minfirst_pass>FinalWinPerBit[u1BitIdx].first_pass)//min_first_pass
				{	
					u1minfirst_pass=FinalWinPerBit[u1BitIdx].first_pass;
				}
				if(u1minlast_pass>FinalWinPerBit[u1BitIdx].last_pass)//min_last_pass
				{
	                        u1minlast_pass=FinalWinPerBit[u1BitIdx].last_pass;
				}
				p->pSavetimeData->u1RxWinPerbit_Min_FirstPass_Save[p->channel][p->rank][p->freq_sel]=u1minfirst_pass;
	                     p->pSavetimeData->u1RxWinPerbit_Min_LastPass_Save[p->channel][p->rank][p->freq_sel]=u1minlast_pass;
 			    }
				//mcSHOW_DBG_MSG(("save RXWindow\n"));
                          }
			#endif
			
#if SUPPORT_HYNIX_RX_DQS_WEAK_PULL
                    if (u1Hynix_rx_scan_enable)
                        if ((FinalWinPerBit[u1BitIdx].last_pass - FinalWinPerBit[u1BitIdx].first_pass + 1) < u2FinalRXPerbitWin[u2FinalRXPerbitWin_index][u1BitIdx])
                        {
                            u2FinalRXPerbitWin[u2FinalRXPerbitWin_index][u1BitIdx] = FinalWinPerBit[u1BitIdx].last_pass - FinalWinPerBit[u1BitIdx].first_pass + 1;
                            s2Final_DQS_Duty_Offset[u2FinalRXPerbitWin_index][u1BitIdx] = iDutyOffset;
                        }
#endif

#ifdef FOR_HQA_TEST_USED
                    if(u1UseTestEngine ==1)
                    {
                        gFinalRXPerbitWin[p->channel][p->rank][u1BitIdx] = FinalWinPerBit[u1BitIdx].win_size;
                    }
#endif
                }
#if SUPPORT_HYNIX_RX_DQS_WEAK_PULL
                    if (u1Hynix_rx_scan_enable) u2FinalRXPerbitWin_index++;
#endif
            }
        }
            #if VREF_SPEED_UP_LP4
            if(u2TempWinSum < (u2rx_window_sum*95/100))
            {
                mcSHOW_DBG_MSG(("\nRX Vref found, early break!\n"));
                u2VrefLevel = u2VrefEnd;
                break;//max vref found, early break;
            }
            #endif
        }

    if(u1UseTestEngine)
    {
        DramcEngine2End(p);
    }
    else
    {
        DramcRxWinRDDQCEnd(p);
    }

    //Set RX Final Vref Here
    if(u1VrefScanEnable==1)
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ5), u2FinalVref, SHU1_B0_DQ5_RG_RX_ARDQ_VREF_SEL_B0);  // LP4 and LP4x with term: 0xe
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DQ5), u2FinalVref, SHU1_B1_DQ5_RG_RX_ARDQ_VREF_SEL_B1);  // LP4 and LP4x with term: 0xe

        mcSHOW_DBG_MSG(("\nFinal RX Vref %d, apply to both rank0 and 1\n", u2FinalVref));
        mcFPRINTF((fp_A60501, "\nFinal RX Vref %d, apply to both rank0 and 1\n", u2FinalVref));
#if VENDER_JV_LOG
        mcSHOW_DBG_MSG5(("\nFinal RX Vref %d, apply to both rank0 and 1\n", u2FinalVref));
#endif

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
 //if(p->pSavetimeData->femmc_Ready==1 )
//{
				
  //              for (u1BitIdx=0; u1BitIdx<p->data_width; u1BitIdx++)
    //            {
//			if((p->freq_sel>2 && FinalWinPerBit[u1BitIdx].win_size< p->pSavetimeData->RXWindowTHLowSpeed) ||(p->freq_sel<3 && FinalWinPerBit[u1BitIdx].win_size< p->pSavetimeData->RXWindowTHHighSpeed))
 //			{
//				if( (u1UseTestEngine ) &&  ((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x20)>>5)==0)
//	   			{
  //                  			 p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]]=1<<5; //RXWindow fail
    //                			 mcSHOW_DBG_MSG(("RXwindow fail %d win_size =%d\n", p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]],FinalWinPerBit[u1BitIdx].win_size));
//	   			}
//			} 		
  //              }
				
//}
if(p->pSavetimeData->femmc_Ready==0 || (((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x04)>>2)==1 && (u1UseTestEngine ==0)) ||(((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x20)>>5)==1 && (u1UseTestEngine ==1)) )
{
	p->pSavetimeData->u1RxWinPerbitVref_Save[p->channel][_MappingFreqArray[p->freq_sel]]=u2FinalVref;
//mcSHOW_DBG_MSG(("save RXVref\n"));
}
#endif

        // When only calibrate RX Vref for Rank 0, apply the same value for Rank 1.
        gFinalRXVrefDQ[p->channel]= u2FinalVref;
    }

#if SIMPLIFY_VREF_SCAN
        if(u1IsLP4Family(p->dram_type) && (u1UseTestEngine==1) && (p->enable_rx_scan_vref==ENABLE) && (p->rank==RANK_0)  && (p->skipVrefScan == DISABLE))
#endif
        {
    #if SUPPORT_SAVE_OFF_LINE_CALIBRATION
            // First TX Vref Scan, need to check window sum
            mcSHOW_DBG_MSG(("\n[DramcRxWindowPerbitCal] SUPPORT_SAVE_OFF_LINE_CALIBRATION  re-calibrate vref "));
            if(gRX_Vref_Scan_Check[p->channel][p->rank][p->dram_fsp] ==0)
            {
                if(u2rx_window_sum < p->pOffLineSaveData->u2RXWinSum_Save[p->channel][p->rank][p->dram_fsp] *9/10)
                {
                    // Check Window sum, need to re-calibrate Vref
                    gRX_Vref_Scan_Check[p->channel][p->rank][p->dram_fsp] =1;
                    mcSHOW_DBG_MSG(("required\n"));

                    return DRAM_K_VREF;
                }
                else
                {
                    // Check Window sum, "NO" need to re-calibrate Vref
                    mcSHOW_DBG_MSG(("not required\n"));
                }

                mcSHOW_DBG_MSG(("rx_window_sum %d, RXWinSum_Save*0.9=%d\n",
                                    u2rx_window_sum,
                                    p->pOffLineSaveData->u2RXWinSum_Save[p->channel][p->rank][p->dram_fsp] * 9 / 10));
            }
            else
            {
                // re-calibrate Vref done, update new Vref and Window sum
                p->pOffLineSaveData->u1RXVref_Save[p->channel][p->rank][p->dram_fsp] = u2FinalVref;
                p->pOffLineSaveData->u2RXWinSum_Save[p->channel][p->rank][p->dram_fsp] = u2rx_window_sum;
                mcSHOW_DBG_MSG(("done\n"
                                "\nRxVref_Save %d, RXWinSum_Save %d\n",
                                                p->pOffLineSaveData->u1RXVref_Save[p->channel][p->rank][p->dram_fsp],
                                                p->pOffLineSaveData->u2RXWinSum_Save[p->channel][p->rank][p->dram_fsp]));
            }
    #endif
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
                iDQSDlyPerbyte[u1ByteIdx] = FinalWinPerBit[u1BitIdx].win_center;

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
        }

        // calculate DQM as average of 8 DQ delay
        iDQMDlyPerbyte[u1ByteIdx] = u2TmpDQMSum/DQS_BIT_NUMBER;
    }

    vPrintCalibrationBasicInfo(p);

    #ifdef ETT_PRINT_FORMAT
    if(u1IsLP4Family(p->dram_type))
    {
        mcSHOW_DBG_MSG(("DQS Delay:\nDQS0 = %d, DQS1 = %d\n"
                        "DQM Delay:\nDQM0 = %d, DQM1 = %d\n",
                            iDQSDlyPerbyte[0], iDQSDlyPerbyte[1],
                            iDQMDlyPerbyte[0], iDQMDlyPerbyte[1]));
    }
#if ENABLE_LP3_SW
    else
    {
        mcSHOW_DBG_MSG(("DQS Delay:\nDQS0 = %d, DQS1 = %d, DQS2 = %d, DQS3 = %d\n"
                        "DQM Delay:\nDQM0 = %d, DQM1 = %d, DQM2 = %d, DQM3 = %d\n",
                            iDQSDlyPerbyte[0], iDQSDlyPerbyte[1], iDQSDlyPerbyte[2], iDQSDlyPerbyte[3],
                            iDQMDlyPerbyte[0], iDQMDlyPerbyte[1], iDQMDlyPerbyte[2], iDQMDlyPerbyte[3]));
    }
#endif /* ENABLE_LP3_SW */
    #else
    if(u1IsLP4Family(p->dram_type))
    {
        mcSHOW_DBG_MSG(("DQS Delay:\nDQS0 = %2d, DQS1 = %2d\n"
                        "DQM Delay:\nDQM0 = %2d, DQM1 = %2d\n",
                            iDQSDlyPerbyte[0], iDQSDlyPerbyte[1],
                            iDQMDlyPerbyte[0], iDQMDlyPerbyte[1]));
    }
#if ENABLE_LP3_SW
    else
    {
        mcSHOW_DBG_MSG(("DQS Delay:\nDQS0 = %2d, DQS1 = %2d, DQS2 = %2d, DQS3 = %2d\n"
                        "DQM Delay:\nDQM0 = %2d, DQM1 = %2d, DQM2 = %2d, DQM3 = %2d\n",
                        iDQSDlyPerbyte[0], iDQSDlyPerbyte[1], iDQSDlyPerbyte[2], iDQSDlyPerbyte[3],
                        iDQMDlyPerbyte[0], iDQMDlyPerbyte[1], iDQMDlyPerbyte[2], iDQMDlyPerbyte[3]));
    }
#endif /* ENABLE_LP3_SW */
    #endif
    mcSHOW_DBG_MSG(("DQ Delay:\n"));

    mcFPRINTF((fp_A60501, "\tdramc_rxdqs_perbit_swcal\n"));
    mcFPRINTF((fp_A60501, "\tchannel=%d(1:cha) \n", p->channel));
    mcFPRINTF((fp_A60501, "\tbus width=%d\n\n", p->data_width));

    if(u1IsLP4Family(p->dram_type))
    {
        mcFPRINTF((fp_A60501, "DQS Delay:\n DQS0 = %2d DQS1 = %2d\n", iDQSDlyPerbyte[0], iDQSDlyPerbyte[1]));
        mcFPRINTF((fp_A60501, "DQM Delay:\n DQM0 = %2d DQM1 = %2d\n", iDQMDlyPerbyte[0], iDQMDlyPerbyte[1]));
    }
#if ENABLE_LP3_SW
    else
    {
        mcFPRINTF((fp_A60501, "DQS Delay:\n DQS0 = %2d DQS1 = %2d DQS2 = %2d DQS3 = %2d\n", iDQSDlyPerbyte[0], iDQSDlyPerbyte[1], iDQSDlyPerbyte[2], iDQSDlyPerbyte[3]));
        mcFPRINTF((fp_A60501, "DQM Delay:\n DQM0 = %2d DQM1 = %2d DQM2 = %2d DQM3 = %2d\n", iDQMDlyPerbyte[0], iDQMDlyPerbyte[1], iDQMDlyPerbyte[2], iDQMDlyPerbyte[3]));
    }
#endif /* ENABLE_LP3_SW */
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
    ShuffleRegCheck =1;
    #endif

#if SUPPORT_HYNIX_RX_DQS_WEAK_PULL
    if (u1Hynix_rx_scan_enable)
    {
        S16 s2Final_Value[4]={0,0,0,0};

        u2min_rx_value[0]=u2min_rx_value[1]=u2min_rx_value[2]=u2min_rx_value[3] = 0x7fff;

        // find min rx value's DQ bit
        for (ii=0; ii<u2FinalRXPerbitWin_index; ii++)
        {
            for (u1ByteIdx=0; u1ByteIdx<p->data_width/DQS_NUMBER; u1ByteIdx++)
            {
                for (u1BitIdx=u1ByteIdx*8; u1BitIdx<((u1ByteIdx+1)*8); u1BitIdx++)
                {
                    if (u2FinalRXPerbitWin[ii][u1BitIdx] < u2min_rx_value[u1ByteIdx])
                    {
                        u2min_rx_value[u1ByteIdx] = u2FinalRXPerbitWin[ii][u1BitIdx];
                        u2min_RX_DQ_bit[u1ByteIdx] = u1BitIdx;
                    }
                }
            }
        }
        mcSHOW_DBG_MSG(("Find min\t[0]\t[1]\n"
                        "min_rx_value\t%d\t%d\n"
                        "min_RX_DQ_bit\t%d\t%d\n",
                            u2min_rx_value[0], u2min_rx_value[1],
                            u2min_RX_DQ_bit[0], u2min_RX_DQ_bit[1]));

        // find this DQ bit's max winsize's DQS duty offset as final DQS duty offset value
        u2max_rx_value[0]=u2max_rx_value[1]=u2max_rx_value[2]=u2max_rx_value[3] = 0;
        for (u1ByteIdx=0; u1ByteIdx<p->data_width/DQS_NUMBER; u1ByteIdx++)
        {
            for (ii=0; ii<u2FinalRXPerbitWin_index; ii++)
            {
                if (u2FinalRXPerbitWin[ii][u2min_RX_DQ_bit[u1ByteIdx]] > u2max_rx_value[u1ByteIdx])
                {
                    u2max_rx_value[u1ByteIdx] = u2FinalRXPerbitWin[ii][u2min_RX_DQ_bit[u1ByteIdx]];

                    s2Final_Value[u1ByteIdx] = s2Final_DQS_Duty_Offset[ii][u2min_RX_DQ_bit[u1ByteIdx]];
                }
            }
        }
        mcSHOW_DBG_MSG(("Find\t\t[0]\t[1]\n"
                        "max_rx_value\t\t%d\t%d\n"
                        "final_dqs_duty_offset\t\t%d\t%d\n",
                            u2max_rx_value[0], u2max_rx_value[1],
                            s2Final_Value[0], s2Final_Value[1]));

        for (u1ByteIdx=0; u1ByteIdx<p->data_width/DQS_NUMBER; u1ByteIdx++)
        {
            if (s2Final_Value[u1ByteIdx] < 0)
            {
                gu2RX_DQS_Duty_Offset[u1ByteIdx][0] = -s2Final_Value[u1ByteIdx];
                gu2RX_DQS_Duty_Offset[u1ByteIdx][1] = 0;
            }
            else
            {
                gu2RX_DQS_Duty_Offset[u1ByteIdx][0] = 0;
                gu2RX_DQS_Duty_Offset[u1ByteIdx][1] = s2Final_Value[u1ByteIdx];
            }
        }
    }
    mcSHOW_DBG_MSG(("Final Duty Offset\n"
                    "DQS0\t%d\t%d\n"
                    "DQS1\t%d\t%d\n",
                        gu2RX_DQS_Duty_Offset[0][0], gu2RX_DQS_Duty_Offset[0][1],
                        gu2RX_DQS_Duty_Offset[1][0], gu2RX_DQS_Duty_Offset[1][1]));
#else
    gu2RX_DQS_Duty_Offset[0][0]=gu2RX_DQS_Duty_Offset[0][1]=gu2RX_DQS_Duty_Offset[1][0]=gu2RX_DQS_Duty_Offset[1][1] = 0;
#endif


    // set dqs delay, (dqm delay)
    for (u1ByteIdx = 0; u1ByteIdx < (p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
    {
        // Set DQS & DQM delay
        if(u1IsLP4Family(p->dram_type))
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ6+ u4AddrOfst*u1ByteIdx), \
                P_Fld(((U32)iDQSDlyPerbyte[u1ByteIdx]+gu2RX_DQS_Duty_Offset[u1ByteIdx][0]),SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0) |P_Fld(((U32)iDQSDlyPerbyte[u1ByteIdx]+gu2RX_DQS_Duty_Offset[u1ByteIdx][1]),SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_F_DLY_B0) |
                P_Fld(((U32)iDQMDlyPerbyte[u1ByteIdx]),SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0) |P_Fld(((U32)iDQMDlyPerbyte[u1ByteIdx]),SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_F_DLY_B0));
        }
#if ENABLE_LP3_SW
        else
        {
            /* p->rank = RANK_0, save to Reg Rank0 and Rank1, p->rank = RANK_1, save to Reg Rank1 */
            for(ii=p->rank; ii<RANK_MAX; ii++)
            {
                vSetRank(p,ii);

                vIO32WriteFldAlign_Phy_Byte(u1ByteIdx, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ6), (U32)iDQSDlyPerbyte[u1ByteIdx], SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0);
                vIO32WriteFldAlign_Phy_Byte(u1ByteIdx, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ6), (U32)iDQSDlyPerbyte[u1ByteIdx], SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_F_DLY_B0);

                Set_RX_DQM_DelayLine_Phy_Byte(p, u1ByteIdx,(U32)iDQMDlyPerbyte[u1ByteIdx]);
            }
            vSetRank(p, backup_rank);
        }
#endif
    }

    #if REG_SHUFFLE_REG_CHECK
    ShuffleRegCheck =0;
    #endif

    DramPhyReset(p);

    #if REG_SHUFFLE_REG_CHECK
    ShuffleRegCheck =1;
    #endif

    // set dq delay
    if(u1IsLP4Family(p->dram_type))
    {
        for (u1BitIdx=0; u1BitIdx<DQS_BIT_NUMBER; u1BitIdx+=2)
        {
             vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ2+u1BitIdx*2), \
                                            P_Fld(((U32)FinalWinPerBit[u1BitIdx].best_dqdly),SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_R_DLY_B0) |\
                                            P_Fld(((U32)FinalWinPerBit[u1BitIdx].best_dqdly),SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_F_DLY_B0)| \
                                            P_Fld(((U32)FinalWinPerBit[u1BitIdx+1].best_dqdly),SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_R_DLY_B0) |\
                                            P_Fld(((U32)FinalWinPerBit[u1BitIdx+1].best_dqdly),SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_F_DLY_B0));

             vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ2+u1BitIdx*2), \
                                            P_Fld(((U32)FinalWinPerBit[u1BitIdx+8].best_dqdly),SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_R_DLY_B1) | \
                                            P_Fld((U32)FinalWinPerBit[u1BitIdx+8].best_dqdly,SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_F_DLY_B1)| \
                                            P_Fld(((U32)FinalWinPerBit[u1BitIdx+9].best_dqdly),SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_R_DLY_B1) |\
                                            P_Fld((U32)FinalWinPerBit[u1BitIdx+9].best_dqdly,SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_F_DLY_B1));

            //mcSHOW_DBG_MSG(("u1BitId %d  Addr 0x%2x = %2d %2d %2d %2d \n", u1BitIdx, DRAMC_REG_ADDR(DDRPHY_RXDQ1+u1BitIdx*2), \
            //                FinalWinPerBit[u1BitIdx].best_dqdly, FinalWinPerBit[u1BitIdx+1].best_dqdly,  FinalWinPerBit[u1BitIdx+8].best_dqdly, FinalWinPerBit[u1BitIdx+9].best_dqdly));
        }
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    if(p->pSavetimeData->femmc_Ready==0 || (((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x04)>>2)==1 && (u1UseTestEngine ==0)) ||(((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x20)>>5)==1 && (u1UseTestEngine ==1)) )
    	{
        for (u1BitIdx=0; u1BitIdx<16; u1BitIdx++)
        	{
         	p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][u1BitIdx]=(U32)FinalWinPerBit[u1BitIdx].first_pass;
        	p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][u1BitIdx]=(U32)FinalWinPerBit[u1BitIdx].last_pass;

      	}
    	}
    #endif

    }
#if ENABLE_LP3_SW
    else //LPDDR3
    {
        /* p->rank = RANK_0, save to Reg Rank0 and Rank1, p->rank = RANK_1, save to Reg Rank1 */
        for(ii=p->rank; ii<RANK_MAX; ii++)
        {
            vSetRank(p,ii);

            //every 2bit dq have the same delay register address
            for (u1ByteIdx=0; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
            {
                    u1BitIdx = u1ByteIdx *DQS_BIT_NUMBER;

                    dl_value[0] = FinalWinPerBit[u1BitIdx].best_dqdly;
                    dl_value[1] = FinalWinPerBit[u1BitIdx+1].best_dqdly;
                    dl_value[2] = FinalWinPerBit[u1BitIdx+2].best_dqdly;
                    dl_value[3] = FinalWinPerBit[u1BitIdx+3].best_dqdly;
                    dl_value[4] = FinalWinPerBit[u1BitIdx+4].best_dqdly;
                    dl_value[5] = FinalWinPerBit[u1BitIdx+5].best_dqdly;
                    dl_value[6] = FinalWinPerBit[u1BitIdx+6].best_dqdly;
                    dl_value[7] = FinalWinPerBit[u1BitIdx+7].best_dqdly;

                 Set_RX_DQ_DelayLine_Phy_Byte(p, u1ByteIdx, dl_value);
            }
        }
        vSetRank(p, backup_rank);
    }
#endif

    #if REG_SHUFFLE_REG_CHECK
    ShuffleRegCheck =0;
    #endif
    mcSHOW_DBG_MSG3(("[DramcRxWindowPerbitCal] Done\n"));
    mcFPRINTF((fp_A60501, "[DramcRxWindowPerbitCal] Done\n"));

#ifdef DRAM_CALIB_LOG
    if (u1UseTestEngine)
    {
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].RxWindowPerbitCal.DQS0_delay = iDQSDlyPerbyte[0];
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].RxWindowPerbitCal.DQS1_delay = iDQSDlyPerbyte[1];
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].RxWindowPerbitCal.DQS2_delay = iDQSDlyPerbyte[2];
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].RxWindowPerbitCal.DQS3_delay = iDQSDlyPerbyte[3];
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].RxWindowPerbitCal.DQM0_delay = iDQMDlyPerbyte[0];
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].RxWindowPerbitCal.DQM1_delay = iDQMDlyPerbyte[1];
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].RxWindowPerbitCal.DQM2_delay = iDQMDlyPerbyte[2];
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].RxWindowPerbitCal.DQM3_delay = iDQMDlyPerbyte[3];

        for (u1BitIdx = 0; u1BitIdx < p->data_width; u1BitIdx=u1BitIdx+4)
        {
            gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].RxWindowPerbitCal.DQ_delay[u1BitIdx] = FinalWinPerBit[u1BitIdx].best_dqdly;
        }
    }
    else
    {
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].RDDQC_Cali.DQS0_delay = iDQSDlyPerbyte[0];
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].RDDQC_Cali.DQS1_delay = iDQSDlyPerbyte[1];
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].RDDQC_Cali.DQS2_delay = iDQSDlyPerbyte[2];
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].RDDQC_Cali.DQS3_delay = iDQSDlyPerbyte[3];
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].RDDQC_Cali.DQM0_delay = iDQMDlyPerbyte[0];
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].RDDQC_Cali.DQM1_delay = iDQMDlyPerbyte[1];
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].RDDQC_Cali.DQM2_delay = iDQMDlyPerbyte[2];
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].RDDQC_Cali.DQM3_delay = iDQMDlyPerbyte[3];

        for (u1BitIdx = 0; u1BitIdx < p->data_width; u1BitIdx=u1BitIdx+4)
        {
            gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].RDDQC_Cali.DQ_delay[u1BitIdx] = FinalWinPerBit[u1BitIdx].best_dqdly;
        }
    }
#endif

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
static void dle_factor_handler(DRAMC_CTX_T *p, U8 curr_val, U8 pip_num)
{
    U8 u1DLECG_OptionStartEXT2=0, u1DLECG_OptionStartEXT3=0;
    U8 u1DLECG_OptionLastEXT2=0, u1DLECG_OptionLastEXT3=0;

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

    // Datlat_dsel = datlat -1, only 1 TX pipe
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_CONF1),
                                        P_Fld(curr_val, SHU_CONF1_DATLAT) |
                                        P_Fld(curr_val -2, SHU_CONF1_DATLAT_DSEL) |
                                        P_Fld(curr_val -2, SHU_CONF1_DATLAT_DSEL_PHY));
    
    if(p->frequency>=1333)
    {
        u1DLECG_OptionStartEXT2=1;
    }
    
    if(curr_val>=24)
    {
        u1DLECG_OptionLastEXT2 = u1DLECG_OptionLastEXT3 = 1;
    }    
    else if(curr_val>=18)
    {
        u1DLECG_OptionLastEXT2 = 1;
    }
        
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_PIPE), P_Fld(0x1, SHU_PIPE_READ_START_EXTEND1)
                | P_Fld(0x1, SHU_PIPE_DLE_LAST_EXTEND1)
                | P_Fld(u1DLECG_OptionStartEXT2, SHU_PIPE_READ_START_EXTEND2)
                | P_Fld(u1DLECG_OptionLastEXT2, SHU_PIPE_DLE_LAST_EXTEND2)
                | P_Fld(u1DLECG_OptionStartEXT3, SHU_PIPE_READ_START_EXTEND3)
                | P_Fld(u1DLECG_OptionLastEXT3, SHU_PIPE_DLE_LAST_EXTEND3));
                
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
    U32 u4AddrOfst = 0x50;
    U32 u4RegBak_DDRPHY_RXDQ_RK0[DQS_NUMBER][5];  // rank 0
    U8 dl_value[8]={0,0,0,0,0,0,0,0};

    // reg backup
#if 0
        for (ii=0; ii<5; ii++)
        {
            if(u1IsLP4Family(p->dram_type))
            {
                u4RegBak_DDRPHY_RXDQ_RK0[0][ii] =(u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ2+ii*4), PHY_FLD_FULL)); // rank 0, byte 0
                u4RegBak_DDRPHY_RXDQ_RK0[1][ii] =(u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ2+ii*4), PHY_FLD_FULL)); // rank 0, byte 1
            }
#if ENABLE_LP3_SW
            else //LPDDR3 , only record rank 0, will not modify rank1
            {
#if 0 //LP3 no need to backup DQ delay line reg
                for(u1ByteIdx=0; u1ByteIdx<4; u1ByteIdx++)
                {
                    u4RegBak_DDRPHY_RXDQ_RK0[u1ByteIdx][ii] =u4IO32ReadFldAlign_Phy_Byte(u1ByteIdx, DRAMC_REG_ADDR(DDRPHY_SHU2_R0_B0_DQ2+ii*4), PHY_FLD_FULL);
                }
#endif
            }
#endif
        }
#endif

        // Adjust DQM output delay to 0
        if(u1IsLP4Family(p->dram_type))
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ6), \
                                P_Fld(0,SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0) |P_Fld(0, SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_F_DLY_B0));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ6), \
                                P_Fld(0,SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1) |P_Fld(0, SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_F_DLY_B1));
        }
#if ENABLE_LP3_SW
        else
        {
            for(u1ByteIdx=0; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
            {
                Set_RX_DQM_DelayLine_Phy_Byte(p, u1ByteIdx, 0);
            }
        }
#endif

        // Adjust DQ output delay to 0
        //every 2bit dq have the same delay register address
        for (ii=0; ii<4; ii++)
        {
            if(u1IsLP4Family(p->dram_type))
            {
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ2+ii*4), 0);  //DQ0~DQ7
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ2+ii*4), 0);//DQ8~DQ15
            }
#if ENABLE_LP3_SW
            else //LPDDR3
            {

                for (u1ByteIdx=0; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
                {
                    Set_RX_DQ_DelayLine_Phy_Byte(p, u1ByteIdx, dl_value);
                }
            }
#endif
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

           if(u1IsLP4Family(p->dram_type))
           {
               SetRxDqDqsDelay(p, iDelay);
            }
#if ENABLE_LP3_SW
           else
            {
                SetRxDqDqsDelay(p, iDelay);
            }
#endif

           if(u1UseTestEngine)
           {
               u4err_value = DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, p->test_pattern);
           }
           else
           {
               u4err_value = DramcRxWinRDDQC(p);
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
            if(u1IsLP4Family(p->dram_type))
            {
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ2+ ii*4), u4RegBak_DDRPHY_RXDQ_RK0[0][ii] , PHY_FLD_FULL);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ2+ ii*4), u4RegBak_DDRPHY_RXDQ_RK0[1][ii] , PHY_FLD_FULL);
            }
#if ENABLE_LP3_SW
            else //LPDDR3
             {
#if 0 //LP3 no need to restore backup DQ delay line reg
                for(u1ByteIdx=0; u1ByteIdx<4; u1ByteIdx++)
                {
                     vIO32WriteFldAlign_Phy_Byte(u1ByteIdx, DRAMC_REG_ADDR(DDRPHY_SHU2_R0_B0_DQ2+ ii*4), u4RegBak_DDRPHY_RXDQ_RK0[u1ByteIdx][ii] , PHY_FLD_FULL);
                }
#endif
            }
#endif
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
        #if 0 //SUPPORT_SAVE_TIME_FOR_CALIBRATION
	if(p->pSavetimeData->femmc_Ready==1 && ((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x010)>>4)==0)
	{
		u2DatlatBegin=(p->pSavetimeData->u1RxDatlat_Save[p->channel][p->rank][p->freq_sel]-p->pSavetimeData->u1RxDatlatoffset);
	}
	else
	#endif
	{
#if (FOR_DV_SIMULATION_USED==0)
		u2DatlatBegin=7;
#else
		u2DatlatBegin=0;
#endif
	}

#if (SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_DATLAT)
	if(p->pSavetimeData->femmc_Ready==1 && ((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x010)>>4)==0)
  		{
             ucbest_step = p->pSavetimeData->u1RxDatlat_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]];		

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
#if ENABLE_LP3_SW
        else  //if(use_rxtx_scan == fcDATLAT_USE_RX_SCAN)//LPDDR3, LP4 Should not enter if datlat calibration is after RDDQC
        {
            u4err_value = Dramc_ta2_rx_scan(p, 1);
        }
#endif

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
    #if 0 //SUPPORT_SAVE_TIME_FOR_CALIBRATION
    	if(p->pSavetimeData->femmc_Ready==1 && ((ii-u2DatlatBegin)==4) && ((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x010)>>4)==0)
	{
          ucbest_step = ucfirst + 1;
    	}
    	else
	#endif
	{
        ucbest_step = ucfirst + 2;
    }
    }
}
    aru1RxDatlatResult[p->channel][p->rank] = ucbest_step;

   #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
	if(p->pSavetimeData->femmc_Ready==0 || ((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x010)>>4)==1)
        {
   	p->pSavetimeData->u1RxDatlat_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]]=ucbest_step;
        //mcSHOW_DBG_MSG(("save DATLAT\n"));
        }
   #endif
    mcSHOW_DBG_MSG(("pattern=%d first_step=%d total pass=%d best_step=%d\n", p->test_pattern, ucfirst, ucsum, ucbest_step));
    mcFPRINTF((fp_A60501, "pattern=%d first_step=%d total pass=%d best_step=%d\n", p->test_pattern, ucfirst, ucsum, ucbest_step));

#ifdef DRAM_CALIB_LOG
    gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].DATLAT.best_step = ucbest_step;
#endif

#if (SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_DATLAT)
	if(p->pSavetimeData->femmc_Ready==1 && ((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x010)>>4)==0)
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
       #if 0 //SUPPORT_SAVE_TIME_FOR_CALIBRATION
	   if(p->pSavetimeData->femmc_Ready==1 &&  ((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x10)>>4)==0)
	   {
                   p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]]=1<<4; //DATLAT fail
	   }
       #endif       
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

    if(u1Datlat0> u1Datlat1)
    {
        u1FinalDatlat= u1Datlat0;
    }
    else
    {
        u1FinalDatlat= u1Datlat1;
    }

    #if ENABLE_READ_DBI
    if(p->DBI_R_onoff[p->dram_fsp])
    {
      u1FinalDatlat++;
    }
    #endif
    
    dle_factor_handler(p, u1FinalDatlat, 3);
    mcSHOW_DBG_MSG(("[DualRankRxdatlatCal] RK0: %d, RK1: %d, Final_Datlat %d\n", u1Datlat0, u1Datlat1, u1FinalDatlat));
    mcFPRINTF((fp_A60501, "[DualRankRxdatlatCal] RK0: %d, RK1: %d, Final_Datlat %d\n", u1Datlat0, u1Datlat1, u1FinalDatlat));

#ifdef DRAM_CALIB_LOG
    gDRAM_CALIB_LOG.CHANNEL[p->channel].DualRankRxdatlatCal.RANK0 = u1Datlat0;
    gDRAM_CALIB_LOG.CHANNEL[p->channel].DualRankRxdatlatCal.RANK1 = u1Datlat1;
    gDRAM_CALIB_LOG.CHANNEL[p->channel].DualRankRxdatlatCal.Final_Datlat = u1FinalDatlat;
#endif

    return DRAM_OK;

}
#endif //SIMULATION_DATLAT

#if SIMULATION_TX_PERBIT
//-------------------------------------------------------------------------
/** DramcTxWindowPerbitCal (v2)
 *  TX DQS per bit SW calibration.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param  apply           (U8): 0 don't apply the register we set  1 apply the register we set ,default don't apply.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL
 */
//-------------------------------------------------------------------------
#define ENABLE_64_PI_TO_UI 1
#if ENABLE_64_PI_TO_UI
#define TX_DQ_UI_TO_PI_TAP         64 // 1 PI = tCK/64, total 128 PI, 1UI = 32 PI
#else
#define TX_DQ_UI_TO_PI_TAP         32 // 1 PI = tCK/64, total 128 PI, 1UI = 32 PI
#endif

#if (SW_CHANGE_FOR_SIMULATION ||FOR_DV_SIMULATION_USED)
#define TX_VREF_RANGE_BEGIN       0
#define TX_VREF_RANGE_END           2 // binary 110010
#define TX_VREF_RANGE_STEP         2
#else
#define TX_VREF_RANGE_BEGIN       0
#define TX_VREF_RANGE_END           50 // binary 110010
#define TX_VREF_RANGE_STEP         2
#endif

static void TxWinTransferDelayToUIPI(DRAMC_CTX_T *p, U16 uiDelay, U8 u1UISmallBak, U8 u1UILargeBak, U8* pu1UILarge, U8* pu1UISmall, U8* pu1PI)
{
    U8 u1Small_ui_to_large;
    U16 u2TmpValue;

    //in LP4, 8 small UI =  1 large UI
    //in LP3, 4 small UI =  1 large UI
    if(u1IsLP4Family(p->dram_type))
        u1Small_ui_to_large =  3;
    else //LPDDR3
        u1Small_ui_to_large =  2;

    #if 0
    *pu1PI = uiDelay% TX_DQ_UI_TO_PI_TAP;
    #else
    *pu1PI = uiDelay & (TX_DQ_UI_TO_PI_TAP-1);
    #endif

    #if ENABLE_64_PI_TO_UI
    u2TmpValue = (uiDelay /TX_DQ_UI_TO_PI_TAP)*2 +u1UISmallBak;
    #else
    u2TmpValue = uiDelay /TX_DQ_UI_TO_PI_TAP +u1UISmallBak;
    #endif

    #if 0
    *pu1UISmall = u2TmpValue % u1Small_ui_to_large;
    *pu1UILarge = u2TmpValue / u1Small_ui_to_large +u1UILargeBak;
    #else
    *pu1UISmall = u2TmpValue - ((u2TmpValue >> u1Small_ui_to_large) <<u1Small_ui_to_large);
    *pu1UILarge = (u2TmpValue >> u1Small_ui_to_large) +u1UILargeBak;
    #endif
}

#if ENABLE_TX_TRACKING
static void TxWinAdjustPIToCenter(DRAMC_CTX_T *p, U8* pu1UILarge, U8* pu1UISmall, U8* pu1UILargeOEN, U8* pu1UISmallOEN, U8* pu1PI)
{
#if ENABLE_64_PI_TO_UI
    U8 u1Small_ui_to_large;
    U16 u2TmpValue;
    U16 u2TmpValueOEN;

    //in LP4, 8 small UI =  1 large UI
    //in LP3, 4 small UI =  1 large UI
    if(u1IsLP4Family(p->dram_type))
        u1Small_ui_to_large =  3;
    else //LPDDR3
        u1Small_ui_to_large =  2;

    u2TmpValue = (*pu1UILarge<<u1Small_ui_to_large) + *pu1UISmall;
    u2TmpValueOEN = (*pu1UILargeOEN<<u1Small_ui_to_large) + *pu1UISmallOEN;

    if(*pu1PI<10)
    {
        *pu1PI += (TX_DQ_UI_TO_PI_TAP)>>1;
        u2TmpValue -= 1;
        u2TmpValueOEN -= 1;
    }
    else
    if(*pu1PI>TX_DQ_UI_TO_PI_TAP-10)
    {
        *pu1PI -= (TX_DQ_UI_TO_PI_TAP)>>1;
        u2TmpValue += 1;
        u2TmpValueOEN += 1;
    }
    *pu1UISmall = u2TmpValue - ((u2TmpValue >> u1Small_ui_to_large) <<u1Small_ui_to_large);
    *pu1UILarge = (u2TmpValue >> u1Small_ui_to_large);
    *pu1UISmallOEN = u2TmpValueOEN - ((u2TmpValueOEN >> u1Small_ui_to_large) <<u1Small_ui_to_large);
    *pu1UILargeOEN = (u2TmpValueOEN >> u1Small_ui_to_large);
#endif
}
#endif

void DramcTXSetVref(DRAMC_CTX_T *p, U8 u1VrefRange, U8 u1VrefValue)
{
    U8 u1TempOPValue = ((u1VrefValue & 0x3f) | (u1VrefRange<<6));
    
    u1MR14Value[p->channel][p->rank][p->dram_fsp] = u1TempOPValue;
    DramcModeRegWriteByRank(p, p->rank, 14, u1TempOPValue);  
}

DRAM_STATUS_T DramcTxWindowPerbitCal(DRAMC_CTX_T *p, DRAM_TX_PER_BIT_CALIBRATION_TYTE_T calType, U8 u1VrefScanEnable)
{
    U8 u1BitIdx, u1ByteIdx, u1SmallestDQSByte=0;
    U8 ucindex;
    U32 uiFinishCount;
    PASS_WIN_DATA_T WinPerBit[DQ_DATA_WIDTH], VrefWinPerBit[DQ_DATA_WIDTH], FinalWinPerBit[DQ_DATA_WIDTH];

    U16 uiDelay, u2DQDelayBegin=0, u2DQDelayEnd=0;
    U8 ucdq_pi, ucdq_ui_small, ucdq_ui_large,ucdq_oen_ui_small, ucdq_oen_ui_large;
    U8 ucdq_ui_small_reg_value, ucdq_ui_large_reg_value;  // for UI and TXDLY change check, if different , set reg.
    static U8 dq_ui_small_bak, dq_ui_large_bak,  dq_oen_ui_small_bak, dq_oen_ui_large_bak;
    static U16 u2TX_DQ_PreCal_LP4[DQS_NUMBER];  // LP4 only
    U16 u2TX_DQ_PreCal_LP4_Samll;
    U8 ucdq_final_pi[DQS_NUMBER], ucdq_final_ui_large[DQS_NUMBER], ucdq_final_ui_small[DQS_NUMBER];
    U8 ucdq_final_oen_ui_large[DQS_NUMBER], ucdq_final_oen_ui_small[DQS_NUMBER];

    U8 ucdq_final_dqm_pi[DQS_NUMBER] = {0}, ucdq_final_dqm_ui_large[DQS_NUMBER] = {0}, ucdq_final_dqm_ui_small[DQS_NUMBER] = {0};
    U8 ucdq_final_dqm_oen_ui_large[DQS_NUMBER] = {0}, ucdq_final_dqm_oen_ui_small[DQS_NUMBER] = {0};

    #if ENABLE_TX_TRACKING
    U16 u2TXTrackingTmpValue_DQ[DQS_NUMBER];
    U16 u2TXTrackingTmpValue_DQM[DQS_NUMBER];
    #endif
    #if SUPPORT_PASS_WINDOW_SPEED_UP
    U8 u1AllBitPassCount, u1SkipThisVref;
    #endif
    #if 1//TX_DQM_CALC_MAX_MIN_CENTER
    U16 u2Center_min[DQS_NUMBER],u2Center_max[DQS_NUMBER];
    #endif
    #if TX_PER_BIT_DELAY_CELL
    U8 u1EnableDelayCell=0;
    U8 u1DelayCellOfst[DQ_DATA_WIDTH];
    #endif
    U32 u4err_value, u4fail_bit;
    //U8 u1VrefScanEnable;
    U16 u2VrefRange, u2VrefLevel, u2FinalVref=0xd;
    U16 u2VrefBegin, u2FinalRange=0, u2VrefEnd, u2VrefStep;
    U16 u2TempWinSum, u2tx_window_sum;
    U32 u4TempRegValue;
    U16 u2TmpValue;
#if ENABLE_CLAIBTAION_WINDOW_LOG_FOR_FT
    U16 u2MinWinSize = 0xffff;
    U8 u1MinWinSizeBitidx;
#endif
    U16 TXPerbitWin_min_max = 0;
    U32 min_bit, min_winsize;

    U8 ii, backup_rank, u1PrintWinData;

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
	S16 u1minfirst_pass=0xff;
	//U8 fBypass_TXWINDOW;
#endif
#if TX_TRIANGLE_EYE_WORKAROUND
    U8 tx_triangle_eye_jump_flag = TX_TRIANGLE_EYE_STATE_START;
#endif

    if (!p)
    {
        mcSHOW_ERR_MSG(("context NULL\n"));
        return DRAM_FAIL;
    }
    
 #if 0 //SUPPORT_SAVE_TIME_FOR_CALIBRATION
 if(p->pSavetimeData->femmc_Ready==1)
 {
 	#if BYPASS_TXWINDOW
 		if(p->freq_sel>1)
 		{
			fBypass_TXWINDOW=1;
 		}
		else
		{
			fBypass_TXWINDOW=0;
		}
	#else
			fBypass_TXWINDOW=0;
	#endif
}
 else
 	{
			fBypass_TXWINDOW=0;
 	}
#endif
    backup_rank = u1GetRank(p);

    //Set TX delay chain to 0
    /*
        TX_ARDQ7_DLY = 0
        TX_ARDQ6_DLY = 0
        TX_ARDQ5_DLY = 0
        TX_ARDQ4_DLY = 0
        TX_ARDQ3_DLY = 0
        TX_ARDQ2_DLY = 0
        TX_ARDQ1_DLY = 0
        TX_ARDQ0_DLY = 0        
    */
    #if TX_PER_BIT_DELAY_CELL

    #if TX_K_DQM_WITH_WDBI
    if(u1IsLP4Family(p->dram_type) && (calType !=TX_DQ_DQS_MOVE_DQM_ONLY))
    #else
    if(u1IsLP4Family(p->dram_type))
    #endif
    {
        #if 1    
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ0), 0);
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ0), 0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ1), 0x0, SHU1_R0_B0_DQ1_RK0_TX_ARDQM0_DLY_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ1), 0x0, SHU1_R0_B1_DQ1_RK0_TX_ARDQM0_DLY_B1);
        #else
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ0), P_Fld(0x0, SHU1_R0_B0_DQ0_RK0_TX_ARDQ7_DLY_B0) 
            | P_Fld(0x0, SHU1_R0_B0_DQ0_RK0_TX_ARDQ6_DLY_B0) 
            | P_Fld(0x0, SHU1_R0_B0_DQ0_RK0_TX_ARDQ5_DLY_B0) 
            | P_Fld(0x0, SHU1_R0_B0_DQ0_RK0_TX_ARDQ4_DLY_B0) 
            | P_Fld(0x0, SHU1_R0_B0_DQ0_RK0_TX_ARDQ3_DLY_B0) 
            | P_Fld(0x0, SHU1_R0_B0_DQ0_RK0_TX_ARDQ2_DLY_B0) 
            | P_Fld(0x0, SHU1_R0_B0_DQ0_RK0_TX_ARDQ1_DLY_B0) 
            | P_Fld(0x0, SHU1_R0_B0_DQ0_RK0_TX_ARDQ0_DLY_B0));    
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ0), P_Fld(0x0, SHU1_R0_B1_DQ0_RK0_TX_ARDQ7_DLY_B1) 
            | P_Fld(0x0, SHU1_R0_B1_DQ0_RK0_TX_ARDQ6_DLY_B1) 
            | P_Fld(0x0, SHU1_R0_B1_DQ0_RK0_TX_ARDQ5_DLY_B1) 
            | P_Fld(0x0, SHU1_R0_B1_DQ0_RK0_TX_ARDQ4_DLY_B1) 
            | P_Fld(0x0, SHU1_R0_B1_DQ0_RK0_TX_ARDQ3_DLY_B1) 
            | P_Fld(0x0, SHU1_R0_B1_DQ0_RK0_TX_ARDQ2_DLY_B1) 
            | P_Fld(0x0, SHU1_R0_B1_DQ0_RK0_TX_ARDQ1_DLY_B1) 
            | P_Fld(0x0, SHU1_R0_B1_DQ0_RK0_TX_ARDQ0_DLY_B1));
        #endif
    }
    #endif

    //A.set RX DQ/DQS in the middle of the pass region from read DQ/DQS calibration
    //B.Fix DQS (RG_PI_**_PBYTE*) at degree from write leveling.
    //   Move DQ (per byte) gradually from 90 to -45 degree to find the left boundary
    //   Move DQ (per byte) gradually from 90 to 225 degree to find the right boundary
    //C.For each DQ delay in step B, start engine test
    //D.After engine test, read per bit results from registers.
    //E.Set RG_PI_**_DQ* to lie in the average of the middle of the pass region in the same byte
    if(fgwrlevel_done)
    {
        // Find smallest DQS delay after write leveling. DQ PI scan from smallest DQS PI.
        u2DQDelayBegin =0xff;
        for(u1ByteIdx=0; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
        {
            if(wrlevel_dqs_final_delay[p->channel][u1ByteIdx] < u2DQDelayBegin)
            {
                u2DQDelayBegin = wrlevel_dqs_final_delay[p->channel][u1ByteIdx];
                u1SmallestDQSByte = u1ByteIdx;
            }
        }
    }
    else
        u1SmallestDQSByte =0;

    if(u1IsLP4Family(p->dram_type))
    {
        // if not do write leveling, it will choose DQS position of byte 0 as DQ start position
        u4TempRegValue= u4IO32Read4B(DRAMC_REG_SHU_SELPH_DQS0);
        dq_ui_large_bak = (u4TempRegValue >> (u1SmallestDQSByte*4)) & 0x7;


        u4TempRegValue= u4IO32Read4B(DRAMC_REG_SHU_SELPH_DQS1);
        dq_ui_small_bak = (u4TempRegValue >> (u1SmallestDQSByte*4)) & 0x7;

        // OEN
        u2TmpValue = (dq_ui_large_bak<<3) + dq_ui_small_bak -TX_DQ_OE_SHIFT; // DQ OEN shift -3
        dq_oen_ui_large_bak= (u2TmpValue>>3);
        dq_oen_ui_small_bak= u2TmpValue - (dq_oen_ui_large_bak << 3);

#if TX_K_DQM_WITH_WDBI
       if((calType==TX_DQ_DQS_MOVE_DQM_ONLY) && (dq_ui_large_bak>1))
            dq_ui_large_bak--;
#endif
        //if Write DBI is on, DQ and DQM -1MCK
//        if(p->DBI_W_onoff)
        //if(u4IO32ReadFldAlign(DRAMC_REG_SHU1_WODT, SHU1_WODT_DBIWR))
//                dq_ui_large_bak --;

        // (1)LP4 will calibration DQM at the first time, K full range, and then rember the TX position.
        // (2)The sencod time will calibrate DQ+Vref, reference TX postion of (1)
        if(calType==TX_DQ_DQS_MOVE_DQ_DQM)
        {
        	#if 0 //SUPPORT_SAVE_TIME_FOR_CALIBRATION
		if(p->pSavetimeData->femmc_Ready==1 && ((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x8)>>3)==0)
		{
            		u2DQDelayBegin = p->pSavetimeData->u1TxWindowPerbit_Min_FirstPass_Save[p->channel][p->rank][p->freq_sel]-p->pSavetimeData->u1TxWindowPerbitoffset; 
		}
		else
		#endif
		{
        u2DQDelayBegin = TX_DQ_DELAY_BEGIN_LP4;
		}
        u2DQDelayEnd = TX_DQ_DELAY_END_LP4;
        }
        else //if(calType==TX_DQ_DQS_MOVE_DQ_ONLY)
        {
            if(u2TX_DQ_PreCal_LP4[0] <u2TX_DQ_PreCal_LP4[1])
                u2TX_DQ_PreCal_LP4_Samll = u2TX_DQ_PreCal_LP4[0];
            else
                u2TX_DQ_PreCal_LP4_Samll = u2TX_DQ_PreCal_LP4[1];

            if(u2TX_DQ_PreCal_LP4_Samll >24)
                u2DQDelayBegin = u2TX_DQ_PreCal_LP4_Samll-24;
        else
                u2DQDelayBegin =0;

        u2DQDelayEnd = u2DQDelayBegin + 64;
        }
    }
#if ENABLE_LP3_SW
    else //LPDDR3
    {
        if(p->fgTXPerbifInit[p->channel][p->rank]== FALSE)
        {
            // Scan from DQ delay = DQS delay
            // For everest, choose DQS 1 which is smaller and won't over 64.
            u4TempRegValue= u4IO32Read4B(DRAMC_REG_SHU_SELPH_DQS0);
            dq_ui_large_bak = (u4TempRegValue >> (u1SmallestDQSByte*4)) & 0x7;
            dq_oen_ui_large_bak= (u4TempRegValue >> (u1SmallestDQSByte*4+16)) & 0x7;

            u4TempRegValue= u4IO32Read4B(DRAMC_REG_SHU_SELPH_DQS1);
            dq_ui_small_bak = (u4TempRegValue >> (u1SmallestDQSByte*4)) & 0x7;
            dq_oen_ui_small_bak= (u4TempRegValue >> (u1SmallestDQSByte*4+16)) & 0x7;

            p->fgTXPerbifInit[p->channel][p->rank]= TRUE;
        }

        u2DQDelayBegin = wrlevel_dqs_final_delay[p->channel][u1SmallestDQSByte];
        u2DQDelayEnd = u2DQDelayBegin +96; //Scan at least 1UI. Scan 64 to cover byte differece. if window found, early break.
    }
#endif

#if 0
    if(u1IsLP4Family(p->dram_type) && (p->enable_tx_scan_vref==ENABLE) && (calType ==TX_DQ_DQS_MOVE_DQ_ONLY)
#if SUPPORT_SAVE_OFF_LINE_CALIBRATION
        && (gTX_Vref_Scan_Check[p->channel][p->rank][p->dram_fsp]==1) && (p->pOffLineSaveData->u2TXWinSum_Save[p->channel][p->rank][p->dram_fsp]>0)
#endif
        )
        u1VrefScanEnable =1;
    else
        u1VrefScanEnable =0;
    #else
    if(u1IsLP4Family(p->dram_type)==0)//LP3
        u1VrefScanEnable=0;
    #endif

#if VENDER_JV_LOG
    if (calType ==TX_DQ_DQS_MOVE_DQ_ONLY)
        vPrintCalibrationBasicInfo_ForJV(p);
#else
        vPrintCalibrationBasicInfo(p);
#endif

    #if SIMPLIFY_VREF_SCAN
    //Skip Vref Scan if current FSP has been scanned
    if(u1IsLP4Family(p->dram_type) && (p->skipVrefScan == ENABLE) && (u1VrefScanEnable ==1))
    {
        u1VrefScanEnable =0;
        DramcModeRegWriteByRank(p, p->rank, 14, u1MR14Value[p->channel][p->rank][p->dram_fsp]);
        mcSHOW_DBG_MSG(("Skip TX Vref(dq) scan. Setting %d\n", u1MR14Value[p->channel][p->rank][p->dram_fsp]));
        mcFPRINTF((fp_A60501, "Skip TX Vref(dq) scan. Setting %d\n", u1MR14Value[p->channel][p->rank][p->dram_fsp]));
        #if VENDER_JV_LOG
        mcSHOW_DBG_MSG5(("Skip TX Vref(dq) scan. Setting %d\n", u1MR14Value[p->channel][p->rank][p->dram_fsp]));
        #endif
    }
#endif /* SIMPLIFY_VREF_SCAN */

    if(u1VrefScanEnable)
    {
    	#if (SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_VREF_CAL)
          if(p->pSavetimeData->femmc_Ready==1&& ((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x8)>>3)==0)
          {
        	u2VrefBegin = p->pSavetimeData->u1TxWindowPerbitVref_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]];                		
        	u2VrefEnd = u2VrefBegin+1;
            u2VrefStep = TX_VREF_RANGE_STEP;            
           }
          else
      #endif
        {
#if VREF_SPEED_UP_LP4		
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
#if PU_CAL_05VDDQ
                    //range 1          
                    u2VrefBegin = 14 - 5; // 250/600(VDDQ)=41.6%
                    u2VrefEnd = 14 + 5;                 
#else
                    //range 1          
                    u2VrefBegin = 27 - 5; // 290/600(VDDQ)=48.3%
                    u2VrefEnd = 27 + 5;                 
#endif                
                }
                u2VrefStep = 1;
            }
            else
#endif                
            {
#if TX_TRIANGLE_EYE_WORKAROUND   
                u2VrefBegin = 0; //if enable TX_TRIANGLE_EYE_WORKAROUND, must scan vref from 0
#else
                u2VrefBegin = TX_VREF_RANGE_BEGIN;
#endif
                u2VrefEnd = TX_VREF_RANGE_END;
                u2VrefStep = TX_VREF_RANGE_STEP;            
            }
        }

    }
    else //LPDDR3, the for loop will only excute u2VrefLevel=TX_VREF_RANGE_END/2.
    {
        u2VrefBegin = 0;
        u2VrefEnd = 0;
        u2VrefStep =  1;
    }


#ifdef TX_VREFDQ_EYE_SCAN
    if (calType ==TX_DQ_DQS_MOVE_DQ_ONLY)
    {
        u2VrefRange = 0;
        u2VrefBegin = 0;
        u2VrefEnd = 50;
        u2VrefStep = 1;
    }
    else
#endif
    u2FinalRange = u2VrefRange = (!p->odt_onoff);

    u2tx_window_sum =0;

    vSetCalibrationResult(p, DRAM_CALIBRATION_TX_PERBIT, DRAM_FAIL);

    mcSHOW_DBG_MSG(("[TxWindowPerbitCal] calType=%d, VrefScanEnable %d\n"
                    "\nBegin, TX DQ(%d, %d), DQ OEN(%d, %d), DQ Scan Range %d~%d\n",
                    calType, u1VrefScanEnable,
                    dq_ui_large_bak, dq_ui_small_bak, dq_oen_ui_large_bak,  dq_oen_ui_small_bak,
                    u2DQDelayBegin, u2DQDelayEnd));

    mcFPRINTF((fp_A60501, "\n[TxWindowPerbitCal] Begin,TX DQ(%d, %d), DQ OEN(%d, %d)\n", dq_ui_large_bak, dq_ui_small_bak,dq_oen_ui_large_bak,  dq_oen_ui_small_bak));
    //Use HW TX tracking value
    //R_DMARPIDQ_SW :drphy_conf (0x170[7])(default set 1)
    //   0: DQS2DQ PI setting controlled by HW
    //R_DMARUIDQ_SW : Dramc_conf(0x156[15])(default set 1)
    //    0: DQS2DQ UI setting controlled by HW
    ///TODO: need backup original setting?
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 1, MISC_CTRL1_R_DMARPIDQ_SW);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR), 1, DQSOSCR_ARUIDQ_SW);

                
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
 if(p->pSavetimeData->femmc_Ready==1 && (p->pSavetimeData->Bypass_TXWINDOW[_MappingFreqArray[p->freq_sel]]))
 	{
			    for (u1ByteIdx=0; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
    			  {
               		 u2Center_min[u1ByteIdx]=p->pSavetimeData->u1TxCenter_min_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][u1ByteIdx];
               		 u2Center_max[u1ByteIdx]=p->pSavetimeData->u1TxCenter_max_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][u1ByteIdx];
		                for (u1BitIdx=0; u1BitIdx<DQS_BIT_NUMBER; u1BitIdx++)
               		 {
                 			U8 u1BitSave = u1ByteIdx*DQS_BIT_NUMBER+u1BitIdx;
               		FinalWinPerBit[u1BitSave].win_center= p->pSavetimeData->u1Txwin_center_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][u1BitSave];
               		 FinalWinPerBit[u1BitSave].first_pass=p->pSavetimeData->u1Txfirst_pass_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][u1BitSave];
               		 FinalWinPerBit[u1BitSave].last_pass=p->pSavetimeData->u1Txlast_pass_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][u1BitSave];
              		             	}
   			  }
                        vSetCalibrationResult(p, DRAM_CALIBRATION_TX_PERBIT, DRAM_OK);
    }

 else
 	#endif
 	{
        #if 0//TX_K_DQM_WITH_WDBI
        if(calType ==TX_DQ_DQS_MOVE_DQM_ONLY)
        {
        DramcEngine2Init(p, p->test2_1, p->test2_2, TEST_AUDIO_PATTERN, 0);
        }
        else
        #endif
        DramcEngine2Init(p, p->test2_1, p->test2_2, p->test_pattern, 0);

    for(u2VrefLevel = u2VrefBegin; u2VrefLevel <= u2VrefEnd; u2VrefLevel += u2VrefStep)
    {
         //   SET tx Vref (DQ) here, LP3 no need to set this.
        if(u1VrefScanEnable)
        {
            #if (!REDUCE_LOG_FOR_PRELOADER)
            mcSHOW_DBG_MSG(("\n\n\tLP4 TX VrefRange %d, VrefLevel=%d\n", u2VrefRange, u2VrefLevel));
            mcFPRINTF((fp_A60501, "\n\n\tLP4 TX VrefRange %d,VrefLevel=%d\n", u2VrefRange, u2VrefLevel));
            #endif

            #if VENDER_JV_LOG
            if(calType ==TX_DQ_DQS_MOVE_DQ_ONLY)
            {
                mcSHOW_DBG_MSG5(("\n\n\tLP4 TX VrefRange %d, VrefLevel=%d\n", u2VrefRange, u2VrefLevel));
            }
            #endif

            u1MR14Value[p->channel][p->rank][p->dram_fsp] = (u2VrefLevel | (u2VrefRange<<6));
            DramcModeRegWriteByRank(p, p->rank, 14, u2VrefLevel | (u2VrefRange<<6));
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
        ucdq_ui_large_reg_value = 0xff;

#if SUPPORT_PASS_WINDOW_SPEED_UP
        u1AllBitPassCount =0;
        u1SkipThisVref =0;
#endif

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
        for (uiDelay = u2DQDelayBegin; uiDelay <=u2DQDelayEnd; uiDelay++)
        //    for (uiDelay = u2DQDelayBegin; uiDelay <=u2DQDelayEnd; uiDelay+=3)  //DBI test
        {
            TxWinTransferDelayToUIPI(p, uiDelay, dq_ui_small_bak, dq_ui_large_bak, &ucdq_ui_large, &ucdq_ui_small, &ucdq_pi);
            TxWinTransferDelayToUIPI(p, uiDelay, dq_oen_ui_small_bak, dq_oen_ui_large_bak, &ucdq_oen_ui_large, &ucdq_oen_ui_small, &ucdq_pi);

            if(calType ==TX_DQ_DQS_MOVE_DQ_ONLY || calType== TX_DQ_DQS_MOVE_DQ_DQM)
            {
                //TXDLY_DQ , TXDLY_OEN_DQ
                if(ucdq_ui_small_reg_value != ucdq_ui_small)
                {
                    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0), \
                                                    P_Fld(ucdq_ui_large, SHURK0_SELPH_DQ0_TXDLY_DQ0) | \
                                                    P_Fld(ucdq_ui_large, SHURK0_SELPH_DQ0_TXDLY_DQ1) | \
                                                    P_Fld(ucdq_ui_large, SHURK0_SELPH_DQ0_TXDLY_DQ2) | \
                                                    P_Fld(ucdq_ui_large, SHURK0_SELPH_DQ0_TXDLY_DQ3) | \
                                                    P_Fld(ucdq_oen_ui_large, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0) | \
                                                    P_Fld(ucdq_oen_ui_large, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1) | \
                                                    P_Fld(ucdq_oen_ui_large, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2) | \
                                                    P_Fld(ucdq_oen_ui_large, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3));
                
                    // DLY_DQ[2:0]
                   vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ2), \
                                                    P_Fld(ucdq_ui_small, SHURK0_SELPH_DQ2_DLY_DQ0) | \
                                                    P_Fld(ucdq_ui_small, SHURK0_SELPH_DQ2_DLY_DQ1) | \
                                                    P_Fld(ucdq_ui_small, SHURK0_SELPH_DQ2_DLY_DQ2) | \
                                                    P_Fld(ucdq_ui_small, SHURK0_SELPH_DQ2_DLY_DQ3) | \
                                                    P_Fld(ucdq_oen_ui_small, SHURK0_SELPH_DQ2_DLY_OEN_DQ0) | \
                                                    P_Fld(ucdq_oen_ui_small, SHURK0_SELPH_DQ2_DLY_OEN_DQ1) | \
                                                    P_Fld(ucdq_oen_ui_small, SHURK0_SELPH_DQ2_DLY_OEN_DQ2) | \
                                                    P_Fld(ucdq_oen_ui_small, SHURK0_SELPH_DQ2_DLY_OEN_DQ3));
                }
            }

            if(calType ==TX_DQ_DQS_MOVE_DQM_ONLY || calType== TX_DQ_DQS_MOVE_DQ_DQM)
            {
                //TXDLY_DQM , TXDLY_OEN_DQM
                if(ucdq_ui_small_reg_value != ucdq_ui_small)
                {
                    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ1), \
                                                    P_Fld(ucdq_ui_large, SHURK0_SELPH_DQ1_TXDLY_DQM0) | \
                                                    P_Fld(ucdq_ui_large, SHURK0_SELPH_DQ1_TXDLY_DQM1) | \
                                                    P_Fld(ucdq_ui_large, SHURK0_SELPH_DQ1_TXDLY_DQM2) | \
                                                    P_Fld(ucdq_ui_large, SHURK0_SELPH_DQ1_TXDLY_DQM3) | \
                                                    P_Fld(ucdq_oen_ui_large, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0) | \
                                                    P_Fld(ucdq_oen_ui_large, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1) | \
                                                    P_Fld(ucdq_oen_ui_large, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2) | \
                                                    P_Fld(ucdq_oen_ui_large, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3));
                
                    // DLY_DQM[2:0]
                   vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ3), \
                                                    P_Fld(ucdq_ui_small, SHURK0_SELPH_DQ3_DLY_DQM0) | \
                                                    P_Fld(ucdq_ui_small, SHURK0_SELPH_DQ3_DLY_DQM1) | \
                                                    P_Fld(ucdq_ui_small, SHURK0_SELPH_DQ3_DLY_DQM2) | \
                                                    P_Fld(ucdq_ui_small, SHURK0_SELPH_DQ3_DLY_DQM3) | \
                                                    P_Fld(ucdq_oen_ui_small, SHURK0_SELPH_DQ3_DLY_OEN_DQM0) | \
                                                    P_Fld(ucdq_oen_ui_small, SHURK0_SELPH_DQ3_DLY_OEN_DQM1) | \
                                                    P_Fld(ucdq_oen_ui_small, SHURK0_SELPH_DQ3_DLY_OEN_DQM2) | \
                                                    P_Fld(ucdq_oen_ui_small, SHURK0_SELPH_DQ3_DLY_OEN_DQM3));
                }
            }

            ucdq_ui_small_reg_value = ucdq_ui_small;
            ucdq_ui_large_reg_value = ucdq_ui_large;

            //set to registers, PI DQ (per byte)
            if(u1IsLP4Family(p->dram_type))
            {
                // update TX DQ PI delay, for rank 0 // need to take care rank 1 and 2
                if(calType ==TX_DQ_DQS_MOVE_DQ_ONLY || calType== TX_DQ_DQS_MOVE_DQ_DQM)
                {
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ucdq_pi, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ7), ucdq_pi, SHU1_R0_B1_DQ7_RK0_ARPI_DQ_B1);
                }

                if(calType ==TX_DQ_DQS_MOVE_DQM_ONLY || calType== TX_DQ_DQS_MOVE_DQ_DQM)
                {
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ucdq_pi, SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0);
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ7), ucdq_pi, SHU1_R0_B1_DQ7_RK0_ARPI_DQM_B1);
                }
            }
#if ENABLE_LP3_SW
            else //LPDDR3
            {
                vIO32WriteFldAlign_Phy_Byte(0, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ucdq_pi, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);
                vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ucdq_pi, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);
                vIO32WriteFldAlign_Phy_Byte(2, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ucdq_pi, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);
                vIO32WriteFldAlign_Phy_Byte(3, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ucdq_pi, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);

                #if (FOR_DV_SIMULATION_USED!=0)
                vIO32WriteFldAlign_Phy_Byte(0, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ucdq_pi, SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0);
                vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ucdq_pi, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CS);
                vIO32WriteFldAlign_Phy_Byte(2, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ucdq_pi, SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0);
                vIO32WriteFldAlign_Phy_Byte(3, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ucdq_pi, SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0);
                #else
                vIO32WriteFldAlign_Phy_Byte(0, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ucdq_pi, SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0);
                vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ucdq_pi, SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0);
                vIO32WriteFldAlign_Phy_Byte(2, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ucdq_pi, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CS);
                vIO32WriteFldAlign_Phy_Byte(3, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ucdq_pi, SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0);
                #endif
            }
#endif

            #if TX_K_DQM_WITH_WDBI
            u4err_value=0;
            if(calType ==TX_DQ_DQS_MOVE_DQM_ONLY)
            { 
                DramcEngine2Init(p, p->test2_1, p->test2_2, TEST_AUDIO_PATTERN, 0);
                u4err_value = DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, TEST_AUDIO_PATTERN);
                
                DramcEngine2Init(p, p->test2_1, p->test2_2, TEST_XTALK_PATTERN, 0);
                u4err_value |= DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, TEST_XTALK_PATTERN);   
            }     
            else
            #endif
            {
                u4err_value = DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, p->test_pattern);
            }

            #if (VREF_SPEED_UP_LP4 && !TX_K_DQM_DEUBG_LOG)
            if(u1VrefScanEnable==0)
            #endif
            {
                //mcSHOW_DBG_MSG(("Delay=%3d |%2d %2d %3d| %2d %2d| 0x%8x [0]",uiDelay, ucdq_ui_large,ucdq_ui_small, ucdq_pi, ucdq_oen_ui_large,ucdq_oen_ui_small, u4err_value));
                #ifdef ETT_PRINT_FORMAT
                #if (!TX_K_DQM_DEUBG_LOG)
                if(u4err_value !=0)
                #endif
                {
                    mcSHOW_DBG_MSG2(("%d |%d %d %d|[0]", uiDelay, ucdq_ui_large, ucdq_ui_small, ucdq_pi));
                }
                #else
                mcSHOW_DBG_MSG2(("Delay=%3d |%2d %2d %3d| 0x%8x [0]", uiDelay, ucdq_ui_large, ucdq_ui_small, ucdq_pi, u4err_value));
                #endif
                mcFPRINTF((fp_A60501, "Delay=%3d | %2d %2d %3d| 0x%8x [0]", uiDelay, ucdq_ui_large,ucdq_ui_small, ucdq_pi, u4err_value));
            }

            #if SUPPORT_PASS_WINDOW_SPEED_UP
            if(u1IsLP4Family(p->dram_type) && u1VrefScanEnable)
            {
                if(u1AllBitPassCount == 4 &&  u4err_value!=0)
                {
                    u1SkipThisVref =1;
                    mcSHOW_DBG_MSG(("SUPPORT_PASS_WINDOW_SPEED_UP TX Overjump\n"));
                    break;
                }

                if(u4err_value ==0) // all bit pass
                    u1AllBitPassCount ++;

                if(u1AllBitPassCount == 3)
                {
                    mcSHOW_DBG_MSG(("SUPPORT_PASS_WINDOW_SPEED_UP TX  Delay%d -> ",uiDelay));
                    if(p->freqGroup == 800)
                    {
                        uiDelay += 15;
                    }
                    else if (p->freqGroup == 1333 || p->freqGroup == 1200)
                    {
                        uiDelay += 13;
                    }
                    else
                    {
                        uiDelay += 11;
                    }

                    u1AllBitPassCount ++;
                    mcSHOW_DBG_MSG(("%d\n",uiDelay));
                }
            }
            #endif

            // check fail bit ,0 ok ,others fail
            for (u1BitIdx = 0; u1BitIdx < p->data_width; u1BitIdx++)
            {
                u4fail_bit = u4err_value&((U32)1<<u1BitIdx);

                #if (VREF_SPEED_UP_LP4  && !TX_K_DQM_DEUBG_LOG)
                if(u1VrefScanEnable==0)
                #endif
                {
                    #if !TX_K_DQM_DEUBG_LOG
                    if(u4err_value != 0)
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
                        }
                        else
                        {
                                mcSHOW_DBG_MSG2(("x"));
                                mcFPRINTF((fp_A60501, "x"));
                        }
                    }
                }

                if(WinPerBit[u1BitIdx].first_pass== PASS_RANGE_NA)
                {
                    if(u4fail_bit==0) //compare correct: pass
                    {
                        WinPerBit[u1BitIdx].first_pass = uiDelay;
                    }
                }
                else if(WinPerBit[u1BitIdx].last_pass == PASS_RANGE_NA)
                {
                    if(u4fail_bit !=0) //compare error : fail
                    {
                        WinPerBit[u1BitIdx].last_pass  = (uiDelay-1);
                    }
                    else if (uiDelay==u2DQDelayEnd)
                    //else if (uiDelay==MAX_TX_DQDLY_TAPS)
                    {
                        WinPerBit[u1BitIdx].last_pass  = uiDelay;
                    }

                    if(WinPerBit[u1BitIdx].last_pass  !=PASS_RANGE_NA)
                    {
                        if((WinPerBit[u1BitIdx].last_pass -WinPerBit[u1BitIdx].first_pass) >= (VrefWinPerBit[u1BitIdx].last_pass -VrefWinPerBit[u1BitIdx].first_pass))
                        {
                            #if 0 //for debug
                            if(VrefWinPerBit[u1BitIdx].last_pass != PASS_RANGE_NA)
                            {
                                mcSHOW_DBG_MSG2(("Bit[%d] Bigger window update %d > %d\n", u1BitIdx, \
                                    (WinPerBit[u1BitIdx].last_pass -WinPerBit[u1BitIdx].first_pass), (VrefWinPerBit[u1BitIdx].last_pass -VrefWinPerBit[u1BitIdx].first_pass)));
                                mcFPRINTF((fp_A60501,"Bit[%d] Bigger window update %d > %d\n", u1BitIdx, \
                                    (WinPerBit[u1BitIdx].last_pass -WinPerBit[u1BitIdx].first_pass), (VrefWinPerBit[u1BitIdx].last_pass -VrefWinPerBit[u1BitIdx].first_pass)));

                            }
                            #endif
                            
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

                #if (VREF_SPEED_UP_LP4 && !TX_K_DQM_DEUBG_LOG)
                if(u1VrefScanEnable==0)
                #endif
                {
                    #if !TX_K_DQM_DEUBG_LOG
                    if(u4err_value != 0)
                    #endif
                    {
                        mcSHOW_DBG_MSG2((" [MSB]\n"));
                        mcFPRINTF((fp_A60501, " [MSB]\n"));
                    }
                }

                //if all bits widnow found and all bits turns to fail again, early break;
                if((u1IsLP4Family(p->dram_type) &&(uiFinishCount == 0xffff)) || \
                    ((p->dram_type == TYPE_LPDDR3) &&(uiFinishCount == 0xffffffff)))
                    {
                        vSetCalibrationResult(p, DRAM_CALIBRATION_TX_PERBIT, DRAM_OK);
                        #if !VREF_SPEED_UP_LP4
                        if(u1VrefScanEnable ==0)
                        #endif
                        {
                            if((u1IsLP4Family(p->dram_type) &&(u4err_value == 0xffff)) || \
                                ((p->dram_type == TYPE_LPDDR3) &&(u4err_value == 0xffffffff)))
                                {
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
                    }
        }

        min_winsize = 0xffff;
        min_bit = 0xff;
        for (u1BitIdx = 0; u1BitIdx < p->data_width; u1BitIdx++)
        {
            VrefWinPerBit[u1BitIdx].win_size = VrefWinPerBit[u1BitIdx].last_pass- VrefWinPerBit[u1BitIdx].first_pass +1;

            if (VrefWinPerBit[u1BitIdx].win_size < min_winsize)
            {
                min_bit = u1BitIdx;
                min_winsize = VrefWinPerBit[u1BitIdx].win_size;
            }

            u2TempWinSum += VrefWinPerBit[u1BitIdx].win_size;  //Sum of CA Windows for vref selection

            #if ENABLE_CLAIBTAION_WINDOW_LOG_FOR_FT
            if(VrefWinPerBit[u1BitIdx].win_size<u2MinWinSize)
            {
                u2MinWinSize = VrefWinPerBit[u1BitIdx].win_size;
                u1MinWinSizeBitidx = u1BitIdx;
            }
            #endif

            #if VENDER_JV_LOG
            if(calType ==TX_DQ_DQS_MOVE_DQ_ONLY)
            {
                mcSHOW_DBG_MSG5(("TX Bit%d, %d%%\n", u1BitIdx,  (VrefWinPerBit[u1BitIdx].win_size*100+31)/32));
            }
            #endif
        }
        mcSHOW_DBG_MSG3(("Min Bit=%d, winsize=%d\n",min_bit, min_winsize));

#if TX_TRIANGLE_EYE_WORKAROUND
#if SIMPLIFY_VREF_SCAN
        if (u1IsLP4Family(p->dram_type) && (p->enable_tx_scan_vref==ENABLE) && (calType == TX_DQ_DQS_MOVE_DQ_ONLY) && (p->skipVrefScan == DISABLE) && (p->odt_onoff == ODT_ON))
#else
    #if TX_K_DQM_WITH_WDBI
        if (u1IsLP4Family(p->dram_type) && (p->enable_tx_scan_vref==ENABLE) && u1VrefScanEnable && (p->odt_onoff == ODT_ON))
    #else
        if (u1IsLP4Family(p->dram_type) && (p->enable_tx_scan_vref==ENABLE) && (calType == TX_DQ_DQS_MOVE_DQ_ONLY) && (p->odt_onoff == ODT_ON))
    #endif
#endif /* SIMPLIFY_VREF_SCAN */
        {
            if (tx_triangle_eye_jump_flag == TX_TRIANGLE_EYE_STATE_START)
            {                          
                for (u1BitIdx = 0; u1BitIdx < p->data_width; u1BitIdx++)
                {
                    if (VrefWinPerBit[u1BitIdx].win_size < 8) break;
                }

                if (u1BitIdx == p->data_width)
                {
                    if (u2VrefLevel != 0)
                    {
                        tx_triangle_eye_jump_flag = TX_TRIANGLE_EYE_STATE_ADD_MARGIN;
                    }
                }
            }

            if (tx_triangle_eye_jump_flag == TX_TRIANGLE_EYE_STATE_RESTART)
            {                          
                tx_triangle_eye_jump_flag = TX_TRIANGLE_EYE_STATE_END;
                
                for (u1BitIdx = 0; u1BitIdx < p->data_width; u1BitIdx++)
                {
                    if (VrefWinPerBit[u1BitIdx].win_size < 8)
                    {
                        mcSHOW_ERR_MSG(("\n[TX TRAINGLE EYE] Eye win_size is too small!!\n\n"));
                    }
                }
            }
        }
#endif     

        #if ENABLE_CLAIBTAION_WINDOW_LOG_FOR_FT
        mcSHOW_DBG_MSG2(("FT log: TX min window : bit %d, size %d\n", u1MinWinSizeBitidx, u2MinWinSize));
        #endif
        #if (!REDUCE_LOG_FOR_PRELOADER || TX_K_DQM_DEUBG_LOG)
        mcSHOW_DBG_MSG(("\nTX Vref %d, min_winsize %d, Window Sum %d\n", u2VrefLevel, min_winsize ,u2TempWinSum));
        mcFPRINTF((fp_A60501, "\nTX Vref %d, min_winsize %d, Window Sum %d\n", u2VrefLevel, min_winsize ,u2TempWinSum));
        #endif

        u1PrintWinData =0;
        #if SUPPORT_PASS_WINDOW_SPEED_UP
        if (((min_winsize > TXPerbitWin_min_max) || ((min_winsize == TXPerbitWin_min_max) && (u2TempWinSum >u2tx_window_sum))) &&(!u1SkipThisVref))
        #else
        if ((min_winsize > TXPerbitWin_min_max) || ((min_winsize == TXPerbitWin_min_max) && (u2TempWinSum >u2tx_window_sum)))
        #endif
        {
            #if (REDUCE_LOG_FOR_PRELOADER && !VENDER_JV_LOG)
            u1PrintWinData =0;
            #else
            u1PrintWinData =1;
            #endif

            #ifdef TX_VREFDQ_EYE_SCAN
            if (calType ==TX_DQ_DQS_MOVE_DQ_ONLY)
            #endif
            {
                mcSHOW_DBG_MSG3(("\nBetter TX Vref found %d, min_winsize %d, Window Sum %d\n", u2VrefLevel, min_winsize ,u2TempWinSum));
                mcFPRINTF((fp_A60501, "\nBetter TX Vref found %d, min_winsize %d, Window Sum %d\n", u2VrefLevel, min_winsize ,u2TempWinSum));
            }

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

                    #if 1//
                    if(FinalWinPerBit[ucindex].win_center < u2Center_min[u1ByteIdx])
                        u2Center_min[u1ByteIdx] = FinalWinPerBit[ucindex].win_center;

                    if(FinalWinPerBit[ucindex].win_center > u2Center_max[u1ByteIdx])
                        u2Center_max[u1ByteIdx] = FinalWinPerBit[ucindex].win_center;
                    #endif

                    #ifdef FOR_HQA_TEST_USED
                    #if TX_K_DQM_WITH_WDBI
                    if(calType == TX_DQ_DQS_MOVE_DQ_ONLY)
                    #endif
                    {
                        gFinalTXPerbitWin[p->channel][p->rank][ucindex] = FinalWinPerBit[ucindex].win_size;
                    }
                    #endif
                }
                #if 0 //SUPPORT_SAVE_TIME_FOR_CALIBRATION
	   	 	if(p->pSavetimeData->femmc_Ready==1 &&  u1VrefScanEnable && ( FinalWinPerBit[ucindex].win_size<  p->pSavetimeData->TxWindowTH) && ((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x08)>>3)==0)
	   	 	{
                      	p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]]=1<<3; //Txwindow fail       
	   	 	}				                
                	if(p->pSavetimeData->femmc_Ready==0 || ((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x8)>>3)==1)//save firtst run pass value
                	{

				if(u1minfirst_pass>FinalWinPerBit[ucindex].first_pass)//min_first_pass
				{	
					u1minfirst_pass=FinalWinPerBit[ucindex].first_pass;
				}
               		 p->pSavetimeData->u1TxWindowPerbit_Min_FirstPass_Save[p->channel][p->rank][p->freq_sel]=u1minfirst_pass;
               		 
                	}
                		
                #endif
            }
        }

        #if TX_K_DQM_DEUBG_LOG
        u1PrintWinData =1;
        #endif

        #ifndef TX_VREFDQ_EYE_SCAN
        if(u1PrintWinData)
        #endif
        {

            mcSHOW_DBG_MSG(("\n\tCH=%d, VrefRange= %d, VrefLevel = %d\n", p->channel, u2VrefRange, u2VrefLevel));
            //mcSHOW_DBG_MSG(("PI DQ (per byte) window\nx=pass dq delay value (min~max)center \ny=0-7bit DQ of every group\n"));
            //mcSHOW_DBG_MSG(("==================================================================\n"));
            //mcSHOW_DBG_MSG(("bit    Byte0    bit      Byte1     bit     Byte2     bit     Byte3\n"));
            vPrintCalibrationBasicInfo(p);

            mcFPRINTF((fp_A60501,"\n\tchannel=%d(2:cha, 3:chb)  u2VrefLevel = %d\n", p->channel, u2VrefLevel));
            mcFPRINTF((fp_A60501,"\tPI DQ (per byte) window\nx=pass dq delay value (min~max)center \ny=0-7bit DQ of every group\n\n"));
            mcFPRINTF((fp_A60501,"==================================================================\n"));
            mcFPRINTF((fp_A60501,"bit   Byte0    bit      Byte1     bit     Byte2     bit     Byte3\n"));

            for (u1BitIdx = 0; u1BitIdx < DQS_BIT_NUMBER; u1BitIdx++)
            {
                #ifdef ETT_PRINT_FORMAT
                mcSHOW_DBG_MSG(("TX Bit%d (%d~%d) %d %d,   Bit%d (%d~%d) %d %d,", \
                    u1BitIdx, FinalWinPerBit[u1BitIdx].first_pass, FinalWinPerBit[u1BitIdx].last_pass, FinalWinPerBit[u1BitIdx].win_size, FinalWinPerBit[u1BitIdx].win_center, \
                    u1BitIdx+8, FinalWinPerBit[u1BitIdx+8].first_pass, FinalWinPerBit[u1BitIdx+8].last_pass, FinalWinPerBit[u1BitIdx+8].win_size, FinalWinPerBit[u1BitIdx+8].win_center));
                #else
                mcSHOW_DBG_MSG(("TX Bit%2d (%2d~%2d) %2d %2d,   Bit%2d (%2d~%2d) %2d %2d,", \
                    u1BitIdx, FinalWinPerBit[u1BitIdx].first_pass, FinalWinPerBit[u1BitIdx].last_pass, FinalWinPerBit[u1BitIdx].win_size, FinalWinPerBit[u1BitIdx].win_center, \
                    u1BitIdx+8, FinalWinPerBit[u1BitIdx+8].first_pass, FinalWinPerBit[u1BitIdx+8].last_pass, FinalWinPerBit[u1BitIdx+8].win_size, FinalWinPerBit[u1BitIdx+8].win_center));
                #endif
                mcFPRINTF((fp_A60501,"TX Bit%2d (%2d~%2d) %2d %2d,   Bit%2d (%2d~%2d) %2d %2d,", \
                    u1BitIdx, FinalWinPerBit[u1BitIdx].first_pass, FinalWinPerBit[u1BitIdx].last_pass, FinalWinPerBit[u1BitIdx].win_size, FinalWinPerBit[u1BitIdx].win_center, \
                    u1BitIdx+8, FinalWinPerBit[u1BitIdx+8].first_pass, FinalWinPerBit[u1BitIdx+8].last_pass, FinalWinPerBit[u1BitIdx+8].win_size, FinalWinPerBit[u1BitIdx+8].win_center));

                if(u1IsLP4Family(p->dram_type))
                {
                    mcSHOW_DBG_MSG(("\n"));
                    mcFPRINTF((fp_A60501,"\n"));
                }
                #if ENABLE_LP3_SW
                else //LPDDR3
                {
                    #ifdef ETT_PRINT_FORMAT
                    mcSHOW_DBG_MSG(("  %d (%d~%d) %d %d,   %d (%d~%d) %d %d\n", \
                        u1BitIdx+16, FinalWinPerBit[u1BitIdx+16].first_pass, FinalWinPerBit[u1BitIdx+16].last_pass, FinalWinPerBit[u1BitIdx+16].win_size, FinalWinPerBit[u1BitIdx+16].win_center, \
                        u1BitIdx+24, FinalWinPerBit[u1BitIdx+24].first_pass, FinalWinPerBit[u1BitIdx+24].last_pass, FinalWinPerBit[u1BitIdx+24].win_size, FinalWinPerBit[u1BitIdx+24].win_center ));
                    #else
                    mcSHOW_DBG_MSG(("  %2d (%2d~%2d) %2d %2d,   %2d (%2d~%2d) %2d %2d\n", \
                        u1BitIdx+16, FinalWinPerBit[u1BitIdx+16].first_pass, FinalWinPerBit[u1BitIdx+16].last_pass, FinalWinPerBit[u1BitIdx+16].win_size, FinalWinPerBit[u1BitIdx+16].win_center, \
                        u1BitIdx+24, FinalWinPerBit[u1BitIdx+24].first_pass, FinalWinPerBit[u1BitIdx+24].last_pass, FinalWinPerBit[u1BitIdx+24].win_size, FinalWinPerBit[u1BitIdx+24].win_center ));
                    #endif

                    mcFPRINTF((fp_A60501,"  %2d (%2d~%2d) %2d %2d,   %2d (%2d~%2d) %2d %2d\n", \
                        u1BitIdx+16, FinalWinPerBit[u1BitIdx+16].first_pass, FinalWinPerBit[u1BitIdx+16].last_pass, FinalWinPerBit[u1BitIdx+16].win_size, FinalWinPerBit[u1BitIdx+16].win_center, \
                        u1BitIdx+24, FinalWinPerBit[u1BitIdx+24].first_pass, FinalWinPerBit[u1BitIdx+24].last_pass, FinalWinPerBit[u1BitIdx+24].win_size, FinalWinPerBit[u1BitIdx+24].win_center ));
                }
                #endif//ENABLE_LP3_SW
            }
            mcSHOW_DBG_MSG(("\n\n"));
            mcFPRINTF((fp_A60501,"\n\n"));
        }
        #if VREF_SPEED_UP_LP4
        #if TX_K_DQM_DEUBG_LOG
        if((u2TempWinSum < (u2tx_window_sum*95/100)) && u1VrefScanEnable)
        #else
        else if((u2TempWinSum < (u2tx_window_sum*95/100)) && u1VrefScanEnable)
        #endif
        {
            mcSHOW_DBG_MSG(("\nTX Vref found, early break! %d< %d\n", u2TempWinSum, (u2tx_window_sum * 95 / 100)));
            break;//max vref found, early break;
        }
        #endif

        #ifdef TX_VREFDQ_EYE_SCAN
        if(u2VrefRange==0 && u2VrefLevel ==50)
        {
            u2VrefRange = 1;
            u2VrefLevel = 20;
        }
        #endif

#if TX_TRIANGLE_EYE_WORKAROUND
        if (tx_triangle_eye_jump_flag== TX_TRIANGLE_EYE_STATE_ADD_MARGIN)
        {
            mcSHOW_DBG_MSG(("[TX_TRIANGLE_EYE] add 10 vref for margin!!\n"));                    
            u2VrefLevel += (10-u2VrefStep); /* minimum margin */
            TXPerbitWin_min_max = 0;
            u2tx_window_sum= 0;
            tx_triangle_eye_jump_flag = TX_TRIANGLE_EYE_STATE_RESTART;
        }
#endif        
    }

    DramcEngine2End(p);

#if SIMPLIFY_VREF_SCAN
    if (u1IsLP4Family(p->dram_type) && (p->enable_tx_scan_vref == ENABLE) && (calType == TX_DQ_DQS_MOVE_DQ_ONLY) && (p->skipVrefScan == DISABLE))
#endif
    {
    #if SUPPORT_SAVE_OFF_LINE_CALIBRATION
        // First TX Vref Scan, need to check window sum
        mcSHOW_DBG_MSG(("\n[TxWindowPerbitCal] SUPPORT_SAVE_OFF_LINE_CALIBRATION  re-calibrate vref "));
        if(gTX_Vref_Scan_Check[p->channel][p->rank][p->dram_fsp] ==0)
        {
            if(u2tx_window_sum < p->pOffLineSaveData->u2TXWinSum_Save[p->channel][p->rank][p->dram_fsp] *9/10)
            {
                // Check Window sum, need to re-calibrate Vref
                gTX_Vref_Scan_Check[p->channel][p->rank][p->dram_fsp] =1;
                mcSHOW_DBG_MSG(("required\n"));

                return DRAM_K_VREF;
            }
            else
            {
                // Check Window sum, "NO" need to re-calibrate Vref
                mcSHOW_DBG_MSG(("not required\n"));
            }

            mcSHOW_DBG_MSG(("tx_window_sum %d , TXWinSum_Save*0.9=%d\n",
                                    u2tx_window_sum, p->pOffLineSaveData->u2TXWinSum_Save[p->channel][p->rank][p->dram_fsp] *9/10));
        }
        else
        {
            // re-calibrate Vref done, update new Vref and Window sum
            p->pOffLineSaveData->u1TXVref_Save[p->channel][p->rank][p->dram_fsp] = ((u2FinalRange<<7) | u2FinalVref);
            p->pOffLineSaveData->u2TXWinSum_Save[p->channel][p->rank][p->dram_fsp] = u2tx_window_sum;
            mcSHOW_DBG_MSG(("done\n"
                            "\nTXVref_Save %d, TXWinSum_Save %d\n",
                                                p->pOffLineSaveData->u1TXVref_Save[p->channel][p->rank][p->dram_fsp],
                                                p->pOffLineSaveData->u2TXWinSum_Save[p->channel][p->rank][p->dram_fsp]));
        }
    #endif
    }

}
            #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
            if(p->pSavetimeData->femmc_Ready==0 || ((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x8)>>3)==1)//save firtst run pass value
            {

                    for (u1ByteIdx=0; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
                    {
                        p->pSavetimeData->u1TxCenter_min_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][u1ByteIdx]=u2Center_min[u1ByteIdx];
                        p->pSavetimeData->u1TxCenter_max_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][u1ByteIdx]=u2Center_max[u1ByteIdx];
                        for (u1BitIdx=0; u1BitIdx<DQS_BIT_NUMBER; u1BitIdx++)
                        {
                            U8 u1BitSave = u1ByteIdx*DQS_BIT_NUMBER+u1BitIdx;
                            p->pSavetimeData->u1Txwin_center_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][u1BitSave]=FinalWinPerBit[u1BitSave].win_center;
                            p->pSavetimeData->u1Txfirst_pass_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][u1BitSave]=FinalWinPerBit[u1BitSave].first_pass;
                            p->pSavetimeData->u1Txlast_pass_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]][u1BitSave]=FinalWinPerBit[u1BitSave].last_pass;
                        }
                    }
               		 
                }
            
                #endif
#ifdef FOR_HQA_TEST_USED
    #if TX_K_DQM_WITH_WDBI
    if(calType == TX_DQ_DQS_MOVE_DQ_ONLY)
    #endif
    {
        gFinalTXPerbitWin_min_max[p->channel][p->rank] = TXPerbitWin_min_max;
    }
#endif

    #if TX_PER_BIT_DELAY_CELL
    // LP3 only use "TX_DQ_DQS_MOVE_DQ_DQM" scan
    // first freq 800(LP4-1600) doesn't support jitter meter(data < 1T), therefore, don't use delay cell
    if((calType == TX_DQ_DQS_MOVE_DQ_ONLY) && (p->frequency >= 1333) && (p->u2DelayCellTimex100!=0))
    {
        u1EnableDelayCell =1;
        mcSHOW_DBG_MSG(("[TX_PER_BIT_DELAY_CELL] DelayCellTimex100 =%d/100 ps\n", p->u2DelayCellTimex100));
    }
    #endif

    //Calculate the center of DQ pass window
    //average the center delay
    for (u1ByteIdx=0; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
    {
        #if TX_PER_BIT_DELAY_CELL
        if(u1EnableDelayCell==0)  
        {   
            uiDelay = ((u2Center_min[u1ByteIdx] + u2Center_max[u1ByteIdx])>>1); //(max +min)/2
            u2TX_DQ_PreCal_LP4[u1ByteIdx] = uiDelay;  //LP4 only, for tDQS2DQ
        }
        else // if(calType == TX_DQ_DQS_MOVE_DQ_ONLY)  //==> LP4 3200 and 2667 only
        {
            uiDelay = u2Center_min[u1ByteIdx];  // for DQ PI delay , will adjust with delay cell
            u2TX_DQ_PreCal_LP4[u1ByteIdx] = ((u2Center_min[u1ByteIdx] + u2Center_max[u1ByteIdx])>>1);  // for DQM PI delay
            
            #if SUPPORT_SAVE_TIME_FOR_CALIBRATION//BYPASS_TX_PER_BIT_DELAY_CELL     
            if(p->pSavetimeData->femmc_Ready==1 && (p->pSavetimeData->Bypass_TXWINDOW[_MappingFreqArray[p->freq_sel]]))
            {	
                U8 u1BitTemp;

            	for (u1BitIdx = 0; u1BitIdx < DQS_BIT_NUMBER; u1BitIdx++)
            	{
            		u1BitTemp = u1ByteIdx*DQS_BIT_NUMBER+u1BitIdx;					  

            		u1DelayCellOfst[u1BitTemp] = p->pSavetimeData->u1TX_PerBit_DelayLine_Save[p->channel][p->rank][p->freq_sel][u1BitTemp];
            	}
            }
            else
            #endif
            {
                // calculate delay cell perbit
                for (u1BitIdx = 0; u1BitIdx < DQS_BIT_NUMBER; u1BitIdx++)
                {
                    U8 u1BitTemp = u1ByteIdx*DQS_BIT_NUMBER+u1BitIdx;
                    U8 u1PIDiff = FinalWinPerBit[u1BitTemp].win_center - u2Center_min[u1ByteIdx];
                    if(p->u2DelayCellTimex100 !=0)
                    {
                        u1DelayCellOfst[u1BitTemp] = (u1PIDiff*100000000/(p->frequency*64))/p->u2DelayCellTimex100;                    
                        #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
                        p->pSavetimeData->u1TX_PerBit_DelayLine_Save[p->channel][p->rank][p->freq_sel][u1BitTemp] = u1DelayCellOfst[u1BitTemp];
                        #endif       
                        mcSHOW_DBG_MSG(("u1DelayCellOfst[%d]=%d cells (%d PI)\n",  u1BitTemp, u1DelayCellOfst[u1BitTemp], u1PIDiff));
                    }
                    else
                    {
                        mcSHOW_ERR_MSG(("Error: Cell time (p->u2DelayCellTimex100) is 0 \n"));
                        break;
                    }
                }
            }
        }   

        #else  //TX_DQM_CALC_MAX_MIN_CENTER
        uiDelay = ((u2Center_min[u1ByteIdx] + u2Center_max[u1ByteIdx])>>1); //(max +min)/2
        if(calType == TX_DQ_DQS_MOVE_DQ_DQM)
        {
            u2TX_DQ_PreCal_LP4[u1ByteIdx] = uiDelay;  //LP4 only, for tDQS2DQ
        }

        #ifdef FOR_HQA_TEST_USED
        U16 u2CenterDiff;
        U8 u1BitTemp;

        gFinalTXPerbitWin_min_margin[p->channel][p->rank] = 0xffff;

        for (u1BitIdx = 0; u1BitIdx < DQS_BIT_NUMBER; u1BitIdx++)
        {
                u1BitTemp = u1ByteIdx*DQS_BIT_NUMBER+u1BitIdx;
                u2CenterDiff= uiDelay- FinalWinPerBit[u1BitTemp].first_pass;  // left margin

            if(u2CenterDiff <gFinalTXPerbitWin_min_margin[p->channel][p->rank])
            {
                gFinalTXPerbitWin_min_margin[p->channel][p->rank] = u2CenterDiff;
                    gFinalTXPerbitWin_min_margin_bit[p->channel][p->rank] =u1BitTemp;
                //mcSHOW_DBG_MSG(("Margin Bit%d, u2CenterDiff %d, Delay %d (%d~%d)\n",  gFinalTXPerbitWin_min_margin_bit[p->channel][p->rank], u2CenterDiff, uiDelay,
                //    FinalWinPerBit[u1ByteIdx *DQS_BIT_NUMBER +u1BitIdx].first_pass, FinalWinPerBit[u1ByteIdx *DQS_BIT_NUMBER +u1BitIdx].last_pass));
            }

            u2CenterDiff= FinalWinPerBit[u1ByteIdx *DQS_BIT_NUMBER +u1BitIdx].last_pass - uiDelay;// right mragin

            if(u2CenterDiff <gFinalTXPerbitWin_min_margin[p->channel][p->rank])
            {
                gFinalTXPerbitWin_min_margin[p->channel][p->rank] = u2CenterDiff;
                    gFinalTXPerbitWin_min_margin_bit[p->channel][p->rank] =u1BitTemp;

                //mcSHOW_DBG_MSG(("margin Bit%d, u2CenterDiff %d, Delay %d (%d~%d)\n", gFinalTXPerbitWin_min_margin_bit[p->channel][p->rank], u2CenterDiff, uiDelay,
                //    FinalWinPerBit[u1ByteIdx *DQS_BIT_NUMBER +u1BitIdx].first_pass, FinalWinPerBit[u1ByteIdx *DQS_BIT_NUMBER +u1BitIdx].last_pass));
            }
        }
        #endif
        #endif

        TxWinTransferDelayToUIPI(p, uiDelay, dq_ui_small_bak, dq_ui_large_bak, &ucdq_final_ui_large[u1ByteIdx], &ucdq_final_ui_small[u1ByteIdx], &ucdq_final_pi[u1ByteIdx]);
        TxWinTransferDelayToUIPI(p, uiDelay, dq_oen_ui_small_bak, dq_oen_ui_large_bak, &ucdq_final_oen_ui_large[u1ByteIdx], &ucdq_final_oen_ui_small[u1ByteIdx], &ucdq_final_pi[u1ByteIdx]);

        #if 1//TX_PER_BIT_DELAY_CELL
        TxWinTransferDelayToUIPI(p, u2TX_DQ_PreCal_LP4[u1ByteIdx] , dq_ui_small_bak, dq_ui_large_bak, &ucdq_final_dqm_ui_large[u1ByteIdx], &ucdq_final_dqm_ui_small[u1ByteIdx], &ucdq_final_dqm_pi[u1ByteIdx]);
        TxWinTransferDelayToUIPI(p, u2TX_DQ_PreCal_LP4[u1ByteIdx] , dq_oen_ui_small_bak, dq_oen_ui_large_bak, &ucdq_final_dqm_oen_ui_large[u1ByteIdx], &ucdq_final_dqm_oen_ui_small[u1ByteIdx], &ucdq_final_dqm_pi[u1ByteIdx]);
        #endif

        #ifdef TX_EYE_SCAN
        aru2TXCaliDelay[p->channel][u1ByteIdx] = (ucdq_final_ui_large[u1ByteIdx]*4+ucdq_final_ui_small[u1ByteIdx])*32+ucdq_final_pi[u1ByteIdx];
        aru2TXCaliDelay_OEN[p->channel][u1ByteIdx] = (ucdq_final_oen_ui_large[u1ByteIdx]*4+ucdq_final_oen_ui_small[u1ByteIdx])*32+ucdq_final_pi[u1ByteIdx];
        #endif

        #if TX_PER_BIT_DELAY_CELL
        mcSHOW_DBG_MSG(("Byte%d, DQ PI dly=%d, DQM PI dly= %d\n",  u1ByteIdx, uiDelay, u2TX_DQ_PreCal_LP4[u1ByteIdx]));
        #else
        mcSHOW_DBG_MSG(("Byte%d, PI DQ dly %d\n",  u1ByteIdx, uiDelay));
        #endif
        mcSHOW_DBG_MSG(("Final DQ PI dly(LargeUI, SmallUI, PI) =(%d ,%d, %d)\n", ucdq_final_ui_large[u1ByteIdx], ucdq_final_ui_small[u1ByteIdx], ucdq_final_pi[u1ByteIdx]));
        mcSHOW_DBG_MSG3(("OEN DQ PI dly(LargeUI, SmallUI, PI) =(%d ,%d, %d)\n\n", ucdq_final_oen_ui_large[u1ByteIdx], ucdq_final_oen_ui_small[u1ByteIdx], ucdq_final_pi[u1ByteIdx]));

        mcFPRINTF((fp_A60501,"Byte%d, PI DQ dly %d\n",  u1ByteIdx, uiDelay));
        mcFPRINTF((fp_A60501,"Final DQ PI dly(LargeUI, SmallUI, PI) =(%d ,%d, %d)\n", ucdq_final_ui_large[u1ByteIdx], ucdq_final_ui_small[u1ByteIdx], ucdq_final_pi[u1ByteIdx]));
        mcFPRINTF((fp_A60501,"OEN DQ PI dly(LargeUI, SmallUI, PI) =(%d ,%d, %d)\n\n", ucdq_final_oen_ui_large[u1ByteIdx], ucdq_final_oen_ui_small[u1ByteIdx], ucdq_final_pi[u1ByteIdx]));

        #if ENABLE_TX_TRACKING
        TxWinAdjustPIToCenter(p, &ucdq_final_ui_large[u1ByteIdx], &ucdq_final_ui_small[u1ByteIdx], &ucdq_final_oen_ui_large[u1ByteIdx], &ucdq_final_oen_ui_small[u1ByteIdx], &ucdq_final_pi[u1ByteIdx]);
        TxWinAdjustPIToCenter(p, &ucdq_final_dqm_ui_large[u1ByteIdx], &ucdq_final_dqm_ui_small[u1ByteIdx], &ucdq_final_dqm_oen_ui_large[u1ByteIdx], &ucdq_final_dqm_oen_ui_small[u1ByteIdx], &ucdq_final_dqm_pi[u1ByteIdx]);

        //if(p->frequency==1600)
        //ucdq_final_pi[u1ByteIdx] -=5;
        mcSHOW_DBG_MSG(("\nNew\n"
                        "Byte%d, PI DQ dly %d\n"
                        "Final DQ PI dly(LargeUI, SmallUI, PI) =(%d ,%d, %d)\n"
                        "OEN DQ PI dly(LargeUI, SmallUI, PI) =(%d ,%d, %d)\n\n",
                            u1ByteIdx, uiDelay,
                            ucdq_final_ui_large[u1ByteIdx], ucdq_final_ui_small[u1ByteIdx], ucdq_final_pi[u1ByteIdx],
                            ucdq_final_oen_ui_large[u1ByteIdx], ucdq_final_oen_ui_small[u1ByteIdx], ucdq_final_pi[u1ByteIdx]));

        mcFPRINTF((fp_A60501, "\nNew\n"
                              "Byte%d, PI DQ dly %d\n"
                              "Final DQ PI dly(LargeUI, SmallUI, PI) =(%d ,%d, %d)\n"
                              "OEN DQ PI dly(LargeUI, SmallUI, PI) =(%d ,%d, %d)\n\n",
                                u1ByteIdx, uiDelay,
                                ucdq_final_ui_large[u1ByteIdx], ucdq_final_ui_small[u1ByteIdx], ucdq_final_pi[u1ByteIdx],
                                ucdq_final_oen_ui_large[u1ByteIdx], ucdq_final_oen_ui_small[u1ByteIdx], ucdq_final_pi[u1ByteIdx]));
        #endif

#ifdef DRAM_CALIB_LOG
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].TxWindowPerbitCal.PI_DQ_elay[u1ByteIdx] = uiDelay;
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].TxWindowPerbitCal.Large_UI[u1ByteIdx] = ucdq_final_ui_large[u1ByteIdx];
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].TxWindowPerbitCal.Small_UI[u1ByteIdx] = ucdq_final_ui_small[u1ByteIdx];
        gDRAM_CALIB_LOG.CHANNEL[p->channel].RANK[p->rank].TxWindowPerbitCal.PI[u1ByteIdx] = ucdq_final_pi[u1ByteIdx];
#endif
    }


#if REG_ACCESS_PORTING_DGB
    RegLogEnable =1;
#endif

        /* p->rank = RANK_0, save to Reg Rank0 and Rank1, p->rank = RANK_1, save to Reg Rank1 */
        for(ii=p->rank; ii<RANK_MAX; ii++)
        {
            vSetRank(p,ii);

            //TXDLY_DQ , TXDLY_OEN_DQ
            if(calType ==TX_DQ_DQS_MOVE_DQ_ONLY || calType== TX_DQ_DQS_MOVE_DQ_DQM)
            {
             vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0), \
                                             P_Fld(ucdq_final_ui_large[0], SHURK0_SELPH_DQ0_TXDLY_DQ0) | \
                                             P_Fld(ucdq_final_ui_large[1], SHURK0_SELPH_DQ0_TXDLY_DQ1) | \
                                             P_Fld(ucdq_final_ui_large[2], SHURK0_SELPH_DQ0_TXDLY_DQ2) | \
                                             P_Fld(ucdq_final_ui_large[3], SHURK0_SELPH_DQ0_TXDLY_DQ3) | \
                                             P_Fld(ucdq_final_oen_ui_large[0], SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0) | \
                                             P_Fld(ucdq_final_oen_ui_large[1], SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1) | \
                                             P_Fld(ucdq_final_oen_ui_large[2], SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2) | \
                                             P_Fld(ucdq_final_oen_ui_large[3], SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3));

             // DLY_DQ[2:0]
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ2), \
                                             P_Fld(ucdq_final_ui_small[0], SHURK0_SELPH_DQ2_DLY_DQ0) | \
                                             P_Fld(ucdq_final_ui_small[1], SHURK0_SELPH_DQ2_DLY_DQ1) | \
                                             P_Fld(ucdq_final_ui_small[2], SHURK0_SELPH_DQ2_DLY_DQ2) | \
                                             P_Fld(ucdq_final_ui_small[3], SHURK0_SELPH_DQ2_DLY_DQ3) | \
                                             P_Fld(ucdq_final_oen_ui_small[0], SHURK0_SELPH_DQ2_DLY_OEN_DQ0) | \
                                             P_Fld(ucdq_final_oen_ui_small[1], SHURK0_SELPH_DQ2_DLY_OEN_DQ1) | \
                                             P_Fld(ucdq_final_oen_ui_small[2], SHURK0_SELPH_DQ2_DLY_OEN_DQ2) | \
                                             P_Fld(ucdq_final_oen_ui_small[3], SHURK0_SELPH_DQ2_DLY_OEN_DQ3));
            }

            #if TX_K_DQM_WITH_WDBI
            if(calType ==TX_DQ_DQS_MOVE_DQM_ONLY || calType== TX_DQ_DQS_MOVE_DQ_DQM ||
                (calType ==TX_DQ_DQS_MOVE_DQ_ONLY && u1VrefScanEnable))
            #endif
            {
             //TXDLY_DQM , TXDLY_OEN_DQM
             vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ1), \
                                         P_Fld(ucdq_final_dqm_ui_large[0], SHURK0_SELPH_DQ1_TXDLY_DQM0) | \
                                         P_Fld(ucdq_final_dqm_ui_large[1], SHURK0_SELPH_DQ1_TXDLY_DQM1) | \
                                         P_Fld(ucdq_final_dqm_ui_large[2], SHURK0_SELPH_DQ1_TXDLY_DQM2) | \
                                         P_Fld(ucdq_final_dqm_ui_large[3], SHURK0_SELPH_DQ1_TXDLY_DQM3) | \
                                         P_Fld(ucdq_final_dqm_oen_ui_large[0], SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0) | \
                                         P_Fld(ucdq_final_dqm_oen_ui_large[1], SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1) | \
                                         P_Fld(ucdq_final_dqm_oen_ui_large[2], SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2) | \
                                         P_Fld(ucdq_final_dqm_oen_ui_large[3], SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3));

             // DLY_DQM[2:0]
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ3), \
                                         P_Fld(ucdq_final_dqm_ui_small[0], SHURK0_SELPH_DQ3_DLY_DQM0) | \
                                         P_Fld(ucdq_final_dqm_ui_small[1], SHURK0_SELPH_DQ3_DLY_DQM1) | \
                                         P_Fld(ucdq_final_dqm_ui_small[2], SHURK0_SELPH_DQ3_DLY_DQM2) | \
                                         P_Fld(ucdq_final_dqm_ui_small[3], SHURK0_SELPH_DQ3_DLY_DQM3) | \
                                         P_Fld(ucdq_final_dqm_oen_ui_small[0], SHURK0_SELPH_DQ3_DLY_OEN_DQM0) | \
                                         P_Fld(ucdq_final_dqm_oen_ui_small[1], SHURK0_SELPH_DQ3_DLY_OEN_DQM1) | \
                                         P_Fld(ucdq_final_dqm_oen_ui_small[2], SHURK0_SELPH_DQ3_DLY_OEN_DQM2) | \
                                         P_Fld(ucdq_final_dqm_oen_ui_small[3], SHURK0_SELPH_DQ3_DLY_OEN_DQM3));
            }

            
             if(u1IsLP4Family(p->dram_type))
             {
                  // update TX DQ PI delay
                 #if TX_K_DQM_WITH_WDBI
                 if(calType ==TX_DQ_DQS_MOVE_DQ_ONLY || calType== TX_DQ_DQS_MOVE_DQ_DQM)
                 {
                     vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ucdq_final_pi[0], SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);
                     vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ7), ucdq_final_pi[1], SHU1_R0_B1_DQ7_RK0_ARPI_DQ_B1);
                 }

                 if(calType ==TX_DQ_DQS_MOVE_DQM_ONLY || calType== TX_DQ_DQS_MOVE_DQ_DQM ||
                    (calType ==TX_DQ_DQS_MOVE_DQ_ONLY && u1VrefScanEnable))
                 {
                     vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ucdq_final_dqm_pi[0], SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0);
                     vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ7), ucdq_final_dqm_pi[1], SHU1_R0_B1_DQ7_RK0_ARPI_DQM_B1);
                 }
                 
                 #else
                 vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), \
                                                     P_Fld(ucdq_final_pi[0], SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0) | \
                                                             P_Fld(ucdq_final_dqm_pi[0], SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0));

                 vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ7), \
                                                     P_Fld(ucdq_final_pi[1], SHU1_R0_B1_DQ7_RK0_ARPI_DQ_B1) | \
                                                             P_Fld(ucdq_final_dqm_pi[1], SHU1_R0_B1_DQ7_RK0_ARPI_DQM_B1));
                 #endif

                 #if TX_PER_BIT_DELAY_CELL
                 // LP3 only use "TX_DQ_DQS_MOVE_DQ_DQM" scan
                 // first freq 800(LP4-1600) doesn't support jilter meter, therefore, don't use delay cell
                 #if TX_K_DQM_WITH_WDBI
                 if(calType ==TX_DQ_DQS_MOVE_DQ_ONLY || calType== TX_DQ_DQS_MOVE_DQ_DQM)
                 #endif
                 {
                     if(u1EnableDelayCell)
                     {
                         vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ0),
                                    P_Fld(u1DelayCellOfst[7] , SHU1_R0_B0_DQ0_RK0_TX_ARDQ7_DLY_B0)
                                 | P_Fld(u1DelayCellOfst[6] , SHU1_R0_B0_DQ0_RK0_TX_ARDQ6_DLY_B0)
                                 | P_Fld(u1DelayCellOfst[5] , SHU1_R0_B0_DQ0_RK0_TX_ARDQ5_DLY_B0)
                                 | P_Fld(u1DelayCellOfst[4] , SHU1_R0_B0_DQ0_RK0_TX_ARDQ4_DLY_B0)
                                 | P_Fld(u1DelayCellOfst[3] , SHU1_R0_B0_DQ0_RK0_TX_ARDQ3_DLY_B0)
                                 | P_Fld(u1DelayCellOfst[2] , SHU1_R0_B0_DQ0_RK0_TX_ARDQ2_DLY_B0)
                                 | P_Fld(u1DelayCellOfst[1] , SHU1_R0_B0_DQ0_RK0_TX_ARDQ1_DLY_B0)
                                 | P_Fld(u1DelayCellOfst[0] , SHU1_R0_B0_DQ0_RK0_TX_ARDQ0_DLY_B0));
                         vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ0),
                                    P_Fld(u1DelayCellOfst[15] , SHU1_R0_B1_DQ0_RK0_TX_ARDQ7_DLY_B1)
                                 | P_Fld(u1DelayCellOfst[14] , SHU1_R0_B1_DQ0_RK0_TX_ARDQ6_DLY_B1)
                                 | P_Fld(u1DelayCellOfst[13] , SHU1_R0_B1_DQ0_RK0_TX_ARDQ5_DLY_B1)
                                 | P_Fld(u1DelayCellOfst[12] , SHU1_R0_B1_DQ0_RK0_TX_ARDQ4_DLY_B1)
                                 | P_Fld(u1DelayCellOfst[11] , SHU1_R0_B1_DQ0_RK0_TX_ARDQ3_DLY_B1)
                                 | P_Fld(u1DelayCellOfst[10] , SHU1_R0_B1_DQ0_RK0_TX_ARDQ2_DLY_B1)
                                 | P_Fld(u1DelayCellOfst[9] , SHU1_R0_B1_DQ0_RK0_TX_ARDQ1_DLY_B1)
                                 | P_Fld(u1DelayCellOfst[8] , SHU1_R0_B1_DQ0_RK0_TX_ARDQ0_DLY_B1));
                     }
                 }
                 #endif

                #if ENABLE_TX_TRACKING
                #if TX_K_DQM_WITH_WDBI
                 if(calType == TX_DQ_DQS_MOVE_DQ_ONLY ||calType ==TX_DQ_DQS_MOVE_DQM_ONLY)
                #else
                if(calType == TX_DQ_DQS_MOVE_DQ_ONLY)
                #endif
                 {
                     //make a copy to dramc reg for TX DQ tracking used
                     #if TX_K_DQM_WITH_WDBI
                     if(calType ==TX_DQ_DQS_MOVE_DQ_ONLY || calType== TX_DQ_DQS_MOVE_DQ_DQM)
                     {
                        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU1RK0_PI), \
                                        P_Fld(ucdq_final_pi[0], SHU1RK0_PI_RK0_ARPI_DQ_B0) | P_Fld(ucdq_final_pi[1], SHU1RK0_PI_RK0_ARPI_DQ_B1));
                     }

                     if(calType ==TX_DQ_DQS_MOVE_DQM_ONLY || calType== TX_DQ_DQS_MOVE_DQ_DQM ||
                        (calType ==TX_DQ_DQS_MOVE_DQ_ONLY && u1VrefScanEnable))
                     {
                        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU1RK0_PI), \
                                        P_Fld(ucdq_final_dqm_pi[0], SHU1RK0_PI_RK0_ARPI_DQM_B0) | P_Fld(ucdq_final_dqm_pi[1], SHU1RK0_PI_RK0_ARPI_DQM_B1));
                     }
                     #else
                      vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU1RK0_PI), \
                                                      P_Fld(ucdq_final_pi[0], SHU1RK0_PI_RK0_ARPI_DQ_B0) | P_Fld(ucdq_final_pi[1], SHU1RK0_PI_RK0_ARPI_DQ_B1) | \
                                                      P_Fld(ucdq_final_dqm_pi[0], SHU1RK0_PI_RK0_ARPI_DQM_B0) | P_Fld(ucdq_final_dqm_pi[1], SHU1RK0_PI_RK0_ARPI_DQM_B1));
                     #endif

                     //u2TXTrackingTmpValue[0] = (((ucdq_final_ui_large[0] <<3) + ucdq_final_ui_small[0]) <<5) + ucdq_final_pi[0];
                     //u2TXTrackingTmpValue[1] = (((ucdq_final_ui_large[1] <<3) + ucdq_final_ui_small[1]) <<5) + ucdq_final_pi[1];
                     //bug fix
                     u2TXTrackingTmpValue_DQ[0] = ucdq_final_pi[0];
                     u2TXTrackingTmpValue_DQ[1] = ucdq_final_pi[1];
                     u2TXTrackingTmpValue_DQM[0] = ucdq_final_dqm_pi[0];
                     u2TXTrackingTmpValue_DQM[1] = ucdq_final_dqm_pi[1];

                     #if TX_K_DQM_WITH_WDBI
                     if(calType ==TX_DQ_DQS_MOVE_DQ_ONLY || calType== TX_DQ_DQS_MOVE_DQ_DQM)
                     #endif
                     {
                         // Source DQ
                         vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU1RK0_DQS2DQ_CAL1),
                                                                     P_Fld(u2TXTrackingTmpValue_DQ[1], SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ1) |
                                                                     P_Fld(u2TXTrackingTmpValue_DQ[0], SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ0));
                         // Target DQ
                          vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU1RK0_DQS2DQ_CAL2),
                                                                      P_Fld(u2TXTrackingTmpValue_DQ[1], SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ1) |
                                                                      P_Fld(u2TXTrackingTmpValue_DQ[0], SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ0));
                     }

                     #if TX_K_DQM_WITH_WDBI
                     if(calType ==TX_DQ_DQS_MOVE_DQM_ONLY || calType== TX_DQ_DQS_MOVE_DQ_DQM ||
                        (calType ==TX_DQ_DQS_MOVE_DQ_ONLY && u1VrefScanEnable))
                     #endif
                     {
                        // Target DQM
                        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU1RK0_DQS2DQ_CAL5),
                                                              P_Fld(u2TXTrackingTmpValue_DQM[1], SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM1) |
                                                              P_Fld(u2TXTrackingTmpValue_DQM[0], SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM0));
                     }

                     // Target DQ/DQM OEN not used after bug fix
                     ///TODO: Jeremy need to check this. 2016/08/11
                     u2TXTrackingTmpValue_DQ[0] = (((ucdq_final_oen_ui_large[0] <<3) + ucdq_final_oen_ui_small[0]) <<5) + ucdq_final_pi[0];
                     u2TXTrackingTmpValue_DQ[1] = (((ucdq_final_oen_ui_large[1] <<3) + ucdq_final_oen_ui_small[1]) <<5) + ucdq_final_pi[1];
                     u2TXTrackingTmpValue_DQM[0] = (((ucdq_final_dqm_oen_ui_large[0] <<3) + ucdq_final_dqm_oen_ui_small[0]) <<5) + ucdq_final_dqm_pi[0];
                     u2TXTrackingTmpValue_DQM[1] = (((ucdq_final_dqm_oen_ui_large[1] <<3) + ucdq_final_dqm_oen_ui_small[1]) <<5) + ucdq_final_dqm_pi[1];

                     // Target DQ OEN
                     #if TX_K_DQM_WITH_WDBI
                     if(calType ==TX_DQ_DQS_MOVE_DQ_ONLY || calType== TX_DQ_DQS_MOVE_DQ_DQM)
                     #endif
                     {
                         vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU1RK0_DQS2DQ_CAL3),
                                                             P_Fld(u2TXTrackingTmpValue_DQ[1] & 0x1f, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1_B4TO0) |
                                                             P_Fld(u2TXTrackingTmpValue_DQ[0] & 0x1f, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0_B4TO0) |
                                                             P_Fld(u2TXTrackingTmpValue_DQ[1] >> 5, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1) |
                                                             P_Fld(u2TXTrackingTmpValue_DQ[0] >> 5, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0));
                     }
                     
                     // Target DQM OEN
                     #if TX_K_DQM_WITH_WDBI
                     if(calType ==TX_DQ_DQS_MOVE_DQM_ONLY || calType== TX_DQ_DQS_MOVE_DQ_DQM ||
                        (calType ==TX_DQ_DQS_MOVE_DQ_ONLY && u1VrefScanEnable))
                     #endif
                     {
                         vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU1RK0_DQS2DQ_CAL4),
                                                             P_Fld(u2TXTrackingTmpValue_DQM[1] & 0x1f, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1_B4TO0) |
                                                             P_Fld(u2TXTrackingTmpValue_DQM[0] & 0x1f, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0_B4TO0) |
                                                             P_Fld(u2TXTrackingTmpValue_DQM[1] >> 5, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1) |
                                                             P_Fld(u2TXTrackingTmpValue_DQM[0] >> 5, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0));
                     }
                 }
                #endif //End ENABLE_TX_TRACKING
            }
            #if ENABLE_LP3_SW
            else
            {
                vIO32WriteFldAlign_Phy_Byte(0, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ucdq_final_pi[0], SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);
                vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ucdq_final_pi[1], SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);
                vIO32WriteFldAlign_Phy_Byte(2, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ucdq_final_pi[2], SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);
                vIO32WriteFldAlign_Phy_Byte(3, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ucdq_final_pi[3], SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);
                
                #if (FOR_DV_SIMULATION_USED!=0)
                vIO32WriteFldAlign_Phy_Byte(0, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ucdq_final_pi[0], SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0);
                vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ucdq_final_pi[1], SHU1_R0_CA_CMD9_RG_RK0_ARPI_CS);
                vIO32WriteFldAlign_Phy_Byte(2, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ucdq_final_pi[2], SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0);
                vIO32WriteFldAlign_Phy_Byte(3, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ucdq_final_pi[3], SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0);
                #else
                vIO32WriteFldAlign_Phy_Byte(0, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ucdq_final_pi[0], SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0);
                vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ucdq_final_pi[1], SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0);
                vIO32WriteFldAlign_Phy_Byte(2, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ucdq_final_pi[2], SHU1_R0_CA_CMD9_RG_RK0_ARPI_CS);
                vIO32WriteFldAlign_Phy_Byte(3, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ucdq_final_pi[3], SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0);
                #endif
                
                #if 0// for LP3 , TX tracking will be disable, don't need to set DQ delay in DramC.
                ///TODO: check LP3 byte mapping of dramC
                vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_PI+(CHANNEL_A<< POS_BANK_NUM), \
                                         P_Fld(ucdq_final_pi[0], SHU1RK0_PI_RK0_ARPI_DQ_B0) | P_Fld(ucdq_final_pi[1], SHU1RK0_PI_RK0_ARPI_DQ_B1));

                vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_PI+(CHANNEL_B<< POS_BANK_NUM), \
                                         P_Fld(ucdq_final_pi[2], SHU1RK0_PI_RK0_ARPI_DQ_B0) | P_Fld(ucdq_final_pi[3], SHU1RK0_PI_RK0_ARPI_DQ_B1));
                #endif
            }
            #endif // end ENABLE_LP3_SW
        }
        
        vSetRank(p, backup_rank);


     // SET tx Vref (DQ) = u2FinalVref, LP3 no need to set this.
    if(u1VrefScanEnable)
    {
#if (SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_VREF_CAL)
 if(p->pSavetimeData->femmc_Ready==1 && (p->pSavetimeData->Bypass_TXWINDOW[_MappingFreqArray[p->freq_sel]]))
 	{
        	u2FinalVref = p->pSavetimeData->u1TxWindowPerbitVref_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]];                		
        u1MR14Value[p->channel][p->rank][p->dram_fsp] = (u2FinalVref | (u2FinalRange<<6));
        DramcModeRegWriteByRank(p, p->rank, 14, u2FinalVref | (u2FinalRange<<6));
        #if VENDER_JV_LOG
        if(calType ==TX_DQ_DQS_MOVE_DQ_ONLY)
        {
            mcSHOW_DBG_MSG5(("\nFinal TX Range %d Vref %d\n\n", u2FinalRange, u2FinalVref));
        }
        #endif
        #ifdef FOR_HQA_TEST_USED
        gFinalTXVrefDQ[p->channel][p->rank] = u2FinalVref;
        #endif
    }
 else 	
 	#endif
 	{		
        u1MR14Value[p->channel][p->rank][p->dram_fsp] = (u2FinalVref | (u2FinalRange<<6));
        DramcModeRegWriteByRank(p, p->rank, 14, u2FinalVref | (u2FinalRange<<6));

        mcSHOW_DBG_MSG(("\nFinal TX Range %d Vref %d\n\n", u2FinalRange, u2FinalVref));
        mcFPRINTF((fp_A60501, "\nFinal TX Range %d Vref %d\n\n", u2FinalRange, u2FinalVref));

        #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
              if(p->pSavetimeData->femmc_Ready==0 || ((p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]] & 0x8)>>3)==1)////save firtst run Vref value
              {
			p->pSavetimeData->u1TxWindowPerbitVref_Save[p->channel][p->rank][_MappingFreqArray[p->freq_sel]]=u2FinalVref;
              }
	  #endif
	  
        #if VENDER_JV_LOG
        if(calType ==TX_DQ_DQS_MOVE_DQ_ONLY)
        {
            mcSHOW_DBG_MSG5(("\nFinal TX Range %d Vref %d\n\n", u2FinalRange, u2FinalVref));
        }
        #endif
        
        #ifdef FOR_HQA_TEST_USED
        #if TX_K_DQM_WITH_WDBI
        if(calType == TX_DQ_DQS_MOVE_DQ_ONLY)
        #endif
        {
            gFinalTXVrefDQ[p->channel][p->rank] = u2FinalVref;
        }
        #endif
    }
}
    
    // BU request RX & TX window size log.
    #if 0//def RELEASE  // for parsing tool
    if(calType ==TX_DQ_DQS_MOVE_DQ_ONLY)
    {
        mcSHOW_DBG_MSG4(("TX CH%d R%d ,Freq %d\n", p->channel, p->rank, p->frequency));
        for (u1BitIdx = 0; u1BitIdx < p->data_width; u1BitIdx++)
        {
            mcSHOW_DBG_MSG4(("%d: %d\n", u1BitIdx, gFinalTXPerbitWin[p->channel][p->rank][u1BitIdx]));
        }
    }
    #endif

#if REG_ACCESS_PORTING_DGB
    RegLogEnable =0;
#endif

    mcSHOW_DBG_MSG3(("[TxWindowPerbitCal] Done\n\n"));
    mcFPRINTF((fp_A60501, "[TxWindowPerbitCal] Done\n\n"));

    #if 0
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_PADCTL4), 1, PADCTL4_CKEFIXON);  // test only
    #endif

#if TX_K_DQM_WITH_WDBI
    U16 u2TXMinWindow=0xffff;
    if(u1IsLP4Family(p->dram_type))
    {
        if(calType==TX_DQ_DQS_MOVE_DQM_ONLY && u1VrefScanEnable==0)
        {
            mcSHOW_DBG_MSG(("\n[TX_K_DQM_WITH_WDBI] Vref=%d,TXPerbitWin_min_max = %d PI, ", u1MR14Value[p->channel][p->rank][p->dram_fsp], TXPerbitWin_min_max));

            #if 1//marked for test
            if(TXPerbitWin_min_max > 18)
            {
                mcSHOW_DBG_MSG(("spec in\n\n"));
                return TX_DQM_WINDOW_SPEC_IN;
            }
            else
            #endif
            {
                mcSHOW_DBG_MSG(("spec out\n\n"));
                return TX_DQM_WINDOW_SPEC_OUT;
            }
        }
        else if(u1VrefScanEnable==1)
            return u2FinalVref;
    }
#endif

    return DRAM_OK;

    // log example
    /*
TX calibration finding left boundary early break. PI DQ delay=0x3e
TX calibration finding right boundary early break. PI DQ delay=0x1c
==================================================================
    TX DQS perbit delay software calibration v3
    channel=2(2:cha, 3:chb)  apply = 1
==================================================================
PI DQ (per byte) window
x=pass dq delay value (min~max)center
y=0-7bit DQ of every group
input delay:Byte0 = 13 Byte1 = 13 Byte2 = 12 Byte3 = 13
==================================================================
bit    Byte0    bit    Byte1    bit    Byte2    bit    Byte3
 0   ( 1~26)13,  8   ( 3~26)14, 16   ( 2~27)14, 24   ( 3~26)14
 1   ( 2~26)14,  9   ( 1~24)12, 17   ( 0~25)12, 25   ( 4~26)15
 2   ( 3~25)14, 10   ( 2~26)14, 18   ( 2~25)13, 26   ( 3~27)15
 3   ( 2~24)13, 11   ( 1~23)12, 19   ( 3~25)14, 27   ( 1~23)12
 4   ( 3~26)14, 12   ( 2~26)14, 20   ( 0~24)12, 28   ( 1~25)13
 5   ( 3~25)14, 13   ( 2~25)13, 21   (-1~25)12, 29   ( 2~24)13
 6   ( 2~26)14, 14   ( 2~24)13, 22   (-1~26)12, 30   ( 3~27)15
 7   ( 1~25)13, 15   ( 2~26)14, 23   (-1~22)10, 31   ( 2~26)14

==================================================================
   */
}
#endif //SIMULATION_TX_PERBIT

#ifdef FOR_HQA_TEST_USED
void HQA_measure_message_reset_all_data(DRAMC_CTX_T *p)
{
    U32 uiCA, u1BitIdx, u1ByteIdx, u1RankIdx, u1ChannelIdx;

    for(u1ChannelIdx=CHANNEL_A; u1ChannelIdx<CHANNEL_NUM; u1ChannelIdx++)
    {
        for(u1RankIdx=RANK_0; u1RankIdx<RANK_MAX; u1RankIdx++)
        {
            for (u1ByteIdx=0; u1ByteIdx<p->data_width/DQS_BIT_NUMBER; u1ByteIdx++)
            {
                gFinalGatingWin[u1ChannelIdx][u1RankIdx][u1ByteIdx] =0;
            }
            
            for (u1BitIdx=0; u1BitIdx<p->data_width; u1BitIdx++)
            {
                gFinalRXPerbitWin[u1ChannelIdx][u1RankIdx][u1BitIdx] =0;        
                gFinalTXPerbitWin[u1ChannelIdx][u1RankIdx][u1BitIdx] =0;
            }
        }
    }
}
#endif

#ifdef FOR_HQA_TEST_USED
#ifdef RELEASE
#undef mcSHOW_DBG_MSG(_x_)
#define mcSHOW_DBG_MSG(_x_)	opt_print _x_
#endif
static U8 get_HQA_shuffleIdx(DRAMC_CTX_T *p)
{
    u8 shuffleIdx = 0;

    /* Retrieve shuffle number from gFreqTbl */
    for (shuffleIdx = 0; shuffleIdx < DRAM_DFS_SHUFFLE_MAX; shuffleIdx++)
    {
        if (gFreqTbl[shuffleIdx].frequency == p->frequency)
        {
            break;
        }
    }

    if (shuffleIdx == DRAM_DFS_SHUFFLE_MAX)
    {
        mcSHOW_ERR_MSG(("shuffle num err!\n"));
    }
    else
    {
        mcSHOW_DBG_MSG3(("shuffleIdx %d\n", shuffleIdx));
    }

    return shuffleIdx;
}

void print_HQA_measure_message(DRAMC_CTX_T *p)
{
    U32 uiCA, u1BitIdx, u1ByteIdx, u1RankIdx, u1ChannelIdx;
    U32 min_ca_value[CHANNEL_NUM][RANK_MAX], min_ca_bit[CHANNEL_NUM][RANK_MAX];
    U32 min_gating_value[CHANNEL_NUM][RANK_MAX], min_rx_value[CHANNEL_NUM][RANK_MAX], min_tx_value[CHANNEL_NUM][RANK_MAX];
    U32 min_RX_DQ_bit[CHANNEL_NUM][RANK_MAX], min_TX_DQ_bit[CHANNEL_NUM][RANK_MAX];
    U8 shuffleIdx;

    mcSHOW_DBG_MSG(("\n\n\n[HQA] information for measurement, "));
    mcSHOW_DBG_MSG(("\tDram Data rate = %d\n",p->frequency*2));
    vPrintCalibrationBasicInfo(p);

    shuffleIdx = get_HQA_shuffleIdx(p);

    for(u1ChannelIdx=CHANNEL_A; u1ChannelIdx<p->support_channel_num; u1ChannelIdx++)
    {
        for(u1RankIdx=RANK_0; u1RankIdx<RANK_MAX; u1RankIdx++)
        {
            min_ca_value[u1ChannelIdx][u1RankIdx] = 0xffff;
            min_gating_value[u1ChannelIdx][u1RankIdx] = 0xffff;
            min_rx_value[u1ChannelIdx][u1RankIdx] = 0xffff;
            min_tx_value[u1ChannelIdx][u1RankIdx] = 0xffff;
            min_RX_DQ_bit[u1ChannelIdx][u1RankIdx] = 0xffff;
            min_TX_DQ_bit[u1ChannelIdx][u1RankIdx] = 0xffff;

            for (uiCA=0; uiCA<((u1IsLP4Family(p->dram_type)==1) ? CATRAINING_NUM_LP4 : CATRAINING_NUM_LP3); uiCA++)
            {
                if (gFinalCBTCA[u1ChannelIdx][u1RankIdx][uiCA] < min_ca_value[u1ChannelIdx][u1RankIdx])
                {
                    min_ca_value[u1ChannelIdx][u1RankIdx] = gFinalCBTCA[u1ChannelIdx][u1RankIdx][uiCA];
                    min_ca_bit[u1ChannelIdx][u1RankIdx] = uiCA;
                }
            }

            for (u1ByteIdx=0; u1ByteIdx<p->data_width/DQS_BIT_NUMBER; u1ByteIdx++)
            {
                if (gFinalGatingWin[u1ChannelIdx][u1RankIdx][u1ByteIdx] < min_gating_value[u1ChannelIdx][u1RankIdx])
                    min_gating_value[u1ChannelIdx][u1RankIdx] = gFinalGatingWin[u1ChannelIdx][u1RankIdx][u1ByteIdx];
            }

            for (u1BitIdx=0; u1BitIdx<p->data_width; u1BitIdx++)
            {
                if (gFinalRXPerbitWin[u1ChannelIdx][u1RankIdx][u1BitIdx] < min_rx_value[u1ChannelIdx][u1RankIdx])
                {
                    min_rx_value[u1ChannelIdx][u1RankIdx] = gFinalRXPerbitWin[u1ChannelIdx][u1RankIdx][u1BitIdx];
                    min_RX_DQ_bit[u1ChannelIdx][u1RankIdx] = u1BitIdx;
                }
                if (gFinalTXPerbitWin[u1ChannelIdx][u1RankIdx][u1BitIdx] < min_tx_value[u1ChannelIdx][u1RankIdx])
                {
                    min_tx_value[u1ChannelIdx][u1RankIdx] = gFinalTXPerbitWin[u1ChannelIdx][u1RankIdx][u1BitIdx];
                    min_TX_DQ_bit[u1ChannelIdx][u1RankIdx] = u1BitIdx;
                }
            }
        }
    }


#ifndef DUAL_RANK_RX_K
    if (p->support_rank_num==RANK_DUAL)
    {
        //Preloader LP3 RX/TX only K Rank0, so Rank1 use Rank0's value
        if(!u1IsLP4Family(p->dram_type))
        {
            min_rx_value[0][1] = min_rx_value[0][0];
            min_RX_DQ_bit[0][1] = min_RX_DQ_bit[0][0];

            min_tx_value[0][1] = min_tx_value[0][0];
            gFinalTXPerbitWin_min_max[0][1] = gFinalTXPerbitWin_min_max[0][0];
            min_TX_DQ_bit[0][1] = min_TX_DQ_bit[0][0];

            #if(TX_PER_BIT_DELAY_CELL==0)
            gFinalTXPerbitWin_min_margin[0][1] = gFinalTXPerbitWin_min_margin[0][0];
            gFinalTXPerbitWin_min_margin_bit[0][1] = gFinalTXPerbitWin_min_margin_bit[0][0];
            #endif
        }
    }
#endif

    if(u1IsLP4Family(p->dram_type))
    {
        mcSHOW_DBG_MSG(("[Cmd Bus Training window]\n"));
        mcSHOW_DBG_MSG(("VrefCA Range : %d\n", gCBT_VREF_RANGE_SEL));
#if CHANNEL_NUM==4
        /*
         * VrefCA
         * Channel A               B               C               D
         * Rank0   29              33              37              31
         * Rank1   31              29              29              31
         */
        mcSHOW_DBG_MSG(("\nVrefCA\n"
                        "Channel\tA\t\tB\t\tC\t\tD\n"
                        "Rank0\t%d\t\t%d\t\t%d\t\t%d\n"
                        "Rank1\t%d\t\t%d\t\t%d\t\t%d\n\n",
                        gFinalCBTVrefCA[0][0], gFinalCBTVrefCA[1][0], gFinalCBTVrefCA[2][0], gFinalCBTVrefCA[3][0],
                        gFinalCBTVrefCA[0][1], gFinalCBTVrefCA[1][1], gFinalCBTVrefCA[2][1], gFinalCBTVrefCA[3][1]));

#if (SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_CBT)
 if(p->pSavetimeData->femmc_Ready==1 )
 	{
    mcSHOW_DBG_MSG(("\n[Cmd Bus Training window bypass calibration]\n"));
 	}
 else

#endif
{
        /*
         * CA_Window
         * Channel A               B               C               D
         * Rank0   58(bit 1)       60(bit 0)       60(bit 3)       58(bit 0)
         * Rank1   58(bit 0)       61(bit 0)       60(bit 4)       59(bit 5)
         */
        mcSHOW_DBG_MSG(("CA_Window\n"
                        "Channel\tA\t\tB\t\tC\t\tD\n"
                        "Rank0\t%d(bit %d)\t%d(bit %d)\t%d(bit %d)\t%d(bit %d)\n"
                        "Rank1\t%d(bit %d)\t%d(bit %d)\t%d(bit %d)\t%d(bit %d)\n\n",
                        min_ca_value[0][0], min_ca_bit[0][0], min_ca_value[1][0], min_ca_bit[1][0], min_ca_value[2][0], min_ca_bit[2][0], min_ca_value[3][0], min_ca_bit[3][0],
                        min_ca_value[0][1], min_ca_bit[0][1], min_ca_value[1][1], min_ca_bit[1][1], min_ca_value[2][1], min_ca_bit[2][1], min_ca_value[3][1], min_ca_bit[3][1]));
#ifdef DRAM_HQA_USED
        /* Min Window(%)
         * Channel A               B               C               D
         * Rank0   91%(PASS)       94%(PASS)       94%(PASS)       91%(PASS)
         * Rank1   91%(PASS)       96%(PASS)       94%(PASS)       93%(PASS)
         */
        mcSHOW_DBG_MSG(("Min Window(%%)\n"
                        "Channel\tA\t\tB\n"));
        for(u1RankIdx = RANK_0; u1RankIdx < 2; u1RankIdx++)
        {
            mcSHOW_DBG_MSG(("Rank%d\t", u1RankIdx));
            for(u1ChannelIdx = CHANNEL_A; u1ChannelIdx < p->support_channel_num; u1ChannelIdx++)
            {
                mcSHOW_DBG_MSG(("%d%%(%s)\t",
                                ((min_ca_value[u1ChannelIdx][u1RankIdx] * 100 + 63) / 64),
                                ((((min_ca_value[u1ChannelIdx][u1RankIdx] * 100 + 63) / 64) >= 30) ? "PASS" : "FAIL")));
            }
            mcSHOW_DBG_MSG(("\n"));
        }
#endif
}
#else //CHANNEL_NUM==4
        mcSHOW_DBG_MSG(("\nVrefCA\n"
                        "Channel\tA\t\tB\n"
                        "Rank0\t%d\t\t%d\n"
                        "Rank1\t%d\t\t%d\n\n",
                        gFinalCBTVrefCA[0][0], gFinalCBTVrefCA[1][0],
                        gFinalCBTVrefCA[0][1], gFinalCBTVrefCA[1][1]));

#if (SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_CBT)
 if(p->pSavetimeData->femmc_Ready==1 )
 	{
    mcSHOW_DBG_MSG(("\n[Cmd Bus Training window bypass calibration]\n"));
 	}
 else

#endif
{
        mcSHOW_DBG_MSG(("CA_Window\n"
                        "Channel\tA\t\tB\n"
                        "Rank0\t%d(bit %d)\t%d(bit %d)\n"
                        "Rank1\t%d(bit %d)\t%d(bit %d)\n\n",
                        min_ca_value[0][0], min_ca_bit[0][0], min_ca_value[1][0], min_ca_bit[1][0],
                        min_ca_value[0][1], min_ca_bit[0][1], min_ca_value[1][1], min_ca_bit[1][1]));
}
#if (SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_CBT)
 if(p->pSavetimeData->femmc_Ready==1 )
 	{
    mcSHOW_DBG_MSG(("\n[Cmd Bus Training window bypass calibration]\n"));
 	}
 else

#endif
{
#ifdef DRAM_HQA_USED
        mcSHOW_DBG_MSG(("Min Window(%%)\n"
                        "Channel\tA\t\tB\n"));
        for(u1RankIdx = RANK_0; u1RankIdx < 2; u1RankIdx++)
        {
            mcSHOW_DBG_MSG(("Rank%d\t", u1RankIdx));
            for(u1ChannelIdx = CHANNEL_A; u1ChannelIdx < 2; u1ChannelIdx++)
            {
                mcSHOW_DBG_MSG(("%d%%(%s)\t",
                                ((min_ca_value[u1ChannelIdx][u1RankIdx] * 100 + 63) / 64),
                                ((((min_ca_value[u1ChannelIdx][u1RankIdx] * 100 + 63) / 64) >= 30) ? "PASS" : "FAIL")));
            }
            mcSHOW_DBG_MSG(("\n"));
        }
#endif
    	}
#endif
    }
    else
    {
        mcSHOW_DBG_MSG(("Min Window(%%)\n"
                        "Channel\tA\t\tB\n"));
        for(u1RankIdx = RANK_0; u1RankIdx < 1; u1RankIdx++)
        {
            mcSHOW_DBG_MSG(("Rank%d\t", u1RankIdx));
            for(u1ChannelIdx = CHANNEL_A; u1ChannelIdx < 1; u1ChannelIdx++)
            {
                mcSHOW_DBG_MSG(("%d%%(%s)\t",
                                ((min_ca_value[u1ChannelIdx][u1RankIdx] * 100 + 63) / 64),
                                ((((min_ca_value[u1ChannelIdx][u1RankIdx] * 100 + 63) / 64) >= 30) ? "PASS" : "FAIL")));
            }
            mcSHOW_DBG_MSG(("\n"));
        }
    }

    /* [Gating minimum per byte window]
     * Channel A               B               C               D
     * Rank0   148             144             164             188
     * Rank1   148             148             164             188
     */
if (min_gating_value[0][0]!=0)
{
    mcSHOW_DBG_MSG(("\n[Gating minimum per byte window]\n"));
#if CHANNEL_NUM==4
    mcSHOW_DBG_MSG(("Channel\tA\t\tB\t\tC\t\tD\n"
                    "Rank0\t%d\t\t%d\t\t%d\t\t%d\n"
                    "Rank1\t%d\t\t%d\t\t%d\t\t%d\n\n",
                    min_gating_value[0][0], min_gating_value[1][0], min_gating_value[2][0], min_gating_value[3][0],
                    min_gating_value[0][1], min_gating_value[1][1], min_gating_value[2][1], min_gating_value[3][1]));
#else
    mcSHOW_DBG_MSG(("Channel\tA\t\tB\n"
                    "Rank0\t%d\t\t%d\n"
                    "Rank1\t%d\t\t%d\n\n",
                    min_gating_value[0][0], min_gating_value[1][0],
                    min_gating_value[0][1], min_gating_value[1][1]));
#endif
}

    /* [RX minimum per bit window]
     * VrefDQ
     * Channel A               B               C               D
     *         4               4               4               4
     */
    mcSHOW_DBG_MSG(("\n[RX minimum per bit window]\n"));
    if (p->enable_rx_scan_vref == ENABLE)
    {
#if CHANNEL_NUM==4
        mcSHOW_DBG_MSG(("VrefDQ\n"
                        "Channel\tA\t\tB\t\tC\t\tD\n"
                        "\t%d\t\t%d\t\t%d\t\t%d\n\n",
                        gFinalRXVrefDQ[CHANNEL_A], gFinalRXVrefDQ[CHANNEL_B], gFinalRXVrefDQ[CHANNEL_C], gFinalRXVrefDQ[CHANNEL_D]));
#else
        mcSHOW_DBG_MSG(("VrefDQ\n"
                        "Channel\tA\t\tB\n"
                        "\t%d\t\t%d\n\n",
                        gFinalRXVrefDQ[CHANNEL_A], gFinalRXVrefDQ[CHANNEL_B]));
#endif
    }
    else
    {
        mcSHOW_DBG_MSG(("RX DQ Vref Scan : Disable\n"));

    }
#if (SUPPORT_SAVE_TIME_FOR_CALIBRATION )
 if(p->pSavetimeData->femmc_Ready==1 && ( p->pSavetimeData->Bypass_RXWINDOW[_MappingFreqArray[p->freq_sel]]))
 	{
    mcSHOW_DBG_MSG(("\n[RX minimum per bit window bypass calibration]\n"));
 	}
 else

#endif
{
#if CHANNEL_NUM==4
    /* Channel A               B               C               D
     * Rank0   29(bit 15)      29(bit 6)       30(bit 3)       29(bit 9)
     * Rank1   24(bit 15)      26(bit 3)       28(bit 3)       26(bit 9)
     */
    mcSHOW_DBG_MSG(("Channel\tA\t\tB\t\tC\t\tD\n"
                    "Rank0\t%d(bit %d)\t%d(bit %d)\t%d(bit %d)\t%d(bit %d)\n"
                    "Rank1\t%d(bit %d)\t%d(bit %d)\t%d(bit %d)\t%d(bit %d)\n\n",
                    min_rx_value[0][0], min_RX_DQ_bit[0][0], min_rx_value[1][0], min_RX_DQ_bit[1][0], min_rx_value[2][0], min_RX_DQ_bit[2][0], min_rx_value[3][0], min_RX_DQ_bit[3][0],
                    min_rx_value[0][1], min_RX_DQ_bit[0][1], min_rx_value[1][1], min_RX_DQ_bit[1][1], min_rx_value[2][1], min_RX_DQ_bit[2][1], min_rx_value[3][1], min_RX_DQ_bit[3][1]));
#ifdef DRAM_HQA_USED
    /* Min Window(ps)(%)
     * Channel A                       B                       C                       D
     * Rank0   25868/100ps(83%)(PASS)  25868/100ps(83%)(PASS)  26760/100ps(86%)(PASS)  25868/100ps(83%)(PASS)
     * Rank1   21408/100ps(69%)(PASS)  23192/100ps(75%)(PASS)  24976/100ps(80%)(PASS)  23192/100ps(75%)(PASS)
     */
    mcSHOW_DBG_MSG(("Min Window(ps)(%%)\n"
                    "Channel\tA\t\t\tB\n"));
    for(u1RankIdx = RANK_0; u1RankIdx < 2; u1RankIdx++)
    {
        mcSHOW_DBG_MSG(("Rank%d\t", u1RankIdx));
        for(u1ChannelIdx = CHANNEL_A; u1ChannelIdx < p->support_channel_num; u1ChannelIdx++)
        {
            mcSHOW_DBG_MSG(("%d/100ps(%d%%)(%s)\t",
                            min_rx_value[u1ChannelIdx][u1RankIdx] * u2gdelay_cell_ps_all[shuffleIdx][u1ChannelIdx],
                            ((min_rx_value[u1ChannelIdx][u1RankIdx] * u2gdelay_cell_ps_all[shuffleIdx][u1ChannelIdx] * p->frequency * 2) + (1000000 - 1)) / 1000000,
                            (min_rx_value[u1ChannelIdx][u1RankIdx] * u2gdelay_cell_ps_all[shuffleIdx][u1ChannelIdx] * p->frequency * 2) / 1000000 >= 40 ? "PASS" : "FAIL"));
        }
        mcSHOW_DBG_MSG(("\n"));
    }
#endif

#else
    mcSHOW_DBG_MSG(("Channel\tA\t\tB\n"
                    "Rank0\t%d(bit %d)\t\t%d(bit %d)\n"
                    "Rank1\t%d(bit %d)\t\t%d(bit %d)\n\n",
                    min_rx_value[0][0], min_RX_DQ_bit[0][0], min_rx_value[1][0], min_RX_DQ_bit[1][0],
                    min_rx_value[0][1], min_RX_DQ_bit[0][1], min_rx_value[1][1], min_RX_DQ_bit[1][1]));

#ifdef DRAM_HQA_USED
    mcSHOW_DBG_MSG(("Min Window(ps)(%%)\n"
                    "Channel\tA\t\t\tB\n"));
    for(u1RankIdx = RANK_0; u1RankIdx < 2; u1RankIdx++)
    {
        mcSHOW_DBG_MSG(("Rank%d\t", u1RankIdx));
        for(u1ChannelIdx = CHANNEL_A; u1ChannelIdx < 2; u1ChannelIdx++)
        {
            mcSHOW_DBG_MSG(("%d/100ps(%d%%)(%s)\t",
                            min_rx_value[u1ChannelIdx][u1RankIdx] * u2gdelay_cell_ps_all[shuffleIdx][u1ChannelIdx],
                            ((min_rx_value[u1ChannelIdx][u1RankIdx] * u2gdelay_cell_ps_all[shuffleIdx][u1ChannelIdx] * p->frequency * 2) + (1000000 - 1)) / 1000000,
                            (min_rx_value[u1ChannelIdx][u1RankIdx] * u2gdelay_cell_ps_all[shuffleIdx][u1ChannelIdx] * p->frequency * 2) / 1000000 >= 40 ? "PASS" : "FAIL"));
        }
        mcSHOW_DBG_MSG(("\n"));
    }
#endif
#endif
}

    /* [TX minimum per bit window]
     * VrefDQ Range : 0
     * VrefDQ
     * Channel A               B               C               D
     * Rank0   14              14              14              14
     * Rank1   20              14              18              16
     */
    mcSHOW_DBG_MSG(("\n[TX minimum per bit window]\n"));
    if (p->enable_tx_scan_vref == ENABLE)    
    {
        mcSHOW_DBG_MSG(("VrefDQ Range : %d\n",(u1MR14Value[p->channel][p->rank][p->dram_fsp]>>6)&1));    
#if CHANNEL_NUM==4

        mcSHOW_DBG_MSG(("VrefDQ\n"
                        "Channel\tA\t\tB\t\tC\t\tD\n"
                        "Rank0\t%d\t\t%d\t\t%d\t\t%d\n"
                        "Rank1\t%d\t\t%d\t\t%d\t\t%d\n\n",
                        gFinalTXVrefDQ[0][0], gFinalTXVrefDQ[1][0], gFinalTXVrefDQ[2][0], gFinalTXVrefDQ[3][0],
                        gFinalTXVrefDQ[0][1], gFinalTXVrefDQ[1][1], gFinalTXVrefDQ[2][1], gFinalTXVrefDQ[3][1]));

#else
        mcSHOW_DBG_MSG(("VrefDQ\n"
                        "Channel\tA\t\tB\n"
                        "Rank0\t%d\t\t%d\n"
                        "Rank1\t%d\t\t%d\n\n",
                        gFinalTXVrefDQ[0][0], gFinalTXVrefDQ[1][0],
                        gFinalTXVrefDQ[0][1], gFinalTXVrefDQ[1][1]));
#endif
    }
    else
    {
        mcSHOW_DBG_MSG(("TX DQ Vref Scan : Disable\n"));
    }

#if (SUPPORT_SAVE_TIME_FOR_CALIBRATION )
 if(p->pSavetimeData->femmc_Ready==1&&( p->pSavetimeData->Bypass_TXWINDOW[_MappingFreqArray[p->freq_sel]]))
 	{
    mcSHOW_DBG_MSG(("\n[TX minimum per bit window bypass calibration]\n"));
 	}
 else

#endif
{
#if CHANNEL_NUM==4
    /* Channel A               B               C               D
     * Rank0   24              24              24              23
     * Rank1   23              24              23              23
     */
    mcSHOW_DBG_MSG(("Channel\tA\t\tB\t\tC\t\tD\n"
                    "Rank0\t%d\t\t%d\t\t%d\t\t%d\n"
                    "Rank1\t%d\t\t%d\t\t%d\t\t%d\n\n",
                    min_tx_value[0][0], min_tx_value[1][0], min_tx_value[2][0], min_tx_value[3][0],
                    min_tx_value[0][1], min_tx_value[1][1], min_tx_value[2][1], min_tx_value[3][1]));
#ifdef DRAM_HQA_USED
    /* Min Window(%)
     * Channel A               B               C               D
     * Rank0   75%(PASS)       75%(PASS)       75%(PASS)       72%(PASS)
     * Rank1   72%(PASS)       75%(PASS)       72%(PASS)       72%(PASS)
     */
    mcSHOW_DBG_MSG(("Min Window(%%)\n"
                    "Channel\tA\t\tB\n"));
    for(u1RankIdx = RANK_0; u1RankIdx < 2; u1RankIdx++)
    {
        mcSHOW_DBG_MSG(("Rank%d\t", u1RankIdx));
        for(u1ChannelIdx = CHANNEL_A; u1ChannelIdx < p->support_channel_num; u1ChannelIdx++)
        {
            mcSHOW_DBG_MSG(("%d%%(%s)\t",
                           (min_tx_value[u1ChannelIdx][u1RankIdx] * 100 + 31) / 32,
                           (min_tx_value[u1ChannelIdx][u1RankIdx] * 100 + 31) / 32 >= 45 ? "PASS" : "FAIL"));
        }
        mcSHOW_DBG_MSG(("\n"));
    }
#endif
#else
    mcSHOW_DBG_MSG(("Channel\tA\t\tB\n"
                    "Rank0\t%d\t\t%d\n"
                    "Rank1\t%d\t\t%d\n\n",
                    min_tx_value[0][0], min_tx_value[1][0],
                    min_tx_value[0][1], min_tx_value[1][1]));
#ifdef DRAM_HQA_USED
    mcSHOW_DBG_MSG(("Min Window(%%)\n"
                    "Channel\tA\t\tB\n"));
    for(u1RankIdx = RANK_0; u1RankIdx < 2; u1RankIdx++)
    {
        mcSHOW_DBG_MSG(("Rank%d\t", u1RankIdx));
        for(u1ChannelIdx = CHANNEL_A; u1ChannelIdx < 2; u1ChannelIdx++)
        {
            mcSHOW_DBG_MSG(("%d%%(%s)\t",
                           (min_tx_value[u1ChannelIdx][u1RankIdx] * 100 + 31) / 32,
                           (min_tx_value[u1ChannelIdx][u1RankIdx] * 100 + 31) / 32 >= 45 ? "PASS" : "FAIL"));
        }
        mcSHOW_DBG_MSG(("\n"));
    }
#endif
#endif
#if CHANNEL_NUM==4
    /* min DQ bit max win size
     * Channel A               B               C               D
     * Rank0   24(bit 0)       24(bit 0)       24(bit 8)       23(bit 0)
     * Rank1   23(bit 0)       24(bit 0)       23(bit 9)       23(bit 1)
     */
    mcSHOW_DBG_MSG(("min DQ bit max win size\n"
                    "Channel\tA\t\tB\t\tC\t\tD\n"
                    "Rank0\t%d(bit %d)\t%d(bit %d)\t%d(bit %d)\t%d(bit %d)\n"
                    "Rank1\t%d(bit %d)\t%d(bit %d)\t%d(bit %d)\t%d(bit %d)\n\n",
                    gFinalTXPerbitWin_min_max[0][0], min_TX_DQ_bit[0][0], gFinalTXPerbitWin_min_max[1][0], min_TX_DQ_bit[1][0], gFinalTXPerbitWin_min_max[2][0], min_TX_DQ_bit[2][0], gFinalTXPerbitWin_min_max[3][0], min_TX_DQ_bit[3][0],
                    gFinalTXPerbitWin_min_max[0][1], min_TX_DQ_bit[0][1], gFinalTXPerbitWin_min_max[1][1], min_TX_DQ_bit[1][1], gFinalTXPerbitWin_min_max[2][1], min_TX_DQ_bit[2][1], gFinalTXPerbitWin_min_max[3][1], min_TX_DQ_bit[3][1]));

#else
    mcSHOW_DBG_MSG(("min DQ bit max win size\n"
                    "Channel\tA\t\tB\n"
                    "Rank0\t%d(bit %d)\t%d(bit %d)\n"
                    "Rank1\t%d(bit %d)\t%d(bit %d)\n\n",
                    gFinalTXPerbitWin_min_max[0][0], min_TX_DQ_bit[0][0], gFinalTXPerbitWin_min_max[1][0], min_TX_DQ_bit[1][0],
                    gFinalTXPerbitWin_min_max[0][1], min_TX_DQ_bit[0][1], gFinalTXPerbitWin_min_max[1][1], min_TX_DQ_bit[1][1]));
#endif

#if(TX_PER_BIT_DELAY_CELL==0)
#if CHANNEL_NUM==4
    mcSHOW_DBG_MSG(("min DQ margin\n"
                    "Channel\tA\t\tB\t\tC\t\tD\n"
                    "Rank0\t%d(bit %d)\t%d(bit %d)\t%d(bit %d)\t%d(bit %d)\n"
                    "Rank1\t%d(bit %d)\t%d(bit %d)\t%d(bit %d)\t%d(bit %d)\n",
                    gFinalTXPerbitWin_min_margin[0][0], gFinalTXPerbitWin_min_margin_bit[0][0],
                    gFinalTXPerbitWin_min_margin[1][0], gFinalTXPerbitWin_min_margin_bit[1][0],
                    gFinalTXPerbitWin_min_margin[2][0], gFinalTXPerbitWin_min_margin_bit[2][0],
                    gFinalTXPerbitWin_min_margin[3][0], gFinalTXPerbitWin_min_margin_bit[3][0],
                    gFinalTXPerbitWin_min_margin[0][1], gFinalTXPerbitWin_min_margin_bit[0][1],
                    gFinalTXPerbitWin_min_margin[1][1], gFinalTXPerbitWin_min_margin_bit[1][1],
                    gFinalTXPerbitWin_min_margin[2][1], gFinalTXPerbitWin_min_margin_bit[2][1],
                    gFinalTXPerbitWin_min_margin[3][1], gFinalTXPerbitWin_min_margin_bit[3][1]));
#else
    mcSHOW_DBG_MSG(("min DQ margin\n"
                    "Channel\tA\t\tB\n"
                    "Rank0\t%d(bit %d)\t%d(bit %d)\n"
                    "Rank1\t%d(bit %d)\t%d(bit %d)\n\n",
                    gFinalTXPerbitWin_min_margin[0][0], gFinalTXPerbitWin_min_margin_bit[0][0],
                    gFinalTXPerbitWin_min_margin[1][0], gFinalTXPerbitWin_min_margin_bit[1][0],
                    gFinalTXPerbitWin_min_margin[0][1], gFinalTXPerbitWin_min_margin_bit[0][1],
                    gFinalTXPerbitWin_min_margin[1][1], gFinalTXPerbitWin_min_margin_bit[1][1]));
#endif
#endif
}
    #if defined(ENABLE_MIOCK_JMETER_HQA) && !defined(RELEASE)
    //if(p->frequency == u2DFSGetHighestFreq(p))
    {
        mcSHOW_DBG_MSG(("\n[DramcMiockJmeter]\n"
                        "Channel\tVCORE\t\t1 delay cell\n"
                        "SHUFFLE 1\n"));

        for (u1ChannelIdx = 0; u1ChannelIdx < p->support_channel_num; u1ChannelIdx++)
        {
            mcSHOW_DBG_MSG(("CH%d\t%d\t\t%d/100 ps\n", u1ChannelIdx, u4gVcore[DRAM_DFS_SHUFFLE_1], u2gdelay_cell_ps_all[DRAM_DFS_SHUFFLE_1][u1ChannelIdx]));
        }

        mcSHOW_DBG_MSG(("\nSHUFFLE 2\n"));
        for (u1ChannelIdx = 0; u1ChannelIdx < p->support_channel_num; u1ChannelIdx++)
        {
            mcSHOW_DBG_MSG(("CH%d\t%d\t\t%d/100 ps\n", u1ChannelIdx, u4gVcore[DRAM_DFS_SHUFFLE_2], u2gdelay_cell_ps_all[DRAM_DFS_SHUFFLE_2][u1ChannelIdx]));
        }

        mcSHOW_DBG_MSG(("\nSHUFFLE 3\n"));
        for (u1ChannelIdx = 0; u1ChannelIdx < p->support_channel_num; u1ChannelIdx++)
        {
            mcSHOW_DBG_MSG(("CH%d\t%d\t\t%d/100 ps\n", u1ChannelIdx, u4gVcore[DRAM_DFS_SHUFFLE_3], u2gdelay_cell_ps_all[DRAM_DFS_SHUFFLE_3][u1ChannelIdx]));
        }

        mcSHOW_DBG_MSG(("\nSHUFFLE 4\n"));
        for (u1ChannelIdx = 0; u1ChannelIdx < p->support_channel_num; u1ChannelIdx++)
        {
            mcSHOW_DBG_MSG(("CH%d\t%d\t\t%d/100 ps\n", u1ChannelIdx, u4gVcore[DRAM_DFS_SHUFFLE_4], u2gdelay_cell_ps_all[DRAM_DFS_SHUFFLE_4][u1ChannelIdx]));
        }
    }
    #endif

    mcSHOW_DBG_MSG(("\n\n\n"));



#if VENDER_JV_LOG
    mcSHOW_DBG_MSG5(("\n\n\n[Summary] information for measurement\n"));
    //mcSHOW_DBG_MSG5(("\tDram Data rate = %d\n",p->frequency*2));
    vPrintCalibrationBasicInfo_ForJV(p);

    if(u1IsLP4Family(p->dram_type))
     {
         mcSHOW_DBG_MSG5(("[Cmd Bus Training window]\n"));
         mcSHOW_DBG_MSG5(("VrefCA Range : %d\n", gCBT_VREF_RANGE_SEL));
#if CHANNEL_NUM==4
        mcSHOW_DBG_MSG5(("CHA_VrefCA_Rank0   CHB_VrefCA_Rank0   CHC_VrefCA_Rank0    CHD_VrefCA_Rank0\n"));
        mcSHOW_DBG_MSG5(("%d                 %d                 %d                  %d\n", gFinalCBTVrefCA[0][0], gFinalCBTVrefCA[1][0], gFinalCBTVrefCA[2][0], gFinalCBTVrefCA[3][0]));
#else
         mcSHOW_DBG_MSG5(("CHA_VrefCA_Rank0   CHB_VrefCA_Rank0\n"));
         mcSHOW_DBG_MSG5(("%d                 %d\n", gFinalCBTVrefCA[0][0], gFinalCBTVrefCA[1][0]));
#endif
         mcSHOW_DBG_MSG5(("CHA_CA_window_Rank0   CHB_CA_winow_Rank0\n"));
         mcSHOW_DBG_MSG5(("%d%%(bit %d)              %d%%(bit %d) \n\n",(min_ca_value[0][0]*100+63)/64, min_ca_bit[0][0],
                                                                     (min_ca_value[1][0]*100+63)/64, min_ca_bit[1][0]));
     }
     else
     {
         mcSHOW_DBG_MSG5(("[CA Training window]\n"));
         mcSHOW_DBG_MSG5(("CHA_CA_win_Rank0\n"));
         mcSHOW_DBG_MSG5(("%d%%(bit %d)\n\n",(min_ca_value[0][0]*100+63)/64, min_ca_bit[0][0]));
     }

     mcSHOW_DBG_MSG5(("[RX minimum per bit window]\n"));
     if (p->enable_rx_scan_vref == ENABLE)
     {
#if CHANNEL_NUM==4
        mcSHOW_DBG_MSG5(("CHA_VrefDQ    CHB_VrefDQ      CHC_VrefDQ      CHD_VrefDQ\n"));
        mcSHOW_DBG_MSG5(("%d            %d              %d              %d \n", gFinalRXVrefDQ[CHANNEL_A], gFinalRXVrefDQ[CHANNEL_B], gFinalRXVrefDQ[CHANNEL_C], gFinalRXVrefDQ[CHANNEL_D]));
#else
         mcSHOW_DBG_MSG5(("CHA_VrefDQ   CHB_VrefDQ\n"));
         mcSHOW_DBG_MSG5(("%d                 %d \n", gFinalRXVrefDQ[CHANNEL_A], gFinalRXVrefDQ[CHANNEL_B]));
#endif
     }
     else
     {
         mcSHOW_DBG_MSG5(("RX DQ Vref Scan : Disable\n"));

     }

#if CHANNEL_NUM==4
    mcSHOW_DBG_MSG5(("CHA_Rank0           CHA_Rank1           CHB_Rank0           CHB_Rank1         CHC_Rank0       CHC_Rank1       CHD_Rank0       CHD_Rank1\n"));
    mcSHOW_DBG_MSG5(("%d%%(bit %d)         %d%%(bit %d)         %d%%(bit %d)         %d%%(bit %d)         %d%%(bit %d)         %d%%(bit %d)         %d%%(bit %d)         %d%%(bit %d)\n\n",
                                    ((min_rx_value[0][0]*u2gdelay_cell_ps_all[shuffleIdx][0]*p->frequency*2)+(1000000-1))/1000000, min_RX_DQ_bit[0][0],
                                    ((min_rx_value[0][1]*u2gdelay_cell_ps_all[shuffleIdx][0]*p->frequency*2)+(1000000-1))/1000000, min_RX_DQ_bit[0][1],
                                    ((min_rx_value[1][0]*u2gdelay_cell_ps_all[shuffleIdx][1]*p->frequency*2)+(1000000-1))/1000000, min_RX_DQ_bit[1][0],
                                    ((min_rx_value[1][1]*u2gdelay_cell_ps_all[shuffleIdx][1]*p->frequency*2)+(1000000-1))/1000000, min_RX_DQ_bit[1][1],
                                    ((min_rx_value[2][0]*u2gdelay_cell_ps_all[shuffleIdx][2]*p->frequency*2)+(1000000-1))/1000000, min_RX_DQ_bit[2][0],
                                    ((min_rx_value[2][1]*u2gdelay_cell_ps_all[shuffleIdx][2]*p->frequency*2)+(1000000-1))/1000000, min_RX_DQ_bit[2][1],
                                    ((min_rx_value[3][0]*u2gdelay_cell_ps_all[shuffleIdx][3]*p->frequency*2)+(1000000-1))/1000000, min_RX_DQ_bit[3][0],
                                    ((min_rx_value[3][1]*u2gdelay_cell_ps_all[shuffleIdx][3]*p->frequency*2)+(1000000-1))/1000000, min_RX_DQ_bit[3][1]));
#else
     mcSHOW_DBG_MSG5(("CHA_Rank0           CHA_Rank1           CHB_Rank0           CHB_Rank1\n"));
     mcSHOW_DBG_MSG5(("%d%%(bit %d)         %d%%(bit %d)         %d%%(bit %d)         %d%%(bit %d)\n\n",
                                     ((min_rx_value[0][0]*u2gdelay_cell_ps_all[shuffleIdx][0]*p->frequency*2)+(1000000-1))/1000000, min_RX_DQ_bit[0][0],
                                     ((min_rx_value[0][1]*u2gdelay_cell_ps_all[shuffleIdx][0]*p->frequency*2)+(1000000-1))/1000000, min_RX_DQ_bit[0][1],
                                     ((min_rx_value[1][0]*u2gdelay_cell_ps_all[shuffleIdx][1]*p->frequency*2)+(1000000-1))/1000000, min_RX_DQ_bit[1][0],
                                     ((min_rx_value[1][1]*u2gdelay_cell_ps_all[shuffleIdx][1]*p->frequency*2)+(1000000-1))/1000000, min_RX_DQ_bit[1][1]));
#endif


     mcSHOW_DBG_MSG5(("[TX minimum per bit window]\n"));
     if (p->enable_tx_scan_vref == ENABLE)    
     {    
         mcSHOW_DBG_MSG5(("VrefDQ Range : %d\n",(u1MR14Value[p->channel][p->rank][p->dram_fsp]>>6)&1));    
#if CHANNEL_NUM==4
        mcSHOW_DBG_MSG5(("CHA_VrefDQ_Rank0   CHA_VrefDQ_Rank1    CHB_VrefDQ_Rank0    CHB_VrefDQ_Rank1   CHC_VrefDQ_Rank0    CHC_VrefDQ_Rank1    CHD_VrefDQ_Rank0    CHD_VrefDQ_Rank1\n"));
        mcSHOW_DBG_MSG5(("%d                 %d                  %d                  %d                 %d                  %d                  %d                  %d\n"
                    , gFinalTXVrefDQ[0][0], gFinalTXVrefDQ[0][1], gFinalTXVrefDQ[1][0], gFinalTXVrefDQ[1][1]
                    , gFinalTXVrefDQ[2][0], gFinalTXVrefDQ[2][1], gFinalTXVrefDQ[3][0], gFinalTXVrefDQ[3][1]
                    ));
#else
         mcSHOW_DBG_MSG5(("CHA_VrefDQ_Rank0   CHA_VrefDQ_Rank1    CHB_VrefDQ_Rank0    CHB_VrefDQ_Rank1\n"));
         mcSHOW_DBG_MSG5(("%d                  %d                   %d                   %d\n", gFinalTXVrefDQ[0][0], gFinalTXVrefDQ[0][1], gFinalTXVrefDQ[1][0], gFinalTXVrefDQ[1][1]));
#endif
     }
     else
     {
         mcSHOW_DBG_MSG5(("TX DQ Vref Scan : Disable\n"));
     }

#if CHANNEL_NUM==4
    mcSHOW_DBG_MSG5(("CHA_Rank0         CHA_Rank1           CHB_Rank0           CHB_Rank1       CHC_Rank0       CHC_Rank1       CHD_Rank0   CHD_Rank1\n"));
    mcSHOW_DBG_MSG5(("%d%%               %d%%                 %d%%                 %d%%                 %d%%                 %d%%                 %d%%                 %d%%\n",
                                        (min_tx_value[0][0]*100+31)/32,
                                        (min_tx_value[0][1]*100+31)/32,
                                        (min_tx_value[1][0]*100+31)/32,
                                        (min_tx_value[1][1]*100+31)/32,
                                        (min_tx_value[2][0]*100+31)/32,
                                        (min_tx_value[2][1]*100+31)/32,
                                        (min_tx_value[3][0]*100+31)/32,
                                        (min_tx_value[3][1]*100+31)/32
                                        ));
#else
     mcSHOW_DBG_MSG5(("CHA_Rank0           CHA_Rank1           CHB_Rank0           CHB_Rank1\n"));
     mcSHOW_DBG_MSG5(("%d%%                %d%%                %d%%                %d%%\n",
                                         (min_tx_value[0][0]*100+31)/32,
                                         (min_tx_value[0][1]*100+31)/32,
                                         (min_tx_value[1][0]*100+31)/32,
                                         (min_tx_value[1][1]*100+31)/32));
#endif
#endif


    // reset all data
    HQA_measure_message_reset_all_data(p);
}
#ifdef RELEASE
#undef mcSHOW_DBG_MSG(_x_)
#define mcSHOW_DBG_MSG(_x_)
#endif
#endif


#ifdef RX_EYE_SCAN
#define RX_EYE_VREF_LEVEL   32
#define RX_EYE_DELAY_LEVEL  63//40
#define RX_EYE_SAVE_TO_FILE 1
#define ENALBE_EYE_SCAN_LOG_DURING_PROCESS 0
#define RX_EYE_SCAN_VREF_EXTEND 0

typedef enum
{
    RX_EYE_SCAN_MOVE_DQS_ENTER=0,
    RX_EYE_SCAN_MOVE_DQS_END,
} RX_EYE_SCAN_MOVE_DQS_STATUS_T;

typedef enum
{
    RX_EYE_SCAN_SCAN_AT_BOOT_TIME=0,
    RX_EYE_SCAN_SCAN_AT_RUN_TIME,
} RX_EYE_SCAN_TIME_T;


void DramcRxEyeScanInit(DRAMC_CTX_T *p)
{
    U8 u1ByteIdx;
    //Enable DQ eye scan
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 1, EYESCAN_RG_RX_EYE_SCAN_EN);

    vIO32WriteFldMulti(DRAMC_REG_EYESCAN, P_Fld(0x1, EYESCAN_EYESCAN_DQS_SYNC_EN)    
                                        | P_Fld(0x1, EYESCAN_EYESCAN_NEW_DQ_SYNC_EN)    
                                        | P_Fld(0x1, EYESCAN_EYESCAN_DQ_SYNC_EN));   

    //Disable MIOCK jitter meter mode (RG_??_RX_DQS_MIOCK_SEL=0, RG_RX_MIOCK_JIT_EN=0)
    //RG_RX_MIOCK_JIT_EN=0
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 0, EYESCAN_RG_RX_MIOCK_JIT_EN);

    if(u1IsLP4Family(p->dram_type))
    {
        //RG_??_RX_EYE_SCAN_EN,   RG_??_RX_VREF_EN,  RG_??_RX_SMT_EN
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ5), 1, B0_DQ5_RG_RX_ARDQ_EYE_EN_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ5), 1, B1_DQ5_RG_RX_ARDQ_EYE_EN_B1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ5), 1, B0_DQ5_RG_RX_ARDQ_EYE_VREF_EN_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ5), 1, B1_DQ5_RG_RX_ARDQ_EYE_VREF_EN_B1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ3), 1, B0_DQ3_RG_RX_ARDQ_SMT_EN_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ3), 1, B1_DQ3_RG_RX_ARDQ_SMT_EN_B1);
    }
#if ENABLE_LP3_SW
    else  //LPDDR3
    {
        for(u1ByteIdx=0; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
        {
            //RG_??_RX_EYE_SCAN_EN,   RG_??_RX_VREF_EN,  RG_??_RX_SMT_EN
            vIO32WriteFldAlign_Phy_Byte(u1ByteIdx, DRAMC_REG_ADDR(DDRPHY_B0_DQ5), 1, B0_DQ5_RG_RX_ARDQ_EYE_EN_B0);
            vIO32WriteFldAlign_Phy_Byte(u1ByteIdx, DRAMC_REG_ADDR(DDRPHY_B0_DQ5), 1, B0_DQ5_RG_RX_ARDQ_EYE_VREF_EN_B0);
            vIO32WriteFldAlign_Phy_Byte(u1ByteIdx, DRAMC_REG_ADDR(DDRPHY_B0_DQ3), 1, B0_DQ3_RG_RX_ARDQ_SMT_EN_B0);
        }
    }
#endif

    DramEyeStbenReset(p);
    DramPhyReset(p);
}

#if 1
static void DramcRxEyeRuntimeDQS(DRAMC_CTX_T *p, U8 enter_or_end)
{
    U32 u4TimeCnt =1000;
    U8 u1Ready[DQS_NUMBER], u1Ready_all, u1Wait=1;

    if(u1IsLP4Family(p->dram_type))
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), enter_or_end, B0_DQ6_RG_RX_ARDQ_EYE_OE_GATE_EN_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), enter_or_end, B1_DQ6_RG_RX_ARDQ_EYE_OE_GATE_EN_B1);

        do
        {
            u1Ready[0]  =(U8)u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_PHY_RGS_DQ), MISC_PHY_RGS_DQ_RGS_RX_ARDQS0_RDY_EYE_B0);
            u1Ready[1] = (U8)u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_PHY_RGS_DQ), MISC_PHY_RGS_DQ_RGS_RX_ARDQS0_RDY_EYE_B1);

            if(enter_or_end ==RX_EYE_SCAN_MOVE_DQS_ENTER)
            {
               u1Ready_all = u1Ready[0] ||u1Ready[1];
            }
            else//RX_EYE_SCAN_MOVE_DQS_END
            {
               u1Ready_all = u1Ready[0] && u1Ready[1];
            }

            u1Wait = (u1Ready_all != enter_or_end) ? 1 : 0;
            u4TimeCnt--;
        }
        while((u1Wait ==1) && (u4TimeCnt >0));

            ///TODO:  need to be fix, Yulia
        //if(u4TimeCnt ==0)
       //    mcSHOW_DBG_MSG(("DQS setup timeout (enter_or_end = %d) %d %d\n", enter_or_end, u1Ready[0] , u1Ready[1]));
    }
#if 0///TODO://ENABLE_LP3_SW
    else //LPDDR3
    {
        vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), enter_or_end, B0_DQ6_RG_RX_ARDQ_EYE_OE_GATE_EN_B0);
        vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), enter_or_end, B1_DQ6_RG_RX_ARDQ_EYE_OE_GATE_EN_B1);
        do
        {
            u1Ready[0] = (U8)u4IO32ReadFldAlign(DDRPHY_PHY_RO_1+(aru1PhyMap2Channel[0]<< POS_BANK_NUM), PHY_RO_1_RGS_RX_ARDQS0_DLY_RDY_EYE);
            u1Ready[1] = (U8)u4IO32ReadFldAlign(DDRPHY_PHY_RO_1+(aru1PhyMap2Channel[1]<< POS_BANK_NUM), PHY_RO_1_RGS_RX_ARDQS0_DLY_RDY_EYE);
            u1Ready[2] = (U8)u4IO32ReadFldAlign(DDRPHY_PHY_RO_1+(aru1PhyMap2Channel[0]<< POS_BANK_NUM), PHY_RO_1_RGS_RX_ARDQS1_DLY_RDY_EYE);
            u1Ready[3] = (U8)u4IO32ReadFldAlign(DDRPHY_PHY_RO_1+(aru1PhyMap2Channel[1]<< POS_BANK_NUM), PHY_RO_1_RGS_RX_ARDQS1_DLY_RDY_EYE);

            if(enter_or_end ==RX_EYE_SCAN_MOVE_DQS_ENTER)
            {
                u1Ready_all = u1Ready[0] ||u1Ready[1]||u1Ready[2]||u1Ready[3];
            }
            else//RX_EYE_SCAN_MOVE_DQS_END
            {
                u1Ready_all = u1Ready[0] && u1Ready[1] && u1Ready[2] && u1Ready[3];
            }

            u1Wait = (u1Ready_all != enter_or_end) ? 1 : 0;

            u4TimeCnt--;

         }
         while((u1Wait ==1) && (u4TimeCnt >0));

         if(u4TimeCnt ==0)
         {
             mcSHOW_DBG_MSG(("DQS setup timeout (enter_or_end = %d) (u1Ready_all %d) u4TimeCnt %d,%d %d %d %d\n", enter_or_end, u1Ready_all, u4TimeCnt, u1Ready[0], u1Ready[1], u1Ready[2], u1Ready[3]));
         }
    }
#endif
}
#endif

//-------------------------------------------------------------------------
/** DramcRxEyeScan
 *  start the rx dq eye scan.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param dq_no            (U8): 0~7.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL
 */
//-------------------------------------------------------------------------
DRAM_STATUS_T DramcRxEyeScanRun(DRAMC_CTX_T *p, U8 boot_or_run, U8 dq_no)
{
    S16 s2vref, s2dq_dly, s2DelayBegin, s2DelayEnd;
    U32  u4err_value = 0xffffffff, u4sample_cnt=0, u4error_cnt[DQS_NUMBER];
    #if RX_EYE_SAVE_TO_FILE
    static U32 arErrorCount[DQS_NUMBER][RX_EYE_VREF_LEVEL][RX_EYE_DELAY_LEVEL];
    static U16 arVrefLevelForLog[RX_EYE_VREF_LEVEL];
    U16 u2VrefIdx, u2DelayIdx, ii,jj, kk;
    #endif
    U16 u2VrefBegin, u2VrefEnd, u2VrefStep;

    U32 uiSrcAddr, uiDestAddr, uiLen;

    U8 u1DramCBit, u1PHYBit;

#if RX_EYE_SCAN_VREF_EXTEND
    U8 Vref_DDR4_SEL =0;
#endif
    mcSHOW_DBG_MSG(("[RxEyeScan]\n"));
    // error handling
    if (!p)
    {
        mcSHOW_ERR_MSG(("context NULL\n"));
        return DRAM_FAIL;
    }

    if (dq_no > 15)
    {
        mcSHOW_ERR_MSG(("DQ number should be 0~7 for 4 bytes\n"));
        return DRAM_FAIL;
    }

	//DDRPHY.SHU1_B*_DQ7.R_DMDQMDBI_SHU_B* = R_DMDQMDBI_EYE_SHU_B* 
	//0: DBI-OFF
	//1: DBI-ON
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ7), p->DBI_R_onoff[p->dram_fsp], SHU1_B0_DQ7_R_DMDQMDBI_EYE_SHU_B0);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DQ7), p->DBI_R_onoff[p->dram_fsp], SHU1_B1_DQ7_R_DMDQMDBI_EYE_SHU_B1);

    // check if SoC platform has "%" operation?!
    #if RX_EYE_SAVE_TO_FILE
    for(ii=0; ii<(p->data_width/DQS_BIT_NUMBER); ii++)
    {
        for(jj=0; jj<RX_EYE_VREF_LEVEL; jj++)
        {
            for(kk=0; kk<RX_EYE_DELAY_LEVEL; kk++)
            {
                arErrorCount[ii][jj][kk] =0xffff;
            }
        }
    }
    #endif

    if(boot_or_run==RX_EYE_SCAN_SCAN_AT_RUN_TIME)
    {
        //uiSrcAddr = DDR_BASE+0x10000000;
        uiSrcAddr = 0x50000000;
        uiLen = 0xff000;

        if (p->support_rank_num==RANK_DUAL)
			mem_test_address_calculation(p, uiSrcAddr, &uiDestAddr);
		else
			uiDestAddr=uiSrcAddr+0x10000000;
    }

    //DRAMC: RG_RX_DQ_EYE_SEL (0~7) for 4 bytes
    u1DramCBit = dq_no% DQS_BIT_NUMBER;
    u1PHYBit = dq_no % DQS_BIT_NUMBER;
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), u1DramCBit, EYESCAN_RX_DQ_EYE_SEL);

    if(u1IsLP4Family(p->dram_type))
    {
        //select DQ to be scanned (0~7)
        //DDRPHY: RG_??_RX_DQ_EYE_SEL
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ5), u1PHYBit, B0_DQ5_RG_RX_ARDQ_EYE_SEL_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ5), u1PHYBit, B1_DQ5_RG_RX_ARDQ_EYE_SEL_B1);

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), !boot_or_run, B0_DQ6_RG_RX_ARDQ_EYE_DLY_DQS_BYPASS_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), !boot_or_run, B1_DQ6_RG_RX_ARDQ_EYE_DLY_DQS_BYPASS_B1);
    }
#if ENABLE_LP3_SW
    else  //LPDDR3
    {
        //select DQ to be scanned (0~7)
        //DDRPHY: RG_??_RX_DQ_EYE_SEL
        vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), u1PHYBit, B0_DQ5_RG_RX_ARDQ_EYE_SEL_B0);
        vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), u1PHYBit, B1_DQ5_RG_RX_ARDQ_EYE_SEL_B1);

        vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), !boot_or_run, B0_DQ6_RG_RX_ARDQ_EYE_DLY_DQS_BYPASS_B0);
        vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), !boot_or_run, B1_DQ6_RG_RX_ARDQ_EYE_DLY_DQS_BYPASS_B1);
    }
#endif

    #if ENALBE_EYE_SCAN_LOG_DURING_PROCESS
    mcSHOW_DBG_MSG(("\n\tDQ RX eye scan (ch=%d, dq_%d, DramCBit %d, PHYBit %d)\n\n", p->channel, dq_no, u1DramCBit, u1PHYBit));
    #endif
    u2VrefBegin =RX_VREF_RANGE_BEGIN;
    u2VrefEnd =RX_VREF_RANGE_END;
    u2VrefStep=RX_VREF_RANGE_STEP;

    #if RX_EYE_SAVE_TO_FILE
    u2VrefIdx =0;
    #endif

    #if RX_EYE_SCAN_VREF_EXTEND
    s2vref =(u2VrefEnd-1);

    while(1)
    #else
    for(s2vref =(u2VrefEnd-1); s2vref > u2VrefBegin; s2vref-=u2VrefStep)
    //s2vref =0xa;
    #endif
    {
        #if RX_EYE_SCAN_VREF_EXTEND
        arVrefLevelForLog[u2VrefIdx] = s2vref;
        #endif

        if(boot_or_run ==RX_EYE_SCAN_SCAN_AT_RUN_TIME)
        {
            DramcDmaEngine((DRAMC_CTX_T *)p, DMA_OP_READ_WRITE, uiSrcAddr, uiDestAddr, uiLen, 8, DMA_CHECK_DATA_ACCESS_ONLY_AND_NO_WAIT, 2);
            DramcRxEyeRuntimeDQS(p, RX_EYE_SCAN_MOVE_DQS_ENTER);
        }
        //s2vref = s2vref | (1<<5);
        if(u1IsLP4Family(p->dram_type))
        {
            //Set Vref voltage
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ5), s2vref, B0_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B0);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ5), s2vref, B1_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B1);

            //Wait for Vref settles down, 1us is enough
            mcDELAY_US(1);

            //Set DQS delay (RG_??_RX_DQS_EYE_DLY) to 0
            vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B0_DQ4), 0);
            vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B1_DQ4), 0);
        }
#if ENABLE_LP3_SW
        else //LPDDR3
        {
            //Set Vref voltage
            vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ5), s2vref, B0_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B0);
            vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ5), s2vref, B1_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B1);

            //Wait for Vref settles down, 1us is enough
            mcDELAY_US(1);

            //Set DQS delay (RG_??_RX_DQS_EYE_DLY) to 0
            vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ4), 0, PHY_FLD_FULL);
            vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ4), 0, PHY_FLD_FULL);
        }
#endif

        if(boot_or_run ==RX_EYE_SCAN_SCAN_AT_RUN_TIME)
        {
            DramcRxEyeRuntimeDQS(p, RX_EYE_SCAN_MOVE_DQS_END);
        }
        else //if(boot_or_run ==0)
        {
            DramEyeStbenReset(p);
            DramPhyReset(p);
        }
 	 #if RX_EYE_SAVE_TO_FILE
        u2DelayIdx=0;
        #endif

        if(u1IsLP4Family(p->dram_type)) //LP4 SBS
        {
            if(p->channel ==CHANNEL_A)
            {
                s2DelayBegin = -35;
                s2DelayEnd = 15;
            }
            else if(p->channel ==CHANNEL_B)
            {
                s2DelayBegin = -30;
                s2DelayEnd = 25;
            }
        }
#if ENABLE_LP3_SW
        else //LPDDR3
        {
            s2DelayBegin = -60;
            s2DelayEnd = 30;
        }
#endif

        for (s2dq_dly=s2DelayBegin; s2dq_dly <s2DelayEnd; s2dq_dly+=4)
        {

            if(boot_or_run ==RX_EYE_SCAN_SCAN_AT_RUN_TIME)
            {
                //Reset eye scan counters (reg_sw_rst): 1 to 0
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 1, EYESCAN_REG_SW_RST);
                mcDELAY_US(1);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 0, EYESCAN_REG_SW_RST);

                DramcDmaEngine((DRAMC_CTX_T *)p, DMA_OP_READ_WRITE, uiSrcAddr, uiDestAddr, uiLen, 8, DMA_CHECK_DATA_ACCESS_ONLY_AND_NO_WAIT, 2);
            }

            if(s2dq_dly >0)  // move DQ delay
            {
                if(u1IsLP4Family(p->dram_type))
                {
                    //Set DQ delay (RG_??_RX_DQ_EYE_DLY)
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ4), s2dq_dly, B0_DQ4_RG_RX_ARDQ_EYE_R_DLY_B0);
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ4), s2dq_dly, B0_DQ4_RG_RX_ARDQ_EYE_F_DLY_B0);
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ4), s2dq_dly, B1_DQ4_RG_RX_ARDQ_EYE_R_DLY_B1);
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ4), s2dq_dly, B1_DQ4_RG_RX_ARDQ_EYE_F_DLY_B1);
                }
#if ENABLE_LP3_SW
                else
                {
                    //Set DQ delay (RG_??_RX_DQ_EYE_DLY)
                    vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ4), s2dq_dly, B0_DQ4_RG_RX_ARDQ_EYE_R_DLY_B0);
                    vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ4), s2dq_dly, B0_DQ4_RG_RX_ARDQ_EYE_F_DLY_B0);
                    vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ4), s2dq_dly, B1_DQ4_RG_RX_ARDQ_EYE_R_DLY_B1);
                    vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ4), s2dq_dly, B1_DQ4_RG_RX_ARDQ_EYE_F_DLY_B1);
                }
#endif
            }
            else// move DQS delay
            {
                if(boot_or_run ==RX_EYE_SCAN_SCAN_AT_RUN_TIME)
                {
                    DramcRxEyeRuntimeDQS(p, RX_EYE_SCAN_MOVE_DQS_ENTER);
                }

                if(u1IsLP4Family(p->dram_type))
                {
                    //Set DQS delay (RG_??_RX_DQS_EYE_DLY)
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ4), -s2dq_dly, B0_DQ4_RG_RX_ARDQS_EYE_R_DLY_B0);
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ4), -s2dq_dly, B0_DQ4_RG_RX_ARDQS_EYE_F_DLY_B0);
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ4), -s2dq_dly, B1_DQ4_RG_RX_ARDQS_EYE_R_DLY_B1);
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ4), -s2dq_dly, B1_DQ4_RG_RX_ARDQS_EYE_F_DLY_B1);
                }
#if ENABLE_LP3_SW
                else //LPDDR3
                {
                    //Set DQS delay (RG_??_RX_DQS_EYE_DLY)
                    vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ4), -s2dq_dly, B0_DQ4_RG_RX_ARDQS_EYE_R_DLY_B0);
                    vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ4), -s2dq_dly, B0_DQ4_RG_RX_ARDQS_EYE_F_DLY_B0);
                    vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ4), -s2dq_dly, B1_DQ4_RG_RX_ARDQS_EYE_R_DLY_B1);
                    vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ4), -s2dq_dly, B1_DQ4_RG_RX_ARDQS_EYE_F_DLY_B1);
                }
#endif

                if(boot_or_run ==RX_EYE_SCAN_SCAN_AT_RUN_TIME)
                {
                    DramcRxEyeRuntimeDQS(p, RX_EYE_SCAN_MOVE_DQS_END);
                }
                else //if(boot_or_run ==RX_EYE_SCAN_SCAN_AT_BOOT_TIME)
                {
                    DramEyeStbenReset(p);
                    DramPhyReset(p);
                }
            }

            if(boot_or_run ==RX_EYE_SCAN_SCAN_AT_BOOT_TIME)//boot time
            {
                //Reset eye scan counters (reg_sw_rst): 1 to 0
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 1, EYESCAN_REG_SW_RST);
                mcDELAY_US(1);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 0, EYESCAN_REG_SW_RST);

                u4err_value= TestEngineCompare(p);
            }
            else
            {
                //run time
                //mcDELAY_MS(1);
                u4err_value = DramcDmaEngine((DRAMC_CTX_T *)p, DMA_OP_READ_WRITE, uiSrcAddr, uiDestAddr, uiLen, 8, DMA_CHECK_COMAPRE_RESULT_ONLY, 2);  // comapre DMA result
            }

            //Read the counter values from registers (toggle_cnt*, dq_err_cnt*);
            //At run time, the counter will change all the time. Therefore, the counter of different bytes and toggle count will be data of different time.
            u4sample_cnt = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TOGGLE_CNT), TOGGLE_CNT_TOGGLE_CNT);
            u4error_cnt[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQ_ERR_CNT0), DQ_ERR_CNT0_DQ_ERR_CNT0);
            u4error_cnt[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQ_ERR_CNT1), DQ_ERR_CNT1_DQ_ERR_CNT1);

            if(u1IsLP4Family(p->dram_type))
            {
                #ifdef ETT_PRINT_FORMAT
                //mcSHOW_DBG_MSG(("RX Vref= %d, Delay= %d, ErrorValue=%d, Toggle %d, ErrorCnt %d\n",  s2vref, s2dq_dly, u4err_value, u4sample_cnt, u4error_cnt[dq_no/8]));
                #else
                mcSHOW_DBG_MSG(("RX Vref= %2d, Delay= %4d, ErrorValue=%d, Toggle %4d, ErrorCnt %4d\n",  s2vref, s2dq_dly, u4err_value u4sample_cnt, u4error_cnt[dq_no/8]));
                #endif
            }
#if ENABLE_LP3_SW
            else
            {
                u4error_cnt[2] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQ_ERR_CNT2), DQ_ERR_CNT2_DQ_ERR_CNT2);
                u4error_cnt[3] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQ_ERR_CNT3), DQ_ERR_CNT3_DQ_ERR_CNT3);
                #if ENALBE_EYE_SCAN_LOG_DURING_PROCESS
                #ifdef ETT_PRINT_FORMAT
                mcSHOW_DBG_MSG(("RX Vref= %d, Delay= %d,  %d|B0->B3| %d, %d, %d, %d\n", s2vref, s2dq_dly, u4sample_cnt,u4error_cnt[0], u4error_cnt[2],u4error_cnt[1],u4error_cnt[3]));
                #else
                mcSHOW_DBG_MSG(("RX Vref= %2d, Delay= %4d,  %d|B0->B3| %4d, %4d, %4d, %4d\n", s2vref, s2dq_dly, u4sample_cnt,u4error_cnt[0], u4error_cnt[2],u4error_cnt[1],u4error_cnt[3]));
                #endif
                #endif
            }
#endif

            #if RX_EYE_SAVE_TO_FILE
            arErrorCount[0][u2VrefIdx][u2DelayIdx] = u4error_cnt[0];
            arErrorCount[1][u2VrefIdx][u2DelayIdx] = u4error_cnt[1];
            if(p->dram_type == TYPE_LPDDR3)
            {
                arErrorCount[2][u2VrefIdx][u2DelayIdx] = u4error_cnt[2];
                arErrorCount[3][u2VrefIdx][u2DelayIdx] = u4error_cnt[3];
            }

            if(u2DelayIdx<RX_EYE_DELAY_LEVEL)
            {
                u2DelayIdx ++;
            }
            else
            {
                mcSHOW_DBG_MSG(("\n\nWARNING: Out of error count array size, Delay %d\n\n", u2DelayIdx));
            }
            #endif
        }

        #if RX_EYE_SAVE_TO_FILE
        if(u2VrefIdx<RX_EYE_VREF_LEVEL)
        {
            u2VrefIdx ++;
        }
        else
        {
            mcSHOW_DBG_MSG(("\n\nWARNING: Out of error count array size, Vref %d\n\n", u2VrefIdx));
        }
        #endif

        #if RX_EYE_SCAN_VREF_EXTEND
        s2vref-=u2VrefStep;

        if( s2vref < u2VrefBegin)  // switch to LP4 scan range
        {
            if(Vref_DDR4_SEL==0)
            {
                s2vref =(u2VrefEnd-1);
                Vref_DDR4_SEL =1; // LP4 range

                vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), 0, B0_DQ6_RG_RX_ARDQ_DDR3_SEL_B0);
                vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), 0, B1_DQ6_RG_RX_ARDQ_DDR3_SEL_B1);

                vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), 1, B0_DQ6_RG_RX_ARDQ_DDR4_SEL_B0);
                vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), 1, B1_DQ6_RG_RX_ARDQ_DDR4_SEL_B1);

                vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ5), 0x3f, SHU1_B0_DQ5_RG_RX_ARDQ_VREF_SEL_B0);
                vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DQ5), 0x3f, SHU1_B1_DQ5_RG_RX_ARDQ_VREF_SEL_B1);
                mcSHOW_DBG_MSG(("\n\nChange to LP4 Vref range\n\n"));
            }
            else
            {
                // Restore LP3 oringinal Vref setttings
                vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), 1, B0_DQ6_RG_RX_ARDQ_DDR3_SEL_B0);
                vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), 1, B1_DQ6_RG_RX_ARDQ_DDR3_SEL_B1);

                vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), 0, B0_DQ6_RG_RX_ARDQ_DDR4_SEL_B0);
                vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), 0, B1_DQ6_RG_RX_ARDQ_DDR4_SEL_B1);

                vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ5), 0xe, SHU1_B0_DQ5_RG_RX_ARDQ_VREF_SEL_B0);
                vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DQ5), 0xe, SHU1_B1_DQ5_RG_RX_ARDQ_VREF_SEL_B1);
                mcSHOW_DBG_MSG(("\n\nRestore LP3 Vref range\n\n"));
                break;
            }
        }
        #endif

        #if ENALBE_EYE_SCAN_LOG_DURING_PROCESS
        mcSHOW_DBG_MSG(("\n"));
        #else
        mcSHOW_DBG_MSG(("."));
        #endif
    }

    #if !ENALBE_EYE_SCAN_LOG_DURING_PROCESS
    mcSHOW_DBG_MSG(("\n"));
    #endif

#if 0  // When run time eye scan, don't disable RX eye scan.
    //Disable DQ eye scan (RG_RX_EYE_SCAN_EN=0)
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL_F), 0, STBCAL_F_RG_EX_EYE_SCAN_EN);

    if(u1IsLP4Family(p->dram_type))
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_EYE3), 1, EYE3_RG_RX_ARDQ_EYE_DLY_DQS_BYPASS_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_EYEB1_3), 1, EYEB1_3_RG_RX_ARDQ_EYE_DLY_DQS_BYPASS_B1);

        //Disable DQ eye scan (RG_RX_EYE_SCAN_EN=0, RG_RX_*RDQ_VREF_EN_B*=0, RG_RX_*RDQ_EYE_VREF_EN_B*=0, RG_RX_*RDQ_SMT_EN_B*=0)
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_EYE2), 0, EYE2_RG_RX_ARDQ_EYE_VREF_EN_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_EYEB1_2), 0, EYEB1_2_RG_RX_ARDQ_EYE_VREF_EN_B1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_TXDQ3), 0, TXDQ3_RG_RX_ARDQ_SMT_EN_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_RXDQ13),0, RXDQ13_RG_RX_ARDQ_SMT_EN_B1);
    }
#if ENABLE_LP3_SW
    else //LPDDR3
    {
        //Disable DQ eye scan (RG_RX_EYE_SCAN_EN=0, RG_RX_*RDQ_VREF_EN_B*=0, RG_RX_*RDQ_EYE_VREF_EN_B*=0, RG_RX_*RDQ_SMT_EN_B*=0)
        vIO32WriteFldAlign_All((DDRPHY_EYE2), 0, EYE2_RG_RX_ARDQ_EYE_VREF_EN_B0);
        vIO32WriteFldAlign_All((DDRPHY_EYEB1_2), 0, EYEB1_2_RG_RX_ARDQ_EYE_VREF_EN_B1);
        vIO32WriteFldAlign_All((DDRPHY_TXDQ3), 0, TXDQ3_RG_RX_ARDQ_SMT_EN_B0);
        vIO32WriteFldAlign_All((DDRPHY_RXDQ13),0, RXDQ13_RG_RX_ARDQ_SMT_EN_B1);
    }
#endif

#endif
    #if RX_EYE_SAVE_TO_FILE
    for(ii=0; ii<(p->data_width/DQS_BIT_NUMBER); ii++)
    {

        mcSHOW_DBG_MSG(("\n\tDQ RX eye scan (ch=%d, Byte %d, dq_%d, DramCBit %d, PHYBit %d)\n\n", p->channel, ii, dq_no, u1DramCBit, u1PHYBit));
        mcFPRINTF((fp_A60501,"\n\tDQ RX eye scan (ch=%d, Byte %d, dq_%d, DramCBit %d, PHYBit %d)\n", p->channel, ii, dq_no, u1DramCBit, u1PHYBit));

        for(jj=0; jj<RX_EYE_VREF_LEVEL; jj++)
        {
            #if RX_EYE_SCAN_VREF_EXTEND
            mcSHOW_DBG_MSG(("%d. Vref = %X, ", jj, arVrefLevelForLog[jj]));
            #endif

            for(kk=0; kk<RX_EYE_DELAY_LEVEL; kk++)
            {
                if(arErrorCount[ii][jj][kk] != 0xffff)
                {
                    mcSHOW_DBG_MSG(("%d, ", arErrorCount[ii][jj][kk]));
                    mcFPRINTF((fp_A60501, "%8d, ", arErrorCount[ii][jj][kk]));
                }
            }
            mcSHOW_DBG_MSG(("\n"));
            mcFPRINTF((fp_A60501, "\n"));
        }
        mcSHOW_DBG_MSG(("\n"));
        mcFPRINTF((fp_A60501, "\n"));
    }
    return DRAM_OK;

    // log example
    /*
 ===============================================================================
     DQ RX eye scan (channel=1, byte=0, dq_0)
 ===============================================================================
 2450, 2311, 2304, 2299, 1721, 1628, 1466,  946,  814,  635,  507,  232,   48,   26,    7,    1,    3,    4,    1,    5,   24,  140,  386, 1435, 2304, 2560, 2560, 2575, 2816, 2816, 2816, 2853, 3072, 3072, 3072,
 2313, 2304, 2304, 2202, 1655, 1620, 1223,  906,  713,  564,  278,   70,   28,    1,    0,    0,    0,    0,    0,    0,    1,    6,  130,  225, 1823, 2548, 2560, 2564, 2813, 2816, 2816, 3069, 3072, 3072, 3072,
 2305, 2304, 2304, 1786, 1620, 1555,  961,  816,  609,  444,   60,   43,    4,    0,    0,    0,    0,    0,    0,    0,    0,    0,    8,  127,  789, 2265, 2560, 2560, 2571, 2816, 2994, 3072, 3072, 3072, 3072,
 2305, 2304, 2246, 1686, 1589, 1248,  909,  660,  577,  231,   47,   31,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    5,  191, 1233, 2559, 2560, 2563, 2600, 3071, 3072, 3072, 3072, 3072,
 2304, 2304, 2028, 1627, 1375, 1027,  848,  639,  516,   88,   30,    1,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,   54,  617, 2175, 2554, 2560, 2571, 3072, 3072, 3072, 3072, 3072,
 2304, 2269, 1757, 1569, 1192,  947,  700,  559,  274,   44,    2,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  208, 1005, 2300, 2560, 2808, 2874, 3072, 3072, 3072, 3072,
 2302, 2141, 1672, 1350, 1000,  834,  602,  412,   63,   33,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,   67,  633, 1644, 2547, 2816, 2985, 3072, 3072, 3072, 3072,
 2301, 1936, 1559, 1159,  852,  658,  504,  223,   45,    7,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  278,  949, 2414, 2816, 3065, 3072, 3072, 3072, 3072,
 2189, 1920, 1444,  994,  692,  561,  327,   66,   42,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  110,  367, 1968, 2847, 3072, 3072, 3072, 3072, 3072,
 2059, 1967, 1452,  826,  609,  441,  105,   44,   29,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    1,  161, 1640, 2965, 3072, 3072, 3072, 3072, 3072,
 2208, 1948, 1561,  946,  548,  261,   46,   42,    5,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    1,  370, 1392, 2816, 3066, 3072, 3072, 3072, 3072,
 2303, 2019, 1546, 1074,  404,   57,   44,   30,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  250,  466, 1631, 2651, 2995, 3072, 3072, 3072, 3072,
 2305, 2270, 1573, 1279,  390,   72,   41,    4,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  418,  738, 1850, 2634, 3057, 3072, 3072, 3072, 3072,
 2305, 2303, 1655, 1334,  723,  171,   25,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  255,  712, 1468, 2149, 2709, 3072, 3072, 3072, 3072, 3072,
 2305, 2247, 1953, 1323,  947,  426,   14,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    1,  492, 1271, 1854, 2308, 2573, 3072, 3072, 3072, 3072, 3072,
 2303, 2221, 2211, 1449, 1150,  704,   91,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  320,  767, 1775, 2193, 2552, 2562, 2923, 3072, 3072, 3072, 3072,

 ===============================================================================
     DQ RX eye scan (channel=1, byte=1, dq_0)
 ===============================================================================
 2311, 2307, 2304, 2278, 2016, 1674, 1280, 1214,  929,  503,  377,  365,  359,  343,  349,  359,  373,  377,  396,  446, 1102, 1977, 2550, 2563, 2563, 2572, 2816, 2816, 2816, 2816, 3066, 3071, 3072, 3072, 3072,
 2308, 2304, 2304, 2150, 1832, 1458, 1270, 1137,  615,  362,  232,  149,  139,  140,  144,  163,  211,  269,  336,  359,  673, 1300, 2223, 2555, 2565, 2564, 2782, 2816, 2816, 2816, 3070, 3072, 3072, 3072, 3072,
 2304, 2304, 2304, 2050, 1708, 1307, 1198,  924,  388,  168,   59,   38,   24,   28,   27,   44,   82,  129,  199,  325,  354,  854, 1433, 2367, 2561, 2563, 2572, 2816, 2816, 2927, 3072, 3072, 3072, 3072, 3072,
 2305, 2304, 2303, 2013, 1516, 1268, 1101,  545,  179,   54,    3,    2,    0,    1,    1,    2,    1,   28,   88,  141,  322,  590, 1097, 1621, 2535, 2564, 2562, 2816, 2816, 3069, 3072, 3072, 3072, 3072, 3072,
 2304, 2304, 2168, 1838, 1332, 1213,  815,  304,   73,    2,    4,    0,    0,    0,    1,    1,    1,    1,    2,   24,  135,  295,  726, 1170, 2182, 2560, 2562, 2579, 2816, 3072, 3072, 3072, 3072, 3072, 3072,
 2304, 2304, 1937, 1672, 1271, 1125,  474,  124,   22,    0,    0,    0,    0,    1,    0,    0,    2,    2,    0,    0,   28,  166,  513,  703, 1374, 2506, 2561, 2565, 2998, 3072, 3072, 3072, 3072, 3072, 3072,
 2304, 2299, 1813, 1468, 1201,  873,  178,   47,    3,    1,    0,    0,    0,    0,    0,    0,    0,    0,    0,    1,    2,   58,  189,  588, 1062, 1768, 2560, 2563, 2878, 3072, 3072, 3072, 3072, 3072, 3072,
 2304, 2086, 1778, 1308, 1153,  475,   56,   31,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    1,    0,  134,  206,  669, 1149, 2491, 2574, 2820, 3066, 3072, 3072, 3072, 3072, 3072,
 2246, 1862, 1645, 1201,  844,  166,   41,    6,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    1,   11,  112,  503,  949, 1626, 2808, 2818, 3072, 3072, 3072, 3072, 3072, 3072,
 2011, 1775, 1426, 1170,  430,   61,   10,    1,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    1,    1,   13,  185,  665, 1173, 2617, 2904, 3072, 3072, 3072, 3072, 3072, 3072,
 1967, 1725, 1219,  842,  224,   43,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    1,   78,  431, 1161, 2096, 3062, 3072, 3072, 3072, 3072, 3072, 3072,
 2044, 1749, 1098,  433,   83,   18,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    1,    5,  188,  940, 1751, 3060, 3072, 3072, 3072, 3072, 3072, 3072,
 2048, 1878,  940,  316,   45,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    1,  316,  853, 2013, 3016, 3072, 3072, 3072, 3072, 3072, 3072,
 2048, 1978,  934,  294,   56,    1,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    1,  260,  925, 2077, 2667, 3072, 3072, 3072, 3072, 3072, 3072,
 2048, 1974, 1267,  402,   91,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  211,  313, 1214, 2095, 2596, 3065, 3072, 3072, 3072, 3072, 3072,
 2056, 1963, 1556,  535,  130,    2,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    1,  254,  728, 1563, 2004, 2892, 3072, 3072, 3072, 3072, 3072, 3072,
   */
#endif
}
#endif


#ifdef TX_EYE_SCAN
#define TX_EYE_VREF_LEVEL   32
#define TX_EYE_DELAY_LEVEL  32//40
#define TX_EYE_SAVE_TO_FILE 1
#define ENALBE_EYE_SCAN_LOG_DURING_PROCESS 1
#define TX_EYE_SCAN_VREF_EXTEND 1


void DramcTxEyeScanInit(DRAMC_CTX_T *p)
{
#if 0
    vIO32WriteFldMulti((DDRPHY_PLL15)+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0, PLL15_RG_RPHYPLL_TST_SEL) | P_Fld(1, PLL15_RG_RPHYPLL_TST_EN) | \
                            P_Fld(0, PLL15_RG_RPHYPLL_TSTCK_EN) | P_Fld(0, PLL15_RG_RPHYPLL_TSTFM_EN) | \
                            P_Fld(0, PLL15_RG_RPHYPLL_TSTOD_EN) | P_Fld(1, PLL15_RG_RPHYPLL_TSTOP_EN) | \
                            P_Fld(1, PLL15_RG_RVREF_VREF_EN) );
    vIO32WriteFldMulti((DDRPHY_PLL15)+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x3f, PLL15_RG_RVREF_SEL_DQ) | \
                            P_Fld(1, PLL15_RG_RVREF_DDR3_SEL) | P_Fld(0, PLL15_RG_RVREF_DDR4_SEL));
#endif
}

DRAM_STATUS_T DramcTxEyeScanDelay(DRAMC_CTX_T *p, S16 uiDelay[], S16 uiOenDelay[])
{
#ifndef OLYMPUS_TO_BE_PORTING

    U8 u1ByteIdx;
    U8 ucdq_pi[DQS_NUMBER], ucdq_ui_small[DQS_NUMBER], ucdq_ui_large[DQS_NUMBER];
    U8 ucdq_oen_ui_small[DQS_NUMBER], ucdq_oen_ui_large[DQS_NUMBER];

    for(u1ByteIdx=0; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
    {
        TxWinTransferDelayToUIPI(p, uiDelay[u1ByteIdx], 0, 0, &(ucdq_ui_large[u1ByteIdx]), &ucdq_ui_small[u1ByteIdx], &ucdq_pi[u1ByteIdx]);
        TxWinTransferDelayToUIPI(p, uiOenDelay[u1ByteIdx], 0, 0, &ucdq_oen_ui_large[u1ByteIdx], &ucdq_oen_ui_small[u1ByteIdx], &ucdq_pi[u1ByteIdx]);

        #if ENALBE_EYE_SCAN_LOG_DURING_PROCESS
        #ifdef ETT_PRINT_FORMAT
        mcSHOW_DBG_MSG(("Delay = %d |%d %d %d \n",uiDelay[u1ByteIdx], ucdq_ui_large[u1ByteIdx],ucdq_ui_small[u1ByteIdx], ucdq_pi[u1ByteIdx]));
        #else
        mcSHOW_DBG_MSG(("Delay = %d |%4d %4d %4d \n",uiDelay[u1ByteIdx], ucdq_ui_large[u1ByteIdx],ucdq_ui_small[u1ByteIdx], ucdq_pi[u1ByteIdx]));
        #endif
        mcFPRINTF((fp_A60501, "Delay = %d | %4d %4d %4d \n",uiDelay[u1ByteIdx], ucdq_ui_large[u1ByteIdx],ucdq_ui_small[u1ByteIdx], ucdq_pi[u1ByteIdx]));
        #endif //#if ENALBE_EYE_SCAN_LOG_DURING_PROCESS
    }

    //mcSHOW_DBG_MSG(("\nTX manual DQ delay = %d\n",uiDelay));
    //mcSHOW_DBG_MSG(("Reg Values = (%d, %d, %d)\n",ucdq_ui_large, ucdq_ui_small,ucdq_pi ));
    //mcSHOW_DBG_MSG(("Reg OEN Values = (%d, %d, %d)\n",ucdq_oen_ui_large, ucdq_oen_ui_small, ucdq_pi));

    //TXDLY_DQ , TXDLY_OEN_DQ
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SELPH7), \
                                        P_Fld(ucdq_ui_large[0], SELPH7_TXDLY_DQ0) | \
                                        P_Fld(ucdq_ui_large[1], SELPH7_TXDLY_DQ1) | \
                                        P_Fld(ucdq_ui_large[2], SELPH7_TXDLY_DQ2) | \
                                        P_Fld(ucdq_ui_large[3], SELPH7_TXDLY_DQ3) | \
                                        P_Fld(ucdq_oen_ui_large[0], SELPH7_TXDLY_OEN_DQ0) | \
                                        P_Fld(ucdq_oen_ui_large[1], SELPH7_TXDLY_OEN_DQ1) | \
                                        P_Fld(ucdq_oen_ui_large[2], SELPH7_TXDLY_OEN_DQ2) | \
                                        P_Fld(ucdq_oen_ui_large[3], SELPH7_TXDLY_OEN_DQ3));

    // DLY_DQ[1:0]
   vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SELPH10), \
                                    P_Fld(ucdq_ui_small[0], SELPH10_DLY_DQ0) | \
                                    P_Fld(ucdq_ui_small[1], SELPH10_DLY_DQ1) | \
                                    P_Fld(ucdq_ui_small[2], SELPH10_DLY_DQ2) | \
                                    P_Fld(ucdq_ui_small[3], SELPH10_DLY_DQ3) | \
                                    P_Fld(ucdq_oen_ui_small[0], SELPH10_DLY_OEN_DQ0) | \
                                    P_Fld(ucdq_oen_ui_small[1], SELPH10_DLY_OEN_DQ1) | \
                                    P_Fld(ucdq_oen_ui_small[2], SELPH10_DLY_OEN_DQ2) | \
                                    P_Fld(ucdq_oen_ui_small[3], SELPH10_DLY_OEN_DQ3));
    // DLY_DQ[2]
   vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SELPH22), \
                                    P_Fld(ucdq_ui_small[0]>>2, SELPH22_DLY_DQ0_2) | \
                                    P_Fld(ucdq_ui_small[1]>>2, SELPH22_DLY_DQ1_2)| \
                                    P_Fld(ucdq_ui_small[2]>>2, SELPH22_DLY_DQ2_2) | \
                                    P_Fld(ucdq_ui_small[3]>>2, SELPH22_DLY_DQ3_2) | \
                                    P_Fld(ucdq_oen_ui_small[0]>>2, SELPH22_DLY_OEN_DQ0_2) | \
                                    P_Fld(ucdq_oen_ui_small[1]>>2, SELPH22_DLY_OEN_DQ1_2) | \
                                    P_Fld(ucdq_oen_ui_small[2]>>2, SELPH22_DLY_OEN_DQ2_2) | \
                                    P_Fld(ucdq_oen_ui_small[3]>>2, SELPH22_DLY_OEN_DQ3_2));

    //TXDLY_DQM , TXDLY_OEN_DQM
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SELPH8), \
                                        P_Fld(ucdq_ui_large[0], SELPH8_TXDLY_DQM0) | \
                                        P_Fld(ucdq_ui_large[1], SELPH8_TXDLY_DQM1) | \
                                        P_Fld(ucdq_ui_large[2], SELPH8_TXDLY_DQM2) | \
                                        P_Fld(ucdq_ui_large[3], SELPH8_TXDLY_DQM3) | \
                                        P_Fld(ucdq_oen_ui_large[0], SELPH8_TXDLY_OEN_DQM0) | \
                                        P_Fld(ucdq_oen_ui_large[1], SELPH8_TXDLY_OEN_DQM1) | \
                                        P_Fld(ucdq_oen_ui_large[2], SELPH8_TXDLY_OEN_DQM2) | \
                                        P_Fld(ucdq_oen_ui_large[3], SELPH8_TXDLY_OEN_DQM3));

    // DLY_DQM[1:0]
   vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SELPH10), \
                                    P_Fld(ucdq_ui_small[0], SELPH10_DLY_DQM0) | \
                                    P_Fld(ucdq_ui_small[1], SELPH10_DLY_DQM1) | \
                                    P_Fld(ucdq_ui_small[2], SELPH10_DLY_DQM2) | \
                                    P_Fld(ucdq_ui_small[3], SELPH10_DLY_DQM3) | \
                                    P_Fld(ucdq_oen_ui_small[0], SELPH10_DLY_OEN_DQM0) | \
                                    P_Fld(ucdq_oen_ui_small[1], SELPH10_DLY_OEN_DQM1) | \
                                    P_Fld(ucdq_oen_ui_small[2], SELPH10_DLY_OEN_DQM2) | \
                                    P_Fld(ucdq_oen_ui_small[3], SELPH10_DLY_OEN_DQM3));
    // DLY_DQM[2]
   vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SELPH22), \
                                    P_Fld(ucdq_ui_small[0]>>2, SELPH22_DLY_DQM0_2) | \
                                    P_Fld(ucdq_ui_small[1]>>2, SELPH22_DLY_DQM1_2)| \
                                    P_Fld(ucdq_ui_small[2]>>2, SELPH22_DLY_DQM2_2) | \
                                    P_Fld(ucdq_ui_small[3]>>2, SELPH22_DLY_DQM3_2) | \
                                    P_Fld(ucdq_oen_ui_small[0]>>2, SELPH22_DLY_OEN_DQM0_2) | \
                                    P_Fld(ucdq_oen_ui_small[1]>>2, SELPH22_DLY_OEN_DQM1_2) | \
                                    P_Fld(ucdq_oen_ui_small[2]>>2, SELPH22_DLY_OEN_DQM2_2) | \
                                    P_Fld(ucdq_oen_ui_small[3]>>2, SELPH22_DLY_OEN_DQM3_2));

    //set to registers, PI DQ (per byte)
    if(u1IsLP4Family(p->dram_type))
    {
        // update TX DQ PI delay, for rank 0 // need to take care rank 1 and 2
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_ARPI_DQ), \
                                        P_Fld(ucdq_pi, ARPI_DQ_RK0_ARPI_DQ_B0) | P_Fld(ucdq_pi, ARPI_DQ_RK0_ARPI_DQ_B1));
    }
#if ENABLE_LP3_SW
    else //LPDDR3
    {
        vIO32WriteFldMulti(DDRPHY_ARPI_DQ+(aru1PhyMap2Channel[0]<< POS_BANK_NUM),\
                                    P_Fld(ucdq_pi[0], ARPI_DQ_RK0_ARPI_DQ_B0) | P_Fld(ucdq_pi[2], ARPI_DQ_RK0_ARPI_DQ_B1));

        vIO32WriteFldMulti(DDRPHY_ARPI_DQ+(aru1PhyMap2Channel[1]<< POS_BANK_NUM), \
                                        P_Fld(ucdq_pi[1], ARPI_DQ_RK0_ARPI_DQ_B0) | P_Fld(ucdq_pi[3], ARPI_DQ_RK0_ARPI_DQ_B1));
    }
#endif

#if 0
    u4err_value= TestEngineCompare(p);
#endif
    //DramPhyReset(p);

#endif
	return DRAM_OK;
}


//-------------------------------------------------------------------------
/** DramcTxEyeScan
 *  start the rx dq eye scan.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param dq_no            (U8): 0~7.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL
 */
//-------------------------------------------------------------------------
//#define EMI_APB_BASE    0x10203000
#define EMI_APB_BASE    0x10219000

DRAM_STATUS_T DramcTxEyeScanRun(DRAMC_CTX_T *p, U8 u1PatternDMA)
{
#ifndef OLYMPUS_TO_BE_PORTING

    S16 s2vref, s2dq_dly, s2Delay[DQS_NUMBER],  s2Delay_OEN[DQS_NUMBER];
    U32  u4err_value = 0xffffffff, u4sample_cnt=0, u4error_cnt[DQS_NUMBER];
    #if TX_EYE_SAVE_TO_FILE
    static U8 arErrorCount[DQ_DATA_WIDTH][TX_EYE_VREF_LEVEL][TX_EYE_DELAY_LEVEL];
    static U16 arVrefLevelForLog[TX_EYE_VREF_LEVEL];
    U16 u2VrefIdx, u2DelayIdx, ii,jj, kk;
    #endif
    U16 u2VrefBegin, u2VrefEnd, u2VrefStep;
    U8 u1ByteIdx, u1BitIdx;
    U32 uiSrcAddr, uiDestAddr, uiLen;

#if TX_EYE_SCAN_VREF_EXTEND
    U8 Vref_DDR4_SEL =0;
#endif
    mcSHOW_DBG_MSG(("[TxEyeScan]\n"));
    // error handling
    if (!p)
    {
        mcSHOW_ERR_MSG(("context NULL\n"));
        return DRAM_FAIL;
    }

    // check if SoC platform has "%" operation?!
    #if TX_EYE_SAVE_TO_FILE
    for(ii=0; ii<DQ_DATA_WIDTH; ii++)
    {
        for(jj=0; jj<TX_EYE_VREF_LEVEL; jj++)
        {
            for(kk=0; kk<TX_EYE_DELAY_LEVEL; kk++)
            {
                arErrorCount[ii][jj][kk] =0xff;
            }
        }
    }
    #endif

    //DMA init address
    if(u1PatternDMA)
    {
        uiSrcAddr = 0x50000000;
        uiDestAddr = 0x58000000;
        uiLen = 0xff000;

	*(volatile unsigned *)(EMI_APB_BASE+0x00000000) &= (~0x01);	// disable dual channel.  SW hang. Don't use change channel on-the-fly

        if(p->channel ==CHANNEL_A)
        {
            *(volatile unsigned *)(EMI_APB_BASE+ 0x000) &= (~(0x01<<18));
        }
        else
        {
            *(volatile unsigned *)(EMI_APB_BASE+ 0x000) |= ((0x01<<18));
        }

        DramcDmaEngine((DRAMC_CTX_T *)p, DMA_OP_READ_WRITE, uiSrcAddr, uiDestAddr, uiLen, 8, DMA_PREPARE_DATA_ONLY, 1);
    }

    u2VrefBegin =0;
    u2VrefEnd =0x3f;
    u2VrefStep=4;

    #if TX_EYE_SAVE_TO_FILE
    u2VrefIdx =0;
    #endif

    #if TX_EYE_SCAN_VREF_EXTEND
    s2vref =u2VrefEnd;

    while(1)
    #else
    for(s2vref =u2VrefEnd; s2vref > u2VrefBegin; s2vref-=u2VrefStep)
    #endif
    {
        #if TX_EYE_SCAN_VREF_EXTEND
        arVrefLevelForLog[u2VrefIdx] = s2vref;
        #endif

        //Set Vref voltage
        vIO32WriteFldAlign((DDRPHY_PLL15) + ((U32)CHANNEL_B<<POS_BANK_NUM), s2vref, PLL15_RG_RVREF_SEL_DQ);

        //Wait for Vref settles down, 1sec. is enough
        mcDELAY_MS(1000);

 	 #if TX_EYE_SAVE_TO_FILE
        u2DelayIdx=0;
        #endif

        for (s2dq_dly=-15; s2dq_dly <20; s2dq_dly+=1)
        {
            for(u1ByteIdx=0; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
            {
                s2Delay[u1ByteIdx] = (S16)aru2TXCaliDelay[p->channel][u1ByteIdx] +s2dq_dly;
                s2Delay_OEN[u1ByteIdx] = (S16)aru2TXCaliDelay_OEN[p->channel][u1ByteIdx] +s2dq_dly;
            }

            DramcTxEyeScanDelay(p, s2Delay, s2Delay_OEN);

            if(u1PatternDMA)
            {
                if(p->support_channel_num == CHANNEL_SINGLE)
                {
                    DramcDmaEngine((DRAMC_CTX_T *)p, DMA_OP_READ_WRITE, uiSrcAddr, uiDestAddr, uiLen, 8, DMA_CHECK_DATA_ACCESS_ONLY_AND_NO_WAIT, 1);
                    u4err_value = DramcDmaEngine((DRAMC_CTX_T *)p, DMA_OP_READ_WRITE, uiSrcAddr, uiDestAddr, uiLen, 8, DMA_CHECK_COMAPRE_RESULT_ONLY, 1);
                }
                else
                {
                    DramcDmaEngine((DRAMC_CTX_T *)p, DMA_OP_READ_WRITE, uiSrcAddr, uiDestAddr, uiLen, 8, DMA_CHECK_DATA_ACCESS_ONLY_AND_NO_WAIT, 2);
                    u4err_value = DramcDmaEngine((DRAMC_CTX_T *)p, DMA_OP_READ_WRITE, uiSrcAddr, uiDestAddr, uiLen, 8, DMA_CHECK_COMAPRE_RESULT_ONLY, 2);
                }
            }
            else
                u4err_value= TestEngineCompare(p);

            #if ENALBE_EYE_SCAN_LOG_DURING_PROCESS
            #ifdef ETT_PRINT_FORMAT
            mcSHOW_DBG_MSG(("TX Vref= %d, Delay= %d,  u4err_value=%X\n", s2vref, s2dq_dly, u4err_value));
            #else
            mcSHOW_DBG_MSG(("TX Vref= %2d, Delay= %4d, u4err_value= %x\n", s2vref, s2dq_dly, u4err_value));
            #endif
            #endif

            #if TX_EYE_SAVE_TO_FILE

            for(u1BitIdx=0; u1BitIdx<DQ_DATA_WIDTH; u1BitIdx++)
            {
                arErrorCount[u1BitIdx][u2VrefIdx][u2DelayIdx] = ((u4err_value >>u1BitIdx)& 0x1);
            }

            if(u2DelayIdx<TX_EYE_DELAY_LEVEL)
            {
                u2DelayIdx ++;
            }
            else
            {
                mcSHOW_DBG_MSG(("\n\nWARNING: Out of error count array size, Delay %d\n\n", u2DelayIdx));
            }
            #endif
        }

        #if TX_EYE_SAVE_TO_FILE
        if(u2VrefIdx<TX_EYE_VREF_LEVEL)
        {
            u2VrefIdx ++;
        }
        else
        {
            mcSHOW_DBG_MSG(("\n\nWARNING: Out of error count array size, Vref %d\n\n", u2VrefIdx));
        }
        #endif

        #if TX_EYE_SCAN_VREF_EXTEND
        s2vref-=u2VrefStep;

        if( s2vref < u2VrefBegin)  // switch to LP4 scan range
        {
            if(Vref_DDR4_SEL==0)
            {
                s2vref =u2VrefEnd;
                Vref_DDR4_SEL =1; // LP4 range

                vIO32WriteFldAlign((DDRPHY_PLL15)+((U32)CHANNEL_B<<POS_BANK_NUM), 0, PLL15_RG_RVREF_DDR3_SEL);
                vIO32WriteFldAlign((DDRPHY_PLL15)+((U32)CHANNEL_B<<POS_BANK_NUM), 1, PLL15_RG_RVREF_DDR4_SEL);
                //vIO32WriteFldAlign((DDRPHY_PLL15)+((U32)CHANNEL_B<<POS_BANK_NUM), 0x3f, PLL15_RG_RVREF_SEL_DQ);
                mcSHOW_DBG_MSG(("\n\nChange to LP4 Vref range\n\n"));
            }
            else
            {
                // Restore LP3 oringinal Vref setttings
                vIO32WriteFldAlign((DDRPHY_PLL15)+((U32)CHANNEL_B<<POS_BANK_NUM), 1, PLL15_RG_RVREF_DDR3_SEL);
                vIO32WriteFldAlign((DDRPHY_PLL15)+((U32)CHANNEL_B<<POS_BANK_NUM), 0, PLL15_RG_RVREF_DDR4_SEL);
                vIO32WriteFldAlign((DDRPHY_PLL15)+((U32)CHANNEL_B<<POS_BANK_NUM), 0xf, PLL15_RG_RVREF_SEL_DQ);
                mcSHOW_DBG_MSG(("\n\nRestore LP3 Vref range\n\n"));

                mcDELAY_MS(1000);

                for(u1ByteIdx=0; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
                {
                    s2Delay[u1ByteIdx] = (S16)aru2TXCaliDelay[p->channel][u1ByteIdx];
                    s2Delay_OEN[u1ByteIdx] = (S16)aru2TXCaliDelay_OEN[p->channel][u1ByteIdx];
                }
                DramcTxEyeScanDelay(p, s2Delay, s2Delay_OEN);

                if(u1PatternDMA)
                {
                    DramcDmaEngine((DRAMC_CTX_T *)p, DMA_OP_READ_WRITE, uiSrcAddr, uiDestAddr, uiLen, 8, DMA_CHECK_DATA_ACCESS_ONLY_AND_NO_WAIT, 1);
                }

                break;
            }
        }
        #endif

        #if ENALBE_EYE_SCAN_LOG_DURING_PROCESS
        mcSHOW_DBG_MSG(("\n"));
        #else
        mcSHOW_DBG_MSG(("."));
        #endif
    }

    #if !ENALBE_EYE_SCAN_LOG_DURING_PROCESS
    mcSHOW_DBG_MSG(("\n"));
    #endif

    #if TX_EYE_SAVE_TO_FILE
    for(ii=0; ii<DQ_DATA_WIDTH; ii++)
    {
        mcSHOW_DBG_MSG(("\n\tDQ TX eye scan (ch=%d, dq_%d)\n\n", p->channel, ii));

        mcFPRINTF((fp_A60501,"\n\tDQ TX eye scan (ch=%d, dq_%d)\n\n", p->channel, ii));

        for(jj=0; jj<TX_EYE_VREF_LEVEL; jj++)
        {
            #if TX_EYE_SCAN_VREF_EXTEND
            mcSHOW_DBG_MSG(("%d. Vref = %X, ", jj, arVrefLevelForLog[jj]));
            #endif

            for(kk=0; kk<TX_EYE_DELAY_LEVEL; kk++)
            {
                if(arErrorCount[ii][jj][kk] != 0xff)
                {
                    mcSHOW_DBG_MSG(("%d, ", arErrorCount[ii][jj][kk]));
                    mcFPRINTF((fp_A60501, "%8d, ", arErrorCount[ii][jj][kk]));
                     mcDELAY_MS(1);
                }
            }
            mcSHOW_DBG_MSG(("\n"));
            mcFPRINTF((fp_A60501, "\n"));
        }
        mcSHOW_DBG_MSG(("\n"));
        mcFPRINTF((fp_A60501, "\n"));
    }
    #endif
    return DRAM_OK;

    // log example
    /*
 ===============================================================================
     DQ TX eye scan (channel=0    dq_18)
 ===============================================================================
 0. Vref = 0000003F  1   1   1   1   1   1   0   0   0   0   0   0   0   0   0   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1
 1. Vref = 0000003B  1   1   1   1   1   1   0   0   0   0   0   0   0   0   0   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1
 2. Vref = 00000037  1   1   1   1   1   1   0   0   0   0   0   0   0   0   0   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1
 3. Vref = 00000033  1   1   1   1   1   1   0   0   0   0   0   0   0   0   0   0   1   1   1   1   1   1   1   1   1   1   1   1   1   1
 4. Vref = 0000002F  1   1   1   1   1   1   0   0   0   0   0   0   0   0   0   0   1   1   1   1   1   1   1   1   1   1   1   1   1   1
 5. Vref = 0000002B  1   1   1   1   1   1   0   0   0   0   0   0   0   0   0   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1
 6. Vref = 00000027  1   1   1   1   1   1   0   0   0   0   0   0   0   0   0   0   1   1   1   1   1   1   1   1   1   1   1   1   1   1
 7. Vref = 00000023  1   1   1   1   1   1   0   0   0   0   0   0   0   0   0   0   1   1   1   1   1   1   1   1   1   1   1   1   1   1
 8. Vref = 0000001F  1   1   1   1   1   0   0   0   0   0   0   0   0   0   0   0   0   1   1   1   1   1   1   1   1   1   1   1   1   1
 9. Vref = 0000001B  1   1   1   1   1   0   0   0   0   0   0   0   0   0   0   0   0   1   1   1   1   1   1   1   1   1   1   1   1   1
 10. Vref = 00000017 1   1   1   1   1   0   0   0   0   0   0   0   0   0   0   0   0   1   1   1   1   1   1   1   1   1   1   1   1   1
 11. Vref = 00000013 1   1   1   1   1   0   0   0   0   0   0   0   0   0   0   0   0   1   1   1   1   1   1   1   1   1   1   1   1   1
 12. Vref = 0000000F 1   1   1   1   1   0   0   0   0   0   0   0   0   0   0   0   0   1   1   1   1   1   1   1   1   1   1   1   1   1
 13. Vref = 0000000B 1   1   1   1   1   0   0   0   0   0   0   0   0   0   0   0   0   1   1   1   1   1   1   1   1   1   1   1   1   1
 14. Vref = 00000007 1   1   1   1   1   0   0   0   0   0   0   0   0   0   0   0   1   1   1   1   1   1   1   1   1   1   1   1   1   1
 15. Vref = 00000003 1   1   1   1   1   0   0   0   0   0   0   0   0   0   0   0   1   1   1   1   1   1   1   1   1   1   1   1   1   1
 16. Vref = 0000003F 1   1   1   1   1   0   0   0   0   0   0   0   0   0   0   0   1   1   1   1   1   1   1   1   1   1   1   1   1   1
 17. Vref = 0000003B 1   1   1   1   1   0   0   0   0   0   0   0   0   0   0   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1
 18. Vref = 00000037 1   1   1   1   1   1   0   0   0   0   0   0   0   0   0   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1
 19. Vref = 00000033 1   1   1   1   1   0   0   0   0   0   0   0   0   0   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1
 20. Vref = 0000002F 1   1   1   1   1   1   0   0   0   0   0   0   0   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1
 21. Vref = 0000002B 1   1   1   1   1   1   0   0   0   0   0   0   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1
 22. Vref = 00000027 1   1   1   1   1   1   0   0   0   0   0   0   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1
 23. Vref = 00000023 1   1   1   1   1   1   0   0   0   0   0   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1
 24. Vref = 0000001F 1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1
 25. Vref = 0000001B 1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1
 26. Vref = 00000017 1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1
 27. Vref = 00000013 1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1
 28. Vref = 0000000F 1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1
 29. Vref = 0000000B 1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1
 30. Vref = 00000007 1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1
 31. Vref = 00000003 1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1
   */
#endif
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
    U8 ucsearch_state, ucdqs_dly, fgcurrent_value, fginitial_value, ucstart_period, ucend_period=0;
    U32 u4sample_cnt, u4ones_cnt[DQS_NUMBER], u4MPDIV_IN_SEL;
    U16 u2real_freq, u2real_period;
    U32 u4prv_register_B0_DQ6[CHANNEL_NUM], u4prv_register_B1_DQ6[CHANNEL_NUM];
    U32 u4prv_register_B0_DQ5[CHANNEL_NUM], u4prv_register_B1_DQ5[CHANNEL_NUM];
    U32 u4prv_register_B0_DQ4[CHANNEL_NUM], u4prv_register_B1_DQ4[CHANNEL_NUM];
    U32 u4prv_register_B0_DQ3[CHANNEL_NUM], u4prv_register_B1_DQ3[CHANNEL_NUM];
    U32 u4prv_register_CA_CMD4[CHANNEL_NUM], u4prv_register_CA_CMD6[CHANNEL_NUM];
    U32 u4prv_register_SHU1_B0_DQ7[CHANNEL_NUM], u4prv_register_SHU1_B1_DQ7[CHANNEL_NUM];
    U32 u4prv_register_MISC_CTRL1[CHANNEL_NUM];
    U32 u4prv_register_EYESCAN, u4prv_register_STBCAL1;

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

    u2gdelay_cell_ps=0;

    // error handling
    if (!p)
    {
        mcSHOW_ERR_MSG(("context NULL\n"));
        return DRAM_FAIL;
    }

    //backup register value
    u4prv_register_EYESCAN = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN));
    u4prv_register_STBCAL1 = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_STBCAL1));

    if(u1IsLP4Family(p->dram_type))
    {
        u4prv_register_B0_DQ6[0] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_B0_DQ6));
        u4prv_register_B1_DQ6[0] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_B1_DQ6));
        u4prv_register_B0_DQ5[0] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_B0_DQ5));
        u4prv_register_B1_DQ5[0] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_B1_DQ5));
        u4prv_register_B0_DQ3[0] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_B0_DQ3));
        u4prv_register_B1_DQ3[0] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_B1_DQ3));
        u4prv_register_SHU1_B0_DQ7[0] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ7));
        u4prv_register_SHU1_B1_DQ7[0] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DQ7));
        u4prv_register_B0_DQ4[0] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_B0_DQ4));
        u4prv_register_B1_DQ4[0] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_B1_DQ4));
        u4prv_register_MISC_CTRL1[0] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1));
    }
#if ENABLE_LP3_SW
    else //LPDDR3
    {
        U8 u1PhyIdx = 0;
        for(u1PhyIdx=0; u1PhyIdx<2; u1PhyIdx++)
        {
            u4prv_register_B0_DQ6[u1PhyIdx] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_B0_DQ6)+(u1PhyIdx<<POS_BANK_NUM));
            u4prv_register_B1_DQ6[u1PhyIdx] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_B1_DQ6)+(u1PhyIdx<<POS_BANK_NUM));
            u4prv_register_B0_DQ5[u1PhyIdx] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_B0_DQ5)+(u1PhyIdx<<POS_BANK_NUM));
            u4prv_register_B1_DQ5[u1PhyIdx] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_B1_DQ5)+(u1PhyIdx<<POS_BANK_NUM));
            u4prv_register_B0_DQ3[u1PhyIdx] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_B0_DQ3)+(u1PhyIdx<<POS_BANK_NUM));
            u4prv_register_B1_DQ3[u1PhyIdx] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_B1_DQ3)+(u1PhyIdx<<POS_BANK_NUM));
            u4prv_register_SHU1_B0_DQ7[u1PhyIdx] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ7)+(u1PhyIdx<<POS_BANK_NUM));
            u4prv_register_SHU1_B1_DQ7[u1PhyIdx] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DQ7)+(u1PhyIdx<<POS_BANK_NUM));
            u4prv_register_B0_DQ4[u1PhyIdx] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_B0_DQ4)+(u1PhyIdx<<POS_BANK_NUM));
            u4prv_register_B1_DQ4[u1PhyIdx] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_B1_DQ4)+(u1PhyIdx<<POS_BANK_NUM));
            u4prv_register_CA_CMD4[u1PhyIdx] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_CA_CMD4)+(u1PhyIdx<<POS_BANK_NUM));
            u4prv_register_CA_CMD6[u1PhyIdx] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_CA_CMD6)+(u1PhyIdx<<POS_BANK_NUM));
            u4prv_register_MISC_CTRL1[u1PhyIdx] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1)+(u1PhyIdx<<POS_BANK_NUM));
        }
    }
#endif

    if(u1IsLP4Family(p->dram_type))
    {
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
        vIO32WriteFldMulti(DRAMC_REG_EYESCAN, P_Fld(0x1, EYESCAN_EYESCAN_DQS_SYNC_EN)    
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
    }
#if ENABLE_LP3_SW
    else
    {
        //MCK4X CG
        vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 0, MISC_CTRL1_R_DMDQSIENCG_EN);

        // Bypass DQS glitch-free mode
        // RG_RX_*RDQ_EYE_DLY_DQS_BYPASS_B**
        vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), 1, B0_DQ6_RG_RX_ARDQ_EYE_DLY_DQS_BYPASS_B0);
        vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), 1, B1_DQ6_RG_RX_ARDQ_EYE_DLY_DQS_BYPASS_B1);

        //Enable DQ eye scan
        //RG_??_RX_EYE_SCAN_EN
        //RG_??_RX_VREF_EN
        //RG_??_RX_SMT_EN
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 1, EYESCAN_RG_RX_EYE_SCAN_EN);
        vIO32WriteFldMulti(DRAMC_REG_EYESCAN, P_Fld(0x1, EYESCAN_EYESCAN_DQS_SYNC_EN)    
                                            | P_Fld(0x1, EYESCAN_EYESCAN_NEW_DQ_SYNC_EN)    
                                            | P_Fld(0x1, EYESCAN_EYESCAN_DQ_SYNC_EN));   
        vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ5), 1, B0_DQ5_RG_RX_ARDQ_EYE_EN_B0);
        vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ5), 1, B1_DQ5_RG_RX_ARDQ_EYE_EN_B1);
        vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ5), 1, B0_DQ5_RG_RX_ARDQ_VREF_EN_B0);
        vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ5), 1, B1_DQ5_RG_RX_ARDQ_VREF_EN_B1);
        vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ3), 1, B0_DQ3_RG_RX_ARDQ_SMT_EN_B0);
        vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ3), 1, B1_DQ3_RG_RX_ARDQ_SMT_EN_B1);

        //JM_SEL
        vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), 1, B0_DQ6_RG_RX_ARDQ_JM_SEL_B0);
        vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), 1, B1_DQ6_RG_RX_ARDQ_JM_SEL_B1);
        vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_CA_CMD6), 1, CA_CMD6_RG_RX_ARCMD_JM_SEL);
    }
#endif


    //Enable MIOCK jitter meter mode ( RG_RX_MIOCK_JIT_EN=1)
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 1, EYESCAN_RG_RX_MIOCK_JIT_EN);

    //Disable DQ eye scan (b'1), for counter clear
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 0, EYESCAN_RG_RX_EYE_SCAN_EN);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_STBCAL1), 0, STBCAL1_DQSERRCNT_DIS);

    ucsearch_state = 0;
    for (ucdqs_dly=0; ucdqs_dly<128; ucdqs_dly++)
    {

        //Set DQS delay (RG_??_RX_DQS_EYE_DLY)
        if(u1IsLP4Family(p->dram_type))
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ4), ucdqs_dly, B0_DQ4_RG_RX_ARDQS_EYE_R_DLY_B0);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ4), ucdqs_dly, B0_DQ4_RG_RX_ARDQS_EYE_F_DLY_B0);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ4), ucdqs_dly, B1_DQ4_RG_RX_ARDQS_EYE_R_DLY_B1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ4), ucdqs_dly, B1_DQ4_RG_RX_ARDQS_EYE_F_DLY_B1);
        }
#if ENABLE_LP3_SW
        else //LPDDR3
        {
            vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ4), ucdqs_dly, B0_DQ4_RG_RX_ARDQS_EYE_R_DLY_B0);
            vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ4), ucdqs_dly, B0_DQ4_RG_RX_ARDQS_EYE_F_DLY_B0);
            vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ4), ucdqs_dly, B1_DQ4_RG_RX_ARDQS_EYE_R_DLY_B1);
            vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ4), ucdqs_dly, B1_DQ4_RG_RX_ARDQS_EYE_F_DLY_B1);
            vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_CA_CMD4), ucdqs_dly, CA_CMD4_RG_RX_ARCLK_EYE_R_DLY);
            vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_CA_CMD4), ucdqs_dly, CA_CMD4_RG_RX_ARCLK_EYE_F_DLY);
        }
#endif

        DramPhyReset(p);

        //Reset eye scan counters (reg_sw_rst): 1 to 0
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 1, EYESCAN_REG_SW_RST);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 0, EYESCAN_REG_SW_RST);

        //Enable DQ eye scan (b'1)
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 1, EYESCAN_RG_RX_EYE_SCAN_EN);

        // 2ns/sample, here we delay 1ms about 500 samples
        mcDELAY_US(1000);

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

    //restore to orignal value
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), u4prv_register_EYESCAN);
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_STBCAL1), u4prv_register_STBCAL1);

    if(u1IsLP4Family(p->dram_type))
    {
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), u4prv_register_B0_DQ6[0]);
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), u4prv_register_B1_DQ6[0]);
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B0_DQ5), u4prv_register_B0_DQ5[0]);
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B1_DQ5), u4prv_register_B1_DQ5[0]);
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B0_DQ4), u4prv_register_B0_DQ4[0]);
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B1_DQ4), u4prv_register_B1_DQ4[0]);
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B0_DQ3), u4prv_register_B0_DQ3[0]);
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B1_DQ3), u4prv_register_B1_DQ3[0]);
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ7), u4prv_register_SHU1_B0_DQ7[0]);
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DQ7), u4prv_register_SHU1_B1_DQ7[0]);
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), u4prv_register_MISC_CTRL1[0]);
    }
#if ENABLE_LP3_SW
    else //LPDDR3
    {
        U8 u1PhyIdx = 0;
        for(u1PhyIdx=0; u1PhyIdx<2; u1PhyIdx++)
        {
            vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B0_DQ6)+(u1PhyIdx<<POS_BANK_NUM), u4prv_register_B0_DQ6[u1PhyIdx]);
            vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B1_DQ6)+(u1PhyIdx<<POS_BANK_NUM), u4prv_register_B1_DQ6[u1PhyIdx]);
            vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B0_DQ5)+(u1PhyIdx<<POS_BANK_NUM), u4prv_register_B0_DQ5[u1PhyIdx]);
            vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B1_DQ5)+(u1PhyIdx<<POS_BANK_NUM), u4prv_register_B1_DQ5[u1PhyIdx]);
            vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B0_DQ4)+(u1PhyIdx<<POS_BANK_NUM), u4prv_register_B0_DQ4[u1PhyIdx]);
            vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B1_DQ4)+(u1PhyIdx<<POS_BANK_NUM), u4prv_register_B1_DQ4[u1PhyIdx]);
            vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B0_DQ3)+(u1PhyIdx<<POS_BANK_NUM), u4prv_register_B0_DQ3[u1PhyIdx]);
            vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B1_DQ3)+(u1PhyIdx<<POS_BANK_NUM), u4prv_register_B1_DQ3[u1PhyIdx]);
            vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ7)+(u1PhyIdx<<POS_BANK_NUM), u4prv_register_SHU1_B0_DQ7[u1PhyIdx]);
            vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DQ7)+(u1PhyIdx<<POS_BANK_NUM), u4prv_register_SHU1_B1_DQ7[u1PhyIdx]);
            vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_CA_CMD4)+(u1PhyIdx<<POS_BANK_NUM), u4prv_register_CA_CMD4[u1PhyIdx]);
            vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_CA_CMD6)+(u1PhyIdx<<POS_BANK_NUM), u4prv_register_CA_CMD6[u1PhyIdx]);
            vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1)+(u1PhyIdx<<POS_BANK_NUM), u4prv_register_MISC_CTRL1[u1PhyIdx]);
        }
    }
#endif

    if(ucsearch_state!=4)
    {
        mcSHOW_DBG_MSG(("\n\tMIOCK jitter meter - ch=%d\n", p->channel));
        mcSHOW_DBG_MSG(("\tLess than 0.5T data. Cannot calculate delay cell time\n\n"));

        return DRAM_FAIL;
    }

    //Calculate 1 delay cell = ? ps
    // 1T = ? delay cell
    ucg_num_dlycell_perT = (ucend_period - ucstart_period);
    // 1T = ? ps

    u1ShuLevel = u4IO32ReadFldAlign(DRAMC_REG_SHUSTATUS, SHUSTATUS_SHUFFLE_LEVEL);
    u4PLL5_ADDR = DDRPHY_SHU1_PLL5+0x500*u1ShuLevel;
    u4PLL8_ADDR = DDRPHY_SHU1_PLL8+0x500*u1ShuLevel;
    u4CA_CMD6 = DDRPHY_SHU1_CA_CMD6+0x500*u1ShuLevel;
    u4SDM_PCW = u4IO32ReadFldAlign(u4PLL5_ADDR, SHU1_PLL5_RG_RPHYPLL_SDM_PCW);
    u4PREDIV = u4IO32ReadFldAlign(u4PLL8_ADDR, SHU1_PLL8_RG_RPHYPLL_PREDIV);
    u4POSDIV = u4IO32ReadFldAlign(u4PLL8_ADDR, SHU1_PLL8_RG_RPHYPLL_POSDIV);
    u4CKDIV4 = u4IO32ReadFldAlign(u4CA_CMD6, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA);
    u4VCOFreq = ((52>>u4PREDIV)*(u4SDM_PCW>>8))>>u4POSDIV;
    u4DataRate = u4VCOFreq>>u4CKDIV4;
    u2real_freq = u4DataRate>>1;

    u2real_period = (U16) (1000000/u2real_freq);
    //calculate delay cell time

    u2gdelay_cell_ps = u2real_period*100 / ucg_num_dlycell_perT;

    mcSHOW_DBG_MSG(("\n\tMIOCK jitter meter\tch=%d\n\n"
                    "1T = (%d-%d) = %d dly cells\n"
                    "Clock freq = %d MHz, period = %d ps, 1 dly cell = %d/100 ps\n",
                        p->channel,
                        ucend_period, ucstart_period, ucg_num_dlycell_perT,
                        u2real_freq, u2real_period, u2gdelay_cell_ps));

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
#endif

#if (ENABLE_CLOCK_DUTY_SCAN || ENABLE_DQS_DUTY_SCAN)
#define DUTY_OFFSET_START -8
#define DUTY_OFFSET_END 8

#define CLOCK_PI_START 0
#define CLOCK_PI_END 63
#define CLOCK_PI_STEP 2

#define DQS_DLY_START 0
#define DQS_DLY_END 128
#define DQS_DLY_STEP 1

#define ClockDutyFailLowerBound 4500    // 45%
#define ClockDutyFailUpperBound 5500    // 55%
#define ClockDutyMiddleBound    5000    // 50%

#define ClockDutyFastK                          1   // 0: full K 1: fast K
#define ClockDutyPrintAllLog                    0
#define ClockDutyPrintAllLog_Type               0   // 0:dqs cnt 1:chart
#define ClockDutyPeriod_Use_JitterMeters_Period 0   // 0: use self's period 1: use jitter meter's period

#define DQSDutyFastK                            1   // 0: full K 1: fast K
#define DQSDutyPrintAllLog                      0
#define DQSDutyPrintAllLog_Type                 0   // 0:dqs cnt 1:chart
#define DQSDutyPeriod_Use_JitterMeters_Period   0   // 0: use self's period 1: use jitter meter's period
#if defined(SLT) || (FT_DSIM_USED==1)
#define DUTY_PI_PREDICTION 0
#else
#define DUTY_PI_PREDICTION 6
#endif
#define DUTY_INCREASE_PRECISION 100  // 1: disable
#define DUTY_TANSITION_INVALID 0x7fffffff
#define DUTY_FINETUNE_K_TIME        1 // 0: disable 1:enable

#define DQS_DUTY_SLT_CONDITION_TEST 0
#define DUTY_TEST_FOR_YH (1&&DQS_DUTY_SLT_CONDITION_TEST)  ///TODO: to be remove
#if DQS_DUTY_SLT_CONDITION_TEST
U8 u1GlobalTestCnt;
#endif

typedef enum
{
    CDS_TRASITION_STATE_BEFORE_FIRST  = 0,
    CDS_TRASITION_STATE_FOUND_FIRST ,
    CDS_TRASITION_STATE_FOUND_SECOND ,
    CDS_TRASITION_STATE_FOUND_THIRD ,
    CDS_TRASITION_STATE_END
} DUTY_SCAN_STATE_T; // DUTY SCAN STATE

typedef enum
{
    CDS_PERIOD_STATE_STILL_NOT_FOUND = 0,
    CDS_PERIOD_STATE_FOUND,
    CDS_PERIOD_STATE_FOUND_COMMON
} DUTY_SCAN_PERIOD_FIND_STATE_T;

U32 u4TrasitionInterpolation(U32 u4CurrentCnt, U32 u4LastCnt, U32 u4MiddleCnt, U32 u4CurrentDelay, U32 u4LastDelay)
{
    #if (DUTY_INCREASE_PRECISION==1)  
    return u4CurrentDelay;   
    #else
    
    S32 s4InterpolatedDelay;
    
    if(u4CurrentDelay ==DQS_DLY_START)
        return 0;
        
    s4InterpolatedDelay = ((S32)(u4MiddleCnt-u4LastCnt)*DUTY_INCREASE_PRECISION/(S32)(u4CurrentCnt-u4LastCnt)) + u4LastDelay*DUTY_INCREASE_PRECISION;

    //mcSHOW_ERR_MSG(("\n[u4TrasitionInterpolation] %d %d %d, Delay %d %d\n", s4InterpolatedDelay, (u4MiddleCnt-u4LastCnt), (u4CurrentCnt-u4LastCnt), u4CurrentDelay, u4LastDelay));

    return (U32)(s4InterpolatedDelay);
    #endif
}
#endif

#if ENABLE_CLOCK_DUTY_SCAN
#if 0
void Print_Clock_Duty_Value(DRAMC_CTX_T *p)
{
    U32 read_CLK_DLY, read_CLK_DLYB;
    
    read_CLK_DLY = u4IO32ReadFldAlign(DDRPHY_SHU1_R0_CA_CMD1, SHU1_R0_CA_CMD1_RK0_TX_ARCLK_DLY);
    read_CLK_DLYB = u4IO32ReadFldAlign(DDRPHY_SHU1_R0_CA_CMD0, SHU1_R0_CA_CMD0_RK0_TX_ARCLK_DLYB);
    if (read_CLK_DLYB == 0)
    {
        mcSHOW_DBG_MSG(("Clock Duty Scan CHA calibration is %d\n",0-read_CLK_DLY));
    }
    else
    {
        mcSHOW_DBG_MSG(("Clock Duty Scan CHA calibration is %d\n",read_CLK_DLYB));    
    }

    read_CLK_DLY = u4IO32ReadFldAlign(DDRPHY_SHU1_R0_CA_CMD1 + ((U32)CHANNEL_B<<POS_BANK_NUM), SHU1_R0_CA_CMD1_RK0_TX_ARCLK_DLY);
    read_CLK_DLYB = u4IO32ReadFldAlign(DDRPHY_SHU1_R0_CA_CMD0 + ((U32)CHANNEL_B<<POS_BANK_NUM), SHU1_R0_CA_CMD0_RK0_TX_ARCLK_DLYB);
    if (read_CLK_DLYB == 0)
    {
        mcSHOW_DBG_MSG(("Clock Duty Scan CHB calibration is %d\n",0-read_CLK_DLY));
    }
    else
    {
        mcSHOW_DBG_MSG(("Clock Duty Scan CHB calibration is %d\n",read_CLK_DLYB));    
    }    

}
#endif
#if 0
void ClockDutyScan_SetRXInputDelay_CA( unsigned int ddrphy_rx_dqs_dly, unsigned int ddrphy_rx_dq_dly )
{
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD2 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(ddrphy_rx_dq_dly,SHU1_R0_CA_CMD2_RG_RK0_RX_ARCA1_F_DLY) | P_Fld(ddrphy_rx_dq_dly,SHU1_R0_CA_CMD2_RG_RK0_RX_ARCA1_R_DLY)
                                                                 | P_Fld(ddrphy_rx_dq_dly,SHU1_R0_CA_CMD2_RG_RK0_RX_ARCA0_F_DLY) | P_Fld(ddrphy_rx_dq_dly,SHU1_R0_CA_CMD2_RG_RK0_RX_ARCA0_R_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD3 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(ddrphy_rx_dq_dly,SHU1_R0_CA_CMD3_RG_RK0_RX_ARCA3_F_DLY) | P_Fld(ddrphy_rx_dq_dly,SHU1_R0_CA_CMD3_RG_RK0_RX_ARCA3_R_DLY)
                                                                 | P_Fld(ddrphy_rx_dq_dly,SHU1_R0_CA_CMD3_RG_RK0_RX_ARCA2_F_DLY) | P_Fld(ddrphy_rx_dq_dly,SHU1_R0_CA_CMD3_RG_RK0_RX_ARCA2_R_DLY));    
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD4 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(ddrphy_rx_dq_dly,SHU1_R0_CA_CMD4_RG_RK0_RX_ARCA5_F_DLY) | P_Fld(ddrphy_rx_dq_dly,SHU1_R0_CA_CMD4_RG_RK0_RX_ARCA5_R_DLY)
                                                                 | P_Fld(ddrphy_rx_dq_dly,SHU1_R0_CA_CMD4_RG_RK0_RX_ARCA4_F_DLY) | P_Fld(ddrphy_rx_dq_dly,SHU1_R0_CA_CMD4_RG_RK0_RX_ARCA4_R_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD5 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(ddrphy_rx_dq_dly,SHU1_R0_CA_CMD5_RG_RK0_RX_ARCKE1_F_DLY) | P_Fld(ddrphy_rx_dq_dly,SHU1_R0_CA_CMD5_RG_RK0_RX_ARCKE1_R_DLY)
                                                                 | P_Fld(ddrphy_rx_dq_dly,SHU1_R0_CA_CMD5_RG_RK0_RX_ARCKE0_F_DLY) | P_Fld(ddrphy_rx_dq_dly,SHU1_R0_CA_CMD5_RG_RK0_RX_ARCKE0_R_DLY));    
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD6 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(ddrphy_rx_dq_dly,SHU1_R0_CA_CMD6_RG_RK0_RX_ARCS0_F_DLY) | P_Fld(ddrphy_rx_dq_dly,SHU1_R0_CA_CMD6_RG_RK0_RX_ARCS0_R_DLY)
                                                                 | P_Fld(ddrphy_rx_dq_dly,SHU1_R0_CA_CMD6_RG_RK0_RX_ARCKE2_F_DLY) | P_Fld(ddrphy_rx_dq_dly,SHU1_R0_CA_CMD6_RG_RK0_RX_ARCKE2_R_DLY));        
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD7 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(ddrphy_rx_dq_dly,SHU1_R0_CA_CMD7_RG_RK0_RX_ARCS2_F_DLY) | P_Fld(ddrphy_rx_dq_dly,SHU1_R0_CA_CMD7_RG_RK0_RX_ARCS2_R_DLY)
                                                                 | P_Fld(ddrphy_rx_dq_dly,SHU1_R0_CA_CMD7_RG_RK0_RX_ARCS1_F_DLY) | P_Fld(ddrphy_rx_dq_dly,SHU1_R0_CA_CMD7_RG_RK0_RX_ARCS1_R_DLY));            
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD8 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(ddrphy_rx_dqs_dly,SHU1_R0_CA_CMD8_RG_RK0_RX_ARCLK_F_DLY) | P_Fld(ddrphy_rx_dqs_dly,SHU1_R0_CA_CMD8_RG_RK0_RX_ARCLK_R_DLY));                
}
void ClockDutyScan_RxReset_seq ( void ) {
    //! for CHA
    //! assert r-data reset
    vIO32WriteFldAlign(DDRPHY_MISC_CTRL1 + ((U32)CHANNEL_B<<POS_BANK_NUM), 1, MISC_CTRL1_R_DMPHYRST);

    //! assert STBEN reset
    vIO32WriteFldMulti(DDRPHY_B0_DQ3 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0, B0_DQ3_RG_ARDQ_RESETB_B0));    
    vIO32WriteFldMulti(DDRPHY_B0_DQ9 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0, B0_DQ9_RG_RX_ARDQ_STBEN_RESETB_B0) | P_Fld(0, B0_DQ9_RG_RX_ARDQS_STBEN_RESETB_B0));            
    vIO32WriteFldMulti(DDRPHY_B1_DQ3 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0, B1_DQ3_RG_ARDQ_RESETB_B1));    
    vIO32WriteFldMulti(DDRPHY_B1_DQ9 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0, B1_DQ9_RG_RX_ARDQ_STBEN_RESETB_B1) | P_Fld(0, B1_DQ9_RG_RX_ARDQS_STBEN_RESETB_B1));            
    vIO32WriteFldMulti(DDRPHY_CA_CMD3 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0, CA_CMD3_RG_ARCMD_RESETB));    
    vIO32WriteFldMulti(DDRPHY_CA_CMD10 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0, CA_CMD10_RG_RX_ARCMD_STBEN_RESETB) | P_Fld(0, CA_CMD10_RG_RX_ARCLK_STBEN_RESETB));        

    mcDELAY_US(1) ;

    //! release r-data reset
    vIO32WriteFldAlign(DDRPHY_MISC_CTRL1 + ((U32)CHANNEL_B<<POS_BANK_NUM), 0, MISC_CTRL1_R_DMPHYRST);    

    //! release STBEN reset
    vIO32WriteFldMulti(DDRPHY_B0_DQ3 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(1, B0_DQ3_RG_ARDQ_RESETB_B0));    
    vIO32WriteFldMulti(DDRPHY_B0_DQ9 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(1, B0_DQ9_RG_RX_ARDQ_STBEN_RESETB_B0) | P_Fld(1, B0_DQ9_RG_RX_ARDQS_STBEN_RESETB_B0));            
    vIO32WriteFldMulti(DDRPHY_B1_DQ3 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(1, B1_DQ3_RG_ARDQ_RESETB_B1));    
    vIO32WriteFldMulti(DDRPHY_B1_DQ9 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(1, B1_DQ9_RG_RX_ARDQ_STBEN_RESETB_B1) | P_Fld(1, B1_DQ9_RG_RX_ARDQS_STBEN_RESETB_B1));            
    vIO32WriteFldMulti(DDRPHY_CA_CMD3 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(1, CA_CMD3_RG_ARCMD_RESETB));    
    vIO32WriteFldMulti(DDRPHY_CA_CMD10 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(1, CA_CMD10_RG_RX_ARCMD_STBEN_RESETB) | P_Fld(1, CA_CMD10_RG_RX_ARCLK_STBEN_RESETB));    
} //! end-ddrphy_RxReset_seq
#endif

void ClockDutyScan_int_lpbk_test_seq (DRAMC_CTX_T *p, U32 backup_DDRPHY_MISC_EXTLB16, U32 backup_DDRPHY_MISC_EXTLB17, U32 backup_DDRPHY_MISC_EXTLB18 )
{
    //! ==============================
    //! - TX setting
    //! ==============================
    //! [21:16]: R_EXTLB_OE_*_ON
#if DUTY_FINETUNE_K_TIME
    vIO32Write4B(DDRPHY_MISC_EXTLB16 + ((U32)CHANNEL_B<<POS_BANK_NUM), backup_DDRPHY_MISC_EXTLB16 | (0x3f<<16));
#else
    vIO32WriteFldMulti(DDRPHY_MISC_EXTLB16 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(1, MISC_EXTLB16_R_EXTLB_OE_DQB0_ON) | P_Fld(1, MISC_EXTLB16_R_EXTLB_OE_DQM0_ON) | P_Fld(1, MISC_EXTLB16_R_EXTLB_OE_DQS0_ON)
                                                              | P_Fld(1, MISC_EXTLB16_R_EXTLB_OE_DQB1_ON) | P_Fld(1, MISC_EXTLB16_R_EXTLB_OE_DQM1_ON) | P_Fld(1, MISC_EXTLB16_R_EXTLB_OE_DQS1_ON));
#endif

    //! [5]: CA_TX_MODE; [4]:DQ_TX_MODE; [3]: DQ_MODE for CA
    //! [9]: CA_RX_MODE; [8]:DQ_RX_MODE; [3]: DQ_MODE for CA
#if DUTY_FINETUNE_K_TIME
    vIO32Write4B(DDRPHY_MISC_EXTLB18 + ((U32)CHANNEL_B<<POS_BANK_NUM), backup_DDRPHY_MISC_EXTLB18 | ((0x7<<3)|(0x3<<8)));
#else   
    vIO32WriteFldMulti(DDRPHY_MISC_EXTLB18 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(1, MISC_EXTLB18_R_LPBK_DQ_MODE_FOR_CA) | P_Fld(1, MISC_EXTLB18_R_LPBK_DQ_TX_MODE) | P_Fld(1, MISC_EXTLB18_R_LPBK_CA_TX_MODE)
                                                              | P_Fld(1, MISC_EXTLB18_R_LPBK_DQ_RX_MODE) | P_Fld(1, MISC_EXTLB18_R_LPBK_CA_RX_MODE));
#endif

    //! ==============================
    //! - APHY setting
    //! ==============================
    //! CA


    //! ==============================
    //! - LPBK setting
    //! ==============================
    //! step-1 : trigger RX SWRST
#if DUTY_FINETUNE_K_TIME
    vIO32Write4B(DDRPHY_MISC_EXTLB17 + ((U32)CHANNEL_B<<POS_BANK_NUM), backup_DDRPHY_MISC_EXTLB17 | (0x1<<1));
    vIO32Write4B(DDRPHY_MISC_EXTLB17 + ((U32)CHANNEL_B<<POS_BANK_NUM), backup_DDRPHY_MISC_EXTLB17 | (0x1<<3));
#else   
    vIO32WriteFldAlign(DDRPHY_MISC_EXTLB17 + ((U32)CHANNEL_B<<POS_BANK_NUM), 1, MISC_EXTLB17_R_EXTLB_RX_SWRST);
    vIO32WriteFldAlign(DDRPHY_MISC_EXTLB17 + ((U32)CHANNEL_B<<POS_BANK_NUM), 1, MISC_EXTLB17_R_EXTLB_TX_EN_OTHERCH_SEL);    
#endif    

    //! step-2 : release RX SWRST
#if DUTY_FINETUNE_K_TIME    
    vIO32Write4B(DDRPHY_MISC_EXTLB17 + ((U32)CHANNEL_B<<POS_BANK_NUM), backup_DDRPHY_MISC_EXTLB17 & ~((0x1<<1)|(0x1<<3)));
    vIO32Write4B(DDRPHY_MISC_EXTLB17 + ((U32)CHANNEL_B<<POS_BANK_NUM), backup_DDRPHY_MISC_EXTLB17 | (0x1<<3));
#else   
    vIO32WriteFldMulti(DDRPHY_MISC_EXTLB17 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0, MISC_EXTLB17_R_EXTLB_RX_SWRST) | P_Fld(0, MISC_EXTLB17_R_EXTLB_TX_EN_OTHERCH_SEL));
    vIO32WriteFldAlign(DDRPHY_MISC_EXTLB17 + ((U32)CHANNEL_B<<POS_BANK_NUM), 1, MISC_EXTLB17_R_EXTLB_TX_EN_OTHERCH_SEL);        
#endif    


    /*TINFO="[LPBK] Trigger TX Enable ..."*/

    //! step-3 : trigger LPBK TX_EN
#if DUTY_FINETUNE_K_TIME
    vIO32Write4B(DDRPHY_MISC_EXTLB17 + ((U32)CHANNEL_B<<POS_BANK_NUM), (backup_DDRPHY_MISC_EXTLB17 & ~(0x1<<3)) | (0x1<<2));
#else       
    vIO32WriteFldMulti(DDRPHY_MISC_EXTLB17 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(1, MISC_EXTLB17_R_EXTLB_TX_EN) | P_Fld(0, MISC_EXTLB17_R_EXTLB_TX_EN_OTHERCH_SEL));    
#endif

    mcDELAY_US(1);

    //! step-4 : wait LPBK done and check result
    //lpbk_test_result = ddrphy_lpbk_chk_result () ;

    //! step-6 : assert reset
    //  *((UINT32P)(Channel_B_PHY_BASE_ADDRESS + 0x0244)) = 0x0000cc03;
    //ClockDutyScan_RxReset_seq () ;

#if DUTY_FINETUNE_K_TIME   
    vIO32Write4B(DDRPHY_MISC_EXTLB17 + ((U32)CHANNEL_B<<POS_BANK_NUM), (backup_DDRPHY_MISC_EXTLB17 & ~(0x1<<2)) | (0x1<<3));
#else       
    vIO32WriteFldMulti(DDRPHY_MISC_EXTLB17 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0, MISC_EXTLB17_R_EXTLB_TX_EN) | P_Fld(1, MISC_EXTLB17_R_EXTLB_TX_EN_OTHERCH_SEL));    
#endif
}


void ClockDutyScan_inter_loopback_preset(DRAMC_CTX_T *p)
{
    //! enable lpbk CG control
    vIO32WriteFldMulti(DDRPHY_MISC_CG_CTRL0 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(1, MISC_CG_CTRL0_W_CHG_MEM) | P_Fld(1, MISC_CG_CTRL0_CLK_MEM_SEL) | P_Fld(1, MISC_CG_CTRL0_RG_CG_RX_CMD_OFF_DISABLE));
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL3 + ((U32)CHANNEL_B<<POS_BANK_NUM), 0xffffffff, MISC_CG_CTRL3_R_LBK_CG_CTRL);            
    vIO32Write4B(DDRPHY_CA_DLL_ARPI2 + ((U32)CHANNEL_B<<POS_BANK_NUM), 0x00000000);            

    //! set RG_TX_ARDQ_PULL_DN_B0/B1 to low
    vIO32WriteFldAlign(DDRPHY_B0_DQ7 + ((U32)CHANNEL_B<<POS_BANK_NUM), 0, B0_DQ7_RG_TX_ARDQ_PULL_DN_B0);                
    vIO32WriteFldAlign(DDRPHY_B1_DQ7 + ((U32)CHANNEL_B<<POS_BANK_NUM), 0, B1_DQ7_RG_TX_ARDQ_PULL_DN_B1);                

    //! enable clock and TX/RX setting of ARCMD/BRCMD
#if 0    
    vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI3 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(1, CA_DLL_ARPI3_RG_ARPI_MCTL_EN_CA) | P_Fld(1, CA_DLL_ARPI3_RG_ARPI_FB_EN_CA) | P_Fld(1, CA_DLL_ARPI3_RG_ARPI_CS_EN) |
                                                                P_Fld(1, CA_DLL_ARPI3_RG_ARPI_CLK_EN) | P_Fld(1, CA_DLL_ARPI3_RG_ARPI_CMD_EN) | P_Fld(1, CA_DLL_ARPI3_RG_ARPI_CLKIEN_EN) );
#else
    vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI3 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(1, CA_DLL_ARPI3_RG_ARPI_MCTL_EN_CA) | P_Fld(1, CA_DLL_ARPI3_RG_ARPI_FB_EN_CA) |
                                                            P_Fld(1, CA_DLL_ARPI3_RG_ARPI_CLK_EN) | P_Fld(1, CA_DLL_ARPI3_RG_ARPI_CLKIEN_EN) );
#endif
    vIO32WriteFldMulti(DDRPHY_CA_CMD3 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(1, CA_CMD3_RG_RX_ARCMD_SMT_EN) | P_Fld(1, CA_CMD3_RG_TX_ARCMD_EN) | P_Fld(1, CA_CMD3_RG_ARCMD_RESETB) | 
                                                           P_Fld(1, CA_CMD3_RG_RX_ARCMD_STBENCMP_EN));
    vIO32WriteFldMulti(DDRPHY_CA_CMD10 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(1, CA_CMD10_RG_RX_ARCMD_STBEN_RESETB) | P_Fld(1, CA_CMD10_RG_RX_ARCLK_STBEN_RESETB) | P_Fld(1, CA_CMD10_RG_RX_ARCLK_DQSIENMODE));
    


    vIO32WriteFldMulti(DDRPHY_CA_CMD5 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x10, CA_CMD5_RG_RX_ARCMD_EYE_VREF_SEL) | P_Fld(1, CA_CMD5_RG_RX_ARCMD_VREF_EN) | P_Fld(1, CA_CMD5_RG_RX_ARCMD_VREF_EN) | 
                                                           P_Fld(1, CA_CMD5_RG_RX_ARCMD_EYE_EN) | P_Fld(1, CA_CMD5_RG_RX_ARCMD_EYE_STBEN_RESETB));

    vIO32WriteFldAlign(DDRPHY_CA_CMD6 + ((U32)CHANNEL_B<<POS_BANK_NUM), 0, CA_CMD6_RG_RX_ARCMD_JM_SEL);                


    //! [2]: XTALK_EN; [1]: SSO_EN; [0]: LFSR_EN
#ifdef LPBK_SSO_OFF
    vIO32WriteFldMulti(DDRPHY_MISC_EXTLB19 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x10, MISC_EXTLB19_R_EXTLB_LFSR_ENABLE) | P_Fld(0, MISC_EXTLB19_R_EXTLB_SSO_ENABLE) | P_Fld(1, MISC_EXTLB19_R_EXTLB_XTALK_ENABLE));
#else
    vIO32WriteFldMulti(DDRPHY_MISC_EXTLB19 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x10, MISC_EXTLB19_R_EXTLB_LFSR_ENABLE) | P_Fld(1, MISC_EXTLB19_R_EXTLB_SSO_ENABLE) | P_Fld(1, MISC_EXTLB19_R_EXTLB_XTALK_ENABLE));
#endif

    //! ==============================
    //! - DDRPHY lpbk test flow
    //! ==============================
    //! step-0 : trigger loopback prepare on
    vIO32WriteFldMulti(DDRPHY_MISC_EXTLB17 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(1, MISC_EXTLB17_R_EXTLB_TX_PRE_ON) | P_Fld(1, MISC_EXTLB17_R_EXTLB));    


    /*TINFO="[LPBK] RX input delay setting ..."*/
    //ClockDutyScan_SetRXInputDelay_CA( 0, 0 ) ;  // only LP3 init set these values
}

void DramcClockDutySetClkDelayCell(DRAMC_CTX_T *p, U8 u1ChannelIdx, U8 u1RankIdx, S8 scFinal_clk_delay_cell)
{
    U8 u1ShuffleIdx;
    U32 save_offset;

    for(u1ShuffleIdx = 0; u1ShuffleIdx<DRAM_DFS_SHUFFLE_MAX; u1ShuffleIdx++)
    {
        save_offset = (u1ChannelIdx << POS_BANK_NUM) + u1ShuffleIdx*0x500 + u1RankIdx*0x100;
        
        if (scFinal_clk_delay_cell<=0)
        {
            //if (u1RankIdx == RANK_0)
            {
                vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD1 + save_offset, P_Fld(0-scFinal_clk_delay_cell, SHU1_R0_CA_CMD1_RK0_TX_ARCLK_DLY) | P_Fld(0-scFinal_clk_delay_cell, SHU1_R0_CA_CMD1_RK0_TX_ARCLKB_DLY));                                                
                vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD0 + save_offset, P_Fld( 0, SHU1_R0_CA_CMD0_RK0_TX_ARCLK_DLYB) | P_Fld( 0, SHU1_R0_CA_CMD0_RK0_TX_ARCLKB_DLYB));                                                            
            }
            //else
            {
            //    vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD1 + save_offset, P_Fld(0-scFinal_clk_delay_cell, SHU1_R1_CA_CMD1_RK1_TX_ARCLK_DLY) | P_Fld(0-scFinal_clk_delay_cell, SHU1_R1_CA_CMD1_RK1_TX_ARCLKB_DLY));                                                
            //    vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD0 + save_offset, P_Fld( 0, SHU1_R1_CA_CMD0_RK1_TX_ARCLK_DLYB) | P_Fld( 0, SHU1_R1_CA_CMD0_RK1_TX_ARCLKB_DLYB));             
            }
        }
        else
        {
            //if (u1RankIdx == RANK_0)
            {
                vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD1 + save_offset, P_Fld(0, SHU1_R0_CA_CMD1_RK0_TX_ARCLK_DLY) | P_Fld(0, SHU1_R0_CA_CMD1_RK0_TX_ARCLKB_DLY));                                                 
                vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD0 + save_offset, P_Fld( scFinal_clk_delay_cell, SHU1_R0_CA_CMD0_RK0_TX_ARCLK_DLYB) | P_Fld( scFinal_clk_delay_cell, SHU1_R0_CA_CMD0_RK0_TX_ARCLKB_DLYB));                       
            }
            //else
            //{
            //    vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD1 + save_offset, P_Fld(0, SHU1_R1_CA_CMD1_RK1_TX_ARCLK_DLY) | P_Fld(0, SHU1_R1_CA_CMD1_RK1_TX_ARCLKB_DLY));
            //    vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD0 + save_offset, P_Fld( scFinal_clk_delay_cell, SHU1_R1_CA_CMD0_RK1_TX_ARCLK_DLYB) | P_Fld( scFinal_clk_delay_cell, SHU1_R1_CA_CMD0_RK1_TX_ARCLKB_DLYB));                                                                                 
            //}
        }    
    }
}

#if FT_DSIM_USED==0
U32 ClockDutyScan_TestEngine_Find_Max_DQS_Cnt(DRAMC_CTX_T *p)
{
    U8 ucdqs_dly;
    U32 u4sample_cnt, u4ones_cnt[DQS_NUMBER], Max_Value=0;
    S8 scinner_duty_ofst;


        for (ucdqs_dly=DQS_DLY_START; ucdqs_dly<DQS_DLY_END; ucdqs_dly+=16)
        {
            //Set DQS delay (RG_??_RX_DQS_EYE_DLY)
            #if 1
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ4), ucdqs_dly, B0_DQ4_RG_RX_ARDQS_EYE_R_DLY_B0); //fra
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ4), ucdqs_dly, B1_DQ4_RG_RX_ARDQS_EYE_R_DLY_B1); //fra   
            #else  //yulia
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ4), ucdqs_dly, B0_DQ4_RG_RX_ARDQS_EYE_R_DLY_B0);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ4), ucdqs_dly, B0_DQ4_RG_RX_ARDQS_EYE_F_DLY_B0);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ4), ucdqs_dly, B1_DQ4_RG_RX_ARDQS_EYE_R_DLY_B1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ4), ucdqs_dly, B1_DQ4_RG_RX_ARDQS_EYE_F_DLY_B1);
            #endif
            DramPhyReset(p);

            //Reset eye scan counters (reg_sw_rst): 1 to 0
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 1, EYESCAN_REG_SW_RST);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 0, EYESCAN_REG_SW_RST);
        
            //Enable DQ eye scan (b'1)
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 1, EYESCAN_RG_RX_EYE_SCAN_EN);
        
            DramcEngine2Run(p, TE_OP_READ_CHECK, p->test_pattern);
        
            // according to shih-hsiu, internal loopback no need to wait 1ms to sample 500 samples
            // 2ns/sample, here we delay 1ms about 500 samples
            //mcDELAY_US(1000);
        
            //Disable DQ eye scan (b'1), for counter latch
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 0, EYESCAN_RG_RX_EYE_SCAN_EN);
        
            //Read the counter values from registers (toggle_cnt*, dqs_err_cnt*);
    //        u4sample_cnt = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TOGGLE_CNT), TOGGLE_CNT_TOGGLE_CNT);
            u4ones_cnt[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQS0_ERR_CNT), DQS0_ERR_CNT_DQS0_ERR_CNT);
            u4ones_cnt[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQS1_ERR_CNT), DQS1_ERR_CNT_DQS1_ERR_CNT);
    //        u4ones_cnt[2] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQS2_ERR_CNT), DQS2_ERR_CNT_DQS2_ERR_CNT);
    //        u4ones_cnt[3] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQS3_ERR_CNT), DQS3_ERR_CNT_DQS3_ERR_CNT);
            if (u4ones_cnt[0] > Max_Value) Max_Value = u4ones_cnt[0];
            if (u4ones_cnt[1] > Max_Value) Max_Value = u4ones_cnt[1];        
        }

    mcSHOW_ERR_MSG(("\n[ClockDutyScan_TestEngine_Find_Max_DQS_Cnt] Max_Value = %d\n", Max_Value)); 
	/*TINFO="\n[ClockDutyScan_TestEngine_Find_Max_DQS_Cnt] Max_Value = %d\n", Max_Value */    

    return Max_Value;
}

void DramcClockDutyScan_TestEngine(DRAMC_CTX_T *p)
{
    U8 u1ByteIdx;
    U32 u4RevBX[DQS_NUMBER];
    U8 ucsearch_state, fgcurrent_value, fginitial_value, ucstart_period, ucend_period=0;
    S32 scdqs_dly;    
    U32 u4sample_cnt, u4ones_cnt[DQS_NUMBER], u4Last_Ones_Cnt[DQS_NUMBER]={0}, u4MPDIV_IN_SEL;
    U16 u2real_freq, u2real_period;
    U32 u4prv_register_B0_DQ6[CHANNEL_NUM], u4prv_register_B1_DQ6[CHANNEL_NUM];
    U32 u4prv_register_B0_DQ5[CHANNEL_NUM], u4prv_register_B1_DQ5[CHANNEL_NUM];
    U32 u4prv_register_B0_DQ4[CHANNEL_NUM], u4prv_register_B1_DQ4[CHANNEL_NUM];
    U32 u4prv_register_B0_DQ3[CHANNEL_NUM], u4prv_register_B1_DQ3[CHANNEL_NUM];
    U32 u4prv_register_CA_CMD4[CHANNEL_NUM], u4prv_register_CA_CMD6[CHANNEL_NUM], u4prv_register_CA_CMD5[CHANNEL_NUM], u4prv_register_CA_CMD3[CHANNEL_NUM];
    U32 u4prv_register_CHB_CA_CMD4[CHANNEL_NUM], u4prv_register_CHB_CA_CMD6[CHANNEL_NUM], u4prv_register_CHB_CA_CMD5[CHANNEL_NUM], u4prv_register_CHB_CA_CMD3[CHANNEL_NUM];    
    U32 u4prv_register_SHU1_B0_DQ7[CHANNEL_NUM], u4prv_register_SHU1_B1_DQ7[CHANNEL_NUM];
    U32 u4prv_register_CHB_MISC_CTRL1[CHANNEL_NUM];
    U32 u4prv_register_EYESCAN, u4prv_register_STBCAL1;
    U32 u4prv_register_DDRPHY_MISC_CTRL1;
    U32 u4prv_register_DRAMC_REG_SHU_CONF0;
    U32 u4prv_register_CHB_DDRPHY_CA_DLL_ARPI3;
    U32 u4prv_register_CHB_DDRPHY_CA_DLL_ARPI2; 

    S8 scinner_duty_ofst=0, scFinal_clk_delay_cell;
    S32 sdqsTransitionPos[4][3];
    U32 ucperiod_scan_state[4]={0,0,0,0};
    U32 ucperiod, ucduty[4];
    U32 ucperiod_common=1;
    U32 ucduty_fail_flag[2];
    U32 ucperiod_duty; 
    BOOL ucfirst_is_low[4];
    U32 ucperiod_find_flag[4]={0,0,0,0};
    U32 ucperiod_duty_max=0, ucperiod_duty_min=100000, ucperiod_duty_max_clk_dly, ucperiod_duty_min_clk_dly;
    U32 ucperiod_duty_averige_temp[2]={0,0};
    U32 ucperiod_duty_averige=0, ucFinal_period_duty_averige=0, ucmost_approach_50_percent=100000;
    U32 ucFinal_duty_swap[2];
    U8 early_break_count=0;
    U8 ucdqs_i, ucDQSSwap;
    U32 HighLowSampleCount_TestEngine;
    BOOL bAbnormalDQSCnt = 0; 
#if FT_DSIM_USED   
    U8 ucg_num_dlycell_perT = 90;    
#endif        

    mcSHOW_ERR_MSG(("\n[DramcClockDutyScan_TestEngine]\n"));        
    /*TINFO="\n[DramcClockDutyScan_TestEngine]\n" */
    #if DQS_DUTY_MEASURE_WITH_WRITE_LEVELING
    DramcClockDutySetClkDelayCell(p, p->channel, RANK_0, -2);
    DramcClockDutySetClkDelayCell(p, p->channel, RANK_1, -2);
    return;
    #endif
    
#if VENDER_JV_LOG
    vPrintCalibrationBasicInfo_ForJV(p);
#else
    vPrintCalibrationBasicInfo(p);
#endif

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    if(p->pSavetimeData->femmc_Ready==1)
    {
#if CLK_DUTY_SCAN_APPLY_RK0_TO_RK1
        DramcClockDutySetClkDelayCell(p, p->channel, RANK_0, p->pSavetimeData->s1ClockDuty_clk_delay_cell[p->channel][RANK_0]);
        DramcClockDutySetClkDelayCell(p, p->channel, RANK_1, p->pSavetimeData->s1ClockDuty_clk_delay_cell[p->channel][RANK_1]);
#else
        DramcClockDutySetClkDelayCell(p, p->channel, p->rank, p->pSavetimeData->s1ClockDuty_clk_delay_cell[p->channel][p->rank]);
#endif        
        return;
    }
#endif

    //backup register value
#if FT_DSIM_USED==0 
    U32 u4RegBackupAddress[] = 
    {       
        (DRAMC_REG_ADDR(DRAMC_REG_EYESCAN)),
        (DRAMC_REG_ADDR(DRAMC_REG_STBCAL1)),

        (DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ7)),
        (DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DQ7)),

        (DRAMC_REG_ADDR(DDRPHY_CA_CMD6)),
        (DRAMC_REG_ADDR(DDRPHY_CA_CMD5)),
        (DRAMC_REG_ADDR(DDRPHY_CA_CMD4)),
        (DRAMC_REG_ADDR(DDRPHY_CA_CMD3)),

        (DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1)),
        (DRAMC_REG_ADDR(DRAMC_REG_SHU_CONF0)),

        (DRAMC_REG_ADDR(DDRPHY_B0_DQ6)),
        (DRAMC_REG_ADDR(DDRPHY_B1_DQ6)),
        (DRAMC_REG_ADDR(DDRPHY_B0_DQ5)),
        (DRAMC_REG_ADDR(DDRPHY_B1_DQ5)),
        (DRAMC_REG_ADDR(DDRPHY_B0_DQ3)),
        (DRAMC_REG_ADDR(DDRPHY_B1_DQ3)),
        (DRAMC_REG_ADDR(DDRPHY_B0_DQ4)),
        (DRAMC_REG_ADDR(DDRPHY_B1_DQ4)),
    };
#endif    

    //backup register value
#if FT_DSIM_USED==0
    DramcBackupRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));
#endif    

    /* Justin: Disable(set to 0) "RX DQS ISI pulse CG function" in DQSSTB to prevent DQSI from being kept high after READ burst in SWAP1 mode
     * (must enable(set to 1) before leaving duty calibration)
     */
    RxDQSIsiPulseCG(p, DISABLE);

    //MCK4X CG
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 0, MISC_CTRL1_R_DMDQSIENCG_EN);   //fra

    // Bypass DQS glitch-free mode
    // RG_RX_*RDQ_EYE_DLY_DQS_BYPASS_B**
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), 1, B0_DQ6_RG_RX_ARDQ_EYE_DLY_DQS_BYPASS_B0); //fra
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), 1, B1_DQ6_RG_RX_ARDQ_EYE_DLY_DQS_BYPASS_B1); //fra

    //Enable DQ eye scan
    //RG_??_RX_EYE_SCAN_EN
    //RG_??_RX_VREF_EN
    //RG_??_RX_SMT_EN
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 1, EYESCAN_RG_RX_EYE_SCAN_EN);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), P_Fld(0x1, EYESCAN_EYESCAN_DQS_SYNC_EN)    
                                        | P_Fld(0x1, EYESCAN_EYESCAN_NEW_DQ_SYNC_EN)    
                                        | P_Fld(0x1, EYESCAN_EYESCAN_DQ_SYNC_EN)); 

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ5), 1, B0_DQ5_RG_RX_ARDQ_EYE_EN_B0); //fra
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ5), 1, B1_DQ5_RG_RX_ARDQ_EYE_EN_B1); //fra
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ5), 1, B0_DQ5_RG_RX_ARDQ_VREF_EN_B0); //fra
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ5), 1, B1_DQ5_RG_RX_ARDQ_VREF_EN_B1); //fra
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ3), 1, B0_DQ3_RG_RX_ARDQ_SMT_EN_B0); //fra 
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ3), 1, B1_DQ3_RG_RX_ARDQ_SMT_EN_B1); //fra    
    

    //JM_SEL
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), 0, B0_DQ6_RG_RX_ARDQ_JM_SEL_B0); //fra
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), 0, B1_DQ6_RG_RX_ARDQ_JM_SEL_B1); //fra
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_CMD6), 0, CA_CMD6_RG_RX_ARCMD_JM_SEL); //fra


    //Enable MIOCK jitter meter mode ( RG_RX_MIOCK_JIT_EN=1)
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 1, EYESCAN_RG_RX_MIOCK_JIT_EN);

    //Disable DQ eye scan (b'1), for counter clear
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 0, EYESCAN_RG_RX_EYE_SCAN_EN);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_STBCAL1), 0, STBCAL1_DQSERRCNT_DIS);

    DramcEngine2Init(p, p->test2_1, p->test2_2, p->test_pattern|0x80, 1); //Make sure no gap between READ bursts

    HighLowSampleCount_TestEngine = ClockDutyScan_TestEngine_Find_Max_DQS_Cnt(p)>>1;

    scFinal_clk_delay_cell = 0;
#if FT_DSIM_USED
#ifdef SET_CLK_DLY
    for(scinner_duty_ofst=-8; scinner_duty_ofst<=-8; scinner_duty_ofst++) //CK PI code increasing scan
#else
    for(scinner_duty_ofst=0; scinner_duty_ofst<=0; scinner_duty_ofst++) //CK PI code increasing scan
#endif
#else
    for(scinner_duty_ofst=-8; scinner_duty_ofst<=8; scinner_duty_ofst++)
#endif    
    {
        if (scinner_duty_ofst<=0)
        {
            vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD1, P_Fld(0-scinner_duty_ofst, SHU1_R0_CA_CMD1_RK0_TX_ARCLK_DLY) | P_Fld(0-scinner_duty_ofst, SHU1_R0_CA_CMD1_RK0_TX_ARCLKB_DLY));                                                  
        }
        else
        {
            vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD0, P_Fld(scinner_duty_ofst, SHU1_R0_CA_CMD0_RK0_TX_ARCLK_DLYB) | P_Fld(scinner_duty_ofst, SHU1_R0_CA_CMD0_RK0_TX_ARCLKB_DLYB));                                                   
        }
    
        DramcModeRegInit_LP4(p);

#if (!__ETT__ && !FOR_DV_SIMULATION_USED && !FT_DSIM_USED)
        mtk_wdt_restart();
#endif

        bAbnormalDQSCnt = FALSE; 
        for(ucDQSSwap=0; ucDQSSwap<2; ucDQSSwap++)  //swap begin
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ3), ucDQSSwap, B0_DQ3_RG_RX_ARDQS0_SWAP_EN_B0); 
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ3), ucDQSSwap, B1_DQ3_RG_RX_ARDQS0_SWAP_EN_B1);

            //ucperiod_duty_max=0;
            //ucperiod_duty_min=100000;
            ucduty_fail_flag[ucDQSSwap] = 1;
        
            for(ucdqs_i=0; ucdqs_i<=1; ucdqs_i++)
            {
                sdqsTransitionPos[ucdqs_i][0] = sdqsTransitionPos[ucdqs_i][1] = sdqsTransitionPos[ucdqs_i][2] = 0;

                ucperiod_scan_state[ucdqs_i]=CDS_TRASITION_STATE_BEFORE_FIRST ;  

#if ClockDutyPeriod_Use_JitterMeters_Period
                ucperiod_find_flag[ucdqs_i] = CDS_PERIOD_STATE_FOUND_COMMON;
                ucperiod_common = p->ucnum_dlycell_perT*DUTY_INCREASE_PRECISION; //period from jitter meter
#else
                ucperiod_find_flag[ucdqs_i] = CDS_PERIOD_STATE_STILL_NOT_FOUND;      
#endif                 
            }
            
            //ucperiod_find_flag = 0;
            for (scdqs_dly=DQS_DLY_START; scdqs_dly<DQS_DLY_END; scdqs_dly++)
            {
                //Set DQS delay (RG_??_RX_DQS_EYE_DLY)
                #if 1
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ4), scdqs_dly, B0_DQ4_RG_RX_ARDQS_EYE_R_DLY_B0); //fra
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ4), scdqs_dly, B1_DQ4_RG_RX_ARDQS_EYE_R_DLY_B1); //fra  
                #else  //yulia 
                 vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ4), scdqs_dly, B0_DQ4_RG_RX_ARDQS_EYE_R_DLY_B0);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ4), scdqs_dly, B0_DQ4_RG_RX_ARDQS_EYE_F_DLY_B0);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ4), scdqs_dly, B1_DQ4_RG_RX_ARDQS_EYE_R_DLY_B1);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ4), scdqs_dly, B1_DQ4_RG_RX_ARDQS_EYE_F_DLY_B1);
                #endif
                DramPhyReset(p);
    
                //Reset eye scan counters (reg_sw_rst): 1 to 0
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 1, EYESCAN_REG_SW_RST);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 0, EYESCAN_REG_SW_RST);
    
                //Enable DQ eye scan (b'1)
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 1, EYESCAN_RG_RX_EYE_SCAN_EN);
    
                DramcEngine2Run(p, TE_OP_READ_CHECK, p->test_pattern);                
                
                // according to shih-hsiu, internal loopback no need to wait 1ms to sample 500 samples
                // 2ns/sample, here we delay 1ms about 500 samples
                //mcDELAY_US(1000);
    
                //Disable DQ eye scan (b'1), for counter latch
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 0, EYESCAN_RG_RX_EYE_SCAN_EN);
    
                //Read the counter values from registers (toggle_cnt*, dqs_err_cnt*);
                u4ones_cnt[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQS0_ERR_CNT), DQS0_ERR_CNT_DQS0_ERR_CNT);
                u4ones_cnt[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQS1_ERR_CNT), DQS1_ERR_CNT_DQS1_ERR_CNT);

                if(u4ones_cnt[0]>(HighLowSampleCount_TestEngine<<2) || u4ones_cnt[1]>(HighLowSampleCount_TestEngine<<2))
                {
                    bAbnormalDQSCnt = TRUE;
                }
#if ClockDutyPrintAllLog                
            #ifdef ETT_PRINT_FORMAT
                mcSHOW_DBG_MSG(("%d : %d %d\n", scdqs_dly, u4ones_cnt[0],u4ones_cnt[1]));
                /*TINFO="%d : %d %d\n", scdqs_dly, u4ones_cnt[0],u4ones_cnt[1] */
            #else
                mcSHOW_DBG_MSG(("%3d : %8d, %8d\n", scdqs_dly, u4ones_cnt[0],u4ones_cnt[1]));
            #endif
            #endif

                //for(ucdqs_i=2; ucdqs_i<=2; ucdqs_i++)
                for(ucdqs_i=0; ucdqs_i<=1; ucdqs_i++)
                {
                    if (scdqs_dly == 0)
                    {
                        if (u4ones_cnt[ucdqs_i] <= HighLowSampleCount_TestEngine) ucfirst_is_low[ucdqs_i] = TRUE;
                        else ucfirst_is_low[ucdqs_i] = FALSE;
                    }

                        switch(ucperiod_scan_state[ucdqs_i])
                        {
                            case CDS_TRASITION_STATE_BEFORE_FIRST :
                                if (((ucfirst_is_low[ucdqs_i]==TRUE) && (u4ones_cnt[ucdqs_i] >= HighLowSampleCount_TestEngine)) ||
                                    ((ucfirst_is_low[ucdqs_i]==FALSE) && (u4ones_cnt[ucdqs_i] < HighLowSampleCount_TestEngine)))
                                {
                                    //ucperiod_first = scdqs_dly;
                                    sdqsTransitionPos[ucdqs_i][0] = u4TrasitionInterpolation(u4ones_cnt[ucdqs_i], u4Last_Ones_Cnt[ucdqs_i],HighLowSampleCount_TestEngine, scdqs_dly, scdqs_dly-DQS_DLY_STEP);;
                                    ucperiod_scan_state[ucdqs_i] = CDS_TRASITION_STATE_FOUND_FIRST ;
                                }
                            break;
                            case CDS_TRASITION_STATE_FOUND_FIRST :
                                if (((ucfirst_is_low[ucdqs_i]==TRUE) && (u4ones_cnt[ucdqs_i] < HighLowSampleCount_TestEngine)) ||
                                    ((ucfirst_is_low[ucdqs_i]==FALSE) && (u4ones_cnt[ucdqs_i] >= HighLowSampleCount_TestEngine)))
                                {
                                    //ucperiod_low_first = scdqs_dly;
                                    sdqsTransitionPos[ucdqs_i][1] = u4TrasitionInterpolation(u4ones_cnt[ucdqs_i], u4Last_Ones_Cnt[ucdqs_i],HighLowSampleCount_TestEngine, scdqs_dly, scdqs_dly-DQS_DLY_STEP);;
                                    ucperiod_scan_state[ucdqs_i] = CDS_TRASITION_STATE_FOUND_SECOND ;
                                }
                            break;
                            case CDS_TRASITION_STATE_FOUND_SECOND :
                                if (((ucfirst_is_low[ucdqs_i]==TRUE) && (u4ones_cnt[ucdqs_i] >= HighLowSampleCount_TestEngine)) ||
                                    ((ucfirst_is_low[ucdqs_i]==FALSE) && (u4ones_cnt[ucdqs_i] < HighLowSampleCount_TestEngine)))
                                {
                                    //ucperiod_last = scdqs_dly-1;
                                    sdqsTransitionPos[ucdqs_i][2] = u4TrasitionInterpolation(u4ones_cnt[ucdqs_i], u4Last_Ones_Cnt[ucdqs_i],HighLowSampleCount_TestEngine, scdqs_dly, scdqs_dly-DQS_DLY_STEP);;
                                    ucperiod_scan_state[ucdqs_i] = CDS_TRASITION_STATE_FOUND_THIRD ;
                                    if (ucperiod_find_flag[ucdqs_i] == CDS_PERIOD_STATE_STILL_NOT_FOUND) ucperiod_find_flag[ucdqs_i] = CDS_PERIOD_STATE_FOUND;
                                }                
                            break;
                        }
                        u4Last_Ones_Cnt[ucdqs_i] = u4ones_cnt[ucdqs_i];
                }
            }


            if (ucperiod_find_flag[0] == CDS_PERIOD_STATE_FOUND || ucperiod_find_flag[1] == CDS_PERIOD_STATE_FOUND)
            {
                if (ucperiod_find_flag[0] == CDS_PERIOD_STATE_FOUND)
                {
                    #if 0
                    if (ucfirst_is_low[0]==TRUE)
                    {
                        ucperiod_common = ucperiod_third_low_2_high[0] - ucperiod_first_low_2_high[0];
                    }
                    else
                    {
                        ucperiod_common = ucperiod_third_high_2_low[0] - ucperiod_first_high_2_low[0];
                    }
                    #else
                    ucperiod_common = sdqsTransitionPos[0][2] - sdqsTransitionPos[0][0];
                    #endif
                }
                else
                {
                    #if 0
                    if (ucfirst_is_low[1]==TRUE)
                    {
                        ucperiod_common = ucperiod_third_low_2_high[1] - ucperiod_first_low_2_high[1];
                    }
                    else
                    {
                        ucperiod_common = ucperiod_third_high_2_low[1] - ucperiod_first_high_2_low[1];
                    }                    
                    #else
                    ucperiod_common = sdqsTransitionPos[1][2] - sdqsTransitionPos[1][0];
                    #endif                
                }

                ucperiod_find_flag[0] = ucperiod_find_flag[1] = CDS_PERIOD_STATE_FOUND_COMMON;

            }

            //for(ucdqs_i=2; ucdqs_i<=2; ucdqs_i++)
            for(ucdqs_i=0; ucdqs_i<=1; ucdqs_i++)
            {                
                if (ucperiod_find_flag[ucdqs_i] == CDS_PERIOD_STATE_FOUND_COMMON)
                {
                    if (ucperiod_scan_state[ucdqs_i] == CDS_TRASITION_STATE_FOUND_THIRD )
                    {
                        #if 0
                        if (ucfirst_is_low[ucdqs_i]==TRUE)
                        {
                            ucperiod = ucperiod_third_low_2_high[ucdqs_i] - ucperiod_first_low_2_high[ucdqs_i];
                        }
                        else
                        {
                            ucperiod = ucperiod_third_high_2_low[ucdqs_i] - ucperiod_first_high_2_low[ucdqs_i];
                        }            
                        #else
                        ucperiod = sdqsTransitionPos[ucdqs_i][2] - sdqsTransitionPos[ucdqs_i][0];
                        #endif
                        ucperiod_common = ucperiod;
                    }
                    else
                    {
                        ucperiod = ucperiod_common;
                    }
                }
                else
                {
                    mcSHOW_ERR_MSG(("Can't find period!! Use Jitter Meter's period!!\n"));
                    ucperiod = p->ucnum_dlycell_perT*DUTY_INCREASE_PRECISION;
                    ucperiod_common = ucperiod;
                    ucperiod_find_flag[ucdqs_i] = CDS_PERIOD_STATE_FOUND_COMMON;
                }

                if (ucperiod_find_flag[ucdqs_i] == CDS_PERIOD_STATE_FOUND_COMMON)
                {        
                    #if 0
                    if (ucfirst_is_low[ucdqs_i]==TRUE)
                    {
                        ucperiod_duty = ucperiod_second_high_2_low[ucdqs_i] - ucperiod_first_low_2_high[ucdqs_i];
                        ucduty[ucdqs_i] = (ucperiod_duty*10000)/ucperiod;                
                    }
                    else
                    {
                        ucperiod_duty = ucperiod_second_low_2_high[ucdqs_i] - ucperiod_first_high_2_low[ucdqs_i];
                        ucduty[ucdqs_i] = 10000 - (ucperiod_duty*10000)/ucperiod;                                
                    }
                    #else
                    ucperiod_duty = sdqsTransitionPos[ucdqs_i][1] - sdqsTransitionPos[ucdqs_i][0];
                    if (ucfirst_is_low[ucdqs_i]==TRUE)
                    {
                         ucduty[ucdqs_i] = (ucperiod_duty*10000)/ucperiod;
                    }
                    else
                    {
                         ucduty[ucdqs_i] = 10000 - (ucperiod_duty*10000)/ucperiod;                                
                    }
                    #endif
            
#if ClockDutyPrintAllLog            
                    mcSHOW_DBG_MSG(("DQS%d, [Inner Clk Delay Cell = %d]\n",ucdqs_i, scinner_duty_ofst));
                    /*TINFO="DQS%d, [Inner Clk Delay Cell = %d]\n",ucdqs_i, scinner_duty_ofst */
                    if (ucfirst_is_low[ucdqs_i]==TRUE)
                    {
                        mcSHOW_DBG_MSG(("Francis ucperiod=%d(%d ~ %d)\n",ucperiod, sdqsTransitionPos[ucdqs_i][0], sdqsTransitionPos[ucdqs_i][2]));
                        /*TINFO="Francis ucperiod=%d(%d ~ %d)\n",ucperiod, sdqsTransitionPos[ucdqs_i][0], sdqsTransitionPos[ucdqs_i][2] */
                        mcSHOW_DBG_MSG(("Francis duty_high=%d(%d ~ %d)\n",ucperiod_duty, sdqsTransitionPos[ucdqs_i][0], sdqsTransitionPos[ucdqs_i][1]));
                        /*TINFO="Francis duty_high=%d(%d ~ %d)\n",ucperiod_duty, sdqsTransitionPos[ucdqs_i][0], sdqsTransitionPos[ucdqs_i][1] */
                    }
                    else
                    {
                        mcSHOW_DBG_MSG(("Francis ucperiod=%d(%d ~ %d)\n",ucperiod, sdqsTransitionPos[ucdqs_i][0], sdqsTransitionPos[ucdqs_i][2]));
                        /*TINFO="Francis ucperiod=%d(%d ~ %d)\n",ucperiod, sdqsTransitionPos[ucdqs_i][0], sdqsTransitionPos[ucdqs_i][2] */
                        mcSHOW_DBG_MSG(("Francis duty_low=%d(%d ~ %d)\n",ucperiod_duty, sdqsTransitionPos[ucdqs_i][1], sdqsTransitionPos[ucdqs_i][2]));
                        /*TINFO="Francis duty_low=%d(%d ~ %d)\n",ucperiod_duty, sdqsTransitionPos[ucdqs_i][1], sdqsTransitionPos[ucdqs_i][2] */
                    }
                    mcSHOW_DBG_MSG(("Francis clock Duty = %d%%(X100)\n",ucduty[ucdqs_i]));
                    /*TINFO="Francis clock Duty = %d%%(X100)\n",ucduty[ucdqs_i] */
#endif                    
                }
            }

            if(bAbnormalDQSCnt==FALSE && (ucduty[0]>0 && ucduty[0] <10000) && (ucduty[1]>0 && ucduty[1]<10000))//DQS0 and 1 duty valid
            {
                ucduty_fail_flag[ucDQSSwap]= 0;

                if (ucduty[0] < ucduty[1])
                {
                    ucperiod_duty_min = ucduty[0];
                    ucperiod_duty_max = ucduty[1];

                    //for SLT, use ERR_MSG to force print log
                    mcSHOW_ERR_MSG(("[%d] DQS1 Duty = %d%%(X100)\n",scinner_duty_ofst, ucperiod_duty_max));
                    /*TINFO="[%d] DQS1 Duty = %d%%(X100)\n",scinner_duty_ofst, ucperiod_duty_max */
                    mcSHOW_ERR_MSG(("[%d] DQS0 Duty = %d%%(X100)\n",scinner_duty_ofst, ucperiod_duty_min));                
                    /*TINFO="[%d] DQS0 Duty = %d%%(X100)\n",scinner_duty_ofst, ucperiod_duty_min */
                }
                else
                {
                    ucperiod_duty_min = ucduty[1];
                    ucperiod_duty_max = ucduty[0];        

                    //for SLT, use ERR_MSG to force print log
                    mcSHOW_ERR_MSG(("[%d] DQS0 Duty = %d%%(X100)\n",scinner_duty_ofst, ucperiod_duty_max));
                    /*TINFO="[%d] DQS0 Duty = %d%%(X100)\n",scinner_duty_ofst, ucperiod_duty_max */
                    mcSHOW_ERR_MSG(("[%d] DQS1 Duty = %d%%(X100)\n",scinner_duty_ofst, ucperiod_duty_min));                
                    /*TINFO="[%d] DQS1 Duty = %d%%(X100)\n",scinner_duty_ofst, ucperiod_duty_min */
                }

                ucperiod_duty_averige_temp[ucDQSSwap]= (ucperiod_duty_max + ucperiod_duty_min)/2;
                mcSHOW_DBG_MSG(("[%d] AVG Duty(Swap%d) = %d%%(X100)\n", scinner_duty_ofst, ucDQSSwap, ucperiod_duty_averige_temp[ucDQSSwap]));
                /*TINFO="[%d] AVG Duty(Swap%d) = %d%%(X100)\n", scinner_duty_ofst, ucDQSSwap, ucperiod_duty_averige_temp[ucDQSSwap] */
            }
        }

        if(ucduty_fail_flag[0]==0 && ucduty_fail_flag[1]==0)//duty valid
        {
            ucperiod_duty_averige = (ucperiod_duty_averige_temp[0] +(10000-ucperiod_duty_averige_temp[1]))>>1;
        }
        else
            ucperiod_duty_averige=0;
        
        mcSHOW_DBG_MSG(("[%d] AVG Duty = %d%%(X100)\n", scinner_duty_ofst, ucperiod_duty_averige));
        /*TINFO="[%d] AVG Duty = %d%%(X100)\n", scinner_duty_ofst, ucperiod_duty_averige */
        
        if (ucperiod_duty_averige >= ClockDutyMiddleBound)
        {
            if ((ucperiod_duty_averige-ClockDutyMiddleBound) <= ucmost_approach_50_percent)
            {
                ucmost_approach_50_percent = ucperiod_duty_averige-ClockDutyMiddleBound;
                scFinal_clk_delay_cell = scinner_duty_ofst;
                ucFinal_period_duty_averige = ucperiod_duty_averige;
                ucFinal_duty_swap[0] = ucperiod_duty_averige_temp[0];
                ucFinal_duty_swap[1] = (10000-ucperiod_duty_averige_temp[1]);
                early_break_count = 0;

                //for SLT, use ERR_MSG to force print log
                mcSHOW_ERR_MSG(("!!! ucmost_approach_50_percent = %d%%(X100) !!!\n",ucmost_approach_50_percent));
                /*TINFO="!!! ucmost_approach_50_percent = %d%%(X100) !!!\n",ucmost_approach_50_percent */
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
                scFinal_clk_delay_cell = scinner_duty_ofst;
                ucFinal_period_duty_averige = ucperiod_duty_averige;
                ucFinal_duty_swap[0] = ucperiod_duty_averige_temp[0];
                ucFinal_duty_swap[1] = (10000-ucperiod_duty_averige_temp[1]);
                early_break_count = 0;

                //for SLT, use ERR_MSG to force print log
                mcSHOW_ERR_MSG(("!!! ucmost_approach_50_percent = %d%%(X100) !!!\n",ucmost_approach_50_percent));                    
                /*TINFO="!!! ucmost_approach_50_percent = %d%%(X100) !!!\n",ucmost_approach_50_percent */
            }            
            else
            {
                early_break_count ++;
                if (early_break_count>=2) break; //early break;
            }                
        }            
    }

    DramcEngine2End(p);

    //for SLT, use ERR_MSG to force print log
    mcSHOW_ERR_MSG(("\n[%d] SWAP0 Duty = %d%%(X100)\n",scFinal_clk_delay_cell, ucFinal_duty_swap[0]));
    /*TINFO="\n[%d] SWAP0 Duty = %d%%(X100)\n",scFinal_clk_delay_cell, ucFinal_duty_swap[0] */
    mcSHOW_ERR_MSG(("[%d] SWAP1 Duty = %d%%(X100)\n",scFinal_clk_delay_cell, ucFinal_duty_swap[1]));
    /*TINFO="[%d] SWAP1 Duty = %d%%(X100)\n",scFinal_clk_delay_cell, ucFinal_duty_swap[1] */
    mcSHOW_ERR_MSG(("[%d] Fianl Duty = %d%%(X100)\n", scFinal_clk_delay_cell, ucFinal_period_duty_averige));
    /*TINFO="[%d] Fianl Duty = %d%%(X100)\n", scFinal_clk_delay_cell, ucFinal_period_duty_averige */
    mcSHOW_ERR_MSG(("Find Final clock duty delay cell = %d\n", scFinal_clk_delay_cell));  
    /*TINFO="Find Final clock duty delay cell = %d\n", scFinal_clk_delay_cell */
    mcSHOW_ERR_MSG(("Final clock duty = %d%%(X100), ",ucFinal_period_duty_averige)); 
    /*TINFO="Final clock duty = %d%%(X100), ",ucFinal_period_duty_averige */
    
#if FT_DSIM_USED
		FT_ClockDuty_TestEngine_Compare_PassFail(ucFinal_period_duty_averige);
#else    
    if (ucFinal_period_duty_averige >= 4700 && ucFinal_period_duty_averige <= 5300)
    {
        //for SLT, use ERR_MSG to force print log
        mcSHOW_ERR_MSG(("\n CHA CLK Duty spec in!!\n"));            
        /*TINFO="\n CHA CLK Duty spec in!!\n" */
    }
    else
    {
        //for SLT, use ERR_MSG to force print log
        mcSHOW_ERR_MSG(("\n CHA CLK Duty spec out!!\n Average duty out of 47%%~53%%\n"));
        /*TINFO="\n CHA CLK Duty spec out!!\n Average duty out of 47%%~53%%\n" */
        
#if __ETT__ || defined(SLT)
        while(1); //stop here
#endif
    }    
#endif    

#if CLK_DUTY_SCAN_APPLY_RK0_TO_RK1
    // I am afraid switching rank will get glitch if rank0/1's DLY setting not the same because CLK doesn't stop when rank switching.
    DramcClockDutySetClkDelayCell(p, p->channel, RANK_0, scFinal_clk_delay_cell);
    DramcClockDutySetClkDelayCell(p, p->channel, RANK_1, scFinal_clk_delay_cell);
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    if(p->pSavetimeData->femmc_Ready==0)
    {
        p->pSavetimeData->s1ClockDuty_clk_delay_cell[p->channel][RANK_0] = scFinal_clk_delay_cell;
        p->pSavetimeData->s1ClockDuty_clk_delay_cell[p->channel][RANK_1] = scFinal_clk_delay_cell;        
    }
#endif 
#else
    DramcClockDutySetClkDelayCell(p, p->channel, p->rank, scFinal_clk_delay_cell);    
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    if(p->pSavetimeData->femmc_Ready==0)
    {
        p->pSavetimeData->s1ClockDuty_clk_delay_cell[p->channel][p->rank] = scFinal_clk_delay_cell;
    }          
#endif           
#endif    

    // Justin: Enable(set to 1) "RX DQS ISI pulse CG function" before leaving duty calibration
    RxDQSIsiPulseCG(p, ENABLE);

#if FT_DSIM_USED==0
    //restore to orignal value
    //backup register value
    DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));
#endif    

    DramcModeRegInit_LP4(p);// After setting CLK duty, reset dram again to prevent Dram no response.(for SamsungB515)
    DramPhyReset(p);                    

    mcSHOW_ERR_MSG(("[DramcClockDutyScan_TestEigine] ====Done====\n"));        
    /*TINFO="[DramcClockDutyScan_TestEigine] ====Done====\n" */
}
#endif

U32 ClockDutyScan_InternalLoopBack_Find_Max_DQS_Cnt(DRAMC_CTX_T *p, U32 backup_DDRPHY_CA_CMD4, U32 backup_DRAMC_REG_EYESCAN, U32 backup_DDRPHY_MISC_EXTLB16, U32 backup_DDRPHY_MISC_EXTLB17, U32 backup_DDRPHY_MISC_EXTLB18)
{
    S32 scdqs_dly;
    U32 u4sample_cnt, u4ones_cnt2, Max_Value=0;

    for (scdqs_dly=DQS_DLY_START; scdqs_dly<DQS_DLY_END; scdqs_dly+=16)
    {
        //Set DQS delay (RG_??_RX_DQS_EYE_DLY)
#if DUTY_FINETUNE_K_TIME
            backup_DDRPHY_CA_CMD4 &= 0xffff8080;
            backup_DDRPHY_CA_CMD4 |= ((scdqs_dly<<8) | (scdqs_dly));
            vIO32Write4B(DDRPHY_CA_CMD4 + (((U32)CHANNEL_B<<POS_BANK_NUM)), backup_DDRPHY_CA_CMD4); 
#else
        vIO32WriteFldMulti(DDRPHY_CA_CMD4 + (((U32)CHANNEL_B<<POS_BANK_NUM)), P_Fld(scdqs_dly, CA_CMD4_RG_RX_ARCLK_EYE_R_DLY) | P_Fld(scdqs_dly, CA_CMD4_RG_RX_ARCLK_EYE_F_DLY));
#endif
    
        //DramPhyReset(p);
    
        //Reset eye scan counters (reg_sw_rst): 1 to 0
#if DUTY_FINETUNE_K_TIME
            vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), backup_DRAMC_REG_EYESCAN | 0x1);
            vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), backup_DRAMC_REG_EYESCAN & ~(0x1));
#else            
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 1, EYESCAN_REG_SW_RST);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 0, EYESCAN_REG_SW_RST);
#endif            
    
        //Enable DQ eye scan (b'1)
#if DUTY_FINETUNE_K_TIME
            vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), backup_DRAMC_REG_EYESCAN | (1<<1));
#else     
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 1, EYESCAN_RG_RX_EYE_SCAN_EN);
#endif
    
        //inter loop back
            ClockDutyScan_int_lpbk_test_seq(p, backup_DDRPHY_MISC_EXTLB16, backup_DDRPHY_MISC_EXTLB17, backup_DDRPHY_MISC_EXTLB18);
    
        // according to shih-hsiu, internal loopback no need to wait 1ms to sample 500 samples
        // 2ns/sample, here we delay 1ms about 500 samples
        //mcDELAY_US(1000);
    
        //Disable DQ eye scan (b'1), for counter latch
#if DUTY_FINETUNE_K_TIME
            vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), backup_DRAMC_REG_EYESCAN & ~(1<<1));
#else  
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 0, EYESCAN_RG_RX_EYE_SCAN_EN);
#endif
    
        //Read the counter values from registers (toggle_cnt*, dqs_err_cnt*);
        u4ones_cnt2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQS2_ERR_CNT), DQS2_ERR_CNT_DQS2_ERR_CNT);
            if (u4ones_cnt2 > Max_Value) Max_Value = u4ones_cnt2;
        }


    mcSHOW_ERR_MSG(("\n[ClockDutyScan_InternalLoopBack_Find_Max_DQS_Cnt] Max_Value = %d\n", Max_Value)); 
    /*TINFO="\n[ClockDutyScan_InternalLoopBack_Find_Max_DQS_Cnt] Max_Value = %d\n", Max_Value */

    return Max_Value;
}

S8 gbackup_scFinal_clk_delay_cell;
DRAM_STATUS_T DramcClockDutyScan_InternalLoopBack(DRAMC_CTX_T *p, U8 out_of_spec_rescan_flag)
{
    U8 u1ByteIdx;
    U32 u4RevBX[DQS_NUMBER];
    U8 ucsearch_state, fgcurrent_value, fginitial_value, ucstart_period, ucend_period=0;
    S32 scdqs_dly, s4PICnt, s4PIBegin, s4PIEnd, s4PICnt_mod64;  
#if DUTY_PI_PREDICTION
    S32 s4PIPredict_MaxminPos[2]={0}, s4PIPredict_PosDiff=0;  
    U8 u1PILoopRangeNumber=0, u1PIRangeIdx;
#endif
    U32 u4sample_cnt, u4ones_cnt2, u4Last_Ones_Cnt=0;
    S8 scinner_duty_ofst=0, scFinal_clk_delay_cell=0;
    S8 scinner_duty_ofst_start, scinner_duty_ofst_end;
    U32 ucperiod_scan_state=CDS_TRASITION_STATE_BEFORE_FIRST ;
    U32 ucperiod, ucduty, ucduty_temp[2]={0,0};
    U32 ucperiod_common=0;
    U32 ucduty_fail_flag[2]={0,0};
    U32 ucperiod_duty; 
    BOOL ucfirst_is_low=0;
    U32 ucperiod_find_flag;
    U32 ucperiod_duty_max=0, ucperiod_duty_min=0xffffffff, ucperiod_duty_max_clk_dly=0, ucperiod_duty_min_clk_dly=0;
    U32 ucperiod_duty_averige=0, ucFinal_period_duty_averige=0, ucmost_approach_50_percent=0xffffffff;
    U32 ucFinal_period_duty_max = 0, ucFinal_period_duty_min = 0;
    U32 ucFinal_duty_max_clk_dly=0,ucFinal_duty_min_clk_dly=0;
    U8 early_break_count=0, ucDQSSwap;
    U32 HighLowSampleCount;
    S32 sdqsTransitionPos[2][3];
#if ClockDutyFastK
    S32 temp_scdqs_dly;
    BOOL ucprevious_first_is_low[2]={0,0};
    BOOL clockduty_scan_first_init_flag[2]={FALSE, FALSE};
    S32 DutyFastKNextScanStartPosition;
#endif
    U32 u4DutyDiff, u4DutyDiff_Limit=900;
    DRAM_STATUS_T retStatus = DRAM_OK;
    U8 u1LPBK_EN;
#if FT_DSIM_USED   
    U8 ucg_num_dlycell_perT = 90;    
#endif    
    U32 backup_DDRPHY_CA_CMD4;
    U32 backup_DRAMC_REG_EYESCAN;
    U32 backup_DDRPHY_MISC_EXTLB16;
    U32 backup_DDRPHY_MISC_EXTLB18;
    U32 backup_DDRPHY_MISC_EXTLB17;

#if FT_DSIM_USED==0
    U32 u4RegBackupAddress[] = 
    {       
        (DRAMC_REG_ADDR(DRAMC_REG_EYESCAN)),
        (DRAMC_REG_ADDR(DRAMC_REG_STBCAL1)),
        (DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1)),
        (DRAMC_REG_ADDR(DRAMC_REG_SHU_CONF0)),
        (DDRPHY_MISC_CTRL1 + ((U32)CHANNEL_B<<POS_BANK_NUM)),  
        (DDRPHY_MISC_CG_CTRL0 + ((U32)CHANNEL_B<<POS_BANK_NUM)),
        (DDRPHY_MISC_CG_CTRL3 + ((U32)CHANNEL_B<<POS_BANK_NUM)),
        (DDRPHY_CA_DLL_ARPI0 + ((U32)CHANNEL_B<<POS_BANK_NUM)),
        (DDRPHY_CA_DLL_ARPI2 + ((U32)CHANNEL_B<<POS_BANK_NUM)),
        (DDRPHY_CA_DLL_ARPI3 + ((U32)CHANNEL_B<<POS_BANK_NUM)),       
        (DDRPHY_B0_DQ6 + ((U32)CHANNEL_B<<POS_BANK_NUM)),
        (DDRPHY_B1_DQ6 + ((U32)CHANNEL_B<<POS_BANK_NUM)), 
        (DDRPHY_B0_DQ7 + ((U32)CHANNEL_B<<POS_BANK_NUM)),
        (DDRPHY_B1_DQ7 + ((U32)CHANNEL_B<<POS_BANK_NUM)), 
        (DDRPHY_CA_CMD0 + ((U32)CHANNEL_B<<POS_BANK_NUM)),
        (DDRPHY_CA_CMD1 + ((U32)CHANNEL_B<<POS_BANK_NUM)), 
        (DDRPHY_CA_CMD2 + ((U32)CHANNEL_B<<POS_BANK_NUM)), 
        (DDRPHY_CA_CMD3 + ((U32)CHANNEL_B<<POS_BANK_NUM)), 
        (DDRPHY_CA_CMD4 + ((U32)CHANNEL_B<<POS_BANK_NUM)), 
        (DDRPHY_CA_CMD5 + ((U32)CHANNEL_B<<POS_BANK_NUM)), 
        (DDRPHY_CA_CMD6 + ((U32)CHANNEL_B<<POS_BANK_NUM)), 
        (DDRPHY_CA_CMD7 + ((U32)CHANNEL_B<<POS_BANK_NUM)), 
        (DDRPHY_CA_CMD8 + ((U32)CHANNEL_B<<POS_BANK_NUM)),
        (DDRPHY_R0_B0_RXDVS2 + ((U32)CHANNEL_B<<POS_BANK_NUM)), 
        (DDRPHY_R0_B1_RXDVS2 + ((U32)CHANNEL_B<<POS_BANK_NUM)), 
        (DDRPHY_SHU1_R0_B0_DQ7 + ((U32)CHANNEL_B<<POS_BANK_NUM)), 
        (DDRPHY_SHU1_R0_B1_DQ7 + ((U32)CHANNEL_B<<POS_BANK_NUM)),       
        (DDRPHY_SHU1_R0_CA_CMD2 + ((U32)CHANNEL_B<<POS_BANK_NUM)),
        (DDRPHY_SHU1_R0_CA_CMD3 + ((U32)CHANNEL_B<<POS_BANK_NUM)),
        (DDRPHY_SHU1_R0_CA_CMD4 + ((U32)CHANNEL_B<<POS_BANK_NUM)),
        (DDRPHY_SHU1_R0_CA_CMD5 + ((U32)CHANNEL_B<<POS_BANK_NUM)),
        (DDRPHY_SHU1_R0_CA_CMD6 + ((U32)CHANNEL_B<<POS_BANK_NUM)),
        (DDRPHY_SHU1_R0_CA_CMD7 + ((U32)CHANNEL_B<<POS_BANK_NUM)),
        (DDRPHY_SHU1_R0_CA_CMD8 + ((U32)CHANNEL_B<<POS_BANK_NUM)),
        (DDRPHY_SHU1_R0_CA_CMD9 + ((U32)CHANNEL_B<<POS_BANK_NUM)),
        (DDRPHY_SHU1_CA_CMD8 + ((U32)CHANNEL_B<<POS_BANK_NUM)),
        (DDRPHY_MISC_EXTLB16 + ((U32)CHANNEL_B<<POS_BANK_NUM)),
        (DDRPHY_MISC_EXTLB17 + ((U32)CHANNEL_B<<POS_BANK_NUM)),
        (DDRPHY_MISC_EXTLB18 + ((U32)CHANNEL_B<<POS_BANK_NUM)),
        (DDRPHY_MISC_EXTLB19 + ((U32)CHANNEL_B<<POS_BANK_NUM)),
        };
#endif        

    mcSHOW_ERR_MSG(("\n[DramcClockDutyScan_InternalLoopBack]\n"));        
    /*TINFO="\n[DramcClockDutyScan_InternalLoopBack]\n" */
    
#if defined(SLT)
    if (out_of_spec_rescan_flag == 1)
    {
        u4DutyDiff_Limit = 535; //520; //500;
        u1LPBK_EN = 1;
    }
    else
#endif
    {
        u4DutyDiff_Limit = 900;    
        u1LPBK_EN = 0;    
    }
    
#if FT_DSIM_USED==0
    vPrintCalibrationBasicInfo(p);
#endif    

#if FT_DSIM_USED
	#ifdef SET_CLK_DLY 
		u1LPBK_EN =0;
		/* TINFO="define SET_CLK_DLY, scan clk dly = -8 ==\n" */
	#else
		u1LPBK_EN =1;
		/* TINFO="no define SET_CLK_DLY, scan clk dly = 0 ==\n" */		
	#endif
#endif

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    if(p->pSavetimeData->femmc_Ready==1)
    {
        DramcClockDutySetClkDelayCell(p, CHANNEL_B, RANK_0, p->pSavetimeData->s1ClockDuty_clk_delay_cell[CHANNEL_B][RANK_0]);
        DramcClockDutySetClkDelayCell(p, CHANNEL_B, RANK_1, p->pSavetimeData->s1ClockDuty_clk_delay_cell[CHANNEL_B][RANK_1]);        
        return DRAM_OK;
    }
#endif

    //backup register value
#if 1
#if FT_DSIM_USED==0
    DramcBackupRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));
#endif    
#else
    u4prv_register_EYESCAN = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN));
    u4prv_register_STBCAL1 = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_STBCAL1));

    u4prv_register_DDRPHY_MISC_CTRL1[CHANNEL_A] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1));
    u4prv_register_DDRPHY_MISC_CTRL1[CHANNEL_B] = u4IO32Read4B(DDRPHY_MISC_CTRL1 + (((U32)CHANNEL_B<<POS_BANK_NUM)));

    u4prv_register_DRAMC_REG_SHU_CONF0 = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SHU_CONF0));

#endif
    //set pin mux to LP3     
    vIO32WriteFldAlign(DDRPHY_MISC_CTRL1, 1,MISC_CTRL1_R_DMPINMUX);
    vIO32WriteFldAlign(DRAMC_REG_SHU_CONF0, 1,SHU_CONF0_FDIV2);
    
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI3 + ((U32)CHANNEL_B<<POS_BANK_NUM), 1,CA_DLL_ARPI3_RG_ARPI_CLKIEN_EN);        
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI2 + ((U32)CHANNEL_B<<POS_BANK_NUM), 0,CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN);                
    /* Golden settings & DCM off currently sets SHU1_CA_CMD8_R_RMRX_TOPHY_CG_IG_CA = 0
     * -> needs to be enabled to read CHB clk duty (So that CHB CA will have RX function in LP3 pinmux mode)
     */
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD8 + ((U32)CHANNEL_B<<POS_BANK_NUM), 1, SHU1_CA_CMD8_R_RMRX_TOPHY_CG_IG_CA);
    /* Justin: Disable(set to 0) "RX DQS ISI pulse CG function" during duty-related calibration
     * (must enable(set to 1) before leaving duty calibration)
     */
    RxDQSIsiPulseCG(p, DISABLE);

            DramcGatingMode(p, 0);


    // DQS duty test 1.
    #if 1
    //mcSHOW_ERR_MSG(("\n[RG*RPI*DQ.DQM*_EN*=0]\n"));
    mcSHOW_ERR_MSG(("\n[CA/CS PI_EN=0]\n"));
    vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI3 + ((U32)CHANNEL_B<<POS_BANK_NUM),
                                         P_Fld(0, CA_DLL_ARPI3_RG_ARPI_CS_EN)
                                        | P_Fld(0, CA_DLL_ARPI3_RG_ARPI_CMD_EN)); 
    //P_Fld(0, CA_DLL_ARPI3_RG_ARPI_CLK_EN)
    #endif

    // DQS duty test 2
    #if 1
    mcSHOW_ERR_MSG(("\n[RG*LPBK_EN*=%d]\n", u1LPBK_EN));
    /*TINFO="\n[RG*LPBK_EN*=%d]\n", u1LPBK_EN */
    vIO32WriteFldAlign(DDRPHY_CA_CMD6 + ((U32)CHANNEL_B<<POS_BANK_NUM), u1LPBK_EN, CA_CMD6_RG_RX_ARCMD_LPBK_EN);
    #endif

    // DQS duty test 3
    #if 1
    //mcSHOW_ERR_MSG(("\n[*PHDET_EN*=0]\n"));
    mcSHOW_ERR_MSG(("\n[*PI*RESETB*=0  *PHDET_EN*=0  *PI_RESETB*=1]\n"));
    /*TINFO="\n[*PI*RESETB*=0  *PHDET_EN*=0  *PI_RESETB*=1]\n" */
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI0 + ((U32)CHANNEL_B<<POS_BANK_NUM), 0x0, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI2 + ((U32)CHANNEL_B<<POS_BANK_NUM), 0x0, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI0 + ((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);
    #endif

    #if 1
    //DQS duty new, need to sync to clk duty....
    vIO32WriteFldAlign(DDRPHY_CA_CMD2+ ((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, CA_CMD2_RG_TX_ARCMD_OE_DIS);  // disable command output to dram
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_CMD7), 0x1, CA_CMD7_RG_TX_ARCMD_PULL_DN);
    #endif

    //MCK4X CG
    vIO32WriteFldAlign(DDRPHY_MISC_CTRL1 + ((U32)CHANNEL_B<<POS_BANK_NUM), 0, MISC_CTRL1_R_DMDQSIENCG_EN);

    // Bypass DQS glitch-free mode
    // RG_RX_*RDQ_EYE_DLY_DQS_BYPASS_B**
    vIO32WriteFldAlign(DDRPHY_CA_CMD6 + ((U32)CHANNEL_B<<POS_BANK_NUM), 1, CA_CMD6_RG_RX_ARCMD_EYE_DLY_DQS_BYPASS);

    //Enable DQ eye scan
    //RG_??_RX_EYE_SCAN_EN
    //RG_??_RX_VREF_EN
    //RG_??_RX_SMT_EN
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 1, EYESCAN_RG_RX_EYE_SCAN_EN);
    vIO32WriteFldMulti(DRAMC_REG_EYESCAN, P_Fld(0x1, EYESCAN_EYESCAN_DQS_SYNC_EN)    
                                        | P_Fld(0x1, EYESCAN_EYESCAN_NEW_DQ_SYNC_EN)    
                                        | P_Fld(0x1, EYESCAN_EYESCAN_DQ_SYNC_EN)); 

    vIO32WriteFldAlign(DDRPHY_CA_CMD5 + ((U32)CHANNEL_B<<POS_BANK_NUM), 1, CA_CMD5_RG_RX_ARCMD_EYE_EN);        
    vIO32WriteFldAlign(DDRPHY_CA_CMD5 + ((U32)CHANNEL_B<<POS_BANK_NUM), 1, CA_CMD5_RG_RX_ARCMD_VREF_EN);        
    vIO32WriteFldAlign(DDRPHY_CA_CMD3 + ((U32)CHANNEL_B<<POS_BANK_NUM), 1, CA_CMD3_RG_RX_ARCMD_SMT_EN);        

    //JM_SEL
    vIO32WriteFldAlign(DDRPHY_CA_CMD6, 0, CA_CMD6_RG_RX_ARCMD_JM_SEL);

    //Enable MIOCK jitter meter mode ( RG_RX_MIOCK_JIT_EN=1)
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 1, EYESCAN_RG_RX_MIOCK_JIT_EN);

    //Disable DQ eye scan (b'1), for counter clear
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 0, EYESCAN_RG_RX_EYE_SCAN_EN);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_STBCAL1), 0, STBCAL1_DQSERRCNT_DIS);

    ClockDutyScan_inter_loopback_preset(p);

#if DUTY_FINETUNE_K_TIME
    backup_DDRPHY_CA_CMD4 = u4IO32Read4B(DDRPHY_CA_CMD4 + (((U32)CHANNEL_B<<POS_BANK_NUM)));
    backup_DRAMC_REG_EYESCAN = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN));
    backup_DDRPHY_MISC_EXTLB16= u4IO32Read4B(DDRPHY_MISC_EXTLB16 + ((U32)CHANNEL_B<<POS_BANK_NUM));
    backup_DDRPHY_MISC_EXTLB18 = u4IO32Read4B(DDRPHY_MISC_EXTLB18 + ((U32)CHANNEL_B<<POS_BANK_NUM));
    backup_DDRPHY_MISC_EXTLB17 = u4IO32Read4B(DDRPHY_MISC_EXTLB17 + ((U32)CHANNEL_B<<POS_BANK_NUM));
#endif

    HighLowSampleCount = ClockDutyScan_InternalLoopBack_Find_Max_DQS_Cnt(p, backup_DDRPHY_CA_CMD4, backup_DRAMC_REG_EYESCAN, backup_DDRPHY_MISC_EXTLB16, backup_DDRPHY_MISC_EXTLB17, backup_DDRPHY_MISC_EXTLB18)>>1;    

#if ClockDutyFastK
    DutyFastKNextScanStartPosition = (CLOCK_PI_STEP*4*ucg_num_dlycell_perT*DUTY_INCREASE_PRECISION)>>6; //predicate next position is CLOCK_PI_STEP*4 (from justin), 1T=64PI
#endif    

    ////scFinal_clk_delay_cell = 0;
    ////ucFinal_period_duty_averige = 0;

#if ClockDutyPeriod_Use_JitterMeters_Period
            ucperiod_find_flag= CDS_PERIOD_STATE_FOUND_COMMON;
            ucperiod_common = p->ucnum_dlycell_perT*DUTY_INCREASE_PRECISION; //period from jitter meter
#else
            ucperiod_find_flag = CDS_PERIOD_STATE_STILL_NOT_FOUND;
#endif  

    if (out_of_spec_rescan_flag == 0)
    {
#if FT_DSIM_USED
#ifdef SET_CLK_DLY
            scinner_duty_ofst_start=-8; 
            scinner_duty_ofst_end=-8; 
#else
            scinner_duty_ofst_start=0; 
            scinner_duty_ofst_end=0; 
#endif
#else
            scinner_duty_ofst_start=DUTY_OFFSET_START; 
            scinner_duty_ofst_end=DUTY_OFFSET_END; 
#endif        
    }
    else
    {
            scinner_duty_ofst_start=gbackup_scFinal_clk_delay_cell; 
            scinner_duty_ofst_end=gbackup_scFinal_clk_delay_cell;     
    }

    for(scinner_duty_ofst=scinner_duty_ofst_start; scinner_duty_ofst<=scinner_duty_ofst_end; scinner_duty_ofst++) 
    {    
        if (scinner_duty_ofst<=0)
        {
            vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD1 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0-scinner_duty_ofst, SHU1_R0_CA_CMD1_RK0_TX_ARCLK_DLY) | P_Fld(0-scinner_duty_ofst, SHU1_R0_CA_CMD1_RK0_TX_ARCLKB_DLY));                          
        }
        else
        {
            vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD0 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(scinner_duty_ofst, SHU1_R0_CA_CMD0_RK0_TX_ARCLK_DLYB) | P_Fld(scinner_duty_ofst, SHU1_R0_CA_CMD0_RK0_TX_ARCLKB_DLYB));                                      
        }

#if (!__ETT__ && !FOR_DV_SIMULATION_USED && !FT_DSIM_USED)
        mtk_wdt_restart();
#endif

CLK_INTERNAL_LOOP_BACK_PI_RESCAN:
        
        ucperiod_duty_max=0;
        ucperiod_duty_min=0xffffffff;

        #if DUTY_PI_PREDICTION
        if((scinner_duty_ofst==DUTY_OFFSET_START) || (ucperiod_find_flag == CDS_PERIOD_STATE_STILL_NOT_FOUND))
        {
            s4PIBegin = CLOCK_PI_START;
            s4PIEnd = CLOCK_PI_END;
            u1PILoopRangeNumber =1; // full scan
            #if ClockDutyPrintAllLog
            mcSHOW_ERR_MSG(("[%d]PI Range Full\n", scinner_duty_ofst));
            /*TINFO="[%d]PI Range Full\n", scinner_duty_ofst */
            #endif
        }
        else
        {
            s4PIPredict_PosDiff = s4PIPredict_MaxminPos[1] -s4PIPredict_MaxminPos[0];
            if (s4PIPredict_PosDiff <= DUTY_PI_PREDICTION)
            {  
                s4PIBegin = s4PIPredict_MaxminPos[0] - DUTY_PI_PREDICTION;
                s4PIEnd = s4PIPredict_MaxminPos[1] + DUTY_PI_PREDICTION;
                u1PILoopRangeNumber =1;
            }
            else
            {
                u1PILoopRangeNumber =2;
            }
        }
#else
        s4PIBegin = CLOCK_PI_START;
        s4PIEnd = CLOCK_PI_END;
#endif

    #if DUTY_PI_PREDICTION
    for(u1PIRangeIdx=0; u1PIRangeIdx < u1PILoopRangeNumber; u1PIRangeIdx++)
    #endif
    {
#if ClockDutyFastK     
        clockduty_scan_first_init_flag[0]= TRUE;    //swap 0
        clockduty_scan_first_init_flag[1]= TRUE;    // swap 1

        sdqsTransitionPos[0][0]=sdqsTransitionPos[0][1]=sdqsTransitionPos[0][2]=DUTY_TANSITION_INVALID;
        sdqsTransitionPos[1][0]=sdqsTransitionPos[1][1]=sdqsTransitionPos[1][2]=DUTY_TANSITION_INVALID;
#endif

            #if DUTY_PI_PREDICTION
            if(u1PILoopRangeNumber==2) // two PI range
            {
                s4PIBegin = s4PIPredict_MaxminPos[u1PIRangeIdx] - DUTY_PI_PREDICTION;
                s4PIEnd = s4PIPredict_MaxminPos[u1PIRangeIdx] + DUTY_PI_PREDICTION;
                #if ClockDutyPrintAllLog
                mcSHOW_ERR_MSG(("[%d]PI Range %d : %d ~ %d\n", scinner_duty_ofst, u1PIRangeIdx, s4PIBegin, s4PIEnd));
                /*TINFO="[%d]PI Range %d : %d ~ %d\n", scinner_duty_ofst, u1PIRangeIdx, s4PIBegin, s4PIEnd */
                #endif
            }
            #endif

        for(s4PICnt=s4PIBegin; s4PICnt<=s4PIEnd; s4PICnt+=CLOCK_PI_STEP)
        {
            s4PICnt_mod64 = (s4PICnt+64)&0x3f;//s4PICnt_mod64 = (s4PICnt+64)%64;
            //if(scinner_duty_ofst!=DUTY_OFFSET_START)
            //    mcSHOW_ERR_MSG(("PI= %d\n", s4PICnt_mod64));

            vIO32WriteFldAlign(DDRPHY_SHU1_R0_CA_CMD9 + ((U32)CHANNEL_B<<POS_BANK_NUM), s4PICnt_mod64, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CLK);                

#if FT_DSIM_USED
#ifdef SET_CLK_DLY 
        for(ucDQSSwap=0; ucDQSSwap<2; ucDQSSwap++)  //swap begin
#else        
        for(ucDQSSwap=0; ucDQSSwap<=0; ucDQSSwap++)  //don't do swap
#endif
#else
        for(ucDQSSwap=0; ucDQSSwap<2; ucDQSSwap++)  //swap begin
#endif        
        {
            vIO32WriteFldAlign(DDRPHY_CA_CMD3+ ((U32)CHANNEL_B<<POS_BANK_NUM), ucDQSSwap, CA_CMD3_RG_RX_ARCLK_SWAP_EN); 

            ucduty_fail_flag[ucDQSSwap] = 1;
            ucperiod_scan_state=CDS_TRASITION_STATE_BEFORE_FIRST ;

#if ClockDutyPrintAllLog
            //for SLT, use ERR_MSG to force print log
            if (ucperiod_find_flag!=CDS_PERIOD_STATE_STILL_NOT_FOUND)
                mcSHOW_ERR_MSG(("[Clk PI = %d] |", s4PICnt_mod64));
                /*TINFO="[Clk PI = %d] |", s4PICnt_mod64 */
#endif
            for (scdqs_dly=DQS_DLY_START; scdqs_dly<DQS_DLY_END; scdqs_dly+=DQS_DLY_STEP)
            {

#if ClockDutyFastK
                temp_scdqs_dly=scdqs_dly;
                if (clockduty_scan_first_init_flag[ucDQSSwap]==FALSE && scdqs_dly==0)
                {
                    if ((ucperiod_find_flag!=CDS_PERIOD_STATE_STILL_NOT_FOUND) && s4PICnt!=s4PIBegin) 
                    {
                        if (sdqsTransitionPos[ucDQSSwap][0]!=DUTY_TANSITION_INVALID && (((sdqsTransitionPos[ucDQSSwap][0]-DutyFastKNextScanStartPosition)/DUTY_INCREASE_PRECISION) > 0))
                        {
                            //skip scanning from scdqs_dly to sdqsTransitionPos[ucDQSSwap][0]-DutyFastKNextScanStartPosition to reduce calibration time
                            scdqs_dly=(sdqsTransitionPos[ucDQSSwap][0]-DutyFastKNextScanStartPosition)/DUTY_INCREASE_PRECISION;
                        }    
                    }
                }
#endif

            
                //Set DQS delay (RG_??_RX_DQS_EYE_DLY)
#if DUTY_FINETUNE_K_TIME
                backup_DDRPHY_CA_CMD4 &= 0xffff8080;
                backup_DDRPHY_CA_CMD4 |= ((scdqs_dly<<8) | (scdqs_dly));
                vIO32Write4B(DDRPHY_CA_CMD4 + (((U32)CHANNEL_B<<POS_BANK_NUM)), backup_DDRPHY_CA_CMD4); 
#else
                vIO32WriteFldMulti(DDRPHY_CA_CMD4 + (((U32)CHANNEL_B<<POS_BANK_NUM)), P_Fld(scdqs_dly, CA_CMD4_RG_RX_ARCLK_EYE_R_DLY) | P_Fld(scdqs_dly, CA_CMD4_RG_RX_ARCLK_EYE_F_DLY));
#endif
    
                //DramPhyReset(p);
    
                //Reset eye scan counters (reg_sw_rst): 1 to 0
#if DUTY_FINETUNE_K_TIME
                vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), backup_DRAMC_REG_EYESCAN | 0x1);
                vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), backup_DRAMC_REG_EYESCAN & ~(0x1));
#else
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 1, EYESCAN_REG_SW_RST);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 0, EYESCAN_REG_SW_RST);
#endif                
    
                //Enable DQ eye scan (b'1)
#if DUTY_FINETUNE_K_TIME
                vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), backup_DRAMC_REG_EYESCAN | (1<<1));
#else
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 1, EYESCAN_RG_RX_EYE_SCAN_EN);
#endif
    
                //inter loop back
                ClockDutyScan_int_lpbk_test_seq(p, backup_DDRPHY_MISC_EXTLB16, backup_DDRPHY_MISC_EXTLB17, backup_DDRPHY_MISC_EXTLB18);

                // according to shih-hsiu, internal loopback no need to wait 1ms to sample 500 samples
                // 2ns/sample, here we delay 1ms about 500 samples
                //mcDELAY_US(1000);
    
                //Disable DQ eye scan (b'1), for counter latch
#if DUTY_FINETUNE_K_TIME
                vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), backup_DRAMC_REG_EYESCAN & ~(1<<1));
#else  
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 0, EYESCAN_RG_RX_EYE_SCAN_EN);
#endif
    
                //Read the counter values from registers (toggle_cnt*, dqs_err_cnt*);
                u4ones_cnt2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQS2_ERR_CNT), DQS2_ERR_CNT_DQS2_ERR_CNT);
#if ClockDutyPrintAllLog
#if ClockDutyPrintAllLog_Type
                //for SLT, use ERR_MSG to force print log
                if (ucperiod_find_flag!=CDS_PERIOD_STATE_STILL_NOT_FOUND)
                {
                    if (u4ones_cnt2 >= HighLowSampleCount) 
                    {
                        //for SLT, use ERR_MSG to force print log
                        mcSHOW_ERR_MSG(("O"));
                        /*TINFO="O" */
                    }
                    else 
                    {
                        //for SLT, use ERR_MSG to force print log
                        mcSHOW_ERR_MSG(("_"));
                        /*TINFO="_" */
                    }
                }
#else
            #ifdef ETT_PRINT_FORMAT
                    //for SLT, use ERR_MSG to force print log
                    mcSHOW_ERR_MSG(("[Clk PI = %d] %d : %d\n", s4PICnt_mod64, scdqs_dly, u4ones_cnt2));
                    /*TINFO="[Clk PI = %d] %d : %d\n", s4PICnt_mod64, scdqs_dly, u4ones_cnt2 */
            #else
                    mcSHOW_DBG_MSG(("[Clk PI = %d] %3d : %8d\n", s4PICnt_mod64, scdqs_dly, u4ones_cnt2));
            #endif
#endif
#endif            
    
#if ClockDutyFastK
                if(temp_scdqs_dly==0)  
                {
                    if (u4ones_cnt2 <= HighLowSampleCount) ucfirst_is_low = TRUE;
                    else ucfirst_is_low = FALSE;

                    if (ucperiod_find_flag!=CDS_PERIOD_STATE_STILL_NOT_FOUND)
                    {
                        if (clockduty_scan_first_init_flag[ucDQSSwap]==FALSE)
                        {
                            if (ucprevious_first_is_low[ucDQSSwap] != ucfirst_is_low)
                            {
                                ucprevious_first_is_low[ucDQSSwap] = ucfirst_is_low;
                                sdqsTransitionPos[ucDQSSwap][0]=sdqsTransitionPos[ucDQSSwap][1]=sdqsTransitionPos[ucDQSSwap][2]=DUTY_TANSITION_INVALID;  ///TODO: te be remove
                                scdqs_dly=0;
                                clockduty_scan_first_init_flag[ucDQSSwap]=TRUE;
                                u4Last_Ones_Cnt = u4ones_cnt2;             
#if ClockDutyPrintAllLog
                                mcSHOW_ERR_MSG(("scdqs_dly rescan\n"));
                                /*TINFO="scdqs_dly rescan\n" */
#endif
                                continue;
                            }
                        }
                        ucprevious_first_is_low[ucDQSSwap] = ucfirst_is_low;
                    }
                }
#else  
                if (scdqs_dly == 0)
                {
                    if (u4ones_cnt2 <= HighLowSampleCount) ucfirst_is_low = TRUE;
                    else ucfirst_is_low = FALSE;
                }
#endif
    
                switch(ucperiod_scan_state)
                {
                    case CDS_TRASITION_STATE_BEFORE_FIRST :
                        //if (u4ones_cnt2 >= HighLowSampleCount)
                        if(((ucfirst_is_low==TRUE)&&(u4ones_cnt2 >= HighLowSampleCount)) ||
                           ((ucfirst_is_low==FALSE)&&(u4ones_cnt2 < HighLowSampleCount)))
                        {
                            sdqsTransitionPos[ucDQSSwap][0] = u4TrasitionInterpolation(u4ones_cnt2, u4Last_Ones_Cnt,HighLowSampleCount, scdqs_dly, scdqs_dly-DQS_DLY_STEP);
                                #if ClockDutyPrintAllLog
                                mcSHOW_ERR_MSG(("sdqsTransitionPos[%d][0] = %d\n", ucDQSSwap, sdqsTransitionPos[ucDQSSwap][0]));
                                /*TINFO="sdqsTransitionPos[%d][0] = %d\n", ucDQSSwap, sdqsTransitionPos[ucDQSSwap][0] */
                                #endif
                            //sdqsTransitionPos[ucDQSSwap][0] = scdqs_dly;  
                            ucperiod_scan_state = CDS_TRASITION_STATE_FOUND_FIRST ;
#if ClockDutyFastK                                
                            temp_scdqs_dly = scdqs_dly;                                
                            if (ucperiod_find_flag!=CDS_PERIOD_STATE_STILL_NOT_FOUND) 
                            {
                                if (clockduty_scan_first_init_flag[ucDQSSwap]==FALSE && sdqsTransitionPos[ucDQSSwap][1]!=DUTY_TANSITION_INVALID) 
                                {
                                    //skip scanning from scdqs_dly to sdqsTransitionPos[ucDQSSwap][1]-DutyFastKNextScanStartPosition to reduce calibration time
                                    scdqs_dly=(sdqsTransitionPos[ucDQSSwap][1]-DutyFastKNextScanStartPosition)/DUTY_INCREASE_PRECISION;
                                }    
                            }
#endif
                        }
                    break;
                    case CDS_TRASITION_STATE_FOUND_FIRST :
                        //if (u4ones_cnt2 < HighLowSampleCount)
                        if(((ucfirst_is_low==TRUE)&&(u4ones_cnt2 < HighLowSampleCount)) ||
                           ((ucfirst_is_low==FALSE)&&(u4ones_cnt2 >= HighLowSampleCount)))
                        {
                            sdqsTransitionPos[ucDQSSwap][1] = u4TrasitionInterpolation(u4ones_cnt2, u4Last_Ones_Cnt,HighLowSampleCount, scdqs_dly, scdqs_dly-DQS_DLY_STEP);
                            #if ClockDutyPrintAllLog                          
                            mcSHOW_ERR_MSG(("sdqsTransitionPos[%d][1] = %d \n", ucDQSSwap, sdqsTransitionPos[ucDQSSwap][1]));
                            /*TINFO="sdqsTransitionPos[%d][1] = %d \n", ucDQSSwap, sdqsTransitionPos[ucDQSSwap][1] */
                            #endif
                            //sdqsTransitionPos[ucDQSSwap][1] = scdqs_dly;
#if ClockDutyPeriod_Use_JitterMeters_Period
                            ucperiod_scan_state = CDS_TRASITION_STATE_END;
#else
                            ucperiod_scan_state = CDS_TRASITION_STATE_FOUND_SECOND ;
#endif
#if ClockDutyFastK                                
                            temp_scdqs_dly = scdqs_dly;                                
                            if (ucperiod_find_flag!=CDS_PERIOD_STATE_STILL_NOT_FOUND) 
                            {   
                                if (clockduty_scan_first_init_flag[ucDQSSwap]==FALSE)
                                {
                                    if (sdqsTransitionPos[ucDQSSwap][2]!=DUTY_TANSITION_INVALID) 
                                    {
                                        //skip scanning from scdqs_dly to sdqsTransitionPos[ucDQSSwap][2]-DutyFastKNextScanStartPosition to reduce calibration time
                                        scdqs_dly=(sdqsTransitionPos[ucDQSSwap][2]-DutyFastKNextScanStartPosition)/DUTY_INCREASE_PRECISION;
                                    }                
                                    else 
                                    {
                                        scdqs_dly=DQS_DLY_END-DutyFastKNextScanStartPosition/DUTY_INCREASE_PRECISION;
                                    }
                                }    
                            }
#endif
#if ClockDutyPeriod_Use_JitterMeters_Period
#if ClockDutyFastK
                            if (clockduty_scan_first_init_flag[ucDQSSwap]==FALSE) 
                            {
                                //skip left calibration
                                scdqs_dly=DQS_DLY_END;
                            }  
#endif
#endif
                        }
                    break;

                    case CDS_TRASITION_STATE_FOUND_SECOND :
                            //if (u4ones_cnt2 >= HighLowSampleCount)
                            if(((ucfirst_is_low==TRUE)&&(u4ones_cnt2 >= HighLowSampleCount)) ||
                               ((ucfirst_is_low==FALSE)&&(u4ones_cnt2 < HighLowSampleCount)))                               
                            {
                            
                                sdqsTransitionPos[ucDQSSwap][2] = u4TrasitionInterpolation(u4ones_cnt2, u4Last_Ones_Cnt,HighLowSampleCount, scdqs_dly, scdqs_dly-DQS_DLY_STEP);
#if ClockDutyPrintAllLog
                                mcSHOW_ERR_MSG(("sdqsTransitionPos[%d][2] = %d\n", ucDQSSwap, sdqsTransitionPos[ucDQSSwap][2]));
                                /*TINFO="sdqsTransitionPos[%d][2] = %d\n", ucDQSSwap, sdqsTransitionPos[ucDQSSwap][2] */
#endif
                                //sdqsTransitionPos[ucDQSSwap][2] = scdqs_dly;
                            ucperiod_scan_state = CDS_TRASITION_STATE_FOUND_THIRD ;
#if ClockDutyFastK                                
                            if (ucperiod_find_flag!=CDS_PERIOD_STATE_STILL_NOT_FOUND) 
                            {
                                if (clockduty_scan_first_init_flag[ucDQSSwap]==FALSE) 
                                {
                                    //skip left calibration
                                    scdqs_dly=DQS_DLY_END;
                                }    
                            }
#endif
                            if (ucperiod_find_flag == CDS_PERIOD_STATE_STILL_NOT_FOUND) ucperiod_find_flag = CDS_PERIOD_STATE_FOUND;
                        }                
                    break;
                }
                u4Last_Ones_Cnt = u4ones_cnt2;
            }
    
#if ClockDutyPrintAllLog
#if ClockDutyPrintAllLog_Type
            //for SLT, use ERR_MSG to force print log
            if (ucperiod_find_flag!=CDS_PERIOD_STATE_STILL_NOT_FOUND) mcSHOW_ERR_MSG(("\n"));
#endif            
#endif  
#if ClockDutyFastK
            if ((ucperiod_find_flag!=CDS_PERIOD_STATE_STILL_NOT_FOUND) && clockduty_scan_first_init_flag[ucDQSSwap]==TRUE)
            {
                clockduty_scan_first_init_flag[ucDQSSwap]=FALSE;
            }    
#endif
                
#if ClockDutyPeriod_Use_JitterMeters_Period
            ucperiod = ucperiod_common;
#else
            if (ucperiod_find_flag == CDS_PERIOD_STATE_FOUND)
            {
                ucperiod_find_flag = CDS_PERIOD_STATE_FOUND_COMMON;

                #if 0
                if (ucfirst_is_low==TRUE)
                {
                    ucperiod_common = ucperiod_third_low_2_high - ucperiod_first_low_2_high;
                }
                else
                {
                    ucperiod_common = ucperiod_third_high_2_low - ucperiod_first_high_2_low;                
                }
                #else
                ucperiod_common = sdqsTransitionPos[ucDQSSwap][2] - sdqsTransitionPos[ucDQSSwap][0];
                #endif

                goto CLK_INTERNAL_LOOP_BACK_PI_RESCAN;
            }
            else if (ucperiod_find_flag == CDS_PERIOD_STATE_FOUND_COMMON)
            {
                if (ucperiod_scan_state == CDS_TRASITION_STATE_FOUND_THIRD)
                {
                    #if 0
                    if (ucfirst_is_low==TRUE)
                    {
                        ucperiod = ucperiod_third_low_2_high - ucperiod_first_low_2_high;
                    }
                    else
                    {
                        ucperiod = ucperiod_third_high_2_low - ucperiod_first_high_2_low;                
                    }            
                    #else
                    ucperiod = sdqsTransitionPos[ucDQSSwap][2] - sdqsTransitionPos[ucDQSSwap][0];
                    ucperiod_common= ucperiod; // yulia change
                    #endif
                }
                else
                {
                    ucperiod = ucperiod_common;
                }
            }
#endif            

            //ucperiod_find_flag_temp[ucDQSSwap] = ucperiod_find_flag;

            if (ucperiod_find_flag == CDS_PERIOD_STATE_FOUND_COMMON)
            {        
                #if 0
                if (ucfirst_is_low==TRUE)
                {
                    ucperiod_duty = ucperiod_second_high_2_low - ucperiod_first_low_2_high;
                    ucduty_temp[ucDQSSwap] = (ucperiod_duty*10000)/ucperiod;                
                }
                else
                {
                    ucperiod_duty = ucperiod_second_low_2_high - ucperiod_first_high_2_low;                
                    ucduty_temp[ucDQSSwap] = 10000 - (ucperiod_duty*10000)/ucperiod;                                
                }
                #else
                
                ucperiod_duty = (U32)(sdqsTransitionPos[ucDQSSwap][1] - sdqsTransitionPos[ucDQSSwap][0]);
                
                ///TODO: Need to debug!!!!ucperiod is 0, transition is wrong.
                if(ucperiod !=0)  // div0 protection  
                {
                    if (ucfirst_is_low==TRUE)
                    {
                        ucduty_temp[ucDQSSwap] = (ucperiod_duty*10000)/ucperiod;                
                    }
                    else
                    {
                        ucduty_temp[ucDQSSwap] = 10000 - (ucperiod_duty*10000)/ucperiod;                                
                    }
                    
                    ucduty_fail_flag[ucDQSSwap] =0; //duty valid
                }
                else
                {
                    ucduty_temp[ucDQSSwap] =0;
                    mcSHOW_ERR_MSG(("[Inner Clk Delay Cell = %d][Clk PI = %d]\n",scinner_duty_ofst, s4PICnt_mod64));
                    #if __ETT__
                    mcSHOW_ERR_MSG(("\nCLK period is 0!!! cannot div 0\n"));
                    while(1);
                    #endif
                }
                #endif
        
                //mcSHOW_ERR_MSG(("sdqsTransitionPos[%d][0] = %d\n", ucDQSSwap, sdqsTransitionPos[ucDQSSwap][0]));
                //mcSHOW_ERR_MSG(("sdqsTransitionPos[%d][1] = %d\n", ucDQSSwap, sdqsTransitionPos[ucDQSSwap][1]));
                //if (ucperiod_scan_state == CDS_TRASITION_STATE_FOUND_THIRD ) mcSHOW_ERR_MSG(("sdqsTransitionPos[%d][2] = %d\n", ucDQSSwap, sdqsTransitionPos[ucDQSSwap][2]));

#if ClockDutyPrintAllLog
                //for SLT, use ERR_MSG to force print log
                mcSHOW_ERR_MSG(("[Inner Clk Delay Cell = %d][Clk PI = %d]\n",scinner_duty_ofst, s4PICnt_mod64));
                /*TINFO="[Inner Clk Delay Cell = %d][Clk PI = %d]\n",scinner_duty_ofst, s4PICnt_mod64 */
                #if 0
                if (ucfirst_is_low==TRUE)
                {
                    //for SLT, use ERR_MSG to force print log
                    mcSHOW_ERR_MSG(("Francis ucperiod=%d(%d ~ %d)\n",ucperiod, ucperiod_first_low_2_high, ucperiod_third_low_2_high));
                    mcSHOW_ERR_MSG(("Francis duty_high=%d(%d ~ %d)\n",ucperiod_duty, ucperiod_first_low_2_high, ucperiod_second_high_2_low));
                }
                else
                {
                    //for SLT, use ERR_MSG to force print log
                    mcSHOW_ERR_MSG(("Francis ucperiod=%d(%d ~ %d)\n",ucperiod, ucperiod_first_high_2_low, ucperiod_third_high_2_low));
                    mcSHOW_ERR_MSG(("Francis duty_low=%d(%d ~ %d)\n",ucperiod_duty, ucperiod_first_high_2_low, ucperiod_second_low_2_high));
                }
                #else
                mcSHOW_ERR_MSG(("Francis ucperiod=%d(%d ~ %d)\n",ucperiod, sdqsTransitionPos[ucDQSSwap][0], sdqsTransitionPos[ucDQSSwap][2]));
                /*TINFO="Francis ucperiod=%d(%d ~ %d)\n",ucperiod, sdqsTransitionPos[ucDQSSwap][0], sdqsTransitionPos[ucDQSSwap][2] */
                if (ucfirst_is_low==TRUE)
                {
                    mcSHOW_ERR_MSG(("Francis duty_high=%d(%d ~ %d)\n",ucperiod_duty, sdqsTransitionPos[ucDQSSwap][0], sdqsTransitionPos[ucDQSSwap][1]));
                    /*TINFO="Francis duty_high=%d(%d ~ %d)\n",ucperiod_duty, sdqsTransitionPos[ucDQSSwap][0], sdqsTransitionPos[ucDQSSwap][1] */
                }
                else
                {   
                    mcSHOW_ERR_MSG(("Francis duty_low=%d(%d ~ %d)\n",ucperiod_duty, sdqsTransitionPos[ucDQSSwap][0], sdqsTransitionPos[ucDQSSwap][1]));
                    /*TINFO="Francis duty_low=%d(%d ~ %d)\n",ucperiod_duty, sdqsTransitionPos[ucDQSSwap][0], sdqsTransitionPos[ucDQSSwap][1] */
                }
                #endif
                mcSHOW_ERR_MSG(("Francis clock Duty temp(Swap%d)= %d%%(X100)\n",ucDQSSwap, ucduty_temp[ucDQSSwap]));
                /*TINFO="Francis clock Duty temp(Swap%d)= %d%%(X100)\n",ucDQSSwap, ucduty_temp[ucDQSSwap] */
#endif                
            }
            }// end of swap

        if ((ucduty_fail_flag[0] == 0) &&(ucduty_fail_flag[1] == 0))  //duty valid for both swap 0 and 1
        {

#if FT_DSIM_USED
#ifdef SET_CLK_DLY 
            ucduty = (ucduty_temp[0]+ (10000-ucduty_temp[1])) >>1;
#else        
        ucduty = ucduty_temp[0];
#endif
#else
        ucduty = (ucduty_temp[0]+ (10000-ucduty_temp[1])) >>1;
#endif            

            
#if ClockDutyPrintAllLog
            mcSHOW_DBG_MSG(("Swap0 Duty %d, Swap1 Duty %d, Francis clock Duty = %d%%(X100)\n", ucduty_temp[0], ucduty_temp[1], ucduty));
            /*TINFO="Swap0 Duty %d, Swap1 Duty %d, Francis clock Duty = %d%%(X100)\n", ucduty_temp[0], ucduty_temp[1], ucduty */
#endif

            if (ucperiod_duty_max < ucduty)
            {
                ucperiod_duty_max = ucduty;
                ucperiod_duty_max_clk_dly = s4PICnt_mod64;
            }
            
            if (ucperiod_duty_min > ucduty)
            {
                ucperiod_duty_min = ucduty;
                ucperiod_duty_min_clk_dly = s4PICnt_mod64;
            }
        }           
#if ClockDutyFastK            
            if (ucperiod_find_flag == CDS_PERIOD_STATE_STILL_NOT_FOUND) s4PICnt+= 15; //to find period quickly
#endif

            }
        }// end of u1PIRangeIdx

  //            mcSHOW_DBG_MSG(("\n[%d] Clock Duty Pass !!!!!\n", scinner_duty_ofst));
            #if DUTY_PI_PREDICTION
            if(scinner_duty_ofst==DUTY_OFFSET_START)
            {
                // make s4PIPredict_MaxminPos[0] < s4PIPredict_MaxminPos[1]
                if(ucperiod_duty_max_clk_dly < ucperiod_duty_min_clk_dly)
                {
                    s4PIPredict_MaxminPos[0] = ucperiod_duty_max_clk_dly;
                    s4PIPredict_MaxminPos[1] = ucperiod_duty_min_clk_dly;
                }
                else
                {              
                    s4PIPredict_MaxminPos[0] = ucperiod_duty_min_clk_dly;
                    s4PIPredict_MaxminPos[1] = ucperiod_duty_max_clk_dly;
                }
            }
            #endif
            ucperiod_duty_averige = (ucperiod_duty_max + ucperiod_duty_min)/2;
        
            //for SLT, use ERR_MSG to force print log
            mcSHOW_ERR_MSG(("[%d] MAX Duty = %d%%(X100), CLK PI=%d\n",scinner_duty_ofst, ucperiod_duty_max, ucperiod_duty_max_clk_dly));
            /*TINFO="[%d] MAX Duty = %d%%(X100), CLK PI=%d\n",scinner_duty_ofst, ucperiod_duty_max, ucperiod_duty_max_clk_dly */
            mcSHOW_ERR_MSG(("[%d] MIN Duty = %d%%(X100), CLK PI=%d\n",scinner_duty_ofst, ucperiod_duty_min, ucperiod_duty_min_clk_dly));
            /*TINFO="[%d] MIN Duty = %d%%(X100), CLK PI=%d\n",scinner_duty_ofst, ucperiod_duty_min, ucperiod_duty_min_clk_dly */

            mcSHOW_ERR_MSG(("[%d] AVG Duty = %d%%(X100)\n", scinner_duty_ofst, ucperiod_duty_averige));        
            /*TINFO="[%d] AVG Duty = %d%%(X100)\n", scinner_duty_ofst, ucperiod_duty_averige */

            if (ucperiod_duty_averige >= ClockDutyMiddleBound)
            {
                if ((ucperiod_duty_averige-ClockDutyMiddleBound) <= ucmost_approach_50_percent)
                {
                    ucmost_approach_50_percent = ucperiod_duty_averige-ClockDutyMiddleBound;
                    scFinal_clk_delay_cell = scinner_duty_ofst;
                    ucFinal_period_duty_averige = ucperiod_duty_averige;
                    ucFinal_period_duty_max = ucperiod_duty_max;
                    ucFinal_period_duty_min = ucperiod_duty_min;                       
                    ucFinal_duty_max_clk_dly = ucperiod_duty_max_clk_dly;
                    ucFinal_duty_min_clk_dly = ucperiod_duty_min_clk_dly;
                    mcSHOW_ERR_MSG(("!!! ucmost_approach_50_percent = %d%%(X100) !!!\n",ucmost_approach_50_percent));
                    /*TINFO="!!! ucmost_approach_50_percent = %d%%(X100) !!!\n",ucmost_approach_50_percent */
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
                    scFinal_clk_delay_cell = scinner_duty_ofst;
                    ucFinal_period_duty_averige = ucperiod_duty_averige;
                    ucFinal_period_duty_max = ucperiod_duty_max;
                    ucFinal_period_duty_min = ucperiod_duty_min; 
                    ucFinal_duty_max_clk_dly = ucperiod_duty_max_clk_dly;
                    ucFinal_duty_min_clk_dly = ucperiod_duty_min_clk_dly;                   
                    mcSHOW_ERR_MSG(("!!! ucmost_approach_50_percent = %d%%(X100) !!!\n",ucmost_approach_50_percent));                    
                    /*TINFO="!!! ucmost_approach_50_percent = %d%%(X100) !!!\n",ucmost_approach_50_percent */
                    early_break_count = 0;
                }            
                else
                {
                    early_break_count ++;
                    if (early_break_count>=2) break; //early break;
                }                
            }            

    }

    //for SLT, use ERR_MSG to force print log
    mcSHOW_ERR_MSG(("Find Final clock duty delay cell = %d\n", scFinal_clk_delay_cell));    
    /*TINFO="Find Final clock duty delay cell = %d\n", scFinal_clk_delay_cell */
    mcSHOW_ERR_MSG(("\n[%d] MAX Duty = %d%%(X100), CLK PI = %d\n",scFinal_clk_delay_cell, ucFinal_period_duty_max, ucFinal_duty_max_clk_dly));
    /*TINFO="\n[%d] MAX Duty = %d%%(X100), CLK PI = %d\n",scFinal_clk_delay_cell, ucFinal_period_duty_max, ucFinal_duty_max_clk_dly */
    mcSHOW_ERR_MSG(("[%d] MIN Duty = %d%%(X100), CLK PI = %d\n",scFinal_clk_delay_cell, ucFinal_period_duty_min, ucFinal_duty_min_clk_dly));
    /*TINFO="[%d] MIN Duty = %d%%(X100), CLK PI = %d\n",scFinal_clk_delay_cell, ucFinal_period_duty_min, ucFinal_duty_min_clk_dly */
    mcSHOW_ERR_MSG(("[%d] AVG Duty = %d%%(X100)\n", scFinal_clk_delay_cell, ucFinal_period_duty_averige));
    /*TINFO="[%d] AVG Duty = %d%%(X100)\n", scFinal_clk_delay_cell, ucFinal_period_duty_averige */
        
    //mcSHOW_ERR_MSG(("Max duty = %d%%(X100) ,Min duty = %d%%(X100) \n",ucFinal_period_duty_max, ucFinal_period_duty_min));
    //mcSHOW_ERR_MSG(("Final clock duty = %d%%(X100), ",ucFinal_period_duty_averige)); 
    
    u4DutyDiff = ucFinal_period_duty_max - ucFinal_period_duty_min;

#if FT_DSIM_USED
		FT_ClockDuty_InternalLoopBack_Compare_PassFail(ucFinal_period_duty_max, ucFinal_period_duty_min);
#else
#if defined(SLT)
		if ((u4DutyDiff < u4DutyDiff_Limit) &&
        ((ucFinal_period_duty_averige >= 4700 && ucFinal_period_duty_averige <= 5300 && out_of_spec_rescan_flag==0) || (out_of_spec_rescan_flag==1)))
#else
		if ((u4DutyDiff < u4DutyDiff_Limit) &&
        (ucFinal_period_duty_averige >= 4700 && ucFinal_period_duty_averige <= 5300))
#endif
    {
        mcSHOW_ERR_MSG(("\n CHB CLK Duty spec in!! Max-Min= %d%%\n", u4DutyDiff));            
        /*TINFO="\n CHB CLK Duty spec in!! Max-Min= %d%%\n", u4DutyDiff */
    }
    else
    {
        mcSHOW_ERR_MSG(("\n CHB CLK Duty spec out!! Max-Min= %d%% >%d%%\n", u4DutyDiff, u4DutyDiff_Limit));
        /*TINFO="\n CHB CLK Duty spec out!! Max-Min= %d%% >%d%%\n", u4DutyDiff, u4DutyDiff_Limit */
        
            #if defined(SLT)
                if (out_of_spec_rescan_flag==1) //only when LPBKEN=1 fail, then while(1);
                {
                while(1); //stop here 
                }
            #endif

            #if __ETT__
                if (out_of_spec_rescan_flag==1)  //only when LPBKEN=1 fail, then while(1);
                {
                while(1); //stop here 
                }
                #endif
    }    
#endif    
    
    if (out_of_spec_rescan_flag==0)
    {
        gbackup_scFinal_clk_delay_cell = scFinal_clk_delay_cell;
        DramcClockDutySetClkDelayCell(p, CHANNEL_B, RANK_0, scFinal_clk_delay_cell);
        DramcClockDutySetClkDelayCell(p, CHANNEL_B, RANK_1, scFinal_clk_delay_cell);    
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(p->pSavetimeData->femmc_Ready==0)
        {
            p->pSavetimeData->s1ClockDuty_clk_delay_cell[CHANNEL_B][RANK_0] = scFinal_clk_delay_cell;
            p->pSavetimeData->s1ClockDuty_clk_delay_cell[CHANNEL_B][RANK_1] = scFinal_clk_delay_cell;        
        }
#endif  
    }
    else
    {
        //restore gbackup_scFinal_clk_delay_cell, no use LPBK_EN=1's value
        DramcClockDutySetClkDelayCell(p, CHANNEL_B, RANK_0, gbackup_scFinal_clk_delay_cell);
        DramcClockDutySetClkDelayCell(p, CHANNEL_B, RANK_1, gbackup_scFinal_clk_delay_cell);            
    }

        DramcGatingMode(p, 1);

    /* Justin: Enable(set to 1) "RX DQS ISI pulse CG function" after duty-related calibration */
    RxDQSIsiPulseCG(p, ENABLE);

#if FT_DSIM_USED==0
    //restore to orignal value
    DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));
#endif    
    
    DramPhyReset(p);                    

    mcSHOW_ERR_MSG(("[DramcClockDutyScan] ====Done====\n"));        
    /*TINFO="[DramcClockDutyScan] ====Done====\n" */

    return retStatus;
}

#endif


#if ENABLE_DQS_DUTY_SCAN
#if 0  //TODO: maybe no need
void DutyScan_RxReset_seq (DRAMC_CTX_T *p) 
{
    //! for CHA
    //! assert r-data reset
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 1, MISC_CTRL1_R_DMPHYRST);

    //! assert STBEN reset
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B0_DQ3), P_Fld(0, B0_DQ3_RG_ARDQ_RESETB_B0));    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B0_DQ9), P_Fld(0, B0_DQ9_RG_RX_ARDQ_STBEN_RESETB_B0) | P_Fld(0, B0_DQ9_RG_RX_ARDQS_STBEN_RESETB_B0));            
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B1_DQ3), P_Fld(0, B1_DQ3_RG_ARDQ_RESETB_B1));    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B1_DQ9), P_Fld(0, B1_DQ9_RG_RX_ARDQ_STBEN_RESETB_B1) | P_Fld(0, B1_DQ9_RG_RX_ARDQS_STBEN_RESETB_B1));            
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_CA_CMD3), P_Fld(0, CA_CMD3_RG_ARCMD_RESETB));    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_CA_CMD10), P_Fld(0, CA_CMD10_RG_RX_ARCMD_STBEN_RESETB) | P_Fld(0, CA_CMD10_RG_RX_ARCLK_STBEN_RESETB));        

    mcDELAY_US(1) ;

    //! release r-data reset
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 0, MISC_CTRL1_R_DMPHYRST);    

    //! release STBEN reset
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B0_DQ3), P_Fld(1, B0_DQ3_RG_ARDQ_RESETB_B0));    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B0_DQ9), P_Fld(1, B0_DQ9_RG_RX_ARDQ_STBEN_RESETB_B0) | P_Fld(1, B0_DQ9_RG_RX_ARDQS_STBEN_RESETB_B0));            
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B1_DQ3), P_Fld(1, B1_DQ3_RG_ARDQ_RESETB_B1));    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B1_DQ9), P_Fld(1, B1_DQ9_RG_RX_ARDQ_STBEN_RESETB_B1) | P_Fld(1, B1_DQ9_RG_RX_ARDQS_STBEN_RESETB_B1));            
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_CA_CMD3), P_Fld(1, CA_CMD3_RG_ARCMD_RESETB));    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_CA_CMD10), P_Fld(1, CA_CMD10_RG_RX_ARCMD_STBEN_RESETB) | P_Fld(1, CA_CMD10_RG_RX_ARCLK_STBEN_RESETB));    
} //! end-ddrphy_RxReset_seq
#endif

void DutyScan_int_lpbk_test_seq (DRAMC_CTX_T *p, U32 backup_DDRPHY_MISC_EXTLB16, U32 backup_DDRPHY_MISC_EXTLB17, U32 backup_DDRPHY_MISC_EXTLB18)
{
    //! ==============================
    //! - TX setting
    //! ==============================
    //! [21:16]: R_EXTLB_OE_*_ON
#if DUTY_FINETUNE_K_TIME
    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_MISC_EXTLB16), backup_DDRPHY_MISC_EXTLB16 | (0x3f<<16));
#else    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MISC_EXTLB16), P_Fld(1, MISC_EXTLB16_R_EXTLB_OE_DQB0_ON) | P_Fld(1, MISC_EXTLB16_R_EXTLB_OE_DQM0_ON) | P_Fld(1, MISC_EXTLB16_R_EXTLB_OE_DQS0_ON)
                                                              | P_Fld(1, MISC_EXTLB16_R_EXTLB_OE_DQB1_ON) | P_Fld(1, MISC_EXTLB16_R_EXTLB_OE_DQM1_ON) | P_Fld(1, MISC_EXTLB16_R_EXTLB_OE_DQS1_ON));
#endif

    //! [5]: CA_TX_MODE; [4]:DQ_TX_MODE; [3]: DQ_MODE for CA
    //! [9]: CA_RX_MODE; [8]:DQ_RX_MODE; [3]: DQ_MODE for CA
#if DUTY_FINETUNE_K_TIME
    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_MISC_EXTLB18), backup_DDRPHY_MISC_EXTLB18 | ((0x7<<3)|(0x3<<8)));
#else     
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MISC_EXTLB18), P_Fld(1, MISC_EXTLB18_R_LPBK_DQ_MODE_FOR_CA) | P_Fld(1, MISC_EXTLB18_R_LPBK_DQ_TX_MODE) | P_Fld(1, MISC_EXTLB18_R_LPBK_CA_TX_MODE)
                                                              | P_Fld(1, MISC_EXTLB18_R_LPBK_DQ_RX_MODE) | P_Fld(1, MISC_EXTLB18_R_LPBK_CA_RX_MODE));
#endif

    //! ==============================
    //! - APHY setting
    //! ==============================
    //! CA


    //! ==============================
    //! - LPBK setting
    //! ==============================
    //! step-1 : trigger RX SWRST
#if DUTY_FINETUNE_K_TIME
    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_MISC_EXTLB17), backup_DDRPHY_MISC_EXTLB17 | (0x1<<1));
#else     
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_EXTLB17), 1, MISC_EXTLB17_R_EXTLB_RX_SWRST);
    ////vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_EXTLB17), 1, MISC_EXTLB17_R_EXTLB_TX_EN_OTHERCH_SEL);    
#endif    

    //! step-6 : assert reset
    //  *((UINT32P)(Channel_B_PHY_BASE_ADDRESS + 0x0244)) = 0x0000cc03;
    //DutyScan_RxReset_seq(p) ; // duty scan doesn't care data , only DQS count. No need to reset.

    //! step-2 : release RX SWRST
#if DUTY_FINETUNE_K_TIME  
    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_MISC_EXTLB17), backup_DDRPHY_MISC_EXTLB17 & ~((0x1<<1)|(0x1<<3)));
#else     
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MISC_EXTLB17), P_Fld(0, MISC_EXTLB17_R_EXTLB_RX_SWRST) | P_Fld(0, MISC_EXTLB17_R_EXTLB_TX_EN_OTHERCH_SEL));
    ////vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_EXTLB17), 1, MISC_EXTLB17_R_EXTLB_TX_EN_OTHERCH_SEL);        
#endif    

    /*TINFO="[LPBK] Trigger TX Enable ..."*/

    //! step-3 : trigger LPBK TX_EN
#if DUTY_FINETUNE_K_TIME
    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_MISC_EXTLB17), (backup_DDRPHY_MISC_EXTLB17 & ~(0x1<<3)) | (0x1<<2));
#else    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MISC_EXTLB17), P_Fld(1, MISC_EXTLB17_R_EXTLB_TX_EN) | P_Fld(0, MISC_EXTLB17_R_EXTLB_TX_EN_OTHERCH_SEL));    
#endif

    mcDELAY_US(1);

    //! step-4 : wait LPBK done and check result
    //lpbk_test_result = ddrphy_lpbk_chk_result () ;
#if DUTY_FINETUNE_K_TIME   
    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_MISC_EXTLB17), (backup_DDRPHY_MISC_EXTLB17 & ~(0x1<<2)) | (0x1<<3));
#else
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MISC_EXTLB17), P_Fld(0, MISC_EXTLB17_R_EXTLB_TX_EN) | P_Fld(1, MISC_EXTLB17_R_EXTLB_TX_EN_OTHERCH_SEL));    
#endif
}


void DutyScan_inter_loopback_preset(DRAMC_CTX_T *p)
{
    //! enable lpbk CG control
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MISC_CG_CTRL0), P_Fld(1, MISC_CG_CTRL0_W_CHG_MEM) | P_Fld(1, MISC_CG_CTRL0_CLK_MEM_SEL) | P_Fld(1, MISC_CG_CTRL0_RG_CG_RX_CMD_OFF_DISABLE));
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_CG_CTRL3), 0xffffffff, MISC_CG_CTRL3_R_LBK_CG_CTRL);            
    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_CA_DLL_ARPI2), 0x00000000);            

    //! set RG_TX_ARDQ_PULL_DN_B0/B1 to low
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ7), 0, B0_DQ7_RG_TX_ARDQ_PULL_DN_B0);                
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ7), 0, B1_DQ7_RG_TX_ARDQ_PULL_DN_B1);                

    //! enable clock and TX/RX setting of ARCMD/BRCMD
#if 0    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_CA_DLL_ARPI3), P_Fld(1, CA_DLL_ARPI3_RG_ARPI_MCTL_EN_CA)
                                                        | P_Fld(1, CA_DLL_ARPI3_RG_ARPI_FB_EN_CA) 
                                                        | P_Fld(1, CA_DLL_ARPI3_RG_ARPI_CS_EN) 
                                                        | P_Fld(1, CA_DLL_ARPI3_RG_ARPI_CLK_EN) 
                                                        | P_Fld(1, CA_DLL_ARPI3_RG_ARPI_CMD_EN) 
                                                        | P_Fld(1, CA_DLL_ARPI3_RG_ARPI_CLKIEN_EN));
#else
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_CA_DLL_ARPI3), P_Fld(1, CA_DLL_ARPI3_RG_ARPI_MCTL_EN_CA)
                                                    | P_Fld(1, CA_DLL_ARPI3_RG_ARPI_FB_EN_CA) 
                                                    | P_Fld(1, CA_DLL_ARPI3_RG_ARPI_CLK_EN) 
                                                    | P_Fld(1, CA_DLL_ARPI3_RG_ARPI_CLKIEN_EN));
#endif
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_CA_CMD3), P_Fld(1, CA_CMD3_RG_RX_ARCMD_SMT_EN) 
                                                        | P_Fld(1, CA_CMD3_RG_TX_ARCMD_EN) 
                                                        | P_Fld(1, CA_CMD3_RG_ARCMD_RESETB) 
                                                        | P_Fld(1, CA_CMD3_RG_RX_ARCMD_STBENCMP_EN));
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_CA_CMD10), P_Fld(1, CA_CMD10_RG_RX_ARCMD_STBEN_RESETB) 
                                                        | P_Fld(1, CA_CMD10_RG_RX_ARCLK_STBEN_RESETB) 
                                                        | P_Fld(1, CA_CMD10_RG_RX_ARCLK_DQSIENMODE));
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_CA_CMD5), P_Fld(0x10, CA_CMD5_RG_RX_ARCMD_EYE_VREF_SEL) 
                                                        | P_Fld(1, CA_CMD5_RG_RX_ARCMD_VREF_EN) 
                                                        | P_Fld(1, CA_CMD5_RG_RX_ARCMD_VREF_EN) 
                                                        | P_Fld(1, CA_CMD5_RG_RX_ARCMD_EYE_EN)
                                                        | P_Fld(1, CA_CMD5_RG_RX_ARCMD_EYE_STBEN_RESETB));

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_CMD6), 0, CA_CMD6_RG_RX_ARCMD_JM_SEL);                

    //! [2]: XTALK_EN; [1]: SSO_EN; [0]: LFSR_EN
#ifdef LPBK_SSO_OFF
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MISC_EXTLB19), P_Fld(0x10, MISC_EXTLB19_R_EXTLB_LFSR_ENABLE) 
                                                        | P_Fld(0, MISC_EXTLB19_R_EXTLB_SSO_ENABLE) 
                                                        | P_Fld(1, MISC_EXTLB19_R_EXTLB_XTALK_ENABLE));
#else
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MISC_EXTLB19), P_Fld(0x10, MISC_EXTLB19_R_EXTLB_LFSR_ENABLE) 
                                                        | P_Fld(1, MISC_EXTLB19_R_EXTLB_SSO_ENABLE) 
                                                        | P_Fld(1, MISC_EXTLB19_R_EXTLB_XTALK_ENABLE));
#endif

    //! ==============================
    //! - DDRPHY lpbk test flow
    //! ==============================
    //! step-0 : trigger loopback prepare on
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MISC_EXTLB17), P_Fld(1, MISC_EXTLB17_R_EXTLB_TX_PRE_ON) | P_Fld(1, MISC_EXTLB17_R_EXTLB));    


    /*TINFO="[LPBK] RX input delay setting ..."*/
    //ClockDutyScan_SetRXInputDelay_CA( 0, 0 ) ;  // only LP3 init set these values
}

void DQSDutyScan_SetDqsDelayCell(DRAMC_CTX_T *p, U8 u1ChannelIdx, S8 *scDutyDelay)
{
    U8 u1ShuffleIdx, u1DQSIdx, u1RankIdx;
    U32 save_offset;
    U8 u1Delay[2], u1DelayB[2];

    for(u1DQSIdx=0; u1DQSIdx<2; u1DQSIdx++)
    {
        if(scDutyDelay[u1DQSIdx] <0)
        {
            u1Delay[u1DQSIdx]  = -(scDutyDelay[u1DQSIdx]);
            u1DelayB[u1DQSIdx]  =0;
        }
        else
        {
            u1Delay[u1DQSIdx]  = 0;
            u1DelayB[u1DQSIdx]  = scDutyDelay[u1DQSIdx];
        }
    }

    for(u1ShuffleIdx = 0; u1ShuffleIdx<DRAM_DFS_SHUFFLE_MAX; u1ShuffleIdx++)
    {
//#if 1//CLK_DUTY_SCAN_APPLY_RK0_TO_RK1
#if DQS_DUTY_RANK1_TEST
        u1RankIdx = p->rank;
#else
        for(u1RankIdx = 0; u1RankIdx<RANK_MAX; u1RankIdx++)
#endif
        {
            for(u1DQSIdx = 0; u1DQSIdx<2; u1DQSIdx++)
            {
                save_offset = u1ShuffleIdx*0x500 + u1RankIdx*0x100 + u1DQSIdx*0x50;                
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ1) + save_offset, P_Fld(u1Delay[u1DQSIdx], SHU1_R0_B0_DQ1_RK0_TX_ARDQS0_DLY_B0) | P_Fld(u1Delay[u1DQSIdx], SHU1_R0_B0_DQ1_RK0_TX_ARDQS0B_DLY_B0));                                                
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ1) + save_offset, P_Fld(u1DelayB[u1DQSIdx], SHU1_R0_B0_DQ1_RK0_TX_ARDQS0_DLYB_B0) | P_Fld(u1DelayB[u1DQSIdx], SHU1_R0_B0_DQ1_RK0_TX_ARDQS0B_DLYB_B0));                                                            
            }
        }
    }
}

U32 DQSDutyScan_Find_Max_DQS_Cnt(DRAMC_CTX_T *p, U32 backup_DDRPHY_B0_DQ4, U32 backup_DDRPHY_B1_DQ4, U32 backup_DRAMC_REG_EYESCAN, U32 backup_DDRPHY_MISC_EXTLB16, U32 backup_DDRPHY_MISC_EXTLB17, U32 backup_DDRPHY_MISC_EXTLB18)
{
    U8 ucdqs_dly;
    U32 u4sample_cnt, u4ones_cnt[2], Max_Value=0;
    //U32 u4LBDone;

    for (ucdqs_dly=DQS_DLY_START; ucdqs_dly<DQS_DLY_END; ucdqs_dly+=16)
    {
        //Set DQS delay (RG_??_RX_DQS_EYE_DLY)       
#if DUTY_FINETUNE_K_TIME
        backup_DDRPHY_B0_DQ4 &= 0xffff8080;
        backup_DDRPHY_B0_DQ4 |= ((ucdqs_dly<<8) | (ucdqs_dly));
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B0_DQ4), backup_DDRPHY_B0_DQ4); 
        backup_DDRPHY_B1_DQ4 &= 0xffff8080;
        backup_DDRPHY_B1_DQ4 |= ((ucdqs_dly<<8) | (ucdqs_dly));
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B1_DQ4), backup_DDRPHY_B1_DQ4);         
#else
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ4), ucdqs_dly, B0_DQ4_RG_RX_ARDQS_EYE_R_DLY_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ4), ucdqs_dly, B0_DQ4_RG_RX_ARDQS_EYE_F_DLY_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ4), ucdqs_dly, B1_DQ4_RG_RX_ARDQS_EYE_R_DLY_B1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ4), ucdqs_dly, B1_DQ4_RG_RX_ARDQS_EYE_F_DLY_B1);
#endif        
        
        //DramPhyReset(p);
    
        //Reset eye scan counters (reg_sw_rst): 1 to 0
#if DUTY_FINETUNE_K_TIME
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), backup_DRAMC_REG_EYESCAN | 0x1);
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), backup_DRAMC_REG_EYESCAN & ~(0x1));
#else        
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 1, EYESCAN_REG_SW_RST);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 0, EYESCAN_REG_SW_RST);
#endif        
    
        //Enable DQ eye scan (b'1)
#if DUTY_FINETUNE_K_TIME
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), backup_DRAMC_REG_EYESCAN | (1<<1));
#else  
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 1, EYESCAN_RG_RX_EYE_SCAN_EN);
#endif
    
        //inter loop back
        DutyScan_int_lpbk_test_seq(p, backup_DDRPHY_MISC_EXTLB16, backup_DDRPHY_MISC_EXTLB17, backup_DDRPHY_MISC_EXTLB18);

        // according to shih-hsiu, internal loopback no need to wait 1ms to sample 500 samples
        // 2ns/sample, here we delay 1ms about 500 samples
        //mcDELAY_US(1000);
    
        //Disable DQ eye scan (b'1), for counter latch
#if DUTY_FINETUNE_K_TIME
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), backup_DRAMC_REG_EYESCAN & ~(1<<1));
#else    
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 0, EYESCAN_RG_RX_EYE_SCAN_EN);
#endif
    
        //Read the counter values from registers (toggle_cnt*, dqs_err_cnt*);
        u4ones_cnt[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQS0_ERR_CNT), DQS0_ERR_CNT_DQS0_ERR_CNT);
        u4ones_cnt[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQS1_ERR_CNT), DQS1_ERR_CNT_DQS1_ERR_CNT);
        if (u4ones_cnt[0] > Max_Value) Max_Value = u4ones_cnt[0];
        if (u4ones_cnt[1] > Max_Value) Max_Value = u4ones_cnt[1]; 
/*TINFO="u4ones_cnt[0]=%d, u4ones_cnt[1]=%d\n",u4ones_cnt[0], u4ones_cnt[1]  */        	
        
        //u4LBDone = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_STA_EXTLB0), MISC_STA_EXTLB0_STA_EXTLB_DONE);
        //mcSHOW_DBG_MSG(("\n[ClockDutyScan_InternalLoopBack_Find_Max_DQS_Cnt] u4LBDone = 0x%x\n", u4LBDone)); 
    }

    mcSHOW_ERR_MSG(("\n[ClockDutyScan_InternalLoopBack_Find_Max_DQS_Cnt] Max_Value = %d\n", Max_Value)); 
    /*TINFO="\n[ClockDutyScan_InternalLoopBack_Find_Max_DQS_Cnt] Max_Value = %d\n", Max_Value */    

    return Max_Value;
}

#if 0
void DramcDQSDutyScan_InternalLoopBack(DRAMC_CTX_T *p, U8 u1Channel)
{
    S8 scinner_duty_ofst;
    U8 u1Cnt;

    DramcEngine2Init(p, p->test2_1, p->test2_2, p->test_pattern, 0);
    
    for(scinner_duty_ofst=-8; scinner_duty_ofst<=8; scinner_duty_ofst++)
    {   
        mcSHOW_DBG_MSG(("\nscinner_clk_delay_cell %d\n", scinner_duty_ofst));
        if (scinner_duty_ofst<=0)
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ1), P_Fld(0-scinner_duty_ofst, SHU1_R0_B0_DQ1_RK0_TX_ARDQS0_DLY_B0) 
                                                                    | P_Fld(0-scinner_duty_ofst, SHU1_R0_B0_DQ1_RK0_TX_ARDQS0B_DLY_B0) 
                                                                    | P_Fld(0, SHU1_R0_B0_DQ1_RK0_TX_ARDQS0_DLYB_B0) 
                                                                    | P_Fld(0, SHU1_R0_B0_DQ1_RK0_TX_ARDQS0B_DLYB_B0));

            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ1), P_Fld(0-scinner_duty_ofst, SHU1_R0_B1_DQ1_RK0_TX_ARDQS0_DLY_B1) 
                                                                    | P_Fld(0-scinner_duty_ofst, SHU1_R0_B1_DQ1_RK0_TX_ARDQS0B_DLY_B1) 
                                                                    | P_Fld(0, SHU1_R0_B1_DQ1_RK0_TX_ARDQS0_DLYB_B1) 
                                                                    | P_Fld(0, SHU1_R0_B1_DQ1_RK0_TX_ARDQS0B_DLYB_B1));
        }
        else
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ1), P_Fld(0, SHU1_R0_B0_DQ1_RK0_TX_ARDQS0_DLY_B0) 
                                                                    | P_Fld(0, SHU1_R0_B0_DQ1_RK0_TX_ARDQS0B_DLY_B0) 
                                                                    | P_Fld(scinner_duty_ofst, SHU1_R0_B0_DQ1_RK0_TX_ARDQS0_DLYB_B0) 
                                                                    | P_Fld(scinner_duty_ofst, SHU1_R0_B0_DQ1_RK0_TX_ARDQS0B_DLYB_B0));
            
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ1), P_Fld(0, SHU1_R0_B1_DQ1_RK0_TX_ARDQS0_DLY_B1) 
                                                                    | P_Fld(0, SHU1_R0_B1_DQ1_RK0_TX_ARDQS0B_DLY_B1) 
                                                                    | P_Fld(scinner_duty_ofst, SHU1_R0_B0_DQ1_RK0_TX_ARDQS0_DLYB_B0) 
                                                                    | P_Fld(scinner_duty_ofst, SHU1_R0_B0_DQ1_RK0_TX_ARDQS0B_DLYB_B0));
        }

        //for(u1Cnt=0; u1Cnt<10; u1Cnt++)
        {
            DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, p->test_pattern); 
        }
        
        mcDELAY_MS(3000);
    }
    
    DramcEngine2End(p);
}
#endif


#if DQS_DUTY_SLT_CONDITION_TEST
U32 u4DQSDutyDutyDly[CHANNEL_NUM][2]={0};
U32 u4DQSDutyDiff_Max[CHANNEL_NUM][2]={0};
U32 u4DQSDutyDiff_Avrg[CHANNEL_NUM][2]={0};
U32 u4DQSDutyDiff_Min[CHANNEL_NUM][2]={0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff};
U32 u4DQSDutyDiff_Rec[CHANNEL_NUM][2][100]={0};
#endif


S8 gbackup_DQSDutyScan_scFinal_clk_delay_cell[DQS_NUMBER];
DRAM_STATUS_T DramcDQSDutyScan_InternalLoopBack(DRAMC_CTX_T *p, U8 out_of_spec_rescan_flag)
{
    DRAM_STATUS_T retStatus = DRAM_OK;
    U8 u1ByteIdx, ucdqs_i;
    U32 u4RevBX[DQS_NUMBER];
    U8 ucsearch_state, fgcurrent_value, fginitial_value, ucstart_period, ucend_period=0;
    S32 scdqs_dly, s4PICnt, s4PIBegin, s4PIEnd, s4PICnt_mod64;
    #if DUTY_PI_PREDICTION
    S32 s4PIPredict_MaxminPos[2]={0}, s4PIPredict_PosDiff=0;  
    U8 u1PILoopRangeNumber=0, u1PIRangeIdx;
    #endif
    U32 u4sample_cnt, u4ones_cnt2, u4Last_Ones_Cnt=0;
    
    S8 scinner_duty_ofst=0, scFinal_clk_delay_cell[DQS_NUMBER]={0,0};
    S8 scinner_duty_ofst_start, scinner_duty_ofst_end;
    U32 ucperiod_scan_state=CDS_TRASITION_STATE_BEFORE_FIRST ;
    U32 ucperiod, ucduty, ucduty_temp[2]={0,0};
    U32 ucperiod_common=0;
    U32 ucduty_fail_flag[2]={0,0};
    U32 ucperiod_duty; 
    BOOL ucfirst_is_low=0;
    U32 ucperiod_find_flag;
    U32 ucperiod_duty_max=0, ucperiod_duty_min=0xffffffff, ucperiod_duty_max_clk_dly=0, ucperiod_duty_min_clk_dly=0;
    U32 ucperiod_duty_averige=0, ucFinal_period_duty_averige[DQS_NUMBER]={0,0}, ucmost_approach_50_percent=0xffffffff;
    U32 ucFinal_period_duty_max[DQS_NUMBER] = {0,0}, ucFinal_period_duty_min[DQS_NUMBER] = {0,0};
    U32 ucFinal_duty_max_clk_dly[DQS_NUMBER]={0},ucFinal_duty_min_clk_dly[DQS_NUMBER]={0};
    U8 early_break_count=0, ucDQSSwap;
    U32 HighLowSampleCount;
    U8 ucDelay, ucDelayB;    

    S32 sdqsTransitionPos[2][3];
#if DQSDutyFastK
    S32 temp_scdqs_dly;
    BOOL ucprevious_first_is_low[2]={0,0};
    BOOL clockduty_scan_first_init_flag[2]={FALSE, FALSE};
    S32 DutyFastKNextScanStartPosition;
#endif

    U32 u4DutyDiff, u4DutyDiff_Limit=900;
	U8 u1LPBK_EN=0; 
#if FT_DSIM_USED
    U8 ucg_num_dlycell_perT = 90;    
#endif	
    U32 backup_DDRPHY_B0_DQ4;
    U32 backup_DDRPHY_B1_DQ4;
    U32 backup_DRAMC_REG_EYESCAN;
    U32 backup_DDRPHY_MISC_EXTLB16;
    U32 backup_DDRPHY_MISC_EXTLB18;
    U32 backup_DDRPHY_MISC_EXTLB17;

#if FT_DSIM_USED==0
    U32 u4RegBackupAddress[] = {
        DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1),
        DRAMC_REG_ADDR(DRAMC_REG_SHU_CONF0),
        DRAMC_REG_ADDR(DRAMC_REG_EYESCAN),
        DRAMC_REG_ADDR(DRAMC_REG_STBCAL1),
        DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7),DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ7),
        DRAMC_REG_ADDR(DDRPHY_B0_DQ3), DRAMC_REG_ADDR(DDRPHY_B1_DQ3), 
        DRAMC_REG_ADDR(DDRPHY_B0_DQ4), DRAMC_REG_ADDR(DDRPHY_B1_DQ4), 
        DRAMC_REG_ADDR(DDRPHY_B0_DQ5), DRAMC_REG_ADDR(DDRPHY_B1_DQ5), 
        DRAMC_REG_ADDR(DDRPHY_B0_DQ6), DRAMC_REG_ADDR(DDRPHY_B1_DQ6), 
        
        DRAMC_REG_ADDR(DDRPHY_MISC_CG_CTRL0),
        DRAMC_REG_ADDR(DDRPHY_MISC_CG_CTRL3),
        
        DRAMC_REG_ADDR(DDRPHY_B0_DLL_ARPI2),
        DRAMC_REG_ADDR(DDRPHY_B1_DLL_ARPI2),
        DRAMC_REG_ADDR(DDRPHY_CA_DLL_ARPI2),

        DRAMC_REG_ADDR(DDRPHY_B0_DLL_ARPI3),
        DRAMC_REG_ADDR(DDRPHY_B1_DLL_ARPI3),
        DRAMC_REG_ADDR(DDRPHY_CA_DLL_ARPI3),

        DRAMC_REG_ADDR(DDRPHY_CA_CMD2),
        DRAMC_REG_ADDR(DDRPHY_CA_CMD3),
        DRAMC_REG_ADDR(DDRPHY_CA_CMD5),
        DRAMC_REG_ADDR(DDRPHY_CA_CMD6),
        DRAMC_REG_ADDR(DDRPHY_CA_CMD10),
        DRAMC_REG_ADDR(DDRPHY_MISC_EXTLB16),
        DRAMC_REG_ADDR(DDRPHY_MISC_EXTLB17),
        DRAMC_REG_ADDR(DDRPHY_MISC_EXTLB18),
        };
#endif        

    mcSHOW_ERR_MSG(("\n[DramcDQSDutyScan_InternalLoopBack]\n"));    
    /*TINFO="\n[DramcDQSDutyScan_InternalLoopBack]\n" */
#if 0//DQS_DUTY_MEASURE_WITH_WRITE_LEVELING
    DramcClockDutySetClkDelayCell(p, p->channel, RANK_0, -2);
    DramcClockDutySetClkDelayCell(p, p->channel, RANK_1, -2);
    return;
#endif

#if defined(SLT)
    if (out_of_spec_rescan_flag == 1)
    {
        u4DutyDiff_Limit = 585; //570; //550;//600;
        u1LPBK_EN = 1;
    }
    else
#endif
    {
        u4DutyDiff_Limit = 900;    
        u1LPBK_EN = 0;    
    }
    
#if FT_DSIM_USED
#ifdef SET_CLK_DLY 
  	u1LPBK_EN =0;
	/* TINFO="define SET_CLK_DLY, scan clk dly = -8 ==\n" */  	
#else
  	u1LPBK_EN =1;
	/* TINFO="no define SET_CLK_DLY, scan clk dly = 0 ==\n" */  	
#endif       
#endif
    
#if FT_DSIM_USED==0
    vPrintCalibrationBasicInfo(p);
#endif    

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    if(p->pSavetimeData->femmc_Ready==1)
    {
        DQSDutyScan_SetDqsDelayCell(p, p->channel, p->pSavetimeData->s1DQSDuty_clk_delay_cell[p->channel]);
        return DRAM_OK;
    }
#endif

#if FT_DSIM_USED==0
    //backup register value
    DramcBackupRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));
#endif

	        DramcGatingMode(p, 0);

    /* Justin: Disable(set to 0) "RX DQS ISI pulse CG function" during duty-related calibration
     * (must enable(set to 1) before leaving duty calibration)
     */
    RxDQSIsiPulseCG(p, DISABLE);

#if 1 // use original LP4 setting
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 0, MISC_CTRL1_R_DMPINMUX);
#endif
    //SHU_CONF0_FDIV2 should be same as DDRCONF0_DM64BITEN.        
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_CONF0), 1, SHU_CONF0_FDIV2);// DDRCONF0_DM64BITEN
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DLL_ARPI3), 1, B0_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B0);        
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DLL_ARPI3), 1, B1_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B1);    
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DLL_ARPI2), 0,B0_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B0); 
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DLL_ARPI2), 0,B1_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B1); 

    // DQS duty test 1.
    #if 1
    //mcSHOW_ERR_MSG(("\n[RG*RPI*DQ.DQM*_EN*=0]\n"));
    mcSHOW_ERR_MSG(("[DQ/DQM/CA/CS/CLK PI_EN=0]\n"));
    /*TINFO="[DQ/DQM/CA/CS/CLK PI_EN=0]\n" */
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B0_DLL_ARPI3), P_Fld(0, B0_DLL_ARPI3_RG_ARPI_DQ_EN_B0)    
                                        | P_Fld(0, B0_DLL_ARPI3_RG_ARPI_DQM_EN_B0)); 
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B1_DLL_ARPI3), P_Fld(0, B0_DLL_ARPI3_RG_ARPI_DQ_EN_B0)    
                                        | P_Fld(0, B0_DLL_ARPI3_RG_ARPI_DQM_EN_B0));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_CA_DLL_ARPI3), P_Fld(0, CA_DLL_ARPI3_RG_ARPI_CLK_EN)
                                        | P_Fld(0, CA_DLL_ARPI3_RG_ARPI_CS_EN)
                                        | P_Fld(0, CA_DLL_ARPI3_RG_ARPI_CMD_EN)); 
    #endif

    // DQS duty test 2
    #if 1
    #if DUTY_TEST_FOR_YH
    u1LPBK_EN=1; //0 external path, 1:internal path

    // Justin add this setting
    mcSHOW_ERR_MSG(("[TX_ARDQ_SER_MODE_B0/1 = 1]\n"));
    /*TINFO="[TX_ARDQ_SER_MODE_B0/1 = 1]\n" */
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), 1, B0_DQ6_RG_TX_ARDQ_SER_MODE_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), 1, B1_DQ6_RG_TX_ARDQ_SER_MODE_B1);
    #endif
    
    mcSHOW_ERR_MSG(("[RG*LPBK_EN*=%d]\n", u1LPBK_EN));
    /*TINFO="[RG*LPBK_EN*=%d]\n", u1LPBK_EN */
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), u1LPBK_EN, B0_DQ6_RG_RX_ARDQ_LPBK_EN_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), u1LPBK_EN, B1_DQ6_RG_RX_ARDQ_LPBK_EN_B1);
    #endif

    // DQS duty test 3
    #if 1
    //mcSHOW_ERR_MSG(("\n[*PHDET_EN*=0]\n"));
    mcSHOW_ERR_MSG(("[*PI*RESETB*=0  *PHDET_EN*=0  *PI_RESETB*=1]\n"));
    /*TINFO="[*PI*RESETB*=0  *PHDET_EN*=0  *PI_RESETB*=1]\n" */
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DLL_ARPI0), 0x0, B0_DLL_ARPI0_RG_ARPI_RESETB_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DLL_ARPI0), 0x0, B1_DLL_ARPI0_RG_ARPI_RESETB_B1);
 
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DLL_ARPI2), 0x0, B0_DLL_ARPI2_RG_ARDLL_PHDET_EN_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DLL_ARPI2), 0x0, B1_DLL_ARPI2_RG_ARDLL_PHDET_EN_B1);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DLL_ARPI0), 0x1, B0_DLL_ARPI0_RG_ARPI_RESETB_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DLL_ARPI0), 0x1, B1_DLL_ARPI0_RG_ARPI_RESETB_B1);
    #endif

    //DQS duty new
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_CMD2), 0x1, CA_CMD2_RG_TX_ARCMD_OE_DIS);  // disable command output to dram
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_CMD7), 0x1, CA_CMD7_RG_TX_ARCMD_PULL_DN);

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

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B0_DQ5), P_Fld(0x1, B0_DQ5_RG_RX_ARDQ_EYE_EN_B0)    
                                                        | P_Fld(0x1, B0_DQ5_RG_RX_ARDQ_VREF_EN_B0)    
                                                        | P_Fld(0x1, B0_DQ3_RG_RX_ARDQ_SMT_EN_B0)); 

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B1_DQ5), P_Fld(0x1, B1_DQ5_RG_RX_ARDQ_EYE_EN_B1)    
                                                        | P_Fld(0x1, B1_DQ5_RG_RX_ARDQ_VREF_EN_B1)    
                                                        | P_Fld(0x1, B1_DQ3_RG_RX_ARDQ_SMT_EN_B1)); 

    //JM_SEL
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), 0, B0_DQ6_RG_RX_ARDQ_JM_SEL_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), 0, B1_DQ6_RG_RX_ARDQ_JM_SEL_B1);
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_CMD6), 0, CA_CMD6_RG_RX_ARCMD_JM_SEL);   // in test engine


    //Enable MIOCK jitter meter mode ( RG_RX_MIOCK_JIT_EN=1)
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 1, EYESCAN_RG_RX_MIOCK_JIT_EN);

    //Disable DQ eye scan (b'1), for counter clear
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 0, EYESCAN_RG_RX_EYE_SCAN_EN);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_STBCAL1), 0, STBCAL1_DQSERRCNT_DIS);

    DutyScan_inter_loopback_preset(p);

#if DUTY_FINETUNE_K_TIME
    backup_DDRPHY_B0_DQ4 = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_B0_DQ4));
    backup_DDRPHY_B1_DQ4 = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_B1_DQ4));
    backup_DRAMC_REG_EYESCAN = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN));
    backup_DDRPHY_MISC_EXTLB16= u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_EXTLB16));
    backup_DDRPHY_MISC_EXTLB18 = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_EXTLB18));
    backup_DDRPHY_MISC_EXTLB17 = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_EXTLB17));  
#endif

    HighLowSampleCount = DQSDutyScan_Find_Max_DQS_Cnt(p, backup_DDRPHY_B0_DQ4, backup_DDRPHY_B1_DQ4, backup_DRAMC_REG_EYESCAN, backup_DDRPHY_MISC_EXTLB16, backup_DDRPHY_MISC_EXTLB17, backup_DDRPHY_MISC_EXTLB18)>>1; 

#if DQSDutyFastK
    DutyFastKNextScanStartPosition = (CLOCK_PI_STEP*4*ucg_num_dlycell_perT*DUTY_INCREASE_PRECISION)>>6; //predicate next position is CLOCK_PI_STEP*4 (from justin), 1T=64PI
#endif    

#if DQSDutyPeriod_Use_JitterMeters_Period
    ucperiod_find_flag= CDS_PERIOD_STATE_FOUND_COMMON;
    ucperiod_common = p->ucnum_dlycell_perT*DUTY_INCREASE_PRECISION; //period from jitter meter
#else
    ucperiod_find_flag = CDS_PERIOD_STATE_STILL_NOT_FOUND;
#endif        


    #if DUTY_TEST_FOR_YH
    ucdqs_i=1;
    #else
#if FT_DSIM_USED
#ifdef FT_DQS0_DUTY_TEST
    for(ucdqs_i=0; ucdqs_i<=0; ucdqs_i++)
#else
#ifdef FT_DQS1_DUTY_TEST
    for(ucdqs_i=1; ucdqs_i<=1; ucdqs_i++)
#endif    
#endif
#else
    for(ucdqs_i=0; ucdqs_i<2; ucdqs_i++)
#endif
    #endif
    { 
    mcSHOW_ERR_MSG(("\n[DQS %d Duty scan]\n", ucdqs_i));
    /*TINFO="\n[DQS %d Duty scan]\n", ucdqs_i */

        if (out_of_spec_rescan_flag == 0)
        {
#if FT_DSIM_USED
#ifdef SET_CLK_DLY
                scinner_duty_ofst_start=-8; 
                scinner_duty_ofst_end=-8; 
#else
                scinner_duty_ofst_start=0; 
                scinner_duty_ofst_end=0; 
#endif
#else
                scinner_duty_ofst_start=DUTY_OFFSET_START; 
                scinner_duty_ofst_end=DUTY_OFFSET_END; 
#endif        
        }
        else
        {
                scinner_duty_ofst_start=gbackup_DQSDutyScan_scFinal_clk_delay_cell[ucdqs_i]; 
                scinner_duty_ofst_end=gbackup_DQSDutyScan_scFinal_clk_delay_cell[ucdqs_i];     
        }    

    ucmost_approach_50_percent=0xffffffff;
    for(scinner_duty_ofst=scinner_duty_ofst_start; scinner_duty_ofst<=scinner_duty_ofst_end; scinner_duty_ofst++)   
    {   
        if (scinner_duty_ofst<=0)
        {
            ucDelay = -scinner_duty_ofst;
            ucDelayB = 0;
        }
        else
        {
            ucDelay = 0;
            ucDelayB= scinner_duty_ofst;
        }

        if(ucdqs_i==0)
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ1), P_Fld(ucDelay, SHU1_R0_B0_DQ1_RK0_TX_ARDQS0_DLY_B0) 
                                                                | P_Fld(ucDelay, SHU1_R0_B0_DQ1_RK0_TX_ARDQS0B_DLY_B0) 
                                                                | P_Fld(ucDelayB, SHU1_R0_B0_DQ1_RK0_TX_ARDQS0_DLYB_B0) 
                                                                | P_Fld(ucDelayB, SHU1_R0_B0_DQ1_RK0_TX_ARDQS0B_DLYB_B0));
        }
        else
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ1), P_Fld(ucDelay, SHU1_R0_B1_DQ1_RK0_TX_ARDQS0_DLY_B1) 
                                                                | P_Fld(ucDelay, SHU1_R0_B1_DQ1_RK0_TX_ARDQS0B_DLY_B1) 
                                                                | P_Fld(ucDelayB, SHU1_R0_B1_DQ1_RK0_TX_ARDQS0_DLYB_B1) 
                                                                | P_Fld(ucDelayB, SHU1_R0_B1_DQ1_RK0_TX_ARDQS0B_DLYB_B1));
        }

#if (!__ETT__ && !FOR_DV_SIMULATION_USED && !FT_DSIM_USED)
        mtk_wdt_restart();
#endif

INTERNAL_LOOP_BACK_PI_RESCAN:
        ucperiod_duty_max=0;
        ucperiod_duty_min=0xffffffff;

#if DUTY_PI_PREDICTION
        if((scinner_duty_ofst==DUTY_OFFSET_START) || (ucperiod_find_flag == CDS_PERIOD_STATE_STILL_NOT_FOUND)) 
        {
            s4PIBegin = CLOCK_PI_START;
            s4PIEnd = CLOCK_PI_END;
            u1PILoopRangeNumber =1; // full scan
            //mcSHOW_ERR_MSG(("PI Range 1 : %d : %d ~ %d\n", scinner_duty_ofst, s4PIBegin, s4PIEnd));
        }
        else 
        {
            s4PIPredict_PosDiff = s4PIPredict_MaxminPos[1] -s4PIPredict_MaxminPos[0];
            if (s4PIPredict_PosDiff <= DUTY_PI_PREDICTION)
            {  
                s4PIBegin = s4PIPredict_MaxminPos[0] - DUTY_PI_PREDICTION;
                s4PIEnd = s4PIPredict_MaxminPos[1] + DUTY_PI_PREDICTION;
                u1PILoopRangeNumber =1;
            }
            else
            {
                u1PILoopRangeNumber =2;
                //mcSHOW_ERR_MSG(("PI Range 2 : %d : %d ~ %d\n", scinner_duty_ofst, s4PIBegin, s4PIEnd));
            }
        }
#else
        s4PIBegin = CLOCK_PI_START;
        s4PIEnd = CLOCK_PI_END;
#endif

    #if DUTY_PI_PREDICTION
    for(u1PIRangeIdx=0; u1PIRangeIdx < u1PILoopRangeNumber; u1PIRangeIdx++)
    #endif
    {
#if DQSDutyFastK     
            clockduty_scan_first_init_flag[0]= TRUE;    //swap 0
            clockduty_scan_first_init_flag[1]= TRUE;    // swap 1

            sdqsTransitionPos[0][0]=sdqsTransitionPos[0][1]=sdqsTransitionPos[0][2]=DUTY_TANSITION_INVALID;
            sdqsTransitionPos[1][0]=sdqsTransitionPos[1][1]=sdqsTransitionPos[1][2]=DUTY_TANSITION_INVALID;
#endif

            #if DUTY_PI_PREDICTION
            if(u1PILoopRangeNumber==2) // two PI range
            {
                s4PIBegin = s4PIPredict_MaxminPos[u1PIRangeIdx] - DUTY_PI_PREDICTION;
                s4PIEnd = s4PIPredict_MaxminPos[u1PIRangeIdx] + DUTY_PI_PREDICTION;
            }
            #endif

            for(s4PICnt=s4PIBegin; s4PICnt<=s4PIEnd; s4PICnt+=CLOCK_PI_STEP)
            {   
                s4PICnt_mod64 = (s4PICnt+64)&0x3f;//s4PICnt_mod64 = (s4PICnt+64)%64;
                //if(scinner_duty_ofst!=DUTY_OFFSET_START)
                //    mcSHOW_ERR_MSG(("PI= %d\n", s4PICnt_mod64));
                
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), s4PICnt_mod64, SHU1_R0_B0_DQ7_RK0_ARPI_PBYTE_B0); 
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ7), s4PICnt_mod64, SHU1_R0_B1_DQ7_RK0_ARPI_PBYTE_B1);              

#if FT_DSIM_USED
#ifdef SET_CLK_DLY 
            for(ucDQSSwap=0; ucDQSSwap<2; ucDQSSwap++)  //swap begin
#else        
            for(ucDQSSwap=0; ucDQSSwap<=0; ucDQSSwap++)  //don't do swap
#endif
#else
            for(ucDQSSwap=0; ucDQSSwap<2; ucDQSSwap++)  //swap begin
#endif    
            {
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ3), ucDQSSwap, B0_DQ3_RG_RX_ARDQS0_SWAP_EN_B0); 
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ3), ucDQSSwap, B1_DQ3_RG_RX_ARDQS0_SWAP_EN_B1);
          
                ucduty_fail_flag[ucDQSSwap] = 1;
                ucperiod_scan_state=CDS_TRASITION_STATE_BEFORE_FIRST ;

#if DQSDutyPrintAllLog
                //for SLT, use ERR_MSG to force print log
                if (ucperiod_find_flag!=CDS_PERIOD_STATE_STILL_NOT_FOUND)
               {
                    mcSHOW_ERR_MSG(("[DQS PI = %d] |", s4PICnt_mod64));
                    /*TINFO="[DQS PI = %d] |", s4PICnt_mod64 */
                }
#endif           

                for (scdqs_dly=DQS_DLY_START; scdqs_dly<DQS_DLY_END; scdqs_dly+=DQS_DLY_STEP)
                {
#if DQSDutyFastK
                    temp_scdqs_dly=scdqs_dly;
                    if (clockduty_scan_first_init_flag[ucDQSSwap]==FALSE && scdqs_dly==0)
                    {
                        if ((ucperiod_find_flag!=CDS_PERIOD_STATE_STILL_NOT_FOUND) && s4PICnt!=s4PIBegin) 
                        {
                            if (sdqsTransitionPos[ucDQSSwap][0]!=DUTY_TANSITION_INVALID && (((sdqsTransitionPos[ucDQSSwap][0]-DutyFastKNextScanStartPosition)/DUTY_INCREASE_PRECISION) > 0))
                            {
                                //skip scanning from scdqs_dly to sdqsTransitionPos[ucDQSSwap][0]-DutyFastKNextScanStartPosition to reduce calibration time
                                scdqs_dly=(sdqsTransitionPos[ucDQSSwap][0]-DutyFastKNextScanStartPosition)/DUTY_INCREASE_PRECISION;
                            }    
                        }
                    }
#endif
                    //Set DQS delay (RG_??_RX_DQS_EYE_DLY)
                    ///TODO:
                    //if(ucdqs_i==0)  // ??? why ?   
                    {
#if DUTY_FINETUNE_K_TIME
                        backup_DDRPHY_B0_DQ4 &= 0xffff8080;
                        backup_DDRPHY_B0_DQ4 |= ((scdqs_dly<<8) | scdqs_dly);
                        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B0_DQ4), backup_DDRPHY_B0_DQ4); 
#else
                        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B0_DQ4), P_Fld(scdqs_dly, B0_DQ4_RG_RX_ARDQS_EYE_R_DLY_B0) | P_Fld(scdqs_dly, B0_DQ4_RG_RX_ARDQS_EYE_F_DLY_B0));
#endif                        
                    }
                   // else
                    {
#if DUTY_FINETUNE_K_TIME
                        backup_DDRPHY_B1_DQ4 &= 0xffff8080;
                        backup_DDRPHY_B1_DQ4 |= ((scdqs_dly<<8) | scdqs_dly);
                        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B1_DQ4), backup_DDRPHY_B1_DQ4); 
#else                    
                        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B1_DQ4), P_Fld(scdqs_dly, B1_DQ4_RG_RX_ARDQS_EYE_R_DLY_B1) | P_Fld(scdqs_dly, B1_DQ4_RG_RX_ARDQS_EYE_F_DLY_B1));                    
#endif
                    }
        
                    //DramPhyReset(p);  ///TODO: to be remove
        
                    //Reset eye scan counters (reg_sw_rst): 1 to 0
#if DUTY_FINETUNE_K_TIME
                    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), backup_DRAMC_REG_EYESCAN | 0x1);
                    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), backup_DRAMC_REG_EYESCAN & ~(0x1));
#else                    
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 1, EYESCAN_REG_SW_RST);
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 0, EYESCAN_REG_SW_RST);
#endif                    
        
                    //Enable DQ eye scan (b'1)
#if DUTY_FINETUNE_K_TIME
                    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), backup_DRAMC_REG_EYESCAN | (1<<1));
#else 
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 1, EYESCAN_RG_RX_EYE_SCAN_EN);
#endif
        
                    //inter loop back
                    DutyScan_int_lpbk_test_seq(p, backup_DDRPHY_MISC_EXTLB16, backup_DDRPHY_MISC_EXTLB17, backup_DDRPHY_MISC_EXTLB18);

                    // according to shih-hsiu, internal loopback no need to wait 1ms to sample 500 samples
                    // 2ns/sample, here we delay 1ms about 500 samples
                    //mcDELAY_US(1000);
        
                    //Disable DQ eye scan (b'1), for counter latch
#if DUTY_FINETUNE_K_TIME
                    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), backup_DRAMC_REG_EYESCAN & ~(1<<1));
#else 
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_EYESCAN), 0, EYESCAN_RG_RX_EYE_SCAN_EN);
#endif
        
                    //Read the counter values from registers (toggle_cnt*, dqs_err_cnt*);

                    if(ucdqs_i==0)
                    {
                        u4ones_cnt2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQS0_ERR_CNT), DQS0_ERR_CNT_DQS0_ERR_CNT);
                    }
                    else //dqs1
                    {
                        u4ones_cnt2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQS1_ERR_CNT), DQS1_ERR_CNT_DQS1_ERR_CNT);
                    }
#if DQSDutyPrintAllLog     
#if DQSDutyPrintAllLog_Type
                    //for SLT, use ERR_MSG to force print log
                    if (ucperiod_find_flag!=CDS_PERIOD_STATE_STILL_NOT_FOUND)
                    {
                        if (u4ones_cnt2 >= HighLowSampleCount) 
                        {
                            mcSHOW_ERR_MSG(("O"));
                            /*TINFO="O" */
                        }
                        else 
                        {
                            mcSHOW_ERR_MSG(("_"));
                            /*TINFO="_" */
                        }
                    }
#else
            #ifdef ETT_PRINT_FORMAT
                        //for SLT, use ERR_MSG to force print log
                        mcSHOW_ERR_MSG(("[Dqs PI = %d] %d : %d\n", s4PICnt_mod64, scdqs_dly, u4ones_cnt2));
                        /*TINFO="[Dqs PI = %d] %d : %d\n", s4PICnt_mod64, scdqs_dly, u4ones_cnt2 */
            #else
                        mcSHOW_DBG_MSG(("[Dqs PI = %d] %3d : %8d\n", s4PICnt_mod64, scdqs_dly, u4ones_cnt2));
                        /*TINFO="[Dqs PI = %d] %3d : %8d\n", s4PICnt_mod64, scdqs_dly, u4ones_cnt2 */
            #endif
#endif
#endif

#if DQSDutyFastK       
                    if(temp_scdqs_dly==0)  
                    {
                        if (u4ones_cnt2 <= HighLowSampleCount) ucfirst_is_low = TRUE;
                        else ucfirst_is_low = FALSE;
                       
                        if (ucperiod_find_flag!=CDS_PERIOD_STATE_STILL_NOT_FOUND)
                        {
                            if (clockduty_scan_first_init_flag[ucDQSSwap]==FALSE)
                            {
                                if (ucprevious_first_is_low[ucDQSSwap] != ucfirst_is_low)
                                {
                                    ucprevious_first_is_low[ucDQSSwap] = ucfirst_is_low;
                                    sdqsTransitionPos[ucDQSSwap][0]=sdqsTransitionPos[ucDQSSwap][1]=sdqsTransitionPos[ucDQSSwap][2]=DUTY_TANSITION_INVALID;  ///TODO: te be remove
                                    scdqs_dly=0;
                                    clockduty_scan_first_init_flag[ucDQSSwap]=TRUE;
                                    u4Last_Ones_Cnt = u4ones_cnt2;
                                    continue;
                                }
                            }
                            ucprevious_first_is_low[ucDQSSwap] = ucfirst_is_low;
                        }
                    }
#else                
                    if (scdqs_dly == 0)
                    {
                        if (u4ones_cnt2 <= HighLowSampleCount) ucfirst_is_low = TRUE;
                        else ucfirst_is_low = FALSE;
                    }
#endif

                    switch(ucperiod_scan_state)
                    {
                        case CDS_TRASITION_STATE_BEFORE_FIRST :
                            //if (u4ones_cnt2 >= HighLowSampleCount)
                            if(((ucfirst_is_low==TRUE)&&(u4ones_cnt2 >= HighLowSampleCount)) ||
                               ((ucfirst_is_low==FALSE)&&(u4ones_cnt2 < HighLowSampleCount)))
                            {
                                sdqsTransitionPos[ucDQSSwap][0] = u4TrasitionInterpolation(u4ones_cnt2, u4Last_Ones_Cnt,HighLowSampleCount, scdqs_dly, scdqs_dly-DQS_DLY_STEP);
                                #if DQSDutyPrintAllLog
                                mcSHOW_ERR_MSG(("sdqsTransitionPos[%d][0] = %d\n", ucDQSSwap, sdqsTransitionPos[ucDQSSwap][0]));
                                /*TINFO="sdqsTransitionPos[%d][0] = %d\n", ucDQSSwap, sdqsTransitionPos[ucDQSSwap][0] */
                                #endif
                                //sdqsTransitionPos[ucDQSSwap][0] = scdqs_dly;  
                                ucperiod_scan_state = CDS_TRASITION_STATE_FOUND_FIRST ;
#if DQSDutyFastK                                
                                temp_scdqs_dly = scdqs_dly;                                
                                if (ucperiod_find_flag!=CDS_PERIOD_STATE_STILL_NOT_FOUND) 
                                {
                                    if (clockduty_scan_first_init_flag[ucDQSSwap]==FALSE && sdqsTransitionPos[ucDQSSwap][1]!=DUTY_TANSITION_INVALID) 
                                    {
                                        //skip scanning from scdqs_dly to sdqsTransitionPos[ucDQSSwap][1]-DutyFastKNextScanStartPosition to reduce calibration time
                                        scdqs_dly=(sdqsTransitionPos[ucDQSSwap][1]-DutyFastKNextScanStartPosition)/DUTY_INCREASE_PRECISION;
                                    }    
                                }
#endif
                            }
                            break;
                            
                        case CDS_TRASITION_STATE_FOUND_FIRST :
                            //if (u4ones_cnt2 < HighLowSampleCount)
                            if(((ucfirst_is_low==TRUE)&&(u4ones_cnt2 < HighLowSampleCount)) ||
                               ((ucfirst_is_low==FALSE)&&(u4ones_cnt2 >= HighLowSampleCount)))
                            {
                            
                                sdqsTransitionPos[ucDQSSwap][1] = u4TrasitionInterpolation(u4ones_cnt2, u4Last_Ones_Cnt,HighLowSampleCount, scdqs_dly, scdqs_dly-DQS_DLY_STEP);
                                #if DQSDutyPrintAllLog
                                mcSHOW_ERR_MSG(("sdqsTransitionPos[%d][1] = %d \n", ucDQSSwap, sdqsTransitionPos[ucDQSSwap][1]));
                                /*TINFO="sdqsTransitionPos[%d][1] = %d \n", ucDQSSwap, sdqsTransitionPos[ucDQSSwap][1] */
                                #endif
                                //sdqsTransitionPos[ucDQSSwap][1] = scdqs_dly;
#if DQSDutyPeriod_Use_JitterMeters_Period
                                ucperiod_scan_state = CDS_TRASITION_STATE_END;
#else
                                ucperiod_scan_state = CDS_TRASITION_STATE_FOUND_SECOND ;
#endif

#if DQSDutyFastK                                
                                temp_scdqs_dly = scdqs_dly;                                
                                if (ucperiod_find_flag!=CDS_PERIOD_STATE_STILL_NOT_FOUND) 
                                {   
                                    if (clockduty_scan_first_init_flag[ucDQSSwap]==FALSE)
                                    {
                                        if (sdqsTransitionPos[ucDQSSwap][2]!=DUTY_TANSITION_INVALID) 
                                        {
                                            //skip scanning from scdqs_dly to sdqsTransitionPos[ucDQSSwap][2]-DutyFastKNextScanStartPosition to reduce calibration time
                                            scdqs_dly=(sdqsTransitionPos[ucDQSSwap][2]-DutyFastKNextScanStartPosition)/DUTY_INCREASE_PRECISION;
                                        }                
                                        else 
                                        {
                                            scdqs_dly=DQS_DLY_END-DutyFastKNextScanStartPosition/DUTY_INCREASE_PRECISION;
                                        }
                                    }    
                                }
#endif
#if DQSDutyPeriod_Use_JitterMeters_Period
#if DQSDutyFastK
                                if (clockduty_scan_first_init_flag[ucDQSSwap]==FALSE) 
                                {
                                    //skip left calibration
                                    scdqs_dly=DQS_DLY_END;
                                }  
#endif
#endif
                            }
                            
                            break;
                            
                            case CDS_TRASITION_STATE_FOUND_SECOND :
                                //if (u4ones_cnt2 >= HighLowSampleCount)
                                if(((ucfirst_is_low==TRUE)&&(u4ones_cnt2 >= HighLowSampleCount)) ||
                                   ((ucfirst_is_low==FALSE)&&(u4ones_cnt2 < HighLowSampleCount)))                               
                                {                            
                                    sdqsTransitionPos[ucDQSSwap][2] = u4TrasitionInterpolation(u4ones_cnt2, u4Last_Ones_Cnt,HighLowSampleCount, scdqs_dly, scdqs_dly-DQS_DLY_STEP);
                                    #if DQSDutyPrintAllLog
                                    mcSHOW_ERR_MSG(("sdqsTransitionPos[%d][2] = %d\n", ucDQSSwap, sdqsTransitionPos[ucDQSSwap][2]));
                                    /*TINFO="sdqsTransitionPos[%d][2] = %d\n", ucDQSSwap, sdqsTransitionPos[ucDQSSwap][2] */
                                    #endif
                                    //sdqsTransitionPos[ucDQSSwap][2] = scdqs_dly;
                                    ucperiod_scan_state = CDS_TRASITION_STATE_FOUND_THIRD ;
#if DQSDutyFastK                                
                                    if (ucperiod_find_flag!=CDS_PERIOD_STATE_STILL_NOT_FOUND) 
                                    {
                                        if (clockduty_scan_first_init_flag[ucDQSSwap]==FALSE) 
                                        {
                                            //skip left calibration
                                            scdqs_dly=DQS_DLY_END;
                                        }    
                                    }
#endif
                                    if (ucperiod_find_flag == CDS_PERIOD_STATE_STILL_NOT_FOUND) ucperiod_find_flag = CDS_PERIOD_STATE_FOUND;
                                }     
                            break;
                        }
                    u4Last_Ones_Cnt = u4ones_cnt2;
                }

#if DQSDutyPrintAllLog
#if DQSDutyPrintAllLog_Type
                //for SLT, use ERR_MSG to force print log
                if (ucperiod_find_flag!=CDS_PERIOD_STATE_STILL_NOT_FOUND) 
               {
                	mcSHOW_ERR_MSG(("\n"));
                	/*TINFO="\n" */
                }
#endif            
#endif  
#if DQSDutyFastK
                if ((ucperiod_find_flag!=CDS_PERIOD_STATE_STILL_NOT_FOUND) && clockduty_scan_first_init_flag[ucDQSSwap]==TRUE)
                {
                    clockduty_scan_first_init_flag[ucDQSSwap]=FALSE;
                }    
#endif

#if DQSDutyPeriod_Use_JitterMeters_Period
                ucperiod = ucperiod_common;
#else
                if (ucperiod_find_flag == CDS_PERIOD_STATE_FOUND)
                {
                    ucperiod_find_flag = CDS_PERIOD_STATE_FOUND_COMMON;

                    #if 0
                    if (ucfirst_is_low==TRUE)
                    {
                        ucperiod_common = ucperiod_third_low_2_high - ucperiod_first_low_2_high;
                    }
                        else
                    {
                        ucperiod_common = ucperiod_third_high_2_low - ucperiod_first_high_2_low;                
                    }
                    #else
                    ucperiod_common = sdqsTransitionPos[ucDQSSwap][2] - sdqsTransitionPos[ucDQSSwap][0];
                    #endif

                    goto INTERNAL_LOOP_BACK_PI_RESCAN;
                }
                else if (ucperiod_find_flag == CDS_PERIOD_STATE_FOUND_COMMON)
                {                
                    if (ucperiod_scan_state == CDS_TRASITION_STATE_FOUND_THIRD)
                    {
                        #if 0
                        if (ucfirst_is_low==TRUE)
                        {
                            ucperiod = ucperiod_third_low_2_high - ucperiod_first_low_2_high;
                        }
                            else
                        {
                            ucperiod = ucperiod_third_high_2_low - ucperiod_first_high_2_low;                
                        }         
                        #else
                        ucperiod = sdqsTransitionPos[ucDQSSwap][2] - sdqsTransitionPos[ucDQSSwap][0];
                        ucperiod_common= ucperiod; // yulia change
                        #endif
                    }
                    else
                    {
                        ucperiod = ucperiod_common;
                    }
                }
#endif   

                //ucperiod_find_flag_temp[ucDQSSwap] = ucperiod_find_flag;

                if (ucperiod_find_flag == CDS_PERIOD_STATE_FOUND_COMMON)
                {        
                    #if 0
                    if (ucfirst_is_low==TRUE)
                    {
                        ucperiod_duty = ucperiod_second_high_2_low - ucperiod_first_low_2_high;
                        ucduty_temp[ucDQSSwap] = (ucperiod_duty*10000)/ucperiod;                
                    }
                    else
                    {
                        ucperiod_duty = ucperiod_second_low_2_high - ucperiod_first_high_2_low;                
                        ucduty_temp[ucDQSSwap] = 10000 - (ucperiod_duty*10000)/ucperiod;                                
                    }
                    #else
                    
                    ucperiod_duty = (U32)(sdqsTransitionPos[ucDQSSwap][1] - sdqsTransitionPos[ucDQSSwap][0]);

                    ///TODO: Need to debug!!!!ucperiod is 0, transition is wrong.
                    if(ucperiod !=0)  // div0 protection  
                    {
                        if (ucfirst_is_low==TRUE)
                        {
                            ucduty_temp[ucDQSSwap] = (ucperiod_duty*10000)/ucperiod;                
                        }
                        else
                        {
                            ucduty_temp[ucDQSSwap] = 10000 - (ucperiod_duty*10000)/ucperiod;                                
                        }
                        
                        ucduty_fail_flag[ucDQSSwap] =0; //duty valid
                    }
                    else
                    {
                        ucduty_temp[ucDQSSwap] =0;
                        mcSHOW_ERR_MSG(("[Inner Clk Delay Cell = %d][Clk PI = %d]\n",scinner_duty_ofst, s4PICnt_mod64));
                        /*TINFO="[Inner Clk Delay Cell = %d][Clk PI = %d]\n",scinner_duty_ofst, s4PICnt_mod64 */
                        #if __ETT__
                        mcSHOW_ERR_MSG(("\nDQS period is 0!!! cannot div 0\n"));
                        while(1);
                        #endif
                    }
                    #endif

                    //mcSHOW_ERR_MSG(("sdqsTransitionPos[%d][0] = %d\n", ucDQSSwap, sdqsTransitionPos[ucDQSSwap][0]));
                    //mcSHOW_ERR_MSG(("sdqsTransitionPos[%d][1] = %d\n", ucDQSSwap, sdqsTransitionPos[ucDQSSwap][1]));
                    //if (ucperiod_scan_state == CDS_TRASITION_STATE_FOUND_THIRD ) mcSHOW_ERR_MSG(("sdqsTransitionPos[%d][2] = %d\n", ucDQSSwap, sdqsTransitionPos[ucDQSSwap][2]));
                
#if DQSDutyPrintAllLog            
                    //for SLT, use ERR_MSG to force print log
                    mcSHOW_ERR_MSG(("[Inner Clk Delay Cell = %d][Clk PI = %d]\n",scinner_duty_ofst, s4PICnt_mod64));
                    /*TINFO="[Inner Clk Delay Cell = %d][Clk PI = %d]\n",scinner_duty_ofst, s4PICnt_mod64 */
                    #if 0
                    if (ucfirst_is_low==TRUE)
                    {
                        mcSHOW_ERR_MSG(("Francis ucperiod=%d(%d ~ %d)\n",ucperiod, ucperiod_first_low_2_high, ucperiod_third_low_2_high));
                        mcSHOW_ERR_MSG(("Francis duty_high=%d(%d ~ %d)\n",ucperiod_duty, ucperiod_first_low_2_high, ucperiod_second_high_2_low));
                    }
                    else
                    {
                        mcSHOW_ERR_MSG(("Francis ucperiod=%d(%d ~ %d)\n",ucperiod, ucperiod_first_high_2_low, ucperiod_third_high_2_low));
                        mcSHOW_ERR_MSG(("Francis duty_low=%d(%d ~ %d)\n",ucperiod_duty, ucperiod_first_high_2_low, ucperiod_second_low_2_high));
                    }
                    #else
                    mcSHOW_ERR_MSG(("Francis ucperiod=%d(%d ~ %d)\n",ucperiod, sdqsTransitionPos[ucDQSSwap][0], sdqsTransitionPos[ucDQSSwap][2]));
                    /*TINFO="Francis ucperiod=%d(%d ~ %d)\n",ucperiod, sdqsTransitionPos[ucDQSSwap][0], sdqsTransitionPos[ucDQSSwap][2] */
                    if (ucfirst_is_low==TRUE)
                    {
                        mcSHOW_ERR_MSG(("Francis duty_high=%d(%d ~ %d)\n",ucperiod_duty, sdqsTransitionPos[ucDQSSwap][0], sdqsTransitionPos[ucDQSSwap][1]));
                        /*TINFO="Francis duty_high=%d(%d ~ %d)\n",ucperiod_duty, sdqsTransitionPos[ucDQSSwap][0], sdqsTransitionPos[ucDQSSwap][1] */
                    }
                    else
                    {   
                        mcSHOW_ERR_MSG(("Francis duty_low=%d(%d ~ %d)\n",ucperiod_duty, sdqsTransitionPos[ucDQSSwap][0], sdqsTransitionPos[ucDQSSwap][1]));
                        /*TINFO="Francis duty_low=%d(%d ~ %d)\n",ucperiod_duty, sdqsTransitionPos[ucDQSSwap][0], sdqsTransitionPos[ucDQSSwap][1] */
                    }
                    #endif
                    mcSHOW_ERR_MSG(("Francis clock Duty temp(Swap%d)= %d%%(X100)\n",ucDQSSwap, ucduty_temp[ucDQSSwap]));
                    /*TINFO="Francis clock Duty temp(Swap%d)= %d%%(X100)\n",ucDQSSwap, ucduty_temp[ucDQSSwap] */
#endif                    
                    }            
                }// end of swap

                if ((ucduty_fail_flag[0] == 0) &&(ucduty_fail_flag[1] == 0))  //duty valid for both swap 0 and 1
                {        
#if FT_DSIM_USED
#ifdef SET_CLK_DLY 
                    ucduty = (ucduty_temp[0]+ (10000-ucduty_temp[1])) >>1;
#else        
                    ucduty = ucduty_temp[0];
#endif
#else
                    ucduty = (ucduty_temp[0]+ (10000-ucduty_temp[1])) >>1;
#endif
                    
#if DQSDutyPrintAllLog
                    mcSHOW_DBG_MSG(("Swap0 Duty %d, Swap1 Duty %d, Francis clock Duty = %d%%(X100)\n", ucduty_temp[0], ucduty_temp[1], ucduty));
                    /*TINFO="Swap0 Duty %d, Swap1 Duty %d, Francis clock Duty = %d%%(X100)\n", ucduty_temp[0], ucduty_temp[1], ucduty */
#endif

                    if (ucperiod_duty_max < ucduty)
                    {
                        ucperiod_duty_max = ucduty;
                        ucperiod_duty_max_clk_dly = s4PICnt_mod64;
                    }
                    
                    if (ucperiod_duty_min > ucduty)
                    {
                        ucperiod_duty_min = ucduty;
                        ucperiod_duty_min_clk_dly = s4PICnt_mod64;
                    }
                }

#if DQSDutyFastK            
                if (ucperiod_find_flag == CDS_PERIOD_STATE_STILL_NOT_FOUND) 
                    s4PICnt += 15; //to find period quickly
#endif

            }
    }// end of u1PIRangeIdx

//        if (ucduty_fail_flag == 0)
        {
//            mcSHOW_DBG_MSG(("\n[%d] Clock Duty Pass !!!!!\n", scinner_duty_ofst));
            #if DUTY_PI_PREDICTION
            if(scinner_duty_ofst==DUTY_OFFSET_START)
            {
                // make s4PIPredict_MaxminPos[0] < s4PIPredict_MaxminPos[1]
                if(ucperiod_duty_max_clk_dly < ucperiod_duty_min_clk_dly)
                {
                    s4PIPredict_MaxminPos[0] = ucperiod_duty_max_clk_dly;
                    s4PIPredict_MaxminPos[1] = ucperiod_duty_min_clk_dly;
                }
                else
                {              
                    s4PIPredict_MaxminPos[0] = ucperiod_duty_min_clk_dly;
                    s4PIPredict_MaxminPos[1] = ucperiod_duty_max_clk_dly;
                }
            }
            #endif
            ucperiod_duty_averige = (ucperiod_duty_max + ucperiod_duty_min)/2;

            //for SLT, use ERR_MSG to force print log
            mcSHOW_ERR_MSG(("[%d] MAX Duty = %d%%(X100), DQS PI=%d\n",scinner_duty_ofst, ucperiod_duty_max, ucperiod_duty_max_clk_dly));
            /*TINFO="[%d] MAX Duty = %d%%(X100), DQS PI=%d\n",scinner_duty_ofst, ucperiod_duty_max, ucperiod_duty_max_clk_dly */
            mcSHOW_ERR_MSG(("[%d] MIN Duty = %d%%(X100), DQS PI=%d\n",scinner_duty_ofst, ucperiod_duty_min, ucperiod_duty_min_clk_dly));
            /*TINFO="[%d] MIN Duty = %d%%(X100), DQS PI=%d\n",scinner_duty_ofst, ucperiod_duty_min, ucperiod_duty_min_clk_dly */
            mcSHOW_ERR_MSG(("[%d] AVG Duty = %d%%(X100)\n", scinner_duty_ofst, ucperiod_duty_averige));        
            /*TINFO="[%d] AVG Duty = %d%%(X100)\n", scinner_duty_ofst, ucperiod_duty_averige */

            if(ucperiod_duty_averige >= ClockDutyMiddleBound)
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
                    mcSHOW_ERR_MSG(("!!! ucmost_approach_50_percent = %d%%(X100) !!!\n",ucmost_approach_50_percent));
                    /*TINFO="!!! ucmost_approach_50_percent = %d%%(X100) !!!\n",ucmost_approach_50_percent */
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
                    mcSHOW_ERR_MSG(("!!! ucmost_approach_50_percent = %d%%(X100) !!!\n",ucmost_approach_50_percent));                    
                    /*TINFO="!!! ucmost_approach_50_percent = %d%%(X100) !!!\n",ucmost_approach_50_percent */
                    early_break_count = 0;
                }            
                else
                {
                    early_break_count ++;
                    if (early_break_count>=2) break; //early break;
                }                
            }            
        }
    }
    }


    for(ucdqs_i=0; ucdqs_i<2; ucdqs_i++)
    {
        //for SLT, use ERR_MSG to force print log
        mcSHOW_ERR_MSG(("\n==DQS %d ==\n", ucdqs_i));
        /*TINFO="\n==DQS %d ==\n", ucdqs_i */
        mcSHOW_ERR_MSG(("Final DQS duty delay cell = %d\n", scFinal_clk_delay_cell[ucdqs_i]));    
        /*TINFO="Final DQS duty delay cell = %d\n", scFinal_clk_delay_cell[ucdqs_i] */
        mcSHOW_ERR_MSG(("[%d] MAX Duty = %d%%(X100), DQS PI = %d\n",scFinal_clk_delay_cell[ucdqs_i], ucFinal_period_duty_max[ucdqs_i], ucFinal_duty_max_clk_dly[ucdqs_i]));
        /*TINFO="[%d] MAX Duty = %d%%(X100), DQS PI = %d\n",scFinal_clk_delay_cell[ucdqs_i], ucFinal_period_duty_max[ucdqs_i], ucFinal_duty_max_clk_dly[ucdqs_i] */
        mcSHOW_ERR_MSG(("[%d] MIN Duty = %d%%(X100), DQS PI = %d\n",scFinal_clk_delay_cell[ucdqs_i], ucFinal_period_duty_min[ucdqs_i], ucFinal_duty_min_clk_dly[ucdqs_i]));
        /*TINFO="[%d] MIN Duty = %d%%(X100), DQS PI = %d\n",scFinal_clk_delay_cell[ucdqs_i], ucFinal_period_duty_min[ucdqs_i], ucFinal_duty_min_clk_dly[ucdqs_i] */
        mcSHOW_ERR_MSG(("[%d] AVG Duty = %d%%(X100)\n", scFinal_clk_delay_cell[ucdqs_i], ucFinal_period_duty_averige[ucdqs_i]));
        /*TINFO="[%d] AVG Duty = %d%%(X100)\n", scFinal_clk_delay_cell[ucdqs_i], ucFinal_period_duty_averige[ucdqs_i] */
    }
    
#if FT_DSIM_USED
		FT_DQSDuty_Compare_PassFail(p->channel, ucFinal_period_duty_max[0] , ucFinal_period_duty_min[0],ucFinal_period_duty_max[1] , ucFinal_period_duty_min[1]);
#else		           
    for(ucdqs_i=0; ucdqs_i<2; ucdqs_i++)
    {
        u4DutyDiff = ucFinal_period_duty_max[ucdqs_i] - ucFinal_period_duty_min[ucdqs_i];

#if DQS_DUTY_SLT_CONDITION_TEST
        u4DQSDutyDiff_Rec[p->channel][ucdqs_i][u1GlobalTestCnt]=u4DutyDiff;

        u4DQSDutyDutyDly[p->channel][ucdqs_i] = scFinal_clk_delay_cell[ucdqs_i];

        if(u4DutyDiff > u4DQSDutyDiff_Max[p->channel][ucdqs_i])
            u4DQSDutyDiff_Max[p->channel][ucdqs_i] = u4DutyDiff;

        if(u4DutyDiff < u4DQSDutyDiff_Min[p->channel][ucdqs_i])
            u4DQSDutyDiff_Min[p->channel][ucdqs_i] = u4DutyDiff;

        u4DQSDutyDiff_Avrg[p->channel][ucdqs_i]  += u4DutyDiff;
#endif
#if defined(SLT)
				if ((u4DutyDiff < u4DutyDiff_Limit) &&
        ((ucFinal_period_duty_averige[ucdqs_i] >= 4700 && ucFinal_period_duty_averige[ucdqs_i] <= 5300 && out_of_spec_rescan_flag==0) || (out_of_spec_rescan_flag==1)))
#else
        if ((u4DutyDiff < u4DutyDiff_Limit) &&
        (ucFinal_period_duty_averige[ucdqs_i] >= 4700 && ucFinal_period_duty_averige[ucdqs_i] <= 5300))
#endif
        {
            mcSHOW_ERR_MSG(("\nCH%d DQS %d Duty spec in!! Max-Min= %d%%\n",p->channel, ucdqs_i, u4DutyDiff));            
            /*TINFO="\nCH%d DQS %d Duty spec in!! Max-Min= %d%%\n",p->channel, ucdqs_i, u4DutyDiff */
        }
        else
        {
            mcSHOW_ERR_MSG(("\nCH%d DQS %d Duty spec out!! Max-Min= %d%% >%d%%\n", p->channel, ucdqs_i, u4DutyDiff, u4DutyDiff_Limit));
            /*TINFO="\nCH%d DQS %d Duty spec out!! Max-Min= %d%% >8%%\n", p->channel, ucdqs_i, u4DutyDiff */

            #if defined(SLT)
                if (out_of_spec_rescan_flag==1)  //only when LPBKEN=1 fail, then while(1);
                {
                while(1); //stop here 
                }
            #endif

            #if __ETT__
                
                #if DQS_DUTY_SLT_CONDITION_TEST
                retStatus = DRAM_FAIL;
                #else
                if (out_of_spec_rescan_flag==1)  //only when LPBKEN=1 fail, then while(1);
                {
                while(1); //stop here 
                }
                #endif
            
            #endif
        }    
    }
#endif    

	        DramcGatingMode(p, 1);

    /* Justin: Enable(set to 1) "RX DQS ISI pulse CG function" after duty-related calibration */
    RxDQSIsiPulseCG(p, ENABLE);

#if !FT_DSIM_USED
    //restore to orignal value
    DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));
#endif    


    if (out_of_spec_rescan_flag==0)
    {
        gbackup_DQSDutyScan_scFinal_clk_delay_cell[0] = scFinal_clk_delay_cell[0];
        gbackup_DQSDutyScan_scFinal_clk_delay_cell[1] = scFinal_clk_delay_cell[1];        

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(p->pSavetimeData->femmc_Ready==0)
        {
            p->pSavetimeData->s1DQSDuty_clk_delay_cell[p->channel][0] = scFinal_clk_delay_cell[0];
            p->pSavetimeData->s1DQSDuty_clk_delay_cell[p->channel][1] = scFinal_clk_delay_cell[1];        
        }
#endif 
        DQSDutyScan_SetDqsDelayCell(p, p->channel, scFinal_clk_delay_cell);          
    }
    else
    {
        DQSDutyScan_SetDqsDelayCell(p, p->channel, gbackup_DQSDutyScan_scFinal_clk_delay_cell);    
    }

    DramPhyReset(p);        
            
    mcSHOW_ERR_MSG(("[DQSDutyScan] ====Done====\n"));    
    /*TINFO="[DQSDutyScan] ====Done====\n" */

    return retStatus;
}

#endif

#if (ENABLE_CLOCK_DUTY_SCAN || ENABLE_DQS_DUTY_SCAN)
#ifndef LOOPBACK_TEST 
void DramcDutyCalibration_LPBK(DRAMC_CTX_T *p)
{
    DRAM_STATUS_T retStatus;
#if ENABLE_DQS_DUTY_SCAN
#if DQS_DUTY_SLT_CONDITION_TEST
        U16 u2TestCnt, u2FailCnt=0, u2TestCntTotal =100;
        U8 u1ByteIdx, u1PI_FB;
        U32 u4Variance;
        
#endif
        DRAM_STATUS_T u2FailStatusByCh[CHANNEL_NUM];
        U8 u1ChannelIdx, u1backup_channel;
#endif    

    // only K highest freq
#if !FT_DSIM_USED
    if((p->frequency == u2DFSGetHighestFreq(p)) && (Get_PRE_MIOCK_JMETER_HQA_USED_flag()==0))
#endif    	
    {    
        mcSHOW_ERR_MSG(("PI_DUTY_IMPROVEMENT_TEST %d\n", PI_DUTY_IMPROVEMENT_TEST));
        mcSHOW_ERR_MSG(("DUTY_PI_PREDICTION %d\n", DUTY_PI_PREDICTION));
        /*TINFO="DUTY_PI_PREDICTION %d\n", DUTY_PI_PREDICTION */
        
        //Clk free run
        EnableDramcPhyDCM(p, 0);        
#if ENABLE_CLOCK_DUTY_SCAN
        retStatus= DramcClockDutyScan_InternalLoopBack(p, 0);
#if defined(SLT)
        //SLT : Must pass two conditions, 
        //From: YH Cho 
        //Sent: Saturday, June 17, 2017 9:19 PM
        //don't care LPBK_EN=0 pass or fail, must K LPBK_EN=1  if (retStatus != DRAM_FAIL)  
        {
            retStatus= DramcClockDutyScan_InternalLoopBack(p, 1);
        }
#endif
#endif            


#if ENABLE_DQS_DUTY_SCAN
        u1backup_channel = p->channel;

    #if DQS_DUTY_SLT_CONDITION_TEST
    #if DUTY_TEST_FOR_YH    
    for(u1PI_FB=0; u1PI_FB<64; u1PI_FB++)
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ5), u1PI_FB, SHU1_B0_DQ5_RG_ARPI_FB_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DQ5), u1PI_FB, SHU1_B1_DQ5_RG_ARPI_FB_B1);

        u4DQSDutyDiff_Max[CHANNEL_A][1]=0;
        u4DQSDutyDiff_Avrg[CHANNEL_A][1]=0;
        u4DQSDutyDiff_Min[CHANNEL_A][1]=0xffffffff;

        u2FailCnt =0;
    #endif
        
        for(u2TestCnt=0; u2TestCnt < u2TestCntTotal; u2TestCnt ++)
    #endif
        {
            #if DQS_DUTY_SLT_CONDITION_TEST
            u1GlobalTestCnt = u2TestCnt;
            #endif
            
            #if DUTY_TEST_FOR_YH
            u1ChannelIdx=0;
            #else
            for(u1ChannelIdx=0; u1ChannelIdx<(p->support_channel_num); u1ChannelIdx++)  
            #endif
            {
                vSetPHY2ChannelMapping(p, u1ChannelIdx);
                
#if !FT_DSIM_USED
                CBT_Switch_Freq(p, CBT_LOW_FREQ);                
                #if SOLUTION_FOR_SS_B422_TX_WIN_TOO_SMALL
                if(p->vendor_id == VENDOR_SAMSUNG) //Samsung dram only
                {
                    DramcModeRegWrite(p, 0xb, 0x03);     //ODT
                }
                else
                #endif
                    DramcModeRegWrite(p, 0xb, 0x04);     //ODT
                vSetDramMRWriteLevelingOnOff(p, 1);// enter write leveling mode to force dram term
                CBT_Switch_Freq(p, CBT_HIGH_FREQ);
#endif                

                u2FailStatusByCh[u1ChannelIdx]= DramcDQSDutyScan_InternalLoopBack(p, 0);
#if defined(SLT)
                //SLT : Must pass two conditions, 
                //From: YH Cho 
                //Sent: Saturday, June 17, 2017 9:19 PM
                //don't care LPBK_EN=0 pass or fail, must K LPBK_EN=1  if (u2FailStatusByCh[u1ChannelIdx] != DRAM_FAIL)
                {
                    u2FailStatusByCh[u1ChannelIdx]= DramcDQSDutyScan_InternalLoopBack(p, 1);
                }
#endif
                
                // Skip to mode register restore due to dram reset will execute in the next step.
                //CBT_Switch_Freq(p, CBT_LOW_FREQ);
                //DramcModeRegWrite(p, 0xb, 0x04);
                //vSetDramMRWriteLevelingOnOff(p, 0);
                //CBT_Switch_Freq(p, CBT_HIGH_FREQ);
                
            }
            
            vSetPHY2ChannelMapping(p, u1backup_channel);

        #if DQS_DUTY_SLT_CONDITION_TEST
            if(u2FailStatusByCh[0]==DRAM_FAIL || u2FailStatusByCh[1]==DRAM_FAIL)
            {
                u2FailCnt++;
            }
            mcSHOW_DBG_MSG(("\n[DQSDutyScan] Fail cnt =%d, TestCnt %d\n", u2FailCnt, u2TestCnt+1)); 
            /*TINFO="\n[DQSDutyScan] Fail cnt =%d, TestCnt %d\n", u2FailCnt, u2TestCnt+1 */
        #endif
        }

    #if DQS_DUTY_SLT_CONDITION_TEST
        for(u1ChannelIdx=0; u1ChannelIdx<(p->support_channel_num); u1ChannelIdx++)
        {
            vSetPHY2ChannelMapping(p, u1ChannelIdx);
            for(u1ByteIdx=0; u1ByteIdx<2; u1ByteIdx++)
            {
                u4DQSDutyDiff_Avrg[u1ChannelIdx][u1ByteIdx] = u4DQSDutyDiff_Avrg[u1ChannelIdx][u1ByteIdx]/u2TestCntTotal;
                u4Variance =0;
                
                for(u2TestCnt=0; u2TestCnt < u2TestCntTotal; u2TestCnt ++)
                {
                    if(u4DQSDutyDiff_Rec[u1ChannelIdx][u1ByteIdx][u2TestCnt] > u4DQSDutyDiff_Avrg[u1ChannelIdx][u1ByteIdx])
                    {
                        u4Variance += ((u4DQSDutyDiff_Rec[u1ChannelIdx][u1ByteIdx][u2TestCnt] - u4DQSDutyDiff_Avrg[u1ChannelIdx][u1ByteIdx]) *
                            (u4DQSDutyDiff_Rec[u1ChannelIdx][u1ByteIdx][u2TestCnt] - u4DQSDutyDiff_Avrg[u1ChannelIdx][u1ByteIdx]));
                    }
                    else
                    {
                        u4Variance += ((u4DQSDutyDiff_Avrg[u1ChannelIdx][u1ByteIdx] -u4DQSDutyDiff_Rec[u1ChannelIdx][u1ByteIdx][u2TestCnt]) *
                            (u4DQSDutyDiff_Avrg[u1ChannelIdx][u1ByteIdx] -u4DQSDutyDiff_Rec[u1ChannelIdx][u1ByteIdx][u2TestCnt]));
                    }  
                }
                
                u4Variance /= u2TestCntTotal;
                
                #if DUTY_TEST_FOR_YH
                mcSHOW_DBG_MSG(("RG_ARPI_FB =%d, ", u1PI_FB));
                /*TINFO="RG_ARPI_FB =%d, ", u1PI_FB */
                #endif
                mcSHOW_DBG_MSG(("CH %d, DQS %d, DutyDly %d, u4DQSDutyDiff_Max =%d, u4DQSDutyDiff_Min %d, u4DQSDutyDiff_Avrg %d, P2P=%d, Variance= %d\n",\
                    p->channel, u1ByteIdx, u4DQSDutyDutyDly[p->channel][u1ByteIdx],\
                    u4DQSDutyDiff_Max[u1ChannelIdx][u1ByteIdx], u4DQSDutyDiff_Min[u1ChannelIdx][u1ByteIdx],\
                    u4DQSDutyDiff_Avrg[u1ChannelIdx][u1ByteIdx], \
                    u4DQSDutyDiff_Max[u1ChannelIdx][u1ByteIdx] - u4DQSDutyDiff_Min[u1ChannelIdx][u1ByteIdx], u4Variance)); 
               /*TINFO="CH %d, DQS %d, DutyDly %d, u4DQSDutyDiff_Max =%d, u4DQSDutyDiff_Min %d, u4DQSDutyDiff_Avrg %d, P2P=%d, Variance= %d\n",  p->channel, u1ByteIdx, u4DQSDutyDutyDly[p->channel][u1ByteIdx],  u4DQSDutyDiff_Max[u1ChannelIdx][u1ByteIdx], u4DQSDutyDiff_Min[u1ChannelIdx][u1ByteIdx], u4DQSDutyDiff_Avrg[u1ChannelIdx][u1ByteIdx], u4DQSDutyDiff_Max[u1ChannelIdx][u1ByteIdx] - u4DQSDutyDiff_Min[u1ChannelIdx][u1ByteIdx], u4Variance   */ 
            }
        }
        
        mcSHOW_DBG_MSG(("\n"));
        /*TINFO="\n" */
        vSetPHY2ChannelMapping(p, u1backup_channel);
        #if DUTY_TEST_FOR_YH
        }
        #endif
    #endif
    
#endif  //end of ENABLE_DQS_DUTY_SCAN
    }
}
#endif            
#endif


#if 0//TX_PER_BIT_DELAY_CELL
DRAM_STATUS_T DramcMiockJmeterForTXCalibration(DRAMC_CTX_T *p)
{
    U16 freq_backup = p->frequency;

    U32 u4VCore1, u4VCore2;
    U16 u2Time1, u2Time2;

    if(u1IsLP4Family(p->dram_type))
    {
        #ifdef ENABLE_MIOCK_JMETER
        p->frequency = 1333;
        vSetVcoreByFreq(p);
        DramcMiockJmeter(p);
        u2Time1 = u2gdelay_cell_ps;
        u4VCore1 = pmic_vcore_voltage_read();

        p->frequency = 1600;
        vSetVcoreByFreq(p);
        DramcMiockJmeter(p);
        u2Time2 = u2gdelay_cell_ps;
        u4VCore2 = pmic_vcore_voltage_read();

        p->frequency = freq_backup;
        vSetVcoreByFreq(p);
        mcSHOW_DBG_MSG(("[MiockJmeterForTXCalibration] CH_%d, (VCORE=%d, cell=%d) (VCORE=%d, cell=%d)\n",p->channel, u4VCore1, u2Time1,  u4VCore2, u2Time2));

        p->u2DelayCellTimex100 = ((u2Time1+u2Time2)>>1);
        #else
        p->u2DelayCellTimex100= 800; // when jilter meter is disable, use 1 delay cell =8 ps
        mcSHOW_DBG_MSG(("[MiockJmeterForTXCalibration] CH_%d, when jitter meter is disable, use 1 delay cell =8 ps\n",p->channel));
        #endif
        return DRAM_OK;
    }
    else
        return DRAM_FAIL;
}
#endif

#ifdef ENABLE_MIOCK_JMETER_HQA
/* "picoseconds per delay cell" depends on Vcore only (frequency doesn't matter)
 * 1. Retrieve current freq's vcore voltage using pmic API
 * 2. Perform delay cell time calculation (Bypass if shuffle vcore value is the same as before)
 */
static void GetVcoreDelayCellTime(DRAMC_CTX_T *p, U8 shuffleIdx)
{
    static U32 u4previousVcore = 0;
    U32 channel_i;

    vSetVcoreByFreq(p);
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
    U16 freq_backup = p->frequency;
    U32 channel_idx;
    U8 shuffleIdx;

    mcSHOW_DBG_MSG(("[MiockJmeterHQA]\n"));

    if (u1IsLP4Family(p->dram_type))
    {
            if(p->channel == CHANNEL_A)
            {
            for (shuffleIdx = DRAM_DFS_SHUFFLE_1; shuffleIdx < DRAM_DFS_SHUFFLE_MAX; shuffleIdx++)
            {
               p->frequency = gFreqTbl[shuffleIdx].frequency;
               GetVcoreDelayCellTime(p, shuffleIdx);
            }

            /* Use highest freq's delay cell time measurement results as reference */
            p->ucnum_dlycell_perT = ucg_num_dlycell_perT_all[DRAM_DFS_SHUFFLE_1][p->channel];
            p->u2DelayCellTimex100 = u2gdelay_cell_ps_all[DRAM_DFS_SHUFFLE_1][p->channel];
            mcSHOW_DBG_MSG3(("DelayCellTimex100 CH_%d, (VCORE=%d, cell=%d)\n",p->channel, u4gVcore[DRAM_DFS_SHUFFLE_1], p->u2DelayCellTimex100));
        }

        for(channel_idx=CHANNEL_B; channel_idx<CHANNEL_NUM; channel_idx++)
        {
            for (shuffleIdx = DRAM_DFS_SHUFFLE_1; shuffleIdx < DRAM_DFS_SHUFFLE_MAX; shuffleIdx++)
            {
                u2gdelay_cell_ps_all[shuffleIdx][channel_idx] = u2gdelay_cell_ps_all[shuffleIdx][CHANNEL_A];
            }
        }
    }
#if ENABLE_LP3_SW
    else
    {
        p->frequency = 933;
        GetVcoreDelayCellTime(p, DRAM_DFS_SHUFFLE_1);

        p->frequency = 800;
        GetVcoreDelayCellTime(p, DRAM_DFS_SHUFFLE_2);

        p->frequency = 600;
        GetVcoreDelayCellTime(p, DRAM_DFS_SHUFFLE_3);
    }
#endif /* ENABLE_LP3_SW */

    p->frequency = freq_backup;
    vSetVcoreByFreq(p);
}
#endif //#ifdef ENABLE_MIOCK_JMETER

void DramcWriteDBIOnOff(DRAMC_CTX_T *p, U8 onoff)
{
    U8 u1value;
   
	// DRAMC Write-DBI On/Off
	if(u1IsLP4Family(p->dram_type))
	{
		vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SHU1_WODT), onoff, SHU1_WODT_DBIWR);

        u1value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_WODT), SHU1_WODT_DBIWR);
        mcSHOW_DBG_MSG(("[DramcWriteDBIOnOff] DRAMC Write-DBI On/Off = %d \n", u1value));
    }
}
        
void DramcReadDBIOnOff(DRAMC_CTX_T *p, U8 onoff)
{
    U8 u1value;
    
	// DRAMC Read-DBI On/Off
	if(u1IsLP4Family(p->dram_type))
	{
		vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ7), onoff, SHU1_B0_DQ7_R_DMDQMDBI_SHU_B0);
		vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DQ7), onoff, SHU1_B1_DQ7_R_DMDQMDBI_SHU_B1);

        u1value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ7), SHU1_B0_DQ7_R_DMDQMDBI_SHU_B0);
        mcSHOW_DBG_MSG(("[DramcReadDBIOnOff] DRAMC Read-DBI On/Off  = %d \n", u1value));
        u1value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DQ7), SHU1_B1_DQ7_R_DMDQMDBI_SHU_B1);
        mcSHOW_DBG_MSG(("[DramcReadDBIOnOff] DRAMC Read-DBI On/Off  = %d \n", u1value));
	}
}
#if ENABLE_READ_DBI
void SetDramModeRegForReadDBIOnOff(DRAMC_CTX_T *p, U8 onoff)
{
    if(u1IsLP4Family(p->dram_type))
	{
      //mcSHOW_DBG_MSG(("--Fsp%d --\n", p->dram_fsp));

      //DRAM MR3[6] read-DBI On/Off
      u1MR03Value[p->dram_fsp] = ((u1MR03Value[p->dram_fsp] & 0xbf) | (onoff<<6));
      DramcModeRegWriteByRank(p, p->rank, 0x3, u1MR03Value[p->dram_fsp]);
    }
}
#endif

#if ENABLE_WRITE_DBI
void DramcWriteMinus1MCKForWriteDBI(DRAMC_CTX_T *p, S8 iShiftUI)
{
  U8 ucdq_ui_large_dqs0, ucdq_ui_large_dqs1;
  U8 ucdq_final_dqm_ui_large_dqs0, ucdq_final_dqm_ui_large_dqs1;
  REG_TRANSFER_T TransferReg[2];
  
  if((u1IsLP4Family(p->dram_type))&&(p->DBI_W_onoff[p->dram_fsp]))
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

void SetDramModeRegForWriteDBIOnOff(DRAMC_CTX_T *p, U8 onoff)
{
    if(u1IsLP4Family(p->dram_type))
	{
      //mcSHOW_DBG_MSG(("--Fsp%d --\n", p->dram_fsp));

      //DRAM MR3[7] write-DBI On/Off
      u1MR03Value[p->dram_fsp] = ((u1MR03Value[p->dram_fsp] & 0x7F) | (onoff<<7));
      DramcModeRegWriteByRank(p, p->rank, 0x3, u1MR03Value[p->dram_fsp]);
    }
}
#endif

#if (SW_CHANGE_FOR_SIMULATION || FOR_DV_SIMULATION_USED)
void vMR2InitForSimulationTest(DRAMC_CTX_T *p)
{
    if(u1IsLP4Family(p->dram_type))
    {
    // Dram Init will set MR2's value
    #if 0
        if(p->frequency >HIGH_FREQ)
        {
            u1MR02Value[p->dram_fsp]  = 0x3f;
        }
        else if(p->frequency <=DUAL_FREQ_LOW_LP4)
        {
            u1MR02Value[p->dram_fsp]  = 0x1b;
        }
        else
        {
            u1MR02Value[p->dram_fsp]  = 0x2d;
        }
    #endif
    }
    #if ENABLE_LP3_SW
    else
    {
        #if SUPPORT_LP3_800
        if(p->frequency<=400)
        {
            u1MR02Value[p->dram_fsp] = 0x18;
        }
        else
        #endif
        if(p->frequency==533)
        {
            u1MR02Value[p->dram_fsp] = 0x16;
        }
        else if(p->frequency == 635)
        {
            u1MR02Value[p->dram_fsp] = 0x18;
        }
        else if(p->frequency == 800)
        {
            u1MR02Value[p->dram_fsp] = 0x1a;
        }
        else
        {
            u1MR02Value[p->dram_fsp] = 0x1c;
        }
    }
    #endif
}
#endif

#if ENABLE_WRITE_DBI_Protect
void ApplyWriteDBIProtect(DRAMC_CTX_T *p, U8 onoff)
{
    U8 *uiLPDDR_PHY_Mapping;
    U16 Temp_PinMux_MaskWrite_WriteDBIOn=0;
    U8 B0_PinMux_MaskWrite_WriteDBIOn=0, B1_PinMux_MaskWrite_WriteDBIOn=0;
    int DQ_index;
    
    uiLPDDR_PHY_Mapping = (U8 *)uiLPDDR4_PHY_Mapping_POP[p->channel];

    // Write DMI/DBI Protect Function
    // Byte0 can not have bit swap between Group1(DQ0/1) and Group2(DQ02~DQ07).
    // Byte1 can not have bit swap between Group1(DQ8/9) and Group2(DQ10~DQ15). 
    // DBIWR_IMP_EN=1 and DBIWR_PINMUX_EN=1
    // set DBIWR_OPTB0[7:0] meet with Byte0 pin MUX table. 
    // set DBIWR_OPTB1[7:0] meet with Byte1 pin MUX table.
    
    for(DQ_index=0; DQ_index<16; DQ_index++)
    {
        Temp_PinMux_MaskWrite_WriteDBIOn |= ((0x7C7C >> uiLPDDR_PHY_Mapping[DQ_index]) & 0x1) << DQ_index;
    }
    B1_PinMux_MaskWrite_WriteDBIOn = (U8)(Temp_PinMux_MaskWrite_WriteDBIOn>>8) & 0xff;
    B0_PinMux_MaskWrite_WriteDBIOn = (U8) Temp_PinMux_MaskWrite_WriteDBIOn & 0xff;
    
    vIO32WriteFldMulti_All(DRAMC_REG_ARBCTL, P_Fld(B1_PinMux_MaskWrite_WriteDBIOn, ARBCTL_DBIWR_OPT_B1)
                                           | P_Fld(B0_PinMux_MaskWrite_WriteDBIOn, ARBCTL_DBIWR_OPT_B0)
                                           | P_Fld(onoff, ARBCTL_DBIWR_PINMUX_EN)
                                           | P_Fld(onoff, ARBCTL_DBIWR_IMP_EN));
}
#endif

#if ENABLE_WRITE_DBI   
void ApplyWriteDBIPowerImprove(DRAMC_CTX_T *p, U8 onoff)
{
    // set DBIWR_IMP_EN = 1
    // DBIWR_OPTB0[1:0]=0, DBIWR_OPT_B0[7]=0
    // DBIWR_OPTB1[1:0]=0, DBIWR_OPT_B1[7]=0
    vIO32WriteFldMulti_All(DRAMC_REG_ARBCTL, P_Fld(0, ARBCTL_DBIWR_OPT_bit15)
                                           | P_Fld(0, ARBCTL_DBIWR_OPT_bit9_8)
                                           | P_Fld(0, ARBCTL_DBIWR_OPT_bit7)
                                           | P_Fld(0, ARBCTL_DBIWR_OPT_bit1_0)
                                           | P_Fld(onoff, ARBCTL_DBIWR_IMP_EN));
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
    // IC and DRAM package
    DramConfig.package = PACKAGE_POP;
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
    DramConfig.fglow_freq_write_en = DISABLE;
    // Switch to ENABLE or DISABLE SSC (Spread Spectrum Clock)
    DramConfig.ssc_en = DISABLE;
    DramConfig.en_4bitMux = DISABLE;

    DramConfig.enable_rx_scan_vref =DISABLE;
    DramConfig.enable_tx_scan_vref =DISABLE;
    //DramConfig.dynamicODT = DISABLE;

    MPLLInit();

    Global_Option_Init(&DramConfig);

    // DramC & PHY init for all channels
    DDRPhyFreqSel(&DramConfig, LP4_DDR1600);


#if WRITE_LEVELING_MOVE_DQS_INSTEAD_OF_CLK
    memset(DramConfig.arfgWriteLevelingInitShif, FALSE, sizeof(DramConfig.arfgWriteLevelingInitShif));
    //>fgWriteLevelingInitShif= FALSE;
#endif
#if TX_PERBIT_INIT_FLOW_CONTROL
    memset(DramConfig.fgTXPerbifInit, FALSE, sizeof(DramConfig.fgTXPerbifInit));
#endif

    DramcInit(&DramConfig);

    vApplyConfigBeforeCalibration(&DramConfig);
    vMR2InitForSimulationTest(&DramConfig);

    vSetPHY2ChannelMapping(&DramConfig, DramConfig.channel);

    #if SIMULATION_SW_IMPED
    if (u1IsLP4Family(DramConfig.dram_type))
    {
        DramcSwImpedanceCal(&DramConfig,1, 1);  //within term
        DramcSwImpedanceCal(&DramConfig,1, 0);  //without term
    }
#if ENABLE_LP3_SW
    else
    {
        DramcSwImpedanceCal(&DramConfig,1, 1);  //within term
    }
#endif /* ENABLE_LP3_SW */
    #endif


#if SIMULATION_LP4_ZQ
     if (DramConfig.dram_type == TYPE_LPDDR4 || DramConfig.dram_type == TYPE_LPDDR4X || DramConfig.dram_type == TYPE_LPDDR4P)
     {
         DramcZQCalibration(&DramConfig);
     }
#endif

    if (u1IsLP4Family(DramConfig.dram_type))
    {
        #if SIMUILATION_LP4_CBT
        CmdBusTrainingLP4(&DramConfig);
        #endif
    }
#if ENABLE_LP3_SW
    else
    {
        #if SIMULATION_LP3_CA_TRAINING
        vSetRank(DramConfig, RANK_0);
        CATrainingLP3(&DramConfig);
        #endif
    }
#endif /* ENABLE_LP3_SW */

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
    DramcRxWindowPerbitCal(&DramConfig, 0);
#endif

    #if SIMULATION_DATLAT
    // RX Datlat calibration of single rank
    DramcRxdatlatCal(&DramConfig);

    // RX Datlat calibration of two rank
    //DramcDualRankRxdatlatCal(&DramConfig);
    #endif

    #if SIMULATION_RX_PERBIT
    DramcRxWindowPerbitCal(&DramConfig, 1);
    #endif

    #if SIMULATION_TX_PERBIT
    DramcTxWindowPerbitCal(&DramConfig, TX_DQ_DQS_MOVE_DQ_DQM);
    DramcTxWindowPerbitCal(&DramConfig, TX_DQ_DQS_MOVE_DQ_ONLY);   
    #endif

    #if ENABLE_READ_DBI
    //Read DBI ON
    SetDramModeRegForReadDBIOnOff(&DramConfig, DramConfig.DBI_R_onoff[DramConfig.dram_fsp]);
    #endif
            
    #if ENABLE_WRITE_DBI
    //Write DBI ON
    DramcWriteMinus1MCKForWriteDBI(&DramConfig, -8); //Tx DQ/DQM -1 MCK for write DBI ON
    SetDramModeRegForWriteDBIOnOff(&DramConfig, DramConfig.DBI_W_onoff[DramConfig.dram_fsp]);
    #endif

    #if ENABLE_READ_DBI
    DramcReadDBIOnOff(&DramConfig, DramConfig.DBI_R_onoff[DramConfig.dram_fsp]);
    #endif

    #if ENABLE_WRITE_DBI      
    DramcWriteDBIOnOff(&DramConfig, DramConfig.DBI_W_onoff[DramConfig.dram_fsp]);
    #endif
}
#endif //SW_CHANGE_FOR_SIMULATION

