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


#define SHUFFLE_GROUP   4               //SHU1~4

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
extern U16 gddrphyfmeter_value;

U8 _MappingFreqArray[DRAM_DFS_SRAM_MAX]={0};

U8 gDVFSCtrlSel=0;
U32 gu4Ddrphy0SPMCtrl0;
U32 gu4Ddrphy1SPMCtrl0;
U32 gu4Ddrphy0SPMCtrl2;
U32 gu4Ddrphy1SPMCtrl2;

void vSetDFSFreqSelByTable(DRAMC_CTX_T *p, DRAM_DFS_FREQUENCY_TABLE_T *pFreqTable)
{
    p->pDFSTable = pFreqTable;
    DDRPhyFreqSel(p, p->pDFSTable->freq_sel);
#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
    vSetVcoreByFreq(p);
#endif
}

void vInitMappingFreqArray(DRAMC_CTX_T *p)
{
    U8 u1ShuffleIdx = 0;
    DRAM_DFS_FREQUENCY_TABLE_T *pFreqTbl;

    pFreqTbl = gFreqTbl;

    for (u1ShuffleIdx = DRAM_DFS_SHUFFLE_1; u1ShuffleIdx < DRAM_DFS_SHUFFLE_MAX; u1ShuffleIdx++)
    {
        _MappingFreqArray[pFreqTbl[u1ShuffleIdx].shuffleIdx] = u1ShuffleIdx;
    }
}

DRAM_DFS_FREQUENCY_TABLE_T* get_FreqTbl_by_shuffleIndex(DRAMC_CTX_T *p, U8 index)
{
    U8 u1ShuffleIdx = 0;
    DRAM_DFS_FREQUENCY_TABLE_T *pFreqTbl;

    pFreqTbl = gFreqTbl;
	
    for (u1ShuffleIdx = DRAM_DFS_SHUFFLE_1; u1ShuffleIdx < DRAM_DFS_SHUFFLE_MAX; u1ShuffleIdx++)
    {
        if (pFreqTbl[u1ShuffleIdx].shuffleIdx == index)
        {
            return &pFreqTbl[u1ShuffleIdx];
        }
    }
    return NULL;
}

U8 get_shuffleIndex_by_Freq(DRAMC_CTX_T *p)
{
    U8 u1ShuffleIdx = 0;
    DRAM_DFS_FREQUENCY_TABLE_T *pFreqTbl;
    DRAM_DFS_SRAM_SHU_T eCurr_shu_level = vGet_Current_ShuLevel(p);

    pFreqTbl = gFreqTbl;

    for (u1ShuffleIdx = DRAM_DFS_SHUFFLE_1; u1ShuffleIdx < DRAM_DFS_SHUFFLE_MAX; u1ShuffleIdx++)
    {
        if (pFreqTbl[u1ShuffleIdx].shuffleIdx == eCurr_shu_level)
        {
            return _MappingFreqArray[pFreqTbl[u1ShuffleIdx].shuffleIdx];
        }
    }
    
    return 0;
}

U8 DFSDllIdleTimeRoundUp(DRAMC_CTX_T *p)
{
#if (fcFOR_CHIP_ID == fcLafite)
    U32 u4DLL_Idle = 0x0;
    U16 u2Freq = gddrphyfmeter_value>>2;
    //90*MCK/( period of 104M or 52 M) --> 90MCK need confirm with Lynx for new chip
    //MCK=DDR/8 or freq/4
    u4DLL_Idle = ((90*(100000/u2Freq)*52)/100000)+1; //DVFS_SM freq: 0: 52Mhz 1:104Mhz
    //mcSHOW_ERR_MSG(("[DFSDllIdleTimeRoundUp] DDR = %d, u1DLL_Idle = 0x%x\n", (p->frequency*2), u4DLL_Idle));
    return u4DLL_Idle;
#else
    #error No defined DFSDllIdleTimeRoundUp for your chip !!!
#endif
}

void DVFSSettings(DRAMC_CTX_T *p)
{
    U8 u1DVFS_52M_104M_SEL = 1; // DVFS_SM freq: 0: 52Mhz 1:104Mhz
    U8 u1DLL_Idle = 0x30;   //DDR800

#if (fcFOR_CHIP_ID == fcLafite)
    /* DVFS_SM LP4: dll_idle 90MCK/19.5ns, LP3: 70MCK   (DE: Lynx)
     * Below values are pre-calculated for each freq and dram type specifically
     * for 52M DVFS_SM mode (104M should multiply values by 2)
     * Since this register value is related to dram's operating freq
     *  -> Each freq must use specific pre-calculated value
     * (Couldn't use formula to calculate this value because floating point numbers are
     *  not supported via preloader)
     */
#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
    u1DLL_Idle = DFSDllIdleTimeRoundUp(p);
#else
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
        case 600:
            u1DLL_Idle = 0x20;
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
#endif

#endif
    //52M_104M_SEL
    //*((volatile unsigned int*)(0x10000104)) |= (0x1 << 14);//104M clock
#if (fcFOR_CHIP_ID == fcCannon)
    if (u1DVFS_52M_104M_SEL == 1)
        u1DLL_Idle = (u1DLL_Idle*109/52); // If DVFS_SM uses 104M mode, u1DLL_Idle value should multiply by 2
#elif (fcFOR_CHIP_ID == fcLafite)
    u1DLL_Idle = (u1DLL_Idle << u1DVFS_52M_104M_SEL); // If DVFS_SM uses 104M mode, u1DLL_Idle value should multiply by 2
#else
    #error No defined u1DLL_Idle for your chip !!!
#endif

#if __ETT__ 
    if(u1DLL_Idle > 0x7f)
    {
        mcSHOW_ERR_MSG(("u1DLL_Idle is overflow!\n"));
        while(1); 
    }
#endif

    //DVFS debug enable - MRR_STATUS2_DVFS_STATE
    vIO32WriteFldAlign_All(DRAMC_REG_DVFSDLL, 1, DVFSDLL_R_DDRPHY_SHUFFLE_DEBUG_ENABLE);

    vIO32WriteFldAlign_All(DDRPHY_DVFS_EMI_CLK, u1DVFS_52M_104M_SEL, DVFS_EMI_CLK_RG_52M_104M_SEL); //Set DVFS_SM's clk
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_DVFSCTL, u1DLL_Idle, SHU_DVFSCTL_R_DLL_IDLE);

    vIO32WriteFldAlign_All(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_SPM_DVFS_CONTROL_SEL); // DFS RG mode for calibration

#if ENABLE_DVFS_CDC_SYNCHRONIZER_OPTION
    //CDC option
    vIO32WriteFldAlign_All(DRAMC_REG_SHUCTRL2, 1, SHUCTRL2_R_DVFS_CDC_OPTION);//Lewis@20170331: Not set SHUCTRL2_R_DVFS_CDC_OPTION to 1 since it will lead DDR reserve mode fail in DDR2400 and DDR1600
    vIO32WriteFldAlign_All(DRAMC_REG_DVFSDLL, 1, DVFSDLL_R_DVFS_SYNC_MODULE_RST_SEL);
    vIO32WriteFldAlign_All(DRAMC_REG_SHUCTRL2, 1, SHUCTRL2_R_CDC_MUX_SEL_OPTION);
#endif

#if ENABLE_BLOCK_APHY_CLOCK_DFS_OPTION
    vIO32WriteFldAlign_All(DRAMC_REG_DVFSDLL, 1, DVFSDLL_R_DVFS_PICG_POSTPONE);
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL6, 1, MISC_CG_CTRL6_ARMCTL_CK_OUT_CG_SEL);
    vIO32WriteFldAlign_All(DRAMC_REG_SHUCTRL2, 1, SHUCTRL2_SHUFFLE_CHANGE_FREQ_OPT);
#endif

#if RDSEL_TRACKING_EN
    vIO32WriteFldAlign_All(DDRPHY_SRAM_DMA1, 0x3b0, SRAM_DMA1_R_SPM_RESTORE_STEP_EN);
