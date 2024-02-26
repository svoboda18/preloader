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

//=============================================================================
//  Include Files                                                      
//=============================================================================
#include "dramc_common.h"
#include "dramc_register.h"
#include "dramc_pi_api.h"
#if __ETT__
#include "common.h"
#endif

//=============================================================================
//  Definition                                                         
//=============================================================================
#define fcWL_ALL   // Do Write Leveling with all DQS together
//#define fcFOR_DQSGDUALP_ENABLE   // Dual-phase DQS clock gating control enabling (new gating, not define as MT6589)

// LPDDR DQ -> PHY DQ mapping
#if fcFOR_CHIP_ID == fcK2
U32 uiLPDDR3_PHY_Mapping_POP_CHA[32] = { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
                       16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};

#endif

#if fcFOR_CHIP_ID == fcJADE
//PCB DQ11 = DRAMPHY 10
U32 uiLPDDR3_PHY_Mapping_POP_CHA[32] = { 0,  2,  1,  4,  5,  3,  6,  7,  8,  9, 12, 10, 11, 15, 14, 13,
                       23, 16, 17, 19, 20, 18, 22, 21, 24, 25, 28, 30, 27, 26, 29, 31};

U32 uiLPDDR3_CA_Mapping_CHA[10] = {2,1,4,0,3,7,8,5,9,6};

#endif


//=============================================================================
//  Global Variables                                                  
//=============================================================================
static U32 dqs_gw[DQS_GW_LEN];
static U8 dqs_gw_coarse, dqs_gw_fine, dqs_gw_fine_cnt;
static U8 opt_gw_coarse_value, opt_gw_fine_value;
U8 CurrentRank = 0;
U8 opt_gw_coarse_value_R0[CHANNEL_NUMBER], opt_gw_fine_value_R0[CHANNEL_NUMBER];
U8 opt_gw_coarse_value_R1[CHANNEL_NUMBER], opt_gw_fine_value_R1[CHANNEL_NUMBER];
static U8 dqs_gw_cnt_break;
static U8 fgwrlevel_done[CHANNEL_NUMBER] = {0,0};
U8 RXPERBIT_LOG_PRINT = 1;
S8 wrlevel_dqs_final_delay[CHANNEL_NUMBER][DQS_NUMBER];
static U16 u2rx_window_sum;
#ifdef TDP_TDN_CAL
static U16 u2tx_window_sum;
#endif

static S8 CATrain_ClkDelay[CHANNEL_NUMBER];
static U8 ucDLESetting;

#ifdef SPM_WORKAROUND_CATRAINING
U32 DUAL_FREQ_HIGH_reg_1a8, DUAL_FREQ_HIGH_reg_1ac, DUAL_FREQ_HIGH_reg_1b0, DUAL_FREQ_HIGH_reg_1b8, DUAL_FREQ_HIGH_reg_1c4, DUAL_FREQ_HIGH_reg_00c;
U32 DUAL_FREQ_LOW_reg_1a8, DUAL_FREQ_LOW_reg_1ac, DUAL_FREQ_LOW_reg_1b0, DUAL_FREQ_LOW_reg_1b8, DUAL_FREQ_LOW_reg_1c4, DUAL_FREQ_LOW_reg_00c;
#endif


#ifdef EYE_SCAN
typedef struct _EYESCAN_WIN
{
  U8 ucsetup_pass_number;
  U8 uchold_pass_number;
  #ifdef ACDIO_TEST 
  U8 window_number;
  #endif  
} EYESCAN_WIN;
EYESCAN_WIN EyeScanWin[32];
#endif


//U8 ucswap_table[2][32] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 , 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 
//                                            0, 1, 2, 3, 8, 9, 10, 11, 4, 5, 6, 7, 12, 13, 14, 15, 16, 17, 18, 19, 24, 25, 26, 27, 20, 21, 22, 23, 28, 29, 30, 31};

//=============================================================================
//  External references                                                
//=============================================================================






#if fcFOR_CHIP_ID == fcK2 || fcFOR_CHIP_ID == fcJADE

//-------------------------------------------------------------------------
/** DramcPllPhaseCal
 *  start PLL Phase Calibration.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param  apply           (U8): 0 don't apply the register we set  1 apply the register we set ,default don't apply.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL 
 */
//-------------------------------------------------------------------------
DRAM_STATUS_T DramcPllPhaseCal(DRAMC_CTX_T *p)
{
// MEMPLL's are different for A60808 and MT6595
    U8 ucstatus = 0;
    U16 one_count = 0, zero_count = 0;
    U8 pll1st_done = 0, pll2nd_done = 0, pll3rd_done = 0, pll4th_done = 0;
    U8 pll1st_dl = 0, pll2nd_dl = 0, pll3rd_dl = 0, pll4th_dl = 0;
    U8 pll1st_phase=0, pll2nd_phase=0, pll3rd_phase=0, pll4th_phase=0;
    S8 ret = 0;
    U32 u4value;
    
    // for PoP, use MEMPLL 2,4 and MEMPLL05 2, 3
    // for SBS, use MEMPLL 2,3,4 (SBS for K2)

    if (p->package == PACKAGE_POP)
    {
        return DRAM_OK;
    }

#if fcFOR_CHIP_ID == fcJADE
  // 0. Set jitter meter clock source CG enable
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x698), &u4value);
  mcCLR_BIT(u4value, 16);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x698), u4value);  
#endif

    mcSHOW_DBG_MSG(("[PLL_Phase_Calib] ===== PLL Phase Calibration:CHANNEL %d (0: CHA, 1: CHB) =====\n", p->channel));

    // 1. Set jitter meter clock to internal FB path    
    // MEMPLL*_FB_MCK_SEL = 0;
    // Not necessary for A60808
    // It has no phase difference when internal or external loop

    // 2. Set jitter meter count number    
    // JMTRCNT = 0x400;    // JMTRCNT:0x74[30:16]. Set jitter meter count number to 1024
    // MEMPLL 2 jitter meter count
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1cc), &u4value);
    mcSET_FIELD(u4value, fcJMETER_COUNT, 0xffff0000, 16);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1cc), u4value);

    // MEMPLL 4 jitter metter count
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1d4), &u4value);
    mcSET_FIELD(u4value, fcJMETER_COUNT, 0xffff0000, 16);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1d4), u4value);

    // MEMPLL 3 jitter meter count
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1d0), &u4value);
    mcSET_FIELD(u4value, fcJMETER_COUNT, 0xffff0000, 16);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1d0), u4value);
    
    while(1)
    {         
            if (!pll1st_done)    // MEMPLL 2
            {
                if (pll1st_phase == 0) // initial phase set to 0 for REF and FBK
                {
                    // RG_MEPLL2_REF_DL:0x618[12:8]. MEMPLL2 REF_DL to 0
                    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x618), &u4value);
                    mcSET_FIELD(u4value, 0x0, 0x00001f00, 8);
                    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x618), u4value);

                    // RG_MEPLL2_FBK_DL:0x618[4:0]. MEMPLL2 FBK_DL to 0
                    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x618), &u4value);
                    mcSET_FIELD(u4value, 0x0, 0x0000001f, 0);
                    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x618), u4value);                    
                }
                else if (pll1st_phase == 1) // REF lag FBK, delay FBK
                {
                    // RG_MEPLL2_REF_DL:0x618[12:8]. MEMPLL2 REF_DL to 0
                    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x618), &u4value);
                    mcSET_FIELD(u4value, 0x0, 0x00001f00, 8);
                    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x618), u4value);

                    // RG_MEPLL2_FBK_DL:0x618[4:0]. MEMPLL2 FBK_DL to increase
                    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x618), &u4value);
                    mcSET_FIELD(u4value, pll1st_dl, 0x0000001f, 0);
                    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x618), u4value); 
                }
                else // REF lead FBK, delay REF
                {
                    // RG_MEPLL2_REF_DL:0x618[12:8]. MEMPLL2 REF_DL to increase
                    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x618), &u4value);
                    mcSET_FIELD(u4value, pll1st_dl, 0x00001f00, 8);
                    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x618), u4value);

                    // RG_MEPLL2_FBK_DL:0x618[4:0]. MEMPLL2 FBK_DL to 0
                    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x618), &u4value);
                    mcSET_FIELD(u4value, 0x0, 0x0000001f, 0);
                    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x618), u4value); 
                }                
            }
            if (!pll2nd_done)    // MEMPLL 4
            {
                if (pll2nd_phase == 0) // initial phase set to 0 for REF and FBK
                {
                    // RG_MEPLL4_REF_DL:0x630[12:8]. MEMPLL4 REF_DL to 0
                    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x630), &u4value);
                    mcSET_FIELD(u4value, 0x0, 0x00001f00, 8);
                    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x630), u4value);

                    // RG_MEPLL4_FBK_DL:0x630[4:0]. MEMPLL4 FBK_DL to 0
                    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x630), &u4value);
                    mcSET_FIELD(u4value, 0x0, 0x0000001f, 0);
                    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x630), u4value);                  
                }
                else if (pll2nd_phase == 1) // REF lag FBK, delay FBK
                {
                    // RG_MEPLL4_REF_DL:0x630[12:8]. MEMPLL4 REF_DL to 0
                    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x630), &u4value);
                    mcSET_FIELD(u4value, 0x0, 0x00001f00, 8);
                    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x630), u4value);

                    // RG_MEPLL4_FBK_DL:0x630[4:0]. MEMPLL4 FBK_DL to increase
                    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x630), &u4value);
                    mcSET_FIELD(u4value, pll2nd_dl, 0x0000001f, 0);
                    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x630), u4value); 
                }
                else // REF lead FBK, delay REF
                {
                    // RG_MEPLL4_REF_DL:0x630[12:8]. MEMPLL4 REF_DL to increase
                    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x630), &u4value);
                    mcSET_FIELD(u4value, pll2nd_dl, 0x00001f00, 8);
                    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x630), u4value);

                    // RG_MEPLL4_FBK_DL:0x630[4:0]. MEMPLL4 FBK_DL to 0
                    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x630), &u4value);
                    mcSET_FIELD(u4value, 0x0, 0x0000001f, 0);
                    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x630), u4value); 
                }                
            }
            if (!pll3rd_done)    // MEMPLL 3
            {
                if (pll3rd_phase == 0) // initial phase set to 0 for REF and FBK
                {
                    // RG_MEPLL3_REF_DL:0x624[12:8]. MEMPLL05_2 REF_DL to 0
                    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x624), &u4value);
                    mcSET_FIELD(u4value, 0x0, 0x00001f00, 8);
                    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x624), u4value);

                    // RG_MEPLL3_FBK_DL:0x624[4:0]. MEMPLL05_2 FBK_DL to 0
                    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x624), &u4value);
                    mcSET_FIELD(u4value, 0x0, 0x0000001f, 0);
                    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x624), u4value);
                }
                else if (pll3rd_phase == 1) // REF lag FBK, delay FBK
                {
                    // RG_MEPLL3_REF_DL:0x624[12:8]. MEMPLL05_2 REF_DL to 0
                    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x624), &u4value);
                    mcSET_FIELD(u4value, 0x0, 0x00001f00, 8);
                    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x624), u4value);

                    // RG_MEPLL3_FBK_DL:0x624[4:0]. MEMPLL05_2 FBK_DL to increase
                    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x624), &u4value);
                    mcSET_FIELD(u4value, pll3rd_dl, 0x0000001f, 0);
                    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x624), u4value);
                }
                else // REF lead FBK, delay REF
                {
                    // RG_MEPLL3_REF_DL:0x624[12:8]. MEMPLL05_2 REF_DL to increase
                    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x624), &u4value);
                    mcSET_FIELD(u4value, pll3rd_dl, 0x00001f00, 8);
                    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x624), u4value);

                    // RG_MEPLL3_FBK_DL:0x624[4:0]. MEMPLL05_2 FBK_DL to 0
                    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x624), &u4value);
                    mcSET_FIELD(u4value, 0x0, 0x0000001f, 0);
                    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x624), u4value);
                }                
            }

            // delay 20us for external loop PLL stable
            mcDELAY_US(20);

      // 4. Enable jitter meter
      //MEMPLL 2 jitter meter enable
      ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1cc), &u4value);
      mcSET_BIT(u4value, 0);
      ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1cc), u4value);

      // MEMPLL 4 jitter metter enable
      ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1d4), &u4value);
      mcSET_BIT(u4value, 0);
      ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1d4), u4value);

      // MEMPLL 3 jitter metter enable
      ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1d0), &u4value);
      mcSET_BIT(u4value, 0);
      ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1d0), u4value);      

            // 5. Wait for jitter meter complete (depend on clock 20MHz, 1024/20M=51.2us )
            mcDELAY_US(fcJMETER_WAIT_DONE_US);
            // Another solution is to use done to check ready. Check Reg.32ch[7:0]=01101010b (0x6a) 0: not ready. 1: update result.

      //BringUp : Maybe bypass.
            // If ok, may remove above delay.
            do 
            {
              ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x32c), &u4value);
            }
            while ((u4value & 0x0e)!=0x0e);
     

            // 6. Check jitter meter counter value
            if (!pll1st_done)    // MEMPLL 2
            {
                ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x320), &u4value);
                one_count = (U16) mcGET_FIELD(u4value, 0xffff0000, 16);
                zero_count = (U16) mcGET_FIELD(u4value, 0x0000ffff, 0);

                if (pll1st_phase == 0)
                {                   
                    if (one_count > (zero_count+fcJMETER_COUNT/10))
                    {
                        // REF lag FBK
                        pll1st_phase = 1;
                        pll1st_dl++;

                        mcSHOW_DBG_MSG(("[PLL_Phase_Calib] PLL2 initial phase: REF lag FBK, one_cnt/zero_cnt = %d/%d\n", one_count, zero_count));
                    }
                    else if (zero_count > (one_count+fcJMETER_COUNT/10))
                    {
                        // REF lead FBK
                        pll1st_phase = 2;
                        pll1st_dl++;

                        mcSHOW_DBG_MSG(("[PLL_Phase_Calib] PLL2 initial phase: REF lead FBK, one_cnt/zero_cnt = %d/%d\n", one_count, zero_count));
                    }
                    else
                    {
                        // in phase at initial
                        pll1st_done = 1;

                        mcSHOW_DBG_MSG(("[PLL_Phase_Calib] PLL2 initial phase: REF in-phase FBK, one_cnt/zero_cnt = %d/%d\n", one_count, zero_count));                        
                    }
                }
                else if (pll1st_phase == 1)
                {
                    if ((zero_count+fcJMETER_COUNT/10) >= one_count)
                    {
                        pll1st_done = 1;
                        mcSHOW_DBG_MSG(("[PLL_Phase_Calib] PLL2 REF_DL: 0x0, FBK_DL: 0x%x, one_cnt/zero_cnt = %d/%d\n", pll1st_dl, one_count, zero_count));
                    }
                    else
                    {
                        pll1st_dl++;
                    }
                }
                else
                {
                    if ((one_count+fcJMETER_COUNT/10) >= zero_count)
                    {
                        pll1st_done = 1;
                        mcSHOW_DBG_MSG(("[PLL_Phase_Calib] PLL2 REF_DL: 0x%x, FBK_DL: 0x0, one_cnt/zero_cnt = %d/%d\n", pll1st_dl, one_count, zero_count));
                    }
                    else
                    {
                        pll1st_dl++;
                    }
                }                
            }

            if (!pll2nd_done)    // MEMPLL 4
            {
                ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x328), &u4value);
                one_count = (U16) mcGET_FIELD(u4value, 0xffff0000, 16);
                zero_count = (U16) mcGET_FIELD(u4value, 0x0000ffff, 0);
                
                if (pll2nd_phase == 0)
                {
                    if (one_count > (zero_count+fcJMETER_COUNT/10))
                    {
                        // REF lag FBK
                        pll2nd_phase = 1;
                        pll2nd_dl++;

                        mcSHOW_DBG_MSG(("[PLL_Phase_Calib] PLL4 initial phase: REF lag FBK, one_cnt/zero_cnt = %d/%d\n", one_count, zero_count));
                    }
                    else if (zero_count > (one_count+fcJMETER_COUNT/10))
                    {
                        // REF lead FBK
                        pll2nd_phase = 2;
                        pll2nd_dl++;

                        mcSHOW_DBG_MSG(("[PLL_Phase_Calib] PLL4 initial phase: REF lead FBK, one_cnt/zero_cnt = %d/%d\n", one_count, zero_count));
                    }
                    else
                    {
                        // in phase at initial
                        pll2nd_done = 1;

                        mcSHOW_DBG_MSG(("[PLL_Phase_Calib] PLL4 initial phase: REF in-phase FBK, one_cnt/zero_cnt = %d/%d\n", one_count, zero_count));                        
                    }
                }
                else if (pll2nd_phase == 1)
                {
                    if ((zero_count+fcJMETER_COUNT/10) >= one_count)
                    {
                        pll2nd_done = 1;
                        mcSHOW_DBG_MSG(("[PLL_Phase_Calib] PLL4 REF_DL: 0x0, FBK_DL: 0x%x, one_cnt/zero_cnt = %d/%d\n", pll2nd_dl, one_count, zero_count));
                    }
                    else
                    {
                        pll2nd_dl++;
                    }
                }
                else
                {
                    if ((one_count+fcJMETER_COUNT/10) >= zero_count)
                    {
                        pll2nd_done = 1;
                        mcSHOW_DBG_MSG(("[PLL_Phase_Calib] PLL4 REF_DL: 0x%x, FBK_DL: 0x0, one_cnt/zero_cnt = %d/%d\n", pll2nd_dl, one_count, zero_count));
                    }
                    else
                    {
                        pll2nd_dl++;
                    }
                }
            }

            if (!pll3rd_done)    // MEMPLL3
            {
                ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x324), &u4value);
                one_count = (U16) mcGET_FIELD(u4value, 0xffff0000, 16);
                zero_count = (U16) mcGET_FIELD(u4value, 0x0000ffff, 0);
                
                if (pll3rd_phase == 0)
                {
                    if (one_count > (zero_count+fcJMETER_COUNT/10))
                    {
                        // REF lag FBK
                        pll3rd_phase = 1;
                        pll3rd_dl++;

                        mcSHOW_DBG_MSG(("[PLL_Phase_Calib] PLL3 initial phase: REF lag FBK, one_cnt/zero_cnt = %d/%d\n", one_count, zero_count));
                    }
                    else if (zero_count > (one_count+fcJMETER_COUNT/10))
                    {
                        // REF lead FBK
                        pll3rd_phase = 2;
                        pll3rd_dl++;

                        mcSHOW_DBG_MSG(("[PLL_Phase_Calib] PLL3 initial phase: REF lead FBK, one_cnt/zero_cnt = %d/%d\n", one_count, zero_count));
                    }
                    else
                    {
                        // in phase at initial
                        pll3rd_done = 1;

                        mcSHOW_DBG_MSG(("[PLL_Phase_Calib] PLL3 initial phase: REF in-phase FBK, one_cnt/zero_cnt = %d/%d\n", one_count, zero_count));                        
                    }
                }
                else if (pll3rd_phase == 1)
                {
                    if ((zero_count+fcJMETER_COUNT/10) >= one_count)
                    {
                        pll3rd_done = 1;
                        mcSHOW_DBG_MSG(("[PLL_Phase_Calib] PLL3 REF_DL: 0x0, FBK_DL: 0x%x, one_cnt/zero_cnt = %d/%d\n", pll3rd_dl, one_count, zero_count));
                    }
                    else
                    {
                        pll3rd_dl++;
                    }
                }
                else
                {
                    if ((one_count+fcJMETER_COUNT/10) >= zero_count)
                    {
                        pll3rd_done = 1;
                        mcSHOW_DBG_MSG(("[PLL_Phase_Calib] PLL3 REF_DL: 0x%x, FBK_DL: 0x0, one_cnt/zero_cnt = %d/%d\n", pll3rd_dl, one_count, zero_count));
                    }
                    else
                    {
                        pll3rd_dl++;
                    }
                }
            }

      // 7. Reset jitter meter value
      //MEMPLL 2 jitter meter disable
      ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1cc), &u4value);
      mcCLR_BIT(u4value, 0);
      ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1cc), u4value);

      // MEMPLL 4 jitter metter disable
      ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1d4), &u4value);
      mcCLR_BIT(u4value, 0);
      ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1d4), u4value);

      // MEMPLL 3 jitter metter disable
      ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1d0), &u4value);
      mcCLR_BIT(u4value, 0);
      ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1d0), u4value);

            // 8. All done?! early break
            if (pll1st_done && pll2nd_done && pll3rd_done)
            {
                ret = 0;
                break;
            }

            // 9. delay line overflow?! break
            if ((pll1st_dl >= 32) || (pll2nd_dl >= 32) || (pll3rd_dl >= 32))
            {
                ret = -1;
                break;
            }
    }         
    
    if (ret != 0)
    {
        mcSHOW_ERR_MSG(("MEMPLL calibration fail\n"));
    }

    if (ucstatus != 0)
    {
        mcSHOW_ERR_MSG(("register access fail!\n"));        
    }
       

    /***********************************************
    * 7. Set jitter meter clock to external FB path
    ************************************************/
    //MEMPLL*_FB_MCK_SEL = 1;
    // Not necessary for A60808 

#ifdef DDR_FT_LOAD_BOARD
    if (ret !=0)
    {
        LoadBoardShowResult(FLAG_PLLPHASE_CALIBRATION, FLAG_CALIBRATION_FAIL, 0, FLAG_NOT_COMPLETE_OR_FAIL);            
        while(1);
    }
    else if ((pll1st_dl >= PLL_PHCALIB_BOUND) || (pll2nd_dl >= PLL_PHCALIB_BOUND) || (pll3rd_dl >= PLL_PHCALIB_BOUND) || (pll4th_dl >=PLL_PHCALIB_BOUND))
    {
        // too large skew between REF and FBK
        LoadBoardShowResult(FLAG_PLLPHASE_CALIBRATION, FLAG_WINDOW_TOO_SMALL, 0, FLAG_NOT_COMPLETE_OR_FAIL);            
        while(1);
    }
#endif

#if fcFOR_CHIP_ID == fcJADE
  // 9. Set jitter meter clock source CG disable to save power
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x698), &u4value);
  mcSET_BIT(u4value, 16);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x698), u4value);  
#endif

    if ((ucstatus != 0) || (ret != 0))
    {
        return DRAM_FAIL;
    }
    else
    {
        return DRAM_OK;
    }

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
DRAM_STATUS_T DramcSwImpedanceCal(DRAMC_CTX_T *p, U8 apply)
{
// drv*_status
//       0: idle
//       1: to be done
//       2: Calibration OK
//     -1: Calibration FAIL
//     -2: Calibration NOT FOUND

    S8 drvp_status, drvn_status;
    U8 ii, drvp = 0xf, drvn = 0xf;
    U8 ucstatus =0;
    U32 u4value, u4backup_reg;

#ifndef RELEASE        
    mcSHOW_DBG_MSG(("[Imp Calibration] Start SW impedance calibration...\n"));
#else
    mcSHOW_USER_MSG(("Imp\n"));
#endif
            
    // 1.Initialization
    // DRVREF (REG.100[24]) = 0: change will be apply directly 
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_OCDK), &u4value);
    mcCLR_BIT(u4value, POS_OCDK_DRVREF);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_OCDK), u4value);
    
    // Enable R_DMMIOCKCTRLOFF (REG.1DC[26]), always no gating
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), &u4value);
    u4backup_reg = u4value;
    mcSET_BIT(u4value, POS_DRAMC_PD_CTRL_MIOCKCTRLOFF);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), u4value);

    // Set R_DMIMPCAL_HW (REG.1C8[1]) = 0 (SW)
    ucstatus |= ucDramC_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), &u4value);
    mcCLR_BIT(u4value, POS_IMPCAL_IMPCAL_HW);
    ucstatus |= ucDramC_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), u4value);

    // Set R_DMIMPCALI_EN (REG.1C8[0]) = 1
    ucstatus |= ucDramC_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), &u4value);
    mcSET_BIT(u4value, POS_IMPCAL_IMPCALI_EN);
    ucstatus |= ucDramC_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), u4value);
            
    // 2.DRVP calibration
    // R_DMIMP_CALI_ENP (REG.1C8[5]) = 1
    ucstatus |= ucDramC_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), &u4value);
    mcSET_BIT(u4value, POS_IMPCAL_IMP_CALI_ENP);
    mcCLR_BIT(u4value, POS_IMPCAL_IMP_CALI_ENN);
    ucstatus |= ucDramC_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), u4value);

    // delay 10ns
    mcDELAY_US(1);

    drvp_status = 0;
    for(ii = 0 ; ii <=15; ii ++)
    {
        // Set R_DMIMPDRVP (REG.1C8[11:8])
        ucstatus |= ucDramC_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), &u4value);
        mcSET_FIELD(u4value, ii, MASK_IMPCAL_IMPDRVP, POS_IMPCAL_IMPDRVP);
        ucstatus |= ucDramC_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), u4value);

        // delay 10ns
        mcDELAY_US(1);

        // Set R_DMIMP_PDP (REG.1C8[7]) = 1
        ucstatus |= ucDramC_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), &u4value);
        mcSET_BIT(u4value, POS_IMPCAL_IMPPDP);
        ucstatus |= ucDramC_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), u4value);

        // delay 10ns
        mcDELAY_US(1);

        // Set R_DMIMP_PDP (REG.1C8[7]) = 0
        ucstatus |= ucDramC_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), &u4value);
        mcCLR_BIT(u4value, POS_IMPCAL_IMPPDP);
        ucstatus |= ucDramC_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), u4value);

        // delay 10ns
        mcDELAY_US(1);

        // Check DMCMPOUT (REG.3DC[31])
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DLLSTATUS0), &u4value);
        //mcSHOW_DBG_MSG(("[Imp Calibration] DRVP: %d, CMPOUT: %d\n", ii, mcTEST_BIT(u4value, POS_DLLSTATUS0_CMPOT)>>POS_DLLSTATUS0_CMPOT));

        if (mcTEST_BIT(u4value, POS_DLLSTATUS0_CMPOT) == 0)
        {
            if (drvp_status == 1)
            {
               
                drvp = ii;
        mcSHOW_DBG_MSG(("[Imp Calibration] Calibration done 0-1-0 case...DRVP:%d\n", drvp));
                drvp_status = 2;

                break;
            }
            else if (drvp_status == 0)
            {
                // 0-0: to next DRVP
            }
            else
            {
                // wrong status
                mcSHOW_ERR_MSG(("[Imp Calibration] DRVP error: wrong status=%d!!\n", drvp_status));
                break;
            }
        }
        else
        {
            if (drvp_status == 1)
            {
                // 1-1: calibration done
                drvp = ii -1;
#ifndef RELEASE                 
                mcSHOW_DBG_MSG(("[Imp Calibration] Calibration done...DRVP:%d\n", drvp));
#else
                mcSHOW_USER_MSG(("%d\n", drvp));
#endif                

                // Set R_DMIMPDRVP (REG.1C8[11:8])
                ucstatus |= ucDramC_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), &u4value);
                mcSET_FIELD(u4value, drvp, MASK_IMPCAL_IMPDRVP, POS_IMPCAL_IMPDRVP);
                ucstatus |= ucDramC_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), u4value);

                drvp_status = 2;
                break;
            }
            else if (drvp_status == 0)
            {
                // 0-1: calibration to be done
                drvp_status = 1;
                //mcSHOW_DBG_MSG(("[Imp Calibration] Calibration to be done...DRVP:%d\n", ii));
            }
            else
            {
                // wrong status
                mcSHOW_ERR_MSG(("[Imp Calibration] DRVP error: wrong status=%d!!\n", drvp_status));
                break;
            }
        }
    }

    if (ii == 16)
    {
    #if 0
        drvp_status = -2;
        mcSHOW_ERR_MSG(("[Imp Calibration] NO VALID DRVP!!\n"));
    #else
        drvp_status = 2;
        drvp = 15;
        mcSHOW_ERR_MSG(("[Imp Calibration] NO VALID DRVP!!Set to 15.\n"));
    #endif
    
    #ifdef DDR_FT_LOAD_BOARD
        LoadBoardShowResult(FLAG_IMPEDANCE_CALIBRATION, FLAG_CALIBRATION_FAIL, 0, FLAG_NOT_COMPLETE_OR_FAIL);            
        while(1);    
    #endif
    }

    // 3.DRVN calibration
    // R_DMIMP_CALI_ENN (REG.1C8[4]) = 1
    ucstatus |= ucDramC_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), &u4value);
    mcCLR_BIT(u4value, POS_IMPCAL_IMP_CALI_ENP);
    mcSET_BIT(u4value, POS_IMPCAL_IMP_CALI_ENN);
    ucstatus |= ucDramC_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), u4value);

    // delay 10ns
    mcDELAY_US(1);

    drvn_status = 0;
    for(ii = 0 ; ii <=15; ii ++)
    {
        // Set R_DMIMPDRVN (REG.1C8[15:12])
        ucstatus |= ucDramC_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), &u4value);
        mcSET_FIELD(u4value, ii, MASK_IMPCAL_IMPDRVN, POS_IMPCAL_IMPDRVN);
        ucstatus |= ucDramC_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), u4value);

        // delay 10ns
        mcDELAY_US(1);

        // Set R_DMIMP_PDN (REG.1C8[6]) = 1
        ucstatus |= ucDramC_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), &u4value);
        mcSET_BIT(u4value, POS_IMPCAL_IMPPDN);
        ucstatus |= ucDramC_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), u4value);

        // delay 10ns
        mcDELAY_US(1);

        // Set R_DMIMP_PDN (REG.1C8[6]) = 0
        ucstatus |= ucDramC_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), &u4value);
        mcCLR_BIT(u4value, POS_IMPCAL_IMPPDN);
        ucstatus |= ucDramC_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), u4value);

        // delay 10ns
        mcDELAY_US(1);

        // Check DMCMPOUTN (REG.3DC[30])
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DLLSTATUS0), &u4value);
        //mcSHOW_DBG_MSG(("[Imp Calibration] DRVN: %d, CMPOUT: %d\n", ii, mcTEST_BIT(u4value, POS_DLLSTATUS0_CMPOUTN)>>POS_DLLSTATUS0_CMPOUTN));

        if (mcTEST_BIT(u4value, POS_DLLSTATUS0_CMPOUTN) == 0)
        {
            if (drvn_status == 1)
            {        
                if (ii >=1)
                {
                    drvn = ii -1;
                }
                else
                {
                    drvn = 0;
                }
                mcSHOW_DBG_MSG(("[Imp Calibration] Calibration done 0-1-1 case!...DRVN:%d\n", drvn));
                // Set R_DMIMPDRVN (REG.1C8[15:12])
                ucstatus |= ucDramC_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), &u4value);
                mcSET_FIELD(u4value, drvn, MASK_IMPCAL_IMPDRVN, POS_IMPCAL_IMPDRVN);
                ucstatus |= ucDramC_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), u4value);

                drvn_status = 2;
                break;
            }
            else if (drvn_status == 0)
            {
                // 0-0: to next DRVN
            }
            else
            {
                // wrong status
                mcSHOW_ERR_MSG(("[Imp Calibration] DRVN error: wrong status=%d!!\n", drvn_status));
                break;
            }
        }
        else
        {
            if (drvn_status == 1)
            {
                // 1-1: calibration done
                if (ii >=2)
                {
                    drvn = ii -2;
                }
                else
                {
                    drvn = 0;
                }
#ifndef RELEASE 
                mcSHOW_DBG_MSG(("[Imp Calibration] Calibration done...DRVN:%d\n", drvn));
#else
                mcSHOW_USER_MSG(("%d\n", drvn));
#endif

                // Set R_DMIMPDRVN (REG.1C8[15:12])
                ucstatus |= ucDramC_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), &u4value);
                mcSET_FIELD(u4value, drvn, MASK_IMPCAL_IMPDRVN, POS_IMPCAL_IMPDRVN);
                ucstatus |= ucDramC_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), u4value);

                drvn_status = 2;
                break;
            }
            else if (drvn_status == 0)
            {
                // 0-1: calibration to be done
                drvn_status = 1;
                //mcSHOW_DBG_MSG(("[Imp Calibration] Calibration to be done...DRVN:%d\n", ii));
            }
            else
            {
                // wrong status
                mcSHOW_ERR_MSG(("[Imp Calibration] DRVN error: wrong status=%d!!\n", drvn_status));
                break;
            }
        }
    }

    if (ii == 16)
    {
    #if 0
        drvn_status = -2;
        mcSHOW_ERR_MSG(("[Imp Calibration] NO VALID DRVN!!\n"));
    #else
        drvn_status = 2;
        drvn = 15;
        mcSHOW_ERR_MSG(("[Imp Calibration] NO VALID DRVN!!Set to 15.\n"));
    #endif
        
    #ifdef DDR_FT_LOAD_BOARD
        LoadBoardShowResult(FLAG_IMPEDANCE_CALIBRATION, FLAG_CALIBRATION_FAIL, 1, FLAG_NOT_COMPLETE_OR_FAIL);            
        while(1);    
    #endif
    }

    // 4. Set calibration result to output driving
    if (apply == 1)
    {
        if ((drvp_status == 2) && (drvn_status == 2))
        {     
SET_DRIVING:      
#ifdef  MAX_DRIVING 
      drvp = drvn = 0x0f;
#endif
  
	drvn--;
	drvp--;
            //DQS, DQ
            ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DRVCTL0), &u4value);
            mcSET_FIELD(u4value, drvp, MASK_DRVCTL0_DQSDRVP, POS_DRVCTL0_DQSDRVP);
            mcSET_FIELD(u4value, drvn, MASK_DRVCTL0_DQSDRVN, POS_DRVCTL0_DQSDRVN);
            mcSET_FIELD(u4value, drvp, MASK_DRVCTL0_DQDRVP, POS_DRVCTL0_DQDRVP);
            mcSET_FIELD(u4value, drvn, MASK_DRVCTL0_DQDRVN, POS_DRVCTL0_DQDRVN);
            ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DRVCTL0), u4value);
            mcSHOW_DBG_MSG(("[Imp Calibration] Reg: 0x%x, Val: 0x%x\n", DRAMC_REG_DRVCTL0, u4value));

            // CLK, CMD
            ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DRVCTL1), &u4value);
            mcSET_FIELD(u4value, drvp, MASK_DRVCTL1_CLKDRVP, POS_DRVCTL1_CLKDRVP);
            mcSET_FIELD(u4value, drvn, MASK_DRVCTL1_CLKDRVN, POS_DRVCTL1_CLKDRVN);
            mcSET_FIELD(u4value, drvp, MASK_DRVCTL1_CMDDRVP, POS_DRVCTL1_CMDDRVP);
            mcSET_FIELD(u4value, drvn, MASK_DRVCTL1_CMDDRVN, POS_DRVCTL1_CMDDRVN);
            ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DRVCTL1), u4value);
            mcSHOW_DBG_MSG(("[Imp Calibration] Reg: 0x%x, Val: 0x%x\n", DRAMC_REG_DRVCTL1, u4value));           

            // DQ_2, CMD_2
            ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_IODRV4), &u4value);
            mcSET_FIELD(u4value, drvp, MASK_IODRV4_DQDRVP_2, POS_IODRV4_DQDRVP_2);
            mcSET_FIELD(u4value, drvn, MASK_IODRV4_DQDRVN_2, POS_IODRV4_DQDRVN_2);
            mcSET_FIELD(u4value, drvp, MASK_IODRV4_CMDDRVP_2, POS_IODRV4_CMDDRVP_2);
            mcSET_FIELD(u4value, drvn, MASK_IODRV4_CMDDRVN_2, POS_IODRV4_CMDDRVN_2);
            ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_IODRV4), u4value);
            mcSHOW_DBG_MSG(("[Imp Calibration] Reg: 0x%x, Val: 0x%x\n", DRAMC_REG_IODRV4, u4value));
        }
    }
    
    // 5. restore settings
    // R_DMMIOCKCTRLOFF (REG.1DC[26])
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), u4backup_reg);

    // R_DMIMP_CALI_ENP (REG.1C8[5]) = 0
    // R_DMIMP_CALI_ENN (REG.1C8[4]) = 0
    // R_DMIMPCALI_EN (REG.1C8[0]) = 0
    // R_DMIMPDRVP / R_DMIMPDRVN = 0 to avoid current leakage
    ucstatus |= ucDramC_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), &u4value);
    mcCLR_BIT(u4value, POS_IMPCAL_IMP_CALI_ENP);
    mcCLR_BIT(u4value, POS_IMPCAL_IMP_CALI_ENN);
    mcCLR_BIT(u4value, POS_IMPCAL_IMPCALI_EN);
    mcSET_FIELD(u4value, 0, MASK_IMPCAL_IMPDRVP, POS_IMPCAL_IMPDRVP);
    mcSET_FIELD(u4value, 0, MASK_IMPCAL_IMPDRVN, POS_IMPCAL_IMPDRVN);
    ucstatus |= ucDramC_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), u4value);

    // In K2, Reg.100h[24]=1 mean uses Reg.3fch as driving settings also. But Reg.3fch is NAO register which value will be lost when suspend. So keep 0.
    // DRVREF (REG.100[24]) = 1: change will be apply during refresh
    //ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_OCDK), &u4value);
    //mcSET_BIT(u4value, POS_OCDK_DRVREF);
    //ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_OCDK), u4value);

    if (ucstatus)
    {
        mcSHOW_ERR_MSG(("register access fail!\n"));        
    }

    if ((ucstatus != 0) || (drvp_status != 2) || (drvn_status != 2))
    {
        return DRAM_FAIL;
    }
    else
    {
        return DRAM_OK;
    }            
}

