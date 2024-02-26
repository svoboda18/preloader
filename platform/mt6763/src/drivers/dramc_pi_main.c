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
#include "upmu_hw.h"
#ifndef MT6763_FPGA
#include <rt5738.h>
#include <pmic.h>
#endif
#endif

#if ! __ETT__
#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
#include "pmic.h"
#ifdef DDR_RESERVE_MODE  
#include "platform.h"
#include "wdt.h"
#endif
#endif
#endif

#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
#if USE_PMIC_CHIP_MT6356
#include <regulator/mtk_regulator.h>
extern struct mtk_regulator reg_vdram, reg_vcore;
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

#ifdef DDR_INIT_TIME_PROFILING
static U32 CPUCycleSum=0;
#endif

DRAM_DFS_FREQUENCY_TABLE_T gFreqTbl[DRAM_DFS_SHUFFLE_MAX] = {
    {LP4_DDR3200, 1600, DRAM_DFS_SHUFFLE_1},
    {LP4_DDR1600,  800, DRAM_DFS_SHUFFLE_2},
    {LP4_DDR2400, 1200, DRAM_DFS_SHUFFLE_3},
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
  PACKAGE_POP,        // DRAM_PACKAGE_T
  DATA_WIDTH_32BIT,     // DRAM_DATA_WIDTH_T
  DEFAULT_TEST2_1_CAL,    // test2_1;
  DEFAULT_TEST2_2_CAL,    // test2_2;
  TEST_XTALK_PATTERN,     // test_pattern;
  667,                  // frequency
  667,                  // freqGroup
  {0,0,0,0},            // shuffle_frequency
  0x88, //vendor_id initial value
  0, //revision id
  0, //density
  {0,0},
  0,  // ucnum_dlycell_perT;  
  0,  // u2DelayCellTimex100;
  DISABLE,  // fglow_freq_write_en;
  DISABLE,  // ssc_en;
  DISABLE,   // en_4bitMux;
  
  DISABLE,  // enable_cbt_scan_vref;
  DISABLE,  // enable_rx_scan_vref;
  DISABLE,   // enable_tx_scan_vref;
#if SIMPLIFY_VREF_SCAN
  DISABLE,   // skipVrefScan;
#endif
#if SUPPORT_SAVE_OFF_LINE_CALIBRATION
  NULL,
#endif

   //aru4CalResultFlag[CHANNEL_NUM][RANK_MAX][DRAM_DFS_MAX]
{{{0,0,0,0,},  {0,0,0,0}},
  {{0,0,0,0,}, {0,0,0,0}}}, 
//aru4CalExecuteFlag[CHANNEL_NUM][RANK_MAX][DRAM_DFS_MAX]
{{{0,0,0,0,},  {0,0,0,0}},
  {{0,0,0,0,}, {0,0,0,0}}}, 

#if WRITE_LEVELING_MOVE_DQS_INSTEAD_OF_CLK
  {{0,0}, {0,0}}, //BOOL arfgWriteLevelingInitShif;
#endif
#if TX_PERBIT_INIT_FLOW_CONTROL
  {{FALSE, FALSE}, {FALSE, FALSE}},//BOOL fgTXPerbifInit;
#endif
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
     &SavetimeData,
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
  LP4_DDR1600,
#else
#if __FLASH_TOOL_DA__
  LP4_DDR1600,
#else
  LP4_DDR2400,
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
  PACKAGE_POP,        // DRAM_PACKAGE_T
  DATA_WIDTH_16BIT,     // DRAM_DATA_WIDTH_T
  DEFAULT_TEST2_1_CAL,    // test2_1;
  DEFAULT_TEST2_2_CAL,    // test2_2;
  TEST_XTALK_PATTERN,     // test_pattern;
  800,                  // frequency
  800,                  // freqGroup
  {0,0,0,0},            // shuffle_frequency  
  0x88, //vendor_id initial value  
  0, //revision id
  0, //density
  {0,0},  
  0,  // ucnum_dlycell_perT;
  0,  // u2DelayCellTimex100;
  DISABLE,  // fglow_freq_write_en;
  DISABLE,  // ssc_en;
  DISABLE,   // en_4bitMux;
  
  ENABLE,   // enable_cbt_scan_vref;
  ENABLE,  // enable_rx_scan_vref;
  ENABLE,   // enable_tx_scan_vref;
#if SIMPLIFY_VREF_SCAN
  DISABLE, // skipVrefScan: disabled by default. Enable individually in FreqSel()
#endif
#if SUPPORT_SAVE_OFF_LINE_CALIBRATION
  NULL,
#endif

   //aru4CalResultFlag[CHANNEL_NUM][RANK_MAX][DRAM_DFS_MAX]
{{{0,0,0,0,},  {0,0,0,0}},
  {{0,0,0,0,}, {0,0,0,0}}}, 
//aru4CalExecuteFlag[CHANNEL_NUM][RANK_MAX][DRAM_DFS_MAX]
{{{0,0,0,0,},  {0,0,0,0}},
  {{0,0,0,0,}, {0,0,0,0}}}, 

#if WRITE_LEVELING_MOVE_DQS_INSTEAD_OF_CLK
  {{0,0}, {0,0}}, //BOOL arfgWriteLevelingInitShif;
#endif
#if TX_PERBIT_INIT_FLOW_CONTROL
  {{FALSE, FALSE}, {FALSE, FALSE}},//BOOL fgTXPerbifInit;
#endif
    	#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
            &SavetimeData,
	#endif
};

#if ENABLE_LP3_SW
U8 const *uiLPDDR3_PHY_Mapping_POP = NULL;
#endif


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

#ifdef DDR_RESERVE_MODE
#define DRAMC_DPY_CLK_SW_CON2 (0x10006474) 
#define LP3_CKE_FIX_ON_TO_LEAVE_SR_WITH_LVDRAM 0
static U32 u4Cha_mr4_backup = 0;
static U32 u4Chb_mr4_backup = 0;
static U32 u4Cha_dqsosc_en_backup = 0;
static U32 u4Chb_dqsosc_en_backup = 0;
static U32 u4Cha_dqsosc_rd_backup = 0;
static U32 u4Chb_dqsosc_rd_backup = 0;
static U32 u4Cha_dummy_read_backup = 0;
static U32 u4Chb_dummy_read_backup = 0;
static U32 u4Cha_phy_clk_dyn_backup = 0;
static U32 u4Chb_phy_clk_dyn_backup = 0;
#if LP3_CKE_FIX_ON_TO_LEAVE_SR_WITH_LVDRAM
#if ENABLE_LP3_SW
static U32 u4Cha_cke_backup = 0;
static U32 u4Chb_cke_backup = 0;
#endif
#endif

static int Reserve_Sync_Writel(U32 addr, unsigned int val)
{
	(*(volatile unsigned int *)(addr)) = val;
	dsb();
	return 0;
}

unsigned int Reserve_Reg_Readl(U32 addr)
{
	return (*(volatile unsigned int *)(addr));
}

//#define SR_ACK 0x03 //LP4 CHA CHB
//#define SR_ACK 0x02 //LP4 CHB
//#define SR_ACK 0x01 //LP3 CHA
#define PDEF_EMI_CLK_OFF_REQ_LSB 1
static void Func_Emi_On(DRAMC_CTX_T * p)//Exit SR
{
    int iDelayUs = 0;
    U32 u4value = 0;
    U32 u4ACK = 3;

    u4ACK = (u1IsLP4Family(p->dram_type)) ? 3 : 1;
    vIO32WriteFldAlign(SPM_POWER_ON_VAL1, 0, SPM_POWER_ON_VAL1_EMI_CLK_OFF_REQ_PCM);
    for(iDelayUs=2; iDelayUs>0; iDelayUs--)
    {
        mcDELAY_US(1);
        if((u4ACK & u4IO32ReadFldAlign(SPM_DRAMC_DPY_CLK_SW_CON, DRAMC_DPY_CLK_SW_CON_SC_EMI_CLK_OFF_ACK)) == 0)
        {
            break;
        }
    }
    if(iDelayUs==0) //no issue Ack =>status = 1
    {
        mcSHOW_ERR_MSG(("!Wait no ack(Leave SR)[0x%X]\n", u4value));
    }
    return;
}

static void Func_Emi_Off(DRAMC_CTX_T * p)//Enter SR
{
    int iDelayUs = 0;
    U32 u4value = 0;
    U32 u4ACK = 3;

    u4ACK = (u1IsLP4Family(p->dram_type)) ? 3 : 1;
    vIO32WriteFldAlign(SPM_POWER_ON_VAL1, 1, SPM_POWER_ON_VAL1_EMI_CLK_OFF_REQ_PCM);
    for(iDelayUs=2; iDelayUs>0; iDelayUs--)
    {
        mcDELAY_US(1);
        u4value = Reserve_Reg_Readl(SPM_DRAMC_DPY_CLK_SW_CON);
        if(u4ACK & u4IO32ReadFldAlign(SPM_DRAMC_DPY_CLK_SW_CON, DRAMC_DPY_CLK_SW_CON_SC_EMI_CLK_OFF_ACK))
        {
            break;
        }
    }
    if(iDelayUs==0) //no issue Ack =>status = 1
    {
        mcSHOW_ERR_MSG(("!Wait no ack(Enter SR)[0x%X]\n", u4value));
        vIO32WriteFldAlign(SPM_POWER_ON_VAL1, 0, SPM_POWER_ON_VAL1_EMI_CLK_OFF_REQ_PCM);
    }
    return;
}

#define RE_POWERON_CONFIG_EN               (0x10006000)
#define RE_PCM_PWR_IO_EN                   (0x1000602C)
#define RE_DRAMC_DPY_CLK_SW_CON_SEL        (0x10006460)
#define RE_DRAMC_DPY_CLK_SW_CON_SEL2       (0x10006470)
#define RE_SPM_S1_MODE_CH                  (0x10006468)
void Set_Spm_Poweron_Config_En(void)
{
    U32 u4value = 0;

    Reserve_Sync_Writel(RE_POWERON_CONFIG_EN, 0x0B160001);
    Reserve_Sync_Writel(RE_PCM_PWR_IO_EN, 0);
    Reserve_Sync_Writel(RE_DRAMC_DPY_CLK_SW_CON_SEL, 0xffffffff);
    Reserve_Sync_Writel(RE_DRAMC_DPY_CLK_SW_CON_SEL2, 0xffffffff);
    u4value = Reserve_Reg_Readl(SPM_POWER_ON_VAL0);
    u4value |= ((0x1 << 8) | (0x1 << 12) | (0x1 << 13) | (0x1 << 14) | (0x1 << 15)); 
    Reserve_Sync_Writel(SPM_POWER_ON_VAL0, u4value);
    u4value = Reserve_Reg_Readl(RE_SPM_S1_MODE_CH);
    u4value |= 0x3;
    Reserve_Sync_Writel(SPM_S1_MODE_CH, u4value);

#if 0
    u4value = Reserve_Reg_Readl(SPM_POWER_ON_VAL0);
    mcSHOW_DBG_MSG(("SPM_POWER_ON_VAL0 =[0x%X] \n", u4value));
    u4value = Reserve_Reg_Readl(RE_POWERON_CONFIG_EN);
    mcSHOW_DBG_MSG(("POWERON_CONFIG_EN =[0x%X] \n", u4value));
    u4value = Reserve_Reg_Readl(RE_PCM_PWR_IO_EN);
    mcSHOW_DBG_MSG(("PCM_PWR_IO_EN =[0x%X] \n", u4value));
    u4value = Reserve_Reg_Readl(RE_DRAMC_DPY_CLK_SW_CON_SEL);
    mcSHOW_DBG_MSG(("DRAMC_DPY_CLK_SW_CON_SEL =[0x%X] \n", u4value));
    u4value = Reserve_Reg_Readl(RE_DRAMC_DPY_CLK_SW_CON_SEL2);
    mcSHOW_DBG_MSG(("DRAMC_DPY_CLK_SW_CON_SEL2 =[0x%X] \n", u4value));
    u4value = Reserve_Reg_Readl(RE_SPM_S1_MODE_CH);
    mcSHOW_DBG_MSG(("SPM_S1_MODE_CH =[0x%X] \n", u4value));
#endif    
    return;
}

