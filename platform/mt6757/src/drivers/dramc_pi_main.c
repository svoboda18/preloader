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
#if (FOR_DV_SIMULATION_USED==0)
#if __ETT__
#include <common.h>
#include <ett_common.h>
#include <api.h>
#include <fan53528.h>
#endif
#endif

#include "dramc_common.h"
#include "dramc_pi_api.h"
#include "x_hal_io.h"
#include "upmu_hw.h"

#if ! __ETT__
#if (FOR_DV_SIMULATION_USED==0)

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

#if defined(DRAM_HQA) || defined(__ETT__) //Darren+
extern unsigned int hqa_vcore_lpm; //sagy
extern unsigned int hqa_vcore_hpm; //sagy
#endif

DRAMC_CTX_T *psCurrDramCtx;

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
  {CBT_NORMAL_MODE, CBT_NORMAL_MODE}, // Only for LP4, useless in LP3
  DBI_OFF,
  DBI_OFF,
  PACKAGE_POP,        // DRAM_PACKAGE_T
  DATA_WIDTH_32BIT,     // DRAM_DATA_WIDTH_T
  DEFAULT_TEST2_1_CAL,    // test2_1;
  DEFAULT_TEST2_2_CAL,    // test2_2;
  TEST_XTALK_PATTERN,     // test_pattern;
  667,
  0x88, //vendor_id initial value
  0, //revision id
  0, //density
  {0,0},
  0,  // u2DelayCellTimex100;
  DISABLE,  // ssc_en;
  DISABLE,   // en_4bitMux;
  
  DISABLE,  // enable_cbt_scan_vref;
  DISABLE,  // enable_rx_scan_vref;
  DISABLE,   // enable_tx_scan_vref;
#if SUPPORT_SAVE_OFF_LINE_CLIABRATION
  NULL,
#endif

   //aru4CalResultFlag[CHANNEL_MAX][RANK_MAX][DRAM_DFS_MAX]
{{{0,0,0,0},  {0,0,0,0}},
  {{0,0,0,0}, {0,0,0,0}}}, 
//aru4CalExecuteFlag[CHANNEL_MAX][RANK_MAX][DRAM_DFS_MAX]
{{{0,0,0,0},  {0,0,0,0}},
  {{0,0,0,0}, {0,0,0,0}}}, 

#if fcFOR_CHIP_ID == fcA60501
  PCB_LOC_ASIDE,//DRAM_PCB_LOC_T pcb_loc;
  {0,0,0,0,}, //U32 ta43_result[4];   // for eye scan to check falling or rising error
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
};

DRAMC_CTX_T DramCtx_LPDDR4 =
{
  CHANNEL_DUAL, // Channel number
  CHANNEL_A,          // DRAM_CHANNEL
  RANK_DUAL,        //DRAM_RANK_NUMBER_T
  RANK_0,               //DRAM_RANK_T
#if DUAL_FREQ_K
  LP4_DDR1600,
#else
  LP4_DDR3200,
#endif  
  DRAM_DFS_SHUFFLE_1,
  TYPE_LPDDR4,        // DRAM_DRAM_TYPE_T
  FSP_0 , //// DRAM Fast switch point type, only for LP4, useless in LP3  
  ODT_OFF,
  {CBT_BYTE_MODE1, CBT_BYTE_MODE1},  // LP4 mass release dram is CBT byte mode
  DBI_OFF,  //read DBI
  DBI_OFF,  // write DBI
  PACKAGE_POP,        // DRAM_PACKAGE_T
  DATA_WIDTH_16BIT,     // DRAM_DATA_WIDTH_T
  DEFAULT_TEST2_1_CAL,    // test2_1;
  DEFAULT_TEST2_2_CAL,    // test2_2;
  TEST_XTALK_PATTERN,     // test_pattern;
  800,
  0x88, //vendor_id initial value  
  0, //revision id
  0, //density
  {0,0},  
  0,  // u2DelayCellTimex100;
  DISABLE,  // ssc_en;
  DISABLE,   // en_4bitMux;
  
  ENABLE,   // enable_cbt_scan_vref;
  ENABLE,  // enable_rx_scan_vref;
  ENABLE,   // enable_tx_scan_vref;
#if SUPPORT_SAVE_OFF_LINE_CLIABRATION
  NULL,
#endif
   //aru4CalResultFlag[CHANNEL_MAX][RANK_MAX][DRAM_DFS_MAX]
{{{0,0,0,0,},  {0,0,0,0}},
  {{0,0,0,0,}, {0,0,0,0}}}, 
//aru4CalExecuteFlag[CHANNEL_MAX][RANK_MAX][DRAM_DFS_MAX]
{{{0,0,0,0,},  {0,0,0,0}},
  {{0,0,0,0,}, {0,0,0,0}}}, 

#if fcFOR_CHIP_ID == fcA60501
  PCB_LOC_ASIDE,//DRAM_PCB_LOC_T pcb_loc;
  {0,0,0,0,}, //U32 ta43_result[4];   // for eye scan to check falling or rising error
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

extern U8 gXRPI_CLK_CMD_CS_offset[CHANNEL_MAX];

extern void EMI_Init(DRAMC_CTX_T *p);
extern void EMI_Init2(void);

#define DDRPHY0AO_BASE Channel_A_PHY_BASE_ADDRESS
#define DDRPHY1AO_BASE Channel_B_PHY_BASE_ADDRESS
#define DRAMC0_AO_BASE Channel_A_DRAMC_AO_BASE_ADDRESS
#define DRAMC1_AO_BASE Channel_B_DRAMC_AO_BASE_ADDRESS
#define DRAMC0NAO_BASE Channel_A_DRAMC_NAO_BASE_ADDRESS
#define DRAMC1NAO_BASE Channel_B_DRAMC_NAO_BASE_ADDRESS
#define SPM_POWER_ON_VAL0 (0x10006004)
#define SPM_SW_RSV_8 (0x10006654) 
#define DRAMC_DPY_CLK_SW_CON2 (0x10006474) 

static U32 u4Cha_mr4_backup = 0;
static U32 u4Chb_mr4_backup = 0;
static U32 u4Cha_dummy_read_backup = 0;
static U32 u4Chb_dummy_read_backup = 0;
static U32 u4Cha_dqsosc_backup = 0;
static U32 u4Chb_dqsosc_backup = 0;

void Dramc_DDR_Reserved_Mode_setting(void)
{
	U32 u4WbrBackup = 0;
	U32 u4DramType = 0;
    DRAMC_CTX_T * p;

	// ---------------------------------------------------------
	// begin of Configure
	/*TINFO="Begin of ddr reserve mode setup for DDRPHY"*/
	//*DRAMC_WBR = 0x3;
	//*DRAMC_WBR = 0x7;
	u4WbrBackup = *((volatile unsigned int*)DRAMC_WBR);
	*((volatile unsigned int*)DRAMC_WBR) = DRAMC_BROADCAST_OFF;
	u4DramType = (*((volatile unsigned int*)(DRAMC0_AO_BASE + 0x0010)) & 0xC00) >> 10;
	mcSHOW_DBG_MSG(("Dramc_DDR_Reserved_Mode_setting dram type =%x \n", u4DramType));
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
    DramcHWGatingOnOff(p,0);//Disable HW Gating tracking for gating tracking fifo mode 

	*((volatile unsigned int*)(0x10006474)) |= (0x03 << 0);//Set spm shuffle enable to 1
	mcDELAY_US(1);
	*((volatile unsigned int*)(0x10006474)) &= ~(0x03 << 0);//Set spm shuffle enable to 0

	if(u4DramType == 1)
	{
		*((volatile unsigned int*)(DDRPHY1AO_BASE + 0x0284 )) &= ~(0x1 << 20);//Open CHB controller clk
	}
	u4Cha_mr4_backup =  *((volatile unsigned int*)(DRAMC0_AO_BASE + 0x0064));
	u4Chb_mr4_backup =  *((volatile unsigned int*)(DRAMC1_AO_BASE + 0x0064));
	*((volatile unsigned int*)(DRAMC0_AO_BASE + 0x0064)) = (u4Cha_mr4_backup | (1 << 29));
	*((volatile unsigned int*)(DRAMC1_AO_BASE + 0x0064)) = (u4Chb_mr4_backup | (1 << 29));
	u4Cha_dqsosc_backup = *((volatile unsigned int*)(DRAMC0_AO_BASE + 0x00C8));
	u4Chb_dqsosc_backup = *((volatile unsigned int*)(DRAMC1_AO_BASE + 0x00C8));
	*((volatile unsigned int*)(DRAMC0_AO_BASE + 0x00C8)) = (u4Cha_dqsosc_backup | (1 << 24) | (1 << 28));
	*((volatile unsigned int*)(DRAMC1_AO_BASE + 0x00C8)) = (u4Chb_dqsosc_backup | (1 << 24) | (1 << 28));
	u4Cha_dummy_read_backup =  *((volatile unsigned int*)(DRAMC0_AO_BASE + 0x00d0));
	u4Chb_dummy_read_backup =  *((volatile unsigned int*)(DRAMC1_AO_BASE + 0x00d0));
	
	//disable DDRPHY dynamic clock gating
	*((volatile unsigned int*)(DRAMC0_AO_BASE  + 0x38 )) &= ~(0x1 << 30);   //R_DMPHYCLKDYNGEN=0
	*((volatile unsigned int*)(DRAMC1_AO_BASE  + 0x38 )) &= ~(0x1 << 30);   //R_DMPHYCLKDYNGEN=0
	
	// disable DDRPHY
	//change mcm_ck to 26M
	//*CLK_CFG_10	= (*CLK_CFG_10) & ~(0x00010000) ; 
	// mem_ck CG enable
	//*CLK_CFG_0	|= (0x1 << 15) ; //bit 15: pdn_mem
	//*CLK_CFG_UPDATE = 0x00000002;
	//! *((volatile unsigned int*)(DDRPHY0AO_BASE + 0x0284)) = 0x00000000; //[5:4] mem_ck mux: 2'b00: 26MHz
	//! clean [5:0]
	*((volatile unsigned int*)(DDRPHY0AO_BASE + 0x0284)) &= ~(0x3f << 0 );  //[5:4] mem_ck mux: 2'b00: 26MHz, [0]: change memory clock 
	//! change clock freq
	*((volatile unsigned int*)(DDRPHY0AO_BASE + 0x0284)) |=  (0x1  << 0 );  //[0]: change memory clock 
	//! disable memory clock change
	*((volatile unsigned int*)(DDRPHY0AO_BASE + 0x0284)) &=  ~(0x1 << 0 );  //[0]: change memory clock 
  
	if((u4DramType == 2) || (u4DramType == 3)) 
	{
		//! *((volatile unsigned int*)(DDRPHY1AO_BASE + 0x0284)) = 0x00000000; //[5:4] mem_ck mux: 2'b00: 26MHz
	  //! clean [5:0]
	*((volatile unsigned int*)(DDRPHY1AO_BASE + 0x0284)) &= ~(0x3f << 0 );  //[5:4] mem_ck mux: 2'b00: 26MHz, [0]: change memory clock 
	//! change to 26M 
	*((volatile unsigned int*)(DDRPHY1AO_BASE + 0x0284)) |=  (0x1  << 0 );  //[0]: change memory clock 
	//! disable memory clock change
	*((volatile unsigned int*)(DDRPHY1AO_BASE + 0x0284)) &=  ~(0x1 << 0 );  //[0]: change memory clock 
	}

	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x025 << 2) )) &= ~(0x1 << 11); //RG_*PHDET_EN=0
	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x045 << 2) )) &= ~(0x1 << 11); //RG_*PHDET_EN=0
	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x065 << 2) )) &= ~(0x1 << 11); //RG_*PHDET_EN=0

	if((u4DramType == 2) || (u4DramType == 3)) 
	{
		*((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x025 << 2) )) &= ~(0x1 << 11); //RG_*PHDET_EN=0
		*((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x045 << 2) )) &= ~(0x1 << 11); //RG_*PHDET_EN=0
		*((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x065 << 2) )) &= ~(0x1 << 11); //RG_*PHDET_EN=0
	}

	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x022 << 2) )) |= ((0x1 << 31) | (0x1 << 27) | (0x1 << 19) | (0x1 << 17) | (0x1 << 15) | (0x1 << 14) | (0x1 << 13) | (0x1 << 11) | (0x1 << 10));   //*PI_CG=1, RG_*MPDIV_CG=1
	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x042 << 2) )) |= ((0x1 << 31) | (0x1 << 27) | (0x1 << 19) | (0x1 << 17) | (0x1 << 15) | (0x1 << 14) | (0x1 << 13) | (0x1 << 11) | (0x1 << 10));   //*PI_CG=1, RG_*MPDIV_CG=1
	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x062 << 2) )) |= ((0x1 << 31) | (0x1 << 27) | (0x1 << 19) | (0x1 << 17) | (0x1 << 15) | (0x1 << 16) | (0x1 << 13) | (0x1 << 11) | (0x1 << 10));   //*PI_CG=1, RG_*MPDIV_CG=1

	if((u4DramType == 2) || (u4DramType == 3))
	{
		*((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x022 << 2) )) |= ((0x1 << 31) | (0x1 << 27) | (0x1 << 19) | (0x1 << 17) | (0x1 << 15) | (0x1 << 14) | (0x1 << 13) | (0x1 << 11) | (0x1 << 10));   //*PI_CG=1, RG_*MPDIV_CG=1
		*((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x042 << 2) )) |= ((0x1 << 31) | (0x1 << 27) | (0x1 << 19) | (0x1 << 17) | (0x1 << 15) | (0x1 << 14) | (0x1 << 13) | (0x1 << 11) | (0x1 << 10));   //*PI_CG=1, RG_*MPDIV_CG=1
		*((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x062 << 2) )) |= ((0x1 << 31) | (0x1 << 27) | (0x1 << 19) | (0x1 << 17) | (0x1 << 15) | (0x1 << 16) | (0x1 << 13) | (0x1 << 11) | (0x1 << 10));   //*PI_CG=1, RG_*MPDIV_CG=1
	}

	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x02c << 2) )) &= ~(0x1 << 12);   //RG_*BIAS_EN=0
	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x04c << 2) )) &= ~(0x1 << 12);   //RG_*BIAS_EN=0
	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x06c << 2) )) &= ~(0x1 << 12);   //RG_*BIAS_EN=0

	if((u4DramType == 2) || (u4DramType == 3)) 
	{
		*((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x02c << 2) )) &= ~(0x1 << 12);   //RG_*BIAS_EN=0
		*((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x04c << 2) )) &= ~(0x1 << 12);   //RG_*BIAS_EN=0
		*((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x06c << 2) )) &= ~(0x1 << 12);   //RG_*BIAS_EN=0
	}

	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x02b << 2) )) &= ~(0x1 << 16);   //RG_*VREF_EN=0
	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x04b << 2) )) &= ~(0x1 << 16);   //RG_*VREF_EN=0
	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x06b << 2) )) &= ~(0x1 << 16);   //RG_*VREF_EN=0

	if((u4DramType == 2) || (u4DramType == 3)) 
	{
		*((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x02b << 2) )) &= ~(0x1 << 16);   //RG_*VREF_EN=0
		*((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x04b << 2) )) &= ~(0x1 << 16);   //RG_*VREF_EN=0
		*((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x06b << 2) )) &= ~(0x1 << 16);   //RG_*VREF_EN=0
	}

	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x003 << 2) )) &= ~((0x1 << 22) | (0x1 << 21));   //RG_*MCK8X_EN=0
	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x306 << 2) )) &= ~(0x1 << 26);   //RG_*MIDPI_EN=0
	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x326 << 2) )) &= ~(0x1 << 26);   //RG_*MIDPI_EN=0
	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x346 << 2) )) &= ~(0x1 << 26);   //RG_*MIDPI_EN=0

	if((u4DramType == 2) || (u4DramType == 3)) 
	{
		*((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x306 << 2) )) &= ~(0x1 << 26);   //RG_*MIDPI_EN=0
		*((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x326 << 2) )) &= ~(0x1 << 26);   //RG_*MIDPI_EN=0
		*((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x346 << 2) )) &= ~(0x1 << 26);   //RG_*MIDPI_EN=0
	}

	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x000 << 2)))  &= ~(0x1 << 31);   //RG_*PLL_EN=0
	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x001 << 2) )) &= ~(0x1 << 31);

	//!set sc_mpll to SPM register 
	*((volatile unsigned int*)(0x1000644C)) |= (0x1 << 8) | (0x1 << 4); //set sc_mpll_off=1 , sc_mpll_s_off=1

	//TBD
	//*((UINT32P)(0x1000631c )) |= (0x1 << 1); //ddrphy_pwr_iso=1 //Lewis@20160621: Fix LP3 Hang from S0 suspend into reserve mode
	//*((UINT32P)(0x100063b8 )) |= (0x1 << 1);

	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x029 << 2) )) &= ~(0x1 << 3);   //RG_*RESETB=0
	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x020 << 2) )) &= ~(0x1 << 3);   //RG_*RESETB=0
	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x049 << 2) )) &= ~(0x1 << 3);   //RG_*RESETB=0
	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x040 << 2) )) &= ~(0x1 << 3);   //RG_*RESETB=0
	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x069 << 2) )) &= ~(0x1 << 3);   //RG_*RESETB=0
	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x060 << 2) )) &= ~(0x1 << 3);   //RG_*RESETB=0
	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x003 << 2) )) &= ~(0x1 << 16);  //RG_*RESETB=0

	//if((u4DramType == 2) || (u4DramType == 3))//Lewis@20160628: Fix LP3 enter S0 then into reserve mode fail due to CHB PHY not reset(LP3 PHY use 2 channels)
	{
		*((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x029 << 2) )) &= ~(0x1 << 3);   //RG_*RESETB=0
		*((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x020 << 2) )) &= ~(0x1 << 3);   //RG_*RESETB=0
		*((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x049 << 2) )) &= ~(0x1 << 3);   //RG_*RESETB=0
		*((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x040 << 2) )) &= ~(0x1 << 3);   //RG_*RESETB=0
		*((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x069 << 2) )) &= ~(0x1 << 3);   //RG_*RESETB=0
		*((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x060 << 2) )) &= ~(0x1 << 3);   //RG_*RESETB=0
		*((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x003 << 2) )) &= ~(0x1 << 16);  //RG_*RESETB=0
	}

