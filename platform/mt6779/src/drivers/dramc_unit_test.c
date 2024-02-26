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



#if PINMUX_AUTO_TEST_PER_BIT_RX
U8 gRX_check_per_bit_flag = 0;
void CheckRxPinMux(DRAMC_CTX_T *p)
{
    U8 u1Rxdelay[DQ_DATA_WIDTH_LP4] = {0};
    U8 u1RxIndex[DQ_DATA_WIDTH_LP4] = {0};
    int i = 0; 
    int j = 0;
    gRX_check_per_bit_flag = 1;
    U16 u2Min = 0xffff;
    U8 u1MinIdx = 0;

    for (i=0; i<DQ_DATA_WIDTH_LP4; i+=1)
    {
        memset(u1Rxdelay, 0, sizeof(u1Rxdelay));
        u1Rxdelay[i] = 0xf;
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ2), 
            P_Fld(u1Rxdelay[0], SHU_R0_B0_DQ2_RK0_RX_ARDQ0_R_DLY_B0) |
            P_Fld(u1Rxdelay[1], SHU_R0_B0_DQ2_RK0_RX_ARDQ1_R_DLY_B0));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ3), 
            P_Fld(u1Rxdelay[2], SHU_R0_B0_DQ3_RK0_RX_ARDQ2_R_DLY_B0) | 
            P_Fld(u1Rxdelay[3], SHU_R0_B0_DQ3_RK0_RX_ARDQ3_R_DLY_B0));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ4), 
            P_Fld(u1Rxdelay[4], SHU_R0_B0_DQ4_RK0_RX_ARDQ4_R_DLY_B0) |
            P_Fld(u1Rxdelay[5], SHU_R0_B0_DQ4_RK0_RX_ARDQ5_R_DLY_B0));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ5), 
            P_Fld(u1Rxdelay[6], SHU_R0_B0_DQ5_RK0_RX_ARDQ6_R_DLY_B0) |
            P_Fld(u1Rxdelay[7], SHU_R0_B0_DQ5_RK0_RX_ARDQ7_R_DLY_B0));

        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ2), 
            P_Fld(u1Rxdelay[8], SHU_R0_B1_DQ2_RK0_RX_ARDQ0_R_DLY_B1) |
            P_Fld(u1Rxdelay[9], SHU_R0_B1_DQ2_RK0_RX_ARDQ1_R_DLY_B1));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ3), 
            P_Fld(u1Rxdelay[10], SHU_R0_B1_DQ3_RK0_RX_ARDQ2_R_DLY_B1) |
            P_Fld(u1Rxdelay[11], SHU_R0_B1_DQ3_RK0_RX_ARDQ3_R_DLY_B1));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ4), 
            P_Fld(u1Rxdelay[12], SHU_R0_B1_DQ4_RK0_RX_ARDQ4_R_DLY_B1) |
            P_Fld(u1Rxdelay[13], SHU_R0_B1_DQ4_RK0_RX_ARDQ5_R_DLY_B1));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ5), 
            P_Fld(u1Rxdelay[14], SHU_R0_B1_DQ5_RK0_RX_ARDQ6_R_DLY_B1) |
            P_Fld(u1Rxdelay[15], SHU_R0_B1_DQ5_RK0_RX_ARDQ7_R_DLY_B1));

        DramcRxWindowPerbitCal(p, 0, RX_VREF_NOT_SPECIFY);//RDDQC
           
        u2Min = 0xffff;
        for(j = 0; j<DQ_DATA_WIDTH_LP4; j++)
        {
            if(gFinalRXPerbitFirstPass[p->channel][j] <= u2Min)
            {
                u2Min = gFinalRXPerbitFirstPass[p->channel][j];
                u1MinIdx = j;
            }
        }
        u1RxIndex[i] = u1MinIdx;
    }  

    for(i = 0; i<DQ_DATA_WIDTH_LP4; i++)
    {
        mcSHOW_DBG_MSG(("CH[%d] Rank[%d] APHY_RX[%d]-->DRAMC(-->RDDQC-->O1)[%d]\n", p->channel, p->rank, i, u1RxIndex[i]));
        if(u1RxIndex[i] != i)
        {
            mcSHOW_DBG_MSG(("!RX APHY DRAMC DQ is not mapping directly\n"));
            while(1);
        }
    }
    gRX_check_per_bit_flag = 0;
    return;
}
#endif