//-------------------------------------------------------------------------
/** DramcHwImpedanceCal
 *  start TX OCD impedance calibration.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param  apply           (U8): 0 don't apply the register we set  1 apply the register we set ,default don't apply.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL 
 */
//-------------------------------------------------------------------------
// only for ES validation
DRAM_STATUS_T DramcHwImpedanceCal(DRAMC_CTX_T *p)
{
    U8 ucstatus =0, uctemp;
    U32 u4value;
        
    mcSHOW_DBG_MSG(("[HW Imp Calibr] Start HW impedance calibration...\n"));
            
    // DRVREF (REG.100[24]) = 0: change will be apply directly 
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_OCDK), &u4value);
    mcCLR_BIT(u4value, POS_OCDK_DRVREF);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_OCDK), u4value);
    
    // Set R_DMIMPCAL_HW (REG.1C8[1]) = 1 (HW)
    // Set R_DMIMPCAL_CHKCYCLE (REG.1C8[19][3:2]) = 3
    // Set R_DMIMPCAL_CALEN_CYCLE (REG.1C8[18:16]) = 3
    // Set R_DMIMPCAL_CALICNT (REG.1C8[31:28]) = 7
    // Set R_DMIMPCALCNT (REG.1C8[27:20]) = 0 @ init; 0xf @ run-time
    ucstatus |= ucDramC_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), &u4value);
    mcSET_BIT(u4value, 1);
    mcSET_FIELD(u4value, 0x3, 0x0000000c, 2);
    mcSET_FIELD(u4value, 0x3, 0x00070000, 16);
    mcSET_FIELD(u4value, 0x7, 0xf0000000, 28);
    mcSET_FIELD(u4value, 0x0, 0x0ff00000, 20);
    ucstatus |= ucDramC_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), u4value);

    // Set R_DMIMPCALI_EN (REG.1C8[0]) = 1
    ucstatus |= ucDramC_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), &u4value);
    mcSET_BIT(u4value, 0);
    ucstatus |= ucDramC_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), u4value);
            
    // Check calibrated result
    // only show in drvp_status in A60808, not sure if HW has filled into driving (Rome will have registers to read)
    // ~1.5us
    mcDELAY_US(5);

    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DLLSTATUS0), &u4value);
    uctemp = (U8) mcGET_FIELD(u4value, 0x3c000000, 26);
    mcSHOW_DBG_MSG(("[HW Imp Calibr] drvp_save  : %d\n",uctemp));
    uctemp = (U8) mcGET_FIELD(u4value, 0x03c00000, 22);
    mcSHOW_DBG_MSG(("[HW Imp Calibr] drvn_save  : %d\n",uctemp));
    uctemp = (U8) mcGET_FIELD(u4value, 0x003c0000, 18);
    mcSHOW_DBG_MSG(("[HW Imp Calibr] drvp_save_2: %d\n",uctemp));
    uctemp = (U8) mcGET_FIELD(u4value, 0x0003c000, 14);
    mcSHOW_DBG_MSG(("[HW Imp Calibr] drvp_save_2: %d\n",uctemp));
        
    // restore settings
    // R_DMIMPCALI_EN (REG.1C8[0]) = 0
    ucstatus |= ucDramC_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), &u4value);
    mcCLR_BIT(u4value, POS_IMPCAL_IMPCALI_EN);
    ucstatus |= ucDramC_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_IMPCAL), u4value);

    // DRVREF (REG.100[24]) = 1: change will be apply during refresh
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_OCDK), &u4value);
    mcSET_BIT(u4value, POS_OCDK_DRVREF);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_OCDK), u4value);

    if (ucstatus)
    {
        mcSHOW_ERR_MSG(("register access fail!\n"));        
    }

    if (ucstatus != 0)
    {
        return DRAM_FAIL;
    }
    else
    {
        return DRAM_OK;
    }            
}


//-------------------------------------------------------------------------
/** DramcCATraining
 *  start the calibrate the skew between Clk pin and CAx pins.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL 
 */
//-------------------------------------------------------------------------
S32 CheckCATrainingTransition(U32 uiCA, U32 capattern, U32 uiRisingEdge, U32 uiFallingEdge)
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

DRAM_STATUS_T DramcCATraining_1(DRAMC_CTX_T *p, U32 capattern, S8 *iFirstClkPass, S8 *iLastClkPass, S8 *iFirstCAPass, S8 *iLastCAPass)
{
    U8 ucstatus = 0;
	U32 u4prv_register_0c4;
    U32 uiTemp, uiDelay, uiFinishCount, uiCA, uiMR41=1, uiReg1DCh, uiReg1E0h, uiRisingEdge, uiFallingEdge;
    U32 u4prv_register_0fc, u4prv_register_044, u4prv_register_63c;
    
    U32 u4prv_register_1a8, u4prv_register_1ac, u4prv_register_1b0, u4prv_register_1b8, u4prv_register_1c4, u4prv_register_0c;
    
    S8 iCenter[CATRAINING_NUM];
    S8 iMaxCenter;
    S8 iCAShift[CATRAINING_NUM];
#ifdef CKE_CS_DLY_SETTING
    S8 CAShift_Avg = 0;
#endif
    S8 iBestFirstClkPass[CATRAINING_NUM], iBestLastClkPass[CATRAINING_NUM];
    S8 iBestFirstCAPass[CATRAINING_NUM], iBestLastCAPass[CATRAINING_NUM];
    S32 iPass, iClkWinSize, iCAWinSize;
    U32 *uiLPDDR_PHY_Mapping = NULL;
    U32 *uiLPDDR_CA_Mapping;
    U32 i;
    U8  DUAL_FREQ_HIGH_CLKDLY;

    
    // error handling
    if (!p)
    {
        mcSHOW_ERR_MSG(("context is NULL\n"));
        return DRAM_FAIL;
    }

    if (p->dram_type != TYPE_LPDDR3)
    {
        mcSHOW_ERR_MSG(("Wrong DRAM TYPE. Only support LPDDR3 in CA training!!\n"));
        return DRAM_FAIL;
    }

#ifdef SPM_WORKAROUND_CATRAINING
    if (p->frequency == DUAL_FREQ_LOW)
    {
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x0c), &DUAL_FREQ_HIGH_reg_00c);
        DUAL_FREQ_HIGH_CLKDLY = (DUAL_FREQ_HIGH_reg_00c >> 24) & 0x0f;

        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1a8), &DUAL_FREQ_HIGH_reg_1a8);
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1ac), &DUAL_FREQ_HIGH_reg_1ac);
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1b0), &DUAL_FREQ_HIGH_reg_1b0);
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1b8), &DUAL_FREQ_HIGH_reg_1b8);
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1c4), &DUAL_FREQ_HIGH_reg_1c4);
    }
#endif

    // backup registers
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x0c), &u4prv_register_0c);   
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1a8), &u4prv_register_1a8);
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1ac), &u4prv_register_1ac);
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1b0), &u4prv_register_1b0);
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1b8), &u4prv_register_1b8);
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1c4), &u4prv_register_1c4);


    // Disable clock gating to prevent DDRPHY enter idle.
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x63c), &uiTemp);
    u4prv_register_63c = uiTemp;
    mcCLR_BIT(uiTemp, 2);
    mcCLR_BIT(uiTemp, 1);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x63c), uiTemp);

    // Edward : no idea why TXP>1 will cause CA training fail. Now set it after CA training.
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xfc), &u4prv_register_0fc);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xfc), (u4prv_register_0fc & 0x8fffffff));

#if fcFOR_CHIP_ID == fcK2
    //disable auto refresh: REFCNT_FR_CLK = 0 (0x1dc[23:16]), ADVREFEN = 0 (0x44[30])
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), &uiTemp);
    u4prv_register_044 = uiTemp;
    mcCLR_BIT(uiTemp, POS_TEST2_3_ADVREFEN);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), uiTemp);
#endif	

#if fcFOR_CHIP_ID == fcJADE
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xc4), &uiTemp);
    u4prv_register_0c4 = uiTemp;
    mcSET_BIT(uiTemp, 0);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xc4), uiTemp);
#endif

    if (p->channel == CHANNEL_A)
    {
        uiLPDDR_PHY_Mapping = (U32 *)uiLPDDR3_PHY_Mapping_POP_CHA;
        uiLPDDR_CA_Mapping = (U32 *)uiLPDDR3_CA_Mapping_CHA;
    }


    for (uiCA=0; uiCA<CATRAINING_NUM; uiCA++) 
    {
        iLastClkPass[uiCA] = iLastCAPass[uiCA] = -1;
        iFirstClkPass[uiCA] = iFirstCAPass[uiCA] = -1;
    }
    
    // Sweep clock output delay first.
       
    // Keep DQ input always ON.
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xd8), &uiTemp);
    uiTemp |= 0x0000f000;
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xd8), uiTemp);

    // Let MIO_CK always ON.
    // Disable auto refresh: REFCNT_FR_CLK = 0 (0x1dc[23:16])
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1dc), &uiTemp);
    uiReg1DCh = uiTemp;
    mcSET_BIT(uiTemp, 26);
#if fcFOR_CHIP_ID == fcK2	
    mcCLR_MASK(uiTemp, MASK_DRAMC_PD_CTRL_REFCNT_FR_CLK);
#endif
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1dc), uiTemp);

#if 0
    //FIXDQIEN = 1111 (0xd8[15:12])
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MCKDLY), &uiTemp);
    uiRegD8h = uiTemp;
    mcSET_MASK(uiTemp, MASK_MCKDLY_FIXDQIEN);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MCKDLY), uiTemp);
#endif

    //Enable DQ_O1, SELO1ASO=1
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_LPDDR2), &uiTemp);
    uiReg1E0h = uiTemp;
    mcSET_BIT(uiTemp, POS_LPDDR2_SELO1ASO);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_LPDDR2), uiTemp);

CA_TRAINING_BEGIN:

    // Set CA0~CA3, CA5~CA8 output delay to 0.
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1a8), 0);    // CA0~CA3
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1ac), 0);    // CA4~CA7
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1b0), 0);    // CA8~CA11
    
    // CS extent enable (need DRAM to support)
    // for testing

    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x80), &uiTemp);
    mcSET_BIT(uiTemp, 13);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x80), uiTemp);


    // Enter MR 41/MR48
    // Set MA & OP.
    if (uiMR41) 
    {
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), 0x00a40029);    
    } 
    else     
    {
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), 0x00c00030);    
    }
    // Hold the CA bus stable for at least one cycle.
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x4c), &uiTemp);
    mcSET_BIT(uiTemp, 2);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x4c), uiTemp);
    // MRW
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1e4), &uiTemp);
    mcSET_BIT(uiTemp, 0);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), uiTemp);
    mcDELAY_US(1);
    mcCLR_BIT(uiTemp, 0);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), uiTemp);
    // Disable CA bus stable.
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x4c), &uiTemp);
    mcCLR_BIT(uiTemp, 2);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x4c), uiTemp);

    // Wait tCACKEL(10 tck) before CKE low
    mcDELAY_US(1);

    // CKE low
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xe4), &uiTemp);
    mcSET_BIT(uiTemp, 3);
    mcCLR_BIT(uiTemp, 2);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xe4), uiTemp);
    
    // Set CA0~CA3, CA5~CA8 rising/falling golden value.
    if (uiMR41) 
    {
        //  01010101b -> 10101010b : Golden value = 1001100110011001b=0x9999
        //  11111111b -> 00000000b : Golden value = 0101010101010101b=0x5555
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x50), capattern);
    }
    else
    {
        //  00010001b -> 00000000b : Golden value = 0000000100000001b=0x0101
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x50), capattern);
    }

    // Wait tCAENT(10 tck) before pattern output
    mcDELAY_US(1);

    // Delay clock output delay to do CA training in order to get the pass window.
    uiFinishCount = 0;
    for (uiDelay=0; uiDelay<=MAX_CLKO_DELAY; uiDelay++) 
    {     
        //DramcEnterSelfRefresh(p, 1);           
        // Set Clk output delay
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x0c), &uiTemp);
        mcSET_FIELD(uiTemp, uiDelay, 0x0f000000, 24);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x0c), uiTemp);    
        //DramcEnterSelfRefresh(p, 0);
        
        // CA training pattern output enable
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x4c), &uiTemp);
        mcSET_BIT(uiTemp, 1);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x4c), uiTemp);
        // delay 2 DRAM clock cycle
        mcDELAY_US(1);
        mcCLR_BIT(uiTemp, 1);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x4c), uiTemp);    
        
        // Wait tADR(20ns) before CA sampled values available in DQ.
        mcDELAY_US(1);

        // Get DQ value.
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x380), &uiTemp);

        //mcSHOW_DBG_MSG(("[CA Training] CLK delay -- %d, MR41 -- %d, DQ_O1 -- 0x%x\n", uiDelay, uiMR41, uiTemp));

        // Compare with golden value.
        if (uiMR41) 
        {
            for (uiCA=0; uiCA<CATRAINING_NUM; uiCA++) 
            {
                if ((uiCA==4) || (uiCA==9)) 
                {
                    continue;
                }
                if ( (iFirstClkPass[uiCA]==-1) || (iLastClkPass[uiCA]==-1)) 
                {
                    if (uiCA<4) 
                    {
                        uiRisingEdge = uiTemp & (0x01 << uiLPDDR_PHY_Mapping[uiCA<<1]);
                        uiFallingEdge = uiTemp & (0x01 << uiLPDDR_PHY_Mapping[(uiCA<<1)+1]); 
                    }
                    else
                    {
                        uiRisingEdge = uiTemp & (0x01 << uiLPDDR_PHY_Mapping[(uiCA-1)<<1]);
                        uiFallingEdge = uiTemp & (0x01 << uiLPDDR_PHY_Mapping[((uiCA-1)<<1)+1]); 
                    }

                    // Check 1->0 transition.
                    iPass = CheckCATrainingTransition(uiCA, capattern, uiRisingEdge, uiFallingEdge);


                    if (iFirstClkPass[uiCA]==-1)
                    {
                        if (iPass == 1) 
                        {
                            iFirstClkPass[uiCA] = uiDelay;
                        }
                    }
                    else
                    {
                        if (iLastClkPass[uiCA]==-1)
                        {                    
                            if (iPass == 0) 
                            {
                                iLastClkPass[uiCA] = uiDelay-1;
                                uiFinishCount++;
                            }
                            else
                            {
                                if (uiDelay==MAX_CLKO_DELAY)
                                {
                                    iLastClkPass[uiCA] = uiDelay;
                                    uiFinishCount++;
                                }
                            }
                        }
                    }
                }
            }

            // Wait tCACD(22clk) before output CA pattern to DDR again..
            mcDELAY_US(1);        
            
            //if (uiFinishCount==8) {
            //    break;
            //} 
        }
        else
        {
            // MR 48 
            uiCA = 4;
            
MR48_CHECKRESULT_CLK:            
    
            if ((iFirstClkPass[uiCA]==-1) || (iLastClkPass[uiCA]==-1))
            {
                uiRisingEdge = uiTemp & (0x01 << uiLPDDR_PHY_Mapping[(uiCA==4) ? 0 : 8]);
                uiFallingEdge = uiTemp & (0x01 << uiLPDDR_PHY_Mapping[(uiCA==4) ? 1 : 9]); 

                // Check 1->0 transition.
                iPass = CheckCATrainingTransition(uiCA, capattern, uiRisingEdge, uiFallingEdge);

                if (iFirstClkPass[uiCA]==-1)
                {
                    if (iPass==1)
                    {
                        iFirstClkPass[uiCA] = uiDelay;
                    }
                }
                else
                {
                    if (iLastClkPass[uiCA]==-1)
                    {
                        if (iPass==0)
                        {
                            iLastClkPass[uiCA] = uiDelay-1;
                            uiFinishCount++;
                        }
                        else
                        {
                            if (uiDelay==MAX_CLKO_DELAY)
                            {
                                iLastClkPass[uiCA] = uiDelay;
                                uiFinishCount++;                            
                            }
                        }                    
                    }
                }
            }

            if (uiCA==4)
            {
                uiCA=9;
                goto MR48_CHECKRESULT_CLK;
            }
            
            // Wait tCACD(22clk) before output CA pattern to DDR again..
            mcDELAY_US(1);                    

            //if (uiFinishCount==2) {
            //    break;
            //} 
        }
    }

    //DramcEnterSelfRefresh(p, 1);    
    // Set Clk output delay to 0.
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x0c), &uiTemp);
    mcSET_FIELD(uiTemp, 0, 0x0f000000, 24);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x0c), uiTemp);  
    //DramcEnterSelfRefresh(p, 0);

    // Delay CA output delay to do CA training in order to get the pass window.
    uiFinishCount = 0;
    for (uiDelay=0; uiDelay<=MAX_CLKO_DELAY; uiDelay++) 
    {
        // Set CA0~CA3, CA5~CA8 output delay.
        uiTemp = uiDelay | (uiDelay<<8) | (uiDelay<<16) | (uiDelay<<24);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1a8), uiTemp);    // CA0~CA3
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1ac), uiTemp);    // CA4~CA7
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1b0), uiTemp);    // CA8~CA11
        
        // CA training pattern output enable
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x4c), &uiTemp);
        mcSET_BIT(uiTemp, 1);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x4c), uiTemp);
        // delay 2 DRAM clock cycle
        mcDELAY_US(1);
        mcCLR_BIT(uiTemp, 1);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x4c), uiTemp);    
        
        // Wait tADR(20ns) before CA sampled values available in DQ.
        mcDELAY_US(1);

        // Get DQ value.
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x380), &uiTemp);

        //mcSHOW_DBG_MSG(("[CA Training] CA delay -- %d, MR41 -- %d, DQ_O1 -- 0x%x\n", uiDelay, uiMR41, uiTemp));

        // Compare with golden value.
        if (uiMR41) 
        {
            for (uiCA=0; uiCA<CATRAINING_NUM; uiCA++) 
            {
                if ((uiCA==4) || (uiCA==9)) 
                {
                    continue;
                }
                if ( (iFirstCAPass[uiCA]==-1) || (iLastCAPass[uiCA]==-1)) 
                {
                    if (uiCA<4) 
                    {
                        uiRisingEdge = uiTemp & (0x01 << uiLPDDR_PHY_Mapping[uiCA<<1]);
                        uiFallingEdge = uiTemp & (0x01 << uiLPDDR_PHY_Mapping[(uiCA<<1)+1]); 
                    }
                    else
                    {
                        uiRisingEdge = uiTemp & (0x01 << uiLPDDR_PHY_Mapping[(uiCA-1)<<1]);
                        uiFallingEdge = uiTemp & (0x01 << uiLPDDR_PHY_Mapping[((uiCA-1)<<1)+1]); 
                    }
                    // Check 1->0 transition.
                    iPass = CheckCATrainingTransition(uiCA, capattern, uiRisingEdge, uiFallingEdge);


                    if (iFirstCAPass[uiCA]==-1)
                    {
                        if (iPass == 1) 
                        {
                            iFirstCAPass[uiCA] = uiDelay;
                        }
                    }
                    else
                    {
                        if (iLastCAPass[uiCA]==-1)
                        {                    
                            if (iPass == 0) 
                            {
                                iLastCAPass[uiCA] = uiDelay-1;
                                uiFinishCount++;
                            }
                            else
                            {
                                if (uiDelay==MAX_CLKO_DELAY)
                                {
                                    iLastCAPass[uiCA] = uiDelay;
                                    uiFinishCount++;
                                }
                            }
                        }
                    }
                }
            }
        
            // Wait tCACD(22clk) before output CA pattern to DDR again..
            mcDELAY_US(1);        
            
            //if (uiFinishCount==8) {
            //    break;
            //} 
        }
        else
        {
            // MR 48
            uiCA = 4;
            
MR48_CHECKRESULT_CA:            
    
            if ((iFirstCAPass[uiCA]==-1) || (iLastCAPass[uiCA]==-1))
            {
                uiRisingEdge = uiTemp & (0x01 << uiLPDDR_PHY_Mapping[(uiCA==4) ? 0 : 8]);
                uiFallingEdge = uiTemp & (0x01 << uiLPDDR_PHY_Mapping[(uiCA==4) ? 1 : 9]); 

                // Check 1->0 transition.
                iPass = CheckCATrainingTransition(uiCA, capattern, uiRisingEdge, uiFallingEdge);

        
                if (iFirstCAPass[uiCA]==-1)
                {
                    if (iPass==1)
                    {
                        iFirstCAPass[uiCA] = uiDelay;
                    }
                }
                else
                {
                    if (iLastCAPass[uiCA]==-1)
                    {
                        if (iPass==0)
                        {
                            iLastCAPass[uiCA] = uiDelay-1;
                            uiFinishCount++;
                        }
                        else
                        {
                            if (uiDelay==MAX_CLKO_DELAY)
                            {
                                iLastCAPass[uiCA] = uiDelay;
                                uiFinishCount++;                            
                            }
                        }                      
                    }
                }
            }

            if (uiCA==4)
            {
                uiCA=9;
                goto MR48_CHECKRESULT_CA;
            }

            // Wait tCACD(22clk) before output CA pattern to DDR again..
            mcDELAY_US(1);                    

            //if (uiFinishCount==2) {
            //    break;
            //}             
        }
    }    

    // CS extent disable
    // for testing
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x80), &uiTemp);
    mcCLR_BIT(uiTemp, 13);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x80), uiTemp);   

    if (uiMR41==0) 
    {
        // Disable fix DQ input enable.
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xd8), &uiTemp);
        uiTemp = uiTemp & 0xffff0fff;
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xd8), uiTemp);
    }


    //dramc clk gating always no gating
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1dc), &uiTemp);
    mcSET_BIT(uiTemp, 26);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1dc), uiTemp);
    mcDELAY_US(1);


    // Wait tCACKEN (10ck)
    mcDELAY_US(1);        

    // CKE high
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xe4), &uiTemp);
    mcCLR_BIT(uiTemp, 3);
    mcSET_BIT(uiTemp, 2);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xe4), uiTemp);

    if (uiMR41) 
    {
        uiMR41 = 0;
        goto CA_TRAINING_BEGIN;
    }

    // CS extent enable
    // for testing
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x80), &uiTemp);
    mcSET_BIT(uiTemp, 13);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x80), uiTemp); 

    // MR42 to leave CA training.
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x88), 0x00a8002a);    
    // Hold the CA bus stable for at least one cycle.
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x4c), &uiTemp);
    mcSET_BIT(uiTemp, 2);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x4c), uiTemp);
    // MRW
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1e4), &uiTemp);
    mcSET_BIT(uiTemp, 0);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), uiTemp);
    mcDELAY_US(1);
    mcCLR_BIT(uiTemp, 0);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1e4), uiTemp);    
    // Disable the hold the CA bus stable for at least one cycle.
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x4c), &uiTemp);
    mcCLR_BIT(uiTemp, 2);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x4c), uiTemp);

    // CS extent disable
    // for testing
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x80), &uiTemp);
    mcCLR_BIT(uiTemp, 13);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x80), uiTemp);   
    
    // Disable CKE high
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xe4), &uiTemp);
    mcCLR_BIT(uiTemp, 2);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xe4), uiTemp);


    //dramc clk gating controlled by dramc
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1dc), &uiTemp);
    mcCLR_BIT(uiTemp, 26);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1dc), uiTemp);



    // Restore the registers' values.
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1dc), uiReg1DCh);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_LPDDR2), uiReg1E0h);
    //ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MCKDLY), uiRegD8h);


    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xfc), u4prv_register_0fc);
#if fcFOR_CHIP_ID == fcK2	
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), u4prv_register_044);
#endif
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x63c), u4prv_register_63c);
#if fcFOR_CHIP_ID == fcJADE
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xc4), u4prv_register_0c4);		
#endif

    // restore registers
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x0c), u4prv_register_0c);   
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1a8), u4prv_register_1a8);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1ac), u4prv_register_1ac);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1b0), u4prv_register_1b0);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1b8), u4prv_register_1b8);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1c4), u4prv_register_1c4);


    mcSHOW_DBG_MSG4(("\n"));
    for (uiCA=0; uiCA<CATRAINING_NUM; uiCA++)
    {
        mcSHOW_DBG_MSG4(("[DramcCATraining_1] CA%d     CA(%d~%d) Clk(%d~%d) \n", uiCA, iLastCAPass[uiCA], iFirstClkPass[uiCA], iFirstClkPass[uiCA], iLastClkPass[uiCA]));

    }
    mcSHOW_DBG_MSG4(("\n"));    



    if (ucstatus)
    {
        mcSHOW_ERR_MSG(("register access fail!\n"));
        return DRAM_FAIL;
    }
    else
    {
        return DRAM_OK;
    }    
}

void DramcCATraining_2(S8 *iFirstClkPass, S8 *iLastClkPass, S8 *iFirstCAPass, S8 *iLastCAPass, S8 *iFirstClkPass2, S8 *iLastClkPass2, S8 *iFirstCAPass2, S8 *iLastCAPass2, S8 *iMaxCenter)
{
    U32 uiCA;
    U32 i;
    S8 minclk_val, maxclk_val, minca_val, maxca_val;

    for (uiCA=0; uiCA<CATRAINING_NUM; uiCA++)
    {
        if (iLastCAPass[uiCA] > iLastCAPass2[uiCA]) iLastCAPass[uiCA] = iLastCAPass2[uiCA];
        if (iFirstCAPass[uiCA] < iFirstCAPass2[uiCA]) iFirstCAPass[uiCA] = iFirstCAPass2[uiCA];        
        
        if (iFirstClkPass[uiCA] < iFirstClkPass2[uiCA]) iFirstClkPass[uiCA] = iFirstClkPass2[uiCA];
        if (iLastClkPass[uiCA] > iLastClkPass2[uiCA]) iLastClkPass[uiCA] = iLastClkPass2[uiCA];
    }

#if 0
    minca_val = MAX_CLKO_DELAY;
    maxca_val = MAX_CLKO_DELAY;        
    minclk_val = MAX_CLKO_DELAY;
    maxclk_val = MAX_CLKO_DELAY;
        
    for (uiCA=0; uiCA<CATRAINING_NUM; uiCA++)
    {
        if (iLastCAPass[uiCA] < maxca_val) maxca_val = iLastCAPass[uiCA];        
        if (iFirstCAPass[uiCA] < minca_val) minca_val = iFirstCAPass[uiCA];        
        
        if (iFirstClkPass[uiCA] < minclk_val) minclk_val = iFirstClkPass[uiCA];
        if (iLastClkPass[uiCA] < maxclk_val) maxclk_val = iLastClkPass[uiCA];
    }
#endif    


    mcSHOW_DBG_MSG4(("\n"));
    for (uiCA=0; uiCA<CATRAINING_NUM; uiCA++)
    {
        mcSHOW_DBG_MSG4(("[DramcCATraining_2111] CA%d     CA(%d~%d) Clk(%d~%d) %d\n", uiCA, iLastCAPass[uiCA], iFirstClkPass[uiCA], iFirstClkPass[uiCA], iLastClkPass[uiCA], *iMaxCenter));
    
    }
    mcSHOW_DBG_MSG4(("\n"));


#if 0
    for (uiCA=0; uiCA<CATRAINING_NUM; uiCA++)
    {
        iLastCAPass[uiCA] = maxca_val;
        iFirstCAPass[uiCA] = minca_val;
        
        iFirstClkPass[uiCA] = minclk_val;
        iLastClkPass[uiCA] = maxclk_val;
    }        


    *iMaxCenter = ((maxclk_val + maxca_val + 1) / 2) - (maxca_val + 1);
    if (*iMaxCenter < 0) *iMaxCenter = 0;

mcSHOW_DBG_MSG4(("\n"));
for (uiCA=0; uiCA<CATRAINING_NUM; uiCA++)
{
    mcSHOW_DBG_MSG4(("[DramcCATraining_2] CA%d     CA(%d~%d) Clk(%d~%d) %d\n", uiCA, iLastCAPass[uiCA], iFirstClkPass[uiCA], iFirstClkPass[uiCA], iLastClkPass[uiCA], *iMaxCenter));

}
mcSHOW_DBG_MSG4(("\n"));
#endif


}

void DramcCATraining_3(DRAMC_CTX_T *p, S8 *iFirstClkPass, S8 *iLastClkPass, S8 *iFirstCAPass, S8 *iLastCAPass, S8 *iCAShift, U8 update_CA_flag)
{
    U8 ucstatus = 0;
	U32 u4prv_register_0c4;
    U32 uiTemp, uiDelay, uiFinishCount, uiCA, uiMR41=1, uiReg1DCh, uiReg1E0h, uiRisingEdge, uiFallingEdge;
    U32 u4prv_register_0fc, u4prv_register_044, u4prv_register_63c;
    
    S8 iCenter[CATRAINING_NUM];
    S8 iMaxCenter;

#ifdef CKE_CS_DLY_SETTING
    S8 CAShift_Avg = 0;
#endif
    S8 iBestFirstClkPass[CATRAINING_NUM], iBestLastClkPass[CATRAINING_NUM];
    S8 iBestFirstCAPass[CATRAINING_NUM], iBestLastCAPass[CATRAINING_NUM];
    S32 iPass, iClkWinSize, iCAWinSize;
    U32 *uiLPDDR_PHY_Mapping;
    U32 *uiLPDDR_CA_Mapping;
    U32 i;
    U8  DUAL_FREQ_HIGH_CLKDLY;

    
    // error handling
    if (!p)
    {
        mcSHOW_ERR_MSG(("context is NULL\n"));
        return;
    }

    if (p->dram_type != TYPE_LPDDR3)
    {
        mcSHOW_ERR_MSG(("Wrong DRAM TYPE. Only support LPDDR3 in CA training!!\n"));
        return;
    }

    if (p->channel == CHANNEL_A)
    {
        uiLPDDR_PHY_Mapping = (U32 *)uiLPDDR3_PHY_Mapping_POP_CHA;
        uiLPDDR_CA_Mapping = (U32 *)uiLPDDR3_CA_Mapping_CHA;
    }


    for (uiCA=0; uiCA<CATRAINING_NUM; uiCA++) 
    {
        iBestLastClkPass[uiCA] = iBestLastCAPass[uiCA] = -1;
        iBestFirstClkPass[uiCA] = iBestFirstCAPass[uiCA] = -1;        
    }
    

    // Delay clock output delay to do CA training in order to get the pass window.
    uiFinishCount = 0;
    for (uiDelay=0; uiDelay<=MAX_CLKO_DELAY; uiDelay++) 
    {     

            for (uiCA=0; uiCA<CATRAINING_NUM; uiCA++) 
            {
                if ( (iLastClkPass[uiCA]-iFirstClkPass[uiCA]) > (iBestLastClkPass[uiCA]-iBestFirstClkPass[uiCA]))
                {
                  iBestLastClkPass[uiCA] = iLastClkPass[uiCA];
                  iBestFirstClkPass[uiCA] = iFirstClkPass[uiCA];
                }  

                if ( (iLastCAPass[uiCA]-iFirstCAPass[uiCA]) > (iBestLastCAPass[uiCA]-iBestFirstCAPass[uiCA]) )
                {
                    iBestLastCAPass[uiCA] = iLastCAPass[uiCA];
                    iBestFirstCAPass[uiCA] = iFirstCAPass[uiCA];
                }                
            }       
    }


    // Calculate the middle range & max middle.

#ifndef RELEASE            
    mcSHOW_DBG_MSG4(("=========================================\n"));
    mcSHOW_DBG_MSG4(("1. [Channel %d] CA training window before adjustment. \n", p->channel));
    mcSHOW_DBG_MSG4(("x=Pass window CA(max~min) Clk(min~max) center. \n"));
    mcSHOW_DBG_MSG4(("y=CA0~CA9\n"));
    mcSHOW_DBG_MSG4(("=========================================\n"));
#else
    mcSHOW_USER_MSG(("\nCA\n"));
#endif

    iMaxCenter = 0 - MAX_CLKO_DELAY;
    for (uiCA=0; uiCA<CATRAINING_NUM; uiCA++)
    {
        iClkWinSize = iBestLastClkPass[uiCA] - iBestFirstClkPass[uiCA];
        iCAWinSize = iBestLastCAPass[uiCA] - iBestFirstCAPass[uiCA];
        if (iClkWinSize >= iCAWinSize)
        {
            if (iCAWinSize>0)
            {
                iCenter[uiCA] =  (iClkWinSize - iCAWinSize)>>1;
            }
            else
            {
                iCenter[uiCA] =  iBestFirstClkPass[uiCA] + (iClkWinSize>>1);
            }
        }
        else
        {
            if (iClkWinSize>0)
            {
                iCenter[uiCA] =  (iClkWinSize - iCAWinSize)/2;
            }
            else
            {
                iCenter[uiCA] =  0-(iBestFirstCAPass[uiCA] + (iCAWinSize>>1));
            }
        }
    
#ifndef RELEASE    
        mcSHOW_DBG_MSG4(("CA%d     CA(%d~%d) Clk(%d~%d) %d\n", uiCA, iBestLastCAPass[uiCA],  iBestFirstCAPass[uiCA],
            iBestFirstClkPass[uiCA], iBestLastClkPass[uiCA], iCenter[uiCA]));
#else
        mcSHOW_USER_MSG(("%d     (%d~%d) (%d~%d) %d\n", uiCA, iBestLastCAPass[uiCA],  iBestFirstCAPass[uiCA],
            iBestFirstClkPass[uiCA], iBestLastClkPass[uiCA], iCenter[uiCA]));
#endif    

        if (iCenter[uiCA]  > iMaxCenter)
        {
            iMaxCenter = iCenter[uiCA];
        }
#ifdef EYE_SCAN
  EyeScanWin[uiCA].ucsetup_pass_number = iCAWinSize;
  EyeScanWin[uiCA].uchold_pass_number = iClkWinSize;
#endif

    #ifdef DDR_FT_LOAD_BOARD
        if ((iCAWinSize==0) && (iClkWinSize == 0))
        {
            LoadBoardShowResult(FLAG_CA_CALIBRATION, FLAG_CALIBRATION_FAIL, p->channel, FLAG_NOT_COMPLETE_OR_FAIL);            
            while(1);    
        }
        else if ((iCAWinSize+iClkWinSize)<=CA_TRAINING_BOUND)
        {
            LoadBoardShowResult(FLAG_CA_CALIBRATION, FLAG_WINDOW_TOO_SMALL, p->channel, FLAG_NOT_COMPLETE_OR_FAIL);            
            while(1);  
        }
    #endif
    }
    mcSHOW_DBG_MSG4(("=========================================\n"));
    mcSHOW_DBG_MSG4(("Max center = %d\n\n", iMaxCenter));

    // Calculate the shift value
    mcSHOW_DBG_MSG4(("=========================================\n"));
    mcSHOW_DBG_MSG4(("2. [Channel %d]CA training window after adjustment.\n", p->channel));
    mcSHOW_DBG_MSG4(("x=CA shift     Adjusted Pass window CA(max~min) Clk(min~max) center.\n"));
    mcSHOW_DBG_MSG4(("y=CA0~CA9\n"));
    mcSHOW_DBG_MSG4(("=========================================\n"));    

    if (iMaxCenter < 0)
    {
        // Clk output delay could not be negative. Need to adjust into 0.
        iMaxCenter = 0;
        mcSHOW_DBG_MSG4(("Max center < 0. Adjust to 0. \n\n"));
    }

    for (uiCA=0; uiCA<CATRAINING_NUM; uiCA++)
    {
        iCAShift[uiCA] = iMaxCenter - iCenter[uiCA]+CATRAINING_STEP;
        if (iCAShift[uiCA]>=MAX_CLKO_DELAY)
        {
            iCAShift[uiCA] = MAX_CLKO_DELAY;
        }

        if (iCAShift[uiCA]<0) iCAShift[uiCA] = 0;



        
#ifdef CKE_CS_DLY_SETTING
        CAShift_Avg += iCAShift[uiCA];
#endif  
        mcSHOW_DBG_MSG4(("CA%d     Shift %d     CA(%d~%d) Clk(%d~%d) %d\n", uiCA, iCAShift[uiCA], 
            iBestLastCAPass[uiCA]-iCAShift[uiCA], iBestFirstCAPass[uiCA],  
            iBestFirstClkPass[uiCA], iBestLastClkPass[uiCA]+iCAShift[uiCA], iCenter[uiCA]+iCAShift[uiCA]));
    }
    mcSHOW_DBG_MSG4(("=========================================\n"));    


if (update_CA_flag == 1)
{
    // Write shift value into CA output delay.
    uiTemp = 0;
    for(i=0;i<=3;i++)
    {
        uiTemp |= (U32)(iCAShift[uiLPDDR_CA_Mapping[i]]) << (8*i);
    }
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1a8), uiTemp);    // CA0~CA3
    mcSHOW_DBG_MSG4(("Reg.1A8h=%xh\n", uiTemp));    

    uiTemp = 0;
    for(i=4;i<=7;i++)
    {
        uiTemp |= (U32)(iCAShift[uiLPDDR_CA_Mapping[i]]) << (8*(i-4));
    }    
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1ac), uiTemp);    // CA4~CA7
    mcSHOW_DBG_MSG4(("Reg.1ACh=%xh\n", uiTemp));    

    uiTemp = 0;
    for(i=8;i<=9;i++)
    {
        uiTemp |= (U32)(iCAShift[uiLPDDR_CA_Mapping[i]]) << (8*(i-8));
    }
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1b0), uiTemp);    // CA8~CA11
    mcSHOW_DBG_MSG4(("Reg.1B0h=%xh\n", uiTemp));           

