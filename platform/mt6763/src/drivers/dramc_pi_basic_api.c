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
 * $RCSfile: pi_basic_api.c,v $
 * $Revision: #5 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_basic_api.c
 *  Basic DRAMC API implementation
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
//#include "..\Common\pd_common.h"
//#include "Register.h"
#if __ETT__
#include "processor.h"
#endif
#include "dramc_common.h"
#include "x_hal_io.h"
#if (FOR_DV_SIMULATION_USED==0)
#include "pll.h"
#include <rt5738.h>
#endif

//#include "DramC_reg.h"
//#include "System_reg.h"
//#include "string.h"
//#define mcFPRINTF(_x_)            do{}while(0)


//#define RESET_PAD_BYPASS_SELPH 1    //hw fix in KIBO


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
#if REG_ACCESS_PORTING_DGB
extern U8 RegLogEnable;
#endif
U8 u1PrintModeRegWrite=0;

U32 PLL_FBKDIV_6_0, PLL_IC_3_0, PLL_BR_3_0, PLL_BC_3_0, PLL_BC_4;
U32 PLL_BP_3_0, PLL_BP_4, PLL_BAND_6_0;

U32 PLL_FBKSEL_0;


const U8 uiLPDDR4_MRR_Mapping_POP[CHANNEL_NUM][16] =
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
const U8 PHY_Mapping_E1[32] =
{
    0, 4, 2, 3, 1, 5, 6, 7,
    11, 9, 10, 8, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23,
    27, 24, 26, 28, 25, 29, 30, 31
};
const U8 PHY_Mapping_E2E3[32] =
{
    0, 4, 2, 3, 1, 5, 6, 7,
    11, 9, 10, 8, 12, 13, 14, 15,
    16, 17, 18, 19, 23, 21, 22, 20,
    27, 26, 24, 28, 25, 29, 30, 31
};
#endif

const U8 uiLPDDR3_MRR_Mapping_POP[16] =
{
    24, 28, 25, 30, 29, 26, 27, 31,
    6, 5, 7, 4, 0, 2, 1, 3
};


#if DUAL_FREQ_K
U8 gu1PhyPLLEn=TRUE;
#endif

#if 0
//{runtime, MR23, MR1819}
const U16 TX3200_Runtime_tDQS2DQ[3][31] =
{
    { 40,  4, 100},
    { 50,  5, 114},
    { 60,  6, 125},
    { 70,  7, 135},
    { 80,  8, 143},
    { 90,  9, 150},
    {100, 10, 156},
    {120, 12, 176},
    {130, 13, 181},
    {150, 15, 197},
    {160, 16, 200},
    {180, 18, 214},
    {200, 20, 227},
    {220, 22, 239},
    {240, 24, 250},
    {260, 26, 260},
    {280, 28, 269},
    {300, 30, 278},
    {320, 32, 286},
    {340, 34, 293},
    {370, 37, 308},
    {390, 39, 315},
    {420, 42, 328},
    {450, 45, 341},
    {470, 47, 346},
    {500, 50, 357},
    {530, 53, 368},
    {560, 56, 378},
    {590, 59, 388},
    {620, 62, 397},
    {660, 66, 413}
};

const U16 TX2667_Runtime_tDQS2DQ[3][31] =
{
    { 36,  3,  90},
    { 36,  3,  82},
    { 48,  4, 100},
    { 60,  5, 115},
    { 72,  6, 129},
    { 72,  6, 120},
    { 84,  7, 131},
    { 96,  8, 141},
    {108,  9, 150},
    {120, 10, 158},
    {132, 11, 165},
    {156, 13, 186},
    {168, 14, 191},
    {180, 15, 196},
    {192, 16, 200},
    {216, 18, 216},
    {228, 19, 219},
    {252, 21, 233},
    {264, 22, 236},
    {288, 24, 248},
    {312, 26, 260},
    {324, 27, 261},
    {348, 29, 272},
    {372, 31, 282},
    {396, 33, 291},
    {420, 35, 300},
    {444, 37, 308},
    {468, 39, 316},
    {492, 41, 324},
    {516, 43, 331},
    {552, 46, 345}
};

const U16 TX1600_Runtime_tDQS2DQ[3][31] =
{
    { 20,  1,  50},
    { 20,  1,  45},
    { 20,  1,  42},
    { 40,  2,  77},
    { 40,  2,  71},
    { 40,  2,  67},
    { 60,  3,  94},
    { 60,  3,  88},
    { 60,  3,  83},
    { 80,  4, 105},
    { 80,  4, 100},
    {100,  5, 119},
    {100,  5, 114},
    {100,  5, 109},
    {120,  6, 125},
    {120,  6, 120},
    {140,  7, 135},
    {140,  7, 130},
    {160,  8, 143},
    {180,  9, 155},
    {180,  9, 150},
    {200, 10, 161},
    {200, 10, 156},
    {220, 11, 167},
    {240, 12, 176},
    {260, 13, 186},
    {260, 13, 181},
    {280, 14, 189},
    {300, 15, 197},
    {320, 16, 205},
    {320, 16, 200}
};
#endif

#if ENABLE_RODT_TRACKING_SAVE_MCK
// global variables for RODT tracking & ROEN
U8 u1ODT_ON;        // infor of p->odt_onoff
U8 u1WDQS_ON = 0;   // infor of WDQS on(ROEN=1)
U8 u1RODT_TRACK=0;  // infor of rodt tracking enable
U8 u1ROEN, u1ModeSel;//status of ROEN, MODESEL setting
#endif

void DramcPrintRXDQDQSStatus(DRAMC_CTX_T *p, U8 u1Channel);
void DramcPrintIMPTrackingStatus(DRAMC_CTX_T *p, U8 u1Channel);

static void TxCalibrationMoveDQSByDifferentWL(DRAMC_CTX_T *p);

void vSetShuffleFrequency(DRAMC_CTX_T *p, DRAM_DFS_SHUFFLE_TYPE_T whitch_shuffle, U16 frequency);
void Hynix_Test_Mode(DRAMC_CTX_T *p);
void DramcHWGatingDebugOnOff(DRAMC_CTX_T *p, U8 u1OnOff);

#if (FOR_DV_SIMULATION_USED==1)
extern void broadcast_on(void);
extern void broadcast_off(void);
#endif

void DramcBroadcastOnOff(U32 bOnOff)
{
#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
    *((volatile unsigned int *)(DRAMC_WBR)) = bOnOff;
#endif
#if (FOR_DV_SIMULATION_USED==1)
    if(bOnOff)
    {broadcast_on();}
    else
    {broadcast_off();}
#endif
}

DRAM_IC_VERSION_T u1GetICVersion_Bianco(void)
{
#if (fcFOR_CHIP_ID == fcBianco)
/*
    1. 1st, read FAB_INFO4 [24:23] ECO code
    2'b00 :   E1
    2'b01 :   E2 or E3
    2'b10 :   E3
    2'b11 :   Reserved
    
    2. If value is 2'b01, then read (0x0800000c)
    32'h00000000: E2
    32'h00000001: E3
    */  

    U32 u4reg_value;

    u4reg_value = ((seclib_get_devinfo_with_index(21) & 0x01800000) >> 23);
    //mcSHOW_ERR_MSG(("u1GetICVersion_Bianco 0x%x\n", u4reg_value));

    if(u4reg_value ==0)
        return IC_VERSION_E1;
    else if(u4reg_value ==2)
        return IC_VERSION_E3;
    else if(u4reg_value ==1) //E2 or E3
    {
        u4reg_value = mt_get_chip_sw_ver();
        
        if(u4reg_value ==0)
            return IC_VERSION_E2;
        else
            return IC_VERSION_E3;
    }
    else
        ASSERT(0);

    return 0;
#else
    return 0;
#endif
}


U32 GetDramcBroadcast(void)
{
#if (FOR_DV_SIMULATION_USED==0) 
    return *((volatile unsigned int *)(DRAMC_WBR));
#endif
}

U8 u1IsLP4Family(DRAM_DRAM_TYPE_T dram_type)
{
    if (dram_type == TYPE_LPDDR4 || dram_type == TYPE_LPDDR4X || dram_type == TYPE_LPDDR4P)
        return 1;
    else
        return 0;
}

/* To support frequencies not on the ACTimingTable, we use read/write latency
 * settings as the next highest freq listed in ACTimingTable. Use this API
 * to set which freq group the target freq belongs to.
 * (Set's DRAMC_CTX_T's p->freqGroup)
 * Currently is designed to be called from DDRPhyFreqSel()
 */
void setFreqGroup(DRAMC_CTX_T *p)
{

    /* Below listed conditions represent freqs that exist in ACTimingTable
     * -> Should cover freqGroup settings for all real freq values
     */
    if (u1IsLP4Family(p->dram_type))
    {
        if (p->frequency <= 800) // DDR1600, DDR800
        {
            p->freqGroup = 800;
        }
        else if (p->frequency <= 1200) //DDR2400, DDR2280
        {
            p->freqGroup = 1200;
        }
        else if (p->frequency <= 1333) // DDR2667
        {
            p->freqGroup = 1333;
        }
        else if (p->frequency <= 1600) // DDR3200
        {
            p->freqGroup = 1600;
        }
        else  // DDR3733
        {
            p->freqGroup = 1866;
        }
    }
    else
    {
        if (p->frequency <= 667) // DDR1200, DDR1333
        {
            p->freqGroup = 667;
        }
        else if (p->frequency <= 800) // DDR1600
        {
            p->freqGroup = 800;
        }
        else // DDR1866
        {
            p->freqGroup = 933;
        }
        //p->freqGroup = 933; //temp workaround for 1600 & 1200
    }

    mcSHOW_DBG_MSG3(("[setFreqGroup] p-> frequency %u, freqGroup: %u\n", p->frequency, p->freqGroup));
    return;
}

void Set_MRR_Pinmux_Mapping(DRAMC_CTX_T *p)
{
    U8 *uiLPDDR_MRR_Mapping = NULL;

    if (p->dram_type == TYPE_LPDDR4 || p->dram_type == TYPE_LPDDR4X || p->dram_type == TYPE_LPDDR4P)
    {
        DRAM_CHANNEL_T channel_bak = vGetPHY2ChannelMapping(p);
        DRAM_CHANNEL_T chIdx = CHANNEL_A;
        for(chIdx = CHANNEL_A; chIdx<p->support_channel_num; chIdx++)
        {
            vSetPHY2ChannelMapping(p, chIdx);
            uiLPDDR_MRR_Mapping = (U8 *)uiLPDDR4_MRR_Mapping_POP[chIdx];
            //Set MRR pin mux
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_MRR_BIT_MUX1), P_Fld( uiLPDDR_MRR_Mapping[0], MRR_BIT_MUX1_MRR_BIT0_SEL) | P_Fld( uiLPDDR_MRR_Mapping[1], MRR_BIT_MUX1_MRR_BIT1_SEL) |
                                                                       P_Fld( uiLPDDR_MRR_Mapping[2], MRR_BIT_MUX1_MRR_BIT2_SEL) | P_Fld( uiLPDDR_MRR_Mapping[3], MRR_BIT_MUX1_MRR_BIT3_SEL));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_MRR_BIT_MUX2), P_Fld( uiLPDDR_MRR_Mapping[4], MRR_BIT_MUX2_MRR_BIT4_SEL) | P_Fld( uiLPDDR_MRR_Mapping[5], MRR_BIT_MUX2_MRR_BIT5_SEL) |
                                                                       P_Fld( uiLPDDR_MRR_Mapping[6], MRR_BIT_MUX2_MRR_BIT6_SEL) | P_Fld( uiLPDDR_MRR_Mapping[7], MRR_BIT_MUX2_MRR_BIT7_SEL));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_MRR_BIT_MUX3), P_Fld( uiLPDDR_MRR_Mapping[8], MRR_BIT_MUX3_MRR_BIT8_SEL) | P_Fld( uiLPDDR_MRR_Mapping[9], MRR_BIT_MUX3_MRR_BIT9_SEL) |
                                                                       P_Fld( uiLPDDR_MRR_Mapping[10], MRR_BIT_MUX3_MRR_BIT10_SEL) | P_Fld(uiLPDDR_MRR_Mapping[11], MRR_BIT_MUX3_MRR_BIT11_SEL));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_MRR_BIT_MUX4), P_Fld( uiLPDDR_MRR_Mapping[12], MRR_BIT_MUX4_MRR_BIT12_SEL) | P_Fld(uiLPDDR_MRR_Mapping[13], MRR_BIT_MUX4_MRR_BIT13_SEL) |
                                                                       P_Fld( uiLPDDR_MRR_Mapping[14], MRR_BIT_MUX4_MRR_BIT14_SEL) | P_Fld(uiLPDDR_MRR_Mapping[15], MRR_BIT_MUX4_MRR_BIT15_SEL));        
        }
        vSetPHY2ChannelMapping(p, channel_bak);
    }
#if ENABLE_LP3_SW
    else
    {
        uiLPDDR_MRR_Mapping = (U8 *)uiLPDDR3_MRR_Mapping_POP;
        
        //Set MRR pin mux
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_MRR_BIT_MUX1), P_Fld( uiLPDDR_MRR_Mapping[0], MRR_BIT_MUX1_MRR_BIT0_SEL) | P_Fld( uiLPDDR_MRR_Mapping[1], MRR_BIT_MUX1_MRR_BIT1_SEL) |
                                                                   P_Fld( uiLPDDR_MRR_Mapping[2], MRR_BIT_MUX1_MRR_BIT2_SEL) | P_Fld( uiLPDDR_MRR_Mapping[3], MRR_BIT_MUX1_MRR_BIT3_SEL));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_MRR_BIT_MUX2), P_Fld( uiLPDDR_MRR_Mapping[4], MRR_BIT_MUX2_MRR_BIT4_SEL) | P_Fld( uiLPDDR_MRR_Mapping[5], MRR_BIT_MUX2_MRR_BIT5_SEL) |
                                                                   P_Fld( uiLPDDR_MRR_Mapping[6], MRR_BIT_MUX2_MRR_BIT6_SEL) | P_Fld( uiLPDDR_MRR_Mapping[7], MRR_BIT_MUX2_MRR_BIT7_SEL));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_MRR_BIT_MUX3), P_Fld( uiLPDDR_MRR_Mapping[8], MRR_BIT_MUX3_MRR_BIT8_SEL) | P_Fld( uiLPDDR_MRR_Mapping[9], MRR_BIT_MUX3_MRR_BIT9_SEL) |
                                                                   P_Fld( uiLPDDR_MRR_Mapping[10], MRR_BIT_MUX3_MRR_BIT10_SEL) | P_Fld(uiLPDDR_MRR_Mapping[11], MRR_BIT_MUX3_MRR_BIT11_SEL));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_MRR_BIT_MUX4), P_Fld( uiLPDDR_MRR_Mapping[12], MRR_BIT_MUX4_MRR_BIT12_SEL) | P_Fld(uiLPDDR_MRR_Mapping[13], MRR_BIT_MUX4_MRR_BIT13_SEL) |
                                                                   P_Fld( uiLPDDR_MRR_Mapping[14], MRR_BIT_MUX4_MRR_BIT14_SEL) | P_Fld(uiLPDDR_MRR_Mapping[15], MRR_BIT_MUX4_MRR_BIT15_SEL));
    }
#endif /* ENABLE_LP3_SW */


}

#if ENABLE_LP3_SW
void PHY_MRR_Pinmux_Init_by_Chipversion(void)
{
    if (u1GetICVersion_Bianco() == IC_VERSION_E1) //if chip version is not E1, DQ 20<->23 and DQ25<->26 should be swap
    {
        uiLPDDR3_PHY_Mapping_POP = PHY_Mapping_E1;
    }
    else //E2 and E3
    {
        uiLPDDR3_PHY_Mapping_POP = PHY_Mapping_E2E3; 
    }  
}
#endif /* ENABLE_LP3_SW */

void Global_Option_Init(DRAMC_CTX_T *p)
{
    SaveCurrDramCtx(p);
    vSetChannelNumber(p);
    vSetRankNumber(p);
#if ENABLE_LP3_SW
    PHY_MRR_Pinmux_Init_by_Chipversion();
#endif
    Set_MRR_Pinmux_Mapping(p);
    vInitGlobalVariablesByCondition();
    
    vSetShuffleFrequency(p, DRAM_DFS_SHUFFLE_1, 800);
    vSetShuffleFrequency(p, DRAM_DFS_SHUFFLE_2, 800);
    vSetShuffleFrequency(p, DRAM_DFS_SHUFFLE_3, 800);
    vSetShuffleFrequency(p, DRAM_DFS_SHUFFLE_4, 800);    
    
#ifdef FOR_HQA_TEST_USED
    HQA_measure_message_reset_all_data(p);
#endif
}

void RISCWriteDRAM(unsigned int UI_offset_address, unsigned int UI_content_value)
{
    *((volatile unsigned int *)(Channel_A_DRAMC_AO_BASE_ADDRESS + UI_offset_address)) = UI_content_value;
    *((volatile unsigned int *)(Channel_A_DRAMC_NAO_BASE_ADDRESS + UI_offset_address)) = UI_content_value;
} // end of RISCWriteDRAM

void RISCWriteDDRPHY(unsigned int UI_offset_address, unsigned int UI_content_value)
{
    *((volatile unsigned int *)(Channel_A_PHY_AO_BASE_VIRTUAL + UI_offset_address)) = UI_content_value;
} // end of RISCWriteDDRPHY

void RISCWrite(unsigned int UI_offset_address, unsigned int UI_content_value)
{
    *((volatile unsigned int *)(UI_offset_address)) = UI_content_value;
} // end of RISCWrite

unsigned int RISCRead(unsigned int UI_offset_address)
{
    return *((volatile unsigned int *)(UI_offset_address));
} // end of RISCWrite

#if 0//__ETT__
void RISCReadAll(void)
{
    unsigned int address = DRAMC0_BASE;
    unsigned int value = 0;

    /*TINFO=">>>>>DRAMC0<<<<<\n"*/
    mcSHOW_DBG_MSG((">>>>>DRAMC0<<<<<\n"));
    for(address=DRAMC0_BASE; address<=DRAMC0_BASE+0xD54; address+=4)
    {
        value = *((volatile unsigned int *)(address));
        #ifdef ETT_PRINT_FORMAT
        mcSHOW_DBG_MSG(("Addr. 0x%X = 0x%X\n", address, value));
        #else
        mcSHOW_DBG_MSG(("Addr. 0x%8x = 0x%8x\n", address, value));
        #endif
        /*TINFO="[REG_ACCESS_PORTING_DBG] Register_Dump Reg(%h) = %h\n", address, value*/
    }

    /*TINFO=">>>>>DRAMC1<<<<<\n"*/
    mcSHOW_DBG_MSG((">>>>>DRAMC1<<<<<\n"));
    for(address=DRAMC1_BASE; address<=DRAMC1_BASE+0xD54; address+=4)
    {
        value = *((volatile unsigned int *)(address));
        #ifdef ETT_PRINT_FORMAT
        mcSHOW_DBG_MSG(("Addr. 0x%X = 0x%X\n", address, value));
        #else
        mcSHOW_DBG_MSG(("Addr. 0x%8x = 0x%8x\n", address, value));
        #endif
        /*TINFO="[REG_ACCESS_PORTING_DBG] Register_Dump Reg(%h) = %h\n", address, value*/
    }

    /*TINFO=">>>>>DRAMC0_NAO<<<<<\n"*/
    mcSHOW_DBG_MSG((">>>>>DRAMC0_NAO<<<<<\n"));
    for(address=DRAMC0_NAO_BASE; address<=DRAMC0_NAO_BASE+0x59C; address+=4)
    {
        value = *((volatile unsigned int *)(address));
        #ifdef ETT_PRINT_FORMAT
        mcSHOW_DBG_MSG(("Addr. 0x%X = 0x%X\n", address, value));
        #else
        mcSHOW_DBG_MSG(("Addr. 0x%8x = 0x%8x\n", address, value));
        #endif
        /*TINFO="[REG_ACCESS_PORTING_DBG] Register_Dump Reg(%h) = %h\n", address, value*/
    }

    /*TINFO=">>>>>DRAMC1_NAO<<<<<\n"*/
    mcSHOW_DBG_MSG((">>>>>DRAMC1_NAO<<<<<\n"));
    for(address=DRAMC1_NAO_BASE; address<=DRAMC1_NAO_BASE+0x59C; address+=4)
    {
        value = *((volatile unsigned int *)(address));
        #ifdef ETT_PRINT_FORMAT
        mcSHOW_DBG_MSG(("Addr. 0x%X = 0x%X\n", address, value));
        #else
        mcSHOW_DBG_MSG(("Addr. 0x%8x = 0x%8x\n", address, value));
        #endif
        /*TINFO="[REG_ACCESS_PORTING_DBG] Register_Dump Reg(%h) = %h\n", address, value*/
    }

    /*TINFO=">>>>>DDRPHY_A<<<<<\n"*/
    mcSHOW_DBG_MSG((">>>>>DDRPHY_A<<<<<\n"));
    for(address=DDRPHY_BASE; address<=DDRPHY_BASE+0xFD4; address+=4)
    {
        value = *((volatile unsigned int *)(address));
        #ifdef ETT_PRINT_FORMAT
        mcSHOW_DBG_MSG(("Addr. 0x%X = 0x%X\n", address, value));
        #else
        mcSHOW_DBG_MSG(("Addr. 0x%8x = 0x%8x\n", address, value));
        #endif
        /*TINFO="[REG_ACCESS_PORTING_DBG] Register_Dump Reg(%h) = %h\n", address, value*/
    }

    /*TINFO=">>>>>DDRPHY_B<<<<<\n"*/
    mcSHOW_DBG_MSG((">>>>>DDRPHY_B<<<<<\n"));
    for(address=DDRPHY1_BASE; address<=DDRPHY1_BASE+0xFD4; address+=4)
    {
        value = *((volatile unsigned int *)(address));
        #ifdef ETT_PRINT_FORMAT
        mcSHOW_DBG_MSG(("Addr. 0x%X = 0x%X\n", address, value));
        #else
        mcSHOW_DBG_MSG(("Addr. 0x%8x = 0x%8x\n", address, value));
        #endif
        /*TINFO="[REG_ACCESS_PORTING_DBG] Register_Dump Reg(%h) = %h\n", address, value*/
    }

    /*TINFO=">>>>>DDRPHY_C<<<<<\n"*/
    mcSHOW_DBG_MSG((">>>>>DDRPHY_C<<<<<\n"));
    for(address=DDRPHY2_BASE; address<=DDRPHY2_BASE+0xFD4; address+=4)
    {
        value = *((volatile unsigned int *)(address));
        #ifdef ETT_PRINT_FORMAT
        mcSHOW_DBG_MSG(("Addr. 0x%X = 0x%X\n", address, value));
        #else
        mcSHOW_DBG_MSG(("Addr. 0x%8x = 0x%8x\n", address, value));
        #endif
        /*TINFO="[REG_ACCESS_PORTING_DBG] Register_Dump Reg(%h) = %h\n", address, value*/
    }
    /*TINFO=">>>>>DDRPHY_D<<<<<\n"*/
    mcSHOW_DBG_MSG((">>>>>DDRPHY_D<<<<<\n"));
    for(address=DDRPHY3_BASE; address<=DDRPHY3_BASE+0xFD4; address+=4)
    {
        value = *((volatile unsigned int *)(address));
        #ifdef ETT_PRINT_FORMAT
        mcSHOW_DBG_MSG(("Addr. 0x%X = 0x%X\n", address, value));
        #else
        mcSHOW_DBG_MSG(("Addr. 0x%8x = 0x%8x\n", address, value));
        #endif
        /*TINFO="[REG_ACCESS_PORTING_DBG] Register_Dump Reg(%h) = %h\n", address, value*/
    }
}
#endif

void AutoRefreshCKEOff(DRAMC_CTX_T *p)
{
    //if(p->vendor_id==VENDOR_SAMSUNG && p->dram_type==TYPE_LPDDR3)
    {
        mcSHOW_DBG_MSG(("AutoRefreshCKEOff AutoREF OFF\n"));
        vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL0, 0x1, REFCTRL0_REFDIS);
        mcDELAY_US(3);
        mcSHOW_DBG_MSG(("DDRPhyPLLSetting-CKEOFF\n"));
        CKEFixOnOff(p, CKE_FIXOFF, CKE_WRITE_TO_ALL_CHANNEL);
        mcDELAY_US(1);
    }
}

void CKEFixOnOff(DRAMC_CTX_T *p, CKE_FIX_OPTION option, CKE_FIX_CHANNEL WriteChannelNUM)
{
    U8 u1CKEOn, u1CKEOff, u1setChannel,u1BackupChannel;

    if (option == CKE_DYNAMIC) //if CKE is dynamic, set both CKE fix On and Off as 0
    {                          //After CKE FIX on/off, CKE should be returned to dynamic (control by HW)
        u1CKEOn = u1CKEOff = 0;
    }
    else //if CKE fix on is set as 1, CKE fix off should also be set as 0; vice versa
    {
        u1CKEOn = option;
        u1CKEOff = (1 - option);
    }

    if (WriteChannelNUM == CKE_WRITE_TO_ALL_CHANNEL) //write register to all channel
    {
    #if SET_CKE_2_RANK_INDEPENDENT
            if (p->rank == RANK_0)
            {
                 vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), P_Fld(u1CKEOff, CKECTRL_CKEFIXOFF)|P_Fld(u1CKEOn, CKECTRL_CKEFIXON));
            }
            else
            {
                 vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), P_Fld(u1CKEOff, CKECTRL_CKE1FIXOFF)|P_Fld(u1CKEOn, CKECTRL_CKE1FIXON));
            }
    #else
            vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), P_Fld(u1CKEOff, CKECTRL_CKEFIXOFF)|P_Fld(u1CKEOn, CKECTRL_CKEFIXON)); 
    #endif
    }
    else
    {
    #if SET_CKE_2_RANK_INDEPENDENT
        if (p->rank == RANK_0)
        {
             vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), P_Fld(u1CKEOff, CKECTRL_CKEFIXOFF)|P_Fld(u1CKEOn, CKECTRL_CKEFIXON));
        }
        else
        {
             vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), P_Fld(u1CKEOff, CKECTRL_CKE1FIXOFF)|P_Fld(u1CKEOn, CKECTRL_CKE1FIXON));
        }
    #else
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), P_Fld(u1CKEOff, CKECTRL_CKEFIXOFF)|P_Fld(u1CKEOn, CKECTRL_CKEFIXON)); 
    #endif
    }
}

/* RxDQSIsiPulseCG() - API for "RX DQS ISI pulse CG function" 0: disable, 1: enable
 * 1. RG_*_RPRE_TOG_EN (16nm APHY): B0_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B0, B1_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B1
 * 2. RG_RX_*RDQS*_DQSSTB_CG_EN*(10nm APHY): B0_DQ8_RG_RX_ARDQS_DQSSTB_CG_EN_B0, B1_DQ8_RG_RX_ARDQS_DQSSTB_CG_EN_B1
 * Supports setting current channel only, add function to set "all channels" in the future
 */
void RxDQSIsiPulseCG(DRAMC_CTX_T *p, U8 u1OnOff)
{
    mcSHOW_DBG_MSG3(("CH%u RX DQS ISI pulse CG: %u (0:disable, 1:enable)\n", u1OnOff));

    /* LP4: Disable(set to 0) "RX DQS ISI pulse CG function" during the below senarios (must enable(set to 1) when done)
     *      1. Gating window calibration
     *      2. Duty related calibration (Justin: prevents DQSI from being kept high after READ burst)
     * LP3: should always be set to 1
     */
#if (fcFOR_CHIP_ID == fcBianco)
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), u1OnOff, B0_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), u1OnOff, B1_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B1);
#else
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ8), u1OnOff, B0_DQ8_RG_RX_ARDQS_DQSSTB_CG_EN_B0));
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ8), u1OnOff, B1_DQ8_RG_RX_ARDQS_DQSSTB_CG_EN_B1));
#endif

    return;
}

void DVFSSettings(DRAMC_CTX_T *p)
{
    U8 u1DVFS_52M_104M_SEL = 1; // DVFS_SM freq: 0: 52Mhz 1:104Mhz
    U8 u1DLL_Idle = 0x30;   //DDR800

#if (fcFOR_CHIP_ID == fcBianco)
    /* DVFS_SM LP4: dll_idle 90MCK/19.5ns, LP3: 70MCK   (DE: Lynx)
     * Below values are pre-calculated for each freq and dram type specifically
     * for 52M DVFS_SM mode (104M should multiply values by 2)
     * Since this register value is related to dram's operating freq
     *  -> Each freq must use specific pre-calculated value
     * (Couldn't use formula to calculate this value because floating point numbers are
     *  not supported via preloader)
     */
    if(u1IsLP4Family(p->dram_type))
    {
        switch (p->frequency)
        {
            case 1866:
                u1DLL_Idle = 0xa;
                break;
            case 1600:
                u1DLL_Idle = 0xc;
                break;
            case 1333:
                u1DLL_Idle = 0xd;
                break;
            case 1200:
                u1DLL_Idle = 0x10;
                break;
            case 800:
                u1DLL_Idle = 0x18;
                break;
            case 400:
                u1DLL_Idle = 0x30;
                break;
            default:
                mcSHOW_ERR_MSG(("u1DLL_Idle err!\n"));
                u1DLL_Idle = 0x30; //Set to a large value, but should check with designer for actual settings
                #if __ETT__
                while(1);
                #endif
        }
    }
    else
    {
        switch (p->frequency)
        {
            case 933:
                u1DLL_Idle = 0x8;
                break;
            case 800:
                u1DLL_Idle = 0x9;
                break;
            case 600:
                u1DLL_Idle = 0xC;
                break;
            default:
                mcSHOW_ERR_MSG(("u1DLL_Idle err!\n"));
                u1DLL_Idle = 0xC; //Set to a large value, but should check with designer for actual settings
                #if __ETT__
                while(1);
                #endif
        }
    }
#endif
    //52M_104M_SEL
    //*((volatile unsigned int*)(0x10000104)) |= (0x1 << 14);//104M clock
    u1DLL_Idle = (u1DLL_Idle << u1DVFS_52M_104M_SEL); // If DVFS_SM uses 104M mode, u1DLL_Idle value should multiply by 2

    //DVFS debug enable - MRR_STATUS2_DVFS_STATE
    vIO32WriteFldAlign_All(DRAMC_REG_DVFSDLL, 1, DVFSDLL_R_DDRPHY_SHUFFLE_DEBUG_ENABLE);

    vIO32WriteFldAlign_All(DDRPHY_DVFS_EMI_CLK, u1DVFS_52M_104M_SEL, DVFS_EMI_CLK_RG_52M_104M_SEL); //Set DVFS_SM's clk
    vIO32WriteFldAlign_All(DRAMC_REG_SHUCTRL2, u1DLL_Idle, SHUCTRL2_R_DLL_IDLE);

    //CDC option
    //vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SHUCTRL2), 1, SHUCTRL2_R_DVFS_CDC_OPTION);//Lewis@20170331: Not set SHUCTRL2_R_DVFS_CDC_OPTION to 1 since it will lead DDR reserve mode fail in DDR2400 and DDR1600
}

void DDRPhyPLLSetting(DRAMC_CTX_T *p)
{
    U8 u1CAP_SEL;
    U8 u1MIDPICAP_SEL;
    U8 u1VTH_SEL;
    U16 u2SDM_PCW = 0;

    u1VTH_SEL = 0x2; /* RG_*RPI_MIDPI_VTH_SEL[1:0] is 2 for all freqs */

#if (fcFOR_CHIP_ID == fcBianco)
    if(p->dram_type == TYPE_LPDDR3)
    {
        if(p->frequency>=933)
        {
            u1CAP_SEL = 0x0;
        }
        else
        {
            u1CAP_SEL = 0x3;
        }
    }
    else //LP4
    {    
        if(p->frequency>=1600)
        {
            u1CAP_SEL = 0x0;
        }
        else
        {
            u1CAP_SEL = 0x3;
        }
    }
#endif

    if(p->dram_type == TYPE_LPDDR3)
    {
        u1MIDPICAP_SEL = 0x0;
    }
    else
    {
        if (p->frequency <= 800)//Lewis@20161129: Fix DDR1600 S-idle Vcore power is too big problem.
        {
            u1MIDPICAP_SEL = 0;
        }
        else if (p->frequency <= 1333)
        {
            u1MIDPICAP_SEL = 0x3;
        }
        else if (p->frequency <= 1600)
        {
            u1MIDPICAP_SEL = 0x2;
        }
        else//3733
        {
            u1MIDPICAP_SEL = 0x1;
        }
    }

#if ENABLE_DLL_MASTER_SLAVE_SHUFFLE
    if(u1IsLP4Family(p->dram_type))
    {
        if (p->frequency <= 933) // Only set DLL to all slave mode if DDR speed <= 1866
        {
            vIO32WriteFldAlign_All(DRAMC_REG_DVFSDLL, 1, DVFSDLL_R_BYPASS_1ST_DLL_SHU1);

            vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_DLL1, 0, SHU1_CA_DLL1_RG_ARDLL_PD_CK_SEL_CA);
            vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_DLL0, 1, SHU1_CA_DLL0_RG_ARDLL_PHDET_OUT_SEL_CA);
            vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_DLL0, 1, SHU1_CA_DLL0_RG_ARDLL_PHDET_IN_SWAP_CA);
            vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_DLL1, 1, SHU1_CA_DLL1_RG_ARDLL_FASTPJ_CK_SEL_CA);
            vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_DLL0, 0x7, SHU1_CA_DLL0_RG_ARDLL_GAIN_CA);
            vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_DLL0, 0x7, SHU1_CA_DLL0_RG_ARDLL_IDLECNT_CA);
            #if (fcFOR_CHIP_ID == fcBianco)
            //Bianco need to set RG_ARPI_RESERVE_CA[1], and not RG_*RDLL_FAST_PSJP_CA
            vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_CMD6, 0, SHU1_CA_CMD6_RG_ARPI_RESERVE_CA_BIT1); // RG_*RPI_RESERVE_CA[1] 1'b1 tracking leaf(slave)
            #else
            vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_DLL0, 0, SHU1_CA_DLL0_RG_ARDLL_FAST_PSJP_CA);
            #endif
        }
        else
        {
            vIO32WriteFldAlign_All(DRAMC_REG_DVFSDLL, 0, DVFSDLL_R_BYPASS_1ST_DLL_SHU1);
            
#if 0 //master setting already init at dram init, no need to set here for saving code size.
            vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_DLL1, 1, SHU1_CA_DLL1_RG_ARDLL_PD_CK_SEL_CA);
            vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_DLL0, 0, SHU1_CA_DLL0_RG_ARDLL_PHDET_OUT_SEL_CA);
            vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_DLL0, 0, SHU1_CA_DLL0_RG_ARDLL_PHDET_IN_SWAP_CA);
            vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_DLL1, 0, SHU1_CA_DLL1_RG_ARDLL_FASTPJ_CK_SEL_CA);
            vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_DLL0, 0x6, SHU1_CA_DLL0_RG_ARDLL_GAIN_CA);
            vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_DLL0, 0x9, SHU1_CA_DLL0_RG_ARDLL_IDLECNT_CA);
            //Bianco need to set RG_ARPI_RESERVE_CA[1], and not RG_*RDLL_FAST_PSJP_CA
            //vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD6, 1, SHU1_CA_CMD6_RG_ARPI_RESERVE_CA_BIT_01);
            vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_DLL0, 1, SHU1_CA_DLL0_RG_ARDLL_FAST_PSJP_CA);
#endif
        }
    }
#endif

    //if(p->vendor_id==VENDOR_SAMSUNG && p->dram_type==TYPE_LPDDR3)
    {
        mcSHOW_DBG_MSG(("DDRPhyPLLSetting-DMSUS\n"));
        vIO32WriteFldAlign_All(DDRPHY_MISC_SPM_CTRL0, 0x0, MISC_SPM_CTRL0_PHY_SPM_CTL0);
        vIO32WriteFldAlign_All(DDRPHY_MISC_SPM_CTRL2, 0x0, MISC_SPM_CTRL2_PHY_SPM_CTL2);
        vIO32WriteFldMulti_All(DDRPHY_MISC_SPM_CTRL1, P_Fld(0x1, MISC_SPM_CTRL1_RG_ARDMSUS_10) | P_Fld(0x1, MISC_SPM_CTRL1_RG_ARDMSUS_10_B0)
                                                   | P_Fld(0x1, MISC_SPM_CTRL1_RG_ARDMSUS_10_B1) | P_Fld(0x1, MISC_SPM_CTRL1_RG_ARDMSUS_10_CA));
    }

    //26M
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0x0, MISC_CG_CTRL0_CLK_MEM_SEL); 

    #ifdef USE_CLK26M
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0x1, MISC_CG_CTRL0_RG_DA_RREF_CK_SEL);
    #endif

        
    //MIDPI
    vIO32WriteFldMulti_All(DDRPHY_SHU1_B0_DQ6, P_Fld(0x0, SHU1_B0_DQ6_RG_ARPI_MIDPI_EN_B0)
                | P_Fld(0x0, SHU1_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0));
    vIO32WriteFldMulti_All(DDRPHY_SHU1_B1_DQ6, P_Fld(0x0, SHU1_B1_DQ6_RG_ARPI_MIDPI_EN_B1)
                | P_Fld(0x0, SHU1_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1));
    vIO32WriteFldMulti_All(DDRPHY_SHU1_CA_CMD6, P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA)
                | P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA));

    vIO32WriteFldMulti_All(DDRPHY_PLL4, P_Fld(0x0, PLL4_RG_RPHYPLL_ADA_MCK8X_EN)
                | P_Fld(0x0, PLL4_RG_RPHYPLL_RESETB));

    //PLL
    vIO32WriteFldAlign_All(DDRPHY_PLL1, 0x0, PLL1_RG_RPHYPLL_EN);
    vIO32WriteFldAlign_All(DDRPHY_PLL2, 0x0, PLL2_RG_RCLRPLL_EN);

    //DLL
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI2, 0x0, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI2, 0x0, B0_DLL_ARPI2_RG_ARDLL_PHDET_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI2, 0x0, B1_DLL_ARPI2_RG_ARDLL_PHDET_EN_B1);

    vIO32WriteFldMulti_All(DDRPHY_B0_DLL_ARPI2, P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_CG_MCK_B0)
                | P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B0)
                | P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_CG_MCTL_B0)
                | P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_CG_FB_B0)
                | P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_CG_DQS_B0)
                | P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_CG_DQM_B0)
                | P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_CG_DQ_B0)
                | P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B0)
                | P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0));
    vIO32WriteFldMulti_All(DDRPHY_B1_DLL_ARPI2, P_Fld(0x1, B1_DLL_ARPI2_RG_ARPI_CG_MCK_B1)
                | P_Fld(0x1, B1_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B1)
                | P_Fld(0x1, B1_DLL_ARPI2_RG_ARPI_CG_MCTL_B1)
                | P_Fld(0x1, B1_DLL_ARPI2_RG_ARPI_CG_FB_B1)
                | P_Fld(0x1, B1_DLL_ARPI2_RG_ARPI_CG_DQS_B1)
                | P_Fld(0x1, B1_DLL_ARPI2_RG_ARPI_CG_DQM_B1)
                | P_Fld(0x1, B1_DLL_ARPI2_RG_ARPI_CG_DQ_B1)
                | P_Fld(0x1, B1_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B1)
                | P_Fld(0x1, B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1));
    vIO32WriteFldMulti_All(DDRPHY_CA_DLL_ARPI2, P_Fld(0x1, CA_DLL_ARPI2_RG_ARPI_CG_MCK_CA)
                | P_Fld(0x1, CA_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_CA)
                | P_Fld(0x1, CA_DLL_ARPI2_RG_ARPI_CG_MCTL_CA)
                | P_Fld(0x1, CA_DLL_ARPI2_RG_ARPI_CG_FB_CA)
                | P_Fld(0x1, CA_DLL_ARPI2_RG_ARPI_CG_CS)
                | P_Fld(0x1, CA_DLL_ARPI2_RG_ARPI_CG_CLK)
                | P_Fld(0x1, CA_DLL_ARPI2_RG_ARPI_CG_CMD)
                | P_Fld(0x1, CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN)
                | P_Fld(0x1, CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA));

    //RESETB
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI0, 0x0, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI0, 0x0, B0_DLL_ARPI0_RG_ARPI_RESETB_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI0, 0x0, B1_DLL_ARPI0_RG_ARPI_RESETB_B1);

    mcDELAY_US(1);

    ///TODO: PLL/MIDPI Settings
    //Ref clock should be 20M~30M, if MPLL=52M, Pre-divider should be set to 1
    #ifdef USE_CLK26M
    vIO32WriteFldMulti_All(DDRPHY_SHU1_PLL8, P_Fld(0x0, SHU1_PLL8_RG_RPHYPLL_POSDIV) | P_Fld(0x0, SHU1_PLL8_RG_RPHYPLL_PREDIV));
    vIO32WriteFldMulti_All(DDRPHY_SHU1_PLL10, P_Fld(0x0, SHU1_PLL10_RG_RCLRPLL_POSDIV) | P_Fld(0x0, SHU1_PLL10_RG_RCLRPLL_PREDIV));
    #else //MPLL 52M
    vIO32WriteFldMulti_All(DDRPHY_SHU1_PLL8, P_Fld(0x0, SHU1_PLL8_RG_RPHYPLL_POSDIV) | P_Fld(0x1, SHU1_PLL8_RG_RPHYPLL_PREDIV));
    vIO32WriteFldMulti_All(DDRPHY_SHU1_PLL10, P_Fld(0x0, SHU1_PLL10_RG_RCLRPLL_POSDIV) | P_Fld(0x1, SHU1_PLL10_RG_RCLRPLL_PREDIV));
    #endif
    if(p->frequency==1866)
    {   // 2722/26=143(0x8f)
        u2SDM_PCW = 0x8f00;
    }
    else if(p->frequency==1600 || p->frequency==800 || p->frequency==400)
    {
        if(p->frequency==1600)
            u2SDM_PCW = 0x7300;
        else
            u2SDM_PCW = 0x7b00;

        if(p->frequency==400)
        {
            vIO32WriteFldAlign_All(DDRPHY_SHU1_PLL8, 0x1, SHU1_PLL8_RG_RPHYPLL_POSDIV);
            vIO32WriteFldAlign_All(DDRPHY_SHU1_PLL10, 0x1, SHU1_PLL10_RG_RCLRPLL_POSDIV);
        }
    }
    else if(p->frequency==1333 || p->frequency==667)
    {
        u2SDM_PCW = 0x6600;
    }
    else if (p->frequency == 1200)
    {
        u2SDM_PCW = 0x5c00;
    }
    else if (p->frequency == 1140)
    {
        u2SDM_PCW = 0x5700;
    }
#if ENABLE_LP3_SW
    //LP3
    else if(p->frequency==933)
    {
        u2SDM_PCW = 0x8f00;
    }
    else if(p->frequency==600)
    {
        u2SDM_PCW = 0x5c00;
    }
    else if(p->frequency==467)
    {
        u2SDM_PCW = 0x4700;
    }
#endif
    vIO32WriteFldAlign_All(DDRPHY_SHU1_PLL5, u2SDM_PCW, SHU1_PLL5_RG_RPHYPLL_SDM_PCW);
    vIO32WriteFldAlign_All(DDRPHY_SHU1_PLL7, u2SDM_PCW, SHU1_PLL7_RG_RCLRPLL_SDM_PCW);

    
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI0, 1, CA_DLL_ARPI0_RG_ARMCTLPLL_CK_SEL_CA);
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI0, 1, B0_DLL_ARPI0_RG_ARMCTLPLL_CK_SEL_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI0, 1, B1_DLL_ARPI0_RG_ARMCTLPLL_CK_SEL_B1);
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI1, 0, CA_DLL_ARPI1_RG_ARPI_CLKIEN_JUMP_EN);
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI1, 0, B0_DLL_ARPI1_RG_ARPI_MCTL_JUMP_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI1, 0, B1_DLL_ARPI1_RG_ARPI_MCTL_JUMP_EN_B1);


    vIO32WriteFldMulti_All(DDRPHY_SHU1_B0_DQ6, P_Fld(u1VTH_SEL, SHU1_B0_DQ6_RG_ARPI_MIDPI_VTH_SEL_B0)
                | P_Fld(u1CAP_SEL, SHU1_B0_DQ6_RG_ARPI_CAP_SEL_B0)
                | P_Fld(u1MIDPICAP_SEL, SHU1_B0_DQ6_RG_ARPI_MIDPI_CAP_SEL_B0));
    vIO32WriteFldMulti_All(DDRPHY_SHU1_B1_DQ6, P_Fld(u1VTH_SEL, SHU1_B1_DQ6_RG_ARPI_MIDPI_VTH_SEL_B1)
                | P_Fld(u1CAP_SEL, SHU1_B1_DQ6_RG_ARPI_CAP_SEL_B1)
                | P_Fld(u1MIDPICAP_SEL, SHU1_B1_DQ6_RG_ARPI_MIDPI_CAP_SEL_B1));
    vIO32WriteFldMulti_All(DDRPHY_SHU1_CA_CMD6, P_Fld(u1VTH_SEL, SHU1_CA_CMD6_RG_ARPI_MIDPI_VTH_SEL_CA)
                | P_Fld(u1CAP_SEL, SHU1_CA_CMD6_RG_ARPI_CAP_SEL_CA)
                | P_Fld(u1MIDPICAP_SEL, SHU1_CA_CMD6_RG_ARPI_MIDPI_CAP_SEL_CA));


    ///TODO: RESETB
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI0, 0x1, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI0, 0x1, B0_DLL_ARPI0_RG_ARPI_RESETB_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI0, 0x1, B1_DLL_ARPI0_RG_ARPI_RESETB_B1);
    mcDELAY_US(1);

    ///TODO: PLL EN
    vIO32WriteFldAlign_All(DDRPHY_PLL1, 0x1, PLL1_RG_RPHYPLL_EN);
    vIO32WriteFldAlign_All(DDRPHY_PLL2, 0x1, PLL2_RG_RCLRPLL_EN);
    mcDELAY_US(100);

    ///TODO: MIDPI Init 1
    vIO32WriteFldMulti_All(DDRPHY_PLL4, P_Fld(0x1, PLL4_RG_RPHYPLL_ADA_MCK8X_EN)
                | P_Fld(0x1, PLL4_RG_RPHYPLL_RESETB));

    mcDELAY_US(1);


    ///TODO: MIDPI Init 2
    /* MIDPI Settings (Olympus): DA_*RPI_MIDPI_EN, DA_*RPI_MIDPI_CKDIV4_EN
     * Justin suggests use frequency > 933 as boundary
     */
    if(p->frequency > 933)
    {
        vIO32WriteFldMulti_All(DDRPHY_SHU1_B0_DQ6, P_Fld(0x1, SHU1_B0_DQ6_RG_ARPI_MIDPI_EN_B0)
                    | P_Fld(0x0, SHU1_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0));
        vIO32WriteFldMulti_All(DDRPHY_SHU1_B1_DQ6, P_Fld(0x1, SHU1_B1_DQ6_RG_ARPI_MIDPI_EN_B1)
                    | P_Fld(0x0, SHU1_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1));
        vIO32WriteFldMulti_All(DDRPHY_SHU1_CA_CMD6, P_Fld(0x1, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA)
                    | P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA));
    }
    else
    {
        vIO32WriteFldMulti_All(DDRPHY_SHU1_B0_DQ6, P_Fld(0x0, SHU1_B0_DQ6_RG_ARPI_MIDPI_EN_B0)
                    | P_Fld(0x1, SHU1_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0));
        vIO32WriteFldMulti_All(DDRPHY_SHU1_B1_DQ6, P_Fld(0x0, SHU1_B1_DQ6_RG_ARPI_MIDPI_EN_B1)
                    | P_Fld(0x1, SHU1_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1));
        vIO32WriteFldMulti_All(DDRPHY_SHU1_CA_CMD6, P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA)
                    | P_Fld(0x1, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA));
    }
    mcDELAY_US(1);

  
    vIO32WriteFldMulti_All(DDRPHY_CA_DLL_ARPI2, P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCK_CA)
                | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_CA)
                | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCTL_CA)
                | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_FB_CA)
                | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CS)
                | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CLK)
                | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CMD)
                | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA));
    vIO32WriteFldMulti_All(DDRPHY_B0_DLL_ARPI2, P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCK_B0)
                | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B0)
                | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCTL_B0)
                | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_FB_B0)
                | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQS_B0)
                | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQM_B0)
                | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQ_B0)
                | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0));
    vIO32WriteFldMulti_All(DDRPHY_B1_DLL_ARPI2, P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_MCK_B1)
                | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B1)
                | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_MCTL_B1)
                | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_FB_B1)
                | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQS_B1)
                | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQM_B1)
                | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQ_B1)
                | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1));
    #if (fcFOR_CHIP_ID == fcBianco)
    if(u1IsLP4Family(p->dram_type))
    {
        vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI2, 1, CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN);
        vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI2, 0, B0_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B0);
        vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI2, 0, B1_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B1);
    }
    else
    {
        vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI2, 1, CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN);
        vIO32WriteFldAlign(DDRPHY_B0_DLL_ARPI2, 1, B0_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B0);
        vIO32WriteFldAlign(DDRPHY_B1_DLL_ARPI2, 0, B1_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B1);
        vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI2+((U32)CHANNEL_B<<POS_BANK_NUM), 0, CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN);
        vIO32WriteFldAlign(DDRPHY_B0_DLL_ARPI2+((U32)CHANNEL_B<<POS_BANK_NUM), 0, B0_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B0);
        vIO32WriteFldAlign(DDRPHY_B1_DLL_ARPI2+((U32)CHANNEL_B<<POS_BANK_NUM), 0, B1_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B1);
    }
    #endif
    
    mcDELAY_US(2);

    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0x1, MISC_CG_CTRL0_CLK_MEM_SEL);
    mcDELAY_US(1);

    //DLL
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI2, 0x1, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);
    mcDELAY_US(1);
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI2, 0x1, B0_DLL_ARPI2_RG_ARDLL_PHDET_EN_B0);
    mcDELAY_US(1);
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI2, 0x1, B1_DLL_ARPI2_RG_ARDLL_PHDET_EN_B1);
    mcDELAY_US(1);

    //if(p->vendor_id==VENDOR_SAMSUNG && p->dram_type==TYPE_LPDDR3)
    {
        mcSHOW_DBG_MSG(("DDRPhyPLLSetting-DMSUS\n\n"));
        vIO32WriteFldMulti_All(DDRPHY_MISC_SPM_CTRL1, P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10) | P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_B0)
                                                   | P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_B1) | P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_CA));
        vIO32WriteFldAlign_All(DDRPHY_MISC_SPM_CTRL0, 0xffffffff, MISC_SPM_CTRL0_PHY_SPM_CTL0);
        vIO32WriteFldAlign_All(DDRPHY_MISC_SPM_CTRL2, 0xffffffff, MISC_SPM_CTRL2_PHY_SPM_CTL2);

        mcSHOW_DBG_MSG(("DDRPhyPLLSetting-CKEON\n\n"));
        CKEFixOnOff(p, CKE_DYNAMIC, CKE_WRITE_TO_ALL_CHANNEL);
    }

    DDRPhyFreqMeter();
}

#if 1
void DramcPowerOnSequence(DRAMC_CTX_T *p)
{
    #if APPLY_LP4_POWER_INIT_SEQUENCE
    //static U8 u1PowerOn=0;
    //if(u1PowerOn ==0)
    {
    //reset dram = low
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 0x0, MISC_CTRL1_R_DMDA_RRESETB_I);

    //vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0, RKCFG_CKE2RANK_OPT2);

    //CKE low
    CKEFixOnOff(p, CKE_FIXOFF, CKE_WRITE_TO_ALL_CHANNEL);
    // delay tINIT1=200us(min) & tINIT2=10ns(min)
    mcDELAY_US(200);

    //reset dram = low
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 0x1, MISC_CTRL1_R_DMDA_RRESETB_I);

    // Disable HW MIOCK control to make CLK always on
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 1, DRAMC_PD_CTRL_MIOCKCTRLOFF);

    //tINIT3=2ms(min)
    mcDELAY_MS(2);

    //CKE high
    CKEFixOnOff(p, CKE_FIXON, CKE_WRITE_TO_ALL_CHANNEL);
    // tINIT5=2us(min)
    mcDELAY_US(2);
    //u1PowerOn=1;
    mcSHOW_DBG_MSG3(("APPLY_LP4_POWER_INIT_SEQUENCE\n"));
    }
    #endif
}

DRAM_STATUS_T DramcModeRegInit_LP4(DRAMC_CTX_T *p)
{
    U32 u4RankIdx;//, u4CKE0Bak, u4CKE1Bak, u4MIOCKBak, u4AutoRefreshBak;
    U16 u2MR3Value;
    U8 u1MRFsp= FSP_0;
    U8 u1ChannelIdx, support_channel_num;
    U8 backup_channel, backup_rank;
    U8 operating_fsp = p->dram_fsp;
    U32 backup_broadcast;

    backup_broadcast = GetDramcBroadcast();

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);

#if SET_CKE_2_RANK_INDEPENDENT
    // set CKE1 is the same as CKE0
    vIO32WriteFldMulti_All(DRAMC_REG_RKCFG, P_Fld(1, RKCFG_CKE2RANK)
                    | P_Fld(0, RKCFG_CKE2RANK_OPT)
                    | P_Fld(0, RKCFG_CKE2RANK_OPT2));
#endif

    DramcPowerOnSequence(p);
    
    backup_channel = p->channel;
    backup_rank = p->rank;


#if VENDER_JV_LOG
    vPrintCalibrationBasicInfo_ForJV(p);
#endif

#if SAMSUNG_LP4_NWR_WORKAROUND
    /* Samsung: Fix nWR value to 30 (MR01[6:4] = 101B)
     * Other vendors: Use default MR01 for each FSP (Set in vInitGlobalVariablesByCondition() )
     */
    if (p->vendor_id == VENDOR_SAMSUNG)
    {
        /* Clear MR01 OP[6:4] */
        u1MR01Value[FSP_0] &= 0x8F;
        u1MR01Value[FSP_1] &= 0x8F;
        /* Set MR01 OP[6:4] to 101B = 5 */
        u1MR01Value[FSP_0] |= (0x5 << 4);
        u1MR01Value[FSP_1] |= (0x5 << 4);

        mcSHOW_DBG_MSG3(("Samsung nWR fixed to 30\n"));
        #if __ETT__
        if (u2DFSGetHighestFreq(p) != 1600)
        {
            mcSHOW_ERR_MSG(("Workaround only suitable for projects with LP4_DDR3200 as highest freq!\n"));
            while(1);
        }
        #endif
    }
#endif

    for(u1ChannelIdx=0; u1ChannelIdx<(p->support_channel_num); u1ChannelIdx++)
    {
        vSetPHY2ChannelMapping(p, u1ChannelIdx);

        for(u4RankIdx =0; u4RankIdx < (U32)(p->support_rank_num); u4RankIdx++)
        {
            vSetRank(p, u4RankIdx);
            
            mcSHOW_DBG_MSG(("[ModeRegInit_LP4] CH%u RK%u\n", u1ChannelIdx, u4RankIdx));
            mcFPRINTF((fp_A60501, "[ModeRegInit_LP4] CH%u RK%d\n", u1ChannelIdx, u4RankIdx));
            #if VENDER_JV_LOG
            mcSHOW_DBG_MSG5(("\n[ModeRegInit_LP4] CH%u RK%d\n", u1ChannelIdx, u4RankIdx));
            #endif

            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u4RankIdx, MRS_MRSRK);

            // Note : MR37 for LP4P should be set before any Mode register.
            // MR37 is not shadow register, just need to set by channel and rank. No need to set by FSP
            if(p->dram_type == TYPE_LPDDR4P)
            {
                //temp solution, need remove later
                #ifndef MT6763_FPGA
                #if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
                rt5738_set_voltage(RT5738_VDDQ, 600000);
                #endif
                #endif

                DramcModeRegWrite(p, 37, 0x1);

                //temp solution, need remove later
                #ifndef MT6763_FPGA
                #if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
                rt5738_set_voltage(RT5738_VDDQ, 400000);
                #endif
                #endif
            }

#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
           if (operating_fsp == FSP_1 && Get_PRE_MIOCK_JMETER_HQA_USED_flag()==0)
           {
               //switch to low freq
               CBT_Switch_Freq(p, CBT_LOW_FREQ);
           }
#endif

#if ENABLE_LP4_ZQ_CAL
           DramcZQCalibration(p); //ZQ calobration should be done before CBT calibration by switching to low frequency
#endif

           // if(p->frequency<=1200)
            {
                /* FSP_0: 1. For un-term freqs   2. Assumes "data rate < DDR2667" are un-term */
                u1MRFsp = FSP_0;
                mcSHOW_DBG_MSG3(("\tFsp%d\n", u1MRFsp));
                mcFPRINTF((fp_A60501, "\tFsp%d\n", u1MRFsp));
                #if VENDER_JV_LOG
                mcSHOW_DBG_MSG5(("\tFsp%d\n", u1MRFsp));
                #endif

                u1MR13Value[u1MRFsp] = 0x0 | (MR13_RRO<<4) | (1<<3);
                DramcModeRegWrite(p, 0xd, u1MR13Value[u1MRFsp]); //FSP-0
                //MR12 use previous value
                DramcModeRegWrite(p, 0xc, u1MR12Value[u1ChannelIdx][u4RankIdx][u1MRFsp]); //MR12 VREF-CA
                DramcModeRegWrite(p, 0x1, u1MR01Value[u1MRFsp]);

                //MR2 set Read/Write Latency
                if (p->freqGroup == 800) // DDR1600, DDR800
                {
                    u1MR02Value[u1MRFsp] = 0x12;
                }
                else if (p->freqGroup == 1200) // DDR2280, DDR2400 (DDR2667 uses FSP_1)
                {
                    u1MR02Value[u1MRFsp] = 0x24;
                }

                DramcModeRegWrite(p, 0x2, u1MR02Value[u1MRFsp]);
                //if(p->odt_onoff)
                    DramcModeRegWrite(p, 0xb, 0x0);     //ODT

                if(p->dram_type == TYPE_LPDDR4)
                {
                    DramcModeRegWrite(p, 0x16, 0x20);    //SOC-ODT, ODTE-CK, ODTE-CS, Disable ODTD-CA
                }
                else  //TYPE_LPDDR4x, LP4P
                {
                    DramcModeRegWrite(p, 0x16, 0x38);    //SOC-ODT, ODTE-CK, ODTE-CS, Disable ODTD-CA
                }

                //MR14 use previous value
                DramcModeRegWrite(p, 0xe, u1MR14Value[u1ChannelIdx][u4RankIdx][u1MRFsp]);    //MR14 VREF-DQ      
                
                //MR3 set write-DBI and read-DBI (Disabled during calibration, enabled after K)
                u1MR03Value[u1MRFsp] = (u1MR03Value[u1MRFsp]&0x3F);
                
                if(p->dram_type == TYPE_LPDDR4X || p->dram_type == TYPE_LPDDR4P)
                {
                    #if PU_CAL_05VDDQ
                        u1MR03Value[u1MRFsp] &= 0xff;
                    #else
                        u1MR03Value[u1MRFsp] &= 0xfe;
                    #endif
                }
                DramcModeRegWrite(p, 0x3, u1MR03Value[u1MRFsp]);
            }
            //else
            {
                /* FSP_1: 1. For term freqs   2. Assumes "data rate >= DDR2667" are terminated */
                u1MRFsp = FSP_1;
                mcSHOW_DBG_MSG3(("\tFsp%d\n", u1MRFsp));
                mcFPRINTF((fp_A60501, "\tFsp%d\n", u1MRFsp));
                #if VENDER_JV_LOG
                mcSHOW_DBG_MSG5(("\tFsp%d\n", u1MRFsp));
                #endif

                u1MR13Value[u1MRFsp] = 0x40 | (MR13_RRO<<4) | (1<<3);
                DramcModeRegWrite(p, 0xd, u1MR13Value[u1MRFsp]); //FSP-1
                //MR12 use previous value
                DramcModeRegWrite(p, 0xc, u1MR12Value[u1ChannelIdx][u4RankIdx][u1MRFsp]); //MR12 VREF-CA
                DramcModeRegWrite(p, 0x1, u1MR01Value[u1MRFsp]);

                //MR2 set Read/Write Latency
                if (p->freqGroup == 1866)
                {
                    u1MR02Value[u1MRFsp] = 0x36;
                }
                else if (p->freqGroup == 1600)
                {
                    u1MR02Value[u1MRFsp] = 0x2d;
                }
                else if (p->freqGroup == 1333)
                {
                    u1MR02Value[u1MRFsp] = 0x24;
                }
                DramcModeRegWrite(p, 0x2, u1MR02Value[u1MRFsp]);

                if(p->dram_type == TYPE_LPDDR4P || p->odt_onoff==ODT_OFF)  //(p->odt_onoff)
                    DramcModeRegWrite(p, 0xb, 0x0);     //ODT
                else
                {
                    #if SOLUTION_FOR_SS_B422_TX_WIN_TOO_SMALL
                    if(p->vendor_id == VENDOR_SAMSUNG) //Samsung dram only
                    {
                        DramcModeRegWrite(p, 0xb, 0x03);     //ODT
                    }
                    else
                    #endif
                        DramcModeRegWrite(p, 0xb, 0x04);     //ODT
                }

                if(p->odt_onoff==ODT_OFF)
                {
                    if(p->dram_type == TYPE_LPDDR4)
                    {
                        DramcModeRegWrite(p, 0x16, 0x20);    //SOC-ODT, ODTE-CK, ODTE-CS, Disable ODTD-CA
                    }
                    else  //TYPE_LPDDR4x, LP4P
                    {
                        DramcModeRegWrite(p, 0x16, 0x38);    //SOC-ODT, ODTE-CK, ODTE-CS, Disable ODTD-CA
                    }
                }
                else
                {
                    if(p->dram_type == TYPE_LPDDR4)
                    {
                        DramcModeRegWrite(p, 0x16, 0x24);    //SOC-ODT, ODTE-CK, ODTE-CS, Disable ODTD-CA
                    }
                    else  //TYPE_LPDDR4x, LP4P
                    {
                        DramcModeRegWrite(p, 0x16, 0x3c);    //SOC-ODT, ODTE-CK, ODTE-CS, Disable ODTD-CA
                    }
                }

                //MR14 use previous value
                if(p->dram_type == TYPE_LPDDR4P || p->odt_onoff==ODT_OFF)  //(p->odt_onoff)
                    DramcModeRegWrite(p, 0xe, u1MR14Value[u1ChannelIdx][u4RankIdx][0]);    //MR14 VREF-DQ
                else
                    DramcModeRegWrite(p, 0xe, u1MR14Value[u1ChannelIdx][u4RankIdx][u1MRFsp]);    //MR14 VREF-DQ

                //MR3 set write-DBI and read-DBI (Disabled during calibration, enabled after K)
                u1MR03Value[u1MRFsp] = (u1MR03Value[u1MRFsp]&0x3F);
       
                if(p->dram_type == TYPE_LPDDR4X || p->dram_type == TYPE_LPDDR4P)
                {
                    #if PU_CAL_05VDDQ
                        u1MR03Value[u1MRFsp] &= 0xff;
                    #else
                        u1MR03Value[u1MRFsp] &= 0xfe;
                    #endif
                }
                DramcModeRegWrite(p, 0x3, u1MR03Value[u1MRFsp]);
            }

            /* freq < 1333 is assumed to be odt_off -> uses FSP_0 */
            if (p->frequency < 1333)
            {
                u1MRFsp = FSP_0;
                u1MR13Value[u1MRFsp] = 0x0 | (MR13_RRO<<4) | (1<<3);
                DramcModeRegWrite(p, 0xd, u1MR13Value[u1MRFsp]); //FSP-0
            }
            else
            {
                u1MRFsp = FSP_1;
                u1MR13Value[u1MRFsp] = 0xc0 | (MR13_RRO<<4) | (1<<3);
                DramcModeRegWrite(p, 0xd, u1MR13Value[u1MRFsp]); //FSP-1
            }

#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
            if (operating_fsp == FSP_1 && Get_PRE_MIOCK_JMETER_HQA_USED_flag()==0)
            {
                //switch to low freq
                CBT_Switch_Freq(p, CBT_HIGH_FREQ);
            }
#endif

        }

        //u1MRFsp = FSP_0; // for fix warning, Uninitialized array index read
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_MR2), P_Fld(u1MR02Value[u1MRFsp], SHU_HWSET_MR2_HWSET_MR2_OP)
                                                                    | P_Fld(2, SHU_HWSET_MR2_HWSET_MR2_MRSMA));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_MR13), P_Fld(u1MR13Value[u1MRFsp]|0x1<<3, SHU_HWSET_MR13_HWSET_MR13_OP)
                                                                    | P_Fld(13, SHU_HWSET_MR13_HWSET_MR13_MRSMA));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_VRCG), P_Fld(u1MR13Value[u1MRFsp]&0xf7, SHU_HWSET_VRCG_HWSET_VRCG_OP)
                                                                    | P_Fld(13, SHU_HWSET_VRCG_HWSET_VRCG_MRSMA));
    }

    vSetPHY2ChannelMapping(p, backup_channel);
    vSetRank(p, backup_rank);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), RANK_0, MRS_MRSRK);

    DramcBroadcastOnOff(backup_broadcast);

    return DRAM_OK;
}
#else

DRAM_STATUS_T DramcModeRegInit_LP4(DRAMC_CTX_T *p, U8 init_type)
{
    U32 u4RankIdx;//, u4CKE0Bak, u4CKE1Bak, u4MIOCKBak, u4AutoRefreshBak;
    U16 u2MR3Value;

    vPrintCalibrationBasicInfo(p);

    #if APPLY_LP4_POWER_INIT_SEQUENCE
    static U8 u1PowerOn=0;
    if(u1PowerOn ==0)
    {
        //reset dram = low
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DDRCONF0), 0, DDRCONF0_GDDR3RST);

        //CKE low
        CKEFixOnOff(p, CKE_FIXOFF, CKE_WRITE_TO_ONE_CHANNEL);
        // delay tINIT1=200us(min) & tINIT2=10ns(min)
        mcDELAY_US(200);

        //reset dram = low
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DDRCONF0), 1, DDRCONF0_GDDR3RST);

        // Disable HW MIOCK control to make CLK always on
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 1, DRAMC_PD_CTRL_MIOCKCTRLOFF);

        //tINIT3=2ms(min)
        mcDELAY_MS(2);

        //CKE high
        CKEFixOnOff(p, CKE_FIXON, CKE_WRITE_TO_ONE_CHANNEL);
        // tINIT5=2us(min)
        mcDELAY_US(2);
        u1PowerOn=1;
        mcSHOW_DBG_MSG3(("APPLY_LP4_POWER_INIT_SEQUENCE\n"));
    }
    #endif

    for(u4RankIdx =0; u4RankIdx < (U32)(p->support_rank_num); u4RankIdx++)
    {
        mcSHOW_DBG_MSG(("ModeRegInit_LP4 RK%d dram_fsp %d\n", u4RankIdx, p->dram_fsp));
        mcFPRINTF((fp_A60501, "ModeRegInit_LP4 RK%d dram_fsp %d\n", u4RankIdx, p->dram_fsp));

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u4RankIdx, MRS_MRSRK);

        // Note : MR37 for LP4P should be set before any Mode register.
        // MR37 is not shadow register, just need to set by channel and rank. No need to set by FSP
        if(p->dram_type == TYPE_LPDDR4P)
        {
            //temp solution, need remove later
            #ifndef MT6763_FPGA
            fan53528buc08x_set_voltage((unsigned long)600000);
            #endif

            DramcModeRegWrite(p, 37, 0x1);

            //temp solution, need remove later
            #ifndef MT6763_FPGA
            fan53528buc08x_set_voltage((unsigned long)400000);
            #endif
        }

        //if(p->frequency<=1200)
        if(p->odt_onoff == ODT_OFF)
        {
            u1MR13Value[p->dram_fsp] = 0 | (MR13_RRO<<4) | (1<<3);
            DramcModeRegWrite(p, 0xd, u1MR13Value[p->dram_fsp]); //FSP-0
//            u1MR12Value[p->channel][p->dram_fsp] = 0x4d;
            DramcModeRegWrite(p, 0xc, u1MR12Value[p->channel][p->dram_fsp]); //MR12 VREF-CA
            u1MR01Value[p->dram_fsp] = 0x56;
            DramcModeRegWrite(p, 0x1, u1MR01Value[p->dram_fsp]);

            if(p->frequency==1600)
            {
                u1MR02Value[p->dram_fsp] = 0x2d;
            }
            else if(p->frequency==1333)
            {
                u1MR02Value[p->dram_fsp] = 0x24;
            }
            else if(p->frequency==1200)
            {
            }
            else if(p->frequency==800)
            {
                u1MR02Value[p->dram_fsp] = 0x12;
            }
            DramcModeRegWrite(p, 0x2, u1MR02Value[p->dram_fsp]);
            DramcModeRegWrite(p, 0xb, 0x0);     //ODT
            if(p->dram_type == TYPE_LPDDR4)
            {
                DramcModeRegWrite(p, 0x16, 0x20);    //SOC-ODT, ODTE-CK, ODTE-CS, disable ODTD-CA
            }
            else  //TYPE_LPDDR4x & TYPE_LPDDR4P
            {
                DramcModeRegWrite(p, 0x16, 0x38);    //SOC-ODT, ODTE-CK, ODTE-CS, disable ODTD-CA
            }
  //          u1MR14Value[p->channel][p->dram_fsp] = 0x4d;
            DramcModeRegWrite(p, 0xe, u1MR14Value[p->channel][p->dram_fsp]);    //MR14 VREF-DQ
        }
        else
        {
            #if 0//CA-Term
            u1MR13Value[p->dram_fsp] = 0xc0 | (MR13_RRO<<4);
            DramcModeRegWrite(p, 0xd, u1MR13Value[p->dram_fsp]); //FSP-1
            u1MR12Value[p->dram_fsp] = 0x10;
            DramcModeRegWrite(p, 0xc, u1MR12Value[p->dram_fsp]); //MR12 VREF-CA
            u1MR01Value[p->dram_fsp] = 0x56;
            DramcModeRegWrite(p, 0x1, u1MR01Value[p->dram_fsp]);

            if(p->frequency==1600)
            {
                u1MR02Value[p->dram_fsp] = 0x2d;
            }
            else if(p->frequency==1333)
            {
                u1MR02Value[p->dram_fsp] = 0x24;
            }
            DramcModeRegWrite(p, 0x2, u1MR02Value[p->dram_fsp]);
            DramcModeRegWrite(p, 0xb, 0x44);     //ODT
            DramcModeRegWrite(p, 0x16, 0xC4);    //SOC-ODT, ODTE-CK, ODTE-CS, ODTD-CA
            u1MR14Value[p->dram_fsp] = 0x10;
            DramcModeRegWrite(p, 0xe, u1MR14Value);    //MR14 VREF-DQ
            #else
            u1MR13Value[p->dram_fsp] = 0xc0 | (MR13_RRO<<4) | (1<<3);
            DramcModeRegWrite(p, 0xd, u1MR13Value[p->dram_fsp]); //FSP-1
//            u1MR12Value[p->channel][p->dram_fsp] = 0x4d;
            DramcModeRegWrite(p, 0xc, u1MR12Value[p->channel][p->dram_fsp]); //MR12 VREF-CA
            DramcModeRegWrite(p, 0x1, 0x56);
            if(p->frequency==1600)
            {
                u1MR02Value[p->dram_fsp] = 0x2d;
            }
            else if(p->frequency==1333)
            {
                u1MR02Value[p->dram_fsp] = 0x24;
            }
            DramcModeRegWrite(p, 0x2, u1MR02Value[p->dram_fsp]);
            DramcModeRegWrite(p, 0xb, 0x04);     //ODT
            if(p->dram_type == TYPE_LPDDR4)
            {
                DramcModeRegWrite(p, 0x16, 0x24);    //SOC-ODT, ODTE-CK, ODTE-CS, disable ODTD-CA
            }
            else  //TYPE_LPDDR4x
            {
                DramcModeRegWrite(p, 0x16, 0x3c);    //SOC-ODT, ODTE-CK, ODTE-CS, disable ODTD-CA
            }

//            u1MR14Value[p->channel][p->dram_fsp] = 0x10;
            DramcModeRegWrite(p, 0xe, u1MR14Value[p->channel][p->dram_fsp]);    //MR14 VREF-DQ
            #endif
        }

        u2MR3Value = 0x31;
        u2MR3Value |= ((p->DBI_W_onoff)<<7 | (p->DBI_R_onoff)<<6);
        if(p->dram_type == TYPE_LPDDR4X || p->dram_type == TYPE_LPDDR4P)
                u2MR3Value &= 0xfe;  // MR3[0] =0, PU-Cal= VEED/2.5

        DramcModeRegWrite(p, 0x3, u2MR3Value);//Enable DBI
    }


    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), RANK_0, MRS_MRSRK);

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_MR2), P_Fld(u1MR02Value[p->dram_fsp], SHU_HWSET_MR2_HWSET_MR2_OP)
                                                                | P_Fld(2, SHU_HWSET_MR2_HWSET_MR2_MRSMA));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_MR13), P_Fld(u1MR13Value[p->dram_fsp]|(1<<3), SHU_HWSET_MR13_HWSET_MR13_OP)
                                                                | P_Fld(13, SHU_HWSET_MR13_HWSET_MR13_MRSMA));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_VRCG), P_Fld(u1MR13Value[p->dram_fsp]&0xf7, SHU_HWSET_VRCG_HWSET_VRCG_OP)
                                                                | P_Fld(13, SHU_HWSET_VRCG_HWSET_VRCG_MRSMA));

    return DRAM_OK;
}

#endif

#if (__ETT__ || ENABLE_LP3_SW) //remove unused function to save space of log string
DRAM_STATUS_T DramcModeRegInit_LP3(DRAMC_CTX_T *p)
{
    U32 u4RankIdx, u4AutoRefreshBak;

    #if 0
    U32 u4CKEFIXBak, u4MIOCKBak;
    u4MIOCKBak =u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), DRAMC_PD_CTRL_MIOCKCTRLOFF);
    u4CKEFIXBak = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL));
    #endif
    u4AutoRefreshBak= u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0), REFCTRL0_REFDIS);

    // Disable HW MIOCK control to make CLK always on
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 1, DRAMC_PD_CTRL_MIOCKCTRLOFF);
    mcDELAY_US(1);

    //if CKE2RANK=1, only need to set CKEFIXON, it will apply to both rank.
    CKEFixOnOff(p, CKE_FIXON, CKE_WRITE_TO_ONE_CHANNEL);

    // disable auto refresh command during Dram reset (MR63)
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0) ,1, REFCTRL0_REFDIS);

    //wait cke to reset timing constraint 'tinit3'
    //for (i=0; i < 10 ; ++i);
    mcDELAY_US(200);

    for(u4RankIdx =0; u4RankIdx < (U32)(p->support_rank_num); u4RankIdx++)
    {
        mcSHOW_DBG_MSG2(("ModeRegInit_LP3 for RK%d\n", u4RankIdx));
        mcFPRINTF((fp_A60501, "ModeRegInit_LP3 for RK%d\n", u4RankIdx));

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u4RankIdx, MRS_MRSRK);

        DramcModeRegWrite(p, 0x3f, 0xfc);       //MR63=0   -> Reset
        mcDELAY_US(10);  //wait 10us for dram reset

        if (p->vendor_id == VENDOR_HYNIX && p->revision_id == 4) //MR6 is the die ID, if u read as 4, that's 21nm die, and if u read as 3, that's 25nm die.
        {
            Hynix_Test_Mode(p);           
        }

        DramcModeRegWrite(p, 0xa, 0xff);   //MR10=0xff  -> ZQ Init

        DramcModeRegWrite(p, 1, 0x83);      //MR1=0x83 nWR: OP[7:5] (14 -> LP3-1866),   A60817 is 0x43  nWR: OP[7:5] (12 -> LP3-1600)

        // MR2
        /*
        if (p->freqGroup <= 533)
        {
            u1MR02Value[p->dram_fsp] = 0x16;
        }
        else
        */
        if (p->freqGroup == 667)
        {
            u1MR02Value[p->dram_fsp] = 0x18;
        }
        else if (p->freqGroup == 800)
        {
            u1MR02Value[p->dram_fsp] = 0x1a;
        }
        else
        {
            u1MR02Value[p->dram_fsp] = 0x1c;
        }
        DramcModeRegWrite(p, 2, u1MR02Value[p->dram_fsp]);

        DramcModeRegWrite(p, 0xb, 0x3);      //MR11=0x3 , ODT disable.
    }

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), 0, MRS_MRSRK);

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_MR2), P_Fld(0, SHU_HWSET_MR2_HWSET_MR2_OP)
                                                                | P_Fld(0, SHU_HWSET_MR2_HWSET_MR2_MRSMA));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_MR13), P_Fld(u1MR02Value[p->dram_fsp], SHU_HWSET_MR13_HWSET_MR13_OP)
                                                                | P_Fld(2, SHU_HWSET_MR13_HWSET_MR13_MRSMA));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_VRCG), P_Fld(0, SHU_HWSET_VRCG_HWSET_VRCG_OP)
                                                                | P_Fld(0, SHU_HWSET_VRCG_HWSET_VRCG_MRSMA));

    // Restore auto refresh command after Dram reset (MR63)
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0) ,u4AutoRefreshBak, REFCTRL0_REFDIS);

#if 0//Set to dynamic in ApplyAfterCalibration();
    // Restore CKE fix on setting
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), u4CKEFIXBak);

    // Restore HW MIOCK control setting
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), u4MIOCKBak, DRAMC_PD_CTRL_MIOCKCTRLOFF);
#endif

    /*TINFO="===  dram initial end (DramcModeRegInit_LP3)==="*/
    return DRAM_OK;
}
#endif //remove unused function to save space of log string

void MPLLInit()
{
#if (FOR_DV_SIMULATION_USED==0)
	unsigned int tmp;

	DRV_WriteReg32(AP_PLL_CON0, 0x85); // CLKSQ Enable
	mcDELAY_US(100);
	DRV_WriteReg32(AP_PLL_CON0, 0x87); // CLKSQ LPF Enable
	mcDELAY_MS(1);
	DRV_WriteReg32(MPLL_PWR_CON0, 0x3); // power on MPLL
	mcDELAY_US(1);
	DRV_WriteReg32(MPLL_PWR_CON0, 0x1); // turn off ISO of  MPLL
	mcDELAY_US(1);
	tmp = DRV_Reg32(MPLL_CON1);
	DRV_WriteReg32(MPLL_CON1, tmp | 0x80000000); // Config MPLL freq
	DRV_WriteReg32(MPLL_CON0, 0x181); // enable MPLL
	mcDELAY_US(20);
#endif
}

void DDRPhyFreqSel(DRAMC_CTX_T *p, DRAM_PLL_FREQ_SEL_T sel)
{
    p->freq_sel = sel;
#if SIMPLIFY_VREF_SCAN
    p->skipVrefScan = DISABLE; //SkipVrefScan is disabled by default, enable indvidually below
#endif

    switch(p->freq_sel)
    {
        case LP4_DDR3733:
            p->frequency=1866;
            p->dram_fsp = FSP_1;
            p->odt_onoff = ODT_ON;

            break;
        case LP4_DDR3200:
            p->frequency=1600;
            p->dram_fsp = FSP_1;
            p->odt_onoff = ODT_ON;
#if (ENABLE_DQ3200_UNTERM == 1)
            p->odt_onoff = ODT_OFF;
#endif

            break;
        case LP4_DDR2667:
            p->frequency=1333;
            p->dram_fsp = FSP_1;
            p->odt_onoff = ODT_ON;
#if (ENABLE_DQ3200_UNTERM == 1)
            p->odt_onoff = ODT_OFF;
#endif
#if SIMPLIFY_VREF_SCAN
            p->skipVrefScan = ENABLE;
#endif

            break;
        case LP4_DDR2400:
            p->frequency=1200;
            p->dram_fsp = FSP_0;
            p->odt_onoff = ODT_OFF;
            
            break;
        case LP4_DDR1600:
            p->frequency=800;
            p->dram_fsp = FSP_0;
            p->odt_onoff = ODT_OFF;
            
            break;
        case LP4_DDR800:
            p->frequency=400;
            p->dram_fsp = FSP_0;
            p->odt_onoff = ODT_OFF;

            break;
#if (__ETT__ || ENABLE_LP3_SW)
	    // scy: reduce code size by removing unused LPDDR3 opp
        case LP3_DDR1866:
            p->frequency=933;
            p->dram_fsp = FSP_0;
            p->odt_onoff = ODT_OFF;
            break;
        case LP3_DDR1600:
            p->frequency=800;
            p->dram_fsp = FSP_0;
            p->odt_onoff = ODT_OFF;
            break;
        case LP3_DDR1333:
            p->frequency=667;
            p->dram_fsp = FSP_0;
            p->odt_onoff = ODT_OFF;
            break;
        case LP3_DDR1200:
            p->frequency=600;
            p->dram_fsp = FSP_0;
            p->odt_onoff = ODT_OFF;
            break;
        case LP3_DDR933:
            p->frequency=467;
            p->dram_fsp = FSP_0;
            p->odt_onoff = ODT_OFF;
            break;
#endif
        default:
            p->frequency=800;
            p->dram_fsp = FSP_0;
            p->odt_onoff = ODT_OFF;
            break;
    }

    if(p->dram_type == TYPE_LPDDR4P)
        p->odt_onoff = ODT_OFF;

    vSetShuffleFrequency(p, DRAM_DFS_SHUFFLE_1, p->frequency);
    setFreqGroup(p); /* Set p->freqGroup to support freqs not in ACTimingTable */

    ///TODO: add DBI_onoff by condition
    //p->DBI_onoff = p->odt_onoff;
}

void vSetShuffleFrequency(DRAMC_CTX_T *p, DRAM_DFS_SHUFFLE_TYPE_T whitch_shuffle, U16 frequency)
{
      p->shuffle_frequency[whitch_shuffle] = frequency;  
}

#if ENABLE_RODT_TRACKING_SAVE_MCK
void SetTxWDQSStatusOnOff(U8 u1OnOff)
{
      u1WDQS_ON = u1OnOff;
}
#endif
static DRAM_STATUS_T UpdateInitialSettings_LP4(DRAMC_CTX_T *p)
{
    U16 u2RXVrefDefault=0x8;

    if(p->odt_onoff==ODT_ON)
    {
        vIO32WriteFldAlign(DRAMC_REG_SHU_ODTCTRL, 1, SHU_ODTCTRL_ROEN);
        vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ7, 0x1, SHU1_B0_DQ7_R_DMRODTEN_B0);
        vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ7, 0x1, SHU1_B1_DQ7_R_DMRODTEN_B1);
    }
    else
    {
        vIO32WriteFldAlign(DRAMC_REG_SHU_ODTCTRL, 0, SHU_ODTCTRL_ROEN);
        vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ7, 0x0, SHU1_B0_DQ7_R_DMRODTEN_B0);
        vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ7, 0x0, SHU1_B1_DQ7_R_DMRODTEN_B1);
    }

    //close RX DQ/DQS tracking to save power
    vIO32WriteFldMulti(DDRPHY_R0_B0_RXDVS2, P_Fld(0x0, R0_B0_RXDVS2_R_RK0_DVS_MODE_B0)
                | P_Fld(0x0, R0_B0_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B0)
                | P_Fld(0x0, R0_B0_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B0));
    vIO32WriteFldMulti(DDRPHY_R1_B0_RXDVS2, P_Fld(0x0, R1_B0_RXDVS2_R_RK1_DVS_MODE_B0)
                | P_Fld(0x0, R1_B0_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_B0)
                | P_Fld(0x0, R1_B0_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_B0));
    vIO32WriteFldMulti(DDRPHY_R0_B1_RXDVS2, P_Fld(0x0, R0_B1_RXDVS2_R_RK0_DVS_MODE_B1)
                | P_Fld(0x0, R0_B1_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B1)
                | P_Fld(0x0, R0_B1_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B1));
    vIO32WriteFldMulti(DDRPHY_R1_B1_RXDVS2, P_Fld(0x0, R1_B1_RXDVS2_R_RK1_DVS_MODE_B1)
                | P_Fld(0x0, R1_B1_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_B1)
                | P_Fld(0x0, R1_B1_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_B1));
    //wei-jen: RX rank_sel for CA is not used(Bianco), set it's dly to 0 to save power
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD7, 0, SHU1_CA_CMD7_R_DMRANKRXDVS_CA);


    //DDRPhyTxRxInitialSettings_LP4
    vIO32WriteFldAlign(DDRPHY_CA_CMD3, 0x1, CA_CMD3_RG_RX_ARCMD_STBENCMP_EN);

    vIO32WriteFldAlign(DDRPHY_CA_CMD10, 0x1, CA_CMD10_RG_RX_ARCLK_DQSIENMODE);

    vIO32WriteFldAlign(DDRPHY_CA_CMD6, 0x1, CA_CMD6_RG_RX_ARCMD_BIAS_VREF_SEL);

    vIO32WriteFldMulti(DDRPHY_B0_DQ3, P_Fld(0x1, B0_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B0)
                        | P_Fld(0x1, B0_DQ3_RG_RX_ARDQM_IN_BUFF_EN_B0)
                        | P_Fld(0x1, B0_DQ3_RG_RX_ARDQS_IN_BUFF_EN_B0));
    vIO32WriteFldMulti(DDRPHY_B1_DQ3, P_Fld(0x1, B1_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B1)
                        | P_Fld(0x1, B1_DQ3_RG_RX_ARDQM_IN_BUFF_EN_B1)
                        | P_Fld(0x1, B1_DQ3_RG_RX_ARDQS_IN_BUFF_EN_B1));
    vIO32WriteFldMulti(DDRPHY_CA_CMD3, P_Fld(0x1, CA_CMD3_RG_RX_ARCMD_IN_BUFF_EN)
                            | P_Fld(0x1, CA_CMD3_RG_RX_ARCLK_IN_BUFF_EN));

    vIO32WriteFldAlign(DDRPHY_B0_DQ3, 0x0, B0_DQ3_RG_RX_ARDQ_SMT_EN_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ3, 0x0, B1_DQ3_RG_RX_ARDQ_SMT_EN_B1);

    vIO32WriteFldAlign(DDRPHY_B0_DQ5, 0x1, B0_DQ5_RG_RX_ARDQS_DVS_EN_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ5, 0x1, B1_DQ5_RG_RX_ARDQS_DVS_EN_B1);
    vIO32WriteFldAlign(DDRPHY_CA_CMD5, 0x1, CA_CMD5_RG_RX_ARCLK_DVS_EN);

    //LP4 no need, follow LP3 first.
    //vIO32WriteFldAlign(DDRPHY_MISC_VREF_CTRL, P_Fld(0x1, MISC_VREF_CTRL_RG_RVREF_DDR3_SEL)
    //                                        | P_Fld(0x0, MISC_VREF_CTRL_RG_RVREF_DDR4_SEL));


    vIO32WriteFldMulti(DDRPHY_CA_CMD6, P_Fld(0x0, CA_CMD6_RG_TX_ARCMD_DDR3_SEL)
                                    | P_Fld(0x1, CA_CMD6_RG_TX_ARCMD_DDR4_SEL)
                                    | P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_DDR3_SEL)
                                    | P_Fld(0x1, CA_CMD6_RG_RX_ARCMD_DDR4_SEL));
    vIO32WriteFldMulti(DDRPHY_CA_CMD6, P_Fld(0x0, CA_CMD6_RG_TX_ARCMD_DDR3_SEL)
                                    | P_Fld(0x1, CA_CMD6_RG_TX_ARCMD_DDR4_SEL)
                                    | P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_DDR3_SEL)
                                    | P_Fld(0x1, CA_CMD6_RG_RX_ARCMD_DDR4_SEL));
    vIO32WriteFldMulti(DDRPHY_MISC_IMP_CTRL0, P_Fld(0x0, MISC_IMP_CTRL0_RG_RIMP_DDR3_SEL)
                                    | P_Fld(0x1, MISC_IMP_CTRL0_RG_RIMP_DDR4_SEL));


    vIO32WriteFldAlign(DDRPHY_B0_DQ6, 0x1, B0_DQ6_RG_RX_ARDQ_O1_SEL_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ6, 0x1, B1_DQ6_RG_RX_ARDQ_O1_SEL_B1);
    vIO32WriteFldAlign(DDRPHY_CA_CMD6, 0x1, CA_CMD6_RG_RX_ARCMD_O1_SEL);

    vIO32WriteFldAlign(DDRPHY_B0_DQ6, 0x1, B0_DQ6_RG_RX_ARDQ_BIAS_PS_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ6, 0x1, B1_DQ6_RG_RX_ARDQ_BIAS_PS_B1);
    vIO32WriteFldAlign(DDRPHY_CA_CMD6, 0x1, CA_CMD6_RG_RX_ARCMD_BIAS_PS);

    vIO32WriteFldAlign(DDRPHY_CA_CMD6, 0x1, CA_CMD6_RG_RX_ARCMD_RES_BIAS_EN);

    vIO32WriteFldAlign(DDRPHY_B0_DQ6, 0x1, B0_DQ6_RG_TX_ARDQ_ODTEN_EXT_DIS_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ6, 0x1, B1_DQ6_RG_TX_ARDQ_ODTEN_EXT_DIS_B1);
    vIO32WriteFldAlign(DDRPHY_CA_CMD6, 0x1, CA_CMD6_RG_TX_ARCMD_ODTEN_EXT_DIS);

    vIO32WriteFldAlign(DDRPHY_B0_DQ6, 0x1, B0_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ6, 0x1, B1_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B1);
    vIO32WriteFldAlign(DDRPHY_CA_CMD6, 0x1, CA_CMD6_RG_RX_ARCMD_RPRE_TOG_EN);


    if (p->dram_type == TYPE_LPDDR4)
    {
        if(p->odt_onoff==ODT_ON)
        {
            u2RXVrefDefault = 0xE;
        }
        else
        {
            u2RXVrefDefault = 0x16;
        }
    }
    else if (p->dram_type == TYPE_LPDDR4X)
    {
        if(p->odt_onoff==ODT_ON)
        {
        #if PU_CAL_05VDDQ
                u2RXVrefDefault = 0xb;
        #else
                u2RXVrefDefault = 0xb;
        #endif
        }
        else
        {
            u2RXVrefDefault = 0x16;
        }
    }
    else //if(p->dram_type == TYPE_LPDDR4P)
    {
        u2RXVrefDefault = 0x10;
    }

    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ5, u2RXVrefDefault, SHU1_B0_DQ5_RG_RX_ARDQ_VREF_SEL_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5, u2RXVrefDefault, SHU1_B1_DQ5_RG_RX_ARDQ_VREF_SEL_B1);
    vIO32WriteFldAlign(DDRPHY_B0_DQ5, u2RXVrefDefault, B0_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ5, u2RXVrefDefault, B1_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B1);

    if((p->dram_type == TYPE_LPDDR4X) ||(p->dram_type == TYPE_LPDDR4P))
    {
        // LP4x eye fine-tune
        vIO32WriteFldMulti_All(DDRPHY_B0_DQ8, P_Fld(0x1, B0_DQ8_RG_TX_ARDQ_EN_LP4P_B0)
                                            | P_Fld(0x1, B0_DQ8_RG_TX_ARDQ_EN_CAP_LP4P_B0)
                                            | P_Fld(0x1, B0_DQ8_RG_TX_ARDQ_CAP_DET_B0));
                                          //| P_Fld(0x1, B0_DQ8_RG_RX_ARDQS_DQSSTB_CG_EN_B0) // Field only exists for 10nm APHY
                                          //  corresponds to B0_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B0 for 16nm APHY
        vIO32WriteFldMulti_All(DDRPHY_B1_DQ8, P_Fld(0x1, B1_DQ8_RG_TX_ARDQ_EN_LP4P_B1)
                                            | P_Fld(0x1, B1_DQ8_RG_TX_ARDQ_EN_CAP_LP4P_B1)
                                            | P_Fld(0x1, B1_DQ8_RG_TX_ARDQ_CAP_DET_B1));
                                          //| P_Fld(0x1, B1_DQ8_RG_RX_ARDQS_DQSSTB_CG_EN_B1) // Field only exists for 10nm APHY
                                          //  corresponds to B1_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B1 for 16nm APHY
        vIO32WriteFldMulti_All(DDRPHY_CA_CMD9, P_Fld(0x1, CA_CMD9_RG_TX_ARCMD_EN_LP4P)
                                            | P_Fld(0x1, CA_CMD9_RG_TX_ARCMD_EN_CAP_LP4P)
                                            | P_Fld(0x1, CA_CMD9_RG_TX_ARCMD_CAP_DET));
    }
    else
    {
        vIO32WriteFldMulti_All(DDRPHY_B0_DQ8, P_Fld(0x0, B0_DQ8_RG_TX_ARDQ_EN_LP4P_B0)
                                            | P_Fld(0x0, B0_DQ8_RG_TX_ARDQ_EN_CAP_LP4P_B0)
                                            | P_Fld(0x0, B0_DQ8_RG_TX_ARDQ_CAP_DET_B0));
                                            //| P_Fld(0x1, B0_DQ8_RG_RX_ARDQS_DQSSTB_CG_EN_B0) // Field only exists for 10nm APHY
                                            //  corresponds to B0_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B0 for 16nm APHY
        vIO32WriteFldMulti_All(DDRPHY_B1_DQ8, P_Fld(0x0, B1_DQ8_RG_TX_ARDQ_EN_LP4P_B1)
                                            | P_Fld(0x0, B1_DQ8_RG_TX_ARDQ_EN_CAP_LP4P_B1)
                                            | P_Fld(0x0, B1_DQ8_RG_TX_ARDQ_CAP_DET_B1));
                                            //| P_Fld(0x1, B1_DQ8_RG_RX_ARDQS_DQSSTB_CG_EN_B1) // Field only exists for 10nm APHY
                                            //  corresponds to B1_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B1 for 16nm APHY
        vIO32WriteFldMulti_All(DDRPHY_CA_CMD9, P_Fld(0x0, CA_CMD9_RG_TX_ARCMD_EN_LP4P)
                                            | P_Fld(0x0, CA_CMD9_RG_TX_ARCMD_EN_CAP_LP4P)
                                            | P_Fld(0x0, CA_CMD9_RG_TX_ARCMD_CAP_DET));
    }

#if (fcFOR_CHIP_ID == fcBianco)
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ3, 0x20, SHU1_B0_DQ3_RG_ARDQ_REV_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ3, 0x20, SHU1_B1_DQ3_RG_ARDQ_REV_B1);
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD3, 0x09E0, SHU1_CA_CMD3_RG_ARCMD_REV);

    /* Set initial default mode to "new burst mode (7UI or new 8UI)" */
    DramcGatingMode(p, 1);
#endif

    vIO32WriteFldAlign(DDRPHY_CA_CMD8, 0x1, CA_CMD8_RG_TX_RRESETB_DDR3_SEL);
    vIO32WriteFldAlign(DDRPHY_CA_CMD8, 0x0, CA_CMD8_RG_TX_RRESETB_DDR4_SEL); //TODO: Remove if register default value is 0
    //End of DDRPhyTxRxInitialSettings_LP4

    //DFS workaround
    vIO32WriteFldAlign(DRAMC_REG_SHU_MISC, 0x2, SHU_MISC_REQQUE_MAXCNT);

    //should set 0x1f, otherwise AC-timing violation
    vIO32WriteFldAlign(DRAMC_REG_SHU1_DQSG, 0x1f, SHU1_DQSG_SCINTV);


    //Update setting for Bianco
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ5, 0x4, SHU1_B0_DQ5_RG_ARPI_FB_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5, 0x4, SHU1_B1_DQ5_RG_ARPI_FB_B1);
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD5, 0x6, SHU1_CA_CMD5_RG_ARPI_FB_CA);

    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ6, P_Fld(0x0, SHU1_B0_DQ6_RG_ARPI_OFFSET_DQSIEN_B0)
                                            | P_Fld(0x1, SHU1_B0_DQ6_RG_ARPI_RESERVE_B0_BIT0) // RG_*RPI_RESERVE_B0[0] 1'b1 prevent leakage
                                            | P_Fld(0, SHU1_B0_DQ6_RG_ARPI_RESERVE_B0_BIT1)); // RG_*RPI_RESERVE_B0[1] 1'b0 tracking leaf(slave)
    vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ6, P_Fld(0x0, SHU1_B1_DQ6_RG_ARPI_OFFSET_DQSIEN_B1)
                                            | P_Fld(0x1, SHU1_B1_DQ6_RG_ARPI_RESERVE_B1_BIT0) // RG_*RPI_RESERVE_B1[0] 1'b1 prevent leakage
                                            | P_Fld(0, SHU1_B1_DQ6_RG_ARPI_RESERVE_B1_BIT1)); // RG_*RPI_RESERVE_B1[1] 1'b0 tracking leaf(slave)

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD6, P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_OFFSET_CLKIEN)
                                            | P_Fld(0x1, SHU1_CA_CMD6_RG_ARPI_RESERVE_CA_BIT0) // RG_*RPI_RESERVE_CA[0] 1'b1 prevent leakage
                                            | P_Fld(0x1, SHU1_CA_CMD6_RG_ARPI_RESERVE_CA_BIT1)); // RG_*RPI_RESERVE_CA[1] 1'b1 tracking local(master)

    vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD6+(1<<POS_BANK_NUM), P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_OFFSET_CLKIEN)
                                                            | P_Fld(0x1, SHU1_CA_CMD6_RG_ARPI_RESERVE_CA_BIT0) // RG_*RPI_RESERVE_CA[0] 1'b1 prevent leakage
                                                            | P_Fld(0, SHU1_CA_CMD6_RG_ARPI_RESERVE_CA_BIT1)); // RG_*RPI_RESERVE_CA[1] 1'b0 tracking leaf(slave)
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);

    //IMP Tracking Init Settings
    //Write (DRAMC _BASE+ 0x219) [31:0] = 32'h80080020//DDR3200 default
    vIO32WriteFldMulti(DRAMC_REG_SHU_IMPCAL1, P_Fld(8, SHU_IMPCAL1_IMPCAL_CALICNT) | P_Fld(0x10, SHU_IMPCAL1_IMPCALCNT) | P_Fld(4, SHU_IMPCAL1_IMPCAL_CALEN_CYCLE) | P_Fld(4, SHU_IMPCAL1_IMPCAL_CHKCYCLE));

#if SUPPORT_HYNIX_RX_DQS_WEAK_PULL
    #if 1//DQS pull down, DQSB pull up
    if (p->vendor_id == VENDOR_HYNIX)
    {
        vIO32WriteFldMulti(DDRPHY_B0_DQ7,
                                           P_Fld(0x0, B0_DQ7_RG_TX_ARDQS_PULL_UP_B0)
                                        | P_Fld(0x1, B0_DQ7_RG_TX_ARDQS_PULL_DN_B0)
                                        | P_Fld(0x1, B0_DQ7_RG_TX_ARDQSB_PULL_UP_B0)
                                        | P_Fld(0x0, B0_DQ7_RG_TX_ARDQSB_PULL_DN_B0));

        vIO32WriteFldMulti(DDRPHY_B1_DQ7,
                                           P_Fld(0x0, B1_DQ7_RG_TX_ARDQS0_PULL_UP_B1)
                                        | P_Fld(0x1, B1_DQ7_RG_TX_ARDQS0_PULL_DN_B1)
                                        | P_Fld(0x1, B1_DQ7_RG_TX_ARDQS0B_PULL_UP_B1)
                                        | P_Fld(0x0, B1_DQ7_RG_TX_ARDQS0B_PULL_DN_B1));
    }
    #endif
#endif

    //for _K_
    vIO32WriteFldMulti(DRAMC_REG_SREFCTRL, P_Fld(0x1, SREFCTRL_SCSM_CGAR)
                                            | P_Fld(0x1, SREFCTRL_SCARB_SM_CGAR)
                                            | P_Fld(0x1, SREFCTRL_RDDQSOSC_CGAR)
                                            | P_Fld(0x1, SREFCTRL_HMRRSEL_CGAR));
    vIO32WriteFldAlign(DRAMC_REG_PRE_TDQSCK1, 0x1, PRE_TDQSCK1_TXUIPI_CAL_CGAR);
    /* DVFS related, PREA interval counter (After DVFS DVT, set to 0xf (originally was 0x1f)) */
    vIO32WriteFldAlign(DRAMC_REG_SHU_MISC, 0xf, SHU_MISC_PREA_INTV);
    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ8, P_Fld(0x1, SHU1_B0_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_R_DMRANK_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_R_DMSTBEN_SYNC_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_R_DMRXDLY_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B0)
                                        | P_Fld(0x0, SHU1_B0_DQ8_R_DMRXDVS_UPD_FORCE_EN_B0)
                                        | P_Fld(0x7fff, SHU1_B0_DQ8_R_DMRXDVS_UPD_FORCE_CYC_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ8, P_Fld(0x1, SHU1_B1_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_R_DMRANK_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_R_DMSTBEN_SYNC_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_R_DMRXDLY_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B1)
                                        | P_Fld(0x0, SHU1_B1_DQ8_R_DMRXDVS_UPD_FORCE_EN_B1)
                                        | P_Fld(0x7fff, SHU1_B1_DQ8_R_DMRXDVS_UPD_FORCE_CYC_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD8, P_Fld(0x1, SHU1_CA_CMD8_R_DMRANK_CHG_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_R_DMRANK_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_SYNC_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_R_DMSTBEN_SYNC_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_R_DMRXDLY_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_PIPE_CG_IG_CA)
                                        | P_Fld(0x0, SHU1_CA_CMD8_R_DMRXDVS_UPD_FORCE_EN_CA)
                                        | P_Fld(0x7fff, SHU1_CA_CMD8_R_DMRXDVS_UPD_FORCE_CYC_CA));
    vIO32WriteFldAlign(DDRPHY_MISC_CTRL3, 0x1, MISC_CTRL3_R_DDRPHY_COMB_CG_IG);
    /* Bianco HW design issue: run-time PBYTE (B0, B1) flags will lose it's function and become per-bit -> set to 0 */
    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7, P_Fld(0x0, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0)
                                        | P_Fld(0x0, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0)
                                        | P_Fld(0x0, SHU1_B0_DQ7_R_DMRXDVS_DQM_FLAGSEL_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ7, P_Fld(0x0, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1)
                                        | P_Fld(0x0, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1)
                                        | P_Fld(0x0, SHU1_B1_DQ7_R_DMRXDVS_DQM_FLAGSEL_B1));

    vIO32WriteFldMulti(DRAMC_REG_CLKAR, P_Fld(0x1, CLKAR_SELPH_CMD_CG_DIS) | P_Fld(0x7FFF, CLKAR_REQQUE_PACG_DIS));


    vIO32WriteFldAlign(DRAMC_REG_SHU_DQSG_RETRY, 0x0, SHU_DQSG_RETRY_R_RETRY_PA_DSIABLE);
    vIO32WriteFldAlign(DRAMC_REG_WRITE_LEV, 0x0, WRITE_LEV_DDRPHY_COMB_CG_SEL);
    vIO32WriteFldAlign(DRAMC_REG_DUMMY_RD, 0x1, DUMMY_RD_DUMMY_RD_PA_OPT);
    vIO32WriteFldMulti(DRAMC_REG_STBCAL2, P_Fld(0x0, STBCAL2_STB_UIDLYCG_IG)
                                            | P_Fld(0x0, STBCAL2_STB_PIDLYCG_IG));
    vIO32WriteFldMulti(DRAMC_REG_EYESCAN, P_Fld(0x1, EYESCAN_EYESCAN_DQS_SYNC_EN)
                                            | P_Fld(0x1, EYESCAN_EYESCAN_NEW_DQ_SYNC_EN)
                                            | P_Fld(0x1, EYESCAN_EYESCAN_DQ_SYNC_EN));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ODTCTRL, P_Fld(0x1, SHU_ODTCTRL_RODTENSTB_SELPH_CG_IG)
                                            | P_Fld(0x1, SHU_ODTCTRL_RODTEN_SELPH_CG_IG));

    #if 1//#ifndef BIANCO_TO_BE_PORTING
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DLL0, 0x1, SHU1_B0_DLL0_RG_ARPISM_MCK_SEL_B0_SHU);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DLL0, 0x1, SHU1_B1_DLL0_RG_ARPISM_MCK_SEL_B1_SHU);
    //vIO32WriteFldAlign(DDRPHY_SHU1_CA_DLL0, 0x1, SHU1_CA_DLL0_RG_ARPISM_MCK_SEL_CA_SHU); move to DramcSetting_Olympus_LP4_ByteMode()
    
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI1, 0x1, CA_DLL_ARPI1_RG_ARPISM_MCK_SEL_CA);
    #endif
    //end _K_

    //DE_UPDATE
    vIO32WriteFldMulti(DRAMC_REG_PERFCTL0, P_Fld(0x1, PERFCTL0_WRFIFO_OPT)
                                        | P_Fld(0x1, PERFCTL0_REORDEREN)
                                        | P_Fld(0x1, PERFCTL0_RWSPLIT));
    vIO32WriteFldAlign(DRAMC_REG_SREFCTRL, 0x1, SREFCTRL_SREF2_OPTION);
    vIO32WriteFldAlign(DRAMC_REG_SHUCTRL1, 0x1a, SHUCTRL1_FC_PRDCNT);

#ifdef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
    vIO32WriteFldMulti(DDRPHY_B0_DQ6, P_Fld(0x1, B0_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B0)
                                | P_Fld(0x1, B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0));
    vIO32WriteFldMulti(DDRPHY_B1_DQ6, P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B1)
                                | P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1));
    vIO32WriteFldMulti(DDRPHY_CA_CMD6, P_Fld(0x1, CA_CMD6_RG_RX_ARCMD_OP_BIAS_SW_EN)
                                    | P_Fld(0x1, CA_CMD6_RG_RX_ARCMD_BIAS_EN));
    vIO32WriteFldAlign(DRAMC_REG_STBCAL2, 0x1, STBCAL2_STB_PICG_EARLY_1T_EN);
    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7, P_Fld(0x0, SHU1_B0_DQ7_R_DMRXRANK_DQS_LAT_B0)
                                    | P_Fld(0x1, SHU1_B0_DQ7_R_DMRXRANK_DQS_EN_B0)
                                    | P_Fld(0x1, SHU1_B0_DQ7_R_DMRXRANK_DQ_LAT_B0)
                                    | P_Fld(0x1, SHU1_B0_DQ7_R_DMRXRANK_DQ_EN_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ7, P_Fld(0x0, SHU1_B1_DQ7_R_DMRXRANK_DQS_LAT_B1)
                                    | P_Fld(0x1, SHU1_B1_DQ7_R_DMRXRANK_DQS_EN_B1)
                                    | P_Fld(0x1, SHU1_B1_DQ7_R_DMRXRANK_DQ_LAT_B1)
                                    | P_Fld(0x1, SHU1_B1_DQ7_R_DMRXRANK_DQ_EN_B1));
#else
    vIO32WriteFldMulti(DDRPHY_B0_DQ6, P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B0)
                                | P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0));
    vIO32WriteFldMulti(DDRPHY_B1_DQ6, P_Fld(0x0, B1_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B1)
                                | P_Fld(0x0, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1));
    vIO32WriteFldMulti(DDRPHY_CA_CMD6, P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_OP_BIAS_SW_EN)
                                    | P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_BIAS_EN));
#endif

    //end DE UPDATE

    //Disable RODT tracking
    vIO32WriteFldAlign(DRAMC_REG_SHU_RODTENSTB, 0, SHU_RODTENSTB_RODTEN_MCK_MODESEL);

    //Rx Gating tracking settings
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DQSG), \
                                                P_Fld(9, SHU1_DQSG_STB_UPDMASKCYC) | \
                                                P_Fld(1, SHU1_DQSG_STB_UPDMASK_EN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQSCAL, P_Fld(0, SHURK0_DQSCAL_R0DQSIENLLMTEN) | P_Fld(0, SHURK0_DQSCAL_R0DQSIENHLMTEN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQSCAL, P_Fld(0, SHURK1_DQSCAL_R1DQSIENLLMTEN) | P_Fld(0, SHURK1_DQSCAL_R1DQSIENHLMTEN));
    vIO32WriteFldMulti(DRAMC_REG_SHU_STBCAL, P_Fld(1, SHU_STBCAL_DQSG_MODE) | P_Fld(1, SHU_STBCAL_PICGLAT));

#ifdef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
    vIO32WriteFldAlign(DDRPHY_B0_DQ9, 0x4, B0_DQ9_R_IN_GATE_EN_LOW_OPT_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ9, 0x4, B1_DQ9_R_IN_GATE_EN_LOW_OPT_B1);
#else
    //Modify for corner IC failed at HQA test XTLV
    vIO32WriteFldAlign(DDRPHY_B0_DQ9, 0x7, B0_DQ9_R_IN_GATE_EN_LOW_OPT_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ9, 0x7, B1_DQ9_R_IN_GATE_EN_LOW_OPT_B1);
#endif
    vIO32WriteFldAlign(DDRPHY_CA_CMD10, 0x0, CA_CMD10_R_IN_GATE_EN_LOW_OPT_CA);

    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ8, 0x1, SHU1_B0_DQ8_R_DMRXDLY_CG_IG_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ8, 0x1, SHU1_B1_DQ8_R_DMRXDLY_CG_IG_B1);

#ifdef DUMMY_READ_FOR_DQS_GATING_RETRY
    if(p->support_rank_num == RANK_SINGLE)
    {    
        vIO32WriteFldAlign(DRAMC_REG_SHU_DQSG_RETRY, 1, SHU_DQSG_RETRY_R_RETRY_1RANK);
    }    
#endif 

    
    if(p->vendor_id==VENDOR_HYNIX)
    {
        #if ENABLE_TX_WDQS
        mcSHOW_DBG_MSG(("Enable WDQS\n"));
        vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ3, P_Fld(1, SHU1_B0_DQ3_DQ_REV_B0_BIT_09_READ_BASE_EN)
                                            | P_Fld(!p->odt_onoff, SHU1_B0_DQ3_DQ_REV_B0_BIT_08_ODT_DISABLE)
                                            | P_Fld(0, SHU1_B0_DQ3_DQ_REV_B0_BIT_01_DQS_PULL_DN)
                                            | P_Fld(0, SHU1_B0_DQ3_DQ_REV_B0_BIT_00_DQSB_PULL_UP));
        vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ3, P_Fld(1, SHU1_B1_DQ3_DQ_REV_B1_BIT_09_READ_BASE_EN)
                                            | P_Fld(!p->odt_onoff, SHU1_B1_DQ3_DQ_REV_B1_BIT_08_ODT_DISABLE)
                                            | P_Fld(0, SHU1_B1_DQ3_DQ_REV_B1_BIT_01_DQS_PULL_DN)
                                            | P_Fld(0, SHU1_B1_DQ3_DQ_REV_B1_BIT_00_DQSB_PULL_UP));
        
        vIO32WriteFldMulti(DRAMC_REG_SHU_ODTCTRL, P_Fld(0x1, SHU_ODTCTRL_RODTE) 
                | P_Fld(0x1, SHU_ODTCTRL_RODTE2) 
                | P_Fld(0x1, SHU_ODTCTRL_ROEN));
                
        vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ7, 0x1, SHU1_B0_DQ7_R_DMRODTEN_B0);
        vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ7, 0x1, SHU1_B1_DQ7_R_DMRODTEN_B1);
#if ENABLE_RODT_TRACKING_SAVE_MCK
        SetTxWDQSStatusOnOff(1);
#endif
        
        #elif SUPPORT_HYNIX_RX_DQS_WEAK_PULL
        mcSHOW_DBG_MSG(("Enable WEAK PULL\n"));
        vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ3, P_Fld(0, SHU1_B0_DQ3_DQ_REV_B0_BIT_09_READ_BASE_EN)
                                            | P_Fld(0, SHU1_B0_DQ3_DQ_REV_B0_BIT_08_ODT_DISABLE)
                                            | P_Fld(1, SHU1_B0_DQ3_DQ_REV_B0_BIT_01_DQS_PULL_DN)
                                            | P_Fld(1, SHU1_B0_DQ3_DQ_REV_B0_BIT_00_DQSB_PULL_UP));
        vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ3, P_Fld(0, SHU1_B1_DQ3_DQ_REV_B1_BIT_09_READ_BASE_EN)
                                            | P_Fld(0, SHU1_B1_DQ3_DQ_REV_B1_BIT_08_ODT_DISABLE)
                                            | P_Fld(1, SHU1_B1_DQ3_DQ_REV_B1_BIT_01_DQS_PULL_DN)
                                            | P_Fld(1, SHU1_B1_DQ3_DQ_REV_B1_BIT_00_DQSB_PULL_UP));
        #else
        vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ3, P_Fld(0, SHU1_B0_DQ3_DQ_REV_B0_BIT_09_READ_BASE_EN)
                                            | P_Fld(0, SHU1_B0_DQ3_DQ_REV_B0_BIT_08_ODT_DISABLE)
                                            | P_Fld(0, SHU1_B0_DQ3_DQ_REV_B0_BIT_01_DQS_PULL_DN)
                                            | P_Fld(0, SHU1_B0_DQ3_DQ_REV_B0_BIT_00_DQSB_PULL_UP));
        vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ3, P_Fld(0, SHU1_B1_DQ3_DQ_REV_B1_BIT_09_READ_BASE_EN)
                                            | P_Fld(0, SHU1_B1_DQ3_DQ_REV_B1_BIT_08_ODT_DISABLE)
                                            | P_Fld(0, SHU1_B1_DQ3_DQ_REV_B1_BIT_01_DQS_PULL_DN)
                                            | P_Fld(0, SHU1_B1_DQ3_DQ_REV_B1_BIT_00_DQSB_PULL_UP));
        #endif
    }
    else
    {        
        vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ3, P_Fld(0, SHU1_B0_DQ3_DQ_REV_B0_BIT_09_READ_BASE_EN)
                                            | P_Fld(0, SHU1_B0_DQ3_DQ_REV_B0_BIT_08_ODT_DISABLE)
                                            | P_Fld(0, SHU1_B0_DQ3_DQ_REV_B0_BIT_01_DQS_PULL_DN)
                                            | P_Fld(0, SHU1_B0_DQ3_DQ_REV_B0_BIT_00_DQSB_PULL_UP));
        vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ3, P_Fld(0, SHU1_B1_DQ3_DQ_REV_B1_BIT_09_READ_BASE_EN)
                                            | P_Fld(0, SHU1_B1_DQ3_DQ_REV_B1_BIT_08_ODT_DISABLE)
                                            | P_Fld(0, SHU1_B1_DQ3_DQ_REV_B1_BIT_01_DQS_PULL_DN)
                                            | P_Fld(0, SHU1_B1_DQ3_DQ_REV_B1_BIT_00_DQSB_PULL_UP));
    }

    //DE review WhitneyE2
    vIO32WriteFldAlign(DRAMC_REG_DRSCTRL, 0x1, DRSCTRL_DRS_SELFWAKE_DMYRD_DIS);
    vIO32WriteFldAlign(DRAMC_REG_REFCTRL0, 0x1, REFCTRL0_REFNA_OPT); 
    vIO32WriteFldAlign(DRAMC_REG_ZQCS, 0x1, ZQCS_ZQCS_MASK_SEL_CGAR);
    vIO32WriteFldMulti(DRAMC_REG_DUMMY_RD, P_Fld(0x1,DUMMY_RD_DMYRD_REORDER_DIS) | P_Fld(0x1,DUMMY_RD_DMYRD_HPRI_DIS));
    vIO32WriteFldAlign(DRAMC_REG_SHUCTRL2, 0x1, SHUCTRL2_R_DVFS_SREF_OPT);
    vIO32WriteFldAlign(DRAMC_REG_SHUCTRL3, 0xb, SHUCTRL3_VRCGDIS_PRDCNT);    
    vIO32WriteFldAlign(DDRPHY_MISC_CTRL3, 0x1, MISC_CTRL3_R_DDRPHY_RX_PIPE_CG_IG);
    //End

    //DE review Bianco
    /* ARPISM_MCK_SEL_B0, B1 set to 1 (Joe): "Due to TX_PICG modify register is set to 1, 
     * ARPISM_MCK_SEL_Bx should be 1 to fulfill APHY TX OE spec for low freq (Ex: DDR1600)"
     */
    vIO32WriteFldMulti(DDRPHY_B0_DLL_ARPI1, P_Fld(0x1, B0_DLL_ARPI1_RG_ARPISM_MCK_SEL_B0_REG_OPT)
                                        | P_Fld(0x1, B0_DLL_ARPI1_RG_ARPISM_MCK_SEL_B0));
    vIO32WriteFldMulti(DDRPHY_B1_DLL_ARPI1, P_Fld(0x1, B1_DLL_ARPI1_RG_ARPISM_MCK_SEL_B1_REG_OPT)
                                        | P_Fld(0x1, B1_DLL_ARPI1_RG_ARPISM_MCK_SEL_B1));
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI1, 0x1, CA_DLL_ARPI1_RG_ARPISM_MCK_SEL_CA_REG_OPT);
    vIO32WriteFldAlign(DDRPHY_MISC_CTRL0, 0, MISC_CTRL0_R_DMSHU_PHYDCM_FORCEOFF);
    vIO32WriteFldAlign(DDRPHY_MISC_RXDVS2, 1, MISC_RXDVS2_R_DMRXDVS_SHUFFLE_CTRL_CG_IG);
    vIO32WriteFldAlign(DRAMC_REG_CLKCTRL, 0x1, CLKCTRL_SEQCLKRUN3);
    vIO32WriteFldAlign(DRAMC_REG_REFCTRL1, 1, REFCTRL1_SREF_CG_OPT);
    vIO32WriteFldMulti(DRAMC_REG_SHUCTRL, P_Fld(0x1, SHUCTRL_DVFS_CG_OPT) | P_Fld(0x3, SHUCTRL_R_DVFS_PICG_MARGIN2) | P_Fld(0x3, SHUCTRL_R_DVFS_PICG_MARGIN3));
    vIO32WriteFldMulti(DRAMC_REG_SHUCTRL2, P_Fld(0x1, SHUCTRL2_SHORTQ_OPT) | P_Fld(0x3, SHUCTRL2_R_DVFS_PICG_MARGIN));
    vIO32WriteFldAlign(DRAMC_REG_STBCAL2, 0x0, STBCAL2_STB_DBG_EN);
    vIO32WriteFldMulti(DRAMC_REG_PRE_TDQSCK1, P_Fld(0x0, PRE_TDQSCK1_APHY_CG_OPT1) | P_Fld(0x0, PRE_TDQSCK1_SHU_PRELOAD_TX_HW));

    if(u2DFSGetHighestFreq(p) >= 1866)
    {
        //if product supports 3733, CLKAR_SELPH_4LCG_DIS always 1 else o, but if 1, comsume more power
        vIO32WriteFldAlign(DRAMC_REG_CLKAR, 1, CLKAR_SELPH_4LCG_DIS);
    }
    
    return DRAM_OK;
}


#if ENABLE_LP3_SW
static DRAM_STATUS_T UpdateInitialSettings_LP3(DRAMC_CTX_T *p)
{

    //close RX DQ/DQS tracking to save power
    vIO32WriteFldMulti(DDRPHY_R0_B1_RXDVS2, P_Fld(0x0, R0_B1_RXDVS2_R_RK0_DVS_MODE_B1)
                | P_Fld(0x0, R0_B1_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B1)
                | P_Fld(0x0, R0_B1_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B1));
    vIO32WriteFldMulti(DDRPHY_R1_B1_RXDVS2, P_Fld(0x0, R1_B1_RXDVS2_R_RK1_DVS_MODE_B1)
                | P_Fld(0x0, R1_B1_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_B1)
                | P_Fld(0x0, R1_B1_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_B1));
    vIO32WriteFldMulti(DDRPHY_R0_B0_RXDVS2+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x0, R0_B0_RXDVS2_R_RK0_DVS_MODE_B0)
                | P_Fld(0x0, R0_B0_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B0)
                | P_Fld(0x0, R0_B0_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B0));
    vIO32WriteFldMulti(DDRPHY_R1_B0_RXDVS2+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x0, R1_B0_RXDVS2_R_RK1_DVS_MODE_B0)
                | P_Fld(0x0, R1_B0_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_B0)
                | P_Fld(0x0, R1_B0_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_B0));
    vIO32WriteFldMulti(DDRPHY_R0_CA_RXDVS2+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x0, R0_CA_RXDVS2_R_RK0_DVS_MODE_CA)
                | P_Fld(0x0, R0_CA_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_CA)
                | P_Fld(0x0, R0_CA_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_CA));
    vIO32WriteFldMulti(DDRPHY_R1_CA_RXDVS2+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x0, R1_CA_RXDVS2_R_RK1_DVS_MODE_CA)
                | P_Fld(0x0, R1_CA_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_CA)
                | P_Fld(0x0, R1_CA_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_CA));
    vIO32WriteFldMulti(DDRPHY_R0_B1_RXDVS2+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x0, R0_B1_RXDVS2_R_RK0_DVS_MODE_B1)
                | P_Fld(0x0, R0_B1_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B1)
                | P_Fld(0x0, R0_B1_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B1));
    vIO32WriteFldMulti(DDRPHY_R1_B1_RXDVS2+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x0, R1_B1_RXDVS2_R_RK1_DVS_MODE_B1)
                | P_Fld(0x0, R1_B1_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_B1)
                | P_Fld(0x0, R1_B1_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_B1));


    //DDRPhyTxRxInitialSettings_LP3
    vIO32WriteFldMulti_All(DDRPHY_B0_DQ6, P_Fld(0x1, B0_DQ6_RG_TX_ARDQ_DDR3_SEL_B0)
                                        | P_Fld(0x1, B0_DQ6_RG_RX_ARDQ_DDR3_SEL_B0)
                                        | P_Fld(0x0, B0_DQ6_RG_TX_ARDQ_DDR4_SEL_B0)
                                        | P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_DDR4_SEL_B0));
    vIO32WriteFldMulti_All(DDRPHY_B1_DQ6, P_Fld(0x1, B1_DQ6_RG_TX_ARDQ_DDR3_SEL_B1)
                                        | P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_DDR3_SEL_B1)
                                        | P_Fld(0x0, B1_DQ6_RG_TX_ARDQ_DDR4_SEL_B1)
                                        | P_Fld(0x0, B1_DQ6_RG_RX_ARDQ_DDR4_SEL_B1));
    vIO32WriteFldMulti_All(DDRPHY_CA_CMD6, P_Fld(0x1, CA_CMD6_RG_TX_ARCMD_DDR3_SEL)
                                        | P_Fld(0x1, CA_CMD6_RG_RX_ARCMD_DDR3_SEL)
                                        | P_Fld(0x0, CA_CMD6_RG_TX_ARCMD_DDR4_SEL)
                                        | P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_DDR4_SEL));

    vIO32WriteFldAlign_All(DDRPHY_B0_DQ8, 0x0, B0_DQ8_RG_TX_ARDQ_EN_LP4P_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DQ8, 0x0, B1_DQ8_RG_TX_ARDQ_EN_LP4P_B1);
    vIO32WriteFldAlign_All(DDRPHY_CA_CMD9, 0x0, CA_CMD9_RG_TX_ARCMD_EN_LP4P);

    vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ5), 1, B0_DQ5_RG_RX_ARDQ_EYE_VREF_EN_B0);
    vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ5), 1, B1_DQ5_RG_RX_ARDQ_EYE_VREF_EN_B1);
    vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_CA_CMD5), 1, CA_CMD5_RG_RX_ARCMD_EYE_VREF_EN);

    vIO32WriteFldMulti_All(DDRPHY_CA_CMD8, P_Fld(0x1, CA_CMD8_RG_TX_RRESETB_DDR3_SEL)
                                         | P_Fld(0x0, CA_CMD8_RG_TX_RRESETB_DDR4_SEL)); //TODO: Remove if register default value is 0

    vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ5), 0xb, B0_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B0);
    vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ5), 0xb, B1_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B1);
    vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_CA_CMD5), 0xb, CA_CMD5_RG_RX_ARCMD_EYE_VREF_SEL);

    vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ5), 0xb, SHU1_B0_DQ5_RG_RX_ARDQ_VREF_SEL_B0);
    vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DQ5), 0xb, SHU1_B1_DQ5_RG_RX_ARDQ_VREF_SEL_B1);        
    vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_SHU1_CA_CMD5), 0xb, SHU1_CA_CMD5_RG_RX_ARCMD_VREF_SEL); 
    
    vIO32WriteFldAlign_All(DDRPHY_B0_DQ6, 0x1, B0_DQ6_RG_RX_ARDQ_RES_BIAS_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DQ6, 0x1, B1_DQ6_RG_RX_ARDQ_RES_BIAS_EN_B1);
    vIO32WriteFldAlign_All(DDRPHY_CA_CMD6, 0x1, CA_CMD6_RG_RX_ARCMD_RES_BIAS_EN);

    vIO32WriteFldAlign_Phy_All(DDRPHY_B0_DQ6, 0x1, B0_DQ6_RG_RX_ARDQ_O1_SEL_B0);
    vIO32WriteFldAlign_Phy_All(DDRPHY_B1_DQ6, 0x1, B1_DQ6_RG_RX_ARDQ_O1_SEL_B1);
    vIO32WriteFldAlign_Phy_All(DDRPHY_CA_CMD6, 0x1, CA_CMD6_RG_RX_ARCMD_O1_SEL);

    vIO32WriteFldAlign_Phy_All(DDRPHY_B0_DQ6, 0x1, B0_DQ6_RG_TX_ARDQ_ODTEN_EXT_DIS_B0);
    vIO32WriteFldAlign_Phy_All(DDRPHY_B1_DQ6, 0x1, B1_DQ6_RG_TX_ARDQ_ODTEN_EXT_DIS_B1);
    vIO32WriteFldAlign_Phy_All(DDRPHY_CA_CMD6, 0x1, CA_CMD6_RG_TX_ARCMD_ODTEN_EXT_DIS);

    //CH_A B0/CA = 0
    vIO32WriteFldMulti(DDRPHY_B0_DQ3, P_Fld(0x0, B0_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B0)
                                        | P_Fld(0x0, B0_DQ3_RG_RX_ARDQM_IN_BUFF_EN_B0)
                                        | P_Fld(0x0, B0_DQ3_RG_RX_ARDQS_IN_BUFF_EN_B0));
    vIO32WriteFldMulti(DDRPHY_B0_DQ3+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, B0_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B0)
                                        | P_Fld(0x1, B0_DQ3_RG_RX_ARDQM_IN_BUFF_EN_B0)
                                        | P_Fld(0x1, B0_DQ3_RG_RX_ARDQS_IN_BUFF_EN_B0));

    vIO32WriteFldMulti_All(DDRPHY_B1_DQ3, P_Fld(0x1, B1_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B1)
                                        | P_Fld(0x1, B1_DQ3_RG_RX_ARDQM_IN_BUFF_EN_B1)
                                        | P_Fld(0x1, B1_DQ3_RG_RX_ARDQS_IN_BUFF_EN_B1));

    vIO32WriteFldMulti(DDRPHY_CA_CMD3, P_Fld(0x0, CA_CMD3_RG_RX_ARCMD_IN_BUFF_EN)
                                         | P_Fld(0x0, CA_CMD3_RG_RX_ARCLK_IN_BUFF_EN));
    vIO32WriteFldMulti(DDRPHY_CA_CMD3+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, CA_CMD3_RG_RX_ARCMD_IN_BUFF_EN)
                                         | P_Fld(0x1, CA_CMD3_RG_RX_ARCLK_IN_BUFF_EN));

    vIO32WriteFldAlign(DDRPHY_B0_DQ9, 0x8e, B0_DQ9_R_IN_GATE_EN_LOW_OPT_B0);
    vIO32WriteFldAlign(DDRPHY_CA_CMD10, 0x8e, CA_CMD10_R_IN_GATE_EN_LOW_OPT_CA);
    
#ifdef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
    vIO32WriteFldAlign(DDRPHY_B1_DQ9, 0x04, B1_DQ9_R_IN_GATE_EN_LOW_OPT_B1);
    vIO32WriteFldAlign(DDRPHY_B0_DQ9+((U32)CHANNEL_B<<POS_BANK_NUM), 0x04, B0_DQ9_R_IN_GATE_EN_LOW_OPT_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ9+((U32)CHANNEL_B<<POS_BANK_NUM), 0x04, B1_DQ9_R_IN_GATE_EN_LOW_OPT_B1);
    vIO32WriteFldAlign(DDRPHY_CA_CMD10+((U32)CHANNEL_B<<POS_BANK_NUM), 0x04, CA_CMD10_R_IN_GATE_EN_LOW_OPT_CA);
#else
    vIO32WriteFldAlign(DDRPHY_B1_DQ9, 0x8e, B1_DQ9_R_IN_GATE_EN_LOW_OPT_B1);
    vIO32WriteFldAlign(DDRPHY_B0_DQ9+((U32)CHANNEL_B<<POS_BANK_NUM), 0x8e, B0_DQ9_R_IN_GATE_EN_LOW_OPT_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ9+((U32)CHANNEL_B<<POS_BANK_NUM), 0x8e, B1_DQ9_R_IN_GATE_EN_LOW_OPT_B1);
    vIO32WriteFldAlign(DDRPHY_CA_CMD10+((U32)CHANNEL_B<<POS_BANK_NUM), 0x8e, CA_CMD10_R_IN_GATE_EN_LOW_OPT_CA);
#endif
    //End of DDRPhyTxRxInitialSettings_LP3

    //DFS workaround
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_MISC, 0x2, SHU_MISC_REQQUE_MAXCNT);

    //should set 0x1f, otherwise AC-timing violation
    vIO32WriteFldAlign_All(DRAMC_REG_SHU1_DQSG, 0x1f, SHU1_DQSG_SCINTV);

    //for _K_
    vIO32WriteFldMulti(DRAMC_REG_SREFCTRL, P_Fld(0x1, SREFCTRL_SCSM_CGAR)
                                            | P_Fld(0x1, SREFCTRL_SCARB_SM_CGAR)
                                            | P_Fld(0x1, SREFCTRL_RDDQSOSC_CGAR)
                                            | P_Fld(0x1, SREFCTRL_HMRRSEL_CGAR));
    vIO32WriteFldAlign(DRAMC_REG_PRE_TDQSCK1, 0x1, PRE_TDQSCK1_TXUIPI_CAL_CGAR);
    /* DVFS related, PREA interval counter (After DVFS DVT, set to 0xf (originally was 0x1f)) */
    vIO32WriteFldAlign(DRAMC_REG_SHU_MISC, 0xf, SHU_MISC_PREA_INTV);
    vIO32WriteFldAlign(DRAMC_REG_DVFSDLL, 0x0, DVFSDLL_R_BYPASS_1ST_DLL_SHU1);
    vIO32WriteFldAlign(DRAMC_REG_DVFSDLL, 0x0, DVFSDLL_R_BYPASS_1ST_DLL_SHU2);
    vIO32WriteFldAlign(DRAMC_REG_DVFSDLL, 0x0, DVFSDLL_R_BYPASS_1ST_DLL_SHU3);
    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ8, P_Fld(0x1, SHU1_B0_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_R_DMRANK_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_R_DMSTBEN_SYNC_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_R_DMRXDLY_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B0)
                                        | P_Fld(0x0, SHU1_B0_DQ8_R_DMRXDVS_UPD_FORCE_EN_B0)
                                        | P_Fld(0x7fff, SHU1_B0_DQ8_R_DMRXDVS_UPD_FORCE_CYC_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ8+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, SHU1_B0_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_R_DMRANK_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_R_DMSTBEN_SYNC_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_R_DMRXDLY_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B0)
                                        | P_Fld(0x0, SHU1_B0_DQ8_R_DMRXDVS_UPD_FORCE_EN_B0)
                                        | P_Fld(0x7fff, SHU1_B0_DQ8_R_DMRXDVS_UPD_FORCE_CYC_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ8, P_Fld(0x1, SHU1_B1_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_R_DMRANK_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_R_DMSTBEN_SYNC_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_R_DMRXDLY_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B1)
                                        | P_Fld(0x0, SHU1_B1_DQ8_R_DMRXDVS_UPD_FORCE_EN_B1)
                                        | P_Fld(0x7fff, SHU1_B1_DQ8_R_DMRXDVS_UPD_FORCE_CYC_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ8+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, SHU1_B1_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_R_DMRANK_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_R_DMSTBEN_SYNC_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_R_DMRXDLY_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU1_B1_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B1)
                                        | P_Fld(0x0, SHU1_B1_DQ8_R_DMRXDVS_UPD_FORCE_EN_B1)
                                        | P_Fld(0x7fff, SHU1_B1_DQ8_R_DMRXDVS_UPD_FORCE_CYC_B1));
    vIO32WriteFldMulti_All(DDRPHY_SHU1_CA_CMD8, P_Fld(0x1, SHU1_CA_CMD8_R_DMRANK_CHG_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_R_DMRANK_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_SYNC_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_R_DMSTBEN_SYNC_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_R_DMRXDLY_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_PIPE_CG_IG_CA)
                                        | P_Fld(0x0, SHU1_CA_CMD8_R_DMRXDVS_UPD_FORCE_EN_CA)
                                        | P_Fld(0x7fff, SHU1_CA_CMD8_R_DMRXDVS_UPD_FORCE_CYC_CA));
    vIO32WriteFldMulti_All(DDRPHY_SHU1_CA_CMD8+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, SHU1_CA_CMD8_R_DMRANK_CHG_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_R_DMRANK_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_SYNC_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_R_DMSTBEN_SYNC_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_R_DMRXDLY_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_PIPE_CG_IG_CA)
                                        | P_Fld(0x0, SHU1_CA_CMD8_R_DMRXDVS_UPD_FORCE_EN_CA)
                                        | P_Fld(0x7fff, SHU1_CA_CMD8_R_DMRXDVS_UPD_FORCE_CYC_CA));
    vIO32WriteFldAlign(DDRPHY_MISC_CTRL3, 0x1, MISC_CTRL3_R_DDRPHY_COMB_CG_IG);
    vIO32WriteFldAlign(DDRPHY_MISC_CTRL3+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, MISC_CTRL3_R_DDRPHY_COMB_CG_IG);
    vIO32WriteFldAlign(DDRPHY_MISC_CTRL0, 0x0, MISC_CTRL0_R_DMSHU_PHYDCM_FORCEOFF);
    vIO32WriteFldAlign(DDRPHY_MISC_CTRL0+((U32)CHANNEL_B<<POS_BANK_NUM), 0x0, MISC_CTRL0_R_DMSHU_PHYDCM_FORCEOFF);
    vIO32WriteFldAlign(DDRPHY_MISC_RXDVS2, 0x1, MISC_RXDVS2_R_DMRXDVS_SHUFFLE_CTRL_CG_IG);
    vIO32WriteFldAlign(DDRPHY_MISC_RXDVS2+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, MISC_RXDVS2_R_DMRXDVS_SHUFFLE_CTRL_CG_IG);

    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7, P_Fld(0x0, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0)
                                        | P_Fld(0x0, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0)
                                        | P_Fld(0x0, SHU1_B0_DQ7_R_DMRXDVS_DQM_FLAGSEL_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x0, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0)
                                        | P_Fld(0x0, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0)
                                        | P_Fld(0x0, SHU1_B0_DQ7_R_DMRXDVS_DQM_FLAGSEL_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ7, P_Fld(0x0, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1)
                                        | P_Fld(0x0, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1)
                                        | P_Fld(0x0, SHU1_B1_DQ7_R_DMRXDVS_DQM_FLAGSEL_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ7+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x0, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1)
                                            | P_Fld(0x0, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1)
                                            | P_Fld(0x0, SHU1_B1_DQ7_R_DMRXDVS_DQM_FLAGSEL_B1));

    if(1) //(mt_get_chip_sw_ver()==CHIP_SW_VER_02)
    {
        vIO32WriteFldMulti(DRAMC_REG_CLKAR, P_Fld(0x1, CLKAR_SELPH_CMD_CG_DIS) | P_Fld(0x7FFF, CLKAR_REQQUE_PACG_DIS));
    }
    else
    {
        vIO32WriteFldAlign(DRAMC_REG_CLKAR, 0x0, CLKAR_SEQCLKRUN);
    }

    vIO32WriteFldAlign(DRAMC_REG_SHU_DQSG_RETRY, 0x1, SHU_DQSG_RETRY_R_RETRY_PA_DSIABLE);
    vIO32WriteFldAlign(DRAMC_REG_WRITE_LEV, 0x0, WRITE_LEV_DDRPHY_COMB_CG_SEL);
    vIO32WriteFldMulti(DRAMC_REG_DUMMY_RD, P_Fld(0x1, DUMMY_RD_DUMMY_RD_PA_OPT)
                                        | P_Fld(0x1, DUMMY_RD_DMYRD_REORDER_DIS) //YH: dummy read with reorder high priority 1:disable 0:enable
                                        | P_Fld(0x1, DUMMY_RD_DMYRD_HPRI_DIS)); // YH: dummy read request with high priority 1:disable 0:enable
    vIO32WriteFldMulti(DRAMC_REG_STBCAL2, P_Fld(0x1, STBCAL2_STB_UIDLYCG_IG)
                                            | P_Fld(0x1, STBCAL2_STB_PIDLYCG_IG));
    vIO32WriteFldMulti(DRAMC_REG_EYESCAN, P_Fld(0x1, EYESCAN_EYESCAN_DQS_SYNC_EN)
                                            | P_Fld(0x1, EYESCAN_EYESCAN_NEW_DQ_SYNC_EN)
                                            | P_Fld(0x1, EYESCAN_EYESCAN_DQ_SYNC_EN));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ODTCTRL, P_Fld(0x1, SHU_ODTCTRL_RODTENSTB_SELPH_CG_IG)
                                            | P_Fld(0x1, SHU_ODTCTRL_RODTEN_SELPH_CG_IG));

    #if 1//#ifndef BIANCO_TO_BE_PORTING
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DLL0, 0x1, SHU1_B0_DLL0_RG_ARPISM_MCK_SEL_B0_SHU);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DLL0, 0x1, SHU1_B1_DLL0_RG_ARPISM_MCK_SEL_B1_SHU);
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_DLL0, 0x1, SHU1_CA_DLL0_RG_ARPISM_MCK_SEL_CA_SHU);
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DLL0+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, SHU1_B0_DLL0_RG_ARPISM_MCK_SEL_B0_SHU);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DLL0+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, SHU1_B1_DLL0_RG_ARPISM_MCK_SEL_B1_SHU);
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_DLL0+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, SHU1_CA_DLL0_RG_ARPISM_MCK_SEL_CA_SHU);
    #endif
    //end _K_

    //Rx Gating tracking settings
    vIO32WriteFldMulti_All(DRAMC_REG_SHURK0_DQSCAL, P_Fld(0, SHURK0_DQSCAL_R0DQSIENLLMTEN) | P_Fld(0, SHURK0_DQSCAL_R0DQSIENHLMTEN));
    vIO32WriteFldMulti_All(DRAMC_REG_SHURK1_DQSCAL, P_Fld(0, SHURK1_DQSCAL_R1DQSIENLLMTEN) | P_Fld(0, SHURK1_DQSCAL_R1DQSIENHLMTEN));
    vIO32WriteFldMulti_All(DRAMC_REG_SHU_STBCAL, P_Fld(1, SHU_STBCAL_DQSG_MODE) | P_Fld(1, SHU_STBCAL_PICGLAT));

    //Jade-like ACTiming
    #if DramcHWDQSGatingTracking_DVT_JADE_TRACKING_MODE
	
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_PIPE, 0xFC000000,PHY_FLD_FULL);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ6, 0x38, SHU1_B1_DQ6_RG_ARPI_OFFSET_DQSIEN_B1);
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ6+((U32)CHANNEL_B<<POS_BANK_NUM), 0x38, SHU1_B0_DQ6_RG_ARPI_OFFSET_DQSIEN_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD6+((U32)CHANNEL_B<<POS_BANK_NUM), 0x38, SHU1_CA_CMD6_RG_ARPI_OFFSET_CLKIEN);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ6+((U32)CHANNEL_B<<POS_BANK_NUM), 0x38, SHU1_B1_DQ6_RG_ARPI_OFFSET_DQSIEN_B1);
    //if(p->frequency==933)//test code
    {
        vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ3, 0, SHU1_B0_DQ3_DQ_REV_B0_BIT_05);
        vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD3, 0, SHU1_CA_CMD3_ARCMD_REV_BIT_05);
        vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ3, 0, SHU1_B1_DQ3_DQ_REV_B1_BIT_05);
        vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ3+((U32)CHANNEL_B<<POS_BANK_NUM), 0, SHU1_B0_DQ3_DQ_REV_B0_BIT_05);
        vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD3+((U32)CHANNEL_B<<POS_BANK_NUM), 0, SHU1_CA_CMD3_ARCMD_REV_BIT_05);
        vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ3+((U32)CHANNEL_B<<POS_BANK_NUM), 0, SHU1_B1_DQ3_DQ_REV_B1_BIT_05);
    }

    vIO32WriteFldAlign_All(DRAMC_REG_SHU1_DQSG, 0x0, SHU1_DQSG_STB_UPDMASKCYC);
    vIO32WriteFldAlign_All(DRAMC_REG_SHU1_DQSG, 0x0, SHU1_DQSG_STB_UPDMASK_EN);
    if(p->frequency==933)
    {
        vIO32WriteFldAlign_All(DRAMC_REG_SHU_STBCAL, 3, SHU_STBCAL_DMSTBLAT);
        //vIO32WriteFldAlign_All(DRAMC_REG_SHU_ACTIM2, 0x7, SHU_ACTIM2_TR2W);
        //vIO32WriteFldAlign_All(DRAMC_REG_SHU_AC_TIME_05T, 0, SHU_AC_TIME_05T_TR2W_05T);
        //vIO32WriteFldAlign_All(DRAMC_REG_SHU_ACTIM_XRT, 0x5, SHU_ACTIM_XRT_XRTW2R);
        //vIO32WriteFldAlign_All(DRAMC_REG_SHU_ACTIM_XRT, 0x6, SHU_ACTIM_XRT_XRTR2W);
    }
    else if(p->frequency==667)
    {
        vIO32WriteFldAlign_All(DRAMC_REG_SHU_STBCAL, 2, SHU_STBCAL_DMSTBLAT);
        //vIO32WriteFldAlign_All(DRAMC_REG_SHU_ACTIM2, 0x5, SHU_ACTIM2_TR2W);
        //vIO32WriteFldAlign_All(DRAMC_REG_SHU_AC_TIME_05T, 0, SHU_AC_TIME_05T_TR2W_05T);
        //vIO32WriteFldAlign_All(DRAMC_REG_SHU_ACTIM_XRT, 0x5, SHU_ACTIM_XRT_XRTW2R);
        //vIO32WriteFldAlign_All(DRAMC_REG_SHU_ACTIM_XRT, 0x6, SHU_ACTIM_XRT_XRTR2W);
    }
    else if(p->frequency==467)
    {
        vIO32WriteFldAlign_All(DRAMC_REG_SHU_STBCAL, 1, SHU_STBCAL_DMSTBLAT);
        //vIO32WriteFldAlign_All(DRAMC_REG_SHU_ACTIM2, 0x4, SHU_ACTIM2_TR2W);
        //vIO32WriteFldAlign_All(DRAMC_REG_SHU_AC_TIME_05T, 0, SHU_AC_TIME_05T_TR2W_05T);
        //vIO32WriteFldAlign_All(DRAMC_REG_SHU_ACTIM_XRT, 0x5, SHU_ACTIM_XRT_XRTW2R);
        //vIO32WriteFldAlign_All(DRAMC_REG_SHU_ACTIM_XRT, 0x6, SHU_ACTIM_XRT_XRTR2W);
    }

    //For LP3 FFFF corner IC pass LTLV test
    vIO32WriteFldAlign_All(DDRPHY_SHU1_B0_DQ5, 4, SHU1_B0_DQ5_RG_RX_ARDQS_DQSIEN_DLY_B0);
    vIO32WriteFldAlign_All(DDRPHY_SHU1_B1_DQ5, 4, SHU1_B1_DQ5_RG_RX_ARDQS_DQSIEN_DLY_B1);
    vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_CMD5, 4, SHU1_CA_CMD5_RG_RX_ARCLK_DQSIEN_DLY);


    #else
    //Valid delay mode debug : monitor window
    //vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ5+((U32)CHANNEL_B<<POS_BANK_NUM), 7, SHU1_B0_DQ5_RG_RX_ARDQS0_DQSIEN_DLY_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5+((U32)CHANNEL_B<<POS_BANK_NUM), 7, SHU1_B1_DQ5_RG_RX_ARDQS0_DQSIEN_DLY_B1);
    //vIO32WriteFldAlign_All(DRAMC_REG_SHU_PIPE, 0xF0000000,PHY_FLD_FULL);
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DQSG), \
                                            P_Fld(9, SHU1_DQSG_STB_UPDMASKCYC) | \
                                            P_Fld(1, SHU1_DQSG_STB_UPDMASK_EN));
    #endif

    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ5, 0x4, SHU1_B0_DQ5_RG_ARPI_FB_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5, 0x4, SHU1_B1_DQ5_RG_ARPI_FB_B1);
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD5, 0x4, SHU1_CA_CMD5_RG_ARPI_FB_CA);
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ5+((U32)CHANNEL_B<<POS_BANK_NUM), 0x4, SHU1_B0_DQ5_RG_ARPI_FB_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5+((U32)CHANNEL_B<<POS_BANK_NUM), 0x4, SHU1_B1_DQ5_RG_ARPI_FB_B1);
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD5+((U32)CHANNEL_B<<POS_BANK_NUM), 0x4, SHU1_CA_CMD5_RG_ARPI_FB_CA);

    //DE_UPDATE
    vIO32WriteFldMulti(DRAMC_REG_DRAMCTRL, P_Fld(0, DRAMCTRL_FW2R)
                                        | P_Fld(0x0, DRAMCTRL_DYNMWREN)
                                        | P_Fld(0x0, DRAMCTRL_CLKWITRFC)
                                        | P_Fld(0x1, DRAMCTRL_TMRR2WDIS)
                                        | P_Fld(0x0, DRAMCTRL_ADRBIT3DEC)
                                        | P_Fld(0x0, DRAMCTRL_CTOREQ_HPRI_OPT));
    vIO32WriteFldAlign(DRAMC_REG_MISCTL0, 0x1, MISCTL0_PBC_ARB_EN); //Darren+ for LPDDR3
    vIO32WriteFldAlign(DRAMC_REG_PERFCTL0, 0x1, PERFCTL0_WRFIFO_OPT);
    vIO32WriteFldMulti(DRAMC_REG_PERFCTL0, P_Fld(0x1, PERFCTL0_REORDEREN)
                    | P_Fld(0x0, PERFCTL0_RWHPRICTL)
                    | P_Fld(0x1, PERFCTL0_EMILLATEN)
                    | P_Fld(0x1, PERFCTL0_RWAGEEN)
                    | P_Fld(0x1, PERFCTL0_RWHPRIEN)); //Darren+ by Derping confirm
    vIO32WriteFldAlign(DRAMC_REG_SREFCTRL, 0x8, SREFCTRL_SREFDLY);
    vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x1, SPCMDCTRL_REFR_BLOCKEN);
    vIO32WriteFldAlign(DRAMC_REG_HW_MRR_FUN, 0x0, HW_MRR_FUN_TMRR_ENA);
    vIO32WriteFldMulti(DRAMC_REG_SHUCTRL, P_Fld(0x0, SHUCTRL_VRCG_EN)
                                        | P_Fld(0x3, SHUCTRL_R_DVFS_PICG_MARGIN3));
    vIO32WriteFldMulti(DRAMC_REG_SHUCTRL2, P_Fld(0x1, SHUCTRL2_SHORTQ_OPT) //YH: short mode selection 0:original (worst case 4 commands in queue before DVFS), 1: new (worst case 2 commands)
                                        | P_Fld(0x3, SHUCTRL2_R_DVFS_PICG_MARGIN)
                                        | P_Fld(0x1, SHUCTRL2_R_DVFS_SREF_OPT)); //sw to enable dvfs + refresh

    vIO32WriteFldMulti(DRAMC_REG_REFCTRL0, P_Fld(0x1, REFCTRL0_REFNA_OPT)
                | P_Fld(0x3, REFCTRL0_DISBYREFNUM)
                | P_Fld(0x1, REFCTRL0_UPDBYWR)); //Darren+ for LPDDR3
    vIO32WriteFldMulti(DDRPHY_B0_DQ6, P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B0)
                                | P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0));
    vIO32WriteFldMulti(DDRPHY_CA_CMD6, P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_OP_BIAS_SW_EN)
                                    | P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_BIAS_EN));

#ifdef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
    vIO32WriteFldMulti(DDRPHY_B0_DQ6+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, B0_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B0)
                                | P_Fld(0x1, B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0));
    vIO32WriteFldMulti(DDRPHY_B1_DQ6, P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B1)
                                | P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1));
    vIO32WriteFldMulti(DDRPHY_B1_DQ6+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B1)
                                | P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1));
    vIO32WriteFldMulti(DDRPHY_CA_CMD6+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, CA_CMD6_RG_RX_ARCMD_OP_BIAS_SW_EN)
                                | P_Fld(0x1, CA_CMD6_RG_RX_ARCMD_BIAS_EN));
    vIO32WriteFldAlign(DRAMC_REG_STBCAL2, 0x1, STBCAL2_STB_PICG_EARLY_1T_EN);

    vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ7, P_Fld(0x1, SHU1_B1_DQ7_R_DMRXRANK_DQS_LAT_B1)
                                | P_Fld(0x1, SHU1_B1_DQ7_R_DMRXRANK_DQS_EN_B1)
                                | P_Fld(0x0, SHU1_B1_DQ7_R_DMRXRANK_DQ_LAT_B1)
                                | P_Fld(0x1, SHU1_B1_DQ7_R_DMRXRANK_DQ_EN_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, SHU1_B0_DQ7_R_DMRXRANK_DQS_LAT_B0)
                                | P_Fld(0x1, SHU1_B0_DQ7_R_DMRXRANK_DQS_EN_B0)
                                | P_Fld(0x0, SHU1_B0_DQ7_R_DMRXRANK_DQ_LAT_B0)
                                | P_Fld(0x1, SHU1_B0_DQ7_R_DMRXRANK_DQ_EN_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ7+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, SHU1_B1_DQ7_R_DMRXRANK_DQS_LAT_B1)
                                | P_Fld(0x1, SHU1_B1_DQ7_R_DMRXRANK_DQS_EN_B1)
                                | P_Fld(0x0, SHU1_B1_DQ7_R_DMRXRANK_DQ_LAT_B1)
                                | P_Fld(0x1, SHU1_B1_DQ7_R_DMRXRANK_DQ_EN_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD7+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, SHU1_CA_CMD7_R_DMRXRANK_CLK_LAT)
                                | P_Fld(0x1, SHU1_CA_CMD7_R_DMRXRANK_CLK_EN)
                                | P_Fld(0x0, SHU1_CA_CMD7_R_DMRXRANK_CMD_LAT)
                                | P_Fld(0x1, SHU1_CA_CMD7_R_DMRXRANK_CMD_EN));

#else

    vIO32WriteFldMulti(DDRPHY_B0_DQ6+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B0)
                                | P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0));
    vIO32WriteFldMulti(DDRPHY_B1_DQ6, P_Fld(0x0, B1_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B1)
                                | P_Fld(0x0, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1));
    vIO32WriteFldMulti(DDRPHY_B1_DQ6+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x0, B1_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B1)
                                | P_Fld(0x0, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1));
    vIO32WriteFldMulti(DDRPHY_CA_CMD6+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_OP_BIAS_SW_EN)
                                | P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_BIAS_EN));
#endif
    //end DE UPDATE

    
    //review
    vIO32WriteFldMulti_All(DDRPHY_CA_DLL_ARPI1, P_Fld(0x0, CA_DLL_ARPI1_RG_ARPI_MCTL_JUMP_EN_CA)
                | P_Fld(0x0, CA_DLL_ARPI1_RG_ARPI_FB_JUMP_EN_CA)
                | P_Fld(0x0, CA_DLL_ARPI1_RG_ARPI_CS_JUMP_EN) 
                | P_Fld(0x0, CA_DLL_ARPI1_RG_ARPI_CLK_JUMP_EN)
                | P_Fld(0x0, CA_DLL_ARPI1_RG_ARPI_CMD_JUMP_EN)
                | P_Fld(0x0, CA_DLL_ARPI1_RG_ARPI_CLKIEN_JUMP_EN));
    vIO32WriteFldMulti_All(DDRPHY_B0_DLL_ARPI1, P_Fld(0x0, B0_DLL_ARPI1_RG_ARPI_MCTL_JUMP_EN_B0)
                | P_Fld(0x0, B0_DLL_ARPI1_RG_ARPI_FB_JUMP_EN_B0)
                | P_Fld(0x0, B0_DLL_ARPI1_RG_ARPI_DQS_JUMP_EN_B0) 
                | P_Fld(0x0, B0_DLL_ARPI1_RG_ARPI_DQM_JUMP_EN_B0)
                | P_Fld(0x0, B0_DLL_ARPI1_RG_ARPI_DQ_JUMP_EN_B0)
                | P_Fld(0x0, B0_DLL_ARPI1_RG_ARPI_DQSIEN_JUMP_EN_B0));
    vIO32WriteFldMulti_All(DDRPHY_B1_DLL_ARPI1, P_Fld(0x0, B1_DLL_ARPI1_RG_ARPI_MCTL_JUMP_EN_B1)
                | P_Fld(0x0, B1_DLL_ARPI1_RG_ARPI_FB_JUMP_EN_B1)
                | P_Fld(0x0, B1_DLL_ARPI1_RG_ARPI_DQS_JUMP_EN_B1) 
                | P_Fld(0x0, B1_DLL_ARPI1_RG_ARPI_DQM_JUMP_EN_B1)
                | P_Fld(0x0, B1_DLL_ARPI1_RG_ARPI_DQ_JUMP_EN_B1)
                | P_Fld(0x0, B1_DLL_ARPI1_RG_ARPI_DQSIEN_JUMP_EN_B1));

#if 0
    vIO32WriteFldMulti_All(DDRPHY_B0_DQ3, P_Fld(0x1, B0_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B0)
                        | P_Fld(0x1, B0_DQ3_RG_RX_ARDQM0_IN_BUFF_EN)
                        | P_Fld(0x1, B0_DQ3_RG_RX_ARDQS0_IN_BUFF_EN_B0));
    vIO32WriteFldMulti_All(DDRPHY_B1_DQ3, P_Fld(0x1, B1_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B1)
                        | P_Fld(0x1, B1_DQ3_RG_RX_ARDQM1_IN_BUFF_EN)
                        | P_Fld(0x1, B1_DQ3_RG_RX_ARDQS1_IN_BUFF_EN));
    vIO32WriteFldMulti_All(DDRPHY_CA_CMD3, P_Fld(0x1, CA_CMD3_RG_RX_ARCMD_IN_BUFF_EN)
                            | P_Fld(0x1, CA_CMD3_RG_RX_ARCLK_IN_BUFF_EN));
#endif

    vIO32WriteFldMulti_All(DDRPHY_CA_CMD2, P_Fld(0x1, CA_CMD2_RG_TX_ARCMD_ODTEN_DIS) 
                | P_Fld(0x1, CA_CMD2_RG_TX_ARCLK_ODTEN_DIS));
    vIO32WriteFldMulti_All(DDRPHY_B0_DQ2, P_Fld(0x1, B0_DQ2_RG_TX_ARDQ_ODTEN_DIS_B0) 
                | P_Fld(0x1, B0_DQ2_RG_TX_ARDQS_ODTEN_DIS_B0));
    vIO32WriteFldMulti_All(DDRPHY_B1_DQ2, P_Fld(0x1, B1_DQ2_RG_TX_ARDQ_ODTEN_DIS_B1) 
                | P_Fld(0x1, B1_DQ2_RG_TX_ARDQS_ODTEN_DIS_B1));

    //vIO32WriteFldAlign_All(DDRPHY_B0_DQ6, 0x0, B0_DQ6_RG_TX_ARDQ_OE_EXT_DIS_B0);    
    //vIO32WriteFldAlign_All(DDRPHY_B1_DQ6, 0x0, B1_DQ6_RG_TX_ARDQ_OE_EXT_DIS_B1);
    //vIO32WriteFldAlign_All(DDRPHY_CA_CMD6, 0x0, CA_CMD6_RG_TX_ARCMD_OE_EXT_DIS);

    vIO32WriteFldAlign_All(DDRPHY_B0_DQ5, 0x1, B0_DQ5_RG_RX_ARDQS_DVS_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DQ5, 0x1, B1_DQ5_RG_RX_ARDQS_DVS_EN_B1);
    vIO32WriteFldAlign_All(DDRPHY_CA_CMD5, 0x1, CA_CMD5_RG_RX_ARCLK_DVS_EN);
    
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL4, 0x13300000, MISC_CG_CTRL4_R_PHY_MCK_CG_CTRL);
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL4+(1<<POS_BANK_NUM), 0x22200a00, MISC_CG_CTRL4_R_PHY_MCK_CG_CTRL);
    
    vIO32WriteFldMulti_All(DRAMC_REG_PERFCTL0, P_Fld(0x0, PERFCTL0_RWSPLIT) | P_Fld(0x0, PERFCTL0_REORDEREN));
    vIO32WriteFldAlign_All(DRAMC_REG_CLKCTRL, 0x1, CLKCTRL_SEQCLKRUN3);
    vIO32WriteFldAlign_All(DRAMC_REG_STBCAL1, 0x0, STBCAL1_DLLFRZ_MON_PBREF_OPT);
    vIO32WriteFldAlign_All(DRAMC_REG_ARBCTL, 0x0, ARBCTL_DBIWR_IMP_EN );

#if (fcFOR_CHIP_ID == fcBianco)
    if(u1GetICVersion_Bianco()>=IC_VERSION_E3)
    {
        vIO32WriteFldAlign_All(DDRPHY_SHU1_B0_DQ3, 0x3, SHU1_B0_DQ3_RG_TX_ARDQ_PU_PRE_B0);
        vIO32WriteFldAlign_All(DDRPHY_SHU1_B1_DQ3, 0x3, SHU1_B1_DQ3_RG_TX_ARDQ_PU_PRE_B1);
        //vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_CMD3, 0x3, SHU1_CA_CMD3_RG_TX_ARCMD_PU_PRE); //move to SW impedance

        vIO32WriteFldAlign_All(DDRPHY_SHU1_B0_DQ6, 0, SHU1_B0_DQ6_RG_ARPI_RESERVE_B0_BIT23);
        vIO32WriteFldAlign_All(DDRPHY_SHU1_B1_DQ6, 0, SHU1_B0_DQ6_RG_ARPI_RESERVE_B0_BIT23);
        vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_CMD6, 0, SHU1_B0_DQ6_RG_ARPI_RESERVE_B0_BIT23);
    }
#endif


    return DRAM_OK;
}
#endif

#if ENABLE_LP3_SW
#if 0
static void DramcSetting_Olympus_LP3_DDR1333(DRAMC_CTX_T *p)
{
    ////DRAMC0-SHU2
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM0, P_Fld(0x7, SHU_ACTIM0_TRCD)
                | P_Fld(0x1, SHU_ACTIM0_TRRD)
                | P_Fld(0x9, SHU_ACTIM0_TWR)
                | P_Fld(0x6, SHU_ACTIM0_TWTR));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM1, P_Fld(0xe, SHU_ACTIM1_TRC)
                | P_Fld(0x5, SHU_ACTIM1_TRAS)
                | P_Fld(0x6, SHU_ACTIM1_TRP)
                | P_Fld(0x1, SHU_ACTIM1_TRPAB));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM2, P_Fld(0x8, SHU_ACTIM2_TFAW)
                | P_Fld(0x4, SHU_ACTIM2_TR2W)
                | P_Fld(0x1, SHU_ACTIM2_TRTP));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM3, P_Fld(0x3a, SHU_ACTIM3_TRFC)
                | P_Fld(0x50, SHU_ACTIM3_REFCNT)
                | P_Fld(0x13, SHU_ACTIM3_TRFCPB));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM4, P_Fld(0x1d, SHU_ACTIM4_TZQCS)
                | P_Fld(0x48, SHU_ACTIM4_TXREFCNT));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM5, P_Fld(0x8, SHU_ACTIM5_TWTPD)
                | P_Fld(0x7, SHU_ACTIM5_TR2PD));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM_XRT, P_Fld(0x2, SHU_ACTIM_XRT_XRTR2W)
                | P_Fld(0x9, SHU_ACTIM_XRT_XRTR2R));
    vIO32WriteFldMulti(DRAMC_REG_SHU_AC_TIME_05T, P_Fld(0x0, SHU_AC_TIME_05T_TFAW_05T)
                | P_Fld(0x1, SHU_AC_TIME_05T_TRRD_05T)
                | P_Fld(0x0, SHU_AC_TIME_05T_TWR_M05T)
                | P_Fld(0x0, SHU_AC_TIME_05T_TRAS_05T)
                | P_Fld(0x0, SHU_AC_TIME_05T_TRPAB_05T)
                | P_Fld(0x0, SHU_AC_TIME_05T_TRP_05T)
                | P_Fld(0x0, SHU_AC_TIME_05T_TRCD_05T)
                | P_Fld(0x1, SHU_AC_TIME_05T_TRTP_05T)
                | P_Fld(0x0, SHU_AC_TIME_05T_TXP_05T)
                | P_Fld(0x0, SHU_AC_TIME_05T_TRC_05T));
    vIO32WriteFldMulti(DRAMC_REG_SHU_CONF1, P_Fld(0xb, SHU_CONF1_DATLAT_DSEL_PHY)
                | P_Fld(0xb, SHU_CONF1_DATLAT_DSEL)
                | P_Fld(0xd, SHU_CONF1_DATLAT));
    vIO32WriteFldMulti(DRAMC_REG_SHU_RANKCTL, P_Fld(0x3, SHU_RANKCTL_RANKINCTL_PHY)
                | P_Fld(0x1, SHU_RANKCTL_RANKINCTL_ROOT1)
                | P_Fld(0x1, SHU_RANKCTL_RANKINCTL));
    //vIO32WriteFldAlign(DRAMC_REG_SHU_CKECTRL, 0x2, SHU_CKECTRL_CKEPRD); CKEPRD's correct value is set in UpdateACTImingReg( )
    vIO32WriteFldAlign(DRAMC_REG_SHU_ODTCTRL, 0x5, SHU_ODTCTRL_RODT);
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS0, P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_OEN_DQS3)
                | P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_OEN_DQS2)
                | P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_OEN_DQS1)
                | P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_OEN_DQS0)
                | P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_DQS3)
                | P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_DQS2)
                | P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_DQS1)
                | P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_DQS0));
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS1, P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS3)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS2)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS1)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS0)
                | P_Fld(0x7, SHU_SELPH_DQS1_DLY_DQS3)
                | P_Fld(0x7, SHU_SELPH_DQS1_DLY_DQS2)
                | P_Fld(0x7, SHU_SELPH_DQS1_DLY_DQS1)
                | P_Fld(0x7, SHU_SELPH_DQS1_DLY_DQS0));
    vIO32WriteFldAlign(DRAMC_REG_SHU1_DQSG, 0x1f, SHU1_DQSG_SCINTV);
    //vIO32WriteFldAlign(DRAMC_REG_SHU_HWSET_MR13, 0x18, SHU_HWSET_MR13_HWSET_MR13_OP);
    vIO32WriteFldAlign(DRAMC_REG_SHURK0_DQSCTL, 0x4, SHURK0_DQSCTL_DQSINCTL);
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN1, P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN_P1)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN_P1)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN_P1)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN_P1)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0, P_Fld(0x0, SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED_P1)
                | P_Fld(0x0, SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED)
                | P_Fld(0x0, SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED_P1)
                | P_Fld(0x0, SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED)
                | P_Fld(0x0, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
                | P_Fld(0x0, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
                | P_Fld(0x0, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
                | P_Fld(0x0, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED_P1)
                | P_Fld(0x4, SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED)
                | P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED_P1)
                | P_Fld(0x4, SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED)
                | P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
                | P_Fld(0x4, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
                | P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
                | P_Fld(0x4, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3)
                | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2)
                | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1)
                | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0)
                | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ3)
                | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ2)
                | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ1)
                | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3)
                | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2)
                | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1)
                | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0)
                | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM3)
                | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM2)
                | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM1)
                | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ2, P_Fld(0x6, SHURK0_SELPH_DQ2_DLY_OEN_DQ3)
                | P_Fld(0x6, SHURK0_SELPH_DQ2_DLY_OEN_DQ2)
                | P_Fld(0x6, SHURK0_SELPH_DQ2_DLY_OEN_DQ1)
                | P_Fld(0x6, SHURK0_SELPH_DQ2_DLY_OEN_DQ0)
                | P_Fld(0x7, SHURK0_SELPH_DQ2_DLY_DQ3)
                | P_Fld(0x7, SHURK0_SELPH_DQ2_DLY_DQ2)
                | P_Fld(0x7, SHURK0_SELPH_DQ2_DLY_DQ1)
                | P_Fld(0x7, SHURK0_SELPH_DQ2_DLY_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ3, P_Fld(0x6, SHURK0_SELPH_DQ3_DLY_OEN_DQM3)
                | P_Fld(0x6, SHURK0_SELPH_DQ3_DLY_OEN_DQM2)
                | P_Fld(0x6, SHURK0_SELPH_DQ3_DLY_OEN_DQM1)
                | P_Fld(0x6, SHURK0_SELPH_DQ3_DLY_OEN_DQM0)
                | P_Fld(0x7, SHURK0_SELPH_DQ3_DLY_DQM3)
                | P_Fld(0x7, SHURK0_SELPH_DQ3_DLY_DQM2)
                | P_Fld(0x7, SHURK0_SELPH_DQ3_DLY_DQM1)
                | P_Fld(0x7, SHURK0_SELPH_DQ3_DLY_DQM0));
    vIO32WriteFldAlign(DRAMC_REG_SHURK1_DQSCTL, 0x4, SHURK1_DQSCTL_R1DQSINCTL);
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN1, P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B2_R1RODTEN_P1)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B2_R1RODTEN)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN_P1)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN_P1)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0, P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS3_GATED_P1)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS3_GATED)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS2_GATED_P1)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS2_GATED)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1, P_Fld(0x2, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS3_GATED_P1)
                | P_Fld(0x0, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS3_GATED)
                | P_Fld(0x2, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS2_GATED_P1)
                | P_Fld(0x0, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS2_GATED)
                | P_Fld(0x2, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
                | P_Fld(0x0, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED)
                | P_Fld(0x2, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1)
                | P_Fld(0x0, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3)
                | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2)
                | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1)
                | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0)
                | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ3)
                | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ2)
                | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ1)
                | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3)
                | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2)
                | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1)
                | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0)
                | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM3)
                | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM2)
                | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM1)
                | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ2, P_Fld(0x6, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ3)
                | P_Fld(0x6, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ2)
                | P_Fld(0x6, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ1)
                | P_Fld(0x6, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ0)
                | P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1DQ3)
                | P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1DQ2)
                | P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1DQ1)
                | P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ3, P_Fld(0x6, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM3)
                | P_Fld(0x6, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM2)
                | P_Fld(0x6, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM1)
                | P_Fld(0x6, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM0)
                | P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1DQM3)
                | P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1DQM2)
                | P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1DQM1)
                | P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1DQM0));
    ////DRAMC1-SHU2
    ////DDRPHY0-SHU2
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5, 0x6, SHU1_B1_DQ5_RG_RX_ARDQS_DVS_DLY_B1);
    //vIO32WriteFldAlign(DDRPHY_SHU1_PLL5, 0x3300, SHU1_PLL5_RG_RPHYPLL_SDM_PCW);
    //vIO32WriteFldAlign(DDRPHY_SHU1_PLL7, 0x3300, SHU1_PLL7_RG_RCLRPLL_SDM_PCW);
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ6, P_Fld(0x16, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0xe, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ0, P_Fld(0x4, SHU1_R1_B1_DQ0_RK1_TX_ARDQ7_DLY_B1)
                | P_Fld(0x4, SHU1_R1_B1_DQ0_RK1_TX_ARDQ6_DLY_B1)
                | P_Fld(0x4, SHU1_R1_B1_DQ0_RK1_TX_ARDQ5_DLY_B1)
                | P_Fld(0x4, SHU1_R1_B1_DQ0_RK1_TX_ARDQ4_DLY_B1)
                | P_Fld(0x4, SHU1_R1_B1_DQ0_RK1_TX_ARDQ3_DLY_B1)
                | P_Fld(0x4, SHU1_R1_B1_DQ0_RK1_TX_ARDQ2_DLY_B1)
                | P_Fld(0x4, SHU1_R1_B1_DQ0_RK1_TX_ARDQ1_DLY_B1)
                | P_Fld(0x4, SHU1_R1_B1_DQ0_RK1_TX_ARDQ0_DLY_B1));
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_B1_DQ1, 0x4, SHU1_R1_B1_DQ1_RK1_TX_ARDQM0_DLY_B1);
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ6, P_Fld(0x17, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0xf, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_R_DLY_B1));
    ////DDRPHY1-SHU2
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ5+((U32)CHANNEL_B<<POS_BANK_NUM), 0x6, SHU1_B0_DQ5_RG_RX_ARDQS_DVS_DLY_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5+((U32)CHANNEL_B<<POS_BANK_NUM), 0x6, SHU1_B1_DQ5_RG_RX_ARDQS_DVS_DLY_B1);
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD5+((U32)CHANNEL_B<<POS_BANK_NUM), 0x6, SHU1_CA_CMD5_RG_RX_ARCLK_DVS_DLY);
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ2+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_F_DLY_B0)
                | P_Fld(0x12, SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_F_DLY_B0));
    vIO32WriteFldAlign(DDRPHY_SHU1_R0_B0_DQ3+((U32)CHANNEL_B<<POS_BANK_NUM), 0x12, SHU1_R0_B0_DQ3_RK0_RX_ARDQ2_F_DLY_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_R0_B0_DQ4+((U32)CHANNEL_B<<POS_BANK_NUM), 0x12, SHU1_R0_B0_DQ4_RK0_RX_ARDQ4_F_DLY_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_R0_B0_DQ5+((U32)CHANNEL_B<<POS_BANK_NUM), 0x12, SHU1_R0_B0_DQ5_RK0_RX_ARDQ6_F_DLY_B0);
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ6+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x16, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_F_DLY_B0)
                | P_Fld(0xe, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0)
                | P_Fld(0x12, SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_F_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ6+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x16, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0xe, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD8+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x16, SHU1_R0_CA_CMD8_RG_RK0_RX_ARCLK_F_DLY)
                | P_Fld(0xe, SHU1_R0_CA_CMD8_RG_RK0_RX_ARCLK_R_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ0+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x4, SHU1_R1_B0_DQ0_RK1_TX_ARDQ7_DLY_B0)
                | P_Fld(0x4, SHU1_R1_B0_DQ0_RK1_TX_ARDQ6_DLY_B0)
                | P_Fld(0x4, SHU1_R1_B0_DQ0_RK1_TX_ARDQ5_DLY_B0)
                | P_Fld(0x4, SHU1_R1_B0_DQ0_RK1_TX_ARDQ4_DLY_B0)
                | P_Fld(0x4, SHU1_R1_B0_DQ0_RK1_TX_ARDQ3_DLY_B0)
                | P_Fld(0x4, SHU1_R1_B0_DQ0_RK1_TX_ARDQ2_DLY_B0)
                | P_Fld(0x4, SHU1_R1_B0_DQ0_RK1_TX_ARDQ1_DLY_B0)
                | P_Fld(0x4, SHU1_R1_B0_DQ0_RK1_TX_ARDQ0_DLY_B0));
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_B0_DQ1+((U32)CHANNEL_B<<POS_BANK_NUM), 0x4, SHU1_R1_B0_DQ1_RK1_TX_ARDQM0_DLY_B0);
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ6+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x17, SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_F_DLY_B0)
                | P_Fld(0xf, SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ0+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x4, SHU1_R1_B1_DQ0_RK1_TX_ARDQ7_DLY_B1)
                | P_Fld(0x4, SHU1_R1_B1_DQ0_RK1_TX_ARDQ6_DLY_B1)
                | P_Fld(0x4, SHU1_R1_B1_DQ0_RK1_TX_ARDQ5_DLY_B1)
                | P_Fld(0x4, SHU1_R1_B1_DQ0_RK1_TX_ARDQ4_DLY_B1)
                | P_Fld(0x4, SHU1_R1_B1_DQ0_RK1_TX_ARDQ3_DLY_B1)
                | P_Fld(0x4, SHU1_R1_B1_DQ0_RK1_TX_ARDQ2_DLY_B1)
                | P_Fld(0x4, SHU1_R1_B1_DQ0_RK1_TX_ARDQ1_DLY_B1)
                | P_Fld(0x4, SHU1_R1_B1_DQ0_RK1_TX_ARDQ0_DLY_B1));
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_B1_DQ1+((U32)CHANNEL_B<<POS_BANK_NUM), 0x4, SHU1_R1_B1_DQ1_RK1_TX_ARDQM0_DLY_B1);
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ6+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x17, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0xf, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD0+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x4, SHU1_R1_CA_CMD0_RK1_TX_ARCA5_DLY)
                | P_Fld(0x4, SHU1_R1_CA_CMD0_RK1_TX_ARCA4_DLY)
                | P_Fld(0x4, SHU1_R1_CA_CMD0_RK1_TX_ARCA3_DLY)
                | P_Fld(0x4, SHU1_R1_CA_CMD0_RK1_TX_ARCA2_DLY)
                | P_Fld(0x4, SHU1_R1_CA_CMD0_RK1_TX_ARCA1_DLY)
                | P_Fld(0x4, SHU1_R1_CA_CMD0_RK1_TX_ARCA0_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD1+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x4, SHU1_R1_CA_CMD1_RK1_TX_ARCKE2_DLY)
                | P_Fld(0x4, SHU1_R1_CA_CMD1_RK1_TX_ARCKE1_DLY)
                | P_Fld(0x4, SHU1_R1_CA_CMD1_RK1_TX_ARCKE0_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD8+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x17, SHU1_R1_CA_CMD8_RG_RK1_RX_ARCLK_F_DLY)
                | P_Fld(0xf, SHU1_R1_CA_CMD8_RG_RK1_RX_ARCLK_R_DLY));
    vIO32WriteFldAlign(DDRPHY_SHU1_R2_B0_DQ1+((U32)CHANNEL_B<<POS_BANK_NUM), 0xb, SHU1_R2_B0_DQ1_RK2_TX_ARDQM0_DLY_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_R2_B1_DQ1+((U32)CHANNEL_B<<POS_BANK_NUM), 0xb, SHU1_R2_B1_DQ1_RK2_TX_ARDQM0_DLY_B1);

}
#endif

#if 0
static void DramcSetting_Olympus_LP3_DDR933(DRAMC_CTX_T *p)
{
    ////DRAMC0-SHU3
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM0, P_Fld(0x4, SHU_ACTIM0_TRCD)
                | P_Fld(0x0, SHU_ACTIM0_TRRD)
                | P_Fld(0x6, SHU_ACTIM0_TWR)
                | P_Fld(0x3, SHU_ACTIM0_TWTR));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM1, P_Fld(0x5, SHU_ACTIM1_TRC)
                | P_Fld(0x0, SHU_ACTIM1_TRAS)
                | P_Fld(0x3, SHU_ACTIM1_TRP)
                | P_Fld(0x0, SHU_ACTIM1_TRPAB));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM2, P_Fld(0x1, SHU_ACTIM2_TFAW)
                | P_Fld(0x3, SHU_ACTIM2_TR2W)
                | P_Fld(0x0, SHU_ACTIM2_TRTP));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM3, P_Fld(0x1e, SHU_ACTIM3_TRFC)
                | P_Fld(0x7, SHU_ACTIM3_TRFCPB));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM4, P_Fld(0x11, SHU_ACTIM4_TZQCS)
                | P_Fld(0x2a, SHU_ACTIM4_TXREFCNT));
    vIO32WriteFldAlign(DRAMC_REG_SHU_ACTIM3, 0x2f, SHU_ACTIM3_REFCNT);
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM5, P_Fld(0x5, SHU_ACTIM5_TWTPD)
                | P_Fld(0x5, SHU_ACTIM5_TR2PD));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM_XRT, P_Fld(0x1, SHU_ACTIM_XRT_XRTR2W)
                | P_Fld(0x9, SHU_ACTIM_XRT_XRTR2R));
    vIO32WriteFldMulti(DRAMC_REG_SHU_AC_TIME_05T, P_Fld(0x0, SHU_AC_TIME_05T_TFAW_05T)
                | P_Fld(0x0, SHU_AC_TIME_05T_TRRD_05T)
                | P_Fld(0x1, SHU_AC_TIME_05T_TWR_M05T)
                | P_Fld(0x1, SHU_AC_TIME_05T_TRAS_05T)
                | P_Fld(0x1, SHU_AC_TIME_05T_TRPAB_05T)
                | P_Fld(0x0, SHU_AC_TIME_05T_TRP_05T)
                | P_Fld(0x0, SHU_AC_TIME_05T_TRCD_05T)
                | P_Fld(0x0, SHU_AC_TIME_05T_TRTP_05T)
                | P_Fld(0x0, SHU_AC_TIME_05T_TXP_05T)
                | P_Fld(0x0, SHU_AC_TIME_05T_TRC_05T));
    vIO32WriteFldMulti(DRAMC_REG_SHU_CONF1, P_Fld(0xa, SHU_CONF1_DATLAT_DSEL_PHY)
                | P_Fld(0xb, SHU_CONF1_DATLAT_DSEL)
                | P_Fld(0xc, SHU_CONF1_DATLAT));
    vIO32WriteFldMulti(DRAMC_REG_SHU_RANKCTL, P_Fld(0x1, SHU_RANKCTL_RANKINCTL_PHY)
                | P_Fld(0x0, SHU_RANKCTL_RANKINCTL_ROOT1)
                | P_Fld(0x0, SHU_RANKCTL_RANKINCTL));
    //vIO32WriteFldAlign(DRAMC_REG_SHU_CKECTRL, 0x2, SHU_CKECTRL_CKEPRD); CKEPRD's correct value is set in UpdateACTImingReg( )
    vIO32WriteFldAlign(DRAMC_REG_SHU_ODTCTRL, 0x3, SHU_ODTCTRL_RODT);
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS0, P_Fld(0x1, SHU_SELPH_DQS0_TXDLY_OEN_DQS3)
                | P_Fld(0x1, SHU_SELPH_DQS0_TXDLY_OEN_DQS2)
                | P_Fld(0x1, SHU_SELPH_DQS0_TXDLY_OEN_DQS1)
                | P_Fld(0x1, SHU_SELPH_DQS0_TXDLY_OEN_DQS0)
                | P_Fld(0x1, SHU_SELPH_DQS0_TXDLY_DQS3)
                | P_Fld(0x1, SHU_SELPH_DQS0_TXDLY_DQS2)
                | P_Fld(0x1, SHU_SELPH_DQS0_TXDLY_DQS1)
                | P_Fld(0x1, SHU_SELPH_DQS0_TXDLY_DQS0));
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS1, P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS3)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS2)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS1)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS0)
                | P_Fld(0x7, SHU_SELPH_DQS1_DLY_DQS3)
                | P_Fld(0x7, SHU_SELPH_DQS1_DLY_DQS2)
                | P_Fld(0x7, SHU_SELPH_DQS1_DLY_DQS1)
                | P_Fld(0x7, SHU_SELPH_DQS1_DLY_DQS0));
    vIO32WriteFldAlign(DRAMC_REG_SHU_SCINTV, 0xf, SHU_SCINTV_SCINTV);
    //vIO32WriteFldAlign(DRAMC_REG_SHU_HWSET_MR13, 0x16, SHU_HWSET_MR13_HWSET_MR13_OP);
    vIO32WriteFldAlign(DRAMC_REG_SHURK0_DQSCTL, 0x2, SHURK0_DQSCTL_DQSINCTL);
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN1, P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN_P1)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN_P1)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN_P1)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN_P1)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0, P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED_P1)
                | P_Fld(0x0, SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED_P1)
                | P_Fld(0x0, SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
                | P_Fld(0x0, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
                | P_Fld(0x0, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(0x1, SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED_P1)
                | P_Fld(0x7, SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED)
                | P_Fld(0x1, SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED_P1)
                | P_Fld(0x7, SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED)
                | P_Fld(0x1, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
                | P_Fld(0x7, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
                | P_Fld(0x1, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
                | P_Fld(0x7, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3)
                | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2)
                | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1)
                | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0)
                | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_DQ3)
                | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_DQ2)
                | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_DQ1)
                | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3)
                | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2)
                | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1)
                | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0)
                | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_DQM3)
                | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_DQM2)
                | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_DQM1)
                | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ2, P_Fld(0x6, SHURK0_SELPH_DQ2_DLY_OEN_DQ3)
                | P_Fld(0x6, SHURK0_SELPH_DQ2_DLY_OEN_DQ2)
                | P_Fld(0x6, SHURK0_SELPH_DQ2_DLY_OEN_DQ1)
                | P_Fld(0x6, SHURK0_SELPH_DQ2_DLY_OEN_DQ0)
                | P_Fld(0x7, SHURK0_SELPH_DQ2_DLY_DQ3)
                | P_Fld(0x7, SHURK0_SELPH_DQ2_DLY_DQ2)
                | P_Fld(0x7, SHURK0_SELPH_DQ2_DLY_DQ1)
                | P_Fld(0x7, SHURK0_SELPH_DQ2_DLY_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ3, P_Fld(0x6, SHURK0_SELPH_DQ3_DLY_OEN_DQM3)
                | P_Fld(0x6, SHURK0_SELPH_DQ3_DLY_OEN_DQM2)
                | P_Fld(0x6, SHURK0_SELPH_DQ3_DLY_OEN_DQM1)
                | P_Fld(0x6, SHURK0_SELPH_DQ3_DLY_OEN_DQM0)
                | P_Fld(0x7, SHURK0_SELPH_DQ3_DLY_DQM3)
                | P_Fld(0x7, SHURK0_SELPH_DQ3_DLY_DQM2)
                | P_Fld(0x7, SHURK0_SELPH_DQ3_DLY_DQM1)
                | P_Fld(0x7, SHURK0_SELPH_DQ3_DLY_DQM0));
    vIO32WriteFldAlign(DRAMC_REG_SHURK1_DQSCTL, 0x2, SHURK1_DQSCTL_R1DQSINCTL);
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN1, P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B2_R1RODTEN_P1)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B2_R1RODTEN)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN_P1)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN_P1)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0, P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS3_GATED_P1)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS3_GATED)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS2_GATED_P1)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS2_GATED)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1, P_Fld(0x3, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS3_GATED_P1)
                | P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS3_GATED)
                | P_Fld(0x3, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS2_GATED_P1)
                | P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS2_GATED)
                | P_Fld(0x3, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
                | P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED)
                | P_Fld(0x3, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1)
                | P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3)
                | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2)
                | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1)
                | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0)
                | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1DQ3)
                | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1DQ2)
                | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1DQ1)
                | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3)
                | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2)
                | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1)
                | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0)
                | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1DQM3)
                | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1DQM2)
                | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1DQM1)
                | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ2, P_Fld(0x6, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ3)
                | P_Fld(0x6, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ2)
                | P_Fld(0x6, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ1)
                | P_Fld(0x6, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ0)
                | P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1DQ3)
                | P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1DQ2)
                | P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1DQ1)
                | P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ3, P_Fld(0x6, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM3)
                | P_Fld(0x6, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM2)
                | P_Fld(0x6, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM1)
                | P_Fld(0x6, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM0)
                | P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1DQM3)
                | P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1DQM2)
                | P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1DQM1)
                | P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1DQM0));
    ////DRAMC1-SHU3
    ////DDRPHY0-SHU3
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5, 0x7, SHU1_B1_DQ5_RG_RX_ARDQS0_DVS_DLY_B1);
    //vIO32WriteFldAlign(DDRPHY_SHU1_PLL5, 0x1e00, SHU1_PLL5_RG_RPHYPLL_SDM_PCW);
    //vIO32WriteFldAlign(DDRPHY_SHU1_PLL7, 0x1e00, SHU1_PLL7_RG_RCLRPLL_SDM_PCW);
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ6, P_Fld(0x26, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0x1e, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ0, P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ7_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ6_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ5_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ4_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ3_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ2_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ1_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ0_DLY_B1));
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_B1_DQ1, 0x0, SHU1_R1_B1_DQ1_RK1_TX_ARDQM0_DLY_B1);
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ6, P_Fld(0x27, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0x1f, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_R_DLY_B1));
    ////DDRPHY1-SHU3
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ5+((U32)CHANNEL_B<<POS_BANK_NUM), 0x7, SHU1_B0_DQ5_RG_RX_ARDQS0_DVS_DLY_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5+((U32)CHANNEL_B<<POS_BANK_NUM), 0x7, SHU1_B1_DQ5_RG_RX_ARDQS0_DVS_DLY_B1);
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD5+((U32)CHANNEL_B<<POS_BANK_NUM), 0x7, SHU1_CA_CMD5_RG_RX_ARCLK_DVS_DLY);
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ2+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_F_DLY_B0));
    vIO32WriteFldAlign(DDRPHY_SHU1_R0_B0_DQ3+((U32)CHANNEL_B<<POS_BANK_NUM), 0xa, SHU1_R0_B0_DQ3_RK0_RX_ARDQ2_F_DLY_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_R0_B0_DQ4+((U32)CHANNEL_B<<POS_BANK_NUM), 0xa, SHU1_R0_B0_DQ4_RK0_RX_ARDQ4_F_DLY_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_R0_B0_DQ5+((U32)CHANNEL_B<<POS_BANK_NUM), 0xa, SHU1_R0_B0_DQ5_RK0_RX_ARDQ6_F_DLY_B0);
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ6+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x26, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_F_DLY_B0)
                | P_Fld(0x1e, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_F_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ6+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x26, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0x1e, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD8+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x26, SHU1_R0_CA_CMD8_RG_RK0_RX_ARCLK_F_DLY)
                | P_Fld(0x1e, SHU1_R0_CA_CMD8_RG_RK0_RX_ARCLK_R_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ0+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ7_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ6_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ5_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ4_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ3_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ2_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ1_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ0_DLY_B0));
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_B0_DQ1+((U32)CHANNEL_B<<POS_BANK_NUM), 0x0, SHU1_R1_B0_DQ1_RK1_TX_ARDQM0_DLY_B0);
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ6+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x27, SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_F_DLY_B0)
                | P_Fld(0x1f, SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ0+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ7_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ6_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ5_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ4_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ3_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ2_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ1_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ0_DLY_B1));
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_B1_DQ1+((U32)CHANNEL_B<<POS_BANK_NUM), 0x0, SHU1_R1_B1_DQ1_RK1_TX_ARDQM0_DLY_B1);
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ6+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x27, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0x1f, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD0+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0xb, SHU1_R1_CA_CMD0_RK1_TX_ARCA5_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD0_RK1_TX_ARCA4_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD0_RK1_TX_ARCA3_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD0_RK1_TX_ARCA2_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD0_RK1_TX_ARCA1_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD0_RK1_TX_ARCA0_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD1+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0xb, SHU1_R1_CA_CMD1_RK1_TX_ARCKE2_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD1_RK1_TX_ARCKE1_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD1_RK1_TX_ARCKE0_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD8+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x27, SHU1_R1_CA_CMD8_RG_RK1_RX_ARCLK_F_DLY)
                | P_Fld(0x1f, SHU1_R1_CA_CMD8_RG_RK1_RX_ARCLK_R_DLY));
    vIO32WriteFldAlign(DDRPHY_SHU1_R2_B0_DQ1+((U32)CHANNEL_B<<POS_BANK_NUM), 0x0, SHU1_R2_B0_DQ1_RK2_TX_ARDQM0_DLY_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_R2_B1_DQ1+((U32)CHANNEL_B<<POS_BANK_NUM), 0x0, SHU1_R2_B1_DQ1_RK2_TX_ARDQM0_DLY_B1);

}
#endif

static void DramcSetting_Olympus_LP3(DRAMC_CTX_T *p)
{
    AutoRefreshCKEOff(p);
#if (fcFOR_CHIP_ID == fcBianco)                                        //before switch clock from 26M to PHY, need to init PHY clock first
    vIO32WriteFldAlign(DDRPHY_CKMUX_SEL, 0x1, CKMUX_SEL_R_PHYCTRLMUX); //move CKMUX_SEL_R_PHYCTRLMUX to here (it was originally between MISC_CG_CTRL0_CLK_MEM_SEL and MISC_CTRL0_R_DMRDSEL_DIV2_OPT)
#endif

    //chg_mem_en = 1
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 0x1, MISC_CG_CTRL0_W_CHG_MEM);

    //26M
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 0x0, MISC_CG_CTRL0_CLK_MEM_SEL); 

    
    vIO32WriteFldAlign(DDRPHY_MISC_CTRL0, 0x1, MISC_CTRL0_R_DMRDSEL_DIV2_OPT);
    vIO32WriteFldAlign(DDRPHY_MISC_CTRL0+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, MISC_CTRL0_R_DMRDSEL_DIV2_OPT);
    
    vIO32WriteFldMulti(DDRPHY_MISC_SPM_CTRL1, P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10) | P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_B0)
                                                   | P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_B1) | P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_CA));
    vIO32WriteFldMulti(DDRPHY_MISC_SPM_CTRL1+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10) | P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_B0)
                                                   | P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_B1) | P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_CA));  
 
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL0, 0xffffffff, MISC_SPM_CTRL0_PHY_SPM_CTL0);
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL0+((U32)CHANNEL_B<<POS_BANK_NUM), 0xffffffff, MISC_SPM_CTRL0_PHY_SPM_CTL0);
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL2, 0xffffffff, MISC_SPM_CTRL2_PHY_SPM_CTL2);
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL2+((U32)CHANNEL_B<<POS_BANK_NUM), 0xffffffff, MISC_SPM_CTRL2_PHY_SPM_CTL2);
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL2, 0x6003bf, MISC_CG_CTRL2_RG_MEM_DCM_CTL);
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL4, 0x13300000, MISC_CG_CTRL4_R_PHY_MCK_CG_CTRL);
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL4+((U32)CHANNEL_B<<POS_BANK_NUM), 0x222a2a00, MISC_CG_CTRL4_R_PHY_MCK_CG_CTRL);
    vIO32WriteFldMulti(DDRPHY_SHU1_PLL1, P_Fld(0x1, SHU1_PLL1_R_SHU_AUTO_PLL_MUX)
                | P_Fld(0x7, SHU1_PLL1_SHU1_PLL1_RFU));
    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7, P_Fld(0x1, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0)
                | P_Fld(0x1, SHU1_B0_DQ7_MIDPI_DIV4_ENABLE)
                | P_Fld(0x0, SHU1_B0_DQ7_R_DMRANKRXDVS_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ7, P_Fld(0x1, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1)
                | P_Fld(0x1, SHU1_B1_DQ7_R_DMRANKRXDVS_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0)
                | P_Fld(0x1, SHU1_B0_DQ7_MIDPI_DIV4_ENABLE)
                | P_Fld(0x1, SHU1_B0_DQ7_R_DMRANKRXDVS_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ7+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1)
                | P_Fld(0x1, SHU1_B1_DQ7_R_DMRANKRXDVS_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD7+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, SHU1_CA_CMD7_R_DMRXDVS_PBYTE_FLAG_OPT_CA)
                | P_Fld(0x1, SHU1_CA_CMD7_R_DMRANKRXDVS_CA));
    //vIO32WriteFldAlign(DDRPHY_SHU1_PLL4, 0xfe, SHU1_PLL4_RG_RPHYPLL_RESERVED);
    //vIO32WriteFldAlign(DDRPHY_SHU1_PLL6, 0xfe, SHU1_PLL6_RG_RCLRPLL_RESERVED);
    vIO32WriteFldMulti(DDRPHY_CA_CMD2, P_Fld(0x0, CA_CMD2_RG_TX_ARCMD_OE_DIS)
                | P_Fld(0x0, CA_CMD2_RG_TX_ARCMD_ODTEN_DIS)
                | P_Fld(0x0, CA_CMD2_RG_TX_ARCLK_OE_DIS)
                | P_Fld(0x0, CA_CMD2_RG_TX_ARCLK_ODTEN_DIS));
    vIO32WriteFldMulti(DDRPHY_B0_DQ2, P_Fld(0x0, B0_DQ2_RG_TX_ARDQ_OE_DIS_B0)
                | P_Fld(0x0, B0_DQ2_RG_TX_ARDQ_ODTEN_DIS_B0)
                | P_Fld(0x0, B0_DQ2_RG_TX_ARDQS_OE_DIS_B0)
                | P_Fld(0x0, B0_DQ2_RG_TX_ARDQS_ODTEN_DIS_B0));
    vIO32WriteFldMulti(DDRPHY_B1_DQ2, P_Fld(0x0, B1_DQ2_RG_TX_ARDQ_OE_DIS_B1)
                | P_Fld(0x0, B1_DQ2_RG_TX_ARDQ_ODTEN_DIS_B1)
                | P_Fld(0x0, B1_DQ2_RG_TX_ARDQS_OE_DIS_B1)
                | P_Fld(0x0, B1_DQ2_RG_TX_ARDQS_ODTEN_DIS_B1));
    vIO32WriteFldAlign(DDRPHY_B0_DQ9, 0x7, B0_DQ9_R_IN_GATE_EN_LOW_OPT_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ9, 0x7, B1_DQ9_R_IN_GATE_EN_LOW_OPT_B1);
    vIO32WriteFldAlign(DDRPHY_CA_CMD10, 0x7, CA_CMD10_R_IN_GATE_EN_LOW_OPT_CA); 
    vIO32WriteFldMulti(DDRPHY_CA_CMD2+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x0, CA_CMD2_RG_TX_ARCMD_OE_DIS)
                | P_Fld(0x0, CA_CMD2_RG_TX_ARCMD_ODTEN_DIS)
                | P_Fld(0x0, CA_CMD2_RG_TX_ARCLK_OE_DIS)
                | P_Fld(0x0, CA_CMD2_RG_TX_ARCLK_ODTEN_DIS));
    vIO32WriteFldMulti(DDRPHY_B0_DQ2+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x0, B0_DQ2_RG_TX_ARDQ_OE_DIS_B0)
                | P_Fld(0x0, B0_DQ2_RG_TX_ARDQ_ODTEN_DIS_B0)
                | P_Fld(0x0, B0_DQ2_RG_TX_ARDQS_OE_DIS_B0)
                | P_Fld(0x0, B0_DQ2_RG_TX_ARDQS_ODTEN_DIS_B0));
    vIO32WriteFldMulti(DDRPHY_B1_DQ2+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x0, B1_DQ2_RG_TX_ARDQ_OE_DIS_B1)
                | P_Fld(0x0, B1_DQ2_RG_TX_ARDQ_ODTEN_DIS_B1)
                | P_Fld(0x0, B1_DQ2_RG_TX_ARDQS_OE_DIS_B1)
                | P_Fld(0x0, B1_DQ2_RG_TX_ARDQS_ODTEN_DIS_B1));
    vIO32WriteFldAlign(DDRPHY_B0_DQ9+((U32)CHANNEL_B<<POS_BANK_NUM), 0x7, B0_DQ9_R_IN_GATE_EN_LOW_OPT_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ9+((U32)CHANNEL_B<<POS_BANK_NUM), 0x7, B1_DQ9_R_IN_GATE_EN_LOW_OPT_B1);
    vIO32WriteFldAlign(DDRPHY_CA_CMD10+((U32)CHANNEL_B<<POS_BANK_NUM), 0x7, CA_CMD10_R_IN_GATE_EN_LOW_OPT_CA);    
    vIO32WriteFldMulti(DDRPHY_MISC_CTRL1, P_Fld(0x1, MISC_CTRL1_R_DMPINMUX)
                | P_Fld(0x0, MISC_CTRL1_R_DMARPIDQ_SW)
                | P_Fld(0x1, MISC_CTRL1_R_DMMCTLPLL_CKSEL));
    vIO32WriteFldMulti(DDRPHY_MISC_CTRL1+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, MISC_CTRL1_R_DMPINMUX)
                | P_Fld(0x0, MISC_CTRL1_R_DMARPIDQ_SW)
                | P_Fld(0x1, MISC_CTRL1_R_DMMCTLPLL_CKSEL));

    vIO32WriteFldAlign(DDRPHY_PLL3, 0x0, PLL3_RG_RPHYPLL_TSTOP_EN);
	
    vIO32WriteFldAlign(DDRPHY_MISC_VREF_CTRL, 0x1, MISC_VREF_CTRL_RG_RVREF_VREF_EN);
    vIO32WriteFldMulti(DDRPHY_SHU1_MISC0, P_Fld(0x0, SHU1_MISC0_RG_RVREF_SEL_CMD)
                | P_Fld(0x1, SHU1_MISC0_RG_RVREF_DDR3_SEL)
                | P_Fld(0x0, SHU1_MISC0_RG_RVREF_DDR4_SEL)
                | P_Fld(0xb, SHU1_MISC0_RG_RVREF_SEL_DQ));
    vIO32WriteFldAlign(DDRPHY_MISC_IMP_CTRL0, 0x1, MISC_IMP_CTRL0_RG_RIMP_DDR3_SEL);
    vIO32WriteFldAlign(DDRPHY_MISC_VREF_CTRL+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, MISC_VREF_CTRL_RG_RVREF_VREF_EN);
    vIO32WriteFldMulti(DDRPHY_SHU1_MISC0+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x0, SHU1_MISC0_RG_RVREF_SEL_CMD)
                | P_Fld(0x1, SHU1_MISC0_RG_RVREF_DDR3_SEL)
                | P_Fld(0x0, SHU1_MISC0_RG_RVREF_DDR4_SEL)
                | P_Fld(0xb, SHU1_MISC0_RG_RVREF_SEL_DQ));
    vIO32WriteFldAlign(DDRPHY_MISC_IMP_CTRL0+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, MISC_IMP_CTRL0_RG_RIMP_DDR3_SEL);
    vIO32WriteFldAlign(DDRPHY_B1_RXDVS0, 0x1, B1_RXDVS0_R_RX_DLY_TRACK_CG_EN_B1);
    vIO32WriteFldMulti(DDRPHY_B0_RXDVS0+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, B0_RXDVS0_R_RX_DLY_TRACK_CG_EN_B0)
                | P_Fld(0x0, B0_RXDVS0_R_RX_RANKINCTL_B0));
    vIO32WriteFldMulti(DDRPHY_CA_RXDVS0+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, CA_RXDVS0_R_RX_DLY_TRACK_CG_EN_CA)
                | P_Fld(0x0, CA_RXDVS0_R_RX_RANKINCTL_CA));
    vIO32WriteFldMulti(DDRPHY_B1_RXDVS0+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, B1_RXDVS0_R_RX_DLY_TRACK_CG_EN_B1)
                | P_Fld(0x0, B1_RXDVS0_R_RX_RANKINCTL_B1));
    vIO32WriteFldMulti(DDRPHY_B1_RXDVS0, P_Fld(0x1, B1_RXDVS0_R_DMRXDVS_CNTCMP_OPT_B1)
                | P_Fld(0x1, B1_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_B1));
    vIO32WriteFldMulti(DDRPHY_B0_RXDVS0+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, B0_RXDVS0_R_DMRXDVS_CNTCMP_OPT_B0)
                | P_Fld(0x1, B0_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_B0));
    vIO32WriteFldMulti(DDRPHY_B1_RXDVS0+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, B1_RXDVS0_R_DMRXDVS_CNTCMP_OPT_B1)
                | P_Fld(0x1, B1_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_B1));
    vIO32WriteFldMulti(DDRPHY_CA_RXDVS0+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, CA_RXDVS0_R_DMRXDVS_CNTCMP_OPT_CA)
                | P_Fld(0x1, CA_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_CA));
    vIO32WriteFldAlign(DDRPHY_R0_B1_RXDVS2, 0x1, R0_B1_RXDVS2_R_RK0_DVS_FDLY_MODE_B1);
    vIO32WriteFldAlign(DDRPHY_R1_B1_RXDVS2, 0x1, R1_B1_RXDVS2_R_RK1_DVS_FDLY_MODE_B1);
    vIO32WriteFldAlign(DDRPHY_R0_B0_RXDVS2+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, R0_B0_RXDVS2_R_RK0_DVS_FDLY_MODE_B0);
    vIO32WriteFldAlign(DDRPHY_R1_B0_RXDVS2+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, R1_B0_RXDVS2_R_RK1_DVS_FDLY_MODE_B0);
	
    vIO32WriteFldAlign(DDRPHY_R0_CA_RXDVS2+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, R0_CA_RXDVS2_R_RK0_DVS_FDLY_MODE_CA);
    vIO32WriteFldAlign(DDRPHY_R1_CA_RXDVS2+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, R1_CA_RXDVS2_R_RK1_DVS_FDLY_MODE_CA);
    vIO32WriteFldAlign(DDRPHY_R0_B1_RXDVS2+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, R0_B1_RXDVS2_R_RK0_DVS_FDLY_MODE_B1);
    vIO32WriteFldAlign(DDRPHY_R1_B1_RXDVS2+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, R1_B1_RXDVS2_R_RK1_DVS_FDLY_MODE_B1);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5, 0x4, SHU1_B1_DQ5_RG_RX_ARDQS_DVS_DLY_B1);
    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ5+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x4, SHU1_B0_DQ5_RG_RX_ARDQS_DVS_DLY_B0)
                | P_Fld(0x0, SHU1_B0_DQ5_RG_RX_ARDQ_VREF_SEL_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD5+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x4, SHU1_CA_CMD5_RG_RX_ARCLK_DVS_DLY)
                | P_Fld(0x0, SHU1_CA_CMD5_RG_RX_ARCMD_VREF_SEL));
    vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ5+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x4, SHU1_B1_DQ5_RG_RX_ARDQS_DVS_DLY_B1)
                | P_Fld(0x0, SHU1_B1_DQ5_RG_RX_ARDQ_VREF_SEL_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ2, P_Fld(0x12, SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ3, P_Fld(0x12, SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ4, P_Fld(0x12, SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ5, P_Fld(0x12, SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ6, P_Fld(0x12, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ2, P_Fld(0x13, SHU1_R1_B1_DQ2_RK1_RX_ARDQ1_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ2_RK1_RX_ARDQ1_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ2_RK1_RX_ARDQ0_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ2_RK1_RX_ARDQ0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ3, P_Fld(0x13, SHU1_R1_B1_DQ3_RK1_RX_ARDQ3_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ3_RK1_RX_ARDQ3_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ3_RK1_RX_ARDQ2_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ3_RK1_RX_ARDQ2_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ4, P_Fld(0x13, SHU1_R1_B1_DQ4_RK1_RX_ARDQ5_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ4_RK1_RX_ARDQ5_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ4_RK1_RX_ARDQ4_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ4_RK1_RX_ARDQ4_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ5, P_Fld(0x13, SHU1_R1_B1_DQ5_RK1_RX_ARDQ7_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ5_RK1_RX_ARDQ7_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ5_RK1_RX_ARDQ6_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ5_RK1_RX_ARDQ6_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ6, P_Fld(0x13, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ6_RK1_RX_ARDQM0_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ6_RK1_RX_ARDQM0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ2+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_R_DLY_B0)
                | P_Fld(0x12, SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ3+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_B0_DQ3_RK0_RX_ARDQ3_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ3_RK0_RX_ARDQ3_R_DLY_B0)
                | P_Fld(0x12, SHU1_R0_B0_DQ3_RK0_RX_ARDQ2_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ3_RK0_RX_ARDQ2_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ4+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_B0_DQ4_RK0_RX_ARDQ5_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ4_RK0_RX_ARDQ5_R_DLY_B0)
                | P_Fld(0x12, SHU1_R0_B0_DQ4_RK0_RX_ARDQ4_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ4_RK0_RX_ARDQ4_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ5+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_B0_DQ5_RK0_RX_ARDQ7_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ5_RK0_RX_ARDQ7_R_DLY_B0)
                | P_Fld(0x12, SHU1_R0_B0_DQ5_RK0_RX_ARDQ6_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ5_RK0_RX_ARDQ6_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ6+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0)
                | P_Fld(0x12, SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ2+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x13, SHU1_R1_B0_DQ2_RK1_RX_ARDQ1_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ2_RK1_RX_ARDQ1_R_DLY_B0)
                | P_Fld(0x13, SHU1_R1_B0_DQ2_RK1_RX_ARDQ0_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ2_RK1_RX_ARDQ0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ3+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x13, SHU1_R1_B0_DQ3_RK1_RX_ARDQ3_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ3_RK1_RX_ARDQ3_R_DLY_B0)
                | P_Fld(0x13, SHU1_R1_B0_DQ3_RK1_RX_ARDQ2_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ3_RK1_RX_ARDQ2_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ4+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x13, SHU1_R1_B0_DQ4_RK1_RX_ARDQ5_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ4_RK1_RX_ARDQ5_R_DLY_B0)
                | P_Fld(0x13, SHU1_R1_B0_DQ4_RK1_RX_ARDQ4_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ4_RK1_RX_ARDQ4_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ5+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x13, SHU1_R1_B0_DQ5_RK1_RX_ARDQ7_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ5_RK1_RX_ARDQ7_R_DLY_B0)
                | P_Fld(0x13, SHU1_R1_B0_DQ5_RK1_RX_ARDQ6_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ5_RK1_RX_ARDQ6_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ6+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x13, SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_R_DLY_B0)
                | P_Fld(0x13, SHU1_R1_B0_DQ6_RK1_RX_ARDQM0_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ6_RK1_RX_ARDQM0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD2+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_CA_CMD2_RG_RK0_RX_ARCA1_F_DLY)
                | P_Fld(0xa, SHU1_R0_CA_CMD2_RG_RK0_RX_ARCA1_R_DLY)
                | P_Fld(0x12, SHU1_R0_CA_CMD2_RG_RK0_RX_ARCA0_F_DLY)
                | P_Fld(0xa, SHU1_R0_CA_CMD2_RG_RK0_RX_ARCA0_R_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD3+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_CA_CMD3_RG_RK0_RX_ARCA3_F_DLY)
                | P_Fld(0xa, SHU1_R0_CA_CMD3_RG_RK0_RX_ARCA3_R_DLY)
                | P_Fld(0x12, SHU1_R0_CA_CMD3_RG_RK0_RX_ARCA2_F_DLY)
                | P_Fld(0xa, SHU1_R0_CA_CMD3_RG_RK0_RX_ARCA2_R_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD4+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_CA_CMD4_RG_RK0_RX_ARCA5_F_DLY)
                | P_Fld(0xa, SHU1_R0_CA_CMD4_RG_RK0_RX_ARCA5_R_DLY)
                | P_Fld(0x12, SHU1_R0_CA_CMD4_RG_RK0_RX_ARCA4_F_DLY)
                | P_Fld(0xa, SHU1_R0_CA_CMD4_RG_RK0_RX_ARCA4_R_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD5+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_CA_CMD5_RG_RK0_RX_ARCKE1_F_DLY)
                | P_Fld(0xa, SHU1_R0_CA_CMD5_RG_RK0_RX_ARCKE1_R_DLY)
                | P_Fld(0x12, SHU1_R0_CA_CMD5_RG_RK0_RX_ARCKE0_F_DLY)
                | P_Fld(0xa, SHU1_R0_CA_CMD5_RG_RK0_RX_ARCKE0_R_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD6+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_CA_CMD6_RG_RK0_RX_ARCS0_F_DLY)
                | P_Fld(0xa, SHU1_R0_CA_CMD6_RG_RK0_RX_ARCS0_R_DLY)
                | P_Fld(0x12, SHU1_R0_CA_CMD6_RG_RK0_RX_ARCKE2_F_DLY)
                | P_Fld(0xa, SHU1_R0_CA_CMD6_RG_RK0_RX_ARCKE2_R_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD7+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_CA_CMD7_RG_RK0_RX_ARCS2_F_DLY)
                | P_Fld(0xa, SHU1_R0_CA_CMD7_RG_RK0_RX_ARCS2_R_DLY)
                | P_Fld(0x12, SHU1_R0_CA_CMD7_RG_RK0_RX_ARCS1_F_DLY)
                | P_Fld(0xa, SHU1_R0_CA_CMD7_RG_RK0_RX_ARCS1_R_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD8+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_CA_CMD8_RG_RK0_RX_ARCLK_F_DLY)
                | P_Fld(0xa, SHU1_R0_CA_CMD8_RG_RK0_RX_ARCLK_R_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD2+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x13, SHU1_R1_CA_CMD2_RG_RK1_RX_ARCA1_F_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD2_RG_RK1_RX_ARCA1_R_DLY)
                | P_Fld(0x13, SHU1_R1_CA_CMD2_RG_RK1_RX_ARCA0_F_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD2_RG_RK1_RX_ARCA0_R_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD3+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x13, SHU1_R1_CA_CMD3_RG_RK1_RX_ARCA3_F_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD3_RG_RK1_RX_ARCA3_R_DLY)
                | P_Fld(0x13, SHU1_R1_CA_CMD3_RG_RK1_RX_ARCA2_F_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD3_RG_RK1_RX_ARCA2_R_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD4+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x13, SHU1_R1_CA_CMD4_RG_RK1_RX_ARCA5_F_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD4_RG_RK1_RX_ARCA5_R_DLY)
                | P_Fld(0x13, SHU1_R1_CA_CMD4_RG_RK1_RX_ARCA4_F_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD4_RG_RK1_RX_ARCA4_R_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD5+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x13, SHU1_R1_CA_CMD5_RG_RK1_RX_ARCKE1_F_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD5_RG_RK1_RX_ARCKE1_R_DLY)
                | P_Fld(0x13, SHU1_R1_CA_CMD5_RG_RK1_RX_ARCKE0_F_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD5_RG_RK1_RX_ARCKE0_R_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD6+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x13, SHU1_R1_CA_CMD6_RG_RK1_RX_ARCS0_F_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD6_RG_RK1_RX_ARCS0_R_DLY)
                | P_Fld(0x13, SHU1_R1_CA_CMD6_RG_RK1_RX_ARCKE2_F_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD6_RG_RK1_RX_ARCKE2_R_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD7+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x13, SHU1_R1_CA_CMD7_RG_RK1_RX_ARCS2_F_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD7_RG_RK1_RX_ARCS2_R_DLY)
                | P_Fld(0x13, SHU1_R1_CA_CMD7_RG_RK1_RX_ARCS1_F_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD7_RG_RK1_RX_ARCS1_R_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD8+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x13, SHU1_R1_CA_CMD8_RG_RK1_RX_ARCLK_F_DLY)
                | P_Fld(0xb, SHU1_R1_CA_CMD8_RG_RK1_RX_ARCLK_R_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ2+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ3+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ4+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ5+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ6+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x12, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ2+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x13, SHU1_R1_B1_DQ2_RK1_RX_ARDQ1_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ2_RK1_RX_ARDQ1_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ2_RK1_RX_ARDQ0_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ2_RK1_RX_ARDQ0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ3+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x13, SHU1_R1_B1_DQ3_RK1_RX_ARDQ3_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ3_RK1_RX_ARDQ3_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ3_RK1_RX_ARDQ2_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ3_RK1_RX_ARDQ2_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ4+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x13, SHU1_R1_B1_DQ4_RK1_RX_ARDQ5_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ4_RK1_RX_ARDQ5_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ4_RK1_RX_ARDQ4_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ4_RK1_RX_ARDQ4_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ5+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x13, SHU1_R1_B1_DQ5_RK1_RX_ARDQ7_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ5_RK1_RX_ARDQ7_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ5_RK1_RX_ARDQ6_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ5_RK1_RX_ARDQ6_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ6+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x13, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ6_RK1_RX_ARDQM0_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ6_RK1_RX_ARDQM0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_R0_B1_RXDVS1, P_Fld(0x4, R0_B1_RXDVS1_R_RK0_B1_DVS_TH_LEAD)
                | P_Fld(0x4, R0_B1_RXDVS1_R_RK0_B1_DVS_TH_LAG));
    vIO32WriteFldMulti(DDRPHY_R1_B1_RXDVS1, P_Fld(0x4, R1_B1_RXDVS1_R_RK1_B1_DVS_TH_LEAD)
                | P_Fld(0x4, R1_B1_RXDVS1_R_RK1_B1_DVS_TH_LAG));
    vIO32WriteFldMulti(DDRPHY_R0_B0_RXDVS1+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x4, R0_B0_RXDVS1_R_RK0_B0_DVS_TH_LEAD)
                | P_Fld(0x4, R0_B0_RXDVS1_R_RK0_B0_DVS_TH_LAG));
    vIO32WriteFldMulti(DDRPHY_R1_B0_RXDVS1+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x4, R1_B0_RXDVS1_R_RK1_B0_DVS_TH_LEAD)
                | P_Fld(0x4, R1_B0_RXDVS1_R_RK1_B0_DVS_TH_LAG));
    vIO32WriteFldMulti(DDRPHY_R0_CA_RXDVS1+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x4, R0_CA_RXDVS1_R_RK0_CA_DVS_TH_LEAD)
                | P_Fld(0x4, R0_CA_RXDVS1_R_RK0_CA_DVS_TH_LAG));
    vIO32WriteFldMulti(DDRPHY_R1_CA_RXDVS1+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x4, R1_CA_RXDVS1_R_RK1_CA_DVS_TH_LEAD)
                | P_Fld(0x4, R1_CA_RXDVS1_R_RK1_CA_DVS_TH_LAG));
    vIO32WriteFldMulti(DDRPHY_R0_B1_RXDVS1+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x4, R0_B1_RXDVS1_R_RK0_B1_DVS_TH_LEAD)
                | P_Fld(0x4, R0_B1_RXDVS1_R_RK0_B1_DVS_TH_LAG));
    vIO32WriteFldMulti(DDRPHY_R1_B1_RXDVS1+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x4, R1_B1_RXDVS1_R_RK1_B1_DVS_TH_LEAD)
                | P_Fld(0x4, R1_B1_RXDVS1_R_RK1_B1_DVS_TH_LAG));

    vIO32WriteFldMulti(DDRPHY_R0_B1_RXDVS2, P_Fld(0x2, R0_B1_RXDVS2_R_RK0_DVS_MODE_B1)
                | P_Fld(0x1, R0_B1_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B1)
                | P_Fld(0x1, R0_B1_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B1));
    vIO32WriteFldMulti(DDRPHY_R1_B1_RXDVS2, P_Fld(0x2, R1_B1_RXDVS2_R_RK1_DVS_MODE_B1)
                | P_Fld(0x1, R1_B1_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_B1)
                | P_Fld(0x1, R1_B1_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_B1));
    vIO32WriteFldMulti(DDRPHY_R0_B0_RXDVS2+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x2, R0_B0_RXDVS2_R_RK0_DVS_MODE_B0)
                | P_Fld(0x1, R0_B0_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B0)
                | P_Fld(0x1, R0_B0_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B0));
    vIO32WriteFldMulti(DDRPHY_R1_B0_RXDVS2+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x2, R1_B0_RXDVS2_R_RK1_DVS_MODE_B0)
                | P_Fld(0x1, R1_B0_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_B0)
                | P_Fld(0x1, R1_B0_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_B0));
	vIO32WriteFldMulti(DDRPHY_R0_CA_RXDVS2+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x2, R0_CA_RXDVS2_R_RK0_DVS_MODE_CA)
                | P_Fld(0x1, R0_CA_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_CA)
                | P_Fld(0x1, R0_CA_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_CA));
    vIO32WriteFldMulti(DDRPHY_R1_CA_RXDVS2+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x2, R1_CA_RXDVS2_R_RK1_DVS_MODE_CA)
                | P_Fld(0x1, R1_CA_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_CA)
                | P_Fld(0x1, R1_CA_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_CA));
    vIO32WriteFldMulti(DDRPHY_R0_B1_RXDVS2+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x2, R0_B1_RXDVS2_R_RK0_DVS_MODE_B1)
                | P_Fld(0x1, R0_B1_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B1)
                | P_Fld(0x1, R0_B1_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B1));
    vIO32WriteFldMulti(DDRPHY_R1_B1_RXDVS2+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x2, R1_B1_RXDVS2_R_RK1_DVS_MODE_B1)
                | P_Fld(0x1, R1_B1_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_B1)
                | P_Fld(0x1, R1_B1_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_B1));

    vIO32WriteFldAlign(DDRPHY_B1_RXDVS0, 0x0, B1_RXDVS0_R_RX_DLY_TRACK_CG_EN_B1);
    vIO32WriteFldAlign(DDRPHY_B0_RXDVS0+((U32)CHANNEL_B<<POS_BANK_NUM), 0x0, B0_RXDVS0_R_RX_DLY_TRACK_CG_EN_B0);
	vIO32WriteFldAlign(DDRPHY_CA_RXDVS0+((U32)CHANNEL_B<<POS_BANK_NUM), 0x0, CA_RXDVS0_R_RX_DLY_TRACK_CG_EN_CA);
	vIO32WriteFldAlign(DDRPHY_B1_RXDVS0+((U32)CHANNEL_B<<POS_BANK_NUM), 0x0, B1_RXDVS0_R_RX_DLY_TRACK_CG_EN_B1);
    vIO32WriteFldAlign(DDRPHY_B1_DQ9, 0x1, B1_DQ9_RG_RX_ARDQ_STBEN_RESETB_B1);
    vIO32WriteFldAlign(DDRPHY_B0_DQ9+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, B0_DQ9_RG_RX_ARDQ_STBEN_RESETB_B0);
    vIO32WriteFldAlign(DDRPHY_CA_CMD10+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, CA_CMD10_RG_RX_ARCMD_STBEN_RESETB);
    vIO32WriteFldAlign(DDRPHY_B1_DQ9+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, B1_DQ9_RG_RX_ARDQ_STBEN_RESETB_B1);
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL1, 0x3f600, MISC_CG_CTRL1_R_DVS_DIV4_CG_CTRL);
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL1+((U32)CHANNEL_B<<POS_BANK_NUM), 0xff7befdf, MISC_CG_CTRL1_R_DVS_DIV4_CG_CTRL);
    vIO32WriteFldAlign(DDRPHY_SHU1_R0_CA_CMD9, 0xc, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CMD);
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_CA_CMD9, 0xc, SHU1_R1_CA_CMD9_RG_RK1_ARPI_CMD);
    //vIO32WriteFldAlign(DDRPHY_SHU2_R0_CA_CMD9, 0xc, SHU2_R0_CA_CMD9_RG_RK0_ARPI_CMD);
    //vIO32WriteFldAlign(DDRPHY_SHU2_R1_CA_CMD9, 0xc, SHU2_R1_CA_CMD9_RG_RK1_ARPI_CMD);
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ0, P_Fld(0x6, SHU1_R1_B1_DQ0_RK1_TX_ARDQ7_DLY_B1)
                | P_Fld(0x6, SHU1_R1_B1_DQ0_RK1_TX_ARDQ6_DLY_B1)
                | P_Fld(0x6, SHU1_R1_B1_DQ0_RK1_TX_ARDQ5_DLY_B1)
                | P_Fld(0x6, SHU1_R1_B1_DQ0_RK1_TX_ARDQ4_DLY_B1)
                | P_Fld(0x6, SHU1_R1_B1_DQ0_RK1_TX_ARDQ3_DLY_B1)
                | P_Fld(0x6, SHU1_R1_B1_DQ0_RK1_TX_ARDQ2_DLY_B1)
                | P_Fld(0x6, SHU1_R1_B1_DQ0_RK1_TX_ARDQ1_DLY_B1)
                | P_Fld(0x6, SHU1_R1_B1_DQ0_RK1_TX_ARDQ0_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B1_DQ0, P_Fld(0xb, SHU1_R2_B1_DQ0_RK2_TX_ARDQ7_DLY_B1)
                | P_Fld(0xb, SHU1_R2_B1_DQ0_RK2_TX_ARDQ6_DLY_B1)
                | P_Fld(0xb, SHU1_R2_B1_DQ0_RK2_TX_ARDQ5_DLY_B1)
                | P_Fld(0xb, SHU1_R2_B1_DQ0_RK2_TX_ARDQ4_DLY_B1)
                | P_Fld(0xb, SHU1_R2_B1_DQ0_RK2_TX_ARDQ3_DLY_B1)
                | P_Fld(0xb, SHU1_R2_B1_DQ0_RK2_TX_ARDQ2_DLY_B1)
                | P_Fld(0xb, SHU1_R2_B1_DQ0_RK2_TX_ARDQ1_DLY_B1)
                | P_Fld(0xb, SHU1_R2_B1_DQ0_RK2_TX_ARDQ0_DLY_B1));
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_B1_DQ1, 0x6, SHU1_R1_B1_DQ1_RK1_TX_ARDQM0_DLY_B1);
    vIO32WriteFldAlign(DDRPHY_SHU1_R2_B1_DQ1, 0xb, SHU1_R2_B1_DQ1_RK2_TX_ARDQM0_DLY_B1);
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD0+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x6, SHU1_R1_CA_CMD0_RK1_TX_ARCA5_DLY)
                | P_Fld(0x6, SHU1_R1_CA_CMD0_RK1_TX_ARCA4_DLY)
                | P_Fld(0x6, SHU1_R1_CA_CMD0_RK1_TX_ARCA3_DLY)
                | P_Fld(0x6, SHU1_R1_CA_CMD0_RK1_TX_ARCA2_DLY)
                | P_Fld(0x6, SHU1_R1_CA_CMD0_RK1_TX_ARCA1_DLY)
                | P_Fld(0x6, SHU1_R1_CA_CMD0_RK1_TX_ARCA0_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_CA_CMD0+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0xb, SHU1_R2_CA_CMD0_RK2_TX_ARCA5_DLY)
                | P_Fld(0xb, SHU1_R2_CA_CMD0_RK2_TX_ARCA4_DLY)
                | P_Fld(0xb, SHU1_R2_CA_CMD0_RK2_TX_ARCA3_DLY)
                | P_Fld(0xb, SHU1_R2_CA_CMD0_RK2_TX_ARCA2_DLY)
                | P_Fld(0xb, SHU1_R2_CA_CMD0_RK2_TX_ARCA1_DLY)
                | P_Fld(0xb, SHU1_R2_CA_CMD0_RK2_TX_ARCA0_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD1+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x6, SHU1_R1_CA_CMD1_RK1_TX_ARCKE2_DLY)
                | P_Fld(0x6, SHU1_R1_CA_CMD1_RK1_TX_ARCKE1_DLY)
                | P_Fld(0x6, SHU1_R1_CA_CMD1_RK1_TX_ARCKE0_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_CA_CMD1+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0xb, SHU1_R2_CA_CMD1_RK2_TX_ARCKE2_DLY)
                | P_Fld(0xb, SHU1_R2_CA_CMD1_RK2_TX_ARCKE1_DLY)
                | P_Fld(0xb, SHU1_R2_CA_CMD1_RK2_TX_ARCKE0_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ0+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ7_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ6_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ5_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ4_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ3_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ2_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ1_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ0_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B0_DQ0+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0xb, SHU1_R2_B0_DQ0_RK2_TX_ARDQ7_DLY_B0)
                | P_Fld(0xb, SHU1_R2_B0_DQ0_RK2_TX_ARDQ6_DLY_B0)
                | P_Fld(0xb, SHU1_R2_B0_DQ0_RK2_TX_ARDQ5_DLY_B0)
                | P_Fld(0xb, SHU1_R2_B0_DQ0_RK2_TX_ARDQ4_DLY_B0)
                | P_Fld(0xb, SHU1_R2_B0_DQ0_RK2_TX_ARDQ3_DLY_B0)
                | P_Fld(0xb, SHU1_R2_B0_DQ0_RK2_TX_ARDQ2_DLY_B0)
                | P_Fld(0xb, SHU1_R2_B0_DQ0_RK2_TX_ARDQ1_DLY_B0)
                | P_Fld(0xb, SHU1_R2_B0_DQ0_RK2_TX_ARDQ0_DLY_B0));
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_B0_DQ1+((U32)CHANNEL_B<<POS_BANK_NUM), 0x6, SHU1_R1_B0_DQ1_RK1_TX_ARDQM0_DLY_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_R2_B0_DQ1+((U32)CHANNEL_B<<POS_BANK_NUM), 0xb, SHU1_R2_B0_DQ1_RK2_TX_ARDQM0_DLY_B0);
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ0+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ7_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ6_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ5_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ4_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ3_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ2_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ1_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ0_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B1_DQ0+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0xb, SHU1_R2_B1_DQ0_RK2_TX_ARDQ7_DLY_B1)
                | P_Fld(0xb, SHU1_R2_B1_DQ0_RK2_TX_ARDQ6_DLY_B1)
                | P_Fld(0xb, SHU1_R2_B1_DQ0_RK2_TX_ARDQ5_DLY_B1)
                | P_Fld(0xb, SHU1_R2_B1_DQ0_RK2_TX_ARDQ4_DLY_B1)
                | P_Fld(0xb, SHU1_R2_B1_DQ0_RK2_TX_ARDQ3_DLY_B1)
                | P_Fld(0xb, SHU1_R2_B1_DQ0_RK2_TX_ARDQ2_DLY_B1)
                | P_Fld(0xb, SHU1_R2_B1_DQ0_RK2_TX_ARDQ1_DLY_B1)
                | P_Fld(0xb, SHU1_R2_B1_DQ0_RK2_TX_ARDQ0_DLY_B1));
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_B1_DQ1+((U32)CHANNEL_B<<POS_BANK_NUM), 0x6, SHU1_R1_B1_DQ1_RK1_TX_ARDQM0_DLY_B1);
    vIO32WriteFldAlign(DDRPHY_SHU1_R2_B1_DQ1+((U32)CHANNEL_B<<POS_BANK_NUM), 0xb, SHU1_R2_B1_DQ1_RK2_TX_ARDQM0_DLY_B1);
    vIO32WriteFldAlign(DDRPHY_SHU1_R0_B0_DQ7, 0xc, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_B0_DQ7, 0xc, SHU1_R1_B0_DQ7_RK1_ARPI_DQ_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_R2_B0_DQ7, 0xc, SHU1_R2_B0_DQ7_RK2_ARPI_DQ_B0);
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ7, P_Fld(0xf, SHU1_R0_B1_DQ7_RK0_ARPI_DQM_B1)
                | P_Fld(0xf, SHU1_R0_B1_DQ7_RK0_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ7, P_Fld(0xf, SHU1_R1_B1_DQ7_RK1_ARPI_DQM_B1)
                | P_Fld(0xf, SHU1_R1_B1_DQ7_RK1_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B1_DQ7, P_Fld(0xf, SHU1_R2_B1_DQ7_RK2_ARPI_DQM_B1)
                | P_Fld(0xf, SHU1_R2_B1_DQ7_RK2_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD9+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0xf, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CMD)
                | P_Fld(0xf, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CS));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD9+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0xf, SHU1_R1_CA_CMD9_RG_RK1_ARPI_CMD)
                | P_Fld(0xf, SHU1_R1_CA_CMD9_RG_RK1_ARPI_CS));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_CA_CMD9+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0xf, SHU1_R2_CA_CMD9_RG_RK2_ARPI_CMD)
                | P_Fld(0xf, SHU1_R2_CA_CMD9_RG_RK2_ARPI_CS));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ7+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0xf, SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0)
                | P_Fld(0xf, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ7+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0xf, SHU1_R1_B0_DQ7_RK1_ARPI_DQM_B0)
                | P_Fld(0xf, SHU1_R1_B0_DQ7_RK1_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B0_DQ7+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0xf, SHU1_R2_B0_DQ7_RK2_ARPI_DQM_B0)
                | P_Fld(0xf, SHU1_R2_B0_DQ7_RK2_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ7+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0xf, SHU1_R0_B1_DQ7_RK0_ARPI_DQM_B1)
                | P_Fld(0xf, SHU1_R0_B1_DQ7_RK0_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ7+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0xf, SHU1_R1_B1_DQ7_RK1_ARPI_DQM_B1)
                | P_Fld(0xf, SHU1_R1_B1_DQ7_RK1_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B1_DQ7+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0xf, SHU1_R2_B1_DQ7_RK2_ARPI_DQM_B1)
                | P_Fld(0xf, SHU1_R2_B1_DQ7_RK2_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_B1_DQ4, P_Fld(0x10, B1_DQ4_RG_RX_ARDQS_EYE_R_DLY_B1)
                | P_Fld(0x10, B1_DQ4_RG_RX_ARDQS_EYE_F_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_B1_DQ5, P_Fld(0x1, B1_DQ5_RG_RX_ARDQ_EYE_EN_B1)
                | P_Fld(0x1, B1_DQ5_RG_RX_ARDQ_EYE_SEL_B1)
                | P_Fld(0x1, B1_DQ5_RG_RX_ARDQ_VREF_EN_B1)
                | P_Fld(0x10, B1_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B1)
                | P_Fld(0x10, B1_DQ5_B1_DQ5_RFU));
    vIO32WriteFldMulti(DDRPHY_B1_DQ6, P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_EYE_DLY_DQS_BYPASS_B1)
                | P_Fld(0x0, B1_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B1)
                | P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1)
                | P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B1));
    vIO32WriteFldMulti(DDRPHY_B1_DQ5, P_Fld(0x1, B1_DQ5_RG_RX_ARDQ_EYE_STBEN_RESETB_B1)
                | P_Fld(0x0, B1_DQ5_B1_DQ5_RFU));
    vIO32WriteFldMulti(DDRPHY_CA_CMD3, P_Fld(0x1, CA_CMD3_RG_RX_ARCMD_IN_BUFF_EN)
                | P_Fld(0x1, CA_CMD3_RG_ARCMD_RESETB)
                | P_Fld(0x1, CA_CMD3_RG_TX_ARCMD_EN));
    vIO32WriteFldMulti(DDRPHY_CA_CMD6, P_Fld(0x1, CA_CMD6_RG_TX_ARCMD_DDR3_SEL)
                | P_Fld(0x0, CA_CMD6_RG_TX_ARCMD_DDR4_SEL)
                | P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_DDR4_SEL)
                | P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_BIAS_VREF_SEL)
                | P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_RES_BIAS_EN)
                | P_Fld(0x1, CA_CMD6_RG_TX_ARCMD_SER_MODE));
    vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD1, P_Fld(0x1, SHU1_CA_CMD1_RG_TX_ARCMD_DRVN)
                | P_Fld(0x1, SHU1_CA_CMD1_RG_TX_ARCMD_DRVP));
    vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD2, P_Fld(0x1, SHU1_CA_CMD2_RG_TX_ARCLK_DRVN)
                | P_Fld(0x1, SHU1_CA_CMD2_RG_TX_ARCLK_DRVP));
    //vIO32WriteFldMulti(DDRPHY_SHU2_CA_CMD1, P_Fld(0x1, SHU2_CA_CMD1_RG_TX_ARCMD_DRVN)
    //            | P_Fld(0x1, SHU2_CA_CMD1_RG_TX_ARCMD_DRVP));
    //vIO32WriteFldMulti(DDRPHY_SHU2_CA_CMD2, P_Fld(0x1, SHU2_CA_CMD2_RG_TX_ARCLK_DRVN)
    //            | P_Fld(0x1, SHU2_CA_CMD2_RG_TX_ARCLK_DRVP));
    vIO32WriteFldMulti(DDRPHY_B0_DQ3, P_Fld(0x1, B0_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B0)
                | P_Fld(0x1, B0_DQ3_RG_ARDQ_RESETB_B0)
                | P_Fld(0x1, B0_DQ3_RG_TX_ARDQ_EN_B0));
    vIO32WriteFldMulti(DDRPHY_B0_DQ6, P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_DDR3_SEL_B0)
                | P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B0)
                | P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_RES_BIAS_EN_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ1, P_Fld(0x1, SHU1_B0_DQ1_RG_TX_ARDQ_DRVN_B0)
                | P_Fld(0x1, SHU1_B0_DQ1_RG_TX_ARDQ_DRVP_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ2, P_Fld(0x1, SHU1_B0_DQ2_RG_TX_ARDQS_DRVN_B0)
                | P_Fld(0x1, SHU1_B0_DQ2_RG_TX_ARDQS_DRVP_B0));
    //vIO32WriteFldMulti(DDRPHY_SHU2_B0_DQ1, P_Fld(0x1, SHU2_B0_DQ1_RG_TX_ARDQ_DRVN_B0)
    //           | P_Fld(0x1, SHU2_B0_DQ1_RG_TX_ARDQ_DRVP_B0));
    //vIO32WriteFldMulti(DDRPHY_SHU2_B0_DQ2, P_Fld(0x1, SHU2_B0_DQ2_RG_TX_ARDQS0_DRVN_B0)
    //            | P_Fld(0x1, SHU2_B0_DQ2_RG_TX_ARDQS0_DRVP_B0));
    vIO32WriteFldMulti(DDRPHY_CA_CMD4+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x10, CA_CMD4_RG_RX_ARCLK_EYE_R_DLY)
                | P_Fld(0x10, CA_CMD4_RG_RX_ARCLK_EYE_F_DLY));
    vIO32WriteFldMulti(DDRPHY_CA_CMD5+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, CA_CMD5_RG_RX_ARCMD_EYE_EN)
                | P_Fld(0x1, CA_CMD5_RG_RX_ARCMD_EYE_SEL)
                | P_Fld(0x1, CA_CMD5_RG_RX_ARCMD_VREF_EN)
                | P_Fld(0x10, CA_CMD5_RG_RX_ARCMD_EYE_VREF_SEL)
                | P_Fld(0x10, CA_CMD5_CA_CMD5_RFU));
    vIO32WriteFldMulti(DDRPHY_CA_CMD6+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, CA_CMD6_RG_RX_ARCMD_EYE_DLY_DQS_BYPASS)
                | P_Fld(0x1, CA_CMD6_RG_TX_ARCMD_DDR3_SEL)
                | P_Fld(0x1, CA_CMD6_RG_RX_ARCMD_DDR3_SEL)
                | P_Fld(0x0, CA_CMD6_RG_TX_ARCMD_DDR4_SEL)
                | P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_DDR4_SEL)
                | P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_BIAS_VREF_SEL)
                | P_Fld(0x1, CA_CMD6_RG_RX_ARCMD_BIAS_EN)
                | P_Fld(0x1, CA_CMD6_RG_RX_ARCMD_OP_BIAS_SW_EN)
                | P_Fld(0x1, CA_CMD6_RG_TX_ARCMD_SER_MODE));
    vIO32WriteFldMulti(DDRPHY_CA_CMD5+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, CA_CMD5_RG_RX_ARCMD_EYE_STBEN_RESETB)
                | P_Fld(0x0, CA_CMD5_CA_CMD5_RFU));
    vIO32WriteFldMulti(DDRPHY_B0_DQ4+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x10, B0_DQ4_RG_RX_ARDQS_EYE_R_DLY_B0)
                | P_Fld(0x10, B0_DQ4_RG_RX_ARDQS_EYE_F_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_B0_DQ5+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x0, B0_DQ5_RG_RX_ARDQ_EYE_EN_B0)
                | P_Fld(0x1, B0_DQ5_RG_RX_ARDQ_EYE_SEL_B0)
                | P_Fld(0x1, B0_DQ5_RG_RX_ARDQ_VREF_EN_B0)
                | P_Fld(0x10, B0_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B0)
                | P_Fld(0x10, B0_DQ5_B0_DQ5_RFU));
    vIO32WriteFldMulti(DDRPHY_B0_DQ6+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, B0_DQ6_RG_RX_ARDQ_EYE_DLY_DQS_BYPASS_B0)
                | P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B0)
                | P_Fld(0x1, B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0)
                | P_Fld(0x1, B0_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B0));
    vIO32WriteFldMulti(DDRPHY_B0_DQ5+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, B0_DQ5_RG_RX_ARDQ_EYE_STBEN_RESETB_B0)
                | P_Fld(0x0, B0_DQ5_B0_DQ5_RFU));
    vIO32WriteFldMulti(DDRPHY_B1_DQ4+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x10, B1_DQ4_RG_RX_ARDQS_EYE_R_DLY_B1)
                | P_Fld(0x10, B1_DQ4_RG_RX_ARDQS_EYE_F_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_B1_DQ5+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x0, B1_DQ5_RG_RX_ARDQ_EYE_EN_B1)
                | P_Fld(0x1, B1_DQ5_RG_RX_ARDQ_EYE_SEL_B1)
                | P_Fld(0x1, B1_DQ5_RG_RX_ARDQ_VREF_EN_B1)
                | P_Fld(0x10, B1_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B1)
                | P_Fld(0x10, B1_DQ5_B1_DQ5_RFU));
    vIO32WriteFldMulti(DDRPHY_B1_DQ6+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_EYE_DLY_DQS_BYPASS_B1)
                | P_Fld(0x0, B1_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B1)
                | P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1)
                | P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B1));
    vIO32WriteFldMulti(DDRPHY_B1_DQ5+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, B1_DQ5_RG_RX_ARDQ_EYE_STBEN_RESETB_B1)
                | P_Fld(0x0, B1_DQ5_B1_DQ5_RFU));
#if 0 // SDM_PCW are set in DDRPhyPLLSetting()
    if(p->frequency==933)
    {
        vIO32WriteFldAlign(DDRPHY_SHU1_PLL5, 0x4700, SHU1_PLL5_RG_RPHYPLL_SDM_PCW);
        vIO32WriteFldAlign(DDRPHY_SHU1_PLL7, 0x4700, SHU1_PLL7_RG_RCLRPLL_SDM_PCW);
    }
    else if(p->frequency==667)
    {
        vIO32WriteFldAlign(DDRPHY_SHU1_PLL5, 0x3300, SHU1_PLL5_RG_RPHYPLL_SDM_PCW);
        vIO32WriteFldAlign(DDRPHY_SHU1_PLL7, 0x3300, SHU1_PLL7_RG_RCLRPLL_SDM_PCW);
    }
    else if(p->frequency==467)
    {
        vIO32WriteFldAlign(DDRPHY_SHU1_PLL5, 0x4700>>1, SHU1_PLL5_RG_RPHYPLL_SDM_PCW);
        vIO32WriteFldAlign(DDRPHY_SHU1_PLL7, 0x4700>>1, SHU1_PLL7_RG_RCLRPLL_SDM_PCW);
    }
#endif
    vIO32WriteFldMulti(DDRPHY_B0_DLL_ARPI2, P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_CG_FB_B0)
                | P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0));
    vIO32WriteFldMulti(DDRPHY_B0_DLL_ARPI2+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_CG_FB_B0)
                | P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0));
    vIO32WriteFldMulti(DDRPHY_B1_DLL_ARPI2, P_Fld(0x1, B1_DLL_ARPI2_RG_ARPI_CG_FB_B1)
                | P_Fld(0x1, B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1));
    vIO32WriteFldMulti(DDRPHY_B1_DLL_ARPI2+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, B1_DLL_ARPI2_RG_ARPI_CG_FB_B1)
                | P_Fld(0x1, B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1));
    vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI2, P_Fld(0x1, CA_DLL_ARPI2_RG_ARPI_CG_FB_CA)
                | P_Fld(0x1, CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA));
    vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI2+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, CA_DLL_ARPI2_RG_ARPI_CG_FB_CA)
                | P_Fld(0x1, CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA));
    mcDELAY_US(1);

    vIO32WriteFldMulti(DDRPHY_PLL4, P_Fld(0x0, PLL4_RG_RPHYPLL_AD_MCK8X_EN)
                | P_Fld(0x1, PLL4_PLL4_RFU)
                | P_Fld(0x1, PLL4_RG_RPHYPLL_MCK8X_SEL));


#if 0//ORIGINAL_PLL_INIT
    vIO32WriteFldAlign(DDRPHY_PLL1, 0x1, PLL1_RG_RPHYPLL_EN);
    mcDELAY_US(20);

    ///TODO: MIDPI
    vIO32WriteFldMulti(DDRPHY_PLL4, P_Fld(0x1, PLL4_RG_RPHYPLL_ADA_MCK8X_EN)
                | P_Fld(0x1, PLL4_RG_RPHYPLL_RESETB));
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI0, 0x1, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);
    vIO32WriteFldAlign(DDRPHY_B0_DLL_ARPI0, 0x1, B0_DLL_ARPI0_RG_ARPI_RESETB_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DLL_ARPI0, 0x1, B1_DLL_ARPI0_RG_ARPI_RESETB_B1);
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI0+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);
    vIO32WriteFldAlign(DDRPHY_B0_DLL_ARPI0+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, B0_DLL_ARPI0_RG_ARPI_RESETB_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DLL_ARPI0+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, B1_DLL_ARPI0_RG_ARPI_RESETB_B1);

    mcDELAY_US(1);

    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ6, 0x1, SHU1_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU2_B0_DQ6, 0x1, SHU2_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ6, 0x1, SHU1_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1);
    //vIO32WriteFldAlign(DDRPHY_SHU2_B1_DQ6, 0x1, SHU2_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1);
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD6, 0x1, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA);
    //vIO32WriteFldAlign(DDRPHY_SHU2_CA_CMD6, 0x1, SHU2_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA);
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ6+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, SHU1_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU2_B0_DQ6+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, SHU2_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ6+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, SHU1_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1);
    //vIO32WriteFldAlign(DDRPHY_SHU2_B1_DQ6+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, SHU2_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1);
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD6+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA);
    //vIO32WriteFldAlign(DDRPHY_SHU2_CA_CMD6+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, SHU2_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA);
    vIO32WriteFldAlign(DDRPHY_SHU1_PLL8, 0x0, SHU1_PLL8_RG_RPHYPLL_POSDIV);
    //vIO32WriteFldAlign(DDRPHY_SHU2_PLL8, 0x0, SHU2_PLL8_RG_RPHYPLL_POSDIV);
    vIO32WriteFldAlign(DDRPHY_SHU1_PLL10, 0x0, SHU1_PLL10_RG_RCLRPLL_POSDIV);
    //vIO32WriteFldAlign(DDRPHY_SHU2_PLL10, 0x0, SHU2_PLL10_RG_RCLRPLL_POSDIV);
#endif
    vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI1, P_Fld(0x1, CA_DLL_ARPI1_RG_ARPISM_MCK_SEL_CA)
                | P_Fld(0x1, CA_DLL_ARPI1_RG_ARPI_CS_JUMP_EN)
                | P_Fld(0x1, CA_DLL_ARPI1_RG_ARPI_CLK_JUMP_EN));
    vIO32WriteFldMulti(DDRPHY_B0_DLL_ARPI1, P_Fld(0x1, B0_DLL_ARPI1_RG_ARPISM_MCK_SEL_B0)
                | P_Fld(0x1, B0_DLL_ARPI1_RG_ARPI_DQ_JUMP_EN_B0));
    vIO32WriteFldAlign(DDRPHY_B1_DLL_ARPI1, 0x1, B1_DLL_ARPI1_RG_ARPI_DQ_JUMP_EN_B1);
    vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI2, P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_FB_CA)
                | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA));
    vIO32WriteFldMulti(DDRPHY_B0_DLL_ARPI2, P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_FB_B0)
                | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0));
    vIO32WriteFldMulti(DDRPHY_B1_DLL_ARPI2, P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_FB_B1)
                | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1));
    vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI3, P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_MCTL_EN_CA)
                | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_FB_EN_CA)
                | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CS_EN)
                | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CLK_EN)
                | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CMD_EN));
    vIO32WriteFldMulti(DDRPHY_B0_DLL_ARPI3, P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_FB_EN_B0)
                | P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQS_EN_B0)
                | P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQM_EN_B0)
                | P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQ_EN_B0)
                | P_Fld(0x0, B0_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B0)); //Shih-shiu: Vinson LP3 doesn't use CHA DQS0 pad, set to 0 to save power
    vIO32WriteFldMulti(DDRPHY_B1_DLL_ARPI3, P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_FB_EN_B1)
                | P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQS_EN_B1)
                | P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQM_EN_B1)
                | P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQ_EN_B1)
                | P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B1));
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI1+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, CA_DLL_ARPI1_RG_ARPI_CMD_JUMP_EN);
    vIO32WriteFldAlign(DDRPHY_B0_DLL_ARPI1+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, B0_DLL_ARPI1_RG_ARPI_DQ_JUMP_EN_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DLL_ARPI1+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, B1_DLL_ARPI1_RG_ARPI_DQ_JUMP_EN_B1);
    vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI2+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_FB_CA)
                | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA));
    vIO32WriteFldMulti(DDRPHY_B0_DLL_ARPI2+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_FB_B0)
                | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0));
    vIO32WriteFldMulti(DDRPHY_B1_DLL_ARPI2+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_FB_B1)
                | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1));
    vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI3+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_FB_EN_CA)
                | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CS_EN)
                | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CLK_EN)
                | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CMD_EN)
                | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CLKIEN_EN));
    vIO32WriteFldMulti(DDRPHY_B0_DLL_ARPI3+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_FB_EN_B0)
                | P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQS_EN_B0)
                | P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQM_EN_B0)
                | P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQ_EN_B0)
                | P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B0));
    vIO32WriteFldMulti(DDRPHY_CA_CMD3, P_Fld(0x1, CA_CMD3_RG_RX_ARCMD_STBENCMP_EN) 
                | P_Fld(0x1, CA_CMD3_RG_ARCMD_RESETB) 
                | P_Fld(0x1, CA_CMD3_RG_TX_ARCMD_EN) 
                | P_Fld(0x1, CA_CMD3_RG_RX_ARCMD_SMT_EN));
    vIO32WriteFldMulti(DDRPHY_B0_DQ3, P_Fld(0x1, B0_DQ3_RG_RX_ARDQ_STBENCMP_EN_B0) 
                | P_Fld(0x1, B0_DQ3_RG_ARDQ_RESETB_B0) 
                | P_Fld(0x1, B0_DQ3_RG_TX_ARDQ_EN_B0) 
                | P_Fld(0x1, B0_DQ3_RG_RX_ARDQ_SMT_EN_B0));
    vIO32WriteFldMulti(DDRPHY_B1_DLL_ARPI3+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_FB_EN_B1)
                | P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQS_EN_B1)
                | P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQM_EN_B1)
                | P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQ_EN_B1)
                | P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B1));
    vIO32WriteFldMulti(DDRPHY_B1_DQ3, P_Fld(0x1, B1_DQ3_RG_RX_ARDQ_STBENCMP_EN_B1)
                | P_Fld(0x1, B1_DQ3_RG_ARDQ_RESETB_B1)
                | P_Fld(0x1, B1_DQ3_RG_TX_ARDQ_EN_B1)
                | P_Fld(0x1, B1_DQ3_RG_RX_ARDQ_SMT_EN_B1));
    vIO32WriteFldMulti(DDRPHY_CA_CMD3+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, CA_CMD3_RG_RX_ARCMD_STBENCMP_EN)
                | P_Fld(0x1, CA_CMD3_RG_ARCMD_RESETB)
                | P_Fld(0x1, CA_CMD3_RG_TX_ARCMD_EN)
                | P_Fld(0x1, CA_CMD3_RG_RX_ARCMD_SMT_EN));
    vIO32WriteFldMulti(DDRPHY_B0_DQ3+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, B0_DQ3_RG_RX_ARDQ_STBENCMP_EN_B0)
                | P_Fld(0x1, B0_DQ3_RG_ARDQ_RESETB_B0)
                | P_Fld(0x1, B0_DQ3_RG_TX_ARDQ_EN_B0)
                | P_Fld(0x1, B0_DQ3_RG_RX_ARDQ_SMT_EN_B0));
    vIO32WriteFldMulti(DDRPHY_B1_DQ3+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, B1_DQ3_RG_RX_ARDQ_STBENCMP_EN_B1)
                | P_Fld(0x1, B1_DQ3_RG_ARDQ_RESETB_B1)
                | P_Fld(0x1, B1_DQ3_RG_TX_ARDQ_EN_B1)
                | P_Fld(0x1, B1_DQ3_RG_RX_ARDQ_SMT_EN_B1));
	
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI5, 0x0, CA_DLL_ARPI5_RG_ARDLL_DIV_MCTL_CA);
    vIO32WriteFldAlign(DDRPHY_B0_DLL_ARPI5, 0x0, B0_DLL_ARPI5_RG_ARDLL_DIV_MCTL_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DLL_ARPI5, 0x0, B1_DLL_ARPI5_RG_ARDLL_DIV_MCTL_B1);
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI5+((U32)CHANNEL_B<<POS_BANK_NUM), 0x0, CA_DLL_ARPI5_RG_ARDLL_DIV_MCTL_CA);
    vIO32WriteFldAlign(DDRPHY_B0_DLL_ARPI5+((U32)CHANNEL_B<<POS_BANK_NUM), 0x0, B0_DLL_ARPI5_RG_ARDLL_DIV_MCTL_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DLL_ARPI5+((U32)CHANNEL_B<<POS_BANK_NUM), 0x0, B1_DLL_ARPI5_RG_ARDLL_DIV_MCTL_B1);

    vIO32WriteFldMulti(DDRPHY_SHU1_CA_DLL0, P_Fld(0x9, SHU1_CA_DLL0_RG_ARDLL_IDLECNT_CA)
                | P_Fld(0x8, SHU1_CA_DLL0_RG_ARDLL_P_GAIN_CA)
                | P_Fld(0x1, SHU1_CA_DLL0_RG_ARDLL_PHJUMP_EN_CA)
                | P_Fld(0x1, SHU1_CA_DLL0_RG_ARDLL_PHDIV_CA));
    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DLL0, P_Fld(0x1, SHU1_B0_DLL0_RG_ARDLL_PHDET_IN_SWAP_B0)
                | P_Fld(0x7, SHU1_B0_DLL0_RG_ARDLL_GAIN_B0)
                | P_Fld(0x7, SHU1_B0_DLL0_RG_ARDLL_IDLECNT_B0)
                | P_Fld(0x8, SHU1_B0_DLL0_RG_ARDLL_P_GAIN_B0)
                | P_Fld(0x1, SHU1_B0_DLL0_RG_ARDLL_PHJUMP_EN_B0)
                | P_Fld(0x1, SHU1_B0_DLL0_RG_ARDLL_PHDIV_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_B1_DLL0, P_Fld(0x1, SHU1_B1_DLL0_RG_ARDLL_PHDET_IN_SWAP_B1)
                | P_Fld(0x7, SHU1_B1_DLL0_RG_ARDLL_GAIN_B1)
                | P_Fld(0x7, SHU1_B1_DLL0_RG_ARDLL_IDLECNT_B1)
                | P_Fld(0x8, SHU1_B1_DLL0_RG_ARDLL_P_GAIN_B1)
                | P_Fld(0x1, SHU1_B1_DLL0_RG_ARDLL_PHJUMP_EN_B1)
                | P_Fld(0x1, SHU1_B1_DLL0_RG_ARDLL_PHDIV_B1));
                
    vIO32WriteFldMulti(DDRPHY_SHU1_CA_DLL0+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, SHU1_CA_DLL0_RG_ARDLL_PHDET_OUT_SEL_CA)
                | P_Fld(0x1, SHU1_CA_DLL0_RG_ARDLL_PHDET_IN_SWAP_CA)
                | P_Fld(0x7, SHU1_CA_DLL0_RG_ARDLL_GAIN_CA)
                | P_Fld(0x7, SHU1_CA_DLL0_RG_ARDLL_IDLECNT_CA)
                | P_Fld(0x8, SHU1_CA_DLL0_RG_ARDLL_P_GAIN_CA)
                | P_Fld(0x1, SHU1_CA_DLL0_RG_ARDLL_PHJUMP_EN_CA)
                | P_Fld(0x1, SHU1_CA_DLL0_RG_ARDLL_PHDIV_CA));
    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DLL0+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, SHU1_B0_DLL0_RG_ARDLL_PHDET_IN_SWAP_B0)
                | P_Fld(0x7, SHU1_B0_DLL0_RG_ARDLL_GAIN_B0)
                | P_Fld(0x7, SHU1_B0_DLL0_RG_ARDLL_IDLECNT_B0)
                | P_Fld(0x8, SHU1_B0_DLL0_RG_ARDLL_P_GAIN_B0)
                | P_Fld(0x1, SHU1_B0_DLL0_RG_ARDLL_PHJUMP_EN_B0)
                | P_Fld(0x1, SHU1_B0_DLL0_RG_ARDLL_PHDIV_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_B1_DLL0+(1<<POS_BANK_NUM), P_Fld(0x1, SHU1_B1_DLL0_RG_ARDLL_PHDET_IN_SWAP_B1)
                | P_Fld(0x7, SHU1_B1_DLL0_RG_ARDLL_GAIN_B1)
                | P_Fld(0x7, SHU1_B1_DLL0_RG_ARDLL_IDLECNT_B1)
                | P_Fld(0x8, SHU1_B1_DLL0_RG_ARDLL_P_GAIN_B1)
                | P_Fld(0x1, SHU1_B1_DLL0_RG_ARDLL_PHJUMP_EN_B1)
                | P_Fld(0x1, SHU1_B1_DLL0_RG_ARDLL_PHDIV_B1));

    vIO32WriteFldMulti(DDRPHY_CA_CMD8, P_Fld(0x1, CA_CMD8_RG_TX_RRESETB_DDR3_SEL)
                | P_Fld(0x1, CA_CMD8_RG_RRESETB_DRVN)
                | P_Fld(0x1, CA_CMD8_RG_RRESETB_DRVP));
    vIO32WriteFldMulti(DDRPHY_CA_CMD8+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, CA_CMD8_RG_TX_RRESETB_DDR3_SEL)
                | P_Fld(0x1, CA_CMD8_RG_RRESETB_DRVN)
                | P_Fld(0x1, CA_CMD8_RG_RRESETB_DRVP));

    mcDELAY_US(1);

    vIO32WriteFldAlign(DDRPHY_SHU1_PLL0, 0x3, SHU1_PLL0_RG_RPHYPLL_TOP_REV);
    //vIO32WriteFldAlign(DDRPHY_SHU2_PLL0, 0x3, SHU2_PLL0_RG_RPHYPLL_TOP_REV);
    //vIO32WriteFldAlign(DDRPHY_SHU3_PLL0, 0x3, SHU3_PLL0_RG_RPHYPLL_TOP_REV);
    //vIO32WriteFldAlign(DDRPHY_SHU4_PLL0, 0x3, SHU4_PLL0_RG_RPHYPLL_TOP_REV);
    vIO32WriteFldAlign(DDRPHY_SHU1_PLL0+((U32)CHANNEL_B<<POS_BANK_NUM), 0x3, SHU1_PLL0_RG_RPHYPLL_TOP_REV);
    //vIO32WriteFldAlign(DDRPHY_SHU2_PLL0+((U32)CHANNEL_B<<POS_BANK_NUM), 0x3, SHU2_PLL0_RG_RPHYPLL_TOP_REV);
    //vIO32WriteFldAlign(DDRPHY_SHU3_PLL0+((U32)CHANNEL_B<<POS_BANK_NUM), 0x3, SHU3_PLL0_RG_RPHYPLL_TOP_REV);
    //vIO32WriteFldAlign(DDRPHY_SHU4_PLL0+((U32)CHANNEL_B<<POS_BANK_NUM), 0x3, SHU4_PLL0_RG_RPHYPLL_TOP_REV);
    vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD0, P_Fld(0x1, SHU1_CA_CMD0_RG_TX_ARCMD_PRE_EN)
                | P_Fld(0x7, SHU1_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
                | P_Fld(0x1, SHU1_CA_CMD0_RG_TX_ARCLK_PRE_EN));
    //vIO32WriteFldMulti(DDRPHY_SHU2_CA_CMD0, P_Fld(0x1, SHU2_CA_CMD0_RG_TX_ARCMD_PRE_EN)
    //            | P_Fld(0x7, SHU2_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
    //            | P_Fld(0x1, SHU2_CA_CMD0_RG_TX_ARCLK_PRE_EN));
    //vIO32WriteFldMulti(DDRPHY_SHU3_CA_CMD0, P_Fld(0x1, SHU3_CA_CMD0_RG_TX_ARCMD_PRE_EN)
    //            | P_Fld(0x7, SHU3_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
    //            | P_Fld(0x1, SHU3_CA_CMD0_RG_TX_ARCLK_PRE_EN));
    //vIO32WriteFldMulti(DDRPHY_SHU4_CA_CMD0, P_Fld(0x1, SHU4_CA_CMD0_RG_TX_ARCMD_PRE_EN)
    //            | P_Fld(0x7, SHU4_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
    //            | P_Fld(0x1, SHU4_CA_CMD0_RG_TX_ARCLK_PRE_EN));
    vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD0+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, SHU1_CA_CMD0_RG_TX_ARCMD_PRE_EN)
                | P_Fld(0x7, SHU1_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
                | P_Fld(0x1, SHU1_CA_CMD0_RG_TX_ARCLK_PRE_EN));
    //vIO32WriteFldMulti(DDRPHY_SHU2_CA_CMD0+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, SHU2_CA_CMD0_RG_TX_ARCMD_PRE_EN)
    //            | P_Fld(0x7, SHU2_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
    //            | P_Fld(0x1, SHU2_CA_CMD0_RG_TX_ARCLK_PRE_EN));
    //vIO32WriteFldMulti(DDRPHY_SHU3_CA_CMD0+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, SHU3_CA_CMD0_RG_TX_ARCMD_PRE_EN)
    //            | P_Fld(0x7, SHU3_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
    //            | P_Fld(0x1, SHU3_CA_CMD0_RG_TX_ARCLK_PRE_EN));
    //vIO32WriteFldMulti(DDRPHY_SHU4_CA_CMD0+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, SHU4_CA_CMD0_RG_TX_ARCMD_PRE_EN)
    //            | P_Fld(0x7, SHU4_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
    //            | P_Fld(0x1, SHU4_CA_CMD0_RG_TX_ARCLK_PRE_EN));
    //vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD6, 0x3, SHU1_CA_CMD6_RG_ARPI_RESERVE_CA);
    //vIO32WriteFldAlign(DDRPHY_SHU2_CA_CMD6, 0x3, SHU2_CA_CMD6_RG_ARPI_RESERVE_CA);
    //vIO32WriteFldAlign(DDRPHY_SHU3_CA_CMD6, 0x3, SHU3_CA_CMD6_RG_ARPI_RESERVE_CA);
    //vIO32WriteFldAlign(DDRPHY_SHU4_CA_CMD6, 0x3, SHU4_CA_CMD6_RG_ARPI_RESERVE_CA);
    //vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD6+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, SHU1_CA_CMD6_RG_ARPI_RESERVE_CA);
    //vIO32WriteFldAlign(DDRPHY_SHU2_CA_CMD6+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, SHU2_CA_CMD6_RG_ARPI_RESERVE_CA);
    //vIO32WriteFldAlign(DDRPHY_SHU3_CA_CMD6+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, SHU3_CA_CMD6_RG_ARPI_RESERVE_CA);
    //vIO32WriteFldAlign(DDRPHY_SHU4_CA_CMD6+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, SHU4_CA_CMD6_RG_ARPI_RESERVE_CA);
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD3, 0x7e2, SHU1_CA_CMD3_RG_ARCMD_REV);
    //vIO32WriteFldAlign(DDRPHY_SHU2_CA_CMD7, 0x7e2, SHU2_CA_CMD7_RG_ARCMD_REV);
    //vIO32WriteFldAlign(DDRPHY_SHU3_CA_CMD7, 0x7e2, SHU3_CA_CMD7_RG_ARCMD_REV);
    //vIO32WriteFldAlign(DDRPHY_SHU4_CA_CMD7, 0x7e2, SHU4_CA_CMD7_RG_ARCMD_REV);
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD3+((U32)CHANNEL_B<<POS_BANK_NUM), 0x720, SHU1_CA_CMD3_RG_ARCMD_REV);
    //vIO32WriteFldMulti(DDRPHY_SHU2_CA_CMD7+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x720, SHU2_CA_CMD7_RG_ARCMD_REV)
    //            | P_Fld(0x0, SHU2_CA_CMD7_R_DMRANKRXDVS_CA));
    //vIO32WriteFldMulti(DDRPHY_SHU3_CA_CMD7+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x720, SHU3_CA_CMD7_RG_ARCMD_REV)
    //            | P_Fld(0x0, SHU3_CA_CMD7_R_DMRANKRXDVS_CA));
    //vIO32WriteFldMulti(DDRPHY_SHU4_CA_CMD7+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x720, SHU4_CA_CMD7_RG_ARCMD_REV)
    //            | P_Fld(0x0, SHU4_CA_CMD7_R_DMRANKRXDVS_CA));
    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ3, P_Fld(0x20, SHU1_B0_DQ3_RG_ARDQ_REV_B0) 
                | P_Fld(0x1, SHU1_B0_DQ3_DQ_REV_B0_BIT_05));
    //vIO32WriteFldAlign(DDRPHY_SHU2_B0_DQ7, 0x20, SHU2_B0_DQ7_RG_ARDQ_REV_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU3_B0_DQ7, 0x20, SHU3_B0_DQ7_RG_ARDQ_REV_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU4_B0_DQ7, 0x20, SHU4_B0_DQ7_RG_ARDQ_REV_B0);
    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ3+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x20, SHU1_B0_DQ3_RG_ARDQ_REV_B0) 
                | P_Fld(0x1, SHU1_B0_DQ3_DQ_REV_B0_BIT_05));
    //vIO32WriteFldMulti(DDRPHY_SHU2_B0_DQ7+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x20, SHU2_B0_DQ7_RG_ARDQ_REV_B0)
    //            | P_Fld(0x0, SHU2_B0_DQ7_R_DMRANKRXDVS_B0));
    //vIO32WriteFldMulti(DDRPHY_SHU3_B0_DQ7+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x20, SHU3_B0_DQ7_RG_ARDQ_REV_B0)
    //            | P_Fld(0x0, SHU3_B0_DQ7_R_DMRANKRXDVS_B0));
    //vIO32WriteFldMulti(DDRPHY_SHU4_B0_DQ7+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x20, SHU4_B0_DQ7_RG_ARDQ_REV_B0)
    //            | P_Fld(0x0, SHU4_B0_DQ7_R_DMRANKRXDVS_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ3, P_Fld(0x20, SHU1_B1_DQ3_RG_ARDQ_REV_B1) 
                | P_Fld(0x1, SHU1_B1_DQ3_DQ_REV_B1_BIT_05));
    //vIO32WriteFldAlign(DDRPHY_SHU2_B1_DQ7, 0x20, SHU2_B1_DQ7_RG_ARDQ_REV_B1);
    //vIO32WriteFldAlign(DDRPHY_SHU3_B1_DQ7, 0x20, SHU3_B1_DQ7_RG_ARDQ_REV_B1);
    //vIO32WriteFldAlign(DDRPHY_SHU4_B1_DQ7, 0x20, SHU4_B1_DQ7_RG_ARDQ_REV_B1);
    vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ3+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x20, SHU1_B1_DQ3_RG_ARDQ_REV_B1) 
                | P_Fld(0x1, SHU1_B1_DQ3_DQ_REV_B1_BIT_05));
    //vIO32WriteFldMulti(DDRPHY_SHU2_B1_DQ7+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x20, SHU2_B1_DQ7_RG_ARDQ_REV_B1)
    //            | P_Fld(0x0, SHU2_B1_DQ7_R_DMRANKRXDVS_B1));
    //vIO32WriteFldMulti(DDRPHY_SHU3_B1_DQ7+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x20, SHU3_B1_DQ7_RG_ARDQ_REV_B1)
    //            | P_Fld(0x0, SHU3_B1_DQ7_R_DMRANKRXDVS_B1));
    //vIO32WriteFldMulti(DDRPHY_SHU4_B1_DQ7+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x20, SHU4_B1_DQ7_RG_ARDQ_REV_B1)
    //            | P_Fld(0x0, SHU4_B1_DQ7_R_DMRANKRXDVS_B1));
    //vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ6, 0x1, SHU1_B0_DQ6_RG_ARPI_RESERVE_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU2_B0_DQ6, 0x1, SHU2_B0_DQ6_RG_ARPI_RESERVE_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU3_B0_DQ6, 0x1, SHU3_B0_DQ6_RG_ARPI_RESERVE_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU4_B0_DQ6, 0x1, SHU4_B0_DQ6_RG_ARPI_RESERVE_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ6+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, SHU1_B0_DQ6_RG_ARPI_RESERVE_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU2_B0_DQ6+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, SHU2_B0_DQ6_RG_ARPI_RESERVE_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU3_B0_DQ6+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, SHU3_B0_DQ6_RG_ARPI_RESERVE_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU4_B0_DQ6+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, SHU4_B0_DQ6_RG_ARPI_RESERVE_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ6, 0x1, SHU1_B1_DQ6_RG_ARPI_RESERVE_B1);
    //vIO32WriteFldAlign(DDRPHY_SHU2_B1_DQ6, 0x1, SHU2_B1_DQ6_RG_ARPI_RESERVE_B1);
    //vIO32WriteFldAlign(DDRPHY_SHU3_B1_DQ6, 0x1, SHU3_B1_DQ6_RG_ARPI_RESERVE_B1);
    //vIO32WriteFldAlign(DDRPHY_SHU4_B1_DQ6, 0x1, SHU4_B1_DQ6_RG_ARPI_RESERVE_B1);
    //vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ6+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, SHU1_B1_DQ6_RG_ARPI_RESERVE_B1);
    //vIO32WriteFldAlign(DDRPHY_SHU2_B1_DQ6+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, SHU2_B1_DQ6_RG_ARPI_RESERVE_B1);
    //vIO32WriteFldAlign(DDRPHY_SHU3_B1_DQ6+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, SHU3_B1_DQ6_RG_ARPI_RESERVE_B1);
    //vIO32WriteFldAlign(DDRPHY_SHU4_B1_DQ6+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, SHU4_B1_DQ6_RG_ARPI_RESERVE_B1);
    vIO32WriteFldMulti(DDRPHY_MISC_SHU_OPT, P_Fld(0x1, MISC_SHU_OPT_R_CA_SHU_PHDET_SPM_EN)
                | P_Fld(0x1, MISC_SHU_OPT_R_CA_SHU_PHY_GATING_RESETB_SPM_EN)
                | P_Fld(0x2, MISC_SHU_OPT_R_DQB1_SHU_PHDET_SPM_EN)
                | P_Fld(0x1, MISC_SHU_OPT_R_DQB1_SHU_PHY_GATING_RESETB_SPM_EN)
                | P_Fld(0x2, MISC_SHU_OPT_R_DQB0_SHU_PHDET_SPM_EN)
                | P_Fld(0x1, MISC_SHU_OPT_R_DQB0_SHU_PHY_GATING_RESETB_SPM_EN));
    vIO32WriteFldMulti(DDRPHY_MISC_SHU_OPT+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x2, MISC_SHU_OPT_R_CA_SHU_PHDET_SPM_EN)
                | P_Fld(0x1, MISC_SHU_OPT_R_CA_SHU_PHY_GATING_RESETB_SPM_EN)
                | P_Fld(0x2, MISC_SHU_OPT_R_DQB1_SHU_PHDET_SPM_EN)
                | P_Fld(0x1, MISC_SHU_OPT_R_DQB1_SHU_PHY_GATING_RESETB_SPM_EN)
                | P_Fld(0x2, MISC_SHU_OPT_R_DQB0_SHU_PHDET_SPM_EN)
                | P_Fld(0x1, MISC_SHU_OPT_R_DQB0_SHU_PHY_GATING_RESETB_SPM_EN));

    mcDELAY_US(1);

    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ6, 0x1, SHU1_B0_DQ6_RG_ARPI_RESERVE_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ6, 0x1, SHU1_B1_DQ6_RG_ARPI_RESERVE_B1);
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD6, 0x3, SHU1_CA_CMD6_RG_ARPI_RESERVE_CA);
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ6+(1<<POS_BANK_NUM), 0x1, SHU1_B0_DQ6_RG_ARPI_RESERVE_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ6+(1<<POS_BANK_NUM), 0x1, SHU1_B1_DQ6_RG_ARPI_RESERVE_B1);
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD6+(1<<POS_BANK_NUM), 0x1, SHU1_CA_CMD6_RG_ARPI_RESERVE_CA);

    vIO32WriteFldAlign(DDRPHY_PLL3, 0x0, PLL3_RG_RPHYPLL_TSTOP_EN);
    vIO32WriteFldAlign(DDRPHY_PLL3+(1<<POS_BANK_NUM), 0x0, PLL3_RG_RPHYPLL_TSTOP_EN);
    
    vIO32WriteFldMulti(DDRPHY_SHU1_PLL9, P_Fld(0x0, SHU1_PLL9_RG_RPHYPLL_MONCK_EN) 
                | P_Fld(0x0, SHU1_PLL9_RG_RPHYPLL_MONVC_EN) 
                | P_Fld(0x0, SHU1_PLL9_RG_RPHYPLL_LVROD_EN) 
                | P_Fld(0x0, SHU1_PLL9_RG_RPHYPLL_RST_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_PLL9+(1<<POS_BANK_NUM), P_Fld(0x0, SHU1_PLL9_RG_RPHYPLL_MONCK_EN) 
                | P_Fld(0x0, SHU1_PLL9_RG_RPHYPLL_MONVC_EN) 
                | P_Fld(0x0, SHU1_PLL9_RG_RPHYPLL_LVROD_EN) 
                | P_Fld(0x0, SHU1_PLL9_RG_RPHYPLL_RST_DLY));
                
    vIO32WriteFldAlign(DDRPHY_SHU1_PLL5, 0x0, SHU1_PLL5_RG_RPHYPLL_SDM_FRA_EN);
    vIO32WriteFldAlign(DDRPHY_SHU1_PLL7, 0x0, SHU1_PLL5_RG_RPHYPLL_SDM_FRA_EN);  
    vIO32WriteFldAlign(DDRPHY_SHU1_PLL5+(1<<POS_BANK_NUM), 0x0, SHU1_PLL5_RG_RPHYPLL_SDM_FRA_EN);
    vIO32WriteFldAlign(DDRPHY_SHU1_PLL7+(1<<POS_BANK_NUM), 0x0, SHU1_PLL5_RG_RPHYPLL_SDM_FRA_EN);  

    vIO32WriteFldAlign(DDRPHY_SHU1_PLL4, 0xfe, SHU1_PLL4_RG_RPHYPLL_RESERVED);
    vIO32WriteFldAlign(DDRPHY_SHU1_PLL6, 0xfe, SHU1_PLL6_RG_RCLRPLL_RESERVED);    
    vIO32WriteFldAlign(DDRPHY_SHU1_PLL4+(1<<POS_BANK_NUM), 0xfe, SHU1_PLL4_RG_RPHYPLL_RESERVED);
    vIO32WriteFldAlign(DDRPHY_SHU1_PLL6+(1<<POS_BANK_NUM), 0xfe, SHU1_PLL6_RG_RCLRPLL_RESERVED); 


    mcDELAY_US(1);


#if 0//ORIGINAL_PLL_INIT
    ///TODO: PLL_EN
    vIO32WriteFldAlign(DDRPHY_PLL1, 0x1, PLL1_RG_RPHYPLL_EN);
    vIO32WriteFldAlign(DDRPHY_PLL2, 0x1, PLL2_RG_RCLRPLL_EN);

    mcDELAY_US(100);
#endif

    vIO32WriteFldAlign(DDRPHY_SHU1_CA_DLL1, 0x1, SHU1_CA_DLL1_RG_ARDLL_PD_CK_SEL_CA);
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DLL1, 0x1, SHU1_B0_DLL1_RG_ARDLL_FASTPJ_CK_SEL_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DLL1, 0x1, SHU1_B1_DLL1_RG_ARDLL_FASTPJ_CK_SEL_B1);
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_DLL1+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, SHU1_CA_DLL1_RG_ARDLL_FASTPJ_CK_SEL_CA);
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DLL1+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, SHU1_B0_DLL1_RG_ARDLL_FASTPJ_CK_SEL_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DLL1+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, SHU1_B1_DLL1_RG_ARDLL_FASTPJ_CK_SEL_B1);
    mcDELAY_US(1);


    //vIO32WriteFldAlign(DDRPHY_PLL2, 0x0, PLL2_RG_RCLRPLL_EN);
    //vIO32WriteFldAlign(DDRPHY_SHU1_PLL4, 0xff, SHU1_PLL4_RG_RPHYPLL_RESERVED);
    //vIO32WriteFldAlign(DDRPHY_SHU1_PLL6, 0xff, SHU1_PLL6_RG_RCLRPLL_RESERVED);
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0+((U32)CHANNEL_B<<POS_BANK_NUM), 0x100000, MISC_CG_CTRL0_CLK_MEM_DFS_CFG);
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 0xf, MISC_CG_CTRL0_CLK_MEM_DFS_CFG);
#if 0//ORIGINAL_PLL_INIT
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 0x1, MISC_CG_CTRL0_CLK_MEM_SEL);
#endif

    mcDELAY_US(1);

    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI2, 0x1, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);

    mcDELAY_US(1);

#if 0//ORIGINAL_PLL_INIT
    ///TODO: DLL_EN
    vIO32WriteFldAlign(DDRPHY_B0_DLL_ARPI5, 0x1, B0_DLL_ARPI5_RG_ARDLL_PHDET_EN_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DLL_ARPI5, 0x1, B1_DLL_ARPI5_RG_ARDLL_PHDET_EN_B1);
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI5+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, CA_DLL_ARPI5_RG_ARDLL_PHDET_EN_CA);
    vIO32WriteFldAlign(DDRPHY_B0_DLL_ARPI5+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, B0_DLL_ARPI5_RG_ARDLL_PHDET_EN_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DLL_ARPI5+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, B1_DLL_ARPI5_RG_ARDLL_PHDET_EN_B1);

    mcDELAY_US(2);
#endif

#if !ORIGINAL_PLL_INIT
    DDRPhyPLLSetting(p);
#endif


    vIO32WriteFldAlign(DDRPHY_B1_DQ9, 0x1, B1_DQ9_RG_RX_ARDQS_STBEN_RESETB_B1);
    vIO32WriteFldAlign(DDRPHY_CA_CMD10+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, CA_CMD10_RG_RX_ARCLK_STBEN_RESETB);
    vIO32WriteFldAlign(DDRPHY_B0_DQ9+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, B0_DQ9_RG_RX_ARDQS_STBEN_RESETB_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ9+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, B1_DQ9_RG_RX_ARDQS_STBEN_RESETB_B1);
    vIO32WriteFldMulti(DDRPHY_MISC_CTRL1, P_Fld(0x1, MISC_CTRL1_R_DMDA_RRESETB_E)
                | P_Fld(0x1, MISC_CTRL1_R_DMDQSIENCG_EN)
                | P_Fld(0x1, MISC_CTRL1_R_DMARPIDQ_SW)
                | P_Fld(0x1, MISC_CTRL1_R_DM_TX_ARCMD_OE)
                | P_Fld(0x1, MISC_CTRL1_R_DM_TX_ARCLK_OE));
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ7, 1, SHU1_B0_DQ7_R_DMRXTRACK_DQM_EN_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ7, 1, SHU1_B1_DQ7_R_DMRXTRACK_DQM_EN_B1);
    vIO32WriteFldAlign(DDRPHY_B0_RXDVS0, 1, B0_RXDVS0_R_HWSAVE_MODE_ENA_B0);
    vIO32WriteFldAlign(DDRPHY_B1_RXDVS0, 1, B1_RXDVS0_R_HWSAVE_MODE_ENA_B1);
    vIO32WriteFldAlign(DDRPHY_CA_RXDVS0, 1, CA_RXDVS0_R_HWSAVE_MODE_ENA_CA);
    vIO32WriteFldMulti(DDRPHY_MISC_CTRL1+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, MISC_CTRL1_R_DMDA_RRESETB_E)
                | P_Fld(0x1, MISC_CTRL1_R_DMDQSIENCG_EN)
                | P_Fld(0x1, MISC_CTRL1_R_DMARPIDQ_SW)
                | P_Fld(0x1, MISC_CTRL1_R_DM_TX_ARCMD_OE)
                | P_Fld(0x1, MISC_CTRL1_R_DM_TX_ARCLK_OE));
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ7+((U32)CHANNEL_B<<POS_BANK_NUM), 1, SHU1_B0_DQ7_R_DMRXTRACK_DQM_EN_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ7+((U32)CHANNEL_B<<POS_BANK_NUM), 1, SHU1_B1_DQ7_R_DMRXTRACK_DQM_EN_B1);
    vIO32WriteFldAlign(DDRPHY_B0_RXDVS0+((U32)CHANNEL_B<<POS_BANK_NUM), 1, B0_RXDVS0_R_HWSAVE_MODE_ENA_B0);
    vIO32WriteFldAlign(DDRPHY_B1_RXDVS0+((U32)CHANNEL_B<<POS_BANK_NUM), 1, B1_RXDVS0_R_HWSAVE_MODE_ENA_B1);
    vIO32WriteFldAlign(DDRPHY_CA_RXDVS0+((U32)CHANNEL_B<<POS_BANK_NUM), 1, CA_RXDVS0_R_HWSAVE_MODE_ENA_CA);
    vIO32WriteFldMulti(DDRPHY_CA_CMD8, P_Fld(0x0, CA_CMD8_RG_TX_RRESETB_PULL_DN)
                | P_Fld(0x0, CA_CMD8_RG_TX_RRESETB_DDR3_SEL));
                //| P_Fld(0x1, CA_CMD8_RG_TX_RRESETB_DDR4_SEL)); Don't toggle RESETB_DDR4_SEL, Justin's feedback on RESETB issue
    vIO32WriteFldAlign(DDRPHY_CA_CMD7, 0x0, CA_CMD7_RG_TX_ARCMD_PULL_DN);
    vIO32WriteFldAlign(DDRPHY_B0_DQ7, 0x0, B0_DQ7_RG_TX_ARDQ_PULL_DN_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ7, 0x0, B1_DQ7_RG_TX_ARDQ_PULL_DN_B1);
    vIO32WriteFldAlign(DDRPHY_CA_CMD7+((U32)CHANNEL_B<<POS_BANK_NUM), 0x0, CA_CMD7_RG_TX_ARCMD_PULL_DN);
    vIO32WriteFldAlign(DDRPHY_B0_DQ7+((U32)CHANNEL_B<<POS_BANK_NUM), 0x0, B0_DQ7_RG_TX_ARDQ_PULL_DN_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ7+((U32)CHANNEL_B<<POS_BANK_NUM), 0x0, B1_DQ7_RG_TX_ARDQ_PULL_DN_B1);
    vIO32WriteFldMulti(DRAMC_REG_SHU_DQSG_RETRY, P_Fld(0x1, SHU_DQSG_RETRY_R_RDY_SEL_DLE)
                | P_Fld(0x6, SHU_DQSG_RETRY_R_DQSIENLAT)
                | P_Fld(0x1, SHU_DQSG_RETRY_R_RETRY_ONCE));
    vIO32WriteFldMulti(DRAMC_REG_SHUCTRL2, P_Fld(0x0, SHUCTRL2_HWSET_WLRL)
                | P_Fld(0x1, SHUCTRL2_SHU_PERIOD_GO_ZERO_CNT)
                | P_Fld(0x1, SHUCTRL2_R_DVFS_OPTION)
                | P_Fld(0x1, SHUCTRL2_R_DVFS_PARK_N)
                | P_Fld(0x1, SHUCTRL2_R_DVFS_DLL_CHA)
                | P_Fld(0xa, SHUCTRL2_R_DLL_IDLE));
    vIO32WriteFldMulti(DRAMC_REG_SHUCTRL2+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x0, SHUCTRL2_HWSET_WLRL)
                | P_Fld(0x1, SHUCTRL2_SHU_PERIOD_GO_ZERO_CNT)
                | P_Fld(0x1, SHUCTRL2_R_DVFS_OPTION)
                | P_Fld(0x1, SHUCTRL2_R_DVFS_PARK_N)
                | P_Fld(0xa, SHUCTRL2_R_DLL_IDLE));
    vIO32WriteFldAlign(DRAMC_REG_DVFSDLL, 0x1, DVFSDLL_DLL_LOCK_SHU_EN);
    vIO32WriteFldMulti(DRAMC_REG_DDRCONF0, P_Fld(0x1, DDRCONF0_LPDDR3EN)
                | P_Fld(0x1, DDRCONF0_DM64BITEN)
                | P_Fld(0x1, DDRCONF0_BK8EN));
    vIO32WriteFldMulti(DRAMC_REG_STBCAL2, P_Fld(0x1, STBCAL2_STB_GERR_B01)
                | P_Fld(0x1, STBCAL2_STB_GERRSTOP));
    vIO32WriteFldAlign(DRAMC_REG_STBCAL2, 0x1, STBCAL2_STB_GERR_RST);
    vIO32WriteFldAlign(DRAMC_REG_STBCAL2, 0x0, STBCAL2_STB_GERR_RST);
    vIO32WriteFldAlign(DRAMC_REG_SHU1_WODT, 0x0, SHU1_WODT_WPST2T);
    vIO32WriteFldAlign(DRAMC_REG_CLKAR, 0x1, CLKAR_PSELAR);
    vIO32WriteFldMulti(DDRPHY_MISC_CTRL0, P_Fld(0x0, MISC_CTRL0_R_DQS0IEN_DIV4_CK_CG_CTRL)
                | P_Fld(0x1, MISC_CTRL0_R_DMDQSIEN_FIFO_EN)
                | P_Fld(0x1, MISC_CTRL0_R_DMSTBEN_OUTSEL)
                | P_Fld(0xf, MISC_CTRL0_R_DMDQSIEN_SYNCOPT));
    vIO32WriteFldMulti(DDRPHY_MISC_CTRL0+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, MISC_CTRL0_R_CLKIEN_DIV4_CK_CG_CTRL)
                | P_Fld(0x1, MISC_CTRL0_R_DMDQSIEN_FIFO_EN)
                | P_Fld(0x1, MISC_CTRL0_R_DMSTBEN_OUTSEL)
                | P_Fld(0xf, MISC_CTRL0_R_DMDQSIEN_SYNCOPT));
    vIO32WriteFldMulti(DRAMC_REG_SHU_CONF0, P_Fld(0x2, SHU_CONF0_MATYPE)
                | P_Fld(0x1, SHU_CONF0_BL4)
                | P_Fld(0x1, SHU_CONF0_FDIV2)
                | P_Fld(0x1, SHU_CONF0_REFTHD)
                | P_Fld(0x1, SHU_CONF0_ADVPREEN)
                | P_Fld(0x3f, SHU_CONF0_DMPGTIM));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ODTCTRL, P_Fld(0x3, SHU_ODTCTRL_TWODT)
                | P_Fld(0x5, SHU_ODTCTRL_RODT)
                | P_Fld(0x1, SHU_ODTCTRL_WOEN));
    vIO32WriteFldAlign(DRAMC_REG_REFCTRL0, 0x5, REFCTRL0_REF_PREGATE_CNT);
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_CA1, P_Fld(0x0, SHU_SELPH_CA1_TXDLY_CS1)
                | P_Fld(0x0, SHU_SELPH_CA1_TXDLY_RAS)
                | P_Fld(0x0, SHU_SELPH_CA1_TXDLY_CAS)
                | P_Fld(0x0, SHU_SELPH_CA1_TXDLY_WE)
                | P_Fld(0x0, SHU_SELPH_CA1_TXDLY_RESET)
                | P_Fld(0x0, SHU_SELPH_CA1_TXDLY_ODT)
                | P_Fld(0x0, SHU_SELPH_CA1_TXDLY_CKE)
                | P_Fld(0x0, SHU_SELPH_CA1_TXDLY_CS));
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_CA2, P_Fld(0x0, SHU_SELPH_CA2_TXDLY_CKE1)
                | P_Fld(0x0, SHU_SELPH_CA2_TXDLY_CMD)
                | P_Fld(0x0, SHU_SELPH_CA2_TXDLY_BA2)
                | P_Fld(0x0, SHU_SELPH_CA2_TXDLY_BA1)
                | P_Fld(0x0, SHU_SELPH_CA2_TXDLY_BA0));
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_CA3, P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA7)
                | P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA6)
                | P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA5)
                | P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA4)
                | P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA3)
                | P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA2)
                | P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA1)
                | P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA0));
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_CA4, P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA15)
                | P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA14)
                | P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA13)
                | P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA12)
                | P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA11)
                | P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA10)
                | P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA9)
                | P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA8));
    vIO32WriteFldAlign(DRAMC_REG_SHU_SELPH_CA5, 0x0, SHU_SELPH_CA5_DLY_ODT);
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS0, P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS3)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS2)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS1)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS0)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_DQS3)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_DQS2)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_DQS1)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_DQS0));
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS1, P_Fld(0x2, SHU_SELPH_DQS1_DLY_OEN_DQS3)
                | P_Fld(0x2, SHU_SELPH_DQS1_DLY_OEN_DQS2)
                | P_Fld(0x2, SHU_SELPH_DQS1_DLY_OEN_DQS1)
                | P_Fld(0x2, SHU_SELPH_DQS1_DLY_OEN_DQS0)
                | P_Fld(0x3, SHU_SELPH_DQS1_DLY_DQS3)
                | P_Fld(0x3, SHU_SELPH_DQS1_DLY_DQS2)
                | P_Fld(0x3, SHU_SELPH_DQS1_DLY_DQS1)
                | P_Fld(0x3, SHU_SELPH_DQS1_DLY_DQS0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3)
                | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2)
                | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1)
                | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0)
                | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ3)
                | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ2)
                | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ1)
                | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3)
                | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2)
                | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1)
                | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0)
                | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM3)
                | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM2)
                | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM1)
                | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ2, P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_OEN_DQ3)
                | P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_OEN_DQ2)
                | P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_OEN_DQ1)
                | P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_OEN_DQ0)
                | P_Fld(0x3, SHURK0_SELPH_DQ2_DLY_DQ3)
                | P_Fld(0x3, SHURK0_SELPH_DQ2_DLY_DQ2)
                | P_Fld(0x3, SHURK0_SELPH_DQ2_DLY_DQ1)
                | P_Fld(0x3, SHURK0_SELPH_DQ2_DLY_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ3, P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_OEN_DQM3)
                | P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_OEN_DQM2)
                | P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_OEN_DQM1)
                | P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_OEN_DQM0)
                | P_Fld(0x3, SHURK0_SELPH_DQ3_DLY_DQM3)
                | P_Fld(0x3, SHURK0_SELPH_DQ3_DLY_DQM2)
                | P_Fld(0x3, SHURK0_SELPH_DQ3_DLY_DQM1)
                | P_Fld(0x3, SHURK0_SELPH_DQ3_DLY_DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3)
                | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2)
                | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1)
                | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0)
                | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ3)
                | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ2)
                | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ1)
                | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3)
                | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2)
                | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1)
                | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0)
                | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM3)
                | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM2)
                | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM1)
                | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ2, P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ3)
                | P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ2)
                | P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ1)
                | P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ0)
                | P_Fld(0x3, SHURK1_SELPH_DQ2_DLY_R1DQ3)
                | P_Fld(0x3, SHURK1_SELPH_DQ2_DLY_R1DQ2)
                | P_Fld(0x3, SHURK1_SELPH_DQ2_DLY_R1DQ1)
                | P_Fld(0x3, SHURK1_SELPH_DQ2_DLY_R1DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ3, P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM3)
                | P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM2)
                | P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM1)
                | P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM0)
                | P_Fld(0x3, SHURK1_SELPH_DQ3_DLY_R1DQM3)
                | P_Fld(0x3, SHURK1_SELPH_DQ3_DLY_R1DQM2)
                | P_Fld(0x3, SHURK1_SELPH_DQ3_DLY_R1DQM1)
                | P_Fld(0x3, SHURK1_SELPH_DQ3_DLY_R1DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN0, P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B3_RODTEN_P1)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B3_RODTEN)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B2_RODTEN_P1)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B2_RODTEN)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN_P1)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN_P1)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN1, P_Fld(0x4, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN_P1)
                | P_Fld(0x4, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN)
                | P_Fld(0x4, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN_P1)
                | P_Fld(0x4, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN)
                | P_Fld(0x4, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN_P1)
                | P_Fld(0x4, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN)
                | P_Fld(0x4, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN_P1)
                | P_Fld(0x4, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN0, P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN_P1)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN_P1)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN_P1)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN_P1)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN1, P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B3_R1RODTEN_P1)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B3_R1RODTEN)
                | P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B2_R1RODTEN_P1)
                | P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B2_R1RODTEN)
                | P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN_P1)
                | P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN)
                | P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN_P1)
                | P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN));
    mcDELAY_US(1);

    vIO32WriteFldMulti(DRAMC_REG_STBCAL, P_Fld(0x0, STBCAL_DQSIENMODE_SELPH)
                | P_Fld(0x1, STBCAL_STB_DQIEN_IG)
                | P_Fld(0x1, STBCAL_PICHGBLOCK_NORD)
                | P_Fld(0x1, STBCAL_PIMASK_RKCHG_OPT));
    vIO32WriteFldMulti(DRAMC_REG_SHU1_DQSG, P_Fld(0x9, SHU1_DQSG_STB_UPDMASKCYC)
                | P_Fld(0x1, SHU1_DQSG_STB_UPDMASK_EN));
    vIO32WriteFldAlign(DRAMC_REG_STBCAL, 0x1, STBCAL_DQSIENMODE);
    vIO32WriteFldMulti(DRAMC_REG_SREFCTRL, P_Fld(0x0, SREFCTRL_SREF_HW_EN)
                | P_Fld(0x8, SREFCTRL_SREFDLY)
                | P_Fld(0x0, SREFCTRL_SREF2_OPTION));
    vIO32WriteFldMulti(DRAMC_REG_SHU_PIPE, P_Fld(0x1, SHU_PIPE_READ_START_EXTEND1)
                | P_Fld(0x1, SHU_PIPE_DLE_LAST_EXTEND1));
    vIO32WriteFldMulti(DRAMC_REG_CKECTRL, P_Fld(0x1, CKECTRL_CKEON)
                | P_Fld(0x1, CKECTRL_CKETIMER_SEL));
    vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0x1, RKCFG_CKE2RANK_OPT2);
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM0, P_Fld(0xa, SHU_ACTIM0_TRCD)
                | P_Fld(0x3, SHU_ACTIM0_TRRD)
                | P_Fld(0xd, SHU_ACTIM0_TWR)
                | P_Fld(0x7, SHU_ACTIM0_TWTR));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM1, P_Fld(0x17, SHU_ACTIM1_TRC)
                | P_Fld(0xb, SHU_ACTIM1_TRAS)
                | P_Fld(0x9, SHU_ACTIM1_TRP)
                | P_Fld(0x1, SHU_ACTIM1_TRPAB));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM2, P_Fld(0xe, SHU_ACTIM2_TFAW)
                | P_Fld(0x6, SHU_ACTIM2_TR2W)
                | P_Fld(0x2, SHU_ACTIM2_TRTP));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM3, P_Fld(0x56, SHU_ACTIM3_TRFC)
                | P_Fld(0x1f, SHU_ACTIM3_TRFCPB));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM5, P_Fld(0x1, SHU_ACTIM5_TMRR2W)
                | P_Fld(0xb, SHU_ACTIM5_TWTPD)
                | P_Fld(0xa, SHU_ACTIM5_TR2PD));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM_XRT, P_Fld(0x8, SHU_ACTIM_XRT_XRTW2W)
                | P_Fld(0x4, SHU_ACTIM_XRT_XRTR2W)
                | P_Fld(0xa, SHU_ACTIM_XRT_XRTR2R));
    vIO32WriteFldMulti(DRAMC_REG_SHU_AC_TIME_05T, P_Fld(0x1, SHU_AC_TIME_05T_TFAW_05T)
                | P_Fld(0x1, SHU_AC_TIME_05T_TRPAB_05T)
                | P_Fld(0x1, SHU_AC_TIME_05T_TRP_05T)
                | P_Fld(0x1, SHU_AC_TIME_05T_TRCD_05T)
                | P_Fld(0x1, SHU_AC_TIME_05T_TRTP_05T)
                | P_Fld(0x1, SHU_AC_TIME_05T_TXP_05T)
                | P_Fld(0x1, SHU_AC_TIME_05T_TRC_05T));
    vIO32WriteFldMulti(DRAMC_REG_SHU_CONF2, P_Fld(0x7, SHU_CONF2_DCMDLYREF)
                | P_Fld(0x32, SHU_CONF2_FSPCHG_PRDCNT));
    vIO32WriteFldMulti(DRAMC_REG_SHU_SCINTV, P_Fld(0x0, SHU_SCINTV_DQS2DQ_SHU_PITHRD)
                | P_Fld(0x15, SHU_SCINTV_MRW_INTV)
                | P_Fld(0x0, SHU_SCINTV_RDDQC_INTV)
                | P_Fld(0x0, SHU_SCINTV_TZQLAT));
    vIO32WriteFldMulti(DRAMC_REG_SHU_CONF1, P_Fld(0xc, SHU_CONF1_DATLAT_DSEL_PHY)
                | P_Fld(0xc, SHU_CONF1_DATLAT_DSEL)
                | P_Fld(0xe, SHU_CONF1_DATLAT));
    vIO32WriteFldAlign(DRAMC_REG_SHUCTRL, 0x1, SHUCTRL_LPSM_BYPASS_B);
    vIO32WriteFldAlign(DRAMC_REG_REFCTRL1, 0x0, REFCTRL1_SREF_PRD_OPT);
    //vIO32WriteFldAlign(DDRPHY_SHU1_PLL4, 0xfe, SHU1_PLL4_RG_RPHYPLL_RESERVED);
    //vIO32WriteFldAlign(DDRPHY_SHU1_PLL6, 0xfe, SHU1_PLL6_RG_RCLRPLL_RESERVED);
    vIO32WriteFldAlign(DRAMC_REG_REFRATRE_FILTER, 0x1, REFRATRE_FILTER_PB2AB_OPT);
    vIO32WriteFldAlign(DDRPHY_MISC_CTRL1, 0x1, MISC_CTRL1_R_DMDA_RRESETB_I);
    vIO32WriteFldAlign(DRAMC_REG_DRAMCTRL, 0x0, DRAMCTRL_CLKWITRFC);
    vIO32WriteFldMulti(DRAMC_REG_MISCTL0, P_Fld(0x1, MISCTL0_REFP_ARB_EN2)
                | P_Fld(0x1, MISCTL0_PBC_ARB_EN)
                | P_Fld(0x1, MISCTL0_REFA_ARB_EN2));
    vIO32WriteFldMulti(DRAMC_REG_PERFCTL0, P_Fld(0x1, PERFCTL0_MWHPRIEN)
                | P_Fld(0x1, PERFCTL0_RWSPLIT)
                | P_Fld(0x1, PERFCTL0_WFLUSHEN)
                | P_Fld(0x1, PERFCTL0_EMILLATEN)
                | P_Fld(0x1, PERFCTL0_RWAGEEN)
                | P_Fld(0x1, PERFCTL0_RWLLATEN)
                | P_Fld(0x1, PERFCTL0_RWHPRIEN)
                | P_Fld(0x1, PERFCTL0_RWOFOEN)
                | P_Fld(0x1, PERFCTL0_DISRDPHASE1)
                | P_Fld(0x1, PERFCTL0_DUALSCHEN));
    vIO32WriteFldAlign(DRAMC_REG_ARBCTL, 0x80, ARBCTL_MAXPENDCNT);
    vIO32WriteFldMulti(DRAMC_REG_PADCTRL, P_Fld(0x1, PADCTRL_DQIENLATEBEGIN)
                | P_Fld(0x1, PADCTRL_DQIENQKEND));
    vIO32WriteFldAlign(DRAMC_REG_DRAMC_PD_CTRL, 0x1, DRAMC_PD_CTRL_DCMREF_OPT);
    vIO32WriteFldAlign(DRAMC_REG_CLKCTRL, 0x1, CLKCTRL_CLK_EN_1);
    vIO32WriteFldMulti(DRAMC_REG_REFCTRL0, P_Fld(0x4, REFCTRL0_DISBYREFNUM)
                | P_Fld(0x1, REFCTRL0_DLLFRZ));
    vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x1, SPCMDCTRL_CLR_EN);
    vIO32WriteFldMulti(DRAMC_REG_CATRAINING1, P_Fld(0x4, CATRAINING1_CATRAIN_INTV)
                | P_Fld(0x3, CATRAINING1_CATRAINLAT));
    vIO32WriteFldAlign(DRAMC_REG_STBCAL, 0x1, STBCAL_REFUICHG);
    vIO32WriteFldMulti(DRAMC_REG_SHU_RANKCTL, P_Fld(0x5, SHU_RANKCTL_RANKINCTL_PHY)
                | P_Fld(0x2, SHU_RANKCTL_RANKINCTL_ROOT1)
                | P_Fld(0x2, SHU_RANKCTL_RANKINCTL));
    vIO32WriteFldAlign(DRAMC_REG_SHU_STBCAL, 0x1, SHU_STBCAL_DMSTBLAT);
    vIO32WriteFldAlign(DRAMC_REG_SHURK0_DQSCTL, 0x4, SHURK0_DQSCTL_DQSINCTL);
    vIO32WriteFldAlign(DRAMC_REG_SHURK1_DQSCTL, 0x4, SHURK1_DQSCTL_R1DQSINCTL);

    mcDELAY_US(2);

    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0, P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED_P1)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED_P1)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(0x3, SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED_P1)
                | P_Fld(0x1, SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED)
                | P_Fld(0x3, SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED_P1)
                | P_Fld(0x1, SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED)
                | P_Fld(0x3, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
                | P_Fld(0x1, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
                | P_Fld(0x3, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
                | P_Fld(0x1, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0, P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS3_GATED_P1)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS3_GATED)
                | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS2_GATED_P1)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS2_GATED)
                | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
                | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1, P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS3_GATED_P1)
                | P_Fld(0x7, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS3_GATED)
                | P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS2_GATED_P1)
                | P_Fld(0x7, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS2_GATED)
                | P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
                | P_Fld(0x7, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED)
                | P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1)
                | P_Fld(0x7, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED_P1)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED_P1)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x3, SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED_P1)
                | P_Fld(0x1, SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED)
                | P_Fld(0x3, SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED_P1)
                | P_Fld(0x1, SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED)
                | P_Fld(0x3, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
                | P_Fld(0x1, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
                | P_Fld(0x3, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
                | P_Fld(0x1, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS3_GATED_P1)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS3_GATED)
                | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS2_GATED_P1)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS2_GATED)
                | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
                | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS3_GATED_P1)
                | P_Fld(0x7, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS3_GATED)
                | P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS2_GATED_P1)
                | P_Fld(0x7, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS2_GATED)
                | P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
                | P_Fld(0x7, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED)
                | P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1)
                | P_Fld(0x7, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED));


    ///TODO: overwrite 1866 setting START
    vIO32WriteFldMulti(DRAMC_REG_SHU1_WODT, P_Fld(0x1, SHU1_WODT_DISWODTE)
                    | P_Fld(0x1, SHU1_WODT_WODTFIXOFF)
                    | P_Fld(0x4, SHU1_WODT_DISWODT));

    //vIO32WriteFldAlign(DRAMC_REG_SHU_CONF3, 0x5, SHU_CONF3_ZQCSCNT);
    //vIO32WriteFldAlign(DRAMC_REG_SHU_CONF3, 0xff, SHU_CONF3_REFRCNT);
    //vIO32WriteFldAlign(DRAMC_REG_SHU_CONF1, 0xb0, SHU_CONF1_REFBW_FR);
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM4, P_Fld(0x29, SHU_ACTIM4_TZQCS)
                | P_Fld(0x65, SHU_ACTIM4_REFCNT_FR_CLK)
                | P_Fld(0x65, SHU_ACTIM4_TXREFCNT));
	
    vIO32WriteFldAlign(DRAMC_REG_SHU_ACTIM3, 0x70, SHU_ACTIM3_REFCNT);
	
    vIO32WriteFldAlign(DRAMC_REG_SHU_STBCAL, 0x1, SHU_STBCAL_DQSG_MODE);
    vIO32WriteFldMulti(DRAMC_REG_SHU_CONF0, P_Fld(0x5, SHU_CONF0_REFTHD)
                | P_Fld(0x15, SHU_CONF0_DMPGTIM));
    //vIO32WriteFldAlign(DRAMC_REG_SHU_CONF1, 0xae, SHU_CONF1_REFBW_FR);
    //vIO32WriteFldMulti(DRAMC_REG_SHU_CONF3, P_Fld(0x8d, SHU_CONF3_REFRCNT)
    //            | P_Fld(0x1f8, SHU_CONF3_ZQCSCNT));
    vIO32WriteFldMulti(DRAMC_REG_SHU_STBCAL, P_Fld(0x1, SHU_STBCAL_PICGLAT)
                | P_Fld(0x0, SHU_STBCAL_DMSTBLAT));
    //vIO32WriteFldAlign(DRAMC_REG_SHU1_WODT, 0x1, SHU1_WODT_TWPSTEXT);

    //if(p->frequency==933)
    {
        //               91423 ===dramc_shu1_lp3_1866 begin===
        vIO32WriteFldMulti(DRAMC_REG_SHU_CONF0, P_Fld(0x1, SHU_CONF0_REFTHD)
                    | P_Fld(0x3f, SHU_CONF0_DMPGTIM));
        vIO32WriteFldAlign(DRAMC_REG_SHU_CONF1, 0xb0, SHU_CONF1_REFBW_FR);
        vIO32WriteFldMulti(DRAMC_REG_SHU_CONF3, P_Fld(0xff, SHU_CONF3_REFRCNT)
                    | P_Fld(0x5, SHU_CONF3_ZQCSCNT));
        vIO32WriteFldAlign(DRAMC_REG_SHU_RANKCTL, 0x3, SHU_RANKCTL_RANKINCTL_PHY);
        //vIO32WriteFldAlign(DRAMC_REG_SHU_CKECTRL, 0x3, SHU_CKECTRL_CKEPRD); CKEPRD's correct value is set in UpdateACTImingReg( )
        vIO32WriteFldAlign(DRAMC_REG_SHU1_WODT, 0x0, SHU1_WODT_TWPSTEXT);
        vIO32WriteFldAlign(DRAMC_REG_SHU1_DQSG, 0xf, SHU1_DQSG_SCINTV);
        //vIO32WriteFldMulti(DRAMC_REG_SHU_HWSET_MR2, P_Fld(0x0, SHU_HWSET_MR2_HWSET_MR2_OP)
        //            | P_Fld(0x0, SHU_HWSET_MR2_HWSET_MR2_MRSMA));
        //vIO32WriteFldMulti(DRAMC_REG_SHU_HWSET_MR13, P_Fld(0x1c, SHU_HWSET_MR13_HWSET_MR13_OP)
        //            | P_Fld(0x2, SHU_HWSET_MR13_HWSET_MR13_MRSMA));
        //vIO32WriteFldMulti(DRAMC_REG_SHU_HWSET_VRCG, P_Fld(0x0, SHU_HWSET_VRCG_HWSET_VRCG_OP)
        //            | P_Fld(0x0, SHU_HWSET_VRCG_HWSET_VRCG_MRSMA));
        vIO32WriteFldMulti(DRAMC_REG_SHU_DQSG_RETRY, P_Fld(0x1, SHU_DQSG_RETRY_R_RETRY_USE_BURST_MDOE)
                    | P_Fld(0x1, SHU_DQSG_RETRY_R_RETRY_ROUND_NUM)
                    | P_Fld(0x1, SHU_DQSG_RETRY_R_XSR_RETRY_SPM_MODE)
                    | P_Fld(0x3, SHU_DQSG_RETRY_R_DQSIENLAT)
                    | P_Fld(0x1, SHU_DQSG_RETRY_R_DM4BYTE));
        //               92288 ===dramc_shu1_lp3_1866 end===

        //               93153 ===ddrphy_shu1_lp3_1866_CHA begin===
        vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ5, 0x4, SHU1_B0_DQ5_RG_ARPI_FB_B0);
        vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5, 0x4, SHU1_B1_DQ5_RG_ARPI_FB_B1);
        vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD5, 0x4, SHU1_CA_CMD5_RG_ARPI_FB_CA);
        //               93918 ===ddrphy_shu1_lp3_1866_CHA end===
        //               93918 ===ddrphy_shu1_lp3_1866_CHB begin===
        vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ5+((U32)CHANNEL_B<<POS_BANK_NUM), 0x4, SHU1_B0_DQ5_RG_ARPI_FB_B0);
        vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5+((U32)CHANNEL_B<<POS_BANK_NUM), 0x4, SHU1_B1_DQ5_RG_ARPI_FB_B1);
        vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD5+((U32)CHANNEL_B<<POS_BANK_NUM), 0x4, SHU1_CA_CMD5_RG_ARPI_FB_CA);
        //vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD6+((U32)CHANNEL_B<<POS_BANK_NUM), 0x3, SHU1_CA_CMD6_RG_ARPI_RESERVE_CA);
        vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ2+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0xa, SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_F_DLY_B0)
                    | P_Fld(0xa, SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_F_DLY_B0));
        //               94591 ===ddrphy_shu1_lp3_1866_CHB end===
    }
    //else
    ///TODO: overwrite 1866 setting END
    
    TxCalibrationMoveDQSByDifferentWL(p); //if frequency is 600 or 800, TX_DQS should be adjusted to prevent TX calibration K fail
                                          //note that the DQS offset is the difference of WL between DDR1866 (=8) and 1200 or 1600 (both=6)

    ///TODO:
    vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0x1, RKCFG_RKMODE);

    UpdateInitialSettings_LP3(p);

#if SIMULATION_SW_IMPED
    DramcSwImpedanceSaveRegister(p, ODT_OFF, ODT_OFF, DRAM_DFS_SHUFFLE_1);
#endif

#ifndef LOOPBACK_TEST
    DDRPhyFreqMeter();
#endif

    DramcModeRegInit_LP3(p);


#if 0
    vIO32WriteFldMulti(DRAMC_REG_MRS, P_Fld(0x0, MRS_MRSRK)
                | P_Fld(0x4, MRS_MRSMA)
                | P_Fld(0x0, MRS_MRSOP));

    mcDELAY_US(1);

    vIO32WriteFldAlign(DRAMC_REG_SPCMD, 0x1, SPCMD_MRREN);

    mcDELAY_US(1);
#endif

    vIO32WriteFldMulti(DRAMC_REG_REFCTRL0, P_Fld(0x0, REFCTRL0_PBREFEN)
                | P_Fld(0x1, REFCTRL0_PBREF_DISBYRATE));
    vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0x1, RKCFG_DQSOSC2RK);
    vIO32WriteFldAlign(DRAMC_REG_CKECTRL, 0x0, CKECTRL_CKEFIXON);
    //vIO32WriteFldAlign(DRAMC_REG_HW_MRR_FUN, 0x1, HW_MRR_FUN_TMRR_ENA);
    vIO32WriteFldAlign(DRAMC_REG_DRAMCTRL, 0x1, DRAMCTRL_PREALL_OPTION);
    vIO32WriteFldAlign(DRAMC_REG_ZQCS, 0x56, ZQCS_ZQCSOP);

    mcDELAY_US(1);


    //vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x0, SPCMDCTRL_REFRDIS);//Lewis@20160613: Fix refresh rate is wrong while diable MR4
    vIO32WriteFldMulti(DRAMC_REG_REFCTRL0, P_Fld(0x1, REFCTRL0_REFFRERUN)
                | P_Fld(0x1, REFCTRL0_REFDIS));
    vIO32WriteFldAlign(DRAMC_REG_SREFCTRL, 0x1, SREFCTRL_SREF_HW_EN);
    vIO32WriteFldAlign(DRAMC_REG_MPC_OPTION, 0x1, MPC_OPTION_MPCRKEN);
    vIO32WriteFldMulti(DRAMC_REG_DRAMC_PD_CTRL, P_Fld(0x1, DRAMC_PD_CTRL_PHYCLKDYNGEN)
                | P_Fld(0x1, DRAMC_PD_CTRL_DCMEN));
    vIO32WriteFldMulti(DRAMC_REG_DRAMC_PD_CTRL+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x0, DRAMC_PD_CTRL_COMBCLKCTRL)
                | P_Fld(0x0, DRAMC_PD_CTRL_DCMENNOTRFC)
                | P_Fld(0x1, DRAMC_PD_CTRL_DCMEN));
    vIO32WriteFldMulti(DRAMC_REG_EYESCAN, P_Fld(0x1, EYESCAN_RX_DQ_EYE_SEL)
                | P_Fld(0x1, EYESCAN_RG_RX_EYE_SCAN_EN));
    vIO32WriteFldMulti(DRAMC_REG_STBCAL1, P_Fld(0x1, STBCAL1_STBCNT_LATCH_EN)
                | P_Fld(0x1, STBCAL1_STBENCMPEN));
    vIO32WriteFldAlign(DRAMC_REG_TEST2_1, 0x10000, TEST2_1_TEST2_BASE);
    vIO32WriteFldAlign(DRAMC_REG_TEST2_2, 0x400, TEST2_2_TEST2_OFF);
    vIO32WriteFldMulti(DRAMC_REG_TEST2_3, P_Fld(0x1, TEST2_3_TEST2WREN2_HW_EN)
                | P_Fld(0x4, TEST2_3_DQSICALSTP)
                | P_Fld(0x1, TEST2_3_TESTAUDPAT));
    vIO32WriteFldAlign(DRAMC_REG_SHUCTRL2, 0x1, SHUCTRL2_MR13_SHU_EN);
    vIO32WriteFldMulti(DRAMC_REG_DRAMCTRL, P_Fld(0x1, DRAMCTRL_REQQUE_THD_EN)
                | P_Fld(0x1, DRAMCTRL_DPDRK_OPT));
    vIO32WriteFldAlign(DRAMC_REG_SHU_CKECTRL, 0x3, SHU_CKECTRL_SREF_CK_DLY);
    vIO32WriteFldAlign(DRAMC_REG_DUMMY_RD, p->support_rank_num, DUMMY_RD_RANK_NUM);
    vIO32WriteFldAlign(DRAMC_REG_TEST2_4, 0x4, TEST2_4_TESTAGENTRKSEL);
    vIO32WriteFldMulti(DRAMC_REG_REFCTRL1, P_Fld(0x1, REFCTRL1_REF_QUE_AUTOSAVE_EN)
                | P_Fld(0x1, REFCTRL1_SLEFREF_AUTOSAVE_EN));
    vIO32WriteFldMulti(DRAMC_REG_RSTMASK, P_Fld(0x0, RSTMASK_GT_SYNC_MASK)
                | P_Fld(0x0, RSTMASK_DVFS_SYNC_MASK)
                | P_Fld(0x0, RSTMASK_GT_SYNC_MASK_FOR_PHY)
                | P_Fld(0x0, RSTMASK_DVFS_SYNC_MASK_FOR_PHY));
    //vIO32WriteFldAlign(DRAMC_REG_DRAMCTRL, 0x0, DRAMCTRL_CTOREQ_HPRI_OPT);
    //               91074 === DE initial sequence done ===
    //#ifdef DVFS_Enable
    vIO32WriteFldAlign(DRAMC_REG_STBCAL1, 0x8, STBCAL1_STBCAL_FILTER);
    vIO32WriteFldMulti(DRAMC_REG_STBCAL, P_Fld(0x0, STBCAL_STBCALEN)
                | P_Fld(0x0, STBCAL_STB_SELPHYCALEN)
                | P_Fld(0x0, STBCAL_REFUICHG)
                | P_Fld(0x1, STBCAL_SREF_DQSGUPD));

    mcDELAY_US(1);

      //*((UINT32P)(DDRPHY0AO_BASE + 0x0000)) = 0x00000000; // trun off when SPM need
    vIO32WriteFldAlign(DRAMC_REG_DDRCONF0, 0x1, DDRCONF0_AG0MWR);
    //vIO32WriteFldMulti(DRAMC_REG_DRAMCTRL, P_Fld(0x1, DRAMCTRL_FW2R)
    //            | P_Fld(0x0, DRAMCTRL_DYNMWREN)
    //            | P_Fld(0x1, DRAMCTRL_CLKWITRFC)
    //            | P_Fld(0x1, DRAMCTRL_ADRBIT3DEC)
    //            | P_Fld(0x1, DRAMCTRL_CTOREQ_HPRI_OPT));
#if 0 // Darren- by Jeremy confirm
    vIO32WriteFldAlign(DRAMC_REG_MISCTL0, 0x0, MISCTL0_PBC_ARB_EN);
    vIO32WriteFldMulti(DRAMC_REG_PERFCTL0, P_Fld(0x1, PERFCTL0_REORDEREN)
                | P_Fld(0x1, PERFCTL0_RWHPRICTL)
                | P_Fld(0x0, PERFCTL0_EMILLATEN)
                | P_Fld(0x0, PERFCTL0_RWAGEEN)
                | P_Fld(0x0, PERFCTL0_RWHPRIEN));
#endif
    vIO32WriteFldMulti(DRAMC_REG_RSTMASK, P_Fld(0x0, RSTMASK_PHY_SYNC_MASK)
                | P_Fld(0x0, RSTMASK_DAT_SYNC_MASK));
    //vIO32WriteFldAlign(DRAMC_REG_SREFCTRL, 0xa, SREFCTRL_SREFDLY);
#if 0 // Darren- by Jeremy confirm
    vIO32WriteFldMulti(DRAMC_REG_REFCTRL0, P_Fld(0x1, REFCTRL0_REFNA_OPT)
                | P_Fld(0x2, REFCTRL0_DISBYREFNUM)
                | P_Fld(0x1, REFCTRL0_UPDBYWR));
#endif
    vIO32WriteFldMulti(DRAMC_REG_REFRATRE_FILTER, P_Fld(0x0, REFRATRE_FILTER_REFRATE_FIL7)
                | P_Fld(0x1, REFRATRE_FILTER_REFRATE_FIL6)
                | P_Fld(0x7, REFRATRE_FILTER_REFRATE_FIL5)
                | P_Fld(0x5, REFRATRE_FILTER_REFRATE_FIL4)
                | P_Fld(0x6, REFRATRE_FILTER_REFRATE_FIL2)
                | P_Fld(0x4, REFRATRE_FILTER_REFRATE_FIL1)
                | P_Fld(0x6, REFRATRE_FILTER_REFRATE_FIL0));
    vIO32WriteFldMulti(DRAMC_REG_ZQCS, P_Fld(0x0, ZQCS_ZQCSDUAL)
                | P_Fld(0x0, ZQCS_ZQCSMASK));
    vIO32WriteFldAlign(DRAMC_REG_HW_MRR_FUN, 0x1, HW_MRR_FUN_MRR_HW_HIPRI);
    vIO32WriteFldAlign(DRAMC_REG_DUMMY_RD, 0x1, DUMMY_RD_SREF_DMYRD_EN);
    vIO32WriteFldAlign(DRAMC_REG_STBCAL1, 0x2, STBCAL1_STBCAL_FILTER);
    //               91423 ===LP3_1866_intial_setting_shu1 end===

    vIO32WriteFldAlign(DRAMC_REG_SHUCTRL, 0x1, SHUCTRL_DVFS_CHB_SEL_B);
    DVFSSettings(p);
}
#endif

#if LEGACY_SET_ACTIMING
static void Legacy_DramcSet_LP4_DDR3200_ACTimingRegs(DRAMC_CTX_T *p)
{
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM0, P_Fld(0x6, SHU_ACTIM0_TRCD)
                    | P_Fld(0x2, SHU_ACTIM0_TRRD)
                    | P_Fld(0xd, SHU_ACTIM0_TWR)
                    | P_Fld(0x8, SHU_ACTIM0_TWTR));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM1, P_Fld(0x10, SHU_ACTIM1_TRC)
                    | P_Fld(0x8, SHU_ACTIM1_TRAS)
                    | P_Fld(0x5, SHU_ACTIM1_TRP)
                    | P_Fld(0x1, SHU_ACTIM1_TRPAB));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM2, P_Fld(0x7, SHU_ACTIM2_TFAW)
                    | P_Fld(0x9, SHU_ACTIM2_TR2W)
                    | P_Fld(0x2, SHU_ACTIM2_TRTP)
                    | P_Fld(0x1, SHU_ACTIM2_TXP));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM3, P_Fld(0x64, SHU_ACTIM3_TRFC)
                    | P_Fld(0x2c, SHU_ACTIM3_TRFCPB));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM5, P_Fld(0x8, SHU_ACTIM5_TMRR2W)
                    | P_Fld(0xc, SHU_ACTIM5_TWTPD)
                    | P_Fld(0xd, SHU_ACTIM5_TR2PD));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM_XRT, P_Fld(0x8, SHU_ACTIM_XRT_XRTW2W)
                    | P_Fld(0x3, SHU_ACTIM_XRT_XRTW2R)
                    | P_Fld(0x7, SHU_ACTIM_XRT_XRTR2W)
                    | P_Fld(0x9, SHU_ACTIM_XRT_XRTR2R));
        vIO32WriteFldMulti(DRAMC_REG_SHU_AC_TIME_05T, P_Fld(0x1, SHU_AC_TIME_05T_TWR_M05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRAS_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRP_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRCD_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRTP_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRC_05T));
        vIO32WriteFldMulti(DRAMC_REG_SHU_CONF1, P_Fld(0xc, SHU_CONF1_DATLAT_DSEL_PHY)
                    | P_Fld(0xc, SHU_CONF1_DATLAT_DSEL)
                    | P_Fld(0xe, SHU_CONF1_DATLAT));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM0, P_Fld(0x6, SHU_ACTIM0_TRCD)
                    | P_Fld(0x2, SHU_ACTIM0_TRRD)
                    | P_Fld(0xc, SHU_ACTIM0_TWR)
                    | P_Fld(0x7, SHU_ACTIM0_TWTR));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM1, P_Fld(0x10, SHU_ACTIM1_TRC)
                    | P_Fld(0x8, SHU_ACTIM1_TRAS)
                    | P_Fld(0x5, SHU_ACTIM1_TRP)
                    | P_Fld(0x1, SHU_ACTIM1_TRPAB));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM2, P_Fld(0x7, SHU_ACTIM2_TFAW)
                    | P_Fld(0x7, SHU_ACTIM2_TR2W)
                    | P_Fld(0x2, SHU_ACTIM2_TRTP)
                    | P_Fld(0x1, SHU_ACTIM2_TXP));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM3, P_Fld(0x64, SHU_ACTIM3_TRFC)
                    | P_Fld(0x2c, SHU_ACTIM3_TRFCPB));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM5, P_Fld(0x8, SHU_ACTIM5_TMRR2W)
                    | P_Fld(0xb, SHU_ACTIM5_TWTPD)
                    | P_Fld(0xa, SHU_ACTIM5_TR2PD));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM_XRT, P_Fld(0x8, SHU_ACTIM_XRT_XRTW2W)
                    | P_Fld(0x3, SHU_ACTIM_XRT_XRTW2R)
                    | P_Fld(0x7, SHU_ACTIM_XRT_XRTR2W)
                    | P_Fld(0x9, SHU_ACTIM_XRT_XRTR2R));
        vIO32WriteFldMulti(DRAMC_REG_SHU_AC_TIME_05T, P_Fld(0x1, SHU_AC_TIME_05T_TR2PD_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TWR_M05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRAS_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRP_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRCD_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRTP_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRC_05T));
        vIO32WriteFldMulti(DRAMC_REG_SHU_CONF1, P_Fld(0xb, SHU_CONF1_DATLAT_DSEL_PHY)
                    | P_Fld(0xb, SHU_CONF1_DATLAT_DSEL)
                    | P_Fld(0xd, SHU_CONF1_DATLAT));
    }

    vIO32WriteFldAlign(DRAMC_REG_SHU_ACTIM3, 0x61, SHU_ACTIM3_REFCNT);
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM4, P_Fld(0x22, SHU_ACTIM4_TZQCS)
                | P_Fld(0x65, SHU_ACTIM4_REFCNT_FR_CLK)
                | P_Fld(0x76, SHU_ACTIM4_TXREFCNT));

    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM2, P_Fld(0x8, SHU_ACTIM2_TR2W)
                    | P_Fld(0x0, SHU_ACTIM2_TXP));
        vIO32WriteFldAlign(DRAMC_REG_SHU_ACTIM5, 0x9, SHU_ACTIM5_TR2PD);
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM_XRT, P_Fld(0x9, SHU_ACTIM_XRT_XRTW2W)
                    | P_Fld(0x8, SHU_ACTIM_XRT_XRTR2W));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM5, P_Fld(0x9, SHU_ACTIM5_TMRR2W)
                    | P_Fld(0xc, SHU_ACTIM5_TR2PD));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM_XRT, P_Fld(0x9, SHU_ACTIM_XRT_XRTW2W)
                    | P_Fld(0x8, SHU_ACTIM_XRT_XRTR2W));
        vIO32WriteFldAlign(DRAMC_REG_SHU_AC_TIME_05T, 0x0, SHU_AC_TIME_05T_TR2PD_05T);
        vIO32WriteFldMulti(DRAMC_REG_SHU_CONF1, P_Fld(0xc, SHU_CONF1_DATLAT_DSEL_PHY)
                    | P_Fld(0xc, SHU_CONF1_DATLAT_DSEL)
                    | P_Fld(0xe, SHU_CONF1_DATLAT));
    }

    //NOT included in parsing tool
    vIO32WriteFldMulti(DRAMC_REG_SHU_AC_TIME_05T, P_Fld(0x0, SHU_AC_TIME_05T_TWTR_M05T)
            | P_Fld(0x0, SHU_AC_TIME_05T_TR2W_05T)
            | P_Fld(0x0, SHU_AC_TIME_05T_TWTPD_M05T)
            | P_Fld(0x0, SHU_AC_TIME_05T_TR2PD_05T)
            | P_Fld(0x0, SHU_AC_TIME_05T_TFAW_05T)
            | P_Fld(0x0, SHU_AC_TIME_05T_TRRD_05T)
            | P_Fld(0x1, SHU_AC_TIME_05T_TWR_M05T)
            | P_Fld(0x1, SHU_AC_TIME_05T_TRAS_05T)
            | P_Fld(0x0, SHU_AC_TIME_05T_TRPAB_05T)
            | P_Fld(0x1, SHU_AC_TIME_05T_TRP_05T)
            | P_Fld(0x1, SHU_AC_TIME_05T_TRCD_05T)
            | P_Fld(0x1, SHU_AC_TIME_05T_TRTP_05T)
            | P_Fld(0x0, SHU_AC_TIME_05T_TXP_05T)
            | P_Fld(0x0, SHU_AC_TIME_05T_TRFC_05T)
            | P_Fld(0x1, SHU_AC_TIME_05T_TRC_05T));

}
#endif /* LEGACY_SET_ACTIMING */

#if LEGACY_SET_ACTIMING
static void Legacy_DramcSet_LP4_DDR2667_ACTimingRegs(DRAMC_CTX_T *p)
{
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM0, P_Fld(0x5, SHU_ACTIM0_TRCD)
                | P_Fld(0x1, SHU_ACTIM0_TRRD)
                | P_Fld(0xa, SHU_ACTIM0_TWR)
                | P_Fld(0x6, SHU_ACTIM0_TWTR));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM1, P_Fld(0xc, SHU_ACTIM1_TRC)
                | P_Fld(0x5, SHU_ACTIM1_TRAS)
                | P_Fld(0x4, SHU_ACTIM1_TRP)
                | P_Fld(0x1, SHU_ACTIM1_TRPAB));
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM2, P_Fld(0x4, SHU_ACTIM2_TFAW)
                    | P_Fld(0x4, SHU_ACTIM2_TR2W)
                    | P_Fld(0x1, SHU_ACTIM2_TRTP)
                    | P_Fld(0x1, SHU_ACTIM2_TXP));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM3, P_Fld(0x51, SHU_ACTIM3_TRFC)
                    | P_Fld(0x51, SHU_ACTIM3_REFCNT)
                    | P_Fld(0x23, SHU_ACTIM3_TRFCPB));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM4, P_Fld(0x1c, SHU_ACTIM4_TZQCS)
                    | P_Fld(0x63, SHU_ACTIM4_TXREFCNT));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM5, P_Fld(0x6, SHU_ACTIM5_TMRR2W)
                    | P_Fld(0xa, SHU_ACTIM5_TWTPD)
                    | P_Fld(0x9, SHU_ACTIM5_TR2PD));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM_XRT, P_Fld(0x8, SHU_ACTIM_XRT_XRTW2W)
                    | P_Fld(0x4, SHU_ACTIM_XRT_XRTR2W));
        vIO32WriteFldMulti(DRAMC_REG_SHU_AC_TIME_05T, P_Fld(0x0, SHU_AC_TIME_05T_TWTR_M05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TR2W_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TWTPD_M05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TR2PD_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TFAW_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRRD_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TWR_M05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRAS_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRPAB_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRP_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRCD_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRTP_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TXP_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRFC_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRC_05T));
        vIO32WriteFldMulti(DRAMC_REG_SHU_CONF1, P_Fld(0xb, SHU_CONF1_DATLAT_DSEL_PHY)
                    | P_Fld(0xb, SHU_CONF1_DATLAT_DSEL)
                    | P_Fld(0xd, SHU_CONF1_DATLAT));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM2, P_Fld(0x4, SHU_ACTIM2_TFAW)
                    | P_Fld(0x7, SHU_ACTIM2_TR2W)
                    | P_Fld(0x1, SHU_ACTIM2_TRTP)
                    | P_Fld(0x0, SHU_ACTIM2_TXP));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM3, P_Fld(0x51, SHU_ACTIM3_TRFC)
                    | P_Fld(0x51, SHU_ACTIM3_REFCNT)
                    | P_Fld(0x23, SHU_ACTIM3_TRFCPB));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM4, P_Fld(0x1c, SHU_ACTIM4_TZQCS)
                    | P_Fld(0x63, SHU_ACTIM4_TXREFCNT));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM5, P_Fld(0x6, SHU_ACTIM5_TMRR2W)
                    | P_Fld(0xa, SHU_ACTIM5_TWTPD)
                    | P_Fld(0xa, SHU_ACTIM5_TR2PD));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM_XRT, P_Fld(0x8, SHU_ACTIM_XRT_XRTW2W)
                    | P_Fld(0x6, SHU_ACTIM_XRT_XRTR2W));
        vIO32WriteFldMulti(DRAMC_REG_SHU_AC_TIME_05T, P_Fld(0x0, SHU_AC_TIME_05T_TWTR_M05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TWTPD_M05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TFAW_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRRD_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TWR_M05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRAS_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRPAB_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRP_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRCD_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRTP_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TXP_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRFC_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRC_05T));
        vIO32WriteFldMulti(DRAMC_REG_SHU_CONF1, P_Fld(0xc, SHU_CONF1_DATLAT_DSEL_PHY)
                    | P_Fld(0xc, SHU_CONF1_DATLAT_DSEL)
                    | P_Fld(0xe, SHU_CONF1_DATLAT));
    }

    vIO32WriteFldAlign(DRAMC_REG_SHURK0_DQSCTL, 0x4, SHURK0_DQSCTL_DQSINCTL);
}
#endif /* LEGACY_SET_ACTIMING */

#if LEGACY_SET_ACTIMING
static void Legacy_DramcSet_LP4_DDR1600_ACTimingRegs(DRAMC_CTX_T *p)
{
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM0, P_Fld(0x3, SHU_ACTIM0_TRCD)
                    | P_Fld(0x0, SHU_ACTIM0_TRRD)
                    | P_Fld(0x8, SHU_ACTIM0_TWR)
                    | P_Fld(0x5, SHU_ACTIM0_TWTR));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM1, P_Fld(0x4, SHU_ACTIM1_TRC)
                    | P_Fld(0x0, SHU_ACTIM1_TRAS)
                    | P_Fld(0x2, SHU_ACTIM1_TRP)
                    | P_Fld(0x0, SHU_ACTIM1_TRPAB));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM2, P_Fld(0x0, SHU_ACTIM2_TFAW)
                    | P_Fld(0x3, SHU_ACTIM2_TR2W)
                    | P_Fld(0x1, SHU_ACTIM2_TRTP)
                    | P_Fld(0x0, SHU_ACTIM2_TXP));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM3, P_Fld(0x2c, SHU_ACTIM3_TRFC)
                    | P_Fld(0x30, SHU_ACTIM3_REFCNT)
                    | P_Fld(0x10, SHU_ACTIM3_TRFCPB));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM4, P_Fld(0x10, SHU_ACTIM4_TZQCS)
                    | P_Fld(0x3d, SHU_ACTIM4_TXREFCNT));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM5, P_Fld(0x3, SHU_ACTIM5_TMRR2W)
                    | P_Fld(0x7, SHU_ACTIM5_TWTPD)
                    | P_Fld(0x4, SHU_ACTIM5_TR2PD));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM_XRT, P_Fld(0x7, SHU_ACTIM_XRT_XRTW2W)
                    | P_Fld(0x3, SHU_ACTIM_XRT_XRTR2W));
        vIO32WriteFldMulti(DRAMC_REG_SHU_AC_TIME_05T, P_Fld(0x1, SHU_AC_TIME_05T_TWTR_M05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TR2W_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TWTPD_M05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TR2PD_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TFAW_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRRD_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TWR_M05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRAS_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRPAB_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRP_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRCD_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRTP_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TXP_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRFC_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRC_05T));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM0, P_Fld(0x3, SHU_ACTIM0_TRCD)
                    | P_Fld(0x0, SHU_ACTIM0_TRRD)
                    | P_Fld(0x7, SHU_ACTIM0_TWR)
                    | P_Fld(0x4, SHU_ACTIM0_TWTR));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM1, P_Fld(0x4, SHU_ACTIM1_TRC)
                    | P_Fld(0x0, SHU_ACTIM1_TRAS)
                    | P_Fld(0x2, SHU_ACTIM1_TRP)
                    | P_Fld(0x0, SHU_ACTIM1_TRPAB));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM2, P_Fld(0x0, SHU_ACTIM2_TFAW)
                    | P_Fld(0x2, SHU_ACTIM2_TR2W)
                    | P_Fld(0x1, SHU_ACTIM2_TRTP)
                    | P_Fld(0x0, SHU_ACTIM2_TXP));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM3, P_Fld(0x2c, SHU_ACTIM3_TRFC)
                    | P_Fld(0x30, SHU_ACTIM3_REFCNT)
                    | P_Fld(0x10, SHU_ACTIM3_TRFCPB));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM4, P_Fld(0x10, SHU_ACTIM4_TZQCS)
                    | P_Fld(0x3d, SHU_ACTIM4_TXREFCNT));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM5, P_Fld(0x3, SHU_ACTIM5_TMRR2W)
                    | P_Fld(0x6, SHU_ACTIM5_TWTPD)
                    | P_Fld(0x6, SHU_ACTIM5_TR2PD));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM_XRT, P_Fld(0x7, SHU_ACTIM_XRT_XRTW2W)
                    | P_Fld(0x3, SHU_ACTIM_XRT_XRTR2W));
        vIO32WriteFldMulti(DRAMC_REG_SHU_AC_TIME_05T, P_Fld(0x1, SHU_AC_TIME_05T_TWTR_M05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TWTPD_M05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TFAW_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRRD_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TWR_M05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRAS_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRPAB_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRP_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRCD_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRTP_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TXP_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRFC_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRC_05T));
    }

    vIO32WriteFldMulti(DRAMC_REG_SHU_CONF1, P_Fld(0xa, SHU_CONF1_DATLAT_DSEL_PHY)
                | P_Fld(0xa, SHU_CONF1_DATLAT_DSEL)
                | P_Fld(0xc, SHU_CONF1_DATLAT));
    vIO32WriteFldAlign(DRAMC_REG_SHURK0_DQSCTL, 0x2, SHURK0_DQSCTL_DQSINCTL);
}
#endif /* LEGACY_SET_ACTIMING */

#if LEGACY_SET_ACTIMING
static void Legacy_DramcSet_LP4_DDR800_ACTimingRegs(DRAMC_CTX_T *p)
{
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM0, P_Fld(0x3, SHU_ACTIM0_TRCD)
                    | P_Fld(0x0, SHU_ACTIM0_TRRD)
                    | P_Fld(0x8, SHU_ACTIM0_TWR)
                    | P_Fld(0x5, SHU_ACTIM0_TWTR));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM1, P_Fld(0x4, SHU_ACTIM1_TRC)
                    | P_Fld(0x0, SHU_ACTIM1_TRAS)
                    | P_Fld(0x2, SHU_ACTIM1_TRP)
                    | P_Fld(0x0, SHU_ACTIM1_TRPAB));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM2, P_Fld(0x0, SHU_ACTIM2_TFAW)
                    | P_Fld(0x3, SHU_ACTIM2_TR2W)
                    | P_Fld(0x1, SHU_ACTIM2_TRTP)
                    | P_Fld(0x0, SHU_ACTIM2_TXP));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM3, P_Fld(0x2c, SHU_ACTIM3_TRFC)
                    | P_Fld(0x30, SHU_ACTIM3_REFCNT)
                    | P_Fld(0x10, SHU_ACTIM3_TRFCPB));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM4, P_Fld(0x10, SHU_ACTIM4_TZQCS)
                    | P_Fld(0x3d, SHU_ACTIM4_TXREFCNT));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM5, P_Fld(0x3, SHU_ACTIM5_TMRR2W)
                    | P_Fld(0x7, SHU_ACTIM5_TWTPD)
                    | P_Fld(0x4, SHU_ACTIM5_TR2PD));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM_XRT, P_Fld(0x7, SHU_ACTIM_XRT_XRTW2W)
                    | P_Fld(0x3, SHU_ACTIM_XRT_XRTR2W));
        vIO32WriteFldMulti(DRAMC_REG_SHU_AC_TIME_05T, P_Fld(0x1, SHU_AC_TIME_05T_TWTR_M05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TR2W_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TWTPD_M05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TR2PD_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TFAW_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRRD_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TWR_M05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRAS_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRPAB_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRP_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRCD_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRTP_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TXP_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRFC_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRC_05T));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM0, P_Fld(0x3, SHU_ACTIM0_TRCD)
                    | P_Fld(0x0, SHU_ACTIM0_TRRD)
                    | P_Fld(0x7, SHU_ACTIM0_TWR)
                    | P_Fld(0x4, SHU_ACTIM0_TWTR));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM1, P_Fld(0x4, SHU_ACTIM1_TRC)
                    | P_Fld(0x0, SHU_ACTIM1_TRAS)
                    | P_Fld(0x2, SHU_ACTIM1_TRP)
                    | P_Fld(0x0, SHU_ACTIM1_TRPAB));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM2, P_Fld(0x0, SHU_ACTIM2_TFAW)
                    | P_Fld(0x2, SHU_ACTIM2_TR2W)
                    | P_Fld(0x1, SHU_ACTIM2_TRTP)
                    | P_Fld(0x0, SHU_ACTIM2_TXP));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM3, P_Fld(0x2c, SHU_ACTIM3_TRFC)
                    | P_Fld(0x30, SHU_ACTIM3_REFCNT)
                    | P_Fld(0x10, SHU_ACTIM3_TRFCPB));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM4, P_Fld(0x10, SHU_ACTIM4_TZQCS)
                    | P_Fld(0x3d, SHU_ACTIM4_TXREFCNT));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM5, P_Fld(0x3, SHU_ACTIM5_TMRR2W)
                    | P_Fld(0x6, SHU_ACTIM5_TWTPD)
                    | P_Fld(0x6, SHU_ACTIM5_TR2PD));
        vIO32WriteFldMulti(DRAMC_REG_SHU_ACTIM_XRT, P_Fld(0x7, SHU_ACTIM_XRT_XRTW2W)
                    | P_Fld(0x3, SHU_ACTIM_XRT_XRTR2W));
        vIO32WriteFldMulti(DRAMC_REG_SHU_AC_TIME_05T, P_Fld(0x1, SHU_AC_TIME_05T_TWTR_M05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TWTPD_M05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TFAW_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRRD_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TWR_M05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRAS_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRPAB_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRP_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRCD_05T)
                    | P_Fld(0x1, SHU_AC_TIME_05T_TRTP_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TXP_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRFC_05T)
                    | P_Fld(0x0, SHU_AC_TIME_05T_TRC_05T));
    }

    vIO32WriteFldMulti(DRAMC_REG_SHU_CONF1, P_Fld(0xa, SHU_CONF1_DATLAT_DSEL_PHY)
                | P_Fld(0xa, SHU_CONF1_DATLAT_DSEL)
                | P_Fld(0xc, SHU_CONF1_DATLAT));

    vIO32WriteFldAlign(DRAMC_REG_SHURK0_DQSCTL, 0x2, SHURK0_DQSCTL_DQSINCTL);
}
#endif /* LEGACY_SET_ACTIMING */


static void DramcSetting_Olympus_LP4_ByteMode_DDR3733(DRAMC_CTX_T *p)
{
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS0, P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS3)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS2)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS1)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS0)
                | P_Fld(0x4, SHU_SELPH_DQS0_TXDLY_DQS3)
                | P_Fld(0x4, SHU_SELPH_DQS0_TXDLY_DQS2)
                | P_Fld(0x4, SHU_SELPH_DQS0_TXDLY_DQS1)
                | P_Fld(0x4, SHU_SELPH_DQS0_TXDLY_DQS0));
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS1, P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS3)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS2)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS1)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS0)
                | P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS3)
                | P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS2)
                | P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS1)
                | P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS0));
}


static void DramcSetting_Olympus_LP4_ByteMode_DDR2667(DRAMC_CTX_T *p)
{
    ////DRAMC0-SHU2
    vIO32WriteFldMulti(DRAMC_REG_PRE_TDQSCK2, P_Fld(0x1a, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO0)
                | P_Fld(0x10, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO1)
                | P_Fld(0x0, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO2)
                | P_Fld(0x26, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO3));
    vIO32WriteFldMulti(DRAMC_REG_PRE_TDQSCK3, P_Fld(0x13, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO4)
                | P_Fld(0x0, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO5)
                | P_Fld(0x40, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO6)
                | P_Fld(0x35, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO7));
    vIO32WriteFldMulti(DRAMC_REG_PRE_TDQSCK4, P_Fld(0x0, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO8)
                | P_Fld(0x0, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO9)
                | P_Fld(0x0, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO10)
                | P_Fld(0x0, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO11));
    vIO32WriteFldAlign(DRAMC_REG_RK0_PRE_TDQSCK1, 0xa, RK0_PRE_TDQSCK1_TDQSCK_UIFREQ2_B0R0);
    vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK2, P_Fld(0x0, RK0_PRE_TDQSCK2_TDQSCK_PIFREQ4_B0R0)
                | P_Fld(0x0, RK0_PRE_TDQSCK2_TDQSCK_UIFREQ4_B0R0)
                | P_Fld(0xb, RK0_PRE_TDQSCK2_TDQSCK_PIFREQ3_B0R0)
                | P_Fld(0xd, RK0_PRE_TDQSCK2_TDQSCK_UIFREQ3_B0R0));
    vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK3, P_Fld(0x0, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ4_P1_B0R0)
                | P_Fld(0x11, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ3_P1_B0R0)
                | P_Fld(0xe, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ2_P1_B0R0));
    vIO32WriteFldAlign(DRAMC_REG_RK0_PRE_TDQSCK4, 0xa, RK0_PRE_TDQSCK4_TDQSCK_UIFREQ2_B1R0);
    vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK5, P_Fld(0x0, RK0_PRE_TDQSCK5_TDQSCK_PIFREQ4_B1R0)
                | P_Fld(0x0, RK0_PRE_TDQSCK5_TDQSCK_UIFREQ4_B1R0)
                | P_Fld(0xb, RK0_PRE_TDQSCK5_TDQSCK_PIFREQ3_B1R0)
                | P_Fld(0xd, RK0_PRE_TDQSCK5_TDQSCK_UIFREQ3_B1R0));
    vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK6, P_Fld(0x0, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ4_P1_B1R0)
                | P_Fld(0x11, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ3_P1_B1R0)
                | P_Fld(0xe, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ2_P1_B1R0));
    vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK1, P_Fld(0x8, RK1_PRE_TDQSCK1_TDQSCK_PIFREQ2_B0R1)
                | P_Fld(0x10, RK1_PRE_TDQSCK1_TDQSCK_UIFREQ2_B0R1));
    vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK2, P_Fld(0x0, RK1_PRE_TDQSCK2_TDQSCK_PIFREQ4_B0R1)
                | P_Fld(0x0, RK1_PRE_TDQSCK2_TDQSCK_UIFREQ4_B0R1)
                | P_Fld(0xe, RK1_PRE_TDQSCK2_TDQSCK_PIFREQ3_B0R1)
                | P_Fld(0x10, RK1_PRE_TDQSCK2_TDQSCK_UIFREQ3_B0R1));
    vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK3, P_Fld(0x0, RK1_PRE_TDQSCK3_TDQSCK_UIFREQ4_P1_B0R1)
                | P_Fld(0x14, RK1_PRE_TDQSCK3_TDQSCK_UIFREQ3_P1_B0R1)
                | P_Fld(0x14, RK1_PRE_TDQSCK3_TDQSCK_UIFREQ2_P1_B0R1));
    vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK4, P_Fld(0x8, RK1_PRE_TDQSCK4_TDQSCK_PIFREQ2_B1R1)
                | P_Fld(0x10, RK1_PRE_TDQSCK4_TDQSCK_UIFREQ2_B1R1));
    vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK5, P_Fld(0x0, RK1_PRE_TDQSCK5_TDQSCK_PIFREQ4_B1R1)
                | P_Fld(0x0, RK1_PRE_TDQSCK5_TDQSCK_UIFREQ4_B1R1)
                | P_Fld(0xe, RK1_PRE_TDQSCK5_TDQSCK_PIFREQ3_B1R1)
                | P_Fld(0x10, RK1_PRE_TDQSCK5_TDQSCK_UIFREQ3_B1R1));
    vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK6, P_Fld(0x0, RK1_PRE_TDQSCK6_TDQSCK_UIFREQ4_P1_B1R1)
                | P_Fld(0x14, RK1_PRE_TDQSCK6_TDQSCK_UIFREQ3_P1_B1R1)
                | P_Fld(0x14, RK1_PRE_TDQSCK6_TDQSCK_UIFREQ2_P1_B1R1));
#if LEGACY_SET_ACTIMING
    Legacy_DramcSet_LP4_DDR2667_ACTimingRegs(p);
#endif /* LEGACY_SET_ACTIMING */
    vIO32WriteFldAlign(DRAMC_REG_SHU_CONF2, 0x54, SHU_CONF2_FSPCHG_PRDCNT);
    vIO32WriteFldMulti(DRAMC_REG_SHU_RANKCTL, P_Fld(0x4, SHU_RANKCTL_RANKINCTL_PHY)
                | P_Fld(0x2, SHU_RANKCTL_RANKINCTL_ROOT1)
                | P_Fld(0x2, SHU_RANKCTL_RANKINCTL));
    vIO32WriteFldMulti(DRAMC_REG_SHU_CKECTRL, P_Fld(0x3, SHU_CKECTRL_TCKESRX));
                //| P_Fld(0x3, SHU_CKECTRL_CKEPRD));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ODTCTRL, P_Fld(0x1, SHU_ODTCTRL_RODTE)
                | P_Fld(0x1, SHU_ODTCTRL_RODTE2)
                | P_Fld(0x4, SHU_ODTCTRL_RODT)
                | P_Fld(0x1, SHU_ODTCTRL_ROEN));
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ7, 0x1, SHU1_B0_DQ7_R_DMRODTEN_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ7, 0x1, SHU1_B1_DQ7_R_DMRODTEN_B1);
    vIO32WriteFldMulti(DRAMC_REG_SHU1_DQSOSC_PRD, P_Fld(0xc, SHU1_DQSOSC_PRD_DQSOSCTHRD_DEC)
                | P_Fld(0xc, SHU1_DQSOSC_PRD_DQSOSCTHRD_INC)
                | P_Fld(0x10, SHU1_DQSOSC_PRD_DQSOSC_PRDCNT));
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS0, P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_OEN_DQS3)
                | P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_OEN_DQS2)
                | P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_OEN_DQS1)
                | P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_OEN_DQS0)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_DQS3)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_DQS2)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_DQS1)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_DQS0));
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS1, P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS3)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS2)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS1)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS0)
                | P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS3)
                | P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS2)
                | P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS1)
                | P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS0));
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldAlign(DRAMC_REG_SHU1_WODT, 0x0, SHU1_WODT_WPST2T);
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU1_WODT, P_Fld(0x0, SHU1_WODT_WPST2T)
                    | P_Fld(0x1, SHU1_WODT_DBIWR));
    }
    vIO32WriteFldAlign(DRAMC_REG_SHU_DQS2DQ_TX, 0x7, SHU_DQS2DQ_TX_OE2DQ_OFFSET);
    //vIO32WriteFldAlign(DRAMC_REG_SHU_HWSET_MR2, 0x24, SHU_HWSET_MR2_HWSET_MR2_OP);
    //vIO32WriteFldAlign(DRAMC_REG_SHU_HWSET_MR13, 0xc8, SHU_HWSET_MR13_HWSET_MR13_OP);
    //vIO32WriteFldAlign(DRAMC_REG_SHU_HWSET_VRCG, 0xc0, SHU_HWSET_VRCG_HWSET_VRCG_OP);
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQSIEN, P_Fld(0x19, SHURK0_DQSIEN_R0DQS1IEN)
                | P_Fld(0x19, SHURK0_DQSIEN_R0DQS0IEN));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_PI, P_Fld(0x14, SHU1RK0_PI_RK0_ARPI_DQM_B1)
                | P_Fld(0x14, SHU1RK0_PI_RK0_ARPI_DQM_B0)
                | P_Fld(0x14, SHU1RK0_PI_RK0_ARPI_DQ_B1)
                | P_Fld(0x14, SHU1RK0_PI_RK0_ARPI_DQ_B0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQSOSC, P_Fld(0x1ae, SHU1RK0_DQSOSC_DQSOSC_BASE_RK0_B1)
                | P_Fld(0x1ae, SHU1RK0_DQSOSC_DQSOSC_BASE_RK0));
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
       vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN0, P_Fld(0x1, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN_P1)
                    | P_Fld(0x1, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN)
                    | P_Fld(0x1, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN_P1)
                    | P_Fld(0x1, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN1, P_Fld(0x1, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN_P1)
                    | P_Fld(0x1, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN)
                    | P_Fld(0x1, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN_P1)
                    | P_Fld(0x1, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0, P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
                    | P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3)
                    | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2)
                    | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1)
                    | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0)
                    | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ3)
                    | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ2)
                    | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ1)
                    | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3)
                    | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2)
                    | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1)
                    | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0)
                    | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM3)
                    | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM2)
                    | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM1)
                    | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM0));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN0, P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN_P1)
                    | P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN)
                    | P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN_P1)
                    | P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN1, P_Fld(0x1, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN_P1)
                    | P_Fld(0x1, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN)
                    | P_Fld(0x1, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN_P1)
                    | P_Fld(0x1, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0, P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
                    | P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
                    | P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
                    | P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
                    | P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3)
                    | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2)
                    | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1)
                    | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0)
                    | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ3)
                    | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ2));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3)
                    | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2)
                    | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1)
                    | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0)
                    | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM3)
                    | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM2));
    }
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ2, P_Fld(0x0, SHURK0_SELPH_DQ2_DLY_OEN_DQ3)
                | P_Fld(0x0, SHURK0_SELPH_DQ2_DLY_OEN_DQ2)
                | P_Fld(0x0, SHURK0_SELPH_DQ2_DLY_OEN_DQ1)
                | P_Fld(0x0, SHURK0_SELPH_DQ2_DLY_OEN_DQ0)
                | P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_DQ3)
                | P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_DQ2)
                | P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_DQ1)
                | P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ3, P_Fld(0x0, SHURK0_SELPH_DQ3_DLY_OEN_DQM3)
                | P_Fld(0x0, SHURK0_SELPH_DQ3_DLY_OEN_DQM2)
                | P_Fld(0x0, SHURK0_SELPH_DQ3_DLY_OEN_DQM1)
                | P_Fld(0x0, SHURK0_SELPH_DQ3_DLY_OEN_DQM0)
                | P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_DQM3)
                | P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_DQM2)
                | P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_DQM1)
                | P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_DQM0));
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL1, P_Fld(0x354, SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ1)
                    | P_Fld(0x354, SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL2, P_Fld(0x354, SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ1)
                    | P_Fld(0x354, SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ0));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL1, P_Fld(0x254, SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ1)
                    | P_Fld(0x254, SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL2, P_Fld(0x254, SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ1)
                    | P_Fld(0x254, SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ0));
    }
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL3, P_Fld(0x14, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1_B4TO0)
                | P_Fld(0x14, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0_B4TO0)
                | P_Fld(0x18, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1)
                | P_Fld(0x18, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL4, P_Fld(0x14, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1_B4TO0)
                | P_Fld(0x14, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0_B4TO0)
                | P_Fld(0x18, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1)
                | P_Fld(0x18, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0));
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL5, P_Fld(0x354, SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM1)
                    | P_Fld(0x354, SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM0));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL5, P_Fld(0x254, SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM1)
                    | P_Fld(0x254, SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM0));
    }
    vIO32WriteFldAlign(DRAMC_REG_SHURK1_DQSCTL, 0x4, SHURK1_DQSCTL_R1DQSINCTL);
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQSIEN, P_Fld(0x1f, SHURK1_DQSIEN_R1DQS1IEN)
                | P_Fld(0x1f, SHURK1_DQSIEN_R1DQS0IEN));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_PI, P_Fld(0x14, SHU1RK1_PI_RK1_ARPI_DQM_B1)
                | P_Fld(0x14, SHU1RK1_PI_RK1_ARPI_DQM_B0)
                | P_Fld(0x14, SHU1RK1_PI_RK1_ARPI_DQ_B1)
                | P_Fld(0x14, SHU1RK1_PI_RK1_ARPI_DQ_B0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQSOSC, P_Fld(0x160, SHU1RK1_DQSOSC_DQSOSC_BASE_RK1_B1)
                | P_Fld(0x160, SHU1RK1_DQSOSC_DQSOSC_BASE_RK1));
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN0, P_Fld(0x1, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN_P1)
                    | P_Fld(0x1, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN)
                    | P_Fld(0x1, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN_P1)
                    | P_Fld(0x1, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN1, P_Fld(0x6, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN_P1)
                    | P_Fld(0x6, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN)
                    | P_Fld(0x6, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN_P1)
                    | P_Fld(0x6, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0, P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
                    | P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
                    | P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
                    | P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN0, P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN_P1)
                    | P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN)
                    | P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN_P1)
                    | P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN1, P_Fld(0x6, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN_P1)
                    | P_Fld(0x6, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN)
                    | P_Fld(0x6, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN_P1)
                    | P_Fld(0x6, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0, P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
                    | P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
                    | P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
                    | P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
    }
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1, P_Fld(0x4, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
                | P_Fld(0x0, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED)
                | P_Fld(0x4, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1)
                | P_Fld(0x0, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED));
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3)
                    | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2)
                    | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1)
                    | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0)
                    | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ3)
                    | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ2)
                    | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ1)
                    | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3)
                    | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2)
                    | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1)
                    | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0)
                    | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM3)
                    | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM2)
                    | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM1)
                    | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM0));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3)
                    | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2)
                    | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1)
                    | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0)
                    | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ3)
                    | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ2));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3)
                    | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2)
                    | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1)
                    | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0)
                    | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM3)
                    | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM2));
    }
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ2, P_Fld(0x0, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ3)
                | P_Fld(0x0, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ2)
                | P_Fld(0x0, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ1)
                | P_Fld(0x0, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ0)
                | P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1DQ3)
                | P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1DQ2)
                | P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1DQ1)
                | P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ3, P_Fld(0x0, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM3)
                | P_Fld(0x0, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM2)
                | P_Fld(0x0, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM1)
                | P_Fld(0x0, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM0)
                | P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1DQM3)
                | P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1DQM2)
                | P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1DQM1)
                | P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1DQM0));
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL1, P_Fld(0x354, SHU1RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ1)
                    | P_Fld(0x354, SHU1RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL2, P_Fld(0x354, SHU1RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ1)
                    | P_Fld(0x354, SHU1RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ0));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL1, P_Fld(0x254, SHU1RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ1)
                    | P_Fld(0x254, SHU1RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL2, P_Fld(0x254, SHU1RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ1)
                    | P_Fld(0x254, SHU1RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ0));
    }
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL3, P_Fld(0x14, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ1_B4TO0)
                | P_Fld(0x14, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ0_B4TO0)
                | P_Fld(0x18, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ1)
                | P_Fld(0x18, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL4, P_Fld(0x14, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM1_B4TO0)
                | P_Fld(0x14, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM0_B4TO0)
                | P_Fld(0x18, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM1)
                | P_Fld(0x18, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM0));
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL5, P_Fld(0x354, SHU1RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM1)
                    | P_Fld(0x354, SHU1RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM0));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL5, P_Fld(0x254, SHU1RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM1)
                    | P_Fld(0x254, SHU1RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM0));
    }
    vIO32WriteFldMulti(DRAMC_REG_SHU_DQSG_RETRY, P_Fld(0x4, SHU_DQSG_RETRY_R_DQSIENLAT)
                | P_Fld(0x1, SHU_DQSG_RETRY_R_DDR1866_PLUS));

    ////DDRPHY0-SHU2
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ5, 0x3, SHU1_B0_DQ5_RG_RX_ARDQS_DVS_DLY_B0);
    //vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ6, P_Fld(0x0, SHU1_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0)
    //            | P_Fld(0x1, SHU1_B0_DQ6_RG_ARPI_MIDPI_EN_B0));
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7, P_Fld(0x0, SHU1_B0_DQ7_MIDPI_DIV4_ENABLE)
                    | P_Fld(0x1, SHU1_B0_DQ7_MIDPI_ENABLE));
    }
    else
    {
        vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7, P_Fld(0x1, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0)
                    | P_Fld(0x1, SHU1_B0_DQ7_R_DMDQMDBI_SHU_B0)
                    | P_Fld(0x0, SHU1_B0_DQ7_MIDPI_DIV4_ENABLE)
                    | P_Fld(0x1, SHU1_B0_DQ7_MIDPI_ENABLE));
    }
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5, 0x3, SHU1_B1_DQ5_RG_RX_ARDQS_DVS_DLY_B1);
    //vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ6, P_Fld(0x0, SHU1_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1)
    //            | P_Fld(0x1, SHU1_B1_DQ6_RG_ARPI_MIDPI_EN_B1));
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
    }
    else
    {
        vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ7, P_Fld(0x1, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1)
                    | P_Fld(0x1, SHU1_B1_DQ7_R_DMDQMDBI_SHU_B1));
    }
    //DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    //vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD6, P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA)
    //            | P_Fld(0x1, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA));
    //vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD6+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA)
    //            | P_Fld(0x1, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA));
    //DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    //vIO32WriteFldAlign(DDRPHY_SHU1_PLL5, 0x3300, SHU1_PLL5_RG_RPHYPLL_SDM_PCW);
    //vIO32WriteFldAlign(DDRPHY_SHU1_PLL7, 0x3300, SHU1_PLL7_RG_RCLRPLL_SDM_PCW);
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ2, P_Fld(0x12, SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_R_DLY_B0)
                | P_Fld(0x12, SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ3, P_Fld(0x12, SHU1_R0_B0_DQ3_RK0_RX_ARDQ3_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ3_RK0_RX_ARDQ3_R_DLY_B0)
                | P_Fld(0x12, SHU1_R0_B0_DQ3_RK0_RX_ARDQ2_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ3_RK0_RX_ARDQ2_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ4, P_Fld(0x12, SHU1_R0_B0_DQ4_RK0_RX_ARDQ5_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ4_RK0_RX_ARDQ5_R_DLY_B0)
                | P_Fld(0x12, SHU1_R0_B0_DQ4_RK0_RX_ARDQ4_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ4_RK0_RX_ARDQ4_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ5, P_Fld(0x12, SHU1_R0_B0_DQ5_RK0_RX_ARDQ7_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ5_RK0_RX_ARDQ7_R_DLY_B0)
                | P_Fld(0x12, SHU1_R0_B0_DQ5_RK0_RX_ARDQ6_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ5_RK0_RX_ARDQ6_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ6, P_Fld(0x1d, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_F_DLY_B0)
                | P_Fld(0x15, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0)
                | P_Fld(0x12, SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ7, P_Fld(0x14, SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0)
                | P_Fld(0x14, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ2, P_Fld(0x12, SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ3, P_Fld(0x12, SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ4, P_Fld(0x12, SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ5, P_Fld(0x12, SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ6, P_Fld(0x1d, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0x15, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ7, P_Fld(0x14, SHU1_R0_B1_DQ7_RK0_ARPI_DQM_B1)
                | P_Fld(0x14, SHU1_R0_B1_DQ7_RK0_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ0, P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ7_DLY_B0)
                | P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ6_DLY_B0)
                | P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ5_DLY_B0)
                | P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ4_DLY_B0)
                | P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ3_DLY_B0)
                | P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ2_DLY_B0)
                | P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ1_DLY_B0)
                | P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ0_DLY_B0));
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_B0_DQ1, 0xa, SHU1_R1_B0_DQ1_RK1_TX_ARDQM0_DLY_B0);
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ2, P_Fld(0x13, SHU1_R1_B0_DQ2_RK1_RX_ARDQ1_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ2_RK1_RX_ARDQ1_R_DLY_B0)
                | P_Fld(0x13, SHU1_R1_B0_DQ2_RK1_RX_ARDQ0_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ2_RK1_RX_ARDQ0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ3, P_Fld(0x13, SHU1_R1_B0_DQ3_RK1_RX_ARDQ3_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ3_RK1_RX_ARDQ3_R_DLY_B0)
                | P_Fld(0x13, SHU1_R1_B0_DQ3_RK1_RX_ARDQ2_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ3_RK1_RX_ARDQ2_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ4, P_Fld(0x13, SHU1_R1_B0_DQ4_RK1_RX_ARDQ5_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ4_RK1_RX_ARDQ5_R_DLY_B0)
                | P_Fld(0x13, SHU1_R1_B0_DQ4_RK1_RX_ARDQ4_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ4_RK1_RX_ARDQ4_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ5, P_Fld(0x13, SHU1_R1_B0_DQ5_RK1_RX_ARDQ7_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ5_RK1_RX_ARDQ7_R_DLY_B0)
                | P_Fld(0x13, SHU1_R1_B0_DQ5_RK1_RX_ARDQ6_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ5_RK1_RX_ARDQ6_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ6, P_Fld(0x1e, SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_F_DLY_B0)
                | P_Fld(0x16, SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_R_DLY_B0)
                | P_Fld(0x13, SHU1_R1_B0_DQ6_RK1_RX_ARDQM0_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ6_RK1_RX_ARDQM0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ7, P_Fld(0x14, SHU1_R1_B0_DQ7_RK1_ARPI_DQM_B0)
                | P_Fld(0x14, SHU1_R1_B0_DQ7_RK1_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ0, P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ7_DLY_B1)
                | P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ6_DLY_B1)
                | P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ5_DLY_B1)
                | P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ4_DLY_B1)
                | P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ3_DLY_B1)
                | P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ2_DLY_B1)
                | P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ1_DLY_B1)
                | P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ0_DLY_B1));
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_B1_DQ1, 0xa, SHU1_R1_B1_DQ1_RK1_TX_ARDQM0_DLY_B1);
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ2, P_Fld(0x13, SHU1_R1_B1_DQ2_RK1_RX_ARDQ1_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ2_RK1_RX_ARDQ1_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ2_RK1_RX_ARDQ0_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ2_RK1_RX_ARDQ0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ3, P_Fld(0x13, SHU1_R1_B1_DQ3_RK1_RX_ARDQ3_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ3_RK1_RX_ARDQ3_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ3_RK1_RX_ARDQ2_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ3_RK1_RX_ARDQ2_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ4, P_Fld(0x13, SHU1_R1_B1_DQ4_RK1_RX_ARDQ5_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ4_RK1_RX_ARDQ5_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ4_RK1_RX_ARDQ4_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ4_RK1_RX_ARDQ4_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ5, P_Fld(0x13, SHU1_R1_B1_DQ5_RK1_RX_ARDQ7_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ5_RK1_RX_ARDQ7_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ5_RK1_RX_ARDQ6_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ5_RK1_RX_ARDQ6_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ6, P_Fld(0x1e, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0x16, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ6_RK1_RX_ARDQM0_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ6_RK1_RX_ARDQM0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ7, P_Fld(0x14, SHU1_R1_B1_DQ7_RK1_ARPI_DQM_B1)
                | P_Fld(0x14, SHU1_R1_B1_DQ7_RK1_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B0_DQ0, P_Fld(0xa, SHU1_R2_B0_DQ0_RK2_TX_ARDQ7_DLY_B0)
                | P_Fld(0xa, SHU1_R2_B0_DQ0_RK2_TX_ARDQ6_DLY_B0)
                | P_Fld(0xa, SHU1_R2_B0_DQ0_RK2_TX_ARDQ5_DLY_B0)
                | P_Fld(0xa, SHU1_R2_B0_DQ0_RK2_TX_ARDQ4_DLY_B0)
                | P_Fld(0xa, SHU1_R2_B0_DQ0_RK2_TX_ARDQ3_DLY_B0)
                | P_Fld(0xa, SHU1_R2_B0_DQ0_RK2_TX_ARDQ2_DLY_B0)
                | P_Fld(0xa, SHU1_R2_B0_DQ0_RK2_TX_ARDQ1_DLY_B0)
                | P_Fld(0xa, SHU1_R2_B0_DQ0_RK2_TX_ARDQ0_DLY_B0));
    vIO32WriteFldAlign(DDRPHY_SHU1_R2_B0_DQ1, 0xa, SHU1_R2_B0_DQ1_RK2_TX_ARDQM0_DLY_B0);
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B0_DQ7, P_Fld(0x1f, SHU1_R2_B0_DQ7_RK2_ARPI_DQM_B0)
                | P_Fld(0x1f, SHU1_R2_B0_DQ7_RK2_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B1_DQ0, P_Fld(0xa, SHU1_R2_B1_DQ0_RK2_TX_ARDQ7_DLY_B1)
                | P_Fld(0xa, SHU1_R2_B1_DQ0_RK2_TX_ARDQ6_DLY_B1)
                | P_Fld(0xa, SHU1_R2_B1_DQ0_RK2_TX_ARDQ5_DLY_B1)
                | P_Fld(0xa, SHU1_R2_B1_DQ0_RK2_TX_ARDQ4_DLY_B1)
                | P_Fld(0xa, SHU1_R2_B1_DQ0_RK2_TX_ARDQ3_DLY_B1)
                | P_Fld(0xa, SHU1_R2_B1_DQ0_RK2_TX_ARDQ2_DLY_B1)
                | P_Fld(0xa, SHU1_R2_B1_DQ0_RK2_TX_ARDQ1_DLY_B1)
                | P_Fld(0xa, SHU1_R2_B1_DQ0_RK2_TX_ARDQ0_DLY_B1));
    vIO32WriteFldAlign(DDRPHY_SHU1_R2_B1_DQ1, 0xa, SHU1_R2_B1_DQ1_RK2_TX_ARDQM0_DLY_B1);
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B1_DQ7, P_Fld(0x1f, SHU1_R2_B1_DQ7_RK2_ARPI_DQM_B1)
                | P_Fld(0x1f, SHU1_R2_B1_DQ7_RK2_ARPI_DQ_B1));

}

#if 0
static void DramcSetting_Olympus_LP4_ByteMode_DDR2400(DRAMC_CTX_T *p)
{}
#endif

static void DramcSetting_Olympus_LP4_ByteMode_DDR1600(DRAMC_CTX_T *p)
{
    ////DRAMC0-SHU3
    vIO32WriteFldMulti(DRAMC_REG_PRE_TDQSCK2, P_Fld(0x1a, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO0)
                | P_Fld(0x10, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO1)
                | P_Fld(0x0, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO2)
                | P_Fld(0x26, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO3));
    vIO32WriteFldMulti(DRAMC_REG_PRE_TDQSCK3, P_Fld(0x13, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO4)
                | P_Fld(0x0, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO5)
                | P_Fld(0x40, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO6)
                | P_Fld(0x35, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO7));
    vIO32WriteFldMulti(DRAMC_REG_PRE_TDQSCK4, P_Fld(0x0, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO8)
                | P_Fld(0x0, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO9)
                | P_Fld(0x0, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO10)
                | P_Fld(0x0, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO11));
    vIO32WriteFldAlign(DRAMC_REG_RK0_PRE_TDQSCK1, 0xa, RK0_PRE_TDQSCK1_TDQSCK_UIFREQ2_B0R0);
    vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK2, P_Fld(0x0, RK0_PRE_TDQSCK2_TDQSCK_PIFREQ4_B0R0)
                | P_Fld(0x0, RK0_PRE_TDQSCK2_TDQSCK_UIFREQ4_B0R0)
                | P_Fld(0xb, RK0_PRE_TDQSCK2_TDQSCK_PIFREQ3_B0R0)
                | P_Fld(0xd, RK0_PRE_TDQSCK2_TDQSCK_UIFREQ3_B0R0));
    vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK3, P_Fld(0x0, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ4_P1_B0R0)
                | P_Fld(0x11, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ3_P1_B0R0)
                | P_Fld(0xe, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ2_P1_B0R0));
    vIO32WriteFldAlign(DRAMC_REG_RK0_PRE_TDQSCK4, 0xa, RK0_PRE_TDQSCK4_TDQSCK_UIFREQ2_B1R0);
    vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK5, P_Fld(0x0, RK0_PRE_TDQSCK5_TDQSCK_PIFREQ4_B1R0)
                | P_Fld(0x0, RK0_PRE_TDQSCK5_TDQSCK_UIFREQ4_B1R0)
                | P_Fld(0xb, RK0_PRE_TDQSCK5_TDQSCK_PIFREQ3_B1R0)
                | P_Fld(0xd, RK0_PRE_TDQSCK5_TDQSCK_UIFREQ3_B1R0));
    vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK6, P_Fld(0x0, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ4_P1_B1R0)
                | P_Fld(0x11, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ3_P1_B1R0)
                | P_Fld(0xe, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ2_P1_B1R0));
    vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK1, P_Fld(0x8, RK1_PRE_TDQSCK1_TDQSCK_PIFREQ2_B0R1)
                | P_Fld(0x10, RK1_PRE_TDQSCK1_TDQSCK_UIFREQ2_B0R1));
    vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK2, P_Fld(0x0, RK1_PRE_TDQSCK2_TDQSCK_PIFREQ4_B0R1)
                | P_Fld(0x0, RK1_PRE_TDQSCK2_TDQSCK_UIFREQ4_B0R1)
                | P_Fld(0xe, RK1_PRE_TDQSCK2_TDQSCK_PIFREQ3_B0R1)
                | P_Fld(0x10, RK1_PRE_TDQSCK2_TDQSCK_UIFREQ3_B0R1));
    vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK3, P_Fld(0x0, RK1_PRE_TDQSCK3_TDQSCK_UIFREQ4_P1_B0R1)
                | P_Fld(0x14, RK1_PRE_TDQSCK3_TDQSCK_UIFREQ3_P1_B0R1)
                | P_Fld(0x14, RK1_PRE_TDQSCK3_TDQSCK_UIFREQ2_P1_B0R1));
    vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK4, P_Fld(0x8, RK1_PRE_TDQSCK4_TDQSCK_PIFREQ2_B1R1)
                | P_Fld(0x10, RK1_PRE_TDQSCK4_TDQSCK_UIFREQ2_B1R1));
    vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK5, P_Fld(0x0, RK1_PRE_TDQSCK5_TDQSCK_PIFREQ4_B1R1)
                | P_Fld(0x0, RK1_PRE_TDQSCK5_TDQSCK_UIFREQ4_B1R1)
                | P_Fld(0xe, RK1_PRE_TDQSCK5_TDQSCK_PIFREQ3_B1R1)
                | P_Fld(0x10, RK1_PRE_TDQSCK5_TDQSCK_UIFREQ3_B1R1));
    vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK6, P_Fld(0x0, RK1_PRE_TDQSCK6_TDQSCK_UIFREQ4_P1_B1R1)
                | P_Fld(0x14, RK1_PRE_TDQSCK6_TDQSCK_UIFREQ3_P1_B1R1)
                | P_Fld(0x14, RK1_PRE_TDQSCK6_TDQSCK_UIFREQ2_P1_B1R1));
#if LEGACY_SET_ACTIMING
    Legacy_DramcSet_LP4_DDR1600_ACTimingRegs(p);
#endif /* LEGACY_SET_ACTIMING */
    vIO32WriteFldAlign(DRAMC_REG_SHU_CONF2, 0x32, SHU_CONF2_FSPCHG_PRDCNT);
    vIO32WriteFldMulti(DRAMC_REG_SHU_RANKCTL, P_Fld(0x2, SHU_RANKCTL_RANKINCTL_PHY)
                | P_Fld(0x0, SHU_RANKCTL_RANKINCTL_ROOT1)
                | P_Fld(0x0, SHU_RANKCTL_RANKINCTL));
    vIO32WriteFldMulti(DRAMC_REG_SHU_CKECTRL, P_Fld(0x0, SHU_CKECTRL_TCKESRX));
                //| P_Fld(0x2, SHU_CKECTRL_CKEPRD));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ODTCTRL, P_Fld(0x0, SHU_ODTCTRL_RODTE)
                | P_Fld(0x0, SHU_ODTCTRL_RODTE2)
                | P_Fld(0x2, SHU_ODTCTRL_RODT)
#ifdef LOOPBACK_TEST
                | P_Fld(0x1, SHU_ODTCTRL_ROEN));
#else
                | P_Fld(0x0, SHU_ODTCTRL_ROEN));
#endif
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ7, 0x0, SHU1_B0_DQ7_R_DMRODTEN_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ7, 0x0, SHU1_B1_DQ7_R_DMRODTEN_B1);
    vIO32WriteFldMulti(DRAMC_REG_SHU1_DQSOSC_PRD, P_Fld(0x14, SHU1_DQSOSC_PRD_DQSOSCTHRD_DEC)
                | P_Fld(0x14, SHU1_DQSOSC_PRD_DQSOSCTHRD_INC)
                | P_Fld(0xf, SHU1_DQSOSC_PRD_DQSOSC_PRDCNT));
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS0, P_Fld(0x1, SHU_SELPH_DQS0_TXDLY_OEN_DQS3)
                | P_Fld(0x1, SHU_SELPH_DQS0_TXDLY_OEN_DQS2)
                | P_Fld(0x1, SHU_SELPH_DQS0_TXDLY_OEN_DQS1)
                | P_Fld(0x1, SHU_SELPH_DQS0_TXDLY_OEN_DQS0)
                | P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_DQS3)
                | P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_DQS2)
                | P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_DQS1)
                | P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_DQS0));
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS1, P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS3)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS2)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS1)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS0)
                | P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS3)
                | P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS2)
                | P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS1)
                | P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS0));
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldAlign(DRAMC_REG_SHU1_WODT, 0x1, SHU1_WODT_WPST2T);
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU1_WODT, P_Fld(0x1, SHU1_WODT_WPST2T)
                    | P_Fld(0x0, SHU1_WODT_DBIWR));
    }
    vIO32WriteFldAlign(DRAMC_REG_SHU_DQS2DQ_TX, 0x4, SHU_DQS2DQ_TX_OE2DQ_OFFSET);
    //vIO32WriteFldAlign(DRAMC_REG_SHU_HWSET_MR2, 0x12, SHU_HWSET_MR2_HWSET_MR2_OP);
    //vIO32WriteFldAlign(DRAMC_REG_SHU_HWSET_MR13, 0x8, SHU_HWSET_MR13_HWSET_MR13_OP);
    //vIO32WriteFldAlign(DRAMC_REG_SHU_HWSET_VRCG, 0x0, SHU_HWSET_VRCG_HWSET_VRCG_OP);
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQSIEN, P_Fld(0x0, SHURK0_DQSIEN_R0DQS1IEN)
                | P_Fld(0x0, SHURK0_DQSIEN_R0DQS0IEN));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_PI, P_Fld(0x1a, SHU1RK0_PI_RK0_ARPI_DQM_B1)
                | P_Fld(0x1a, SHU1RK0_PI_RK0_ARPI_DQM_B0)
                | P_Fld(0x1a, SHU1RK0_PI_RK0_ARPI_DQ_B1)
                | P_Fld(0x1a, SHU1RK0_PI_RK0_ARPI_DQ_B0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQSOSC, P_Fld(0x2d0, SHU1RK0_DQSOSC_DQSOSC_BASE_RK0_B1)
                | P_Fld(0x2d0, SHU1RK0_DQSOSC_DQSOSC_BASE_RK0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN0, P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN_P1)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN_P1)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN1, P_Fld(0x5, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN_P1)
                | P_Fld(0x5, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN)
                | P_Fld(0x5, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN_P1)
                | P_Fld(0x5, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN));
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0, P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
                    | P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3)
                    | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2)
                    | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1)
                    | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0)
                    | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ3)
                    | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ2)
                    | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ1)
                    | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3)
                    | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2)
                    | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1)
                    | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0)
                    | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM3)
                    | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM2)
                    | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM1)
                    | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM0));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0, P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
                    | P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(0x0, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
                    | P_Fld(0x4, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
                    | P_Fld(0x0, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
                    | P_Fld(0x4, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3)
                    | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2)
                    | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1)
                    | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0)
                    | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ3)
                    | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ2));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3)
                    | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2)
                    | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1)
                    | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0)
                    | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM3)
                    | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM2));
    }
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ2, P_Fld(0x7, SHURK0_SELPH_DQ2_DLY_OEN_DQ3)
                | P_Fld(0x7, SHURK0_SELPH_DQ2_DLY_OEN_DQ2)
                | P_Fld(0x7, SHURK0_SELPH_DQ2_DLY_OEN_DQ1)
                | P_Fld(0x7, SHURK0_SELPH_DQ2_DLY_OEN_DQ0)
                | P_Fld(0x1, SHURK0_SELPH_DQ2_DLY_DQ3)
                | P_Fld(0x1, SHURK0_SELPH_DQ2_DLY_DQ2)
                | P_Fld(0x1, SHURK0_SELPH_DQ2_DLY_DQ1)
                | P_Fld(0x1, SHURK0_SELPH_DQ2_DLY_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ3, P_Fld(0x7, SHURK0_SELPH_DQ3_DLY_OEN_DQM3)
                | P_Fld(0x7, SHURK0_SELPH_DQ3_DLY_OEN_DQM2)
                | P_Fld(0x7, SHURK0_SELPH_DQ3_DLY_OEN_DQM1)
                | P_Fld(0x7, SHURK0_SELPH_DQ3_DLY_OEN_DQM0)
                | P_Fld(0x1, SHURK0_SELPH_DQ3_DLY_DQM3)
                | P_Fld(0x1, SHURK0_SELPH_DQ3_DLY_DQM2)
                | P_Fld(0x1, SHURK0_SELPH_DQ3_DLY_DQM1)
                | P_Fld(0x1, SHURK0_SELPH_DQ3_DLY_DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL1, P_Fld(0x23a, SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ1)
                | P_Fld(0x23a, SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL2, P_Fld(0x23a, SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ1)
                | P_Fld(0x23a, SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL3, P_Fld(0x1a, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1_B4TO0)
                | P_Fld(0x1a, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0_B4TO0)
                | P_Fld(0xf, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1)
                | P_Fld(0xf, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL4, P_Fld(0x1a, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1_B4TO0)
                | P_Fld(0x1a, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0_B4TO0)
                | P_Fld(0xf, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1)
                | P_Fld(0xf, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL5, P_Fld(0x23e, SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM1)
                | P_Fld(0x23e, SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM0));
    vIO32WriteFldAlign(DRAMC_REG_SHURK1_DQSCTL, 0x2, SHURK1_DQSCTL_R1DQSINCTL);
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQSIEN, P_Fld(0x0, SHURK1_DQSIEN_R1DQS1IEN)
                | P_Fld(0x0, SHURK1_DQSIEN_R1DQS0IEN));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_PI, P_Fld(0x1e, SHU1RK1_PI_RK1_ARPI_DQM_B1)
                | P_Fld(0x1e, SHU1RK1_PI_RK1_ARPI_DQM_B0)
                | P_Fld(0x1e, SHU1RK1_PI_RK1_ARPI_DQ_B1)
                | P_Fld(0x1e, SHU1RK1_PI_RK1_ARPI_DQ_B0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQSOSC, P_Fld(0x24e, SHU1RK1_DQSOSC_DQSOSC_BASE_RK1_B1)
                | P_Fld(0x24e, SHU1RK1_DQSOSC_DQSOSC_BASE_RK1));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN0, P_Fld(0x1, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN_P1)
                | P_Fld(0x1, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN)
                | P_Fld(0x1, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN_P1)
                | P_Fld(0x1, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN1, P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN_P1)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN_P1)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN));
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0, P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
                    | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
                    | P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
                    | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1, P_Fld(0x0, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
                    | P_Fld(0x4, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED)
                    | P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1)
                    | P_Fld(0x5, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3)
                    | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2)
                    | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1)
                    | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0)
                    | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ3)
                    | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ2)
                    | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ1)
                    | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3)
                    | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2)
                    | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1)
                    | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0)
                    | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM3)
                    | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM2)
                    | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM1)
                    | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM0));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0, P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
                    | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
                    | P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
                    | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1, P_Fld(0x3, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
                    | P_Fld(0x7, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED)
                    | P_Fld(0x3, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1)
                    | P_Fld(0x7, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3)
                    | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2)
                    | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1)
                    | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0)
                    | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ3)
                    | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ2));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3)
                    | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2)
                    | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1)
                    | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0)
                    | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM3)
                    | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM2));
    }
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ2, P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ3)
                | P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ2)
                | P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ1)
                | P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ0)
                | P_Fld(0x1, SHURK1_SELPH_DQ2_DLY_R1DQ3)
                | P_Fld(0x1, SHURK1_SELPH_DQ2_DLY_R1DQ2)
                | P_Fld(0x1, SHURK1_SELPH_DQ2_DLY_R1DQ1)
                | P_Fld(0x1, SHURK1_SELPH_DQ2_DLY_R1DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ3, P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM3)
                | P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM2)
                | P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM1)
                | P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM0)
                | P_Fld(0x1, SHURK1_SELPH_DQ3_DLY_R1DQM3)
                | P_Fld(0x1, SHURK1_SELPH_DQ3_DLY_R1DQM2)
                | P_Fld(0x1, SHURK1_SELPH_DQ3_DLY_R1DQM1)
                | P_Fld(0x1, SHURK1_SELPH_DQ3_DLY_R1DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL1, P_Fld(0x23e, SHU1RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ1)
                | P_Fld(0x23e, SHU1RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL2, P_Fld(0x23e, SHU1RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ1)
                | P_Fld(0x23e, SHU1RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL3, P_Fld(0x1e, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ1_B4TO0)
                | P_Fld(0x1e, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ0_B4TO0)
                | P_Fld(0xf, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ1)
                | P_Fld(0xf, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL4, P_Fld(0x1e, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM1_B4TO0)
                | P_Fld(0x1e, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM0_B4TO0)
                | P_Fld(0xf, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM1)
                | P_Fld(0xf, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL5, P_Fld(0x23e, SHU1RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM1)
                | P_Fld(0x23e, SHU1RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHU_DQSG_RETRY, P_Fld(0x3, SHU_DQSG_RETRY_R_DQSIENLAT)
                | P_Fld(0x0, SHU_DQSG_RETRY_R_DDR1866_PLUS));

    ////DDRPHY0-SHU3
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ5, 0x4, SHU1_B0_DQ5_RG_RX_ARDQS_DVS_DLY_B0);
    //vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ6, P_Fld(0x1, SHU1_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0)
    //            | P_Fld(0x0, SHU1_B0_DQ6_RG_ARPI_MIDPI_EN_B0));
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7, P_Fld(0x1, SHU1_B0_DQ7_MIDPI_DIV4_ENABLE)
                    | P_Fld(0x0, SHU1_B0_DQ7_MIDPI_ENABLE));
    }
    else
    {
        vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7, P_Fld(0x0, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0)
                    | P_Fld(0x0, SHU1_B0_DQ7_R_DMDQMDBI_SHU_B0)
                    | P_Fld(0x1, SHU1_B0_DQ7_MIDPI_DIV4_ENABLE)
                    | P_Fld(0x0, SHU1_B0_DQ7_MIDPI_ENABLE));
    }
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5, 0x4, SHU1_B1_DQ5_RG_RX_ARDQS_DVS_DLY_B1);
    //vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ6, P_Fld(0x1, SHU1_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1)
    //            | P_Fld(0x0, SHU1_B1_DQ6_RG_ARPI_MIDPI_EN_B1));
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {}
    else
    {
        vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ7, P_Fld(0x0, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1)
                    | P_Fld(0x0, SHU1_B1_DQ7_R_DMDQMDBI_SHU_B1));
    }
    //DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    //vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD6, P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA)
    //            | P_Fld(0x1, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA));
    //vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD6+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA)
    //            | P_Fld(0x1, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA));
    //DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    //vIO32WriteFldAlign(DDRPHY_SHU1_PLL5, 0x3d00, SHU1_PLL5_RG_RPHYPLL_SDM_PCW);
    //vIO32WriteFldAlign(DDRPHY_SHU1_PLL7, 0x3d00, SHU1_PLL7_RG_RCLRPLL_SDM_PCW);
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ2, P_Fld(0xc, SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_F_DLY_B0)
                | P_Fld(0x4, SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_R_DLY_B0)
                | P_Fld(0xc, SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_F_DLY_B0)
                | P_Fld(0x4, SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ3, P_Fld(0xc, SHU1_R0_B0_DQ3_RK0_RX_ARDQ3_F_DLY_B0)
                | P_Fld(0x4, SHU1_R0_B0_DQ3_RK0_RX_ARDQ3_R_DLY_B0)
                | P_Fld(0xc, SHU1_R0_B0_DQ3_RK0_RX_ARDQ2_F_DLY_B0)
                | P_Fld(0x4, SHU1_R0_B0_DQ3_RK0_RX_ARDQ2_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ4, P_Fld(0xc, SHU1_R0_B0_DQ4_RK0_RX_ARDQ5_F_DLY_B0)
                | P_Fld(0x4, SHU1_R0_B0_DQ4_RK0_RX_ARDQ5_R_DLY_B0)
                | P_Fld(0xc, SHU1_R0_B0_DQ4_RK0_RX_ARDQ4_F_DLY_B0)
                | P_Fld(0x4, SHU1_R0_B0_DQ4_RK0_RX_ARDQ4_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ5, P_Fld(0xc, SHU1_R0_B0_DQ5_RK0_RX_ARDQ7_F_DLY_B0)
                | P_Fld(0x4, SHU1_R0_B0_DQ5_RK0_RX_ARDQ7_R_DLY_B0)
                | P_Fld(0xc, SHU1_R0_B0_DQ5_RK0_RX_ARDQ6_F_DLY_B0)
                | P_Fld(0x4, SHU1_R0_B0_DQ5_RK0_RX_ARDQ6_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ6, P_Fld(0x1d, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_F_DLY_B0)
                | P_Fld(0x15, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0)
                | P_Fld(0xc, SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_F_DLY_B0)
                | P_Fld(0x4, SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ7, P_Fld(0x1a, SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0)
                | P_Fld(0x1a, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ2, P_Fld(0xc, SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_F_DLY_B1)
                | P_Fld(0x4, SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_R_DLY_B1)
                | P_Fld(0xc, SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_F_DLY_B1)
                | P_Fld(0x4, SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ3, P_Fld(0xc, SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_F_DLY_B1)
                | P_Fld(0x4, SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_R_DLY_B1)
                | P_Fld(0xc, SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_F_DLY_B1)
                | P_Fld(0x4, SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ4, P_Fld(0xc, SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_F_DLY_B1)
                | P_Fld(0x4, SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_R_DLY_B1)
                | P_Fld(0xc, SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_F_DLY_B1)
                | P_Fld(0x4, SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ5, P_Fld(0xc, SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_F_DLY_B1)
                | P_Fld(0x4, SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_R_DLY_B1)
                | P_Fld(0xc, SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_F_DLY_B1)
                | P_Fld(0x4, SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ6, P_Fld(0x1d, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0x15, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1)
                | P_Fld(0xc, SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_F_DLY_B1)
                | P_Fld(0x4, SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ7, P_Fld(0x1a, SHU1_R0_B1_DQ7_RK0_ARPI_DQM_B1)
                | P_Fld(0x1a, SHU1_R0_B1_DQ7_RK0_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ0, P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ7_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ6_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ5_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ4_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ3_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ2_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ1_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ0_DLY_B0));
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_B0_DQ1, 0xd, SHU1_R1_B0_DQ1_RK1_TX_ARDQM0_DLY_B0);
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ2, P_Fld(0xd, SHU1_R1_B0_DQ2_RK1_RX_ARDQ1_F_DLY_B0)
                | P_Fld(0x5, SHU1_R1_B0_DQ2_RK1_RX_ARDQ1_R_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ2_RK1_RX_ARDQ0_F_DLY_B0)
                | P_Fld(0x5, SHU1_R1_B0_DQ2_RK1_RX_ARDQ0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ3, P_Fld(0xd, SHU1_R1_B0_DQ3_RK1_RX_ARDQ3_F_DLY_B0)
                | P_Fld(0x5, SHU1_R1_B0_DQ3_RK1_RX_ARDQ3_R_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ3_RK1_RX_ARDQ2_F_DLY_B0)
                | P_Fld(0x5, SHU1_R1_B0_DQ3_RK1_RX_ARDQ2_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ4, P_Fld(0xd, SHU1_R1_B0_DQ4_RK1_RX_ARDQ5_F_DLY_B0)
                | P_Fld(0x5, SHU1_R1_B0_DQ4_RK1_RX_ARDQ5_R_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ4_RK1_RX_ARDQ4_F_DLY_B0)
                | P_Fld(0x5, SHU1_R1_B0_DQ4_RK1_RX_ARDQ4_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ5, P_Fld(0xd, SHU1_R1_B0_DQ5_RK1_RX_ARDQ7_F_DLY_B0)
                | P_Fld(0x5, SHU1_R1_B0_DQ5_RK1_RX_ARDQ7_R_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ5_RK1_RX_ARDQ6_F_DLY_B0)
                | P_Fld(0x5, SHU1_R1_B0_DQ5_RK1_RX_ARDQ6_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ6, P_Fld(0x1e, SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_F_DLY_B0)
                | P_Fld(0x16, SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_R_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ6_RK1_RX_ARDQM0_F_DLY_B0)
                | P_Fld(0x5, SHU1_R1_B0_DQ6_RK1_RX_ARDQM0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ7, P_Fld(0x1e, SHU1_R1_B0_DQ7_RK1_ARPI_DQM_B0)
                | P_Fld(0x1e, SHU1_R1_B0_DQ7_RK1_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ0, P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ7_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ6_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ5_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ4_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ3_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ2_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ1_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ0_DLY_B1));
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_B1_DQ1, 0xd, SHU1_R1_B1_DQ1_RK1_TX_ARDQM0_DLY_B1);
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ2, P_Fld(0xd, SHU1_R1_B1_DQ2_RK1_RX_ARDQ1_F_DLY_B1)
                | P_Fld(0x5, SHU1_R1_B1_DQ2_RK1_RX_ARDQ1_R_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ2_RK1_RX_ARDQ0_F_DLY_B1)
                | P_Fld(0x5, SHU1_R1_B1_DQ2_RK1_RX_ARDQ0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ3, P_Fld(0xd, SHU1_R1_B1_DQ3_RK1_RX_ARDQ3_F_DLY_B1)
                | P_Fld(0x5, SHU1_R1_B1_DQ3_RK1_RX_ARDQ3_R_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ3_RK1_RX_ARDQ2_F_DLY_B1)
                | P_Fld(0x5, SHU1_R1_B1_DQ3_RK1_RX_ARDQ2_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ4, P_Fld(0xd, SHU1_R1_B1_DQ4_RK1_RX_ARDQ5_F_DLY_B1)
                | P_Fld(0x5, SHU1_R1_B1_DQ4_RK1_RX_ARDQ5_R_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ4_RK1_RX_ARDQ4_F_DLY_B1)
                | P_Fld(0x5, SHU1_R1_B1_DQ4_RK1_RX_ARDQ4_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ5, P_Fld(0xd, SHU1_R1_B1_DQ5_RK1_RX_ARDQ7_F_DLY_B1)
                | P_Fld(0x5, SHU1_R1_B1_DQ5_RK1_RX_ARDQ7_R_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ5_RK1_RX_ARDQ6_F_DLY_B1)
                | P_Fld(0x5, SHU1_R1_B1_DQ5_RK1_RX_ARDQ6_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ6, P_Fld(0x1e, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0x16, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_R_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ6_RK1_RX_ARDQM0_F_DLY_B1)
                | P_Fld(0x5, SHU1_R1_B1_DQ6_RK1_RX_ARDQM0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ7, P_Fld(0x1e, SHU1_R1_B1_DQ7_RK1_ARPI_DQM_B1)
                | P_Fld(0x1e, SHU1_R1_B1_DQ7_RK1_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B0_DQ0, P_Fld(0xd, SHU1_R2_B0_DQ0_RK2_TX_ARDQ7_DLY_B0)
                | P_Fld(0xd, SHU1_R2_B0_DQ0_RK2_TX_ARDQ6_DLY_B0)
                | P_Fld(0xd, SHU1_R2_B0_DQ0_RK2_TX_ARDQ5_DLY_B0)
                | P_Fld(0xd, SHU1_R2_B0_DQ0_RK2_TX_ARDQ4_DLY_B0)
                | P_Fld(0xd, SHU1_R2_B0_DQ0_RK2_TX_ARDQ3_DLY_B0)
                | P_Fld(0xd, SHU1_R2_B0_DQ0_RK2_TX_ARDQ2_DLY_B0)
                | P_Fld(0xd, SHU1_R2_B0_DQ0_RK2_TX_ARDQ1_DLY_B0)
                | P_Fld(0xd, SHU1_R2_B0_DQ0_RK2_TX_ARDQ0_DLY_B0));
    vIO32WriteFldAlign(DDRPHY_SHU1_R2_B0_DQ1, 0xd, SHU1_R2_B0_DQ1_RK2_TX_ARDQM0_DLY_B0);
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B0_DQ7, P_Fld(0x20, SHU1_R2_B0_DQ7_RK2_ARPI_DQM_B0)
                | P_Fld(0x20, SHU1_R2_B0_DQ7_RK2_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B1_DQ0, P_Fld(0xd, SHU1_R2_B1_DQ0_RK2_TX_ARDQ7_DLY_B1)
                | P_Fld(0xd, SHU1_R2_B1_DQ0_RK2_TX_ARDQ6_DLY_B1)
                | P_Fld(0xd, SHU1_R2_B1_DQ0_RK2_TX_ARDQ5_DLY_B1)
                | P_Fld(0xd, SHU1_R2_B1_DQ0_RK2_TX_ARDQ4_DLY_B1)
                | P_Fld(0xd, SHU1_R2_B1_DQ0_RK2_TX_ARDQ3_DLY_B1)
                | P_Fld(0xd, SHU1_R2_B1_DQ0_RK2_TX_ARDQ2_DLY_B1)
                | P_Fld(0xd, SHU1_R2_B1_DQ0_RK2_TX_ARDQ1_DLY_B1)
                | P_Fld(0xd, SHU1_R2_B1_DQ0_RK2_TX_ARDQ0_DLY_B1));
    vIO32WriteFldAlign(DDRPHY_SHU1_R2_B1_DQ1, 0xd, SHU1_R2_B1_DQ1_RK2_TX_ARDQM0_DLY_B1);
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B1_DQ7, P_Fld(0x20, SHU1_R2_B1_DQ7_RK2_ARPI_DQM_B1)
                | P_Fld(0x20, SHU1_R2_B1_DQ7_RK2_ARPI_DQ_B1));

}


static void DramcSetting_Olympus_LP4_ByteMode_DDR800(DRAMC_CTX_T *p)
{
    ////DRAMC0-SHU3
    vIO32WriteFldMulti(DRAMC_REG_PRE_TDQSCK2, P_Fld(0x1a, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO0)
                | P_Fld(0x10, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO1)
                | P_Fld(0x0, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO2)
                | P_Fld(0x26, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO3));
    vIO32WriteFldMulti(DRAMC_REG_PRE_TDQSCK3, P_Fld(0x13, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO4)
                | P_Fld(0x0, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO5)
                | P_Fld(0x40, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO6)
                | P_Fld(0x35, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO7));
    vIO32WriteFldMulti(DRAMC_REG_PRE_TDQSCK4, P_Fld(0x0, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO8)
                | P_Fld(0x0, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO9)
                | P_Fld(0x0, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO10)
                | P_Fld(0x0, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO11));
    vIO32WriteFldAlign(DRAMC_REG_RK0_PRE_TDQSCK1, 0xa, RK0_PRE_TDQSCK1_TDQSCK_UIFREQ2_B0R0);
    vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK2, P_Fld(0x0, RK0_PRE_TDQSCK2_TDQSCK_PIFREQ4_B0R0)
                | P_Fld(0x0, RK0_PRE_TDQSCK2_TDQSCK_UIFREQ4_B0R0)
                | P_Fld(0xb, RK0_PRE_TDQSCK2_TDQSCK_PIFREQ3_B0R0)
                | P_Fld(0xd, RK0_PRE_TDQSCK2_TDQSCK_UIFREQ3_B0R0));
    vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK3, P_Fld(0x0, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ4_P1_B0R0)
                | P_Fld(0x11, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ3_P1_B0R0)
                | P_Fld(0xe, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ2_P1_B0R0));
    vIO32WriteFldAlign(DRAMC_REG_RK0_PRE_TDQSCK4, 0xa, RK0_PRE_TDQSCK4_TDQSCK_UIFREQ2_B1R0);
    vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK5, P_Fld(0x0, RK0_PRE_TDQSCK5_TDQSCK_PIFREQ4_B1R0)
                | P_Fld(0x0, RK0_PRE_TDQSCK5_TDQSCK_UIFREQ4_B1R0)
                | P_Fld(0xb, RK0_PRE_TDQSCK5_TDQSCK_PIFREQ3_B1R0)
                | P_Fld(0xd, RK0_PRE_TDQSCK5_TDQSCK_UIFREQ3_B1R0));
    vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK6, P_Fld(0x0, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ4_P1_B1R0)
                | P_Fld(0x11, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ3_P1_B1R0)
                | P_Fld(0xe, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ2_P1_B1R0));
    vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK1, P_Fld(0x8, RK1_PRE_TDQSCK1_TDQSCK_PIFREQ2_B0R1)
                | P_Fld(0x10, RK1_PRE_TDQSCK1_TDQSCK_UIFREQ2_B0R1));
    vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK2, P_Fld(0x0, RK1_PRE_TDQSCK2_TDQSCK_PIFREQ4_B0R1)
                | P_Fld(0x0, RK1_PRE_TDQSCK2_TDQSCK_UIFREQ4_B0R1)
                | P_Fld(0xe, RK1_PRE_TDQSCK2_TDQSCK_PIFREQ3_B0R1)
                | P_Fld(0x10, RK1_PRE_TDQSCK2_TDQSCK_UIFREQ3_B0R1));
    vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK3, P_Fld(0x0, RK1_PRE_TDQSCK3_TDQSCK_UIFREQ4_P1_B0R1)
                | P_Fld(0x14, RK1_PRE_TDQSCK3_TDQSCK_UIFREQ3_P1_B0R1)
                | P_Fld(0x14, RK1_PRE_TDQSCK3_TDQSCK_UIFREQ2_P1_B0R1));
    vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK4, P_Fld(0x8, RK1_PRE_TDQSCK4_TDQSCK_PIFREQ2_B1R1)
                | P_Fld(0x10, RK1_PRE_TDQSCK4_TDQSCK_UIFREQ2_B1R1));
    vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK5, P_Fld(0x0, RK1_PRE_TDQSCK5_TDQSCK_PIFREQ4_B1R1)
                | P_Fld(0x0, RK1_PRE_TDQSCK5_TDQSCK_UIFREQ4_B1R1)
                | P_Fld(0xe, RK1_PRE_TDQSCK5_TDQSCK_PIFREQ3_B1R1)
                | P_Fld(0x10, RK1_PRE_TDQSCK5_TDQSCK_UIFREQ3_B1R1));
    vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK6, P_Fld(0x0, RK1_PRE_TDQSCK6_TDQSCK_UIFREQ4_P1_B1R1)
                | P_Fld(0x14, RK1_PRE_TDQSCK6_TDQSCK_UIFREQ3_P1_B1R1)
                | P_Fld(0x14, RK1_PRE_TDQSCK6_TDQSCK_UIFREQ2_P1_B1R1));
#if LEGACY_SET_ACTIMING
    Legacy_DramcSet_LP4_DDR800_ACTimingRegs(p);
#endif /* LEGACY_SET_ACTIMING */
    vIO32WriteFldAlign(DRAMC_REG_SHU_CONF2, 0x32, SHU_CONF2_FSPCHG_PRDCNT);
    vIO32WriteFldMulti(DRAMC_REG_SHU_RANKCTL, P_Fld(0x2, SHU_RANKCTL_RANKINCTL_PHY)
                | P_Fld(0x0, SHU_RANKCTL_RANKINCTL_ROOT1)
                | P_Fld(0x0, SHU_RANKCTL_RANKINCTL));
    vIO32WriteFldMulti(DRAMC_REG_SHU_CKECTRL, P_Fld(0x0, SHU_CKECTRL_TCKESRX));
                //| P_Fld(0x2, SHU_CKECTRL_CKEPRD));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ODTCTRL, P_Fld(0x0, SHU_ODTCTRL_RODTE)
                | P_Fld(0x0, SHU_ODTCTRL_RODTE2)
                | P_Fld(0x2, SHU_ODTCTRL_RODT)
#ifdef LOOPBACK_TEST
                | P_Fld(0x1, SHU_ODTCTRL_ROEN));
#else
                | P_Fld(0x0, SHU_ODTCTRL_ROEN));
#endif
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ7, 0x0, SHU1_B0_DQ7_R_DMRODTEN_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ7, 0x0, SHU1_B1_DQ7_R_DMRODTEN_B1);
    vIO32WriteFldMulti(DRAMC_REG_SHU1_DQSOSC_PRD, P_Fld(0x14, SHU1_DQSOSC_PRD_DQSOSCTHRD_DEC)
                | P_Fld(0x14, SHU1_DQSOSC_PRD_DQSOSCTHRD_INC)
                | P_Fld(0xf, SHU1_DQSOSC_PRD_DQSOSC_PRDCNT));
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS0, P_Fld(0x1, SHU_SELPH_DQS0_TXDLY_OEN_DQS3)
                | P_Fld(0x1, SHU_SELPH_DQS0_TXDLY_OEN_DQS2)
                | P_Fld(0x1, SHU_SELPH_DQS0_TXDLY_OEN_DQS1)
                | P_Fld(0x1, SHU_SELPH_DQS0_TXDLY_OEN_DQS0)
                | P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_DQS3)
                | P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_DQS2)
                | P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_DQS1)
                | P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_DQS0));
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS1, P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS3)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS2)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS1)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS0)
                | P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS3)
                | P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS2)
                | P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS1)
                | P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS0));
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldAlign(DRAMC_REG_SHU1_WODT, 0x1, SHU1_WODT_WPST2T);
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU1_WODT, P_Fld(0x1, SHU1_WODT_WPST2T)
                    | P_Fld(0x0, SHU1_WODT_DBIWR));
    }
    vIO32WriteFldAlign(DRAMC_REG_SHU_DQS2DQ_TX, 0x4, SHU_DQS2DQ_TX_OE2DQ_OFFSET);
    //vIO32WriteFldAlign(DRAMC_REG_SHU_HWSET_MR2, 0x12, SHU_HWSET_MR2_HWSET_MR2_OP);
    //vIO32WriteFldAlign(DRAMC_REG_SHU_HWSET_MR13, 0x8, SHU_HWSET_MR13_HWSET_MR13_OP);
    //vIO32WriteFldAlign(DRAMC_REG_SHU_HWSET_VRCG, 0x0, SHU_HWSET_VRCG_HWSET_VRCG_OP);
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQSIEN, P_Fld(0x0, SHURK0_DQSIEN_R0DQS1IEN)
                | P_Fld(0x0, SHURK0_DQSIEN_R0DQS0IEN));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_PI, P_Fld(0x1a, SHU1RK0_PI_RK0_ARPI_DQM_B1)
                | P_Fld(0x1a, SHU1RK0_PI_RK0_ARPI_DQM_B0)
                | P_Fld(0x1a, SHU1RK0_PI_RK0_ARPI_DQ_B1)
                | P_Fld(0x1a, SHU1RK0_PI_RK0_ARPI_DQ_B0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQSOSC, P_Fld(0x2d0, SHU1RK0_DQSOSC_DQSOSC_BASE_RK0_B1)
                | P_Fld(0x2d0, SHU1RK0_DQSOSC_DQSOSC_BASE_RK0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN0, P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN_P1)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN_P1)
                | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN1, P_Fld(0x5, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN_P1)
                | P_Fld(0x5, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN)
                | P_Fld(0x5, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN_P1)
                | P_Fld(0x5, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN));
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0, P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
                    | P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3)
                    | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2)
                    | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1)
                    | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0)
                    | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ3)
                    | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ2)
                    | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ1)
                    | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3)
                    | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2)
                    | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1)
                    | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0)
                    | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM3)
                    | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM2)
                    | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM1)
                    | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM0));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0, P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
                    | P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(0x0, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
                    | P_Fld(0x4, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
                    | P_Fld(0x0, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
                    | P_Fld(0x4, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3)
                    | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2)
                    | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1)
                    | P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0)
                    | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ3)
                    | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ2));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3)
                    | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2)
                    | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1)
                    | P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0)
                    | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM3)
                    | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM2));
    }
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ2, P_Fld(0x7, SHURK0_SELPH_DQ2_DLY_OEN_DQ3)
                | P_Fld(0x7, SHURK0_SELPH_DQ2_DLY_OEN_DQ2)
                | P_Fld(0x7, SHURK0_SELPH_DQ2_DLY_OEN_DQ1)
                | P_Fld(0x7, SHURK0_SELPH_DQ2_DLY_OEN_DQ0)
                | P_Fld(0x1, SHURK0_SELPH_DQ2_DLY_DQ3)
                | P_Fld(0x1, SHURK0_SELPH_DQ2_DLY_DQ2)
                | P_Fld(0x1, SHURK0_SELPH_DQ2_DLY_DQ1)
                | P_Fld(0x1, SHURK0_SELPH_DQ2_DLY_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ3, P_Fld(0x7, SHURK0_SELPH_DQ3_DLY_OEN_DQM3)
                | P_Fld(0x7, SHURK0_SELPH_DQ3_DLY_OEN_DQM2)
                | P_Fld(0x7, SHURK0_SELPH_DQ3_DLY_OEN_DQM1)
                | P_Fld(0x7, SHURK0_SELPH_DQ3_DLY_OEN_DQM0)
                | P_Fld(0x1, SHURK0_SELPH_DQ3_DLY_DQM3)
                | P_Fld(0x1, SHURK0_SELPH_DQ3_DLY_DQM2)
                | P_Fld(0x1, SHURK0_SELPH_DQ3_DLY_DQM1)
                | P_Fld(0x1, SHURK0_SELPH_DQ3_DLY_DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL1, P_Fld(0x23a, SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ1)
                | P_Fld(0x23a, SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL2, P_Fld(0x23a, SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ1)
                | P_Fld(0x23a, SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL3, P_Fld(0x1a, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1_B4TO0)
                | P_Fld(0x1a, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0_B4TO0)
                | P_Fld(0xf, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1)
                | P_Fld(0xf, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL4, P_Fld(0x1a, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1_B4TO0)
                | P_Fld(0x1a, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0_B4TO0)
                | P_Fld(0xf, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1)
                | P_Fld(0xf, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL5, P_Fld(0x23e, SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM1)
                | P_Fld(0x23e, SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM0));
    vIO32WriteFldAlign(DRAMC_REG_SHURK1_DQSCTL, 0x2, SHURK1_DQSCTL_R1DQSINCTL);
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQSIEN, P_Fld(0x0, SHURK1_DQSIEN_R1DQS1IEN)
                | P_Fld(0x0, SHURK1_DQSIEN_R1DQS0IEN));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_PI, P_Fld(0x1e, SHU1RK1_PI_RK1_ARPI_DQM_B1)
                | P_Fld(0x1e, SHU1RK1_PI_RK1_ARPI_DQM_B0)
                | P_Fld(0x1e, SHU1RK1_PI_RK1_ARPI_DQ_B1)
                | P_Fld(0x1e, SHU1RK1_PI_RK1_ARPI_DQ_B0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQSOSC, P_Fld(0x24e, SHU1RK1_DQSOSC_DQSOSC_BASE_RK1_B1)
                | P_Fld(0x24e, SHU1RK1_DQSOSC_DQSOSC_BASE_RK1));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN0, P_Fld(0x1, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN_P1)
                | P_Fld(0x1, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN)
                | P_Fld(0x1, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN_P1)
                | P_Fld(0x1, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN1, P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN_P1)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN_P1)
                | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN));
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0, P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
                    | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
                    | P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
                    | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1, P_Fld(0x0, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
                    | P_Fld(0x4, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED)
                    | P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1)
                    | P_Fld(0x5, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3)
                    | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2)
                    | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1)
                    | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0)
                    | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ3)
                    | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ2)
                    | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ1)
                    | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3)
                    | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2)
                    | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1)
                    | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0)
                    | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM3)
                    | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM2)
                    | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM1)
                    | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM0));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0, P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
                    | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
                    | P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
                    | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1, P_Fld(0x3, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
                    | P_Fld(0x7, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED)
                    | P_Fld(0x3, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1)
                    | P_Fld(0x7, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3)
                    | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2)
                    | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1)
                    | P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0)
                    | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ3)
                    | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ2));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3)
                    | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2)
                    | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1)
                    | P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0)
                    | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM3)
                    | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM2));
    }
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ2, P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ3)
                | P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ2)
                | P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ1)
                | P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ0)
                | P_Fld(0x1, SHURK1_SELPH_DQ2_DLY_R1DQ3)
                | P_Fld(0x1, SHURK1_SELPH_DQ2_DLY_R1DQ2)
                | P_Fld(0x1, SHURK1_SELPH_DQ2_DLY_R1DQ1)
                | P_Fld(0x1, SHURK1_SELPH_DQ2_DLY_R1DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ3, P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM3)
                | P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM2)
                | P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM1)
                | P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM0)
                | P_Fld(0x1, SHURK1_SELPH_DQ3_DLY_R1DQM3)
                | P_Fld(0x1, SHURK1_SELPH_DQ3_DLY_R1DQM2)
                | P_Fld(0x1, SHURK1_SELPH_DQ3_DLY_R1DQM1)
                | P_Fld(0x1, SHURK1_SELPH_DQ3_DLY_R1DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL1, P_Fld(0x23e, SHU1RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ1)
                | P_Fld(0x23e, SHU1RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL2, P_Fld(0x23e, SHU1RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ1)
                | P_Fld(0x23e, SHU1RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL3, P_Fld(0x1e, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ1_B4TO0)
                | P_Fld(0x1e, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ0_B4TO0)
                | P_Fld(0xf, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ1)
                | P_Fld(0xf, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL4, P_Fld(0x1e, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM1_B4TO0)
                | P_Fld(0x1e, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM0_B4TO0)
                | P_Fld(0xf, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM1)
                | P_Fld(0xf, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL5, P_Fld(0x23e, SHU1RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM1)
                | P_Fld(0x23e, SHU1RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHU_DQSG_RETRY, P_Fld(0x3, SHU_DQSG_RETRY_R_DQSIENLAT)
                | P_Fld(0x0, SHU_DQSG_RETRY_R_DDR1866_PLUS));

    ////DDRPHY0-SHU3
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ5, 0x4, SHU1_B0_DQ5_RG_RX_ARDQS_DVS_DLY_B0);
    //vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ6, P_Fld(0x1, SHU1_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0)
    //            | P_Fld(0x0, SHU1_B0_DQ6_RG_ARPI_MIDPI_EN_B0));
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7, P_Fld(0x1, SHU1_B0_DQ7_MIDPI_DIV4_ENABLE)
                    | P_Fld(0x0, SHU1_B0_DQ7_MIDPI_ENABLE));
    }
    else
    {
        vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7, P_Fld(0x0, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0)
                    | P_Fld(0x0, SHU1_B0_DQ7_R_DMDQMDBI_SHU_B0)
                    | P_Fld(0x1, SHU1_B0_DQ7_MIDPI_DIV4_ENABLE)
                    | P_Fld(0x0, SHU1_B0_DQ7_MIDPI_ENABLE));
    }
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5, 0x4, SHU1_B1_DQ5_RG_RX_ARDQS_DVS_DLY_B1);
    //vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ6, P_Fld(0x1, SHU1_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1)
    //            | P_Fld(0x0, SHU1_B1_DQ6_RG_ARPI_MIDPI_EN_B1));
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {}
    else
    {
        vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ7, P_Fld(0x0, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1)
                    | P_Fld(0x0, SHU1_B1_DQ7_R_DMDQMDBI_SHU_B1));
    }
    //DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    //vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD6, P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA)
    //            | P_Fld(0x1, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA));
    //vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD6+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA)
    //            | P_Fld(0x1, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA));
    //DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    //vIO32WriteFldAlign(DDRPHY_SHU1_PLL5, 0x3d00, SHU1_PLL5_RG_RPHYPLL_SDM_PCW);
    //vIO32WriteFldAlign(DDRPHY_SHU1_PLL7, 0x3d00, SHU1_PLL7_RG_RCLRPLL_SDM_PCW);
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ2, P_Fld(0xc, SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_F_DLY_B0)
                | P_Fld(0x4, SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_R_DLY_B0)
                | P_Fld(0xc, SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_F_DLY_B0)
                | P_Fld(0x4, SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ3, P_Fld(0xc, SHU1_R0_B0_DQ3_RK0_RX_ARDQ3_F_DLY_B0)
                | P_Fld(0x4, SHU1_R0_B0_DQ3_RK0_RX_ARDQ3_R_DLY_B0)
                | P_Fld(0xc, SHU1_R0_B0_DQ3_RK0_RX_ARDQ2_F_DLY_B0)
                | P_Fld(0x4, SHU1_R0_B0_DQ3_RK0_RX_ARDQ2_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ4, P_Fld(0xc, SHU1_R0_B0_DQ4_RK0_RX_ARDQ5_F_DLY_B0)
                | P_Fld(0x4, SHU1_R0_B0_DQ4_RK0_RX_ARDQ5_R_DLY_B0)
                | P_Fld(0xc, SHU1_R0_B0_DQ4_RK0_RX_ARDQ4_F_DLY_B0)
                | P_Fld(0x4, SHU1_R0_B0_DQ4_RK0_RX_ARDQ4_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ5, P_Fld(0xc, SHU1_R0_B0_DQ5_RK0_RX_ARDQ7_F_DLY_B0)
                | P_Fld(0x4, SHU1_R0_B0_DQ5_RK0_RX_ARDQ7_R_DLY_B0)
                | P_Fld(0xc, SHU1_R0_B0_DQ5_RK0_RX_ARDQ6_F_DLY_B0)
                | P_Fld(0x4, SHU1_R0_B0_DQ5_RK0_RX_ARDQ6_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ6, P_Fld(0x1d, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_F_DLY_B0)
                | P_Fld(0x15, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0)
                | P_Fld(0xc, SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_F_DLY_B0)
                | P_Fld(0x4, SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ7, P_Fld(0x1a, SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0)
                | P_Fld(0x1a, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ2, P_Fld(0xc, SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_F_DLY_B1)
                | P_Fld(0x4, SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_R_DLY_B1)
                | P_Fld(0xc, SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_F_DLY_B1)
                | P_Fld(0x4, SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ3, P_Fld(0xc, SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_F_DLY_B1)
                | P_Fld(0x4, SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_R_DLY_B1)
                | P_Fld(0xc, SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_F_DLY_B1)
                | P_Fld(0x4, SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ4, P_Fld(0xc, SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_F_DLY_B1)
                | P_Fld(0x4, SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_R_DLY_B1)
                | P_Fld(0xc, SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_F_DLY_B1)
                | P_Fld(0x4, SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ5, P_Fld(0xc, SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_F_DLY_B1)
                | P_Fld(0x4, SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_R_DLY_B1)
                | P_Fld(0xc, SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_F_DLY_B1)
                | P_Fld(0x4, SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ6, P_Fld(0x1d, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0x15, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1)
                | P_Fld(0xc, SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_F_DLY_B1)
                | P_Fld(0x4, SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ7, P_Fld(0x1a, SHU1_R0_B1_DQ7_RK0_ARPI_DQM_B1)
                | P_Fld(0x1a, SHU1_R0_B1_DQ7_RK0_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ0, P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ7_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ6_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ5_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ4_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ3_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ2_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ1_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ0_DLY_B0));
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_B0_DQ1, 0xd, SHU1_R1_B0_DQ1_RK1_TX_ARDQM0_DLY_B0);
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ2, P_Fld(0xd, SHU1_R1_B0_DQ2_RK1_RX_ARDQ1_F_DLY_B0)
                | P_Fld(0x5, SHU1_R1_B0_DQ2_RK1_RX_ARDQ1_R_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ2_RK1_RX_ARDQ0_F_DLY_B0)
                | P_Fld(0x5, SHU1_R1_B0_DQ2_RK1_RX_ARDQ0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ3, P_Fld(0xd, SHU1_R1_B0_DQ3_RK1_RX_ARDQ3_F_DLY_B0)
                | P_Fld(0x5, SHU1_R1_B0_DQ3_RK1_RX_ARDQ3_R_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ3_RK1_RX_ARDQ2_F_DLY_B0)
                | P_Fld(0x5, SHU1_R1_B0_DQ3_RK1_RX_ARDQ2_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ4, P_Fld(0xd, SHU1_R1_B0_DQ4_RK1_RX_ARDQ5_F_DLY_B0)
                | P_Fld(0x5, SHU1_R1_B0_DQ4_RK1_RX_ARDQ5_R_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ4_RK1_RX_ARDQ4_F_DLY_B0)
                | P_Fld(0x5, SHU1_R1_B0_DQ4_RK1_RX_ARDQ4_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ5, P_Fld(0xd, SHU1_R1_B0_DQ5_RK1_RX_ARDQ7_F_DLY_B0)
                | P_Fld(0x5, SHU1_R1_B0_DQ5_RK1_RX_ARDQ7_R_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ5_RK1_RX_ARDQ6_F_DLY_B0)
                | P_Fld(0x5, SHU1_R1_B0_DQ5_RK1_RX_ARDQ6_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ6, P_Fld(0x1e, SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_F_DLY_B0)
                | P_Fld(0x16, SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_R_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ6_RK1_RX_ARDQM0_F_DLY_B0)
                | P_Fld(0x5, SHU1_R1_B0_DQ6_RK1_RX_ARDQM0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ7, P_Fld(0x1e, SHU1_R1_B0_DQ7_RK1_ARPI_DQM_B0)
                | P_Fld(0x1e, SHU1_R1_B0_DQ7_RK1_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ0, P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ7_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ6_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ5_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ4_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ3_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ2_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ1_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ0_DLY_B1));
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_B1_DQ1, 0xd, SHU1_R1_B1_DQ1_RK1_TX_ARDQM0_DLY_B1);
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ2, P_Fld(0xd, SHU1_R1_B1_DQ2_RK1_RX_ARDQ1_F_DLY_B1)
                | P_Fld(0x5, SHU1_R1_B1_DQ2_RK1_RX_ARDQ1_R_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ2_RK1_RX_ARDQ0_F_DLY_B1)
                | P_Fld(0x5, SHU1_R1_B1_DQ2_RK1_RX_ARDQ0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ3, P_Fld(0xd, SHU1_R1_B1_DQ3_RK1_RX_ARDQ3_F_DLY_B1)
                | P_Fld(0x5, SHU1_R1_B1_DQ3_RK1_RX_ARDQ3_R_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ3_RK1_RX_ARDQ2_F_DLY_B1)
                | P_Fld(0x5, SHU1_R1_B1_DQ3_RK1_RX_ARDQ2_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ4, P_Fld(0xd, SHU1_R1_B1_DQ4_RK1_RX_ARDQ5_F_DLY_B1)
                | P_Fld(0x5, SHU1_R1_B1_DQ4_RK1_RX_ARDQ5_R_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ4_RK1_RX_ARDQ4_F_DLY_B1)
                | P_Fld(0x5, SHU1_R1_B1_DQ4_RK1_RX_ARDQ4_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ5, P_Fld(0xd, SHU1_R1_B1_DQ5_RK1_RX_ARDQ7_F_DLY_B1)
                | P_Fld(0x5, SHU1_R1_B1_DQ5_RK1_RX_ARDQ7_R_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ5_RK1_RX_ARDQ6_F_DLY_B1)
                | P_Fld(0x5, SHU1_R1_B1_DQ5_RK1_RX_ARDQ6_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ6, P_Fld(0x1e, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0x16, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_R_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ6_RK1_RX_ARDQM0_F_DLY_B1)
                | P_Fld(0x5, SHU1_R1_B1_DQ6_RK1_RX_ARDQM0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ7, P_Fld(0x1e, SHU1_R1_B1_DQ7_RK1_ARPI_DQM_B1)
                | P_Fld(0x1e, SHU1_R1_B1_DQ7_RK1_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B0_DQ0, P_Fld(0xd, SHU1_R2_B0_DQ0_RK2_TX_ARDQ7_DLY_B0)
                | P_Fld(0xd, SHU1_R2_B0_DQ0_RK2_TX_ARDQ6_DLY_B0)
                | P_Fld(0xd, SHU1_R2_B0_DQ0_RK2_TX_ARDQ5_DLY_B0)
                | P_Fld(0xd, SHU1_R2_B0_DQ0_RK2_TX_ARDQ4_DLY_B0)
                | P_Fld(0xd, SHU1_R2_B0_DQ0_RK2_TX_ARDQ3_DLY_B0)
                | P_Fld(0xd, SHU1_R2_B0_DQ0_RK2_TX_ARDQ2_DLY_B0)
                | P_Fld(0xd, SHU1_R2_B0_DQ0_RK2_TX_ARDQ1_DLY_B0)
                | P_Fld(0xd, SHU1_R2_B0_DQ0_RK2_TX_ARDQ0_DLY_B0));
    vIO32WriteFldAlign(DDRPHY_SHU1_R2_B0_DQ1, 0xd, SHU1_R2_B0_DQ1_RK2_TX_ARDQM0_DLY_B0);
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B0_DQ7, P_Fld(0x20, SHU1_R2_B0_DQ7_RK2_ARPI_DQM_B0)
                | P_Fld(0x20, SHU1_R2_B0_DQ7_RK2_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B1_DQ0, P_Fld(0xd, SHU1_R2_B1_DQ0_RK2_TX_ARDQ7_DLY_B1)
                | P_Fld(0xd, SHU1_R2_B1_DQ0_RK2_TX_ARDQ6_DLY_B1)
                | P_Fld(0xd, SHU1_R2_B1_DQ0_RK2_TX_ARDQ5_DLY_B1)
                | P_Fld(0xd, SHU1_R2_B1_DQ0_RK2_TX_ARDQ4_DLY_B1)
                | P_Fld(0xd, SHU1_R2_B1_DQ0_RK2_TX_ARDQ3_DLY_B1)
                | P_Fld(0xd, SHU1_R2_B1_DQ0_RK2_TX_ARDQ2_DLY_B1)
                | P_Fld(0xd, SHU1_R2_B1_DQ0_RK2_TX_ARDQ1_DLY_B1)
                | P_Fld(0xd, SHU1_R2_B1_DQ0_RK2_TX_ARDQ0_DLY_B1));
    vIO32WriteFldAlign(DDRPHY_SHU1_R2_B1_DQ1, 0xd, SHU1_R2_B1_DQ1_RK2_TX_ARDQM0_DLY_B1);
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B1_DQ7, P_Fld(0x20, SHU1_R2_B1_DQ7_RK2_ARPI_DQM_B1)
                | P_Fld(0x20, SHU1_R2_B1_DQ7_RK2_ARPI_DQ_B1));

}


/*
CANNOT use DRAMC_WBR :
DDRPHY_CA_DLL_ARPI0 -> DDRPHY_SHU1_CA_DLL1
DDRPHY_CA_DLL_ARPI3
DDRPHY_CA_DLL_ARPI5 -> DDRPHY_SHU1_CA_DLL0
DDRPHY_SHU1_CA_CMD6
*/
static void DramcSetting_Olympus_LP4_ByteMode(DRAMC_CTX_T *p)
{
    U8 u1CAP_SEL;
    U8 u1MIDPICAP_SEL;
    //U16 u2SDM_PCW = 0; // SDM_PCW are set in DDRPhyPLLSetting()

    AutoRefreshCKEOff(p);

    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
                                                                                                                   //before switch clock from 26M to PHY, need to init PHY clock first
    vIO32WriteFldMulti(DDRPHY_CKMUX_SEL, P_Fld(0x1, CKMUX_SEL_R_PHYCTRLMUX) | P_Fld(0x1, CKMUX_SEL_R_PHYCTRLDCM)); //move CKMUX_SEL_R_PHYCTRLMUX to here (it was originally between MISC_CG_CTRL0_CLK_MEM_SEL and MISC_CTRL0_R_DMRDSEL_DIV2_OPT)

    //chg_mem_en = 1
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 0x1, MISC_CG_CTRL0_W_CHG_MEM);
    //26M
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 0x0, MISC_CG_CTRL0_CLK_MEM_SEL); 

    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ3, 0x0, SHU1_B0_DQ3_RG_ARDQ_REV_B0); 
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ3, 0x0, SHU1_B1_DQ3_RG_ARDQ_REV_B1); 

    vIO32WriteFldAlign(DDRPHY_MISC_CTRL0, 0x0, MISC_CTRL0_R_DMRDSEL_DIV2_OPT);

    //                   0 ===LP4_3200_intial_setting_shu1 begin===
    //Francis : pin mux issue, need to set CHD
    // TODO: ARDMSUS_10 already set to 0 in SwimpedanceCal(), may be removed here?
    vIO32WriteFldMulti(DDRPHY_MISC_SPM_CTRL1, P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10) | P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_B0)
                                           | P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_B1) | P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_CA));
    
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL2, 0x0, MISC_SPM_CTRL2_PHY_SPM_CTL2);
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL0, 0x0, MISC_SPM_CTRL0_PHY_SPM_CTL0);
    
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL2, 0x6003bf, MISC_CG_CTRL2_RG_MEM_DCM_CTL);
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL4, 0x333f3f00, MISC_CG_CTRL4_R_PHY_MCK_CG_CTRL);
    vIO32WriteFldMulti(DDRPHY_SHU1_PLL1, P_Fld(0x1, SHU1_PLL1_R_SHU_AUTO_PLL_MUX)
                | P_Fld(0x7, SHU1_PLL1_SHU1_PLL1_RFU));
    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7, P_Fld(0x1, SHU1_B0_DQ7_MIDPI_ENABLE)
                | P_Fld(0x0, SHU1_B0_DQ7_MIDPI_DIV4_ENABLE)
                | P_Fld(0x1, SHU1_B0_DQ7_R_DMRANKRXDVS_B0));
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ7, 0x1, SHU1_B1_DQ7_R_DMRANKRXDVS_B1);
    vIO32WriteFldAlign(DDRPHY_SHU1_PLL4, 0xfe, SHU1_PLL4_RG_RPHYPLL_RESERVED);
    vIO32WriteFldMulti(DDRPHY_SHU1_PLL4, P_Fld(0x1, SHU1_PLL4_RG_RPHYPLL_IBIAS) | P_Fld(0x1, SHU1_PLL4_RG_RPHYPLL_ICHP) | P_Fld(0x2, SHU1_PLL4_RG_RPHYPLL_FS));
    vIO32WriteFldAlign(DDRPHY_SHU1_PLL6, 0xfe, SHU1_PLL6_RG_RCLRPLL_RESERVED);
    vIO32WriteFldMulti(DDRPHY_SHU1_PLL6, P_Fld(0x1, SHU1_PLL6_RG_RCLRPLL_IBIAS) | P_Fld(0x1, SHU1_PLL6_RG_RCLRPLL_ICHP) | P_Fld(0x2, SHU1_PLL6_RG_RCLRPLL_FS));
    vIO32WriteFldAlign(DDRPHY_SHU1_PLL14, 0x0, SHU1_PLL14_RG_RPHYPLL_SDM_SSC_PH_INIT);
    vIO32WriteFldAlign(DDRPHY_SHU1_PLL20, 0x0, SHU1_PLL20_RG_RCLRPLL_SDM_SSC_PH_INIT);
    vIO32WriteFldMulti(DDRPHY_CA_CMD2, P_Fld(0x0, CA_CMD2_RG_TX_ARCMD_OE_DIS)
                | P_Fld(0x0, CA_CMD2_RG_TX_ARCMD_ODTEN_DIS)
                | P_Fld(0x0, CA_CMD2_RG_TX_ARCLK_OE_DIS)
                | P_Fld(0x0, CA_CMD2_RG_TX_ARCLK_ODTEN_DIS));
    vIO32WriteFldMulti(DDRPHY_B0_DQ2, P_Fld(0x0, B0_DQ2_RG_TX_ARDQ_OE_DIS_B0)
                | P_Fld(0x0, B0_DQ2_RG_TX_ARDQ_ODTEN_DIS_B0)
                | P_Fld(0x0, B0_DQ2_RG_TX_ARDQS_OE_DIS_B0)
                | P_Fld(0x0, B0_DQ2_RG_TX_ARDQS_ODTEN_DIS_B0));
    vIO32WriteFldMulti(DDRPHY_B1_DQ2, P_Fld(0x0, B1_DQ2_RG_TX_ARDQ_OE_DIS_B1)
                | P_Fld(0x0, B1_DQ2_RG_TX_ARDQ_ODTEN_DIS_B1)
                | P_Fld(0x0, B1_DQ2_RG_TX_ARDQS_OE_DIS_B1)
                | P_Fld(0x0, B1_DQ2_RG_TX_ARDQS_ODTEN_DIS_B1));
                
    vIO32WriteFldAlign(DDRPHY_B0_DQ9, 0x8e, B0_DQ9_R_IN_GATE_EN_LOW_OPT_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ9, 0x8e, B1_DQ9_R_IN_GATE_EN_LOW_OPT_B1);
    vIO32WriteFldAlign(DDRPHY_CA_CMD10, 0x8e, CA_CMD10_R_IN_GATE_EN_LOW_OPT_CA);        
    vIO32WriteFldAlign(DDRPHY_B0_DQ9, 0x1, B0_DQ9_R_DMRXDVS_RDSEL_LAT_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ9, 0x1, B1_DQ9_R_DMRXDVS_RDSEL_LAT_B1);
    vIO32WriteFldAlign(DDRPHY_CA_CMD10, 0x0, CA_CMD10_R_DMRXDVS_RDSEL_LAT_CA);

    vIO32WriteFldAlign(DDRPHY_B0_RXDVS0, 0x1, B0_RXDVS0_R_RX_DLY_TRACK_CG_EN_B0);
    vIO32WriteFldAlign(DDRPHY_B1_RXDVS0, 0x1, B1_RXDVS0_R_RX_DLY_TRACK_CG_EN_B1);
    vIO32WriteFldAlign(DDRPHY_B0_RXDVS0, 0x1, B0_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_B0);
    vIO32WriteFldAlign(DDRPHY_B1_RXDVS0, 0x1, B1_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_B1);
    vIO32WriteFldAlign(DDRPHY_R0_B0_RXDVS2, 0x1, R0_B0_RXDVS2_R_RK0_DVS_FDLY_MODE_B0);
    vIO32WriteFldAlign(DDRPHY_R1_B0_RXDVS2, 0x1, R1_B0_RXDVS2_R_RK1_DVS_FDLY_MODE_B0);
    vIO32WriteFldAlign(DDRPHY_R0_B1_RXDVS2, 0x1, R0_B1_RXDVS2_R_RK0_DVS_FDLY_MODE_B1);
    vIO32WriteFldAlign(DDRPHY_R1_B1_RXDVS2, 0x1, R1_B1_RXDVS2_R_RK1_DVS_FDLY_MODE_B1);
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ5, 0x3, SHU1_B0_DQ5_RG_RX_ARDQS_DVS_DLY_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5, 0x3, SHU1_B1_DQ5_RG_RX_ARDQS_DVS_DLY_B1);
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ2, P_Fld(0x12, SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_R_DLY_B0)
                | P_Fld(0x12, SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ3, P_Fld(0x12, SHU1_R0_B0_DQ3_RK0_RX_ARDQ3_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ3_RK0_RX_ARDQ3_R_DLY_B0)
                | P_Fld(0x12, SHU1_R0_B0_DQ3_RK0_RX_ARDQ2_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ3_RK0_RX_ARDQ2_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ4, P_Fld(0x12, SHU1_R0_B0_DQ4_RK0_RX_ARDQ5_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ4_RK0_RX_ARDQ5_R_DLY_B0)
                | P_Fld(0x12, SHU1_R0_B0_DQ4_RK0_RX_ARDQ4_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ4_RK0_RX_ARDQ4_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ5, P_Fld(0x12, SHU1_R0_B0_DQ5_RK0_RX_ARDQ7_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ5_RK0_RX_ARDQ7_R_DLY_B0)
                | P_Fld(0x12, SHU1_R0_B0_DQ5_RK0_RX_ARDQ6_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ5_RK0_RX_ARDQ6_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ6, P_Fld(0x14, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_F_DLY_B0)
                | P_Fld(0xc, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0)
                | P_Fld(0x12, SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_F_DLY_B0)
                | P_Fld(0xa, SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ2, P_Fld(0x13, SHU1_R1_B0_DQ2_RK1_RX_ARDQ1_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ2_RK1_RX_ARDQ1_R_DLY_B0)
                | P_Fld(0x13, SHU1_R1_B0_DQ2_RK1_RX_ARDQ0_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ2_RK1_RX_ARDQ0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ3, P_Fld(0x13, SHU1_R1_B0_DQ3_RK1_RX_ARDQ3_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ3_RK1_RX_ARDQ3_R_DLY_B0)
                | P_Fld(0x13, SHU1_R1_B0_DQ3_RK1_RX_ARDQ2_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ3_RK1_RX_ARDQ2_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ4, P_Fld(0x13, SHU1_R1_B0_DQ4_RK1_RX_ARDQ5_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ4_RK1_RX_ARDQ5_R_DLY_B0)
                | P_Fld(0x13, SHU1_R1_B0_DQ4_RK1_RX_ARDQ4_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ4_RK1_RX_ARDQ4_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ5, P_Fld(0x13, SHU1_R1_B0_DQ5_RK1_RX_ARDQ7_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ5_RK1_RX_ARDQ7_R_DLY_B0)
                | P_Fld(0x13, SHU1_R1_B0_DQ5_RK1_RX_ARDQ6_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ5_RK1_RX_ARDQ6_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ6, P_Fld(0x15, SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_F_DLY_B0)
                | P_Fld(0xd, SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_R_DLY_B0)
                | P_Fld(0x13, SHU1_R1_B0_DQ6_RK1_RX_ARDQM0_F_DLY_B0)
                | P_Fld(0xb, SHU1_R1_B0_DQ6_RK1_RX_ARDQM0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ2, P_Fld(0x12, SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ3, P_Fld(0x12, SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ4, P_Fld(0x12, SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ5, P_Fld(0x12, SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ6, P_Fld(0x14, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0xc, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1)
                | P_Fld(0x12, SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_F_DLY_B1)
                | P_Fld(0xa, SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ2, P_Fld(0x13, SHU1_R1_B1_DQ2_RK1_RX_ARDQ1_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ2_RK1_RX_ARDQ1_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ2_RK1_RX_ARDQ0_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ2_RK1_RX_ARDQ0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ3, P_Fld(0x13, SHU1_R1_B1_DQ3_RK1_RX_ARDQ3_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ3_RK1_RX_ARDQ3_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ3_RK1_RX_ARDQ2_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ3_RK1_RX_ARDQ2_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ4, P_Fld(0x13, SHU1_R1_B1_DQ4_RK1_RX_ARDQ5_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ4_RK1_RX_ARDQ5_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ4_RK1_RX_ARDQ4_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ4_RK1_RX_ARDQ4_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ5, P_Fld(0x13, SHU1_R1_B1_DQ5_RK1_RX_ARDQ7_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ5_RK1_RX_ARDQ7_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ5_RK1_RX_ARDQ6_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ5_RK1_RX_ARDQ6_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ6, P_Fld(0x15, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_F_DLY_B1)
                | P_Fld(0xd, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_R_DLY_B1)
                | P_Fld(0x13, SHU1_R1_B1_DQ6_RK1_RX_ARDQM0_F_DLY_B1)
                | P_Fld(0xb, SHU1_R1_B1_DQ6_RK1_RX_ARDQM0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_R0_B0_RXDVS1, P_Fld(0x2, R0_B0_RXDVS1_R_RK0_B0_DVS_TH_LEAD)
                | P_Fld(0x2, R0_B0_RXDVS1_R_RK0_B0_DVS_TH_LAG));
    vIO32WriteFldMulti(DDRPHY_R1_B0_RXDVS1, P_Fld(0x2, R1_B0_RXDVS1_R_RK1_B0_DVS_TH_LEAD)
                | P_Fld(0x2, R1_B0_RXDVS1_R_RK1_B0_DVS_TH_LAG));
    vIO32WriteFldMulti(DDRPHY_R0_B1_RXDVS1, P_Fld(0x2, R0_B1_RXDVS1_R_RK0_B1_DVS_TH_LEAD)
                | P_Fld(0x2, R0_B1_RXDVS1_R_RK0_B1_DVS_TH_LAG));
    vIO32WriteFldMulti(DDRPHY_R1_B1_RXDVS1, P_Fld(0x2, R1_B1_RXDVS1_R_RK1_B1_DVS_TH_LEAD)
                | P_Fld(0x2, R1_B1_RXDVS1_R_RK1_B1_DVS_TH_LAG));

    vIO32WriteFldMulti(DDRPHY_R0_B0_RXDVS2, P_Fld(0x2, R0_B0_RXDVS2_R_RK0_DVS_MODE_B0)
                | P_Fld(0x1, R0_B0_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B0)
                | P_Fld(0x1, R0_B0_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B0));
    vIO32WriteFldMulti(DDRPHY_R1_B0_RXDVS2, P_Fld(0x2, R1_B0_RXDVS2_R_RK1_DVS_MODE_B0)
                | P_Fld(0x1, R1_B0_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_B0)
                | P_Fld(0x1, R1_B0_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_B0));
    vIO32WriteFldMulti(DDRPHY_R0_B1_RXDVS2, P_Fld(0x2, R0_B1_RXDVS2_R_RK0_DVS_MODE_B1)
                | P_Fld(0x1, R0_B1_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B1)
                | P_Fld(0x1, R0_B1_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B1));
    vIO32WriteFldMulti(DDRPHY_R1_B1_RXDVS2, P_Fld(0x2, R1_B1_RXDVS2_R_RK1_DVS_MODE_B1)
                | P_Fld(0x1, R1_B1_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_B1)
                | P_Fld(0x1, R1_B1_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_B1));

    vIO32WriteFldAlign(DDRPHY_B0_RXDVS0, 0x0, B0_RXDVS0_R_RX_DLY_TRACK_CG_EN_B0);
    vIO32WriteFldAlign(DDRPHY_B1_RXDVS0, 0x0, B1_RXDVS0_R_RX_DLY_TRACK_CG_EN_B1);
    vIO32WriteFldAlign(DDRPHY_B0_DQ9, 0x1, B0_DQ9_RG_RX_ARDQ_STBEN_RESETB_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ9, 0x1, B1_DQ9_RG_RX_ARDQ_STBEN_RESETB_B1);
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ0, P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ7_DLY_B0)
                | P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ6_DLY_B0)
                | P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ5_DLY_B0)
                | P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ4_DLY_B0)
                | P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ3_DLY_B0)
                | P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ2_DLY_B0)
                | P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ1_DLY_B0)
                | P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ0_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B0_DQ0, P_Fld(0xa, SHU1_R2_B0_DQ0_RK2_TX_ARDQ7_DLY_B0)
                | P_Fld(0xa, SHU1_R2_B0_DQ0_RK2_TX_ARDQ6_DLY_B0)
                | P_Fld(0xa, SHU1_R2_B0_DQ0_RK2_TX_ARDQ5_DLY_B0)
                | P_Fld(0xa, SHU1_R2_B0_DQ0_RK2_TX_ARDQ4_DLY_B0)
                | P_Fld(0xa, SHU1_R2_B0_DQ0_RK2_TX_ARDQ3_DLY_B0)
                | P_Fld(0xa, SHU1_R2_B0_DQ0_RK2_TX_ARDQ2_DLY_B0)
                | P_Fld(0xa, SHU1_R2_B0_DQ0_RK2_TX_ARDQ1_DLY_B0)
                | P_Fld(0xa, SHU1_R2_B0_DQ0_RK2_TX_ARDQ0_DLY_B0));
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_B0_DQ1, 0xa, SHU1_R1_B0_DQ1_RK1_TX_ARDQM0_DLY_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_R2_B0_DQ1, 0xa, SHU1_R2_B0_DQ1_RK2_TX_ARDQM0_DLY_B0);
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ0, P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ7_DLY_B1)
                | P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ6_DLY_B1)
                | P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ5_DLY_B1)
                | P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ4_DLY_B1)
                | P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ3_DLY_B1)
                | P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ2_DLY_B1)
                | P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ1_DLY_B1)
                | P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ0_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B1_DQ0, P_Fld(0xa, SHU1_R2_B1_DQ0_RK2_TX_ARDQ7_DLY_B1)
                | P_Fld(0xa, SHU1_R2_B1_DQ0_RK2_TX_ARDQ6_DLY_B1)
                | P_Fld(0xa, SHU1_R2_B1_DQ0_RK2_TX_ARDQ5_DLY_B1)
                | P_Fld(0xa, SHU1_R2_B1_DQ0_RK2_TX_ARDQ4_DLY_B1)
                | P_Fld(0xa, SHU1_R2_B1_DQ0_RK2_TX_ARDQ3_DLY_B1)
                | P_Fld(0xa, SHU1_R2_B1_DQ0_RK2_TX_ARDQ2_DLY_B1)
                | P_Fld(0xa, SHU1_R2_B1_DQ0_RK2_TX_ARDQ1_DLY_B1)
                | P_Fld(0xa, SHU1_R2_B1_DQ0_RK2_TX_ARDQ0_DLY_B1));
    vIO32WriteFldAlign(DDRPHY_SHU1_R1_B1_DQ1, 0xa, SHU1_R1_B1_DQ1_RK1_TX_ARDQM0_DLY_B1);
    vIO32WriteFldAlign(DDRPHY_SHU1_R2_B1_DQ1, 0xa, SHU1_R2_B1_DQ1_RK2_TX_ARDQM0_DLY_B1);
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ7, P_Fld(0x1f, SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0)
                | P_Fld(0x1f, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ7, P_Fld(0x1f, SHU1_R0_B1_DQ7_RK0_ARPI_DQM_B1)
                | P_Fld(0x1f, SHU1_R0_B1_DQ7_RK0_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ7, P_Fld(0x1f, SHU1_R1_B0_DQ7_RK1_ARPI_DQM_B0)
                | P_Fld(0x1f, SHU1_R1_B0_DQ7_RK1_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ7, P_Fld(0x1f, SHU1_R1_B1_DQ7_RK1_ARPI_DQM_B1)
                | P_Fld(0x1f, SHU1_R1_B1_DQ7_RK1_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B0_DQ7, P_Fld(0x1f, SHU1_R2_B0_DQ7_RK2_ARPI_DQM_B0)
                | P_Fld(0x1f, SHU1_R2_B0_DQ7_RK2_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R2_B1_DQ7, P_Fld(0x1f, SHU1_R2_B1_DQ7_RK2_ARPI_DQM_B1)
                | P_Fld(0x1f, SHU1_R2_B1_DQ7_RK2_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_B0_DQ4, P_Fld(0x10, B0_DQ4_RG_RX_ARDQS_EYE_R_DLY_B0)
                | P_Fld(0x10, B0_DQ4_RG_RX_ARDQS_EYE_F_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_B0_DQ5, P_Fld(0x0, B0_DQ5_RG_RX_ARDQ_EYE_EN_B0)
                | P_Fld(0x1, B0_DQ5_RG_RX_ARDQ_EYE_SEL_B0)
                | P_Fld(0x1, B0_DQ5_RG_RX_ARDQ_VREF_EN_B0)
                | P_Fld(0xe, B0_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B0)
                | P_Fld(0x10, B0_DQ5_B0_DQ5_RFU));
    vIO32WriteFldMulti(DDRPHY_B0_DQ6, P_Fld(0x1, B0_DQ6_RG_RX_ARDQ_EYE_DLY_DQS_BYPASS_B0)
                | P_Fld(0x0, B0_DQ6_RG_TX_ARDQ_DDR3_SEL_B0)
                | P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_DDR3_SEL_B0)
                | P_Fld(0x1, B0_DQ6_RG_TX_ARDQ_DDR4_SEL_B0)
                | P_Fld(0x1, B0_DQ6_RG_RX_ARDQ_DDR4_SEL_B0)
                | P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B0)
                | P_Fld(0x1, B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0)
                | P_Fld(0x1, B0_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B0)
                | P_Fld(0x0, B0_DQ6_RG_TX_ARDQ_SER_MODE_B0));
    vIO32WriteFldMulti(DDRPHY_B0_DQ5, P_Fld(0x1, B0_DQ5_RG_RX_ARDQ_EYE_STBEN_RESETB_B0)
                | P_Fld(0x0, B0_DQ5_B0_DQ5_RFU));
    vIO32WriteFldMulti(DDRPHY_B1_DQ4, P_Fld(0x10, B1_DQ4_RG_RX_ARDQS_EYE_R_DLY_B1)
                | P_Fld(0x10, B1_DQ4_RG_RX_ARDQS_EYE_F_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_B1_DQ5, P_Fld(0x0, B1_DQ5_RG_RX_ARDQ_EYE_EN_B1)
                | P_Fld(0x1, B1_DQ5_RG_RX_ARDQ_EYE_SEL_B1)
                | P_Fld(0x1, B1_DQ5_RG_RX_ARDQ_VREF_EN_B1)
                | P_Fld(0xe, B1_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B1)
                | P_Fld(0x10, B1_DQ5_B1_DQ5_RFU));
    vIO32WriteFldMulti(DDRPHY_B1_DQ6, P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_EYE_DLY_DQS_BYPASS_B1)
                | P_Fld(0x0, B1_DQ6_RG_TX_ARDQ_DDR3_SEL_B1)
                | P_Fld(0x0, B1_DQ6_RG_RX_ARDQ_DDR3_SEL_B1)
                | P_Fld(0x1, B1_DQ6_RG_TX_ARDQ_DDR4_SEL_B1)
                | P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_DDR4_SEL_B1)
                | P_Fld(0x0, B1_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B1)
                | P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1)
                | P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B1)
                | P_Fld(0x0, B1_DQ6_RG_TX_ARDQ_SER_MODE_B1));
    vIO32WriteFldMulti(DDRPHY_B1_DQ5, P_Fld(0x1, B1_DQ5_RG_RX_ARDQ_EYE_STBEN_RESETB_B1)
                | P_Fld(0x0, B1_DQ5_B1_DQ5_RFU));
    vIO32WriteFldMulti(DDRPHY_CA_CMD3, P_Fld(0x1, CA_CMD3_RG_RX_ARCMD_IN_BUFF_EN)
                | P_Fld(0x1, CA_CMD3_RG_ARCMD_RESETB)
                | P_Fld(0x1, CA_CMD3_RG_TX_ARCMD_EN));
    vIO32WriteFldMulti(DDRPHY_CA_CMD6, P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_DDR4_SEL)
                | P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_BIAS_VREF_SEL)
                | P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_RES_BIAS_EN));
    vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD1, P_Fld(0x1, SHU1_CA_CMD1_RG_TX_ARCMD_DRVN)
                | P_Fld(0x1, SHU1_CA_CMD1_RG_TX_ARCMD_DRVP));
    vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD2, P_Fld(0x1, SHU1_CA_CMD2_RG_TX_ARCLK_DRVN)
                | P_Fld(0x1, SHU1_CA_CMD2_RG_TX_ARCLK_DRVP));
    //vIO32WriteFldMulti(DDRPHY_SHU2_CA_CMD1, P_Fld(0x1, SHU2_CA_CMD1_RG_TX_ARCMD_DRVN)
    //            | P_Fld(0x1, SHU2_CA_CMD1_RG_TX_ARCMD_DRVP));
    //vIO32WriteFldMulti(DDRPHY_SHU2_CA_CMD2, P_Fld(0x1, SHU2_CA_CMD2_RG_TX_ARCLK_DRVN)
    //            | P_Fld(0x1, SHU2_CA_CMD2_RG_TX_ARCLK_DRVP));
    vIO32WriteFldAlign(DDRPHY_PLL3, 0x0, PLL3_RG_RPHYPLL_TSTOP_EN);

    vIO32WriteFldAlign(DDRPHY_SHU1_PLL5, 0x0, SHU1_PLL5_RG_RPHYPLL_SDM_FRA_EN);
    vIO32WriteFldAlign(DDRPHY_SHU1_PLL7, 0x0, SHU1_PLL5_RG_RPHYPLL_SDM_FRA_EN);
#if 0 // SDM_PCW are set in DDRPhyPLLSetting()
    if(p->frequency==1600 || p->frequency==800)
    {   // 3200/26=123(0x7b)
        u2SDM_PCW = 0x7b00;
    }
    else if(p->frequency==1333)
    {
        u2SDM_PCW = 0x6600;
    }
    else if (p->frequency == 1200)
    {
        u2SDM_PCW = 0x5c00;
    }
    else if (p->frequency == 1140)
    {
        u2SDM_PCW = 0x5700;
    }
    vIO32WriteFldAlign(DDRPHY_SHU1_PLL5, u2SDM_PCW, SHU1_PLL5_RG_RPHYPLL_SDM_PCW);
    vIO32WriteFldAlign(DDRPHY_SHU1_PLL7, u2SDM_PCW, SHU1_PLL7_RG_RCLRPLL_SDM_PCW);
#endif


#if 0//ORIGINAL_PLL_INIT
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI0, 0x1, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI0+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    vIO32WriteFldAlign(DDRPHY_B0_DLL_ARPI0, 0x1, B0_DLL_ARPI0_RG_ARPI_RESETB_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DLL_ARPI0, 0x1, B1_DLL_ARPI0_RG_ARPI_RESETB_B1);
#endif
    vIO32WriteFldAlign(DDRPHY_B0_DQ3, 0x1, B0_DQ3_RG_ARDQ_RESETB_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ3, 0x1, B1_DQ3_RG_ARDQ_RESETB_B1);

    mcDELAY_US(1);

    //Ref clock should be 20M~30M, if MPLL=52M, Pre-divider should be set to 1
    vIO32WriteFldMulti(DDRPHY_SHU1_PLL8, P_Fld(0x0, SHU1_PLL8_RG_RPHYPLL_POSDIV) | P_Fld(0x1, SHU1_PLL8_RG_RPHYPLL_PREDIV));
    //vIO32WriteFldAlign(DDRPHY_SHU2_PLL8, 0x0, SHU2_PLL8_RG_RPHYPLL_POSDIV);
    //vIO32WriteFldAlign(DDRPHY_SHU3_PLL8, 0x0, SHU3_PLL8_RG_RPHYPLL_POSDIV);
    //vIO32WriteFldAlign(DDRPHY_SHU4_PLL8, 0x0, SHU4_PLL8_RG_RPHYPLL_POSDIV);

    mcDELAY_US(1);

    vIO32WriteFldMulti(DDRPHY_SHU1_PLL9, P_Fld(0x0, SHU1_PLL9_RG_RPHYPLL_MONCK_EN)
                | P_Fld(0x0, SHU1_PLL9_RG_RPHYPLL_MONVC_EN)
                | P_Fld(0x0, SHU1_PLL9_RG_RPHYPLL_LVROD_EN)
                | P_Fld(0x0, SHU1_PLL9_RG_RPHYPLL_RST_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU1_PLL11, P_Fld(0x0, SHU1_PLL11_RG_RCLRPLL_MONCK_EN)
                | P_Fld(0x0, SHU1_PLL11_RG_RCLRPLL_MONVC_EN)
                | P_Fld(0x0, SHU1_PLL11_RG_RCLRPLL_LVROD_EN)
                | P_Fld(0x0, SHU1_PLL11_RG_RCLRPLL_RST_DLY));

    mcDELAY_US(1);

    //Ref clock should be 20M~30M, if MPLL=52M, Pre-divider should be set to 1
    vIO32WriteFldMulti(DDRPHY_SHU1_PLL10, P_Fld(0x0, SHU1_PLL10_RG_RCLRPLL_POSDIV) | P_Fld(0x1, SHU1_PLL10_RG_RCLRPLL_PREDIV));
    //vIO32WriteFldAlign(DDRPHY_SHU2_PLL10, 0x0, SHU2_PLL10_RG_RCLRPLL_POSDIV);
    //vIO32WriteFldAlign(DDRPHY_SHU3_PLL10, 0x0, SHU3_PLL10_RG_RCLRPLL_POSDIV);
    //vIO32WriteFldAlign(DDRPHY_SHU4_PLL10, 0x0, SHU4_PLL10_RG_RCLRPLL_POSDIV);

    mcDELAY_US(1);

    ///TODO: PLL EN
#if 0//ORIGINAL_PLL_INIT
    vIO32WriteFldAlign(DDRPHY_PLL1, 0x1, PLL1_RG_RPHYPLL_EN);
    vIO32WriteFldAlign(DDRPHY_PLL2, 0x1, PLL2_RG_RCLRPLL_EN);
    mcDELAY_US(100);
#endif


    ///TODO: MIDPI Init 1
    vIO32WriteFldMulti(DDRPHY_PLL4, P_Fld(0x0, PLL4_RG_RPHYPLL_AD_MCK8X_EN)
                | P_Fld(0x1, PLL4_PLL4_RFU)
                | P_Fld(0x1, PLL4_RG_RPHYPLL_MCK8X_SEL));

#if 0//ORIGINAL_PLL_INIT
    vIO32WriteFldMulti(DDRPHY_PLL4, P_Fld(0x1, PLL4_RG_RPHYPLL_ADA_MCK8X_EN)
                | P_Fld(0x1, PLL4_RG_RPHYPLL_RESETB));
#endif


    mcDELAY_US(1);

    vIO32WriteFldAlign(DDRPHY_SHU1_PLL0, 0x3, SHU1_PLL0_RG_RPHYPLL_TOP_REV); // debug1111, org:3 -> mdf:0
    //vIO32WriteFldAlign(DDRPHY_SHU2_PLL0, 0x3, SHU2_PLL0_RG_RPHYPLL_TOP_REV);
    //vIO32WriteFldAlign(DDRPHY_SHU3_PLL0, 0x3, SHU3_PLL0_RG_RPHYPLL_TOP_REV);
    //vIO32WriteFldAlign(DDRPHY_SHU4_PLL0, 0x3, SHU4_PLL0_RG_RPHYPLL_TOP_REV);

    mcDELAY_US(1);


    ///TODO: MIDPI Init 2
#if 0//ORIGINAL_PLL_INIT
    if(p->frequency>=1200)//DDR-2400, DDR-2667, DDR-3200, DDR3667
    {
        vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ6, P_Fld(0x1, SHU1_B0_DQ6_RG_ARPI_MIDPI_EN_B0)
                    | P_Fld(0x0, SHU1_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0));
        vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ6, P_Fld(0x1, SHU1_B1_DQ6_RG_ARPI_MIDPI_EN_B1)
                    | P_Fld(0x0, SHU1_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1));

        DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
            vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD6, P_Fld(0x1, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA)
                        | P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA));
            vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD6+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA)
                        | P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA));
        DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    }
    else
    {
        vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ6, P_Fld(0x0, SHU1_B0_DQ6_RG_ARPI_MIDPI_EN_B0)
                    | P_Fld(0x1, SHU1_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0));
        vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ6, P_Fld(0x0, SHU1_B1_DQ6_RG_ARPI_MIDPI_EN_B1)
                    | P_Fld(0x1, SHU1_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1));

        DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
        vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD6, P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA)
                    | P_Fld(0x1, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA));
        vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD6+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA)
                    | P_Fld(0x1, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA));
        DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    }
#endif

    mcDELAY_US(1);

    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI1, 0x1, CA_DLL_ARPI1_RG_ARPISM_MCK_SEL_CA);

#if (fcFOR_CHIP_ID == fcBianco)
        DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
        vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI3, P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_MCTL_EN_CA) 
                    | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_FB_EN_CA) 
                    | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CS_EN) 
                    | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CLK_EN) 
                    | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CMD_EN));
        vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI3+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x0, CA_DLL_ARPI3_RG_ARPI_MCTL_EN_CA) //CH_B CA slave
                    | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_FB_EN_CA) 
                    | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CS_EN) 
                    | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CLK_EN) 
                    | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CMD_EN));
        DramcBroadcastOnOff(DRAMC_BROADCAST_ON);                
#endif
    vIO32WriteFldMulti(DDRPHY_B0_DLL_ARPI3, P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_FB_EN_B0)
                | P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQS_EN_B0)
                | P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQM_EN_B0)
                | P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQ_EN_B0)
                | P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B0));
    vIO32WriteFldMulti(DDRPHY_B1_DLL_ARPI3, P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_FB_EN_B1)
                | P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQS_EN_B1)
                | P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQM_EN_B1)
                | P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQ_EN_B1)
                | P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B1));
    vIO32WriteFldMulti(DDRPHY_B0_DQ3, P_Fld(0x1, B0_DQ3_RG_RX_ARDQ_STBENCMP_EN_B0)
                | P_Fld(0x1, B0_DQ3_RG_TX_ARDQ_EN_B0)
                | P_Fld(0x1, B0_DQ3_RG_RX_ARDQ_SMT_EN_B0));
    vIO32WriteFldMulti(DDRPHY_B1_DQ3, P_Fld(0x1, B1_DQ3_RG_RX_ARDQ_STBENCMP_EN_B1)
                | P_Fld(0x1, B1_DQ3_RG_TX_ARDQ_EN_B1)
                | P_Fld(0x1, B1_DQ3_RG_RX_ARDQ_SMT_EN_B1));

#if (fcFOR_CHIP_ID == fcBianco)
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    #if 1//#ifndef BIANCO_TO_BE_PORTING
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_DLL0, 0x1, SHU1_CA_DLL0_RG_ARPISM_MCK_SEL_CA_SHU);
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_DLL0+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, SHU1_CA_DLL0_RG_ARPISM_MCK_SEL_CA_SHU);
    #endif
    vIO32WriteFldMulti(DDRPHY_SHU1_CA_DLL0, P_Fld(0x0, SHU1_CA_DLL0_RG_ARDLL_PHDET_OUT_SEL_CA) 
                | P_Fld(0x0, SHU1_CA_DLL0_RG_ARDLL_PHDET_IN_SWAP_CA) 
                | P_Fld(0x6, SHU1_CA_DLL0_RG_ARDLL_GAIN_CA) 
                | P_Fld(0x9, SHU1_CA_DLL0_RG_ARDLL_IDLECNT_CA) 
                | P_Fld(0x8, SHU1_CA_DLL0_RG_ARDLL_P_GAIN_CA) 
                | P_Fld(0x1, SHU1_CA_DLL0_RG_ARDLL_PHJUMP_EN_CA) 
                | P_Fld(0x1, SHU1_CA_DLL0_RG_ARDLL_PHDIV_CA)
                | P_Fld(0x1, SHU1_CA_DLL0_RG_ARDLL_FAST_PSJP_CA));
    //CH_B CA slave
    vIO32WriteFldMulti(DDRPHY_SHU1_CA_DLL0+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, SHU1_CA_DLL0_RG_ARDLL_PHDET_OUT_SEL_CA) 
                | P_Fld(0x1, SHU1_CA_DLL0_RG_ARDLL_PHDET_IN_SWAP_CA) 
                | P_Fld(0x7, SHU1_CA_DLL0_RG_ARDLL_GAIN_CA) 
                | P_Fld(0x7, SHU1_CA_DLL0_RG_ARDLL_IDLECNT_CA) 
                | P_Fld(0x8, SHU1_CA_DLL0_RG_ARDLL_P_GAIN_CA) 
                | P_Fld(0x1, SHU1_CA_DLL0_RG_ARDLL_PHJUMP_EN_CA) 
                | P_Fld(0x1, SHU1_CA_DLL0_RG_ARDLL_PHDIV_CA)
                | P_Fld(0x0, SHU1_CA_DLL0_RG_ARDLL_FAST_PSJP_CA));
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
#endif

    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DLL0, P_Fld(0x1, SHU1_B0_DLL0_RG_ARDLL_PHDET_IN_SWAP_B0)
                | P_Fld(0x7, SHU1_B0_DLL0_RG_ARDLL_GAIN_B0)
                | P_Fld(0x7, SHU1_B0_DLL0_RG_ARDLL_IDLECNT_B0)
                | P_Fld(0x8, SHU1_B0_DLL0_RG_ARDLL_P_GAIN_B0)
                | P_Fld(0x1, SHU1_B0_DLL0_RG_ARDLL_PHJUMP_EN_B0)
                | P_Fld(0x1, SHU1_B0_DLL0_RG_ARDLL_PHDIV_B0)
                | P_Fld(0x0, SHU1_B0_DLL0_RG_ARDLL_FAST_PSJP_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_B1_DLL0, P_Fld(0x1, SHU1_B1_DLL0_RG_ARDLL_PHDET_IN_SWAP_B1)
                | P_Fld(0x7, SHU1_B1_DLL0_RG_ARDLL_GAIN_B1)
                | P_Fld(0x7, SHU1_B1_DLL0_RG_ARDLL_IDLECNT_B1)
                | P_Fld(0x8, SHU1_B1_DLL0_RG_ARDLL_P_GAIN_B1)
                | P_Fld(0x1, SHU1_B1_DLL0_RG_ARDLL_PHJUMP_EN_B1)
                | P_Fld(0x1, SHU1_B1_DLL0_RG_ARDLL_PHDIV_B1)
                | P_Fld(0x0, SHU1_B1_DLL0_RG_ARDLL_FAST_PSJP_B1));
    vIO32WriteFldMulti(DDRPHY_CA_CMD8, P_Fld(0xF, CA_CMD8_RG_RRESETB_DRVN)
                | P_Fld(0xF, CA_CMD8_RG_RRESETB_DRVP)); // Darren
                //P_Fld(0x1, CA_CMD8_RG_TX_RRESETB_DDR4_SEL) Don't toggle RESETB_DDR4_SEL, Justin's feedback on RESETB issue

    vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD5, 0x0, SHU1_CA_CMD5_RG_RX_ARCMD_VREF_SEL);
    //vIO32WriteFldAlign(DDRPHY_SHU2_CA_CMD5, 0x0, SHU2_CA_CMD5_RG_RX_ARCMD_VREF_SEL);
    //vIO32WriteFldAlign(DDRPHY_SHU3_CA_CMD5, 0x0, SHU3_CA_CMD5_RG_RX_ARCMD_VREF_SEL);
    //vIO32WriteFldAlign(DDRPHY_SHU4_CA_CMD5, 0x0, SHU4_CA_CMD5_RG_RX_ARCMD_VREF_SEL);
    vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD0, P_Fld(0x1, SHU1_CA_CMD0_RG_TX_ARCMD_PRE_EN)
                | P_Fld(0x4, SHU1_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
                | P_Fld(0x1, SHU1_CA_CMD0_RG_TX_ARCLK_PRE_EN));
    //vIO32WriteFldMulti(DDRPHY_SHU2_CA_CMD0, P_Fld(0x1, SHU2_CA_CMD0_RG_TX_ARCMD_PRE_EN)
    //            | P_Fld(0x4, SHU2_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
    //            | P_Fld(0x1, SHU2_CA_CMD0_RG_TX_ARCLK_PRE_EN));
    //vIO32WriteFldMulti(DDRPHY_SHU3_CA_CMD0, P_Fld(0x1, SHU3_CA_CMD0_RG_TX_ARCMD_PRE_EN)
    //            | P_Fld(0x4, SHU3_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
    //            | P_Fld(0x1, SHU3_CA_CMD0_RG_TX_ARCLK_PRE_EN));
    //vIO32WriteFldMulti(DDRPHY_SHU4_CA_CMD0, P_Fld(0x1, SHU4_CA_CMD0_RG_TX_ARCMD_PRE_EN)
    //            | P_Fld(0x4, SHU4_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
    //            | P_Fld(0x1, SHU4_CA_CMD0_RG_TX_ARCLK_PRE_EN));
#if (fcFOR_CHIP_ID == fcBianco)
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD6, 0x3, SHU1_CA_CMD6_RG_ARPI_RESERVE_CA);
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD6+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, SHU1_CA_CMD6_RG_ARPI_RESERVE_CA);
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
#else
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD6, 0x3, SHU1_CA_CMD6_RG_ARPI_RESERVE_CA);
#endif
    //vIO32WriteFldAlign(DDRPHY_SHU2_CA_CMD6, 0x3, SHU2_CA_CMD6_RG_ARPI_RESERVE_CA);
    //vIO32WriteFldAlign(DDRPHY_SHU3_CA_CMD6, 0x3, SHU3_CA_CMD6_RG_ARPI_RESERVE_CA);
    //vIO32WriteFldAlign(DDRPHY_SHU4_CA_CMD6, 0x3, SHU4_CA_CMD6_RG_ARPI_RESERVE_CA);
    vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD3, 0x4e1, SHU1_CA_CMD3_RG_ARCMD_REV);
    //vIO32WriteFldAlign(DDRPHY_SHU2_CA_CMD7, 0x4e1, SHU2_CA_CMD7_RG_ARCMD_REV);
    //vIO32WriteFldAlign(DDRPHY_SHU3_CA_CMD7, 0x4e1, SHU3_CA_CMD7_RG_ARCMD_REV);
    //vIO32WriteFldAlign(DDRPHY_SHU4_CA_CMD7, 0x4e1, SHU4_CA_CMD7_RG_ARCMD_REV);
    //vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7, P_Fld(0x00, SHU1_B0_DQ7_RG_ARDQ_REV_B0)
    //            | P_Fld(0x0, SHU1_B0_DQ7_DQ_REV_B0_BIT_05));
    //vIO32WriteFldAlign(DDRPHY_SHU2_B0_DQ7, 0x20, SHU2_B0_DQ7_RG_ARDQ_REV_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU3_B0_DQ7, 0x20, SHU3_B0_DQ7_RG_ARDQ_REV_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU4_B0_DQ7, 0x20, SHU4_B0_DQ7_RG_ARDQ_REV_B0);
    //vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ7, P_Fld(0x00, SHU1_B1_DQ7_RG_ARDQ_REV_B1)
    //            | P_Fld(0x0, SHU1_B1_DQ7_DQ_REV_B1_BIT_05));

 //lynx added
    //vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ7, 0x0, SHU1_B0_DQ7_RG_ARDQ_REV_B0); 
    //vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ7, 0x0, SHU1_B1_DQ7_RG_ARDQ_REV_B1); 
 //
    //vIO32WriteFldAlign(DDRPHY_SHU2_B1_DQ7, 0x20, SHU2_B1_DQ7_RG_ARDQ_REV_B1);
    //vIO32WriteFldAlign(DDRPHY_SHU3_B1_DQ7, 0x20, SHU3_B1_DQ7_RG_ARDQ_REV_B1);
    //vIO32WriteFldAlign(DDRPHY_SHU4_B1_DQ7, 0x20, SHU4_B1_DQ7_RG_ARDQ_REV_B1);
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ6, 0x1, SHU1_B0_DQ6_RG_ARPI_RESERVE_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU2_B0_DQ6, 0x1, SHU2_B0_DQ6_RG_ARPI_RESERVE_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU3_B0_DQ6, 0x1, SHU3_B0_DQ6_RG_ARPI_RESERVE_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU4_B0_DQ6, 0x1, SHU4_B0_DQ6_RG_ARPI_RESERVE_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ6, 0x1, SHU1_B1_DQ6_RG_ARPI_RESERVE_B1);
    //vIO32WriteFldAlign(DDRPHY_SHU2_B1_DQ6, 0x1, SHU2_B1_DQ6_RG_ARPI_RESERVE_B1);
    //vIO32WriteFldAlign(DDRPHY_SHU3_B1_DQ6, 0x1, SHU3_B1_DQ6_RG_ARPI_RESERVE_B1);
    //vIO32WriteFldAlign(DDRPHY_SHU4_B1_DQ6, 0x1, SHU4_B1_DQ6_RG_ARPI_RESERVE_B1);
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    vIO32WriteFldMulti(DDRPHY_MISC_SHU_OPT, P_Fld(0x1, MISC_SHU_OPT_R_CA_SHU_PHDET_SPM_EN)
                | P_Fld(0x1, MISC_SHU_OPT_R_CA_SHU_PHY_GATING_RESETB_SPM_EN)
                | P_Fld(0x2, MISC_SHU_OPT_R_DQB1_SHU_PHDET_SPM_EN)
                | P_Fld(0x1, MISC_SHU_OPT_R_DQB1_SHU_PHY_GATING_RESETB_SPM_EN)
                | P_Fld(0x2, MISC_SHU_OPT_R_DQB0_SHU_PHDET_SPM_EN)
                | P_Fld(0x1, MISC_SHU_OPT_R_DQB0_SHU_PHY_GATING_RESETB_SPM_EN));
    vIO32WriteFldMulti(DDRPHY_MISC_SHU_OPT+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x2, MISC_SHU_OPT_R_CA_SHU_PHDET_SPM_EN) 
                | P_Fld(0x1, MISC_SHU_OPT_R_CA_SHU_PHY_GATING_RESETB_SPM_EN) 
                | P_Fld(0x2, MISC_SHU_OPT_R_DQB1_SHU_PHDET_SPM_EN) 
                | P_Fld(0x1, MISC_SHU_OPT_R_DQB1_SHU_PHY_GATING_RESETB_SPM_EN) 
                | P_Fld(0x2, MISC_SHU_OPT_R_DQB0_SHU_PHDET_SPM_EN) 
                | P_Fld(0x1, MISC_SHU_OPT_R_DQB0_SHU_PHY_GATING_RESETB_SPM_EN));
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);

    mcDELAY_US(9);

#if (fcFOR_CHIP_ID == fcBianco)
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    vIO32WriteFldMulti(DDRPHY_SHU1_CA_DLL1, P_Fld(0x1, SHU1_CA_DLL1_RG_ARDLL_PD_CK_SEL_CA) | P_Fld(0x0, SHU1_CA_DLL1_RG_ARDLL_FASTPJ_CK_SEL_CA));
    vIO32WriteFldMulti(DDRPHY_SHU1_CA_DLL1+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x0, SHU1_CA_DLL1_RG_ARDLL_PD_CK_SEL_CA)
                | P_Fld(0x1, SHU1_CA_DLL1_RG_ARDLL_FASTPJ_CK_SEL_CA));//CH_B CA slave mode
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
#endif
    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DLL1, P_Fld(0x0, SHU1_B0_DLL1_RG_ARDLL_PD_CK_SEL_B0) | P_Fld(0x1, SHU1_B0_DLL1_RG_ARDLL_FASTPJ_CK_SEL_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_B1_DLL1, P_Fld(0x0, SHU1_B1_DLL1_RG_ARDLL_PD_CK_SEL_B1) | P_Fld(0x1, SHU1_B1_DLL1_RG_ARDLL_FASTPJ_CK_SEL_B1));

    mcDELAY_US(1);

    vIO32WriteFldAlign(DDRPHY_PLL2, 0x0, PLL2_RG_RCLRPLL_EN);
    vIO32WriteFldAlign(DDRPHY_SHU1_PLL4, 0xff, SHU1_PLL4_RG_RPHYPLL_RESERVED);
    vIO32WriteFldAlign(DDRPHY_SHU1_PLL6, 0xff, SHU1_PLL6_RG_RCLRPLL_RESERVED);
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 0xf, MISC_CG_CTRL0_CLK_MEM_DFS_CFG);
#if 0//ORIGINAL_PLL_INIT
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 0x1, MISC_CG_CTRL0_CLK_MEM_SEL);
#endif

    mcDELAY_US(1);


    ///TODO: DLL EN
#if 0//ORIGINAL_PLL_INIT
    //*((UINT32P)(DDRPHY0AO_BASE + 0x0194)) = 0x00698e08;
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI5, 0x1, CA_DLL_ARPI5_RG_ARDLL_PHDET_EN_CA);
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI5+((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, CA_DLL_ARPI5_RG_ARDLL_PHDET_EN_CA);
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    mcDELAY_US(1);

    vIO32WriteFldAlign(DDRPHY_B0_DLL_ARPI5, 0x1, B0_DLL_ARPI5_RG_ARDLL_PHDET_EN_B0);
    mcDELAY_US(1);
    vIO32WriteFldAlign(DDRPHY_B1_DLL_ARPI5, 0x1, B1_DLL_ARPI5_RG_ARDLL_PHDET_EN_B1);
    mcDELAY_US(1);
#endif

#if !ORIGINAL_PLL_INIT
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    DDRPhyPLLSetting(p);
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
#endif

    vIO32WriteFldAlign(DDRPHY_B0_DQ9, 0x1, B0_DQ9_RG_RX_ARDQS_STBEN_RESETB_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ9, 0x1, B1_DQ9_RG_RX_ARDQS_STBEN_RESETB_B1);
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQSIEN, P_Fld(0xf, SHURK1_DQSIEN_R1DQS3IEN)
                | P_Fld(0xf, SHURK1_DQSIEN_R1DQS2IEN)
                | P_Fld(0xf, SHURK1_DQSIEN_R1DQS1IEN)
                | P_Fld(0xf, SHURK1_DQSIEN_R1DQS0IEN));
    vIO32WriteFldMulti(DRAMC_REG_STBCAL1, P_Fld(0x0, STBCAL1_DLLFRZ_MON_PBREF_OPT)
                | P_Fld(0x1, STBCAL1_STB_FLAGCLR)
                | P_Fld(0x1, STBCAL1_STBCNT_MODESEL));
    vIO32WriteFldMulti(DRAMC_REG_SHU_DQSG_RETRY, P_Fld(0x1, SHU_DQSG_RETRY_R_RETRY_USE_BURST_MDOE)
                | P_Fld(0x1, SHU_DQSG_RETRY_R_RDY_SEL_DLE)
                | P_Fld(0x6, SHU_DQSG_RETRY_R_DQSIENLAT)
                | P_Fld(0x1, SHU_DQSG_RETRY_R_RETRY_ONCE));
    vIO32WriteFldMulti(DRAMC_REG_SHU1_DRVING1, P_Fld(0xa, SHU1_DRVING1_DQSDRVP2) | P_Fld(0xa, SHU1_DRVING1_DQSDRVN2)
                | P_Fld(0xa, SHU1_DRVING1_DQSDRVP1) | P_Fld(0xa, SHU1_DRVING1_DQSDRVN1)
                | P_Fld(0xa, SHU1_DRVING1_DQDRVP2) | P_Fld(0xa, SHU1_DRVING1_DQDRVN2));
    vIO32WriteFldMulti(DRAMC_REG_SHU1_DRVING2, P_Fld(0xa, SHU1_DRVING2_DQDRVP1) | P_Fld(0xa, SHU1_DRVING2_DQDRVN1)
                | P_Fld(0xa, SHU1_DRVING2_CMDDRVP2) | P_Fld(0xa, SHU1_DRVING2_CMDDRVN2)
                | P_Fld(0xa, SHU1_DRVING2_CMDDRVP1) | P_Fld(0xa, SHU1_DRVING2_CMDDRVN1));
    vIO32WriteFldMulti(DRAMC_REG_SHU1_DRVING3, P_Fld(0xa, SHU1_DRVING3_DQSODTP2) | P_Fld(0xa, SHU1_DRVING3_DQSODTN2)
                | P_Fld(0xa, SHU1_DRVING3_DQSODTP) | P_Fld(0xa, SHU1_DRVING3_DQSODTN)
                | P_Fld(0xa, SHU1_DRVING3_DQODTP2) | P_Fld(0xa, SHU1_DRVING3_DQODTN2));
    vIO32WriteFldMulti(DRAMC_REG_SHU1_DRVING4, P_Fld(0xa, SHU1_DRVING4_DQODTP1) | P_Fld(0xa, SHU1_DRVING4_DQODTN1)
                | P_Fld(0xa, SHU1_DRVING4_CMDODTP2) | P_Fld(0xa, SHU1_DRVING4_CMDODTN2)
                | P_Fld(0xa, SHU1_DRVING4_CMDODTP1) | P_Fld(0xa, SHU1_DRVING4_CMDODTN1));
    /*
    vIO32WriteFldMulti(DRAMC_REG_SHU2_DRVING1, P_Fld(0x14a, SHU2_DRVING1_DQSDRV2)
                | P_Fld(0x14a, SHU2_DRVING1_DQSDRV1)
                | P_Fld(0x14a, SHU2_DRVING1_DQDRV2));
    vIO32WriteFldMulti(DRAMC_REG_SHU2_DRVING2, P_Fld(0x14a, SHU2_DRVING2_DQDRV1)
                | P_Fld(0x14a, SHU2_DRVING2_CMDDRV2)
                | P_Fld(0x14a, SHU2_DRVING2_CMDDRV1));
    vIO32WriteFldMulti(DRAMC_REG_SHU2_DRVING3, P_Fld(0x14a, SHU2_DRVING3_DQSODT2)
                | P_Fld(0x14a, SHU2_DRVING3_DQSODT1)
                | P_Fld(0x14a, SHU2_DRVING3_DQODT2));
    vIO32WriteFldMulti(DRAMC_REG_SHU2_DRVING4, P_Fld(0x14a, SHU2_DRVING4_DQODT1)
                | P_Fld(0x14a, SHU2_DRVING4_CMDODT2)
                | P_Fld(0x14a, SHU2_DRVING4_CMDODT1));
    vIO32WriteFldMulti(DRAMC_REG_SHU3_DRVING1, P_Fld(0x14a, SHU3_DRVING1_DQSDRV2)
                | P_Fld(0x14a, SHU3_DRVING1_DQSDRV1)
                | P_Fld(0x14a, SHU3_DRVING1_DQDRV2));
    vIO32WriteFldMulti(DRAMC_REG_SHU3_DRVING2, P_Fld(0x14a, SHU3_DRVING2_DQDRV1)
                | P_Fld(0x14a, SHU3_DRVING2_CMDDRV2)
                | P_Fld(0x14a, SHU3_DRVING2_CMDDRV1));
    vIO32WriteFldMulti(DRAMC_REG_SHU3_DRVING3, P_Fld(0x14a, SHU3_DRVING3_DQSODT2)
                | P_Fld(0x14a, SHU3_DRVING3_DQSODT1)
                | P_Fld(0x14a, SHU3_DRVING3_DQODT2));
    vIO32WriteFldMulti(DRAMC_REG_SHU3_DRVING4, P_Fld(0x14a, SHU3_DRVING4_DQODT1)
                | P_Fld(0x14a, SHU3_DRVING4_CMDODT2)
                | P_Fld(0x14a, SHU3_DRVING4_CMDODT1));
    vIO32WriteFldMulti(DRAMC_REG_SHU4_DRVING1, P_Fld(0x14a, SHU4_DRVING1_DQSDRV2)
                | P_Fld(0x14a, SHU4_DRVING1_DQSDRV1)
                | P_Fld(0x14a, SHU4_DRVING1_DQDRV2));
    vIO32WriteFldMulti(DRAMC_REG_SHU4_DRVING2, P_Fld(0x14a, SHU4_DRVING2_DQDRV1)
                | P_Fld(0x14a, SHU4_DRVING2_CMDDRV2)
                | P_Fld(0x14a, SHU4_DRVING2_CMDDRV1));
    */
    //  *((UINT32P)(DRAMC1_AO_BASE + 0x08a8)) = 0x14a5294a;
    //  *((UINT32P)(DRAMC1_AO_BASE + 0x08ac)) = 0x14a5294a;
    //  *((UINT32P)(DRAMC1_AO_BASE + 0x08b0)) = 0x14a5294a;
    //  *((UINT32P)(DRAMC1_AO_BASE + 0x08b4)) = 0x14a5294a;
    //  *((UINT32P)(DRAMC1_AO_BASE + 0x0ea8)) = 0x14a5294a;
    //  *((UINT32P)(DRAMC1_AO_BASE + 0x0eac)) = 0x14a5294a;
    //  *((UINT32P)(DRAMC1_AO_BASE + 0x0eb0)) = 0x14a5294a;
    //  *((UINT32P)(DRAMC1_AO_BASE + 0x0eb4)) = 0x14a5294a;
    //  *((UINT32P)(DRAMC1_AO_BASE + 0x14a8)) = 0x14a5294a;
    //  *((UINT32P)(DRAMC1_AO_BASE + 0x14ac)) = 0x14a5294a;
    //  *((UINT32P)(DRAMC1_AO_BASE + 0x14b0)) = 0x14a5294a;
    //  *((UINT32P)(DRAMC1_AO_BASE + 0x14b4)) = 0x14a5294a;
    //  *((UINT32P)(DRAMC1_AO_BASE + 0x1aa8)) = 0x14a5294a;
    //  *((UINT32P)(DRAMC1_AO_BASE + 0x1aac)) = 0x14a5294a;
    vIO32WriteFldMulti(DRAMC_REG_SHUCTRL2, P_Fld(0x0, SHUCTRL2_HWSET_WLRL)
                | P_Fld(0x1, SHUCTRL2_SHU_PERIOD_GO_ZERO_CNT)
                | P_Fld(0x1, SHUCTRL2_R_DVFS_OPTION)
                | P_Fld(0x1, SHUCTRL2_R_DVFS_PARK_N)
                | P_Fld(0x1, SHUCTRL2_R_DVFS_DLL_CHA)
                | P_Fld(0xa, SHUCTRL2_R_DLL_IDLE));
    vIO32WriteFldAlign(DRAMC_REG_DVFSDLL, 0x1, DVFSDLL_DLL_LOCK_SHU_EN);
    vIO32WriteFldMulti(DRAMC_REG_DDRCONF0, P_Fld(0x1, DDRCONF0_LPDDR4EN)
                | P_Fld(0x1, DDRCONF0_DM64BITEN)
                | P_Fld(0x1, DDRCONF0_BC4OTF)
                | P_Fld(0x1, DDRCONF0_BK8EN));
    vIO32WriteFldMulti(DRAMC_REG_STBCAL2, P_Fld(0x1, STBCAL2_STB_GERR_B01)
                | P_Fld(0x1, STBCAL2_STB_GERRSTOP)
                | P_Fld(0x1, EYESCAN_EYESCAN_RD_SEL_OPT));
    vIO32WriteFldAlign(DRAMC_REG_STBCAL2, 0x1, STBCAL2_STB_GERR_RST);
    vIO32WriteFldAlign(DRAMC_REG_STBCAL2, 0x0, STBCAL2_STB_GERR_RST);
    vIO32WriteFldAlign(DRAMC_REG_CLKAR, 0x1, CLKAR_PSELAR);
    vIO32WriteFldAlign(DDRPHY_B0_DQ9, 0x1, B0_DQ9_R_DMDQSIEN_RDSEL_LAT_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ9, 0x1, B1_DQ9_R_DMDQSIEN_RDSEL_LAT_B1);
    vIO32WriteFldAlign(DDRPHY_CA_CMD10, 0x0, CA_CMD10_R_DMDQSIEN_RDSEL_LAT_CA);
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DDRPHY_MISC_CTRL0, P_Fld(0x1, MISC_CTRL0_R_STBENCMP_DIV4CK_EN)
                    | P_Fld(0x1, MISC_CTRL0_R_DMDQSIEN_FIFO_EN)
                    | P_Fld(0x1, MISC_CTRL0_R_DMSTBEN_OUTSEL)
                    | P_Fld(0xf, MISC_CTRL0_R_DMDQSIEN_SYNCOPT));
    }
    else
    {
        vIO32WriteFldMulti(DDRPHY_MISC_CTRL0, P_Fld(0x1, MISC_CTRL0_R_STBENCMP_DIV4CK_EN)
                    | P_Fld(0x1, MISC_CTRL0_R_DMDQSIEN_FIFO_EN)
                    | P_Fld(0x1, MISC_CTRL0_R_DMSTBEN_OUTSEL)
                    | P_Fld(0xf, MISC_CTRL0_R_DMDQSIEN_SYNCOPT));
    }
    //vIO32WriteFldMulti(DDRPHY_MISC_CTRL1, P_Fld(0x1, MISC_CTRL1_R_DMDA_RRESETB_E) //Already set in vDramcInit_PreSettings()
    vIO32WriteFldMulti(DDRPHY_MISC_CTRL1, P_Fld(0x1, MISC_CTRL1_R_DMDQSIENCG_EN)
                | P_Fld(0x1, MISC_CTRL1_R_DMRXDVS_SHUREG_EN) 
                | P_Fld(0x1, MISC_CTRL1_R_DM_TX_ARCMD_OE)
                | P_Fld(0x1, MISC_CTRL1_R_DM_TX_ARCLK_OE));
    vIO32WriteFldAlign(DDRPHY_B0_RXDVS0, 1, B0_RXDVS0_R_HWSAVE_MODE_ENA_B0);
    vIO32WriteFldAlign(DDRPHY_B1_RXDVS0, 1, B1_RXDVS0_R_HWSAVE_MODE_ENA_B1);
    vIO32WriteFldAlign(DDRPHY_CA_RXDVS0, 0, CA_RXDVS0_R_HWSAVE_MODE_ENA_CA);
    
    vIO32WriteFldAlign(DDRPHY_CA_CMD7, 0x0, CA_CMD7_RG_TX_ARCMD_PULL_DN);
    vIO32WriteFldAlign(DDRPHY_CA_CMD7, 0x0, CA_CMD7_RG_TX_ARCS_PULL_DN); // Added by Lingyun.Wu, 11-15
    vIO32WriteFldAlign(DDRPHY_B0_DQ7, 0x0, B0_DQ7_RG_TX_ARDQ_PULL_DN_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ7, 0x0, B1_DQ7_RG_TX_ARDQ_PULL_DN_B1);
    //vIO32WriteFldAlign(DDRPHY_CA_CMD8, 0x0, CA_CMD8_RG_TX_RRESETB_PULL_DN); //Already set in vDramcInit_PreSettings()
    
    vIO32WriteFldMulti(DRAMC_REG_SHU_CONF0, P_Fld(0x2, SHU_CONF0_MATYPE)
                | P_Fld(0x1, SHU_CONF0_BL4)
                | P_Fld(0x1, SHU_CONF0_FREQDIV4)
                | P_Fld(0x1, SHU_CONF0_REFTHD)
                | P_Fld(0x1, SHU_CONF0_ADVPREEN)
                | P_Fld(0x3f, SHU_CONF0_DMPGTIM));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ODTCTRL, P_Fld(0x1, SHU_ODTCTRL_RODTE)
                | P_Fld(0x1, SHU_ODTCTRL_RODTE2)
                | P_Fld(0x1, SHU_ODTCTRL_TWODT)
                | P_Fld(0x5, SHU_ODTCTRL_RODT)
                | P_Fld(0x1, SHU_ODTCTRL_WOEN)
                | P_Fld(0x1, SHU_ODTCTRL_ROEN));
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ7, 0x1, SHU1_B0_DQ7_R_DMRODTEN_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ7, 0x1, SHU1_B1_DQ7_R_DMRODTEN_B1);
    vIO32WriteFldAlign(DRAMC_REG_REFCTRL0, 0x5, REFCTRL0_REF_PREGATE_CNT);
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_CA1, P_Fld(0x0, SHU_SELPH_CA1_TXDLY_CS1)
                | P_Fld(0x0, SHU_SELPH_CA1_TXDLY_RAS)
                | P_Fld(0x0, SHU_SELPH_CA1_TXDLY_CAS)
                | P_Fld(0x0, SHU_SELPH_CA1_TXDLY_WE)
                | P_Fld(0x0, SHU_SELPH_CA1_TXDLY_RESET)
                | P_Fld(0x0, SHU_SELPH_CA1_TXDLY_ODT)
                | P_Fld(0x0, SHU_SELPH_CA1_TXDLY_CKE)
                | P_Fld(0x0, SHU_SELPH_CA1_TXDLY_CS));
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_CA2, P_Fld(0x0, SHU_SELPH_CA2_TXDLY_CKE1)
                | P_Fld(0x7, SHU_SELPH_CA2_TXDLY_CMD)
                | P_Fld(0x0, SHU_SELPH_CA2_TXDLY_BA2)
                | P_Fld(0x0, SHU_SELPH_CA2_TXDLY_BA1)
                | P_Fld(0x0, SHU_SELPH_CA2_TXDLY_BA0));
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_CA3, P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA7)
                | P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA6)
                | P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA5)
                | P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA4)
                | P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA3)
                | P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA2)
                | P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA1)
                | P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA0));
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_CA4, P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA15)
                | P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA14)
                | P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA13)
                | P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA12)
                | P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA11)
                | P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA10)
                | P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA9)
                | P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA8));
    vIO32WriteFldAlign(DRAMC_REG_SHU_SELPH_CA5, 0x0, SHU_SELPH_CA5_DLY_ODT);
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS0, P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS3)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS2)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS1)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS0)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_DQS3)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_DQS2)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_DQS1)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_DQS0));
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS1, P_Fld(0x2, SHU_SELPH_DQS1_DLY_OEN_DQS3)
                | P_Fld(0x2, SHU_SELPH_DQS1_DLY_OEN_DQS2)
                | P_Fld(0x2, SHU_SELPH_DQS1_DLY_OEN_DQS1)
                | P_Fld(0x2, SHU_SELPH_DQS1_DLY_OEN_DQS0)
                | P_Fld(0x5, SHU_SELPH_DQS1_DLY_DQS3)
                | P_Fld(0x5, SHU_SELPH_DQS1_DLY_DQS2)
                | P_Fld(0x5, SHU_SELPH_DQS1_DLY_DQS1)
                | P_Fld(0x5, SHU_SELPH_DQS1_DLY_DQS0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3)
                | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2)
                | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1)
                | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0)
                | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ3)
                | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ2)
                | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ1)
                | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3)
                | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2)
                | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1)
                | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0)
                | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM3)
                | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM2)
                | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM1)
                | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ2, P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_OEN_DQ3)
                | P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_OEN_DQ2)
                | P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_OEN_DQ1)
                | P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_OEN_DQ0)
                | P_Fld(0x6, SHURK0_SELPH_DQ2_DLY_DQ3)
                | P_Fld(0x6, SHURK0_SELPH_DQ2_DLY_DQ2)
                | P_Fld(0x6, SHURK0_SELPH_DQ2_DLY_DQ1)
                | P_Fld(0x6, SHURK0_SELPH_DQ2_DLY_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ3, P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_OEN_DQM3)
                | P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_OEN_DQM2)
                | P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_OEN_DQM1)
                | P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_OEN_DQM0)
                | P_Fld(0x6, SHURK0_SELPH_DQ3_DLY_DQM3)
                | P_Fld(0x6, SHURK0_SELPH_DQ3_DLY_DQM2)
                | P_Fld(0x6, SHURK0_SELPH_DQ3_DLY_DQM1)
                | P_Fld(0x6, SHURK0_SELPH_DQ3_DLY_DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3)
                | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2)
                | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1)
                | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0)
                | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ3)
                | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ2)
                | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ1)
                | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3)
                | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2)
                | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1)
                | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0)
                | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM3)
                | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM2)
                | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM1)
                | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ2, P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ3)
                | P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ2)
                | P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ1)
                | P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ0)
                | P_Fld(0x6, SHURK1_SELPH_DQ2_DLY_R1DQ3)
                | P_Fld(0x6, SHURK1_SELPH_DQ2_DLY_R1DQ2)
                | P_Fld(0x6, SHURK1_SELPH_DQ2_DLY_R1DQ1)
                | P_Fld(0x6, SHURK1_SELPH_DQ2_DLY_R1DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ3, P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM3)
                | P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM2)
                | P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM1)
                | P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM0)
                | P_Fld(0x6, SHURK1_SELPH_DQ3_DLY_R1DQM3)
                | P_Fld(0x6, SHURK1_SELPH_DQ3_DLY_R1DQM2)
                | P_Fld(0x6, SHURK1_SELPH_DQ3_DLY_R1DQM1)
                | P_Fld(0x6, SHURK1_SELPH_DQ3_DLY_R1DQM0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ7, P_Fld(0x1a, SHU1_R0_B1_DQ7_RK0_ARPI_DQM_B1)
                | P_Fld(0x1a, SHU1_R0_B1_DQ7_RK0_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ7, P_Fld(0x1a, SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0)
                | P_Fld(0x1a, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ7, P_Fld(0x14, SHU1_R1_B1_DQ7_RK1_ARPI_DQM_B1)
                | P_Fld(0x14, SHU1_R1_B1_DQ7_RK1_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ7, P_Fld(0x14, SHU1_R1_B0_DQ7_RK1_ARPI_DQM_B0)
                | P_Fld(0x14, SHU1_R1_B0_DQ7_RK1_ARPI_DQ_B0));
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN1, P_Fld(0x2, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN_P1)
                    | P_Fld(0x2, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN)
                    | P_Fld(0x2, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN_P1)
                    | P_Fld(0x2, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN)
                    | P_Fld(0x6, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN_P1)
                    | P_Fld(0x6, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN)
                    | P_Fld(0x6, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN_P1)
                    | P_Fld(0x6, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN0, P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN_P1)
                    | P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN)
                    | P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN_P1)
                    | P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN)
                    | P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN_P1)
                    | P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN)
                    | P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN_P1)
                    | P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN1, P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B3_R1RODTEN_P1)
                    | P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B3_R1RODTEN)
                    | P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B2_R1RODTEN_P1)
                    | P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B2_R1RODTEN)
                    | P_Fld(0x2, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN_P1)
                    | P_Fld(0x2, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN)
                    | P_Fld(0x2, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN_P1)
                    | P_Fld(0x2, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN1, P_Fld(0x2, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN_P1)
                    | P_Fld(0x2, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN)
                    | P_Fld(0x2, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN_P1)
                    | P_Fld(0x2, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN));    
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN0, P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN_P1)
                    | P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN)
                    | P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN_P1)
                    | P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN1, P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B3_R1RODTEN_P1)
                    | P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B3_R1RODTEN)
                    | P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B2_R1RODTEN_P1)
                    | P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B2_R1RODTEN)
                    | P_Fld(0x7, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN_P1)
                    | P_Fld(0x7, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN)
                    | P_Fld(0x7, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN_P1)
                    | P_Fld(0x7, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN));
    }

    mcDELAY_US(1);

    vIO32WriteFldAlign(DDRPHY_B1_DQ9, 0x1, B1_DQ9_RG_RX_ARDQS_DQSIENMODE_B1);
    vIO32WriteFldAlign(DDRPHY_B0_DQ9, 0x1, B0_DQ9_RG_RX_ARDQS_DQSIENMODE_B0);
    vIO32WriteFldAlign(DDRPHY_B0_DQ6, 0x1, B0_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ6, 0x1, B1_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B1);
    vIO32WriteFldAlign(DRAMC_REG_STBCAL, 0x1, STBCAL_DQSIENMODE);
    vIO32WriteFldMulti(DRAMC_REG_SREFCTRL, P_Fld(0x0, SREFCTRL_SREF_HW_EN)
                | P_Fld(0x8, SREFCTRL_SREFDLY));
    vIO32WriteFldMulti(DRAMC_REG_SHU_CKECTRL, P_Fld(0x3, SHU_CKECTRL_SREF_CK_DLY)
                | P_Fld(0x3, SHU_CKECTRL_TCKESRX));
                //| P_Fld(0x3, SHU_CKECTRL_CKEPRD));
    vIO32WriteFldMulti(DRAMC_REG_SHU_PIPE, P_Fld(0x1, SHU_PIPE_READ_START_EXTEND1)
                | P_Fld(0x1, SHU_PIPE_DLE_LAST_EXTEND1));
    vIO32WriteFldMulti(DRAMC_REG_CKECTRL, P_Fld(0x1, CKECTRL_CKEON)
                | P_Fld(0x1, CKECTRL_CKETIMER_SEL));
    vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0x1, RKCFG_CKE2RANK_OPT2);
#if LEGACY_SET_ACTIMING
    Legacy_DramcSet_LP4_DDR3200_ACTimingRegs(p);
#endif /* LEGACY_SET_ACTIMING */
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU_CONF2, P_Fld(0x1, SHU_CONF2_WPRE2T)
                    | P_Fld(0x7, SHU_CONF2_DCMDLYREF)
                    | P_Fld(0x64, SHU_CONF2_FSPCHG_PRDCNT));
        vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x1, SPCMDCTRL_CLR_EN);
        vIO32WriteFldAlign(DRAMC_REG_SHU_SCINTV, 0xf, SHU_SCINTV_MRW_INTV);
        vIO32WriteFldAlign(DRAMC_REG_SHUCTRL1, 0x40, SHUCTRL1_FC_PRDCNT);
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU_CONF2, P_Fld(0x1, SHU_CONF2_WPRE2T)
                    | P_Fld(0x7, SHU_CONF2_DCMDLYREF)
                    | P_Fld(0x64, SHU_CONF2_FSPCHG_PRDCNT));
        vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x1, SPCMDCTRL_CLR_EN);
        vIO32WriteFldAlign(DRAMC_REG_SHU_SCINTV, 0xf, SHU_SCINTV_MRW_INTV);
        vIO32WriteFldAlign(DRAMC_REG_SHUCTRL1, 0x40, SHUCTRL1_FC_PRDCNT);
    }
    vIO32WriteFldAlign(DRAMC_REG_SHUCTRL, 0x1, SHUCTRL_LPSM_BYPASS_B);
    vIO32WriteFldMulti(DRAMC_REG_REFCTRL1, P_Fld(0x0, REFCTRL1_SREF_PRD_OPT) | P_Fld(0x0, REFCTRL1_PSEL_OPT1) | P_Fld(0x0, REFCTRL1_PSEL_OPT2) | P_Fld(0x0, REFCTRL1_PSEL_OPT3));
    vIO32WriteFldAlign(DDRPHY_SHU1_PLL4, 0xfe, SHU1_PLL4_RG_RPHYPLL_RESERVED);
    vIO32WriteFldAlign(DDRPHY_SHU1_PLL6, 0xfe, SHU1_PLL6_RG_RCLRPLL_RESERVED);
    vIO32WriteFldMulti(DRAMC_REG_REFRATRE_FILTER, P_Fld(0x1, REFRATRE_FILTER_PB2AB_OPT) | P_Fld(0x0, REFRATRE_FILTER_PB2AB_OPT1));

#if !APPLY_LP4_POWER_INIT_SEQUENCE
    vIO32WriteFldAlign(DDRPHY_MISC_CTRL1, 0x1, MISC_CTRL1_R_DMDA_RRESETB_I);
#endif
    vIO32WriteFldAlign(DRAMC_REG_DRAMCTRL, 0x0, DRAMCTRL_CLKWITRFC);
    vIO32WriteFldMulti(DRAMC_REG_MISCTL0, P_Fld(0x1, MISCTL0_REFP_ARB_EN2)
                | P_Fld(0x1, MISCTL0_PBC_ARB_EN)
                | P_Fld(0x1, MISCTL0_REFA_ARB_EN2));
    vIO32WriteFldMulti(DRAMC_REG_PERFCTL0, P_Fld(0x1, PERFCTL0_MWHPRIEN)
                | P_Fld(0x1, PERFCTL0_RWSPLIT)
                | P_Fld(0x1, PERFCTL0_WFLUSHEN)
                | P_Fld(0x1, PERFCTL0_EMILLATEN)
                | P_Fld(0x1, PERFCTL0_RWAGEEN)
                | P_Fld(0x1, PERFCTL0_RWLLATEN)
                | P_Fld(0x1, PERFCTL0_RWHPRIEN)
                | P_Fld(0x1, PERFCTL0_RWOFOEN)
                | P_Fld(0x1, PERFCTL0_DISRDPHASE1)
                | P_Fld(0x1, PERFCTL0_DUALSCHEN));
    vIO32WriteFldAlign(DRAMC_REG_ARBCTL, 0x80, ARBCTL_MAXPENDCNT);
    vIO32WriteFldMulti(DRAMC_REG_PADCTRL, P_Fld(0x1, PADCTRL_DQIENLATEBEGIN)
                | P_Fld(0x1, PADCTRL_DQIENQKEND));
    vIO32WriteFldAlign(DRAMC_REG_DRAMC_PD_CTRL, 0x1, DRAMC_PD_CTRL_DCMREF_OPT);
    vIO32WriteFldAlign(DRAMC_REG_CLKCTRL, 0x1, CLKCTRL_CLK_EN_1);
    vIO32WriteFldMulti(DRAMC_REG_REFCTRL0, P_Fld(0x4, REFCTRL0_DISBYREFNUM)
                | P_Fld(0x1, REFCTRL0_DLLFRZ));
    vIO32WriteFldMulti(DRAMC_REG_CATRAINING1, P_Fld(0xff, CATRAINING1_CATRAIN_INTV)
                | P_Fld(0x0, CATRAINING1_CATRAINLAT));
    vIO32WriteFldMulti(DRAMC_REG_SHU_RANKCTL, P_Fld(0x6, SHU_RANKCTL_RANKINCTL_PHY)
                | P_Fld(0x4, SHU_RANKCTL_RANKINCTL_ROOT1)
                | P_Fld(0x4, SHU_RANKCTL_RANKINCTL));
                
    vIO32WriteFldAlign(DRAMC_REG_SHU_STBCAL, 0x1, SHU_STBCAL_DMSTBLAT);
    vIO32WriteFldAlign(DRAMC_REG_SHURK0_DQSCTL, 0x5, SHURK0_DQSCTL_DQSINCTL);
    vIO32WriteFldAlign(DRAMC_REG_SHURK1_DQSCTL, 0x5, SHURK1_DQSCTL_R1DQSINCTL);

    mcDELAY_US(2);

    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0, P_Fld(0x4, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
                    | P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
                    | P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
                    | P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(0x4, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
                    | P_Fld(0x7, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
                    | P_Fld(0x3, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0, P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
                    | P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
                    | P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
                    | P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1, P_Fld(0x3, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
                    | P_Fld(0x7, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED)
                    | P_Fld(0x5, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1)
                    | P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0, P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
                    | P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
                    | P_Fld(0x2, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQSIEN, P_Fld(0x19, SHURK0_DQSIEN_R0DQS1IEN)
                    | P_Fld(0x19, SHURK0_DQSIEN_R0DQS0IEN));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0, P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
                    | P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
                    | P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
                    | P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1, P_Fld(0x4, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
                    | P_Fld(0x4, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQSIEN, P_Fld(0x0, SHURK1_DQSIEN_R1DQS3IEN)
                    | P_Fld(0x0, SHURK1_DQSIEN_R1DQS2IEN)
                    | P_Fld(0x1b, SHURK1_DQSIEN_R1DQS1IEN)
                    | P_Fld(0x1b, SHURK1_DQSIEN_R1DQS0IEN));
    }
    //               41536 === over_write_setting_begin ===
    //               41536 === over_write_setting_end ===
    vIO32WriteFldAlign(DRAMC_REG_DRAMCTRL, 0x1, DRAMCTRL_PREALL_OPTION);
    vIO32WriteFldAlign(DRAMC_REG_ZQCS, 0x56, ZQCS_ZQCSOP);

    mcDELAY_US(1);

    vIO32WriteFldAlign(DRAMC_REG_SHU_CONF3, 0xff, SHU_CONF3_REFRCNT);
    vIO32WriteFldAlign(DRAMC_REG_REFCTRL0, 0x1, REFCTRL0_REFFRERUN);
    vIO32WriteFldAlign(DRAMC_REG_SREFCTRL, 0x1, SREFCTRL_SREF_HW_EN);
    vIO32WriteFldAlign(DRAMC_REG_MPC_OPTION, 0x1, MPC_OPTION_MPCRKEN);
    vIO32WriteFldAlign(DRAMC_REG_DRAMC_PD_CTRL, 0x1, DRAMC_PD_CTRL_PHYCLKDYNGEN);
    vIO32WriteFldAlign(DRAMC_REG_DRAMC_PD_CTRL, 0x1, DRAMC_PD_CTRL_DCMEN);
    vIO32WriteFldMulti(DRAMC_REG_EYESCAN, P_Fld(0x1, EYESCAN_RX_DQ_EYE_SEL)
                | P_Fld(0x0, EYESCAN_RG_RX_EYE_SCAN_EN));
    vIO32WriteFldMulti(DRAMC_REG_STBCAL1, P_Fld(0x1, STBCAL1_STBCNT_LATCH_EN)
                | P_Fld(0x1, STBCAL1_STBENCMPEN));
    vIO32WriteFldAlign(DRAMC_REG_TEST2_1, 0x10000, TEST2_1_TEST2_BASE);
#if (FOR_DV_SIMULATION_USED==1 || SW_CHANGE_FOR_SIMULATION==1)
    //because cmd_len=1 has bug with byte mode, so need to set cmd_len=0, then it will cost more time to do a pattern test
    //workaround: reduce TEST2_OFF to make less test agent cmd. make lpddr4-1600 can finish in 60us (Mengru)
    vIO32WriteFldAlign(DRAMC_REG_TEST2_2, 0x20, TEST2_2_TEST2_OFF);
#else
    vIO32WriteFldAlign(DRAMC_REG_TEST2_2, 0x400, TEST2_2_TEST2_OFF);
#endif
    vIO32WriteFldMulti(DRAMC_REG_TEST2_3, P_Fld(0x1, TEST2_3_TEST2WREN2_HW_EN)
                | P_Fld(0x4, TEST2_3_DQSICALSTP)
                | P_Fld(0x1, TEST2_3_TESTAUDPAT));
    vIO32WriteFldAlign(DRAMC_REG_RSTMASK, 0x0, RSTMASK_DAT_SYNC_MASK);
    vIO32WriteFldAlign(DRAMC_REG_RSTMASK, 0x0, RSTMASK_PHY_SYNC_MASK);

    mcDELAY_US(1);

    vIO32WriteFldMulti(DRAMC_REG_HW_MRR_FUN, P_Fld(0x0, HW_MRR_FUN_TRPMRR_EN)
                    | P_Fld(0x0, HW_MRR_FUN_TRCDMRR_EN) | P_Fld(0x0, HW_MRR_FUN_TMRR_ENA));
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_PERFCTL0, P_Fld(0x1, PERFCTL0_WRFIO_MODE2)
                    | P_Fld(0x0, PERFCTL0_RWSPLIT));
        vIO32WriteFldMulti(DRAMC_REG_PERFCTL0, P_Fld(0x1, PERFCTL0_REORDEREN)
                    | P_Fld(0x0, PERFCTL0_REORDER_MODE));
        vIO32WriteFldAlign(DRAMC_REG_RSTMASK, 0x0, RSTMASK_GT_SYNC_MASK);
        vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0x1, RKCFG_DQSOSC2RK);
        vIO32WriteFldAlign(DRAMC_REG_MRS, 0x0, MRS_MPCRK);
        vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x1, SPCMDCTRL_REFR_BLOCKEN);
        vIO32WriteFldAlign(DRAMC_REG_EYESCAN, 0x1, EYESCAN_RG_RX_MIOCK_JIT_EN);
    }
    else
    {
        vIO32WriteFldAlign(DRAMC_REG_DRAMCTRL, 0x0, DRAMCTRL_CTOREQ_HPRI_OPT);
        vIO32WriteFldMulti(DRAMC_REG_PERFCTL0, P_Fld(0x1, PERFCTL0_REORDEREN)
                    | P_Fld(0x0, PERFCTL0_REORDER_MODE));
        vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x1, SPCMDCTRL_REFR_BLOCKEN);
        vIO32WriteFldAlign(DRAMC_REG_RSTMASK, 0x0, RSTMASK_GT_SYNC_MASK);
        vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0x1, RKCFG_DQSOSC2RK);
        //vIO32WriteFldAlign(DRAMC_REG_MRS, 0x1, MRS_MPCRK);
        vIO32WriteFldAlign(DRAMC_REG_MRS, 0, MRS_MPCRK);
        vIO32WriteFldAlign(DRAMC_REG_MPC_OPTION, 1, MPC_OPTION_MPCRKEN);
        vIO32WriteFldAlign(DRAMC_REG_EYESCAN, 0x1, EYESCAN_RG_RX_MIOCK_JIT_EN);
        vIO32WriteFldAlign(DRAMC_REG_SHU1_WODT, 0x1, SHU1_WODT_DBIWR);
        vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ7, 0x1, SHU1_B0_DQ7_R_DMDQMDBI_SHU_B0);
        vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ7, 0x1, SHU1_B1_DQ7_R_DMDQMDBI_SHU_B1);
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0, P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
                    | P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
                    | P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
                    | P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0, P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
                    | P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
                    | P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
                    | P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
        vIO32WriteFldAlign(DRAMC_REG_SHU_RANKCTL, 0x4, SHU_RANKCTL_RANKINCTL);
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN0, P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN)
                    | P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN0, P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN)
                    | P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ1)
                    | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM1)
                    | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ1)
                    | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM1)
                    | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM0));
    }
    mcDELAY_US(5);

    vIO32WriteFldAlign(DRAMC_REG_STBCAL1, 0x3, STBCAL1_STBCAL_FILTER);
    vIO32WriteFldAlign(DRAMC_REG_STBCAL1, 0x1, STBCAL1_STBCAL_FILTER);
    vIO32WriteFldMulti(DRAMC_REG_STBCAL, P_Fld(0x1, STBCAL_STB_DQIEN_IG)
                | P_Fld(0x1, STBCAL_PICHGBLOCK_NORD)
                | P_Fld(0x0, STBCAL_STBCALEN)
                | P_Fld(0x0, STBCAL_STB_SELPHYCALEN)
                | P_Fld(0x1, STBCAL_PIMASK_RKCHG_OPT));
    vIO32WriteFldAlign(DRAMC_REG_STBCAL1, 0x1, STBCAL1_STB_SHIFT_DTCOUT_IG);
    vIO32WriteFldMulti(DRAMC_REG_SHU1_DQSG, P_Fld(0x9, SHU1_DQSG_STB_UPDMASKCYC)
                | P_Fld(0x1, SHU1_DQSG_STB_UPDMASK_EN));
    vIO32WriteFldAlign(DDRPHY_MISC_CTRL0, 0x0, MISC_CTRL0_R_DMDQSIEN_SYNCOPT);
    vIO32WriteFldAlign(DRAMC_REG_SHU_STBCAL, 0x1, SHU_STBCAL_DQSG_MODE);
    vIO32WriteFldAlign(DRAMC_REG_STBCAL, 0x1, STBCAL_SREF_DQSGUPD);
    //M17_Remap:vIO32WriteFldAlign(DDRPHY_MISC_CTRL1, 0x0, MISC_CTRL1_R_DMDQMDBI);
    /* RX Tracking DQM SM enable (actual values are set in DramcRxInputDelayTrackingHW()) */
#if ENABLE_RX_TRACKING_LP4
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ7, p->DBI_R_onoff[p->dram_fsp], SHU1_B0_DQ7_R_DMRXTRACK_DQM_EN_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ7, p->DBI_R_onoff[p->dram_fsp], SHU1_B1_DQ7_R_DMRXTRACK_DQM_EN_B1);
#else
    vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ7, 0, SHU1_B0_DQ7_R_DMRXTRACK_DQM_EN_B0);
    vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ7, 0, SHU1_B1_DQ7_R_DMRXTRACK_DQM_EN_B1);
#endif
    vIO32WriteFldMulti(DRAMC_REG_SHU_STBCAL, P_Fld(0x1, SHU_STBCAL_PICGLAT)
                | P_Fld(0x0, SHU_STBCAL_DMSTBLAT));
    vIO32WriteFldMulti(DRAMC_REG_REFCTRL1, P_Fld(0x1, REFCTRL1_REF_QUE_AUTOSAVE_EN)
                | P_Fld(0x1, REFCTRL1_SLEFREF_AUTOSAVE_EN));
    vIO32WriteFldMulti(DRAMC_REG_DQSOSCR, P_Fld(0x1, DQSOSCR_SREF_TXPI_RELOAD_OPT)
                | P_Fld(0x1, DQSOSCR_SREF_TXUI_RELOAD_OPT));
    vIO32WriteFldMulti(DRAMC_REG_RSTMASK, P_Fld(0x0, RSTMASK_DVFS_SYNC_MASK)
                | P_Fld(0x0, RSTMASK_GT_SYNC_MASK_FOR_PHY)
                | P_Fld(0x0, RSTMASK_DVFS_SYNC_MASK_FOR_PHY));
    vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0x1, RKCFG_RKMODE);

#if !APPLY_LP4_POWER_INIT_SEQUENCE
    vIO32WriteFldMulti(DRAMC_REG_CKECTRL, P_Fld(0x1, CKECTRL_CKEFIXON)
                | P_Fld(0x1, CKECTRL_CKE1FIXON));
#endif

    mcDELAY_US(12);

    ///TODO: DDR3200
    //if(p->frequency==1600)
    {
    //               60826 ===dramc_shu1_lp4_3200 begin===
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
        {
                vIO32WriteFldMulti(DRAMC_REG_SHU_RANKCTL, P_Fld(0x5, SHU_RANKCTL_RANKINCTL_PHY)
                            | P_Fld(0x3, SHU_RANKCTL_RANKINCTL_ROOT1)
                            | P_Fld(0x3, SHU_RANKCTL_RANKINCTL));
        }
        else
        {
                vIO32WriteFldMulti(DRAMC_REG_SHU_RANKCTL, P_Fld(0x6, SHU_RANKCTL_RANKINCTL_PHY)
                            | P_Fld(0x4, SHU_RANKCTL_RANKINCTL_ROOT1));
        }
        vIO32WriteFldMulti(DRAMC_REG_SHU1_DQSOSC_PRD, P_Fld(0xa, SHU1_DQSOSC_PRD_DQSOSCTHRD_DEC)
                    | P_Fld(0xa, SHU1_DQSOSC_PRD_DQSOSCTHRD_INC)
                    | P_Fld(0x10, SHU1_DQSOSC_PRD_DQSOSC_PRDCNT));
        vIO32WriteFldAlign(DRAMC_REG_SHU_DQSOSCR, 0x10, SHU_DQSOSCR_DQSOSCRCNT);
        vIO32WriteFldAlign(DRAMC_REG_SHU1_WODT, 0x0, SHU1_WODT_WPST2T);
        //vIO32WriteFldAlign(DRAMC_REG_SHU_HWSET_MR2, 0x2d, SHU_HWSET_MR2_HWSET_MR2_OP);
        if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
        {
                vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQSIEN, P_Fld(0x19, SHURK0_DQSIEN_R0DQS1IEN)
                            | P_Fld(0x19, SHURK0_DQSIEN_R0DQS0IEN));
        }
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_PI, P_Fld(0x1a, SHU1RK0_PI_RK0_ARPI_DQM_B1)
                    | P_Fld(0x1a, SHU1RK0_PI_RK0_ARPI_DQM_B0)
                    | P_Fld(0x1a, SHU1RK0_PI_RK0_ARPI_DQ_B1)
                    | P_Fld(0x1a, SHU1RK0_PI_RK0_ARPI_DQ_B0));
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQSOSC, P_Fld(0x168, SHU1RK0_DQSOSC_DQSOSC_BASE_RK0_B1)
                    | P_Fld(0x168, SHU1RK0_DQSOSC_DQSOSC_BASE_RK0));
        if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
        {
                vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN0, P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B3_RODTEN_P1)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B3_RODTEN)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B2_RODTEN_P1)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B2_RODTEN)
                            | P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN_P1)
                            | P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN)
                            | P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN_P1)
                            | P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN));
                vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN1, P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN_P1)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN_P1)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN_P1)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN_P1)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN));
                vIO32WriteFldAlign(DRAMC_REG_SHURK0_SELPH_DQSG0, 0x4, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1);
                vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(0x1, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
                            | P_Fld(0x5, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
                            | P_Fld(0x0, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
                            | P_Fld(0x4, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
        }
        else
        {
                vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN0, P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B3_RODTEN_P1)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B3_RODTEN)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B2_RODTEN_P1)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B2_RODTEN)
                            | P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN_P1)
                            | P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN_P1));
                vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN1, P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN_P1)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN_P1)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN_P1)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN_P1)
                            | P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN));
                vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(0x7, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
                            | P_Fld(0x3, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
                            | P_Fld(0x7, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
                            | P_Fld(0x3, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));

        }
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ2, P_Fld(0x4, SHURK0_SELPH_DQ2_DLY_OEN_DQ3)
                    | P_Fld(0x4, SHURK0_SELPH_DQ2_DLY_OEN_DQ2)
                    | P_Fld(0x4, SHURK0_SELPH_DQ2_DLY_OEN_DQ1)
                    | P_Fld(0x4, SHURK0_SELPH_DQ2_DLY_OEN_DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ3, P_Fld(0x4, SHURK0_SELPH_DQ3_DLY_OEN_DQM3)
                    | P_Fld(0x4, SHURK0_SELPH_DQ3_DLY_OEN_DQM2)
                    | P_Fld(0x4, SHURK0_SELPH_DQ3_DLY_OEN_DQM1)
                    | P_Fld(0x4, SHURK0_SELPH_DQ3_DLY_OEN_DQM0));
        if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
        {
                vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL1, P_Fld(0x3da, SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ1)
                            | P_Fld(0x3da, SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ0));
                vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL2, P_Fld(0x3da, SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ1)
                            | P_Fld(0x3da, SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ0));
        }
        else
        {
                vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL1, P_Fld(0x2da, SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ1)
                            | P_Fld(0x2da, SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ0));
                vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL2, P_Fld(0x2da, SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ1)
                            | P_Fld(0x2da, SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ0));
        }
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL3, P_Fld(0x1a, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1_B4TO0)
                    | P_Fld(0x1a, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0_B4TO0)
                    | P_Fld(0x1c, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1)
                    | P_Fld(0x1c, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL4, P_Fld(0x1a, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1_B4TO0)
                    | P_Fld(0x1a, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0_B4TO0)
                    | P_Fld(0x1c, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1)
                    | P_Fld(0x1c, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0));
        if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
        {
                vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL5, P_Fld(0x3da, SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM1)
                            | P_Fld(0x3da, SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM0));
                vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQSIEN, P_Fld(0x0, SHURK1_DQSIEN_R1DQS3IEN)
                            | P_Fld(0x0, SHURK1_DQSIEN_R1DQS2IEN)
                            | P_Fld(0x1b, SHURK1_DQSIEN_R1DQS1IEN)
                            | P_Fld(0x1b, SHURK1_DQSIEN_R1DQS0IEN));
        }
        else
        {
                vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL5, P_Fld(0x2da, SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM1)
                            | P_Fld(0x2da, SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM0));
        }
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_PI, P_Fld(0x14, SHU1RK1_PI_RK1_ARPI_DQM_B1)
                    | P_Fld(0x14, SHU1RK1_PI_RK1_ARPI_DQM_B0)
                    | P_Fld(0x14, SHU1RK1_PI_RK1_ARPI_DQ_B1)
                    | P_Fld(0x14, SHU1RK1_PI_RK1_ARPI_DQ_B0));
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQSOSC, P_Fld(0x127, SHU1RK1_DQSOSC_DQSOSC_BASE_RK1_B1)
                    | P_Fld(0x127, SHU1RK1_DQSOSC_DQSOSC_BASE_RK1));
        if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
        {
                vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN0, P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN_P1)
                            | P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN)
                            | P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN_P1)
                            | P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN));
        }
        else
        {
                vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN0, P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN_P1)
                            | P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN)
                            | P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN_P1)
                            | P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN)
                            | P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN_P1)
                            | P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN_P1));
        }
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN1, P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B3_R1RODTEN_P1)
                    | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B3_R1RODTEN)
                    | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B2_R1RODTEN_P1)
                    | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B2_R1RODTEN)
                    | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN_P1)
                    | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN)
                    | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN_P1)
                    | P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN));
        if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
        {
                vIO32WriteFldAlign(DRAMC_REG_SHURK1_SELPH_DQSG0, 0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED);
                vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1, P_Fld(0x4, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
                            | P_Fld(0x0, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED)
                            | P_Fld(0x6, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1)
                            | P_Fld(0x2, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED));
        }
        else
        {
                vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1, P_Fld(0x5, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
                            | P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED)
                            | P_Fld(0x5, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1)
                            | P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED));
        }
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ2, P_Fld(0x4, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ3)
                    | P_Fld(0x4, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ2)
                    | P_Fld(0x4, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ1)
                    | P_Fld(0x4, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ3, P_Fld(0x4, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM3)
                    | P_Fld(0x4, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM2)
                    | P_Fld(0x4, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM1)
                    | P_Fld(0x4, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM0));
        if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
        {
                vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL1, P_Fld(0x3d4, SHU1RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ1)
                            | P_Fld(0x3d4, SHU1RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ0));
                vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL2, P_Fld(0x3d4, SHU1RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ1)
                            | P_Fld(0x3d4, SHU1RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ0));
        }
        else
        {
                vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL1, P_Fld(0x2d4, SHU1RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ1)
                            | P_Fld(0x2d4, SHU1RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ0));
                vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL2, P_Fld(0x2d4, SHU1RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ1)
                            | P_Fld(0x2d4, SHU1RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ0));
        }
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL3, P_Fld(0x14, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ1_B4TO0)
                    | P_Fld(0x14, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ0_B4TO0)
                    | P_Fld(0x1c, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ1)
                    | P_Fld(0x1c, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL4, P_Fld(0x14, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM1_B4TO0)
                    | P_Fld(0x14, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM0_B4TO0)
                    | P_Fld(0x1c, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM1)
                    | P_Fld(0x1c, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM0));
        if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
        {
                vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL5, P_Fld(0x3d4, SHU1RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM1)
                            | P_Fld(0x3d4, SHU1RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM0));
        }
        else
        {
                vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL5, P_Fld(0x2d4, SHU1RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM1)
                            | P_Fld(0x2d4, SHU1RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM0));
        }
        vIO32WriteFldMulti(DRAMC_REG_SHU_DQSG_RETRY, P_Fld(0x1, SHU_DQSG_RETRY_R_RETRY_ROUND_NUM)
                    | P_Fld(0x0, SHU_DQSG_RETRY_R_XSR_RETRY_SPM_MODE)
                    | P_Fld(0x5, SHU_DQSG_RETRY_R_DQSIENLAT)
                    | P_Fld(0x1, SHU_DQSG_RETRY_R_DDR1866_PLUS));
        vIO32WriteFldMulti(DRAMC_REG_PRE_TDQSCK2, P_Fld(0x25, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO0)
                    | P_Fld(0x18, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO1)
                    | P_Fld(0x10, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO2)
                    | P_Fld(0x1b, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO3));
        vIO32WriteFldMulti(DRAMC_REG_PRE_TDQSCK3, P_Fld(0x14, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO4)
                    | P_Fld(0xd, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO5)
                    | P_Fld(0x2a, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO6)
                    | P_Fld(0x31, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO7));
        vIO32WriteFldMulti(DRAMC_REG_PRE_TDQSCK4, P_Fld(0x15, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO8)
                    | P_Fld(0x40, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO9)
                    | P_Fld(0x4a, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO10)
                    | P_Fld(0x30, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO11));
        vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK1, P_Fld(0x1a, RK0_PRE_TDQSCK1_TDQSCK_PIFREQ2_B0R0)
                    | P_Fld(0x1c, RK0_PRE_TDQSCK1_TDQSCK_UIFREQ2_B0R0)
                    | P_Fld(0x14, RK0_PRE_TDQSCK1_TDQSCK_UIFREQ1_B0R0));
        vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK2, P_Fld(0xb, RK0_PRE_TDQSCK2_TDQSCK_PIFREQ4_B0R0)
                    | P_Fld(0xd, RK0_PRE_TDQSCK2_TDQSCK_UIFREQ4_B0R0)
                    | P_Fld(0x10, RK0_PRE_TDQSCK2_TDQSCK_PIFREQ3_B0R0)
                    | P_Fld(0xa, RK0_PRE_TDQSCK2_TDQSCK_UIFREQ3_B0R0));
        vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK3, P_Fld(0x11, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ4_P1_B0R0)
                    | P_Fld(0xe, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ3_P1_B0R0)
                    | P_Fld(0x20, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ2_P1_B0R0)
                    | P_Fld(0x18, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ1_P1_B0R0));
        vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK4, P_Fld(0x1a, RK0_PRE_TDQSCK4_TDQSCK_PIFREQ2_B1R0)
                    | P_Fld(0x1c, RK0_PRE_TDQSCK4_TDQSCK_UIFREQ2_B1R0)
                    | P_Fld(0x14, RK0_PRE_TDQSCK4_TDQSCK_UIFREQ1_B1R0));
        vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK5, P_Fld(0xb, RK0_PRE_TDQSCK5_TDQSCK_PIFREQ4_B1R0)
                    | P_Fld(0xd, RK0_PRE_TDQSCK5_TDQSCK_UIFREQ4_B1R0)
                    | P_Fld(0x10, RK0_PRE_TDQSCK5_TDQSCK_PIFREQ3_B1R0)
                    | P_Fld(0xa, RK0_PRE_TDQSCK5_TDQSCK_UIFREQ3_B1R0));
        vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK6, P_Fld(0x11, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ4_P1_B1R0)
                    | P_Fld(0xe, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ3_P1_B1R0)
                    | P_Fld(0x20, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ2_P1_B1R0)
                    | P_Fld(0x18, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ1_P1_B1R0));
        vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK1, P_Fld(0xb, RK1_PRE_TDQSCK1_TDQSCK_PIFREQ2_B0R1)
                    | P_Fld(0x23, RK1_PRE_TDQSCK1_TDQSCK_UIFREQ2_B0R1)
                    | P_Fld(0xf, RK1_PRE_TDQSCK1_TDQSCK_PIFREQ1_B0R1)
                    | P_Fld(0x19, RK1_PRE_TDQSCK1_TDQSCK_UIFREQ1_B0R1));
        vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK2, P_Fld(0xe, RK1_PRE_TDQSCK2_TDQSCK_PIFREQ4_B0R1)
                    | P_Fld(0x10, RK1_PRE_TDQSCK2_TDQSCK_UIFREQ4_B0R1)
                    | P_Fld(0x1f, RK1_PRE_TDQSCK2_TDQSCK_PIFREQ3_B0R1)
                    | P_Fld(0xe, RK1_PRE_TDQSCK2_TDQSCK_UIFREQ3_B0R1));
        vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK3, P_Fld(0x14, RK1_PRE_TDQSCK3_TDQSCK_UIFREQ4_P1_B0R1)
                    | P_Fld(0x12, RK1_PRE_TDQSCK3_TDQSCK_UIFREQ3_P1_B0R1)
                    | P_Fld(0x27, RK1_PRE_TDQSCK3_TDQSCK_UIFREQ2_P1_B0R1)
                    | P_Fld(0x1d, RK1_PRE_TDQSCK3_TDQSCK_UIFREQ1_P1_B0R1));
        vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK4, P_Fld(0xb, RK1_PRE_TDQSCK4_TDQSCK_PIFREQ2_B1R1)
                    | P_Fld(0x23, RK1_PRE_TDQSCK4_TDQSCK_UIFREQ2_B1R1)
                    | P_Fld(0xf, RK1_PRE_TDQSCK4_TDQSCK_PIFREQ1_B1R1)
                    | P_Fld(0x19, RK1_PRE_TDQSCK4_TDQSCK_UIFREQ1_B1R1));
        vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK5, P_Fld(0xe, RK1_PRE_TDQSCK5_TDQSCK_PIFREQ4_B1R1)
                    | P_Fld(0x10, RK1_PRE_TDQSCK5_TDQSCK_UIFREQ4_B1R1)
                    | P_Fld(0x1f, RK1_PRE_TDQSCK5_TDQSCK_PIFREQ3_B1R1)
                    | P_Fld(0xe, RK1_PRE_TDQSCK5_TDQSCK_UIFREQ3_B1R1));
        vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK6, P_Fld(0x14, RK1_PRE_TDQSCK6_TDQSCK_UIFREQ4_P1_B1R1)
                    | P_Fld(0x12, RK1_PRE_TDQSCK6_TDQSCK_UIFREQ3_P1_B1R1)
                    | P_Fld(0x27, RK1_PRE_TDQSCK6_TDQSCK_UIFREQ2_P1_B1R1)
                    | P_Fld(0x1d, RK1_PRE_TDQSCK6_TDQSCK_UIFREQ1_P1_B1R1));
        //               61832 ===dramc_shu1_lp4_3200 end===


        //               66870 ===ddrphy_shu1_lp4_3200_CHA begin===
        if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
        {
                vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ7, 0x1, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0);
                vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ7, 0x1, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1);
        }
        else
        {
                vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7, P_Fld(0x1, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0)
                            | P_Fld(0x1, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0));
                vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ7, P_Fld(0x1, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1)
                            | P_Fld(0x1, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1));
//francis remove : it will make CLRPLL frequency wrong!
//francis remove                vIO32WriteFldMulti(DDRPHY_SHU1_PLL7, P_Fld(0x3d00, SHU1_PLL7_RG_RCLRPLL_SDM_PCW)
//francis remove                            | P_Fld(0x1, SHU1_PLL7_RG_RCLRPLL_SDM_PCW_CHG));
        }
        //               67761 ===ddrphy_shu1_lp4_3200_CHA end===

        //NOT included in parsing tool
        vIO32WriteFldAlign(DRAMC_REG_SHU_DQS2DQ_TX, 0x0, SHU_DQS2DQ_TX_OE2DQ_OFFSET);
    }
    ///TODO: DDR3733
    if(p->freqGroup == 1866)
    {
        DramcSetting_Olympus_LP4_ByteMode_DDR3733(p);
    }
    ///TODO: DDR2667
    else if (p->freqGroup == 1333 || p->freqGroup == 1200) // TODO: Initial settings for DDR2400?
    {
        DramcSetting_Olympus_LP4_ByteMode_DDR2667(p);
    }
    else // (p->freqGroup == 1600)
    {
        if(p->frequency==800)
        {
            DramcSetting_Olympus_LP4_ByteMode_DDR1600(p);
        }
        ///TODO: DDR1600
        if(p->frequency==400)
        {
            DramcSetting_Olympus_LP4_ByteMode_DDR800(p);
        }
    }

    UpdateInitialSettings_LP4(p);
#if SIMULATION_SW_IMPED
    if (p->dram_type == TYPE_LPDDR4)
        DramcSwImpedanceSaveRegister(p, ODT_ON, ODT_ON, DRAM_DFS_SHUFFLE_1);
    else if (p->dram_type == TYPE_LPDDR4X)
        DramcSwImpedanceSaveRegister(p, ODT_OFF, p->odt_onoff, DRAM_DFS_SHUFFLE_1);
    else if (p->dram_type == TYPE_LPDDR4P)
        DramcSwImpedanceSaveRegister(p, ODT_OFF, ODT_OFF, DRAM_DFS_SHUFFLE_1);
#endif

#ifndef LOOPBACK_TEST
    DDRPhyFreqMeter();
#endif

#if 0
    vIO32WriteFldMulti(DRAMC_REG_MRS, P_Fld(0x0, MRS_MRSRK)
                | P_Fld(0x4, MRS_MRSMA)
                | P_Fld(0x0, MRS_MRSOP));
    mcDELAY_US(1);
    vIO32WriteFldAlign(DRAMC_REG_SPCMD, 0x1, SPCMD_MRREN);
    vIO32WriteFldAlign(DRAMC_REG_SPCMD, 0x0, SPCMD_MRREN);
#endif

    vIO32WriteFldAlign(DRAMC_REG_TEST2_4, 0x0, TEST2_4_TEST_REQ_LEN1);

    vIO32WriteFldAlign(DRAMC_REG_SHU_CONF3, 0x5, SHU_CONF3_ZQCSCNT);

    mcDELAY_US(1);

#if !APPLY_LP4_POWER_INIT_SEQUENCE
    vIO32WriteFldMulti(DRAMC_REG_CKECTRL, P_Fld(0x0, CKECTRL_CKEFIXON)
                | P_Fld(0x0, CKECTRL_CKE1FIXON));
#endif
    vIO32WriteFldMulti(DRAMC_REG_REFCTRL0, P_Fld(0x1, REFCTRL0_PBREFEN)
                | P_Fld(0x1, REFCTRL0_PBREF_DISBYRATE));
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldAlign(DRAMC_REG_DQSOSCR, 0x1, DQSOSCR_RK0_BYTE_MODE);
        vIO32WriteFldAlign(DRAMC_REG_DQSOSCR, 0x1, DQSOSCR_RK1_BYTE_MODE);
    }
    vIO32WriteFldMulti(DRAMC_REG_SHUCTRL2, P_Fld(0x1, SHUCTRL2_MR13_SHU_EN)
                | P_Fld(0x1, SHUCTRL2_HWSET_WLRL));
    vIO32WriteFldAlign(DRAMC_REG_REFCTRL0, 0x1, REFCTRL0_REFDIS);
    //vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x0, SPCMDCTRL_REFRDIS);//Lewis@20160613: Fix refresh rate is wrong while diable MR4
    vIO32WriteFldMulti(DRAMC_REG_DRAMCTRL, P_Fld(0x1, DRAMCTRL_REQQUE_THD_EN)
                | P_Fld(0x1, DRAMCTRL_DPDRK_OPT));
    vIO32WriteFldMulti(DRAMC_REG_DUMMY_RD, P_Fld(0x1, DUMMY_RD_DQSG_DMYRD_EN)
                | P_Fld(p->support_rank_num, DUMMY_RD_RANK_NUM)
                | P_Fld(0x1, DUMMY_RD_DUMMY_RD_CNT6)
                | P_Fld(0x1, DUMMY_RD_DUMMY_RD_CNT5)
                | P_Fld(0x1, DUMMY_RD_DUMMY_RD_CNT3)
                | P_Fld(0x1, DUMMY_RD_DUMMY_RD_SW));
    vIO32WriteFldAlign(DRAMC_REG_TEST2_4, 0x4, TEST2_4_TESTAGENTRKSEL);
    vIO32WriteFldAlign(DRAMC_REG_DRAMCTRL, 0x0, DRAMCTRL_CTOREQ_HPRI_OPT);
    //               60759 === DE initial sequence done ===

///TODO: DVFS_Enable

    mcDELAY_US(1);

      //*((UINT32P)(DDRPHY0AO_BASE + 0x0000)) = 0x00000000;

    mcDELAY_US(1);

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);    
    //CH-A
    vIO32WriteFldMulti(DRAMC_REG_SHUCTRL, P_Fld(0x1, SHUCTRL_DVFS_CHB_SEL_B) | P_Fld(0x1, SHUCTRL_R_DRAMC_CHA) | P_Fld(0x0, SHUCTRL_SHU_PHYRST_SEL));
    vIO32WriteFldAlign(DRAMC_REG_SHUCTRL2, 0x1, SHUCTRL2_R_DVFS_DLL_CHA);
    //CH-B
    vIO32WriteFldMulti(DRAMC_REG_SHUCTRL+((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x1, SHUCTRL_DVFS_CHB_SEL_B) | P_Fld(0x0, SHUCTRL_R_DRAMC_CHA) | P_Fld(0x1, SHUCTRL_SHU_PHYRST_SEL));
    vIO32WriteFldAlign(DRAMC_REG_SHUCTRL2+((U32)CHANNEL_B<<POS_BANK_NUM), 0x0, SHUCTRL2_R_DVFS_DLL_CHA);
    //               60826 ===LP4_3200_intial_setting_shu1 end===


#if (fcFOR_CHIP_ID == fcBianco)
    mcSHOW_ERR_MSG(("IC_VERSION_E%d\n",u1GetICVersion_Bianco()+1));

    if(u1GetICVersion_Bianco()>=IC_VERSION_E3)
    {
        U8 u1CrossLatch;
        U8 u1Hysteresis;  

        u1Hysteresis = (p->frequency > 933) ? 0 : 3;
        vIO32WriteFldAlign_All(DDRPHY_SHU1_B0_DQ3, u1Hysteresis, SHU1_B0_DQ3_RG_TX_ARDQ_PU_PRE_B0);
        vIO32WriteFldAlign_All(DDRPHY_SHU1_B1_DQ3, u1Hysteresis, SHU1_B1_DQ3_RG_TX_ARDQ_PU_PRE_B1);
        ///vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_CMD3, u1Hysteresis, SHU1_CA_CMD3_RG_TX_ARCMD_PU_PRE);  //move to SW impedance

        u1CrossLatch = (p->frequency > 933) ? 3 : 0;
        vIO32WriteFldAlign_All(DDRPHY_SHU1_B0_DQ6, u1CrossLatch, SHU1_B0_DQ6_RG_ARPI_RESERVE_B0_BIT23);
        vIO32WriteFldAlign_All(DDRPHY_SHU1_B1_DQ6, u1CrossLatch, SHU1_B1_DQ6_RG_ARPI_RESERVE_B1_BIT23);
        vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_CMD6, u1CrossLatch, SHU1_CA_CMD6_RG_ARPI_RESERVE_CA_BIT23);//DDRPHY_SHU1_CA_CMD6 cannot be broadcast
    }
    else // for E1 and E2
    {
    #if PI_DUTY_IMPROVEMENT_TEST
        U8 u1CrossLatch;
        //vIO32WriteFldAlign(DDRPHY_B0_DLL_ARPI4, 0, B0_DLL_ARPI4_RG_ARPI_BYPASS_DQS_B0);
        //vIO32WriteFldAlign(DDRPHY_B1_DLL_ARPI4, 0, B1_DLL_ARPI4_RG_ARPI_BYPASS_DQS_B1);

        u1CrossLatch = (p->frequency >=1600) ? 1 : 0;
        vIO32WriteFldAlign_All(DDRPHY_SHU1_B0_DQ6, u1CrossLatch, SHU1_B0_DQ6_RG_ARPI_RESERVE_B0_BIT3);
        vIO32WriteFldAlign_All(DDRPHY_SHU1_B1_DQ6, u1CrossLatch, SHU1_B1_DQ6_RG_ARPI_RESERVE_B1_BIT3);
        vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_CMD6, u1CrossLatch, SHU1_CA_CMD6_RG_ARPI_RESERVE_CA_BIT3);  //DDRPHY_SHU1_CA_CMD6 cannot be broadcast
    #endif
    }
#endif


#if (ENABLE_CLOCK_DUTY_SCAN || ENABLE_DQS_DUTY_SCAN)
    DramcDutyCalibration_LPBK(p);
#endif

    DVFSSettings(p);

    DramcModeRegInit_LP4(p);

}

enum
{
    AC_TIMING_DRAM_TYPE=0,
    AC_TIMING_FREQUENCY,
    AC_TIMING_DBI_ONOFF,// Olympus new
    AC_TIMING_BYTE_MODE,// Olympus new

    AC_TIMING_TRAS,
    AC_TIMING_TRP,
    AC_TIMING_TRPAB,
    AC_TIMING_TRC,
    AC_TIMING_TRFC,
    AC_TIMING_TRFCPB,
    AC_TIMING_TXP,
    AC_TIMING_TRTP,
    AC_TIMING_TRCD,
    AC_TIMING_TWR,
    AC_TIMING_TWTR,
    AC_TIMING_TRRD,
    AC_TIMING_TFAW,
    AC_TIMING_TRTW_ODT_OFF,// Olympus new (External)
    AC_TIMING_TRTW_ODT_ON,// Olympus new (External)
    AC_TIMING_REFCNT, //(REFFRERUN = 0)
    AC_TIMING_REFCNT_FR_CLK, //(REFFRERUN = 1)
    AC_TIMING_TXREFCNT,
    AC_TIMING_TZQCS,
    // Olympus new (External)
    AC_TIMING_TRTPD,                // LP4/LP3
    AC_TIMING_TWTPD,                // LP4/LP3
    AC_TIMING_TMRR2W_ODT_OFF,  // LP4
    AC_TIMING_TMRR2W_ODT_ON,   // LP4
    //AC_TIMING_CKEPRD,               // LP3
    // End of Olympus new

    AC_TIMING_TRAS_05T,
    AC_TIMING_TRP_05T,
    AC_TIMING_TRPAB_05T,
    AC_TIMING_TRC_05T,
    AC_TIMING_TRFC_05T,
    AC_TIMING_TRFCPB_05T,
    AC_TIMING_TXP_05T,
    AC_TIMING_TRTP_05T,
    AC_TIMING_TRCD_05T,
    AC_TIMING_TWR_05T,
    AC_TIMING_TWTR_05T,
    AC_TIMING_TRRD_05T,
    AC_TIMING_TFAW_05T,
    AC_TIMING_TRTW_05T_ODT_OFF,
    AC_TIMING_TRTW_05T_ODT_ON,

     // Remove by Yulia, no 0.5T.
    //AC_TIMING_REFCNT_05T, //(REFFRERUN = 0)
    //AC_TIMING_REFCNT_FR_CLK_05T, //(REFFRERUN = 1)
    //AC_TIMING_TXREFCNT_05T,
    //AC_TIMING_TZQCS_05T,

    // Olympus new (External)
    AC_TIMING_TRTPD_05T,                // LP4/LP3
    AC_TIMING_TWTPD_05T,                // LP4/LP3
    //AC_TIMING_TMRR2W_05T_ODT_OFF,  // LP4 no 0.5T
    //AC_TIMING_TMRR2W_05T_ODT_ON,   // LP4  no 0.5T
    //AC_TIMING_CKEPRD,               // LP3
    // End of Olympus new

    AC_TIMING_XRTW2W,
    AC_TIMING_XRTW2R,
    AC_TIMING_XRTR2W,
    AC_TIMING_XRTR2R,

    AC_TIMING_DMCATRAIN_INTV,   // Olympus new, LP4 internal
    ////AC_TIMING_DMMRW_INTV,           // LP4 internal, move to initial setting
    ////AC_TIMING_DMFSPCHG_PRDCNT,  //LP4 internal, move to initial setting
    #if 0
    AC_TIMING_WRITE_LATENCY_0x41c,
    AC_TIMING_WRITE_LATENCY_0x420,
    AC_TIMING_WRITE_LATENCY_0x424,
    AC_TIMING_WRITE_LATENCY_0x428,
    AC_TIMING_WRITE_LATENCY_0x42c,
    #endif

    AC_TIMING_DQSINCTL_FOR_GATING,
    AC_TIMING_DATLAT,
    AC_TIMING_MODE_REG_WL,
    AC_TIMING_MODE_REG_RL,

    AC_TIMING_ITEM_NUM
};

#define TOTAL_AC_TIMING_NUMBER  (AC_TIMING_NUMBER_LP3 + AC_TIMING_NUMBER_LP4)

/*
 * DMCATRAIN_INTV: not a shuffle reg, set to highest freq's value (LP4_DDR3733)
 * WL, RL aren't currently used (MRW are set in ModeRegInit() for each freq)
 * For freq's not in ACTimingTable, use ACTimings as the next highest freq (See setFreqGroup())
 * DBI_ONOFF (Read DBI on/off), CKEPRD (CKE Pulse Width): Set in UpdateACTimingReg()
 * Since READ_DBI is enable/disabled using preprocessor C define
 * -> Save code size by excluding unneeded ACTimingTable entries
 * Note: Update AC_TIMING_NUMBER_LP3/4's enum if LP3/4 actimings are added/removed
 */
const U16 ACTimingTable[TOTAL_AC_TIMING_NUMBER][AC_TIMING_ITEM_NUM] =
{
    //======= LP4 =================================================================
    // LP4 ACTimingTable originated from "Alaska LP4 ACTiming excel file"
#if (ENABLE_READ_DBI == 1)
    //--------LP4 byte mode (DBI_ON) ---------------------------------------------
    // LP4-3733, 1866MHz, DBI_ON, byte mode
    {
        TYPE_LPDDR4, // Dram type
        1866 , // Dram frequency
        DBI_ON,
        CBT_BYTE_MODE1,

        //====  AC timing ====
#if (ACTIMING_TRAS_LOOSEN == 1)
        12,    // TRAS
#else
        11,    // TRAS
#endif
        6,    // TRP
        1,    // TPRAB
        20,    // TRC
        118,    // TFRC
        53,    // TRFCPB
        1,    // TXP
        2,    // TRTP
        7,    // TRCD
        15,    // TWR
        9,    // TWTR
        3,    // TRRD
        10,    // TFAW
        8,    // TRTW_ODT_OFF
        11,    // TRTW_ODT_ON
        113,    // REFCNT
        101,    // REFCNT_FR_CLK
        138,    // TXREFCNT
        40,    // TZQCS

        14,    // TRTPD
        14,    // TWTPD
        11,    // TMRR2W
        13,    // TMRR2W

        // ====  AC timing 0.5T ====
        1,    // TRAS_0.5T
        1,    // TRP_0.5T
        1,    // TRPAB_0.5T
        1,    // TRC_0.5T
        1,    // TRFC_0.5T
        1,    // TRFCPB_0.5T
        1,    // TXP_0.5T
        0,    // TRTP_0.5T
        1,    // TRCD_0.5T
        1,    // TWR_0.5T
        0,    // TWTR_0.5T
        0,    // TRRD_0.5T
        0,    // TFAW_0.5T
        0,    // TRTW_0.5T_ODT_OFF
        0,    // TRTW_0.5T_ODT_ON

        0,    // AC_TIMING_TRTPD_05T
        0,    // AC_TIMING_TWTPD_05T

        // ==== Cross rank setting ====
#ifdef XRTW2W_PERFORM_ENHANCE_TX
        5,    // XRTW2W
#else
        6,    // XRTW2W
#endif
        3,    // XRTW2R
        10,    // XRTR2W
#ifdef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
        9,    // XRTR2R
#else
        12,   // XRTR2R
#endif
        10,    // DMCATRAIN_INTV, no shuffle, set as ACTimingTable[0]'s value
        7,    // DQSINCTL
        18,    // DATLAT
        16,    // WL
        40    // RL
    },

    // LP4-3200, 1600MHz, DBI_ON, byte mode
    {
        TYPE_LPDDR4, // Dram type
        1600 , // Dram frequency
        DBI_ON,
        CBT_BYTE_MODE1,

        //====  AC timing ====
#if (ACTIMING_TRAS_LOOSEN == 1)
        9,    // TRAS
#else
        8,    // TRAS
#endif
        5,    // TRP
        1,    // TPRAB
        16,    // TRC
        100,    // TFRC
        44,    // TRFCPB
        1,    // TXP
        2,    // TRTP
        6,    // TRCD
        13,    // TWR
        8,    // TWTR
        2,    // TRRD
        7,    // TFAW
        7,    // TRTW_ODT_OFF
        10,    // TRTW_ODT_ON
        97,    // REFCNT
        101,    // REFCNT_FR_CLK
        119,    // TXREFCNT
        34,    // TZQCS

        13,    // TRTPD
        12,    // TWTPD
        10,    // TMRR2W
        12,    // TMRR2W

        // ====  AC timing 0.5T ====
        1,    // TRAS_0.5T
        1,    // TRP_0.5T
        0,    // TRPAB_0.5T
        1,    // TRC_0.5T
        0,    // TRFC_0.5T
        0,    // TRFCPB_0.5T
        0,    // TXP_0.5T
        1,    // TRTP_0.5T
        1,    // TRCD_0.5T
        1,    // TWR_0.5T
        0,    // TWTR_0.5T
        0,    // TRRD_0.5T
        0,    // TFAW_0.5T
        0,    // TRTW_0.5T_ODT_OFF
        0,    // TRTW_0.5T_ODT_ON

        0,    // AC_TIMING_TRTPD_05T
        0,    // AC_TIMING_TWTPD_05T

        // ==== Cross rank setting ====
#ifdef XRTW2W_PERFORM_ENHANCE_TX
        5,    // XRTW2W
#else
        6,    // XRTW2W
#endif
        3,    // XRTW2R
        9,    // XRTR2W
#ifdef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
        9,    // XRTR2R
#else
        12,   // XRTR2R
#endif

        8,    // DMCATRAIN_INTV, no shuffle, set as ACTimingTable[0]'s value
        5,    // DQSINCTL
        16,    // DATLAT
        14,    // WL
        36    // RL
    },

    // LP4-2667, 1333MHz, DBI_ON, byte mode
    {
        TYPE_LPDDR4, // Dram type
        1333 , // Dram frequency
        DBI_ON,
        CBT_BYTE_MODE1,

        //====  AC timing ====
#if (ACTIMING_TRAS_LOOSEN == 1)
        6,    // TRAS
#else
        5,    // TRAS
#endif
        4,    // TRP
        1,    // TPRAB
        12,    // TRC
        81,    // TFRC
        35,    // TRFCPB
        0,    // TXP
        1,    // TRTP
        5,    // TRCD
        11,    // TWR
        7,    // TWTR
        1,    // TRRD
        4,    // TFAW
        6,    // TRTW_ODT_OFF
        9,    // TRTW_ODT_ON
        81,    // REFCNT
        101,    // REFCNT_FR_CLK
        100,    // TXREFCNT
        28,    // TZQCS

        11,    // TRTPD
        11,    // TWTPD
        8,    // TMRR2W
        10,    // TMRR2W

        // ====  AC timing 0.5T ====
        1,    // TRAS_0.5T
        0,    // TRP_0.5T
        0,    // TRPAB_0.5T
        0,    // TRC_0.5T
        1,    // TRFC_0.5T
        0,    // TRFCPB_0.5T
        1,    // TXP_0.5T
        0,    // TRTP_0.5T
        0,    // TRCD_0.5T
        1,    // TWR_0.5T
        1,    // TWTR_0.5T
        1,    // TRRD_0.5T
        1,    // TFAW_0.5T
        0,    // TRTW_0.5T_ODT_OFF
        0,    // TRTW_0.5T_ODT_ON

        0,    // AC_TIMING_TRTPD_05T
        1,    // AC_TIMING_TWTPD_05T

        // ==== Cross rank setting ====
#ifdef XRTW2W_PERFORM_ENHANCE_TX
        5,    // XRTW2W
#else
        6,    // XRTW2W
#endif
        3,    // XRTW2R
        8,    // XRTR2W
#ifdef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
        9,    // XRTR2R
#else
        12,   // XRTR2R
#endif

        7,    // DMCATRAIN_INTV, no shuffle, set as ACTimingTable[0]'s value
        4,    // DQSINCTL
        15,    // DATLAT
        12,    // WL
        30    // RL
    },
#endif // ENABLE_READ_DBI == 1
    //--------LP4 byte mode (DBI_OFF) ---------------------------------------------
#if (ENABLE_READ_DBI == 0)
    // LP4-3733, 1866MHz, DBI_OFF, byte mode
    {
        TYPE_LPDDR4, // Dram type
        1866, // Dram frequency
        DBI_OFF,
        CBT_BYTE_MODE1,

        //====  AC timing ====
#if (ACTIMING_TRAS_LOOSEN == 1)
        12,    // TRAS
#else
        11,    // TRAS
#endif
        6,    // TRP
        1,    // TPRAB
        20,    // TRC
        118,    // TFRC
        53,    // TRFCPB
        1,    // TXP
        2,    // TRTP
        7,    // TRCD
        15,    // TWR
        9,    // TWTR
        3,    // TRRD
        10,    // TFAW
        7,    // TRTW_ODT_OFF
        10,    // TRTW_ODT_ON
        113,    // REFCNT
        101,    // REFCNT_FR_CLK
        138,    // TXREFCNT
        40,    // TZQCS

        13,    // TRTPD
        14,    // TWTPD
        10,    // TMRR2W
        12,    // TMRR2W

        // ====  AC timing 0.5T ====
        1,    // TRAS_0.5T
        1,    // TRP_0.5T
        1,    // TRPAB_0.5T
        1,    // TRC_0.5T
        1,    // TRFC_0.5T
        1,    // TRFCPB_0.5T
        1,    // TXP_0.5T
        0,    // TRTP_0.5T
        1,    // TRCD_0.5T
        1,    // TWR_0.5T
        0,    // TWTR_0.5T
        0,    // TRRD_0.5T
        0,    // TFAW_0.5T
        0,    // TRTW_0.5T_ODT_OFF
        0,    // TRTW_0.5T_ODT_ON

        0,    // AC_TIMING_TRTPD_05T
        0,    // AC_TIMING_TWTPD_05T

        // ==== Cross rank setting ====
#ifdef XRTW2W_PERFORM_ENHANCE_TX
        5,    // XRTW2W
#else
        6,    // XRTW2W
#endif
        3,    // XRTW2R
        9,    // XRTR2W
#ifdef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
        9,    // XRTR2R
#else
        12,   // XRTR2R
#endif

        10,    // DMCATRAIN_INTV, no shuffle, set as ACTimingTable[0]'s value
        7,    // DQSINCTL
        17,    // DATLAT
        16,    // WL
        36    // RL
    },

    // LP4-3200, 1600MHz, DBI_OFF, byte mode
    {
        TYPE_LPDDR4, // Dram type
        1600, // Dram frequency
        DBI_OFF,
        CBT_BYTE_MODE1,

        //====  AC timing ====
#if (ACTIMING_TRAS_LOOSEN == 1)
        9,    // TRAS
#else
        8,    // TRAS
#endif
        5,    // TRP
        1,    // TPRAB
        16,    // TRC
        100,    // TFRC
        44,    // TRFCPB
        1,    // TXP
        2,    // TRTP
        6,    // TRCD
        13,    // TWR
        8,    // TWTR
        2,    // TRRD
        7,    // TFAW
        6,    // TRTW_ODT_OFF
        9,    // TRTW_ODT_ON
        97,    // REFCNT
        101,    // REFCNT_FR_CLK
        119,    // TXREFCNT
        34,    // TZQCS

        12,    // TRTPD
        12,    // TWTPD
        9,    // TMRR2W
        11,    // TMRR2W

        // ====  AC timing 0.5T ====
        1,    // TRAS_0.5T
        1,    // TRP_0.5T
        0,    // TRPAB_0.5T
        1,    // TRC_0.5T
        0,    // TRFC_0.5T
        0,    // TRFCPB_0.5T
        0,    // TXP_0.5T
        1,    // TRTP_0.5T
        1,    // TRCD_0.5T
        1,    // TWR_0.5T
        0,    // TWTR_0.5T
        0,    // TRRD_0.5T
        0,    // TFAW_0.5T
        0,    // TRTW_0.5T_ODT_OFF
        0,    // TRTW_0.5T_ODT_ON

        0,    // AC_TIMING_TRTPD_05T
        0,    // AC_TIMING_TWTPD_05T

        // ==== Cross rank setting ====
#ifdef XRTW2W_PERFORM_ENHANCE_TX
        5,    // XRTW2W
#else
        6,    // XRTW2W
#endif
        3,    // XRTW2R
        8,    // XRTR2W
#ifdef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
        9,    // XRTR2R
#else
        12,   // XRTR2R
#endif

        8,    // DMCATRAIN_INTV, no shuffle, set as ACTimingTable[0]'s value
        5,    // DQSINCTL
        16,    // DATLAT
        14,    // WL
        32,    // RL
    },

    // LP4-2667, 1333MHz, DBI_OFF, byte mode
    {
        TYPE_LPDDR4, // Dram type
        1333 , // Dram frequency
        DBI_OFF,       // LP4-2667 byte mode can apply to both DBI_ON and DBI_Off
        CBT_BYTE_MODE1,

        //====  AC timing ====
#if (ACTIMING_TRAS_LOOSEN == 1)
        6,    // TRAS
#else
        5,    // TRAS
#endif
        4,    // TRP
        1,    // TPRAB
        12,    // TRC
        81,    // TFRC
        35,    // TRFCPB
        0,    // TXP
        1,    // TRTP
        5,    // TRCD
        11,    // TWR
        7,    // TWTR
        1,    // TRRD
        4,    // TFAW
        5,    // TRTW_ODT_OFF
        8,    // TRTW_ODT_ON
        81,    // REFCNT
        101,    // REFCNT_FR_CLK
        100,    // TXREFCNT
        28,    // TZQCS

        10,    // TRTPD
        11,    // TWTPD
        7,    // TMRR2W
        9,    // TMRR2W

        // ====  AC timing 0.5T ====
        1,    // TRAS_0.5T
        0,    // TRP_0.5T
        0,    // TRPAB_0.5T
        0,    // TRC_0.5T
        1,    // TRFC_0.5T
        0,    // TRFCPB_0.5T
        1,    // TXP_0.5T
        0,    // TRTP_0.5T
        0,    // TRCD_0.5T
        1,    // TWR_0.5T
        1,    // TWTR_0.5T
        1,    // TRRD_0.5T
        1,    // TFAW_0.5T
        0,    // TRTW_0.5T_ODT_OFF
        0,    // TRTW_0.5T_ODT_ON

        0,    // AC_TIMING_TRTPD_05T
        1,    // AC_TIMING_TWTPD_05T

        // ==== Cross rank setting ====
#ifdef XRTW2W_PERFORM_ENHANCE_TX
        5,    // XRTW2W
#else
        6,    // XRTW2W
#endif
        3,    // XRTW2R
        7,    // XRTR2W
#ifdef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
        9,    // XRTR2R
#else
        12,   // XRTR2R
#endif

        7,    // DMCATRAIN_INTV, no shuffle, set as ACTimingTable[0]'s value
        4,    // DQSINCTL
        14,    // DATLAT
        12,    // WL
        26    // RL
    },
#endif // ENABLE_READ_DBI == 0

    // LP4-2400, 1200MHz, DBI_OFF, byte mode
    {
        TYPE_LPDDR4, // Dram type
        1200 , // Dram frequency
        DBI_OFF,       // LP4-2667 byte mode can apply to both DBI_ON and DBI_Off
        CBT_BYTE_MODE1,

        //====  AC timing ====
#if (ACTIMING_TRAS_LOOSEN == 1)
        5,    // TRAS
#else
        4,    // TRAS
#endif
        3,    // TRP
        1,    // TPRAB
        10,    // TRC
        72,    // TFRC
        30,    // TRFCPB
        0,    // TXP
        1,    // TRTP
        4,    // TRCD
        10,    // TWR
        6,    // TWTR
        1,    // TRRD
        3,    // TFAW
        5,    // TRTW_ODT_OFF
        8,    // TRTW_ODT_ON
        73,    // REFCNT
        101,    // REFCNT_FR_CLK
        91,    // TXREFCNT
        25,    // TZQCS

        10,    // TRTPD
        10,    // TWTPD
        7,    // TMRR2W
        9,    // TMRR2W

        // ====  AC timing 0.5T ====
        1,    // TRAS_0.5T
        1,    // TRP_0.5T
        0,    // TRPAB_0.5T
        0,    // TRC_0.5T
        0,    // TRFC_0.5T
        0,    // TRFCPB_0.5T
        1,    // TXP_0.5T
        0,    // TRTP_0.5T
        1,    // TRCD_0.5T
        0,    // TWR_0.5T
        0,    // TWTR_0.5T
        0,    // TRRD_0.5T
        0,    // TFAW_0.5T
        0,    // TRTW_0.5T_ODT_OFF
        0,    // TRTW_0.5T_ODT_ON

        0,    // AC_TIMING_TRTPD_05T
        1,    // AC_TIMING_TWTPD_05T

        // ==== Cross rank setting ====
#ifdef XRTW2W_PERFORM_ENHANCE_TX
        5,    // XRTW2W
#else
        6,    // XRTW2W
#endif
        3,    // XRTW2R
        7,    // XRTR2W
#ifdef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
        8,    // XRTR2R
#else
        12,   // XRTR2R
#endif

        6,    // DMCATRAIN_INTV, no shuffle, set as ACTimingTable[0]'s value
        4,    // DQSINCTL //TODO
        14,    // DATLAT  //TODO
        12,    // WL
        26    // RL
    },

    // LP4-1600, 800MHz, DBI_OFF, byte mode
    {
        TYPE_LPDDR4, // Dram type
        800 , // Dram frequency
        DBI_OFF,       // LP4-1600 byte mode can apply to both DBI_ON and DBI_Off
        CBT_BYTE_MODE1,

        //====  AC timing ====
#if (ACTIMING_TRAS_LOOSEN == 1)
        1,    // TRAS
#else
        0,    // TRAS
#endif
        2,    // TRP
        0,    // TPRAB
        4,    // TRC
        44,    // TFRC
        16,    // TRFCPB
        0,    // TXP
        1,    // TRTP
        3,    // TRCD
        7,    // TWR
        4,    // TWTR
        0,    // TRRD
        0,    // TFAW
        3,    // TRTW_ODT_OFF
        5,    // TRTW_ODT_ON
        48,    // REFCNT
        101,    // REFCNT_FR_CLK
        62,    // TXREFCNT
        16,    // TZQCS

        7,    // TRTPD
        7,    // TWTPD
        4,    // TMRR2W
        6,    // TMRR2W

        // ====  AC timing 0.5T ====
        0,    // TRAS_0.5T
        0,    // TRP_0.5T
        1,    // TRPAB_0.5T
        0,    // TRC_0.5T
        0,    // TRFC_0.5T
        0,    // TRFCPB_0.5T
        0,    // TXP_0.5T
        1,    // TRTP_0.5T
        0,    // TRCD_0.5T
        0,    // TWR_0.5T
        0,    // TWTR_0.5T
        0,    // TRRD_0.5T
        0,    // TFAW_0.5T
        0,    // TRTW_0.5T_ODT_OFF
        0,    // TRTW_0.5T_ODT_ON

        0,    // AC_TIMING_TRTPD_05T
        1,    // AC_TIMING_TWTPD_05T

        // ==== Cross rank setting ====
#ifdef XRTW2W_PERFORM_ENHANCE_TX
        5,    // XRTW2W
#else
        6,    // XRTW2W
#endif
        3,    // XRTW2R
        4,    // XRTR2W
#ifdef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
        8,    // XRTR2R
#else
        12,   // XRTR2R
#endif

        4,    // DMCATRAIN_INTV, no shuffle, set as ACTimingTable[0]'s value
        2,    // DQSINCTL
        11,    // DATLAT = normal mode +1
        8,    // WL
        16    // RL
    },

    //--------LP4 normal mode (DBI_ON) ----------------------------------
#if (ENABLE_READ_DBI == 1)
    // LP4-3733, 1866MHz, DBI_ON, normal mode
    {
        TYPE_LPDDR4, // Dram type
        1866 , // Dram frequency
        DBI_ON,
        CBT_NORMAL_MODE,

        //====  AC timing ====
#if (ACTIMING_TRAS_LOOSEN == 1)
        12,    // TRAS
#else
        11,    // TRAS
#endif
        6,    // TRP
        1,    // TPRAB
        20,    // TRC
        118,    // TFRC
        53,    // TRFCPB
        1,    // TXP
        2,    // TRTP
        7,    // TRCD
        14,    // TWR
        8,    // TWTR
        3,    // TRRD
        10,    // TFAW
        7,    // TRTW_ODT_OFF
        10,    // TRTW_ODT_ON
        113,    // REFCNT
        101,    // REFCNT_FR_CLK
        138,    // TXREFCNT
        40,    // TZQCS

        13,    // TRTPD
        13,    // TWTPD
        10,    // TMRR2W
        12,    // TMRR2W

        // ====  AC timing 0.5T ====
        1,    // TRAS_0.5T
        1,    // TRP_0.5T
        1,    // TRPAB_0.5T
        1,    // TRC_0.5T
        1,    // TRFC_0.5T
        1,    // TRFCPB_0.5T
        1,    // TXP_0.5T
        0,    // TRTP_0.5T
        1,    // TRCD_0.5T
        1,    // TWR_0.5T
        0,    // TWTR_0.5T
        0,    // TRRD_0.5T
        0,    // TFAW_0.5T
        0,    // TRTW_0.5T_ODT_OFF
        0,    // TRTW_0.5T_ODT_ON

        0,    // AC_TIMING_TRTPD_05T
        0,    // AC_TIMING_TWTPD_05T

        // ==== Cross rank setting ====
#ifdef XRTW2W_PERFORM_ENHANCE_TX
        5,    // XRTW2W
#else
        6,    // XRTW2W
#endif
        3,    // XRTW2R
        9,    // XRTR2W
#ifdef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
        9,    // XRTR2R
#else
        12,   // XRTR2R
#endif

        10,    // DMCATRAIN_INTV, no shuffle, set as ACTimingTable[0]'s value
        6,    // DQSINCTL
        18,    //13, //17, //21, //18, //19 //15 , //Datlat
        16,    // WL
        36    // RL
    },

    // LP4-3200, 1600MHz, DBI_ON, normal mode
    {
        TYPE_LPDDR4, // Dram type
        1600 , // Dram frequency
        DBI_ON,
        CBT_NORMAL_MODE,

        //====  AC timing ====
#if (ACTIMING_TRAS_LOOSEN == 1)
        9,    // TRAS
#else
        8,    // TRAS
#endif
        5,    // TRP
        1,    // TPRAB
        16,    // TRC
        100,    // TFRC
        44,    // TRFCPB
        1,    // TXP
        2,    // TRTP
        6,    // TRCD
        12,    // TWR
        7,    // TWTR
        2,    // TRRD
        7,    // TFAW
        6,    // TRTW_ODT_OFF
        9,    // TRTW_ODT_ON
        97,    // REFCNT
        101,    // REFCNT_FR_CLK
        119,    // TXREFCNT
        34,    // TZQCS

        12,    // TRTPD
        12,    // TWTPD
        9,    // TMRR2W
        11,    // TMRR2W

        // ====  AC timing 0.5T ====
        1,    // TRAS_0.5T
        1,    // TRP_0.5T
        0,    // TRPAB_0.5T
        1,    // TRC_0.5T
        0,    // TRFC_0.5T
        0,    // TRFCPB_0.5T
        0,    // TXP_0.5T
        1,    // TRTP_0.5T
        1,    // TRCD_0.5T
        1,    // TWR_0.5T
        0,    // TWTR_0.5T
        0,    // TRRD_0.5T
        0,    // TFAW_0.5T
        0,    // TRTW_0.5T_ODT_OFF
        0,    // TRTW_0.5T_ODT_ON

        0,    // AC_TIMING_TRTPD_05T
        1,    // AC_TIMING_TWTPD_05T

        // ==== Cross rank setting ====
#ifdef XRTW2W_PERFORM_ENHANCE_TX
        5,    // XRTW2W
#else
        6,    // XRTW2W
#endif
        3,    // XRTW2R
        8,    // XRTR2W
#ifdef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
        9,    // XRTR2R
#else
        12,   // XRTR2R
#endif

        8,    // DMCATRAIN_INTV, no shuffle, set as ACTimingTable[0]'s value
        4,    //DQSINCTL = 4 for DBI OFF
        15,    // DATLAT
        14,    // WL
        32    // RL
    },

    // LP4-2667, 1333MHz, DBI_ON, normal mode
    {
        TYPE_LPDDR4, // Dram type
        1333 , // Dram frequency
        DBI_ON,
        CBT_NORMAL_MODE,

        //====  AC timing ====
#if (ACTIMING_TRAS_LOOSEN == 1)
        6,    // TRAS
#else
        5,    // TRAS
#endif
        4,    // TRP
        1,    // TPRAB
        12,    // TRC
        81,    // TFRC
        35,    // TRFCPB
        0,    // TXP
        1,    // TRTP
        5,    // TRCD
        10,    // TWR
        6,    // TWTR
        1,    // TRRD
        4,    // TFAW
        5,    // TRTW_ODT_OFF
        8,    // TRTW_ODT_ON
        81,    // REFCNT
        101,    // REFCNT_FR_CLK
        100,    // TXREFCNT
        28,    // TZQCS

        10,    // TRTPD
        10,    // TWTPD
        7,    // TMRR2W
        9,    // TMRR2W

        // ====  AC timing 0.5T ====
        1,    // TRAS_0.5T
        0,    // TRP_0.5T
        0,    // TRPAB_0.5T
        0,    // TRC_0.5T
        1,    // TRFC_0.5T
        0,    // TRFCPB_0.5T
        1,    // TXP_0.5T
        0,    // TRTP_0.5T
        0,    // TRCD_0.5T
        0,    // TWR_0.5T
        0,    // TWTR_0.5T
        1,    // TRRD_0.5T
        1,    // TFAW_0.5T
        0,    // TRTW_0.5T_ODT_OFF
        0,    // TRTW_0.5T_ODT_ON

        0,    // AC_TIMING_TRTPD_05T
        1,    // AC_TIMING_TWTPD_05T

        // ==== Cross rank setting ====
#ifdef XRTW2W_PERFORM_ENHANCE_TX
        5,    // XRTW2W
#else
        6,    // XRTW2W
#endif
        3,    // XRTW2R
        7,    // XRTR2W
#ifdef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
        9,    // XRTR2R
#else
        12,   // XRTR2R
#endif

        7,    // DMCATRAIN_INTV, no shuffle, set as ACTimingTable[0]'s value
        3,    // DQSINCTL = 3 for DBI OFF
        13,    // DATLAT
        12,    // WL
        28    // RL
    },
#endif // ENABLE_READ_DBI == 1
    //--------LP4 normal mode (DBI_OFF) ---------------------------------
#if (ENABLE_READ_DBI == 0)
    // LP4-3733, 1866MHz, DBI_OFF, normal mode
    {
        TYPE_LPDDR4, // Dram type
        1866 , // Dram frequency
        DBI_OFF,
        CBT_NORMAL_MODE,

        //====  AC timing ====
#if (ACTIMING_TRAS_LOOSEN == 1)
        12,    // TRAS
#else
        11,    // TRAS
#endif
        6,    // TRP
        1,    // TPRAB
        20,    // TRC
        118,    // TFRC
        53,    // TRFCPB
        1,    // TXP
        2,    // TRTP
        7,    // TRCD
        14,    // TWR
        8,    // TWTR
        3,    // TRRD
        10,    // TFAW
        6,    // TRTW_ODT_OFF
        9,    // TRTW_ODT_ON
        113,    // REFCNT
        101,    // REFCNT_FR_CLK
        138,    // TXREFCNT
        40,    // TZQCS

        12,    // TRTPD
        13,    // TWTPD
        9,    // TMRR2W
        11,    // TMRR2W
        
        // ====  AC timing 0.5T ====
        1,    // TRAS_0.5T
        1,    // TRP_0.5T
        1,    // TRPAB_0.5T
        1,    // TRC_0.5T
        1,    // TRFC_0.5T
        1,    // TRFCPB_0.5T
        1,    // TXP_0.5T
        0,    // TRTP_0.5T
        1,    // TRCD_0.5T
        1,    // TWR_0.5T
        0,    // TWTR_0.5T
        0,    // TRRD_0.5T
        0,    // TFAW_0.5T
        0,    // TRTW_0.5T_ODT_OFF
        0,    // TRTW_0.5T_ODT_ON

        0,    // AC_TIMING_TRTPD_05T
        0,    // AC_TIMING_TWTPD_05T

        // ==== Cross rank setting ====
#ifdef XRTW2W_PERFORM_ENHANCE_TX
        5,    // XRTW2W
#else
        6,    // XRTW2W
#endif
        3,    // XRTW2R
        8,    // XRTR2W
#ifdef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
        9,    // XRTR2R
#else
        12,   // XRTR2R
#endif

        10,    // DMCATRAIN_INTV, no shuffle, set as ACTimingTable[0]'s value
        4,    // DQSINCTL
        18,    //13, //17, //21, //18, //19 //15 , //Datlat
        16,    // WL
        32    // RL
    },

    // LP4-3200, 1600MHz, DBI_OFF, normal mode
    {
        TYPE_LPDDR4, // Dram type
        1600 , // Dram frequency
        DBI_OFF,
        CBT_NORMAL_MODE,

        //====  AC timing ====
#if (ACTIMING_TRAS_LOOSEN == 1)
        9,    // TRAS
#else
        8,    // TRAS
#endif
        5,    // TRP
        1,    // TPRAB
        16,    // TRC
        100,    // TFRC
        44,    // TRFCPB
        1,    // TXP
        2,    // TRTP
        6,    // TRCD
        12,    // TWR
        7,    // TWTR
        2,    // TRRD
        7,    // TFAW
        5,    // TRTW_ODT_OFF
        8,    // TRTW_ODT_ON
        97,    // REFCNT
        101,    // REFCNT_FR_CLK
        119,    // TXREFCNT
        34,    // TZQCS

        11,    // TRTPD
        12,    // TWTPD
        8,    // TMRR2W
        10,    // TMRR2W

        // ====  AC timing 0.5T ====
        1,    // TRAS_0.5T
        1,    // TRP_0.5T
        0,    // TRPAB_0.5T
        1,    // TRC_0.5T
        0,    // TRFC_0.5T
        0,    // TRFCPB_0.5T
        0,    // TXP_0.5T
        1,    // TRTP_0.5T
        1,    // TRCD_0.5T
        1,    // TWR_0.5T
        0,    // TWTR_0.5T
        0,    // TRRD_0.5T
        0,    // TFAW_0.5T
        0,    // TRTW_0.5T_ODT_OFF
        0,    // TRTW_0.5T_ODT_ON

        0,    // AC_TIMING_TRTPD_05T
        1,    // AC_TIMING_TWTPD_05T

        // ==== Cross rank setting ====
#ifdef XRTW2W_PERFORM_ENHANCE_TX
        5,    // XRTW2W
#else
        6,    // XRTW2W
#endif
        3,    // XRTW2R
        7,    // XRTR2W
#ifdef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
        9,    // XRTR2R
#else
        12,   // XRTR2R
#endif

        8,    // DMCATRAIN_INTV, no shuffle, set as ACTimingTable[0]'s value
        4,    // DQSINCTL
        15,    // DATLAT
        14,    // WL
        28    // RL
    },

    // LP4-2667, 1333MHz, DBI_OFF, normal mode
    {
        TYPE_LPDDR4, // Dram type
        1333 , // Dram frequency
        DBI_OFF,
        CBT_NORMAL_MODE,

        //====  AC timing ====
#if (ACTIMING_TRAS_LOOSEN == 1)
        6,    // TRAS
#else
        5,    // TRAS
#endif
        4,    // TRP
        1,    // TPRAB
        12,    // TRC
        81,    // TFRC
        35,    // TRFCPB
        0,    // TXP
        1,    // TRTP
        5,    // TRCD
        10,    // TWR
        6,    // TWTR
        1,    // TRRD
        4,    // TFAW
        4,    // TRTW_ODT_OFF
        7,    // TRTW_ODT_ON
        81,    // REFCNT
        101,    // REFCNT_FR_CLK
        100,    // TXREFCNT
        28,    // TZQCS

        9,    // TRTPD
        10,    // TWTPD
        6,    // TMRR2W
        8,    // TMRR2W

        // ====  AC timing 0.5T ====
        1,    // TRAS_0.5T
        0,    // TRP_0.5T
        0,    // TRPAB_0.5T
        0,    // TRC_0.5T
        1,    // TRFC_0.5T
        0,    // TRFCPB_0.5T
        1,    // TXP_0.5T
        0,    // TRTP_0.5T
        0,    // TRCD_0.5T
        0,    // TWR_0.5T
        0,    // TWTR_0.5T
        1,    // TRRD_0.5T
        1,    // TFAW_0.5T
        0,    // TRTW_0.5T_ODT_OFF
        0,    // TRTW_0.5T_ODT_ON

        0,    // AC_TIMING_TRTPD_05T
        1,    // AC_TIMING_TWTPD_05T

        // ==== Cross rank setting ====
#ifdef XRTW2W_PERFORM_ENHANCE_TX
        5,    // XRTW2W
#else
        6,    // XRTW2W
#endif
        3,    // XRTW2R
        6,    // XRTR2W
#ifdef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
        9,    // XRTR2R
#else
        12,   // XRTR2R
#endif

        7,    // DMCATRAIN_INTV, no shuffle, set as ACTimingTable[0]'s value
        3,    // DQSINCTL
        13,    // DATLAT
        12,    // WL
        24    // RL
    },
#endif // ENABLE_READ_DBI == 0
    // LP4-2400, 1200MHz, DBI_OFF, normal mode
    {
        TYPE_LPDDR4, // Dram type
        1200 , // Dram frequency
        DBI_OFF,
        CBT_NORMAL_MODE,

        //====  AC timing ====
#if (ACTIMING_TRAS_LOOSEN == 1)
        5,    // TRAS
#else
        4,    // TRAS
#endif
        3,    // TRP
        1,    // TPRAB
        10,    // TRC
        72,    // TFRC
        30,    // TRFCPB
        0,    // TXP
        1,    // TRTP
        4,    // TRCD
        10,    // TWR
        6,    // TWTR
        1,    // TRRD
        3,    // TFAW
        4,    // TRTW_ODT_OFF
        7,    // TRTW_ODT_ON
        73,    // REFCNT
        101,    // REFCNT_FR_CLK
        91,    // TXREFCNT
        25,    // TZQCS

        9,    // TRTPD
        9,    // TWTPD
        6,    // TMRR2W
        8,    // TMRR2W

        // ====  AC timing 0.5T ====
        1,    // TRAS_0.5T
        1,    // TRP_0.5T
        0,    // TRPAB_0.5T
        0,    // TRC_0.5T
        0,    // TRFC_0.5T
        0,    // TRFCPB_0.5T
        1,    // TXP_0.5T
        0,    // TRTP_0.5T
        1,    // TRCD_0.5T
        1,    // TWR_0.5T
        1,    // TWTR_0.5T
        0,    // TRRD_0.5T
        0,    // TFAW_0.5T
        0,    // TRTW_0.5T_ODT_OFF
        0,    // TRTW_0.5T_ODT_ON

        0,    // AC_TIMING_TRTPD_05T
        0,    // AC_TIMING_TWTPD_05T

        // ==== Cross rank setting ====
#ifdef XRTW2W_PERFORM_ENHANCE_TX
        5,    // XRTW2W // TODO
#else
        6,    // XRTW2W
#endif
        3,    // XRTW2R
        6,    // XRTR2W
#ifdef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
        8,    // XRTR2R
#else
        12,   // XRTR2R
#endif

        6,    // DMCATRAIN_INTV, no shuffle, set as ACTimingTable[0]'s value
        3,    // DQSINCTL // TODO
        13,    // DATLAT  // TODO
        12,    // WL
        24    // RL
    },

    // LP4-1600, 800MHz, DBI_OFF, normal mode
    {
        TYPE_LPDDR4, // Dram type
        800 , // Dram frequency
        DBI_OFF,
        CBT_NORMAL_MODE,

        //====  AC timing ====
#if (ACTIMING_TRAS_LOOSEN == 1)
        1,    // TRAS
#else
        0,    // TRAS
#endif
        2,    // TRP
        0,    // TPRAB
        4,    // TRC
        44,    // TFRC
        16,    // TRFCPB
        0,    // TXP
        1,    // TRTP
        3,    // TRCD
        7,    // TWR
        4,    // TWTR
        0,    // TRRD
        0,    // TFAW
        2,    // TRTW_ODT_OFF
        5,    // TRTW_ODT_ON
        48,    // REFCNT
        101,    // REFCNT_FR_CLK
        62,    // TXREFCNT
        16,    // TZQCS

        6,    // TRTPD
        6,    // TWTPD
        3,    // TMRR2W
        5,    // TMRR2W

        // ====  AC timing 0.5T ====
        0,    // TRAS_0.5T
        0,    // TRP_0.5T
        1,    // TRPAB_0.5T
        0,    // TRC_0.5T
        0,    // TRFC_0.5T
        0,    // TRFCPB_0.5T
        0,    // TXP_0.5T
        1,    // TRTP_0.5T
        0,    // TRCD_0.5T
        1,    // TWR_0.5T
        1,    // TWTR_0.5T
        0,    // TRRD_0.5T
        0,    // TFAW_0.5T
        0,    // TRTW_0.5T_ODT_OFF
        0,    // TRTW_0.5T_ODT_ON

        0,    // AC_TIMING_TRTPD_05T
        0,    // AC_TIMING_TWTPD_05T

        // ==== Cross rank setting ====
#ifdef XRTW2W_PERFORM_ENHANCE_TX
        5,    // XRTW2W
#else
        6,    // XRTW2W
#endif
        3,    // XRTW2R
        4,    // XRTR2W
#ifdef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
        8,    // XRTR2R
#else
        12,   // XRTR2R
#endif

        4,    // DMCATRAIN_INTV, no shuffle, set as ACTimingTable[0]'s value
        1,    // DQSINCTL
        10,    //Datlat = normal mode +1
        8,    // WL
        14    // RL
    },

#if ENABLE_LP3_SW
    //======= LP3 =================================================================
    // LP3 ACTimingTable originated from "Bianco LP3 ACTiming excel file"
    // LP3-1866, 933MHz
    {
        TYPE_LPDDR3, // Dram type
        933, // Dram frequency
        DBI_OFF,
        CBT_NORMAL_MODE,

        //====  AC timing ====
#if (ACTIMING_TRAS_LOOSEN == 1)
        12,    // TRAS
#else
        11,    // TRAS
#endif
        9,    // TRP
        1,    // TPRAB
        23,    // TRC
        86,    // TFRC
        30,    // TRFCPB
        0,    // TXP
        2,    // TRTP
        10,    // TRCD
        12,    // TWR
        7,    // TWTR
        3,    // TRRD
        14,    // TFAW
#if DramcHWDQSGatingTracking_DVT_JADE_TRACKING_MODE
        7,    // TRTW_ODT_OFF
        7,    // TRTW_ODT_ON
#else
        6,    // TRTW_ODT_OFF
        6,    // TRTW_ODT_ON
#endif
        112,    // REFCNT
        101,    // REFCNT_FR_CLK
        101,    // TXREFCNT
        41,    // TZQCS

        10,    // TRTPD
        11,    // TWTPD
        0,    // TMRR2W
        0,    // TMRR2W

        // ====  AC timing 0.5T ====
        0,    // TRAS_0.5T
        1,    // TRP_0.5T
        1,    // TRPAB_0.5T
        1,    // TRC_0.5T
        0,    // TRFC_0.5T
        0,    // TRFCPB_0.5T
        1,    // TXP_0.5T
        1,    // TRTP_0.5T
        1,    // TRCD_0.5T
        1,    // TWR_0.5T
        0,    // TWTR_0.5T
        0,    // TRRD_0.5T
        1,    // TFAW_0.5T
        1,    // TRTW_0.5T_ODT_OFF
        1,    // TRTW_0.5T_ODT_ON

        0,    // AC_TIMING_TRTPD_05T (Can't find!)
        0,    // AC_TIMING_TWTPD_05T (Can't find!)

        // ==== Cross rank setting ====
#ifdef XRTW2W_PERFORM_ENHANCE_TX
        6,    // XRTW2W
#else
        7,    // XRTW2W
#endif
#if DramcHWDQSGatingTracking_DVT_JADE_TRACKING_MODE
        5,    // XRTW2R
#else
        2,    // XRTW2R
#endif
        6,    // XRTR2W
        10,    // XRTR2R

        0,    // DMCATRAIN_INTV

        5,    // DQSINCTL
        17,    //Datlat
        8,    // WL
        14    // RL
    },

    // LP3-1600, 800MHz
    {
        TYPE_LPDDR3, // Dram type
        800, // Dram frequency
        DBI_OFF,
        CBT_NORMAL_MODE,

        //====  AC timing ====
#if (ACTIMING_TRAS_LOOSEN == 1)
        9,    // TRAS
#else
        8,    // TRAS
#endif
        8,    // TRP
        1,    // TPRAB
        19,    // TRC
        72,    // TFRC
        24,    // TRFCPB
        0,    // TXP
        1,    // TRTP
        9,    // TRCD
        10,    // TWR
        6,    // TWTR
        2,    // TRRD
        11,    // TFAW
#if DramcHWDQSGatingTracking_DVT_JADE_TRACKING_MODE
        6,    // TRTW_ODT_OFF
        6,    // TRTW_ODT_ON
#else
        5,    // TRTW_ODT_OFF
        5,    // TRTW_ODT_ON
#endif
        96,    // REFCNT
        101,    // REFCNT_FR_CLK
        86,    // TXREFCNT
        35,    // TZQCS

        8,    // TRTPD
        9,    // TWTPD
        0,    // TMRR2W (can't find in table)
        0,    // TMRR2W (can't find in table)

        // ====  AC timing 0.5T ====
        0,    // TRAS_0.5T
        0,    // TRP_0.5T
        0,    // TRPAB_0.5T
        0,    // TRC_0.5T
        0,    // TRFC_0.5T
        0,    // TRFCPB_0.5T
        0,    // TXP_0.5T
        0,    // TRTP_0.5T
        0,    // TRCD_0.5T
        1,    // TWR_0.5T
        1,    // TWTR_0.5T
        0,    // TRRD_0.5T
        0,    // TFAW_0.5T
        1,    // TRTW_0.5T_ODT_OFF
        1,    // TRTW_0.5T_ODT_ON

        0,    // AC_TIMING_TRTPD_05T (Can't find!)
        0,    // AC_TIMING_TWTPD_05T (Can't find!)

        // ==== Cross rank setting ====
#ifdef XRTW2W_PERFORM_ENHANCE_TX
        6,    // XRTW2W
#else
        7,    // XRTW2W
#endif
#if DramcHWDQSGatingTracking_DVT_JADE_TRACKING_MODE
        5,    // XRTW2R
#else
        2,    // XRTW2R
#endif
        6,    // XRTR2W
        10,    // XRTR2R

        0,    // DMCATRAIN_INTV

        4,    // DQSINCTL (TODO)
        15,    //Datlat (TODO)
        6,    // WL
        12    // RL
    },

    // LP3-1333, 667MHz
    {
        TYPE_LPDDR3, // Dram type
        667, // Dram frequency
        DBI_OFF,
        CBT_NORMAL_MODE,

        //====  AC timing ====
#if (ACTIMING_TRAS_LOOSEN == 1)
        6,    // TRAS
#else
        5,    // TRAS
#endif
        6,    // TRP
        1,    // TPRAB
        14,    // TRC
        58,    // TFRC
        18,    // TRFCPB
        0,    // TXP (is -1 in Table)
        1,    // TRTP
        7,    // TRCD
        9,    // TWR
        5,    // TWTR
        1,    // TRRD
        8,    // TFAW
#if DramcHWDQSGatingTracking_DVT_JADE_TRACKING_MODE
        5,    // TRTW_ODT_OFF
        5,    // TRTW_ODT_ON
#else
        4,    // TRTW_ODT_OFF
        4,    // TRTW_ODT_ON
#endif
        80,    // REFCNT
        101,    // REFCNT_FR_CLK
        72,    // TXREFCNT
        29,    // TZQCS

        7,    // TRTPD
        8,    // TWTPD
        0,    // TMRR2W (can't find in table)
        0,    // TMRR2W (can't find in table)

        // ====  AC timing 0.5T ====
        0,    // TRAS_0.5T
        0,    // TRP_0.5T
        0,    // TRPAB_0.5T
        0,    // TRC_0.5T
        0,    // TRFC_0.5T
        0,    // TRFCPB_0.5T
        1,    // TXP_0.5T
        1,    // TRTP_0.5T
        0,    // TRCD_0.5T
        1,    // TWR_0.5T
        0,    // TWTR_0.5T
        1,    // TRRD_0.5T
        0,    // TFAW_0.5T
        1,    // TRTW_0.5T_ODT_OFF
        1,    // TRTW_0.5T_ODT_ON

        0,    // AC_TIMING_TRTPD_05T (Can't find!)
        0,    // AC_TIMING_TWTPD_05T (Can't find!)

        // ==== Cross rank setting ====
#ifdef XRTW2W_PERFORM_ENHANCE_TX
        6,    // XRTW2W
#else
        7,    // XRTW2W
#endif
#if DramcHWDQSGatingTracking_DVT_JADE_TRACKING_MODE
        5,    // XRTW2R
#else
        2,    // XRTW2R
#endif
        6,    // XRTR2W
        9,    // XRTR2R

        0,    // DMCATRAIN_INTV

        3,    // DQSINCTL
        14,    //Datlat
        6,    // WL
        10    // RL
    }
    // LP3-DDR933 is removed due to gating PI hole
#endif /* ENABLE_LP3_SW */
};

#if ENABLE_MRR_AFTER_FIRST_K
/* JESD209-4B: tRFCab has 4 settings for 7 density settings (130, 180, 280, 380)
 *             use 4 in ACTimingOptimize arrays to save code size
 */
#define tRFCAB_SETTING_COUNT 4

/* Optimize all-bank refresh parameters (by density) for LP4 */
void vDramcACTimingOptimize(DRAMC_CTX_T *p)
{
    /* TRFC: tRFCab
     *       Refresh Cycle Time (All Banks)
     * TXREFCNT: tXSR max((tRFCab + 7.5ns), 2nCK)
     *           Min self refresh time (Entry to Exit)
     * u1ExecuteOptimize: Indicate if ACTimings are updated at the end of this function
     */
    U8 u1RFCabGrpIdx = 0, u1FreqGrpIdx = 0, u1ExecuteOptimize = ENABLE;
    U8 u1TRFC=101, u1TRFC_05T=0, u1TRFCpb=44, u1TRFCpb_05T=0, u1TXREFCNT=118;
    typedef struct
    {   /* Bitfield sizes set to current project register field's size */
        U8 u1TRFC      : 8;
        U8 u1TRFRC_05T : 1;
        U8 u1TRFCpb      : 8;
        U8 u1TRFRCpb_05T : 1;
        U16 u2TXREFCNT : 10;
    } optimizeACTime;
    /* Used to indicate tRFCab group (since some densities share the same tRFCab) */
    enum tRFCABIdx{tRFCAB_130 = 0, tRFCAB_180, tRFCAB_280, tRFCAB_380, tRFCAB_NUM};
    enum ACTimeIdx{GRP_DDR1600_ACTIM = 0, GRP_DDR2400_ACTIM, GRP_DDR2667_ACTIM, GRP_DDR3200_ACTIM, GRP_DDR3733_ACTIM, GRP_ACTIM_NUM};
    /* Values retrieved from 1. Alaska ACTiming excel file 2. JESD209-4B Refresh requirement table */
    optimizeACTime tRFCab_Opt [GRP_ACTIM_NUM][tRFCAB_NUM] =
    {
        //For freqGroup DDR1600
        {{.u1TRFC = 14, .u1TRFRC_05T = 0, .u1TRFCpb = 0, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 32},     //tRFCab = 130, tRFCpb = 60
         {.u1TRFC = 24, .u1TRFRC_05T = 0, .u1TRFCpb = 6, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 42},     //tRFCab = 180, tRFCpb = 90
         {.u1TRFC = 44, .u1TRFRC_05T = 0, .u1TRFCpb = 16, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 62},    //tRFCab = 280, tRFCpb = 140
         {.u1TRFC = 64, .u1TRFRC_05T = 0, .u1TRFCpb = 26, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 82}},   //tRFCab = 380, tRFCpb = 190
        //For freqGroup DDR2400
        {{.u1TRFC = 27, .u1TRFRC_05T = 0, .u1TRFCpb = 6, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 46},     //tRFCab = 130, tRFCpb = 60
         {.u1TRFC = 42, .u1TRFRC_05T = 0, .u1TRFCpb = 15, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 61},    //tRFCab = 180, tRFCpb = 90
         {.u1TRFC = 72, .u1TRFRC_05T = 0, .u1TRFCpb = 30, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 91},    //tRFCab = 280, tRFCpb = 140
         {.u1TRFC = 102, .u1TRFRC_05T = 0, .u1TRFCpb = 45, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 121}}, //tRFCab = 380, tRFCpb = 190
        //For freqGroup DDR2667
        {{.u1TRFC = 31, .u1TRFRC_05T = 1, .u1TRFCpb = 8, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 50},     //tRFCab = 130, tRFCpb = 60
         {.u1TRFC = 48, .u1TRFRC_05T = 0, .u1TRFCpb = 18, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 67},    //tRFCab = 180, tRFCpb = 90
         {.u1TRFC = 81, .u1TRFRC_05T = 1, .u1TRFCpb = 35, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 100},   //tRFCab = 280, tRFCpb = 140
         {.u1TRFC = 115, .u1TRFRC_05T = 0, .u1TRFCpb = 51, .u1TRFRCpb_05T = 1, .u2TXREFCNT = 134}}, //tRFCab = 380, tRFCpb = 190
        //For freqGroup DDR3200
       {{.u1TRFC = 40, .u1TRFRC_05T = 0, .u1TRFCpb = 12, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 59},     //tRFCab = 130, tRFCpb = 60
         {.u1TRFC = 60, .u1TRFRC_05T = 0, .u1TRFCpb = 24, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 79},    //tRFCab = 180, tRFCpb = 90
         {.u1TRFC = 100, .u1TRFRC_05T = 0, .u1TRFCpb = 44, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 119},  //tRFCab = 280, tRFCpb = 140
         {.u1TRFC = 140, .u1TRFRC_05T = 0, .u1TRFCpb = 64, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 159}}, //tRFCab = 380, tRFCpb = 190
        //For freqGroup DDR3733
        {{.u1TRFC = 48, .u1TRFRC_05T = 1, .u1TRFCpb = 16, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 68},    //tRFCab = 130, tRFCpb = 60
         {.u1TRFC = 72, .u1TRFRC_05T = 0, .u1TRFCpb = 30, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 92},    //tRFCab = 180, tRFCpb = 90
         {.u1TRFC = 118, .u1TRFRC_05T = 1, .u1TRFCpb = 53, .u1TRFRCpb_05T = 1, .u2TXREFCNT = 138},  //tRFCab = 280, tRFCpb = 140
         {.u1TRFC = 165, .u1TRFRC_05T = 0, .u1TRFCpb = 76, .u1TRFRCpb_05T = 1, .u2TXREFCNT = 185}}  //tRFCab = 380, tRFCpb = 190
    };
    mcSHOW_DBG_MSG(("[ACTimingOptimize]"));

    if(u1IsLP4Family(p->dram_type))  //already read MR8 for density update
    {
        /* Set tRFCab group idx p->density = MR8 OP[5:2]*/
        switch (p->density)
        {
            case 0x0:   //4Gb per die  (2Gb per channel),  tRFCab=130
                u1RFCabGrpIdx = tRFCAB_130;
                break;
            case 0x1:   //6Gb per die  (3Gb per channel),  tRFCab=180
            case 0x2:   //8Gb per die  (4Gb per channel),  tRFCab=180
                u1RFCabGrpIdx = tRFCAB_180;
                break;
            case 0x3:   //12Gb per die (6Gb per channel),  tRFCab=280
            case 0x4:   //16Gb per die (8Gb per channel),  tRFCab=280
                u1RFCabGrpIdx = tRFCAB_280;
                break;
            case 0x5:   //24Gb per die (12Gb per channel), tRFCab=380
            case 0x6:   //32Gb per die (16Gb per channel), tRFCab=380
                u1RFCabGrpIdx = tRFCAB_380;
                break;
            default:
                u1ExecuteOptimize = DISABLE;
                mcSHOW_ERR_MSG(("MR8 density err!\n"));
        }
        /* Set freqGroup Idx */
        switch (p->freqGroup)
        {
            case 800:
                u1FreqGrpIdx = GRP_DDR1600_ACTIM;
                break;
            case 1200:
                u1FreqGrpIdx = GRP_DDR2400_ACTIM;
                break;
            case 1333:
                u1FreqGrpIdx = GRP_DDR2667_ACTIM;
                break;
            case 1600:
                u1FreqGrpIdx = GRP_DDR3200_ACTIM;
                break;
            case 1866:
                u1FreqGrpIdx = GRP_DDR3733_ACTIM;
                break;
            default:
                u1ExecuteOptimize = DISABLE;
                mcSHOW_ERR_MSG(("freqGroup err!\n"));
        }

        u1TRFC = tRFCab_Opt[u1FreqGrpIdx][u1RFCabGrpIdx].u1TRFC;
        u1TRFC_05T = tRFCab_Opt[u1FreqGrpIdx][u1RFCabGrpIdx].u1TRFRC_05T;
        u1TRFCpb = tRFCab_Opt[u1FreqGrpIdx][u1RFCabGrpIdx].u1TRFCpb;
        u1TRFCpb_05T = tRFCab_Opt[u1FreqGrpIdx][u1RFCabGrpIdx].u1TRFRCpb_05T;
        u1TXREFCNT = tRFCab_Opt[u1FreqGrpIdx][u1RFCabGrpIdx].u2TXREFCNT;
    }
    else
    { /* TODO: LP3 tRFCab by density */
        u1ExecuteOptimize = DISABLE;
    }

    /* Only execute ACTimingOptimize(write to regs) when corresponding values have been found */
    if (u1ExecuteOptimize == ENABLE)
    {
        vIO32WriteFldAlign_All(DRAMC_REG_SHU_ACTIM3, u1TRFC, SHU_ACTIM3_TRFC);
        vIO32WriteFldAlign_All(DRAMC_REG_SHU_AC_TIME_05T, u1TRFC_05T, SHU_AC_TIME_05T_TRFC_05T);
        vIO32WriteFldAlign_All(DRAMC_REG_SHU_ACTIM4, u1TXREFCNT, SHU_ACTIM4_TXREFCNT);
        vIO32WriteFldAlign_All(DRAMC_REG_SHU_ACTIM3, u1TRFCpb, SHU_ACTIM3_TRFCPB);
        vIO32WriteFldAlign_All(DRAMC_REG_SHU_AC_TIME_05T, u1TRFCpb_05T, SHU_AC_TIME_05T_TRFCPB_05T);

        mcSHOW_DBG_MSG(("Density (MR8 OP[5:2]) %u, TRFC %u, TRFC_05T %u, TXREFCNT %u, TRFCpb %u, TRFCpb_05T %u\n", p->density, u1TRFC, u1TRFC_05T, u1TXREFCNT, u1TRFCpb, u1TRFCpb_05T));
        mcFPRINTF((fp_A60501, "Density (MR8 (OP[5:2]) %u, TRFC %u, TRFC_05T %u, TXREFCNT %u, TRFCpb %u, TRFCpb_05T %u\n", p->density, u1TRFC, u1TRFC_05T, u1TXREFCNT, u1TRFCpb, u1TRFCpb_05T));
    }

    return;
}
#endif

DRAM_STATUS_T UpdatetRFCMaxTiming(DRAMC_CTX_T *p, U16 u2tRFCTiming, U8 u1Div, U8 u1tCK, U8 *u1tRFC_tmp, U8 *u1tRFC_05T_tmp)
{
    U16 u2DRAMClkCycles = 0, u2DRAMCyclesRoundUp = 0;

    u2DRAMClkCycles = ((u2tRFCTiming*1000) * u1tCK)/(100000000/(((p->frequency*2)*100) >> u1Div)); //tRFCab max is 380ns for 12Gb up
    u2DRAMCyclesRoundUp = ((u2tRFCTiming*1000) * u1tCK)%(100000000/(((p->frequency*2)*100) >> u1Div));
    if (u2DRAMCyclesRoundUp != 0)
        u2DRAMClkCycles++;

    u2DRAMClkCycles = u2DRAMClkCycles - u1tCK*11;  //DRAMC internal margin = Div Mode * 11tCK
    *u1tRFC_tmp = u2DRAMClkCycles/u1tCK; // MCK - Rounddown
    *u1tRFC_05T_tmp = u2DRAMClkCycles%u1tCK; //05T
    //mcSHOW_DBG_MSG(("[UpdatetRFCMaxTiming] u1tRFC_tmp = %d, u1tRFC_05T_tmp = %d\n", *u1tRFC_tmp, *u1tRFC_05T_tmp));

    // 05T
    if (*u1tRFC_05T_tmp >= 3)
    {
        (*u1tRFC_tmp)++;
        *u1tRFC_05T_tmp = 0;
    }
    else if (*u1tRFC_05T_tmp >= 1)
    {
        *u1tRFC_05T_tmp = 1;
    }

    return DRAM_OK;
}

DRAM_STATUS_T DdrUpdateACTimingReg(DRAMC_CTX_T *p, const U16 *ACTable)
{
    DRAM_ODT_MODE_T r2w_odt_onoff = ODT_ON;
    U32 RODT_TRACKING_SAVEING_MCK=0;
    U8 u1ROOT=0, u1TXRANKINCTL=0, u1TXDLY=0, u1RANKINCTL, u1DATLAT_DSEL;
    U8 u1CkePulseWidth = 0; // reg field name:CKEPRD
#if SAMSUNG_LP4_NWR_WORKAROUND
    U8 u1TWTR = 0, u1TWTR_05T = 0, u1TWTR_TMP = 0;
#endif

    U16 u2TXREFCNT = 0, u2tRFCabTiming = 380, u2tRFCpbTiming = 190;
    U8 u1DivMode = 3, u1MCKtoTck = 4;
    U8 u1tRFCab = 0, u1tRFCab_05T = 0, u1tRFCpb = 0, u1tRFCpb_05T = 0;

    if(ACTable == NULL)
        return DRAM_FAIL;

        #if ENABLE_TX_WDQS
    if(p->vendor_id == VENDOR_HYNIX)
    {
        r2w_odt_onoff = ODT_ON;
    }
    else
    {
        r2w_odt_onoff = p->odt_onoff;
    }
        #else
        r2w_odt_onoff = p->odt_onoff;
        #endif

#if ENABLE_RODT_TRACKING_SAVE_MCK
    // for rodt tracking save 1 MCK and rodt tracking enable or not(RODTEN_MCK_MODESEL)
    u1ODT_ON = p->odt_onoff;
    u1RODT_TRACK = ENABLE_RODT_TRACKING;
    u1ROEN = u1WDQS_ON | u1ODT_ON;
    u1ModeSel = u1RODT_TRACK & u1ROEN;
    
    // when WDQS on and RODT Track define open and un-term, RODT_TRACKING_SAVEING_MCK = 1 for the future setting
    // Maybe "Save 1 MCK" will be set after Vinson project, but Bianco & Vinson can not.(different with performance team)
    //if (u1RODT_TRACK && (u1ROEN==1))
    //    RODT_TRACKING_SAVEING_MCK = 1;
#endif

#if (ENABLE_RODT_TRACKING || defined(XRTR2W_PERFORM_ENHANCE_RODTEN))
	/* yr: same code
    // set to 0, let TRTW & XRTR2W setting values are the smae with DV-sim's value that DE provided
    if (r2w_odt_onoff == ODT_ON) RODT_TRACKING_SAVEING_MCK = 0; //RODT_TRACKING eanble can save r2w 1 MCK
    else RODT_TRACKING_SAVEING_MCK = 0;
	*/
    RODT_TRACKING_SAVEING_MCK = 0;
#endif   

    // tRFC formula start============================================================
    if (u1IsLP4Family(p->dram_type))
    {
        // tRFCab = 380ns
        UpdatetRFCMaxTiming(p, u2tRFCabTiming, u1DivMode, u1MCKtoTck, &u1tRFCab, &u1tRFCab_05T);
    
        // TXREFCNT
        u2TXREFCNT = ((u2tRFCabTiming*10+75)*1000)/(1000000000/(((p->frequency*2)*100) >> u1DivMode)); //tRFCab max is 380ns for 12Gb up
        if (((u2tRFCabTiming*10+75)*1000)%(1000000000/(((p->frequency*2)*100) >> u1DivMode)) != 0) // Round up
            u2TXREFCNT++;
        u2TXREFCNT = u2TXREFCNT - 3 + 7; // -3 + 7 by formula
    
        // tRFCpb = 190ns
        UpdatetRFCMaxTiming(p, u2tRFCpbTiming, u1DivMode, u1MCKtoTck, &u1tRFCpb, &u1tRFCpb_05T);
    
        mcSHOW_DBG_MSG(("[DdrUpdateACTimingReg] Freq = %d, tRFCab = %d, tRFCab_05T = %d, TXREFCNT = %d, tRFCpb = %d, tRFCpb_05T = %d\n", p->frequency, u1tRFCab, u1tRFCab_05T, u2TXREFCNT, u1tRFCpb, u1tRFCpb_05T));
    }
    else // LPDDR3
    {
        u1tRFCab = ACTable[AC_TIMING_TRFC];
        u1tRFCab_05T = ACTable[AC_TIMING_TRFC_05T];
        u2TXREFCNT = ACTable[AC_TIMING_TXREFCNT];
        u1tRFCpb = ACTable[AC_TIMING_TRFCPB];
        u1tRFCpb_05T = ACTable[AC_TIMING_TRFCPB_05T];
    }
    // =========================================================== tRFC formula end

    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM0),
                                    P_Fld( ACTable[AC_TIMING_TRCD], SHU_ACTIM0_TRCD)   |
                                    P_Fld( ACTable[AC_TIMING_TRRD], SHU_ACTIM0_TRRD)    |
                                    P_Fld( ACTable[AC_TIMING_TWR], SHU_ACTIM0_TWR)      |
                                    P_Fld( ACTable[AC_TIMING_TWTR], SHU_ACTIM0_TWTR));

    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM1),
                                    P_Fld( ACTable[AC_TIMING_TRC], SHU_ACTIM1_TRC)         |
                                    P_Fld( ACTable[AC_TIMING_TRAS], SHU_ACTIM1_TRAS)     |
                                    P_Fld( ACTable[AC_TIMING_TRP] ,SHU_ACTIM1_TRP)         |
                                    P_Fld( ACTable[AC_TIMING_TRPAB], SHU_ACTIM1_TRPAB));

    #if 1//!SUPPORT_HYNIX_RX_DQS_WEAK_PULL
    if((p->dram_type == TYPE_LPDDR4P) && (p->vendor_id == VENDOR_HYNIX)) //temp solution, need to discuss with SY
    {
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM2),
                                    P_Fld( ACTable[AC_TIMING_TFAW], SHU_ACTIM2_TFAW)         |
                                    P_Fld( ACTable[AC_TIMING_TRTW_ODT_OFF + 1] - RODT_TRACKING_SAVEING_MCK, SHU_ACTIM2_TR2W)               |
                                    P_Fld( ACTable[AC_TIMING_TRTP], SHU_ACTIM2_TRTP)         |
                                    P_Fld( ACTable[AC_TIMING_TXP], SHU_ACTIM2_TXP));
    }
    else
    #endif
    {
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM2),
                                    P_Fld( ACTable[AC_TIMING_TFAW], SHU_ACTIM2_TFAW)         |
                                    P_Fld( ACTable[AC_TIMING_TRTW_ODT_OFF + r2w_odt_onoff] - RODT_TRACKING_SAVEING_MCK, SHU_ACTIM2_TR2W)               |
                                    P_Fld( ACTable[AC_TIMING_TRTP], SHU_ACTIM2_TRTP)         |
                                    P_Fld( ACTable[AC_TIMING_TXP], SHU_ACTIM2_TXP));
    }

    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM3),
                                    P_Fld( u1tRFCab, SHU_ACTIM3_TRFC)         |
                                    P_Fld( ACTable[AC_TIMING_REFCNT], SHU_ACTIM3_REFCNT)         |
                                    P_Fld( u1tRFCpb, SHU_ACTIM3_TRFCPB));

    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM4),
                                    P_Fld( ACTable[AC_TIMING_TZQCS], SHU_ACTIM4_TZQCS)         |
                                    P_Fld( ACTable[AC_TIMING_REFCNT_FR_CLK], SHU_ACTIM4_REFCNT_FR_CLK)               |
                                    P_Fld( u2TXREFCNT, SHU_ACTIM4_TXREFCNT));

    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM5),
                                    P_Fld( ACTable[AC_TIMING_TMRR2W_ODT_OFF+ r2w_odt_onoff], SHU_ACTIM5_TMRR2W) |
                                    P_Fld( ACTable[AC_TIMING_TWTPD], SHU_ACTIM5_TWTPD) |
                                    P_Fld( ACTable[AC_TIMING_TRTPD], SHU_ACTIM5_TR2PD));

    // AC timing 0.5T
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_SHU_AC_TIME_05T),
                                    P_Fld(ACTable[AC_TIMING_TWTR_05T], SHU_AC_TIME_05T_TWTR_M05T)  |
                                    P_Fld(ACTable[AC_TIMING_TRTW_05T_ODT_OFF+ r2w_odt_onoff],        SHU_AC_TIME_05T_TR2W_05T)           |
                                    P_Fld(ACTable[AC_TIMING_TWTPD_05T],       SHU_AC_TIME_05T_TWTPD_M05T ) |
                                    P_Fld(ACTable[AC_TIMING_TRTPD_05T],     SHU_AC_TIME_05T_TR2PD_05T)         |
                                    P_Fld(ACTable[AC_TIMING_TFAW_05T],        SHU_AC_TIME_05T_TFAW_05T)               |
                                    P_Fld(ACTable[AC_TIMING_TRRD_05T],       SHU_AC_TIME_05T_TRRD_05T)         |
                                    P_Fld(ACTable[AC_TIMING_TWR_05T],   SHU_AC_TIME_05T_TWR_M05T)           |
                                    P_Fld(ACTable[AC_TIMING_TRAS_05T] ,    SHU_AC_TIME_05T_TRAS_05T ) |
                                    P_Fld(ACTable[AC_TIMING_TRPAB_05T],   SHU_AC_TIME_05T_TRPAB_05T)         |
                                    P_Fld(ACTable[AC_TIMING_TRP_05T],   SHU_AC_TIME_05T_TRP_05T)               |
                                    P_Fld(ACTable[AC_TIMING_TRCD_05T] ,  SHU_AC_TIME_05T_TRCD_05T)         |
                                    P_Fld(ACTable[AC_TIMING_TRTP_05T],   SHU_AC_TIME_05T_TRTP_05T)        |
                                    P_Fld(ACTable[AC_TIMING_TXP_05T],   SHU_AC_TIME_05T_TXP_05T)         |
                                    P_Fld(u1tRFCab_05T,   SHU_AC_TIME_05T_TRFC_05T)               |
                                    P_Fld(u1tRFCpb_05T,  SHU_AC_TIME_05T_TRFCPB_05T)         |
                                    P_Fld(ACTable[AC_TIMING_TRC_05T],   SHU_AC_TIME_05T_TRC_05T)
                                    );
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM_XRT),
                                    P_Fld(ACTable[AC_TIMING_XRTW2W], SHU_ACTIM_XRT_XRTW2W) |
                                    P_Fld(ACTable[AC_TIMING_XRTW2R], SHU_ACTIM_XRT_XRTW2R) |
                                    P_Fld(ACTable[AC_TIMING_XRTR2W] - RODT_TRACKING_SAVEING_MCK, SHU_ACTIM_XRT_XRTR2W) |
                                    P_Fld(ACTable[AC_TIMING_XRTR2R], SHU_ACTIM_XRT_XRTR2R));

    /* CATRAIN_INTV isn't a shuffle register, use ACTimingTable[0]'s value (It should be highest available freq.) */
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_CATRAINING1), ACTimingTable[0][AC_TIMING_DMCATRAIN_INTV], CATRAINING1_CATRAIN_INTV);

    // Write latency
#if 0//ndef OLYMPUS_TO_BE_PORTING
    vIO32Write4B_All(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0), ACTable[AC_TIMING_WRITE_LATENCY_0x41c]);
    vIO32Write4B_All(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ1), ACTable[AC_TIMING_WRITE_LATENCY_0x420]);
    vIO32Write4B_All(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_DQS0), ACTable[AC_TIMING_WRITE_LATENCY_0x424]);
    vIO32Write4B_All(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ3), ACTable[AC_TIMING_WRITE_LATENCY_0x428]);
    vIO32Write4B_All(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_DQS1), ACTable[AC_TIMING_WRITE_LATENCY_0x42c]);
#endif

    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQSCTL), ACTable[AC_TIMING_DQSINCTL_FOR_GATING], SHURK0_DQSCTL_DQSINCTL);// Rank 0 DQSINCTL
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SHURK1_DQSCTL), ACTable[AC_TIMING_DQSINCTL_FOR_GATING], SHURK1_DQSCTL_R1DQSINCTL);// Rank 1 DQSINCTL

    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SHU_ODTCTRL), ACTable[AC_TIMING_DQSINCTL_FOR_GATING], SHU_ODTCTRL_RODT);

    u1DATLAT_DSEL = ACTable[AC_TIMING_DATLAT] -2;
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_SHU_CONF1),
                                        P_Fld(ACTable[AC_TIMING_DATLAT], SHU_CONF1_DATLAT) |
                                        P_Fld(u1DATLAT_DSEL, SHU_CONF1_DATLAT_DSEL) |
                                        P_Fld(u1DATLAT_DSEL, SHU_CONF1_DATLAT_DSEL_PHY));
        
#ifdef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
    //Wei-Jen: Ininital setting values are the same, RANKINCTL_RXDLY = RANKINCTL = RANKINCTL_ROOT1
    //XRTR2R setting will be updated in RxdqsGatingPostProcess
    u1RANKINCTL = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_RANKCTL), SHU_RANKCTL_RANKINCTL);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SHU_RANKCTL), u1RANKINCTL, SHU_RANKCTL_RANKINCTL_RXDLY);
#endif

    /* Der-ping: CKEPRD - CKE pulse width
     * LP3: always set to 3
     * LP4:         freq <= 800, set to 2
     *      1866 >= freq >  800, set to 3
     */
    if (p->dram_type == TYPE_LPDDR3)
    {
        u1CkePulseWidth = 3;
    }
    else // u1IsLP4Family
    {
        if (p->frequency <= 800)
        {
            u1CkePulseWidth = 2;
        }
        else
        {
            u1CkePulseWidth = 3;
        }
    }
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_CKECTRL, u1CkePulseWidth, SHU_CKECTRL_CKEPRD);

    //Update releated RG of XRTW2W
#ifdef XRTW2W_PERFORM_ENHANCE_TX
    if (u1IsLP4Family(p->dram_type))
    {
        if (p->frequency <= 800) 
        {
            u1ROOT = 0; u1TXRANKINCTL=0; u1TXDLY=1;
        }
        else
        {
            u1ROOT = (p->frequency==1866)?1:0; 
            u1TXRANKINCTL=1; u1TXDLY=2;
        }
    }
    else
    {
        if (p->frequency <= 667)    //LP3-1333
        {
            u1ROOT = 0; u1TXRANKINCTL=0; u1TXDLY=1;
        }
        else
        {
            u1ROOT = 0; u1TXRANKINCTL=1; u1TXDLY=2;
        }
    }
#else
    if (u1IsLP4Family(p->dram_type))
    {
        if (p->frequency <= 800) 
        {
            u1ROOT = 0; u1TXRANKINCTL=0; u1TXDLY=0;
        }
        else
        {
            u1ROOT = 0; u1TXRANKINCTL=1; u1TXDLY=1;
        }
    }
    else
    {
        if (p->frequency <= 667)    //LP3-1333
        {
            u1ROOT = 0; u1TXRANKINCTL=0; u1TXDLY=0;
        }
        else
        {
            u1ROOT = 0; u1TXRANKINCTL=1; u1TXDLY=1;
        }
    }
#endif
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_SHU_RANKCTL), P_Fld(u1ROOT, SHU_RANKCTL_TXRANKINCTL_ROOT) | P_Fld(u1TXRANKINCTL, SHU_RANKCTL_TXRANKINCTL) | P_Fld(u1TXDLY, SHU_RANKCTL_TXRANKINCTL_TXDLY));

#if SAMSUNG_LP4_NWR_WORKAROUND
    // If nWR is fixed to 30 for all freqs, tWTR@800Mhz should add 2tCK gap, allowing sufficient Samsung DRAM internal IO precharge time
    if (u1IsLP4Family(p->dram_type) && (p->vendor_id == VENDOR_SAMSUNG) && (p->frequency == 800)) //LP4X, Samsung, DDR1600
    {
        u1TWTR_TMP = (ACTable[AC_TIMING_TWTR] * 4 - ACTable[AC_TIMING_TWTR_05T] * 2) + 2; //Convert TWTR to tCK, and add 2tCK
        if ((u1TWTR_TMP % 4) == 0) //TWTR can be transferred to TWTR directly
        {
            u1TWTR = u1TWTR_TMP >> 2;
            u1TWTR_05T = 0;
        }
        else //Can't be transfered to TWTR directly
        {
            u1TWTR = (u1TWTR_TMP + 2) >> 2; //Add 2 tCK and set TWTR value (Then minus 2tCK using 05T)
            u1TWTR_05T = 1;  //05T means minus 2tCK
        }

        vIO32WriteFldAlign_All(DRAMC_REG_SHU_ACTIM0, u1TWTR, SHU_ACTIM0_TWTR);
        vIO32WriteFldAlign_All(DRAMC_REG_SHU_AC_TIME_05T, u1TWTR_05T, SHU_AC_TIME_05T_TWTR_M05T);
    }
#endif

    return DRAM_OK;
}


DRAM_STATUS_T DdrUpdateACTiming(DRAMC_CTX_T *p)
{
    U8 u1TimingIdx=0xff, u1TmpIdx;
    U8 u1TmpDramType = p->dram_type;

    mcSHOW_DBG_MSG3(("[UpdateACTiming]\n"));
    // LP4 and LP4x use same table.
    if(u1TmpDramType== TYPE_LPDDR4X || u1TmpDramType == TYPE_LPDDR4P)
        u1TmpDramType = TYPE_LPDDR4;

    for(u1TmpIdx=0; u1TmpIdx<TOTAL_AC_TIMING_NUMBER; u1TmpIdx++)
    {
        if((ACTimingTable[u1TmpIdx][AC_TIMING_DRAM_TYPE] == u1TmpDramType) &&
            /* p->frequency may not be in ACTimingTable, use p->freqGroup */
            (ACTimingTable[u1TmpIdx][AC_TIMING_FREQUENCY] == p->freqGroup) &&
            (ACTimingTable[u1TmpIdx][AC_TIMING_DBI_ONOFF] == p->DBI_R_onoff[p->dram_fsp]) &&
            (ACTimingTable[u1TmpIdx][AC_TIMING_BYTE_MODE] == vGet_Dram_CBT_Mode(p)))
        {
            u1TimingIdx = u1TmpIdx;
            mcSHOW_DBG_MSG(("match AC timing %d\n", u1TimingIdx));
            mcFPRINTF((fp_A60501, "match AC timing %d\n", u1TimingIdx));
            break;
        }
    }

    if(u1TimingIdx == 0xff)
    {
        #if 0
        if(u1TmpDramType = TYPE_LPDDR4)
            u1TimingIdx =0;
        else // LPDDR3
            u1TimingIdx =6;
        mcSHOW_ERR_MSG(("Error, no match AC timing, use default timing %d\n", u1TimingIdx));
        mcFPRINTF((fp_A60501, "Error, no match AC timing, use default timing %d\n", u1TimingIdx));
        #else
        mcSHOW_ERR_MSG(("Error, no match AC timing, not apply table\n"));
        mcFPRINTF((fp_A60501, "Error, no match AC timing, not apply table\n"));
        return DRAM_FAIL;
        #endif
    }

    DdrUpdateACTimingReg(p, ACTimingTable[u1TimingIdx]);

    return DRAM_OK;
}

#if ((!SW_CHANGE_FOR_SIMULATION) && (!FOR_DV_SIMULATION_USED && SW_CHANGE_FOR_SIMULATION==0) && (!__ETT__))
DRAM_STATUS_T DdrUpdateACTiming_EMI(DRAMC_CTX_T *p, AC_TIMING_EXTERNAL_T *ACRegFromEmi)
{
    U8 u1TimingIdx=0xff, u1TmpIdx;
    U16 ACTiming[AC_TIMING_ITEM_NUM];
    U8 u1TmpDramType = p->dram_type;

    mcSHOW_DBG_MSG3(("[DdrUpdateACTiming_EMI]\n"));

   if(ACRegFromEmi == NULL)
        return DRAM_FAIL;

    //Get AC timing from emi setting
    ACTiming[AC_TIMING_DRAM_TYPE] =  p->dram_type;
    ACTiming[AC_TIMING_FREQUENCY] =  ACRegFromEmi->AC_TIME_EMI_FREQUENCY ;
    ACTiming[AC_TIMING_TRAS] =  ACRegFromEmi->AC_TIME_EMI_TRAS ;
    ACTiming[AC_TIMING_TRP] =  ACRegFromEmi->AC_TIME_EMI_TRP ;

    ACTiming[AC_TIMING_TRPAB] =  ACRegFromEmi->AC_TIME_EMI_TRPAB ;
    ACTiming[AC_TIMING_TRC] =  ACRegFromEmi->AC_TIME_EMI_TRC ;
    ACTiming[AC_TIMING_TRFC] =  ACRegFromEmi->AC_TIME_EMI_TRFC ;
    ACTiming[AC_TIMING_TRFCPB] =  ACRegFromEmi->AC_TIME_EMI_TRFCPB ;

    ACTiming[AC_TIMING_TXP] =  ACRegFromEmi->AC_TIME_EMI_TXP ;
    ACTiming[AC_TIMING_TRTP] =  ACRegFromEmi->AC_TIME_EMI_TRTP ;
    ACTiming[AC_TIMING_TRCD] =  ACRegFromEmi->AC_TIME_EMI_TRCD ;
    ACTiming[AC_TIMING_TWR] =  ACRegFromEmi->AC_TIME_EMI_TWR ;
    ACTiming[AC_TIMING_TWTR] =  ACRegFromEmi->AC_TIME_EMI_TWTR ;
    ACTiming[AC_TIMING_TRRD] =  ACRegFromEmi->AC_TIME_EMI_TRRD ;
    ACTiming[AC_TIMING_TFAW] =  ACRegFromEmi->AC_TIME_EMI_TFAW ;
    ACTiming[AC_TIMING_TRTW_ODT_OFF] =  ACRegFromEmi->AC_TIME_EMI_TRTW_ODT_OFF ;
    ACTiming[AC_TIMING_TRTW_ODT_ON] =  ACRegFromEmi->AC_TIME_EMI_TRTW_ODT_ON ;
    ACTiming[AC_TIMING_REFCNT] =  ACRegFromEmi->AC_TIME_EMI_REFCNT ; //(REFFRERUN = 0)
    ACTiming[AC_TIMING_REFCNT_FR_CLK] =  ACRegFromEmi->AC_TIME_EMI_REFCNT_FR_CLK ; //(REFFRERUN = 1)
    ACTiming[AC_TIMING_TXREFCNT] =  ACRegFromEmi->AC_TIME_EMI_TXREFCNT ;
    ACTiming[AC_TIMING_TZQCS] =  ACRegFromEmi->AC_TIME_EMI_TZQCS ;

    ACTiming[AC_TIMING_TRTPD] =  ACRegFromEmi->AC_TIME_EMI_TRTPD ;// oylmpus new
    ACTiming[AC_TIMING_TWTPD] =  ACRegFromEmi->AC_TIME_EMI_TWTPD ;// oylmpus new
    ACTiming[AC_TIMING_TMRR2W_ODT_OFF] =  ACRegFromEmi->AC_TIME_EMI_TMRR2W_ODT_OFF ;// oylmpus new
    ACTiming[AC_TIMING_TMRR2W_ODT_ON] =  ACRegFromEmi->AC_TIME_EMI_TMRR2W_ODT_ON;// oylmpus new

    ACTiming[AC_TIMING_TRAS_05T] =  ACRegFromEmi->AC_TIME_EMI_TRAS_05T ;
    ACTiming[AC_TIMING_TRP_05T] =  ACRegFromEmi->AC_TIME_EMI_TRP_05T ;
    ACTiming[AC_TIMING_TRPAB_05T] =  ACRegFromEmi->AC_TIME_EMI_TRPAB_05T ;
    ACTiming[AC_TIMING_TRC_05T] =  ACRegFromEmi->AC_TIME_EMI_TRC_05T ;
    ACTiming[AC_TIMING_TRFC_05T] =  ACRegFromEmi->AC_TIME_EMI_TRFC_05T ;
    ACTiming[AC_TIMING_TRFCPB_05T] =  ACRegFromEmi->AC_TIME_EMI_TRFCPB_05T ;
    ACTiming[AC_TIMING_TXP_05T] =  ACRegFromEmi->AC_TIME_EMI_TXP_05T ;
    ACTiming[AC_TIMING_TRTP_05T] =  ACRegFromEmi->AC_TIME_EMI_TRTP_05T ;
    ACTiming[AC_TIMING_TRCD_05T] =  ACRegFromEmi->AC_TIME_EMI_TRCD_05T ;
    ACTiming[AC_TIMING_TWR_05T] =  ACRegFromEmi->AC_TIME_EMI_TWR_05T ;
    ACTiming[AC_TIMING_TWTR_05T] =  ACRegFromEmi->AC_TIME_EMI_TWTR_05T ;

    ACTiming[AC_TIMING_TRRD_05T] =  ACRegFromEmi->AC_TIME_EMI_TRRD_05T ;
    ACTiming[AC_TIMING_TFAW_05T] =  ACRegFromEmi->AC_TIME_EMI_TFAW_05T ;
    ACTiming[AC_TIMING_TRTW_05T_ODT_OFF] =  ACRegFromEmi->AC_TIME_EMI_TRTW_ODT_OFF_05T ;
    ACTiming[AC_TIMING_TRTW_05T_ODT_ON] =  ACRegFromEmi->AC_TIME_EMI_TRTW_ODT_ON_05T ;

    ACTiming[AC_TIMING_TRTPD_05T] =  ACRegFromEmi->AC_TIME_EMI_TRTPD_05T ;
    ACTiming[AC_TIMING_TWTPD_05T] =  ACRegFromEmi->AC_TIME_EMI_TWTPD_05T ;

    //ACTiming[AC_TIMING_REFCNT_05T] =  ACRegFromEmi->AC_TIME_EMI_REFCNT_05T ; //(REFFRERUN = 0)
    //ACTiming[AC_TIMING_REFCNT_FR_CLK_05T] =  ACRegFromEmi->AC_TIME_EMI_REFCNT_FR_CLK_05T ; //(REFFRERUN = 1)
    //ACTiming[AC_TIMING_TXREFCNT_05T] =  ACRegFromEmi->AC_TIME_EMI_TXREFCNT_05T ;
    //ACTiming[AC_TIMING_TZQCS_05T] =  ACRegFromEmi->AC_TIME_EMI_TZQCS_05T ;

    // LP4 and LP4x use same table.
    if(u1TmpDramType== TYPE_LPDDR4X || u1TmpDramType == TYPE_LPDDR4P)
        u1TmpDramType = TYPE_LPDDR4;

    //Get AC timing from internal ACTimingTable
    for(u1TmpIdx=0; u1TmpIdx<TOTAL_AC_TIMING_NUMBER; u1TmpIdx++)
    {
        if((ACTimingTable[u1TmpIdx][AC_TIMING_DRAM_TYPE] == u1TmpDramType) &&
            /* p->frequency may not be in ACTimingTable, use p->freqGroup */
            (ACTimingTable[u1TmpIdx][AC_TIMING_FREQUENCY] == p->freqGroup) &&
            //(ACTimingTable[u1TmpIdx][AC_TIMING_DBI_ONOFF] == p->DBI_R_onoff[p->dram_fsp]) &&
            (ACTimingTable[u1TmpIdx][AC_TIMING_BYTE_MODE] == vGet_Dram_CBT_Mode(p)))
        {
            u1TimingIdx = u1TmpIdx;
            mcSHOW_DBG_MSG(("match AC timing %d\n", u1TimingIdx));
            mcFPRINTF((fp_A60501, "match AC timing %d\n", u1TimingIdx));
            break;
        }
    }

    if(u1TimingIdx == 0xff)
    {
        u1TimingIdx =0;
        mcSHOW_DBG_MSG(("Error, no match AC timing, use default timing 0\n"));
        mcFPRINTF((fp_A60501, "Error, no match AC timing, use default timing 0\n"));
        //return DRAM_FAIL;
    }

    ACTiming[AC_TIMING_XRTW2W] =  ACTimingTable[u1TimingIdx][AC_TIMING_XRTW2W];
    ACTiming[AC_TIMING_XRTW2R] =  ACTimingTable[u1TimingIdx][AC_TIMING_XRTW2R];
    ACTiming[AC_TIMING_XRTR2W] =  ACTimingTable[u1TimingIdx][AC_TIMING_XRTR2W];
    ACTiming[AC_TIMING_XRTR2R] =  ACTimingTable[u1TimingIdx][AC_TIMING_XRTR2R];

    ACTiming[AC_TIMING_DMCATRAIN_INTV] =  ACTimingTable[u1TimingIdx][AC_TIMING_DMCATRAIN_INTV];

#if 0
    ACTiming[AC_TIMING_WRITE_LATENCY_0x41c] =  ACTimingTable[u1TimingIdx][AC_TIMING_WRITE_LATENCY_0x41c];
    ACTiming[AC_TIMING_WRITE_LATENCY_0x420] =  ACTimingTable[u1TimingIdx][AC_TIMING_WRITE_LATENCY_0x420];
    ACTiming[AC_TIMING_WRITE_LATENCY_0x424] =  ACTimingTable[u1TimingIdx][AC_TIMING_WRITE_LATENCY_0x424];
    ACTiming[AC_TIMING_WRITE_LATENCY_0x428] =  ACTimingTable[u1TimingIdx][AC_TIMING_WRITE_LATENCY_0x428];
    ACTiming[AC_TIMING_WRITE_LATENCY_0x42c] =  ACTimingTable[u1TimingIdx][AC_TIMING_WRITE_LATENCY_0x42c];
#endif
    ACTiming[AC_TIMING_DQSINCTL_FOR_GATING] =  ACTimingTable[u1TimingIdx][AC_TIMING_DQSINCTL_FOR_GATING];
    ACTiming[AC_TIMING_DATLAT] =  ACTimingTable[u1TimingIdx][AC_TIMING_DATLAT];
    ACTiming[AC_TIMING_MODE_REG_WL] =  ACTimingTable[u1TimingIdx][AC_TIMING_MODE_REG_WL];
    ACTiming[AC_TIMING_MODE_REG_RL] =  ACTimingTable[u1TimingIdx][AC_TIMING_MODE_REG_RL];

    DdrUpdateACTimingReg(p, ACTiming);

    return DRAM_OK;
}
#endif

/* PAD_RRESETB control sequence */
void vDramcInit_PreSettings(DRAMC_CTX_T *p)
{
    //remove twice dram reset pin pulse before dram power on sequence flow
    vIO32WriteFldMulti_All(DDRPHY_CA_CMD8, P_Fld(0x0, CA_CMD8_RG_TX_RRESETB_PULL_DN)
                                         | P_Fld(0x1, CA_CMD8_RG_TX_RRESETB_DDR3_SEL));
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 0x1, MISC_CTRL1_R_DMRRESETB_I_OPT); //Change to glitch-free path
    //replace DDRCONF0_GDDR3RST with MISC_CTRL1_R_DMDA_RRESETB_I
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 0x1, MISC_CTRL1_R_DMDA_RRESETB_I);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 0x1, MISC_CTRL1_R_DMDA_RRESETB_E);

    return;
}

DRAM_STATUS_T DramcInit(DRAMC_CTX_T *p)
{
    U32 save_ch, dram_t, highest_freq=0; //Darren
    #if (!__ETT__ && !FOR_DV_SIMULATION_USED && SW_CHANGE_FOR_SIMULATION==0)
    EMI_SETTINGS *emi_set; //Darren
    #endif
    U8 dram_cbt_mode;

    mcSHOW_DBG_MSG(("\n[DramcInit]\n"));
    mcFPRINTF((fp_A60501, "\n[DramcInit] Begin\n"));

    vSetPHY2ChannelMapping(p, CHANNEL_A);

#if WRITE_LEVELING_MOVE_DQS_INSTEAD_OF_CLK    
    memset(p->arfgWriteLevelingInitShif, FALSE, sizeof(p->arfgWriteLevelingInitShif));
#endif
#if TX_PERBIT_INIT_FLOW_CONTROL
    memset(p->fgTXPerbifInit, FALSE, sizeof(p->fgTXPerbifInit));
#endif

    vIO32Write4B_All(DRAMC_REG_ADDR(DDRPHY_MISC_SPM_CTRL3), 0); //SPM control with to SW control

    if(u1IsLP4Family(p->dram_type))
    {
        highest_freq = 1600; //Darren
        DramcSetting_Olympus_LP4_ByteMode(p);
    }
#if ENABLE_LP3_SW
    else if(p->dram_type == TYPE_LPDDR3)
    {
        highest_freq = 933; //Darren
        DramcSetting_Olympus_LP3(p);
    }
#endif

#if (!__ETT__ && !FOR_DV_SIMULATION_USED && SW_CHANGE_FOR_SIMULATION==0)
    if(p->frequency == highest_freq)
    {
        if(emi_setting_index == -1)
            emi_set = &default_emi_setting;
        else
            emi_set = &emi_settings[emi_setting_index];
        if(emi_set->AcTimeEMI.AC_TIME_EMI_FREQUENCY == p->frequency)
        {
            DdrUpdateACTiming_EMI(p, &(emi_set->AcTimeEMI));
        }
        else
        {
            mcSHOW_DBG_MSG(("[DdrUpdateACTiming_EMI] frequency not match (MDL=%d, target=%d), use inner table setting\n", emi_set->AcTimeEMI.AC_TIME_EMI_FREQUENCY, p->frequency));
            DdrUpdateACTiming(p);
        }
    }
    else
#endif
    {
        //if(u1IsLP4Family(p->dram_type) && (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1))  // tmp solution, need to take care normal mode and DBI
            DdrUpdateACTiming(p);
    }

#if 0  //update refresh rate
    // for free-run clk 26MHz, 0x62 * (1/26) = 3.8ns
    vIO32WriteFldAlign_All(DRAMC_REG_DRAMC_PD_CTRL, 0x62, DRAMC_PD_CTRL_REFCNT_FR_CLK);
    // for non-fre-run clk,  reg = 3.8 ns * f / 4 / 16;
    u4RefreshRate = 38*p->frequency/640;
    vIO32WriteFldAlign_All(DRAMC_REG_CONF2, u4RefreshRate, CONF2_REFCNT);
#endif

#if (fcFOR_CHIP_ID == fcBianco)
    // For kernel api for check LPDDR3/4/4X (Darren), only for fcOlympus and fcElbrus.
    // For Other chip, please confirm the register is free for SW use.
    save_ch = vGetPHY2ChannelMapping(p);
    vSetPHY2ChannelMapping(p, CHANNEL_A);

    switch (p->dram_type)
    {
#if ENABLE_LP3_SW
        case TYPE_LPDDR3:
            dram_t = 1;
            break;
#endif /* ENABLE_LP3_SW */
        case TYPE_LPDDR4:
            dram_t = 2;
            break;
        case TYPE_LPDDR4X:
            dram_t = 3;
            break;
        case TYPE_LPDDR4P:
            dram_t = 4;
            break;
        default:
            dram_t = 0;
            mcSHOW_ERR_MSG(("Incorrect DRAM Type!\n"));
            break;
    }
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_ARBCTL), dram_t, ARBCTL_RSV_DRAM_TYPE);

    // For DRAM normal, byte and mixed mode
    if ((p->dram_cbt_mode[RANK_0] == CBT_NORMAL_MODE) && (p->dram_cbt_mode[RANK_1] == CBT_NORMAL_MODE))
        dram_cbt_mode = CBT_R0_R1_NORMAL;
    else if ((p->dram_cbt_mode[RANK_0] == CBT_BYTE_MODE1) && (p->dram_cbt_mode[RANK_1] == CBT_BYTE_MODE1))
        dram_cbt_mode = CBT_R0_R1_BYTE;
    else if ((p->dram_cbt_mode[RANK_0] == CBT_NORMAL_MODE) && (p->dram_cbt_mode[RANK_1] == CBT_BYTE_MODE1))
        dram_cbt_mode = CBT_R0_NORMAL_R1_BYTE;
    else if ((p->dram_cbt_mode[RANK_0] == CBT_BYTE_MODE1) && (p->dram_cbt_mode[RANK_1] == CBT_NORMAL_MODE))
        dram_cbt_mode = CBT_R0_BYTE_R1_NORMAL;
    else
        dram_cbt_mode = CBT_R0_R1_NORMAL;
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RSTMASK), dram_cbt_mode, RSTMASK_RSV_DRAM_CBT_MIXED);
    
    // Sagy: Keep original setting till OS kernel ready, if ready, remove it
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_ARBCTL), (p->dram_cbt_mode[RANK_0]|p->dram_cbt_mode[RANK_1]), ARBCTL_RSV_DRAM_CBT);

    vSetPHY2ChannelMapping(p, save_ch);
#endif

    mcSHOW_DBG_MSG3(("[DramcInit] Done\n"));
    mcFPRINTF((fp_A60501, "[DramcInit] Done\n\n"));

    return DRAM_OK;
}



#if 0
DRAM_STATUS_T ApplySSCSettingReg(U8 u1Percent, U8 u1Slope, U8 u1Dir)
{
    U32 u4PRD, u4Delta, u4Delta1;

    u4PRD = 13000/u1Slope;
    u4Delta = ((76 << 18) *u1Percent)/ (u4PRD *100);
    u4Delta1 = u4Delta;

    #if EVEREST_PORTING_TODO
    vIO32WriteFldAlign(SYS_REG_ADDR(PLLGP_RG_02), u4PRD, RG_02_MEMPLL_SDM_SSC_PRD);
    vIO32WriteFldAlign(SYS_REG_ADDR(PLLGP_RG_03), u4Delta1, RG_03_MEMPLL_SDM_SSC_DELTA1);
    vIO32WriteFldAlign(SYS_REG_ADDR(PLLGP_RG_04), u4Delta, RG_04_MEMPLL_SDM_SSC_DELTA);
#endif

    mcSHOW_DBG_MSG2(("[ApplySSCSettingReg]\n"
                     "Percent %d, Slope %d kHz, Dir %d (Down)\n"
                     "PRD =(0x%x) , Delta (0x%x),  Delta1 (0x%x)\n",
                        u1Percent, u1Slope, u1Dir,
                        u4PRD, u4Delta, u4Delta1));

    return DRAM_OK;
}


DRAM_STATUS_T SscEnable(DRAMC_CTX_T *p)
{
    if (p->ssc_en == ENABLE)
    {
        mcSHOW_DBG_MSG(("Enable SSC\n"));
        mcFPRINTF((fp_A60501, "Enable SSC\n"));
        // RG_SYSPLL_SDM_SSC_EN = 1 (0x14[26])

#if EVEREST_PORTING_TODO
        vIO32WriteFldAlign(SYS_REG_ADDR(PLLGP_RG_11), 1, RG_11_SYSPLL_SDM_SSC_EN);
#endif
    }
    return DRAM_OK;
}
#endif

#if 0 //Comment out unused code
#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
void DramcEnterSelfRefresh(DRAMC_CTX_T *p, U8 op)
{
    U8 ucstatus = 0;
    U32 uiTemp;
    U32 u4TimeCnt;

    u4TimeCnt = TIME_OUT_CNT;

    mcSHOW_DBG_MSG(("[EnterSelfRefresh] %s\n", ((op == 1) ? "enter" : "exit")));
    mcFPRINTF((fp_A60501, "[EnterSelfRefresh] %s\n", ((op == 1) ? "enter" : "exit")));

    if (op == 1) // enter self refresh
    {
        if(u1IsLP4Family(p->dram_type))
        {
            // ONLY work for LP4, not LP3
            // MISCA_SRFPD_DIS =1, self-refresh
            // MISCA_SRFPD_DIS =0, self-refresh power down
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SREFCTRL), 1, SREFCTRL_SRFPD_DIS);
        }

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SREFCTRL), 1, SREFCTRL_SELFREF);
        mcDELAY_US(2);
        uiTemp = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MISC_STATUSA), MISC_STATUSA_SREF_STATE);
        while((uiTemp==0) &&(u4TimeCnt>0))
        {
            mcSHOW_DBG_MSG2(("Still not enter self refresh(%d)\n",u4TimeCnt));
            mcDELAY_US(1);
            uiTemp = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MISC_STATUSA), MISC_STATUSA_SREF_STATE);
            u4TimeCnt --;
        }
    }
    else // exit self refresh
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SREFCTRL), 0, SREFCTRL_SELFREF);

        mcDELAY_US(2);
        uiTemp = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MISC_STATUSA), MISC_STATUSA_SREF_STATE);
        while ((uiTemp!=0) &&(u4TimeCnt>0))
        {
            mcSHOW_DBG_MSG2(("Still not exit self refresh(%d)\n", u4TimeCnt));
            mcDELAY_US(1);
            uiTemp = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MISC_STATUSA), MISC_STATUSA_SREF_STATE);
            u4TimeCnt--;
        }
    }

   if(u4TimeCnt ==0)
   {
        mcSHOW_DBG_MSG(("Self refresh fail\n"));
        mcFPRINTF((fp_A60501, "Self refresh fail\n"));
   }
   else
   {
        mcSHOW_DBG_MSG(("Self refresh done\n"));
        mcFPRINTF((fp_A60501, "Self refresh done\n"));
   }
}
#endif
#endif //Comment out unused code


#ifdef DUMMY_READ_FOR_TRACKING
void DramcDummyReadForTrackingEnable(DRAMC_CTX_T *p)
{
    vIO32WriteFldAlign_All(DRAMC_REG_DUMMY_RD, p->support_rank_num, DUMMY_RD_RANK_NUM);
    /* Dummy read pattern (Better efficiency during rx dly tracking) DE: YH Tsai, Wei-jen */
    vIO32Write4B_All(DRAMC_REG_RK0_DUMMY_RD_WDATA0, 0xAAAA5555); // Field RK0_DUMMY_RD_WDATA0_DMY_RD_RK0_WDATA0
    vIO32Write4B_All(DRAMC_REG_RK0_DUMMY_RD_WDATA1, 0xAAAA5555); // Field RK0_DUMMY_RD_WDATA1_DMY_RD_RK0_WDATA1
    vIO32Write4B_All(DRAMC_REG_RK0_DUMMY_RD_WDATA2, 0xAAAA5555); // Field RK0_DUMMY_RD_WDATA2_DMY_RD_RK0_WDATA2
    vIO32Write4B_All(DRAMC_REG_RK0_DUMMY_RD_WDATA3, 0xAAAA5555); // Field RK0_DUMMY_RD_WDATA3_DMY_RD_RK0_WDATA3

    vIO32Write4B_All(DRAMC_REG_RK1_DUMMY_RD_WDATA0, 0xAAAA5555); // Field RK1_DUMMY_RD_WDATA0_DMY_RD_RK1_WDATA0
    vIO32Write4B_All(DRAMC_REG_RK1_DUMMY_RD_WDATA1, 0xAAAA5555); // Field RK1_DUMMY_RD_WDATA1_DMY_RD_RK1_WDATA1
    vIO32Write4B_All(DRAMC_REG_RK1_DUMMY_RD_WDATA2, 0xAAAA5555); // Field RK1_DUMMY_RD_WDATA2_DMY_RD_RK1_WDATA2
    vIO32Write4B_All(DRAMC_REG_RK1_DUMMY_RD_WDATA3, 0xAAAA5555); // Field RK1_DUMMY_RD_WDATA3_DMY_RD_RK1_WDATA3
 
    vIO32WriteFldAlign_All(DRAMC_REG_TEST2_4, 4, TEST2_4_TESTAGENTRKSEL);//Dummy Read rank selection is controlled by Test Agent
    vIO32WriteFldAlign_All(DRAMC_REG_RK1_DUMMY_RD_ADR, 0, RK1_DUMMY_RD_ADR_DMY_RD_RK1_LEN);
    vIO32WriteFldAlign_All(DRAMC_REG_RK0_DUMMY_RD_ADR, 0, RK0_DUMMY_RD_ADR_DMY_RD_RK0_LEN);
    vIO32WriteFldMulti_All(DRAMC_REG_DUMMY_RD, P_Fld(1, DUMMY_RD_DMY_RD_RX_TRACK) | P_Fld(1, DUMMY_RD_DUMMY_RD_EN));

    return;
}
#endif

#ifdef IMPEDANCE_TRACKING_ENABLE
void DramcImpedanceHWSaving(DRAMC_CTX_T *p)
{
    vIO32WriteFldAlign_All(DRAMC_REG_IMPCAL, 1, IMPCAL_IMPCAL_HW_SAVE_ENABLE);
    vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL0, 1, REFCTRL0_DQDRVSWUPD);
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_IMPCAL1, 3, SHU_IMPCAL1_IMPCALCNT);  
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_SELPH_CA2, 0x7, SHU_SELPH_CA2_TXDLY_CMD);
}
void DramcImpedanceTrackingEnable(DRAMC_CTX_T *p)
{
    //Write (DRAMC _BASE+ 0x8B) [31:0] = 32'he4000000//enable impedance tracking
    vIO32WriteFldAlign(DDRPHY_MISC_CTRL0, 0x1, MISC_CTRL0_IMPCAL_CHAB_EN);//Set CHA this bit to enable dual channel tracking

    vIO32WriteFldMulti_All(DRAMC_REG_IMPCAL, P_Fld(1, IMPCAL_IMPCAL_HW) | P_Fld(0, IMPCAL_IMPCAL_EN) | P_Fld(1, IMPCAL_IMPCAL_SWVALUE_EN) | P_Fld(1, IMPCAL_IMPCAL_NEW_OLD_SL));

    // no update imp CA, because CA is unterm now
    vIO32WriteFldAlign_All(DRAMC_REG_IMPCAL, 1, IMPCAL_IMPCAL_BYPASS_UP_CA_DRV);   

    //vIO32WriteFldAlign(DRAMC_REG_REFCTRL0+((U32)CHANNEL_B<<POS_BANK_NUM), 1, REFCTRL0_DRVCGWREF);
    vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL0, 1, REFCTRL0_DRVCGWREF);
    vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL0, 1, REFCTRL0_DQDRVSWUPD); 
}
#endif
#if TDQSCK_PRECALCULATION_FOR_DVFS
/* divRoundClosest() - to round up to the nearest integer
 * discard four, but treat five as whole (of decimal points)
 */
int divRoundClosest(const int n, const int d)
{
  return ((n < 0) ^ (d < 0)) ? ((n - d/2)/d) : ((n + d/2)/d);
}


void DramcDQSPrecalculation_preset(DRAMC_CTX_T *p)//Test tDQSCK_temp Pre-calculation
{
    DRAM_CHANNEL_T eOriChannel = p->channel;
    DRAM_RANK_T eOriRank = p->rank;
    int irank_num = 0, ichannel_num = 0;
    U32 u4value = 0;
    U32 u4value1 = 0;
    int shuffle_src_index,shuffle_dst_index,jump_ratio_index;
    U16 u2JumpRatio[12] = {0}; /* Used to record "jump ratio" calulation results */

    /* Calculate jump ratios and save to u2JumpRatio array */
    jump_ratio_index=0;
    for(shuffle_src_index=DRAM_DFS_SHUFFLE_1; shuffle_src_index<DRAM_DFS_SHUFFLE_MAX; shuffle_src_index++)
    {
        for(shuffle_dst_index=DRAM_DFS_SHUFFLE_1; shuffle_dst_index<DRAM_DFS_SHUFFLE_MAX; shuffle_dst_index++)            
        {
            if (shuffle_src_index == shuffle_dst_index)
            {
                continue;
            }
            u2JumpRatio[jump_ratio_index] = divRoundClosest((p->shuffle_frequency[shuffle_dst_index] * 32), p->shuffle_frequency[shuffle_src_index]);
            mcSHOW_DBG_MSG3(("shuffle_%d=DDR%d / shuffle_%d=DDR%d \n", shuffle_dst_index, p->shuffle_frequency[shuffle_dst_index]<<1,
                                                                        shuffle_src_index, p->shuffle_frequency[shuffle_src_index]<<1));
            mcSHOW_DBG_MSG3(("Jump_RATIO_%d : 0x%x\n", jump_ratio_index, u2JumpRatio[jump_ratio_index],
                                                        p->shuffle_frequency[shuffle_src_index]));
            jump_ratio_index++;
        }
    }

    for(ichannel_num=0; ichannel_num<p->support_channel_num; ichannel_num++)
    {
        vSetPHY2ChannelMapping(p, ichannel_num);  
        
        //mcSHOW_DBG_MSG(("Step1: Set DVFS HW enable\n"));
        //Step1: Set DVFS HW enable
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PRE_TDQSCK1), 1, PRE_TDQSCK1_TDQSCK_PRECAL_HW);

        //mcSHOW_DBG_MSG(("Step2: Set jump ratio\n"));
        //Step2: Set jump ratio
/*
    F1:3200
    F2:2667
    F3:2667
    F4:800
    
    Name            DVFS setting        Location    Value
    Jump_RATIO_0    00 to 01(F1 to F2)  0x87[31:24] 01(Freq)/00(freq)  * 0x20---1A
    Jump_RATIO_1    00 to 10(F1 to F3)  0x87[23:16] 10(Freq)/00(freq)  * 0x20---1A
    Jump_RATIO_2    00 to 11(F1 to F4)  0x87[15:8]  11(Freq)/00(freq)  * 0x20---8
    Jump_RATIO_3    01 to 00(F2 to F1)  0x87[7:0]   00(Freq)/01(freq)  * 0x20---26
    Jump_RATIO_4    01 to 10(F2 to F3)  0x88[31:24] 10(Freq)/01(freq)  * 0x20---20
    Jump_RATIO_5    01 to 11(F2 to F4)  0x88[23:16] 11(Freq)/01(freq)  * 0x20---9
    Jump_RATIO_6    10 to 00(F3 to F1)  0x88[15:8]  00(Freq)/10(freq)  * 0x20---26
    Jump_RATIO_7    10 to 01(F3 to F2)  0x88[7:0]   01(Freq)/10(freq)  * 0x20---20
    Jump_RATIO_8    10 to 11(F3 to F4)  0x89[31:24] 11(Freq)/10(freq)  * 0x20---9
    Jump_RATIO_9    11 to 00(F4 to F1)  0x89[23:16] 00(Freq)/11(freq)  * 0x20---80
    Jump_RATIO_10   11 to 01(F4 to F2)  0x89[15:8]  01(Freq)/11(freq)  * 0x20---6A
    Jump_RATIO_11   11 to 10(F4 to F3)  0x89[7:0]   10(Freq)/11(freq)  * 0x20---6A
*/    
        /* Save jumpRatios into corresponding register fields */
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_PRE_TDQSCK2), P_Fld(u2JumpRatio[0], PRE_TDQSCK2_TDDQSCK_JUMP_RATIO0)
                                                                | P_Fld(u2JumpRatio[1], PRE_TDQSCK2_TDDQSCK_JUMP_RATIO1)
                                                                | P_Fld(u2JumpRatio[2], PRE_TDQSCK2_TDDQSCK_JUMP_RATIO2)
                                                                | P_Fld(u2JumpRatio[3], PRE_TDQSCK2_TDDQSCK_JUMP_RATIO3));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_PRE_TDQSCK3), P_Fld(u2JumpRatio[4], PRE_TDQSCK3_TDDQSCK_JUMP_RATIO4)
                                                                | P_Fld(u2JumpRatio[5], PRE_TDQSCK3_TDDQSCK_JUMP_RATIO5)
                                                                | P_Fld(u2JumpRatio[6], PRE_TDQSCK3_TDDQSCK_JUMP_RATIO6)
                                                                | P_Fld(u2JumpRatio[7], PRE_TDQSCK3_TDDQSCK_JUMP_RATIO7));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_PRE_TDQSCK4), P_Fld(u2JumpRatio[8], PRE_TDQSCK4_TDDQSCK_JUMP_RATIO8)
                                                                | P_Fld(u2JumpRatio[9], PRE_TDQSCK4_TDDQSCK_JUMP_RATIO9)
                                                                | P_Fld(u2JumpRatio[10], PRE_TDQSCK4_TDDQSCK_JUMP_RATIO10)
                                                                | P_Fld(u2JumpRatio[11], PRE_TDQSCK4_TDDQSCK_JUMP_RATIO11));

        for(irank_num=0; irank_num<2; irank_num++)
        {
            vSetRank(p,irank_num);
        
            //mcSHOW_DBG_MSG(("Step3: Write UI/PI info to RG\n"));
            //Step3: Write UI/PI info to RG
//B0 ==========================            
            //Shuffle 0 =========================================
            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK1), (u4value << 3) | u4value1, RK0_PRE_TDQSCK1_TDQSCK_UIFREQ1_B0R0);//0xcc[5:0]

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQSIEN), SHURK0_DQSIEN_R0DQS0IEN);   
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK1), u4value, RK0_PRE_TDQSCK1_TDQSCK_PIFREQ1_B0R0);//0xcc[12:6]

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK3), (u4value << 3) | u4value1, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ1_P1_B0R0);//0xce[5:0]
            
            //Shuffle 1 =========================================
            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK1), (u4value << 3) | u4value1, RK0_PRE_TDQSCK1_TDQSCK_UIFREQ2_B0R0);//0xcc[18:13] 

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_DQSIEN), SHURK0_DQSIEN_R0DQS0IEN);   
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK1), u4value, RK0_PRE_TDQSCK1_TDQSCK_PIFREQ2_B0R0);//0xcc[25:19]

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK3), (u4value << 3) | u4value1, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ2_P1_B0R0);//0xce[11:6]            
            
            //Shuffle 2 =========================================
            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK2), (u4value << 3) | u4value1, RK0_PRE_TDQSCK2_TDQSCK_UIFREQ3_B0R0);//0xcd[5:0] 

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_DQSIEN), SHURK0_DQSIEN_R0DQS0IEN);   
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK2), u4value, RK0_PRE_TDQSCK2_TDQSCK_PIFREQ3_B0R0);//0xcd[12:6]

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK3), (u4value << 3) | u4value1, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ3_P1_B0R0);//0xce[17:12]                        
            
//B1 ==========================            
            //Shuffle 0 =========================================
            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK4), (u4value << 3) | u4value1, RK0_PRE_TDQSCK4_TDQSCK_UIFREQ1_B1R0);//0xcf[5:0]

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQSIEN), SHURK0_DQSIEN_R0DQS1IEN);   
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK4), u4value, RK0_PRE_TDQSCK4_TDQSCK_PIFREQ1_B1R0);//0xcf[12:6]

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK6), (u4value << 3) | u4value1, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ1_P1_B1R0);//0xd1[5:0]
            
            //Shuffle 1 =========================================
            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK4), (u4value << 3) | u4value1, RK0_PRE_TDQSCK4_TDQSCK_UIFREQ2_B1R0);//0xcf[18:13] 

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_DQSIEN), SHURK0_DQSIEN_R0DQS1IEN);   
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK4), u4value, RK0_PRE_TDQSCK4_TDQSCK_PIFREQ2_B1R0);//0xcf[25:19]

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK6), (u4value << 3) | u4value1, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ2_P1_B1R0);//0xd1[11:6]            
            
            //Shuffle 2 =========================================
            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK5), (u4value << 3) | u4value1, RK0_PRE_TDQSCK5_TDQSCK_UIFREQ3_B1R0);//0xd0[5:0] 

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_DQSIEN), SHURK0_DQSIEN_R0DQS1IEN);   
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK5), u4value, RK0_PRE_TDQSCK5_TDQSCK_PIFREQ3_B1R0);//0xd0[12:6]

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK6), (u4value << 3) | u4value1, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ3_P1_B1R0);//0xd1[17:12]                        
            
//if Project is combo LP3+LP4, then needs to set B2 and B3
//B2 ==========================            
            //Shuffle 0 =========================================
            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK7), (u4value << 3) | u4value1, RK0_PRE_TDQSCK7_TDQSCK_UIFREQ1_B2R0);//0xcf[5:0]

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQSIEN), SHURK0_DQSIEN_R0DQS2IEN);   
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK7), u4value, RK0_PRE_TDQSCK7_TDQSCK_PIFREQ1_B2R0);//0xcf[12:6]

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED_P1);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED_P1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK9), (u4value << 3) | u4value1, RK0_PRE_TDQSCK9_TDQSCK_UIFREQ1_P1_B2R0);//0xd1[5:0]

            //Shuffle 1 =========================================
            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK7), (u4value << 3) | u4value1, RK0_PRE_TDQSCK7_TDQSCK_UIFREQ2_B2R0);//0xcf[18:13] 

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_DQSIEN), SHURK0_DQSIEN_R0DQS2IEN);   
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK7), u4value, RK0_PRE_TDQSCK7_TDQSCK_PIFREQ2_B2R0);//0xcf[25:19]

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED_P1);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED_P1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK9), (u4value << 3) | u4value1, RK0_PRE_TDQSCK9_TDQSCK_UIFREQ2_P1_B2R0);//0xd1[11:6]            

            //Shuffle 2 =========================================
            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK8), (u4value << 3) | u4value1, RK0_PRE_TDQSCK8_TDQSCK_UIFREQ3_B2R0);//0xd0[5:0] 

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_DQSIEN), SHURK0_DQSIEN_R0DQS2IEN);   
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK8), u4value, RK0_PRE_TDQSCK8_TDQSCK_PIFREQ3_B2R0);//0xd0[12:6]

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED_P1);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED_P1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK9), (u4value << 3) | u4value1, RK0_PRE_TDQSCK9_TDQSCK_UIFREQ3_P1_B2R0);//0xd1[17:12]                        
            
//B3 ==========================            
            //Shuffle 0 =========================================
            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK10), (u4value << 3) | u4value1, RK0_PRE_TDQSCK10_TDQSCK_UIFREQ1_B3R0);//0xcf[5:0]

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQSIEN), SHURK0_DQSIEN_R0DQS3IEN);   
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK10), u4value, RK0_PRE_TDQSCK10_TDQSCK_PIFREQ1_B3R0);//0xcf[12:6]

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED_P1);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED_P1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK12), (u4value << 3) | u4value1, RK0_PRE_TDQSCK12_TDQSCK_UIFREQ1_P1_B3R0);//0xd1[5:0]

            //Shuffle 1 =========================================
            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK10), (u4value << 3) | u4value1, RK0_PRE_TDQSCK10_TDQSCK_UIFREQ2_B3R0);//0xcf[18:13] 

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_DQSIEN), SHURK0_DQSIEN_R0DQS3IEN);   
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK10), u4value, RK0_PRE_TDQSCK10_TDQSCK_PIFREQ2_B3R0);//0xcf[25:19]

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED_P1);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED_P1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK12), (u4value << 3) | u4value1, RK0_PRE_TDQSCK12_TDQSCK_UIFREQ2_P1_B3R0);//0xd1[11:6]            

            //Shuffle 2 =========================================
            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK11), (u4value << 3) | u4value1, RK0_PRE_TDQSCK11_TDQSCK_UIFREQ3_B3R0);//0xd0[5:0] 

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_DQSIEN), SHURK0_DQSIEN_R0DQS3IEN);   
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK11), u4value, RK0_PRE_TDQSCK11_TDQSCK_PIFREQ3_B3R0);//0xd0[12:6]

            u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED_P1);
            u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED_P1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK12), (u4value << 3) | u4value1, RK0_PRE_TDQSCK12_TDQSCK_UIFREQ3_P1_B3R0);//0xd1[17:12]                                    
         }    
        //mcSHOW_DBG_MSG((" Select HW flow\n"));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_PRE_TDQSCK1), P_Fld(0x1, PRE_TDQSCK1_TDQSCK_REG_DVFS));
        //Step4: Set Auto save to RG
        //mcSHOW_DBG_MSG(("Step 4: Select HW flow\n"));
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PRE_TDQSCK1), 1, PRE_TDQSCK1_TDQSCK_HW_SW_UP_SEL);

    }
    vSetPHY2ChannelMapping(p, eOriChannel);
    vSetRank(p, eOriRank);
    return;
}
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
    U32 backupRegShu1_SCINTV[CHANNEL_NUM], backupRegShu2_SCINTV[CHANNEL_NUM], backupRegShu3_SCINTV[CHANNEL_NUM], backupRegShu4_SCINTV[CHANNEL_NUM];
    U32 channel_idx, backup_channel;

    backup_channel = vGetPHY2ChannelMapping(p);
    
    for(channel_idx=CHANNEL_A; channel_idx<p->support_channel_num; channel_idx++)
    {
        vSetPHY2ChannelMapping(p, channel_idx);
        backupReg0x64[channel_idx] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL));
        backupReg0xC8[channel_idx] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR));
        backupReg0xD0[channel_idx] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD));
        backupRegShu1_SCINTV[channel_idx] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SHU_SCINTV));
        backupRegShu2_SCINTV[channel_idx] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SHU2_SCINTV));
        backupRegShu3_SCINTV[channel_idx] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SHU3_SCINTV));
    }
    vSetPHY2ChannelMapping(p, backup_channel);

    //Disable MR4 MR18/MR19, TxHWTracking, Dummy RD before reset
    vIO32WriteFldAlign_All(DRAMC_REG_SPCMDCTRL, 0x1, SPCMDCTRL_REFRDIS);    //MR4 Disable
    vIO32WriteFldAlign_All(DRAMC_REG_DQSOSCR, 0x1, DQSOSCR_DQSOSCRDIS);  //MR18, MR19 Disable
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_SCINTV, 0x1, SHU_SCINTV_DQSOSCENDIS);
    vIO32WriteFldAlign_All(DRAMC_REG_SHU2_SCINTV, 0x1, SHU2_SCINTV_DQSOSCENDIS);
    vIO32WriteFldAlign_All(DRAMC_REG_SHU3_SCINTV, 0x1, SHU3_SCINTV_DQSOSCENDIS);
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
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU_SCINTV), backupRegShu1_SCINTV[channel_idx]);
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU2_SCINTV), backupRegShu2_SCINTV[channel_idx]);
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU3_SCINTV), backupRegShu3_SCINTV[channel_idx]);
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

    mpdivInSel= u4IO32ReadFldAlign(DDRPHY_MISC_CTRL2, MISC_CTRL2_PLL_SHU_GP);
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

void vAutoRefreshSwitch(DRAMC_CTX_T *p, U8 option)
{
    if (option == ENABLE)
    {
        //enable autorefresh
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0), 0, REFCTRL0_REFDIS);      //REFDIS=0, enable auto refresh        
    }
    else
    {
        //disable autorefresh
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0), 1, REFCTRL0_REFDIS);      //REFDIS=1, disable auto refresh    

        //because HW will actually disable autorefresh after refresh_queue empty, so we need to wait quene empty.
        mcDELAY_US(u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MISC_STATUSA), MISC_STATUSA_REFRESH_QUEUE_CNT)*4);   //wait refresh_queue_cnt * 3.9us
    }
}

void DramcDebugTest(DRAMC_CTX_T *p)
{
#if 0
    U32 u4ErrorValue;

    // TestEngine2 R/W test
    p->test2_1 = DEFAULT_TEST2_1_CAL;
    u4ErrorValue = TestEngineCompare(p);
    mcSHOW_ERR_MSG(("[DramcDebugTest] First R/W test. Rank0: u4ErrorValue 0x%X\n", u4ErrorValue));
    #if DUAL_RANK_ENABLE
    if (p->support_rank_num==RANK_DUAL)
    {
        p->test2_1 = 0x55a00000;
        u4ErrorValue = TestEngineCompare(p);
        mcSHOW_ERR_MSG(("[DramcDebugTest] First R/W test. Rank1: u4ErrorValue 0x%X\n", u4ErrorValue));
    }
    #endif

    // Reset PHY
    DramPhyReset(p);

    // TestEngine2 R/W test
    p->test2_1 = DEFAULT_TEST2_1_CAL;
    u4ErrorValue = TestEngineCompare(p);
    mcSHOW_ERR_MSG(("[DramcDebugTest] Reset PHY R/W test. Rank0: u4ErrorValue 0x%X\n", u4ErrorValue));

    #if DUAL_RANK_ENABLE
    if (p->support_rank_num==RANK_DUAL)
    {
        p->test2_1 = 0x55a00000;
        u4ErrorValue = TestEngineCompare(p);
        mcSHOW_ERR_MSG(("[DramcDebugTest] Reset PHY R/W test . Rank1:u4ErrorValue 0x%X\n", u4ErrorValue));
    }
    #endif

    // Reset Dram
    Dram_Reset(p);

    // TestEngine2 R/W test
    p->test2_1 = DEFAULT_TEST2_1_CAL;
    u4ErrorValue = TestEngineCompare(p);
    mcSHOW_ERR_MSG(("[DramcDebugTest] Reset Dram R/W test. Rank0: u4ErrorValue 0x%X\n", u4ErrorValue));

    #if DUAL_RANK_ENABLE
    if (p->support_rank_num==RANK_DUAL)
    {
        p->test2_1 = 0x55a00000;
        u4ErrorValue = TestEngineCompare(p);
        mcSHOW_ERR_MSG(("[DramcDebugTest] Reset Dram R/W test . Rank1:u4ErrorValue 0x%X\n", u4ErrorValue));
    }
    #endif

    p->test2_1 = DEFAULT_TEST2_1_CAL;
    #endif
}


void TransferToSPMControl(DRAMC_CTX_T *p)
{
    U32 u4WbrBackup = 0;

    if(u1IsLP4Family(p->dram_type))
    {
        vIO32WriteFldAlign_All(DDRPHY_MISC_SPM_CTRL0, 0xfbffefff, MISC_SPM_CTRL0_PHY_SPM_CTL0);
        vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL2, 0xffffffef, MISC_SPM_CTRL2_PHY_SPM_CTL2);
        vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL2 + ((U32)CHANNEL_B<<POS_BANK_NUM), 0x7fffffef, MISC_SPM_CTRL2_PHY_SPM_CTL2);//Lewis@20170627: Set CHB CA DLL type to slave mode 
    }
    else //LP3
    {
#if ENABLE_LP3_SW
        vIO32WriteFldAlign_All(DDRPHY_MISC_SPM_CTRL0, 0xfbffefff, MISC_SPM_CTRL0_PHY_SPM_CTL0);
        vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL2, 0xffffffef, MISC_SPM_CTRL2_PHY_SPM_CTL2);
        vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL2 + ((U32)CHANNEL_B<<POS_BANK_NUM), 0x7fffffef, MISC_SPM_CTRL2_PHY_SPM_CTL2);//Lewis@20170731: Set CHB CA DLL type to slave mode since CHB CA is data in LP3 
#endif
    }
    //vIO32WriteFldAlign_All(DDRPHY_MISC_SPM_CTRL0, 0xfffffe7f, MISC_SPM_CTRL0_PHY_SPM_CTL0);//Lewis@20160505: PLL control is by SW RG no matter Low power
    //vIO32WriteFldAlign_All(DDRPHY_MISC_SPM_CTRL2, 0xffffffff, MISC_SPM_CTRL2_PHY_SPM_CTL2);
    //vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL2 + ((U32)CHANNEL_B<<POS_BANK_NUM), 0x7fffffff, MISC_SPM_CTRL2_PHY_SPM_CTL2);//Lewis@20160805: Change CHB CA DLL to slave mode, fix LP4 LPM(DDR1600) MD EE in S0(suspend, deep idle, SODI)
    return;
}

void TransferPLLToSPMControl(DRAMC_CTX_T *p)
{
#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)    
    U8 shu_level ;
    shu_level = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHUSTATUS), SHUSTATUS_SHUFFLE_LEVEL);
    
    /*TINFO="DRAM : enter SW DVFS"*/

    /*TINFO="DRAM : SPM presetting for pinmux"*/
    //! set SPM project code and enable clock enable
    *((volatile unsigned int *)(SPM_POWERON_CONFIG_EN)) = 0xB16 << 16 | 0x1;

    //! set SPM pinmux
    *((volatile unsigned int *)(SPM_PCM_PWR_IO_EN))     = 0;

    *((volatile unsigned int *)(SPM_DRAMC_DPY_CLK_SW_CON_SEL))  = 0xffffffff;
    *((volatile unsigned int *)(SPM_DRAMC_DPY_CLK_SW_CON_SEL2)) = 0xffffffff;

    //! set  sc_dpy_2nd_dll_en, sc_dpy_dll_en, sc_dpy_dll_ck_en ,sc_dpy_vref_en , sc_phypll_en = 1
    *((volatile unsigned int *)(SPM_POWER_ON_VAL0))      |= (0x1 << 8) | (0x1 << 12) | (0x1 << 13) | (0x1 << 14) | (0x1 << 15) ;

    vIO32WriteFldAlign(SPM_S1_MODE_CH, 3, S1_MODE_CH_SPM_S1_MODE_CH);

    mcSHOW_DBG_MSG(("TransferPLLToSPMControl - MODE SW "));
    if(shu_level==1)
    {
        mcSHOW_DBG_MSG(("CLRPLL\n"));
        *((volatile unsigned int *)(SPM_POWER_ON_VAL0)) &= ~(0x1<<28);
        *((volatile unsigned int *)(SPM_POWER_ON_VAL0)) |= (0x1<<29);
        *((volatile unsigned int *)(SPM_DRAMC_DPY_CLK_SW_CON2)) &= ~(0x1<<2);
        *((volatile unsigned int *)(SPM_DRAMC_DPY_CLK_SW_CON2)) |= (0x1<<3);
    }
    else
    {
        mcSHOW_DBG_MSG(("PHYPLL\n"));
        *((volatile unsigned int *)(SPM_POWER_ON_VAL0)) &= ~(0x1<<29);
        *((volatile unsigned int *)(SPM_POWER_ON_VAL0)) |= (0x1<<28);
        *((volatile unsigned int *)(SPM_DRAMC_DPY_CLK_SW_CON2)) &= ~(0x1<<3);
        *((volatile unsigned int *)(SPM_DRAMC_DPY_CLK_SW_CON2)) |= (0x1<<2);
    }
    mcDELAY_US(1);
    vIO32WriteFldAlign_All(DDRPHY_PLL1, 0x0, PLL1_RG_RPHYPLL_EN);
    vIO32WriteFldAlign_All(DDRPHY_PLL2, 0x0, PLL2_RG_RCLRPLL_EN);
#endif

    return;
}


void TransferToRegControl(void)
{
    /* Chen-Hsiang@20160323: After leave preloader and low power scenario, conf is controller by RG*/
}

void Switch26MHzDisableDummyReadRefreshAllBank(DRAMC_CTX_T *p)
{
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_CLK_MEM_SEL);//Switch clk to 26MHz
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_W_CHG_MEM);
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_W_CHG_MEM);

    vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL0, 0, REFCTRL0_PBREFEN);//Switch to all bank refresh

    vIO32WriteFldMulti_All(DRAMC_REG_DUMMY_RD, P_Fld(0x0, DUMMY_RD_DQSG_DMYWR_EN)//Disable Dummy Read
                | P_Fld(0x0, DUMMY_RD_DQSG_DMYRD_EN) | P_Fld(0x0, DUMMY_RD_SREF_DMYRD_EN)
                | P_Fld(0x0, DUMMY_RD_DUMMY_RD_EN) | P_Fld(0x0, DUMMY_RD_DMY_RD_DBG)
                | P_Fld(0x0, DUMMY_RD_DMY_WR_DBG));
    return;
}

#define DUMP_GOLDEN_SETTING 0
#if DUMP_GOLDEN_SETTING
void DumpGoldenSetting()
{
    U32 u4Value = 0;
    
    mcSHOW_DBG_MSG(("*** DCM related \n"));
    u4Value = (*(volatile unsigned int *)(0x1022A038));
    mcSHOW_DBG_MSG(("0x1022A038[2] = 0x%X \n", (u4Value >> 2) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x10232038));
    mcSHOW_DBG_MSG(("0x10232038[2] = 0x%X \n", (u4Value >> 2) & 0x1));

    u4Value = (*(volatile unsigned int *)(0x1022A038));
    mcSHOW_DBG_MSG(("0x1022A038[31] = 0x%X \n", (u4Value >> 31) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x10232038));
    mcSHOW_DBG_MSG(("0x10232038[31] = 0x%X \n", (u4Value >> 31) & 0x1));

    u4Value = (*(volatile unsigned int *)(0x1022A038));
    mcSHOW_DBG_MSG(("0x1022A038[30] = 0x%X \n", (u4Value >> 30) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x10232038));
    mcSHOW_DBG_MSG(("0x10232038[30] = 0x%X \n", (u4Value >> 30) & 0x1));

    u4Value = (*(volatile unsigned int *)(0x1022A038));
    mcSHOW_DBG_MSG(("0x1022A038[0] = 0x%X \n", (u4Value >> 0) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x10232038));
    mcSHOW_DBG_MSG(("0x10232038[0] = 0x%X \n", (u4Value >> 0) & 0x1));

    u4Value = (*(volatile unsigned int *)(0x1022A038));
    mcSHOW_DBG_MSG(("0x1022A038[5] = 0x%X \n", (u4Value >> 5) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x10232038));
    mcSHOW_DBG_MSG(("0x10232038[5] = 0x%X \n", (u4Value >> 5) & 0x1));

    u4Value = (*(volatile unsigned int *)(0x1022A038));
    mcSHOW_DBG_MSG(("0x1022A038[1] = 0x%X \n", (u4Value >> 1) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x10232038));
    mcSHOW_DBG_MSG(("0x10232038[1] = 0x%X \n", (u4Value >> 1) & 0x1));

    u4Value = (*(volatile unsigned int *)(0x10228284));
    mcSHOW_DBG_MSG(("0x10228284[19][17:8] = 0x%X, 0x%X\n", (u4Value >> 19) & 0x1, (u4Value >> 8) & 0x3ff));
    u4Value = (*(volatile unsigned int *)(0x10230284));
    mcSHOW_DBG_MSG(("0x10230284[19][17:8] = 0x%X, 0x%X\n", (u4Value >> 19) & 0x1, (u4Value >> 8) & 0x3ff));

    u4Value = (*(volatile unsigned int *)(0x10228284));
    mcSHOW_DBG_MSG(("0x10228284[20] = 0x%X\n", (u4Value >> 20) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x10230284));
    mcSHOW_DBG_MSG(("0x10230284[20] = 0x%X\n", (u4Value >> 20) & 0x1));

    u4Value = (*(volatile unsigned int *)(0x1022828C));
    mcSHOW_DBG_MSG(("0x1022828C = 0x%X\n", u4Value));
    u4Value = (*(volatile unsigned int *)(0x1023028C));
    mcSHOW_DBG_MSG(("0x1023028C = 0x%X\n", u4Value));

    u4Value = (*(volatile unsigned int *)(0x102282a8));
    mcSHOW_DBG_MSG(("0x102282a8[26] = 0x%X\n", (u4Value >> 26) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x102302a8));
    mcSHOW_DBG_MSG(("0x102302a8[26] = 0x%X\n", (u4Value >> 26) & 0x1));

    u4Value = (*(volatile unsigned int *)(0x102282a8));
    mcSHOW_DBG_MSG(("0x102282a8[27] = 0x%X\n", (u4Value >> 27) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x102302a8));
    mcSHOW_DBG_MSG(("0x102302a8[27] = 0x%X\n", (u4Value >> 27) & 0x1));

    u4Value = (*(volatile unsigned int *)(0x10228c20));
    mcSHOW_DBG_MSG(("0x10228c20[31:20][19] = 0x%X, 0x%X\n", (u4Value >> 20) & 0xfff, (u4Value >> 19) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x10229120));
    mcSHOW_DBG_MSG(("0x10229120[31:20][19] = 0x%X, 0x%X\n", (u4Value >> 20) & 0xfff, (u4Value >> 19) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x10229620));
    mcSHOW_DBG_MSG(("0x10229620[31:20][19] = 0x%X, 0x%X\n", (u4Value >> 20) & 0xfff, (u4Value >> 19) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x10230c20));
    mcSHOW_DBG_MSG(("0x10230c20[31:20][19] = 0x%X, 0x%X\n", (u4Value >> 20) & 0xfff, (u4Value >> 19) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x10231120));
    mcSHOW_DBG_MSG(("0x10231120[31:20][19] = 0x%X, 0x%X\n", (u4Value >> 20) & 0xfff, (u4Value >> 19) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x10231620));
    mcSHOW_DBG_MSG(("0x10231620[31:20][19] = 0x%X, 0x%X\n", (u4Value >> 20) & 0xfff, (u4Value >> 19) & 0x1));

    u4Value = (*(volatile unsigned int *)(0x10228cA0));
    mcSHOW_DBG_MSG(("0x10228cA0[31:20][19] = 0x%X, 0x%X\n", (u4Value >> 20) & 0xfff, (u4Value >> 19) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x102291A0));
    mcSHOW_DBG_MSG(("0x102291A0[31:20][19] = 0x%X, 0x%X\n", (u4Value >> 20) & 0xfff, (u4Value >> 19) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x102296A0));
    mcSHOW_DBG_MSG(("0x102296A0[31:20][19] = 0x%X, 0x%X\n", (u4Value >> 20) & 0xfff, (u4Value >> 19) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x10230cA0));
    mcSHOW_DBG_MSG(("0x10230cA0[31:20][19] = 0x%X, 0x%X\n", (u4Value >> 20) & 0xfff, (u4Value >> 19) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x102311A0));
    mcSHOW_DBG_MSG(("0x102311A0[31:20][19] = 0x%X, 0x%X\n", (u4Value >> 20) & 0xfff, (u4Value >> 19) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x102316A0));
    mcSHOW_DBG_MSG(("0x102316A0[31:20][19] = 0x%X, 0x%X\n", (u4Value >> 20) & 0xfff, (u4Value >> 19) & 0x1));

    u4Value = (*(volatile unsigned int *)(0x10228D20));
    mcSHOW_DBG_MSG(("0x10228D20[31:20][19] = 0x%X, 0x%X\n", (u4Value >> 20) & 0xfff, (u4Value >> 19) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x10229220));
    mcSHOW_DBG_MSG(("0x10229220[31:20][19] = 0x%X, 0x%X\n", (u4Value >> 20) & 0xfff, (u4Value >> 19) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x10229720));
    mcSHOW_DBG_MSG(("0x10229720[31:20][19] = 0x%X, 0x%X\n", (u4Value >> 20) & 0xfff, (u4Value >> 19) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x10230D20));
    mcSHOW_DBG_MSG(("0x10230D20[31:20][19] = 0x%X, 0x%X\n", (u4Value >> 20) & 0xfff, (u4Value >> 19) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x10231220));
    mcSHOW_DBG_MSG(("0x10231220[31:20][19] = 0x%X, 0x%X\n", (u4Value >> 20) & 0xfff, (u4Value >> 19) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x10231720));
    mcSHOW_DBG_MSG(("0x10231720[31:20][19] = 0x%X, 0x%X\n", (u4Value >> 20) & 0xfff, (u4Value >> 19) & 0x1));

    u4Value = (*(volatile unsigned int *)(0x10228298));
    mcSHOW_DBG_MSG(("0x10228298[18:16] = 0x%X\n", (u4Value >> 16) & 0x7));
    u4Value = (*(volatile unsigned int *)(0x10230298));
    mcSHOW_DBG_MSG(("0x10230298[18:16] = 0x%X\n", (u4Value >> 16) & 0x7));

    u4Value = (*(volatile unsigned int *)(0x10228298));
    mcSHOW_DBG_MSG(("0x10228298[22:20] = 0x%X\n", (u4Value >> 20) & 0x7));
    u4Value = (*(volatile unsigned int *)(0x10230298));
    mcSHOW_DBG_MSG(("0x10230298[22:20] = 0x%X\n", (u4Value >> 20) & 0x7));

    u4Value = (*(volatile unsigned int *)(0x10228c34));
    mcSHOW_DBG_MSG(("0x10228c34[0] = 0x%X\n", (u4Value >> 0) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x10228cb4));
    mcSHOW_DBG_MSG(("0x10228cb4[0] = 0x%X\n", (u4Value >> 0) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x10228d34));
    mcSHOW_DBG_MSG(("0x10228d34[0] = 0x%X\n", (u4Value >> 0) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x10229134));
    mcSHOW_DBG_MSG(("0x10229134[0] = 0x%X\n", (u4Value >> 0) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x102291b4));
    mcSHOW_DBG_MSG(("0x102291b4[0] = 0x%X\n", (u4Value >> 0) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x10229234));
    mcSHOW_DBG_MSG(("0x10229234[0] = 0x%X\n", (u4Value >> 0) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x10229634));
    mcSHOW_DBG_MSG(("0x10229634[0] = 0x%X\n", (u4Value >> 0) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x102296b4));
    mcSHOW_DBG_MSG(("0x102296b4[0] = 0x%X\n", (u4Value >> 0) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x10229734));
    mcSHOW_DBG_MSG(("0x10229734[0] = 0x%X\n", (u4Value >> 0) & 0x1));

    u4Value = (*(volatile unsigned int *)(0x10230c34));
    mcSHOW_DBG_MSG(("0x10230c34[0] = 0x%X\n", (u4Value >> 0) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x10230cb4));
    mcSHOW_DBG_MSG(("0x10230cb4[0] = 0x%X\n", (u4Value >> 0) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x10230d34));
    mcSHOW_DBG_MSG(("0x10230d34[0] = 0x%X\n", (u4Value >> 0) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x10231134));
    mcSHOW_DBG_MSG(("0x10231134[0] = 0x%X\n", (u4Value >> 0) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x102311b4));
    mcSHOW_DBG_MSG(("0x102311b4[0] = 0x%X\n", (u4Value >> 0) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x10231234));
    mcSHOW_DBG_MSG(("0x10231234[0] = 0x%X\n", (u4Value >> 0) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x10231634));
    mcSHOW_DBG_MSG(("0x10231634[0] = 0x%X\n", (u4Value >> 0) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x102316b4));
    mcSHOW_DBG_MSG(("0x102316b4[0] = 0x%X\n", (u4Value >> 0) & 0x1));
    u4Value = (*(volatile unsigned int *)(0x10231734));
    mcSHOW_DBG_MSG(("0x10231734[0] = 0x%X\n", (u4Value >> 0) & 0x1));

    u4Value = (*(volatile unsigned int *)(0x10228c1c));
    mcSHOW_DBG_MSG(("0x10228c1c[19:17] = 0x%X\n", (u4Value >> 17) & 0x7));
    u4Value = (*(volatile unsigned int *)(0x10228c9c));
    mcSHOW_DBG_MSG(("0x10228c9c[19:17] = 0x%X\n", (u4Value >> 17) & 0x7));
    u4Value = (*(volatile unsigned int *)(0x10228d1c));
    mcSHOW_DBG_MSG(("0x10228d1c[19:17] = 0x%X\n", (u4Value >> 17) & 0x7));
    u4Value = (*(volatile unsigned int *)(0x1022911c));
    mcSHOW_DBG_MSG(("0x1022911c[19:17] = 0x%X\n", (u4Value >> 17) & 0x7));
    u4Value = (*(volatile unsigned int *)(0x1022919c));
    mcSHOW_DBG_MSG(("0x1022919c[19:17] = 0x%X\n", (u4Value >> 17) & 0x7));
    u4Value = (*(volatile unsigned int *)(0x1022921c));
    mcSHOW_DBG_MSG(("0x1022921c[19:17] = 0x%X\n", (u4Value >> 17) & 0x7));
    u4Value = (*(volatile unsigned int *)(0x1022961c));
    mcSHOW_DBG_MSG(("0x1022961c[19:17] = 0x%X\n", (u4Value >> 17) & 0x7));
    u4Value = (*(volatile unsigned int *)(0x1022969c));
    mcSHOW_DBG_MSG(("0x1022969c[19:17] = 0x%X\n", (u4Value >> 17) & 0x7));
    u4Value = (*(volatile unsigned int *)(0x1022971c));
    mcSHOW_DBG_MSG(("0x1022971c[19:17] = 0x%X\n", (u4Value >> 17) & 0x7));

    u4Value = (*(volatile unsigned int *)(0x10230c1c));
    mcSHOW_DBG_MSG(("0x10230c1c[19:17] = 0x%X\n", (u4Value >> 17) & 0x7));
    u4Value = (*(volatile unsigned int *)(0x10230c9c));
    mcSHOW_DBG_MSG(("0x10230c9c[19:17] = 0x%X\n", (u4Value >> 17) & 0x7));
    u4Value = (*(volatile unsigned int *)(0x10230d1c));
    mcSHOW_DBG_MSG(("0x10230d1c[19:17] = 0x%X\n", (u4Value >> 17) & 0x7));
    u4Value = (*(volatile unsigned int *)(0x1023111c));
    mcSHOW_DBG_MSG(("0x1023111c[19:17] = 0x%X\n", (u4Value >> 17) & 0x7));
    u4Value = (*(volatile unsigned int *)(0x1023119c));
    mcSHOW_DBG_MSG(("0x1023119c[19:17] = 0x%X\n", (u4Value >> 17) & 0x7));
    u4Value = (*(volatile unsigned int *)(0x1023121c));
    mcSHOW_DBG_MSG(("0x1023121c[19:17] = 0x%X\n", (u4Value >> 17) & 0x7));
    u4Value = (*(volatile unsigned int *)(0x1023161c));
    mcSHOW_DBG_MSG(("0x1023161c[19:17] = 0x%X\n", (u4Value >> 17) & 0x7));
    u4Value = (*(volatile unsigned int *)(0x1023169c));
    mcSHOW_DBG_MSG(("0x1023169c[19:17] = 0x%X\n", (u4Value >> 17) & 0x7));
    u4Value = (*(volatile unsigned int *)(0x1023171c));
    mcSHOW_DBG_MSG(("0x1023171c[19:17] = 0x%X\n", (u4Value >> 17) & 0x7));

    mcSHOW_DBG_MSG(("*** Low power related \n"));

    u4Value = (*(volatile unsigned int *)(0x10228088));
    mcSHOW_DBG_MSG(("0x10228088[31:10] = 0x%X\n", (u4Value >> 10) & 0x3fffff));
    u4Value = (*(volatile unsigned int *)(0x10230088));
    mcSHOW_DBG_MSG(("0x10230088[31:10] = 0x%X\n", (u4Value >> 10) & 0x3fffff));

    u4Value = (*(volatile unsigned int *)(0x1022808C));
    mcSHOW_DBG_MSG(("0x1022808C = 0x%X\n", u4Value));
    u4Value = (*(volatile unsigned int *)(0x1023008C));
    mcSHOW_DBG_MSG(("0x1023008C = 0x%X\n", u4Value));

    u4Value = (*(volatile unsigned int *)(0x10228108));
    mcSHOW_DBG_MSG(("0x10228108[31:10] = 0x%X\n", (u4Value >> 10) & 0x3fffff));
    u4Value = (*(volatile unsigned int *)(0x10230108));
    mcSHOW_DBG_MSG(("0x10230108[31:10] = 0x%X\n", (u4Value >> 10) & 0x3fffff));

    u4Value = (*(volatile unsigned int *)(0x1022810C));
    mcSHOW_DBG_MSG(("0x1022810C = 0x%X\n", u4Value));
    u4Value = (*(volatile unsigned int *)(0x1023010C));
    mcSHOW_DBG_MSG(("0x1023010C = 0x%X\n", u4Value));

    u4Value = (*(volatile unsigned int *)(0x10228188));
    mcSHOW_DBG_MSG(("0x10228188[31:10] = 0x%X\n", (u4Value >> 10) & 0x3fffff));
    u4Value = (*(volatile unsigned int *)(0x10230188));
    mcSHOW_DBG_MSG(("0x10230188[31:10] = 0x%X\n", (u4Value >> 10) & 0x3fffff));

    u4Value = (*(volatile unsigned int *)(0x1022818C));
    mcSHOW_DBG_MSG(("0x1022818C = 0x%X\n", u4Value));
    u4Value = (*(volatile unsigned int *)(0x1023018C));
    mcSHOW_DBG_MSG(("0x1023018C = 0x%X\n", u4Value));

    u4Value = (*(volatile unsigned int *)(0x10228274));
    mcSHOW_DBG_MSG(("0x10228274 = 0x%X\n", u4Value));
    u4Value = (*(volatile unsigned int *)(0x10230274));
    mcSHOW_DBG_MSG(("0x10230274 = 0x%X\n", u4Value));
    u4Value = (*(volatile unsigned int *)(0x1022827C));
    mcSHOW_DBG_MSG(("0x1022827C = 0x%X\n", u4Value));
    u4Value = (*(volatile unsigned int *)(0x1023027C));
    mcSHOW_DBG_MSG(("0x1023027C = 0x%X\n", u4Value));

    return;
}
#endif

#if (SW_CHANGE_FOR_SIMULATION==0) 
/* LP4 2CH */
void LP4EnableDramcPhyDCM2Channel(DRAMC_CTX_T *p, BOOL bEn)//Should refer to "vSetChannelNumber"
{
    U32 u4WbrBackup = 0;
    INT16 i2ShuIdx = 0;
#if DUAL_FREQ_K
    INT16 i2ShuffleMax = DRAM_DFS_SHUFFLE_3;
#else
    INT16 i2ShuffleMax = DRAM_DFS_SHUFFLE_1;
#endif    

    if(bEn) //DCM on
    {
        vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL0, P_Fld(0x0, MISC_CG_CTRL0_RG_CG_INFRA_OFF_DISABLE)
                | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_RX_COMB1_OFF_DISABLE) | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_RX_COMB0_OFF_DISABLE)
                | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_RX_CMD_OFF_DISABLE) | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_COMB1_OFF_DISABLE)
                | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_COMB0_OFF_DISABLE) | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_CMD_OFF_DISABLE)
                | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_COMB_OFF_DISABLE) | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_PHY_OFF_DIABLE)
                | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_DRAMC_OFF_DISABLE) | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_EMI_OFF_DISABLE)
                | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_DRAMC_CHB_CK_OFF));//Lewis@20170206 for lp4 2ch confirm
        for(i2ShuIdx = DRAM_DFS_SHUFFLE_1; i2ShuIdx <= i2ShuffleMax; i2ShuIdx++)
        {
            vIO32WriteFldMulti_All(DDRPHY_SHU1_B0_DQ8 + 0x500 * i2ShuIdx, P_Fld(0x0, SHU1_B0_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B0)
                    | P_Fld(0x0, SHU1_B0_DQ8_R_DMRANK_PIPE_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B0)
                    | P_Fld(0x0, SHU1_B0_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B0)
                    | P_Fld(0x0, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_DMSTBEN_SYNC_CG_IG_B0)
                    | P_Fld(0x0, SHU1_B0_DQ8_R_DMRXDLY_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B0)
                    | P_Fld(0x0, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B0) | P_Fld(0x1, SHU1_B0_DQ8_R_RMRX_TOPHY_CG_IG_B0)
                    | P_Fld(0x0, SHU1_B0_DQ8_R_RMRODTEN_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B0));//19th
            vIO32WriteFldMulti_All(DDRPHY_SHU1_B1_DQ8 + 0x500 * i2ShuIdx, P_Fld(0x0, SHU1_B1_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B1)
                    | P_Fld(0x0, SHU1_B1_DQ8_R_DMRANK_PIPE_CG_IG_B1) | P_Fld(0x0, SHU1_B1_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B1)
                    | P_Fld(0x0, SHU1_B1_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B1) | P_Fld(0x0, SHU1_B1_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B1)
                    | P_Fld(0x0, SHU1_B1_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B1) | P_Fld(0x0, SHU1_B1_DQ8_R_DMSTBEN_SYNC_CG_IG_B1)
                    | P_Fld(0x0, SHU1_B1_DQ8_R_DMRXDLY_CG_IG_B1) | P_Fld(0x0, SHU1_B1_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B1)
                    | P_Fld(0x0, SHU1_B1_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B1) | P_Fld(0x1, SHU1_B1_DQ8_R_RMRX_TOPHY_CG_IG_B1)
                    | P_Fld(0x0, SHU1_B1_DQ8_R_RMRODTEN_CG_IG_B1) | P_Fld(0x0, SHU1_B1_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B1));//19th
            vIO32WriteFldMulti_All(DDRPHY_SHU1_CA_CMD8 + 0x500 * i2ShuIdx, P_Fld(0x0, SHU1_CA_CMD8_R_DMRANK_CHG_PIPE_CG_IG_CA)
                    | P_Fld(0x0, SHU1_CA_CMD8_R_DMRANK_PIPE_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_CA)
                    | P_Fld(0x0, SHU1_CA_CMD8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_PIPE_CG_IG_CA)
                    | P_Fld(0x0, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_SYNC_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMSTBEN_SYNC_CG_IG_CA)
                    | P_Fld(0x0, SHU1_CA_CMD8_R_DMRXDLY_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_CA)
                    | P_Fld(0x0, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_PIPE_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_RMRX_TOPHY_CG_IG_CA)
                    | P_Fld(0x0, SHU1_CA_CMD8_R_RMRODTEN_CG_IG_CA)
                    | P_Fld(0x0, SHU1_CA_CMD8_R_DMRANK_RXDLY_PIPE_CG_IG_CA));//19th
        }
        vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL5, P_Fld(0x1, MISC_CG_CTRL5_R_CA_DLY_DCM_EN)
            | P_Fld(0x1, MISC_CG_CTRL5_R_DQ0_DLY_DCM_EN)
            | P_Fld(0x1, MISC_CG_CTRL5_R_DQ1_DLY_DCM_EN)
            | P_Fld(0x1, MISC_CG_CTRL5_R_CA_PI_DCM_EN)
            | P_Fld(0x1, MISC_CG_CTRL5_R_DQ0_PI_DCM_EN)
            | P_Fld(0x1, MISC_CG_CTRL5_R_DQ1_PI_DCM_EN));

    }
    else //DCM off
    {
        vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL0, P_Fld(0x1, MISC_CG_CTRL0_RG_CG_INFRA_OFF_DISABLE)
                | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_RX_COMB1_OFF_DISABLE) | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_RX_COMB0_OFF_DISABLE)
                | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_RX_CMD_OFF_DISABLE) | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_COMB1_OFF_DISABLE)
                | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_COMB0_OFF_DISABLE) | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_CMD_OFF_DISABLE)
                | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_COMB_OFF_DISABLE) | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_PHY_OFF_DIABLE)
                | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_DRAMC_OFF_DISABLE) | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_EMI_OFF_DISABLE)
                | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_DRAMC_CHB_CK_OFF));//Lewis@20170115

        for(i2ShuIdx = DRAM_DFS_SHUFFLE_1; i2ShuIdx <= i2ShuffleMax; i2ShuIdx++)
        {
            vIO32WriteFldMulti_All(DDRPHY_SHU1_B0_DQ8 + 0x500 * i2ShuIdx, P_Fld(0x1, SHU1_B0_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B0)
                    | P_Fld(0x1, SHU1_B0_DQ8_R_DMRANK_PIPE_CG_IG_B0) | P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B0)
                    | P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B0) | P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B0)
                    | P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B0) | P_Fld(0x1, SHU1_B0_DQ8_R_DMSTBEN_SYNC_CG_IG_B0)
                    | P_Fld(0x1, SHU1_B0_DQ8_R_DMRXDLY_CG_IG_B0) | P_Fld(0x1, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B0)
                    | P_Fld(0x1, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B0) | P_Fld(0x1, SHU1_B0_DQ8_R_RMRX_TOPHY_CG_IG_B0)
                    | P_Fld(0x1, SHU1_B0_DQ8_R_RMRODTEN_CG_IG_B0) | P_Fld(0x1, SHU1_B0_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B0));//19th
            vIO32WriteFldMulti_All(DDRPHY_SHU1_B1_DQ8 + 0x500 * i2ShuIdx, P_Fld(0x1, SHU1_B1_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B1)
                    | P_Fld(0x1, SHU1_B1_DQ8_R_DMRANK_PIPE_CG_IG_B1) | P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B1)
                    | P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B1) | P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B1)
                    | P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B1) | P_Fld(0x1, SHU1_B1_DQ8_R_DMSTBEN_SYNC_CG_IG_B1)
                    | P_Fld(0x1, SHU1_B1_DQ8_R_DMRXDLY_CG_IG_B1) | P_Fld(0x1, SHU1_B1_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B1)
                    | P_Fld(0x1, SHU1_B1_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B1) | P_Fld(0x1, SHU1_B1_DQ8_R_RMRX_TOPHY_CG_IG_B1)
                    | P_Fld(0x1, SHU1_B1_DQ8_R_RMRODTEN_CG_IG_B1) | P_Fld(0x1, SHU1_B1_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B1));//19th
            vIO32WriteFldMulti_All(DDRPHY_SHU1_CA_CMD8 + 0x500 * i2ShuIdx, P_Fld(0x0, SHU1_CA_CMD8_R_DMRANK_CHG_PIPE_CG_IG_CA)
                    | P_Fld(0x0, SHU1_CA_CMD8_R_DMRANK_PIPE_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_CA)
                    | P_Fld(0x0, SHU1_CA_CMD8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_PIPE_CG_IG_CA)
                    | P_Fld(0x0, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_SYNC_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMSTBEN_SYNC_CG_IG_CA)
                    | P_Fld(0x0, SHU1_CA_CMD8_R_DMRXDLY_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_CA)
                    | P_Fld(0x0, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_PIPE_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_RMRX_TOPHY_CG_IG_CA)
                    | P_Fld(0x0, SHU1_CA_CMD8_R_RMRODTEN_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMRANK_RXDLY_PIPE_CG_IG_CA));//19th
        }
        vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL5, P_Fld(0x0, MISC_CG_CTRL5_R_CA_PI_DCM_EN)
                | P_Fld(0x0, MISC_CG_CTRL5_R_DQ0_PI_DCM_EN)
                | P_Fld(0x0, MISC_CG_CTRL5_R_DQ1_PI_DCM_EN)
                | P_Fld(0x0, MISC_CG_CTRL5_R_CA_DLY_DCM_EN)
                | P_Fld(0x0, MISC_CG_CTRL5_R_DQ0_DLY_DCM_EN)
                | P_Fld(0x0, MISC_CG_CTRL5_R_DQ1_DLY_DCM_EN));        
    }
    return;
}


#if ENABLE_LP3_SW
/* LP3 */
void LP3EnableDramcPhyDCM(DRAMC_CTX_T *p, BOOL bEn)//Should refer to "vSetChannelNumber"
{
    U32 u4WbrBackup = 0;
    INT16 i2ShuIdx = 0;
#if DUAL_FREQ_K
    INT16 i2ShuffleMax = DRAM_DFS_SHUFFLE_3;
#else
    INT16 i2ShuffleMax = DRAM_DFS_SHUFFLE_1;
#endif    

    if(bEn) //DCM on
    {
        vIO32WriteFldMulti(DDRPHY_MISC_CG_CTRL0, P_Fld(0x0, MISC_CG_CTRL0_RG_CG_INFRA_OFF_DISABLE)
                | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_RX_COMB1_OFF_DISABLE) | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_RX_COMB0_OFF_DISABLE)
                | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_RX_CMD_OFF_DISABLE) | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_COMB1_OFF_DISABLE)
                | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_COMB0_OFF_DISABLE) | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_CMD_OFF_DISABLE)
                | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_COMB_OFF_DISABLE) | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_PHY_OFF_DIABLE)
                | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_DRAMC_OFF_DISABLE) | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_EMI_OFF_DISABLE)
                | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_DRAMC_CHB_CK_OFF));//20th
        vIO32WriteFldMulti(((U32)CHANNEL_B<<POS_BANK_NUM) + DDRPHY_MISC_CG_CTRL0, P_Fld(0x0, MISC_CG_CTRL0_RG_CG_INFRA_OFF_DISABLE)
                | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_RX_COMB1_OFF_DISABLE) | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_RX_COMB0_OFF_DISABLE)
                | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_RX_CMD_OFF_DISABLE) | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_COMB1_OFF_DISABLE)
                | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_COMB0_OFF_DISABLE) | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_CMD_OFF_DISABLE)
                | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_COMB_OFF_DISABLE) | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_PHY_OFF_DIABLE)
                | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_DRAMC_OFF_DISABLE) | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_EMI_OFF_DISABLE)
                | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_DRAMC_CHB_CK_OFF));//20th
        for(i2ShuIdx = DRAM_DFS_SHUFFLE_1; i2ShuIdx <= i2ShuffleMax; i2ShuIdx++)
        {
            vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ8 + 0x500 * i2ShuIdx, P_Fld(0x0, SHU1_B0_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B0)
                    | P_Fld(0x0, SHU1_B0_DQ8_R_DMRANK_PIPE_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B0)
                    | P_Fld(0x0, SHU1_B0_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B0)
                    | P_Fld(0x0, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_DMSTBEN_SYNC_CG_IG_B0)
                    | P_Fld(0x0, SHU1_B0_DQ8_R_DMRXDLY_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B0)
                    | P_Fld(0x0, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_RMRX_TOPHY_CG_IG_B0)
                    | P_Fld(0x0, SHU1_B0_DQ8_R_RMRODTEN_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B0));//19th
            vIO32WriteFldMulti(((U32)CHANNEL_B<<POS_BANK_NUM) + DDRPHY_SHU1_B0_DQ8 + 0x500 * i2ShuIdx, P_Fld(0x0, SHU1_B0_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B0)
                    | P_Fld(0x0, SHU1_B0_DQ8_R_DMRANK_PIPE_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B0)
                    | P_Fld(0x0, SHU1_B0_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B0)
                    | P_Fld(0x0, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_DMSTBEN_SYNC_CG_IG_B0)
                    | P_Fld(0x0, SHU1_B0_DQ8_R_DMRXDLY_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B0)
                    | P_Fld(0x0, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B0) | P_Fld(0x1, SHU1_B0_DQ8_R_RMRX_TOPHY_CG_IG_B0)
                    | P_Fld(0x0, SHU1_B0_DQ8_R_RMRODTEN_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B0));//19th
            vIO32WriteFldMulti_All(DDRPHY_SHU1_B1_DQ8 + 0x500 * i2ShuIdx, P_Fld(0x0, SHU1_B1_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B1)
                    | P_Fld(0x0, SHU1_B1_DQ8_R_DMRANK_PIPE_CG_IG_B1) | P_Fld(0x0, SHU1_B1_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B1)
                    | P_Fld(0x0, SHU1_B1_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B1) | P_Fld(0x0, SHU1_B1_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B1)
                    | P_Fld(0x0, SHU1_B1_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B1) | P_Fld(0x0, SHU1_B1_DQ8_R_DMSTBEN_SYNC_CG_IG_B1)
                    | P_Fld(0x0, SHU1_B1_DQ8_R_DMRXDLY_CG_IG_B1) | P_Fld(0x0, SHU1_B1_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B1)
                    | P_Fld(0x0, SHU1_B1_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B1) | P_Fld(0x1, SHU1_B1_DQ8_R_RMRX_TOPHY_CG_IG_B1)
                    | P_Fld(0x0, SHU1_B1_DQ8_R_RMRODTEN_CG_IG_B1) | P_Fld(0x0, SHU1_B1_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B1));//19th
            vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD8 + 0x500 * i2ShuIdx, P_Fld(0x0, SHU1_CA_CMD8_R_DMRANK_CHG_PIPE_CG_IG_CA)
                    | P_Fld(0x0, SHU1_CA_CMD8_R_DMRANK_PIPE_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_CA)
                    | P_Fld(0x0, SHU1_CA_CMD8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_PIPE_CG_IG_CA)
                    | P_Fld(0x0, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_SYNC_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMSTBEN_SYNC_CG_IG_CA)
                    | P_Fld(0x0, SHU1_CA_CMD8_R_DMRXDLY_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_CA)
                    | P_Fld(0x0, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_PIPE_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_RMRX_TOPHY_CG_IG_CA)
                    | P_Fld(0x0, SHU1_CA_CMD8_R_RMRODTEN_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMRANK_RXDLY_PIPE_CG_IG_CA));//19th
            vIO32WriteFldMulti(((U32)CHANNEL_B<<POS_BANK_NUM) + DDRPHY_SHU1_CA_CMD8 + 0x500 * i2ShuIdx, P_Fld(0x0, SHU1_CA_CMD8_R_DMRANK_CHG_PIPE_CG_IG_CA)
                    | P_Fld(0x0, SHU1_CA_CMD8_R_DMRANK_PIPE_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_CA)
                    | P_Fld(0x0, SHU1_CA_CMD8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_PIPE_CG_IG_CA)
                    | P_Fld(0x0, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_SYNC_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMSTBEN_SYNC_CG_IG_CA)
                    | P_Fld(0x0, SHU1_CA_CMD8_R_DMRXDLY_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_CA)
                    | P_Fld(0x0, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_PIPE_CG_IG_CA) | P_Fld(0x1, SHU1_CA_CMD8_R_RMRX_TOPHY_CG_IG_CA)
                    | P_Fld(0x0, SHU1_CA_CMD8_R_RMRODTEN_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMRANK_RXDLY_PIPE_CG_IG_CA));//19th
        }
        //Lewis@20170421: LP3 should set DCM off(CHA:DQ0, CHB:DQ0 DQ1 CA) since DCM on will degrade TX DQ/DQS window to 4 PI while at the first write after DVFS(DE:Lynx.Lin)
        vIO32WriteFldMulti(DDRPHY_MISC_CG_CTRL5, P_Fld(0x1, MISC_CG_CTRL5_R_CA_DLY_DCM_EN)
            | P_Fld(0x1, MISC_CG_CTRL5_R_DQ0_DLY_DCM_EN)
            | P_Fld(0x1, MISC_CG_CTRL5_R_DQ1_DLY_DCM_EN)
            | P_Fld(0x1, MISC_CG_CTRL5_R_CA_PI_DCM_EN)
            | P_Fld(0x0, MISC_CG_CTRL5_R_DQ0_PI_DCM_EN)
            | P_Fld(0x1, MISC_CG_CTRL5_R_DQ1_PI_DCM_EN));
        vIO32WriteFldMulti(((U32)CHANNEL_B<<POS_BANK_NUM) + DDRPHY_MISC_CG_CTRL5, P_Fld(0x1, MISC_CG_CTRL5_R_CA_DLY_DCM_EN)
            | P_Fld(0x1, MISC_CG_CTRL5_R_DQ0_DLY_DCM_EN)
            | P_Fld(0x1, MISC_CG_CTRL5_R_DQ1_DLY_DCM_EN)
            | P_Fld(0x0, MISC_CG_CTRL5_R_CA_PI_DCM_EN)
            | P_Fld(0x0, MISC_CG_CTRL5_R_DQ0_PI_DCM_EN)
            | P_Fld(0x0, MISC_CG_CTRL5_R_DQ1_PI_DCM_EN));
    }
    else //DCM off
    {
        vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL0, P_Fld(0x1, MISC_CG_CTRL0_RG_CG_INFRA_OFF_DISABLE)
                | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_RX_COMB1_OFF_DISABLE) | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_RX_COMB0_OFF_DISABLE)
                | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_RX_CMD_OFF_DISABLE) | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_COMB1_OFF_DISABLE)
                | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_COMB0_OFF_DISABLE) | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_CMD_OFF_DISABLE)
                | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_COMB_OFF_DISABLE) | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_PHY_OFF_DIABLE)
                | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_DRAMC_OFF_DISABLE) | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_EMI_OFF_DISABLE)
                | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_DRAMC_CHB_CK_OFF));//20th
        for(i2ShuIdx = DRAM_DFS_SHUFFLE_1; i2ShuIdx <= i2ShuffleMax; i2ShuIdx++)
        {
            vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ8 + 0x500 * i2ShuIdx, P_Fld(0x0, SHU1_B0_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B0)
                    | P_Fld(0x0, SHU1_B0_DQ8_R_DMRANK_PIPE_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B0)
                    | P_Fld(0x0, SHU1_B0_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B0)
                    | P_Fld(0x0, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_DMSTBEN_SYNC_CG_IG_B0)
                    | P_Fld(0x0, SHU1_B0_DQ8_R_DMRXDLY_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B0)
                    | P_Fld(0x0, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_RMRX_TOPHY_CG_IG_B0)
                    | P_Fld(0x0, SHU1_B0_DQ8_R_RMRODTEN_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B0));//19th
            vIO32WriteFldMulti(((U32)CHANNEL_B<<POS_BANK_NUM) + DDRPHY_SHU1_B0_DQ8 + 0x500 * i2ShuIdx, P_Fld(0x1, SHU1_B0_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B0)
                    | P_Fld(0x1, SHU1_B0_DQ8_R_DMRANK_PIPE_CG_IG_B0) | P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B0)
                    | P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B0) | P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B0)
                    | P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B0) | P_Fld(0x1, SHU1_B0_DQ8_R_DMSTBEN_SYNC_CG_IG_B0)
                    | P_Fld(0x1, SHU1_B0_DQ8_R_DMRXDLY_CG_IG_B0) | P_Fld(0x1, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B0)
                    | P_Fld(0x1, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B0) | P_Fld(0x1, SHU1_B0_DQ8_R_RMRX_TOPHY_CG_IG_B0)
                    | P_Fld(0x1, SHU1_B0_DQ8_R_RMRODTEN_CG_IG_B0) | P_Fld(0x1, SHU1_B0_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B0));//19th
            vIO32WriteFldMulti_All(DDRPHY_SHU1_B1_DQ8 + 0x500 * i2ShuIdx, P_Fld(0x1, SHU1_B1_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B1)
                    | P_Fld(0x1, SHU1_B1_DQ8_R_DMRANK_PIPE_CG_IG_B1) | P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B1)
                    | P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B1) | P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B1)
                    | P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B1) | P_Fld(0x1, SHU1_B1_DQ8_R_DMSTBEN_SYNC_CG_IG_B1)
                    | P_Fld(0x1, SHU1_B1_DQ8_R_DMRXDLY_CG_IG_B1) | P_Fld(0x1, SHU1_B1_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B1)
                    | P_Fld(0x1, SHU1_B1_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B1) | P_Fld(0x1, SHU1_B1_DQ8_R_RMRX_TOPHY_CG_IG_B1)
                    | P_Fld(0x1, SHU1_B1_DQ8_R_RMRODTEN_CG_IG_B1) | P_Fld(0x1, SHU1_B1_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B1));//19th
            vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD8 + 0x500 * i2ShuIdx, P_Fld(0x0, SHU1_CA_CMD8_R_DMRANK_CHG_PIPE_CG_IG_CA)
                    | P_Fld(0x0, SHU1_CA_CMD8_R_DMRANK_PIPE_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_CA)
                    | P_Fld(0x0, SHU1_CA_CMD8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_PIPE_CG_IG_CA)
                    | P_Fld(0x0, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_SYNC_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMSTBEN_SYNC_CG_IG_CA)
                    | P_Fld(0x0, SHU1_CA_CMD8_R_DMRXDLY_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_CA)
                    | P_Fld(0x0, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_PIPE_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_RMRX_TOPHY_CG_IG_CA)
                    | P_Fld(0x0, SHU1_CA_CMD8_R_RMRODTEN_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMRANK_RXDLY_PIPE_CG_IG_CA));//19th
            vIO32WriteFldMulti(((U32)CHANNEL_B<<POS_BANK_NUM) + DDRPHY_SHU1_CA_CMD8 + 0x500 * i2ShuIdx, P_Fld(0x1, SHU1_CA_CMD8_R_DMRANK_CHG_PIPE_CG_IG_CA)
                    | P_Fld(0x1, SHU1_CA_CMD8_R_DMRANK_PIPE_CG_IG_CA) | P_Fld(0x1, SHU1_CA_CMD8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_CA)
                    | P_Fld(0x1, SHU1_CA_CMD8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_CA) | P_Fld(0x1, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_PIPE_CG_IG_CA)
                    | P_Fld(0x1, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_SYNC_CG_IG_CA) | P_Fld(0x1, SHU1_CA_CMD8_R_DMSTBEN_SYNC_CG_IG_CA)
                    | P_Fld(0x1, SHU1_CA_CMD8_R_DMRXDLY_CG_IG_CA) | P_Fld(0x1, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_CA)
                    | P_Fld(0x1, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_PIPE_CG_IG_CA) | P_Fld(0x1, SHU1_CA_CMD8_R_RMRX_TOPHY_CG_IG_CA)
                    | P_Fld(0x1, SHU1_CA_CMD8_R_RMRODTEN_CG_IG_CA) | P_Fld(0x1, SHU1_CA_CMD8_R_DMRANK_RXDLY_PIPE_CG_IG_CA));//19th
        }
        vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL5, P_Fld(0x0, MISC_CG_CTRL5_R_CA_PI_DCM_EN)
                | P_Fld(0x0, MISC_CG_CTRL5_R_DQ0_PI_DCM_EN)
                | P_Fld(0x0, MISC_CG_CTRL5_R_DQ1_PI_DCM_EN)
                | P_Fld(0x0, MISC_CG_CTRL5_R_CA_DLY_DCM_EN)
                | P_Fld(0x0, MISC_CG_CTRL5_R_DQ0_DLY_DCM_EN)
                | P_Fld(0x0, MISC_CG_CTRL5_R_DQ1_DLY_DCM_EN));        
    }
    return;
}
#endif

//Common part + different by LP4_2CH, LP3, LP4_1CH
void EnableDramcPhyDCM(DRAMC_CTX_T *p, BOOL bEn)//Should refer to "vSetChannelNumber" 
{
    U32 u4WbrBackup = 0;
    INT16 i2ShuIdx = 0;
#if DUAL_FREQ_K
    INT16 i2ShuffleMax = DRAM_DFS_SHUFFLE_3;
#else
    INT16 i2ShuffleMax = DRAM_DFS_SHUFFLE_1;
#endif    
    if(bEn)
    {
        vIO32WriteFldMulti_All(DRAMC_REG_DRAMC_PD_CTRL, P_Fld(0x1, DRAMC_PD_CTRL_DCMENNOTRFC)
                | P_Fld(0x1, DRAMC_PD_CTRL_COMBCLKCTRL)
                | P_Fld(0x1, DRAMC_PD_CTRL_PHYCLKDYNGEN)
                | P_Fld(0x1, DRAMC_PD_CTRL_DCMEN) | P_Fld(0x0, DRAMC_PD_CTRL_COMBPHY_CLKENSAME)
                | P_Fld(0x1, DRAMC_PD_CTRL_DCMEN2));
        //mem_dcm
        vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL2, 0x806003BE, MISC_CG_CTRL2_RG_MEM_DCM_CTL);//divided freq change to 1/4
        vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL2, 0x806003BF, MISC_CG_CTRL2_RG_MEM_DCM_CTL);//divided freq change to 1/4
        vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL2, 0x806003BE, MISC_CG_CTRL2_RG_MEM_DCM_CTL);//divided freq change to 1/4
        //Lewis@20170206 for lp4 2ch confirm
        vIO32WriteFldMulti_All(DDRPHY_MISC_CTRL3, P_Fld(0x0, MISC_CTRL3_R_DDRPHY_COMB_CG_IG)
                | P_Fld(0x0, MISC_CTRL3_R_DDRPHY_RX_PIPE_CG_IG));
        for(i2ShuIdx = DRAM_DFS_SHUFFLE_1; i2ShuIdx <= i2ShuffleMax; i2ShuIdx++)
        {
            vIO32WriteFldAlign_All(DDRPHY_SHU1_B0_DLL0 + 0x500 * i2ShuIdx, 0x1, SHU1_B0_DLL0_RG_ARPISM_MCK_SEL_B0_SHU);//PI_CLOCK_CG_IMPROVEMENT PIC:JOE
            vIO32WriteFldAlign_All(DDRPHY_SHU1_B1_DLL0 + 0x500 * i2ShuIdx, 0x1, SHU1_B1_DLL0_RG_ARPISM_MCK_SEL_B1_SHU);//PI_CLOCK_CG_IMPROVEMENT PIC:JOE
            vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_DLL0 + 0x500 * i2ShuIdx, 0x1, SHU1_CA_DLL0_RG_ARPISM_MCK_SEL_CA_SHU);//PI_CLOCK_CG_IMPROVEMENT PIC:JOE

            vIO32WriteFldMulti_All(DDRPHY_SHU1_B0_DQ7 + 0x500 * i2ShuIdx, P_Fld(0x1, SHU1_B0_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B0)
                    | P_Fld(0x1, SHU1_B0_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B0)
                    | P_Fld(0x1, SHU1_B0_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B0));//PI_CLOCK_CG_IMPROVEMENT PIC:JOE
            vIO32WriteFldMulti_All(DDRPHY_SHU1_B1_DQ7 + 0x500 * i2ShuIdx, P_Fld(0x1, SHU1_B1_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B1)
                    | P_Fld(0x1, SHU1_B1_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B1)
                    | P_Fld(0x1, SHU1_B1_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B1));//PI_CLOCK_CG_IMPROVEMENT PIC:JOE
            vIO32WriteFldMulti_All(DDRPHY_SHU1_CA_CMD7 + 0x500 * i2ShuIdx, P_Fld(0x1, SHU1_CA_CMD7_R_DMTX_ARPI_CG_CS_NEW)
                    | P_Fld(0x1, SHU1_CA_CMD7_R_DMTX_ARPI_CG_CLK_NEW)
                    | P_Fld(0x1, SHU1_CA_CMD7_R_DMTX_ARPI_CG_CMD_NEW));//PI_CLOCK_CG_IMPROVEMENT PIC:JOE
        }
    }
    else //DCM off
    {
        vIO32WriteFldMulti_All( DRAMC_REG_DRAMC_PD_CTRL, P_Fld(0x0, DRAMC_PD_CTRL_DCMENNOTRFC)
                | P_Fld(0x0, DRAMC_PD_CTRL_COMBCLKCTRL)
                | P_Fld(0x0, DRAMC_PD_CTRL_PHYCLKDYNGEN)
                | P_Fld(0x0, DRAMC_PD_CTRL_DCMEN) | P_Fld(0x1, DRAMC_PD_CTRL_COMBPHY_CLKENSAME)
                | P_Fld(0x0, DRAMC_PD_CTRL_DCMEN2));
        //mem_dcm
        vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL2, 0x8060037E, MISC_CG_CTRL2_RG_MEM_DCM_CTL);//divided freq change to 1/4
        vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL2, 0x8060037F, MISC_CG_CTRL2_RG_MEM_DCM_CTL);//divided freq change to 1/4
        vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL2, 0x8060037E, MISC_CG_CTRL2_RG_MEM_DCM_CTL);//divided freq change to 1/4
        //Lewis@20170115
        vIO32WriteFldMulti_All(DDRPHY_MISC_CTRL3, P_Fld(0x1, MISC_CTRL3_R_DDRPHY_COMB_CG_IG)
                | P_Fld(0x1, MISC_CTRL3_R_DDRPHY_RX_PIPE_CG_IG));
        for(i2ShuIdx = DRAM_DFS_SHUFFLE_1; i2ShuIdx <= i2ShuffleMax; i2ShuIdx++)
        {
            vIO32WriteFldAlign_All(DDRPHY_SHU1_B0_DLL0 + 0x500 * i2ShuIdx, 0x0, SHU1_B0_DLL0_RG_ARPISM_MCK_SEL_B0_SHU);//PI_CLOCK_CG_IMPROVEMENT PIC:JOE
            vIO32WriteFldAlign_All(DDRPHY_SHU1_B1_DLL0 + 0x500 * i2ShuIdx, 0x0, SHU1_B1_DLL0_RG_ARPISM_MCK_SEL_B1_SHU);//PI_CLOCK_CG_IMPROVEMENT PIC:JOE
            vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_DLL0 + 0x500 * i2ShuIdx, 0x0, SHU1_CA_DLL0_RG_ARPISM_MCK_SEL_CA_SHU);//PI_CLOCK_CG_IMPROVEMENT PIC:JOE

            vIO32WriteFldMulti_All(DDRPHY_SHU1_B0_DQ7 + 0x500 * i2ShuIdx, P_Fld(0x0, SHU1_B0_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B0)
                    | P_Fld(0x0, SHU1_B0_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B0)
                    | P_Fld(0x0, SHU1_B0_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B0));//PI_CLOCK_CG_IMPROVEMENT PIC:JOE
            vIO32WriteFldMulti_All(DDRPHY_SHU1_B1_DQ7 + 0x500 * i2ShuIdx, P_Fld(0x0, SHU1_B1_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B1)
                    | P_Fld(0x0, SHU1_B1_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B1)
                    | P_Fld(0x0, SHU1_B1_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B1));//PI_CLOCK_CG_IMPROVEMENT PIC:JOE
            vIO32WriteFldMulti_All(DDRPHY_SHU1_CA_CMD7 + 0x500 * i2ShuIdx, P_Fld(0x0, SHU1_CA_CMD7_R_DMTX_ARPI_CG_CS_NEW)
                    | P_Fld(0x0, SHU1_CA_CMD7_R_DMTX_ARPI_CG_CLK_NEW)
                    | P_Fld(0x0, SHU1_CA_CMD7_R_DMTX_ARPI_CG_CMD_NEW));//PI_CLOCK_CG_IMPROVEMENT PIC:JOE
        }
    }
    /* Different part */
    if(u1IsLP4Family(p->dram_type))
    {
        LP4EnableDramcPhyDCM2Channel(p, bEn);
    }
    else //LP3
    {
#if ENABLE_LP3_SW
        LP3EnableDramcPhyDCM(p, bEn);
#endif
    }
}


void LP4DDRPhyLowPowerEnable2Channel(DRAMC_CTX_T *p)
{
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI2, 0, B0_DLL_ARPI2_BIT_31_10);
    vIO32Write4B_All(DDRPHY_B0_DLL_ARPI3, 0x2E800);
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI2, 0, B1_DLL_ARPI2_BIT_31_10);
    vIO32Write4B_All(DDRPHY_B1_DLL_ARPI3, 0x2E800);
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI2, 0x2, CA_DLL_ARPI2_BIT_31_10);
    vIO32Write4B(DDRPHY_CA_DLL_ARPI3, 0xBA000);
    vIO32Write4B(DDRPHY_CA_DLL_ARPI3 + ((U32)CHANNEL_B<<POS_BANK_NUM), 0x3A000);
    return;
}

#if ENABLE_LP3_SW
void LP3DDRPhyLowPowerEnable(DRAMC_CTX_T *p)
{
    vIO32WriteFldAlign(DDRPHY_B0_DLL_ARPI2, 0x2, B0_DLL_ARPI2_BIT_31_10);
    vIO32WriteFldAlign(((U32)CHANNEL_B<<POS_BANK_NUM) + DDRPHY_B0_DLL_ARPI2, 0, B0_DLL_ARPI2_BIT_31_10);
    vIO32Write4B(DDRPHY_B0_DLL_ARPI3, 0x2E000);
    vIO32Write4B(((U32)CHANNEL_B<<POS_BANK_NUM) + DDRPHY_B0_DLL_ARPI3, 0x2E800);
    vIO32WriteFldAlign(DDRPHY_B1_DLL_ARPI2, 0, B1_DLL_ARPI2_BIT_31_10);
    vIO32WriteFldAlign(((U32)CHANNEL_B<<POS_BANK_NUM) + DDRPHY_B1_DLL_ARPI2, 0, B1_DLL_ARPI2_BIT_31_10);
    vIO32Write4B_All(DDRPHY_B1_DLL_ARPI3, 0x2E800);
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI2, 0x2, CA_DLL_ARPI2_BIT_31_10);
    vIO32WriteFldAlign(((U32)CHANNEL_B<<POS_BANK_NUM) + DDRPHY_CA_DLL_ARPI2, 0, CA_DLL_ARPI2_BIT_31_10);
    vIO32Write4B(DDRPHY_CA_DLL_ARPI3, 0xBA000);
    vIO32Write4B(DDRPHY_CA_DLL_ARPI3 + ((U32)CHANNEL_B<<POS_BANK_NUM), 0x3A800);
    return;
}
#endif

void DDRPhyLowPowerEnable(DRAMC_CTX_T *p)//Should refer to "vSetChannelNumber"
{
    if(u1IsLP4Family(p->dram_type))
    {
        LP4DDRPhyLowPowerEnable2Channel(p);
    }
    else //LP3
    {
#if ENABLE_LP3_SW
        LP3DDRPhyLowPowerEnable(p);
#endif
    }
}
#endif

void DummyReadForDqsGatingRetry(DRAMC_CTX_T *p, bool bEn)
{
    if(bEn == 1)
    {
        vIO32WriteFldAlign_All(DRAMC_REG_SHU_DQSG_RETRY, 0, SHU_DQSG_RETRY_R_RETRY_ROUND_NUM);//Retry once
        vIO32WriteFldAlign_All(DRAMC_REG_TEST2_4, 4, TEST2_4_TESTAGENTRKSEL);//Dummy Read rank selection is controlled by Test Agent
        vIO32WriteFldMulti_All(DRAMC_REG_SHU_DQSG_RETRY, P_Fld(1, SHU_DQSG_RETRY_R_XSR_RETRY_SPM_MODE)
                                         | P_Fld(0, SHU_DQSG_RETRY_R_XSR_DQSG_RETRY_EN)
                                         | P_Fld(0, SHU_DQSG_RETRY_R_DQSG_RETRY_SW_EN));
        vIO32WriteFldMulti_All(DRAMC_REG_DUMMY_RD, P_Fld(1, DUMMY_RD_DQSG_DMYRD_EN)
                                         | P_Fld(p->support_rank_num, DUMMY_RD_RANK_NUM)
                                         | P_Fld(1, DUMMY_RD_DUMMY_RD_SW));
        vIO32WriteFldAlign_All(DRAMC_REG_RK1_DUMMY_RD_ADR, 0, RK1_DUMMY_RD_ADR_DMY_RD_RK1_LEN);
        vIO32WriteFldAlign_All(DRAMC_REG_RK0_DUMMY_RD_ADR, 0, RK0_DUMMY_RD_ADR_DMY_RD_RK0_LEN);
        vIO32WriteFldMulti_All(DRAMC_REG_SHU_DQSG_RETRY, P_Fld(1, SHU_DQSG_RETRY_R_RETRY_USE_BURST_MDOE)
                                         | P_Fld(1, SHU_DQSG_RETRY_R_RDY_SEL_DLE)
                                         | P_Fld(1, SHU_DQSG_RETRY_R_DDR1866_PLUS));
    }
    else
    {
        vIO32WriteFldMulti_All(DRAMC_REG_SHU_DQSG_RETRY, P_Fld(0, SHU_DQSG_RETRY_R_XSR_RETRY_SPM_MODE)
                                         | P_Fld(0, SHU_DQSG_RETRY_R_XSR_DQSG_RETRY_EN)
                                         | P_Fld(0, SHU_DQSG_RETRY_R_DQSG_RETRY_SW_EN));
    }
}

void HwSaveForSR(DRAMC_CTX_T *p)
{
    vIO32WriteFldMulti_All(DRAMC_REG_RSTMASK, P_Fld(0, RSTMASK_GT_SYNC_MASK)
                | P_Fld(0, RSTMASK_GT_SYNC_MASK_FOR_PHY));
    vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL1, 1, REFCTRL1_SLEFREF_AUTOSAVE_EN);
    vIO32WriteFldMulti_All(DRAMC_REG_SREFCTRL, P_Fld(1, SREFCTRL_SREF2_OPTION)
                | P_Fld(0, SREFCTRL_SREF3_OPTION));
}

#if (SET_CKE_2_RANK_INDEPENDENT | SET_CKE_2_RANK_INDEPENDENT_RUN_TIME)
void SetCKE2RankIndependent(DRAMC_CTX_T *p)
{
    //mcSHOW_DBG_MSG(("\n\t[CKE2RankIndependent]\n"));
    vIO32WriteFldMulti_All(DRAMC_REG_RKCFG, P_Fld(0, RKCFG_CKE2RANK)
                    | P_Fld(0, RKCFG_CKE2RANK_OPT)
                    | P_Fld(1, RKCFG_CKE2RANK_OPT2));

    vIO32WriteFldMulti_All(DRAMC_REG_CKECTRL, P_Fld(0, CKECTRL_CKE2RANK_OPT3)
                    | P_Fld(0, CKECTRL_CKE2RANK_OPT5)
                    | P_Fld(0, CKECTRL_CKE2RANK_OPT6)
                    | P_Fld(1, CKECTRL_CKE2RANK_OPT7)
                    | P_Fld(0, CKECTRL_CKE2RANK_OPT8)//Except WE1 is 1
                    | P_Fld(0, CKECTRL_CKETIMER_SEL)
                    | P_Fld(1, CKECTRL_FASTWAKE_SEL)
                    | P_Fld(0, CKECTRL_CKEWAKE_SEL));

    vIO32WriteFldMulti_All(DRAMC_REG_SHU_CONF2, P_Fld(1,SHU_CONF2_FASTWAKE) | P_Fld(1,SHU_CONF2_FASTWAKE2));
    vIO32WriteFldMulti_All(DRAMC_REG_SHU2_CONF2, P_Fld(1,SHU_CONF2_FASTWAKE) | P_Fld(1,SHU_CONF2_FASTWAKE2));
    vIO32WriteFldMulti_All(DRAMC_REG_SHU3_CONF2, P_Fld(1,SHU_CONF2_FASTWAKE) | P_Fld(1,SHU_CONF2_FASTWAKE2));

    vIO32WriteFldAlign_All(DRAMC_REG_DRAMCTRL, 0,DRAMCTRL_CLKWITRFC);
}
#endif

#ifdef CLK_FREE_FUN_FOR_DRAMC_PSEL
//If dramc enter SREF and power down, all configure need to sync 2T again after exit SREF.
//If Psel is 1, clock will be free run at the periof of 2T to let conf be applied. 
//If Psel is 0, Clock will be gated
void ClkFreeRunForDramcPsel(DRAMC_CTX_T *p)
{
    vIO32WriteFldMulti_All(DRAMC_REG_REFCTRL1, P_Fld(0, REFCTRL1_PSEL_OPT1)
            | P_Fld(0, REFCTRL1_PSEL_OPT2)
            | P_Fld(0, REFCTRL1_PSEL_OPT3));
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_CLKAR), 0, CLKAR_PSELAR);
}
#endif

#if PA_IMPROVEMENT_FOR_DRAMC_ACTIVE_POWER
void DramcPAImprove(DRAMC_CTX_T *p)
{
    vIO32WriteFldMulti_All(DRAMC_REG_CLKAR, P_Fld(0, CLKAR_REQQUE_PACG_DIS)
            | P_Fld(0, CLKAR_SELPH_CMD_CG_DIS));
    /* Dummy_RD_PA_OPT should be set to 1, or else some functions would fail (YH Tsai)
     * Already set to 1 in in UpdateInitialSettings(), so comment out set to 0 here
     */
    //vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD), 0, DUMMY_RD_DUMMY_RD_PA_OPT);
    vIO32WriteFldMulti_All(DRAMC_REG_SREFCTRL, P_Fld(0, SREFCTRL_SCSM_CGAR)
            | P_Fld(0, SREFCTRL_SCARB_SM_CGAR)
            | P_Fld(0, SREFCTRL_RDDQSOSC_CGAR)
            | P_Fld(0, SREFCTRL_HMRRSEL_CGAR));
    vIO32WriteFldAlign_All(DRAMC_REG_ZQCS, 0x0, ZQCS_ZQCS_MASK_SEL_CGAR);
    vIO32WriteFldAlign_All(DRAMC_REG_PRE_TDQSCK1, 0x0, PRE_TDQSCK1_TXUIPI_CAL_CGAR);

    vIO32WriteFldAlign_All(DRAMC_REG_ZQCS, 0x0, ZQCS_ZQCS_MASK_SEL_CGAR);//AE1/AE2/WE2/M17   
    vIO32WriteFldAlign_All(DRAMC_REG_PRE_TDQSCK1, 0x0, PRE_TDQSCK1_TXUIPI_CAL_CGAR);// AE1/AE2/WE2/M17 
    vIO32WriteFldMulti_All(DRAMC_REG_SHU_ODTCTRL, P_Fld(0x0, SHU_ODTCTRL_RODTENSTB_SELPH_CG_IG)
                                            | P_Fld(0x0, SHU_ODTCTRL_RODTEN_SELPH_CG_IG));
    
}
#endif

void Hynix_Test_Mode(DRAMC_CTX_T *p)
{
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

void DramcRunTimeConfig(DRAMC_CTX_T *p)
{
    vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL0, 0x0, REFCTRL0_REFDIS); //after k, auto refresh should be enable
    mcSHOW_DBG_MSG(("[DramcRunTimeConfig]\n"));

#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
    //#ifdef SPM_CONTROL_AFTERK
    TransferPLLToSPMControl(p);
    //#endif
    
    if(p->frequency>=1600)
        vIO32WriteFldAlign(SPM_SW_RSV_8, 3, SW_RSV_8_RX_TRACKING_EN);
#endif

#if ENABLE_TX_TRACKING
    if(u1IsLP4Family(p->dram_type))
    {
        U8 backup_channel= p->channel;
        U8 channelIdx;
        
        for(channelIdx=CHANNEL_A; channelIdx<p->support_channel_num; channelIdx++)
        {
            vSetPHY2ChannelMapping(p, channelIdx);
            DramcHwDQSOSC(p);
        }
        
        vSetPHY2ChannelMapping(p, backup_channel);
        mcSHOW_DBG_MSG(("TX_TRACKING: ON\n"));
    }
    else
    {
        mcSHOW_DBG_MSG(("TX_TRACKING: OFF\n"));
    }
#else
    mcSHOW_DBG_MSG(("TX_TRACKING: OFF\n"));
#endif

#if ENABLE_RX_TRACKING_LP4
    if(u1IsLP4Family(p->dram_type))
    {
        DramcRxInputDelayTrackingInit_Common(p);
        DramcRxInputDelayTrackingHW(p);
        mcSHOW_DBG_MSG(("RX_TRACKING: ON\n"));
    }
#else
    mcSHOW_DBG_MSG(("RX_TRACKING: OFF\n"));
#endif

#ifdef HW_GATING
        DramcHWGatingInit(p);                                   // HW gating initial before RunTime config.
        DramcHWGatingOnOff((DRAMC_CTX_T *) p, 1); // Enable HW gating tracking
        DramcHWGatingDebugOnOff(p, ENABLE);
        mcSHOW_DBG_MSG(("HW_GATING: ON\n"));
#else
        DramcHWGatingOnOff((DRAMC_CTX_T *) p, 0);
        mcSHOW_DBG_MSG(("HW_GATING: OFF\n"));
#endif

#ifdef DUMMY_READ_FOR_TRACKING
    if (u1IsLP4Family(p->dram_type))
    {
        if (p->frequency >= 1600)
        {
            DramcDummyReadForTrackingEnable(p);
            mcSHOW_DBG_MSG(("DUMMY_READ_FOR_TRACKING: ON\n"));
        }
        else
        {
            mcSHOW_DBG_MSG(("DUMMY_READ_FOR_TRACKING: OFF\n"));
        }
    }
    else
    {
        mcSHOW_DBG_MSG(("DUMMY_READ_FOR_TRACKING: OFF\n"));
    }
#else
    mcSHOW_DBG_MSG(("DUMMY_READ_FOR_TRACKING: OFF\n"));
#endif

//HW sync gating tracking
#if DFS_HW_SYNC_GATING_TRACKING
    vIO32WriteFldAlign_All(DRAMC_REG_RSTMASK, 0, RSTMASK_DVFS_SYNC_MASK);
    mcSHOW_DBG_MSG(("DFS_HW_SYNC_GATING_TRACKING: ON\n"));
#else
    mcSHOW_DBG_MSG(("DFS_HW_SYNC_GATING_TRACKING: OFF\n"));
#endif
    if(u1IsLP4Family(p->dram_type))
    {
#ifdef ZQCS_ENABLE_LP4
        // ZQCSMASK setting: (Ch A, Ch B) = (1,0) or (0,1)
        // if CHA.ZQCSMASK=1, and then set CHA.ZQCALDISB=1 first, else set CHB.ZQCALDISB=1 first
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), 1, SPCMDCTRL_ZQCALDISB);// LP3 and LP4 are different, be careful.
        mcSHOW_DBG_MSG(("ZQCS_ENABLE_LP4: ON\n"));
#else
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), 0, SPCMDCTRL_ZQCALDISB);// LP3 and LP4 are different, be careful.
        mcSHOW_DBG_MSG(("ZQCS_ENABLE_LP4: OFF\n"));
#endif
    }
    else
    {
#ifdef ZQCS_ENABLE_LP3
        vIO32WriteFldAlign_All(DRAMC_REG_SPCMDCTRL, 1, SPCMDCTRL_ZQCSDISB);
        mcSHOW_DBG_MSG(("ZQCS_ENABLE_LP3: ON\n"));
#else
        vIO32WriteFldAlign_All(DRAMC_REG_SPCMDCTRL, 0, SPCMDCTRL_ZQCSDISB);
        mcSHOW_DBG_MSG(("ZQCS_ENABLE_LP3: OFF\n"));
#endif
    }
#if APPLY_LOWPOWER_GOLDEN_SETTINGS
    DDRPhyLowPowerEnable(p);
#if (SW_CHANGE_FOR_SIMULATION==0)
    EnableDramcPhyDCM(p, 1);
#endif
    mcSHOW_DBG_MSG(("LOWPOWER_GOLDEN_SETTINGS(DCM): ON\n"));
#else
#if (SW_CHANGE_FOR_SIMULATION==0)
    EnableDramcPhyDCM(p, 0);
#endif
    mcSHOW_DBG_MSG(("LOWPOWER_GOLDEN_SETTINGS(DCM): OFF\n"));
#endif

#ifdef DUMMY_READ_FOR_DQS_GATING_RETRY
    if(u1IsLP4Family(p->dram_type))
    {
        DummyReadForDqsGatingRetry(p, 1);
        mcSHOW_DBG_MSG(("DUMMY_READ_FOR_DQS_GATING_RETRY: ON\n"));
    }
    else
    {
        DummyReadForDqsGatingRetry(p, 0);
        mcSHOW_DBG_MSG(("DUMMY_READ_FOR_DQS_GATING_RETRY: OFF\n"));
    }
#else
    mcSHOW_DBG_MSG(("DUMMY_READ_FOR_DQS_GATING_RETRY: OFF\n"));
#endif

#ifdef SPM_CONTROL_AFTERK
      TransferToSPMControl(p);  //don't enable in ETT
      mcSHOW_DBG_MSG(("SPM_CONTROL_AFTERK: ON\n"));
  #else
      mcSHOW_DBG_MSG(("SPM_CONTROL_AFTERK: OFF\n"));
#endif

#ifdef IMPEDANCE_TRACKING_ENABLE
    if(p->dram_type == TYPE_LPDDR4 || p->dram_type == TYPE_LPDDR4X)
    {
        DramcImpedanceTrackingEnable(p);
        mcSHOW_DBG_MSG(("IMPEDANCE_TRACKING: ON\n"));

#ifdef IMPEDANCE_HW_SAVING
        DramcImpedanceHWSaving(p);
#endif
    }
#else
    mcSHOW_DBG_MSG(("IMPEDANCE_TRACKING: OFF\n"));
#endif

    // 0x1c0[31]
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQSCAL0), 0, DQSCAL0_STBCALEN);

#ifdef TEMP_SENSOR_ENABLE
    if(u1IsLP4Family(p->dram_type))
    {
        //Enable HW-MRR4 : SPCMDCTRL_REFRDIS=0
        vIO32WriteFldAlign_All(DRAMC_REG_SPCMDCTRL, 0, SPCMDCTRL_REFRDIS);

        //HW-MRR4 don't block normal EMI request : SPCMDCTRL_REFR_BLOCKEN=0
        vIO32WriteFldAlign_All(DRAMC_REG_SPCMDCTRL, 0, SPCMDCTRL_REFR_BLOCKEN);
	
        //DRAMC will consider tMRR ac-timing : HW_MRR_FUN_TMRR_ENA =1
        vIO32WriteFldAlign_All(DRAMC_REG_HW_MRR_FUN, 1, HW_MRR_FUN_TMRR_ENA);

        //Set HW-MRR4 command in queue to high priority : R_DMMRR_HW_HIPRI = 1
        vIO32WriteFldAlign_All(DRAMC_REG_HW_MRR_FUN, 1, HW_MRR_FUN_MRR_HW_HIPRI);
    }
#if ENABLE_LP3_SW
    else
    {   
        //lp3 only has CHA
        vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0, SPCMDCTRL_REFRDIS);
    }
#endif /* ENABLE_LP3_SW */

    mcSHOW_DBG_MSG(("TEMP_SENSOR: ON\n"));
#else
    vIO32WriteFldAlign_All(DRAMC_REG_SPCMDCTRL, 1, SPCMDCTRL_REFRDIS);
    mcSHOW_DBG_MSG(("TEMP_SENSOR: OFF\n"));
#endif

#if ENABLE_PER_BANK_REFRESH
    vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL0, 1, REFCTRL0_PBREFEN);
    mcSHOW_DBG_MSG(("PER_BANK_REFRESH: ON\n"));
#else
    vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL0, 0, REFCTRL0_PBREFEN);
    mcSHOW_DBG_MSG(("PER_BANK_REFRESH: OFF\n"));
#endif

#if ENABLE_LP3_SW
    //Lewis@20160506:[DDR reserve mode] LPDDR3 Let CHB enter SR, let SR ACK always 1 to avoid WDT conf could not enter SR
    if (p->dram_type == TYPE_LPDDR3)
    {
        DRAM_CHANNEL_T backChannel = p->channel;
        p->channel = CHANNEL_B;
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SREFCTRL), 1, SREFCTRL_SELFREF);//Let CHB enter SR
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), 1, RKCFG_RK0DPD);//Set to let CHB SREF_STATE always 1, fix(a)LP3 WDT TO to let DRAM into SR fail at second time (b) LP3 could not enter SR in S0
        p->channel = backChannel;
    }
#endif /* ENABLE_LP3_SW */

#ifdef HW_SAVE_FOR_SR
    HwSaveForSR(p);
    mcSHOW_DBG_MSG(("HW_SAVE_FOR_SR: ON\n"));
#else
    mcSHOW_DBG_MSG(("HW_SAVE_FOR_SR: OFF\n"));
#endif

#if SET_CKE_2_RANK_INDEPENDENT_RUN_TIME
    SetCKE2RankIndependent(p);
    mcSHOW_DBG_MSG(("SET_CKE_2_RANK_INDEPENDENT_RUN_TIME: ON\n"));
#else
    mcSHOW_DBG_MSG(("SET_CKE_2_RANK_INDEPENDENT_RUN_TIME: OFF\n"));
#endif

#ifdef CLK_FREE_FUN_FOR_DRAMC_PSEL
    ClkFreeRunForDramcPsel(p);
    mcSHOW_DBG_MSG(("CLK_FREE_FUN_FOR_DRAMC_PSEL: ON\n"));
#else
    mcSHOW_DBG_MSG(("CLK_FREE_FUN_FOR_DRAMC_PSEL: OFF\n"));
#endif

#if PA_IMPROVEMENT_FOR_DRAMC_ACTIVE_POWER
    DramcPAImprove(p);
    mcSHOW_DBG_MSG(("PA_IMPROVEMENT_FOR_DRAMC_ACTIVE_POWER: ON\n"));
#else
    mcSHOW_DBG_MSG(("PA_IMPROVEMENT_FOR_DRAMC_ACTIVE_POWER: OFF\n"));
#endif

#if ENABLE_RODT_TRACKING
    mcSHOW_DBG_MSG(("Read ODT Tracking: ON\n"));
#else
    mcSHOW_DBG_MSG(("Read ODT Tracking: OFF\n"));
#endif

#if TDQSCK_PRECALCULATION_FOR_DVFS
#if DUAL_FREQ_K
        DramcDQSPrecalculation_preset(p);
        mcSHOW_DBG_MSG(("DQS Precalculation for DVFS: ON\n"));
#endif
#else
        mcSHOW_DBG_MSG(("DQS Precalculation for DVFS: OFF\n"));
#endif
    mcSHOW_DBG_MSG(("=========================\n"));

#if ENABLE_DRS
    if (p->support_rank_num==RANK_DUAL)
        DramcDRS(p, 1);
    else
        DramcDRS(p, 0);
#endif

#if ENABLE_DLL_MASTER_SLAVE_SHUFFLE
    vIO32WriteFldAlign_All(DDRPHY_DVFS_EMI_CLK, 1, DVFS_EMI_CLK_RG_DLL_SHUFFLE);
    vIO32WriteFldAlign_All(DRAMC_REG_DVFSDLL, 1, DVFSDLL_RG_DLL_SHUFFLE);
#endif 

#if DUMP_GOLDEN_SETTING
    DumpGoldenSetting();
#endif
}


void DramcSetRankEngine2(DRAMC_CTX_T *p, U8 u1RankSel)
{
    //LPDDR2_3_ADRDECEN_TARKMODE =0, always rank0
    /* ADRDECEN_TARKMODE: rank input selection
     *   1'b1 select CTO_AGENT1_RANK, 1'b0 rank by address decode
     */
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMCTRL), 1, DRAMCTRL_ADRDECEN_TARKMODE);

    // DUMMY_TESTAGENTRKSEL =0, select rank according to CATRAIN_TESTAGENTRK
    /* TESTAGENTRKSEL: Test agent access rank mode selection
     * 2'b00: rank selection by TESTAGENTRK,           2'b01: rank selection by CTO_AGENT_1_BK_ADR[0]
     * 2'b10: rank selection by CTO_AGENT1_COL_ADR[3], 2'b11: rank selection by CTO_AGENT1_COL_ADR[4]
     */
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), 0, TEST2_4_TESTAGENTRKSEL);

    //CATRAIN_TESTAGENTRK = u1RankSel
    /* TESTAGENTRK: Specify test agent rank
     * 2'b00 rank 0, 2'b01 rank 1, 2'b10 rank 2
     */
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), u1RankSel, TEST2_4_TESTAGENTRK);
}

//-------------------------------------------------------------------------
/** DramcEngine2
 *  start the self test engine 2 inside dramc to test dram w/r.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param  wr              (DRAM_TE_OP_T): TE operation
 *  @param  test2_1         (U32): 28bits,base address[27:0].
 *  @param  test2_2         (U32): 28bits,offset address[27:0]. (unit is 16-byte, i.e: 0x100 is 0x1000).
 *  @param  loopforever     (S16): 0    read\write one time ,then exit
 *                                >0 enable eingie2, after "loopforever" second ,write log and exit
 *                                -1 loop forever to read\write, every "period" seconds ,check result ,only when we find error,write log and exit
 *                                -2 loop forever to read\write, every "period" seconds ,write log ,only when we find error,write log and exit
 *                                -3 just enable loop forever ,then exit
 *  @param period           (U8):  it is valid only when loopforever <0; period should greater than 0
 *  @param log2loopcount    (U8): test loop number of test agent2 loop number =2^(log2loopcount) ,0 one time
 *  @retval status          (U32): return the value of DM_CMP_ERR  ,0  is ok ,others mean  error
 */
//-------------------------------------------------------------------------
static U32 uiReg0D0h;
DRAM_STATUS_T DramcEngine2Init(DRAMC_CTX_T *p, U32 test2_1, U32 test2_2, U8 testaudpat, U8 log2loopcount)
{
    U8 Use_Len1_Flag;

    // error handling
    if (!p)
    {
        mcSHOW_ERR_MSG(("context is NULL\n"));
        return DRAM_FAIL;
    }

    // check loop number validness
//    if ((log2loopcount > 15) || (log2loopcount < 0))		// U8 >=0 always.
    if (log2loopcount > 15)
    {
        mcSHOW_ERR_MSG(("wrong param: log2loopcount > 15\n"));
        return DRAM_FAIL;
    }

    Use_Len1_Flag = testaudpat & 0x80;
    testaudpat = testaudpat & 0x7f;

    DramcSetRankEngine2(p, p->rank);

    uiReg0D0h=u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD), P_Fld(0, DUMMY_RD_DQSG_DMYRD_EN) | P_Fld(0, DUMMY_RD_DQSG_DMYWR_EN) | P_Fld(0, DUMMY_RD_DUMMY_RD_EN) | P_Fld(0, DUMMY_RD_SREF_DMYRD_EN) | P_Fld(0, DUMMY_RD_DMY_RD_DBG) | P_Fld(0, DUMMY_RD_DMY_WR_DBG));  //must close dummy read when do test agent

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TESTCHIP_DMA1), 0, TESTCHIP_DMA1_DMA_LP4MATAB_OPT);

    // disable self test engine1 and self test engine2
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(0, TEST2_3_TEST2W) | P_Fld(0, TEST2_3_TEST2R) | P_Fld(0, TEST2_3_TEST1));


    // 1.set pattern ,base address ,offset address
    // 2.select  ISI pattern or audio pattern or xtalk pattern
    // 3.set loop number
    // 4.enable read or write
    // 5.loop to check DM_CMP_CPT
    // 6.return CMP_ERR
    // currently only implement ucengine_status = 1, others are left for future extension

    // 1
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_0), P_Fld(test2_1>>24,TEST2_0_TEST2_PAT0)|P_Fld(test2_2>>24,TEST2_0_TEST2_PAT1));

#if (FOR_DV_SIMULATION_USED==1 || SW_CHANGE_FOR_SIMULATION==1)
    //DV sim memory 0~0x100 has values, can't used
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_1), (test2_1+0x100) & 0x00ffffff, TEST2_1_TEST2_BASE);
#else
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_1), test2_1 & 0x00ffffff, TEST2_1_TEST2_BASE);
#endif
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_2), test2_2 & 0x00ffffff, TEST2_2_TEST2_OFF);

    // 2 & 3
    // (TESTXTALKPAT, TESTAUDPAT) = 00 (ISI), 01 (AUD), 10 (XTALK), 11 (UNKNOWN)
    if (testaudpat == 2)   // xtalk
    {
        //TEST_REQ_LEN1=1 is new feature, hope to make dq bus continously.
        //but DV simulation will got problem of compare err
        //so go back to use old way
        //TEST_REQ_LEN1=0, R_DMRWOFOEN=1
        if (Use_Len1_Flag != 0)
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), P_Fld(1, TEST2_4_TEST_REQ_LEN1));   //test agent 2 with cmd length = 0        
        }
        else
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), P_Fld(0, TEST2_4_TEST_REQ_LEN1));   //test agent 2 with cmd length = 0
        }
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_PERFCTL0), P_Fld(1, PERFCTL0_RWOFOEN));

        // select XTALK pattern
        // set addr 0x044 [7] to 0
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(0, TEST2_3_TESTAUDPAT)|P_Fld(log2loopcount,TEST2_3_TESTCNT)); //dont use audio pattern

        // set addr 0x48[16] to 1, TESTXTALKPAT = 1
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), P_Fld(1, TEST2_4_TESTXTALKPAT)|P_Fld(0,TEST2_4_TESTAUDMODE)|P_Fld(0,TEST2_4_TESTAUDBITINV));  //use XTALK pattern, dont use audio pattern

        // R_DMTESTSSOPAT=0, R_DMTESTSSOXTALKPAT=0
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), P_Fld(0, TEST2_4_TESTSSOPAT)|P_Fld(0,TEST2_4_TESTSSOXTALKPAT));   //dont use sso, sso+xtalk pattern
    }
    else if (testaudpat == 1)   // audio
    {
        // set AUDINIT=0x11 AUDINC=0x0d AUDBITINV=1 AUDMODE=1
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), \
            P_Fld(0x00000011, TEST2_4_TESTAUDINIT)|P_Fld(0x0000000d, TEST2_4_TESTAUDINC)| \
            P_Fld(0, TEST2_4_TESTXTALKPAT)|P_Fld(0,TEST2_4_TESTAUDMODE)|P_Fld(1,TEST2_4_TESTAUDBITINV));

        // set addr 0x044 [7] to 1 ,select audio pattern
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(1, TEST2_3_TESTAUDPAT)|P_Fld(log2loopcount,TEST2_3_TESTCNT));
    }
    else   // ISI
    {
        // select ISI pattern
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(0, TEST2_3_TESTAUDPAT)|P_Fld(log2loopcount,TEST2_3_TESTCNT));
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), 0, TEST2_4_TESTXTALKPAT);
    }
    return DRAM_OK;
}


U32 DramcEngine2Run(DRAMC_CTX_T *p, DRAM_TE_OP_T wr, U8 testaudpat)
{
    U32 u4loop_count = 0;
    U32 u4result = 0xffffffff;

    // 4
    if (wr == TE_OP_READ_CHECK)
    {
        if ((testaudpat == 1) || (testaudpat == 2))
        {
            //if audio pattern, enable read only (disable write after read), AUDMODE=0x48[15]=0
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), 0, TEST2_4_TESTAUDMODE);
        }

        // enable read, 0x008[31:29]
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(0, TEST2_3_TEST2W) | P_Fld(1, TEST2_3_TEST2R) | P_Fld(0, TEST2_3_TEST1));
    }
    else if (wr == TE_OP_WRITE_READ_CHECK)
    {
        // enable write
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(1, TEST2_3_TEST2W) | P_Fld(0, TEST2_3_TEST2R) | P_Fld(0, TEST2_3_TEST1));

        // read data compare ready check
        u4loop_count = 0;
        while((u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TESTRPT), TESTRPT_DM_CMP_CPT_RK0))==0)
        {
            //ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TESTRPT), &u4value);
            mcDELAY_US(CMP_CPT_POLLING_PERIOD);
            u4loop_count++;
            if ((u4loop_count > 3) &&(u4loop_count <= MAX_CMP_CPT_WAIT_LOOP))
            {
                //mcSHOW_ERR_MSG(("TESTRPT_DM_CMP_CPT (Write): %d\n", u4loop_count));
            }
            else if(u4loop_count > MAX_CMP_CPT_WAIT_LOOP)
            {
               /*TINFO="fcWAVEFORM_MEASURE_A %d: time out\n", u4loop_count*/
               mcSHOW_DBG_MSG(("fcWAVEFORM_MEASURE_A %d :time out, [22:20]=0x%x\n", u4loop_count, u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TESTRPT), TESTRPT_TESTSTAT)));
               mcFPRINTF((fp_A60501, "fcWAVEFORM_MEASURE_A %d: time out\n", u4loop_count));
                break;
            }
        }

        // disable write
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(0, TEST2_3_TEST2W) | P_Fld(0, TEST2_3_TEST2R) | P_Fld(0, TEST2_3_TEST1));

        mcDELAY_US(1);

        // enable read
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(0, TEST2_3_TEST2W) | P_Fld(1, TEST2_3_TEST2R) | P_Fld(0, TEST2_3_TEST1));
    }

    // 5
    // read data compare ready check
    u4loop_count = 0;
    while((u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TESTRPT), TESTRPT_DM_CMP_CPT_RK0))==0)
    {
            mcDELAY_US(CMP_CPT_POLLING_PERIOD);
            u4loop_count++;
            if ((u4loop_count > 3)&&(u4loop_count <= MAX_CMP_CPT_WAIT_LOOP))
            {
                //mcSHOW_ERR_MSG(("TESTRPT_DM_CMP_CPT (Read): %d\n", u4loop_count));
            }
            else if(u4loop_count > MAX_CMP_CPT_WAIT_LOOP)
            {
               /*TINFO="fcWAVEFORM_MEASURE_B %d: time out\n", u4loop_count*/
               mcSHOW_DBG_MSG3(("fcWAVEFORM_MEASURE_B %d :time out, [22:20]=0x%x\n", u4loop_count, u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TESTRPT), TESTRPT_TESTSTAT)));               
               mcFPRINTF((fp_A60501, "fcWAVEFORM_MEASURE_B %d: time out\n", u4loop_count));
               break;
            }
    }

    // delay 10ns after ready check from DE suggestion (1ms here)
    mcDELAY_US(1);

    // 6
    // return CMP_ERR, 0 is ok ,others are fail,diable test2w or test2r
    // get result
    // or all result
    u4result = (u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CMP_ERR)));
    // disable read
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(0, TEST2_3_TEST2W) | P_Fld(0, TEST2_3_TEST2R) | P_Fld(0, TEST2_3_TEST1));

    return u4result;
}

void DramcEngine2End(DRAMC_CTX_T *p)
{
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), P_Fld(0, TEST2_4_TEST_REQ_LEN1));   //test agent 2 with cmd length = 0

    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD), uiReg0D0h);
}

// Full set of usage test engine 2, including of DramcEngine2Init->DramcEngine2Run->DramcEngine2End
// if you don't care the performance, and just for convinent, you may use this API (TestEngineCompare)
U32 TestEngineCompare(DRAMC_CTX_T *p)
{
    U8 jj;
    U32 u4err_value;

    if(p->test_pattern <= TEST_XTALK_PATTERN)
    {
        DramcEngine2Init(p, p->test2_1, p->test2_2, p->test_pattern, 0);
        u4err_value = DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, p->test_pattern);
        DramcEngine2End(p);
    }
    else if (p->test_pattern == TEST_MIX_PATTERN)
    {
        DramcEngine2Init(p, p->test2_1, p->test2_2, TEST_AUDIO_PATTERN, 0);
        u4err_value = DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, TEST_AUDIO_PATTERN);
        DramcEngine2End(p);

        DramcEngine2Init(p, p->test2_1, p->test2_2, TEST_XTALK_PATTERN, 0);
        u4err_value |= DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, TEST_XTALK_PATTERN);
        DramcEngine2End(p);
    }
    else
    {
        mcSHOW_ERR_MSG(("Test pattern error! Using default xtalk pattern\n"));
        DramcEngine2Init(p, p->test2_1, p->test2_2, TEST_XTALK_PATTERN, 0);
        u4err_value = DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, TEST_XTALK_PATTERN);
        DramcEngine2End(p);
    }
    return u4err_value;
}


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
        mcSHOW_DBG_MSG(("Reg(0x%xh) Addr 0x%X = 0x%X\n", (ii&0xfff)>>2, ii, (*(volatile unsigned int *)(ii))));
    }
}

/* Calculate regs dump range for each type
 * Using last register definition of each group
 * Ex: Bianco DramC NAO last register definition
 *     #define DRAMC_REG_RK2_B23_STB_DBG_INFO_15 (DRAMC_NAO_BASE_ADDRESS + 0x08FC)
 *     Dump range end offset would be (DRAMC_REG_RK2_B23_STB_DBG_INFO_15 - DRAMC_NAO_BASE_ADDRESS)
 */
 #if (fcFOR_CHIP_ID == fcBianco)
#define DRAMC_NAO_DUMP_RANGE (DRAMC_REG_RK2_B23_STB_DBG_INFO_15 - DRAMC_NAO_BASE_ADDRESS)
#define DRAMC_AO_DUMP_RANGE (DRAMC_REG_SHU3_DQSG_RETRY - DRAMC_AO_BASE_ADDRESS)
#define DDRPHY_NAO_DUMP_RANGE (DDRPHY_RFU_0X1ACC - DDRPHY_AO_BASE_ADDR)
#define DDRPHY_AO_DUMP_RANGE (DDRPHY_MISC_PHY_RGS_STBEN_CMD - DDRPHY_NAO_BASE_ADDR)
#endif
DRAM_STATUS_T DramcRegDump(DRAMC_CTX_T *p)
{    /* Must modify Dump range end address accordingly for each project */
     /* Tip, add mcSHOW_DBG_MSGDUMP and set a large delay value for DramcRegDump related message output */
    mcSHOW_DBG_MSG(("\n\n\tCHA_DRAMC_NAO_BASE\n"));
    DramcRegDumpRange(Channel_A_DRAMC_NAO_BASE_ADDRESS, Channel_A_DRAMC_NAO_BASE_ADDRESS + DRAMC_NAO_DUMP_RANGE);
    mcSHOW_DBG_MSG(("\n\tCHB_DRAMC_NAO_BASE\n"));
    DramcRegDumpRange(Channel_B_DRAMC_NAO_BASE_ADDRESS, Channel_B_DRAMC_NAO_BASE_ADDRESS + DRAMC_NAO_DUMP_RANGE);

    mcSHOW_DBG_MSG(("\n\tCHA_DRAMC_AO_BASE\n"));
    DramcRegDumpRange(Channel_A_DRAMC_AO_BASE_ADDRESS, Channel_A_DRAMC_AO_BASE_ADDRESS + DRAMC_AO_DUMP_RANGE);
    mcSHOW_DBG_MSG(("\n\tCHB_DRAMC_AO_BASE\n"));
    DramcRegDumpRange(Channel_B_DRAMC_AO_BASE_ADDRESS, Channel_B_DRAMC_AO_BASE_ADDRESS + DRAMC_AO_DUMP_RANGE);

    mcSHOW_DBG_MSG(("\n\tCHA_PHY_NAO_BASE\n"));
    DramcRegDumpRange(Channel_A_PHY_NAO_BASE_ADDRESS, Channel_A_PHY_NAO_BASE_ADDRESS + DDRPHY_NAO_DUMP_RANGE);
    mcSHOW_DBG_MSG(("\n\tCHB_PHY_NAO_BASE\n"));
    DramcRegDumpRange(Channel_B_PHY_NAO_BASE_ADDRESS, Channel_B_PHY_NAO_BASE_ADDRESS + DDRPHY_NAO_DUMP_RANGE);

    mcSHOW_DBG_MSG(("\n\tCHA_PHY_AO_BASE\n"));
    DramcRegDumpRange(Channel_A_PHY_AO_BASE_ADDRESS, Channel_A_PHY_AO_BASE_ADDRESS + DDRPHY_AO_DUMP_RANGE);
    mcSHOW_DBG_MSG(("\n\tCHB_PHY_AO_BASE\n"));
    DramcRegDumpRange(Channel_B_PHY_AO_BASE_ADDRESS, Channel_B_PHY_AO_BASE_ADDRESS + DDRPHY_AO_DUMP_RANGE);

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




void DramcTest_DualSch_stress(DRAMC_CTX_T *p)
{
    U32 count = 0;
    U16 u2Value = 0;

    vIO32WriteFldAlign_All(DRAMC_REG_PERFCTL0, 1, PERFCTL0_DUALSCHEN);
    while(count<10)
    {
        count++;

        u1MR12Value[p->channel][p->rank][p->dram_fsp] = 0x14;
        DramcModeRegWrite(p, 12, u1MR12Value[p->channel][p->rank][p->dram_fsp]);
        DramcModeRegRead(p, 12, &u2Value);
        //mcSHOW_DBG_MSG(("MR12 = 0x%0X\n", u1Value));
    }
}


void DramcModeRegRead(DRAMC_CTX_T *p, U8 u1MRIdx, U16 *u2pValue)
{
    U32 u4MRValue;
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), p->rank, MRS_MRRRK);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1MRIdx, MRS_MRSMA);

    // MRR command will be fired when MRREN 0->1
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1, SPCMD_MRREN);

    // wait MRR command fired.
    while(u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_MRR_RESPONSE) ==0)
    {
        mcDELAY_US(1);
    }

    // Read out mode register value
    u4MRValue = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRR_STATUS), MRR_STATUS_MRR_REG);
    *u2pValue = (U16)u4MRValue;

    // Set MRREN =0 for next time MRR.
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_MRREN);

    mcSHOW_DBG_MSG3(("Read MR%d =0x%x\n", u1MRIdx, u4MRValue));
}


void DramcModeRegReadByRank(DRAMC_CTX_T *p, U8 u1Rank, U8 u1MRIdx, U16 *u2pValue)
{
    U16 u2Value = 0;
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1Rank, MRS_MRRRK);
    DramcModeRegRead(p,u1MRIdx, &u2Value);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), 0, MRS_MRRRK);
    *u2pValue = u2Value;
}

void DramcModeRegWrite(DRAMC_CTX_T *p, U8 u1MRIdx, U8 u1Value)
{
    U32 counter=0;
    U32 u4Rank = 0;
    U32 u4register_024;

    //backup register of CKE fix on/off
    u4register_024 = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL));
    
    //CKE must be fix on when doing MRW
    CKEFixOnOff(p, CKE_FIXON, CKE_WRITE_TO_ONE_CHANNEL);
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), p->rank, MRS_MRSRK);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1MRIdx, MRS_MRSMA);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1Value, MRS_MRSOP);

    // MRW command will be fired when MRWEN 0->1
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1, SPCMD_MRWEN);

    // wait MRW command fired.
    u4Rank = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), MRS_MRSRK);
    while(u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_MRW_RESPONSE) ==0)
    {
        counter++;
        mcSHOW_DBG_MSG2(("wait MRW command Rank%d MR%d =0x%x fired (%d)\n", u4Rank, u1MRIdx, u1Value, counter));
        mcDELAY_US(1);
    }

    // Set MRWEN =0 for next time MRW.
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_MRWEN);
    
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), u4register_024); //restore CKEFIXON value 
    
    u4Rank = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), MRS_MRSRK);
    if(u1PrintModeRegWrite)
    {
        #if VENDER_JV_LOG
        mcSHOW_DBG_MSG5(("Write Rank%d MR%d =0x%x\n", u4Rank, u1MRIdx, u1Value));
        #endif
        mcSHOW_DBG_MSG3(("Write Rank%d MR%d =0x%x\n", u4Rank, u1MRIdx, u1Value));
        mcFPRINTF((fp_A60501, "Write Rank%d MR%d =0x%x\n", u4Rank, u1MRIdx, u1Value));
    }
}

void DramcModeRegWriteByRank(DRAMC_CTX_T *p, U8 u1Rank, U8 u1MRIdx, U8 u1Value)
{
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1Rank, MRS_MRSRK);
    DramcModeRegWrite(p,u1MRIdx, u1Value);
}

#define SHUFFLE_GROUP   4               //SHU1~4

#if DUAL_FREQ_K
#define SHUFFLE_ADDR_NUM_DRAMC          4
#define SHUFFLE_ADDR_NUM_DDRPHY         14
#define SHUFFLE_GROPU_OFFSET_DRAMC      0x600
#define SHUFFLE_GROPU_OFFSET_DDRPHY     0x500

typedef struct _SHUFFLE_REG_ADDR
{
    U32 u4StartAddr;
    U32 u4EndAddr;
} SHUFFLE_REG_ADDR;

const SHUFFLE_REG_ADDR ShuffleRegTableDramc[SHUFFLE_ADDR_NUM_DRAMC] =
{
    {DRAMC_AO_BASE_ADDRESS+(0x0800), DRAMC_AO_BASE_ADDRESS+(0x08DC) },
    {DRAMC_AO_BASE_ADDRESS+(0x0A00), DRAMC_AO_BASE_ADDRESS+(0x0A50) },
    {DRAMC_AO_BASE_ADDRESS+(0x0B00), DRAMC_AO_BASE_ADDRESS+(0x0B50) },
    {DRAMC_AO_BASE_ADDRESS+(0x0C00), DRAMC_AO_BASE_ADDRESS+(0x0C54) },
};
const SHUFFLE_REG_ADDR ShuffleRegTableDDRPhy[SHUFFLE_ADDR_NUM_DDRPHY] =
{
    {DDRPHY_AO_BASE_ADDR+(0x0C00), DDRPHY_AO_BASE_ADDR+(0x0C38) },
    {DDRPHY_AO_BASE_ADDR+(0x0C80), DDRPHY_AO_BASE_ADDR+(0x0CB8) },
    {DDRPHY_AO_BASE_ADDR+(0x0D00), DDRPHY_AO_BASE_ADDR+(0x0D38) },
    {DDRPHY_AO_BASE_ADDR+(0x0D80), DDRPHY_AO_BASE_ADDR+(0x0DBC) },//PLL
    {DDRPHY_AO_BASE_ADDR+(0x0DD0), DDRPHY_AO_BASE_ADDR+(0x0DF0) },//PLL    
    {DDRPHY_AO_BASE_ADDR+(0x0E00), DDRPHY_AO_BASE_ADDR+(0x0E2C) },//R0
    {DDRPHY_AO_BASE_ADDR+(0x0E50), DDRPHY_AO_BASE_ADDR+(0x0E7C) },
    {DDRPHY_AO_BASE_ADDR+(0x0EA0), DDRPHY_AO_BASE_ADDR+(0x0ECC) },
    {DDRPHY_AO_BASE_ADDR+(0x0F00), DDRPHY_AO_BASE_ADDR+(0x0F2C) },//R1
    {DDRPHY_AO_BASE_ADDR+(0x0F50), DDRPHY_AO_BASE_ADDR+(0x0F7C) },
    {DDRPHY_AO_BASE_ADDR+(0x0FA0), DDRPHY_AO_BASE_ADDR+(0x0FCC) },
    {DDRPHY_AO_BASE_ADDR+(0x1000), DDRPHY_AO_BASE_ADDR+(0x102C) },//R2
    {DDRPHY_AO_BASE_ADDR+(0x1050), DDRPHY_AO_BASE_ADDR+(0x107C) },
    {DDRPHY_AO_BASE_ADDR+(0x10A0), DDRPHY_AO_BASE_ADDR+(0x10CC) },
};

#if REG_SHUFFLE_REG_CHECK
void ShuffleRegCheckProgram(U32 u4Addr)
{
#ifndef OLYMPUS_TO_BE_PORTING

    U32 Offset, TmpAddr;

    if((u4Addr >= DRAMC_AO_BASE_ADDRESS)  && (u4Addr < DDRPHY_BASE_ADDR))
    {
        TmpAddr = (DRAMC_AO_BASE_ADDRESS | (u4Addr &0xffff));
    }
    else
    {
        TmpAddr = (DDRPHY_BASE_ADDR | (u4Addr &0xffff));
    }

        for (Offset = 0; Offset < FREQREG_SIZE; Offset++)
        {
            if(TmpAddr ==LowFreq_ShuffleReg[Offset].uiSourceAddr)
            {
                mcSHOW_DBG_MSG(("\n[ShuffleRegCheck] OK 0x%x \n",u4Addr));
                mcFPRINTF((fp_A60501, "\n[ShuffleRegCheck] OK 0x%x\n",u4Addr));
                break;
            }
        }

        if(Offset ==FREQREG_SIZE)
        {
            mcSHOW_DBG_MSG(("\n[ShuffleRegCheck] Not in shuffle 0x%x \n",u4Addr));
            mcFPRINTF((fp_A60501, "\n[ShuffleRegCheck] Not in shuffle 0x%x\n",u4Addr));
        }
#endif
}
#endif


#if __ETT__
void DramcDumpFreqSetting(DRAMC_CTX_T *p)
{
    U32 Offset;
    U32 u4RegValue;
    U32 i,j;
    U32 tbIdx;

    mcSHOW_DBG_MSG(("Dump shuffle regs\n"));
    for(i=0; i<CHANNEL_NUM; i++)
    {
        mcSHOW_DBG_MSG(("DRAMC CH_%d\n", i));
        for(tbIdx=0; tbIdx<SHUFFLE_ADDR_NUM_DRAMC; tbIdx++)
        {
            U32 normalAddr=ShuffleRegTableDramc[tbIdx].u4StartAddr+((U32)i << POS_BANK_NUM);
            while(normalAddr<=ShuffleRegTableDramc[tbIdx].u4EndAddr+((U32)i << POS_BANK_NUM))
            {
                for(j=0; j<SHUFFLE_GROUP; j++)
                {
                    U32 shuffleAddr = normalAddr + SHUFFLE_GROPU_OFFSET_DRAMC*j;
                    U32 regValue = u4IO32Read4B(shuffleAddr);

                    #ifdef ETT_PRINT_FORMAT
                    mcSHOW_DBG_MSG(("[SHU%d]Addr 0x%X=0x%X ", j,  shuffleAddr, regValue));
                    #else
                    mcSHOW_DBG_MSG(("[SHU%2d] Addr 0x%8xh=0x%8xh  ", j,  shuffleAddr, regValue));
                    #endif
                }
                mcSHOW_DBG_MSG(("\n"));
                normalAddr+=4;
            }
        }
    }

    //PHY
    for(i=0; i<CHANNEL_NUM; i++)
    {
        mcSHOW_DBG_MSG(("DDRPHY CH_%d\n", i));
        for(tbIdx=0; tbIdx<SHUFFLE_ADDR_NUM_DDRPHY; tbIdx++)
        {
            U32 normalAddr=ShuffleRegTableDDRPhy[tbIdx].u4StartAddr+((U32)i << POS_BANK_NUM);
            while(normalAddr<=ShuffleRegTableDDRPhy[tbIdx].u4EndAddr+((U32)i << POS_BANK_NUM))
            {
                for(j=0; j<SHUFFLE_GROUP; j++)
                {
                    U32 shuffleAddr = normalAddr + SHUFFLE_GROPU_OFFSET_DDRPHY*j;
                    U32 regValue = u4IO32Read4B(shuffleAddr);

                    #ifdef ETT_PRINT_FORMAT
                    mcSHOW_DBG_MSG(("[SHU%d]Addr 0x%X=0x%X ", j,  shuffleAddr, regValue));
                    #else
                    mcSHOW_DBG_MSG(("[SHU%2d]Addr 0x%8xh=0x%8xh  ", j,  shuffleAddr, regValue));
                    #endif
                }
                mcSHOW_DBG_MSG(("\n"));
                normalAddr+=4;
            }
        }
    }
}
#endif  //__ETT__

#if __ETT__ || defined(SLT)
void DramcDFSTestCode(DRAMC_CTX_T *p)
{
#ifdef HW_GATING
    if(p->dram_type == TYPE_LPDDR3|| (u1IsLP4Family(p->dram_type)))
    {
        vIO32WriteFldMulti_All(DRAMC_REG_SHUCTRL2, P_Fld(0x1, SHUCTRL2_R_DVFS_OPTION)
                | P_Fld(0x1, SHUCTRL2_R_DVFS_PARK_N));
    }
    else
    {
        vIO32WriteFldMulti_All(DRAMC_REG_SHUCTRL2, P_Fld(0x0, SHUCTRL2_R_DVFS_OPTION)
                    | P_Fld(0x0, SHUCTRL2_R_DVFS_PARK_N));
    }
#else
    vIO32WriteFldMulti_All(DRAMC_REG_SHUCTRL2, P_Fld(0x0, SHUCTRL2_R_DVFS_OPTION)
                    | P_Fld(0x0, SHUCTRL2_R_DVFS_PARK_N));
#endif
//    vIO32WriteFldMulti_All(DRAMC_REG_SHUCTRL2, P_Fld(0x3F, SHUCTRL2_R_DLL_IDLE));
//    vIO32WriteFldAlign_All(DRAMC_REG_DVFSDLL, 0xFFFFFFFF, Fld(32,0,AC_FULLDW));
//    vIO32WriteFldAlign_All(DRAMC_REG_SHU_MISC, 0x0, SHU_MISC_REQQUE_MAXCNT);
//    vIO32WriteFldMulti_All(DRAMC_REG_SHUCTRL2,  P_Fld(0xFF, SHUCTRL2_SHU_PERIOD_GO_ZERO_CNT));
}

#if DUAL_FREQ_K
DRAM_PLL_FREQ_SEL_T currFreqSel=PLL_FREQ_SEL_MAX;
void DramcDFS(DRAMC_CTX_T *p, int iDoDMA)
{
    DRAM_PLL_FREQ_SEL_T targetFreqSel = p->freq_sel;
    //if(currFreqSel==p->freq_sel)
    //    return;
    mcSHOW_ERR_MSG(("[DramcDFS] currFreqSel=%d, target=%d\n", currFreqSel, targetFreqSel));

#ifdef IMPEDANCE_TRACKING_ENABLE
    vIO32WriteFldAlign_All(DRAMC_REG_IMPCAL, 0, IMPCAL_IMPCAL_HW);
#endif

#if __ETT__
    if(iDoDMA)
    {
        DramcCompareDMA(p);
        DramcTriggerDMA();
    }
#endif    
    
#if ENABLE_DRS
    EnableDRS_EMI(0);
#endif
        
    ///TODO: DFS scenario
    if(targetFreqSel==LP4_DDR3200)
    {
        DramcDFSDirectJump(p, 0);
    }    
    else if(targetFreqSel==LP4_DDR2400)
    {
        DramcDFSDirectJump(p, 2);
    }
    else if(targetFreqSel==LP4_DDR1600)
    {
        DramcDFSDirectJump(p, 1);
    }
    
    if(targetFreqSel==LP3_DDR1866)
    {
        DramcDFSDirectJump(p, 0);
    }    
    else if(targetFreqSel==LP3_DDR1600)
    {
        DramcDFSDirectJump(p, 1);
    }
    else if(targetFreqSel==LP3_DDR1200)
    {
        DramcDFSDirectJump(p, 2);
    }

#if ENABLE_DRS
    EnableDRS_EMI(1);
#endif

#ifdef IMPEDANCE_TRACKING_ENABLE
    vIO32WriteFldAlign_All(DRAMC_REG_IMPCAL, 1, IMPCAL_IMPCAL_HW);
#endif

    currFreqSel= p->freq_sel;
}

static unsigned int su4DfsCnt=0;
static unsigned int su4DfsRWCnt=0;
void DFSTestProgram(DRAMC_CTX_T *p, int iDoDMA)
{
    DramcDFSTestCode(p);
    su4DfsRWCnt++;
    ///TODO: DVFS scenario
    if(u1IsLP4Family(p->dram_type))
    {
        switch(su4DfsCnt%4)
        {
            //3200->2400->1600->2400->3200
            case 0:
                DDRPhyFreqSel(p, LP4_DDR3200);
                break;
            case 1:
                DDRPhyFreqSel(p, LP4_DDR2400);
                break;
            case 2:
                DDRPhyFreqSel(p, LP4_DDR1600);
                break;
            case 3:
                DDRPhyFreqSel(p, LP4_DDR2400);
                break;
            default:
                break;
        }
    }    
    else
    {
        switch(su4DfsCnt%4)    
        {   case 0:
                DDRPhyFreqSel(p, LP3_DDR1866);
                break;
            case 1:
                DDRPhyFreqSel(p, LP3_DDR1600);
                break;
            case 2:
                DDRPhyFreqSel(p, LP3_DDR1200);
                break;
            case 3:
                DDRPhyFreqSel(p, LP3_DDR1600);
                break;                
        }
    }

    if(su4DfsCnt%10==0)
    {
        DramcDFSDirectJump_SwitchCtrlMode(p);
    }

#ifndef MT6763_FPGA
    if(p->freq_sel==LP4_DDR3200 || p->freq_sel==LP4_DDR2667 || p->freq_sel==LP4_DDR1600)
    {
        //buck_set_voltage(VCORE, 900000);
    }
    DramcDFS(p, iDoDMA);
    if(p->freq_sel==LP4_DDR800)
    {
        //buck_set_voltage(VCORE, 800000);
    }
#endif

#if __ETT__
    GetPhyPllFrequency(p);
#endif

    su4DfsCnt++;
}
#endif //DUAL_FREQ_K

#endif //__ETT__ || defined(SLT)

void DramcSaveToShuffleReg(DRAMC_CTX_T *p, DRAM_DFS_SHUFFLE_TYPE_T srcRG, DRAM_DFS_SHUFFLE_TYPE_T dstRG)
{
    U32 Offset;
    U32 u4RegValue;
    U32 i;
    U32 tbIdx;

    mcSHOW_DBG_MSG(("Save freq reg settings into shuffle regs\n"));

    //DRAMC
    for(i=0; i<CHANNEL_NUM; i++)
    {
        for(tbIdx=0; tbIdx<SHUFFLE_ADDR_NUM_DRAMC; tbIdx++)
        {
            U32 normalAddr=ShuffleRegTableDramc[tbIdx].u4StartAddr+((U32)i << POS_BANK_NUM);
            while(normalAddr<=ShuffleRegTableDramc[tbIdx].u4EndAddr+((U32)i << POS_BANK_NUM))
            {
                U32 srcAddr = normalAddr + SHUFFLE_GROPU_OFFSET_DRAMC*srcRG;
                U32 shuffleAddr = normalAddr + SHUFFLE_GROPU_OFFSET_DRAMC*dstRG;
                U32 regValue = u4IO32Read4B(srcAddr);
                vIO32Write4B(shuffleAddr, regValue);
                //mcSHOW_DBG_MSG(("0x%X -> 0x%X\n", normalAddr, shuffleAddr));
                normalAddr+=4;
            }
        }
    }

    //DRAMC-exception-1
	for(i=0; i<CHANNEL_NUM; i++)
	{	
        U32 srcAddr = DRAMC_REG_SHUCTRL2+((U32)i << POS_BANK_NUM);
        U32 shuffleAddr = DRAMC_REG_DVFSDLL+((U32)i << POS_BANK_NUM);
        U32 regValue = u4IO32ReadFldAlign(srcAddr, SHUCTRL2_R_DLL_IDLE);

        switch (dstRG)
        {
            case DRAM_DFS_SHUFFLE_2:
                vIO32WriteFldAlign(shuffleAddr, regValue, DVFSDLL_DLL_IDLE_SHU2);
                break;
            case DRAM_DFS_SHUFFLE_3:
                vIO32WriteFldAlign(shuffleAddr, regValue, DVFSDLL_DLL_IDLE_SHU3);
                break;
            case DRAM_DFS_SHUFFLE_4:
                vIO32WriteFldAlign(shuffleAddr, regValue, DVFSDLL_DLL_IDLE_SHU4);
                break;
            default:
                vIO32WriteFldAlign(srcAddr, regValue, SHUCTRL2_R_DLL_IDLE);
        }
        //mcSHOW_DBG_MSG(("0x%X -> 0x%X\n", normalAddr, shuffleAddr));
    }

    //DRAMC-exception-2
    for(i=0; i<CHANNEL_NUM; i++)
    {	
        U32 srcAddr = DRAMC_REG_DVFSDLL+((U32)i << POS_BANK_NUM);
        U32 regValue = u4IO32ReadFldAlign(srcAddr, DVFSDLL_R_BYPASS_1ST_DLL_SHU1);
        switch (dstRG)
        {
            case DRAM_DFS_SHUFFLE_2:
                vIO32WriteFldAlign(srcAddr, regValue, DVFSDLL_R_BYPASS_1ST_DLL_SHU2);
                break;
            case DRAM_DFS_SHUFFLE_3:
                vIO32WriteFldAlign(srcAddr, regValue, DVFSDLL_R_BYPASS_1ST_DLL_SHU3);
                break;
            case DRAM_DFS_SHUFFLE_4:
                vIO32WriteFldAlign(srcAddr, regValue, DVFSDLL_R_BYPASS_1ST_DLL_SHU4);
                break;
            default:
                vIO32WriteFldAlign(srcAddr, regValue, DVFSDLL_R_BYPASS_1ST_DLL_SHU1);
        }
    }

    //PHY
    for(i=0; i<CHANNEL_NUM; i++)
    {
        for(tbIdx=0; tbIdx<SHUFFLE_ADDR_NUM_DDRPHY; tbIdx++)
        {
            U32 normalAddr=ShuffleRegTableDDRPhy[tbIdx].u4StartAddr+((U32)i << POS_BANK_NUM);
            while(normalAddr<=ShuffleRegTableDDRPhy[tbIdx].u4EndAddr+((U32)i << POS_BANK_NUM))
            {
                U32 srcAddr = normalAddr + SHUFFLE_GROPU_OFFSET_DDRPHY*srcRG;
                U32 shuffleAddr = normalAddr + SHUFFLE_GROPU_OFFSET_DDRPHY*dstRG;
                U32 regValue = u4IO32Read4B(srcAddr);
                vIO32Write4B(shuffleAddr, regValue);
                //mcSHOW_DBG_MSG(("0x%X -> 0x%X\n", normalAddr, shuffleAddr));
                normalAddr+=4;
            }
        }
    }

    //PHY-exception
#ifndef BIANCO_TO_BE_PORTING
    for(i=0; i<CHANNEL_NUM; i++)
    {
        U32 normalAddr=DDRPHY_SSC_SHU1_PLL5+((U32)i << POS_BANK_NUM);
        while(normalAddr<=DDRPHY_SSC_SHU1_PLL8+((U32)i << POS_BANK_NUM))
        {
            U32 srcOffset = (srcRG==0)? 0 : 0x20;
            U32 dstOffset = (dstRG==0)? 0 : 0x20;
            U32 srcAddr = normalAddr + 0x10*srcRG + srcOffset;
            U32 shuffleAddr = normalAddr + 0x10*dstRG + dstOffset;
            U32 regValue = u4IO32Read4B(srcAddr);
            vIO32Write4B(shuffleAddr, regValue);
            //mcSHOW_DBG_MSG(("0x%X -> 0x%X\n", normalAddr, shuffleAddr));
            normalAddr+=4;
        }
    }
#endif

    //save shuffle freq
    vSetShuffleFrequency(p, dstRG, p->shuffle_frequency[srcRG]);
}

void SPM_Pinmux_Setting(void)
{
 #if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
    *((volatile unsigned int *)(SPM_POWERON_CONFIG_EN)) = 0xB16 << 16 | 0x1;
    
     //! set SPM pinmux
     *((volatile unsigned int *)(SPM_PCM_PWR_IO_EN))     = 0;
    
     *((volatile unsigned int *)(SPM_DRAMC_DPY_CLK_SW_CON_SEL))  = 0xffffffff;
     *((volatile unsigned int *)(SPM_DRAMC_DPY_CLK_SW_CON_SEL2)) = 0xffffffff;
 #endif
}


#if __ETT__ || defined(SLT)
U8 gDVFSCtrlSel=0;
void DramcDFSDirectJump_SwitchCtrlMode(DRAMC_CTX_T *p)
{
    gDVFSCtrlSel = !gDVFSCtrlSel;
    if(gDVFSCtrlSel==0)//SPM
    {
        if(!gu1PhyPLLEn)
        {
            mcSHOW_DBG_MSG(("DramcDFSDirectJump_SwitchCtrlMode(%d) current=CLRPLL\n", gDVFSCtrlSel));
            *((volatile unsigned int *)(SPM_POWER_ON_VAL0)) &= ~(0x1<<28);
            *((volatile unsigned int *)(SPM_POWER_ON_VAL0)) |= (0x1<<29);
            *((volatile unsigned int *)(SPM_DRAMC_DPY_CLK_SW_CON2)) &= ~(0x1<<2);
            *((volatile unsigned int *)(SPM_DRAMC_DPY_CLK_SW_CON2)) |= (0x1<<3);
        }
        else
        {
            mcSHOW_DBG_MSG(("DramcDFSDirectJump_SwitchCtrlMode(%d) current=PHYPLL\n", gDVFSCtrlSel));
            *((volatile unsigned int *)(SPM_POWER_ON_VAL0)) &= ~(0x1<<29);
            *((volatile unsigned int *)(SPM_POWER_ON_VAL0)) |= (0x1<<28);
            *((volatile unsigned int *)(SPM_DRAMC_DPY_CLK_SW_CON2)) &= ~(0x1<<3);
            *((volatile unsigned int *)(SPM_DRAMC_DPY_CLK_SW_CON2)) |= (0x1<<2);
        }
    }
    else//SPM-RG Mode
    {
        if(!gu1PhyPLLEn)
        {
            mcSHOW_DBG_MSG(("DramcDFSDirectJump_SwitchCtrlMode(%d) current=CLRPLL\n", gDVFSCtrlSel));
            vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_PHYPLL_SHU_EN);
            vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_PHYPLL_MODE_SW);
            vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_PHYPLL2_SHU_EN);
            vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_PHYPLL2_MODE_SW);
        }
        else
        {
            mcSHOW_DBG_MSG(("DramcDFSDirectJump_SwitchCtrlMode(%d) current=PHYPLL\n", gDVFSCtrlSel));
            vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_PHYPLL_SHU_EN);
            vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_PHYPLL_MODE_SW);
            vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_PHYPLL2_SHU_EN);
            vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_PHYPLL2_MODE_SW);
        }
    }
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, gDVFSCtrlSel, MISC_SPM_CTRL1_SPM_DVFS_CONTROL_SEL);
}

void DramcDFSDirectJump_SPMMode(DRAMC_CTX_T *p, U8 shu_level)
{
    U8 u1ShuAck = 0;
    U8 i = 0;
    for(i=0;i<p->support_channel_num; i++)
    {
        u1ShuAck |= (0x1<<i);
    }

    if(gu1PhyPLLEn)
    {
        mcSHOW_DBG_MSG3(("DFSDirectJump to CLRPLL, SHU_LEVEL=%d, ACK=%x\n", shu_level, u1ShuAck));
    }
    else
    {
        mcSHOW_DBG_MSG3(("DFSDirectJump to PHYPLL, SHU_LEVEL=%d, ACK=%x\n", shu_level, u1ShuAck));
    }


    vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_PHYPLL_SHU_EN_PCM);
    vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_PHYPLL2_SHU_EN_PCM);

    vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DR_SHU_LEVEL_PCM);
    vIO32WriteFldAlign(SPM_POWER_ON_VAL0, shu_level, SPM_POWER_ON_VAL0_SC_DR_SHU_LEVEL_PCM);

    if(gu1PhyPLLEn)
    {
        vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL2_SHU_EN_PCM);
        mcDELAY_US(1);
        vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL2_MODE_SW_PCM);
        mcSHOW_DBG_MSG3(("Enable CLRPLL\n"));
    }
    else
    {
        vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL_SHU_EN_PCM);
        mcDELAY_US(1);
        vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL_MODE_SW_PCM);
        mcSHOW_DBG_MSG3(("Enable PHYPLL\n"));
    }
    mcDELAY_US(20);

    mcSHOW_DBG_MSG3(("Enable SHORT-QUEUE\n"));
    vIO32WriteFldAlign(SPM_POWER_ON_VAL1, 1, SPM_POWER_ON_VAL1_SC_DR_SHORT_QUEUE_PCM);
    while ((u4IO32ReadFldAlign(SPM_DRAMC_DPY_CLK_SW_CON, DRAMC_DPY_CLK_SW_CON_SC_DR_SHORT_QUEUE_ACK) & u1ShuAck ) != u1ShuAck)
    {
        mcSHOW_DBG_MSG3(("\twait short queue ack.\n"));
    }


    /*TINFO="DRAM : set ddrphy_fb_ck_en=1"*/
    vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_DDRPHY_FB_CK_EN_PCM);


    mcSHOW_DBG_MSG3(("Disable RX-Tracking\n"));
    vIO32WriteFldAlign(SPM_SW_RSV_8, 0, SW_RSV_8_RX_TRACKING_EN);


    mcSHOW_DBG_MSG3(("SHUFFLE Start\n"));
    vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1,  SPM_POWER_ON_VAL0_SC_DR_SHU_EN_PCM);
    while ((u4IO32ReadFldAlign(SPM_DRAMC_DPY_CLK_SW_CON, DRAMC_DPY_CLK_SW_CON_SC_DMDRAMCSHU_ACK) & u1ShuAck ) != u1ShuAck)
    {
        mcSHOW_DBG_MSG3(("\twait shu_en ack.\n"));
    }

    vIO32WriteFldAlign(SPM_POWER_ON_VAL1, 0, SPM_POWER_ON_VAL1_SC_DR_SHORT_QUEUE_PCM);
    vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0,  SPM_POWER_ON_VAL0_SC_DR_SHU_EN_PCM);
    mcSHOW_DBG_MSG3(("SHUFFLE End\n"));

    if(shu_level == 0)//LP4-2CH
    {
        mcSHOW_DBG_MSG3(("Enable RX-Tracking for shuffle-0\n"));
        vIO32WriteFldAlign(SPM_SW_RSV_8, 3, SW_RSV_8_RX_TRACKING_EN);
    }

    /*TINFO="DRAM : set ddrphy_fb_ck_en=0"*/
    vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DDRPHY_FB_CK_EN_PCM);

    if(gu1PhyPLLEn)
    {
        vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_PHYPLL_MODE_SW_PCM);
    }
    else
    {
        /*TINFO="DRAM : set sc_phypll_mode_sw=0"*/
        vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_PHYPLL2_MODE_SW_PCM);
    }

    mcSHOW_DBG_MSG3(("Shuffle flow complete\n"));

    gu1PhyPLLEn = !gu1PhyPLLEn;
    return;
}

void DramcDFSDirectJump_RGMode(DRAMC_CTX_T *p, U8 shu_level)
{
    U8 u1ShuAck = 0;
    U8 i = 0;

    for(i=0;i<p->support_channel_num; i++)
    {
        u1ShuAck |= (0x1<<i);
    }

    if(gu1PhyPLLEn)
    {
        mcSHOW_DBG_MSG3(("DFSDirectJump to CLRPLL, SHU_LEVEL=%d, ACK=%x\n", shu_level, u1ShuAck));
    }
    else
    {
        mcSHOW_DBG_MSG3(("DFSDirectJump to PHYPLL, SHU_LEVEL=%d, ACK=%x\n", shu_level, u1ShuAck));
    }


    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_PHYPLL_SHU_EN);
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_PHYPLL2_SHU_EN);

    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_DR_SHU_LEVEL);
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, shu_level, MISC_SPM_CTRL1_RG_DR_SHU_LEVEL);

    if(gu1PhyPLLEn)
    {
        vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_PHYPLL2_SHU_EN);
        mcDELAY_US(1);
        vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_PHYPLL2_MODE_SW);
        mcSHOW_DBG_MSG3(("Enable CLRPLL\n"));
    }
    else
    {
        vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_PHYPLL_SHU_EN);
        mcDELAY_US(1);
        vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_PHYPLL_MODE_SW);
        mcSHOW_DBG_MSG3(("Enable PHYPLL\n"));
    }
    mcDELAY_US(20);

    mcSHOW_DBG_MSG3(("Enable SHORT-QUEUE\n"));
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_DR_SHORT_QUEUE);

    mcSHOW_DBG_MSG3(("\twait 5us for short queue ack.\n"));
    mcDELAY_US(5);

    /*TINFO="DRAM : set ddrphy_fb_ck_en=1"*/
    vIO32WriteFldMulti(DDRPHY_MISC_SPM_CTRL1, P_Fld(1, MISC_SPM_CTRL1_RG_DDRPHY_FB_CK_CH1_EN) | P_Fld(1, MISC_SPM_CTRL1_RG_DDRPHY_FB_CK_CH0_EN)); 

    //mcSHOW_DBG_MSG(("Disable RX-Tracking\n"));
    //vIO32WriteFldAlign(SPM_SW_RSV_8, 0, SW_RSV_8_RX_TRACKING_EN);


    mcSHOW_DBG_MSG3(("SHUFFLE Start\n"));
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1,  MISC_SPM_CTRL1_RG_DR_SHU_EN);

    mcSHOW_DBG_MSG3(("\twait 5us for shu_en ack.\n"));
    mcDELAY_US(5);

    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_DR_SHORT_QUEUE);
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_DR_SHU_EN);
    mcSHOW_DBG_MSG3(("SHUFFLE End\n"));

    if(shu_level == 0)//LP4-2CH
    {
        //mcSHOW_DBG_MSG(("Enable RX-Tracking for shuffle-0\n"));
        //vIO32WriteFldAlign(SPM_SW_RSV_8, 3, SW_RSV_8_RX_TRACKING_EN);
    }

    /*TINFO="DRAM : set ddrphy_fb_ck_en=0"*/
    vIO32WriteFldMulti(DDRPHY_MISC_SPM_CTRL1, P_Fld(0, MISC_SPM_CTRL1_RG_DDRPHY_FB_CK_CH1_EN) | P_Fld(0, MISC_SPM_CTRL1_RG_DDRPHY_FB_CK_CH0_EN)); 

    if(gu1PhyPLLEn)
    {
        vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_PHYPLL_MODE_SW);
    }
    else
    {
        /*TINFO="DRAM : set sc_phypll_mode_sw=0"*/
        vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_PHYPLL2_MODE_SW);
    }

    mcSHOW_DBG_MSG3(("Shuffle flow complete\n"));

    gu1PhyPLLEn = !gu1PhyPLLEn;
    return;
}

void DramcDFSDirectJump(DRAMC_CTX_T *p, U8 shu_level)
{
    if(gDVFSCtrlSel==0)
    {
        DramcDFSDirectJump_SPMMode(p, shu_level);
    }
    else
    {
        DramcDFSDirectJump_RGMode(p, shu_level);
    }
}

#endif

void CBT_DramcDFSDirectJump(DRAMC_CTX_T *p, U8 shu_level)
{
    U8 u1ShuAck = 0;
    U8 i = 0;
    //enable another PLL
    if(gu1PhyPLLEn)
    {
        mcSHOW_DBG_MSG3(("Disable CLRPLL\n"));
        //vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, 1, DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL2_MODE_SW);
        vIO32WriteFldAlign(DDRPHY_PLL2, 0, PLL2_RG_RCLRPLL_EN);
    }
    else
    {
        mcSHOW_DBG_MSG3(("Disable PHYPLL\n"));
        //vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, 1, DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL_MODE_SW);
        vIO32WriteFldAlign(DDRPHY_PLL1, 0, PLL1_RG_RPHYPLL_EN);        
    }

    for(i=0;i<p->support_channel_num; i++)
    {
        u1ShuAck |= (0x1<<i);
    }

    if(gu1PhyPLLEn)
    {
        mcSHOW_DBG_MSG3(("DFSDirectJump to CLRPLL, SHU_LEVEL=%d, ACK=%x\n", shu_level, u1ShuAck));
    }
    else
    {
        mcSHOW_DBG_MSG3(("DFSDirectJump to PHYPLL, SHU_LEVEL=%d, ACK=%x\n", shu_level, u1ShuAck));
    }

    vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_PHYPLL_SHU_EN_PCM);
    vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_PHYPLL2_SHU_EN_PCM);

    vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DR_SHU_LEVEL_PCM);
    vIO32WriteFldAlign(SPM_POWER_ON_VAL0, shu_level, SPM_POWER_ON_VAL0_SC_DR_SHU_LEVEL_PCM);

    if(gu1PhyPLLEn)
    {
        vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL2_SHU_EN_PCM);
        mcDELAY_US(1);
        //vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL2_MODE_SW_PCM);
        vIO32WriteFldAlign(DDRPHY_PLL2, 1, PLL2_RG_RCLRPLL_EN);
        mcSHOW_DBG_MSG3(("Enable CLRPLL\n"));
    }
    else
    {
        vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL_SHU_EN_PCM);
        mcDELAY_US(1);
        //vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL_MODE_SW_PCM);
        vIO32WriteFldAlign(DDRPHY_PLL1, 1, PLL1_RG_RPHYPLL_EN);        
        mcSHOW_DBG_MSG3(("Enable PHYPLL\n"));
    }
    mcDELAY_US(20);

    mcSHOW_DBG_MSG3(("Enable SHORT-QUEUE\n"));
    vIO32WriteFldAlign(SPM_POWER_ON_VAL1, 1, SPM_POWER_ON_VAL1_SC_DR_SHORT_QUEUE_PCM);
    while ((u4IO32ReadFldAlign(SPM_DRAMC_DPY_CLK_SW_CON, DRAMC_DPY_CLK_SW_CON_SC_DR_SHORT_QUEUE_ACK) & u1ShuAck ) != u1ShuAck)
    {
        mcSHOW_DBG_MSG3(("\twait short queue ack.\n"));
    }


    /*TINFO="DRAM : set ddrphy_fb_ck_en=1"*/
    vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_DDRPHY_FB_CK_EN_PCM);


    mcSHOW_DBG_MSG3(("Disable RX-Tracking\n"));
    vIO32WriteFldAlign(SPM_SW_RSV_8, 0, SW_RSV_8_RX_TRACKING_EN);


    mcSHOW_DBG_MSG3(("SHUFFLE Start\n"));
    vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1,  SPM_POWER_ON_VAL0_SC_DR_SHU_EN_PCM);
    while ((u4IO32ReadFldAlign(SPM_DRAMC_DPY_CLK_SW_CON, DRAMC_DPY_CLK_SW_CON_SC_DMDRAMCSHU_ACK) & u1ShuAck ) != u1ShuAck)
    {
        mcSHOW_DBG_MSG3(("\twait shu_en ack.\n"));
    }

    vIO32WriteFldAlign(SPM_POWER_ON_VAL1, 0, SPM_POWER_ON_VAL1_SC_DR_SHORT_QUEUE_PCM);
    vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0,  SPM_POWER_ON_VAL0_SC_DR_SHU_EN_PCM);
    mcSHOW_DBG_MSG3(("SHUFFLE End\n"));

    if(shu_level == 0)//LP4-2CH
    {
        mcSHOW_DBG_MSG3(("Enable RX-Tracking for shuffle-0\n"));
        vIO32WriteFldAlign(SPM_SW_RSV_8, 3, SW_RSV_8_RX_TRACKING_EN);
    }

    /*TINFO="DRAM : set ddrphy_fb_ck_en=0"*/
    vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DDRPHY_FB_CK_EN_PCM);

    if(gu1PhyPLLEn)
    {
        //vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_PHYPLL_MODE_SW_PCM);
        vIO32WriteFldAlign(DDRPHY_PLL1, 0, PLL1_RG_RPHYPLL_EN);
    }
    else
    {
        /*TINFO="DRAM : set sc_phypll_mode_sw=0"*/
        //vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_PHYPLL2_MODE_SW_PCM);
        vIO32WriteFldAlign(DDRPHY_PLL2, 0, PLL2_RG_RCLRPLL_EN);        
    }

    mcSHOW_DBG_MSG3(("Shuffle flow complete\n"));

    gu1PhyPLLEn = !gu1PhyPLLEn;
    return;
}

void No_Parking_On_CLRPLL(DRAMC_CTX_T *p)
{
    U8 shu_level;

    if (gu1PhyPLLEn) return; /* already parking on PHYPLL */

    shu_level = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHUSTATUS), SHUSTATUS_SHUFFLE_LEVEL);

    CBT_DramcDFSDirectJump(p, shu_level); /* parking on PHYPLL */
}

#endif  // DUAL_FREQ_K


U16 u2DFSGetHighestFreq(DRAMC_CTX_T * p)
{
    if(u1IsLP4Family(p->dram_type))
    {
        return 1600;
    }
#if ENABLE_LP3_SW
    else
    {
        return 933;
    }
#endif /* ENABLE_LP3_SW */
}


// both "ENABLE_RANK_NUMBER_AUTO_DETECTION" and "DUAL_FREQ_K" use this API
void DFSInitForCalibration(DRAMC_CTX_T *p)
{
    u1PrintModeRegWrite=1;
    DramcInit(p);
    u1PrintModeRegWrite=0;
    vApplyConfigBeforeCalibration(p);
}


void DramcHWDQSGatingTracking_DVT(DRAMC_CTX_T *p)
{
#ifdef HW_GATING
if(u1IsLP4Family(p->dram_type))
{
#if DramcHWDQSGatingTracking_DVT_FIFO_MODE
    //REFUICHG=0, STB_SHIFT_DTCOUT_IG=0, DQSG_MODE=1, NARROW_IG=0
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), P_Fld(1, STBCAL_STB_DQIEN_IG) | P_Fld(1, STBCAL_PICHGBLOCK_NORD) | \
                                            P_Fld(0, STBCAL_REFUICHG) | P_Fld(0, STBCAL_PHYVALID_IG) | \
                                            P_Fld(0, STBCAL_STBSTATE_OPT)| P_Fld(0, STBCAL_STBDLELAST_FILTER) | \
                                            P_Fld(0, STBCAL_STBDLELAST_PULSE) | \
                                            P_Fld(0, STBCAL_STBDLELAST_OPT)|  \
                                            P_Fld(1, STBCAL_PIMASK_RKCHG_OPT));

    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL1), \
                                            P_Fld(1, STBCAL1_STBCAL_FILTER)| \
                                            P_Fld(1, STBCAL1_STB_FLAGCLR) | \
                                            P_Fld(0, STBCAL1_STB_SHIFT_DTCOUT_IG));

    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL0), \
                                            P_Fld(1, MISC_CTRL0_R_DMDQSIEN_FIFO_EN) | \
                                            P_Fld(0, MISC_CTRL0_R_DMVALID_DLY)|  \
                                            P_Fld(0, MISC_CTRL0_R_DMVALID_DLY_OPT) | \
                                            P_Fld(0, MISC_CTRL0_R_DMVALID_NARROW_IG) | \
                                            P_Fld(0, MISC_CTRL0_R_DMDQSIEN_SYNCOPT));

    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), 0, B0_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), 0, B1_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B1);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_CA_CMD6), 0, CA_CMD6_RG_RX_ARCMD_DMRANK_OUTSEL);

    #else
    //REFUICHG=0, STB_SHIFT_DTCOUT_IG=0, DQSG_MODE=1, NARROW_IG=0
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), P_Fld(1, STBCAL_STB_DQIEN_IG) | P_Fld(1, STBCAL_PICHGBLOCK_NORD) | \
                                            P_Fld(0, STBCAL_REFUICHG) | P_Fld(0, STBCAL_PHYVALID_IG) | \
                                            P_Fld(0, STBCAL_STBSTATE_OPT)| P_Fld(0, STBCAL_STBDLELAST_FILTER) | \
                                            P_Fld(0, STBCAL_STBDLELAST_PULSE) | \
                                            P_Fld(0, STBCAL_STBDLELAST_OPT)|  \
                                            P_Fld(1, STBCAL_PIMASK_RKCHG_OPT));

    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL1), \
                                            P_Fld(1, STBCAL1_STBCAL_FILTER)| \
                                            P_Fld(1, STBCAL1_STB_FLAGCLR) | \
                                            P_Fld(0, STBCAL1_STB_SHIFT_DTCOUT_IG));


    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL0), \
                                            P_Fld(0, MISC_CTRL0_R_DMDQSIEN_FIFO_EN) | \
                                            P_Fld(3, MISC_CTRL0_R_DMVALID_DLY)|  \
                                            P_Fld(1, MISC_CTRL0_R_DMVALID_DLY_OPT) | \
                                            P_Fld(0, MISC_CTRL0_R_DMVALID_NARROW_IG) | \
                                            P_Fld(0xf, MISC_CTRL0_R_DMDQSIEN_SYNCOPT));

    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), 1, B0_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), 1, B1_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B1);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_CA_CMD6), 1, CA_CMD6_RG_RX_ARCMD_DMRANK_OUTSEL);
    #endif

}
#if ENABLE_LP3_SW
else
{
#if DramcHWDQSGatingTracking_DVT_JADE_TRACKING_MODE
        //UPDBYWR=0, REFUICHG=1, STB_SHIFT_DTCOUT_IG=0, DQSG_MODE=1, NARROW_IG=1
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0), 0, REFCTRL0_UPDBYWR);
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), P_Fld(0, STBCAL_STB_DQIEN_IG) | P_Fld(0, STBCAL_PICHGBLOCK_NORD) | \
                                                P_Fld(1, STBCAL_REFUICHG) | P_Fld(0, STBCAL_PHYVALID_IG) | \
                                                P_Fld(1, STBCAL_STBSTATE_OPT)| P_Fld(0, STBCAL_STBDLELAST_FILTER) | \
                                                P_Fld(0, STBCAL_STBDLELAST_PULSE) | \
                                                P_Fld(0, STBCAL_STBDLELAST_OPT)|  \
                                                P_Fld(0, STBCAL_PIMASK_RKCHG_OPT));

        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL1), \
                                                P_Fld(0, STBCAL1_STBCAL_FILTER)| \
                                                P_Fld(1, STBCAL1_STB_FLAGCLR) | \
                                                P_Fld(0, STBCAL1_STB_SHIFT_DTCOUT_IG));

        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL0), \
                                                P_Fld(0, MISC_CTRL0_R_DMDQSIEN_FIFO_EN) | \
                                                P_Fld(2, MISC_CTRL0_R_DMVALID_DLY)|  \
                                                P_Fld(1, MISC_CTRL0_R_DMVALID_DLY_OPT) | \
                                                P_Fld(1, MISC_CTRL0_R_DMVALID_NARROW_IG) | \
                                                P_Fld(0xf, MISC_CTRL0_R_DMDQSIEN_SYNCOPT));

#else
        //REFUICHG=0, STB_SHIFT_DTCOUT_IG=0, DQSG_MODE=1, NARROW_IG=0
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), P_Fld(1, STBCAL_STB_DQIEN_IG) | P_Fld(1, STBCAL_PICHGBLOCK_NORD) | \
                                                P_Fld(0, STBCAL_REFUICHG) | P_Fld(0, STBCAL_PHYVALID_IG) | \
                                                P_Fld(0, STBCAL_STBSTATE_OPT)| P_Fld(0, STBCAL_STBDLELAST_FILTER) | \
                                                P_Fld(0, STBCAL_STBDLELAST_PULSE) | \
                                                P_Fld(0, STBCAL_STBDLELAST_OPT)|  \
                                                P_Fld(1, STBCAL_PIMASK_RKCHG_OPT));

        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL1), \
                                                P_Fld(1, STBCAL1_STBCAL_FILTER)| \
                                                P_Fld(1, STBCAL1_STB_FLAGCLR) | \
                                                P_Fld(0, STBCAL1_STB_SHIFT_DTCOUT_IG));

        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL0), \
                                                P_Fld(0, MISC_CTRL0_R_DMDQSIEN_FIFO_EN) | \
                                                P_Fld(5, MISC_CTRL0_R_DMVALID_DLY)|  \
                                                P_Fld(1, MISC_CTRL0_R_DMVALID_DLY_OPT) | \
                                                P_Fld(0, MISC_CTRL0_R_DMVALID_NARROW_IG) | \
                                                P_Fld(0xf, MISC_CTRL0_R_DMDQSIEN_SYNCOPT));

#endif

    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), 1, B0_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), 1, B1_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B1);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_CA_CMD6), 1, CA_CMD6_RG_RX_ARCMD_DMRANK_OUTSEL);


}
#endif /* ENABLE_LP3_SW */

#endif
}


void DramcHWGatingInit(DRAMC_CTX_T *p)
{
#ifdef HW_GATING
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), P_Fld(0, STBCAL_STBCALEN) | \
                                            P_Fld(0, STBCAL_STBCAL2R)| P_Fld(0, STBCAL_STB_SELPHYCALEN) | \
                                            P_Fld(0, STBCAL_STBSTATE_OPT) | \
                                            P_Fld(0, STBCAL_RKCHGMASKDIS)| P_Fld(0, STBCAL_REFUICHG) | \
                                            P_Fld(1, STBCAL_PICGEN));

    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), P_Fld(1, STBCAL_DQSIENCG_CHG_EN));
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), 0, STBCAL_CG_RKEN);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), 1, STBCAL_DQSIENCG_NORMAL_EN);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 1, MISC_CTRL1_R_DMDQSIENCG_EN);

    DramcHWDQSGatingTracking_DVT(p);
#endif
#if 0 // Moved into DramcGatingMode()
#ifdef GATING_7UI_FOR_TRACKING
	vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL1), 0x1, STBCAL1_DQSIEN_7UI_EN);		
	vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), 0x2, B0_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B0);		
	vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), 0x2, B1_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B1);

	mcSHOW_DBG_MSG(("GATING_7UI_FOR_TRACKING: ON\n"));
#else
	mcSHOW_DBG_MSG(("GATING_7UI_FOR_TRACKING: OFF\n"));
#endif
#endif
}


void DramcHWGatingOnOff(DRAMC_CTX_T *p, U8 u1OnOff)
{
    #ifdef HW_GATING
    vIO32WriteFldMulti_All(DRAMC_REG_SHUCTRL2, P_Fld(u1OnOff, SHUCTRL2_R_DVFS_OPTION)
                | P_Fld(u1OnOff, SHUCTRL2_R_DVFS_PARK_N));
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL2), u1OnOff, STBCAL2_STB_GERRSTOP);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), u1OnOff, STBCAL_STBCALEN);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), u1OnOff, STBCAL_STB_SELPHYCALEN);
    #else
    vIO32WriteFldMulti_All(DRAMC_REG_SHUCTRL2, P_Fld(0x0, SHUCTRL2_R_DVFS_OPTION)
                    | P_Fld(0x0, SHUCTRL2_R_DVFS_PARK_N));
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL2), 0, STBCAL2_STB_GERRSTOP);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), 0, STBCAL_STBCALEN);   // PI tracking off = HW gating tracking off
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), 0, STBCAL_STB_SELPHYCALEN);
    #endif
}

void DramcHWGatingDebugOnOff(DRAMC_CTX_T *p, U8 u1OnOff)
{
    #ifdef HW_GATING
    vIO32WriteFldMulti_All(DRAMC_REG_STBCAL2, P_Fld(u1OnOff, STBCAL2_STB_DBG_EN_B1) | 
                                              P_Fld(u1OnOff, STBCAL2_STB_DBG_EN_B0) | 
                                              P_Fld(0, STBCAL2_STB_DBG_CG_AO) |
                                              P_Fld(0, STBCAL2_STB_DBG_UIPI_UPD_OPT));
    #endif
}

#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
#if (__ETT__ || CPU_RW_TEST_AFTER_K)
#if 0 // Please use memeset to initail value, due to different CHANNEL_NUM 
U16 u2MaxGatingPos[CHANNEL_NUM][RANK_MAX][DQS_NUMBER]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
U16 u2MinGatingPos[CHANNEL_NUM][RANK_MAX][DQS_NUMBER]={0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
#endif
void DramcPrintHWGatingStatus(DRAMC_CTX_T *p, U8 u1Channel)
{
#ifdef HW_GATING
    U8 u1RankIdx, u1RankMax, u1ChannelBak, u1ByteIdx, ii;
    U32 u4ResultDQS_PI, u4ResultDQS_UI, u4ResultDQS_UI_P1;
    U8 u1Dqs_pi[DQS_BIT_NUMBER], u1Dqs_ui[DQS_BIT_NUMBER],u1Dqs_ui_P1[DQS_BIT_NUMBER];
    U16 u2TmpValue, u2TmpUI[DQS_NUMBER], u2TmpPI[DQS_NUMBER];
    U32 MANUDLLFRZ_bak, STBSTATE_OPT_bak;
    U32 backup_rank;

    u1ChannelBak= p->channel;
    vSetPHY2ChannelMapping(p, u1Channel);
    backup_rank = u1GetRank(p);

    if (p->support_rank_num==RANK_DUAL)
        u1RankMax = RANK_MAX;
    else
        u1RankMax =RANK_1;

    MANUDLLFRZ_bak = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), TEST2_3_MANUDLLFRZ);
    STBSTATE_OPT_bak = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), STBCAL_STBSTATE_OPT);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), 1, TEST2_3_MANUDLLFRZ);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), 1, STBCAL_STBSTATE_OPT);

    for(u1RankIdx=0; u1RankIdx<u1RankMax; u1RankIdx++)
    {
        vSetRank(p, u1RankIdx);
        mcSHOW_DBG_MSG(("[DramcHWGatingStatus] Channel=%d, Rank=%d\n", p->channel, u1RankIdx));
        u4ResultDQS_PI = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_RK0_DQSIENDLY));
        u1Dqs_pi[0]  = u4ResultDQS_PI & 0xff;
        u1Dqs_pi[1]  = (u4ResultDQS_PI >>8) & 0xff;
        u1Dqs_pi[2]  = (u4ResultDQS_PI >>16) & 0xff;
        u1Dqs_pi[3]  = (u4ResultDQS_PI >>24) & 0xff;
        //mcSHOW_DBG_MSG(("[DramcHWGatingStatus] PI status (DQS0, DQS1, DQS2, DQS3) =(%d, %d, %d, %d)\n", u1Dqs_pi[0], u1Dqs_pi[1], u1Dqs_pi[2], u1Dqs_pi[3]));

        u4ResultDQS_UI = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_RK0_DQSIENUIDLY));
        u1Dqs_ui[0]  = u4ResultDQS_UI & 0xff;
        u1Dqs_ui[1]  = (u4ResultDQS_UI >>8) & 0xff;
        u1Dqs_ui[2]  = (u4ResultDQS_UI >>16) & 0xff;
        u1Dqs_ui[3]  = (u4ResultDQS_UI >>24) & 0xff;
        //mcSHOW_DBG_MSG(("[DramcHWGatingStatus] UI status (DQS0, DQS1, DQS2, DQS3) =(%d, %d, %d, %d)\n", u1Dqs_ui[0], u1Dqs_ui[1], u1Dqs_ui[2], u1Dqs_ui[3]));
        mcSHOW_DBG_MSG(("Byte0(2T, 0.5T, PI) =(%d, %d, %d)\n Byte1(2T, 0.5T, PI) =(%d, %d, %d)\n Byte2(2T, 0.5T, PI) =(%d, %d, %d)\n Byte3(2T, 0.5T, PI) =(%d, %d, %d)\n", \
                                        u1Dqs_ui[0]/8, u1Dqs_ui[0]%8, u1Dqs_pi[0], u1Dqs_ui[1]/8, u1Dqs_ui[1]%8, u1Dqs_pi[1], \
                                        u1Dqs_ui[2]/8, u1Dqs_ui[2]%8, u1Dqs_pi[2], u1Dqs_ui[3]/8, u1Dqs_ui[3]%8, u1Dqs_pi[3]));

        u4ResultDQS_UI_P1 = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_RK0_DQSIENUIDLY_P1));
        u1Dqs_ui_P1[0]  = u4ResultDQS_UI_P1 & 0xff;
        u1Dqs_ui_P1[1]  = (u4ResultDQS_UI_P1 >>8) & 0xff;
        u1Dqs_ui_P1[2]  = (u4ResultDQS_UI_P1 >>16) & 0xff;
        u1Dqs_ui_P1[3]  = (u4ResultDQS_UI_P1 >>24) & 0xff;
        mcSHOW_DBG_MSG(("UI_Phase1 (DQS0~3) =(%d, %d, %d, %d)\n\n", u1Dqs_ui_P1[0], u1Dqs_ui_P1[1], u1Dqs_ui_P1[2], u1Dqs_ui_P1[3]));
    }
    vSetRank(p, backup_rank);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), STBSTATE_OPT_bak, STBCAL_STBSTATE_OPT);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), MANUDLLFRZ_bak, TEST2_3_MANUDLLFRZ);

    p->channel = u1ChannelBak;
    vSetPHY2ChannelMapping(p, u1ChannelBak);
#endif
}

void DramcHWGatingTrackingRecord(DRAMC_CTX_T *p, U8 u1Channel)
{
#ifdef HW_GATING
    U8 u1RankIdx, u1RankMax, u1Info_NUM, u1Info_Max_MUM=16;
    U8 u1ChannelBak, u1RankBak;
    
    U8 u1DBG_Dqs0_DFS, u1DBG_Dqs0_Lead, u1DBG_Dqs0_Lag, u1DBG_Dqs0_UI, u1DBG_Dqs0_PI;
    U8 u1DBG_Dqs1_DFS, u1DBG_Dqs1_Lead, u1DBG_Dqs1_Lag, u1DBG_Dqs1_UI, u1DBG_Dqs1_PI;
    U16 u4DBG_Dqs0_Info, u4DBG_Dqs1_Info;
    U32 u4DBG_Dqs01_Info;

    U32 u4Dqs0_MAX_MIN_DLY, u4Dqs1_MAX_MIN_DLY;
    U16 u2Dqs0_UI_MAX_DLY, u2Dqs0_PI_MAX_DLY, u2Dqs0_UI_MIN_DLY, u2Dqs0_PI_MIN_DLY;
    U16 u2Dqs1_UI_MAX_DLY, u2Dqs1_PI_MAX_DLY, u2Dqs1_UI_MIN_DLY, u2Dqs1_PI_MIN_DLY;
    U8 u1ShuffleLevel;
    
    u1ChannelBak= p->channel;
    vSetPHY2ChannelMapping(p, u1Channel);
    u1RankBak = u1GetRank(p);

    if (p->support_rank_num==RANK_DUAL)
        u1RankMax = RANK_MAX;
    else
        u1RankMax =RANK_1;
    
    //Run Time HW Gating Debug Information
    //for(u1RankIdx=0; u1RankIdx<u1RankMax; u1RankIdx++)
    for(u1RankIdx=0; u1RankIdx<u1RankMax; u1RankIdx++)    
    {
        vSetRank(p, u1RankIdx);
        
        u1ShuffleLevel = u4IO32ReadFldAlign(DRAMC_REG_SHUSTATUS, SHUSTATUS_SHUFFLE_LEVEL);

        mcSHOW_DBG_MSG3(("\n[HWGatingTrackingRecord] Channel=%d, Rank=%d, SHU_LEVEL=%d\n", p->channel, u1RankIdx, u1ShuffleLevel));
      
        mcSHOW_DBG_MSG3(("Run Time HW Gating Debug Information :\n"));
        mcSHOW_DBG_MSG3(("        B0=(DFS,Lead,Lag,4T, UI, PI), B1=(DFS,Lead,Lag,4T, UI, PI)\n"));
        
        for(u1Info_NUM=0; u1Info_NUM < u1Info_Max_MUM; u1Info_NUM++)
        {
          //DFS_ST(Shuffle Level): bit[15:14]
          //Shift_R(Lead): bit[13]
          //Shift_L(Lag) : bit[12]
          //UI_DLY : bit[11:06]
          //PI_DLY : bit[05:00]
          u4DBG_Dqs01_Info= u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_RK0_B01_STB_DBG_INFO_00 + 0x4*u1Info_NUM));
          u4DBG_Dqs0_Info = (u4DBG_Dqs01_Info >>  0)& 0xffff;
          u4DBG_Dqs1_Info = (u4DBG_Dqs01_Info >> 16)& 0xffff;
          
          u1DBG_Dqs0_DFS  = (u4DBG_Dqs0_Info >> 14) & 0x03;
          u1DBG_Dqs0_Lead = (u4DBG_Dqs0_Info >> 13) & 0x01;
          u1DBG_Dqs0_Lag  = (u4DBG_Dqs0_Info >> 12) & 0x01;
          u1DBG_Dqs0_UI   = (u4DBG_Dqs0_Info >>  6) & 0x3f; 
          u1DBG_Dqs0_PI   = (u4DBG_Dqs0_Info >>  0) & 0x3f;

          u1DBG_Dqs1_DFS  = (u4DBG_Dqs1_Info >> 14) & 0x03;
          u1DBG_Dqs1_Lead = (u4DBG_Dqs1_Info >> 13) & 0x01;
          u1DBG_Dqs1_Lag  = (u4DBG_Dqs1_Info >> 12) & 0x01;
          u1DBG_Dqs1_UI   = (u4DBG_Dqs1_Info >>  6) & 0x3f; 
          u1DBG_Dqs1_PI   = (u4DBG_Dqs1_Info >>  0) & 0x3f;
          
          if (u1Info_NUM<10)
          {
            mcSHOW_DBG_MSG3(("Info= %d ", u1Info_NUM));
          }
          else
          {
            mcSHOW_DBG_MSG3(("Info=%d ", u1Info_NUM));
          }
          
          mcSHOW_DBG_MSG3(("B0=(  %d,  %d,  %d,  %d,  %d, %d), B1=(  %d,  %d,  %d,  %d,  %d,  %d)\n",
          u1DBG_Dqs0_DFS, u1DBG_Dqs0_Lead, u1DBG_Dqs0_Lag, u1DBG_Dqs0_UI/8, u1DBG_Dqs0_UI%8, u1DBG_Dqs0_PI,
          u1DBG_Dqs1_DFS, u1DBG_Dqs1_Lead, u1DBG_Dqs1_Lag, u1DBG_Dqs1_UI/8, u1DBG_Dqs1_UI%8, u1DBG_Dqs1_PI));
        }

        //Run Time HW Gating Max and Min Value Record
        //Run Time HW Gating MAX_DLY UI : bit[27:22]
        //Run Time HW Gating MAX_DLY PI : bit[21:16]
        //Run Time HW Gating MIN_DLY UI : bit[11:06]
        //Run Time HW Gating MIN_DLY PI : bit[05:00]
        u4Dqs0_MAX_MIN_DLY = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_RK0_B0_STB_MAX_MIN_DLY));
        u2Dqs0_UI_MAX_DLY  = (u4Dqs0_MAX_MIN_DLY >> 22) & 0x3f;
        u2Dqs0_PI_MAX_DLY  = (u4Dqs0_MAX_MIN_DLY >> 16) & 0x3f;
        u2Dqs0_UI_MIN_DLY  = (u4Dqs0_MAX_MIN_DLY >>  6) & 0x3f;
        u2Dqs0_PI_MIN_DLY  = (u4Dqs0_MAX_MIN_DLY >>  0) & 0x3f;

        u4Dqs1_MAX_MIN_DLY = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_RK0_B1_STB_MAX_MIN_DLY));
        u2Dqs1_UI_MAX_DLY  = (u4Dqs1_MAX_MIN_DLY >> 22) & 0x3f;
        u2Dqs1_PI_MAX_DLY  = (u4Dqs1_MAX_MIN_DLY >> 16) & 0x3f;
        u2Dqs1_UI_MIN_DLY  = (u4Dqs1_MAX_MIN_DLY >>  6) & 0x3f;
        u2Dqs1_PI_MIN_DLY  = (u4Dqs1_MAX_MIN_DLY >>  0) & 0x3f;
        
        mcSHOW_DBG_MSG3(("B0 = MAX(4T, UI, PI) MIN(4T, UI, PI), B1 = MAX(4T, UI, PI) MIN(4T, UI, PI)\n"));
        mcSHOW_DBG_MSG3(("B0 = MAX( %d,  %d, %d) MIN( %d,  %d, %d),  B1 = MAX( %d,  %d, %d) MIN( %d,  %d, %d)\n", 
                        u2Dqs0_UI_MAX_DLY/8, u2Dqs0_UI_MAX_DLY%8, u2Dqs0_PI_MAX_DLY, 
                        u2Dqs0_UI_MIN_DLY/8, u2Dqs0_UI_MIN_DLY%8, u2Dqs0_PI_MIN_DLY,
                        u2Dqs1_UI_MAX_DLY/8, u2Dqs1_UI_MAX_DLY%8, u2Dqs1_PI_MAX_DLY, 
                        u2Dqs1_UI_MIN_DLY/8, u2Dqs1_UI_MIN_DLY%8, u2Dqs1_PI_MIN_DLY));
    }
    vSetRank(p, u1RankBak);
    p->channel = u1ChannelBak;
    vSetPHY2ChannelMapping(p, u1ChannelBak);
#endif
}

void DramcPrintRXDQDQSStatus(DRAMC_CTX_T *p, U8 u1Channel)
{
    U8 u1RankIdx, u1RankMax, u1ChannelBak, u1ByteIdx, ii;
    U32 u4ResultDQS_PI, u4ResultDQS_UI, u4ResultDQS_UI_P1;
    U8 u1Dqs_pi[DQS_BIT_NUMBER], u1Dqs_ui[DQS_BIT_NUMBER],u1Dqs_ui_P1[DQS_BIT_NUMBER];
    U16 u2TmpValue, u2TmpUI[DQS_NUMBER], u2TmpPI[DQS_NUMBER];
    U32 MANUDLLFRZ_bak, STBSTATE_OPT_bak;
    U32 backup_rank;
    U8 u1DQX_B0, u1DQS0, u1DQX_B1, u1DQS1;

    u1ChannelBak= p->channel;
    vSetPHY2ChannelMapping(p, u1Channel);
    backup_rank = u1GetRank(p);

    if (p->support_rank_num==RANK_DUAL)
        u1RankMax = RANK_MAX;
    else
        u1RankMax =RANK_1;

    for(u1RankIdx=0; u1RankIdx<u1RankMax; u1RankIdx++)
    {
        vSetRank(p, u1RankIdx);
        mcSHOW_DBG_MSG(("[RXDQDQSStatus] CH%d, RK%d\n", p->channel, u1RankIdx));
        if (u1RankIdx==0)
        u4ResultDQS_PI = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_DQ_RXDLY_TRRO22));
        if (u1RankIdx==1)
        u4ResultDQS_PI = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_DQ_RXDLY_TRRO23));

        u1DQX_B0  = u4ResultDQS_PI & 0xff;
        u1DQS0  = (u4ResultDQS_PI >>8) & 0xff;
        u1DQX_B1  = (u4ResultDQS_PI >>16) & 0xff;
        u1DQS1  = (u4ResultDQS_PI >>24) & 0xff;

        mcSHOW_DBG_MSG(("DQX_B0, DQS0, DQX_B1, DQS1 =(%d, %d, %d, %d)\n\n", u1DQX_B0, u1DQS0, u1DQX_B1, u1DQS1));

        }
    vSetRank(p, backup_rank);

    p->channel = u1ChannelBak;
    vSetPHY2ChannelMapping(p, u1ChannelBak);
}


void DramcPrintIMPTrackingStatus(DRAMC_CTX_T *p, U8 u1Channel)
{
#ifdef IMPEDANCE_TRACKING_ENABLE

    U8 u1RankIdx, u1RankMax, u1ChannelBak, u1ByteIdx, ii;
    U8  DQS_DRVP_2,DQS_ODTN_2,DQS_DRVP,DQS_ODTN;
    U8  DQ_DRVP_2,DQ_ODTN_2,DQ_DRVP,DQ_ODTN;
    U8  CMD_DRVP_2,CMD_ODTN_2,CMD_DRVP,CMD_ODTN;

    u1ChannelBak= p->channel;
    vSetPHY2ChannelMapping(p, u1Channel);

    mcSHOW_DBG_MSG(("[IMPTrackingStatus] CH=%d\n", p->channel));

//    if (u1Channel == CHANNEL_A)
    {
        DQS_DRVP_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS1), IMPCAL_STATUS1_DRVPDQS_SAVE2);
        DQS_ODTN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS1), IMPCAL_STATUS1_DRVNDQS_SAVE2);
        DQS_DRVP = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS1), IMPCAL_STATUS1_DRVPDQS_SAVE1);
        DQS_ODTN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS1), IMPCAL_STATUS1_DRVNDQS_SAVE1);

        DQ_DRVP_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS1), IMPCAL_STATUS1_DRVPDQ_SAVE2);
        DQ_ODTN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS1), IMPCAL_STATUS1_DRVNDQ_SAVE2);
        DQ_DRVP = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS2), IMPCAL_STATUS2_DRVPDQ_SAVE1);
        DQ_ODTN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS2), IMPCAL_STATUS2_DRVNDQ_SAVE1);

        CMD_DRVP_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS2), IMPCAL_STATUS2_DRVPCMD_SAVE2);
        CMD_ODTN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS2), IMPCAL_STATUS2_DRVNCMD_SAVE2);
        CMD_DRVP = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS2), IMPCAL_STATUS2_DRVPCMD_SAVE1);
        CMD_ODTN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS2), IMPCAL_STATUS2_DRVNCMD_SAVE1);
    }
#if 0
    else
    {
        U8 shu_level;

        //Channel B is workaround
        shu_level = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHUSTATUS), SHUSTATUS_SHUFFLE_LEVEL);
        mcSHOW_DBG_MSG(("shu_level=%d\n", shu_level));

        //DQ
        DQ_DRVP_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING1 + shu_level*0x600), SHU1_DRVING1_DQDRV2_DRVP);
        DQ_DRVP   = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING2 + shu_level*0x600), SHU1_DRVING2_DQDRV1_DRVP);
        DQ_ODTN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING3 + shu_level*0x600), SHU1_DRVING3_DQODT2_ODTN);
        DQ_ODTN   = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING4 + shu_level*0x600), SHU1_DRVING4_DQODT1_ODTN);

        //DQS
        DQS_DRVP_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING1 + shu_level*0x600), SHU1_DRVING1_DQSDRV2_DRVP);
        DQS_DRVP   = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING1 + shu_level*0x600), SHU1_DRVING1_DQSDRV1_DRVP);
        DQS_ODTN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING3 + shu_level*0x600), SHU1_DRVING3_DQSODT2_ODTN);
        DQS_ODTN   = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING3 + shu_level*0x600), SHU1_DRVING3_DQSODT1_ODTN);

        //CMD
        CMD_DRVP_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING2 + shu_level*0x600), SHU1_DRVING2_CMDDRV2_DRVP);
        CMD_DRVP   = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING2 + shu_level*0x600), SHU1_DRVING2_CMDDRV1_DRVP);
        CMD_ODTN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING4 + shu_level*0x600), SHU1_DRVING4_CMDODT2_ODTN);
        CMD_ODTN   = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING4 + shu_level*0x600), SHU1_DRVING4_CMDODT1_ODTN);
    }
#endif

    mcSHOW_DBG_MSG(("\tDRVP_2\tODTN_2\tDRVP\tODTN\n"
                    "DQS\t%d\t%d\t%d\t%d\n"
                    "DQ\t%d\t%d\t%d\t%d\n"
                    "CMD\t%d\t%d\t%d\t%d\n",
                    DQS_DRVP_2, DQS_ODTN_2, DQS_DRVP, DQS_ODTN,
                    DQ_DRVP_2, DQ_ODTN_2, DQ_DRVP, DQ_ODTN,
                    CMD_DRVP_2, CMD_ODTN_2, CMD_DRVP, CMD_ODTN));

    p->channel = u1ChannelBak;
    vSetPHY2ChannelMapping(p, u1ChannelBak);
#endif
}
#endif  //#if __ETT__
#endif


U8 u1GetMR4RefreshRate(DRAMC_CTX_T *p, DRAM_CHANNEL_T channel)
{
    U8 u1Dummy, u1RefreshRate;

    vSetPHY2ChannelMapping(p, channel);

    //DramcModeRegRead(p, 4, &u1Dummy);
    //mcSHOW_DBG_MSG(("[u2GetRefreshRate] MR4 0x%x,  u1RefreshRate= 0x%x\n", u1Dummy, u1RefreshRate));
    u1RefreshRate = (U8)u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MISC_STATUSA), MISC_STATUSA_REFRESH_RATE);
    //mcSHOW_DBG_MSG(("[u2GetRefreshRate] channel = %d, u1RefreshRate= 0x%x\n", channel, u1RefreshRate));

    return u1RefreshRate;
}

void vGetImpedanceResult(DRAMC_CTX_T *p, U8 *drvp, U8* drvn)
{
    *drvp = (U8)u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ1), SHU1_B0_DQ1_RG_TX_ARDQ_DRVP_B0);
    *drvn = (U8)u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ1), SHU1_B0_DQ1_RG_TX_ARDQ_DRVN_B0);
}

U8 gEnableSelfWakeup = 0;
void DramcDRS(DRAMC_CTX_T *p, U8 bEnable)
{
if(u1IsLP4Family(p->dram_type))
{
    //R_DMDRS_CNTX[6:0](DVT set 0, HQA set 4 or 5)
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_DRSCTRL), P_Fld(0, DRSCTRL_DRSPB2AB_OPT)
                                                            | P_Fld(0, DRSCTRL_DRSMON_CLR)
                                                            | P_Fld(8, DRSCTRL_DRSDLY)
                                                            | P_Fld(0, DRSCTRL_DRSACKWAITREF)
                                                            | P_Fld(!bEnable, DRSCTRL_DRSDIS)
                                                            | P_Fld(1, DRSCTRL_DRSCLR_EN)
                                                            | P_Fld(3, DRSCTRL_DRS_CNTX)
                                                            | P_Fld(!gEnableSelfWakeup, DRSCTRL_DRS_SELFWAKE_DMYRD_DIS)
                                                            | P_Fld(0, DRSCTRL_DRS_DRSOPT2));
                                                            
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD), P_Fld(1, DUMMY_RD_DMYRD_REORDER_DIS)
                                                            | P_Fld(1, DUMMY_RD_DMYRD_HPRI_DIS));
}                                                                
}


#if ENABLE_DDRPHY_FREQ_METER
#define CKGEN_FMETER 0x0
#define ABIST_FMETER 0x1

/*
1. Select meter clock input: CLK_DBG_CFG[1:0] = 0x0
2. Select clock source from below table: CLK_DBG_CFG[21:16] = 0x39
3. Setup meter div: CLK_MISC_CFG_0[31:24] = 0x0
4. Enable frequency meter: CLK26CALI_0[12] = 0x1
5. Trigger frequency meter: CLK26CALI_0[4] = 0x1
6. Wait until CLK26CALI_0[4] = 0x0
7. Read meter count: CLK26CALI_1[15:0]
8. Calculate measured frequency: freq. = (26 * cal_cnt) / 1024

DE: HK Wean
*/
static U32 FMeter(U8 u1CLKMeterSel, U8 u1CLKMuxSel)
{
#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
    U32 tmp, u4CalCnt;

    // enable fmeter
	DRV_WriteReg32(CLK26CALI_0, (0x1 << 12)); // [12]=1: [8]=0: switch in_clk/clk27m
	tmp = DRV_Reg32(CLK_DBG_CFG);
	tmp &= ~0x3;

	if (u1CLKMeterSel == CKGEN_FMETER)
		tmp |= 0x1;

	DRV_WriteReg32(CLK_DBG_CFG, tmp);


    // select fmeter's input clock
	tmp = DRV_Reg32(CLK_DBG_CFG);

	if (u1CLKMeterSel == CKGEN_FMETER) {
		tmp &= ~(0x3F << 8);
		tmp |=  u1CLKMuxSel << 8;
	} else {
		tmp &= ~(0x3F << 16);
		tmp |=  u1CLKMuxSel << 16;
	}

	DRV_WriteReg32(CLK_DBG_CFG, tmp);

	// setup fmeter div
	tmp = DRV_Reg32(CLK_MISC_CFG_0);
	tmp &= ~(0xFF << 24);
	DRV_WriteReg32(CLK_MISC_CFG_0, tmp);

	// trigger fmeter
	tmp = DRV_Reg32(CLK26CALI_0);
	tmp |= (0x1 << 4);
	DRV_WriteReg32(CLK26CALI_0, tmp);

	// wait
	while (DRV_Reg32(CLK26CALI_0) & (0x1 << 4)) {
		mcDELAY_US(1);
	}

	// read
	u4CalCnt = DRV_Reg32(CLK26CALI_1) & 0xFFFF;

    // disable fmeter
	tmp = DRV_Reg32(CLK26CALI_0);
	tmp &= ~(0x1 << 12);
	DRV_WriteReg32(CLK26CALI_0, tmp);

    return ((u4CalCnt * 26) / 1024);
#endif
}

void DDRPhyFreqMeter()
{
#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
    U32 reg;

    mcSHOW_DBG_MSG3(("\n[PhyFreqMeter]\n"));

    // abist_clk50: AD_MPLL_208M_CK
    mcSHOW_DBG_MSG(("AD_MPLL_208M_CK FREQ=%d\n", FMeter(ABIST_FMETER, 50)));

    // abist_clk57: DA_MPLL_52M_DIV_CK
    mcSHOW_DBG_MSG(("DA_MPLL_52M_DIV_CK FREQ=%d\n", FMeter(ABIST_FMETER, 57)));

	// enable ck_bfe_dcm_en for freqmeter measure ddrphy clock, not needed for normal use
    reg = DRV_Reg32(DRAMC_CH0_TOP0_BASE + 0x2a0);
    DRV_WriteReg32(DRAMC_CH0_TOP0_BASE + 0x2a0, reg | (1 << 11));

    // ckgen_clk55: fmem_ck_bfe_dcm_ch0 (DRAMC CHA's clock before idle mask)
    mcSHOW_DBG_MSG(("fmem_ck_bfe_dcm_ch0 FREQ=%d\n", FMeter(CKGEN_FMETER, 55)));

    // ckgen_clk56: fmem_ck_aft_dcm_ch0 (DRAMC CHA's clock after idle mask, the one really be used)
    mcSHOW_DBG_MSG(("fmem_ck_aft_dcm_ch0 FREQ=%d\n", FMeter(CKGEN_FMETER, 56)));

    DRV_WriteReg32(DRAMC_CH0_TOP0_BASE + 0x2a0, reg);
#endif
}
#endif



#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
#if __ETT__
void GetPhyPllFrequency(DRAMC_CTX_T *p)
{
#if 1
    U8 u1ShuLevel = u4IO32ReadFldAlign(DRAMC_REG_SHUSTATUS, SHUSTATUS_SHUFFLE_LEVEL);
    U32 u4PLL5_ADDR = DDRPHY_SHU1_PLL5+0x500*u1ShuLevel;
    U32 u4PLL8_ADDR = DDRPHY_SHU1_PLL8+0x500*u1ShuLevel;
    U32 u4CA_CMD6 = DDRPHY_SHU1_CA_CMD6+0x500*u1ShuLevel;

    /* VCOFreq = FREQ_XTAL x ((RG_RCLRPLL_SDM_PCW) / 2^(RG_*_RCLRPLL_PREDIV)) / 2^(RG_*_RCLRPLL_POSDIV) */
    U32 u4SDM_PCW = u4IO32ReadFldAlign(u4PLL5_ADDR, SHU1_PLL5_RG_RPHYPLL_SDM_PCW);
    U32 u4PREDIV = u4IO32ReadFldAlign(u4PLL8_ADDR, SHU1_PLL8_RG_RPHYPLL_PREDIV);
    U32 u4POSDIV = u4IO32ReadFldAlign(u4PLL8_ADDR, SHU1_PLL8_RG_RPHYPLL_POSDIV);
    U32 u4CKDIV4 = u4IO32ReadFldAlign(u4CA_CMD6, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA);

    U32 u4VCOFreq = ((52>>u4PREDIV)*(u4SDM_PCW>>8))>>u4POSDIV;
    U32 u4DataRate = u4VCOFreq>>u4CKDIV4;
#else
    U8 u1ShuLevel = (*((volatile unsigned int *)(0x10006474))&0x000000c0)>>6;
    U32 u4PLL5_ADDR = 0x10228d94+0x500*u1ShuLevel;
    U32 u4PLL8_ADDR = 0x10228da0+0x500*u1ShuLevel;
    U32 u4CA_CMD6   = 0x10228d18+0x500*u1ShuLevel;

    U32 u4SDM_PCW = *((volatile unsigned int *)(u4PLL5_ADDR))>>16;
    U32 u4PREDIV = (*((volatile unsigned int *)(u4PLL8_ADDR))&0x000c0000)>>18;
    U32 u4POSDIV = (*((volatile unsigned int *)(u4PLL8_ADDR))&0x00000007);
    U32 u4CKDIV4 = (*((volatile unsigned int *)(u4CA_CMD6))&0x08000000)>>27;


    U32 u4VCOFreq = ((52>>u4PREDIV)*(u4SDM_PCW>>8))>>u4POSDIV;
    U32 u4DataRate = u4VCOFreq>>u4CKDIV4;
#endif
    mcSHOW_DBG_MSG(("PCW=0x%X, u4PREDIV=%d, u4POSDIV=%d, CKDIV4=%d, DataRate=%d\n", u4SDM_PCW, u4PREDIV, u4POSDIV, u4CKDIV4, u4DataRate));
}
#endif
#endif

#if __ETT__
const U32 u4Cannot_Use_Dramc_WBR_Reg[]=
{
    DDRPHY_SHU1_CA_CMD6,
    DDRPHY_SHU1_CA_DLL0,
    DDRPHY_SHU1_CA_DLL1,
    DDRPHY_CA_DLL_ARPI3
};
#define CANNOT_USE_WBR_SIZE ((sizeof(u4Cannot_Use_Dramc_WBR_Reg))/(sizeof(U32)))
void CheckDramcWBR(U32 u4address)
{
    U32 i;
    if (GetDramcBroadcast()==DRAMC_BROADCAST_ON)
    {
        for (i=0; i<CANNOT_USE_WBR_SIZE; i++)
        {
            if (u4Cannot_Use_Dramc_WBR_Reg[i]==u4address)
            {
                mcSHOW_ERR_MSG(("Error! virtual address %x cannot use Dramc WBR\n", u4address));
                while(1);
            }
         }
     }
}
#endif

static void TxCalibrationMoveDQSByDifferentWL(DRAMC_CTX_T *p)
{
    U8 u1ByteIdx;
    U8 backup_rank, u1RankIdx;
    S8 u1freqGroup933minus800or667_WL;

    u1freqGroup933minus800or667_WL = 8-6; //this variable is WL of 1866 - WL of 1200 or 1600

    backup_rank = u1GetRank(p);

    if ((p->freqGroup == 667) || (p->freqGroup == 800)) //if frequency=600 or 800, TX_DQS should be shiffted
    {
        for(u1ByteIdx =0 ; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++) //DQS, DQS_OEN and DQ shold be moved simultaneously
        {
            MoveDramC_TX_DQS(p, u1ByteIdx, -(u1freqGroup933minus800or667_WL<<1)); //WL of 1866 - WL of 1200 or 1600 is the moving value of DQS offset
                                                                                  //note that both WL of DDR1200 and 1600 are equal to 6, and WL of 1866 is 8
            MoveDramC_TX_DQS_OEN(p, u1ByteIdx, -(u1freqGroup933minus800or667_WL<<1));

            for(u1RankIdx=RANK_0; u1RankIdx<p->support_rank_num; u1RankIdx++)
            {
                vSetRank(p, u1RankIdx);
                MoveDramC_TX_DQ(p, u1ByteIdx, -(u1freqGroup933minus800or667_WL<<1));
            }
            vSetRank(p, backup_rank);
        }
    }
}
 
///TODO: EVEREST_PORTING_TODO
#if 0 //#ifdef LOOPBACK_TEST
#define DDRPHY0AO_BASE          0x10228000
#define DDRPHY1AO_BASE          0x10230000
#define DRAMC0_AO_BASE          0x1022a000
#define DRAMC1_AO_BASE          0x10232000
#ifdef LPBK_INTERNAL_EN
void ddrphy_int_lpbk_test_seq (DRAMC_CTX_T *p)
{
    int           lpbk_test_result = 0 ;
    unsigned int  reg_cha_r_data = 0 ;
    unsigned int  reg_chb_r_data = 0 ;
    int           DDRPHY_TX_BASE = 0x0 ;
    int           DDRPHY_RX_BASE = 0x0 ;

    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);

    /*TINFO="[LPBK] Starting to DDRPHY internal loopback test ..."*/

    //*GPIO_DOUT1 = 0x00000100; //! EINT8 - trigger lpbk test

    //! ==============================
    //! - TX setting
    //! ==============================
    //! [21:16]: R_EXTLB_OE_*_ON
    *((UINT32P)(DDRPHY0AO_BASE + 0x0240)) = 0x003fb401;

    //! [5]: CA_TX_MODE; [4]:DQ_TX_MODE; [3]: DQ_MODE for CA
    //! [9]: CA_RX_MODE; [8]:DQ_RX_MODE; [3]: DQ_MODE for CA
    *((UINT32P)(DDRPHY0AO_BASE + 0x0248)) = 0x00000338;

    //! ==============================
    //! - APHY setting
    //! ==============================
    //! B0
    *((UINT32P)(DDRPHY0AO_BASE + 0x00a4)) = 0x000005bc;
    *((UINT32P)(DDRPHY0AO_BASE + 0x00b0)) = 0x010311c1;
    //! B1
    *((UINT32P)(DDRPHY0AO_BASE + 0x0124)) = 0x000005bc;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0130)) = 0x010311c1;
    //! CA
    *((UINT32P)(DDRPHY0AO_BASE + 0x01a4)) = 0x000005bc;
    *((UINT32P)(DDRPHY0AO_BASE + 0x01b0)) = 0x010311c1;

    //! ==============================
    //! - LPBK setting
    //! ==============================
    //! step-1 : trigger RX SWRST
    *((UINT32P)(DDRPHY0AO_BASE + 0x0244)) = 0x0000cc83;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0244)) = 0x0000cc8b;

    //! step-2 : release RX SWRST
    *((UINT32P)(DDRPHY0AO_BASE + 0x0244)) = 0x0000cc81;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0244)) = 0x0000cc89;

    /*TINFO="[LPBK] Trigger TX Enable ..."*/

    //! step-3 : trigger LPBK TX_EN
    *((UINT32P)(DDRPHY0AO_BASE + 0x0244)) = 0x0000cc85;

    mcDELAY_US(1);

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);

    //DramcRegDump(p);

    //! step-4 : wait LPBK done
    do {
    reg_cha_r_data = *((UINT32P)(DDRPHY0AO_BASE + 0x0580)) ;
    reg_chb_r_data = *((UINT32P)(DDRPHY1AO_BASE + 0x0580)) ;
    /*TINFO="[LPBK] Waiting for CHA RX test done ; current RX status is [%x] ...", reg_cha_r_data*/
    /*TINFO="[LPBK] Waiting for CHB RX test done ; current RX status is [%x] ...", reg_chb_r_data*/
    }while( (reg_cha_r_data != 0x0fffffff) || (reg_chb_r_data != 0x0fffffff) ) ;  //! B0-9bits ; B1-9bits ; CA-10bits


    *((UINT32P)(DDRPHY0AO_BASE + 0x0244)) = 0x0000cc81;
    *((UINT32P)(DDRPHY1AO_BASE + 0x0244)) = 0x0000cc81;

    //*GPIO_DOUT1 = 0x00000300; //! EINT9 - trigger lpbk done

    //! step-5 : check fail
    reg_cha_r_data = *((UINT32P)(DDRPHY0AO_BASE + 0x0584)) ;
    reg_chb_r_data = *((UINT32P)(DDRPHY1AO_BASE + 0x0584)) ;

    mcSHOW_DBG_MSG(("[ddrphy_int_lpbk] CH_A=0x%x\n", reg_cha_r_data));
    mcSHOW_DBG_MSG(("[ddrphy_int_lpbk] CH_B=0x%x\n", reg_chb_r_data));


    if( (reg_cha_r_data != 0) || (reg_chb_r_data != 0) ){
    /*TINFO="[LPBK] DDRPHY loopback test FAIL"*/
    /*TINFO="[LPBK] LPBK CHA fail status = %x", reg_cha_r_data*/
    /*TINFO="[LPBK] LPBK CHB fail status = %x", reg_chb_r_data*/
    lpbk_test_result = 0 ;
    } else {
    /*TINFO="[LPBK] DDRPHY loopback test PASS !!!"*/
    lpbk_test_result = 1 ;
    //*GPIO_DOUT1 = 0x00000700; //! EINT10 - trigger lpbk pass
    }

    //*MDM_TM_ENDSIM = 0x0;
    //while(1);


} //! end of ddrphy_int_lpbk_test_seq
#else
//! ----------------------------------------------
//! ddrphy external loopback test sequence control
//! ----------------------------------------------
void ddrphy_ext_lpbk_test_seq ( DRAMC_CTX_T *p, unsigned int LPBK_TX )
{
  int           lpbk_test_result = 0 ;
  unsigned int  reg_r_data = 0 ;
  int           DDRPHY_TX_BASE = 0x0 ;
  int           DDRPHY_RX_BASE = 0x0 ;

  /*TINFO="[LPBK] Starting to DDRPHY external loopback test ..."*/

  //*GPIO_DOUT1 = 0x00000100; //! EINT8 - trigger lpbk test

  if( LPBK_TX==0 ){
    //! CHN-A is TX
    /*TINFO="[LPBK] CHN-A is TX ; CHN-B is RX ..."*/
    DDRPHY_TX_BASE = DDRPHY0AO_BASE ;
    DDRPHY_RX_BASE = DDRPHY1AO_BASE ;
  } else {
    //! CHN-B is TX
    /*TINFO="[LPBK] CHN-B is TX ; CHN-A is RX ..."*/
    DDRPHY_TX_BASE = DDRPHY1AO_BASE ;
    DDRPHY_RX_BASE = DDRPHY0AO_BASE ;
  }

  //! step-1 : APHY TX/RX setting

  //! ==============================
  //! - TX setting
  //! ==============================
  //! [21:16]: R_EXTLB_OE_*_ON
  *((UINT32P)(DDRPHY_TX_BASE + 0x0240)) = 0x003fb401;

  //! [5]: CA_TX_MODE; [4]:DQ_TX_MODE; [3]: DQ_MODE for CA
  *((UINT32P)(DDRPHY_TX_BASE + 0x0248)) = 0x00000038;

  //! ==============================
  //! - RX setting
  //! ==============================
  //! DDRPHY_GLUE_MISC_CHB
  //! [9]: CA_RX_MODE; [8]:DQ_RX_MODE; [3]: DQ_MODE for CA
  *((UINT32P)(DDRPHY_RX_BASE + 0x0248)) = 0x00000308;

  *((UINT32P)(DDRPHY_RX_BASE + 0x0C14)) = 0x00000010;
  *((UINT32P)(DDRPHY_RX_BASE + 0x0C94)) = 0x00000010;
  *((UINT32P)(DDRPHY_RX_BASE + 0x0D14)) = 0x00000010;
  //! APHY-B0
  *((UINT32P)(DDRPHY_RX_BASE + 0x00A4)) = 0x000005b8;
  *((UINT32P)(DDRPHY_RX_BASE + 0x00AC)) = 0x00010000;
  *((UINT32P)(DDRPHY_RX_BASE + 0x00B0)) = 0x00001041;
  //! APHY-B1
  *((UINT32P)(DDRPHY_RX_BASE + 0x0124)) = 0x000005b8;
  *((UINT32P)(DDRPHY_RX_BASE + 0x012C)) = 0x00010000;
  *((UINT32P)(DDRPHY_RX_BASE + 0x0130)) = 0x00001041;
  //! APHY-CA
  *((UINT32P)(DDRPHY_RX_BASE + 0x01A4)) = 0x000005b8;
  *((UINT32P)(DDRPHY_RX_BASE + 0x01AC)) = 0x00010000;
  *((UINT32P)(DDRPHY_RX_BASE + 0x01B0)) = 0x00001041;

  //! step-2 : trigger RX SWRST
  *((UINT32P)(DDRPHY_TX_BASE + 0x0244)) = 0x0000cc83;
  *((UINT32P)(DDRPHY_TX_BASE + 0x0244)) = 0x0000cc8b;
  *((UINT32P)(DDRPHY_RX_BASE + 0x0244)) = 0x0000cc83;
  *((UINT32P)(DDRPHY_RX_BASE + 0x0244)) = 0x0000cc8b;

  //! step-3 : release RX SWRST
  *((UINT32P)(DDRPHY_TX_BASE + 0x0244)) = 0x0000cc81;
  *((UINT32P)(DDRPHY_TX_BASE + 0x0244)) = 0x0000cc89;
  *((UINT32P)(DDRPHY_RX_BASE + 0x0244)) = 0x0000cc81;
  *((UINT32P)(DDRPHY_RX_BASE + 0x0244)) = 0x0000cc89;

  /*TINFO="[LPBK] Trigger TX Enable ..."*/

  //! step-4 : trigger LPBK TX_EN
  *((UINT32P)(DDRPHY_TX_BASE + 0x0244)) = 0x0000cc85;

  mcDELAY_US(1);

  reg_r_data = *((UINT32P)(DDRPHY_RX_BASE + 0x0580)) ;
  mcSHOW_DBG_MSG(("Reg 0x580=%x\n", reg_r_data));
  //! step-5 : wait LPBK done
  do {
    reg_r_data = *((UINT32P)(DDRPHY_RX_BASE + 0x0580)) ;

    reg_r_data = *((UINT32P)(DDRPHY_RX_BASE + 0x0580)) ;
    mcSHOW_DBG_MSG(("Reg 0x580=%x\n", reg_r_data));


    /*TINFO="[LPBK] Waiting for RX test done ; current RX status is [%x] ...", reg_r_data*/
    //!temply mark : waiting for SH update ECO RTL files }while( reg_r_data != 0x0fffffff ) ;  //! B0-9bits ; B1-9bits ; CA-10bits
  }while( reg_r_data != 0xfffffff ) ;  //! B0-9bits ; B1-9bits ; CA-10bits

  //*GPIO_DOUT1 = 0x00000300; //! EINT-9 : trigger lpbk done

  *((UINT32P)(DDRPHY0AO_BASE + 0x0244)) = 0x0000cc81;
  *((UINT32P)(DDRPHY1AO_BASE + 0x0244)) = 0x0000cc81;

  //! check fail
  reg_r_data = *((UINT32P)(DDRPHY_RX_BASE + 0x0584)) ;
  mcSHOW_DBG_MSG(("Reg 0x584=%x\n", reg_r_data));
  if( reg_r_data != 0 ){
    /*TINFO="[LPBK] DDRPHY loopback test FAIL"*/
    /*TINFO="[LPBK] LPBK fail status = %x", reg_r_data*/
    lpbk_test_result = 0 ;
  } else {
    /*TINFO="[LPBK] DDRPHY loopback test PASS !!!"*/
    lpbk_test_result = 1 ;
    //*GPIO_DOUT1 = 0x00000700; //! EINT-10 : trigger lpbk pass
  }

  reg_r_data = *((UINT32P)(DDRPHY_RX_BASE + 0x0588)) ;
  mcSHOW_DBG_MSG(("Reg 0x588=%x\n", reg_r_data));

  //*MDM_TM_ENDSIM = 0x0;
  //while(1);

}//! end of ddrphy_ext_lpbk_test_seq

#endif

//#define DDR1600_LPBK
DRAM_STATUS_T DramcLoopbackTest(DRAMC_CTX_T *p)
{
    int           lpbk_tx     = 0 ;
    int           fpc_result  = 0 ;
    unsigned int  ddrphy_drvp = 0 ;
    unsigned int  ddrphy_drvn = 0 ;
    unsigned int  ddrphy_odtn = 0 ;

#if 0//mem.c
    /*TINFO="[LPBK] start to do ddrphy initial setting ..."*/
    /*TINFO="[LPBK] 2CH simultaneous configuration"*/
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    *((UINT32P)(DDRPHY0AO_BASE + 0x0040)) = 0x00000001;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0278)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x027c)) = 0xfe3fffff;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0274)) = 0xffffffff;
    *((UINT32P)(DDRPHY0AO_BASE + 0x028c)) = 0x006003bf;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0294)) = 0x11300000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0d84)) = 0x0000001f;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0c1c)) = 0x00000011;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0c9c)) = 0x00000001;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0d90)) = 0xe57800fe;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0d98)) = 0xe57800fe;
    *((UINT32P)(DDRPHY0AO_BASE + 0x01a0)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x00a0)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0120)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x05e4)) = 0x8000000e;
    *((UINT32P)(DDRPHY0AO_BASE + 0x05e8)) = 0x00000005;
    *((UINT32P)(DDRPHY0AO_BASE + 0x05f0)) = 0x10000020;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0670)) = 0x10000020;
    *((UINT32P)(DDRPHY0AO_BASE + 0x05f0)) = 0x10000220;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0670)) = 0x10000220;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0608)) = 0x20000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0808)) = 0x20000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0688)) = 0x20000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0888)) = 0x20000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0c14)) = 0x0030000e;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0c94)) = 0x0030000e;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e08)) = 0x120a120a;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e0c)) = 0x120a120a;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e10)) = 0x120a120a;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e14)) = 0x120a120a;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e18)) = 0x140c120a;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0f08)) = 0x130b130b;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0f0c)) = 0x130b130b;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0f10)) = 0x130b130b;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0f14)) = 0x130b130b;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0f18)) = 0x150d130b;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e58)) = 0x120a120a;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e5c)) = 0x120a120a;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e60)) = 0x120a120a;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e64)) = 0x120a120a;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e68)) = 0x140c120a;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0f58)) = 0x130b130b;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0f5c)) = 0x130b130b;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0f60)) = 0x130b130b;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0f64)) = 0x130b130b;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0f68)) = 0x150d130b;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0604)) = 0x00020002;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0804)) = 0x00020002;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0684)) = 0x00020002;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0884)) = 0x00020002;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0608)) = 0xb0800000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0808)) = 0xb0800000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0688)) = 0xb0800000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0888)) = 0xb0800000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x05f0)) = 0x00000220;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0670)) = 0x00000220;
    *((UINT32P)(DDRPHY0AO_BASE + 0x00a4)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0124)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x00a4)) = 0x00000010;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0124)) = 0x00000010;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0288)) = 0xffffffff;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e00)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0f00)) = 0xaaaaaaaa;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1000)) = 0xaaaaaaaa;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e04)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0f04)) = 0x0000000a;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1004)) = 0x0000000a;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e50)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0f50)) = 0xaaaaaaaa;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1050)) = 0xaaaaaaaa;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e54)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0f54)) = 0x0000000a;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1054)) = 0x0000000a;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e1c)) = 0x001f1f00;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e6c)) = 0x001f1f00;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0f1c)) = 0x001f1f00;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0f6c)) = 0x001f1f00;
    *((UINT32P)(DDRPHY0AO_BASE + 0x101c)) = 0x001f1f00;
    *((UINT32P)(DDRPHY0AO_BASE + 0x106c)) = 0x001f1f00;
    *((UINT32P)(DDRPHY0AO_BASE + 0x00a8)) = 0x00001010;
    *((UINT32P)(DDRPHY0AO_BASE + 0x00ac)) = 0x01111010;
    *((UINT32P)(DDRPHY0AO_BASE + 0x00b0)) = 0x010310c0;
    *((UINT32P)(DDRPHY0AO_BASE + 0x00ac)) = 0x03111000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0128)) = 0x00001010;
    *((UINT32P)(DDRPHY0AO_BASE + 0x012c)) = 0x01111010;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0130)) = 0x010310c0;
    *((UINT32P)(DDRPHY0AO_BASE + 0x012c)) = 0x03111000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x01a4)) = 0x0000008c;
    *((UINT32P)(DDRPHY0AO_BASE + 0x01b0)) = 0x00020000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0d04)) = 0x00000101;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0d08)) = 0x00000101;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1204)) = 0x00000101;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1208)) = 0x00000101;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0000)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0004)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0008)) = 0x00000010;
#ifdef O_0P7V_SDF
    *((UINT32P)(DDRPHY0AO_BASE + 0x0d94)) = 0x33000003;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0d9c)) = 0x33000003;
#else
#ifdef DDR1600_LPBK
    //! 1600
    *((UINT32P)(DDRPHY0AO_BASE + 0x0d94)) = 0x1e400003;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0d9c)) = 0x0f200001;
#else
    //! 3200
    *((UINT32P)(DDRPHY0AO_BASE + 0x0d94)) = 0x3d000003;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0d9c)) = 0x1e400001;
#endif
#endif
    *((UINT32P)(DDRPHY0AO_BASE + 0x0088)) = 0x880aec00;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0108)) = 0x880aec00;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0188)) = 0x880aec00;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0180)) = 0x00000008;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0080)) = 0x00000008;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0100)) = 0x00000008;
    *((UINT32P)(DDRPHY0AO_BASE + 0x00a4)) = 0x00000018;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0124)) = 0x00000018;
    *((UINT32P)(DDRPHY0AO_BASE + 0x01a4)) = 0x0000008c;

    mcDELAY_US(1);

    *((UINT32P)(DDRPHY0AO_BASE + 0x0da0)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x12a0)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x17a0)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1ca0)) = 0x00000000;

    mcDELAY_US(1);

    *((UINT32P)(DDRPHY0AO_BASE + 0x0da4)) = 0x00019200;
    *((UINT32P)(DDRPHY0AO_BASE + 0x12a4)) = 0x00019200;
    *((UINT32P)(DDRPHY0AO_BASE + 0x17a4)) = 0x00019200;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1ca4)) = 0x00019200;

    mcDELAY_US(1);

    *((UINT32P)(DDRPHY0AO_BASE + 0x0da8)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x12a8)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x17a8)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1ca8)) = 0x00000000;

    mcDELAY_US(1);

    *((UINT32P)(DDRPHY0AO_BASE + 0x0000)) = 0x80000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0004)) = 0x80000000;

    mcDELAY_US(2);

    *((UINT32P)(DDRPHY0AO_BASE + 0x000c)) = 0x006d0000;

    mcDELAY_US(1);

    *((UINT32P)(DDRPHY0AO_BASE + 0x0d80)) = 0x00000003;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1280)) = 0x00000003;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1780)) = 0x00000003;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1c80)) = 0x00000003;

    mcDELAY_US(1);

    *((UINT32P)(DDRPHY0AO_BASE + 0x0c18)) = 0x04000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1118)) = 0x04000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1618)) = 0x04000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1b18)) = 0x04000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0c98)) = 0x04000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1198)) = 0x04000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1698)) = 0x04000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1b98)) = 0x04000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0d18)) = 0x04000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1218)) = 0x04000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1718)) = 0x04000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1c18)) = 0x04000000;

    mcDELAY_US(1);

    *((UINT32P)(DDRPHY0AO_BASE + 0x0184)) = 0x00200000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0084)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0104)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0188)) = 0x00000800;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0088)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0108)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x018c)) = 0x000ba000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x008c)) = 0x0002e800;
    *((UINT32P)(DDRPHY0AO_BASE + 0x010c)) = 0x0002e800;
    *((UINT32P)(DDRPHY0AO_BASE + 0x00a4)) = 0x0000041e;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0124)) = 0x0000041e;
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    *((UINT32P)(DDRPHY0AO_BASE + 0x0194)) = 0x00698608;
    *((UINT32P)(DDRPHY1AO_BASE + 0x0194)) = 0xc0778608;// change chb ca dll to slave mode
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    *((UINT32P)(DDRPHY0AO_BASE + 0x0094)) = 0xc0778608;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0114)) = 0xc0778608;
    *((UINT32P)(DDRPHY0AO_BASE + 0x01b8)) = 0x00140101;
    *((UINT32P)(DDRPHY0AO_BASE + 0x026c)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0d14)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1214)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1714)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1c14)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0d00)) = 0x00104010;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1200)) = 0x00104010;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1700)) = 0x00104010;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1c00)) = 0x00104010;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0d18)) = 0x04030000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1218)) = 0x04030000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1718)) = 0x04030000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1c18)) = 0x04030000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0d1c)) = 0x04e10002;
    *((UINT32P)(DDRPHY0AO_BASE + 0x121c)) = 0x04e10002;
    *((UINT32P)(DDRPHY0AO_BASE + 0x171c)) = 0x04e10002;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1c1c)) = 0x04e10002;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0c1c)) = 0x00200011;
    *((UINT32P)(DDRPHY0AO_BASE + 0x111c)) = 0x00200002;
    *((UINT32P)(DDRPHY0AO_BASE + 0x161c)) = 0x00200002;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1b1c)) = 0x00200002;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0c9c)) = 0x00200001;
    *((UINT32P)(DDRPHY0AO_BASE + 0x119c)) = 0x00200002;
    *((UINT32P)(DDRPHY0AO_BASE + 0x169c)) = 0x00200002;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1b9c)) = 0x00200002;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0c18)) = 0x04010000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1118)) = 0x04010000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1618)) = 0x04010000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1b18)) = 0x04010000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0c98)) = 0x04010000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1198)) = 0x04010000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1698)) = 0x04010000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x1b98)) = 0x04010000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0294)) = 0x11400000;
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    *((UINT32P)(DDRPHY0AO_BASE + 0x0270)) = 0xfff7fbfb;
    *((UINT32P)(DDRPHY1AO_BASE + 0x0270)) = 0xfffbfbfb;// change chb ca dll to slave mode
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);

    mcDELAY_US(10);

    *((UINT32P)(DDRPHY0AO_BASE + 0x0180)) = 0x0000000c;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0080)) = 0x00000009;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0100)) = 0x00000009;

    mcDELAY_US(1);

    *((UINT32P)(DDRPHY0AO_BASE + 0x0004)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0d90)) = 0xe57800ff;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0d98)) = 0xe57800ff;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0284)) = 0x0000001f;

    mcDELAY_US(1);

    //*((UINT32P)(DDRPHY0AO_BASE + 0x0194)) = 0x00698e08;
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    *((UINT32P)(DDRPHY0AO_BASE + 0x0194)) = 0x00698e08;
    *((UINT32P)(DDRPHY1AO_BASE + 0x0194)) = 0xc0778e08; // change chb ca dll to slave mode
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);

    mcDELAY_US(1);

    *((UINT32P)(DDRPHY0AO_BASE + 0x0094)) = 0xc0778e08;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0114)) = 0xc0778e08;

    mcDELAY_US(2);

    *((UINT32P)(DDRPHY0AO_BASE + 0x00a4)) = 0x0001051e;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0124)) = 0x0001051e;
    *((UINT32P)(DRAMC0_AO_BASE + 0x0b04)) = 0x0f0f0f0f;
    *((UINT32P)(DRAMC0_AO_BASE + 0x0c04)) = 0x0f0f0f0f;
    *((UINT32P)(DRAMC0_AO_BASE + 0x0204)) = 0x00014310;

    //! ==============================
    //! - DDRPHY lpbk common setting
    //! ==============================
    /*TINFO="[LPBK] start to do loopback test CHA/B common configuration ..."*/

    /*TINFO="[LPBK] DRVP, DRVN and ODT setting ..."*/
    //! 2016.04.28, from SA's result, SW calibration ODT turn-on
    ddrphy_drvp = 5   ;
    ddrphy_drvn = 9   ;
    ddrphy_odtn = 4   ;
    *((UINT32P)(DRAMC0_AO_BASE + 0x08a8)) = (ddrphy_drvp << 25) |
                  (ddrphy_drvn << 20) |
                  (ddrphy_drvp << 15) |
                  (ddrphy_drvn << 10) |
                  (ddrphy_drvp <<  5) |
                  (ddrphy_drvn <<  0) ;

    *((UINT32P)(DRAMC0_AO_BASE + 0x08ac)) = (ddrphy_drvp << 25) |
                  (ddrphy_drvn << 20) |
                  (ddrphy_drvp << 15) |
                  (ddrphy_drvn << 10) |
                  (ddrphy_drvp <<  5) |
                  (ddrphy_drvn <<  0) ;

    *((UINT32P)(DRAMC0_AO_BASE + 0x08b0)) = (ddrphy_odtn << 20) |
                  (ddrphy_odtn << 10) |
                  (ddrphy_odtn <<  0) ;

    *((UINT32P)(DRAMC0_AO_BASE + 0x08b4)) = (ddrphy_odtn << 20) |
                  (ddrphy_odtn << 10) |
                  (ddrphy_odtn <<  0) ;
#endif
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);

    //close all dcm
#if (SW_CHANGE_FOR_SIMULATION==0)
    EnableDramcPhyDCM(p, 0);
#endif

    /*TINFO="[LPBK] RX input delay setting ..."*/
    //! for Byte-0
    *((UINT32P)(DDRPHY0AO_BASE + 0x0608)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e08)) = 0x00000000; //! [29:24]: RG_RK0_RX_ARDQ1_F_DLY / [21:16]: RG_RK0_RX_ARDQ1_R_DLY
                              //! [13: 8]: RG_RK0_RX_ARDQ0_F_DLY / [ 5: 0]: RG_RK0_RX_ARDQ0_R_DLY
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e0c)) = 0x00000000; //! [29:24]: RG_RK0_RX_ARDQ3_F_DLY / [21:16]: RG_RK0_RX_ARDQ3_R_DLY
                              //! [13: 8]: RG_RK0_RX_ARDQ2_F_DLY / [ 5: 0]: RG_RK0_RX_ARDQ3_R_DLY
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e10)) = 0x00000000; //! [29:24]: RG_RK0_RX_ARDQ5_F_DLY / [21:16]: RG_RK0_RX_ARDQ5_R_DLY
                              //! [13: 8]: RG_RK0_RX_ARDQ4_F_DLY / [ 5: 0]: RG_RK0_RX_ARDQ4_R_DLY
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e14)) = 0x00000000; //! [29:24]: RG_RK0_RX_ARDQ7_F_DLY / [21:16]: RG_RK0_RX_ARDQ7_R_DLY
                              //! [13: 8]: RG_RK0_RX_ARDQ6_F_DLY / [ 5: 0]: RG_RK0_RX_ARDQ6_R_DLY
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e18)) = 0x00000000; //! [29:24]: RG_RK0_RX_ARDQS_F_DLY / [21:16]: RG_RK0_RX_ARDQS_R_DLY
                              //! [13: 8]: RG_RK0_RX_ARDQM_F_DLY / [ 5: 0]: RG_RK0_RX_ARDQM_R_DLY
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e1c)) = 0x00000000; //! it dosn't need to set TX delay

    //! for Byte-1
    *((UINT32P)(DDRPHY0AO_BASE + 0x0688)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e58)) = 0x00000000; //! [29:24]: RG_RK0_RX_ARDQ1_F_DLY / [21:16]: RG_RK0_RX_ARDQ1_R_DLY
                              //! [13: 8]: RG_RK0_RX_ARDQ0_F_DLY / [ 5: 0]: RG_RK0_RX_ARDQ0_R_DLY
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e5c)) = 0x00000000; //! [29:24]: RG_RK0_RX_ARDQ3_F_DLY / [21:16]: RG_RK0_RX_ARDQ3_R_DLY
                              //! [13: 8]: RG_RK0_RX_ARDQ2_F_DLY / [ 5: 0]: RG_RK0_RX_ARDQ3_R_DLY
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e60)) = 0x00000000; //! [29:24]: RG_RK0_RX_ARDQ5_F_DLY / [21:16]: RG_RK0_RX_ARDQ5_R_DLY
                              //! [13: 8]: RG_RK0_RX_ARDQ4_F_DLY / [ 5: 0]: RG_RK0_RX_ARDQ4_R_DLY
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e64)) = 0x00000000; //! [29:24]: RG_RK0_RX_ARDQ7_F_DLY / [21:16]: RG_RK0_RX_ARDQ7_R_DLY
                              //! [13: 8]: RG_RK0_RX_ARDQ6_F_DLY / [ 5: 0]: RG_RK0_RX_ARDQ6_R_DLY
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e68)) = 0x00000000; //! [29:24]: RG_RK0_RX_ARDQS_F_DLY / [21:16]: RG_RK0_RX_ARDQS_R_DLY
                              //! [13: 8]: RG_RK0_RX_ARDQM_F_DLY / [ 5: 0]: RG_RK0_RX_ARDQM_R_DLY
    *((UINT32P)(DDRPHY0AO_BASE + 0x0e6c)) = 0x00000000; //! it dosn't need to set TX delay

    //! enable lpbk CG control
    *((UINT32P)(DDRPHY0AO_BASE + 0x0284)) = 0x0000801f;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0290)) = 0xffffffff;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0188)) = 0x00000000;

    //! set RG_TX_ARDQ_PULL_DN_B0/B1 to low
    *((UINT32P)(DDRPHY0AO_BASE + 0x00b4)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0134)) = 0x00000000;

    //! enable clock and TX/RX setting of ARCMD/BRCMD
    *((UINT32P)(DDRPHY0AO_BASE + 0x018C)) = 0x000ba800;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0198)) = 0x0f000060;
    *((UINT32P)(DDRPHY0AO_BASE + 0x019C)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x01A0)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x01A4)) = 0x0001071e;
    *((UINT32P)(DDRPHY0AO_BASE + 0x01A8)) = 0x00001010;
    *((UINT32P)(DDRPHY0AO_BASE + 0x01AC)) = 0x03111010;
    *((UINT32P)(DDRPHY0AO_BASE + 0x01B0)) = 0x010310c0;
    *((UINT32P)(DDRPHY0AO_BASE + 0x01B4)) = 0x00000000;
    *((UINT32P)(DDRPHY0AO_BASE + 0x01B8)) = 0x00000000;

    //! [0]: LFSR_EN; [1]: SSO_EN; [2]: XTALK_EN
#if 0
    *((UINT32P)(DDRPHY0AO_BASE + 0x024C)) = 0x00000007;
#else
    *((UINT32P)(DDRPHY0AO_BASE + 0x024C)) = 0x00000004;
#endif

    /*TINFO="[LPBK] Ignore pinmux  ..."*/
    //! xtalk TX bit toggle setting
    //! x-talk TX
    *((UINT32P)(DDRPHY0AO_BASE + 0x0250)) = 0x77777777  ; //! 0x76543210;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0254)) = 0x77777777  ; //! 0x65432100;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0258)) = 0x77777777  ; //! 0x54321007;
    *((UINT32P)(DDRPHY0AO_BASE + 0x025C)) = 0x77777777  ; //! 0x54321076;
    //! x-talk RX
    *((UINT32P)(DDRPHY0AO_BASE + 0x02F8)) = 0x77777777  ; //! 0x76543210;
    *((UINT32P)(DDRPHY0AO_BASE + 0x02FC)) = 0x77777777  ; //! 0x65432100;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0300)) = 0x77777777  ; //! 0x54321007;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0304)) = 0x77777777  ; //! 0x54321076;

    //! TX LFSR
    *((UINT32P)(DDRPHY0AO_BASE + 0x0200)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0204)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0208)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x020c)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0210)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0214)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0218)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x021c)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0220)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0224)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0228)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x022c)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0230)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0234)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x0238)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x023c)) = 0x4c044c04  ;

    //! RX LFSR
    *((UINT32P)(DDRPHY0AO_BASE + 0x02b4)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x02b8)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x02bc)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x02c0)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x02c4)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x02c8)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x02cc)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x02d0)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x02d4)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x02d8)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x02dc)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x02e0)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x02e4)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x02e8)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x02ec)) = 0x4c044c04  ;
    *((UINT32P)(DDRPHY0AO_BASE + 0x02f0)) = 0x4c044c04  ;

    //! ==============================
    //! - DDRPHY lpbk test flow
    //! ==============================
    //! step-0 : trigger loopback prepare on
    *((UINT32P)(DDRPHY0AO_BASE + 0x0244)) = 0x0000cc81;

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);

#ifdef LPBK_MODE_0
    lpbk_tx = 0 ; //! CHN-A is TX
#else
    lpbk_tx = 1 ; //! CHN-B is TX
#endif

#if 0
    //! initial debug output
    *GPIO_MODE1 = 0x0;
    *GPIO_DIR1  = 0x00000700;
    *GPIO_DOUT1 = 0x00000000; //PAD_EINT8/9/10
#endif


    #if 0//Rx DQ Delay
    {
        UINT8 iDelay = 0;
        UINT8 ch, ii = 0;
        for(iDelay=1; iDelay<64; iDelay++)
        {
            U32 u4value = ((U32) iDelay) | (((U32)iDelay)<<8) | (((U32)iDelay)<<16) | (((U32)iDelay)<<24);
            for(ch=CHANNEL_A; ch<CHANNEL_NUM; ch++)
            {
                p->channel = ch;
                for (ii=0; ii<4; ii++)
                {
                    if(u1IsLP4Family(p->dram_type))
                    {
                        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ2+ ii*4), u4value);//DQ0~DQ7
                        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ2+ ii*4), u4value);//DQ8~DQ15
                    }
                }
                if(p->dram_type == TYPE_LPDDR4 || p->dram_type == TYPE_LPDDR4X || p->dram_type == TYPE_LPDDR4P)
                {
                    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ6), \
                                            P_Fld(iDelay,SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0) |P_Fld(iDelay,SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_F_DLY_B0));
                    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ6), \
                                    P_Fld(iDelay,SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1) |P_Fld(iDelay,SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_F_DLY_B1));
                }
                DramPhyReset(p);
            }
            mcSHOW_DBG_MSG(("u1RxDqDly=%d\n", iDelay));
#ifdef LPBK_INTERNAL_EN
            ddrphy_int_lpbk_test_seq ( p ) ;
#else
            ddrphy_ext_lpbk_test_seq ( p, 1 ) ;
#endif

            //mcDELAY_MS(1000);
        }
    }
    #elif 0//RxDQS Delay
    {
        UINT8 iDelay = 0;
        UINT8 ch, ii = 0;
        for(iDelay=0; iDelay<64; iDelay++)
        {
            U32 u4value = ((U32) iDelay) | (((U32)iDelay)<<8) | (((U32)iDelay)<<16) | (((U32)iDelay)<<24);
            for(ch=CHANNEL_A; ch<CHANNEL_NUM; ch++)
            {
                p->channel = ch;
                //for (ii=0; ii<4; ii++)
                {
                    // Set DQS delay
                    if(u1IsLP4Family(p->dram_type))
                    {
                        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ6), \
                                                P_Fld((iDelay),SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0) |P_Fld((iDelay),SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_F_DLY_B0));
                        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ6), \
                                        P_Fld(iDelay,SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1) |P_Fld(iDelay,SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_F_DLY_B1));
                    }
                }
                DramPhyReset(p);
            }
            mcSHOW_DBG_MSG(("u1RxDqsDly=%d\n", iDelay));
#ifdef LPBK_INTERNAL_EN
            ddrphy_int_lpbk_test_seq ( p ) ;
#else
            ddrphy_ext_lpbk_test_seq ( p, 1 ) ;
#endif

            //mcDELAY_MS(1000);
        }
    }
    #elif 1//Rx CA Delay
    {
        UINT8 iDelay = 0;
        UINT8 ch, ii = 0;
        for(iDelay=20; iDelay<=20; iDelay++)
        {
            U32 u4value = ((U32) iDelay) | (((U32)iDelay)<<8) | (((U32)iDelay)<<16) | (((U32)iDelay)<<24);
            for(ch=CHANNEL_A; ch<CHANNEL_NUM; ch++)
            {
                p->channel = ch;
                for (ii=0; ii<4; ii++)
                {
                    if(u1IsLP4Family(p->dram_type))
                    {
                        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ2+ ii*4), u4value);//DQ0~DQ7
                        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ2+ ii*4), u4value);//DQ8~DQ15
                    }
                }
                if(p->dram_type == TYPE_LPDDR4 || p->dram_type == TYPE_LPDDR4X || p->dram_type == TYPE_LPDDR4P)
                {
                    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ6), \
                                            P_Fld(iDelay,SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0) |P_Fld(iDelay,SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_F_DLY_B0));
                    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ6), \
                                    P_Fld(iDelay,SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1) |P_Fld(iDelay,SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_F_DLY_B1));
                }
            }
            mcSHOW_DBG_MSG(("u1RxDqDly=%d\n", iDelay));
            //mcDELAY_MS(1000);
        }
        for(iDelay=0; iDelay<64; iDelay++)
        {
            U32 u4value = ((U32) iDelay) | (((U32)iDelay)<<8) | (((U32)iDelay)<<16) | (((U32)iDelay)<<24);
            for(ch=CHANNEL_A; ch<CHANNEL_NUM; ch++)
            {
                p->channel = ch;
                if(u1IsLP4Family(p->dram_type))
                {
                    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD2), u4value);//CA0, CA1
                    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD3), u4value);//CA2, CA3
                    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD4), u4value);//CA4, CA5
                    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD5), u4value);//CKE0, CKE1
                    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD6), u4value);//CKE2, CS0
                    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD7), u4value);//CS1, CS2
                }
                DramPhyReset(p);
            }
            mcSHOW_DBG_MSG(("u1RxCADly=%d\n", iDelay));
#ifdef LPBK_INTERNAL_EN
            ddrphy_int_lpbk_test_seq(p) ;
#else
            ddrphy_ext_lpbk_test_seq ( p, 1 ) ;
#endif

            //mcDELAY_MS(1000);
        }
        DramPhyReset(p);

#ifdef LPBK_INTERNAL_EN
        ddrphy_int_lpbk_test_seq(p) ;
#else
        ddrphy_ext_lpbk_test_seq ( p, 0 ) ;
#endif
    }
    #endif
    //ddrphy_ext_lpbk_test_seq ( lpbk_tx ) ;
}//! end of fpc_lpbk_mem_init
#endif


#ifdef DDR_INIT_TIME_PROFILING
//Kevin-CW Chen
//M0 ARMPLL div 1  = 897Mhz
//L1.mt6797.dev for smt ? 1.3Ghz
UINT32 temp;

void TimeProfileBegin(void)
{
    /* enable ARM CPU PMU */
    asm volatile(
        "MRC p15, 0, %0, c9, c12, 0\n"
        "BIC %0, %0, #1 << 0\n"   /* disable */
        "ORR %0, %0, #1 << 2\n"   /* reset cycle count */
        "BIC %0, %0, #1 << 3\n"   /* count every clock cycle */
        "MCR p15, 0, %0, c9, c12, 0\n"
        : "+r"(temp)
        :
        : "cc"
    );
    asm volatile(
        "MRC p15, 0, %0, c9, c12, 0\n"
        "ORR %0, %0, #1 << 0\n"   /* enable */
        "MCR p15, 0, %0, c9, c12, 0\n"
        "MRC p15, 0, %0, c9, c12, 1\n"
        "ORR %0, %0, #1 << 31\n"
        "MCR p15, 0, %0, c9, c12, 1\n"
        : "+r"(temp)
        :
        : "cc"
    );
}

UINT32 TimeProfileEnd(void)
{
    UINT32 div = 897*1024;
    asm volatile(
        "MRC p15, 0, %0, c9, c12, 0\n"
        "BIC %0, %0, #1 << 0\n"   /* disable */
        "MCR p15, 0, %0, c9, c12, 0\n"
        "MRC p15, 0, %0, c9, c13, 0\n"
        : "+r"(temp)
        :
        : "cc"
    );

    //mcSHOW_ERR_MSG((" TimeProfileEnd: %d CPU cycles(%d ms)\n\r", temp, temp/div));
    return (temp/div);
}
#endif

U32 u4gpRegBackupVlaue[64];
void DramcBackupRegisters(DRAMC_CTX_T *p, U32 *backup_addr, U32 backup_num)
{
    U32 u4RegIdx;

#if __ETT__
    if (backup_num>64)
    {
        mcSHOW_ERR_MSG(("[DramcBackupRegisters] backup number over 64!!!\n"));
        while(1);
    }
#endif

    for(u4RegIdx=0; u4RegIdx < backup_num; u4RegIdx++)
    {
        u4gpRegBackupVlaue[u4RegIdx] = u4IO32Read4B(backup_addr[u4RegIdx]);
        //mcSHOW_DBG_MSG(("Backup Reg(0x%X) = 0x%X\n", backup_addr[u4RegIdx], u4gpRegBackupVlaue[u4RegIdx]));
    }
}

void DramcRestoreRegisters(DRAMC_CTX_T *p, U32 *restore_addr, U32 restore_num)
{
    U32 u4RegIdx;

    for(u4RegIdx=0; u4RegIdx < restore_num; u4RegIdx++)
    {
        vIO32Write4B(restore_addr[u4RegIdx], u4gpRegBackupVlaue[u4RegIdx]);
        //mcSHOW_DBG_MSG(("Restore Reg(0x%X) = 0x%X\n", restore_addr[u4RegIdx], u4gpRegBackupVlaue[u4RegIdx]));
    }
}

#if defined(SLT)
void O1Path_Test(DRAMC_CTX_T * p)
{
    U32 channel_idx, backup_channel;
    U32 u4dq_o1, u4ca_o1, u4fail=0;
    
    mcSHOW_ERR_MSG(("[Write Leveling] O1 TEST\n"));
    backup_channel = vGetPHY2ChannelMapping(p);
    
    O1PathOnOff(p, 1);
    for(channel_idx=CHANNEL_A; channel_idx<p->support_channel_num; channel_idx++)
    {
        vSetPHY2ChannelMapping(p, channel_idx);
        U32 u4RegBackupAddress[] =
        {
            (DRAMC_REG_ADDR(DDRPHY_B0_DQ3)),
            (DRAMC_REG_ADDR(DDRPHY_B1_DQ3)),
            (DRAMC_REG_ADDR(DDRPHY_B0_DQ5)),
            (DRAMC_REG_ADDR(DDRPHY_B1_DQ5)),
            (DRAMC_REG_ADDR(DDRPHY_CA_CMD3)),
            (DRAMC_REG_ADDR(DDRPHY_CA_CMD5)),
            (DRAMC_REG_ADDR(DDRPHY_B0_DQ2)),
            (DRAMC_REG_ADDR(DDRPHY_B1_DQ2)),
            (DRAMC_REG_ADDR(DDRPHY_B0_DQ7)),
            (DRAMC_REG_ADDR(DDRPHY_B1_DQ7)),
            (DRAMC_REG_ADDR(DDRPHY_CA_CMD2)),
            (DRAMC_REG_ADDR(DDRPHY_CA_CMD7)),
        };
        DramcBackupRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));

        if (u1IsLP4Family(p->dram_type))
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_CMD3), 1, CA_CMD3_RG_RX_ARCMD_SMT_EN);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_CMD5), 1, CA_CMD5_RG_RX_ARCMD_EYE_VREF_EN);

            // DQ, CMD OE_DIS
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B0_DQ2), P_Fld(0x1, B0_DQ2_RG_TX_ARDQ_OE_DIS_B0)|P_Fld(0x1, B0_DQ2_RG_TX_ARDQM_OE_DIS));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B1_DQ2), P_Fld(0x1, B1_DQ2_RG_TX_ARDQ_OE_DIS_B1)|P_Fld(0x1, B1_DQ2_RG_TX_ARDQM_OE_DIS));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_CA_CMD2), P_Fld(0x1, CA_CMD2_RG_TX_ARCMD_OE_DIS)|P_Fld(0x1, CA_CMD2_RG_TX_ARCLK_OE_DIS));
        
            // pull down DQ, DQM, CMD, CS, CLK
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B0_DQ7), P_Fld(0x1, B0_DQ7_RG_TX_ARDQ_PULL_DN_B0)|P_Fld(0x1, B0_DQ7_RG_TX_ARDQM_PULL_DN_B0));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B1_DQ7), P_Fld(0x1, B1_DQ7_RG_TX_ARDQ_PULL_DN_B1)|P_Fld(0x1, B1_DQ7_RG_TX_ARDQM0_PULL_DN_B1));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_CA_CMD7), P_Fld(0x1, CA_CMD7_RG_TX_ARCMD_PULL_DN)|P_Fld(0x1, CA_CMD7_RG_TX_ARCS_PULL_DN)|P_Fld(0x1, CA_CMD7_RG_TX_ARCLK_PULL_DN));
        
            mcDELAY_US(1);

            u4dq_o1 = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_AD_RX_DQ_O1));
            u4ca_o1 = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_AD_RX_CMD_O1));
            if (u4dq_o1!=0 || u4ca_o1!=0)
                u4fail = 1;
            mcSHOW_ERR_MSG(("[Write Leveling] CH_%d PULL DN=1, DQ-DQM O1=0x%x (%s), CS-CKE-CA O1=0x%x (%s)\n",channel_idx,u4dq_o1,(u4dq_o1==0)?"pass":"fail",u4ca_o1,(u4ca_o1==0)?"pass":"fail"));


            // pull up DQ, DQM, CMD, CS, CLK
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B0_DQ7), P_Fld(0x0, B0_DQ7_RG_TX_ARDQ_PULL_DN_B0) | P_Fld(0x0, B0_DQ7_RG_TX_ARDQM_PULL_DN_B0) |
                                                  P_Fld(0x1, B0_DQ7_RG_TX_ARDQ_PULL_UP_B0) | P_Fld(0x1, B0_DQ7_RG_TX_ARDQM_PULL_UP_B0));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B1_DQ7), P_Fld(0x0, B1_DQ7_RG_TX_ARDQ_PULL_DN_B1)|P_Fld(0x0, B1_DQ7_RG_TX_ARDQM0_PULL_DN_B1)|
                                                  P_Fld(0x1, B1_DQ7_RG_TX_ARDQ_PULL_UP_B1)|P_Fld(0x1, B1_DQ7_RG_TX_ARDQM0_PULL_UP_B1));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_CA_CMD7),P_Fld(0x0, CA_CMD7_RG_TX_ARCMD_PULL_DN)|P_Fld(0x0, CA_CMD7_RG_TX_ARCS_PULL_DN)|P_Fld(0x0, CA_CMD7_RG_TX_ARCLK_PULL_DN)|
                                                  P_Fld(0x1, CA_CMD7_RG_TX_ARCMD_PULL_UP)|P_Fld(0x1, CA_CMD7_RG_TX_ARCS_PULL_UP)|P_Fld(0x1, CA_CMD7_RG_TX_ARCLK_PULL_UP));

            mcDELAY_US(1);

            u4dq_o1 = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_AD_RX_DQ_O1));
            u4ca_o1 = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_AD_RX_CMD_O1));
            if (u4dq_o1!=0x1FF01FF || u4ca_o1!=0x6C3F) //refer to RG coda field
                u4fail = 1;
            mcSHOW_ERR_MSG(("[Write Leveling] CH_%d PULL UP=1, DQ-DQM O1=0x%x (%s), CS-CKE-CA O1=0x%x (%s)\n",channel_idx,u4dq_o1,(u4dq_o1==0x1FF01FF)?"pass":"fail",u4ca_o1,(u4ca_o1==0x6C3F)?"pass":"fail"));
        }
        DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));
    }
    O1PathOnOff(p, 0);

    vSetPHY2ChannelMapping(p, backup_channel);

    if (u4fail)
        while(1);
        
}
#endif