#endif

    //Cann_on CDC options
    vIO32WriteFldMulti_All(DDRPHY_MISC_CTRL0, P_Fld(1, MISC_CTRL0_IMPCAL_CDC_ECO_OPT) | P_Fld(1, MISC_CTRL0_IDLE_DCM_CHB_CDC_ECO_OPT));    
    //DLL_SHUFFLE should be set enable before switch frequency
    vIO32WriteFldAlign_All(DDRPHY_DVFS_EMI_CLK, 1, DVFS_EMI_CLK_RG_DLL_SHUFFLE);
    vIO32WriteFldAlign_All(DRAMC_REG_DVFSDLL, 1, DVFSDLL_RG_DLL_SHUFFLE);
}

#if ENABLE_DFS_SSC_WA
void DDRSSCSetting(DRAMC_CTX_T * p)
{
    U32 u4DELTA1=0;
    
    if (p->frequency == 1866)
    {
        u4DELTA1 = 0xE14;
    }
    else if (p->frequency == 1600)
    {
        u4DELTA1 = 0xC1C;
    }
    else if (p->frequency == 1200)
    {
        u4DELTA1 = 0x90F;     
    }
    else
    {
        return;
    }

    vIO32WriteFldAlign_All(DDRPHY_SHU_PLL5, 0x1, SHU_PLL5_RG_RPHYPLL_SDM_FRA_EN);
    vIO32WriteFldAlign_All(DDRPHY_SHU_PLL7, 0x1, SHU_PLL5_RG_RPHYPLL_SDM_FRA_EN); 
    
    vIO32WriteFldAlign_All(DDRPHY_SHU_PLL14, 0x1, SHU_PLL14_RG_RPHYPLL_SDM_SSC_PH_INIT);
    vIO32WriteFldAlign_All(DDRPHY_SHU_PLL20, 0x1, SHU_PLL20_RG_RCLRPLL_SDM_SSC_PH_INIT);
    
    vIO32WriteFldAlign_All(DDRPHY_SHU_PLL14, 0x0208, SHU_PLL14_RG_RPHYPLL_SDM_SSC_PRD);
    vIO32WriteFldAlign_All(DDRPHY_SHU_PLL20, 0x0208, SHU_PLL20_RG_RCLRPLL_SDM_SSC_PRD);
    
    vIO32WriteFldAlign_All(DDRPHY_SHU_PLL15, 0x0, SHU_PLL15_RG_RPHYPLL_SDM_SSC_DELTA);
    vIO32WriteFldAlign_All(DDRPHY_SHU_PLL21, 0x0, SHU_PLL21_RG_RCLRPLL_SDM_SSC_DELTA);
    
    vIO32WriteFldAlign_All(DDRPHY_SHU_PLL15, u4DELTA1, SHU_PLL15_RG_RPHYPLL_SDM_SSC_DELTA1);
    vIO32WriteFldAlign_All(DDRPHY_SHU_PLL21, u4DELTA1, SHU_PLL21_RG_RCLRPLL_SDM_SSC_DELTA1);

    //vIO32WriteFldAlign_All(DDRPHY_PLL1, 0x1, PLL1_RG_RPHYPLL_SDM_SSC_EN);
    //vIO32WriteFldAlign_All(DDRPHY_PLL2, 0x1, PLL2_RG_RCLRPLL_SDM_SSC_EN); 
}

void DramcSSCHoppingOnOff(DRAMC_CTX_T *p, U8 cur_shu_level, U8 u1OnOff)
{
    if ((cur_shu_level == 0x0)  || (cur_shu_level == 0x8)  || (cur_shu_level == 0x9)  || (cur_shu_level == 0x6)  || (cur_shu_level == 0x5))
    {
        if (!(p->u1PLLMode == PHYPLL_MODE))
            vIO32WriteFldAlign(DDRPHY_PLL2, u1OnOff, PLL2_RG_RCLRPLL_SDM_SSC_EN); // CLRPLL SSC
        else
            vIO32WriteFldAlign(DDRPHY_PLL1, u1OnOff, PLL1_RG_RPHYPLL_SDM_SSC_EN); // PHYPLL SSC
    }
}
#endif


#if DVT_TEST_DUMMY_RD_SIDEBAND_FROM_SPM || ENABLE_DFS_SSC_WA
void DVS_DMY_RD_ENTR(DRAMC_CTX_T *p)
{
    /*TINFO="DRAM : SPM DVS DMY RD ENTR"*/

    /*TINFO="DRAM : set sc_ddrphy_fb_ck_en = 1"*/  
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_DDRPHY_FB_CK_EN);


    mcDELAY_US(1);

    /*TINFO="DRAM : set sc_dmyrd_en_mod_sel = 1"*/  
    //! diff with WE
    vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SPM_CON, 1, DRAMC_DPY_CLK_SPM_CON_SC_DMYRD_EN_MOD_SEL_PCM);

    mcDELAY_US(1);

    /*TINFO="DRAM : set sc_dmyrd_intv_sel = 1"*/  
    //! diff with WE    
    vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SPM_CON, 1, DRAMC_DPY_CLK_SPM_CON_SC_DMYRD_INTV_SEL_PCM);

    mcDELAY_US(1);

    /*TINFO="DRAM : set sc_dmyrd_en = 1"*/  
    //! diff with WE    
    vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SPM_CON, 1, DRAMC_DPY_CLK_SPM_CON_SC_DMYRD_EN_PCM);

    mcDELAY_US(1);
}

void DVS_DMY_RD_EXIT(DRAMC_CTX_T *p)
{
    /*TINFO="DRAM : SPM DVS DMY RD EXIT"*/

    /*TINFO="DRAM : set sc_dmyrd_en = 0"*/  
    //! diff with WE
    vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SPM_CON, 0, DRAMC_DPY_CLK_SPM_CON_SC_DMYRD_EN_PCM);

    mcDELAY_US(1);

    /*TINFO="DRAM : set sc_dmyrd_intv_sel = 0"*/  
    //! diff with WE
    vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SPM_CON, 0, DRAMC_DPY_CLK_SPM_CON_SC_DMYRD_INTV_SEL_PCM);

    mcDELAY_US(1);

    /*TINFO="DRAM : set sc_dmyrd_en_mod_sel = 0"*/  
    //! diff with WE
    vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SPM_CON, 0, DRAMC_DPY_CLK_SPM_CON_SC_DMYRD_EN_MOD_SEL_PCM);

    mcDELAY_US(1);

    /*TINFO="DRAM : set sc_ddrphy_fb_ck_en = 0"*/  
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DDRPHY_FB_CK_EN);   

    mcDELAY_US(1);


    /*TINFO="DRAM : SPM DVS DMY RD EXIT end "*/
}
#endif


#if 1//(FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
void EnableDramcTrackingByShuffle(DRAMC_CTX_T *p, U8 shu_level, U8 u1OnOff)
{
    if (u1OnOff == ENABLE)
    {
        if ((shu_level == SRAM_SHU0) || (shu_level == SRAM_SHU1) || (shu_level == SRAM_SHU2) /*|| ( shu_level == SRAM_SHU4) || (shu_level == SRAM_SHU6) || (shu_level == SRAM_SHU8)*/)
        {   //0.70V @DDR3200, 0.625V @DDR1200, 0.625V @DDR1600, 0.625V @DDR2400, 0.8V @DDR3200, 0.8V @DDR3733, 
            // Here can't use p->frequency instead of shu_level
            vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_DPHY_RXDLY_TRACK_EN);
        }
        //VCORE DVFS Scenario only, SUSPEND/DEEPIDLE/SODI Scenario keep sc_tx_tracking_dis = 1
        if (shu_level != SRAM_SHU9) //0.625V @DDR800 tx tacking always disable
            vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_TX_TRACKING_DIS);
        //if(sw_options_0 & RUN_COMMON_SCENARIO){
        //    clr_ctrl0(CTRL0_SC_TX_TRACKING_DIS);
        //}
    }
    else if (u1OnOff == DISABLE)
    {
        mcSHOW_DBG_MSG3(("Disable RX-Tracking\n"));
        vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DPHY_RXDLY_TRACK_EN);

        mcSHOW_DBG_MSG3(("Disable TX-Tracking\n"));
        vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_TX_TRACKING_DIS);
    }
}


