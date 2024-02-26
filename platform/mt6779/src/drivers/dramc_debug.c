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

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "dramc_common.h"
#include "x_hal_io.h"
#include "dramc_pi_api.h"

#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
#ifndef MT6779_FPGA 
#if !__FLASH_TOOL_DA__
#include <pmic.h>
#else
#include "../pmic/pmic.h"
#endif
#endif
#endif


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------

#ifdef FOR_HQA_TEST_USED
extern U16 gFinalCBTVrefCA[CHANNEL_NUM][RANK_MAX];
extern U16 gFinalCBTCA[CHANNEL_NUM][RANK_MAX][10];
extern U16 gFinalRXPerbitWin[CHANNEL_NUM][RANK_MAX][DQ_DATA_WIDTH];
extern U16 gFinalTXPerbitWin[CHANNEL_NUM][RANK_MAX][DQ_DATA_WIDTH];
extern U16 gFinalTXPerbitWin_min_max[CHANNEL_NUM][RANK_MAX];
extern U16 gFinalTXPerbitWin_min_margin[CHANNEL_NUM][RANK_MAX];
extern U16 gFinalTXPerbitWin_min_margin_bit[CHANNEL_NUM][RANK_MAX];
extern S8 gFinalClkDuty[CHANNEL_NUM];
extern U32 gFinalClkDutyMinMax[CHANNEL_NUM][2];
extern S8 gFinalDQSDuty[CHANNEL_NUM][DQS_NUMBER];
extern U32 gFinalDQSDutyMinMax[CHANNEL_NUM][DQS_NUMBER][2];

extern U8 gCBT_VREF_RANGE_SEL;
extern U32 u4gVcore[DRAM_DFS_SHUFFLE_MAX];
#endif

#if ENABLE_EYESCAN_GRAPH || defined(FOR_HQA_REPORT_USED)
extern const U16 gRXVref_Voltage_Table_LP4[RX_VREF_RANGE_END+1];
extern const U16 gVref_Voltage_Table_LP4X[VREF_RANGE_MAX][VREF_VOLTAGE_TABLE_NUM];
extern const U16 gVref_Voltage_Table_LP4[VREF_RANGE_MAX][VREF_VOLTAGE_TABLE_NUM];
#endif

extern U8 gHQA_Test_Freq_Vcore_Level;

#if __ETT__
#define __NOBITS_SECTION__(x) 
#endif

#if ENABLE_EYESCAN_GRAPH
__NOBITS_SECTION__(.gEyeScan_Min) S8 gEyeScan_Min[VREF_TOTAL_NUM_WITH_RANGE][DQ_DATA_WIDTH_LP4][EYESCAN_BROKEN_NUM];
__NOBITS_SECTION__(.gEyeScan_Max) S8 gEyeScan_Max[VREF_TOTAL_NUM_WITH_RANGE][DQ_DATA_WIDTH_LP4][EYESCAN_BROKEN_NUM];
U16  gEyeScan_CaliDelay[DQS_NUMBER];
S16 gEyeScan_MinMax_store_delay[DQS_NUMBER];
__NOBITS_SECTION__(.gEyeScan_WinSize) U8 gEyeScan_WinSize[VREF_TOTAL_NUM_WITH_RANGE][DQ_DATA_WIDTH_LP4];
S8  gEyeScan_DelayCellPI[DQ_DATA_WIDTH_LP4];
U16  gEyeScan_ContinueVrefHeight[DQ_DATA_WIDTH_LP4];
U16  gEyeScan_TotalPassCount[DQ_DATA_WIDTH_LP4];

extern const U8 uiLPDDR4_O1_Mapping_POP[CHANNEL_NUM][16]; 
#endif

U8* HQA_LOG_Parsing_Freq(void)
{
    if (gHQA_Test_Freq_Vcore_Level != 0)
    {
        return (gHQA_Test_Freq_Vcore_Level==1 ? "_L" : "_H");
    }

    return " ";
}
void HQA_LOG_Print_Prefix_String(DRAMC_CTX_T *p)
{
    mcSHOW_DBG_MSG(("[HQALOG] %d%s ", p->frequency*2, HQA_LOG_Parsing_Freq() ));
}

void HQA_LOG_Print_Freq_String(DRAMC_CTX_T *p)
{
#ifdef VENDER_JV_LOG
    mcSHOW_EYESCAN_MSG(("%d%s ", p->frequency*2, HQA_LOG_Parsing_Freq() ));
#else
    mcSHOW_DBG_MSG(("%d%s ", p->frequency*2, HQA_LOG_Parsing_Freq() ));
#endif
}

void EYESCAN_LOG_Print_Prefix_String(void)
{
    mcSHOW_DBG_MSG(("[EYESCAN_LOG] "));
}

U8* print_EYESCAN_LOG_type(U8 print_type)
{
    switch (print_type)
    {
        case 0: return "CBT";
        case 1: return "RX";
        default:
        case 2: return "TX";
    }
}


#ifdef FOR_HQA_TEST_USED
void HQA_measure_message_reset_all_data(DRAMC_CTX_T *p)
{
    U32 uiCA, u1BitIdx, u1ByteIdx, u1RankIdx, u1ChannelIdx;

    for(u1ChannelIdx=CHANNEL_A; u1ChannelIdx<CHANNEL_NUM; u1ChannelIdx++)
    {
        for(u1RankIdx=RANK_0; u1RankIdx<RANK_MAX; u1RankIdx++)
        {
            for (u1BitIdx=0; u1BitIdx<p->data_width; u1BitIdx++)
            {
                gFinalRXPerbitWin[u1ChannelIdx][u1RankIdx][u1BitIdx] =0;
                gFinalTXPerbitWin[u1ChannelIdx][u1RankIdx][u1BitIdx] =0;
            }
        }
    }
}

#ifdef RELEASE
#undef mcSHOW_DBG_MSG
#define mcSHOW_DBG_MSG(_x_)	opt_print _x_
#endif
void print_HQA_measure_message(DRAMC_CTX_T *p)
{
    U32 uiCA, u1BitIdx, u1ByteIdx, u1RankIdx, u1ChannelIdx;
    U32 min_ca_value[CHANNEL_NUM][RANK_MAX], min_ca_bit[CHANNEL_NUM][RANK_MAX];
    U32 min_rx_value[CHANNEL_NUM][RANK_MAX], min_tx_value[CHANNEL_NUM][RANK_MAX];
    U32 min_RX_DQ_bit[CHANNEL_NUM][RANK_MAX], min_TX_DQ_bit[CHANNEL_NUM][RANK_MAX];
    U8 shuffleIdx, local_channel_num=2, shuffle_index;
    U8 print_imp_option[2]={FALSE, FALSE};
    int i;
    U32 read_voltage_value;

    mcSHOW_DBG_MSG(("\n\n\n[HQA] information for measurement, "));
    mcSHOW_DBG_MSG(("\tDram Data rate = ")); HQA_LOG_Print_Freq_String(p); mcSHOW_DBG_MSG(("\n"));
    vPrintCalibrationBasicInfo(p);
    HQA_LOG_Print_Prefix_String(p); mcSHOW_DBG_MSG(("Frequency = %u\n", DDRPhyFMeter()));

    shuffleIdx = get_shuffleIndex_by_Freq(p);

    if(u1IsLP4Family(p->dram_type))
    {
        local_channel_num = p->support_channel_num;
    }

    for(u1ChannelIdx=CHANNEL_A; u1ChannelIdx<local_channel_num; u1ChannelIdx++)
    {
        for(u1RankIdx=RANK_0; u1RankIdx<RANK_MAX; u1RankIdx++)
        {
            min_ca_value[u1ChannelIdx][u1RankIdx] = 0xffff;
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


    if (p->support_rank_num==RANK_DUAL)
    {
        //Preloader LP3 RX/TX only K Rank0, so Rank1 use Rank0's value
        if(!u1IsLP4Family(p->dram_type))
        {
#ifndef LP3_DUAL_RANK_RX_K    
            min_rx_value[0][1] = min_rx_value[0][0];
            min_RX_DQ_bit[0][1] = min_RX_DQ_bit[0][0];
#endif        

#ifndef LP3_DUAL_RANK_TX_K
            min_tx_value[0][1] = min_tx_value[0][0];
            gFinalTXPerbitWin_min_max[0][1] = gFinalTXPerbitWin_min_max[0][0];
            min_TX_DQ_bit[0][1] = min_TX_DQ_bit[0][0];
#endif        

            #if 0//(TX_PER_BIT_DELAY_CELL==0)
            gFinalTXPerbitWin_min_margin[0][1] = gFinalTXPerbitWin_min_margin[0][0];
            gFinalTXPerbitWin_min_margin_bit[0][1] = gFinalTXPerbitWin_min_margin_bit[0][0];
            #endif
        }
    }


#if defined(DRAM_HQA)
        mcSHOW_DBG_MSG(("[Read Voltage]\n"));
        read_voltage_value = vGetVoltage(p, 0);
        HQA_LOG_Print_Prefix_String(p); mcSHOW_DBG_MSG(("Vcore_HQA = %d\n", read_voltage_value));

        if (u1IsLP4Family(p->dram_type)) {
            /* LPDDR4 */
            read_voltage_value = vGetVoltage(p, 1);
            HQA_LOG_Print_Prefix_String(p); mcSHOW_DBG_MSG(("Vdram_HQA = %d\n", read_voltage_value));
            read_voltage_value = vGetVoltage(p, 2);
            HQA_LOG_Print_Prefix_String(p); mcSHOW_DBG_MSG(("Vddq_HQA = %d\n", read_voltage_value));
            read_voltage_value = vGetVoltage(p, 3);
            HQA_LOG_Print_Prefix_String(p); mcSHOW_DBG_MSG(("Vdd1_HQA = %d\n", read_voltage_value));  /* confirm with pmic owner¡Athis vio18 is pmic 6359 's vm18, is different with vio18 of system */
            read_voltage_value = vGetVoltage(p, 4);
            HQA_LOG_Print_Prefix_String(p); mcSHOW_DBG_MSG(("Vmddr_HQA = %d\n", read_voltage_value));
        } else {
            /* LPDDR3 */
            read_voltage_value = vGetVoltage(p, 1);
            HQA_LOG_Print_Prefix_String(p); mcSHOW_DBG_MSG(("Vdram_HQA = %d\n", read_voltage_value));
        }
        mcSHOW_DBG_MSG(("\n"));
#endif

    /*
        [Impedance Calibration]

        freq_region=0
        [HQALOG] Impedance freq_region=0 DRVP 11
        [HQALOG] Impedance freq_region=0 DRVN 7

        freq_region=1
        [HQALOG] Impedance freq_region=1 DRVP 13
        [HQALOG] Impedance freq_region=1 ODTN 15
    */
    if (p->dram_type == TYPE_LPDDR4)
    {
        print_imp_option[1] = TRUE;
    }
    else if (p->dram_type == TYPE_LPDDR4X)
    {
        print_imp_option[0] = TRUE;
        print_imp_option[1] = TRUE;
    }
    else
    {
      //TYPE_LPDDR4P, TYPE_LPDDR3
      print_imp_option[0] = TRUE;
    }

#ifdef FOR_HQA_REPORT_USED
if (gHQALog_flag==1)
{
    mcSHOW_DBG_MSG(("[Impedance Calibration]\n"));

    HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT6, "Impedance freq_region=0 ", "DRVP", 0, gDramcSwImpedanceResult[0][DRVP], NULL);            
    HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT6, "Impedance freq_region=0 ", "DRVN", 0, gDramcSwImpedanceResult[0][DRVN], NULL);                                
    HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT6, "Impedance freq_region=0 ", "ODTN", 0, gDramcSwImpedanceResult[0][ODTN], NULL);                                    

    mcSHOW_DBG_MSG(("\n"));
}
#endif





    mcSHOW_DBG_MSG(("\n[Cmd Bus Training window]\n"));
    if(u1IsLP4Family(p->dram_type))
    {
        mcSHOW_DBG_MSG(("VrefCA Range : %d\n", gCBT_VREF_RANGE_SEL));
        /*
         VrefCA
             [HQALOG] 1600 VrefCA Channel0 Rank0 32
             [HQALOG] 1600 VrefCA Channel0 Rank1 24
             [HQALOG] 1600 VrefCA Channel1 Rank0 26
             [HQALOG] 1600 VrefCA Channel1 Rank1 30
         */
        mcSHOW_DBG_MSG(("VrefCA\n"));
        for(u1ChannelIdx=CHANNEL_A; u1ChannelIdx<local_channel_num; u1ChannelIdx++)
        {
            for(u1RankIdx = RANK_0; u1RankIdx < 2; u1RankIdx++)
            {
                HQA_LOG_Print_Prefix_String(p); mcSHOW_DBG_MSG(("VrefCA Channel%d "
                                "Rank%d %d\n",
                            u1ChannelIdx,
                                u1RankIdx,
                                gFinalCBTVrefCA[u1ChannelIdx][u1RankIdx]));
            }
        }
    }

#if 0//(SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_CBT)
    if(p->femmc_Ready==1 )
    {
        mcSHOW_DBG_MSG(("\n[Cmd Bus Training window bypass calibration]\n"));
    }
    else
#endif
    {
        /*
         CA_Window
             [HQALOG] 1600 CA_Window Channel0 Rank0 61(bit 2)
             [HQALOG] 1600 CA_Window Channel0 Rank1 62(bit 1)
             [HQALOG] 1600 CA_Window Channel1 Rank0 60(bit 5)
             [HQALOG] 1600 CA_Window Channel1 Rank1 60(bit 5)
         */
        mcSHOW_DBG_MSG(("CA_Window\n"));
#ifdef FOR_HQA_REPORT_USED
if (gHQALog_flag==1)
{
        for(u1ChannelIdx=CHANNEL_A; u1ChannelIdx<local_channel_num; u1ChannelIdx++)
        {
            for(u1RankIdx = RANK_0; u1RankIdx < 2; u1RankIdx++)
            {
                    HQA_Log_Message_for_Report(p, u1ChannelIdx, u1RankIdx, HQA_REPORT_FORMAT2, "CA", "_Window", 0, min_ca_value[u1ChannelIdx][u1RankIdx], NULL);
                    HQA_Log_Message_for_Report(p, u1ChannelIdx, u1RankIdx, HQA_REPORT_FORMAT2, "CA", "_Window_bit", 0, min_ca_bit[u1ChannelIdx][u1RankIdx], NULL);
            }
        }
        mcSHOW_DBG_MSG(("\n"));
}
else
#endif
{
        for(u1ChannelIdx=CHANNEL_A; u1ChannelIdx<local_channel_num; u1ChannelIdx++)
        {
            for(u1RankIdx = RANK_0; u1RankIdx < 2; u1RankIdx++)
            {
                HQA_LOG_Print_Prefix_String(p); mcSHOW_DBG_MSG(("CA_Window Channel%d "
                                "Rank%d %d (bit %d)\n",
                                u1ChannelIdx,
                                u1RankIdx,
                                min_ca_value[u1ChannelIdx][u1RankIdx], min_ca_bit[u1ChannelIdx][u1RankIdx]));
            }
        }
}

        /*
         CA Min Window(%)
             [HQALOG] 1600 CA_Window(%) Channel0 Rank0 96%(PASS)
             [HQALOG] 1600 CA_Window(%) Channel0 Rank1 97%(PASS)
             [HQALOG] 1600 CA_Window(%) Channel1 Rank0 94%(PASS)
             [HQALOG] 1600 CA_Window(%) Channel1 Rank1 94%(PASS)
         */
        mcSHOW_DBG_MSG(("CA Min Window(%%)\n"));
#ifdef FOR_HQA_REPORT_USED
if (gHQALog_flag==1)
{
        for(u1ChannelIdx=CHANNEL_A; u1ChannelIdx<local_channel_num; u1ChannelIdx++)
        {
            for(u1RankIdx = RANK_0; u1RankIdx < 2; u1RankIdx++)
            {
                HQA_Log_Message_for_Report(p, u1ChannelIdx, u1RankIdx, HQA_REPORT_FORMAT2, "CA", "_Window(%)", 0, ((min_ca_value[u1ChannelIdx][u1RankIdx] * 100 + (u1IsLP4Family(p->dram_type)==1?63:31)) / (u1IsLP4Family(p->dram_type)==1?64:32)), NULL);
                HQA_Log_Message_for_Report(p, u1ChannelIdx, u1RankIdx, HQA_REPORT_FORMAT4, "CA", "_Window_PF", 0, 0, ((((min_ca_value[u1ChannelIdx][u1RankIdx] * 100 + (u1IsLP4Family(p->dram_type)==1?63:31)) / (u1IsLP4Family(p->dram_type)==1?64:32)) >= 30) ? "PASS" : "FAIL"));
            }
        }
        mcSHOW_DBG_MSG(("\n"));
}
else
#endif
{
        for(u1ChannelIdx=CHANNEL_A; u1ChannelIdx<local_channel_num; u1ChannelIdx++)
        {
            for(u1RankIdx = RANK_0; u1RankIdx < 2; u1RankIdx++)
            {
                HQA_LOG_Print_Prefix_String(p); mcSHOW_DBG_MSG(("CA_Window(%%) Channel%d "
                                "Rank%d %d%% (%s)\n",
                                u1ChannelIdx,
                                u1RankIdx,
                                        ((min_ca_value[u1ChannelIdx][u1RankIdx] * 100 + (u1IsLP4Family(p->dram_type)==1?63:31)) / (u1IsLP4Family(p->dram_type)==1?64:32)),
                                    ((((min_ca_value[u1ChannelIdx][u1RankIdx] * 100 + (u1IsLP4Family(p->dram_type)==1?63:31)) / (u1IsLP4Family(p->dram_type)==1?64:32)) >= 30) ? "PASS" : "FAIL")));
            }
        }
}
    }
            mcSHOW_DBG_MSG(("\n"));




    /*
    [RX minimum per bit window]
    Delay cell measurement (/100ps)
    [HQALOG] 3200 delaycell 892
    */
    mcSHOW_DBG_MSG(("\n[RX minimum per bit window]\n"));
    mcSHOW_DBG_MSG(("Delaycell measurement(/100ps)\n"));    
#if !defined(RELEASE) && (VENDER_JV_LOG==0)    
    HQA_LOG_Print_Prefix_String(p); mcSHOW_DBG_MSG(("delaycell %d\n",
                    gHQALOG_RX_delay_cell_ps_075V));
#endif
    /*
     VrefDQ
         [HQALOG] 1600 VrefRX Channel0 24
         [HQALOG] 1600 VrefRX Channel1 24
     */

    if(u1IsLP4Family(p->dram_type))
    {
        if (Get_Vref_Calibration_OnOff(p)==VREF_CALI_ON)
        {
            mcSHOW_DBG_MSG(("VrefRX\n"));
            if (gRX_EYE_Scan_flag==1)
            {
                for(u1ChannelIdx=CHANNEL_A; u1ChannelIdx<local_channel_num; u1ChannelIdx++)
                {
                    for(u1RankIdx = RANK_0; u1RankIdx < 2; u1RankIdx++)
                    {
                        for(u1ByteIdx = 0; u1ByteIdx < p->data_width/DQS_BIT_NUMBER; u1ByteIdx++)
                        {
                            HQA_LOG_Print_Prefix_String(p); mcSHOW_DBG_MSG(("VrefRX Channel%d Rank%d Byte%d %d\n",
                                        u1ChannelIdx,
                                        u1RankIdx,
                                        u1ByteIdx,
                                        gFinalRXVrefDQ[u1ChannelIdx][u1RankIdx][u1ByteIdx]));
                        }
                    }
                }
            }
            else
            {
                for(u1ChannelIdx=CHANNEL_A; u1ChannelIdx<local_channel_num; u1ChannelIdx++)
                {
                    for(u1ByteIdx = 0; u1ByteIdx < p->data_width/DQS_BIT_NUMBER; u1ByteIdx++)
                    {
                        HQA_LOG_Print_Prefix_String(p); mcSHOW_DBG_MSG(("VrefRX Channel%d Byte%d %d\n",
                                    u1ChannelIdx,
                                    u1ByteIdx,
                                    gFinalRXVrefDQ[u1ChannelIdx][RANK_0][u1ByteIdx]));
                    }
                }
            }
        }
        else
        {
            mcSHOW_DBG_MSG(("RX DQ Vref Scan : Disable\n"));
        }
    }
    
#if 0//(SUPPORT_SAVE_TIME_FOR_CALIBRATION )
    if(p->femmc_Ready==1 && ( p->Bypass_RXWINDOW))
 	{
        mcSHOW_DBG_MSG(("\n[RX minimum per bit window bypass calibration]\n"));
 	}
    else
#endif
    {
        /*
         RX_Window
             [HQALOG] 1600 RX_Window Channel0 Rank0 52(bit 2)
             [HQALOG] 1600 RX_Window Channel0 Rank1 52(bit 2)
             [HQALOG] 1600 RX_Window Channel1 Rank0 60(bit 12)
             [HQALOG] 1600 RX_Window Channel1 Rank1 62(bit 9)
         */
        mcSHOW_DBG_MSG(("RX_Window\n"));
#ifdef FOR_HQA_REPORT_USED
if (gHQALog_flag==1)
{
        for(u1ChannelIdx=CHANNEL_A; u1ChannelIdx<local_channel_num; u1ChannelIdx++)
        {
            for(u1RankIdx = RANK_0; u1RankIdx < 2; u1RankIdx++)
            {
                    HQA_Log_Message_for_Report(p, u1ChannelIdx, u1RankIdx, HQA_REPORT_FORMAT2, "RX", "_Window", 0, min_rx_value[u1ChannelIdx][u1RankIdx], NULL);
                    HQA_Log_Message_for_Report(p, u1ChannelIdx, u1RankIdx, HQA_REPORT_FORMAT2, "RX", "_Window_bit", 0, min_RX_DQ_bit[u1ChannelIdx][u1RankIdx], NULL);
            }
        }
}
else
#endif
{
        for(u1ChannelIdx=CHANNEL_A; u1ChannelIdx<local_channel_num; u1ChannelIdx++)
        {
            for(u1RankIdx = RANK_0; u1RankIdx < 2; u1RankIdx++)
            {
                HQA_LOG_Print_Prefix_String(p); mcSHOW_DBG_MSG(("RX_Window Channel%d "
                                "Rank%d %d (bit %d)\n",
                            u1ChannelIdx,
                                u1RankIdx,
                                min_rx_value[u1ChannelIdx][u1RankIdx], min_RX_DQ_bit[u1ChannelIdx][u1RankIdx]));
            }
        }
}

        /*
         RX Min Window(%)
             [HQALOG] 1600 RX_Window(%) Channel0 Rank0 43316/100ps(70%)(PASS)
             [HQALOG] 1600 RX_Window(%) Channel0 Rank1 43316/100ps(70%)(PASS)
             [HQALOG] 1600 RX_Window(%) Channel1 Rank0 49980/100ps(80%)(PASS)
             [HQALOG] 1600 RX_Window(%) Channel1 Rank1 51646/100ps(83%)(PASS)
         */
        mcSHOW_DBG_MSG(("RX Window(%%)\n"));
#ifdef FOR_HQA_REPORT_USED
if (gHQALog_flag==1)
{
        for(u1ChannelIdx=CHANNEL_A; u1ChannelIdx<local_channel_num; u1ChannelIdx++)
        {
            for(u1RankIdx = RANK_0; u1RankIdx < 2; u1RankIdx++)
            {
                    HQA_Log_Message_for_Report(p, u1ChannelIdx, u1RankIdx, HQA_REPORT_FORMAT2, "RX", "_Window(%)", 0, ((min_rx_value[u1ChannelIdx][u1RankIdx] * gHQALOG_RX_delay_cell_ps_075V * p->frequency * 2) + (1000000 - 1)) / 1000000, NULL);
                    HQA_Log_Message_for_Report(p, u1ChannelIdx, u1RankIdx, HQA_REPORT_FORMAT4, "RX", "_Window_PF", 0, 0, (min_rx_value[u1ChannelIdx][u1RankIdx] * gHQALOG_RX_delay_cell_ps_075V * p->frequency * 2) / 1000000 >= 40 ? "PASS" : "FAIL");
            }
        }
}
else
#endif
{
        for(u1ChannelIdx=CHANNEL_A; u1ChannelIdx<local_channel_num; u1ChannelIdx++)
        {
            for(u1RankIdx = RANK_0; u1RankIdx < 2; u1RankIdx++)
            {
                HQA_LOG_Print_Prefix_String(p); mcSHOW_DBG_MSG(("RX_Window(%%) Channel%d "
                                "Rank%d "
                                "%d/100ps (%d%%) (%s)\n",
                                u1ChannelIdx,
                                u1RankIdx,
                                min_rx_value[u1ChannelIdx][u1RankIdx] * gHQALOG_RX_delay_cell_ps_075V,
                                ((min_rx_value[u1ChannelIdx][u1RankIdx] * gHQALOG_RX_delay_cell_ps_075V * p->frequency * 2) + (1000000 - 1)) / 1000000,
                                ((min_rx_value[u1ChannelIdx][u1RankIdx] * gHQALOG_RX_delay_cell_ps_075V * p->frequency * 2) + (1000000 - 1)) / 1000000 >= 40 ? "PASS" : "FAIL"));
            }
        }
}

        mcSHOW_DBG_MSG(("\n"));
    }






    /* [TX minimum per bit window]
     VrefDQ Range : 1
     VrefDQ
         [HQALOG] 1600 VrefTX Channel0 Rank0 30
         [HQALOG] 1600 VrefTX Channel0 Rank1 25
         [HQALOG] 1600 VrefTX Channel1 Rank0 24
         [HQALOG] 1600 VrefTX Channel1 Rank1 23
     */
    mcSHOW_DBG_MSG(("\n[TX minimum per bit window]\n"));
    if(u1IsLP4Family(p->dram_type))
    {
        if (Get_Vref_Calibration_OnOff(p)==VREF_CALI_ON)
        {
            mcSHOW_DBG_MSG(("VrefDQ Range : %d\n",(u1MR14Value[p->channel][p->rank][p->dram_fsp]>>6)&1));
            mcSHOW_DBG_MSG(("VrefDQ\n"));
            for(u1ChannelIdx=CHANNEL_A; u1ChannelIdx<local_channel_num; u1ChannelIdx++)
            {
                for(u1RankIdx = RANK_0; u1RankIdx < 2; u1RankIdx++)
                {
                    HQA_LOG_Print_Prefix_String(p); mcSHOW_DBG_MSG(("VrefDQ Channel%d "
                                    "Rank%d %d\n",
                                    u1ChannelIdx,
                                    u1RankIdx,
                                    gFinalTXVrefDQ[u1ChannelIdx][u1RankIdx]));
                }
            }
        }
        else
        {
            mcSHOW_DBG_MSG(("TX DQ Vref Scan : Disable\n"));
        }
    }
#if 0//(SUPPORT_SAVE_TIME_FOR_CALIBRATION )
    if(p->femmc_Ready==1 && (p->Bypass_TXWINDOW))
 	{
        mcSHOW_DBG_MSG(("\n[TX minimum per bit window bypass calibration]\n"));
 	}
    else
