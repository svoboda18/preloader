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
#ifndef MT6739_FPGA
#include <pmic.h>
#endif
#endif
#endif

#include "dramc_common.h"
#include "dramc_pi_api.h"
#include "x_hal_io.h"
#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0) 
#include "upmu_hw.h"
#endif

#if ! __ETT__
#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)

#include "emi.h"
#include "pmic.h"
#ifdef DDR_RESERVE_MODE  
#include "platform.h"
#include "wdt.h"
#endif
#endif
#endif
//=============================================================================
//  Definition                                                         
//=============================================================================

//=============================================================================
//  Global Variables                                                  
//=============================================================================
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
#include "emi.h"
   SAVE_TIME_FOR_CALIBRATION_T SavetimeData;
#endif

#ifdef DDR_INIT_TIME_PROFILING
static U32 CPUCycleSum=0;
#endif

DRAM_DFS_FREQUENCY_TABLE_T gFreqTbl[DRAM_DFS_SHUFFLE_MAX] = {
    {LP4_DDR3200, 1600, DRAM_DFS_SHUFFLE_1},
    {LP4_DDR2667, 1333, DRAM_DFS_SHUFFLE_2},
    {LP4_DDR2667, 1333, DRAM_DFS_SHUFFLE_3},
    {LP4_DDR800,   400, DRAM_DFS_SHUFFLE_4},
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
  LP3_DDR1333,
#else
  LP3_DDR1866,
#endif  
  DRAM_DFS_SHUFFLE_1,
  TYPE_LPDDR3,        // DRAM_DRAM_TYPE_T
  FSP_0 , //// DRAM Fast switch point type, only for LP4, useless in LP3
  ODT_OFF,
  CBT_NORMAL_MODE,
  DBI_OFF,
  DBI_OFF,
  PACKAGE_POP,        // DRAM_PACKAGE_T
  DATA_WIDTH_32BIT,     // DRAM_DATA_WIDTH_T
  DEFAULT_TEST2_1_CAL,    // test2_1;
  DEFAULT_TEST2_2_CAL,    // test2_2;
  TEST_XTALK_PATTERN,     // test_pattern;
  667,
  0x88, //vendor_id initial value
  0, //density
  {0,0},
  0,  // u2DelayCellTimex100;
  DISABLE,  // fglow_freq_write_en;
  DISABLE,  // ssc_en;
  DISABLE,   // en_4bitMux;
  
  DISABLE,  // enable_cbt_scan_vref;
  DISABLE,  // enable_rx_scan_vref;
  DISABLE,   // enable_tx_scan_vref;
#if SUPPORT_SAVE_OFF_LINE_CALIBRATION
  NULL,
#endif

   //aru4CalResultFlag[CHANNEL_MAX][RANK_MAX][DRAM_DFS_MAX]
{{{0,0,0,0,},  {0,0,0,0}},
 {{0,0,0,0,},  {0,0,0,0}},
 {{0,0,0,0,},  {0,0,0,0}},
  {{0,0,0,0,}, {0,0,0,0}}}, 
//aru4CalExecuteFlag[CHANNEL_MAX][RANK_MAX][DRAM_DFS_MAX]
{{{0,0,0,0,},  {0,0,0,0}},
 {{0,0,0,0,},  {0,0,0,0}},
 {{0,0,0,0,},  {0,0,0,0}},
  {{0,0,0,0,}, {0,0,0,0}}}, 

#if WRITE_LEVELING_MOVE_DQS_INSTEAD_OF_CLK
  {{0,0}, {0,0}, {0,0}, {0,0}}, //BOOL arfgWriteLevelingInitShif;
#endif
#if TX_PERBIT_INIT_FLOW_CONTROL
  {{FALSE, FALSE}, {FALSE, FALSE}, {FALSE, FALSE}, {FALSE, FALSE}},//BOOL fgTXPerbifInit;
#endif
};
#endif

DRAMC_CTX_T DramCtx_LPDDR4 =
{
  CHANNEL_DUAL, // Channel number
  CHANNEL_A,          // DRAM_CHANNEL
  RANK_DUAL,        //DRAM_RANK_NUMBER_T
  RANK_0,               //DRAM_RANK_T
#if DUAL_FREQ_K
  LP4_DDR800,
#else
  LP4_DDR3200,
#endif  
  DRAM_DFS_SHUFFLE_1,
  TYPE_LPDDR4X,        // DRAM_DRAM_TYPE_T
  FSP_0 , //// DRAM Fast switch point type, only for LP4, useless in LP3  
  ODT_OFF,
  CBT_NORMAL_MODE,   // bring up LP4X non-BYTE mode
  DBI_OFF,  //read DBI
  DBI_OFF,  // write DBI
  PACKAGE_POP,        // DRAM_PACKAGE_T
  DATA_WIDTH_16BIT,     // DRAM_DATA_WIDTH_T
  DEFAULT_TEST2_1_CAL,    // test2_1;
  DEFAULT_TEST2_2_CAL,    // test2_2;
  TEST_XTALK_PATTERN,     // test_pattern;
  800,
  0x88, //vendor_id initial value  
  0, //density
  {0,0},  
  0,  // u2DelayCellTimex100;
  DISABLE,  // fglow_freq_write_en;
  DISABLE,  // ssc_en;
  DISABLE,   // en_4bitMux;
  
  ENABLE,   // enable_cbt_scan_vref;
  ENABLE,  // enable_rx_scan_vref;
  ENABLE,   // enable_tx_scan_vref;
#if SUPPORT_SAVE_OFF_LINE_CALIBRATION
  NULL,
#endif

   //aru4CalResultFlag[CHANNEL_MAX][RANK_MAX][DRAM_DFS_MAX]
{{{0,0,0,0,},  {0,0,0,0}},
 {{0,0,0,0,},  {0,0,0,0}},
 {{0,0,0,0,},  {0,0,0,0}},
  {{0,0,0,0,}, {0,0,0,0}}}, 
//aru4CalExecuteFlag[CHANNEL_MAX][RANK_MAX][DRAM_DFS_MAX]
{{{0,0,0,0,},  {0,0,0,0}},
 {{0,0,0,0,},  {0,0,0,0}},
 {{0,0,0,0,},  {0,0,0,0}},
  {{0,0,0,0,}, {0,0,0,0}}}, 

#if WRITE_LEVELING_MOVE_DQS_INSTEAD_OF_CLK
  {{0,0}, {0,0}, {0,0}, {0,0}}, //BOOL arfgWriteLevelingInitShif;
#endif
#if TX_PERBIT_INIT_FLOW_CONTROL
  {{FALSE, FALSE}, {FALSE, FALSE}, {FALSE, FALSE}, {FALSE, FALSE}},//BOOL fgTXPerbifInit;
#endif
    	#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
            &SavetimeData,
	#endif
};

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

extern void EMI_Init(DRAMC_CTX_T *p);
extern void EMI_Init2(void);

#define DDRPHY0AO_BASE Channel_A_PHY_BASE_ADDRESS
#define DDRPHY1AO_BASE Channel_B_PHY_BASE_ADDRESS
#define DDRPHY2AO_BASE Channel_C_PHY_BASE_ADDRESS
#define DDRPHY3AO_BASE Channel_D_PHY_BASE_ADDRESS
#define DRAMC0_AO_BASE Channel_A_DRAMC_AO_BASE_ADDRESS
#define DRAMC1_AO_BASE Channel_B_DRAMC_AO_BASE_ADDRESS
#define DRAMC2_AO_BASE Channel_C_DRAMC_AO_BASE_ADDRESS
#define DRAMC3_AO_BASE Channel_D_DRAMC_AO_BASE_ADDRESS
#define DRAMC0NAO_BASE Channel_A_DRAMC_NAO_BASE_ADDRESS
#define DRAMC1NAO_BASE Channel_B_DRAMC_NAO_BASE_ADDRESS
#define DRAMC2NAO_BASE Channel_C_DRAMC_NAO_BASE_ADDRESS
#define DRAMC3NAO_BASE Channel_D_DRAMC_NAO_BASE_ADDRESS
#define WHITNEY_SPM_POWER_ON_VAL0 (0x10B00004)
#define WHITNEY_DRAMC_DPY_CLK_SW_CON3 (0x10B00474)

static U32 u4Cha_mr4_backup = 0; //Suppose 4 channel with the same value
//static U32 u4Chb_mr4_backup = 0;
//static U32 u4Chc_mr4_backup = 0;
//static U32 u4Chd_mr4_backup = 0;
static U32 u4Cha_dummy_read_backup = 0; //Suppose 4 channel with the same value
//static U32 u4Chb_dummy_read_backup = 0;
//static U32 u4Chc_dummy_read_backup = 0;
//static U32 u4Chd_dummy_read_backup = 0;
static U32 u4Cha_dqsosc_backup = 0; //Suppose 4 channel with the same value
//static U32 u4Chb_dqsosc_backup = 0;
//static U32 u4Chc_dqsosc_backup = 0;
//static U32 u4Chd_dqsosc_backup = 0;
void Dramc_DDR_Reserved_Mode_setting(void)//Must be four channel
{
    DRAMC_CTX_T *p = &DramCtx_LPDDR4;
    U32 u4WbrBackup = 0;
    U32 regValue = 0;

    //scy: workaround for DDR reserve mode
    DramcHWGatingOnOff(p, 0);

    TransferPLLToSPMControl(p);

    // ---------------------------------------------------------
    // begin of Configure
    /*TINFO="Begin of ddr reserve mode setup for DDRPHY"*/
    u4WbrBackup = *((volatile unsigned int*)DRAMC_WBR);
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);

    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 1, DRAMC_PD_CTRL_MIOCKCTRLOFF);   //MIOCKCTRLOFF=1
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 0, DRAMC_PD_CTRL_PHYCLKDYNGEN);   //PHYCLKDYNGEN=0

    *((volatile unsigned int*)(0x10006474)) |= (0x03 << 26);//Set spm shuffle enable to 1
    mcDELAY_US(1);
    *((volatile unsigned int*)(0x10006474)) &= ~(0x03 << 26);//Set spm shuffle enable to 0

    MPLLInit();//52MHz
    mcSHOW_DBG_MSG(("###Ori MPLLINIT + DQ0 DQ1 + spm control CMD\n"));
    mcSHOW_DBG_MSG(("###SPM_POWER_ON_VAL0:0x%x\n", SPM_POWER_ON_VAL0));

    vIO32WriteFldMulti(SPM_POWER_ON_VAL0, P_Fld(1, SPM_POWER_ON_VAL0_SC_PHYPLL_SHU_EN_PCM) 
        | P_Fld(1, SPM_POWER_ON_VAL0_SC_PHYPLL2_SHU_EN_PCM));
    mcDELAY_US(1);
    vIO32WriteFldMulti(SPM_POWER_ON_VAL0, P_Fld(0, SPM_POWER_ON_VAL0_SC_PHYPLL_SHU_EN_PCM) 
        | P_Fld(0, SPM_POWER_ON_VAL0_SC_PHYPLL2_SHU_EN_PCM));


    u4Cha_mr4_backup = u4IO32Read4B(DRAMC_REG_SPCMDCTRL);
    vIO32WriteFldAlign_All(DRAMC_REG_SPCMDCTRL, 1, SPCMDCTRL_REFRDIS);
    
    u4Cha_dqsosc_backup = u4IO32Read4B(DRAMC_REG_DQSOSCR);
    vIO32WriteFldMulti_All(DRAMC_REG_DQSOSCR, P_Fld(1, DQSOSCR_DQSOSCRDIS) | P_Fld(1, DQSOSCR_DQSOSCENDIS));

    u4Cha_dummy_read_backup = u4IO32Read4B(DRAMC_REG_DUMMY_RD);

    //disable DDRPHY dynamic clock gating
    vIO32WriteFldAlign_All(DRAMC_REG_DRAMC_PD_CTRL, 0, DRAMC_PD_CTRL_PHYCLKDYNGEN);//R_DMPHYCLKDYNGEN=0
    
    // disable DDRPHY
    //change mcm_ck to 26M
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_CLK_MEM_SEL);//[5:4] mem_ck mux: 2'b00: 26MHz
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_W_CHG_MEM);////[5:4] mem_ck mux: 2'b00: 26MHz, freq change 
    mcDELAY_US(1);
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_W_CHG_MEM);////[5:4] mem_ck mux: 2'b00: 26MHz, disable freq change 
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI5, 0, B0_DLL_ARPI5_RG_ARDLL_PHDET_EN_B0);//RG_*PHDET_EN=0
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI5, 0, B1_DLL_ARPI5_RG_ARDLL_PHDET_EN_B1);//RG_*PHDET_EN=0
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI5, 0, CA_DLL_ARPI5_RG_ARDLL_PHDET_EN_CA);//RG_*PHDET_EN=0

    vIO32WriteFldMulti_All(DDRPHY_B0_DLL_ARPI2, P_Fld(1, B0_DLL_ARPI2_RG_ARPI_CG_MCK_B0) 
        | P_Fld(1, B0_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B0)
        | P_Fld(1, B0_DLL_ARPI2_RG_ARPI_CG_MCTL_B0)
        | P_Fld(1, B0_DLL_ARPI2_RG_ARPI_CG_FB_B0)
        | P_Fld(1, B0_DLL_ARPI2_RG_ARPI_CG_DQS_B0)
        | P_Fld(1, B0_DLL_ARPI2_RG_ARPI_CG_DQM_B0)
        | P_Fld(1, B0_DLL_ARPI2_RG_ARPI_CG_DQ_B0)
        | P_Fld(1, B0_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B0)
        | P_Fld(1, B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0)); //*PI_CG=1, RG_*MPDIV_CG=1
    vIO32WriteFldMulti_All(DDRPHY_B1_DLL_ARPI2, P_Fld(1, B1_DLL_ARPI2_RG_ARPI_CG_MCK_B1) 
        | P_Fld(1, B1_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B1)
        | P_Fld(1, B1_DLL_ARPI2_RG_ARPI_CG_MCTL_B1)
        | P_Fld(1, B1_DLL_ARPI2_RG_ARPI_CG_FB_B1)
        | P_Fld(1, B1_DLL_ARPI2_RG_ARPI_CG_DQS_B1)
        | P_Fld(1, B1_DLL_ARPI2_RG_ARPI_CG_DQM_B1)
        | P_Fld(1, B1_DLL_ARPI2_RG_ARPI_CG_DQ_B1)
        | P_Fld(1, B1_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B1)
        | P_Fld(1, B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1)); //*PI_CG=1, RG_*MPDIV_CG=1
    vIO32WriteFldMulti_All(DDRPHY_CA_DLL_ARPI2, P_Fld(1, CA_DLL_ARPI2_RG_ARPI_CG_MCK_CA) 
        | P_Fld(1, CA_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_CA)
        | P_Fld(1, CA_DLL_ARPI2_RG_ARPI_CG_MCTL_CA)
        | P_Fld(1, CA_DLL_ARPI2_RG_ARRPI_CG_FB_CA)
        | P_Fld(1, CA_DLL_ARPI2_RG_ARPI_CG_CS)
        | P_Fld(1, CA_DLL_ARPI2_RG_ARPI_CG_CLK)
        | P_Fld(1, CA_DLL_ARPI2_RG_ARPI_CG_CMD)
        | P_Fld(1, CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN)
        | P_Fld(1, CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA)); //*PI_CG=1, RG_*MPDIV_CG=1

    vIO32WriteFldAlign_All(DDRPHY_B0_DQ6, 0, B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0); //RG_*BIAS_EN=0
    vIO32WriteFldAlign_All(DDRPHY_B1_DQ6, 0, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1); //RG_*BIAS_EN=0
    vIO32WriteFldAlign_All(DDRPHY_CA_CMD6, 0, CA_CMD6_RG_RX_ARCMD_BIAS_EN); //RG_*BIAS_EN=0

    vIO32WriteFldAlign_All(DDRPHY_B0_DQ5, 0, B0_DQ5_RG_RX_ARDQ_VREF_EN_B0); //RG_*VREF_EN=0
    vIO32WriteFldAlign_All(DDRPHY_B1_DQ5, 0, B1_DQ5_RG_RX_ARDQ_VREF_EN_B1); //RG_*VREF_EN=0
    vIO32WriteFldAlign_All(DDRPHY_CA_CMD5, 0, CA_CMD5_RG_RX_ARCMD_VREF_EN); //RG_*VREF_EN=0

    vIO32WriteFldMulti_All(DDRPHY_PLL4, P_Fld(0, PLL4_RG_RPHYPLL_ADA_MCK8X_EN) 
                            | P_Fld(0, PLL4_RG_RPHYPLL_AD_MCK8X_EN));//RG_*MCK8X_EN=0
    vIO32WriteFldAlign_All(DDRPHY_SHU1_B0_DQ6, 0, SHU1_B0_DQ6_RG_ARPI_MIDPI_EN_B0); //RG_*MIDPI_EN=0
    vIO32WriteFldAlign_All(DDRPHY_SHU1_B1_DQ6, 0, SHU1_B1_DQ6_RG_ARPI_MIDPI_EN_B1); //RG_*MIDPI_EN=0
    vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_CMD6, 0, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA); //RG_*MIDPI_EN=0

    vIO32WriteFldAlign_All(DDRPHY_PLL1, 0, PLL1_RG_RPHYPLL_EN); //RG_*PLL_EN=0
    vIO32WriteFldAlign_All(DDRPHY_PLL2, 0, PLL2_RG_RCLRPLL_EN); //RG_*PLL_EN=0

    //!set sc_mpll to SPM register 
    //*((volatile unsigned int*)(0x10B00004)) |= (0x1 << 8) | (0x1 << 13); //set sc_mpll_off=1 , sc_mpll_s_off=1
    vIO32WriteFldMulti(SPM_POWER_ON_VAL0, P_Fld(1, SPM_POWER_ON_VAL0_SC_MPLL_OFF) 
        | P_Fld(1, SPM_POWER_ON_VAL0_SC_MPLL_S_OFF));
    *((UINT32P)(0x10B00318 )) |= (0x1 << 1); //ddrphy_pwr_iso=1

    vIO32WriteFldAlign_All(DDRPHY_B0_DQ3, 0, B0_DQ3_RG_ARDQ_RESETB_B0); //RG_*RESETB=0
    vIO32WriteFldAlign_All(DDRPHY_B0_DQ3, 0, B0_DQ3_RG_TX_ARDQ_EN_B0); ///TODO:DQ B0 diable
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI0, 0, B0_DLL_ARPI0_RG_ARPI_RESETB_B0); //RG_*RESETB=0

    vIO32WriteFldAlign_All(DDRPHY_B1_DQ3, 0, B1_DQ3_RG_ARDQ_RESETB_B1); //RG_*RESETB=0
    vIO32WriteFldAlign_All(DDRPHY_B1_DQ3, 0, B1_DQ3_RG_TX_ARDQ_EN_B1); ///TODO: DQ B1 diable 

    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI0, 0, B1_DLL_ARPI0_RG_ARPI_RESETB_B1); //RG_*RESETB=0
    vIO32WriteFldAlign_All(DDRPHY_CA_CMD3, 0, CA_CMD3_RG_ARCMD_RESETB); //RG_*RESETB=0
    vIO32WriteFldAlign_All(DDRPHY_CA_CMD3, 0, CA_CMD3_RG_TX_ARCMD_EN); ///TODO: CMD disable

    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI0, 0, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);  //RG_*RESETB=0
    vIO32WriteFldAlign_All(DDRPHY_PLL4, 0, PLL4_RG_RPHYPLL_RESETB);  //RG_*RESETB=0
    
    // wait 1us
    mcDELAY_US(1);
    
    // enable DDRPHY by normal flow
    
    // enable DDRPHY by normal flow
    //marked by kaihsin on May 7th, not necessary to PDN DDRPHY
    //*((UINT32P)(0x1000631c )) |= (0x1 << 2); //ddrphy_pwr_on=1
    //*((UINT32P)(0x100063b8 )) |= (0x1 << 2);
    // wait 1us
    mcDELAY_US(1);

    vIO32WriteFldAlign_All(DDRPHY_B0_DQ3, 1, B0_DQ3_RG_ARDQ_RESETB_B0); //RG_*RESETB=1
    vIO32WriteFldAlign_All(DDRPHY_B0_DQ3, 1, B0_DQ3_RG_TX_ARDQ_EN_B0); ///TODO:DQ B0 enable

    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI0, 1, B0_DLL_ARPI0_RG_ARPI_RESETB_B0); //RG_*RESETB=1
    vIO32WriteFldAlign_All(DDRPHY_B1_DQ3, 1, B1_DQ3_RG_ARDQ_RESETB_B1); //RG_*RESETB=1
    vIO32WriteFldAlign_All(DDRPHY_B1_DQ3, 1, B1_DQ3_RG_TX_ARDQ_EN_B1); ///TODO: DQ B1 enable
    
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI0, 1, B1_DLL_ARPI0_RG_ARPI_RESETB_B1); //RG_*RESETB=1
    vIO32WriteFldAlign_All(DDRPHY_CA_CMD3, 1, CA_CMD3_RG_ARCMD_RESETB); //RG_*RESETB=1
    vIO32WriteFldAlign_All(DDRPHY_CA_CMD3, 1, CA_CMD3_RG_TX_ARCMD_EN); ///TODO: CMD enable
    
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI0, 1, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);  //RG_*RESETB=1
    vIO32WriteFldAlign_All(DDRPHY_PLL4, 1, PLL4_RG_RPHYPLL_RESETB);  //RG_*RESETB=1

    *((UINT32P)(0x10B00318 )) &= ~(0x1 << 1); //ddrphy_pwr_iso=0

    //MPLL enable -- TBD
    // wait 20us for MPLL
    //!set sc_mpll to SPM register 
    vIO32WriteFldMulti(SPM_POWER_ON_VAL0, P_Fld(0, SPM_POWER_ON_VAL0_SC_MPLL_OFF) 
        | P_Fld(0, SPM_POWER_ON_VAL0_SC_MPLL_S_OFF));
    mcDELAY_US(20);
    
    vIO32WriteFldAlign_All(DDRPHY_PLL1, 1, PLL1_RG_RPHYPLL_EN); //RG_*PLL_EN=1
    vIO32WriteFldAlign_All(DDRPHY_PLL2, 1, PLL2_RG_RCLRPLL_EN); //RG_*PLL_EN=1

    // wait 20us for MEMPLL
    mcDELAY_US(20);
    
    vIO32WriteFldAlign_All(DDRPHY_B0_DQ5, 1, B0_DQ5_RG_RX_ARDQ_VREF_EN_B0); //RG_*VREF_EN=1
    vIO32WriteFldAlign_All(DDRPHY_B1_DQ5, 1, B1_DQ5_RG_RX_ARDQ_VREF_EN_B1); //RG_*VREF_EN=1

    vIO32WriteFldMulti_All(DDRPHY_PLL4, P_Fld(1, PLL4_RG_RPHYPLL_ADA_MCK8X_EN));//RG_*MCK8X_EN=1
    vIO32WriteFldAlign_All(DDRPHY_SHU1_B0_DQ6, 1, SHU1_B0_DQ6_RG_ARPI_MIDPI_EN_B0); //RG_*MIDPI_EN=1
    vIO32WriteFldAlign_All(DDRPHY_SHU1_B1_DQ6, 1, SHU1_B1_DQ6_RG_ARPI_MIDPI_EN_B1); //RG_*MIDPI_EN=1
    vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_CMD6, 1, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA); //RG_*MIDPI_EN=1

    // wait 1us
    mcDELAY_US(1);

    vIO32WriteFldMulti_All(DDRPHY_B0_DLL_ARPI2, P_Fld(0, B0_DLL_ARPI2_RG_ARPI_CG_MCK_B0) 
        | P_Fld(0, B0_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B0)
        | P_Fld(0, B0_DLL_ARPI2_RG_ARPI_CG_MCTL_B0)
        | P_Fld(0, B0_DLL_ARPI2_RG_ARPI_CG_FB_B0)
        | P_Fld(0, B0_DLL_ARPI2_RG_ARPI_CG_DQS_B0)
        | P_Fld(0, B0_DLL_ARPI2_RG_ARPI_CG_DQM_B0)
        | P_Fld(0, B0_DLL_ARPI2_RG_ARPI_CG_DQ_B0)
        | P_Fld(0, B0_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B0)
        | P_Fld(0, B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0)); //*PI_CG=0, RG_*MPDIV_CG=0
    vIO32WriteFldMulti_All(DDRPHY_B1_DLL_ARPI2, P_Fld(0, B1_DLL_ARPI2_RG_ARPI_CG_MCK_B1) 
        | P_Fld(0, B1_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B1)
        | P_Fld(0, B1_DLL_ARPI2_RG_ARPI_CG_MCTL_B1)
        | P_Fld(0, B1_DLL_ARPI2_RG_ARPI_CG_FB_B1)
        | P_Fld(0, B1_DLL_ARPI2_RG_ARPI_CG_DQS_B1)
        | P_Fld(0, B1_DLL_ARPI2_RG_ARPI_CG_DQM_B1)
        | P_Fld(0, B1_DLL_ARPI2_RG_ARPI_CG_DQ_B1)
        | P_Fld(0, B1_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B1)
        | P_Fld(0, B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1)); //*PI_CG=0, RG_*MPDIV_CG=0
    vIO32WriteFldMulti_All(DDRPHY_CA_DLL_ARPI2, P_Fld(0, CA_DLL_ARPI2_RG_ARPI_CG_MCK_CA) 
        | P_Fld(0, CA_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_CA)
        | P_Fld(0, CA_DLL_ARPI2_RG_ARPI_CG_MCTL_CA)
        | P_Fld(0, CA_DLL_ARPI2_RG_ARRPI_CG_FB_CA)
        | P_Fld(0, CA_DLL_ARPI2_RG_ARPI_CG_CS)
        | P_Fld(0, CA_DLL_ARPI2_RG_ARPI_CG_CLK)
        | P_Fld(0, CA_DLL_ARPI2_RG_ARPI_CG_CMD)
        | P_Fld(0, CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA)); //*PI_CG=0, RG_*MPDIV_CG=0


    // wait 1us
    mcDELAY_US(1);
    
