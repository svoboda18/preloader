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
#include <pmic.h>
#endif
#endif


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
#define SLT_LS0 1
#define SLT_LS1 0

#if (ENABLE_EMI_LPBK_TEST)
int gSLT_LS = SLT_LS1; // test @ LS1
#endif

void EMI_LPBK_memory_test_start(DRAMC_CTX_T * p, U8 K_win);




#if defined(SLT)
void O1Path_Test(DRAMC_CTX_T * p)
{
    U32 channel_idx, backup_channel;
    U32 u4dq_o1, u4ca_o1, u4fail_up=0, u4fail_dn=0, fail_cnt=0; //, loop=0, retry_dn_cnt=0, retry_up_cnt=0;
    
    mcSHOW_ERR_MSG(("[O1Path] O1 TEST\n"));
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
            ((DDRPHY_B0_DQ2)),
            ((DDRPHY_B1_DQ2)),
            ((DDRPHY_B0_DQ2)+SHIFT_TO_CHB_ADDR),
            ((DDRPHY_B1_DQ2)+SHIFT_TO_CHB_ADDR),
            (DRAMC_REG_ADDR(DDRPHY_B0_DQ7)),
            (DRAMC_REG_ADDR(DDRPHY_B1_DQ7)),
            ((DDRPHY_CA_CMD2)),
            ((DDRPHY_CA_CMD2)+SHIFT_TO_CHB_ADDR),
            (DRAMC_REG_ADDR(DDRPHY_CA_CMD7)),
            (DRAMC_REG_ADDR(DDRPHY_B0_DQ0)),
            (DRAMC_REG_ADDR(DDRPHY_B0_DQ1)),
            (DRAMC_REG_ADDR(DDRPHY_B1_DQ0)),
            (DRAMC_REG_ADDR(DDRPHY_B1_DQ1)),
            (DRAMC_REG_ADDR(DDRPHY_CA_CMD0)),
            (DRAMC_REG_ADDR(DDRPHY_CA_CMD1)),
            (DRAMC_REG_ADDR(DDRPHY_CA_CMD6)),
            (DRAMC_REG_ADDR(DDRPHY_MISC_EXTLB18)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ8)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_B1_DQ8)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_CA_CMD8)),
            (DRAMC_REG_ADDR(DDRPHY_SHU_CA_CMD5)),
        };
        DramcBackupRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));

        // add new RG setting for Sylv_ia/Cann_on O1Path pull dn/up setting
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_CMD5), 1, CA_CMD5_RG_RX_ARCMD_VREF_EN);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_CA_CMD5), 0x29, SHU_CA_CMD5_RG_RX_ARCMD_VREF_SEL);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MISC_EXTLB18), P_Fld(0x1, MISC_EXTLB18_R_LPBK_DQ_RX_MODE)|P_Fld(0x1, MISC_EXTLB18_R_LPBK_CA_RX_MODE));
      
        //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_CMD6), 0, CA_CMD6_RG_TX_ARCMD_DDR4_SEL);//for sim 
        
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ8), 1, SHU_B0_DQ8_R_RMRX_TOPHY_CG_IG_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B1_DQ8), 1, SHU_B1_DQ8_R_RMRX_TOPHY_CG_IG_B1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_CA_CMD8), 1, SHU_CA_CMD8_R_RMRX_TOPHY_CG_IG_CA);
          
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_CMD3), 1, CA_CMD3_RG_RX_ARCMD_SMT_EN);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_CMD5), 1, CA_CMD5_RG_RX_ARCMD_EYE_VREF_EN);
                                    
        // DQ, CMD OE_DIS, On no dram load board, CH_A/B must concurrently set OE_DIS=1 
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ2), P_Fld(0x1, B0_DQ2_RG_TX_ARDQ_OE_DIS_B0)|P_Fld(0x1, B0_DQ2_RG_TX_ARDQM0_OE_DIS_B0));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ2), P_Fld(0x1, B1_DQ2_RG_TX_ARDQ_OE_DIS_B1)|P_Fld(0x1, B1_DQ2_RG_TX_ARDQM0_OE_DIS_B1));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_CA_CMD2), P_Fld(0x1, CA_CMD2_RG_TX_ARCMD_OE_DIS)|P_Fld(0x1, CA_CMD2_RG_TX_ARCLK_OE_DIS));
    
        // pull down DQ, DQM, CMD, CS, CLK
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B0_DQ7), P_Fld(0x1, B0_DQ7_RG_TX_ARDQ_PULL_DN_B0)|P_Fld(0x1, B0_DQ7_RG_TX_ARDQM0_PULL_DN_B0));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B1_DQ7), P_Fld(0x1, B1_DQ7_RG_TX_ARDQ_PULL_DN_B1)|P_Fld(0x1, B1_DQ7_RG_TX_ARDQM0_PULL_DN_B1));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_CA_CMD7), P_Fld(0x1, CA_CMD7_RG_TX_ARCMD_PULL_DN)|P_Fld(0x1, CA_CMD7_RG_TX_ARCS_PULL_DN)|P_Fld(0x1, CA_CMD7_RG_TX_ARCLK_PULL_DN));
    
        mcDELAY_US(1);

        u4dq_o1 = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_AD_RX_DQ_O1));
        u4ca_o1 = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_AD_RX_CMD_O1));
        if (u4dq_o1!=0 || u4ca_o1!=0) //refer to RG coda field
            u4fail_dn = 1;
        mcSHOW_ERR_MSG(("[O1Path] CH_%d PULL DN=1, DQ-DQM O1=0x%x (%s), CS-CKE-CA O1=0x%x (%s)\n",channel_idx,u4dq_o1,(u4dq_o1==0)?"pass":"fail",u4ca_o1,(u4ca_o1==0)?"pass":"fail"));


        // pull up DQ, DQM, CMD, CS, CLK
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B0_DQ7), P_Fld(0x0, B0_DQ7_RG_TX_ARDQ_PULL_DN_B0) | 
                                                          P_Fld(0x0, B0_DQ7_RG_TX_ARDQM0_PULL_DN_B0) |
                                                          P_Fld(0x1, B0_DQ7_RG_TX_ARDQ_PULL_UP_B0) | 
                                                          P_Fld(0x1, B0_DQ7_RG_TX_ARDQM0_PULL_UP_B0));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B1_DQ7), P_Fld(0x0, B1_DQ7_RG_TX_ARDQ_PULL_DN_B1)|
                                                          P_Fld(0x0, B1_DQ7_RG_TX_ARDQM0_PULL_DN_B1)|
                                                          P_Fld(0x1, B1_DQ7_RG_TX_ARDQ_PULL_UP_B1)|
                                                          P_Fld(0x1, B1_DQ7_RG_TX_ARDQM0_PULL_UP_B1));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_CA_CMD7),P_Fld(0x0, CA_CMD7_RG_TX_ARCMD_PULL_DN)|
                                                          P_Fld(0x0, CA_CMD7_RG_TX_ARCS_PULL_DN)|
                                                          P_Fld(0x0, CA_CMD7_RG_TX_ARCLK_PULL_DN)|
                                                          P_Fld(0x1, CA_CMD7_RG_TX_ARCMD_PULL_UP)|
                                                          P_Fld(0x1, CA_CMD7_RG_TX_ARCS_PULL_UP)|
                                                          P_Fld(0x1, CA_CMD7_RG_TX_ARCLK_PULL_UP));
        
        mcDELAY_US(1);

        u4dq_o1 = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_AD_RX_DQ_O1));
        u4ca_o1 = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_AD_RX_CMD_O1));
        if (u4dq_o1!=0x1FF01FF || u4ca_o1!=0x6C3F) //refer to RG coda field
            u4fail_up = 1;
        mcSHOW_ERR_MSG(("[O1Path] CH_%d PULL UP=1, DQ-DQM O1=0x%x (%s), CS-CKE-CA O1=0x%x (%s)\n",channel_idx,u4dq_o1,(u4dq_o1==0x1FF01FF)?"pass":"fail",u4ca_o1,(u4ca_o1==0x6C3F)?"pass":"fail"));

        DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));
        if (u4fail_up||u4fail_dn)
        {
            fail_cnt |= 1<<p->channel;
        }
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B0_DQ7), P_Fld(0x0, B0_DQ7_RG_TX_ARDQ_PULL_DN_B0) | 
                                                          P_Fld(0x0, B0_DQ7_RG_TX_ARDQM0_PULL_DN_B0) |
                                                          P_Fld(0x0, B0_DQ7_RG_TX_ARDQ_PULL_UP_B0) | 
                                                          P_Fld(0x0, B0_DQ7_RG_TX_ARDQM0_PULL_UP_B0));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B1_DQ7), P_Fld(0x0, B1_DQ7_RG_TX_ARDQ_PULL_DN_B1)|
                                                          P_Fld(0x0, B1_DQ7_RG_TX_ARDQM0_PULL_DN_B1)|
                                                          P_Fld(0x0, B1_DQ7_RG_TX_ARDQ_PULL_UP_B1)|
                                                          P_Fld(0x0, B1_DQ7_RG_TX_ARDQM0_PULL_UP_B1));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_CA_CMD7),P_Fld(0x0, CA_CMD7_RG_TX_ARCMD_PULL_DN)|
                                                          P_Fld(0x0, CA_CMD7_RG_TX_ARCS_PULL_DN)|
                                                          P_Fld(0x0, CA_CMD7_RG_TX_ARCLK_PULL_DN)|
                                                          P_Fld(0x0, CA_CMD7_RG_TX_ARCMD_PULL_UP)|
                                                          P_Fld(0x0, CA_CMD7_RG_TX_ARCS_PULL_UP)|
                                                          P_Fld(0x0, CA_CMD7_RG_TX_ARCLK_PULL_UP));
    }
    O1PathOnOff(p, 0);

    vSetPHY2ChannelMapping(p, backup_channel);
    if (fail_cnt)
        while(1);
}
#endif //#if defined(SLT)




//======================== Low_Power_Scenarios_Test Begin =====================================
#if (ENABLE_EMI_LPBK_TEST)
#if (fcFOR_CHIP_ID == fcLafite)

void SLT_Func_Emi_Off(DRAMC_CTX_T *p)
{
    int i = 0;
    U32 u4value = 0;
    U32 u4SrAck = (u1IsLP4Family(p->dram_type)) ? 3 : 1;//LP4: CHA CHB; LP3: CHA

    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL1, 1, SPM_POWER_ON_VAL1_EMI_CLK_OFF_REQ);
    for(i = 2; i > 0; i--)
    {
        mcDELAY_US(1);
        u4value = u4IO32ReadFldAlign(SPM_DRAMC_DPY_CLK_SW_CON, DRAMC_DPY_CLK_SW_CON_EMI_CLK_OFF_ACK);
        if(u4SrAck == u4value)
        {
            break;
        }
    }

    if(i == 0) //no issue Ack =>status = 1
    {
        mcSHOW_DBG_MSG(("!Wait no ack(Enter SR)[0x%X]\n", u4value));
        vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL1, 0, SPM_POWER_ON_VAL1_EMI_CLK_OFF_REQ);
    }
    return;
}


void SLT_Func_Emi_On(DRAMC_CTX_T *p)
{
    int i = 0;
    U32 u4value = 0;
    U32 u4SrAck = (u1IsLP4Family(p->dram_type)) ? 3 : 1;//LP4: CHA CHB; LP3: CHA

    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL1, 0, SPM_POWER_ON_VAL1_EMI_CLK_OFF_REQ);
    
    for(i = 2; i > 0; i--)
    {
        mcDELAY_US(1);
        u4value = u4IO32ReadFldAlign(SPM_DRAMC_DPY_CLK_SW_CON, DRAMC_DPY_CLK_SW_CON_EMI_CLK_OFF_ACK);
        if((u4SrAck & u4value) == 0)
        {
            break;
        }
    }
    if(i == 0) //no issue Ack =>status = 1
    {
        mcSHOW_DBG_MSG(("!Wait no ack(Leave SR)[0x%X]\n", u4value));
    }
}

void SLT_FUNC_DDRPHY_DLL_OFF_INFRA_CG_EN_ENTER(DRAMC_CTX_T *p)
{
    //Pause frequency hopping; {no freq hopping}
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL1, 1, SPM_POWER_ON_VAL1_SC_FHC_PAUSE_MEM);//LewisToDo
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL1, 1, SPM_POWER_ON_VAL1_SC_FHC_PAUSE_MAIN);//LewisToDo 

    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL1, 1, SPM_POWER_ON_VAL1_SC_FHC_PAUSE_MEM);//LewisToDo
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL1, 1, SPM_POWER_ON_VAL1_SC_FHC_PAUSE_MAIN);//LewisToDo   

    //DDRPHY switch to SPM control
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_DPY_MODE_SW);

#if !(ENABLE_EMI_LPBK_TEST)
    //IO enter suspend{tie to specific value, SR state}
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_DMSUS_OFF);
#endif

#ifdef CHECK_CONF_FOR_DEBUG
    mcSHOW_DBG_MSG(("[Enter]CHECK_CONF_FOR_DEBUG\n"));
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_CLK_MEM_SEL);//Switch clk to 26MHz
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_W_CHG_MEM);
    mcDELAY_US(1);
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_W_CHG_MEM);
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_DDRPHY_FB_CK_EN);
#endif

    //MEM clk off
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_MEM_CK_OFF);

    //DLL disable
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DPY_2ND_DLL_EN);
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DPY_DLL_EN);

    //All PI disable & CG enable{Aphy cg enable, Not do PI delay Calculation}
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DPY_DLL_CK_EN);

    //VREF disable, MCK8X CG enable and  MIDPI disable{MIDPI: PI of DLL}
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DPY_VREF_EN);

    return;
}

void SLT_FUNC_DDRPHY_DLL_OFF_INFRA_CG_EN_EXIT(DRAMC_CTX_T *p)
{
    //VREF enable, MCK8X CG disable and MIDPI enable
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_DPY_VREF_EN);
    //All PI enable & CG disable
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_DPY_DLL_CK_EN);

#ifdef CHECK_CONF_FOR_DEBUG
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_CLK_MEM_SEL);//Switch clk to mem_ck
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_W_CHG_MEM);
    mcDELAY_US(1);
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_W_CHG_MEM);
#endif
    //Set TOP feedback MCK to full frequency
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_DDRPHY_FB_CK_EN);

    //1st DLL enable
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_DPY_DLL_EN);
    //wait 70 MCK
    mcDELAY_US(1);
    //2nd DLL enable
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_DPY_2ND_DLL_EN);
    //wait 90 MCK
    mcDELAY_US(1);

    //DDRPHY switch to SW control
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DPY_MODE_SW);

    //mem_ck CG disable
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_MEM_CK_OFF);
    //Wait 150ns
    mcDELAY_US(1);

    //IO exit suspend
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DMSUS_OFF);

    //Release frequency hopping
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL1, 0, SPM_POWER_ON_VAL1_SC_FHC_PAUSE_MEM);//LewisToDo
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL1, 0, SPM_POWER_ON_VAL1_SC_FHC_PAUSE_MAIN);//LewisToDo  

#ifdef TEST_DQS_RETRY_CAPABILITY    
    Set_DQS_Gating_Value(p);//Minus 2 UI
#endif

    //Release self-refresh request
    SLT_Func_Emi_On(p);
    if(!u1IsLP4Family(p->dram_type))//LPDDR3
    {
        vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0 + SHIFT_TO_CHB_ADDR, 1, MISC_CG_CTRL0_RG_CG_DRAMC_CHB_CK_OFF);
    }
    //Set TOP feedback MCK to divided frequency
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DDRPHY_FB_CK_EN);

#if 0//TDQSCK_PRECALCULATION_FOR_DVFS
    //Joe@20171030: In sy_lvia, If use SPM_DRAMC_DPY_CLK_SW_CON2, you need to set SPM_DRAMC_DPY_CLK_SW_CON_SEL2 to 0.So we use VAL0 
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_DPHY_PRECAL_UP);
    //wait 80ns
    mcDELAY_US(1);
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DPHY_PRECAL_UP);
#endif

    return;
}

void SLT_Set_Spm_Poweron_Config_En(DRAMC_CTX_T *p)
{
    vIO32WriteFldMulti(SPM_POWERON_CONFIG_EN, P_Fld(0x1, POWERON_CONFIG_EN_BCLK_CG_EN)//SPM power on
            | P_Fld(0xB16, POWERON_CONFIG_EN_PROJECT_CODE));
}

void SLT_Set_Spm_Side_Band_Pinmux(DRAMC_CTX_T *p)
{
    vIO32WriteFldAlign(SPM_PCM_PWR_IO_EN, 0, PCM_PWR_IO_EN_FULL);
    vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON_SEL, 0x00ffffff, DRAMC_DPY_CLK_SW_CON_SEL_FULL); 
    vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON_SEL2, 0xffffffff, DRAMC_DPY_CLK_SW_CON_SEL2_FULL);
    vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON_SEL3, 0x000fffff, DRAMC_DPY_CLK_SW_CON_SEL3_FULL);
    vIO32WriteFldMulti(SPM_SPM_POWER_ON_VAL0, P_Fld(0x1, SPM_POWER_ON_VAL0_SC_DPY_2ND_DLL_EN)
                | P_Fld(0x1, SPM_POWER_ON_VAL0_SC_DPY_DLL_EN)
                | P_Fld(0x1, SPM_POWER_ON_VAL0_SC_DPY_DLL_CK_EN)
                | P_Fld(0x1, SPM_POWER_ON_VAL0_SC_DPY_VREF_EN)
                | P_Fld(0x1, SPM_POWER_ON_VAL0_SC_PHYPLL_EN));
}