#if (fcFOR_CHIP_ID == fcOlympus)
	*((volatile unsigned int *)(MPLL_PWR_CON0)) |= 0x1; //[0]: MPLL_PWR_ON=1
	mcDELAY_US(1);
	*((volatile unsigned int *)(MPLL_PWR_CON0)) &= ~(0x1 <<1); //[1]: MPLL_ISO_EN=0
	// *MDM_TM_WAIT_US = 1; // Wait 1us
	// while (*MDM_TM_WAIT_US>0);
	*((volatile unsigned int *)(MPLL_CON1)) |= (0x1<<31); //[31]: MPLL_SDM_PCW_CHG=1
	*((volatile unsigned int *)(MPLL_CON0)) |= 0x1; //[0]: MPLL_EN=1
	mcDELAY_US(20);
#elif (fcFOR_CHIP_ID == fcElbrus)
	*((volatile unsigned int *)(MPLL_PWR_CON0))  |= 0x1;    //[0]: MPLL_PWR_ON=1
	mcDELAY_US(30);
	*((volatile unsigned int *)(MPLL_PWR_CON0))  &=  ~(0x1 <<1); //[1]: MPLL_ISO_EN=0
	mcDELAY_US(1);
	*((volatile unsigned int *)(MPLL_CON1))  = 0xC0200000;
	*((volatile unsigned int *)(MPLL_CON0))  = 0xFED10101;    //[24]: MPLL_DIV_RSTB=0, [0]: MPLL_EN=1
	mcDELAY_US(20);
	*((volatile unsigned int *)(MPLL_CON0))  = 0xFFD10101;    //[24]: MPLL_DIV_RSTB=1
#endif

	//marked by kaihsin on May 7th, not necessary to PDN DDRPHY
	//*((UINT32P)(0x1000631c )) &= ~(0x1 << 2); //ddrphy_pwr_on=0
	//*((UINT32P)(0x100063b8 )) &= ~(0x1 << 2);

	// wait 1us
	mcDELAY_US(1);

	// enable DDRPHY by normal flow

	// enable DDRPHY by normal flow
	//marked by kaihsin on May 7th, not necessary to PDN DDRPHY
	//*((UINT32P)(0x1000631c )) |= (0x1 << 2); //ddrphy_pwr_on=1
	//*((UINT32P)(0x100063b8 )) |= (0x1 << 2);
	// wait 1us
	mcDELAY_US(1);

	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x029 << 2) )) |= (0x1 << 3);   //RG_*RESETB=1
	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x020 << 2) )) |= (0x1 << 3);   //RG_*RESETB=1
	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x049 << 2) )) |= (0x1 << 3);   //RG_*RESETB=1
	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x040 << 2) )) |= (0x1 << 3);   //RG_*RESETB=1
	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x069 << 2) )) |= (0x1 << 3);   //RG_*RESETB=1
	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x060 << 2) )) |= (0x1 << 3);   //RG_*RESETB=1
	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x003 << 2) )) |= (0x1 << 16);  //RG_*RESETB=1

	//if((u4DramType == 2) || (u4DramType == 3))//Lewis@20160628: Fix LP3 enter S0 then into reserve mode fail due to CHB PHY not reset(LP3 PHY use 2 channels) 
	{
		*((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x029 << 2) )) |= (0x1 << 3);   //RG_*RESETB=1
		*((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x020 << 2) )) |= (0x1 << 3);   //RG_*RESETB=1
		*((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x049 << 2) )) |= (0x1 << 3);   //RG_*RESETB=1
		*((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x040 << 2) )) |= (0x1 << 3);   //RG_*RESETB=1
		*((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x069 << 2) )) |= (0x1 << 3);   //RG_*RESETB=1
		*((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x060 << 2) )) |= (0x1 << 3);   //RG_*RESETB=1
		*((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x003 << 2) )) |= (0x1 << 16);  //RG_*RESETB=1
	}

	//TBD
	//*((UINT32P)(0x1000631c )) &= ~(0x1 << 1); //ddrphy_pwr_iso=0 //Lewis@20160621: Fix LP3 Hang from S0 suspend into reserve mode
	//*((UINT32P)(0x100063b8 )) &= ~(0x1 << 1);

	//MPLL enable -- TBD
	// wait 20us for MPLL
	//!set sc_mpll to SPM register 
	*((volatile unsigned int*)(0x1000644C)) &= ~((0x1 << 8) | (0x1 << 4)); //set sc_mpll_off=0 , sc_mpll_s_off=0
	mcDELAY_US(20);
	
	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x000 << 2))) |= (0x1 << 31);   //RG_*PLL_EN=1
	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x001 << 2))) |= (0x1 << 31);

	// wait 20us for MEMPLL
	mcDELAY_US(20);
	
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

	//! orig *((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x003 << 2) )) |= ((0x1 << 22) | (0x1 << 21));   //RG_*MCK8X_EN=0
	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x003 << 2) )) |= (0x1 << 22);                            //RG_*ADA_MCK8X_EN=1
	//!Olympus : LP3 preloader  don't set this bit, *((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x306 << 2) )) |= (0x1 << 26);   //RG_*MIDPI_EN=0
	//!Olympus : LP3 preloader  don't set this bit, *((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x326 << 2) )) |= (0x1 << 26);   //RG_*MIDPI_EN=0
	//!Olympus : LP3 preloader  don't set this bit, *((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x346 << 2) )) |= (0x1 << 26);   //RG_*MIDPI_EN=0

	if((u4DramType == 2) || (u4DramType == 3)) 
	{
		//! Olympus : LP4 preloader need RG_*MIDPI_EN
	    *((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x306 << 2) )) |= (0x1 << 26);   //RG_*MIDPI_EN=0
	    *((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x326 << 2) )) |= (0x1 << 26);   //RG_*MIDPI_EN=0
	    *((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x346 << 2) )) |= (0x1 << 26);   //RG_*MIDPI_EN=0
	    *((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x306 << 2) )) |= (0x1 << 26);   //RG_*MIDPI_EN=0
	    *((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x326 << 2) )) |= (0x1 << 26);   //RG_*MIDPI_EN=0
	    *((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x346 << 2) )) |= (0x1 << 26);   //RG_*MIDPI_EN=0
	}
	// wait 1us
	mcDELAY_US(1);

	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x022 << 2) )) &= ~((0x1 << 31) | (0x1 << 27) | (0x1 << 19) | (0x1 << 17) | (0x1 << 15) | (0x1 << 14) | (0x1 << 13) | (0x1 << 11) | (0x1 << 10));   //*PI_CG=0, RG_*MPDIV_CG=0
	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x042 << 2) )) &= ~((0x1 << 31) | (0x1 << 27) | (0x1 << 19) | (0x1 << 17) | (0x1 << 15) | (0x1 << 14) | (0x1 << 13) | (0x1 << 11) | (0x1 << 10));   //*PI_CG=0, RG_*MPDIV_CG=0
	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x062 << 2) )) &= ~((0x1 << 31) | (0x1 << 27) | (0x1 << 19) | (0x1 << 17) | (0x1 << 15) | (0x1 << 16) | (0x1 << 13) | (0x1 << 11) | (0x1 << 10));   //*PI_CG=0, RG_*MPDIV_CG=0

	if((u4DramType == 2) || (u4DramType == 3)) 
	{
		//!Olympus : LP4 preloader  ARPI_CG_CLKIEN = 1
		*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x062 << 2) )) |=   (0x1 << 11);   //*PI_CG=0, RG_*MPDIV_CG=0
		*((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x022 << 2) )) &= ~((0x1 << 31) | (0x1 << 27) | (0x1 << 19) | (0x1 << 17) | (0x1 << 15) | (0x1 << 14) | (0x1 << 13) | (0x1 << 11) | (0x1 << 10));   //*PI_CG=0, RG_*MPDIV_CG=0
		*((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x042 << 2) )) &= ~((0x1 << 31) | (0x1 << 27) | (0x1 << 19) | (0x1 << 17) | (0x1 << 15) | (0x1 << 14) | (0x1 << 13) | (0x1 << 11) | (0x1 << 10));   //*PI_CG=0, RG_*MPDIV_CG=0
		//! orig *((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x062 << 2) )) &= ~((0x1 << 31) | (0x1 << 27) | (0x1 << 19) | (0x1 << 17) | (0x1 << 15) | (0x1 << 16) | (0x1 << 13) | (0x1 << 11) | (0x1 << 10));   //*PI_CG=0, RG_*MPDIV_CG=0
		//!Olympus : LP4 preloader  ARPI_CG_CLKIEN = 1
		*((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x062 << 2) )) &= ~((0x1 << 31) | (0x1 << 27) | (0x1 << 19) | (0x1 << 17) | (0x1 << 15) | (0x1 << 16) | (0x1 << 13) | (0x1 << 10));                 //*PI_CG=0, RG_*MPDIV_CG=0
	}

	//!Olympus : LP3 preloader  don't set this bit, *((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x02c << 2) )) |= (0x1 << 12);   //RG_*BIAS_EN=1
	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x04c << 2) )) |= (0x1 << 12);   //RG_*BIAS_EN=1
	//! olympus LP3,LP4 preloader don't assert this bit, *((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x06c << 2) )) |= (0x1 << 12);   //RG_*BIAS_EN=1

	if((u4DramType == 2) || (u4DramType == 3)) 
	{
		//! Olympus : LP4 preloader  set this bit,
		*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x02c << 2) )) |= (0x1 << 12);   //RG_*BIAS_EN=1
		
		*((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x02c << 2) )) |= (0x1 << 12);   //RG_*BIAS_EN=1
		*((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x04c << 2) )) |= (0x1 << 12);   //RG_*BIAS_EN=1
		//! olympus LP3,LP4 preloader don't assert this bit, *((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x06c << 2) )) |= (0x1 << 12);   //RG_*BIAS_EN=1
	}
	
	// wait 1us
	mcDELAY_US(1);

	//change mem_ck to mempll
	//*CLK_CFG_10	= (*CLK_CFG_10) | 0x00010000 ; 
	//mem_ck CG disable
	//*CLK_CFG_0	&= ~(0x1 << 15) ; //bit 15: pdn_mem
	//*CLK_CFG_UPDATE = 0x00000002;
	//!orig : *((volatile unsigned int*)(DDRPHY0AO_BASE + 0x0284)) = 0x0000001f; //[5:4] mem_ck mux: 2'b01: memory clock
	*((volatile unsigned int*)(DDRPHY0AO_BASE + 0x0284)) |=  (0x1 << 4);   //[5:4] mem_ck mux: 2'b01: memory clock
	//! change clock freq
	*((volatile unsigned int*)(DDRPHY0AO_BASE + 0x0284)) |=  (0x1 <<0 );   //[0]: change memory clock 
	//! disable memory clock change
	*((volatile unsigned int*)(DDRPHY0AO_BASE + 0x0284)) &= ~(0x1 <<0 );   //[0]: change memory clock 

	if((u4DramType == 2) || (u4DramType == 3)) 
	{
		//! orig : *((volatile unsigned int*)(DDRPHY1AO_BASE + 0x0284)) = 0x0000001f; //[5:4] mem_ck mux: 2'b01: 26MHz
		*((volatile unsigned int*)(DDRPHY1AO_BASE + 0x0284)) |=  (0x1 << 4);   //[5:4] mem_ck mux: 2'b01: memory clock
		//! change clock freq
		*((volatile unsigned int*)(DDRPHY1AO_BASE + 0x0284)) |=  (0x1 <<0 );   //[0]: change memory clock 
		//! disable memory clock change
		*((volatile unsigned int*)(DDRPHY1AO_BASE + 0x0284)) &= ~(0x1 <<0 );   //[0]: change memory clock 
	}
	
	//force top feedback MCK not divide
	//rg_ddrphy_fb_ck_force_en = 1 -- TBD
	//*CLK_MEM_DFS_CFG |= (0x1 << 8); //rg_ddrphy_fb_ck_force_en = 1
	*((volatile unsigned int*)(0x10060004 )) |= (0x1 << 16) ; //set sc_ddrphy_fb_ck_ch*_en = 1
	*((volatile unsigned int*)(0x10060464 )) |= (0x3 << 20) ; //set sc_ddrphy_fb_ck_ch*_en = 1

	//*DRAMC_WBR = 0x3;
	
	//1st DLL enable
	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x065 << 2) )) |= (0x1 << 11); //RG_*PHDET_EN=1 (CA)
	// wait 1us
	mcDELAY_US(1);
	
	//2nd DLL enable

	if((u4DramType == 2) || (u4DramType == 3)) 
	{
		*((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x065 << 2) )) |= (0x1 << 11); //RG_*PHDET_EN=1 (CA)
	}

	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x025 << 2) )) |= (0x1 << 11); //RG_*PHDET_EN=1 (B0/B1)
	*((volatile unsigned int*)(DDRPHY0AO_BASE  + (0x045 << 2) )) |= (0x1 << 11); //RG_*PHDET_EN=1 (B0/B1)

	if((u4DramType == 2) || (u4DramType == 3)) 
	{
		*((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x025 << 2) )) |= (0x1 << 11); //RG_*PHDET_EN=1 (B0/B1)
		*((volatile unsigned int*)(DDRPHY1AO_BASE  + (0x045 << 2) )) |= (0x1 << 11); //RG_*PHDET_EN=1 (B0/B1)
	}
	// wait 1us
	//DELAY_US(1);
	
	//top feedback MCK to divided frequency -- TBD
	//*CLK_MEM_DFS_CFG &= ~(0x1 << 8); //rg_ddrphy_fb_ck_force_en = 0
	*((volatile unsigned int*)(0x10060004 )) &= ~(0x1 << 16) ; //set sc_ddrphy_fb_ck_ch*_en = 0
	*((volatile unsigned int*)(0x10060464 )) &= ~(0x3 << 20) ; //set sc_ddrphy_fb_ck_ch*_en = 0

	//*((UINT32P)(DRAMC0_AO_BASE  + 0x0f4 )) |= (0x1 << 25);   //R_DMRDATRST=1
	//*((UINT32P)(DRAMC1_BASE  + 0x0f4 )) |= (0x1 << 25);   //R_DMRDATRST=1
	//*((UINT32P)(DRAMC0_AO_BASE  + 0x0f4 )) &= ~(0x1 << 25);   //R_DMRDATRST=0
	//*((UINT32P)(DRAMC1_BASE  + 0x0f4 )) &= ~(0x1 << 25);   //R_DMRDATRST=0
	*((volatile unsigned int*)(DRAMC0_AO_BASE  + 0x0 )) |=  (0x1 << 0);   //R_DMRDATRST = 1
	*((volatile unsigned int*)(DRAMC0_AO_BASE  + 0x0 )) &= ~(0x1 << 0);   //R_DMRDATRST = 0

	if((u4DramType == 2) || (u4DramType == 3)) 
	{
		*((volatile unsigned int*)(DRAMC1_AO_BASE  + 0x0 )) |=  (0x1 << 0);   //R_DMRDATRST = 1
		*((volatile unsigned int*)(DRAMC1_AO_BASE  + 0x0 )) &= ~(0x1 << 0);   //R_DMRDATRST = 0
		}
	//*DRAMC_WBR = 0x7;

	//enable DDRPHY dynamic clock gating
	*((volatile unsigned int*)(DRAMC0_AO_BASE  + 0x38 )) |= (0x1 << 30);   //R_DMPHYCLKDYNGEN=1

	if((u4DramType == 2) || (u4DramType == 3)) 
	{
		*((volatile unsigned int*)(DRAMC1_AO_BASE  + 0x38 )) |= (0x1 << 30);   //R_DMPHYCLKDYNGEN=1
	}

	mcDELAY_US(1);
	//! set SPM to control PLL enable and disable PLL enable from ddrphy conf
	/*TINFO ="Switching PHYPLL enable path from DDRPHY to SPM control by setting SPM SW" */
	*((volatile unsigned int*)SPM_POWER_ON_VAL0)      |= (0x1 << 28);
	*((volatile unsigned int*)DRAMC_DPY_CLK_SW_CON2)  |= (0x1 << 2);
	mcDELAY_US(1);
	*((volatile unsigned int*)(DDRPHY0AO_BASE + 0x0000)) &= ~(0x1 << 31); //disable DDRPHY PHYPLL enable
	*((volatile unsigned int*)(DDRPHY0AO_BASE + 0x0004)) &= ~(0x1 << 31); //disable DDRPHY CLRPLL enable

	if((u4DramType == 2) || (u4DramType == 3)) 
	{
		/*TINFO ="Setting RX input delay tracking enable from SPM side(un-paused)" */
		//! speed > 3200 need enable RX input delay tracking 
		*((volatile unsigned int*)SPM_SW_RSV_8)  |= (0x3 << 6);  //! setting SPM rsv_8 (SLEEP_BASE + 0x654) [7:6] = 0x3 for RX input delay HW tracking enable (un-paused)
		/*TINFO ="enable DUMMY RD function for 4us " */
		//! LP4 need 4us run time dummy read 
		//*((volatile unsigned int*)(DRAMC0_AO_BASE + 0x00d0)) |= (1 << 20);
	}
	/*TINFO="End of ddr reserve mode setup for DDRPHY"*/

	//! diable dummy read when exit sref
	*((volatile unsigned int*)(DRAMC0_AO_BASE + 0x00d0)) &= ~((1 << 23)|(1 << 22)|(1 << 21)|(1 << 20)|(1 << 7)|(1 << 6));

	if((u4DramType == 2) || (u4DramType == 3))
	{
		*((volatile unsigned int*)(DRAMC1_AO_BASE + 0x00d0)) &= ~((1 << 23)|(1 << 22)|(1 << 21)|(1 << 20)|(1 << 7)|(1 << 6));
	}

	*((volatile unsigned int*)DRAMC_WBR) = u4WbrBackup;
	return;
}