#if PINMUX_AUTO_TEST_PER_BIT_TX
U8 gTX_check_per_bit_flag = 0;
void CheckTxPinMux(DRAMC_CTX_T *p)
{
    U8 u1Txdelay[16] = {0};
    U8 u1TxIndex[16] = {0};
    int i = 0; 
    int j = 0;
    gTX_check_per_bit_flag = 1;
    U16 u2Min = 0xffff;
    U8 u1MinIdx = 0;

    for(i = 0; i<DQ_DATA_WIDTH_LP4; i++)
    {
        memset(u1Txdelay, 0, sizeof(u1Txdelay));
        u1Txdelay[i] = 0xf;
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ0), P_Fld(u1Txdelay[7], SHU_R0_B0_DQ0_RK0_TX_ARDQ7_DLY_B0) 
            | P_Fld(u1Txdelay[6], SHU_R0_B0_DQ0_RK0_TX_ARDQ6_DLY_B0) 
            | P_Fld(u1Txdelay[5], SHU_R0_B0_DQ0_RK0_TX_ARDQ5_DLY_B0) 
            | P_Fld(u1Txdelay[4], SHU_R0_B0_DQ0_RK0_TX_ARDQ4_DLY_B0) 
            | P_Fld(u1Txdelay[3], SHU_R0_B0_DQ0_RK0_TX_ARDQ3_DLY_B0) 
            | P_Fld(u1Txdelay[2], SHU_R0_B0_DQ0_RK0_TX_ARDQ2_DLY_B0) 
            | P_Fld(u1Txdelay[1], SHU_R0_B0_DQ0_RK0_TX_ARDQ1_DLY_B0) 
            | P_Fld(u1Txdelay[0], SHU_R0_B0_DQ0_RK0_TX_ARDQ0_DLY_B0));    
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ0), P_Fld(u1Txdelay[15], SHU_R0_B1_DQ0_RK0_TX_ARDQ7_DLY_B1) 
            | P_Fld(u1Txdelay[14], SHU_R0_B1_DQ0_RK0_TX_ARDQ6_DLY_B1) 
            | P_Fld(u1Txdelay[13], SHU_R0_B1_DQ0_RK0_TX_ARDQ5_DLY_B1) 
            | P_Fld(u1Txdelay[12], SHU_R0_B1_DQ0_RK0_TX_ARDQ4_DLY_B1) 
            | P_Fld(u1Txdelay[11], SHU_R0_B1_DQ0_RK0_TX_ARDQ3_DLY_B1) 
            | P_Fld(u1Txdelay[10], SHU_R0_B1_DQ0_RK0_TX_ARDQ2_DLY_B1) 
            | P_Fld(u1Txdelay[9], SHU_R0_B1_DQ0_RK0_TX_ARDQ1_DLY_B1) 
            | P_Fld(u1Txdelay[8], SHU_R0_B1_DQ0_RK0_TX_ARDQ0_DLY_B1));
        DramcTxWindowPerbitCal((DRAMC_CTX_T *) p, TX_DQ_DQS_MOVE_DQ_ONLY, FALSE);
        mcSHOW_DBG_MSG(("set 1 ranks set:0xf\n"));
                
        u2Min = 0xffff;
        for(j = 0; j<DQ_DATA_WIDTH_LP4; j++)
        {
            if(gFinalTXPerbitFirstPass[p->channel][j] <= u2Min)
            {
                u2Min = gFinalTXPerbitFirstPass[p->channel][j];
                u1MinIdx = j;
            }
        }
        u1TxIndex[i] = u1MinIdx;
    }  

    for(i = 0; i<DQ_DATA_WIDTH_LP4; i++)
    {
        mcSHOW_DBG_MSG(("CH[%d] Rank[%d] APHY_TX[%d]-->DRAMC(->TA2->)[%d]\n", p->channel, p->rank, i, u1TxIndex[i]));
        if(u1TxIndex[i] != i)
        {
            mcSHOW_DBG_MSG(("!TX APHY DRAMC DQ is not mapping directly\n"));
            while(1);
        }
    }
    gTX_check_per_bit_flag = 0;
    return;
}
#endif