void EnableDramcTrackingBySPMControl(DRAMC_CTX_T *p)
{
    if(p->frequency>=1600)
        vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_DPHY_RXDLY_TRACK_EN); 
}

void TransferToRegControl(void)
{
    /* Chen-Hsiang@20160323: After leave preloader and low power scenario, conf is controller by RG*/
}


void TransferToSPMControl(DRAMC_CTX_T *p)//Open all APHY controls from SPM path except PWR_ON && ISO (related to low power and DVFS)
{
#if ENABLE_TX_TRACKING //HW mode
    vIO32WriteFldAlign_All(DRAMC_REG_PRE_TDQSCK1, 0x0, PRE_TDQSCK1_TX_TRACKING_OPT);//Set TX_TRACKING_OPT = 0 to let spm side band control HW TX tracking
#endif
    vIO32WriteFldAlign_All(DDRPHY_MISC_SPM_CTRL0, 0xfbffefff, MISC_SPM_CTRL0_PHY_SPM_CTL0);
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL2, 0xffffffef, MISC_SPM_CTRL2_PHY_SPM_CTL2);
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL2 + SHIFT_TO_CHB_ADDR, 0x7fffffef, MISC_SPM_CTRL2_PHY_SPM_CTL2);//Lewis@20170627: Set CHB CA DLL type to slave mode 

    return;
}

//-------------------------------------------------------------------------
/** TransferPLLToSPMControl
 *  1. Enable DVFS to SPM control
 *  2. Configs SPM pinmux
 *  3. To control PLL between PHYPLL and CLRPLL via SPM
 *  4. set current SRAM SHU index for SPM mode DFS latch/restore
 */
//-------------------------------------------------------------------------
void TransferPLLToSPMControl(DRAMC_CTX_T *p)
{
    //U8 shu_level ;
    //shu_level = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHUSTATUS), SHUSTATUS_SHUFFLE_LEVEL);
    
    /*TINFO="DRAM : enter SW DVFS"*/
    //! To DFS SPM mode after calibration
    // Enable DVFS to SPM control
    vIO32WriteFldAlign_All(DDRPHY_MISC_SPM_CTRL1, 0x0, MISC_SPM_CTRL1_SPM_DVFS_CONTROL_SEL);

    /*TINFO="DRAM : SPM presetting for pinmux"*/
    //! set SPM project code and enable clock enable
    vIO32WriteFldMulti(SPM_POWERON_CONFIG_EN, P_Fld(0xB16, POWERON_CONFIG_EN_PROJECT_CODE) | P_Fld(1, POWERON_CONFIG_EN_BCLK_CG_EN)); 

    //! set SPM pinmux
    vIO32WriteFldMulti(SPM_PCM_PWR_IO_EN, P_Fld(0, PCM_PWR_IO_EN_PCM_PWR_IO_EN) | P_Fld(0, PCM_PWR_IO_EN_RG_RF_SYNC_EN)); 

    vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON_SEL, 0x00ffffff, DRAMC_DPY_CLK_SW_CON_SEL_FULL);
    vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON_SEL2, 0xffffffff, DRAMC_DPY_CLK_SW_CON_SEL2_FULL);
    vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON_SEL3, 0x000fffff, DRAMC_DPY_CLK_SW_CON_SEL3_FULL);

    //! set  sc_dpy_2nd_dll_en, sc_dpy_dll_en, sc_dpy_dll_ck_en ,sc_dpy_vref_en , sc_phypll_en = 1
    vIO32WriteFldMulti(SPM_SPM_POWER_ON_VAL0, P_Fld(1, SPM_POWER_ON_VAL0_SC_DPY_2ND_DLL_EN)
		| P_Fld(1, SPM_POWER_ON_VAL0_SC_DPY_DLL_EN) | P_Fld(1, SPM_POWER_ON_VAL0_SC_DPY_DLL_CK_EN)
		| P_Fld(1, SPM_POWER_ON_VAL0_SC_DPY_VREF_EN) | P_Fld(1, SPM_POWER_ON_VAL0_SC_PHYPLL_EN)); 

    vIO32WriteFldAlign(SPM_SPM_S1_MODE_CH, 0x0, SPM_S1_MODE_CH_SPM_S1_MODE_CH); //Joe confirm: no use keep default values

    mcSHOW_DBG_MSG(("TransferPLLToSPMControl - MODE SW "));
    //if(shu_level==1)
    if(p->u1PLLMode == PHYPLL_MODE)
    {
        mcSHOW_DBG_MSG(("PHYPLL\n"));
        vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_PHYPLL2_SHU_EN);
        vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL_SHU_EN);  // PHYPLL for part of SHU RG
        vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_PHYPLL2_MODE_SW); // same as DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL2_MODE_SW by MUX
        vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL_MODE_SW); // same as DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL_MODE_SW by MUX
        vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, 0, DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL2_MODE_SW);
        vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, 1, DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL_MODE_SW);
    }
    else
    {
        mcSHOW_DBG_MSG(("CLRPLL\n"));
        vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_PHYPLL_SHU_EN);
        vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL2_SHU_EN);  // CLRPLL for part of SHU RG
        vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_PHYPLL_MODE_SW); // same as DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL2_MODE_SW by MUX
        vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL2_MODE_SW); // same as DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL_MODE_SW by MUX
        vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, 0, DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL_MODE_SW);
        vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, 1, DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL2_MODE_SW);   
    }
    mcDELAY_US(1);
    vIO32WriteFldAlign_All(DDRPHY_PLL1, 0x0, PLL1_RG_RPHYPLL_EN);
    vIO32WriteFldAlign_All(DDRPHY_PLL2, 0x0, PLL2_RG_RCLRPLL_EN);

    //set current SRAM SHU index for SPM mode DFS latch/restore
    mcSHOW_DBG_MSG(("TransferPLLToSPMControl - Current SRAM SHU LEVEL = %d\n", vGet_Current_ShuLevel(p)));
    vIO32WriteFldMulti((SPM_DRAMC_DPY_CLK_SPM_CON), P_Fld(vGet_Current_ShuLevel(p), DRAMC_DPY_CLK_SPM_CON_SC_DR_SHU_LEVEL_SRAM_CH0_PCM)
                | P_Fld(vGet_Current_ShuLevel(p), DRAMC_DPY_CLK_SPM_CON_SC_DR_SHU_LEVEL_SRAM_CH1_PCM));

    return;
}
#endif


#if ENABLE_DVFS_BYPASS_MR13_FSP
void DFSBypassMR13HwSet(DRAMC_CTX_T *p)
{
#if (fcFOR_CHIP_ID == fcLafite)
    U8 u1ShuffleIdx, BFSP = 0, u1SramShuIdx = 0;
    REG_TRANSFER_T TransferReg;

    TransferReg.u4Addr = DRAMC_REG_BYPASS_FSPOP;
    TransferReg.u4Fld = BYPASS_FSPOP_BPFSP_SET_SHU3;
    for(u1ShuffleIdx = 0; u1ShuffleIdx<DRAM_DFS_SRAM_MAX; u1ShuffleIdx++)
    {
        u1SramShuIdx = gFreqTbl[u1ShuffleIdx].shuffleIdx;
        switch(u1SramShuIdx)
        {
            case 0:
                TransferReg.u4Fld = BYPASS_FSPOP_BPFSP_SET_SHU0;
                break;
            case 1:
                TransferReg.u4Fld = BYPASS_FSPOP_BPFSP_SET_SHU1;
                break;
            case 2:
                TransferReg.u4Fld = BYPASS_FSPOP_BPFSP_SET_SHU2;
                break;
            case 3:
                TransferReg.u4Fld = BYPASS_FSPOP_BPFSP_SET_SHU3;
                break;
            case 4:
                TransferReg.u4Fld = BYPASS_FSPOP_BPFSP_SET_SHU4;
                break;
            case 5:
                TransferReg.u4Fld = BYPASS_FSPOP_BPFSP_SET_SHU5;
                break;
            case 6:
                TransferReg.u4Fld = BYPASS_FSPOP_BPFSP_SET_SHU6;
                break;
            case 7:
                TransferReg.u4Fld = BYPASS_FSPOP_BPFSP_SET_SHU7;
                break;
            case 8:
                TransferReg.u4Fld = BYPASS_FSPOP_BPFSP_SET_SHU8;
                break;
            case 9:
                TransferReg.u4Fld = BYPASS_FSPOP_BPFSP_SET_SHU9;
                break;
            default:
                mcSHOW_ERR_MSG(("[DFSBypassMR13HwSet] fail at BPFSP_SHU%d incorrect !!!\n", u1SramShuIdx));
                break;        
        }
        BFSP = (gFreqTbl[u1ShuffleIdx].freq_sel <= LP4_DDR2667) ? 0x1 : 0x0; //0x1 (Bypass), 0x0 (Not bypass)
        //mcSHOW_DBG_MSG(("[DFSBypassMR13HwSet] BPFSP_SHU%d = 0x%x\n", u1SramShuIdx, BFSP));
        vIO32WriteFldAlign_All(TransferReg.u4Addr, BFSP, TransferReg.u4Fld);
    }
    vIO32WriteFldAlign_All(DRAMC_REG_RK0_PRE_TDQSCK15, 0x1, RK0_PRE_TDQSCK15_SHUFFLE_LEVEL_MODE_SELECT); // 1: shuffle level = 10, 0: shuffle level =4
    vIO32WriteFldAlign_All(DDRPHY_MISC_SPM_CTRL3, 0x0, MISC_SPM_CTRL3_REG_CDC_BYPASS_DBG);
    vIO32WriteFldAlign_All(DRAMC_REG_BYPASS_FSPOP, 0x1, BYPASS_FSPOP_BPFSP_OPT);
#endif
}
#endif