void Dramc_DDR_Reserved_Mode_AfterSR(void)
{
	U32 u4DramType = 0;
    DRAMC_CTX_T *p = psCurrDramCtx;
    U8 u1channel = CHANNEL_A;
    U32 backup_broadcast = GetDramcBroadcast();

	DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);	
	u4DramType = (*((volatile unsigned int*)(DRAMC0_AO_BASE + 0x0010)) & 0xC00) >> 10;
	mcSHOW_DBG_MSG(("Dramc_DDR_Reserved_Mode_AfterSR dram type =%x \n", u4DramType));

	if(u4DramType == 1)
	{
        vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0 + ((U32)CHANNEL_B<<POS_BANK_NUM), 1, MISC_CG_CTRL0_K_RG_CG_DRAMC_CHB_CK_OFF);//Close CHB controller clk

        p->channel = CHANNEL_A;
        DramcModeRegWriteByRank(p, 0, 2, 0x1c);
        DramcModeRegWriteByRank(p, 0, 17, 0);//R1 lost data WA, set R0 R1 both for safe{On charging mode, system may issue MR17=0xFF in R1}
        if(u4IO32ReadFldAlign(DRAMC_REG_RSTMASK, RSTMASK_RSV_DRAM_SUPPORT_RANK_NUM) == 0)//DRAM is dual rank
        {
            DramcModeRegWriteByRank(p, 1, 2, 0x1c);
            DramcModeRegWriteByRank(p, 1, 17, 0);//R1 lost data WA, set R0 R1 both for safe{On charging mode, system may issue MR17=0xFF in R1}
        }
	} 
	else if((u4DramType == 2) || (u4DramType == 3))
	{	// LPDDR4

        for(u1channel = CHANNEL_A; u1channel < p->support_channel_num; u1channel++)
        {
            p->channel = u1channel;
            DramcModeRegWriteByRank(p, 0, 13, 0xc0);
            DramcModeRegWriteByRank(p, 0, 2, 0x2d);
            DramcModeRegWriteByRank(p, 0, 17, 0);//R1 lost data WA, set R0 R1 both for safe{On charging mode, system may issue MR17=0xFF in R1}
            if(u4IO32ReadFldAlign(DRAMC_REG_RSTMASK, RSTMASK_RSV_DRAM_SUPPORT_RANK_NUM) == 0)//DRAM is dual rank
            {
                DramcModeRegWriteByRank(p, 1, 13, 0xc0);
                DramcModeRegWriteByRank(p, 1, 2, 0x2d);
                DramcModeRegWriteByRank(p, 1, 17, 0);//R1 lost data WA, set R0 R1 both for safe{On charging mode, system may issue MR17=0xFF in R1}
            }
        }
	}
	*((volatile unsigned int*)(DRAMC0_AO_BASE + 0x0064)) = u4Cha_mr4_backup;
	*((volatile unsigned int*)(DRAMC1_AO_BASE + 0x0064)) = u4Chb_mr4_backup;
	*((volatile unsigned int*)(DRAMC0_AO_BASE + 0x00d0)) = u4Cha_dummy_read_backup;
	*((volatile unsigned int*)(DRAMC1_AO_BASE + 0x00d0)) = u4Chb_dummy_read_backup;
	*((volatile unsigned int*)(DRAMC1_AO_BASE + 0x00C8)) = u4Chb_dqsosc_backup;
	*((volatile unsigned int*)(DRAMC0_AO_BASE + 0x00C8)) = u4Cha_dqsosc_backup;
    DramcHWGatingOnOff(p,1);//Enable HW Gating tracking for gating tracking fifo mode    
    DramcBroadcastOnOff(backup_broadcast);
	return;
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
#ifdef DRAM_HQA //Darren+
    set_vcore_by_freq(p); //sagy for HQA
#else
#if fcFOR_CHIP_ID == fcOlympus
    if(p->dram_type == TYPE_LPDDR4P)
    { 
#if defined(PMIC_CHIP_MT6355) 
        regulator_set_voltage(reg_vdram2, 400000, MAX_VDRAM2);
#else
        fan53528buc08x_set_voltage((unsigned long)400000); 
        //fan53526_get_voltage();
#endif
    }
    else if(p->dram_type == TYPE_LPDDR4X)
    {
#if defined(PMIC_CHIP_MT6355) 
        regulator_set_voltage(reg_vdram2, 600000, MAX_VDRAM2);
#else
        fan53528buc08x_set_voltage((unsigned long)600000);
#endif
    }    
    
    if(u1IsLP4Family(p->dram_type))    
    {
        if(p->frequency>=1600)
        {
#if defined(PMIC_CHIP_MT6355) 
            regulator_set_voltage(reg_vcore, 800000, MAX_VCORE);
#else
            pmic_config_interface(MT6351_BUCK_VCORE_CON4,0x20,0x7F,0); // 0.8V
            pmic_config_interface(MT6351_BUCK_VCORE_CON5,0x20,0x7F,0); // 0.8V
#endif
        }
        else if(p->frequency>=1333)
        {
#if defined(PMIC_CHIP_MT6355) 
            regulator_set_voltage(reg_vcore, 750000, MAX_VCORE);
#else
            pmic_config_interface(MT6351_BUCK_VCORE_CON4,0x18,0x7F,0); // 0.75V
            pmic_config_interface(MT6351_BUCK_VCORE_CON5,0x18,0x7F,0); // 0.75V
#endif
        }
        else if(p->frequency>=800)
        {
#if defined(PMIC_CHIP_MT6355) 
            regulator_set_voltage(reg_vcore, 700000, MAX_VCORE);
#else
            pmic_config_interface(MT6351_BUCK_VCORE_CON4,0x10,0x7F,0); // 0.7V
            pmic_config_interface(MT6351_BUCK_VCORE_CON5,0x10,0x7F,0); // 0.7V
#endif
        }
    }
    else if (p->dram_type == TYPE_LPDDR3)
    {
        if(p->frequency>=933)
        {
#if defined(PMIC_CHIP_MT6355) 
            regulator_set_voltage(reg_vcore, 800000, MAX_VCORE);
#else
            pmic_config_interface(MT6351_BUCK_VCORE_CON4,0x20,0x7F,0); // 0.8V
            pmic_config_interface(MT6351_BUCK_VCORE_CON5,0x20,0x7F,0); // 0.8V
#endif
        }
        else if(p->frequency>=667)
        {
#if defined(PMIC_CHIP_MT6355) 
            regulator_set_voltage(reg_vcore, 800000, MAX_VCORE);
#else
            pmic_config_interface(MT6351_BUCK_VCORE_CON4,0x20,0x7F,0); // 0.8V
            pmic_config_interface(MT6351_BUCK_VCORE_CON5,0x20,0x7F,0); // 0.8V
#endif
        }
        else if(p->frequency>=467)
        {
#if defined(PMIC_CHIP_MT6355) 
            regulator_set_voltage(reg_vcore, 700000, MAX_VCORE); //no use
#else
            pmic_config_interface(MT6351_BUCK_VCORE_CON4,0x10,0x7F,0); // 0.7V
            pmic_config_interface(MT6351_BUCK_VCORE_CON5,0x10,0x7F,0); // 0.7V
#endif
        }   
    }
#elif fcFOR_CHIP_ID == fcElbrus
    //Keep Vcore=0.8
#endif
#endif //end of #ifdef DRAM_HQA
}


void mem_test_address_calculation(DRAMC_CTX_T * p, U32 uiSrcAddr, U32*pu4Dest)
{
    U32 u4RankSize;
#if ENABLE_MRR_AFTER_FIRST_K
    *pu4Dest = uiSrcAddr + p->ranksize[RANK_0];
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
        #if (FOR_DV_SIMULATION_USED==0)
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
		 DDRfreq=LP4_DDR1600;
		 else if(freq==1)
		 DDRfreq=LP4_DDR2667;
		 else if(freq==2)
		 DDRfreq=LP4_DDR3200;
 	
		for(ch=0;ch<2;ch++)
		{
			for(rank=0;rank<2;rank++)
			{
				//mcSHOW_DBG_MSG(("p->pSavetimeData->u1WriteLeveling_Save[%d][%d][%d]=%d;\n",ch,rank,DDRfreq,p->pSavetimeData->u1WriteLeveling_Save[ch][rank][DDRfreq]));
				for(byte=0;byte<2;byte++)
				{		
		  			 mcSHOW_DBG_MSG(("p->pSavetimeData->u1Gating2T_Save[%d][%d][%d][%d]=%d;\n",ch,rank,DDRfreq,byte,p->pSavetimeData->u1Gating2T_Save[ch][rank][DDRfreq][byte]));
		  			mcSHOW_DBG_MSG(("p->pSavetimeData->u1Gating05T_Save[%d][%d][%d][%d]=%d;\n",ch,rank,DDRfreq,byte,p->pSavetimeData->u1Gating05T_Save[ch][rank][DDRfreq][byte]));
		  			mcSHOW_DBG_MSG(("p->pSavetimeData->u1Gatingfine_tune_Save[%d][%d][%d][%d]=%d;\n",ch,rank,DDRfreq,byte,p->pSavetimeData->u1Gatingfine_tune_Save[ch][rank][DDRfreq][byte]));
		  			mcSHOW_DBG_MSG(("p->pSavetimeData->u1Gatingucpass_count_Save[%d][%d][%d][%d]=%d;\n",ch,rank,DDRfreq,byte,p->pSavetimeData->u1Gatingucpass_count_Save[ch][rank][DDRfreq][byte]));
		  			mcSHOW_DBG_MSG(("p->pSavetimeData->u1WriteLeveling_bypass_Save[%d][%d][%d][%d]=%d;\n",ch,rank,DDRfreq,byte,p->pSavetimeData->u1WriteLeveling_bypass_Save[ch][rank][DDRfreq][byte]));

				mcSHOW_DBG_MSG(("p->pSavetimeData->u1TxCenter_min_Save[%d][%d][%d][%d]=%d;\n",ch,rank,DDRfreq,byte,p->pSavetimeData->u1TxCenter_min_Save[ch][rank][DDRfreq][byte]));
 				mcSHOW_DBG_MSG(("p->pSavetimeData->u1TxCenter_max_Save[%d][%d][%d][%d]=%d;\n",ch,rank,DDRfreq,byte,p->pSavetimeData->u1TxCenter_max_Save[ch][rank][DDRfreq][byte]));
				}
				 for (DQ=0; DQ<16; DQ++)
        			{
 		  			mcSHOW_DBG_MSG(("p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[%d][%d][%d][%d]=%d;\n",ch,rank,DDRfreq,DQ,p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[ch][rank][DDRfreq][DQ]));
		  			mcSHOW_DBG_MSG(("p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[%d][%d][%d][%d]=%d;\n",ch,rank,DDRfreq,DQ,p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[ch][rank][DDRfreq][DQ]));
		  			
				mcSHOW_DBG_MSG(("p->pSavetimeData->u1Txwin_center_Save[%d][%d][%d][%d]=%d;\n",ch,rank,DDRfreq,DQ,p->pSavetimeData->u1Txwin_center_Save[ch][rank][DDRfreq][DQ]));
 				mcSHOW_DBG_MSG(("p->pSavetimeData->u1Txfirst_pass_Save[%d][%d][%d][%d]=%d;\n",ch,rank,DDRfreq,DQ,p->pSavetimeData->u1Txfirst_pass_Save[ch][rank][DDRfreq][DQ]));
				mcSHOW_DBG_MSG(("p->pSavetimeData->u1Txlast_pass_Save[%d][%d][%d][%d]=%d;\n",ch,rank,DDRfreq,DQ,p->pSavetimeData->u1Txlast_pass_Save[ch][rank][DDRfreq][DQ]));
        			}

				mcSHOW_DBG_MSG(("p->pSavetimeData->u1CBTVref_Save[%d][%d][%d]=%d;\n",ch,rank,DDRfreq,p->pSavetimeData->u1CBTVref_Save[ch][rank][DDRfreq]));
				mcSHOW_DBG_MSG(("p->pSavetimeData->u1CBTClkDelay_Save[%d][%d][%d]=%d;\n",ch,rank,DDRfreq,p->pSavetimeData->u1CBTClkDelay_Save[ch][rank][DDRfreq]));
				mcSHOW_DBG_MSG(("p->pSavetimeData->u1CBTCmdDelay_Save[%d][%d][%d]=%d;\n",ch,rank,DDRfreq,p->pSavetimeData->u1CBTCmdDelay_Save[ch][rank][DDRfreq]));
				mcSHOW_DBG_MSG(("p->pSavetimeData->u1CBTCsDelay_Save[%d][%d][%d]=%d;\n",ch,rank,DDRfreq,p->pSavetimeData->u1CBTCsDelay_Save[ch][rank][DDRfreq]));
				mcSHOW_DBG_MSG(("p->pSavetimeData->u1RxDatlat_Save[%d][%d][%d]=%d;\n",ch,rank,DDRfreq,p->pSavetimeData->u1RxDatlat_Save[ch][rank][DDRfreq]));
//				mcSHOW_DBG_MSG(("p->pSavetimeData->u1TxWindowPerbit_Min_FirstPass_Save[%d][%d][%d]=%d;\n",ch,rank,DDRfreq,p->pSavetimeData->u1TxWindowPerbit_Min_FirstPass_Save[ch][rank][DDRfreq]));
				mcSHOW_DBG_MSG(("p->pSavetimeData->u1TxWindowPerbitVref_Save[%d][%d][%d]=%d;\n",ch,rank,DDRfreq,p->pSavetimeData->u1TxWindowPerbitVref_Save[ch][rank][DDRfreq]));	
//				mcSHOW_DBG_MSG(("p->pSavetimeData->u1RxWinPerbit_Min_FirstPass_Save[%d][%d][%d]=%d;\n",ch,rank,DDRfreq,p->pSavetimeData->u1RxWinPerbit_Min_FirstPass_Save[ch][rank][DDRfreq]));
//				mcSHOW_DBG_MSG(("p->pSavetimeData->u1RxWinPerbit_Min_LastPass_Save[%d][%d][%d]=%d;\n",ch,rank,DDRfreq,p->pSavetimeData->u1RxWinPerbit_Min_LastPass_Save[ch][rank][DDRfreq]));
			}
			mcSHOW_DBG_MSG(("p->pSavetimeData->u1RxWinPerbitVref_Save[%d][%d]=%d;\n",ch,DDRfreq,p->pSavetimeData->u1RxWinPerbitVref_Save[ch][DDRfreq]));
		}

	}


}
#endif

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
u32 g_dram_save_time_init_done;

void DramcSave_Time_For_Cal_Init(DRAMC_CTX_T *p)
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

        //p->pSavetimeData->u1CBTVrefoffset = CBT_VREF_OFFSET;
        //p->pSavetimeData->u1WriteLevelingoffset = WRITE_LEVELING_OFFSET;
        //p->pSavetimeData->u1GatingStartoffset = GATING_START_OFFSET;	//UI
        //p->pSavetimeData->u1GatingPass_Winoffset = GATING_PASS_WIN_OFFSET;	//UI
        //p->pSavetimeData->u1RxWinPerbitoffset = RX_WIN_PERBIT_OFFSET;
        //p->pSavetimeData->u1RxWinPerbitVrefoffset = RX_WIN_PERBIT_VREF_OFFSET;
        //p->pSavetimeData->u1TxWindowPerbitoffset = TX_WIN_PERBIT_OFFSET;
        //p->pSavetimeData->u1TxWindowPerbititVrefoffset = TX_WIN_PERBIT_VREF_OFFSET;
        //p->pSavetimeData->u1RxDatlatoffset = RX_DATLAT_OFFSET;
        //p->pSavetimeData->RXWindowTHHighSpeed = RX_WIN_HIGH_SPEED_TH;
        //p->pSavetimeData->RXWindowTHLowSpeed = RX_WIN_LOW_SPEED_TH;
        //p->pSavetimeData->TxWindowTH = TX_WIN_TH;
        p->pSavetimeData->Bypass_RDDQC[LP4_DDR1600]=1;
        p->pSavetimeData->Bypass_RDDQC[LP4_DDR2667]=1;
        p->pSavetimeData->Bypass_RDDQC[LP4_DDR3200]=0;
        p->pSavetimeData->Bypass_RXWINDOW[LP4_DDR1600]=1;
        p->pSavetimeData->Bypass_RXWINDOW[LP4_DDR2667]=1;
        p->pSavetimeData->Bypass_RXWINDOW[LP4_DDR3200]=0;
        p->pSavetimeData->Bypass_TXWINDOW[LP4_DDR1600]=1;
        p->pSavetimeData->Bypass_TXWINDOW[LP4_DDR2667]=1;
        p->pSavetimeData->Bypass_TXWINDOW[LP4_DDR3200]=0;
    }
    #if FORLOOP_TEST_SAVE_TIME_FOR_CALIBRATION_ON_ETT
        mcSHOW_DBG_MSG(("[save time for cal] dump calibration data\n"));
        vDramfirstCalibrationdump(p);
    #endif
