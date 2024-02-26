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

//U8 gu1MR23Done = FALSE; /* Not used starting from Vinson (all freqs use MR23=0x3F) */
U8 gu1MR23[CHANNEL_NUM][RANK_MAX];
/* DQSOSCTHRD_INC & _DEC are 12 bits (Starting from Vinson) */
U16 gu2DQSOSCTHRD_INC[CHANNEL_NUM][RANK_MAX];
U16 gu2DQSOSCTHRD_DEC[CHANNEL_NUM][RANK_MAX];
U16 gu2MR18[CHANNEL_NUM][RANK_MAX]; /* Stores MRR MR18 (DQS ocillator count - MSB) */
U16 gu2MR19[CHANNEL_NUM][RANK_MAX]; /* Stores MRR MR19 (DQS ocillator count - LSB) */
U16 gu2DQSOSC[CHANNEL_NUM][RANK_MAX]; /* Stores tDQSOSC results */
U16 gu2DQSOscCnt[CHANNEL_NUM][RANK_MAX][2];



void DramcDQSOSCInit(void)
{
    memset(gu1MR23, 0x3F, sizeof(gu1MR23)); /* MR23 should be 0x3F for all freqs (Starting from Vinson) */
    memset(gu2DQSOSCTHRD_INC, 0x6, sizeof(gu2DQSOSCTHRD_INC));
    memset(gu2DQSOSCTHRD_DEC, 0x4, sizeof(gu2DQSOSCTHRD_DEC));
}

DRAM_STATUS_T DramcStartDQSOSC(DRAMC_CTX_T *p)
{
    U32 u4Response;
    U32 u4TimeCnt;

    u4TimeCnt = TIME_OUT_CNT;
    mcSHOW_DBG_MSG(("[DQSOSC]\n"));

    //R_DMDQSOSCENEN, 0x1E4[10]=1 for DQSOSC Start
    //Wait dqsoscen_response=1 (dramc_conf_nao, 0x3b8[29])
    //R_DMDQSOSCENEN, 0x1E4[10]=0
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1, SPCMD_DQSOSCENEN);
    do
    {
        u4Response = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_DQSOSCEN_RESPONSE);
        u4TimeCnt --;
        mcDELAY_US(1);
    }while((u4Response==0) &&(u4TimeCnt>0));

    if(u4TimeCnt==0)//time out
    {
        mcSHOW_DBG_MSG(("Start fail (time out)\n"));
        //mcFPRINTF((fp_A60501, "[DramcStartDQSOSC]  Start fail (time out)\n"));
        return DRAM_FAIL;
    }
    else
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_DQSOSCENEN);
    }

    return DRAM_OK;
}


DRAM_STATUS_T DramcDQSOSCAuto(DRAMC_CTX_T *p)
{
    U8 u1MR23 = gu1MR23[p->channel][p->rank], shu_index;
    U16 u2MR18, u2MR19;
    U16 u2DQSCnt;
    U16 u2DQSOsc[2];
    U32 u4RegBak[3];

#if MRW_CHECK_ONLY
    mcSHOW_MRW_MSG(("\n==[MR Dump] %s==\n", __func__));
#endif

    u4RegBak[0] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_MRS));
    u4RegBak[1] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL));
    u4RegBak[2] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL));

    //!!R_DMMRSRK(R_DMMPCRKEN=1) specify rank0 or rank1
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), 0, RKCFG_DQSOSC2RK);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1GetRank(p), MRS_MRSRK);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MPC_OPTION), 1, MPC_OPTION_MPCRKEN);

    //LPDDR4-3200,     PI resolution = tCK/64 =9.76ps
    //Only if MR23>=16, then error < PI resolution.
    //Set MR23 == 0x3f, stop after 63*16 clock
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1GetRank(p), MRS_MRSRK);
    DramcModeRegWriteByRank(p, p->rank, 23, u1MR23);

    //SW mode
    //for(shu_index = DRAM_DFS_SHUFFLE_1; shu_index < DRAM_DFS_SHUFFLE_MAX; shu_index++)
        //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SCINTV) + SHU_GRP_DRAMC_OFFSET*shu_index, 1, SHU_SCINTV_DQSOSCENDIS);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SCINTV), 1, SHU_SCINTV_DQSOSCENDIS);
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2_SCINTV), 1, SHU2_SCINTV_DQSOSCENDIS);
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3_SCINTV), 1, SHU3_SCINTV_DQSOSCENDIS);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 1, DRAMC_PD_CTRL_MIOCKCTRLOFF);   //MIOCKCTRLOFF=1

    CKEFixOnOff(p, p->rank, CKE_FIXON, CKE_WRITE_TO_ONE_CHANNEL);

    DramcStartDQSOSC(p);
    mcDELAY_US(1);
    
    DramcModeRegReadByRank(p, p->rank, 18, &u2MR18);
    DramcModeRegReadByRank(p, p->rank, 19, &u2MR19);

#if (SW_CHANGE_FOR_SIMULATION==0)
    //B0
    u2DQSCnt = (u2MR18 & 0x00FF) | ((u2MR19 & 0x00FF) <<8);
    if(u2DQSCnt !=0)
        u2DQSOsc[0] = u1MR23*16*1000000/(2 * u2DQSCnt * p->frequency); //tDQSOSC = 16*MR23*tCK/2*count
    else
        u2DQSOsc[0] = 0;

    //B1
    u2DQSCnt = (u2MR18 >> 8) | ((u2MR19 & 0xFF00));
    if(u2DQSCnt !=0)
        u2DQSOsc[1] = u1MR23*16*1000000/(2 * u2DQSCnt * p->frequency); //tDQSOSC = 16*MR23*tCK/2*count
    else
        u2DQSOsc[1] = 0;
    mcSHOW_DBG_MSG(("[DQSOSCAuto] RK%d, (LSB)MR18= 0x%x, (MSB)MR19= 0x%x, tDQSOscB0 = %d ps tDQSOscB1 = %d ps\n", u1GetRank(p), u2MR18, u2MR19, u2DQSOsc[0], u2DQSOsc[1]));
#endif

    gu2MR18[p->channel][p->rank] = u2MR18;
    gu2MR19[p->channel][p->rank] = u2MR19;
    gu2DQSOSC[p->channel][p->rank] = u2DQSOsc[0];

    if(u2DQSOsc[1]!=0 && u2DQSOsc[1]<u2DQSOsc[0])
        gu2DQSOSC[p->channel][p->rank] = u2DQSOsc[1];

    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_MRS), u4RegBak[0]);
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), u4RegBak[1]);
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), u4RegBak[2]);

    /* Starting from Vinson, MR23 should be 0x3F for all case ("HW tracking modify" DVT)
     * -> Value is already set in during gu1MR23 array init
     * The below code snippet is used for KIBO/Alaska/Whitney E2 projects
     * (Adjusts MR23 according to freq, since back then _INC _DEC bit's weren't sufficient)
     */
#if 0
    if(gu1MR23Done==FALSE)
    {
        DRAM_CHANNEL_T chIdx = CHANNEL_A;
        DRAM_RANK_T rankIdx = RANK_0;
        if(gu2DQSOSC[p->channel][p->rank]>500)
        {
            u1MR23 = 0x30;
        }
        else if(gu2DQSOSC[p->channel][p->rank]>350)
        {
            u1MR23 = 0x20;
        }
        else
        {
            u1MR23 = 0x10;
        }
        for(chIdx=CHANNEL_A; chIdx<p->support_channel_num; chIdx++)
        {
            for(rankIdx=RANK_0; rankIdx<RANK_MAX; rankIdx++)
            {
                gu1MR23[chIdx][rankIdx] = u1MR23;
            }
        }
        mcSHOW_DBG_MSG(("Update MR23=%d\n", u1MR23));
        gu1MR23Done = TRUE;
    }
#endif

    return DRAM_OK;
}

#if 1
/* Using gu2DQSOSC results calculated from DramcDQSOSCAuto
 * -> calculate DQSOSCTHRD_INC, DQSOSCTHRD_DEC
 * _INC, _DEC formulas are extracted from "Verification plan of Vinson LPDDR4 HW TX Tracking" doc
 */
DRAM_STATUS_T DramcDQSOSCMR23(DRAMC_CTX_T *p)
{
#if (SW_CHANGE_FOR_SIMULATION==0)
#if 1
    /* Preloader doesn't support floating point numbers -> Manually expand/simpify _INC, _DEC formula */
    U8 u1MR23 = gu1MR23[p->channel][p->rank];
    U16 u2DQSOSC = gu2DQSOSC[p->channel][p->rank];
    U32 u4tCK = 1000000 / p->frequency;

    if(u2DQSOSC !=0)
    {
        gu2DQSOSCTHRD_INC[p->channel][p->rank] = (3 * u1MR23 * u4tCK * u4tCK) / (u2DQSOSC * u2DQSOSC * 20);
        gu2DQSOSCTHRD_DEC[p->channel][p->rank] = (u1MR23 * u4tCK * u4tCK) / (u2DQSOSC * u2DQSOSC * 10);
    }
#else
    /* Formulas implemented directly as mentioned (using floating point numbers) */
    #define DQSOSC_ALPHA 0.2
    /*
     * actualAlpha, tickTime, goldenMR23 formulas are extracted from
     * "Vinson HW TX Tracking Modify" DVT document's excel file
     */
    float actualAlpha = DQSOSC_ALPHA/((1600/(float)p->frequency)*(1600/(float)p->frequency)); //DDR3200's alpha = 0.2, use it as base
    float tickTime = (1/(float)p->frequency)*1000000;
    float goldenMR23 = (2*4/DQSOSC_ALPHA)*(((float)u2DQSOSC/tickTime)*((float)u2DQSOSC/tickTime));

    /* (Floating point + 0.5) and truncated into unsigned integer -> roundup floating point number */
    gu2DQSOSCTHRD_INC[p->channel][p->rank] = (5*1.2*((float)u4MR23/goldenMR23)+0.5);
    gu2DQSOSCTHRD_DEC[p->channel][p->rank] = (5*0.8*((float)u4MR23/goldenMR23)+0.5);
#endif
    mcSHOW_DBG_MSG(("CH%d_RK%d: MR19=0x%X, MR18=0x%X, DQSOSC=%d, MR23=%u, INC=%u, DEC=%u\n", p->channel, p->rank,
                    gu2MR19[p->channel][p->rank], gu2MR18[p->channel][p->rank], gu2DQSOSC[p->channel][p->rank],
                    u1MR23, gu2DQSOSCTHRD_INC[p->channel][p->rank], gu2DQSOSCTHRD_DEC[p->channel][p->rank]));
#endif
    return DRAM_OK;
}
#else
/* Used for previous projects (before Vinson) */
DRAM_STATUS_T DramcDQSOSCMR23(DRAMC_CTX_T *p)
{
#if (SW_CHANGE_FOR_SIMULATION==0)
    U16 u2DQSOSC = gu2DQSOSC[p->channel][p->rank];
    U32 u4tCK = 1000000/p->frequency;
    U32 u4RunTime = (32*4*5*(u2DQSOSC*u2DQSOSC)/(u4tCK*u4tCK))+1;
    //U32 u4MR23 = (u4RunTime/16)+1;
    U32 u4MR23 = gu1MR23[p->channel][p->rank];

    gu2DQSOSCTHRD_INC[p->channel][p->rank] = (6*u4MR23*16)/(u4RunTime);
    gu2DQSOSCTHRD_DEC[p->channel][p->rank] = (4*u4MR23*16)/(u4RunTime);
    mcSHOW_DBG_MSG(("CH%d_RK%d: MR19=%X, MR18=%X, DQSOSC=%d, Runtime=%d, MR23=%d, INC=%d, DEC=%d\n",
        p->channel, p->rank, gu2MR19[p->channel][p->rank], gu2MR18[p->channel][p->rank], gu2DQSOSC[p->channel][p->rank],
        u4RunTime,u4MR23, gu2DQSOSCTHRD_INC[p->channel][p->rank], gu2DQSOSCTHRD_DEC[p->channel][p->rank]));
#endif
    return DRAM_OK;
}
#endif

/* Sets DQSOSC_BASE for specified rank/byte */
DRAM_STATUS_T DramcDQSOSCSetMR18MR19(DRAMC_CTX_T *p)
{
    U16 u2DQSOscCnt[2];
    DramcDQSOSCAuto(p);
    
    //B0
    gu2DQSOscCnt[p->channel][p->rank][0] = u2DQSOscCnt[0] = (gu2MR18[p->channel][p->rank] & 0x00FF) | ((gu2MR19[p->channel][p->rank] & 0x00FF) <<8);
    //B1
    gu2DQSOscCnt[p->channel][p->rank][1] = u2DQSOscCnt[1] = (gu2MR18[p->channel][p->rank] >> 8) | ((gu2MR19[p->channel][p->rank] & 0xFF00));

    if((p->dram_cbt_mode[p->rank]==CBT_NORMAL_MODE) && (gu2DQSOscCnt[p->channel][p->rank][1]==0))
    {
        gu2DQSOscCnt[p->channel][p->rank][1] = u2DQSOscCnt[1] = u2DQSOscCnt[0];
    }

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQSOSC), P_Fld(u2DQSOscCnt[0], SHURK0_DQSOSC_DQSOSC_BASE_RK0)| P_Fld(u2DQSOscCnt[1], SHURK0_DQSOSC_DQSOSC_BASE_RK0_B1));

    mcSHOW_DBG_MSG(("CH%d RK%d: MR19=%X, MR18=%X\n", p->channel, p->rank, gu2MR19[p->channel][p->rank], gu2MR18[p->channel][p->rank]));
    return DRAM_OK;
}