#if 0//DRAMC_SHU_OLD_MODE
#define SHUFFLE_ADDR_NUM_DRAMC          4
#define SHUFFLE_ADDR_NUM_DDRPHY         14

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
                    U32 shuffleAddr = normalAddr + SHU_GRP_DRAMC_OFFSET * j;
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
                    U32 shuffleAddr = normalAddr + SHU_GRP_DDRPHY_OFFSET * j;
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
                U32 srcAddr = normalAddr + SHU_GRP_DRAMC_OFFSET * srcRG;
                U32 shuffleAddr = normalAddr + SHU_GRP_DRAMC_OFFSET * dstRG;
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
                U32 srcAddr = normalAddr + SHU_GRP_DDRPHY_OFFSET * srcRG;
                U32 shuffleAddr = normalAddr + SHU_GRP_DDRPHY_OFFSET * dstRG;
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
        U32 normalAddr=DDRPHY_SSC_SHU_PLL5+((U32)i << POS_BANK_NUM);
        while(normalAddr<=DDRPHY_SSC_SHU_PLL8+((U32)i << POS_BANK_NUM))
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
}
#endif

#if FOR_DV_SIMULATION_USED
void DFSSwitchtoRGMode(DRAMC_CTX_T *p)
{
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_SPM_DVFS_CONTROL_SEL);
}
#endif

U8 vGet_Current_SRAMShuLevel(DRAMC_CTX_T *p)
{
    U8 curr_shu_level;

    curr_shu_level = u4IO32ReadFldAlign(SPM_DRAMC_DPY_CLK_SPM_CON, DRAMC_DPY_CLK_SPM_CON_SC_DR_SHU_LEVEL_SRAM_CH0_PCM);

    return curr_shu_level;
}

void DramcSaveToShuffleSRAM(DRAMC_CTX_T *p, DRAM_DFS_SHUFFLE_TYPE_T srcRG, DRAM_DFS_SHUFFLE_TYPE_T dstRG)
{
    U32 i;
    U8 u1value;
    DRAM_CHANNEL_T eOriChannel = p->channel;

    for(i=0; i<CHANNEL_NUM; i++)
    {
        p->channel = i;

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SRAM_DMA0), 0, SRAM_DMA0_R_SW_DMA_FIRE);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SRAM_DMA0), 0, SRAM_DMA0_R_APB_SLV_SEL);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SRAM_DMA0), 1, SRAM_DMA0_R_SW_MODE);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SRAM_DMA0), 1, SRAM_DMA0_R_SW_STEP_EN_MODE);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SRAM_DMA0), 1, SRAM_DMA0_R_SRAM_WR_MODE);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SRAM_DMA0), 0, SRAM_DMA0_R_APB_WR_MODE);
    
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SRAM_DMA0), srcRG, SRAM_DMA0_R_SW_SHU_LEVEL_APB);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SRAM_DMA0), dstRG, SRAM_DMA0_R_SW_SHU_LEVEL_SRAM);	
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SRAM_DMA0), 1, SRAM_DMA0_R_SW_DMA_FIRE);
        do {
            u1value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_DMA_DEBUG0), MISC_DMA_DEBUG0_SRAM_DONE);
            u1value |= (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_DMA_DEBUG0), MISC_DMA_DEBUG0_APB_DONE) << 1);
            mcSHOW_DBG_MSG3(("\twait dramc to shuffle sram done.\n"));
        } while(u1value != 0x3);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SRAM_DMA0), 0, SRAM_DMA0_R_SW_DMA_FIRE);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SRAM_DMA0), 0, SRAM_DMA0_R_SW_STEP_EN_MODE);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SRAM_DMA0), 0, SRAM_DMA0_R_SW_MODE);
    }

    p->channel = eOriChannel;
}

void LoadShuffleSRAMtoDramc(DRAMC_CTX_T *p, DRAM_DFS_SHUFFLE_TYPE_T srcRG, DRAM_DFS_SHUFFLE_TYPE_T dstRG)
{
    U32 i;
    U8 u1value;
    DRAM_CHANNEL_T eOriChannel = p->channel;

    for(i=0; i<CHANNEL_NUM; i++)
    {
        p->channel = i;

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SRAM_DMA0), 0, SRAM_DMA0_R_SW_DMA_FIRE);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SRAM_DMA0), 0, SRAM_DMA0_R_APB_SLV_SEL);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SRAM_DMA0), 1, SRAM_DMA0_R_SW_MODE);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SRAM_DMA0), 1, SRAM_DMA0_R_SW_STEP_EN_MODE);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SRAM_DMA0), 0, SRAM_DMA0_R_SRAM_WR_MODE); //diff with DramcSaveToShuffleSRAM
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SRAM_DMA0), 1, SRAM_DMA0_R_APB_WR_MODE); // diff with DramcSaveToShuffleSRAM
    
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SRAM_DMA0), dstRG, SRAM_DMA0_R_SW_SHU_LEVEL_APB);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SRAM_DMA0), srcRG, SRAM_DMA0_R_SW_SHU_LEVEL_SRAM);	
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SRAM_DMA0), 1, SRAM_DMA0_R_SW_DMA_FIRE);
        do {
            u1value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_DMA_DEBUG0), MISC_DMA_DEBUG0_SRAM_DONE);
            u1value |= (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_DMA_DEBUG0), MISC_DMA_DEBUG0_APB_DONE) << 1);
            mcSHOW_DBG_MSG3(("\twait shuffle sram to dramc done.\n"));
        } while(u1value != 0x3);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SRAM_DMA0), 0, SRAM_DMA0_R_SW_DMA_FIRE);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SRAM_DMA0), 0, SRAM_DMA0_R_SW_STEP_EN_MODE);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SRAM_DMA0), 0, SRAM_DMA0_R_SW_MODE);
    }

    p->channel = eOriChannel;
}