#ifdef CKE_CS_DLY_SETTING
    CAShift_Avg = (CAShift_Avg + (CATRAINING_NUM>>1)) /CATRAINING_NUM;
    // CKEDLY : Reg.1B8h[12:8].  CSDLY : Reg.1B8h[4:0]
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1b8), &uiTemp);
    uiTemp = (uiTemp & 0xffffe0e0) | (CAShift_Avg << 8) | (CAShift_Avg << 0);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1b8), uiTemp);
    mcSHOW_DBG_MSG4(("Reg.1B8h=%xh\n", uiTemp));    

#if 0
    // CKE1DLY : Reg.1C4h[28:24]
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1c4), &uiTemp);
    uiTemp = (uiTemp & 0xe0ffffff) | (CAShift_Avg << 24);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1c4), uiTemp);
    mcSHOW_DBG_MSG4(("Reg.1C4h=%xh\n", uiTemp));    
#else
    // CKE1DLY->RASDLY : Reg.1b8h[20:16]
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1b8), &uiTemp);
    uiTemp = (uiTemp & 0xffe0ffff) | (CAShift_Avg << 16);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1b8), uiTemp);
    mcSHOW_DBG_MSG4(("Reg.1B8h=%xh\n", uiTemp));    
#endif

    // CS1DLY : Reg.0Ch[31:28]
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x0c), &uiTemp);
    uiTemp = (uiTemp & 0x0fffffff) | (CAShift_Avg << 28);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x0c), uiTemp);
    mcSHOW_DBG_MSG4(("Reg.00ch=%xh\n", uiTemp));    
#endif    

    DramcEnterSelfRefresh(p, 1);    
    // Write max center value into Clk output delay.
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x0c), &uiTemp);
    mcSET_FIELD(uiTemp, iMaxCenter+CATRAINING_STEP, 0x0f000000, 24);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x0c), uiTemp);    
    mcSHOW_DBG_MSG4(("Reg.0Ch=%xh\n", uiTemp));    
    DramcEnterSelfRefresh(p, 0);

    CATrain_ClkDelay[p->channel] = iMaxCenter;
    mcSHOW_DBG_MSG4(("CATrain_ClkDelay=%d...\n", CATrain_ClkDelay[p->channel]));
}
  
}

void DramcCATraining_4(DRAMC_CTX_T *p, S8 *iFirstClkPass, S8 *iLastClkPass, S8 *iFirstCAPass, S8 *iLastCAPass, S8 *iCAShift, S8 *iFirstClkPass2, S8 *iLastClkPass2, S8 *iFirstCAPass2, S8 *iLastCAPass2, U8 update_CA_flag)
{
    U32 uiCA;
    S8 iMaxCenter = 0;
    U32 i, uiTemp;
    U8 ucstatus = 0;
    S8 minclk_val, maxclk_val, minca_val, maxca_val;  

#ifdef CKE_CS_DLY_SETTING
    S8 CAShift_Avg = 0;
#endif

        S8 iCenter[CATRAINING_NUM];
        S8 iBestFirstClkPass[CATRAINING_NUM], iBestLastClkPass[CATRAINING_NUM];
        S8 iBestFirstCAPass[CATRAINING_NUM], iBestLastCAPass[CATRAINING_NUM];
        S32 iPass, iClkWinSize, iCAWinSize;
        U32 uiDelay;


    U32 *uiLPDDR_PHY_Mapping;
    U32 *uiLPDDR_CA_Mapping;

    
    // error handling
    if (!p)
    {
        mcSHOW_ERR_MSG(("context is NULL\n"));
        return;
    }

    if (p->dram_type != TYPE_LPDDR3)
    {
        mcSHOW_ERR_MSG(("Wrong DRAM TYPE. Only support LPDDR3 in CA training!!\n"));
        return;
    }

    if (p->channel == CHANNEL_A)
    {
        uiLPDDR_PHY_Mapping = (U32 *)uiLPDDR3_PHY_Mapping_POP_CHA;
        uiLPDDR_CA_Mapping = (U32 *)uiLPDDR3_CA_Mapping_CHA;
    }    

#if 0
    for (uiCA=0; uiCA<CATRAINING_NUM; uiCA++)
    {
        iLastCAPass[uiCA] -= iCAShift[uiCA]; 
        iLastClkPass[uiCA] += iCAShift[uiCA];         

        iLastCAPass2[uiCA] -= iCAShift[uiCA];         
        iLastClkPass2[uiCA] += iCAShift[uiCA]; 
    }
#endif

for (uiCA=0; uiCA<CATRAINING_NUM; uiCA++)
{
    mcSHOW_DBG_MSG4(("[DramcCATraining_4] CA%d     CA(%d~%d) Clk(%d~%d) \n", uiCA, iLastCAPass[uiCA], iFirstCAPass[uiCA], iFirstClkPass[uiCA], iLastClkPass[uiCA]));
}    
for (uiCA=0; uiCA<CATRAINING_NUM; uiCA++)
{
    mcSHOW_DBG_MSG4(("[DramcCATraining_4] CA%d     CA2(%d~%d) Clk2(%d~%d) \n", uiCA, iLastCAPass2[uiCA], iFirstCAPass2[uiCA], iFirstClkPass2[uiCA], iLastClkPass2[uiCA]));    
}    


    DramcCATraining_2(iFirstClkPass, iLastClkPass, iFirstCAPass, iLastCAPass, iFirstClkPass2, iLastClkPass2, iFirstCAPass2, iLastCAPass2, &iMaxCenter);



        for (uiCA=0; uiCA<CATRAINING_NUM; uiCA++) 
        {
            iBestLastClkPass[uiCA] = iBestLastCAPass[uiCA] = -1;
            iBestFirstClkPass[uiCA] = iBestFirstCAPass[uiCA] = -1;        
        }
        
        
        // Delay clock output delay to do CA training in order to get the pass window.
        for (uiDelay=0; uiDelay<=MAX_CLKO_DELAY; uiDelay++) 
        {     
        
                for (uiCA=0; uiCA<CATRAINING_NUM; uiCA++) 
                {
                    if ( (iLastClkPass[uiCA]-iFirstClkPass[uiCA]) >= (iBestLastClkPass[uiCA]-iBestFirstClkPass[uiCA]))
                    {
                      iBestLastClkPass[uiCA] = iLastClkPass[uiCA];
                      iBestFirstClkPass[uiCA] = iFirstClkPass[uiCA];
                    }  
        
                    if ( (iLastCAPass[uiCA]-iFirstCAPass[uiCA]) >= (iBestLastCAPass[uiCA]-iBestFirstCAPass[uiCA]) )
                    {
                        iBestLastCAPass[uiCA] = iLastCAPass[uiCA];
                        iBestFirstCAPass[uiCA] = iFirstCAPass[uiCA];
                    }                
                }       
        }


        iMaxCenter = 0 - MAX_CLKO_DELAY;
        for (uiCA=0; uiCA<CATRAINING_NUM; uiCA++)
        {
            iClkWinSize = iBestLastClkPass[uiCA] - iBestFirstClkPass[uiCA];
            iCAWinSize = iBestLastCAPass[uiCA] - iBestFirstCAPass[uiCA];
            if (iClkWinSize >= iCAWinSize)
            {
                if (iCAWinSize>0)
                {
                    iCenter[uiCA] =  (iClkWinSize - iCAWinSize)>>1;
                }
                else
                {
                    iCenter[uiCA] =  iBestFirstClkPass[uiCA] + (iClkWinSize>>1);
                }
            }
            else
            {
                if (iClkWinSize>0)
                {
                    iCenter[uiCA] =  (iClkWinSize - iCAWinSize)/2;
                }
                else
                {
                    iCenter[uiCA] =  0-(iBestFirstCAPass[uiCA] + (iCAWinSize>>1));
                }
            } 
        
#ifndef RELEASE    
            mcSHOW_DBG_MSG4(("CA%d     CA(%d~%d) Clk(%d~%d) %d\n", uiCA, iBestLastCAPass[uiCA],  iBestFirstCAPass[uiCA],
                iBestFirstClkPass[uiCA], iBestLastClkPass[uiCA], iCenter[uiCA]));
#else
            mcSHOW_USER_MSG(("%d     (%d~%d) (%d~%d) %d\n", uiCA, iBestLastCAPass[uiCA],  iBestFirstCAPass[uiCA],
                iBestFirstClkPass[uiCA], iBestLastClkPass[uiCA], iCenter[uiCA]));
#endif    
    
            if (iCenter[uiCA]  > iMaxCenter)
            {
                iMaxCenter = iCenter[uiCA];
            }
}
        mcSHOW_DBG_MSG4(("=========================================\n"));
        mcSHOW_DBG_MSG4(("Max center = %d\n\n", iMaxCenter));


    if (iMaxCenter < 0)
    { 
        // Clk output delay could not be negative. Need to adjust into 0.
        iMaxCenter = 0;
        mcSHOW_DBG_MSG4(("Max center < 0. Adjust to 0. \n\n"));
    }

    for (uiCA=0; uiCA<CATRAINING_NUM; uiCA++)
    {
        iCAShift[uiCA] = iMaxCenter - iCenter[uiCA]+CATRAINING_STEP;
        if (iCAShift[uiCA]>=MAX_CLKO_DELAY)
        {
            iCAShift[uiCA] = MAX_CLKO_DELAY;
        }

        if (iCAShift[uiCA]<0) iCAShift[uiCA] = 0;



        
#ifdef CKE_CS_DLY_SETTING
        CAShift_Avg += iCAShift[uiCA];
#endif  
        mcSHOW_DBG_MSG4(("4_CA%d     Shift %d     CA(%d~%d) Clk(%d~%d) %d\n", uiCA, iCAShift[uiCA], 
            iBestLastCAPass[uiCA]-iCAShift[uiCA], iBestFirstCAPass[uiCA],  
            iBestFirstClkPass[uiCA], iBestLastClkPass[uiCA]+iCAShift[uiCA], iCenter[uiCA]+iCAShift[uiCA]));
    }


#if 1
    minca_val = MAX_CLKO_DELAY;
    maxca_val = MAX_CLKO_DELAY;        
    minclk_val = MAX_CLKO_DELAY;
    maxclk_val = MAX_CLKO_DELAY;
        
    for (uiCA=0; uiCA<CATRAINING_NUM; uiCA++)
    {
#if 0    
        if (iLastCAPass[uiCA] < maxca_val) maxca_val = iLastCAPass[uiCA];        
        if (iFirstCAPass[uiCA] < minca_val) minca_val = iFirstCAPass[uiCA];        
        
        if (iFirstClkPass[uiCA] < minclk_val) minclk_val = iFirstClkPass[uiCA];
        if (iLastClkPass[uiCA] < maxclk_val) maxclk_val = iLastClkPass[uiCA];
#else
        if (iBestLastCAPass[uiCA]-iCAShift[uiCA] < maxca_val) maxca_val = iBestLastCAPass[uiCA]-iCAShift[uiCA];        
        if (iBestFirstCAPass[uiCA] < minca_val) minca_val = iBestFirstCAPass[uiCA];        

        if (iBestFirstClkPass[uiCA] < minclk_val) minclk_val = iBestFirstClkPass[uiCA];
        if (iBestLastClkPass[uiCA]+iCAShift[uiCA] < maxclk_val) maxclk_val = iBestLastClkPass[uiCA]+iCAShift[uiCA];
#endif
    }
#endif    

#if 1
    for (uiCA=0; uiCA<CATRAINING_NUM; uiCA++)
    {
        iLastCAPass[uiCA] = maxca_val;
        iFirstCAPass[uiCA] = minca_val;
        
        iFirstClkPass[uiCA] = minclk_val;
        iLastClkPass[uiCA] = maxclk_val;
    }        


    iMaxCenter = ((maxclk_val + maxca_val + 1) / 2) - (maxca_val + 1);
    if (iMaxCenter < 0) iMaxCenter = 0;

mcSHOW_DBG_MSG4(("\n"));
for (uiCA=0; uiCA<CATRAINING_NUM; uiCA++)
{
    mcSHOW_DBG_MSG4(("[DramcCATraining_4_1] CA%d     CA(%d~%d) Clk(%d~%d) %d\n", uiCA, iLastCAPass[uiCA], iFirstClkPass[uiCA], iFirstClkPass[uiCA], iLastClkPass[uiCA], iMaxCenter));

}
mcSHOW_DBG_MSG4(("\n"));
#endif    



    if (update_CA_flag == 1)
    {
        // Write shift value into CA output delay.
        uiTemp = 0;
        for(i=0;i<=3;i++)
        {
            uiTemp |= (U32)(iCAShift[uiLPDDR_CA_Mapping[i]]) << (8*i);
        }
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1a8), uiTemp);    // CA0~CA3
        mcSHOW_DBG_MSG4(("Reg.1A8h=%xh\n", uiTemp));    
    
        uiTemp = 0;
        for(i=4;i<=7;i++)
        {
            uiTemp |= (U32)(iCAShift[uiLPDDR_CA_Mapping[i]]) << (8*(i-4));
        }    
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1ac), uiTemp);    // CA4~CA7
        mcSHOW_DBG_MSG4(("Reg.1ACh=%xh\n", uiTemp));    
    
        uiTemp = 0;
        for(i=8;i<=9;i++)
        {
            uiTemp |= (U32)(iCAShift[uiLPDDR_CA_Mapping[i]]) << (8*(i-8));
        }
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1b0), uiTemp);    // CA8~CA11
        mcSHOW_DBG_MSG4(("Reg.1B0h=%xh\n", uiTemp));        
    
#ifdef CKE_CS_DLY_SETTING
        CAShift_Avg = (CAShift_Avg + (CATRAINING_NUM>>1)) /CATRAINING_NUM;
        // CKEDLY : Reg.1B8h[12:8].  CSDLY : Reg.1B8h[4:0]
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1b8), &uiTemp);
        uiTemp = (uiTemp & 0xffffe0e0) | (CAShift_Avg << 8) | (CAShift_Avg << 0);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1b8), uiTemp);
        mcSHOW_DBG_MSG4(("Reg.1B8h=%xh\n", uiTemp));    
    
#if 0
        // CKE1DLY : Reg.1C4h[28:24]
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1c4), &uiTemp);
        uiTemp = (uiTemp & 0xe0ffffff) | (CAShift_Avg << 24);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1c4), uiTemp);
        mcSHOW_DBG_MSG4(("Reg.1C4h=%xh\n", uiTemp));    
#else
        // CKE1DLY->RASDLY : Reg.1b8h[20:16]
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1b8), &uiTemp);
        uiTemp = (uiTemp & 0xffe0ffff) | (CAShift_Avg << 16);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1b8), uiTemp);
        mcSHOW_DBG_MSG4(("Reg.1B8h=%xh\n", uiTemp));    
#endif
    
        // CS1DLY : Reg.0Ch[31:28]
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x0c), &uiTemp);
        uiTemp = (uiTemp & 0x0fffffff) | (CAShift_Avg << 28);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x0c), uiTemp);
        mcSHOW_DBG_MSG4(("Reg.00ch=%xh\n", uiTemp));    
#endif    
    
        DramcEnterSelfRefresh(p, 1);    
        // Write max center value into Clk output delay.
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x0c), &uiTemp);
        mcSET_FIELD(uiTemp, iMaxCenter+CATRAINING_STEP, 0x0f000000, 24);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x0c), uiTemp);    
        mcSHOW_DBG_MSG4(("Reg.0Ch=%xh\n", uiTemp));    
        DramcEnterSelfRefresh(p, 0);
    
        CATrain_ClkDelay[p->channel] = iMaxCenter;
        mcSHOW_DBG_MSG4(("CATrain_ClkDelay=%d...\n", CATrain_ClkDelay[p->channel]));
    }
}






#ifdef SPM_WORKAROUND_CATRAINING
void DramcRestoreDual_Freq_High_CATraining_Reg(void)
{
    U8 ucstatus;
    U32 u4value;


    //backup 1PLL CATraining Regs
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1a8), &DUAL_FREQ_LOW_reg_1a8);
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1ac), &DUAL_FREQ_LOW_reg_1ac);    
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1b0), &DUAL_FREQ_LOW_reg_1b0);
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1b8), &DUAL_FREQ_LOW_reg_1b8);
#if 0    
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1c4), &DUAL_FREQ_LOW_reg_1c4);
#endif
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x0c), &DUAL_FREQ_LOW_reg_00c);

    

    //save 3PLL 1800 Reg0x0c to Reg0x188 for SPM used
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x188), DUAL_FREQ_HIGH_reg_00c);

    //save 1PLL 1300 Reg0x0c to Reg0x18c for SPM used
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x18c), DUAL_FREQ_LOW_reg_00c);


    //restore 3PLL 1800 CA training reg
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1a8), DUAL_FREQ_HIGH_reg_1a8);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1ac), DUAL_FREQ_HIGH_reg_1ac);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1b0), DUAL_FREQ_HIGH_reg_1b0);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1b8), DUAL_FREQ_HIGH_reg_1b8);
#if 0    
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1c4), DUAL_FREQ_HIGH_reg_1ac4;    
#endif
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x0c), DUAL_FREQ_HIGH_reg_00c);
}
#endif

//-------------------------------------------------------------------------
/** DramcWriteLeveling
 *  start Write Leveling Calibration.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param  apply           (U8): 0 don't apply the register we set  1 apply the register we set ,default don't apply.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL 
 */
//-------------------------------------------------------------------------
DRAM_STATUS_T DramcWriteLeveling(DRAMC_CTX_T *p)
{
// Note that below procedure is based on "ODT off"
    U8 ucstatus = 0;
    U32 u4value, u4dq_o1, u4temp;
    U32 u4prv_register_1dc, u4prv_register_044, u4prv_register_0e4, u4prv_register_13c, u4prv_register_0d8, u4prv_register_0c4;
    U8 ucsample_status[DQS_NUMBER], ucdq_o1_perbyte[DQS_NUMBER], ucdq_o1_index[DQS_NUMBER];
    U8 byte_i, ucsample_count;
    S8 ii, ClockDelayMax = MAX_TX_DQSDLY_TAPS;
#ifdef CKE_CS_DLY_SETTING
    S8 CAShift_Avg=0;
#endif
    EMI_SETTINGS *emi_set;
    
    if(emi_setting_index == -1)
        emi_set = default_emi_setting;
    else
        emi_set = &emi_settings[emi_setting_index];  

    // error handling
    if (!p)
    {
        mcSHOW_ERR_MSG(("context is NULL\n"));
        return DRAM_FAIL;
    }

    fgwrlevel_done[p->channel] = 0;
    if ((p->dram_type != TYPE_PCDDR3) && (p->dram_type != TYPE_LPDDR3))
    {
        mcSHOW_ERR_MSG(("Wrong DRAM TYPE. Only support DDR3 and LPDDR3 in write leveling!!\n"));
        return DRAM_FAIL;
    }

    // this depends on pinmux
    // select first bit of each byte


    if (p->channel == CHANNEL_A)
    {
        ucdq_o1_index[0]=(U8) uiLPDDR3_PHY_Mapping_POP_CHA[0];
        ucdq_o1_index[1]=(U8) uiLPDDR3_PHY_Mapping_POP_CHA[8];
        ucdq_o1_index[2]=(U8) uiLPDDR3_PHY_Mapping_POP_CHA[16];
        ucdq_o1_index[3]=(U8) uiLPDDR3_PHY_Mapping_POP_CHA[24];
    }
    else
    {
        mcSHOW_ERR_MSG(("Only Channel A in K2!!\n"));
        return DRAM_FAIL;
    }


    // backup mode settings
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), &u4prv_register_1dc);
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), &u4prv_register_044);
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL4), &u4prv_register_0e4);
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_WRLEV), &u4prv_register_13c);
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MCKDLY), &u4prv_register_0d8);
#if fcFOR_CHIP_ID == fcJADE
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xc4), &u4prv_register_0c4);	
#endif


    //dramc clk gating always no gating
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1dc), &u4value);
    mcSET_BIT(u4value, 26);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1dc), u4value);
    mcDELAY_US(1);


    //write leveling mode initialization
    //Make CKE fixed at 1 (Put this before issuing MRS): CKEFIXON = 1 (0xe4[2])
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL4), &u4value);
    mcSET_BIT(u4value, POS_PADCTL4_CKEFIXON);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL4), u4value);

#if fcFOR_CHIP_ID == fcK2
    //disable auto refresh: REFCNT_FR_CLK = 0 (0x1dc[23:16]), ADVREFEN = 0 (0x44[30])
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), &u4value);
    mcCLR_MASK(u4value, MASK_DRAMC_PD_CTRL_REFCNT_FR_CLK);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), u4value);

    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), &u4value);
    mcCLR_BIT(u4value, POS_TEST2_3_ADVREFEN);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), u4value);
#endif

#if fcFOR_CHIP_ID == fcJADE
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xc4), &u4value);
    mcSET_BIT(u4value, 0);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xc4), u4value);	
#endif
    
/*
    //Enable Write ODT: WOEN = 1 (0x7c[3])
    //may no need to set here, initial value
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DDR2CTL), &u4value);
    mcSET_BIT(u4value, POS_DDR2CTL_WOEN);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DDR2CTL), u4value);

    //ODT, DQIEN fixed at 1; FIXODT = 1 (0xd8[23]), FIXDQIEN = 1111 (0xd8[15:12])
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MCKDLY), &u4value);
    mcSET_BIT(u4value, POS_MCKDLY_FIXODT);
    mcSET_MASK(u4value, MASK_MCKDLY_FIXDQIEN);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MCKDLY), u4value);
*/
    //FIXDQIEN = 1111 (0xd8[15:12])
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MCKDLY), &u4value);
    mcSET_MASK(u4value, MASK_MCKDLY_FIXDQIEN);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MCKDLY), u4value);

    //Enable DQ_O1, SELO1ASO=1
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_LPDDR2), &u4value);
    mcSET_BIT(u4value, POS_LPDDR2_SELO1ASO);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_LPDDR2), u4value);

    // enable DDR write leveling mode
    if (p->dram_type == TYPE_PCDDR3)
    {
        //issue MR1[7] to enable write leveling
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MRS), DEFAULT_MR1_VALUE_DDR3 | 0x00000080);
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD), &u4value);
        mcSET_BIT(u4value, POS_SPCMD_MRWEN);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD), u4value);
        mcDELAY_US(1);
        mcCLR_BIT(u4value, POS_SPCMD_MRWEN);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD), u4value);
    }
    else // LPDDR3
    {
        // issue MR2[7] to enable write leveling (refer to DEFAULT MR2 value)
//      #ifdef DUAL_FREQ_DIFF_RLWL
    if (p->frequency == DUAL_FREQ_LOW)
    {
          ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MRS), LPDDR3_MODE_REG_2_LOW | 0x00800000);
    }
    else
    {
          ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MRS), emi_set->iLPDDR3_MODE_REG_2 | 0x00800000);
    }     
//  #else
//        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MRS), emi_set->iLPDDR3_MODE_REG_2 | 0x00800000);
//  #endif

        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD), &u4value);
        mcSET_BIT(u4value, POS_SPCMD_MRWEN);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD), u4value);
        mcDELAY_US(1);
        mcCLR_BIT(u4value, POS_SPCMD_MRWEN);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD), u4value);
    }

    //wait tWLDQSEN (25 nCK / 25ns) after enabling write leveling mode (DDR3 / LPDDDR3)
    mcDELAY_US(1);

    //Set {R_DQS_B3_G R_DQS_B2_G R_DQS_B1_G R_DQS_B0_G}=1010: 0x13c[4:1] (this depends on sel_ph setting)
    //Enable Write leveling: 0x13c[0]
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_WRLEV), &u4value);
    mcSET_FIELD(u4value, 0xa, MASK_WRLEV_DQS_Bx_G, POS_WRLEV_DQS_Bx_G);
    mcSET_BIT(u4value, POS_WRLEV_WRITE_LEVEL_EN);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_WRLEV), u4value);

    // wait tWLMRD (40 nCL / 40 ns) before DQS pulse (DDR3 / LPDDR3)
    mcDELAY_US(1);    

    //Proceed write leveling...
    //Initilize sw parameters
    for (ii=0; ii < (S8)(p->data_width/DQS_BIT_NUMBER); ii++)
    {
        ucsample_status[ii] = 0;
        wrlevel_dqs_final_delay[p->channel][ii] = 0;
    }

    //used for WL done status
    // each bit of sample_cnt represents one-byte WL status
    // 1: done or N/A. 0: NOK
    if (p->data_width == DATA_WIDTH_32BIT)
    {
        ucsample_count = 0xf0;
    }
    else if ((p->data_width == DATA_WIDTH_16BIT))
    {
        ucsample_count = 0xfc;
    }
    else
    {
        ucsample_count = 0xf0;
    }

#ifndef RELEASE   
    mcSHOW_DBG_MSG(("===============================================================================\n"));
    mcSHOW_DBG_MSG(("\n        [Channel %d]dramc_write_leveling_swcal\n", p->channel));
    mcSHOW_DBG_MSG(("===============================================================================\n"));
    mcSHOW_DBG_MSG(("delay  byte0  byte1  byte2  byte3\n"));
    mcSHOW_DBG_MSG(("-----------------------------\n"));
#else
    mcSHOW_USER_MSG(("\nWL\n"));
#endif

    //not sure LP3 can be WL together 
#ifndef fcWL_ALL
    for (byte_i = 0; byte_i < (p->data_width/DQS_BIT_NUMBER);  byte_i++)
#endif
    {
    #ifndef fcWL_ALL
        // select respective DQS
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_WRLEV), &u4value);
        mcSET_FIELD(u4value, ((U8)1<<byte_i), MASK_WRLEV_DQS_SEL, POS_WRLEV_DQS_SEL);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_WRLEV), u4value);
    #else
        // select all DQS
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_WRLEV), &u4value);
        mcSET_FIELD(u4value, 0xf, MASK_WRLEV_DQS_SEL, POS_WRLEV_DQS_SEL);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_WRLEV), u4value);
    #endif
    
        for (ii=(-MAX_TX_DQSDLY_TAPS+1); ii<MAX_TX_DQSDLY_TAPS; ii++)
        {
        #ifndef fcWL_ALL
            if (ii >= 0)
      {        
            // Set Clk output delay to 0.
              ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x0c), &u4value);
          mcSET_FIELD(u4value, 0, 0x0f000000, 24);
            ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x0c), u4value);    
            
              // set DQS delay
              ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL3), &u4value); 
              mcSET_FIELD(u4value, ii, ((U32)0xf)<<(4*byte_i), 4*byte_i);       
              ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL3), u4value);
            }
            else
            {
                  // Adjust Clk output delay.
              ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x0c), &u4value);
          mcSET_FIELD(u4value, -ii, 0x0f000000, 24);
            ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x0c), u4value);    

                  // Set DQS output delay to 0
          u4value = 0;
              ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL3), u4value);
            }
        #else
            if (ii >= 0)
      {
            // Set Clk output delay to 0.
              ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x0c), &u4value);
          mcSET_FIELD(u4value, 0, 0x0f000000, 24);
            ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x0c), u4value);    
            
            // Adjust DQS output delay.
          u4value = 0;
              for (byte_i= 0; byte_i < (p->data_width/DQS_BIT_NUMBER); byte_i++)
              {
                  u4value += (((U32)ii)<<(4*byte_i));        
              }
              ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL3), u4value);
            }
            else
            {
              // Adjust Clk output delay.
              ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x0c), &u4value);
          mcSET_FIELD(u4value, -ii, 0x0f000000, 24);
            ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x0c), u4value);    

              // Set DQS output delay to 0
          u4value = 0;
              ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL3), u4value);
            }
        #endif            
            
            //Trigger DQS pulse, R_DQS_WLEV: 0x13c[8] from 1 to 0
            ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_WRLEV), &u4value);
            mcSET_BIT(u4value, POS_WRLEV_DQS_WLEV);
            ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_WRLEV), u4value);
            mcCLR_BIT(u4value, POS_WRLEV_DQS_WLEV);
            ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_WRLEV), u4value);

            //wait tWLO (7.5ns / 20ns) before output (DDR3 / LPDDR3)
            mcDELAY_US(1);

            //Read DQ_O1 from register, 0x380
            ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x380), &u4dq_o1);
            //mcSHOW_DBG_MSG(("loop: %d DQ_O1: 0x%x \n", ii, u4dq_o1));

        #ifdef fcWL_ALL
            #ifndef RELEASE  
              mcSHOW_DBG_MSG(("%d    ", ii));
            #else
              mcSHOW_USER_MSG(("%d    ", ii));
            #endif
            for (byte_i = 0; byte_i < (p->data_width/DQS_BIT_NUMBER);  byte_i++)
        #endif
            {
                ucdq_o1_perbyte[byte_i] = (U8)((u4dq_o1>>ucdq_o1_index[byte_i])&0x00000001);        

      #ifdef WL_TEST
        if (byte_i==0)
        {
          // 0-1-0
      if ((ii <= -11) || (ii>-5) || (ii==-9))
      {
        ucdq_o1_perbyte[byte_i]  = 1;
      } 
      else
      {
        ucdq_o1_perbyte[byte_i]  = 0;
      }
        }
        else if (byte_i==1)
        {
          // 1-0-1-0-1
      if ((ii <= -11) || (ii>=-3) || (ii==-9) || (ii==-7))
      {
        ucdq_o1_perbyte[byte_i]  = 1;
      } 
      else
      {
        ucdq_o1_perbyte[byte_i]  = 0;
      }
        
        }
        else if (byte_i==2)
        {
          // 1-0-1-1-0-0-0
      if ((ii <= -11) || (ii>-2) || (ii==-9) || (ii==-8))
      {
        ucdq_o1_perbyte[byte_i]  = 1;
      } 
      else
      {
        ucdq_o1_perbyte[byte_i]  = 0;
      }
        
        }
        else if (byte_i==3)
        {
      ucdq_o1_perbyte[byte_i]  = 0;
      // 0-0-..-0-1
/*      if (ii == (MAX_TX_DQSDLY_TAPS-1))
      {
        ucdq_o1_perbyte[byte_i]  = 1;
      } 
      else
      {
        ucdq_o1_perbyte[byte_i]  = 0;
      }*/
        
          // 1-0-1-1-1-0-0-0
/*      if ((ii <= -3) || (ii>5) || (ii==-1) || (ii==0) || (ii==1))
      {
        ucdq_o1_perbyte[byte_i]  = 1;
      } 
      else
      {
        ucdq_o1_perbyte[byte_i]  = 0;
      }*/
        
        }
          
      #endif
      
            #ifndef fcWL_ALL
                mcSHOW_DBG_MSG(("%d    %d\n", ii,  ucdq_o1_perbyte[byte_i]));
            #else
              #ifndef RELEASE    
                mcSHOW_DBG_MSG(("%d    ", ucdq_o1_perbyte[byte_i]));
              #else
                mcSHOW_USER_MSG(("%d    ", ucdq_o1_perbyte[byte_i]));
              #endif
            #endif

                //sample from 0 to 1        
                // Mark the following because of the case 1000000....00000001111..
               /* if ((ucsample_status[byte_i]==0) && (ucdq_o1_perbyte[byte_i]==1))
                {
                    ucsample_status[byte_i] = 2;
                    //record delay value
                    wrlevel_dqs_final_delay[p->channel][byte_i] = ii;
                #ifndef fcWL_ALL
                    // to the next byte
                    //break;
                #else
                    //used for early break
                    ucsample_count++;
                #endif
                }
                else */
                // ucsample_status :
                //    1 : 0.
                //    2 : 0-1
                //    3 : 0-1-1
                //    4 : 0-1-1-1
                //    ...
                if ((ucsample_status[byte_i]==0) && (ucdq_o1_perbyte[byte_i]==0))
                {
                    ucsample_status[byte_i] = 1;
                }
                else if ((ucsample_status[byte_i]==1) && (ucdq_o1_perbyte[byte_i]==1))
                {
                    ucsample_status[byte_i] = 2;
                    if (ii==(MAX_TX_DQSDLY_TAPS-1)) // Only in last tap, otherwise keep searching consecutive 1s.
                    {
                      //record delay value
                      wrlevel_dqs_final_delay[p->channel][byte_i] = ii;
                  #ifndef fcWL_ALL
                      // to the next byte
                      //break;
                  #else
                      //used for WL done status
                      ucsample_count |= (0x01 << byte_i);
                  #endif
                    }
                }
                else if (ucsample_status[byte_i]>=2)  // (ucsample_status[byte_i]-1) consecutive 1s.
                {
                    if (ucdq_o1_perbyte[byte_i]==1)
                    {
                   // 0-1-1
                   ucsample_status[byte_i]++;
                   //record delay value
                   wrlevel_dqs_final_delay[p->channel][byte_i] = ii -(ucsample_status[byte_i]-2);
               #ifndef fcWL_ALL
                   // to the next byte
                   //break;
               #else
                   //used for WL done status
                   ucsample_count |= (0x01 << byte_i);
               #endif                 
               }
                    else
                    {
                        if (ucsample_status[byte_i] <= 4)
                        {
                            // 0-1-0, 0
                            // Maybe unstable or glitch during transition boundary like 1-1-0-1-0-0-0-0-0
                            ucsample_status[byte_i] = 0;
                            ucsample_count &= (~(0x01 << byte_i));
                        }
                    }
                }
            }
        #ifdef fcWL_ALL
          #ifndef RELEASE
            mcSHOW_DBG_MSG(("\n"));
          #else
            mcSHOW_USER_MSG(("\n"));
          #endif           
        #endif
        }
    }

    if (ucsample_count==0xff)
    {
        // all bytes are done
        fgwrlevel_done[p->channel] = 1;        
    }   

