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
#include "dramc_actiming.h"

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------




//-------------------------------------------------------------------------
/** u1GetACTimingIdx()
 *  Retrieve internal ACTimingTbl's index according to dram type, freqGroup, Read DBI status
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @retval u1TimingIdx     Return ACTimingTbl entry's index
 */
//-------------------------------------------------------------------------
static U8 u1GetACTimingIdx(DRAMC_CTX_T *p)
{
    U8 u1TimingIdx = 0xff, u1TmpIdx;
    U8 u1TmpDramType = p->dram_type;

    // LP4/LP4P/LP4X use same table
    if(u1TmpDramType== TYPE_LPDDR4X || u1TmpDramType == TYPE_LPDDR4P)
        u1TmpDramType = TYPE_LPDDR4;

    for(u1TmpIdx = 0; u1TmpIdx < TOTAL_AC_TIMING_NUMBER; u1TmpIdx++)
    {
        if((ACTimingTbl[u1TmpIdx].dramType == u1TmpDramType) &&
            /* p->frequency may not be in ACTimingTable, use p->freqGroup */
            (ACTimingTbl[u1TmpIdx].freq == p->freqGroup) &&
            (ACTimingTbl[u1TmpIdx].readDBI == p->DBI_R_onoff[p->dram_fsp]) &&
            (ACTimingTbl[u1TmpIdx].DivMode == vGet_Div_Mode(p)) && // Darren for LP4 1:4 and 1:8 mode
            (ACTimingTbl[u1TmpIdx].cbtMode == vGet_Dram_CBT_Mode(p)) //LP4 byte/mixed mode dram both use byte mode ACTiming
           )
        {
            u1TimingIdx = u1TmpIdx;
            mcSHOW_DBG_MSG(("match AC timing %d\n", u1TimingIdx));
            mcDUMP_REG_MSG(("match AC timing %d\n", u1TimingIdx));
            mcFPRINTF((fp_A60501, "match AC timing %d\n", u1TimingIdx));
            break;
        }
    }

    return u1TimingIdx;
}

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

    if (vGet_Div_Mode(p) == DIV4_MODE)
    {
        if (*u1tRFC_05T_tmp != 0) // MCK - Roundup
            (*u1tRFC_tmp)++;        

        *u1tRFC_05T_tmp = 0;
    }
    else // DIV8_MODE
    {
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
    }

    return DRAM_OK;
}

//-------------------------------------------------------------------------
/** UpdateACTimingReg()
 *  ACTiming related register field update
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param  ACTbl           Pointer to correct ACTiming table struct
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL
 */
//-------------------------------------------------------------------------
DRAM_STATUS_T DdrUpdateACTimingReg(DRAMC_CTX_T *p, const ACTime_T *ACTbl)
{
    ACTime_T ACTblFinal; //ACTblFinal: Use to set correct ACTiming values and write into registers
    DRAM_ODT_MODE_T r2w_odt_onoff = p->odt_onoff; //Variable used in step 1 (decide to use odt on or off ACTiming)
    // ACTiming regs that have ODT on/off values -> declare variables to save the wanted value
    // -> Used to retrieve correct SHU_ACTIM2_TR2W value and write into final register field
    U8 u1TRTW, u1TRTW_05T, u1TMRR2W;

#ifdef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
    U8 u1RANKINCTL = 0;
#endif
    U8 RODT_TRACKING_SAVEING_MCK = 0, u1ROOT = 0, u1TXRANKINCTL = 0, u1TXDLY = 0, u1DATLAT_DSEL = 0; //Used to store tmp ACTiming values

#if SAMSUNG_LP4_NWR_WORKAROUND
    U8 u1TWTR = 0, u1TWTR_05T = 0, u1TWTR_TMP = 0;
    U8 u1MCKtoTck = 0, u105TtoTck = 0;
#endif
    // ACTiming regs that aren't currently in ACTime_T struct
    U8 u1TREFBW  = 0; //REFBW_FR (tREFBW) for LP3, REFBW_FR=0 & TREFBWIG=1 (by CF)
    U16 u2Xrtr2r=12;
	
#if XRTRTR_NEW_CROSS_RK_MODE
    U16 u2PHSINCTL = 0;
#endif

    U32 u4RankINCTL_ROOT;
    U8 u1CMDCKE = 0;

    U16 u2TXREFCNT = 0, u2tRFCabTiming = 380, u2tRFCpbTiming = 190;
    U8 u1DivMode = 0, u1tRFCab = 0, u1tRFCab_05T = 0, u1tRFCpb = 0, u1tRFCpb_05T = 0;

    if(ACTbl == NULL)
        return DRAM_FAIL;
    ACTblFinal = *ACTbl;

    // ----Step 1: Perform ACTiming table adjustments according to different usage/scenarios--------------------------
#if ENABLE_TX_WDQS
    r2w_odt_onoff = ODT_ON;
#else
    r2w_odt_onoff = p->odt_onoff;
#endif

    // ACTimings that have different values for odt on/off, retrieve the correct one and store in local variable
    if (r2w_odt_onoff == ODT_ON) //odt_on
    {
        u1TRTW = ACTblFinal.trtw_ODT_on;
        u1TRTW_05T = ACTblFinal.trtw_ODT_on_05T;
        u1TMRR2W = ACTblFinal.tmrr2w_ODT_on;

    }
    else //odt_off
    {
        u1TRTW = ACTblFinal.trtw_ODT_off;
        u1TRTW_05T = ACTblFinal.trtw_ODT_off_05T;
        u1TMRR2W = ACTblFinal.tmrr2w_ODT_off;
    }

    // Override the above tRTW & tRTW_05T selection for Hynix LPDDR4P dram (always use odt_on's value for tRTW)  
    if ((p->dram_type == TYPE_LPDDR4P) && (p->vendor_id == VENDOR_HYNIX)) //!SUPPORT_HYNIX_RX_DQS_WEAK_PULL (temp solution, need to discuss with SY)
    {
        u1TRTW = ACTblFinal.trtw_ODT_on;
        u1TRTW_05T = ACTblFinal.trtw_ODT_on_05T;
    }

#if ENABLE_RODT_TRACKING_SAVE_MCK
    // for rodt tracking save 1 MCK and rodt tracking enable or not(RODTENSTB_TRACK_EN)
    u1ODT_ON = p->odt_onoff;
    u1RODT_TRACK = ENABLE_RODT_TRACKING;
    u1ROEN = u1WDQS_ON | u1ODT_ON;
    u1ModeSel = u1RODT_TRACK & u1ROEN;
    
    // when WDQS on and RODT Track define open and un-term, RODT_TRACKING_SAVEING_MCK = 1 for the future setting
    // Maybe "Save 1 MCK" will be set after Vins_on project, but Bian_co & Vins_on can not.(different with performance team)
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

    // Update values that are used by RODT_TRACKING_SAVEING_MCK
    u1TRTW = u1TRTW - RODT_TRACKING_SAVEING_MCK;
    ACTblFinal.xrtr2w = ACTblFinal.xrtr2w - RODT_TRACKING_SAVEING_MCK;

#if SAMSUNG_LP4_NWR_WORKAROUND
    // If nWR is fixed to 30 for all freqs, tWTR@800Mhz should add 2tCK gap, allowing sufficient Samsung DRAM internal IO precharge time
    if ((p->vendor_id == VENDOR_SAMSUNG) && (p->frequency <= 800)) //LP4X, Samsung, <=DDR1600
    {
        if (vGet_Div_Mode(p) == DIV8_MODE)
        {
            u1MCKtoTck = 4;
            u105TtoTck = 2;
        }
        else
        {
            u1MCKtoTck = 2;
            u105TtoTck= 1;
        }
        
        u1TWTR_TMP = (ACTblFinal.twtr * u1MCKtoTck - ACTblFinal.twtr_05T * u105TtoTck) + 2; //Convert TWTR to tCK, and add 2tCK
        if ((u1TWTR_TMP % u1MCKtoTck) == 0) //TWTR can be transferred to TWTR directly
        {
            u1TWTR = u1TWTR_TMP/u1MCKtoTck;
            u1TWTR_05T = 0;
        }
        else //Can't be transfered to TWTR directly
        {
            u1TWTR = (u1TWTR_TMP + u105TtoTck)/u1MCKtoTck; //Add 2 tCK and set TWTR value (Then minus 2tCK using 05T)
            u1TWTR_05T = 1;  //05T means minus 2tCK
        }

        ACTblFinal.twtr = u1TWTR;
        ACTblFinal.twtr_05T = u1TWTR_05T;
    }
#endif

    //DATLAT related
#if RX_PIPE_BYPASS_EN
    u1DATLAT_DSEL = ACTblFinal.datlat - 1;
#else
    u1DATLAT_DSEL = ACTblFinal.datlat - 2;
#endif
    
#if XRTRTR_NEW_CROSS_RK_MODE
    if (p->dram_cbt_mode[RANK_0] != p->dram_cbt_mode[RANK_1])
    {
        ACTblFinal.xrtr2r ++;
    }
#else
    if(p->frequency >= 1600)
        u2Xrtr2r = 7;
    else if((p->frequency >= 800) && (vGet_Div_Mode(p) == DIV8_MODE))
        u2Xrtr2r = 6;		 
    else if((p->frequency >= 600) && (vGet_Div_Mode(p) == DIV4_MODE))
        u2Xrtr2r = 10;
    else if((p->frequency >= 400) && (vGet_Div_Mode(p) == DIV4_MODE))
        u2Xrtr2r = 8;
	
    ACTblFinal.xrtr2r = u2Xrtr2r;
#endif

#if (!XRTWTW_NEW_CROSS_RK_MODE)
    if(vGet_Div_Mode(p) == DIV8_MODE)
        ACTblFinal.xrtw2w = 5;
    else
        ACTblFinal.xrtw2w = 8;
#endif

#if TX_OE_EXTEND    
    ACTblFinal.xrtw2w += 1;
    ACTblFinal.xrtw2r += 1;
#endif

#if (!CMD_CKE_WORKAROUND_FIX)
    U8 u1Txp=0, u1Txp0p5=0;

    if(((p->frequency <= 1866) && (p->frequency >= 1600)) || ((vGet_Div_Mode(p) == DIV4_MODE) && (p->frequency == 400)))
    {
        u1Txp = 1;
    }
    else if((p->frequency == 2133) || ((vGet_Div_Mode(p) == DIV4_MODE) && ((p->frequency <= 800) || (p->frequency >= 600))))
    {
        u1Txp = 2;
    }

    if((p->frequency == 1866) || ((p->frequency <= 1333) && (p->frequency >= 1200)))
    {
        u1Txp0p5 = 1;
    }
	
    ACTblFinal.txp = u1Txp;
    ACTblFinal.txp_05T = u1Txp0p5;
    ACTblFinal.ckelckcnt = 4;
    ACTblFinal.earlyckecnt = 0;
    ACTblFinal.ckeprd -= 1;
#endif

    // tRFC formula start============================================================
    if (vGet_Div_Mode(p) == DIV8_MODE)
    {
        u1DivMode = 3;
        u1MCKtoTck = 4;
    }
    else if (vGet_Div_Mode(p) == DIV4_MODE)
    {
        u1DivMode = 2;
        u1MCKtoTck = 2;
    }
    else
    {
        mcSHOW_ERR_MSG(("tRFC Div mode is fail !!!\n"));
        #if __ETT__
        while(1);
        #endif
    }

    // tRFCab = 380ns
    UpdatetRFCMaxTiming(p, u2tRFCabTiming, u1DivMode, u1MCKtoTck, &u1tRFCab, &u1tRFCab_05T);

    // TXREFCNT
    u2TXREFCNT = ((u2tRFCabTiming*10+75)*1000)/(1000000000/(((p->frequency*2)*100) >> u1DivMode)); //tRFCab max is 380ns for 12Gb up
    if (((u2tRFCabTiming*10+75)*1000)%(1000000000/(((p->frequency*2)*100) >> u1DivMode)) != 0) // Round up
        u2TXREFCNT++;
    u2TXREFCNT = u2TXREFCNT -3 + 7; // -3 + 7 by formula

    // tRFCpb = 190ns
    UpdatetRFCMaxTiming(p, u2tRFCpbTiming, u1DivMode, u1MCKtoTck, &u1tRFCpb, &u1tRFCpb_05T);

    mcSHOW_DBG_MSG(("[DdrUpdateACTimingReg] Freq = %d, Div Mode = %d, tRFCab = %d, tRFCab_05T = %d, TXREFCNT = %d, tRFCpb = %d, tRFCpb_05T = %d\n", p->frequency, vGet_Div_Mode(p), u1tRFCab, u1tRFCab_05T, u2TXREFCNT, u1tRFCpb, u1tRFCpb_05T));
    // =========================================================== tRFC formula end
    

    // ----Step 2: Perform register writes for entries in ACTblFinal struct & ACTiming excel file (all actiming adjustments should be done in Step 1)-------
    vIO32WriteFldMulti_All(DRAMC_REG_SHU_ACTIM0, P_Fld(ACTblFinal.trcd, SHU_ACTIM0_TRCD)
                                                | P_Fld(ACTblFinal.trrd, SHU_ACTIM0_TRRD)
                                                | P_Fld(ACTblFinal.twr, SHU_ACTIM0_TWR)
                                                | P_Fld(ACTblFinal.twtr, SHU_ACTIM0_TWTR));

    vIO32WriteFldMulti_All(DRAMC_REG_SHU_ACTIM1, P_Fld(ACTblFinal.trc, SHU_ACTIM1_TRC)
                                                | P_Fld(ACTblFinal.tras, SHU_ACTIM1_TRAS)
                                                | P_Fld(ACTblFinal.trp ,SHU_ACTIM1_TRP)
                                                | P_Fld(ACTblFinal.trpab, SHU_ACTIM1_TRPAB));

    vIO32WriteFldMulti_All(DRAMC_REG_SHU_ACTIM2, P_Fld(ACTblFinal.tfaw, SHU_ACTIM2_TFAW)
                                                | P_Fld(u1TRTW, SHU_ACTIM2_TR2W) // Value has odt_on/off difference, use local variable u1TRTW
                                                | P_Fld(ACTblFinal.trtp, SHU_ACTIM2_TRTP)
                                                | P_Fld(ACTblFinal.earlyckecnt, SHU_ACTIM2_EARLYCKECNT)
                                                | P_Fld(ACTblFinal.txp, SHU_ACTIM2_TXP));

    vIO32WriteFldMulti_All(DRAMC_REG_SHU_ACTIM3, P_Fld(u1tRFCab, SHU_ACTIM3_TRFC)
                                                | P_Fld(ACTblFinal.refcnt, SHU_ACTIM3_REFCNT)
                                                | P_Fld(u1tRFCpb, SHU_ACTIM3_TRFCPB));

    vIO32WriteFldMulti_All(DRAMC_REG_SHU_ACTIM4, P_Fld(ACTblFinal.tzqcs, SHU_ACTIM4_TZQCS)
                                                | P_Fld(ACTblFinal.refcnt_fr_clk, SHU_ACTIM4_REFCNT_FR_CLK)
                                                | P_Fld(u2TXREFCNT, SHU_ACTIM4_TXREFCNT));

    vIO32WriteFldMulti_All(DRAMC_REG_SHU_ACTIM5, P_Fld(u1TMRR2W, SHU_ACTIM5_TMRR2W)  // Value has odt_on/off difference, use local variable u1TMRR2W
                                                | P_Fld(ACTblFinal.twtpd, SHU_ACTIM5_TWTPD)
                                                | P_Fld(ACTblFinal.trtpd, SHU_ACTIM5_TR2PD));

    vIO32WriteFldMulti_All(DRAMC_REG_SHU_ACTIM_XRT, P_Fld(ACTblFinal.xrtw2w, SHU_ACTIM_XRT_XRTW2W)
                                                    | P_Fld(ACTblFinal.xrtw2r, SHU_ACTIM_XRT_XRTW2R)
                                                    | P_Fld(ACTblFinal.xrtr2w, SHU_ACTIM_XRT_XRTR2W)
                                                    | P_Fld(ACTblFinal.xrtr2r, SHU_ACTIM_XRT_XRTR2R));

    // AC timing 0.5T
    vIO32WriteFldMulti_All(DRAMC_REG_SHU_AC_TIME_05T, P_Fld(ACTblFinal.twtr_05T, SHU_AC_TIME_05T_TWTR_M05T)
                                                    | P_Fld(u1TRTW_05T, SHU_AC_TIME_05T_TR2W_05T) // Value has odt_on/off difference, use local variable u1TRTW
                                                    | P_Fld(ACTblFinal.twtpd_05T, SHU_AC_TIME_05T_TWTPD_M05T)
                                                    | P_Fld(ACTblFinal.trtpd_05T, SHU_AC_TIME_05T_TR2PD_05T)
                                                    | P_Fld(ACTblFinal.tfaw_05T, SHU_AC_TIME_05T_TFAW_05T)
                                                    | P_Fld(ACTblFinal.trrd_05T, SHU_AC_TIME_05T_TRRD_05T)
                                                    | P_Fld(ACTblFinal.twr_05T, SHU_AC_TIME_05T_TWR_M05T)
                                                    | P_Fld(ACTblFinal.tras_05T, SHU_AC_TIME_05T_TRAS_05T)
                                                    | P_Fld(ACTblFinal.trpab_05T, SHU_AC_TIME_05T_TRPAB_05T)
                                                    | P_Fld(ACTblFinal.trp_05T, SHU_AC_TIME_05T_TRP_05T)
                                                    | P_Fld(ACTblFinal.trcd_05T, SHU_AC_TIME_05T_TRCD_05T)
                                                    | P_Fld(ACTblFinal.trtp_05T, SHU_AC_TIME_05T_TRTP_05T)
                                                    | P_Fld(ACTblFinal.txp_05T, SHU_AC_TIME_05T_TXP_05T)
                                                    | P_Fld(u1tRFCab_05T, SHU_AC_TIME_05T_TRFC_05T)
                                                    | P_Fld(u1tRFCpb_05T, SHU_AC_TIME_05T_TRFCPB_05T)
                                                    | P_Fld(ACTblFinal.trc_05T, SHU_AC_TIME_05T_TRC_05T));

    //[Ei_ger] DVT item RD2MRR & MRR2RD
    if(vGet_Div_Mode(p) == DIV4_MODE)
    {
        vIO32WriteFldMulti_All(DRAMC_REG_SHU_ACTIM3, P_Fld(0x8, SHU_ACTIM3_TR2MRR)|P_Fld(0x8, SHU_ACTIM3_MANTMRR)); 
    }
    else
    {
        vIO32WriteFldMulti_All(DRAMC_REG_SHU_ACTIM3, P_Fld(0x4, SHU_ACTIM3_TR2MRR)|P_Fld(0x4, SHU_ACTIM3_MANTMRR));
    }

    /* CATRAIN_INTV isn't a shuffle register, but only affects LP4 CBT timing intv. during calibration
     * Berson: CATRAIN_INTV is calculated based on CATRAINLAT = 0
     */
    vIO32WriteFldMulti_All(DRAMC_REG_CATRAINING1, P_Fld(ACTblFinal.r_dmcatrain_intv, CATRAINING1_CATRAIN_INTV)
                                                | P_Fld(0x0, CATRAINING1_CATRAINLAT));

    // DQSINCTL related
    vIO32WriteFldAlign_All(DRAMC_REG_SHURK0_DQSCTL, ACTblFinal.dqsinctl, SHURK0_DQSCTL_DQSINCTL);// Rank 0 DQSINCTL
    vIO32WriteFldAlign_All(DRAMC_REG_SHURK1_DQSCTL, ACTblFinal.dqsinctl, SHURK1_DQSCTL_R1DQSINCTL);// Rank 1 DQSINCTL
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_ODTCTRL, ACTblFinal.dqsinctl, SHU_ODTCTRL_RODT);

    if (ACTblFinal.dqsinctl >= 2)
    {
        u4RankINCTL_ROOT = ACTblFinal.dqsinctl - 2;
    }
    else
    {
        mcSHOW_ERR_MSG(("u4RankINCTL_ROOT <2, Please check\n"));
        u4RankINCTL_ROOT = 0;
    }

    vIO32WriteFldMulti_All(DRAMC_REG_SHU_RANKCTL, P_Fld(ACTblFinal.dqsinctl, SHU_RANKCTL_RANKINCTL_PHY)
                                                       | P_Fld(u4RankINCTL_ROOT, SHU_RANKCTL_RANKINCTL_ROOT1)
                                                       | P_Fld(u4RankINCTL_ROOT, SHU_RANKCTL_RANKINCTL));