void SLT_Low_Power_Scenarios_Init(DRAMC_CTX_T *p)
{
    SLT_Set_Spm_Poweron_Config_En(p);
    SLT_Set_Spm_Side_Band_Pinmux(p);
}

#if 0
#define TOTAL_RANK_NUMBER 2
#define WRITE_READ_1024_BYTES 1024
#define DDR_BASE_ADDR 0x40000000
#define APMCU_WRITE_PATTERN 0xffffffff
#define RANK0_MAX_SIZE_ETT		0x40000000ULL

static void SLT_Write_Ranks_With_1024bytes()
{
    unsigned int i = 0;
    unsigned int rank0_size = RANK0_MAX_SIZE_ETT;
    unsigned int current_rank = 0;

    for(current_rank = 0; current_rank<TOTAL_RANK_NUMBER; current_rank++)
    {
        mcSHOW_DBG_MSG(("1. Write golden patterns to Rank%d\n", current_rank));

        for(i=0; i<WRITE_READ_1024_BYTES; i+=4)
        {
            if (current_rank == 0)
            {               
                *(volatile unsigned int *)(DDR_BASE_ADDR+i) = APMCU_WRITE_PATTERN;
            }
            else
            {               
                *(volatile unsigned int *)(DDR_BASE_ADDR+i+rank0_size) = APMCU_WRITE_PATTERN;         
            }
        }
    }
    return;
}

void SLT_Read_And_Compare_Ranks_With_1024bytes()
{
    unsigned int i = 0;
    unsigned int rank0_size = RANK0_MAX_SIZE_ETT;
    unsigned int current_rank = 0;
    unsigned int read_val = 0;
    
    for(current_rank = 0; current_rank<TOTAL_RANK_NUMBER; current_rank++)
    {
        mcSHOW_DBG_MSG(("2. Check Rank%d data\n", current_rank));
        for(i=0; i<WRITE_READ_1024_BYTES; i+=4)
        {
            if (current_rank==0)
            {       
                read_val = *(volatile unsigned int *)(DDR_BASE_ADDR+i);
                if (read_val != APMCU_WRITE_PATTERN)
                {
                    mcSHOW_DBG_MSG(("\n\nCompare Error!! Rank0, Source %xh = %xh, not %xh!!\n", DDR_BASE_ADDR+i, read_val, APMCU_WRITE_PATTERN));
                    while(1);
                }
            }
            else if(current_rank==1)
            {
                read_val = *(volatile unsigned int *)(DDR_BASE_ADDR+i+rank0_size);
                if (read_val != APMCU_WRITE_PATTERN)
                {
                    mcSHOW_DBG_MSG(("\n\nCompare Error!! Rank1, Source %xh = %xh, not %xh!!\n", DDR_BASE_ADDR+i+rank0_size, read_val, APMCU_WRITE_PATTERN));
                    while(1);
                }
            }
        }
    }
    return;
}

void SLT_Access_Memory_Test(DRAMC_CTX_T *p)
{
#if EMI_USE_TA2//LOW_POWER_STRESS_TEST_WITH_TA2
    Do_Memory_Test_TA2(p);
#else
    SLT_Write_Ranks_With_1024bytes();
    SLT_Read_And_Compare_Ranks_With_1024bytes();
#endif    
}
#endif

#define IGNORE_MTCMOS_CHECK
int SLT_spm_mtcmos_ctrl_dpy_ch0_apmcu(DRAMC_CTX_T *p, int state)
{
    int err = 0;
    
    if (state == 0) 
    {   //! Power Down
        //power off SRAM group
        vIO32WriteFldAlign(SPM_DPY_PWR_CON, 0xF, DPY_PWR_CON_DPY_SRAM_PDN);
#ifndef IGNORE_MTCMOS_CHECK
        /* TINFO="Wait until  DPY_CH0_SRAM_PDN_ACK = 1" */
        while ((u4IO32ReadFldAlign(SPM_DPY_PWR_CON, DPY_PWR_CON_DPY_SRAM_PDN_ACK) & 1) !=  1) 
        {
        }
#endif

#if !(ENABLE_EMI_LPBK_TEST)
        /* TINFO="Set  PWR_ISO = 1" */
        vIO32WriteFldAlign(SPM_DPY_PWR_CON, 0x1, DPY_PWR_CON_DPY_PWR_ISO);
        /* TINFO="Set  PWR_CLK_DIS = 1" */
        vIO32WriteFldAlign(SPM_DPY_PWR_CON, 0x1, DPY_PWR_CON_DPY_PWR_CLK_DIS);
        /* TINFO="Set  PWR_RST_B = 0" */
        vIO32WriteFldAlign(SPM_DPY_PWR_CON, 0x0, DPY_PWR_CON_DPY_PWR_RST_B);
		/* TINFO="Set  PWR_ON = 0" */
        vIO32WriteFldAlign(SPM_DPY_PWR_CON, 0x0, DPY_PWR_CON_DPY_PWR_ON);
        /* TINFO="Set  PWR_ON_2ND = 0" */
        vIO32WriteFldAlign(SPM_DPY_PWR_CON, 0x0, DPY_PWR_CON_DPY_PWR_ON_2ND);
#endif
#ifndef IGNORE_MTCMOS_CHECK
        while(((u4IO32Read4B(SPM_PWR_STATUS) >> 2) & 1) || ((u4IO32Read4B(SPM_PWR_STATUS_2ND) >> 2) & 1))
        {

        }
#endif
    }
    else 
    {   /* STA_POWER_ON  1 */
        /* TINFO="Set  PWR_ON = 1" */
        vIO32WriteFldAlign(SPM_DPY_PWR_CON, 0x1, DPY_PWR_CON_DPY_PWR_ON);
        mcDELAY_US(2);
        
        /* TINFO="Set  PWR_ON_2ND = 1" */
        vIO32WriteFldAlign(SPM_DPY_PWR_CON, 0x1, DPY_PWR_CON_DPY_PWR_ON_2ND);
        mcDELAY_US(2);
#ifndef IGNORE_MTCMOS_CHECK
        /* TINFO="Wait until  PWR_STATUS = 1 and  PWR_STATUS_2ND = 1" */
        while(((u4IO32Read4B(SPM_PWR_STATUS) >> 2) & 1) != 1 || ((u4IO32Read4B(SPM_PWR_STATUS_2ND) >> 2) & 1) != 1)
        {

        }
#endif
        /* TINFO="Set  PWR_CLK_DIS = 0" */
        vIO32WriteFldAlign(SPM_DPY_PWR_CON, 0x0, DPY_PWR_CON_DPY_PWR_CLK_DIS);
        /* TINFO="Set  PWR_ISO = 0" */
        vIO32WriteFldAlign(SPM_DPY_PWR_CON, 0x0, DPY_PWR_CON_DPY_PWR_ISO);
        /* TINFO="Set  PWR_RST_B = 1" */
        vIO32WriteFldAlign(SPM_DPY_PWR_CON, 0x1, DPY_PWR_CON_DPY_PWR_RST_B);
        
        /* TINFO="Set SRAM_PDN = 0" */
        vIO32WriteFldAlign(SPM_DPY_PWR_CON, 0xE, DPY_PWR_CON_DPY_SRAM_PDN);
#ifndef IGNORE_MTCMOS_CHECK
        /* TINFO="Wait until  DPY_CH0_SRAM_PDN_ACK_BIT0 = 0" */
        while (u4IO32ReadFldAlign(SPM_DPY_PWR_CON, DPY_PWR_CON_DPY_SRAM_PDN_ACK) & 1)
        {
        }
#endif

        //Reg_Sync_Writel(DPY_CH0_PWR_CON, Reg_Readl(DPY_CH0_PWR_CON) & ~(0x1 << 9));
        vIO32WriteFldAlign(SPM_DPY_PWR_CON, 0xC, DPY_PWR_CON_DPY_SRAM_PDN);
#ifndef IGNORE_MTCMOS_CHECK
        /* TINFO="Wait until  DPY_CH0_SRAM_PDN_ACK_BIT0 = 0" */
        while (u4IO32ReadFldAlign(SPM_DPY_PWR_CON, DPY_PWR_CON_DPY_SRAM_PDN_ACK) >> 1 & 1)
        {
        }        
#endif

        //Reg_Sync_Writel(DPY_CH0_PWR_CON, Reg_Readl(DPY_CH0_PWR_CON) & ~(0x1 << 10));
        vIO32WriteFldAlign(SPM_DPY_PWR_CON, 0x8, DPY_PWR_CON_DPY_SRAM_PDN);
#ifndef IGNORE_MTCMOS_CHECK
        /* TINFO="Wait until  DPY_CH0_SRAM_PDN_ACK_BIT0 = 0" */
        while (u4IO32ReadFldAlign(SPM_DPY_PWR_CON, DPY_PWR_CON_DPY_SRAM_PDN_ACK) >> 2 & 1)
        {
        }
#endif

        //Reg_Sync_Writel(DPY_CH0_PWR_CON, Reg_Readl(DPY_CH0_PWR_CON) & ~(0x1 << 11));
        vIO32WriteFldAlign(SPM_DPY_PWR_CON, 0x0, DPY_PWR_CON_DPY_SRAM_PDN);
#ifndef IGNORE_MTCMOS_CHECK
        /* TINFO="Wait until  DPY_CH0_SRAM_PDN_ACK_BIT0 = 0" */
        while (u4IO32ReadFldAlign(SPM_DPY_PWR_CON, DPY_PWR_CON_DPY_SRAM_PDN_ACK) >> 3 & 1)
        {
        }
#endif        
        /* TINFO="Release bus protect" */
        //Reg_Sync_Writel(INFRA_TOPAXI_PROTECTEN_1_CLR,  DPY_CH0_PROT_BIT_MASK);
    }
    return err;
}

//S1--> S0{PLL disable, PHY MTCMOS off }
void SLT_FUNC_DDRPHY_INFRA_MTCMOS_OFF_ENTER(DRAMC_CTX_T *p)
{
    //PHYPLL off
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0x0, SPM_POWER_ON_VAL0_SC_PHYPLL_EN);

    //MPLL off
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0x1, SPM_POWER_ON_VAL0_SC_MPLLOUT_OFF);
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0x1, SPM_POWER_ON_VAL0_SC_MPLL_S_OFF);
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0x1, SPM_POWER_ON_VAL0_SC_MPLL_OFF);

    /*TINFO="DRAM : DPY power down, DRAMC_DPY_CLK_SW_CON_SEL2[30:28] = 3'b111, means turn off CHA, BCD will also turn off  "*/ 
    SLT_spm_mtcmos_ctrl_dpy_ch0_apmcu(p, 0);
    return;
}

void SLT_FUNC_DDRPHY_INFRA_MTCMOS_OFF_EXIT(DRAMC_CTX_T *p)
{
    SLT_spm_mtcmos_ctrl_dpy_ch0_apmcu(p, 1);
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_MPLLOUT_OFF);
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_MPLL_S_OFF);
    mcDELAY_US(20);
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_MPLLOUT_OFF);

    //Joe@20171030: In sy_lvia, If use SPM_DRAMC_DPY_CLK_SW_CON2, you need to set SPM_DRAMC_DPY_CLK_SW_CON_SEL2 to 0.So we use VAL0 
    /*TINFO="DRAM : set sc_dpy_bclk_en = 1"*/
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_DPY_BCLK_ENABLE);

    mcDELAY_US(1);
    /*TINFO="DRAM : set sc_shu_restore_en = 1"*/
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_SHU_RESTORE);

    //wait 80ns
    mcDELAY_US(1);
    /*TINFO="DRAM : set sc_shu_restore_en = 0"*/
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_SHU_RESTORE);

    //wait 195ns
    mcDELAY_US(1);
    /*TINFO="DRAM : set sc_dpy_bclk_en = 0"*/
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DPY_BCLK_ENABLE);

    //PHYPLL on
    ///TODO: (Step 10) PHYPLL on{PHYPLL  || CLRPLL}
    /* HW will use this bit AND normal path signal(PHYPLL2_MODE_SW_PCM, PHYPLL_MODE_SW_PCM) to decide which PLL is on/off */
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_PHYPLL_EN);

    //wait 20us
    ///TODO: (Step 11) wait 20us
    mcDELAY_US(20);
    
    return;
}

void SLT_Func_Apsrc_Sleep (DRAMC_CTX_T *p, int intoS0)
{
    mcSHOW_DBG_MSG(("[Start]Func_Apsrc_Sleep\n"));
    mcDELAY_US(12);

    SLT_Func_Emi_Off(p);
    //------------
    // S-idle-->S1-->S0
    //------------
    SLT_FUNC_DDRPHY_DLL_OFF_INFRA_CG_EN_ENTER(p);
    
    if(intoS0 == 1)
    {
        SLT_FUNC_DDRPHY_INFRA_MTCMOS_OFF_ENTER(p);
        mcDELAY_US(1);
    }
    mcSHOW_DBG_MSG(("[Stop]Func_Apsrc_Sleep\n"));
    return;
}

void SLT_Func_Apsrc_Wakeup(DRAMC_CTX_T *p, int fromS0)
{
    mcSHOW_DBG_MSG(("[Start]Func_Apsrc_Wakeup\n"));
    
    if(fromS0 == 1)
    {
        SLT_FUNC_DDRPHY_INFRA_MTCMOS_OFF_EXIT(p);
        mcDELAY_US(1);
    }
    SLT_FUNC_DDRPHY_DLL_OFF_INFRA_CG_EN_EXIT(p);
    mcDELAY_US(1);

#if 0
    //-- Signal MD/MD32/MD2/CONN resource ready
    Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL1, Reg_Readl(PDEF_SPM_POWER_ON_VAL1) | (1U << PDEF_MD_APSRC_ACK_LSB));
#endif

    return;
}

void SLT_Suspend_Low_Power_Scenario(DRAMC_CTX_T *p)
{
    int intoS0 = gSLT_LS;
    mcSHOW_DBG_MSG(("Low Power Scenario \033[1;32mS%d\033[m\n",intoS0?0:1));
    SLT_Func_Apsrc_Sleep(p, intoS0);
}

void SLT_Resume_Low_Power_Scenario(DRAMC_CTX_T *p)
{
    int fromS0 = gSLT_LS;
    SLT_Func_Apsrc_Wakeup(p, fromS0);
}

void SLT_Low_Power_Scenarios_Test(DRAMC_CTX_T *p, U8 dfs_idx)
{
    int i = 0;
    int testCnt = 0;
    U32 u4value = 0;
    U8 shu_num=3,shu_idx[3] = {9, 3, 0};//9:1600, 3:3733, 0:3200
    
    SLT_Low_Power_Scenarios_Init(p);
    
    //gVcore = pmic_vcore_voltage_read();//Lewis@20151110: Use Vcore and (Vcore-100) mV to do DVS

    testCnt = 10;
    for(i=0; i<testCnt;i++)
    {
        mcSHOW_DBG_MSG(("**** Start Low Power scenarios DDR[\033[1;32m%d\033[m]****\n", (GetFreqBySel(p, gFreqTbl[dfs_idx].freq_sel)<<1)));
        mcSHOW_DBG_MSG(("**** Test[\033[1;32m%d\033[m] ****\n", i+1));

        EMI_LPBK_memory_test_start(p, 0); //Emi lpbk test
        //SLT_Access_Memory_Test(p);

        /* Suspend */        
        mcSHOW_DBG_MSG(("**** Suspend\n"));
        SLT_Suspend_Low_Power_Scenario(p);

        /* Resume */
        mcSHOW_DBG_MSG(("**** Resume\n"));
        SLT_Resume_Low_Power_Scenario(p);

#if 0
        dfs_idx = shu_idx[i%shu_num];
#else
        if (dfs_idx==0)
            dfs_idx = 3;
        else
            dfs_idx = 0;
#endif

#if FT_DSIM_USED
        DramcDFSDirectJump_RGMode(p, gFreqTbl[dfs_idx].shuffleIdx);
#else
        //DVsim use DramcDFSDirectJump_SRAMShuRGMode()
        //DramcDFSDirectJump_SRAMShuRGMode(p, gFreqTbl[dfs_idx].shuffleIdx);
        DramcDFSDirectJump_SPMMode(p, gFreqTbl[dfs_idx].shuffleIdx);
        mcSHOW_DBG_MSG(("**** DFS to DDR[\033[1;32m%d\033[m]****\n", (GetFreqBySel(p, gFreqTbl[dfs_idx].freq_sel)<<1)));
        //DFSTestProgramForLowPower(p);
        //DFSTestProgram(p, 0);//Since S0 + DVFS will lead CQDMA could not finished(only in M17 project).{Refer to M17_toogle_mode_issue.pptx}    
#endif

        EMI_LPBK_memory_test_start(p, 0); //Emi lpbk test
        //SLT_Access_Memory_Test(p);
        
        mcSHOW_DBG_MSG(("**** Stop Power scenarios test End ****\n"));
        mcSHOW_DBG_MSG(("\n\n\n\n\n\n"));

        gSLT_LS = (gSLT_LS+1)&1;
    }
    //Dma_Test(p);
}
#endif //#if (fcFOR_CHIP_ID == fcLafite)
#endif //#if (ENABLE_EMI_LPBK_TEST)