#else
    p->pSavetimeData->femmc_Ready=0;
    for(fClr_calfail=0;fClr_calfail<PLL_FREQ_SEL_MAX;fClr_calfail++)
    {
        p->pSavetimeData->fcalfail[p->freq_sel]=0;
    }
    //p->pSavetimeData->u1CBTVrefoffset=2;       
    //p->pSavetimeData->u1WriteLevelingoffset=5;       
    //p->pSavetimeData->u1GatingStartoffset=0;  //UI
    //p->pSavetimeData->u1GatingPass_Winoffset=3;  //UI
    //p->pSavetimeData->u1RxWinPerbitoffset=5;
    //p->pSavetimeData->u1RxWinPerbitVrefoffset=4;
    //p->pSavetimeData->u1TxWindowPerbitoffset=5;
    //p->pSavetimeData->u1TxWindowPerbititVrefoffset=4;
    //p->pSavetimeData->u1RxDatlatoffset=1;
    //p->pSavetimeData->RXWindowTHHighSpeed=10;
    //p->pSavetimeData->RXWindowTHLowSpeed=100;
    //p->pSavetimeData->TxWindowTH=12;
    p->pSavetimeData->Bypass_RDDQC[LP4_DDR1600]=1;
    p->pSavetimeData->Bypass_RDDQC[LP4_DDR2667]=1;
    p->pSavetimeData->Bypass_RDDQC[LP4_DDR3200]=0;
    p->pSavetimeData->Bypass_RXWINDOW[LP4_DDR1600]=1;
    p->pSavetimeData->Bypass_RXWINDOW[LP4_DDR2667]=1;
    p->pSavetimeData->Bypass_RXWINDOW[LP4_DDR3200]=0;
    p->pSavetimeData->Bypass_TXWINDOW[LP4_DDR1600]=1;
    p->pSavetimeData->Bypass_TXWINDOW[LP4_DDR2667]=1;
    p->pSavetimeData->Bypass_TXWINDOW[LP4_DDR3200]=0;