DRAM_STATUS_T DramcDQSOSCShuSettings(DRAMC_CTX_T *p)
{
    U8 u1MR23 = 0x3F;
    U16 u2PRDCNT = 0x3FF;
    U8 u1PRDCNT_DIV = 4;
    U16 u2DQSOSCENCNT = 0xFFF;
    U16 u2Thrd_inc, u2Thrd_dec;
    U8 u1FILT_PITHRD = 0;
    U8 u1W2R_SEL = 0;

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SCINTV),  0x0, SHU_SCINTV_DQS2DQ_SHU_PITHRD);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_DQSOSC),  0x0, RK0_DQSOSC_R_DMDQS2DQ_FILT_OPT);
    if(p->frequency<=400)
    {
        u1FILT_PITHRD = 0x4;
        u1W2R_SEL = 0x5;
    }
    else if(p->frequency<=600)
    {
        u1FILT_PITHRD = 0x6;
        u1W2R_SEL = 0x5;
    }
    else if(p->frequency<=800)
    {
        u1FILT_PITHRD = 0x7;
        u1W2R_SEL = 0x5;
    }
    else if(p->frequency<=1200)
    {
        u1FILT_PITHRD = 0xb;
        u1W2R_SEL = 0x2;
    }
    else if(p->frequency<=1333)
    {
        u1FILT_PITHRD = 0xc;
        u1W2R_SEL = 0x2;
    }
    else if(p->frequency<=1600)
    {
        u1FILT_PITHRD = 0xE;
        u1W2R_SEL = 0x2;
    }
    else if(p->frequency<=1866)
    {
        u1FILT_PITHRD = 0x12;
        u1W2R_SEL = 0x2;
    }
    else    //4266
    {
        u1FILT_PITHRD = 0x15;
        u1W2R_SEL = 0x2;
    }

    if(vGet_Div_Mode(p)== DIV4_MODE)
    {
        u1PRDCNT_DIV = 2;
    }
    
    u2PRDCNT = (gu1MR23[p->channel][RANK_0]/u1PRDCNT_DIV)+3;
#ifdef DUAL_RANK_ENABLE
    if(gu1MR23[p->channel][RANK_0]>gu1MR23[p->channel][RANK_1])
        u2PRDCNT = (gu1MR23[p->channel][RANK_0]/u1PRDCNT_DIV)+3;
    else
        u2PRDCNT = (gu1MR23[p->channel][RANK_1]/u1PRDCNT_DIV)+3;
#endif

    //Don't power down dram during DQS interval timer run time, (MR23[7:0] /4) + (tOSCO/MCK unit/16)
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DQSOSC_PRD), u2PRDCNT, SHU_DQSOSC_PRD_DQSOSC_PRDCNT);

    //set tOSCO constraint to read MR18/MR19, should be > 40ns/MCK
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DQSOSCR), ((p->frequency-1)/100)+1, SHU_DQSOSCR_DQSOSCRCNT);//unit: MCK to meet spec. tOSCO
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SCINTV),  u1FILT_PITHRD, SHU_SCINTV_DQS2DQ_FILT_PITHRD);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_WODT),  P_Fld(u1W2R_SEL, SHU_WODT_TXUPD_W2R_SEL) | P_Fld(0x0, SHU_WODT_TXUPD_SEL));

    /* Starting from Vinson, DQSOSCTHRD_INC & _DEC is split into RK0 and RK1 */
    //Rank 0
    u2Thrd_inc = gu2DQSOSCTHRD_INC[p->channel][RANK_0];
    u2Thrd_dec = gu2DQSOSCTHRD_DEC[p->channel][RANK_0];
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DQSOSCTHRD), u2Thrd_inc, SHU_DQSOSCTHRD_DQSOSCTHRD_INC_RK0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DQSOSCTHRD), u2Thrd_dec, SHU_DQSOSCTHRD_DQSOSCTHRD_DEC_RK0);

    //Rank 1
    u2Thrd_inc = gu2DQSOSCTHRD_INC[p->channel][RANK_1];
    u2Thrd_dec = gu2DQSOSCTHRD_DEC[p->channel][RANK_1];
    /* DQSOSCTHRD_INC_RK1 is split into 2 register fields (starting from Vinson) */
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DQSOSCTHRD), (u2Thrd_inc & 0x0FF), SHU_DQSOSCTHRD_DQSOSCTHRD_INC_RK1_7TO0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DQSOSC_PRD), ((u2Thrd_inc & 0xF00) >> 8), SHU_DQSOSC_PRD_DQSOSCTHRD_INC_RK1_11TO8);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DQSOSC_PRD), u2Thrd_dec, SHU_DQSOSC_PRD_DQSOSCTHRD_DEC_RK1);

    //set interval to do MPC(start DQSOSC) command, and dramc send DQSOSC start to rank0/1/2 at the same time
    //TX tracking period unit: 3.9us
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DQSOSCR2), u2DQSOSCENCNT, SHU_DQSOSCR2_DQSOSCENCNT);

    return DRAM_OK;
}

void DramcHwDQSOSC(DRAMC_CTX_T *p)
{
    U8 u1MR23 = 0x3F, shu_index;
    DRAM_RANK_T rank_bak = u1GetRank(p);
    DRAM_CHANNEL_T ch_bak = p->channel;

    //Enable TX tracking new mode
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK15), 1, RK0_PRE_TDQSCK15_SHUFFLE_LEVEL_MODE_SELECT);

    //Enable Freq_RATIO update
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_PRE_TDQSCK1), P_Fld(1, PRE_TDQSCK1_SHU_PRELOAD_TX_HW)
                                            | P_Fld(0, PRE_TDQSCK1_SHU_PRELOAD_TX_START)
                                            | P_Fld(0, PRE_TDQSCK1_SW_UP_TX_NOW_CASE));

    //DQSOSC MPC command violation
#if ENABLE_TMRRI_NEW_MODE
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MPC_OPTION), 1, MPC_OPTION_MPC_BLOCKALE_OPT);
#else
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MPC_OPTION), 0, MPC_OPTION_MPC_BLOCKALE_OPT);
#endif

    //DQS2DQ UI/PI setting controlled by HW
    #if ENABLE_SW_TX_TRACKING
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 1, MISC_CTRL1_R_DMARPIDQ_SW);
    #else
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 0, MISC_CTRL1_R_DMARPIDQ_SW);
    #endif
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR), 1, DQSOSCR_ARUIDQ_SW);

    //Set dqsosc oscillator run time by MRW
    //write RK0 MR23
    #if 0
    vSetRank(p, RANK_0);
    vSetPHY2ChannelMapping(p, CHANNEL_A);
    DramcModeRegWrite(p, 23, u1MR23);
    vSetPHY2ChannelMapping(p, CHANNEL_B);
    DramcModeRegWrite(p, 23, u1MR23);
    //write RK1 MR23
    vSetRank(p, RANK_1);
    vSetPHY2ChannelMapping(p, CHANNEL_A);
    DramcModeRegWrite(p, 23, u1MR23);
    vSetPHY2ChannelMapping(p, CHANNEL_B);
    DramcModeRegWrite(p, 23, u1MR23);
    #endif

    //Enable HW read MR18/MR19 for each rank
    #if ENABLE_SW_TX_TRACKING
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR), 1, DQSOSCR_DQSOSCRDIS);
    #else
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR), 0, DQSOSCR_DQSOSCRDIS);
    #endif

    vSetRank(p, RANK_0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_DQSOSC), 1, RK0_DQSOSC_DQSOSCR_RK0EN);
    if (p->support_rank_num == RANK_DUAL)
    {
        vSetRank(p, RANK_1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK1_DQSOSC), 1, RK1_DQSOSC_DQSOSCR_RK1EN);
    }

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR), 1, DQSOSCR_DQSOSC_CALEN);

    vSetRank(p, rank_bak);
    vSetPHY2ChannelMapping(p, ch_bak);
}

#if ENABLE_TX_TRACKING		 
void Enable_TX_Tracking(DRAMC_CTX_T *p, U32 u4DramcShuOffset, U8 u1ShuffleIdx)
{
    if (u1ShuffleIdx == SRAM_SHU9) //DDR800 do not enable tracking
    {
        vIO32WriteFldAlign_All(DRAMC_REG_SHU_SCINTV+u4DramcShuOffset, 1, SHU_SCINTV_DQSOSCENDIS);
    }
	else
    {
        vIO32WriteFldAlign_All(DRAMC_REG_SHU_SCINTV+u4DramcShuOffset, 0, SHU_SCINTV_DQSOSCENDIS);
    }
}
#endif

#if RDSEL_TRACKING_EN		 
void Enable_RDSEL_Tracking(DRAMC_CTX_T *p, U32 u4DramcShuOffset, U8 u1ShuffleIdx)
{
    if (u1ShuffleIdx != SRAM_SHU9) //DDR800 do not enable tracking
    {
        vIO32WriteFldMulti_All(DRAMC_REG_SHU_RDSEL_TRACK+u4DramcShuOffset, P_Fld(0x1, SHU_RDSEL_TRACK_R_RDSEL_TRACK_EN) 
                                                                         | P_Fld(0x0, SHU_RDSEL_TRACK_R_RDSEL_HWSAVE_MSK));
    }

}
#endif

void Enable_and_Trigger_TX_Retry(DRAMC_CTX_T *p)
{
#if TX_RETRY_CONTROL_BY_SPM
    vIO32WriteFldAlign_All(DRAMC_REG_PPR_CTRL, 1, PPR_CTRL_XSR_TX_RETRY_SPM_MODE); 
#else
    vIO32WriteFldAlign_All(DRAMC_REG_PPR_CTRL, 0, PPR_CTRL_XSR_TX_RETRY_SPM_MODE); 
#endif
    vIO32WriteFldMulti_All(DRAMC_REG_PPR_CTRL, P_Fld(1, PPR_CTRL_TX_RETRY_SHU_RESP_OPT)
                                            | P_Fld(1, PPR_CTRL_TX_RETRY_UPDPI_CG_OPT)
                                            | P_Fld(1, PPR_CTRL_XSR_TX_RETRY_OPT)
                                            | P_Fld(0, PPR_CTRL_XSR_TX_RETRY_EN)
                                            | P_Fld(0, PPR_CTRL_XSR_TX_RETRY_SW_EN)); //If using SW mode, don;t have to set this field
    if (p->frequency == 400)
    {
        vIO32WriteFldAlign_All(DRAMC_REG_SHU_SCINTV, 1, SHU_SCINTV_DQSOSCENDIS);
    }
    else
    {
        vIO32WriteFldAlign_All(DRAMC_REG_SHU_SCINTV, 0, SHU_SCINTV_DQSOSCENDIS);
    }
}