//======================== Low_Power_Scenarios_Test End =====================================


//======================== EMI LPBK TEST Definition Begin ===================================
#if (ENABLE_EMI_LPBK_TEST)

U8  gEmiLpbkTest = 0;

void Set_Emi_Lpbk_Test_Mode(U8 u1OnOff)
{
    gEmiLpbkTest = u1OnOff?1:0;
}

U32 DFS_Freq_Meter_Check(DRAMC_CTX_T *p, U32 ret_value)
{   
    //static 
    U8 first_time=1;
    U8 shu;
    U32 before_value, after_value, meter_value=0;
    U32 value, value_5_percent, test_result=1;
    
    //U32 LP3_freqmeter[3]={LP3_1866_freq_meter, LP3_1600_freq_meter, LP3_1200_freq_meter};  //Shuffle  0,1,2 1866/1600/1200

#if 0
#if defined(MTK_LP4_DDR3200)
    //#if defined(SLT)
    //U32 LP4_freqmeter[3]={LP4_2400_freq_meter, LP4_3200_freq_meter, LP4_1600_freq_meter};  //Shuffle  0,1,2 3200/2400/1600    
    //#else
//    U32 LP4_freqmeter[3]={LP4_3200_freq_meter, LP4_2400_freq_meter, LP4_1600_freq_meter};  //Shuffle  0,1,2 3200/2400/1600    
    //#endif
#elif LP4_HIGHEST_DDR3733
    //#if defined(SLT)
    //U32 LP4_freqmeter[3]={LP4_3200_freq_meter, LP4_3600_freq_meter, LP4_1600_freq_meter};  //Shuffle  0,1,2 3200/2400/1600    
    //#else
//    U32 LP4_freqmeter[3]={LP4_3600_freq_meter, LP4_3200_freq_meter, LP4_1600_freq_meter};  //Shuffle  0,1,2 3200/2400/1600    
    //#endif
#else
    U32 LP4_freqmeter[3]={LP4_3200_freq_meter, LP4_2400_freq_meter, LP4_1600_freq_meter};  //Shuffle  0,1,2 3200/2400/1600
#endif
#endif    

    U32 LP4_freqmeter[10] = {464, 299, 299, 191, 383, 299, 299, 199, 386, 386};

    shu = u4IO32ReadFldAlign(DRAMC_REG_SHUSTATUS, SHUSTATUS_SHUFFLE_LEVEL);
    shu = vGet_Current_ShuLevel(p);
    if (first_time)
    {
        DDRPhyFMeter_Init(p);
        //saint meter_value = DDRPhyFreqMeter();
        first_time = 0;
    }
    before_value = (meter_value>>16) & 0xffff;
    after_value = meter_value & 0xffff;

    if(u1IsLP4Family(p->dram_type))
    {
        value = LP4_freqmeter[shu];
        value_5_percent = (value * 5) / 100;

#if FT_DSIM_USED
        FT_DFS_FREQ_Meter_Compare_PassFail(p->shuffle_index, p->u1PhyPLLEn, before_value, after_value, value);
#else   
        /* pass criterio judgement */
        if ((before_value>=(value-value_5_percent) && before_value<=(value+value_5_percent)) && (after_value>=(value-value_5_percent) && after_value<=(value+value_5_percent)))
        {
            test_result = 1; /* pass */
        }
        else
        {
            mcSHOW_ERR_MSG(("Fail!! fmem_ck_bfe_dcm_ch0 FREQ=\033[1;36m%d!=%d\033[m\n", before_value, value));
            mcSHOW_ERR_MSG(("Fail!! fmem_ck_aft_dcm_ch0 FREQ=\033[1;36m%d!=%d\033[m\n", after_value, value)); 
            test_result = 0; /* fail */
        }    
#endif        
    }
#if 0
    else
    {
        value = LP3_freqmeter[shu];
        value_5_percent = (value * 5) / 100;

#if FT_DSIM_USED
        FT_DFS_FREQ_Meter_Compare_PassFail(p->shuffle_index, p->u1PhyPLLEn, before_value, after_value, value);
#else        
        /* pass criterio judgement */
        if ((before_value>=(value-value_5_percent) && before_value<=(value+value_5_percent)) && (after_value>=(value-value_5_percent) && after_value<=(value+value_5_percent)))
        {
            test_result = 1; /* pass */
        }
        else
        {
            mcSHOW_ERR_MSG(("Fail!! fmem_ck_bfe_dcm_ch0 FREQ=%d!=%d\n", before_value, value));
            mcSHOW_ERR_MSG(("Fail!! fmem_ck_aft_dcm_ch0 FREQ=%d!=%d\n", after_value, value));   
            test_result = 0; /* fail */
        }
#endif        
    }
#endif
    //mcSHOW_ERR_MSG(("now is [\033[1;36m%s\033[m]\n",p->u1PhyPLLEn?"PHYPLL":"CLRPLL"));
#if 0
    if(!p->u1PhyPLLEn)
    {
        mcSHOW_ERR_MSG(("now is [CLRPLL]\n"));
    }
    else
    {
        mcSHOW_ERR_MSG(("now is [PHYPLL]\n"));
    }
#endif

    if (test_result==1)
    {
        //saint GetPhyPllFrequency(p);
        mcSHOW_ERR_MSG(("DFS and Freq Meter test Pass!!\n\n"));
    }
    else
    {
        mcSHOW_ERR_MSG(("DFS and Freq Meter test Fail!!\n"));
        //go_ahead(p,0,0);    //saint      
#if defined(SLT)  
        //while(1);
#endif        
    }

    return test_result;
}

void SetRxDqDqsDelay_FT(DRAMC_CTX_T *p, U16 dqs_Delay, U16 dq_Delay)
{
    U8 ii, u1ByteIdx;
    U32 u4value;
    U8 dl_value[8];

    // Set DQS delay
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ6), dqs_Delay, SHU_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ6), dqs_Delay, SHU_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1);

    // Adjust DQM output delay.
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ6), dq_Delay, SHU_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ6), dq_Delay, SHU_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1);

    DramPhyReset(p);
    
    // Adjust DQ output delay.
    if(u1IsLP4Family(p->dram_type))
    {
        u4value = ((U32) dq_Delay) | (((U32)dq_Delay)<<16);
        for (ii=0; ii<4; ii++)
        {
            vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ2+ ii*4), u4value);//DQ0~DQ7
            vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ2+ ii*4), u4value);//DQ8~DQ15
        }
    }
    /*
    if(u1IsLP4Family(p->dram_type))
    {
        //u4value = ((U32) iDelay) | (((U32)iDelay)<<8) | (((U32)iDelay)<<16) | (((U32)iDelay)<<24);
        for (ii=0; ii<4; ii++)
        {
            SetRxDqDelay(p, ii, dq_Delay);
        }
    }
    */
}

#define TX_DQS 0
#define TX_DQ  1

void SetTxDqDqsDelay_FT(DRAMC_CTX_T *p, U16 Type, U16 Delay)
{
    U8 ii, u1ByteIdx;
    U32 u4value;
    U8 dl_value[8];

    if (Type==TX_DQS)
    {
        //adjust DQS dly
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS2), 0x0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ7), Delay, SHU_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);//DQS dly
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2), 0x0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ7), Delay, SHU_R0_B1_DQ7_RK0_ARPI_PBYTE_B1);
    }
    
    if (Type==TX_DQ)
    {
        //adjust DQ dly
        //vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS2), 0x0);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ7), P_Fld(Delay, SHU_R0_B0_DQ7_RK0_ARPI_DQM_B0)|P_Fld(Delay, SHU_R0_B0_DQ7_RK0_ARPI_DQ_B0));//DQ dly
        //vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2), 0x0);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ7), P_Fld(Delay, SHU_R0_B1_DQ7_RK0_ARPI_DQM_B1)|P_Fld(Delay, SHU_R0_B1_DQ7_RK0_ARPI_DQ_B1));//DQ dly
    }
}

void Reset_RxFIFO_DRAMCFIFO_FT(DRAMC_CTX_T *p)
{
    // When write data, please follow the flow
    // 1.	Reset rxfifo and dramc fifo
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 1, MISC_CTRL1_R_DMPHYRST);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DDRCONF0), 1, DDRCONF0_RDATRST);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 0, MISC_CTRL1_R_DMPHYRST);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DDRCONF0), 0, DDRCONF0_RDATRST);
    
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ3), 0, B0_DQ3_RG_ARDQ_RESETB_B0);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B0_DQ9), P_Fld(0, B0_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B0)|P_Fld(0, B0_DQ9_RG_RX_ARDQ_STBEN_RESETB_B0));
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ3), 0, B1_DQ3_RG_ARDQ_RESETB_B1);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B1_DQ9), P_Fld(0, B1_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B1)|P_Fld(0, B1_DQ9_RG_RX_ARDQ_STBEN_RESETB_B1));
    
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ3), 1, B0_DQ3_RG_ARDQ_RESETB_B0);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B0_DQ9), P_Fld(1, B0_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B0)|P_Fld(1, B0_DQ9_RG_RX_ARDQ_STBEN_RESETB_B0));
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ3), 1, B1_DQ3_RG_ARDQ_RESETB_B1);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B1_DQ9), P_Fld(1, B1_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B1)|P_Fld(1, B1_DQ9_RG_RX_ARDQ_STBEN_RESETB_B1));
    
    mcDELAY_US(1); //MUST need to dly 1us
}

void SetCommomSetting_FT(DRAMC_CTX_T *p)
{
    U8 ddrphy_drvpn, ddrphy_odtn;

    //set Tx DQ/DQS driving
    ddrphy_drvpn = 10;
    ddrphy_odtn = 12;
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING1), P_Fld(ddrphy_drvpn, SHU_DRVING1_DQSDRVP2)|
                                                   P_Fld(ddrphy_drvpn, SHU_DRVING1_DQSDRVN2)|
                                                   P_Fld(ddrphy_drvpn, SHU_DRVING1_DQSDRVP1)|
                                                   P_Fld(ddrphy_drvpn, SHU_DRVING1_DQSDRVN1)|
                                                   P_Fld(ddrphy_drvpn, SHU_DRVING1_DQDRVP2)|
                                                   P_Fld(ddrphy_drvpn, SHU_DRVING1_DQDRVN2));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING2), P_Fld(ddrphy_drvpn, SHU_DRVING2_DQDRVP1)|
                                                   P_Fld(ddrphy_drvpn, SHU_DRVING2_DQDRVN1)|
                                                   P_Fld(ddrphy_drvpn, SHU_DRVING2_CMDDRVP2)|
                                                   P_Fld(ddrphy_drvpn, SHU_DRVING2_CMDDRVN2)|
                                                   P_Fld(ddrphy_drvpn, SHU_DRVING2_CMDDRVP1)|
                                                   P_Fld(ddrphy_drvpn, SHU_DRVING2_CMDDRVN1));
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING3), P_Fld(0, SHU_DRVING3_DQSODTP2)|
                                                   P_Fld(ddrphy_odtn, SHU_DRVING3_DQSODTN2)|
                                                   P_Fld(0, SHU_DRVING3_DQSODTP)|
                                                   P_Fld(ddrphy_odtn, SHU_DRVING3_DQSODTN)|
                                                   P_Fld(0, SHU_DRVING3_DQODTP2)|
                                                   P_Fld(ddrphy_odtn, SHU_DRVING3_DQODTN2));
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING4), P_Fld(0, SHU_DRVING4_DQODTP1)|
                                                   P_Fld(ddrphy_odtn, SHU_DRVING4_DQODTN1)|
                                                   P_Fld(0, SHU_DRVING4_CMDODTP2)|
                                                   P_Fld(ddrphy_odtn, SHU_DRVING4_CMDODTN2)|
                                                   P_Fld(0, SHU_DRVING4_CMDODTP1)|
                                                   P_Fld(ddrphy_odtn, SHU_DRVING4_CMDODTN1));

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ8), 0x0, B0_DQ8_RG_TX_ARDQ_CAP_DET_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ8), 0x0, B1_DQ8_RG_TX_ARDQ_CAP_DET_B1);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_CA_CMD9), P_Fld(0x0, CA_CMD9_RG_TX_ARCMD_CAP_DET)
                                            | P_Fld(0x0, CA_CMD9_RG_TX_ARCMD_EN_CAP_LP4P));
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_CA_CMD0), 0x0, SHU_CA_CMD0_RG_TX_ARCLK_PRE_EN);
    
    // 
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DLL_ARPI1), 0x0, B0_DLL_ARPI1_RG_ARPISM_MCK_SEL_B0);    //set 0
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DLL_ARPI1), 0x0, B1_DLL_ARPI1_RG_ARPISM_MCK_SEL_B1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_DLL_ARPI1), 0x0, CA_DLL_ARPI1_RG_ARPISM_MCK_SEL_CA);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DLL_ARPI0), 1, B0_DLL_ARPI0_RG_ARPI_MCK8X_SEL_B0); // @YY DVT pass, set 1
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DLL_ARPI0), 1, B1_DLL_ARPI0_RG_ARPI_MCK8X_SEL_B1); // @YY
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_DLL_ARPI0), 1, CA_DLL_ARPI0_RG_ARPI_MCK8X_SEL_CA); // @YY

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ6), 0x0, SHU_B0_DQ6_RG_ARPI_OFFSET_MCTL_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B1_DQ6), 0x0, SHU_B1_DQ6_RG_ARPI_OFFSET_MCTL_B1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_CA_CMD6), 0x0, SHU_CA_CMD6_RG_ARPI_OFFSET_MCTL_CA);

}

void EMI_LPBK_TA2_test_start(DRAMC_CTX_T *p)
{
    TA2_Test_Run_Time_HW_Presetting(p, 0x10000, TA2_RKSEL_HW);  //TEST2_2_TEST2_OFF = 0x400
    TA2_Test_Run_Time_Pat_Setting(p, TA2_PAT_SWITCH_OFF);
    //TA2_Test_Run_Time_HW_Write(p, ENABLE);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), 1, TEST2_3_TEST2W);
}