#if ENABLE_PICG_FREERUN_TO_SHORT_PULSE // The workaround for 1/8 short pulse
void EnableDramcDFSPICGFreeRun(DRAMC_CTX_T *p, U8 cur_shu_level, U8 next_shu_level)
{
    U32 tmp1, tmp2, dmy_read;
    U8 mctrl_picg_bypass;

    gu4Ddrphy0SPMCtrl0 = u4IO32ReadFldAlign(DDRPHY_MISC_SPM_CTRL0, MISC_SPM_CTRL0_PHY_SPM_CTL0);
    gu4Ddrphy1SPMCtrl0 = u4IO32ReadFldAlign(DDRPHY_MISC_SPM_CTRL0+SHIFT_TO_CHB_ADDR, MISC_SPM_CTRL0_PHY_SPM_CTL0);
    gu4Ddrphy0SPMCtrl2 = u4IO32ReadFldAlign(DDRPHY_MISC_SPM_CTRL2, MISC_SPM_CTRL2_PHY_SPM_CTL2);
    gu4Ddrphy1SPMCtrl2 = u4IO32ReadFldAlign(DDRPHY_MISC_SPM_CTRL2+SHIFT_TO_CHB_ADDR, MISC_SPM_CTRL2_PHY_SPM_CTL2);

    tmp1 = ((cur_shu_level == 0x7)  | (cur_shu_level == 0x1)  | (cur_shu_level == 0x2)  | (cur_shu_level == 0x3)  | (cur_shu_level == 0x4));
    tmp2 = ((next_shu_level == 0x7) | (next_shu_level == 0x1) | (next_shu_level == 0x2) | (next_shu_level == 0x3) | (next_shu_level == 0x4));

    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL0, 0x0, MISC_SPM_CTRL0_PHY_SPM_CTL0);
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL0+SHIFT_TO_CHB_ADDR, 0x0, MISC_SPM_CTRL0_PHY_SPM_CTL0);

    mctrl_picg_bypass = tmp1 & tmp2;

    if (mctrl_picg_bypass == 0x1) // below as DDR1600
    {
        vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL2, 0x10001C0, MISC_SPM_CTRL2_PHY_SPM_CTL2); //! CHA MCTL_PICG_CA, FB_PICG, MCK_FB2DLL_PICG free-run during DFS
    }
    else
    {
        vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL2, 0x140, MISC_SPM_CTRL2_PHY_SPM_CTL2); //! CHA FB_PICG, MCK_FB2DLL_PICG  free-run during DFS
    }
  
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL2+SHIFT_TO_CHB_ADDR, 0x0, MISC_SPM_CTRL2_PHY_SPM_CTL2);
    dmy_read = u4IO32ReadFldAlign(DDRPHY_MISC_SPM_CTRL2+SHIFT_TO_CHB_ADDR, MISC_SPM_CTRL2_PHY_SPM_CTL2);  // dummy read for preventing RG write early response
    
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_DPY_MODE_SW); //PICG freerun
}

void RestoreDramcDFSPICGFreeRun(DRAMC_CTX_T *p)
{
    U32 tmp1, tmp2, dmy_read;
    U8 cur_shu_level, mctrl_picg_bypass;

    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DPY_MODE_SW); //PICG freerun

    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL0, gu4Ddrphy0SPMCtrl0, MISC_SPM_CTRL0_PHY_SPM_CTL0);
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL0+SHIFT_TO_CHB_ADDR, gu4Ddrphy1SPMCtrl0, MISC_SPM_CTRL0_PHY_SPM_CTL0);
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL2, gu4Ddrphy0SPMCtrl2, MISC_SPM_CTRL2_PHY_SPM_CTL2);
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL2+SHIFT_TO_CHB_ADDR, gu4Ddrphy1SPMCtrl2, MISC_SPM_CTRL2_PHY_SPM_CTL2);
}
#endif

void DramcDFSDirectJump_SRAMShuRGMode(DRAMC_CTX_T *p, U8 shu_level)
{
    U8 u1ShuAck = 0;
    U8 i = 0, pingpong_shu_level = 0;

    if(p->u1PLLMode == PHYPLL_MODE)
    {
        mcSHOW_DBG_MSG3(("Disable CLRPLL\n"));
        vIO32WriteFldAlign(DDRPHY_PLL2, 0, PLL2_RG_RCLRPLL_EN);
    }
    else
    {
        mcSHOW_DBG_MSG3(("Disable PHYPLL\n"));
        vIO32WriteFldAlign(DDRPHY_PLL1, 0, PLL1_RG_RPHYPLL_EN);        
    }

    for(i=0; i<p->support_channel_num; i++)
    {
        u1ShuAck |= (0x1<<i);
    }

    if(p->u1PLLMode == PHYPLL_MODE)
    {
        mcSHOW_DBG_MSG3(("DFSDirectJump to CLRPLL, SHU_LEVEL=%d, ACK=%x\n", shu_level, u1ShuAck));
    }
    else
    {
        mcSHOW_DBG_MSG3(("DFSDirectJump to PHYPLL, SHU_LEVEL=%d, ACK=%x\n", shu_level, u1ShuAck));
    }

    /*TINFO="DRAM : set ddrphy_fb_ck_en=1"*/
    vIO32WriteFldMulti(DDRPHY_MISC_SPM_CTRL1, P_Fld(1, MISC_SPM_CTRL1_RG_DDRPHY_DB_CK_CH1_EN) | P_Fld(1, MISC_SPM_CTRL1_RG_DDRPHY_DB_CK_CH0_EN)); 

    // sram latch
    vIO32WriteFldMulti((DDRPHY_MISC_SPM_CTRL3), P_Fld(1, MISC_SPM_CTRL3_RG_DR_SHU_LEVEL_SRAM_LATCH_CH1)
                        | P_Fld(1, MISC_SPM_CTRL3_RG_DR_SHU_LEVEL_SRAM_LATCH_CH0));
    mcDELAY_US(1);
    vIO32WriteFldMulti((DDRPHY_MISC_SPM_CTRL3), P_Fld(0, MISC_SPM_CTRL3_RG_DR_SHU_LEVEL_SRAM_LATCH_CH1)
                        | P_Fld(0, MISC_SPM_CTRL3_RG_DR_SHU_LEVEL_SRAM_LATCH_CH0));

    if(p->u1PLLMode == PHYPLL_MODE)
    {
        vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_PHYPLL_SHU_EN);
        vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, !p->u1PLLMode, MISC_SPM_CTRL1_RG_DR_SHU_LEVEL);
        vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_PHYPLL2_SHU_EN);
        mcSHOW_DBG_MSG3(("Enable CLRPLL\n"));
    }
    else
    {
        vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_PHYPLL2_SHU_EN);
        vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, !p->u1PLLMode, MISC_SPM_CTRL1_RG_DR_SHU_LEVEL);
        vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_PHYPLL_SHU_EN);
        mcSHOW_DBG_MSG3(("Enable PHYPLL\n"));
    }
    mcDELAY_US(1);

    //vIO32WriteFldMulti((DDRPHY_MISC_SPM_CTRL3), P_Fld(0, MISC_SPM_CTRL3_RG_DR_SHU_LEVEL_SRAM_CH1)
    //                    | P_Fld(0, MISC_SPM_CTRL3_RG_DR_SHU_LEVEL_SRAM_CH0));
    vIO32WriteFldMulti((DDRPHY_MISC_SPM_CTRL3), P_Fld(shu_level, MISC_SPM_CTRL3_RG_DR_SHU_LEVEL_SRAM_CH1)
                        | P_Fld(shu_level, MISC_SPM_CTRL3_RG_DR_SHU_LEVEL_SRAM_CH0));

    //wait sram load ack.
    vIO32WriteFldMulti((DDRPHY_MISC_SPM_CTRL3), P_Fld(1, MISC_SPM_CTRL3_RG_DR_SRAM_LOAD_CH1)
                        | P_Fld(1, MISC_SPM_CTRL3_RG_DR_SRAM_LOAD_CH0));
    //while (!u4IO32ReadFldAlign(DDRPHY_MISC_DMA_DEBUG0, MISC_DMA_DEBUG0_SC_DR_SRAM_PLL_LOAD_ACK)); // wait SRAM PLL load ack
    while (!u4IO32ReadFldAlign(DDRPHY_MISC_DMA_DEBUG0, MISC_DMA_DEBUG0_SC_DR_SRAM_LOAD_ACK))
    {
        mcSHOW_DBG_MSG3(("\twait sram load ack.\n"));
    }
    vIO32WriteFldMulti((DDRPHY_MISC_SPM_CTRL3), P_Fld(0, MISC_SPM_CTRL3_RG_DR_SRAM_LOAD_CH1)
                        | P_Fld(0, MISC_SPM_CTRL3_RG_DR_SRAM_LOAD_CH0));

    if(p->u1PLLMode == PHYPLL_MODE)
    {
        //vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_PHYPLL2_MODE_SW);
        vIO32WriteFldAlign(DDRPHY_PLL2, 1, PLL2_RG_RCLRPLL_EN); // Darren NOTE: Don't use PHYPLLx_MODE_SW and it will lock RCLRPLL_EN and RPHYPLL_EN control
    }    
    else
    {
        //vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_PHYPLL_MODE_SW);
        vIO32WriteFldAlign(DDRPHY_PLL1, 1, PLL1_RG_RPHYPLL_EN); // Darren NOTE: Don't use PHYPLLx_MODE_SW and it will lock RCLRPLL_EN and RPHYPLL_EN control
    }