#endif
    {
        /*
         TX_Window
             [HQALOG] 1600 TX_Window Channel0 Rank0 25(bit 2)
             [HQALOG] 1600 TX_Window Channel0 Rank1 25(bit 2)
             [HQALOG] 1600 TX_Window Channel1 Rank0 22(bit 9)
             [HQALOG] 1600 TX_Window Channel1 Rank1 23(bit 9)
         */
        mcSHOW_DBG_MSG(("TX_Window\n"));
#ifdef FOR_HQA_REPORT_USED
if (gHQALog_flag==1)
{
        for(u1ChannelIdx=CHANNEL_A; u1ChannelIdx<local_channel_num; u1ChannelIdx++)
        {
            for(u1RankIdx = RANK_0; u1RankIdx < 2; u1RankIdx++)
            {
                    HQA_Log_Message_for_Report(p, u1ChannelIdx, u1RankIdx, HQA_REPORT_FORMAT2, "TX", "_Window", 0, gFinalTXPerbitWin_min_max[u1ChannelIdx][u1RankIdx], NULL);
                    HQA_Log_Message_for_Report(p, u1ChannelIdx, u1RankIdx, HQA_REPORT_FORMAT2, "TX", "_Window_bit", 0, min_TX_DQ_bit[u1ChannelIdx][u1RankIdx], NULL);
            }
        }
}
else
#endif
{
        for(u1ChannelIdx=CHANNEL_A; u1ChannelIdx<local_channel_num; u1ChannelIdx++)
        {
            for(u1RankIdx = RANK_0; u1RankIdx < 2; u1RankIdx++)
            {
                HQA_LOG_Print_Prefix_String(p); mcSHOW_DBG_MSG(("TX_Window Channel%d "
                                "Rank%d %d (bit %d)\n",
                                u1ChannelIdx,
                                u1RankIdx,
                                gFinalTXPerbitWin_min_max[u1ChannelIdx][u1RankIdx], min_TX_DQ_bit[u1ChannelIdx][u1RankIdx]));
            }
        }
}
#if 0//(TX_PER_BIT_DELAY_CELL==0)
        mcSHOW_DBG_MSG(("min DQ margin\n"));
        for(u1ChannelIdx=CHANNEL_A; u1ChannelIdx<local_channel_num; u1ChannelIdx++)
        {
            for(u1RankIdx = RANK_0; u1RankIdx < 2; u1RankIdx++)
            {
                HQA_LOG_Print_Prefix_String(p); mcSHOW_DBG_MSG(("min_DQ_margin Channel%d "
                                "Rank%d %d (bit %d)\n",
                                u1ChannelIdx,
                                u1RankIdx,
                                gFinalTXPerbitWin_min_margin[u1ChannelIdx][u1RankIdx], gFinalTXPerbitWin_min_margin_bit[u1ChannelIdx][u1RankIdx]));
            }
        }
#endif


        /*
         TX Min Window(%)
             [HQALOG] 1600 TX_Window(%) Channel0 Rank0 79%(PASS)
             [HQALOG] 1600 TX_Window(%) Channel0 Rank1 79%(PASS)
             [HQALOG] 1600 TX_Window(%) Channel1 Rank0 69%(PASS)
             [HQALOG] 1600 TX_Window(%) Channel1 Rank1 72%(PASS)
         */
        mcSHOW_DBG_MSG(("TX Min Window(%%)\n"));
#ifdef FOR_HQA_REPORT_USED
if (gHQALog_flag==1)
{
        for(u1ChannelIdx=CHANNEL_A; u1ChannelIdx<local_channel_num; u1ChannelIdx++)
        {
            for(u1RankIdx = RANK_0; u1RankIdx < 2; u1RankIdx++)
            {
                /*                    
                                Item Data Rate Mode Status 
                                1 DDR1600 1:4 mode Done 
                                2 DDR1200 1:4 mode Done 
                                3 DDR800 1:4 mode 1UI=64PI special case
                */
                HQA_Log_Message_for_Report(p, u1ChannelIdx, u1RankIdx, HQA_REPORT_FORMAT2, "TX", "_Window(%)", 0, (min_tx_value[u1ChannelIdx][u1RankIdx] * 100 + (vGet_DDR800_Mode(p) == DDR800_CLOSE_LOOP ? 63 : 31)) / (vGet_DDR800_Mode(p) == DDR800_CLOSE_LOOP ? 64 : 32), NULL);             
                HQA_Log_Message_for_Report(p, u1ChannelIdx, u1RankIdx, HQA_REPORT_FORMAT4, "TX", "_Window_PF", 0, 0, (min_tx_value[u1ChannelIdx][u1RankIdx] * 100 + (vGet_DDR800_Mode(p) == DDR800_CLOSE_LOOP ? 63 : 31)) / (vGet_DDR800_Mode(p) == DDR800_CLOSE_LOOP ? 64 : 32) >= 45 ? "PASS" : "FAIL");
            }
        }
}
else
#endif
{
        for(u1ChannelIdx=CHANNEL_A; u1ChannelIdx<local_channel_num; u1ChannelIdx++)
        {
            for(u1RankIdx = RANK_0; u1RankIdx < 2; u1RankIdx++)
            {
                /*                    
                                Item Data Rate Mode Status 
                                1 DDR1600 1:4 mode Done 
                                2 DDR1200 1:4 mode Done 
                                3 DDR800 1:4 mode 1UI=64PI special case
                */            
                HQA_LOG_Print_Prefix_String(p); mcSHOW_DBG_MSG(("TX_Window(%%) Channel%d "
                                "Rank%d %d%% (%s)\n",
                                u1ChannelIdx,
                                u1RankIdx,
                               (min_tx_value[u1ChannelIdx][u1RankIdx] * 100 + (vGet_DDR800_Mode(p) == DDR800_CLOSE_LOOP ? 63 : 31)) / (vGet_DDR800_Mode(p) == DDR800_CLOSE_LOOP ? 64 : 32),
                               (min_tx_value[u1ChannelIdx][u1RankIdx] * 100 + (vGet_DDR800_Mode(p) == DDR800_CLOSE_LOOP ? 63 : 31)) / (vGet_DDR800_Mode(p) == DDR800_CLOSE_LOOP ? 64 : 32) >= 45 ? "PASS" : "FAIL"));
            }
        }
}

        mcSHOW_DBG_MSG(("\n"));
    }



        /*
            [Duty Calibration]
            CLK Duty Final Delay Cell
            [HQALOG] DUTY CLK_Final_Delay Channel0 0
            [HQALOG] DUTY CLK_Final_Delay Channel1 -2    
        */    
#if !defined(RELEASE) && (VENDER_JV_LOG==0)        
    if (u1IsLP4Family(p->dram_type) && (vGet_Duty_Calibration_Mode(p) != DUTY_DEFAULT))
    {
        mcSHOW_DBG_MSG(("[duty Calibration]\n"));
        mcSHOW_DBG_MSG(("CLK Duty Final Delay Cell\n"));
        for(u1ChannelIdx=CHANNEL_A; u1ChannelIdx<local_channel_num; u1ChannelIdx++)
        {
                     HQA_LOG_Print_Prefix_String(p); mcSHOW_DBG_MSG(("DUTY CLK_Final_Delay Channel%d %d\n", u1ChannelIdx, gFinalClkDuty[u1ChannelIdx]));
        }

        /*
            CLK Duty MAX
            [HQALOG] DUTY CLK_MAX Channel0 4765%(X100)
            [HQALOG] DUTY CLK_MAX Channel1 5212%(X100)
        */
        mcSHOW_DBG_MSG(("CLK Duty MAX\n"));
        for(u1ChannelIdx=CHANNEL_A; u1ChannelIdx<local_channel_num; u1ChannelIdx++)
        {
#ifdef FOR_HQA_REPORT_USED
                if (gHQALog_flag==1)
                {
                    HQA_Log_Message_for_Report(p, u1ChannelIdx, 0, HQA_REPORT_FORMAT3, "DUTY CLK_", "MAX", 0, gFinalClkDutyMinMax[u1ChannelIdx][1], NULL);
                }
                else
#endif
                {
                    HQA_LOG_Print_Prefix_String(p); mcSHOW_DBG_MSG(("DUTY CLK_MAX Channel%d %d%%(X100)\n", u1ChannelIdx, gFinalClkDutyMinMax[u1ChannelIdx][1]));
                }
        }

        /*
            CLK Duty MIN
            [HQALOG] DUTY CLK_MIN Channel0 4565%(X100)
            [HQALOG] DUTY CLK_MIN Channel1 5012%(X100)
        */
        mcSHOW_DBG_MSG(("CLK Duty MIN\n"));
        for(u1ChannelIdx=CHANNEL_A; u1ChannelIdx<local_channel_num; u1ChannelIdx++)
        {
#ifdef FOR_HQA_REPORT_USED
            if (gHQALog_flag==1)
            {
                HQA_Log_Message_for_Report(p, u1ChannelIdx, 0, HQA_REPORT_FORMAT3, "DUTY CLK_", "MIN", 0, gFinalClkDutyMinMax[u1ChannelIdx][0], NULL);
                HQA_Log_Message_for_Report(p, u1ChannelIdx, 0, HQA_REPORT_FORMAT3, "DUTY CLK_", "MAX-MIN", 0, gFinalClkDutyMinMax[u1ChannelIdx][1]-gFinalClkDutyMinMax[u1ChannelIdx][0], NULL);
            }
            else
#endif
            {
                HQA_LOG_Print_Prefix_String(p); mcSHOW_DBG_MSG(("DUTY CLK_MIN Channel%d %d%%(X100)\n", u1ChannelIdx, gFinalClkDutyMinMax[u1ChannelIdx][0]));
            }
        }

        mcSHOW_DBG_MSG(("\n"));
    }




    /*
        DQS Duty Final Delay Cell
        [HQALOG] DUTY DQS_Final_Delay Channel0 DQS0 0
        [HQALOG] DUTY DQS_Final_Delay Channel0 DQS1 1
        [HQALOG] DUTY DQS_Final_Delay Channel1 DQS0 -2
        [HQALOG] DUTY DQS_Final_Delay Channel1 DQS1 -1
    */
    if (u1IsLP4Family(p->dram_type) && (vGet_Duty_Calibration_Mode(p) != DUTY_DEFAULT))
    {
        mcSHOW_DBG_MSG(("DQS Duty Final Delay Cell\n"));
        for(u1ChannelIdx=CHANNEL_A; u1ChannelIdx<local_channel_num; u1ChannelIdx++)
        {
            HQA_LOG_Print_Prefix_String(p); mcSHOW_DBG_MSG(("DUTY DQS_Final_Delay Channel%d DQS0 %d\n", u1ChannelIdx, gFinalDQSDuty[u1ChannelIdx][0]));
            HQA_LOG_Print_Prefix_String(p); mcSHOW_DBG_MSG(("DUTY DQS_Final_Delay Channel%d DQS1 %d\n", u1ChannelIdx, gFinalDQSDuty[u1ChannelIdx][1]));            
        }

        /*
            DQS Duty MAX
            [HQALOG] DUTY DQS_MAX Channel0 DQS0 4765%(X100)
            [HQALOG] DUTY DQS_MAX Channel0 DQS1 5212%(X100)
            [HQALOG] DUTY DQS_MAX Channel1 DQS0 4765%(X100)
            [HQALOG] DUTY DQS_MAX Channel1 DQS1 5212%(X100)
        */
        mcSHOW_DBG_MSG(("DQS Duty MAX\n"));
        for(u1ChannelIdx=CHANNEL_A; u1ChannelIdx<local_channel_num; u1ChannelIdx++)
        {
#ifdef FOR_HQA_REPORT_USED
                if (gHQALog_flag==1)
                {
                    HQA_Log_Message_for_Report(p, u1ChannelIdx, 0, HQA_REPORT_FORMAT0, "", "DUTY DQS_MAX", 0, gFinalDQSDutyMinMax[u1ChannelIdx][0][1], NULL);
                    HQA_Log_Message_for_Report(p, u1ChannelIdx, 0, HQA_REPORT_FORMAT0, "", "DUTY DQS_MAX", 1, gFinalDQSDutyMinMax[u1ChannelIdx][1][1], NULL);
                }
                else
#endif
                {
                    HQA_LOG_Print_Prefix_String(p); mcSHOW_DBG_MSG(("DUTY DQS_MAX Channel%d DQS0 %d%%(X100)\n", u1ChannelIdx, gFinalDQSDutyMinMax[u1ChannelIdx][0][1]));
                    HQA_LOG_Print_Prefix_String(p); mcSHOW_DBG_MSG(("DUTY DQS_MAX Channel%d DQS1 %d%%(X100)\n", u1ChannelIdx, gFinalDQSDutyMinMax[u1ChannelIdx][1][1]));            
                }
        }

        /*
            DQS Duty MIN
            [HQALOG] DUTY DQS_MIN Channel0 DQS0 4765%(X100)
            [HQALOG] DUTY DQS_MIN Channel0 DQS1 5212%(X100)
            [HQALOG] DUTY DQS_MIN Channel1 DQS0 4765%(X100)
            [HQALOG] DUTY DQS_MIN Channel1 DQS1 5212%(X100)
        */
        mcSHOW_DBG_MSG(("DQS Duty MIN\n"));
        for(u1ChannelIdx=CHANNEL_A; u1ChannelIdx<local_channel_num; u1ChannelIdx++)
        {
#ifdef FOR_HQA_REPORT_USED
            if (gHQALog_flag==1)
            {
                HQA_Log_Message_for_Report(p, u1ChannelIdx, 0, HQA_REPORT_FORMAT0, "DUTY DQS_", "MIN", 0, gFinalDQSDutyMinMax[u1ChannelIdx][0][0], NULL);
                HQA_Log_Message_for_Report(p, u1ChannelIdx, 0, HQA_REPORT_FORMAT0, "DUTY DQS_", "MIN", 1, gFinalDQSDutyMinMax[u1ChannelIdx][1][0], NULL);
                HQA_Log_Message_for_Report(p, u1ChannelIdx, 0, HQA_REPORT_FORMAT0, "DUTY DQS_", "MAX-MIN", 0, gFinalDQSDutyMinMax[u1ChannelIdx][0][1]-gFinalDQSDutyMinMax[u1ChannelIdx][0][0], NULL);
                HQA_Log_Message_for_Report(p, u1ChannelIdx, 0, HQA_REPORT_FORMAT0, "DUTY DQS_", "MAX-MIN", 1, gFinalDQSDutyMinMax[u1ChannelIdx][1][1]-gFinalDQSDutyMinMax[u1ChannelIdx][1][0], NULL);
            }
            else
#endif
            {
                HQA_LOG_Print_Prefix_String(p); mcSHOW_DBG_MSG(("DUTY DQS_MIN Channel%d DQS0 %d%%(X100)\n", u1ChannelIdx, gFinalDQSDutyMinMax[u1ChannelIdx][0][0]));
                HQA_LOG_Print_Prefix_String(p); mcSHOW_DBG_MSG(("DUTY DQS_MIN Channel%d DQS1 %d%%(X100)\n", u1ChannelIdx, gFinalDQSDutyMinMax[u1ChannelIdx][1][0]));            
            }
        }

        mcSHOW_DBG_MSG(("\n"));
    }
#endif











    #if defined(ENABLE_MIOCK_JMETER) && !defined(RELEASE)
    //if(p->frequency == u2DFSGetHighestFreq(p))
    {
        if(u1IsLP4Family(p->dram_type))	//LP4 Series
        {
            mcSHOW_DBG_MSG(("\n[DramcMiockJmeter]\n"
                        "Channel\tVCORE\t\t1 delay cell\n"));

            for(shuffle_index=DRAM_DFS_SHUFFLE_1; shuffle_index<DRAM_DFS_SHUFFLE_MAX; shuffle_index++)
            {
                mcSHOW_DBG_MSG(("\nSHUFFLE %d\n", shuffle_index+1));

                for (u1ChannelIdx = 0; u1ChannelIdx < local_channel_num; u1ChannelIdx++)
                {
                    mcSHOW_DBG_MSG(("CH%d\t%d\t\t%d/100 ps\n", u1ChannelIdx, u4gVcore[shuffle_index], u2gdelay_cell_ps_all[shuffle_index][u1ChannelIdx]));
                }
            }
        }
    }
    #endif

    mcSHOW_DBG_MSG(("\n\n\n"));



#if VENDER_JV_LOG
    mcSHOW_DBG_MSG5(("\n\n\n[Summary] information for measurement\n"));
    //mcSHOW_DBG_MSG5(("\tDram Data rate = ")); HQA_LOG_Print_Freq_String(p); mcSHOW_DBG_MSG(("\n"));
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
     if (Get_Vref_Calibration_OnOff(p)==VREF_CALI_ON)
     {
#if CHANNEL_NUM==4
        mcSHOW_DBG_MSG5(("CHA_VrefDQ    CHB_VrefDQ      CHC_VrefDQ      CHD_VrefDQ\n"));
        mcSHOW_DBG_MSG5(("B0 %d            %d              %d              %d \n", gFinalRXVrefDQ[CHANNEL_A][RANK_0][0], gFinalRXVrefDQ[CHANNEL_B][RANK_0][0], gFinalRXVrefDQ[CHANNEL_C][RANK_0][0], gFinalRXVrefDQ[CHANNEL_D][RANK_0][0]));
        mcSHOW_DBG_MSG5(("B1 %d            %d              %d              %d \n", gFinalRXVrefDQ[CHANNEL_A][RANK_0][1], gFinalRXVrefDQ[CHANNEL_B][RANK_0][1], gFinalRXVrefDQ[CHANNEL_C][RANK_0][1], gFinalRXVrefDQ[CHANNEL_D][RANK_0][1]));
#else
         mcSHOW_DBG_MSG5(("CHA_VrefDQ   CHB_VrefDQ\n"));
         mcSHOW_DBG_MSG5(("B0 %d           %d \n", gFinalRXVrefDQ[CHANNEL_A][RANK_0][0], gFinalRXVrefDQ[CHANNEL_B][RANK_0][0]));
         mcSHOW_DBG_MSG5(("B1 %d           %d \n", gFinalRXVrefDQ[CHANNEL_A][RANK_0][1], gFinalRXVrefDQ[CHANNEL_B][RANK_0][1]));
#endif
     }
     else
     {
         mcSHOW_DBG_MSG5(("RX DQ Vref Scan : Disable\n"));

     }

    if(u1IsLP4Family(p->dram_type))
    {
#if CHANNEL_NUM==4
    mcSHOW_DBG_MSG5(("CHA_Rank0           CHA_Rank1           CHB_Rank0           CHB_Rank1         CHC_Rank0       CHC_Rank1       CHD_Rank0       CHD_Rank1\n"));
    mcSHOW_DBG_MSG5(("%d%%(bit %d)         %d%%(bit %d)         %d%%(bit %d)         %d%%(bit %d)         %d%%(bit %d)         %d%%(bit %d)         %d%%(bit %d)         %d%%(bit %d)\n\n",
                                    ((min_rx_value[0][0]*gHQALOG_RX_delay_cell_ps_075V*p->frequency*2)+(1000000-1))/1000000, min_RX_DQ_bit[0][0],
                                    ((min_rx_value[0][1]*gHQALOG_RX_delay_cell_ps_075V*p->frequency*2)+(1000000-1))/1000000, min_RX_DQ_bit[0][1],
                                    ((min_rx_value[1][0]*gHQALOG_RX_delay_cell_ps_075V*p->frequency*2)+(1000000-1))/1000000, min_RX_DQ_bit[1][0],
                                    ((min_rx_value[1][1]*gHQALOG_RX_delay_cell_ps_075V*p->frequency*2)+(1000000-1))/1000000, min_RX_DQ_bit[1][1],
                                    ((min_rx_value[2][0]*gHQALOG_RX_delay_cell_ps_075V*p->frequency*2)+(1000000-1))/1000000, min_RX_DQ_bit[2][0],
                                    ((min_rx_value[2][1]*gHQALOG_RX_delay_cell_ps_075V*p->frequency*2)+(1000000-1))/1000000, min_RX_DQ_bit[2][1],
                                    ((min_rx_value[3][0]*gHQALOG_RX_delay_cell_ps_075V*p->frequency*2)+(1000000-1))/1000000, min_RX_DQ_bit[3][0],
                                    ((min_rx_value[3][1]*gHQALOG_RX_delay_cell_ps_075V*p->frequency*2)+(1000000-1))/1000000, min_RX_DQ_bit[3][1]));
#else
     mcSHOW_DBG_MSG5(("CHA_Rank0           CHA_Rank1           CHB_Rank0           CHB_Rank1\n"));
     mcSHOW_DBG_MSG5(("%d%%(bit %d)         %d%%(bit %d)         %d%%(bit %d)         %d%%(bit %d)\n\n",
                                     ((min_rx_value[0][0]*gHQALOG_RX_delay_cell_ps_075V*p->frequency*2)+(1000000-1))/1000000, min_RX_DQ_bit[0][0],
                                     ((min_rx_value[0][1]*gHQALOG_RX_delay_cell_ps_075V*p->frequency*2)+(1000000-1))/1000000, min_RX_DQ_bit[0][1],
                                     ((min_rx_value[1][0]*gHQALOG_RX_delay_cell_ps_075V*p->frequency*2)+(1000000-1))/1000000, min_RX_DQ_bit[1][0],
                                     ((min_rx_value[1][1]*gHQALOG_RX_delay_cell_ps_075V*p->frequency*2)+(1000000-1))/1000000, min_RX_DQ_bit[1][1]));
#endif
    }
#if ENABLE_LP3_SW     
    else
    {
        mcSHOW_DBG_MSG5(("CHA_Rank0           CHA_Rank1\n"));
        mcSHOW_DBG_MSG5(("%d%%(bit %d)         %d%%(bit %d)\n\n",
                                        ((min_rx_value[0][0]*u2gdelay_cell_ps_all[shuffleIdx][0]*p->frequency*2)+(1000000-1))/1000000, min_RX_DQ_bit[0][0],
                                        ((min_rx_value[0][1]*u2gdelay_cell_ps_all[shuffleIdx][0]*p->frequency*2)+(1000000-1))/1000000, min_RX_DQ_bit[0][1]));    
    }
#endif

     mcSHOW_DBG_MSG5(("[TX minimum per bit window]\n"));
     if (Get_Vref_Calibration_OnOff(p)==VREF_CALI_ON)
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

    if(u1IsLP4Family(p->dram_type))
    {
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
    }
    else
    {
     mcSHOW_DBG_MSG5(("CHA_Rank0           CHA_Rank1\n"));
     mcSHOW_DBG_MSG5(("%d%%                %d%%\n",
                                         (min_tx_value[0][0]*100+31)/32,
                                         (min_tx_value[0][1]*100+31)/32));    
    }
#endif


    // reset all data
    HQA_measure_message_reset_all_data(p);
}
#ifdef RELEASE
#undef mcSHOW_DBG_MSG
#define mcSHOW_DBG_MSG(_x_)
#endif
#endif


#ifdef FOR_HQA_REPORT_USED
void print_EyeScanVcent_for_HQA_report_used(DRAMC_CTX_T *p, U8 print_type, U8 u1ChannelIdx, U8 u1RankIdx, U8 u1ByteIdx, U8 *EyeScanVcent, U8 EyeScanVcentUpperBound, U8 EyeScanVcentUpperBound_bit, U8 EyeScanVcentLowerBound, U8 EyeScanVcentLowerBound_bit)
{
    U8 uiCA, u1BitIdx;
    U16 *pVref_Voltage_Table[VREF_VOLTAGE_TABLE_NUM]={0};
    U8  VrefRange;
    U32 vddq=0;
    U8 local_channel_num=2;
    U8 shuffleIdx;
    U8 u1CBTEyeScanEnable, u1RXEyeScanEnable, u1TXEyeScanEnable;
    U16 Final_Vref_Vcent, VdlVWHigh_Upper_Vcent, VdlVWHigh_Lower_Vcent;
    U32 Vcent_UpperBound_Window_percent=0, Vcent_LowerBound_Window_percent=0;
    U32 Perbit_Window_percent, Perbit_BestWindow_percent, Perbit_Window_Upperbond_percent, Perbit_Window_Lowerbond_percent, Perbit_Eye_Height, Perbit_Eye_Area;
    U8 u1BitIndex_Begin=0, u1BitIndex_End=p->data_width;
    
    if (gHQALog_flag==0)
    {
	mcSHOW_DBG_MSG(("\n"));
        return;
    }

    if(u1IsLP4Family(p->dram_type))
    {
        local_channel_num = p->support_channel_num;
    }
    else
    {
        //LP3
        local_channel_num = 1;
    }

    u1CBTEyeScanEnable = (gCBT_EYE_Scan_flag==1 && ((gCBT_EYE_Scan_only_higheset_freq_flag==1 && p->frequency == u2DFSGetHighestFreq(p)) || gCBT_EYE_Scan_only_higheset_freq_flag==0));
    u1RXEyeScanEnable = (gRX_EYE_Scan_flag==1 && ((gRX_EYE_Scan_only_higheset_freq_flag==1 && p->frequency == u2DFSGetHighestFreq(p)) || gRX_EYE_Scan_only_higheset_freq_flag==0));
    u1TXEyeScanEnable = (gTX_EYE_Scan_flag==1 && ((gTX_EYE_Scan_only_higheset_freq_flag==1 && p->frequency == u2DFSGetHighestFreq(p)) || gTX_EYE_Scan_only_higheset_freq_flag==0));

    shuffleIdx = get_shuffleIndex_by_Freq(p);

    if (print_type==1)
    {
#if ENABLE_EYESCAN_GRAPH 
        pVref_Voltage_Table[VREF_RANGE_0]= (U16 *)gRXVref_Voltage_Table_LP4;
#endif

        u1BitIndex_Begin = u1ByteIdx*DQS_BIT_NUMBER;
        u1BitIndex_End = u1ByteIdx*DQS_BIT_NUMBER+DQS_BIT_NUMBER;
    }
    else
    {   
        if (u1CBTEyeScanEnable || u1TXEyeScanEnable)
        {
            vddq=vGetVoltage(p, 2)/1000; //mv

#if ENABLE_EYESCAN_GRAPH 
            if (p->dram_type == TYPE_LPDDR4)
            {
                pVref_Voltage_Table[VREF_RANGE_0] = (U16 *)gVref_Voltage_Table_LP4[VREF_RANGE_0];
                pVref_Voltage_Table[VREF_RANGE_1] = (U16 *)gVref_Voltage_Table_LP4[VREF_RANGE_1];
            }
            if (p->dram_type == TYPE_LPDDR4X)
            {
                pVref_Voltage_Table[VREF_RANGE_0] = (U16 *)gVref_Voltage_Table_LP4X[VREF_RANGE_0];
                pVref_Voltage_Table[VREF_RANGE_1] = (U16 *)gVref_Voltage_Table_LP4X[VREF_RANGE_1];
            }
#endif    
        }
    }

	if (print_type==0)
	{
        if (u1CBTEyeScanEnable)
        {
            VrefRange = (u1MR12Value[p->channel][p->rank][p->dram_fsp]>>6)&1;

            Vcent_UpperBound_Window_percent = (EyeScanVcentUpperBound * 100 + (u1IsLP4Family(p->dram_type)==1?63:31)) / (u1IsLP4Family(p->dram_type)==1?64:32);
            Vcent_LowerBound_Window_percent = (EyeScanVcentLowerBound * 100 + (u1IsLP4Family(p->dram_type)==1?63:31)) / (u1IsLP4Family(p->dram_type)==1?64:32);
        }
    }

    if (print_type==1)
    {
        if (u1RXEyeScanEnable)
        {
#if ENABLE_EYESCAN_GRAPH 
            Final_Vref_Vcent = gRXVref_Voltage_Table_LP4[EyeScanVcent[1]];
            VdlVWHigh_Upper_Vcent = gRXVref_Voltage_Table_LP4[EyeScanVcent[3]];
            VdlVWHigh_Lower_Vcent = gRXVref_Voltage_Table_LP4[EyeScanVcent[5]];
#endif

            Vcent_UpperBound_Window_percent = ((EyeScanVcentUpperBound * gHQALOG_RX_delay_cell_ps_075V * p->frequency * 2) + (1000000 - 1)) / 1000000;
            Vcent_LowerBound_Window_percent = ((EyeScanVcentLowerBound * gHQALOG_RX_delay_cell_ps_075V * p->frequency * 2) + (1000000 - 1)) / 1000000;
        }
    }

	if (print_type==2)
	{
        if (u1TXEyeScanEnable)
        {
            VrefRange = (u1MR14Value[p->channel][p->rank][p->dram_fsp]>>6)&1;
        
            Final_Vref_Vcent = pVref_Voltage_Table[EyeScanVcent[0]][EyeScanVcent[1]]*vddq/100;
            VdlVWHigh_Upper_Vcent = pVref_Voltage_Table[EyeScanVcent[2]][EyeScanVcent[3]]*vddq/100;
            VdlVWHigh_Lower_Vcent = pVref_Voltage_Table[EyeScanVcent[4]][EyeScanVcent[5]]*vddq/100;

            Vcent_UpperBound_Window_percent = (EyeScanVcentUpperBound * 100 + (u1IsLP4Div4DDR800(p) == TRUE ? 63 : 31)) / (u1IsLP4Div4DDR800(p) == TRUE ? 64 : 32);
            Vcent_LowerBound_Window_percent = (EyeScanVcentLowerBound * 100 + (u1IsLP4Div4DDR800(p) == TRUE ? 63 : 31)) / (u1IsLP4Div4DDR800(p) == TRUE ? 64 : 32);
        }
   }

    mcSHOW_DBG_MSG(("\n\n\n[HQA] information for measurement, "));
    mcSHOW_DBG_MSG(("\tDram Data rate = ")); HQA_LOG_Print_Freq_String(p); mcSHOW_DBG_MSG(("\n"));

#if ENABLE_EYESCAN_GRAPH 
    mcSHOW_DBG_MSG(("%s Eye Scan Vcent Voltage\n", print_EYESCAN_LOG_type(print_type)));
    if (print_type==1)
    {
            HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT2_1, print_EYESCAN_LOG_type(print_type), "_Final_Vref Vcent", u1ByteIdx, pVref_Voltage_Table[0][EyeScanVcent[1]], NULL);
            HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT2_1, print_EYESCAN_LOG_type(print_type), "_VdlVWHigh_Upper Vcent", u1ByteIdx, pVref_Voltage_Table[0][EyeScanVcent[3]], NULL);
            HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT2_1, print_EYESCAN_LOG_type(print_type), "_VdlVWHigh_Lower Vcent", u1ByteIdx, pVref_Voltage_Table[0][EyeScanVcent[5]], NULL);    
    }
    else
    {
            HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT2, print_EYESCAN_LOG_type(print_type), "_Final_Vref Vcent", 0, pVref_Voltage_Table[EyeScanVcent[0]][EyeScanVcent[1]]*vddq/100, NULL);
            HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT2, print_EYESCAN_LOG_type(print_type), "_VdlVWHigh_Upper Vcent", 0, pVref_Voltage_Table[EyeScanVcent[2]][EyeScanVcent[3]]*vddq/100, NULL);
            HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT2, print_EYESCAN_LOG_type(print_type), "_VdlVWHigh_Lower Vcent", 0, pVref_Voltage_Table[EyeScanVcent[4]][EyeScanVcent[5]]*vddq/100, NULL);
    }

    mcSHOW_DBG_MSG(("\n"));