void EMI_LPBK_memory_test_start(DRAMC_CTX_T * p, U8 K_win)
{
    static U32 shu_failcnt[2][3] = {{0,0,0}, {0,0,0}};
    U8 channel_idx, rank_idx, backup_channel, backup_rank, K_Rank_Num;
    U32 shu, u4dq_o1, u4ca_o1, u4fail_up=0, u4fail_dn=0, loop=0, fail_cnt=0, retry_dn_cnt=0, retry_up_cnt=0;
    U32 rx_enable=1, tx_enable=1;
    U32 Tx_dqs_dly=0;
    S32 win[2],first=1,last=1, pi_dly_step=1;
    U32 dq_center=0,dqs_center=0;
    unsigned long randSeed = 12345678;
    
    U8 ddrphy_drvn=9, ddrphy_drvp=3, ddrphy_odtn=14,dq_term_option,DisImpHw; 
    U8 emi_test_fail[2];

    //Rx variables
    S32 loop_w, k_type, len, pass = 0, win_size, k_again_dq=0, k_again_dqs=0, max_idx;
    U32 addr_w=0, addr_r=0, addr_s, pattern=0, u4value=0, step, k_cnt, w_addr, w_offset, win_center;
    U32 dq_Delay, dqs_Delay;
    U32 bit_error, dly, B0, B1;
    S32 addr_idx, addr_num, pa_idx, addr_offset, pi_dly;
    U32 k_addr[1] = 
        {
            0x40000024,
#if 0
            0x56000000,
#endif
        };
    U32 test_addr[4] = 
        {
            0x40000000,
            0x40024000,
            0x56000000,
            0x5E000000,
#if 0
            0x40024000, //360, 
            0x56000000, //saint 713
            0x5E000000, //saint 713
            0x60000000, //saint 713
            0x40026000, //365
            0x7ffffe00,
            0x55aaaa00,
            0x40024000, //360, 
            0x40024008, //466, 398, 77
            0x40024038, //360, 
            0x40024108, //7
            0x40024100, //345, 380
            0x40024400, //405
            0x40024A00, //405
            0x40025600, //405
            0x40026000, //365
#endif
        };
        emi_test_fail[0] = emi_test_fail[1] = 0;

    shu = u4IO32ReadFldAlign(DRAMC_REG_SHUSTATUS, SHUSTATUS_SHUFFLE_LEVEL);
    K_Rank_Num = (EMI_LPBK_K_RK1==1)?p->support_rank_num:1;
    
#if EMI_INT_LPBK_WL_DQS_RINGCNT==0     
    //saint GetPhyPllFrequency(p);
#if !FOR_DV_SIMULATION_USED
    mcDELAY_US(2);   
#endif
    mcSHOW_ERR_MSG(("[EMI Internal LPBK] Shu: %d, DDR%d\n",shu,p->frequency<<1));
    //saint mcSHOW_ERR_MSG(("Vcore: %d\n",(int)pmic_vcore_voltage_read()));
#endif    
    backup_channel = vGetPHY2ChannelMapping(p);

#if EMI_INT_LPBK_WL_DQS_RINGCNT
    rx_enable=0;
    tx_enable=0;
#endif
#if !EMI_LPBK_K_TX
    tx_enable=0;
#endif

#if EMI_INT_LPBK_WL_DQS_RINGCNT
    mcSHOW_ERR_MSG(("EMI_Internal_LPBK_DQS_RingCounter_Test\n"));
#elif EMI_LPBK_USE_THROUGH_IO
    mcSHOW_ERR_MSG(("EMI_Internal_LPBK_Through_IO_Test\n"));
#elif EMI_LPBK_ADDRESS_DEFECT
    mcSHOW_ERR_MSG(("EMI_Internal_LPBK_Address_Defect_Test\n\t7W: address = 0x70000000\n"));
#else
    mcSHOW_ERR_MSG(("EMI_Internal_LPBK_Test\n"));
#endif



    for(channel_idx=CHANNEL_A; channel_idx<p->support_channel_num; channel_idx++)
    //for(channel_idx=CHANNEL_A; channel_idx<1; channel_idx++)
    {
        vSetPHY2ChannelMapping(p, channel_idx);

        if (u1IsLP4Family(p->dram_type))
        {
            if (K_win)
            {
                // EMI internal lpbk initial
                #if FOR_DV_SIMULATION_USED
                mcSHOW_ERR_MSG(("=== EMI Intlpbk Init, DDR%d, CH_%c\n",p->frequency<<1,'A'+channel_idx));
                #else
                mcSHOW_ERR_MSG(("=== EMI Intlpbk Init, DDR%d, CH_%c\n",DDRPhyFMeter()<<1,'A'+channel_idx));
                #endif
                //mcSHOW_ERR_MSG(("=== CH_%c, O1 Path initial setting\n",'A'+channel_idx));
                //vIO32WriteFldAlign_All((DRAMC_REG_PADCTRL), 1, PADCTRL_DRAMOEN);
                //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ2), 0x1, B0_DQ2_RG_TX_ARDQ_OE_DIS_B0);
                //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ2), 0x1, B1_DQ2_RG_TX_ARDQ_OE_DIS_B1);

#if (ENABLE_PRE_POSTAMBLE==0)
                //[La_fite only] disable preamble/postamble
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_WODT), 0x0, SHU_WODT_WPST1P5T);// 0:0.5T, 1:1.5T write post-amble
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_CONF2), 0x0, SHU_CONF2_WPRE2T);// 0:1T, 1:2T write pre-amble
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_MISCTL0), P_Fld(0x1, MISCTL0_WPRE0T)|P_Fld(0x1, MISCTL0_DRAMC_LOOP_BAK_EN)|P_Fld(0x1, MISCTL0_DRAMC_LOOP_BAK_CMP_EN));
#endif
                // add new RG setting for Sy_lvia/Ca_nnon O1Path pull dn/up setting
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_CMD5), 1, CA_CMD5_RG_RX_ARCMD_VREF_EN);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_CA_CMD5), 0xb, SHU_CA_CMD5_RG_RX_ARCMD_VREF_SEL);
                //vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MISC_EXTLB18), P_Fld(0x1, MISC_EXTLB18_R_LPBK_DQ_RX_MODE)|P_Fld(0x1, MISC_EXTLB18_R_LPBK_CA_RX_MODE));
                //vIO32WriteFldMulti_All((DDRPHY_MISC_EXTLB18), P_Fld(0x1, MISC_EXTLB18_R_LPBK_DQ_RX_MODE)|P_Fld(0x0, MISC_EXTLB18_R_LPBK_CA_RX_MODE));

                //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_CMD6), 0, CA_CMD6_RG_TX_ARCMD_DDR4_SEL);//for sim 

                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ8), 1, SHU_B0_DQ8_R_RMRX_TOPHY_CG_IG_B0);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B1_DQ8), 1, SHU_B1_DQ8_R_RMRX_TOPHY_CG_IG_B1);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_CA_CMD8), 1, SHU_CA_CMD8_R_RMRX_TOPHY_CG_IG_CA);

                //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_CMD3), 1, CA_CMD3_RG_RX_ARCMD_SMT_EN);
                //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_CMD5), 1, CA_CMD5_RG_RX_ARCMD_EYE_VREF_EN);

                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B0_DQ2), P_Fld(0x0, B0_DQ2_RG_TX_ARDQ_OE_DIS_B0)|P_Fld(0x1, B0_DQ2_RG_TX_ARDQM0_OE_DIS_B0));
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B1_DQ2), P_Fld(0x0, B1_DQ2_RG_TX_ARDQ_OE_DIS_B1)|P_Fld(0x1, B1_DQ2_RG_TX_ARDQM0_OE_DIS_B1));

#if 1//FT_DSIM_USED || FOR_DV_SIMULATION_USED
                //Ying-Yu suggest: for FT initial common setting
                SetCommomSetting_FT(p);
#endif

                //mcDELAY_MS(1);

                // 5. Set lpbk mode, force RX_DQSIEN free run
                // Set LPBK mode, 
                //vIO32WriteFldAlign_All((DDRPHY_MISC_EXTLB18), 1, MISC_EXTLB18_R_LPBK_DQ_RX_MODE);
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MISC_EXTLB18), P_Fld(0x1, MISC_EXTLB18_R_LPBK_DQ_RX_MODE)|P_Fld(0x0, MISC_EXTLB18_R_LPBK_CA_RX_MODE));

#if EMI_LPBK_USE_THROUGH_IO
                // Set LPBK_DQ_RX_MODE=0, EMI LPBK CA lpbk_en=1, DQ B0/B1 lpbk_en = 0
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), 0, B0_DQ6_RG_RX_ARDQ_LPBK_EN_B0);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), 0, B1_DQ6_RG_RX_ARDQ_LPBK_EN_B1);
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), P_Fld(0x1, MISC_CTRL1_R_DMRRESETB_I_OPT)|P_Fld(0x0, MISC_CTRL1_R_DMDA_RRESETB_I));

                /* Set IMP_VREF_SEL register field's value */
                //Sy_lvia: term: 0x1b, unterm: 0x1a
                //La_fite: 0x35
#if FT_DSIM_USED || FOR_DV_SIMULATION_USED
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_CA_CMD11), 0x35, SHU_CA_CMD11_RG_RIMP_VREF_SEL);
#else
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_CA_CMD11), 0x1b, SHU_CA_CMD11_RG_RIMP_VREF_SEL);
#endif
                // Set IMP driving: drvp, drvn, odtn
                //vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING1), 0xffffffff);
                //vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING2), 0xffffffff);
                //vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING3), 0xffffffff);
                //vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING4), 0xffffffff);
#if EMI_LPBK_USE_DDR_800
                //Ying-Yu:La_fite: DDR800 thru io: correct setting: use DDR1600, REV_BIT_23_FRATE_EN = 1, no pcw or postdiv adjustment
                //vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), 1, RG_ARDQ_REV_BIT_23_FRATE_EN);
                //vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), 1, RG_ARDQ_REV_BIT_23_FRATE_EN);
                //vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_CA_CMD6), 1, RG_ARCMD_REV_BIT_23_FRATE_EN);
#endif

#else
                // Set LPBK_DQ_RX_MODE=1, EMI LPBK CA, DQ B0/B1 lpbk_en = 1
                //vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_CA_CMD6), 1, CA_CMD6_RG_RX_ARCMD_LPBK_EN);
                vIO32WriteFldAlign_All((DDRPHY_B0_DQ6), 1, B0_DQ6_RG_RX_ARDQ_LPBK_EN_B0);
                vIO32WriteFldAlign_All((DDRPHY_B1_DQ6), 1, B1_DQ6_RG_RX_ARDQ_LPBK_EN_B1);

#if (!EMI_LPBK_DRAM_USED)
                ddrphy_drvp = 9;
                ddrphy_drvn = 9;
                ddrphy_odtn = 12;
                dq_term_option = (p->frequency>=1600)?1:0;
                DisImpHw = (p->frequency >= 1333)?0:1;
                
                vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING1), 0);
                vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING2), 0);
                vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING3), 0);
                vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING4), 0);
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING1), P_Fld(DisImpHw, SHU_DRVING1_DIS_IMPCAL_HW)|
                                                                           P_Fld(0, SHU_DRVING1_DIS_IMP_ODTN_TRACK)|
                                                                           P_Fld(ddrphy_drvp, SHU_DRVING1_DQSDRVP2)|
                                                                           P_Fld(ddrphy_drvn, SHU_DRVING1_DQSDRVN2)|
                                                                           P_Fld(ddrphy_drvp, SHU_DRVING1_DQSDRVP1)|
                                                                           P_Fld(ddrphy_drvn, SHU_DRVING1_DQSDRVN1)|
                                                                           P_Fld(ddrphy_drvp, SHU_DRVING1_DQDRVP2)|
                                                                           P_Fld(ddrphy_drvn, SHU_DRVING1_DQDRVN2));

                vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING2), P_Fld((!dq_term_option), SHU_DRVING2_DIS_IMPCAL_ODT_EN)|
                                                                           P_Fld(ddrphy_drvp, SHU_DRVING2_DQDRVP1)|
                                                                           P_Fld(ddrphy_drvn, SHU_DRVING2_DQDRVN1)|
                                                                           P_Fld(ddrphy_drvp, SHU_DRVING2_CMDDRVP2)|
                                                                           P_Fld(ddrphy_drvn, SHU_DRVING2_CMDDRVN2)|
                                                                           P_Fld(ddrphy_drvp, SHU_DRVING2_CMDDRVP1)|
                                                                           P_Fld(ddrphy_drvn, SHU_DRVING2_CMDDRVN1));

                vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING3), P_Fld(0, SHU_DRVING3_DQSODTP2)|
                                                                           P_Fld(ddrphy_odtn, SHU_DRVING3_DQSODTN2)|
                                                                           P_Fld(0, SHU_DRVING3_DQSODTP)|
                                                                           P_Fld(ddrphy_odtn, SHU_DRVING3_DQSODTN)|
                                                                           P_Fld(0, SHU_DRVING3_DQODTP2)|
                                                                           P_Fld(ddrphy_odtn, SHU_DRVING3_DQODTN2));

                vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING4), P_Fld(0, SHU_DRVING4_DQODTP1)|
                                                                           P_Fld(ddrphy_odtn, SHU_DRVING4_DQODTN1)|
                                                                           P_Fld(0, SHU_DRVING4_CMDODTP2)|
                                                                           P_Fld(ddrphy_odtn, SHU_DRVING4_CMDODTN2)|
                                                                           P_Fld(0, SHU_DRVING4_CMDODTP1)|
                                                                           P_Fld(ddrphy_odtn, SHU_DRVING4_CMDODTN1));
#endif
#endif  //EMI_LPBK_USE_THROUGH_IO

                // RX_DQSIEN_FORCE_ON_EN=1 free run
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DLL1), 1, SHU_B0_DLL1_RG_RX_DQSIEN_FORCE_ON_EN_B0);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B1_DLL1), 1, SHU_B1_DLL1_RG_RX_DQSIEN_FORCE_ON_EN_B1);

                vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0);
                vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_MRS), 0x329);
                vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1);

#if FT_DSIM_USED || FOR_DV_SIMULATION_USED
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ5), 0x29, SHU_B0_DQ5_RG_RX_ARDQ_VREF_SEL_B0);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B1_DQ5), 0x29, SHU_B1_DQ5_RG_RX_ARDQ_VREF_SEL_B1);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_CA_CMD5), 0x29, SHU_CA_CMD5_RG_RX_ARCMD_VREF_SEL);
#else
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ5), 0xe, SHU_B0_DQ5_RG_RX_ARDQ_VREF_SEL_B0);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B1_DQ5), 0xe, SHU_B1_DQ5_RG_RX_ARDQ_VREF_SEL_B1);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_CA_CMD5), 0xe, SHU_CA_CMD5_RG_RX_ARCMD_VREF_SEL);
#endif


                // 6. Set tx dqs/dq delay (now set 0)
#if 0
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ7), P_Fld(0, SHU_R0_B0_DQ7_RK0_ARPI_PBYTE_B0)|
                                                                         P_Fld(0, SHU_R0_B0_DQ7_RK0_ARPI_DQM_B0)|
                                                                         P_Fld(0, SHU_R0_B0_DQ7_RK0_ARPI_DQ_B0));
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ7), P_Fld(0, SHU_R0_B1_DQ7_RK0_ARPI_PBYTE_B1)|
                                                                         P_Fld(0, SHU_R0_B1_DQ7_RK0_ARPI_DQM_B1)|
                                                                         P_Fld(0, SHU_R0_B1_DQ7_RK0_ARPI_DQ_B1));
#endif
                Tx_dqs_dly = 0;

                //mcSHOW_ERR_MSG(("=== Tx_dqs_dly = %d\n",Tx_dqs_dly));

                backup_rank = u1GetRank(p);
                for(rank_idx=0; rank_idx<K_Rank_Num; rank_idx++)
                {
                    vSetRank(p, rank_idx);
                    SetTxDqDqsDelay_FT(p, TX_DQS, Tx_dqs_dly);
                }
                vSetRank(p, backup_rank);

                // 7. Enable CG
                //vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_MISC_CG_CTRL0), 0x3fc1f);
                //vIO32Write4B((DDRPHY_MISC_CG_CTRL0)+SHIFT_TO_CHB_ADDR, 0x3fc1f);
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_MISC_CG_CTRL0), 0x3fc1f);

                //releated golden setting, if enable, 
                //vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_CA_DLL_ARPI2), 0x0);
                //vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL3), 0x15001500);
                //vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B0_DQ7), 0x0);
                //vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B1_DQ7), 0x0);
                //vIO32Write4B((DDRPHY_B0_DQ7)+SHIFT_TO_CHB_ADDR, 0x0);
                //vIO32Write4B((DDRPHY_B1_DQ7)+SHIFT_TO_CHB_ADDR, 0x0);
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B0_DQ7), 0x0);
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B1_DQ7), 0x0);

                // Jouling add "adjust TX DQ SELPH", + disable R/W DBI define + remove MR3 bit[7:6]
                //Lin-Yi: TXD DQ/DQM/DQS MCK/UI delay setting(1:8 for all speed), 
                //DQ/DQS/DQM PI = 0 (RK*_ARPI_DQ_B*/ RK*_ARPI_PBYTE_B* / RK*_ARPI_DQM_B* = 6'b0),TX DQ/DQS will be align
                vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_DQS0), 0x33334444);
                vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_DQS1), 0x66661111);

                vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0), 0x33334444);
                vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ1), 0x33334444);
                vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ2), 0x66661111);
                vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ3), 0x66661111);

                vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK1_SELPH_DQ0), 0x33334444);
                vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK1_SELPH_DQ1), 0x33334444);
                vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK1_SELPH_DQ2), 0x66661111);
                vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK1_SELPH_DQ3), 0x66661111);

                // Joe Disable RODT
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_ODTCTRL), 0, SHU_ODTCTRL_ROEN);

                // Joe Disable write dbi
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_WODT), 0, SHU_WODT_DBIWR);
                
                // Joe Disable read dbi
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ7), P_Fld(0, SHU_B0_DQ7_R_DMDQMDBI_SHU_B0)|P_Fld(0, SHU_B0_DQ7_R_DMDQMDBI_EYE_SHU_B0));
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_B1_DQ7), P_Fld(0, SHU_B1_DQ7_R_DMDQMDBI_SHU_B1)|P_Fld(0, SHU_B1_DQ7_R_DMDQMDBI_EYE_SHU_B1));