void dump_SR(U32 u4DramType)
{
    U32 u4value = 0;

    u4value = u4IO32ReadFldAlign(DRAMC_REG_MISC_STATUSA, MISC_STATUSA_SREF_STATE);
    mcSHOW_ERR_MSG(("!CHA SREF_STATE[0x%X]\n", u4value));
    if((u4DramType == 2) || (u4DramType == 3))
    { 
        u4value = u4IO32ReadFldAlign(DRAMC_REG_MISC_STATUSA + ((U32) CHANNEL_B << POS_BANK_NUM), MISC_STATUSA_SREF_STATE);
        mcSHOW_ERR_MSG(("!CHB SREF_STATE[0x%X]\n", u4value));
    }
    return;
}

void dump_gating_error_rg(U32 u4DramType)
{
    U32 u4value = 0;

    u4value = u4IO32Read4B(DDRPHY_MISC_STBERR_RK0_R);
    mcSHOW_ERR_MSG(("###DDR reserve CHA Gating error[0x%X]\n", u4value));
    if((u4DramType == 2) || (u4DramType == 3))
    {
        u4value = u4IO32Read4B(DDRPHY_MISC_STBERR_RK0_R + ((U32) CHANNEL_B << POS_BANK_NUM));
        mcSHOW_ERR_MSG(("###DDR reserve CHB Gating error[0x%X]\n", u4value));
    }
    return;
}

//#define USE_CDC_OPTION_PATCH
#ifdef USE_CDC_OPTION_PATCH
/* Only use while CDC option is 1 originally. But we use the patch for gating/tx/rx delay reload from AO to NAO in M17 since CDC_OPTION is 0*/
static void DDR_Reserved_Mode_Cdc_Option_Patch(DRAMC_CTX_T *p)
{
    U32 u4ChannelIdx = 0;
    U32 u4BkChannel = 0;
    U32 u4BackupReg0x38[2]= {0};
    U32 u4backupReg0x298[2]= {0};
    U32 u4backupReg0x218[2]= {0};
    U32 u4backupReg0x200[2]= {0};
    U32 u4backupReg0x2A0[2]= {0};
    U32 u4backupReg0x5F0[2]= {0};
    U32 u4backupReg0x670[2]= {0};
    U32 u4backupReg0x608[2]= {0};
    U32 u4backupReg0x808[2]= {0};
    U32 u4backupReg0x688[2]= {0};
    U32 u4backupReg0x888[2]= {0};

    //Backup regs
    u4BkChannel = vGetPHY2ChannelMapping(p);
    for(u4ChannelIdx = CHANNEL_A; u4ChannelIdx < p->support_channel_num; u4ChannelIdx++)
    {
        vSetPHY2ChannelMapping(p, u4ChannelIdx);
        u4BackupReg0x38[u4ChannelIdx] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL));
        u4backupReg0x298[u4ChannelIdx] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_CG_CTRL5));
        u4backupReg0x218[u4ChannelIdx] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_PRE_TDQSCK1));
        u4backupReg0x200[u4ChannelIdx] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_STBCAL));
        u4backupReg0x2A0[u4ChannelIdx] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1));
        u4backupReg0x5F0[u4ChannelIdx] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_B0_RXDVS0));
        u4backupReg0x670[u4ChannelIdx] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_B1_RXDVS0));
        u4backupReg0x608[u4ChannelIdx] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS2));
        u4backupReg0x808[u4ChannelIdx] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_R1_B0_RXDVS2));
        u4backupReg0x688[u4ChannelIdx] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2));
        u4backupReg0x888[u4ChannelIdx] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_R1_B1_RXDVS2));
    }

	mcSHOW_DBG_MSG(("Apply DDR_Reserved_Mode_Cdc_Option_Patch\n"));

    //1. Toggle for shuffle level reset
    //vIO32WriteFldAlign_All(DRAMC_REG_SHUCTRL2, 0, SHUCTRL2_R_DVFS_CDC_OPTION);//Lynx: Need to toggle this bit to toggle shuffle level from spm side band to conf AO
    //mcDELAY_US(1);//delay 20NS
    //vIO32WriteFldAlign_All(DRAMC_REG_SHUCTRL2, 1, SHUCTRL2_R_DVFS_CDC_OPTION);    

    //2. DQ DCM OFF
    vIO32WriteFldAlign_All(DRAMC_REG_DRAMC_PD_CTRL, 0, DRAMC_PD_CTRL_COMBCLKCTRL);
    //3. TX DLY/PI DCM OFF
    vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL5, P_Fld(0x0, MISC_CG_CTRL5_R_CA_PI_DCM_EN)
                | P_Fld(0x0, MISC_CG_CTRL5_R_DQ0_PI_DCM_EN)
                | P_Fld(0x0, MISC_CG_CTRL5_R_DQ1_PI_DCM_EN)
                | P_Fld(0x0, MISC_CG_CTRL5_R_CA_DLY_DCM_EN)
                | P_Fld(0x0, MISC_CG_CTRL5_R_DQ0_DLY_DCM_EN)
                | P_Fld(0x0, MISC_CG_CTRL5_R_DQ1_DLY_DCM_EN));
    //4. Dramc_idle OFF
    vIO32WriteFldAlign_All(DRAMC_REG_DRAMC_PD_CTRL, 0, DRAMC_PD_CTRL_DCMEN2);
    //5. TX tracking DCM disable
    vIO32WriteFldAlign_All(DRAMC_REG_PRE_TDQSCK1, 1, PRE_TDQSCK1_TXUIPI_CAL_CGAR);
    //6. Gating tracking disable
    vIO32WriteFldMulti_All(DRAMC_REG_STBCAL, P_Fld(0x0, STBCAL_STBCALEN)
                | P_Fld(0x0, STBCAL_STB_SELPHYCALEN));
    //Gating tracking Enable
    vIO32WriteFldMulti_All(DRAMC_REG_STBCAL, P_Fld(0x1, STBCAL_STBCALEN)
                | P_Fld(0x1, STBCAL_STB_SELPHYCALEN));    
    //7. Load TX PI from conf to tx tracking
    vIO32WriteFldAlign_All(DDRPHY_MISC_CTRL1, 1, MISC_CTRL1_R_DMARPIDQ_SW);
    vIO32WriteFldAlign_All(DDRPHY_MISC_CTRL1, 0, MISC_CTRL1_R_DMARPIDQ_SW);
    //8. TX tracking DCM Enable 
    vIO32WriteFldAlign_All(DRAMC_REG_PRE_TDQSCK1, 0, PRE_TDQSCK1_TXUIPI_CAL_CGAR);

    //9. RX delay reload from conf to RX DLY tracking
    vIO32WriteFldAlign_All(DDRPHY_B0_RXDVS0, 1, B0_RXDVS0_R_RX_DLY_TRACK_CLR_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_RXDVS0, 1, B1_RXDVS0_R_RX_DLY_TRACK_CLR_B1);

    vIO32WriteFldAlign_All(DDRPHY_B0_RXDVS0, 0, B0_RXDVS0_R_RX_DLY_TRACK_CLR_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_RXDVS0, 0, B1_RXDVS0_R_RX_DLY_TRACK_CLR_B1);

    vIO32WriteFldAlign_All(DDRPHY_R0_B0_RXDVS2, 0, R0_B0_RXDVS2_R_RK0_DVS_MODE_B0);
    vIO32WriteFldAlign_All(DDRPHY_R1_B0_RXDVS2, 0, R1_B0_RXDVS2_R_RK1_DVS_MODE_B0);
    vIO32WriteFldAlign_All(DDRPHY_R0_B1_RXDVS2, 0, R0_B1_RXDVS2_R_RK0_DVS_MODE_B1);
    vIO32WriteFldAlign_All(DDRPHY_R1_B1_RXDVS2, 0, R1_B1_RXDVS2_R_RK1_DVS_MODE_B1);

    vIO32WriteFldAlign_All(DDRPHY_R0_B0_RXDVS2, 2, R0_B0_RXDVS2_R_RK0_DVS_MODE_B0);
    vIO32WriteFldAlign_All(DDRPHY_R1_B0_RXDVS2, 2, R1_B0_RXDVS2_R_RK1_DVS_MODE_B0);
    vIO32WriteFldAlign_All(DDRPHY_R0_B1_RXDVS2, 2, R0_B1_RXDVS2_R_RK0_DVS_MODE_B1);
    vIO32WriteFldAlign_All(DDRPHY_R1_B1_RXDVS2, 2, R1_B1_RXDVS2_R_RK1_DVS_MODE_B1);

    //10. DQ DCM ON
    vIO32WriteFldAlign_All(DRAMC_REG_DRAMC_PD_CTRL, 1, DRAMC_PD_CTRL_COMBCLKCTRL);
    //11. Dramc_idle ON
    vIO32WriteFldAlign_All(DRAMC_REG_DRAMC_PD_CTRL, 1, DRAMC_PD_CTRL_DCMEN2);
    //12. TX DLY/PI DCM ON
    vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL5, P_Fld(0x1, MISC_CG_CTRL5_R_CA_PI_DCM_EN)
                | P_Fld(0x1, MISC_CG_CTRL5_R_DQ0_PI_DCM_EN)
                | P_Fld(0x1, MISC_CG_CTRL5_R_DQ1_PI_DCM_EN)
                | P_Fld(0x1, MISC_CG_CTRL5_R_CA_DLY_DCM_EN)
                | P_Fld(0x1, MISC_CG_CTRL5_R_DQ0_DLY_DCM_EN)
                | P_Fld(0x1, MISC_CG_CTRL5_R_DQ1_DLY_DCM_EN));

    //Restore regs
    for(u4ChannelIdx = CHANNEL_A; u4ChannelIdx < p->support_channel_num; u4ChannelIdx++)
    {
        vSetPHY2ChannelMapping(p, u4ChannelIdx);
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), u4BackupReg0x38[u4ChannelIdx]);
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_MISC_CG_CTRL5), u4backupReg0x298[u4ChannelIdx]);
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_PRE_TDQSCK1), u4backupReg0x218[u4ChannelIdx]);
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), u4backupReg0x200[u4ChannelIdx]);
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), u4backupReg0x2A0[u4ChannelIdx]);
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B0_RXDVS0), u4backupReg0x5F0[u4ChannelIdx]);
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B1_RXDVS0), u4backupReg0x670[u4ChannelIdx]);
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS2), u4backupReg0x608[u4ChannelIdx]);
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_R1_B0_RXDVS2), u4backupReg0x808[u4ChannelIdx]);
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2), u4backupReg0x688[u4ChannelIdx]);
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_R1_B1_RXDVS2), u4backupReg0x888[u4ChannelIdx]);
    }
    vSetPHY2ChannelMapping(p, u4BkChannel);

    return;
}
#endif

#define RESERVE_PDEF_SPM_PLL_CON (0x1000644C)
void Dramc_DDR_Reserved_Mode_setting(void)
{
    U32 u4WbrBackup = 0;
    U32 u4DramType = 0;
    U32 u4value = 0;
    U32 backup_broadcast = GetDramcBroadcast();
    DRAMC_CTX_T * p;

    mcSHOW_DBG_MSG(("Enter Dramc_DDR_Reserved_Mode_setting \n"));
    Set_Spm_Poweron_Config_En();
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);	
    u4DramType = (*((volatile unsigned int *)(0x1022A010)) >> 10 & 0x7);
    mcSHOW_DBG_MSG(("Dram type =%x \n", u4DramType));
    if(u4DramType == 1)
    {
        psCurrDramCtx = &DramCtx_LPDDR3;
        p = psCurrDramCtx; 
    } 
    else if((u4DramType == 2) || (u4DramType == 3))
    {   // LPDDR4
        psCurrDramCtx = &DramCtx_LPDDR4;
        p = psCurrDramCtx;     
    }
    /* Could use our API safely */
    SaveCurrDramCtx(p);

    u4value = u4IO32ReadFldAlign(DRAMC_REG_SHUSTATUS, SHUSTATUS_SHUFFLE_LEVEL);
    mcSHOW_ERR_MSG(("### shuffle level[%d]\n", u4value));
