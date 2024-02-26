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



//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
U8 gDRSEnableSelfWakeup = 0;


void EnableDramcPhyDCMNonShuffle(DRAMC_CTX_T *p, BOOL bEn)
{
    //Common
    vIO32WriteFldAlign_All(DRAMC_REG_CLKCTRL, 0x1, CLKCTRL_SEQCLKRUN3);
    vIO32WriteFldAlign_All(DRAMC_REG_DUMMY_RD, 0x1, DUMMY_RD_DUMMY_RD_PA_OPT);
    vIO32WriteFldMulti_All(DRAMC_REG_EYESCAN, P_Fld(0x0, EYESCAN_RX_DQ_EYE_SEL_B3)
        | P_Fld(0x0, EYESCAN_RX_DQ_EYE_SEL_B2)
        | P_Fld(0x0, EYESCAN_RX_DQ_EYE_SEL_B1)
        | P_Fld(0x0, EYESCAN_RX_DQ_EYE_SEL)
        | P_Fld(0x0, EYESCAN_DCBLNCINS)
        | P_Fld(0x0, EYESCAN_DCBLNCEN)
        | P_Fld(0x0, EYESCAN_EYESCAN_DQS_OPT)
        | P_Fld(0x0, EYESCAN_EYESCAN_NEW_DQ_SYNC_EN)
        | P_Fld(0x0, EYESCAN_EYESCAN_DQ_SYNC_EN)
        | P_Fld(0x0, EYESCAN_EYESCAN_TOG_OPT)
        | P_Fld(0x0, EYESCAN_EYESCAN_CHK_OPT)
        | P_Fld(0x0, EYESCAN_EYESCAN_RD_SEL_OPT));
    vIO32WriteFldAlign_All(DRAMC_REG_MIOCK_JIT_MTR, 0xFFFFFFFF, MIOCK_JIT_MTR_RX_MIOCK_JIT_LIMIT);
    vIO32WriteFldAlign_All(DDRPHY_B0_DQ9, 0x1, B0_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DQ9, 0x1, B1_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B1);
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL4, 0x11400000, MISC_CG_CTRL4_R_PHY_MCK_CG_CTRL);
    vIO32WriteFldAlign_All(DDRPHY_MISC_CTRL0, 0x1, MISC_CTRL0_R_DMSHU_PHYDCM_FORCEOFF);
    vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL6, P_Fld(0x1, MISC_CG_CTRL6_RG_MCK4X_O_FB_CK_CG_OFF)
        | P_Fld(0x1, MISC_CG_CTRL6_RG_CG_DDR400_MCK4X_O_OFF)
        | P_Fld(0x1, MISC_CG_CTRL6_RG_MCK4X_O_OPENLOOP_MODE_EN)
        | P_Fld(0x1, MISC_CG_CTRL6_RG_MCK4X_Q_FB_CK_CG_OFF)
        | P_Fld(0x1, MISC_CG_CTRL6_RG_CG_DDR400_MCK4X_Q_OFF)
        | P_Fld(0x1, MISC_CG_CTRL6_RG_MCK4X_Q_OPENLOOP_MODE_EN)
        | P_Fld(0x1, MISC_CG_CTRL6_RG_MCK4X_I_FB_CK_CG_OFF)
        | P_Fld(0x1, MISC_CG_CTRL6_RG_CG_DDR400_MCK4X_I_OFF)
        | P_Fld(0x1, MISC_CG_CTRL6_RG_MCK4X_I_OPENLOOP_MODE_EN)
        | P_Fld(0x1, MISC_CG_CTRL6_RG_M_CK_OPENLOOP_MODE_EN)
        | P_Fld(0x1, MISC_CG_CTRL6_ARMCTL_CK_OUT_CG_SEL));
    vIO32WriteFldAlign_All(DRAMC_REG_DRAMC_PD_CTRL, 0x1, DRAMC_PD_CTRL_DISSTOP26M); // @Darren, Fixed S1 cause gating FM issue.
    
    if(bEn)
    {
        vIO32WriteFldAlign_All(DRAMC_REG_MISCTL0, 0x0, MISCTL0_REFP_ARBMASK_PBR2PBR_PA_DIS);
        vIO32WriteFldMulti_All(DRAMC_REG_DRAMC_PD_CTRL, P_Fld(0x1, DRAMC_PD_CTRL_COMBCLKCTRL)
            | P_Fld(0x1, DRAMC_PD_CTRL_PHYCLKDYNGEN)
            | P_Fld(0x0, DRAMC_PD_CTRL_MIOCKCTRLOFF)
            | P_Fld(0x0, DRAMC_PD_CTRL_COMBPHY_CLKENSAME)
            | P_Fld(0x1, DRAMC_PD_CTRL_DCMENNOTRFC)
#ifndef DDR_INIT_TIME_PROFILING //To avoid pending between "100 times profile tests" with DDR1600
            | P_Fld(0x1, DRAMC_PD_CTRL_DCMEN2)
#endif
            | P_Fld(0x1, DRAMC_PD_CTRL_DCMEN));
        vIO32WriteFldMulti_All(DRAMC_REG_CLKAR, P_Fld(0x0, CLKAR_RDYCKAR)
            | P_Fld(0x0, CLKAR_CMDCKAR)
            | P_Fld(0x0, CLKAR_CALCKAR)
            | P_Fld(0x0, CLKAR_SEQCLKRUN)
            | P_Fld(0x0, CLKAR_REQQUECLKRUN)
            | P_Fld(0x0, CLKAR_REFCLKRUN)
            | P_Fld(0x0, CLKAR_DWCLKRUN) // @Jouling, for Hw Tx CG
            | P_Fld(0x0, CLKAR_PHYGLUECLKRUN)
            | P_Fld(0x0, CLKAR_TESTCLKRUN)
            | P_Fld(0x0, CLKAR_BCLKAR)
            | P_Fld(0x0, CLKAR_PSELAR)
            | P_Fld(0x0, CLKAR_RDATCKAR)
            | P_Fld(0x0, CLKAR_SELPH_CMD_CG_DIS)
            | P_Fld(0x0, CLKAR_REQQUE_PACG_DIS));
        vIO32WriteFldAlign_All(DRAMC_REG_CLKCTRL, 0x0, CLKCTRL_SEQCLKRUN2);
        vIO32WriteFldMulti_All(DRAMC_REG_SREFCTRL, P_Fld(0x0, SREFCTRL_SCSM_CGAR)
            | P_Fld(0x0, SREFCTRL_SCARB_SM_CGAR)
            | P_Fld(0x0, SREFCTRL_RDDQSOSC_CGAR)
            | P_Fld(0x0, SREFCTRL_HMRRSEL_CGAR));
        vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL1, 0x0, REFCTRL1_SREF_CG_OPT);
        vIO32WriteFldMulti_All(DRAMC_REG_ZQCS, P_Fld(0x0, ZQCS_ZQMASK_CGAR)
            | P_Fld(0x0, ZQCS_ZQCS_MASK_SEL_CGAR));
        vIO32WriteFldAlign_All(DRAMC_REG_SHUCTRL, 0x0, SHUCTRL_DVFS_CG_OPT);

        vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL2, P_Fld(0x0, MISC_CG_CTRL2_RG_MEM_DCM_FORCE_OFF)
            | P_Fld(0x0, MISC_CG_CTRL2_RG_MEM_DCM_FORCE_ON)
            | P_Fld(0x0, MISC_CG_CTRL2_RG_MEM_DCM_APB_TOG));//toggle MEM_DCM_APB_TOG 0->1->0 to let RG setting take effect
        vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL2, 0x1, MISC_CG_CTRL2_RG_MEM_DCM_APB_TOG);
        vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL2, 0x0, MISC_CG_CTRL2_RG_MEM_DCM_APB_TOG);

        vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL5, P_Fld(0x1, MISC_CG_CTRL5_R_CA_PI_DCM_EN)
            | P_Fld(0x1, MISC_CG_CTRL5_R_DQ0_PI_DCM_EN)
            | P_Fld(0x1, MISC_CG_CTRL5_R_DQ1_PI_DCM_EN)
            | P_Fld(0x1, MISC_CG_CTRL5_R_CA_DLY_DCM_EN)
            | P_Fld(0x1, MISC_CG_CTRL5_R_DQ0_DLY_DCM_EN)
            | P_Fld(0x1, MISC_CG_CTRL5_R_DQ1_DLY_DCM_EN));
        vIO32WriteFldMulti_All(DDRPHY_MISC_CTRL4, P_Fld(0x1, MISC_CTRL4_R_OPT2_CG_CS)
            | P_Fld(0x1, MISC_CTRL4_R_OPT2_CG_CLK)
#if (RX_PICG_NEW_MODE || TX_PICG_NEW_MODE)
            | P_Fld(0x1, MISC_CTRL4_R_OPT2_CG_MCK)
            | P_Fld(0x1, MISC_CTRL4_R_OPT2_MPDIV_CG)
#endif           
#if RX_PICG_NEW_MODE
            | P_Fld(0x1, MISC_CTRL4_R_OPT2_CG_DQSIEN)
#endif            
#if TX_PICG_NEW_MODE            
            | P_Fld(0x1, MISC_CTRL4_R_OPT2_CG_DQ)
            | P_Fld(0x1, MISC_CTRL4_R_OPT2_CG_DQS)
            | P_Fld(0x1, MISC_CTRL4_R_OPT2_CG_DQM)
#endif            
            | P_Fld(0x1, MISC_CTRL4_R_OPT2_CG_CMD));
        vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL6, P_Fld(0x0, MISC_CG_CTRL6_RG_DDR400_MCK4X_O_FORCE_ON)
            | P_Fld(0x0, MISC_CG_CTRL6_RG_DDR400_MCK4X_Q_FORCE_ON)
            | P_Fld(0x0, MISC_CG_CTRL6_RG_DDR400_MCK4X_I_FORCE_ON));
    }
    else
    {
        vIO32WriteFldAlign_All(DRAMC_REG_MISCTL0, 0x1, MISCTL0_REFP_ARBMASK_PBR2PBR_PA_DIS);
        vIO32WriteFldMulti_All(DRAMC_REG_DRAMC_PD_CTRL, P_Fld(0x0, DRAMC_PD_CTRL_COMBCLKCTRL)
            | P_Fld(0x0, DRAMC_PD_CTRL_PHYCLKDYNGEN)
            | P_Fld(0x1, DRAMC_PD_CTRL_MIOCKCTRLOFF)
            | P_Fld(0x1, DRAMC_PD_CTRL_COMBPHY_CLKENSAME)
            | P_Fld(0x0, DRAMC_PD_CTRL_DCMENNOTRFC)
            | P_Fld(0x0, DRAMC_PD_CTRL_DCMEN2)
            | P_Fld(0x0, DRAMC_PD_CTRL_DCMEN));
        vIO32WriteFldMulti_All(DRAMC_REG_CLKAR, P_Fld(0x1, CLKAR_RDYCKAR)
            | P_Fld(0x1, CLKAR_CMDCKAR)
            | P_Fld(0x1, CLKAR_CALCKAR)
            | P_Fld(0x1, CLKAR_SEQCLKRUN)
            | P_Fld(0x1, CLKAR_REQQUECLKRUN)
            | P_Fld(0x1, CLKAR_REFCLKRUN)
            | P_Fld(0x1, CLKAR_DWCLKRUN) // @Jouling, for Hw Tx CG
            | P_Fld(0x1, CLKAR_PHYGLUECLKRUN)
            | P_Fld(0x1, CLKAR_TESTCLKRUN)
            | P_Fld(0x1, CLKAR_BCLKAR)
            | P_Fld(0x1, CLKAR_PSELAR)
            | P_Fld(0x1, CLKAR_RDATCKAR)
            | P_Fld(0x1, CLKAR_SELPH_CMD_CG_DIS)
            | P_Fld(0x7FFF, CLKAR_REQQUE_PACG_DIS));
        vIO32WriteFldAlign_All(DRAMC_REG_CLKCTRL, 0x1, CLKCTRL_SEQCLKRUN2);
        vIO32WriteFldMulti_All(DRAMC_REG_SREFCTRL, P_Fld(0x1, SREFCTRL_SCSM_CGAR)
            | P_Fld(0x1, SREFCTRL_SCARB_SM_CGAR)
            | P_Fld(0x1, SREFCTRL_RDDQSOSC_CGAR)
            | P_Fld(0x1, SREFCTRL_HMRRSEL_CGAR));
        vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL1, 0x1, REFCTRL1_SREF_CG_OPT);
        vIO32WriteFldMulti_All(DRAMC_REG_ZQCS, P_Fld(0x1, ZQCS_ZQMASK_CGAR)
            | P_Fld(0x1, ZQCS_ZQCS_MASK_SEL_CGAR));
        vIO32WriteFldAlign_All(DRAMC_REG_SHUCTRL, 0x1, SHUCTRL_DVFS_CG_OPT);

        vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL2, P_Fld(0x0, MISC_CG_CTRL2_RG_MEM_DCM_FORCE_OFF)
            | P_Fld(0x1, MISC_CG_CTRL2_RG_MEM_DCM_FORCE_ON)
            | P_Fld(0x0, MISC_CG_CTRL2_RG_MEM_DCM_APB_TOG));//toggle MEM_DCM_APB_TOG 0->1->0 to let RG setting take effect
        vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL2, 0x1, MISC_CG_CTRL2_RG_MEM_DCM_APB_TOG);
        vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL2, 0x0, MISC_CG_CTRL2_RG_MEM_DCM_APB_TOG);

        vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL5, P_Fld(0x0, MISC_CG_CTRL5_R_CA_PI_DCM_EN)
            | P_Fld(0x0, MISC_CG_CTRL5_R_DQ0_PI_DCM_EN)
            | P_Fld(0x0, MISC_CG_CTRL5_R_DQ1_PI_DCM_EN)
            | P_Fld(0x0, MISC_CG_CTRL5_R_CA_DLY_DCM_EN)
            | P_Fld(0x0, MISC_CG_CTRL5_R_DQ0_DLY_DCM_EN)
            | P_Fld(0x0, MISC_CG_CTRL5_R_DQ1_DLY_DCM_EN));
        vIO32WriteFldMulti_All(DDRPHY_MISC_CTRL4, P_Fld(0x0, MISC_CTRL4_R_OPT2_CG_CS)
            | P_Fld(0x0, MISC_CTRL4_R_OPT2_CG_CLK)
            | P_Fld(0x0, MISC_CTRL4_R_OPT2_CG_CMD)
            | P_Fld(0x0, MISC_CTRL4_R_OPT2_CG_DQSIEN)
            | P_Fld(0x0, MISC_CTRL4_R_OPT2_CG_DQ)
            | P_Fld(0x0, MISC_CTRL4_R_OPT2_CG_DQS)
            | P_Fld(0x0, MISC_CTRL4_R_OPT2_CG_DQM)
            | P_Fld(0x0, MISC_CTRL4_R_OPT2_CG_MCK)
            | P_Fld(0x0, MISC_CTRL4_R_OPT2_MPDIV_CG));
        vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL6, P_Fld(0x1, MISC_CG_CTRL6_RG_DDR400_MCK4X_O_FORCE_ON)
            | P_Fld(0x1, MISC_CG_CTRL6_RG_DDR400_MCK4X_Q_FORCE_ON)
            | P_Fld(0x1, MISC_CG_CTRL6_RG_DDR400_MCK4X_I_FORCE_ON));
    }
    return;
}