#if ENABLE_SW_TX_TRACKING & __ETT__
void DramcSWTxTracking(DRAMC_CTX_T *p)
{
    DRAM_RANK_T rank_bak = u1GetRank(p);
    U8 u1MR4OnOff = 1;
    U8 rankIdx = RANK_0;
    U8 u1SRAMShuLevel = get_shuffleIndex_by_Freq(p);
    U8 u1CurrentShuLevel = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHUSTATUS), SHUSTATUS_SHUFFLE_LEVEL);
    U16 u2DramcOffset = SHU_GRP_DRAMC_OFFSET * u1CurrentShuLevel;
    U16 u2DdrphyOffset = SHU_GRP_DDRPHY_OFFSET * u1CurrentShuLevel;
    U16 u2MR1819_Base[CHANNEL_NUM][RANK_MAX][2];
    U16 u2MR1819_Runtime[CHANNEL_NUM][RANK_MAX][2];
    U16 u2DQSOSC_INC[RANK_MAX] = {6}, u2DQSOSC_DEC[RANK_MAX] = {4};
    U8 u1AdjPI[RANK_MAX][2];
    U8 u1OriginalPI_DQ[DRAM_DFS_SHUFFLE_MAX][RANK_MAX][2];
    U8 u1UpdatedPI_DQ[DRAM_DFS_SHUFFLE_MAX][RANK_MAX][2];
    U8 u1OriginalPI_DQM[DRAM_DFS_SHUFFLE_MAX][RANK_MAX][2];
    U8 u1UpdatedPI_DQM[DRAM_DFS_SHUFFLE_MAX][RANK_MAX][2];
    U8 u1FreqRatioTX[DRAM_DFS_SHUFFLE_MAX];
    U8 shuIdx=0;

    for(shuIdx=0; shuIdx<DRAM_DFS_SHUFFLE_MAX; shuIdx++)
    { 
        DRAM_DFS_FREQUENCY_TABLE_T *pDstFreqTbl = get_FreqTbl_by_shuffleIndex(p,shuIdx);
        if(pDstFreqTbl==NULL)
        {
            mcSHOW_ERR_MSG(("NULL pFreqTbl\n"));
            while(1);
        }   

        if (pDstFreqTbl->freq_sel == LP4_DDR800)
        {
            u1FreqRatioTX[shuIdx] = 0;
        }
        else
        {
            u1FreqRatioTX[shuIdx] = ((GetFreqBySel(p, pDstFreqTbl->freq_sel))*8)/p->frequency;
            mcSHOW_DBG_MSG(("[SWTxTracking] ShuLevel=%d, Ratio[%d]=%d", u1SRAMShuLevel, shuIdx, u1FreqRatioTX[shuIdx]));
        }
    }

    vSetRank(p, RANK_0);

    /* Starting from Vinson, DQSOSCTHRD_INC & _DEC is split into RK0 and RK1 */
    //Rank 0
    u2DQSOSC_INC[RANK_0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DQSOSCTHRD) + u2DramcOffset, SHU_DQSOSCTHRD_DQSOSCTHRD_INC_RK0);
    u2DQSOSC_DEC[RANK_0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DQSOSCTHRD) + u2DramcOffset, SHU_DQSOSCTHRD_DQSOSCTHRD_DEC_RK0);


    /* DQSOSCTHRD_INC_RK1 is split into 2 register fields (starting from Vinson) */
    //Rank 1
    u2DQSOSC_INC[RANK_1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DQSOSC_PRD) + u2DramcOffset, SHU_DQSOSC_PRD_DQSOSCTHRD_INC_RK1_11TO8);
    u2DQSOSC_INC[RANK_1] = u2DQSOSC_INC[RANK_1] << 8; /* Shift [3:0] to correct position [11:8] */
    u2DQSOSC_INC[RANK_1] = u2DQSOSC_INC[RANK_1] | u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DQSOSCTHRD) + u2DramcOffset, SHU_DQSOSCTHRD_DQSOSCTHRD_INC_RK1_7TO0);

    u2DQSOSC_DEC[RANK_1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DQSOSC_PRD) + u2DramcOffset, SHU_DQSOSC_PRD_DQSOSCTHRD_DEC_RK1);

    for(shuIdx=0; shuIdx<DRAM_DFS_SHUFFLE_MAX; shuIdx++)
    {
        LoadShuffleSRAMtoDramc(p, shuIdx, !u1CurrentShuLevel);
        
        u1OriginalPI_DQ[shuIdx][RANK_0][0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_PI)+(SHU_GRP_DRAMC_OFFSET * (!u1CurrentShuLevel)), SHURK0_PI_RK0_ARPI_DQ_B0);
        u1OriginalPI_DQ[shuIdx][RANK_0][1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_PI)+(SHU_GRP_DRAMC_OFFSET * (!u1CurrentShuLevel)), SHURK0_PI_RK0_ARPI_DQ_B1);
        u1OriginalPI_DQ[shuIdx][RANK_1][0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK1_PI)+(SHU_GRP_DRAMC_OFFSET * (!u1CurrentShuLevel)), SHURK1_PI_RK1_ARPI_DQ_B0);
        u1OriginalPI_DQ[shuIdx][RANK_1][1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK1_PI)+(SHU_GRP_DRAMC_OFFSET * (!u1CurrentShuLevel)), SHURK1_PI_RK1_ARPI_DQ_B1);

        u1OriginalPI_DQM[shuIdx][RANK_0][0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_PI)+(SHU_GRP_DRAMC_OFFSET * (!u1CurrentShuLevel)), SHURK0_PI_RK0_ARPI_DQM_B0);
        u1OriginalPI_DQM[shuIdx][RANK_0][1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_PI)+(SHU_GRP_DRAMC_OFFSET * (!u1CurrentShuLevel)), SHURK0_PI_RK0_ARPI_DQM_B1);
        u1OriginalPI_DQM[shuIdx][RANK_1][0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK1_PI)+(SHU_GRP_DRAMC_OFFSET * (!u1CurrentShuLevel)), SHURK1_PI_RK1_ARPI_DQM_B0);
        u1OriginalPI_DQM[shuIdx][RANK_1][1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK1_PI)+(SHU_GRP_DRAMC_OFFSET * (!u1CurrentShuLevel)), SHURK1_PI_RK1_ARPI_DQM_B1);

        DramcSaveToShuffleSRAM(p, !u1CurrentShuLevel, shuIdx);
    }

    u2MR1819_Base[p->channel][RANK_0][0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQSOSC)+u2DramcOffset, SHURK0_DQSOSC_DQSOSC_BASE_RK0);
    u2MR1819_Base[p->channel][RANK_0][1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQSOSC)+u2DramcOffset, SHURK0_DQSOSC_DQSOSC_BASE_RK0_B1);
    u2MR1819_Base[p->channel][RANK_1][0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK1_DQSOSC)+u2DramcOffset, SHURK1_DQSOSC_DQSOSC_BASE_RK1);
    u2MR1819_Base[p->channel][RANK_1][1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK1_DQSOSC)+u2DramcOffset, SHURK1_DQSOSC_DQSOSC_BASE_RK1_B1);

    u1MR4OnOff = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), SPCMDCTRL_REFRDIS);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), 1, SPCMDCTRL_REFRDIS);
    for(rankIdx=RANK_0; rankIdx<RANK_MAX; rankIdx++)
    {
        U8 byteIdx=0;

        vSetRank(p, rankIdx);
        DramcDQSOSCAuto(p);
        u2MR1819_Runtime[p->channel][p->rank][0] = (gu2MR18[p->channel][p->rank] & 0x00FF) | ((gu2MR19[p->channel][p->rank] & 0x00FF) <<8);
        if(p->dram_cbt_mode[p->rank]==CBT_BYTE_MODE1)
        {
            u2MR1819_Runtime[p->channel][p->rank][1] = (gu2MR18[p->channel][p->rank] >> 8) | ((gu2MR19[p->channel][p->rank] & 0xFF00));
        }
        else
        {
            u2MR1819_Runtime[p->channel][p->rank][1] = u2MR1819_Runtime[p->channel][p->rank][0];
        }
        //INC : MR1819>base. PI-
        //DEC : MR1819<base. PI+
        for(byteIdx=0; byteIdx<2; byteIdx++)
        {
            U16 deltaMR1819=0;

            if(u2MR1819_Runtime[p->channel][p->rank][byteIdx] >= u2MR1819_Base[p->channel][p->rank][byteIdx])
            {
                deltaMR1819  = u2MR1819_Runtime[p->channel][p->rank][byteIdx] - u2MR1819_Base[p->channel][p->rank][byteIdx];
                u1AdjPI[rankIdx][byteIdx] = deltaMR1819/u2DQSOSC_INC[rankIdx];
                for(shuIdx=0; shuIdx<DRAM_DFS_SHUFFLE_MAX; shuIdx++)
                {
                    u1UpdatedPI_DQ[shuIdx][rankIdx][byteIdx] = u1OriginalPI_DQ[shuIdx][rankIdx][byteIdx] - (u1AdjPI[rankIdx][byteIdx]*u1FreqRatioTX[shuIdx]/u1FreqRatioTX[u1SRAMShuLevel]);
                    u1UpdatedPI_DQM[shuIdx][rankIdx][byteIdx] = u1OriginalPI_DQM[shuIdx][rankIdx][byteIdx] - (u1AdjPI[rankIdx][byteIdx]*u1FreqRatioTX[shuIdx]/u1FreqRatioTX[u1SRAMShuLevel]);
                    mcSHOW_DBG_MSG(("SHU%u CH%d RK%d B%d, Base=%X Runtime=%X delta=%d INC=%d PI=0x%B Adj=%d newPI=0x%B\n", shuIdx, p->channel, u1GetRank(p), byteIdx
                            , u2MR1819_Base[p->channel][p->rank][byteIdx], u2MR1819_Runtime[p->channel][p->rank][byteIdx], deltaMR1819, u2DQSOSC_INC[rankIdx]
                            , u1OriginalPI_DQ[shuIdx][rankIdx][byteIdx], (u1AdjPI[rankIdx][byteIdx]*u1FreqRatioTX[shuIdx]/u1FreqRatioTX[u1SRAMShuLevel]), u1UpdatedPI_DQ[shuIdx][rankIdx][byteIdx]));
                }
            }
            else
            {
                deltaMR1819  = u2MR1819_Base[p->channel][p->rank][byteIdx] - u2MR1819_Runtime[p->channel][p->rank][byteIdx];
                u1AdjPI[rankIdx][byteIdx] = deltaMR1819/u2DQSOSC_DEC[rankIdx];
                for(shuIdx=0; shuIdx<DRAM_DFS_SHUFFLE_MAX; shuIdx++)
                {
                    u1UpdatedPI_DQ[shuIdx][rankIdx][byteIdx] = u1OriginalPI_DQ[shuIdx][rankIdx][byteIdx] + (u1AdjPI[rankIdx][byteIdx]*u1FreqRatioTX[shuIdx]/u1FreqRatioTX[u1SRAMShuLevel]);
                    u1UpdatedPI_DQM[shuIdx][rankIdx][byteIdx] = u1OriginalPI_DQM[shuIdx][rankIdx][byteIdx] + (u1AdjPI[rankIdx][byteIdx]*u1FreqRatioTX[shuIdx]/u1FreqRatioTX[u1SRAMShuLevel]);
                    mcSHOW_DBG_MSG(("SHU%u CH%d RK%d B%d, Base=%X Runtime=%X delta=%d DEC=%d PI=0x%B Adj=%d newPI=0x%B\n", shuIdx, p->channel,u1GetRank(p), byteIdx
                            , u2MR1819_Base[p->channel][p->rank][byteIdx], u2MR1819_Runtime[p->channel][p->rank][byteIdx], deltaMR1819, u2DQSOSC_DEC[rankIdx]
                            , u1OriginalPI_DQ[shuIdx][rankIdx][byteIdx], (u1AdjPI[rankIdx][byteIdx]*u1FreqRatioTX[shuIdx]/u1FreqRatioTX[u1SRAMShuLevel]), u1UpdatedPI_DQ[shuIdx][rankIdx][byteIdx]));
                }
            }
        }
    }

    vSetRank(p, RANK_0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR), 1, DQSOSCR_TXUPDMODE);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR), 1, DQSOSCR_MANUTXUPD);

    for(shuIdx=0; shuIdx<DRAM_DFS_SHUFFLE_MAX; shuIdx++)
    {
        LoadShuffleSRAMtoDramc(p, shuIdx, !u1CurrentShuLevel);
			
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ7)+(SHU_GRP_DDRPHY_OFFSET * (!u1CurrentShuLevel)), P_Fld(u1UpdatedPI_DQ[shuIdx][RANK_0][0], SHU_R0_B0_DQ7_RK0_ARPI_DQ_B0)
                                                                 | P_Fld(u1UpdatedPI_DQM[shuIdx][RANK_0][0], SHU_R0_B0_DQ7_RK0_ARPI_DQM_B0));

        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ7)+(SHU_GRP_DDRPHY_OFFSET * (!u1CurrentShuLevel)), P_Fld(u1UpdatedPI_DQ[shuIdx][RANK_0][1], SHU_R0_B1_DQ7_RK0_ARPI_DQ_B1)
                                                                 | P_Fld(u1UpdatedPI_DQM[shuIdx][RANK_0][1], SHU_R0_B1_DQ7_RK0_ARPI_DQM_B1));

        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R1_B0_DQ7)+(SHU_GRP_DDRPHY_OFFSET * (!u1CurrentShuLevel)), P_Fld(u1UpdatedPI_DQ[shuIdx][RANK_1][0], SHU_R1_B0_DQ7_RK1_ARPI_DQ_B0)
                                                                 | P_Fld(u1UpdatedPI_DQM[shuIdx][RANK_1][0], SHU_R1_B0_DQ7_RK1_ARPI_DQM_B0));

        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R1_B1_DQ7)+(SHU_GRP_DDRPHY_OFFSET * (!u1CurrentShuLevel)), P_Fld(u1UpdatedPI_DQ[shuIdx][RANK_1][1], SHU_R1_B1_DQ7_RK1_ARPI_DQ_B1)
                                                                 | P_Fld(u1UpdatedPI_DQM[shuIdx][RANK_1][1], SHU_R1_B1_DQ7_RK1_ARPI_DQM_B1));

        DramcSaveToShuffleSRAM(p, !u1CurrentShuLevel, shuIdx);
    }

    while((u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TCMDO1LAT),TCMDO1LAT_MANUTXUPD_B0_DONE)!=1) && (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TCMDO1LAT),TCMDO1LAT_MANUTXUPD_B1_DONE)!=1))
    {
        mcDELAY_US(1);
    }

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR), 0, DQSOSCR_TXUPDMODE);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR), 0, DQSOSCR_MANUTXUPD);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), u1MR4OnOff, SPCMDCTRL_REFRDIS);
}
#endif

#if ENABLE_RX_TRACKING_LP4
void DramcRxInputDelayTrackingInit_Common(DRAMC_CTX_T *p)
{
    U8 ii, backup_rank;

    backup_rank = u1GetRank(p);

    //Enable RX_FIFO macro DIV4 clock CG
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_MISC_CG_CTRL1), 0xffffffff, MISC_CG_CTRL1_R_DVS_DIV4_CG_CTRL);

    //DVS mode to RG mode
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS2),0x0, R0_B0_RXDVS2_R_RK0_DVS_MODE_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2),0x0, R0_B1_RXDVS2_R_RK0_DVS_MODE_B1);

    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R1_B0_RXDVS2),0x0, R1_B0_RXDVS2_R_RK1_DVS_MODE_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R1_B1_RXDVS2),0x0, R1_B1_RXDVS2_R_RK1_DVS_MODE_B1);

    //Tracking lead/lag counter >> Rx DLY adjustment fixed to 1
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_RXDVS0),0x0, B0_RXDVS0_R_DMRXDVS_CNTCMP_OPT_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_RXDVS0),0x0, B1_RXDVS0_R_DMRXDVS_CNTCMP_OPT_B1);

    //DQIEN pre-state option to block update for RX ASVA  1-2
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_RXDVS0),0x1, B0_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_RXDVS0),0x1, B1_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_B1);

    //Turn off F_DLY individual calibration option (CTO_AGENT_RDAT cannot separate DR/DF error)
    //tracking rising and update rising/falling together
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS2),0x0, R0_B0_RXDVS2_R_RK0_DVS_FDLY_MODE_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2),0x0, R0_B1_RXDVS2_R_RK0_DVS_FDLY_MODE_B1);

    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R1_B0_RXDVS2),0x0, R1_B0_RXDVS2_R_RK1_DVS_FDLY_MODE_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R1_B1_RXDVS2),0x0, R1_B1_RXDVS2_R_RK1_DVS_FDLY_MODE_B1);

    for(ii=RANK_0; ii<RANK_MAX; ii++)
    {
        vSetRank(p, ii);

        //DQ/DQM/DQS DLY MAX/MIN value under Tracking mode
        /* Byte 0 */
#if (fcFOR_CHIP_ID == fcLafite)
        /* DQS, DQ, DQM (DQ, DQM are tied together now) -> controlled using DQM MAX_MIN */
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS7), P_Fld(0x0, R0_B0_RXDVS7_RG_RK0_ARDQ_MIN_DLY_B0) | P_Fld(0x3f, R0_B0_RXDVS7_RG_RK0_ARDQ_MAX_DLY_B0)
                                                                  | P_Fld(0x0, R0_B0_RXDVS7_RG_RK0_ARDQS0_MIN_DLY_B0) | P_Fld(0x7f, R0_B0_RXDVS7_RG_RK0_ARDQS0_MAX_DLY_B0));