#ifdef LOOPBACK_TEST
void DramcLoopbackTest_settings(DRAMC_CTX_T *p, U8 u1Type)
{

    //close DCM, or DQ no clock
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 0, MISC_CTRL1_R_DMDQSIENCG_EN);

    if (u1Type==1) //external loop back
    {
        //IOBIAS settings
        vIO32WriteFldAlign_All(DDRPHY_B0_DQ6, 1, B0_DQ6_RG_RX_ARDQ_BIAS_PS_B0);
        vIO32WriteFldAlign_All(DDRPHY_B1_DQ6, 1, B1_DQ6_RG_RX_ARDQ_BIAS_PS_B1);
        vIO32WriteFldAlign_All(DDRPHY_CA_CMD6, 1, CA_CMD6_RG_RX_ARCMD_BIAS_PS);

        //after initial, must set 0 of PHY registers
        vIO32WriteFldMulti_All(DDRPHY_B0_DQ3, P_Fld(0, B0_DQ3_RG_RX_ARDQS0_SWAP_EN_B0) | P_Fld(0, B0_DQ3_RG_RX_ARDQ_OFFC_EN_B0) | P_Fld(0, B0_DQ3_RG_RX_ARDQ_SMT_EN_B0));
        vIO32WriteFldMulti_All(DDRPHY_B1_DQ3, P_Fld(0, B1_DQ3_RG_RX_ARDQS0_SWAP_EN_B1) | P_Fld(0, B1_DQ3_RG_RX_ARDQ_OFFC_EN_B1) | P_Fld(0, B1_DQ3_RG_RX_ARDQ_SMT_EN_B1));
        vIO32WriteFldMulti_All(DDRPHY_CA_CMD3, P_Fld(0, CA_CMD3_RG_RX_ARCLK_SWAP_EN) | P_Fld(0, CA_CMD3_RG_RX_ARCMD_OFFC_EN) | P_Fld(0, CA_CMD3_RG_RX_ARCMD_SMT_EN));

        //after initial, must set 1 of PHY registers
        vIO32WriteFldMulti_All(DDRPHY_B0_DQ3, P_Fld(1, B0_DQ3_RG_RX_ARDQ_STBENCMP_EN_B0) | P_Fld(1, B0_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B0) | P_Fld(1, B0_DQ3_RG_RX_ARDQS0_IN_BUFF_EN_B0) | P_Fld(1, B0_DQ3_RG_ARDQ_RESETB_B0));
        vIO32WriteFldAlign_All(DDRPHY_B0_DQ9, 1, B0_DQ9_RG_RX_ARDQ_STBEN_RESETB_B0);
        vIO32WriteFldAlign_All(DDRPHY_B0_DQ5, 1, B0_DQ5_RG_RX_ARDQ_VREF_EN_B0);
        vIO32WriteFldMulti_All(DDRPHY_B0_DQ6, P_Fld(1, B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0) | P_Fld(1, B0_DQ6_RG_RX_ARDQ_RES_BIAS_EN_B0));

        vIO32WriteFldMulti_All(DDRPHY_B1_DQ3, P_Fld(1, B1_DQ3_RG_RX_ARDQ_STBENCMP_EN_B1) | P_Fld(1, B1_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B1) | P_Fld(1, B1_DQ3_RG_RX_ARDQS0_IN_BUFF_EN_B1) | P_Fld(1, B1_DQ3_RG_ARDQ_RESETB_B1));
        vIO32WriteFldAlign_All(DDRPHY_B1_DQ9, 1, B1_DQ9_RG_RX_ARDQ_STBEN_RESETB_B1);
        vIO32WriteFldAlign_All(DDRPHY_B1_DQ5, 1, B1_DQ5_RG_RX_ARDQ_VREF_EN_B1);
        vIO32WriteFldMulti_All(DDRPHY_B1_DQ6, P_Fld(1, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1) | P_Fld(1, B1_DQ6_RG_RX_ARDQ_RES_BIAS_EN_B1));

        vIO32WriteFldMulti_All(DDRPHY_CA_CMD3, P_Fld(1, CA_CMD3_RG_RX_ARCMD_STBENCMP_EN) | P_Fld(1, CA_CMD3_RG_RX_ARCMD_IN_BUFF_EN) | P_Fld(1, CA_CMD3_RG_RX_ARCLK_IN_BUFF_EN) | P_Fld(1, CA_CMD3_RG_ARCMD_RESETB));
        vIO32WriteFldAlign_All(DDRPHY_CA_CMD10, 1, CA_CMD10_RG_RX_ARCMD_STBEN_RESETB);
        vIO32WriteFldAlign_All(DDRPHY_CA_CMD5, 1, CA_CMD5_RG_RX_ARCMD_VREF_EN);
        vIO32WriteFldMulti_All(DDRPHY_CA_CMD6, P_Fld(1, CA_CMD6_RG_RX_ARCMD_BIAS_EN) | P_Fld(1, CA_CMD6_RG_RX_ARCMD_RES_BIAS_EN));
    }
}
#endif