#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
    mcDELAY_US(20); // for SRAM shuffle DV sim spec > 20us
#else
    mcDELAY_XUS(20); // for SRAM shuffle DV sim spec > 20us
#endif

#if 0
    mcSHOW_DBG_MSG3(("Enable SHORT-QUEUE\n"));
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_DR_SHORT_QUEUE);

    mcSHOW_DBG_MSG3(("\twait 5us for short queue ack.\n"));
    mcDELAY_US(5);
#endif

    //mcSHOW_DBG_MSG(("Disable RX-Tracking\n"));
    //vIO32WriteFldAlign(SPM_SW_RSV_8, 0, SW_RSV_8_RX_TRACKING_EN);

#if (TDQSCK_PRECALCULATION_FOR_DVFS || defined(HW_GATING))
    DramcDQSPrecalculation_TrackingOff(p, shu_level);
#endif

    mcSHOW_DBG_MSG3(("SHUFFLE Start\n"));
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1,  MISC_SPM_CTRL1_RG_DR_SHU_EN); // NOTE: from SHU_EN=1 to ACK, DV spec < 5.1us

    // Fixed DV sim spec for DFS shu_en=1 < 5.1us and shu_en=0 < 120ns
#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
    //mcSHOW_DBG_MSG3(("\twait 5us for shu_en ack.\n"));
    //mcDELAY_US(5);
    while (!u4IO32ReadFldAlign(DRAMC_REG_SHUSTATUS, SHUSTATUS_SHUFFLE_END))
    {
        mcSHOW_DBG_MSG3(("\twait shu_en ack.\n"));
    }
#else
    while (!u4IO32ReadFldAlign(DRAMC_REG_SHUSTATUS, SHUSTATUS_SHUFFLE_END));
#endif

    //vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_DR_SHORT_QUEUE);
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_DR_SHU_EN); // NOTE: from ACK to SHU_EN=0, DV spec < 120ns
    mcSHOW_DBG_MSG3(("SHUFFLE End\n"));

    //if(shu_level == 0)//LP4-2CH
    //{
        //mcSHOW_DBG_MSG(("Enable RX-Tracking for shuffle-0\n"));
        //vIO32WriteFldAlign(SPM_SW_RSV_8, 3, SW_RSV_8_RX_TRACKING_EN);
    //}

    if(p->u1PLLMode == PHYPLL_MODE)
    {
        /*TINFO="DRAM : set sc_phypll_mode_sw=0"*/
        //vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_PHYPLL_MODE_SW); // Disable PHYPLL
        vIO32WriteFldAlign(DDRPHY_PLL1, 0, PLL1_RG_RPHYPLL_EN); // Darren NOTE: Don't use PHYPLLx_MODE_SW and it will lock RCLRPLL_EN and RPHYPLL_EN control
    }
    else
    {
        /*TINFO="DRAM : set sc_phypll2_mode_sw=0"*/
        //vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_PHYPLL2_MODE_SW); // Disable CLRPLL
        vIO32WriteFldAlign(DDRPHY_PLL2, 0, PLL2_RG_RCLRPLL_EN); // Darren NOTE: Don't use PHYPLLx_MODE_SW and it will lock RCLRPLL_EN and RPHYPLL_EN control
    }

    vIO32WriteFldMulti((DDRPHY_MISC_SPM_CTRL3), P_Fld(1, MISC_SPM_CTRL3_RG_DR_SRAM_RESTORE_CH1)
                        | P_Fld(1, MISC_SPM_CTRL3_RG_DR_SRAM_RESTORE_CH0));
    while (!u4IO32ReadFldAlign(DDRPHY_MISC_DMA_DEBUG0, MISC_DMA_DEBUG0_SC_DR_SRAM_RESTORE_ACK))
    {
        mcSHOW_DBG_MSG3(("\twait sram restore ack.\n"));
    }
    vIO32WriteFldMulti((DDRPHY_MISC_SPM_CTRL3), P_Fld(0, MISC_SPM_CTRL3_RG_DR_SRAM_RESTORE_CH1)
                        | P_Fld(0, MISC_SPM_CTRL3_RG_DR_SRAM_RESTORE_CH0));

    /*TINFO="DRAM : set ddrphy_fb_ck_en=0"*/
    vIO32WriteFldMulti(DDRPHY_MISC_SPM_CTRL1, P_Fld(0, MISC_SPM_CTRL1_RG_DDRPHY_DB_CK_CH1_EN) | P_Fld(0, MISC_SPM_CTRL1_RG_DDRPHY_DB_CK_CH0_EN)); 

#if (TDQSCK_PRECALCULATION_FOR_DVFS || defined(HW_GATING))
    DramcDQSPrecalculation_TrackingOn(p, shu_level);   
#endif

    mcSHOW_DBG_MSG3(("Shuffle flow complete\n"));

    p->u1PLLMode = !p->u1PLLMode;
    return;
}


void DramcDFSDirectJump_RGMode(DRAMC_CTX_T *p, U8 shu_level)
{
    U8 u1ShuAck = 0;
    U8 i = 0;
    U8 u1shu_level = 0;

    if(p->u1PLLMode == PHYPLL_MODE)
    {
        mcSHOW_DBG_MSG3(("Disable CLRPLL\n"));
        vIO32WriteFldAlign(DDRPHY_PLL2, 0, PLL2_RG_RCLRPLL_EN);
    }
    else
    {
        mcSHOW_DBG_MSG3(("Disable PHYPLL\n"));
        vIO32WriteFldAlign(DDRPHY_PLL1, 0, PLL1_RG_RPHYPLL_EN);        
    }

    for(i=0; i<p->support_channel_num; i++)
    {
        u1ShuAck |= (0x1<<i);
    }

    if(p->u1PLLMode == PHYPLL_MODE)
    {
        mcSHOW_DBG_MSG3(("DFSDirectJump to CLRPLL, SHU_LEVEL=%d, ACK=%x\n", shu_level, u1ShuAck));
    }
    else
    {
        mcSHOW_DBG_MSG3(("DFSDirectJump to PHYPLL, SHU_LEVEL=%d, ACK=%x\n", shu_level, u1ShuAck));
    }

    /*TINFO="DRAM : set ddrphy_fb_ck_en=1"*/
    vIO32WriteFldMulti(DDRPHY_MISC_SPM_CTRL1, P_Fld(1, MISC_SPM_CTRL1_RG_DDRPHY_DB_CK_CH1_EN) | P_Fld(1, MISC_SPM_CTRL1_RG_DDRPHY_DB_CK_CH0_EN)); 

    if (shu_level == DRAM_DFS_SHUFFLE_1)
        u1shu_level = shu_level; // Darren: shuffle to shu0 status (original calib flow.)
    else
        u1shu_level = 1; // Darren: Using shu1 for backup/restore, it diff with SPM mode

    if(p->u1PLLMode == PHYPLL_MODE)
    {
        vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_PHYPLL_SHU_EN);
        vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, u1shu_level, MISC_SPM_CTRL1_RG_DR_SHU_LEVEL);
        vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_PHYPLL2_SHU_EN);
        mcSHOW_DBG_MSG3(("Enable CLRPLL\n"));
    }
    else
    {
        vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_PHYPLL2_SHU_EN);
        vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, u1shu_level, MISC_SPM_CTRL1_RG_DR_SHU_LEVEL);
        vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_PHYPLL_SHU_EN);
        mcSHOW_DBG_MSG3(("Enable PHYPLL\n"));
    }
    mcDELAY_US(1);

    if(p->u1PLLMode == PHYPLL_MODE)
    {
        //vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_PHYPLL2_MODE_SW);
        vIO32WriteFldAlign(DDRPHY_PLL2, 1, PLL2_RG_RCLRPLL_EN); // Darren NOTE: Don't use PHYPLLx_MODE_SW and it will lock RCLRPLL_EN and RPHYPLL_EN control
    }    
    else
    {
        //vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_PHYPLL_MODE_SW);
        vIO32WriteFldAlign(DDRPHY_PLL1, 1, PLL1_RG_RPHYPLL_EN); // Darren NOTE: Don't use PHYPLLx_MODE_SW and it will lock RCLRPLL_EN and RPHYPLL_EN control
    }