#if XRTRTR_NEW_CROSS_RK_MODE
    u2PHSINCTL = (ACTblFinal.dqsinctl == 0) ? 0 : (ACTblFinal.dqsinctl - 1); 
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_RANK_SEL_STB, u2PHSINCTL, SHU_RANK_SEL_STB_RANK_SEL_PHSINCTL);
#endif

    // DATLAT related, tREFBW
    vIO32WriteFldMulti_All(DRAMC_REG_SHU_CONF1, P_Fld(ACTblFinal.datlat, SHU_CONF1_DATLAT)
                                            | P_Fld(u1DATLAT_DSEL, SHU_CONF1_DATLAT_DSEL)
                                            | P_Fld(u1DATLAT_DSEL, SHU_CONF1_DATLAT_DSEL_PHY)
                                            | P_Fld(u1TREFBW, SHU_CONF1_REFBW_FR));

    // FSPCHG_PRDCNT: LPDDR4 tFC constraint (LP3 unused, set to default value 0)
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_CONF2, ACTblFinal.r_dmfspchg_prdcnt, SHU_CONF2_FSPCHG_PRDCNT);

    //TODO: MRW_INTV can be set to different values for each freq, request new forumula/values from Berson
    // (Currently set to 0xF for both LP3/LP4 due to YH Tsai's M17 golden settings review)
    vIO32WriteFldMulti_All(DRAMC_REG_SHU_SCINTV, P_Fld(ACTblFinal.r_dmmrw_intv, SHU_SCINTV_MRW_INTV)
                                                | P_Fld(ACTblFinal.zqlat2 , SHU_SCINTV_TZQLAT2));

    // CKEPRD - CKE pulse width
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_CKECTRL, ACTblFinal.ckeprd, SHU_CKECTRL_CKEPRD);

    // @Robert for timing MP settings
    if (vGet_Div_Mode(p) == DIV4_MODE)
        u1CMDCKE = 0x4;
    else
        u1CMDCKE = 0x1;
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_CKECTRL, u1CMDCKE, SHU_CKECTRL_CMDCKE);

    //CKELCKCNT: Valid clock requirement after CKE input low
    vIO32WriteFldAlign_All(DRAMC_REG_CKECTRL, ACTblFinal.ckelckcnt, CKECTRL_CKELCKCNT);

    // ----Step 3: Perform register writes/calculation for other regs (That aren't in ACTblFinal struct)------------------------------------------------
#ifdef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
    //Wei-Jen: Ininital setting values are the same, RANKINCTL_RXDLY = RANKINCTL = RANKINCTL_ROOT1
    //XRTR2R setting will be updated in RxdqsGatingPostProcess
    u1RANKINCTL = u4IO32ReadFldAlign(DRAMC_REG_SHU_RANKCTL, SHU_RANKCTL_RANKINCTL);
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_RANKCTL, u1RANKINCTL, SHU_RANKCTL_RANKINCTL_RXDLY);
#endif

    //Update releated RG of XRTW2W (old cross ranks mode)
    if (p->frequency <= 800) 
    {
        if (vGet_Div_Mode(p) == DIV4_MODE)
        {
            u1ROOT = 0; u1TXRANKINCTL=1; u1TXDLY=2;
        }
        else
        {
            u1ROOT = 0; u1TXRANKINCTL=0; u1TXDLY=1;
        }
    }
    else
    {
        u1ROOT = (p->frequency >= 1866) ? 1 : 0; 
        u1TXRANKINCTL = (p->frequency >= 2133) ? 2 : 1; 
        u1TXDLY = (p->frequency >= 2133) ? 3 : 2; 
    }

    #if TX_OE_EXTEND    
    if (p->frequency >= 1333)
    {
        u1TXRANKINCTL += 1;
        u1TXDLY += 1;
    }
    #endif

    vIO32WriteFldMulti_All(DRAMC_REG_SHU_RANKCTL, P_Fld(u1ROOT, SHU_RANKCTL_TXRANKINCTL_ROOT)
                                                | P_Fld(u1TXRANKINCTL, SHU_RANKCTL_TXRANKINCTL)
                                                | P_Fld(u1TXDLY, SHU_RANKCTL_TXRANKINCTL_TXDLY));

#if ENABLE_TPBR2PBR_REFRESH_TIMING
    vIO32WriteFldMulti_All(DRAMC_REG_SHU_ACTIM5, P_Fld(0x1, SHU_ACTIM5_PBR2PBR_OPT)
                                                | P_Fld(ACTblFinal.tpbr2pbr, SHU_ACTIM5_TPBR2PBR)
                                                | P_Fld(ACTblFinal.tpbr2pbr_05T, SHU_ACTIM5_TPBR2PBR_05T));
#endif

    return DRAM_OK;
}


DRAM_STATUS_T DdrUpdateACTiming(DRAMC_CTX_T *p)
{
    U8 u1TimingIdx = 0;

    mcSHOW_DBG_MSG3(("[UpdateACTiming]\n"));

    //Retrieve ACTimingTable's corresponding index
    u1TimingIdx = u1GetACTimingIdx(p);

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

    //Set ACTiming registers
    DdrUpdateACTimingReg(p, &ACTimingTbl[u1TimingIdx]);

    return DRAM_OK;
}