void EnableDramcPhyDCMShuffle(DRAMC_CTX_T *p, BOOL bEn, U32 u4DramcShuOffset, U32 u4DDRPhyShuOffset)
{
        //Common
        vIO32WriteFldAlign_All(DDRPHY_SHU_B0_DQ8 + u4DDRPhyShuOffset, 0x1, SHU_B0_DQ8_R_RMRX_TOPHY_CG_IG_B0);
        vIO32WriteFldAlign_All(DDRPHY_SHU_B1_DQ7 + u4DDRPhyShuOffset, 0x0, SHU_B1_DQ7_R_DMRANKRXDVS_B1);
        vIO32WriteFldMulti_All(DDRPHY_SHU_B1_DQ8 + u4DDRPhyShuOffset, P_Fld(0x0, SHU_B1_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B1)
            | P_Fld(0x1, SHU_B1_DQ8_R_RMRX_TOPHY_CG_IG_B1));
        vIO32WriteFldAlign_All(DDRPHY_SHU_CA_CMD7 + u4DDRPhyShuOffset, 0x0, SHU_CA_CMD7_R_DMTX_ARPI_CG_CLK_NEW);
        vIO32WriteFldAlign(DDRPHY_SHU_PLL22 + u4DDRPhyShuOffset, 0x1, SHU_PLL22_RG_RPHYPLL_ADA_MCK8X_EN_SHU);
        if(vGet_DDR800_Mode(p) == DDR800_SEMI_LOOP)
        {
            vIO32WriteFldAlign_All(DDRPHY_SHU_B0_DLL0 + u4DDRPhyShuOffset, 0x0, SHU_B0_DLL0_RG_ARDLL_PHDET_EN_B0_SHU);
            vIO32WriteFldAlign_All(DDRPHY_SHU_B1_DLL0 + u4DDRPhyShuOffset, 0x0, SHU_B1_DLL0_RG_ARDLL_PHDET_EN_B1_SHU);
            vIO32WriteFldAlign(DDRPHY_SHU_CA_DLL0 + u4DDRPhyShuOffset, 0x1, SHU_CA_DLL0_RG_ARDLL_PHDET_EN_CA_SHU);
            vIO32WriteFldAlign(DDRPHY_SHU_CA_DLL0 + SHIFT_TO_CHB_ADDR + u4DDRPhyShuOffset, 0x0, SHU_CA_DLL0_RG_ARDLL_PHDET_EN_CA_SHU);
        }
        else
        {
            vIO32WriteFldAlign_All(DDRPHY_SHU_B0_DLL0 + u4DDRPhyShuOffset, 0x1, SHU_B0_DLL0_RG_ARDLL_PHDET_EN_B0_SHU);
            vIO32WriteFldAlign_All(DDRPHY_SHU_B1_DLL0 + u4DDRPhyShuOffset, 0x1, SHU_B1_DLL0_RG_ARDLL_PHDET_EN_B1_SHU);
            vIO32WriteFldAlign_All(DDRPHY_SHU_CA_DLL0 + u4DDRPhyShuOffset, 0x1, SHU_CA_DLL0_RG_ARDLL_PHDET_EN_CA_SHU);
        }        

        if(bEn)
        {
#if TX_PICG_NEW_MODE
            vIO32WriteFldAlign_All(DRAMC_REG_SHU_APHY_TX_PICG_CTRL + u4DramcShuOffset, 0x1, SHU_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_OPT);
#endif
            vIO32WriteFldMulti_All(DDRPHY_SHU_B0_DQ7 + u4DDRPhyShuOffset, P_Fld(0x1, SHU_B0_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B0)
                | P_Fld(0x1, SHU_B0_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B0)
                | P_Fld(0x1, SHU_B0_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B0));
            vIO32WriteFldMulti_All(DDRPHY_SHU_B0_DQ8 + u4DDRPhyShuOffset, P_Fld(0x0, SHU_B0_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B0)
                | P_Fld(0x0, SHU_B0_DQ8_R_DMRANK_PIPE_CG_IG_B0)
                | P_Fld(0x0, SHU_B0_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B0)
                | P_Fld(0x0, SHU_B0_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B0)
                | P_Fld(0x0, SHU_B0_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B0)
                | P_Fld(0x0, SHU_B0_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B0)
                | P_Fld(0x0, SHU_B0_DQ8_R_DMSTBEN_SYNC_CG_IG_B0)
                | P_Fld(0x0, SHU_B0_DQ8_R_DMRXDLY_CG_IG_B0)
                | P_Fld(0x0, SHU_B0_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B0)
                | P_Fld(0x0, SHU_B0_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B0)
                | P_Fld(0x0, SHU_B0_DQ8_R_RMRODTEN_CG_IG_B0)
                | P_Fld(0x0, SHU_B0_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B0));
            vIO32WriteFldMulti_All(DDRPHY_SHU_B1_DQ7 + u4DDRPhyShuOffset, P_Fld(0x1, SHU_B1_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B1)
                | P_Fld(0x1, SHU_B1_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B1)
                | P_Fld(0x1, SHU_B1_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B1));
            vIO32WriteFldMulti_All(DDRPHY_SHU_B1_DQ8 + u4DDRPhyShuOffset, P_Fld(0x0, SHU_B1_DQ8_R_DMRANK_PIPE_CG_IG_B1)
                | P_Fld(0x0, SHU_B1_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B1)
                | P_Fld(0x0, SHU_B1_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B1)
                | P_Fld(0x0, SHU_B1_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B1)
                | P_Fld(0x0, SHU_B1_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B1)
                | P_Fld(0x0, SHU_B1_DQ8_R_DMSTBEN_SYNC_CG_IG_B1)
                | P_Fld(0x0, SHU_B1_DQ8_R_DMRXDLY_CG_IG_B1)
                | P_Fld(0x0, SHU_B1_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B1)
                | P_Fld(0x0, SHU_B1_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B1)
                | P_Fld(0x0, SHU_B1_DQ8_R_RMRODTEN_CG_IG_B1)
                | P_Fld(0x0, SHU_B1_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B1));
            vIO32WriteFldMulti_All(DDRPHY_SHU_CA_CMD7 + u4DDRPhyShuOffset, P_Fld(0x1, SHU_CA_CMD7_R_DMTX_ARPI_CG_CS_NEW)
                | P_Fld(0x1, SHU_CA_CMD7_R_DMTX_ARPI_CG_CMD_NEW)
                | P_Fld(0x0, SHU_CA_CMD7_R_DMRANKRXDVS_CA));
            vIO32WriteFldMulti_All(DDRPHY_SHU_CA_CMD8 + u4DDRPhyShuOffset, P_Fld(0x0, SHU_CA_CMD8_R_DMRANK_CHG_PIPE_CG_IG_CA)
                | P_Fld(0x0, SHU_CA_CMD8_R_DMRANK_PIPE_CG_IG_CA)
                | P_Fld(0x0, SHU_CA_CMD8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_CA)
                | P_Fld(0x0, SHU_CA_CMD8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_CA)
                | P_Fld(0x0, SHU_CA_CMD8_R_DMDQSIEN_FLAG_PIPE_CG_IG_CA)
                | P_Fld(0x0, SHU_CA_CMD8_R_DMDQSIEN_FLAG_SYNC_CG_IG_CA)
                | P_Fld(0x0, SHU_CA_CMD8_R_RMRX_TOPHY_CG_IG_CA)
                | P_Fld(0x0, SHU_CA_CMD8_R_RMRODTEN_CG_IG_CA));
        }
        else
        {
            vIO32WriteFldAlign_All(DRAMC_REG_SHU_APHY_TX_PICG_CTRL + u4DramcShuOffset, 0x0, SHU_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_OPT);
            vIO32WriteFldMulti_All(DDRPHY_SHU_B0_DQ7 + u4DDRPhyShuOffset, P_Fld(0x0, SHU_B0_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B0)
                | P_Fld(0x0, SHU_B0_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B0)
                | P_Fld(0x0, SHU_B0_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B0));
            vIO32WriteFldMulti_All(DDRPHY_SHU_B0_DQ8 + u4DDRPhyShuOffset, P_Fld(0x1, SHU_B0_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B0)
                | P_Fld(0x1, SHU_B0_DQ8_R_DMRANK_PIPE_CG_IG_B0)
                | P_Fld(0x1, SHU_B0_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B0)
                | P_Fld(0x1, SHU_B0_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B0)
                | P_Fld(0x1, SHU_B0_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B0)
                | P_Fld(0x1, SHU_B0_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B0)
                | P_Fld(0x1, SHU_B0_DQ8_R_DMSTBEN_SYNC_CG_IG_B0)
                | P_Fld(0x1, SHU_B0_DQ8_R_DMRXDLY_CG_IG_B0)
                | P_Fld(0x1, SHU_B0_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B0)
                | P_Fld(0x1, SHU_B0_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B0)
                | P_Fld(0x1, SHU_B0_DQ8_R_RMRODTEN_CG_IG_B0)
                | P_Fld(0x1, SHU_B0_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B0));
            vIO32WriteFldMulti_All(DDRPHY_SHU_B1_DQ7 + u4DDRPhyShuOffset, P_Fld(0x0, SHU_B1_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B1)
                | P_Fld(0x0, SHU_B1_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B1)
                | P_Fld(0x0, SHU_B1_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B1));
            vIO32WriteFldMulti_All(DDRPHY_SHU_B1_DQ8 + u4DDRPhyShuOffset, P_Fld(0x1, SHU_B1_DQ8_R_DMRANK_PIPE_CG_IG_B1)
                | P_Fld(0x1, SHU_B1_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B1)
                | P_Fld(0x1, SHU_B1_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B1)
                | P_Fld(0x1, SHU_B1_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B1)
                | P_Fld(0x1, SHU_B1_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B1)
                | P_Fld(0x1, SHU_B1_DQ8_R_DMSTBEN_SYNC_CG_IG_B1)
                | P_Fld(0x1, SHU_B1_DQ8_R_DMRXDLY_CG_IG_B1)
                | P_Fld(0x1, SHU_B1_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B1)
                | P_Fld(0x1, SHU_B1_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B1)
                | P_Fld(0x1, SHU_B1_DQ8_R_RMRODTEN_CG_IG_B1)
                | P_Fld(0x1, SHU_B1_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B1));
            vIO32WriteFldMulti_All(DDRPHY_SHU_CA_CMD7 + u4DDRPhyShuOffset, P_Fld(0x0, SHU_CA_CMD7_R_DMTX_ARPI_CG_CS_NEW)
                | P_Fld(0x0, SHU_CA_CMD7_R_DMTX_ARPI_CG_CMD_NEW)
                | P_Fld(0x1, SHU_CA_CMD7_R_DMRANKRXDVS_CA));
            vIO32WriteFldMulti_All(DDRPHY_SHU_CA_CMD8 + u4DDRPhyShuOffset, P_Fld(0x1, SHU_CA_CMD8_R_DMRANK_CHG_PIPE_CG_IG_CA)
                | P_Fld(0x1, SHU_CA_CMD8_R_DMRANK_PIPE_CG_IG_CA)
                | P_Fld(0x1, SHU_CA_CMD8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_CA)
                | P_Fld(0x1, SHU_CA_CMD8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_CA)
                | P_Fld(0x1, SHU_CA_CMD8_R_DMDQSIEN_FLAG_PIPE_CG_IG_CA)
                | P_Fld(0x1, SHU_CA_CMD8_R_DMDQSIEN_FLAG_SYNC_CG_IG_CA)
                | P_Fld(0x1, SHU_CA_CMD8_R_RMRX_TOPHY_CG_IG_CA)
                | P_Fld(0x1, SHU_CA_CMD8_R_RMRODTEN_CG_IG_CA));
        }
    return;
}