#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
    mcDELAY_US(20); // for SRAM shuffle DV sim spec > 20us
#else
    mcDELAY_XUS(20); // for SRAM shuffle DV sim spec > 20us
#endif

#if 0
    mcSHOW_DBG_MSG3(("Enable SHORT-QUEUE\n"));
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_DR_SHORT_QUEUE);

    mcSHOW_DBG_MSG3(("\twait 5us for short queue ack.\n"));
    mcDELAY_US(5);
#endif

    //mcSHOW_DBG_MSG(("Disable RX-Tracking\n"));
    //vIO32WriteFldAlign(SPM_SW_RSV_8, 0, SW_RSV_8_RX_TRACKING_EN);


    mcSHOW_DBG_MSG3(("SHUFFLE Start\n"));
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1,  MISC_SPM_CTRL1_RG_DR_SHU_EN);

    //mcSHOW_DBG_MSG3(("\twait 5us for shu_en ack.\n"));
    //mcDELAY_US(5);
    while (!u4IO32ReadFldAlign(DRAMC_REG_SHUSTATUS, SHUSTATUS_SHUFFLE_END))
    {
        mcSHOW_DBG_MSG3(("\twait shu_en ack.\n"));
    }

    //vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_DR_SHORT_QUEUE);
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_DR_SHU_EN);
    mcSHOW_DBG_MSG3(("SHUFFLE End\n"));

    //if(shu_level == 0)//LP4-2CH
    //{
        //mcSHOW_DBG_MSG(("Enable RX-Tracking for shuffle-0\n"));
        //vIO32WriteFldAlign(SPM_SW_RSV_8, 3, SW_RSV_8_RX_TRACKING_EN);
    //}

    if(p->u1PLLMode == PHYPLL_MODE)
    {
        /*TINFO="DRAM : set sc_phypll_mode_sw=0"*/
        //vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_PHYPLL_MODE_SW); // Disable PHYPLL
        vIO32WriteFldAlign(DDRPHY_PLL1, 0, PLL1_RG_RPHYPLL_EN); // Darren NOTE: Don't use PHYPLLx_MODE_SW and it will lock RCLRPLL_EN and RPHYPLL_EN control
    }
    else
    {
        /*TINFO="DRAM : set sc_phypll2_mode_sw=0"*/
        //vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_PHYPLL2_MODE_SW); // Disable CLRPLL
        vIO32WriteFldAlign(DDRPHY_PLL2, 0, PLL2_RG_RCLRPLL_EN); // Darren NOTE: Don't use PHYPLLx_MODE_SW and it will lock RCLRPLL_EN and RPHYPLL_EN control
    }

    /*TINFO="DRAM : set ddrphy_fb_ck_en=0"*/
    vIO32WriteFldMulti(DDRPHY_MISC_SPM_CTRL1, P_Fld(0, MISC_SPM_CTRL1_RG_DDRPHY_DB_CK_CH1_EN) | P_Fld(0, MISC_SPM_CTRL1_RG_DDRPHY_DB_CK_CH0_EN)); 

    mcSHOW_DBG_MSG3(("Shuffle flow complete\n"));

    p->u1PLLMode = !p->u1PLLMode;
    return;
}

void DramcDFSDirectJump_SPMMode(DRAMC_CTX_T *p, U8 shu_level)
{
    U8 u1ShuAck = 0;
    U8 i = 0;
    bool pingpong_shu_level; // for shu0/1
    U8 cur_shu_level = vGet_Current_SRAMShuLevel(p);

    for (i=0; i<p->support_channel_num; i++)
    {
        u1ShuAck |= (0x1 << i);
    }

    if (p->u1PLLMode == PHYPLL_MODE)
    {
        mcSHOW_DBG_MSG3(("DramcDFSDirectJump_SPMMode to CLRPLL, SHU_LEVEL=%d, ACK=%x\n", shu_level, u1ShuAck));
    }
    else
    {
        mcSHOW_DBG_MSG3(("DramcDFSDirectJump_SPMMode to PHYPLL, SHU_LEVEL=%d, ACK=%x\n", shu_level, u1ShuAck));
    }

    vIO32WriteFldAlign(DRAMC_REG_STBCAL2, 0x1, STBCAL2_STB_DBG_STATUS); // HJ Huang
    /*TINFO="DRAM : set ddrphy_fb_ck_en=1"*/
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_DDRPHY_FB_CK_EN);
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_DR_SHU_LEVEL_DRAM_LATCH);
    mcDELAY_US(1);
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DR_SHU_LEVEL_DRAM_LATCH);

    pingpong_shu_level = u4IO32ReadFldAlign(SPM_SPM_POWER_ON_VAL0, SPM_POWER_ON_VAL0_SC_DR_SHU_LEVEL0);

    if (p->u1PLLMode == PHYPLL_MODE)
    {
        vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_PHYPLL_SHU_EN);
        vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, !pingpong_shu_level, SPM_POWER_ON_VAL0_SC_DR_SHU_LEVEL0);
        vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL2_SHU_EN);
        mcSHOW_DBG_MSG3(("Enable CLRPLL\n"));
    }
    else
    {
        vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_PHYPLL2_SHU_EN);
        vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, !pingpong_shu_level, SPM_POWER_ON_VAL0_SC_DR_SHU_LEVEL0);
        vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL_SHU_EN);
        mcSHOW_DBG_MSG3(("Enable PHYPLL\n"));
    }
    mcDELAY_US(1);

#if 0 //Darren test+
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DR_SHU_LEVEL);
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, shu_level, SPM_POWER_ON_VAL0_SC_DR_SHU_LEVEL);
#else
    //vIO32WriteFldMulti((SPM_DRAMC_DPY_CLK_SPM_CON), P_Fld(0, SPM_DRAMC_DPY_CLK_SPM_CON_SC_DR_SHU_LEVEL_SRAM_CH0_PCM)
    //                    | P_Fld(0, SPM_DRAMC_DPY_CLK_SPM_CON_SC_DR_SHU_LEVEL_SRAM_CH1_PCM));
    vIO32WriteFldMulti((SPM_DRAMC_DPY_CLK_SPM_CON), P_Fld(shu_level, DRAMC_DPY_CLK_SPM_CON_SC_DR_SHU_LEVEL_SRAM_CH0_PCM)
                        | P_Fld(shu_level, DRAMC_DPY_CLK_SPM_CON_SC_DR_SHU_LEVEL_SRAM_CH1_PCM));

    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_DR_SRAM_LOAD);
    while ((u4IO32ReadFldAlign(SPM_DRAMC_DPY_CLK_SW_CON3, DRAMC_DPY_CLK_SW_CON3_SC_DR_SRAM_LOAD_ACK) & u1ShuAck ) != u1ShuAck)
    {
        mcSHOW_DBG_MSG3(("\twait sram load ack.\n"));
    }
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DR_SRAM_LOAD);
#endif

    vIO32WriteFldAlign(DRAMC_REG_STBCAL2, 0x2, STBCAL2_STB_DBG_STATUS);

#if ENABLE_DFS_SSC_WA
    DVS_DMY_RD_ENTR(p);
#endif
    if (p->u1PLLMode == PHYPLL_MODE)
        vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL2_MODE_SW);
    else
        vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL_MODE_SW);
#if ENABLE_DFS_SSC_WA    
    DramcSSCHoppingOnOff(p, cur_shu_level, ENABLE);
#endif

    EnableDramcTrackingByShuffle(p, shu_level, DISABLE);

    mcDELAY_US(20);

    /*TINFO="DRAM : set ddrphy_fb_ck_en=0"*/
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DDRPHY_FB_CK_EN);

    /*TINFO="DRAM : set ddrphy_fb_ck_en=1"*/
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_DDRPHY_FB_CK_EN);