#ifdef USE_CDC_OPTION_PATCH
    DDR_Reserved_Mode_Cdc_Option_Patch(p);
#endif
#ifdef HW_GATING
    DramcHWGatingOnOff(p,0);//Disable HW Gating tracking for gating tracking fifo mode
#endif
    u4DramType = u4IO32ReadFldAlign(DRAMC_REG_ARBCTL, ARBCTL_RSV_DRAM_TYPE);
    mcSHOW_DBG_MSG(("Dramc_DDR_Reserved_Mode_setting dram type =%x \n", u4DramType));
    vIO32WriteFldMulti(SPM_POWER_ON_VAL0, P_Fld(1, SPM_POWER_ON_VAL0_SC_PHYPLL_SHU_EN_PCM) 
                | P_Fld(1, SPM_POWER_ON_VAL0_SC_PHYPLL2_SHU_EN_PCM));//Set spm shuffle enable to 1
    mcDELAY_US(1);
    vIO32WriteFldMulti(SPM_POWER_ON_VAL0, P_Fld(0, SPM_POWER_ON_VAL0_SC_PHYPLL_SHU_EN_PCM) 
                | P_Fld(0, SPM_POWER_ON_VAL0_SC_PHYPLL2_SHU_EN_PCM));//Set spm shuffle enable to 0

    if(u4DramType == 1)//LPDDR3
    {
        vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0 + ((U32)CHANNEL_B<<POS_BANK_NUM), 0, MISC_CG_CTRL0_RG_CG_DRAMC_CHB_CK_OFF);//Open CHB controller clk
    }
    //MR4
    u4Cha_mr4_backup = u4IO32Read4B(DRAMC_REG_SPCMDCTRL);
    u4Chb_mr4_backup = u4IO32Read4B(DRAMC_REG_SPCMDCTRL + ((U32)CHANNEL_B<<POS_BANK_NUM));
    vIO32WriteFldAlign_All(DRAMC_REG_SPCMDCTRL, 1, SPCMDCTRL_REFRDIS);

    //DQSOSC en
    u4Cha_dqsosc_en_backup = u4IO32Read4B(DRAMC_REG_SHU_SCINTV);
    u4Chb_dqsosc_en_backup = u4IO32Read4B(DRAMC_REG_SHU_SCINTV + ((U32)CHANNEL_B<<POS_BANK_NUM));
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_SCINTV, 1, SHU_SCINTV_DQSOSCENDIS);

    //DQSOSC rd
    u4Cha_dqsosc_rd_backup = u4IO32Read4B(DRAMC_REG_DQSOSCR);
    u4Chb_dqsosc_rd_backup = u4IO32Read4B(DRAMC_REG_DQSOSCR + ((U32)CHANNEL_B<<POS_BANK_NUM));
    vIO32WriteFldAlign_All(DRAMC_REG_DQSOSCR, 1, DQSOSCR_DQSOSCRDIS);

    //Dummy Read
    u4Cha_dummy_read_backup = u4IO32Read4B(DRAMC_REG_DUMMY_RD);
    u4Chb_dummy_read_backup = u4IO32Read4B(DRAMC_REG_DUMMY_RD + ((U32)CHANNEL_B<<POS_BANK_NUM));

    //PHY clk
    u4Cha_phy_clk_dyn_backup = u4IO32Read4B(DRAMC_REG_DRAMC_PD_CTRL);
    u4Chb_phy_clk_dyn_backup = u4IO32Read4B(DRAMC_REG_DRAMC_PD_CTRL + ((U32)CHANNEL_B<<POS_BANK_NUM));
    vIO32WriteFldAlign_All(DRAMC_REG_DRAMC_PD_CTRL, 0, DRAMC_PD_CTRL_PHYCLKDYNGEN);//disable DDRPHY dynamic clock gating

    //To 26MHz
    if((u4DramType == 2) || (u4DramType == 3))
    {
        vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL0, P_Fld(0, MISC_CG_CTRL0_W_CHG_MEM) 
                    | P_Fld(0, MISC_CG_CTRL0_CLK_MEM_SEL));//[5:4] mem_ck mux: 2'b00: 26MHz, [0]: change memory clock
        vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_W_CHG_MEM);//change clock freq
        mcDELAY_US(1);
        vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_W_CHG_MEM);//disable memory clock change
    }
    else//LP3
    {
#if LP3_CKE_FIX_ON_TO_LEAVE_SR_WITH_LVDRAM
#if ENABLE_LP3_SW
        //Dram clock free run
        vIO32WriteFldAlign_All(DRAMC_REG_DRAMC_PD_CTRL, 1, DRAMC_PD_CTRL_MIOCKCTRLOFF);//disable DDRPHY dynamic clock gating

        //CKE control
        u4Cha_cke_backup = u4IO32Read4B(DRAMC_REG_CKECTRL);
        u4Chb_cke_backup = u4IO32Read4B(DRAMC_REG_CKECTRL + ((U32)CHANNEL_B<<POS_BANK_NUM));
        //CKE fix on
        vIO32WriteFldMulti_All(DRAMC_REG_CKECTRL, P_Fld(0x0, CKECTRL_CKE1FIXOFF) | 
            P_Fld(0x0, CKECTRL_CKEFIXOFF) |
            P_Fld(0x1, CKECTRL_CKE1FIXON) |
            P_Fld(0x1, CKECTRL_CKEFIXON));   
#endif
#endif
        vIO32WriteFldMulti(DDRPHY_MISC_CG_CTRL0, P_Fld(0, MISC_CG_CTRL0_W_CHG_MEM) 
                    | P_Fld(0, MISC_CG_CTRL0_CLK_MEM_SEL));//[5:4] mem_ck mux: 2'b00: 26MHz, [0]: change memory clock
        vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_W_CHG_MEM);//change clock freq
        mcDELAY_US(1);
        vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_W_CHG_MEM);//disable memory clock change
    }

    //RG_*PHDET_EN=0 (DLL)
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI2, 0x0, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI2, 0x0, B0_DLL_ARPI2_RG_ARDLL_PHDET_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI2, 0x0, B1_DLL_ARPI2_RG_ARDLL_PHDET_EN_B1);
  
    //*PI_CG=1, RG_*MPDIV_CG=1
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

    //RG_*BIAS_EN=0
    vIO32WriteFldAlign_All(DDRPHY_B0_DQ6, 0x0, B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DQ6, 0x0, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1);
    vIO32WriteFldAlign_All(DDRPHY_CA_CMD6, 0x0, CA_CMD6_RG_RX_ARCMD_BIAS_EN);

    //RG_*VREF_EN=0
    vIO32WriteFldAlign_All(DDRPHY_B0_DQ5, 0x0, B0_DQ5_RG_RX_ARDQ_VREF_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DQ5, 0x0, B1_DQ5_RG_RX_ARDQ_VREF_EN_B1);
    vIO32WriteFldAlign_All(DDRPHY_CA_CMD5, 0x0, CA_CMD5_RG_RX_ARCMD_VREF_EN);

    ///TODO: only CHA?
    //RG_*MCK8X_EN=0	
    vIO32WriteFldMulti(DDRPHY_PLL4, P_Fld(0, PLL4_RG_RPHYPLL_ADA_MCK8X_EN) 
                    | P_Fld(0, PLL4_RG_RPHYPLL_AD_MCK8X_EN));//RG_*MCK8X_EN=0; Since there is only 1 PLL, only to control CHA
    //RG_*MIDPI_EN=0
    vIO32WriteFldAlign_All(DDRPHY_SHU1_B0_DQ6, 0x0, SHU1_B0_DQ6_RG_ARPI_MIDPI_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_SHU1_B1_DQ6, 0x0, SHU1_B1_DQ6_RG_ARPI_MIDPI_EN_B1);
    vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_CMD6, 0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA);

    vIO32WriteFldAlign(DDRPHY_PLL1, 0x0, PLL1_RG_RPHYPLL_EN); //RG_*PLL_EN=0; Since there is only 1 PLL, only control CHA
    vIO32WriteFldAlign(DDRPHY_PLL2, 0x0, PLL2_RG_RCLRPLL_EN); //RG_*PLL_EN=0; Since there is only 1 PLL, only control CHA

    //!set sc_mpll to SPM register //Chen-Hsiang modify @20170316
    //u4value = Reserve_Reg_Readl(RESERVE_PDEF_SPM_PLL_CON);   
    //Reserve_Sync_Writel(RESERVE_PDEF_SPM_PLL_CON, u4value | (0x1 << 8) | (0x1 << 4));//set sc_mpll_off=1 , sc_mpll_s_off=1
    //TBD
    //*((UINT32P)(0x1000631c )) |= (0x1 << 1); //ddrphy_pwr_iso=1 //Lewis@20160621: Fix LP3 Hang from S0 suspend into reserve mode
    //*((UINT32P)(0x100063b8 )) |= (0x1 << 1);

    //RG_*RESETB=0
    vIO32WriteFldAlign_All(DDRPHY_B0_DQ3, 0x0, B0_DQ3_RG_ARDQ_RESETB_B0);
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI0, 0x0, B0_DLL_ARPI0_RG_ARPI_RESETB_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DQ3, 0x0, B1_DQ3_RG_ARDQ_RESETB_B1);
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI0, 0x0, B1_DLL_ARPI0_RG_ARPI_RESETB_B1);
    vIO32WriteFldAlign_All(DDRPHY_CA_CMD3, 0x0, CA_CMD3_RG_ARCMD_RESETB);
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI0, 0x0, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);
    vIO32WriteFldAlign(DDRPHY_PLL4, 0x0, PLL4_RG_RPHYPLL_RESETB);//Since there is only 1 PLL, only control CHA
    //Lewis@20160628: Fix LP3 enter S0 then into reserve mode fail due to CHB PHY not reset(LP3 PHY use 2 channels)

    MPLLInit();

    //marked by kaihsin on May 7th, not necessary to PDN DDRPHY
    //*((UINT32P)(0x1000631c )) &= ~(0x1 << 2); //ddrphy_pwr_on=0
    //*((UINT32P)(0x100063b8 )) &= ~(0x1 << 2);

    // wait 1us
    mcDELAY_US(1);

    //marked by kaihsin on May 7th, not necessary to PDN DDRPHY
    //*((UINT32P)(0x1000631c )) |= (0x1 << 2); //ddrphy_pwr_on=1
    //*((UINT32P)(0x100063b8 )) |= (0x1 << 2);
    // wait 1us
    mcDELAY_US(1);

    //RG_*RESETB=1
    vIO32WriteFldAlign_All(DDRPHY_B0_DQ3, 0x1, B0_DQ3_RG_ARDQ_RESETB_B0);
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI0, 0x1, B0_DLL_ARPI0_RG_ARPI_RESETB_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DQ3, 0x1, B1_DQ3_RG_ARDQ_RESETB_B1);
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI0, 0x1, B1_DLL_ARPI0_RG_ARPI_RESETB_B1);
    vIO32WriteFldAlign_All(DDRPHY_CA_CMD3, 0x1, CA_CMD3_RG_ARCMD_RESETB);
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI0, 0x1, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);
    vIO32WriteFldAlign(DDRPHY_PLL4, 0x1, PLL4_RG_RPHYPLL_RESETB);//Since there is only 1 PLL, only control CHA

    //TBD
    //*((UINT32P)(0x1000631c )) &= ~(0x1 << 1); //ddrphy_pwr_iso=0 //Lewis@20160621: Fix LP3 Hang from S0 suspend into reserve mode
    //*((UINT32P)(0x100063b8 )) &= ~(0x1 << 1);

    u4value = Reserve_Reg_Readl(RESERVE_PDEF_SPM_PLL_CON);   
    Reserve_Sync_Writel(RESERVE_PDEF_SPM_PLL_CON, u4value & ~((0x1 << 8) | (0x1 << 4)));//set sc_mpll_off=0 , sc_mpll_s_off=0
    mcDELAY_US(20);

    //RG_*PLL_EN=1
    vIO32WriteFldAlign(DDRPHY_PLL1, 0x1, PLL1_RG_RPHYPLL_EN); //RG_*PLL_EN=1; Since there is only 1 PLL, only control CHA
    vIO32WriteFldAlign(DDRPHY_PLL2, 0x1, PLL2_RG_RCLRPLL_EN); //RG_*PLL_EN=1; Since there is only 1 PLL, only control CHA

     //Wait 20us for MEMPLL
     mcDELAY_US(20);
     
    //RG_*VREF_EN=1 (Vref is only used in read, DQ(B0, B1)is use for RX)
    if((u4DramType == 2) || (u4DramType == 3))
    {
        vIO32WriteFldAlign_All(DDRPHY_B0_DQ5, 0x1, B0_DQ5_RG_RX_ARDQ_VREF_EN_B0);
        vIO32WriteFldAlign_All(DDRPHY_B1_DQ5, 0x1, B1_DQ5_RG_RX_ARDQ_VREF_EN_B1);
    }
    else
    {
        vIO32WriteFldAlign_All(DDRPHY_B1_DQ5, 0x1, B1_DQ5_RG_RX_ARDQ_VREF_EN_B1);
        vIO32WriteFldAlign(DDRPHY_B0_DQ5  + ((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, B0_DQ5_RG_RX_ARDQ_VREF_EN_B0);
        vIO32WriteFldAlign(DDRPHY_CA_CMD5  + ((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, CA_CMD5_RG_RX_ARCMD_VREF_EN);
    }
#if 0
    //!Olympus : LP3 preloader  don't set this bit, *((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x02b << 2) )) |= (0x1 << 16);   //RG_*VREF_EN=1
    *((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x04b << 2) )) |= (0x1 << 16);   //RG_*VREF_EN=1
    //! olympus LP3,LP4 preloader don't assert this bit, *((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x06b << 2) )) |= (0x1 << 16);   //RG_*VREF_EN=1
    if((u4DramType == 2) || (u4DramType == 3)) 
    {
        //! Olympus : LP4 preloader  set this bit,
        *((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x02b << 2) )) |= (0x1 << 16);   //RG_*VREF_EN=1
        *((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x02b << 2) )) |= (0x1 << 16);   //RG_*VREF_EN=1
        *((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x04b << 2) )) |= (0x1 << 16);   //RG_*VREF_EN=1
        //! olympus LP3,LP4 preloader don't assert this bit, *((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x06b << 2) )) |= (0x1 << 16);   //RG_*VREF_EN=1
    }