#if 0
    regValue = u4IO32Read4B(0x10330288);
    mcSHOW_DBG_MSG(("### (0x288) = [0x%x]\n", regValue));
    regValue = u4IO32Read4B(0x10330294);
    mcSHOW_DBG_MSG(("### (0x294) = [0x%x]\n", regValue));
    regValue = u4IO32Read4B(0x103305f0);
    mcSHOW_DBG_MSG(("### (0x5f0) = [0x%x]\n", regValue));
    regValue = u4IO32Read4B(0x10330670);
    mcSHOW_DBG_MSG(("### (0x670) = [0x%x]\n", regValue));
    regValue = u4IO32Read4B(0x10330284);
    mcSHOW_DBG_MSG(("### (0x284) = [0x%x]\n", regValue));
#endif
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL1, 0xffffffff, MISC_CG_CTRL1_R_DVS_DIV4_CG_CTRL);
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL4, 0x11400000, MISC_CG_CTRL4_R_PHY_MCK_CG_CTRL);
    vIO32WriteFldAlign_All(DDRPHY_B0_RXDVS0, 1, B0_RXDVS0_R_RX_DLY_TRACK_CG_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_RXDVS0, 1, B1_RXDVS0_R_RX_DLY_TRACK_CG_EN_B1);
    vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL0, P_Fld(1, MISC_CG_CTRL0_RG_CG_INFRA_OFF_DISABLE) 
        | P_Fld(1, MISC_CG_CTRL0_RG_CG_IDLE_SYNC_EN)
        | P_Fld(1, MISC_CG_CTRL0_RG_CG_RX_COMB1_OFF_DISABLE)
        | P_Fld(1, MISC_CG_CTRL0_RG_CG_RX_COMB0_OFF_DISABLE)
        | P_Fld(1, MISC_CG_CTRL0_RG_CG_RX_CMD_OFF_DISABLE)
        | P_Fld(1, MISC_CG_CTRL0_RG_CG_COMB1_OFF_DISABLE)
        | P_Fld(1, MISC_CG_CTRL0_RG_CG_COMB0_OFF_DISABLE)
        | P_Fld(1, MISC_CG_CTRL0_RG_CG_CMD_OFF_DISABLE)
        | P_Fld(1, MISC_CG_CTRL0_RG_CG_COMB_OFF_DISABLE)
        | P_Fld(1, MISC_CG_CTRL0_RG_CG_PHY_OFF_DIABLE)
        | P_Fld(1, MISC_CG_CTRL0_RG_CG_DRAMC_OFF_DISABLE)
        | P_Fld(1, MISC_CG_CTRL0_RG_CG_EMI_OFF_DISABLE)
    );

    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_CLK_MEM_SEL);//[5:4] mem_ck mux: 2'b00: 26MHz
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_W_CHG_MEM);////[5:4] mem_ck mux: 2'b00: 26MHz, freq change 
    mcDELAY_US(1);
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_W_CHG_MEM);////[5:4] mem_ck mux: 2'b00: 26MHz, disable freq change 

    //force top feedback MCK not divide
    vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, 0xf, DRAMC_DPY_CLK_SW_CON2_SW_DDRPHY_FB_CK_EN); 
    vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_DDRPHY_FB_CK_EN_PCM); 
    
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI5, 1, CA_DLL_ARPI5_RG_ARDLL_PHDET_EN_CA);//RG_*PHDET_EN=1 (CA)
    // wait 1us
    mcDELAY_US(1);
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI5, 1, B0_DLL_ARPI5_RG_ARDLL_PHDET_EN_B0);//RG_*PHDET_EN=0 (B0)
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI5, 1, B1_DLL_ARPI5_RG_ARDLL_PHDET_EN_B1);//RG_*PHDET_EN=0 (B1)
    mcDELAY_US(1);

    //top feedback MCK to divided frequency -- TBD
    //*CLK_MEM_DFS_CFG &= ~(0x1 << 8); //rg_ddrphy_fb_ck_force_en = 0
    //*((volatile unsigned int*)(0x10B0046C )) &= ~(0xf << 8) ; //set sc_ddrphy_fb_ck_ch*_en = 0
    //*((volatile unsigned int*)(0x10B00004 )) &= ~(0x1 << 24) ; //set sc_ddrphy_fb_ck_ch*_en = 0
    vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, 0, DRAMC_DPY_CLK_SW_CON2_SW_DDRPHY_FB_CK_EN); 
    vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DDRPHY_FB_CK_EN_PCM); 

    vIO32WriteFldAlign_All(DRAMC_REG_DDRCONF0, 1, DDRCONF0_RDATRST); //R_DMRDATRST = 1
    vIO32WriteFldAlign_All(DRAMC_REG_DDRCONF0, 0, DDRCONF0_RDATRST); //R_DMRDATRST = 0

    vIO32WriteFldAlign_All(DRAMC_REG_DRAMC_PD_CTRL, 1, DRAMC_PD_CTRL_PHYCLKDYNGEN); //R_DMPHYCLKDYNGEN=1

    /*TINFO ="Setting RX input delay tracking enable from SPM side(un-paused)" */
    //*((volatile unsigned int*)(0x10B00498))  |= (0x1 << 3);  //! setting SPM rsv_8 (SLEEP_BASE + 0x654) [7:6] = 0x3 for RX input delay HW tracking enable (un-paused)
    //! diable dummy read when exit sref
    vIO32WriteFldMulti_All(DRAMC_REG_DUMMY_RD, P_Fld(0, DUMMY_RD_DQSG_DMYWR_EN) 
        | P_Fld(0, DUMMY_RD_DQSG_DMYRD_EN)
        | P_Fld(0, DUMMY_RD_SREF_DMYRD_EN)
        | P_Fld(0, DUMMY_RD_DUMMY_RD_EN)
        | P_Fld(0, DUMMY_RD_DMY_RD_DBG)
        | P_Fld(0, DUMMY_RD_DMY_WR_DBG)); 

    //! set DDRPHY PLL control path to normal path for DCS
    ///TODO: Whitney E1 only Channel C and Channel D will need to do this, Whitney E2 will need to modify 
    regValue = u4IO32Read4B(DDRPHY_MISC_SPM_CTRL0 + (2<<POS_BANK_NUM));
    vIO32Write4B(DDRPHY_MISC_SPM_CTRL0 + (2<<POS_BANK_NUM), regValue &= ~(0x3 << 7)); // switch PLL path from low power to normal mode  
    regValue = u4IO32Read4B(DDRPHY_MISC_SPM_CTRL0 + (3<<POS_BANK_NUM));
    vIO32Write4B(DDRPHY_MISC_SPM_CTRL0 + (3<<POS_BANK_NUM), regValue &= ~(0x3 << 7)); // switch PLL path from low power to normal mode  

    //Lewis@20161017: Modify after Chen-Hsiang compare the register
    // 1.PLL switch to SPM
    vIO32WriteFldAlign_All(DDRPHY_PLL1, 0, PLL1_RG_RPHYPLL_EN); //RG_*PLL_EN=0
    vIO32WriteFldAlign_All(DDRPHY_PLL2, 0, PLL2_RG_RCLRPLL_EN); //RG_*PLL_EN=0
    // 2.CG enable //PHY AO 284[9:19] = 0 ==>save power
    vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL0, P_Fld(0, MISC_CG_CTRL0_RG_CG_INFRA_OFF_DISABLE) 
        | P_Fld(0, MISC_CG_CTRL0_RG_CG_IDLE_SYNC_EN)
        | P_Fld(0, MISC_CG_CTRL0_RG_CG_RX_COMB1_OFF_DISABLE)
        | P_Fld(0, MISC_CG_CTRL0_RG_CG_RX_COMB0_OFF_DISABLE)
        | P_Fld(0, MISC_CG_CTRL0_RG_CG_RX_CMD_OFF_DISABLE)
        | P_Fld(0, MISC_CG_CTRL0_RG_CG_COMB1_OFF_DISABLE)
        | P_Fld(0, MISC_CG_CTRL0_RG_CG_COMB0_OFF_DISABLE)
        | P_Fld(0, MISC_CG_CTRL0_RG_CG_CMD_OFF_DISABLE)
        | P_Fld(0, MISC_CG_CTRL0_RG_CG_COMB_OFF_DISABLE)
        | P_Fld(0, MISC_CG_CTRL0_RG_CG_PHY_OFF_DIABLE)
        | P_Fld(0, MISC_CG_CTRL0_RG_CG_DRAMC_OFF_DISABLE)
    );
    // 3.mem_dcm ==>save power
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL2, 0x83e003BE, MISC_CG_CTRL2_RG_MEM_DCM_CTL);
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL2, 0x83e003BF, MISC_CG_CTRL2_RG_MEM_DCM_CTL);
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL2, 0x83e003BE, MISC_CG_CTRL2_RG_MEM_DCM_CTL);

    *((volatile unsigned int*)DRAMC_WBR) = u4WbrBackup;
    return;
}

U32 TestEngine2Test(DRAMC_CTX_T * p)
{
    U32 err_count = 0;
    U32 pass_count = 0; 
    U32 u4ErrorValue = 0;
    U8 u1ChannelIdx, u1RankIdx, u1RankMax;

    for(u1ChannelIdx =0; u1ChannelIdx<CHANNEL_NUMBER; u1ChannelIdx++)

    {  
        if((p->support_channel_num == CHANNEL_SINGLE) && (u1ChannelIdx>0))
            break; //Skip channel B
        
        p->channel = u1ChannelIdx;

        #if DUAL_RANK_ENABLE
        if(p->support_rank_num == RANK_DUAL)
            u1RankMax = RANK_MAX;
        else
        #endif
             u1RankMax =RANK_1;

        for(u1RankIdx = 0 ; u1RankIdx<u1RankMax; u1RankIdx++)
        {
            u4ErrorValue = TestEngineCompare(p);
            
            if(u4ErrorValue>0)
            {
                err_count++;
                mcSHOW_DBG_MSG(("channel(%d) Rank(%d), TA2 failed, pass_cnt:%d,err_cnt:%d, u4ErrorValue 0x%x\n", u1ChannelIdx, u1RankIdx, pass_count,err_count, u4ErrorValue));
            }
            else
            {
                pass_count++;
                mcSHOW_DBG_MSG(("channel (%d) Rank(%d),TA2 pass, pass_cnt:%d,err_cnt:%d\n", u1ChannelIdx, u1RankIdx, pass_count,err_count));
            }        
        }
    }

    return err_count; // return 0 -> TA2 pass ; otherwise -> TA2 fail
}

#if 0
void call_init_dram_again(void)
{
    int index;
    EMI_SETTINGS *emi_set;
    DRAM_INFO_BY_MRR_T DramInfo;

    printf("###call_init_dram_again \n");
    emi_setting_index = 0;
    emi_set = &emi_settings[emi_setting_index];
    printf("DdrPhySetting_Everest_LP3()+DramcSetting_Everest_LP3() emi_setting_index:%d \n", emi_setting_index);

#if ENABLE_MRR_AFTER_FIRST_K
    ReInit_DRAM((emi_set->type & 0xF), emi_set->dram_cbt_mode, &DramInfo);
#endif
    ReInit_DRAM((emi_set->type & 0xF), emi_set->dram_cbt_mode, NULL);
}
#endif

#if !__ETT__
// Preloader
// scy: workaround for ddr reserve mode
void DDRPhyFreqMeter_1()
{
    U32 u4CLK_CFG_20 = *((volatile unsigned int *)(0x10210210));
    U32 u4CLK_CFG_21 = *((volatile unsigned int *)(0x10210214));
    U32 u4CLK_CFG_11 = *((volatile unsigned int *)(0x102101B0));
    U32 u4CLK26CALI_0 = *((volatile unsigned int *)(0x10210520));
    U8  u1CLKMeterSel;	//CLK_CFG_20[14]
    U8  u1CLKMuxSel;	//CLK_CFG_20[13:8] or CLK_CFG_20[12:8]
    U32 u4CalCnt;
    U32 u4Freq;


    mcSHOW_DBG_MSG(("DDRPhyFreqMeter...\n"));

    //AD_MPLL_208M_CK
    u1CLKMeterSel = 0x1;
    u1CLKMuxSel = 39;
    *((volatile unsigned int *)(0x10210210)) = (u4CLK_CFG_20 & 0xFFFF80FF) | (0x01 << 31) | (u1CLKMuxSel << 8);
    *((volatile unsigned int *)(0x10210414)) = 0x0;
    *((volatile unsigned int *)(0x10210520)) = 0x00000081;
    while((*((volatile unsigned int *)(0x10210520)) & 0x00000001)!=0) { mcDELAY_US(1); }
    u4CalCnt = *((volatile unsigned int *)(0x10210524)) & 0x0000FFFF;
    u4Freq = (u4CalCnt * 26) / 1024;
    mcSHOW_DBG_MSG(("AD_MPLL_CK FREQUENCY=%d\n", u4Freq));

    u1CLKMeterSel = 0x0;
    u1CLKMuxSel = 46;
    *((volatile unsigned int *)(0x102101b0)) = (u4CLK_CFG_11 & 0xFFFFF0FF) | (0x2 << 8);
    *((volatile unsigned int *)(0x10210214)) = (u4CLK_CFG_21 & 0x7F80FFFF) | (u1CLKMeterSel << 22) | (u1CLKMuxSel << 16);
    *((volatile unsigned int *)(0x10210414)) = 0x0;
    *((volatile unsigned int *)(0x10210520)) = 0x00000090;
    while((*((volatile unsigned int *)(0x10210520)) & 0x00000010)!=0) { mcDELAY_US(1); }
    u4CalCnt = *((volatile unsigned int *)(0x10210528)) & 0x0000FFFF;
    u4Freq = u4CalCnt*26/1024;
    mcSHOW_DBG_MSG(("CH-A_hf_femi_ck FREQUENCY=%d\n", u4Freq));

}