#if ((!SW_CHANGE_FOR_SIMULATION) && (!FOR_DV_SIMULATION_USED && SW_CHANGE_FOR_SIMULATION==0) && (!__ETT__))
DRAM_STATUS_T DdrUpdateACTiming_EMI(DRAMC_CTX_T *p, AC_TIMING_EXTERNAL_T *ACRegFromEmi)
{
    U8 u1TimingIdx = 0;
    ACTime_T ACTime;
    mcSHOW_DBG_MSG3(("[DdrUpdateACTiming_EMI]\n"));

   if(ACRegFromEmi == NULL)
        return DRAM_FAIL;

    //Retrieve ACTimingTable's corresponding index
    u1TimingIdx = u1GetACTimingIdx(p);
    ACTime = ACTimingTbl[u1TimingIdx];

    //Overwrite AC timing from emi settings
    ACTime.dramType = p->dram_type;
#if 1 // Will use MDL ac timing, Others from internal ac timing
    ACTime.trp = ACRegFromEmi->AC_TIME_EMI_TRP;
    ACTime.trpab = ACRegFromEmi->AC_TIME_EMI_TRPAB;
    ACTime.trc = ACRegFromEmi->AC_TIME_EMI_TRC;
    ACTime.trcd = ACRegFromEmi->AC_TIME_EMI_TRCD;

    ACTime.trp_05T = ACRegFromEmi->AC_TIME_EMI_TRP_05T;
    ACTime.trpab_05T = ACRegFromEmi->AC_TIME_EMI_TRPAB_05T;
    ACTime.trc_05T = ACRegFromEmi->AC_TIME_EMI_TRC_05T;
    ACTime.trcd_05T = ACRegFromEmi->AC_TIME_EMI_TRCD_05T;
#else
    ACTime.freq = ACRegFromEmi->AC_TIME_EMI_FREQUENCY;
    ACTime.tras = ACRegFromEmi->AC_TIME_EMI_TRAS;
    ACTime.trp = ACRegFromEmi->AC_TIME_EMI_TRP;

    ACTime.trpab = ACRegFromEmi->AC_TIME_EMI_TRPAB;
    ACTime.trc = ACRegFromEmi->AC_TIME_EMI_TRC;
    ACTime.trfc = ACRegFromEmi->AC_TIME_EMI_TRFC;
    ACTime.trfcpb = ACRegFromEmi->AC_TIME_EMI_TRFCPB;

    ACTime.txp = ACRegFromEmi->AC_TIME_EMI_TXP;
    ACTime.trtp = ACRegFromEmi->AC_TIME_EMI_TRTP;
    ACTime.trcd = ACRegFromEmi->AC_TIME_EMI_TRCD;
    ACTime.twr = ACRegFromEmi->AC_TIME_EMI_TWR;

    ACTime.twtr = ACRegFromEmi->AC_TIME_EMI_TWTR;
    ACTime.trrd = ACRegFromEmi->AC_TIME_EMI_TRRD;
    ACTime.tfaw = ACRegFromEmi->AC_TIME_EMI_TFAW;
    ACTime.trtw_ODT_off = ACRegFromEmi->AC_TIME_EMI_TRTW_ODT_OFF;
    ACTime.trtw_ODT_on = ACRegFromEmi->AC_TIME_EMI_TRTW_ODT_ON;

    ACTime.refcnt = ACRegFromEmi->AC_TIME_EMI_REFCNT; //(REFFRERUN = 0)
    ACTime.refcnt_fr_clk = ACRegFromEmi->AC_TIME_EMI_REFCNT_FR_CLK; //(REFFRERUN = 1)
    ACTime.txrefcnt = ACRegFromEmi->AC_TIME_EMI_TXREFCNT;
    ACTime.tzqcs = ACRegFromEmi->AC_TIME_EMI_TZQCS;

    ACTime.trtpd = ACRegFromEmi->AC_TIME_EMI_TRTPD;
    ACTime.twtpd = ACRegFromEmi->AC_TIME_EMI_TWTPD;
    ACTime.tmrr2w_ODT_off = ACRegFromEmi->AC_TIME_EMI_TMRR2W_ODT_OFF;
    ACTime.tmrr2w_ODT_on = ACRegFromEmi->AC_TIME_EMI_TMRR2W_ODT_ON;

    ACTime.tras_05T = ACRegFromEmi->AC_TIME_EMI_TRAS_05T;
    ACTime.trp_05T = ACRegFromEmi->AC_TIME_EMI_TRP_05T;
    ACTime.trpab_05T = ACRegFromEmi->AC_TIME_EMI_TRPAB_05T;
    ACTime.trc_05T = ACRegFromEmi->AC_TIME_EMI_TRC_05T;
    ACTime.trfc_05T = ACRegFromEmi->AC_TIME_EMI_TRFC_05T;
    ACTime.trfcpb_05T = ACRegFromEmi->AC_TIME_EMI_TRFCPB_05T;
    ACTime.txp_05T = ACRegFromEmi->AC_TIME_EMI_TXP_05T;
    ACTime.trtp_05T = ACRegFromEmi->AC_TIME_EMI_TRTP_05T;
    ACTime.trcd_05T = ACRegFromEmi->AC_TIME_EMI_TRCD_05T;
    ACTime.twr_05T = ACRegFromEmi->AC_TIME_EMI_TWR_05T;
    ACTime.twtr_05T = ACRegFromEmi->AC_TIME_EMI_TWTR_05T;
    ACTime.trrd_05T = ACRegFromEmi->AC_TIME_EMI_TRRD_05T;
    ACTime.tfaw_05T = ACRegFromEmi->AC_TIME_EMI_TFAW_05T;
    ACTime.trtw_ODT_off_05T = ACRegFromEmi->AC_TIME_EMI_TRTW_ODT_OFF_05T;
    ACTime.trtw_ODT_on_05T = ACRegFromEmi->AC_TIME_EMI_TRTW_ODT_ON_05T;
    ACTime.trtpd_05T = ACRegFromEmi->AC_TIME_EMI_TRTPD_05T;
    ACTime.twtpd_05T = ACRegFromEmi->AC_TIME_EMI_TWTPD_05T;
#endif

    //Set ACTiming registers
    DdrUpdateACTimingReg(p, &ACTime);

    return DRAM_OK;
}
#endif

U8 vDramcACTimingGetDatLat(DRAMC_CTX_T *p)
{
    U8 u1TimingIdx = u1GetACTimingIdx(p);
    ACTime_T ACTime = ACTimingTbl[u1TimingIdx];

    return ACTime.datlat;
}

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
    U8 u1TRFC=101, u1TRFC_05T=0, u1TRFCpb=44, u1TRFCpb_05T=0,u1TXREFCNT=118;
    typedef struct
    {   /* Bitfield sizes set to current project register field's size */
        U8 u1TRFC      : 8;
        U8 u1TRFRC_05T : 1;
        U8 u1TRFCpb      : 8;
        U8 u1TRFRCpb_05T : 1;
        U16 u2TXREFCNT : 10;
    } optimizeACTime;
    /* JESD209-4B: tRFCab has 4 settings for 7 density settings (130, 180, 280, 380)
     * tRFCAB_NUM: Used to indicate tRFCab group (since some densities share the same tRFCab)
     */
    enum tRFCABIdx{tRFCAB_130 = 0, tRFCAB_180, tRFCAB_280, tRFCAB_380, tRFCAB_NUM};
    enum ACTimeIdx{GRP_DDR1600_ACTIM = 0, GRP_DDR2400_ACTIM, GRP_DDR2667_ACTIM, GRP_DDR3200_ACTIM, GRP_DDR3733_ACTIM, GRP_DDR4266_ACTIM, GRP_ACTIM_NUM};
    enum ACTimeIdxDiv4{GRP_DDR800_DIV4_ACTIM = 0, GRP_DDR1200_DIV4_ACTIM, GRP_DDR1600_DIV4_ACTIM, GRP_ACTIM_NUM_DIV4};
    /* Values retrieved from 1. Alaska ACTiming excel file 2. JESD209-4B Refresh requirement table */

    optimizeACTime *ptRFCab_Opt;

    optimizeACTime tRFCab_Opt [GRP_ACTIM_NUM][tRFCAB_NUM] =
    {
        //For freqGroup DDR1600
        {{.u1TRFC = 14, .u1TRFRC_05T = 0, .u1TRFCpb = 0, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 32}, //tRFCab = 130, tRFCpb = 60
         {.u1TRFC = 24, .u1TRFRC_05T = 0, .u1TRFCpb = 6, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 42}, //tRFCab = 180, tRFCpb = 90
         {.u1TRFC = 44, .u1TRFRC_05T = 0, .u1TRFCpb = 16, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 62}, //tRFCab = 280, tRFCpb = 140
         {.u1TRFC = 64, .u1TRFRC_05T = 0, .u1TRFCpb = 26, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 82}},//tRFCab = 380, tRFCpb = 190
        //For freqGroup DDR2400
        {{.u1TRFC = 27, .u1TRFRC_05T = 0,  .u1TRFCpb = 6, .u1TRFRCpb_05T = 1, .u2TXREFCNT = 46},   //tRFCab = 130, tRFCpb = 60
         {.u1TRFC = 42, .u1TRFRC_05T = 0,  .u1TRFCpb = 15, .u1TRFRCpb_05T = 1, .u2TXREFCNT = 61},   //tRFCab = 180, tRFCpb = 90
         {.u1TRFC = 72, .u1TRFRC_05T = 0,  .u1TRFCpb = 30, .u1TRFRCpb_05T = 1, .u2TXREFCNT = 91},   //tRFCab = 280, tRFCpb = 140
         {.u1TRFC = 102, .u1TRFRC_05T = 1,  .u1TRFCpb = 45, .u1TRFRCpb_05T = 1, .u2TXREFCNT = 121}},//tRFCab = 380, tRFCpb = 190
        //For freqGroup DDR2667
        {{.u1TRFC = 31, .u1TRFRC_05T = 1,  .u1TRFCpb = 8, .u1TRFRCpb_05T = 1, .u2TXREFCNT = 50},    //tRFCab = 130, tRFCpb = 60
         {.u1TRFC = 48, .u1TRFRC_05T = 0,  .u1TRFCpb = 18, .u1TRFRCpb_05T = 1, .u2TXREFCNT = 67},    //tRFCab = 180, tRFCpb = 90
         {.u1TRFC = 81, .u1TRFRC_05T = 1,  .u1TRFCpb = 35, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 100},   //tRFCab = 280, tRFCpb = 140
         {.u1TRFC = 115, .u1TRFRC_05T = 0,  .u1TRFCpb = 51, .u1TRFRCpb_05T = 1, .u2TXREFCNT = 134}}, //tRFCab = 380, tRFCpb = 190
        //For freqGroup DDR3200
       {{.u1TRFC = 40, .u1TRFRC_05T = 0,  .u1TRFCpb = 12, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 59},    //tRFCab = 130, tRFCpb = 60
         {.u1TRFC = 60, .u1TRFRC_05T = 0,  .u1TRFCpb = 24, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 79},    //tRFCab = 180, tRFCpb = 90
         {.u1TRFC = 100, .u1TRFRC_05T = 0, .u1TRFCpb = 44, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 119},   //tRFCab = 280, tRFCpb = 140
         {.u1TRFC = 140, .u1TRFRC_05T = 0, .u1TRFCpb = 64, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 159}}, //tRFCab = 380, tRFCpb = 190
        //For freqGroup DDR3733
        {{.u1TRFC = 48, .u1TRFRC_05T = 1, .u1TRFCpb = 16, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 68},    //tRFCab = 130, tRFCpb = 60
         {.u1TRFC = 72, .u1TRFRC_05T = 0, .u1TRFCpb = 30, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 92},    //tRFCab = 180, tRFCpb = 90
         {.u1TRFC = 118, .u1TRFRC_05T = 1, .u1TRFCpb = 53, .u1TRFRCpb_05T = 1, .u2TXREFCNT = 138},  //tRFCab = 280, tRFCpb = 140
         {.u1TRFC = 165, .u1TRFRC_05T = 1, .u1TRFCpb = 77, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 185}}, //tRFCab = 380, tRFCpb = 190
        //For freqGroup DDR4266
        {{.u1TRFC = 57, .u1TRFRC_05T = 1, .u1TRFCpb = 20, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 78},    //tRFCab = 130, tRFCpb = 60
         {.u1TRFC = 84, .u1TRFRC_05T = 0, .u1TRFCpb = 36, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 104},    //tRFCab = 180, tRFCpb = 90
         {.u1TRFC = 137, .u1TRFRC_05T = 1, .u1TRFCpb = 63, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 158},  //tRFCab = 280, tRFCpb = 140
         {.u1TRFC = 191, .u1TRFRC_05T = 0, .u1TRFCpb = 89, .u1TRFRCpb_05T = 1, .u2TXREFCNT = 211}} //tRFCab = 380, tRFCpb = 190
    };

    optimizeACTime tRFCab_Opt_Div4 [GRP_ACTIM_NUM_DIV4][tRFCAB_NUM] =
    {
        //NOTE: @Darren, For freqGroup DDR816
        {{.u1TRFC = 15, .u1TRFRC_05T = 0, .u1TRFCpb = 1, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 33}, //tRFCab = 130, tRFCpb = 60
         {.u1TRFC = 25, .u1TRFRC_05T = 0, .u1TRFCpb = 7, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 43}, //tRFCab = 180, tRFCpb = 90
         {.u1TRFC = 46, .u1TRFRC_05T = 0, .u1TRFCpb = 17, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 63}, //tRFCab = 280, tRFCpb = 140
         {.u1TRFC = 66, .u1TRFRC_05T = 0, .u1TRFCpb = 27, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 84}},//tRFCab = 380, tRFCpb = 190
        //For freqGroup DDR1200
        {{.u1TRFC = 28, .u1TRFRC_05T = 0,  .u1TRFCpb = 7, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 46},   //tRFCab = 130, tRFCpb = 60
         {.u1TRFC = 43, .u1TRFRC_05T = 0,  .u1TRFCpb = 16, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 61},   //tRFCab = 180, tRFCpb = 90
         {.u1TRFC = 73, .u1TRFRC_05T = 0,  .u1TRFCpb = 31, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 91},   //tRFCab = 280, tRFCpb = 140
         {.u1TRFC = 103, .u1TRFRC_05T = 0,  .u1TRFCpb = 46, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 121}},//tRFCab = 380, tRFCpb = 190
        //For freqGroup DDR1600
       {{.u1TRFC = 40, .u1TRFRC_05T = 0,  .u1TRFCpb = 12, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 59},    //tRFCab = 130, tRFCpb = 60
         {.u1TRFC = 60, .u1TRFRC_05T = 0,  .u1TRFCpb = 24, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 79},    //tRFCab = 180, tRFCpb = 90
         {.u1TRFC = 100, .u1TRFRC_05T = 0, .u1TRFCpb = 44, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 119},   //tRFCab = 280, tRFCpb = 140
         {.u1TRFC = 140, .u1TRFRC_05T = 0, .u1TRFCpb = 64, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 159}}, //tRFCab = 380, tRFCpb = 190
    };


    mcSHOW_DBG_MSG(("[ACTimingOptimize]"));