void EnableDramcPhyDCM(DRAMC_CTX_T *p, BOOL bEn)
{
    U32 u4WbrBackup = GetDramcBroadcast();
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);

    EnableDramcPhyDCMNonShuffle(p, bEn);
    EnableDramcPhyDCMShuffle(p, bEn, 0, 0);//only need to set SHU0 RG while init, SHU0 will copy to others

    DramcBroadcastOnOff(u4WbrBackup);
    return;
}

void HwSaveForSR(DRAMC_CTX_T *p)
{
    vIO32WriteFldMulti_All(DRAMC_REG_RSTMASK, P_Fld(0, RSTMASK_GT_SYNC_MASK)
                | P_Fld(0, RSTMASK_GT_SYNC_MASK_FOR_PHY));
    vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL1, 1, REFCTRL1_SLEFREF_AUTOSAVE_EN);
    vIO32WriteFldMulti_All(DRAMC_REG_SREFCTRL, P_Fld(1, SREFCTRL_SREF2_OPTION)
                | P_Fld(0, SREFCTRL_SREF3_OPTION));
}


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
    U8 u1ShuIdx = 0, u1ShuCnt = 3; //TODO: change u1ShuCnt to actual shuffle num define
    U32 u4targetAddr = 0; //For SHU_ODTCTRL_RODTENSTB_SELPH_CG_IG, SHU_ODTCTRL_RODTEN_SELPH_CG_IG shuffle regs

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

    // Below loop sets SHU*_ODTCTRL_RODTENSTB_SELPH_CG_IG, SHU*_ODTCTRL_RODTEN_SELPH_CG_IG (wei-jen)
    for (u1ShuIdx = DRAM_DFS_SHUFFLE_1; u1ShuIdx < u1ShuCnt; u1ShuIdx++)
    {
        u4targetAddr = DRAMC_REG_SHU_ODTCTRL + SHU_GRP_DRAMC_OFFSET * u1ShuIdx;
        vIO32WriteFldMulti_All(u4targetAddr, P_Fld(0x0, SHU_ODTCTRL_RODTENSTB_SELPH_CG_IG)
                                            | P_Fld(0x0, SHU_ODTCTRL_RODTEN_SELPH_CG_IG));
    }

}
#endif