#endif    

    mcSHOW_DBG_MSG(("%s Eye Scan Vcent_UpperBound window\n", print_EYESCAN_LOG_type(print_type)));
            HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT2, print_EYESCAN_LOG_type(print_type), "_Vcent_UpperBound_Window", 0, EyeScanVcentUpperBound, NULL);
    mcSHOW_DBG_MSG(("%s Eye Scan Vcent_UpperBound_Window worse bit\n", print_EYESCAN_LOG_type(print_type)));
            HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT2, print_EYESCAN_LOG_type(print_type), "_Vcent_UpperBound_Window_bit", 0, EyeScanVcentUpperBound_bit, NULL);
    mcSHOW_DBG_MSG(("%s Eye Scan Vcent_UpperBound Min Window(%%)\n", print_EYESCAN_LOG_type(print_type)));
            HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT2, print_EYESCAN_LOG_type(print_type), "_Vcent_UpperBound_Window(%)", 0, Vcent_UpperBound_Window_percent, NULL);

    mcSHOW_DBG_MSG(("\n"));

    mcSHOW_DBG_MSG(("%s Eye Scan Vcent_LowerBound window\n", print_EYESCAN_LOG_type(print_type)));
            HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT2, print_EYESCAN_LOG_type(print_type), "_Vcent_LowerBound_Window", 0, EyeScanVcentLowerBound, NULL);
    mcSHOW_DBG_MSG(("%s Eye Scan Vcent_LowerBound_Window worse bit\n", print_EYESCAN_LOG_type(print_type)));
            HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT2, print_EYESCAN_LOG_type(print_type), "_Vcent_LowerBound_Window_bit", 0, EyeScanVcentLowerBound_bit, NULL);
    mcSHOW_DBG_MSG(("%s Eye Scan Vcent_UpperBound Min Window(%%)\n", print_EYESCAN_LOG_type(print_type)));
            HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT2, print_EYESCAN_LOG_type(print_type), "_Vcent_LowerBound_Window(%)", 0, Vcent_LowerBound_Window_percent, NULL);

    if (print_type==1 || print_type==2)
    {
        mcSHOW_DBG_MSG(("\n"));
        mcSHOW_DBG_MSG(("%s Eye Scan per_bit window(%%)\n", print_EYESCAN_LOG_type(print_type)));
        for (u1BitIdx=u1BitIndex_Begin; u1BitIdx<u1BitIndex_End; u1BitIdx++)
        {
#if ENABLE_EYESCAN_GRAPH 
            if (print_type==1)
            {
                Perbit_Window_percent = ((gEyeScan_WinSize[EyeScanVcent[1]/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx] * gHQALOG_RX_delay_cell_ps_075V * p->frequency * 2) + (1000000 - 1)) / 1000000;
                Perbit_BestWindow_percent = ((gEyeScan_WinSize[EyeScanVcent[10+u1BitIdx]/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx] * gHQALOG_RX_delay_cell_ps_075V * p->frequency * 2) + (1000000 - 1)) / 1000000;
                Perbit_Window_Upperbond_percent = ((gEyeScan_WinSize[EyeScanVcent[3]/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx] * gHQALOG_RX_delay_cell_ps_075V * p->frequency * 2) + (1000000 - 1)) / 1000000;
                Perbit_Window_Lowerbond_percent = ((gEyeScan_WinSize[EyeScanVcent[5]/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx] * gHQALOG_RX_delay_cell_ps_075V * p->frequency * 2) + (1000000 - 1)) / 1000000;
                Perbit_Eye_Height = (gRXVref_Voltage_Table_LP4[((gEyeScan_ContinueVrefHeight[u1BitIdx]>>8) & 0xff)] - gRXVref_Voltage_Table_LP4[(gEyeScan_ContinueVrefHeight[u1BitIdx] & 0xff)])/100; //RX vref height last - first
                Perbit_Eye_Area = gEyeScan_TotalPassCount[u1BitIdx]*gHQALOG_RX_delay_cell_ps_075V*(((gRXVref_Voltage_Table_LP4[((gEyeScan_ContinueVrefHeight[u1BitIdx]>>8) & 0xff)] - gRXVref_Voltage_Table_LP4[(gEyeScan_ContinueVrefHeight[u1BitIdx] & 0xff)]))/((((gEyeScan_ContinueVrefHeight[u1BitIdx]>>8) & 0xff)-(gEyeScan_ContinueVrefHeight[u1BitIdx] & 0xff))*10))/1000; //total count*jitter metter delay cell*(1/freq*10^6 ps)*(1330mv)
            }
            else //if (print_type==2)
            {
                Perbit_Window_percent = ((gEyeScan_WinSize[(EyeScanVcent[0]*30+EyeScanVcent[1])/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx]) * 100 + 31) / 32;
                Perbit_BestWindow_percent = ((gEyeScan_WinSize[(EyeScanVcent[10+u1BitIdx*2]*30+EyeScanVcent[10+u1BitIdx*2+1])/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx]) * 100 + 31) / 32;
                Perbit_Window_Upperbond_percent = ((gEyeScan_WinSize[(EyeScanVcent[2]*30+EyeScanVcent[3])/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx]) * 100 + 31) / 32;
                Perbit_Window_Lowerbond_percent = ((gEyeScan_WinSize[(EyeScanVcent[4]*30+EyeScanVcent[5])/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx]) * 100 + 31) / 32;
                Perbit_Eye_Height = (gEyeScan_ContinueVrefHeight[u1BitIdx]-1)*6*vddq/1000;
                Perbit_Eye_Area = (gEyeScan_TotalPassCount[u1BitIdx]*10*3*vddq/(32*DDRPhyFMeter()))*100; //total count*1/32UI*(1/freq*10^6 ps)*(0.6%vddq)            
            }

            HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT0_1, print_EYESCAN_LOG_type(print_type), "_Perbit_Window(%)", u1BitIdx, Perbit_Window_percent, NULL);
            HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT0_1, print_EYESCAN_LOG_type(print_type), "_Perbit_BestWindow(%)", u1BitIdx, Perbit_BestWindow_percent, NULL);
            HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT0_1, print_EYESCAN_LOG_type(print_type), "_Perbit_Window_Upperbond(%)", u1BitIdx, Perbit_Window_Upperbond_percent, NULL);
            HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT0_1, print_EYESCAN_LOG_type(print_type), "_Perbit_Window_Lowerbond(%) ", u1BitIdx, Perbit_Window_Lowerbond_percent, NULL);
            HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT0_1, print_EYESCAN_LOG_type(print_type), "_Perbit_Eye_Height", u1BitIdx, Perbit_Eye_Height, NULL);
            HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT0_1, print_EYESCAN_LOG_type(print_type), "_Perbit_Eye_Area", u1BitIdx, Perbit_Eye_Area, NULL);
#endif
        }   
    }
}

void HQA_Log_Message_for_Report(DRAMC_CTX_T *p, U8 u1ChannelIdx, U8 u1RankIdx, U32 who_am_I, U8 *main_str, U8 *main_str2, U8 byte_bit_idx, S32 value1, U8 *ans_str)
{
    // HQA_REPORT_FORMAT0 : [HQALOG] 3200 Gating_Center_2T Channel0 Rank0 Byte0 3
    // HQA_REPORT_FORMAT0_1:[HQALOG] 3200 Gating_Center_2T Channel0 Rank0 Bit0 3
    // HQA_REPORT_FORMAT0_2:[HQALOG] 3200 Gating_Center_2T Channel0 Rank0 CA0 3    
    // HQA_REPORT_FORMAT1 : [HQALOG] 3200 WriteLeveling_DQS0 Channel0 Rank0 35
    // HQA_REPORT_FORMAT2 : [HQALOG] 3200 TX_Final_Vref Vcent Channel0 Rank0 16860
    // HQA_REPORT_FORMAT2_1:[HQALOG] 3200 RX_Final_Vref Vcent Channel0 Rank0 B0 16860    
    // HQA_REPORT_FORMAT3 : [HQALOG] 3200 DUTY CLK_MAX Channel0 5171
    // HQA_REPORT_FORMAT4 : [HQALOG] 3200 TX_Vcent_LowerBound_Window_PF Channel0 Rank0 PASS
    // HQA_REPORT_FORMAT5 : [HQALOG] 3200 AAAAAAAAAAAA BBBBB
    // HQA_REPORT_FORMAT6 : [HQALOG] 3200 AAAAAAAAAAAA 0

    if (gHQALog_flag==1)
    {
        HQA_LOG_Print_Prefix_String(p); mcSHOW_DBG_MSG(("%s%s", main_str, main_str2));
        switch (who_am_I)
        {
    		case HQA_REPORT_FORMAT1:
    		     mcSHOW_DBG_MSG(("%d", byte_bit_idx));
    			break;
        }

        if (who_am_I == HQA_REPORT_FORMAT3)
        {
            mcSHOW_DBG_MSG((" Channel%d ", u1ChannelIdx));
        }
        else if (who_am_I != HQA_REPORT_FORMAT5 && who_am_I != HQA_REPORT_FORMAT6)
        {
            mcSHOW_DBG_MSG((" Channel%d Rank%d ", u1ChannelIdx, u1RankIdx));
        }

    	switch (who_am_I)
    	{
            case HQA_REPORT_FORMAT2_1:
    		case HQA_REPORT_FORMAT0:
    			mcSHOW_DBG_MSG(("Byte%d %d\n", byte_bit_idx, value1));
    			break;
            case HQA_REPORT_FORMAT0_1:
    			mcSHOW_DBG_MSG(("Bit%x %d\n", byte_bit_idx, value1));                		                
                break;
            case HQA_REPORT_FORMAT0_2:
    			mcSHOW_DBG_MSG(("CA%x %d\n", byte_bit_idx, value1));                		                
                break;                
    		case HQA_REPORT_FORMAT1:
            case HQA_REPORT_FORMAT2:
            case HQA_REPORT_FORMAT3:
            case HQA_REPORT_FORMAT6:
    			mcSHOW_DBG_MSG(("%d\n", value1));
    			break;
            case HQA_REPORT_FORMAT4:
            case HQA_REPORT_FORMAT5:
                mcSHOW_DBG_MSG((" %s\n", ans_str));
                break;
    	}
    }
}

#endif


#ifdef RELEASE
#undef mcSHOW_DBG_MSG
#define mcSHOW_DBG_MSG(_x_)	opt_print _x_
#endif
#if ENABLE_EYESCAN_GRAPH || defined(FOR_HQA_REPORT_USED)
const U16 gRXVref_Voltage_Table_LP4[RX_VREF_RANGE_END+1]={
        (364+821)/2,
        (1202+1621)/2,
        (2039+2420)/2,
        (2875+3219)/2,
        (3711+4018)/2,
        (4548+4817)/2,
        (5384+5615)/2,
        (6221+6414)/2,
        (7057+7213)/2,
        (7893+8012)/2,
        (8727+8811)/2,
        (9554+9618)/2,
        (10380+10430)/2,
        (11200+11250)/2,
        (12010+12080)/2,
        (12810+12910)/2,
        (13120+13540)/2,
        (13830+14210)/2,
        (14530+14870)/2,
        (15240+15540)/2,
        (15950+16210)/2,
        (16650+16880)/2,
        (17360+17550)/2,
        (18070+18220)/2,
        (18770+18890)/2,
        (19470+19560)/2,
        (20160+20240)/2,
        (20850+20930)/2,
        (21520+21610)/2,
        (22190+22310)/2,
        (22860+23010)/2,
        (23540+23720)/2,
        (23330+23770)/2,
        (24040+24430)/2,
        (24740+25090)/2,
        (25450+25740)/2,
        (26150+26400)/2,
        (26860+27060)/2,
        (27560+27730)/2,
        (28240+28400)/2,
        (28920+29070)/2,
        (29580+29730)/2,
        (30230+30410)/2,
        (30890+31100)/2,
        (31540+31790)/2,
        (32190+32490)/2,
        (32840+33200)/2,
        (33500+33900)/2,
        (33540+34030)/2,
        (34250+34660)/2,
        (34950+35300)/2,
        (35650+35950)/2,
        (36360+36600)/2,
        (37060+37250)/2,
        (37760+37900)/2,
        (38420+38560)/2,
        (39050+39230)/2,
        (39680+39900)/2,
        (40310+40590)/2,
        (40940+41280)/2,
        (41570+41990)/2,
        (42190+42690)/2,
        (42820+43400)/2,
        (43450+44100)/2,
};
const U16 gVref_Voltage_Table_LP4X[VREF_RANGE_MAX][VREF_VOLTAGE_TABLE_NUM]={
    {1500,1560,1620,1680,1740,1800,1860,1920,1980,2040,2100,2160,2220,2280,2340,2400,2460,2510,2570,2630,2690,2750,2810,2870,2930,2990,3050,3110,3170,3230,3290,3350,3410,3470,3530,3590,3650,3710,3770,3830,3890,3950,4010,4070,4130,4190,4250,4310,4370,4430,4490},
    {3290,3350,3410,3470,3530,3590,3650,3710,3770,3830,3890,3950,4010,4070,4130,4190,4250,4310,4370,4430,4490,4550,4610,4670,4730,4790,4850,4910,4970,5030,5090,5150,5210,5270,5330,5390,5450,5510,5570,5630,5690,5750,5810,5870,5930,5990,6050,6110,6170,6230,6290}
};
const U16 gVref_Voltage_Table_LP4[VREF_RANGE_MAX][VREF_VOLTAGE_TABLE_NUM]={
    {1000,1040,1080,1120,1160,1200,1240,1280,1320,1360,1400,1440,1480,1520,1560,1600,1640,1680,1720,1760,1800,1840,1880,1920,1960,2000,2040,2080,2120,2160,2200,2240,2280,2320,2360,2400,2440,2480,2520,2560,2600,2640,2680,2720,2760,2800,2840,2880,2920,2960,3000},
    {2200,2240,2280,2320,2360,2400,2440,2480,2520,2560,2600,2640,2680,2720,2760,2800,2840,2880,2920,2960,3000,3040,3080,3120,3160,3200,3240,3280,3320,3360,3400,3440,3480,3520,3560,3600,3640,3680,3720,3760,3880,3840,3880,3920,3960,4000,4040,4080,4120,4160,4200}
};
#endif

#if ENABLE_EYESCAN_GRAPH
#define EyeScan_Pic_draw_line_Mirror 1
#define EysScan_Pic_draw_1UI_line 1
void EyeScan_Pic_draw_line(DRAMC_CTX_T *p, U8 draw_type, U8 u1VrefRange, U8 u1VrefIdx, U8 u1BitIdx, S16 u2DQDelayBegin, S16 u2DQDelayEnd, U8 u1FinalVrefRange, U16 Final_Vref_val, U8 VdlVWHigh_Upper_Vcent_Range, U32 VdlVWHigh_Upper_Vcent, U8 VdlVWHigh_Lower_Vcent_Range, U32 VdlVWHigh_Lower_Vcent, U16 FinalDQCaliDelay, S8 EyeScan_DelayCellPI_value, U16 delay_cell_ps, U16 Max_EyeScan_Min_val)
{
    int i;
    int local_VrefIdx, local_Upper_Vcent, local_Lower_Vcent, local_Final_VrefIdx;
    S8 EyeScan_Index=0;
    S16 EyeScan_Min_val, EyeScan_Max_val, Final_EyeScan_Min_val=EYESCAN_DATA_INVALID, Final_EyeScan_Max_val=EYESCAN_DATA_INVALID, Final_EyeScan_winsize=1;
    U16 *pVref_Voltage_Table[VREF_VOLTAGE_TABLE_NUM];
    U32 PI_of_1_UI;

    if (draw_type == 1)
    {
        pVref_Voltage_Table[VREF_RANGE_0]= (U16 *)gRXVref_Voltage_Table_LP4;
        if(p->u2DelayCellTimex100!=0)
        {
            PI_of_1_UI = (50000000/(p->frequency*p->u2DelayCellTimex100));

            FinalDQCaliDelay = (U16)EyeScan_DelayCellPI_value;
            EyeScan_DelayCellPI_value = 0;
        }
        else
        {
            PI_of_1_UI = 0;
            mcSHOW_ERR_MSG(("DelayCell is 0\n"));
        }
    }
    else
    {
        if (p->dram_type == TYPE_LPDDR4)
        {
            pVref_Voltage_Table[VREF_RANGE_0] = (U16 *)gVref_Voltage_Table_LP4[VREF_RANGE_0];
            pVref_Voltage_Table[VREF_RANGE_1] = (U16 *)gVref_Voltage_Table_LP4[VREF_RANGE_1];
        }
        if (p->dram_type == TYPE_LPDDR4X)
        {
            pVref_Voltage_Table[VREF_RANGE_0] = (U16 *)gVref_Voltage_Table_LP4X[VREF_RANGE_0];
            pVref_Voltage_Table[VREF_RANGE_1] = (U16 *)gVref_Voltage_Table_LP4X[VREF_RANGE_1];
        }

        PI_of_1_UI = 32;
    }

    if (u1VrefRange==1 && u1VrefIdx <=20)
    {
        u1VrefRange=0;
        u1VrefIdx += 30;
    }
    if (u1FinalVrefRange==1 && Final_Vref_val <=20)
    {
        u1FinalVrefRange=0;
        Final_Vref_val += 30;
    }
    if (u1VrefRange != u1FinalVrefRange)
    {
        Final_Vref_val = 0xff;
    }

    local_Upper_Vcent = VdlVWHigh_Upper_Vcent_Range*VREF_VOLTAGE_TABLE_NUM+VdlVWHigh_Upper_Vcent;
    local_Lower_Vcent = VdlVWHigh_Lower_Vcent_Range*VREF_VOLTAGE_TABLE_NUM+VdlVWHigh_Lower_Vcent;
    local_VrefIdx = u1VrefRange*VREF_VOLTAGE_TABLE_NUM+u1VrefIdx;
    local_Final_VrefIdx = u1FinalVrefRange*VREF_VOLTAGE_TABLE_NUM+Final_Vref_val;

    if (VdlVWHigh_Upper_Vcent_Range==VREF_RANGE_1 && VdlVWHigh_Upper_Vcent<=20) local_Upper_Vcent = VdlVWHigh_Upper_Vcent_Range*VREF_VOLTAGE_TABLE_NUM+VdlVWHigh_Upper_Vcent-20;
    if (VdlVWHigh_Lower_Vcent_Range==VREF_RANGE_1 && VdlVWHigh_Lower_Vcent<=20) local_Lower_Vcent = VdlVWHigh_Lower_Vcent_Range*VREF_VOLTAGE_TABLE_NUM+VdlVWHigh_Lower_Vcent-20;

    mcSHOW_EYESCAN_MSG(("Vref-"));

    if (draw_type == 1 && u1VrefIdx <= 11)
    {
        if (u1VrefIdx==0) 
        {
            mcSHOW_EYESCAN_MSG((" "));
        }
        mcSHOW_EYESCAN_MSG((" "));
    }

    mcSHOW_EYESCAN_MSG(("%d.%d%d",pVref_Voltage_Table[u1VrefRange][u1VrefIdx]/100, ((pVref_Voltage_Table[u1VrefRange][u1VrefIdx]%100)/10), pVref_Voltage_Table[u1VrefRange][u1VrefIdx]%10));

    if (draw_type == 1)
    {
        mcSHOW_EYESCAN_MSG(("m|"));
    }
    else
    {
        mcSHOW_EYESCAN_MSG(("%%|"));
    }




#if VENDER_JV_LOG || defined(RELEASE)
#if EyeScan_Pic_draw_line_Mirror
    EyeScan_DelayCellPI_value = 0-EyeScan_DelayCellPI_value;
#endif
#endif

#if EyeScan_Pic_draw_line_Mirror
    EyeScan_Index=EYESCAN_BROKEN_NUM-1;
    EyeScan_Min_val = gEyeScan_Min[(u1VrefIdx+u1VrefRange*30)/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx][EyeScan_Index];
    EyeScan_Max_val = gEyeScan_Max[(u1VrefIdx+u1VrefRange*30)/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx][EyeScan_Index];

    while(EyeScan_Min_val==EYESCAN_DATA_INVALID && EyeScan_Index>0)
    {
        EyeScan_Index--;
        EyeScan_Min_val = gEyeScan_Min[(u1VrefIdx+u1VrefRange*30)/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx][EyeScan_Index];
        EyeScan_Max_val = gEyeScan_Max[(u1VrefIdx+u1VrefRange*30)/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx][EyeScan_Index];
    }
#else
    EyeScan_Index=0;
    EyeScan_Min_val = gEyeScan_Min[(u1VrefIdx+u1VrefRange*30)/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx][EyeScan_Index];
    EyeScan_Max_val = gEyeScan_Max[(u1VrefIdx+u1VrefRange*30)/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx][EyeScan_Index];
#endif

    if (draw_type == 2)
    {
        EyeScan_Min_val += gEyeScan_MinMax_store_delay[u1BitIdx/8];
        EyeScan_Max_val += gEyeScan_MinMax_store_delay[u1BitIdx/8];    
    }

    if ((EyeScan_Max_val - EyeScan_Min_val + 1) > Final_EyeScan_winsize)
    {
#if EyeScan_Pic_draw_line_Mirror
        Final_EyeScan_Max_val = EyeScan_Max_val;
        Final_EyeScan_Min_val = EyeScan_Min_val;
#else
        Final_EyeScan_Max_val = EyeScan_Max_val;
        Final_EyeScan_Min_val = EyeScan_Min_val;
#endif
        Final_EyeScan_winsize =  (EyeScan_Max_val - EyeScan_Min_val + 1);
    }

#if VENDER_JV_LOG || defined(RELEASE)
#if EyeScan_Pic_draw_line_Mirror
    for(i=(Max_EyeScan_Min_val+PI_of_1_UI+EyeScan_DelayCellPI_value)*delay_cell_ps/100; i>(Max_EyeScan_Min_val+EyeScan_DelayCellPI_value)*delay_cell_ps/100; i-=10)
#else
    for(i=(Max_EyeScan_Min_val+EyeScan_DelayCellPI_value)*delay_cell_ps/100; i<(Max_EyeScan_Min_val+PI_of_1_UI+EyeScan_DelayCellPI_value)*delay_cell_ps/100; i+=10)
#endif
#else
#if EyeScan_Pic_draw_line_Mirror
    for(i=u2DQDelayEnd; i>=u2DQDelayBegin; i--)
#else
    for(i=u2DQDelayBegin; i<=u2DQDelayEnd; i++)
#endif
#endif
    {

#if VENDER_JV_LOG || defined(RELEASE)
#if EyeScan_Pic_draw_line_Mirror
        if (i<=((EyeScan_Min_val+EyeScan_DelayCellPI_value)*delay_cell_ps/100) && EyeScan_Index!= 0)
        {
            EyeScan_Index--;
            EyeScan_Min_val = gEyeScan_Min[(u1VrefIdx+u1VrefRange*30)/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx][EyeScan_Index];
            EyeScan_Max_val = gEyeScan_Max[(u1VrefIdx+u1VrefRange*30)/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx][EyeScan_Index];

            if (draw_type == 2)
            {
                EyeScan_Min_val += gEyeScan_MinMax_store_delay[u1BitIdx/8];
                EyeScan_Max_val += gEyeScan_MinMax_store_delay[u1BitIdx/8];    
            }

            if ((EyeScan_Max_val - EyeScan_Min_val + 1) > Final_EyeScan_winsize)
            {
                Final_EyeScan_Max_val = EyeScan_Max_val;
                Final_EyeScan_Min_val = EyeScan_Min_val;
                Final_EyeScan_winsize =  (EyeScan_Max_val - EyeScan_Min_val + 1);
            }

        }
#endif
#else
#if EyeScan_Pic_draw_line_Mirror
        if (i==(EyeScan_Min_val) && EyeScan_Index!= 0)
        {
            EyeScan_Index--;
            EyeScan_Min_val = gEyeScan_Min[(u1VrefIdx+u1VrefRange*30)/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx][EyeScan_Index];
            EyeScan_Max_val = gEyeScan_Max[(u1VrefIdx+u1VrefRange*30)/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx][EyeScan_Index];

            if (draw_type == 2)
            {
                EyeScan_Min_val += gEyeScan_MinMax_store_delay[u1BitIdx/8];
                EyeScan_Max_val += gEyeScan_MinMax_store_delay[u1BitIdx/8];    
            }

            if ((EyeScan_Max_val - EyeScan_Min_val + 1) > Final_EyeScan_winsize)
            {
                Final_EyeScan_Max_val = EyeScan_Max_val;
                Final_EyeScan_Min_val = EyeScan_Min_val;
                Final_EyeScan_winsize =  (EyeScan_Max_val - EyeScan_Min_val + 1);
            }

        }
#endif
#endif

#if VENDER_JV_LOG || defined(RELEASE)
        if (i>=((EyeScan_Min_val+EyeScan_DelayCellPI_value)*delay_cell_ps/100) && i<=((EyeScan_Max_val+EyeScan_DelayCellPI_value)*delay_cell_ps/100))
#else
        if (i>=(EyeScan_Min_val) && i<=(EyeScan_Max_val))
#endif
        {
#if !VENDER_JV_LOG && !defined(RELEASE)
            if (i==FinalDQCaliDelay+EyeScan_DelayCellPI_value) //Final DQ delay
            {
                if (gEye_Scan_color_flag)
                {
                    mcSHOW_EYESCAN_MSG(("\033[0;105mH\033[m"));
                }
                else
                {
                    mcSHOW_EYESCAN_MSG(("H"));
                }
            }
            else
            if (local_VrefIdx==local_Final_VrefIdx) //Final Vref
            {
                if (gEye_Scan_color_flag)
                {
                    mcSHOW_EYESCAN_MSG(("\033[0;105mV\033[m"));
                }
                else
                {
                    mcSHOW_EYESCAN_MSG(("V"));
                }
            }
            else //spec in margin
            if (local_VrefIdx<=local_Upper_Vcent && local_VrefIdx>=local_Lower_Vcent && i>=(FinalDQCaliDelay+EyeScan_DelayCellPI_value-3) && i<=(FinalDQCaliDelay+EyeScan_DelayCellPI_value+3))
            {
                if (gEye_Scan_color_flag)
                {
                    mcSHOW_EYESCAN_MSG(("\033[0;103mQ\033[m"));
                }
                else
                {
                    mcSHOW_EYESCAN_MSG(("Q"));
                }
            }
            else //pass margin
#endif
            {
#if VENDER_JV_LOG || defined(RELEASE)
                if (gEye_Scan_color_flag)
                {
                    mcSHOW_EYESCAN_MSG(("\033[0;102mO\033[m"));
                }
                else
                {
                    mcSHOW_EYESCAN_MSG(("O"));
                }
#else
                if (gEye_Scan_color_flag)
                {
                    mcSHOW_EYESCAN_MSG(("\033[0;102mO\033[m"));
                }
                else
                {
                    mcSHOW_EYESCAN_MSG(("O"));
                }
#endif
            }
        }
        else
        {
#if !VENDER_JV_LOG && !defined(RELEASE)
#if EysScan_Pic_draw_1UI_line
            if (i==(int)(Max_EyeScan_Min_val) || i==(int)(Max_EyeScan_Min_val+PI_of_1_UI))
            {
                if (gEye_Scan_color_flag)
                {
                    mcSHOW_EYESCAN_MSG(("\033[0;107m.\033[m"));
                }
                else
                {
                    mcSHOW_EYESCAN_MSG(("."));
                }
            }
            else
#endif
#endif
            {
                //not valid
#if VENDER_JV_LOG || defined(RELEASE)
                if (gEye_Scan_color_flag)
                {
                    mcSHOW_EYESCAN_MSG(("\033[0;100m.\033[m"));
                }
                else
                {
                    mcSHOW_EYESCAN_MSG(("."));
                }
#else
                if (gEye_Scan_color_flag)
                {
                    mcSHOW_EYESCAN_MSG(("\033[0;100m.\033[m"));
                }
                else
                {
                    mcSHOW_EYESCAN_MSG(("."));
                }
#endif
            }
        }
    }


#if EyeScan_Pic_draw_line_Mirror
    if (Final_EyeScan_Min_val!=EYESCAN_DATA_INVALID && Final_EyeScan_Max_val!=EYESCAN_DATA_INVALID)
    {
#if !VENDER_JV_LOG && !defined(RELEASE)
        if (Final_EyeScan_Max_val>(FinalDQCaliDelay+EyeScan_DelayCellPI_value) && (FinalDQCaliDelay+EyeScan_DelayCellPI_value)>Final_EyeScan_Min_val)
        {
            mcSHOW_EYESCAN_MSG((" -%d ",(Final_EyeScan_Max_val-(FinalDQCaliDelay+EyeScan_DelayCellPI_value))));
            mcSHOW_EYESCAN_MSG(("%d ", ((FinalDQCaliDelay+EyeScan_DelayCellPI_value)-Final_EyeScan_Min_val)));
        }
        else if (Final_EyeScan_Max_val>(FinalDQCaliDelay+EyeScan_DelayCellPI_value) && Final_EyeScan_Min_val>(FinalDQCaliDelay+EyeScan_DelayCellPI_value))
        {
            mcSHOW_EYESCAN_MSG((" -%d ",(Final_EyeScan_Max_val-Final_EyeScan_Min_val)));
            mcSHOW_EYESCAN_MSG((" --- "));
        }
        else if ((FinalDQCaliDelay+EyeScan_DelayCellPI_value)>Final_EyeScan_Max_val && (FinalDQCaliDelay+EyeScan_DelayCellPI_value)>Final_EyeScan_Min_val)
        {
            mcSHOW_EYESCAN_MSG((" --- "));
            mcSHOW_EYESCAN_MSG(("%d ", (Final_EyeScan_Max_val-Final_EyeScan_Min_val)));
        }
        else
        {
            mcSHOW_EYESCAN_MSG((" --- "));
            mcSHOW_EYESCAN_MSG((" --- "));
        }
#endif
        //window
#if VENDER_JV_LOG || defined(RELEASE)
        mcSHOW_EYESCAN_MSG(("%dps", Final_EyeScan_winsize*delay_cell_ps/100));
#else
        mcSHOW_EYESCAN_MSG(("%d", Final_EyeScan_winsize));
#endif
    }
#else
    if (Final_EyeScan_Max_val != Final_EyeScan_Min_val && Final_EyeScan_Max_val!=EYESCAN_DATA_INVALID)
    {
#if !VENDER_JV_LOG && !defined(RELEASE)
        if (Final_EyeScan_Max_val>(FinalDQCaliDelay+EyeScan_DelayCellPI_value) && (FinalDQCaliDelay+EyeScan_DelayCellPI_value)>Final_EyeScan_Min_val)
        {
            mcSHOW_EYESCAN_MSG((" -%d ", ((FinalDQCaliDelay+EyeScan_DelayCellPI_value)-Final_EyeScan_Min_val)));
            mcSHOW_EYESCAN_MSG(("%d ",(Final_EyeScan_Max_val-(FinalDQCaliDelay+EyeScan_DelayCellPI_value))));
        }
        else if (Final_EyeScan_Max_val>(FinalDQCaliDelay+EyeScan_DelayCellPI_value) && Final_EyeScan_Min_val>(FinalDQCaliDelay+EyeScan_DelayCellPI_value))
        {
            mcSHOW_EYESCAN_MSG((" --- "));
            mcSHOW_EYESCAN_MSG(("%d ",(Final_EyeScan_Max_val-Final_EyeScan_Min_val)));
        }
        else if ((FinalDQCaliDelay+EyeScan_DelayCellPI_value)>Final_EyeScan_Max_val && (FinalDQCaliDelay+EyeScan_DelayCellPI_value)>Final_EyeScan_Min_val)
        {
            mcSHOW_EYESCAN_MSG((" -%d ", (Final_EyeScan_Max_val-Final_EyeScan_Min_val)));
            mcSHOW_EYESCAN_MSG((" --- "));
        }
        else
        {
            mcSHOW_EYESCAN_MSG((" --- "));
            mcSHOW_EYESCAN_MSG((" --- "));
        }
#endif
        //window
#if VENDER_JV_LOG || defined(RELEASE)
        mcSHOW_EYESCAN_MSG(("%dps", Final_EyeScan_winsize*delay_cell_ps/100));
#else
        mcSHOW_EYESCAN_MSG(("%d", Final_EyeScan_winsize));
#endif
    }
#endif

    mcSHOW_EYESCAN_MSG(("\n"));


}