#endif

    //RG_*MCK8X_EN=1	
    vIO32WriteFldAlign(DDRPHY_PLL4, 1, PLL4_RG_RPHYPLL_ADA_MCK8X_EN);//RG_*MCK8X_EN=1; Since there is only 1 PLL, only to control CHA

    // wait 1us
    mcDELAY_US(1);

    if((u4DramType == 2) || (u4DramType == 3))
    {
        vIO32WriteFldAlign_All(DDRPHY_SHU1_B0_DQ6, 0x1, SHU1_B0_DQ6_RG_ARPI_MIDPI_EN_B0);
        vIO32WriteFldAlign_All(DDRPHY_SHU1_B1_DQ6, 0x1, SHU1_B1_DQ6_RG_ARPI_MIDPI_EN_B1);
        vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_CMD6, 0x1, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA);

        //*PI_CG=0, RG_*MPDIV_CG=0
        vIO32WriteFldMulti_All(DDRPHY_B0_DLL_ARPI2, P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCK_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCTL_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_FB_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQS_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQM_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQ_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0));
        vIO32WriteFldMulti_All(DDRPHY_B1_DLL_ARPI2, P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_MCK_B1)
                    | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B1)
                    | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_MCTL_B1)
                    | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_FB_B1)
                    | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQS_B1)
                    | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQM_B1)
                    | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQ_B1)
                    | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B1)
                    | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1));
        vIO32WriteFldMulti_All(DDRPHY_CA_DLL_ARPI2, P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCK_CA)//not include 11th bit(CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_CA)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCTL_CA)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_FB_CA)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CS)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CLK)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CMD)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA));
    }
    else
    {
        //*PI_CG=0, RG_*MPDIV_CG=0
        vIO32WriteFldMulti(DDRPHY_B0_DLL_ARPI2, P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCK_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCTL_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_FB_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQS_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQM_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQ_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0));
        vIO32WriteFldMulti(DDRPHY_B0_DLL_ARPI2 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCK_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCTL_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_FB_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQS_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQM_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQ_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0));
        vIO32WriteFldMulti_All(DDRPHY_B1_DLL_ARPI2, P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_MCK_B1)
                    | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B1)
                    | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_MCTL_B1)
                    | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_FB_B1)
                    | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQS_B1)
                    | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQM_B1)
                    | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQ_B1)
                    | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B1)
                    | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1));

        vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI2, P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCK_CA)//not include 11th bit(CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_CA)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCTL_CA)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_FB_CA)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CS)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CLK)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CMD)
                   // | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN)//11th
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA));
        vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI2 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCK_CA)//not include 11th bit(CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_CA)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCTL_CA)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_FB_CA)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CS)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CLK)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CMD)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN)//11th
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA));
    }

    //RG_*BIAS_EN=1
    //vIO32WriteFldAlign_All(DDRPHY_B0_DQ6, 0x0, B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0);//Chen-Hsiang modify @20170316
    //vIO32WriteFldAlign_All(DDRPHY_B1_DQ6, 0x0, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1);//Chen-Hsiang modify @20170316
    
    // wait 1us
    mcDELAY_US(1);

    if((u4DramType == 2) || (u4DramType == 3))
    {
        vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL0, P_Fld(0, MISC_CG_CTRL0_W_CHG_MEM) 
                    | P_Fld(1, MISC_CG_CTRL0_CLK_MEM_SEL));//[5:4] mem_ck mux: 2'b01: memory clock, [0]: change memory clock
        vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_W_CHG_MEM);//change clock freq
        mcDELAY_US(1);
        vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_W_CHG_MEM);//disable memory clock change
    }
    else
    {
        vIO32WriteFldMulti(DDRPHY_MISC_CG_CTRL0, P_Fld(0, MISC_CG_CTRL0_W_CHG_MEM) 
                    | P_Fld(1, MISC_CG_CTRL0_CLK_MEM_SEL));//[5:4] mem_ck mux: 2'b01: memory clock, [0]: change memory clock
        vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_W_CHG_MEM);//change clock freq
        mcDELAY_US(1);
        vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_W_CHG_MEM);//disable memory clock change        
    }
    
    //force top feedback MCK not divide
    //rg_ddrphy_fb_ck_force_en = 1 -- TBD
    //*CLK_MEM_DFS_CFG |= (0x1 << 8); //rg_ddrphy_fb_ck_force_en = 1
    u4value = Reserve_Reg_Readl(0x10060464);
    Reserve_Sync_Writel(0x10060464, u4value | (0x3 << 20)); //set sc_ddrphy_fb_ck_ch*_en = 1
    u4value = Reserve_Reg_Readl(0x10060004);
    Reserve_Sync_Writel(0x10060004, u4value | (0x1 << 16)); //set sc_ddrphy_fb_ck_ch*_en = 1

    //*DRAMC_WBR = 0x3;
    //1st DLL enable
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI2, 0x1, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);//Since CHA CA is DLL master
    // wait 1us
    mcDELAY_US(1);
    //2nd DLL enable
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI2+ ((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI2, 0x1, B0_DLL_ARPI2_RG_ARDLL_PHDET_EN_B0);//Chen-Hsiang modify @20170316
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI2, 0x1, B1_DLL_ARPI2_RG_ARDLL_PHDET_EN_B1);//Chen-Hsiang modify @20170316
    // wait 1us
    mcDELAY_US(1);
    
    //top feedback MCK to divided frequency -- TBD
    //*CLK_MEM_DFS_CFG &= ~(0x1 << 8); //rg_ddrphy_fb_ck_force_en = 0
    u4value = Reserve_Reg_Readl(0x10060464);
    Reserve_Sync_Writel(0x10060464, u4value & ~(0x3 << 20)); //set sc_ddrphy_fb_ck_ch*_en = 0
    u4value = Reserve_Reg_Readl(0x10060004);
    Reserve_Sync_Writel(0x10060004, u4value & ~(0x1 << 16)); //set sc_ddrphy_fb_ck_ch*_en = 0

    vIO32WriteFldAlign_All(DRAMC_REG_DDRCONF0, 1, DDRCONF0_RDATRST);//R_DMRDATRST = 1
    vIO32WriteFldAlign_All(DRAMC_REG_DDRCONF0, 0, DDRCONF0_RDATRST);//R_DMRDATRST = 0

    //! set SPM to control PLL enable and disable PLL enable from ddrphy conf
    /*TINFO ="Switching PHYPLL enable path from DDRPHY to SPM control by setting SPM SW" */
    vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL_MODE_SW_PCM);
    u4value = Reserve_Reg_Readl(DRAMC_DPY_CLK_SW_CON2);
    Reserve_Sync_Writel(DRAMC_DPY_CLK_SW_CON2, u4value | (0x1 << 2));
    mcDELAY_US(1);
    //RG_*PLL_EN=1
    vIO32WriteFldAlign(DDRPHY_PLL1, 0x0, PLL1_RG_RPHYPLL_EN); //disable DDRPHY PHYPLL enable, RG_*PLL_EN=0; Since there is only 1 PLL, only control CHA
    vIO32WriteFldAlign(DDRPHY_PLL2, 0x0, PLL2_RG_RCLRPLL_EN); //disable DDRPHY CLRPLL enable, RG_*PLL_EN=0; Since there is only 1 PLL, only control CHA
    /*TINFO ="Setting RX input delay tracking enable from SPM side(un-paused)" */
    //! speed > 3200 need enable RX input delay tracking 
    if((u4DramType == 2) || (u4DramType == 3))
    {
        u4value = Reserve_Reg_Readl(SPM_SW_RSV_8);
        Reserve_Sync_Writel(SPM_SW_RSV_8, u4value | (0x3 << 6));
    }
    //! diable dummy read when exit sref
    vIO32WriteFldMulti_All(DRAMC_REG_DUMMY_RD, P_Fld(0x0, DUMMY_RD_DQSG_DMYWR_EN)
            | P_Fld(0x0, DUMMY_RD_DQSG_DMYRD_EN)
            | P_Fld(0x0, DUMMY_RD_SREF_DMYRD_EN)
            | P_Fld(0x0, DUMMY_RD_DUMMY_RD_EN)
            | P_Fld(0x0, DUMMY_RD_DMY_RD_DBG)
            | P_Fld(0x0, DUMMY_RD_DMY_WR_DBG));

    DramcBroadcastOnOff(backup_broadcast);
    //DDRPhyFreqMeter();
    //dump_gating_error_rg();
    //dump_SR();
}