#ifdef fcWL_ALL
    mcSHOW_DBG_MSG(("pass byte mask = %xh, fgwrlevel_done[%d]=%xh\n", ucsample_count, p->channel, fgwrlevel_done[p->channel]));
#endif
    mcSHOW_DBG_MSG(("byte_i    status    best delay\n"));
    for (byte_i = 0; byte_i < (p->data_width/DQS_BIT_NUMBER);  byte_i++)
    {
        mcSHOW_DBG_MSG(("%d    %d    %d\n", byte_i, ucsample_status[byte_i], wrlevel_dqs_final_delay[p->channel][byte_i]));
        if (ClockDelayMax > wrlevel_dqs_final_delay[p->channel][byte_i])
        {
          ClockDelayMax = wrlevel_dqs_final_delay[p->channel][byte_i];
        }
    }    
    mcSHOW_DBG_MSG(("========================================\n"));

#ifdef DDR_FT_LOAD_BOARD
    if (!fgwrlevel_done[p->channel])
    {
        LoadBoardShowResult(FLAG_WL_CALIBRATION, FLAG_CALIBRATION_FAIL, p->channel, FLAG_NOT_COMPLETE_OR_FAIL);            
        while(1); 
    }
#endif

    if (ClockDelayMax > 0)
    {
      ClockDelayMax = 0;
    }
    else
    {
      ClockDelayMax = -ClockDelayMax;
    }

    // Adjust Clk & CA if needed
    if (CATrain_ClkDelay[p->channel] < ClockDelayMax)
    {
      S32 Diff = ClockDelayMax - CATrain_ClkDelay[p->channel];
      U8 RAXDLY[4];

  //DramcEnterSelfRefresh(p, 1);    
  // Write max center value into Clk output delay.
  ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x0c), &u4value);
  mcSET_FIELD(u4value, ClockDelayMax, 0x0f000000, 24);
  ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x0c), u4value);    
  mcSHOW_DBG_MSG(("Reg.0Ch=%xh\n", u4value));    
  //DramcEnterSelfRefresh(p, 0);      

  mcSHOW_DBG_MSG(("CA adjust %d taps... \n", Diff));

  // Write shift value into CA output delay.
  ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1a8), &u4value);
  for (ii=0; ii<4; ii++)
  {
    RAXDLY[ii] =  ((u4value>>(ii<<3)) & 0x0f) + Diff;
    if (RAXDLY[ii] > 0x0f)
    {
      RAXDLY[ii] = 0x0f;
    }
#ifdef CKE_CS_DLY_SETTING
    CAShift_Avg += RAXDLY[ii];
#endif
  }

  u4value = RAXDLY[0] + (RAXDLY[1]<<8) + (RAXDLY[2]<<16) + (RAXDLY[3]<<24);
  ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1a8), u4value);    // CA0~CA3
  mcSHOW_DBG_MSG(("Reg.1A8h=%xh\n", u4value));    

  ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1ac), &u4value);
  for (ii=0; ii<4; ii++)
  {
    RAXDLY[ii] =  ((u4value>>(ii<<3)) & 0x0f) + Diff;
    if (RAXDLY[ii] > 0x0f)
    {
      RAXDLY[ii] = 0x0f;
    }
#ifdef CKE_CS_DLY_SETTING
    CAShift_Avg += RAXDLY[ii];
#endif  
  }
  u4value = RAXDLY[0] + (RAXDLY[1]<<8) + (RAXDLY[2]<<16) + (RAXDLY[3]<<24);
  ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1ac), u4value);    // CA4~CA7
  mcSHOW_DBG_MSG(("Reg.1ACh=%xh\n", u4value));    

  ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1b0), &u4value);
  for (ii=0; ii<2; ii++)
  {
    RAXDLY[ii] =  ((u4value>>(ii<<3)) & 0x0f) + Diff;
    if (RAXDLY[ii] > 0x0f)
    {
      RAXDLY[ii] = 0x0f;
    }
#ifdef CKE_CS_DLY_SETTING
    CAShift_Avg += RAXDLY[ii];
#endif  
  }
  u4value = RAXDLY[0] + (RAXDLY[1]<<8);
  ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1b0), u4value);    // CA4~CA7
  mcSHOW_DBG_MSG(("Reg.1B0h=%xh\n", u4value));    

#ifdef CKE_CS_DLY_SETTING
  CAShift_Avg = (CAShift_Avg + (CATRAINING_NUM>>1)) /CATRAINING_NUM;
  // CKEDLY : Reg.1B8h[12:8].  CSDLY : Reg.1B8h[4:0]
  ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1b8), &u4value);
  u4value = (u4value & 0xffffe0e0) | (CAShift_Avg << 8) | (CAShift_Avg << 0);
  ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1b8), u4value);
  mcSHOW_DBG_MSG(("Reg.1B8h=%xh\n", u4value));    

#if 0
  // CKE1DLY : Reg.1C4h[28:24]
  ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1c4), &u4value);
  u4value = (u4value & 0xe0ffffff) | (CAShift_Avg << 24);
  ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1c4), u4value);
  mcSHOW_DBG_MSG(("Reg.1C4h=%xh\n", u4value));    
#else
    // CKE1DLY->RASDLY : Reg.1b8h[20:16]
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1b8), &u4value);
    u4value = (u4value & 0xffe0ffff) | (CAShift_Avg << 16);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1b8), u4value);
    mcSHOW_DBG_MSG4(("Reg.1B8h=%xh\n", u4value)); 
#endif

  // CS1DLY : Reg.0Ch[31:28]
  ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x0c), &u4value);
  u4value = (u4value & 0x0fffffff) | (CAShift_Avg << 28);
  ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x0c), u4value);
  mcSHOW_DBG_MSG(("Reg.00ch=%xh\n", u4value));    
#endif     
    }
    else
    {
      ClockDelayMax = CATrain_ClkDelay[p->channel];
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x0c), &u4value);
    mcSET_FIELD(u4value, ClockDelayMax, 0x0f000000, 24);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x0c), u4value);         
    }    
    mcSHOW_DBG_MSG(("After adjustment...\n"));
    mcSHOW_DBG_MSG(("Clk output delay = %d\n", ClockDelayMax));
    
    for (byte_i = 0; byte_i < (p->data_width/DQS_BIT_NUMBER);  byte_i++)
    {
      wrlevel_dqs_final_delay[p->channel][byte_i] += (ClockDelayMax);
      if (wrlevel_dqs_final_delay[p->channel][byte_i] > 0xf)
      {
        wrlevel_dqs_final_delay[p->channel][byte_i] = 0x0f;
      }
      mcSHOW_DBG_MSG(("DQS%d output delay =  %d\n", byte_i, wrlevel_dqs_final_delay[p->channel][byte_i]));
    }

    // write leveling done, mode settings recovery if necessary
    // recover mode registers
    if (p->dram_type == TYPE_PCDDR3)
    {
        //issue MR1[7] to enable write leveling
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MRS), DEFAULT_MR1_VALUE_DDR3);
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD), &u4value);
        mcSET_BIT(u4value, POS_SPCMD_MRWEN);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD), u4value);
        mcDELAY_US(1);
        mcCLR_BIT(u4value, POS_SPCMD_MRWEN);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD), u4value);
    }
    else // LPDDR3
    {
        // issue MR2[7] to enable write leveling (refer to DEFAULT MR2 value)
//      #ifdef DUAL_FREQ_DIFF_RLWL
  if (p->frequency == DUAL_FREQ_LOW)
  {
          ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MRS), LPDDR3_MODE_REG_2_LOW);
  }
  else
  {
          ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MRS), emi_set->iLPDDR3_MODE_REG_2);
  } 
//  #else
//        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MRS), emi_set->iLPDDR3_MODE_REG_2);
//  #endif

        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD), &u4value);
        mcSET_BIT(u4value, POS_SPCMD_MRWEN);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD), u4value);
        mcDELAY_US(1);
        mcCLR_BIT(u4value, POS_SPCMD_MRWEN);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD), u4value);
    }

    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_WRLEV), u4prv_register_13c);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), u4prv_register_1dc);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), u4prv_register_044);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL4), u4prv_register_0e4);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MCKDLY), u4prv_register_0d8);    
#if fcFOR_CHIP_ID == fcJADE
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xc4), u4prv_register_0c4);   	
#endif
    

    //dramc clk gating controlled by dramc
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1dc), &u4value);
    mcCLR_BIT(u4value, 26);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1dc), u4value);


    //Disable DQ_O1, SELO1ASO=0 for power saving
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_LPDDR2), &u4value);
    mcCLR_BIT(u4value, POS_LPDDR2_SELO1ASO);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_LPDDR2), u4value);

    // set to best values
    // DQS
    u4value = 0;
    for (byte_i= 0; byte_i < (p->data_width/DQS_BIT_NUMBER); byte_i++)
    {
        u4value += (((U32)wrlevel_dqs_final_delay[p->channel][byte_i])<<(4*byte_i));        
    }
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL3), u4value);

    mcSHOW_DBG_MSG(("[write leveling]DQS: 0x%x", u4value));
    
    // DQM
    if (p->data_width == DATA_WIDTH_16BIT)
    {
        // for DQC case, DQM3 is CS#
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL2), &u4temp);
        mcSET_FIELD(u4temp, u4value, 0x000000ff, 0);
        u4value = u4temp;
    }
    else
    {
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL2), &u4temp);
        mcSET_FIELD(u4temp, u4value, 0x0000ffff, 0);
        u4value = u4temp;        
    }
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL2), u4value);

    mcSHOW_DBG_MSG((" DQM: 0x%x\n", u4value));

    // DQ delay, each DQ has 4 bits. Each register contains 8-bit DQ's
    for (byte_i = 0; byte_i < (p->data_width/DQS_BIT_NUMBER); byte_i++)
    {
        u4value = 0;
        for (ii = 0; ii < DQS_BIT_NUMBER; ii++)
        {
            u4value += (((U32) wrlevel_dqs_final_delay[p->channel][byte_i]) << (4*ii));
        }
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQODLY1+4*byte_i), u4value);
        mcSHOW_DBG_MSG(("[write leveling]DQ byte%d reg: 0x%x val: 0x%x\n", byte_i, mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQODLY1+4*byte_i), u4value));
    }   

    if (ucstatus)
    {
        mcSHOW_ERR_MSG(("register access fail!\n"));
        return DRAM_FAIL;
    }
    else
    {
        return DRAM_OK;
    }

    // log example
/*
===================================================================

                dramc_write_leveling_swcal
                apply=1 channel=2(2:cha, 3:chb)
===================================================================
delay  byte0  byte1  byte2  byte3
-----------------------------
  0    0    0    0    1
  1    0    0    0    1
  2    0    0    1    1
  3    0    0    1    1
  4    0    0    1    1
  5    0    0    1    1
  6    0    0    1    1
  7    0    0    1    1
  8    0    0    1    1
  9    0    0    1    1
 10    0    0    1    1
 11    1    1    1    1
pass bytecount = 4
byte_i    status    best delay
0         2         11
1         2         11
2         2         2
3         2         0
*/
    return DRAM_OK;
}



#if 1//add by KT, define the function in ett_cust.hw_dqsi_gw.c
//-------------------------------------------------------------------------
/** DramcRxdqsGatingCal
 *  start the dqsien software calibration.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL 
 */
//-------------------------------------------------------------------------
/*
 * nr_bit_set: Get the number of bits set in the given value.
 * @val: the gieven value
 * Return the number of bits set.
 */
static U8 nr_bit_set(U32 val)
{
  U8 i, cnt;
  
  for (cnt = 0, i = 0; i < 32; i++) 
  {
    if ((val & (1 << i)) && (i==31))    //Francis : process last bit if 1 
    {
      cnt++;
    }
    else if ((val & (1 << i)) && (val & (1 << (i+1))) && (i < 31))  //Francis add to prevent 1-0-1 case 
    {
      cnt++;
    }
    else if ((val & (1 << i)) && (val & (1 << (i-1))) && (i > 0))   //Francis : add last 1 , 1-1-1-1-1-0
    {
      cnt++;
    }
    else
    {
      if (cnt != 0)
      {
        // cnt !=0, val ==0, break;
        dqs_gw_cnt_break = 1;
        break;
      }
    }
  }

  return cnt;
}

/*
 * first_bit_set: Get the first bit set in the given value.
 * @val: the gieven value
 * Return the first bit set.
 */
static U8 first_bit_set(U32 val)
{
    U8 i;

    for (i = 0; i < 32; i++) 
    {
        if ((val & (1 << i)) && (val & (1 << (i+1))))   //Francis add to prevent 1-0-1 case
        {
            return i;
        }
    }

    return (0xff);
}

#ifdef ADJUST_CROSS_RANK_TIMING
void dqsi_gw_dly_coarse_factor_handler(DRAMC_CTX_T *p, U8 curr_val, U8 DQSINCTL) 
{
    U8 ucstatus = 0;
    U32 u4value;
    U8 curr_val_P1;
    U32 u4Temp;
  U32 u4CoarseTuneStart;
  
  if (DQSINCTL != 0xff) u4CoarseTuneStart = DQSINCTL; // According to DE, DQSINCTL & R1DQSINCTL set to the same.
  else u4CoarseTuneStart = curr_val>>2;

    if (u4CoarseTuneStart > 15) {
      u4CoarseTuneStart = 15;
    }

    curr_val_P1 = curr_val + 2; // diff is 0.5T (need to check with DE)
    
    // Rank 0 P0/P1 coarse tune settings.
    // DQSINCTL: 0xe0[27:24]: A60808 with extra 1 bit (unit : 1 DRAMC clock)
    // From coarse tune 22, so set DQSINCTL = 4 first....or (DQS_GW_COARSE_START)>>2??
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQSCTL1), &u4value);
    mcSET_FIELD(u4value, u4CoarseTuneStart, MASK_DQSCTL1_DQSINCTL, POS_DQSCTL1_DQSINCTL);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQSCTL1), u4value);

    // TXDLY_DQSGATE: 0x404[14:12] (unit : 1 DRAMC clock). 
    // DQSINCTL does not have P1. So need to use TXDLY_DQSGATE/TXDLY_DQSGATE_P1 to set different 1 M_CK coarse tune values for P0 & P1.
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SELPH2), &u4value);
    u4Temp = (curr_val>>2) - (u4CoarseTuneStart);
    mcSET_FIELD(u4value, u4Temp, MASK_SELPH2_TXDLY_DQSGATE, POS_SELPH2_TXDLY_DQSGATE);
    u4Temp = (curr_val_P1>>2) - (u4CoarseTuneStart);
    mcSET_FIELD(u4value, u4Temp, MASK_SELPH2_TXDLY_DQSGATE_P1, POS_SELPH2_TXDLY_DQSGATE_P1);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SELPH2), u4value);

    // dly_DQSGATE: 0x410[23:22] (unit : 0.25 DRAMC clock)
    // dly_DQSGATE_P1: 0x410[25:24] (unit : 0.25 DRAMC clock)
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SELPH5), &u4value);
    mcSET_FIELD(u4value, curr_val&0x3, MASK_SELPH5_dly_DQSGATE, POS_SELPH5_dly_DQSGATE);
    mcSET_FIELD(u4value, curr_val_P1&0x3, MASK_SELPH5_dly_DQSGATE_P1, POS_SELPH5_dly_DQSGATE_P1);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SELPH5), u4value);
}

#else
void dqsi_gw_dly_coarse_factor_handler(DRAMC_CTX_T *p, U8 curr_val, U8 DQSINCTL) 
{
    U8 ucstatus = 0;
    U32 u4value;
    U8 curr_val_P1;
    U32 u4Temp;
#if 1    
    U32 u4CoarseTuneStart = curr_val>>2;
    // This margin is to have rank1 coarse tune 3 M_CK smaller margin (up & down).
    // According to SY, DQSINCTL=0 will have problem?? need to confirm again...???
    if (u4CoarseTuneStart > 3)
    {
      u4CoarseTuneStart -= 3; 
    } 
    else
    {
      if (u4CoarseTuneStart)
      {
        u4CoarseTuneStart = 1;
      }
    }
#else
    U32 u4CoarseTuneStart = DQS_GW_COARSE_START>>2;
#endif

    if (u4CoarseTuneStart > 15) {
      u4CoarseTuneStart = 15;
    }

    curr_val_P1 = curr_val + 2; // diff is 0.5T (need to check with DE)
    
    // Rank 0 P0/P1 coarse tune settings.
    // DQSINCTL: 0xe0[27:24]: A60808 with extra 1 bit (unit : 1 DRAMC clock)
    // From coarse tune 22, so set DQSINCTL = 4 first....or (DQS_GW_COARSE_START)>>2??
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQSCTL1), &u4value);
    mcSET_FIELD(u4value, u4CoarseTuneStart, MASK_DQSCTL1_DQSINCTL, POS_DQSCTL1_DQSINCTL);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQSCTL1), u4value);

    // TXDLY_DQSGATE: 0x404[14:12] (unit : 1 DRAMC clock). 
    // DQSINCTL does not have P1. So need to use TXDLY_DQSGATE/TXDLY_DQSGATE_P1 to set different 1 M_CK coarse tune values for P0 & P1.
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SELPH2), &u4value);
    u4Temp = (curr_val>>2) - (u4CoarseTuneStart);
    mcSET_FIELD(u4value, u4Temp, MASK_SELPH2_TXDLY_DQSGATE, POS_SELPH2_TXDLY_DQSGATE);
    u4Temp = (curr_val_P1>>2) - (u4CoarseTuneStart);
    mcSET_FIELD(u4value, u4Temp, MASK_SELPH2_TXDLY_DQSGATE_P1, POS_SELPH2_TXDLY_DQSGATE_P1);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SELPH2), u4value);

    // dly_DQSGATE: 0x410[23:22] (unit : 0.25 DRAMC clock)
    // dly_DQSGATE_P1: 0x410[25:24] (unit : 0.25 DRAMC clock)
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SELPH5), &u4value);
    mcSET_FIELD(u4value, curr_val&0x3, MASK_SELPH5_dly_DQSGATE, POS_SELPH5_dly_DQSGATE);
    mcSET_FIELD(u4value, curr_val_P1&0x3, MASK_SELPH5_dly_DQSGATE_P1, POS_SELPH5_dly_DQSGATE_P1);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SELPH5), u4value);
}
#endif

void dqsi_gw_dly_fine_factor_handler(DRAMC_CTX_T *p, U8 curr_val) 
{
    U8 ucstatus = 0;
    U32 u4value;

    // DQS?IEN: 0x94 (each with 7 bits)
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQSIEN), &u4value);
    mcSET_FIELD(u4value, curr_val&0x7f, MASK_DQSIEN_R0DQS0IEN, POS_DQSIEN_R0DQS0IEN);
    mcSET_FIELD(u4value, curr_val&0x7f, MASK_DQSIEN_R0DQS1IEN, POS_DQSIEN_R0DQS1IEN);
    mcSET_FIELD(u4value, curr_val&0x7f, MASK_DQSIEN_R0DQS2IEN, POS_DQSIEN_R0DQS2IEN);
    mcSET_FIELD(u4value, curr_val&0x7f, MASK_DQSIEN_R0DQS3IEN, POS_DQSIEN_R0DQS3IEN);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQSIEN), u4value);
}

#ifdef USE_COARSE_TUNE_ALIGN_UPDATE
void dqsi_gw_dly_coarse_factor_handler_rank1(DRAMC_CTX_T *p, U8 curr_val, U8 DQSINCTL) 
{
      U8 ucstatus = 0;
      U32 u4value;
      U8 curr_val_P1;
      U32 u4Temp;
    U32 u4CoarseTuneStart;
    
    if (DQSINCTL != 0xff) u4CoarseTuneStart = DQSINCTL; // According to DE, DQSINCTL & R1DQSINCTL set to the same.
    else u4CoarseTuneStart = curr_val>>2;
    
      if (u4CoarseTuneStart > 15) {
        u4CoarseTuneStart = 15;
      }
    
      curr_val_P1 = curr_val + 2; // diff is 0.5T (need to check with DE)
      
      // TO be simple, assume curr_val < 62
      // R1DQSINCTL: 0x118[3:0]: (unit : 1 DRAMC clock)
      // From coarse tune 22, so set DQSINCTL = 4 first....or (DQS_GW_COARSE_START)>>2.
      // According to Derping, 6595 will only check DQSINCTL and ignore R1DQSINCTL because of P1 supporting. So R1DQSINCTL may be removed. But here still set the same value as DQSINCTL for safety.
      ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQSCTL2), &u4value);
      mcSET_FIELD(u4value, u4CoarseTuneStart, MASK_DQSCTL2_DQSINCTL, POS_DQSCTL2_DQSINCTL);
      ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQSCTL2), u4value);
    
    // TXDLY_R1DQSGATE:      0x418[6:4] (unit : 1 DRAMC clock)
    // TXDLY_R1DQSGATE_P1: 0x418[10:8] (unit : 1 DRAMC clock)
      ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SELPH6_1), &u4value);
      u4Temp = (curr_val>>2) - (u4CoarseTuneStart);
      mcSET_FIELD(u4value, u4Temp, MASK_SELPH6_1_TXDLY_R1DQSGATE, POS_SELPH6_1_TXDLY_R1DQSGATE);
      u4Temp = (curr_val_P1>>2) - (u4CoarseTuneStart);
      mcSET_FIELD(u4value, u4Temp, MASK_SELPH6_1_TXDLY_R1DQSGATE_P1, POS_SELPH6_1_TXDLY_R1DQSGATE_P1);
      ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SELPH6_1), u4value);
    
    // dly_R1DQSGATE:      0x418[1:0] (unit : 0.25 DRAMC clock)
    // dly_R1DQSGATE_P1: 0x418[3:2] (unit : 0.25 DRAMC clock)
      ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SELPH6_1), &u4value);
      mcSET_FIELD(u4value, curr_val&0x3, MASK_SELPH6_1_dly_R1DQSGATE, POS_SELPH6_1_dly_R1DQSGATE);
      mcSET_FIELD(u4value, curr_val_P1&0x3, MASK_SELPH6_1_dly_R1DQSGATE_P1, POS_SELPH6_1_dly_R1DQSGATE_P1);
      ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SELPH6_1), u4value);
}

#else
void dqsi_gw_dly_coarse_factor_handler_rank1(DRAMC_CTX_T *p, U8 curr_val, U8 DQSINCTL) 
{
    U8 ucstatus = 0;
    U32 u4value;
    U8 curr_val_P1;
    U32 u4Temp;
#if 1    
    U32 u4CoarseTuneStart = DQSINCTL; // According to DE, DQSINCTL & R1DQSINCTL set to the same.
#else
    U32 u4CoarseTuneStart = DQS_GW_COARSE_START>>2;
#endif

    // Should not happen >15.
    /*
    if (u4CoarseTuneStart > 15) {
      u4CoarseTuneStart = 15;
    }*/
    
    curr_val_P1 = curr_val + 2; // diff is 0.5T (need to check with DE)
    
    // TO be simple, assume curr_val < 62
    // R1DQSINCTL: 0x118[3:0]: (unit : 1 DRAMC clock)
    // From coarse tune 22, so set DQSINCTL = 4 first....or (DQS_GW_COARSE_START)>>2.
    // According to Derping, 6595 will only check DQSINCTL and ignore R1DQSINCTL because of P1 supporting. So R1DQSINCTL may be removed. But here still set the same value as DQSINCTL for safety.
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQSCTL2), &u4value);
    mcSET_FIELD(u4value, u4CoarseTuneStart, MASK_DQSCTL2_DQSINCTL, POS_DQSCTL2_DQSINCTL);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQSCTL2), u4value);

    // TXDLY_R1DQSGATE:      0x418[6:4] (unit : 1 DRAMC clock)
    // TXDLY_R1DQSGATE_P1: 0x418[10:8] (unit : 1 DRAMC clock)
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SELPH6_1), &u4value);
    u4Temp = (curr_val>>2) - (u4CoarseTuneStart);
    mcSET_FIELD(u4value, u4Temp, MASK_SELPH6_1_TXDLY_R1DQSGATE, POS_SELPH6_1_TXDLY_R1DQSGATE);
    u4Temp = (curr_val_P1>>2) - (u4CoarseTuneStart);
    mcSET_FIELD(u4value, u4Temp, MASK_SELPH6_1_TXDLY_R1DQSGATE_P1, POS_SELPH6_1_TXDLY_R1DQSGATE_P1);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SELPH6_1), u4value);

    // dly_R1DQSGATE:      0x418[1:0] (unit : 0.25 DRAMC clock)
    // dly_R1DQSGATE_P1: 0x418[3:2] (unit : 0.25 DRAMC clock)
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SELPH6_1), &u4value);
    mcSET_FIELD(u4value, curr_val&0x3, MASK_SELPH6_1_dly_R1DQSGATE, POS_SELPH6_1_dly_R1DQSGATE);
    mcSET_FIELD(u4value, curr_val_P1&0x3, MASK_SELPH6_1_dly_R1DQSGATE_P1, POS_SELPH6_1_dly_R1DQSGATE_P1);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SELPH6_1), u4value);

}
#endif

void dqsi_gw_dly_fine_factor_handler_rank1(DRAMC_CTX_T *p, U8 curr_val) 
{
    U8 ucstatus = 0;
    U32 u4value;

    // R1DQS?IEN: 0x98 (each with 7 bits)
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_R1DQSIEN), &u4value);
    mcSET_FIELD(u4value, curr_val&0x7f, MASK_DQSIEN_R1DQS0IEN, POS_DQSIEN_R1DQS0IEN);
    mcSET_FIELD(u4value, curr_val&0x7f, MASK_DQSIEN_R1DQS1IEN, POS_DQSIEN_R1DQS1IEN);
    mcSET_FIELD(u4value, curr_val&0x7f, MASK_DQSIEN_R1DQS2IEN, POS_DQSIEN_R1DQS2IEN);
    mcSET_FIELD(u4value, curr_val&0x7f, MASK_DQSIEN_R1DQS3IEN, POS_DQSIEN_R1DQS3IEN);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_R1DQSIEN), u4value);
}

void dqs_gw_counter_reset(DRAMC_CTX_T *p) 
{
    U8 ucstatus = 0;//, ucref_cnt;
    U32 u4value;
    
    // reset dqs_counter ,we should set the bit to 1,then to 0, it can't return 0 by itself
    // 0x1e4[9] = 1 -> 0
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD), &u4value);
    mcSET_BIT(u4value, POS_SPCMD_DQSGCNTRST);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD), u4value);
    mcCLR_BIT(u4value, POS_SPCMD_DQSGCNTRST);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD), u4value);

    DramcPhyReset(p);    
}

void dqs_gw_result(DRAMC_CTX_T *p)
{
    U8 ucstatus = 0;
    U32 u4coarse_result01, u4coarse_result23, u4golden_counter;

    // enable TE2, audio pattern
    DramcEngine2(p, TE_OP_READ_CHECK, 0x55000000, 0xaa000000 | DQS_GW_TE_OFFSET, 1, 0, 0, 0);

    // Get 0x3c0, 0x3c4 : coarse result of DQS0, 1, 2, 3
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQSGNWCNT0), &u4coarse_result01);
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQSGNWCNT1), &u4coarse_result23);

    mcSHOW_DBG_MSG(("%x    %x\n", u4coarse_result01, u4coarse_result23));

    u4golden_counter = DQS_GW_GOLD_COUNTER_32BIT;
    
    if ((u4coarse_result01 == u4golden_counter) && (u4coarse_result23 == u4golden_counter))    
    {
        dqs_gw[dqs_gw_coarse] |= (1 << dqs_gw_fine); //attension:dqs_gw_fine steps must less then 32
    }    

    dqs_gw_fine++;
    dqs_gw_fine_cnt++;
    
    if ((dqs_gw_fine >= DQS_GW_LEN_PER_COARSE_ELEMENT) || (dqs_gw_fine_cnt >= DQS_GW_FINE_MAX))
    {
        dqs_gw_coarse++; 
        dqs_gw_fine = 0;
        
        if (dqs_gw_fine_cnt >= DQS_GW_FINE_MAX)
        {
            dqs_gw_fine_cnt = 0;
        }        
    }
    if (dqs_gw_coarse > DQS_GW_LEN) 
    {
        mcSHOW_ERR_MSG(("Critical error!! dqs_gw_coarse > DQS_GW_LEN\n"));
    }

    
}

void dqs_gw_calib(DRAMC_CTX_T *p)
{
    U8 i, j;
    //U32 u4value;
    
    for ( i = DQS_GW_COARSE_START; i <= DQS_GW_COARSE_END; i+=DQS_GW_COARSE_STEP) 
    {
        //adjust factor steps
        dqsi_gw_dly_coarse_factor_handler(p, i, 0xff);        

        for ( j = DQS_GW_FINE_START; j <= DQS_GW_FINE_END; j+=DQS_GW_FINE_STEP) 
        {
            //adjust factor steps
            dqsi_gw_dly_fine_factor_handler(p, j);

            mcSHOW_DBG_MSG(("COARSE TUNE: %d; FINE TUNE: %d -- ", i, j));
            //ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQSCTL1), &u4value);
            //mcSHOW_DBG_MSG(("%x, ", u4value));
            //ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x404), &u4value);
            //mcSHOW_DBG_MSG(("%x, ", u4value));
            //ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x40c), &u4value);
            //mcSHOW_DBG_MSG(("%x, ", u4value));
            
            // gating counter, phy and read data counter reset
            dqs_gw_counter_reset(p);

            //run test code
            dqs_gw_result(p);
        }
        dqs_gw_fine = 0;    
        dqs_gw_fine_cnt = 0;
    }    
    dqs_gw_coarse = 0;

    // gating counter, phy and read data counter reset
    dqs_gw_counter_reset(p);
}

#ifdef USE_COARSE_TUNE_ALIGN_UPDATE
DRAM_STATUS_T DramcRxdqsGatingCal(DRAMC_CTX_T *p, unsigned int use_default_dqsinctl)
#else
DRAM_STATUS_T DramcRxdqsGatingCal(DRAMC_CTX_T *p)
#endif
{
    U8 ucstatus = 0;
    U32 u4value, dqs_gw_val;
    U8 i, j, k, opt_coarse_idx = 0, opt_fine_idx = 0, cnt, first_bit_idx, max=0;
#ifdef fcNEW_GATING_FINETUNE_LIMIT
    U8 opt_fine_idx_0 = 0, first_bit_idx_0 = 0;
#endif

#ifdef fcNEW_GATING_FINETUNE_LIMIT_2
    U8 opt_fine_idx_new = 0xff, opt_fine_idx_old = 0xff;
#endif

#ifdef FINETUNE_CENTER
    U8 MinCenterDiff = 0xff, CenterDiff, fine_idx = 0;
#endif

#ifndef RELEASE    
    mcSHOW_DBG_MSG(("\n\n[Channel %d] [Rank %d] Start tuning DRAMC Gating Window for rank 0...\n\n", p->channel, CurrentRank));
#else
    mcSHOW_USER_MSG(("\nR%d GW\n", CurrentRank));
#endif

    dqs_gw_coarse = 0;      //from begin of coarse tune, reset to 0
    dqs_gw_fine = 0;          //from begin of fine tune, reset to 0
    dqs_gw_fine_cnt = 0;   //from begin of fine tune, reset to 0
    dqs_gw_cnt_break = 0;

    for (i = 0; i < DQS_GW_LEN; i++) 
    {
        dqs_gw[i] = 0;
    }

    #if 1
    // Disable HW gating first
    // 0x1c0[31]
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQSCAL0), &u4value);
    mcCLR_BIT(u4value, POS_DQSCAL0_STBCALEN);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQSCAL0), u4value);
    #endif
    
    /* 1.enable burst mode for gating window */
    /*   enable DQS gating window counter */
    // 0x0e0[28] = 1
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQSCTL1), &u4value);
    mcSET_BIT(u4value, POS_DQSCTL1_DQSIENMODE);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQSCTL1), u4value);
    // 0x1e4[8] = 1
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD), &u4value);
    mcSET_BIT(u4value, POS_SPCMD_DQSGCNTEN);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD), u4value);

    // new gating, dual-phase DQS clock gating control enabling
    // 0x124[30] = 1
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQSGCTL), &u4value);
    mcSET_BIT(u4value, POS_DQSGCTL_DQSGDUALP);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQSGCTL), u4value);

    dqs_gw_calib(p);

    for (i = 0; i < DQS_GW_COARSE_MAX; i++) 
    {
        // find the max passed window
        cnt = 0;
        dqs_gw_cnt_break = 0;
        for (j = 0; j < DQS_GW_LEN_PER_COARSE; j++)
        {        
            cnt += nr_bit_set(dqs_gw[i*DQS_GW_LEN_PER_COARSE + j]);
            if (dqs_gw_cnt_break == 1)
            {
                // Has count but the next is "0", break
                break;
            }
        }

    #ifdef fcNEW_GATING_FINETUNE_LIMIT_2
        if (cnt) 
        {
            for (j = 0; j < DQS_GW_LEN_PER_COARSE; j++)
            {
                first_bit_idx = first_bit_set(dqs_gw[i*DQS_GW_LEN_PER_COARSE+j]);
                if (first_bit_idx != (0xff))
                {
                    opt_fine_idx_new = j*DQS_GW_LEN_PER_COARSE_ELEMENT + first_bit_idx + cnt / 2;                    
                    break;
                }
            }

            mcSHOW_DBG_MSG(("[New Gating with FineTune Limit2] cnt=%d, first_bit_idx=%d, coarse_idx=%d, opt_fine_idx_new=%d,  opt_fine_idx_old=%d\n", cnt, first_bit_idx, i, opt_fine_idx_new, opt_fine_idx_old));                
            if (opt_fine_idx_new > opt_fine_idx_old)
            {
                mcSHOW_DBG_MSG(("[New Gating with FineTune Limit2] Current OPT finetune > Previous. Fake window. BREAK!!\n"));
                
                break;
            }
            else
            {
                opt_fine_idx_old = opt_fine_idx_new;
            }
        }
    #endif

    #ifdef FINETUNE_CENTER
        if (cnt)
        {
      for (j = 0; j < DQS_GW_LEN_PER_COARSE; j++)
      {
        first_bit_idx = first_bit_set(dqs_gw[i*DQS_GW_LEN_PER_COARSE+j]);
        if (first_bit_idx != (0xff))
        {
          fine_idx = j*DQS_GW_LEN_PER_COARSE_ELEMENT + first_bit_idx + cnt / 2;                    
          break;
        }
      }    
      fine_idx = DQS_GW_FINE_START+fine_idx*DQS_GW_FINE_STEP;

      if (fine_idx > (DQS_GW_FINE_END>>1))
      {
        CenterDiff = fine_idx - (DQS_GW_FINE_END>>1);
      }
      else
      {
        CenterDiff = (DQS_GW_FINE_END>>1) - fine_idx ;
      }
      mcSHOW_DBG_MSG(("first_bit_idx=%d, fine_idx=%d, CenterDiff=%d, MinCenterDiff=%d\n", 
      first_bit_idx, fine_idx, CenterDiff, MinCenterDiff));
#ifdef USE_COARSE_TUNE_ALIGN_UPDATE
          if (use_default_dqsinctl == 0xff)
          {
              if (CenterDiff < MinCenterDiff)
              {
                MinCenterDiff = CenterDiff;
                max = cnt;
                opt_coarse_idx = i;
              }
          }
          else
          {
            if (((i*DQS_GW_LEN_PER_COARSE+j)+DQS_GW_COARSE_START) == use_default_dqsinctl)
            {
                MinCenterDiff = CenterDiff;
                max = cnt;
                opt_coarse_idx = i;
            }
          }
#else
          if (CenterDiff < MinCenterDiff)
          {
            MinCenterDiff = CenterDiff;
            max = cnt;
            opt_coarse_idx = i;
          }
#endif
          if (first_bit_idx==0)
          {
            break;
          }
        }
    #else            
        if (cnt >= max)   // Edward : coarse tune with left window has smaller shift in case of voltage change.
        {
            max = cnt;
            opt_coarse_idx = i;
        }
     #endif
    }