#if __ETT__
    if (p->density==0xff)
    {
        mcSHOW_ERR_MSG(("Error : No call MR8 to get density!!\n"));
        while(1);
    }
#endif

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
        case 400:
            if (vGet_Div_Mode(p) == DIV4_MODE)
                u1FreqGrpIdx = GRP_DDR800_DIV4_ACTIM;
            else
            {
                u1ExecuteOptimize = DISABLE;
                mcSHOW_ERR_MSG(("freqGroup err!\n"));
                #if __ETT__
                while(1);
                #endif
            }
            break;
        case 600:
            if (vGet_Div_Mode(p) == DIV4_MODE)
                u1FreqGrpIdx = GRP_DDR1200_DIV4_ACTIM;
            else
            {
                u1ExecuteOptimize = DISABLE;
                mcSHOW_ERR_MSG(("freqGroup err!\n"));
                #if __ETT__
                while(1);
                #endif
            }
            break;
        case 800:
            if (vGet_Div_Mode(p) == DIV4_MODE)
                u1FreqGrpIdx = GRP_DDR1600_DIV4_ACTIM;
            else
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
        case 2133:
            u1FreqGrpIdx = GRP_DDR4266_ACTIM;
            break;
        default:
            u1ExecuteOptimize = DISABLE;
            mcSHOW_ERR_MSG(("freqGroup err!\n"));
            #if __ETT__
            while(1);
            #endif
    }

    if (vGet_Div_Mode(p) == DIV4_MODE && u1FreqGrpIdx >= GRP_ACTIM_NUM_DIV4)
    {
            u1ExecuteOptimize = DISABLE;
            mcSHOW_ERR_MSG(("freqGroup err!\n"));
            #if __ETT__
            while(1);
            #endif
    }
    if (vGet_Div_Mode(p) == DIV4_MODE && u1FreqGrpIdx < GRP_ACTIM_NUM_DIV4)
        ptRFCab_Opt = &tRFCab_Opt_Div4[u1FreqGrpIdx][0];
    else
        ptRFCab_Opt = &tRFCab_Opt[u1FreqGrpIdx][0];

    u1TRFC = ptRFCab_Opt[u1RFCabGrpIdx].u1TRFC;
    u1TRFC_05T = ptRFCab_Opt[u1RFCabGrpIdx].u1TRFRC_05T;
    u1TRFCpb = ptRFCab_Opt[u1RFCabGrpIdx].u1TRFCpb;
    u1TRFCpb_05T = ptRFCab_Opt[u1RFCabGrpIdx].u1TRFRCpb_05T;
    u1TXREFCNT = ptRFCab_Opt[u1RFCabGrpIdx].u2TXREFCNT;

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


/* ACTimingTbl: All freq's ACTiming from ACTiming excel file
 * (Some fields don't exist for LP3 -> set to 0)
 * Note: !!All ACTiming adjustments should not be set in-table should be moved into UpdateACTimingReg()!!
 *       Or else preloader's highest freq ACTimings may be set to different values than expected.
 */