#if EMI_LPBK_USE_THROUGH_IO
                if (channel_idx == CHANNEL_A)
                {
                    // channel A
                    vIO32Write4B(DDRPHY_B0_DQ2, 0);
                    vIO32Write4B(DDRPHY_B1_DQ2, 0);
                    vIO32Write4B(DDRPHY_CA_CMD2, 0x300000);
                    vIO32WriteFldMulti(DDRPHY_SHU_B0_DLL1, P_Fld(1, SHU_B0_DLL1_RG_READ_BASE_DQS_EN_B0) | P_Fld(1, SHU_B0_DLL1_RG_READ_BASE_DQSB_EN_B0));
                    vIO32WriteFldMulti(DDRPHY_SHU_B1_DLL1, P_Fld(1, SHU_B1_DLL1_RG_READ_BASE_DQS_EN_B1) | P_Fld(1, SHU_B1_DLL1_RG_READ_BASE_DQSB_EN_B1));
                    // channel B
                    vIO32Write4B(DDRPHY_B0_DQ2+SHIFT_TO_CHB_ADDR, 0xffffffff);
                    vIO32Write4B(DDRPHY_B1_DQ2+SHIFT_TO_CHB_ADDR, 0xffffffff);
                    vIO32Write4B(DDRPHY_CA_CMD2+SHIFT_TO_CHB_ADDR, 0xffffffff);
                    vIO32WriteFldMulti(DDRPHY_SHU_B0_DLL1+SHIFT_TO_CHB_ADDR, P_Fld(0, SHU_B0_DLL1_RG_READ_BASE_DQS_EN_B0) | P_Fld(0, SHU_B0_DLL1_RG_READ_BASE_DQSB_EN_B0));
                    vIO32WriteFldMulti(DDRPHY_SHU_B1_DLL1+SHIFT_TO_CHB_ADDR, P_Fld(0, SHU_B1_DLL1_RG_READ_BASE_DQS_EN_B1) | P_Fld(0, SHU_B1_DLL1_RG_READ_BASE_DQSB_EN_B1));
                }
                else
                {
                    vIO32Write4B(DDRPHY_B0_DQ2, 0xffffffff);
                    vIO32Write4B(DDRPHY_B1_DQ2, 0xffffffff);
                    vIO32Write4B(DDRPHY_CA_CMD2, 0xffffffff);
                    vIO32WriteFldMulti(DDRPHY_SHU_B0_DLL1, P_Fld(0, SHU_B0_DLL1_RG_READ_BASE_DQS_EN_B0) | P_Fld(0, SHU_B0_DLL1_RG_READ_BASE_DQSB_EN_B0));
                    vIO32WriteFldMulti(DDRPHY_SHU_B1_DLL1, P_Fld(0, SHU_B1_DLL1_RG_READ_BASE_DQS_EN_B1) | P_Fld(0, SHU_B1_DLL1_RG_READ_BASE_DQSB_EN_B1));
                    
                    vIO32Write4B(DDRPHY_B0_DQ2+SHIFT_TO_CHB_ADDR, 0);
                    vIO32Write4B(DDRPHY_B1_DQ2+SHIFT_TO_CHB_ADDR, 0);
                    vIO32Write4B(DDRPHY_CA_CMD2+SHIFT_TO_CHB_ADDR, 0x300000);
                    vIO32WriteFldMulti(DDRPHY_SHU_B0_DLL1+SHIFT_TO_CHB_ADDR, P_Fld(1, SHU_B0_DLL1_RG_READ_BASE_DQS_EN_B0) | P_Fld(1, SHU_B0_DLL1_RG_READ_BASE_DQSB_EN_B0));
                    vIO32WriteFldMulti(DDRPHY_SHU_B1_DLL1+SHIFT_TO_CHB_ADDR, P_Fld(1, SHU_B1_DLL1_RG_READ_BASE_DQS_EN_B1) | P_Fld(1, SHU_B1_DLL1_RG_READ_BASE_DQSB_EN_B1));

                }
#endif

                //Francis test , parameter same with LP4 calibration
                #if 0
                if (u1IsLP4Family(p->dram_type))
                {
                    vIO32WriteFldAlign_All((DDRPHY_B0_DQ2), 0x0, B0_DQ2_RG_TX_ARDQM0_OE_DIS_B0);
                    vIO32WriteFldAlign_All((DDRPHY_B1_DQ2), 0x0, B1_DQ2_RG_TX_ARDQM0_OE_DIS_B1);

                    vIO32WriteFldAlign_All((DDRPHY_B0_DQ5), 0x16, B0_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B0);
                    vIO32WriteFldAlign_All((DDRPHY_B1_DQ5), 0x16, B1_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B1); 

                    vIO32WriteFldAlign_All(DDRPHY_B0_DQ6, 0x1, B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0);
                    vIO32WriteFldAlign_All(DDRPHY_B1_DQ6, 0x1, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1);

                    vIO32WriteFldAlign_All(DDRPHY_SHU1_B0_DQ5, 0x4, SHU1_B0_DQ5_RG_RX_ARDQS0_DVS_DLY_B0);
                    vIO32WriteFldAlign_All(DDRPHY_SHU1_B1_DQ5, 0x4, SHU1_B1_DQ5_RG_RX_ARDQS0_DVS_DLY_B1);

                    vIO32WriteFldAlign_All((DDRPHY_SHU1_B0_DQ5), 0x17, SHU1_B0_DQ5_RG_RX_ARDQ_VREF_SEL_B0);
                    vIO32WriteFldAlign_All((DDRPHY_SHU1_B1_DQ5), 0x17, SHU1_B1_DQ5_RG_RX_ARDQ_VREF_SEL_B1);

                    vIO32WriteFldAlign_All(DDRPHY_SHU1_B0_DQ6, 0, SHU1_B0_DQ6_RG_ARPI_CAP_SEL_B0);
                    vIO32WriteFldAlign_All(DDRPHY_SHU1_B1_DQ6, 0, SHU1_B1_DQ6_RG_ARPI_CAP_SEL_B1);

                    vIO32WriteFldAlign_All(DDRPHY_SHU1_B0_DQ6, 3, SHU1_B0_DQ6_RG_ARPI_MIDPI_CAP_SEL_B0);
                    vIO32WriteFldAlign_All(DDRPHY_SHU1_B1_DQ6, 3, SHU1_B1_DQ6_RG_ARPI_MIDPI_CAP_SEL_B1);

                    vIO32WriteFldAlign_All(DDRPHY_SHU1_B0_DQ6, 0x181d, SHU1_B0_DQ6_RG_ARPI_RESERVE_B0);
                    vIO32WriteFldAlign_All(DDRPHY_SHU1_B1_DQ6, 0x181d, SHU1_B1_DQ6_RG_ARPI_RESERVE_B1);

                    //vIO32WriteFldAlign_All((DDRPHY_SHU1_B0_DLL1), 0x224, SHU1_B0_DLL1_RG_ARDQ_REV_B0);
                    //vIO32WriteFldAlign_All((DDRPHY_SHU1_B1_DLL1), 0x224, SHU1_B1_DLL1_RG_ARDQ_REV_B1);
                }
                #endif

                #if 0
                // Rx DQS/DQ delay (DDR1600)
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ5), 0x00500018);
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ2), 0x0B0B0B0B);
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ3), 0x07070B0B);
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ4), 0x07070A0A);
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ5), 0x09090707);
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ6), 0x00000909);
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ2), 0x0A0A0808);
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ3), 0x0B0B0A0A);
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ4), 0x08080A0A);
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ5), 0x0A0A0808);
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ6), 0x00000909);
                #endif
            }                



            first = last = 1;
#if FT_DSIM_USED
            win[0]=0;
            win[1]=0;
#else
            memset(&win, 0, sizeof(win));
#endif

//K Rx dely window
#if !EMI_INT_LPBK_WL_DQS_RINGCNT
if (rx_enable)
{
            if (K_win>0)
            {
                dq_center=0,dqs_center=0;
                backup_rank = u1GetRank(p);
                for(rank_idx=0; rank_idx<K_Rank_Num; rank_idx++)
                {
                    vSetRank(p, rank_idx);
                    SetRxDqDqsDelay_FT(p, dqs_center, dq_center);//saint
                }
                vSetRank(p, backup_rank);
                
                for (k_type=2;k_type<4;k_type++) //2: Adjust DQ dly, 3: Adjust DQS dly
                {
                    if (k_again_dq==1)
                    {
                        k_again_dq = 2;
                        k_type = 2;
                    }
                    if (k_again_dqs==1)
                    {
                        k_again_dqs = 2;
                    }
                    
                    for (addr_idx=0;addr_idx<1;addr_idx++) //use different address to K Tx DQ/DQS dly
                    {
                        addr_s = k_addr[addr_idx];
                        //len = 0x44000;  //0x10;
#if FT_DSIM_USED || FOR_DV_SIMULATION_USED
                        len = 0x20; //0x20000;//0x40;
#else
                        len = 0x200; //0x20000;//0x40;
#endif
                        step = 0x4;

#if !EMI_USE_TA2 || FOR_DV_SIMULATION_USED
                        pi_dly_step = 4;
#endif
                        if (k_type==3)
                        {
#if !EMI_USE_TA2 || FOR_DV_SIMULATION_USED
                            pi_dly_step = 8;
#endif
                            max_idx = 127;  //DQS
                        }
                        else
                            max_idx = 63;   //DQ

                        mcSHOW_ERR_MSG(("\n=== K Rx %s dly window, address: 0x%x, len= 0x%x, pi_dly_step= %d\n",(k_type==2)?"DQ":(k_type==3)?"DQS":(k_type==0)?"CLK":"CMD",(addr_s&~0x100)|(0x100*channel_idx),len,pi_dly_step));

                        //for (ii=0;ii<=max_idx;ii++) // PI dly
                        for (pi_dly=0;pi_dly<=max_idx;pi_dly+=pi_dly_step) //robin // PI dly
                        {
                            //pattern = (ii==0)?0:0xffffffff;
                            //pattern = (ii==0)?0:(ii==1)?0xffffffff:(ii==2)?0x55aa55aa:0xaaaa5555;
                            //pattern = (ii==3)?0:(ii==2)?0xffffffff:(ii==1)?0x55aa55aa:0xaaaa5555;
                            //pattern = 0x11223344;
                            //pattern = (ii==0)?0x55aa55aa:0xaaaa5555;
                            
                            backup_rank = u1GetRank(p);
                            for(rank_idx=0; rank_idx<K_Rank_Num; rank_idx++)
                            {
                                vSetRank(p, rank_idx);
                                if (k_type==3)
                                {
                                    //adjust DQS dly
                                    SetRxDqDqsDelay_FT(p, pi_dly, dq_center);
                                }
                                else if (k_type==2)
                                {
                                    //adjust DQ dly
                                    SetRxDqDqsDelay_FT(p, dqs_center, pi_dly);
                                }
                                else if (k_type==0)
                                {
                                    //adjust clk dly
                                }
                                else
                                {
                                    //adjust cmd dly
                                }
                            }
                            vSetRank(p, backup_rank);

                            //mcSHOW_ERR_MSG(("\nCH_%c, O%d pattern 0x%X: from 0x%x ~ 0x%x\n",'A'+channel_idx,pi_dly,pattern,addr_s,addr_s+len));
                            //mcSHOW_ERR_MSG(("\nCH_%c, from 0x%x ~ 0x%x\n",'A'+channel_idx,addr_s,addr_s+len));
#if EMI_USE_TA2
                            len = step; // for address 0xXXXXXXX0 only
#else
                            len = len;
#endif
                            for (addr_offset=0;addr_offset<len;addr_offset+=step)
                            {
#if !EMI_USE_TA2
                                addr_w = addr_s + addr_offset;
                                addr_w = (addr_w & (~0x100)) | 0x100*channel_idx;
                                if ((channel_idx==0)&&(addr_w&0x100)||(channel_idx==1)&&((addr_w&0x100)==0))
                                    continue;

                                pattern = (randSeed = 69069 * randSeed + 362437);
                                //mcSHOW_ERR_MSG(("[%d]pattern = 0x%x\n",addr_offset,pattern));
                                //pattern = 0x55aa55aa;
#endif //EMI_USE_TA2
                                // When write data, please follow the flow
                                // 1.	Reset rxfifo and dramc fifo
                                Reset_RxFIFO_DRAMCFIFO_FT(p);

#if EMI_USE_TA2
                                EMI_LPBK_TA2_test_start(p);
                                bit_error = TA2_Test_Run_Time_HW_Status(p);
                                pass = bit_error==0?1:0;
                                if (k_type==2)
                                {
                                    //mcSHOW_DBG_MSG(("[%d] === CH_%c, bit_error: 0x%x, K Rx DQ = %d, DQS = %d (%s)\n",pi_dly,'A'+channel_idx, bit_error,pi_dly,dqs_center,pass?"Pass":"Fail"));
                                }
                                else
                                {
                                    //mcSHOW_DBG_MSG(("[%d] === CH_%c, bit_error: 0x%x, K Rx DQ = %d, DQS = %d (%s)\n",pi_dly,'A'+channel_idx, bit_error,dq_center,pi_dly,pass?"Pass":"Fail"));
                                }
#else
                                {
                                    dly = 1;    //saint

                                    w_addr = addr_w & ~0x1f;
                                    w_offset = addr_w & 0x1f;

                                    addr_r = addr_w;
                                    if (w_offset < 0x10)
                                    {
                                #if (ENABLE_PRE_POSTAMBLE==1)
                                        for (loop_w=0;loop_w<7;loop_w++)
                                        {
                                        #if !FOR_DV_SIMULATION_USED
                                            mcDELAY_US(dly);
                                        #endif
                                            *((UINT32P)(addr_w)) = pattern;
                                        }
                                #endif
                                        #if !FOR_DV_SIMULATION_USED
                                            mcDELAY_US(dly);
                                        #endif
                                            *((UINT32P)(addr_w)) = pattern;
                                        #if !FOR_DV_SIMULATION_USED
                                            mcDELAY_US(2); 
                                        #endif
                                        u4value = *((UINT32P)(addr_r));
                                    }
                                    else if (w_offset>=0x10 && w_offset<0x20)
                                    {
                                #if (ENABLE_PRE_POSTAMBLE==1)
                                        for (loop_w=0;loop_w<9;loop_w++)
                                        {
                                        #if !FOR_DV_SIMULATION_USED
                                            mcDELAY_US(dly);
                                        #endif
                                            *((UINT32P)(addr_w)) = pattern;
                                        }
                                #endif
                                        #if !FOR_DV_SIMULATION_USED
                                            mcDELAY_US(dly);
                                        #endif
                                            *((UINT32P)(addr_w)) = pattern;
                                        #if !FOR_DV_SIMULATION_USED
                                            mcDELAY_US(2);
                                        #endif
                                        u4value = *((UINT32P)(addr_r));
                                    }
                                }

                                if (u4value!=pattern) 
                                    pass = 0;
                                else
                                    pass = 1;
#endif
                                {
                                    if (first&&pass)
                                    {
                                        //saint win[0] = win[1] = ii;
                                        win[0] = pi_dly;
                                        win[1] = max_idx;
                                        first = 0;
                                    }
                                    else if (first==0 && last && !pass)
                                    {
                                        win[1] = pi_dly-1;
                                        if (win[1]<0)
                                            win[1] = 0;
                                        last = 0;
                                        if (win[0] >= win[1])
                                        {
                                            first = 1;
                                            last = 1;
                                        }
                                    }
                                #if FOR_DV_SIMULATION_USED
                                    if (1)
                                    {
                                        //mcSHOW_ERR_MSG(("[%d] CH_%c, cmp: 0x%x, [%d]pattern: 0x%x, write addr: 0x%x, read addr: 0x%x, value: 0x%x (%s)\n",pi_dly,'A'+channel_idx,u4value^pattern,addr_offset>>4,pattern,addr_w,addr_r,u4value,pass?"Pass":"Fail"));
                                    }
                                #else
                                    if (!pass)
                                    {
                                        //mcSHOW_ERR_MSG(("[%d] CH_%c, cmp: 0x\033[1;36m%x\033[m, [%d]pattern: 0x%x, write addr: 0x%x, read addr: 0x%x, value: 0x%x (%s)\n",pi_dly,'A'+channel_idx,u4value^pattern,addr_offset>>4,pattern,addr_w,addr_r,u4value,pass?"Pass":"\033[1;36mFail\033[m"));
                                    }
                                #endif
                                    //break;
                                }
                                if (first==0 && last==0 && !pass)
                                    break;
                            }
                            if (first==0 && last==0 && !pass)
                                break;
                        }
                    
                        //mcSHOW_ERR_MSG(("\n=== saint %d ~ %d, size= \033[1;36m%d\033[m\n",win[0],win[1],win[1]-win[0]+1));
                        win_size = win[1]-win[0]+1;
                        if (win_size<0) win_size = 0 - win_size;
                        if (win_size >= 8)
                            break;
                        else
                        {
                            first = last = 1;
                            win[0] = win[1] = 0;
                        }
                    }
                    {
                        //U32 win_center;
                        win_center = ((win[1]+win[0])>>1);
                        #if FOR_DV_SIMULATION_USED
                        mcSHOW_ERR_MSG(("=== saint Rx %s %d ~ %d, size= %d\n",(k_type==2)?"DQ":(k_type==3)?"DQS":(k_type==0)?"CLK":"CMD",win[0],win[1],win[1]-win[0]+1));
                        #else
                        mcSHOW_ERR_MSG(("=== saint Rx %s %d ~ %d, size= \033[1;36m%d\033[m\n",(k_type==2)?"DQ":(k_type==3)?"DQS":(k_type==0)?"CLK":"CMD",win[0],win[1],win[1]-win[0]+1));
                        #endif

                        if (k_type==3)
                        {
                            //adjust DQS dly
                            dqs_center = win_center;
                            backup_rank = u1GetRank(p);
                            for(rank_idx=0; rank_idx<K_Rank_Num; rank_idx++)
                            {
                                vSetRank(p, rank_idx);
                                SetRxDqDqsDelay_FT(p, dqs_center, dq_center);
                            }
                            vSetRank(p, backup_rank);
                            
                            
                            if (dqs_center==0&&k_again_dqs==0)
                            {
                                dqs_center = 32;
                                mcSHOW_ERR_MSG(("\n=== saint Rx DQS K fail, adjust DQ dly = 32\n"));
                                backup_rank = u1GetRank(p);
                                for(rank_idx=0; rank_idx<K_Rank_Num; rank_idx++)
                                {
                                    vSetRank(p, rank_idx);
                                    SetRxDqDqsDelay_FT(p, dqs_center, 0);
                                }
                                vSetRank(p, backup_rank);
                                
                                k_again_dqs = 1;
                                k_type = 0;
                            }
                        }
                        else if (k_type==2)
                        {
                            //adjust DQ dly
                            dq_center = win_center;
                            backup_rank = u1GetRank(p);
                            for(rank_idx=0; rank_idx<K_Rank_Num; rank_idx++)
                            {
                                vSetRank(p, rank_idx);
                                SetRxDqDqsDelay_FT(p, dqs_center, dq_center);
                            }
                            vSetRank(p, backup_rank);
                            
                            if (dq_center==0&&k_again_dq==0)
                            {
                                dqs_center = 32;
                                mcSHOW_ERR_MSG(("\n=== saint Rx DQ K fail, adjust DQS dly = 32\n"));
                                backup_rank = u1GetRank(p);
                                for(rank_idx=0; rank_idx<K_Rank_Num; rank_idx++)
                                {
                                    vSetRank(p, rank_idx);
                                    SetRxDqDqsDelay_FT(p, dqs_center, dq_center);
                                }
                                vSetRank(p, backup_rank);
                                k_again_dq = 1;
                            }
                            
                            if ((dq_center!=0||dqs_center!=0)&&k_again_dq!=1)  
                            {
                                first = last = 1;
                                win[0] = win[1] = 0;
                                continue;//break;
                            }
                        }
                        first = last = 1;
                        win[0] = win[1] = 0;
                    }
                }
                mcSHOW_ERR_MSG(("\n=== Rx dqs_center = %d, dq_center = %d\n",dqs_center,dq_center));
            }
            
            if (K_win)
            {
                // Get Rx DQS delay
                dqs_Delay = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ6), SHU_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0);
                // Get Rx DQ/DQM delay
                dq_Delay = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ6), SHU_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0);
                #if FOR_DV_SIMULATION_USED
                mcSHOW_ERR_MSG(("\n=== Rx dly, DQ/DQM center: %d, DQS center: %d\n",dq_Delay,dqs_Delay));
                #else
                mcSHOW_ERR_MSG(("\n=== Rx dly, DQ/DQM center: \033[1;32m%d\033[m, DQS center: \033[1;32m%d\033[m\n",dq_Delay,dqs_Delay));
                #endif
            }
            //while(1);
}                
#endif
               