#ifdef DUAL_RANKS
// MT6595 coarse tune could be set into different values in dual ranks no matter 0.25T or 1T. If something wrong, need to enable this again.  
#endif

#ifdef CUSTOMIZE_COARSE_TUNE
    // 1 M_CK difference + 0.25 M_CK difference.  R0>R1 (28, 27)
    // 1 M_CK difference + 0.25 M_CK difference.  R0<R1   (27, 28)
    // 1 M_CK difference.  R0>R1  (28,24)  (29,25)
    // 1 M_CK difference.  R0<R1  (24,28)  (29,25)
    // 0.25 M_CK difference.  R0>R1 (27,24) (26,25)
    // 0.25 M_CK difference.  R0<R1 (24,27) (25,26)
    if (CurrentRank==0)
    {
      if (p->channel==CHANNEL_A)
        opt_coarse_idx = 30;
      else
        opt_coarse_idx = 29;
    }
    else
    {
      if (p->channel==CHANNEL_A)
        opt_coarse_idx = 28;
      else
        opt_coarse_idx = 27;
    }
    opt_coarse_idx -=DQS_GW_COARSE_START;
    for (j = 0; j < DQS_GW_LEN_PER_COARSE; j++)
    {        
        cnt += nr_bit_set(dqs_gw[opt_coarse_idx*DQS_GW_LEN_PER_COARSE + j]);
        if (dqs_gw_cnt_break == 1)
        {
            // Has count but the next is "0", break
            break;
        }
    }  
    max = cnt;
#endif

    cnt = max;
    if (cnt) 
    {
        for (i = 0; i < DQS_GW_LEN_PER_COARSE; i++)
        {
            first_bit_idx = first_bit_set(dqs_gw[opt_coarse_idx*DQS_GW_LEN_PER_COARSE+i]);
            if (first_bit_idx != (0xff))
            {
                opt_fine_idx = i*DQS_GW_LEN_PER_COARSE_ELEMENT + first_bit_idx + cnt / 2;
                break;
            }
        }        

    #ifdef fcNEW_GATING_FINETUNE_LIMIT   // to filter fake passed window
        for (i = 0; i < DQS_GW_LEN_PER_COARSE; i++)
        {
            first_bit_idx_0 = first_bit_set(dqs_gw[(opt_coarse_idx-1)*DQS_GW_LEN_PER_COARSE+i]);
            if (first_bit_idx_0 != (0xff))
            {
                opt_fine_idx_0 = i*DQS_GW_LEN_PER_COARSE_ELEMENT + first_bit_idx_0;
                break;
            }
        }

        mcSHOW_DBG_MSG(("[New Gating with FineTune Limit] opt_coarse_idx=%d/%d, first_bit_idx=%d/%d, opt_fine_idx=%d/%d\n", opt_coarse_idx-1, opt_coarse_idx, first_bit_idx_0, first_bit_idx, opt_fine_idx_0, opt_fine_idx));

        if ((first_bit_idx_0>first_bit_idx) && (opt_fine_idx_0 < opt_fine_idx))
        {
            opt_fine_idx = opt_fine_idx_0;
            mcSHOW_DBG_MSG(("[New Gating with FineTune Limit] Limit opt_fine_idx to %d!!\n", opt_fine_idx_0));
        }        
    #endif
        
        opt_gw_coarse_value = DQS_GW_COARSE_START+opt_coarse_idx*DQS_GW_COARSE_STEP;
        opt_gw_fine_value = DQS_GW_FINE_START+opt_fine_idx*DQS_GW_FINE_STEP;

#ifdef DUAL_RANKS
  if (CurrentRank==0) 
  {
    opt_gw_coarse_value_R0[p->channel] = opt_gw_coarse_value;
    opt_gw_fine_value_R0[p->channel] = opt_gw_fine_value;
  }
  else
  {
    opt_gw_coarse_value_R1[p->channel] = opt_gw_coarse_value;
    opt_gw_fine_value_R1[p->channel] = opt_gw_fine_value;
  }
#endif  
  
  
        mcSHOW_DBG_MSG(("*********************************************************\n"));
        mcSHOW_DBG_MSG(("Rank %d DQS GW Calibration\n", CurrentRank));
        mcSHOW_DBG_MSG(("Optimal coarse tune value %d, optimal fine tune value %d\n", opt_gw_coarse_value, opt_gw_fine_value));
        mcSHOW_DBG_MSG(("*********************************************************\n"));
      
        for (i = 0; i < DQS_GW_FINE_MAX; i++)
        {
            if ((i%16) == 0)
            {
                mcSHOW_USER_MSG(("\n    "));
            }
            mcSHOW_USER_MSG(("%d", DQS_GW_FINE_START+i*DQS_GW_FINE_STEP));
        }
        mcSHOW_USER_MSG(("\n     ---------------------------------------------------------------\n"));
        
        for (i = 0; i < DQS_GW_COARSE_MAX; i++)
        {
            for (j = 0; j < DQS_GW_LEN_PER_COARSE; j++)
            {
                dqs_gw_val = dqs_gw[i*DQS_GW_LEN_PER_COARSE+j];
                for (k = 0; k < DQS_GW_LEN_PER_COARSE_ELEMENT; k++)
                {
                    if ((k%16) == 0)
                    {
                        mcSHOW_USER_MSG(("\n%d|",DQS_GW_COARSE_START+i*DQS_GW_COARSE_STEP));
                    }
                    mcSHOW_USER_MSG(("%d", ((dqs_gw_val >> k) & 0x1)));
                    if ((j == (DQS_GW_LEN_PER_COARSE-1)) && ((j*DQS_GW_LEN_PER_COARSE_ELEMENT+k) == (DQS_GW_FINE_MAX-1)))
                    {
                        break;
                    }
                }
            }
        }

        mcSHOW_USER_MSG(("\n"));

        /* setup the opt coarse value and fine value according to calibration result*/
        dqsi_gw_dly_coarse_factor_handler(p, opt_gw_coarse_value, 0xff);
        dqsi_gw_dly_fine_factor_handler(p, opt_gw_fine_value);        
    }
    else 
    {    
        mcSHOW_ERR_MSG(("Cannot find any pass-window\n"));
    #ifdef DDR_FT_LOAD_BOARD
        LoadBoardShowResult(FLAG_GATING_CALIBRATION, FLAG_CALIBRATION_FAIL, p->channel, FLAG_NOT_COMPLETE_OR_FAIL);            
        while(1); 
    #endif
    }

#ifdef DDR_FT_LOAD_BOARD
    // To check gating window size reasonable?! (consider fake passed window)
#endif
    
    /* disable DQS gating window counter */
    // 0x1e4[8] = 0
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD), &u4value);
    mcCLR_BIT(u4value, POS_SPCMD_DQSGCNTEN);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD), u4value);
    
    #if 0 // for testing, move to the last
    // Enable HW gating here?!
    // 0x1c0[31]
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQSCAL0), &u4value);
    mcSET_BIT(u4value, POS_DQSCAL0_STBCALEN);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQSCAL0), u4value);
    #endif
    
// DramcRxdqsGatingCal_exit:
    if (ucstatus)
    {
        mcSHOW_ERR_MSG(("register access fail!\n"));
        return DRAM_FAIL;
    }
    else
    {
        return DRAM_OK;
    }

/*
          0    8   16   24   32   40   48   56   64   72   80   88   96  104  112  120
      --------------------------------------------------------------------------------
0007:|    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0
0008:|    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0
0009:|    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0
000A:|    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0
000B:|    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0
000C:|    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0
000D:|    0    0    0    0    0    0    0    0    0    0    0    0    1    1    1    1
000E:|    0    0    0    0    0    0    0    1    1    1    1    1    1    1    1    1
000F:|    0    0    0    1    1    1    1    1    1    1    1    0    0    0    0    0
0010:|    1    1    1    1    1    1    1    0    0    0    0    0    0    0    0    0
0011:|    1    1    1    0    0    0    0    0    0    0    0    0    0    0    0    0
0012:|    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0
0013:|    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0
0014:|    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0
0015:|    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0
0016:|    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0
*/
}

#ifdef MATYPE_ADAPTATION  
U8 MATYPE_Adaptation(DRAMC_CTX_T *p, U32 u4Rank)
{
  U8 ucstatus = 0;
  U32 u4value, u4Rank_Col;

  // Modify MA type based on EMI_CONA
  u4value = *(volatile unsigned *)(EMI_APB_BASE+0x0);

  if (u4Rank == 0)
  {
    // Rank 0.
    if (p->channel == CHANNEL_A)
    {
      // Channel A
      u4Rank_Col = mcGET_FIELD(u4value, 0x00000030, 4);
    }
    else
    {
      // Channel B.
      u4Rank_Col = mcGET_FIELD(u4value, 0x00300000, 20);
    }
  }
  else
  {
    // Rank 1.
    if (p->channel == CHANNEL_A)
    {
      // Channel A
      u4Rank_Col = mcGET_FIELD(u4value, 0x000000c0, 6);
    }
    else
    {
      // Channel B.
      u4Rank_Col = mcGET_FIELD(u4value, 0x00c00000, 22);
    }
  }

  ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x04), &u4value);
  
  if (u4Rank_Col<3) 
  {
          mcSET_FIELD(u4value, u4Rank_Col+1, 0x00000300, 8);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x04), u4value);
  }
  else
  {
    // Should not be 3. Something wrong...
    return 1;
  }   

  return ucstatus;
}
#endif

#ifdef ADJUST_CROSS_RANK_TIMING
DRAM_STATUS_T DualRankDramcRxdqsGatingCal(DRAMC_CTX_T *p)
{
  U8 ucstatus = 0;
  U32 uiTemp, DQSINCTL;
  DRAM_STATUS_T Ret = DRAM_OK;

  // Rank 0 GW calibration.
  CurrentRank = 0;
#ifdef MATYPE_ADAPTATION  
  MATYPE_Adaptation(p, 0);
#endif

#ifdef USE_COARSE_TUNE_ALIGN_UPDATE
  DramcRxdqsGatingCal(p, 0xff); 
#else
  DramcRxdqsGatingCal(p); 
#endif

  // Get Reg.e0h[27:24] DQSINCTL after rank 0 calibration.
  ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xe0), &DQSINCTL);  
  DQSINCTL = (DQSINCTL >> 24) & 0xf;


  // Rank 1 GW calibration.
  // Swap CS0 and CS1.
  ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x110), &uiTemp);
  uiTemp = uiTemp |0x08;
  ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x110), uiTemp);

  CurrentRank = 1;
#ifdef MATYPE_ADAPTATION  
  MATYPE_Adaptation(p, 1);
#endif

#ifdef USE_COARSE_TUNE_ALIGN_UPDATE
  DramcRxdqsGatingCal(p, 0xff); 
#else
  DramcRxdqsGatingCal(p); 
#endif

#ifdef USE_COARSE_TUNE_ALIGN_UPDATE
  if ((opt_gw_coarse_value_R0[p->channel]/4) < (opt_gw_coarse_value_R1[p->channel]/4))
  {
    DramcRxdqsGatingCal(p, opt_gw_coarse_value_R0[p->channel]);  
    dqsi_gw_dly_coarse_factor_handler_rank1(p, opt_gw_coarse_value_R0[p->channel], 0xff);
    dqsi_gw_dly_fine_factor_handler_rank1(p, opt_gw_fine_value_R0[p->channel]); 

  // Swap CS back.
  ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x110), &uiTemp);
  uiTemp = uiTemp & (~0x08);
  ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x110), uiTemp);

  CurrentRank = 0;   

#ifdef MATYPE_ADAPTATION  
  MATYPE_Adaptation(p, 0);
#endif  
  // Set rank 0 coarse tune and fine tune back.
        dqsi_gw_dly_coarse_factor_handler(p, opt_gw_coarse_value_R0[p->channel], 0xff);
        dqsi_gw_dly_fine_factor_handler(p, opt_gw_fine_value_R0[p->channel]);   
  }
  else if ((opt_gw_coarse_value_R0[p->channel]/4) > (opt_gw_coarse_value_R1[p->channel]/4))
  {
    dqsi_gw_dly_coarse_factor_handler_rank1(p, opt_gw_coarse_value_R1[p->channel], 0xff);
        dqsi_gw_dly_fine_factor_handler_rank1(p, opt_gw_fine_value_R1[p->channel]);   

  // Swap CS back.
  ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x110), &uiTemp);
  uiTemp = uiTemp & (~0x08);
  ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x110), uiTemp);

  CurrentRank = 0;  

    DramcRxdqsGatingCal(p, opt_gw_coarse_value_R1[p->channel]);
  }
  else
  {
      dqsi_gw_dly_coarse_factor_handler_rank1(p, opt_gw_coarse_value_R1[p->channel], 0xff);
      dqsi_gw_dly_fine_factor_handler_rank1(p, opt_gw_fine_value_R1[p->channel]);   

      // Swap CS back.
      ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x110), &uiTemp);
      uiTemp = uiTemp & (~0x08);
      ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x110), uiTemp);

      CurrentRank = 0;  

#ifdef MATYPE_ADAPTATION  
  MATYPE_Adaptation(p, 0);
#endif  
  // Set rank 0 coarse tune and fine tune back.
        dqsi_gw_dly_coarse_factor_handler(p, opt_gw_coarse_value_R0[p->channel], 0xff);
        dqsi_gw_dly_fine_factor_handler(p, opt_gw_fine_value_R0[p->channel]);         
  }
#else
  if (opt_gw_coarse_value_R0[p->channel] <= opt_gw_coarse_value_R1[p->channel])
  {
    //No need to set rank 1 coarse tune because only one set registers for coarse tune in 808
      dqsi_gw_dly_coarse_factor_handler_rank1(p, opt_gw_coarse_value_R1[p->channel], DQSINCTL);
      dqsi_gw_dly_fine_factor_handler_rank1(p, opt_gw_fine_value_R1[p->channel]);   
      DQSINCTL = 0xff;
  }
  else
  {
    DQSINCTL = opt_gw_coarse_value_R1[p->channel] >> 2;
  //No need to set rank 1 coarse tune because only one set registers for coarse tune in 808
        dqsi_gw_dly_coarse_factor_handler_rank1(p, opt_gw_coarse_value_R1[p->channel], DQSINCTL);
        dqsi_gw_dly_fine_factor_handler_rank1(p, opt_gw_fine_value_R1[p->channel]);   
  }

  // Swap CS back.
  ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x110), &uiTemp);
  uiTemp = uiTemp & (~0x08);
  ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x110), uiTemp);

  CurrentRank = 0;
#ifdef MATYPE_ADAPTATION  
  MATYPE_Adaptation(p, 0);
#endif  
  // Set rank 0 coarse tune and fine tune back.
        dqsi_gw_dly_coarse_factor_handler(p, opt_gw_coarse_value_R0[p->channel], DQSINCTL);
        dqsi_gw_dly_fine_factor_handler(p, opt_gw_fine_value_R0[p->channel]);         
#endif

		
        return Ret;
}

#else
DRAM_STATUS_T DualRankDramcRxdqsGatingCal(DRAMC_CTX_T *p)
{
  U8 ucstatus;
  U32 uiTemp, DQSINCTL;
  DRAM_STATUS_T Ret = DRAM_OK;

  // Rank 0 GW calibration.
  CurrentRank = 0;
#ifdef MATYPE_ADAPTATION  
  MATYPE_Adaptation(p, 0);
#endif

  
#ifdef USE_COARSE_TUNE_ALIGN_UPDATE
  DramcRxdqsGatingCal(p, 0xff); 
#else
  DramcRxdqsGatingCal(p); 
#endif

  // Get Reg.e0h[27:24] DQSINCTL after rank 0 calibration.
  ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xe0), &DQSINCTL);  
  DQSINCTL = (DQSINCTL >> 24) & 0xf;


  // Rank 1 GW calibration.
  // Swap CS0 and CS1.
  ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x110), &uiTemp);
  uiTemp = uiTemp |0x08;
  ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x110), uiTemp);

  CurrentRank = 1;
#ifdef MATYPE_ADAPTATION  
  MATYPE_Adaptation(p, 1);
#endif

#ifdef USE_COARSE_TUNE_ALIGN_UPDATE
  DramcRxdqsGatingCal(p, 0xff); 
#else
  DramcRxdqsGatingCal(p); 
#endif


  //No need to set rank 1 coarse tune because only one set registers for coarse tune in 808
        dqsi_gw_dly_coarse_factor_handler_rank1(p, opt_gw_coarse_value_R1[p->channel], DQSINCTL);
        dqsi_gw_dly_fine_factor_handler_rank1(p, opt_gw_fine_value_R1[p->channel]);     
        
  // Swap CS back.
  ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x110), &uiTemp);
  uiTemp = uiTemp & (~0x08);
  ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x110), uiTemp);

  CurrentRank = 0;
#ifdef MATYPE_ADAPTATION  
  MATYPE_Adaptation(p, 0);
#endif  
  // Set rank 0 coarse tune and fine tune back.
        dqsi_gw_dly_coarse_factor_handler(p, opt_gw_coarse_value_R0[p->channel], 0xff);
        dqsi_gw_dly_fine_factor_handler(p, opt_gw_fine_value_R0[p->channel]);        

        return Ret;
}
#endif
#endif

#ifdef ENABLE_DQSI_PER_RANK
DRAM_STATUS_T DramcRxWindowPerbitCal_RK1(DRAMC_CTX_T *p)
{
  U8 ucstatus = 0, jj;
  U32 u4value;
  //U32 uiReg18h_backup;
  
  // Swap CS0 and CS1.
  ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x110), &u4value);
  u4value = u4value |0x08;
  ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x110), u4value);  

  // Backup rank 0 DQS input delay.
  //ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x18), &uiReg18h_backup);

  //DramcRxWindowDQSCal((DRAMC_CTX_T *)p);
  DramcRxWindowPerbitCal((DRAMC_CTX_T *)p);
  
  // Copy DQS input delay to rank 1.
  ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x18), &u4value);
  ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1c), u4value); 
  
  // Restore rank 0 DQS input delay.
  //ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x18), uiReg18h_backup); 

  // Swap CS back.
  ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x110), &u4value);
  u4value = u4value & (~0x08);
  ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x110), u4value);  

  // Enable DQS input delay switching for different ranks.
  ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x100), &u4value);
  u4value = u4value |(0x01 << 15);
  ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x100), u4value);  
}

DRAM_STATUS_T DramcRxRegPrint(DRAMC_CTX_T *p)
{
  U8 ucstatus = 0, jj;
  U32 u4value;
  
  ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DELDLY1), &u4value);
  mcSHOW_DBG_MSG(("Reg.%xh=%xh\n", DRAMC_REG_DELDLY1, u4value));
  ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1c), &u4value);
  mcSHOW_DBG_MSG(("Reg.%xh=%xh\n", 0x01c, u4value));    

  for (jj=0; jj<p->data_width; jj=jj+4)
  {
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQIDLY1+jj), &u4value);
    mcSHOW_DBG_MSG(("Reg.%xh=%xh\n", DRAMC_REG_DQIDLY1+jj, u4value));
  }         
}
#endif

//-------------------------------------------------------------------------
/** DramcRxWindowPerbitCal (v2 version)
 *  start the rx dqs perbit sw calibration.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL 
 */
//-------------------------------------------------------------------------
DRAM_STATUS_T DramcRxWindowPerbitCal(DRAMC_CTX_T *p)
{
    U8 ucstatus = 0, fgfail, ii, jj, kk;
    U32 u4value, u4err_value, u4fail_bit;

    RXDQS_PERBIT_DLY_T dqdqs_perbit_dly[DQ_DATA_WIDTH];
    U8 ucbit_first, ucbit_last, uchold_pass_number, ucsetup_pass_number;
    U8 ucmax_dqsdly_byte[DQS_NUMBER];
    
    // error handling
    if (!p)
    {
        mcSHOW_ERR_MSG(("context is NULL\n"));
        return DRAM_FAIL;
    }    

    // for debug
    mcSHOW_DBG_MSG4(("\n[Channel %d] RX DQS per bit para : test pattern=%d, test2_1=0x%x, test2_2=0x%x, LSW_HSR=%d\n", p->channel, p->test_pattern, p->test2_1, p->test2_2, p->fglow_freq_write_en));    

    // 1.delay DQ ,find the pass widnow (left boundary).
    // 2.delay DQS find the pass window (right boundary). 
    // 3.Find the best DQ / DQS to satify the middle value of the overall pass window per bit
    // 4.Set DQS delay to the max per byte, delay DQ to de-skew

    // 1
    // set DQS delay to 0 first
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DELDLY1), 0);

    fgfail = 0;

    // initialize parameters
    for (ii = 0; ii < p->data_width; ii++)
    {
        dqdqs_perbit_dly[ii].first_dqdly_pass = -1;
        dqdqs_perbit_dly[ii].last_dqdly_pass = -2;
        dqdqs_perbit_dly[ii].first_dqsdly_pass = -1;
        dqdqs_perbit_dly[ii].last_dqsdly_pass = -2;
        dqdqs_perbit_dly[ii].best_first_dqdly_pass = -1;
        dqdqs_perbit_dly[ii].best_last_dqdly_pass = -2;
        dqdqs_perbit_dly[ii].best_first_dqsdly_pass = -1;
        dqdqs_perbit_dly[ii].best_last_dqsdly_pass = -2;        
    }

#ifndef RELEASE   
    mcSHOW_DBG_MSG4(("------------------------------------------------------\n")); 
    mcSHOW_DBG_MSG4(("Start DQ delay to find pass range, DQS delay fixed to 0...\n"));
    mcSHOW_DBG_MSG4(("------------------------------------------------------\n")); 
    mcSHOW_DBG_MSG4(("x-axis is bit #; y-axis is DQ delay (%d~%d)\n", 0, MAX_RX_DQDLY_TAPS-1));
#else
    mcSHOW_USER_MSG(("\nRX\n"));
#endif
    
    // delay DQ from 0 to 15 to get the setup time
    for (ii = 0; ii < MAX_RX_DQDLY_TAPS; ii++)
    {
        for (jj=0; jj<p->data_width; jj=jj+4)
        {
            //every 4bit dq have the same delay register address
            u4value = ((U32) ii) + (((U32)ii)<<8) + (((U32)ii)<<16) + (((U32)ii)<<24);  
            ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQIDLY1+jj), u4value);            
        }

/*
No need due to no RX DQS glitch for A60808
        //Reset after scan to avoid error gating counter due to DQS glitch
        //reset phy R_DMPHYRST: 0xf0[28] 
        // 0x0f0[28] = 1 -> 0
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1), &u4value);
        mcSET_BIT(u4value, POS_PHYCTL1_PHYRST);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1), u4value);
        //delay 10ns, 1ms here
        mcDELAY_MS(1);
        mcCLR_BIT(u4value, POS_PHYCTL1_PHYRST);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1), u4value);

        // read data counter reset
        // 0x0f4[25] = 1 -> 0
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1), &u4value);
        mcSET_BIT(u4value, POS_GDDR3CTL1_RDATRST);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1), u4value);
        //delay 10ns, 1ms here
        mcDELAY_MS(1);                
        mcCLR_BIT(u4value, POS_GDDR3CTL1_RDATRST);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1), u4value);       
*/
#ifdef XTALK_SSO_K
  *(volatile unsigned *)(EMI_APB_BASE+0x00000000) &= (~0x01); // disable dual channel.
  if (p->channel == CHANNEL_A)
  {
    *(volatile unsigned *)(EMI_APB_BASE+0x00000000) &= (~(0x01<<18));
  }
  else
  {
    *(volatile unsigned *)(EMI_APB_BASE+0x00000000) |= ((0x01<<18));
  }
        u4err_value = DramcDmaEngine((DRAMC_CTX_T *)p, DMA_OP_READ_WRITE, DDR_BASE, 
          DDR_BASE+0x100000, XTALK_SSO_LEN<<3, 8, 1, 1, 0);

  #ifdef SINGLE_CHANNEL_ENABLE        
        *(volatile unsigned *)(EMI_APB_BASE+0x00000000) = LPDDR3_EMI_CONA;
  #else
        *(volatile unsigned *)(EMI_APB_BASE+0x00000000) |= (0x01);  // enable dual channel.
        #endif
#else
        if (p->test_pattern== TEST_AUDIO_PATTERN)
        {
            // enable TE2, audio pattern
            if (p->fglow_freq_write_en == ENABLE)
            {
                u4err_value = DramcEngine2(p, TE_OP_READ_CHECK, p->test2_1, p->test2_2, 1, 0, 0, 0);
            }
            else
            {
                u4err_value = 0;
                for (jj = 0; jj < 1; jj++)
                {
                    u4err_value |= DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 1, 0, 0, 0);
                }
            }
        }
        else if (p->test_pattern == TEST_ISI_PATTERN)
        {
            // enable TE2, ISI pattern
            u4err_value = 0;
            for (jj = 0; jj < 1; jj++)
            {
                u4err_value |= DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 0, 0, 0, 0);
            }            
        }
        else if (p->test_pattern == TEST_XTALK_PATTERN)
        {
            if (p->fglow_freq_write_en == ENABLE)
            {
                u4err_value = DramcEngine2(p, TE_OP_READ_CHECK, p->test2_1, p->test2_2, 2, 0, 0, 0);
            }
            else
            {
                u4err_value = DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 2, 0, 0, 0);
            }
        }
        else if (p->test_pattern == TEST_MIX_PATTERN)
        {
            u4err_value = DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 1, 0, 0, 0);
            u4err_value |= DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 2, 0, 0, 0);
        }
        else
        {
            mcSHOW_ERR_MSG(("Not support test pattern!! Use audio pattern by default.\n"));
            u4err_value = DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 1, 0, 0, 0);
        }        
#endif

        // check fail bit ,0 ok ,others fail
        for (jj = 0; jj < p->data_width; jj++)
        {
            u4fail_bit = u4err_value&((U32)1<<jj);

            if (u4fail_bit == 0)
            {
                if (dqdqs_perbit_dly[jj].first_dqdly_pass == -1)
                {
                    // first DQ pass delay tap
                    dqdqs_perbit_dly[jj].first_dqdly_pass = ii;
                }
                if (dqdqs_perbit_dly[jj].last_dqdly_pass == -2)
                {
                    if (ii == (MAX_RX_DQDLY_TAPS-1))
                    {
                        // pass to the last tap
                        dqdqs_perbit_dly[jj].last_dqdly_pass = ii;
                        if ( (dqdqs_perbit_dly[jj].last_dqdly_pass-dqdqs_perbit_dly[jj].first_dqdly_pass) > (dqdqs_perbit_dly[jj].best_last_dqdly_pass-dqdqs_perbit_dly[jj].best_first_dqdly_pass))
                        {
                   dqdqs_perbit_dly[jj].best_last_dqdly_pass =  dqdqs_perbit_dly[jj].last_dqdly_pass;
                   dqdqs_perbit_dly[jj].best_first_dqdly_pass = dqdqs_perbit_dly[jj].first_dqdly_pass;
                        }   
                        // clear to find the next pass range if it has
                        dqdqs_perbit_dly[jj].first_dqdly_pass = -1;
                        dqdqs_perbit_dly[jj].last_dqdly_pass = -2;                        
                    }
                }
            }
            else
            {
                if ((dqdqs_perbit_dly[jj].first_dqdly_pass != -1)&&(dqdqs_perbit_dly[jj].last_dqdly_pass == -2))
                {
                    dqdqs_perbit_dly[jj].last_dqdly_pass = ii -1;
                    if ( (dqdqs_perbit_dly[jj].last_dqdly_pass-dqdqs_perbit_dly[jj].first_dqdly_pass) > (dqdqs_perbit_dly[jj].best_last_dqdly_pass-dqdqs_perbit_dly[jj].best_first_dqdly_pass))
                    {
               dqdqs_perbit_dly[jj].best_last_dqdly_pass =  dqdqs_perbit_dly[jj].last_dqdly_pass;
               dqdqs_perbit_dly[jj].best_first_dqdly_pass = dqdqs_perbit_dly[jj].first_dqdly_pass;
                    }   
                    // clear to find the next pass range if it has
                    dqdqs_perbit_dly[jj].first_dqdly_pass = -1;
                    dqdqs_perbit_dly[jj].last_dqdly_pass = -2;
                }
            }

                    
                
            if (u4fail_bit == 0)
            {
                mcSHOW_USER_MSG(("o"));
            }
            else
            {
                {
                    mcSHOW_USER_MSG(("x"));
                }
            }
            
        }
        mcSHOW_USER_MSG(("\n"));
    }

    // 2
    //set dq delay to 0
    for (ii=0; ii<p->data_width; ii=ii+4)
    {
        //every 4bit dq have the same delay register address
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQIDLY1+ii), 0);            
    }

#ifndef RELEASE  
    //dqs from 1
    mcSHOW_DBG_MSG4(("------------------------------------------------------\n")); 
    mcSHOW_DBG_MSG4(("Start DQS delay to find pass range, DQ delay fixed to 0...\n"));
    mcSHOW_DBG_MSG4(("------------------------------------------------------\n")); 
    mcSHOW_DBG_MSG4(("x-axis is bit #; y-axis is DQS delay (%d~%d)\n", 1, MAX_RX_DQSDLY_TAPS-1));
#else
    mcSHOW_USER_MSG(("\n"));
#endif
    
    // because the tap DQdly=0 DQSdly=0 will be counted when we delay dq ,so we don't count it here
    // so we set first dqs delay to 1
    for (ii = 1; ii < MAX_RX_DQSDLY_TAPS; ii++)
    {
        // 0x18
        u4value = 0;
        for (jj = 0; jj < (p->data_width/DQS_BIT_NUMBER); jj++)
        {
            u4value += (((U32)ii)<<(8*jj));        
        }
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DELDLY1), u4value);        

#ifdef XTALK_SSO_K
    *(volatile unsigned *)(EMI_APB_BASE+0x00000000) &= (~0x01); // disable dual channel.
    if (p->channel == CHANNEL_A)
    {
      *(volatile unsigned *)(EMI_APB_BASE+0x00000000) &= (~(0x01<<18));
    }
    else
    {
      *(volatile unsigned *)(EMI_APB_BASE+0x00000000) |= ((0x01<<18));
    }
        u4err_value = DramcDmaEngine((DRAMC_CTX_T *)p, DMA_OP_READ_WRITE, DDR_BASE, 
          DDR_BASE+0x100000, XTALK_SSO_LEN<<3, 8, 1, 1, 0);
        
  #ifdef SINGLE_CHANNEL_ENABLE        
        *(volatile unsigned *)(EMI_APB_BASE+0x00000000) = LPDDR3_EMI_CONA;
  #else
        *(volatile unsigned *)(EMI_APB_BASE+0x00000000) |= (0x01);  // enable dual channel.
        #endif
#else
        if (p->test_pattern== TEST_AUDIO_PATTERN)
        {
            // enable TE2, audio pattern
            if (p->fglow_freq_write_en == ENABLE)
            {
                u4err_value = DramcEngine2(p, TE_OP_READ_CHECK, p->test2_1, p->test2_2, 1, 0, 0, 0);
            }
            else
            {
                u4err_value = 0;
                for (jj = 0; jj < 1; jj++)
                {
                    u4err_value |= DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 1, 0, 0, 0);
                }
            }          
        }
        else if (p->test_pattern == TEST_ISI_PATTERN)
        {
            // enable TE2, ISI pattern
            u4err_value = 0;
            for (jj = 0; jj < 1; jj++)
            {
                u4err_value |= DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 0, 0, 0, 0);
            }            
        }
        else if (p->test_pattern == TEST_XTALK_PATTERN)
        {
            if (p->fglow_freq_write_en == ENABLE)
            {
                u4err_value = DramcEngine2(p, TE_OP_READ_CHECK, p->test2_1, p->test2_2, 2, 0, 0, 0);
            }
            else
            {
                u4err_value = DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 2, 0, 0, 0);
            }
        }
        else if (p->test_pattern == TEST_MIX_PATTERN)
        {
            u4err_value = DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 1, 0, 0, 0);
            u4err_value |= DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 2, 0, 0, 0);
        }
        else
        {
            mcSHOW_ERR_MSG(("not support test pattern\n"));
            u4err_value = DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 1, 0, 0, 0);
        }