#else
        /* Previous design DQ, DQM MAX/MIN are controlled using different register fields */
        /* DQM, DQS */
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS7), P_Fld(0x0, R0_B0_RXDVS7_RG_RK0_ARDQM0_MIN_DLY_B0) | P_Fld(0x3f, R0_B0_RXDVS7_RG_RK0_ARDQM0_MAX_DLY_B0)
                                                                  | P_Fld(0x0, R0_B0_RXDVS7_RG_RK0_ARDQS0_MIN_DLY_B0) | P_Fld(0x7f, R0_B0_RXDVS7_RG_RK0_ARDQS0_MAX_DLY_B0));
        /* DQ */
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS3), P_Fld(0x0, R0_B0_RXDVS3_RG_RK0_ARDQ0_MIN_DLY_B0) | P_Fld(0x3f, R0_B0_RXDVS3_RG_RK0_ARDQ0_MAX_DLY_B0));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS3), P_Fld(0x0, R0_B0_RXDVS3_RG_RK0_ARDQ1_MIN_DLY_B0) | P_Fld(0x3f, R0_B0_RXDVS3_RG_RK0_ARDQ1_MAX_DLY_B0));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS4), P_Fld(0x0, R0_B0_RXDVS4_RG_RK0_ARDQ2_MIN_DLY_B0) | P_Fld(0x3f, R0_B0_RXDVS4_RG_RK0_ARDQ2_MAX_DLY_B0));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS4), P_Fld(0x0, R0_B0_RXDVS4_RG_RK0_ARDQ3_MIN_DLY_B0) | P_Fld(0x3f, R0_B0_RXDVS4_RG_RK0_ARDQ3_MAX_DLY_B0));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS5), P_Fld(0x0, R0_B0_RXDVS5_RG_RK0_ARDQ4_MIN_DLY_B0) | P_Fld(0x3f, R0_B0_RXDVS5_RG_RK0_ARDQ4_MAX_DLY_B0));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS5), P_Fld(0x0, R0_B0_RXDVS5_RG_RK0_ARDQ5_MIN_DLY_B0) | P_Fld(0x3f, R0_B0_RXDVS5_RG_RK0_ARDQ5_MAX_DLY_B0));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS6), P_Fld(0x0, R0_B0_RXDVS6_RG_RK0_ARDQ6_MIN_DLY_B0) | P_Fld(0x3f, R0_B0_RXDVS6_RG_RK0_ARDQ6_MAX_DLY_B0));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS6), P_Fld(0x0, R0_B0_RXDVS6_RG_RK0_ARDQ7_MIN_DLY_B0) | P_Fld(0x3f, R0_B0_RXDVS6_RG_RK0_ARDQ7_MAX_DLY_B0));
#endif

        /* Byte 1 */
#if (fcFOR_CHIP_ID == fcLafite)
        /* DQS, DQ, DQM (DQ, DQM are tied together now) -> controlled using DQM MAX_MIN */
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS7), P_Fld(0x0, R0_B1_RXDVS7_RG_RK0_ARDQ_MIN_DLY_B1) | P_Fld(0x3f, R0_B1_RXDVS7_RG_RK0_ARDQ_MAX_DLY_B1)
                                                                  | P_Fld(0x0, R0_B1_RXDVS7_RG_RK0_ARDQS0_MIN_DLY_B1) | P_Fld(0x7f, R0_B1_RXDVS7_RG_RK0_ARDQS0_MAX_DLY_B1));
#else
        /* Previous design DQ, DQM MAX/MIN are controlled using different register fields */
        /* DQM, DQS */
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS7), P_Fld(0x0, R0_B1_RXDVS7_RG_RK0_ARDQM0_MIN_DLY_B1) | P_Fld(0x3f, R0_B1_RXDVS7_RG_RK0_ARDQM0_MAX_DLY_B1) 
                                                                  | P_Fld(0x0, R0_B1_RXDVS7_RG_RK0_ARDQS0_MIN_DLY_B1) | P_Fld(0x7f, R0_B1_RXDVS7_RG_RK0_ARDQS0_MAX_DLY_B1));
        /* DQ */
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS3), P_Fld(0x0, R0_B1_RXDVS3_RG_RK0_ARDQ0_MIN_DLY_B1) | P_Fld(0x3f, R0_B1_RXDVS3_RG_RK0_ARDQ0_MAX_DLY_B1));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS3), P_Fld(0x0, R0_B1_RXDVS3_RG_RK0_ARDQ1_MIN_DLY_B1) | P_Fld(0x3f, R0_B1_RXDVS3_RG_RK0_ARDQ1_MAX_DLY_B1));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS4), P_Fld(0x0, R0_B1_RXDVS4_RG_RK0_ARDQ2_MIN_DLY_B1) | P_Fld(0x3f, R0_B1_RXDVS4_RG_RK0_ARDQ2_MAX_DLY_B1));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS4), P_Fld(0x0, R0_B1_RXDVS4_RG_RK0_ARDQ3_MIN_DLY_B1) | P_Fld(0x3f, R0_B1_RXDVS4_RG_RK0_ARDQ3_MAX_DLY_B1));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS5), P_Fld(0x0, R0_B1_RXDVS5_RG_RK0_ARDQ4_MIN_DLY_B1) | P_Fld(0x3f, R0_B1_RXDVS5_RG_RK0_ARDQ4_MAX_DLY_B1));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS5), P_Fld(0x0, R0_B1_RXDVS5_RG_RK0_ARDQ5_MIN_DLY_B1) | P_Fld(0x3f, R0_B1_RXDVS5_RG_RK0_ARDQ5_MAX_DLY_B1));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS6), P_Fld(0x0, R0_B1_RXDVS6_RG_RK0_ARDQ6_MIN_DLY_B1) | P_Fld(0x3f, R0_B1_RXDVS6_RG_RK0_ARDQ6_MAX_DLY_B1));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS6), P_Fld(0x0, R0_B1_RXDVS6_RG_RK0_ARDQ7_MIN_DLY_B1) | P_Fld(0x3f, R0_B1_RXDVS6_RG_RK0_ARDQ7_MAX_DLY_B1));
#endif

        //Threshold for LEAD/LAG filter
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS1), P_Fld(0x2, R0_B0_RXDVS1_R_RK0_B0_DVS_TH_LEAD) | P_Fld(0x2, R0_B0_RXDVS1_R_RK0_B0_DVS_TH_LAG));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS1), P_Fld(0x2, R0_B1_RXDVS1_R_RK0_B1_DVS_TH_LEAD) | P_Fld(0x2, R0_B1_RXDVS1_R_RK0_B1_DVS_TH_LAG));

        //DQ/DQS Rx DLY adjustment for tracking mode
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS2), P_Fld(0x0, R0_B0_RXDVS2_R_RK0_RX_DLY_RIS_DQ_SCALE_B0) | P_Fld(0x0, R0_B0_RXDVS2_R_RK0_RX_DLY_RIS_DQS_SCALE_B0));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2), P_Fld(0x0, R0_B1_RXDVS2_R_RK0_RX_DLY_RIS_DQ_SCALE_B1) | P_Fld(0x0, R0_B1_RXDVS2_R_RK0_RX_DLY_RIS_DQS_SCALE_B1));

        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS2), P_Fld(0x0, R0_B0_RXDVS2_R_RK0_RX_DLY_FAL_DQ_SCALE_B0) | P_Fld(0x0, R0_B0_RXDVS2_R_RK0_RX_DLY_FAL_DQS_SCALE_B0));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2), P_Fld(0x0, R0_B1_RXDVS2_R_RK0_RX_DLY_FAL_DQ_SCALE_B1) | P_Fld(0x0, R0_B1_RXDVS2_R_RK0_RX_DLY_FAL_DQS_SCALE_B1));
    }

    vSetRank(p, backup_rank);
}
#endif

void DramcRxInputDelayTrackingInit_byFreq(DRAMC_CTX_T *p)
{
    U8 u1DVS_Delay;
    //Monitor window size setting
    //DDRPHY.SHU*_B*_DQ5.RG_RX_ARDQS0_DVS_DLY_B* (suggested value from A-PHY owner)
//WHITNEY_TO_BE_PORTING
#if (fcFOR_CHIP_ID == fcLafite)
    //          Speed   Voltage     DVS_DLY
    //======================================
    //SHU1      3200    0.8V        3
    //SHU2      2667    0.8V-0.7V   4
    //SHU3      1600    0.7V-0.65V  5
    if(p->freqGroup == 2133)
    {
        u1DVS_Delay =2;    
    }
    else if(p->freqGroup == 1866)        
    {
        u1DVS_Delay =3;    
    }
    else if(p->freqGroup == 1600)
    {
        u1DVS_Delay =3;
    }
    else if(p->freqGroup == 1333 || p->freqGroup == 1200)
    {
        u1DVS_Delay =4;
    }
    else// if(p->freqGroup == 800)
    {
        u1DVS_Delay =5;
    }
#endif

    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ5), u1DVS_Delay, SHU_B0_DQ5_RG_RX_ARDQS0_DVS_DLY_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_SHU_B1_DQ5), u1DVS_Delay, SHU_B1_DQ5_RG_RX_ARDQS0_DVS_DLY_B1);

    /* Bian_co HW design issue: run-time PBYTE flag will lose it's function and become per-bit -> set to 0 */
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ7), P_Fld(0x0, SHU_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0)
                                                            | P_Fld(0x0, SHU_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0));
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_SHU_B1_DQ7), P_Fld(0x0, SHU_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1)
                                                            | P_Fld(0x0, SHU_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1));
}

#if ENABLE_RX_TRACKING_LP4
void DramcRxInputDelayTrackingHW(DRAMC_CTX_T *p)
{
    DRAM_CHANNEL_T channel_bak = p->channel;
    U8 updateDone=0;
    U16 u2DVS_TH=0x0;
    U16 u2MinDly=0x14;
    U16 u2MaxDly=0x30;
    U8 ii, backup_rank;

    vSetPHY2ChannelMapping(p, CHANNEL_A);
    backup_rank = u1GetRank(p);

    //Rx DLY tracking setting (Static)
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_B0_RXDVS0),
                                           P_Fld(1, B0_RXDVS0_R_RX_DLY_TRACK_SPM_CTRL_B0) |
                                           P_Fld(0, B0_RXDVS0_R_RX_RANKINCTL_B0)|
                                           P_Fld(1, B0_RXDVS0_R_RX_RANKINSEL_B0));

    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_B1_RXDVS0),
                                           P_Fld(1, B1_RXDVS0_R_RX_DLY_TRACK_SPM_CTRL_B1) |
                                           P_Fld(0, B1_RXDVS0_R_RX_RANKINCTL_B1)|
                                           P_Fld(1, B1_RXDVS0_R_RX_RANKINSEL_B1));

#if (fcFOR_CHIP_ID == fcLafite)
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ9), P_Fld(0x1, B0_DQ9_R_DMRXDVS_RDSEL_LAT_B0 | P_Fld(0, B0_DQ9_R_DMRXDVS_VALID_LAT_B0)));
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ9), P_Fld(0x1, B1_DQ9_R_DMRXDVS_RDSEL_LAT_B1) | P_Fld(0, B1_DQ9_R_DMRXDVS_VALID_LAT_B1));
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_CA_CMD10), P_Fld(0,CA_CMD10_R_DMRXDVS_RDSEL_LAT_CA) | P_Fld(0, CA_CMD10_R_DMRXDVS_VALID_LAT_CA));

    /* DMRXTRACK_DQM_B* (rxdly_track SM DQM enable) -> need to be set to 1 if R_DBI is on
     *  They are shuffle regs -> move setting to DramcSetting_Olympus_LP4_ByteMode()
     */

    //Enable A-PHY DVS LEAD/LAG
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ5), 0x1, B0_DQ5_RG_RX_ARDQS0_DVS_EN_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ5), 0x1, B1_DQ5_RG_RX_ARDQS0_DVS_EN_B1);
#else
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_MISC_RXDVS2), P_Fld(0x1, MISC_RXDVS2_R_RXDVS_RDSEL_TOG_LAT)
                                                            | P_Fld(0x2, MISC_RXDVS2_R_RXDVS_RDSEL_BUS_LAT));
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 0x0, MISC_CTRL1_R_DMDQMDBI);

    //Enable A-PHY DVS LEAD/LAG
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ5), 0x1, B0_DQ5_RG_RX_ARDQS0_DVS_EN_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ5), 0x1, B1_DQ5_RG_RX_ARDQS0_DVS_EN_B1);
#endif

    //Rx DLY tracking function CG enable
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_RXDVS0), 0x1, B0_RXDVS0_R_RX_DLY_TRACK_CG_EN_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_RXDVS0), 0x1, B1_RXDVS0_R_RX_DLY_TRACK_CG_EN_B1);

    //Rx DLY tracking lead/lag counter enable
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_RXDVS0), 0x1, B0_RXDVS0_R_RX_DLY_TRACK_ENA_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_RXDVS0), 0x1, B1_RXDVS0_R_RX_DLY_TRACK_ENA_B1);

    for(ii=RANK_0; ii<RANK_MAX; ii++)
    {
        vSetRank(p, ii);

        //Rx DLY tracking update enable (HW mode)
        #if 0
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS2), 0x2, R0_B0_RXDVS2_R_RK0_DVS_MODE_B0);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2), 0x2, R0_B1_RXDVS2_R_RK0_DVS_MODE_B1);

        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS2), 0x1, R0_B0_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B0);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2), 0x1, R0_B1_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B1);

        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS2), 0x1, R0_B0_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B0);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2), 0x1, R0_B1_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B1);
        #else
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS2),
                                               P_Fld(2, R0_B0_RXDVS2_R_RK0_DVS_MODE_B0) |
                                               P_Fld(1, R0_B0_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B0)|
                                               P_Fld(1, R0_B0_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B0));

        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2),
                                               P_Fld(2, R0_B1_RXDVS2_R_RK0_DVS_MODE_B1) |
                                               P_Fld(1, R0_B1_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B1)|
                                               P_Fld(1, R0_B1_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B1));
        #endif
    }

    vSetRank(p, backup_rank);

#if 0
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_RXDVS0),0x1, B0_RXDVS0_R_RX_DLY_RK_OPT_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_RXDVS0),0x1, B1_RXDVS0_R_RX_DLY_RK_OPT_B1);

    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 1, MISC_CTRL1_R_HWSAVE_MODE_ENA);
#endif

    vSetPHY2ChannelMapping(p, channel_bak);
}
#endif