#if (TDQSCK_PRECALCULATION_FOR_DVFS || defined(HW_GATING))
    DramcDQSPrecalculation_TrackingOff(p, shu_level);
#endif
    //func_imp_tracking_value_backup();
    //func_imp_tracking_off();
    //func_force_mm_ultra();

#if ENABLE_PICG_FREERUN_TO_SHORT_PULSE
    EnableDramcDFSPICGFreeRun(p, cur_shu_level, shu_level);
#endif

#if ENABLE_DFS_SSC_WA
    DVS_DMY_RD_EXIT(p);
    //DramcSSCHoppingOnOff(p, cur_shu_level, ENABLE); // for waveform measure
    //mcDELAY_US(10); // for waveform measure
#endif

#if ENABLE_DDR800_OPEN_LOOP_MODE_OPTION
    DDR800semiPowerSavingOn(p, shu_level, DISABLE);
#endif

#if (ENABLE_TX_TRACKING && TX_RETRY_ENABLE)
    SPMTx_Track_Retry_OnOff(p, shu_level, ENABLE);
#endif

    mcSHOW_DBG_MSG3(("SHUFFLE Start\n"));
    vIO32WriteFldAlign(DRAMC_REG_STBCAL2, 0x3, STBCAL2_STB_DBG_STATUS);
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1,  SPM_POWER_ON_VAL0_SC_DR_SHU_EN);
    while ((u4IO32ReadFldAlign(SPM_DRAMC_DPY_CLK_SW_CON, DRAMC_DPY_CLK_SW_CON_SC_DR_SHU_EN_ACK) & u1ShuAck ) != u1ShuAck)
    {
        mcSHOW_DBG_MSG3(("\twait shu_en ack.\n"));
    }
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0,  SPM_POWER_ON_VAL0_SC_DR_SHU_EN);
    vIO32WriteFldAlign(DRAMC_REG_STBCAL2, 0x4, STBCAL2_STB_DBG_STATUS);
    mcSHOW_DBG_MSG3(("SHUFFLE End\n"));

    if (p->u1PLLMode == PHYPLL_MODE)
        vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_PHYPLL_MODE_SW); // PHYPLL off
    else
        vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_PHYPLL2_MODE_SW); // CLRPLL off

#if ENABLE_DDR800_OPEN_LOOP_MODE_OPTION
    DDR800semiPowerSavingOn(p, shu_level, ENABLE);
#endif

#if (ENABLE_TX_TRACKING && TX_RETRY_ENABLE)
    SPMTx_Track_Retry_OnOff(p, shu_level, DISABLE);
#endif

#if ENABLE_DFS_SSC_WA
    DramcSSCHoppingOnOff(p, cur_shu_level, DISABLE);
#endif

#if ENABLE_PICG_FREERUN_TO_SHORT_PULSE
    RestoreDramcDFSPICGFreeRun(p);
#endif

    //func_imp_tracking_on();
#if 1 //Darren test+
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_DR_SRAM_RESTORE);
    while ((u4IO32ReadFldAlign(SPM_DRAMC_DPY_CLK_SW_CON3, DRAMC_DPY_CLK_SW_CON3_SC_DR_SRAM_RESTORE_ACK) & u1ShuAck ) != u1ShuAck)
    {
        mcSHOW_DBG_MSG3(("\twait sram restore ack.\n"));
    }
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DR_SRAM_RESTORE);
#endif

    /*TINFO="DRAM : set ddrphy_fb_ck_en=0"*/
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DDRPHY_FB_CK_EN);
#if (TDQSCK_PRECALCULATION_FOR_DVFS || defined(HW_GATING))
    DramcDQSPrecalculation_TrackingOn(p, shu_level);
#endif
    EnableDramcTrackingByShuffle(p, shu_level, ENABLE);

    //-----------------------------------
    //     TRIGGER DRAM GATING ERROR
    //-----------------------------------
    //func_dram_dummy_read_on();
    //mcDELAY_US(2);
    //func_dram_dummy_read_off(); 

    vIO32WriteFldAlign(DRAMC_REG_STBCAL2, 0x5, STBCAL2_STB_DBG_STATUS);
    mcSHOW_DBG_MSG3(("Shuffle flow complete\n"));

    p->u1PLLMode = !p->u1PLLMode;
    return;
}

void DramcDFSDirectJump(DRAMC_CTX_T *p, U8 shu_level)
{
#if (DRAMC_DFS_MODE == 2)
    gDVFSCtrlSel = 1;
#endif
    if(gDVFSCtrlSel==0)
    {
        DramcDFSDirectJump_SPMMode(p, shu_level);
    }
    else
    {
        DramcDFSDirectJump_SRAMShuRGMode(p, shu_level);
    }
}

void No_Parking_On_CLRPLL(DRAMC_CTX_T *p)
{
    if (p->u1PLLMode == PHYPLL_MODE) return; /* already parking on PHYPLL */

    DramcDFSDirectJump_RGMode(p, DRAM_DFS_SHUFFLE_1); /* parking on PHYPLL */
}

//ETT TEST, currently no use
#if 0
void DramcDFSDirectJump_SwitchCtrlMode(DRAMC_CTX_T *p)
{
#if (DRAMC_DFS_MODE == 1)
    gDVFSCtrlSel = 0;
#elif (DRAMC_DFS_MODE == 2)
    gDVFSCtrlSel = 1;
#else
   U8 shu_level;
   gDVFSCtrlSel = !gDVFSCtrlSel;
#endif

    if(gDVFSCtrlSel==0)//SPM Mode
    {
        if(p->u1PLLMode == PHYPLL_MODE)
        {
            mcSHOW_DBG_MSG(("SPM DramcDFSDirectJump_SwitchCtrlMode(%d) current=PHYPLL\n", gDVFSCtrlSel));
            vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_PHYPLL2_MODE_SW);
            vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL_MODE_SW);
            vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, 0, SPM_DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL2_MODE_SW);
            vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, 1, SPM_DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL_MODE_SW);
        }
        else
        {
            mcSHOW_DBG_MSG(("SPM DramcDFSDirectJump_SwitchCtrlMode(%d) current=CLRPLL\n", gDVFSCtrlSel));
            vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_PHYPLL_MODE_SW);
            vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL2_MODE_SW);
            vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, 0, SPM_DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL_MODE_SW);
            vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, 1, SPM_DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL2_MODE_SW);
        }
#if (DRAMC_DFS_MODE == 0)
        shu_level = u4IO32ReadFldAlign(DDRPHY_MISC_SPM_CTRL1, MISC_SPM_CTRL1_RG_DR_SHU_LEVEL);
        vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, shu_level, SPM_POWER_ON_VAL0_SC_DR_SHU_LEVEL);
#endif
    }
    else//RG Mode
    {
        if(p->u1PLLMode == PHYPLL_MODE)
        {
            mcSHOW_DBG_MSG(("RG DramcDFSDirectJump_SwitchCtrlMode(%d) current=PHYPLL\n", gDVFSCtrlSel));
            vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_PHYPLL_SHU_EN);
            vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_PHYPLL_MODE_SW);
            vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_PHYPLL2_SHU_EN);
            vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_PHYPLL2_MODE_SW);
        }
        else
        {
            mcSHOW_DBG_MSG(("RG DramcDFSDirectJump_SwitchCtrlMode(%d) current=CLRPLL\n", gDVFSCtrlSel));
            vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_PHYPLL_SHU_EN);
            vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_PHYPLL_MODE_SW);
            vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_PHYPLL2_SHU_EN);
            vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_PHYPLL2_MODE_SW);
        }
#if (DRAMC_DFS_MODE == 0)
        shu_level = u4IO32ReadFldAlign(SPM_SPM_POWER_ON_VAL0, SPM_POWER_ON_VAL0_SC_DR_SHU_LEVEL);
        vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, shu_level, MISC_SPM_CTRL1_RG_DR_SHU_LEVEL);
#endif
    }
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, gDVFSCtrlSel, MISC_SPM_CTRL1_SPM_DVFS_CONTROL_SEL);
}
#endif