#endif


        // check fail bit ,0 ok ,others fail
        for (jj = 0; jj < p->data_width; jj++)
        {
            u4fail_bit = u4err_value&((U32)1<<jj);

            if (u4fail_bit == 0)
            {
                if (dqdqs_perbit_dly[jj].first_dqsdly_pass == -1)
                {
                    // first DQS pass delay tap
                    dqdqs_perbit_dly[jj].first_dqsdly_pass = ii;
                }
                if (dqdqs_perbit_dly[jj].last_dqsdly_pass == -2)
                {
                    if (ii == (MAX_RX_DQSDLY_TAPS-1))
                    {
                        // pass to the last tap
                        dqdqs_perbit_dly[jj].last_dqsdly_pass = ii;
                        if ( (dqdqs_perbit_dly[jj].last_dqsdly_pass-dqdqs_perbit_dly[jj].first_dqsdly_pass) > (dqdqs_perbit_dly[jj].best_last_dqsdly_pass-dqdqs_perbit_dly[jj].best_first_dqsdly_pass))
                        {
                  dqdqs_perbit_dly[jj].best_last_dqsdly_pass =  dqdqs_perbit_dly[jj].last_dqsdly_pass;
                  dqdqs_perbit_dly[jj].best_first_dqsdly_pass = dqdqs_perbit_dly[jj].first_dqsdly_pass;
                        }   
                        // clear to find the next pass range if it has
                        dqdqs_perbit_dly[jj].first_dqsdly_pass = -1;
                        dqdqs_perbit_dly[jj].last_dqsdly_pass = -2;                               
                    }
                }
            }
            else
            {
                if ((dqdqs_perbit_dly[jj].first_dqsdly_pass != -1)&&(dqdqs_perbit_dly[jj].last_dqsdly_pass == -2))
                {
                    dqdqs_perbit_dly[jj].last_dqsdly_pass = ii -1;
                    if ( (dqdqs_perbit_dly[jj].last_dqsdly_pass-dqdqs_perbit_dly[jj].first_dqsdly_pass) > (dqdqs_perbit_dly[jj].best_last_dqsdly_pass-dqdqs_perbit_dly[jj].best_first_dqsdly_pass))
                    {
               dqdqs_perbit_dly[jj].best_last_dqsdly_pass =  dqdqs_perbit_dly[jj].last_dqsdly_pass;
               dqdqs_perbit_dly[jj].best_first_dqsdly_pass = dqdqs_perbit_dly[jj].first_dqsdly_pass;
                    }   
                    // clear to find the next pass range if it has
                    dqdqs_perbit_dly[jj].first_dqsdly_pass = -1;
                    dqdqs_perbit_dly[jj].last_dqsdly_pass = -2;                                       
                }
            }

            if (u4fail_bit == 0)
            {
                mcSHOW_USER_MSG(("o"));
            }
            else
            {
                {
                    mcSHOW_USER_MSG(("x"));
                }
            }            
        }
        mcSHOW_USER_MSG(("\n"));
    }

    // 3
    mcSHOW_DBG_MSG4(("------------------------------------------------------\n")); 
    mcSHOW_DBG_MSG4(("Start calculate dq time and dqs time / \n"));
    mcSHOW_DBG_MSG4(("Find max DQS delay per byte / Adjust DQ delay to align DQS...\n"));
    mcSHOW_DBG_MSG4(("------------------------------------------------------\n")); 

    u2rx_window_sum = 0;
    //As per byte, check max DQS delay in 8-bit. Except for the bit of max DQS delay, delay DQ to fulfill setup time = hold time
    for (ii = 0; ii < (p->data_width/DQS_BIT_NUMBER); ii++)
    {
        ucbit_first = DQS_BIT_NUMBER*ii;
        ucbit_last = DQS_BIT_NUMBER*ii+DQS_BIT_NUMBER-1;
        ucmax_dqsdly_byte[ii] = 0;
        for (jj = ucbit_first; jj <= ucbit_last; jj++)
        {
            // hold time = DQS pass taps
            uchold_pass_number = dqdqs_perbit_dly[jj].best_last_dqsdly_pass - dqdqs_perbit_dly[jj].best_first_dqsdly_pass + 1;
            // setup time = DQ pass taps
            ucsetup_pass_number = dqdqs_perbit_dly[jj].best_last_dqdly_pass - dqdqs_perbit_dly[jj].best_first_dqdly_pass + 1;
            u2rx_window_sum += (uchold_pass_number+ucsetup_pass_number);

            if (uchold_pass_number > ucsetup_pass_number)
            {
                if (ucsetup_pass_number != 0)
                {
                    // like this:
                    // setup time(dq delay)     hold time(dqs delay)
                    // 15                  0 1                       15 tap
                    // xxxxxxxxxxxxxoooooooo|ooooooooooooooooooooxxxxx
                    dqdqs_perbit_dly[jj].best_dqdly = 0;
                    dqdqs_perbit_dly[jj].best_dqsdly = (uchold_pass_number - ucsetup_pass_number) / 2;

                    if (dqdqs_perbit_dly[jj].best_dqsdly > ucmax_dqsdly_byte[ii])
                    {
                        ucmax_dqsdly_byte[ii] = dqdqs_perbit_dly[jj].best_dqsdly;
                    }
                }
                else
                {
                    // like this:
                    // setup time(dq delay)     hold time(dqs delay)
                    // 15                  0 1                       15 tap
                    // xxxxxxxxxxxxxxxxxxxxx|xxxooooooooooxxxxxxxxxxxx
                    dqdqs_perbit_dly[jj].best_dqdly = 0;
                    dqdqs_perbit_dly[jj].best_dqsdly = (uchold_pass_number - ucsetup_pass_number) / 2 + dqdqs_perbit_dly[jj].best_first_dqsdly_pass;

                    if (dqdqs_perbit_dly[jj].best_dqsdly > ucmax_dqsdly_byte[ii])
                    {
                        ucmax_dqsdly_byte[ii] = dqdqs_perbit_dly[jj].best_dqsdly;
                    }
                }
            }
            else if (uchold_pass_number < ucsetup_pass_number)
            {
                if (uchold_pass_number != 0)
                {
                    // like this:
                    // setup time(dq delay)     hold time(dqs delay)
                    // 15                  0 1                       15 tap
                    // xxxoooooooooooooooooo|ooooooooxxxxxxxxxxxxxxxxx
                    dqdqs_perbit_dly[jj].best_dqsdly = 0;
                    dqdqs_perbit_dly[jj].best_dqdly = (ucsetup_pass_number - uchold_pass_number) / 2;                    
                }
                else
                {
                    // like this:
                    // setup time(dq delay)     hold time(dqs delay)
                    // 15                  0 1                       15 tap
                    // xxxoooooooooooooooxxx|xxxxxxxxxxxxxxxxxxxxxxxxx
                    dqdqs_perbit_dly[jj].best_dqsdly = 0;
                    dqdqs_perbit_dly[jj].best_dqdly = (ucsetup_pass_number - uchold_pass_number) / 2 + dqdqs_perbit_dly[jj].best_first_dqdly_pass;                    
                }
            }
            else   // hold time == setup time
            {
                if (uchold_pass_number != 0)
                {
                    // like this:
                    // setup time(dq delay)     hold time(dqs delay)
                    // 15                  0 1                       15 tap
                    // xxxxxxxxxxxxxxxoooooo|ooooooxxxxxxxxxxxxxxxxxxx
                    dqdqs_perbit_dly[jj].best_dqsdly = 0;
                    dqdqs_perbit_dly[jj].best_dqdly = 0;    
                }
                else
                {
                    // like this:
                    // setup time(dq delay)     hold time(dqs delay)
                    // 15                  0 1                       15 tap
                    // xxxxxxxxxxxxxxxxxxxxx|xxxxxxxxxxxxxxxxxxxxxxxxx
                    // mean this bit is error
                    mcSHOW_DBG_MSG4(("error on bit %d ,setup_time =hold_time =0!!\n", jj));
                    dqdqs_perbit_dly[jj].best_dqsdly = 0;
                    dqdqs_perbit_dly[jj].best_dqdly = 0;
                    fgfail = 1;
                #ifdef DDR_FT_LOAD_BOARD
                    if (RXPERBIT_LOG_PRINT == 1) // ignore duty DramcClkDutyCal()
                    {
                        LoadBoardShowResult(FLAG_RX_CALIBRATION, FLAG_CALIBRATION_FAIL, p->channel, FLAG_NOT_COMPLETE_OR_FAIL);            
                        while(1); 
                    }
                #endif
                }
            }            
            mcSHOW_DBG_MSG4(("bit#%d : dq time=%d dqs time=%d win=%d center=(%d, %d)\n", jj, ucsetup_pass_number, uchold_pass_number, ucsetup_pass_number+uchold_pass_number, dqdqs_perbit_dly[jj].best_dqdly, dqdqs_perbit_dly[jj].best_dqsdly));
#ifdef EYE_SCAN
      EyeScanWin[jj].ucsetup_pass_number = ucsetup_pass_number;
      EyeScanWin[jj].uchold_pass_number = uchold_pass_number;
#endif

        #ifdef DDR_FT_LOAD_BOARD
            if (RXPERBIT_LOG_PRINT == 1) // ignore duty DramcClkDutyCal()
            {
                if ((ucsetup_pass_number+uchold_pass_number)<=RXWIN_CALIB_BOUND)
                {
                    LoadBoardShowResult(FLAG_RX_CALIBRATION, FLAG_WINDOW_TOO_SMALL, p->channel, FLAG_NOT_COMPLETE_OR_FAIL);            
                    while(1); 
                }
            }
        #endif
        }

        mcSHOW_DBG_MSG4(("----seperate line----\n"));

        // we delay DQ or DQS to let DQS sample the middle of tx pass window for all the 8 bits,
        for (jj = ucbit_first; jj <= ucbit_last; jj++)
        {
            // set DQS to max for 8-bit
            if (dqdqs_perbit_dly[jj].best_dqsdly < ucmax_dqsdly_byte[ii])
            {
                // delay DQ to compensate extra DQS delay
                dqdqs_perbit_dly[jj].best_dqdly = dqdqs_perbit_dly[jj].best_dqdly + (ucmax_dqsdly_byte[ii] - dqdqs_perbit_dly[jj].best_dqsdly);
                // max limit to 15
                dqdqs_perbit_dly[jj].best_dqdly = ((dqdqs_perbit_dly[jj].best_dqdly > (MAX_RX_DQDLY_TAPS-1)) ? (MAX_RX_DQDLY_TAPS-1) : dqdqs_perbit_dly[jj].best_dqdly);
            }            
        }
    }
       
    mcSHOW_DBG_MSG4(("==================================================\n"));
    mcSHOW_DBG_MSG4(("    dramc_rxdqs_perbit_swcal\n"));
    mcSHOW_DBG_MSG4(("    channel=%d(0:cha, 1:chb) \n", p->channel));
    mcSHOW_DBG_MSG4(("    bus width=%d\n", p->data_width));
    mcSHOW_DBG_MSG4(("==================================================\n"));
    mcSHOW_DBG_MSG4(("DQS Delay :\n DQS0 = %d DQS1 = %d DQS2 = %d DQS3 = %d\n", ucmax_dqsdly_byte[0], ucmax_dqsdly_byte[1], ucmax_dqsdly_byte[2], ucmax_dqsdly_byte[3]));
    mcSHOW_DBG_MSG4(("DQ Delay :\n"));

   
    for (ii = 0; ii < p->data_width; ii=ii+4)
    {
        mcSHOW_DBG_MSG4(("DQ%d = %d DQ%d = %d DQ%d = %d DQ%d = %d \n", ii, dqdqs_perbit_dly[ii].best_dqdly, ii+1, dqdqs_perbit_dly[ii+1].best_dqdly, ii+2, dqdqs_perbit_dly[ii+2].best_dqdly, ii+3, dqdqs_perbit_dly[ii+3].best_dqdly));
    }
    mcSHOW_DBG_MSG4(("________________________________________________________________________\n"));

    
    // set dqs delay
    u4value = 0;
    for (jj = 0; jj < (p->data_width/DQS_BIT_NUMBER); jj++)
    {
        u4value += (((U32)ucmax_dqsdly_byte[jj])<<(8*jj));        
    }
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DELDLY1), u4value);    
#ifndef ENABLE_DQSI_PER_RANK    
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1c), u4value);    // Edward : Set rank 1 also.
#endif    

    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DELDLY1), &u4value);
    mcSHOW_DBG_MSG4(("Reg.%xh=%xh\n", DRAMC_REG_DELDLY1, u4value));
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1c), &u4value);
    mcSHOW_DBG_MSG4(("Reg.%xh=%xh\n", 0x01c, u4value));    
    
    // set dq delay
    for (jj=0; jj<p->data_width; jj=jj+4)
    {
        // 20130528, no need to swap, since DQ_DLY & CMP_ERR has swapped by HW
        #if 0
        if (p->en_4bitMux == ENABLE)
        {
            kk = ucswap_table[1][jj];
        }
        else
        {
            kk = ucswap_table[0][jj];
        }
        #else
            kk = jj;            
        #endif
        //every 4bit dq have the same delay register address
        u4value = ((U32) dqdqs_perbit_dly[kk].best_dqdly) + (((U32)dqdqs_perbit_dly[kk+1].best_dqdly)<<8) + (((U32)dqdqs_perbit_dly[kk+2].best_dqdly)<<16) + (((U32)dqdqs_perbit_dly[kk+3].best_dqdly)<<24);  
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQIDLY1+jj), u4value);            
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQIDLY1+jj), &u4value);
    mcSHOW_DBG_MSG4(("Reg.%xh=%xh\n", DRAMC_REG_DQIDLY1+jj, u4value));
    }              
    
    DramcPhyReset(p);   // Wrong duty may cause GW wrong. Reset here.

    // Log example
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
    
    if (fgfail == 1)
    {
        mcSHOW_DBG_MSG4(("RX DQ/DQS calibration fail!\n"));
        return DRAM_FAIL;
    }
    
    if (ucstatus)
    {
        mcSHOW_ERR_MSG(("register access fail!\n"));
        return DRAM_FAIL;
    }
    else
    {
        return DRAM_OK;
    }
}

#ifdef TDP_TDN_CAL
void  DramcClkTDNTDPCal(DRAMC_CTX_T *p)
{
    U16 max_win_size = 0;
    U8 ii, jj, max_TDP, max_TDN, ucstatus = 0;
    U32 u4value;
    
    for (ii = 0; ii < 4; ii++)    // TDP
    {        

        for (jj = 0; jj < 4; jj++)  // TDN
        {
            DramcEnterSelfRefresh(p, 1);    
            ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xd4), &u4value);
            if (ii & 0x01)
            {
                mcSET_BIT(u4value, 0);
            }
            else
            {
                mcCLR_BIT(u4value, 0);
            }            
            if (jj & 0x01)
            {
                mcSET_BIT(u4value, 1);
            }            
            else
            {
                mcCLR_BIT(u4value, 1);
            }
            ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xd4), u4value);

            ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x640), &u4value);
            if (ii & 0x02)  // TDP
            {
              u4value |= (0x1 << 20);
            }
            else
            {
              u4value &= (~(0x1 << 20));
            }            
            if (jj & 0x02)
            {
              u4value |= (0x1 << 21);
            }
            else
            {
              u4value &= (~(0x1 << 21));
            }            
            ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x640), u4value);
            
            DramcEnterSelfRefresh(p, 0);    

            RXPERBIT_LOG_PRINT = 0;
            DramcRxWindowPerbitCal(p);
            RXPERBIT_LOG_PRINT = 1;

            if (u2rx_window_sum > max_win_size)
            {
                max_win_size = u2rx_window_sum;
                max_TDP = ii;
                max_TDN = jj;
            }

            mcSHOW_DBG_MSG(("[Channel %d CLK DUTY CALIB] TDP=%d, TDN=%d,  --> rx window size=%d\n", p->channel, ii, jj, u2rx_window_sum));
            ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xd4), &u4value);
            mcSHOW_DBG_MSG(("[CLK DUTY CALIB] Reg.d4h=%xh \n", u4value));
            ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x640), &u4value);
            mcSHOW_DBG_MSG(("[CLK DUTY CALIB] Reg.640h=%xh \n", u4value));            
        }
    }

    // set optimal CLK duty settings
    DramcEnterSelfRefresh(p, 1);    
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xd4), &u4value);

    if (max_TDP & 0x01)
    {
        mcSET_BIT(u4value, 0);
    }
    else
    {
        mcCLR_BIT(u4value, 0);
    }            
    if (max_TDN & 0x01)
    {
        mcSET_BIT(u4value, 1);
    }            
    else
    {
        mcCLR_BIT(u4value, 1);
    }
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xd4), u4value);

    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x640), &u4value);
    if (max_TDP & 0x02) // TDP
    {
      u4value |= (0x1 << 20);
    }
    else
    {
      u4value &= (~(0x1 << 20));
    }            
    if (max_TDN & 0x02)
    {
      u4value |= (0x1 << 21);
    }
    else
    {
      u4value &= (~(0x1 << 21));
    }            
    
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x640), u4value);
    DramcEnterSelfRefresh(p, 0);    

    mcSHOW_DBG_MSG(("[Channel %d CLK DUTY Final] TDP=%d, TDN=%d  --> rx window size=%d\n", p->channel, max_TDP, max_TDN, max_win_size));
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xd4), &u4value);
    mcSHOW_DBG_MSG(("[CLK DUTY CALIB] Reg.d4h=%xh \n", u4value));
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x640), &u4value);
    mcSHOW_DBG_MSG(("[CLK DUTY CALIB] Reg.640h=%xh \n", u4value));     
}

void  DramcDQSTDNTDPCal(DRAMC_CTX_T *p)
{
    U16 max_win_size = 0;
    U8 ii, jj, max_TDP, max_TDN, ucstatus = 0;
    U32 u4value;
    
    for (ii = 0; ii < 4; ii++)    // TDP
    {        

        for (jj = 0; jj < 4; jj++)  // TDN
        {
            DramcEnterSelfRefresh(p, 1);    
            ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xd4), &u4value);
            if (ii & 0x01)    // TDP
            {
              u4value |= 0x55550000;
            }
            else
            {
              u4value &= (~0x55550000);
            }            
            if (jj & 0x01)  // TDN
            {
              u4value |= 0xaaaa0000;
            }
            else
            {
              u4value &= (~0xaaaa0000);
            }            
            ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xd4), u4value);

            ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x63c), &u4value);
            if (ii & 0x02)  // TDP
            {
              u4value |= 0x00005500;
            }
            else
            {
              u4value &= (~0x00005500);
            }            
            if (jj & 0x02)
            {
              u4value |= 0x0000aa00;
            }
            else
            {
              u4value &= (~0x0000aa00);
            }            
            ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x63c), u4value);
            
            DramcEnterSelfRefresh(p, 0);    

            RXPERBIT_LOG_PRINT = 0;
            DramcTxWindowPerbitCal(p);
            RXPERBIT_LOG_PRINT = 1;

            if (u2tx_window_sum > max_win_size)
            {
                max_win_size = u2tx_window_sum;
                max_TDP = ii;
                max_TDN = jj;
            }

            mcSHOW_DBG_MSG(("[Channel %d DQ/DQS DUTY CALIB] TDP=%d, TDN=%d --> tx window size=%d\n", p->channel, ii, jj, u2tx_window_sum));
            ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xd4), &u4value);
            mcSHOW_DBG_MSG(("[DQ/DQS DUTY CALIB] Reg.d4h=%xh \n", u4value));
            ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x63c), &u4value);
            mcSHOW_DBG_MSG(("[DQ/DQS DUTY CALIB] Reg.63ch=%xh \n", u4value));
        }
    }

    // set optimal CLK duty settings
    DramcEnterSelfRefresh(p, 1);    
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xd4), &u4value);
    if (max_TDP & 0x01)     // TDP
    {
      u4value |= 0x55550000;
    }
    else
    {
      u4value &= (~0x55550000);
    }            
    if (max_TDN & 0x01) // TDN
    {
      u4value |= 0xaaaa0000;
    }
    else
    {
      u4value &= (~0xaaaa0000);
    }            
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xd4), u4value);

    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x63c), &u4value);
    if (max_TDP & 0x02) // TDP
    {
      u4value |= 0x00005500;
    }
    else
    {
      u4value &= (~0x00005500);
    }            
    if (max_TDN & 0x02)
    {
      u4value |= 0x0000aa00;
    }
    else
    {
      u4value &= (~0x0000aa00);
    }            
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x63c), u4value);
    DramcEnterSelfRefresh(p, 0);    

    mcSHOW_DBG_MSG(("[Channel %d DQ/DQS DUTY Final] TDP=%d, TDN=%d --> tx window size=%d\n", p->channel, max_TDP, max_TDN, max_win_size));
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xd4), &u4value);
    mcSHOW_DBG_MSG(("[DQ/DQS DUTY Final] Reg.d4h=%xh \n", u4value));
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x63c), &u4value);
    mcSHOW_DBG_MSG(("[DQ/DQS DUTY Final] Reg.63ch=%xh \n", u4value));
}

#endif

void DramcClkDutyCal(DRAMC_CTX_T *p)
{
    U16 max_win_size = 0;
    U8 ii, jj, max_duty_sel=0, max_duty=0, ucstatus = 0;
    U32 u4value;
    
    for (ii = 0; ii < 2; ii++)
    {        
        for (jj = 0; jj < 4; jj++)
        {
            DramcEnterSelfRefresh(p, 1);    
            ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCLKDUTY), &u4value);
            if (ii == 0)
            {
                mcCLR_BIT(u4value, POS_PHYCLKDUTY_CMDCLKP0DUTYSEL);
                mcSET_FIELD(u4value, 0, MASK_PHYCLKDUTY_CMDCLKP0DUTYN, POS_PHYCLKDUTY_CMDCLKP0DUTYN);
                mcSET_FIELD(u4value, jj, MASK_PHYCLKDUTY_CMDCLKP0DUTYP, POS_PHYCLKDUTY_CMDCLKP0DUTYP);
            }
            else
            {
                mcSET_BIT(u4value, POS_PHYCLKDUTY_CMDCLKP0DUTYSEL);
                mcSET_FIELD(u4value, jj, MASK_PHYCLKDUTY_CMDCLKP0DUTYN, POS_PHYCLKDUTY_CMDCLKP0DUTYN);
                mcSET_FIELD(u4value, 0, MASK_PHYCLKDUTY_CMDCLKP0DUTYP, POS_PHYCLKDUTY_CMDCLKP0DUTYP);
            }            
            ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCLKDUTY), u4value);
#if 1
        mcDELAY_US(20); // Wait PLL lock.
#else
        *(volatile unsigned int*)(0x10006000) = 0x0b160001; 
        *(volatile unsigned int*)(0x10006010) |= 0x08000000;  //(4) 0x10006010[27]=1  //Request MEMPLL reset/pdn mode 
        mcDELAY_US(2);
        *(volatile unsigned int*)(0x10006010) &= ~(0x08000000);  //(1) 0x10006010[27]=0 //Unrequest MEMPLL reset/pdn mode and wait settle (1us for reset)
        mcDELAY_US(13);
        //mcSHOW_DBG_MSG(("Mempll reset\n"));
#endif      
            
            DramcEnterSelfRefresh(p, 0);    

            RXPERBIT_LOG_PRINT = 0;
            DramcRxWindowPerbitCal(p);
            RXPERBIT_LOG_PRINT = 1;

            if (u2rx_window_sum > max_win_size)
            {
                max_win_size = u2rx_window_sum;
                max_duty_sel = ii;
                max_duty = jj;
            }

            mcSHOW_DBG_MSG(("[Channel %d CLK DUTY CALIB] DUTY_SEL=%d, DUTY=%d --> rx window size=%d\n", p->channel, ii, jj, u2rx_window_sum));
        }
    }

    // set optimal CLK duty settings
    DramcEnterSelfRefresh(p, 1);    
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCLKDUTY), &u4value);
    if (max_duty_sel == 0)
    {
        mcCLR_BIT(u4value, POS_PHYCLKDUTY_CMDCLKP0DUTYSEL);
        mcSET_FIELD(u4value, 0, MASK_PHYCLKDUTY_CMDCLKP0DUTYN, POS_PHYCLKDUTY_CMDCLKP0DUTYN);
        mcSET_FIELD(u4value, max_duty, MASK_PHYCLKDUTY_CMDCLKP0DUTYP, POS_PHYCLKDUTY_CMDCLKP0DUTYP);
    }
    else
    {
        mcSET_BIT(u4value, POS_PHYCLKDUTY_CMDCLKP0DUTYSEL);
        mcSET_FIELD(u4value, max_duty, MASK_PHYCLKDUTY_CMDCLKP0DUTYN, POS_PHYCLKDUTY_CMDCLKP0DUTYN);
        mcSET_FIELD(u4value, 0, MASK_PHYCLKDUTY_CMDCLKP0DUTYP, POS_PHYCLKDUTY_CMDCLKP0DUTYP);
    }
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCLKDUTY), u4value);
#if 1
    mcDELAY_US(20); // Wait PLL lock.
#else
    *(volatile unsigned int*)(0x10006000) = 0x0b160001; 
    *(volatile unsigned int*)(0x10006010) |= 0x08000000;  //(4) 0x10006010[27]=1  //Request MEMPLL reset/pdn mode 
    mcDELAY_US(2);
    *(volatile unsigned int*)(0x10006010) &= ~(0x08000000);  //(1) 0x10006010[27]=0 //Unrequest MEMPLL reset/pdn mode and wait settle (1us for reset)
    mcDELAY_US(13);
    //mcSHOW_DBG_MSG(("Mempll reset\n"));
#endif   

    DramcEnterSelfRefresh(p, 0);    



    mcSHOW_DBG_MSG(("[Channel %d CLK DUTY CALIB] Final DUTY_SEL=%d, DUTY=%d --> rx window size=%d, Reg.148h=%xh\n", p->channel, max_duty_sel, max_duty, max_win_size, u4value));
}

#if 1 //add by KT, define the function in ett_cust.dle.c
void dle_factor_handler(DRAMC_CTX_T *p, U8 curr_val) 
{
    U8 ucstatus = 0;
    U32 u4value;
    U32 u4curr_val_DSEL;

    /* DATLAT: Reg.80h[20:16], 3 bits */
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x80), &u4value);
    u4value = (u4value & 0xffe0ffff) | ((U32)curr_val << 16);
    //mcSET_FIELD(u4value, curr_val, 0x001f0000, 16);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x80), u4value);

    // DATLAT_DSEL (Reg.80h[12:8]) = DATLAT - 2*3 - 2*1 (Speed >=1600)
    // DATLAT_DSEL = DATLAT - 2*1 - 2*1 (Speed <1600)
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x80), &u4value);
    if ((u4value & 0xe0)==0xe0)
    {
      // 3 pipes
      if (curr_val>=DLE_DLEDSEL_DIFF)
      {
        u4curr_val_DSEL = curr_val -DLE_DLEDSEL_DIFF;
      }
      else
      {
        u4curr_val_DSEL = 0;
      }     
    }
    else
    {
      // should be 1 pipe.
      if (curr_val>=(DLE_DLEDSEL_DIFF-4))
      {
        u4curr_val_DSEL = curr_val -(DLE_DLEDSEL_DIFF-4);
      }
      else
      {
        u4curr_val_DSEL = 0;
      }     
    }

    /*
    if (p->frequency < 1600)
    {
      if (curr_val>=4)
      {
        u4curr_val_DSEL = curr_val -4;
      }
      else
      {
        u4curr_val_DSEL = 0;
      }
    }
    else
    {
      if (curr_val>=8)
      {
        u4curr_val_DSEL = curr_val -8;
      }
      else
      {
        u4curr_val_DSEL = 0;
      }
    }
    */

    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x80), &u4value);
    mcSET_FIELD(u4value, u4curr_val_DSEL, 0x00001f00, 8);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x80), u4value); 
       

    // only for HW run time test engine use, optimize bandwidth

    if (curr_val >= 3)
    {
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x80), &u4value);
        mcSET_FIELD(u4value, (curr_val-3)&0x1f, 0x0000001f, 0);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x80), u4value);    
    }
    else
    {
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x80), &u4value);
        mcSET_FIELD(u4value, 0, 0x0000001f, 0);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x80), u4value);
    }

}

// The difference of tDQSCK (2.5~5.5ns) is 3ns which is larger than 1 M_CK (2.14ns in 1866Mbps).
// Should set to the maximum of dual rank. 
// If calibration could detect the end of window, set the center of overlapped window.
DRAM_STATUS_T DramcDualRankRxdatlatCal(DRAMC_CTX_T *p)
{
  U8 ucstatus = 0, ucR0DLESetting;
  U32 u4value, err;
  U32 DQSINCTL;

#ifdef ADJUST_CROSS_RANK_TIMING
  // Get Reg.e0h[27:24] DQSINCTL after rank 0 calibration.
  ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xe0), &DQSINCTL);  
  DQSINCTL = (DQSINCTL >> 24) & 0xf;
#endif

  // Rank 0 DLE calibration.
  CurrentRank = 0;
  if (DramcRxdatlatCal((DRAMC_CTX_T *) p, fcDATLAT_USE_TXRX_DEFAULT))
  {
      if (DramcRxdatlatCal((DRAMC_CTX_T *) p, fcDATLAT_USE_RX_SCAN))
      {
          DramcRxdatlatCal((DRAMC_CTX_T *) p, fcDATLAT_USE_TXRX_SCAN);
      }
  }

  ucR0DLESetting = ucDLESetting;

  // Rank 1 DLE calibration.
  // Swap CS0 and CS1.
  ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x110), &u4value);
  u4value = u4value |0x08;
  ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x110), u4value);

#ifdef ADJUST_CROSS_RANK_TIMING
  // Set rank 1 coarse tune and fine tune back.
  if (opt_gw_coarse_value_R0[p->channel] <= opt_gw_coarse_value_R1[p->channel])
  {
      dqsi_gw_dly_coarse_factor_handler(p, opt_gw_coarse_value_R1[p->channel], DQSINCTL);
      dqsi_gw_dly_fine_factor_handler(p, opt_gw_fine_value_R1[p->channel]);   
      DQSINCTL = 0xff;
  }
  else
  {
    DQSINCTL = opt_gw_coarse_value_R1[p->channel] >> 2;
  //No need to set rank 1 coarse tune because only one set registers for coarse tune in 808
        dqsi_gw_dly_coarse_factor_handler(p, opt_gw_coarse_value_R1[p->channel], DQSINCTL);
        dqsi_gw_dly_fine_factor_handler(p, opt_gw_fine_value_R1[p->channel]);   
  }
#else
    dqsi_gw_dly_coarse_factor_handler(p, opt_gw_coarse_value_R1[p->channel], 0xff);
        dqsi_gw_dly_fine_factor_handler(p, opt_gw_fine_value_R1[p->channel]);        
#endif 

  
  CurrentRank = 1;
  //DramcPhyReset(p);

  if (DramcRxdatlatCal((DRAMC_CTX_T *) p, fcDATLAT_USE_TXRX_DEFAULT))
  {
      if (DramcRxdatlatCal((DRAMC_CTX_T *) p, fcDATLAT_USE_RX_SCAN))
      {
          DramcRxdatlatCal((DRAMC_CTX_T *) p, fcDATLAT_USE_TXRX_SCAN);
      }
  }

  mcSHOW_DBG_MSG(("Rank 0 DLE calibrated setting = %xh. Rank 1 DLE calibrated setting = %xh\n", ucR0DLESetting, ucDLESetting));
  if (ucDLESetting < ucR0DLESetting)
  {
    mcSHOW_DBG_MSG(("Rank 0 %xh > Rank 1 %xh. Set to rank 0 %xh.\n", ucR0DLESetting, ucDLESetting, ucR0DLESetting));
    dle_factor_handler(p, ucR0DLESetting);
  }
  else
  {
    mcSHOW_DBG_MSG(("Rank 0 %xh < Rank 1 %xh. Use rank 1 %xh.\n", ucR0DLESetting, ucDLESetting, ucDLESetting));
  }
  
  // Swap CS back.
  ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x110), &u4value);
  u4value = u4value & (~0x08);
  ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x110), u4value);


#ifdef ADJUST_CROSS_RANK_TIMING
    // Set rank 0 coarse tune and fine tune back.
    dqsi_gw_dly_coarse_factor_handler(p, opt_gw_coarse_value_R0[p->channel], DQSINCTL);
    dqsi_gw_dly_fine_factor_handler(p, opt_gw_fine_value_R0[p->channel]);  
#else
  // Set rank 0 coarse tune and fine tune back.
  dqsi_gw_dly_coarse_factor_handler(p, opt_gw_coarse_value_R0[p->channel], 0xff);
  dqsi_gw_dly_fine_factor_handler(p, opt_gw_fine_value_R0[p->channel]);     
#endif  

  CurrentRank = 0;
  //DramcPhyReset(p);
   return DRAM_OK;
}
      
unsigned int ta2_enable_write_read(DRAMC_CTX_T *p)
{
	U32 u4err_value = 0;

#if 1
        if (p->test_pattern== TEST_AUDIO_PATTERN)
        {
            // enable TE2, audio pattern
            if (p->fglow_freq_write_en == ENABLE)
            {
                u4err_value = DramcEngine2(p, TE_OP_READ_CHECK, p->test2_1, p->test2_2, 1, 0, 0, 0);
            }
            else
            {
                u4err_value = DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 1, 0, 0, 0);
            }            
        }
        else if (p->test_pattern == TEST_ISI_PATTERN)
        {
            // enable TE2, ISI pattern
            u4err_value |= DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 0, 0, 0, 0);                      
        }
        else if (p->test_pattern == TEST_XTALK_PATTERN)
        {
            if (p->fglow_freq_write_en == ENABLE)
            {
                u4err_value = DramcEngine2(p, TE_OP_READ_CHECK, p->test2_1, p->test2_2, 2, 0, 0, 0);
            }
            else
            {
                u4err_value = DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 2, 0, 0, 0);
            }            
        }
        else if (p->test_pattern == TEST_MIX_PATTERN)
        {
            u4err_value = DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 1, 0, 0, 0);
            u4err_value |= DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 2, 0, 0, 0);
        }
        else if (p->test_pattern == TEST_TA1_SIMPLE)
        {
            u4err_value = DramcEngine1(p, 0x55000000, 0xaa0007ff, 0, 0);
        }
        else
        {
            mcSHOW_ERR_MSG(("Not support test pattern!! Use audio pattern by default.\n"));
            u4err_value = DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 1, 0, 0, 0);
        }
#else	 // fix TA1 to do DATLAT
        u4err_value = DramcEngine1(p, p->test2_1, p->test2_2, 0, 0);
#endif

		return u4err_value;
}

unsigned int ta2_rx_scan(DRAMC_CTX_T *p)
{
//#define dbg_print         
	unsigned int i;    
	unsigned int dqs_input_delay;    
	unsigned int cmp_err;        
	unsigned int u4prv_register_018, u4prv_register_210[8]; 
	U8 ucstatus = 0;
	U32 u4value;
	
	// backup registers    
	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x18), &u4value);
	u4prv_register_018 = u4value;    
	for (i = 0; i < 8; i++)    
	{        
		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x210+4*i), &u4value);
		u4prv_register_210[i] = u4value;    
	}    

	// set DQ delay to 0x0.    
	for (i = 0; i < 8; i++)    
	{        
	    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x210+4*i), 0x0);    
	}              

	/* find the minimum and maximum DQS input delay*/    
	for (i = 0; i < 32; i=i+2)    
	{        
		dqs_input_delay = (i) + (i << 8) + (i << 16) + (i << 24);        
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x18), dqs_input_delay);              
		
		cmp_err = ta2_enable_write_read(p);
		
		if (cmp_err == 0)        
		{            
			break;        
		}           
	}    

	mcSHOW_DBG_MSG(("RX Scan: DQS delay %d \n", i));    

	// restore registers    
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x18), u4prv_register_018);
	for (i = 0; i < 8; i++)    
	{        
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x210+4*i), u4prv_register_210[i]);        
	}    

	return cmp_err;    
}

unsigned int ta2_txrx_scan(DRAMC_CTX_T *p)
{
//#define dbg_print         
	unsigned int i;    
	unsigned int dqs_input_delay;    
	unsigned int cmp_err;        
    unsigned int u4prv_register_010, u4prv_register_014, u4prv_register_200[4];
    unsigned int tx_scan_done = 0;    
	U8 ucstatus = 0;
	U32 u4value;

    // backup registers
    // DQM
	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x10), &u4value);
	u4prv_register_014= u4value;    
    //DQ
    for (i = 0 ; i < 4; i ++)
    {
		ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x200+4*i), &u4value);
		u4prv_register_200[i] = u4value;       
    }
    //DQS
	ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x14), &u4value);
	u4prv_register_014 = u4value;    
  
    // TX DQ/DQM set to 0
    /*DQM*/
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x10), 0x0);      
    /*TX DQ*/
    for (i = 0 ; i < 4; i ++)
    {
	    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x200+4*i), 0x0);            
    }

    // scan DQS
    for (i = 0; i < MAX_TX_DQSDLY_TAPS ; i = i+2)
    {
        ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x14), &u4value);
        u4value = (u4value & 0xFFFF0000) | ((i <<0) | (i<<4) | (i <<8) | (i <<12));
        //dbg_print("mask:%x, data:%x\n",mask,data);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x14), u4value);

        cmp_err = ta2_rx_scan(p);

        if (cmp_err == 0)
        {
            tx_scan_done = 1;
            break;
        }
    }

    mcSHOW_DBG_MSG(("TX Scan: DQ delay 0, DQS delay %d \n", i));

    if (tx_scan_done == 0)
    {
        /* fix DQS delay = 0, delay DQ to find the pass range  */
        ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x14), &u4value);
        u4value = u4value & 0xFFFF0000;
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x14), u4value);


        for (i = 1; i < MAX_TX_DQDLY_TAPS; i = i+2)
        {

            /* delay DQ output delay */
            u4value = 
                ((i & 0xF) << 0)       /* DQM0DLY: DRAMC_PADCTL2[0:3],         4 bits */
              | ((i & 0xF) << 4)       /* DQM1DLY: DRAMC_PADCTL2[4:7], 4 bits */
              | ((i & 0xF) << 8)       /* DQM2DLY: DRAMC_PADCTL2[8:11],        4 bits */
              | ((i & 0xF) << 12);
            ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x10), u4value);

            u4value =    
                ((i & 0xF) << 0)             /* DQ0DLY: DRAMC_DQODLY1[0:3],          4 bits */
              | ((i & 0xF) << 4)               /* DQ1DLY: DRAMC_DQODLY1[4:7],          4 bits */
              | ((i & 0xF) << 8)               /* DQ2DLY: DRAMC_DQODLY1[8:11],         4 bits */
              | ((i & 0xF) << 12)      /* DQ3DLY: DRAMC_DQODLY1[12:15],        4 bits */
              | ((i & 0xF) << 16)      /* DQ4DLY: DRAMC_DQODLY1[16:19],        4 bits */
              | ((i & 0xF) << 20)      /* DQ5DLY: DRAMC_DQODLY1[20:23],        4 bits */
              | ((i & 0xF) << 24)      /* DQ6DLY: DRAMC_DQODLY1[24:27],        4 bits */
              | ((i & 0xF) << 28); 
            ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x200), u4value);
            ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x204), u4value);
            ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x208), u4value);
            ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x20c), u4value);

            cmp_err = ta2_rx_scan(p);

            if (cmp_err == 0)
            {
                tx_scan_done = 1;
                break;
            }
        }

        mcSHOW_DBG_MSG(("TX Scan: DQS delay 0, DQ delay %d \n", i));        
    }