#if RX_DLY_TRACK_ONLY_FOR_DEBUG
void DramcRxDlyTrackDebug(DRAMC_CTX_T *p)
{
    /* indicate ROW_ADR = 2 for dummy write & read for Rx dly track debug feature, avoid pattern overwrite by MEM_TEST
     * pattern(0xAAAA5555) locates: 0x40010000, 0x40010100, 0x80010000, 0x80010100 */
    
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_RK0_DUMMY_RD_ADR), P_Fld(2, RK0_DUMMY_RD_ADR_DMY_RD_RK0_ROW_ADR)
                        | P_Fld(0, RK0_DUMMY_RD_ADR_DMY_RD_RK0_COL_ADR)
                        | P_Fld(0, RK0_DUMMY_RD_ADR_DMY_RD_RK0_LEN));
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_RK1_DUMMY_RD_ADR), P_Fld(2, RK1_DUMMY_RD_ADR_DMY_RD_RK1_ROW_ADR)
                        | P_Fld(0, RK1_DUMMY_RD_ADR_DMY_RD_RK1_COL_ADR)
                        | P_Fld(0, RK1_DUMMY_RD_ADR_DMY_RD_RK1_LEN));
    vIO32WriteFldAlign_All(DRAMC_REG_RK0_DUMMY_RD_BK, 0, RK0_DUMMY_RD_BK_DMY_RD_RK0_BK);
    vIO32WriteFldAlign_All(DRAMC_REG_RK1_DUMMY_RD_BK, 0, RK1_DUMMY_RD_BK_DMY_RD_RK1_BK);
    
    vIO32Write4B_All(DRAMC_REG_RK0_DUMMY_RD_WDATA0, 0xAAAA5555);
    vIO32Write4B_All(DRAMC_REG_RK0_DUMMY_RD_WDATA1, 0xAAAA5555);
    vIO32Write4B_All(DRAMC_REG_RK0_DUMMY_RD_WDATA2, 0xAAAA5555);
    vIO32Write4B_All(DRAMC_REG_RK0_DUMMY_RD_WDATA3, 0xAAAA5555);
    vIO32Write4B_All(DRAMC_REG_RK1_DUMMY_RD_WDATA0, 0xAAAA5555);
    vIO32Write4B_All(DRAMC_REG_RK1_DUMMY_RD_WDATA1, 0xAAAA5555);
    vIO32Write4B_All(DRAMC_REG_RK1_DUMMY_RD_WDATA2, 0xAAAA5555);
    vIO32Write4B_All(DRAMC_REG_RK1_DUMMY_RD_WDATA3, 0xAAAA5555);

    //disable Rx dly track debug and clear status lock
    vIO32WriteFldMulti_All((DDRPHY_MISC_RXDVS2), P_Fld(0, MISC_RXDVS2_R_DMRXDVS_DBG_MON_EN)
                        | P_Fld(1, MISC_RXDVS2_R_DMRXDVS_DBG_MON_CLR)
                        | P_Fld(0, MISC_RXDVS2_R_DMRXDVS_DBG_PAUSE_EN));

    //trigger dummy write pattern 0xAAAA5555
    vIO32WriteFldAlign_All(DRAMC_REG_DUMMY_RD, 0x1, DUMMY_RD_DMY_WR_DBG);
    vIO32WriteFldAlign_All(DRAMC_REG_DUMMY_RD, 0x0, DUMMY_RD_DMY_WR_DBG);

    // enable Rx dly track debug feature
    vIO32WriteFldMulti_All((DDRPHY_MISC_RXDVS2), P_Fld(1, MISC_RXDVS2_R_DMRXDVS_DBG_MON_EN)
                        | P_Fld(0, MISC_RXDVS2_R_DMRXDVS_DBG_MON_CLR)
                        | P_Fld(1, MISC_RXDVS2_R_DMRXDVS_DBG_PAUSE_EN));
}

void DramcPrintRxDlyTrackDebugStatus(DRAMC_CTX_T *p)
{
    U32 backup_rank, u1ChannelBak,u4value;
    U8 u1ChannelIdx, u1ChannelMax=p->support_channel_num;//channel A/B ...
    
    u1ChannelBak= p->channel;
    backup_rank = u1GetRank(p);

    for (u1ChannelIdx=CHANNEL_A; u1ChannelIdx<p->support_channel_num; u1ChannelIdx++)
    {
        p->channel = u1ChannelIdx;
        
        u4value= u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_DQ_RXDLY_TRRO18));
        //mcSHOW_DBG_MSG(("\nCH_%d DQ_RXDLY_TRRO18 = 0x\033[1;36m%x\033[m\n",u1ChannelIdx,u4value));
        if (u4value&1)
        {
            mcSHOW_DBG_MSG(("=== CH_%d DQ_RXDLY_TRRO18 = 0x\033[1;36m%x\033[m, %s %s shu: %d\n",u1ChannelIdx,u4value,
                        u4value&0x2?"RK0: fail":"",u4value&0x4?"RK1: fail":"",(u4value>>4)&0x3));
        }
    }
}
#endif

#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
#if (__ETT__ || CPU_RW_TEST_AFTER_K)
void DramcPrintRXDQDQSStatus(DRAMC_CTX_T *p, U8 u1Channel)
{
    U8 u1RankIdx, u1RankMax, u1ChannelBak, u1ByteIdx, ii;
    U32 u4ResultDQS_PI, u4ResultDQS_UI, u4ResultDQS_UI_P1;
    U8 u1Dqs_pi[DQS_BIT_NUMBER], u1Dqs_ui[DQS_BIT_NUMBER],u1Dqs_ui_P1[DQS_BIT_NUMBER];
    U16 u2TmpValue, u2TmpUI[DQS_NUMBER], u2TmpPI[DQS_NUMBER];
    U32 MANUDLLFRZ_bak, STBSTATE_OPT_bak;
    U32 backup_rank;
    U8 u1DQX_B0, u1DQS0, u1DQX_B1, u1DQS1;

    u1ChannelBak= p->channel;
    vSetPHY2ChannelMapping(p, u1Channel);
    backup_rank = u1GetRank(p);

    if (p->support_rank_num==RANK_DUAL)
        u1RankMax = RANK_MAX;
    else
         u1RankMax =RANK_1;

    for(u1RankIdx=0; u1RankIdx<u1RankMax; u1RankIdx++)
    {
        vSetRank(p, u1RankIdx);
        mcSHOW_DBG_MSG(("[RXDQDQSStatus] CH%d, RK%d\n", p->channel, u1RankIdx));
        if (u1RankIdx==0)
        u4ResultDQS_PI = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_DQ_RXDLY_TRRO22));
        if (u1RankIdx==1)
        u4ResultDQS_PI = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_DQ_RXDLY_TRRO23));

        u1DQX_B0  = u4ResultDQS_PI & 0xff;
        u1DQS0  = (u4ResultDQS_PI >>8) & 0xff;
        u1DQX_B1  = (u4ResultDQS_PI >>16) & 0xff;
        u1DQS1  = (u4ResultDQS_PI >>24) & 0xff;

        mcSHOW_DBG_MSG(("DQX_B0, DQS0, DQX_B1, DQS1 =(%d, %d, %d, %d)\n\n", u1DQX_B0, u1DQS0, u1DQX_B1, u1DQS1));

        }
    vSetRank(p, backup_rank);

    p->channel = u1ChannelBak;
    vSetPHY2ChannelMapping(p, u1ChannelBak);
}
#endif
#endif

void DummyReadForDqsGatingRetryShuffle(DRAMC_CTX_T *p, bool bEn)
{
    if(bEn == 1)
    {
        vIO32WriteFldMulti_All(DRAMC_REG_SHU_DQSG_RETRY, P_Fld(0, SHU_DQSG_RETRY_R_RETRY_ROUND_NUM)//Retry once
                                     | P_Fld(1, SHU_DQSG_RETRY_R_XSR_RETRY_SPM_MODE)
                                     | P_Fld(0, SHU_DQSG_RETRY_R_XSR_DQSG_RETRY_EN)
                                     | P_Fld(0, SHU_DQSG_RETRY_R_DQSG_RETRY_SW_EN)
                                     | P_Fld(1, SHU_DQSG_RETRY_R_RETRY_USE_BURST_MDOE)
                                     | P_Fld(1, SHU_DQSG_RETRY_R_RDY_SEL_DLE)
                                     | P_Fld(1, SHU_DQSG_RETRY_R_DDR1866_PLUS));
    }
    else
    {
        vIO32WriteFldMulti_All(DRAMC_REG_SHU_DQSG_RETRY, P_Fld(0, SHU_DQSG_RETRY_R_XSR_RETRY_SPM_MODE)
                                     | P_Fld(0, SHU_DQSG_RETRY_R_XSR_DQSG_RETRY_EN)
                                     | P_Fld(0, SHU_DQSG_RETRY_R_DQSG_RETRY_SW_EN));
    }
    return;
}

void DummyReadForDqsGatingRetryNonShuffle(DRAMC_CTX_T *p, bool bEn)
{
    if(bEn == 1)
    {
        vIO32WriteFldAlign_All(DRAMC_REG_TEST2_4, 4, TEST2_4_TESTAGENTRKSEL);//Dummy Read rank selection is controlled by Test Agent
        vIO32WriteFldMulti_All(DRAMC_REG_DUMMY_RD, P_Fld(1, DUMMY_RD_DQSG_DMYRD_EN)
                                     | P_Fld(p->support_rank_num, DUMMY_RD_RANK_NUM)
                                     | P_Fld(1, DUMMY_RD_DUMMY_RD_SW));
        vIO32WriteFldAlign_All(DRAMC_REG_RK1_DUMMY_RD_ADR, 0, RK1_DUMMY_RD_ADR_DMY_RD_RK1_LEN);
        vIO32WriteFldAlign_All(DRAMC_REG_RK0_DUMMY_RD_ADR, 0, RK0_DUMMY_RD_ADR_DMY_RD_RK0_LEN);
    }
    else
    {
    }
    return;
}

#ifdef DUMMY_READ_FOR_TRACKING
void DramcDummyReadAddressSetting(DRAMC_CTX_T *p)
{
    U8 backup_channel= p->channel, backup_rank= p->rank;
    U8 channelIdx, rankIdx;
    dram_addr_t dram_addr;
    
    for (channelIdx=CHANNEL_A; channelIdx<CHANNEL_NUM; channelIdx++)
    {
        vSetPHY2ChannelMapping(p, channelIdx);
        for (rankIdx=RANK_0; rankIdx<RANK_MAX; rankIdx++)
        {
            vSetRank(p, rankIdx);
            
            dram_addr.ch = channelIdx;
            dram_addr.rk = rankIdx;
            
            get_dummy_read_addr(&dram_addr);
            mcSHOW_DBG_MSG3(("=== dummy read address: CH_%d, RK%d, row: 0x%x, bk: %d, col: 0x%x\n\n",
                    channelIdx,rankIdx,dram_addr.row,dram_addr.bk,dram_addr.col));

            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RK0_DUMMY_RD_ADR), P_Fld(dram_addr.row, RK0_DUMMY_RD_ADR_DMY_RD_RK0_ROW_ADR)
                                | P_Fld(dram_addr.col, RK0_DUMMY_RD_ADR_DMY_RD_RK0_COL_ADR)
                                | P_Fld(0, RK0_DUMMY_RD_ADR_DMY_RD_RK0_LEN));
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_DUMMY_RD_BK), dram_addr.bk, RK0_DUMMY_RD_BK_DMY_RD_RK0_BK);
        }
    }
    
    vSetPHY2ChannelMapping(p, backup_channel);
    vSetRank(p, backup_rank);
    
}

void DramcDummyReadForTrackingEnable(DRAMC_CTX_T *p)
{
    vIO32WriteFldAlign_All(DRAMC_REG_DUMMY_RD, p->support_rank_num, DUMMY_RD_RANK_NUM);
    /* Dummy read pattern (Better efficiency during rx dly tracking) DE: YH Tsai, Wei-jen */
    vIO32Write4B_All(DRAMC_REG_RK0_DUMMY_RD_WDATA0, 0xAAAA5555); // Field RK0_DUMMY_RD_WDATA0_DMY_RD_RK0_WDATA0
    vIO32Write4B_All(DRAMC_REG_RK0_DUMMY_RD_WDATA1, 0xAAAA5555); // Field RK0_DUMMY_RD_WDATA1_DMY_RD_RK0_WDATA1
    vIO32Write4B_All(DRAMC_REG_RK0_DUMMY_RD_WDATA2, 0xAAAA5555); // Field RK0_DUMMY_RD_WDATA2_DMY_RD_RK0_WDATA2
    vIO32Write4B_All(DRAMC_REG_RK0_DUMMY_RD_WDATA3, 0xAAAA5555); // Field RK0_DUMMY_RD_WDATA3_DMY_RD_RK0_WDATA3

    vIO32Write4B_All(DRAMC_REG_RK1_DUMMY_RD_WDATA0, 0xAAAA5555); // Field RK1_DUMMY_RD_WDATA0_DMY_RD_RK1_WDATA0
    vIO32Write4B_All(DRAMC_REG_RK1_DUMMY_RD_WDATA1, 0xAAAA5555); // Field RK1_DUMMY_RD_WDATA1_DMY_RD_RK1_WDATA1
    vIO32Write4B_All(DRAMC_REG_RK1_DUMMY_RD_WDATA2, 0xAAAA5555); // Field RK1_DUMMY_RD_WDATA2_DMY_RD_RK1_WDATA2
    vIO32Write4B_All(DRAMC_REG_RK1_DUMMY_RD_WDATA3, 0xAAAA5555); // Field RK1_DUMMY_RD_WDATA3_DMY_RD_RK1_WDATA3

    vIO32WriteFldAlign_All(DRAMC_REG_TEST2_4, 4, TEST2_4_TESTAGENTRKSEL);//Dummy Read rank selection is controlled by Test Agent
    
#if 0//__ETT__
    /* indicate ROW_ADR = 2 for Dummy Write pattern address, in order to avoid pattern will be overwrited by MEM_TEST(test range 0xffff)
     * Pattern locates: 0x40010000, 0x40010100, 0x80010000, 0x80010100 */
    dram_addr_t dram_addr;
    
    dram_addr.ch = 0;
    dram_addr.rk = 0;
    get_dummy_read_addr(&dram_addr);
    
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_RK0_DUMMY_RD_ADR), P_Fld(dram_addr.row, RK0_DUMMY_RD_ADR_DMY_RD_RK0_ROW_ADR)
                        | P_Fld(dram_addr.col, RK0_DUMMY_RD_ADR_DMY_RD_RK0_COL_ADR)
                        | P_Fld(0, RK0_DUMMY_RD_ADR_DMY_RD_RK0_LEN));
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_RK0_DUMMY_RD_BK), dram_addr.bk, RK0_DUMMY_RD_BK_DMY_RD_RK0_BK);
    
    dram_addr.rk = 1;
    get_dummy_read_addr(&dram_addr);
    
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_RK1_DUMMY_RD_ADR), P_Fld(dram_addr.row, RK1_DUMMY_RD_ADR_DMY_RD_RK1_ROW_ADR)
                        | P_Fld(dram_addr.col, RK1_DUMMY_RD_ADR_DMY_RD_RK1_COL_ADR)
                        | P_Fld(0, RK1_DUMMY_RD_ADR_DMY_RD_RK1_LEN));
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_RK1_DUMMY_RD_BK), dram_addr.bk, RK1_DUMMY_RD_BK_DMY_RD_RK1_BK);

    /* trigger dummy write pattern 0xAAAA5555 */
    vIO32WriteFldAlign_All(DRAMC_REG_DUMMY_RD, 0x1, DUMMY_RD_DMY_WR_DBG);
    vIO32WriteFldAlign_All(DRAMC_REG_DUMMY_RD, 0x0, DUMMY_RD_DMY_WR_DBG);