#define APPLY_PPR_WA_LAFITE 1//a.Use dummy read to trigger ACT b.Set RG to let PRE work

#ifdef ENABLE_POST_PACKAGE_REPAIR
void DramcPostPackageRepair(DRAMC_CTX_T *p)
{
    U32 u4Response = 0;
    U32 u4TimeCnt = TIME_OUT_CNT;
    U16 u2Value = 0;
    U16 u2FailRow = 0x5ACD;//Fail IC 1-13 setting
    U8 ucFailBK = 2;//Fail IC 1-13 setting

    mcSHOW_DBG_MSG(("[DramcPostPackageRepair]\n"
                    "\n\tFreq=%d, CH=%d, Rank=%d\n", p->frequency, p->channel, p->rank));
    mcFPRINTF((fp_A60501, "[DramcPostPackageRepair]"
                          "\n\tFreq=%d, CH=%d, Rank=%d\n", p->frequency, p->channel, p->rank));
#if MRW_CHECK_ONLY
    mcSHOW_MRW_MSG(("\n==[MR Dump] %s==\n", __func__));
#endif
    //MR25 contains one bit of readout per bank indicating that at least one resource is available for Post Package Repair programming.
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1GetRank(p), MRS_MRSRK);
    DramcModeRegRead(p, 25, &u2Value);
    mcSHOW_DBG_MSG(("Before PostPackageRepair, MR25 = 0x%x\n", u2Value & 0xFF));
    mcFPRINTF((fp_A60501, "Before PostPackageRepair, MR25 = 0x%x\n", u2Value & 0xFF));

#if APPLY_PPR_WA_LAFITE
    //set RG to let PRE work
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_CONF0), 0, SHU_CONF0_ADVPREEN);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), 1, SPCMDCTRL_SPREA_EN);
#endif

    //1.DRAMC DCM freerun,
    //R_DMDCMEN2(dramc conf AO 0x38[1])=0
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 0, DRAMC_PD_CTRL_DCMEN2);

    //2.PHY DCM freerun,
    //R_DMPHYCLKDYNGEN(dramc conf AO 0x38[30])=0
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 0, DRAMC_PD_CTRL_PHYCLKDYNGEN);

    //3.Dram clock freerun,
    //R_DMMIOCKCTRLOFF(dramc	conf AO 0x38[26])=1
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 1, DRAMC_PD_CTRL_MIOCKCTRLOFF);

    //4.Fix CKE0 and CKE1 be high,
    //R_DMCKEFIXON(dramc conf AO 0x24[6], CKECTRL.CKEFIXON)=1
    //R_DMCKE1FIXON(dramc conf AO 0x24[4], CKECTRL.CKE1FIXON)=1
    CKEFixOnOff(p, p->rank, CKE_FIXON, CKE_WRITE_TO_ALL_CHANNEL);

    //Step 0: disable refresh for PPR
    //Let R_DMREFDIS=1
    vAutoRefreshSwitch(p, DISABLE);

    //Step 1: before enter PPR mode, all banks must be precharged
    //Set R_DMPREAEN=1
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_PREAEN);
    mcDELAY_US(1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1, SPCMD_PREAEN);

    //wait dramc_conf_nao.prea_response=1
    u4TimeCnt= TIME_OUT_CNT;
    u4Response = 0;
    do
    {
        u4Response = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_PREA_RESPONSE);
        u4TimeCnt --;
        mcDELAY_US(1);
    }while((u4Response ==0) &&(u4TimeCnt>0));

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_PREAEN);

    if(u4TimeCnt==0)//time out
    {
        mcSHOW_DBG_MSG(("dramc_conf_nao.prea_response fail (time out)\n"));
        mcFPRINTF((fp_A60501, "dramc_conf_nao.prea_response fail (time out)\n"));
        //return DRAM_FAIL;
    }

    //Step 2: enter PPR mode
    if(p->vendor_id == 0x6) //Skhynix Manufacturer ID
    {
        //Skhynix DRAM PPR Enable Sequence
        //Assumption: MR3 OP<2> - LOW
        //Skhynix 2z 6Gb/4Gb LPDDR4x PPR Guard key
        //PPR Guard Key, MR9 with following OP codes : B8 - E8 - 98 - BF - EF - 9F - B9 - E9 - 99 - 84 - A2 - 81
        //Skhynix 2z 8Gb LPDDR4x PPR Guard key
        //PPR Guard Key, MR9 with following OP codes : CD - AD -FD -C9 -A9 -F9 -C7 -A7 -F7
        DramcModeRegWrite(p, 9, 0xB8);
        DramcModeRegWrite(p, 9, 0xE8);
        DramcModeRegWrite(p, 9, 0x98);
        DramcModeRegWrite(p, 9, 0xBF);
        DramcModeRegWrite(p, 9, 0xEF);
        DramcModeRegWrite(p, 9, 0x9F);
        DramcModeRegWrite(p, 9, 0xB9);
        DramcModeRegWrite(p, 9, 0xE9);
        DramcModeRegWrite(p, 9, 0x99);
        DramcModeRegWrite(p, 9, 0x84);
        DramcModeRegWrite(p, 9, 0xA2);
        DramcModeRegWrite(p, 9, 0x81);
    }

    //Set MR4[4]=1: PPR entry
    DramcModeRegWrite(p, 4, 0x10);

    //Step 3: wait tMRD
    //mcDELAY_US(1000);
    mcDELAY_MS(1000);