#if 0
	// restore registers    
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x10), u4prv_register_010);
	for (i = 0; i < 4; i++)    
	{        
		ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x200+4*i), u4prv_register_200[i]);        
	}    
	ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x14), u4prv_register_014);    
#endif    

	return cmp_err;    
}
      
//-------------------------------------------------------------------------
/** DramcRxdatlatCal
 *  scan the pass range of DATLAT for DDRPHY read data window.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL 
 */
//-------------------------------------------------------------------------
DRAM_STATUS_T DramcRxdatlatCal(DRAMC_CTX_T *p, unsigned int use_rxtx_scan)
{
//A60807
    U8 ucstatus = 0, ii, ucStartCalVal=0;
    U32 u4prv_register_07c, u4prv_register_0e4;
    U32 u4prv_register_0f0, u4prv_register_080, u4prv_register_0d8;

    U32 u4value, u4err_value;
    U8 ucfirst, ucbegin, ucsum, ucbest_step;    
    U32 result, start_tap, end_tap;

    // error handling
    if (!p)
    {
        mcSHOW_ERR_MSG(("context is NULL\n"));
        return DRAM_FAIL;
    }
#ifndef RELEASE        
    mcSHOW_DBG_MSG(("\n==============================================================\n"));
    if (use_rxtx_scan == fcDATLAT_USE_RX_SCAN)
    {
        mcSHOW_DBG_MSG(("    NEW DATLAT calibration with RX scan \n"));
    }
    else if (use_rxtx_scan == fcDATLAT_USE_TXRX_SCAN)
	{        
        mcSHOW_DBG_MSG(("    NEW DATLAT calibration with TX/RX scan \n"));
	}    
	else    
	{        
		mcSHOW_DBG_MSG(("    DATLAT calibration \n"));    
	}

    mcSHOW_DBG_MSG(("    [Channel %d] [Rank %d] DATLAT calibration \n", p->channel, CurrentRank));
    mcSHOW_DBG_MSG(("    channel=%d(0:cha) \n", p->channel));
    mcSHOW_DBG_MSG(("==============================================================\n"));
#else
    mcSHOW_USER_MSG(("\nR%d LAT\n", CurrentRank));
#endif

#if 0
    if (use_default_rx == 0)    
	{        
		start_tap = 3;        
		end_tap = 12;    
	}    
	else    
	{        
		start_tap = 0;        
		end_tap = 15;    
	}
#endif	

    // [11:10] DQIENQKEND 01 -> 00 for DATLAT calibration issue, DQS input enable will refer to DATLAT
    // if need to enable this (for power saving), do it after all calibration done
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MCKDLY), &u4value);
    u4prv_register_0d8 = u4value;
    mcSET_FIELD(u4value, 0x00, MASK_MCKDLY_DQIENQKEND, POS_MCKDLY_DQIENQKEND);
    mcCLR_BIT(u4value, 4);
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MCKDLY), u4value);    


    /* K2 : DATLAT: Reg.80h[20:16], 5 bits */
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x80), &u4prv_register_080);

    // init best_step to default
    //ucbest_step = (U8) (((u4prv_register_07c>>4)&0x7) | ((u4prv_register_0e4>>1)&0x8) | ((u4prv_register_0f0>>21)&0x10));
    ucbest_step = (U8) ((u4prv_register_080 >> 16) & 0x1f);

    mcSHOW_DBG_MSG(("DATLAT Default value = 0x%x\n", ucbest_step));

    // 1.set DATLAT 0-15 (0-21 for MT6595)
    // 2.enable engine1 or engine2 
    // 3.check result  ,3~4 taps pass 
    // 4.set DATLAT 2nd value for optimal

    // Initialize
    ucfirst = 0xff;
    ucbegin = 0;
    ucsum = 0;        

    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x80), &u4value);

    if ((u4value & 0xe0)==0xe0)
    {
      // 3 pipes
      ucStartCalVal = DLE_DLEDSEL_DIFF;
    }
    else
    {
      // Should be 1 pipe.
      ucStartCalVal = (DLE_DLEDSEL_DIFF-4);
    }
/*    
   if (p->frequency < 1600)
   {
    ucStartCalVal = 4;
   }
   else
   {
    ucStartCalVal = 8;
   }
*/   
        
    for (ii = ucStartCalVal; ii < DATLAT_TAP_NUMBER; ii++)
    // for testing
    //for (ii = 8; ii < DATLAT_TAP_NUMBER; ii++)
    {        
        // 1
        dle_factor_handler(p, ii);

        // 2
        if (use_rxtx_scan == fcDATLAT_USE_RX_SCAN)
        {
            u4err_value = ta2_rx_scan(p);
        }
        else if (use_rxtx_scan == fcDATLAT_USE_TXRX_SCAN)
        {
            u4err_value = ta2_txrx_scan(p);
        }                      
        else
        {
           u4err_value = ta2_enable_write_read(p);    
        }  

        
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

  DramcPhyReset(p);   // Wrong duty may cause GW wrong. Reset here.
  
#ifndef RELEASE      
        mcSHOW_DBG_MSG(("TAP=%d, err_value=0x%x, begin=%d, first=%d, sum=%d\n", ii, u4err_value, ucbegin, ucfirst, ucsum));
#else
      mcSHOW_USER_MSG(("%d, 0x%x\n", ii, u4err_value));
#endif

		if ((ucsum>=2) && (use_rxtx_scan != fcDATLAT_USE_TXRX_DEFAULT))		
		{			 
			mcSHOW_USER_MSG(("New DATLAT with RX/TX scan. Early Break!!\n")); 		   
			break;		 
		}

    }

    // 4
    if (ucsum == 0)
    {
        mcSHOW_ERR_MSG(("no DATLAT taps pass, DATLAT calibration fail!!\n"));
		result = 1;
    } 
    else if (ucsum == 1)
    {
        ucbest_step = ucfirst;
        mcSHOW_ERR_MSG(("only one pass tap!!\n"));
		result = 1;
    }
    else
    {
        //if test engine2, DLE return not so density, once pass, always pass        
        //so we choose the second one as the best    
        ucbest_step = ucfirst + 2;
		result = 0;
    }

   #ifdef FIX_DLE_19
   ucbest_step = 19;
   #endif
    mcSHOW_DBG_MSG(("pattern=%d(0: ISI, 1: AUDIO, 2: TA4, 3: TA4-3) first_step=%d total pass=%d best_step=%d\n", p->test_pattern, ucfirst, ucsum, ucbest_step));
        
    if (ucsum == 0)
    {
        mcSHOW_ERR_MSG(("DATLAT calibration fail, write back to default values!\n"));

        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x80), u4prv_register_080);
    }
    else
    {
        dle_factor_handler(p, ucbest_step);
    }

    ucDLESetting = ucbest_step;
    // [11:10] DQIENQKEND 01 -> 00 for DATLAT calibration issue, DQS input enable will refer to DATLAT
    // if need to enable this (for power saving), do it after all calibration done    

    mcSET_FIELD(u4prv_register_0d8, 0x01, MASK_MCKDLY_DQIENQKEND, POS_MCKDLY_DQIENQKEND);
    mcSET_BIT(u4prv_register_0d8, 4);     // If init value is 0 and DE ask to set to 1, this line should be executed.
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MCKDLY), u4prv_register_0d8);

#ifdef DDR_FT_LOAD_BOARD
    if (ucsum == 0)
    {
        LoadBoardShowResult(FLAG_DATLAT_CALIBRATION, FLAG_CALIBRATION_FAIL, p->channel, FLAG_NOT_COMPLETE_OR_FAIL);            
        while(1);
    }
    else if (ucsum <= DATLAT_CALIB_BOUND)
    {
        LoadBoardShowResult(FLAG_DATLAT_CALIBRATION, FLAG_WINDOW_TOO_SMALL, p->channel, FLAG_NOT_COMPLETE_OR_FAIL);            
        while(1);
    }
#endif

#if 0
    if (ucstatus)
    {
        mcSHOW_ERR_MSG(("register access fail!\n"));
        return DRAM_FAIL;
    }
    else
    {
        return DRAM_OK;
    }
#else
	return result;
#endif
}
#endif

#ifdef DQM_SHIFT
S32 DQMSkew[DQS_NUMBER] = {-3, -2, -2, -2};
#endif

#ifdef DQM_CALIBRATION
DRAM_STATUS_T DramcDQMCal(DRAMC_CTX_T *p)
{
    U8 ucstatus = 0, uc_dly, jj, kk, CalStop = 0, ucDQ_B_Avg[DQS_NUMBER];
    S8 ii;
    U32 u4value, u4Wvalue, u4err_value, u4fail_bit;
    TXDQM_DLY_T dqm_dly[DQS_NUMBER];

    // error handling
    if (!p)
    {
        mcSHOW_ERR_MSG(("context is NULL\n"));
        return DRAM_FAIL;
    }

    // initialize parameters
    for (ii = 0; ii < (p->data_width/DQS_BIT_NUMBER); ii++)
    {
        dqm_dly[ii].first_dqmdly_pass = -16;
        dqm_dly[ii].last_dqmdly_pass = -17;
        dqm_dly[ii].best_first_dqmdly_pass = -16;
        dqm_dly[ii].best_last_dqmdly_pass = -17;
        ucDQ_B_Avg[ii] = 0;
    }
    
    DramcPhyReset(p);

    mcSHOW_DBG_MSG(("------------------------------------------------------\n")); 
    mcSHOW_DBG_MSG(("DQM calibration...\n"));
    mcSHOW_DBG_MSG(("------------------------------------------------------\n")); 
    mcSHOW_DBG_MSG(("x-axis is DQM ##; y-axis is DQM delay-DQDQS delay. \n"));    

    // delay DQ from first_dq_dly to 15 to get the setup time
    for (ii = -MAX_TX_DQDLY_TAPS+1 ; ii < MAX_TX_DQDLY_TAPS; ii++)
    {    
      if (ii <= 0)
      {
        // Adjust DQM
          uc_dly = (U8)(-ii);    
          u4value = 0;

          // set DQM delay        
          if (p->data_width == DATA_WIDTH_16BIT)
          {
              // for DQC, DQM3 is CS#
              ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL2), &u4value);
              mcSET_FIELD(u4value, 0, 0x000000ff, 0);
          }
          else
          {
          ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL2), &u4value);
              u4value &= 0xffff0000;
          }
          for (jj = 0; jj < (p->data_width/DQS_BIT_NUMBER); jj++)
          {
              u4value += (((U32) uc_dly) << (4*jj));
          }
          ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL2), u4value);
          mcSHOW_DBG_MSG(("DRAMC_REG_PADCTL2 = 0x%x\n", u4value));
      }
      else
      {
        // Adjust DQS/DQ

        
        // Adjust DQS.
    ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL3), &u4value);
      
          u4Wvalue = 0;
          for (jj = 0; jj < (p->data_width/DQS_BIT_NUMBER); jj++)
          {
      uc_dly = (u4value >> (4*jj)) & 0x0f;
      if (uc_dly < 0x0f) 
      {
        uc_dly++;
        u4Wvalue += (((U32)uc_dly)<<(4*jj));        
      }

      if (uc_dly == 0x0f)     
      {
        CalStop = 1;
      }
          }
          ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL3), u4Wvalue);
          mcSHOW_DBG_MSG(("DRAMC_REG_PADCTL3 = 0x%x\n", u4Wvalue));

          // Adjust DQ
          for (jj = 0; jj < (p->data_width/DQS_BIT_NUMBER); jj++)
          {
      ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQODLY1+4*jj), &u4value);
            u4Wvalue = 0;
      for (kk = 0; kk < 8; kk++)
      {
        uc_dly = (u4value >> (4*kk)) & 0x0f;
        if (ii==1)
        {
          // first time. Record the average calibration value.
          ucDQ_B_Avg[jj] += uc_dly;
        }
        if (uc_dly < 0x0f) 
        {
          uc_dly++;
          u4Wvalue += (((U32)uc_dly)<<(4*kk));        
        }

        if (uc_dly == 0x0f)     
        {
          CalStop = 1;
        }       
      }
          
      ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQODLY1+4*jj), u4Wvalue);
            mcSHOW_DBG_MSG(("Reg.%xh = %xh\n", DRAMC_REG_DQODLY1+4*jj, u4Wvalue));
          }         
      }

#ifdef XTALK_SSO_K
  *(volatile unsigned *)(EMI_APB_BASE+0x00000000) &= (~0x01); // disable dual channel.
  if (p->channel == CHANNEL_A)
  {
    *(volatile unsigned *)(EMI_APB_BASE+0x00000000) &= (~(0x01<<18));
  }
  else
  {
    *(volatile unsigned *)(EMI_APB_BASE+0x00000000) |= ((0x01<<18));
  }
        u4err_value = DramcDmaEngine((DRAMC_CTX_T *)p, DMA_OP_READ_WRITE, DDR_BASE, 
          DDR_BASE+0x100000, XTALK_SSO_LEN<<3, 8, 1, 1, 0);
        
  #ifdef SINGLE_CHANNEL_ENABLE        
        *(volatile unsigned *)(EMI_APB_BASE+0x00000000) = LPDDR3_EMI_CONA;
  #else
        *(volatile unsigned *)(EMI_APB_BASE+0x00000000) |= (0x01);  // enable dual channel.
        #endif
#else
        if (p->test_pattern== TEST_AUDIO_PATTERN)
        {
            // enable TE2, audio pattern
            u4err_value = 0;
            for (jj = 0; jj < 1; jj++)
            {
                u4err_value |= DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 1, 0, 0, 0);
            }            
        }
        else if (p->test_pattern == TEST_ISI_PATTERN)
        {
            // enable TE2, ISI pattern
            u4err_value = 0;
            for (jj = 0; jj < 1; jj++)
            {
                u4err_value |= DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 0, 0, 0, 0);
            }            
        }
        else if (p->test_pattern == TEST_XTALK_PATTERN)
        {
            u4err_value = DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 2, 0, 0, 0);
        }
        else if (p->test_pattern == TEST_MIX_PATTERN)
        {
            u4err_value = DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 1, 0, 0, 0);
            u4err_value |= DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 2, 0, 0, 0);
        }
        else
        {
            mcSHOW_ERR_MSG(("Not support test pattern!! Use audio pattern by default.\n"));
            u4err_value = DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 1, 0, 0, 0);
        }
#endif

        // check fail bit ,0 ok ,others fail
        for (jj = 0; jj < (p->data_width/DQS_BIT_NUMBER); jj++)
        {
            u4fail_bit = u4err_value & ((U32)0xff<<(jj*8));

            if (u4fail_bit == 0)
            {
                if (dqm_dly[jj].first_dqmdly_pass == -16)
                {
                    dqm_dly[jj].first_dqmdly_pass = ii;
                }
                if (dqm_dly[jj].last_dqmdly_pass == -17)
                {
                    if ((ii == (MAX_TX_DQDLY_TAPS-1)) || CalStop)
                    {
                        // pass to the last tap
                        dqm_dly[jj].last_dqmdly_pass = ii;
                        if ( (dqm_dly[jj].last_dqmdly_pass-dqm_dly[jj].first_dqmdly_pass) > (dqm_dly[jj].best_last_dqmdly_pass-dqm_dly[jj].best_first_dqmdly_pass))
                        {
               dqm_dly[jj].best_last_dqmdly_pass =  dqm_dly[jj].last_dqmdly_pass;
               dqm_dly[jj].best_first_dqmdly_pass = dqm_dly[jj].first_dqmdly_pass;
                        }   
                        // clear to find the next pass range if it has
                        dqm_dly[jj].first_dqmdly_pass = -16;
                        dqm_dly[jj].last_dqmdly_pass = -17;                           
                    }
                }
            }
            else
            {
                if ((dqm_dly[jj].first_dqmdly_pass != -16)&&(dqm_dly[jj].last_dqmdly_pass == -17))
                {
                    dqm_dly[jj].last_dqmdly_pass = ii -1;
                    if ( (dqm_dly[jj].last_dqmdly_pass-dqm_dly[jj].first_dqmdly_pass) > (dqm_dly[jj].best_last_dqmdly_pass-dqm_dly[jj].best_first_dqmdly_pass))
                    {
           dqm_dly[jj].best_last_dqmdly_pass =  dqm_dly[jj].last_dqmdly_pass;
           dqm_dly[jj].best_first_dqmdly_pass = dqm_dly[jj].first_dqmdly_pass;
                    }   
                    // clear to find the next pass range if it has
                    dqm_dly[jj].first_dqmdly_pass = -16;
                    dqm_dly[jj].last_dqmdly_pass = -17;                    
                }
            }

            if (u4fail_bit == 0)
            {
                mcSHOW_DBG_MSG(("o"));
            }
            else
            {
                 mcSHOW_DBG_MSG(("x"));
            }            
        }
        mcSHOW_DBG_MSG(("\n"));
        if (CalStop) 
        {
          break;
        }        
    }

    for (jj = 0; jj < (p->data_width/DQS_BIT_NUMBER); jj++)
    {
        ucDQ_B_Avg[jj] = (ucDQ_B_Avg[jj]+7) / 8;
  DQMSkew[jj] = dqm_dly[jj].best_first_dqmdly_pass + ((dqm_dly[jj].best_last_dqmdly_pass - dqm_dly[jj].best_first_dqmdly_pass)/2);
  mcSHOW_DBG_MSG(("Center[%d] = %d, best_last_dqmdly_pass = %d, best_first_dqmdly_pass=%d, DQ_B_Avg[%d]=%d ", 
    jj, DQMSkew[jj], dqm_dly[jj].best_last_dqmdly_pass, dqm_dly[jj].best_first_dqmdly_pass, jj, ucDQ_B_Avg[jj]));
  
  DQMSkew[jj] = -DQMSkew[jj];
  if (DQMSkew[jj] >=0)
  {
    DQMSkew[jj] = DQMSkew[jj] - ucDQ_B_Avg[jj] -1;
  }
  else
  {
    DQMSkew[jj] = DQMSkew[jj] + ucDQ_B_Avg[jj] -1;
  }
  mcSHOW_DBG_MSG(("DQMSkew[%d] = %d\n", jj, DQMSkew[jj] ))
    }

    DramcPhyReset(p);

    return DRAM_OK;
}
#endif

//-------------------------------------------------------------------------
/** DramcTxWindowPerbitCal (v2)
 *  TX DQS per bit SW calibration.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param  apply           (U8): 0 don't apply the register we set  1 apply the register we set ,default don't apply.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL 
 */
//-------------------------------------------------------------------------
DRAM_STATUS_T DramcTxWindowPerbitCal(DRAMC_CTX_T *p)
{
    U8 ucstatus = 0, ii, jj, uc_dly;
    U32 u4value, u4err_value, u4fail_bit;

    TXDQS_PERBIT_DLY_T dqdqs_perbit_dly[DQ_DATA_WIDTH]={0,0,0,0,0,0,0,0,0,0};
    U8 ucbit_first, ucbit_last, uchold_pass_number, ucsetup_pass_number;
    U8 ucmax_dqsdly_byte[DQS_NUMBER]={0}, ucave_dqdly_byte[DQS_NUMBER]={0};
    U8 ucfail = 0;

    // error handling
    if (!p)
    {
        mcSHOW_ERR_MSG(("context is NULL\n"));
        return DRAM_FAIL;
    }
    
    DramcPhyReset(p);

    // for debug
#ifndef RELEASE       
    mcSHOW_DBG_MSG4(("[Channel %d] TX DQS per bit para : test pattern=%d, test2_1=0x%x, test2_2=0x%x\n", p->channel, p->test_pattern, p->test2_1, p->test2_2));    
#else
    mcSHOW_USER_MSG(("\nTX\n"));
#endif

    // delay DQS from first_dqs_dly to 15 to get the hold time
    // set DQ delay first
    uc_dly = FIRST_TX_DQ_DELAY;
    u4value = 0;
    for (ii = 0; ii < DQS_BIT_NUMBER; ii++)
    {
        u4value += (((U32) uc_dly) << (4*ii));
    }

    for (ii = 0; ii < (p->data_width/DQS_BIT_NUMBER); ii++)
    {
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQODLY1+4*ii), u4value);
    }

    // set DQM delay
    if (p->data_width == DATA_WIDTH_16BIT)
    {
        // for DQC, DQM3 is CS#
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL2), &u4value);
        mcSET_FIELD(u4value, 0, 0x000000ff, 0);
    }
    else
    {
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL2), &u4value);
        u4value &= 0xffff0000;
    }
    for (ii = 0; ii < (p->data_width/DQS_BIT_NUMBER); ii++)
    {
        #ifdef DQM_SHIFT
            { 
              S32 DQMdly = (S32)uc_dly + DQMSkew[ii];
              if (DQMdly < 0)
              {
                DQMdly = 0;
              }
              else if (DQMdly > 0x0f)
              {
                DQMdly = 0x0f;
              }
          u4value += (DQMdly << (4*ii));
            }
        #else
        u4value += (((U32) uc_dly) << (4*ii));
        #endif
    }    
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL2), u4value);
        
    // initialize parameters
    for (ii = 0; ii < p->data_width; ii++)
    {
        dqdqs_perbit_dly[ii].first_dqdly_pass = -1;
        dqdqs_perbit_dly[ii].last_dqdly_pass = -2;
        dqdqs_perbit_dly[ii].first_dqsdly_pass = -1;
        dqdqs_perbit_dly[ii].last_dqsdly_pass = -2;
        dqdqs_perbit_dly[ii].best_first_dqdly_pass = -1;
        dqdqs_perbit_dly[ii].best_last_dqdly_pass = -2;
        dqdqs_perbit_dly[ii].best_first_dqsdly_pass = -1;
        dqdqs_perbit_dly[ii].best_last_dqsdly_pass = -2;                
    }

    // for debug
    mcSHOW_DBG_MSG4(("------------------------------------------------------\n")); 
    mcSHOW_DBG_MSG4(("Start DQS delay to find pass range, DQ delay fixed to 0x%x...\n", u4value));
    mcSHOW_DBG_MSG4(("------------------------------------------------------\n")); 
    mcSHOW_DBG_MSG4(("x-axis is bit #; y-axis is DQS delay (%d~%d)\n", FIRST_TX_DQS_DELAY+1, MAX_TX_DQSDLY_TAPS-1));    

 
    // because the tap DQdly=0 DQSdly=0 will be counted when we delay dq ,so we don't count it here
    // so we set first dqs delay to 1
    for (ii = (FIRST_TX_DQS_DELAY+1); ii < MAX_TX_DQSDLY_TAPS; ii++)
    {    
        uc_dly = ii;    
        u4value = 0;
        for (jj = 0; jj < (p->data_width/DQS_BIT_NUMBER); jj++)
        {
            u4value += (((U32)uc_dly)<<(4*jj));        
        }
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL3), u4value);
        
        // for debug
        //mcSHOW_DBG_MSG4(("DQS dly:%d, DQS delay(swap):%d, Reg:0x%x\n", ii, uc_dly, u4value));

#ifdef XTALK_SSO_K
  *(volatile unsigned *)(EMI_APB_BASE+0x00000000) &= (~0x01); // disable dual channel.
  if (p->channel == CHANNEL_A)
  {
    *(volatile unsigned *)(EMI_APB_BASE+0x00000000) &= (~(0x01<<18));
  }
  else
  {
    *(volatile unsigned *)(EMI_APB_BASE+0x00000000) |= ((0x01<<18));
  }
        u4err_value = DramcDmaEngine((DRAMC_CTX_T *)p, DMA_OP_READ_WRITE, DDR_BASE, 
          DDR_BASE+0x100000, XTALK_SSO_LEN<<3, 8, 1, 1, 0);
        
  #ifdef SINGLE_CHANNEL_ENABLE        
        *(volatile unsigned *)(EMI_APB_BASE+0x00000000) = LPDDR3_EMI_CONA;
  #else
        *(volatile unsigned *)(EMI_APB_BASE+0x00000000) |= (0x01);  // enable dual channel.
        #endif
#else
        if (p->test_pattern== TEST_AUDIO_PATTERN)
        {
            // enable TE2, audio pattern
            u4err_value = 0;
            for (jj = 0; jj < 1; jj++)
            {
                u4err_value |= DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 1, 0, 0, 0);
            }            
        }
        else if (p->test_pattern == TEST_ISI_PATTERN)
        {
            // enable TE2, ISI pattern
            u4err_value = 0;
            for (jj = 0; jj < 1; jj++)
            {
                u4err_value |= DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 0, 0, 0, 0);
            }            
        }
        else if (p->test_pattern == TEST_XTALK_PATTERN)
        {
            u4err_value = DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 2, 0, 0, 0);
        }
        else if (p->test_pattern == TEST_MIX_PATTERN)
        {
            u4err_value = DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 1, 0, 0, 0);
            u4err_value |= DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 2, 0, 0, 0);
        }
        else
        {
            mcSHOW_ERR_MSG(("Not support test pattern!! Use audio pattern by default.\n"));
            u4err_value = DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 1, 0, 0, 0);
        }
#endif

        // check fail bit ,0 ok ,others fail
        for (jj = 0; jj < p->data_width; jj++)
        {
            u4fail_bit = u4err_value&((U32)1<<jj);

            if (u4fail_bit == 0)
            {
                if (dqdqs_perbit_dly[jj].first_dqsdly_pass == -1)
                {
                    // first DQS pass delay tap
                    dqdqs_perbit_dly[jj].first_dqsdly_pass = ii;
                }
                if (dqdqs_perbit_dly[jj].last_dqsdly_pass == -2)
                {
                    if (ii == (MAX_TX_DQSDLY_TAPS-1))
                    {
                        // pass to the last tap
                        dqdqs_perbit_dly[jj].last_dqsdly_pass = ii;
                        if ( (dqdqs_perbit_dly[jj].last_dqsdly_pass-dqdqs_perbit_dly[jj].first_dqsdly_pass) > (dqdqs_perbit_dly[jj].best_last_dqsdly_pass-dqdqs_perbit_dly[jj].best_first_dqsdly_pass))
                        {
               dqdqs_perbit_dly[jj].best_last_dqsdly_pass =  dqdqs_perbit_dly[jj].last_dqsdly_pass;
               dqdqs_perbit_dly[jj].best_first_dqsdly_pass = dqdqs_perbit_dly[jj].first_dqsdly_pass;
                        }   
                        // clear to find the next pass range if it has
                        dqdqs_perbit_dly[jj].first_dqsdly_pass = -1;
                        dqdqs_perbit_dly[jj].last_dqsdly_pass = -2;                                
                    }
                }
            }
            else
            {
                if ((dqdqs_perbit_dly[jj].first_dqsdly_pass != -1)&&(dqdqs_perbit_dly[jj].last_dqsdly_pass == -2))
                {
                    dqdqs_perbit_dly[jj].last_dqsdly_pass = ii -1;
                    if ( (dqdqs_perbit_dly[jj].last_dqsdly_pass-dqdqs_perbit_dly[jj].first_dqsdly_pass) > (dqdqs_perbit_dly[jj].best_last_dqsdly_pass-dqdqs_perbit_dly[jj].best_first_dqsdly_pass))
                    {
           dqdqs_perbit_dly[jj].best_last_dqsdly_pass =  dqdqs_perbit_dly[jj].last_dqsdly_pass;
           dqdqs_perbit_dly[jj].best_first_dqsdly_pass = dqdqs_perbit_dly[jj].first_dqsdly_pass;
                    }   
                    // clear to find the next pass range if it has
                    dqdqs_perbit_dly[jj].first_dqsdly_pass = -1;
                    dqdqs_perbit_dly[jj].last_dqsdly_pass = -2;                                   
                }
            }

            if (u4fail_bit == 0)
            {
                mcSHOW_USER_MSG(("o"));
            }
            else
            {
                {
                    mcSHOW_USER_MSG(("x"));
                }
            }            
        }
        mcSHOW_USER_MSG(("\n"));
    }

    // set first DQS delay
    uc_dly = FIRST_TX_DQS_DELAY;
    u4value = 0;
    for (jj = 0; jj < (p->data_width/DQS_BIT_NUMBER); jj++)
    {
        u4value += (((U32)uc_dly)<<(4*jj));        
    }
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL3), u4value);
    
    // for debug
#ifndef RELEASE         
    mcSHOW_DBG_MSG4(("------------------------------------------------------\n")); 
    mcSHOW_DBG_MSG4(("Start DQ delay to find pass range, DQS delay fixed to 0x%x...\n", u4value));
    mcSHOW_DBG_MSG4(("------------------------------------------------------\n")); 
    mcSHOW_DBG_MSG4(("x-axis is bit #; y-axis is DQ delay (%d~%d)\n", FIRST_TX_DQ_DELAY, MAX_TX_DQDLY_TAPS-1));

#else
    mcSHOW_USER_MSG(("\n"));
#endif    

    // delay DQ from first_dq_dly to 15 to get the setup time
    for (ii = FIRST_TX_DQ_DELAY; ii < MAX_TX_DQDLY_TAPS; ii++)
    {    
        uc_dly = ii;    
        // set DQ delay
        u4value = 0;
        for (jj = 0; jj < DQS_BIT_NUMBER; jj++)
        {
            u4value += (((U32) uc_dly) << (4*jj));
        }
        for (jj = 0; jj < (p->data_width/DQS_BIT_NUMBER); jj++)
        {
            ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQODLY1+4*jj), u4value);
        }
        
        // set DQM delay        
        if (p->data_width == DATA_WIDTH_16BIT)
        {
            // for DQC, DQM3 is CS#
            ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL2), &u4value);
            mcSET_FIELD(u4value, 0, 0x000000ff, 0);
        }
        else
        {
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL2), &u4value);
            u4value &= 0xffff0000;
        }
        for (jj = 0; jj < (p->data_width/DQS_BIT_NUMBER); jj++)
        {
        #ifdef DQM_SHIFT
            { 
              S32 DQMdly = (S32)uc_dly + DQMSkew[jj];
              if (DQMdly < 0)
              {
                DQMdly = 0;
              }
              else if (DQMdly > 0x0f)
              {
                DQMdly = 0x0f;
              }
          u4value += (DQMdly << (4*jj));
            }
        #else
            u4value += (((U32) uc_dly) << (4*jj));
        #endif
        }
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL2), u4value);
        //mcSHOW_DBG_MSG4(("DRAMC_REG_PADCTL2 = 0x%x\n", u4value));

        // for debug
        //mcSHOW_DBG_MSG4(("DQ delay:%d, DQ delay (swap):%d, Register:0x%x\n", ii, uc_dly, u4value));

#ifdef XTALK_SSO_K
  *(volatile unsigned *)(EMI_APB_BASE+0x00000000) &= (~0x01); // disable dual channel.
  if (p->channel == CHANNEL_A)
  {
    *(volatile unsigned *)(EMI_APB_BASE+0x00000000) &= (~(0x01<<18));
  }
  else
  {
    *(volatile unsigned *)(EMI_APB_BASE+0x00000000) |= ((0x01<<18));
  }
        u4err_value = DramcDmaEngine((DRAMC_CTX_T *)p, DMA_OP_READ_WRITE, DDR_BASE, 
          DDR_BASE+0x100000, XTALK_SSO_LEN<<3, 8, 1, 1, 0);
        
  #ifdef SINGLE_CHANNEL_ENABLE        
        *(volatile unsigned *)(EMI_APB_BASE+0x00000000) = LPDDR3_EMI_CONA;
  #else
        *(volatile unsigned *)(EMI_APB_BASE+0x00000000) |= (0x01);  // enable dual channel.
        #endif
#else
        if (p->test_pattern== TEST_AUDIO_PATTERN)
        {
            // enable TE2, audio pattern
            u4err_value = 0;
            for (jj = 0; jj < 1; jj++)
            {
                u4err_value |= DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 1, 0, 0, 0);
            }            
        }
        else if (p->test_pattern == TEST_ISI_PATTERN)
        {
            // enable TE2, ISI pattern
            u4err_value = 0;
            for (jj = 0; jj < 1; jj++)
            {
                u4err_value |= DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 0, 0, 0, 0);
            }            
        }
        else if (p->test_pattern == TEST_XTALK_PATTERN)
        {
            u4err_value = DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 2, 0, 0, 0);
        }
        else if (p->test_pattern == TEST_MIX_PATTERN)
        {
            u4err_value = DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 1, 0, 0, 0);
            u4err_value |= DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 2, 0, 0, 0);
        }
        else
        {
            mcSHOW_ERR_MSG(("Not support test pattern!! Use audio pattern by default.\n"));
            u4err_value = DramcEngine2(p, TE_OP_WRITE_READ_CHECK, p->test2_1, p->test2_2, 1, 0, 0, 0);
        }
#endif

        // check fail bit ,0 ok ,others fail
        for (jj = 0; jj < p->data_width; jj++)
        {
            u4fail_bit = u4err_value&((U32)1<<jj);

            if (u4fail_bit == 0)
            {
                if (dqdqs_perbit_dly[jj].first_dqdly_pass == -1)
                {
                    // first DQS pass delay tap
                    dqdqs_perbit_dly[jj].first_dqdly_pass = ii;
                }
                if (dqdqs_perbit_dly[jj].last_dqdly_pass == -2)
                {
                    if (ii == (MAX_TX_DQDLY_TAPS-1))
                    {
                        // pass to the last tap
                        dqdqs_perbit_dly[jj].last_dqdly_pass = ii;
                        if ( (dqdqs_perbit_dly[jj].last_dqdly_pass-dqdqs_perbit_dly[jj].first_dqdly_pass) > (dqdqs_perbit_dly[jj].best_last_dqdly_pass-dqdqs_perbit_dly[jj].best_first_dqdly_pass))
                        {
               dqdqs_perbit_dly[jj].best_last_dqdly_pass =  dqdqs_perbit_dly[jj].last_dqdly_pass;
               dqdqs_perbit_dly[jj].best_first_dqdly_pass = dqdqs_perbit_dly[jj].first_dqdly_pass;
                        }   
                        // clear to find the next pass range if it has
                        dqdqs_perbit_dly[jj].first_dqdly_pass = -1;
                        dqdqs_perbit_dly[jj].last_dqdly_pass = -2;                           
                    }
                }
            }
            else
            {
                if ((dqdqs_perbit_dly[jj].first_dqdly_pass != -1)&&(dqdqs_perbit_dly[jj].last_dqdly_pass == -2))
                {
                    dqdqs_perbit_dly[jj].last_dqdly_pass = ii -1;
                    if ( (dqdqs_perbit_dly[jj].last_dqdly_pass-dqdqs_perbit_dly[jj].first_dqdly_pass) > (dqdqs_perbit_dly[jj].best_last_dqdly_pass-dqdqs_perbit_dly[jj].best_first_dqdly_pass))
                    {
           dqdqs_perbit_dly[jj].best_last_dqdly_pass =  dqdqs_perbit_dly[jj].last_dqdly_pass;
           dqdqs_perbit_dly[jj].best_first_dqdly_pass = dqdqs_perbit_dly[jj].first_dqdly_pass;
                    }   
                    // clear to find the next pass range if it has
                    dqdqs_perbit_dly[jj].first_dqdly_pass = -1;
                    dqdqs_perbit_dly[jj].last_dqdly_pass = -2;                    
                }
            }

            if (u4fail_bit == 0)
            {
                mcSHOW_USER_MSG(("o"));
            }
            else
            {
                {
                    mcSHOW_USER_MSG(("x"));
                }
            }            
        }
        mcSHOW_USER_MSG(("\n"));
    }

    // for debug
    mcSHOW_DBG_MSG4(("------------------------------------------------------\n")); 
    mcSHOW_DBG_MSG4(("Start calculate setup time and hold time / \n"));
    mcSHOW_DBG_MSG4(("Find max DQS delay per byte / Adjust DQ delay to align DQS...\n"));
    mcSHOW_DBG_MSG4(("------------------------------------------------------\n")); 
    