#else
    DramcDummyReadAddressSetting(p);
#endif

    /* DUMMY_RD_RX_TRACK = 1:
     * During "RX input delay tracking enable" and "DUMMY_RD_EN=1" Dummy read will force a read command to a certain rank,
     * ignoring whether or not EMI has executed a read command to that certain rank in the past 4us.
     */
    if(p->frequency >= 1600)
    {
        vIO32WriteFldMulti_All(DRAMC_REG_DUMMY_RD, P_Fld(1, DUMMY_RD_DMY_RD_RX_TRACK) | P_Fld(1, DUMMY_RD_DUMMY_RD_EN));
        mcSHOW_DBG_MSG(("High Freq DUMMY_READ_FOR_TRACKING: ON\n"));
    }
    else
    {
        mcSHOW_DBG_MSG(("Low Freq DUMMY_READ_FOR_TRACKING: OFF\n"));
    }
    return;
}
#endif



#ifdef IMPEDANCE_HW_SAVING
void DramcImpedanceHWSaving(DRAMC_CTX_T *p)
{
    vIO32WriteFldAlign_All(DRAMC_REG_IMPCAL, 1, IMPCAL_IMPCAL_HWSAVE_EN);
}
#endif
#ifdef IMPEDANCE_TRACKING_ENABLE
void DramcImpedanceTrackingEnable(DRAMC_CTX_T *p)
{
    #if 0  //Impedance tracking offset for DRVP+2
    vIO32WriteFldMulti_All(DRAMC_REG_IMPEDAMCE_CTRL1, P_Fld(2, IMPEDAMCE_CTRL1_DQS1_OFF) | P_Fld(2, IMPEDAMCE_CTRL1_DOS2_OFF));
    vIO32WriteFldMulti_All(DRAMC_REG_IMPEDAMCE_CTRL2, P_Fld(2, IMPEDAMCE_CTRL2_DQ1_OFF) | P_Fld(2, IMPEDAMCE_CTRL2_DQ2_OFF));
    #endif

    //Write (DRAMC _BASE+ 0x8B) [31:0] = 32'he4000000//enable impedance tracking
    vIO32WriteFldAlign(DDRPHY_MISC_CTRL0, 0x1, MISC_CTRL0_IMPCAL_CHAB_EN);//Set CHA this bit to enable dual channel tracking

    //During shuffle, after CH_A IMP update done, CH_B has no enough time to update (IMPCAL_IMPCAL_DRVUPDOPT=1)
    //enable ECO function for impedance load last tracking result of previous shuffle level (IMPCAL_IMPCAL_CHGDRV_ECO_OPT=1)
    //enable ECO function for impcal_sm hange when DRVP>=0x1D (IMPCAL_IMPCAL_SM_ECO_OPT=1)
    vIO32WriteFldMulti_All(DRAMC_REG_IMPCAL, P_Fld(1, IMPCAL_IMPCAL_HW) | P_Fld(0, IMPCAL_IMPCAL_EN) | P_Fld(1, IMPCAL_IMPCAL_SWVALUE_EN) | 
                                             P_Fld(1, IMPCAL_IMPCAL_NEW_OLD_SL) | P_Fld(1, IMPCAL_IMPCAL_DRVUPDOPT) |
                                             P_Fld(1, IMPCAL_IMPCAL_CHGDRV_ECO_OPT)|P_Fld(1, IMPCAL_IMPCAL_SM_ECO_OPT) |
                                             P_Fld(1, IMPCAL_IMPCAL_ECO_OPT)|P_Fld(1, IMPCAL_DRV_ECO_OPT));

    //dual channel continuously tracking @ system busy, self-refresh, Hhbrid-S1
    vIO32WriteFldAlign_All(DDRPHY_MISC_CTRL0, 0x1, MISC_CTRL0_IMPCAL_LP_ECO_OPT);
    
    // no update imp CA, because CA is unterm now
    vIO32WriteFldAlign_All(DRAMC_REG_IMPCAL, 1, IMPCAL_IMPCAL_BYPASS_UP_CA_DRV);

    // CH_A set 1, CH_B set 0 (mp setting)
    vIO32WriteFldMulti(DRAMC_REG_IMPCAL, P_Fld(0, IMPCAL_DIS_SUS_CH0_DRV) | P_Fld(1, IMPCAL_DIS_SUS_CH1_DRV));
    vIO32WriteFldMulti(DRAMC_REG_IMPCAL+SHIFT_TO_CHB_ADDR, P_Fld(1, IMPCAL_DIS_SUS_CH0_DRV) | P_Fld(0, IMPCAL_DIS_SUS_CH1_DRV));

    //Maoauo: keep following setting for SPMFW enable REFCTRL0_DRVCGWREF = 1 (Imp SW Save mode)
    //vIO32WriteFldAlign(DRAMC_REG_REFCTRL0+((U32)CHANNEL_B<<POS_BANK_NUM), 1, REFCTRL0_DRVCGWREF);
    vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL0, 1, REFCTRL0_DQDRVSWUPD);
}
#endif

void DramcPrintIMPTrackingStatus(DRAMC_CTX_T *p, U8 u1Channel)
{
#ifdef IMPEDANCE_TRACKING_ENABLE

    U8 u1RankIdx, u1RankMax, u1ChannelBak, u1ByteIdx, ii;
    U8 DQS_DRVN_2, DQS_DRVP_2, DQS_ODTN_2, DQS_DRVN, DQS_DRVP, DQS_ODTN;
    U8 DQ_DRVN_2, DQ_DRVP_2, DQ_ODTN_2, DQ_DRVN, DQ_DRVP, DQ_ODTN;
    U8 CMD_DRVN_2, CMD_DRVP_2, CMD_ODTN_2, CMD_DRVN, CMD_DRVP, CMD_ODTN;

    u1ChannelBak= p->channel;
    vSetPHY2ChannelMapping(p, u1Channel);

    mcSHOW_DBG_MSG(("[IMPTrackingStatus] CH=%d\n", p->channel));

//    if (u1Channel == CHANNEL_A)
    {//460 464: ODTN DRVP, 468 46C: DRVN DRVP 

        //DQS
        DQS_DRVN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQDRV_STATUS), DQDRV_STATUS_DRVNDQS_2);
        DQS_DRVP_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS1), IMPCAL_STATUS1_DRVPDQS_SAVE2);
        DQS_ODTN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS1), IMPCAL_STATUS1_DRVNDQS_SAVE2);
        DQS_DRVN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQDRV_STATUS), DQDRV_STATUS_DRVNDQS_1);
        DQS_DRVP = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS1), IMPCAL_STATUS1_DRVPDQS_SAVE1);
        DQS_ODTN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS1), IMPCAL_STATUS1_DRVNDQS_SAVE1);

        //DQ
        DQ_DRVN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQDRV_STATUS), DQDRV_STATUS_DRVNDQ_2);
        DQ_DRVP_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS1), IMPCAL_STATUS1_DRVPDQ_SAVE2);
        DQ_ODTN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS1), IMPCAL_STATUS1_DRVNDQ_SAVE2);
        DQ_DRVN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CMDDRV_STATUS), CMDDRV_STATUS_DRVNDQ_1);
        DQ_DRVP = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS2), IMPCAL_STATUS2_DRVPDQ_SAVE1);
        DQ_ODTN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS2), IMPCAL_STATUS2_DRVNDQ_SAVE1);

        //CMD
        CMD_DRVN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CMDDRV_STATUS), CMDDRV_STATUS_DRVNCMD_2);
        CMD_DRVP_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS2), IMPCAL_STATUS2_DRVPCMD_SAVE2);
        CMD_ODTN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS2), IMPCAL_STATUS2_DRVNCMD_SAVE2);
        CMD_DRVN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CMDDRV_STATUS), CMDDRV_STATUS_DRVNCMD_1);
        CMD_DRVP = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS2), IMPCAL_STATUS2_DRVPCMD_SAVE1);
        CMD_ODTN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS2), IMPCAL_STATUS2_DRVNCMD_SAVE1);
    }
#if 0
    else
    {
        U8 shu_level;

        //Channel B is workaround
        shu_level = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHUSTATUS), SHUSTATUS_SHUFFLE_LEVEL);
        mcSHOW_DBG_MSG(("shu_level=%d\n", shu_level));

        //DQ
        DQ_DRVP_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING1 + shu_level * SHU_GRP_DRAMC_OFFSET), SHU_DRVING1_DQDRV2_DRVP);
        DQ_DRVP   = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING2 + shu_level * SHU_GRP_DRAMC_OFFSET), SHU_DRVING2_DQDRV1_DRVP);
        DQ_ODTN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING3 + shu_level * SHU_GRP_DRAMC_OFFSET), SHU_DRVING3_DQODT2_ODTN);
        DQ_ODTN   = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING4 + shu_level * SHU_GRP_DRAMC_OFFSET), SHU_DRVING4_DQODT1_ODTN);

        //DQS
        DQS_DRVP_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING1 + shu_level * SHU_GRP_DRAMC_OFFSET), SHU_DRVING1_DQSDRV2_DRVP);
        DQS_DRVP   = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING1 + shu_level * SHU_GRP_DRAMC_OFFSET), SHU_DRVING1_DQSDRV1_DRVP);
        DQS_ODTN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING3 + shu_level * SHU_GRP_DRAMC_OFFSET), SHU_DRVING3_DQSODT2_ODTN);
        DQS_ODTN   = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING3 + shu_level * SHU_GRP_DRAMC_OFFSET), SHU_DRVING3_DQSODT1_ODTN);

        //CMD
        CMD_DRVP_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING2 + shu_level * SHU_GRP_DRAMC_OFFSET), SHU_DRVING2_CMDDRV2_DRVP);
        CMD_DRVP   = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING2 + shu_level * SHU_GRP_DRAMC_OFFSET), SHU_DRVING2_CMDDRV1_DRVP);
        CMD_ODTN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING4 + shu_level * SHU_GRP_DRAMC_OFFSET), SHU_DRVING4_CMDODT2_ODTN);
        CMD_ODTN   = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING4 + shu_level * SHU_GRP_DRAMC_OFFSET), SHU_DRVING4_CMDODT1_ODTN);
    }
#endif

    mcSHOW_DBG_MSG(("\tDRVN_2\tDRVP_2\tODTN_2\tDRVN\tDRVP\tODTN\n"
                    "DQS\t%d\t%d\t%d\t%d\t%d\t%d\n"
                    "DQ\t%d\t%d\t%d\t%d\t%d\t%d\n"
                    "CMD\t%d\t%d\t%d\t%d\t%d\t%d\n",
                    DQS_DRVN_2, DQS_DRVP_2, DQS_ODTN_2, DQS_DRVN, DQS_DRVP, DQS_ODTN,
                    DQ_DRVN_2, DQ_DRVP_2, DQ_ODTN_2, DQ_DRVN, DQ_DRVP, DQ_ODTN,
                    CMD_DRVN_2, CMD_DRVP_2, CMD_ODTN_2, CMD_DRVN, CMD_DRVP, CMD_ODTN));

    p->channel = u1ChannelBak;
    vSetPHY2ChannelMapping(p, u1ChannelBak);
#endif
}

/* divRoundClosest() - to round up to the nearest integer
 * discard four, but treat five as whole (of decimal points)
 */
int divRoundClosest(const int n, const int d)
{
  return ((n < 0) ^ (d < 0)) ? ((n - d/2)/d) : ((n + d/2)/d);
}