#if 0
p->pSavetimeData->u1Gating2T_Save[0][0][4][0]=2;
p->pSavetimeData->u1Gating05T_Save[0][0][4][0]=3;
p->pSavetimeData->u1Gatingfine_tune_Save[0][0][4][0]=16;
p->pSavetimeData->u1Gatingucpass_count_Save[0][0][4][0]=1;
p->pSavetimeData->u1WriteLeveling_bypass_Save[0][0][4][0]=36;
p->pSavetimeData->u1TxCenter_min_Save[0][0][4][0]=79;
p->pSavetimeData->u1TxCenter_max_Save[0][0][4][0]=81;
p->pSavetimeData->u1Gating2T_Save[0][0][4][1]=2;
p->pSavetimeData->u1Gating05T_Save[0][0][4][1]=3;
p->pSavetimeData->u1Gatingfine_tune_Save[0][0][4][1]=16;
p->pSavetimeData->u1Gatingucpass_count_Save[0][0][4][1]=1;
p->pSavetimeData->u1WriteLeveling_bypass_Save[0][0][4][1]=34;
p->pSavetimeData->u1TxCenter_min_Save[0][0][4][1]=76;
p->pSavetimeData->u1TxCenter_max_Save[0][0][4][1]=78;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][4][0]=-18;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][4][0]=37;
p->pSavetimeData->u1Txwin_center_Save[0][0][4][0]=80;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][4][0]=67;
p->pSavetimeData->u1Txlast_pass_Save[0][0][4][0]=93;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][4][1]=-18;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][4][1]=39;
p->pSavetimeData->u1Txwin_center_Save[0][0][4][1]=81;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][4][1]=68;
p->pSavetimeData->u1Txlast_pass_Save[0][0][4][1]=95;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][4][2]=-18;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][4][2]=33;
p->pSavetimeData->u1Txwin_center_Save[0][0][4][2]=79;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][4][2]=67;
p->pSavetimeData->u1Txlast_pass_Save[0][0][4][2]=92;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][4][3]=-20;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][4][3]=37;
p->pSavetimeData->u1Txwin_center_Save[0][0][4][3]=80;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][4][3]=68;
p->pSavetimeData->u1Txlast_pass_Save[0][0][4][3]=92;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][4][4]=-18;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][4][4]=35;
p->pSavetimeData->u1Txwin_center_Save[0][0][4][4]=80;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][4][4]=68;
p->pSavetimeData->u1Txlast_pass_Save[0][0][4][4]=92;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][4][5]=-16;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][4][5]=39;
p->pSavetimeData->u1Txwin_center_Save[0][0][4][5]=81;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][4][5]=68;
p->pSavetimeData->u1Txlast_pass_Save[0][0][4][5]=94;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][4][6]=-18;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][4][6]=39;
p->pSavetimeData->u1Txwin_center_Save[0][0][4][6]=81;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][4][6]=69;
p->pSavetimeData->u1Txlast_pass_Save[0][0][4][6]=94;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][4][7]=-16;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][4][7]=41;
p->pSavetimeData->u1Txwin_center_Save[0][0][4][7]=81;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][4][7]=68;
p->pSavetimeData->u1Txlast_pass_Save[0][0][4][7]=95;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][4][8]=-18;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][4][8]=37;
p->pSavetimeData->u1Txwin_center_Save[0][0][4][8]=77;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][4][8]=65;
p->pSavetimeData->u1Txlast_pass_Save[0][0][4][8]=89;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][4][9]=-22;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][4][9]=35;
p->pSavetimeData->u1Txwin_center_Save[0][0][4][9]=77;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][4][9]=65;
p->pSavetimeData->u1Txlast_pass_Save[0][0][4][9]=90;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][4][10]=-20;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][4][10]=33;
p->pSavetimeData->u1Txwin_center_Save[0][0][4][10]=76;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][4][10]=63;
p->pSavetimeData->u1Txlast_pass_Save[0][0][4][10]=89;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][4][11]=-24;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][4][11]=35;
p->pSavetimeData->u1Txwin_center_Save[0][0][4][11]=76;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][4][11]=64;
p->pSavetimeData->u1Txlast_pass_Save[0][0][4][11]=89;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][4][12]=-16;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][4][12]=37;
p->pSavetimeData->u1Txwin_center_Save[0][0][4][12]=77;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][4][12]=65;
p->pSavetimeData->u1Txlast_pass_Save[0][0][4][12]=89;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][4][13]=-18;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][4][13]=35;
p->pSavetimeData->u1Txwin_center_Save[0][0][4][13]=78;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][4][13]=66;
p->pSavetimeData->u1Txlast_pass_Save[0][0][4][13]=90;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][4][14]=-18;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][4][14]=35;
p->pSavetimeData->u1Txwin_center_Save[0][0][4][14]=78;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][4][14]=66;
p->pSavetimeData->u1Txlast_pass_Save[0][0][4][14]=90;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][4][15]=-16;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][4][15]=37;
p->pSavetimeData->u1Txwin_center_Save[0][0][4][15]=78;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][4][15]=66;
p->pSavetimeData->u1Txlast_pass_Save[0][0][4][15]=91;
p->pSavetimeData->u1CBTVref_Save[0][0][4]=13;
p->pSavetimeData->u1CBTClkDelay_Save[0][0][4]=0;
p->pSavetimeData->u1CBTCmdDelay_Save[0][0][4]=0;
p->pSavetimeData->u1CBTCsDelay_Save[0][0][4]=1;
p->pSavetimeData->u1RxDatlat_Save[0][0][4]=11;
p->pSavetimeData->u1TxWindowPerbitVref_Save[0][0][4]=18;
p->pSavetimeData->u1Gating2T_Save[0][1][4][0]=2;
p->pSavetimeData->u1Gating05T_Save[0][1][4][0]=3;
p->pSavetimeData->u1Gatingfine_tune_Save[0][1][4][0]=12;
p->pSavetimeData->u1Gatingucpass_count_Save[0][1][4][0]=2;
p->pSavetimeData->u1WriteLeveling_bypass_Save[0][1][4][0]=36;
p->pSavetimeData->u1TxCenter_min_Save[0][1][4][0]=79;
p->pSavetimeData->u1TxCenter_max_Save[0][1][4][0]=81;
p->pSavetimeData->u1Gating2T_Save[0][1][4][1]=2;
p->pSavetimeData->u1Gating05T_Save[0][1][4][1]=3;
p->pSavetimeData->u1Gatingfine_tune_Save[0][1][4][1]=4;
p->pSavetimeData->u1Gatingucpass_count_Save[0][1][4][1]=2;
p->pSavetimeData->u1WriteLeveling_bypass_Save[0][1][4][1]=35;
p->pSavetimeData->u1TxCenter_min_Save[0][1][4][1]=75;
p->pSavetimeData->u1TxCenter_max_Save[0][1][4][1]=78;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][4][0]=-22;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][4][0]=37;
p->pSavetimeData->u1Txwin_center_Save[0][1][4][0]=80;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][4][0]=67;
p->pSavetimeData->u1Txlast_pass_Save[0][1][4][0]=93;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][4][1]=-20;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][4][1]=41;
p->pSavetimeData->u1Txwin_center_Save[0][1][4][1]=80;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][4][1]=67;
p->pSavetimeData->u1Txlast_pass_Save[0][1][4][1]=94;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][4][2]=-20;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][4][2]=37;
p->pSavetimeData->u1Txwin_center_Save[0][1][4][2]=79;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][4][2]=67;
p->pSavetimeData->u1Txlast_pass_Save[0][1][4][2]=91;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][4][3]=-18;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][4][3]=37;
p->pSavetimeData->u1Txwin_center_Save[0][1][4][3]=79;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][4][3]=67;
p->pSavetimeData->u1Txlast_pass_Save[0][1][4][3]=91;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][4][4]=-20;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][4][4]=37;
p->pSavetimeData->u1Txwin_center_Save[0][1][4][4]=79;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][4][4]=67;
p->pSavetimeData->u1Txlast_pass_Save[0][1][4][4]=91;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][4][5]=-20;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][4][5]=41;
p->pSavetimeData->u1Txwin_center_Save[0][1][4][5]=80;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][4][5]=67;
p->pSavetimeData->u1Txlast_pass_Save[0][1][4][5]=93;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][4][6]=-18;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][4][6]=39;
p->pSavetimeData->u1Txwin_center_Save[0][1][4][6]=81;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][4][6]=68;
p->pSavetimeData->u1Txlast_pass_Save[0][1][4][6]=94;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][4][7]=-18;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][4][7]=41;
p->pSavetimeData->u1Txwin_center_Save[0][1][4][7]=81;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][4][7]=68;
p->pSavetimeData->u1Txlast_pass_Save[0][1][4][7]=94;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][4][8]=-24;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][4][8]=39;
p->pSavetimeData->u1Txwin_center_Save[0][1][4][8]=77;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][4][8]=64;
p->pSavetimeData->u1Txlast_pass_Save[0][1][4][8]=90;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][4][9]=-24;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][4][9]=39;
p->pSavetimeData->u1Txwin_center_Save[0][1][4][9]=76;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][4][9]=63;
p->pSavetimeData->u1Txlast_pass_Save[0][1][4][9]=89;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][4][10]=-26;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][4][10]=37;
p->pSavetimeData->u1Txwin_center_Save[0][1][4][10]=75;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][4][10]=62;
p->pSavetimeData->u1Txlast_pass_Save[0][1][4][10]=88;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][4][11]=-26;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][4][11]=39;
p->pSavetimeData->u1Txwin_center_Save[0][1][4][11]=76;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][4][11]=63;
p->pSavetimeData->u1Txlast_pass_Save[0][1][4][11]=89;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][4][12]=-18;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][4][12]=41;
p->pSavetimeData->u1Txwin_center_Save[0][1][4][12]=77;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][4][12]=65;
p->pSavetimeData->u1Txlast_pass_Save[0][1][4][12]=89;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][4][13]=-22;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][4][13]=39;
p->pSavetimeData->u1Txwin_center_Save[0][1][4][13]=77;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][4][13]=65;
p->pSavetimeData->u1Txlast_pass_Save[0][1][4][13]=90;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][4][14]=-24;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][4][14]=41;
p->pSavetimeData->u1Txwin_center_Save[0][1][4][14]=77;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][4][14]=65;
p->pSavetimeData->u1Txlast_pass_Save[0][1][4][14]=90;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][4][15]=-20;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][4][15]=41;
p->pSavetimeData->u1Txwin_center_Save[0][1][4][15]=78;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][4][15]=66;
p->pSavetimeData->u1Txlast_pass_Save[0][1][4][15]=91;
p->pSavetimeData->u1CBTVref_Save[0][1][4]=0;
p->pSavetimeData->u1CBTClkDelay_Save[0][1][4]=0;
p->pSavetimeData->u1CBTCmdDelay_Save[0][1][4]=0;
p->pSavetimeData->u1CBTCsDelay_Save[0][1][4]=0;
p->pSavetimeData->u1RxDatlat_Save[0][1][4]=11;
p->pSavetimeData->u1TxWindowPerbitVref_Save[0][1][4]=14;
p->pSavetimeData->u1RxWinPerbitVref_Save[0][4]=26;
p->pSavetimeData->u1Gating2T_Save[1][0][4][0]=2;
p->pSavetimeData->u1Gating05T_Save[1][0][4][0]=3;
p->pSavetimeData->u1Gatingfine_tune_Save[1][0][4][0]=12;
p->pSavetimeData->u1Gatingucpass_count_Save[1][0][4][0]=2;
p->pSavetimeData->u1WriteLeveling_bypass_Save[1][0][4][0]=30;
p->pSavetimeData->u1TxCenter_min_Save[1][0][4][0]=74;
p->pSavetimeData->u1TxCenter_max_Save[1][0][4][0]=75;
p->pSavetimeData->u1Gating2T_Save[1][0][4][1]=2;
p->pSavetimeData->u1Gating05T_Save[1][0][4][1]=3;
p->pSavetimeData->u1Gatingfine_tune_Save[1][0][4][1]=8;
p->pSavetimeData->u1Gatingucpass_count_Save[1][0][4][1]=2;
p->pSavetimeData->u1WriteLeveling_bypass_Save[1][0][4][1]=32;
p->pSavetimeData->u1TxCenter_min_Save[1][0][4][1]=74;
p->pSavetimeData->u1TxCenter_max_Save[1][0][4][1]=76;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][4][0]=-20;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][4][0]=37;
p->pSavetimeData->u1Txwin_center_Save[1][0][4][0]=75;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][4][0]=62;
p->pSavetimeData->u1Txlast_pass_Save[1][0][4][0]=88;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][4][1]=-20;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][4][1]=39;
p->pSavetimeData->u1Txwin_center_Save[1][0][4][1]=75;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][4][1]=62;
p->pSavetimeData->u1Txlast_pass_Save[1][0][4][1]=88;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][4][2]=-18;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][4][2]=35;
p->pSavetimeData->u1Txwin_center_Save[1][0][4][2]=74;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][4][2]=62;
p->pSavetimeData->u1Txlast_pass_Save[1][0][4][2]=86;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][4][3]=-20;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][4][3]=35;
p->pSavetimeData->u1Txwin_center_Save[1][0][4][3]=74;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][4][3]=62;
p->pSavetimeData->u1Txlast_pass_Save[1][0][4][3]=86;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][4][4]=-16;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][4][4]=35;
p->pSavetimeData->u1Txwin_center_Save[1][0][4][4]=74;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][4][4]=62;
p->pSavetimeData->u1Txlast_pass_Save[1][0][4][4]=86;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][4][5]=-16;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][4][5]=35;
p->pSavetimeData->u1Txwin_center_Save[1][0][4][5]=75;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][4][5]=62;
p->pSavetimeData->u1Txlast_pass_Save[1][0][4][5]=88;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][4][6]=-16;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][4][6]=37;
p->pSavetimeData->u1Txwin_center_Save[1][0][4][6]=74;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][4][6]=62;
p->pSavetimeData->u1Txlast_pass_Save[1][0][4][6]=87;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][4][7]=-16;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][4][7]=37;
p->pSavetimeData->u1Txwin_center_Save[1][0][4][7]=75;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][4][7]=62;
p->pSavetimeData->u1Txlast_pass_Save[1][0][4][7]=88;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][4][8]=-18;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][4][8]=35;
p->pSavetimeData->u1Txwin_center_Save[1][0][4][8]=75;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][4][8]=63;
p->pSavetimeData->u1Txlast_pass_Save[1][0][4][8]=88;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][4][9]=-24;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][4][9]=33;
p->pSavetimeData->u1Txwin_center_Save[1][0][4][9]=74;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][4][9]=62;
p->pSavetimeData->u1Txlast_pass_Save[1][0][4][9]=87;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][4][10]=-20;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][4][10]=35;
p->pSavetimeData->u1Txwin_center_Save[1][0][4][10]=74;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][4][10]=62;
p->pSavetimeData->u1Txlast_pass_Save[1][0][4][10]=87;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][4][11]=-22;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][4][11]=35;
p->pSavetimeData->u1Txwin_center_Save[1][0][4][11]=74;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][4][11]=62;
p->pSavetimeData->u1Txlast_pass_Save[1][0][4][11]=87;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][4][12]=-18;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][4][12]=39;
p->pSavetimeData->u1Txwin_center_Save[1][0][4][12]=75;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][4][12]=63;
p->pSavetimeData->u1Txlast_pass_Save[1][0][4][12]=87;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][4][13]=-18;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][4][13]=37;
p->pSavetimeData->u1Txwin_center_Save[1][0][4][13]=76;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][4][13]=64;
p->pSavetimeData->u1Txlast_pass_Save[1][0][4][13]=88;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][4][14]=-18;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][4][14]=37;
p->pSavetimeData->u1Txwin_center_Save[1][0][4][14]=76;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][4][14]=64;
p->pSavetimeData->u1Txlast_pass_Save[1][0][4][14]=88;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][4][15]=-16;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][4][15]=39;
p->pSavetimeData->u1Txwin_center_Save[1][0][4][15]=76;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][4][15]=64;
p->pSavetimeData->u1Txlast_pass_Save[1][0][4][15]=89;
p->pSavetimeData->u1CBTVref_Save[1][0][4]=13;
p->pSavetimeData->u1CBTClkDelay_Save[1][0][4]=0;
p->pSavetimeData->u1CBTCmdDelay_Save[1][0][4]=0;
p->pSavetimeData->u1CBTCsDelay_Save[1][0][4]=2;
p->pSavetimeData->u1RxDatlat_Save[1][0][4]=11;
p->pSavetimeData->u1TxWindowPerbitVref_Save[1][0][4]=16;
p->pSavetimeData->u1Gating2T_Save[1][1][4][0]=2;
p->pSavetimeData->u1Gating05T_Save[1][1][4][0]=3;
p->pSavetimeData->u1Gatingfine_tune_Save[1][1][4][0]=12;
p->pSavetimeData->u1Gatingucpass_count_Save[1][1][4][0]=1;
p->pSavetimeData->u1WriteLeveling_bypass_Save[1][1][4][0]=31;
p->pSavetimeData->u1TxCenter_min_Save[1][1][4][0]=74;
p->pSavetimeData->u1TxCenter_max_Save[1][1][4][0]=76;
p->pSavetimeData->u1Gating2T_Save[1][1][4][1]=2;
p->pSavetimeData->u1Gating05T_Save[1][1][4][1]=3;
p->pSavetimeData->u1Gatingfine_tune_Save[1][1][4][1]=4;
p->pSavetimeData->u1Gatingucpass_count_Save[1][1][4][1]=1;
p->pSavetimeData->u1WriteLeveling_bypass_Save[1][1][4][1]=32;
p->pSavetimeData->u1TxCenter_min_Save[1][1][4][1]=73;
p->pSavetimeData->u1TxCenter_max_Save[1][1][4][1]=75;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][4][0]=-20;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][4][0]=39;
p->pSavetimeData->u1Txwin_center_Save[1][1][4][0]=75;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][4][0]=62;
p->pSavetimeData->u1Txlast_pass_Save[1][1][4][0]=89;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][4][1]=-18;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][4][1]=41;
p->pSavetimeData->u1Txwin_center_Save[1][1][4][1]=75;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][4][1]=62;
p->pSavetimeData->u1Txlast_pass_Save[1][1][4][1]=89;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][4][2]=-20;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][4][2]=37;
p->pSavetimeData->u1Txwin_center_Save[1][1][4][2]=74;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][4][2]=62;
p->pSavetimeData->u1Txlast_pass_Save[1][1][4][2]=87;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][4][3]=-16;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][4][3]=37;
p->pSavetimeData->u1Txwin_center_Save[1][1][4][3]=74;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][4][3]=62;
p->pSavetimeData->u1Txlast_pass_Save[1][1][4][3]=87;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][4][4]=-18;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][4][4]=39;
p->pSavetimeData->u1Txwin_center_Save[1][1][4][4]=74;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][4][4]=62;
p->pSavetimeData->u1Txlast_pass_Save[1][1][4][4]=87;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][4][5]=-18;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][4][5]=39;
p->pSavetimeData->u1Txwin_center_Save[1][1][4][5]=75;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][4][5]=62;
p->pSavetimeData->u1Txlast_pass_Save[1][1][4][5]=89;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][4][6]=-16;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][4][6]=39;
p->pSavetimeData->u1Txwin_center_Save[1][1][4][6]=75;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][4][6]=62;
p->pSavetimeData->u1Txlast_pass_Save[1][1][4][6]=88;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][4][7]=-16;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][4][7]=41;
p->pSavetimeData->u1Txwin_center_Save[1][1][4][7]=76;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][4][7]=63;
p->pSavetimeData->u1Txlast_pass_Save[1][1][4][7]=89;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][4][8]=-22;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][4][8]=37;
p->pSavetimeData->u1Txwin_center_Save[1][1][4][8]=73;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][4][8]=61;
p->pSavetimeData->u1Txlast_pass_Save[1][1][4][8]=86;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][4][9]=-24;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][4][9]=35;
p->pSavetimeData->u1Txwin_center_Save[1][1][4][9]=73;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][4][9]=60;
p->pSavetimeData->u1Txlast_pass_Save[1][1][4][9]=86;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][4][10]=-22;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][4][10]=37;
p->pSavetimeData->u1Txwin_center_Save[1][1][4][10]=73;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][4][10]=60;
p->pSavetimeData->u1Txlast_pass_Save[1][1][4][10]=86;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][4][11]=-24;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][4][11]=39;
p->pSavetimeData->u1Txwin_center_Save[1][1][4][11]=73;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][4][11]=60;
p->pSavetimeData->u1Txlast_pass_Save[1][1][4][11]=86;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][4][12]=-18;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][4][12]=41;
p->pSavetimeData->u1Txwin_center_Save[1][1][4][12]=74;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][4][12]=62;
p->pSavetimeData->u1Txlast_pass_Save[1][1][4][12]=86;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][4][13]=-22;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][4][13]=39;
p->pSavetimeData->u1Txwin_center_Save[1][1][4][13]=74;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][4][13]=62;
p->pSavetimeData->u1Txlast_pass_Save[1][1][4][13]=87;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][4][14]=-20;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][4][14]=41;
p->pSavetimeData->u1Txwin_center_Save[1][1][4][14]=74;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][4][14]=62;
p->pSavetimeData->u1Txlast_pass_Save[1][1][4][14]=87;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][4][15]=-22;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][4][15]=41;
p->pSavetimeData->u1Txwin_center_Save[1][1][4][15]=75;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][4][15]=63;
p->pSavetimeData->u1Txlast_pass_Save[1][1][4][15]=87;
p->pSavetimeData->u1CBTVref_Save[1][1][4]=0;
p->pSavetimeData->u1CBTClkDelay_Save[1][1][4]=0;
p->pSavetimeData->u1CBTCmdDelay_Save[1][1][4]=0;
p->pSavetimeData->u1CBTCsDelay_Save[1][1][4]=0;
p->pSavetimeData->u1RxDatlat_Save[1][1][4]=11;
p->pSavetimeData->u1TxWindowPerbitVref_Save[1][1][4]=18;
p->pSavetimeData->u1RxWinPerbitVref_Save[1][4]=26;
p->pSavetimeData->u1Gating2T_Save[0][0][2][0]=3;
p->pSavetimeData->u1Gating05T_Save[0][0][2][0]=3;
p->pSavetimeData->u1Gatingfine_tune_Save[0][0][2][0]=8;
p->pSavetimeData->u1Gatingucpass_count_Save[0][0][2][0]=2;
p->pSavetimeData->u1WriteLeveling_bypass_Save[0][0][2][0]=42;
p->pSavetimeData->u1TxCenter_min_Save[0][0][2][0]=103;
p->pSavetimeData->u1TxCenter_max_Save[0][0][2][0]=106;
p->pSavetimeData->u1Gating2T_Save[0][0][2][1]=3;
p->pSavetimeData->u1Gating05T_Save[0][0][2][1]=3;
p->pSavetimeData->u1Gatingfine_tune_Save[0][0][2][1]=4;
p->pSavetimeData->u1Gatingucpass_count_Save[0][0][2][1]=1;
p->pSavetimeData->u1WriteLeveling_bypass_Save[0][0][2][1]=39;
p->pSavetimeData->u1TxCenter_min_Save[0][0][2][1]=98;
p->pSavetimeData->u1TxCenter_max_Save[0][0][2][1]=102;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][2][0]=3;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][2][0]=42;
p->pSavetimeData->u1Txwin_center_Save[0][0][2][0]=104;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][2][0]=93;
p->pSavetimeData->u1Txlast_pass_Save[0][0][2][0]=116;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][2][1]=7;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][2][1]=45;
p->pSavetimeData->u1Txwin_center_Save[0][0][2][1]=106;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][2][1]=94;
p->pSavetimeData->u1Txlast_pass_Save[0][0][2][1]=118;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][2][2]=2;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][2][2]=40;
p->pSavetimeData->u1Txwin_center_Save[0][0][2][2]=103;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][2][2]=92;
p->pSavetimeData->u1Txlast_pass_Save[0][0][2][2]=115;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][2][3]=5;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][2][3]=41;
p->pSavetimeData->u1Txwin_center_Save[0][0][2][3]=104;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][2][3]=93;
p->pSavetimeData->u1Txlast_pass_Save[0][0][2][3]=116;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][2][4]=5;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][2][4]=39;
p->pSavetimeData->u1Txwin_center_Save[0][0][2][4]=105;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][2][4]=93;
p->pSavetimeData->u1Txlast_pass_Save[0][0][2][4]=117;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][2][5]=7;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][2][5]=46;
p->pSavetimeData->u1Txwin_center_Save[0][0][2][5]=106;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][2][5]=94;
p->pSavetimeData->u1Txlast_pass_Save[0][0][2][5]=118;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][2][6]=6;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][2][6]=44;
p->pSavetimeData->u1Txwin_center_Save[0][0][2][6]=106;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][2][6]=94;
p->pSavetimeData->u1Txlast_pass_Save[0][0][2][6]=118;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][2][7]=8;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][2][7]=47;
p->pSavetimeData->u1Txwin_center_Save[0][0][2][7]=106;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][2][7]=95;
p->pSavetimeData->u1Txlast_pass_Save[0][0][2][7]=117;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][2][8]=3;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][2][8]=39;
p->pSavetimeData->u1Txwin_center_Save[0][0][2][8]=100;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][2][8]=88;
p->pSavetimeData->u1Txlast_pass_Save[0][0][2][8]=112;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][2][9]=0;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][2][9]=40;
p->pSavetimeData->u1Txwin_center_Save[0][0][2][9]=100;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][2][9]=89;
p->pSavetimeData->u1Txlast_pass_Save[0][0][2][9]=111;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][2][10]=-1;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][2][10]=38;
p->pSavetimeData->u1Txwin_center_Save[0][0][2][10]=98;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][2][10]=86;
p->pSavetimeData->u1Txlast_pass_Save[0][0][2][10]=111;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][2][11]=0;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][2][11]=40;
p->pSavetimeData->u1Txwin_center_Save[0][0][2][11]=99;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][2][11]=87;
p->pSavetimeData->u1Txlast_pass_Save[0][0][2][11]=112;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][2][12]=5;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][2][12]=44;
p->pSavetimeData->u1Txwin_center_Save[0][0][2][12]=101;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][2][12]=89;
p->pSavetimeData->u1Txlast_pass_Save[0][0][2][12]=113;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][2][13]=5;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][2][13]=43;
p->pSavetimeData->u1Txwin_center_Save[0][0][2][13]=101;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][2][13]=89;
p->pSavetimeData->u1Txlast_pass_Save[0][0][2][13]=113;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][2][14]=1;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][2][14]=42;
p->pSavetimeData->u1Txwin_center_Save[0][0][2][14]=101;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][2][14]=89;
p->pSavetimeData->u1Txlast_pass_Save[0][0][2][14]=113;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][2][15]=4;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][2][15]=44;
p->pSavetimeData->u1Txwin_center_Save[0][0][2][15]=102;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][2][15]=89;
p->pSavetimeData->u1Txlast_pass_Save[0][0][2][15]=115;
p->pSavetimeData->u1CBTVref_Save[0][0][2]=20;
p->pSavetimeData->u1CBTClkDelay_Save[0][0][2]=0;
p->pSavetimeData->u1CBTCmdDelay_Save[0][0][2]=4;
p->pSavetimeData->u1CBTCsDelay_Save[0][0][2]=4;
p->pSavetimeData->u1RxDatlat_Save[0][0][2]=14;
p->pSavetimeData->u1TxWindowPerbitVref_Save[0][0][2]=0;
p->pSavetimeData->u1Gating2T_Save[0][1][2][0]=3;
p->pSavetimeData->u1Gating05T_Save[0][1][2][0]=3;
p->pSavetimeData->u1Gatingfine_tune_Save[0][1][2][0]=4;
p->pSavetimeData->u1Gatingucpass_count_Save[0][1][2][0]=2;
p->pSavetimeData->u1WriteLeveling_bypass_Save[0][1][2][0]=43;
p->pSavetimeData->u1TxCenter_min_Save[0][1][2][0]=103;
p->pSavetimeData->u1TxCenter_max_Save[0][1][2][0]=105;
p->pSavetimeData->u1Gating2T_Save[0][1][2][1]=3;
p->pSavetimeData->u1Gating05T_Save[0][1][2][1]=2;
p->pSavetimeData->u1Gatingfine_tune_Save[0][1][2][1]=16;
p->pSavetimeData->u1Gatingucpass_count_Save[0][1][2][1]=2;
p->pSavetimeData->u1WriteLeveling_bypass_Save[0][1][2][1]=39;
p->pSavetimeData->u1TxCenter_min_Save[0][1][2][1]=94;
p->pSavetimeData->u1TxCenter_max_Save[0][1][2][1]=96;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][2][0]=1;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][2][0]=42;
p->pSavetimeData->u1Txwin_center_Save[0][1][2][0]=103;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][2][0]=91;
p->pSavetimeData->u1Txlast_pass_Save[0][1][2][0]=116;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][2][1]=5;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][2][1]=44;
p->pSavetimeData->u1Txwin_center_Save[0][1][2][1]=105;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][2][1]=93;
p->pSavetimeData->u1Txlast_pass_Save[0][1][2][1]=117;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][2][2]=3;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][2][2]=41;
p->pSavetimeData->u1Txwin_center_Save[0][1][2][2]=104;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][2][2]=93;
p->pSavetimeData->u1Txlast_pass_Save[0][1][2][2]=116;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][2][3]=5;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][2][3]=41;
p->pSavetimeData->u1Txwin_center_Save[0][1][2][3]=104;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][2][3]=92;
p->pSavetimeData->u1Txlast_pass_Save[0][1][2][3]=116;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][2][4]=5;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][2][4]=40;
p->pSavetimeData->u1Txwin_center_Save[0][1][2][4]=104;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][2][4]=93;
p->pSavetimeData->u1Txlast_pass_Save[0][1][2][4]=116;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][2][5]=8;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][2][5]=44;
p->pSavetimeData->u1Txwin_center_Save[0][1][2][5]=104;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][2][5]=91;
p->pSavetimeData->u1Txlast_pass_Save[0][1][2][5]=117;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][2][6]=6;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][2][6]=44;
p->pSavetimeData->u1Txwin_center_Save[0][1][2][6]=104;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][2][6]=92;
p->pSavetimeData->u1Txlast_pass_Save[0][1][2][6]=116;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][2][7]=7;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][2][7]=46;
p->pSavetimeData->u1Txwin_center_Save[0][1][2][7]=104;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][2][7]=91;
p->pSavetimeData->u1Txlast_pass_Save[0][1][2][7]=117;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][2][8]=3;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][2][8]=40;
p->pSavetimeData->u1Txwin_center_Save[0][1][2][8]=95;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][2][8]=84;
p->pSavetimeData->u1Txlast_pass_Save[0][1][2][8]=107;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][2][9]=0;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][2][9]=38;
p->pSavetimeData->u1Txwin_center_Save[0][1][2][9]=94;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][2][9]=84;
p->pSavetimeData->u1Txlast_pass_Save[0][1][2][9]=105;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][2][10]=0;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][2][10]=38;
p->pSavetimeData->u1Txwin_center_Save[0][1][2][10]=94;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][2][10]=83;
p->pSavetimeData->u1Txlast_pass_Save[0][1][2][10]=106;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][2][11]=0;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][2][11]=40;
p->pSavetimeData->u1Txwin_center_Save[0][1][2][11]=96;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][2][11]=84;
p->pSavetimeData->u1Txlast_pass_Save[0][1][2][11]=108;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][2][12]=7;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][2][12]=43;
p->pSavetimeData->u1Txwin_center_Save[0][1][2][12]=96;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][2][12]=85;
p->pSavetimeData->u1Txlast_pass_Save[0][1][2][12]=107;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][2][13]=5;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][2][13]=42;
p->pSavetimeData->u1Txwin_center_Save[0][1][2][13]=96;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][2][13]=85;
p->pSavetimeData->u1Txlast_pass_Save[0][1][2][13]=107;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][2][14]=3;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][2][14]=40;
p->pSavetimeData->u1Txwin_center_Save[0][1][2][14]=95;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][2][14]=84;
p->pSavetimeData->u1Txlast_pass_Save[0][1][2][14]=107;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][2][15]=5;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][2][15]=45;
p->pSavetimeData->u1Txwin_center_Save[0][1][2][15]=95;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][2][15]=84;
p->pSavetimeData->u1Txlast_pass_Save[0][1][2][15]=107;
p->pSavetimeData->u1CBTVref_Save[0][1][2]=0;
p->pSavetimeData->u1CBTClkDelay_Save[0][1][2]=0;
p->pSavetimeData->u1CBTCmdDelay_Save[0][1][2]=0;
p->pSavetimeData->u1CBTCsDelay_Save[0][1][2]=0;
p->pSavetimeData->u1RxDatlat_Save[0][1][2]=14;
p->pSavetimeData->u1TxWindowPerbitVref_Save[0][1][2]=0;
p->pSavetimeData->u1RxWinPerbitVref_Save[0][2]=0;
p->pSavetimeData->u1Gating2T_Save[1][0][2][0]=3;
p->pSavetimeData->u1Gating05T_Save[1][0][2][0]=3;
p->pSavetimeData->u1Gatingfine_tune_Save[1][0][2][0]=4;
p->pSavetimeData->u1Gatingucpass_count_Save[1][0][2][0]=2;
p->pSavetimeData->u1WriteLeveling_bypass_Save[1][0][2][0]=36;
p->pSavetimeData->u1TxCenter_min_Save[1][0][2][0]=96;
p->pSavetimeData->u1TxCenter_max_Save[1][0][2][0]=99;
p->pSavetimeData->u1Gating2T_Save[1][0][2][1]=3;
p->pSavetimeData->u1Gating05T_Save[1][0][2][1]=2;
p->pSavetimeData->u1Gatingfine_tune_Save[1][0][2][1]=28;
p->pSavetimeData->u1Gatingucpass_count_Save[1][0][2][1]=2;
p->pSavetimeData->u1WriteLeveling_bypass_Save[1][0][2][1]=39;
p->pSavetimeData->u1TxCenter_min_Save[1][0][2][1]=99;
p->pSavetimeData->u1TxCenter_max_Save[1][0][2][1]=103;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][2][0]=5;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][2][0]=42;
p->pSavetimeData->u1Txwin_center_Save[1][0][2][0]=99;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][2][0]=86;
p->pSavetimeData->u1Txlast_pass_Save[1][0][2][0]=112;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][2][1]=7;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][2][1]=45;
p->pSavetimeData->u1Txwin_center_Save[1][0][2][1]=99;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][2][1]=87;
p->pSavetimeData->u1Txlast_pass_Save[1][0][2][1]=112;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][2][2]=3;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][2][2]=40;
p->pSavetimeData->u1Txwin_center_Save[1][0][2][2]=97;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][2][2]=85;
p->pSavetimeData->u1Txlast_pass_Save[1][0][2][2]=109;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][2][3]=4;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][2][3]=42;
p->pSavetimeData->u1Txwin_center_Save[1][0][2][3]=96;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][2][3]=85;
p->pSavetimeData->u1Txlast_pass_Save[1][0][2][3]=108;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][2][4]=6;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][2][4]=41;
p->pSavetimeData->u1Txwin_center_Save[1][0][2][4]=97;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][2][4]=86;
p->pSavetimeData->u1Txlast_pass_Save[1][0][2][4]=109;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][2][5]=7;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][2][5]=44;
p->pSavetimeData->u1Txwin_center_Save[1][0][2][5]=99;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][2][5]=87;
p->pSavetimeData->u1Txlast_pass_Save[1][0][2][5]=111;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][2][6]=6;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][2][6]=45;
p->pSavetimeData->u1Txwin_center_Save[1][0][2][6]=99;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][2][6]=87;
p->pSavetimeData->u1Txlast_pass_Save[1][0][2][6]=112;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][2][7]=8;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][2][7]=46;
p->pSavetimeData->u1Txwin_center_Save[1][0][2][7]=98;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][2][7]=86;
p->pSavetimeData->u1Txlast_pass_Save[1][0][2][7]=111;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][2][8]=1;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][2][8]=41;
p->pSavetimeData->u1Txwin_center_Save[1][0][2][8]=101;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][2][8]=89;
p->pSavetimeData->u1Txlast_pass_Save[1][0][2][8]=113;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][2][9]=-2;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][2][9]=38;
p->pSavetimeData->u1Txwin_center_Save[1][0][2][9]=99;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][2][9]=87;
p->pSavetimeData->u1Txlast_pass_Save[1][0][2][9]=111;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][2][10]=-1;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][2][10]=39;
p->pSavetimeData->u1Txwin_center_Save[1][0][2][10]=100;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][2][10]=88;
p->pSavetimeData->u1Txlast_pass_Save[1][0][2][10]=113;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][2][11]=2;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][2][11]=42;
p->pSavetimeData->u1Txwin_center_Save[1][0][2][11]=101;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][2][11]=89;
p->pSavetimeData->u1Txlast_pass_Save[1][0][2][11]=113;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][2][12]=5;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][2][12]=42;
p->pSavetimeData->u1Txwin_center_Save[1][0][2][12]=102;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][2][12]=91;
p->pSavetimeData->u1Txlast_pass_Save[1][0][2][12]=114;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][2][13]=5;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][2][13]=43;
p->pSavetimeData->u1Txwin_center_Save[1][0][2][13]=103;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][2][13]=92;
p->pSavetimeData->u1Txlast_pass_Save[1][0][2][13]=115;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][2][14]=4;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][2][14]=42;
p->pSavetimeData->u1Txwin_center_Save[1][0][2][14]=102;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][2][14]=91;
p->pSavetimeData->u1Txlast_pass_Save[1][0][2][14]=114;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][2][15]=6;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][2][15]=43;
p->pSavetimeData->u1Txwin_center_Save[1][0][2][15]=103;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][2][15]=92;
p->pSavetimeData->u1Txlast_pass_Save[1][0][2][15]=115;
p->pSavetimeData->u1CBTVref_Save[1][0][2]=20;
p->pSavetimeData->u1CBTClkDelay_Save[1][0][2]=0;
p->pSavetimeData->u1CBTCmdDelay_Save[1][0][2]=5;
p->pSavetimeData->u1CBTCsDelay_Save[1][0][2]=5;
p->pSavetimeData->u1RxDatlat_Save[1][0][2]=14;
p->pSavetimeData->u1TxWindowPerbitVref_Save[1][0][2]=0;
p->pSavetimeData->u1Gating2T_Save[1][1][2][0]=3;
p->pSavetimeData->u1Gating05T_Save[1][1][2][0]=3;
p->pSavetimeData->u1Gatingfine_tune_Save[1][1][2][0]=0;
p->pSavetimeData->u1Gatingucpass_count_Save[1][1][2][0]=2;
p->pSavetimeData->u1WriteLeveling_bypass_Save[1][1][2][0]=36;
p->pSavetimeData->u1TxCenter_min_Save[1][1][2][0]=95;
p->pSavetimeData->u1TxCenter_max_Save[1][1][2][0]=98;
p->pSavetimeData->u1Gating2T_Save[1][1][2][1]=3;
p->pSavetimeData->u1Gating05T_Save[1][1][2][1]=2;
p->pSavetimeData->u1Gatingfine_tune_Save[1][1][2][1]=16;
p->pSavetimeData->u1Gatingucpass_count_Save[1][1][2][1]=2;
p->pSavetimeData->u1WriteLeveling_bypass_Save[1][1][2][1]=38;
p->pSavetimeData->u1TxCenter_min_Save[1][1][2][1]=94;
p->pSavetimeData->u1TxCenter_max_Save[1][1][2][1]=96;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][2][0]=7;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][2][0]=38;
p->pSavetimeData->u1Txwin_center_Save[1][1][2][0]=95;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][2][0]=85;
p->pSavetimeData->u1Txlast_pass_Save[1][1][2][0]=106;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][2][1]=9;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][2][1]=41;
p->pSavetimeData->u1Txwin_center_Save[1][1][2][1]=96;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][2][1]=86;
p->pSavetimeData->u1Txlast_pass_Save[1][1][2][1]=107;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][2][2]=6;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][2][2]=36;
p->pSavetimeData->u1Txwin_center_Save[1][1][2][2]=96;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][2][2]=86;
p->pSavetimeData->u1Txlast_pass_Save[1][1][2][2]=106;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][2][3]=9;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][2][3]=35;
p->pSavetimeData->u1Txwin_center_Save[1][1][2][3]=96;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][2][3]=86;
p->pSavetimeData->u1Txlast_pass_Save[1][1][2][3]=107;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][2][4]=7;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][2][4]=38;
p->pSavetimeData->u1Txwin_center_Save[1][1][2][4]=96;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][2][4]=86;
p->pSavetimeData->u1Txlast_pass_Save[1][1][2][4]=107;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][2][5]=10;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][2][5]=37;
p->pSavetimeData->u1Txwin_center_Save[1][1][2][5]=96;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][2][5]=86;
p->pSavetimeData->u1Txlast_pass_Save[1][1][2][5]=107;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][2][6]=11;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][2][6]=38;
p->pSavetimeData->u1Txwin_center_Save[1][1][2][6]=95;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][2][6]=85;
p->pSavetimeData->u1Txlast_pass_Save[1][1][2][6]=106;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][2][7]=12;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][2][7]=39;
p->pSavetimeData->u1Txwin_center_Save[1][1][2][7]=98;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][2][7]=87;
p->pSavetimeData->u1Txlast_pass_Save[1][1][2][7]=109;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][2][8]=8;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][2][8]=36;
p->pSavetimeData->u1Txwin_center_Save[1][1][2][8]=95;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][2][8]=86;
p->pSavetimeData->u1Txlast_pass_Save[1][1][2][8]=104;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][2][9]=5;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][2][9]=34;
p->pSavetimeData->u1Txwin_center_Save[1][1][2][9]=96;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][2][9]=88;
p->pSavetimeData->u1Txlast_pass_Save[1][1][2][9]=104;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][2][10]=5;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][2][10]=36;
p->pSavetimeData->u1Txwin_center_Save[1][1][2][10]=95;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][2][10]=86;
p->pSavetimeData->u1Txlast_pass_Save[1][1][2][10]=105;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][2][11]=7;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][2][11]=36;
p->pSavetimeData->u1Txwin_center_Save[1][1][2][11]=95;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][2][11]=86;
p->pSavetimeData->u1Txlast_pass_Save[1][1][2][11]=105;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][2][12]=10;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][2][12]=39;
p->pSavetimeData->u1Txwin_center_Save[1][1][2][12]=95;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][2][12]=87;
p->pSavetimeData->u1Txlast_pass_Save[1][1][2][12]=103;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][2][13]=10;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][2][13]=38;
p->pSavetimeData->u1Txwin_center_Save[1][1][2][13]=94;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][2][13]=86;
p->pSavetimeData->u1Txlast_pass_Save[1][1][2][13]=103;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][2][14]=8;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][2][14]=39;
p->pSavetimeData->u1Txwin_center_Save[1][1][2][14]=95;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][2][14]=86;
p->pSavetimeData->u1Txlast_pass_Save[1][1][2][14]=104;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][2][15]=10;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][2][15]=40;
p->pSavetimeData->u1Txwin_center_Save[1][1][2][15]=96;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][2][15]=86;
p->pSavetimeData->u1Txlast_pass_Save[1][1][2][15]=106;
p->pSavetimeData->u1CBTVref_Save[1][1][2]=0;
p->pSavetimeData->u1CBTClkDelay_Save[1][1][2]=0;
p->pSavetimeData->u1CBTCmdDelay_Save[1][1][2]=0;
p->pSavetimeData->u1CBTCsDelay_Save[1][1][2]=0;
p->pSavetimeData->u1RxDatlat_Save[1][1][2]=14;
p->pSavetimeData->u1TxWindowPerbitVref_Save[1][1][2]=0;
p->pSavetimeData->u1RxWinPerbitVref_Save[1][2]=0;
p->pSavetimeData->u1Gating2T_Save[0][0][1][0]=4;
p->pSavetimeData->u1Gating05T_Save[0][0][1][0]=1;
p->pSavetimeData->u1Gatingfine_tune_Save[0][0][1][0]=8;
p->pSavetimeData->u1Gatingucpass_count_Save[0][0][1][0]=2;
p->pSavetimeData->u1WriteLeveling_bypass_Save[0][0][1][0]=47;
p->pSavetimeData->u1TxCenter_min_Save[0][0][1][0]=116;
p->pSavetimeData->u1TxCenter_max_Save[0][0][1][0]=119;
p->pSavetimeData->u1Gating2T_Save[0][0][1][1]=4;
p->pSavetimeData->u1Gating05T_Save[0][0][1][1]=1;
p->pSavetimeData->u1Gatingfine_tune_Save[0][0][1][1]=0;
p->pSavetimeData->u1Gatingucpass_count_Save[0][0][1][1]=2;
p->pSavetimeData->u1WriteLeveling_bypass_Save[0][0][1][1]=41;
p->pSavetimeData->u1TxCenter_min_Save[0][0][1][1]=108;
p->pSavetimeData->u1TxCenter_max_Save[0][0][1][1]=112;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][1][0]=9;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][1][0]=38;
p->pSavetimeData->u1Txwin_center_Save[0][0][1][0]=116;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][1][0]=106;
p->pSavetimeData->u1Txlast_pass_Save[0][0][1][0]=127;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][1][1]=13;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][1][1]=42;
p->pSavetimeData->u1Txwin_center_Save[0][0][1][1]=119;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][1][1]=109;
p->pSavetimeData->u1Txlast_pass_Save[0][0][1][1]=130;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][1][2]=9;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][1][2]=35;
p->pSavetimeData->u1Txwin_center_Save[0][0][1][2]=117;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][1][2]=107;
p->pSavetimeData->u1Txlast_pass_Save[0][0][1][2]=127;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][1][3]=12;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][1][3]=37;
p->pSavetimeData->u1Txwin_center_Save[0][0][1][3]=117;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][1][3]=106;
p->pSavetimeData->u1Txlast_pass_Save[0][0][1][3]=129;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][1][4]=9;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][1][4]=38;
p->pSavetimeData->u1Txwin_center_Save[0][0][1][4]=118;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][1][4]=107;
p->pSavetimeData->u1Txlast_pass_Save[0][0][1][4]=129;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][1][5]=11;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][1][5]=42;
p->pSavetimeData->u1Txwin_center_Save[0][0][1][5]=118;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][1][5]=107;
p->pSavetimeData->u1Txlast_pass_Save[0][0][1][5]=130;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][1][6]=13;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][1][6]=40;
p->pSavetimeData->u1Txwin_center_Save[0][0][1][6]=119;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][1][6]=109;
p->pSavetimeData->u1Txlast_pass_Save[0][0][1][6]=130;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][1][7]=13;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][1][7]=42;
p->pSavetimeData->u1Txwin_center_Save[0][0][1][7]=119;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][1][7]=109;
p->pSavetimeData->u1Txlast_pass_Save[0][0][1][7]=130;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][1][8]=7;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][1][8]=38;
p->pSavetimeData->u1Txwin_center_Save[0][0][1][8]=111;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][1][8]=100;
p->pSavetimeData->u1Txlast_pass_Save[0][0][1][8]=122;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][1][9]=7;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][1][9]=35;
p->pSavetimeData->u1Txwin_center_Save[0][0][1][9]=110;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][1][9]=100;
p->pSavetimeData->u1Txlast_pass_Save[0][0][1][9]=120;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][1][10]=3;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][1][10]=33;
p->pSavetimeData->u1Txwin_center_Save[0][0][1][10]=108;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][1][10]=97;
p->pSavetimeData->u1Txlast_pass_Save[0][0][1][10]=120;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][1][11]=6;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][1][11]=35;
p->pSavetimeData->u1Txwin_center_Save[0][0][1][11]=109;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][1][11]=98;
p->pSavetimeData->u1Txlast_pass_Save[0][0][1][11]=121;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][1][12]=11;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][1][12]=37;
p->pSavetimeData->u1Txwin_center_Save[0][0][1][12]=111;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][1][12]=101;
p->pSavetimeData->u1Txlast_pass_Save[0][0][1][12]=122;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][1][13]=11;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][1][13]=36;
p->pSavetimeData->u1Txwin_center_Save[0][0][1][13]=112;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][1][13]=101;
p->pSavetimeData->u1Txlast_pass_Save[0][0][1][13]=124;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][1][14]=8;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][1][14]=36;
p->pSavetimeData->u1Txwin_center_Save[0][0][1][14]=112;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][1][14]=101;
p->pSavetimeData->u1Txlast_pass_Save[0][0][1][14]=123;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][0][1][15]=11;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][0][1][15]=39;
p->pSavetimeData->u1Txwin_center_Save[0][0][1][15]=112;
p->pSavetimeData->u1Txfirst_pass_Save[0][0][1][15]=101;
p->pSavetimeData->u1Txlast_pass_Save[0][0][1][15]=124;
p->pSavetimeData->u1CBTVref_Save[0][0][1]=20;
p->pSavetimeData->u1CBTClkDelay_Save[0][0][1]=0;
p->pSavetimeData->u1CBTCmdDelay_Save[0][0][1]=2;
p->pSavetimeData->u1CBTCsDelay_Save[0][0][1]=4;
p->pSavetimeData->u1RxDatlat_Save[0][0][1]=16;
p->pSavetimeData->u1TxWindowPerbitVref_Save[0][0][1]=8;
p->pSavetimeData->u1Gating2T_Save[0][1][1][0]=4;
p->pSavetimeData->u1Gating05T_Save[0][1][1][0]=1;
p->pSavetimeData->u1Gatingfine_tune_Save[0][1][1][0]=0;
p->pSavetimeData->u1Gatingucpass_count_Save[0][1][1][0]=3;
p->pSavetimeData->u1WriteLeveling_bypass_Save[0][1][1][0]=46;
p->pSavetimeData->u1TxCenter_min_Save[0][1][1][0]=115;
p->pSavetimeData->u1TxCenter_max_Save[0][1][1][0]=117;
p->pSavetimeData->u1Gating2T_Save[0][1][1][1]=4;
p->pSavetimeData->u1Gating05T_Save[0][1][1][1]=0;
p->pSavetimeData->u1Gatingfine_tune_Save[0][1][1][1]=8;
p->pSavetimeData->u1Gatingucpass_count_Save[0][1][1][1]=2;
p->pSavetimeData->u1WriteLeveling_bypass_Save[0][1][1][1]=40;
p->pSavetimeData->u1TxCenter_min_Save[0][1][1][1]=103;
p->pSavetimeData->u1TxCenter_max_Save[0][1][1][1]=105;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][1][0]=5;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][1][0]=39;
p->pSavetimeData->u1Txwin_center_Save[0][1][1][0]=116;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][1][0]=105;
p->pSavetimeData->u1Txlast_pass_Save[0][1][1][0]=127;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][1][1]=9;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][1][1]=42;
p->pSavetimeData->u1Txwin_center_Save[0][1][1][1]=116;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][1][1]=105;
p->pSavetimeData->u1Txlast_pass_Save[0][1][1][1]=127;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][1][2]=7;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][1][2]=37;
p->pSavetimeData->u1Txwin_center_Save[0][1][1][2]=116;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][1][2]=105;
p->pSavetimeData->u1Txlast_pass_Save[0][1][1][2]=127;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][1][3]=9;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][1][3]=37;
p->pSavetimeData->u1Txwin_center_Save[0][1][1][3]=115;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][1][3]=104;
p->pSavetimeData->u1Txlast_pass_Save[0][1][1][3]=127;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][1][4]=9;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][1][4]=36;
p->pSavetimeData->u1Txwin_center_Save[0][1][1][4]=116;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][1][4]=105;
p->pSavetimeData->u1Txlast_pass_Save[0][1][1][4]=127;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][1][5]=12;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][1][5]=42;
p->pSavetimeData->u1Txwin_center_Save[0][1][1][5]=117;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][1][5]=106;
p->pSavetimeData->u1Txlast_pass_Save[0][1][1][5]=128;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][1][6]=10;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][1][6]=40;
p->pSavetimeData->u1Txwin_center_Save[0][1][1][6]=116;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][1][6]=105;
p->pSavetimeData->u1Txlast_pass_Save[0][1][1][6]=127;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][1][7]=12;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][1][7]=42;
p->pSavetimeData->u1Txwin_center_Save[0][1][1][7]=116;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][1][7]=105;
p->pSavetimeData->u1Txlast_pass_Save[0][1][1][7]=128;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][1][8]=6;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][1][8]=36;
p->pSavetimeData->u1Txwin_center_Save[0][1][1][8]=103;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][1][8]=93;
p->pSavetimeData->u1Txlast_pass_Save[0][1][1][8]=114;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][1][9]=4;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][1][9]=35;
p->pSavetimeData->u1Txwin_center_Save[0][1][1][9]=104;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][1][9]=94;
p->pSavetimeData->u1Txlast_pass_Save[0][1][1][9]=114;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][1][10]=3;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][1][10]=34;
p->pSavetimeData->u1Txwin_center_Save[0][1][1][10]=104;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][1][10]=93;
p->pSavetimeData->u1Txlast_pass_Save[0][1][1][10]=115;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][1][11]=4;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][1][11]=37;
p->pSavetimeData->u1Txwin_center_Save[0][1][1][11]=104;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][1][11]=93;
p->pSavetimeData->u1Txlast_pass_Save[0][1][1][11]=116;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][1][12]=11;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][1][12]=40;
p->pSavetimeData->u1Txwin_center_Save[0][1][1][12]=104;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][1][12]=94;
p->pSavetimeData->u1Txlast_pass_Save[0][1][1][12]=115;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][1][13]=10;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][1][13]=40;
p->pSavetimeData->u1Txwin_center_Save[0][1][1][13]=105;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][1][13]=95;
p->pSavetimeData->u1Txlast_pass_Save[0][1][1][13]=115;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][1][14]=7;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][1][14]=38;
p->pSavetimeData->u1Txwin_center_Save[0][1][1][14]=104;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][1][14]=94;
p->pSavetimeData->u1Txlast_pass_Save[0][1][1][14]=115;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[0][1][1][15]=10;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[0][1][1][15]=42;
p->pSavetimeData->u1Txwin_center_Save[0][1][1][15]=105;
p->pSavetimeData->u1Txfirst_pass_Save[0][1][1][15]=94;
p->pSavetimeData->u1Txlast_pass_Save[0][1][1][15]=116;
p->pSavetimeData->u1CBTVref_Save[0][1][1]=0;
p->pSavetimeData->u1CBTClkDelay_Save[0][1][1]=0;
p->pSavetimeData->u1CBTCmdDelay_Save[0][1][1]=0;
p->pSavetimeData->u1CBTCsDelay_Save[0][1][1]=0;
p->pSavetimeData->u1RxDatlat_Save[0][1][1]=16;
p->pSavetimeData->u1TxWindowPerbitVref_Save[0][1][1]=8;
p->pSavetimeData->u1RxWinPerbitVref_Save[0][1]=12;
p->pSavetimeData->u1Gating2T_Save[1][0][1][0]=4;
p->pSavetimeData->u1Gating05T_Save[1][0][1][0]=1;
p->pSavetimeData->u1Gatingfine_tune_Save[1][0][1][0]=4;
p->pSavetimeData->u1Gatingucpass_count_Save[1][0][1][0]=2;
p->pSavetimeData->u1WriteLeveling_bypass_Save[1][0][1][0]=37;
p->pSavetimeData->u1TxCenter_min_Save[1][0][1][0]=105;
p->pSavetimeData->u1TxCenter_max_Save[1][0][1][0]=109;
p->pSavetimeData->u1Gating2T_Save[1][0][1][1]=4;
p->pSavetimeData->u1Gating05T_Save[1][0][1][1]=0;
p->pSavetimeData->u1Gatingfine_tune_Save[1][0][1][1]=20;
p->pSavetimeData->u1Gatingucpass_count_Save[1][0][1][1]=3;
p->pSavetimeData->u1WriteLeveling_bypass_Save[1][0][1][1]=42;
p->pSavetimeData->u1TxCenter_min_Save[1][0][1][1]=109;
p->pSavetimeData->u1TxCenter_max_Save[1][0][1][1]=114;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][1][0]=11;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][1][0]=38;
p->pSavetimeData->u1Txwin_center_Save[1][0][1][0]=108;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][1][0]=96;
p->pSavetimeData->u1Txlast_pass_Save[1][0][1][0]=120;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][1][1]=14;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][1][1]=41;
p->pSavetimeData->u1Txwin_center_Save[1][0][1][1]=109;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][1][1]=98;
p->pSavetimeData->u1Txlast_pass_Save[1][0][1][1]=121;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][1][2]=9;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][1][2]=36;
p->pSavetimeData->u1Txwin_center_Save[1][0][1][2]=106;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][1][2]=94;
p->pSavetimeData->u1Txlast_pass_Save[1][0][1][2]=118;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][1][3]=13;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][1][3]=36;
p->pSavetimeData->u1Txwin_center_Save[1][0][1][3]=105;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][1][3]=95;
p->pSavetimeData->u1Txlast_pass_Save[1][0][1][3]=116;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][1][4]=11;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][1][4]=38;
p->pSavetimeData->u1Txwin_center_Save[1][0][1][4]=106;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][1][4]=95;
p->pSavetimeData->u1Txlast_pass_Save[1][0][1][4]=118;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][1][5]=15;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][1][5]=38;
p->pSavetimeData->u1Txwin_center_Save[1][0][1][5]=109;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][1][5]=98;
p->pSavetimeData->u1Txlast_pass_Save[1][0][1][5]=121;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][1][6]=15;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][1][6]=39;
p->pSavetimeData->u1Txwin_center_Save[1][0][1][6]=108;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][1][6]=97;
p->pSavetimeData->u1Txlast_pass_Save[1][0][1][6]=120;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][1][7]=16;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][1][7]=40;
p->pSavetimeData->u1Txwin_center_Save[1][0][1][7]=109;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][1][7]=97;
p->pSavetimeData->u1Txlast_pass_Save[1][0][1][7]=121;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][1][8]=9;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][1][8]=35;
p->pSavetimeData->u1Txwin_center_Save[1][0][1][8]=111;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][1][8]=100;
p->pSavetimeData->u1Txlast_pass_Save[1][0][1][8]=123;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][1][9]=6;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][1][9]=32;
p->pSavetimeData->u1Txwin_center_Save[1][0][1][9]=109;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][1][9]=99;
p->pSavetimeData->u1Txlast_pass_Save[1][0][1][9]=119;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][1][10]=5;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][1][10]=33;
p->pSavetimeData->u1Txwin_center_Save[1][0][1][10]=110;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][1][10]=99;
p->pSavetimeData->u1Txlast_pass_Save[1][0][1][10]=122;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][1][11]=7;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][1][11]=36;
p->pSavetimeData->u1Txwin_center_Save[1][0][1][11]=111;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][1][11]=100;
p->pSavetimeData->u1Txlast_pass_Save[1][0][1][11]=123;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][1][12]=11;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][1][12]=38;
p->pSavetimeData->u1Txwin_center_Save[1][0][1][12]=113;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][1][12]=102;
p->pSavetimeData->u1Txlast_pass_Save[1][0][1][12]=124;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][1][13]=12;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][1][13]=38;
p->pSavetimeData->u1Txwin_center_Save[1][0][1][13]=113;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][1][13]=102;
p->pSavetimeData->u1Txlast_pass_Save[1][0][1][13]=125;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][1][14]=8;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][1][14]=38;
p->pSavetimeData->u1Txwin_center_Save[1][0][1][14]=112;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][1][14]=101;
p->pSavetimeData->u1Txlast_pass_Save[1][0][1][14]=124;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][0][1][15]=11;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][0][1][15]=39;
p->pSavetimeData->u1Txwin_center_Save[1][0][1][15]=114;
p->pSavetimeData->u1Txfirst_pass_Save[1][0][1][15]=103;
p->pSavetimeData->u1Txlast_pass_Save[1][0][1][15]=125;
p->pSavetimeData->u1CBTVref_Save[1][0][1]=20;
p->pSavetimeData->u1CBTClkDelay_Save[1][0][1]=0;
p->pSavetimeData->u1CBTCmdDelay_Save[1][0][1]=4;
p->pSavetimeData->u1CBTCsDelay_Save[1][0][1]=5;
p->pSavetimeData->u1RxDatlat_Save[1][0][1]=16;
p->pSavetimeData->u1TxWindowPerbitVref_Save[1][0][1]=8;
p->pSavetimeData->u1Gating2T_Save[1][1][1][0]=4;
p->pSavetimeData->u1Gating05T_Save[1][1][1][0]=1;
p->pSavetimeData->u1Gatingfine_tune_Save[1][1][1][0]=0;
p->pSavetimeData->u1Gatingucpass_count_Save[1][1][1][0]=2;
p->pSavetimeData->u1WriteLeveling_bypass_Save[1][1][1][0]=37;
p->pSavetimeData->u1TxCenter_min_Save[1][1][1][0]=105;
p->pSavetimeData->u1TxCenter_max_Save[1][1][1][0]=106;
p->pSavetimeData->u1Gating2T_Save[1][1][1][1]=4;
p->pSavetimeData->u1Gating05T_Save[1][1][1][1]=0;
p->pSavetimeData->u1Gatingfine_tune_Save[1][1][1][1]=8;
p->pSavetimeData->u1Gatingucpass_count_Save[1][1][1][1]=3;
p->pSavetimeData->u1WriteLeveling_bypass_Save[1][1][1][1]=43;
p->pSavetimeData->u1TxCenter_min_Save[1][1][1][1]=106;
p->pSavetimeData->u1TxCenter_max_Save[1][1][1][1]=108;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][1][0]=8;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][1][0]=40;
p->pSavetimeData->u1Txwin_center_Save[1][1][1][0]=105;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][1][0]=93;
p->pSavetimeData->u1Txlast_pass_Save[1][1][1][0]=117;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][1][1]=12;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][1][1]=42;
p->pSavetimeData->u1Txwin_center_Save[1][1][1][1]=106;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][1][1]=94;
p->pSavetimeData->u1Txlast_pass_Save[1][1][1][1]=118;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][1][2]=8;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][1][2]=38;
p->pSavetimeData->u1Txwin_center_Save[1][1][1][2]=105;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][1][2]=93;
p->pSavetimeData->u1Txlast_pass_Save[1][1][1][2]=117;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][1][3]=9;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][1][3]=40;
p->pSavetimeData->u1Txwin_center_Save[1][1][1][3]=105;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][1][3]=94;
p->pSavetimeData->u1Txlast_pass_Save[1][1][1][3]=117;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][1][4]=11;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][1][4]=38;
p->pSavetimeData->u1Txwin_center_Save[1][1][1][4]=105;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][1][4]=95;
p->pSavetimeData->u1Txlast_pass_Save[1][1][1][4]=115;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][1][5]=11;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][1][5]=42;
p->pSavetimeData->u1Txwin_center_Save[1][1][1][5]=106;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][1][5]=94;
p->pSavetimeData->u1Txlast_pass_Save[1][1][1][5]=118;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][1][6]=11;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][1][6]=42;
p->pSavetimeData->u1Txwin_center_Save[1][1][1][6]=105;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][1][6]=93;
p->pSavetimeData->u1Txlast_pass_Save[1][1][1][6]=117;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][1][7]=12;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][1][7]=43;
p->pSavetimeData->u1Txwin_center_Save[1][1][1][7]=105;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][1][7]=93;
p->pSavetimeData->u1Txlast_pass_Save[1][1][1][7]=117;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][1][8]=6;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][1][8]=39;
p->pSavetimeData->u1Txwin_center_Save[1][1][1][8]=107;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][1][8]=96;
p->pSavetimeData->u1Txlast_pass_Save[1][1][1][8]=119;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][1][9]=3;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][1][9]=35;
p->pSavetimeData->u1Txwin_center_Save[1][1][1][9]=107;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][1][9]=97;
p->pSavetimeData->u1Txlast_pass_Save[1][1][1][9]=118;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][1][10]=3;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][1][10]=37;
p->pSavetimeData->u1Txwin_center_Save[1][1][1][10]=107;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][1][10]=96;
p->pSavetimeData->u1Txlast_pass_Save[1][1][1][10]=119;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][1][11]=5;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][1][11]=39;
p->pSavetimeData->u1Txwin_center_Save[1][1][1][11]=107;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][1][11]=95;
p->pSavetimeData->u1Txlast_pass_Save[1][1][1][11]=119;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][1][12]=11;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][1][12]=41;
p->pSavetimeData->u1Txwin_center_Save[1][1][1][12]=107;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][1][12]=97;
p->pSavetimeData->u1Txlast_pass_Save[1][1][1][12]=118;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][1][13]=10;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][1][13]=41;
p->pSavetimeData->u1Txwin_center_Save[1][1][1][13]=108;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][1][13]=97;
p->pSavetimeData->u1Txlast_pass_Save[1][1][1][13]=119;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][1][14]=7;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][1][14]=40;
p->pSavetimeData->u1Txwin_center_Save[1][1][1][14]=108;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][1][14]=97;
p->pSavetimeData->u1Txlast_pass_Save[1][1][1][14]=119;
p->pSavetimeData->u1RxWinPerbitDQ_firsbypass_Save[1][1][1][15]=9;
p->pSavetimeData->u1RxWinPerbitDQ_lastbypass_Save[1][1][1][15]=42;
p->pSavetimeData->u1Txwin_center_Save[1][1][1][15]=106;
p->pSavetimeData->u1Txfirst_pass_Save[1][1][1][15]=95;
p->pSavetimeData->u1Txlast_pass_Save[1][1][1][15]=118;
p->pSavetimeData->u1CBTVref_Save[1][1][1]=0;
p->pSavetimeData->u1CBTClkDelay_Save[1][1][1]=0;
p->pSavetimeData->u1CBTCmdDelay_Save[1][1][1]=0;
p->pSavetimeData->u1CBTCsDelay_Save[1][1][1]=0;
p->pSavetimeData->u1RxDatlat_Save[1][1][1]=16;
p->pSavetimeData->u1TxWindowPerbitVref_Save[1][1][1]=8;
p->pSavetimeData->u1RxWinPerbitVref_Save[1][1]=12;
#endif
    				