void Reg_Sync_Writel(U32 addr, unsigned int val)
{
	(*(volatile unsigned int *)(addr)) = val;
	mcSHOW_DBG_MSG(("@@@ Write Reg:0x%X Value:0x%X\n", addr, val));
	dsb();
	return;
}

unsigned int Reg_Readl(U32 addr)
{
	return (*(volatile unsigned int *)(addr));
}

#define  INFRA_TOPAXI_PROTECTEN_1_CLR	    (0x100002AC)
#define  INFRA_TOPAXI_PROTECTEN_1_SET       (0x100002A8)
#define  DPY_CH0_PROT_BIT_MASK              (0x1 << 0)
#define  DPY_CH0_PWR_CON                    (0x10B00318)
#define  MOCMOS_PWR_RST_B                   (0x1 << 0)
#define  MOCMOS_PWR_ISO                     (0x1 << 1)                       
#define  MOCMOS_DPY_CH0_SRAM_PDN            (0xF << 8)
#define  MOCMOS_DPY_CH0_SRAM_PDN_ACK        (0x1 << 12)
#define  MOCMOS_PWR_CLK_DIS                 (0x1 << 4)
#define  MOCMOS_DPY_CH0_SRAM_PDN_ACK_BIT0   (0x1 << 12)
//#define IGNORE_MTCMOS_CHECK
int spm_mtcmos_ctrl_dpy_ch0_apmcu(int state)
{
    int err = 0;
    U32 u4value = 0;
    
    if (state == 0) 
    {   //! Power Down
        /* TINFO="Start to turn off DPY_CH0" */
        /* TINFO="Set bus protect" */
        //!mazar   spm_write_apmcu( INFRA_TOPAXI_PROTECTEN_1_SET,  DPY_CH0_PROT_BIT_MASK);
        Reg_Sync_Writel(INFRA_TOPAXI_PROTECTEN_1_SET,  DPY_CH0_PROT_BIT_MASK);
#ifndef IGNORE_MTCMOS_CHECK
        //!     while ((spm_read_apmcu( INFRA_TOPAXI_PROTECTSTA1_1) &  DPY_CH0_PROT_BIT_ACK_MASK) !=  DPY_CH0_PROT_BIT_ACK_MASK) {
        //!     }
#endif
        /* TINFO="Set SRAM_PDN = 1" */
        u4value = Reg_Readl(DPY_CH0_PWR_CON);
        Reg_Sync_Writel(DPY_CH0_PWR_CON, u4value | (MOCMOS_DPY_CH0_SRAM_PDN));
#ifndef IGNORE_MTCMOS_CHECK
        /* TINFO="Wait until  DPY_CH0_SRAM_PDN_ACK = 1" */
        u4value = Reg_Readl(DPY_CH0_PWR_CON);
        while ((u4value & MOCMOS_DPY_CH0_SRAM_PDN_ACK) !=  MOCMOS_DPY_CH0_SRAM_PDN_ACK) 
        {
            u4value = Reg_Readl(DPY_CH0_PWR_CON);
        }
#endif
        /* TINFO="Set  PWR_ISO = 1" */
        u4value = Reg_Readl(DPY_CH0_PWR_CON);
        Reg_Sync_Writel(DPY_CH0_PWR_CON, u4value | MOCMOS_PWR_ISO);
        /* TINFO="Set  PWR_CLK_DIS = 1" */
        u4value = Reg_Readl(DPY_CH0_PWR_CON);
        Reg_Sync_Writel(DPY_CH0_PWR_CON, u4value | MOCMOS_PWR_CLK_DIS);
        /* TINFO="Set  PWR_RST_B = 0" */
        u4value = Reg_Readl(DPY_CH0_PWR_CON);
        Reg_Sync_Writel(DPY_CH0_PWR_CON, u4value & ~MOCMOS_PWR_RST_B);
//! WE1 no pwr control		/* TINFO="Set  PWR_ON = 0" */
//! WE1 no pwr control		 spm_write_apmcu( DPY_CH0_PWR_CON, spm_read_apmcu( DPY_CH0_PWR_CON) & ~PWR_ON);
//! WE1 no pwr control		/* TINFO="Set  PWR_ON_2ND = 0" */
//! WE1 no pwr control		 spm_write_apmcu( DPY_CH0_PWR_CON, spm_read_apmcu( DPY_CH0_PWR_CON) & ~PWR_ON_2ND);
//! WE1 no pwr control#ifndef IGNORE_MTCMOS_CHECK
//! WE1 no pwr control		/* TINFO="Wait until  PWR_STATUS = 0 and  PWR_STATUS_2ND = 0" */	
//! WE1 no pwr control		while ((spm_read_apmcu( PWR_STATUS) &  DPY_CH0_PWR_STA_MASK)
//! WE1 no pwr control		       || (spm_read_apmcu( PWR_STATUS_2ND) &  DPY_CH0_PWR_STA_MASK)) {
//! WE1 no pwr control				/*  */
//! WE1 no pwr control		}
//! WE1 no pwr control#endif
        /* TINFO="Finish to turn off DPY_CH0" */
    }
    else 
    {    /* STA_POWER_ON  1 */
        /* TINFO="Start to turn on DPY_CH0" */
//! WE1 no pwr control		/* TINFO="Set  PWR_ON = 1" */
//! WE1 no pwr control		 spm_write_apmcu( DPY_CH0_PWR_CON, spm_read_apmcu( DPY_CH0_PWR_CON) |  PWR_ON);
//! WE1 no pwr control		/* TINFO="Set  PWR_ON_2ND = 1" */
//! WE1 no pwr control		 spm_write_apmcu( DPY_CH0_PWR_CON, spm_read_apmcu( DPY_CH0_PWR_CON) |  PWR_ON_2ND);
//! WE1 no pwr control#ifndef IGNORE_MTCMOS_CHECK
//! WE1 no pwr control		/* TINFO="Wait until  PWR_STATUS = 1 and  PWR_STATUS_2ND = 1" */
//! WE1 no pwr control		while (((spm_read_apmcu( PWR_STATUS) &  DPY_CH0_PWR_STA_MASK) !=  DPY_CH0_PWR_STA_MASK)
//! WE1 no pwr control		       || ((spm_read_apmcu( PWR_STATUS_2ND) &  DPY_CH0_PWR_STA_MASK) !=  DPY_CH0_PWR_STA_MASK)) {
//! WE1 no pwr control				/*  */
//! WE1 no pwr control		}
//! WE1 no pwr control#endif
        /* TINFO="Set  PWR_CLK_DIS = 0" */
        u4value = Reg_Readl(DPY_CH0_PWR_CON);
        Reg_Sync_Writel(DPY_CH0_PWR_CON, u4value & ~MOCMOS_PWR_CLK_DIS);
        /* TINFO="Set  PWR_ISO = 0" */
        u4value = Reg_Readl(DPY_CH0_PWR_CON);
        Reg_Sync_Writel(DPY_CH0_PWR_CON, u4value & ~MOCMOS_PWR_ISO);

        /* TINFO="Set  PWR_RST_B = 1" */
        u4value = Reg_Readl(DPY_CH0_PWR_CON);
        Reg_Sync_Writel(DPY_CH0_PWR_CON, u4value | MOCMOS_PWR_RST_B);

        /* TINFO="Set SRAM_PDN = 0" */
        u4value = Reg_Readl(DPY_CH0_PWR_CON);
        Reg_Sync_Writel(DPY_CH0_PWR_CON, u4value & ~(0x1 << 8));

#ifndef IGNORE_MTCMOS_CHECK
        /* TINFO="Wait until  DPY_CH0_SRAM_PDN_ACK_BIT0 = 0" */
        u4value = Reg_Readl(DPY_CH0_PWR_CON);
        while (u4value & MOCMOS_DPY_CH0_SRAM_PDN_ACK_BIT0) 
        {
            u4value = Reg_Readl(DPY_CH0_PWR_CON);
        }
#endif
        u4value = Reg_Readl(DPY_CH0_PWR_CON);
        Reg_Sync_Writel(DPY_CH0_PWR_CON, u4value & ~(0x1 << 9));
        u4value = Reg_Readl(DPY_CH0_PWR_CON);
        Reg_Sync_Writel(DPY_CH0_PWR_CON, u4value & ~(0x1 << 10));
        u4value = Reg_Readl(DPY_CH0_PWR_CON);
        Reg_Sync_Writel(DPY_CH0_PWR_CON, u4value & ~(0x1 << 11));
        /* TINFO="Release bus protect" */
        Reg_Sync_Writel(INFRA_TOPAXI_PROTECTEN_1_CLR,  DPY_CH0_PROT_BIT_MASK);
#ifndef IGNORE_MTCMOS_CHECK
        /* Note that this protect ack check after releasing protect has been ignored */
#endif
        /* TINFO="Finish to turn on DPY_CH0" */
    }
    return err;
}
#define SC_26M_CK_OFF               0
#define SC_ARM_CK_OFF_PCM           1
#define SC_MEM_CK_OFF_PCM           2
#define SC_AXI_CK_OFF               3
#define SC_SMI_CK_OFF               4
#define SC_MD26M_CK_OFF             5
#define SC_ARMPLLOUT_OFF_PCM        6
#define SC_MAINPLLOUT_OFF           7
#define SC_MPLL_OFF                 8
#define SC_ARMPLL_OFF_PCM           9
#define SC_MAINPLL_OFF              10
#define SC_CKSQ0_OFF                11
#define SC_CKSQ1_OFF                12
#define SC_MPLL_S_OFF               13
#define SC_ARMDDS_OFF_PCM           14
#define SC_MAINDDS_OFF              15
#define SC_DR_GATE_RETRY_EN_PCM     16
#define SC_DPY_MODE_SW_PCM          17
#define SC_DMSUS_OFF_PCM            18
#define SC_DPY_2ND_DLL_EN_PCM       19
#define SC_DPY_DLL_EN_PCM           20
#define SC_DPY_DLL_CK_EN_PCM        21
#define SC_DPY_VREF_EN_PCM          22
#define SC_PHYPLL_EN_PCM            23
#define SC_DDRPHY_FB_CK_EN_PCM      24
#define SC_DR_SHU_EN_PCM            25
#define SC_PHYPLL_SHU_EN_PCM        26
#define SC_PHYPLL2_SHU_EN_PCM       27
#define SC_PHYPLL_MODE_SW_PCM       28
#define SC_PHYPLL2_MODE_SW_PCM      29
#define SC_DR_SHU_LEVEL_PCM_0       30
#define SC_DR_SHU_LEVEL_PCM_1       31

#define PWRAP_SLEEP_REQ             0
#define EMI_CLK_OFF_REQ_PCM         1
#define PCM_TOP_MAS_PAU_REQ         2
#define SC_DR_SHORT_QUEUE_PCM       3
#define PCM_CK_SEL_0                4
#define PCM_CK_SEL_1                5
#define SPM_PC_TRACE_SW             6
#define FHC_PAUSE_ARM_PCM           7
#define SC_26M_CK_SEL               8
#define PCM_TIMER_SET               9
#define PCM_TIMER_CLR               10
#define SRCVOLTEN                   11
#define CSYSPWRUPACK                12
#define PCM_IM_SLP_EN               13
#define SRCCLKENO_0                 14
#define SYSRST                      15
#define MD_APSRC_ACK_0              16
#define CPU_SYS_TIMER_CLK_SEL       17
#define SC_AXI_DCM_DIS              18
#define FHC_PAUSE_MAIN              19
#define FHC_PAUSE_MEM               20
#define SRCCLKENO_1                 21
#define PCM_WDT_KICK_P              22
#define PCM_TWAM_EVENT_CLR          23
#define WAKEUP_EXT_W_SEL            24
#define WAKEUP_EXT_R_SEL            25
#define PMIC_IRQ_REQ_EN             26
#define FHC_PAUSE_MPLL              27
#define FORCE_F26M_WAKE             28
#define FORCE_APSRC_WAKE            29
#define FORCE_INFRA_WAKE            30
#define FORCE_VRF18_WAKE            31

#define DRAMC_DPY_CLK_SPM_CON        (0x10B00498) 
//-- DRAMC_DPY_CLK_SPM_CON
#define PCM_DPY_BCLK_ENABLE         0
#define PCM_DPY_SHU_RESTORE         1
#define PCM_DPY_RESERVED_0          2
#define PCM_DPY_RESERVED_1          3

static void FUNC_DDRPHY_INFRA_MTCMOS_OFF_EXIT(DRAMC_CTX_T *p)
{
    U32 u4value = 0;
    /*TINFO="DRAM : start from S0 to S1!!"*/
    /*TINFO="DRAM : DPY power on, DRAMC_DPY_CLK_CON_SEL2[30:28] = 3'b111, means turn off CHA, BCD will also turn on  "*/ 
    spm_mtcmos_ctrl_dpy_ch0_apmcu(1);

    ///TODO: (Step 7) MPLL on
    //MPLL on
    /*TINFO="DRAM : set sc_mpll_off = 1"*/
    u4value = Reg_Readl(WHITNEY_SPM_POWER_ON_VAL0);
    Reg_Sync_Writel(WHITNEY_SPM_POWER_ON_VAL0, u4value & ~(1U << SC_MPLL_OFF));  
    
    //MPLL_S on
    /*TINFO="DRAM : set sc_mpll_s_off = 1"*/
    u4value = Reg_Readl(WHITNEY_SPM_POWER_ON_VAL0);
    Reg_Sync_Writel(WHITNEY_SPM_POWER_ON_VAL0, u4value & ~(1U << SC_MPLL_S_OFF));

    //wait 20us
    //r14 = #0x201
    ///TODO: (Step 8) wait 20 us
    mcDELAY_US(20);

    //r0 = r0 | r1 << PDEF_SC_DR_GATE_RETRY_EN_LSB
    ///TODO: (Step 9) set DQS gating retry enable
    //Lewis@20160408: Remove set DQS gating retry setting in low power scenario, since it has chance to retry all the time after SR without dummy read
    /*TINFO="DRAM : set sc_dr_gate_retry_en = 1"*/
    //u4value = Reg_Readl(WHITNEY_SPM_POWER_ON_VAL0);
    //Reg_Sync_Writel(WHITNEY_SPM_POWER_ON_VAL0, u4value | (1U << SC_DR_GATE_RETRY_EN_PCM));


    ///TODO: (Step 9-1 ~ 9-7)
    /*TINFO="DRAM : set sc_dpy_bclk_en = 1"*/
    u4value = Reg_Readl(DRAMC_DPY_CLK_SPM_CON);
    Reg_Sync_Writel(DRAMC_DPY_CLK_SPM_CON, u4value | (1U << PCM_DPY_BCLK_ENABLE));

    //wait 80ns
    //r14 = #PDEF_PAUSE_26CK_WAIT_114N
    mcDELAY_US(1);

    /*TINFO="DRAM : set sc_shu_restore_en = 1"*/
    u4value = Reg_Readl(DRAMC_DPY_CLK_SPM_CON);
    Reg_Sync_Writel(DRAMC_DPY_CLK_SPM_CON, u4value | (1U << PCM_DPY_SHU_RESTORE));

    mcDELAY_US(1);

    /*TINFO="DRAM : set sc_shu_restore_en = 0"*/
    u4value = Reg_Readl(DRAMC_DPY_CLK_SPM_CON);
    Reg_Sync_Writel(DRAMC_DPY_CLK_SPM_CON, u4value & ~(1U << PCM_DPY_SHU_RESTORE));

    mcDELAY_US(1);

    /*TINFO="DRAM : set sc_dpy_bclk_en = 0"*/
    u4value = Reg_Readl(DRAMC_DPY_CLK_SPM_CON);
    Reg_Sync_Writel(DRAMC_DPY_CLK_SPM_CON, u4value & ~(1U << PCM_DPY_BCLK_ENABLE));

    /*TINFO="DRAM : set sc_phypll_en = 1"*/ //confirmed
    //u4value = Reg_Readl(WHITNEY_SPM_POWER_ON_VAL0);
    //Reg_Sync_Writel(WHITNEY_SPM_POWER_ON_VAL0, u4value | (1U << SC_PHYPLL_EN_PCM));

    u4value = Reg_Readl(SPM_SW_RSV_5);
    if(u4value & (0x01))//PHY_PLL
    {
        //u4PhyPLL = 1;   
        u4value = Reg_Readl(WHITNEY_SPM_POWER_ON_VAL0);
        Reg_Sync_Writel(WHITNEY_SPM_POWER_ON_VAL0, u4value | (1U << SC_PHYPLL_MODE_SW_PCM));  
    }
    else
    {
        //u4PhyPLL = 0; 
        u4value = Reg_Readl(WHITNEY_SPM_POWER_ON_VAL0);
        Reg_Sync_Writel(WHITNEY_SPM_POWER_ON_VAL0, u4value | (1U << SC_PHYPLL2_MODE_SW_PCM));
    }
    //ucstatus |= Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL0, u4value | (1U << PDEF_SC_PHYPLL_EN));

    ///TODO: (Step 11) wait 20us
    mcDELAY_US(20);
    /*TINFO="DRAM : finish from S0 to S1!!"*/
    return;
}

static void FUNC_DDRPHY_INFRA_MTCMOS_OFF_ENTER(DRAMC_CTX_T *p)
{
    U32 u4value = 0;
    ///TODO: (Step 2) PHY PLL off {PHYPLL && CLRPLL}
    /*TINFO="DRAM : set pll = 0"*/
    u4value = Reg_Readl(SPM_SW_RSV_5);
    if(u4value & (0x01))//PHY_PLL
    {
        u4value = Reg_Readl(WHITNEY_SPM_POWER_ON_VAL0);
        Reg_Sync_Writel(WHITNEY_SPM_POWER_ON_VAL0, u4value & ~(1U << SC_PHYPLL_MODE_SW_PCM));  
    }
    else
    {
        u4value = Reg_Readl(WHITNEY_SPM_POWER_ON_VAL0);
        Reg_Sync_Writel(WHITNEY_SPM_POWER_ON_VAL0, u4value & ~(1U << SC_PHYPLL2_MODE_SW_PCM));
    }
    /*TINFO="DRAM : set sc_mpll_s_off = 1"*/
    u4value = Reg_Readl(WHITNEY_SPM_POWER_ON_VAL0);
    Reg_Sync_Writel(WHITNEY_SPM_POWER_ON_VAL0, u4value | (1U << SC_MPLL_S_OFF));

    /*TINFO="DRAM : set sc_mpll_off = 1"*/
    u4value = Reg_Readl(WHITNEY_SPM_POWER_ON_VAL0);
    Reg_Sync_Writel(WHITNEY_SPM_POWER_ON_VAL0, u4value | (1U << SC_MPLL_OFF));

    /*TINFO="DRAM : DPY power down, DRAMC_DPY_CLK_CON_SEL2[30:28] = 3'b111, means turn off CHA, BCD will also turn off  "*/ 
    spm_mtcmos_ctrl_dpy_ch0_apmcu(0);
    /*TINFO="DRAM : finished from S1 to S0!!"*/

    mcDELAY_US(10);
}