#if APPLY_PPR_WA_LAFITE
    //Use dummy read to trigger ACT
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RK0_DUMMY_RD_ADR), P_Fld(u2FailRow, RK0_DUMMY_RD_ADR_DMY_RD_RK0_ROW_ADR)
                        | P_Fld(0, RK0_DUMMY_RD_ADR_DMY_RD_RK0_LEN));
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_DUMMY_RD_BK), ucFailBK, RK0_DUMMY_RD_BK_DMY_RD_RK0_BK);
    
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD), 0, DUMMY_RD_DUMMY_RD_EN); //DUMMY_RD_DMY_RD_DBG
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD), 2, DUMMY_RD_RANK_NUM); //To R0 and R1, use guard key to select wrong rank
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD), 0, DUMMY_RD_DMY_RD_DBG);
    mcDELAY_MS(1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD), 1, DUMMY_RD_DMY_RD_DBG);
    mcSHOW_DBG_MSG(("####Trigger Dummy read(DBG) for ACT\n"));
#endif

    //Step 4: issue ACT command with fail row address
    //Set R_DMACTEN_ROW, R_DMACTEN_BK, then set R_DMACTEN,
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MPC_OPTION), 1, MPC_OPTION_MPCRKEN);
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1GetRank(p), MRS_MRSRK);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PPR_CTRL), u2FailRow, PPR_CTRL_ACTEN_ROW);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PPR_CTRL), ucFailBK, PPR_CTRL_ACTEN_BK);
    mcSHOW_DBG_MSG(("PPR, Fail Row = %d,  Fail Bank = %d\n", u2FailRow, ucFailBK));
    mcFPRINTF((fp_A60501, "PPR, Fail Row = %d,  Fail Bank = %d\n", u2FailRow, ucFailBK));

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_ACTEN);
    mcDELAY_US(1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1, SPCMD_ACTEN);

    //wait dramc_conf_nao.act_response=1
    u4TimeCnt= TIME_OUT_CNT;
    u4Response=0;
    do
    {
        u4Response = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_ACT_RESPONSE);
        u4TimeCnt --;
        mcDELAY_US(1);
    }while((u4Response ==0) &&(u4TimeCnt>0));

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_ACTEN);

    if(u4TimeCnt==0)//time out
    {
        mcSHOW_DBG_MSG(("dramc_conf_nao.act_response fail (time out)\n"));
        mcFPRINTF((fp_A60501, "dramc_conf_nao.act_response fail (time out)\n"));
        //return DRAM_FAIL;
    }

    //Step 5: wait tPGM to allow DRAM repair internally
    mcDELAY_MS(3000);

    //Step 6: issue PRE
    //Set R_DMPREAEN=1
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_PREAEN);
    mcDELAY_US(1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1, SPCMD_PREAEN);

    //wait dramc_conf_nao.prea_response=1
    u4TimeCnt= TIME_OUT_CNT;
    u4Response=0;
    do
    {
        u4Response = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_PREA_RESPONSE);
        u4TimeCnt --;
        mcDELAY_US(1);
    }while((u4Response ==0) &&(u4TimeCnt>0));

    if(u4TimeCnt==0)//time out
    {
        mcSHOW_DBG_MSG(("dramc_conf_nao.prea_response fail (time out)\n"));
        mcFPRINTF((fp_A60501, "dramc_conf_nao.prea_response fail (time out)\n"));
        //return DRAM_FAIL;
    }

    //Step 7: wait tPGM_Exit
    //mcDELAY_US(1000);
    mcDELAY_MS(1000);

    //Step 8: exit PPR
    //Set MR4[4]=0: PPR exit
    DramcModeRegWrite(p, 4, 0x0);

    //Step 9: wait tPGMPST, them dram is ready for any valid command
    //mcDELAY_US(1000);
    mcDELAY_MS(1000);

    //MR25 contains one bit of readout per bank indicating that at least one resource is available for Post Package Repair programming.
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1GetRank(p), MRS_MRRRK);
    DramcModeRegRead(p, 25, &u2Value);
    mcSHOW_DBG_MSG(("After PostPackageRepair, MR25 = 0x%x\n", u2Value & 0xFF));
    mcFPRINTF((fp_A60501, "After PostPackageRepair, MR25 = 0x%x\n", u2Value & 0xFF));

    return;
}
#endif