//K Tx dely window
#if EMI_LPBK_K_TX
#if !EMI_INT_LPBK_WL_DQS_RINGCNT
if (tx_enable)
{
            if (K_win>0)
            {
                dq_center=0,dqs_center=0;
                for (k_type=2;k_type<4;k_type++) //2: Adjust DQ dly, 3: Adjust DQS dly
                {
                    //mcSHOW_DBG_MSG(("fra k_type=%d =============================\n",k_type));            
                    if (k_again_dq==1)
                    {
                        k_again_dq = 2;
                        k_type = 2;
                    }
                    if (k_again_dqs==1)
                    {
                        k_again_dqs = 2;
                    }
                    for (addr_idx=0;addr_idx<1;addr_idx++) //use different address to K Tx DQ/DQS dly
                    {
                        addr_s = k_addr[addr_idx];
                        //len = 0x44000;  //0x10;
#if FT_DSIM_USED || FOR_DV_SIMULATION_USED
                        len = 0x20; //0x20000;//0x40;
#else
                        len = 0x200; //0x20000;//0x40;
#endif                
                        step = 0x4;

                        max_idx = 63;
#if !EMI_USE_TA2 || FOR_DV_SIMULATION_USED
                        pi_dly_step = 4;
#endif

                        mcSHOW_ERR_MSG(("\n=== K Tx %s dly window, address: 0x%x, len= 0x%x, pi_dly_step= %d\n",(k_type==2)?"DQ":(k_type==3)?"DQS":(k_type==0)?"CLK":"CMD",(addr_s&~0x100)|(0x100*channel_idx),len,pi_dly_step));
                        for (pi_dly=0;pi_dly<=max_idx;pi_dly+=pi_dly_step) //adjust pi_dly
                        {

                            backup_rank = u1GetRank(p);
                            for(rank_idx=0; rank_idx<K_Rank_Num; rank_idx++)
                            {
                                vSetRank(p, rank_idx);
                                if (k_type==3)
                                {
                                    //adjust DQS dly
                                    SetTxDqDqsDelay_FT(p, TX_DQS, pi_dly);
                                }
                                else if (k_type==2)
                                {
                                    //adjust DQ dly
                                    SetTxDqDqsDelay_FT(p, TX_DQ, pi_dly);
                                }
                                else if (k_type==0)
                                {
                                }
                                else
                                {
                                }
                            }
                            vSetRank(p, backup_rank);
                         
                            //mcSHOW_ERR_MSG(("\nCH_%c, O%d pattern 0x%X: from 0x%x ~ 0x%x\n",'A'+channel_idx,pi_dly,pattern,addr_s,addr_s+len));
                            //mcSHOW_ERR_MSG(("\nCH_%c, from 0x%x ~ 0x%x\n",'A'+channel_idx,addr_s,addr_s+len));
#if EMI_USE_TA2
                            len = step; // for address 0xXXXXXXX0 only
#else
                            len = len;
#endif
                            for (addr_offset=0;addr_offset<len;addr_offset+=step)
                            {
#if !EMI_USE_TA2
                                addr_w = addr_s + addr_offset;
                                addr_w = (addr_w & (~0x100)) | 0x100*channel_idx;
                                if ((channel_idx==0)&&(addr_w&0x100)||(channel_idx==1)&&((addr_w&0x100)==0))
                                    continue;
                                
                                pattern = (randSeed = 69069 * randSeed + 362437);
                                //mcSHOW_ERR_MSG(("[%d]pattern = 0x%x\n",addr_offset,pattern));
                                //pattern = 0x55aa55aa;
#endif //EMI_USE_TA2
                                // When write data, please follow the flow
                                // 1.	Reset rxfifo and dramc fifo
                                Reset_RxFIFO_DRAMCFIFO_FT(p);

#if EMI_USE_TA2
                                EMI_LPBK_TA2_test_start(p);
                                bit_error = TA2_Test_Run_Time_HW_Status(p);
                                pass = bit_error==0?1:0;
                                if (k_type==2)
                                {
                                    //mcSHOW_DBG_MSG(("[%d] === CH_%c, bit_error: 0x%x, K Tx DQ = %d, DQS = %d (%s)\n",pi_dly,'A'+channel_idx,bit_error,pi_dly,dqs_center, pass?"Pass":"Fail"));
                                }
                                else
                                {
                                    //mcSHOW_DBG_MSG(("[%d] === CH_%c, bit_error: 0x%x, K Tx DQ = %d, DQS = %d (%s)\n",pi_dly,'A'+channel_idx, bit_error,dq_center,pi_dly,pass?"Pass":"Fail"));
                                }
#else
                                {
                                    dly = 1;    //saint

                                    w_addr = addr_w & ~0x1f;
                                    w_offset = addr_w & 0x1f;

                                    addr_r = addr_w;
                                    if (w_offset < 0x10)
                                    {
                                #if (ENABLE_PRE_POSTAMBLE==1)
                                        for (loop_w=0;loop_w<7;loop_w++)
                                        {
                                            #if !FOR_DV_SIMULATION_USED
                                            mcDELAY_US(dly);
                                            #endif
                                            *((UINT32P)(addr_w)) = pattern;
                                        }
                                #endif
                                        #if !FOR_DV_SIMULATION_USED
                                            mcDELAY_US(dly);
                                        #endif
                                            *((UINT32P)(addr_w)) = pattern;
                                        #if !FOR_DV_SIMULATION_USED
                                            mcDELAY_US(2);
                                        #endif
                                        u4value = *((UINT32P)(addr_r));
                                    }
                                    else if (w_offset>=0x10 && w_offset<0x20)
                                    {
                                #if (ENABLE_PRE_POSTAMBLE==1)
                                        for (loop_w=0;loop_w<9;loop_w++)
                                        {
                                            #if !FOR_DV_SIMULATION_USED
                                            mcDELAY_US(dly);
                                            #endif
                                            *((UINT32P)(addr_w)) = pattern;
                                        }
                                #endif
                                        #if !FOR_DV_SIMULATION_USED
                                            mcDELAY_US(dly);
                                        #endif
                                            *((UINT32P)(addr_w)) = pattern;
                                        #if !FOR_DV_SIMULATION_USED
                                            mcDELAY_US(2);
                                        #endif
                                        u4value = *((UINT32P)(addr_r));
                                    }
                                }


                                //if (k_type==0 || k_type==1) mcSHOW_DBG_MSG(("Fra tx (0x%x, 0x%x), pi_dly=%d\n",u4value, pattern, pi_dly));
                                //if (pi_dly==25) mcSHOW_DBG_MSG(("Fra tx (0x%x, 0x%x), pi_dly=%d\n",u4value, pattern, pi_dly));


                                if (u4value!=pattern)
                                    pass = 0;
                                else
                                    pass = 1;
                                
#endif
                                {
                                    if (first&&pass)
                                    {
                                        //saint win[0] = win[1] = pi_dly;
                                        win[0] = pi_dly;
                                        win[1] = max_idx;
                                        first = 0;
                                    }
                                    else if (first==0 && last && !pass)
                                    {
                                        win[1] = pi_dly-1;
                                        if (win[1]<0)
                                            win[1] = 0;
                                        last = 0;
                                        if (win[0] >= win[1])
                                        {
                                            first = 1;
                                            last = 1;
                                        }
                                    }
                                #if FOR_DV_SIMULATION_USED
                                    if (1)
                                    {
                                        //mcSHOW_ERR_MSG(("[%d] CH_%c, cmp: 0x%x, [%d]pattern: 0x%x, write addr: 0x%x, read addr: 0x%x, value: 0x%x (%s)\n",pi_dly,'A'+channel_idx,u4value^pattern,addr_offset>>4,pattern,addr_w,addr_r,u4value,pass?"Pass":"Fail"));
                                    }
                                #else
                                    if (!pass)
                                    {
                                        //mcSHOW_ERR_MSG(("[%d] CH_%c, cmp: 0x\033[1;36m%x\033[m, [%d]pattern: 0x%x, write addr: 0x%x, read addr: 0x%x, value: 0x%x (%s)\n",pi_dly,'A'+channel_idx,u4value^pattern,addr_offset>>4,pattern,addr_w,addr_r,u4value,pass?"Pass":"\033[1;36mFail\033[m"));
                                    }
                                #endif
                                    //break;
                                }
                                //mcSHOW_ERR_MSG(("[%d]: first: %d, last: %d, win[0]: %d, win[1]: %d\n",pi_dly,first,last,win[0],win[1]));
                                if (first==0 && last==0 && !pass)
                                    break;
                            }
                            if (first==0 && last==0 && !pass)
                                break;
                        }
                
                    //mcSHOW_ERR_MSG(("\n=== saint %d ~ %d, size= \033[1;36m%d\033[m\n",win[0],win[1],win[1]-win[0]+1));
                    win_size = win[1]-win[0]+1;
                    if (win_size<0) win_size = 0 - win_size;
                    if (win_size >= 8)
                        break;
                    else
                    {
                        first = last = 1;
                        win[0] = win[1] = 0;
                    }
                }
                {
                    //U32 win_center;
                    win_center = ((win[1]+win[0])>>1);
                    #if FOR_DV_SIMULATION_USED
                    mcSHOW_ERR_MSG(("\n=== saint Tx %s %d ~ %d, size= %d\n",(k_type==2)?"DQ":(k_type==3)?"DQS":(k_type==0)?"CLK":"CMD",win[0],win[1],win[1]-win[0]+1));
                    #else
                    mcSHOW_ERR_MSG(("\n=== saint Tx %s %d ~ %d, size= \033[1;36m%d\033[m\n",(k_type==2)?"DQ":(k_type==3)?"DQS":(k_type==0)?"CLK":"CMD",win[0],win[1],win[1]-win[0]+1));
                    #endif
            if (k_type==3)
            {
                    dqs_center = win_center;
                    //adjust DQS dly
                    backup_rank = u1GetRank(p);
                    for(rank_idx=0; rank_idx<K_Rank_Num; rank_idx++)
                    {
                        vSetRank(p, rank_idx);
                        SetTxDqDqsDelay_FT(p, TX_DQS, dqs_center);
                    }
                    vSetRank(p, backup_rank);
                            
                    if (dqs_center==0&&k_again_dqs==0)
                    {
                        dq_center = 32;
                        mcSHOW_ERR_MSG(("\n=== saint Tx DQS K fail, adjust DQ dly = 32\n"));
                        backup_rank = u1GetRank(p);
                        for(rank_idx=0; rank_idx<K_Rank_Num; rank_idx++)
                        {
                            vSetRank(p, rank_idx);
                            SetTxDqDqsDelay_FT(p, TX_DQ, dq_center);
                        }
                        vSetRank(p, backup_rank);
                        k_again_dqs = 1;
                        k_type = 2;
                        goto _label_1;
                    }

            }
            else if (k_type==2)
            {
                    //adjust DQ dly
                    dq_center = win_center;
                    backup_rank = u1GetRank(p);
                    for(rank_idx=0; rank_idx<K_Rank_Num; rank_idx++)
                    {
                        vSetRank(p, rank_idx);
                        SetTxDqDqsDelay_FT(p, TX_DQ, dq_center);
                    }
                    vSetRank(p, backup_rank);
                    if (dq_center==0&&k_again_dq==0)
                    {
                        dqs_center = 32;
                        mcSHOW_ERR_MSG(("\n=== saint Tx DQ K fail, adjust DQS dly = 32\n"));
                        backup_rank = u1GetRank(p);
                        for(rank_idx=0; rank_idx<K_Rank_Num; rank_idx++)
                        {
                            vSetRank(p, rank_idx);
                            SetTxDqDqsDelay_FT(p, TX_DQS, dqs_center);
                        }
                        vSetRank(p, backup_rank);
                        k_again_dq = 1;
                    }

                    //if ((dq_center!=0||dqs_center!=0)&&k_again_dq!=1)
                    //    break;
                    if ((dq_center!=0||dqs_center!=0)&&k_again_dq!=1)  
                    {
                        first = last = 1;
                        win[0] = win[1] = 0;
                        continue;//break;
                    }
            }
            else if (k_type==0)
            {
                    //adjust CLK dly
                    vIO32WriteFldMulti(DDRPHY_SHU_R0_CA_CMD9 + (1<<16), P_Fld(win_center, SHU_R0_CA_CMD9_RG_RK0_ARPI_CLK)|P_Fld(0, SHU_R0_CA_CMD9_RG_RK0_ARPI_CMD)); 
            }
            else
            {
                    //adjust CMD dly
                    vIO32WriteFldMulti(DDRPHY_SHU_R0_CA_CMD9 + (1<<16), P_Fld(0, SHU_R0_CA_CMD9_RG_RK0_ARPI_CLK)|P_Fld(win_center, SHU_R0_CA_CMD9_RG_RK0_ARPI_CMD)); 
            }
    _label_1:
                    first = last = 1;
                    win[0] = win[1] = 0;
                }
                }
            }

            if (K_win)
            {
                //U32 dq_Delay, dqs_Delay;
                
                // Get Tx DQS per byte delay
                dqs_Delay = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ7), SHU_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);
                // Get Tx DQ/DQM per byte delay
                dq_Delay = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ7), SHU_R0_B0_DQ7_RK0_ARPI_DQM_B0);
                #if FOR_DV_SIMULATION_USED
                mcSHOW_ERR_MSG(("\n=== Tx dly, DQ/DQM center: %d, DQS center: %d\n",dq_Delay,dqs_Delay));
                #else
                mcSHOW_ERR_MSG(("\n=== Tx dly, DQ/DQM center: \033[1;32m%d\033[m, DQS center: \033[1;32m%d\033[m\n",dq_Delay,dqs_Delay));
                #endif
            }
            //while(1);
}                
#endif
#endif //EMI_LPBK_K_TX




#if EMI_INT_LPBK_WL_DQS_RINGCNT
{
            U32 drv = 12; //12;
            
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING1),
                                       P_Fld(drv, SHU_DRVING1_DQSDRVP2)|
                                       P_Fld(drv, SHU_DRVING1_DQSDRVN2)|
                                       P_Fld(drv, SHU_DRVING1_DQSDRVP1)|
                                       P_Fld(drv, SHU_DRVING1_DQSDRVN1));

            vIO32WriteFldAlign_All((DDRPHY_B0_DQ2), 1, B0_DQ2_RG_TX_ARDQ_OE_DIS_B0);
            vIO32WriteFldAlign_All((DDRPHY_B1_DQ2), 1, B1_DQ2_RG_TX_ARDQ_OE_DIS_B1);
            
            // When write data, please follow the flow
            // 1.   Reset rxfifo and dramc fifo
            Reset_RxFIFO_DRAMCFIFO_FT(p);

            //saint 710
            //go_ahead(0,0,"SLT 710 check DQS ? WL-1UI DDR800 EMI intlpbk thr io + 1CPU write");
            //U32 B0, B1, pattern, ii;
            B0 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_PHY_RGS_STBEN_B0), MISC_PHY_RGS_STBEN_B0_AD_RX_ARDQ0_STBEN_B0);
            B1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_PHY_RGS_STBEN_B1), MISC_PHY_RGS_STBEN_B1_AD_RX_ARDQ0_STBEN_B1);
            mcSHOW_DBG_MSG(("\nbefore === CH_%d, pattern: 0x%x, B0: 0x%x, B1: 0x%x\n",channel_idx,pattern,B0,B1));