void print_EYESCAN_LOG_message(DRAMC_CTX_T *p, U8 print_type)
{
    U32 u1ChannelIdx=p->channel, u1RankIdx=p->rank;
    S8 u1VrefIdx;
    U8 u1VrefRange;
    U8 u1BitIdx, u1CA;
    U32 VdlVWTotal, VdlVWLow, VdlVWHigh, Vcent_DQ;
    U32 VdlVWHigh_Upper_Vcent=VREF_VOLTAGE_TABLE_NUM-1, VdlVWHigh_Lower_Vcent=0, VdlVWBest_Vcent=0;
    U32 VdlVWHigh_Upper_Vcent_Range=1, VdlVWHigh_Lower_Vcent_Range=0, VdlVWBest_Vcent_Range=1;;
    U8 Upper_Vcent_pass_flag=0, Lower_Vcent_pass_flag=0;
    S32 i, vrefrange_i;
    U8 local_channel_num=2;
    U8 shuffleIdx;
    U8 TXVrefRange, CBTVrefRange;
    U16 finalVref=0, finalVrefRange=0; 
    S32 scan_vref_number=0;
    U32 vddq;
    U8 Min_Value_1UI_Line;
    S8 EyeScan_Index;
    U8 cal_length=0;
    U16 *pVref_Voltage_Table[VREF_VOLTAGE_TABLE_NUM];
    S8 EyeScan_DelayCellPI_value;
    U8 EyeScanVcent[10+DQ_DATA_WIDTH_LP4*2], max_winsize;
    U8 minEyeScanVcentUpperBound=0xff, minEyeScanVcentUpperBound_bit=0;
    U8 minEyeScanVcentLowerBound=0xff, minEyeScanVcentLowerBound_bit=0;
    U16 one_pi_ps=100000000/(p->frequency*2*32);
    U8 u1CBTEyeScanEnable, u1RXEyeScanEnable, u1TXEyeScanEnable;

    U32 u2DQDelayEnd, u2TX_DQ_PreCal_LP4_Samll;
    S32 u2DQDelayBegin;

    U8 u1ByteIdx, u1ByteIdx_Begin=0, u1ByteIdx_End=0;

    local_channel_num = p->support_channel_num;

    if (p->dram_type == TYPE_LPDDR4)
    {
        pVref_Voltage_Table[VREF_RANGE_0] = (U16 *)gVref_Voltage_Table_LP4[VREF_RANGE_0];
        pVref_Voltage_Table[VREF_RANGE_1] = (U16 *)gVref_Voltage_Table_LP4[VREF_RANGE_1];
    }
    if (p->dram_type == TYPE_LPDDR4X)
    {
        pVref_Voltage_Table[VREF_RANGE_0] = (U16 *)gVref_Voltage_Table_LP4X[VREF_RANGE_0];
        pVref_Voltage_Table[VREF_RANGE_1] = (U16 *)gVref_Voltage_Table_LP4X[VREF_RANGE_1];
    }

    u1CBTEyeScanEnable = (gCBT_EYE_Scan_flag==1 && ((gCBT_EYE_Scan_only_higheset_freq_flag==1 && p->frequency == u2DFSGetHighestFreq(p)) || gCBT_EYE_Scan_only_higheset_freq_flag==0));
    u1RXEyeScanEnable = (gRX_EYE_Scan_flag==1 && ((gRX_EYE_Scan_only_higheset_freq_flag==1 && p->frequency == u2DFSGetHighestFreq(p)) || gRX_EYE_Scan_only_higheset_freq_flag==0));
    u1TXEyeScanEnable = (gTX_EYE_Scan_flag==1 && ((gTX_EYE_Scan_only_higheset_freq_flag==1 && p->frequency == u2DFSGetHighestFreq(p)) || gTX_EYE_Scan_only_higheset_freq_flag==0));


#if 0 //fra test
    u1CBTEyeScanEnable = u1CBTEyeScanEnable & (p->channel==0 && p->rank==0);
#endif

/**************************************************************************************
    CBT/RX/TX EYESCAN log
***************************************************************************************/
    if (print_type==0)
    {
        if (p->frequency <=934) VdlVWTotal = 17500; //VcIVW 175mv
        else if (p->frequency <= 1600) VdlVWTotal = 15500; //VcIVW 155mv
        else VdlVWTotal = 14500; //VcIVW 145mv
    }
    else
    {
#if 0
        if (p->frequency <=1600) VdlVWTotal = 10000; //14000; //140mv
        else VdlVWTotal = 10000; //12000; //120mv    
#else
        VdlVWTotal = 10000;
#endif
    }

    CBTVrefRange = (u1MR12Value[p->channel][p->rank][p->dram_fsp]>>6)&1;
    TXVrefRange = (u1MR14Value[p->channel][p->rank][p->dram_fsp]>>6)&1;

    if (print_type == 1)
    {
        u1ByteIdx_Begin = 0;
        u1ByteIdx_End = 1;
    }

for(u1ByteIdx=u1ByteIdx_Begin; u1ByteIdx<=u1ByteIdx_End; u1ByteIdx++)
{

#if 1 //#if !VENDER_JV_LOG && !defined(RELEASE)
    if ((print_type==0 && u1CBTEyeScanEnable) || (print_type==1 && u1RXEyeScanEnable) || (print_type==2 && u1TXEyeScanEnable))
    {   
        vddq=vGetVoltage(p, 2)/1000; //mv
        EYESCAN_LOG_Print_Prefix_String(); mcSHOW_DBG_MSG(("VDDQ=%dmV\n",vddq));

        if (print_type==0)
        {
            finalVrefRange = CBTVrefRange;
            finalVref = gFinalCBTVrefDQ[u1ChannelIdx][u1RankIdx];

            cal_length = CATRAINING_NUM_LP4;
        }
        else if (print_type==1)
        {
            finalVrefRange = 0;
            finalVref = gFinalRXVrefDQ[u1ChannelIdx][u1RankIdx][u1ByteIdx];

            cal_length = DQS_BIT_NUMBER; //p->data_width;
        }
        else
//        if (print_type==2)
        {
            finalVrefRange = TXVrefRange;
            finalVref = gFinalTXVrefDQ[u1ChannelIdx][u1RankIdx];

            cal_length = p->data_width;
        }

        EYESCAN_LOG_Print_Prefix_String(); mcSHOW_DBG_MSG(("%s Window\n", print_EYESCAN_LOG_type(print_type)));

        EYESCAN_LOG_Print_Prefix_String(); mcSHOW_DBG_MSG(("%s Channel%d ", print_EYESCAN_LOG_type(print_type), u1ChannelIdx));
        if (print_type==1)
        {
            mcSHOW_DBG_MSG(("Final_Vref Byte%d Vcent %d(%dmV(X100))\n",
                                                    u1ByteIdx,
                                                    finalVref,
                                                    gRXVref_Voltage_Table_LP4[finalVref]));

            Vcent_DQ = gRXVref_Voltage_Table_LP4[gFinalRXVrefDQ[u1ChannelIdx][u1RankIdx][u1ByteIdx]];
        }
        else
        {
            mcSHOW_DBG_MSG(("Range %d Final_Vref Vcent=%d(%dmV(X100))\n",
                                                finalVrefRange,
                                                finalVref,
                                                pVref_Voltage_Table[finalVrefRange][finalVref]*vddq/100));

            Vcent_DQ = pVref_Voltage_Table[finalVrefRange][finalVref]*vddq/100;
        }


        //find VdlVWHigh first
        if (print_type==1)
        {
            VdlVWHigh_Upper_Vcent_Range = 0;
            VdlVWHigh_Upper_Vcent = RX_VREF_RANGE_END;
            vrefrange_i = 0;
            scan_vref_number = RX_VREF_RANGE_END;
        }
        else
        {
            VdlVWHigh_Upper_Vcent_Range = 1;
            VdlVWHigh_Upper_Vcent = VREF_VOLTAGE_TABLE_NUM-1;
            vrefrange_i = finalVrefRange;
            scan_vref_number = VREF_VOLTAGE_TABLE_NUM;
        }
        for(i=finalVref; i<scan_vref_number; i+=EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV)
        {
            if (print_type==1)
            {
                if (gRXVref_Voltage_Table_LP4[i] - Vcent_DQ >= VdlVWTotal/2)
                {
                    /* find VdlVWHigh upper bound */
                    VdlVWHigh_Upper_Vcent = i;
                    break;
                }
            }
            else
            {
                if (((pVref_Voltage_Table[vrefrange_i][i]*vddq/100 - Vcent_DQ)) >= VdlVWTotal/2)
                {
                    /* find VdlVWHigh upper bound */
                    VdlVWHigh_Upper_Vcent = i;
                    VdlVWHigh_Upper_Vcent_Range = vrefrange_i;
                    break;
                }
                if (i==(VREF_VOLTAGE_TABLE_NUM-1) && vrefrange_i==0)
                {
                    vrefrange_i=1;
                    i=20;
                }
            }
        }

        EYESCAN_LOG_Print_Prefix_String(); mcSHOW_DBG_MSG(("%s VdlVWHigh_Upper ", print_EYESCAN_LOG_type(print_type)));
        if (print_type!=1) 
        {
            mcSHOW_DBG_MSG(("Range=%d ", VdlVWHigh_Upper_Vcent_Range));
        }
        else 
        {
            mcSHOW_DBG_MSG(("Byte%d ", u1ByteIdx));
        }
        mcSHOW_DBG_MSG(("Vcent=%d(%dmV(X100))\n",
            VdlVWHigh_Upper_Vcent,
            print_type==1 ? gRXVref_Voltage_Table_LP4[VdlVWHigh_Upper_Vcent] : pVref_Voltage_Table[VdlVWHigh_Upper_Vcent_Range][VdlVWHigh_Upper_Vcent]*vddq/100));

        //find VldVWLow first
        VdlVWHigh_Lower_Vcent_Range = 0;
        VdlVWHigh_Lower_Vcent = 0;
        vrefrange_i = finalVrefRange;
        for(i=(finalVref); i>=0; i-=EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV)
        {
            if (print_type==1)
            {
                if (Vcent_DQ - gRXVref_Voltage_Table_LP4[i] >= VdlVWTotal/2)
                {
                    /* find VdlVWHigh lower bound */
                    VdlVWHigh_Lower_Vcent = i;
                    break;
                }
            }
            else
            {
                if (((Vcent_DQ - pVref_Voltage_Table[vrefrange_i][i]*vddq/100)) >= VdlVWTotal/2)
                {
                    /* find VdlVWHigh lower bound */
                    VdlVWHigh_Lower_Vcent = i;
                    VdlVWHigh_Lower_Vcent_Range = vrefrange_i;
                    break;
                }
                if (i<=21 && vrefrange_i==1)
                {
                    vrefrange_i=0;
                    i=VREF_VOLTAGE_TABLE_NUM-(21-i);
                }
            }
        }

        EYESCAN_LOG_Print_Prefix_String(); mcSHOW_DBG_MSG(("%s VdlVWHigh_Lower ", print_EYESCAN_LOG_type(print_type)));
        if (print_type!=1) 
        {
            mcSHOW_DBG_MSG(("Range=%d ", VdlVWHigh_Lower_Vcent_Range));
        }
        else 
        {
            mcSHOW_DBG_MSG(("Byte%d ", u1ByteIdx));
        }        
        mcSHOW_DBG_MSG(("Vcent=%d(%dmV(X100))\n",
            VdlVWHigh_Lower_Vcent,
            print_type==1 ? gRXVref_Voltage_Table_LP4[VdlVWHigh_Lower_Vcent] : pVref_Voltage_Table[VdlVWHigh_Lower_Vcent_Range][VdlVWHigh_Lower_Vcent]*vddq/100));

#ifdef FOR_HQA_TEST_USED
            EyeScanVcent[0] = finalVrefRange;
            EyeScanVcent[1] = finalVref;
            EyeScanVcent[2] = VdlVWHigh_Upper_Vcent_Range;
            EyeScanVcent[3] = VdlVWHigh_Upper_Vcent;
            EyeScanVcent[4] = VdlVWHigh_Lower_Vcent_Range;
            EyeScanVcent[5] = VdlVWHigh_Lower_Vcent;
#endif

        shuffleIdx = get_shuffleIndex_by_Freq(p);

#ifdef FOR_HQA_REPORT_USED
        if (print_type==1) 
        {
            EYESCAN_LOG_Print_Prefix_String(); mcSHOW_DBG_MSG(("delay cell %d/100ps\n", gHQALOG_RX_delay_cell_ps_075V));
        }
#endif        

//            EYESCAN_LOG_Print_Prefix_String(); mcSHOW_DBG_MSG(("delay cell %d/100ps\n", u2gdelay_cell_ps_all[shuffleIdx][u1ChannelIdx]));

        for (u1BitIdx=u1ByteIdx*DQS_BIT_NUMBER; u1BitIdx<(u1ByteIdx*DQS_BIT_NUMBER+cal_length); u1BitIdx++)
        {
            if (print_type!=1)
            {
                // compare Upper/Lower Vcent pass criterion is pass or fail?
                for(u1VrefIdx=finalVref+finalVrefRange*30; u1VrefIdx<=(S8)(VdlVWHigh_Upper_Vcent+VdlVWHigh_Upper_Vcent_Range*30); u1VrefIdx+=EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV)
                {
                    Upper_Vcent_pass_flag = 0;
                    for (EyeScan_Index=0; EyeScan_Index<EYESCAN_BROKEN_NUM; EyeScan_Index++)
                    {
                        if (gEyeScan_Min[u1VrefIdx/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx][EyeScan_Index] != EYESCAN_DATA_INVALID && gEyeScan_Max[u1VrefIdx/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx][EyeScan_Index] != EYESCAN_DATA_INVALID)                    
                        if ((print_type==0 && ((((gEyeScan_CaliDelay[0]+gEyeScan_DelayCellPI[u1BitIdx]) - gEyeScan_Min[u1VrefIdx/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx][EyeScan_Index]) >=3 ) && ((gEyeScan_Max[u1VrefIdx/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx][EyeScan_Index] - (gEyeScan_CaliDelay[0]+gEyeScan_DelayCellPI[u1BitIdx])) >=3 ))) ||
                            (print_type==2 && ((((gEyeScan_CaliDelay[u1BitIdx/8]+gEyeScan_DelayCellPI[u1BitIdx]) - (gEyeScan_Min[u1VrefIdx/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx][EyeScan_Index]+gEyeScan_MinMax_store_delay[u1BitIdx/8])) >=3 ) && (((gEyeScan_Max[u1VrefIdx/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx][EyeScan_Index]+gEyeScan_MinMax_store_delay[u1BitIdx/8]) - (gEyeScan_CaliDelay[u1BitIdx/8]+gEyeScan_DelayCellPI[u1BitIdx])) >=3 ))))
                        {
                            Upper_Vcent_pass_flag = 1;
                        }
                    }
                    if (Upper_Vcent_pass_flag == 0) break; // fail!!
                }
                for(u1VrefIdx=VdlVWHigh_Lower_Vcent+VdlVWHigh_Lower_Vcent_Range*30; u1VrefIdx<=(S8)(finalVref+finalVrefRange*30); u1VrefIdx+=EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV)
                {
                    Lower_Vcent_pass_flag = 0;
                    for (EyeScan_Index=0; EyeScan_Index<EYESCAN_BROKEN_NUM; EyeScan_Index++)
                    {
                        if (gEyeScan_Min[u1VrefIdx/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx][EyeScan_Index] != EYESCAN_DATA_INVALID && gEyeScan_Max[u1VrefIdx/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx][EyeScan_Index] != EYESCAN_DATA_INVALID)
                        if ((print_type==0 && ((((gEyeScan_CaliDelay[0]+gEyeScan_DelayCellPI[u1BitIdx]) - gEyeScan_Min[u1VrefIdx/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx][EyeScan_Index]) >=3 ) && ((gEyeScan_Max[u1VrefIdx/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx][EyeScan_Index] - (gEyeScan_CaliDelay[0]+gEyeScan_DelayCellPI[u1BitIdx])) >=3 ))) ||
                             (print_type==2 && ((((gEyeScan_CaliDelay[u1BitIdx/8]+gEyeScan_DelayCellPI[u1BitIdx]) - (gEyeScan_Min[u1VrefIdx/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx][EyeScan_Index]+gEyeScan_MinMax_store_delay[u1BitIdx/8])) >=3 ) && (((gEyeScan_Max[u1VrefIdx/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx][EyeScan_Index]+gEyeScan_MinMax_store_delay[u1BitIdx/8]) - (gEyeScan_CaliDelay[u1BitIdx/8]+gEyeScan_DelayCellPI[u1BitIdx])) >=3 ))))
                        {
                            Lower_Vcent_pass_flag = 1;
                        }
                    }
                    if (Lower_Vcent_pass_flag == 0) break; //fail!!
                }
            }

#ifdef FOR_HQA_TEST_USED
            //find VdlVWBest Vref Range and Vref
            VdlVWBest_Vcent_Range = 1;
            VdlVWBest_Vcent = VREF_VOLTAGE_TABLE_NUM-1;
            if (print_type==1) vrefrange_i=0;
            else vrefrange_i = 1;
            max_winsize = 0;   
            for(i=(scan_vref_number - (scan_vref_number==VREF_VOLTAGE_TABLE_NUM ? 1 : 0)) & ~0x1; i>=0; i-=EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV)
            {
                if (gEyeScan_WinSize[(i+vrefrange_i*30)/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx] > max_winsize)
                {
                    max_winsize = gEyeScan_WinSize[(i+vrefrange_i*30)/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx];
                    VdlVWBest_Vcent_Range = vrefrange_i;
                    VdlVWBest_Vcent = i;
                }
                if (print_type!=1 && i==21 && vrefrange_i==1)
                {
                    vrefrange_i=0;
                    i=VREF_VOLTAGE_TABLE_NUM;
                }                    
            }
            
            if (print_type==1)
            {
                EyeScanVcent[10+u1BitIdx] = VdlVWBest_Vcent;
            }
            else
            {
                EyeScanVcent[10+u1BitIdx*2] = VdlVWBest_Vcent_Range;
                EyeScanVcent[10+u1BitIdx*2+1] = VdlVWBest_Vcent;                    
            }
#endif

            if (print_type==1)
            {
#ifdef FOR_HQA_REPORT_USED            
                EYESCAN_LOG_Print_Prefix_String(); HQA_LOG_Print_Freq_String(p); mcSHOW_DBG_MSG(("Channel%d Rank%d Bit%d(DRAM DQ%d)\tHigher VdlTW=%d/100ps\tLower VdlTW=%d/100ps\n",
                    u1ChannelIdx,
                    u1RankIdx,
                    u1BitIdx,
                    uiLPDDR4_O1_Mapping_POP[p->channel][u1BitIdx],
                    gEyeScan_WinSize[VdlVWHigh_Upper_Vcent/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx]*gHQALOG_RX_delay_cell_ps_075V,
                    gEyeScan_WinSize[VdlVWHigh_Lower_Vcent/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx]*gHQALOG_RX_delay_cell_ps_075V
                    ));            
#endif
            }
            else
            {
                EYESCAN_LOG_Print_Prefix_String(); HQA_LOG_Print_Freq_String(p); mcSHOW_DBG_MSG(("Channel%d Rank%d %s%d",
                    u1ChannelIdx,
                    u1RankIdx,
                    print_type == 0 ? "CA" : "Bit",
                    u1BitIdx
                    ));
                    if (print_type==2) 
                    {
                        mcSHOW_DBG_MSG(("(DRAM DQ%d)", uiLPDDR4_O1_Mapping_POP[p->channel][u1BitIdx]));
                    }
                mcSHOW_DBG_MSG(("\tHigher VdlTW=%dPI(%d/100ps)(%s)\tLower VdlTW=%dpi(%d/100ps)(%s)\n",
                    gEyeScan_WinSize[(VdlVWHigh_Upper_Vcent+VdlVWHigh_Upper_Vcent_Range*30)/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx],
                    gEyeScan_WinSize[(VdlVWHigh_Upper_Vcent+VdlVWHigh_Upper_Vcent_Range*30)/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx]*one_pi_ps,
                    Upper_Vcent_pass_flag==1 ? "PASS" : "FAIL",
                    gEyeScan_WinSize[(VdlVWHigh_Lower_Vcent+VdlVWHigh_Lower_Vcent_Range*30)/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx],
                    gEyeScan_WinSize[(VdlVWHigh_Lower_Vcent+VdlVWHigh_Lower_Vcent_Range*30)/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx]*one_pi_ps,
                    Lower_Vcent_pass_flag==1 ? "PASS" : "FAIL"
                    ));
            }

            if (gEyeScan_WinSize[(VdlVWHigh_Upper_Vcent+VdlVWHigh_Upper_Vcent_Range*30)/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx] < minEyeScanVcentUpperBound)
            {
                minEyeScanVcentUpperBound = gEyeScan_WinSize[(VdlVWHigh_Upper_Vcent+VdlVWHigh_Upper_Vcent_Range*30)/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx];
                minEyeScanVcentUpperBound_bit = u1BitIdx;
            }
            if (gEyeScan_WinSize[(VdlVWHigh_Lower_Vcent+VdlVWHigh_Lower_Vcent_Range*30)/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx] < minEyeScanVcentLowerBound)
            {
                minEyeScanVcentLowerBound = gEyeScan_WinSize[(VdlVWHigh_Lower_Vcent+VdlVWHigh_Lower_Vcent_Range*30)/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV][u1BitIdx];
                minEyeScanVcentLowerBound_bit = u1BitIdx;
            }
        }
#ifdef FOR_HQA_TEST_USED
#ifdef FOR_HQA_REPORT_USED
        print_EyeScanVcent_for_HQA_report_used(p, print_type, u1ChannelIdx, u1RankIdx, u1ByteIdx, EyeScanVcent, minEyeScanVcentUpperBound, minEyeScanVcentUpperBound_bit, minEyeScanVcentLowerBound, minEyeScanVcentLowerBound_bit);
#endif
#endif      
    }


#endif


    if ((print_type==0 && u1CBTEyeScanEnable) || (print_type==1 && u1RXEyeScanEnable) || (print_type==2 && u1TXEyeScanEnable))
    {   U8 space_num;

        mcSHOW_DBG_MSG(("\n\n"));

        if (print_type==0 || print_type==1)
        {
            if (print_type==0) u2DQDelayBegin = 0;
            else u2DQDelayBegin = -32; //gEyeScan_Min[gFinalRXVrefDQ[u1ChannelIdx][u1RankIdx]][u1BitIdx][EyeScan_Index]-5; 
            u2DQDelayEnd = 64;

            space_num = 8+one_pi_ps*32/1000;
        }
        else 
//        if (print_type==2)
        {
            if(gEyeScan_CaliDelay[0] <gEyeScan_CaliDelay[1])
                u2DQDelayBegin = gEyeScan_CaliDelay[0]-24;
            else
                u2DQDelayBegin = gEyeScan_CaliDelay[1]-24;

            u2DQDelayEnd = u2DQDelayBegin + 64;

            space_num = 15+u2DQDelayEnd-u2DQDelayBegin+2;
        }

        for (u1BitIdx=u1ByteIdx*DQS_BIT_NUMBER; u1BitIdx<(u1ByteIdx*DQS_BIT_NUMBER+cal_length); u1BitIdx++)
        {
            EyeScan_Index = 0;

            if (print_type==1)
            {
                Min_Value_1UI_Line = gEyeScan_DelayCellPI[u1BitIdx]-16;

                EYESCAN_LOG_Print_Prefix_String(); mcSHOW_EYESCAN_MSG(("RX EYESCAN Channel%d, Rank%d, DQ%d ===\n",p->channel, p->rank, u1BitIdx));
            }
            else
            {
#if EyeScan_Pic_draw_line_Mirror
                EyeScan_DelayCellPI_value = 0-gEyeScan_DelayCellPI[u1BitIdx];
#else
                EyeScan_DelayCellPI_value = gEyeScan_DelayCellPI[u1BitIdx];
#endif
                Min_Value_1UI_Line = gEyeScan_CaliDelay[u1BitIdx/8]-16-EyeScan_DelayCellPI_value;

                EYESCAN_LOG_Print_Prefix_String(); mcSHOW_EYESCAN_MSG(("%s EYESCAN Channel%d, Rank%d, Bit%d", print_type==0 ? "CBT" : "TX DQ", p->channel, p->rank, u1BitIdx));
                if (print_type==2) 
                {
                    mcSHOW_EYESCAN_MSG(("(DRAM DQ%d)", uiLPDDR4_O1_Mapping_POP[p->channel][u1BitIdx]));
                }
                mcSHOW_EYESCAN_MSG((" ===\n"));
            }


#if VENDER_JV_LOG
            for(i=0; i<8+one_pi_ps*32/1000; i++) mcSHOW_EYESCAN_MSG((" "));
            mcSHOW_EYESCAN_MSG(("window\n"));
#else
            for(i=0; i<space_num; i++) mcSHOW_EYESCAN_MSG((" "));
            mcSHOW_EYESCAN_MSG(("first last window\n"));
#endif

                if (print_type==1) u1VrefRange=VREF_RANGE_0;
                else u1VrefRange=VREF_RANGE_1;
                for (u1VrefIdx=(scan_vref_number - (scan_vref_number==VREF_VOLTAGE_TABLE_NUM ? 1 : 0)) & ~0x1; u1VrefIdx>=0; u1VrefIdx-=EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV)
                {
                        EyeScan_Pic_draw_line(p, print_type, u1VrefRange, u1VrefIdx, u1BitIdx, u2DQDelayBegin, u2DQDelayEnd, finalVrefRange, finalVref, VdlVWHigh_Upper_Vcent_Range, VdlVWHigh_Upper_Vcent, VdlVWHigh_Lower_Vcent_Range, VdlVWHigh_Lower_Vcent, gEyeScan_CaliDelay[u1BitIdx/8], gEyeScan_DelayCellPI[u1BitIdx], one_pi_ps, Min_Value_1UI_Line);

                        if (print_type!=1 && u1VrefRange==VREF_RANGE_1 && u1VrefIdx==(21-(EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV-1)))
                        {
                            u1VrefRange=VREF_RANGE_0;
                            u1VrefIdx=VREF_VOLTAGE_TABLE_NUM-(EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV-1);
                        }
                }

            mcSHOW_EYESCAN_MSG(("\n\n"));

        }
    }
}
}

 
#ifdef RELEASE
#undef mcSHOW_DBG_MSG
#define mcSHOW_DBG_MSG(_x_)
#endif
#endif