//#define USE_TA2_IN_DDR_RESERVE_MODE //Just for debug
#ifdef USE_TA2_IN_DDR_RESERVE_MODE
#define TA2_STRESS_TO 10000
static void TA2_test_run_time_HW(DRAMC_CTX_T * p)//notice: Rank number and channel number should handle
{
    DRAM_CHANNEL_T channel_bak = p->channel;
    DRAM_RANK_T rank_bak = p->rank;
    U8 u1ChannelIdx = 0;
    U8 u1RankIdx = 0, u1RankMax = 0;
    U32 u4loop_count = 0;
    U32 u4ErrorValue = 0;
    U32 u4ErrRegField = 0;
    static U32 err_count = 0;
    static U32 pass_count = 0;

    if (p->support_rank_num==RANK_DUAL)
        u1RankMax = RANK_MAX;
    else
        u1RankMax =RANK_1;

    vIO32WriteFldAlign_All(DRAMC_REG_TEST2_3, u1RankMax - 1, TEST2_3_TESTCNT);//1: HW switch R0 and R1 automatically 0: only R0 
    vIO32WriteFldAlign_All(DRAMC_REG_TEST2_4, 4, TEST2_4_TESTAGENTRKSEL);//Controlled By HW
    //vIO32WriteFldAlign_All(DRAMC_REG_TEST2_1, 0x10000, TEST2_1_TEST2_BASE); //Not change, suppose this area is preserved for memeory test
    //vIO32WriteFldAlign_All(DRAMC_REG_TEST2_5, 0x10000, TEST2_5_TEST2_BASE_2); //Not change, suppose this area is preserved for memeory test
    //vIO32WriteFldAlign_All(DRAMC_REG_TEST2_2, 0x400, TEST2_2_TEST2_OFF); //Not change, suppose this area is preserved for memeory test
    
    for(u1ChannelIdx = CHANNEL_A; u1ChannelIdx<p->support_channel_num; u1ChannelIdx++)
    {  
        p->channel = u1ChannelIdx;
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), 1, TEST2_3_TEST2W);
        mcDELAY_US(1);

        while(((u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TESTRPT), TESTRPT_DM_CMP_CPT_RK0))==0) ||
            ((u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TESTRPT), TESTRPT_DM_CMP_CPT_RK1))==0))
        {
            mcDELAY_US(1);
            u4loop_count++;
            if(u4loop_count > TA2_STRESS_TO)
            {
                mcSHOW_DBG_MSG(("fcWAVEFORM_MEASURE_A %d :time out, [22:20]=0x%x\n", u4loop_count, u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TESTRPT), TESTRPT_TESTSTAT)));
                break;
            }
        }
        
        for(u1RankIdx = 0; u1RankIdx<u1RankMax; u1RankIdx++)
        {
            u4ErrRegField = (u1RankIdx) ? (TESTRPT_DM_CMP_ERR_RK1):(TESTRPT_DM_CMP_ERR_RK0);
            u4ErrorValue = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TESTRPT), u4ErrRegField);
            if(u4ErrorValue > 0)
            {
                err_count++;
                mcSHOW_DBG_MSG(("Channel(%d) Rank(%d),HW TA2 failed, pass_cnt:%d,err_cnt:%d, u4ErrorValue 0x%x\n", u1ChannelIdx, u1RankIdx, pass_count,err_count, u4ErrorValue));
            }
            else
            {
                pass_count++;
                mcSHOW_DBG_MSG(("Channel(%d) Rank(%d),HW TA2 pass, pass_cnt:%d,err_cnt:%d\n", u1ChannelIdx, u1RankIdx, pass_count,err_count));
            }
        }
#if 0
        mcSHOW_DBG_MSG(("CH[%d] CMP_ERR_RK0=%d CMP_ERR_RK1=%d CMP_OK_RK0=%d CMP_OK_RK1=%d\n", 
            p->channel,
            u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TESTRPT), TESTRPT_DM_CMP_ERR_RK0),
            u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TESTRPT), TESTRPT_DM_CMP_ERR_RK1),
            u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TESTRPT), TESTRPT_DLE_CNT_OK_RK0),
            u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TESTRPT), TESTRPT_DLE_CNT_OK_RK1)));
#endif
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), 0, TEST2_3_TEST2W);
    }
    p->channel = channel_bak;
    p->rank = rank_bak;
    return;
}
#endif

void Dramc_DDR_Reserved_Mode_AfterSR(void)
{
    U32 u4WbrBackup = 0;
    U32 u4DramType = 0;
    U32 u4LP3_MR2RLWL = 0;
    U32 u4LP4_MR13FSP = 0;
    U32 u4LP4_MR2RLWL = 0;
    DRAMC_CTX_T * p;
    U32 backup_broadcast = GetDramcBroadcast();
    U8 u1channel = CHANNEL_A;

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);	
    u4DramType = u4IO32ReadFldAlign(DRAMC_REG_ARBCTL, ARBCTL_RSV_DRAM_TYPE);
    mcSHOW_DBG_MSG(("Dramc_DDR_Reserved_Mode_AfterSR dram type =%x \n", u4DramType));

    p = psCurrDramCtx; //done in ddr reserve mode setting
    Func_Emi_Off(p);
    Func_Emi_On(p);
    if(u4DramType == 1)
    {   //LPDDR3
        vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0 + ((U32)CHANNEL_B<<POS_BANK_NUM), 1, MISC_CG_CTRL0_RG_CG_DRAMC_CHB_CK_OFF);//Close CHB controller clk

        //Since we do MR2 with (RG of Mr13) first while DVFS, we use (RG of MR13) to apply OP of MR2
        u4LP3_MR2RLWL = u4IO32ReadFldAlign(DRAMC_REG_SHU_HWSET_MR13, SHU_HWSET_MR13_HWSET_MR13_OP);
        p->channel = CHANNEL_A;
        DramcModeRegWriteByRank(p, 0, 2, u4LP3_MR2RLWL);
        DramcModeRegWriteByRank(p, 0, 17, 0);//R1 lost data WA, set R0 R1 both for safe{On charging mode, system may issue MR17=0xFF in R1}
        if(u4IO32ReadFldAlign(DRAMC_REG_RSTMASK, RSTMASK_RSV_DRAM_SUPPORT_RANK_NUM) == 0)//DRAM is dual rank
        {
            DramcModeRegWriteByRank(p, 1, 2, u4LP3_MR2RLWL);
            DramcModeRegWriteByRank(p, 1, 17, 0);//R1 lost data WA, set R0 R1 both for safe{On charging mode, system may issue MR17=0xFF in R1}
        }
    } 
    else if((u4DramType == 2) || (u4DramType == 3))
    {   //LPDDR4
        u4LP4_MR13FSP = u4IO32ReadFldAlign(DRAMC_REG_SHU_HWSET_MR13, SHU_HWSET_MR13_HWSET_MR13_OP);
        u4LP4_MR2RLWL = u4IO32ReadFldAlign(DRAMC_REG_SHU_HWSET_MR2, SHU_HWSET_MR2_HWSET_MR2_OP);
        for(u1channel = CHANNEL_A; u1channel < p->support_channel_num; u1channel++)
        {
            p->channel = u1channel;
            DramcModeRegWriteByRank(p, 0, 13, u4LP4_MR13FSP);
            DramcModeRegWriteByRank(p, 0, 2, u4LP4_MR2RLWL);
            DramcModeRegWriteByRank(p, 0, 17, 0);//R1 lost data WA, set R0 R1 both for safe{On charging mode, system may issue MR17=0xFF in R1}
            if(u4IO32ReadFldAlign(DRAMC_REG_RSTMASK, RSTMASK_RSV_DRAM_SUPPORT_RANK_NUM) == 0)//DRAM is dual rank
            {
                DramcModeRegWriteByRank(p, 1, 13, u4LP4_MR13FSP);
                DramcModeRegWriteByRank(p, 1, 2, u4LP4_MR2RLWL);
                DramcModeRegWriteByRank(p, 1, 17, 0);//R1 lost data WA, set R0 R1 both for safe{On charging mode, system may issue MR17=0xFF in R1}
            }
        }
    }
#ifdef USE_TA2_IN_DDR_RESERVE_MODE
    {
        int i = 0;
        for(i=0; i<10; i++)
        {
            TA2_test_run_time_HW(p);
        }
    }
#endif
    vIO32Write4B(DRAMC_REG_SPCMDCTRL, u4Cha_mr4_backup);
    vIO32Write4B(DRAMC_REG_SPCMDCTRL + ((U32)CHANNEL_B<<POS_BANK_NUM), u4Chb_mr4_backup);
    vIO32Write4B(DRAMC_REG_SHU_SCINTV, u4Cha_dqsosc_en_backup);
    vIO32Write4B(DRAMC_REG_SHU_SCINTV + ((U32)CHANNEL_B<<POS_BANK_NUM), u4Chb_dqsosc_en_backup);
    vIO32Write4B(DRAMC_REG_DQSOSCR, u4Cha_dqsosc_rd_backup);
    vIO32Write4B(DRAMC_REG_DQSOSCR + ((U32)CHANNEL_B<<POS_BANK_NUM), u4Chb_dqsosc_rd_backup);
    vIO32Write4B(DRAMC_REG_DUMMY_RD, u4Cha_dummy_read_backup);
    vIO32Write4B(DRAMC_REG_DUMMY_RD + ((U32)CHANNEL_B<<POS_BANK_NUM), u4Chb_dummy_read_backup);
    vIO32Write4B(DRAMC_REG_DRAMC_PD_CTRL, u4Cha_phy_clk_dyn_backup);
    vIO32Write4B(DRAMC_REG_DRAMC_PD_CTRL + ((U32)CHANNEL_B<<POS_BANK_NUM), u4Chb_phy_clk_dyn_backup);

#if LP3_CKE_FIX_ON_TO_LEAVE_SR_WITH_LVDRAM
#if ENABLE_LP3_SW
    if(u4DramType == 1)
    {
        vIO32Write4B(DRAMC_REG_CKECTRL, u4Cha_cke_backup);
        vIO32Write4B(DRAMC_REG_CKECTRL + ((U32)CHANNEL_B<<POS_BANK_NUM), u4Chb_cke_backup);
    }
#endif
#endif
    
#ifdef HW_GATING
    DramcHWGatingOnOff(p,1);//Enable HW Gating tracking for gating tracking fifo mode