const ACTime_T ACTimingTbl[TOTAL_AC_TIMING_NUMBER] = 
{
    //----------LPDDR4---------------------------
#if SUPPORT_LP4_DDR4266_ACTIM
    //LP4_DDR4266 ACTiming---------------------------------
#if (ENABLE_READ_DBI == 1)
    //AC_TIME_LP4_BYTE_DDR4266_DBI_ON
    // LP4-4266, 2133MHz, RDBI_ON, byte mode
    {
        .dramType = TYPE_LPDDR4, .freq = 2133, .cbtMode = CBT_BYTE_MODE1, .readDBI = DBI_ON,
        .readLat = 44,	.writeLat = 18,
        .DivMode = DIV8_MODE,
        
        .tras = 14, .tras_05T = 0,
        .trp = 8,	.trp_05T = 0,
        .trpab = 1, .trpab_05T = 1,
        .trc = 25,	.trc_05T = 0,
        .trfc = 137,	.trfc_05T = 1,
        .trfcpb = 63,	.trfcpb_05T = 0,
        .txp = 0,	.txp_05T = 0,
        .trtp = 3,	.trtp_05T = 1,
        .trcd = 9,	.trcd_05T = 0,
        .twr = 16,	.twr_05T = 0,
        .twtr = 11,	.twtr_05T = 1,
        .tpbr2pbr = 41,	.tpbr2pbr_05T = 0,
        .trrd = 3,	.trrd_05T = 1,
        .tfaw = 12, .tfaw_05T = 1,
        .trtw_ODT_off = 8,	.trtw_ODT_off_05T = 0,
        .trtw_ODT_on = 11,	.trtw_ODT_on_05T = 0,
        .refcnt = 130,
        .refcnt_fr_clk = 101,
        .txrefcnt = 158,
        .tzqcs = 46,
        .xrtw2w = 5,
        .xrtw2r = 1,
        .xrtr2w = 9,
        .xrtr2r = 3,
        .r_dmcatrain_intv = 14,
        .r_dmmrw_intv = 0xf, //Berson: LP3/4 both use this field -> Formula may change, set to 0xF for now
        .r_dmfspchg_prdcnt = 134,
        .trtpd = 15,	.trtpd_05T = 0,
        .twtpd = 16,	.twtpd_05T = 0,
        .tmrr2w_ODT_off = 12,
        .tmrr2w_ODT_on = 14,
        .ckeprd = 4,
        .ckelckcnt = 3,
        .earlyckecnt = 2,
        .zqlat2 = 16,
        
        //DQSINCTL, DATLAT aren't in ACTiming excel file
        .dqsinctl = 9,
        #if RX_PIPE_BYPASS_EN
        .datlat = 18
        #else
        .datlat = 19
        #endif
    },
        
    //AC_TIME_LP4_NORM_DDR4266_DBI_ON
    // LP4-4266, 2133MHz, RDBI_ON, normal mode
    {
        .dramType = TYPE_LPDDR4, .freq = 2133, .cbtMode = CBT_NORMAL_MODE, .readDBI = DBI_ON,
        .readLat = 40,	.writeLat = 18,
        .DivMode = DIV8_MODE,
        
        .tras = 14, .tras_05T = 0,
        .trp = 8,	.trp_05T = 0,
        .trpab = 1, .trpab_05T = 1,
        .trc = 25,	.trc_05T = 0,
        .trfc = 137,	.trfc_05T = 1,
        .trfcpb = 63,	.trfcpb_05T = 1,
        .txp = 0,	.txp_05T = 0,
        .trtp = 3,	.trtp_05T = 1,
        .trcd = 9,	.trcd_05T = 0,
        .twr = 15,	.twr_05T = 0,
        .twtr = 10,	.twtr_05T = 1,
        .tpbr2pbr = 41,	.tpbr2pbr_05T = 0,
        .trrd = 3,	.trrd_05T = 1,
        .tfaw = 12, .tfaw_05T = 1,
        .trtw_ODT_off = 7,	.trtw_ODT_off_05T = 0,
        .trtw_ODT_on = 10,	.trtw_ODT_on_05T = 0,
        .refcnt = 130,
        .refcnt_fr_clk = 101,
        .txrefcnt = 158,
        .tzqcs = 46,
        .xrtw2w = 5,
        .xrtw2r = 1,
        .xrtr2w = 8,
        .xrtr2r = 3,
        .r_dmcatrain_intv = 14,
        .r_dmmrw_intv = 0xf, //Berson: LP3/4 both use this field -> Formula may change, set to 0xF for now
        .r_dmfspchg_prdcnt = 134,
        .trtpd = 14,	.trtpd_05T = 0,
        .twtpd = 15,	.twtpd_05T = 0,
        .tmrr2w_ODT_off = 11,
        .tmrr2w_ODT_on = 13,
        .ckeprd = 4,
        .ckelckcnt = 3,
        .earlyckecnt = 2,
        .zqlat2 = 16,
        
        //DQSINCTL, DATLAT aren't in ACTiming excel file
        .dqsinctl = 8,
        #if RX_PIPE_BYPASS_EN
        .datlat = 17
        #else
        .datlat = 18
        #endif
    },
#else //ENABLE_READ_DBI == 0)
    //AC_TIME_LP4_BYTE_DDR4266_DBI_OFF
    // LP4-4266, 2133MHz, RDBI_OFF, byte mode
    {
        .dramType = TYPE_LPDDR4, .freq = 2133, .cbtMode = CBT_BYTE_MODE1, .readDBI = DBI_OFF,
        .readLat = 40,	.writeLat = 18,
        .DivMode = DIV8_MODE,
        
        .tras = 14, .tras_05T = 0,
        .trp = 8,	.trp_05T = 0,
        .trpab = 1, .trpab_05T = 1,
        .trc = 25,	.trc_05T = 0,
        .trfc = 137,	.trfc_05T = 1,
        .trfcpb = 63,	.trfcpb_05T = 0,
        .txp = 0,	.txp_05T = 0,
        .trtp = 3,	.trtp_05T = 1,
        .trcd = 9,	.trcd_05T = 0,
        .twr = 16,	.twr_05T = 0,
        .twtr = 11,	.twtr_05T = 1,
        .tpbr2pbr = 41,	.tpbr2pbr_05T = 0,
        .trrd = 3,	.trrd_05T = 1,
        .tfaw = 12, .tfaw_05T = 1,
        .trtw_ODT_off = 7,	.trtw_ODT_off_05T = 0,
        .trtw_ODT_on = 10,	.trtw_ODT_on_05T = 0,
        .refcnt = 130,
        .refcnt_fr_clk = 101,
        .txrefcnt = 158,
        .tzqcs = 46,
        .xrtw2w = 5,
        .xrtw2r = 1,
        .xrtr2w = 8,
        .xrtr2r = 3,
        .r_dmcatrain_intv = 14,
        .r_dmmrw_intv = 0xf, //Berson: LP3/4 both use this field -> Formula may change, set to 0xF for now
        .r_dmfspchg_prdcnt = 134,
        .trtpd = 14,	.trtpd_05T = 0,
        .twtpd = 16,	.twtpd_05T = 0,
        .tmrr2w_ODT_off = 11,
        .tmrr2w_ODT_on = 13,
        .ckeprd = 4,
        .ckelckcnt = 3,
        .earlyckecnt = 2,
        .zqlat2 = 16,
        
        //DQSINCTL, DATLAT aren't in ACTiming excel file
        .dqsinctl = 9,	 .datlat = 18
    },
        
    //AC_TIME_LP4_NORM_DDR4266_DBI_OFF
    // LP4-4266, 2133MHz, RDBI_OFF, normal mode
    {
        .dramType = TYPE_LPDDR4, .freq = 2133, .cbtMode = CBT_NORMAL_MODE, .readDBI = DBI_OFF,
        .readLat = 36,	.writeLat = 18,
        .DivMode = DIV8_MODE,
        
        .tras = 14, .tras_05T = 0,
        .trp = 8,	.trp_05T = 0,
        .trpab = 1, .trpab_05T = 1,
        .trc = 25,	.trc_05T = 0,
        .trfc = 137,	.trfc_05T = 1,
        .trfcpb = 63,	.trfcpb_05T = 0,
        .txp = 0,	.txp_05T = 0,
        .trtp = 3,	.trtp_05T = 1,
        .trcd = 9,	.trcd_05T = 0,
        .twr = 15,	.twr_05T = 0,
        .twtr = 10,	.twtr_05T = 1,
        .tpbr2pbr = 41,	.tpbr2pbr_05T = 0,
        .trrd = 3,	.trrd_05T = 1,
        .tfaw = 12, .tfaw_05T = 1,
        .trtw_ODT_off = 6,	.trtw_ODT_off_05T = 0,
        .trtw_ODT_on = 9,	.trtw_ODT_on_05T = 0,
        .refcnt = 130,
        .refcnt_fr_clk = 101,
        .txrefcnt = 158,
        .tzqcs = 46,
        .xrtw2w = 5,
        .xrtw2r = 1,
        .xrtr2w = 7,
        .xrtr2r = 3,
        .r_dmcatrain_intv = 14,
        .r_dmmrw_intv = 0xf, //Berson: LP3/4 both use this field -> Formula may change, set to 0xF for now
        .r_dmfspchg_prdcnt = 134,
        .trtpd = 13,	.trtpd_05T = 0,
        .twtpd = 15,	.twtpd_05T = 0,
        .tmrr2w_ODT_off = 10,
        .tmrr2w_ODT_on = 12,
        .ckeprd = 4,
        .ckelckcnt = 3,
        .earlyckecnt = 2,
        .zqlat2 = 16,
        
        //DQSINCTL, DATLAT aren't in ACTiming excel file
        .dqsinctl = 8,
        #if RX_PIPE_BYPASS_EN
        .datlat = 17
        #else
        .datlat = 18
        #endif
    },
#endif //(ENABLE_READ_DBI)
#endif //SUPPORT_LP4_DDR4266_ACTIM

#if SUPPORT_LP4_DDR3733_ACTIM
    //LP4_DDR3733 ACTiming---------------------------------
#if (ENABLE_READ_DBI == 1)
    //AC_TIME_LP4_BYTE_DDR3733_DBI_ON
    // LP4-3733, 1866MHz, RDBI_ON, byte mode
    {
        .dramType = TYPE_LPDDR4, .freq = 1866, .cbtMode = CBT_BYTE_MODE1, .readDBI = DBI_ON,
        .readLat = 40,	.writeLat = 16,
        .DivMode = DIV8_MODE,

        .tras = 11,	.tras_05T = 1,
        .trp = 6,	.trp_05T = 1,
        .trpab = 1,	.trpab_05T = 1,
        .trc = 20,	.trc_05T = 1,
        .trfc = 119,	.trfc_05T = 0,
        .trfcpb = 53,	.trfcpb_05T = 1,
        .txp = 0,	.txp_05T = 0,
        .trtp = 2,	.trtp_05T = 0,
        .trcd = 7,	.trcd_05T = 1,
        .twr = 15,	.twr_05T = 1,
        .twtr = 9,	.twtr_05T = 0,
        .tpbr2pbr = 35,	.tpbr2pbr_05T = 0,
        .trrd = 3,	.trrd_05T = 0,
        .tfaw = 10,	.tfaw_05T = 0,
        .trtw_ODT_off = 8,	.trtw_ODT_off_05T = 0,
        .trtw_ODT_on = 10,	.trtw_ODT_on_05T = 0,
        .refcnt = 113,
        .refcnt_fr_clk = 101,
        .txrefcnt = 139,
        .tzqcs = 40,
        .xrtw2w = 4,
        .xrtw2r = 1,
        .xrtr2w = 9,
        .xrtr2r = 3,
        .r_dmcatrain_intv = 13,
        .r_dmmrw_intv = 0xf, //Berson: LP3/4 both use this field -> Formula may change, set to 0xF for now
        .r_dmfspchg_prdcnt = 117,
        .trtpd = 14,	.trtpd_05T = 0,
        .twtpd = 14,	.twtpd_05T = 0,
        .tmrr2w_ODT_off = 11,
        .tmrr2w_ODT_on = 13,
        .ckeprd = 4,
        .ckelckcnt = 3,
        .earlyckecnt = 2,
        .zqlat2 = 14,

        //DQSINCTL, DATLAT aren't in ACTiming excel file
        .dqsinctl = 7,
    #if RX_PIPE_BYPASS_EN
        .datlat = 17
    #else
        .datlat = 18
    #endif
    },

    //AC_TIME_LP4_NORM_DDR3733_DBI_ON
    // LP4-3733, 1866MHz, RDBI_ON, normal mode
    {
        .dramType = TYPE_LPDDR4, .freq = 1866, .cbtMode = CBT_NORMAL_MODE, .readDBI = DBI_ON,
        .readLat = 36,	.writeLat = 16,
        .DivMode = DIV8_MODE,

        .tras = 11,	.tras_05T = 1,
        .trp = 6,	.trp_05T = 1,
        .trpab = 1,	.trpab_05T = 1,
        .trc = 20,	.trc_05T = 1,
        .trfc = 119,	.trfc_05T = 0,
        .trfcpb = 53,	.trfcpb_05T = 1,
        .txp = 0,	.txp_05T = 0,
        .trtp = 2,	.trtp_05T = 0,
        .trcd = 7,	.trcd_05T = 1,
        .twr = 14,	.twr_05T = 1,
        .twtr = 8,	.twtr_05T = 0,
        .tpbr2pbr = 35,	.tpbr2pbr_05T = 0,
        .trrd = 3,	.trrd_05T = 0,
        .tfaw = 10,	.tfaw_05T = 0,
        .trtw_ODT_off = 7,	.trtw_ODT_off_05T = 0,
        .trtw_ODT_on = 9,	.trtw_ODT_on_05T = 0,
        .refcnt = 113,
        .refcnt_fr_clk = 101,
        .txrefcnt = 139,
        .tzqcs = 40,
        .xrtw2w = 4,
        .xrtw2r = 1,
        .xrtr2w = 8,
        .xrtr2r = 3,
        .r_dmcatrain_intv = 13,
        .r_dmmrw_intv = 0xf, //Berson: LP3/4 both use this field -> Formula may change, set to 0xF for now
        .r_dmfspchg_prdcnt = 117,
        .trtpd = 13,	.trtpd_05T = 0,
        .twtpd = 13,	.twtpd_05T = 0,
        .tmrr2w_ODT_off = 10,
        .tmrr2w_ODT_on = 12,
        .ckeprd = 4,
        .ckelckcnt = 3,
        .earlyckecnt = 2,
        .zqlat2 = 14,

        //DQSINCTL, DATLAT aren't in ACTiming excel file
        .dqsinctl = 6,
    #if RX_PIPE_BYPASS_EN
        .datlat = 16
    #else
        .datlat = 17
    #endif
    },
#else //ENABLE_READ_DBI == 0)
    //AC_TIME_LP4_BYTE_DDR3733_DBI_OFF
    // LP4-3733, 1866MHz, RDBI_OFF, byte mode
    {
        .dramType = TYPE_LPDDR4, .freq = 1866, .cbtMode = CBT_BYTE_MODE1, .readDBI = DBI_OFF,
        .readLat = 36,	.writeLat = 16,
        .DivMode = DIV8_MODE,

        .tras = 11,	.tras_05T = 1,
        .trp = 6,	.trp_05T = 1,
        .trpab = 1,	.trpab_05T = 1,
        .trc = 20,	.trc_05T = 1,
        .trfc = 119,	.trfc_05T = 0,
        .trfcpb = 53,	.trfcpb_05T = 1,
        .txp = 0,	.txp_05T = 0,
        .trtp = 2,	.trtp_05T = 0,
        .trcd = 7,	.trcd_05T = 1,
        .twr = 15,	.twr_05T = 1,
        .twtr = 9,	.twtr_05T = 0,
        .tpbr2pbr = 35,	.tpbr2pbr_05T = 0,
        .trrd = 3,	.trrd_05T = 0,
        .tfaw = 10,	.tfaw_05T = 0,
        .trtw_ODT_off = 7,	.trtw_ODT_off_05T = 0,
        .trtw_ODT_on = 9,	.trtw_ODT_on_05T = 0,
        .refcnt = 113,
        .refcnt_fr_clk = 101,
        .txrefcnt = 139,
        .tzqcs = 40,
        .xrtw2w = 4,
        .xrtw2r = 1,
        .xrtr2w = 8,
        .xrtr2r = 3,
        .r_dmcatrain_intv = 13,
        .r_dmmrw_intv = 0xf, //Berson: LP3/4 both use this field -> Formula may change, set to 0xF for now
        .r_dmfspchg_prdcnt = 117,
        .trtpd = 13,	.trtpd_05T = 0,
        .twtpd = 14,	.twtpd_05T = 0,
        .tmrr2w_ODT_off = 10,
        .tmrr2w_ODT_on = 12,
        .ckeprd = 4,
        .ckelckcnt = 3,
        .earlyckecnt = 2,
        .zqlat2 = 14,

        //DQSINCTL, DATLAT aren't in ACTiming excel file
        .dqsinctl = 7,	 .datlat = 17
    },

    //AC_TIME_LP4_NORM_DDR3733_DBI_OFF
    // LP4-3733, 1866MHz, RDBI_OFF, normal mode
    {
        .dramType = TYPE_LPDDR4, .freq = 1866, .cbtMode = CBT_NORMAL_MODE, .readDBI = DBI_OFF,
        .readLat = 32,	.writeLat = 16,
        .DivMode = DIV8_MODE,

        .tras = 11,	.tras_05T = 1,
        .trp = 6,	.trp_05T = 1,
        .trpab = 1,	.trpab_05T = 1,
        .trc = 20,	.trc_05T = 1,
        .trfc = 119,	.trfc_05T = 0,
        .trfcpb = 53,	.trfcpb_05T = 1,
        .txp = 0,	.txp_05T = 0,
        .trtp = 2,	.trtp_05T = 0,
        .trcd = 7,	.trcd_05T = 1,
        .twr = 14,	.twr_05T = 1,
        .twtr = 8,	.twtr_05T = 0,
        .tpbr2pbr = 35,	.tpbr2pbr_05T = 0,
        .trrd = 3,	.trrd_05T = 0,
        .tfaw = 10,	.tfaw_05T = 0,
        .trtw_ODT_off = 6,	.trtw_ODT_off_05T = 0,
        .trtw_ODT_on = 8,	.trtw_ODT_on_05T = 0,
        .refcnt = 113,
        .refcnt_fr_clk = 101,
        .txrefcnt = 139,
        .tzqcs = 40,
        .xrtw2w = 4,
        .xrtw2r = 1,
        .xrtr2w = 7,
        .xrtr2r = 3,
        .r_dmcatrain_intv = 13,
        .r_dmmrw_intv = 0xf, //Berson: LP3/4 both use this field -> Formula may change, set to 0xF for now
        .r_dmfspchg_prdcnt = 117,
        .trtpd = 12,	.trtpd_05T = 0,
        .twtpd = 13,	.twtpd_05T = 0,
        .tmrr2w_ODT_off = 9,
        .tmrr2w_ODT_on = 11,
        .ckeprd = 4,
        .ckelckcnt = 3,
        .earlyckecnt = 2,
        .zqlat2 = 14,

        //DQSINCTL, DATLAT aren't in ACTiming excel file
        .dqsinctl = 6,
    #if RX_PIPE_BYPASS_EN
        .datlat = 16
    #else
        .datlat = 17
    #endif
    },
#endif //(ENABLE_READ_DBI)
#endif //SUPPORT_LP4_DDR3733_ACTIM

#if SUPPORT_LP4_DDR3200_ACTIM
    //LP4_DDR3200 ACTiming---------------------------------
#if (ENABLE_READ_DBI == 1)
    //AC_TIME_LP4_BYTE_DDR3200_DBI_ON
    // LP4-3200, 1600MHz, RDBI_ON, byte mode
    {
        .dramType = TYPE_LPDDR4, .freq = 1600, .cbtMode = CBT_BYTE_MODE1, .readDBI = DBI_ON,
        .readLat = 36,	.writeLat = 14,
        .DivMode = DIV8_MODE,

        .tras = 8,	.tras_05T = 1,
        .trp = 5,	.trp_05T = 1,
        .trpab = 1,	.trpab_05T = 0,
        .trc = 16,	.trc_05T = 1,
        .trfc = 100,	.trfc_05T = 0,
        .trfcpb = 44,	.trfcpb_05T = 0,
        .txp = 0,	.txp_05T = 0,
        .trtp = 2,	.trtp_05T = 1,
        .trcd = 6,	.trcd_05T = 1,
        .twr = 13,	.twr_05T = 1,
        .twtr = 8,	.twtr_05T = 0,
        .tpbr2pbr = 29,	.tpbr2pbr_05T = 0,
        .trrd = 2,	.trrd_05T = 0,
        .tfaw = 7,	.tfaw_05T = 0,
        .trtw_ODT_off = 7,	.trtw_ODT_off_05T = 0,
        .trtw_ODT_on = 9,	.trtw_ODT_on_05T = 0,
        .refcnt = 97,
        .refcnt_fr_clk = 101,
        .txrefcnt = 119,
        .tzqcs = 34,
        .xrtw2w = 4,
        .xrtw2r = 1,
        .xrtr2w = 7,
        .xrtr2r = 3,
        .r_dmcatrain_intv = 11,
        .r_dmmrw_intv = 0xf, //Berson: LP3/4 both use this field -> Formula may change, set to 0xF for now
        .r_dmfspchg_prdcnt = 100,
        .trtpd = 13,	.trtpd_05T = 0,
        .twtpd = 12,	.twtpd_05T = 0,
        .tmrr2w_ODT_off = 10,
        .tmrr2w_ODT_on = 12,
        .ckeprd = 3,
        .ckelckcnt = 3,
        .earlyckecnt = 2,
        .zqlat2 = 12,

        //DQSINCTL, DATLAT aren't in ACTiming excel file
        .dqsinctl = 5,
     #if RX_PIPE_BYPASS_EN
        .datlat = 16
    #else
        .datlat = 17
    #endif
    },

    //AC_TIME_LP4_NORM_DDR3200_DBI_ON
    // LP4-3200, 1600MHz, RDBI_ON, normal mode
    {
        .dramType = TYPE_LPDDR4, .freq = 1600, .cbtMode = CBT_NORMAL_MODE, .readDBI = DBI_ON,
        .readLat = 32,	.writeLat = 14,
        .DivMode = DIV8_MODE,

        .tras = 8,	.tras_05T = 1,
        .trp = 5,	.trp_05T = 1,
        .trpab = 1,	.trpab_05T = 0,
        .trc = 16,	.trc_05T = 1,
        .trfc = 100,	.trfc_05T = 0,
        .trfcpb = 44,	.trfcpb_05T = 0,
        .txp = 0,	.txp_05T = 0,
        .trtp = 2,	.trtp_05T = 1,
        .trcd = 6,	.trcd_05T = 1,
        .twr = 12,	.twr_05T = 1,
        .twtr = 7,	.twtr_05T = 0,
        .tpbr2pbr = 29,	.tpbr2pbr_05T = 0,
        .trrd = 2,	.trrd_05T = 0,
        .tfaw = 7,	.tfaw_05T = 0,
        .trtw_ODT_off = 6,	.trtw_ODT_off_05T = 0,
        .trtw_ODT_on = 8,	.trtw_ODT_on_05T = 0,
        .refcnt = 97,
        .refcnt_fr_clk = 101,
        .txrefcnt = 119,
        .tzqcs = 34,
        .xrtw2w = 4,
        .xrtw2r = 1,
        .xrtr2w = 6,
        .xrtr2r = 3,
        .r_dmcatrain_intv = 11,
        .r_dmmrw_intv = 0xf, //Berson: LP3/4 both use this field -> Formula may change, set to 0xF for now
        .r_dmfspchg_prdcnt = 100,
        .trtpd = 12,	.trtpd_05T = 0,
        .twtpd = 12,	.twtpd_05T = 1,
        .tmrr2w_ODT_off = 9,
        .tmrr2w_ODT_on = 11,
        .ckeprd = 3,
        .ckelckcnt = 3,
        .earlyckecnt = 2,
        .zqlat2 = 12,

        //DQSINCTL, DATLAT aren't in ACTiming excel file
        .dqsinctl = 4,
    #if RX_PIPE_BYPASS_EN
        .datlat = 15
    #else
        .datlat = 16
    #endif
    },
#else //ENABLE_READ_DBI == 0)
    //AC_TIME_LP4_BYTE_DDR3200_DBI_OFF
    // LP4-3200, 1600MHz, RDBI_OFF, byte mode
    {
        .dramType = TYPE_LPDDR4, .freq = 1600, .cbtMode = CBT_BYTE_MODE1, .readDBI = DBI_OFF,
        .readLat = 32,	.writeLat = 14,
        .DivMode = DIV8_MODE,

        .tras = 8,	.tras_05T = 1,
        .trp = 5,	.trp_05T = 1,
        .trpab = 1,	.trpab_05T = 0,
        .trc = 16,	.trc_05T = 1,
        .trfc = 100,	.trfc_05T = 0,
        .trfcpb = 44,	.trfcpb_05T = 0,
        .txp = 0,	.txp_05T = 0,
        .trtp = 2,	.trtp_05T = 1,
        .trcd = 6,	.trcd_05T = 1,
        .twr = 13,	.twr_05T = 1,
        .twtr = 8,	.twtr_05T = 0,
        .tpbr2pbr = 29,	.tpbr2pbr_05T = 0,
        .trrd = 2,	.trrd_05T = 0,
        .tfaw = 7,	.tfaw_05T = 0,
        .trtw_ODT_off = 6,	.trtw_ODT_off_05T = 0,
        .trtw_ODT_on = 8,	.trtw_ODT_on_05T = 0,
        .refcnt = 97,
        .refcnt_fr_clk = 101,
        .txrefcnt = 119,
        .tzqcs = 34,
        .xrtw2w = 4,
        .xrtw2r = 1,
        .xrtr2w = 6,
        .xrtr2r = 3,
        .r_dmcatrain_intv = 11,
        .r_dmmrw_intv = 0xf, //Berson: LP3/4 both use this field -> Formula may change, set to 0xF for now
        .r_dmfspchg_prdcnt = 100,
        .trtpd = 12,	.trtpd_05T = 0,
        .twtpd = 12,	.twtpd_05T = 0,
        .tmrr2w_ODT_off = 9,
        .tmrr2w_ODT_on = 11,
        .ckeprd = 3,
        .ckelckcnt = 3,
        .earlyckecnt = 2,
        .zqlat2 = 12,

        //DQSINCTL, DATLAT aren't in ACTiming excel file
        .dqsinctl = 5,
    #if RX_PIPE_BYPASS_EN
        .datlat = 16
    #else
        .datlat = 17
    #endif
    },

    //AC_TIME_LP4_NORM_DDR3200_DBI_OFF
    // LP4-3200, 1600MHz, RDBI_OFF, normal mode
    {
        .dramType = TYPE_LPDDR4, .freq = 1600, .cbtMode = CBT_NORMAL_MODE, .readDBI = DBI_OFF,
        .readLat = 28,	.writeLat = 14,
        .DivMode = DIV8_MODE,

        .tras = 8,	.tras_05T = 1,
        .trp = 5,	.trp_05T = 1,
        .trpab = 1,	.trpab_05T = 0,
        .trc = 16,	.trc_05T = 1,
        .trfc = 100,	.trfc_05T = 0,
        .trfcpb = 44,	.trfcpb_05T = 0,
        .txp = 0,	.txp_05T = 0,
        .trtp = 2,	.trtp_05T = 1,
        .trcd = 6,	.trcd_05T = 1,
        .twr = 12,	.twr_05T = 1,
        .twtr = 7,	.twtr_05T = 0,
        .tpbr2pbr = 29,	.tpbr2pbr_05T = 0,
        .trrd = 2,	.trrd_05T = 0,
        .tfaw = 7,	.tfaw_05T = 0,
        .trtw_ODT_off = 5,	.trtw_ODT_off_05T = 0,
        .trtw_ODT_on = 7,	.trtw_ODT_on_05T = 0,
        .refcnt = 97,
        .refcnt_fr_clk = 101,
        .txrefcnt = 119,
        .tzqcs = 34,
        .xrtw2w = 4,
        .xrtw2r = 1,
        .xrtr2w = 5,
        .xrtr2r = 3,
        .r_dmcatrain_intv = 11,
        .r_dmmrw_intv = 0xf, //Berson: LP3/4 both use this field -> Formula may change, set to 0xF for now
        .r_dmfspchg_prdcnt = 100,
        .trtpd = 11,	.trtpd_05T = 0,
        .twtpd = 12,	.twtpd_05T = 1,
        .tmrr2w_ODT_off = 8,
        .tmrr2w_ODT_on = 10,
        .ckeprd = 3,
        .ckelckcnt = 3,
        .earlyckecnt = 2,
        .zqlat2 = 12,

        //DQSINCTL, DATLAT aren't in ACTiming excel file
        .dqsinctl = 4,
    #if RX_PIPE_BYPASS_EN
        .datlat = 15
    #else
        .datlat = 16
    #endif
    },
#endif //(ENABLE_READ_DBI)
#endif //SUPPORT_LP4_DDR3200_ACTIM

#if SUPPORT_LP4_DDR2667_ACTIM
    //LP4_DDR2667 ACTiming---------------------------------
#if (ENABLE_READ_DBI == 1)
    //AC_TIME_LP4_BYTE_DDR2667_DBI_ON
    // LP4-2667, 1333MHz, RDBI_ON, byte mode
    {
        .dramType = TYPE_LPDDR4, .freq = 1333, .cbtMode = CBT_BYTE_MODE1, .readDBI = DBI_ON,
        .readLat = 30,	.writeLat = 12,
        .DivMode = DIV8_MODE,

        .tras = 6,	.tras_05T = 0,
        .trp = 4,	.trp_05T = 1,
        .trpab = 1,	.trpab_05T = 0,
        .trc = 12,	.trc_05T = 1,
        .trfc = 81,	.trfc_05T = 1,
        .trfcpb = 35,	.trfcpb_05T = 0,
        .txp = 0,	.txp_05T = 0,
        .trtp = 2,	.trtp_05T = 1,
        .trcd = 5,	.trcd_05T = 1,
        .twr = 11,	.twr_05T = 1,
        .twtr = 7,	.twtr_05T = 1,
        .tpbr2pbr = 23,	.tpbr2pbr_05T = 1,
        .trrd = 1,	.trrd_05T = 1,
        .tfaw = 4,	.tfaw_05T = 1,
        .trtw_ODT_off = 6,	.trtw_ODT_off_05T = 0,
        .trtw_ODT_on = 8,	.trtw_ODT_on_05T = 0,
        .refcnt = 81,
        .refcnt_fr_clk = 101,
        .txrefcnt = 100,
        .tzqcs = 29,
        .xrtw2w = 4,
        .xrtw2r = 1,
        .xrtr2w = 6,
        .xrtr2r = 3,
        .r_dmcatrain_intv = 10,
        .r_dmmrw_intv = 0xf, //Berson: LP3/4 both use this field -> Formula may change, set to 0xF for now
        .r_dmfspchg_prdcnt = 84,
        .trtpd = 11,	.trtpd_05T = 0,
        .twtpd = 11,	.twtpd_05T = 1,
        .tmrr2w_ODT_off = 8,
        .tmrr2w_ODT_on = 10,
        .ckeprd = 3,
        .ckelckcnt = 3,
        .earlyckecnt = 2,
        .zqlat2 = 11,

        //DQSINCTL, DATLAT aren't in ACTiming excel file
        .dqsinctl = 4,	 .datlat = 15
    },

    //AC_TIME_LP4_NORM_DDR2667_DBI_ON
    // LP4-2667, 1333MHz, RDBI_ON, normal mode
    {
        .dramType = TYPE_LPDDR4, .freq = 1333, .cbtMode = CBT_NORMAL_MODE, .readDBI = DBI_ON,
        .readLat = 28,	.writeLat = 12,
        .DivMode = DIV8_MODE,

        .tras = 6,	.tras_05T = 0,
        .trp = 4,	.trp_05T = 1,
        .trpab = 1,	.trpab_05T = 0,
        .trc = 12,	.trc_05T = 1,
        .trfc = 81,	.trfc_05T = 1,
        .trfcpb = 35,	.trfcpb_05T = 0,
        .txp = 0,	.txp_05T = 0,
        .trtp = 2,	.trtp_05T = 1,
        .trcd = 5,	.trcd_05T = 1,
        .twr = 10,	.twr_05T = 0,
        .twtr = 6,	.twtr_05T = 0,
        .tpbr2pbr = 23,	.tpbr2pbr_05T = 1,
        .trrd = 1,	.trrd_05T = 1,
        .tfaw = 4,	.tfaw_05T = 1,
        .trtw_ODT_off = 5,	.trtw_ODT_off_05T = 0,
        .trtw_ODT_on = 7,	.trtw_ODT_on_05T = 0,
        .refcnt = 81,
        .refcnt_fr_clk = 101,
        .txrefcnt = 100,
        .tzqcs = 29,
        .xrtw2w = 4,
        .xrtw2r = 1,
        .xrtr2w = 6,
        .xrtr2r = 3,
        .r_dmcatrain_intv = 10,
        .r_dmmrw_intv = 0xf, //Berson: LP3/4 both use this field -> Formula may change, set to 0xF for now
        .r_dmfspchg_prdcnt = 84,
        .trtpd = 10,	.trtpd_05T = 0,
        .twtpd = 10,	.twtpd_05T = 0,
        .tmrr2w_ODT_off = 7,
        .tmrr2w_ODT_on = 9,
        .ckeprd = 3,
        .ckelckcnt = 3,
        .earlyckecnt = 2,
        .zqlat2 = 11,

        //DQSINCTL, DATLAT aren't in ACTiming excel file
        .dqsinctl = 3,	 .datlat = 13
    },
#else //ENABLE_READ_DBI == 0)
    //AC_TIME_LP4_BYTE_DDR2667_DBI_OFF
    // LP4-2667, 1333MHz, RDBI_OFF, byte mode
    {
        .dramType = TYPE_LPDDR4, .freq = 1333, .cbtMode = CBT_BYTE_MODE1, .readDBI = DBI_OFF,
        .readLat = 26,	.writeLat = 12,
        .DivMode = DIV8_MODE,

        .tras = 6,	.tras_05T = 0,
        .trp = 4,	.trp_05T = 1,
        .trpab = 1,	.trpab_05T = 0,
        .trc = 12,	.trc_05T = 1,
        .trfc = 81,	.trfc_05T = 1,
        .trfcpb = 35,	.trfcpb_05T = 0,
        .txp = 0,	.txp_05T = 0,
        .trtp = 2,	.trtp_05T = 1,
        .trcd = 5,	.trcd_05T = 1,
        .twr = 11,	.twr_05T = 1,
        .twtr = 7,	.twtr_05T = 1,
        .tpbr2pbr = 23,	.tpbr2pbr_05T = 1,
        .trrd = 1,	.trrd_05T = 1,
        .tfaw = 4,	.tfaw_05T = 1,
        .trtw_ODT_off = 5,	.trtw_ODT_off_05T = 0,
        .trtw_ODT_on = 7,	.trtw_ODT_on_05T = 0,
        .refcnt = 81,
        .refcnt_fr_clk = 101,
        .txrefcnt = 100,
        .tzqcs = 29,
        .xrtw2w = 4,
        .xrtw2r = 1,
        .xrtr2w = 5,
        .xrtr2r = 3,
        .r_dmcatrain_intv = 10,
        .r_dmmrw_intv = 0xf, //Berson: LP3/4 both use this field -> Formula may change, set to 0xF for now
        .r_dmfspchg_prdcnt = 84,
        .trtpd = 10,	.trtpd_05T = 0,
        .twtpd = 11,	.twtpd_05T = 1,
        .tmrr2w_ODT_off = 7,
        .tmrr2w_ODT_on = 9,
        .ckeprd = 3,
        .ckelckcnt = 3,
        .earlyckecnt = 2,
        .zqlat2 = 11,

        //DQSINCTL, DATLAT aren't in ACTiming excel file
        .dqsinctl = 4,	 .datlat = 16
    },

    //AC_TIME_LP4_NORM_DDR2667_DBI_OFF
    // LP4-2667, 1333MHz, RDBI_OFF, normal mode
    {
        .dramType = TYPE_LPDDR4, .freq = 1333, .cbtMode = CBT_NORMAL_MODE, .readDBI = DBI_OFF,
        .readLat = 24,	.writeLat = 12,
        .DivMode = DIV8_MODE,

        .tras = 6,	.tras_05T = 0,
        .trp = 4,	.trp_05T = 1,
        .trpab = 1,	.trpab_05T = 0,
        .trc = 12,	.trc_05T = 1,
        .trfc = 81,	.trfc_05T = 1,
        .trfcpb = 35,	.trfcpb_05T = 0,
        .txp = 0,	.txp_05T = 0,
        .trtp = 2,	.trtp_05T = 1,
        .trcd = 5,	.trcd_05T = 1,
        .twr = 10,	.twr_05T = 0,
        .twtr = 6,	.twtr_05T = 0,
        .tpbr2pbr = 23,	.tpbr2pbr_05T = 1,
        .trrd = 1,	.trrd_05T = 1,
        .tfaw = 4,	.tfaw_05T = 1,
        .trtw_ODT_off = 4,	.trtw_ODT_off_05T = 0,
        .trtw_ODT_on = 6,	.trtw_ODT_on_05T = 0,
        .refcnt = 81,
        .refcnt_fr_clk = 101,
        .txrefcnt = 100,
        .tzqcs = 29,
        .xrtw2w = 4,
        .xrtw2r = 1,
        .xrtr2w = 5,
        .xrtr2r = 3,
        .r_dmcatrain_intv = 10,
        .r_dmmrw_intv = 0xf, //Berson: LP3/4 both use this field -> Formula may change, set to 0xF for now
        .r_dmfspchg_prdcnt = 84,
        .trtpd = 9,	.trtpd_05T = 0,
        .twtpd = 10,	.twtpd_05T = 0,
        .tmrr2w_ODT_off = 6,
        .tmrr2w_ODT_on = 8,
        .ckeprd = 3,
        .ckelckcnt = 3,
        .earlyckecnt = 2,
        .zqlat2 = 11,

        //DQSINCTL, DATLAT aren't in ACTiming excel file
        .dqsinctl = 3,	 .datlat = 15
    },
#endif //(ENABLE_READ_DBI)
#endif //SUPPORT_LP4_DDR2667_ACTIM

#if SUPPORT_LP4_DDR2400_ACTIM
    //LP4_DDR2400 ACTiming---------------------------------
    //AC_TIME_LP4_BYTE_DDR2400_DBI_OFF
    // LP4-2400, 1200MHz, RDBI_OFF, byte mode
    {
        .dramType = TYPE_LPDDR4, .freq = 1200, .cbtMode = CBT_BYTE_MODE1, .readDBI = DBI_OFF,
        .readLat = 26,	.writeLat = 12,
        .DivMode = DIV8_MODE,

        .tras = 4,	.tras_05T = 1,
        .trp = 3,	.trp_05T = 1,
        .trpab = 1,	.trpab_05T = 0,
        .trc = 10,	.trc_05T = 0,
        .trfc = 72,	.trfc_05T = 1,
        .trfcpb = 30,	.trfcpb_05T = 1,
        .txp = 0,	.txp_05T = 0,
        .trtp = 1,	.trtp_05T = 0,
        .trcd = 4,	.trcd_05T = 1,
        .twr = 10,	.twr_05T = 0,
        .twtr = 6,	.twtr_05T = 0,
        .tpbr2pbr = 20,	.tpbr2pbr_05T = 1,
        .trrd = 1,	.trrd_05T = 1,
        .tfaw = 3,	.tfaw_05T = 1,
        .trtw_ODT_off = 5,	.trtw_ODT_off_05T = 0,
        .trtw_ODT_on = 7,	.trtw_ODT_on_05T = 0,
        .refcnt = 73,
        .refcnt_fr_clk = 101,
        .txrefcnt = 91,
        .tzqcs = 26,
        .xrtw2w = 3,
        .xrtw2r = 1,
        .xrtr2w = 5,
        .xrtr2r = 3,
        .r_dmcatrain_intv = 9,
        .r_dmmrw_intv = 0xf, //Berson: LP3/4 both use this field -> Formula may change, set to 0xF for now
        .r_dmfspchg_prdcnt = 76,
        .trtpd = 10,	.trtpd_05T = 0,
        .twtpd = 10,	.twtpd_05T = 1,
        .tmrr2w_ODT_off = 7,
        .tmrr2w_ODT_on = 9,
        .ckeprd = 3,
        .ckelckcnt = 3,
        .earlyckecnt = 2,
        .zqlat2 = 10,

        //DQSINCTL, DATLAT aren't in ACTiming excel file
        .dqsinctl = 4,
    #if RX_PIPE_BYPASS_EN
        .datlat = 14
    #else
        .datlat = 15
    #endif
    },

    //AC_TIME_LP4_NORM_DDR2400_DBI_OFF
    // LP4-2400, 1200MHz, RDBI_OFF, normal mode
    {
        .dramType = TYPE_LPDDR4, .freq = 1200, .cbtMode = CBT_NORMAL_MODE, .readDBI = DBI_OFF,
        .readLat = 24,	.writeLat = 12,
        .DivMode = DIV8_MODE,

        .tras = 4,	.tras_05T = 1,
        .trp = 3,	.trp_05T = 1,
        .trpab = 1,	.trpab_05T = 0,
        .trc = 10,	.trc_05T = 0,
        .trfc = 72,	.trfc_05T = 1,
        .trfcpb = 30,	.trfcpb_05T = 1,
        .txp = 0,	.txp_05T = 0,
        .trtp = 1,	.trtp_05T = 0,
        .trcd = 4,	.trcd_05T = 1,
        .twr = 10,	.twr_05T = 1,
        .twtr = 6,	.twtr_05T = 1,
        .tpbr2pbr = 20,	.tpbr2pbr_05T = 1,
        .trrd = 1,	.trrd_05T = 1,
        .tfaw = 3,	.tfaw_05T = 1,
        .trtw_ODT_off = 4,	.trtw_ODT_off_05T = 0,
        .trtw_ODT_on = 7,	.trtw_ODT_on_05T = 0,
        .refcnt = 73,
        .refcnt_fr_clk = 101,
        .txrefcnt = 91,
        .tzqcs = 26,
        .xrtw2w = 3,
        .xrtw2r = 2,
        .xrtr2w = 5,
        .xrtr2r = 3,
        .r_dmcatrain_intv = 9,
        .r_dmmrw_intv = 0xf, //Berson: LP3/4 both use this field -> Formula may change, set to 0xF for now
        .r_dmfspchg_prdcnt = 76,
        .trtpd = 9,	.trtpd_05T = 0,
        .twtpd = 9,	.twtpd_05T = 0,
        .tmrr2w_ODT_off = 6,
        .tmrr2w_ODT_on = 8,
        .ckeprd = 3,
        .ckelckcnt = 3,
        .earlyckecnt = 2,
        .zqlat2 = 10,

        //DQSINCTL, DATLAT aren't in ACTiming excel file
        .dqsinctl = 3,
    #if RX_PIPE_BYPASS_EN
        .datlat = 13
    #else
        .datlat = 14
    #endif
    },
#endif

#if SUPPORT_LP4_DDR1600_ACTIM
    //LP4_DDR1600 ACTiming---------------------------------
    //AC_TIME_LP4_BYTE_DDR1600_DBI_OFF
    // LP4-1600, 800MHz, RDBI_OFF, byte mode
    {
        .dramType = TYPE_LPDDR4, .freq = 800, .cbtMode = CBT_BYTE_MODE1, .readDBI = DBI_OFF,
        .readLat = 16,	.writeLat = 8,
        .DivMode = DIV8_MODE,

        .tras = 0,	.tras_05T = 0,
        .trp = 2,	.trp_05T = 0,
        .trpab = 0,	.trpab_05T = 1,
        .trc = 4,	.trc_05T = 0,
        .trfc = 44,	.trfc_05T = 0,
        .trfcpb = 16,	.trfcpb_05T = 0,
        .txp = 0,	.txp_05T = 0,
        .trtp = 1,	.trtp_05T = 1,
        .trcd = 3,	.trcd_05T = 0,
        .twr = 7,	.twr_05T = 0,
        .twtr = 4,	.twtr_05T = 0,
        .tpbr2pbr = 11,	.tpbr2pbr_05T = 0,
        .trrd = 0,	.trrd_05T = 0,
        .tfaw = 0,	.tfaw_05T = 0,
        .trtw_ODT_off = 3,	.trtw_ODT_off_05T = 0,
        .trtw_ODT_on = 4,	.trtw_ODT_on_05T = 0,
        .refcnt = 48,
        .refcnt_fr_clk = 101,
        .txrefcnt = 62,
        .tzqcs = 16,
        .xrtw2w = 3,
        .xrtw2r = 2,
        .xrtr2w = 3,
        .xrtr2r = 3,
        .r_dmcatrain_intv = 8,
        .r_dmmrw_intv = 0xf, //Berson: LP3/4 both use this field -> Formula may change, set to 0xF for now
        .r_dmfspchg_prdcnt = 50,
        .trtpd = 7,	.trtpd_05T = 0,
        .twtpd = 7,	.twtpd_05T = 1,
        .tmrr2w_ODT_off = 4,
        .tmrr2w_ODT_on = 6,
        .ckeprd = 2,
        .ckelckcnt = 3,
        .earlyckecnt = 2,
        .zqlat2 = 6,

        //DQSINCTL, DATLAT aren't in ACTiming excel file
        .dqsinctl = 2,
    #if RX_PIPE_BYPASS_EN
        .datlat = 11
    #else
        .datlat = 12
    #endif
    },

    //AC_TIME_LP4_NORM_DDR1600_DBI_OFF
    // LP4-1600, 800MHz, RDBI_OFF, normal mode
    {
        .dramType = TYPE_LPDDR4, .freq = 800, .cbtMode = CBT_NORMAL_MODE, .readDBI = DBI_OFF,
        .readLat = 14,	.writeLat = 8,
        .DivMode = DIV8_MODE,

        .tras = 0,	.tras_05T = 0,
        .trp = 2,	.trp_05T = 0,
        .trpab = 0,	.trpab_05T = 1,
        .trc = 4,	.trc_05T = 0,
        .trfc = 44,	.trfc_05T = 0,
        .trfcpb = 16,	.trfcpb_05T = 0,
        .txp = 0,	.txp_05T = 0,
        .trtp = 1,	.trtp_05T = 1,
        .trcd = 3,	.trcd_05T = 0,
        .twr = 7,	.twr_05T = 1,
        .twtr = 4,	.twtr_05T = 1,
        .tpbr2pbr = 11,	.tpbr2pbr_05T = 0,
        .trrd = 0,	.trrd_05T = 0,
        .tfaw = 0,	.tfaw_05T = 0,
        .trtw_ODT_off = 2,	.trtw_ODT_off_05T = 0,
        .trtw_ODT_on = 4,	.trtw_ODT_on_05T = 0,
        .refcnt = 48,
        .refcnt_fr_clk = 101,
        .txrefcnt = 62,
        .tzqcs = 16,
        .xrtw2w = 3,
        .xrtw2r = 3,
        .xrtr2w = 3,
        .xrtr2r = 3,
        .r_dmcatrain_intv = 8,
        .r_dmmrw_intv = 0xf, //Berson: LP3/4 both use this field -> Formula may change, set to 0xF for now
        .r_dmfspchg_prdcnt = 50,
        .trtpd = 6,	.trtpd_05T = 0,
        .twtpd = 6,	.twtpd_05T = 0,
        .tmrr2w_ODT_off = 3,
        .tmrr2w_ODT_on = 5,
        .ckeprd = 2,
        .ckelckcnt = 3,
        .earlyckecnt = 2,
        .zqlat2 = 6, //LP3 doesn't use this field

        //DQSINCTL, DATLAT aren't in ACTiming excel file
        .dqsinctl = 2,
    #if RX_PIPE_BYPASS_EN
        .datlat = 10
    #else
        .datlat = 11
    #endif
    },

    //LP4_DDR1600_DIV4 ACTiming---------------------------------
    //AC_TIME_LP4_BYTE_DDR1600_DBI_OFF
    // LP4-1600, 800MHz, RDBI_OFF, byte mode (1:4 mode)
    {
        .dramType = TYPE_LPDDR4, .freq = 800, .cbtMode = CBT_BYTE_MODE1, .readDBI = DBI_OFF,
        .readLat = 16,	.writeLat =  8,
        .DivMode = DIV4_MODE,
        
        .tras = 9,	.tras_05T = 0,
        .trp = 6,	.trp_05T = 0,
        .trpab = 2,	.trpab_05T = 0,
        .trc = 17,	.trc_05T = 0,
        .trfc = 100,	.trfc_05T = 0,
        .trfcpb = 44,	.trfcpb_05T = 0,
        .txp = 0,	.txp_05T = 0,
        .trtp = 3,	.trtp_05T = 0,
        .trcd = 7,	.trcd_05T = 0,
        .twr = 16,	.twr_05T = 0,
        .twtr = 11,	.twtr_05T = 0,
        .tpbr2pbr = 29,	.tpbr2pbr_05T = 0,
        .trrd = 2,	.trrd_05T = 0,
        .tfaw = 7,	.tfaw_05T = 0,
        .trtw_ODT_off = 8,	.trtw_ODT_off_05T = 0,
        .trtw_ODT_on = 10,	.trtw_ODT_on_05T = 0,
        .refcnt = 195,
        .refcnt_fr_clk = 101,
        .txrefcnt = 119,
        .tzqcs = 34,
        .xrtw2w = 7,
        .xrtw2r = 5,
        .xrtr2w = 11,
        .xrtr2r = 7,
        .r_dmcatrain_intv = 16,
        .r_dmmrw_intv = 0xf, //Berson: LP3/4 both use this field -> Formula may change, set to 0xF for now
        .r_dmfspchg_prdcnt = 100,
        .trtpd = 16,	.trtpd_05T = 0,
        .twtpd = 16,	.twtpd_05T = 0,
        .tmrr2w_ODT_off = 11,
        .tmrr2w_ODT_on = 13,
        .ckeprd = 3,
        .ckelckcnt = 3,
        .earlyckecnt = 2,
        .zqlat2 = 12,

        //DQSINCTL, DATLAT aren't in ACTiming excel file
        .dqsinctl = 6,
    #if RX_PIPE_BYPASS_EN
        .datlat = 17
    #else
        .datlat = 18
    #endif
    },

    //AC_TIME_LP4_NORM_DDR1600_DBI_OFF
    // LP4-1600, 800MHz, RDBI_OFF, normal mode (1:4 mode)
    {
        .dramType = TYPE_LPDDR4, .freq = 800, .cbtMode = CBT_NORMAL_MODE, .readDBI = DBI_OFF,
        .readLat = 14,	.writeLat =  8,
        .DivMode = DIV4_MODE,
        
        .tras = 9,	.tras_05T = 0,
        .trp = 6,	.trp_05T = 0,
        .trpab = 2,	.trpab_05T = 0,
        .trc = 17,	.trc_05T = 0,
        .trfc = 100,	.trfc_05T = 0,
        .trfcpb = 44,	.trfcpb_05T = 0,
        .txp = 0,	.txp_05T = 0,
        .trtp = 3,	.trtp_05T = 0,
        .trcd = 7,	.trcd_05T = 0,
        .twr = 15,	.twr_05T = 0,
        .twtr = 10,	.twtr_05T = 0,
        .tpbr2pbr = 29,	.tpbr2pbr_05T = 0,
        .trrd = 2,	.trrd_05T = 0,
        .tfaw = 7,	.tfaw_05T = 0,
        .trtw_ODT_off = 7,	.trtw_ODT_off_05T = 0,
        .trtw_ODT_on = 9,	.trtw_ODT_on_05T = 0,
        .refcnt = 195,
        .refcnt_fr_clk = 101,
        .txrefcnt = 119,
        .tzqcs = 34,
        .xrtw2w = 7,
        .xrtw2r = 6,
        .xrtr2w = 10,
        .xrtr2r = 7,
        .r_dmcatrain_intv = 16,
        .r_dmmrw_intv = 0xf, //Berson: LP3/4 both use this field -> Formula may change, set to 0xF for now
        .r_dmfspchg_prdcnt = 100,
        .trtpd = 15,	.trtpd_05T = 0,
        .twtpd = 15,	.twtpd_05T = 0,
        .tmrr2w_ODT_off = 10,
        .tmrr2w_ODT_on = 12,
        .ckeprd = 3,
        .ckelckcnt = 3,
        .earlyckecnt = 2,
        .zqlat2 = 12,

        //DQSINCTL, DATLAT aren't in ACTiming excel file
        .dqsinctl = 5,
    #if RX_PIPE_BYPASS_EN
        .datlat = 16
    #else
        .datlat = 17
    #endif
    },
#endif //SUPPORT_LP4_DDR1600_ACTIM

#if SUPPORT_LP4_DDR1200_ACTIM
    //LP4_DDR1200 ACTiming---------------------------------
    //AC_TIME_LP4_BYTE_DDR1200_DBI_OFF
    // LP4-1200, 600MHz, RDBI_OFF, byte mode (1:4 mode)
    {
        .dramType = TYPE_LPDDR4, .freq = 600, .cbtMode = CBT_BYTE_MODE1, .readDBI = DBI_OFF,
        .readLat = 16,	.writeLat =  8,
        .DivMode = DIV4_MODE,

        .tras = 5,	.tras_05T = 0,
        .trp = 4,	.trp_05T = 0,
        .trpab = 2,	.trpab_05T = 0,
        .trc = 10,	.trc_05T = 0,
        .trfc = 73,	.trfc_05T = 0,
        .trfcpb = 31,	.trfcpb_05T = 0,
        .txp = 0,	.txp_05T = 0,
        .trtp = 3,	.trtp_05T = 0,
        .trcd = 5,	.trcd_05T = 0,
        .twr = 14,	.twr_05T = 0,
        .twtr = 10,	.twtr_05T = 0,
        .tpbr2pbr = 21,	.tpbr2pbr_05T = 0,
        .trrd = 2,	.trrd_05T = 0,
        .tfaw = 4,	.tfaw_05T = 0,
        .trtw_ODT_off = 8,	.trtw_ODT_off_05T = 0,
        .trtw_ODT_on = 10,	.trtw_ODT_on_05T = 0,
        .refcnt = 146,
        .refcnt_fr_clk = 101,
        .txrefcnt = 91,
        .tzqcs = 26,
        .xrtw2w = 7,
        .xrtw2r = 5,
        .xrtr2w = 11,
        .xrtr2r = 7,
        .r_dmcatrain_intv = 16,
        .r_dmmrw_intv = 0xf, //Berson: LP3/4 both use this field -> Formula may change, set to 0xF for now
        .r_dmfspchg_prdcnt = 76,
        .trtpd = 16,	.trtpd_05T = 0,
        .twtpd = 14,	.twtpd_05T = 0,
        .tmrr2w_ODT_off = 11,
        .tmrr2w_ODT_on = 13,
        .ckeprd = 3,
        .ckelckcnt = 3,
        .earlyckecnt = 2,
        .zqlat2 = 10,

        //DQSINCTL, DATLAT aren't in ACTiming excel file
        .dqsinctl = 6,
    #if RX_PIPE_BYPASS_EN
        .datlat = 16
    #else
        .datlat = 17
    #endif
    },

    //AC_TIME_LP4_NORM_DDR1200_DBI_OFF
    // LP4-1200, 600MHz, RDBI_OFF, normal mode (1:4 mode)
    {
        .dramType = TYPE_LPDDR4, .freq = 600, .cbtMode = CBT_NORMAL_MODE, .readDBI = DBI_OFF,
        .readLat = 14,	.writeLat =  8,
        .DivMode = DIV4_MODE,
        
        .tras = 5,	.tras_05T = 0,
        .trp = 4,	.trp_05T = 0,
        .trpab = 2,	.trpab_05T = 0,
        .trc = 10,	.trc_05T = 0,
        .trfc = 73,	.trfc_05T = 0,
        .trfcpb = 31,	.trfcpb_05T = 0,
        .txp = 0,	.txp_05T = 0,
        .trtp = 3,	.trtp_05T = 0,
        .trcd = 5,	.trcd_05T = 0,
        .twr = 13,	.twr_05T = 0,
        .twtr = 10,	.twtr_05T = 0,
        .tpbr2pbr = 21,	.tpbr2pbr_05T = 0,
        .trrd = 2,	.trrd_05T = 0,
        .tfaw = 4,	.tfaw_05T = 0,
        .trtw_ODT_off = 7,	.trtw_ODT_off_05T = 0,
        .trtw_ODT_on = 9,	.trtw_ODT_on_05T = 0,
        .refcnt = 146,
        .refcnt_fr_clk = 101,
        .txrefcnt = 91,
        .tzqcs = 26,
        .xrtw2w = 7,
        .xrtw2r = 6,
        .xrtr2w = 10,
        .xrtr2r = 7,
        .r_dmcatrain_intv = 16,
        .r_dmmrw_intv = 0xf, //Berson: LP3/4 both use this field -> Formula may change, set to 0xF for now
        .r_dmfspchg_prdcnt = 76,
        .trtpd = 15,	.trtpd_05T = 0,
        .twtpd = 13,	.twtpd_05T = 0,
        .tmrr2w_ODT_off = 10,
        .tmrr2w_ODT_on = 12,
        .ckeprd = 3,
        .ckelckcnt = 3,
        .earlyckecnt = 2,
        .zqlat2 = 10,

        //DQSINCTL, DATLAT aren't in ACTiming excel file
        .dqsinctl = 5,
    #if RX_PIPE_BYPASS_EN
        .datlat = 15
    #else
        .datlat = 16
    #endif
    },
#endif //SUPPORT_LP4_DDR1200_ACTIM

#if SUPPORT_LP4_DDR800_ACTIM
    //LP4_DDR800 ACTiming---------------------------------
    //AC_TIME_LP4_BYTE_DDR800_DBI_OFF
    // LP4-816, 408MHz, RDBI_OFF, byte mode (1:4 mode)
    {
        .dramType = TYPE_LPDDR4, .freq = 400, .cbtMode = CBT_BYTE_MODE1, .readDBI = DBI_OFF,
        .readLat = 16,	.writeLat =  8,
        .DivMode = DIV4_MODE,
        
        .tras = 1,	.tras_05T = 0,
        .trp = 2,	.trp_05T = 0,
        .trpab = 1,	.trpab_05T = 0,
        .trc = 4,	.trc_05T = 0,
        .trfc = 46,	.trfc_05T = 0,
        .trfcpb = 17,	.trfcpb_05T = 0,
        .txp = 0,	.txp_05T = 0,
        .trtp = 3,	.trtp_05T = 0,
        .trcd = 3,	.trcd_05T = 0,
        .twr = 12,	.twr_05T = 0,
        .twtr = 10,	.twtr_05T = 0,
        .tpbr2pbr = 11,	.tpbr2pbr_05T = 0,
        .trrd = 1,	.trrd_05T = 0,
        .tfaw = 0,	.tfaw_05T = 0,
        .trtw_ODT_off = 7,	.trtw_ODT_off_05T = 0,
        .trtw_ODT_on = 10,	.trtw_ODT_on_05T = 0,
        .refcnt = 99,
        .refcnt_fr_clk = 101,
        .txrefcnt = 63,
        .tzqcs = 17,
        .xrtw2w = 7,
        .xrtw2r = 4,
        .xrtr2w = 10,
        .xrtr2r = 6,
        .r_dmcatrain_intv = 16,
        .r_dmmrw_intv = 0xf, //Berson: LP3/4 both use this field -> Formula may change, set to 0xF for now
        .r_dmfspchg_prdcnt = 52,
        .trtpd = 16,	.trtpd_05T = 0,
        .twtpd = 12,	.twtpd_05T = 0,
        .tmrr2w_ODT_off = 11,
        .tmrr2w_ODT_on = 13,
        .ckeprd = 3,
        .ckelckcnt = 3,
        .earlyckecnt = 2,
        .zqlat2 = 7,

        //DQSINCTL, DATLAT aren't in ACTiming excel file
        .dqsinctl = 6,
    #if RX_PIPE_BYPASS_EN
        .datlat = 16
    #else
        .datlat = 17
    #endif
    },

    //AC_TIME_LP4_NORM_DDR800_DBI_OFF
    // LP4-816, 408MHz, RDBI_OFF, normal mode (1:4 mode)
    {
        .dramType = TYPE_LPDDR4, .freq = 400, .cbtMode = CBT_NORMAL_MODE, .readDBI = DBI_OFF,
        .readLat = 14,	.writeLat =  8,
        .DivMode = DIV4_MODE,
        
        .tras = 1,	.tras_05T = 0,
        .trp = 2,	.trp_05T = 0,
        .trpab = 1,	.trpab_05T = 0,
        .trc = 4,	.trc_05T = 0,
        .trfc = 46,	.trfc_05T = 0,
        .trfcpb = 17,	.trfcpb_05T = 0,
        .txp = 0,	.txp_05T = 0,
        .trtp = 3,	.trtp_05T = 0,
        .trcd = 3,	.trcd_05T = 0,
        .twr = 12,	.twr_05T = 0,
        .twtr = 10,	.twtr_05T = 0,
        .tpbr2pbr = 11,	.tpbr2pbr_05T = 0,
        .trrd = 1,	.trrd_05T = 0,
        .tfaw = 0,	.tfaw_05T = 0,
        .trtw_ODT_off = 6,	.trtw_ODT_off_05T = 0,
        .trtw_ODT_on = 9,	.trtw_ODT_on_05T = 0,
        .refcnt = 99,
        .refcnt_fr_clk = 101,
        .txrefcnt = 63,
        .tzqcs = 17,
        .xrtw2w = 7,
        .xrtw2r = 5,
        .xrtr2w = 9,
        .xrtr2r = 6,
        .r_dmcatrain_intv = 16,
        .r_dmmrw_intv = 0xf, //Berson: LP3/4 both use this field -> Formula may change, set to 0xF for now
        .r_dmfspchg_prdcnt = 52,
        .trtpd = 15,	.trtpd_05T = 0,
        .twtpd = 12,	.twtpd_05T = 0,
        .tmrr2w_ODT_off = 10,
        .tmrr2w_ODT_on = 12,
        .ckeprd = 3,
        .ckelckcnt = 3,
        .earlyckecnt = 2,
        .zqlat2 = 7,

        //DQSINCTL, DATLAT aren't in ACTiming excel file
        .dqsinctl = 5,	 .datlat = 15
    },
#endif //SUPPORT_LP4_DDR800_ACTIM
};

 