#if GATING_ONLY_FOR_DEBUG
void DramcGatingDebugRankSel(DRAMC_CTX_T *p, U8 u1Rank)
{
    if (p->support_rank_num==RANK_SINGLE)
    {
        u1Rank =0;
    }

    vIO32WriteFldAlign_All(DDRPHY_MISC_CTRL1, u1Rank, MISC_CTRL1_R_DMSTBENCMP_RK_OPT);
}

void DramcGatingDebugInit(DRAMC_CTX_T *p)
{
    U32 backupReg0x64[CHANNEL_NUM], backupReg0xC8[CHANNEL_NUM], backupReg0xD0[CHANNEL_NUM];
    U32 backupRegShu_SCINTV[DRAM_DFS_SHUFFLE_MAX][CHANNEL_NUM];
    U32 channel_idx, backup_channel, shu_index;

    backup_channel = vGetPHY2ChannelMapping(p);
    
    for(channel_idx=CHANNEL_A; channel_idx<p->support_channel_num; channel_idx++)
    {
        vSetPHY2ChannelMapping(p, channel_idx);
        backupReg0x64[channel_idx] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL));
        backupReg0xC8[channel_idx] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR));
        backupReg0xD0[channel_idx] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD));
        for(shu_index = DRAM_DFS_SHUFFLE_1; shu_index < DRAM_DFS_SHUFFLE_MAX; shu_index++)
            backupRegShu_SCINTV[shu_index][channel_idx] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SHU_SCINTV) + SHU_GRP_DRAMC_OFFSET*shu_index);
    }
    vSetPHY2ChannelMapping(p, backup_channel);

    //Disable MR4 MR18/MR19, TxHWTracking, Dummy RD before reset
    vIO32WriteFldAlign_All(DRAMC_REG_SPCMDCTRL, 0x1, SPCMDCTRL_REFRDIS);    //MR4 Disable
    vIO32WriteFldAlign_All(DRAMC_REG_DQSOSCR, 0x1, DQSOSCR_DQSOSCRDIS);  //MR18, MR19 Disable
    for(shu_index = DRAM_DFS_SHUFFLE_1; shu_index < DRAM_DFS_SHUFFLE_MAX; shu_index++)
        vIO32WriteFldAlign_All(DRAMC_REG_SHU_SCINTV + SHU_GRP_DRAMC_OFFSET*shu_index, 0x1, SHU_SCINTV_DQSOSCENDIS);
    vIO32WriteFldMulti_All(DRAMC_REG_DUMMY_RD, P_Fld(0x0, DUMMY_RD_DUMMY_RD_EN)
                                            | P_Fld(0x0, DUMMY_RD_SREF_DMYRD_EN)
                                            | P_Fld(0x0, DUMMY_RD_DQSG_DMYRD_EN)
                                            | P_Fld(0x0, DUMMY_RD_DMY_RD_DBG));
    mcDELAY_US(4);

    DramPhyReset(p);

    //Restore backup regs
    for(channel_idx=CHANNEL_A; channel_idx<p->support_channel_num; channel_idx++)
    {
        vSetPHY2ChannelMapping(p, channel_idx);
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), backupReg0x64[channel_idx]);
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR), backupReg0xC8[channel_idx]);
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD), backupReg0xD0[channel_idx]);
        for(shu_index = DRAM_DFS_SHUFFLE_1; shu_index < DRAM_DFS_SHUFFLE_MAX; shu_index++)
            vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU_SCINTV) + SHU_GRP_DRAMC_OFFSET*shu_index, backupRegShu_SCINTV[shu_index][channel_idx]);
    }
    vSetPHY2ChannelMapping(p, backup_channel);


    //enable &reset DQS counter
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1, SPCMD_DQSGCNTEN);
    mcDELAY_US(4);//wait 1 auto refresh after DQS Counter enable

    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1, SPCMD_DQSGCNTRST);
    mcDELAY_US(1);//delay 2T
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_DQSGCNTRST);
    //mcSHOW_DBG_MSG(("DramcGatingDebugInit done\n" ));
}

void DramcGatingDebugExit(DRAMC_CTX_T *p)
{
    //enable &reset DQS counter
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_DQSGCNTEN);
    vIO32WriteFldAlign_All(DDRPHY_MISC_CTRL1, 0, MISC_CTRL1_R_DMSTBENCMP_RK_OPT);
}

void DramcGatingDebug(DRAMC_CTX_T *p, U8 u1Channel)
{
    U32 LP3_DataPerByte[DQS_NUMBER];
    U32 u4DebugCnt[DQS_NUMBER];
    U16 u2DebugCntPerByte;

    U32 u4value, u4all_result_R, u4all_result_F, u4err_value;
    U8 backup_channel;

    backup_channel = p->channel;
    vSetPHY2ChannelMapping(p, u1Channel);

    mcDELAY_MS(10);

    LP3_DataPerByte[0] = (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_STBERR_RK0_R), MISC_STBERR_RK0_R_STBERR_RK0_R));
    LP3_DataPerByte[2] = (LP3_DataPerByte[0] >>8) & 0xff;
    LP3_DataPerByte[0] &= 0xff;

    u4all_result_R = LP3_DataPerByte[0] | (LP3_DataPerByte[2] <<8);

    // falling
    LP3_DataPerByte[0] = (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_STBERR_RK0_F), MISC_STBERR_RK0_F_STBERR_RK0_F));
    LP3_DataPerByte[2] = (LP3_DataPerByte[0] >>8) & 0xff;
    LP3_DataPerByte[0] &= 0xff;

    u4all_result_F = LP3_DataPerByte[0] | (LP3_DataPerByte[2] <<8);

     //read DQS counter
    u4DebugCnt[0] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DQSGNWCNT0));
    u4DebugCnt[1] = (u4DebugCnt[0] >> 16) & 0xffff;
    u4DebugCnt[0] &= 0xffff;

    u4DebugCnt[2] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DQSGNWCNT1));
    u4DebugCnt[3] = (u4DebugCnt[2] >> 16) & 0xffff;
    u4DebugCnt[2] &= 0xffff;

    mcSHOW_DBG_MSG(("\n[GatingDebug] CH %d, DQS count (B3->B0) 0x%H, 0x%H, 0x%H, 0x%H \nError flag Rank0 (B3->B0) %B %B  %B %B  %B %B  %B %B\n", \
                                   u1Channel, u4DebugCnt[3], u4DebugCnt[2], u4DebugCnt[1], u4DebugCnt[0], \
                                   (u4all_result_F>>24)&0xff, (u4all_result_R>>24)&0xff, \
                                   (u4all_result_F>>16)&0xff, (u4all_result_R>>16)&0xff, \
                                   (u4all_result_F>>8)&0xff,   (u4all_result_R>>8)&0xff, \
                                   (u4all_result_F)&0xff,         (u4all_result_R)&0xff));

    if (p->support_rank_num==RANK_DUAL)
    {
        LP3_DataPerByte[0] = (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_STBERR_RK1_R), MISC_STBERR_RK1_R_STBERR_RK1_R));//PHY_B
        LP3_DataPerByte[2] = (LP3_DataPerByte[0] >>8) & 0xff;
        LP3_DataPerByte[0] &= 0xff;

        u4all_result_R = LP3_DataPerByte[0] | (LP3_DataPerByte[2] <<8);

        // falling
        LP3_DataPerByte[0] = (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_STBERR_RK1_F), MISC_STBERR_RK1_F_STBERR_RK1_F));//PHY_B
        LP3_DataPerByte[2] = (LP3_DataPerByte[0] >>8) & 0xff;
        LP3_DataPerByte[0] &= 0xff;

        u4all_result_F = LP3_DataPerByte[0];

        mcSHOW_DBG_MSG(("Error flag Rank1 (B3->B0) %B %B  %B %B  %B %B  %B %B\n", \
                                       (u4all_result_F>>24)&0xff, (u4all_result_R>>24)&0xff, \
                                       (u4all_result_F>>16)&0xff, (u4all_result_R>>16)&0xff, \
                                       (u4all_result_F>>8)&0xff,   (u4all_result_R>>8)&0xff, \
                                       (u4all_result_F)&0xff,         (u4all_result_R)&0xff));
    }
    else
    {
        mcSHOW_DBG_MSG(("\n"));
    }

    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1, SPCMD_DQSGCNTRST);
    //mcDELAY_US(1);//delay 2T
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_DQSGCNTRST);

    vSetPHY2ChannelMapping(p, backup_channel);
}
#endif

#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
#if (__ETT__ || CPU_RW_TEST_AFTER_K)
void DramcDumpDebugInfo(DRAMC_CTX_T *p)
{
    U8 mpdivInSel, cali_shu_sel, mpdiv_shu_sel, u1RefreshRate;
    DRAM_CHANNEL_T channelIdx;

    mpdivInSel= u4IO32ReadFldAlign(DDRPHY_MISC_CTRL2, MISC_CTRL2_CLRPLL_SHU_GP);
    cali_shu_sel= u4IO32ReadFldAlign(DRAMC_REG_SHUCTRL, SHUCTRL_R_OTHER_SHU_GP);
    mpdiv_shu_sel= u4IO32ReadFldAlign(DRAMC_REG_SHUCTRL, SHUCTRL_R_MPDIV_SHU_GP);

    // Read shuffle selection
    mcSHOW_ERR_MSG(("\n\n[DumpDebugInfo]\n"
                    "\tmpdivInSel %d, cali_shu_sel %d, mpdiv_shu_sel %d\n",
                         mpdivInSel, cali_shu_sel, mpdiv_shu_sel));

    #if GATING_ONLY_FOR_DEBUG
    // Read gating error flag
    //DramcGatingDebugInit(p);
    for(channelIdx=CHANNEL_A; channelIdx< p->support_channel_num; channelIdx++)
    {
        DramcGatingDebug(p, channelIdx);
    }
    #endif

#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
    // Read HW gating tracking
#ifdef HW_GATING
    for(channelIdx=CHANNEL_A; channelIdx< p->support_channel_num; channelIdx++)
        {
        DramcPrintHWGatingStatus(p, channelIdx);
        }
#endif

    DramcPrintRXFIFODebugStatus(p);

#if (ENABLE_RX_TRACKING_LP4 && RX_DLY_TRACK_ONLY_FOR_DEBUG && defined(DUMMY_READ_FOR_TRACKING))
    DramcPrintRxDlyTrackDebugStatus(p);
#endif

#if ENABLE_RX_TRACKING_LP4
    for(channelIdx=CHANNEL_A; channelIdx< p->support_channel_num; channelIdx++)
        {
        DramcPrintRXDQDQSStatus(p, channelIdx);
        }
#endif

#ifdef IMPEDANCE_TRACKING_ENABLE
        if(u1IsLP4Family(p->dram_type))
        {
        for(channelIdx=CHANNEL_A; channelIdx< p->support_channel_num; channelIdx++)
        {
            DramcPrintIMPTrackingStatus(p, channelIdx);
        }
        
        }
#endif

#ifdef TEMP_SENSOR_ENABLE
    for(channelIdx=CHANNEL_A; channelIdx< p->support_channel_num; channelIdx++)
    {
        u1RefreshRate = u1GetMR4RefreshRate(p, channelIdx);
        mcSHOW_ERR_MSG(("[CH%d] MRR(MR4) [10:8]=%x\n", channelIdx, u1RefreshRate));
    }      
#endif
#endif//(FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
}
#endif
#endif


#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
#if __ETT__
//-------------------------------------------------------------------------
/** DramcRegDump
 *  Dump all registers (DDRPHY and DRAMC)
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL
 */
//-------------------------------------------------------------------------
void DramcRegDumpRange(U32 u4StartAddr, U32 u4EndAddr)
{
    U32 ii;

    for(ii = u4StartAddr; ii <= u4EndAddr; ii += 4)
    {
        mcSHOW_DBG_MSG(("Reg(0x%xh) Address 0x%X = 0x%X\n", (ii&0xfff)>>2, ii, (*(volatile unsigned int *)(ii))));
        mcDELAY_US(20000); //Large delay to prevent UART overflow
    }
}

/* Calculate regs dump range for each type
 * Using last register definition of each group
 * Ex: Bian_co DramC NAO last register definition
 *     #define DRAMC_REG_RK2_B23_STB_DBG_INFO_15 (DRAMC_NAO_BASE_ADDRESS + 0x08FC)
 *     Dump range end offset would be (DRAMC_REG_RK2_B23_STB_DBG_INFO_15 - DRAMC_NAO_BASE_ADDRESS)
 */
 #if (fcFOR_CHIP_ID == fcLafite)
#define DRAMC_NAO_DUMP_RANGE (DRAMC_REG_RK2_B23_STB_DBG_INFO_15 - DRAMC_NAO_BASE_ADDRESS)
#define DDRPHY_NAO_DUMP_RANGE (DDRPHY_MISC_MBIST_STATUS2 - DDRPHY_NAO_BASE_ADDR)

#define DRAMC_AO_NONSHU_DUMP_RANGE (DRAMC_REG_RK1_PRE_TDQSCK27 - DRAMC_AO_BASE_ADDRESS)
#define DRAMC_AO_SHU_BASE (DRAMC_REG_SHU_ACTIM0 - DRAMC_AO_BASE_ADDRESS)
#define DRAMC_AO_SHU_DUMP_RANGE (DRAMC_REG_SHURK1_DQS2DQ_CAL5 - DRAMC_REG_SHU_ACTIM0)

#define DDRPHY_AO_NONSHU_DUMP_RANGE (DDRPHY_RFU_0XB2C - DDRPHY_AO_BASE_ADDR)
#define DDRPHY_AO_SHU_BASE (DDRPHY_SHU_B0_DQ0 - DDRPHY_AO_BASE_ADDR)
#define DDRPHY_AO_SHU_DUMP_RANGE (DDRPHY_SHU_R2_CA_CMD9 - DDRPHY_SHU_B0_DQ0)
#endif

DRAM_STATUS_T DramcRegDump(DRAMC_CTX_T *p, DRAM_DFS_SRAM_SHU_T sram_shu_level)
{
     /* Must modify Dump range end address accordingly for each project */
     /* Tip, add mcSHOW_DBG_MSGDUMP and set a large delay value for DramcRegDump related message output */
    U32 u4DramcShuOffset=0, u4DDRPhyShuOffset=0;

    if(gAndroid_DVFS_en)
    {
        // Aceess DMA SRAM by APB bus use debug mode by conf SHU3
        vIO32WriteFldAlign_All(DDRPHY_SRAM_DMA0, 0x1, SRAM_DMA0_R_APB_SLV_SEL);
        vIO32WriteFldAlign_All(DDRPHY_SRAM_DMA2, 0x1, SRAM_DMA2_R_APB_DMA_DBG_ACCESS);
        vIO32WriteFldAlign_All(DDRPHY_SRAM_DMA2, sram_shu_level, SRAM_DMA2_R_APB_DMA_DBG_LEVEL);
        u4DramcShuOffset = DRAMC_AO_SHU_BASE + (SHU_GRP_DRAMC_OFFSET*3); //SHU3
        u4DDRPhyShuOffset = DDRPHY_AO_SHU_BASE + (SHU_GRP_DDRPHY_OFFSET*3); //SHU3
    }
    else
    {
        u4DramcShuOffset = DRAMC_AO_SHU_BASE; 
        u4DDRPhyShuOffset = DDRPHY_AO_SHU_BASE; 
    }

    mcSHOW_DBG_MSG(("\n\n\tCHA_DRAMC_NAO_BASE\n"));
    DramcRegDumpRange(Channel_A_DRAMC_NAO_BASE_ADDRESS, Channel_A_DRAMC_NAO_BASE_ADDRESS + DRAMC_NAO_DUMP_RANGE);
    mcSHOW_DBG_MSG(("\n\tCHB_DRAMC_NAO_BASE\n"));
    DramcRegDumpRange(Channel_B_DRAMC_NAO_BASE_ADDRESS, Channel_B_DRAMC_NAO_BASE_ADDRESS + DRAMC_NAO_DUMP_RANGE);

    mcSHOW_DBG_MSG(("\n\tCHA_DRAMC_AO_NONSHU_BASE\n"));
    DramcRegDumpRange(Channel_A_DRAMC_AO_BASE_ADDRESS, Channel_A_DRAMC_AO_BASE_ADDRESS + DRAMC_AO_NONSHU_DUMP_RANGE);
    mcSHOW_DBG_MSG(("\n\tCHB_DRAMC_AO_NONSHU_BASE\n"));
    DramcRegDumpRange(Channel_B_DRAMC_AO_BASE_ADDRESS, Channel_B_DRAMC_AO_BASE_ADDRESS + DRAMC_AO_NONSHU_DUMP_RANGE);

    mcSHOW_DBG_MSG(("\n\tCHA_DRAMC_AO_SHU_BASE\n"));
    DramcRegDumpRange(Channel_A_DRAMC_AO_BASE_ADDRESS + u4DramcShuOffset, Channel_A_DRAMC_AO_BASE_ADDRESS + u4DramcShuOffset + DRAMC_AO_SHU_DUMP_RANGE);
    mcSHOW_DBG_MSG(("\n\tCHB_DRAMC_AO_SHU_BASE\n"));
    DramcRegDumpRange(Channel_B_DRAMC_AO_BASE_ADDRESS + u4DramcShuOffset, Channel_B_DRAMC_AO_BASE_ADDRESS + u4DramcShuOffset + DRAMC_AO_SHU_DUMP_RANGE);

    mcSHOW_DBG_MSG(("\n\tCHA_PHY_NAO_BASE\n"));
    DramcRegDumpRange(Channel_A_PHY_NAO_BASE_ADDRESS, Channel_A_PHY_NAO_BASE_ADDRESS + DDRPHY_NAO_DUMP_RANGE);
    mcSHOW_DBG_MSG(("\n\tCHB_PHY_NAO_BASE\n"));
    DramcRegDumpRange(Channel_B_PHY_NAO_BASE_ADDRESS, Channel_B_PHY_NAO_BASE_ADDRESS + DDRPHY_NAO_DUMP_RANGE);

    mcSHOW_DBG_MSG(("\n\tCHA_PHY_AO_NONSHU_BASE\n"));
    DramcRegDumpRange(Channel_A_PHY_AO_BASE_ADDRESS, Channel_A_PHY_AO_BASE_ADDRESS + DDRPHY_AO_NONSHU_DUMP_RANGE);
    mcSHOW_DBG_MSG(("\n\tCHB_PHY_AO_NONSHU_BASE\n"));
    DramcRegDumpRange(Channel_B_PHY_AO_BASE_ADDRESS, Channel_B_PHY_AO_BASE_ADDRESS + DDRPHY_AO_NONSHU_DUMP_RANGE);

    mcSHOW_DBG_MSG(("\n\tCHA_PHY_AO_SHU_BASE\n"));
    DramcRegDumpRange(Channel_A_PHY_AO_BASE_ADDRESS + u4DDRPhyShuOffset, Channel_A_PHY_AO_BASE_ADDRESS + u4DDRPhyShuOffset + DDRPHY_AO_SHU_DUMP_RANGE);
    mcSHOW_DBG_MSG(("\n\tCHB_PHY_AO_SHU_BASE\n"));
    DramcRegDumpRange(Channel_B_PHY_AO_BASE_ADDRESS + u4DDRPhyShuOffset, Channel_B_PHY_AO_BASE_ADDRESS + u4DDRPhyShuOffset + DDRPHY_AO_SHU_DUMP_RANGE);

    return DRAM_OK;
}