#endif
    DramcBroadcastOnOff(backup_broadcast);
    dump_gating_error_rg(u4DramType);
    //DramcRegDump(p);
    return;
}
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
void vSetVcoreByFreq(DRAMC_CTX_T *p)
{
#ifndef MT6763_FPGA
#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
#if defined(DRAM_HQA) && defined(__ETT__)
	hqa_set_voltage_by_freq(p);
#elif __FLASH_TOOL_DA__
    mtk_regulator_set_voltage(&reg_vcore, 700000, MAX_VCORE);
#else
#if USE_PMIC_CHIP_MT6356
	unsigned int voltage;

    if(p->dram_type == TYPE_LPDDR4P)
//        mtk_regulator_set_voltage(&reg_vddq, 400000, MAX_VDDQ);
        rt5738_set_voltage(RT5738_VDDQ, SEL_PREFIX_VDDQ); //***LP4P is use for Hynix old die, Please do not adjust the voltage. Thanks.***
    else if(p->dram_type == TYPE_LPDDR4X)
//        mtk_regulator_set_voltage(&reg_vddq, VDDQ_BY_FREQ, MAX_VDDQ);
        rt5738_set_voltage(RT5738_VDDQ, SEL_PREFIX_VDDQ);
    
    if(u1IsLP4Family(p->dram_type))    
    {
    // for 3733, 3200
    if(p->frequency >= 1600)
			#ifdef VCORE_BIN
				voltage = (get_vcore_ptp_volt(p->dram_type, 0) + get_vcore_ptp_volt(p->dram_type, 1)) >> 1;
			#else
				#if defined(SLT)
					if (seclib_get_devinfo_with_index(25) & 0x100)
						#if defined(NVCORE_NVDRAM) 
						voltage = 760000; //0.76V
						#else
						voltage = (SEL_PREFIX_VCORE(LP4, KOPP0) + SEL_PREFIX_VCORE(LP4, KOPP0)) >> 1;
						#endif
					else
				#endif
						#if defined(SLT) && defined(NVCORE_NVDRAM) 
						voltage = 660000; //0.66V
						#else
						voltage = (SEL_PREFIX_VCORE(LP4, KOPP0) + SEL_PREFIX_VCORE(LP4, KOPP1)) >> 1;
						#endif
			#endif
		else if (p->frequency == 1200)
			#ifdef VCORE_BIN
				voltage = (get_vcore_ptp_volt(p->dram_type, 1) + get_vcore_ptp_volt(p->dram_type, 2)) >> 1;
			#else
				#if defined(SLT)
					if (seclib_get_devinfo_with_index(25) & 0x100)
						voltage = (SEL_PREFIX_VCORE(LP4, KOPP0) + SEL_PREFIX_VCORE(LP4, KOPP2)) >> 1;
					else
				#endif
						voltage = (SEL_PREFIX_VCORE(LP4, KOPP1) + SEL_PREFIX_VCORE(LP4, KOPP2)) >> 1;
			#endif
		else if (p->frequency == 800)
			#ifdef VCORE_BIN
				voltage = (get_vcore_ptp_volt(p->dram_type, 2) + get_vcore_ptp_volt(p->dram_type, 3)) >> 1;
			#else
				voltage = (SEL_PREFIX_VCORE(LP4, KOPP2) + SEL_PREFIX_VCORE(LP4, KOPP3)) >> 1;
			#endif
		else
			return ;
    }
    else
    {
        // for 1866
        if(p->frequency >= 933)
            voltage = SEL_PREFIX_VCORE(LP3, KOPP0);
        else if(p->frequency == 800) // for 1600
            voltage = (SEL_PREFIX_VCORE(LP3, KOPP0) + SEL_PREFIX_VCORE(LP3, KOPP2)) >> 1;
        else if(p->frequency == 600) // for 1200
            voltage = (SEL_PREFIX_VCORE(LP3, KOPP2) + SEL_PREFIX_VCORE(LP3, KOPP3)) >> 1;
		else
			return ;
    }

	mtk_regulator_set_voltage(&reg_vcore, voltage, MAX_VCORE);

	printf("Setup voltage for %d\n", p->frequency);
	printf("Vcore = %d\n", mtk_regulator_get_voltage(&reg_vcore));

	if (u1IsLP4Family(p->dram_type)) {
		/* LPDDR4 */
		printf("Vdram = %d\n", rt5738_get_voltage(RT5738_VDD2));
		printf("Vddq = %d\n", rt5738_get_voltage(RT5738_VDDQ));
	} else {
		/* LPDDR3 */
		printf("Vdram = %d\n", dramc_get_lp3_vdram());
	}
#endif
#endif
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

    if (u1IsLP4Family(p->dram_type))
    {
        u4RankSize = 0x40000000 * p->support_channel_num;
    }
#if ENABLE_LP3_SW
    else
    {
        u4RankSize = 0x80000000 * p->support_channel_num;
    }
#endif /* ENABLE_LP3_SW */

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
            mcSHOW_DBG_MSG(("[MEM_TEST] Rank %d Fail.", u1RankIdx));
            u1DumpInfo =1;
        }
        else
        {
            mcSHOW_DBG_MSG(("[MEM_TEST] Rank %d OK.", u1RankIdx));
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
	int fClr_calfail;
	#if EMMC_READY
	// scy: only need to read emmc one time for each boot-up
	if (g_dram_save_time_init_done == 1)
		return DRAM_OK;
	else
		g_dram_save_time_init_done = 1;
   	if(!u1IsLP4Family(p->dram_type) || read_offline_dram_calibration_data(p->pSavetimeData)<0)
    	{
 		p->pSavetimeData->femmc_Ready=0;

            memset(p->pSavetimeData, 0, sizeof(SAVE_TIME_FOR_CALIBRATION_T));
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

		p->pSavetimeData->Bypass_RDDQC[_MappingFreqArray[5]]=1;
		p->pSavetimeData->Bypass_RDDQC[_MappingFreqArray[2]]=1;
		p->pSavetimeData->Bypass_RDDQC[_MappingFreqArray[1]]=0;
		p->pSavetimeData->Bypass_RXWINDOW[_MappingFreqArray[5]]=1;
		p->pSavetimeData->Bypass_RXWINDOW[_MappingFreqArray[2]]=1;
		p->pSavetimeData->Bypass_RXWINDOW[_MappingFreqArray[1]]=0;
		p->pSavetimeData->Bypass_TXWINDOW[_MappingFreqArray[5]]=1;
		p->pSavetimeData->Bypass_TXWINDOW[_MappingFreqArray[2]]=1;
		p->pSavetimeData->Bypass_TXWINDOW[_MappingFreqArray[1]]=0;

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
extern U16 gFinalTXVrefDQ[CHANNEL_NUM][RANK_MAX];
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
#ifdef DDR_INIT_TIME_PROFILING
    CPU_Cycle=TimeProfileEnd();
    CPUCycleSum += CPU_Cycle;
    mcSHOW_ERR_MSG(("Rank %d CBT takes %d ms\n\r", s1RankIdx, CPU_Cycle));
    TimeProfileBegin();
#endif
#endif //ENABLE_CA_TRAINING

    for(s1RankIdx=RANK_0; s1RankIdx<u1RankMax; s1RankIdx++)   
    {
        vSetRank(p, s1RankIdx);
  
//#if ENABLE_LP4_ZQ_CAL
        //DramcZQCalibration(p); //ZQ calibration should be done before CBT and operated at low frequency, so it is moved to mode register init
//#endif

#if ENABLE_CLOCK_DUTY_SCAN
        // only K 3200
#if CLK_DUTY_SCAN_APPLY_RK0_TO_RK1
        if((p->channel == CHANNEL_A)&&(p->frequency == u2DFSGetHighestFreq(p))&&p->rank==RANK_0)
#else
        if((p->channel == CHANNEL_A)&&(p->frequency == u2DFSGetHighestFreq(p)))
#endif            
        {
            DramcGatingMode(p, 0);
            DramcClockDutyScan_TestEngine(p);
            DramcGatingMode(p, 1);
            // DramcModeRegInit_LP4(p);
        }
#endif            
        
        #if ENABLE_WRITE_LEVELING            
        DramcWriteLeveling((DRAMC_CTX_T *) p);//Dram will be reset when finish write leveling         
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(((p->pSavetimeData->fcalfail[p->freq_sel] & 0x01)>>0)==1)
        {
         DramcWriteLeveling((DRAMC_CTX_T *) p);//Dram will be reset when finish write leveling         
        }
#endif
        
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

            vAutoRefreshSwitch(p, ENABLE); //when doing gating, RX and TX calibration, auto refresh should be enable
        
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

        #ifdef DDR_INIT_TIME_PROFILING
        CPU_Cycle=TimeProfileEnd();   
        CPUCycleSum += CPU_Cycle;
        mcSHOW_ERR_MSG(("Rank %d RX RDDQC takes %d ms\n\r", s1RankIdx, CPU_Cycle));
        TimeProfileBegin();
        #endif

        #if LJPLL_FREQ_DEBUG_LOG
            DDRPhyFreqMeter();
        #endif        

#if TX_K_DQM_WITH_WDBI
        U8 u1TXVrefDQ, u1TXVrefDQM, u1TXFinalVref=0xff, u1DQMSpecResult, u1TempVref;

        //DramcWriteDBIOnOff() control both rank, need to recover for rank1 tx calibration
        DramcWriteDBIOnOff(p, 0);
#endif
        DramcTxWindowPerbitCal((DRAMC_CTX_T *) p, TX_DQ_DQS_MOVE_DQ_DQM, FALSE);  //Vref scan disable
        u1TXVrefDQ = DramcTxWindowPerbitCal((DRAMC_CTX_T *) p, TX_DQ_DQS_MOVE_DQ_ONLY, p->enable_tx_scan_vref); 

#if TX_K_DQM_WITH_WDBI
        if (p->DBI_W_onoff[p->dram_fsp]==DBI_ON)
        {
            // K DQM with DBI_ON, and check DQM window spec.
            mcSHOW_DBG_MSG(("[TX_K_DQM_WITH_WDBI] Step1: K DQM with DBI_ON, and check DQM window spec.\n\n"));
            vSwitchWriteDBISettings(p, DBI_ON);
            u1DQMSpecResult = DramcTxWindowPerbitCal((DRAMC_CTX_T *) p, TX_DQ_DQS_MOVE_DQM_ONLY, FALSE);

            #if VREF_SPEED_UP_LP4
            #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
            if((u1DQMSpecResult==TX_DQM_WINDOW_SPEC_IN) || (p->pSavetimeData->femmc_Ready==1))
            #else
            if(u1DQMSpecResult==TX_DQM_WINDOW_SPEC_IN)
            #endif
            {
                vSwitchWriteDBISettings(p, DBI_OFF);
            }
            else // DQM window out of spec
            #endif
            {
                //Scan DQM + Vref
                mcSHOW_DBG_MSG(("[TX_K_DQM_WITH_WDBI] Step2: K DQM and Vref with DBI_ON\n\n"));
                u1TXVrefDQM = DramcTxWindowPerbitCal((DRAMC_CTX_T *) p, TX_DQ_DQS_MOVE_DQM_ONLY, TRUE);

                // Set Fincal TX Vref to MR14
                u1TXFinalVref = (u1TXVrefDQ+u1TXVrefDQM)>>1;
                #ifdef FOR_HQA_TEST_USED
                gFinalTXVrefDQ[p->channel][p->rank] = u1TXFinalVref;
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


 #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(((p->pSavetimeData->fcalfail[p->freq_sel] & 0x8)>>3)==1)
        {
            DramcTxWindowPerbitCal((DRAMC_CTX_T *) p, TX_DQ_DQS_MOVE_DQ_DQM, FALSE);  //Vref scan disable
            DramcTxWindowPerbitCal((DRAMC_CTX_T *) p, TX_DQ_DQS_MOVE_DQ_ONLY, p->enable_tx_scan_vref); 
        }
#endif
        #if SUPPORT_SAVE_OFF_LINE_CALIBRATION
        if(VrefStatus == DRAM_K_VREF)
        DramcTxWindowPerbitCal((DRAMC_CTX_T *) p, TX_DQ_DQS_MOVE_DQ_ONLY, p->enable_tx_scan_vref);
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
    
        DramcRxWindowPerbitCal((DRAMC_CTX_T *) p, 1);
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(((p->pSavetimeData->fcalfail[p->freq_sel] & 0x20)>>5)==1)
        {
            DramcRxWindowPerbitCal((DRAMC_CTX_T *) p, 1);
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

        vAutoRefreshSwitch(p, DISABLE); //After gating, Rx and Tx calibration, auto refresh should be disable 

        #if ENABLE_TX_TRACKING
        if(gu1MR23Done==FALSE)
        {
            DramcDQSOSCAuto(p);
        }
        DramcDQSOSCAuto(p);
        DramcDQSOSCMR23(p);
        DramcDQSOSCSetMR18MR19(p);
        #endif

        /* Enable/Disable calibrated rank's DBI function accordingly */
        #if ENABLE_READ_DBI
        //Read DBI ON
        SetDramModeRegForReadDBIOnOff(p, p->DBI_R_onoff[p->dram_fsp]);
        #endif
        
        #if ENABLE_WRITE_DBI
        //Write DBI ON
        DramcWriteMinus1MCKForWriteDBI(p, -8); //Tx DQ/DQM -1 MCK for write DBI ON
        SetDramModeRegForWriteDBIOnOff(p, p->DBI_W_onoff[p->dram_fsp]);
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
    
#if 0//ENABLE_CLOCK_DUTY_SCAN
    if (p->frequency == u2DFSGetHighestFreq(p))
    {
        Print_Clock_Duty_Value(p);
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
    mcSHOW_ERR_MSG(("DRAMC CA train takes %d ms\n\r", CPU_Cycle));
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
                    mcSHOW_ERR_MSG(("Rank %d Write leveling takes %d ms\n\r", s1RankIdx, CPU_Cycle));
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
    
        if (p->support_rank_num==RANK_DUAL)
        #ifndef LP3_DUAL_RANK_RX_K
            if(s1RankIdx==RANK_0)
        #endif
            {
                DramcRxWindowPerbitCal((DRAMC_CTX_T *) p, 1);
                
            #ifdef DDR_INIT_TIME_PROFILING
                CPU_Cycle=TimeProfileEnd();    
                mcSHOW_ERR_MSG(("Rnak %d RX takes %d ms\n\r", s1RankIdx, CPU_Cycle));      
                TimeProfileBegin();
            #endif
            }
    
        if (p->support_rank_num==RANK_DUAL)
        #ifndef LP3_DUAL_RANK_TX_K
            if(s1RankIdx==RANK_0)
        #endif
            {
                DramcTxWindowPerbitCal((DRAMC_CTX_T *) p, TX_DQ_DQS_MOVE_DQ_DQM, FALSE);   
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
    U8 channel_idx;
    
    for(channel_idx=CHANNEL_A; channel_idx<p->support_channel_num; channel_idx++)
    {        
        vSetPHY2ChannelMapping(p, channel_idx);// when switching channel, must update PHY to Channel Mapping
        vDramCalibrationSingleChannel(p);

        if (Get_MDL_Used_Flag()==GET_MDL_USED)
        {
            return;  // only K CHA to save time
        }
    }

    vSetPHY2ChannelMapping(p, CHANNEL_A);

#ifdef FOR_HQA_TEST_USED
    print_HQA_measure_message(p);
#endif 

#ifdef DRAM_CALIB_LOG  
    i4WriteSramLog(gDRAM_CALIB_LOG_pointer, (unsigned int*) &gDRAM_CALIB_LOG, sizeof(gDRAM_CALIB_LOG)/4);
    gDRAM_CALIB_LOG_pointer += sizeof(gDRAM_CALIB_LOG)/4;
#endif

     #if ENABLE_READ_DBI
     DramcReadDBIOnOff(p, p->DBI_R_onoff[p->dram_fsp]);
     #endif
     
     #if ENABLE_WRITE_DBI     
     DramcWriteDBIOnOff(p, p->DBI_W_onoff[p->dram_fsp]);

     // Improve Write DBI Power
     ApplyWriteDBIPowerImprove(p, ENABLE);
        
     #if ENABLE_WRITE_DBI_Protect
     ApplyWriteDBIProtect(p, ENABLE);
     #endif
     #endif
}

#if ENABLE_MRR_AFTER_FIRST_K
int GetDramInforAfterCalByMRR(DRAMC_CTX_T *p, DRAM_INFO_BY_MRR_T *DramInfo)
{
    U8 u1ChannelIdx, u1RankIdx, u1RankMax, u1DieNumber=0;
    U16 u2Density;
    U64 u8Size = 0;
    U64 u8ChannelSize;
    U32 u4ChannelNumber=1, u4RankNumber=1;
    
    vSetPHY2ChannelMapping(p, CHANNEL_A);

    // Read MR5 for Vendor ID
    DramcModeRegReadByRank(p, RANK_0, 5, &(DramInfo->u2MR5VendorID));
    DramInfo->u2MR5VendorID &= 0xff; // for byte mode, don't show value of another die.
    p->vendor_id = DramInfo->u2MR5VendorID;
    mcSHOW_DBG_MSG(("[GetDramInforAfterCalByMRR] Vendor %x.\n", DramInfo->u2MR5VendorID));

    // Read MR6 for Revision ID
    DramcModeRegReadByRank(p, RANK_0, 6, &(DramInfo->u2MR6RevisionID));
    DramInfo->u2MR6RevisionID &= 0xff; // for byte mode, don't show value of another die.
    p->revision_id = DramInfo->u2MR6RevisionID;
    mcSHOW_DBG_MSG(("[GetDramInforAfterCalByMRR] Revision %x.\n", DramInfo->u2MR6RevisionID)); 

     for(u1ChannelIdx=0; u1ChannelIdx<(p->support_channel_num); u1ChannelIdx++)
         for(u1RankIdx =0; u1RankIdx<RANK_MAX; u1RankIdx++)
            DramInfo->u8MR8Density[u1ChannelIdx][u1RankIdx] =0;

     // Read MR8 for dram density
    for(u1ChannelIdx=0; u1ChannelIdx<(p->support_channel_num); u1ChannelIdx++)
        for(u1RankIdx =0; u1RankIdx<(p->support_rank_num); u1RankIdx++)
        {
            if((p->aru4CalExecuteFlag[u1ChannelIdx][u1RankIdx][p->shu_type] !=0)  && \
                (p->aru4CalResultFlag[u1ChannelIdx][u1RankIdx][p->shu_type]==0))
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
                p->density = u2Density;

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

                u8Size *= u1DieNumber;
                u8ChannelSize = u8Size;
                u4ChannelNumber = p->support_channel_num;

                if (p->support_rank_num==RANK_DUAL)
                    u4RankNumber = 2;

                p->ranksize[u1RankIdx] = (u8ChannelSize/u4RankNumber)*u4ChannelNumber;
                DramInfo->u8MR8Density[u1ChannelIdx][u1RankIdx] = u8Size;
         }
        	
        mcSHOW_DBG_MSG(("CH%d, RK%d, DieNum %d, Density %llx, RKsize %llx.\n", u1ChannelIdx, u1RankIdx, u1DieNumber, DramInfo->u8MR8Density[u1ChannelIdx][u1RankIdx], p->ranksize[u1RankIdx]));
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
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RSTMASK), 0, RSTMASK_RSV_DRAM_SUPPORT_RANK_NUM);  //keep support_rank_num to reserved rg
    }
    else
    {
        p->support_rank_num = RANK_SINGLE;
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RSTMASK), 1, RSTMASK_RSV_DRAM_SUPPORT_RANK_NUM);  //keep support_rank_num to reserved rg
    }
    mcSHOW_DBG_MSG(("[RankNumberDetection] %d, %X\n", p->support_rank_num, p->aru4CalResultFlag[p->channel][p->rank][p->shu_type]));

    vSetRank(p, u1RankBak);  // restore rank setting
}
#endif

#define M17_SPM_DRAMC_DPY_CLK_SW_CON2  0x10006474
void Before_Init_DRAM_While_Reserve_Mode_fail(DRAM_DRAM_TYPE_T dram_type)
{
    DRAMC_CTX_T * p;
    mcSHOW_DBG_MSG(("\n\tReserve mode fail\tBefore_Init_DRAM_While_Reserve_Mode_fail\n"));
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
	mcSHOW_DBG_MSG(("LPDDR3 not supported\n"));
	ASSERT(0);
#endif
#else
        psCurrDramCtx = &DramCtx_LPDDR3;
#endif
    }
    p = psCurrDramCtx;  

#if (SW_CHANGE_FOR_SIMULATION==0)
    EnableDramcPhyDCM(p, 0);
#endif
    Switch26MHzDisableDummyReadRefreshAllBank(p);
	*((volatile unsigned int*)M17_SPM_DRAMC_DPY_CLK_SW_CON2) &= ~(0x1 << 2);//20170210: confirmed by chen-Hsiang 
 	*((volatile unsigned int*)SPM_POWER_ON_VAL0) &= ~(0x1 << 28);//20170210: confirmed by chen-Hsiang
}

U8 gPRE_MIOCK_JMETER_HQA_USED_flag=0;
void Set_PRE_MIOCK_JMETER_HQA_USED_flag(U8 value)
{
    gPRE_MIOCK_JMETER_HQA_USED_flag = value;
}
U8 Get_PRE_MIOCK_JMETER_HQA_USED_flag(U8 value)
{
    return gPRE_MIOCK_JMETER_HQA_USED_flag;
}

#ifdef ENABLE_MIOCK_JMETER_HQA
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
    
    p->freq_sel = LP4_DDR3200;   
    DDRPhyFreqSel(p, p->freq_sel);
    vSetVcoreByFreq(p);
    DramcInit((DRAMC_CTX_T *) p);

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


//#define TEST_MODE_MRS
#if SUPPORT_SAVE_OFF_LINE_CALIBRATION
/* DRAM_CBT_MODE_EXTERN_T: Used to differentiate normal vs mixed mode LP4 (dram cbt mode type) */
int Init_DRAM(DRAM_DRAM_TYPE_T dram_type, DRAM_CBT_MODE_EXTERN_T dram_cbt_mode_extern, DRAM_INFO_BY_MRR_T *DramInfo, U8 get_mdl_used, SAVE_OFF_LINE_CALIBRATION_T *offLine_SaveData)
#else
int Init_DRAM(DRAM_DRAM_TYPE_T dram_type, DRAM_CBT_MODE_EXTERN_T dram_cbt_mode_extern, DRAM_INFO_BY_MRR_T *DramInfo, U8 get_mdl_used)
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
    U8 chIdx;

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
    #if SUPPORT_SAVE_OFF_LINE_CALIBRATION
    p->pOffLineSaveData = offLine_SaveData;
    #endif

#if ENABLE_MRR_AFTER_FIRST_K
    if(DramInfo != NULL)
#endif
    {
        if (gfirst_init_flag == 0)
        {
            MPLLInit();
            Global_Option_Init(p);
            gfirst_init_flag = 1;
        }

#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0) 
        EMI_Init(p);    
#endif

// for samsung dram, use original MR14 default value. Don't use u1MR14Value_06VDDQ
// use defulat u1MR14Value[u1CHIdx][u1RankIdx][u1FSPIdx] = (u1FSPIdx==FSP_0) ? 0x4d : 0x10;
// difference : FSP1 MR14 default 0x1e->0x10
#if 0  

    //vender_id is set after EMI_Init
    if (p->dram_type == TYPE_LPDDR4X)
    {
        if(p->vendor_id==VENDOR_HYNIX)
        {
            memcpy(u1MR14Value, u1MR14Value_06VDDQ, sizeof(u1MR14Value));
        }
        else
        {
            #if PU_CAL_05VDDQ
            memcpy(u1MR14Value, u1MR14Value_05VDDQ, sizeof(u1MR14Value));
            #else
            memcpy(u1MR14Value, u1MR14Value_06VDDQ, sizeof(u1MR14Value));
            #endif
        }
    }
#endif
        mcSHOW_DBG_MSG(("\n\n[Bianco] ETT version 0.0.0.1\n dram_type %d, R0 cbt_mode %d, R1 cbt_mode %d VENDOR=%d\n\n", p->dram_type, p->dram_cbt_mode[RANK_0], p->dram_cbt_mode[RANK_1], p->vendor_id));

#if defined(DDR_INIT_TIME_PROFILING)
        TimeProfileBegin();
#endif


    #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    	DramcSave_Time_For_Cal_Init(p);
    #endif

    vDramcInit_PreSettings(p);

#ifdef ENABLE_MIOCK_JMETER_HQA
    if (Get_MDL_Used_Flag()==FIRST_USED)
    {
        if(u1IsLP4Family(p->dram_type))
        {
            PRE_MIOCK_JMETER_HQA_USED(p);
        }
    }
#endif

    #if DUAL_FREQ_K
    SPM_Pinmux_Setting();
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

      //DdrPhyInit((DRAMC_CTX_T *) p);
      u1PrintModeRegWrite =1;
      DramcInit((DRAMC_CTX_T *) p);   
      u1PrintModeRegWrite =0;
//ETT_NO_DRAM }
//ETT_NO_DRAM #if 0 	

      vApplyConfigBeforeCalibration((DRAMC_CTX_T *) p);

#if defined(SLT)
      O1Path_Test(p);
#endif

#ifdef LOOPBACK_TEST
      mcSHOW_DBG_MSG(("\n\nDDR loop-back test\n"));
      // 10nm: B0_DQ8_RG_RX_ARDQS_DQSSTB_CG_EN_B0, 16nm B0_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B0
      // 10nm: B1_DQ8_RG_RX_ARDQS_DQSSTB_CG_EN_B1, 16nm B1_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B1
      vIO32WriteFldAlign_All(DDRPHY_B0_DQ6, 0x1, B0_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B0);
      vIO32WriteFldAlign_All(DDRPHY_B1_DQ6, 0x1, B1_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B1);

      vIO32WriteFldMulti_All(DDRPHY_B0_DQ8, P_Fld(0x0, B0_DQ8_RG_TX_ARDQ_EN_LP4P_B0)
                                          | P_Fld(0x0, B0_DQ8_RG_TX_ARDQ_EN_CAP_LP4P_B0)
                                          | P_Fld(0x0, B0_DQ8_RG_TX_ARDQ_CAP_DET_B0));
                                        //| P_Fld(0x1, B0_DQ8_RG_RX_ARDQS_DQSSTB_CG_EN_B0) // Field only exists for 10nm APHY
      vIO32WriteFldMulti_All(DDRPHY_B1_DQ8, P_Fld(0x0, B1_DQ8_RG_TX_ARDQ_EN_LP4P_B1)
                                          | P_Fld(0x0, B1_DQ8_RG_TX_ARDQ_EN_CAP_LP4P_B1)
                                          | P_Fld(0x0, B1_DQ8_RG_TX_ARDQ_CAP_DET_B1));
                                        //| P_Fld(0x1, B1_DQ8_RG_RX_ARDQS_DQSSTB_CG_EN_B1) // Field only exists for 10nm APHY

      vIO32WriteFldAlign_All(DDRPHY_SHU1_B0_DQ3, 0, SHU1_B0_DQ3_DQ_REV_B0_BIT_09_READ_BASE_EN);

      vIO32WriteFldAlign_All(DDRPHY_SHU1_B1_DQ3, 0, SHU1_B1_DQ3_DQ_REV_B1_BIT_09_READ_BASE_EN);

      //DramcLoopbackTest(p);
      ddrphy_lpbk_test_ctrl();
      while (1);
#endif

#ifdef TEST_MODE_MRS
    if(global_which_test  == 0)          
      TestModeTestMenu();
#endif



#if ENABLE_RANK_NUMBER_AUTO_DETECTION  // only need to do this when DUAL_RANK_ENABLE is 1
#if 0//!__ETT__
if (Get_MDL_Used_Flag()==GET_MDL_USED)
#endif
    {
        DramRankNumberDetection(p);
        DFSInitForCalibration(p);  // Restore setting after rank dection (especially DQ= DQS+16)
    }
#endif

#if defined(DDR_INIT_TIME_PROFILING)
              CPU_Cycle = TimeProfileEnd();
              mcSHOW_ERR_MSG(("\nInit take %d ms\n\r", CPU_Cycle));
              TimeProfileBegin();
#endif
        #if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
        EMI_Init2(); 
        #endif
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
    #ifdef MTK_FIXDDR1600_SUPPORT
        //DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_1, DRAM_DFS_SHUFFLE_4);
        DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_1, DRAM_DFS_SHUFFLE_2);
        DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_1, DRAM_DFS_SHUFFLE_3);
    #else
        DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_1, DRAM_DFS_SHUFFLE_2);

	#if defined(SLT)
		if ((seclib_get_devinfo_with_index(25) & 0x200) >> 9)
			DDRPhyFreqSel(p, LP4_DDR2400);
		else
			DDRPhyFreqSel(p, LP4_DDR3200);
	#else
        DDRPhyFreqSel(p, LP4_DDR2400);
	#endif
        vSetVcoreByFreq(p);
        DFSInitForCalibration(p);
        vDramCalibrationAllChannel(p);    
        #if ENABLE_MRR_AFTER_FIRST_K
        vDramcACTimingOptimize(p);
        #endif
        DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_1, DRAM_DFS_SHUFFLE_3);

	#if defined(SLT)
        DDRPhyFreqSel(p, LP4_DDR2400);
	#else
        DDRPhyFreqSel(p, LP4_DDR3200);
	#endif
        vSetVcoreByFreq(p);
        DFSInitForCalibration(p);
        vDramCalibrationAllChannel(p);  
        #if ENABLE_MRR_AFTER_FIRST_K
        vDramcACTimingOptimize(p);
        #endif

    #endif
    }
    else
    {    
    #if 0
        //DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_1, DRAM_DFS_SHUFFLE_4);
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

    vApplyConfigAfterCalibration(p);
#if FORLOOP_TEST_SAVE_TIME_FOR_CALIBRATION_ON_ETT
	vDramfirstCalibrationdump(p);
#endif
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
	if(u1IsLP4Family(p->dram_type) && (p->pSavetimeData->femmc_Ready==0
			|| (p->pSavetimeData->fcalfail[_MappingFreqArray[LP4_DDR800]])
			|| (p->pSavetimeData->fcalfail[_MappingFreqArray[LP4_DDR2667]])
			|| (p->pSavetimeData->fcalfail[_MappingFreqArray[LP4_DDR3200]])))
	{
		#if EMMC_READY
		write_offline_dram_calibration_data(p->pSavetimeData);
		#endif
		mcSHOW_DBG_MSG(("fcalfail[LP4_DDR800] = 0x%x\n",  p->pSavetimeData->fcalfail[_MappingFreqArray[LP4_DDR800]]));
		mcSHOW_DBG_MSG(("fcalfail[LP4_DDR2667] = 0x%x\n",  p->pSavetimeData->fcalfail[_MappingFreqArray[LP4_DDR2667]]));
		mcSHOW_DBG_MSG(("fcalfail[LP4_DDR3200] = 0x%x\n",  p->pSavetimeData->fcalfail[_MappingFreqArray[LP4_DDR3200]]));
		mcSHOW_DBG_MSG(("Save calibration result to emmc\n"));
	} else {
		mcSHOW_DBG_MSG(("Bypass saving calibration result to emmc\n"));
	}
#endif


#if defined(DDR_INIT_TIME_PROFILING)
    CPU_Cycle=TimeProfileEnd();  
    CPUCycleSum += CPU_Cycle;
    mcSHOW_ERR_MSG(("Total calibrations take %d ms\n\r", CPUCycleSum));
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

    for(chIdx=CHANNEL_A; chIdx<p->support_channel_num; chIdx++)
    {
        mcSHOW_DBG_MSG(("Channel %d RX eye scan.\n\n", chIdx));
        vSetPHY2ChannelMapping(p, chIdx);
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

    //while(1);
#endif


#ifdef TX_EYE_SCAN
    DramcTxEyeScanInit(p);

    for(chIdx=CHANNEL_A; chIdx<p->support_channel_num; chIdx++)
    {
        mcSHOW_DBG_MSG(("Channel %d TX eye scan.\n\n", chIdx));
        vSetPHY2ChannelMapping(p, chIdx);
    {
        #if 1
        DramcTxEyeScanRun(p, 0); //use test engine 2
        #else
        DramcTxEyeScanRun(p, 1);  // use DMA
        #endif        
    }    
    }

    while(1);
#endif //TX_EYE_SCAN
// ETT_NO_DRAM #endif   

#if ETT_NO_DRAM    
    //NoDramDramcRegDump(p);    
    NoDramRegFill();
#endif
    
#if DUMMY_Read_sidebank_signal_from_SPM_DVT_Test
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

    #if CPU_RW_TEST_AFTER_K
    mcSHOW_DBG_MSG(("\n[MEM_TEST] 02: After DFS, before run time config\n"));
    vDramCPUReadWriteTestAfterCalibration(p);
    //vDramCPUReadWriteTestAfterCalibration2(p);
    #endif
    
    mcSHOW_DBG_MSG(("\n\nSettings after calibration\n\n"));
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
        mcSHOW_DBG_MSG(("1st complex R/W mem test pass\n"));
    }
    else
    {
        mcSHOW_DBG_MSG(("1st complex R/W mem test fail :-%d\n", -s4value));
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
    // IC and DRAM read DBI
    DramConfig.DBI_R_onoff[FSP_0] = DBI_OFF;             // only for LP4, uesless in LP3
    DramConfig.DBI_R_onoff[FSP_1] = DBI_ON;              // only for LP4, uesless in LP3
    // IC and DRAM write DBI
    DramConfig.DBI_W_onoff[FSP_0] = DBI_OFF;             // only for LP4, uesless in LP3
    DramConfig.DBI_W_onoff[FSP_1] = DBI_ON;              // only for LP4, uesless in LP3
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
    Init_DRAM(DramConfig.dram_type, CBT_R0_R1_NORMAL, &DramInfo, FIRST_USED);
#endif
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
#define DV_SIMULATION_RUNTIME_CONFIG 1
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
#if SIMPLIFY_VREF_SCAN
    DramConfig->skipVrefScan = DISABLE;
#endif
    //DramConfig->dynamicODT = DISABLE;

    Global_Option_Init(DramConfig);

    vDramcInit_PreSettings(DramConfig);

    DDRPhyFreqSel(DramConfig, DramConfig->freq_sel);

    vSetPHY2ChannelMapping(DramConfig, DramConfig->channel);

    #if DV_SIMULATION_SW_IMPED
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
    DramConfig->freq_sel = LP4_DDR3200;
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
    DramConfig->vendor_id = 0x6;
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
#if SIMPLIFY_VREF_SCAN
    DramConfig->skipVrefScan = DISABLE;
#endif
    //DramConfig->dynamicODT = DISABLE;

    Global_Option_Init(DramConfig);

    vDramcInit_PreSettings(DramConfig);

    //vender_id is set after EMI_Init
    if(DramConfig->vendor_id==VENDOR_HYNIX)
    {
        memcpy(u1MR14Value, u1MR14Value_06VDDQ, sizeof(u1MR14Value));
    }
    else
    {
        #if PU_CAL_05VDDQ
        memcpy(u1MR14Value, u1MR14Value_05VDDQ, sizeof(u1MR14Value));
        #else
        memcpy(u1MR14Value, u1MR14Value_06VDDQ, sizeof(u1MR14Value));
        #endif
    }

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

#if SIMULATION_LP4_ZQ
         DramcZQCalibration(DramConfig);
#endif

#if 0//DV_SIMULATION_CA_TRAINING
    vSetRank(DramConfig, RANK_0);
    CmdBusTrainingLP4(DramConfig);
#endif


    for(ii=RANK_0; ii<RANK_MAX; ii++)
    {
        vSetRank(DramConfig, ii);

        #if DV_SIMULATION_LP4_ZQ
        DramcZQCalibration(DramConfig);
        #endif

        vAutoRefreshSwitch(DramConfig, DISABLE);


        #if DV_SIMULATION_CA_TRAINING
        #if SUPPORT_CBT_K_RANK1==0        
        if(ii == RANK_0)
        #endif
        {
            VrefStatus = CmdBusTrainingLP4(DramConfig);

            #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
            if(((DramConfig->pSavetimeData->fcalfail[DramConfig->freq_sel] & 0x40)>>6)==1)
            {
                    VrefStatus = CmdBusTrainingLP4(DramConfig);
            }
            #endif
            #if SUPPORT_SAVE_OFF_LINE_CALIBRATION
            if(VrefStatus == DRAM_K_VREF)
            CmdBusTrainingLP4(DramConfig);
            #endif

            #if DUAL_FREQ_K
            No_Parking_On_CLRPLL(DramConfig);
            #endif
        }
        #endif


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