#ifdef MPC_SW_FIFO
void DramcMPC_FIFO(DRAMC_CTX_T *p)
{
    U32 u4Result3,u4Result2,u4Result1,u4Result0;
	U32 u4Result, u4Response;
    U32 u4TimeCnt= TIME_OUT_CNT;
    U32 u4RegBackupAddress[] =
    {
        (DRAMC_REG_ADDR(DRAMC_REG_LBWDAT3)),
        (DRAMC_REG_ADDR(DRAMC_REG_LBWDAT2)),
        (DRAMC_REG_ADDR(DRAMC_REG_LBWDAT1)),
        (DRAMC_REG_ADDR(DRAMC_REG_LBWDAT0)),
        (DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0)),
        (DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL)),
        (DRAMC_REG_ADDR(DRAMC_REG_SHU_SCINTV)),
        (DRAMC_REG_ADDR(DRAMC_REG_PERFCTL0)),
    };


    mcSHOW_DBG_MSG(("[DramcMPC_FIFO]\n"));
 	//Register backup
    DramcBackupRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));

	//WRFIFO and RDFIFO's golden data
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_LBWDAT3), 0xAAAAAAAA, LBWDAT3_LBWDATA3);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_LBWDAT2), 0x55555555, LBWDAT2_LBWDATA2);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_LBWDAT1), 0xAAAAAAAA, LBWDAT1_LBWDATA1);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_LBWDAT0), 0x55555555, LBWDAT0_LBWDATA0);

	u4Result3 = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_LBWDAT3));
	mcSHOW_DBG_MSG(("DRAMC_REG_LBWDAT3: 0x%x\n", u4Result3));
	mcFPRINTF((fp_A60501, "DRAMC_REG_LBWDAT3: 0x%x\n", u4Result3));
	u4Result2 = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_LBWDAT2));
	mcSHOW_DBG_MSG(("DRAMC_REG_LBWDAT2: 0x%x\n", u4Result2));
	mcFPRINTF((fp_A60501, "DRAMC_REG_LBWDAT2: 0x%x\n", u4Result2));
	u4Result1 = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_LBWDAT1));
	mcSHOW_DBG_MSG(("DRAMC_REG_LBWDAT1: 0x%x\n", u4Result1));
	mcFPRINTF((fp_A60501, "DRAMC_REG_LBWDAT1: 0x%x\n", u4Result1));
	u4Result0 = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_LBWDAT0));
	mcSHOW_DBG_MSG(("DRAMC_REG_LBWDAT0: 0x%x\n", u4Result0));
	mcFPRINTF((fp_A60501, "DRAMC_REG_LBWDAT0: 0x%x\n", u4Result0));


	//Other command is not valid during "WRFIFO and RDFIFO"  period.
	//Disable auto refresh: set R_DMREFDIS=1
    vAutoRefreshSwitch(p, DISABLE);
	//Disable MR4: set R_DMREFRDIS=1
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), 1, SPCMDCTRL_REFRDIS);
	//Disable ZQCAL/ZQLAT command: set R_DMZQCALDISB=0
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), 0, SPCMDCTRL_ZQCALDISB);


	//When R_DMWRFIFOEN=1, MPC WRFIFO can send single request or 5 requests by R_DMRDDQC_INTV[1:0] (0x8C8[12:11])
	//Set R_DMRDDQC_INTV=2'b00 and Set R_DMWRFIO_MODE2 = 1'b0 for single MPC WRFIFO (single mode)
	//Set R_DMRDDQC_INTV=2'b01 and Set R_DMWRFIO_MODE2 = 1'b1 for five MPC WRFIFO (burst mode)
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SCINTV), 0, SHU_SCINTV_RDDQC_INTV);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PERFCTL0), 0, PERFCTL0_WRFIO_MODE2);


    //Issue MPC RD FIFO
    //R_DMWRFIFOEN, 0x0C[31]=1 for WR FIFO
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PERFCTL0), 1, PERFCTL0_WRFIFOEN);

	//Wait wrfifo_response=1 (dramc_conf_nao, 0x88[31])
    do
    {
        u4Response = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_WRFIFO_RESPONSE);
        u4TimeCnt --;
        mcDELAY_US(1);
    }while((u4Response ==0) &&(u4TimeCnt>0));

    if(u4TimeCnt==0)//time out
    {
        mcSHOW_DBG_MSG(("[DramcMPC_FIFO] Resp fail (time out)\n"));
        mcFPRINTF((fp_A60501, "[DramcMPC_FIFO] Resp fail (time out)\n"));
        //return DRAM_FAIL;
    }

    //R_DMWRFIFOEN, 0x0C[31]=0
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PERFCTL0), 0, PERFCTL0_WRFIFOEN);



	//Issue MPC RD FIFO
    //R_DMRDFIFOEN, 0x0C[30]=1 for RD FIFO
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PERFCTL0), 1, PERFCTL0_RDFIFOEN);

	//Wait wrfifo_response=1 (dramc_conf_nao, 0x88[31])
    do
    {
        u4Response = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_RDFIFO_RESPONSE);
        u4TimeCnt --;
        mcDELAY_US(1);
    }while((u4Response ==0) &&(u4TimeCnt>0));

    if(u4TimeCnt==0)//time out
    {
        mcSHOW_DBG_MSG(("[DramcMPC_FIFO] Resp fail (time out)\n"));
        mcFPRINTF((fp_A60501, "[DramcMPC_FIFO] Resp fail (time out)\n"));
        //return DRAM_FAIL;
    }

    //Then read RDFIFO compare result (dramc_conf_nao, 0x124)
    //Must do WRFIO first, then do RDFIFO, then compare it.
    u4Result = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CMP_ERR));
	mcSHOW_DBG_MSG(("[DramcMPC_FIFO] Read RDFIFO compare result: 0x%x\n", u4Result));

    //R_DMRDFIFOEN, 0x0C[30]=0
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PERFCTL0), 0, PERFCTL0_RDFIFOEN);

	//Restore registers
    DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));

}
#endif