#if ETT_NO_DRAM
void NoDramDramcRegDumpRange(U32 u4StartAddr, U32 u4EndAddr)
{
    U32 ii;

    for(ii=u4StartAddr; ii<u4EndAddr; ii+=4)
    {
        mcSHOW_DBG_MSG(("*(volatile unsigned int *)0x%X = 0x%X;\n", ii, (*(volatile unsigned int *)(ii))));
        //mcSHOW_DBG_MSG(("mcSHOW_DBG_MSG((\"0x%X\"));\n", ii));
    }
}

DRAM_STATUS_T NoDramDramcRegDump(DRAMC_CTX_T *p)
{
    mcSHOW_DBG_MSG(("\n\n\tCHA_DRAMC_AO_BASE\n"));
    NoDramDramcRegDumpRange(Channel_A_DRAMC_AO_BASE_ADDRESS, Channel_A_DRAMC_AO_BASE_ADDRESS+0x1E58);
    mcSHOW_DBG_MSG(("\n\tCHB_DRAMC_AO_BASE\n"));
    NoDramDramcRegDumpRange(Channel_B_DRAMC_AO_BASE_ADDRESS, Channel_B_DRAMC_AO_BASE_ADDRESS+0x1E58);
    mcSHOW_DBG_MSG(("\n\tPHY_A_BASE\n"));
    NoDramDramcRegDumpRange(Channel_A_PHY_BASE_ADDRESS, Channel_A_PHY_BASE_ADDRESS+0x1FC8);
    mcSHOW_DBG_MSG(("\n\tPHY_B_BASE\n"));
    NoDramDramcRegDumpRange(Channel_B_PHY_BASE_ADDRESS, Channel_B_PHY_BASE_ADDRESS+0x1FC8);

    return DRAM_OK;
}

DRAM_STATUS_T NoDramRegFill(void)
{
}
#endif
#endif
#endif

#if DRAMC_MODEREG_CHECK
void DramcModeReg_Check(DRAMC_CTX_T *p)
{
    U8 backup_channel, backup_rank;
    U8 u1ChannelIdx, u1RankIdx;
    U8 u1MRFsp, operating_fsp = p->dram_fsp;
    U8 ii, u1MR[] = {5,12,14,4,18,19}; //MR5, MR12, MR14, MR18, MR19
    U16 u2MRValue = 0, u2Value = 0;
    U8 u1match=0;
    U8 backup_u1MR13Value=0;

    mcSHOW_DBG_MSG(("\n\n[DramcModeReg_Check] Freq_%d, FSP_%d\n",p->frequency,p->dram_fsp));

    backup_channel = vGetPHY2ChannelMapping(p);
    backup_rank = u1GetRank(p);

    if (u1IsLP4Family(p->dram_type))
    {
        backup_u1MR13Value = u1MR13Value;

        #if MRW_CHECK_ONLY
        mcSHOW_MRW_MSG(("\n==[MR Dump] %s==\n", __func__));
        #endif
        
        for (u1MRFsp=FSP_0; u1MRFsp<FSP_MAX; u1MRFsp++)
        {            
            for(u1ChannelIdx=0; u1ChannelIdx<(p->support_channel_num); u1ChannelIdx++)
            {
                vSetPHY2ChannelMapping(p, u1ChannelIdx);
    
                for(u1RankIdx =0; u1RankIdx < (U32)(p->support_rank_num); u1RankIdx++)
                {            
                    mcSHOW_DBG_MSG(("FSP_%d, CH_%d, RK%d\n",u1MRFsp,u1ChannelIdx,u1RankIdx));
                    
                    if (u1MRFsp==FSP_1)
                        u1MR13Value |= 0x40;   //Read/Write FSP
                    else
                        u1MR13Value &= (~0x40);   //Read/Write FSP
                    
                    DramcModeRegWriteByRank(p, u1RankIdx, 13, u1MR13Value);

                    for (ii=0; ii<sizeof(u1MR);ii++)
                    {
                        DramcModeRegReadByRank(p, u1RankIdx, u1MR[ii], &u2Value);
                        u2Value &= 0xFF;
    
                        if ((u1MR[ii]==12)||(u1MR[ii]==14)) //need to compare final setting with global variants
                        {
                            if (u1MR[ii]==12)
                            {
                                u2MRValue = u1MR12Value[u1ChannelIdx][u1RankIdx][u1MRFsp];
                            }
                            else if (u1MR[ii]==14)
                            {
                                u2MRValue = u1MR14Value[u1ChannelIdx][u1RankIdx][u1MRFsp];
                            }
                            u1match = (u2Value==u2MRValue)?1:0;
    
    #ifdef FOR_HQA_REPORT_USED
                            if (gHQALog_flag==1)
                            {
                                HQA_Log_Message_for_Report(p, u1ChannelIdx, u1RankIdx, HQA_REPORT_FORMAT1, "DramcModeReg_Read_FSP_", u1MRFsp==0?"0_MR":"1_MR", u1MR[ii], u2Value, NULL);
                            }
                            else
    #endif
                            {
                                mcSHOW_DBG_MSG(("\t\tMR%d = 0x%x (global = 0x%x)\t%smatch\n",u1MR[ii],u2Value,u2MRValue, (u1match==1)?"":"mis"));
                            }
                        }
                        else
                        {
    #ifdef FOR_HQA_REPORT_USED
                            if (gHQALog_flag==1)
                            {
                                HQA_Log_Message_for_Report(p, u1ChannelIdx, u1RankIdx, HQA_REPORT_FORMAT1, "DramcModeReg_Read_FSP_", u1MRFsp==0?"0_MR":"1_MR", u1MR[ii], u2Value, NULL);
                            }
                            else
    #endif
                            {
                                char *str_vender="";
                                if (u1MR[ii]==5)
                                {
                                    //Vendor ID 1: Samsung, 6: Hynix
                                    str_vender = (u2Value==1)?"Samsung":(u2Value==6)?"Hynix":"mismatch";
                                }
                                mcSHOW_DBG_MSG(("\t\tMR%d = 0x%x %s\n",u1MR[ii],u2Value,str_vender));
                            }
                        }
    
                    }
                }
            }
        }

        // resotre MR13 settings
        u1MR13Value = backup_u1MR13Value;
        for(u1ChannelIdx=0; u1ChannelIdx<(p->support_channel_num); u1ChannelIdx++)
        {
            vSetPHY2ChannelMapping(p, u1ChannelIdx);
            for(u1RankIdx =0; u1RankIdx < (U32)(p->support_rank_num); u1RankIdx++)
                DramcModeRegWriteByRank(p, u1RankIdx, 13, backup_u1MR13Value);
        }
    }

    vSetPHY2ChannelMapping(p, backup_channel);
    vSetRank(p, backup_rank);
    return;
}
#endif

#if MRW_CHECK_ONLY
void vPrintFinalModeRegisterSetting(DRAMC_CTX_T * p)
{
    U8 u1CHIdx, u1RankIdx, u1FSPIdx, u1MRIdx;
    U16 u2MRValue;

    mcSHOW_MRW_MSG(("\n\n\n==vPrintFinalModeRegisterSetting====\n"));

    for(u1CHIdx=0; u1CHIdx<CHANNEL_NUM; u1CHIdx++)
        for(u1RankIdx=0; u1RankIdx<RANK_MAX; u1RankIdx++)
            for(u1FSPIdx=0; u1FSPIdx<FSP_MAX; u1FSPIdx++)
            {
                for(u1MRIdx=0; u1MRIdx<MR_NUM; u1MRIdx++)
                {
                    u2MRValue = u2MRRecord[u1CHIdx][u1RankIdx][u1FSPIdx][u1MRIdx];
                    if(u2MRValue != 0xffff) //value changed
                    {
                        mcSHOW_MRW_MSG(("[MR Dump] CH%d Rank%d Fsp%d MR%d =0x%x\n", u1CHIdx, u1RankIdx, u1FSPIdx, u1MRIdx, u2MRValue));
                    }
                }
                mcSHOW_MRW_MSG(("\n"));
            }

}
#endif

#if CHECK_GOLDEN_SETTING
typedef struct _GOLDEN_FIELD_T
{
    char field[64];//field name
    U32 u4ChaValue;
    U32 u4ChbValue;    
} GOLDEN_FIELD_T;

#if APPLY_LOWPOWER_GOLDEN_SETTINGS
GOLDEN_FIELD_T golden_setting_anwer[] =
{
    {"COMBCLKCTRL", 0x1, 0x1},
    {"PHYCLKDYNGEN", 0x1, 0x1},
    {"MIOCKCTRLOFF", 0x0, 0x0},
    {"COMBPHY_CLKENSAME", 0x0, 0x0},
    {"DCMENNOTRFC", 0x1, 0x1},
    {"DCMEN2", 0x1, 0x1},
    {"DCMEN", 0x1, 0x1},
    {"RDYCKAR", 0x0, 0x0},
    {"CMDCKAR", 0x0, 0x0},
    {"CALCKAR", 0x0, 0x0},
    {"SEQCLKRUN", 0x1, 0x1},
    {"REQQUECLKRUN", 0x0, 0x0},
    {"REFCLKRUN", 0x0, 0x0},
    {"DWCLKRUN", 0x0, 0x0},
    {"PHYGLUECLKRUN", 0x0, 0x0},
    {"TESTCLKRUN", 0x0, 0x0},
    {"BCLKAR", 0x0, 0x0},
    {"PSELAR", 0x0, 0x0},
    {"RDATCKAR", 0x0, 0x0},
    {"SELPH_CMD_CG_DIS", 0x0, 0x0},
    {"REQQUE_PACG_DIS", 0x0, 0x0},
    {"SCSM_CGAR", 0x0, 0x0},
    {"SCARB_SM_CGAR", 0x0, 0x0},
    {"RDDQSOSC_CGAR", 0x0, 0x0},
    {"HMRRSEL_CGAR", 0x0, 0x0},
    {"SREF_CG_OPT", 0x0, 0x0},
    {"ZQMASK_CGAR", 0x0, 0x0},
    {"ZQCS_MASK_SEL_CGAR", 0x0, 0x0},
    {"DUMMY_RD_PA_OPT", 0x1, 0x1},
    {"DVFS_CG_OPT", 0x0, 0x0},
    {"TXUIPI_CAL_CGAR", 0x0, 0x0},
    {"DDRPHY_CLK_EN_COMB_TX_OPT", 0x0, 0x0},
    {"RG_RX_ARDQS0_STBEN_RESETB_B0", 0x1, 0x1},
    {"R_DM_TX_MCK_FRUN_B0", 0x0, 0x0},
    {"RG_RX_ARDQS0_STBEN_RESETB_B1", 0x1, 0x1},
    {"R_DM_TX_MCK_FRUN_B1", 0x0, 0x0},
    {"RG_ARPISM_MCK_SEL_CA_REG_OPT", 0x0, 0x0},
    {"R_DM_TX_MCK_FRUN_CA", 0x11C0, 0x11C0},
    {"RG_GATING_EMI_NEW", 0x0, 0x0},
    {"RG_52M_104M_SEL", 0x1, 0x1},
    {"RG_CG_INFRA_OFF_DISABLE", 0x0, 0x0},
    {"RG_CG_RX_COMB1_OFF_DISABLE", 0x0, 0x0},
    {"RG_CG_RX_COMB0_OFF_DISABLE", 0x0, 0x0},
    {"RG_CG_RX_CMD_OFF_DISABLE", 0x0, 0x0},
    {"RG_CG_COMB1_OFF_DISABLE", 0x0, 0x0},
    {"RG_CG_COMB0_OFF_DISABLE", 0x0, 0x0},
    {"RG_CG_CMD_OFF_DISABLE", 0x0, 0x0},
    {"RG_CG_COMB_OFF_DISABLE", 0x0, 0x0},
    {"RG_CG_PHY_OFF_DIABLE", 0x0, 0x0},
    {"RG_CG_DRAMC_OFF_DISABLE", 0x0, 0x0},
    {"RG_CG_EMI_OFF_DISABLE", 0x1, 0x1},
    {"RG_MEM_DCM_FORCE_OFF", 0x0, 0x0},
    {"RG_MEM_DCM_FORCE_ON", 0x0, 0x0},
    {"R_PHY_MCK_CG_CTRL", 0x11400000, 0x11400000},
    {"R_CA_PI_DCM_EN", 0x1, 0x1},
    {"R_DQ0_PI_DCM_EN", 0x1, 0x1},
    {"R_DQ1_PI_DCM_EN", 0x1, 0x1},
    {"R_CA_DLY_DCM_EN", 0x1, 0x1},
    {"R_DQ0_DLY_DCM_EN", 0x1, 0x1},
    {"R_DQ1_DLY_DCM_EN", 0x1, 0x1},
    {"R_DMSHU_PHYDCM_FORCEOFF", 0x1, 0x1},
    {"R_DMDA_RRESETB_E", 0x1, 0x1},
    {"R_WL_DOWNSP", 0x0, 0x0},
    {"R_DMDRAMCLKEN1", 0x0, 0x0},
    {"R_DMDRAMCLKEN0", 0x0, 0x0},
    {"R_DMMUXCA_SEC", 0x0, 0x0},
    {"R_DMDA_RRESETB_I", 0x1, 0x1},
    {"R_DMRRESETB_I_OPT", 0x1, 0x1},
    {"CK_BFE_DCM_EN", 0x0, 0x0},
    {"R_DMARPICA_SW_UPDX", 0x0, 0x0},
    {"R_DMPINMUX", 0x0, 0x0},
    {"R_DMMUXCA", 0x0, 0x0},
    {"R_DMMCTLPLL_CKSEL", 0x0, 0x0},
    {"R_DM_TX_ARCMD_OE", 0x1, 0x1},
    {"R_DM_TX_ARCLK_OE", 0x1, 0x1},
    {"R_DMPHYRST", 0x0, 0x0},
    {"R_OPT2_CG_CS", 0x1, 0x1},
    {"R_OPT2_CG_CLK", 0x1, 0x1},
    {"R_OPT2_CG_CMD", 0x1, 0x1},
    {"R_OPT2_CG_DQSIEN", 0x1, 0x1},
    {"R_OPT2_CG_DQ", 0x1, 0x1},
    {"R_OPT2_CG_DQS", 0x1, 0x1},
    {"R_OPT2_CG_DQM", 0x1, 0x1},
    {"R_OPT2_CG_MCK", 0x1, 0x1},
    {"R_OPT2_MPDIV_CG", 0x1, 0x1},
    {"R_SRAM_SLEEPB", 0x1, 0x1},
    {"R_SRAM_ISOINTB", 0x1, 0x1},
    {"R_SRAM_HDEN", 0x0, 0x0},
    {"R_SPM_SRAM_SLP_MSK", 0x0, 0x0},
    {"MBIST_RSTB", 0x0, 0x0},
    {"R_CS_MARK", 0x0, 0x0},
    {"R_MBIST_HOLDB", 0x1, 0x1},
    {"R_SLEEP_TEST", 0x0, 0x0},
    {"R_SLEEP_INV", 0x0, 0x0},
    {"R_SLEEP_R", 0x0, 0x0},
    {"R_SLEEP_W", 0x0, 0x0},
    {"R_MBIST_BACKGROUND", 0x0, 0x0},
    {"R_MBIST_MODE", 0x0, 0x0},
    {"R_MBIST_RPRSTB", 0x1, 0x1},
    {"R_MBIST_RPREG_SEL", 0x0, 0x0},
    {"R_MBIST_RPREG_LOAD", 0x0, 0x0},
    {"R_USE_DEFAULT_DELSEL", 0x1, 0x1},
    {"R_SRAM_DELSEL", 0x2A4, 0x2A4},
    {"R_PHYCTRLDCM", 0x1, 0x1},
    {"R_PHYCTRLMUX", 0x1, 0x1},
    {"RG_MCK4X_O_FB_CK_CG_OFF", 0x1, 0x1},
    {"RG_DDR400_MCK4X_O_FORCE_ON", 0x0, 0x0},
    {"RG_CG_DDR400_MCK4X_O_OFF", 0x1, 0x1},
    {"RG_MCK4X_O_OPENLOOP_MODE_EN", 0x1, 0x1},
    {"RG_MCK4X_Q_FB_CK_CG_OFF", 0x1, 0x1},
    {"RG_DDR400_MCK4X_Q_FORCE_ON", 0x0, 0x0},
    {"RG_CG_DDR400_MCK4X_Q_OFF", 0x1, 0x1},
    {"RG_MCK4X_Q_OPENLOOP_MODE_EN", 0x1, 0x1},
    {"RG_MCK4X_I_FB_CK_CG_OFF", 0x1, 0x1},
    {"RG_DDR400_MCK4X_I_FORCE_ON", 0x0, 0x0},
    {"RG_CG_DDR400_MCK4X_I_OFF", 0x1, 0x1},
    {"RG_MCK4X_I_OPENLOOP_MODE_EN", 0x1, 0x1},
    {"RG_M_CK_OPENLOOP_MODE_EN", 0x1, 0x1},
    {"ARMCTL_CK_OUT_CG_SEL", 0x1, 0x1},
    {"R_DMTX_ARPI_CG_DQM_NEW_B0", 0x1, 0x1},
    {"R_DMTX_ARPI_CG_DQS_NEW_B0", 0x1, 0x1},
    {"R_DMTX_ARPI_CG_DQ_NEW_B0", 0x1, 0x1},
    {"R_DMARPI_CG_FB2DLL_DCM_EN_B0", 0x0, 0x0},
    {"R_DMRANK_CHG_PIPE_CG_IG_B0", 0x0, 0x0},
    {"R_DMRANK_PIPE_CG_IG_B0", 0x0, 0x0},
    {"R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B0", 0x0, 0x0},
    {"R_DMDQSIEN_RDSEL_PIPE_CG_IG_B0", 0x0, 0x0},
    {"R_DMDQSIEN_FLAG_PIPE_CG_IG_B0", 0x0, 0x0},
    {"R_DMDQSIEN_FLAG_SYNC_CG_IG_B0", 0x0, 0x0},
    {"R_DMSTBEN_SYNC_CG_IG_B0", 0x0, 0x0},
    {"R_DMRXDLY_CG_IG_B0", 0x0, 0x0},
    {"R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B0", 0x0, 0x0},
    {"R_DMRXDVS_RDSEL_PIPE_CG_IG_B0", 0x0, 0x0},
    {"R_RMRX_TOPHY_CG_IG_B0", 0x1, 0x1},
    {"R_RMRODTEN_CG_IG_B0", 0x0, 0x0},
    {"R_DMRANK_RXDLY_PIPE_CG_IG_B0", 0x0, 0x0},
    {"RG_ARDLL_PHDET_EN_B0_SHU", 0x1, 0x1},
    {"RG_ARPISM_MCK_SEL_B0_SHU", 0x1, 0x1},
    {"R_DMTX_ARPI_CG_DQM_NEW_B1", 0x1, 0x1},
    {"R_DMTX_ARPI_CG_DQS_NEW_B1", 0x1, 0x1},
    {"R_DMTX_ARPI_CG_DQ_NEW_B1", 0x1, 0x1},
    {"R_DMARPI_CG_FB2DLL_DCM_EN_B1", 0x0, 0x0},
    {"R_DMRANK_CHG_PIPE_CG_IG_B1", 0x0, 0x0},
    {"R_DMRANK_PIPE_CG_IG_B1", 0x0, 0x0},
    {"R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B1", 0x0, 0x0},
    {"R_DMDQSIEN_RDSEL_PIPE_CG_IG_B1", 0x0, 0x0},
    {"R_DMDQSIEN_FLAG_PIPE_CG_IG_B1", 0x0, 0x0},
    {"R_DMDQSIEN_FLAG_SYNC_CG_IG_B1", 0x0, 0x0},
    {"R_DMSTBEN_SYNC_CG_IG_B1", 0x0, 0x0},
    {"R_DMRXDLY_CG_IG_B1", 0x0, 0x0},
    {"R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B1", 0x0, 0x0},
    {"R_DMRXDVS_RDSEL_PIPE_CG_IG_B1", 0x0, 0x0},
    {"R_RMRX_TOPHY_CG_IG_B1", 0x1, 0x1},
    {"R_RMRODTEN_CG_IG_B1", 0x0, 0x0},
    {"R_DMRANK_RXDLY_PIPE_CG_IG_B1", 0x0, 0x0},
    {"RG_ARDLL_PHDET_EN_B1_SHU", 0x1, 0x1},
    {"RG_ARPISM_MCK_SEL_B1_SHU", 0x1, 0x1},
    {"R_DMTX_ARPI_CG_CS_NEW", 0x1, 0x1},
    {"R_DMTX_ARPI_CG_CLK_NEW", 0x0, 0x0},
    {"R_DMTX_ARPI_CG_CMD_NEW", 0x1, 0x1},
    {"R_DMARPI_CG_FB2DLL_DCM_EN_CA", 0x0, 0x0},
    {"R_DMRANKRXDVS_CA", 0x0, 0x0},
    {"R_DMRANK_CHG_PIPE_CG_IG_CA", 0x0, 0x0},
    {"R_DMRANK_PIPE_CG_IG_CA", 0x0, 0x0},
    {"R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_CA", 0x0, 0x0},
    {"R_DMDQSIEN_RDSEL_PIPE_CG_IG_CA", 0x0, 0x0},
    {"R_DMDQSIEN_FLAG_PIPE_CG_IG_CA", 0x0, 0x0},
    {"R_DMDQSIEN_FLAG_SYNC_CG_IG_CA", 0x0, 0x0},
    {"R_RMRX_TOPHY_CG_IG_CA", 0x0, 0x0},
    {"R_RMRODTEN_CG_IG_CA", 0x0, 0x0},
    {"RG_ARDLL_PHDET_EN_CA_SHU", 0x1, 0x1},
    {"RG_ARPISM_MCK_SEL_CA_SHU", 0x1, 0x1},
    {"RG_RPHYPLL_ADA_MCK8X_EN_SHU", 0x1, 0x0},//Modify by KaiHsin, PLL related(only need set CHA)
    {"R_RX_PIPE_BYPASS_EN", 0x0, 0x0},
};
#else
GOLDEN_FIELD_T golden_setting_anwer[] =
{
    {"COMBCLKCTRL", 0x0, 0x0},
    {"PHYCLKDYNGEN", 0x0, 0x0},
    {"MIOCKCTRLOFF", 0x1, 0x1},
    {"COMBPHY_CLKENSAME", 0x1, 0x1},
    {"DCMENNOTRFC", 0x0, 0x0},
    {"DCMEN2", 0x0, 0x0},
    {"DCMEN", 0x0, 0x0},
    {"RDYCKAR", 0x1, 0x1},
    {"CMDCKAR", 0x1, 0x1},
    {"CALCKAR", 0x1, 0x1},
    {"SEQCLKRUN", 0x1, 0x1},
    {"REQQUECLKRUN", 0x1, 0x1},
    {"REFCLKRUN", 0x1, 0x1},
    {"DWCLKRUN", 0x1, 0x1},
    {"PHYGLUECLKRUN", 0x1, 0x1},
    {"TESTCLKRUN", 0x1, 0x1},
    {"BCLKAR", 0x1, 0x1},
    {"PSELAR", 0x1, 0x1},
    {"RDATCKAR", 0x1, 0x1},
    {"SELPH_CMD_CG_DIS", 0x1, 0x1},
    {"REQQUE_PACG_DIS", 0x7FFF, 0x7FFF},
    {"SCSM_CGAR", 0x1, 0x1},
    {"SCARB_SM_CGAR", 0x1, 0x1},
    {"RDDQSOSC_CGAR", 0x1, 0x1},
    {"HMRRSEL_CGAR", 0x1, 0x1},
    {"SREF_CG_OPT", 0x1, 0x1},
    {"ZQMASK_CGAR", 0x0, 0x0},
    {"ZQCS_MASK_SEL_CGAR", 0x1, 0x1},
    {"DUMMY_RD_PA_OPT", 0x1, 0x1},
    {"DVFS_CG_OPT", 0x1, 0x1},
    {"TXUIPI_CAL_CGAR", 0x1, 0x1},
    {"DDRPHY_CLK_EN_COMB_TX_OPT", 0x1, 0x1},
    {"RG_RX_ARDQS0_STBEN_RESETB_B0", 0x1, 0x1},
    {"R_DM_TX_MCK_FRUN_B0", 0x0, 0x0},
    {"RG_RX_ARDQS0_STBEN_RESETB_B1", 0x1, 0x1},
    {"R_DM_TX_MCK_FRUN_B1", 0x0, 0x0},
    {"RG_ARPISM_MCK_SEL_CA_REG_OPT", 0x0, 0x0},
    {"R_DM_TX_MCK_FRUN_CA", 0x11C0, 0x11C0},
    {"RG_GATING_EMI_NEW", 0x0, 0x0},
    {"RG_52M_104M_SEL", 0x1, 0x1},
    {"RG_CG_INFRA_OFF_DISABLE", 0x0, 0x0},
    {"RG_CG_RX_COMB1_OFF_DISABLE", 0x0, 0x0},
    {"RG_CG_RX_COMB0_OFF_DISABLE", 0x0, 0x0},
    {"RG_CG_RX_CMD_OFF_DISABLE", 0x0, 0x0},
    {"RG_CG_COMB1_OFF_DISABLE", 0x0, 0x0},
    {"RG_CG_COMB0_OFF_DISABLE", 0x0, 0x0},
    {"RG_CG_CMD_OFF_DISABLE", 0x0, 0x0},
    {"RG_CG_COMB_OFF_DISABLE", 0x0, 0x0},
    {"RG_CG_PHY_OFF_DIABLE", 0x0, 0x0},
    {"RG_CG_DRAMC_OFF_DISABLE", 0x0, 0x0},
    {"RG_CG_EMI_OFF_DISABLE", 0x1, 0x1},
    {"RG_MEM_DCM_FORCE_OFF", 0x1, 0x1},
    {"RG_MEM_DCM_FORCE_ON", 0x1, 0x1},
    {"R_PHY_MCK_CG_CTRL", 0x11400000, 0x11400000},
    {"R_CA_PI_DCM_EN", 0x0, 0x0},
    {"R_DQ0_PI_DCM_EN", 0x0, 0x0},
    {"R_DQ1_PI_DCM_EN", 0x0, 0x0},
    {"R_CA_DLY_DCM_EN", 0x0, 0x0},
    {"R_DQ0_DLY_DCM_EN", 0x0, 0x0},
    {"R_DQ1_DLY_DCM_EN", 0x0, 0x0},
    {"R_DMSHU_PHYDCM_FORCEOFF", 0x1, 0x1},
    {"R_DMDA_RRESETB_E", 0x1, 0x1},
    {"R_WL_DOWNSP", 0x0, 0x0},
    {"R_DMDRAMCLKEN1", 0x0, 0x0},
    {"R_DMDRAMCLKEN0", 0x0, 0x0},
    {"R_DMMUXCA_SEC", 0x0, 0x0},
    {"R_DMDA_RRESETB_I", 0x1, 0x1},
    {"R_DMRRESETB_I_OPT", 0x1, 0x1},
    {"CK_BFE_DCM_EN", 0x0, 0x0},
    {"R_DMARPICA_SW_UPDX", 0x0, 0x0},
    {"R_DMPINMUX", 0x0, 0x0},
    {"R_DMMUXCA", 0x0, 0x0},
    {"R_DMMCTLPLL_CKSEL", 0x0, 0x0},
    {"R_DM_TX_ARCMD_OE", 0x1, 0x1},
    {"R_DM_TX_ARCLK_OE", 0x1, 0x1},
    {"R_DMPHYRST", 0x0, 0x0},
    {"R_OPT2_CG_CS", 0x0, 0x0},
    {"R_OPT2_CG_CLK", 0x0, 0x0},
    {"R_OPT2_CG_CMD", 0x0, 0x0},
    {"R_OPT2_CG_DQSIEN", 0x0, 0x0},
    {"R_OPT2_CG_DQ", 0x0, 0x0},
    {"R_OPT2_CG_DQS", 0x0, 0x0},
    {"R_OPT2_CG_DQM", 0x0, 0x0},
    {"R_OPT2_CG_MCK", 0x0, 0x0},
    {"R_OPT2_MPDIV_CG", 0x0, 0x0},
    {"R_SRAM_SLEEPB", 0x1, 0x1},
    {"R_SRAM_ISOINTB", 0x1, 0x1},
    {"R_SRAM_HDEN", 0x0, 0x0},
    {"R_SPM_SRAM_SLP_MSK", 0x0, 0x0},
    {"MBIST_RSTB", 0x0, 0x0},
    {"R_CS_MARK", 0x0, 0x0},
    {"R_MBIST_HOLDB", 0x1, 0x1},
    {"R_SLEEP_TEST", 0x0, 0x0},
    {"R_SLEEP_INV", 0x0, 0x0},
    {"R_SLEEP_R", 0x0, 0x0},
    {"R_SLEEP_W", 0x0, 0x0},
    {"R_MBIST_BACKGROUND", 0x0, 0x0},
    {"R_MBIST_MODE", 0x0, 0x0},
    {"R_MBIST_RPRSTB", 0x1, 0x1},
    {"R_MBIST_RPREG_SEL", 0x0, 0x0},
    {"R_MBIST_RPREG_LOAD", 0x0, 0x0},
    {"R_USE_DEFAULT_DELSEL", 0x1, 0x1},
    {"R_SRAM_DELSEL", 0x2A4, 0x2A4},
    {"R_PHYCTRLDCM", 0x1, 0x1},
    {"R_PHYCTRLMUX", 0x1, 0x1},
    {"RG_MCK4X_O_FB_CK_CG_OFF", 0x1, 0x1},
    {"RG_DDR400_MCK4X_O_FORCE_ON", 0x1, 0x1},
    {"RG_CG_DDR400_MCK4X_O_OFF", 0x1, 0x1},
    {"RG_MCK4X_O_OPENLOOP_MODE_EN", 0x1, 0x1},
    {"RG_MCK4X_Q_FB_CK_CG_OFF", 0x1, 0x1},
    {"RG_DDR400_MCK4X_Q_FORCE_ON", 0x1, 0x1},
    {"RG_CG_DDR400_MCK4X_Q_OFF", 0x1, 0x1},
    {"RG_MCK4X_Q_OPENLOOP_MODE_EN", 0x1, 0x1},
    {"RG_MCK4X_I_FB_CK_CG_OFF", 0x1, 0x1},
    {"RG_DDR400_MCK4X_I_FORCE_ON", 0x1, 0x1},
    {"RG_CG_DDR400_MCK4X_I_OFF", 0x1, 0x1},
    {"RG_MCK4X_I_OPENLOOP_MODE_EN", 0x1, 0x1},
    {"RG_M_CK_OPENLOOP_MODE_EN", 0x1, 0x1},
    {"ARMCTL_CK_OUT_CG_SEL", 0x1, 0x1},
    {"R_DMTX_ARPI_CG_DQM_NEW_B0", 0x0, 0x0},
    {"R_DMTX_ARPI_CG_DQS_NEW_B0", 0x0, 0x0},
    {"R_DMTX_ARPI_CG_DQ_NEW_B0", 0x0, 0x0},
    {"R_DMARPI_CG_FB2DLL_DCM_EN_B0", 0x0, 0x0},
    {"R_DMRANK_CHG_PIPE_CG_IG_B0", 0x1, 0x1},
    {"R_DMRANK_PIPE_CG_IG_B0", 0x1, 0x1},
    {"R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B0", 0x1, 0x1},
    {"R_DMDQSIEN_RDSEL_PIPE_CG_IG_B0", 0x1, 0x1},
    {"R_DMDQSIEN_FLAG_PIPE_CG_IG_B0", 0x1, 0x1},
    {"R_DMDQSIEN_FLAG_SYNC_CG_IG_B0", 0x1, 0x1},
    {"R_DMSTBEN_SYNC_CG_IG_B0", 0x1, 0x1},
    {"R_DMRXDLY_CG_IG_B0", 0x1, 0x1},
    {"R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B0", 0x1, 0x1},
    {"R_DMRXDVS_RDSEL_PIPE_CG_IG_B0", 0x1, 0x1},
    {"R_RMRX_TOPHY_CG_IG_B0", 0x1, 0x1},
    {"R_RMRODTEN_CG_IG_B0", 0x1, 0x1},
    {"R_DMRANK_RXDLY_PIPE_CG_IG_B0", 0x1, 0x1},
    {"RG_ARDLL_PHDET_EN_B0_SHU", 0x1, 0x1},
    {"RG_ARPISM_MCK_SEL_B0_SHU", 0x0, 0x0},
    {"R_DMTX_ARPI_CG_DQM_NEW_B1", 0x0, 0x0},
    {"R_DMTX_ARPI_CG_DQS_NEW_B1", 0x0, 0x0},
    {"R_DMTX_ARPI_CG_DQ_NEW_B1", 0x0, 0x0},
    {"R_DMARPI_CG_FB2DLL_DCM_EN_B1", 0x0, 0x0},
    {"R_DMRANK_CHG_PIPE_CG_IG_B1", 0x1, 0x1},
    {"R_DMRANK_PIPE_CG_IG_B1", 0x1, 0x1},
    {"R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B1", 0x1, 0x1},
    {"R_DMDQSIEN_RDSEL_PIPE_CG_IG_B1", 0x1, 0x1},
    {"R_DMDQSIEN_FLAG_PIPE_CG_IG_B1", 0x1, 0x1},
    {"R_DMDQSIEN_FLAG_SYNC_CG_IG_B1", 0x1, 0x1},
    {"R_DMSTBEN_SYNC_CG_IG_B1", 0x1, 0x1},
    {"R_DMRXDLY_CG_IG_B1", 0x1, 0x1},
    {"R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B1", 0x1, 0x1},
    {"R_DMRXDVS_RDSEL_PIPE_CG_IG_B1", 0x1, 0x1},
    {"R_RMRX_TOPHY_CG_IG_B1", 0x1, 0x1},
    {"R_RMRODTEN_CG_IG_B1", 0x1, 0x1},
    {"R_DMRANK_RXDLY_PIPE_CG_IG_B1", 0x1, 0x1},
    {"RG_ARDLL_PHDET_EN_B1_SHU", 0x1, 0x1},
    {"RG_ARPISM_MCK_SEL_B1_SHU", 0x0, 0x0},
    {"R_DMTX_ARPI_CG_CS_NEW", 0x0, 0x0},
    {"R_DMTX_ARPI_CG_CLK_NEW", 0x0, 0x0},
    {"R_DMTX_ARPI_CG_CMD_NEW", 0x0, 0x0},
    {"R_DMARPI_CG_FB2DLL_DCM_EN_CA", 0x0, 0x0},
    {"R_DMRANKRXDVS_CA", 0x1, 0x1},
    {"R_DMRANK_CHG_PIPE_CG_IG_CA", 0x1, 0x1},
    {"R_DMRANK_PIPE_CG_IG_CA", 0x1, 0x1},
    {"R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_CA", 0x1, 0x1},
    {"R_DMDQSIEN_RDSEL_PIPE_CG_IG_CA", 0x1, 0x1},
    {"R_DMDQSIEN_FLAG_PIPE_CG_IG_CA", 0x1, 0x1},
    {"R_DMDQSIEN_FLAG_SYNC_CG_IG_CA", 0x1, 0x1},
    {"R_RMRX_TOPHY_CG_IG_CA", 0x1, 0x1},
    {"R_RMRODTEN_CG_IG_CA", 0x1, 0x1},
    {"RG_ARDLL_PHDET_EN_CA_SHU", 0x1, 0x1},
    {"RG_ARPISM_MCK_SEL_CA_SHU", 0x0, 0x0},
    {"RG_RPHYPLL_ADA_MCK8X_EN_SHU", 0x1, 0x0},//Modify by KaiHsin, PLL related(only need set CHA)
    {"R_RX_PIPE_BYPASS_EN", 0x0, 0x0},
};
#endif