#if (ENABLE_TX_TRACKING || TDQSCK_PRECALCULATION_FOR_DVFS)
void FreqJumpRatioCalculation(DRAMC_CTX_T *p)
{
    U32 shuffle_src_freq,shuffle_dst_index,jump_ratio_index;
    U16 u2JumpRatio[12] = {0}; /* Used to record "jump ratio" calulation results */
    U16 u2Freq=0;

    /* Calculate jump ratios and save to u2JumpRatio array */
    jump_ratio_index=0;

    if(p->frequency != 400)
    {
        shuffle_src_freq = p->frequency;
        for(shuffle_dst_index=DRAM_DFS_SHUFFLE_1; shuffle_dst_index<DRAM_DFS_SHUFFLE_MAX; shuffle_dst_index++)            
        {
            DRAM_DFS_FREQUENCY_TABLE_T *pDstFreqTbl = get_FreqTbl_by_shuffleIndex(p,shuffle_dst_index);
            if(pDstFreqTbl==NULL)
            {
                mcSHOW_ERR_MSG(("NULL pFreqTbl\n"));
                while(1);
            }    
            if (pDstFreqTbl->freq_sel == LP4_DDR800)
            {
                u2JumpRatio[jump_ratio_index] = 0;
            }
            else
            {
                u2Freq = GetFreqBySel(p, pDstFreqTbl->freq_sel);
                u2JumpRatio[jump_ratio_index] = divRoundClosest(u2Freq*32, shuffle_src_freq);
				//u2JumpRatio[jump_ratio_index] = (pDstFreqTbl->frequency/shuffle_src_freq)*32;
                //mcSHOW_DBG_MSG3(("shuffle_%d=DDR%d / shuffle_%d=DDR%d \n", shuffle_dst_index, pFreqTbl->frequency<<1,
                //                                                            shuffle_src_index, get_FreqTbl_by_shuffleIndex(p,shuffle_src_index)->frequency<<1));
                //mcSHOW_DBG_MSG3(("Jump_RATIO_%d : 0x%x\n", jump_ratio_index, u2JumpRatio[jump_ratio_index],
                //                                            get_FreqTbl_by_shuffleIndex(p,shuffle_src_index)->frequency));
            }
            mcSHOW_DBG_MSG3(("Jump_RATIO [%d]: %x\tFreq %d -> %d\tDDR%d -> DDR%d\n",jump_ratio_index,u2JumpRatio[jump_ratio_index],get_shuffleIndex_by_Freq(p),shuffle_dst_index,shuffle_src_freq<<1,u2Freq<<1));
            jump_ratio_index++;
        }
    }

    /* Save jumpRatios into corresponding register fields */
    vIO32WriteFldMulti_All(DRAMC_REG_SHU_PRE_TDQSCK1, P_Fld(u2JumpRatio[0], SHU_PRE_TDQSCK1_TDQSCK_JUMP_RATIO0)
                                                            | P_Fld(u2JumpRatio[1], SHU_PRE_TDQSCK1_TDQSCK_JUMP_RATIO1)
                                                            | P_Fld(u2JumpRatio[2], SHU_PRE_TDQSCK1_TDQSCK_JUMP_RATIO2)
                                                            | P_Fld(u2JumpRatio[3], SHU_PRE_TDQSCK1_TDQSCK_JUMP_RATIO3));
    vIO32WriteFldMulti_All(DRAMC_REG_SHU_PRE_TDQSCK2, P_Fld(u2JumpRatio[4], SHU_PRE_TDQSCK2_TDQSCK_JUMP_RATIO4)
                                                            | P_Fld(u2JumpRatio[5], SHU_PRE_TDQSCK2_TDQSCK_JUMP_RATIO5)
                                                            | P_Fld(u2JumpRatio[6], SHU_PRE_TDQSCK2_TDQSCK_JUMP_RATIO6)
                                                            | P_Fld(u2JumpRatio[7], SHU_PRE_TDQSCK2_TDQSCK_JUMP_RATIO7));
    vIO32WriteFldMulti_All(DRAMC_REG_SHU_DVFSCTL, P_Fld(u2JumpRatio[8], SHU_DVFSCTL_TDQSCK_JUMP_RATIO8)
                                                            | P_Fld(u2JumpRatio[9], SHU_DVFSCTL_TDQSCK_JUMP_RATIO9));
    return;
}
#endif

#if TDQSCK_PRECALCULATION_FOR_DVFS
void DramcDQSPrecalculation_preset(DRAMC_CTX_T *p)//Test tDQSCK_temp Pre-calculation
{
    U8 u1ByteIdx, u1RankNum, u1RankBackup=p->rank;
    U8 u1ShuLevel = vGet_Current_ShuLevel(p);
	U8 u1UI_value, u1PI_value, u1MCK_value;
    U16 u2Byte_offset;
    U32 u1Delay_Addr[2]={0}, u1Delay_Fld[2];
    REG_FLD_DQS_PRE_K TransferReg = {0, 0, {0}};

    mcSHOW_DBG_MSG(("Pre-setting of DQS Precalculation\n"));

    if((u1ShuLevel >= SRAM_SHU4) && (u1ShuLevel <= SRAM_SHU7))
    { //SHU4, 5, 6, 7
        u1Delay_Addr[0] = ((u1ShuLevel/6)*0x4) + 0x30; //Offset of phase0 UI register 
        u1Delay_Addr[1] = 0x38; //Offset of phase1 UI register
        u2Byte_offset = 0xc;
    }
    else if(u1ShuLevel >= SRAM_SHU8)
    {  //SHU8, 9
        u1Delay_Addr[0] = 0x260; //Offset of phase0 UI register 
        u1Delay_Addr[1] = 0x268; //Offset of phase1 UI register
        u2Byte_offset = 0x4;
    }
    else //SHU0, 1, 2, 3
    {
        u1Delay_Addr[0] = ((u1ShuLevel/2)*0x4); //Offset of phase0 UI register 
        u1Delay_Addr[1] = 0x8; //Offset of phase1 UI register
        u2Byte_offset = 0xc;
    }

    u1Delay_Fld[0] = u1ShuLevel%2; //Field of phase0 PI and UI
    u1Delay_Fld[1] = u1ShuLevel%4; //Field of phase1 UI

    switch (u1Delay_Fld[0]) //Phase0 UI and PI
    {
        case 0: 
            TransferReg.u4UI_Fld = RK0_PRE_TDQSCK1_TDQSCK_UIFREQ1_B0R0;
            TransferReg.u4PI_Fld = RK0_PRE_TDQSCK1_TDQSCK_PIFREQ1_B0R0;
            break;
        case 1:
            TransferReg.u4UI_Fld = RK0_PRE_TDQSCK1_TDQSCK_UIFREQ2_B0R0;
            TransferReg.u4PI_Fld = RK0_PRE_TDQSCK1_TDQSCK_PIFREQ2_B0R0;
            break;
        default:
            break;
    }    
	
    if(u1ShuLevel == SRAM_SHU8)
    {
        TransferReg.u4UI_Fld_P1[0] = RK0_PRE_TDQSCK27_TDQSCK_UIFREQ9_P1_B0R0; //Byte0
        TransferReg.u4UI_Fld_P1[1] = RK0_PRE_TDQSCK27_TDQSCK_UIFREQ9_P1_B1R0; //Byte1
    }
    else if(u1ShuLevel == SRAM_SHU9)
    {
        TransferReg.u4UI_Fld_P1[0] = RK0_PRE_TDQSCK27_TDQSCK_UIFREQ10_P1_B0R0; //Byte0
        TransferReg.u4UI_Fld_P1[1] = RK0_PRE_TDQSCK27_TDQSCK_UIFREQ10_P1_B1R0; //Byte1
    }    
    else //(u1ShuLevel < SRAM_SHU8)
    {
        switch (u1Delay_Fld[1]) //Phase1 UI 
        {
            case 0: 
                TransferReg.u4UI_Fld_P1[0] = RK0_PRE_TDQSCK3_TDQSCK_UIFREQ1_P1_B0R0;
                break;
            case 1:
                TransferReg.u4UI_Fld_P1[0] = RK0_PRE_TDQSCK3_TDQSCK_UIFREQ2_P1_B0R0;
                break;
            case 2:
                TransferReg.u4UI_Fld_P1[0] = RK0_PRE_TDQSCK3_TDQSCK_UIFREQ3_P1_B0R0;
                break;
            case 3:
                TransferReg.u4UI_Fld_P1[0] = RK0_PRE_TDQSCK3_TDQSCK_UIFREQ4_P1_B0R0;
                break;
            default:
                break;
        }
    }
	
    for(u1ByteIdx =0 ; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
    {
        for(u1RankNum=0; u1RankNum<p->support_rank_num; u1RankNum++)
        {
            vSetRank(p,u1RankNum);
			
            if(u1ByteIdx == 0)
            {
                u1MCK_value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED);
                u1UI_value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED);
                u1PI_value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQSIEN), SHURK0_DQSIEN_R0DQS0IEN);   
            }
            else //Byte1
            {
                u1MCK_value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED);
                u1UI_value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED);
                u1PI_value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQSIEN), SHURK0_DQSIEN_R0DQS1IEN);   
            }

            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK1 + u1Delay_Addr[0] + (u1ByteIdx*u2Byte_offset)), (u1MCK_value << 3) | u1UI_value, TransferReg.u4UI_Fld);//UI
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK1 + u1Delay_Addr[0] + (u1ByteIdx*u2Byte_offset)), u1PI_value, TransferReg.u4PI_Fld); //PI

            if(u1ByteIdx == 0)
            {
                u1MCK_value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1);
                u1UI_value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1);
            }
            else //Byte1
            {
                u1MCK_value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1);
                u1UI_value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1);
            }

            if((u1ShuLevel == SRAM_SHU8) || (u1ShuLevel == SRAM_SHU9))
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK1 + u1Delay_Addr[1]), (u1MCK_value << 3) | u1UI_value, TransferReg.u4UI_Fld_P1[u1ByteIdx]); //phase1 UI
            else
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK1 + u1Delay_Addr[1] + (u1ByteIdx*u2Byte_offset)), (u1MCK_value << 3) | u1UI_value, TransferReg.u4UI_Fld_P1[0]); //phase1 UI
        }
    }
    vSetRank(p, u1RankBackup);

    return;
}

void DramcDQSPrecalculation_enable(DRAMC_CTX_T *p)
{
    //DQS pre-K new mode
    vIO32WriteFldAlign_All(DRAMC_REG_RK0_PRE_TDQSCK15, 0x1, RK0_PRE_TDQSCK15_SHUFFLE_LEVEL_MODE_SELECT);
    //Enable pre-K HW
    vIO32WriteFldAlign_All(DRAMC_REG_PRE_TDQSCK1, 0x1, PRE_TDQSCK1_TDQSCK_PRECAL_HW);
    //Select HW flow
    vIO32WriteFldAlign_All(DRAMC_REG_PRE_TDQSCK1, 0x1, PRE_TDQSCK1_TDQSCK_REG_DVFS);
    //Set Auto save to RG
    vIO32WriteFldAlign_All(DRAMC_REG_PRE_TDQSCK1, 0x1, PRE_TDQSCK1_TDQSCK_HW_SW_UP_SEL);
}
#endif

#if RDSEL_TRACKING_EN
void RDSELRunTimeTracking_preset(DRAMC_CTX_T *p)
{
	S32 s4PosVH;

    s4PosVH = divRoundClosest(400, ((1000000/p->frequency)/64));
	
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_RDSEL_TRACK), P_Fld(s4PosVH, SHU_RDSEL_TRACK_RG_SHU_GW_THRD_POS)
                                                    | P_Fld(-s4PosVH, SHU_RDSEL_TRACK_RG_SHU_GW_THRD_NEG));
}
#endif

void DramcHWGatingInit(DRAMC_CTX_T *p)
{
#ifdef HW_GATING
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), P_Fld(0, STBCAL_STBCALEN) | \
                                            P_Fld(0, STBCAL_STBCAL2R)| P_Fld(0, STBCAL_STB_SELPHYCALEN) | \
                                            P_Fld(0, STBCAL_STBSTATE_OPT) | \
                                            P_Fld(0, STBCAL_RKCHGMASKDIS)| P_Fld(0, STBCAL_REFUICHG) | \
                                            P_Fld(1, STBCAL_PICGEN));

    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), P_Fld(1, STBCAL_DQSIENCG_CHG_EN));
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), 0, STBCAL_CG_RKEN);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), 1, STBCAL_DQSIENCG_NORMAL_EN);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 1, MISC_CTRL1_R_DMDQSIENCG_EN);

    DramcHWDQSGatingTracking_DVT(p);
#endif
}


void DramcHWGatingOnOff(DRAMC_CTX_T *p, U8 u1OnOff)
{
    #ifdef HW_GATING
    if (u1OnOff)
    	u1OnOff = (U8) (doe_get_config("dram_hw_gating"))? 0: 1;
    
    vIO32WriteFldMulti_All(DRAMC_REG_SHUCTRL2, P_Fld(u1OnOff, SHUCTRL2_R_DVFS_OPTION)
                | P_Fld(u1OnOff, SHUCTRL2_R_DVFS_PARK_N));
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL2), u1OnOff, STBCAL2_STB_GERRSTOP);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), u1OnOff, STBCAL_STBCALEN);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), u1OnOff, STBCAL_STB_SELPHYCALEN);
    #else
    vIO32WriteFldMulti_All(DRAMC_REG_SHUCTRL2, P_Fld(0x0, SHUCTRL2_R_DVFS_OPTION)
                    | P_Fld(0x0, SHUCTRL2_R_DVFS_PARK_N));
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL2), 0, STBCAL2_STB_GERRSTOP);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), 0, STBCAL_STBCALEN);   // PI tracking off = HW gating tracking off
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), 0, STBCAL_STB_SELPHYCALEN);
    #endif
}

void DramcHWGatingDebugOnOff(DRAMC_CTX_T *p, U8 u1OnOff)
{
    #ifdef HW_GATING
    // STBCAL2_STB_DBG_EN = 0x3, byte0/1 enable
    U8 u1EnB0B1 = (u1OnOff == ENABLE) ? 0x3 : 0x0;
    vIO32WriteFldMulti_All(DRAMC_REG_STBCAL2, P_Fld(u1EnB0B1, STBCAL2_STB_DBG_EN) | 
                                              P_Fld(u1OnOff, STBCAL2_STB_PIDLYCG_IG) |
                                              P_Fld(u1OnOff, STBCAL2_STB_UIDLYCG_IG) |
                                              P_Fld(u1OnOff, STBCAL2_STB_GERRSTOP) | 
                                              P_Fld(0, STBCAL2_STB_DBG_CG_AO) |
                                              P_Fld(0, STBCAL2_STB_DBG_UIPI_UPD_OPT));
    #endif
}