#if (SW_CHANGE_FOR_SIMULATION || FOR_DV_SIMULATION_USED)
void vMR2InitForSimulationTest(DRAMC_CTX_T *p)
{
    // Dram Init will set MR2's value
    #if 0
        if(p->frequency >HIGH_FREQ)
        {
            u1MR02Value[p->dram_fsp]  = 0x3f;
        }
        else if(p->frequency <=DUAL_FREQ_LOW_LP4)
        {
            u1MR02Value[p->dram_fsp]  = 0x1b;
        }
        else
        {
            u1MR02Value[p->dram_fsp]  = 0x2d;
        }
    #endif
}
#endif


#if ENABLE_APB_MASK_WRITE
void EnableDramcAPBMaskWrite(DRAMC_CTX_T *p)
{
    vIO32WriteFldAlign_All(DDRPHY_MISC_CTRL2, 0x1, MISC_CTRL2_R_APB_WBIT_MASK_EN);
}

void TestAPBMaskWriteFunc(DRAMC_CTX_T *p)
{
    U8 u1start = 0;
    U32 u4Fld = 0, u4RGFld = 0, pass_cnt = 0, err_cnt = 0;
    U8 u1startMax = 16; // from APB Mask Write DVT doc [31:27]
    U8 u1wid = 0, i;
    U8 u1widMax = 16;
    U16 u2Data = 0, u2Val = 0;

    for (u1start = 0; u1start <= u1startMax; u1start++)
    {
        for (u1wid = 0; u1wid < u1widMax; u1wid++)
        {
            u4Fld = Fld(u1wid, u1start, 0); // for APB mode
            u4RGFld = Fld(u1wid+1, u1start, 0); // for RG mode read
            for (i = 0; i <= u1wid; i++)
                u2Data |= (0x1 << i);

            vIO32WriteFldAlign_All(DDRPHY_MISC_CTRL2, 0x1, MISC_CTRL2_R_APB_WBIT_MASK_EN);
            vIO32WriteAPBFldAlign(DRAMC_REG_SPCMDCTRL, u2Data, u4Fld);
            vIO32WriteAPBFldAlign(DRAMC_REG_SPCMDCTRL+SHIFT_TO_CHB_ADDR, u2Data, u4Fld);   
            vIO32WriteAPBFldAlign(DDRPHY_MISC_CTRL2, 0, MISC_CTRL2_R_APB_WBIT_MASK_EN);
            vIO32WriteAPBFldAlign(DDRPHY_MISC_CTRL2+SHIFT_TO_CHB_ADDR, 0, MISC_CTRL2_R_APB_WBIT_MASK_EN);
            u2Val = u4IO32ReadFldAlign(DRAMC_REG_SPCMDCTRL, u4RGFld);
            u2Val |= u4IO32ReadFldAlign(DRAMC_REG_SPCMDCTRL+SHIFT_TO_CHB_ADDR, u4RGFld);

            //mcSHOW_DBG_MSG(("[TestAPBMaskWriteFunc] u2Data= 0x%x, u4Fld= 0x%x, u2Val = 0x%x, u4RGFld = 0x%x\n", u2Data, u4Fld, u2Val, u4RGFld));
    
            if(u2Data == u2Val)
            {
                pass_cnt++;
            }
            else
            {
                err_cnt++;
            }
            u2Data = 0;
        }
    }

    if(err_cnt == 0)
    {
        mcSHOW_DBG_MSG(("[TestAPBMaskWriteFunc] PASS_CNT = %d PASS\n", pass_cnt));
    }
    else
    {
        mcSHOW_DBG_MSG(("[TestAPBMaskWriteFunc] ERR_CNT=%d FAIL!!!\n", err_cnt));
    }
}