U16 FoundIdxOfArray(char *str)
{
    U16 u2Idx = 0;
    for(u2Idx = 0; u2Idx < sizeof(golden_setting_anwer)/sizeof(golden_setting_anwer[0]); u2Idx++)
    {
        if(!strcmp(golden_setting_anwer[u2Idx].field, str))
            break;
    }
    return u2Idx;
}


DRAM_STATUS_T CheckGoldenField(DRAMC_CTX_T *p, U32 u4Reg, U32 u4Field, char *str)
{
    U8 backup_channel = vGetPHY2ChannelMapping(p);
    U16 u2Index = FoundIdxOfArray(str);
    DRAM_STATUS_T eStatus = DRAM_OK;
    U8 channel_idx;
    U32 u4Value = 0;
    U32 u4Answer = 0;
    char *schannel;

    for(channel_idx = CHANNEL_A; channel_idx<p->support_channel_num; channel_idx++)
    {
        vSetPHY2ChannelMapping(p, channel_idx);
        u4Value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(u4Reg), u4Field);
        u4Answer = (channel_idx == CHANNEL_A) ? (golden_setting_anwer[u2Index].u4ChaValue) : (golden_setting_anwer[u2Index].u4ChbValue);
        schannel = (channel_idx == CHANNEL_A) ? ("CHA") : ("CHB");

        if(u4Answer == u4Value)
        {
            //mcSHOW_DBG_MSG(("[%d]****** OK\n", u2Index)); 
        }    
        else
        {
            mcSHOW_DBG_MSG(("![%s][%s][0x%x][ANS:0x%x]****** fail\n", str, schannel, u4Value, u4Answer));           
            eStatus = DRAM_FAIL;
        }
    }
    vSetPHY2ChannelMapping(p, backup_channel);
    return eStatus;    
}   


DRAM_STATUS_T CheckGoldenSetting(DRAMC_CTX_T *p)
{
    DRAM_STATUS_T eStatus = DRAM_OK;

    eStatus |= CheckGoldenField(p, DRAMC_REG_DRAMC_PD_CTRL, DRAMC_PD_CTRL_COMBCLKCTRL, "COMBCLKCTRL");
    eStatus |= CheckGoldenField(p, DRAMC_REG_DRAMC_PD_CTRL, DRAMC_PD_CTRL_PHYCLKDYNGEN, "PHYCLKDYNGEN");
    eStatus |= CheckGoldenField(p, DRAMC_REG_DRAMC_PD_CTRL, DRAMC_PD_CTRL_MIOCKCTRLOFF, "MIOCKCTRLOFF");
    eStatus |= CheckGoldenField(p, DRAMC_REG_DRAMC_PD_CTRL, DRAMC_PD_CTRL_COMBPHY_CLKENSAME, "COMBPHY_CLKENSAME");
    eStatus |= CheckGoldenField(p, DRAMC_REG_DRAMC_PD_CTRL, DRAMC_PD_CTRL_DCMENNOTRFC, "DCMENNOTRFC");
    eStatus |= CheckGoldenField(p, DRAMC_REG_DRAMC_PD_CTRL, DRAMC_PD_CTRL_DCMEN2, "DCMEN2");
    eStatus |= CheckGoldenField(p, DRAMC_REG_DRAMC_PD_CTRL, DRAMC_PD_CTRL_DCMEN, "DCMEN");
    eStatus |= CheckGoldenField(p, DRAMC_REG_CLKAR, CLKAR_RDYCKAR, "RDYCKAR");
    eStatus |= CheckGoldenField(p, DRAMC_REG_CLKAR, CLKAR_CMDCKAR, "CMDCKAR");
    eStatus |= CheckGoldenField(p, DRAMC_REG_CLKAR, CLKAR_CALCKAR, "CALCKAR");
    eStatus |= CheckGoldenField(p, DRAMC_REG_CLKAR, CLKAR_SEQCLKRUN, "SEQCLKRUN");
    eStatus |= CheckGoldenField(p, DRAMC_REG_CLKAR, CLKAR_REQQUECLKRUN, "REQQUECLKRUN");
    eStatus |= CheckGoldenField(p, DRAMC_REG_CLKAR, CLKAR_REFCLKRUN, "REFCLKRUN");
    eStatus |= CheckGoldenField(p, DRAMC_REG_CLKAR, CLKAR_DWCLKRUN, "DWCLKRUN");
    eStatus |= CheckGoldenField(p, DRAMC_REG_CLKAR, CLKAR_PHYGLUECLKRUN, "PHYGLUECLKRUN");
    eStatus |= CheckGoldenField(p, DRAMC_REG_CLKAR, CLKAR_TESTCLKRUN, "TESTCLKRUN");
    eStatus |= CheckGoldenField(p, DRAMC_REG_CLKAR, CLKAR_BCLKAR, "BCLKAR");
    eStatus |= CheckGoldenField(p, DRAMC_REG_CLKAR, CLKAR_PSELAR, "PSELAR");
    eStatus |= CheckGoldenField(p, DRAMC_REG_CLKAR, CLKAR_RDATCKAR, "RDATCKAR");
    eStatus |= CheckGoldenField(p, DRAMC_REG_CLKAR, CLKAR_SELPH_CMD_CG_DIS, "SELPH_CMD_CG_DIS");
    eStatus |= CheckGoldenField(p, DRAMC_REG_CLKAR, CLKAR_REQQUE_PACG_DIS, "REQQUE_PACG_DIS");
    eStatus |= CheckGoldenField(p, DRAMC_REG_SREFCTRL, SREFCTRL_SCSM_CGAR, "SCSM_CGAR");
    eStatus |= CheckGoldenField(p, DRAMC_REG_SREFCTRL, SREFCTRL_SCARB_SM_CGAR, "SCARB_SM_CGAR");
    eStatus |= CheckGoldenField(p, DRAMC_REG_SREFCTRL, SREFCTRL_RDDQSOSC_CGAR, "RDDQSOSC_CGAR");
    eStatus |= CheckGoldenField(p, DRAMC_REG_SREFCTRL, SREFCTRL_HMRRSEL_CGAR, "HMRRSEL_CGAR");
    eStatus |= CheckGoldenField(p, DRAMC_REG_REFCTRL1, REFCTRL1_SREF_CG_OPT, "SREF_CG_OPT");
    eStatus |= CheckGoldenField(p, DRAMC_REG_ZQCS, ZQCS_ZQMASK_CGAR, "ZQMASK_CGAR");
    eStatus |= CheckGoldenField(p, DRAMC_REG_ZQCS, ZQCS_ZQCS_MASK_SEL_CGAR, "ZQCS_MASK_SEL_CGAR");
    eStatus |= CheckGoldenField(p, DRAMC_REG_DUMMY_RD, DUMMY_RD_DUMMY_RD_PA_OPT, "DUMMY_RD_PA_OPT");
    eStatus |= CheckGoldenField(p, DRAMC_REG_SHUCTRL, SHUCTRL_DVFS_CG_OPT, "DVFS_CG_OPT");
    eStatus |= CheckGoldenField(p, DRAMC_REG_PRE_TDQSCK1, PRE_TDQSCK1_TXUIPI_CAL_CGAR, "TXUIPI_CAL_CGAR");
    eStatus |= CheckGoldenField(p, DRAMC_REG_SHU_APHY_TX_PICG_CTRL, SHU_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_OPT, "DDRPHY_CLK_EN_COMB_TX_OPT");
    eStatus |= CheckGoldenField(p, DDRPHY_B0_DQ9, B0_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B0, "RG_RX_ARDQS0_STBEN_RESETB_B0");
    eStatus |= CheckGoldenField(p, DDRPHY_B0_TX_MCK, B0_TX_MCK_R_DM_TX_MCK_FRUN_B0, "R_DM_TX_MCK_FRUN_B0");
    eStatus |= CheckGoldenField(p, DDRPHY_B1_DQ9, B1_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B1, "RG_RX_ARDQS0_STBEN_RESETB_B1");
    eStatus |= CheckGoldenField(p, DDRPHY_B1_TX_MCK, B1_TX_MCK_R_DM_TX_MCK_FRUN_B1, "R_DM_TX_MCK_FRUN_B1");
    eStatus |= CheckGoldenField(p, DDRPHY_CA_DLL_ARPI1, CA_DLL_ARPI1_RG_ARPISM_MCK_SEL_CA_REG_OPT, "RG_ARPISM_MCK_SEL_CA_REG_OPT");
    eStatus |= CheckGoldenField(p, DDRPHY_CA_TX_MCK, CA_TX_MCK_R_DM_TX_MCK_FRUN_CA, "R_DM_TX_MCK_FRUN_CA");
    eStatus |= CheckGoldenField(p, DDRPHY_DVFS_EMI_CLK, DVFS_EMI_CLK_RG_GATING_EMI_NEW, "RG_GATING_EMI_NEW");
    eStatus |= CheckGoldenField(p, DDRPHY_DVFS_EMI_CLK, DVFS_EMI_CLK_RG_52M_104M_SEL, "RG_52M_104M_SEL");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CG_CTRL0, MISC_CG_CTRL0_RG_CG_INFRA_OFF_DISABLE, "RG_CG_INFRA_OFF_DISABLE");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CG_CTRL0, MISC_CG_CTRL0_RG_CG_RX_COMB1_OFF_DISABLE, "RG_CG_RX_COMB1_OFF_DISABLE");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CG_CTRL0, MISC_CG_CTRL0_RG_CG_RX_COMB0_OFF_DISABLE, "RG_CG_RX_COMB0_OFF_DISABLE");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CG_CTRL0, MISC_CG_CTRL0_RG_CG_RX_CMD_OFF_DISABLE, "RG_CG_RX_CMD_OFF_DISABLE");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CG_CTRL0, MISC_CG_CTRL0_RG_CG_COMB1_OFF_DISABLE, "RG_CG_COMB1_OFF_DISABLE");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CG_CTRL0, MISC_CG_CTRL0_RG_CG_COMB0_OFF_DISABLE, "RG_CG_COMB0_OFF_DISABLE");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CG_CTRL0, MISC_CG_CTRL0_RG_CG_CMD_OFF_DISABLE, "RG_CG_CMD_OFF_DISABLE");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CG_CTRL0, MISC_CG_CTRL0_RG_CG_COMB_OFF_DISABLE, "RG_CG_COMB_OFF_DISABLE");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CG_CTRL0, MISC_CG_CTRL0_RG_CG_PHY_OFF_DIABLE, "RG_CG_PHY_OFF_DIABLE");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CG_CTRL0, MISC_CG_CTRL0_RG_CG_DRAMC_OFF_DISABLE, "RG_CG_DRAMC_OFF_DISABLE");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CG_CTRL0, MISC_CG_CTRL0_RG_CG_EMI_OFF_DISABLE, "RG_CG_EMI_OFF_DISABLE");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CG_CTRL2, MISC_CG_CTRL2_RG_MEM_DCM_FORCE_OFF, "RG_MEM_DCM_FORCE_OFF");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CG_CTRL2, MISC_CG_CTRL2_RG_MEM_DCM_FORCE_ON, "RG_MEM_DCM_FORCE_ON");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CG_CTRL4, MISC_CG_CTRL4_R_PHY_MCK_CG_CTRL, "R_PHY_MCK_CG_CTRL");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CG_CTRL5, MISC_CG_CTRL5_R_CA_PI_DCM_EN, "R_CA_PI_DCM_EN");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CG_CTRL5, MISC_CG_CTRL5_R_DQ0_PI_DCM_EN, "R_DQ0_PI_DCM_EN");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CG_CTRL5, MISC_CG_CTRL5_R_DQ1_PI_DCM_EN, "R_DQ1_PI_DCM_EN");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CG_CTRL5, MISC_CG_CTRL5_R_CA_DLY_DCM_EN, "R_CA_DLY_DCM_EN");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CG_CTRL5, MISC_CG_CTRL5_R_DQ0_DLY_DCM_EN, "R_DQ0_DLY_DCM_EN");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CG_CTRL5, MISC_CG_CTRL5_R_DQ1_DLY_DCM_EN, "R_DQ1_DLY_DCM_EN");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL0, MISC_CTRL0_R_DMSHU_PHYDCM_FORCEOFF, "R_DMSHU_PHYDCM_FORCEOFF");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL1, MISC_CTRL1_R_DMDA_RRESETB_E, "R_DMDA_RRESETB_E");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL1, MISC_CTRL1_R_WL_DOWNSP, "R_WL_DOWNSP");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL1, MISC_CTRL1_R_DMDRAMCLKEN1, "R_DMDRAMCLKEN1");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL1, MISC_CTRL1_R_DMDRAMCLKEN0, "R_DMDRAMCLKEN0");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL1, MISC_CTRL1_R_DMMUXCA_SEC, "R_DMMUXCA_SEC");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL1, MISC_CTRL1_R_DMDA_RRESETB_I, "R_DMDA_RRESETB_I");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL1, MISC_CTRL1_R_DMRRESETB_I_OPT, "R_DMRRESETB_I_OPT");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL1, MISC_CTRL1_CK_BFE_DCM_EN, "CK_BFE_DCM_EN");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL1, MISC_CTRL1_R_DMARPICA_SW_UPDX, "R_DMARPICA_SW_UPDX");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL1, MISC_CTRL1_R_DMPINMUX, "R_DMPINMUX");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL1, MISC_CTRL1_R_DMMUXCA, "R_DMMUXCA");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL1, MISC_CTRL1_R_DMMCTLPLL_CKSEL, "R_DMMCTLPLL_CKSEL");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL1, MISC_CTRL1_R_DM_TX_ARCMD_OE, "R_DM_TX_ARCMD_OE");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL1, MISC_CTRL1_R_DM_TX_ARCLK_OE, "R_DM_TX_ARCLK_OE");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL1, MISC_CTRL1_R_DMPHYRST, "R_DMPHYRST");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL4, MISC_CTRL4_R_OPT2_CG_CS, "R_OPT2_CG_CS");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL4, MISC_CTRL4_R_OPT2_CG_CLK, "R_OPT2_CG_CLK");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL4, MISC_CTRL4_R_OPT2_CG_CMD, "R_OPT2_CG_CMD");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL4, MISC_CTRL4_R_OPT2_CG_DQSIEN, "R_OPT2_CG_DQSIEN");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL4, MISC_CTRL4_R_OPT2_CG_DQ, "R_OPT2_CG_DQ");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL4, MISC_CTRL4_R_OPT2_CG_DQS, "R_OPT2_CG_DQS");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL4, MISC_CTRL4_R_OPT2_CG_DQM, "R_OPT2_CG_DQM");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL4, MISC_CTRL4_R_OPT2_CG_MCK, "R_OPT2_CG_MCK");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL4, MISC_CTRL4_R_OPT2_MPDIV_CG, "R_OPT2_MPDIV_CG");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL5, MISC_CTRL5_R_SRAM_SLEEPB, "R_SRAM_SLEEPB");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL5, MISC_CTRL5_R_SRAM_ISOINTB, "R_SRAM_ISOINTB");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL5, MISC_CTRL5_R_SRAM_HDEN, "R_SRAM_HDEN");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL5, MISC_CTRL5_R_SPM_SRAM_SLP_MSK, "R_SPM_SRAM_SLP_MSK");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL5, MISC_CTRL5_MBIST_RSTB, "MBIST_RSTB");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL5, MISC_CTRL5_R_CS_MARK, "R_CS_MARK");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL5, MISC_CTRL5_R_MBIST_HOLDB, "R_MBIST_HOLDB");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL5, MISC_CTRL5_R_SLEEP_TEST, "R_SLEEP_TEST");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL5, MISC_CTRL5_R_SLEEP_INV, "R_SLEEP_INV");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL5, MISC_CTRL5_R_SLEEP_R, "R_SLEEP_R");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL5, MISC_CTRL5_R_SLEEP_W, "R_SLEEP_W");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL5, MISC_CTRL5_R_MBIST_BACKGROUND, "R_MBIST_BACKGROUND");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL5, MISC_CTRL5_R_MBIST_MODE, "R_MBIST_MODE");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL5, MISC_CTRL5_R_MBIST_RPRSTB, "R_MBIST_RPRSTB");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL5, MISC_CTRL5_R_MBIST_RPREG_SEL, "R_MBIST_RPREG_SEL");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL5, MISC_CTRL5_R_MBIST_RPREG_LOAD, "R_MBIST_RPREG_LOAD");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL5, MISC_CTRL5_R_USE_DEFAULT_DELSEL, "R_USE_DEFAULT_DELSEL");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CTRL5, MISC_CTRL5_R_SRAM_DELSEL, "R_SRAM_DELSEL");
    eStatus |= CheckGoldenField(p, DDRPHY_CKMUX_SEL, CKMUX_SEL_R_PHYCTRLDCM, "R_PHYCTRLDCM");
    eStatus |= CheckGoldenField(p, DDRPHY_CKMUX_SEL, CKMUX_SEL_R_PHYCTRLMUX, "R_PHYCTRLMUX");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CG_CTRL6, MISC_CG_CTRL6_RG_MCK4X_O_FB_CK_CG_OFF, "RG_MCK4X_O_FB_CK_CG_OFF");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CG_CTRL6, MISC_CG_CTRL6_RG_DDR400_MCK4X_O_FORCE_ON, "RG_DDR400_MCK4X_O_FORCE_ON");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CG_CTRL6, MISC_CG_CTRL6_RG_CG_DDR400_MCK4X_O_OFF, "RG_CG_DDR400_MCK4X_O_OFF");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CG_CTRL6, MISC_CG_CTRL6_RG_MCK4X_O_OPENLOOP_MODE_EN, "RG_MCK4X_O_OPENLOOP_MODE_EN");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CG_CTRL6, MISC_CG_CTRL6_RG_MCK4X_Q_FB_CK_CG_OFF, "RG_MCK4X_Q_FB_CK_CG_OFF");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CG_CTRL6, MISC_CG_CTRL6_RG_DDR400_MCK4X_Q_FORCE_ON, "RG_DDR400_MCK4X_Q_FORCE_ON");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CG_CTRL6, MISC_CG_CTRL6_RG_CG_DDR400_MCK4X_Q_OFF, "RG_CG_DDR400_MCK4X_Q_OFF");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CG_CTRL6, MISC_CG_CTRL6_RG_MCK4X_Q_OPENLOOP_MODE_EN, "RG_MCK4X_Q_OPENLOOP_MODE_EN");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CG_CTRL6, MISC_CG_CTRL6_RG_MCK4X_I_FB_CK_CG_OFF, "RG_MCK4X_I_FB_CK_CG_OFF");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CG_CTRL6, MISC_CG_CTRL6_RG_DDR400_MCK4X_I_FORCE_ON, "RG_DDR400_MCK4X_I_FORCE_ON");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CG_CTRL6, MISC_CG_CTRL6_RG_CG_DDR400_MCK4X_I_OFF, "RG_CG_DDR400_MCK4X_I_OFF");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CG_CTRL6, MISC_CG_CTRL6_RG_MCK4X_I_OPENLOOP_MODE_EN, "RG_MCK4X_I_OPENLOOP_MODE_EN");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CG_CTRL6, MISC_CG_CTRL6_RG_M_CK_OPENLOOP_MODE_EN, "RG_M_CK_OPENLOOP_MODE_EN");
    eStatus |= CheckGoldenField(p, DDRPHY_MISC_CG_CTRL6, MISC_CG_CTRL6_ARMCTL_CK_OUT_CG_SEL, "ARMCTL_CK_OUT_CG_SEL");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B0_DQ7, SHU_B0_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B0, "R_DMTX_ARPI_CG_DQM_NEW_B0");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B0_DQ7, SHU_B0_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B0, "R_DMTX_ARPI_CG_DQS_NEW_B0");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B0_DQ7, SHU_B0_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B0, "R_DMTX_ARPI_CG_DQ_NEW_B0");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B0_DQ7, SHU_B0_DQ7_R_DMARPI_CG_FB2DLL_DCM_EN_B0, "R_DMARPI_CG_FB2DLL_DCM_EN_B0");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B0_DQ8, SHU_B0_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B0, "R_DMRANK_CHG_PIPE_CG_IG_B0");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B0_DQ8, SHU_B0_DQ8_R_DMRANK_PIPE_CG_IG_B0, "R_DMRANK_PIPE_CG_IG_B0");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B0_DQ8, SHU_B0_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B0, "R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B0");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B0_DQ8, SHU_B0_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B0, "R_DMDQSIEN_RDSEL_PIPE_CG_IG_B0");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B0_DQ8, SHU_B0_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B0, "R_DMDQSIEN_FLAG_PIPE_CG_IG_B0");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B0_DQ8, SHU_B0_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B0, "R_DMDQSIEN_FLAG_SYNC_CG_IG_B0");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B0_DQ8, SHU_B0_DQ8_R_DMSTBEN_SYNC_CG_IG_B0, "R_DMSTBEN_SYNC_CG_IG_B0");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B0_DQ8, SHU_B0_DQ8_R_DMRXDLY_CG_IG_B0, "R_DMRXDLY_CG_IG_B0");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B0_DQ8, SHU_B0_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B0, "R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B0");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B0_DQ8, SHU_B0_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B0, "R_DMRXDVS_RDSEL_PIPE_CG_IG_B0");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B0_DQ8, SHU_B0_DQ8_R_RMRX_TOPHY_CG_IG_B0, "R_RMRX_TOPHY_CG_IG_B0");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B0_DQ8, SHU_B0_DQ8_R_RMRODTEN_CG_IG_B0, "R_RMRODTEN_CG_IG_B0");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B0_DQ8, SHU_B0_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B0, "R_DMRANK_RXDLY_PIPE_CG_IG_B0");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B0_DLL0, SHU_B0_DLL0_RG_ARDLL_PHDET_EN_B0_SHU, "RG_ARDLL_PHDET_EN_B0_SHU");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B0_DLL0, SHU_B0_DLL0_RG_ARPISM_MCK_SEL_B0_SHU, "RG_ARPISM_MCK_SEL_B0_SHU");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B1_DQ7, SHU_B1_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B1, "R_DMTX_ARPI_CG_DQM_NEW_B1");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B1_DQ7, SHU_B1_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B1, "R_DMTX_ARPI_CG_DQS_NEW_B1");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B1_DQ7, SHU_B1_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B1, "R_DMTX_ARPI_CG_DQ_NEW_B1");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B1_DQ7, SHU_B1_DQ7_R_DMARPI_CG_FB2DLL_DCM_EN_B1, "R_DMARPI_CG_FB2DLL_DCM_EN_B1");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B1_DQ8, SHU_B1_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B1, "R_DMRANK_CHG_PIPE_CG_IG_B1");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B1_DQ8, SHU_B1_DQ8_R_DMRANK_PIPE_CG_IG_B1, "R_DMRANK_PIPE_CG_IG_B1");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B1_DQ8, SHU_B1_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B1, "R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B1");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B1_DQ8, SHU_B1_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B1, "R_DMDQSIEN_RDSEL_PIPE_CG_IG_B1");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B1_DQ8, SHU_B1_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B1, "R_DMDQSIEN_FLAG_PIPE_CG_IG_B1");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B1_DQ8, SHU_B1_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B1, "R_DMDQSIEN_FLAG_SYNC_CG_IG_B1");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B1_DQ8, SHU_B1_DQ8_R_DMSTBEN_SYNC_CG_IG_B1, "R_DMSTBEN_SYNC_CG_IG_B1");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B1_DQ8, SHU_B1_DQ8_R_DMRXDLY_CG_IG_B1, "R_DMRXDLY_CG_IG_B1");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B1_DQ8, SHU_B1_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B1, "R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B1");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B1_DQ8, SHU_B1_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B1, "R_DMRXDVS_RDSEL_PIPE_CG_IG_B1");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B1_DQ8, SHU_B1_DQ8_R_RMRX_TOPHY_CG_IG_B1, "R_RMRX_TOPHY_CG_IG_B1");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B1_DQ8, SHU_B1_DQ8_R_RMRODTEN_CG_IG_B1, "R_RMRODTEN_CG_IG_B1");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B1_DQ8, SHU_B1_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B1, "R_DMRANK_RXDLY_PIPE_CG_IG_B1");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B1_DLL0, SHU_B1_DLL0_RG_ARDLL_PHDET_EN_B1_SHU, "RG_ARDLL_PHDET_EN_B1_SHU");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_B1_DLL0, SHU_B1_DLL0_RG_ARPISM_MCK_SEL_B1_SHU, "RG_ARPISM_MCK_SEL_B1_SHU");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_CA_CMD7, SHU_CA_CMD7_R_DMTX_ARPI_CG_CS_NEW, "R_DMTX_ARPI_CG_CS_NEW");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_CA_CMD7, SHU_CA_CMD7_R_DMTX_ARPI_CG_CLK_NEW, "R_DMTX_ARPI_CG_CLK_NEW");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_CA_CMD7, SHU_CA_CMD7_R_DMTX_ARPI_CG_CMD_NEW, "R_DMTX_ARPI_CG_CMD_NEW");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_CA_CMD7, SHU_CA_CMD7_R_DMARPI_CG_FB2DLL_DCM_EN_CA, "R_DMARPI_CG_FB2DLL_DCM_EN_CA");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_CA_CMD7, SHU_CA_CMD7_R_DMRANKRXDVS_CA, "R_DMRANKRXDVS_CA");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_CA_CMD8, SHU_CA_CMD8_R_DMRANK_CHG_PIPE_CG_IG_CA, "R_DMRANK_CHG_PIPE_CG_IG_CA");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_CA_CMD8, SHU_CA_CMD8_R_DMRANK_PIPE_CG_IG_CA, "R_DMRANK_PIPE_CG_IG_CA");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_CA_CMD8, SHU_CA_CMD8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_CA, "R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_CA");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_CA_CMD8, SHU_CA_CMD8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_CA, "R_DMDQSIEN_RDSEL_PIPE_CG_IG_CA");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_CA_CMD8, SHU_CA_CMD8_R_DMDQSIEN_FLAG_PIPE_CG_IG_CA, "R_DMDQSIEN_FLAG_PIPE_CG_IG_CA");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_CA_CMD8, SHU_CA_CMD8_R_DMDQSIEN_FLAG_SYNC_CG_IG_CA, "R_DMDQSIEN_FLAG_SYNC_CG_IG_CA");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_CA_CMD8, SHU_CA_CMD8_R_RMRX_TOPHY_CG_IG_CA, "R_RMRX_TOPHY_CG_IG_CA");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_CA_CMD8, SHU_CA_CMD8_R_RMRODTEN_CG_IG_CA, "R_RMRODTEN_CG_IG_CA");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_CA_DLL0, SHU_CA_DLL0_RG_ARDLL_PHDET_EN_CA_SHU, "RG_ARDLL_PHDET_EN_CA_SHU");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_CA_DLL0, SHU_CA_DLL0_RG_ARPISM_MCK_SEL_CA_SHU, "RG_ARPISM_MCK_SEL_CA_SHU");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_PLL22, SHU_PLL22_RG_RPHYPLL_ADA_MCK8X_EN_SHU, "RG_RPHYPLL_ADA_MCK8X_EN_SHU");
    eStatus |= CheckGoldenField(p, DDRPHY_SHU_MISC0, SHU_MISC0_R_RX_PIPE_BYPASS_EN, "R_RX_PIPE_BYPASS_EN");
    return eStatus;
}
#endif