#if ENABLE_DDR800_OPEN_LOOP_MODE_OPTION
void EnableDllCg(DRAMC_CTX_T *p, U32 u4OnOff)
{
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI2, u4OnOff, B0_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI2, u4OnOff, B1_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B1);
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI2+SHIFT_TO_CHB_ADDR, u4OnOff, CA_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_CA);
}


void DDR800semiPowerSavingOn(DRAMC_CTX_T *p, U8 next_shu_level, U8 u1OnOff)
{
    U8 u1ShuLevel = u4IO32ReadFldAlign(DRAMC_REG_SHUSTATUS, SHUSTATUS_SHUFFLE_LEVEL);
    U8 u1IsDdr800Semi = u4IO32ReadFldAlign(DDRPHY_SHU_PLL1+(SHU_GRP_DDRPHY_OFFSET*u1ShuLevel), SHU_PLL1_RG_RPHYPLL_DDR400_EN);  

    if (u1IsDdr800Semi != 1) // close mode will return
        return;

    if ((next_shu_level != SRAM_SHU9) && (u1OnOff == DISABLE))
    {
        // for NORMAL_CLOSE_LOOP
        EnableDllCg(p, DISABLE);
    }
    else if ((next_shu_level == SRAM_SHU9) && (u1OnOff == ENABLE))
    {
        // for DDR800_SEMI_LOOP power saving, clock gating
        EnableDllCg(p, ENABLE);
    }
}
#endif