#if EMI_USE_TA2
            {
                EMI_LPBK_TA2_test_start(p);
                bit_error = TA2_Test_Run_Time_HW_Status(p);
                pass = bit_error==0?1:0;
                //mcSHOW_DBG_MSG(("[%d] === CH_%c, bit_error: 0x%x (%s)\n",0,'A'+channel_idx, bit_error,pass?"Pass":"Fail"));
            }
#else
            pattern = 0xffffffff;
            *((volatile UINT32P)(0x40000000|(0x100*channel_idx))) = pattern;
#endif
            B0 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_PHY_RGS_STBEN_B0), MISC_PHY_RGS_STBEN_B0_AD_RX_ARDQ0_STBEN_B0);
            B1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_PHY_RGS_STBEN_B1), MISC_PHY_RGS_STBEN_B1_AD_RX_ARDQ0_STBEN_B1);
            mcSHOW_DBG_MSG(("after  === CH_%d, pattern: 0x%x, B0: 0x%x, B1: 0x%x\n",channel_idx,pattern,B0,B1));
            if ((B0!=0x3 && B0!=0x30)||(B1!=0x3 && B1!=0x30))
            {
                #if FOR_DV_SIMULATION_USED
                mcSHOW_DBG_MSG(("\n=== CH_%d, DQS Ring Cnt fail\n\n",channel_idx));
                #else
                mcSHOW_DBG_MSG(("\n=== CH_%d, DQS Ring Cnt \033[1;32mfail\033[m\n\n",channel_idx));
                #endif
                //go_ahead(p,0,0);
#if FT_DSIM_USED || FOR_DV_SIMULATION_USED
                emi_test_fail[channel_idx]++;
#endif
            }
            else
            {
                mcSHOW_DBG_MSG(("\n=== CH_%d, DQS Ring Cnt pass\n\n",channel_idx));
            }
            //2400 through io, SLT 710(B1 DQS1 fail) or good ic
            //WL-1UI, good ic : 0x30/0x30(5 times), 710: 0x30/0xC(5 times)
            continue; // for next channel
            while(1);
}
#endif


            //EMI Internal LPBK test
    #if (!EMI_LPBK_USE_DDR_800)
            if (K_win==0)
    #endif
            {
                mcSHOW_ERR_MSG(("\n===== Start EMI LPBK Test .... =====, %s\n",EMI_USE_TA2?"EMI_USE_TA2":"CPU Write"));
                addr_s = test_addr[0];
                //len = 0x44000;  //0x10;
#if FT_DSIM_USED || FOR_DV_SIMULATION_USED
                len = 0x20;//0x40;
#else
                len = 0x400;//0x40;
#endif                
                step = 0x4;

#if FOR_DV_SIMULATION_USED || EMI_USE_TA2
                addr_num = 1;
#else
                addr_num = 4;
#endif
                for (addr_idx=0;addr_idx<addr_num;addr_idx++)
                {
                    addr_s = test_addr[addr_idx];
                for (pa_idx=4;pa_idx<5;pa_idx++) // 0:O0 pattern, 1:O1 pattern
                {
#if !EMI_USE_TA2
                    //pattern = (randSeed = 69069 * randSeed + 362437);
                    //pattern = (ii==0)?randSeed:(ii==1)?0xffffffff:(ii==2)?0:(ii==3)?0xAAAAAAAA:0x55555555;
                    pattern = (pa_idx==0)?0:(pa_idx==1)?0xffffffff:(pa_idx==2)?0xAAAAAAAA:(pa_idx==3)?0x55555555:(randSeed = 69069 * randSeed + 362437);
#endif

#if EMI_USE_TA2
                    len = step;
#else
                    len = len;
#endif                 
                    //mcSHOW_ERR_MSG(("\nCH_%c, O%d pattern 0x%X: from 0x%x ~ 0x%x\n",'A'+channel_idx,ii,pattern,addr_s,addr_s+len));
                    mcSHOW_ERR_MSG(("\nCH_%c, from 0x%x ~ 0x%x, len= 0x%x, pattern: %d\n",'A'+channel_idx,addr_s,addr_s+len,len,pa_idx));

                    for (addr_offset=0;addr_offset<len;addr_offset+=step) // for address 0xXXXXXXX0 only
                    {
#if !EMI_USE_TA2
                        addr_w = addr_s + addr_offset;
                        //addr_w = (addr_w & (~0x100)) | 0x100*channel_idx;
                        if ((channel_idx==0)&&(addr_w&0x100)||(channel_idx==1)&&((addr_w&0x100)==0))
                            continue;

#if EMI_LPBK_ADDRESS_DEFECT
                        if (addr_w&0x10)
                            continue;
#endif
                        if (pa_idx==4)
                        {
                            pattern = (randSeed = 69069 * randSeed + 362437);
                        }

                        //static unsigned long randSeed=123456789;
                        //pattern = (randSeed = 69069 * randSeed + 362437);
                        //pattern = (ii==0)?randSeed:(ii==1)?0xffffffff:(ii==2)?0:(ii==3)0xAAAAAAAA:0x55555555;
                        //mcSHOW_ERR_MSG(("[%d]pattern = 0x%x\n",addr_offset,pattern));
#endif //!EMI_USE_TA2
                        // When write data, please follow the flow
                        // 1.	Reset rxfifo and dramc fifo
                        Reset_RxFIFO_DRAMCFIFO_FT(p);

#if EMI_USE_TA2
                        {
                            EMI_LPBK_TA2_test_start(p);
                            bit_error = TA2_Test_Run_Time_HW_Status(p);
                            pass = bit_error==0?1:0;
                            if (!pass)
                            {
                                mcSHOW_DBG_MSG(("[%d] === CH_%c, bit_error: 0x%x (%s)\n",addr_offset,'A'+channel_idx, bit_error,pass?"Pass":"Fail"));
                            }
                        }
#else //CPU WRITE

#if EMI_LPBK_ADDRESS_DEFECT
#define EMI_APB_BASE   0x10219000
#endif

                        {
                            //U32 dly = 2;
                            //U32 w_offset;
                            //U32 w_addr;
                            dly = 1;    //saint

                            w_addr = addr_w & ~0x1f;
                            w_offset = addr_w & 0x1f;

                            addr_r = addr_w;
                            if (w_offset < 0x10)
                            {
#if (ENABLE_PRE_POSTAMBLE==1)
                                for (loop_w=0;loop_w<7;loop_w++)
                                {
                                    #if !FOR_DV_SIMULATION_USED
                                    mcDELAY_US(dly);
                                    #endif
                                    if (loop_w<7)
#if EMI_LPBK_ADDRESS_DEFECT
                                    *((UINT32P)(0x70000000|(0x100*channel_idx))) = ~pattern;
#else                                        
                                    *((UINT32P)(addr_w)) = ~pattern;
#endif
                                }
#endif
                                    {
#if EMI_LPBK_ADDRESS_DEFECT
                                        *((UINT32P)(EMI_APB_BASE + 0x00000100)) = 0xFF005000; // M6/M7 read age = 0
                                        if (channel_idx==CHANNEL_A)
                                        {
                                            *((UINT32P)(CHN0_EMI_BASE + 0x00000050)) |= (1<<16);    //urgent read first
                                            *((UINT32P)(CHN0_EMI_BASE + 0x00000160))  = 0x100110f2; //rank grouping
                                            *((UINT32P)(CHN0_EMI_BASE + 0x00000160)) |= (1<<0);     //rank grouping
                                        }
                                        else
                                        {
                                            *((UINT32P)(CHN1_EMI_BASE + 0x00000050)) |= (1<<16);    //urgent read first
                                            *((UINT32P)(CHN1_EMI_BASE + 0x00000160))  = 0x100110f2; //rank grouping
                                            *((UINT32P)(CHN1_EMI_BASE + 0x00000160)) |= (1<<0);     //rank grouping
                                        }
#endif
                                        #if !FOR_DV_SIMULATION_USED
                                        mcDELAY_US(2);
                                        #endif
                                        *((UINT32P)(addr_w)) = pattern;
                                    }
                                
                                #if !FOR_DV_SIMULATION_USED
                                    mcDELAY_US(2);
                                #endif
                                u4value = *((UINT32P)(addr_r));

                            
#if EMI_LPBK_ADDRESS_DEFECT
                                #if !FOR_DV_SIMULATION_USED
                                mcDELAY_US(5);
                                #endif
                                if (channel_idx==CHANNEL_A)
                                    *((UINT32P)(CHN0_EMI_BASE + 0x00000160)) &= ~(1<<0);
                                else
                                    *((UINT32P)(CHN1_EMI_BASE + 0x00000160)) &= ~(1<<0);
#endif
                                //mcSHOW_ERR_MSG(("=== 7W1W1R done\n"));
                            }
                            else if (w_offset>=0x10 && w_offset<0x20)
                            {
#if (ENABLE_PRE_POSTAMBLE==1)
                                for (loop_w=0;loop_w<9;loop_w++)
                                {
                                    mcDELAY_US(dly);
                                    if (loop_w<9)
                                    *((UINT32P)(addr_w)) = ~pattern;
                                    else
                                    {
#if 0 //EMI_LPBK_ADDRESS_DEFECT
                                        *((UINT32P)(EMI_APB_BASE + 0x00000100)) = 0xFF005000; // M6/M7 read age = 0
                                        if (channel_idx==CHANNEL_A)
                                        {
                                            *((UINT32P)(CHN0_EMI_BASE + 0x00000050)) |= (1<<16);    //urgent read first
                                            *((UINT32P)(CHN0_EMI_BASE + 0x00000160))  = 0x100110f2; //rank grouping
                                            *((UINT32P)(CHN0_EMI_BASE + 0x00000160)) |= (1<<0);     //rank grouping
                                        }
                                        else
                                        {
                                            *((UINT32P)(CHN1_EMI_BASE + 0x00000050)) |= (1<<16);    //urgent read first
                                            *((UINT32P)(CHN1_EMI_BASE + 0x00000160))  = 0x100110f2; //rank grouping
                                            *((UINT32P)(CHN1_EMI_BASE + 0x00000160)) |= (1<<0);     //rank grouping
                                        }
#endif
                                    }
                                }
#endif
                                mcDELAY_US(dly);
                                *((UINT32P)(addr_w)) = pattern;
                                
                                mcDELAY_US(2);
                                u4value = *((UINT32P)(addr_r));

#if 0 //EMI_LPBK_ADDRESS_DEFECT
                                mcDELAY_US(5);
                                if (channel_idx==CHANNEL_A)
                                    *((UINT32P)(CHN0_EMI_BASE + 0x00000160)) &= ~(1<<0);
                                else
                                    *((UINT32P)(CHN1_EMI_BASE + 0x00000160)) &= ~(1<<0);
#endif
                                //mcSHOW_ERR_MSG(("=== 9W1W1R done\n"));
                            }
                        }

                        if (u4value!=pattern) 
                            pass = 0;
                        else
                            pass = 1;
#endif //EMI_USE_TA2
                        if (!pass)//robin
                        //if (!pass)
                        {
                            #if FOR_DV_SIMULATION_USED 
                                mcSHOW_ERR_MSG(("[%d]=== saint mem test CH_%c, (%s)\n\n\n",addr_offset,'A'+channel_idx,pass?"Pass":"Fail"));
                            #else
                            #if !EMI_USE_TA2
                                mcSHOW_ERR_MSG(("CH_%c, cmp: 0x\033[1;36m%x\033[m, [%d]pattern: 0x%x, write addr: 0x%x, read addr: 0x%x, value: 0x%x (%s)\n",'A'+channel_idx,u4value^pattern,addr_offset>>2,pattern,addr_w,addr_r,u4value,pass?"Pass":"\033[1;36mFail\033[m"));
                                break;
                            #endif
                            #endif
                            #if defined(SLT)
                                //while(1);
                            #endif
                            //shu_failcnt[channel_idx][shu]++;
                            //break;//robin
                        }
                        if (!pass)
                        {
                            emi_test_fail[channel_idx]++;
                        }
                        
                    }
                }
                }
            }
                //while(1);
        }

        mcSHOW_ERR_MSG(("=== CH_%c, test done\n\n\n",'A'+channel_idx));
    }
    for(channel_idx=0;channel_idx<p->support_channel_num;channel_idx++)
    {
#if EMI_INT_LPBK_WL_DQS_RINGCNT
        mcSHOW_ERR_MSG(("=== CH_%c, RingCnt fail: %d times (%s)\n",'A'+channel_idx,emi_test_fail[channel_idx],emi_test_fail[channel_idx]?"FAIL":"PASS"));
#else
        mcSHOW_ERR_MSG(("=== CH_%c, EMI LPBK fail: %d times (%s)\n",'A'+channel_idx,emi_test_fail[channel_idx],emi_test_fail[channel_idx]?"FAIL":"PASS"));
#endif
    }
    #if defined(SLT)
    if (emi_test_fail[0]||emi_test_fail[1])
        while(1);
    #endif
    //DramcRegDump(p); //fra

            
    //mcSHOW_ERR_MSG(("=== CH_%c, restore RG\n",'A'+channel_idx));
#if FT_DSIM_USED==0
    //saint DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));
#endif


#if EMI_INT_LPBK_WL_DQS_RINGCNT==0
    //mcSHOW_ERR_MSG(("=== Total err cnt:\t\t\t[0]:0x%x/0x%x, [1]:0x%x/0x%x, [2]:0x%x/0x%x\n\n\n",shu_failcnt[0][0],shu_failcnt[1][0],shu_failcnt[0][1],shu_failcnt[1][1],shu_failcnt[0][2],shu_failcnt[1][2]));
#endif


    //mcSHOW_ERR_MSG(("=== All Channel test done\n"));
    vSetPHY2ChannelMapping(p, backup_channel);
    //vIO32WriteFldAlign_All(DRAMC_REG_PADCTRL, 0, PADCTRL_DRAMOEN);
#if (EMI_LPBK_USE_DDR_800)
    //while(1);
#endif
    //while(1);
}