#if defined(TEST_MODE_MRS) || SAMSUNG_TEST_MODE_MRS_FOR_PRELOADER
const U8 PME_Key[4][6] =
{
    { 2, 5, 5, 2, 4, 8},
    { 2, 5, 5, 2, 4, 8},
    { 2, 5, 5, 2, 4, 8},
    { 10, 13, 13, 10, 12, 13}
};


static U32 u4TestModeV0[2] = {};
static U32 u4TestModeV1[2] = {};
static U32 u4TestModeV2[2] = {};
static U32 u4TestModeV3[2] = {};
static U32 u4TestModeV4[2] = {};
static U32 u4TestModeV5[2] = {};
static U32 u4TestModeV6[2] = {};

void BackupRGBeforeTestMode(DRAMC_CTX_T *p)
{
    DRAM_CHANNEL_T eOriChannel = p->channel;
    int i = 0;
     p->channel = CHANNEL_A;
    for(i=0; i<2; i++)
    {
        if(i == 0)
        {
            mcSHOW_DBG_MSG(("***CHA\n"));
        }    
        else
        {
            mcSHOW_DBG_MSG(("***CHB\n"));
        }

        u4TestModeV0[i] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_MRS));
        mcSHOW_DBG_MSG4(("DRAMC_REG_MRS[0x%x]\n", u4TestModeV0[i]));
        u4TestModeV1[i] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL));
        mcSHOW_DBG_MSG4(("DRAMC_REG_DRAMC_PD_CTRL[0x%x]\n", u4TestModeV1[i]));
        u4TestModeV2[i] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_STBCAL));//STBCAL_DQSIENCG_NORMAL_EN
        mcSHOW_DBG_MSG4(("DRAMC_REG_STBCAL[0x%x]\n", u4TestModeV2[i]));
        u4TestModeV3[i] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0));//Auto refresh
        mcSHOW_DBG_MSG4(("DRAMC_REG_REFCTRL0[0x%x]\n", u4TestModeV3[i]));
        u4TestModeV4[i] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL));
        mcSHOW_DBG_MSG4(("DRAMC_REG_SPCMDCTRL[0x%x]\n", u4TestModeV4[i]));
        u4TestModeV5[i] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL));
        mcSHOW_DBG_MSG4(("DRAMC_REG_CKECTRL[0x%x]\n", u4TestModeV5[i]));
        u4TestModeV6[i] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE));
        mcSHOW_DBG_MSG4(("DRAMC_REG_SLP4_TESTMODE[0x%x]\n", u4TestModeV6[i]));
        p->channel = CHANNEL_B;
    }
    p->channel = eOriChannel;
    return;

}

void RestoreRGBeforeTestMode(DRAMC_CTX_T *p)
{
    DRAM_CHANNEL_T eOriChannel = p->channel;
    int i = 0;
    p->channel = CHANNEL_A;
    for(i=0; i<2; i++)
    {
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_MRS), u4TestModeV0[i]);
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), u4TestModeV1[i]);
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), u4TestModeV2[i]);
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0), u4TestModeV3[i]);
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), u4TestModeV4[i]);
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), u4TestModeV5[i]);
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), u4TestModeV6[i]);

        p->channel = CHANNEL_B;
    }
    p->channel = eOriChannel;
    return;
}

void ProgramModeEnter(DRAMC_CTX_T *p)
{
    DRAM_CHANNEL_T eOriChannel = p->channel;
    int i = 0;
    int j = 0;
    int k = 0;
    U8 u1Rank = 0;

    BackupRGBeforeTestMode(p);
    p->channel = CHANNEL_A;
    mcSHOW_DBG_MSG(("ProgramModeEnter [Begin]\n"));

    for(i=0; i<4; i++)
    {
        for(j=0; j<6; j++)
        {
             mcSHOW_DBG_MSG(("Key[%d] CA[%d] => [%d]\n", i, j, PME_Key[i][j])); 
        }
        mcSHOW_DBG_MSG(("\n")); 
    }

    for(i=0; i<2; i++)
    {
        if(i == 0)
        {
            mcSHOW_DBG_MSG(("***CHA\n"));
        }    
        else
        {
            mcSHOW_DBG_MSG(("***CHB\n"));
        }
        for(j=0; j<2; j++)
        {
            if(j == 0)
            {
                u1Rank = 0;
                mcSHOW_DBG_MSG(("***R0\n"));
            }    
            else
            {
                u1Rank = 1;  
                mcSHOW_DBG_MSG(("***R1\n"));
            }
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1Rank, MRS_MRSRK);

            //Disable dramc and DDRPHY clock gated, let clock freerun
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 0, DRAMC_PD_CTRL_DCMEN);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 0, DRAMC_PD_CTRL_PHYCLKDYNGEN);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), 0, STBCAL_DQSIENCG_NORMAL_EN);

            //Disable MR4, refresh
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0), 1, REFCTRL0_REFDIS); 
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), 1, SPCMDCTRL_REFRDIS);  
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), 1, CKECTRL_CKEFIXON);  

            for(k=0; k<4; k++)
            {
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), PME_Key[k][0], SLP4_TESTMODE_CA0_TEST); 
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), PME_Key[k][1], SLP4_TESTMODE_CA1_TEST); 
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), PME_Key[k][2], SLP4_TESTMODE_CA2_TEST); 
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), PME_Key[k][3], SLP4_TESTMODE_CA3_TEST); 
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), PME_Key[k][4], SLP4_TESTMODE_CA4_TEST); 
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), PME_Key[k][5], SLP4_TESTMODE_CA5_TEST); 

                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), 1, SLP4_TESTMODE_STESTEN);  
                while(u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_STEST_RESPONSE))
                {
                    mcSHOW_DBG_MSG(("status[%d]: %d\n", k, u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_STEST_RESPONSE)));
                    break;
                }
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), 0, SLP4_TESTMODE_STESTEN); 
            }
        }
        p->channel = CHANNEL_B;
    }
    mcSHOW_DBG_MSG(("ProgramModeEnter [Stop]\n"));
    p->channel = eOriChannel;
    RestoreRGBeforeTestMode(p);
    return;
}

void ProgramCodeInput(DRAMC_CTX_T *p, U16 u2A_value, U16 u2B_value, U16 u2C_value)
{
    DRAM_CHANNEL_T eOriChannel = p->channel;
    int i = 0;
    int j = 0;
    int k = 0;
    int keyNumber = 0;
    U16 u2Value = 0;
    U8 u1Rank = 0;

    BackupRGBeforeTestMode(p);
    p->channel = CHANNEL_A;
    //CA0, CA1, CA2, CA3, CA4, CA5
    U8 PCI_Key[5][6] = 
    {
        { 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0},
        { 2, 5, 1, 2, 0, 0},
        { 2, 5, 1, 2, 0, 0}
    };        

    mcSHOW_DBG_MSG(("ProgramCodeInput [Begin]\n"));

    for(keyNumber=0; keyNumber<3; keyNumber++)
    {
        if(keyNumber == 0)
            u2Value = u2A_value;
        else if(keyNumber == 1)
            u2Value = u2B_value;
        else
            u2Value = u2C_value;

        PCI_Key[keyNumber][0] = (u2Value == 0) ? (10) : (2);
        PCI_Key[keyNumber][1] = (u2Value == 1) ? (13) : (5);
        PCI_Key[keyNumber][2] = (u2Value == 10) ? (5) : ((u2Value == 2) ? (9):(1));
        PCI_Key[keyNumber][3] = (u2Value == 9) ? (6) : ((u2Value == 3) ? (10):(2));
        PCI_Key[keyNumber][4] = (u2Value == 8) ? (4) : ((u2Value == 4) ? (8):(0));
        PCI_Key[keyNumber][5] = (u2Value == 7) ? (1) : ((u2Value == 6) ? (4):((u2Value == 5) ? (8):(0)));
    }

    for(i=0; i<5; i++)
    {
        for(j=0; j<6; j++)
        {
             mcSHOW_DBG_MSG(("Key[%d] CA[%d] => [%d]\n", i, j, PCI_Key[i][j])); 
        }
        mcSHOW_DBG_MSG(("\n")); 
    }

    for(i=0; i<2; i++)
    {
        if(i == 0)
        {
            mcSHOW_DBG_MSG(("***CHA\n"));
        }    
        else
        {
            mcSHOW_DBG_MSG(("***CHB\n"));
        }
        for(j=0; j<2; j++)
        {
            if(j == 0)
            {
                u1Rank = 0;
                mcSHOW_DBG_MSG(("***R0\n"));
            }    
            else
            {
                u1Rank = 1;  
                mcSHOW_DBG_MSG(("***R1\n"));
            }
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1Rank, MRS_MRSRK);

            //Disable dramc and DDRPHY clock gated, let clock freerun
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 0, DRAMC_PD_CTRL_DCMEN);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 0, DRAMC_PD_CTRL_PHYCLKDYNGEN);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), 0, STBCAL_DQSIENCG_NORMAL_EN);

            //Disable MR4, refresh
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0), 1, REFCTRL0_REFDIS); 
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), 1, SPCMDCTRL_REFRDIS);  
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), 1, CKECTRL_CKEFIXON);  

            for(k=0; k<5; k++)
            {
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), PCI_Key[k][0], SLP4_TESTMODE_CA0_TEST); 
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), PCI_Key[k][1], SLP4_TESTMODE_CA1_TEST); 
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), PCI_Key[k][2], SLP4_TESTMODE_CA2_TEST); 
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), PCI_Key[k][3], SLP4_TESTMODE_CA3_TEST); 
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), PCI_Key[k][4], SLP4_TESTMODE_CA4_TEST); 
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), PCI_Key[k][5], SLP4_TESTMODE_CA5_TEST); 

                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), 1, SLP4_TESTMODE_STESTEN);  
                while(u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_STEST_RESPONSE))
                {
                    mcSHOW_DBG_MSG(("status[%d]: %d\n", k, u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_STEST_RESPONSE)));
                    break;
                }
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), 0, SLP4_TESTMODE_STESTEN); 
            }
        }
        p->channel = CHANNEL_B;
    }
    mcSHOW_DBG_MSG(("ProgramCodeInput [Stop]\n"));
    p->channel = eOriChannel;
    RestoreRGBeforeTestMode(p);
    return;
}

void vApplyProgramSequence(DRAMC_CTX_T *p)
{
    #if SAMSUNG_TEST_MODE_MRS_FOR_PRELOADER==1
    //Buffer sensitivity decrease1
    //test 2. TMRS enter -> 000 -> 390 -> 120 -> 8A7
    ProgramModeEnter(p);
    ProgramCodeInput(p, 0, 0, 0);
    ProgramCodeInput(p, 3, 9, 0);
    ProgramCodeInput(p, 1, 2, 0);
    ProgramCodeInput(p, 8, 0xa, 7);
    ProgramCodeInput(p, 2, 5, 8);
    mcSHOW_DBG_MSG(("Buffer sensitivity decrease1: TMRS enter -> 000 -> 390 -> 120 -> 8A7 -> 258\n"));
    
    #elif SAMSUNG_TEST_MODE_MRS_FOR_PRELOADER==2    
    //Buffer sensitivity decrease2
    //test 3. TMRS enter -> 000 -> 390 -> 120 -> 803
    ProgramModeEnter(p);
    ProgramCodeInput(p, 0, 0, 0);
    ProgramCodeInput(p, 3, 9, 0);
    ProgramCodeInput(p, 1, 2, 0);
    ProgramCodeInput(p, 8, 0, 3);
    ProgramCodeInput(p, 2, 5, 8);
    mcSHOW_DBG_MSG(("Buffer sensitivity decrease1: TMRS enter -> 000 -> 390 -> 120 -> 803 -> 258\n"));
    
    #elif SAMSUNG_TEST_MODE_MRS_FOR_PRELOADER==3
    //2014 + 2863
    //test 4.TMRS enter -> 000 -> 390 -> 120 -> 014 -> 863
    ProgramModeEnter(p);
    ProgramCodeInput(p, 0, 0, 0);
    ProgramCodeInput(p, 3, 9, 0);
    ProgramCodeInput(p, 1, 2, 0);
    ProgramCodeInput(p, 0, 1, 4);
    ProgramCodeInput(p, 8, 6, 3);    
    ProgramCodeInput(p, 2, 5, 8);
    mcSHOW_DBG_MSG(("2014 + 2863: test 4.TMRS enter -> 000 -> 390 -> 120 -> 014 -> 863 -> 258\n"));
    
    #elif SAMSUNG_TEST_MODE_MRS_FOR_PRELOADER==4 
    //252A
    //test 5. TMRS enter -> 000 -> 390 -> 120 -> 52A
    ProgramModeEnter(p);
    ProgramCodeInput(p, 0, 0, 0);
    ProgramCodeInput(p, 3, 9, 0);
    ProgramCodeInput(p, 1, 2, 0);
    ProgramCodeInput(p, 5, 2, 0xa);
    ProgramCodeInput(p, 2, 5, 8);
    mcSHOW_DBG_MSG(("252A: test 5. TMRS enter -> 000 -> 390 -> 120 -> 52A -> 258\n"));
    
    #else
    //test 1. TMRS enter -> 000 -> 390 -> 021
    ProgramModeEnter(p);
    ProgramCodeInput(p, 0, 0, 0);
    ProgramCodeInput(p, 3, 9, 0);
    ProgramCodeInput(p, 0, 2, 1);
    ProgramCodeInput(p, 2, 5, 8);
    mcSHOW_DBG_MSG(("Buffer sensitivity decrease1: TMRS enter -> 000 -> 390 -> 021\n"));
    #endif
}
#endif


void Hynix_Test_Mode(DRAMC_CTX_T *p)
{
#if MRW_CHECK_ONLY
    mcSHOW_MRW_MSG(("\n==[MR Dump] %s==\n", __func__));
#endif

    DramcModeRegWrite(p, 9, 0xb8);
    DramcModeRegWrite(p, 9, 0xe8);
    DramcModeRegWrite(p, 9, 0x98);
    DramcModeRegWrite(p, 9, 0xbf);
    DramcModeRegWrite(p, 9, 0xef);
    DramcModeRegWrite(p, 9, 0x9f);
    DramcModeRegWrite(p, 9, 0xb9);
    DramcModeRegWrite(p, 9, 0xe9);
    DramcModeRegWrite(p, 9, 0x99);
    DramcModeRegWrite(p, 9, 0xd8);
    DramcModeRegWrite(p, 9, 0x88);
    DramcModeRegWrite(p, 9, 0xa3);
    DramcModeRegWrite(p, 9, 0xe0);    
}


#if RUNTIME_SHMOO_RELEATED_FUNCTION && SUPPORT_SAVE_TIME_FOR_CALIBRATION
void DramcRunTimeShmooRG_BackupRestore(DRAMC_CTX_T *p)
{
    U8 channel_idx, channel_backup;
    U32 ii, u4RgBackupNum, *u4RG_Backup=0;
    
    channel_backup = vGetPHY2ChannelMapping(p);

    for(channel_idx=CHANNEL_A; channel_idx<p->support_channel_num; channel_idx++)
    {
        vSetPHY2ChannelMapping(p, channel_idx);

        U32 u4RegBackupAddress[] =
        {
            // Rx shmoo backup RG
            (DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ5)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ2)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ3)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ4)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ5)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ6)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ2)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ3)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ4)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ5)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ6)),
            
            (DRAMC_REG_ADDR(DDRPHY_SHU_B1_DQ5)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R1_B0_DQ2)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R1_B0_DQ3)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R1_B0_DQ4)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R1_B0_DQ5)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R1_B0_DQ6)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R1_B1_DQ2)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R1_B1_DQ3)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R1_B1_DQ4)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R1_B1_DQ5)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R1_B1_DQ6)),

            // Tx shmoo backup RG
            (DRAMC_REG_ADDR(DRAMC_REG_SHURK0_PI)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHURK1_PI)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHURK1_SELPH_DQ0)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ1)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHURK1_SELPH_DQ1)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ2)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHURK1_SELPH_DQ2)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ3)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHURK1_SELPH_DQ3)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQS2DQ_CAL1)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHURK1_DQS2DQ_CAL1)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQS2DQ_CAL2)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHURK1_DQS2DQ_CAL2)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQS2DQ_CAL5)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHURK1_DQS2DQ_CAL5)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ0)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R1_B0_DQ0)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ0)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R1_B1_DQ0)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ1)),            
            (DRAMC_REG_ADDR(DDRPHY_SHU_R1_B0_DQ1)),            
            (DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ1)),            
            (DRAMC_REG_ADDR(DDRPHY_SHU_R1_B1_DQ1)),            
            (DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ2)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R1_B0_DQ2)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ2)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R1_B1_DQ2)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ3)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R1_B0_DQ3)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ3)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R1_B1_DQ3)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ4)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R1_B0_DQ4)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ4)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R1_B1_DQ4)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ5)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R1_B0_DQ5)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ5)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R1_B1_DQ5)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ6)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R1_B0_DQ6)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ6)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R1_B1_DQ6)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ7)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R1_B0_DQ7)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ7)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R1_B1_DQ7)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R0_CA_CMD9)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_R1_CA_CMD9)),
        };
        u4RgBackupNum = sizeof(u4RegBackupAddress)>>2;
        u4RG_Backup = (U32*)(p->pSavetimeData->u4RG_Backup[channel_idx]);

        for (ii=0; ii<u4RgBackupNum; ii++)
        {
            if (ii >= RUNTIME_SHMOO_RG_BACKUP_NUM)
            {
                mcSHOW_DBG_MSG(("\nRuntime Shmoo Backup/Restore buffer(%d) empty!!\n",RUNTIME_SHMOO_RG_BACKUP_NUM));
                while(1);
            }
            if(u1IsLP4Family(p->dram_type) && (p->femmc_Ready==0))
            {
                *(U32*)(u4RG_Backup+ii) = u4IO32Read4B(u4RegBackupAddress[ii]);
                mcSHOW_DBG_MSG(("=== run time shmoo backup : *(U32*)(0x%x) = 0x%x\n",u4RegBackupAddress[ii],*(U32*)(u4RG_Backup+ii)));
            }
            else
            {
                vIO32Write4B(u4RegBackupAddress[ii], *(U32*)(u4RG_Backup+ii));
                mcSHOW_DBG_MSG(("=== run time shmoo restore: *(U32*)(0x%x) = 0x%x\n",u4RegBackupAddress[ii],*(U32*)(u4RG_Backup+ii)));
            }
        }
    }
    vSetPHY2ChannelMapping(p, channel_backup);

    //DramcRegDump(p);//for run time Tx eye scan RG check
}
#endif