static void FUNC_DDRPHY_DLL_OFF_INFRA_CG_EN_ENTER(void)
{
    U32 u4value = 0;

    ///TODO: (Step 6) Pause frequency hopping; {no freq hopping}
    //u4value = Reg_Readl(PDEF_SPM_POWER_ON_VAL1);
    //ucstatus |= Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL1, u4value | (1U << PDEF_FREQH_PAUSE_MEM_LSB));
    ///TODO: {Lewis: switch to SPM low power mode}
    ///TODO: (Step 7) DDRPHY switch to SPM control
    /*TINFO="DRAM : set sc_dpy_mode_sw = 1"*/
    u4value = Reg_Readl(WHITNEY_SPM_POWER_ON_VAL0);
    Reg_Sync_Writel(WHITNEY_SPM_POWER_ON_VAL0, u4value | (1U << SC_DPY_MODE_SW_PCM));

    ///TODO: (Step 8) IO enter suspend{tie to specific value, SR state}
    /*TINFO="DRAM : set sc_dmsus_off = 1"*/
    u4value = Reg_Readl(WHITNEY_SPM_POWER_ON_VAL0);
    Reg_Sync_Writel(WHITNEY_SPM_POWER_ON_VAL0, u4value | (1U << SC_DMSUS_OFF_PCM));

    ///TODO: (Step 9) CG enable of mem_ck{dcm clk off}
    /*TINFO="DRAM : set sc_mem_ck_off = 1"*/
    u4value = Reg_Readl(WHITNEY_SPM_POWER_ON_VAL0);
    Reg_Sync_Writel(WHITNEY_SPM_POWER_ON_VAL0, u4value | (1U << SC_MEM_CK_OFF_PCM));

    ///TODO: (Step 10) DLL disable
    /*TINFO="DRAM : set sc_dpy_2nd_dll_en = 0"*/    
    u4value = Reg_Readl(WHITNEY_SPM_POWER_ON_VAL0);
    Reg_Sync_Writel(WHITNEY_SPM_POWER_ON_VAL0, u4value & ~(1U << SC_DPY_2ND_DLL_EN_PCM));

    /*TINFO="DRAM : set sc_dpy_dll_en = 0"*/
    u4value = Reg_Readl(WHITNEY_SPM_POWER_ON_VAL0);
    Reg_Sync_Writel(WHITNEY_SPM_POWER_ON_VAL0, u4value & ~(1U << SC_DPY_DLL_EN_PCM));

    ///TODO: (Step 11) All PI disable & CG enable{Aphy cg enable, Not do PI delay Calculation}
     /*TINFO="DRAM : set sc_dpy_dll_ck_en = 0"*/
    u4value = Reg_Readl(WHITNEY_SPM_POWER_ON_VAL0);
    Reg_Sync_Writel(WHITNEY_SPM_POWER_ON_VAL0, u4value & ~(1U << SC_DPY_DLL_CK_EN_PCM));

    ///TODO: (Step 12) VREF disable, MCK8X CG enable and  MIDPI disable{MIDPI: PI of DLL }
    /*TINFO="DRAM : set sc_dpy_vref_en = 0"*/
    u4value = Reg_Readl(WHITNEY_SPM_POWER_ON_VAL0);
    Reg_Sync_Writel(WHITNEY_SPM_POWER_ON_VAL0, u4value & ~(1U << SC_DPY_VREF_EN_PCM));

    ///TODO: (Step 13) set DQS gating retry disable
    /*TINFO="DRAM : set sc_dr_gate_retry_en = 0"*/
    //u4value = Reg_Readl(WHITNEY_SPM_POWER_ON_VAL0);
    //Reg_Sync_Writel(WHITNEY_SPM_POWER_ON_VAL0, u4value & ~(1U << SC_DR_GATE_RETRY_EN_PCM));

    return;
}

static void FUNC_DDRPHY_DLL_OFF_INFRA_CG_EN_EXIT(DRAMC_CTX_T *p)
{
    U32 u4value = 0;

    /*TINFO="DRAM : start from S1 to Sidle!!"*/
    ///TODO: (Step 1)  VREF enable MCK8X CG disable MIDPI enable
    /*TINFO="DRAM : set sc_dpy_vref_en = 1"*/
    u4value = Reg_Readl(WHITNEY_SPM_POWER_ON_VAL0);
    Reg_Sync_Writel(WHITNEY_SPM_POWER_ON_VAL0, u4value | (1U << SC_DPY_VREF_EN_PCM));
    
    ///TODO: (Step 2)  All PI enable & CG disable
    /*TINFO="DRAM : set sc_dpy_dll_ck_en = 1"*/
    u4value = Reg_Readl(WHITNEY_SPM_POWER_ON_VAL0);
    Reg_Sync_Writel(WHITNEY_SPM_POWER_ON_VAL0, u4value | (1U << SC_DPY_DLL_CK_EN_PCM));
    
    ///TODO: (Step 3)  set TOP feedback MCK to full frequency
    /*TINFO="DRAM : set ddrphy_fb_ck_en=1"*/
    u4value = Reg_Readl(WHITNEY_SPM_POWER_ON_VAL0);
    Reg_Sync_Writel(WHITNEY_SPM_POWER_ON_VAL0, u4value | (1U << SC_DDRPHY_FB_CK_EN_PCM));
    
    ///TODO: (Step 4) 1st DLL enable
    /*TINFO="DRAM : set sc_dpy_dll_en = 1"*/
    u4value = Reg_Readl(WHITNEY_SPM_POWER_ON_VAL0);
    Reg_Sync_Writel(WHITNEY_SPM_POWER_ON_VAL0, u4value | (1U << SC_DPY_DLL_EN_PCM));
    
    //wait 350 ns
    //r14 = #0x0A
    ///TODO: (Step 5) wait 150ns
    mcDELAY_US(1);
    
    ///TODO: (Step 6)  2nd DLL enable
    /*TINFO="DRAM : set sc_dpy_2nd_dll_en = 1"*/ 
    u4value = Reg_Readl(WHITNEY_SPM_POWER_ON_VAL0);
    Reg_Sync_Writel(WHITNEY_SPM_POWER_ON_VAL0, u4value | (1U << SC_DPY_2ND_DLL_EN_PCM));
    
    //wait 350 ns
    //r14 = #0x0A
    ///TODO: (Step 7)  wait 150ns
    mcDELAY_US(1);
    
    ///TODO: {Lewis: switch to SPM normal mode}
    //r0 = r0 & ~r1 << PDEF_SC_DPY_MODE_SW
    ///TODO: (Step 8)  DDRPHY switch to DRAMC control
    /*TINFO="DRAM : set sc_dpy_mode_sw = 0"*/
    u4value = Reg_Readl(WHITNEY_SPM_POWER_ON_VAL0);
    Reg_Sync_Writel(WHITNEY_SPM_POWER_ON_VAL0, u4value & ~(1U << SC_DPY_MODE_SW_PCM));
    
    //mem_ck CG disable, need double check signal width 
    //r0 = r0 & ~r1 << PDEF_SC_MEM_CK_OFF_LSB
    ///TODO: (Step 9)  mem_ck CG disable
    /*TINFO="DRAM : set sc_mem_ck_off = 0"*/
    u4value = Reg_Readl(WHITNEY_SPM_POWER_ON_VAL0);
    Reg_Sync_Writel(WHITNEY_SPM_POWER_ON_VAL0, u4value & ~(1U << SC_MEM_CK_OFF_PCM));
    
    //set TOP feedback MCK to divided frequency
    //r0 = r0 & ~r1 << PDEF_SC_DDRPHY_FB_CK_EN
    ///TODO: (Step 10)  set TOP feedback MCK to divided frequency
    /*TINFO="DRAM : set ddrphy_fb_ck_en=0"*/
    u4value = Reg_Readl(WHITNEY_SPM_POWER_ON_VAL0);
    Reg_Sync_Writel(WHITNEY_SPM_POWER_ON_VAL0, u4value & ~(1U << SC_DDRPHY_FB_CK_EN_PCM));
    
    //IO exit suspend, need double check signal width
    //r14 = #0x04
    ///TODO: (Step 11)  wait 150ns
    mcDELAY_US(1);
    
    //r0 = r0 & ~r1 << PDEF_SC_DMSUS_OFF
    ///TODO: (Step 12)  IO exit suspend
    /*TINFO="DRAM : set sc_dmsus_off = 0"*/
    u4value = Reg_Readl(WHITNEY_SPM_POWER_ON_VAL0);
    Reg_Sync_Writel(WHITNEY_SPM_POWER_ON_VAL0, u4value & ~(1U << SC_DMSUS_OFF_PCM));
    
    //release frequency hopping, need double check method !!
    //r7 = r7 & ~r1 << PDEF_FREQH_PAUSE_MEM_LSB //Enhao_TBD freq pause may do two times
    ///TODO: (Step 13)  release frequency hopping 
    //Ask DE
    //Lewis@20160408: Remove freq hopping flow in low power scenario
    //u4value = Reg_Readl(PDEF_SPM_POWER_ON_VAL1);
    //ucstatus |= Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL1, u4value & ~(1U << PDEF_FREQH_PAUSE_MEM_LSB));
    Func_Emi_On();

    //r0 = r0 & ~r1 << PDEF_SC_DR_GATE_RETRY_EN_LSB
    ///TODO: (Step 19)  set DQS gating retry disable
    //Lewis@20160408: Remove set DQS gating retry setting in low power scenario, since it has chance to retry all the time after SR without dummy read
    /*TINFO="DRAM : set sc_dr_gate_retry_en = 0"*/
    //u4value = Reg_Readl(WHITNEY_SPM_POWER_ON_VAL0);
    //Reg_Sync_Writel(WHITNEY_SPM_POWER_ON_VAL0, u4value & ~(1U << SC_DR_GATE_RETRY_EN_PCM));
    return;
}
#endif

#define WHITNEY_SPM_POWER_ON_VAL1    (0x10B00008)
#define DRAMC_DPY_CLK_SW_CON2        (0x10B0046C)
#define EMI_ACK 0xF
#define EMI_CLK_OFF_REQ_PCM         1
void Func_Emi_On(void)
{
    U32 u4value = 0;

    ///TODO: (Step 14)  release self-refresh request
    /*TINFO="DRAM : set emi_clk_off_req = 0"*/
    u4value = Reg_Readl(WHITNEY_SPM_POWER_ON_VAL1);
    Reg_Sync_Writel(WHITNEY_SPM_POWER_ON_VAL1, u4value & ~(1U << EMI_CLK_OFF_REQ_PCM));


    /*TINFO="DRAM : wait emi_clk_off_req_ack == 4'h0"*/
    while(1)//Exit the loop until SR ack is 0
    {
        mcDELAY_US(1);
        ///TODO: (Step 18)  wait exit self-refresh ack
        u4value = Reg_Readl(DRAMC_DPY_CLK_SW_CON2);
        if((EMI_ACK & (u4value >> 16)) == 0)
        {
            break;
        }
        else
        {
            mcSHOW_DBG_MSG(("!Wait no ack [not 0]\n"));
        }
    }
}

void Func_Emi_Off(void)
{
    int i = 0;
    U32 u4value = 0;

    //r4 = r1 << 3  //8 * 4 * 26M clock ~= 1.2us
    //r7 = r7 | r1<<PDEF_EMI_CLK_OFF_REQ_LSB
    ///TODO: (Step 3) request self-refresh
    u4value = Reg_Readl(WHITNEY_SPM_POWER_ON_VAL1);
    Reg_Sync_Writel(WHITNEY_SPM_POWER_ON_VAL1, u4value | (1U << EMI_CLK_OFF_REQ_PCM));

    //r14 = #PDEF_PAUSE_26CK_WAIT_1P2U
    //Note: Could be reduce to 1u/380n
    //LABEL CHECK_EMI_OFF_ACK
    //!r4 ? &(#EMI_OFF_ABORT)
    //r3 = r1 & ~r13 >> PDEF_EMI_CLK_OFF_ACK_LSB
    //r3 ? &(#CHECK_EMI_OFF_ACK)
    //r4 = r4 - r1
    for(i=2; i>0; i--)
    {
        mcDELAY_US(1);
        u4value = Reg_Readl(DRAMC_DPY_CLK_SW_CON2);
        if((EMI_ACK& (u4value >> 16)) == EMI_ACK)
        {
            break;
        }
    }
    // if emi_clk_off_ack doesn't response, r3 will be set to 1'b1
    //   1) set R7[EMI_CLK_OFF_REQ_LSB] == 0
    //   2) fire CPU_WAKEUP_EVENT and then assert
    //LABEL EMI_OFF_ABORT
    //r7 = r7 & ~r3 << PDEF_EMI_CLK_OFF_REQ_LSB
    ///TODO: (Step 5) wait self-refresh ack
    if(i==0) //no issue Ack =>status = 1
    {
        mcSHOW_DBG_MSG(("!Wait no ack\n"));
        u4value = Reg_Readl(WHITNEY_SPM_POWER_ON_VAL1);
        Reg_Sync_Writel(WHITNEY_SPM_POWER_ON_VAL1, u4value & ~(1U << EMI_CLK_OFF_REQ_PCM));//Set to 0      
    }
    return;
}