#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
#if (__ETT__ || CPU_RW_TEST_AFTER_K)
#if 0 // Please use memeset to initail value, due to different CHANNEL_NUM 
U16 u2MaxGatingPos[CHANNEL_NUM][RANK_MAX][DQS_NUMBER]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
U16 u2MinGatingPos[CHANNEL_NUM][RANK_MAX][DQS_NUMBER]={0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
#endif
void DramcPrintHWGatingStatus(DRAMC_CTX_T *p, U8 u1Channel)
{
#ifdef HW_GATING
    U8 u1RankIdx, u1RankMax, u1ChannelBak, u1ByteIdx, ii;
    U32 u4ResultDQS_PI, u4ResultDQS_UI, u4ResultDQS_UI_P1;
    U8 u1Dqs_pi[DQS_BIT_NUMBER], u1Dqs_ui[DQS_BIT_NUMBER],u1Dqs_ui_P1[DQS_BIT_NUMBER];
    U16 u2TmpValue, u2TmpUI[DQS_NUMBER], u2TmpPI[DQS_NUMBER];
    U32 MANUDLLFRZ_bak, STBSTATE_OPT_bak;
    U32 backup_rank;

    u1ChannelBak= p->channel;
    vSetPHY2ChannelMapping(p, u1Channel);
    backup_rank = u1GetRank(p);

    if (p->support_rank_num==RANK_DUAL)
        u1RankMax = RANK_MAX;
    else
         u1RankMax =RANK_1;

    MANUDLLFRZ_bak = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), TEST2_3_MANUDLLFRZ);
    STBSTATE_OPT_bak = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), STBCAL_STBSTATE_OPT);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), 1, TEST2_3_MANUDLLFRZ);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), 1, STBCAL_STBSTATE_OPT);

    for(u1RankIdx=0; u1RankIdx<u1RankMax; u1RankIdx++)
    {
        vSetRank(p, u1RankIdx);
        mcSHOW_DBG_MSG(("[DramcHWGatingStatus] Channel=%d, Rank=%d\n", p->channel, u1RankIdx));
        u4ResultDQS_PI = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_RK0_DQSIENDLY));
        u1Dqs_pi[0]  = u4ResultDQS_PI & 0xff;
        u1Dqs_pi[1]  = (u4ResultDQS_PI >>8) & 0xff;
        u1Dqs_pi[2]  = (u4ResultDQS_PI >>16) & 0xff;
        u1Dqs_pi[3]  = (u4ResultDQS_PI >>24) & 0xff;
        //mcSHOW_DBG_MSG(("[DramcHWGatingStatus] PI status (DQS0, DQS1, DQS2, DQS3) =(%d, %d, %d, %d)\n", u1Dqs_pi[0], u1Dqs_pi[1], u1Dqs_pi[2], u1Dqs_pi[3]));

        u4ResultDQS_UI = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_RK0_DQSIENUIDLY));
        u1Dqs_ui[0]  = u4ResultDQS_UI & 0xff;
        u1Dqs_ui[1]  = (u4ResultDQS_UI >>8) & 0xff;
        u1Dqs_ui[2]  = (u4ResultDQS_UI >>16) & 0xff;
        u1Dqs_ui[3]  = (u4ResultDQS_UI >>24) & 0xff;
        //mcSHOW_DBG_MSG(("[DramcHWGatingStatus] UI status (DQS0, DQS1, DQS2, DQS3) =(%d, %d, %d, %d)\n", u1Dqs_ui[0], u1Dqs_ui[1], u1Dqs_ui[2], u1Dqs_ui[3]));
        mcSHOW_DBG_MSG(("Byte0(2T, 0.5T, PI) =(%d, %d, %d)\n Byte1(2T, 0.5T, PI) =(%d, %d, %d)\n Byte2(2T, 0.5T, PI) =(%d, %d, %d)\n Byte3(2T, 0.5T, PI) =(%d, %d, %d)\n", \
                                        u1Dqs_ui[0]/8, u1Dqs_ui[0]%8, u1Dqs_pi[0], u1Dqs_ui[1]/8, u1Dqs_ui[1]%8, u1Dqs_pi[1], \
                                        u1Dqs_ui[2]/8, u1Dqs_ui[2]%8, u1Dqs_pi[2], u1Dqs_ui[3]/8, u1Dqs_ui[3]%8, u1Dqs_pi[3]));

        u4ResultDQS_UI_P1 = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_RK0_DQSIENUIDLY_P1));
        u1Dqs_ui_P1[0]  = u4ResultDQS_UI_P1 & 0xff;
        u1Dqs_ui_P1[1]  = (u4ResultDQS_UI_P1 >>8) & 0xff;
        u1Dqs_ui_P1[2]  = (u4ResultDQS_UI_P1 >>16) & 0xff;
        u1Dqs_ui_P1[3]  = (u4ResultDQS_UI_P1 >>24) & 0xff;
        mcSHOW_DBG_MSG(("UI_Phase1 (DQS0~3) =(%d, %d, %d, %d)\n\n", u1Dqs_ui_P1[0], u1Dqs_ui_P1[1], u1Dqs_ui_P1[2], u1Dqs_ui_P1[3]));
    }
    vSetRank(p, backup_rank);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), STBSTATE_OPT_bak, STBCAL_STBSTATE_OPT);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), MANUDLLFRZ_bak, TEST2_3_MANUDLLFRZ);

    p->channel = u1ChannelBak;
    vSetPHY2ChannelMapping(p, u1ChannelBak);
#endif
}

void DramcHWGatingTrackingRecord(DRAMC_CTX_T *p, U8 u1Channel)
{
#ifdef HW_GATING
    U8 u1RankIdx, u1RankMax, u1Info_NUM, u1Info_Max_MUM=16;
    U8 u1ChannelBak, u1RankBak;
    
    U8 u1DBG_Dqs0_DFS, u1DBG_Dqs0_Lead, u1DBG_Dqs0_Lag, u1DBG_Dqs0_UI, u1DBG_Dqs0_PI;
    U8 u1DBG_Dqs1_DFS, u1DBG_Dqs1_Lead, u1DBG_Dqs1_Lag, u1DBG_Dqs1_UI, u1DBG_Dqs1_PI;
    U16 u4DBG_Dqs0_Info, u4DBG_Dqs1_Info;
    U32 u4DBG_Dqs01_Info;

    U32 u4Dqs0_MAX_MIN_DLY, u4Dqs1_MAX_MIN_DLY;
    U16 u2Dqs0_UI_MAX_DLY, u2Dqs0_PI_MAX_DLY, u2Dqs0_UI_MIN_DLY, u2Dqs0_PI_MIN_DLY;
    U16 u2Dqs1_UI_MAX_DLY, u2Dqs1_PI_MAX_DLY, u2Dqs1_UI_MIN_DLY, u2Dqs1_PI_MIN_DLY;
    U8 u1ShuffleLevel;
    
    u1ChannelBak= p->channel;
    vSetPHY2ChannelMapping(p, u1Channel);
    u1RankBak = u1GetRank(p);

    if (p->support_rank_num==RANK_DUAL)
        u1RankMax = RANK_MAX;
    else
         u1RankMax =RANK_1;
    
    //Run Time HW Gating Debug Information
    //for(u1RankIdx=0; u1RankIdx<u1RankMax; u1RankIdx++)
    for(u1RankIdx=0; u1RankIdx<u1RankMax; u1RankIdx++)    
    {
        vSetRank(p, u1RankIdx);
        
        u1ShuffleLevel = u4IO32ReadFldAlign(DRAMC_REG_SHUSTATUS, SHUSTATUS_SHUFFLE_LEVEL);

        mcSHOW_DBG_MSG3(("\n[HWGatingTrackingRecord] Channel=%d, Rank=%d, SHU_LEVEL=%d\n", p->channel, u1RankIdx, u1ShuffleLevel));
      
        mcSHOW_DBG_MSG3(("Run Time HW Gating Debug Information :\n"));
        mcSHOW_DBG_MSG3(("        B0=(DFS,Lead,Lag,4T, UI, PI), B1=(DFS,Lead,Lag,4T, UI, PI)\n"));
        
        for(u1Info_NUM=0; u1Info_NUM < u1Info_Max_MUM; u1Info_NUM++)
        {
          //DFS_ST(Shuffle Level): bit[15:14]
          //Shift_R(Lead): bit[13]
          //Shift_L(Lag) : bit[12]
          //UI_DLY : bit[11:06]
          //PI_DLY : bit[05:00]
          u4DBG_Dqs01_Info= u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_RK0_B01_STB_DBG_INFO_00 + 0x4*u1Info_NUM));
          u4DBG_Dqs0_Info = (u4DBG_Dqs01_Info >>  0)& 0xffff;
          u4DBG_Dqs1_Info = (u4DBG_Dqs01_Info >> 16)& 0xffff;
          
          u1DBG_Dqs0_DFS  = (u4DBG_Dqs0_Info >> 14) & 0x03;
          u1DBG_Dqs0_Lead = (u4DBG_Dqs0_Info >> 13) & 0x01;
          u1DBG_Dqs0_Lag  = (u4DBG_Dqs0_Info >> 12) & 0x01;
          u1DBG_Dqs0_UI   = (u4DBG_Dqs0_Info >>  6) & 0x3f; 
          u1DBG_Dqs0_PI   = (u4DBG_Dqs0_Info >>  0) & 0x3f;

          u1DBG_Dqs1_DFS  = (u4DBG_Dqs1_Info >> 14) & 0x03;
          u1DBG_Dqs1_Lead = (u4DBG_Dqs1_Info >> 13) & 0x01;
          u1DBG_Dqs1_Lag  = (u4DBG_Dqs1_Info >> 12) & 0x01;
          u1DBG_Dqs1_UI   = (u4DBG_Dqs1_Info >>  6) & 0x3f; 
          u1DBG_Dqs1_PI   = (u4DBG_Dqs1_Info >>  0) & 0x3f;
          
          if (u1Info_NUM<10)
          {
            mcSHOW_DBG_MSG3(("Info= %d ", u1Info_NUM));
          }
          else
          {
            mcSHOW_DBG_MSG3(("Info=%d ", u1Info_NUM));
          }
          
          mcSHOW_DBG_MSG3(("B0=(  %d,  %d,  %d,  %d,  %d, %d), B1=(  %d,  %d,  %d,  %d,  %d,  %d)\n",
          u1DBG_Dqs0_DFS, u1DBG_Dqs0_Lead, u1DBG_Dqs0_Lag, u1DBG_Dqs0_UI/8, u1DBG_Dqs0_UI%8, u1DBG_Dqs0_PI,
          u1DBG_Dqs1_DFS, u1DBG_Dqs1_Lead, u1DBG_Dqs1_Lag, u1DBG_Dqs1_UI/8, u1DBG_Dqs1_UI%8, u1DBG_Dqs1_PI));
        }

        //Run Time HW Gating Max and Min Value Record
        //Run Time HW Gating MAX_DLY UI : bit[27:22]
        //Run Time HW Gating MAX_DLY PI : bit[21:16]
        //Run Time HW Gating MIN_DLY UI : bit[11:06]
        //Run Time HW Gating MIN_DLY PI : bit[05:00]
        u4Dqs0_MAX_MIN_DLY = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_RK0_B0_STB_MAX_MIN_DLY));
        u2Dqs0_UI_MAX_DLY  = (u4Dqs0_MAX_MIN_DLY >> 22) & 0x3f;
        u2Dqs0_PI_MAX_DLY  = (u4Dqs0_MAX_MIN_DLY >> 16) & 0x3f;
        u2Dqs0_UI_MIN_DLY  = (u4Dqs0_MAX_MIN_DLY >>  6) & 0x3f;
        u2Dqs0_PI_MIN_DLY  = (u4Dqs0_MAX_MIN_DLY >>  0) & 0x3f;

        u4Dqs1_MAX_MIN_DLY = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_RK0_B1_STB_MAX_MIN_DLY));
        u2Dqs1_UI_MAX_DLY  = (u4Dqs1_MAX_MIN_DLY >> 22) & 0x3f;
        u2Dqs1_PI_MAX_DLY  = (u4Dqs1_MAX_MIN_DLY >> 16) & 0x3f;
        u2Dqs1_UI_MIN_DLY  = (u4Dqs1_MAX_MIN_DLY >>  6) & 0x3f;
        u2Dqs1_PI_MIN_DLY  = (u4Dqs1_MAX_MIN_DLY >>  0) & 0x3f;
        
        mcSHOW_DBG_MSG3(("B0 = MAX(4T, UI, PI) MIN(4T, UI, PI), B1 = MAX(4T, UI, PI) MIN(4T, UI, PI)\n"));
        mcSHOW_DBG_MSG3(("B0 = MAX( %d,  %d, %d) MIN( %d,  %d, %d),  B1 = MAX( %d,  %d, %d) MIN( %d,  %d, %d)\n", 
                        u2Dqs0_UI_MAX_DLY/8, u2Dqs0_UI_MAX_DLY%8, u2Dqs0_PI_MAX_DLY, 
                        u2Dqs0_UI_MIN_DLY/8, u2Dqs0_UI_MIN_DLY%8, u2Dqs0_PI_MIN_DLY,
                        u2Dqs1_UI_MAX_DLY/8, u2Dqs1_UI_MAX_DLY%8, u2Dqs1_PI_MAX_DLY, 
                        u2Dqs1_UI_MIN_DLY/8, u2Dqs1_UI_MIN_DLY%8, u2Dqs1_PI_MIN_DLY));
    }
    vSetRank(p, u1RankBak);
    p->channel = u1ChannelBak;
    vSetPHY2ChannelMapping(p, u1ChannelBak);
#endif
}

void DramcPrintRXFIFODebugStatus(DRAMC_CTX_T *p)
{
#if RX_PICG_NEW_MODE
    //RX FIFO debug feature, MP setting should enable debug function for Gating error information
    //APHY control new mode
    U32 u1ChannelBak,u4value;
    U8 u1ChannelIdx;
    
    u1ChannelBak= p->channel;

    for (u1ChannelIdx=CHANNEL_A; u1ChannelIdx<p->support_channel_num; u1ChannelIdx++)
    {
        p->channel = u1ChannelIdx;
        
        u4value= u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_STBERR_RK0_R)) & (0xf<<24); //DDRPHY NAO bit24~27 
        if (u4value)
        {
            mcSHOW_DBG_MSG(("\n[RXFIFODebugStatus] CH_%d MISC_STBERR_RK0_R_RX_ARDQ = 0x\033[1;36m%x\033[m for Gating error information\n",u1ChannelIdx,u4value));
        }
    }
    p->channel = u1ChannelBak;
    vSetPHY2ChannelMapping(p, u1ChannelBak);
#endif
}
#endif  //#if __ETT__
#endif