void DramcDRS(DRAMC_CTX_T *p, U8 bEnable)
{
    //R_DMDRS_CNTX[6:0](DVT set 0, HQA set 4 or 5)
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_DRSCTRL), P_Fld(0, DRSCTRL_DRSPB2AB_OPT)
                                                            | P_Fld(0, DRSCTRL_DRSMON_CLR)
                                                            | P_Fld(8, DRSCTRL_DRSDLY)
                                                            | P_Fld(0, DRSCTRL_DRSACKWAITREF)
                                                            | P_Fld(!bEnable, DRSCTRL_DRSDIS)
                                                            | P_Fld(0, DRSCTRL_DRSCLR_EN) //@Jouling, power improve
                                                            | P_Fld(3, DRSCTRL_DRS_CNTX)
                                                            | P_Fld(!gDRSEnableSelfWakeup, DRSCTRL_DRS_SELFWAKE_DMYRD_DIS)
                                                            | P_Fld(0, DRSCTRL_DRSOPT2));
}

#if ENABLE_MCK8X_MODE
void EnableMck8xEnShu(DRAMC_CTX_T *p)
{
    vIO32WriteFldAlign_All(DDRPHY_DVFS_EMI_CLK, 0x1, DVFS_EMI_CLK_RG_ADA_MCK8X_EN_SHUFFLE);
    vIO32WriteFldMulti_All(DDRPHY_MISC_CTRL2, P_Fld(0x1, MISC_CTRL2_RG_ADA_MCK8X_EN_SHU_OPT) 
                | P_Fld(0x1, MISC_CTRL2_RG_PHDET_EN_SHU_OPT));
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
        // ONLY work for LP4, not LP3
        // MISCA_SRFPD_DIS =1, self-refresh
        // MISCA_SRFPD_DIS =0, self-refresh power down
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SREFCTRL), 1, SREFCTRL_SRFPD_DIS);

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