void EMI_LPBK_memory_test(DRAMC_CTX_T * p)
{
    U32 ii;
    U8 u1Org_Rank;

    mcSHOW_ERR_MSG(("\nline: %d, DDR%d\n",__LINE__,p->frequency<<1));


#if 1//defined(SLT) //slt preloader emi lpbk DFS/S0/S1
#if 0
DRAM_DFS_FREQUENCY_TABLE_T gFreqTbl[DRAM_DFS_SHUFFLE_MAX] = {
   {LP4_SHU8_FREQSEL /*3200*/, LP4_SHU8_FREQ, DIV8_MODE, DRAM_DFS_SRAM_SHU8, DUTY_USE_PREVIOUS_K_DELAY, VREF_CALI_OFF, IGNORE_LOOP_MODE},  //francis : can't change this shuffle's k position due to duty K
   {LP4_SHU6_FREQSEL /*2400*/, LP4_SHU6_FREQ, DIV8_MODE, DRAM_DFS_SRAM_SHU6, DUTY_USE_PREVIOUS_K_DELAY, VREF_CALI_OFF, IGNORE_LOOP_MODE},  //francis : can't change this shuffle's k position due to duty K  
   {LP4_SHU9_FREQSEL /*3200*/, LP4_SHU9_FREQ, DIV8_MODE, DRAM_DFS_SRAM_SHU9, DUTY_NEED_K,               VREF_CALI_OFF, IGNORE_LOOP_MODE},  //francis : can't change this shuffle's k position due to duty K
   {LP4_SHU0_FREQSEL /*3733*/, LP4_SHU0_FREQ, DIV8_MODE, DRAM_DFS_SRAM_SHU0, DUTY_NEED_K,               VREF_CALI_ON,  IGNORE_LOOP_MODE},  // highest freq of term group (3733) must k first.
   {LP4_SHU4_FREQSEL /*1600*/, LP4_SHU4_FREQ, DIV4_MODE, DRAM_DFS_SRAM_SHU4, DUTY_USE_DEFAULT_DELAY_0,  VREF_CALI_OFF, IGNORE_LOOP_MODE},
   {LP4_SHU2_FREQSEL /*1200*/, LP4_SHU2_FREQ, DIV4_MODE, DRAM_DFS_SRAM_SHU2, DUTY_USE_DEFAULT_DELAY_0,  VREF_CALI_OFF, IGNORE_LOOP_MODE},
   {LP4_SHU1_FREQSEL /*1200*/, LP4_SHU1_FREQ, DIV4_MODE, DRAM_DFS_SRAM_SHU1, DUTY_USE_DEFAULT_DELAY_0,  VREF_CALI_OFF, IGNORE_LOOP_MODE},
   {LP4_SHU7_FREQSEL /* 800*/, LP4_SHU7_FREQ, DIV4_MODE, DRAM_DFS_SRAM_SHU7, DUTY_USE_DEFAULT_DELAY_0,  VREF_CALI_OFF, DDR800_CLOSE_LOOP_MODE},
   {LP4_SHU5_FREQSEL /*2400*/, LP4_SHU5_FREQ, DIV8_MODE, DRAM_DFS_SRAM_SHU5, DUTY_NEED_K,               VREF_CALI_ON,  IGNORE_LOOP_MODE},// highest freq of unterm group (2400) must k first.
   {LP4_SHU3_FREQSEL /*1600*/, LP4_SHU3_FREQ, DIV8_MODE, DRAM_DFS_SRAM_SHU3, DUTY_USE_DEFAULT_DELAY_0,  VREF_CALI_ON,  IGNORE_LOOP_MODE},  //Darren: DDR1600 for MRW (DramcModeRegInit_LP4 and CBT)
};
DRAM_DFS_FREQUENCY_TABLE_T gFreqTbl[DRAM_DFS_SHUFFLE_MAX] = {
   {LP4_DDR3200 /*0*/, DIV8_MODE, SRAM_SHU8, DUTY_LAST_K,  VREF_CALI_OFF, NORMAL_CLOSE_LOOP},  //francis : can't change this shuffle's k position due to duty K
   {LP4_DDR2400 /*1*/, DIV8_MODE, SRAM_SHU6, DUTY_LAST_K,  VREF_CALI_OFF, NORMAL_CLOSE_LOOP},  //francis : can't change this shuffle's k position due to duty K  
   {LP4_DDR3200 /*2*/, DIV8_MODE, SRAM_SHU9, DUTY_NEED_K,  VREF_CALI_OFF, NORMAL_CLOSE_LOOP},  //francis : can't change this shuffle's k position due to duty K
   {LP4_DDR3733 /*3*/, DIV8_MODE, SRAM_SHU0, DUTY_NEED_K,  VREF_CALI_ON,  NORMAL_CLOSE_LOOP},  // highest freq of term group (3733) must k first.
   {LP4_DDR1600 /*4*/, DIV4_MODE, SRAM_SHU4, DUTY_DEFAULT, VREF_CALI_OFF, NORMAL_CLOSE_LOOP},
   {LP4_DDR1200 /*5*/, DIV4_MODE, SRAM_SHU2, DUTY_DEFAULT, VREF_CALI_OFF, NORMAL_CLOSE_LOOP},
   {LP4_DDR1200 /*6*/, DIV4_MODE, SRAM_SHU1, DUTY_DEFAULT, VREF_CALI_OFF, NORMAL_CLOSE_LOOP},
   {LP4_DDR800  /*7*/, DIV4_MODE, SRAM_SHU7, DUTY_DEFAULT, VREF_CALI_OFF, DDR800_CLOSE_LOOP},
   {LP4_DDR2400 /*8*/, DIV8_MODE, SRAM_SHU5, DUTY_NEED_K,  VREF_CALI_ON,  NORMAL_CLOSE_LOOP},// highest freq of unterm group (2400) must k first.
   {LP4_DDR1600 /*9*/, DIV8_MODE, SRAM_SHU3, DUTY_DEFAULT, VREF_CALI_ON,  NORMAL_CLOSE_LOOP},  //Darren: DDR1600 for MRW (DramcModeRegInit_LP4 and CBT)
};
#endif

    if (gEmiLpbkTest)
    {   //EMI LPBK Test + DFS
        U8 updown=0, dfs_idx, next_shu=0;
        U8 shu_num=3,shu_idx[5] = {9,0,3,2,1}; //  9:16, 3:3733, 2:32, 0:32, 1:24
        S32 ii;
        U8 u4RankIdx;

        for (ii=0;ii<shu_num;ii++)
        {
            dfs_idx = shu_idx[ii%shu_num];
            EnableDramcPhyDCM(p, 0);
            mcSHOW_DBG_MSG(("[\033[1;32m%d\033[m]=== saint K win, DDR\033[1;32m%d, 1:%d mode, %s\033[m\n",dfs_idx,GetFreqBySel(p,gFreqTbl[dfs_idx].freq_sel)<<1,gFreqTbl[dfs_idx].divmode==0?8:4,EMI_USE_TA2?"EMI_USE_TA2":"CPU Write"));
            EMI_LPBK_memory_test_start(p, 1);
            //EnableDramcPhyDCMShuffle(p, 1);
            
            TXPICGSetting(p);
            TXPICGNewModeEnable(p);
            
            mcSHOW_DBG_MSG(("=== saint DRAM_DFS_SHUFFLE_1 --> gFreqTbl[%d].shuffleIdx = %d\n\n\n",dfs_idx,gFreqTbl[dfs_idx].shuffleIdx));
            DramcSaveToShuffleSRAM(p, DRAM_DFS_SHUFFLE_1, gFreqTbl[dfs_idx].shuffleIdx); //p->MappingFreqArray[p->gDFSTable[u1ShuffleIdx].shuffleIdx]
            if (dfs_idx==9)//1600
            {
                LoadShuffleSRAMtoDramc(p, gFreqTbl[dfs_idx].shuffleIdx, DRAM_DFS_SHUFFLE_2);
            }

            //EMI_LPBK_memory_test_start(p, 0);

            if (ii==(shu_num-1))
                break;
        
            next_shu= shu_idx[(ii+1)%shu_num];
                
            vSetDFSFreqSelByTable(p, &gFreqTbl[next_shu]);
            vSet_Div_Mode(p, gFreqTbl[next_shu].divmode);
            vSet_Current_ShuLevel(p, gFreqTbl[next_shu].shuffleIdx); // for Vcore adjust
            //DramcInit(p);
            DramcSetting_Olympus_LP4_ByteMode(p);
            DramcModeRegInit_LP4(p);
            DdrUpdateACTiming(p);            
            vApplyConfigBeforeCalibration(p);

#if (EMI_USE_TA2==0)

            if (p->frequency >= 1600)
            {
                u1Org_Rank = p->rank;
            
                //switch to low freq
                if (p->dram_fsp == FSP_1)
                {
                    CBT_Switch_Freq(p, CBT_LOW_FREQ);
                }
            
                for(u4RankIdx =0; u4RankIdx < 2; u4RankIdx++)
                {
                    vSetRank(p, u4RankIdx);            

                    DramcMRWriteFldAlign(p, 13, 1, MR13_FSP_WR, TO_MR);

                    u1MR03Value[FSP_1] &= ~(1<<1); //Set write post-amble as 0.5 tck
                    DramcModeRegWriteByRank(p, u4RankIdx, 3, u1MR03Value[FSP_1]);
                    
                    DramcMRWriteFldAlign(p, 13, 0, MR13_FSP_WR, TO_MR);
                }

                p->rank = u1Org_Rank;
                
                //switch to high freq
                if (p->dram_fsp == FSP_1)
                {
                    CBT_Switch_Freq(p, CBT_HIGH_FREQ);
                }
                
                vIO32WriteFldAlign_All(DRAMC_REG_SHU_WODT, 0x0, SHU_WODT_WPST1P5T); //Set write post-amble by FSP with MR3
            }
#endif 
        }
        
        mcSHOW_DBG_MSG(("EnableDramcPhyDCM: on\n\n"));
        EnableDramcPhyDCM(p, 1);

        TransferToSPMControl(p);
        TransferPLLToSPMControl(p);

        // if use DramcDFSDirectJump_SRAMShuRGMode, please unmask following setting
        //vIO32WriteFldMulti_All((DDRPHY_MISC_SPM_CTRL3), P_Fld(gFreqTbl[dfs_idx].shuffleIdx, MISC_SPM_CTRL3_RG_DR_SHU_LEVEL_SRAM_CH1)
        //                        | P_Fld(gFreqTbl[dfs_idx].shuffleIdx, MISC_SPM_CTRL3_RG_DR_SHU_LEVEL_SRAM_CH0));

        //for(jj=0;jj<2;jj++)
        {
            // EMI LPBK + LS0/1
            SLT_Low_Power_Scenarios_Test(p, dfs_idx);

            // EMI LPBK + DFS
            for (ii=0;ii<10;ii++)
            {
                next_shu = shu_idx[ii%shu_num];
                if (GetFreqBySel(p,gFreqTbl[dfs_idx].freq_sel) > GetFreqBySel(p,gFreqTbl[next_shu].freq_sel))
                    updown = 0;
                else
                    updown = 1;

                mcSHOW_DBG_MSG(("\n\n\n updown: %d, dfs_idx(%d, freq %d) --> next(%d, freq %d)\n",updown,dfs_idx,GetFreqBySel(p,gFreqTbl[dfs_idx].freq_sel),next_shu,GetFreqBySel(p,gFreqTbl[next_shu].freq_sel)));

                dfs_idx = next_shu;

#if 0//ENABLE_DVS
                if(updown)
                {
                    SetDVSProgram(p);
                }
#endif

                DramcDFSDirectJump_SPMMode(p, gFreqTbl[dfs_idx].shuffleIdx);

                //DramcDFSDirectJump_SRAMShuRGMode(p, gFreqTbl[dfs_idx].shuffleIdx);
#if 0//ENABLE_DVS
                if(!updown)
                {
                    SetDVSProgram(p);
                }
#endif

                mcSHOW_DBG_MSG(("[\033[1;32m%d\033[m]=== saint mem test[%d], DDR\033[1;32m%d\033[m, 1:%d mode\n",dfs_idx,ii,GetFreqBySel(p,gFreqTbl[dfs_idx].freq_sel)<<1,gFreqTbl[dfs_idx].divmode==0?8:4));

                EMI_LPBK_memory_test_start(p, 0); //Emi lpbk test

            }
        }
    }

    #if 0
    if (gEmiLpbkTest)
    {   //EMI LPBK Test + S0/S1 + DFS
        U8 dfs_idx, dfs_1st_idx=9, dfs_2nd_idx=3, dfs_3rd_idx=0, next_shu=0;
        U8 shu_num=3,shu_idx[3] = {9, 3, 0};//9:1600, 3:3733, 0:3200
        U8 ii;

        for (ii=0; ii<shu_num; ii++)
        {
            dfs_idx = shu_idx[ii%shu_num];
            EnableDramcPhyDCM(p, 0);
            mcSHOW_DBG_MSG(("[\033[1;32m%d\033[m]=== saint K win, DDR\033[1;32m%d\033[m, 1:%d mode\n",dfs_idx,GetFreqBySel(p,gFreqTbl[dfs_idx].freq_sel)<<1,gFreqTbl[dfs_idx].divmode==0?8:4));
            EMI_LPBK_memory_test_start(p, 1);
            EnableDramcPhyDCMShuffle(p, 1);
            
            mcSHOW_DBG_MSG(("=== saint DRAM_DFS_SHUFFLE_1 --> gFreqTbl[%d].shuffleIdx = %d\n\n\n",dfs_idx,gFreqTbl[dfs_idx].shuffleIdx));
            DramcSaveToShuffleSRAM(p, DRAM_DFS_SHUFFLE_1, gFreqTbl[dfs_idx].shuffleIdx); //p->MappingFreqArray[p->gDFSTable[u1ShuffleIdx].shuffleIdx]
            if (dfs_idx==9)//1600
            {
                LoadShuffleSRAMtoDramc(p, gFreqTbl[dfs_idx].shuffleIdx, DRAM_DFS_SHUFFLE_2);
            }

            if (ii==(shu_num-1))
                break;
            
            next_shu= shu_idx[(ii+1)%shu_num];
            
            vSetDFSFreqSelByTable(p, &gFreqTbl[next_shu]);
            vSet_Div_Mode(p, gFreqTbl[next_shu].divmode);
            vSet_Current_ShuLevel(p, gFreqTbl[next_shu].shuffleIdx); // for Vcore adjust
            DramcInit(p);
            vApplyConfigBeforeCalibration(p);
        }
        //dfs_idx = shu_idx[ii]; //dfs_2nd_idx;
        
        mcSHOW_DBG_MSG(("EnableDramcPhyDCM: on\n\n"));
        EnableDramcPhyDCM(p, 1);
        
        TransferToSPMControl(p);
        TransferPLLToSPMControl(p);

        vIO32WriteFldMulti_All((DDRPHY_MISC_SPM_CTRL3), P_Fld(gFreqTbl[dfs_idx].shuffleIdx, MISC_SPM_CTRL3_RG_DR_SHU_LEVEL_SRAM_CH1)
                                | P_Fld(gFreqTbl[dfs_idx].shuffleIdx, MISC_SPM_CTRL3_RG_DR_SHU_LEVEL_SRAM_CH0));

        SLT_Low_Power_Scenarios_Test(p, dfs_idx);

        return;
    }
    #endif
#endif //defined(SLT)

#if 0
        //for (ii=0;ii<400;ii++)  // Do 3 times EMI LPBK test
        for (ii=0;ii<1;ii++)  // Do 3 times EMI LPBK test
        {
            //GetPhyPllFrequency(p);
            
            //DFS_Freq_Meter_Check(p, DDRPhyFreqMeter());
            
            EMI_LPBK_memory_test_start(p, 1); //Emi lpbk K Rx, Tx window

            if (EMI_INT_LPBK_WL_DQS_RINGCNT==0)
            {
                //Enable golden setting
                mcSHOW_DBG_MSG(("EnableDramcPhyDCM: on\n\n"));
                EnableDramcPhyDCM(p, 1);
                
                EMI_LPBK_memory_test_start(p, 0); //Emi lpbk test
            }
            
            //DFSTestProgram(p, 0);
        }
#endif

#if 0
#if EMI_INT_LPBK_WL_DQS_RINGCNT //Dramc_init+WL(adjust selPH, -1UI)+ LP4-2400 EMI loop-bak pattern + check waveform 
        
        //U8 ii;
        
        for (ii=0;ii<5;ii++)
        {        
            // WL-1UI
            if (ii)
            {
                WriteLevelingMoveDQSInsteadOfCLK(p);
            }
            
            mcSHOW_ERR_MSG(("\n\n=== \033[1;32mWL - %dUI\033[m\n",ii));
            
            // EMI internal lpbk test
            EMI_LPBK_memory_test_start(p);
        }
        
#else        
        // use SLT EMI LPBK test 
        EMI_LPBK_memory_test_start(p);//saint
#endif
#endif //FT_DSIM_USED==0

}
#endif //#if (ENABLE_EMI_LPBK_TEST)
//======================== EMI LPBK TEST Definition End =====================================


//======================== SLT TEST Main Flow ===============================================
#if defined(SLT)
void SLT_Test_Main_Flow(DRAM_DRAM_TYPE_T dram_type, DRAM_CBT_MODE_EXTERN_T dram_cbt_mode_extern, DRAM_INFO_BY_MRR_T *DramInfo, U8 get_mdl_used)
{
    int mem_start,len, s4value;
    DRAMC_CTX_T * p;
    U8 ucstatus = 0;
    U32 u4value;
    U8 chIdx;

    psCurrDramCtx = &DramCtx_LPDDR4;

    p = psCurrDramCtx;  

    Set_MDL_Used_Flag(get_mdl_used);

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

#if (ENABLE_EMI_LPBK_TEST)
#if (!__ETT__)
	DRAM_SLT_DATA_T slt_data;
	int result;

	result = read_slt_data(&slt_data);
	if (result < 0) {
		mcSHOW_DBG_MSG(("[SLT] read fail. %d\n", result));
		while(1);
	}

	slt_data.header.stage_status = (slt_data.header.stage_status == -1)? 1 : 0;

	mcSHOW_DBG_MSG(("PK_SLT stage:%d\n", slt_data.header.stage_status));

	result = write_slt_data(&slt_data);
	if (result < 0) {
		mcSHOW_DBG_MSG(("[SLT] write fail. %d\n", result));
		while(1);
	}

   	Set_Emi_Lpbk_Test_Mode(slt_data.header.stage_status);

#else
    Set_Emi_Lpbk_Test_Mode(1);
#endif
    if (!gEmiLpbkTest) return;
#endif

    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);   //LP4 broadcast on

    if (gfirst_init_flag == 0)
    {
        MPLLInit();
        Global_Option_Init(p);
        gfirst_init_flag = 1;
    }

#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
    {
        U8 backup_broadcast;
        backup_broadcast = GetDramcBroadcast();
        EMI_Init(p);
        TA2_Test_Run_Time_HW_Set_Column_Num(p);
        DramcBroadcastOnOff(backup_broadcast);
    }
#endif

    vDramcInit_PreSettings(p);

    // DramC & PHY init for all channels
    //===  First frequency ======
    
    vSetDFSFreqSelByTable(p, &gFreqTbl[DRAM_DFS_SHUFFLE_MAX-1]);

    //DFSInitForCalibration(p);
    DramcSetting_Olympus_LP4_ByteMode(p);

    DramcModeRegInit_LP4(p);

    DdrUpdateACTiming(p);    

    vApplyConfigBeforeCalibration(p);

  

    #if ENABLE_EMI_LPBK_TEST
    if (!gEmiLpbkTest)
    #endif
    {
      O1Path_Test(p);
    }

    #if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
    U8 backup_broadcast;
    backup_broadcast = GetDramcBroadcast();
    EMI_Init2();
    DramcBroadcastOnOff(backup_broadcast);
    #endif
        
#if (ENABLE_EMI_LPBK_TEST)
    if (gEmiLpbkTest)
    {
        EMI_LPBK_memory_test(p);
        mcSHOW_DBG_MSG(("\nCold Reboot...\n"));
		pmic_config_interface(PMIC_RG_CRST_ADDR, 1, PMIC_RG_CRST_MASK, PMIC_RG_CRST_SHIFT);  //trigger full pmic reset
    }
#endif

}
#endif
//======================== SLT TEST Main Flow End ===========================================


