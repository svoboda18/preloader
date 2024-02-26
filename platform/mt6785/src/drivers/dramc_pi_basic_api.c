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
#include "dramc_actiming.h"
#if (FOR_DV_SIMULATION_USED==0)
#include "pll.h"
#endif

//#include "DramC_reg.h"
//#include "System_reg.h"
//#include "string.h"
//#define mcFPRINTF(_x_)            do{}while(0)

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
#if REG_ACCESS_PORTING_DGB
extern U8 RegLogEnable;
#endif
U8 u1PrintModeRegWrite=0;

#if FOR_DV_SIMULATION_USED
U8 u1BroadcastOnOff=0;
extern U8 gu1BroadcastIsLP4;
#endif

#if APOLLO_SPECIFIC
#if (CA_PER_BIT_DELAY_CELL || PINMUX_AUTO_TEST_PER_BIT_CA)
const U8 uiLPDDR4_CA_Mapping_POP[CHANNEL_NUM][6] =
{
    //CH-A
    {
        5, 3, 4, 0, 1, 2
    },
};
#endif

//MRR DRAM->DRAMC
const U8 uiLPDDR4_MRR_Mapping_POP[CHANNEL_NUM][16] =
{
    //CH-A
    {
        0, 1, 6, 7, 3, 5, 4, 2,
        8, 9, 13, 14, 12, 15, 10, 11
    },
};

//O1 DRAM->DRAMC
const U8 uiLPDDR4_O1_Mapping_POP[CHANNEL_NUM][16] =
{
    //CH-A
    {
        0, 1, 6, 7, 3, 5, 4, 2,
        8, 9, 13, 14, 12, 15, 10, 11
    },
};
#else //Lafi_te
//CA APHY->DRAM
#if (CA_PER_BIT_DELAY_CELL || PINMUX_AUTO_TEST_PER_BIT_CA)
const U8 uiLPDDR4_CA_Mapping_POP[CHANNEL_NUM][6] =
{
    //CH-A
    {
        3, 0, 4, 2, 1, 5
    },
    //CH-B
    {
        0, 4, 2, 3, 1, 5
    }
};
#endif

//MRR DRAM->DRAMC
const U8 uiLPDDR4_MRR_Mapping_POP[CHANNEL_NUM][16] =
{
    //CH-A
    {
        0, 1, 3, 5, 4, 7, 6, 2,
        9, 8, 12, 10, 11, 15, 13, 14
    },
    //CH-B
    {
        0, 1, 4, 6, 3, 7, 5, 2,
        9, 8, 10, 15, 11, 13, 14, 12
    }
};

//O1 DRAM->DRAMC
const U8 uiLPDDR4_O1_Mapping_POP[CHANNEL_NUM][16] =
{
    //CH-A
    {
        0, 1, 3, 5, 4, 7, 6, 2,
        9, 8, 12, 10, 11, 15, 13, 14
    },
    //CH-B
    {
        0, 1, 4, 6, 3, 7, 5, 2,
        9, 8, 10, 15, 11, 13, 14, 12
    }
};
#if 0//(FOR_DV_SIMULATION_USED!=0)
const U8 uiLPDDR4_O1_Mapping_POP[CHANNEL_NUM][16] =
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

#if (TDQSCK_PRECALCULATION_FOR_DVFS || defined(HW_GATING))
U8 gIsddr800DisGating = 0;
#endif

static void TxCalibrationMoveDQSByDifferentWL(DRAMC_CTX_T *p);


#ifdef FOR_HQA_REPORT_USED
extern U8 gHQALog_flag;
#endif







void Set_MRR_Pinmux_Mapping(DRAMC_CTX_T *p)
{
    U8 *uiLPDDR_MRR_Mapping = NULL;
    U8 backup_broadcast, backup_channel;
    DRAM_CHANNEL_T chIdx = CHANNEL_A;

    //Backup channel & broadcast
    backup_channel = vGetPHY2ChannelMapping(p);
    backup_broadcast = GetDramcBroadcast();

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF); //Disable broadcast

    //LP4: Set each channel's pinmux individually, LP3: Only has 1 channel (support_channel_num == 1)
    for(chIdx = CHANNEL_A; chIdx < p->support_channel_num; chIdx++)
    {
        vSetPHY2ChannelMapping(p, chIdx);

        uiLPDDR_MRR_Mapping = (U8 *)uiLPDDR4_MRR_Mapping_POP[chIdx];

        //Set MRR pin mux
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_MRR_BIT_MUX1), P_Fld(uiLPDDR_MRR_Mapping[0], MRR_BIT_MUX1_MRR_BIT0_SEL) | P_Fld(uiLPDDR_MRR_Mapping[1], MRR_BIT_MUX1_MRR_BIT1_SEL) |
                                                                   P_Fld(uiLPDDR_MRR_Mapping[2], MRR_BIT_MUX1_MRR_BIT2_SEL) | P_Fld(uiLPDDR_MRR_Mapping[3], MRR_BIT_MUX1_MRR_BIT3_SEL));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_MRR_BIT_MUX2), P_Fld(uiLPDDR_MRR_Mapping[4], MRR_BIT_MUX2_MRR_BIT4_SEL) | P_Fld(uiLPDDR_MRR_Mapping[5], MRR_BIT_MUX2_MRR_BIT5_SEL) |
                                                                   P_Fld(uiLPDDR_MRR_Mapping[6], MRR_BIT_MUX2_MRR_BIT6_SEL) | P_Fld(uiLPDDR_MRR_Mapping[7], MRR_BIT_MUX2_MRR_BIT7_SEL));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_MRR_BIT_MUX3), P_Fld(uiLPDDR_MRR_Mapping[8], MRR_BIT_MUX3_MRR_BIT8_SEL) | P_Fld(uiLPDDR_MRR_Mapping[9], MRR_BIT_MUX3_MRR_BIT9_SEL) |
                                                                   P_Fld(uiLPDDR_MRR_Mapping[10], MRR_BIT_MUX3_MRR_BIT10_SEL) | P_Fld(uiLPDDR_MRR_Mapping[11], MRR_BIT_MUX3_MRR_BIT11_SEL));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_MRR_BIT_MUX4), P_Fld(uiLPDDR_MRR_Mapping[12], MRR_BIT_MUX4_MRR_BIT12_SEL) | P_Fld(uiLPDDR_MRR_Mapping[13], MRR_BIT_MUX4_MRR_BIT13_SEL) |
                                                                   P_Fld(uiLPDDR_MRR_Mapping[14], MRR_BIT_MUX4_MRR_BIT14_SEL) | P_Fld(uiLPDDR_MRR_Mapping[15], MRR_BIT_MUX4_MRR_BIT15_SEL));
    }

    //Recover channel & broadcast
    vSetPHY2ChannelMapping(p, backup_channel);
    DramcBroadcastOnOff(backup_broadcast);
}

static void SetRankInfoToConf(DRAMC_CTX_T *p)
{
#if(FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
    EMI_SETTINGS *emi_set;
    U32 u4value = 0;
#if (!__ETT__)//preloader
    if(emi_setting_index == -1)
        emi_set = &default_emi_setting;
    else
        emi_set = &emi_settings[emi_setting_index];
#else//ett
    emi_set = &default_emi_setting;
#endif

    u4value = ((emi_set->EMI_CONA_VAL & (0x1 << 17)) >> 17) ? 0 : 1;//CONA 17th bit 0: Disable dual rank mode 1: Enable dual rank mode
    vIO32WriteFldAlign(DRAMC_REG_RSTMASK, u4value, RSTMASK_RSV_DRAM_SUPPORT_RANK_NUM);//0: dual rank 1: single rank
    mcSHOW_JV_LOG_MSG(("Rank info: %d emi_setting_index: %d CONA[0x%x]\n", u4value, emi_setting_index, emi_set->EMI_CONA_VAL));
#endif
    return;
}

void UpdateDFSTbltoDDR4266(DRAMC_CTX_T *p)
{
#if(FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
    EMI_SETTINGS *emi_set;
    U16 u2HighestFreq = u2DFSGetHighestFreq(p);
    DRAM_PLL_FREQ_SEL_T highestfreqsel = 0;
    U8 u1ShuffleIdx = 0;

#if (!__ETT__)//preloader
    if(emi_setting_index == -1)
        emi_set = &default_emi_setting;
    else
        emi_set = &emi_settings[emi_setting_index];
#else//ett
    emi_set = &default_emi_setting;
#endif

    highestfreqsel = GetSelByFreq(p, u2HighestFreq);
    for (u1ShuffleIdx = DRAM_DFS_SHUFFLE_1; u1ShuffleIdx < DRAM_DFS_SHUFFLE_MAX; u1ShuffleIdx++)
        if (gFreqTbl[u1ShuffleIdx].freq_sel == highestfreqsel)
            break;

    if (emi_set->DRAMC_ACTIME_UNION[0] == 4266) // @Darren for shuffle calib table
    {
        gFreqTbl[u1ShuffleIdx].freq_sel = LP4_DDR4266;
    }
    else // others to DDR3733
    {
        gFreqTbl[u1ShuffleIdx].freq_sel = LP4_DDR3733;
    }

    gUpdateHighestFreq = TRUE;
    u2HighestFreq = u2DFSGetHighestFreq(p); // @Darren, Update u2FreqMax variables
    #if __ETT__
    UpdateEttDFVSTbltoDDR4266(p, u2HighestFreq); //@Darren, Update for DDR4266 ETT DVFS stress
    #endif
    mcSHOW_DBG_MSG(("[UpdateDFSTbltoDDR4266] Get Highest Freq is %d\n", u2HighestFreq));
#endif
}

void Global_Option_Init(DRAMC_CTX_T *p)
{
    U8 u1ShuffleIdx;

    //SaveCurrDramCtx(p);
    vSetChannelNumber(p);
    SetRankInfoToConf(p);
    vSetRankNumber(p);
    Set_MRR_Pinmux_Mapping(p);
    vInitGlobalVariablesByCondition();
    vInitMappingFreqArray(p);
    DramcDQSOSCInit();
    
#ifdef FOR_HQA_TEST_USED
    HQA_measure_message_reset_all_data(p);
#endif
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
#if (fcFOR_CHIP_ID == fcKrug)
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), u1OnOff, B0_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), u1OnOff, B1_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B1);
#else
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ8), u1OnOff, B0_DQ8_RG_RX_ARDQS_DQSSTB_CG_EN_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ8), u1OnOff, B1_DQ8_RG_RX_ARDQS_DQSSTB_CG_EN_B1);
#endif

    return;
}

void OpenLoopModeSetting(DRAMC_CTX_T * p, DDR800_MODE_T eDDR800Mode)
{
    //Enable SHU option for MCK8X_EN and PHDET_EN
    if (eDDR800Mode == DDR800_OPEN_LOOP)
    {
        vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL6, P_Fld(0x1, MISC_CG_CTRL6_RG_M_CK_OPENLOOP_MODE_EN) 
                    | P_Fld(0x1, MISC_CG_CTRL6_RG_MCK4X_I_OPENLOOP_MODE_EN) 
                    | P_Fld(0x1, MISC_CG_CTRL6_RG_CG_DDR400_MCK4X_I_OFF) 
                    | P_Fld(0x0, MISC_CG_CTRL6_RG_DDR400_MCK4X_I_FORCE_ON) 
                    | P_Fld(0x1, MISC_CG_CTRL6_RG_MCK4X_I_FB_CK_CG_OFF) 
                    | P_Fld(0x1, MISC_CG_CTRL6_RG_MCK4X_Q_OPENLOOP_MODE_EN) 
                    | P_Fld(0x1, MISC_CG_CTRL6_RG_CG_DDR400_MCK4X_Q_OFF)
                    | P_Fld(0x0, MISC_CG_CTRL6_RG_DDR400_MCK4X_Q_FORCE_ON)
                    | P_Fld(0x1, MISC_CG_CTRL6_RG_MCK4X_Q_FB_CK_CG_OFF)
                    | P_Fld(0x1, MISC_CG_CTRL6_RG_MCK4X_O_OPENLOOP_MODE_EN)
                    | P_Fld(0x1, MISC_CG_CTRL6_RG_CG_DDR400_MCK4X_O_OFF)
                    | P_Fld(0x0, MISC_CG_CTRL6_RG_DDR400_MCK4X_O_FORCE_ON)
                    | P_Fld(0x1, MISC_CG_CTRL6_RG_MCK4X_O_FB_CK_CG_OFF));
    
        vIO32WriteFldAlign_All(DDRPHY_SHU_MISC2, 1, SHU_MISC2_M_CK_OPENLOOP_MODE_SEL);
    }
}

/* DDR800 mode struct declaration (declared here due Fld_wid for each register type) */
typedef struct _DDR800Mode_T
{
    U8 dll_phdet_en_b0 :Fld_wid(SHU_B0_DLL0_RG_ARDLL_PHDET_EN_B0_SHU);
    U8 dll_phdet_en_b1 :Fld_wid(SHU_B1_DLL0_RG_ARDLL_PHDET_EN_B1_SHU);
    U8 dll_phdet_en_ca_cha :Fld_wid(SHU_CA_DLL0_RG_ARDLL_PHDET_EN_CA_SHU);
    U8 dll_phdet_en_ca_chb :Fld_wid(SHU_CA_DLL0_RG_ARDLL_PHDET_EN_CA_SHU);
    U8 phypll_ada_mck8x_en :Fld_wid(SHU_PLL22_RG_RPHYPLL_ADA_MCK8X_EN_SHU);
    U8 ddr400_en_b0 :Fld_wid(SHU_B0_DQ6_RG_ARPI_DDR400_EN_B0);
    U8 ddr400_en_b1 :Fld_wid(SHU_B1_DQ6_RG_ARPI_DDR400_EN_B1);
    U8 ddr400_en_ca :Fld_wid(SHU_CA_CMD6_RG_ARPI_DDR400_EN_CA);
    U8 phypll_ddr400_en :Fld_wid(SHU_PLL1_RG_RPHYPLL_DDR400_EN);
    U8 ddr400_dqs_ps_b0 :Fld_wid(SHU_B0_DQ9_RG_DDR400_DQS_PS_B0);
    U8 ddr400_dqs_ps_b1 :Fld_wid(SHU_B1_DQ9_RG_DDR400_DQS_PS_B1);
    U8 ddr400_dq_ps_b0 :Fld_wid(SHU_B0_DQ9_RG_DDR400_DQ_PS_B0);
    U8 ddr400_dq_ps_b1 :Fld_wid(SHU_B1_DQ9_RG_DDR400_DQ_PS_B1);
    U8 ddr400_dqs_ps_ca :Fld_wid(SHU_CA_CMD9_RG_DDR400_DQS_PS_CA);
    U8 ddr400_dq_ps_ca :Fld_wid(SHU_CA_CMD9_RG_DDR400_DQ_PS_CA);
    U8 ddr400_semi_en_b0 :Fld_wid(SHU_B0_DQ9_RG_DDR400_SEMI_EN_B0);
    U8 ddr400_semi_en_b1 :Fld_wid(SHU_B1_DQ9_RG_DDR400_SEMI_EN_B1);
    U8 ddr400_semi_en_ca :Fld_wid(SHU_CA_CMD9_RG_DDR400_SEMI_EN_CA);
    U8 ddr400_semi_open_en :Fld_wid(SHU_PLL0_RG_DDR400_SEMI_OPEN_EN);
    U8 pll0_ada_mck8x_chb_en :Fld_wid(SHU_PLL0_ADA_MCK8X_CHB_EN);
    U8 pll0_ada_mck8x_cha_en :Fld_wid(SHU_PLL0_ADA_MCK8X_CHA_EN);
} DDR800Mode_T;

void APhyModeSetting(DRAMC_CTX_T * p, DDR800_MODE_T eMode, U8 *uDLL1, U8 *uMode, U8 *uDLL0)
{
    DDR800Mode_T DDR800Mode;

    OpenLoopModeSetting(p, eMode);

    if ((eMode == DDR800_CLOSE_LOOP) || (eMode == NORMAL_CLOSE_LOOP))
    {
        DDR800Mode.phypll_ddr400_en = 0;
        DDR800Mode.ddr400_en_b0 = 0;
        DDR800Mode.ddr400_en_b1 = 0;
        DDR800Mode.ddr400_en_ca = 0;
        DDR800Mode.dll_phdet_en_b0 = 1;
        DDR800Mode.dll_phdet_en_b1 = 1;
        DDR800Mode.dll_phdet_en_ca_cha = 1;
        DDR800Mode.dll_phdet_en_ca_chb = 1;
        DDR800Mode.phypll_ada_mck8x_en = 1;
        DDR800Mode.ddr400_dqs_ps_b0 = 0;
        DDR800Mode.ddr400_dqs_ps_b1 = 0;
        DDR800Mode.ddr400_dq_ps_b0 = 0;
        DDR800Mode.ddr400_dq_ps_b1 = 0;
        DDR800Mode.ddr400_dqs_ps_ca = 0;
        DDR800Mode.ddr400_dq_ps_ca = 0;  
        DDR800Mode.ddr400_semi_en_b0 = 0;
        DDR800Mode.ddr400_semi_en_b1 = 0;
        DDR800Mode.ddr400_semi_en_ca = 0;     
        DDR800Mode.ddr400_semi_open_en = 0;
        DDR800Mode.pll0_ada_mck8x_chb_en = 1;
        DDR800Mode.pll0_ada_mck8x_cha_en = 1;

        if (p->frequency <= 400) //DDR800 1:4 mode
        {
            *uDLL1= 1;
            *uMode = 0;
            *uDLL0= 2;
        }
        else if (p->frequency <= 800)
        {
            if (vGet_Div_Mode(p) == DIV8_MODE) // DDR1600 1:8 mode
            {
                *uDLL1 = 0;
                *uMode = 0;
                *uDLL0 = 2;
            }
            else // DDR1600 1:4 mode
            {
                *uDLL1 = 0;
                *uMode = 1;
                *uDLL0 = 0;
            }
        }
        else // 1:8 mode
        {
            *uDLL1 = 0;
            *uMode = 0;
            *uDLL0 = 2;
        }
    }
    else if (eMode == DDR800_OPEN_LOOP) // For DDR800
    {
        DDR800Mode.phypll_ddr400_en = 1;
        DDR800Mode.ddr400_en_b0 = 1;
        DDR800Mode.ddr400_en_b1 = 1;
        DDR800Mode.ddr400_en_ca = 1;
        DDR800Mode.dll_phdet_en_b0 = 0;
        DDR800Mode.dll_phdet_en_b1 = 0;
        DDR800Mode.dll_phdet_en_ca_cha = 0;
        DDR800Mode.dll_phdet_en_ca_chb = 0;
        DDR800Mode.phypll_ada_mck8x_en = 0;
#if(FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0) 
        DDR800Mode.ddr400_dqs_ps_b0 = 0;  // 2'b00-0 degree, 2'b01-45 degree, 2'b10-90 degree
        DDR800Mode.ddr400_dqs_ps_b1 = 0;
        DDR800Mode.ddr400_dq_ps_b0 = 0;
        DDR800Mode.ddr400_dq_ps_b1 = 0;
        DDR800Mode.ddr400_dqs_ps_ca = 0; // clk
        DDR800Mode.ddr400_dq_ps_ca = 0;  // ca
#else
        DDR800Mode.ddr400_dqs_ps_b0 = 1;
        DDR800Mode.ddr400_dqs_ps_b1 = 1;
        DDR800Mode.ddr400_dq_ps_b0 = 0;
        DDR800Mode.ddr400_dq_ps_b1 = 0;
        DDR800Mode.ddr400_dqs_ps_ca = 1;
        DDR800Mode.ddr400_dq_ps_ca = 2;  
#endif
        DDR800Mode.ddr400_semi_en_b0 = 0;
        DDR800Mode.ddr400_semi_en_b1 = 0;
        DDR800Mode.ddr400_semi_en_ca = 0;     
        DDR800Mode.ddr400_semi_open_en = 0;
        DDR800Mode.pll0_ada_mck8x_chb_en = 0;
        DDR800Mode.pll0_ada_mck8x_cha_en = 0;
        *uDLL1= 0;
        *uMode = 1;
        *uDLL0= 0;
    }
    else if (eMode == DDR800_SEMI_LOOP) // For DDR800
    {
        DDR800Mode.phypll_ddr400_en = 1;
        DDR800Mode.ddr400_en_b0 = 1;
        DDR800Mode.ddr400_en_b1 = 1;
        DDR800Mode.ddr400_en_ca = 1;
        DDR800Mode.dll_phdet_en_b0 = 0;
        DDR800Mode.dll_phdet_en_b1 = 0;
        DDR800Mode.dll_phdet_en_ca_cha = 1;
        DDR800Mode.dll_phdet_en_ca_chb = 0;
        DDR800Mode.phypll_ada_mck8x_en = 1;
#if(FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0) 
        DDR800Mode.ddr400_dqs_ps_b0 = 0;  // 2'b00-0 degree, 2'b01-45 degree, 2'b10-90 degree
        DDR800Mode.ddr400_dqs_ps_b1 = 0;
        DDR800Mode.ddr400_dq_ps_b0 = 0;
        DDR800Mode.ddr400_dq_ps_b1 = 0;
        DDR800Mode.ddr400_dqs_ps_ca = 0; // clk
        DDR800Mode.ddr400_dq_ps_ca = 0;  // ca
#else
        DDR800Mode.ddr400_dqs_ps_b0 = 1;
        DDR800Mode.ddr400_dqs_ps_b1 = 1;
        DDR800Mode.ddr400_dq_ps_b0 = 0;
        DDR800Mode.ddr400_dq_ps_b1 = 0;
        DDR800Mode.ddr400_dqs_ps_ca = 0; //@Darren, CLK confirm from YY Hsu
        DDR800Mode.ddr400_dq_ps_ca = 0;  //@Darren, CA confirm from YY Hsu 
#endif
        DDR800Mode.ddr400_semi_en_b0 = 1;
        DDR800Mode.ddr400_semi_en_b1 = 1;
        DDR800Mode.ddr400_semi_en_ca = 1;     
        DDR800Mode.ddr400_semi_open_en = 1;
        DDR800Mode.pll0_ada_mck8x_chb_en = 0;
        DDR800Mode.pll0_ada_mck8x_cha_en = 1;
        *uDLL1= 0;
        *uMode = 1;
        *uDLL0= 2;
    }
    else // Others
    {
        mcSHOW_ERR_MSG(("[FAIL] APhy mode incorrect !!!\n"));
        #if __ETT__
        while(1);
        #endif
    }

    // Enable DDR800 RG
    vIO32WriteFldAlign(DDRPHY_SHU_PLL1, DDR800Mode.phypll_ddr400_en, SHU_PLL1_RG_RPHYPLL_DDR400_EN); // CHA only
    //vIO32WriteFldAlign_All(DDRPHY_PLL4, DDR800Mode.phypll_ada_mck8x_en, PLL4_RG_RPHYPLL_ADA_MCK8X_EN);
    vIO32WriteFldAlign_All(DDRPHY_SHU_B0_DQ6, DDR800Mode.ddr400_en_b0, SHU_B0_DQ6_RG_ARPI_DDR400_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_SHU_B1_DQ6, DDR800Mode.ddr400_en_b1, SHU_B1_DQ6_RG_ARPI_DDR400_EN_B1);
    vIO32WriteFldAlign_All(DDRPHY_SHU_CA_CMD6, DDR800Mode.ddr400_en_ca, SHU_CA_CMD6_RG_ARPI_DDR400_EN_CA);

    // DLL & Clock
    vIO32WriteFldAlign_All(DDRPHY_SHU_B0_DLL0, DDR800Mode.dll_phdet_en_b0, SHU_B0_DLL0_RG_ARDLL_PHDET_EN_B0_SHU);
    vIO32WriteFldAlign_All(DDRPHY_SHU_B1_DLL0, DDR800Mode.dll_phdet_en_b1, SHU_B1_DLL0_RG_ARDLL_PHDET_EN_B1_SHU);
    vIO32WriteFldAlign(DDRPHY_SHU_CA_DLL0, DDR800Mode.dll_phdet_en_ca_cha, SHU_CA_DLL0_RG_ARDLL_PHDET_EN_CA_SHU);
    vIO32WriteFldAlign(DDRPHY_SHU_CA_DLL0+SHIFT_TO_CHB_ADDR, DDR800Mode.dll_phdet_en_ca_chb, SHU_CA_DLL0_RG_ARDLL_PHDET_EN_CA_SHU);
    vIO32WriteFldAlign(DDRPHY_SHU_PLL22, DDR800Mode.phypll_ada_mck8x_en, SHU_PLL22_RG_RPHYPLL_ADA_MCK8X_EN_SHU);
    vIO32WriteFldAlign_All(DDRPHY_SHU_PLL0, DDR800Mode.pll0_ada_mck8x_chb_en, SHU_PLL0_ADA_MCK8X_CHB_EN);
    vIO32WriteFldAlign_All(DDRPHY_SHU_PLL0, DDR800Mode.pll0_ada_mck8x_cha_en, SHU_PLL0_ADA_MCK8X_CHA_EN);

    // CA/DQS/DQ position
    vIO32WriteFldAlign_All(DDRPHY_SHU_B0_DQ9, DDR800Mode.ddr400_dqs_ps_b0, SHU_B0_DQ9_RG_DDR400_DQS_PS_B0);
    vIO32WriteFldAlign_All(DDRPHY_SHU_B1_DQ9, DDR800Mode.ddr400_dqs_ps_b1, SHU_B1_DQ9_RG_DDR400_DQS_PS_B1);
    vIO32WriteFldAlign_All(DDRPHY_SHU_B0_DQ9, DDR800Mode.ddr400_dq_ps_b0, SHU_B0_DQ9_RG_DDR400_DQ_PS_B0);
    vIO32WriteFldAlign_All(DDRPHY_SHU_B1_DQ9, DDR800Mode.ddr400_dq_ps_b1, SHU_B1_DQ9_RG_DDR400_DQ_PS_B1);
    vIO32WriteFldAlign_All(DDRPHY_SHU_CA_CMD9, DDR800Mode.ddr400_dqs_ps_ca, SHU_CA_CMD9_RG_DDR400_DQS_PS_CA);
    vIO32WriteFldAlign_All(DDRPHY_SHU_CA_CMD9, DDR800Mode.ddr400_dq_ps_ca, SHU_CA_CMD9_RG_DDR400_DQ_PS_CA);

    // Semi Open Enable
    vIO32WriteFldAlign_All(DDRPHY_SHU_B0_DQ9, DDR800Mode.ddr400_semi_en_b0, SHU_B0_DQ9_RG_DDR400_SEMI_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_SHU_B1_DQ9, DDR800Mode.ddr400_semi_en_b1, SHU_B1_DQ9_RG_DDR400_SEMI_EN_B1);
    vIO32WriteFldAlign_All(DDRPHY_SHU_CA_CMD9, DDR800Mode.ddr400_semi_en_ca, SHU_CA_CMD9_RG_DDR400_SEMI_EN_CA);
    vIO32WriteFldAlign_All(DDRPHY_SHU_PLL0, DDR800Mode.ddr400_semi_open_en, SHU_PLL0_RG_DDR400_SEMI_OPEN_EN);
}

void DDRDllModeSetting(DRAMC_CTX_T * p)
{
    U8 uDLL1 = 0, uMode = 0, uDLL0 = 2; // 1:8 mode for bring-up

#if ENABLE_REMOVE_MCK8X_UNCERT_DFS_OPTION
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI0, 1, B0_DLL_ARPI0_RG_ARPI_MCK8X_SEL_B0); // @YY
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI0, 1, B1_DLL_ARPI0_RG_ARPI_MCK8X_SEL_B1); // @YY
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI0, 1, CA_DLL_ARPI0_RG_ARPI_MCK8X_SEL_CA); // @YY
#endif

    APhyModeSetting(p, vGet_DDR800_Mode(p), &uDLL1, &uMode, &uDLL0);

    vIO32WriteFldAlign_All(DDRPHY_SHU_B0_DLL1, uDLL1, SHU_B0_DLL1_FRATE_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_SHU_B1_DLL1, uDLL1, SHU_B1_DLL1_FRATE_EN_B1);
    vIO32WriteFldAlign_All(DDRPHY_SHU_CA_DLL2, uDLL1, SHU_CA_DLL2_FRATE_EN);

    vIO32WriteFldAlign_All(DDRPHY_SHU_B0_DQ6, uMode, SHU_B0_DQ6_RG_TX_ARDQ_SER_MODE_B0);
    vIO32WriteFldAlign_All(DDRPHY_SHU_B1_DQ6, uMode, SHU_B1_DQ6_RG_TX_ARDQ_SER_MODE_B1);
    vIO32WriteFldAlign_All(DDRPHY_SHU_CA_CMD6, uMode, SHU_CA_CMD6_RG_TX_ARCMD_SER_MODE);
    vIO32WriteFldAlign_All(DDRPHY_SHU_B0_DQ6, uMode, SHU_B0_DQ6_RG_RX_ARDQ_RANK_SEL_SER_MODE_B0);
    vIO32WriteFldAlign_All(DDRPHY_SHU_B1_DQ6, uMode, SHU_B1_DQ6_RG_RX_ARDQ_RANK_SEL_SER_MODE_B1);
    vIO32WriteFldAlign_All(DDRPHY_SHU_CA_CMD6, uMode, SHU_CA_CMD6_RG_RX_ARCMD_RANK_SEL_SER_MODE);

    vIO32WriteFldAlign_All(DDRPHY_SHU_B0_DLL0, uDLL0, SHU_B0_DLL0_RG_ARDLL_DIV_MCTL_B0);
    vIO32WriteFldAlign_All(DDRPHY_SHU_B1_DLL0, uDLL0, SHU_B1_DLL0_RG_ARDLL_DIV_MCTL_B1);   
    vIO32WriteFldAlign_All(DDRPHY_SHU_CA_DLL0, uDLL0, SHU_CA_DLL0_RG_ARDLL_DIV_MCTL_CA);
}


void DDRPhyPLLSetting(DRAMC_CTX_T *p)
{
    U8 u1CAP_SEL;
    U8 u1MIDPICAP_SEL;
    U8 u1VTH_SEL;
    U16 u2SDM_PCW = 0, u2PLLRsv = 0;
    U8 u1CA_DLL_Mode[2];
    U8 iChannel=CHANNEL_A;
    U8 u1BRPI_MCTL_EN_CA = 0;
    U8 u1PCW_CHG = 0;
#if ENABLE_TMRRI_NEW_MODE
    U8 u1RankIdx;
#endif
    U8 u1Gain_Ca_ChA = 0, u1Gain_Ca_ChB = 0;
    U8 u1CurrShuLevel = 0;
#if ENABLE_APHY_DLL_IDLE_MODE_OPTION //Should before the PHDET_EN = 1
    U8 u1PdZone = (u2DFSGetHighestFreq(p) == 2133) ? 0x2 : 0x3;
#endif

    u1VTH_SEL = 0x2; /* RG_*RPI_MIDPI_VTH_SEL[1:0] is 2 for all freqs */

#if (fcFOR_CHIP_ID == fcKrug)
    if (p->frequency <= 400)
    {
        u1CAP_SEL = 0xb;
    }
    else if (p->frequency <= 600)
    {
        u1CAP_SEL = 0xf;
    }
    else if (p->frequency <= 800)
    {
        u1CAP_SEL = 0xb;
    }
    else if (p->frequency <= 1200)
    {
        u1CAP_SEL = 0x8;
    }
    else if (p->frequency <= 1333)
    {
        u1CAP_SEL = 0x8;
    }
    else if (p->frequency <= 1600)
    {
        u1CAP_SEL = 0x4;
    }
    else if (p->frequency <= 1866)
    {
        u1CAP_SEL = 0x2;
    }
    else//4266
    {
        u1CAP_SEL = 0x1;
    }

    if (p->frequency <= 933) //Lewis@20161129: Fix DDR1600 S-idle Vcore power is too big problem.
    {
        u1MIDPICAP_SEL = 0x2;
    }
    else if (p->frequency <= 1200)
    {
        u1MIDPICAP_SEL = 0x3;
    }
    else if (p->frequency <= 1333)
    {
        u1MIDPICAP_SEL = 0x3;
    }
    else if (p->frequency <= 1600)
    {
        u1MIDPICAP_SEL = 0x2;
    }
    else if (p->frequency <= 1866)
    {
        u1MIDPICAP_SEL = 0x1;
    }
    else//4266
    {
        u1MIDPICAP_SEL = 0x0;
    }
#if EMI_LPBK_USE_DDR_800
    if (p->frequency==800)
    {
        u1CAP_SEL = 0xf;    //Ying-Yu suggest setting for FT pattern
    }
#endif
#endif

    if(p->frequency==2133)
        u2PLLRsv = 0x2000;
    else
        u2PLLRsv = 0x0;
    vIO32WriteFldAlign_All(DDRPHY_SHU_PLL4, u2PLLRsv, SHU_PLL4_RG_RPHYPLL_RESERVED);
    vIO32WriteFldAlign_All(DDRPHY_SHU_PLL6, u2PLLRsv, SHU_PLL6_RG_RCLRPLL_RESERVED);

#if (fcFOR_CHIP_ID == fcKrug)
    #if DLL_ASYNC_MODE
    u1BRPI_MCTL_EN_CA=1;
    u1CA_DLL_Mode[CHANNEL_A] = u1CA_DLL_Mode[CHANNEL_B] = DLL_MASTER;
    vIO32WriteFldAlign(DDRPHY_MISC_SHU_OPT+((U32)CHANNEL_A<<POS_BANK_NUM), 1, MISC_SHU_OPT_R_CA_SHU_PHDET_SPM_EN);
    vIO32WriteFldAlign(DDRPHY_MISC_SHU_OPT+SHIFT_TO_CHB_ADDR, 1, MISC_SHU_OPT_R_CA_SHU_PHDET_SPM_EN);
    vIO32WriteFldMulti(DDRPHY_CKMUX_SEL+((U32)CHANNEL_A<<POS_BANK_NUM), P_Fld(0, CKMUX_SEL_FMEM_CK_MUX) | P_Fld(0, CKMUX_SEL_FB_CK_MUX));
    vIO32WriteFldMulti(DDRPHY_CKMUX_SEL+SHIFT_TO_CHB_ADDR, P_Fld(2, CKMUX_SEL_FMEM_CK_MUX) | P_Fld(2, CKMUX_SEL_FB_CK_MUX));
    #else
    u1CA_DLL_Mode[CHANNEL_A] = DLL_MASTER;
    u1CA_DLL_Mode[CHANNEL_B] = DLL_SLAVE;
    vIO32WriteFldAlign(DDRPHY_MISC_SHU_OPT+((U32)CHANNEL_A<<POS_BANK_NUM), 1, MISC_SHU_OPT_R_CA_SHU_PHDET_SPM_EN);
    vIO32WriteFldAlign(DDRPHY_MISC_SHU_OPT+SHIFT_TO_CHB_ADDR, 2, MISC_SHU_OPT_R_CA_SHU_PHDET_SPM_EN);
    vIO32WriteFldMulti(DDRPHY_CKMUX_SEL+((U32)CHANNEL_A<<POS_BANK_NUM), P_Fld(1, CKMUX_SEL_FMEM_CK_MUX) | P_Fld(1, CKMUX_SEL_FB_CK_MUX));
    vIO32WriteFldMulti(DDRPHY_CKMUX_SEL+SHIFT_TO_CHB_ADDR, P_Fld(1, CKMUX_SEL_FMEM_CK_MUX) | P_Fld(1, CKMUX_SEL_FB_CK_MUX));
    #endif
    
    #if ENABLE_DLL_ALL_SLAVE_MODE
    if ((p->frequency <= 933) && (vGet_DDR800_Mode(p) != DDR800_SEMI_LOOP))
    {
        u1CA_DLL_Mode[CHANNEL_A] = u1CA_DLL_Mode[CHANNEL_B] = DLL_SLAVE;
    }
    #endif

    if(u1CA_DLL_Mode[CHANNEL_A]==DLL_SLAVE)//All slave mode
    {
        vIO32WriteFldAlign_All(DRAMC_REG_SHU_DVFSCTL, 1, SHU_DVFSCTL_R_BYPASS_1ST_DLL);
    }
    else
    {
        vIO32WriteFldAlign_All(DRAMC_REG_SHU_DVFSCTL, 0, SHU_DVFSCTL_R_BYPASS_1ST_DLL);
    }

    for(iChannel=CHANNEL_A; iChannel<=CHANNEL_B; iChannel++)
    {
        if(u1CA_DLL_Mode[iChannel]==DLL_MASTER)
        {
            vIO32WriteFldMulti(DDRPHY_SHU_CA_DLL0+((U32)iChannel<<POS_BANK_NUM), P_Fld(0x0, SHU_CA_DLL0_RG_ARDLL_PHDET_OUT_SEL_CA) 
                        | P_Fld(0x0, SHU_CA_DLL0_RG_ARDLL_PHDET_IN_SWAP_CA) 
                        | P_Fld(0x6, SHU_CA_DLL0_RG_ARDLL_GAIN_CA) 
                        | P_Fld(0x9, SHU_CA_DLL0_RG_ARDLL_IDLECNT_CA) 
                        | P_Fld(0x8, SHU_CA_DLL0_RG_ARDLL_P_GAIN_CA) 
                        | P_Fld(0x1, SHU_CA_DLL0_RG_ARDLL_PHJUMP_EN_CA) 
                        | P_Fld(0x1, SHU_CA_DLL0_RG_ARDLL_PHDIV_CA)
                        | P_Fld(0x1, SHU_CA_DLL0_RG_ARDLL_FAST_PSJP_CA));
            vIO32WriteFldMulti(DDRPHY_SHU_CA_DLL1+((U32)iChannel<<POS_BANK_NUM), P_Fld(0x1, SHU_CA_DLL1_RG_ARDLL_PD_CK_SEL_CA) | P_Fld(0x0, SHU_CA_DLL1_RG_ARDLL_FASTPJ_CK_SEL_CA));    
            vIO32WriteFldAlign(DDRPHY_SHU_CA_CMD9+((U32)iChannel<<POS_BANK_NUM), 1, SHU_CA_CMD9_RG_DLL_FAST_PSJP_CA); // RG_*RPI_RESERVE_CA[1] 1'b1 tracking leaf(slave)
        }
        else
        {
            vIO32WriteFldMulti(DDRPHY_SHU_CA_DLL0+((U32)iChannel<<POS_BANK_NUM), P_Fld(0x1, SHU_CA_DLL0_RG_ARDLL_PHDET_OUT_SEL_CA) 
                        | P_Fld(0x1, SHU_CA_DLL0_RG_ARDLL_PHDET_IN_SWAP_CA) 
                        | P_Fld(0x7, SHU_CA_DLL0_RG_ARDLL_GAIN_CA) 
                        | P_Fld(0x7, SHU_CA_DLL0_RG_ARDLL_IDLECNT_CA) 
                        | P_Fld(0x8, SHU_CA_DLL0_RG_ARDLL_P_GAIN_CA) 
                        | P_Fld(0x1, SHU_CA_DLL0_RG_ARDLL_PHJUMP_EN_CA) 
                        | P_Fld(0x1, SHU_CA_DLL0_RG_ARDLL_PHDIV_CA)
                        | P_Fld(0x0, SHU_CA_DLL0_RG_ARDLL_FAST_PSJP_CA));
            vIO32WriteFldMulti(DDRPHY_SHU_CA_DLL1+((U32)iChannel<<POS_BANK_NUM), P_Fld(0x0, SHU_CA_DLL1_RG_ARDLL_PD_CK_SEL_CA) | P_Fld(0x1, SHU_CA_DLL1_RG_ARDLL_FASTPJ_CK_SEL_CA));
            vIO32WriteFldAlign(DDRPHY_SHU_CA_CMD9+((U32)iChannel<<POS_BANK_NUM), 0, SHU_CA_CMD9_RG_DLL_FAST_PSJP_CA); // RG_*RPI_RESERVE_CA[1] 1'b1 tracking leaf(slave)
        }
    }
#endif

#if (fcFOR_CHIP_ID == fcKrug)
    // @Darren, enhance DLL Gain for 4S corner DDR2400/DDR1600 0.6125V DVS fail
    // DDR3200 (shu2) enhance DLL Gain for corner tightening
    u1CurrShuLevel = vGet_Current_ShuLevel(p);
    if ((u1CurrShuLevel == SRAM_SHU4) || (u1CurrShuLevel == SRAM_SHU6) || (u1CurrShuLevel == SRAM_SHU2))
    {
        vIO32WriteFldAlign_All(DDRPHY_SHU_B0_DLL0, 0x8, SHU_B0_DLL0_RG_ARDLL_GAIN_B0);
        vIO32WriteFldAlign_All(DDRPHY_SHU_B1_DLL0, 0x8, SHU_B1_DLL0_RG_ARDLL_GAIN_B1);
        u1Gain_Ca_ChA = u4IO32ReadFldAlign(DDRPHY_SHU_CA_DLL0, SHU_CA_DLL0_RG_ARDLL_GAIN_CA);
        u1Gain_Ca_ChB = u4IO32ReadFldAlign(DDRPHY_SHU_CA_DLL0+SHIFT_TO_CHB_ADDR, SHU_CA_DLL0_RG_ARDLL_GAIN_CA);
        u1Gain_Ca_ChA += 1;
        u1Gain_Ca_ChB += 1;
        vIO32WriteFldAlign(DDRPHY_SHU_CA_DLL0, u1Gain_Ca_ChA, SHU_CA_DLL0_RG_ARDLL_GAIN_CA);
        vIO32WriteFldAlign(DDRPHY_SHU_CA_DLL0+SHIFT_TO_CHB_ADDR, u1Gain_Ca_ChB, SHU_CA_DLL0_RG_ARDLL_GAIN_CA);
    }
#endif

    U32 u4RegBackupAddress[] =
    {
        (DDRPHY_B0_DQ7),
        (DDRPHY_B1_DQ7),
        (DDRPHY_CA_CMD7),
        (DDRPHY_B0_DQ7 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_B1_DQ7 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_CA_CMD7 + SHIFT_TO_CHB_ADDR),
    };

    //if(p->vendor_id==VENDOR_SAMSUNG && p->dram_type==TYPE_LPDDR3)
    {
#if 0
        mcSHOW_DBG_MSG(("DDRPhyPLLSetting-DMSUS\n"));
        vIO32WriteFldAlign_All(DDRPHY_MISC_SPM_CTRL0, 0x0, MISC_SPM_CTRL0_PHY_SPM_CTL0);
        vIO32WriteFldAlign_All(DDRPHY_MISC_SPM_CTRL2, 0x0, MISC_SPM_CTRL2_PHY_SPM_CTL2);
        vIO32WriteFldMulti_All(DDRPHY_MISC_SPM_CTRL1, P_Fld(0x1, MISC_SPM_CTRL1_RG_ARDMSUS_10) | P_Fld(0x1, MISC_SPM_CTRL1_RG_ARDMSUS_10_B0)
                                                   | P_Fld(0x1, MISC_SPM_CTRL1_RG_ARDMSUS_10_B1) | P_Fld(0x1, MISC_SPM_CTRL1_RG_ARDMSUS_10_CA));
#else
        DramcBackupRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));
        vIO32WriteFldMulti_All(DDRPHY_B0_DQ7, P_Fld(0x1, B0_DQ7_RG_TX_ARDQ_PULL_DN_B0) | P_Fld(0x1, B0_DQ7_RG_TX_ARDQM0_PULL_DN_B0)
                                                   | P_Fld(0x1, B0_DQ7_RG_TX_ARDQS0_PULL_DN_B0) | P_Fld(0x1, B0_DQ7_RG_TX_ARDQS0B_PULL_DN_B0));
        vIO32WriteFldMulti_All(DDRPHY_B1_DQ7, P_Fld(0x1, B1_DQ7_RG_TX_ARDQ_PULL_DN_B1) | P_Fld(0x1, B1_DQ7_RG_TX_ARDQM0_PULL_DN_B1)
                                                   | P_Fld(0x1, B1_DQ7_RG_TX_ARDQS0_PULL_DN_B1) | P_Fld(0x1, B1_DQ7_RG_TX_ARDQS0B_PULL_DN_B1));
        vIO32WriteFldMulti_All(DDRPHY_CA_CMD7, P_Fld(0x1, CA_CMD7_RG_TX_ARCMD_PULL_DN) | P_Fld(0x1, CA_CMD7_RG_TX_ARCS_PULL_DN)
                                                   | P_Fld(0x1, CA_CMD7_RG_TX_ARCLK_PULL_DN) | P_Fld(0x1, CA_CMD7_RG_TX_ARCLKB_PULL_DN));
        
        // DMSUS replaced by CA_CMD2_RG_TX_ARCMD_OE_DIS, CMD_OE_DIS(1) will prevent illegal command ouput
        // And DRAM 1st reset_n pulse will disappear if use CA_CMD2_RG_TX_ARCMD_OE_DIS
        vIO32WriteFldAlign_All(DDRPHY_CA_CMD2, 1, CA_CMD2_RG_TX_ARCMD_OE_DIS);
#endif
    }

    //26M
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0x0, MISC_CG_CTRL0_CLK_MEM_SEL); 

    #ifdef USE_CLK26M
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0x1, MISC_CG_CTRL0_RG_DA_RREF_CK_SEL);
    #endif

    //PLL close sequence: 
    //DLL_PHDET_EN_* = 0 
    //==> CG = 1 
    //==> MIDPI_EN = 0(async) 
    //==> RG_ARPI_RESETB_* = 0 
    //==> MCK8X_EN(source of clk gating) = 0 
    //==> PLL_EN = 0 PIC: Ying-Yu

    //DLL
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI2, 0x0, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI2, 0x0, B0_DLL_ARPI2_RG_ARDLL_PHDET_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI2, 0x0, B1_DLL_ARPI2_RG_ARDLL_PHDET_EN_B1);

    //CG
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

    //MIDPI_EN
    vIO32WriteFldMulti_All(DDRPHY_SHU_B0_DQ6, P_Fld(0x0, SHU_B0_DQ6_RG_ARPI_MIDPI_EN_B0)
                | P_Fld(0x0, SHU_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0));
    vIO32WriteFldMulti_All(DDRPHY_SHU_B1_DQ6, P_Fld(0x0, SHU_B1_DQ6_RG_ARPI_MIDPI_EN_B1)
                | P_Fld(0x0, SHU_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1));
    vIO32WriteFldMulti_All(DDRPHY_SHU_CA_CMD6, P_Fld(0x0, SHU_CA_CMD6_RG_ARPI_MIDPI_EN_CA)
                | P_Fld(0x0, SHU_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA));

    //RESETB
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI0, 0x0, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI0, 0x0, B0_DLL_ARPI0_RG_ARPI_RESETB_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI0, 0x0, B1_DLL_ARPI0_RG_ARPI_RESETB_B1);
    mcDELAY_US(1);

    //MCK8X_EN
    vIO32WriteFldMulti_All(DDRPHY_PLL4, P_Fld(0x0, PLL4_RG_RPHYPLL_ADA_MCK8X_EN)
                | P_Fld(0x0, PLL4_RG_RPHYPLL_RESETB));

    //PLL
    vIO32WriteFldAlign_All(DDRPHY_PLL1, 0x0, PLL1_RG_RPHYPLL_EN);
    vIO32WriteFldAlign_All(DDRPHY_PLL2, 0x0, PLL2_RG_RCLRPLL_EN);

    ///TODO: PLL/MIDPI Settings
    //Ref clock should be 20M~30M, if MPLL=52M, Pre-divider should be set to 1
    #ifdef USE_CLK26M
    vIO32WriteFldMulti_All(DDRPHY_SHU_PLL8, P_Fld(0x0, SHU_PLL8_RG_RPHYPLL_POSDIV) | P_Fld(0x0, SHU_PLL8_RG_RPHYPLL_PREDIV));
    vIO32WriteFldMulti_All(DDRPHY_SHU_PLL10, P_Fld(0x0, SHU_PLL10_RG_RCLRPLL_POSDIV) | P_Fld(0x0, SHU_PLL10_RG_RCLRPLL_PREDIV));
    #else //MPLL 52M
    vIO32WriteFldMulti_All(DDRPHY_SHU_PLL8, P_Fld(0x0, SHU_PLL8_RG_RPHYPLL_POSDIV) | P_Fld(0x1, SHU_PLL8_RG_RPHYPLL_PREDIV));
    vIO32WriteFldMulti_All(DDRPHY_SHU_PLL10, P_Fld(0x0, SHU_PLL10_RG_RCLRPLL_POSDIV) | P_Fld(0x1, SHU_PLL10_RG_RCLRPLL_PREDIV));
    #endif

    if(p->frequency==2133) 
    {
        u2SDM_PCW = 0x5200;
    }
    else if(p->frequency==1866)
    {   // 2722/26=143(0x8f)
        #if ENABLE_FIX_SHORT_PLUSE
        u2SDM_PCW = 0x7b00; // DDR3200
        #else
        u2SDM_PCW = 0x8f00;
        #endif
    }
    else if(p->frequency==1600 || p->frequency==800 || p->frequency==400)
    {
        #if ENABLE_FIX_SHORT_PLUSE
        if (p->frequency==1600)
            u2SDM_PCW = 0x6c00; //DDR2800
        #else
        if (p->frequency==1600)
            u2SDM_PCW = 0x7700; //DDR3200(3094)
        else if (p->frequency==800)
            u2SDM_PCW = 0x7600; //DDR1600(1534)
        #endif
        else if ((p->frequency==400) && (vGet_DDR800_Mode(p) == DDR800_OPEN_LOOP))
            u2SDM_PCW = 0x3c00; //DDR800 Open Loop Mode        
        else if (p->frequency==400)
            u2SDM_PCW = 0x7e00; //DDR826 for avoid GPS de-sense
        else
            u2SDM_PCW = 0x7b00; //DDR1600

#if EMI_LPBK_USE_DDR_800 // For Ei_ger DDR800 no need
        if(p->frequency==800)
        {
            vIO32WriteFldAlign_All(DDRPHY_SHU_PLL8, 0x1, SHU_PLL8_RG_RPHYPLL_POSDIV);
            vIO32WriteFldAlign_All(DDRPHY_SHU_PLL10, 0x1, SHU_PLL10_RG_RCLRPLL_POSDIV);
        }
#endif
    }
    else if(p->frequency==1333 || p->frequency==667)
    {
        u2SDM_PCW = 0x6600;
    }
    else if (p->frequency == 1200)
    {
        #if ENABLE_FIX_SHORT_PLUSE
        u2SDM_PCW = 0x5100; //DDR2100
        #else
        u2SDM_PCW = 0x5c00;
        #endif
    }
    else if (p->frequency == 1140)
    {
        u2SDM_PCW = 0x5700;
    }
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

    /* SDM_PCW: Feedback divide ratio (8-bit integer + 8-bit fraction)
     * PLL_SDM_FRA_EN: SDMPLL fractional mode enable (0:Integer mode, 1:Fractional mode)
     */
    vIO32WriteFldMulti_All(DDRPHY_SHU_PLL5, P_Fld(u2SDM_PCW, SHU_PLL5_RG_RPHYPLL_SDM_PCW)
                                            | P_Fld(0x0, SHU_PLL5_RG_RPHYPLL_SDM_FRA_EN)); // Disable fractional mode
    vIO32WriteFldMulti_All(DDRPHY_SHU_PLL7, P_Fld(u2SDM_PCW, SHU_PLL7_RG_RCLRPLL_SDM_PCW)
                                            | P_Fld(0x0, SHU_PLL7_RG_RCLRPLL_SDM_FRA_EN)); // Disable fractional mode

#if (fcFOR_CHIP_ID == fcKrug) // YH for LPDDR4 1:4 mode and 1:8 mode, Darren confirm with YH Cho
    DDRDllModeSetting(p);
#endif
    
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI0, 0, CA_DLL_ARPI0_RG_ARMCTLPLL_CK_SEL_CA);
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI0, 0, B0_DLL_ARPI0_RG_ARMCTLPLL_CK_SEL_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI0, 0, B1_DLL_ARPI0_RG_ARMCTLPLL_CK_SEL_B1);
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI1, 0, CA_DLL_ARPI1_RG_ARPI_CLKIEN_JUMP_EN);
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI1, 0, B0_DLL_ARPI1_RG_ARPI_MCTL_JUMP_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI1, 0, B1_DLL_ARPI1_RG_ARPI_MCTL_JUMP_EN_B1);


    vIO32WriteFldMulti_All(DDRPHY_SHU_B0_DQ6, P_Fld(u1VTH_SEL, SHU_B0_DQ6_RG_ARPI_MIDPI_VTH_SEL_B0)
                | P_Fld(u1CAP_SEL, SHU_B0_DQ6_RG_ARPI_CAP_SEL_B0)
                | P_Fld(u1MIDPICAP_SEL, SHU_B0_DQ6_RG_ARPI_MIDPI_CAP_SEL_B0));
    vIO32WriteFldMulti_All(DDRPHY_SHU_B1_DQ6, P_Fld(u1VTH_SEL, SHU_B1_DQ6_RG_ARPI_MIDPI_VTH_SEL_B1)
                | P_Fld(u1CAP_SEL, SHU_B1_DQ6_RG_ARPI_CAP_SEL_B1)
                | P_Fld(u1MIDPICAP_SEL, SHU_B1_DQ6_RG_ARPI_MIDPI_CAP_SEL_B1));
    vIO32WriteFldMulti_All(DDRPHY_SHU_CA_CMD6, P_Fld(u1VTH_SEL, SHU_CA_CMD6_RG_ARPI_MIDPI_VTH_SEL_CA)
                | P_Fld(u1CAP_SEL, SHU_CA_CMD6_RG_ARPI_CAP_SEL_CA)
                | P_Fld(u1MIDPICAP_SEL, SHU_CA_CMD6_RG_ARPI_MIDPI_CAP_SEL_CA));

    //PLL open sequence
    //PLL_EN = 1 
    //==> some of MIDPI*_EN = 1(async) 
    //==> RG_ARPI_RESETB_* = 1(async, open first) 
    //==> MCK8X_EN(source of clk gating) = 1 
    //==> CG = 0 
    //==> DLL_PHDET_EN_* = 1 PIC: Ying-Yu

    //PLL 
    vIO32WriteFldAlign_All(DDRPHY_PLL1, 0x1, PLL1_RG_RPHYPLL_EN);
    vIO32WriteFldAlign_All(DDRPHY_PLL2, 0x1, PLL2_RG_RCLRPLL_EN);
    mcDELAY_US(100);

    ///TODO: MIDPI Init 2
    /* MIDPI Settings (Olymp_us): DA_*RPI_MIDPI_EN, DA_*RPI_MIDPI_CKDIV4_EN
     * Justin suggests use frequency > 933 as boundary
     */
    if(p->frequency > 933)
    {
        vIO32WriteFldMulti_All(DDRPHY_SHU_B0_DQ6, P_Fld(0x1, SHU_B0_DQ6_RG_ARPI_MIDPI_EN_B0)
                    | P_Fld(0x0, SHU_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0));
        vIO32WriteFldMulti_All(DDRPHY_SHU_B1_DQ6, P_Fld(0x1, SHU_B1_DQ6_RG_ARPI_MIDPI_EN_B1)
                    | P_Fld(0x0, SHU_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1));
        vIO32WriteFldMulti_All(DDRPHY_SHU_CA_CMD6, P_Fld(0x1, SHU_CA_CMD6_RG_ARPI_MIDPI_EN_CA)
                    | P_Fld(0x0, SHU_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA));
    }
    else
    {
        if ((p->frequency == 400) && (vGet_DDR800_Mode(p) != DDR800_CLOSE_LOOP)) // For *DDR800_OPEN_LOOP
        {
            vIO32WriteFldMulti_All(DDRPHY_SHU_B0_DQ6, P_Fld(0x0, SHU_B0_DQ6_RG_ARPI_MIDPI_EN_B0)
                        | P_Fld(0x0, SHU_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0));
            vIO32WriteFldMulti_All(DDRPHY_SHU_B1_DQ6, P_Fld(0x0, SHU_B1_DQ6_RG_ARPI_MIDPI_EN_B1)
                        | P_Fld(0x0, SHU_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1));
            if (vGet_DDR800_Mode(p) == DDR800_SEMI_LOOP)
            {
                // DDR800_SEMI_LOOP from YY comment to DVT (1/0, CHA CA/other)
                vIO32WriteFldAlign_All(DDRPHY_SHU_CA_CMD6, 0x0, SHU_CA_CMD6_RG_ARPI_MIDPI_EN_CA);
                vIO32WriteFldAlign(DDRPHY_SHU_CA_CMD6, 0x1, SHU_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA);
                vIO32WriteFldAlign(DDRPHY_SHU_CA_CMD6+SHIFT_TO_CHB_ADDR, 0x0, SHU_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA);
            }
            else
            {   //DDR800_OPEN_LOOP
                vIO32WriteFldMulti_All(DDRPHY_SHU_CA_CMD6, P_Fld(0x0, SHU_CA_CMD6_RG_ARPI_MIDPI_EN_CA)
                            | P_Fld(0x0, SHU_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA));
            }
        }
        else
        {
            //MIDPI_EN
            vIO32WriteFldMulti_All(DDRPHY_SHU_B0_DQ6, P_Fld(0x0, SHU_B0_DQ6_RG_ARPI_MIDPI_EN_B0)
                        | P_Fld(0x1, SHU_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0));
            vIO32WriteFldMulti_All(DDRPHY_SHU_B1_DQ6, P_Fld(0x0, SHU_B1_DQ6_RG_ARPI_MIDPI_EN_B1)
                        | P_Fld(0x1, SHU_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1));
            vIO32WriteFldMulti_All(DDRPHY_SHU_CA_CMD6, P_Fld(0x0, SHU_CA_CMD6_RG_ARPI_MIDPI_EN_CA)
                        | P_Fld(0x1, SHU_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA));
        }
    }
    mcDELAY_US(1);

    //RESETB 
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI0, 0x1, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI0, 0x1, B0_DLL_ARPI0_RG_ARPI_RESETB_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI0, 0x1, B1_DLL_ARPI0_RG_ARPI_RESETB_B1);
    mcDELAY_US(1);

    ///TODO: MIDPI Init 1
    //MCK8X_EN
    vIO32WriteFldMulti_All(DDRPHY_PLL4, P_Fld(0x1, PLL4_RG_RPHYPLL_ADA_MCK8X_EN)
                | P_Fld(0x1, PLL4_RG_RPHYPLL_RESETB));
    mcDELAY_US(1);

    #if (fcFOR_CHIP_ID == fcKrug)
    vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI3, P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_MCTL_EN_CA) 
                | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_FB_EN_CA) 
                | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CS_EN) 
                | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CLK_EN) 
                | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CMD_EN));
    vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI3+SHIFT_TO_CHB_ADDR, P_Fld(u1BRPI_MCTL_EN_CA, CA_DLL_ARPI3_RG_ARPI_MCTL_EN_CA) //CH_B CA slave
                | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_FB_EN_CA) 
                | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CS_EN) 
                | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CLK_EN) 
                | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CMD_EN));
    #endif
    vIO32WriteFldMulti_All(DDRPHY_B0_DLL_ARPI3, P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_FB_EN_B0)
                | P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQS_EN_B0)
                | P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQM_EN_B0)
                | P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQ_EN_B0)
                | P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B0));
    vIO32WriteFldMulti_All(DDRPHY_B1_DLL_ARPI3, P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_FB_EN_B1)
                | P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQS_EN_B1)
                | P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQM_EN_B1)
                | P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQ_EN_B1)
                | P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B1));
   //CG
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
    #if (fcFOR_CHIP_ID == fcKrug)
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI2, 1, CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN);
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI2, 0, B0_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI2, 0, B1_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B1);
    #endif
    
    mcDELAY_US(2);

    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0x1, MISC_CG_CTRL0_CLK_MEM_SEL);
    mcDELAY_US(1);

#if ENABLE_APHY_DLL_IDLE_MODE_OPTION //Should before the PHDET_EN = 1
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI5, 0x0, CA_DLL_ARPI5_RG_ARDLL_IDLE_EN_CA);
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI5+SHIFT_TO_CHB_ADDR, 0x1, CA_DLL_ARPI5_RG_ARDLL_IDLE_EN_CA);
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI5, 0x1, B0_DLL_ARPI5_RG_ARDLL_IDLE_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI5, 0x1, B1_DLL_ARPI5_RG_ARDLL_IDLE_EN_B1);

    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI5, u1PdZone, B0_DLL_ARPI5_RG_ARDLL_PD_ZONE_B0);//Zone1: 48ps Zone2: 96ps if(8ps/delay cell)
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI5, u1PdZone, B1_DLL_ARPI5_RG_ARDLL_PD_ZONE_B1);//Intrinsic 2 delay cell, setting: x 
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI5, u1PdZone, CA_DLL_ARPI5_RG_ARDLL_PD_ZONE_CA);//Zone1 = 2 + 2x; Zone2 = 2 * Zone1

    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI5, 0xC, B0_DLL_ARPI5_RG_ARDLL_MON_SEL_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI5, 0xC, B1_DLL_ARPI5_RG_ARDLL_MON_SEL_B1);
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI5, 0xC, CA_DLL_ARPI5_RG_ARDLL_MON_SEL_CA);
#endif

    //DLL
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI2, 0x1, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);
    mcDELAY_US(1);
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI2, 0x1, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);
    mcDELAY_US(1);
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI2, 0x1, B0_DLL_ARPI2_RG_ARDLL_PHDET_EN_B0);
    mcDELAY_US(1);
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI2, 0x1, B1_DLL_ARPI2_RG_ARDLL_PHDET_EN_B1);
    mcDELAY_US(1);

    //if(p->vendor_id==VENDOR_SAMSUNG && p->dram_type==TYPE_LPDDR3)
    {
#if 0
        mcSHOW_DBG_MSG(("DDRPhyPLLSetting-DMSUS\n\n"));
        vIO32WriteFldMulti_All(DDRPHY_MISC_SPM_CTRL1, P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10) | P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_B0)
                                                   | P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_B1) | P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_CA));
        vIO32WriteFldAlign_All(DDRPHY_MISC_SPM_CTRL0, 0xffffffff, MISC_SPM_CTRL0_PHY_SPM_CTL0);
        vIO32WriteFldAlign_All(DDRPHY_MISC_SPM_CTRL2, 0xffffffff, MISC_SPM_CTRL2_PHY_SPM_CTL2);
#else
        // DMSUS replaced by CA_CMD2_RG_TX_ARCMD_OE_DIS, CMD_OE_DIS(1) will prevent illegal command ouput
        // And DRAM 1st reset_n pulse will disappear if use CA_CMD2_RG_TX_ARCMD_OE_DIS
        vIO32WriteFldAlign_All(DDRPHY_CA_CMD2, 0, CA_CMD2_RG_TX_ARCMD_OE_DIS);
        DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));
#endif

        mcSHOW_DBG_MSG(("DDRPhyPLLSetting-CKEON\n\n"));
        
        CKEFixOnOff(p, CKE_WRITE_TO_ALL_RANK, CKE_DYNAMIC, CKE_WRITE_TO_ALL_CHANNEL);
    }

#if ENABLE_DFS_SSC_WA
    DDRSSCSetting(p);
#endif

    DDRPhyFreqMeter();
}

void DramcCmdUIDelaySetting(DRAMC_CTX_T *p, U8 value)
{
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA7), P_Fld(value, SHU_SELPH_CA7_DLY_RA0) |
                                                                P_Fld(value, SHU_SELPH_CA7_DLY_RA1) |
                                                                P_Fld(value, SHU_SELPH_CA7_DLY_RA2) |
                                                                P_Fld(value, SHU_SELPH_CA7_DLY_RA3) |
                                                                P_Fld(value, SHU_SELPH_CA7_DLY_RA4) |
                                                                P_Fld(value, SHU_SELPH_CA7_DLY_RA5));
    
    // Note: CKE UI must sync CA UI (CA and CKE delay circuit are same) @Lin-Yi
    // To avoid tXP timing margin issue
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA5), value, SHU_SELPH_CA5_DLY_CKE);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA6), value, SHU_SELPH_CA6_DLY_CKE1);  
}

#if 1
void DramcPowerOnSequence(DRAMC_CTX_T *p)
{
#if ENABLE_TMRRI_NEW_MODE
    U8 u1RankIdx;
#endif
    #if APPLY_LP4_POWER_INIT_SEQUENCE
    //static U8 u1PowerOn=0;
    //if(u1PowerOn ==0)
    {
    //reset dram = low
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 0x0, MISC_CTRL1_R_DMDA_RRESETB_I);

    //vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0, RKCFG_CKE2RANK_OPT2);

    //CKE low
    CKEFixOnOff(p, CKE_WRITE_TO_ALL_RANK, CKE_FIXOFF, CKE_WRITE_TO_ALL_CHANNEL);

    // delay tINIT1=200us(min) & tINIT2=10ns(min)
    mcDELAY_US(200);

    //reset dram = low
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 0x1, MISC_CTRL1_R_DMDA_RRESETB_I);

    // Disable HW MIOCK control to make CLK always on
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 1, DRAMC_PD_CTRL_MIOCKCTRLOFF);

    //tINIT3=2ms(min)
    mcDELAY_MS(2);

    //CKE high
    CKEFixOnOff(p, CKE_WRITE_TO_ALL_RANK, CKE_FIXON, CKE_WRITE_TO_ALL_CHANNEL);

    // tINIT5=2us(min)
    mcDELAY_US(2);
    //u1PowerOn=1;
    mcSHOW_DBG_MSG3(("APPLY_LP4_POWER_INIT_SEQUENCE\n"));
    }
    #endif
}

U8 gCATermWA = 0;
DRAM_STATUS_T DramcModeRegInit_CATerm(DRAMC_CTX_T *p, U8 bWorkAround)
{
    U32 u4RankIdx;//, u4CKE0Bak, u4CKE1Bak, u4MIOCKBak, u4AutoRefreshBak;
    U16 u2MR3Value;
    U8 u1MRFsp= FSP_0;
    U8 u1ChannelIdx, support_channel_num;
    U8 backup_channel, backup_rank;
    U8 operating_fsp = p->dram_fsp;
    U32 backup_broadcast;
    U8 u1MR11Value;
    U8 u1MR22Value;

    if(gCATermWA==bWorkAround)
        return DRAM_OK;

    gCATermWA=bWorkAround;
    
    backup_broadcast = GetDramcBroadcast();

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    
    u1ChannelIdx = p->channel;
    backup_rank = p->rank;

    u1PrintModeRegWrite=1;
#if VENDER_JV_LOG
    vPrintCalibrationBasicInfo_ForJV(p);
#endif

    //for(u1ChannelIdx=0; u1ChannelIdx<(p->support_channel_num); u1ChannelIdx++)
    //{
    //    vSetPHY2ChannelMapping(p, u1ChannelIdx);

        for(u4RankIdx =0; u4RankIdx < (U32)(p->support_rank_num); u4RankIdx++)
        {
            vSetRank(p, u4RankIdx);
            
            mcSHOW_DBG_MSG(("[DramcModeRegInit_CATerm] CH%u RK%u bWorkAround=%d\n", u1ChannelIdx, u4RankIdx, bWorkAround));
            mcDUMP_REG_MSG(("[DramcModeRegInit_CATerm] CH%u RK%u bWorkAround=%d\n", u1ChannelIdx, u4RankIdx, bWorkAround));

            mcFPRINTF((fp_A60501, "[DramcModeRegInit_CATerm] CH%u RK%d\n", u1ChannelIdx, u4RankIdx));
            #if VENDER_JV_LOG
            mcSHOW_JV_LOG_MSG(("\n[DramcModeRegInit_CATerm] CH%u RK%d\n", u1ChannelIdx, u4RankIdx));
            #endif
            #if MRW_CHECK_ONLY
            mcSHOW_MRW_MSG(("\n==[MR Dump] %s==\n", __func__));
            #endif
        
            //else
            {
                /* FSP_1: 1. For term freqs   2. Assumes "data rate >= DDR2667" are terminated */
                u1MRFsp = FSP_1;
                mcSHOW_DBG_MSG3(("\tFsp%d\n", u1MRFsp));
                mcFPRINTF((fp_A60501, "\tFsp%d\n", u1MRFsp));
                #if VENDER_JV_LOG
                mcSHOW_JV_LOG_MSG(("\tFsp%d\n", u1MRFsp));
                #endif

                #if MRW_CHECK_ONLY
                p->dram_fsp = FSP_1;
                #endif

                DramcMRWriteFldAlign(p, 13, 1, MR13_FSP_WR, TO_MR);
                
                //MR12 use previous value
                //DramcModeRegWriteByRank(p, u4RankIdx, 12, u1MR12Value[u1ChannelIdx][u4RankIdx][u1MRFsp]); //MR12 VREF-CA

                if(p->dram_type == TYPE_LPDDR4P)
                    u1MR11Value = 0x0;                  //ODT disable
                else
                {
#if ENABLE_SAMSUNG_NT_ODT
                    if (p->vendor_id == VENDOR_SAMSUNG)
                    {
                        u1MR11Value = 0x2;              //@Darren, DQ ODT:120ohm -> parallel to 60ohm
                        u1MR11Value |= (0x1 << 3);   //@Darren, MR11[3]=1 to enable NT-ODT for B707
                    }
                    else
#endif
                        u1MR11Value = 0x3;              //DQ ODT:80ohm

                    #if FSP1_CLKCA_TERM
                    if(p->dram_cbt_mode[u4RankIdx]==CBT_NORMAL_MODE)
                    {
                        u1MR11Value |= 0x40;              //CA ODT:60ohm for byte mode
                    }
                    else
                    {
                        u1MR11Value |= 0x20;              //CA ODT:120ohm for byte mode
                    }
                    #endif
                }
                #if APPLY_SIGNAL_WAVEFORM_SETTINGS_ADJUST
                    if (gDramcDqOdtRZQAdjust>=0)
                    {
                        u1MR11Value &= ~(0x7);
                        u1MR11Value = gDramcDqOdtRZQAdjust;
                    }
                #endif
                    DramcModeRegWriteByRank(p, u4RankIdx, 11, u1MR11Value); //ODT

                if(p->dram_type == TYPE_LPDDR4)
                {
                    u1MR22Value = 0x24;     //SOC-ODT, ODTE-CK, ODTE-CS, Disable ODTD-CA
                }
                else  //TYPE_LPDDR4x, LP4P
                {
                    u1MR22Value = 0x3c;     //Disable CA-CS-CLK ODT, SOC ODT=RZQ/4
                    #if FSP1_CLKCA_TERM
                    if(bWorkAround)
                    {
                        u1MR22Value = 0x4;
                    }
                    else
                    {
                        if(u4RankIdx==RANK_0)
                        {
                            u1MR22Value = 0x4;     //Enable CA-CS-CLK ODT, SOC ODT=RZQ/4
                        }
                        else
                        {
                            u1MR22Value = 0x2c;     //Enable CS ODT, SOC ODT=RZQ/4
                        }
                    }
                    #endif
                }
                #if APPLY_SIGNAL_WAVEFORM_SETTINGS_ADJUST
                if (gDramcMR22SoCODTAdjust[u1MRFsp]>=0)
                {
                    u1MR22Value = (u1MR22Value&~(0x7))|gDramcMR22SoCODTAdjust[u1MRFsp];
                }
                #endif
                DramcModeRegWriteByRank(p, u4RankIdx, 22, u1MR22Value);

            }
        }
    //}

    //vSetPHY2ChannelMapping(p, backup_channel);
    vSetRank(p, backup_rank);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_MRS), RANK_0, MRS_MRSRK);
#if MRW_CHECK_ONLY
    p->dram_fsp = operating_fsp;
#endif

    DramcBroadcastOnOff(backup_broadcast);
    u1PrintModeRegWrite=0;
    return DRAM_OK;
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
    U8 u1MR11Value;
    U8 u1MR22Value;
    U8 u1nWR=0;
    U16 u2FreqMax = u2DFSGetHighestFreq(p);
    U8 u1set_mrsrk=0;

    backup_broadcast = GetDramcBroadcast();

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    

    DramcPowerOnSequence(p);
    
    backup_channel = p->channel;
    backup_rank = p->rank;


#if VENDER_JV_LOG
    vPrintCalibrationBasicInfo_ForJV(p);
#endif

    /* Fix nWR value to 30 (MR01[6:4] = 101B) for DDR3200
     * Fix nWR value to 34 (MR01[6:4] = 110B) for DDR3733
     * Other vendors: Use default MR01 for each FSP (Set in vInitGlobalVariablesByCondition() )
     */
    {
        /* Clear MR01 OP[6:4] */
        u1MR01Value[FSP_0] &= 0x8F;
        u1MR01Value[FSP_1] &= 0x8F;
        if (u2FreqMax == 2133)
        {
            /* Set MR01 OP[6:4] to 111B = 7 */
            u1MR01Value[FSP_0] |= (0x7 << 4);
            u1MR01Value[FSP_1] |= (0x7 << 4);
            u1nWR = 40;
        }
        else if (u2FreqMax == 1866)
        {
            /* Set MR01 OP[6:4] to 110B = 6 */
            u1MR01Value[FSP_0] |= (0x6 << 4);
            u1MR01Value[FSP_1] |= (0x6 << 4);
            u1nWR = 34;
        }
        else // Freq <= 1600
        {
            /* Set MR01 OP[6:4] to 101B = 5 */
            u1MR01Value[FSP_0] |= (0x5 << 4);
            u1MR01Value[FSP_1] |= (0x5 << 4);
            u1nWR = 30;
        }

        mcSHOW_DBG_MSG(("nWR fixed to %d\n", u1nWR));
        mcDUMP_REG_MSG(("nWR fixed to %d\n", u1nWR));
    }

#if (SW_CHANGE_FOR_SIMULATION == 0 && !defined(DUMP_INIT_RG_LOG_TO_DE))
    if (Get_PRE_MIOCK_JMETER_HQA_USED_flag() == 0)
    {
       //switch to low freq
       (p->dram_fsp == FSP_1)? CBT_Switch_Freq(p, CBT_LOW_FREQ): NULL;
    }
#endif

    for(u1ChannelIdx=0; u1ChannelIdx<(p->support_channel_num); u1ChannelIdx++)
    {
        vSetPHY2ChannelMapping(p, u1ChannelIdx);

        for(u4RankIdx =0; u4RankIdx < (U32)(p->support_rank_num); u4RankIdx++)
        {
            vSetRank(p, u4RankIdx);
            
            mcSHOW_DBG_MSG(("[ModeRegInit_LP4] CH%u RK%u\n", u1ChannelIdx, u4RankIdx));
            mcDUMP_REG_MSG(("[ModeRegInit_LP4] CH%u RK%u\n", u1ChannelIdx, u4RankIdx));
            mcFPRINTF((fp_A60501, "[ModeRegInit_LP4] CH%u RK%d\n", u1ChannelIdx, u4RankIdx));
            #if VENDER_JV_LOG
            mcSHOW_JV_LOG_MSG(("\n[ModeRegInit_LP4] CH%u RK%d\n", u1ChannelIdx, u4RankIdx));
            #endif
            #if MRW_CHECK_ONLY
            mcSHOW_MRW_MSG(("\n==[MR Dump] %s==\n", __func__));
            #endif

            //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u4RankIdx, MRS_MRSRK);

            // Note : MR37 for LP4P should be set before any Mode register.
            // MR37 is not shadow register, just need to set by channel and rank. No need to set by FSP
            if(p->dram_type == TYPE_LPDDR4P)
            {
                //temp solution, need remove later
                #ifndef MT6779_FPGA
                #if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
                    dramc_set_vddq_voltage(p->dram_type, 600000);
                #endif
                #endif
                
                DramcModeRegWriteByRank(p, u4RankIdx, 37, 0x1);

                //temp solution, need remove later
                #ifndef MT6779_FPGA
                #if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
                    dramc_set_vddq_voltage(p->dram_type, 400000);
                #endif
                #endif
            }

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
                mcSHOW_JV_LOG_MSG(("\tFsp%d\n", u1MRFsp));
                #endif
                
                #if MRW_CHECK_ONLY
                p->dram_fsp = FSP_0;
                #endif


                u1MR13Value = 0;
                MRWriteFldMulti(p, 13, P_Fld(MR13_RRO_VALUE, MR13_PRO) |
                                       P_Fld(1, MR13_VRCG), 
                                       TO_MR);
                
                //MR12 use previous value
                DramcModeRegWriteByRank(p, u4RankIdx, 12, u1MR12Value[u1ChannelIdx][u4RankIdx][u1MRFsp]);
                DramcModeRegWriteByRank(p, u4RankIdx, 1, u1MR01Value[u1MRFsp]);

                //MR2 set Read/Write Latency
                if (p->freqGroup == 400) // DDR800
                {
                    u1MR02Value[u1MRFsp] = 0x12; // the minimum of WL is 8, and the minimum of RL is 14 (Jouling and HJ)
                }
                else if ((p->freqGroup == 800) || (p->freqGroup == 600)) // DDR1600, DDR1200
                {
                    u1MR02Value[u1MRFsp] = 0x12;
                }
                else if (p->freqGroup == 1200) // DDR2280, DDR2400 (DDR2667 uses FSP_1)
                {
                    u1MR02Value[u1MRFsp] = 0x24;
                }
                
                DramcModeRegWriteByRank(p, u4RankIdx, 2, u1MR02Value[u1MRFsp]);
                //if(p->odt_onoff)
                DramcModeRegWriteByRank(p, u4RankIdx, 11, 0x0); //ODT disable

                if(p->dram_type == TYPE_LPDDR4)
                {
                    u1MR22Value = 0x20;     //SOC-ODT, ODTE-CK, ODTE-CS, Disable ODTD-CA
                }
                else  //TYPE_LPDDR4x, LP4P
                {
                    u1MR22Value = 0x38;     //SOC-ODT, ODTE-CK, ODTE-CS, Disable ODTD-CA
                }
                #if APPLY_SIGNAL_WAVEFORM_SETTINGS_ADJUST
                if (gDramcMR22SoCODTAdjust[u1MRFsp]>=0)
                {
                    u1MR22Value = (u1MR22Value&~(0x7))|gDramcMR22SoCODTAdjust[u1MRFsp];
                }
                #endif
                DramcModeRegWriteByRank(p, u4RankIdx, 22, u1MR22Value);

                //MR14 use previous value
                DramcModeRegWriteByRank(p, u4RankIdx, 14, u1MR14Value[u1ChannelIdx][u4RankIdx][u1MRFsp]); //MR14 VREF-DQ 
                
                #if CALIBRATION_SPEED_UP_DEBUG
                mcSHOW_DBG_MSG(("Yulia CBT Vref Init: CH%d Rank%d FSP%d, Range %d Vref %d\n\n",p->channel, p->rank, u1MRFsp, u1MR12Value[u1ChannelIdx][u4RankIdx][u1MRFsp]>>6, (u1MR12Value[u1ChannelIdx][u4RankIdx][u1MRFsp] & 0x3f)));
                mcSHOW_DBG_MSG(("Yulia TX Vref Init: CH%d Rank%d FSP%d, TX Range %d Vref %d\n\n",p->channel, p->rank, u1MRFsp,u1MR14Value[u1ChannelIdx][u4RankIdx][u1MRFsp]>>6, (u1MR14Value[u1ChannelIdx][u4RankIdx][u1MRFsp] & 0x3f)));
                #endif
                
                //MR3 set write-DBI and read-DBI (Disabled during calibration, enabled after K)
                u1MR03Value[u1MRFsp] = (u1MR03Value[u1MRFsp]&0x3F);
                
                if(p->dram_type == TYPE_LPDDR4X || p->dram_type == TYPE_LPDDR4P)
                {
                    u1MR03Value[u1MRFsp] &= 0xfe;
                }
                #if APPLY_SIGNAL_WAVEFORM_SETTINGS_ADJUST
                if (gDramcMR03PDDSAdjust[u1MRFsp]>=0)
                {
                    u1MR03Value[u1MRFsp] = (u1MR03Value[u1MRFsp]&~(0x7<<3))|(gDramcMR03PDDSAdjust[u1MRFsp]<<3);
                }
                #endif
                DramcModeRegWriteByRank(p, u4RankIdx, 3, u1MR03Value[u1MRFsp]);
            }
            //else
            {
                /* FSP_1: 1. For term freqs   2. Assumes "data rate >= DDR2667" are terminated */
                u1MRFsp = FSP_1;
                mcSHOW_DBG_MSG3(("\tFsp%d\n", u1MRFsp));
                mcFPRINTF((fp_A60501, "\tFsp%d\n", u1MRFsp));
                #if VENDER_JV_LOG
                mcSHOW_JV_LOG_MSG(("\tFsp%d\n", u1MRFsp));
                #endif

                #if MRW_CHECK_ONLY
                p->dram_fsp = FSP_1;
                #endif

                DramcMRWriteFldAlign(p, 13, 1, MR13_FSP_WR, TO_MR);
                
                //MR12 use previous value
                DramcModeRegWriteByRank(p, u4RankIdx, 12, u1MR12Value[u1ChannelIdx][u4RankIdx][u1MRFsp]); //MR12 VREF-CA
                DramcModeRegWriteByRank(p, u4RankIdx, 1, u1MR01Value[u1MRFsp]); 

                //MR2 set Read/Write Latency
                if (p->freqGroup == 2133)
                {
                    u1MR02Value[u1MRFsp] = 0x3f;
                }
                else if (p->freqGroup == 1866)
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
                DramcModeRegWriteByRank(p, u4RankIdx, 2, u1MR02Value[u1MRFsp]); 

                if(p->dram_type == TYPE_LPDDR4P)
                    u1MR11Value = 0x0;                  //ODT disable
                else
                {
#if ENABLE_SAMSUNG_NT_ODT
                    if (p->vendor_id == VENDOR_SAMSUNG)
                    {
                        u1MR11Value = 0x2;              //@Darren, DQ ODT:120ohm -> parallel to 60ohm
                        u1MR11Value |= (0x1 << 3);   //@Darren, MR11[3]=1 to enable NT-ODT for B707
                    }
                    else
#endif
                        u1MR11Value = 0x3;              //DQ ODT:80ohm

                    #if FSP1_CLKCA_TERM
                    if(p->dram_cbt_mode[u4RankIdx]==CBT_NORMAL_MODE)
                    {
                        u1MR11Value |= 0x40;              //CA ODT:60ohm for normal mode die
                    }
                    else
                    {
                        u1MR11Value |= 0x20;              //CA ODT:120ohm for byte mode die
                    }
                    #endif
                }
                #if APPLY_SIGNAL_WAVEFORM_SETTINGS_ADJUST
                    if (gDramcDqOdtRZQAdjust>=0)
                    {
                        u1MR11Value &= ~(0x7);
                        u1MR11Value = gDramcDqOdtRZQAdjust;
                    }
                #endif
                    DramcModeRegWriteByRank(p, u4RankIdx, 11, u1MR11Value); //ODT

                if(p->dram_type == TYPE_LPDDR4)
                {
                    u1MR22Value = 0x24;     //SOC-ODT, ODTE-CK, ODTE-CS, Disable ODTD-CA
                }
                else  //TYPE_LPDDR4x, LP4P
                {
                    u1MR22Value = 0x3c;     //Disable CA-CS-CLK ODT, SOC ODT=RZQ/4
                    #if FSP1_CLKCA_TERM
                    if(u4RankIdx==RANK_0)
                    {
                        u1MR22Value = 0x4;     //Enable CA-CS-CLK ODT, SOC ODT=RZQ/4
                    }
                    else
                    {
                        u1MR22Value = 0x2c;     //Enable CS ODT, SOC ODT=RZQ/4
                    }
                    #endif
                }
                #if APPLY_SIGNAL_WAVEFORM_SETTINGS_ADJUST
                if (gDramcMR22SoCODTAdjust[u1MRFsp]>=0)
                {
                    u1MR22Value = (u1MR22Value&~(0x7))|gDramcMR22SoCODTAdjust[u1MRFsp];
                }
                #endif
                DramcModeRegWriteByRank(p, u4RankIdx, 22, u1MR22Value);

                //MR14 use previous value
                DramcModeRegWriteByRank(p, u4RankIdx, 14, u1MR14Value[u1ChannelIdx][u4RankIdx][u1MRFsp]); //MR14 VREF-DQ
                
                #if CALIBRATION_SPEED_UP_DEBUG
                mcSHOW_DBG_MSG(("Yulia CBT Vref Init: CH%d Rank%d FSP%d, Range %d Vref %d\n\n",p->channel, p->rank, u1MRFsp, u1MR12Value[u1ChannelIdx][u4RankIdx][u1MRFsp]>>6, (u1MR12Value[u1ChannelIdx][u4RankIdx][u1MRFsp] & 0x3f)));
                mcSHOW_DBG_MSG(("Yulia TX Vref Init: CH%d Rank%d FSP%d, TX Range %d Vref %d\n\n",p->channel, p->rank, u1MRFsp, u1MR14Value[u1ChannelIdx][u4RankIdx][u1MRFsp]>>6, (u1MR14Value[u1ChannelIdx][u4RankIdx][u1MRFsp] & 0x3f)));
                #endif
                
                //MR3 set write-DBI and read-DBI (Disabled during calibration, enabled after K)
                u1MR03Value[u1MRFsp] = (u1MR03Value[u1MRFsp]&0x3F);
       
                if(p->dram_type == TYPE_LPDDR4X || p->dram_type == TYPE_LPDDR4P)
                {
                    u1MR03Value[u1MRFsp] &= 0xfe;
                }
                #if APPLY_SIGNAL_WAVEFORM_SETTINGS_ADJUST
                if (gDramcMR03PDDSAdjust[u1MRFsp]>=0)
                {
                    u1MR03Value[u1MRFsp] = (u1MR03Value[u1MRFsp]&~(0x7<<3))|(gDramcMR03PDDSAdjust[u1MRFsp]<<3);
                }
                #endif
                DramcModeRegWriteByRank(p, u4RankIdx, 3, u1MR03Value[u1MRFsp]);
            }
        }

        /* freq < 1333 is assumed to be odt_off -> uses FSP_0 */
        //if (p->frequency < MRFSP_TERM_FREQ)
        if(operating_fsp == FSP_0)
        {
            DramcMRWriteFldAlign(p, 13, 0, MR13_FSP_OP, JUST_TO_GLOBAL_VALUE);
            DramcMRWriteFldAlign(p, 13, 0, MR13_FSP_WR, JUST_TO_GLOBAL_VALUE);
        }
        else
        {
            DramcMRWriteFldAlign(p, 13, 1, MR13_FSP_OP, JUST_TO_GLOBAL_VALUE);
            DramcMRWriteFldAlign(p, 13, 1, MR13_FSP_WR, JUST_TO_GLOBAL_VALUE);
        }

#if 0
        for(u4RankIdx =0; u4RankIdx < (U32)(p->support_rank_num); u4RankIdx++)
        {
            DramcModeRegWriteByRank(p, u4RankIdx, 13, u1MR13Value);
        }
#endif

        /* MRS two ranks simutaniously */
        if (p->support_rank_num == RANK_DUAL)
            u1set_mrsrk = 0x3;
        else
            u1set_mrsrk = RANK_0;
        DramcModeRegWriteByRank(p, u1set_mrsrk, 13, u1MR13Value);

        /* Auto-MRW related register write (Used during HW DVFS frequency switch flow)
         * VRCG seems to be enabled/disabled even when switching to same FSP(but different freq) to simplify HW DVFS flow
         */
        // 1. MR13 OP[3] = 1 : Enable "high current mode" to reduce the settling time when changing FSP(freq) during operation
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_MR13), P_Fld(u1MR13Value | (0x1 << 3), SHU_HWSET_MR13_HWSET_MR13_OP)
                                                                    | P_Fld(13, SHU_HWSET_MR13_HWSET_MR13_MRSMA));
        // 2. MR13 OP[3] = 1 : Enable "high current mode" after FSP(freq) switch operation for calibration
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_VRCG), P_Fld(u1MR13Value | (0x1 << 3), SHU_HWSET_VRCG_HWSET_VRCG_OP)
                                                                    | P_Fld(13, SHU_HWSET_VRCG_HWSET_VRCG_MRSMA));
        // 3. MR2 : Set RL/WL after FSP(freq) switch
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_MR2), P_Fld(u1MR02Value[operating_fsp], SHU_HWSET_MR2_HWSET_MR2_OP)
                                                                    | P_Fld(2, SHU_HWSET_MR2_HWSET_MR2_MRSMA));
    }

#if (SW_CHANGE_FOR_SIMULATION == 0 && !defined(DUMP_INIT_RG_LOG_TO_DE))
    if (Get_PRE_MIOCK_JMETER_HQA_USED_flag() == 0)
    {
        //switch to low freq
        (p->dram_fsp == FSP_1)? CBT_Switch_Freq(p, CBT_HIGH_FREQ): NULL;
    }
#endif

    vSetPHY2ChannelMapping(p, backup_channel);
    vSetRank(p, backup_rank);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_MRS), RANK_0, MRS_MRSRK);
#if MRW_CHECK_ONLY
    p->dram_fsp = operating_fsp;
#endif

    DramcBroadcastOnOff(backup_broadcast);

#if SAMSUNG_TEST_MODE_MRS_FOR_PRELOADER
    vApplyProgramSequence(p);
#endif

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
            #ifndef MT6779_FPGA
            #if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
                dramc_set_vddq_voltage(p->dram_type, 600000);
            #endif
            #endif

            DramcModeRegWrite(p, 37, 0x1);

            //temp solution, need remove later
            #ifndef MT6779_FPGA
            #if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
                dramc_set_vddq_voltage(p->dram_type, 400000);
            #endif
            #endif
        }

        //if(p->frequency<=1200)
        if(p->odt_onoff == ODT_OFF)
        {
            u1MR13Value[p->dram_fsp] = 0 | (MR13_RRO<<4) | (1<<3);
            DramcModeRegWrite(p, 13, u1MR13Value[p->dram_fsp]); //FSP-0
//            u1MR12Value[p->channel][p->dram_fsp] = 0x4d;
            DramcModeRegWrite(p, 12, u1MR12Value[p->channel][p->dram_fsp]); //MR12 VREF-CA
            u1MR01Value[p->dram_fsp] = 0x56;
            DramcModeRegWrite(p, 1, u1MR01Value[p->dram_fsp]);

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
            DramcModeRegWrite(p, 2, u1MR02Value[p->dram_fsp]);
            DramcModeRegWrite(p, 11, 0x0);     //ODT
            if(p->dram_type == TYPE_LPDDR4)
            {
                DramcModeRegWrite(p, 22, 0x20);    //SOC-ODT, ODTE-CK, ODTE-CS, disable ODTD-CA
            }
            else  //TYPE_LPDDR4x & TYPE_LPDDR4P
            {
                DramcModeRegWrite(p, 22, 0x38);    //SOC-ODT, ODTE-CK, ODTE-CS, disable ODTD-CA
            }
  //          u1MR14Value[p->channel][p->dram_fsp] = 0x4d;
            DramcModeRegWrite(p, 14, u1MR14Value[p->channel][p->dram_fsp]);    //MR14 VREF-DQ
        }
        else
        {
            #if 0//CA-Term
            u1MR13Value[p->dram_fsp] = 0xc0 | (MR13_RRO<<4);
            DramcModeRegWrite(p, 13, u1MR13Value[p->dram_fsp]); //FSP-1
            u1MR12Value[p->dram_fsp] = 0x10;
            DramcModeRegWrite(p, 12, u1MR12Value[p->dram_fsp]); //MR12 VREF-CA
            u1MR01Value[p->dram_fsp] = 0x56;
            DramcModeRegWrite(p, 1, u1MR01Value[p->dram_fsp]);

            if(p->frequency==1600)
            {
                u1MR02Value[p->dram_fsp] = 0x2d;
            }
            else if(p->frequency==1333)
            {
                u1MR02Value[p->dram_fsp] = 0x24;
            }
            DramcModeRegWrite(p, 2, u1MR02Value[p->dram_fsp]);
            DramcModeRegWrite(p, 11, 0x44);     //ODT
            DramcModeRegWrite(p, 22, 0xC4);    //SOC-ODT, ODTE-CK, ODTE-CS, ODTD-CA
            u1MR14Value[p->dram_fsp] = 0x10;
            DramcModeRegWrite(p, 14, u1MR14Value);    //MR14 VREF-DQ
            #else
            u1MR13Value[p->dram_fsp] = 0xc0 | (MR13_RRO<<4) | (1<<3);
            DramcModeRegWrite(p, 13, u1MR13Value[p->dram_fsp]); //FSP-1
//            u1MR12Value[p->channel][p->dram_fsp] = 0x4d;
            DramcModeRegWrite(p, 12, u1MR12Value[p->channel][p->dram_fsp]); //MR12 VREF-CA
            DramcModeRegWrite(p, 1, 0x56);
            if(p->frequency==1600)
            {
                u1MR02Value[p->dram_fsp] = 0x2d;
            }
            else if(p->frequency==1333)
            {
                u1MR02Value[p->dram_fsp] = 0x24;
            }
            DramcModeRegWrite(p, 2, u1MR02Value[p->dram_fsp]);
            DramcModeRegWrite(p, 11, 0x04);     //ODT
            if(p->dram_type == TYPE_LPDDR4)
            {
                DramcModeRegWrite(p, 22, 0x24);    //SOC-ODT, ODTE-CK, ODTE-CS, disable ODTD-CA
            }
            else  //TYPE_LPDDR4x
            {
                DramcModeRegWrite(p, 22, 0x3c);    //SOC-ODT, ODTE-CK, ODTE-CS, disable ODTD-CA
            }

//            u1MR14Value[p->channel][p->dram_fsp] = 0x10;
            DramcModeRegWrite(p, 14, u1MR14Value[p->channel][p->dram_fsp]);    //MR14 VREF-DQ
            #endif
        }

        u2MR3Value = 0x31;
        u2MR3Value |= ((p->DBI_W_onoff)<<7 | (p->DBI_R_onoff)<<6);
        if(p->dram_type == TYPE_LPDDR4X || p->dram_type == TYPE_LPDDR4P)
                u2MR3Value &= 0xfe;  // MR3[0] =0, PU-Cal= VEED/2.5

        DramcModeRegWrite(p, 3, u2MR3Value);//Enable DBI
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

void MPLLInit()
{
#if (FOR_DV_SIMULATION_USED==0)
	unsigned int tmp;

	DRV_WriteReg32(AP_PLL_CON0, 0x11); // CLKSQ Enable
	mcDELAY_US(100);
	DRV_WriteReg32(AP_PLL_CON0, 0x13); // CLKSQ LPF Enable
	mcDELAY_MS(1);
	DRV_WriteReg32(MPLL_PWR_CON0, 0x3); // power on MPLL
	mcDELAY_US(30);
	DRV_WriteReg32(MPLL_PWR_CON0, 0x1); // turn off ISO of  MPLL
	mcDELAY_US(1);
	tmp = DRV_Reg32(MPLL_CON1);
	DRV_WriteReg32(MPLL_CON1, tmp | 0x80000000); // Config MPLL freq
	DRV_WriteReg32(MPLL_CON0, 0x181); // enable MPLL
	mcDELAY_US(20);
#endif
}


#if ENABLE_RODT_TRACKING_SAVE_MCK
void SetTxWDQSStatusOnOff(U8 u1OnOff)
{
      u1WDQS_ON = u1OnOff;
}
#endif

void DDRPhyReservedRGSetting(DRAMC_CTX_T *p)
{
    U8 u1HYST_SEL=0;
    U8 u1MIDPI_CAP_SEL2=0;
    U8 u1LP3_SEL=0;
    U8 u1SER_RST_MODE=1;
    U8 u1TX_READ_BASE_EN=1;
    U8 u1PSMUX_XLAT_FORCE=0;
    U8 u1Bypass_SR = 1;
    U8 u1Smt_Xlat_Force = 0;

    if(p->frequency<=1333)
        u1HYST_SEL=1;

    if(p->frequency == 1200) // Review by YY Hsu
        u1MIDPI_CAP_SEL2=1;
    else
        u1MIDPI_CAP_SEL2=0;

    if(p->frequency<=1200) // @YY, for duty enhance. if low voltage duty becomes worse that it must be 1'b0
        u1LP3_SEL=1;

    if (p->frequency >= 1600) //@YY, DDR3200 up for duty enhance
        u1PSMUX_XLAT_FORCE=1;

    if ((p->frequency == 400) && (vGet_DDR800_Mode(p) == DDR800_SEMI_LOOP))
    {
        //@Darren for DDR800semi (reduce leakage)
        // DDR400open need confirm for P_etrus
        // DDR800close same with DDR1600close
        u1Bypass_SR = 0;
        u1Smt_Xlat_Force = 0;
    }
    else if(p->frequency<=800)
    {
        u1Bypass_SR = 0;
        u1Smt_Xlat_Force = 1;
    }
    else
    {
        u1Bypass_SR = 1;
        u1Smt_Xlat_Force = 0;
    }

    //PI
    vIO32WriteFldMulti(DDRPHY_SHU_CA_CMD9, P_Fld(0x1, SHU_CA_CMD9_RG_PI_TX_CG_EN_CA) // RG_*RPI_RESERVE_B0[0] 1'b1 prevent leakage
                                        | P_Fld(1, SHU_CA_CMD9_RG_DLL_FAST_PSJP_CA) // by Master/Slave
                                        | P_Fld(u1HYST_SEL, SHU_CA_CMD9_RG_HYST_SEL_CA)
                                        | P_Fld(u1MIDPI_CAP_SEL2, SHU_CA_CMD9_RG_MIDPI_CAP_SEL_2_CA)
                                        | P_Fld(1, SHU_CA_CMD9_RG_8PH_XLAT_FORCE_CA)
                                        | P_Fld(u1PSMUX_XLAT_FORCE, SHU_CA_CMD9_RG_PSMUX_XLAT_FORCE_CA)
                                        | P_Fld(u1PSMUX_XLAT_FORCE, SHU_CA_CMD9_RG_PSMUX_XLAT_FORCE_DQS_CA)
                                        | P_Fld(u1Smt_Xlat_Force, SHU_CA_CMD9_RG_SMT_XLAT_FORCE_CA)
                                        | P_Fld(u1Smt_Xlat_Force, SHU_CA_CMD9_RG_SMT_XLAT_FORCE_DQS_CA)
                                        | P_Fld(1, SHU_CA_CMD9_RG_BUFGP_XLAT_FORCE_CA)
                                        | P_Fld(1, SHU_CA_CMD9_RG_BUFGPX_XLAT_FORCE_DQS_CA)
                                        | P_Fld(u1Bypass_SR, SHU_CA_CMD9_RG_BYPASS_SR_CA)
                                        | P_Fld(u1Bypass_SR, SHU_CA_CMD9_RG_BYPASS_SR_DQS_CA)
                                        | P_Fld(0, SHU_CA_CMD9_RG_CG_SYNC_ENB_CA)
                                        | P_Fld(u1LP3_SEL, SHU_CA_CMD9_RG_LP3_SEL_CA)
                                        | P_Fld(1, SHU_CA_CMD9_RG_MIDPI_DUMMY_ON_CA));


    vIO32WriteFldMulti(DDRPHY_SHU_CA_CMD9+SHIFT_TO_CHB_ADDR, P_Fld(0x1, SHU_CA_CMD9_RG_PI_TX_CG_EN_CA) // RG_*RPI_RESERVE_B0[0] 1'b1 prevent leakage
                                        | P_Fld(0, SHU_CA_CMD9_RG_DLL_FAST_PSJP_CA) // by Master/Slave
                                        | P_Fld(u1HYST_SEL, SHU_CA_CMD9_RG_HYST_SEL_CA)
                                        | P_Fld(u1MIDPI_CAP_SEL2, SHU_CA_CMD9_RG_MIDPI_CAP_SEL_2_CA)
                                        | P_Fld(1, SHU_CA_CMD9_RG_8PH_XLAT_FORCE_CA)
                                        | P_Fld(u1PSMUX_XLAT_FORCE, SHU_CA_CMD9_RG_PSMUX_XLAT_FORCE_CA)
                                        | P_Fld(u1PSMUX_XLAT_FORCE, SHU_CA_CMD9_RG_PSMUX_XLAT_FORCE_DQS_CA)
                                        | P_Fld(u1Smt_Xlat_Force, SHU_CA_CMD9_RG_SMT_XLAT_FORCE_CA)
                                        | P_Fld(u1Smt_Xlat_Force, SHU_CA_CMD9_RG_SMT_XLAT_FORCE_DQS_CA)
                                        | P_Fld(1, SHU_CA_CMD9_RG_BUFGP_XLAT_FORCE_CA)
                                        | P_Fld(1, SHU_CA_CMD9_RG_BUFGPX_XLAT_FORCE_DQS_CA)
                                        | P_Fld(u1Bypass_SR, SHU_CA_CMD9_RG_BYPASS_SR_CA)
                                        | P_Fld(u1Bypass_SR, SHU_CA_CMD9_RG_BYPASS_SR_DQS_CA)
                                        | P_Fld(0, SHU_CA_CMD9_RG_CG_SYNC_ENB_CA)
                                        | P_Fld(u1LP3_SEL, SHU_CA_CMD9_RG_LP3_SEL_CA)
                                        | P_Fld(1, SHU_CA_CMD9_RG_MIDPI_DUMMY_ON_CA));


    vIO32WriteFldMulti_All(DDRPHY_SHU_CA_DLL2, P_Fld(0x1, SHU_CA_DLL2_RG_TX_CG_DQ_LSH)
                                        | P_Fld(0x0, SHU_CA_DLL2_RG_TX_CG_DQS_LSH)
                                        | P_Fld(0x0, SHU_CA_DLL2_RG_TX_CG_DQM_LSH)
                                        | P_Fld(0x0, SHU_CA_DLL2_RG_RX_DQS_GATE_EN_MODE)
                                        | P_Fld(0x0, SHU_CA_DLL2_DA_RX_ARDQSIEN_RB_DLY)
                                        | P_Fld(u1SER_RST_MODE, SHU_CA_DLL2_RG_RX_SER_RST_MODE)
                                        | P_Fld(0x1, SHU_CA_DLL2_RG_MCK4X_SEL_0)
                                        | P_Fld(0x1, SHU_CA_DLL2_RG_MCK4X_SEL_1)
                                        | P_Fld(0x4, SHU_CA_DLL2_RG_TX_ARCKE_DRVN_B0)
                                        | P_Fld(0x0, SHU_CA_DLL2_RG_TX_ARDQ_DDR3_CKE_SEL)
                                        | P_Fld(0x0, SHU_CA_DLL2_RG_TX_ARDQ_DDR4_CKE_SEL)
                                        | P_Fld(0x0, SHU_CA_DLL2_RG_TX_ARDQ_DDR4P_CKE_SEL)
                                        | P_Fld(0x0, SHU_CA_DLL2_RG_TX_LP4Y_SEL)
                                        | P_Fld(0x0, SHU_CA_DLL2_RG_TX_LP4Y_EN)
                                        | P_Fld(0x0, SHU_CA_DLL2_RG_RX_DQSIEN_FORCE_ON_EN)
                                        | P_Fld(0x0, SHU_CA_DLL2_DATA_SWAP_EN)
                                        | P_Fld(0x0, SHU_CA_DLL2_DATA_SWAP)
                                        | P_Fld(0x0, SHU_CA_DLL2_FRATE_EN)
                                        | P_Fld(0x1, SHU_CA_DLL2_RG_MCK4X_SEL_2)
                                        | P_Fld(0x1, SHU_CA_DLL2_RG_MCK4X_SEL_3));

        
    vIO32WriteFldMulti_All(DDRPHY_SHU_B0_DQ9, P_Fld(0x1, SHU_B0_DQ9_RG_PI_TX_CG_EN_B0) // RG_*RPI_RESERVE_B0[0] 1'b1 prevent leakage
                                        | P_Fld(0, SHU_B0_DQ9_RG_DLL_FAST_PSJP_B0)
                                        | P_Fld(u1HYST_SEL, SHU_B0_DQ9_RG_HYST_SEL_B0)
                                        | P_Fld(u1MIDPI_CAP_SEL2, SHU_B0_DQ9_RG_MIDPI_CAP_SEL_2_B0)
                                        | P_Fld(1, SHU_B0_DQ9_RG_8PH_XLAT_FORCE_B0)
                                        | P_Fld(u1PSMUX_XLAT_FORCE, SHU_B0_DQ9_RG_PSMUX_XLAT_FORCE_B0)
                                        | P_Fld(u1PSMUX_XLAT_FORCE, SHU_B0_DQ9_RG_PSMUX_XLAT_FORCE_DQS_B0)
                                        | P_Fld(u1Smt_Xlat_Force, SHU_B0_DQ9_RG_SMT_XLAT_FORCE_B0)
                                        | P_Fld(u1Smt_Xlat_Force, SHU_B0_DQ9_RG_SMT_XLAT_FORCE_DQS_B0)
                                        | P_Fld(1, SHU_B0_DQ9_RG_BUFGP_XLAT_FORCE_B0)
                                        | P_Fld(1, SHU_B0_DQ9_RG_BUFGPX_XLAT_FORCE_DQS_B0)
                                        | P_Fld(u1Bypass_SR, SHU_B0_DQ9_RG_BYPASS_SR_B0)
                                        | P_Fld(u1Bypass_SR, SHU_B0_DQ9_RG_BYPASS_SR_DQS_B0)
                                        | P_Fld(0, SHU_B0_DQ9_RG_CG_SYNC_ENB_B0)
                                        | P_Fld(u1LP3_SEL, SHU_B0_DQ9_RG_LP3_SEL_B0)
                                        | P_Fld(1, SHU_B0_DQ9_RG_MIDPI_DUMMY_ON_B0));

    
    vIO32WriteFldMulti_All(DDRPHY_SHU_B1_DQ9, P_Fld(0x1, SHU_B1_DQ9_RG_PI_TX_CG_EN_B1) // RG_*RPI_RESERVE_B0[0] 1'b1 prevent leakage
                                        | P_Fld(0, SHU_B1_DQ9_RG_DLL_FAST_PSJP_B1)
                                        | P_Fld(u1HYST_SEL, SHU_B1_DQ9_RG_HYST_SEL_B1)
                                        | P_Fld(u1MIDPI_CAP_SEL2, SHU_B1_DQ9_RG_MIDPI_CAP_SEL_2_B1)
                                        | P_Fld(1, SHU_B1_DQ9_RG_8PH_XLAT_FORCE_B1)
                                        | P_Fld(u1PSMUX_XLAT_FORCE, SHU_B1_DQ9_RG_PSMUX_XLAT_FORCE_B1)
                                        | P_Fld(u1PSMUX_XLAT_FORCE, SHU_B1_DQ9_RG_PSMUX_XLAT_FORCE_DQS_B1)
                                        | P_Fld(u1Smt_Xlat_Force, SHU_B1_DQ9_RG_SMT_XLAT_FORCE_B1)
                                        | P_Fld(u1Smt_Xlat_Force, SHU_B1_DQ9_RG_SMT_XLAT_FORCE_DQS_B1)
                                        | P_Fld(1, SHU_B1_DQ9_RG_BUFGP_XLAT_FORCE_B1)
                                        | P_Fld(1, SHU_B1_DQ9_RG_BUFGPX_XLAT_FORCE_DQS_B1)
                                        | P_Fld(u1Bypass_SR, SHU_B1_DQ9_RG_BYPASS_SR_B1)
                                        | P_Fld(u1Bypass_SR, SHU_B1_DQ9_RG_BYPASS_SR_DQS_B1)
                                        | P_Fld(0, SHU_B1_DQ9_RG_CG_SYNC_ENB_B1)
                                        | P_Fld(u1LP3_SEL, SHU_B1_DQ9_RG_LP3_SEL_B1)
                                        | P_Fld(1, SHU_B1_DQ9_RG_MIDPI_DUMMY_ON_B1));


    //TX
    vIO32WriteFldMulti_All(DDRPHY_SHU_B0_DLL1, P_Fld(0x0, SHU_B0_DLL1_DQS_MCK4X_DLY_EN_B0)
                                        | P_Fld(0x0, SHU_B0_DLL1_DQS_MCK4XB_DLY_EN_B0)
                                        | P_Fld(u1TX_READ_BASE_EN, SHU_B0_DLL1_RG_READ_BASE_DQSB_EN_B0)
                                        | P_Fld(0x0, SHU_B0_DLL1_RG_RX_DQS_GATE_EN_MODE_B0)
                                        | P_Fld(0x0, SHU_B0_DLL1_DA_RX_ARDQSIEN_RB_DLY_B0)
                                        | P_Fld(u1SER_RST_MODE, SHU_B0_DLL1_RG_RX_SER_RST_MODE_B0)
                                        | P_Fld(0x0, SHU_B0_DLL1_RG_MCK4X_SEL_B0)
                                        | P_Fld(0x0, SHU_B0_DLL1_RG_ODT_DISABLE_B0)
                                        | P_Fld(u1TX_READ_BASE_EN, SHU_B0_DLL1_RG_READ_BASE_DQS_EN_B0)
                                        | P_Fld(0x0, SHU_B0_DLL1_RG_TX_PRE_EN_B0)
                                        | P_Fld(0x0, SHU_B0_DLL1_RG_TX_LP4Y_SEL_B0)
                                        | P_Fld(0x0, SHU_B0_DLL1_RG_TX_LP4Y_EN_B0)
                                        | P_Fld(0x0, SHU_B0_DLL1_RG_RX_DQSIEN_FORCE_ON_EN_B0)
                                        | P_Fld(0x0, SHU_B0_DLL1_DATA_SWAP_B0)
                                        | P_Fld(0x0, SHU_B0_DLL1_DATA_SWAP_B0)
                                        | P_Fld(0x0, SHU_B0_DLL1_FRATE_EN_B0));

                                        
    vIO32WriteFldMulti_All(DDRPHY_SHU_B1_DLL1, P_Fld(0x0, SHU_B1_DLL1_DQS_MCK4X_DLY_EN_B1)
                                        | P_Fld(0x0, SHU_B1_DLL1_DQS_MCK4XB_DLY_EN_B1)
                                        | P_Fld(u1TX_READ_BASE_EN, SHU_B1_DLL1_RG_READ_BASE_DQSB_EN_B1)
                                        | P_Fld(0x0, SHU_B1_DLL1_RG_RX_DQS_GATE_EN_MODE_B1)
                                        | P_Fld(0x0, SHU_B1_DLL1_DA_RX_ARDQSIEN_RB_DLY_B1)
                                        | P_Fld(u1SER_RST_MODE, SHU_B1_DLL1_RG_RX_SER_RST_MODE_B1)
                                        | P_Fld(0x0, SHU_B1_DLL1_RG_MCK4X_SEL_B1)
                                        | P_Fld(0x0, SHU_B1_DLL1_RG_ODT_DISABLE_B1)
                                        | P_Fld(u1TX_READ_BASE_EN, SHU_B1_DLL1_RG_READ_BASE_DQS_EN_B1)
                                        | P_Fld(0x0, SHU_B1_DLL1_RG_TX_PRE_EN_B1)
                                        | P_Fld(0x0, SHU_B1_DLL1_RG_TX_LP4Y_SEL_B1)
                                        | P_Fld(0x0, SHU_B1_DLL1_RG_TX_LP4Y_EN_B1)
                                        | P_Fld(0x0, SHU_B1_DLL1_RG_RX_DQSIEN_FORCE_ON_EN_B1)
                                        | P_Fld(0x0, SHU_B1_DLL1_DATA_SWAP_EN_B1)
                                        | P_Fld(0x0, SHU_B1_DLL1_DATA_SWAP_B1)
                                        | P_Fld(0x0, SHU_B1_DLL1_FRATE_EN_B1));

    #if TX_DQ_PRE_EMPHASIS // Darren: Need confirm for DDR4266 up
    if((p->frequency > 1600) && (gDramcSwImpedanceResult[0][DRVP] > 0))
    {
        vIO32WriteFldAlign_All(DDRPHY_SHU_B0_DLL1, 0x1, SHU_B0_DLL1_RG_TX_PRE_EN_B0); 
        vIO32WriteFldAlign_All(DDRPHY_SHU_B1_DLL1, 0x1, SHU_B1_DLL1_RG_TX_PRE_EN_B1); 

        mcSHOW_DBG_MSG(("[TX_DQ_PRE_EMPHASIS] Enable TX DQ pre-emphasis driving\n"));
    }
    #endif                                  

    #if 0
    //LP4-Init
    vIO32WriteFldAlign(DDRPHY_SHU_B0_DQ3, 0x0, SHU_B0_DQ3_RG_ARDQ_REV_B0); 
    vIO32WriteFldAlign(DDRPHY_SHU_B1_DQ3, 0x0, SHU_B1_DQ3_RG_ARDQ_REV_B1); 

    //gating
    vIO32WriteFldAlign(DDRPHY_SHU_B0_DQ3, 0x20, SHU_B0_DQ3_RG_ARDQ_REV_B0);
    vIO32WriteFldAlign(DDRPHY_SHU_B1_DQ3, 0x20, SHU_B1_DQ3_RG_ARDQ_REV_B1);
    vIO32WriteFldAlign(DDRPHY_SHU_CA_CMD3, 0x09E0, SHU_CA_CMD3_RG_ARCMD_REV);


    //LP3-INIT
    vIO32WriteFldAlign(DDRPHY_SHU_CA_CMD3, 0x7e2, SHU_CA_CMD3_RG_ARCMD_REV);
    vIO32WriteFldAlign(DDRPHY_SHU_CA_CMD3+SHIFT_TO_CHB_ADDR, 0x720, SHU_CA_CMD3_RG_ARCMD_REV);
    #endif
}

#if XRTRTR_NEW_CROSS_RK_MODE
void XRTRTR_SHU_Setting(DRAMC_CTX_T * p)
{
    U8 u1ShuRkMode = 0, u1RkSelUIMinu = 1, u1RkSelSerMode = 3;

    if(vGet_Div_Mode(p) == DIV4_MODE)
    {
        u1ShuRkMode = 1;
        u1RkSelUIMinu = 2;
        u1RkSelSerMode = 2;
    }
    vIO32WriteFldAlign_All(DDRPHY_SHU_B0_DQ6, u1ShuRkMode, SHU_B0_DQ6_RG_RX_ARDQ_RANK_SEL_SER_MODE_B0);
    vIO32WriteFldAlign_All(DDRPHY_SHU_B1_DQ6, u1ShuRkMode, SHU_B1_DQ6_RG_RX_ARDQ_RANK_SEL_SER_MODE_B1);
    //DRAMC setting
    vIO32WriteFldMulti_All(DRAMC_REG_SHU_RANK_SEL_STB, P_Fld(0x0, SHU_RANK_SEL_STB_RANK_SEL_STB_MCK_MINUS)
                                       | P_Fld(u1RkSelUIMinu, SHU_RANK_SEL_STB_RANK_SEL_STB_UI_MINUS)
                                       | P_Fld(0x0, SHU_RANK_SEL_STB_RANK_SEL_STB_MCK_PLUS)
                                       | P_Fld(0x0, SHU_RANK_SEL_STB_RANK_SEL_STB_UI_PLUS)
                                       | P_Fld(0x1, SHU_RANK_SEL_STB_RANK_SEL_STB_PHASE_EN)
                                       | P_Fld(0x0, SHU_RANK_SEL_STB_RANK_SEL_RXDLY_TRACK)
                                       | P_Fld(0x1, SHU_RANK_SEL_STB_RANK_SEL_STB_TRACK)
                                       | P_Fld(u1RkSelSerMode, SHU_RANK_SEL_STB_RANK_SEL_STB_SERMODE)
                                       | P_Fld(0x0, SHU_RANK_SEL_STB_RANK_SEL_STB_EN_B23)
                                       | P_Fld(0x1, SHU_RANK_SEL_STB_RANK_SEL_STB_EN));	
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_STBCAL, 0x1, SHU_STBCAL_DQSIEN_RX_SELPH_OPT);
	
    vIO32WriteFldMulti_All(DRAMC_REG_SHURK0_DQSCAL, P_Fld(0x1, SHURK0_DQSCAL_R0DQSIENHLMTEN)
                                       | P_Fld(0x3f, SHURK0_DQSCAL_R0DQSIENHLMT)
                                       | P_Fld(0x1, SHURK0_DQSCAL_R0DQSIENLLMTEN)
                                       | P_Fld(0x60, SHURK0_DQSCAL_R0DQSIENLLMT));
    vIO32WriteFldMulti_All(DRAMC_REG_SHURK1_DQSCAL, P_Fld(0x1, SHURK1_DQSCAL_R1DQSIENHLMTEN)
                                       | P_Fld(0x3f, SHURK1_DQSCAL_R1DQSIENHLMT)
                                       | P_Fld(0x1, SHURK1_DQSCAL_R1DQSIENLLMTEN)
                                       | P_Fld(0x60, SHURK1_DQSCAL_R1DQSIENLLMT));

}
void ENABLE_XRTRTR_Setting(DRAMC_CTX_T * p)
{
    //PHY setting
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI1, 0x1, B0_DLL_ARPI1_RG_ARPI_MCTL_JUMP_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI3, 0x1, B0_DLL_ARPI3_RG_ARPI_MCTL_EN_B0);

    vIO32WriteFldMulti_All(DDRPHY_B0_DQ3, P_Fld(0x1, B0_DQ3_RG_ARPI_PD_MCTL_SEL_B0)
                                       | P_Fld(0x1, B0_DQ3_RG_ARPI_OFFSET_LAT_EN_B0)
                                       | P_Fld(0x1, B0_DQ3_RG_ARPI_OFFSET_ASYNC_EN_B0)
                                       | P_Fld(0x1, B0_DQ3_RG_RX_ARDQSIEN_RANK_SEL_LAT_EN_B0)
                                       | P_Fld(0x1, B0_DQ3_RG_RX_ARDQS0_RANK_SEL_LAT_EN_B0)
                                       | P_Fld(0x1, B0_DQ3_RG_RX_ARDQ_RANK_SEL_SER_EN_B0)
                                       | P_Fld(0x1, B0_DQ3_RG_RX_ARDQ_RANK_SEL_LAT_EN_B0)
                                       | P_Fld(0x1, B0_DQ3_RG_TX_ARDQ_DLY_LAT_EN_B0));
	
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI1, 0x1, B1_DLL_ARPI1_RG_ARPI_MCTL_JUMP_EN_B1);
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI3, 0x1, B1_DLL_ARPI3_RG_ARPI_MCTL_EN_B1);
	
    vIO32WriteFldMulti_All(DDRPHY_B1_DQ3, P_Fld(0x1, B1_DQ3_RG_ARPI_PD_MCTL_SEL_B1)
                                       | P_Fld(0x1, B1_DQ3_RG_ARPI_OFFSET_LAT_EN_B1)
                                       | P_Fld(0x1, B1_DQ3_RG_ARPI_OFFSET_ASYNC_EN_B1)
                                       | P_Fld(0x1, B1_DQ3_RG_RX_ARDQSIEN_RANK_SEL_LAT_EN_B1)
                                       | P_Fld(0x1, B1_DQ3_RG_RX_ARDQS0_RANK_SEL_LAT_EN_B1)
                                       | P_Fld(0x1, B1_DQ3_RG_RX_ARDQ_RANK_SEL_SER_EN_B1)
                                       | P_Fld(0x1, B1_DQ3_RG_RX_ARDQ_RANK_SEL_LAT_EN_B1)
                                       | P_Fld(0x1, B1_DQ3_RG_TX_ARDQ_DLY_LAT_EN_B1));	
	
    vIO32WriteFldMulti_All(DDRPHY_B0_DQ9, P_Fld(0x0, B0_DQ9_R_IN_GATE_EN_LOW_OPT_B0) | P_Fld(0x1, B0_DQ9_R_DMRXDVS_R_F_DLY_RK_OPT_B0));
    vIO32WriteFldMulti_All(DDRPHY_B1_DQ9, P_Fld(0x0, B1_DQ9_R_IN_GATE_EN_LOW_OPT_B1) | P_Fld(0x1, B1_DQ9_R_DMRXDVS_R_F_DLY_RK_OPT_B1));
    vIO32WriteFldMulti_All(DDRPHY_CA_CMD10, P_Fld(0, CA_CMD10_R_IN_GATE_EN_LOW_OPT_CA) | P_Fld(0, CA_CMD10_R_DMRXDVS_R_F_DLY_RK_OPT));
	vIO32WriteFldAlign_All(DDRPHY_MISC_CTRL3, 0x1, MISC_CTRL3_ARPI_CG_RK1_SRC_SEL);
	
    vIO32WriteFldMulti_All(DRAMC_REG_STBCAL2, P_Fld(0x1, STBCAL2_DQSIEN_SELPH_BY_RANK_EN)
                                       | P_Fld(0x1, STBCAL2_STB_RST_BY_RANK)
                                       | P_Fld(0x1, STBCAL2_STB_IG_XRANK_CG_RST));	
    //Extend 1T timing of FIFO mode rank switch 
    vIO32WriteFldAlign_All(DDRPHY_B0_DQ9, 0x2, B0_DQ9_R_DMDQSIEN_RDSEL_LAT_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DQ9, 0x2, B1_DQ9_R_DMDQSIEN_RDSEL_LAT_B1);
    vIO32WriteFldAlign_All(DDRPHY_B0_DQ9, 0x1, B0_DQ9_R_DMDQSIEN_VALID_LAT_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DQ9, 0x1, B1_DQ9_R_DMDQSIEN_VALID_LAT_B1);
}
#endif

#if XRTWTW_NEW_CROSS_RK_MODE
void XRTWTW_SHU_Setting(DRAMC_CTX_T * p)
{
    U8 u1RankIdx, u1ByteIdx;
    U8 u1Rank_bak = u1GetRank(p);
    U16 u2TxDly_OEN_RK[2][2]={0}, u2TxPI_UPD[2]={0}, u2TxRankINCTL, u2TxDly_OEN_RK_max, u2TxPI_UPD_max;

    for(u1RankIdx=RANK_0; u1RankIdx<p->support_rank_num; u1RankIdx++)
    {
        vSetRank(p, u1RankIdx);
			
        u2TxDly_OEN_RK[u1RankIdx][0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0), SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0); //Byte0
        u2TxDly_OEN_RK[u1RankIdx][1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0), SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1); //Byte1
    }
    vSetRank(p, u1Rank_bak);

	for(u1ByteIdx =0 ; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
    {
        u2TxDly_OEN_RK_max = (u2TxDly_OEN_RK[0][u1ByteIdx] > u2TxDly_OEN_RK[1][u1ByteIdx])? u2TxDly_OEN_RK[0][u1ByteIdx] : u2TxDly_OEN_RK[1][u1ByteIdx];
        u2TxPI_UPD[u1ByteIdx] = (u2TxDly_OEN_RK_max > 1)? (u2TxDly_OEN_RK_max - 1) : 0; //Byte0
    }

    u2TxPI_UPD_max = (u2TxPI_UPD[0] > u2TxPI_UPD[1])? u2TxPI_UPD[0] : u2TxPI_UPD[1];
    u2TxRankINCTL = (u2TxPI_UPD_max > 1)? (u2TxPI_UPD_max - 1) : 0;

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_DQSOSCR), P_Fld(0x0, SHU_DQSOSCR_TXPI_UPD_MODE)
                                           | P_Fld(u2TxPI_UPD[0], SHU_DQSOSCR_TX_PI_UPDCTL_B0)
                                           | P_Fld(u2TxPI_UPD[1], SHU_DQSOSCR_TX_PI_UPDCTL_B1));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_RANKCTL), P_Fld(0x0, SHU_RANKCTL_TXRANKINCTL_ROOT)
                                            | P_Fld(u2TxRankINCTL, SHU_RANKCTL_TXRANKINCTL)
                                            | P_Fld(u2TxRankINCTL, SHU_RANKCTL_TXRANKINCTL_TXDLY));
}

void ENABLE_XRTWTW_Setting(DRAMC_CTX_T * p)
{
    vIO32WriteFldMulti_All(DDRPHY_B0_DQ3, P_Fld(0x1, B0_DQ3_RG_ARPI_OFFSET_ASYNC_EN_B0)
                                       | P_Fld(0x1, B0_DQ3_RG_ARPI_OFFSET_LAT_EN_B0)
                                       | P_Fld(0x1, B0_DQ3_RG_TX_ARDQ_DLY_LAT_EN_B0));
	
    vIO32WriteFldMulti_All(DDRPHY_B1_DQ3, P_Fld(0x1, B1_DQ3_RG_ARPI_OFFSET_ASYNC_EN_B1)
                                       | P_Fld(0x1, B1_DQ3_RG_ARPI_OFFSET_LAT_EN_B1)
                                       | P_Fld(0x1, B1_DQ3_RG_TX_ARDQ_DLY_LAT_EN_B1));	
	
    vIO32WriteFldMulti_All(DDRPHY_CA_CMD3, P_Fld(0x0, CA_CMD3_RG_ARPI_OFFSET_ASYNC_EN_CA)
                                       | P_Fld(0x0, CA_CMD3_RG_ARPI_OFFSET_LAT_EN_CA)
                                       | P_Fld(0x0, CA_CMD3_RG_TX_ARCMD_DLY_LAT_EN));
}
#endif

#if CMD_CKE_WORKAROUND_FIX
void CMD_CKE_Modified_txp_Setting(DRAMC_CTX_T * p)
{
    U8 u1CmdTxPipe=0, u1CkTxPipe=0, u1SrefPdSel=0;
	
    if(vGet_Div_Mode(p) == DIV4_MODE)
    {
        u1CkTxPipe = 1;
        u1SrefPdSel = 1;
    }
    else if(p->frequency >= 1866)
    {
        u1CmdTxPipe = 1;
        u1CkTxPipe = 1;
    }
	
    vIO32WriteFldAlign(DDRPHY_SHU_MISC0, u1CkTxPipe, SHU_MISC0_RG_CK_TXPIPE_BYPASS_EN);
    vIO32WriteFldAlign(DDRPHY_SHU_MISC0, u1CmdTxPipe, SHU_MISC0_RG_CMD_TXPIPE_BYPASS_EN);
    vIO32WriteFldAlign(DRAMC_REG_SHU_CONF0, u1SrefPdSel, SHU_CONF0_SREF_PD_SEL);
}
#endif

#if TX_OE_EXTEND
static void UpdateTxOEN(DRAMC_CTX_T *p)
{
    U8 u1ByteIdx, backup_rank, ii;
    U8 u1DQ_OE_CNT;
    
    // For LP4
    // 1. R_DMDQOE_OPT (dramc_conf 0x8C0[11])
    //    set 1'b1: adjust DQSOE/DQOE length with R_DMDQOE_CNT
    // 2. R_DMDQOE_CNT (dramc_conf 0x8C0[10:8])
    //    set 3'h3
    // 3. Initial TX setting OE/DATA
    //    OE = DATA - 4 UI

    // For LP3
    // 1. R_DMDQOE_OPT (dramc_conf 0x8C0[11])
    //    set 1'b1: adjust DQSOE/DQOE length with R_DMDQOE_CNT
    // 2. R_DMDQOE_CNT (dramc_conf 0x8C0[10:8])
    //    set 3'h2
    // 3. Initial TX setting OE/DATA
    //    OE = DATA - 2 UI
    
    u1DQ_OE_CNT =3;
    
    vIO32WriteFldMulti(DRAMC_REG_SHU_WODT, P_Fld(1, SHU_WODT_DQOE_OPT) | P_Fld(u1DQ_OE_CNT, SHU_WODT_DQOE_CNT));

    backup_rank = u1GetRank(p);
    for(u1ByteIdx =0 ; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
    {
        MoveDramC_TX_DQS_OEN(p, u1ByteIdx, -1);

        for(ii=RANK_0; ii<RANK_MAX; ii++)
        {
            vSetRank(p, ii);
            MoveDramC_TX_DQ_OEN(p, u1ByteIdx, -1);
        }
        vSetRank(p, backup_rank);
    }
}
#endif

static DRAM_STATUS_T UpdateInitialSettings_LP4(DRAMC_CTX_T *p)
{
    U16 u2RXVrefDefault=0x8;
    U16 u1ChannelIdx, u1RankIdx, u1ByteIdx;
    U16 u2Clk_Dyn_Gating_Sel=0x4, u2CG_CK_SEL=0xb;
    #if CBT_MOVE_CA_INSTEAD_OF_CLK
    U8 u1CaPI=0, u1CaUI=0;
    #endif
    U32 u4RsvEMI=0;
    U8 u1RsvEco1=0;

    if(p->odt_onoff==ODT_ON)
    {
        vIO32WriteFldAlign(DRAMC_REG_SHU_ODTCTRL, 1, SHU_ODTCTRL_ROEN);
        vIO32WriteFldAlign(DDRPHY_SHU_B0_DQ7, 0x1, SHU_B0_DQ7_R_DMRODTEN_B0);
        vIO32WriteFldAlign(DDRPHY_SHU_B1_DQ7, 0x1, SHU_B1_DQ7_R_DMRODTEN_B1);
        vIO32WriteFldMulti(DDRPHY_SHU_CA_CMD0, P_Fld(0x0, SHU_CA_CMD0_RG_TX_ARCMD_PRE_EN) // OE Suspend EN
                | P_Fld(0x1, SHU_CA_CMD0_RG_TX_ARCLK_PRE_EN)); //ODT Suspend EN
    }
    else
    {
        vIO32WriteFldAlign(DRAMC_REG_SHU_ODTCTRL, 0, SHU_ODTCTRL_ROEN);
        vIO32WriteFldAlign(DDRPHY_SHU_B0_DQ7, 0x0, SHU_B0_DQ7_R_DMRODTEN_B0);
        vIO32WriteFldAlign(DDRPHY_SHU_B1_DQ7, 0x0, SHU_B1_DQ7_R_DMRODTEN_B1);
        vIO32WriteFldMulti(DDRPHY_SHU_CA_CMD0, P_Fld(0x0, SHU_CA_CMD0_RG_TX_ARCMD_PRE_EN) // OE Suspend EN
                | P_Fld(0x0, SHU_CA_CMD0_RG_TX_ARCLK_PRE_EN)); //ODT Suspend EN
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
    //wei-jen: RX rank_sel for CA is not used(Bi_anco), set it's dly to 0 to save power
    vIO32WriteFldAlign(DDRPHY_SHU_CA_CMD7, 0, SHU_CA_CMD7_R_DMRANKRXDVS_CA); //Move to DCM off setting


    //DDRPhyTxRxInitialSettings_LP4
    vIO32WriteFldAlign(DDRPHY_CA_CMD3, 0x1, CA_CMD3_RG_RX_ARCMD_STBENCMP_EN);

    vIO32WriteFldAlign(DDRPHY_CA_CMD10, 0x1, CA_CMD10_RG_RX_ARCLK_DQSIENMODE);

    vIO32WriteFldAlign(DDRPHY_CA_CMD6, 0x2, CA_CMD6_RG_RX_ARCMD_BIAS_VREF_SEL);

    vIO32WriteFldMulti(DDRPHY_B0_DQ3, P_Fld(0x1, B0_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B0)
                        | P_Fld(0x1, B0_DQ3_RG_RX_ARDQM0_IN_BUFF_EN_B0)
                        | P_Fld(0x1, B0_DQ3_RG_RX_ARDQS0_IN_BUFF_EN_B0));
    vIO32WriteFldMulti(DDRPHY_B1_DQ3, P_Fld(0x1, B1_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B1)
                        | P_Fld(0x1, B1_DQ3_RG_RX_ARDQM0_IN_BUFF_EN_B1)
                        | P_Fld(0x1, B1_DQ3_RG_RX_ARDQS0_IN_BUFF_EN_B1));
    vIO32WriteFldMulti(DDRPHY_CA_CMD3, P_Fld(0x1, CA_CMD3_RG_RX_ARCMD_IN_BUFF_EN)
                            | P_Fld(0x1, CA_CMD3_RG_RX_ARCLK_IN_BUFF_EN));

    vIO32WriteFldAlign(DDRPHY_B0_DQ3, 0x0, B0_DQ3_RG_RX_ARDQ_SMT_EN_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ3, 0x0, B1_DQ3_RG_RX_ARDQ_SMT_EN_B1);

    vIO32WriteFldAlign(DDRPHY_B0_DQ5, 0x1, B0_DQ5_RG_RX_ARDQS0_DVS_EN_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ5, 0x1, B1_DQ5_RG_RX_ARDQS0_DVS_EN_B1);
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


    vIO32WriteFldAlign(DDRPHY_B0_DQ6, 0x0, B0_DQ6_RG_RX_ARDQ_O1_SEL_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ6, 0x0, B1_DQ6_RG_RX_ARDQ_O1_SEL_B1);
    vIO32WriteFldAlign(DDRPHY_CA_CMD6, 0x0, CA_CMD6_RG_RX_ARCMD_O1_SEL);

    vIO32WriteFldAlign(DDRPHY_B0_DQ6, 0x1, B0_DQ6_RG_RX_ARDQ_BIAS_PS_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ6, 0x1, B1_DQ6_RG_RX_ARDQ_BIAS_PS_B1);
    vIO32WriteFldAlign(DDRPHY_CA_CMD6, 0x1, CA_CMD6_RG_RX_ARCMD_BIAS_PS);
    vIO32WriteFldAlign(DDRPHY_CA_CMD6, 0x1, CA_CMD6_RG_RX_ARCMD_RES_BIAS_EN);

    vIO32WriteFldAlign(DDRPHY_B0_DQ6, 0x0, B0_DQ6_RG_TX_ARDQ_ODTEN_EXT_DIS_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ6, 0x0, B1_DQ6_RG_TX_ARDQ_ODTEN_EXT_DIS_B1);
    vIO32WriteFldAlign(DDRPHY_CA_CMD6, 0x0, CA_CMD6_RG_TX_ARCMD_ODTEN_EXT_DIS);

    vIO32WriteFldAlign(DDRPHY_B0_DQ6, 0x1, B0_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ6, 0x1, B1_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B1);
    vIO32WriteFldAlign(DDRPHY_CA_CMD6, 0x1, CA_CMD6_RG_RX_ARCMD_RPRE_TOG_EN);

    if (p->dram_type == TYPE_LPDDR4)
    {
        if(p->odt_onoff==ODT_ON)
        {
            u2RXVrefDefault = 0x17; // 0.6*VDDQ, 0x12=0.5*VDDQ
        }
        else
        {
            u2RXVrefDefault = 0x29;
        }
    }
    else if (p->dram_type == TYPE_LPDDR4X)
    {
        if(p->odt_onoff==ODT_ON)
        {
            u2RXVrefDefault = 0x17; // 0.6*VDDQ, 0x12=0.5*VDDQ
        }
        else
        {
            u2RXVrefDefault = 0x29;
        }
    }
    else //if(p->dram_type == TYPE_LPDDR4P)
    {
        u2RXVrefDefault = 0x10;
    }

    #if CALIBRATION_SPEED_UP_DEBUG
    mcSHOW_DBG_MSG(("\nInit Yulia RX Vref %d, apply to both rank0 and 1\n", u2RXVrefDefault));
    #endif

    vIO32WriteFldAlign(DDRPHY_SHU_B0_DQ5, u2RXVrefDefault, SHU_B0_DQ5_RG_RX_ARDQ_VREF_SEL_B0);
    vIO32WriteFldAlign(DDRPHY_SHU_B1_DQ5, u2RXVrefDefault, SHU_B1_DQ5_RG_RX_ARDQ_VREF_SEL_B1);
    vIO32WriteFldAlign(DDRPHY_B0_DQ5, u2RXVrefDefault, B0_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ5, u2RXVrefDefault, B1_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B1);
    
    for(u1ChannelIdx=CHANNEL_A; u1ChannelIdx<CHANNEL_NUM; u1ChannelIdx++)
    {
        for(u1RankIdx = RANK_0; u1RankIdx < RANK_MAX; u1RankIdx++)
        {
            for(u1ByteIdx =0 ; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
            {
                gFinalRXVrefDQ[u1ChannelIdx][u1RankIdx][u1ByteIdx] = u2RXVrefDefault;
            }
        }
    }

    if((p->dram_type == TYPE_LPDDR4X) || (p->dram_type == TYPE_LPDDR4P))
    {
        // LP4x eye fine-tune
        // APHY Review by YY Hsu
        vIO32WriteFldAlign(DDRPHY_B0_DQ8, 0x1, B0_DQ8_RG_TX_ARDQ_EN_LP4P_B0);
        vIO32WriteFldAlign(DDRPHY_B1_DQ8, 0x1, B1_DQ8_RG_TX_ARDQ_EN_LP4P_B1);
        vIO32WriteFldAlign(DDRPHY_CA_CMD9, 0x1, CA_CMD9_RG_TX_ARCMD_EN_LP4P);
    }


    /* Set initial default mode to "new burst mode (7UI or new 8UI)" */
    DramcGatingMode(p, 1);

    vIO32WriteFldAlign(DDRPHY_CA_CMD8, 0x1, CA_CMD8_RG_TX_RRESETB_DDR3_SEL);
    vIO32WriteFldAlign(DDRPHY_CA_CMD8, 0x0, CA_CMD8_RG_TX_RRESETB_DDR4_SEL); //TODO: Remove if register default value is 0
    //End of DDRPhyTxRxInitialSettings_LP4

    //DFS workaround
    vIO32WriteFldAlign(DRAMC_REG_SHU_MISC, 0x2, SHU_MISC_REQQUE_MAXCNT);

    //should set 0x2a, otherwise AC-timing violation from Berson
    vIO32WriteFldMulti(DRAMC_REG_SHU_DQSG, P_Fld(0x2a, SHU_DQSG_SCINTV) | P_Fld(0x1, SHU_DQSG_DQSINCTL_PRE_SEL));


    //Update setting for Bi_anco
    vIO32WriteFldAlign(DDRPHY_SHU_B0_DQ5, 0x0, SHU_B0_DQ5_RG_ARPI_FB_B0);
    vIO32WriteFldAlign(DDRPHY_SHU_B1_DQ5, 0x0, SHU_B1_DQ5_RG_ARPI_FB_B1);
    vIO32WriteFldAlign(DDRPHY_SHU_CA_CMD5, 0x0, SHU_CA_CMD5_RG_ARPI_FB_CA);


    //Reserved bits usage, check with PHY owners
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    vIO32WriteFldAlign_All(DDRPHY_SHU_B0_DQ6, 0x0, SHU_B0_DQ6_RG_ARPI_OFFSET_DQSIEN_B0);
    vIO32WriteFldAlign_All(DDRPHY_SHU_B1_DQ6, 0x0, SHU_B1_DQ6_RG_ARPI_OFFSET_DQSIEN_B1);
    vIO32WriteFldMulti_All(DDRPHY_SHU_CA_CMD6, P_Fld(0x0, SHU_CA_CMD6_RG_ARPI_OFFSET_CLKIEN)
                                        | P_Fld(0x0, SHU_CA_CMD6_RG_ARPI_OFFSET_MCTL_CA));
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);

    //IMP Tracking Init Settings
    //Write (DRAMC _BASE+ 0x219) [31:0] = 32'h80080020//DDR3200 default
    //SHU_IMPCAL1_IMPCAL_CHKCYCLE should > 12.5/MCK, 1:4 mode will disable imp tracking -> don't care
    // @Maoauo, Darren
    // SHU_IMPCAL1_IMPCAL_CALICNT=0 & SHU_IMPCAL1_IMPCAL_CALEN_CYCLE=0 are no use. (Using default settings)
    // SHU_IMPCAL1_IMPCALCNT_OPT=0 for IMP tracking mode optimization (Using default settings)
    vIO32WriteFldMulti(DRAMC_REG_SHU_IMPCAL1, P_Fld(0x10, SHU_IMPCAL1_IMPCALCNT) 
                                            | P_Fld(1, SHU_IMPCAL1_IMPCALCNT_OPT)
                                            | P_Fld((p->frequency*25/8000)+1, SHU_IMPCAL1_IMPCAL_CHKCYCLE));

    //for _K_
    vIO32WriteFldMulti(DRAMC_REG_SREFCTRL, P_Fld(0x1, SREFCTRL_SCSM_CGAR)
                                            | P_Fld(0x1, SREFCTRL_SCARB_SM_CGAR)
                                            | P_Fld(0x1, SREFCTRL_RDDQSOSC_CGAR)
                                            | P_Fld(0x1, SREFCTRL_HMRRSEL_CGAR));
    vIO32WriteFldAlign(DRAMC_REG_PRE_TDQSCK1, 0x1, PRE_TDQSCK1_TXUIPI_CAL_CGAR);
    /* DVFS related, PREA interval counter (After DVFS DVT, set to 0xf (originally was 0x1f)) */
    vIO32WriteFldAlign(DRAMC_REG_SHU_MISC, 0xf, SHU_MISC_PREA_INTV);
    vIO32WriteFldMulti(DDRPHY_SHU_B0_DQ8, P_Fld(0x1, SHU_B0_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU_B0_DQ8_R_DMRANK_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU_B0_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU_B0_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU_B0_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU_B0_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B0)
                                        | P_Fld(0x1, SHU_B0_DQ8_R_DMSTBEN_SYNC_CG_IG_B0)
                                        | P_Fld(0x1, SHU_B0_DQ8_R_DMRXDLY_CG_IG_B0)
                                        | P_Fld(0x1, SHU_B0_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B0)
                                        | P_Fld(0x1, SHU_B0_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B0)
                                        | P_Fld(0x0, SHU_B0_DQ8_R_DMRXDVS_UPD_FORCE_EN_B0)
                                        | P_Fld(0x7fff, SHU_B0_DQ8_R_DMRXDVS_UPD_FORCE_CYC_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_B1_DQ8, P_Fld(0x1, SHU_B1_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU_B1_DQ8_R_DMRANK_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU_B1_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU_B1_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU_B1_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU_B1_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B1)
                                        | P_Fld(0x1, SHU_B1_DQ8_R_DMSTBEN_SYNC_CG_IG_B1)
                                        | P_Fld(0x1, SHU_B1_DQ8_R_DMRXDLY_CG_IG_B1)
                                        | P_Fld(0x1, SHU_B1_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B1)
                                        | P_Fld(0x1, SHU_B1_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B1)
                                        | P_Fld(0x0, SHU_B1_DQ8_R_DMRXDVS_UPD_FORCE_EN_B1)
                                        | P_Fld(0x7fff, SHU_B1_DQ8_R_DMRXDVS_UPD_FORCE_CYC_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_CA_CMD8, P_Fld(0x1, SHU_CA_CMD8_R_DMRANK_CHG_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU_CA_CMD8_R_DMRANK_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU_CA_CMD8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU_CA_CMD8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU_CA_CMD8_R_DMDQSIEN_FLAG_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU_CA_CMD8_R_DMDQSIEN_FLAG_SYNC_CG_IG_CA)
                                        | P_Fld(0x1, SHU_CA_CMD8_R_DMSTBEN_SYNC_CG_IG_CA)
                                        | P_Fld(0x1, SHU_CA_CMD8_R_DMRXDLY_CG_IG_CA)
                                        | P_Fld(0x1, SHU_CA_CMD8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_CA)
                                        | P_Fld(0x1, SHU_CA_CMD8_R_DMRXDVS_RDSEL_PIPE_CG_IG_CA)
                                        | P_Fld(0x0, SHU_CA_CMD8_R_DMRXDVS_UPD_FORCE_EN_CA)
                                        | P_Fld(0x7fff, SHU_CA_CMD8_R_DMRXDVS_UPD_FORCE_CYC_CA));
    vIO32WriteFldAlign(DDRPHY_MISC_CTRL3, 0x1, MISC_CTRL3_R_DDRPHY_COMB_CG_IG);
    /* Bi_anco HW design issue: run-time PBYTE (B0, B1) flags will lose it's function and become per-bit -> set to 0 */
    vIO32WriteFldMulti(DDRPHY_SHU_B0_DQ7, P_Fld(0x0, SHU_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0)
                                        | P_Fld(0x0, SHU_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0)
                                        | P_Fld(0x0, SHU_B0_DQ7_R_DMRXDVS_DQM_FLAGSEL_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_B1_DQ7, P_Fld(0x0, SHU_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1)
                                        | P_Fld(0x0, SHU_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1)
                                        | P_Fld(0x0, SHU_B1_DQ7_R_DMRXDVS_DQM_FLAGSEL_B1));

    vIO32WriteFldMulti(DRAMC_REG_CLKAR, P_Fld(0x1, CLKAR_SELPH_CMD_CG_DIS) | P_Fld(0x7FFF, CLKAR_REQQUE_PACG_DIS));

    vIO32WriteFldAlign(DRAMC_REG_SHU_DQSG_RETRY, 0x0, SHU_DQSG_RETRY_R_RETRY_PA_DSIABLE); //SH: Set to 0 -> save power
    vIO32WriteFldAlign(DRAMC_REG_WRITE_LEV, 0x0, WRITE_LEV_DDRPHY_COMB_CG_SEL);
    vIO32WriteFldAlign(DRAMC_REG_DUMMY_RD, 0x1, DUMMY_RD_DUMMY_RD_PA_OPT);
    vIO32WriteFldMulti(DRAMC_REG_STBCAL2, P_Fld(0x0, STBCAL2_STB_UIDLYCG_IG)
                                            | P_Fld(0x0, STBCAL2_STB_PIDLYCG_IG));
    vIO32WriteFldMulti(DRAMC_REG_EYESCAN, P_Fld(0x1, EYESCAN_EYESCAN_DQS_SYNC_EN)
                                            | P_Fld(0x1, EYESCAN_EYESCAN_NEW_DQ_SYNC_EN)
                                            | P_Fld(0x1, EYESCAN_EYESCAN_DQ_SYNC_EN));
    //vIO32WriteFldMulti(DRAMC_REG_SHU_ODTCTRL, P_Fld(0x1, SHU_ODTCTRL_RODTENSTB_SELPH_CG_IG)
    //                                        | P_Fld(0x1, SHU_ODTCTRL_RODTEN_SELPH_CG_IG));

    vIO32WriteFldAlign(DDRPHY_SHU_B0_DLL0, 0x1, SHU_B0_DLL0_RG_ARPISM_MCK_SEL_B0_SHU);//Move to DCM off setting
    vIO32WriteFldAlign(DDRPHY_SHU_B1_DLL0, 0x1, SHU_B1_DLL0_RG_ARPISM_MCK_SEL_B1_SHU);//Move to DCM off setting
    //vIO32WriteFldAlign(DDRPHY_SHU_CA_DLL0, 0x1, SHU_CA_DLL0_RG_ARPISM_MCK_SEL_CA_SHU); move to DramcSetting_Olympus_LP4_ByteMode()
    
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI1, 0x1, CA_DLL_ARPI1_RG_ARPISM_MCK_SEL_CA);

    //end _K_

    //DE_UPDATE
    #if (fcFOR_CHIP_ID == fcKrug)
    // Must check with EMI owners -> Asynchronous EMI: Can't turn on RWSPLIT, Synchronous EMI: Can enable RWSPLIT (DE: JL Wu)
    vIO32WriteFldMulti(DRAMC_REG_PERFCTL0, P_Fld(0x0, PERFCTL0_WRFIFO_OPT) //@Jouling, WR fifo (no use)
                                        | P_Fld(0x0, PERFCTL0_REORDEREN) // from Unique  review (it will remove)
                                        | P_Fld(0x1, PERFCTL0_RWSPLIT)); //synchronous EMI -> can turn on RWSPLIT
    #endif
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
    vIO32WriteFldMulti(DDRPHY_SHU_B0_DQ7, P_Fld(0x0, SHU_B0_DQ7_R_DMRXRANK_DQS_LAT_B0)
                                    | P_Fld(0x1, SHU_B0_DQ7_R_DMRXRANK_DQS_EN_B0)
                                    | P_Fld(0x1, SHU_B0_DQ7_R_DMRXRANK_DQ_LAT_B0)
                                    | P_Fld(0x1, SHU_B0_DQ7_R_DMRXRANK_DQ_EN_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_B1_DQ7, P_Fld(0x0, SHU_B1_DQ7_R_DMRXRANK_DQS_LAT_B1)
                                    | P_Fld(0x1, SHU_B1_DQ7_R_DMRXRANK_DQS_EN_B1)
                                    | P_Fld(0x1, SHU_B1_DQ7_R_DMRXRANK_DQ_LAT_B1)
                                    | P_Fld(0x1, SHU_B1_DQ7_R_DMRXRANK_DQ_EN_B1));
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
    //vIO32WriteFldAlign(DRAMC_REG_SHU_RODTENSTB, 0, SHU_RODTENSTB_RODTENSTB_TRACK_EN);

    //Rx Gating tracking settings
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_DQSG), \
                                                P_Fld(9, SHU_DQSG_STB_UPDMASKCYC) | \
                                                P_Fld(1, SHU_DQSG_STB_UPDMASK_EN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQSCAL, P_Fld(0, SHURK0_DQSCAL_R0DQSIENLLMTEN) | P_Fld(0, SHURK0_DQSCAL_R0DQSIENHLMTEN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQSCAL, P_Fld(0, SHURK1_DQSCAL_R1DQSIENLLMTEN) | P_Fld(0, SHURK1_DQSCAL_R1DQSIENHLMTEN));
    vIO32WriteFldMulti(DRAMC_REG_SHU_STBCAL, P_Fld(1, SHU_STBCAL_DQSG_MODE) 
                                        | P_Fld(1, SHU_STBCAL_PICGLAT));

#ifdef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
    vIO32WriteFldAlign(DDRPHY_B0_DQ9, 0x4, B0_DQ9_R_IN_GATE_EN_LOW_OPT_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ9, 0x4, B1_DQ9_R_IN_GATE_EN_LOW_OPT_B1);
#else
    //Modify for corner IC failed at HQA test XTLV
    vIO32WriteFldAlign(DDRPHY_B0_DQ9, 0x7, B0_DQ9_R_IN_GATE_EN_LOW_OPT_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ9, 0x7, B1_DQ9_R_IN_GATE_EN_LOW_OPT_B1);
#endif
    vIO32WriteFldAlign(DDRPHY_CA_CMD10, 0x0, CA_CMD10_R_IN_GATE_EN_LOW_OPT_CA);

    vIO32WriteFldAlign(DDRPHY_SHU_B0_DQ8, 0x1, SHU_B0_DQ8_R_DMRXDLY_CG_IG_B0);
    vIO32WriteFldAlign(DDRPHY_SHU_B1_DQ8, 0x1, SHU_B1_DQ8_R_DMRXDLY_CG_IG_B1);

#ifdef DUMMY_READ_FOR_DQS_GATING_RETRY
    if(p->support_rank_num == RANK_SINGLE)
    {    
        vIO32WriteFldAlign(DRAMC_REG_SHU_DQSG_RETRY, 1, SHU_DQSG_RETRY_R_RETRY_1RANK);
    }    
#endif 


    #if ENABLE_TX_WDQS
    mcSHOW_DBG_MSG(("Enable WDQS\n"));
    //Check reserved bits with PHY integrator
    vIO32WriteFldMulti(DDRPHY_SHU_B0_DLL1, P_Fld(1, SHU_B0_DLL1_RG_READ_BASE_DQS_EN_B0)  | P_Fld(1, SHU_B0_DLL1_RG_READ_BASE_DQSB_EN_B0)
                                        | P_Fld(!p->odt_onoff, SHU_B0_DLL1_RG_ODT_DISABLE_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_B1_DLL1, P_Fld(1, SHU_B1_DLL1_RG_READ_BASE_DQS_EN_B1)  | P_Fld(1, SHU_B1_DLL1_RG_READ_BASE_DQSB_EN_B1)
                                        | P_Fld(!p->odt_onoff, SHU_B1_DLL1_RG_ODT_DISABLE_B1));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ODTCTRL, P_Fld(0x1, SHU_ODTCTRL_RODTE) 
            | P_Fld(0x1, SHU_ODTCTRL_RODTE2)
            | P_Fld(0x1, SHU_ODTCTRL_ROEN));
            
    vIO32WriteFldAlign(DDRPHY_SHU_B0_DQ7, 0x1, SHU_B0_DQ7_R_DMRODTEN_B0);
    vIO32WriteFldAlign(DDRPHY_SHU_B1_DQ7, 0x1, SHU_B1_DQ7_R_DMRODTEN_B1);
    #if ENABLE_RODT_TRACKING_SAVE_MCK
    SetTxWDQSStatusOnOff(1);
    #endif

    #else //WDQS and reak pull are disable
    //Check reserved bits with PHY integrator
    vIO32WriteFldMulti(DDRPHY_SHU_B0_DLL1, P_Fld(0, SHU_B0_DLL1_RG_READ_BASE_DQS_EN_B0) | P_Fld(0, SHU_B0_DLL1_RG_READ_BASE_DQSB_EN_B0)
                                        | P_Fld(0, SHU_B0_DLL1_RG_ODT_DISABLE_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_B1_DLL1, P_Fld(0, SHU_B1_DLL1_RG_READ_BASE_DQS_EN_B1) | P_Fld(0, SHU_B1_DLL1_RG_READ_BASE_DQSB_EN_B1)
                                        | P_Fld(0, SHU_B1_DLL1_RG_ODT_DISABLE_B1));
    #endif


    //DE review WhitneyE2
    vIO32WriteFldAlign(DRAMC_REG_DRSCTRL, 0x1, DRSCTRL_DRS_SELFWAKE_DMYRD_DIS);
    vIO32WriteFldAlign(DRAMC_REG_REFCTRL0, 0x1, REFCTRL0_REFNA_OPT); 
    vIO32WriteFldAlign(DRAMC_REG_ZQCS, 0x1, ZQCS_ZQCS_MASK_SEL_CGAR);//Move to DCM off setting
    vIO32WriteFldMulti(DRAMC_REG_DUMMY_RD, P_Fld(0x1,DUMMY_RD_DMYRD_REORDER_DIS) | P_Fld(0x0,DUMMY_RD_DMYRD_HPRI_DIS)); //La_fite MP setting = 0
    vIO32WriteFldAlign(DRAMC_REG_SHUCTRL2, 0x1, SHUCTRL2_R_DVFS_SREF_OPT);
    vIO32WriteFldAlign(DRAMC_REG_SHUCTRL3, 0xb, SHUCTRL3_VRCGDIS_PRDCNT);    
    vIO32WriteFldAlign(DDRPHY_MISC_CTRL3, 0x1, MISC_CTRL3_R_DDRPHY_RX_PIPE_CG_IG);
    //End

    //DE review Bi_anco
    /* ARPISM_MCK_SEL_B0, B1 set to 1 (Joe): "Due to TX_PICG modify register is set to 1, 
     * ARPISM_MCK_SEL_Bx should be 1 to fulfill APHY TX OE spec for low freq (Ex: DDR1600)"
     */
    vIO32WriteFldMulti(DDRPHY_B0_DLL_ARPI1, P_Fld(0x1, B0_DLL_ARPI1_RG_ARPISM_MCK_SEL_B0_REG_OPT)
                                        | P_Fld(0x1, B0_DLL_ARPI1_RG_ARPISM_MCK_SEL_B0));
    vIO32WriteFldMulti(DDRPHY_B1_DLL_ARPI1, P_Fld(0x1, B1_DLL_ARPI1_RG_ARPISM_MCK_SEL_B1_REG_OPT)
                                        | P_Fld(0x1, B1_DLL_ARPI1_RG_ARPISM_MCK_SEL_B1));
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI1, 0x1, CA_DLL_ARPI1_RG_ARPISM_MCK_SEL_CA_REG_OPT);//Move to DCM off setting
    vIO32WriteFldAlign(DDRPHY_MISC_CTRL0, 0, MISC_CTRL0_R_DMSHU_PHYDCM_FORCEOFF);//Move to DCM off setting

    vIO32WriteFldAlign(DDRPHY_MISC_RXDVS2, 1, MISC_RXDVS2_R_DMRXDVS_SHUFFLE_CTRL_CG_IG);
    vIO32WriteFldAlign(DRAMC_REG_CLKCTRL, 0x1, CLKCTRL_SEQCLKRUN3);
    vIO32WriteFldAlign(DRAMC_REG_REFCTRL1, 1, REFCTRL1_SREF_CG_OPT);
    vIO32WriteFldMulti(DRAMC_REG_SHUCTRL, P_Fld(0x0, SHUCTRL_DVFS_CG_OPT) | P_Fld(0x3, SHUCTRL_R_DVFS_PICG_MARGIN2) | P_Fld(0x3, SHUCTRL_R_DVFS_PICG_MARGIN3));//Move to DCM off setting(SHUCTRL_DVFS_CG_OPT)
    //vIO32WriteFldMulti(DRAMC_REG_SHUCTRL, P_Fld(0x3, SHUCTRL_R_DVFS_PICG_MARGIN2) | P_Fld(0x3, SHUCTRL_R_DVFS_PICG_MARGIN3));
    vIO32WriteFldMulti(DRAMC_REG_SHUCTRL2, P_Fld(0x1, SHUCTRL2_SHORTQ_OPT) | P_Fld(0x3, SHUCTRL2_R_DVFS_PICG_MARGIN));
    vIO32WriteFldAlign(DRAMC_REG_STBCAL2, 0x0, STBCAL2_STB_DBG_EN);
    vIO32WriteFldMulti(DRAMC_REG_PRE_TDQSCK1, P_Fld(0x0, PRE_TDQSCK1_APHY_CG_OPT1) | P_Fld(0x0, PRE_TDQSCK1_SHU_PRELOAD_TX_HW));

    #ifndef FIRST_BRING_UP
    if(u2DFSGetHighestFreq(p) >= 1866)
    #endif
    {
        //if product supports 3733, CLKAR_SELPH_4LCG_DIS always 1 else o, but if 1, comsume more power
        vIO32WriteFldAlign(DRAMC_REG_CLKAR, 1, CLKAR_SELPH_4LCG_DIS);
    }

    #if TX_OE_EXTEND
    UpdateTxOEN(p);
    #endif

    vIO32WriteFldAlign(DRAMC_REG_CKECTRL, 0x1, CKECTRL_CKEPBDIS);

    vIO32WriteFldMulti(DDRPHY_CA_TX_MCK, P_Fld(0x1, CA_TX_MCK_R_DMRESET_FRPHY_OPT) | P_Fld(0xa, CA_TX_MCK_R_DMRESETB_DRVP_FRPHY) | P_Fld(0xa, CA_TX_MCK_R_DMRESETB_DRVN_FRPHY));
 
    //Sy_lvia MP setting should set CKECTRL_CKELCKFIX as 0 to rollback to M17 
    //Ei_ger review by CF Chang
    vIO32WriteFldAlign(DRAMC_REG_CKECTRL, 0x0, CKECTRL_CKELCKFIX);

    //Gating error problem happened in M17 has been solved by setting this RG as 0 (when RODT tracking on (Bi_anco), TX DLY of byte2,3 must not be zero)
    vIO32WriteFldAlign(DRAMC_REG_SHU_RODTENSTB, 0x0, SHU_RODTENSTB_RODTENSTB_4BYTE_EN);

    #if (fcFOR_CHIP_ID == fcKrug) //RODT old mode
    // RODT old mode (it must set SHU_RODTENSTB_RODTENSTB_TRACK_EN=1) from HJ review
    vIO32WriteFldAlign(DRAMC_REG_SHU_ODTCTRL, 0x1, SHU_ODTCTRL_RODTEN_OPT);
    vIO32WriteFldMulti(DRAMC_REG_SHU_RODTENSTB, P_Fld(0, SHU_RODTENSTB_RODTEN_P1_ENABLE) 
                                        | P_Fld(1, SHU_RODTENSTB_RODTENSTB_TRACK_UDFLWCTRL)
                                        | P_Fld(1, SHU_RODTENSTB_RODTENSTB_SELPH_MODE));
    vIO32WriteFldAlign(DRAMC_REG_SHU_STBCAL, 0x0, SHU_STBCAL_DQSIEN_PICG_MODE);
    #if ENABLE_RODT_TRACKING // The SHU_RODTENSTB_RODTENSTB_TRACK_EN will depend on SHU_ODTCTRL_ROEN setting
    if(vGet_Div_Mode(p) == DIV4_MODE)
        vIO32WriteFldAlign(DRAMC_REG_SHU_RODTENSTB, 0x21, SHU_RODTENSTB_RODTENSTB_OFFSET); 
    else		
        vIO32WriteFldAlign(DRAMC_REG_SHU_RODTENSTB, 0x11, SHU_RODTENSTB_RODTENSTB_OFFSET); 
    #endif
	
    //Enable RODT tracking at Init, because RODT position should be changed with Gating position simultaneously
    U8 u1ReadROEN;
    u1ReadROEN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_ODTCTRL), SHU_ODTCTRL_ROEN);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_RODTENSTB), P_Fld(0xff, SHU_RODTENSTB_RODTENSTB_EXT)|\
                                                                P_Fld(u1ReadROEN, SHU_RODTENSTB_RODTENSTB_TRACK_EN));
    #endif

    #if (fcFOR_CHIP_ID == fcKrug) //DDRPHY settings review from YH Cho
    vIO32WriteFldAlign(DDRPHY_SHU_B0_DQ3, 0x0, SHU_B0_DQ3_RG_TX_ARDQS0_PU_PRE_B0); 
    vIO32WriteFldAlign(DDRPHY_SHU_B1_DQ3, 0x0, SHU_B1_DQ3_RG_TX_ARDQS0_PU_PRE_B1); 
    #endif

    #if ENABLE_FIX_PRE8_DVFS_AT_CKEPRD
    vIO32WriteFldAlign(DRAMC_REG_MISCTL0, 0x1, MISCTL0_PG_WAKEUP_OPT); 
    #endif
	
#if ENABLE_TMRRI_NEW_MODE
    //[Ce_rvino DVT](1)dram auto refersh rate by hardware mr4 for rank0 and rank1; (2)After suspend resume, HW MR4 will be fire autoly (Berson)
    vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, (p->support_rank_num == RANK_DUAL) ? (1) : (0), SPCMDCTRL_HMR4_TOG_OPT);
#else
    vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x0, SPCMDCTRL_HMR4_TOG_OPT);
#endif

    //CMD PICG, must match APHY SPEC
#if CMD_PICG_NEW_MODE
    #if (fcFOR_CHIP_ID == fcKrug)
    if (vGet_Div_Mode(p)== DIV4_MODE)
    {
        u2Clk_Dyn_Gating_Sel = 0x6;
        u2CG_CK_SEL = 0xd;
    }
    else //DIV8_MODE
    {
        u2Clk_Dyn_Gating_Sel = 0x5;
        u2CG_CK_SEL = 0xc;
    }
	
    vIO32WriteFldMulti(DRAMC_REG_SHU_APHY_TX_PICG_CTRL, P_Fld(1, SHU_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_OPT) | P_Fld(u2Clk_Dyn_Gating_Sel, SHU_APHY_TX_PICG_CTRL_DDRPHY_CLK_DYN_GATING_SEL));
    vIO32WriteFldMulti(DRAMC_REG_SHU_APHY_TX_PICG_CTRL, P_Fld(1, SHU_APHY_TX_PICG_CTRL_APHYPI_CG_CK_OPT) | P_Fld(u2CG_CK_SEL, SHU_APHY_TX_PICG_CTRL_APHYPI_CG_CK_SEL));
    #endif
#endif

#if RX_PICG_NEW_MODE
    RXPICGSetting(p);
#endif

    //[Ce_rvino DVT]RX FIFO debug feature, MP setting should enable debug function
    vIO32WriteFldAlign(DDRPHY_B0_DQ9, 0x1, B0_DQ9_R_DMRXFIFO_STBENCMP_EN_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ9, 0x1, B1_DQ9_R_DMRXFIFO_STBENCMP_EN_B1);

    // E2 - new -start =========================================================================
    //Design Review Meeting dramc_stbcal_cross rank read to read for new APHY spec - HJ Huang
    vIO32WriteFldMulti(DDRPHY_B0_DQ9, P_Fld(4, B0_DQ9_R_IN_GATE_EN_LOW_OPT_B0) | P_Fld(0, B0_DQ9_R_DMRXDVS_R_F_DLY_RK_OPT_B0));
    vIO32WriteFldMulti(DDRPHY_B1_DQ9, P_Fld(4, B1_DQ9_R_IN_GATE_EN_LOW_OPT_B1) | P_Fld(0, B1_DQ9_R_DMRXDVS_R_F_DLY_RK_OPT_B1));
    vIO32WriteFldMulti(DDRPHY_CA_CMD10, P_Fld(0, CA_CMD10_R_IN_GATE_EN_LOW_OPT_CA) | P_Fld(0, CA_CMD10_R_DMRXDVS_R_F_DLY_RK_OPT));
    vIO32WriteFldAlign(DDRPHY_MISC_CTRL3, 0x0, MISC_CTRL3_ARPI_CG_RK1_SRC_SEL);
    vIO32WriteFldAlign(DRAMC_REG_SHU_RANK_SEL_STB, 0x1, SHU_RANK_SEL_STB_RANK_SEL_RXDLY_TRACK);
    vIO32WriteFldAlign(DRAMC_REG_SHU_RANK_SEL_STB, 0x0, SHU_RANK_SEL_STB_RANK_SEL_STB_SERMODE); // for old rank mode settings - HJ Huang

    // Update VREF1/VREF2 per bit select - Ying-Yu Hsu
    // Note: The RG (SHU_B1_DQ3_RG_TX_ARDQS0_PDB_B1) need update when using VREF2 per bit select
    vIO32WriteFldMulti(DDRPHY_SHU_B0_DQ3, P_Fld(0, SHU_B0_DQ3_RG_TX_ARDQS0_PDB_B0) | P_Fld(0, SHU_B0_DQ3_RG_TX_ARDQ_PDB_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_B1_DQ3, P_Fld(0, SHU_B1_DQ3_RG_TX_ARDQS0_PDB_B1) | P_Fld(0, SHU_B1_DQ3_RG_TX_ARDQ_PDB_B1));

    // DQSI DLY 3B in FIFO for adjustmen by YingMin
    vIO32WriteFldAlign(DDRPHY_SHU_B0_DQ5, 0x4, SHU_B0_DQ5_RG_RX_ARDQ_FIFO_DQSI_DLY_B0);
    vIO32WriteFldAlign(DDRPHY_SHU_B1_DQ5, 0x4, SHU_B1_DQ5_RG_RX_ARDQ_FIFO_DQSI_DLY_B1);

#if ENABLE_REMOVE_MCK8X_UNCERT_LOWPOWER_OPTION
    vIO32WriteFldAlign(DDRPHY_MISC_VREF_CTRL, 0x7F, MISC_VREF_CTRL_MISC_LP_8X_MUX); // @Mazar: MCK8X uncertainty remove
    vIO32WriteFldAlign(DDRPHY_MISC_VREF_CTRL, 0x7F, MISC_VREF_CTRL_MISC_LP_DDR400_MUX); // @Mazar: open loop mode setting(found connection problem in CHB in DSIM)
#endif

#if ENABLE_REMOVE_MCK8X_UNCERT_DFS_OPTION
    vIO32WriteFldAlign(DDRPHY_DVFS_EMI_CLK, 1, DVFS_EMI_CLK_R_DDRPHY_SHUFFLE_MUX_ENABLE); // @Lynx
    vIO32WriteFldAlign(DRAMC_REG_DVFSDLL2, 1, DVFSDLL2_R_SHUFFLE_PI_RESET_ENABLE); // @Lynx
    vIO32WriteFldAlign(DRAMC_REG_DVFSDLL2, 3, DVFSDLL2_R_DVFS_MCK8X_MARGIN); // @Lynx
    vIO32WriteFldAlign(DRAMC_REG_DVFSDLL2, 3, DVFSDLL2_R_DVFS_PICG_MARGIN4_NEW); // @Lynx
#endif

    vIO32WriteFldAlign(DDRPHY_SHU_MISC1, 0x00000020, SHU_MISC1_DR_EMI_RESERVE); // @TK, For EMI slice timing 

    //@Ton, DQS retry off
    vIO32WriteFldMulti(DRAMC_REG_SHU_DQSG_RETRY, P_Fld(0x0, SHU_DQSG_RETRY_R_RETRY_USE_BURST_MDOE)
                | P_Fld(0x0, SHU_DQSG_RETRY_R_RDY_SEL_DLE)
                | P_Fld(0x0, SHU_DQSG_RETRY_R_DQSIENLAT)
                | P_Fld(0x0, SHU_DQSG_RETRY_R_RETRY_ROUND_NUM)
                | P_Fld(0x0, SHU_DQSG_RETRY_R_RETRY_ONCE));

    vIO32WriteFldAlign(DRAMC_REG_PRE_TDQSCK1, 0x1, PRE_TDQSCK1_TX_TRACKING_OPT); //@Jouling

    //@Berson, NOTE: Please set to 1 when DVT is verified. SPMFW must updated by DVT owner.
    vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x1, SPCMDCTRL_SPDR_MR4_OPT); //S0 from suspend to resume trigger HW MR4

    //@YY, APHY init settings review
    vIO32WriteFldAlign(DDRPHY_SHU_B0_DQ6, 0x0, SHU_B0_DQ6_RG_ARPI_OFFSET_MCTL_B0);
    vIO32WriteFldAlign(DDRPHY_SHU_B1_DQ6, 0x0, SHU_B1_DQ6_RG_ARPI_OFFSET_MCTL_B1);
    vIO32WriteFldAlign(DDRPHY_CA_CMD5, 0x29, CA_CMD5_RG_RX_ARCMD_EYE_VREF_SEL);
    vIO32WriteFldAlign(DDRPHY_SHU_CA_CMD5, 0x4, SHU_CA_CMD5_RG_RX_ARCMD_FIFO_DQSI_DLY);
    // E2 - new -end =========================================================================
		
    #if CMD_CKE_WORKAROUND_FIX
    CMD_CKE_Modified_txp_Setting(p);
    #endif

    #if RX_PIPE_BYPASS_EN
    vIO32WriteFldAlign(DDRPHY_SHU_MISC0, 0x1, SHU_MISC0_R_RX_PIPE_BYPASS_EN);
    #endif

    #if CBT_MOVE_CA_INSTEAD_OF_CLK
    if (u1IsDDR800PhaseMode(p) == FALSE)
    {
        u1CaUI = 0;
        u1CaPI = 32;
    }
    else
    {
        u1CaUI = 1;
        u1CaPI = 0;
    }
    // CA delay shift u1CaUI*UI
    DramcCmdUIDelaySetting(p, u1CaUI);

    // Rank0/1 u1CaPI*PI CA delay
    vIO32WriteFldAlign(DDRPHY_SHU_R0_CA_CMD9, u1CaPI, SHU_R0_CA_CMD9_RG_RK0_ARPI_CMD);
    vIO32WriteFldAlign(DDRPHY_SHU_R1_CA_CMD9, u1CaPI, SHU_R1_CA_CMD9_RG_RK1_ARPI_CMD);

    vIO32WriteFldAlign(DDRPHY_SHU_R0_CA_CMD9, CLK_SHIFT_PI_DELAY, SHU_R0_CA_CMD9_RG_RK0_ARPI_CLK);    
    vIO32WriteFldAlign(DDRPHY_SHU_R1_CA_CMD9, CLK_SHIFT_PI_DELAY, SHU_R1_CA_CMD9_RG_RK1_ARPI_CLK);    
    #endif
	
	//Reset XRTRTR related setting
    #if XRTRTR_NEW_CROSS_RK_MODE
    vIO32WriteFldAlign(DDRPHY_SHU_B0_DQ6, 0, SHU_B0_DQ6_RG_RX_ARDQ_RANK_SEL_SER_MODE_B0);
    vIO32WriteFldAlign(DDRPHY_SHU_B1_DQ6, 0, SHU_B1_DQ6_RG_RX_ARDQ_RANK_SEL_SER_MODE_B1);
    vIO32WriteFldAlign(DRAMC_REG_SHU_RANK_SEL_STB, 0, SHU_RANK_SEL_STB_RANK_SEL_STB_EN);	
    #endif

    #if RDSEL_TRACKING_EN
    vIO32WriteFldAlign(DRAMC_REG_SHU_RDSEL_TRACK, 0, SHU_RDSEL_TRACK_R_DMDATLAT_I); //DMDATLAT_I should be set as 0 before set datlat k value, otherwise the status flag wil be set as 1
    #endif
	
    //Update MP setting
    vIO32WriteFldMulti(DRAMC_REG_STBCAL2, P_Fld(0x7, STBCAL2_STBCAL_UI_UPD_MASK_OPT) 
                                         |P_Fld(0x1, STBCAL2_STBCAL_UI_UPD_MASK_EN)); //HJ, Gating tracking
    #if XRTRTR_NEW_CROSS_RK_MODE
    vIO32WriteFldMulti(DRAMC_REG_SHU_PHY_RX_CTRL, P_Fld(0x2, SHU_PHY_RX_CTRL_RX_IN_GATE_EN_PRE_OFFSET) 
                                         |P_Fld(0x2, SHU_PHY_RX_CTRL_RANK_RXDLY_UPD_OFFSET)
                                         |P_Fld(0x1, SHU_PHY_RX_CTRL_RANK_RXDLY_UPDLAT_EN)); //HJ, R2R
    #endif

    #if (fcFOR_CHIP_ID == fcKrug)
    // K_rug
    // PBR2PBR pa cg free-run
    vIO32WriteFldAlign(DRAMC_REG_MISCTL0, 0x1, MISCTL0_REFP_ARBMASK_PBR2PBR_PA_DIS); //Unique

    // Ton review for MP/Bring-up settings
    vIO32WriteFldMulti(DDRPHY_MISC_CTRL3, P_Fld(0, MISC_CTRL3_ARPI_CG_MCK_CA_OPT) |
                                                               P_Fld(0, MISC_CTRL3_ARPI_MPDIV_CG_CA_OPT) |
                                                               P_Fld(0, MISC_CTRL3_ARPI_CG_CLK_OPT) |
                                                               P_Fld(0, MISC_CTRL3_ARPI_CG_CMD_OPT));

    if (p->frequency == 2133) // for DDR4266
    {
        u4RsvEMI = 0xC0000000;
        u1RsvEco1 = 0x0;
    }
    else
    {
        u4RsvEMI = 0xC0000020;
        u1RsvEco1 = 0x2;
    }
    vIO32WriteFldAlign(DDRPHY_SHU_MISC1, u4RsvEMI, SHU_MISC1_DR_EMI_RESERVE); //TK
    vIO32WriteFldMulti(DDRPHY_SHU_MISC2, P_Fld(0x3, SHU_MISC2_DR_EMI_RESERVE_ECO2) |
                                                               P_Fld(u1RsvEco1, SHU_MISC2_DR_EMI_RESERVE_ECO1)); // TK
    #endif

#if ENABLE_TPBR2PBR_REFRESH_TIMING
    vIO32WriteFldAlign(DRAMC_REG_REFCTRL1, 0x1, REFCTRL1_REF_OVERHEAD_PBR2PB_ENA); //@Derping
    vIO32WriteFldAlign(DRAMC_REG_MISCTL0, 0x1, MISCTL0_REFP_ARBMASK_PBR2PBR_ENA); //@Unique
#endif

    // @Jouling, Update MP settings for  TX UI tracking (Now no use!old design)
    vIO32WriteFldAlign(DRAMC_REG_SHU_DQS2DQ_TX, 0x0, SHU_DQS2DQ_TX_OE2DQ_OFFSET);

    return DRAM_OK;
}

#if LEGACY_DELAY_CELL
/* Legacy CA, TX DQ, TX DQM delay cell initial settings
 * RK#_TX_ARCA#_DLY (RK0~2, CA0~5), RK#_TX_ARDQ#_DLY_B# (RK0~2, DQ0~8, B0~1), RK#_TX_ARDQM0_DLY_B# (RK0~2, B0~1)
 */
static void LegacyDlyCellInitLP4_DDR2667(DRAMC_CTX_T *p)
{
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B0_DQ0, P_Fld(0xa, SHU_R1_B0_DQ0_RK1_TX_ARDQ7_DLY_B0)
                | P_Fld(0xa, SHU_R1_B0_DQ0_RK1_TX_ARDQ6_DLY_B0)
                | P_Fld(0xa, SHU_R1_B0_DQ0_RK1_TX_ARDQ5_DLY_B0)
                | P_Fld(0xa, SHU_R1_B0_DQ0_RK1_TX_ARDQ4_DLY_B0)
                | P_Fld(0xa, SHU_R1_B0_DQ0_RK1_TX_ARDQ3_DLY_B0)
                | P_Fld(0xa, SHU_R1_B0_DQ0_RK1_TX_ARDQ2_DLY_B0)
                | P_Fld(0xa, SHU_R1_B0_DQ0_RK1_TX_ARDQ1_DLY_B0)
                | P_Fld(0xa, SHU_R1_B0_DQ0_RK1_TX_ARDQ0_DLY_B0));
    vIO32WriteFldAlign(DDRPHY_SHU_R1_B0_DQ1, 0xa, SHU_R1_B0_DQ1_RK1_TX_ARDQM0_DLY_B0);
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B1_DQ0, P_Fld(0xa, SHU_R1_B1_DQ0_RK1_TX_ARDQ7_DLY_B1)
                | P_Fld(0xa, SHU_R1_B1_DQ0_RK1_TX_ARDQ6_DLY_B1)
                | P_Fld(0xa, SHU_R1_B1_DQ0_RK1_TX_ARDQ5_DLY_B1)
                | P_Fld(0xa, SHU_R1_B1_DQ0_RK1_TX_ARDQ4_DLY_B1)
                | P_Fld(0xa, SHU_R1_B1_DQ0_RK1_TX_ARDQ3_DLY_B1)
                | P_Fld(0xa, SHU_R1_B1_DQ0_RK1_TX_ARDQ2_DLY_B1)
                | P_Fld(0xa, SHU_R1_B1_DQ0_RK1_TX_ARDQ1_DLY_B1)
                | P_Fld(0xa, SHU_R1_B1_DQ0_RK1_TX_ARDQ0_DLY_B1));
    vIO32WriteFldAlign(DDRPHY_SHU_R1_B1_DQ1, 0xa, SHU_R1_B1_DQ1_RK1_TX_ARDQM0_DLY_B1);
}

static void LegacyDlyCellInitLP4_DDR1600(DRAMC_CTX_T *p)
{
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B0_DQ0, P_Fld(0xd, SHU_R1_B0_DQ0_RK1_TX_ARDQ7_DLY_B0)
                | P_Fld(0xd, SHU_R1_B0_DQ0_RK1_TX_ARDQ6_DLY_B0)
                | P_Fld(0xd, SHU_R1_B0_DQ0_RK1_TX_ARDQ5_DLY_B0)
                | P_Fld(0xd, SHU_R1_B0_DQ0_RK1_TX_ARDQ4_DLY_B0)
                | P_Fld(0xd, SHU_R1_B0_DQ0_RK1_TX_ARDQ3_DLY_B0)
                | P_Fld(0xd, SHU_R1_B0_DQ0_RK1_TX_ARDQ2_DLY_B0)
                | P_Fld(0xd, SHU_R1_B0_DQ0_RK1_TX_ARDQ1_DLY_B0)
                | P_Fld(0xd, SHU_R1_B0_DQ0_RK1_TX_ARDQ0_DLY_B0));
    vIO32WriteFldAlign(DDRPHY_SHU_R1_B0_DQ1, 0xd, SHU_R1_B0_DQ1_RK1_TX_ARDQM0_DLY_B0);
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B1_DQ0, P_Fld(0xd, SHU_R1_B1_DQ0_RK1_TX_ARDQ7_DLY_B1)
                | P_Fld(0xd, SHU_R1_B1_DQ0_RK1_TX_ARDQ6_DLY_B1)
                | P_Fld(0xd, SHU_R1_B1_DQ0_RK1_TX_ARDQ5_DLY_B1)
                | P_Fld(0xd, SHU_R1_B1_DQ0_RK1_TX_ARDQ4_DLY_B1)
                | P_Fld(0xd, SHU_R1_B1_DQ0_RK1_TX_ARDQ3_DLY_B1)
                | P_Fld(0xd, SHU_R1_B1_DQ0_RK1_TX_ARDQ2_DLY_B1)
                | P_Fld(0xd, SHU_R1_B1_DQ0_RK1_TX_ARDQ1_DLY_B1)
                | P_Fld(0xd, SHU_R1_B1_DQ0_RK1_TX_ARDQ0_DLY_B1));
    vIO32WriteFldAlign(DDRPHY_SHU_R1_B1_DQ1, 0xd, SHU_R1_B1_DQ1_RK1_TX_ARDQM0_DLY_B1);
}

static void LegacyDlyCellInitLP4_DDR3200(DRAMC_CTX_T *p)
{
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B0_DQ0, P_Fld(0xa, SHU_R1_B0_DQ0_RK1_TX_ARDQ7_DLY_B0)
                | P_Fld(0xa, SHU_R1_B0_DQ0_RK1_TX_ARDQ6_DLY_B0)
                | P_Fld(0xa, SHU_R1_B0_DQ0_RK1_TX_ARDQ5_DLY_B0)
                | P_Fld(0xa, SHU_R1_B0_DQ0_RK1_TX_ARDQ4_DLY_B0)
                | P_Fld(0xa, SHU_R1_B0_DQ0_RK1_TX_ARDQ3_DLY_B0)
                | P_Fld(0xa, SHU_R1_B0_DQ0_RK1_TX_ARDQ2_DLY_B0)
                | P_Fld(0xa, SHU_R1_B0_DQ0_RK1_TX_ARDQ1_DLY_B0)
                | P_Fld(0xa, SHU_R1_B0_DQ0_RK1_TX_ARDQ0_DLY_B0));
    vIO32WriteFldAlign(DDRPHY_SHU_R1_B0_DQ1, 0xa, SHU_R1_B0_DQ1_RK1_TX_ARDQM0_DLY_B0);
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B1_DQ0, P_Fld(0xa, SHU_R1_B1_DQ0_RK1_TX_ARDQ7_DLY_B1)
                | P_Fld(0xa, SHU_R1_B1_DQ0_RK1_TX_ARDQ6_DLY_B1)
                | P_Fld(0xa, SHU_R1_B1_DQ0_RK1_TX_ARDQ5_DLY_B1)
                | P_Fld(0xa, SHU_R1_B1_DQ0_RK1_TX_ARDQ4_DLY_B1)
                | P_Fld(0xa, SHU_R1_B1_DQ0_RK1_TX_ARDQ3_DLY_B1)
                | P_Fld(0xa, SHU_R1_B1_DQ0_RK1_TX_ARDQ2_DLY_B1)
                | P_Fld(0xa, SHU_R1_B1_DQ0_RK1_TX_ARDQ1_DLY_B1)
                | P_Fld(0xa, SHU_R1_B1_DQ0_RK1_TX_ARDQ0_DLY_B1));
    vIO32WriteFldAlign(DDRPHY_SHU_R1_B1_DQ1, 0xa, SHU_R1_B1_DQ1_RK1_TX_ARDQM0_DLY_B1);
}
#endif

#if LEGACY_TX_TRACK
/* Legacy tx tracking related initial settings (actual correct values are set during calibration steps)
 * BOOT_ORIG_UI_RK#_DQ#, BOOT_TARG_UI_RK#_DQ#, BOOT_TARG_UI_RK#_DQM#, BOOT_TARG_UI_RK#_OEN_DQ#, BOOT_TARG_UI_RK#_OEN_DQM#
 * DQSOSCTHRD_DEC, DQSOSCTHRD_INC, DQSOSC_PRDCNT
 * DQSOSC_BASE_RK#, DQSOSCR_RK#_BYTE_MODE, DQSOSCR_DQSOSCRCNT
 */
static void LegacyTxTrackLP4_DDR2667(DRAMC_CTX_T *p)
{
    vIO32WriteFldMulti(DRAMC_REG_SHU_DQSOSCTHRD, P_Fld(0xc, SHU_DQSOSCTHRD_DQSOSCTHRD_DEC_RK0)
                | P_Fld(0xc, SHU_DQSOSCTHRD_DQSOSCTHRD_INC_RK0)
                | P_Fld(0xc, SHU_DQSOSCTHRD_DQSOSCTHRD_INC_RK1_7TO0));    
    vIO32WriteFldMulti(DRAMC_REG_SHU_DQSOSC_PRD, P_Fld(0xc, SHU_DQSOSC_PRD_DQSOSCTHRD_DEC_RK1)
                | P_Fld(0x0, SHU_DQSOSC_PRD_DQSOSCTHRD_INC_RK1_11TO8)
                | P_Fld(0x10, SHU_DQSOSC_PRD_DQSOSC_PRDCNT));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQSOSC, P_Fld(0x1ae, SHURK0_DQSOSC_DQSOSC_BASE_RK0_B1)
                | P_Fld(0x1ae, SHURK0_DQSOSC_DQSOSC_BASE_RK0));
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQS2DQ_CAL1, P_Fld(0x354, SHURK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ1)
                    | P_Fld(0x354, SHURK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQS2DQ_CAL2, P_Fld(0x354, SHURK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ1)
                    | P_Fld(0x354, SHURK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ0));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQS2DQ_CAL1, P_Fld(0x254, SHURK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ1)
                    | P_Fld(0x254, SHURK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQS2DQ_CAL2, P_Fld(0x254, SHURK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ1)
                    | P_Fld(0x254, SHURK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ0));
    }
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQS2DQ_CAL3, P_Fld(0x14, SHURK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1_B4TO0)
                | P_Fld(0x14, SHURK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0_B4TO0)
                | P_Fld(0x18, SHURK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1)
                | P_Fld(0x18, SHURK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQS2DQ_CAL4, P_Fld(0x14, SHURK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1_B4TO0)
                | P_Fld(0x14, SHURK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0_B4TO0)
                | P_Fld(0x18, SHURK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1)
                | P_Fld(0x18, SHURK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0));
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQS2DQ_CAL5, P_Fld(0x354, SHURK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM1)
                    | P_Fld(0x354, SHURK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM0));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQS2DQ_CAL5, P_Fld(0x254, SHURK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM1)
                    | P_Fld(0x254, SHURK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM0));
    }
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQSOSC, P_Fld(0x160, SHURK1_DQSOSC_DQSOSC_BASE_RK1_B1)
                | P_Fld(0x160, SHURK1_DQSOSC_DQSOSC_BASE_RK1));
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQS2DQ_CAL1, P_Fld(0x354, SHURK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ1)
                    | P_Fld(0x354, SHURK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQS2DQ_CAL2, P_Fld(0x354, SHURK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ1)
                    | P_Fld(0x354, SHURK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ0));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQS2DQ_CAL1, P_Fld(0x254, SHURK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ1)
                    | P_Fld(0x254, SHURK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQS2DQ_CAL2, P_Fld(0x254, SHURK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ1)
                    | P_Fld(0x254, SHURK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ0));
    }
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQS2DQ_CAL3, P_Fld(0x14, SHURK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ1_B4TO0)
                | P_Fld(0x14, SHURK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ0_B4TO0)
                | P_Fld(0x18, SHURK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ1)
                | P_Fld(0x18, SHURK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQS2DQ_CAL4, P_Fld(0x14, SHURK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM1_B4TO0)
                | P_Fld(0x14, SHURK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM0_B4TO0)
                | P_Fld(0x18, SHURK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM1)
                | P_Fld(0x18, SHURK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM0));
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQS2DQ_CAL5, P_Fld(0x354, SHURK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM1)
                    | P_Fld(0x354, SHURK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM0));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQS2DQ_CAL5, P_Fld(0x254, SHURK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM1)
                    | P_Fld(0x254, SHURK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM0));
    }
}

static void LegacyTxTrackLP4_DDR1600(DRAMC_CTX_T *p)
{
    vIO32WriteFldMulti(DRAMC_REG_SHU_DQSOSCTHRD, P_Fld(0x14, SHU_DQSOSCTHRD_DQSOSCTHRD_DEC_RK0)
                | P_Fld(0x14, SHU_DQSOSCTHRD_DQSOSCTHRD_INC_RK0)
                | P_Fld(0x14, SHU_DQSOSCTHRD_DQSOSCTHRD_INC_RK1_7TO0));    
    vIO32WriteFldMulti(DRAMC_REG_SHU_DQSOSC_PRD, P_Fld(0x14, SHU_DQSOSC_PRD_DQSOSCTHRD_DEC_RK1)
                | P_Fld(0x0, SHU_DQSOSC_PRD_DQSOSCTHRD_INC_RK1_11TO8)
                | P_Fld(0xf, SHU_DQSOSC_PRD_DQSOSC_PRDCNT));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQSOSC, P_Fld(0x2d0, SHURK0_DQSOSC_DQSOSC_BASE_RK0_B1)
                | P_Fld(0x2d0, SHURK0_DQSOSC_DQSOSC_BASE_RK0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQS2DQ_CAL1, P_Fld(0x23a, SHURK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ1)
                | P_Fld(0x23a, SHURK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQS2DQ_CAL2, P_Fld(0x23a, SHURK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ1)
                | P_Fld(0x23a, SHURK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQS2DQ_CAL3, P_Fld(0x1a, SHURK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1_B4TO0)
                | P_Fld(0x1a, SHURK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0_B4TO0)
                | P_Fld(0xf, SHURK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1)
                | P_Fld(0xf, SHURK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQS2DQ_CAL4, P_Fld(0x1a, SHURK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1_B4TO0)
                | P_Fld(0x1a, SHURK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0_B4TO0)
                | P_Fld(0xf, SHURK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1)
                | P_Fld(0xf, SHURK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQS2DQ_CAL5, P_Fld(0x23e, SHURK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM1)
                | P_Fld(0x23e, SHURK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQSOSC, P_Fld(0x24e, SHURK1_DQSOSC_DQSOSC_BASE_RK1_B1)
                | P_Fld(0x24e, SHURK1_DQSOSC_DQSOSC_BASE_RK1));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQS2DQ_CAL1, P_Fld(0x23e, SHURK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ1)
                | P_Fld(0x23e, SHURK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQS2DQ_CAL2, P_Fld(0x23e, SHURK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ1)
                | P_Fld(0x23e, SHURK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQS2DQ_CAL3, P_Fld(0x1e, SHURK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ1_B4TO0)
                | P_Fld(0x1e, SHURK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ0_B4TO0)
                | P_Fld(0xf, SHURK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ1)
                | P_Fld(0xf, SHURK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQS2DQ_CAL4, P_Fld(0x1e, SHURK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM1_B4TO0)
                | P_Fld(0x1e, SHURK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM0_B4TO0)
                | P_Fld(0xf, SHURK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM1)
                | P_Fld(0xf, SHURK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQS2DQ_CAL5, P_Fld(0x23e, SHURK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM1)
                | P_Fld(0x23e, SHURK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM0));
}

static void LegacyTxTrackLP4_DDR3200(DRAMC_CTX_T *p)
{
    vIO32WriteFldMulti(DRAMC_REG_SHU_DQSOSCTHRD, P_Fld(0xa, SHU_DQSOSCTHRD_DQSOSCTHRD_DEC_RK0)
                | P_Fld(0xa, SHU_DQSOSCTHRD_DQSOSCTHRD_INC_RK0)
                | P_Fld(0xa, SHU_DQSOSCTHRD_DQSOSCTHRD_INC_RK1_7TO0));    
    vIO32WriteFldMulti(DRAMC_REG_SHU_DQSOSC_PRD, P_Fld(0xa, SHU_DQSOSC_PRD_DQSOSCTHRD_DEC_RK1)
                | P_Fld(0x0, SHU_DQSOSC_PRD_DQSOSCTHRD_INC_RK1_11TO8)
                | P_Fld(0x10, SHU_DQSOSC_PRD_DQSOSC_PRDCNT));
    vIO32WriteFldAlign(DRAMC_REG_SHU_DQSOSCR, 0x10, SHU_DQSOSCR_DQSOSCRCNT);
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQSOSC, P_Fld(0x168, SHURK0_DQSOSC_DQSOSC_BASE_RK0_B1)
                | P_Fld(0x168, SHURK0_DQSOSC_DQSOSC_BASE_RK0));
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
            vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQS2DQ_CAL1, P_Fld(0x3da, SHURK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ1)
                        | P_Fld(0x3da, SHURK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ0));
            vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQS2DQ_CAL2, P_Fld(0x3da, SHURK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ1)
                        | P_Fld(0x3da, SHURK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ0));
    }
    else
    {
            vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQS2DQ_CAL1, P_Fld(0x2da, SHURK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ1)
                        | P_Fld(0x2da, SHURK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ0));
            vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQS2DQ_CAL2, P_Fld(0x2da, SHURK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ1)
                        | P_Fld(0x2da, SHURK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ0));
    }
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQS2DQ_CAL3, P_Fld(0x1a, SHURK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1_B4TO0)
                | P_Fld(0x1a, SHURK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0_B4TO0)
                | P_Fld(0x1c, SHURK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1)
                | P_Fld(0x1c, SHURK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQS2DQ_CAL4, P_Fld(0x1a, SHURK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1_B4TO0)
                | P_Fld(0x1a, SHURK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0_B4TO0)
                | P_Fld(0x1c, SHURK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1)
                | P_Fld(0x1c, SHURK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0));
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
            vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQS2DQ_CAL5, P_Fld(0x3da, SHURK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM1)
                        | P_Fld(0x3da, SHURK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM0));
    }
    else
    {
            vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQS2DQ_CAL5, P_Fld(0x2da, SHURK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM1)
                        | P_Fld(0x2da, SHURK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM0));
    }
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQSOSC, P_Fld(0x127, SHURK1_DQSOSC_DQSOSC_BASE_RK1_B1)
                | P_Fld(0x127, SHURK1_DQSOSC_DQSOSC_BASE_RK1));
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
            vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQS2DQ_CAL1, P_Fld(0x3d4, SHURK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ1)
                        | P_Fld(0x3d4, SHURK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ0));
            vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQS2DQ_CAL2, P_Fld(0x3d4, SHURK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ1)
                        | P_Fld(0x3d4, SHURK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ0));
    }
    else
    {
            vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQS2DQ_CAL1, P_Fld(0x2d4, SHURK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ1)
                        | P_Fld(0x2d4, SHURK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ0));
            vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQS2DQ_CAL2, P_Fld(0x2d4, SHURK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ1)
                        | P_Fld(0x2d4, SHURK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ0));
    }
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQS2DQ_CAL3, P_Fld(0x14, SHURK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ1_B4TO0)
                | P_Fld(0x14, SHURK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ0_B4TO0)
                | P_Fld(0x1c, SHURK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ1)
                | P_Fld(0x1c, SHURK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQS2DQ_CAL4, P_Fld(0x14, SHURK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM1_B4TO0)
                | P_Fld(0x14, SHURK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM0_B4TO0)
                | P_Fld(0x1c, SHURK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM1)
                | P_Fld(0x1c, SHURK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM0));
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
            vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQS2DQ_CAL5, P_Fld(0x3d4, SHURK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM1)
                        | P_Fld(0x3d4, SHURK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM0));
    }
    else
    {
            vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQS2DQ_CAL5, P_Fld(0x2d4, SHURK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM1)
                        | P_Fld(0x2d4, SHURK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM0));
    }
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldAlign(DRAMC_REG_DQSOSCR, 0x1, DQSOSCR_RK0_BYTE_MODE);
        vIO32WriteFldAlign(DRAMC_REG_DQSOSCR, 0x1, DQSOSCR_RK1_BYTE_MODE);
    }
}
#endif

#if LEGACY_TDQSCK_PRECAL
/* Legacy tDQSCK precal related initial settings (actual correct values are set during calibration)
 * Ex: TDQSCK_JUMP_RATIO, TDQSCK_UIFREQ#, TDQSCK_PIFREQ#
 */
static void LegacyPreCalLP4_DDR2667(DRAMC_CTX_T *p)
{
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
}

static void LegacyPreCalLP4_DDR1600(DRAMC_CTX_T *p)
{
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
}

static void LegacyPreCalLP4_DDR3200(DRAMC_CTX_T *p)
{
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
}
#endif

#if LEGACY_GATING_DLY
/* Legacy initial settings (actual correct values are set during gating calibration)
 * Ex: TX_DLY_DQS#_GATED, TX_DLY_DQS#_GATED_P1, REG_DLY_DQS#_GATED, REG_DLY_DQS#_GATED_P1
 *     TXDLY_B#_RODTEN, TXDLY_B#_RODTEN_P1, DLY_B#_RODTEN, DLY_B#_RODTEN_P1
 */
static void LegacyGatingDlyLP3(DRAMC_CTX_T *p)
{
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
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0, P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED_P1)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED_P1)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(0x4, SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED_P1)
                | P_Fld(0x2, SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED)
                | P_Fld(0x4, SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED_P1)
                | P_Fld(0x2, SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED)
                | P_Fld(0x4, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
                | P_Fld(0x2, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
                | P_Fld(0x4, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
                | P_Fld(0x2, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
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
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0+SHIFT_TO_CHB_ADDR, P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED_P1)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED_P1)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
                | P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1+SHIFT_TO_CHB_ADDR, P_Fld(0x3, SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED_P1)
                | P_Fld(0x1, SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED)
                | P_Fld(0x3, SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED_P1)
                | P_Fld(0x1, SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED)
                | P_Fld(0x3, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
                | P_Fld(0x1, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
                | P_Fld(0x3, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
                | P_Fld(0x1, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0+SHIFT_TO_CHB_ADDR, P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS3_GATED_P1)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS3_GATED)
                | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS2_GATED_P1)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS2_GATED)
                | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
                | P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
                | P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1+SHIFT_TO_CHB_ADDR, P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS3_GATED_P1)
                | P_Fld(0x7, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS3_GATED)
                | P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS2_GATED_P1)
                | P_Fld(0x7, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS2_GATED)
                | P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
                | P_Fld(0x7, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED)
                | P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1)
                | P_Fld(0x7, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED));
}

static void LegacyGatingDlyLP4_DDR800(DRAMC_CTX_T *p)
{

    U8 ucR0GatingMCK = 0, ucR0GatingB0UI = 0, ucR0GatingB1UI = 0;
    U8 ucR0GatingP1MCK = 0, ucR0GatingB0P1UI = 0, ucR0GatingB1P1UI = 0;

    U8 ucR1GatingMCK = 0, ucR1GatingB0UI = 0, ucR1GatingB1UI = 0;
    U8 ucR1GatingP1MCK = 0, ucR1GatingB0P1UI = 0, ucR1GatingB1P1UI = 0;

    U8 ucR0GatingB0PI = 0, ucR0GatingB1PI = 0;
    U8 ucR1GatingB0PI = 0, ucR1GatingB1PI = 0;

    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        // Byte mode don't use. need fine-tune for DV sim for DDR800
        //RK0
        ucR0GatingMCK = 0x2;
        ucR0GatingP1MCK = 0x2;
        ucR0GatingB0UI = 0x2;
        ucR0GatingB1UI = 0x2;
        ucR0GatingB0P1UI = 0x6;
        ucR0GatingB1P1UI = 0x6;
        //RK1
        ucR1GatingMCK = 0x2;
        ucR1GatingP1MCK = 0x3;
        ucR1GatingB0UI = 0x5;
        ucR1GatingB1UI = 0x5;
        ucR1GatingB0P1UI = 0x1;
        ucR1GatingB1P1UI = 0x1;
    }
    else
    {
        //RK0
        ucR0GatingMCK = 0x1;
        ucR0GatingP1MCK = 0x1;
        ucR0GatingB0UI = 0x4;
        ucR0GatingB1UI = 0x4;
        ucR0GatingB0P1UI = 0x6;
        ucR0GatingB1P1UI = 0x6;
        //RK1
        ucR1GatingMCK = 0x1;
        ucR1GatingP1MCK = 0x1;
        ucR1GatingB0UI = 0x5;
        ucR1GatingB1UI = 0x5;
        ucR1GatingB0P1UI = 0x7;
        ucR1GatingB1P1UI = 0x7;
    }

    //Gating PI
    if (vGet_DDR800_Mode(p) == DDR800_CLOSE_LOOP)
    {
        ucR0GatingB0PI = 0xc;
        ucR0GatingB1PI = 0xe;
        ucR1GatingB0PI = 0x1e;
        ucR1GatingB1PI = 0x1e;
    }
    else
    {
        // DDR800_SEMI_LOOP and DDR800_OPEN_LOOP
        ucR0GatingB0PI = 0x0;
        ucR0GatingB1PI = 0x0;
        ucR1GatingB0PI = 0x0;
        ucR1GatingB1PI = 0x0;
    }

    //Gating RK0 MCK
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0, P_Fld(ucR0GatingP1MCK, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
                | P_Fld(ucR0GatingMCK, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
                | P_Fld(ucR0GatingP1MCK, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
                | P_Fld(ucR0GatingMCK, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
    //Gating RK0 UI
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(ucR0GatingB1P1UI, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
                | P_Fld(ucR0GatingB1UI, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
                | P_Fld(ucR0GatingB0P1UI, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
                | P_Fld(ucR0GatingB0UI, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));

    //Gating RK1 MCK
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0, P_Fld(ucR1GatingP1MCK, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
                | P_Fld(ucR1GatingMCK, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
                | P_Fld(ucR1GatingP1MCK, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
                | P_Fld(ucR1GatingMCK, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
    //Gating RK1 UI
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1, P_Fld(ucR1GatingB1P1UI, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
                | P_Fld(ucR1GatingB1UI, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED)
                | P_Fld(ucR1GatingB0P1UI, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1)
                | P_Fld(ucR1GatingB0UI, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED));

    //Gating RK0 PI
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQSIEN, P_Fld(ucR0GatingB1PI, SHURK0_DQSIEN_R0DQS1IEN)
                | P_Fld(ucR0GatingB0PI, SHURK0_DQSIEN_R0DQS0IEN));
    //Gating RK1 PI
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQSIEN, P_Fld(ucR1GatingB1PI, SHURK1_DQSIEN_R1DQS1IEN)
                | P_Fld(ucR1GatingB0PI, SHURK1_DQSIEN_R1DQS0IEN));
}


static void LegacyGatingDlyLP4_DDR1600(DRAMC_CTX_T *p)
{
    U8 ucR0GatingMCK = 0, ucR0GatingB0UI = 0, ucR0GatingB1UI = 0;
    U8 ucR0GatingP1MCK = 0, ucR0GatingB0P1UI = 0, ucR0GatingB1P1UI = 0;

    U8 ucR1GatingMCK = 0, ucR1GatingB0UI = 0, ucR1GatingB1UI = 0;
    U8 ucR1GatingP1MCK = 0, ucR1GatingB0P1UI = 0, ucR1GatingB1P1UI = 0;

    U8 ucR0GatingB0PI = 0, ucR0GatingB1PI = 0;
    U8 ucR1GatingB0PI = 0, ucR1GatingB1PI = 0;

    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        // DV sim mix mode = RK0_BYTE and RK1_NORMAL
        if ((p->dram_cbt_mode[RANK_0] == CBT_BYTE_MODE1) && (p->dram_cbt_mode[RANK_1] == CBT_NORMAL_MODE)) // DV sim mixed mode
        {
            //RK0
            ucR0GatingMCK = 0x2;
            ucR0GatingP1MCK = 0x2;
            ucR0GatingB0UI = 0x0;
            ucR0GatingB1UI = 0x0;
            ucR0GatingB0P1UI = 0x4;
            ucR0GatingB1P1UI = 0x4;
            //RK1
            ucR1GatingMCK = 0x2;
            ucR1GatingP1MCK = 0x2;
            ucR1GatingB0UI = 0x3;
            ucR1GatingB1UI = 0x2;
            ucR1GatingB0P1UI = 0x7;
            ucR1GatingB1P1UI = 0x6;
        }
        else
        {
            //RK0
            ucR0GatingMCK = 0x2;
            ucR0GatingP1MCK = 0x2;
            ucR0GatingB0UI = 0x0;
            ucR0GatingB1UI = 0x1;
            ucR0GatingB0P1UI = 0x4;
            ucR0GatingB1P1UI = 0x5;
            //RK1
            ucR1GatingMCK = 0x1;
            ucR1GatingP1MCK = 0x2;
            ucR1GatingB0UI = 0x7;
            ucR1GatingB1UI = 0x7;
            ucR1GatingB0P1UI = 0x3;
            ucR1GatingB1P1UI = 0x3;
        }
    }
    else
    {
        // Normal mode DRAM
        //Normal mode dram, B0/1 tDQSCK = 1.5ns
        //Byte/Mix mode dram, B0 tDQSCK = 1.5ns, B1 tDQSCK = 1.95ns
        //RK1, tDQSCK=3.5ns
        if (vGet_Div_Mode(p) == DIV4_MODE)
        {
            //RK0
            ucR0GatingMCK = 0x1;
            ucR0GatingP1MCK = 0x1;
            ucR0GatingB0UI = 0x4;
            ucR0GatingB1UI = 0x4;
            ucR0GatingB0P1UI = 0x6;
            ucR0GatingB1P1UI = 0x6;
            //RK1
            ucR1GatingMCK = 0x1;
            ucR1GatingP1MCK = 0x2;
            ucR1GatingB0UI = 0x7;
            ucR1GatingB1UI = 0x7;
            ucR1GatingB0P1UI = 0x1;
            ucR1GatingB1P1UI = 0x1;
        }
        else
        {
            //RK0
            ucR0GatingMCK = 0x2;
            ucR0GatingP1MCK = 0x3;
            ucR0GatingB0UI = 0x4;
            ucR0GatingB1UI = 0x4;
            ucR0GatingB0P1UI = 0x0;
            ucR0GatingB1P1UI = 0x0;
            //RK1
            ucR1GatingMCK = 0x2;
            ucR1GatingP1MCK = 0x3;
            ucR1GatingB0UI = 0x7;
            ucR1GatingB1UI = 0x7;
            ucR1GatingB0P1UI = 0x3;
            ucR1GatingB1P1UI = 0x3;
        }
    }

    //Gating PI
    if (vGet_Div_Mode(p) == DIV4_MODE)
    {
        ucR0GatingB0PI = 0xb;
        ucR0GatingB1PI = 0x0;
        ucR1GatingB0PI = 0x1;
        ucR1GatingB1PI = 0x1;
    }
    else
    {
        // for DDR1600 1:8 mode
        if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
        {
            if ((p->dram_cbt_mode[RANK_0] == CBT_BYTE_MODE1) && (p->dram_cbt_mode[RANK_1] == CBT_BYTE_MODE1)) // RK1 gating for byte mode
            {
                ucR0GatingB0PI = 0xa;
                ucR0GatingB1PI = 0x0;
                ucR1GatingB0PI = 0xc;
                ucR1GatingB1PI = 0x16;
            }
            else if ((p->dram_cbt_mode[RANK_0] == CBT_BYTE_MODE1) && (p->dram_cbt_mode[RANK_1] == CBT_NORMAL_MODE)) // DV sim mixed mode
            {
                ucR0GatingB0PI = 0xa;
                ucR0GatingB1PI = 0x1e;
                ucR1GatingB0PI = 0xc;
                ucR1GatingB1PI = 0xc;
            }
            else //RK0: normal and RK1: byte
            {
                ucR0GatingB0PI = 0xa;
                ucR0GatingB1PI = 0x0;
                ucR1GatingB0PI = 0xc;
                ucR1GatingB1PI = 0xc;

            }
        }
        else
        {
            ucR0GatingB0PI = 0x0;
            ucR0GatingB1PI = 0x0;
            ucR1GatingB0PI = 0x0;
            ucR1GatingB1PI = 0x0;
        }
    }

    //Gating RK0 MCK
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0, P_Fld(ucR0GatingP1MCK, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
                | P_Fld(ucR0GatingMCK, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
                | P_Fld(ucR0GatingP1MCK, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
                | P_Fld(ucR0GatingMCK, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
    //Gating RK0 UI
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(ucR0GatingB1P1UI, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
                | P_Fld(ucR0GatingB1UI, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
                | P_Fld(ucR0GatingB0P1UI, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
                | P_Fld(ucR0GatingB0UI, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));

    //Gating RK1 MCK
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0, P_Fld(ucR1GatingP1MCK, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
                | P_Fld(ucR1GatingMCK, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
                | P_Fld(ucR1GatingP1MCK, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
                | P_Fld(ucR1GatingMCK, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
    //Gating RK1 UI
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1, P_Fld(ucR1GatingB1P1UI, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
                | P_Fld(ucR1GatingB1UI, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED)
                | P_Fld(ucR1GatingB0P1UI, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1)
                | P_Fld(ucR1GatingB0UI, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED));

    //Gating RK0 PI
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQSIEN, P_Fld(ucR0GatingB1PI, SHURK0_DQSIEN_R0DQS1IEN)
                | P_Fld(ucR0GatingB0PI, SHURK0_DQSIEN_R0DQS0IEN));
    //Gating RK1 PI
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQSIEN, P_Fld(ucR1GatingB1PI, SHURK1_DQSIEN_R1DQS1IEN)
                | P_Fld(ucR1GatingB0PI, SHURK1_DQSIEN_R1DQS0IEN));
}

static void LegacyGatingDlyLP4_DDR2667(DRAMC_CTX_T *p)
{
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
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(0x4, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
                    | P_Fld(0x0, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
                    | P_Fld(0x4, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
                    | P_Fld(0x0, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
    }
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
                    | P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
                    | P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
                    | P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
    }
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1, P_Fld(0x2, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
                | P_Fld(0x6, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED)
                | P_Fld(0x2, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1)
                | P_Fld(0x6, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED));
}

static void LegacyGatingDlyLP4_DDR3200(DRAMC_CTX_T *p)
{
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
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0, P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
                    | P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
                    | P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
                    | P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1, P_Fld(0x4, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
                    | P_Fld(0x4, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1));
    }
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0, P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
                    | P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
                    | P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
                    | P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0, P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
                    | P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
                    | P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
                    | P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN0, P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN)
                    | P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN0, P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN)
                    | P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN));
    }
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
}

static void LegacyGatingDlyLP4_DDR3733(DRAMC_CTX_T *p)
{
    U8 ucR0GatingMCK = 0, ucR0GatingB0UI = 0, ucR0GatingB1UI = 0;
    U8 ucR1GatingMCK = 0, ucR1GatingB0UI = 0, ucR1GatingB1UI = 0;

    U8 ucR0GatingP1MCK = 0, ucR0GatingB0P1UI = 0, ucR0GatingB1P1UI = 0;
    U8 ucR1GatingP1MCK = 0, ucR1GatingB0P1UI = 0, ucR1GatingB1P1UI = 0;

    U8 ucR0GatingB0PI = 0, ucR0GatingB1PI = 0;
    U8 ucR1GatingB0PI = 0, ucR1GatingB1PI = 0;

    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        // Byte mode don't use. need fine-tune for DV sim
        //RK0
        ucR0GatingMCK = 0x3;
        ucR0GatingP1MCK = 0x3;
        ucR0GatingB0UI = 0x3;
        ucR0GatingB1UI = 0x3;
        ucR0GatingB0P1UI = 0x7;
        ucR0GatingB1P1UI = 0x7;
        ucR0GatingB0PI = 0x14;
        ucR0GatingB1PI = 0x14;
        //RK1
        ucR1GatingMCK = 0x4;
        ucR1GatingP1MCK = 0x4;
        ucR1GatingB0UI = 0x4;
        ucR1GatingB1UI = 0x4;
        ucR1GatingB0P1UI = 0x7;
        ucR1GatingB1P1UI = 0x7;
        ucR1GatingB0PI = 0x4;    
        ucR1GatingB1PI = 0x4;    
    }
    else
    {
        //RK0
        //Normal mode dram, B0/1 tDQSCK = 1.5ns
        //Byte/Mix mode dram, B0 tDQSCK = 1.5ns, B1 tDQSCK = 1.95ns
        ucR0GatingMCK = 0x2;
        ucR0GatingP1MCK = 0x2;
        ucR0GatingB0UI = 0x3;
        ucR0GatingB1UI = 0x3;
        ucR0GatingB0P1UI = 0x7;
        ucR0GatingB1P1UI = 0x7;
        ucR0GatingB0PI = 0x11;
        ucR0GatingB1PI = 0x11;
        //RK1, tDQSCK=3.5ns
        ucR1GatingMCK = 0x3;
        ucR1GatingP1MCK = 0x3;
        ucR1GatingB0UI = 0x2;
        ucR1GatingB1UI = 0x2;
        ucR1GatingB0P1UI = 0x6;
        ucR1GatingB1P1UI = 0x6;
        ucR1GatingB0PI = 0x1f;
        ucR1GatingB1PI = 0x1f;
    }

    #if ENABLE_READ_DBI
    ucR0GatingMCK++;
    ucR0GatingP1MCK++;
    ucR1GatingMCK++;
    ucR1GatingP1MCK++;
    #endif

    //Gating RK0 MCK
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0, P_Fld(ucR0GatingP1MCK, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
                | P_Fld(ucR0GatingMCK, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
                | P_Fld(ucR0GatingP1MCK, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
                | P_Fld(ucR0GatingMCK, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
    //Gating RK0 UI
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(ucR0GatingB1P1UI, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
                | P_Fld(ucR0GatingB1UI, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
                | P_Fld(ucR0GatingB0P1UI, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
                | P_Fld(ucR0GatingB0UI, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));

    //Gating RK1 MCK
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0, P_Fld(ucR1GatingP1MCK, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
                | P_Fld(ucR1GatingMCK, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
                | P_Fld(ucR1GatingP1MCK, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
                | P_Fld(ucR1GatingMCK, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
    //Gating RK1 UI
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1, P_Fld(ucR1GatingB1P1UI, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
                | P_Fld(ucR1GatingB1UI, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED)
                | P_Fld(ucR1GatingB0P1UI, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1)
                | P_Fld(ucR1GatingB0UI, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED));

    //Gating RK0 PI
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQSIEN, P_Fld(0x0, SHURK0_DQSIEN_R0DQS3IEN)
                | P_Fld(0x0, SHURK0_DQSIEN_R0DQS2IEN)
                | P_Fld(ucR0GatingB1PI, SHURK0_DQSIEN_R0DQS1IEN)
                | P_Fld(ucR0GatingB0PI, SHURK0_DQSIEN_R0DQS0IEN));

    //Gating RK1 PI
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQSIEN, P_Fld(0x0, SHURK1_DQSIEN_R1DQS3IEN)
                | P_Fld(0x0, SHURK1_DQSIEN_R1DQS2IEN)
                | P_Fld(ucR1GatingB1PI, SHURK1_DQSIEN_R1DQS1IEN)
                | P_Fld(ucR1GatingB0PI, SHURK1_DQSIEN_R1DQS0IEN));
}

#endif //LEGACY_GATING_DLY

#if LEGACY_RX_DLY
/* Legacy RX related delay initial settings:
 * RK#_RX_ARDQ#_F_DLY_B#, RK#_RX_ARDQ#_R_DLY_B# (DQ Rx per bit falling/rising edge delay line control)
 * RK#_RX_ARDQS#_F_DLY_B#, RK#_RX_ARDQS#_R_DLY_B# (DQS RX per bit falling/rising edge delay line control)
 * RK#_RX_ARDQM#_F_DLY_B#, RK#_RX_ARDQM#_R_DLY_B# (DQM RX per bit falling/rising edge delay line control)
 * TODO: Channel B's PHY, seems to be all used for LP3's DQ, DQS, DQM -> LP3's Channel B RX CA, CKE etc.. delay should be regarded as RX_DLY?
 */
static void LegacyRxDly_LP4_DDR2667(DRAMC_CTX_T *p)
{
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B0_DQ2, P_Fld(0xa, SHU_R0_B0_DQ2_RK0_RX_ARDQ1_R_DLY_B0)
                | P_Fld(0xa, SHU_R0_B0_DQ2_RK0_RX_ARDQ0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B0_DQ3, P_Fld(0xa, SHU_R0_B0_DQ3_RK0_RX_ARDQ3_R_DLY_B0)
                | P_Fld(0xa, SHU_R0_B0_DQ3_RK0_RX_ARDQ2_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B0_DQ4, P_Fld(0xa, SHU_R0_B0_DQ4_RK0_RX_ARDQ5_R_DLY_B0)
                | P_Fld(0xa, SHU_R0_B0_DQ4_RK0_RX_ARDQ4_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B0_DQ5, P_Fld(0xa, SHU_R0_B0_DQ5_RK0_RX_ARDQ7_R_DLY_B0)
                | P_Fld(0xa, SHU_R0_B0_DQ5_RK0_RX_ARDQ6_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B0_DQ6, P_Fld(0x15, SHU_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0)
                | P_Fld(0xa, SHU_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B1_DQ2, P_Fld(0xa, SHU_R0_B1_DQ2_RK0_RX_ARDQ1_R_DLY_B1)
                | P_Fld(0xa, SHU_R0_B1_DQ2_RK0_RX_ARDQ0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B1_DQ3, P_Fld(0xa, SHU_R0_B1_DQ3_RK0_RX_ARDQ3_R_DLY_B1)
                | P_Fld(0xa, SHU_R0_B1_DQ3_RK0_RX_ARDQ2_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B1_DQ4, P_Fld(0xa, SHU_R0_B1_DQ4_RK0_RX_ARDQ5_R_DLY_B1)
                | P_Fld(0xa, SHU_R0_B1_DQ4_RK0_RX_ARDQ4_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B1_DQ5, P_Fld(0xa, SHU_R0_B1_DQ5_RK0_RX_ARDQ7_R_DLY_B1)
                | P_Fld(0xa, SHU_R0_B1_DQ5_RK0_RX_ARDQ6_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B1_DQ6, P_Fld(0x15, SHU_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1)
                | P_Fld(0xa, SHU_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B0_DQ2, P_Fld(0xb, SHU_R1_B0_DQ2_RK1_RX_ARDQ1_R_DLY_B0)
                | P_Fld(0xb, SHU_R1_B0_DQ2_RK1_RX_ARDQ0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B0_DQ3, P_Fld(0xb, SHU_R1_B0_DQ3_RK1_RX_ARDQ3_R_DLY_B0)
                | P_Fld(0xb, SHU_R1_B0_DQ3_RK1_RX_ARDQ2_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B0_DQ4, P_Fld(0xb, SHU_R1_B0_DQ4_RK1_RX_ARDQ5_R_DLY_B0)
                | P_Fld(0xb, SHU_R1_B0_DQ4_RK1_RX_ARDQ4_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B0_DQ5, P_Fld(0xb, SHU_R1_B0_DQ5_RK1_RX_ARDQ7_R_DLY_B0)
                | P_Fld(0xb, SHU_R1_B0_DQ5_RK1_RX_ARDQ6_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B0_DQ6, P_Fld(0x16, SHU_R1_B0_DQ6_RK1_RX_ARDQS0_R_DLY_B0)
                | P_Fld(0xb, SHU_R1_B0_DQ6_RK1_RX_ARDQM0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B1_DQ2, P_Fld(0xb, SHU_R1_B1_DQ2_RK1_RX_ARDQ1_R_DLY_B1)
                | P_Fld(0xb, SHU_R1_B1_DQ2_RK1_RX_ARDQ0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B1_DQ3, P_Fld(0xb, SHU_R1_B1_DQ3_RK1_RX_ARDQ3_R_DLY_B1)
                | P_Fld(0xb, SHU_R1_B1_DQ3_RK1_RX_ARDQ2_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B1_DQ4, P_Fld(0xb, SHU_R1_B1_DQ4_RK1_RX_ARDQ5_R_DLY_B1)
                | P_Fld(0xb, SHU_R1_B1_DQ4_RK1_RX_ARDQ4_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B1_DQ5, P_Fld(0xb, SHU_R1_B1_DQ5_RK1_RX_ARDQ7_R_DLY_B1)
                | P_Fld(0xb, SHU_R1_B1_DQ5_RK1_RX_ARDQ6_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B1_DQ6, P_Fld(0x16, SHU_R1_B1_DQ6_RK1_RX_ARDQS0_R_DLY_B1)
                | P_Fld(0xb, SHU_R1_B1_DQ6_RK1_RX_ARDQM0_R_DLY_B1));
}

static void LegacyRxDly_LP4_DDR800(DRAMC_CTX_T *p)
{
    U8 u1Dq = 0x0;
    U8 u1Dqm = 0x0;
    U8 u1Dqs = 0x26;

    //RK0_B0
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B0_DQ2, P_Fld(u1Dq, SHU_R0_B0_DQ2_RK0_RX_ARDQ1_R_DLY_B0)
                | P_Fld(u1Dq, SHU_R0_B0_DQ2_RK0_RX_ARDQ0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B0_DQ3, P_Fld(u1Dq, SHU_R0_B0_DQ3_RK0_RX_ARDQ3_R_DLY_B0)
                | P_Fld(u1Dq, SHU_R0_B0_DQ3_RK0_RX_ARDQ2_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B0_DQ4, P_Fld(u1Dq, SHU_R0_B0_DQ4_RK0_RX_ARDQ5_R_DLY_B0)
                | P_Fld(u1Dq, SHU_R0_B0_DQ4_RK0_RX_ARDQ4_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B0_DQ5, P_Fld(u1Dq, SHU_R0_B0_DQ5_RK0_RX_ARDQ7_R_DLY_B0)
                | P_Fld(u1Dq, SHU_R0_B0_DQ5_RK0_RX_ARDQ6_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B0_DQ6, P_Fld(u1Dqs, SHU_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0)
                | P_Fld(u1Dqm, SHU_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0));
    //RK0_B1
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B1_DQ2, P_Fld(u1Dq, SHU_R0_B1_DQ2_RK0_RX_ARDQ1_R_DLY_B1)
                | P_Fld(u1Dq, SHU_R0_B1_DQ2_RK0_RX_ARDQ0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B1_DQ3, P_Fld(u1Dq, SHU_R0_B1_DQ3_RK0_RX_ARDQ3_R_DLY_B1)
                | P_Fld(u1Dq, SHU_R0_B1_DQ3_RK0_RX_ARDQ2_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B1_DQ4, P_Fld(u1Dq, SHU_R0_B1_DQ4_RK0_RX_ARDQ5_R_DLY_B1)
                | P_Fld(u1Dq, SHU_R0_B1_DQ4_RK0_RX_ARDQ4_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B1_DQ5, P_Fld(u1Dq, SHU_R0_B1_DQ5_RK0_RX_ARDQ7_R_DLY_B1)
                | P_Fld(u1Dq, SHU_R0_B1_DQ5_RK0_RX_ARDQ6_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B1_DQ6, P_Fld(u1Dqs, SHU_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1)
                | P_Fld(u1Dqm, SHU_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1));
    //RK1_B0
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B0_DQ2, P_Fld(u1Dq, SHU_R1_B0_DQ2_RK1_RX_ARDQ1_R_DLY_B0)
                | P_Fld(u1Dq, SHU_R1_B0_DQ2_RK1_RX_ARDQ0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B0_DQ3, P_Fld(u1Dq, SHU_R1_B0_DQ3_RK1_RX_ARDQ3_R_DLY_B0)
                | P_Fld(u1Dq, SHU_R1_B0_DQ3_RK1_RX_ARDQ2_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B0_DQ4, P_Fld(u1Dq, SHU_R1_B0_DQ4_RK1_RX_ARDQ5_R_DLY_B0)
                | P_Fld(u1Dq, SHU_R1_B0_DQ4_RK1_RX_ARDQ4_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B0_DQ5, P_Fld(u1Dq, SHU_R1_B0_DQ5_RK1_RX_ARDQ7_R_DLY_B0)
                | P_Fld(u1Dq, SHU_R1_B0_DQ5_RK1_RX_ARDQ6_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B0_DQ6, P_Fld(u1Dqs, SHU_R1_B0_DQ6_RK1_RX_ARDQS0_R_DLY_B0)
                | P_Fld(u1Dqm, SHU_R1_B0_DQ6_RK1_RX_ARDQM0_R_DLY_B0));
    //RK1_B1
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B1_DQ2, P_Fld(u1Dq, SHU_R1_B1_DQ2_RK1_RX_ARDQ1_R_DLY_B1)
                | P_Fld(u1Dq, SHU_R1_B1_DQ2_RK1_RX_ARDQ0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B1_DQ3, P_Fld(u1Dq, SHU_R1_B1_DQ3_RK1_RX_ARDQ3_R_DLY_B1)
                | P_Fld(u1Dq, SHU_R1_B1_DQ3_RK1_RX_ARDQ2_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B1_DQ4, P_Fld(u1Dq, SHU_R1_B1_DQ4_RK1_RX_ARDQ5_R_DLY_B1)
                | P_Fld(u1Dq, SHU_R1_B1_DQ4_RK1_RX_ARDQ4_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B1_DQ5, P_Fld(u1Dq, SHU_R1_B1_DQ5_RK1_RX_ARDQ7_R_DLY_B1)
                | P_Fld(u1Dq, SHU_R1_B1_DQ5_RK1_RX_ARDQ6_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B1_DQ6, P_Fld(u1Dqs, SHU_R1_B1_DQ6_RK1_RX_ARDQS0_R_DLY_B1)
                | P_Fld(u1Dqm, SHU_R1_B1_DQ6_RK1_RX_ARDQM0_R_DLY_B1));
}

static void LegacyRxDly_LP4_DDR1600(DRAMC_CTX_T *p)
{
    U8 u1Rk0_Dq, u1Rk0_Dq5_6;
    U8 u1Rk0_Dqm;
    U8 u1Rk0_Dqs;
    U8 u1Rk1_Dq;
    U8 u1Rk1_Dqm;
    U8 u1Rk1_Dqs;

    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        u1Rk0_Dq = 0x0;
        u1Rk0_Dq5_6 = 0x2;
        u1Rk0_Dqm = 0x0;
        u1Rk0_Dqs = 0x7;
        u1Rk1_Dq = 0x0;
        u1Rk1_Dqm = 0x0;
        u1Rk1_Dqs = 0x5;
    }
    else
    {
        u1Rk0_Dq = 0x4;
        u1Rk0_Dq5_6 = 0x4;
        u1Rk0_Dqm = 0x4;
        u1Rk0_Dqs = 0x15;
        u1Rk1_Dq = 0x5;
        u1Rk1_Dqm = 0x5;
        u1Rk1_Dqs = 0x16;
    }

    //RK0_B0
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B0_DQ2, P_Fld(u1Rk0_Dq, SHU_R0_B0_DQ2_RK0_RX_ARDQ1_R_DLY_B0)
                | P_Fld(u1Rk0_Dq, SHU_R0_B0_DQ2_RK0_RX_ARDQ0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B0_DQ3, P_Fld(u1Rk0_Dq, SHU_R0_B0_DQ3_RK0_RX_ARDQ3_R_DLY_B0)
                | P_Fld(u1Rk0_Dq, SHU_R0_B0_DQ3_RK0_RX_ARDQ2_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B0_DQ4, P_Fld(u1Rk0_Dq5_6, SHU_R0_B0_DQ4_RK0_RX_ARDQ5_R_DLY_B0)
                | P_Fld(u1Rk0_Dq, SHU_R0_B0_DQ4_RK0_RX_ARDQ4_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B0_DQ5, P_Fld(u1Rk0_Dq, SHU_R0_B0_DQ5_RK0_RX_ARDQ7_R_DLY_B0)
                | P_Fld(u1Rk0_Dq5_6, SHU_R0_B0_DQ5_RK0_RX_ARDQ6_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B0_DQ6, P_Fld(u1Rk0_Dqs, SHU_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0)
                | P_Fld(u1Rk0_Dqm, SHU_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0));
    //RK0_B1
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B1_DQ2, P_Fld(u1Rk0_Dq, SHU_R0_B1_DQ2_RK0_RX_ARDQ1_R_DLY_B1)
                | P_Fld(u1Rk0_Dq, SHU_R0_B1_DQ2_RK0_RX_ARDQ0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B1_DQ3, P_Fld(u1Rk0_Dq, SHU_R0_B1_DQ3_RK0_RX_ARDQ3_R_DLY_B1)
                | P_Fld(u1Rk0_Dq, SHU_R0_B1_DQ3_RK0_RX_ARDQ2_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B1_DQ4, P_Fld(u1Rk0_Dq5_6, SHU_R0_B1_DQ4_RK0_RX_ARDQ5_R_DLY_B1)
                | P_Fld(u1Rk0_Dq, SHU_R0_B1_DQ4_RK0_RX_ARDQ4_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B1_DQ5, P_Fld(u1Rk0_Dq, SHU_R0_B1_DQ5_RK0_RX_ARDQ7_R_DLY_B1)
                | P_Fld(u1Rk0_Dq5_6, SHU_R0_B1_DQ5_RK0_RX_ARDQ6_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B1_DQ6, P_Fld(u1Rk0_Dqs, SHU_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1)
                | P_Fld(u1Rk0_Dqm, SHU_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1));
    //RK1_B0
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B0_DQ2, P_Fld(u1Rk1_Dq, SHU_R1_B0_DQ2_RK1_RX_ARDQ1_R_DLY_B0)
                | P_Fld(u1Rk1_Dq, SHU_R1_B0_DQ2_RK1_RX_ARDQ0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B0_DQ3, P_Fld(u1Rk1_Dq, SHU_R1_B0_DQ3_RK1_RX_ARDQ3_R_DLY_B0)
                | P_Fld(u1Rk1_Dq, SHU_R1_B0_DQ3_RK1_RX_ARDQ2_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B0_DQ4, P_Fld(u1Rk1_Dq, SHU_R1_B0_DQ4_RK1_RX_ARDQ5_R_DLY_B0)
                | P_Fld(u1Rk1_Dq, SHU_R1_B0_DQ4_RK1_RX_ARDQ4_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B0_DQ5, P_Fld(u1Rk1_Dq, SHU_R1_B0_DQ5_RK1_RX_ARDQ7_R_DLY_B0)
                | P_Fld(u1Rk1_Dq, SHU_R1_B0_DQ5_RK1_RX_ARDQ6_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B0_DQ6, P_Fld(u1Rk1_Dqs, SHU_R1_B0_DQ6_RK1_RX_ARDQS0_R_DLY_B0)
                | P_Fld(u1Rk1_Dqm, SHU_R1_B0_DQ6_RK1_RX_ARDQM0_R_DLY_B0));
    //RK1_B1
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B1_DQ2, P_Fld(u1Rk1_Dq, SHU_R1_B1_DQ2_RK1_RX_ARDQ1_R_DLY_B1)
                | P_Fld(u1Rk1_Dq, SHU_R1_B1_DQ2_RK1_RX_ARDQ0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B1_DQ3, P_Fld(u1Rk1_Dq, SHU_R1_B1_DQ3_RK1_RX_ARDQ3_R_DLY_B1)
                | P_Fld(u1Rk1_Dq, SHU_R1_B1_DQ3_RK1_RX_ARDQ2_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B1_DQ4, P_Fld(u1Rk1_Dq, SHU_R1_B1_DQ4_RK1_RX_ARDQ5_R_DLY_B1)
                | P_Fld(u1Rk1_Dq, SHU_R1_B1_DQ4_RK1_RX_ARDQ4_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B1_DQ5, P_Fld(u1Rk1_Dq, SHU_R1_B1_DQ5_RK1_RX_ARDQ7_R_DLY_B1)
                | P_Fld(u1Rk1_Dq, SHU_R1_B1_DQ5_RK1_RX_ARDQ6_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B1_DQ6, P_Fld(u1Rk1_Dqs, SHU_R1_B1_DQ6_RK1_RX_ARDQS0_R_DLY_B1)
                | P_Fld(u1Rk1_Dqm, SHU_R1_B1_DQ6_RK1_RX_ARDQM0_R_DLY_B1));    
}

static void LegacyRxDly_LP4_DDR3200(DRAMC_CTX_T *p)
{
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B0_DQ2, P_Fld(0xa, SHU_R0_B0_DQ2_RK0_RX_ARDQ1_R_DLY_B0)
                | P_Fld(0xa, SHU_R0_B0_DQ2_RK0_RX_ARDQ0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B0_DQ3, P_Fld(0xa, SHU_R0_B0_DQ3_RK0_RX_ARDQ3_R_DLY_B0)
                | P_Fld(0xa, SHU_R0_B0_DQ3_RK0_RX_ARDQ2_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B0_DQ4, P_Fld(0xa, SHU_R0_B0_DQ4_RK0_RX_ARDQ5_R_DLY_B0)
                | P_Fld(0xa, SHU_R0_B0_DQ4_RK0_RX_ARDQ4_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B0_DQ5, P_Fld(0xa, SHU_R0_B0_DQ5_RK0_RX_ARDQ7_R_DLY_B0)
                | P_Fld(0xa, SHU_R0_B0_DQ5_RK0_RX_ARDQ6_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B0_DQ6, P_Fld(0xc, SHU_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0)
                | P_Fld(0xa, SHU_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B0_DQ2, P_Fld(0xb, SHU_R1_B0_DQ2_RK1_RX_ARDQ1_R_DLY_B0)
                | P_Fld(0xb, SHU_R1_B0_DQ2_RK1_RX_ARDQ0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B0_DQ3, P_Fld(0xb, SHU_R1_B0_DQ3_RK1_RX_ARDQ3_R_DLY_B0)
                | P_Fld(0xb, SHU_R1_B0_DQ3_RK1_RX_ARDQ2_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B0_DQ4, P_Fld(0xb, SHU_R1_B0_DQ4_RK1_RX_ARDQ5_R_DLY_B0)
                | P_Fld(0xb, SHU_R1_B0_DQ4_RK1_RX_ARDQ4_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B0_DQ5, P_Fld(0xb, SHU_R1_B0_DQ5_RK1_RX_ARDQ7_R_DLY_B0)
                | P_Fld(0xb, SHU_R1_B0_DQ5_RK1_RX_ARDQ6_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B0_DQ6, P_Fld(0xd, SHU_R1_B0_DQ6_RK1_RX_ARDQS0_R_DLY_B0)
                | P_Fld(0xb, SHU_R1_B0_DQ6_RK1_RX_ARDQM0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B1_DQ2, P_Fld(0xa, SHU_R0_B1_DQ2_RK0_RX_ARDQ1_R_DLY_B1)
                | P_Fld(0xa, SHU_R0_B1_DQ2_RK0_RX_ARDQ0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B1_DQ3, P_Fld(0xa, SHU_R0_B1_DQ3_RK0_RX_ARDQ3_R_DLY_B1)
                | P_Fld(0xa, SHU_R0_B1_DQ3_RK0_RX_ARDQ2_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B1_DQ4, P_Fld(0xa, SHU_R0_B1_DQ4_RK0_RX_ARDQ5_R_DLY_B1)
                | P_Fld(0xa, SHU_R0_B1_DQ4_RK0_RX_ARDQ4_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B1_DQ5, P_Fld(0xa, SHU_R0_B1_DQ5_RK0_RX_ARDQ7_R_DLY_B1)
                | P_Fld(0xa, SHU_R0_B1_DQ5_RK0_RX_ARDQ6_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B1_DQ6, P_Fld(0xc, SHU_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1)
                | P_Fld(0xa, SHU_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B1_DQ2, P_Fld(0xb, SHU_R1_B1_DQ2_RK1_RX_ARDQ1_R_DLY_B1)
                | P_Fld(0xb, SHU_R1_B1_DQ2_RK1_RX_ARDQ0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B1_DQ3, P_Fld(0xb, SHU_R1_B1_DQ3_RK1_RX_ARDQ3_R_DLY_B1)
                | P_Fld(0xb, SHU_R1_B1_DQ3_RK1_RX_ARDQ2_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B1_DQ4, P_Fld(0xb, SHU_R1_B1_DQ4_RK1_RX_ARDQ5_R_DLY_B1)
                | P_Fld(0xb, SHU_R1_B1_DQ4_RK1_RX_ARDQ4_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B1_DQ5, P_Fld(0xb, SHU_R1_B1_DQ5_RK1_RX_ARDQ7_R_DLY_B1)
                | P_Fld(0xb, SHU_R1_B1_DQ5_RK1_RX_ARDQ6_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B1_DQ6, P_Fld(0xd, SHU_R1_B1_DQ6_RK1_RX_ARDQS0_R_DLY_B1)
                | P_Fld(0xb, SHU_R1_B1_DQ6_RK1_RX_ARDQM0_R_DLY_B1));
}

static void LegacyRxDly_LP4_DDR3733(DRAMC_CTX_T *p)
{
    U8 u1Dq = 0xf;
    U8 u1Dqm = 0xf;
    U8 u1Dqs = 0x0;

    //RK0_B0
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B0_DQ2, P_Fld(u1Dq, SHU_R0_B0_DQ2_RK0_RX_ARDQ1_R_DLY_B0)
                | P_Fld(u1Dq, SHU_R0_B0_DQ2_RK0_RX_ARDQ0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B0_DQ3, P_Fld(u1Dq, SHU_R0_B0_DQ3_RK0_RX_ARDQ3_R_DLY_B0)
                | P_Fld(u1Dq, SHU_R0_B0_DQ3_RK0_RX_ARDQ2_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B0_DQ4, P_Fld(u1Dq, SHU_R0_B0_DQ4_RK0_RX_ARDQ5_R_DLY_B0)
                | P_Fld(u1Dq, SHU_R0_B0_DQ4_RK0_RX_ARDQ4_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B0_DQ5, P_Fld(u1Dq, SHU_R0_B0_DQ5_RK0_RX_ARDQ7_R_DLY_B0)
                | P_Fld(u1Dq, SHU_R0_B0_DQ5_RK0_RX_ARDQ6_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B0_DQ6, P_Fld(u1Dqs, SHU_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0)
                | P_Fld(u1Dqm, SHU_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0));
    //RK1_B0
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B0_DQ2, P_Fld(u1Dq, SHU_R1_B0_DQ2_RK1_RX_ARDQ1_R_DLY_B0)
                | P_Fld(u1Dq, SHU_R1_B0_DQ2_RK1_RX_ARDQ0_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B0_DQ3, P_Fld(u1Dq, SHU_R1_B0_DQ3_RK1_RX_ARDQ3_R_DLY_B0)
                | P_Fld(u1Dq, SHU_R1_B0_DQ3_RK1_RX_ARDQ2_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B0_DQ4, P_Fld(u1Dq, SHU_R1_B0_DQ4_RK1_RX_ARDQ5_R_DLY_B0)
                | P_Fld(u1Dq, SHU_R1_B0_DQ4_RK1_RX_ARDQ4_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B0_DQ5, P_Fld(u1Dq, SHU_R1_B0_DQ5_RK1_RX_ARDQ7_R_DLY_B0)
                | P_Fld(u1Dq, SHU_R1_B0_DQ5_RK1_RX_ARDQ6_R_DLY_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B0_DQ6, P_Fld(u1Dqs, SHU_R1_B0_DQ6_RK1_RX_ARDQS0_R_DLY_B0)
                | P_Fld(u1Dqm, SHU_R1_B0_DQ6_RK1_RX_ARDQM0_R_DLY_B0));
    //RK0_B1
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B1_DQ2, P_Fld(u1Dq, SHU_R0_B1_DQ2_RK0_RX_ARDQ1_R_DLY_B1)
                | P_Fld(u1Dq, SHU_R0_B1_DQ2_RK0_RX_ARDQ0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B1_DQ3, P_Fld(u1Dq, SHU_R0_B1_DQ3_RK0_RX_ARDQ3_R_DLY_B1)
                | P_Fld(u1Dq, SHU_R0_B1_DQ3_RK0_RX_ARDQ2_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B1_DQ4, P_Fld(u1Dq, SHU_R0_B1_DQ4_RK0_RX_ARDQ5_R_DLY_B1)
                | P_Fld(u1Dq, SHU_R0_B1_DQ4_RK0_RX_ARDQ4_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B1_DQ5, P_Fld(u1Dq, SHU_R0_B1_DQ5_RK0_RX_ARDQ7_R_DLY_B1)
                | P_Fld(u1Dq, SHU_R0_B1_DQ5_RK0_RX_ARDQ6_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B1_DQ6, P_Fld(u1Dqs, SHU_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1)
                | P_Fld(u1Dqm, SHU_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1));
    //RK1_B1
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B1_DQ2, P_Fld(u1Dq, SHU_R1_B1_DQ2_RK1_RX_ARDQ1_R_DLY_B1)
                | P_Fld(u1Dq, SHU_R1_B1_DQ2_RK1_RX_ARDQ0_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B1_DQ3, P_Fld(u1Dq, SHU_R1_B1_DQ3_RK1_RX_ARDQ3_R_DLY_B1)
                | P_Fld(u1Dq, SHU_R1_B1_DQ3_RK1_RX_ARDQ2_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B1_DQ4, P_Fld(u1Dq, SHU_R1_B1_DQ4_RK1_RX_ARDQ5_R_DLY_B1)
                | P_Fld(u1Dq, SHU_R1_B1_DQ4_RK1_RX_ARDQ4_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B1_DQ5, P_Fld(u1Dq, SHU_R1_B1_DQ5_RK1_RX_ARDQ7_R_DLY_B1)
                | P_Fld(u1Dq, SHU_R1_B1_DQ5_RK1_RX_ARDQ6_R_DLY_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B1_DQ6, P_Fld(u1Dqs, SHU_R1_B1_DQ6_RK1_RX_ARDQS0_R_DLY_B1)
                | P_Fld(u1Dqm, SHU_R1_B1_DQ6_RK1_RX_ARDQM0_R_DLY_B1));
}

#endif //LEGACY_RX_DLY

#if LEGACY_DAT_LAT
static void LegacyDatlatLP4_DDR3733(DRAMC_CTX_T *p)
{
    // set by DramcRxdqsGatingPostProcess
#if 0
    vIO32WriteFldMulti(DRAMC_REG_SHU_RANKCTL, P_Fld(0x7, SHU_RANKCTL_RANKINCTL_PHY)
                | P_Fld(0x5, SHU_RANKCTL_RANKINCTL_ROOT1)
                | P_Fld(0x5, SHU_RANKCTL_RANKINCTL));
#endif

    // DATLAT init by AcTiming Table
    /*vIO32WriteFldMulti(DRAMC_REG_SHU_CONF1, P_Fld(0x11, SHU_CONF1_DATLAT)
                 | P_Fld(0xf, SHU_CONF1_DATLAT_DSEL)
                 | P_Fld(0xf, SHU_CONF1_DATLAT_DSEL_PHY));*/

    //DATLAT 1:8 mode DDR3733
    vIO32WriteFldMulti(DDRPHY_SHU_B0_DQ7, P_Fld(0x0, SHU_B0_DQ7_R_DMRDSEL_DIV2_OPT)
                | P_Fld(0x0, SHU_B0_DQ7_R_DMRDSEL_LOBYTE_OPT)
                | P_Fld(0x0, SHU_B0_DQ7_R_DMRDSEL_HIBYTE_OPT));

    //RDATA PIPE from Ton
    vIO32WriteFldMulti(DRAMC_REG_SHU_PIPE, P_Fld(0x1, SHU_PIPE_READ_START_EXTEND1)
                | P_Fld(0x1, SHU_PIPE_DLE_LAST_EXTEND1)
                | P_Fld(0x1, SHU_PIPE_READ_START_EXTEND2)
                | P_Fld(0x1, SHU_PIPE_DLE_LAST_EXTEND2)
                | P_Fld(0x0, SHU_PIPE_READ_START_EXTEND3)
                | P_Fld(0x0, SHU_PIPE_DLE_LAST_EXTEND3));
}
#endif

static void DramcSetting_LP4_TX_Delay_DDR3733(DRAMC_CTX_T *p, DRAM_RANK_T eRank)
{
    U8 ucR0TxdlyOendq = 0, ucR0Txdlydq = 0;
    U8 ucR1TxdlyOendq = 0, ucR1Txdlydq = 0;

    if (eRank == RANK_0)
    {
        if (vGet_Div_Mode(p) == DIV4_MODE)
        {
            ucR0TxdlyOendq = 0x3; // don't use
            ucR0Txdlydq = 0x4; // don't use
        }
        else
        {
            ucR0TxdlyOendq = 0x3;
            ucR0Txdlydq = 0x4;
        }

        if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
        {
            vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3)
                        | P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2)
                        | P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1)
                        | P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0)
                        | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ0_TXDLY_DQ3)
                        | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ0_TXDLY_DQ2)
                        | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ0_TXDLY_DQ1)
                        | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ0_TXDLY_DQ0));
            vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3)
                        | P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2)
                        | P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1)
                        | P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0)
                        | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ1_TXDLY_DQM3)
                        | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ1_TXDLY_DQM2)
                        | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ1_TXDLY_DQM1)
                        | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ1_TXDLY_DQM0));
        }
        else
        {
            vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3)
                        | P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2)
                        | P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1)
                        | P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0)
                        | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ0_TXDLY_DQ3)
                        | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ0_TXDLY_DQ2)
                        | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ0_TXDLY_DQ1)
                        | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ0_TXDLY_DQ0));
            vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3)
                        | P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2)
                        | P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1)
                        | P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0)
                        | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ1_TXDLY_DQM3)
                        | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ1_TXDLY_DQM2)
                        | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ1_TXDLY_DQM1)
                        | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ1_TXDLY_DQM0));
        }
        if (vGet_Div_Mode(p) == DIV4_MODE)
        {
            ucR0TxdlyOendq = 0x2; //don't use
            ucR0Txdlydq = 0x1; //don't use
        }
        else
        {
            ucR0TxdlyOendq = 0x7; // TX OE -3UI
            ucR0Txdlydq = 0x2;
        }
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ2, P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ2_DLY_OEN_DQ3)
                    | P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ2_DLY_OEN_DQ2)
                    | P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ2_DLY_OEN_DQ1)
                    | P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ2_DLY_OEN_DQ0)
                    | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ2_DLY_DQ3)
                    | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ2_DLY_DQ2)
                    | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ2_DLY_DQ1)
                    | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ2_DLY_DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ3, P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ3_DLY_OEN_DQM3)
                    | P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ3_DLY_OEN_DQM2)
                    | P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ3_DLY_OEN_DQM1)
                    | P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ3_DLY_OEN_DQM0)
                    | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ3_DLY_DQM3)
                    | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ3_DLY_DQM2)
                    | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ3_DLY_DQM1)
                    | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ3_DLY_DQM0));
    }
    else //RANK_1
    {
        if (vGet_Div_Mode(p) == DIV4_MODE)
        {
            ucR1TxdlyOendq = 0x3; //don't use
            ucR1Txdlydq = 0x4; //don't use
        }
        else
        {
            ucR1TxdlyOendq = 0x4;
            ucR1Txdlydq = 0x4;
        }
        if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
        {
            vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3)
                        | P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2)
                        | P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1)
                        | P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0)
                        | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ0_TX_DLY_R1DQ3)
                        | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ0_TX_DLY_R1DQ2)
                        | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ0_TX_DLY_R1DQ1)
                        | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ0_TX_DLY_R1DQ0));
            vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3)
                        | P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2)
                        | P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1)
                        | P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0)
                        | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ1_TX_DLY_R1DQM3)
                        | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ1_TX_DLY_R1DQM2)
                        | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ1_TX_DLY_R1DQM1)
                        | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ1_TX_DLY_R1DQM0));
        }
        else
        {
            vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3)
                        | P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2)
                        | P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1)
                        | P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0)
                        | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ0_TX_DLY_R1DQ3)
                        | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ0_TX_DLY_R1DQ2)
                        | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ0_TX_DLY_R1DQ1)
                        | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ0_TX_DLY_R1DQ0));
            vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3)
                        | P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2)
                        | P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1)
                        | P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0)
                        | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ1_TX_DLY_R1DQM3)
                        | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ1_TX_DLY_R1DQM2)
                        | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ1_TX_DLY_R1DQM1)
                        | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ1_TX_DLY_R1DQM0));
        }
        if (vGet_Div_Mode(p) == DIV4_MODE)
        {
            ucR1TxdlyOendq = 0x3; //don't use
            ucR1Txdlydq = 0x2; //don't use
        }
        else
        {
            ucR1TxdlyOendq = 0x0;
            ucR1Txdlydq = 0x3;
        }
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ2, P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ3)
                    | P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ2)
                    | P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ1)
                    | P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ0)
                    | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ2_DLY_R1DQ3)
                    | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ2_DLY_R1DQ2)
                    | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ2_DLY_R1DQ1)
                    | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ2_DLY_R1DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ3, P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM3)
                    | P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM2)
                    | P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM1)
                    | P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM0)
                    | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ3_DLY_R1DQM3)
                    | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ3_DLY_R1DQM2)
                    | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ3_DLY_R1DQM1)
                    | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ3_DLY_R1DQM0));
    }
}

static void DramcSetting_Olympus_LP4_ByteMode_DDR4266(DRAMC_CTX_T *p)
{
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS0, P_Fld(0x4, SHU_SELPH_DQS0_TXDLY_OEN_DQS3)
                | P_Fld(0x4, SHU_SELPH_DQS0_TXDLY_OEN_DQS2)
                | P_Fld(0x4, SHU_SELPH_DQS0_TXDLY_OEN_DQS1)
                | P_Fld(0x4, SHU_SELPH_DQS0_TXDLY_OEN_DQS0)
                | P_Fld(0x4, SHU_SELPH_DQS0_TXDLY_DQS3)
                | P_Fld(0x4, SHU_SELPH_DQS0_TXDLY_DQS2)
                | P_Fld(0x4, SHU_SELPH_DQS0_TXDLY_DQS1)
                | P_Fld(0x4, SHU_SELPH_DQS0_TXDLY_DQS0));
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS1, P_Fld(0x2, SHU_SELPH_DQS1_DLY_OEN_DQS3)
                | P_Fld(0x2, SHU_SELPH_DQS1_DLY_OEN_DQS2)
                | P_Fld(0x2, SHU_SELPH_DQS1_DLY_OEN_DQS1)
                | P_Fld(0x2, SHU_SELPH_DQS1_DLY_OEN_DQS0)
                | P_Fld(0x5, SHU_SELPH_DQS1_DLY_DQS3)
                | P_Fld(0x5, SHU_SELPH_DQS1_DLY_DQS2)
                | P_Fld(0x5, SHU_SELPH_DQS1_DLY_DQS1)
                | P_Fld(0x5, SHU_SELPH_DQS1_DLY_DQS0));
}

static void DramcSetting_Olympus_LP4_ByteMode_DDR3733(DRAMC_CTX_T *p)
{
    DramcSetting_LP4_TX_Delay_DDR3733(p, RANK_0); // TX DQ/DQM MCK/UI
    DramcSetting_LP4_TX_Delay_DDR3733(p, RANK_1); // TX DQ/DQM MCK/UI
    // TX DQ/DQM PI
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B0_DQ7, P_Fld(0xe, SHU_R0_B0_DQ7_RK0_ARPI_DQM_B0)
                | P_Fld(0x13, SHU_R0_B0_DQ7_RK0_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B1_DQ7, P_Fld(0xe, SHU_R0_B1_DQ7_RK0_ARPI_DQM_B1)
                | P_Fld(0x13, SHU_R0_B1_DQ7_RK0_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B0_DQ7, P_Fld(0x19, SHU_R1_B0_DQ7_RK1_ARPI_DQM_B0)
                | P_Fld(0x22, SHU_R1_B0_DQ7_RK1_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B1_DQ7, P_Fld(0x19, SHU_R1_B1_DQ7_RK1_ARPI_DQM_B1)
                | P_Fld(0x22, SHU_R1_B1_DQ7_RK1_ARPI_DQ_B1));

    //TX DQS MCK by WriteLeveling
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS0, P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS3)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS2)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS1)
                | P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS0)
                | P_Fld(0x4, SHU_SELPH_DQS0_TXDLY_DQS3)
                | P_Fld(0x4, SHU_SELPH_DQS0_TXDLY_DQS2)
                | P_Fld(0x4, SHU_SELPH_DQS0_TXDLY_DQS1)
                | P_Fld(0x4, SHU_SELPH_DQS0_TXDLY_DQS0));
    //TX DQS UI by WriteLeveling
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS1, P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS3)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS2)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS1)
                | P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS0)
                | P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS3)
                | P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS2)
                | P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS1)
                | P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS0));

#if LEGACY_GATING_DLY
    /*vIO32WriteFldMulti(DRAMC_REG_SHU_PHY_RX_CTRL, P_Fld(0x1, SHU_PHY_RX_CTRL_RANK_RXDLY_UPDLAT_EN)
                | P_Fld(0x2, SHU_PHY_RX_CTRL_RANK_RXDLY_UPD_OFFSET)
                | P_Fld(0x2, SHU_PHY_RX_CTRL_RX_IN_GATE_EN_PRE_OFFSET)
                | P_Fld(0x1, SHU_PHY_RX_CTRL_RX_IN_GATE_EN_HEAD)
                | P_Fld(0x0, SHU_PHY_RX_CTRL_RX_IN_GATE_EN_TAIL)
                | P_Fld(0x2, SHU_PHY_RX_CTRL_RX_IN_BUFF_EN_HEAD)
                | P_Fld(0x0, SHU_PHY_RX_CTRL_RX_IN_BUFF_EN_TAIL));*/

    LegacyGatingDlyLP4_DDR3733(p);
#endif

#if LEGACY_RX_DLY
    LegacyRxDly_LP4_DDR3733(p);
#endif


#if LEGACY_DAT_LAT // TODO: Darren
    LegacyDatlatLP4_DDR3733(p);
#endif
}


static void DramcSetting_Olympus_LP4_ByteMode_DDR2667(DRAMC_CTX_T *p)
{
    ////DRAMC0-SHU2
#if LEGACY_TDQSCK_PRECAL
    LegacyPreCalLP4_DDR2667(p);
#endif
    //vIO32WriteFldAlign(DRAMC_REG_SHU_CONF2, 0x54, SHU_CONF2_FSPCHG_PRDCNT);//ACTiming related -> set in UpdateACTiming_Reg()
#if 0
    vIO32WriteFldMulti(DRAMC_REG_SHU_RANKCTL, P_Fld(0x4, SHU_RANKCTL_RANKINCTL_PHY)
                | P_Fld(0x2, SHU_RANKCTL_RANKINCTL_ROOT1)
                | P_Fld(0x2, SHU_RANKCTL_RANKINCTL));
#endif
    vIO32WriteFldMulti(DRAMC_REG_SHU_CKECTRL, P_Fld(0x3, SHU_CKECTRL_TCKESRX));
                //| P_Fld(0x3, SHU_CKECTRL_CKEPRD));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ODTCTRL, P_Fld(0x1, SHU_ODTCTRL_RODTE)
                | P_Fld(0x1, SHU_ODTCTRL_RODTE2)
                //| P_Fld(0x4, SHU_ODTCTRL_RODT) //Set in UpdateACTimingReg()
                | P_Fld(0x1, SHU_ODTCTRL_ROEN));
    vIO32WriteFldAlign(DDRPHY_SHU_B0_DQ7, 0x1, SHU_B0_DQ7_R_DMRODTEN_B0);
    vIO32WriteFldAlign(DDRPHY_SHU_B1_DQ7, 0x1, SHU_B1_DQ7_R_DMRODTEN_B1);
#if LEGACY_TX_TRACK
    LegacyTxTrackLP4_DDR2667(p);
#endif
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

    vIO32WriteFldAlign(DRAMC_REG_SHU_DQS2DQ_TX, 0x7, SHU_DQS2DQ_TX_OE2DQ_OFFSET);
    //vIO32WriteFldAlign(DRAMC_REG_SHU_HWSET_MR2, 0x24, SHU_HWSET_MR2_HWSET_MR2_OP);
    //vIO32WriteFldAlign(DRAMC_REG_SHU_HWSET_MR13, 0xc8, SHU_HWSET_MR13_HWSET_MR13_OP);
    //vIO32WriteFldAlign(DRAMC_REG_SHU_HWSET_VRCG, 0xc0, SHU_HWSET_VRCG_HWSET_VRCG_OP);
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQSIEN, P_Fld(0x19, SHURK0_DQSIEN_R0DQS1IEN)
                | P_Fld(0x19, SHURK0_DQSIEN_R0DQS0IEN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_PI, P_Fld(0x14, SHURK0_PI_RK0_ARPI_DQM_B1)
                | P_Fld(0x14, SHURK0_PI_RK0_ARPI_DQM_B0)
                | P_Fld(0x14, SHURK0_PI_RK0_ARPI_DQ_B1)
                | P_Fld(0x14, SHURK0_PI_RK0_ARPI_DQ_B0));

    #if LEGACY_GATING_DLY
    LegacyGatingDlyLP4_DDR2667(p);
    #endif

    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3)
                    | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2)
                    | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1)
                    | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0)
                    | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ3)
                    | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ2)
                    | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ1)
                    | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3)
                    | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2)
                    | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1)
                    | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0)
                    | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM3)
                    | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM2)
                    | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM1)
                    | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM0));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3)
                    | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2)
                    | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1)
                    | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0)
                    | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ3)
                    | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ2)
                    | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ1)
                    | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3)
                    | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2)
                    | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1)
                    | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0)
                    | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM3)
                    | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM2)
                    | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM1)
                    | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM0));
    }
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ2, P_Fld(0x6, SHURK0_SELPH_DQ2_DLY_OEN_DQ3)
                | P_Fld(0x6, SHURK0_SELPH_DQ2_DLY_OEN_DQ2)
                | P_Fld(0x6, SHURK0_SELPH_DQ2_DLY_OEN_DQ1)
                | P_Fld(0x6, SHURK0_SELPH_DQ2_DLY_OEN_DQ0)
                | P_Fld(0x1, SHURK0_SELPH_DQ2_DLY_DQ3)
                | P_Fld(0x1, SHURK0_SELPH_DQ2_DLY_DQ2)
                | P_Fld(0x1, SHURK0_SELPH_DQ2_DLY_DQ1)
                | P_Fld(0x1, SHURK0_SELPH_DQ2_DLY_DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ3, P_Fld(0x6, SHURK0_SELPH_DQ3_DLY_OEN_DQM3)
                | P_Fld(0x6, SHURK0_SELPH_DQ3_DLY_OEN_DQM2)
                | P_Fld(0x6, SHURK0_SELPH_DQ3_DLY_OEN_DQM1)
                | P_Fld(0x6, SHURK0_SELPH_DQ3_DLY_OEN_DQM0)
                | P_Fld(0x1, SHURK0_SELPH_DQ3_DLY_DQM3)
                | P_Fld(0x1, SHURK0_SELPH_DQ3_DLY_DQM2)
                | P_Fld(0x1, SHURK0_SELPH_DQ3_DLY_DQM1)
                | P_Fld(0x1, SHURK0_SELPH_DQ3_DLY_DQM0));
    //vIO32WriteFldAlign(DRAMC_REG_SHURK1_DQSCTL, 0x4, SHURK1_DQSCTL_R1DQSINCTL); //Set in UpdateACTimingReg()
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQSIEN, P_Fld(0x1f, SHURK1_DQSIEN_R1DQS1IEN)
                | P_Fld(0x1f, SHURK1_DQSIEN_R1DQS0IEN));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_PI, P_Fld(0x14, SHURK1_PI_RK1_ARPI_DQM_B1)
                | P_Fld(0x14, SHURK1_PI_RK1_ARPI_DQM_B0)
                | P_Fld(0x14, SHURK1_PI_RK1_ARPI_DQ_B1)
                | P_Fld(0x14, SHURK1_PI_RK1_ARPI_DQ_B0));
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3)
                    | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2)
                    | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1)
                    | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0)
                    | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ3)
                    | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ2)
                    | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ1)
                    | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3)
                    | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2)
                    | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1)
                    | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0)
                    | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM3)
                    | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM2)
                    | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM1)
                    | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM0));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3)
                    | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2)
                    | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1)
                    | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0)
                    | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ3)
                    | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ2)
                    | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ1)
                    | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3)
                    | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2)
                    | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1)
                    | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0)
                    | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM3)
                    | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM2)
                    | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM1)
                    | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM0));
    }
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ2, P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ3)
                | P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ2)
                | P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ1)
                | P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ0)
                | P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1DQ3)
                | P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1DQ2)
                | P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1DQ1)
                | P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1DQ0));
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ3, P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM3)
                | P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM2)
                | P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM1)
                | P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM0)
                | P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1DQM3)
                | P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1DQM2)
                | P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1DQM1)
                | P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1DQM0));
    vIO32WriteFldMulti(DRAMC_REG_SHU_DQSG_RETRY, P_Fld(0x4, SHU_DQSG_RETRY_R_DQSIENLAT)
                | P_Fld(0x0, SHU_DQSG_RETRY_R_DDR1866_PLUS)); // @YT, don't care

    ////DDRPHY0-SHU2
    vIO32WriteFldAlign(DDRPHY_SHU_B0_DQ5, 0x3, SHU_B0_DQ5_RG_RX_ARDQS0_DVS_DLY_B0);
    //vIO32WriteFldMulti(DDRPHY_SHU_B0_DQ6, P_Fld(0x0, SHU_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0)
    //            | P_Fld(0x1, SHU_B0_DQ6_RG_ARPI_MIDPI_EN_B0));
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DDRPHY_SHU_B0_DQ7, P_Fld(0x0, SHU_B0_DQ7_MIDPI_DIV4_ENABLE)
                    | P_Fld(0x1, SHU_B0_DQ7_MIDPI_ENABLE));
    }
    else
    {
        vIO32WriteFldMulti(DDRPHY_SHU_B0_DQ7, P_Fld(0x1, SHU_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0)
                    | P_Fld(0x1, SHU_B0_DQ7_R_DMDQMDBI_SHU_B0)
                    | P_Fld(0x0, SHU_B0_DQ7_MIDPI_DIV4_ENABLE)
                    | P_Fld(0x1, SHU_B0_DQ7_MIDPI_ENABLE));
    }
    vIO32WriteFldAlign(DDRPHY_SHU_B1_DQ5, 0x3, SHU_B1_DQ5_RG_RX_ARDQS0_DVS_DLY_B1);
    //vIO32WriteFldMulti(DDRPHY_SHU_B1_DQ6, P_Fld(0x0, SHU_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1)
    //            | P_Fld(0x1, SHU_B1_DQ6_RG_ARPI_MIDPI_EN_B1));
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
    }
    else
    {
        vIO32WriteFldMulti(DDRPHY_SHU_B1_DQ7, P_Fld(0x1, SHU_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1)
                    | P_Fld(0x1, SHU_B1_DQ7_R_DMDQMDBI_SHU_B1));
    }
    //DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    //vIO32WriteFldMulti(DDRPHY_SHU_CA_CMD6, P_Fld(0x0, SHU_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA)
    //            | P_Fld(0x1, SHU_CA_CMD6_RG_ARPI_MIDPI_EN_CA));
    //vIO32WriteFldMulti(DDRPHY_SHU_CA_CMD6+SHIFT_TO_CHB_ADDR, P_Fld(0x0, SHU_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA)
    //            | P_Fld(0x1, SHU_CA_CMD6_RG_ARPI_MIDPI_EN_CA));
    //DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    //vIO32WriteFldAlign(DDRPHY_SHU_PLL5, 0x3300, SHU_PLL5_RG_RPHYPLL_SDM_PCW);
    //vIO32WriteFldAlign(DDRPHY_SHU_PLL7, 0x3300, SHU_PLL7_RG_RCLRPLL_SDM_PCW);
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B0_DQ7, P_Fld(0x1e, SHU_R0_B0_DQ7_RK0_ARPI_DQM_B0)
                | P_Fld(0x1e, SHU_R0_B0_DQ7_RK0_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B1_DQ7, P_Fld(0x1e, SHU_R0_B1_DQ7_RK0_ARPI_DQM_B1)
                | P_Fld(0x1e, SHU_R0_B1_DQ7_RK0_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B0_DQ7, P_Fld(0x1e, SHU_R1_B0_DQ7_RK1_ARPI_DQM_B0)
                | P_Fld(0x1e, SHU_R1_B0_DQ7_RK1_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B1_DQ7, P_Fld(0x1e, SHU_R1_B1_DQ7_RK1_ARPI_DQM_B1)
                | P_Fld(0x1e, SHU_R1_B1_DQ7_RK1_ARPI_DQ_B1));
#if LEGACY_RX_DLY
    LegacyRxDly_LP4_DDR2667(p);
#endif
#if LEGACY_DELAY_CELL
    LegacyDlyCellInitLP4_DDR2667(p);
#endif

}

static void DramcSetting_LP4_TX_Delay_DDR1600(DRAMC_CTX_T *p, DRAM_RANK_T eRank)
{
    U8 ucR0TxdlyOendq = 0, ucR0Txdlydq = 0;
    U8 ucR1TxdlyOendq = 0, ucR1Txdlydq = 0;
    U8 u1R0B0Pi = 0, u1R0B1Pi = 0; 
    U8 u1R1B0Pi = 0, u1R1B1Pi = 0; 

    if (eRank == RANK_0)
    {
        if (vGet_Div_Mode(p) == DIV4_MODE)
        {
            ucR0TxdlyOendq = 0x3;
            ucR0Txdlydq = 0x4;
        }
        else
        {
            if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
            {
                ucR0TxdlyOendq = 0x1;
                ucR0Txdlydq = 0x2;
            }
            else
            {
                ucR0TxdlyOendq = 0x1;
                ucR0Txdlydq = 0x2;
            }
        }

        if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
        {
            vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3)
                        | P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2)
                        | P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1)
                        | P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0)
                        | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ0_TXDLY_DQ3)
                        | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ0_TXDLY_DQ2)
                        | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ0_TXDLY_DQ1)
                        | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ0_TXDLY_DQ0));
            vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3)
                        | P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2)
                        | P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1)
                        | P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0)
                        | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ1_TXDLY_DQM3)
                        | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ1_TXDLY_DQM2)
                        | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ1_TXDLY_DQM1)
                        | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ1_TXDLY_DQM0));
        }
        else
        {
            vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3)
                        | P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2)
                        | P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1)
                        | P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0)
                        | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ0_TXDLY_DQ3)
                        | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ0_TXDLY_DQ2)
                        | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ0_TXDLY_DQ1)
                        | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ0_TXDLY_DQ0));
            vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3)
                        | P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2)
                        | P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1)
                        | P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0)
                        | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ1_TXDLY_DQM3)
                        | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ1_TXDLY_DQM2)
                        | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ1_TXDLY_DQM1)
                        | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ1_TXDLY_DQM0));
        }
        if (vGet_Div_Mode(p) == DIV4_MODE)
        {
            ucR0TxdlyOendq = 0x2;
            ucR0Txdlydq = 0x1;
        }
        else
        {
            if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
            {
                ucR0TxdlyOendq = 0x6;
                ucR0Txdlydq = 0x1;
            }
            else
            {
                ucR0TxdlyOendq = 0x7;
                ucR0Txdlydq = 0x1;
            }
        }
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ2, P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ2_DLY_OEN_DQ3)
                    | P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ2_DLY_OEN_DQ2)
                    | P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ2_DLY_OEN_DQ1)
                    | P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ2_DLY_OEN_DQ0)
                    | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ2_DLY_DQ3)
                    | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ2_DLY_DQ2)
                    | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ2_DLY_DQ1)
                    | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ2_DLY_DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ3, P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ3_DLY_OEN_DQM3)
                    | P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ3_DLY_OEN_DQM2)
                    | P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ3_DLY_OEN_DQM1)
                    | P_Fld(ucR0TxdlyOendq, SHURK0_SELPH_DQ3_DLY_OEN_DQM0)
                    | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ3_DLY_DQM3)
                    | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ3_DLY_DQM2)
                    | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ3_DLY_DQM1)
                    | P_Fld(ucR0Txdlydq, SHURK0_SELPH_DQ3_DLY_DQM0));
    }
    else //RANK_1
    {
        if (vGet_Div_Mode(p) == DIV4_MODE)
        {
            ucR1TxdlyOendq = 0x3;
            ucR1Txdlydq = 0x4;
        }
        else
        {
            if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
            {
                ucR1TxdlyOendq = 0x1;
                ucR1Txdlydq = 0x2;
            }
            else
            {
                ucR1TxdlyOendq = 0x1;
                ucR1Txdlydq = 0x2;
            }
        }
        if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
        {
            vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3)
                        | P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2)
                        | P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1)
                        | P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0)
                        | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ0_TX_DLY_R1DQ3)
                        | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ0_TX_DLY_R1DQ2)
                        | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ0_TX_DLY_R1DQ1)
                        | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ0_TX_DLY_R1DQ0));
            vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3)
                        | P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2)
                        | P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1)
                        | P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0)
                        | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ1_TX_DLY_R1DQM3)
                        | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ1_TX_DLY_R1DQM2)
                        | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ1_TX_DLY_R1DQM1)
                        | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ1_TX_DLY_R1DQM0));
        }
        else
        {
            vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3)
                        | P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2)
                        | P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1)
                        | P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0)
                        | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ0_TX_DLY_R1DQ3)
                        | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ0_TX_DLY_R1DQ2)
                        | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ0_TX_DLY_R1DQ1)
                        | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ0_TX_DLY_R1DQ0));
            vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3)
                        | P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2)
                        | P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1)
                        | P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0)
                        | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ1_TX_DLY_R1DQM3)
                        | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ1_TX_DLY_R1DQM2)
                        | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ1_TX_DLY_R1DQM1)
                        | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ1_TX_DLY_R1DQM0));
        }
        if (vGet_Div_Mode(p) == DIV4_MODE)
        {
            ucR1TxdlyOendq = 0x3;
            ucR1Txdlydq = 0x2;
        }
        else
        {
            if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
            {
                ucR1TxdlyOendq = 0x7;
                ucR1Txdlydq = 0x2;
            }
            else
            {
                ucR1TxdlyOendq = 0x7;
                ucR1Txdlydq = 0x1;
            }
        }
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ2, P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ3)
                    | P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ2)
                    | P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ1)
                    | P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ0)
                    | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ2_DLY_R1DQ3)
                    | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ2_DLY_R1DQ2)
                    | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ2_DLY_R1DQ1)
                    | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ2_DLY_R1DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ3, P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM3)
                    | P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM2)
                    | P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM1)
                    | P_Fld(ucR1TxdlyOendq, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM0)
                    | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ3_DLY_R1DQM3)
                    | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ3_DLY_R1DQM2)
                    | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ3_DLY_R1DQM1)
                    | P_Fld(ucR1Txdlydq, SHURK1_SELPH_DQ3_DLY_R1DQM0));
    }

    if (eRank == RANK_0)
    {
        if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
        {
            // Byte mode For RK0 Tx PI
            u1R0B0Pi = 0x1e;
            u1R0B1Pi = 0x1e;
        }
        else
        {
            // For RK0 Tx PI
            u1R0B0Pi = 0x1a;
            u1R0B1Pi = 0x1a;
        }
        vIO32WriteFldMulti(DDRPHY_SHU_R0_B0_DQ7, P_Fld(u1R0B0Pi, SHU_R0_B0_DQ7_RK0_ARPI_DQM_B0)
                    | P_Fld(u1R0B0Pi, SHU_R0_B0_DQ7_RK0_ARPI_DQ_B0));
        vIO32WriteFldMulti(DDRPHY_SHU_R0_B1_DQ7, P_Fld(u1R0B1Pi, SHU_R0_B1_DQ7_RK0_ARPI_DQM_B1)
                    | P_Fld(u1R0B1Pi, SHU_R0_B1_DQ7_RK0_ARPI_DQ_B1));
    }
    else
    {
        // For RK1 Tx PI
        if (vGet_Div_Mode(p) == DIV4_MODE)
        {
            // for DDR1600 1:4 mode
            u1R1B0Pi = 0x1b;
            u1R1B1Pi = 0x1b;
        }
        else
        {
            if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
            {
                // Byte mode for DDR1600 1:8 mode
                u1R1B0Pi = 0x13;
                u1R1B1Pi = 0x12;
            }
            else
            {
                // for DDR1600 1:8 mode
                u1R1B0Pi = 0x26;
                u1R1B1Pi = 0x26;
            }
        }
        vIO32WriteFldMulti(DDRPHY_SHU_R1_B0_DQ7, P_Fld(u1R1B0Pi, SHU_R1_B0_DQ7_RK1_ARPI_DQM_B0)
                    | P_Fld(u1R1B0Pi, SHU_R1_B0_DQ7_RK1_ARPI_DQ_B0));
        vIO32WriteFldMulti(DDRPHY_SHU_R1_B1_DQ7, P_Fld(u1R1B1Pi, SHU_R1_B1_DQ7_RK1_ARPI_DQM_B1)
                    | P_Fld(u1R1B1Pi, SHU_R1_B1_DQ7_RK1_ARPI_DQ_B1));

    }
}

static void DramcSetting_LP4_TX_Delay_DDR800(DRAMC_CTX_T *p, DRAM_RANK_T eRank)
{
    if (eRank == RANK_0)
    {
        if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
        {
            vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3)
                        | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2)
                        | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1)
                        | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0)
                        | P_Fld(0x4, SHURK0_SELPH_DQ0_TXDLY_DQ3)
                        | P_Fld(0x4, SHURK0_SELPH_DQ0_TXDLY_DQ2)
                        | P_Fld(0x4, SHURK0_SELPH_DQ0_TXDLY_DQ1)
                        | P_Fld(0x4, SHURK0_SELPH_DQ0_TXDLY_DQ0));
            vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3)
                        | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2)
                        | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1)
                        | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0)
                        | P_Fld(0x4, SHURK0_SELPH_DQ1_TXDLY_DQM3)
                        | P_Fld(0x4, SHURK0_SELPH_DQ1_TXDLY_DQM2)
                        | P_Fld(0x4, SHURK0_SELPH_DQ1_TXDLY_DQM1)
                        | P_Fld(0x4, SHURK0_SELPH_DQ1_TXDLY_DQM0));
        }
        else
        {
            vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3)
                        | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2)
                        | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1)
                        | P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0)
                        | P_Fld(0x4, SHURK0_SELPH_DQ0_TXDLY_DQ3)
                        | P_Fld(0x4, SHURK0_SELPH_DQ0_TXDLY_DQ2)
                        | P_Fld(0x4, SHURK0_SELPH_DQ0_TXDLY_DQ1)
                        | P_Fld(0x4, SHURK0_SELPH_DQ0_TXDLY_DQ0));
            vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3)
                        | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2)
                        | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1)
                        | P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0)
                        | P_Fld(0x4, SHURK0_SELPH_DQ1_TXDLY_DQM3)
                        | P_Fld(0x4, SHURK0_SELPH_DQ1_TXDLY_DQM2)
                        | P_Fld(0x4, SHURK0_SELPH_DQ1_TXDLY_DQM1)
                        | P_Fld(0x4, SHURK0_SELPH_DQ1_TXDLY_DQM0));
        }

        if (vGet_DDR800_Mode(p) == DDR800_CLOSE_LOOP)
        {
            // TX OE -3*UI
            vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ2, P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_OEN_DQ3)
                        | P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_OEN_DQ2)
                        | P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_OEN_DQ1)
                        | P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_OEN_DQ0)
                        | P_Fld(0x1, SHURK0_SELPH_DQ2_DLY_DQ3)
                        | P_Fld(0x1, SHURK0_SELPH_DQ2_DLY_DQ2)
                        | P_Fld(0x1, SHURK0_SELPH_DQ2_DLY_DQ1)
                        | P_Fld(0x1, SHURK0_SELPH_DQ2_DLY_DQ0));
            vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ3, P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_OEN_DQM3)
                        | P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_OEN_DQM2)
                        | P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_OEN_DQM1)
                        | P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_OEN_DQM0)
                        | P_Fld(0x1, SHURK0_SELPH_DQ3_DLY_DQM3)
                        | P_Fld(0x1, SHURK0_SELPH_DQ3_DLY_DQM2)
                        | P_Fld(0x1, SHURK0_SELPH_DQ3_DLY_DQM1)
                        | P_Fld(0x1, SHURK0_SELPH_DQ3_DLY_DQM0));
        }
        else //if (vGet_DDR800_Mode(p) == DDR800_OPEN_LOOP)
        {
            // DDR800_OPEN_LOOP and DDR800_SEMI_LOOP
            vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ2, P_Fld(0x3, SHURK0_SELPH_DQ2_DLY_OEN_DQ3)
                        | P_Fld(0x3, SHURK0_SELPH_DQ2_DLY_OEN_DQ2)
                        | P_Fld(0x3, SHURK0_SELPH_DQ2_DLY_OEN_DQ1)
                        | P_Fld(0x3, SHURK0_SELPH_DQ2_DLY_OEN_DQ0)
                        | P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_DQ3)
                        | P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_DQ2)
                        | P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_DQ1)
                        | P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_DQ0));
            vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ3, P_Fld(0x3, SHURK0_SELPH_DQ3_DLY_OEN_DQM3)
                        | P_Fld(0x3, SHURK0_SELPH_DQ3_DLY_OEN_DQM2)
                        | P_Fld(0x3, SHURK0_SELPH_DQ3_DLY_OEN_DQM1)
                        | P_Fld(0x3, SHURK0_SELPH_DQ3_DLY_OEN_DQM0)
                        | P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_DQM3)
                        | P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_DQM2)
                        | P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_DQM1)
                        | P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_DQM0));
        }
    }
    else
    {
        if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
        {
            vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3)
                        | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2)
                        | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1)
                        | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0)
                        | P_Fld(0x4, SHURK1_SELPH_DQ0_TX_DLY_R1DQ3)
                        | P_Fld(0x4, SHURK1_SELPH_DQ0_TX_DLY_R1DQ2)
                        | P_Fld(0x4, SHURK1_SELPH_DQ0_TX_DLY_R1DQ1)
                        | P_Fld(0x4, SHURK1_SELPH_DQ0_TX_DLY_R1DQ0));
            vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3)
                        | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2)
                        | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1)
                        | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0)
                        | P_Fld(0x4, SHURK1_SELPH_DQ1_TX_DLY_R1DQM3)
                        | P_Fld(0x4, SHURK1_SELPH_DQ1_TX_DLY_R1DQM2)
                        | P_Fld(0x4, SHURK1_SELPH_DQ1_TX_DLY_R1DQM1)
                        | P_Fld(0x4, SHURK1_SELPH_DQ1_TX_DLY_R1DQM0));
        }
        else
        {
            vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3)
                        | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2)
                        | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1)
                        | P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0)
                        | P_Fld(0x4, SHURK1_SELPH_DQ0_TX_DLY_R1DQ3)
                        | P_Fld(0x4, SHURK1_SELPH_DQ0_TX_DLY_R1DQ2)
                        | P_Fld(0x4, SHURK1_SELPH_DQ0_TX_DLY_R1DQ1)
                        | P_Fld(0x4, SHURK1_SELPH_DQ0_TX_DLY_R1DQ0));
            vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3)
                        | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2)
                        | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1)
                        | P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0)
                        | P_Fld(0x4, SHURK1_SELPH_DQ1_TX_DLY_R1DQM3)
                        | P_Fld(0x4, SHURK1_SELPH_DQ1_TX_DLY_R1DQM2)
                        | P_Fld(0x4, SHURK1_SELPH_DQ1_TX_DLY_R1DQM1)
                        | P_Fld(0x4, SHURK1_SELPH_DQ1_TX_DLY_R1DQM0));
        }

        if (vGet_DDR800_Mode(p) == DDR800_CLOSE_LOOP)
        {
            // TX OE -3*UI
            vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ2, P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ3)
                        | P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ2)
                        | P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ1)
                        | P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ0)
                        | P_Fld(0x1, SHURK1_SELPH_DQ2_DLY_R1DQ3)
                        | P_Fld(0x1, SHURK1_SELPH_DQ2_DLY_R1DQ2)
                        | P_Fld(0x1, SHURK1_SELPH_DQ2_DLY_R1DQ1)
                        | P_Fld(0x1, SHURK1_SELPH_DQ2_DLY_R1DQ0));
            vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ3, P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM3)
                        | P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM2)
                        | P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM1)
                        | P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM0)
                        | P_Fld(0x1, SHURK1_SELPH_DQ3_DLY_R1DQM3)
                        | P_Fld(0x1, SHURK1_SELPH_DQ3_DLY_R1DQM2)
                        | P_Fld(0x1, SHURK1_SELPH_DQ3_DLY_R1DQM1)
                        | P_Fld(0x1, SHURK1_SELPH_DQ3_DLY_R1DQM0));
        }
        else //if (vGet_DDR800_Mode(p) == DDR800_OPEN_LOOP)
        {       
            // DDR800_OPEN_LOOP and DDR800_SEMI_LOOP
            vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ2, P_Fld(0x3, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ3)
                        | P_Fld(0x3, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ2)
                        | P_Fld(0x3, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ1)
                        | P_Fld(0x3, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ0)
                        | P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1DQ3)
                        | P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1DQ2)
                        | P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1DQ1)
                        | P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1DQ0));
            vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ3, P_Fld(0x3, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM3)
                        | P_Fld(0x3, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM2)
                        | P_Fld(0x3, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM1)
                        | P_Fld(0x3, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM0)
                        | P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1DQM3)
                        | P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1DQM2)
                        | P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1DQM1)
                        | P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1DQM0));
        }
    }

    if (vGet_DDR800_Mode(p) == DDR800_CLOSE_LOOP)
    {
        // DDR800 1:4 mode TX PI
        vIO32WriteFldMulti(DDRPHY_SHU_R0_B0_DQ7, P_Fld(0x1a, SHU_R0_B0_DQ7_RK0_ARPI_DQM_B0)
                    | P_Fld(0x1a, SHU_R0_B0_DQ7_RK0_ARPI_DQ_B0));
        vIO32WriteFldMulti(DDRPHY_SHU_R0_B1_DQ7, P_Fld(0x1a, SHU_R0_B1_DQ7_RK0_ARPI_DQM_B1)
                    | P_Fld(0x1a, SHU_R0_B1_DQ7_RK0_ARPI_DQ_B1));
        vIO32WriteFldMulti(DDRPHY_SHU_R1_B0_DQ7, P_Fld(0x25, SHU_R1_B0_DQ7_RK1_ARPI_DQM_B0)
                    | P_Fld(0x25, SHU_R1_B0_DQ7_RK1_ARPI_DQ_B0));
        vIO32WriteFldMulti(DDRPHY_SHU_R1_B1_DQ7, P_Fld(0x25, SHU_R1_B1_DQ7_RK1_ARPI_DQM_B1)
                    | P_Fld(0x25, SHU_R1_B1_DQ7_RK1_ARPI_DQ_B1));
    }
    else
    {
        // DDR800_SEMI_LOOP and DDR800_OPEN_LOOP
        // DDR800 1:4 mode TX PI
        vIO32WriteFldMulti(DDRPHY_SHU_R0_B0_DQ7, P_Fld(0x0, SHU_R0_B0_DQ7_RK0_ARPI_DQM_B0)
                    | P_Fld(0x0, SHU_R0_B0_DQ7_RK0_ARPI_DQ_B0));
        vIO32WriteFldMulti(DDRPHY_SHU_R0_B1_DQ7, P_Fld(0x0, SHU_R0_B1_DQ7_RK0_ARPI_DQM_B1)
                    | P_Fld(0x0, SHU_R0_B1_DQ7_RK0_ARPI_DQ_B1));
        vIO32WriteFldMulti(DDRPHY_SHU_R1_B0_DQ7, P_Fld(0x0, SHU_R1_B0_DQ7_RK1_ARPI_DQM_B0)
                    | P_Fld(0x0, SHU_R1_B0_DQ7_RK1_ARPI_DQ_B0));
        vIO32WriteFldMulti(DDRPHY_SHU_R1_B1_DQ7, P_Fld(0x0, SHU_R1_B1_DQ7_RK1_ARPI_DQM_B1)
                    | P_Fld(0x0, SHU_R1_B1_DQ7_RK1_ARPI_DQ_B1));
    }
}

static void DramcSetting_Olympus_LP4_ByteMode_DDR1600(DRAMC_CTX_T *p)
{
    U8 ucMCKTxdlyOendqs = 0, ucMCKTxdlydqs = 0;
    U8 ucUITxdlyOendqs = 0, ucUITxdlydqs = 0;

    ////DRAMC0-SHU3
#if LEGACY_TDQSCK_PRECAL
    LegacyPreCalLP4_DDR1600(p);
#endif
#if LEGACY_TX_TRACK
     LegacyTxTrackLP4_DDR1600(p);
#endif

    //vIO32WriteFldAlign(DRAMC_REG_SHU_CONF2, 0x32, SHU_CONF2_FSPCHG_PRDCNT); //ACTiming related -> set in UpdateACTiming_Reg()
    //vIO32WriteFldMulti(DRAMC_REG_SHU_CKECTRL, P_Fld(0x0, SHU_CKECTRL_TCKESRX)); //from CF review, only work for DDR3/4 (Darren)
                //| P_Fld(0x2, SHU_CKECTRL_CKEPRD));
    vIO32WriteFldMulti(DRAMC_REG_SHU_ODTCTRL, P_Fld(0x0, SHU_ODTCTRL_RODTE)
                | P_Fld(0x0, SHU_ODTCTRL_RODTE2)
                //| P_Fld(0x2, SHU_ODTCTRL_RODT) //Set in UpdateACTimingReg()
#ifdef LOOPBACK_TEST
                | P_Fld(0x1, SHU_ODTCTRL_ROEN));
#else
                | P_Fld(0x0, SHU_ODTCTRL_ROEN));
#endif
    vIO32WriteFldAlign(DDRPHY_SHU_B0_DQ7, 0x0, SHU_B0_DQ7_R_DMRODTEN_B0);
    vIO32WriteFldAlign(DDRPHY_SHU_B1_DQ7, 0x0, SHU_B1_DQ7_R_DMRODTEN_B1);

    if (vGet_Div_Mode(p) == DIV4_MODE)
    {
        ucMCKTxdlyOendqs = 0x3;
        ucMCKTxdlydqs = 0x4;
        ucUITxdlyOendqs = 0x2;
        ucUITxdlydqs = 0x1;
    }
    else
    {
        ucMCKTxdlyOendqs = 0x1;
        ucMCKTxdlydqs = 0x2;
        ucUITxdlyOendqs = 0x6;
        ucUITxdlydqs = 0x1;
    }

    //MCK by Wleveling
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS0, P_Fld(ucMCKTxdlyOendqs, SHU_SELPH_DQS0_TXDLY_OEN_DQS3)
                | P_Fld(ucMCKTxdlyOendqs, SHU_SELPH_DQS0_TXDLY_OEN_DQS2)
                | P_Fld(ucMCKTxdlyOendqs, SHU_SELPH_DQS0_TXDLY_OEN_DQS1)
                | P_Fld(ucMCKTxdlyOendqs, SHU_SELPH_DQS0_TXDLY_OEN_DQS0)
                | P_Fld(ucMCKTxdlydqs, SHU_SELPH_DQS0_TXDLY_DQS3)
                | P_Fld(ucMCKTxdlydqs, SHU_SELPH_DQS0_TXDLY_DQS2)
                | P_Fld(ucMCKTxdlydqs, SHU_SELPH_DQS0_TXDLY_DQS1)
                | P_Fld(ucMCKTxdlydqs, SHU_SELPH_DQS0_TXDLY_DQS0));
    //UI by Wleveling
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS1, P_Fld(ucUITxdlyOendqs, SHU_SELPH_DQS1_DLY_OEN_DQS3)
                | P_Fld(ucUITxdlyOendqs, SHU_SELPH_DQS1_DLY_OEN_DQS2)
                | P_Fld(ucUITxdlyOendqs, SHU_SELPH_DQS1_DLY_OEN_DQS1)
                | P_Fld(ucUITxdlyOendqs, SHU_SELPH_DQS1_DLY_OEN_DQS0)
                | P_Fld(ucUITxdlydqs, SHU_SELPH_DQS1_DLY_DQS3)
                | P_Fld(ucUITxdlydqs, SHU_SELPH_DQS1_DLY_DQS2)
                | P_Fld(ucUITxdlydqs, SHU_SELPH_DQS1_DLY_DQS1)
                | P_Fld(ucUITxdlydqs, SHU_SELPH_DQS1_DLY_DQS0));

    vIO32WriteFldAlign(DRAMC_REG_SHU_DQS2DQ_TX, 0x4, SHU_DQS2DQ_TX_OE2DQ_OFFSET);

    vIO32WriteFldMulti(DRAMC_REG_SHURK0_PI, P_Fld(0x1a, SHURK0_PI_RK0_ARPI_DQM_B1)
                | P_Fld(0x1a, SHURK0_PI_RK0_ARPI_DQM_B0)
                | P_Fld(0x1a, SHURK0_PI_RK0_ARPI_DQ_B1)
                | P_Fld(0x1a, SHURK0_PI_RK0_ARPI_DQ_B0));
    //Rank0/1 TX Delay
    if(p->freqGroup == 400)
    {
        DramcSetting_LP4_TX_Delay_DDR800(p, RANK_0);
        DramcSetting_LP4_TX_Delay_DDR800(p, RANK_1);
    }
    else // for DDR1200 and DDR1600
    {
        DramcSetting_LP4_TX_Delay_DDR1600(p, RANK_0);
        DramcSetting_LP4_TX_Delay_DDR1600(p, RANK_1);
    }
    // Tx tracking
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_PI, P_Fld(0x1e, SHURK1_PI_RK1_ARPI_DQM_B1)
                | P_Fld(0x1e, SHURK1_PI_RK1_ARPI_DQM_B0)
                | P_Fld(0x1e, SHURK1_PI_RK1_ARPI_DQ_B1)
                | P_Fld(0x1e, SHURK1_PI_RK1_ARPI_DQ_B0));

#if LEGACY_GATING_DLY
    if(p->freqGroup == 400)
        LegacyGatingDlyLP4_DDR800(p); // for 1:4 mode settings
    else
        LegacyGatingDlyLP4_DDR1600(p); // for 1:8 and 1:4 mode settings for DDR1200 and DDR1600
#endif

    vIO32WriteFldMulti(DRAMC_REG_SHU_DQSG_RETRY, P_Fld(0x3, SHU_DQSG_RETRY_R_DQSIENLAT)
                | P_Fld(0x0, SHU_DQSG_RETRY_R_DDR1866_PLUS));

    ////DDRPHY0-SHU3
    vIO32WriteFldAlign(DDRPHY_SHU_B0_DQ5, 0x5, SHU_B0_DQ5_RG_RX_ARDQS0_DVS_DLY_B0); // for Rx guard
    //vIO32WriteFldMulti(DDRPHY_SHU_B0_DQ6, P_Fld(0x1, SHU_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0)
    //            | P_Fld(0x0, SHU_B0_DQ6_RG_ARPI_MIDPI_EN_B0));
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DDRPHY_SHU_B0_DQ7, P_Fld(0x1, SHU_B0_DQ7_MIDPI_DIV4_ENABLE)
                    | P_Fld(0x0, SHU_B0_DQ7_MIDPI_ENABLE));
    }
    else
    {
        vIO32WriteFldMulti(DDRPHY_SHU_B0_DQ7, P_Fld(0x0, SHU_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0)
                    | P_Fld(0x0, SHU_B0_DQ7_R_DMDQMDBI_SHU_B0)
                    | P_Fld(0x1, SHU_B0_DQ7_MIDPI_DIV4_ENABLE)
                    | P_Fld(0x0, SHU_B0_DQ7_MIDPI_ENABLE));
    }
    vIO32WriteFldAlign(DDRPHY_SHU_B1_DQ5, 0x5, SHU_B1_DQ5_RG_RX_ARDQS0_DVS_DLY_B1); // for Rx guard
    //vIO32WriteFldMulti(DDRPHY_SHU_B1_DQ6, P_Fld(0x1, SHU_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1)
    //            | P_Fld(0x0, SHU_B1_DQ6_RG_ARPI_MIDPI_EN_B1));
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {}
    else
    {
        vIO32WriteFldMulti(DDRPHY_SHU_B1_DQ7, P_Fld(0x0, SHU_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1)
                    | P_Fld(0x0, SHU_B1_DQ7_R_DMDQMDBI_SHU_B1));
    }

#if LEGACY_RX_DLY
    if (p->freqGroup == 400) // for DDR800 1:4 mode
        LegacyRxDly_LP4_DDR800(p);
    else
        LegacyRxDly_LP4_DDR1600(p);
#endif
#if LEGACY_DELAY_CELL
    LegacyDlyCellInitLP4_DDR1600(p);
#endif

    if (vGet_Div_Mode(p) == DIV4_MODE)
    {
#if 0
        if (p->freqGroup == 400) // for DDR800 1:4 mode
        {
            vIO32WriteFldMulti(DRAMC_REG_SHU_RANKCTL, P_Fld(0x6, SHU_RANKCTL_RANKINCTL_PHY)
                        | P_Fld(0x4, SHU_RANKCTL_RANKINCTL_ROOT1)
                        | P_Fld(0x4, SHU_RANKCTL_RANKINCTL));
        }
        else // for DDR1600 1:4 mode
        {
            vIO32WriteFldMulti(DRAMC_REG_SHU_RANKCTL, P_Fld(0x6, SHU_RANKCTL_RANKINCTL_PHY)
                        | P_Fld(0x4, SHU_RANKCTL_RANKINCTL_ROOT1)
                        | P_Fld(0x4, SHU_RANKCTL_RANKINCTL));
        }
#endif
        // 1:4 mode
        vIO32WriteFldMulti(DRAMC_REG_SHU_CONF0, P_Fld(0x0, SHU_CONF0_DM64BITEN)
                    | P_Fld(0x1, SHU_CONF0_FDIV2)
                    | P_Fld(0x0, SHU_CONF0_FREQDIV4)
                    | P_Fld(0x0, SHU_CONF0_DUALSCHEN)
                    | P_Fld(0x1, SHU_CONF0_WDATRGO));
#if 0 // 1:8 mode
        vIO32WriteFldMulti(DRAMC_REG_SHU_CONF0, P_Fld(0x1, SHU_CONF0_DM64BITEN)
                    | P_Fld(0x0, SHU_CONF0_FDIV2)
                    | P_Fld(0x1, SHU_CONF0_FREQDIV4)
                    | P_Fld(0x1, SHU_CONF0_DUALSCHEN)
                    | P_Fld(0x0, SHU_CONF0_WDATRGO));
#endif

    #if (!CMD_CKE_WORKAROUND_FIX)
        vIO32WriteFldAlign(DRAMC_REG_SHU_APHY_TX_PICG_CTRL, 0x7, SHU_APHY_TX_PICG_CTRL_DDRPHY_CLK_DYN_GATING_SEL);
        vIO32WriteFldAlign(DRAMC_REG_SHU_SELPH_CA1, 0x1, SHU_SELPH_CA1_TXDLY_CKE);
        vIO32WriteFldAlign(DRAMC_REG_SHU_SELPH_CA2, 0x1, SHU_SELPH_CA2_TXDLY_CKE1);
    #endif
        //DATLAT 1:4 mode from Ton
        vIO32WriteFldMulti(DDRPHY_SHU_B0_DQ7, P_Fld(0x1, SHU_B0_DQ7_R_DMRDSEL_DIV2_OPT)
                    | P_Fld(0x1, SHU_B0_DQ7_R_DMRDSEL_LOBYTE_OPT)
                    | P_Fld(0x0, SHU_B0_DQ7_R_DMRDSEL_HIBYTE_OPT));
        //RDATA PIPE from Ton
        vIO32WriteFldMulti(DRAMC_REG_SHU_PIPE, P_Fld(0x1, SHU_PIPE_READ_START_EXTEND1)
                    | P_Fld(0x1, SHU_PIPE_DLE_LAST_EXTEND1)
                    | P_Fld(0x1, SHU_PIPE_READ_START_EXTEND2)
                    | P_Fld(0x1, SHU_PIPE_DLE_LAST_EXTEND2)
                    | P_Fld(0x0, SHU_PIPE_READ_START_EXTEND3)
                    | P_Fld(0x0, SHU_PIPE_DLE_LAST_EXTEND3));
    }
#if 0
    else
    {
        // for DDR1600 1:8 mode
        vIO32WriteFldMulti(DRAMC_REG_SHU_RANKCTL, P_Fld(0x2, SHU_RANKCTL_RANKINCTL_PHY)
                    | P_Fld(0x0, SHU_RANKCTL_RANKINCTL_ROOT1)
                    | P_Fld(0x0, SHU_RANKCTL_RANKINCTL));
    }
#endif
}

/*
CANNOT use DRAMC_WBR :
DDRPHY_CA_DLL_ARPI0 -> DDRPHY_SHU_CA_DLL1
DDRPHY_CA_DLL_ARPI3
DDRPHY_CA_DLL_ARPI5 -> DDRPHY_SHU_CA_DLL0
DDRPHY_SHU_CA_CMD6
*/

#if ENABLE_DUTY_SCAN_V2
#ifdef DDR_INIT_TIME_PROFILING
U32 gu4DutyCalibrationTime;
#endif
#endif

void DramcSetting_Olympus_LP4_ByteMode(DRAMC_CTX_T *p)
{

    U8 u1CAP_SEL;
    U8 u1MIDPICAP_SEL;
    //U16 u2SDM_PCW = 0; // SDM_PCW are set in DDRPhyPLLSetting()
    U8 u1TXDLY_CMD;
    U8 u1TAIL_LAT;
    U8 u1RxInGateEn=0;

    AutoRefreshCKEOff(p);

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
                                                                             //before switch clock from 26M to PHY, need to init PHY clock first
    vIO32WriteFldMulti_All(DDRPHY_CKMUX_SEL, P_Fld(0x1, CKMUX_SEL_R_PHYCTRLMUX)  //move CKMUX_SEL_R_PHYCTRLMUX to here (it was originally between MISC_CG_CTRL0_CLK_MEM_SEL and MISC_CTRL0_R_DMRDSEL_DIV2_OPT)
                                        | P_Fld(0x1, CKMUX_SEL_R_PHYCTRLDCM)); // PHYCTRLDCM 1: follow DDRPHY_conf DCM settings, 0: follow infra DCM settings
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);

    //chg_mem_en = 1
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 0x1, MISC_CG_CTRL0_W_CHG_MEM);
    //26M
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 0x0, MISC_CG_CTRL0_CLK_MEM_SEL); 

    vIO32WriteFldMulti(DDRPHY_SHU_B0_DQ7, P_Fld(0x0, SHU_B0_DQ7_R_DMRDSEL_DIV2_OPT)
                | P_Fld(0x0, SHU_B0_DQ7_R_DMRDSEL_LOBYTE_OPT)
                | P_Fld(0x0, SHU_B0_DQ7_R_DMRDSEL_HIBYTE_OPT));

    //                   0 ===LP4_3200_intial_setting_shu1 begin===
    #if APOLLO_SPECIFIC
    vIO32WriteFldAlign(DRAMC_REG_RKCFG, 1, RKCFG_CS1FORCE0);
    #endif
    //Francis : pin mux issue, need to set CHD
    // TODO: ARDMSUS_10 already set to 0 in SwimpedanceCal(), may be removed here?
    vIO32WriteFldMulti(DDRPHY_MISC_SPM_CTRL1, P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10) | P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_B0)
                                           | P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_B1) | P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_CA));
    
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL2, 0x0, MISC_SPM_CTRL2_PHY_SPM_CTL2);
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL0, 0x0, MISC_SPM_CTRL0_PHY_SPM_CTL0);
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL2, 0x6003bf, MISC_CG_CTRL2_RG_MEM_DCM_CTL);//Move to DCM off setting
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL4, 0x333f3f00, MISC_CG_CTRL4_R_PHY_MCK_CG_CTRL);//Move to DCM off setting
    vIO32WriteFldMulti(DDRPHY_SHU_PLL1, P_Fld(0x1, SHU_PLL1_R_SHU_AUTO_PLL_MUX)
                | P_Fld(0x7, SHU_PLL1_SHU1_PLL1_RFU));
    vIO32WriteFldMulti(DDRPHY_SHU_B0_DQ7, P_Fld(0x1, SHU_B0_DQ7_MIDPI_ENABLE)
                | P_Fld(0x0, SHU_B0_DQ7_MIDPI_DIV4_ENABLE)
                | P_Fld(0, SHU_B0_DQ7_R_DMRANKRXDVS_B0));
    vIO32WriteFldAlign(DDRPHY_SHU_B1_DQ7, 0, SHU_B1_DQ7_R_DMRANKRXDVS_B1);
    //vIO32WriteFldAlign(DDRPHY_SHU_PLL4, 0xfe, SHU_PLL4_RG_RPHYPLL_RESERVED);
    vIO32WriteFldMulti(DDRPHY_SHU_PLL4, P_Fld(0x1, SHU_PLL4_RG_RPHYPLL_IBIAS) | P_Fld(0x1, SHU_PLL4_RG_RPHYPLL_ICHP) | P_Fld(0x1, SHU_PLL4_RG_RPHYPLL_FS));
    //vIO32WriteFldAlign(DDRPHY_SHU_PLL6, 0xfe, SHU_PLL6_RG_RCLRPLL_RESERVED);
    vIO32WriteFldMulti(DDRPHY_SHU_PLL6, P_Fld(0x1, SHU_PLL6_RG_RCLRPLL_IBIAS) | P_Fld(0x1, SHU_PLL6_RG_RCLRPLL_ICHP) | P_Fld(0x1, SHU_PLL6_RG_RCLRPLL_FS));
    vIO32WriteFldAlign(DDRPHY_SHU_PLL14, 0x0, SHU_PLL14_RG_RPHYPLL_SDM_SSC_PH_INIT);
    vIO32WriteFldAlign(DDRPHY_SHU_PLL20, 0x0, SHU_PLL20_RG_RCLRPLL_SDM_SSC_PH_INIT);
    vIO32WriteFldMulti(DDRPHY_CA_CMD2, P_Fld(0x0, CA_CMD2_RG_TX_ARCMD_OE_DIS)
                | P_Fld(0x0, CA_CMD2_RG_TX_ARCMD_ODTEN_DIS)
                | P_Fld(0x0, CA_CMD2_RG_TX_ARCLK_OE_DIS)
                | P_Fld(0x0, CA_CMD2_RG_TX_ARCLK_ODTEN_DIS));
    vIO32WriteFldMulti(DDRPHY_B0_DQ2, P_Fld(0x0, B0_DQ2_RG_TX_ARDQ_OE_DIS_B0)
                | P_Fld(0x0, B0_DQ2_RG_TX_ARDQ_ODTEN_DIS_B0)
                | P_Fld(0x0, B0_DQ2_RG_TX_ARDQS0_OE_DIS_B0)
                | P_Fld(0x0, B0_DQ2_RG_TX_ARDQS0_ODTEN_DIS_B0));
    vIO32WriteFldMulti(DDRPHY_B1_DQ2, P_Fld(0x0, B1_DQ2_RG_TX_ARDQ_OE_DIS_B1)
                | P_Fld(0x0, B1_DQ2_RG_TX_ARDQ_ODTEN_DIS_B1)
                | P_Fld(0x0, B1_DQ2_RG_TX_ARDQS0_OE_DIS_B1)
                | P_Fld(0x0, B1_DQ2_RG_TX_ARDQS0_ODTEN_DIS_B1));
    #if 0 //Correct settings are set in UpdateInitialSettings_LP4() 
    vIO32WriteFldAlign(DDRPHY_B0_DQ9, 0x0, B0_DQ9_R_IN_GATE_EN_LOW_OPT_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ9, 0x7, B1_DQ9_R_IN_GATE_EN_LOW_OPT_B1);
    vIO32WriteFldAlign(DDRPHY_CA_CMD10, 0x0, CA_CMD10_R_IN_GATE_EN_LOW_OPT_CA);
    #endif
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
    vIO32WriteFldAlign(DDRPHY_SHU_B0_DQ5, 0x3, SHU_B0_DQ5_RG_RX_ARDQS0_DVS_DLY_B0);
    vIO32WriteFldAlign(DDRPHY_SHU_B1_DQ5, 0x3, SHU_B1_DQ5_RG_RX_ARDQS0_DVS_DLY_B1);
#if LEGACY_RX_DLY
    LegacyRxDly_LP4_DDR3200(p);
#endif
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
#if LEGACY_DELAY_CELL
    LegacyDlyCellInitLP4_DDR3200(p);
#endif
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B0_DQ7, P_Fld(0x1f, SHU_R0_B0_DQ7_RK0_ARPI_DQM_B0)
                | P_Fld(0x1f, SHU_R0_B0_DQ7_RK0_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B1_DQ7, P_Fld(0x1f, SHU_R0_B1_DQ7_RK0_ARPI_DQM_B1)
                | P_Fld(0x1f, SHU_R0_B1_DQ7_RK0_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B0_DQ7, P_Fld(0x1f, SHU_R1_B0_DQ7_RK1_ARPI_DQM_B0)
                | P_Fld(0x1f, SHU_R1_B0_DQ7_RK1_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B1_DQ7, P_Fld(0x1f, SHU_R1_B1_DQ7_RK1_ARPI_DQM_B1)
                | P_Fld(0x1f, SHU_R1_B1_DQ7_RK1_ARPI_DQ_B1));
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
                | P_Fld(0x1, B0_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B0));
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
                | P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B1));
    vIO32WriteFldMulti(DDRPHY_B1_DQ5, P_Fld(0x1, B1_DQ5_RG_RX_ARDQ_EYE_STBEN_RESETB_B1)
                | P_Fld(0x0, B1_DQ5_B1_DQ5_RFU));
    vIO32WriteFldMulti(DDRPHY_CA_CMD3, P_Fld(0x1, CA_CMD3_RG_RX_ARCMD_IN_BUFF_EN)
                | P_Fld(0x1, CA_CMD3_RG_ARCMD_RESETB)
                | P_Fld(0x1, CA_CMD3_RG_TX_ARCMD_EN));
    vIO32WriteFldMulti(DDRPHY_CA_CMD6, P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_DDR4_SEL)
                | P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_BIAS_VREF_SEL)
                | P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_RES_BIAS_EN));
    /* ARCMD_DRVP, DRVN , ARCLK_DRVP, DRVN not used anymore
    vIO32WriteFldMulti(DDRPHY_SHU_CA_CMD1, P_Fld(0x1, SHU_CA_CMD1_RG_TX_ARCMD_DRVN)
                | P_Fld(0x1, SHU_CA_CMD1_RG_TX_ARCMD_DRVP));
    vIO32WriteFldMulti(DDRPHY_SHU_CA_CMD2, P_Fld(0x1, SHU_CA_CMD2_RG_TX_ARCLK_DRVN)
                | P_Fld(0x1, SHU_CA_CMD2_RG_TX_ARCLK_DRVP));
     */
    //vIO32WriteFldMulti(DDRPHY_SHU2_CA_CMD1, P_Fld(0x1, SHU2_CA_CMD1_RG_TX_ARCMD_DRVN)
    //            | P_Fld(0x1, SHU2_CA_CMD1_RG_TX_ARCMD_DRVP));
    //vIO32WriteFldMulti(DDRPHY_SHU2_CA_CMD2, P_Fld(0x1, SHU2_CA_CMD2_RG_TX_ARCLK_DRVN)
    //            | P_Fld(0x1, SHU2_CA_CMD2_RG_TX_ARCLK_DRVP));
    vIO32WriteFldMulti(DDRPHY_PLL3, P_Fld(0x0, PLL3_RG_RPHYPLL_TSTOP_EN) | P_Fld(0x0, PLL3_RG_RPHYPLL_TST_EN));
    vIO32WriteFldAlign(DDRPHY_MISC_VREF_CTRL, 0x0, MISC_VREF_CTRL_RG_RVREF_VREF_EN); //LP3 VREF

    vIO32WriteFldAlign(DDRPHY_B0_DQ3, 0x1, B0_DQ3_RG_ARDQ_RESETB_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ3, 0x1, B1_DQ3_RG_ARDQ_RESETB_B1);

    mcDELAY_US(1);

    //Ref clock should be 20M~30M, if MPLL=52M, Pre-divider should be set to 1
    vIO32WriteFldMulti(DDRPHY_SHU_PLL8, P_Fld(0x0, SHU_PLL8_RG_RPHYPLL_POSDIV) | P_Fld(0x1, SHU_PLL8_RG_RPHYPLL_PREDIV));
    //vIO32WriteFldAlign(DDRPHY_SHU2_PLL8, 0x0, SHU2_PLL8_RG_RPHYPLL_POSDIV);
    //vIO32WriteFldAlign(DDRPHY_SHU3_PLL8, 0x0, SHU3_PLL8_RG_RPHYPLL_POSDIV);
    //vIO32WriteFldAlign(DDRPHY_SHU4_PLL8, 0x0, SHU4_PLL8_RG_RPHYPLL_POSDIV);

    mcDELAY_US(1);

    vIO32WriteFldMulti(DDRPHY_SHU_PLL9, P_Fld(0x0, SHU_PLL9_RG_RPHYPLL_MONCK_EN)
                | P_Fld(0x0, SHU_PLL9_RG_RPHYPLL_MONVC_EN)
                | P_Fld(0x0, SHU_PLL9_RG_RPHYPLL_LVROD_EN)
                | P_Fld(0x1, SHU_PLL9_RG_RPHYPLL_RST_DLY));
    vIO32WriteFldMulti(DDRPHY_SHU_PLL11, P_Fld(0x0, SHU_PLL11_RG_RCLRPLL_MONCK_EN)
                | P_Fld(0x0, SHU_PLL11_RG_RCLRPLL_MONVC_EN)
                | P_Fld(0x0, SHU_PLL11_RG_RCLRPLL_LVROD_EN)
                | P_Fld(0x1, SHU_PLL11_RG_RCLRPLL_RST_DLY));

    mcDELAY_US(1);

    //Ref clock should be 20M~30M, if MPLL=52M, Pre-divider should be set to 1
    vIO32WriteFldMulti(DDRPHY_SHU_PLL10, P_Fld(0x0, SHU_PLL10_RG_RCLRPLL_POSDIV) | P_Fld(0x1, SHU_PLL10_RG_RCLRPLL_PREDIV));
    //vIO32WriteFldAlign(DDRPHY_SHU2_PLL10, 0x0, SHU2_PLL10_RG_RCLRPLL_POSDIV);
    //vIO32WriteFldAlign(DDRPHY_SHU3_PLL10, 0x0, SHU3_PLL10_RG_RCLRPLL_POSDIV);
    //vIO32WriteFldAlign(DDRPHY_SHU4_PLL10, 0x0, SHU4_PLL10_RG_RCLRPLL_POSDIV);

    mcDELAY_US(1);


    ///TODO: MIDPI Init 1
    vIO32WriteFldMulti(DDRPHY_PLL4, P_Fld(0x0, PLL4_RG_RPHYPLL_AD_MCK8X_EN)
                | P_Fld(0x1, PLL4_PLL4_RFU)
                | P_Fld(0x1, PLL4_RG_RPHYPLL_MCK8X_SEL));


    mcDELAY_US(1);

    //@Darren-vIO32WriteFldAlign(DDRPHY_SHU_PLL0, 0x3, SHU_PLL0_RG_RPHYPLL_TOP_REV); // debug1111, org:3 -> mdf:0
    vIO32WriteFldMulti(DDRPHY_SHU_PLL0, P_Fld(0x1, SHU_PLL0_ADA_MCK8X_CHB_EN)
                | P_Fld(0x1, SHU_PLL0_ADA_MCK8X_CHA_EN)); //@Darren+
    //vIO32WriteFldAlign(DDRPHY_SHU2_PLL0, 0x3, SHU2_PLL0_RG_RPHYPLL_TOP_REV);
    //vIO32WriteFldAlign(DDRPHY_SHU3_PLL0, 0x3, SHU3_PLL0_RG_RPHYPLL_TOP_REV);
    //vIO32WriteFldAlign(DDRPHY_SHU4_PLL0, 0x3, SHU4_PLL0_RG_RPHYPLL_TOP_REV);

    mcDELAY_US(1);


    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI1, 0x1, CA_DLL_ARPI1_RG_ARPISM_MCK_SEL_CA);
    vIO32WriteFldMulti(DDRPHY_B0_DQ3, P_Fld(0x1, B0_DQ3_RG_RX_ARDQ_STBENCMP_EN_B0)
                | P_Fld(0x1, B0_DQ3_RG_TX_ARDQ_EN_B0)
                | P_Fld(0x1, B0_DQ3_RG_RX_ARDQ_SMT_EN_B0));
    vIO32WriteFldMulti(DDRPHY_B1_DQ3, P_Fld(0x1, B1_DQ3_RG_RX_ARDQ_STBENCMP_EN_B1)
                | P_Fld(0x1, B1_DQ3_RG_TX_ARDQ_EN_B1)
                | P_Fld(0x1, B1_DQ3_RG_RX_ARDQ_SMT_EN_B1));



#if (fcFOR_CHIP_ID == fcKrug)
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    vIO32WriteFldAlign(DDRPHY_SHU_CA_DLL0, 0x1, SHU_CA_DLL0_RG_ARPISM_MCK_SEL_CA_SHU);//Move to DCM off setting
    vIO32WriteFldAlign(DDRPHY_SHU_CA_DLL0+SHIFT_TO_CHB_ADDR, 0x1, SHU_CA_DLL0_RG_ARPISM_MCK_SEL_CA_SHU);//Move to DCM off setting
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
#endif

    vIO32WriteFldMulti(DDRPHY_SHU_B0_DLL0, P_Fld(0x1, SHU_B0_DLL0_RG_ARDLL_PHDET_IN_SWAP_B0)
                | P_Fld(0x7, SHU_B0_DLL0_RG_ARDLL_GAIN_B0)
                | P_Fld(0x7, SHU_B0_DLL0_RG_ARDLL_IDLECNT_B0)
                | P_Fld(0x8, SHU_B0_DLL0_RG_ARDLL_P_GAIN_B0)
                | P_Fld(0x1, SHU_B0_DLL0_RG_ARDLL_PHJUMP_EN_B0)
                | P_Fld(0x1, SHU_B0_DLL0_RG_ARDLL_PHDIV_B0)
                | P_Fld(0x0, SHU_B0_DLL0_RG_ARDLL_FAST_PSJP_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_B1_DLL0, P_Fld(0x1, SHU_B1_DLL0_RG_ARDLL_PHDET_IN_SWAP_B1)
                | P_Fld(0x7, SHU_B1_DLL0_RG_ARDLL_GAIN_B1)
                | P_Fld(0x7, SHU_B1_DLL0_RG_ARDLL_IDLECNT_B1)
                | P_Fld(0x8, SHU_B1_DLL0_RG_ARDLL_P_GAIN_B1)
                | P_Fld(0x1, SHU_B1_DLL0_RG_ARDLL_PHJUMP_EN_B1)
                | P_Fld(0x1, SHU_B1_DLL0_RG_ARDLL_PHDIV_B1)
                | P_Fld(0x0, SHU_B1_DLL0_RG_ARDLL_FAST_PSJP_B1));

    vIO32WriteFldAlign(DDRPHY_SHU_CA_CMD5, 0x29, SHU_CA_CMD5_RG_RX_ARCMD_VREF_SEL);
    //vIO32WriteFldAlign(DDRPHY_SHU2_CA_CMD5, 0x0, SHU2_CA_CMD5_RG_RX_ARCMD_VREF_SEL);
    //vIO32WriteFldAlign(DDRPHY_SHU3_CA_CMD5, 0x0, SHU3_CA_CMD5_RG_RX_ARCMD_VREF_SEL);
    //vIO32WriteFldAlign(DDRPHY_SHU4_CA_CMD5, 0x0, SHU4_CA_CMD5_RG_RX_ARCMD_VREF_SEL);
    vIO32WriteFldMulti(DDRPHY_SHU_CA_CMD0, P_Fld(0x1, SHU_CA_CMD0_RG_TX_ARCMD_PRE_EN)
                | P_Fld(0x4, SHU_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
                | P_Fld(0x1, SHU_CA_CMD0_RG_TX_ARCLK_PRE_EN));
    //vIO32WriteFldMulti(DDRPHY_SHU2_CA_CMD0, P_Fld(0x1, SHU2_CA_CMD0_RG_TX_ARCMD_PRE_EN)
    //            | P_Fld(0x4, SHU2_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
    //            | P_Fld(0x1, SHU2_CA_CMD0_RG_TX_ARCLK_PRE_EN));
    //vIO32WriteFldMulti(DDRPHY_SHU3_CA_CMD0, P_Fld(0x1, SHU3_CA_CMD0_RG_TX_ARCMD_PRE_EN)
    //            | P_Fld(0x4, SHU3_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
    //            | P_Fld(0x1, SHU3_CA_CMD0_RG_TX_ARCLK_PRE_EN));
    //vIO32WriteFldMulti(DDRPHY_SHU4_CA_CMD0, P_Fld(0x1, SHU4_CA_CMD0_RG_TX_ARCMD_PRE_EN)
    //            | P_Fld(0x4, SHU4_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
    //            | P_Fld(0x1, SHU4_CA_CMD0_RG_TX_ARCLK_PRE_EN));
#if (fcFOR_CHIP_ID == fcKrug)
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    vIO32WriteFldAlign_All(DDRPHY_SHU_CA_CMD6, 0x0, SHU_CA_CMD6_RG_TX_ARCMD_SER_MODE);
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
#endif
    //vIO32WriteFldAlign(DDRPHY_SHU2_CA_CMD6, 0x3, SHU2_CA_CMD6_RG_ARPI_RESERVE_CA);
    //vIO32WriteFldAlign(DDRPHY_SHU3_CA_CMD6, 0x3, SHU3_CA_CMD6_RG_ARPI_RESERVE_CA);
    //vIO32WriteFldAlign(DDRPHY_SHU4_CA_CMD6, 0x3, SHU4_CA_CMD6_RG_ARPI_RESERVE_CA);
    //vIO32WriteFldAlign(DDRPHY_SHU_CA_CMD3, 0x4e1, SHU_CA_CMD3_RG_ARCMD_REV);
    //vIO32WriteFldAlign(DDRPHY_SHU2_CA_CMD7, 0x4e1, SHU2_CA_CMD7_RG_ARCMD_REV);
    //vIO32WriteFldAlign(DDRPHY_SHU3_CA_CMD7, 0x4e1, SHU3_CA_CMD7_RG_ARCMD_REV);
    //vIO32WriteFldAlign(DDRPHY_SHU4_CA_CMD7, 0x4e1, SHU4_CA_CMD7_RG_ARCMD_REV);
    //vIO32WriteFldMulti(DDRPHY_SHU_B0_DQ7, P_Fld(0x00, SHU_B0_DQ7_RG_ARDQ_REV_B0)
    //            | P_Fld(0x0, SHU_B0_DQ7_DQ_REV_B0_BIT_05));
    //vIO32WriteFldAlign(DDRPHY_SHU2_B0_DQ7, 0x20, SHU2_B0_DQ7_RG_ARDQ_REV_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU3_B0_DQ7, 0x20, SHU3_B0_DQ7_RG_ARDQ_REV_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU4_B0_DQ7, 0x20, SHU4_B0_DQ7_RG_ARDQ_REV_B0);
    //vIO32WriteFldMulti(DDRPHY_SHU_B1_DQ7, P_Fld(0x00, SHU_B1_DQ7_RG_ARDQ_REV_B1)
    //            | P_Fld(0x0, SHU_B1_DQ7_DQ_REV_B1_BIT_05));

 //lynx added
    //vIO32WriteFldAlign(DDRPHY_SHU_B0_DQ7, 0x0, SHU_B0_DQ7_RG_ARDQ_REV_B0); 
    //vIO32WriteFldAlign(DDRPHY_SHU_B1_DQ7, 0x0, SHU_B1_DQ7_RG_ARDQ_REV_B1); 
 //
    //vIO32WriteFldAlign(DDRPHY_SHU2_B1_DQ7, 0x20, SHU2_B1_DQ7_RG_ARDQ_REV_B1);
    //vIO32WriteFldAlign(DDRPHY_SHU3_B1_DQ7, 0x20, SHU3_B1_DQ7_RG_ARDQ_REV_B1);
    //vIO32WriteFldAlign(DDRPHY_SHU4_B1_DQ7, 0x20, SHU4_B1_DQ7_RG_ARDQ_REV_B1);
    vIO32WriteFldAlign(DDRPHY_SHU_B0_DQ6, 0x0, SHU_B0_DQ6_RG_TX_ARDQ_SER_MODE_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU2_B0_DQ6, 0x1, SHU2_B0_DQ6_RG_ARPI_RESERVE_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU3_B0_DQ6, 0x1, SHU3_B0_DQ6_RG_ARPI_RESERVE_B0);
    //vIO32WriteFldAlign(DDRPHY_SHU4_B0_DQ6, 0x1, SHU4_B0_DQ6_RG_ARPI_RESERVE_B0);
    vIO32WriteFldAlign(DDRPHY_SHU_B1_DQ6, 0x0, SHU_B1_DQ6_RG_TX_ARDQ_SER_MODE_B1);
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
    vIO32WriteFldMulti(DDRPHY_MISC_SHU_OPT+SHIFT_TO_CHB_ADDR, P_Fld(0x2, MISC_SHU_OPT_R_CA_SHU_PHDET_SPM_EN) 
                | P_Fld(0x1, MISC_SHU_OPT_R_CA_SHU_PHY_GATING_RESETB_SPM_EN) 
                | P_Fld(0x2, MISC_SHU_OPT_R_DQB1_SHU_PHDET_SPM_EN) 
                | P_Fld(0x1, MISC_SHU_OPT_R_DQB1_SHU_PHY_GATING_RESETB_SPM_EN) 
                | P_Fld(0x2, MISC_SHU_OPT_R_DQB0_SHU_PHDET_SPM_EN) 
                | P_Fld(0x1, MISC_SHU_OPT_R_DQB0_SHU_PHY_GATING_RESETB_SPM_EN));
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);

    mcDELAY_US(9);

#if (fcFOR_CHIP_ID == fcKrug)
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    vIO32WriteFldMulti(DDRPHY_SHU_CA_DLL1, P_Fld(0x1, SHU_CA_DLL1_RG_ARDLL_PD_CK_SEL_CA) | P_Fld(0x0, SHU_CA_DLL1_RG_ARDLL_FASTPJ_CK_SEL_CA));
    vIO32WriteFldMulti(DDRPHY_SHU_CA_DLL1+SHIFT_TO_CHB_ADDR, P_Fld(0x0, SHU_CA_DLL1_RG_ARDLL_PD_CK_SEL_CA)
                | P_Fld(0x1, SHU_CA_DLL1_RG_ARDLL_FASTPJ_CK_SEL_CA));//CH_B CA slave mode
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
#endif
    vIO32WriteFldMulti(DDRPHY_SHU_B0_DLL1, P_Fld(0x0, SHU_B0_DLL1_RG_ARDLL_PD_CK_SEL_B0) | P_Fld(0x1, SHU_B0_DLL1_RG_ARDLL_FASTPJ_CK_SEL_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_B1_DLL1, P_Fld(0x0, SHU_B1_DLL1_RG_ARDLL_PD_CK_SEL_B1) | P_Fld(0x1, SHU_B1_DLL1_RG_ARDLL_FASTPJ_CK_SEL_B1));

    mcDELAY_US(1);

    vIO32WriteFldAlign(DDRPHY_PLL2, 0x0, PLL2_RG_RCLRPLL_EN);
    //vIO32WriteFldAlign(DDRPHY_SHU_PLL4, 0xff, SHU_PLL4_RG_RPHYPLL_RESERVED);
    //vIO32WriteFldAlign(DDRPHY_SHU_PLL6, 0xff, SHU_PLL6_RG_RCLRPLL_RESERVED);
    vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 0xf, MISC_CG_CTRL0_CLK_MEM_DFS_CFG);

    mcDELAY_US(1);

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    DDRPhyReservedRGSetting(p);
    DDRPhyPLLSetting(p);
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);

    //rollback tMRRI design change
#if ENABLE_TMRRI_NEW_MODE
    //fix rank at 0 to trigger new TMRRI setting
    vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0, RKCFG_TXRANK);
    vIO32WriteFldAlign(DRAMC_REG_RKCFG, 1, RKCFG_TXRANKFIX);
    vIO32WriteFldAlign(DRAMC_REG_DRSCTRL, 0x0, DRSCTRL_RK_SCINPUT_OPT);// new mode, HW_MRR: R_DMMRRRK, SW_MRR: R_DMMRSRK
    vIO32WriteFldMulti(DRAMC_REG_DRAMCTRL, P_Fld(0x0, DRAMCTRL_MRRIOPT) | P_Fld(0x0, DRAMCTRL_TMRRIBYRK_DIS) | P_Fld(0x1, DRAMCTRL_TMRRICHKDIS));
    vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x1, SPCMDCTRL_SC_PG_UPD_OPT);
    vIO32WriteFldMulti(DRAMC_REG_SPCMDCTRL, P_Fld(0x0, SPCMDCTRL_SC_PG_MPRW_DIS) | P_Fld(0x0, SPCMDCTRL_SC_PG_STCMD_AREF_DIS)
                                             | P_Fld(0x0, SPCMDCTRL_SC_PG_OPT2_DIS) | P_Fld(0x0, SPCMDCTRL_SC_PG_MAN_DIS));
    vIO32WriteFldMulti(DRAMC_REG_MPC_OPTION, P_Fld(0x1, MPC_OPTION_ZQ_BLOCKALE_OPT) | P_Fld(0x1, MPC_OPTION_MPC_BLOCKALE_OPT2) | 
                                               P_Fld(0x1, MPC_OPTION_MPC_BLOCKALE_OPT1) | P_Fld(0x1, MPC_OPTION_MPC_BLOCKALE_OPT));
    //fix rank at 0 to trigger new TMRRI setting
    vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0, RKCFG_TXRANK);
    vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0, RKCFG_TXRANKFIX);
#else
    vIO32WriteFldAlign(DRAMC_REG_DRSCTRL, 0x1, DRSCTRL_RK_SCINPUT_OPT);// old mode, HW/SW MRR: R_DMMRRRK
    vIO32WriteFldMulti(DRAMC_REG_DRAMCTRL, P_Fld(0x1, DRAMCTRL_MRRIOPT) | P_Fld(0x1, DRAMCTRL_TMRRIBYRK_DIS) | P_Fld(0x0, DRAMCTRL_TMRRICHKDIS));
    vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x0, SPCMDCTRL_SC_PG_UPD_OPT);
    vIO32WriteFldMulti(DRAMC_REG_SPCMDCTRL, P_Fld(0x1, SPCMDCTRL_SC_PG_MPRW_DIS) | P_Fld(0x1, SPCMDCTRL_SC_PG_STCMD_AREF_DIS)
                                             | P_Fld(0x1, SPCMDCTRL_SC_PG_OPT2_DIS) | P_Fld(0x1, SPCMDCTRL_SC_PG_MAN_DIS));
#endif
    vIO32WriteFldAlign(DRAMC_REG_CKECTRL, 0x1, CKECTRL_RUNTIMEMRRCKEFIX);//Set Run time MRR CKE fix to 1 in tMRRI old mode to avoid no ACK from precharge all
    vIO32WriteFldAlign(DRAMC_REG_CKECTRL, 0x0, CKECTRL_RUNTIMEMRRMIODIS);

    vIO32WriteFldAlign(DDRPHY_B0_DQ9, 0x1, B0_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ9, 0x1, B1_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B1);
    vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQSIEN, P_Fld(0xf, SHURK1_DQSIEN_R1DQS3IEN)
                | P_Fld(0xf, SHURK1_DQSIEN_R1DQS2IEN)
                | P_Fld(0xf, SHURK1_DQSIEN_R1DQS1IEN)
                | P_Fld(0xf, SHURK1_DQSIEN_R1DQS0IEN));
    vIO32WriteFldMulti(DRAMC_REG_STBCAL1, P_Fld(0x0, STBCAL1_DLLFRZ_MON_PBREF_OPT)
                | P_Fld(0x1, STBCAL1_STB_FLAGCLR)
                | P_Fld(0x1, STBCAL1_STBCNT_SHU_RST_EN) // from HJ review bring-up setting
                | P_Fld(0x1, STBCAL1_STBCNT_MODESEL));
    /* Darren-
    vIO32WriteFldMulti(DRAMC_REG_SHU_DQSG_RETRY, P_Fld(0x1, SHU_DQSG_RETRY_R_RETRY_USE_BURST_MDOE)
                | P_Fld(0x1, SHU_DQSG_RETRY_R_RDY_SEL_DLE)
                | P_Fld(0x6, SHU_DQSG_RETRY_R_DQSIENLAT)
                | P_Fld(0x1, SHU_DQSG_RETRY_R_RETRY_ONCE));
    */
    vIO32WriteFldMulti(DRAMC_REG_SHU_DRVING1, P_Fld(0xa, SHU_DRVING1_DQSDRVP2) | P_Fld(0xa, SHU_DRVING1_DQSDRVN2)
                | P_Fld(0xa, SHU_DRVING1_DQSDRVP1) | P_Fld(0xa, SHU_DRVING1_DQSDRVN1)
                | P_Fld(0xa, SHU_DRVING1_DQDRVP2) | P_Fld(0xa, SHU_DRVING1_DQDRVN2));
    vIO32WriteFldMulti(DRAMC_REG_SHU_DRVING2, P_Fld(0xa, SHU_DRVING2_DQDRVP1) | P_Fld(0xa, SHU_DRVING2_DQDRVN1)
                | P_Fld(0xa, SHU_DRVING2_CMDDRVP2) | P_Fld(0xa, SHU_DRVING2_CMDDRVN2)
                | P_Fld(0xa, SHU_DRVING2_CMDDRVP1) | P_Fld(0xa, SHU_DRVING2_CMDDRVN1));
    vIO32WriteFldMulti(DRAMC_REG_SHU_DRVING3, P_Fld(0xa, SHU_DRVING3_DQSODTP2) | P_Fld(0xa, SHU_DRVING3_DQSODTN2)
                | P_Fld(0xa, SHU_DRVING3_DQSODTP) | P_Fld(0xa, SHU_DRVING3_DQSODTN)
                | P_Fld(0xa, SHU_DRVING3_DQODTP2) | P_Fld(0xa, SHU_DRVING3_DQODTN2));
    vIO32WriteFldMulti(DRAMC_REG_SHU_DRVING4, P_Fld(0xa, SHU_DRVING4_DQODTP1) | P_Fld(0xa, SHU_DRVING4_DQODTN1)
                | P_Fld(0xa, SHU_DRVING4_CMDODTP2) | P_Fld(0xa, SHU_DRVING4_CMDODTN2)
                | P_Fld(0xa, SHU_DRVING4_CMDODTP1) | P_Fld(0xa, SHU_DRVING4_CMDODTN1));
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
                | P_Fld(0x1, MISC_CTRL1_R_DM_TX_ARCMD_OE)
#if ENABLE_PINMUX_FOR_RANK_SWAP
                | P_Fld(0x1, MISC_CTRL1_R_RK_PINMUXSWAP_EN)
#endif
                | P_Fld(0x1, MISC_CTRL1_R_DM_TX_ARCLK_OE));
    vIO32WriteFldAlign(DDRPHY_B0_RXDVS0, 1, B0_RXDVS0_R_HWSAVE_MODE_ENA_B0);
    vIO32WriteFldAlign(DDRPHY_B1_RXDVS0, 1, B1_RXDVS0_R_HWSAVE_MODE_ENA_B1);
    vIO32WriteFldAlign(DDRPHY_CA_RXDVS0, 0, CA_RXDVS0_R_HWSAVE_MODE_ENA_CA);
    
    vIO32WriteFldAlign(DDRPHY_CA_CMD7, 0x0, CA_CMD7_RG_TX_ARCMD_PULL_DN);
    vIO32WriteFldAlign(DDRPHY_CA_CMD7, 0x0, CA_CMD7_RG_TX_ARCS_PULL_DN); // Added by Lingyun.Wu, 11-15
    vIO32WriteFldAlign(DDRPHY_B0_DQ7, 0x0, B0_DQ7_RG_TX_ARDQ_PULL_DN_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ7, 0x0, B1_DQ7_RG_TX_ARDQ_PULL_DN_B1);
    //vIO32WriteFldAlign(DDRPHY_CA_CMD8, 0x0, CA_CMD8_RG_TX_RRESETB_PULL_DN); //Already set in vDramcInit_PreSettings()
    
    //For 1:8 mode start=================
    // 1:8 mode
    vIO32WriteFldMulti(DRAMC_REG_SHU_CONF0, P_Fld(0x1, SHU_CONF0_DM64BITEN)
                | P_Fld(0x0, SHU_CONF0_FDIV2)
                | P_Fld(0x1, SHU_CONF0_FREQDIV4)
                | P_Fld(0x1, SHU_CONF0_DUALSCHEN)
                | P_Fld(0x0, SHU_CONF0_WDATRGO)
                | P_Fld(u1MaType, SHU_CONF0_MATYPE)                
                | P_Fld(0x1, SHU_CONF0_BL4)
                | P_Fld(0x1, SHU_CONF0_REFTHD)
                | P_Fld(0x1, SHU_CONF0_ADVPREEN)
                | P_Fld(0x3f, SHU_CONF0_DMPGTIM));                

    vIO32WriteFldAlign(DRAMC_REG_SHU_APHY_TX_PICG_CTRL, 0x5, SHU_APHY_TX_PICG_CTRL_DDRPHY_CLK_DYN_GATING_SEL);
    vIO32WriteFldAlign(DRAMC_REG_SHU_SELPH_CA1, 0x0, SHU_SELPH_CA1_TXDLY_CKE);
    vIO32WriteFldAlign(DRAMC_REG_SHU_SELPH_CA2, 0x0, SHU_SELPH_CA2_TXDLY_CKE1);

#if (fcFOR_CHIP_ID == fcKrug)
    if(vGet_Div_Mode(p) == DIV4_MODE)
    {
        u1TAIL_LAT = 1;
    }
    else
 //DIV8_MODE
    {
        u1TAIL_LAT = 0;
    }

    //PICG old mode
    vIO32WriteFldMulti(DRAMC_REG_SHU_APHY_TX_PICG_CTRL, P_Fld(0x0, SHU_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_DQ_RK0_SEL_P0)
                | P_Fld(0x0, SHU_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_DQ_RK0_SEL_P1)
                | P_Fld(0x0, SHU_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_PICG_CNT));

    //PICG_DQSIEN old/new project have same settings
    vIO32WriteFldMulti(DRAMC_REG_SHU_STBCAL, P_Fld(u1TAIL_LAT, SHU_STBCAL_R1_DQSIEN_PICG_TAIL_EXT_LAT) //TAIL should be set as same value by old/new mode, no matter it is old or new mode
                | P_Fld(u1TAIL_LAT, SHU_STBCAL_R0_DQSIEN_PICG_TAIL_EXT_LAT) //TAIL should be set as same value by old/new mode, no matter it is old or new mode
                | P_Fld(u1TAIL_LAT, SHU_STBCAL_DQSIEN_RX_SELPH_OPT));

    //RX_IN_GATE_EN old mode
    if(vGet_Div_Mode(p) == DIV4_MODE) // @HJ, Gate_en extend for Rx DQS
    {
        if(p->frequency <= 400)
            u1RxInGateEn = 0x2;
        else
            u1RxInGateEn = 0x1;
    }
    else            
        u1RxInGateEn = 0x0;
    vIO32WriteFldMulti(DRAMC_REG_SHU_PHY_RX_CTRL, P_Fld(u1RxInGateEn, SHU_PHY_RX_CTRL_RX_IN_GATE_EN_TAIL)
                | P_Fld(0x0, SHU_PHY_RX_CTRL_RX_IN_GATE_EN_HEAD));
#endif
    //For 1:8 mode end=================
	
    vIO32WriteFldMulti(DRAMC_REG_SHU_ODTCTRL, P_Fld(0x1, SHU_ODTCTRL_RODTE)
                | P_Fld(0x1, SHU_ODTCTRL_RODTE2)
                | P_Fld(0x0, SHU_ODTCTRL_TWODT) // @Jouling, design no use (floating)
                //| P_Fld(0x5, SHU_ODTCTRL_RODT) //Set in UpdateACTimingReg()
                | P_Fld(0x0, SHU_ODTCTRL_WOEN) // @Jouling, design no use (floating)
                | P_Fld(0x1, SHU_ODTCTRL_ROEN));
    vIO32WriteFldAlign(DDRPHY_SHU_B0_DQ7, 0x1, SHU_B0_DQ7_R_DMRODTEN_B0);
    vIO32WriteFldAlign(DDRPHY_SHU_B1_DQ7, 0x1, SHU_B1_DQ7_R_DMRODTEN_B1);
    vIO32WriteFldAlign(DRAMC_REG_REFCTRL0, 0x5, REFCTRL0_REF_PREGATE_CNT);
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_CA1, P_Fld(0x0, SHU_SELPH_CA1_TXDLY_CS1)
                | P_Fld(0x0, SHU_SELPH_CA1_TXDLY_RAS)
                | P_Fld(0x0, SHU_SELPH_CA1_TXDLY_CAS)
                | P_Fld(0x0, SHU_SELPH_CA1_TXDLY_WE)
                | P_Fld(0x0, SHU_SELPH_CA1_TXDLY_RESET)
                | P_Fld(0x0, SHU_SELPH_CA1_TXDLY_ODT)
                | P_Fld(0x0, SHU_SELPH_CA1_TXDLY_CS));
    
    if (p->frequency<1333)
        u1TXDLY_CMD = 0x7;
    else if (p->frequency<1600)
        u1TXDLY_CMD = 0x8;
    else if (p->frequency<1866)
        u1TXDLY_CMD = 0x9;
    else if (p->frequency<2133)
        u1TXDLY_CMD = 0xA;
    else
        u1TXDLY_CMD = 0xB;
    
    vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_CA2, P_Fld(u1TXDLY_CMD, SHU_SELPH_CA2_TXDLY_CMD)
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
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B1_DQ7, P_Fld(0x1a, SHU_R0_B1_DQ7_RK0_ARPI_DQM_B1)
                | P_Fld(0x1a, SHU_R0_B1_DQ7_RK0_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R0_B0_DQ7, P_Fld(0x1a, SHU_R0_B0_DQ7_RK0_ARPI_DQM_B0)
                | P_Fld(0x1a, SHU_R0_B0_DQ7_RK0_ARPI_DQ_B0));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B1_DQ7, P_Fld(0x14, SHU_R1_B1_DQ7_RK1_ARPI_DQM_B1)
                | P_Fld(0x14, SHU_R1_B1_DQ7_RK1_ARPI_DQ_B1));
    vIO32WriteFldMulti(DDRPHY_SHU_R1_B0_DQ7, P_Fld(0x14, SHU_R1_B0_DQ7_RK1_ARPI_DQM_B0)
                | P_Fld(0x14, SHU_R1_B0_DQ7_RK1_ARPI_DQ_B0));

    mcDELAY_US(1);

    vIO32WriteFldAlign(DDRPHY_B1_DQ9, 0x1, B1_DQ9_RG_RX_ARDQS0_DQSIENMODE_B1);
    vIO32WriteFldAlign(DDRPHY_B0_DQ9, 0x1, B0_DQ9_RG_RX_ARDQS0_DQSIENMODE_B0);
    vIO32WriteFldAlign(DDRPHY_B0_DQ6, 0x2, B0_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B0);
    vIO32WriteFldAlign(DDRPHY_B1_DQ6, 0x2, B1_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B1);
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
    vIO32WriteFldMulti(DRAMC_REG_RKCFG, P_Fld(0x1, RKCFG_CKE2RANK)
                | P_Fld(0x1, RKCFG_CKE2RANK_OPT2)); // Darren for CKE dependent
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU_CONF2, P_Fld(0x1, SHU_CONF2_WPRE2T)
                    | P_Fld(0x7, SHU_CONF2_DCMDLYREF));
                    //| P_Fld(0x64, SHU_CONF2_FSPCHG_PRDCNT)); //ACTiming related -> set in UpdateACTiming_Reg()
        vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x1, SPCMDCTRL_CLR_EN);
        //vIO32WriteFldAlign(DRAMC_REG_SHU_SCINTV, 0xf, SHU_SCINTV_MRW_INTV); (Set in UpdateACTimingReg())
        vIO32WriteFldAlign(DRAMC_REG_SHUCTRL1, 0x40, SHUCTRL1_FC_PRDCNT);
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHU_CONF2, P_Fld(0x1, SHU_CONF2_WPRE2T)
                    | P_Fld(0x7, SHU_CONF2_DCMDLYREF));
                    //| P_Fld(0x64, SHU_CONF2_FSPCHG_PRDCNT)); //ACTiming related -> set in UpdateACTiming_Reg()
        vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x1, SPCMDCTRL_CLR_EN);
        //vIO32WriteFldAlign(DRAMC_REG_SHU_SCINTV, 0xf, SHU_SCINTV_MRW_INTV); (Set in UpdateACTimingReg())
        vIO32WriteFldAlign(DRAMC_REG_SHUCTRL1, 0x40, SHUCTRL1_FC_PRDCNT);
    }
    vIO32WriteFldAlign(DRAMC_REG_SHUCTRL, 0x1, SHUCTRL_LPSM_BYPASS_B);
    vIO32WriteFldMulti(DRAMC_REG_REFCTRL1, P_Fld(0x0, REFCTRL1_SREF_PRD_OPT) | P_Fld(0x0, REFCTRL1_PSEL_OPT1) | P_Fld(0x0, REFCTRL1_PSEL_OPT2) | P_Fld(0x0, REFCTRL1_PSEL_OPT3));
    //vIO32WriteFldAlign(DDRPHY_SHU_PLL4, 0xfe, SHU_PLL4_RG_RPHYPLL_RESERVED);
    //vIO32WriteFldAlign(DDRPHY_SHU_PLL6, 0xfe, SHU_PLL6_RG_RCLRPLL_RESERVED);
    vIO32WriteFldMulti(DRAMC_REG_REFRATRE_FILTER, P_Fld(0x1, REFRATRE_FILTER_PB2AB_OPT) | P_Fld(0x0, REFRATRE_FILTER_PB2AB_OPT1));

#if !APPLY_LP4_POWER_INIT_SEQUENCE
    vIO32WriteFldAlign(DDRPHY_MISC_CTRL1, 0x1, MISC_CTRL1_R_DMDA_RRESETB_I);
#endif
    vIO32WriteFldAlign(DRAMC_REG_DRAMCTRL, 0x0, DRAMCTRL_CLKWITRFC);
    vIO32WriteFldMulti(DRAMC_REG_MISCTL0, P_Fld(0x1, MISCTL0_REFP_ARB_EN2)
                | P_Fld(0x1, MISCTL0_PRE_DLE_VLD_OPT) // MISCTL0_PRE_DLE_VLD_OPT from Chris review
                | P_Fld(0x1, MISCTL0_PBC_ARB_EN)
                | P_Fld(0x1, MISCTL0_REFA_ARB_EN2));
    vIO32WriteFldMulti(DRAMC_REG_PERFCTL0, P_Fld(0x1, PERFCTL0_MWHPRIEN)
                //| P_Fld(0x1, PERFCTL0_RWSPLIT) //Set in UpdateInitialSettings_LP4()
                | P_Fld(0x1, PERFCTL0_WFLUSHEN)
                | P_Fld(0x1, PERFCTL0_EMILLATEN)
                | P_Fld(0x1, PERFCTL0_RWAGEEN)
                | P_Fld(0x1, PERFCTL0_RWLLATEN)
                | P_Fld(0x1, PERFCTL0_RWHPRIEN)
                | P_Fld(0x1, PERFCTL0_RWOFOEN)
                | P_Fld(0x1, PERFCTL0_DISRDPHASE1));
    vIO32WriteFldAlign(DRAMC_REG_ARBCTL, 0x80, ARBCTL_MAXPENDCNT);
    vIO32WriteFldMulti(DRAMC_REG_PADCTRL, P_Fld(0x1, PADCTRL_DQIENLATEBEGIN)
                | P_Fld(0x1, PADCTRL_DQIENQKEND));
    vIO32WriteFldAlign(DRAMC_REG_DRAMC_PD_CTRL, 0x1, DRAMC_PD_CTRL_DCMREF_OPT);
    vIO32WriteFldMulti(DRAMC_REG_CLKCTRL, P_Fld(0x1, CLKCTRL_CLK_EN_1)
                | P_Fld(0x1, CLKCTRL_CLK_EN_0)); // CLK_EN_0 from Jouling review
    vIO32WriteFldMulti(DRAMC_REG_REFCTRL0, P_Fld(0x4, REFCTRL0_DISBYREFNUM)
                | P_Fld(0x1, REFCTRL0_DLLFRZ));
#if 0 //CATRAIN_INTV, CATRAINLAT: Correct values are set in UpdateACTimingReg( )
    vIO32WriteFldMulti(DRAMC_REG_CATRAINING1, P_Fld(0xff, CATRAINING1_CATRAIN_INTV)
                | P_Fld(0x0, CATRAINING1_CATRAINLAT));
#endif
#if 0
    vIO32WriteFldMulti(DRAMC_REG_SHU_RANKCTL, P_Fld(0x6, SHU_RANKCTL_RANKINCTL_PHY)
                | P_Fld(0x4, SHU_RANKCTL_RANKINCTL_ROOT1)
                | P_Fld(0x4, SHU_RANKCTL_RANKINCTL));
#endif

    //vIO32WriteFldAlign(DRAMC_REG_SHU_STBCAL, 0x1, SHU_STBCAL_DMSTBLAT);
    //vIO32WriteFldAlign(DRAMC_REG_SHURK0_DQSCTL, 0x5, SHURK0_DQSCTL_DQSINCTL); //DQSINCTL: set in UpdateACTimingReg()
    //vIO32WriteFldAlign(DRAMC_REG_SHURK1_DQSCTL, 0x5, SHURK1_DQSCTL_R1DQSINCTL); //Set in UpdateACTimingReg()

    mcDELAY_US(2);
    #if LEGACY_GATING_DLY
    LegacyGatingDlyLP4_DDR3200(p);
    #endif
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQSIEN, P_Fld(0x19, SHURK0_DQSIEN_R0DQS1IEN)
                    | P_Fld(0x19, SHURK0_DQSIEN_R0DQS0IEN));
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

    vIO32WriteFldAlign(DRAMC_REG_SHU_CONF1, 0x1, SHU_CONF1_TREFBWIG);
    vIO32WriteFldAlign(DRAMC_REG_SHU_CONF3, 0xff, SHU_CONF3_REFRCNT);
    vIO32WriteFldAlign(DRAMC_REG_REFCTRL0, 0x1, REFCTRL0_REFFRERUN);
    vIO32WriteFldAlign(DRAMC_REG_SREFCTRL, 0x1, SREFCTRL_SREF_HW_EN);
    vIO32WriteFldAlign(DRAMC_REG_MPC_OPTION, 0x1, MPC_OPTION_MPCRKEN);
    vIO32WriteFldAlign(DRAMC_REG_DRAMC_PD_CTRL, 0x1, DRAMC_PD_CTRL_PHYCLKDYNGEN);//Move to DCM off setting
    vIO32WriteFldAlign(DRAMC_REG_DRAMC_PD_CTRL, 0x1, DRAMC_PD_CTRL_DCMEN);//Move to DCM off setting
    vIO32WriteFldMulti(DRAMC_REG_EYESCAN, P_Fld(0x0, EYESCAN_RX_DQ_EYE_SEL)
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
                | P_Fld(0x1, TEST2_3_TESTAUDPAT));
    vIO32WriteFldAlign(DRAMC_REG_RSTMASK, 0x0, RSTMASK_DAT_SYNC_MASK);
    vIO32WriteFldAlign(DRAMC_REG_RSTMASK, 0x0, RSTMASK_PHY_SYNC_MASK);

    mcDELAY_US(1);

    vIO32WriteFldMulti(DRAMC_REG_HW_MRR_FUN, P_Fld(0x0, HW_MRR_FUN_TRPMRR_EN)
                    | P_Fld(0x0, HW_MRR_FUN_TRCDMRR_EN) | P_Fld(0x0, HW_MRR_FUN_TMRR_ENA)
                    | P_Fld(0x0, HW_MRR_FUN_MANTMRR_EN)); // from YH Tsai review for samsung MRR/Read cmd issue
    if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
    {
        vIO32WriteFldAlign(DRAMC_REG_PERFCTL0, 0x1, PERFCTL0_WRFIO_MODE2);
                    //| P_Fld(0x0, PERFCTL0_RWSPLIT)); //Set in UpdateInitialSettings_LP4()
        vIO32WriteFldMulti(DRAMC_REG_PERFCTL0, P_Fld(0x1, PERFCTL0_REORDEREN)
                    | P_Fld(0x0, PERFCTL0_REORDER_MODE));
        vIO32WriteFldAlign(DRAMC_REG_RSTMASK, 0x0, RSTMASK_GT_SYNC_MASK);
        vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0x1, RKCFG_DQSOSC2RK);
        vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x1, SPCMDCTRL_REFR_BLOCKEN);
        vIO32WriteFldAlign(DRAMC_REG_EYESCAN, 0x0, EYESCAN_RG_RX_MIOCK_JIT_EN);
    }
    else
    {
        vIO32WriteFldAlign(DRAMC_REG_DRAMCTRL, 0x0, DRAMCTRL_CTOREQ_HPRI_OPT);
        vIO32WriteFldMulti(DRAMC_REG_PERFCTL0, P_Fld(0x1, PERFCTL0_REORDEREN)
                    | P_Fld(0x0, PERFCTL0_REORDER_MODE));
        vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x1, SPCMDCTRL_REFR_BLOCKEN);
        vIO32WriteFldAlign(DRAMC_REG_RSTMASK, 0x0, RSTMASK_GT_SYNC_MASK);
        vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0x1, RKCFG_DQSOSC2RK);
        vIO32WriteFldAlign(DRAMC_REG_MPC_OPTION, 1, MPC_OPTION_MPCRKEN);
        vIO32WriteFldAlign(DRAMC_REG_EYESCAN, 0x0, EYESCAN_RG_RX_MIOCK_JIT_EN);
        vIO32WriteFldAlign(DDRPHY_SHU_B0_DQ7, 0x1, SHU_B0_DQ7_R_DMDQMDBI_SHU_B0);
        vIO32WriteFldAlign(DDRPHY_SHU_B1_DQ7, 0x1, SHU_B1_DQ7_R_DMDQMDBI_SHU_B1);
        vIO32WriteFldAlign(DRAMC_REG_SHU_RANKCTL, 0x4, SHU_RANKCTL_RANKINCTL);
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ1)
                    | P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM1)
                    | P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ1)
                    | P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM1)
                    | P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM0));
    }
#if ENABLE_TMRRI_NEW_MODE
    vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x0, SPCMDCTRL_REFR_BLOCKEN);
    vIO32WriteFldAlign(DRAMC_REG_HW_MRR_FUN, 0x1, HW_MRR_FUN_TMRR_ENA);
#endif
    mcDELAY_US(5);

    vIO32WriteFldAlign(DRAMC_REG_STBCAL1, 0x3, STBCAL1_STBCAL_FILTER);
    vIO32WriteFldAlign(DRAMC_REG_STBCAL1, 0x1, STBCAL1_STBCAL_FILTER);
    vIO32WriteFldMulti(DRAMC_REG_STBCAL, P_Fld(0x1, STBCAL_STB_DQIEN_IG)
                | P_Fld(0x1, STBCAL_PICHGBLOCK_NORD)
                | P_Fld(0x0, STBCAL_STBCALEN)
                | P_Fld(0x0, STBCAL_STB_SELPHYCALEN)
                | P_Fld(0x1, STBCAL_PIMASK_RKCHG_OPT));
    vIO32WriteFldAlign(DRAMC_REG_STBCAL1, 0x1, STBCAL1_STB_SHIFT_DTCOUT_IG);
    vIO32WriteFldMulti(DRAMC_REG_SHU_DQSG, P_Fld(0x9, SHU_DQSG_STB_UPDMASKCYC)
                | P_Fld(0x1, SHU_DQSG_STB_UPDMASK_EN));
    vIO32WriteFldAlign(DDRPHY_MISC_CTRL0, 0x0, MISC_CTRL0_R_DMDQSIEN_SYNCOPT);
    vIO32WriteFldAlign(DRAMC_REG_SHU_STBCAL, 0x1, SHU_STBCAL_DQSG_MODE);
    vIO32WriteFldAlign(DRAMC_REG_STBCAL, 0x1, STBCAL_SREF_DQSGUPD);
    //M17_Remap:vIO32WriteFldAlign(DDRPHY_MISC_CTRL1, 0x0, MISC_CTRL1_R_DMDQMDBI);
    /* RX Tracking DQM SM enable (actual values are set in DramcRxInputDelayTrackingHW()) */
#if ENABLE_RX_TRACKING_LP4
    vIO32WriteFldAlign(DDRPHY_SHU_B0_DQ7, p->DBI_R_onoff[p->dram_fsp], SHU_B0_DQ7_R_DMRXTRACK_DQM_EN_B0);
    vIO32WriteFldAlign(DDRPHY_SHU_B1_DQ7, p->DBI_R_onoff[p->dram_fsp], SHU_B1_DQ7_R_DMRXTRACK_DQM_EN_B1);
#else
    vIO32WriteFldAlign(DDRPHY_SHU_B0_DQ7, 0, SHU_B0_DQ7_R_DMRXTRACK_DQM_EN_B0);
    vIO32WriteFldAlign(DDRPHY_SHU_B1_DQ7, 0, SHU_B1_DQ7_R_DMRXTRACK_DQM_EN_B1);
#endif
//Sylv_ia MP setting should set SHU_STBCAL_DMSTBLAT as 0x2 (review by HJ Huang)
    vIO32WriteFldMulti(DRAMC_REG_SHU_STBCAL, P_Fld(0x1, SHU_STBCAL_PICGLAT)
                | P_Fld(0x2, SHU_STBCAL_DMSTBLAT));
    vIO32WriteFldMulti(DRAMC_REG_REFCTRL1, P_Fld(0x1, REFCTRL1_REF_QUE_AUTOSAVE_EN)
                | P_Fld(0x1, REFCTRL1_SLEFREF_AUTOSAVE_EN));
    vIO32WriteFldMulti(DRAMC_REG_DQSOSCR, P_Fld(0x1, DQSOSCR_SREF_TXPI_RELOAD_OPT)
                | P_Fld(0x0, DQSOSCR_SREF_TXUI_RELOAD_OPT)); //@Jouling, UI reloade path is updated.
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
#if 0
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
#endif
#if LEGACY_TX_TRACK
        LegacyTxTrackLP4_DDR3200(p);
#endif

#if ENABLE_WRITE_POST_AMBLE_1_POINT_5_TCK
        vIO32WriteFldAlign(DRAMC_REG_SHU_WODT, p->dram_fsp, SHU_WODT_WPST1P5T); //Set write post-amble by FSP with MR3
#else
        vIO32WriteFldAlign(DRAMC_REG_SHU_WODT, 0x0, SHU_WODT_WPST1P5T); //Set write post-amble by FSP with MR3
#endif
        
        //vIO32WriteFldAlign(DRAMC_REG_SHU_HWSET_MR2, 0x2d, SHU_HWSET_MR2_HWSET_MR2_OP);
        if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
        {
                vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQSIEN, P_Fld(0x19, SHURK0_DQSIEN_R0DQS1IEN)
                            | P_Fld(0x19, SHURK0_DQSIEN_R0DQS0IEN));
        }
        vIO32WriteFldMulti(DRAMC_REG_SHURK0_PI, P_Fld(0x1a, SHURK0_PI_RK0_ARPI_DQM_B1)
                    | P_Fld(0x1a, SHURK0_PI_RK0_ARPI_DQM_B0)
                    | P_Fld(0x1a, SHURK0_PI_RK0_ARPI_DQ_B1)
                    | P_Fld(0x1a, SHURK0_PI_RK0_ARPI_DQ_B0));
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
                vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQSIEN, P_Fld(0x0, SHURK1_DQSIEN_R1DQS3IEN)
                            | P_Fld(0x0, SHURK1_DQSIEN_R1DQS2IEN)
                            | P_Fld(0x1b, SHURK1_DQSIEN_R1DQS1IEN)
                            | P_Fld(0x1b, SHURK1_DQSIEN_R1DQS0IEN));
        }
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_PI, P_Fld(0x14, SHURK1_PI_RK1_ARPI_DQM_B1)
                    | P_Fld(0x14, SHURK1_PI_RK1_ARPI_DQM_B0)
                    | P_Fld(0x14, SHURK1_PI_RK1_ARPI_DQ_B1)
                    | P_Fld(0x14, SHURK1_PI_RK1_ARPI_DQ_B0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ2, P_Fld(0x4, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ3)
                    | P_Fld(0x4, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ2)
                    | P_Fld(0x4, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ1)
                    | P_Fld(0x4, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ0));
        vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ3, P_Fld(0x4, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM3)
                    | P_Fld(0x4, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM2)
                    | P_Fld(0x4, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM1)
                    | P_Fld(0x4, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM0));
        vIO32WriteFldMulti(DRAMC_REG_SHU_DQSG_RETRY, P_Fld(0x0, SHU_DQSG_RETRY_R_XSR_RETRY_SPM_MODE)
                    | P_Fld(0x0, SHU_DQSG_RETRY_R_DDR1866_PLUS)); //La_fite MP setting = 0
#if LEGACY_TDQSCK_PRECAL
        LegacyPreCalLP4_DDR3200(p);
#endif
        //               61832 ===dramc_shu1_lp4_3200 end===


        //               66870 ===ddrphy_shu1_lp4_3200_CHA begin===
        if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
        {
                vIO32WriteFldAlign(DDRPHY_SHU_B0_DQ7, 0x1, SHU_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0);
                vIO32WriteFldAlign(DDRPHY_SHU_B1_DQ7, 0x1, SHU_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1);
        }
        else
        {
                vIO32WriteFldMulti(DDRPHY_SHU_B0_DQ7, P_Fld(0x1, SHU_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0)
                            | P_Fld(0x1, SHU_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0));
                vIO32WriteFldMulti(DDRPHY_SHU_B1_DQ7, P_Fld(0x1, SHU_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1)
                            | P_Fld(0x1, SHU_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1));
//francis remove : it will make CLRPLL frequency wrong!
//francis remove                vIO32WriteFldMulti(DDRPHY_SHU_PLL7, P_Fld(0x3d00, SHU_PLL7_RG_RCLRPLL_SDM_PCW)
//francis remove                            | P_Fld(0x1, SHU_PLL7_RG_RCLRPLL_SDM_PCW_CHG));
        }
        //               67761 ===ddrphy_shu1_lp4_3200_CHA end===
    }
    ///TODO: DDR3733
    if(p->freqGroup == 2133)
    {
        DramcSetting_Olympus_LP4_ByteMode_DDR4266(p);
    }
    else if(p->freqGroup == 1866)
    {
        DramcSetting_Olympus_LP4_ByteMode_DDR3733(p);
    }
    ///TODO: DDR2667
    else if (p->freqGroup == 1333 || p->freqGroup == 1200) // TODO: Initial settings for DDR2400?
    {
        DramcSetting_Olympus_LP4_ByteMode_DDR2667(p);
    }
    else if ((p->freqGroup == 800) || (p->freqGroup == 600) || (p->freqGroup == 400))
    {
        DramcSetting_Olympus_LP4_ByteMode_DDR1600(p);
    }

    UpdateInitialSettings_LP4(p);

#if SIMULATION_SW_IMPED // Darren: Need porting by E2 IMP Calib DVT owner
    #if FSP1_CLKCA_TERM
    U8 u1CASwImpFreqRegion = (p->dram_fsp==FSP_0) ? IMP_LOW_FREQ : IMP_HIGH_FREQ;
    #else
    U8 u1CASwImpFreqRegion = (p->frequency <= 1866) ? IMP_LOW_FREQ : IMP_HIGH_FREQ;
    #endif
    U8 u1DQSwImpFreqRegion = (p->frequency <= 1866) ? IMP_LOW_FREQ : IMP_HIGH_FREQ;

    if (p->dram_type == TYPE_LPDDR4X)
        DramcSwImpedanceSaveRegister(p, u1CASwImpFreqRegion, u1DQSwImpFreqRegion, DRAM_DFS_SHUFFLE_1);
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
    vIO32WriteFldMulti(DRAMC_REG_SHUCTRL2, P_Fld(0x1, SHUCTRL2_MR13_SHU_EN)
                | P_Fld(0x1, SHUCTRL2_HWSET_WLRL));
    vIO32WriteFldAlign(DRAMC_REG_REFCTRL0, 0x1, REFCTRL0_REFDIS);
    //vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x0, SPCMDCTRL_REFRDIS);//Lewis@20160613: Fix refresh rate is wrong while diable MR4
    vIO32WriteFldAlign(DRAMC_REG_DRAMCTRL, 0x1, DRAMCTRL_REQQUE_THD_EN);
                //| P_Fld(0x1, DRAMCTRL_DPDRK_OPT)); //DPDRK_OPT doesn't exit for Sylv_ia
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
    vIO32WriteFldMulti(DRAMC_REG_SHUCTRL, P_Fld(0x1, SHUCTRL_R_DRAMC_CHA) | P_Fld(0x0, SHUCTRL_SHU_PHYRST_SEL));
    vIO32WriteFldAlign(DRAMC_REG_SHUCTRL2, 0x1, SHUCTRL2_R_DVFS_DLL_CHA);
    //CH-B
    vIO32WriteFldMulti(DRAMC_REG_SHUCTRL+SHIFT_TO_CHB_ADDR, P_Fld(0x0, SHUCTRL_R_DRAMC_CHA) | P_Fld(0x1, SHUCTRL_SHU_PHYRST_SEL));
    vIO32WriteFldAlign(DRAMC_REG_SHUCTRL2+SHIFT_TO_CHB_ADDR, 0x0, SHUCTRL2_R_DVFS_DLL_CHA);
    //               60826 ===LP4_3200_intial_setting_shu1 end===

#ifndef LOOPBACK_TEST
    DDRPhyFMeter_Init(p);
#endif

    DVFSSettings(p);

}

void vApplyConfigAfterCalibration(DRAMC_CTX_T *p)
{
    U8 shu_index;
#if ENABLE_TMRRI_NEW_MODE
    U8 u1RankIdx;
#endif

/*================================
    PHY RX Settings
==================================*/
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL4, 0x11400000, MISC_CG_CTRL4_R_PHY_MCK_CG_CTRL);
    vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL1, 0x0, REFCTRL1_SREF_CG_OPT);
    vIO32WriteFldAlign_All(DRAMC_REG_SHUCTRL, 0x0, SHUCTRL_DVFS_CG_OPT);//Move to DCM off setting

    /* Burst mode settings are removed from here due to
     *  1. Set in UpdateInitialSettings_LP4
     *  2. DQS Gating ensures new burst mode is switched when to done
     *     (or doesn't switch gatingMode at all, depending on "LP4_GATING_OLD_BURST_MODE")
     */

    vIO32WriteFldAlign_All(DDRPHY_CA_CMD6, 0x0, CA_CMD6_RG_RX_ARCMD_RES_BIAS_EN);
#if 0
    vIO32WriteFldAlign_All(DDRPHY_B0_DQ6, 0x0, B0_DQ6_RG_TX_ARDQ_OE_EXT_DIS_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DQ6, 0x0, B1_DQ6_RG_TX_ARDQ_OE_EXT_DIS_B1);
    vIO32WriteFldAlign_All(DDRPHY_CA_CMD6, 0x0, CA_CMD6_RG_TX_ARCMD_OE_EXT_DIS);
#endif

#if ENABLE_WRITE_DBI
    EnableDRAMModeRegWriteDBIAfterCalibration(p);
#endif

#if ENABLE_READ_DBI
    EnableDRAMModeRegReadDBIAfterCalibration(p);
#endif

    // Set VRCG{MR13[3]} to 0 both to DRAM and DVFS
    SetMr13VrcgToNormalOperation(p);
    
    //DA mode
    vIO32WriteFldAlign_All(DDRPHY_B0_DQ6, 0x0, B0_DQ6_RG_RX_ARDQ_BIAS_PS_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DQ6, 0x0, B1_DQ6_RG_RX_ARDQ_BIAS_PS_B1);
    vIO32WriteFldAlign_All(DDRPHY_CA_CMD6, 0x0, CA_CMD6_RG_RX_ARCMD_BIAS_PS);

    vIO32WriteFldAlign_All(DDRPHY_B0_DQ6, 0x1, B0_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DQ6, 0x1, B1_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B1);
    vIO32WriteFldAlign_All(DDRPHY_CA_CMD6, 0x1, CA_CMD6_RG_RX_ARCMD_RPRE_TOG_EN);

/*================================
    IMPCAL Settings
==================================*/
    vIO32WriteFldMulti_All(DRAMC_REG_IMPCAL, P_Fld(0, IMPCAL_IMPCAL_IMPPDP) | P_Fld(0, IMPCAL_IMPCAL_IMPPDN));    //RG_RIMP_BIAS_EN and RG_RIMP_VREF_EN move to IMPPDP and IMPPDN
    vIO32WriteFldAlign_All(DDRPHY_MISC_IMP_CTRL0, 0, MISC_IMP_CTRL0_RG_IMP_EN);

/*================================
    MR1
==================================*/

    //MR1 op[7]=0 already be setted at end of gating calibration, no need to set here again
/*
    u1MR01Value[p->dram_fsp] &= 0x7f;
    DramcModeRegWrite(p, 1, u1MR01Value[p->dram_fsp]);
*/
    //Prevent M_CK OFF because of hardware auto-sync
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0, Fld(4,0,AC_MSKB0));

    //DFS- fix Gating Tracking settings
    vIO32WriteFldAlign_All(DDRPHY_MISC_CTRL0, 0, MISC_CTRL0_R_STBENCMP_DIV4CK_EN);
    vIO32WriteFldAlign_All(DDRPHY_MISC_CTRL1, 0, MISC_CTRL1_R_DMSTBENCMP_RK_OPT);

    ///TODO: Disable MR4 MR18/MR19, TxHWTracking, Dummy RD - for DFS workaround
    vIO32WriteFldAlign_All(DRAMC_REG_SPCMDCTRL, 0x1, SPCMDCTRL_REFRDIS);    //MR4 Disable
    //vIO32WriteFldAlign_All(DRAMC_REG_DQSOSCR, 0x1, DQSOSCR_DQSOSCRDIS);  //MR18, MR19 Disable
    //for(shu_index = DRAM_DFS_SHUFFLE_1; shu_index < DRAM_DFS_SHUFFLE_MAX; shu_index++)
	  //vIO32WriteFldAlign_All(DRAMC_REG_SHU_SCINTV + SHU_GRP_DRAMC_OFFSET*shu_index, 0x1, SHU_SCINTV_DQSOSCENDIS);
    //vIO32WriteFldAlign_All(DRAMC_REG_SHU_SCINTV, 0x1, SHU_SCINTV_DQSOSCENDIS);
    //vIO32WriteFldAlign_All(DRAMC_REG_SHU2_SCINTV, 0x1, SHU2_SCINTV_DQSOSCENDIS);
    //vIO32WriteFldAlign_All(DRAMC_REG_SHU3_SCINTV, 0x1, SHU3_SCINTV_DQSOSCENDIS);
    vIO32WriteFldMulti_All(DRAMC_REG_DUMMY_RD, P_Fld(0x0, DUMMY_RD_DUMMY_RD_EN)
                                            | P_Fld(0x0, DUMMY_RD_SREF_DMYRD_EN)
                                            | P_Fld(0x0, DUMMY_RD_DQSG_DMYRD_EN)
                                            | P_Fld(0x0, DUMMY_RD_DMY_RD_DBG));

#if APPLY_LP4_POWER_INIT_SEQUENCE
    //CKE dynamic
#if ENABLE_TMRRI_NEW_MODE
    CKEFixOnOff(p, CKE_WRITE_TO_ALL_RANK, CKE_DYNAMIC, CKE_WRITE_TO_ALL_CHANNEL);
#else
    CKEFixOnOff(p, RANK_0, CKE_DYNAMIC, CKE_WRITE_TO_ALL_CHANNEL);
#endif
    
    //// Enable  HW MIOCK control to make CLK dynamic
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 0, DRAMC_PD_CTRL_MIOCKCTRLOFF);
#endif

    //close eyescan to save power
    vIO32WriteFldMulti_All(DRAMC_REG_EYESCAN, P_Fld(0x0, EYESCAN_EYESCAN_DQS_SYNC_EN)
                                        | P_Fld(0x0, EYESCAN_EYESCAN_NEW_DQ_SYNC_EN)
                                        | P_Fld(0x0, EYESCAN_EYESCAN_DQ_SYNC_EN));

    /* TESTAGENT2 */
    vIO32WriteFldAlign_All(DRAMC_REG_TEST2_4, 4, TEST2_4_TESTAGENTRKSEL); // Rank selection is controlled by Test Agent
    vIO32WriteFldAlign_All(DRAMC_REG_TEST2_2, 0x20, TEST2_2_TEST2_OFF); //@Chris, MP setting for runtime TA2 Length
}


void vApplyConfigBeforeCalibration(DRAMC_CTX_T *p)
{
    U8 read_xrtw2w, shu_index;
    U8 u1RankIdx, u1RankIdxBak;
    u1RankIdxBak = u1GetRank(p);

    //Clk free run {Move to Init_DRAM() and only call once}
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
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_SCINTV, 0x1B, SHU_SCINTV_TZQLAT); //ZQ Calibration Time, unit: 38.46ns, tZQCAL min is 1 us. need to set larger than 0x1b
    //for(shu_index = DRAM_DFS_SHUFFLE_1; shu_index < DRAM_DFS_SHUFFLE_MAX; shu_index++)
        //vIO32WriteFldAlign_All(DRAMC_REG_SHU_CONF3 + SHU_GRP_DRAMC_OFFSET*shu_index, 0x1ff, SHU_CONF3_ZQCSCNT); //Every refresh number to issue ZQCS commands, only for DDR3/LPDDR2/LPDDR3/LPDDR4
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_CONF3, 0x1ff, SHU_CONF3_ZQCSCNT); //Every refresh number to issue ZQCS commands, only for DDR3/LPDDR2/LPDDR3/LPDDR4
    //vIO32WriteFldAlign_All(DRAMC_REG_SHU2_CONF3, 0x1ff, SHU_CONF3_ZQCSCNT); //Every refresh number to issue ZQCS commands, only for DDR3/LPDDR2/LPDDR3/LPDDR4
    //vIO32WriteFldAlign_All(DRAMC_REG_SHU3_CONF3, 0x1ff, SHU_CONF3_ZQCSCNT); //Every refresh number to issue ZQCS commands, only for DDR3/LPDDR2/LPDDR3/LPDDR4
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

        //Move to DCM off setting
        vIO32WriteFldMulti_All(DRAMC_REG_ZQCS, P_Fld(1, ZQCS_ZQCSDUAL) | \
                                               P_Fld(0, ZQCS_ZQCSMASK_OPT) | \
                                               P_Fld(0, ZQCS_ZQMASK_CGAR) | \
                                               P_Fld(0, ZQCS_ZQCS_MASK_SEL_CGAR));
        //vIO32WriteFldMulti_All(DRAMC_REG_ZQCS, P_Fld(1, ZQCS_ZQCSDUAL) | \
        //                                       P_Fld(0, ZQCS_ZQCSMASK_OPT) | \
        //                                       P_Fld(0, ZQCS_ZQMASK_CGAR));

        // DRAMC CHA(CHN0):ZQCSMASK=1, DRAMC CHB(CHN1):ZQCSMASK=0.
        // ZQCSMASK setting: (Ch A, Ch B) = (1,0) or (0,1)
        // if CHA.ZQCSMASK=1, and then set CHA.ZQCALDISB=1 first, else set CHB.ZQCALDISB=1 first
        vIO32WriteFldAlign(DRAMC_REG_ZQCS + (CHANNEL_A<< POS_BANK_NUM), 1, ZQCS_ZQCSMASK);
        vIO32WriteFldAlign(DRAMC_REG_ZQCS + SHIFT_TO_CHB_ADDR, 0, ZQCS_ZQCSMASK);

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
    //for(shu_index = DRAM_DFS_SHUFFLE_1; shu_index < DRAM_DFS_SHUFFLE_MAX; shu_index++)
        //vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SHU_SCINTV) + SHU_GRP_DRAMC_OFFSET*shu_index, 0x1, SHU_SCINTV_DQSOSCENDIS);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SHU_SCINTV), 0x1, SHU_SCINTV_DQSOSCENDIS);
    //vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SHU2_SCINTV), 0x1, SHU2_SCINTV_DQSOSCENDIS);
    //vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SHU3_SCINTV), 0x1, SHU3_SCINTV_DQSOSCENDIS);
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

    // set MRSRK to 0, MPCRKEN always set 1 (Derping)
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_MRS), 0, MRS_MRSRK);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_MPC_OPTION), 1, MPC_OPTION_MPCRKEN);

    //RG mode
    vIO32WriteFldAlign_All(DDRPHY_B0_DQ6, 0x1, B0_DQ6_RG_RX_ARDQ_BIAS_PS_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DQ6, 0x1, B1_DQ6_RG_RX_ARDQ_BIAS_PS_B1);
    vIO32WriteFldAlign_All(DDRPHY_CA_CMD6, 0x1, CA_CMD6_RG_RX_ARCMD_BIAS_PS);

#if ENABLE_RX_TRACKING_LP4
    DramcRxInputDelayTrackingInit_byFreq(p);
#endif

#ifdef LOOPBACK_TEST
#ifdef LPBK_INTERNAL_EN
    DramcLoopbackTest_settings(p, 0);   //0: internal loopback test 1: external loopback test
#else
    DramcLoopbackTest_settings(p, 1);   //0: internal loopback test 1: external loopback test
#endif
#endif

#if ENABLE_TMRRI_NEW_MODE
    SetCKE2RankIndependent(p);
#endif

#ifdef DUMMY_READ_FOR_TRACKING
    vIO32WriteFldAlign_All(DRAMC_REG_DUMMY_RD, 1, DUMMY_RD_DMY_RD_RX_TRACK);
#endif

    vIO32WriteFldAlign_All(DRAMC_REG_DRSCTRL, 1, DRSCTRL_DRSDIS);

#ifdef IMPEDANCE_TRACKING_ENABLE
    // set correct setting to control IMPCAL HW Tracking in shuffle RG
    // if p->freq >= 1333, enable IMP HW tracking(SHU_DRVING1_DIS_IMPCAL_HW=0), else SHU_DRVING1_DIS_IMPCAL_HW = 1
    U8 u1DisImpHw;

    u1DisImpHw = (p->frequency >= 1333)?0:1;
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_DRVING1, u1DisImpHw, SHU_DRVING1_DIS_IMPCAL_HW);
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_DRVING1, u1DisImpHw, SHU_DRVING1_DIS_IMP_ODTN_TRACK);
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_DRVING2, u1DisImpHw, SHU_DRVING2_DIS_IMPCAL_ODT_EN);
#endif

    /* SRAM SHU DMA Write data from 2T to 1T*/
    vIO32WriteFldAlign_All(DDRPHY_SRAM_DMA0, 0x1, SRAM_DMA0_R_PENABLE_LAT_WR); // @Joe, MP setting

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION && RX_DELAY_PRE_CAL
    s2RxDelayPreCal =PASS_RANGE_NA; // reset variable for fast k test
#endif
}



/* vDramcInit_PreSettings(): Initial register settings(which are required to be set before all calibration flow) */
void vDramcInit_PreSettings(DRAMC_CTX_T *p)
{
    /* PAD_RRESETB control sequence */
    //remove twice dram reset pin pulse before dram power on sequence flow
    vIO32WriteFldMulti(DDRPHY_CA_CMD8, P_Fld(0x0, CA_CMD8_RG_TX_RRESETB_PULL_UP) | P_Fld(0x0, CA_CMD8_RG_TX_RRESETB_PULL_DN)
                                     | P_Fld(0x1, CA_CMD8_RG_TX_RRESETB_DDR3_SEL) | P_Fld(0x0, CA_CMD8_RG_TX_RRESETB_DDR4_SEL)
                                     | P_Fld(0xa, CA_CMD8_RG_RRESETB_DRVP) | P_Fld(0xa, CA_CMD8_RG_RRESETB_DRVN));
    vIO32WriteFldAlign(DDRPHY_MISC_CTRL1, 0x1, MISC_CTRL1_R_DMRRESETB_I_OPT); //Change to glitch-free path
    //replace DDRCONF0_GDDR3RST with MISC_CTRL1_R_DMDA_RRESETB_I
    vIO32WriteFldAlign(DDRPHY_MISC_CTRL1, 0x0, MISC_CTRL1_R_DMDA_RRESETB_I);
    vIO32WriteFldAlign(DDRPHY_MISC_CTRL1, 0x1, MISC_CTRL1_R_DMDA_RRESETB_E);

#if __ETT__
    if (GetDramcBroadcast() == DRAMC_BROADCAST_OFF)
    {   // In this function, broadcast is assumed to be ON(LP4) -> Show error if broadcast is OFF
        mcSHOW_ERR_MSG(("Err! Broadcast is OFF!\n"));
    }
#endif
    return;
}

void DramcInit_DutyCalibration(DRAMC_CTX_T *p)
{
#if ENABLE_DUTY_SCAN_V2
#ifdef DDR_INIT_TIME_PROFILING
    U32 u4low_tick0, u4high_tick0, u4low_tick1, u4high_tick1;
#if __ETT__
    u4low_tick0 = GPT_GetTickCount(&u4high_tick0);
#else
    u4low_tick0 = get_timer(0);
#endif
#endif

#ifndef DUMP_INIT_RG_LOG_TO_DE
    if (Get_MDL_Used_Flag()==NORMAL_USED)
    {
        DramcNewDutyCalibration(p);
    }
#endif

#ifdef DDR_INIT_TIME_PROFILING
#if __ETT__
    u4low_tick1 = GPT_GetTickCount(&u4high_tick1);
    gu4DutyCalibrationTime = ((u4low_tick1-u4low_tick0)*76)/1000000;
#else
    u4low_tick1 = get_timer(u4low_tick0);
    gu4DutyCalibrationTime = u4low_tick1;
#endif
#endif
#endif
}

DRAM_STATUS_T DramcInit(DRAMC_CTX_T *p)
{
    U32 save_ch, dram_t; //Darren
    #if (!__ETT__ && !FOR_DV_SIMULATION_USED && SW_CHANGE_FOR_SIMULATION==0)
    EMI_SETTINGS *emi_set; //Darren
    #endif
    U8 dram_cbt_mode;

#ifdef FOR_HQA_REPORT_USED        
    if (gHQALog_flag==1)
    {
        mcSHOW_DBG_MSG(("[HQA] Log parsing, "));
        mcSHOW_DBG_MSG(("\tDram Data rate = ")); HQA_LOG_Print_Freq_String(p); mcSHOW_DBG_MSG(("\n"));
    }        
#endif    

    mcSHOW_DBG_MSG(("\n[DramcInit]\n"));
    mcFPRINTF((fp_A60501, "\n[DramcInit] Begin\n"));

    vSetPHY2ChannelMapping(p, CHANNEL_A);

#if WRITE_LEVELING_MOVE_DQS_INSTEAD_OF_CLK
    memset(p->arfgWriteLevelingInitShif, FALSE, sizeof(p->arfgWriteLevelingInitShif));
#endif

    //default set DRAM status = NO K
    memset(p->aru4CalResultFlag, 0xffff, sizeof(p->aru4CalResultFlag));
    memset(p->aru4CalExecuteFlag, 0, sizeof(p->aru4CalExecuteFlag));

    DramcSetting_Olympus_LP4_ByteMode(p);

    DramcInit_DutyCalibration(p);

    DramcModeRegInit_LP4(p);

    DdrUpdateACTiming(p);

#if 0  //update refresh rate
    // for free-run clk 26MHz, 0x62 * (1/26) = 3.8ns
    vIO32WriteFldAlign_All(DRAMC_REG_DRAMC_PD_CTRL, 0x62, DRAMC_PD_CTRL_REFCNT_FR_CLK);
    // for non-fre-run clk,  reg = 3.8 ns * f / 4 / 16;
    u4RefreshRate = 38*p->frequency/640;
    vIO32WriteFldAlign_All(DRAMC_REG_CONF2, u4RefreshRate, CONF2_REFCNT);
#endif

#if (fcFOR_CHIP_ID == fcKrug)
    // For kernel api for check LPDDR3/4/4X (Darren), only for fcOlympus and fcElbrus.
    // For Other chip, please confirm the register is free for SW use.
    save_ch = vGetPHY2ChannelMapping(p);
    vSetPHY2ChannelMapping(p, CHANNEL_A);

    switch (p->dram_type)
    {
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


void DramcDebugTest(DRAMC_CTX_T *p)
{
#if 0
    U32 u4ErrorValue;

    // TestEngine2 R/W test
    p->test2_1 = DEFAULT_TEST2_1_CAL;
    u4ErrorValue = TestEngineCompare(p);
    mcSHOW_ERR_MSG(("[DramcDebugTest] First R/W test. Rank0: u4ErrorValue 0x%X\n", u4ErrorValue));
    if (p->support_rank_num==RANK_DUAL)
    {
        p->test2_1 = 0x55a00000;
        u4ErrorValue = TestEngineCompare(p);
        mcSHOW_ERR_MSG(("[DramcDebugTest] First R/W test. Rank1: u4ErrorValue 0x%X\n", u4ErrorValue));
    }

    // Reset PHY
    DramPhyReset(p);

    // TestEngine2 R/W test
    p->test2_1 = DEFAULT_TEST2_1_CAL;
    u4ErrorValue = TestEngineCompare(p);
    mcSHOW_ERR_MSG(("[DramcDebugTest] Reset PHY R/W test. Rank0: u4ErrorValue 0x%X\n", u4ErrorValue));

    if (p->support_rank_num==RANK_DUAL)
    {
        p->test2_1 = 0x55a00000;
        u4ErrorValue = TestEngineCompare(p);
        mcSHOW_ERR_MSG(("[DramcDebugTest] Reset PHY R/W test . Rank1:u4ErrorValue 0x%X\n", u4ErrorValue));
    }

    // Reset Dram
    Dram_Reset(p);

    // TestEngine2 R/W test
    p->test2_1 = DEFAULT_TEST2_1_CAL;
    u4ErrorValue = TestEngineCompare(p);
    mcSHOW_ERR_MSG(("[DramcDebugTest] Reset Dram R/W test. Rank0: u4ErrorValue 0x%X\n", u4ErrorValue));

    if (p->support_rank_num==RANK_DUAL)
    {
        p->test2_1 = 0x55a00000;
        u4ErrorValue = TestEngineCompare(p);
        mcSHOW_ERR_MSG(("[DramcDebugTest] Reset Dram R/W test . Rank1:u4ErrorValue 0x%X\n", u4ErrorValue));
    }

    p->test2_1 = DEFAULT_TEST2_1_CAL;
    #endif
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



#if ENABLE_TMRRI_NEW_MODE
void SetCKE2RankIndependent(DRAMC_CTX_T *p)
{
    #if ENABLE_TMRRI_NEW_MODE//Newly added CKE control mode API
	mcSHOW_DBG_MSG(("SET_CKE_2_RANK_INDEPENDENT_RUN_TIME: ON\n"));
    vCKERankCtrl(p, CKE_RANK_INDEPENDENT);
    #else //Legacy individual CKE control register settings
	mcSHOW_DBG_MSG(("SET_CKE_2_RANK_INDEPENDENT_RUN_TIME: OFF\n"));
	vCKERankCtrl(p, CKE_RANK_DEPENDENT);
    #endif
}
#endif


#if ENABLE_WRITE_DBI
void EnableDRAMModeRegWriteDBIAfterCalibration(DRAMC_CTX_T *p)
{
    U8 channel_idx, rank_idx;
    U8 ch_backup, rank_backup, u1FSPIdx = 0;
    S8 u1ShuffleIdx;

    ch_backup = p->channel;
    rank_backup = p->rank;

    for (channel_idx = CHANNEL_A; channel_idx < p->support_channel_num; channel_idx++)
    {
        vSetPHY2ChannelMapping(p, channel_idx);
        for (rank_idx = RANK_0; rank_idx < RANK_MAX; rank_idx++)
        {
            vSetRank(p, rank_idx); 
            for(u1FSPIdx=FSP_0; u1FSPIdx<FSP_MAX; u1FSPIdx++)
            {
                //mcSHOW_DBG_MSG(("[EnableDRAMModeRegWriteDBIAfterCalibration] FSP_%d, MR13 = 0x%x\n", u1FSPIdx, u1MR13Value[u1FSPIdx]));
                if (u1FSPIdx)
                {
                    DramcMRWriteFldAlign(p, 13, 1, MR13_FSP_WR, TO_MR);
                }
                else
                {
                    DramcMRWriteFldAlign(p, 13, 0, MR13_FSP_WR, TO_MR);
                }


                SetDramModeRegForWriteDBIOnOff(p, u1FSPIdx, p->DBI_W_onoff[u1FSPIdx]);
            }
        }
    }
	
    vSetRank(p, rank_backup); 
    vSetPHY2ChannelMapping(p, ch_backup);
}
#endif

#if ENABLE_READ_DBI
void EnableDRAMModeRegReadDBIAfterCalibration(DRAMC_CTX_T *p)
{
    U8 channel_idx, rank_idx;
    U8 ch_backup, rank_backup, u1FSPIdx = 0;
    S8 u1ShuffleIdx;

    ch_backup = p->channel;
    rank_backup = p->rank;

    for (channel_idx = CHANNEL_A; channel_idx < p->support_channel_num; channel_idx++)
    {
        vSetPHY2ChannelMapping(p, channel_idx);
        for (rank_idx = RANK_0; rank_idx < RANK_MAX; rank_idx++)
        {
            vSetRank(p, rank_idx); 
            for(u1FSPIdx=FSP_0; u1FSPIdx<FSP_MAX; u1FSPIdx++)
            {
                //mcSHOW_DBG_MSG(("[EnableDRAMModeRegReadDBIAfterCalibration] FSP_%d, MR13 = 0x%x\n", u1FSPIdx, u1MR13Value[u1FSPIdx]));
                if (u1FSPIdx)
                {
                    DramcMRWriteFldAlign(p, 13, 1, MR13_FSP_WR, TO_MR);
                }
                else
                {
                    DramcMRWriteFldAlign(p, 13, 0, MR13_FSP_WR, TO_MR);
                }

                SetDramModeRegForReadDBIOnOff(p, u1FSPIdx, p->DBI_R_onoff[u1FSPIdx]);
            }
        }
    }

    //[Ei_ger] DVT item RD2MRR & MRR2RD
    vIO32WriteFldMulti_All(DRAMC_REG_HW_MRR_FUN, P_Fld(0x1, HW_MRR_FUN_TR2MRR_ENA)
                                           | P_Fld(0x1, HW_MRR_FUN_R2MRRHPRICTL)
                                           | P_Fld(0x1, HW_MRR_FUN_MANTMRR_EN));

    vSetRank(p, rank_backup); 
    vSetPHY2ChannelMapping(p, ch_backup);
}
#endif


void SetMr13VrcgToNormalOperationShuffle(DRAMC_CTX_T *p)//Only set DRAM_DFS_SHUFFLE_1
{
    DRAM_CHANNEL_T eOriChannel = vGetPHY2ChannelMapping(p);
    U8 u1ChIdx = CHANNEL_A;
    U32 u4Value = 0;

    for(u1ChIdx = CHANNEL_A; u1ChIdx < p->support_channel_num; u1ChIdx++)
    {
        vSetPHY2ChannelMapping(p, u1ChIdx);  
        //DVFS MRW
        u4Value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_VRCG), SHU_HWSET_VRCG_HWSET_VRCG_OP);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_VRCG), u4Value & ~(0x1<<3), SHU_HWSET_VRCG_HWSET_VRCG_OP);
    }
    vSetPHY2ChannelMapping(p, (U8)eOriChannel);  
    return;
}

void SetMr13VrcgToNormalOperation(DRAMC_CTX_T *p)
{
    DRAM_CHANNEL_T eOriChannel = vGetPHY2ChannelMapping(p);
    U8 u1ChIdx = CHANNEL_A;
    U8 u1RankIdx = 0;
    U8 u1MRFsp = (p->frequency < MRFSP_TERM_FREQ) ? FSP_0 : FSP_1;
#if MRW_CHECK_ONLY
    mcSHOW_MRW_MSG(("\n==[MR Dump] %s==\n", __func__));
#endif

    DramcMRWriteFldAlign(p, 13, 0, MR13_VRCG, JUST_TO_GLOBAL_VALUE);

    for(u1ChIdx = CHANNEL_A; u1ChIdx < p->support_channel_num; u1ChIdx++)
    {
        vSetPHY2ChannelMapping(p, u1ChIdx);  
        //To DRAM: MR13[3] = 0
        for(u1RankIdx = 0; u1RankIdx < p->support_rank_num; u1RankIdx++)
        {
            DramcModeRegWriteByRank(p, u1RankIdx, 13, u1MR13Value);
        }
    }
    vSetPHY2ChannelMapping(p, (U8)eOriChannel);  
    return;
}

void DramcShuTrackingDcmEnBySRAM(DRAMC_CTX_T *p)
{
    DRAM_DFS_FREQUENCY_TABLE_T *pFreqTable = p->pDFSTable; // from dramc conf shu0
    U8 u1ShuffleIdx;
    U32 u4DramcShuOffset = 0;
    U32 u4DDRPhyShuOffset = 0;
    U16 u2Freq = 0;
    U32 u4RegBackupAddress[] = 
    {       
        (DDRPHY_SRAM_DMA0),
        (DDRPHY_SRAM_DMA0 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_SRAM_DMA2),
        (DDRPHY_SRAM_DMA2 + SHIFT_TO_CHB_ADDR),
    };

    //Backup regs
    DramcBackupRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));
	
    #if(ENABLE_TX_TRACKING && TX_RETRY_ENABLE)
    Enable_and_Trigger_TX_Retry(p);
    #endif
	
    //Aceess DMA SRAM by APB bus use debug mode by conf SHU3
    vIO32WriteFldAlign_All(DDRPHY_SRAM_DMA0, 0x1, SRAM_DMA0_R_APB_SLV_SEL);
    vIO32WriteFldAlign_All(DDRPHY_SRAM_DMA2, 0x1, SRAM_DMA2_R_APB_DMA_DBG_ACCESS);

    for(u1ShuffleIdx = 0; u1ShuffleIdx<=DRAM_DFS_SRAM_MAX; u1ShuffleIdx++) //fill SHU1 of conf while (u1ShuffleIdx==DRAM_DFS_SRAM_MAX)
    {
        if(u1ShuffleIdx == DRAM_DFS_SRAM_MAX)
        {
            //for SHU0 restore to SRAM
            vSetDFSTable(p, pFreqTable);//Restore DFS table
            u4DramcShuOffset = 0;
            u4DDRPhyShuOffset = 0;
            //Restore regs, or SHU0 RG cannot be set
            DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));
        }
        else
        {
            //Aceess DMA SRAM by APB bus use debug mode by conf SHU3
            vSetDFSTable(p, get_FreqTbl_by_shuffleIndex(p, u1ShuffleIdx));//Update DFS table
            u4DramcShuOffset = SHU_GRP_DRAMC_OFFSET*3; //SHU3 
            u4DDRPhyShuOffset = SHU_GRP_DDRPHY_OFFSET*3; //SHU3
            vIO32WriteFldAlign_All(DDRPHY_SRAM_DMA2, u1ShuffleIdx, SRAM_DMA2_R_APB_DMA_DBG_LEVEL);
        }

        u2Freq = GetFreqBySel(p, p->pDFSTable->freq_sel);

        // add your function
        // For example: EnableDramcPhyDCMShuffle(p, enable_dcm, u4DramcShuOffset, u4DDRPhyShuOffset, u1ShuffleIdx);
        #if ENABLE_TX_TRACKING		 
            Enable_TX_Tracking(p,u4DramcShuOffset,u2Freq);
        #endif
        #if RDSEL_TRACKING_EN
            Enable_RDSEL_Tracking(p,u4DramcShuOffset,u2Freq);
        #endif
        #if (SW_CHANGE_FOR_SIMULATION == 0)
        #if APPLY_LOWPOWER_GOLDEN_SETTINGS
            int enable_dcm = (doe_get_config("dramc_dcm")) ? 0 : 1;
            EnableDramcPhyDCMShuffle(p, enable_dcm, u4DramcShuOffset, u4DDRPhyShuOffset);
        #else
            EnableDramcPhyDCMShuffle(p, 0, u4DramcShuOffset, u4DDRPhyShuOffset);
        #endif
        #endif        
    }
}

void DramcRunTimeConfig(DRAMC_CTX_T *p)
{
    mcSHOW_DBG_MSG(("[DramcRunTimeConfig]\n"));
#if ENABLE_DDR800_OPEN_LOOP_MODE_OPTION
    if(vGet_DDR800_Mode(p) == DDR800_SEMI_LOOP)
    {
        EnableDllCg(p, ENABLE); //open CG to save power       
    }
#endif

    //Auto refresh related
    vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL1, 0x5, REFCTRL1_MPENDREF_CNT); //Refresh pend threshold; Align MP setting PIC:Unique

    vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL0, 0x0, REFCTRL0_REFDIS); //After k, auto refresh should be enable

#if REFRESH_OVERHEAD_REDUCTION
    vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL1, 0x1, REFCTRL1_REF_OVERHEAD_SLOW_REFPB_ENA); 
    mcSHOW_DBG_MSG(("REFRESH_OVERHEAD_REDUCTION: ON\n"));
#else
    mcSHOW_DBG_MSG(("REFRESH_OVERHEAD_REDUCTION: OFF\n"));
#endif

#if ENABLE_PER_BANK_REFRESH
    vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL0, 1, REFCTRL0_PBREFEN);
    mcSHOW_DBG_MSG(("PER_BANK_REFRESH: ON\n"));
#else
    vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL0, 0, REFCTRL0_PBREFEN);
    mcSHOW_DBG_MSG(("PER_BANK_REFRESH: OFF\n"));
#endif

#if CMD_PICG_NEW_MODE    
    mcSHOW_DBG_MSG(("CMD_PICG_NEW_MODE: ON\n"));
#else
    mcSHOW_DBG_MSG(("CMD_PICG_NEW_MODE: OFF\n"));
#endif

#if XRTWTW_NEW_CROSS_RK_MODE
    if(p->support_rank_num == RANK_DUAL)
    {
        ENABLE_XRTWTW_Setting(p);
        mcSHOW_DBG_MSG(("XRTWTW_NEW_MODE: ON\n"));
    }
#else
    mcSHOW_DBG_MSG(("XRTWTW_NEW_MODE: OFF\n"));
#endif

#if XRTRTR_NEW_CROSS_RK_MODE
    if(p->support_rank_num == RANK_DUAL)
    {
	    ENABLE_XRTRTR_Setting(p);
	    mcSHOW_DBG_MSG(("XRTRTR_NEW_MODE: ON\n"));
    }
#else
    mcSHOW_DBG_MSG(("XRTRTR_NEW_MODE: OFF\n"));
#endif

#if ENABLE_TX_TRACKING
    mcSHOW_DBG_MSG(("TX_TRACKING: ON\n"));
    #if __ETT__
    CheckTxTrackingHwLimit(p);
    #endif
#else
    mcSHOW_DBG_MSG(("TX_TRACKING: OFF\n"));
#endif
#if RDSEL_TRACKING_EN
    mcSHOW_DBG_MSG(("RDSEL_TRACKING: ON\n"));
#else
    mcSHOW_DBG_MSG(("RDSEL_TRACKING: OFF\n"));
#endif

#if TDQSCK_PRECALCULATION_FOR_DVFS
    mcSHOW_DBG_MSG(("DQS Precalculation for DVFS: "));
    /* Maoauo: Enable DQS precalculation for LP4, disable for LP3(same as Kibo) */
    DramcDQSPrecalculation_enable(p);
    mcSHOW_DBG_MSG(("ON\n"));
#else
    mcSHOW_DBG_MSG(("DQS Precalculation for DVFS: OFF\n"));
#endif

#if ENABLE_RX_TRACKING_LP4
    DramcRxInputDelayTrackingInit_Common(p);
    DramcRxInputDelayTrackingHW(p);
    mcSHOW_DBG_MSG(("RX_TRACKING: ON\n"));
#else
    mcSHOW_DBG_MSG(("RX_TRACKING: OFF\n"));
#endif

#if (ENABLE_RX_TRACKING_LP4 && RX_DLY_TRACK_ONLY_FOR_DEBUG && defined(DUMMY_READ_FOR_TRACKING))
    mcSHOW_DBG_MSG(("RX_DLY_TRACK_DBG: ON\n"));
    DramcRxDlyTrackDebug(p);
#endif

#ifdef HW_GATING
    if (p->frequency != 400)
    {	 
        DramcHWGatingInit(p);     // HW gating initial before RunTime config.
        DramcHWGatingOnOff(p, 1); // Enable HW gating tracking
        mcSHOW_DBG_MSG(("HW_GATING: ON\n"));
    }
    else
    {
        mcSHOW_DBG_MSG(("HW_GATING: OFF\n"));
    }    
#else
    mcSHOW_DBG_MSG(("HW_GATING: OFF\n"));
#endif //HW_GATING

/* HW gating - Disabled by default(in preloader) to save power (DE: HJ Huang) */
#if (defined(HW_GATING) && GATING_ONLY_FOR_DEBUG)
    mcSHOW_DBG_MSG(("HW_GATING DBG: ON\n"));
    DramcHWGatingDebugOnOff(p, ENABLE);
#else
    mcSHOW_DBG_MSG(("HW_GATING DBG: OFF\n"));
    DramcHWGatingDebugOnOff(p, DISABLE);
#endif

#ifdef DUMMY_READ_FOR_TRACKING
    DramcDummyReadForTrackingEnable(p);
#else
    mcSHOW_DBG_MSG(("DUMMY_READ_FOR_TRACKING: OFF\n"));
#endif

#ifdef ZQCS_ENABLE_LP4
    // ZQCSMASK setting: (Ch A, Ch B) = (1,0) or (0,1)
    // if CHA.ZQCSMASK=1, and then set CHA.ZQCALDISB=1 first, else set CHB.ZQCALDISB=1 first
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), 1, SPCMDCTRL_ZQCALDISB);// LP3 and LP4 are different, be careful.
    mcSHOW_DBG_MSG(("ZQCS_ENABLE_LP4: ON\n"));
#else
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), 0, SPCMDCTRL_ZQCALDISB);// LP3 and LP4 are different, be careful.
    mcSHOW_DBG_MSG(("ZQCS_ENABLE_LP4: OFF\n"));
#endif

#ifdef DUMMY_READ_FOR_DQS_GATING_RETRY
    DummyReadForDqsGatingRetryNonShuffle(p, 1);
    mcSHOW_DBG_MSG(("DUMMY_READ_FOR_DQS_GATING_RETRY: ON\n"));
#else
    DummyReadForDqsGatingRetryNonShuffle(p, 0);
    mcSHOW_DBG_MSG(("DUMMY_READ_FOR_DQS_GATING_RETRY: OFF\n"));
#endif

#if RX_PICG_NEW_MODE    
    mcSHOW_DBG_MSG(("RX_PICG_NEW_MODE: ON\n"));
#else
    mcSHOW_DBG_MSG(("RX_PICG_NEW_MODE: OFF\n"));
#endif

#if TX_PICG_NEW_MODE    
    TXPICGNewModeEnable(p);    
    mcSHOW_DBG_MSG(("TX_PICG_NEW_MODE: ON\n"));
#else
    mcSHOW_DBG_MSG(("TX_PICG_NEW_MODE: OFF\n"));
#endif

#if (SW_CHANGE_FOR_SIMULATION == 0)
#if APPLY_LOWPOWER_GOLDEN_SETTINGS
    int enable_dcm = (doe_get_config("dramc_dcm")) ? 0 : 1;
    char *str = (enable_dcm == 1) ? ("ON") : ("OFF");
    EnableDramcPhyDCMNonShuffle(p, enable_dcm);
    mcSHOW_DBG_MSG(("LOWPOWER_GOLDEN_SETTINGS(DCM): %s\n", str));
#else
    EnableDramcPhyDCMNonShuffle(p, 0);
    mcSHOW_DBG_MSG(("LOWPOWER_GOLDEN_SETTINGS(DCM): OFF\n"));
#endif
#endif

    DramcShuTrackingDcmEnBySRAM(p);

#ifdef SPM_CONTROL_AFTERK
    TransferToSPMControl(p);
    mcSHOW_DBG_MSG(("SPM_CONTROL_AFTERK: ON\n"));
#else
    mcSHOW_DBG_MSG(("!!! SPM_CONTROL_AFTERK: OFF\n"));
    mcSHOW_DBG_MSG(("!!! SPM could not control APHY\n"));
#endif

// when time profiling multi times, SW impedance tracking will fail when trakcing enable.
// ignor SW impedance tracking when doing time profling
#ifndef DDR_INIT_TIME_PROFILING  
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
#endif

    //0x1c0[31]
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQSCAL0), 0, DQSCAL0_STBCALEN);

#ifdef TEMP_SENSOR_ENABLE
    //Enable HW-MRR4 : SPCMDCTRL_REFRDIS=0
    vIO32WriteFldAlign_All(DRAMC_REG_SPCMDCTRL, 0, SPCMDCTRL_REFRDIS);

    //HW-MRR4 don't block normal EMI request : SPCMDCTRL_REFR_BLOCKEN=0
    vIO32WriteFldAlign_All(DRAMC_REG_SPCMDCTRL, 0, SPCMDCTRL_REFR_BLOCKEN);

    //DRAMC will consider tMRR ac-timing : HW_MRR_FUN_TMRR_ENA =1
    vIO32WriteFldAlign_All(DRAMC_REG_HW_MRR_FUN, 1, HW_MRR_FUN_TMRR_ENA);

    //Set HW-MRR4 command in queue to high priority : R_DMMRR_HW_HIPRI = 1
    vIO32WriteFldAlign_All(DRAMC_REG_HW_MRR_FUN, 1, HW_MRR_FUN_MRR_HW_HIPRI);

    mcSHOW_DBG_MSG(("TEMP_SENSOR: ON\n"));
#else
    vIO32WriteFldAlign_All(DRAMC_REG_SPCMDCTRL, 1, SPCMDCTRL_REFRDIS);
    mcSHOW_DBG_MSG(("TEMP_SENSOR: OFF\n"));
#endif

#ifdef HW_SAVE_FOR_SR
    HwSaveForSR(p);
    mcSHOW_DBG_MSG(("HW_SAVE_FOR_SR: ON\n"));
#else
    mcSHOW_DBG_MSG(("HW_SAVE_FOR_SR: OFF\n"));
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

#if (ENABLE_DRS)
    if (p->support_rank_num==RANK_DUAL)
    {
        DramcDRS(p, 1);
    }
    else
    {
        DramcDRS(p, 0);
    }
#else
    DramcDRS(p, 0);
#endif

#if CHECK_GOLDEN_SETTING
    char *sCheckStr = (CheckGoldenSetting(p) == DRAM_OK) ? ("OK") : ("NG");
    mcSHOW_DBG_MSG(("Golden setting check: %s\n", sCheckStr));
#endif

#if ENABLE_DVFS_BYPASS_MR13_FSP
    DFSBypassMR13HwSet(p);
#endif

#if ENABLE_MCK8X_MODE
    EnableMck8xEnShu(p);
    mcSHOW_DBG_MSG(("ENABLE_MCK8X_MODE: ON\n"));
#endif
    mcSHOW_DBG_MSG(("=========================\n"));

    //ETT_DRM(p);
}


void DramcTest_DualSch_stress(DRAMC_CTX_T *p)
{
    U32 count = 0;
    U16 u2Value = 0;
    
#if MRW_CHECK_ONLY
    mcSHOW_MRW_MSG(("\n==[MR Dump] %s==\n", __func__));
#endif

    //vIO32WriteFldAlign_All(DRAMC_REG_PERFCTL0, 1, PERFCTL0_DUALSCHEN);
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_CONF0, 1, SHU_CONF0_DUALSCHEN);

    while(count<10)
    {
        count++;

        u1MR12Value[p->channel][p->rank][p->dram_fsp] = 0x14;
        DramcModeRegWriteByRank(p, p->rank, 12, u1MR12Value[p->channel][p->rank][p->dram_fsp]);
        DramcModeRegReadByRank(p, p->rank, 12, &u2Value);
        //mcSHOW_DBG_MSG(("MR12 = 0x%0X\n", u1Value));
    }
}

#if (ENABLE_TX_TRACKING && TX_RETRY_ENABLE)
void SPMTx_Track_Retry_OnOff(DRAMC_CTX_T *p, U8 shu_level, U8 onoff)
{
    static U8 gIsddr800TxRetry = 0;

    // MCK still available for DRAMC RG access from Joe comment
    if (shu_level == SRAM_SHU9)
    {
        gIsddr800TxRetry = 1;
    }

    if((gIsddr800TxRetry == 1) && (shu_level != SRAM_SHU9)) //Need to do tx retry when DDR800 -> DDr1200
    {
        if(onoff==ENABLE)
        {
            mcSHOW_DBG_MSG(("TX track retry: ENABLE! (DDR800 to DDR1200)\n"));
            vIO32WriteFldAlign_All(DRAMC_REG_PPR_CTRL, 1,  PPR_CTRL_XSR_TX_RETRY_BLOCK_ALE_MASK);
            mcDELAY_US(1); 
            #if TX_RETRY_CONTROL_BY_SPM
            vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1,  SPM_POWER_ON_VAL0_SC_TX_TRACK_RETRY_EN);
            #else //control by DRAMC
            vIO32WriteFldAlign_All(DRAMC_REG_PPR_CTRL, 1,  PPR_CTRL_XSR_TX_RETRY_EN);
            #endif
        }
        else //DISABLE
        {
            mcSHOW_DBG_MSG(("TX track retry: DISABLE! (DDR800 to DDR1200)\n"));
            #if TX_RETRY_CONTROL_BY_SPM
            vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0,  SPM_POWER_ON_VAL0_SC_TX_TRACK_RETRY_EN);
            #else //control by DRAMC
            vIO32WriteFldAlign_All(DRAMC_REG_PPR_CTRL, 0,  PPR_CTRL_XSR_TX_RETRY_EN);
            #endif
            mcDELAY_US(1); //add 1us delay to wait emi and tx retry be done (because PPR_CTRL_TX_RETRY_SHU_RESP_OPT=1)
            vIO32WriteFldAlign_All(DRAMC_REG_PPR_CTRL, 0,  PPR_CTRL_XSR_TX_RETRY_BLOCK_ALE_MASK); //enable block emi to let tx retry be finish
            gIsddr800TxRetry = 0;
        }
    }
}
#endif
#if SW_TX_RETRY_ENABLE
void SWTx_Track_Retry_OnOff(DRAMC_CTX_T *p)
{
    U8 u4Response;

    mcSHOW_DBG_MSG(("SW TX track retry!\n"));
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PPR_CTRL), 0,  PPR_CTRL_XSR_TX_RETRY_SW_EN);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PPR_CTRL), 1,  PPR_CTRL_XSR_TX_RETRY_BLOCK_ALE_MASK);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PPR_CTRL), 1,  PPR_CTRL_XSR_TX_RETRY_SW_EN);
    do
    {
        u4Response = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_TX_RETRY_DONE_RESPONSE);
        mcDELAY_US(1);  // Wait tZQCAL(min) 1us or wait next polling
        mcSHOW_DBG_MSG3(("still wait tx retry be done\n", u4Response));
    }while(u4Response==0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PPR_CTRL), 0,  PPR_CTRL_XSR_TX_RETRY_SW_EN);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PPR_CTRL), 0,  PPR_CTRL_XSR_TX_RETRY_BLOCK_ALE_MASK);
}
#endif

#if (TDQSCK_PRECALCULATION_FOR_DVFS || defined(HW_GATING))
void DramcDQSPrecalculation_TrackingOff(DRAMC_CTX_T *p, U8 shu_level)
{
    // MCK still available for DRAMC RG access from Joe comment
    if (shu_level == SRAM_SHU9)
    {
    #ifdef HW_GATING
        vIO32WriteFldAlign_All(DRAMC_REG_STBCAL1, 1, STBCAL1_STBCNT_SW_RST);
    #endif

    #if TDQSCK_PRECALCULATION_FOR_DVFS
        vIO32WriteFldAlign_All(DRAMC_REG_PRE_TDQSCK1, 0, PRE_TDQSCK1_TDQSCK_HW_SW_UP_SEL);
    #endif
        gIsddr800DisGating = 1;
    }
}

void DramcDQSPrecalculation_TrackingOn(DRAMC_CTX_T *p, U8 shu_level)
{
    // MCK still available for DRAMC RG access from Joe comment
    if ((shu_level != SRAM_SHU9) && (gIsddr800DisGating == 1))
    {
    #ifdef HW_GATING
        vIO32WriteFldAlign_All(DRAMC_REG_STBCAL1, 0, STBCAL1_STBCNT_SW_RST);
    #endif

    #if TDQSCK_PRECALCULATION_FOR_DVFS
        vIO32WriteFldAlign_All(DRAMC_REG_PRE_TDQSCK1, 1, PRE_TDQSCK1_TDQSCK_HW_SW_UP_SEL);
    #endif
        gIsddr800DisGating = 0;
    }
}
#endif


// The "ENABLE_RANK_NUMBER_AUTO_DETECTION" use this API
void DFSInitForCalibration(DRAMC_CTX_T *p)
{
#ifdef DDR_INIT_TIME_PROFILING
    U32 CPU_Cycle;
    mcSHOW_TIME_MSG(("*** Data rate %d ***\n\n", p->frequency <<1));

    TimeProfileBegin();
#endif

    u1PrintModeRegWrite=1;
    DramcInit(p);
    u1PrintModeRegWrite=0;
    vApplyConfigBeforeCalibration(p);
    
#ifdef DUMP_INIT_RG_LOG_TO_DE
    while(1);
#endif

#if ENABLE_DUTY_SCAN_V2
#ifdef DDR_INIT_TIME_PROFILING
    CPU_Cycle=TimeProfileEnd();
    mcSHOW_TIME_MSG(("  (1) DFSInitForCalibration() take %d ms\n\n", CPU_Cycle/1000 - gu4DutyCalibrationTime));
    mcSHOW_TIME_MSG(("  (2) DramcNewDutyCalibration take %d ms\n\r",gu4DutyCalibrationTime));
#endif
#endif

#if !(defined(DUMP_INIT_RG_LOG_TO_DE))
    #ifdef ENABLE_MIOCK_JMETER
    if ((Get_MDL_Used_Flag()==NORMAL_USED) && (p->frequency >= 800))
    {
        Get_RX_DelayCell(p);
    }
    #endif
#endif    

#if !__ETT__
    if (p->frequency >= 1333)
#endif        
    {
#ifdef DDR_INIT_TIME_PROFILING
        TimeProfileBegin();
#endif 

#ifndef DUMP_INIT_RG_LOG_TO_DE
    #ifdef ENABLE_MIOCK_JMETER
    if (Get_MDL_Used_Flag()==NORMAL_USED)
    {    
        PRE_MIOCK_JMETER_HQA_USED(p);
    }
    #endif
#endif

#ifdef DDR_INIT_TIME_PROFILING
      CPU_Cycle=TimeProfileEnd();    
      mcSHOW_TIME_MSG(("  (3) JMeter takes %d ms\n\r", CPU_Cycle/1000));      
#endif 
    }

}

void DramcHWDQSGatingTracking_DVT(DRAMC_CTX_T *p)
{
#ifdef HW_GATING
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
                                            P_Fld(1, STBCAL1_STB_SHIFT_DTCOUT_IG));

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
#endif
}




/* Since initial register settings are for LP3_DDR1866, TX DQS, DQS_OEN (output enable), DQ delay
 * need to be adjusted according to the difference between Write Latencies for target frequency group
 */
static void TxCalibrationMoveDQSByDifferentWL(DRAMC_CTX_T *p)
{
    U8 u1ByteIdx, u1ACTableIdx = 0;
    U8 backup_rank, u1RankIdx;
    S8 s1WriteLatencyDiff = 0; /* Difference of write latency compared with LP3_DDR1866 */
    S8 s1TargetFreqGroupWL = 0, s1FreqGroup933WL = 0;

    // Find target freqGroup, and LP3_DDR1866's Write latency ACTimings
    for (u1ACTableIdx = 0; u1ACTableIdx < TOTAL_AC_TIMING_NUMBER; u1ACTableIdx++)
    {
        if (ACTimingTbl[u1ACTableIdx].dramType != TYPE_LPDDR3)  // This function is for LP3 only, skip other dram type ACTimingTable entries
        {
            continue;
        }

        if (ACTimingTbl[u1ACTableIdx].freq == p->freqGroup) // Retrieve target ACTiming FreqGroup write latency
        {
            s1TargetFreqGroupWL = ACTimingTbl[u1ACTableIdx].writeLat;
        }

        if (ACTimingTbl[u1ACTableIdx].freq == 933) //Retrieve LP3_DDR1866 (933Mhz) write latency
        {
            s1FreqGroup933WL = ACTimingTbl[u1ACTableIdx].writeLat;
        }
    }

    // Check if either source (LP3_DDR1866) or target FreqGroup's Write latency wasn't found
    if ((s1FreqGroup933WL == 0) || (s1TargetFreqGroupWL == 0))
    {
        mcSHOW_ERR_MSG(("Write latency search err!\n"));
        #if __ETT__
        while(1);
        #endif
    }

    s1WriteLatencyDiff = ((s1FreqGroup933WL - s1TargetFreqGroupWL)<<1); //Calculate Write Latency Difference (compared with LP3_DDR1866)
    mcSHOW_DBG_MSG3(("u1WriteLatencyDiff x2 = %d\n", s1WriteLatencyDiff));

    backup_rank = u1GetRank(p);

    /* Set DQS, DQS_OEN and DQ delay according to Write Latency difference with LP3_DDR1866 */
    for(u1ByteIdx = 0 ; u1ByteIdx < (p->data_width / DQS_BIT_NUMBER); u1ByteIdx++)
    {
        MoveDramC_TX_DQS(p, u1ByteIdx, -s1WriteLatencyDiff);
        MoveDramC_TX_DQS_OEN(p, u1ByteIdx, -s1WriteLatencyDiff);

        for(u1RankIdx = RANK_0; u1RankIdx < p->support_rank_num; u1RankIdx++)
        {
            vSetRank(p, u1RankIdx);
            MoveDramC_TX_DQ(p, u1ByteIdx, -s1WriteLatencyDiff);
            MoveDramC_TX_DQ_OEN(p, u1ByteIdx, -s1WriteLatencyDiff);
        }
        vSetRank(p, backup_rank);
    }

    return;
}

#if TX_PICG_NEW_MODE
void TXPICGSetting(DRAMC_CTX_T * p)
{
    U32 u4DQS_OEN_final, u4DQ_OEN_final;
    U16 u2DQS_OEN_2T[2], u2DQS_OEN_05T[2], u2DQS_OEN_Delay[2];
    U16 u2DQ_OEN_2T[2], u2DQ_OEN_05T[2], u2DQ_OEN_Delay[2];
    U16 u2COMB_TX_SEL[2];
    U16 u2Shift_Div[2];
    U16 u2COMB_TX_PICG_CNT;
    U8 u1CHIdx, u1RankIdx, u1Rank_bak=u1GetRank(p), u1backup_CH=vGetPHY2ChannelMapping(p), u1Div_ratio;

    if(vGet_Div_Mode(p) == DIV4_MODE)
    {
        u2Shift_Div[0] = 2;
        u2Shift_Div[1] = 0;
        u2COMB_TX_PICG_CNT = 9;
    #if TX_OE_EXTEND
        u2COMB_TX_PICG_CNT++;
    #endif
        u1Div_ratio = 2;
    }
    else //DIV8_MODE 
    {
        u2Shift_Div[0] = 10;
        u2Shift_Div[1] = 6;
        u2COMB_TX_PICG_CNT = 7;
    #if TX_OE_EXTEND
        u2COMB_TX_PICG_CNT++;
    #endif
        u1Div_ratio = 3;
    }

    for(u1CHIdx=0; u1CHIdx<p->support_channel_num; u1CHIdx++)
    {
        vSetPHY2ChannelMapping(p, u1CHIdx);  
        //Set TX DQS PICG
        //DQS0
        u2DQS_OEN_2T[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_DQS0), SHU_SELPH_DQS0_TXDLY_OEN_DQS0);
        u2DQS_OEN_05T[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_DQS1), SHU_SELPH_DQS1_DLY_OEN_DQS0);
        u2DQS_OEN_Delay[0] = (u2DQS_OEN_2T[0] <<u1Div_ratio) + u2DQS_OEN_05T[0];
        //DQS1
        u2DQS_OEN_2T[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_DQS0), SHU_SELPH_DQS0_TXDLY_OEN_DQS1);
        u2DQS_OEN_05T[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_DQS1), SHU_SELPH_DQS1_DLY_OEN_DQS1);
        u2DQS_OEN_Delay[1] = (u2DQS_OEN_2T[1] <<u1Div_ratio) + u2DQS_OEN_05T[1];

        u4DQS_OEN_final = (u2DQS_OEN_Delay[0] > u2DQS_OEN_Delay[1]) ? u2DQS_OEN_Delay[1] : u2DQS_OEN_Delay[0]; //choose minimum value

        u2COMB_TX_SEL[0] = (u4DQS_OEN_final > u2Shift_Div[0]) ? ((u4DQS_OEN_final - u2Shift_Div[0])>>u1Div_ratio) : 0;

        if(vGet_Div_Mode(p) == DIV4_MODE)
            u2COMB_TX_SEL[1] = 0;
        else
            u2COMB_TX_SEL[1] = (u4DQS_OEN_final > u2Shift_Div[1]) ? ((u4DQS_OEN_final - u2Shift_Div[1])>>u1Div_ratio) : 0;

        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_APHY_TX_PICG_CTRL), P_Fld(u2COMB_TX_SEL[0], SHU_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_DQS_SEL_P0)
                                             | P_Fld(u2COMB_TX_SEL[1], SHU_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_DQS_SEL_P1)
                                             | P_Fld(u2COMB_TX_PICG_CNT, SHU_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_PICG_CNT));
        //Set TX RK0 and RK1 DQ PICG
        for(u1RankIdx=RANK_0; u1RankIdx<p->support_rank_num; u1RankIdx++)
        {
            vSetRank(p, u1RankIdx);
			//DQ0
            u2DQ_OEN_2T[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0), SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0);
            u2DQ_OEN_05T[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ2), SHURK0_SELPH_DQ2_DLY_OEN_DQ0);
            u2DQ_OEN_Delay[0] = (u2DQ_OEN_2T[0] <<u1Div_ratio) + u2DQ_OEN_05T[0];
            //DQ1
            u2DQ_OEN_2T[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0), SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1);
            u2DQ_OEN_05T[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ2), SHURK0_SELPH_DQ2_DLY_OEN_DQ1);
            u2DQ_OEN_Delay[1] = (u2DQ_OEN_2T[1] <<u1Div_ratio) + u2DQ_OEN_05T[1];
			
            //Reset Shift_Div if is DVI8_MODE
            if(vGet_Div_Mode(p) == DIV8_MODE)
            {
                u2Shift_Div[0] = 8;
                u2Shift_Div[1] = 4;
            }
            else //DIV4_MODE
            {
                u2Shift_Div[0] = 0;
            }

            u4DQ_OEN_final = (u2DQ_OEN_Delay[0] > u2DQ_OEN_Delay[1]) ? u2DQ_OEN_Delay[1] : u2DQ_OEN_Delay[0]; //choose minimum value

            u2COMB_TX_SEL[0] = (u4DQ_OEN_final > u2Shift_Div[0]) ? ((u4DQ_OEN_final - u2Shift_Div[0])>>u1Div_ratio) : 0;

            if(vGet_Div_Mode(p) == DIV4_MODE)
               u2COMB_TX_SEL[1] = 0;
            else
               u2COMB_TX_SEL[1] = (u4DQ_OEN_final > u2Shift_Div[1]) ? ((u4DQ_OEN_final - u2Shift_Div[1])>>u1Div_ratio) : 0;
			
            if(u1RankIdx == RANK_0)
            {
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_APHY_TX_PICG_CTRL), P_Fld(u2COMB_TX_SEL[0], SHU_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_DQ_RK0_SEL_P0)
                                                 | P_Fld(u2COMB_TX_SEL[1], SHU_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_DQ_RK0_SEL_P1));
            }
            else //RANK1 
            {
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_WODT), P_Fld(u2COMB_TX_SEL[0], SHU_WODT_DDRPHY_CLK_EN_COMB_TX_DQ_RK1_SEL_P0)
                                                 | P_Fld(u2COMB_TX_SEL[1], SHU_WODT_DDRPHY_CLK_EN_COMB_TX_DQ_RK1_SEL_P1));
            }
        }
        vSetRank(p, u1Rank_bak);
    }
    vSetPHY2ChannelMapping(p, u1backup_CH);
}
#endif

#if RX_PICG_NEW_MODE
void RXPICGSetting(DRAMC_CTX_T * p)
{
    //PI_CG_DQSIEN new mode
    vIO32WriteFldAlign(DRAMC_REG_STBCAL1, 1, STBCAL1_STBCNT_SHU_RST_EN);
	vIO32WriteFldAlign(DRAMC_REG_STBCAL2, 1, STBCAL2_DQSIEN_SELPH_BY_RANK_EN);
    vIO32WriteFldMulti(DRAMC_REG_SHU_STBCAL, P_Fld(1, SHU_STBCAL_DQSIEN_RX_SELPH_OPT)
                                             | P_Fld(1, SHU_STBCAL_DQSIEN_PICG_MODE));
	//APHY control new mode
    vIO32WriteFldMulti(DRAMC_REG_SHU_STBCAL, P_Fld(1, SHU_STBCAL_RX_IN_GATE_EN_OPT)
                                             | P_Fld(1, SHU_STBCAL_RX_IN_BUFF_EN_OPT));

    //Dummy code (based on DVT document Verification plan of RX PICG efficiency improvment.docx)
    //No need to set since HW setting or setting in other place
    //Pls. don't remove for the integrity
#if 0
    {
        U8 u1TAIL_LAT = (vGet_Div_Mode(p) == DIV4_MODE) ? 1 : 0;

        vIO32WriteFldAlign(DRAMC_REG_STBCAL2, 0, STBCAL2_STB_STBENRST_EARLY_1T_EN);
        vIO32WriteFldMulti(DRAMC_REG_SHU_STBCAL, P_Fld(u1TAIL_LAT, SHU_STBCAL_R1_DQSIEN_PICG_TAIL_EXT_LAT) 
                    | P_Fld(0, SHU_STBCAL_R1_DQSIEN_PICG_HEAD_EXT_LAT) 
                    | P_Fld(u1TAIL_LAT, SHU_STBCAL_R0_DQSIEN_PICG_TAIL_EXT_LAT) 
                    | P_Fld(0, SHU_STBCAL_R0_DQSIEN_PICG_HEAD_EXT_LAT));
        vIO32WriteFldMulti(DRAMC_REG_PHY_RX_INCTL, P_Fld(0, PHY_RX_INCTL_DIS_IN_BUFF_EN) 
                    | P_Fld(0, PHY_RX_INCTL_FIX_IN_BUFF_EN) 
                    | P_Fld(0, PHY_RX_INCTL_RX_IN_BUFF_EN_4BYTE_EN) 
                    | P_Fld(0, PHY_RX_INCTL_DIS_IN_GATE_EN)
                    | P_Fld(0, PHY_RX_INCTL_FIX_IN_GATE_EN)
                    | P_Fld(0, PHY_RX_INCTL_RX_IN_GATE_EN_4BYTE_EN));
    }
#endif    
}
#endif

#if TX_PICG_NEW_MODE
void TXPICGNewModeEnable(DRAMC_CTX_T * p)
{
    //Switch TX PICG to new mode
    vIO32WriteFldMulti_All(DDRPHY_MISC_CTRL3, P_Fld(0, MISC_CTRL3_ARPI_CG_MCK_DQ_OPT)
                                         | P_Fld(0, MISC_CTRL3_ARPI_MPDIV_CG_DQ_OPT)
                                         | P_Fld(0, MISC_CTRL3_ARPI_CG_DQS_OPT)
                                         | P_Fld(0, MISC_CTRL3_ARPI_CG_DQ_OPT));
}
#endif

#if ENABLE_WRITE_DBI_Protect
void ApplyWriteDBIProtect(DRAMC_CTX_T *p, U8 onoff)
{
    U8 *uiLPDDR_O1_Mapping;
    U16 Temp_PinMux_MaskWrite_WriteDBIOn=0;
    U8 B0_PinMux_MaskWrite_WriteDBIOn=0, B1_PinMux_MaskWrite_WriteDBIOn=0;
    int DQ_index;

    uiLPDDR_O1_Mapping = (U8 *)uiLPDDR4_O1_Mapping_POP[p->channel];

    // Write DMI/DBI Protect Function
    // Byte0 can not have bit swap between Group1(DQ0/1) and Group2(DQ02~DQ07).
    // Byte1 can not have bit swap between Group1(DQ8/9) and Group2(DQ10~DQ15).
    // DBIWR_IMP_EN=1 and DBIWR_PINMUX_EN=1
    // set DBIWR_OPTB0[7:0] meet with Byte0 pin MUX table.
    // set DBIWR_OPTB1[7:0] meet with Byte1 pin MUX table.

    for(DQ_index=0; DQ_index<16; DQ_index++)
    {
        Temp_PinMux_MaskWrite_WriteDBIOn |= ((0x7C7C >> uiLPDDR_O1_Mapping[DQ_index]) & 0x1) << DQ_index;
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