#ifdef TDP_TDN_CAL
    u2tx_window_sum = 0;
#endif    
    //As per byte, check max DQS delay in 8-bit. Except for the bit of max DQS delay, delay DQ to fulfill setup time = hold time
    for (ii = 0; ii < (p->data_width/DQS_BIT_NUMBER); ii++)
    {
        ucbit_first = DQS_BIT_NUMBER*ii;
        ucbit_last = DQS_BIT_NUMBER*ii+DQS_BIT_NUMBER-1;
        ucmax_dqsdly_byte[ii] = 0;
        // init accumulation variable to 0
        ucave_dqdly_byte[ii] = 0;    // for DQM
        
        for (jj = ucbit_first; jj <= ucbit_last; jj++)
        {
            // hold time = DQS pass taps
            uchold_pass_number = dqdqs_perbit_dly[jj].best_last_dqsdly_pass - dqdqs_perbit_dly[jj].best_first_dqsdly_pass + 1;
            // setup time = DQ pass taps
            ucsetup_pass_number = dqdqs_perbit_dly[jj].best_last_dqdly_pass - dqdqs_perbit_dly[jj].best_first_dqdly_pass + 1;

#ifdef TDP_TDN_CAL
      u2tx_window_sum += (uchold_pass_number+ucsetup_pass_number);
#endif 

            if (uchold_pass_number > ucsetup_pass_number)
            {
                if (ucsetup_pass_number != 0)
                {
                    // like this:
                    // setup time(dq delay)     hold time(dqs delay)
                    // 15                  0 1                       15 tap
                    // xxxxxxxxxxxxxoooooooo|ooooooooooooooooooooxxxxx
                    dqdqs_perbit_dly[jj].best_dqdly = 0;
                    dqdqs_perbit_dly[jj].best_dqsdly = (uchold_pass_number - ucsetup_pass_number) / 2;

                    if (dqdqs_perbit_dly[jj].best_dqsdly > ucmax_dqsdly_byte[ii])
                    {
                        ucmax_dqsdly_byte[ii] = dqdqs_perbit_dly[jj].best_dqsdly;
                    }
                }
                else
                {
                    // like this:
                    // setup time(dq delay)     hold time(dqs delay)
                    // 15                  0 1                       15 tap
                    // xxxxxxxxxxxxxxxxxxxxx|xxxooooooooooxxxxxxxxxxxx
                    dqdqs_perbit_dly[jj].best_dqdly = 0;
                    dqdqs_perbit_dly[jj].best_dqsdly = (uchold_pass_number - ucsetup_pass_number) / 2 + dqdqs_perbit_dly[jj].best_first_dqsdly_pass;

                    if (dqdqs_perbit_dly[jj].best_dqsdly > ucmax_dqsdly_byte[ii])
                    {
                        ucmax_dqsdly_byte[ii] = dqdqs_perbit_dly[jj].best_dqsdly;
                    }
                }
            }
            else if (uchold_pass_number < ucsetup_pass_number)
            {
                if (uchold_pass_number != 0)
                {
                    // like this:
                    // setup time(dq delay)     hold time(dqs delay)
                    // 15                  0 1                       15 tap
                    // xxxoooooooooooooooooo|ooooooooxxxxxxxxxxxxxxxxx
                    dqdqs_perbit_dly[jj].best_dqsdly = 0;
                    dqdqs_perbit_dly[jj].best_dqdly = (ucsetup_pass_number - uchold_pass_number) / 2;                    
                }
                else
                {
                    // like this:
                    // setup time(dq delay)     hold time(dqs delay)
                    // 15                  0 1                       15 tap
                    // xxxoooooooooooooooxxx|xxxxxxxxxxxxxxxxxxxxxxxxx
                    dqdqs_perbit_dly[jj].best_dqsdly = 0;
                    dqdqs_perbit_dly[jj].best_dqdly = (ucsetup_pass_number - uchold_pass_number) / 2 + dqdqs_perbit_dly[jj].best_first_dqdly_pass;                    
                }
            }
            else   // hold time == setup time
            {
                if (uchold_pass_number != 0)
                {
                    // like this:
                    // setup time(dq delay)     hold time(dqs delay)
                    // 15                  0 1                       15 tap
                    // xxxxxxxxxxxxxxxoooooo|ooooooxxxxxxxxxxxxxxxxxxx
                    dqdqs_perbit_dly[jj].best_dqsdly = 0;
                    dqdqs_perbit_dly[jj].best_dqdly = 0;    
                }
                else
                {
                    // like this:
                    // setup time(dq delay)     hold time(dqs delay)
                    // 15                  0 1                       15 tap
                    // xxxxxxxxxxxxxxxxxxxxx|xxxxxxxxxxxxxxxxxxxxxxxxx
                    // mean this bit is error
                    mcSHOW_DBG_MSG4(("error on bit %d ,setup_time =hold_time =0!!\n", jj));
                    dqdqs_perbit_dly[jj].best_dqsdly = 0;
                    dqdqs_perbit_dly[jj].best_dqdly = 0;
                    ucfail = 1;
                #ifdef DDR_FT_LOAD_BOARD
                    LoadBoardShowResult(FLAG_TX_CALIBRATION, FLAG_CALIBRATION_FAIL, p->channel, FLAG_NOT_COMPLETE_OR_FAIL);            
                    while(1); 
                #endif
                }
            }            
            mcSHOW_DBG_MSG4(("bit#%d : setup time=%d hold time=%d win=%d, center (DQ,DQS)=(%d, %d)\n", jj, ucsetup_pass_number, uchold_pass_number, ucsetup_pass_number+uchold_pass_number, dqdqs_perbit_dly[jj].best_dqdly, dqdqs_perbit_dly[jj].best_dqsdly));
#ifdef EYE_SCAN
      EyeScanWin[jj].ucsetup_pass_number = ucsetup_pass_number;
      EyeScanWin[jj].uchold_pass_number = uchold_pass_number;
#endif

        #ifdef DDR_FT_LOAD_BOARD
            if ((ucsetup_pass_number+uchold_pass_number)<=TXWIN_CALIB_BOUND)
            {
                LoadBoardShowResult(FLAG_TX_CALIBRATION, FLAG_WINDOW_TOO_SMALL, p->channel, FLAG_NOT_COMPLETE_OR_FAIL);            
                while(1); 
            }
        #endif
        }

        // we delay DQ or DQS to let DQS sample the middle of tx pass window for all the 8 bits,
        for (jj = ucbit_first; jj <= ucbit_last; jj++)
        {
            // set DQS to max for 8-bit
            if (dqdqs_perbit_dly[jj].best_dqsdly < ucmax_dqsdly_byte[ii])
            {
                // delay DQ to compensate extra DQS delay
                dqdqs_perbit_dly[jj].best_dqdly = dqdqs_perbit_dly[jj].best_dqdly + (ucmax_dqsdly_byte[ii] - dqdqs_perbit_dly[jj].best_dqsdly);
                // max limit to 15
                dqdqs_perbit_dly[jj].best_dqdly = ((dqdqs_perbit_dly[jj].best_dqdly > (MAX_TX_DQDLY_TAPS-1)) ? (MAX_TX_DQDLY_TAPS-1) : dqdqs_perbit_dly[jj].best_dqdly);
            }
            ucave_dqdly_byte[ii] += dqdqs_perbit_dly[jj].best_dqdly;            
        }
        // take the average of DQ for DQM
        ucave_dqdly_byte[ii] = ucave_dqdly_byte[ii] / DQS_BIT_NUMBER;
    }
       
   mcSHOW_DBG_MSG4(("==================================================\n"));
   mcSHOW_DBG_MSG4(("        dramc_txdqs_perbit_swcal\n"));
   mcSHOW_DBG_MSG4(("           channel=%d(0:cha, 1:chb) \n", p->channel));
   mcSHOW_DBG_MSG4(("           bus width=%d\n", p->data_width));
   mcSHOW_DBG_MSG4(("==================================================\n"));
   mcSHOW_DBG_MSG4(("DQS Delay :\n DQS0 = %d DQS1 = %d DQS2 = %d DQS3 = %d\n", ucmax_dqsdly_byte[0], ucmax_dqsdly_byte[1], ucmax_dqsdly_byte[2], ucmax_dqsdly_byte[3]));
   mcSHOW_DBG_MSG4(("DQM Delay :\n DQM0 = %d DQM1 = %d DQM2 = %d DQM3 = %d\n", ucave_dqdly_byte[0], ucave_dqdly_byte[1], ucave_dqdly_byte[2], ucave_dqdly_byte[3]));
   mcSHOW_DBG_MSG4(("DQ Delay :\n"));

    
    for (ii = 0; ii < p->data_width; ii=ii+4)
    {
       mcSHOW_DBG_MSG4(("DQ%d = %d DQ%d = %d DQ%d = %d DQ%d = %d \n", ii, dqdqs_perbit_dly[ii].best_dqdly, ii+1, dqdqs_perbit_dly[ii+1].best_dqdly, ii+2, dqdqs_perbit_dly[ii+2].best_dqdly, ii+3, dqdqs_perbit_dly[ii+3].best_dqdly));
    }
   mcSHOW_DBG_MSG4(("________________________________________________________________________\n")); 

    // Add CLK to DQS/DQ skew after write leveling
    if (fgwrlevel_done[p->channel])
    {
       mcSHOW_DBG_MSG4(("Add CLK to DQS/DQ skew based on write leveling.\n"));
        
        for (ii = 0; ii < (p->data_width/DQS_BIT_NUMBER); ii++)
        {
           mcSHOW_DBG_MSG4(("DQS%d: %d  ", ii, wrlevel_dqs_final_delay[p->channel][ii]));

      if (ucmax_dqsdly_byte[ii] <= wrlevel_dqs_final_delay[p->channel][ii])
      {
    // DQ
    ucbit_first = DQS_BIT_NUMBER*ii;
    ucbit_last = DQS_BIT_NUMBER*ii+DQS_BIT_NUMBER-1;
    for (jj = ucbit_first; jj <= ucbit_last; jj++)
    {
        dqdqs_perbit_dly[jj].best_dqdly += (wrlevel_dqs_final_delay[p->channel][ii] - ucmax_dqsdly_byte[ii]);
        // max limit to 15
        dqdqs_perbit_dly[jj].best_dqdly = ((dqdqs_perbit_dly[jj].best_dqdly > (MAX_TX_DQDLY_TAPS-1)) ? (MAX_TX_DQDLY_TAPS-1) : dqdqs_perbit_dly[jj].best_dqdly);
    }       
    // DQM
    ucave_dqdly_byte[ii] += (wrlevel_dqs_final_delay[p->channel][ii] - ucmax_dqsdly_byte[ii]);
    // max limit to 15
    ucave_dqdly_byte[ii] = ((ucave_dqdly_byte[ii] > (MAX_TX_DQDLY_TAPS-1)) ? (MAX_TX_DQDLY_TAPS-1) : ucave_dqdly_byte[ii]);

    // DQS
    ucmax_dqsdly_byte[ii] = wrlevel_dqs_final_delay[p->channel][ii];    
      }
      else
      {
        // ucmax_dqsdly_byte[ii] > wrlevel_dqs_final_delay[p->channel][ii])
        // Originally should move clk delay and CA delay accordingly. Then GW calibration again. Too complicated.
        // DQ/DQS skew should not be large according to DE. So sacrifice the Clk/DQS margin by keeping the clk out delay.
    mcSHOW_DBG_MSG4(("[Warning] DQSO %d in TX per-bit = %d > DQSO %d in WL = %d  ", 
      ii, ucmax_dqsdly_byte[ii], ii, wrlevel_dqs_final_delay[p->channel][ii]));
      }


        }
       mcSHOW_DBG_MSG4(("\n"));
    }
   
    // Set best delay to registers
    // 0x014, DQS delay, each DQS has 4 bits. DQS0 [3:0], DQS1 [7:4], DQS2 [11:8], DQS3 [15:12]
    u4value = 0;
    for (jj = 0; jj < (p->data_width/DQS_BIT_NUMBER); jj++)
    {
        u4value += (((U32)ucmax_dqsdly_byte[jj])<<(4*jj));        
    }
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL3), u4value);
   mcSHOW_DBG_MSG4(("TX DQS register 0x%x=0x%x\n", DRAMC_REG_PADCTL3, u4value));

    // 0x200~0x20c, DQ delay, each DQ has 4 bits. Each register contains 8-bit DQ's
    for (ii = 0; ii < (p->data_width/DQS_BIT_NUMBER); ii++)
    {
        u4value = 0;
        ucbit_first = DQS_BIT_NUMBER*ii;
        ucbit_last = DQS_BIT_NUMBER*ii+DQS_BIT_NUMBER-1;
        for (jj = ucbit_first; jj <= ucbit_last; jj++)
        {
            u4value += (((U32) (dqdqs_perbit_dly[jj].best_dqdly)) << (4*(jj-ucbit_first)));
        }
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQODLY1+4*ii), u4value);
       mcSHOW_DBG_MSG4(("TX DQ register 0x%x=0x%x\n", DRAMC_REG_DQODLY1+4*ii, u4value));
    } 
    // set DQM delay
    {
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL2), &u4value);
        u4value &= 0xffff0000;
    }
    for (jj = 0; jj < (p->data_width/DQS_BIT_NUMBER); jj++)
    {
        #ifdef DQM_SHIFT
            { 
              S32 DQMdly = (S32)ucave_dqdly_byte[jj] + DQMSkew[jj];
              if (DQMdly < 0)
              {
                DQMdly = 0;
              }
              else if (DQMdly > 0x0f)
              {
                DQMdly = 0x0f;
              }
          u4value += (DQMdly << (4*jj));
            }
        #else
        u4value += (((U32) ucave_dqdly_byte[jj]) << (4*jj));
        #endif
    
    }
    ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL2), u4value);
    mcSHOW_DBG_MSG4(("TX DQM register 0x%x=0x%x\n", DRAMC_REG_PADCTL2, u4value));

    DramcPhyReset(p);   

    // Log Example
/*
DQS Delay :
 DQS0 = 1 DQS1 = 2 DQS2 = 0 DQS3 = 1
DQ Delay :
 DQ0 = 0 DQ1 = 2 DQ2 = 1 DQ3 = 0 
 DQ4 = 2 DQ5 = 2 DQ6 = 3 DQ7 = 1 
 DQ8 = 1 DQ9 = 0 DQ10 = 2 DQ11 = 0 
 DQ12 = 2 DQ13 = 0 DQ14 = 6 DQ15 = 4 
 DQ16 = 1 DQ17 = 1 DQ18 = 0 DQ19 = 1 
 DQ20 = 1 DQ21 = 2 DQ22 = 1 DQ23 = 0 
 DQ24 = 1 DQ25 = 0 DQ26 = 0 DQ27 = 1 
 DQ28 = 3 DQ29 = 0 DQ30 = 5 DQ31 = 2 
________________________________________________________________________
bit         setup time           hold time
          15             0 1             15
 BIT 0 |  xxxxxxxxx======= =========xxxxxx  setup time :7 hold time : 9
 BIT 1 |  xxxxxxx========= =======xxxxxxxx  setup time :9 hold time : 7
 BIT 2 |  xxxxxxxx======== =======xxxxxxxx  setup time :8 hold time : 7
 BIT 3 |  xxxxxxxxxx====== =========xxxxxx  setup time :6 hold time : 9
 BIT 4 |  xxxxxxxx======== ======xxxxxxxxx  setup time :8 hold time : 6
 BIT 5 |  xxxxxxxx======== =====xxxxxxxxxx  setup time :8 hold time : 5
 BIT 6 |  xxxxxxx========= =====xxxxxxxxxx  setup time :9 hold time : 5
 BIT 7 |  xxxxxxxxxx====== =======xxxxxxxx  setup time :6 hold time : 7

 BIT 8 |  xxxxxxxxxx====== =========xxxxxx  setup time :6 hold time : 9
 BIT 9 |  xxxxxxxxxxxx==== =========xxxxxx  setup time :4 hold time : 9
BIT 10 |  xxxxxxxxxx====== =======xxxxxxxx  setup time :6 hold time : 7
BIT 11 |  xxxxxxxxxxxx==== ========xxxxxxx  setup time :4 hold time : 8
BIT 12 |  xxxxxxxx======== =======xxxxxxxx  setup time :8 hold time : 7
BIT 13 |  xxxxxxxxxxxx==== =========xxxxxx  setup time :4 hold time : 9
BIT 14 |  xxxx============ ====xxxxxxxxxxx  setup time :12 hold time : 4
BIT 15 |  xxxxxx========== =====xxxxxxxxxx  setup time :10 hold time : 5

BIT 16 |  xxxxxxxxxx====== ====xxxxxxxxxxx  setup time :6 hold time : 4
BIT 17 |  xxxxxxxxxx====== ====xxxxxxxxxxx  setup time :6 hold time : 4
BIT 18 |  xxxxxxxxxxx===== ====xxxxxxxxxxx  setup time :5 hold time : 4
BIT 19 |  xxxxxxxxx======= ====xxxxxxxxxxx  setup time :7 hold time : 4
BIT 20 |  xxxxxxxxxx====== ====xxxxxxxxxxx  setup time :6 hold time : 4
BIT 21 |  xxxxxxxx======== ====xxxxxxxxxxx  setup time :8 hold time : 4
BIT 22 |  xxxxxxxxx======= ====xxxxxxxxxxx  setup time :7 hold time : 4
BIT 23 |  xxxxxxxxxxx===== ====xxxxxxxxxxx  setup time :5 hold time : 4

BIT 24 |  xxxxxxxx======== =========xxxxxx  setup time :8 hold time : 9
BIT 25 |  xxxxxxxx======== ==========xxxxx  setup time :8 hold time : 10
BIT 26 |  xxxxxxxx======== ==========xxxxx  setup time :8 hold time : 10
BIT 27 |  xxxxxxx========= ========xxxxxxx  setup time :9 hold time : 8
BIT 28 |  xxxxxx========== ======xxxxxxxxx  setup time :10 hold time : 6
BIT 29 |  xxxxxxxx======== ==========xxxxx  setup time :8 hold time : 10
BIT 30 |  xxx============= =====xxxxxxxxxx  setup time :13 hold time : 5
BIT 31 |  xxxxxx========== ========xxxxxxx  setup time :10 hold time : 8
*/

    if (ucfail == 1)
    {
        return DRAM_FAIL;
    }

#ifdef DDR_FT_LOAD_BOARD
    if (p->channel == CHANNEL_B)
    {
        LoadBoardShowResult(FLAG_TX_CALIBRATION, FLAG_CALIBRATION_PASS, p->channel, FLAG_COMPLETE_AND_PASS);
    }
#endif

    return DRAM_OK;
}


#ifdef EYE_SCAN

#ifdef ACDIO_TEST 
#define REFTEST_NUM   3
const S32 RefTest[3] = {4, 15, 27};
const S32 RefTestV[3] = {400, 600, 800};
#endif

void DramcEyeScan(DRAMC_CTX_T *p)
{
  U8 ucstatus = 0;
  U32 u4value;
  U8 ucBit, ucCount;
  U32 *uiLPDDR_PHY_Mapping = NULL;
  U32 APRefTune, DQRefTune, CARefTune;
  U32 DS = 3;
  S32 ucRefTune;
#ifdef ACDIO_TEST 
  S32 ucRefTestCnt;
  S32 TotalWin;
#endif  

#if fcFOR_CHIP_ID == fcK2 || fcFOR_CHIP_ID == fcJADE  
    //if (p->dram_type == TYPE_LPDDR3)  
       uiLPDDR_PHY_Mapping = (U32 *)uiLPDDR3_PHY_Mapping_POP_CHA;
#endif  

#ifndef ACDIO_TEST  
  // RX data eye
  mcSHOW_DBG_MSG(("***************  RX data eye *******************\n"));
  for (ucBit=0; ucBit<32; ucBit++)
  {
    mcSHOW_DBG_MSG(("\n[RX] DDRPHY DQ %d LPDDR DQ %d\n", ucBit, uiLPDDR_PHY_Mapping[ucBit]));
    mcSHOW_DBG_MSG(("================================================= \n"));

    for (ucRefTune=31; ucRefTune>0; ucRefTune--)
    {
      if (ucRefTune<10) {
        mcSHOW_DBG_MSG((" %d | ", ucRefTune));
      } else {
        mcSHOW_DBG_MSG(("%d | ", ucRefTune));
      }   


    #if fcFOR_CHIP_ID == fcK2 || fcFOR_CHIP_ID == fcJADE
            ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x644), &u4value);
      u4value &= 0xffffff00;
      u4value |= (((ucRefTune<<3) | (DS<<1) | (1)) << 0);
            ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x644), u4value);        
    #endif            
      
      mcDELAY_MS(25);

      RXPERBIT_LOG_PRINT = 0;
      DramcRxWindowPerbitCal((DRAMC_CTX_T *) p);  
      RXPERBIT_LOG_PRINT = 1;
      for (ucCount=16; ucCount>0; ucCount--) 
      {
        if (ucCount>EyeScanWin[ucBit].ucsetup_pass_number)
        {
          mcSHOW_DBG_MSG(("X"));
        }
        else
        {
          mcSHOW_DBG_MSG(("O"));
        }
      }
      for (ucCount=1; ucCount<=64; ucCount++) 
      {
        if (ucCount>EyeScanWin[ucBit].uchold_pass_number)
        {
          mcSHOW_DBG_MSG(("X"));
        }
        else
        {
          mcSHOW_DBG_MSG(("O"));
        }
      }
      mcSHOW_DBG_MSG((" setup time=%d hold time=%d win size=%d",  
        EyeScanWin[ucBit].ucsetup_pass_number, EyeScanWin[ucBit].uchold_pass_number, EyeScanWin[ucBit].ucsetup_pass_number+EyeScanWin[ucBit].uchold_pass_number));
      
      mcSHOW_DBG_MSG(("\n"));

    }
  }

#if fcFOR_CHIP_ID == fcK2 || fcFOR_CHIP_ID == fcJADE
  // Reg.644h[7:3] INTREF0_REFTUNE=01111b      [2:1]INTREF0_DS_DQ=10b      [0] EN_INTREF0_DQ=1b
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x644), &u4value);
  u4value &= 0xffffff00;
  u4value |= 0x0000007d;
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x644), u4value);        
#endif        
  
  DramcRxWindowPerbitCal((DRAMC_CTX_T *) p);  
#endif  

#if 1
  // TX data eye
  mcSHOW_DBG_MSG(("***************  TX data eye *******************\n"));
  #ifdef ACDIO_TEST 
  TotalWin = 0;
  #endif
  for (ucBit=0; ucBit<32; ucBit++)
  {
    mcSHOW_DBG_MSG(("\n[TX] DDRPHY DQ %d LPDDR %d TX \n", ucBit, uiLPDDR_PHY_Mapping[ucBit]));
    mcSHOW_DBG_MSG(("================================================= \n"));

    #ifdef ACDIO_TEST 
    for (ucRefTestCnt=0; ucRefTestCnt<REFTEST_NUM; ucRefTestCnt++)
    {
      ucRefTune = RefTest[ucRefTestCnt];
      if (ucRefTune<10) {
        mcSHOW_DBG_MSG((" %d(%dmv) | ", ucRefTune, RefTestV[ucRefTestCnt]));
      } else {
        mcSHOW_DBG_MSG(("%d(%dmv) | ", ucRefTune, RefTestV[ucRefTestCnt]));
      }   
    #else
    for (ucRefTune=31; ucRefTune>0; ucRefTune--)
    {
      if (ucRefTune<10) {
        mcSHOW_DBG_MSG((" %d | ", ucRefTune));
      } else {
        mcSHOW_DBG_MSG(("%d | ", ucRefTune));
      }
    #endif

    #if fcFOR_CHIP_ID == fcK2 || fcFOR_CHIP_ID == fcJADE
            ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x648), &u4value);
      u4value &= 0xffff00ff;
      u4value |= (((ucRefTune<<3) | (DS<<1) | (1)) << 8);
            ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x648), u4value);        
    #endif          
      mcDELAY_MS(25);
      
      RXPERBIT_LOG_PRINT = 0;
      DramcTxWindowPerbitCal((DRAMC_CTX_T *) p);  
      RXPERBIT_LOG_PRINT = 1;
      #ifdef ACDIO_TEST 
      EyeScanWin[ucBit].window_number = EyeScanWin[ucBit].ucsetup_pass_number+EyeScanWin[ucBit].uchold_pass_number;
      mcSHOW_DBG_MSG(("window=%d setup time=%d hold time=%d",  
        EyeScanWin[ucBit].window_number ,
        EyeScanWin[ucBit].ucsetup_pass_number, EyeScanWin[ucBit].uchold_pass_number));
      TotalWin += EyeScanWin[ucBit].window_number ;
      #endif

      #ifndef ACDIO_TEST  
      for (ucCount=16; ucCount>0; ucCount--) 
      {
        if (ucCount>EyeScanWin[ucBit].ucsetup_pass_number)
        {
          mcSHOW_DBG_MSG(("X", ucRefTune));
        }
        else
        {
          mcSHOW_DBG_MSG(("O"));
        }
      }
      for (ucCount=1; ucCount<=15; ucCount++) 
      {
        if (ucCount>EyeScanWin[ucBit].uchold_pass_number)
        {
          mcSHOW_DBG_MSG(("X"));
        }
        else
        {
          mcSHOW_DBG_MSG(("O"));
        }
      }
      #endif
      mcSHOW_DBG_MSG((" setup time=%d hold time=%d win size=%d",  
        EyeScanWin[ucBit].ucsetup_pass_number, EyeScanWin[ucBit].uchold_pass_number, EyeScanWin[ucBit].ucsetup_pass_number+EyeScanWin[ucBit].uchold_pass_number));
      mcSHOW_DBG_MSG(("\n"));
    }
  } 
  #ifdef ACDIO_TEST 
  mcSHOW_DBG_MSG(("\n\nChannel %d Total window=%d\n\n",  p->channel, TotalWin));
  #endif    

#if fcFOR_CHIP_ID == fcK2 || fcFOR_CHIP_ID == fcJADE
  // Reg.648h[15:11] INTREF1_REFTUNE=01111b      [10:9]INTREF1_DS_DQ=10b      [8] EN_INTREF1_DQ=1b
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x648), &u4value);
  u4value &= 0xffff00ff;
  u4value |= 0x00007d00;
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x648), u4value);        
#endif

#ifndef ACDIO_TEST  
  // CA eye
  mcSHOW_DBG_MSG(("***************  CA eye *******************\n"));
  for (ucBit=0; ucBit<10; ucBit++)
  {
    mcSHOW_DBG_MSG(("\nCA %d  \n", ucBit));
    mcSHOW_DBG_MSG(("================================================= \n"));
  
    for (ucRefTune=31; ucRefTune>0; ucRefTune--)
    {
      if (ucRefTune<10) {
        mcSHOW_DBG_MSG((" %d | ", ucRefTune));
      } else {
        mcSHOW_DBG_MSG(("%d | ", ucRefTune));
      }

    #if fcFOR_CHIP_ID == fcK2 || fcFOR_CHIP_ID == fcJADE
            ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x648), &u4value);
      u4value &= 0xffffff00;
      u4value |= (((ucRefTune<<3) | (DS<<1) | (1)) << 0);
            ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x648), u4value);        
    #endif            
      
      mcDELAY_MS(25);
      
      RXPERBIT_LOG_PRINT = 0;
      DramcCATraining((DRAMC_CTX_T *) p);
      RXPERBIT_LOG_PRINT = 1;
      for (ucCount=16; ucCount>0; ucCount--) 
      {
        if (ucCount>EyeScanWin[ucBit].ucsetup_pass_number)
        {
          mcSHOW_DBG_MSG(("X", ucRefTune));
        }
        else
        {
          mcSHOW_DBG_MSG(("O"));
        }
      }
      for (ucCount=1; ucCount<=15; ucCount++) 
      {
        if (ucCount>EyeScanWin[ucBit].uchold_pass_number)
        {
          mcSHOW_DBG_MSG(("X"));
        }
        else
        {
          mcSHOW_DBG_MSG(("O"));
        }
      }
      mcSHOW_DBG_MSG((" setup time=%d hold time=%d win size=%d",  
        EyeScanWin[ucBit].ucsetup_pass_number, EyeScanWin[ucBit].uchold_pass_number, EyeScanWin[ucBit].ucsetup_pass_number+EyeScanWin[ucBit].uchold_pass_number));
      mcSHOW_DBG_MSG(("\n"));

    }
  } 

#if fcFOR_CHIP_ID == fcK2 || fcFOR_CHIP_ID == fcJADE
  // Reg.648h[7:3] INTREF1_REFTUNE=01111b      [2:1]INTREF1_DS_DQ=10b      [0] EN_INTREF1_DQ=1b
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x648), &u4value);
  u4value &= 0xffffff00;
  u4value |= 0x0000007d;
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x648), u4value);        
#endif

#endif

#endif  
}
#endif    

#ifdef LOOPBACK_TEST

void DramcLoopbackTest(DRAMC_CTX_T *p)
{
  unsigned int DATLAT=0, WLAT=0, CoarseTune=0, ucStartCalVal;  
  U8 ucstatus = 0, i, j;
  U32 u4value;
  
  mcSHOW_DBG_MSG(("Loop back test start...\n"));

  #if 1
  // Disable HW gating first
  // 0x1c0[31]
  ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQSCAL0), &u4value);
  mcCLR_BIT(u4value, POS_DQSCAL0_STBCALEN);
  ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQSCAL0), u4value);
  #endif

  /* 1.enable burst mode for gating window */
  // 0x0e0[28] = 1
  ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQSCTL1), &u4value);
  mcSET_BIT(u4value, POS_DQSCTL1_DQSIENMODE);
  ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQSCTL1), u4value);

  // new gating, dual-phase DQS clock gating control enabling
  // 0x124[30] = 1
  ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQSGCTL), &u4value);
  mcSET_BIT(u4value, POS_DQSGCTL_DQSGDUALP);
  ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQSGCTL), u4value);

  ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x80), &u4value);
  if ((u4value & 0xe0)==0xe0)
  {
    // 3 pipes
    ucStartCalVal = 8;
  }
  else
  {
    // Should be 1 pipe.
    ucStartCalVal = 4;
  }
        
  for  (DATLAT = ucStartCalVal; DATLAT < DATLAT_TAP_NUMBER; DATLAT++) {
    mcSHOW_DBG_MSG(("\nDATLAT = %xh...\n", DATLAT));
    dle_factor_handler(p, DATLAT);
    ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DDR2CTL), &u4value);
    mcSHOW_DBG_MSG(("Reg.7ch=%xh\n", u4value));
    ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL4), &u4value);
    mcSHOW_DBG_MSG(("Reg.e4h=%xh\n", u4value));
    ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1), &u4value);
    mcSHOW_DBG_MSG(("Reg.f0h=%xh\n", u4value));
    ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x80), &u4value);
    mcSHOW_DBG_MSG(("Reg.80h=%xh\n", u4value));

    mcSHOW_DBG_MSG(("   |0   8  16  24  32  40  48  56  64  72  80  88  96 104 112 120\n"));
    mcSHOW_DBG_MSG(("=============================================================\n"));

    for ( i = 1; i <= DQS_GW_COARSE_END; i+=DQS_GW_COARSE_STEP) 
    {
                        if (i<10) {
                                mcSHOW_DBG_MSG((" %d |", i));
                        } else {
                                mcSHOW_DBG_MSG(("%d |", i));
                        }   
      //adjust factor steps
      dqsi_gw_dly_coarse_factor_handler(p, i, 0xff);        

      for ( j = DQS_GW_FINE_START; j <= DQS_GW_FINE_END; j+=DQS_GW_FINE_STEP) 
      {
        //adjust factor steps
        dqsi_gw_dly_fine_factor_handler(p, j);

        //0xd8[15:12] = 0xF             DQIEN fix high
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xd8), &u4value);
        u4value |= 0x0000f000;
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xd8), u4value);
            
        //0xfc[17] = 0x1                    internal loopback data path switch enable
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xfc), &u4value);
        mcSET_BIT(u4value, 17);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xfc), u4value);

        //0x104 = 0x55aaaa55         loopback test pattern1
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x104), 0x55aaaa55);
        
        //0x108 = 0xcc3333cc          loopback test pattern2
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x108), 0xcc3333cc);

        //0x10c = 0x78788787         loopback test pattern3
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x10c), 0x78788787);

        //0x1dc[26] and [3] = 0x1  turn off dynamic clock  
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x1dc), &u4value);
        mcSET_BIT(u4value, 26);
        mcSET_BIT(u4value, 3);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x1dc), u4value);
        
        //0xf8[8] = 1                           enable loopback test
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xf8), &u4value);
        mcSET_BIT(u4value, 8);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xf8), u4value);

        mcDELAY_US(20);
        
        //Result, see 0x3fc[24] = 0x0 (loopback test fail flag)
        ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x3fc), &u4value);
        if (u4value & (0x1 << 24))
        {
          // Fail
          mcSHOW_DBG_MSG(("X   "));
        }
        else
        {
          // Pass
          mcSHOW_DBG_MSG(("O   "));
        }

        //0xf8[8] = 0                           disable loopback test
        ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xf8), &u4value);
        mcCLR_BIT(u4value, 8);
        ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xf8), u4value);
        
              // Dramc software reset 0xe4[11] (dramc ao register)
        // Toggle DRAMC SW RESET.
              ucstatus |= ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0xe4), &u4value);
              mcSET_BIT(u4value, 11);
              ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xe4), u4value);
              mcCLR_BIT(u4value, 11);
              ucstatus |= ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0xe4), u4value);

        //ddrphy software reset 0xf0[28] (ddrphy register) 
        DramcPhyReset(p);

        // ddrphy chip reset 0x100000060[31:24]=0x88, 0x100000060[6]=0x1
        u4value = (*(volatile unsigned int *)(0x10000060));
        u4value &= (0x00ffffff);
        u4value |= (0x88000000);
        u4value |= (0x01 << 6);
        (*(volatile unsigned int *)(0x10000060)) = u4value;
      }
      ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQSCTL1), &u4value);
      mcSHOW_DBG_MSG(("    Reg.e0h=%xh", u4value));
      ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SELPH2), &u4value);
      mcSHOW_DBG_MSG(("    Reg.404h=%xh", u4value));
      ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SELPH5), &u4value);
      mcSHOW_DBG_MSG(("    Reg.410h=%xh", u4value));
      mcSHOW_DBG_MSG(("\n"));
    }
  }

}

#endif