void DramcRegAPBWRange(DRAMC_CTX_T *p, U32 u4StartAddr, U32 u4EndAddr)
{
    U32 ii;
    U8 u1MskEn;
    U32 u4Fld = 0;

    u1MskEn = u4IO32ReadFldAlign(DDRPHY_MISC_CTRL2, MISC_CTRL2_R_APB_WBIT_MASK_EN);

    for(ii = u4StartAddr; ii <= u4EndAddr; ii += 4)
    {
        u4Fld = Fld(15, 0, 0); //len=15, start=0
        if (u1MskEn == TRUE)
            vIO32WriteAPBFldAlign(ii, 0x7fff, u4Fld); // data=0x7fff
        else
            vIO32WriteFldAlign(ii, 0x7fff, u4Fld);
        //mcDELAY_US(10);
    }
}

#define DRAMC_AO_APB_RANGE (DRAMC_REG_SHURK1_DQS2DQ_CAL5 - DRAMC_AO_BASE_ADDRESS)
#define DDRPHY_AO_APB_RANGE (DDRPHY_RFU_0X10CC - DDRPHY_AO_BASE_ADDR)
DRAM_STATUS_T DramcRegAPBWriteMask(DRAMC_CTX_T *p)
{
    //mcSHOW_DBG_MSG(("APB Write Mask CHA_DRAMC_AO_BASE\n"));
    DramcRegAPBWRange(p, Channel_A_DRAMC_AO_BASE_ADDRESS, Channel_A_DRAMC_AO_BASE_ADDRESS + DRAMC_AO_APB_RANGE);
    //mcSHOW_DBG_MSG(("APB Write Mask CHB_DRAMC_AO_BASE\n"));
    DramcRegAPBWRange(p, Channel_B_DRAMC_AO_BASE_ADDRESS, Channel_B_DRAMC_AO_BASE_ADDRESS + DRAMC_AO_APB_RANGE);

    //mcSHOW_DBG_MSG(("APB Write Mask CHA_PHY_AO_BASE\n"));
    DramcRegAPBWRange(p, Channel_A_PHY_AO_BASE_ADDRESS, Channel_A_PHY_AO_BASE_ADDRESS + DDRPHY_AO_APB_RANGE);
    //mcSHOW_DBG_MSG(("APB Write Mask CHB_PHY_AO_BASE\n"));
    DramcRegAPBWRange(p, Channel_B_PHY_AO_BASE_ADDRESS, Channel_B_PHY_AO_BASE_ADDRESS + DDRPHY_AO_APB_RANGE);
}
#endif