//void DramcDFSDirectJump(DRAMC_CTX_T *p, U8 shu_level);
void Dramc_DDR_Reserved_Mode_AfterSR(void)//Must be 4 channel
{
#if 1
    DRAMC_CTX_T *p = &DramCtx_LPDDR4;
    int i = 0;
    U32 u4WbrBackup = 0;
    u4WbrBackup = *((volatile unsigned int*)DRAMC_WBR);
    DRAM_CHANNEL_T channel_bak = p->channel;

#if DUAL_FREQ_K
    for(i=0; i<4; i++)
    {
        switch(i)
        {
            case 0:
                p->channel = CHANNEL_A;
                break;
            case 1:
                p->channel = CHANNEL_B;
                break;
            case 2:
                p->channel = CHANNEL_C;
                break;
            case 3:
                p->channel = CHANNEL_D;
                break;
            default:
                mcSHOW_DBG_MSG(("! Error at reserve mode\n"));
        }
        DramcModeRegWriteByRank(p, 0, 13, 0xd8);
        DramcModeRegWriteByRank(p, 1, 13, 0xd8);
        DramcModeRegWriteByRank(p, 0, 2, 0x2d);
        DramcModeRegWriteByRank(p, 1, 2, 0x2d);
    }
#endif

#if 0
    {   // LPDDR4
        /*TINFO="Setting LP4 WL/RL "*/
        //!//! MR2 for R0 3200 RL/WL
        //!*((UINT32P)(DRAMC0_AO_BASE + 0x005c)) = (0x2d << 0) | (0x2 << 8) | (0x0 << 24) | (0x1 << 31);
        //!//! MRW trigger
        //!*((UINT32P)(DRAMC0_AO_BASE + 0x0060)) |= (1 << 0) ;
        //!//! polling done (nao + 0x88[0])
        //!while((*((UINT32P)(DRAMC0NAO_BASE + 0x0088)) & 0x1) == 0 );
        //!//! MRW trigger disable
        //!*((UINT32P)(DRAMC0_AO_BASE + 0x0060)) &= ~(1 << 0) ;
        //!//! MR2 for R1 3200 RL/WL
        //!*((UINT32P)(DRAMC0_AO_BASE + 0x005c)) = (0x2d << 0) | (0x2 << 8) | (0x1 << 24) | (0x1 << 31);
        //!//! MRW trigger
        //!*((UINT32P)(DRAMC0_AO_BASE + 0x0060)) |= (1 << 0) ;
        //!//! polling done (nao + 0x88[0])
        //!while((*((UINT32P)(DRAMC0NAO_BASE + 0x0088)) & 0x1) == 0 );
        //!//! MRW trigger disable
        //!*((UINT32P)(DRAMC0_AO_BASE + 0x0060)) &= ~(1 << 0) ;
    
        /*TINFO="Setting LP4 FSP-OP"*/
        //! MR13 for R0 [CHA]
        *((volatile unsigned int*)(DRAMC0_AO_BASE + 0x005c)) = (0xc0 << 0) | (0xd << 8) | (0x0 << 24) | (0x1 << 31);
        //! MRW trigger
        *((volatile unsigned int*)(DRAMC0_AO_BASE + 0x0060)) |= (1 << 0) ;
        //! polling done (nao + 0x88[0])
        while((*((volatile unsigned int*)(DRAMC0NAO_BASE + 0x0088)) & 0x1) == 0 );
        //! MRW trigger disable
        *((volatile unsigned int*)(DRAMC0_AO_BASE + 0x0060)) &= ~(1 << 0) ;
        //! MR13 for R1 
        *((volatile unsigned int*)(DRAMC0_AO_BASE + 0x005c)) = (0xc0 << 0) | (0xd << 8) | (0x1 << 24) | (0x1 << 31);
        //! MRW trigger
        *((volatile unsigned int*)(DRAMC0_AO_BASE + 0x0060)) |= (1 << 0) ;
        //! polling done (nao + 0x88[0])
        while((*((volatile unsigned int*)(DRAMC0NAO_BASE + 0x0088)) & 0x1) == 0 );
        //! MRW trigger disable
        *((volatile unsigned int*)(DRAMC0_AO_BASE + 0x0060)) &= ~(1 << 0) ;
    
        //! MR13 for R0 [CHB]
        *((volatile unsigned int*)(DRAMC1_AO_BASE + 0x005c)) = (0xc0 << 0) | (0xd << 8) | (0x0 << 24) | (0x1 << 31);
        //! MRW trigger
        *((volatile unsigned int*)(DRAMC1_AO_BASE + 0x0060)) |= (1 << 0) ;
        //! polling done (nao + 0x88[0])
        while((*((volatile unsigned int*)(DRAMC1NAO_BASE + 0x0088)) & 0x1) == 0 );
        //! MRW trigger disable
        *((volatile unsigned int*)(DRAMC1_AO_BASE + 0x0060)) &= ~(1 << 0) ;
        //! MR13 for R1 
        *((volatile unsigned int*)(DRAMC1_AO_BASE + 0x005c)) = (0xc0 << 0) | (0xd << 8) | (0x1 << 24) | (0x1 << 31);
        //! MRW trigger
        *((volatile unsigned int*)(DRAMC1_AO_BASE + 0x0060)) |= (1 << 0) ;
        //! polling done (nao + 0x88[0])
        while((*((volatile unsigned int*)(DRAMC1NAO_BASE + 0x0088)) & 0x1) == 0 );
        //! MRW trigger disable
        *((volatile unsigned int*)(DRAMC1_AO_BASE + 0x0060)) &= ~(1 << 0) ;
    
        //! MR13 for R0 [CHC]
        *((volatile unsigned int*)(DRAMC2_AO_BASE + 0x005c)) = (0xc0 << 0) | (0xd << 8) | (0x0 << 24) | (0x1 << 31);
        //! MRW trigger
        *((volatile unsigned int*)(DRAMC2_AO_BASE + 0x0060)) |= (1 << 0) ;
        //! polling done (nao + 0x88[0])
        while((*((volatile unsigned int*)(DRAMC2NAO_BASE + 0x0088)) & 0x1) == 0 );
        //! MRW trigger disable
        *((volatile unsigned int*)(DRAMC2_AO_BASE + 0x0060)) &= ~(1 << 0) ;
        //! MR13 for R1 
        *((volatile unsigned int*)(DRAMC2_AO_BASE + 0x005c)) = (0xc0 << 0) | (0xd << 8) | (0x1 << 24) | (0x1 << 31);
        //! MRW trigger
        *((volatile unsigned int*)(DRAMC2_AO_BASE + 0x0060)) |= (1 << 0) ;
        //! polling done (nao + 0x88[0])
        while((*((volatile unsigned int*)(DRAMC2NAO_BASE + 0x0088)) & 0x1) == 0 );
        //! MRW trigger disable
        *((volatile unsigned int*)(DRAMC2_AO_BASE + 0x0060)) &= ~(1 << 0) ;
    
        //! MR13 for R0 [CHD]
        *((volatile unsigned int*)(DRAMC3_AO_BASE + 0x005c)) = (0xc0 << 0) | (0xd << 8) | (0x0 << 24) | (0x1 << 31);
        //! MRW trigger
        *((volatile unsigned int*)(DRAMC3_AO_BASE + 0x0060)) |= (1 << 0) ;
        //! polling done (nao + 0x88[0])
        while((*((volatile unsigned int*)(DRAMC3NAO_BASE + 0x0088)) & 0x1) == 0 );
        //! MRW trigger disable
        *((volatile unsigned int*)(DRAMC3_AO_BASE + 0x0060)) &= ~(1 << 0) ;
        //! MR13 for R1 
        *((volatile unsigned int*)(DRAMC3_AO_BASE + 0x005c)) = (0xc0 << 0) | (0xd << 8) | (0x1 << 24) | (0x1 << 31);
        //! MRW trigger
        *((volatile unsigned int*)(DRAMC3_AO_BASE + 0x0060)) |= (1 << 0) ;
        //! polling done (nao + 0x88[0])
        while((*((volatile unsigned int*)(DRAMC3NAO_BASE + 0x0088)) & 0x1) == 0 );
        //! MRW trigger disable
        *((volatile unsigned int*)(DRAMC3_AO_BASE + 0x0060)) &= ~(1 << 0) ;
    
        /*TINFO="Restore CH0 LP4 3200 RL/WL"*/
        //! MR2 for R0 3200 RL/WL [CHA]
        *((volatile unsigned int*)(DRAMC0_AO_BASE + 0x005c)) = (0x2d << 0) | (0x2 << 8) | (0x0 << 24) | (0x1 << 31);
        //! MRW trigger
        *((volatile unsigned int*)(DRAMC0_AO_BASE + 0x0060)) |= (1 << 0) ;
        //! polling done (nao + 0x88[0])
        while((*((volatile unsigned int*)(DRAMC0NAO_BASE + 0x0088)) & 0x1) == 0 );
        //! MRW trigger disable
        *((volatile unsigned int*)(DRAMC0_AO_BASE + 0x0060)) &= ~(1 << 0) ;
        //! MR2 for R1 3200 RL/WL
        *((volatile unsigned int*)(DRAMC0_AO_BASE + 0x005c)) = (0x2d << 0) | (0x2 << 8) | (0x1 << 24) | (0x1 << 31);
        //! MRW trigger
        *((volatile unsigned int*)(DRAMC0_AO_BASE + 0x0060)) |= (1 << 0) ;
        //! polling done (nao + 0x88[0])
        while((*((volatile unsigned int*)(DRAMC0NAO_BASE + 0x0088)) & 0x1) == 0 );
        //! MRW trigger disable
        *((volatile unsigned int*)(DRAMC0_AO_BASE + 0x0060)) &= ~(1 << 0) ;
    
        /*TINFO="Restore CH1 LP4 3200 RL/WL"*/
        //! MR2 for R0 3200 RL/WL [CHB]
        *((volatile unsigned int*)(DRAMC1_AO_BASE + 0x005c)) = (0x2d << 0) | (0x2 << 8) | (0x0 << 24) | (0x1 << 31);
        //! MRW trigger
        *((volatile unsigned int*)(DRAMC1_AO_BASE + 0x0060)) |= (1 << 0) ;
        //! polling done (nao + 0x88[0])
        while((*((volatile unsigned int*)(DRAMC1NAO_BASE + 0x0088)) & 0x1) == 0 );
        //! MRW trigger disable
        *((volatile unsigned int*)(DRAMC1_AO_BASE + 0x0060)) &= ~(1 << 0) ;
        //! MR2 for R1 3200 RL/WL
        *((volatile unsigned int*)(DRAMC1_AO_BASE + 0x005c)) = (0x2d << 0) | (0x2 << 8) | (0x1 << 24) | (0x1 << 31);
        //! MRW trigger
        *((volatile unsigned int*)(DRAMC1_AO_BASE + 0x0060)) |= (1 << 0) ;
        //! polling done (nao + 0x88[0])
        while((*((volatile unsigned int*)(DRAMC1NAO_BASE + 0x0088)) & 0x1) == 0 );
        //! MRW trigger disable
        *((volatile unsigned int*)(DRAMC1_AO_BASE + 0x0060)) &= ~(1 << 0) ;
    
        /*TINFO="Restore CH0 LP4 3200 RL/WL"*/
        //! MR2 for R0 3200 RL/WL [CHC]
        *((volatile unsigned int*)(DRAMC2_AO_BASE + 0x005c)) = (0x2d << 0) | (0x2 << 8) | (0x0 << 24) | (0x1 << 31);
        //! MRW trigger
        *((volatile unsigned int*)(DRAMC2_AO_BASE + 0x0060)) |= (1 << 0) ;
        //! polling done (nao + 0x88[0])
        while((*((volatile unsigned int*)(DRAMC2NAO_BASE + 0x0088)) & 0x1) == 0 );
        //! MRW trigger disable
        *((volatile unsigned int*)(DRAMC2_AO_BASE + 0x0060)) &= ~(1 << 0) ;
        //! MR2 for R1 3200 RL/WL
        *((volatile unsigned int*)(DRAMC2_AO_BASE + 0x005c)) = (0x2d << 0) | (0x2 << 8) | (0x1 << 24) | (0x1 << 31);
        //! MRW trigger
        *((volatile unsigned int*)(DRAMC2_AO_BASE + 0x0060)) |= (1 << 0) ;
        //! polling done (nao + 0x88[0])
        while((*((volatile unsigned int*)(DRAMC2NAO_BASE + 0x0088)) & 0x1) == 0 );
        //! MRW trigger disable
        *((volatile unsigned int*)(DRAMC2_AO_BASE + 0x0060)) &= ~(1 << 0) ;
    
        /*TINFO="Restore CH1 LP4 3200 RL/WL"*/
        //! MR2 for R0 3200 RL/WL [CHD]
        *((volatile unsigned int*)(DRAMC3_AO_BASE + 0x005c)) = (0x2d << 0) | (0x2 << 8) | (0x0 << 24) | (0x1 << 31);
        //! MRW trigger
        *((volatile unsigned int*)(DRAMC3_AO_BASE + 0x0060)) |= (1 << 0) ;
        //! polling done (nao + 0x88[0])
        while((*((volatile unsigned int*)(DRAMC3NAO_BASE + 0x0088)) & 0x1) == 0 );
        //! MRW trigger disable
        *((volatile unsigned int*)(DRAMC3_AO_BASE + 0x0060)) &= ~(1 << 0) ;
        //! MR2 for R1 3200 RL/WL
        *((volatile unsigned int*)(DRAMC3_AO_BASE + 0x005c)) = (0x2d << 0) | (0x2 << 8) | (0x1 << 24) | (0x1 << 31);
        //! MRW trigger
        *((volatile unsigned int*)(DRAMC3_AO_BASE + 0x0060)) |= (1 << 0) ;
        //! polling done (nao + 0x88[0])
        while((*((volatile unsigned int*)(DRAMC3NAO_BASE + 0x0088)) & 0x1) == 0 );
        //! MRW trigger disable
        *((volatile unsigned int*)(DRAMC3_AO_BASE + 0x0060)) &= ~(1 << 0) ;
    }
#endif
    //call_init_dram_again();
#if !__ETT__ & defined(DDR_RESERVE_MODE)
    // scy: not to test on rank1 to avoid memory corruption
    //TestEngine2Test(p);
    //mcSHOW_DBG_MSG(("Exit Test_Engine2_test 1\n"));
#else
    TestEngine2Test(p);
    mcSHOW_DBG_MSG(("Exit Test_Engine2_test 1\n"));
#endif
#if 0
    Func_Emi_Off();
    FUNC_DDRPHY_DLL_OFF_INFRA_CG_EN_ENTER();
    FUNC_DDRPHY_INFRA_MTCMOS_OFF_ENTER(p);
    FUNC_DDRPHY_INFRA_MTCMOS_OFF_EXIT(p);
    FUNC_DDRPHY_DLL_OFF_INFRA_CG_EN_EXIT(p);
#endif    
    Func_Emi_Off();
    Func_Emi_On();

#if !__ETT__ & defined(DDR_RESERVE_MODE)
// Preloader & DDR reserve mode enabled
    // scy: not to test on rank1 to avoid memory corruption
    //g_ddr_reserve_ta_err = TestEngine2Test(p);
    g_ddr_reserve_ta_err = 0;
#else
    TestEngine2Test(p);
#endif
    mcSHOW_DBG_MSG(("Exit Test_Engine2_test 2 + EMI off then on\n"));
    vIO32Write4B_All(DRAMC_REG_SPCMDCTRL, u4Cha_mr4_backup);
    vIO32Write4B_All(DRAMC_REG_DUMMY_RD, u4Cha_dummy_read_backup);
    vIO32Write4B_All(DRAMC_REG_DQSOSCR, u4Cha_dqsosc_backup);
    DDRPhyFreqMeter_1();
    //scy: workaround for DDR reserve mode
    DramcHWGatingOnOff(p, 1);
    p->channel = channel_bak;
    *((volatile unsigned int*)DRAMC_WBR) = u4WbrBackup;
    return;
#endif
}

/* Shuffle group idx is at RG of SPM side which will been reset */
#if 0
void Dramc_DDR_Reserved_Mode_JumpToHigh(void)
{
#if DUAL_FREQ_K
#if DFS_COMBINATION_TYPE1
    DDRPhyFreqSel(&DramCtx_LPDDR3, LJ_DDR1866);
    DramcDFSDirectJump(&DramCtx_LPDDR3, 0, 0, 0);
#elif DFS_COMBINATION_TYPE3    
    DDRPhyFreqSel(&DramCtx_LPDDR3, LJ_DDR1700);
    DramcDFSDirectJump(&DramCtx_LPDDR3, 0, 0, 0);
#else
    DDRPhyFreqSel(&DramCtx_LPDDR3, LJ_DDR1600);
    DramcDFSDirectJump(&DramCtx_LPDDR3, 0, 0, 0);
    DDRPhyFreqSel(&DramCtx_LPDDR3, LC_DDR1600);
    DramcDFSDirectJump(&DramCtx_LPDDR3, 0, 1, 0);
#endif
#endif
}
#endif

/* Shuffle group idx is at RG of SPM side which will been reset */
#if 0
void Dramc_DDR_Reserved_Mode_ResetShuffle(void)
{
#if DUAL_FREQ_K
	DRAM_PLL_FREQ_SEL_T dft_freq_sel = DramCtx_LPDDR3.freq_sel;

	*((UINT32P)(DDRPHY_BASE + 0x63c )) &= ~(0x3);
	*((UINT32P)(DDRPHY1_BASE + 0x63c )) &= ~(0x3);
	*((UINT32P)(DDRPHY2_BASE + 0x63c )) &= ~(0x3);
	*((UINT32P)(DDRPHY3_BASE + 0x63c )) &= ~(0x3);

#if DFS_COMBINATION_TYPE1
    DDRPhyFreqSel(&DramCtx_LPDDR3, LJ_DDR1866);
    DramcDFSDirectJump(&DramCtx_LPDDR3, 0, 0, 0);
#elif DFS_COMBINATION_TYPE3    
    DDRPhyFreqSel(&DramCtx_LPDDR3, LJ_DDR1700);
    DramcDFSDirectJump(&DramCtx_LPDDR3, 0, 0, 0);
#else
    DDRPhyFreqSel(&DramCtx_LPDDR3, LJ_DDR1600);
    DramcDFSDirectJump(&DramCtx_LPDDR3, 0, 0, 0);
#endif
	DDRPhyFreqSel(&DramCtx_LPDDR3, dft_freq_sel);
#endif
}
#endif

void vSetVcoreByFreq(DRAMC_CTX_T *p)
{
#ifndef MT6739_FPGA
#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
    if(p->dram_type == TYPE_LPDDR4P)
    {  
#ifndef DRAM_HQA
        rt5738_set_voltage(400000);
#else
        rt5738_set_voltage(HQA_VDDQ);
#endif
    }
    else if(p->dram_type == TYPE_LPDDR4X)
    {
#ifndef DRAM_HQA
        rt5738_set_voltage(600000);
#else
        rt5738_set_voltage(HQA_VDDQ);
#endif
    }    
    
    if(u1IsLP4Family(p->dram_type))    
    {
        // 2667 & 800 has 0.9V and 0.8V, so K 0.85V
        if(p->frequency==1333 && p->frequency==400)
        {
#ifndef DRAM_HQA
            buck_set_voltage(VCORE, 850000);        
#else
            buck_set_voltage(VCORE, HQA_VCORE);
#endif
        }
        else if(p->frequency>=800)
        {
#ifndef DRAM_HQA
            buck_set_voltage(VCORE, 900000);
#else
            buck_set_voltage(VCORE, HQA_VCORE);
#endif
        }
        else if(p->frequency>=400)
        {
#ifndef DRAM_HQA
            buck_set_voltage(VCORE, 800000);
#else
            buck_set_voltage(VCORE, HQA_VCORE);
#endif
        }
    }
#endif
#endif
}