#endif
}
#endif

static void vCalibration_Flow_LP4(DRAMC_CTX_T *p)
{
    U8 u1RankMax;
    S8 s1RankIdx;
    DRAM_STATUS_T VrefStatus, retStatus;
    
#ifdef DDR_INIT_TIME_PROFILING
    UINT32 CPU_Cycle;
    //TimeProfileBegin();
#endif    

#if TX_PER_BIT_DELAY_CELL
    if((p->channel == CHANNEL_A)&&(p->frequency == 1600 || p->frequency == 1333))
    {
        DramcMiockJmeterForTXCalibration(p);
    }
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
    
        vAutoRefreshSwitch(p, DISABLE);

#if ENABLE_CA_TRAINING //&& (!LP4_DRAM_TYPE_SELECT)) // skip when bring up

        for(s1RankIdx=RANK_0; s1RankIdx<u1RankMax; s1RankIdx++)   
        {
            vSetRank(p, s1RankIdx);
    
                VrefStatus = CmdBusTrainingLP4(p);
                
                #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
                if(((p->pSavetimeData->fcalfail[p->freq_sel] & 0x40)>>6)==1)
                {
                    VrefStatus = CmdBusTrainingLP4(p);
                }
                #endif

                #if SUPPORT_SAVE_OFF_LINE_CLIABRATION
                if(VrefStatus == DRAM_K_VREF)
                    CmdBusTrainingLP4(p);
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
        // only K highest freq
#if CLK_DUTY_SCAN_APPLY_RK0_TO_RK1
        if((p->channel == CHANNEL_A)&&(p->frequency == u2DFSGetHighestFreq(p))&&p->rank==RANK_0)
#else
        if((p->channel == CHANNEL_A)&&(p->frequency == u2DFSGetHighestFreq(p)))
#endif            
        {
            DramcGatingMode(p, 0);

            retStatus = DramcClockDutyScan_TestEngine(p, 0);
            if (retStatus == DRAM_FAIL)
            {   // set 0 k fail, then set RG_ARPI_CLK/CMD/CS to +8 and rescan : Justin
                gXRPI_CLK_CMD_CS_offset[p->channel]=8;
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), P_Fld(u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), SHU1_R0_CA_CMD9_RG_RK0_ARPI_CLK)+gXRPI_CLK_CMD_CS_offset[p->channel], SHU1_R0_CA_CMD9_RG_RK0_ARPI_CLK) |
                                                                           P_Fld(u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), SHU1_R0_CA_CMD9_RG_RK0_ARPI_CMD)+gXRPI_CLK_CMD_CS_offset[p->channel], SHU1_R0_CA_CMD9_RG_RK0_ARPI_CMD) |
                                                                           P_Fld(u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), SHU1_R0_CA_CMD9_RG_RK0_ARPI_CS)+gXRPI_CLK_CMD_CS_offset[p->channel], SHU1_R0_CA_CMD9_RG_RK0_ARPI_CS) );
                retStatus = DramcClockDutyScan_TestEngine(p, 1);
            }
            DramcGatingMode(p, 1);

            //fra DramcModeRegInit_LP4(p);
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
        #if SUPPORT_SAVE_OFF_LINE_CLIABRATION
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
        
        #if SUPPORT_SAVE_OFF_LINE_CLIABRATION
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

    #if DUAL_CHANNEL_ENABLE
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

     for(u1ChannelIdx=0; u1ChannelIdx<CHANNEL_MAX; u1ChannelIdx++)
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
                p->ranksize[u1RankIdx] = u4Size;
                DramInfo->u4MR8Density[u1ChannelIdx][u1RankIdx] = u4Size;
         }
        	
        mcSHOW_DBG_MSG(("[GetDramInforAfterCalByMRR] Channel %d, Rank %d, u1DieNumber %d, Desity %x.\n", u1ChannelIdx,u1RankIdx, u1DieNumber, DramInfo->u4MR8Density[u1ChannelIdx][u1RankIdx]));
        }

    vSetPHY2ChannelMapping(p, CHANNEL_A);

        //while(1);
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
        psCurrDramCtx = &DramCtx_LPDDR3;
    }
    p = psCurrDramCtx;  

    EnableDramcPhyDCM(p, 0);
    Switch26MHzDisableDummyReadRefreshAllBank(p);
 	*((volatile unsigned int*)SPM_POWER_ON_VAL0)      &= ~(0x1 << 28);//Switch PLL to RG mode
	*((volatile unsigned int*)DRAMC_DPY_CLK_SW_CON2)  &= ~(0x1 << 2);//Switch PLL to RG mode  
}

//#define TEST_MODE_MRS
/* DRAM_CBT_MODE_EXTERN_T: Used to differentiate normal vs mixed mode LP4 (dram cbt mode type) */
int Init_DRAM(DRAM_DRAM_TYPE_T dram_type, DRAM_CBT_MODE_EXTERN_T dram_cbt_mode_extern, DRAM_INFO_BY_MRR_T *DramInfo)
{
   #if !SW_CHANGE_FOR_SIMULATION

    #ifdef DDR_INIT_TIME_PROFILING
    UINT32 CPU_Cycle;
    #endif
    int mem_start,len;
    DRAMC_CTX_T * p;
    U8 ucstatus = 0;
    U32 u4value;

    if(u1IsLP4Family(dram_type))
    {
        psCurrDramCtx = &DramCtx_LPDDR4;
    }
    else
    {
        psCurrDramCtx = &DramCtx_LPDDR3;
    }
    p = psCurrDramCtx;  

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

    MPLLInit(p);

    Global_Option_Init(p);

#if (FOR_DV_SIMULATION_USED==0) 
    EMI_Init(p);    
#endif

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


#if ENABLE_MRR_AFTER_FIRST_K
   if(DramInfo != NULL)
#endif
    {
        mcSHOW_DBG_MSG(("\n\n[Kibo] ETT version 0.0.0.1\n dram_type %d, R0 cbt_mode %d, R1 cbt_mode %d VENDOR=%d\n\n", p->dram_type, p->dram_cbt_mode[RANK_0], p->dram_cbt_mode[RANK_1], p->vendor_id));

#if defined(DDR_INIT_TIME_PROFILING)
        TimeProfileBegin();
#endif

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    if(u1IsLP4Family(dram_type))
        DramcSave_Time_For_Cal_Init(p);
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

      //update ODTP/ODTN of term to unterm
      DramcUpdateImpedanceTerm2UnTerm(p);
        
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

#if fcFOR_CHIP_ID == fcElbrus
#if (FOR_DV_SIMULATION_USED==0) 
			EMI_Init2();

#endif
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
        DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_1, DRAM_DFS_SHUFFLE_3);

        DDRPhyFreqSel(p, LP4_DDR3200);
        vSetVcoreByFreq(p);
        DFSInitForCalibration(p);
        vDramCalibrationAllChannel(p);
#if ENABLE_MRR_AFTER_FIRST_K
        vDramcACTimingOptimize(p);
#endif
        DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_1, DRAM_DFS_SHUFFLE_4);

        DDRPhyFreqSel(p, LP4_DDR2667);
        vSetVcoreByFreq(p);
        DFSInitForCalibration(p);
        vDramCalibrationAllChannel(p);    