void mem_test_address_calculation(DRAMC_CTX_T * p, U32 uiSrcAddr, U32*pu4Dest)
{
    U32 u4RankSize;
#if ENABLE_MRR_AFTER_FIRST_K
#if defined(__ETT__)
    *pu4Dest = uiSrcAddr - RANK0_START_VA + RANK1_START_VA;
#else
    *pu4Dest = uiSrcAddr + p->ranksize[RANK_0];
#endif
#else
    if(u1IsLP4Family(p->dram_type))
    {
        u4RankSize = 0x40000000 * p->support_channel_num;
    }
    else //LPDDR3
    {
        u4RankSize = 0x80000000 * p->support_channel_num;
    }

    if(p->support_rank_num == RANK_SINGLE)
    {
        *pu4Dest = uiSrcAddr + (u4RankSize >>1);
    }
    else
    {
        *pu4Dest = uiSrcAddr + u4RankSize;
    }
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
        
        if(err_count)
        {       
            mcSHOW_DBG_MSG(("[MEM_TEST] Rank %d Fail. ", u1RankIdx));
            u1DumpInfo =1;
        }
        else
        {
            mcSHOW_DBG_MSG(("[MEM_TEST] Rank %d OK. ", u1RankIdx));
        }
        mcSHOW_DBG_MSG(("(uiFixedAddr 0x%X, Pass count =%d, Fail count =%d)\n", uiFixedAddr, pass_count, err_count));
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
 	
		for(ch=0;ch<4;ch++)
		{
			for(rank=0;rank<2;rank++)
			{
				//mcSHOW_DBG_MSG(("p->pSavetimeData->u1WriteLeveling_Save[%d][%d][%d]=%d;\n",ch,rank,DDRfreq,p->pSavetimeData->u1WriteLeveling_Save[ch][rank][DDRfreq]));
				for(byte=0;byte<2;byte++)
				{		
		  			 mcSHOW_DBG_MSG(("p->pSavetimeData->u1Gating2T_Save[%d][%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],byte,p->pSavetimeData->u1Gating2T_Save[ch][rank][_MappingFreqArray[DDRfreq]][byte]));
		  			mcSHOW_DBG_MSG(("p->pSavetimeData->u1Gating05T_Save[%d][%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],byte,p->pSavetimeData->u1Gating05T_Save[ch][rank][_MappingFreqArray[DDRfreq]][byte]));
		  			mcSHOW_DBG_MSG(("p->pSavetimeData->u1Gatingfine_tune_Save[%d][%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],byte,p->pSavetimeData->u1Gatingfine_tune_Save[ch][rank][_MappingFreqArray[DDRfreq]][byte]));
		  			mcSHOW_DBG_MSG(("p->pSavetimeData->u1Gatingucpass_count_Save[%d][%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],byte,p->pSavetimeData->u1Gatingucpass_count_Save[ch][rank][_MappingFreqArray[DDRfreq]][byte]));
		  			mcSHOW_DBG_MSG(("p->pSavetimeData->u1WriteLeveling_bypass_Save[%d][%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],byte,p->pSavetimeData->u1WriteLeveling_bypass_Save[ch][rank][_MappingFreqArray[DDRfreq]][byte]));

				mcSHOW_DBG_MSG(("p->pSavetimeData->u1TxCenter_min_Save[%d][%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],byte,p->pSavetimeData->u1TxCenter_min_Save[ch][rank][_MappingFreqArray[DDRfreq]][byte]));
 				mcSHOW_DBG_MSG(("p->pSavetimeData->u1TxCenter_max_Save[%d][%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],byte,p->pSavetimeData->u1TxCenter_max_Save[ch][rank][_MappingFreqArray[DDRfreq]][byte]));
				}
				 for (DQ=0; DQ<16; DQ++)
        			{
 		  			mcSHOW_DBG_MSG(("p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[%d][%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],DQ,p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[ch][rank][_MappingFreqArray[DDRfreq]][DQ]));
		  			mcSHOW_DBG_MSG(("p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[%d][%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],DQ,p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[ch][rank][_MappingFreqArray[DDRfreq]][DQ]));
		  			
				mcSHOW_DBG_MSG(("p->pSavetimeData->u1Txwin_center_Save[%d][%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],DQ,p->pSavetimeData->u1Txwin_center_Save[ch][rank][_MappingFreqArray[DDRfreq]][DQ]));
 				mcSHOW_DBG_MSG(("p->pSavetimeData->u1Txfirst_pass_Save[%d][%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],DQ,p->pSavetimeData->u1Txfirst_pass_Save[ch][rank][_MappingFreqArray[DDRfreq]][DQ]));
				mcSHOW_DBG_MSG(("p->pSavetimeData->u1Txlast_pass_Save[%d][%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],DQ,p->pSavetimeData->u1Txlast_pass_Save[ch][rank][_MappingFreqArray[DDRfreq]][DQ]));
#if CA_PER_BIT_DELAY_CELL
				if(DQ<=5)
					{
				mcSHOW_DBG_MSG(("p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[%d][%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],DQ,p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[ch][rank][_MappingFreqArray[DDRfreq]][DQ]));
					}
#endif                
        			}

				mcSHOW_DBG_MSG(("p->pSavetimeData->u1CBTVref_Save[%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],p->pSavetimeData->u1CBTVref_Save[ch][rank][_MappingFreqArray[DDRfreq]]));
				mcSHOW_DBG_MSG(("p->pSavetimeData->u1CBTClkDelay_Save[%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],p->pSavetimeData->u1CBTClkDelay_Save[ch][rank][_MappingFreqArray[DDRfreq]]));
				mcSHOW_DBG_MSG(("p->pSavetimeData->u1CBTCmdDelay_Save[%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],p->pSavetimeData->u1CBTCmdDelay_Save[ch][rank][_MappingFreqArray[DDRfreq]]));
				mcSHOW_DBG_MSG(("p->pSavetimeData->u1CBTFinalDelay_Save[%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],p->pSavetimeData->u1CBTFinalDelay_Save[ch][rank][_MappingFreqArray[DDRfreq]]));
				mcSHOW_DBG_MSG(("p->pSavetimeData->u1RxDatlat_Save[%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],p->pSavetimeData->u1RxDatlat_Save[ch][rank][_MappingFreqArray[DDRfreq]]));
//				mcSHOW_DBG_MSG(("p->pSavetimeData->u1TxWindowPerbit_Min_FirstPass_Save[%d][%d][%d]=%d;\n",ch,rank,DDRfreq,p->pSavetimeData->u1TxWindowPerbit_Min_FirstPass_Save[ch][rank][DDRfreq]));
				mcSHOW_DBG_MSG(("p->pSavetimeData->u1TxWindowPerbitVref_Save[%d][%d][%d]=%d;\n",ch,rank,_MappingFreqArray[DDRfreq],p->pSavetimeData->u1TxWindowPerbitVref_Save[ch][rank][_MappingFreqArray[DDRfreq]]));	
//				mcSHOW_DBG_MSG(("p->pSavetimeData->u1RxWinPerbit_Min_FirstPass_Save[%d][%d][%d]=%d;\n",ch,rank,DDRfreq,p->pSavetimeData->u1RxWinPerbit_Min_FirstPass_Save[ch][rank][DDRfreq]));
//				mcSHOW_DBG_MSG(("p->pSavetimeData->u1RxWinPerbit_Min_LastPass_Save[%d][%d][%d]=%d;\n",ch,rank,DDRfreq,p->pSavetimeData->u1RxWinPerbit_Min_LastPass_Save[ch][rank][DDRfreq]));
			}
			mcSHOW_DBG_MSG(("p->pSavetimeData->u1RxWinPerbitVref_Save[%d][%d]=%d;\n",ch,_MappingFreqArray[DDRfreq],p->pSavetimeData->u1RxWinPerbitVref_Save[ch][_MappingFreqArray[DDRfreq]]));
		}

	}


}
#endif
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
u32 g_dram_save_time_init_done;

DRAM_STATUS_T DramcSave_Time_For_Cal_Init(DRAMC_CTX_T *p)
{
	int fClr_calfail;
	#if EMMC_READY
	// scy: only need to read emmc one time for each boot-up
	if (g_dram_save_time_init_done == 1)
		return;
	else
		g_dram_save_time_init_done = 1;
   	if(read_offline_dram_calibration_data(p->pSavetimeData)<0)
    	{
 		p->pSavetimeData->femmc_Ready=0;
 		for(fClr_calfail=0;fClr_calfail<PLL_FREQ_SEL_MAX;fClr_calfail++)
 			{
				p->pSavetimeData->fcalfail[p->freq_sel]=0;
 			}
   	 }
    	else
    	{
		p->pSavetimeData->femmc_Ready=1;
 		for(fClr_calfail=0;fClr_calfail<PLL_FREQ_SEL_MAX;fClr_calfail++)
 			{
				p->pSavetimeData->fcalfail[p->freq_sel]=0;
 			}

//		p->pSavetimeData->u1CBTVrefoffset = CBT_VREF_OFFSET;
//		p->pSavetimeData->u1WriteLevelingoffset = WRITE_LEVELING_OFFSET;
//      		p->pSavetimeData->u1GatingStartoffset = GATING_START_OFFSET;	//UI
//      		p->pSavetimeData->u1GatingPass_Winoffset = GATING_PASS_WIN_OFFSET;	//UI
//		p->pSavetimeData->u1RxWinPerbitoffset = RX_WIN_PERBIT_OFFSET;
 //   		p->pSavetimeData->u1RxWinPerbitVrefoffset = RX_WIN_PERBIT_VREF_OFFSET;
 //   		p->pSavetimeData->u1TxWindowPerbitoffset = TX_WIN_PERBIT_OFFSET;
//	    	p->pSavetimeData->u1TxWindowPerbititVrefoffset = TX_WIN_PERBIT_VREF_OFFSET;
//    		p->pSavetimeData->u1RxDatlatoffset = RX_DATLAT_OFFSET;
//    		p->pSavetimeData->RXWindowTHHighSpeed = RX_WIN_HIGH_SPEED_TH;
//	    	p->pSavetimeData->RXWindowTHLowSpeed = RX_WIN_LOW_SPEED_TH;
//    		p->pSavetimeData->TxWindowTH = TX_WIN_TH;


		p->pSavetimeData->Bypass_RDDQC[5]=1;
    		p->pSavetimeData->Bypass_RDDQC[2]=1;
    		p->pSavetimeData->Bypass_RDDQC[1]=0;
    		p->pSavetimeData->Bypass_RXWINDOW[5]=1;
    		p->pSavetimeData->Bypass_RXWINDOW[2]=1;
    		p->pSavetimeData->Bypass_RXWINDOW[1]=0;
   		p->pSavetimeData->Bypass_TXWINDOW[5]=1;
    		p->pSavetimeData->Bypass_TXWINDOW[2]=1;
    		p->pSavetimeData->Bypass_TXWINDOW[1]=0;
   	}
#if FORLOOP_TEST_SAVE_TIME_FOR_CALIBRATION_ON_ETT
	mcSHOW_DBG_MSG(("[save time for cal] dump calibration data\n"));
	vDramfirstCalibrationdump(p);
#endif
    	#else
                memset(p->pSavetimeData, 0, sizeof(SAVE_TIME_FOR_CALIBRATION_T));
				p->pSavetimeData->femmc_Ready=1;
				for(fClr_calfail=0;fClr_calfail<PLL_FREQ_SEL_MAX;fClr_calfail++)
 				{
				p->pSavetimeData->fcalfail[_MappingFreqArray[p->freq_sel]]=0;
 				}

                
//		       	p->pSavetimeData->u1CBTVrefoffset=2;       
//		       	p->pSavetimeData->u1WriteLevelingoffset=5;       
//      				p->pSavetimeData->u1GatingStartoffset=0;  //UI
//      				p->pSavetimeData->u1GatingPass_Winoffset=3;  //UI
//				p->pSavetimeData->u1RxWinPerbitoffset=5;
 //   				p->pSavetimeData->u1RxWinPerbitVrefoffset=4;
//    				p->pSavetimeData->u1TxWindowPerbitoffset=5;
//	    			p->pSavetimeData->u1TxWindowPerbititVrefoffset=4;
//    				p->pSavetimeData->u1RxDatlatoffset=1;
//    				p->pSavetimeData->RXWindowTHHighSpeed=10;
//	    			p->pSavetimeData->RXWindowTHLowSpeed=100;
//    				p->pSavetimeData->TxWindowTH=12;
     				p->pSavetimeData->Bypass_RDDQC[_MappingFreqArray[5]]=1;
    				p->pSavetimeData->Bypass_RDDQC[_MappingFreqArray[2]]=1;
    				p->pSavetimeData->Bypass_RDDQC[_MappingFreqArray[1]]=0;
    				p->pSavetimeData->Bypass_RXWINDOW[_MappingFreqArray[5]]=1;
    				p->pSavetimeData->Bypass_RXWINDOW[_MappingFreqArray[2]]=1;
    				p->pSavetimeData->Bypass_RXWINDOW[_MappingFreqArray[1]]=0;
   				p->pSavetimeData->Bypass_TXWINDOW[_MappingFreqArray[5]]=1;
    				p->pSavetimeData->Bypass_TXWINDOW[_MappingFreqArray[2]]=1;
    				p->pSavetimeData->Bypass_TXWINDOW[_MappingFreqArray[1]]=0;
    				
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
p->pSavetimeData->u1CBTFinalDelay_Save[0][0][2]=2;
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
p->pSavetimeData->u1CBTFinalDelay_Save[0][1][2]=1;
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
p->pSavetimeData->u1CBTFinalDelay_Save[1][0][2]=1;
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
p->pSavetimeData->u1CBTFinalDelay_Save[1][1][2]=1;
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
p->pSavetimeData->u1CBTFinalDelay_Save[2][0][2]=1;
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
p->pSavetimeData->u1CBTFinalDelay_Save[2][1][2]=1;
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
p->pSavetimeData->u1CBTFinalDelay_Save[3][0][2]=2;
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
p->pSavetimeData->u1CBTFinalDelay_Save[3][1][2]=2;
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
p->pSavetimeData->u1CBTFinalDelay_Save[0][0][1]=2;
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
p->pSavetimeData->u1CBTFinalDelay_Save[0][1][1]=2;
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
p->pSavetimeData->u1CBTFinalDelay_Save[1][0][1]=2;
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
p->pSavetimeData->u1CBTFinalDelay_Save[1][1][1]=2;
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
p->pSavetimeData->u1CBTFinalDelay_Save[2][0][1]=2;
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
p->pSavetimeData->u1CBTFinalDelay_Save[2][1][1]=2;
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
p->pSavetimeData->u1CBTFinalDelay_Save[3][0][1]=2;
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
p->pSavetimeData->u1CBTFinalDelay_Save[3][1][1]=2;
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
p->pSavetimeData->u1CBTFinalDelay_Save[0][0][0]=2;
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
p->pSavetimeData->u1CBTFinalDelay_Save[0][1][0]=2;
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
p->pSavetimeData->u1CBTFinalDelay_Save[1][0][0]=1;
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
p->pSavetimeData->u1CBTFinalDelay_Save[1][1][0]=2;
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
p->pSavetimeData->u1CBTFinalDelay_Save[2][0][0]=2;
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
p->pSavetimeData->u1CBTFinalDelay_Save[2][1][0]=2;
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
p->pSavetimeData->u1CBTFinalDelay_Save[3][0][0]=2;
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
p->pSavetimeData->u1CBTFinalDelay_Save[3][1][0]=2;
p->pSavetimeData->u1RxDatlat_Save[3][1][0]=16;
p->pSavetimeData->u1TxWindowPerbitVref_Save[3][1][0]=14;
p->pSavetimeData->u1RxWinPerbitVref_Save[3][0]=4;
	#endif
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

static void vCalibration_Flow_LP4(DRAMC_CTX_T *p)
{
    U8 u1RankMax;
    S8 s1RankIdx;
    DRAM_STATUS_T VrefStatus;
    
#ifdef DDR_INIT_TIME_PROFILING
    UINT32 CPU_Cycle;
    //TimeProfileBegin();
#endif    

#ifdef ENABLE_MIOCK_JMETER
#if 0//TX_PER_BIT_DELAY_CELL
    if((p->channel == CHANNEL_A)&&(p->frequency == 1600 || p->frequency == 1333))
    {
        DramcMiockJmeterForTXCalibration(p);
    }
#endif
#endif

#if ENABLE_PHY_RX_INPUT_OFFSET  // skip when bring up
    ///TODO: no shuffle, only need to do once under highest freq.
    if(p->frequency == u2DFSGetHighestFreq(p))
    DramcRXInputBufferOffsetCal(p);
    
#ifdef DDR_INIT_TIME_PROFILING
    CPU_Cycle=TimeProfileEnd();
    CPUCycleSum += CPU_Cycle;
    mcSHOW_ERR_MSG(("RX input cal takes %d ms\n\r", CPU_Cycle));
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
    
        for(s1RankIdx=RANK_0; s1RankIdx<u1RankMax; s1RankIdx++)   
        {
            vSetRank(p, s1RankIdx);

			//Disable DBI
			DramcEnableDBI(p, 0);
			
        #if ENABLE_LP4_ZQ_CAL
            DramcZQCalibration(p);
        #endif

        vAutoRefreshSwitch(p, DISABLE);

        #if ENABLE_CA_TRAINING //&& (!LP4_DRAM_TYPE_SELECT)) // skip when bring up
#if SUPPORT_CBT_K_RANK1==0        
            if(s1RankIdx == RANK_0)
#endif                
            {
                VrefStatus = CmdBusTrainingLP4(p);

		#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        	if(((p->pSavetimeData->fcalfail[p->freq_sel] & 0x40)>>6)==1)
        	{
                	VrefStatus = CmdBusTrainingLP4(p);
        	}
		#endif
                #if SUPPORT_SAVE_OFF_LINE_CALIBRATION
                if(VrefStatus == DRAM_K_VREF)
                CmdBusTrainingLP4(p);
                #endif

                #if DUAL_FREQ_K
                No_Parking_On_CLRPLL(p); 
                #endif               
            }
         #ifdef DDR_INIT_TIME_PROFILING
        CPU_Cycle=TimeProfileEnd();
        CPUCycleSum += CPU_Cycle;
        mcSHOW_ERR_MSG(("Rank %d CBT takes %d ms\n\r", s1RankIdx, CPU_Cycle));
        TimeProfileBegin();
        #endif
        #endif
        
        #if ENABLE_WRITE_LEVELING            
        DramcWriteLeveling((DRAMC_CTX_T *) p);//Dram will be reset when finish write leveling         
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(((p->pSavetimeData->fcalfail[p->freq_sel] & 0x01)>>0)==1)
        {
         DramcWriteLeveling((DRAMC_CTX_T *) p);//Dram will be reset when finish write leveling         
        }
#endif
        
        vAutoRefreshSwitch(p, ENABLE);
        
        #ifdef DDR_INIT_TIME_PROFILING
        CPU_Cycle=TimeProfileEnd();
        CPUCycleSum += CPU_Cycle;
        mcSHOW_ERR_MSG(("Rank %d Write leveling takes %d ms\n\r", s1RankIdx, CPU_Cycle));
        TimeProfileBegin();
        #endif
        #endif        
    
        #if LJPLL_FREQ_DEBUG_LOG
            DDRPhyFreqMeter();
        #endif
                    
            DramcRxdqsGatingCal(p);
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(((p->pSavetimeData->fcalfail[p->freq_sel] & 0x02)>>1)==1)
        {
             DramcRxdqsGatingCal(p);
        }
#endif           
        #ifdef DDR_INIT_TIME_PROFILING
        CPU_Cycle=TimeProfileEnd();    
        CPUCycleSum += CPU_Cycle;
        mcSHOW_ERR_MSG(("Rank %d Gating takes %d ms\n\r", s1RankIdx, CPU_Cycle));
        TimeProfileBegin();
        #endif
    
        #if LJPLL_FREQ_DEBUG_LOG
            DDRPhyFreqMeter();
        #endif

        DramcRxWindowPerbitCal((DRAMC_CTX_T *) p, 0);
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
         if(((p->pSavetimeData->fcalfail[p->freq_sel] & 0x04)>>2)==1)
        {
       	DramcRxWindowPerbitCal((DRAMC_CTX_T *) p, 0);
         }
#endif
		//Recover DBI
		DramcEnableDBI(p, 1);

        #ifdef DDR_INIT_TIME_PROFILING
        CPU_Cycle=TimeProfileEnd();   
        CPUCycleSum += CPU_Cycle;
        mcSHOW_ERR_MSG(("Rank %d RX RDDQC takes %d ms\n\r", s1RankIdx, CPU_Cycle));
        TimeProfileBegin();
        #endif

        #if LJPLL_FREQ_DEBUG_LOG
            DDRPhyFreqMeter();
        #endif        

        DramcTxWindowPerbitCal((DRAMC_CTX_T *) p, TX_DQ_DQS_MOVE_DQ_DQM);
        VrefStatus = DramcTxWindowPerbitCal((DRAMC_CTX_T *) p, TX_DQ_DQS_MOVE_DQ_ONLY);        
 #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(((p->pSavetimeData->fcalfail[p->freq_sel] & 0x8)>>3)==1)
        {
         DramcTxWindowPerbitCal((DRAMC_CTX_T *) p, TX_DQ_DQS_MOVE_DQ_DQM);
       VrefStatus = DramcTxWindowPerbitCal((DRAMC_CTX_T *) p, TX_DQ_DQS_MOVE_DQ_ONLY);     
        }
#endif
        #if SUPPORT_SAVE_OFF_LINE_CALIBRATION
        if(VrefStatus == DRAM_K_VREF)
        DramcTxWindowPerbitCal((DRAMC_CTX_T *) p, TX_DQ_DQS_MOVE_DQ_ONLY);
        #endif

        #ifdef DDR_INIT_TIME_PROFILING
        CPU_Cycle=TimeProfileEnd();  
        CPUCycleSum += CPU_Cycle;
        mcSHOW_ERR_MSG(("Rank %d TX calibration takes %d ms\n\r", s1RankIdx, CPU_Cycle));
        TimeProfileBegin();
        #endif   

        #if LJPLL_FREQ_DEBUG_LOG
            DDRPhyFreqMeter();
        #endif        
        
        DramcRxdatlatCal((DRAMC_CTX_T *) p);
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION    
        if(((p->pSavetimeData->fcalfail[p->freq_sel] & 0x10)>>4)==1)
        {
          DramcRxdatlatCal((DRAMC_CTX_T *) p);
        }
 #endif 
        #ifdef DDR_INIT_TIME_PROFILING
        CPU_Cycle=TimeProfileEnd();  
        CPUCycleSum += CPU_Cycle;
        mcSHOW_ERR_MSG(("Rank %d Datlat takes %d ms\n\r", s1RankIdx, CPU_Cycle));
        TimeProfileBegin();
        #endif   
    
        #if LJPLL_FREQ_DEBUG_LOG
            DDRPhyFreqMeter();
        #endif
    
        VrefStatus=DramcRxWindowPerbitCal((DRAMC_CTX_T *) p, 1);
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(((p->pSavetimeData->fcalfail[p->freq_sel] & 0x20)>>5)==1)
        {
        VrefStatus=DramcRxWindowPerbitCal((DRAMC_CTX_T *) p, 1);
        }
#endif        
        #if SUPPORT_SAVE_OFF_LINE_CALIBRATION
        if(VrefStatus == DRAM_K_VREF)
            DramcRxWindowPerbitCal((DRAMC_CTX_T *) p, 1);
        #endif
                
            #ifdef DDR_INIT_TIME_PROFILING
            CPU_Cycle=TimeProfileEnd();
            CPUCycleSum += CPU_Cycle;
            mcSHOW_ERR_MSG(("Rank %d RX calibration takes %d ms\n\r", s1RankIdx, CPU_Cycle));      
            TimeProfileBegin();
            #endif
           // DramcRxdqsGatingCal(p);

            #if ENABLE_TX_TRACKING
            if(gu1MR23Done==FALSE)
            {
                DramcDQSOSCAuto(p);
            }
            DramcDQSOSCAuto(p);
            DramcDQSOSCMR23(p);
            DramcDQSOSCSetMR18MR19(p);
            #endif

            #if PRINT_CBT_VREFCA_VALUE
            Print_CBT_VREFCA_Value(p);
            #endif
        }    
        #if ENABLE_TX_TRACKING
        DramcDQSOSCShuSettings(p);
        #endif    

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

        #ifdef DDR_INIT_TIME_PROFILING
        CPU_Cycle=TimeProfileEnd();
        CPUCycleSum += CPU_Cycle;
        mcSHOW_ERR_MSG(("Misc takes %d ms\n\r", s1RankIdx, CPU_Cycle));      
        TimeProfileBegin();
        #endif

}

#if ENABLE_LP3_SW
static void vCalibration_Flow_LP3(DRAMC_CTX_T *p)
{
    U8 u1RankMax;
    S8 s1RankIdx;

#ifdef DDR_INIT_TIME_PROFILING
    UINT32 CPU_Cycle;
    //TimeProfileBegin();
#endif    

#ifdef ENABLE_MIOCK_JMETER_HQA
    if(p->frequency == u2DFSGetHighestFreq(p))
    {
        DramcMiockJmeterHQA(p);
    }
#endif

    vAutoRefreshSwitch(p, DISABLE);

#if ENABLE_CA_TRAINING  // skip when bring up
    vSetRank(p, RANK_0);
    CATrainingLP3(p);
#endif

#ifdef DDR_INIT_TIME_PROFILING
    CPU_Cycle=TimeProfileEnd();    
    mcSHOW_ERR_MSG(("DRAMC CA train takes %d ms\n\r", CPU_Cycle));
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

        vAutoRefreshSwitch(p, DISABLE);
    
#if DUAL_RANK_ENABLE
            if((p->support_rank_num==RANK_SINGLE) || ((p->support_rank_num==RANK_DUAL) && (s1RankIdx == RANK_0)))
#endif
            {
                #if ENABLE_WRITE_LEVELING            
                    DramcWriteLeveling((DRAMC_CTX_T *) p);//Dram will be reset when finish write leveling
                #endif        
    
                #ifdef DDR_INIT_TIME_PROFILING
                    CPU_Cycle=TimeProfileEnd();    
                    mcSHOW_ERR_MSG(("Rank %d Write leveling takes %d ms\n\r", s1RankIdx, CPU_Cycle));
                    TimeProfileBegin();
                #endif
            }
    
        vAutoRefreshSwitch(p, ENABLE);
    
        #if LJPLL_FREQ_DEBUG_LOG
            DDRPhyFreqMeter();
        #endif
                    
            DramcRxdqsGatingCal(p);
            
        #ifdef DDR_INIT_TIME_PROFILING
            CPU_Cycle=TimeProfileEnd();    
            mcSHOW_ERR_MSG(("Rank %d Gating takes %d ms\n\r", s1RankIdx, CPU_Cycle));
            TimeProfileBegin();
        #endif
    
        #if LJPLL_FREQ_DEBUG_LOG
            DDRPhyFreqMeter();
        #endif
    
            DramcRxdatlatCal((DRAMC_CTX_T *) p);
    
        #ifdef DDR_INIT_TIME_PROFILING
            CPU_Cycle=TimeProfileEnd();    
            mcSHOW_ERR_MSG(("Rank %d Datlat takes %d ms\n\r", s1RankIdx, CPU_Cycle));
            TimeProfileBegin();
        #endif
    
        #if LJPLL_FREQ_DEBUG_LOG
            DDRPhyFreqMeter();
        #endif
    
        #if DUAL_RANK_ENABLE
        #ifndef DUAL_RANK_RX_K
            if(s1RankIdx==RANK_0)
        #endif
        #endif
            {
                DramcRxWindowPerbitCal((DRAMC_CTX_T *) p, 1);
                
            #ifdef DDR_INIT_TIME_PROFILING
                CPU_Cycle=TimeProfileEnd();    
                mcSHOW_ERR_MSG(("Rnak %d RX takes %d ms\n\r", s1RankIdx, CPU_Cycle));      
                TimeProfileBegin();
            #endif
            }
    
        #if DUAL_RANK_ENABLE
        #ifndef DUAL_RANK_TX_K
            if(s1RankIdx==RANK_0)
        #endif
        #endif
            {
                DramcTxWindowPerbitCal((DRAMC_CTX_T *) p, TX_DQ_DQS_MOVE_DQ_DQM);   
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
}
#endif


static void vDramCalibrationSingleChannel(DRAMC_CTX_T *p)
{
#if !__ETT__
    /*
     * Since DRAM calibration will cost much time,
     * kick wdt here to prevent watchdog timeout.
     */
    mtk_wdt_restart();
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
    vSetPHY2ChannelMapping(p, CHANNEL_A); // when switching channel, must update PHY to Channel Mapping
    vDramCalibrationSingleChannel(p);

    if (Get_MDL_Used_Flag()==GET_MDL_USED) return;  // only K CHA to save time

    #if CHANNEL_NUMBER==4
    if(p->support_channel_num == CHANNEL_FOURTH)
    {
        vSetPHY2ChannelMapping(p, CHANNEL_B);// when switching channel, must update PHY to Channel Mapping
        vDramCalibrationSingleChannel(p);

        vSetPHY2ChannelMapping(p, CHANNEL_C);// when switching channel, must update PHY to Channel Mapping
        vDramCalibrationSingleChannel(p);

        vSetPHY2ChannelMapping(p, CHANNEL_D);// when switching channel, must update PHY to Channel Mapping
        vDramCalibrationSingleChannel(p);        

        vSetPHY2ChannelMapping(p, CHANNEL_A);
    }   
    #endif
    #if CHANNEL_NUMBER==2
    if(p->support_channel_num == CHANNEL_DUAL)
    {
        vSetPHY2ChannelMapping(p, CHANNEL_B);// when switching channel, must update PHY to Channel Mapping
        vDramCalibrationSingleChannel(p);

        vSetPHY2ChannelMapping(p, CHANNEL_A);
    }
    #endif

    
    

#ifdef FOR_HQA_TEST_USED
    print_HQA_measure_message(p);
#endif 

#ifdef DRAM_CALIB_LOG  
    i4WriteSramLog(gDRAM_CALIB_LOG_pointer, (unsigned int*) &gDRAM_CALIB_LOG, sizeof(gDRAM_CALIB_LOG)/4);
    gDRAM_CALIB_LOG_pointer += sizeof(gDRAM_CALIB_LOG)/4;
#endif

}

#if ENABLE_MRR_AFTER_FIRST_K
int GetDramInforAfterCalByMRR(DRAMC_CTX_T *p, DRAM_INFO_BY_MRR_T *DramInfo)
{
    U8 u1ChannelIdx, u1RankIdx, u1RankMax, u1DieNumber=0;
    U16 u2Density;
    U32 u4Size;
    U32 u4ChannelSize, u4ChannelNumber=1, u4RankNumber=1;
    
    vSetPHY2ChannelMapping(p, CHANNEL_A);

    // Read MR5 for Vendor ID
    DramcModeRegReadByRank(p, RANK_0, 5, &(DramInfo->u2MR5VendorID));
    DramInfo->u2MR5VendorID &= 0xff; // for byte mode, don't show value of another die.
    p->vendor_id = DramInfo->u2MR5VendorID;
    mcSHOW_DBG_MSG(("[GetDramInforAfterCalByMRR] Vendor %x.\n", DramInfo->u2MR5VendorID));

     for(u1ChannelIdx=0; u1ChannelIdx<CHANNEL_NUMBER; u1ChannelIdx++)
         for(u1RankIdx =0; u1RankIdx<RANK_MAX; u1RankIdx++)
            DramInfo->u4MR8Density[u1ChannelIdx][u1RankIdx] =0;

     // Read MR8 for dram density
    for(u1ChannelIdx=0; u1ChannelIdx<(p->support_channel_num); u1ChannelIdx++)
        for(u1RankIdx =0; u1RankIdx<(p->support_rank_num); u1RankIdx++)
        {
            if((p->aru4CalExecuteFlag[u1ChannelIdx][u1RankIdx][p->shu_type] !=0)  && \
                (p->aru4CalResultFlag[u1ChannelIdx][u1RankIdx][p->shu_type]==0))
            {         
                vSetPHY2ChannelMapping(p, u1ChannelIdx);
                DramcModeRegReadByRank(p, u1RankIdx, 8, &u2Density);
                mcSHOW_DBG_MSG(("[GetDramInforAfterCalByMRR] MR8 %x.\n", u2Density));

                u1DieNumber =1;
                if(p->dram_type == TYPE_LPDDR3)
                {
                    if(((u2Density >> 6) & 0x3)==1) //OP[7:6] =0, x16 (2 die)
                        u1DieNumber =2;
                }
                
                u2Density = (u2Density>>2)&0xf;
                p->density = u2Density;
                
                if(u1IsLP4Family(p->dram_type))
                {
            	    switch(u2Density)
            	    {
            	        ///TODO: Darren, please check the value of u4Size.
            	        case 0x0:
            	            u4Size = 0x20000000;  //4Gb
            	            //DBG_MSG("[EMI]DRAM density = 4Gb\n");
            	            break;
            	        case 0x1:
            	            u4Size = 0x30000000;  //6Gb
            	            //DBG_MSG("[EMI]DRAM density = 6Gb\n");
            	            break;
            	        case 0x2:
            	            u4Size = 0x40000000;  //8Gb
            	            //DBG_MSG("[EMI]DRAM density = 8Gb\n");
            	            break;
            	        case 0x3:
            	            u4Size = 0x60000000;  //12Gb
            	            //DBG_MSG("[EMI]DRAM density = 12Gb\n");
            	            break;
            	        case 0x4:
            	            u4Size = 0x80000000;  //16Gb
            	            //DBG_MSG("[EMI]DRAM density = 16Gb\n");
            	            break;
            	        //case 0x5:
            	            //u4Size = 0x100000000L; //32Gb
            	            //DBG_MSG("[EMI]DRAM density = 32Gb\n");
            	            break;
            	        default:
            	            u4Size = 0; //reserved
            	    }  
                }
                else //LPDDR3 
                {
                    switch(u2Density)
                    {
                        case 0x6:
                            u4Size = 0x20000000;  //4Gb
                            //DBG_MSG("[EMI]DRAM density = 4Gb\n");
                            break;
                        case 0xE:
                            u4Size = 0x30000000;  //6Gb
                            //DBG_MSG("[EMI]DRAM density = 6Gb\n");
                            break;
                        case 0x7:
                            u4Size = 0x40000000;  //8Gb
                            //DBG_MSG("[EMI]DRAM density = 8Gb\n");
                            break;
                        case 0xD:
                            u4Size = 0x60000000;  //12Gb
                            //DBG_MSG("[EMI]DRAM density = 12Gb\n");
                            break;
                        case 0x8:
                            u4Size = 0x80000000;  //16Gb
                            //DBG_MSG("[EMI]DRAM density = 16Gb\n");
                            break;
                        //case 0x9:
                            //u4Size = 0x100000000L; //32Gb
                            //DBG_MSG("[EMI]DRAM density = 32Gb\n");
                            //break;
                        default:
                            u4Size = 0; //reserved
                    }  
                }
                
                u4Size *= u1DieNumber;
                u4ChannelSize = u4Size;
#if CHANNEL_NUMBER==2
                u4ChannelNumber = 2;
#elif CHANNEL_NUMBER==4
                u4ChannelNumber = 4;
#endif

#if DUAL_RANK_ENABLE
                u4RankNumber = 2;
#endif
                p->ranksize[u1RankIdx] = u4ChannelSize*(u4ChannelNumber/u4RankNumber);
                DramInfo->u4MR8Density[u1ChannelIdx][u1RankIdx] = u4Size;
         }
        	
        mcSHOW_DBG_MSG(("[GetDramInforAfterCalByMRR] Channel %d, Rank %d, u1DieNumber %d, Desity %x, Ranksize %x.\n", u1ChannelIdx,u1RankIdx, u1DieNumber, DramInfo->u4MR8Density[u1ChannelIdx][u1RankIdx], p->ranksize[u1RankIdx]));
        }

    vSetPHY2ChannelMapping(p, CHANNEL_A);

    return 0;
}
#endif

#if ENABLE_RANK_NUMBER_AUTO_DETECTION
void DramRankNumberDetection(DRAMC_CTX_T *p)
{
    U8 u1RankBak;

    u1RankBak = u1GetRank(p);  // backup current rank setting
    
    vSetPHY2ChannelMapping(p, CHANNEL_A); // when switching channel, must update PHY to Channel Mapping
    vSetRank(p, RANK_1); 


    DramcWriteLeveling((DRAMC_CTX_T *) p);//Dram will be reset when finish write leveling
   
    if(((p->aru4CalResultFlag[p->channel][p->rank][p->shu_type] >>DRAM_CALIBRATION_WRITE_LEVEL) &0x1) ==0)
    {
        p->support_rank_num = RANK_DUAL;
    }
    else
    {
        p->support_rank_num = RANK_SINGLE;
    }
    mcSHOW_DBG_MSG(("[DramRankNumberDetection] %d, %X\n", p->support_rank_num, p->aru4CalResultFlag[p->channel][p->rank][p->shu_type]));
    
    vSetRank(p, u1RankBak);  // restore rank setting
}
#endif

void Before_Init_DRAM_While_Reserve_Mode_fail(DRAM_DRAM_TYPE_T dram_type)
{
    DRAMC_CTX_T * p;
    mcSHOW_DBG_MSG(("========Reserve mode fall --> Before_Init_DRAM_While_Reserve_Mode_fail=======\n"));
    if(u1IsLP4Family(dram_type))
    {
        psCurrDramCtx = &DramCtx_LPDDR4;
    }
    else
    {
#if (!__ETT__ && ENABLE_LP3_SW==0)
// Preloader which does not support LP3
// scy: reduce code size by removing unused LPDDR3 structure
	mcSHOW_DBG_MSG(("in Before_Init_DRAM_While_Reserve_Mode_fail: not support LPDDR3\n"));
	ASSERT(0);
#else
        psCurrDramCtx = &DramCtx_LPDDR3;
#endif
    }
    p = psCurrDramCtx;  

#if (SW_CHANGE_FOR_SIMULATION==0)
    EnableDramcPhyDCM(p, 0);
#endif
    Switch26MHzDisableDummyReadRefreshAllBank(p);
 	*((volatile unsigned int*)WHITNEY_SPM_POWER_ON_VAL0)      &= ~(0x1 << 28);//Switch PLL to RG mode
	*((volatile unsigned int*)WHITNEY_DRAMC_DPY_CLK_SW_CON3)  &= ~(0x1 << 2);//Switch PLL to RG mode  
}

#ifdef ENABLE_MIOCK_JMETER_HQA
void PRE_MIOCK_JMETER_HQA_USED(DRAMC_CTX_T *p)
{   
    U32 backup_freq_sel, backup_channel;
    U32 channel_idx;

    backup_freq_sel = p->freq_sel;
    backup_channel = p->channel;
    
    p->freq_sel = LP4_DDR3200;   
    DDRPhyFreqSel(p, p->freq_sel);
    vSetVcoreByFreq(p);
    DramcInit((DRAMC_CTX_T *) p);

    vSetPHY2ChannelMapping(p, CHANNEL_A); 
    DramcMiockJmeterHQA(p);
    vSetPHY2ChannelMapping(p, backup_channel);

    #if 0
    p->freq_sel = LP4_DDR2667;   
    DDRPhyFreqSel(p, p->freq_sel);
    vSetVcoreByFreq(p);
    DramcInit((DRAMC_CTX_T *) p);   
    for(channel_idx=CHANNEL_A; channel_idx<CHANNEL_NUMBER; channel_idx++)
    {
        vSetPHY2ChannelMapping(p, channel_idx); 
        DramcMiockJmeterHQA(p);
    }
    vSetPHY2ChannelMapping(p, backup_channel);
    #endif

    p->freq_sel = backup_freq_sel;
}
#endif


//#define TEST_MODE_MRS
#if SUPPORT_SAVE_OFF_LINE_CALIBRATION
int Init_DRAM(DRAM_DRAM_TYPE_T dram_type, DRAM_CBT_MODE_T dram_cbt_mode, DRAM_INFO_BY_MRR_T *DramInfo, U8 get_mdl_used, SAVE_OFF_LINE_CALIBRATION_T *offLine_SaveData)
#else
int Init_DRAM(DRAM_DRAM_TYPE_T dram_type, DRAM_CBT_MODE_T dram_cbt_mode, DRAM_INFO_BY_MRR_T *DramInfo, U8 get_mdl_used)
#endif
{
   #if !SW_CHANGE_FOR_SIMULATION

#if 0//SUPPORT_SAVE_OFF_LINE_CALIBRATION
   read_offline_dram_calibration_data(offLine_SaveData);
   while(1);
#endif

    #ifdef DDR_INIT_TIME_PROFILING
    UINT32 CPU_Cycle;
    #endif
    int mem_start,len, s4value;
    DRAMC_CTX_T * p;
    U8 ucstatus = 0;
    U32 u4value;

    if(u1IsLP4Family(dram_type))
    {
        psCurrDramCtx = &DramCtx_LPDDR4;
    }
    else
    {
#if (!__ETT__ && ENABLE_LP3_SW==0)
// Preloader which does not support LP3
// scy: reduce code size by removing unused LPDDR3 structure
	mcSHOW_DBG_MSG(("in Init_DRAM: not support LPDDR3\n"));
	ASSERT(0);
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
            p->dram_cbt_mode = LP4_CBT_MODE;
        }
        else  //Preloader & ETTLPDDR3
    #endif
        {        
            p->dram_type = dram_type;
            p->dram_cbt_mode = dram_cbt_mode;
        }
    #if SUPPORT_SAVE_OFF_LINE_CALIBRATION
    p->pOffLineSaveData = offLine_SaveData;
    #endif

#if ENABLE_MRR_AFTER_FIRST_K
    if(DramInfo != NULL)
#endif
    {
#if !__ETT__
        if (Get_MDL_Used_Flag()==GET_MDL_USED)
#else
        if (Get_MDL_Used_Flag()==FIRST_USED)
#endif
        {
            MPLLInit();
            Global_Option_Init(p);
        }

#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0) 
        EMI_Init(p);    
#endif

        mcSHOW_DBG_MSG(("\n\n[Olympus] ETT version 0.0.0.1\n dram_type %d, cbt_mode %d \n\n", p->dram_type, p->dram_cbt_mode));

#if defined(DDR_INIT_TIME_PROFILING)
        TimeProfileBegin();
#endif


    #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    	DramcSave_Time_For_Cal_Init(p);
    #endif

#ifdef ENABLE_MIOCK_JMETER_HQA
    if (Get_MDL_Used_Flag()==FIRST_USED)
    {
        PRE_MIOCK_JMETER_HQA_USED(p);
    }
#endif


#if 0//defined(DDR_INIT_TIME_PROFILING)
          TimeProfileBegin();
          mcDELAY_US(1000000);
          CPU_Cycle=TimeProfileEnd();    
          mcSHOW_ERR_MSG((" mcDELAY_US(1000) takes %d us\n\r", CPU_Cycle));  
#endif
      
#if 0//def Enable_GP6_TIMER
      U32 l_low_tick0, l_high_tick0;
          GPT_Start(GPT6);
          l_low_tick0 = GPT_GetTickCount(&l_high_tick0);
          must_print("Before GPT_GetTickCount = 0x%x 0x%x\n", l_high_tick0, l_low_tick0);
          delay_a_while (6500000);
          l_low_tick0 = GPT_GetTickCount(&l_high_tick0);
          must_print("After GPT_GetTickCount = 0x%x 0x%x\n", l_high_tick0, l_low_tick0);
          //while(1);
#endif

   // DramC & PHY init for all channels  
      DDRPhyFreqSel(p, p->freq_sel);
      vSetVcoreByFreq(p);

      if (p->dram_type == TYPE_LPDDR4X || p->dram_type == TYPE_LPDDR4P)
      {
          DramcSwImpedanceCal(p,1, 0);  //without term
      }
      
      if (p->dram_type != TYPE_LPDDR4P)
      {
          DramcSwImpedanceCal(p,1, 1);  //within term
      }

      //DdrPhyInit((DRAMC_CTX_T *) p);
      DramcInit((DRAMC_CTX_T *) p);   
//ETT_NO_DRAM }
//ETT_NO_DRAM #if 0 	

      vApplyConfigBeforeCalibration((DRAMC_CTX_T *) p);

#ifdef LOOPBACK_TEST
      mcSHOW_DBG_MSG(("\n\nDDR loop-back test \n"));
      DramcLoopbackTest(p);
      while (1);
#endif

#ifdef TEST_MODE_MRS
    if(global_which_test  == 0)          
      TestModeTestMenu();
#endif



#if ENABLE_RANK_NUMBER_AUTO_DETECTION  // only need to do this when DUAL_RANK_ENABLE is 1
        DramRankNumberDetection(p);
        DFSInitForCalibration(p);  // Restore setting after rank dection (especially DQ= DQS+16)
#endif

#if defined(DDR_INIT_TIME_PROFILING)
              CPU_Cycle = TimeProfileEnd();
              mcSHOW_ERR_MSG(("\nInit  take %d ms\n\r", CPU_Cycle));                   
              TimeProfileBegin();
#endif
        EMI_Init2(); 
        vDramCalibrationAllChannel(p);

#if ENABLE_MRR_AFTER_FIRST_K
        GetDramInforAfterCalByMRR(p, DramInfo);
        vDramcACTimingOptimize(p);
        return 0;
#endif
    }
  
 
#if DUAL_FREQ_K
    if(u1IsLP4Family(p->dram_type))
    {
    #if 0
        DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_1, DRAM_DFS_SHUFFLE_4);
        DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_1, DRAM_DFS_SHUFFLE_3);
        DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_1, DRAM_DFS_SHUFFLE_2);
    #else
        DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_1, DRAM_DFS_SHUFFLE_4);

        DDRPhyFreqSel(p, LP4_DDR2667);
        vSetVcoreByFreq(p);
        DFSInitForCalibration(p);
        vDramCalibrationAllChannel(p);    
        #if ENABLE_MRR_AFTER_FIRST_K
        vDramcACTimingOptimize(p);
        #endif
        DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_1, DRAM_DFS_SHUFFLE_3);
        DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_1, DRAM_DFS_SHUFFLE_2);

        DDRPhyFreqSel(p, LP4_DDR3200);
        vSetVcoreByFreq(p);
        DFSInitForCalibration(p);
        vDramCalibrationAllChannel(p);    
        #if ENABLE_MRR_AFTER_FIRST_K
        vDramcACTimingOptimize(p);
        #endif

    #endif
    }
#endif

    vApplyConfigAfterCalibration(p);
#if FORLOOP_TEST_SAVE_TIME_FOR_CALIBRATION_ON_ETT
	vDramfirstCalibrationdump(p);
#endif
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
	if(p->pSavetimeData->femmc_Ready==0
			|| (p->pSavetimeData->fcalfail[LP4_DDR800])
			|| (p->pSavetimeData->fcalfail[LP4_DDR2667])
			|| (p->pSavetimeData->fcalfail[LP4_DDR3200]))
	{
		#if EMMC_READY
		write_offline_dram_calibration_data(p->pSavetimeData);
		#endif
		mcSHOW_DBG_MSG(("[save time for cal] fcalfail[LP4_DDR800] = 0x%x\n",  p->pSavetimeData->fcalfail[LP4_DDR800]));
		mcSHOW_DBG_MSG(("[save time for cal] fcalfail[LP4_DDR2667] = 0x%x\n",  p->pSavetimeData->fcalfail[LP4_DDR2667]));
		mcSHOW_DBG_MSG(("[save time for cal] fcalfail[LP4_DDR3200] = 0x%x\n",  p->pSavetimeData->fcalfail[LP4_DDR3200]));
		mcSHOW_DBG_MSG(("[save time for cal] Save calibration result to emmc\n"));		
	} else {
		mcSHOW_DBG_MSG(("[save time for cal] Bypass saving calibration result to emmc\n"));
	}
#endif


#if defined(DDR_INIT_TIME_PROFILING)
    CPU_Cycle=TimeProfileEnd();  
    CPUCycleSum += CPU_Cycle;
    mcSHOW_ERR_MSG((" Total calibrations take %d ms\n\r", CPUCycleSum));  
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

#ifdef RX_EYE_SCAN
    int i;

    mcSHOW_DBG_MSG(("Channel A RX eye scan.\n\n"));
    vSetPHY2ChannelMapping(p, CHANNEL_A);
    DramcRxEyeScanInit(p);
    for(i=0; i<8; i++)
    {
        #if 0
        DramcRxEyeScanRun(p, 0, i); // use test engine 2
        #else
        DramcRxEyeScanRun(p, 1, i);  // use DMA
        #endif
    }
	
	#if CHANNEL_NUMBER==4
    if(p->support_channel_num == CHANNEL_FOURTH)
    {
    	mcSHOW_DBG_MSG(("Channel B RX eye scan.\n\n"));
    	vSetPHY2ChannelMapping(p, CHANNEL_B);
    	DramcRxEyeScanInit(p);
    	for(i=0; i<8; i++)
    	{
        	#if 0
        	DramcRxEyeScanRun(p, 0, i); // use test engine 2
        	#else
        	DramcRxEyeScanRun(p, 1, i);  // use DMA
       		#endif
    	}

		mcSHOW_DBG_MSG(("Channel C RX eye scan.\n\n"));
    	vSetPHY2ChannelMapping(p, CHANNEL_C);
    	DramcRxEyeScanInit(p);
    	for(i=0; i<8; i++)
    	{
        	#if 0
        	DramcRxEyeScanRun(p, 0, i); // use test engine 2
        #else
        	DramcRxEyeScanRun(p, 1, i);  // use DMA
        #endif
    }

		mcSHOW_DBG_MSG(("Channel D RX eye scan.\n\n"));
    	vSetPHY2ChannelMapping(p, CHANNEL_D);
   		DramcRxEyeScanInit(p);
    	for(i=0; i<8; i++)
    	{
        	#if 0
        	DramcRxEyeScanRun(p, 0, i); // use test engine 2
        	#else
        	DramcRxEyeScanRun(p, 1, i);  // use DMA
        	#endif
    	}
    }
	#endif
	#if CHANNEL_NUMBER==2
    if(p->support_channel_num == CHANNEL_DUAL)
    {
    	mcSHOW_DBG_MSG(("Channel B RX eye scan.\n\n"));
    vSetPHY2ChannelMapping(p, CHANNEL_B);
    DramcRxEyeScanInit(p);
    for(i=0; i<8; i++)
    {
        #if 0
        	DramcRxEyeScanRun(p, 0, i); // use test engine 2
        #else
        	DramcRxEyeScanRun(p, 1, i);  // use DMA
        #endif
    }
	}
	#endif
    //while(1);
#endif


#ifdef TX_EYE_SCAN
    DramcTxEyeScanInit(p);

    mcSHOW_DBG_MSG(("Channel A TX eye scan.\n\n"));
    vSetPHY2ChannelMapping(p, CHANNEL_A);
    {
        #if 1
        DramcTxEyeScanRun(p, 0); //use test engine 2
        #else
        DramcTxEyeScanRun(p, 1);  // use DMA
        #endif
    }

    #if CHANNEL_NUMBER==4
    if(p->support_channel_num == CHANNEL_FOURTH)
    {
        mcSHOW_DBG_MSG(("Channel B TX eye scan.\n\n"));
        vSetPHY2ChannelMapping(p, CHANNEL_B);
        #if 1
        DramcTxEyeScanRun(p, 0); //use test engine 2
        #else
        DramcTxEyeScanRun(p, 1);  // use DMA
        #endif

        mcSHOW_DBG_MSG(("Channel C TX eye scan.\n\n"));
        vSetPHY2ChannelMapping(p, CHANNEL_C);
        #if 1
        DramcTxEyeScanRun(p, 0); //use test engine 2
        #else
        DramcTxEyeScanRun(p, 1);  // use DMA
        #endif        

        mcSHOW_DBG_MSG(("Channel D TX eye scan.\n\n"));
        vSetPHY2ChannelMapping(p, CHANNEL_D);
        #if 1
        DramcTxEyeScanRun(p, 0); //use test engine 2
        #else
        DramcTxEyeScanRun(p, 1);  // use DMA
        #endif        
    }    
    #endif
    #if CHANNEL_NUMBER==2
    if(p->support_channel_num == CHANNEL_DUAL)
    {
        mcSHOW_DBG_MSG(("Channel B TX eye scan.\n\n"));
        vSetPHY2ChannelMapping(p, CHANNEL_B);
        #if 1
        DramcTxEyeScanRun(p, 0); //use test engine 2
        #else
        DramcTxEyeScanRun(p, 1);  // use DMA
        #endif
    }
    #endif
    while(1);
#endif //TX_EYE_SCAN
// ETT_NO_DRAM #endif   

#if ETT_NO_DRAM    
    //NoDramDramcRegDump(p);    
    NoDramRegFill();
#endif

    // scy: set hf_femi_ck to DMPLLCH0_CK 
    DRV_WriteReg32(0x102101B0, 0x04030204);

    //EMI/DRAMC clock source using channel 2 (check reg 0x102101b0[11:8] = 0xB)
    *(volatile unsigned int*) 0x102101b0 = (*((volatile unsigned int*) 0x102101b0) & 0xfffff8ff ) | 0x300;

    #if CPU_RW_TEST_AFTER_K
    mcSHOW_DBG_MSG(("\n[MEM_TEST] 02: After DFS, before run time config\n"));
    vDramCPUReadWriteTestAfterCalibration(p);
    //vDramCPUReadWriteTestAfterCalibration2(p);
    #endif
    
    mcSHOW_DBG_MSG(("\n\nSettings after calibration ...\n\n"));
    DramcRunTimeConfig(p);
    
    #if CPU_RW_TEST_AFTER_K
    mcSHOW_DBG_MSG(("\n[MEM_TEST] 03: After run time config\n"));
    vDramCPUReadWriteTestAfterCalibration(p);
    //vDramCPUReadWriteTestAfterCalibration2(p);
    #endif

#if (__ETT__ && CPU_RW_TEST_AFTER_K)
    /* 0x46000000 is LK base addr */
    //if ((s4value = complex_mem_test (0x46000000, 0x2000)) == 0)
    if ((s4value = complex_mem_test (0x40024000, 0x2000)) == 0)
    {
        mcSHOW_DBG_MSG(("[Francis] 1st complex R/W mem test pass\n"));
    }
    else
    {
        mcSHOW_DBG_MSG(("[Francis] 1st complex R/W mem test fail :-%d\n", -s4value));
    }
#endif	    


#endif//SW_CHANGE_FOR_SIMULATION
    return 0;
}


#if (FOR_DV_SIMULATION_USED!=0)
void DPI_main(void)
{
    DRAM_INFO_BY_MRR_T DramInfo;
    DRAMC_CTX_T DramConfig;
#if SUPPORT_SAVE_OFF_LINE_CALIBRATION
    SAVE_OFF_LINE_CALIBRATION_T SaveOffLineData;
#endif

    DramConfig.channel = CHANNEL_A;
    DramConfig.support_rank_num = RANK_DUAL;
    // DramRank
    DramConfig.rank = RANK_0;
    // DRAM type
    DramConfig.dram_type = TYPE_LPDDR3;
    // DRAM Fast switch point type, only for LP4, useless in LP3
    DramConfig.dram_fsp = FSP_0;
    // DRAM CBT mode, only for LP4, useless in LP3
    DramConfig.dram_cbt_mode = CBT_NORMAL_MODE;    
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
    DramConfig.u2DelayCellTimex100 = 0;
    DramConfig.fglow_freq_write_en = DISABLE;
    // Switch to ENABLE or DISABLE SSC (Spread Spectrum Clock)
    DramConfig.ssc_en = DISABLE;    
    DramConfig.en_4bitMux = DISABLE;

    DramConfig.enable_rx_scan_vref =DISABLE;
    DramConfig.enable_tx_scan_vref =DISABLE;
    //DramConfig.dynamicODT = DISABLE;    

    printf("main functino start!\n");

#if ENABLE_MRR_AFTER_FIRST_K 
    Init_DRAM(DramConfig.dram_type, DramConfig.dram_cbt_mode, &DramInfo, FIRST_USED);
#endif
    Init_DRAM(DramConfig.dram_type, DramConfig.dram_cbt_mode, NULL, NORMAL_USED);
}

#define DV_SIMULATION_INIT_C    1
#define DV_SIMULATION_BEFORE_K  1
#define DV_SIMULATION_SW_IMPED 0
#define DV_SIMULATION_CA_TRAINING 0
#define DV_SIMULATION_WRITE_LEVELING  0
#define DV_SIMULATION_GATING 0
#define DV_SIMULATION_DATLAT 0
#define DV_SIMULATION_RX_PERBIT    0
#define DV_SIMULATION_TX_PERBIT    0 // Please enable with write leveling
#define DV_SIMULATION_AFTER_K   0
#define DV_SIMULATION_RUN_TIME_MRW 0

#if ENABLE_LP3_SW
void DPI_SW_main_LP3(DRAMC_CTX_T *DramConfig)
{
    int ii;

    DramConfig->channel = CHANNEL_A;
    DramConfig->support_rank_num = RANK_DUAL;
    // DramRank
    DramConfig->rank = RANK_0;
    // DRAM type
    DramConfig->dram_type = TYPE_LPDDR3;
    // DRAM Fast switch point type, only for LP4, useless in LP3
    DramConfig->dram_fsp = FSP_0;
    // DRAM CBT mode, only for LP4, useless in LP3
    DramConfig->dram_cbt_mode = CBT_NORMAL_MODE;    
    // IC and DRAM package
    DramConfig->package = PACKAGE_POP;
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
    DramConfig->fglow_freq_write_en = DISABLE;
    // Switch to ENABLE or DISABLE SSC (Spread Spectrum Clock)
    DramConfig->ssc_en = DISABLE;    
    DramConfig->en_4bitMux = DISABLE;

    DramConfig->enable_rx_scan_vref =DISABLE;
    DramConfig->enable_tx_scan_vref =DISABLE;
    //DramConfig->dynamicODT = DISABLE;

    Global_Option_Init(DramConfig);

    vSetPHY2ChannelMapping(DramConfig, DramConfig->channel);

    #if DV_SIMULATION_SW_IMPED
    DramcSwImpedanceCal(DramConfig,1, 1);  //within term
    #endif


#if DV_SIMULATION_INIT_C
    DramcInit(DramConfig);
    #ifdef ENABLE_MIOCK_JMETER
    DramcMiockJmeter(DramConfig);
    #endif
#endif

#if WRITE_LEVELING_MOVE_DQS_INSTEAD_OF_CLK    
    memset(DramConfig->arfgWriteLevelingInitShif, FALSE, sizeof(DramConfig->arfgWriteLevelingInitShif));
    //>fgWriteLevelingInitShif= FALSE;
#endif
#if TX_PERBIT_INIT_FLOW_CONTROL
    memset(DramConfig->fgTXPerbifInit, FALSE, sizeof(DramConfig->fgTXPerbifInit));
#endif

#if DV_SIMULATION_TX_PERBIT
    //INIT+TX_K : set CA/TX delay chain to 0
    //INIT only : keep CA/TX delay chain
    vResetDelayChainBeforeCalibration(DramConfig);
#endif

#if DV_SIMULATION_BEFORE_K
    vApplyConfigBeforeCalibration(DramConfig);
    vMR2InitForSimulationTest(DramConfig);
#endif

#if DV_SIMULATION_CA_TRAINING
    vSetRank(DramConfig, RANK_0);
    CATrainingLP3(DramConfig);
#endif


    for(ii=RANK_0; ii<RANK_MAX; ii++)
    {
        vSetRank(DramConfig, ii);

        #if DV_SIMULATION_WRITE_LEVELING
            DramcWriteLeveling(DramConfig);
        #endif

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
            DramcTxWindowPerbitCal(DramConfig, TX_DQ_DQS_MOVE_DQ_DQM);
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
    DramConfig->dram_cbt_mode = CBT_NORMAL_MODE;
    DramConfig->DBI_R_onoff = DBI_OFF;
    DramConfig->DBI_W_onoff = DBI_OFF;
    // IC and DRAM package
    DramConfig->package = PACKAGE_POP;
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
    DramConfig->vendor_id = 0x88;
    DramConfig->density = 0;
    //DramConfig->ranksize = {0,0};
    // Switch to ENABLE or DISABLE low frequency write and high frequency read
    DramConfig->fglow_freq_write_en = DISABLE;
    // Switch to ENABLE or DISABLE SSC (Spread Spectrum Clock)
    DramConfig->ssc_en = DISABLE;    
    DramConfig->en_4bitMux = DISABLE;

    DramConfig->enable_cbt_scan_vref = DISABLE;
    DramConfig->enable_rx_scan_vref =DISABLE;
    DramConfig->enable_tx_scan_vref =DISABLE;
    //DramConfig->dynamicODT = DISABLE;

    Global_Option_Init(DramConfig);

    DDRPhyFreqSel(DramConfig, DramConfig->freq_sel);

    vSetPHY2ChannelMapping(DramConfig, DramConfig->channel);

#if DV_SIMULATION_SW_IMPED
    if (DramConfig->dram_type == TYPE_LPDDR4X)
    {
        DramcSwImpedanceCal(DramConfig,1, 0);  //without term
    }
    DramcSwImpedanceCal(DramConfig,1, 1);  //within term
#endif


#if DV_SIMULATION_INIT_C
    DramcInit(DramConfig);
    #ifdef ENABLE_MIOCK_JMETER
    DramcMiockJmeter(DramConfig);
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

#if DV_SIMULATION_TX_PERBIT
    //INIT+TX_K : set CA/TX delay chain to 0
    //INIT only : keep CA/TX delay chain
    vResetDelayChainBeforeCalibration(DramConfig);
#endif

#if DV_SIMULATION_BEFORE_K
    vApplyConfigBeforeCalibration(DramConfig);
    vMR2InitForSimulationTest(DramConfig);
#endif

#if DV_SIMULATION_GATING
DramcRxdqsGatingPreProcess(DramConfig);
#endif

#if SIMULATION_LP4_ZQ
         DramcZQCalibration(DramConfig);
#endif

#if DV_SIMULATION_CA_TRAINING
    vSetRank(DramConfig, RANK_0);
    CmdBusTrainingLP4(DramConfig);
#endif


    for(ii=RANK_0; ii<RANK_MAX; ii++)
    {
        vSetRank(DramConfig, ii);

#if DV_SIMULATION_WRITE_LEVELING
        //if (ii==RANK_1)
        DramcWriteLeveling(DramConfig);
#endif

        #if DV_SIMULATION_GATING
        // Gating calibration of single rank 
        DramcRxdqsGatingCal(DramConfig);
        #endif

        #if DV_SIMULATION_RX_PERBIT
            DramcRxWindowPerbitCal(DramConfig, 0);
        #endif
        
        #if DV_SIMULATION_TX_PERBIT
            DramcTxWindowPerbitCal(DramConfig, TX_DQ_DQS_MOVE_DQ_DQM);
        #endif

        #if DV_SIMULATION_DATLAT
        // RX Datlat calibration of single rank
        DramcRxdatlatCal(DramConfig);
        #endif

        #if DV_SIMULATION_RX_PERBIT
        DramcRxWindowPerbitCal(DramConfig, 1);
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

#if DV_SIMULATION_RUN_TIME_MRW
    enter_pasr_dpd_config(0, 0xFF);
#endif
}

#endif