#if ENABLE_MRR_AFTER_FIRST_K
        vDramcACTimingOptimize(p);
#endif
        DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_1, DRAM_DFS_SHUFFLE_2);

#if ENABLE_LOW_POWER_BOOT
            //Jump to DDR2667@0.7v
            TransferPLLToSPMControl(p);
            DramcDFSDirectJump(p, 1);
#if defined(PMIC_CHIP_MT6355) 
    #ifdef DRAM_HQA //Darren+
            //hqa_vcore_lpm -= REDUCE_3_STEPS; //no use, it's LPM bootup
            regulator_set_voltage(reg_vcore, hqa_vcore_lpm, MAX_VCORE);
    #else
            regulator_set_voltage(reg_vcore, 700000, MAX_VCORE);
    #endif
#else
    #ifdef DRAM_HQA //Darren+
            pmic_config_interface(MT6351_BUCK_VCORE_CON4,hqa_vcore_lpm,0x7F,0); // sagy, 0.7V
            pmic_config_interface(MT6351_BUCK_VCORE_CON5,hqa_vcore_lpm,0x7F,0); // sagy, 0.7V
    #else
            pmic_config_interface(MT6351_BUCK_VCORE_CON4,0x10,0x7F,0); // 0.7V
            pmic_config_interface(MT6351_BUCK_VCORE_CON5,0x10,0x7F,0); // 0.7V
    #endif
#endif
            DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_4, DRAM_DFS_SHUFFLE_1);
#else
            TransferPLLToSPMControl(p);
            DramcDFSDirectJump(p, 1);
#if defined(PMIC_CHIP_MT6355) 
    #ifdef DRAM_HQA //Darren+
            regulator_set_voltage(reg_vcore, hqa_vcore_hpm, MAX_VCORE);
    #else
            regulator_set_voltage(reg_vcore, 800000, MAX_VCORE);
    #endif
#else
    #ifdef DRAM_HQA //Darren+
            pmic_config_interface(MT6351_BUCK_VCORE_CON4,hqa_vcore_hpm,0x7F,0); // sagy, 0.8V
            pmic_config_interface(MT6351_BUCK_VCORE_CON5,hqa_vcore_hpm,0x7F,0); // sagy, 0.8V
    #else
            pmic_config_interface(MT6351_BUCK_VCORE_CON4,0x20,0x7F,0); // 0.8V
            pmic_config_interface(MT6351_BUCK_VCORE_CON5,0x20,0x7F,0); // 0.8V
    #endif
#endif
            DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_4, DRAM_DFS_SHUFFLE_1);
            DDRPhyFreqSel(p, LP4_DDR3200);
            DramcDFSDirectJump(p, 0);
#endif // end of ENABLE_LOW_POWER_BOOT
        //DramcDumpFreqSetting(p);
    }
    else
    {
        DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_1, DRAM_DFS_SHUFFLE_2);   //save 1333 0.74V shuffle_2

        DDRPhyFreqSel(p, LP3_DDR1333);
        //AutoRefreshCKEOff(p);
//        vSetVcoreByFreq(p);
#if defined(PMIC_CHIP_MT6355) 
    #ifdef DRAM_HQA //Darren+
        //hqa_vcore_lpm -= REDUCE_3_STEPS;
        regulator_set_voltage(reg_vcore, hqa_vcore_lpm, MAX_VCORE);
    #else
        regulator_set_voltage(reg_vcore, 700000, MAX_VCORE); // 0.74V
    #endif
#else
    #ifdef DRAM_HQA //Darren+
        pmic_config_interface(MT6351_BUCK_VCORE_CON4,hqa_vcore_lpm,0x7F,0); // sagy, 0.7V
        pmic_config_interface(MT6351_BUCK_VCORE_CON5,hqa_vcore_lpm,0x7F,0); // sagy, 0.7V
    #else
        pmic_config_interface(MT6351_BUCK_VCORE_CON4,0x10,0x7F,0); // 0.7V
        pmic_config_interface(MT6351_BUCK_VCORE_CON5,0x10,0x7F,0); // 0.7V
    #endif
#endif
        DFSInitForCalibration(p);
        vDramCalibrationAllChannel(p);
        DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_1, DRAM_DFS_SHUFFLE_3);   //save 1333 0.74V shuffle_3

        DDRPhyFreqSel(p, LP3_DDR1866);
        //AutoRefreshCKEOff(p);
        vSetVcoreByFreq(p);
        DFSInitForCalibration(p);
        vDramCalibrationAllChannel(p);    

#if ENABLE_LOW_POWER_BOOT
            //Jump to DDR1333@0.7v
            //0.8V/1866, 0.7V/1333, 0.7V/933->1333
            TransferPLLToSPMControl(p);        
            DramcDFSDirectJump(p, 1);
#if defined(PMIC_CHIP_MT6355) 
    #ifdef DRAM_HQA //Darren+
            //hqa_vcore_lpm -= REDUCE_3_STEPS; // no use, it's LPM bootup
            regulator_set_voltage(reg_vcore, hqa_vcore_lpm, MAX_VCORE);
    #else
            regulator_set_voltage(reg_vcore, 700000, MAX_VCORE);
    #endif
#else
    #ifdef DRAM_HQA //Darren+
            pmic_config_interface(MT6351_BUCK_VCORE_CON4,hqa_vcore_lpm,0x7F,0); // sagy, 0.7V
            pmic_config_interface(MT6351_BUCK_VCORE_CON5,hqa_vcore_lpm,0x7F,0); // sagy, 0.7V
    #else
            pmic_config_interface(MT6351_BUCK_VCORE_CON4,0x10,0x7F,0); // 0.7V
            pmic_config_interface(MT6351_BUCK_VCORE_CON5,0x10,0x7F,0); // 0.7V
    #endif
#endif
#endif // end of ENABLE_LOW_POWER_BOOT
    }
#endif

    vApplyConfigAfterCalibration(p);
#if FORLOOP_TEST_SAVE_TIME_FOR_CALIBRATION_ON_ETT
	vDramfirstCalibrationdump(p);
#endif
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
	if(p->pSavetimeData->femmc_Ready==0
			|| (p->pSavetimeData->fcalfail[LP4_DDR1600])
			|| (p->pSavetimeData->fcalfail[LP4_DDR2667])
			|| (p->pSavetimeData->fcalfail[LP4_DDR3200]))
	{
		#if EMMC_READY
		write_offline_dram_calibration_data(p->pSavetimeData);
		#endif
		mcSHOW_DBG_MSG(("[save time for cal] fcalfail[LP4_DDR1600] = 0x%x\n",  p->pSavetimeData->fcalfail[LP4_DDR1600]));
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
#if (FOR_DV_SIMULATION_USED==0) 
    print_DBG_info(p);
    Dump_EMIRegisters(p); 
    #endif
#endif

#if 0
    DramcRegDump(p);
#endif

#ifdef RX_EYE_SCAN
    int i;

    mcSHOW_DBG_MSG(("Channel A eye scan.\n\n"));
    vSetPHY2ChannelMapping(p, CHANNEL_A);
    DramcRxEyeScanInit(p);
    for(i=0; i<8; i++)
    {
        #if 0
        DramcRxEyeScanRun(p, 0, i); // boot time
        #else
        DramcRxEyeScanRun(p, 1, i);  // run time
        #endif
    }

    mcSHOW_DBG_MSG(("Channel B eye scan.\n\n"));
    vSetPHY2ChannelMapping(p, CHANNEL_B);
    DramcRxEyeScanInit(p);
    for(i=0; i<8; i++)
    {
        #if 0
        DramcRxEyeScanRun(p, 0, i); // boot time
        #else
        DramcRxEyeScanRun(p, 1, i);  // run time
        #endif
    }
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

    #if DUAL_CHANNEL_ENABLE
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

#endif//SW_CHANGE_FOR_SIMULATION
    return 0;
}


#if (FOR_DV_SIMULATION_USED!=0)
void DPI_main(void)
{
    DRAM_INFO_BY_MRR_T DramInfo;
    DRAMC_CTX_T DramConfig;

    DramConfig.channel = CHANNEL_A;
    DramConfig.support_rank_num = RANK_DUAL;
    // DramRank
    DramConfig.rank = RANK_0;
    // DRAM type
    DramConfig.dram_type = TYPE_LPDDR3;
    // DRAM Fast switch point type, only for LP4, useless in LP3
    DramConfig.dram_fsp = FSP_0;
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
    // Switch to ENABLE or DISABLE SSC (Spread Spectrum Clock)
    DramConfig.ssc_en = DISABLE;    
    DramConfig.en_4bitMux = DISABLE;

    DramConfig.enable_rx_scan_vref =DISABLE;
    DramConfig.enable_tx_scan_vref =DISABLE;
    //DramConfig.dynamicODT = DISABLE;    

    printf("main functino start!\n");

#if ENABLE_MRR_AFTER_FIRST_K 
    Init_DRAM(DramConfig.dram_type, CBT_R0_R1_NORMAL, &DramInfo);
#endif
    Init_DRAM(DramConfig.dram_type, CBT_R0_R1_NORMAL, NULL);
}

#define DV_SIMULATION_INIT_C    1
#define DV_SIMULATION_SW_IMPED 1
#define DV_SIMULATION_CA_TRAINING 1
#define DV_SIMULATION_WRITE_LEVELING  1
#define DV_SIMULATION_GATING 0
#define DV_SIMULATION_DATLAT 0
#define DV_SIMULATION_RX_PERBIT    0
#define DV_SIMULATION_TX_PERBIT    0 // Please enable with write leveling
#define DV_SIMULATION_RUN_TIME_MRW 0

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
    DramConfig->dram_cbt_mode[RANK_0] = CBT_NORMAL_MODE;    
    DramConfig->dram_cbt_mode[RANK_1] = CBT_NORMAL_MODE;        
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

    vApplyConfigBeforeCalibration(DramConfig);
    vMR2InitForSimulationTest(DramConfig);

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

    vApplyConfigAfterCalibration(DramConfig);
}

void DPI_SW_main_LP4(DRAMC_CTX_T *DramConfig)
    {
    int ii;

    DramConfig->channel = CHANNEL_A;
    DramConfig->support_rank_num = RANK_DUAL;
    // DramRank
    DramConfig->rank = RANK_0;
    // DRAM type
    DramConfig->dram_type = TYPE_LPDDR4;
    // DRAM Fast switch point type, only for LP4, useless in LP3
    DramConfig->dram_fsp = FSP_0;
    // DRAM CBT mode, only for LP4, useless in LP3
    DramConfig->dram_cbt_mode[RANK_0] = CBT_NORMAL_MODE;
    DramConfig->dram_cbt_mode[RANK_1] = CBT_NORMAL_MODE;
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
    // Switch to ENABLE or DISABLE SSC (Spread Spectrum Clock)
    DramConfig->ssc_en = DISABLE;    
    DramConfig->en_4bitMux = DISABLE;

    DramConfig->enable_rx_scan_vref =DISABLE;
    DramConfig->enable_tx_scan_vref =DISABLE;
    //DramConfig->dynamicODT = DISABLE;

    Global_Option_Init(DramConfig);

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
    DramcRxInputDelayTrackingInit_byFreq(DramConfig);
    DramcRxInputDelayTrackingInit_Common(DramConfig);    
    DramcRxInputDelayTrackingHW(DramConfig);
#endif


#if WRITE_LEVELING_MOVE_DQS_INSTEAD_OF_CLK    
    memset(DramConfig->arfgWriteLevelingInitShif, FALSE, sizeof(DramConfig->arfgWriteLevelingInitShif));
    //>fgWriteLevelingInitShif= FALSE;
#endif
#if TX_PERBIT_INIT_FLOW_CONTROL
    memset(DramConfig->fgTXPerbifInit, FALSE, sizeof(DramConfig->fgTXPerbifInit));
#endif

    vApplyConfigBeforeCalibration(DramConfig);
    vMR2InitForSimulationTest(DramConfig);

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

    vApplyConfigAfterCalibration(DramConfig);

#if DV_SIMULATION_RUN_TIME_MRW
    enter_pasr_dpd_config(0, 0xFF);
#endif
}

#endif


