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

#if (FOR_DV_SIMULATION_USED==0)
#include "pll.h"
#endif


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------

#if (FOR_DV_SIMULATION_USED==1)
extern void broadcast_on(void);
extern void broadcast_off(void);
#endif


void DramcBroadcastOnOff(U32 bOnOff)
{
#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
    U8 u1BroadcastStatus = 0;

    // INFRA_RSVD3[9:8] = protect_set_clr_mask
    u1BroadcastStatus = (*((volatile unsigned int *)(INFRA_RSVD3)) >> 8) & 0x3;
    if (u1BroadcastStatus & 0x1) // Enable new infra access by Preloader
    {
        if (bOnOff == DRAMC_BROADCAST_ON)
            *((volatile unsigned int *)(DRAMC_WBR_SET)) = DRAMC_BROADCAST_SET;
        else
            *((volatile unsigned int *)(DRAMC_WBR_CLR)) = DRAMC_BROADCAST_CLR;
    }
    else
        *((volatile unsigned int *)(DRAMC_WBR)) = bOnOff;
#endif
#if (FOR_DV_SIMULATION_USED==1)
    if(gu1BroadcastIsLP4 == TRUE)
    {
        #if APOLLO_SPECIFIC
        broadcast_off();mcSHOW_DBG_MSG(("Broadcast OFF\n"));
        u1BroadcastOnOff = 0;
        #else
        if(bOnOff)
        {
            broadcast_on();mcSHOW_DBG_MSG(("Broadcast ON\n"));
            u1BroadcastOnOff = 1;
        }
        else
        {
            broadcast_off();mcSHOW_DBG_MSG(("Broadcast OFF\n"));
            u1BroadcastOnOff = 0;
        }
        #endif
    }
    else //LP3
    {
        {
            broadcast_off();mcSHOW_DBG_MSG(("Broadcast OFF\n"));
            u1BroadcastOnOff = 0;
        }
    }
#endif

#ifdef DUMP_INIT_RG_LOG_TO_DE
    if(gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag)    
    {
        U8 u1BroadcastStatus = 0;
        U32 addr, val;

        // INFRA_RSVD3[9:8] = protect_set_clr_mask
        u1BroadcastStatus = (*((volatile unsigned int *)(INFRA_RSVD3)) >> 8) & 0x3;
        if (u1BroadcastStatus & 0x1) // Enable new infra access by Preloader
        {
            if (bOnOff == DRAMC_BROADCAST_ON)
            {
                addr = DRAMC_WBR_SET;
                val = DRAMC_BROADCAST_SET;
                // *((volatile unsigned int *)(DRAMC_WBR_SET)) = DRAMC_BROADCAST_SET;
            }
            else
            {
                addr = DRAMC_WBR_CLR;
                val = DRAMC_BROADCAST_CLR;
                // *((volatile unsigned int *)(DRAMC_WBR_CLR)) = DRAMC_BROADCAST_CLR;
            }
        }
        else
        {
                addr = DRAMC_WBR;
                val = bOnOff;
                // *((volatile unsigned int *)(DRAMC_WBR)) = bOnOff;
        }
    
        mcSHOW_DUMP_INIT_RG_MSG(("*((UINT32P)(0x%x)) = 0x%x;\n",addr, val));
//        mcDELAY_MS(1);    
#if (FOR_DV_SIMULATION_USED==0)
        GPT_Delay_ms(1);
#endif
    }    
#endif

}

U32 GetDramcBroadcast(void)
{
#if (FOR_DV_SIMULATION_USED==0)
    return *((volatile unsigned int *)(DRAMC_WBR));
#endif
#if (FOR_DV_SIMULATION_USED==1)
    return u1BroadcastOnOff;
#endif
}


#if __ETT__
const U32 u4Cannot_Use_Dramc_WBR_Reg[]=
{
    DDRPHY_SHU_CA_CMD6,
    DDRPHY_SHU_CA_DLL0,
    DDRPHY_SHU_CA_DLL1,
    DDRPHY_CA_DLL_ARPI3,
    DDRPHY_MISC_SHU_OPT,
    DDRPHY_CKMUX_SEL,
    DRAMC_REG_RK0_PRE_TDQSCK1,
    DRAMC_REG_RK0_PRE_TDQSCK2,
    DRAMC_REG_RK0_PRE_TDQSCK3,
    DRAMC_REG_RK0_PRE_TDQSCK4,
    DRAMC_REG_RK0_PRE_TDQSCK5,
    DRAMC_REG_RK0_PRE_TDQSCK6,
    DRAMC_REG_RK0_PRE_TDQSCK13,
    DRAMC_REG_RK0_PRE_TDQSCK14,
    DRAMC_REG_RK0_PRE_TDQSCK15,
    DRAMC_REG_RK0_PRE_TDQSCK16,
    DRAMC_REG_RK0_PRE_TDQSCK17,
    DRAMC_REG_RK0_PRE_TDQSCK18,
    DRAMC_REG_RK1_PRE_TDQSCK1,
    DRAMC_REG_RK1_PRE_TDQSCK2,
    DRAMC_REG_RK1_PRE_TDQSCK3,
    DRAMC_REG_RK1_PRE_TDQSCK4,
    DRAMC_REG_RK1_PRE_TDQSCK5,
    DRAMC_REG_RK1_PRE_TDQSCK6,
    DRAMC_REG_RK1_PRE_TDQSCK13,
    DRAMC_REG_RK1_PRE_TDQSCK14,
    DRAMC_REG_RK1_PRE_TDQSCK15,
    DRAMC_REG_RK1_PRE_TDQSCK16,
    DRAMC_REG_RK1_PRE_TDQSCK17,
    DRAMC_REG_RK1_PRE_TDQSCK18,
    DRAMC_REG_RK0_PRE_TDQSCK25,
    DRAMC_REG_RK0_PRE_TDQSCK26,
    DRAMC_REG_RK0_PRE_TDQSCK27,
    DRAMC_REG_RK1_PRE_TDQSCK25,
    DRAMC_REG_RK1_PRE_TDQSCK26,
    DRAMC_REG_RK1_PRE_TDQSCK27
};
#define CANNOT_USE_WBR_SIZE ((sizeof(u4Cannot_Use_Dramc_WBR_Reg))/(sizeof(U32)))
void CheckDramcWBR(U32 u4address)
{
	U32 i, channel_and_value;
    if (GetDramcBroadcast()==DRAMC_BROADCAST_ON)
    {
    #if ((CHANNEL_NUM == 1) || (CHANNEL_NUM == 2))
        channel_and_value = 0x1;
    #else //for channel number = 3 or 4
        channel_and_value = 0x3;
    #endif
    if ((((u4address - Channel_A_DRAMC_NAO_BASE_VIRTUAL) >> POS_BANK_NUM) & channel_and_value) != CHANNEL_A)
    {
        mcSHOW_ERR_MSG(("Error! virtual address 0x%x is not CHA and cannot use Dramc WBR\n", u4address));
        while(1);
    }
        for (i=0; i<CANNOT_USE_WBR_SIZE; i++)
        {
            if (u4Cannot_Use_Dramc_WBR_Reg[i]==u4address)
            {
                mcSHOW_ERR_MSG(("Error! virtual address 0x%x cannot use Dramc WBR\n", u4address));
                while(1);
            }
         }
     }
}
#endif


U8 u1IsLP4Family(DRAM_DRAM_TYPE_T dram_type)
{
    if (dram_type == TYPE_LPDDR4 || dram_type == TYPE_LPDDR4X || dram_type == TYPE_LPDDR4P)
        return 1;
    else
        return 0;
}

void vSetChannelNumber(DRAMC_CTX_T *p)
{
    #if (!FOR_DV_SIMULATION_USED)
    p->support_channel_num = CHANNEL_NUM; // 4 channel
    #else
    p->support_channel_num = CHANNEL_SINGLE;
    #endif
}

void vSetRankNumber(DRAMC_CTX_T *p)
{
#if(FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
    if (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RSTMASK), RSTMASK_RSV_DRAM_SUPPORT_RANK_NUM) == 0)
    {
        p->support_rank_num =RANK_DUAL;
    }
    else
    {
        p->support_rank_num =RANK_SINGLE;
    }
#endif
}

void vSetPHY2ChannelMapping(DRAMC_CTX_T *p, U8 u1Channel)
{
    p->channel =u1Channel;
}

U8 vGetPHY2ChannelMapping(DRAMC_CTX_T *p)
{
    return p->channel;
}

void vSetRank(DRAMC_CTX_T *p, U8 ucRank)
{
    p->rank = ucRank;
}

U8 u1GetRank(DRAMC_CTX_T *p)
{
    return p->rank;
}

/* To support frequencies not on the ACTimingTable, we use read/write latency
 * settings as the next highest freq listed in ACTimingTable. Use this API
 * to set which freq group the target freq belongs to.
 * (Set's DRAMC_CTX_T's p->freqGroup)
 * Currently is designed to be called from DDRPhyFreqSel()
 */
void setFreqGroup(DRAMC_CTX_T *p)
{

    /* Below listed conditions represent freqs that exist in ACTimingTable
     * -> Should cover freqGroup settings for all real freq values
     */
     if (p->frequency <= 400) // DDR800
    {
        p->freqGroup = 400;
    }
    else if (p->frequency <= 600) // DDR1200
    {
        p->freqGroup = 600;
    }
    else if (p->frequency <= 800) // DDR1600
    {
        p->freqGroup = 800;
    }
    else if (p->frequency <= 1200) //DDR2400, DDR2280
    {
        p->freqGroup = 1200;
    }
    else if (p->frequency <= 1333) // DDR2667
    {
        p->freqGroup = 1333;
    }
    else if (p->frequency <= 1600) // DDR3200
    {
        p->freqGroup = 1600;
    }
    else if (p->frequency <= 1866) // DDR3733
    {
        p->freqGroup = 1866;
    }
    else // DDR4266 
    {
        p->freqGroup = 2133;
    }
 
    mcSHOW_DBG_MSG3(("[setFreqGroup] p-> frequency %u, freqGroup: %u\n", p->frequency, p->freqGroup));
    return;
}

U16 GetFreqBySel(DRAMC_CTX_T *p, DRAM_PLL_FREQ_SEL_T sel)
{
    U16 u2freq=0;
        
    switch(sel)
    {
        case LP4_DDR4266:
            u2freq=2133;
            break;
        case LP4_DDR3733:
            u2freq=1866;
            break;
        case LP4_DDR3200:
            u2freq=1600;
            break;
        case LP4_DDR2667:
            u2freq=1333;
            break;
        case LP4_DDR2400:
            u2freq=1200;
            break;
        case LP4_DDR1600:
            u2freq=800;
            break;
        case LP4_DDR1200:
            u2freq=600;
            break;
        case LP4_DDR800:
            u2freq=400;
            break;
        default:
            mcSHOW_ERR_MSG(("[GetFreqBySel] freq sel is incorrect !!!\n"));
            break;
    }

    return u2freq;
}

DRAM_PLL_FREQ_SEL_T GetSelByFreq(DRAMC_CTX_T *p, U16 u2freq)
{
    DRAM_PLL_FREQ_SEL_T sel=0;
        
    switch(u2freq)
    {
        case 2133:
            sel=LP4_DDR4266;
            break;
        case 1866:
            sel=LP4_DDR3733;
            break;
        case 1600:
            sel=LP4_DDR3200;
            break;
        case 1333:
            sel=LP4_DDR2667;
            break;
        case 1200:
            sel=LP4_DDR2400;
            break;
        case 800:
            sel=LP4_DDR1600;
            break;
        case 600:
            sel=LP4_DDR1200;
            break;
        case 400:
            sel=LP4_DDR800;
            break;
        default:
            mcSHOW_ERR_MSG(("[GetSelByFreq] sel is incorrect !!!\n"));
            break;
    }

    return sel;
}

void DDRPhyFreqSel(DRAMC_CTX_T *p, DRAM_PLL_FREQ_SEL_T sel)
{
    switch(sel)
    {
        case LP4_DDR4266:
            p->frequency=2133;
            break;
        case LP4_DDR3733:
            p->frequency=1866;
            break;
        case LP4_DDR3200:
            p->frequency=1600;
            break;
        case LP4_DDR2667:
            p->frequency=1333;
            break;
        case LP4_DDR2400:
            p->frequency=1200;
            break;
        case LP4_DDR1600:
            p->frequency=800;
            break;
        case LP4_DDR1200:
            p->frequency=600;
            break;
        case LP4_DDR800:
            p->frequency=400;
            break;
        default:
            p->frequency=800;
            break;
    }

    p->dram_fsp = (p->frequency < MRFSP_TERM_FREQ) ? FSP_0 : FSP_1;
    p->odt_onoff = (p->frequency < MRFSP_TERM_FREQ) ? ODT_OFF : ODT_ON;

    if(p->dram_type == TYPE_LPDDR4P)
        p->odt_onoff = ODT_OFF;

    p->shu_type = get_shuffleIndex_by_Freq(p);
    setFreqGroup(p); /* Set p->freqGroup to support freqs not in ACTimingTable */

    ///TODO: add DBI_onoff by condition
    //p->DBI_onoff = p->odt_onoff;
}

U16 u2FreqMax=0;
U16 u2DFSGetHighestFreq(DRAMC_CTX_T * p)
{
    U8 u1ShuffleIdx = 0;
    U16 u2Freq=0;

    if ((u2FreqMax == 0) || (gUpdateHighestFreq == TRUE))
    {
        gUpdateHighestFreq = FALSE;
        for (u1ShuffleIdx = DRAM_DFS_SHUFFLE_1; u1ShuffleIdx < DRAM_DFS_SHUFFLE_MAX; u1ShuffleIdx++)
        { 
            u2Freq = GetFreqBySel(p, gFreqTbl[u1ShuffleIdx].freq_sel);
            if(u2FreqMax < u2Freq)
                u2FreqMax = u2Freq;
        }
    }

    return u2FreqMax;
 }

U8 GetEyeScanEnable(DRAMC_CTX_T * p, U8 get_type)
{
#if ENABLE_EYESCAN_GRAPH
#if (fcFOR_CHIP_ID == fcKrug)  //need check unterm highest freq is saved at SRAM_SHU4??

    //CBT
    if (get_type == 0)
    {
        if (gCBT_EYE_Scan_flag==0) return DISABLE;
        if (gCBT_EYE_Scan_only_higheset_freq_flag == 0) return ENABLE;  //K All freq 
        if (p->frequency == u2DFSGetHighestFreq(p)) return ENABLE;      // K highest freq
        if (vGet_Current_ShuLevel(p)==SRAM_SHU4) return ENABLE;         // K unterm highest freq
    }

    //RX
    if (get_type == 1)
    {
        if (gRX_EYE_Scan_flag==0) return DISABLE;
        if (gRX_EYE_Scan_only_higheset_freq_flag == 0) return ENABLE;  //K All freq 
        if (p->frequency == u2DFSGetHighestFreq(p)) return ENABLE;      // K highest freq
        if (vGet_Current_ShuLevel(p)==SRAM_SHU4) return ENABLE;         // K unterm highest freq
    }

    //TX
    if (get_type == 2)
    {
        if (gTX_EYE_Scan_flag==0) return DISABLE;
        if (gTX_EYE_Scan_only_higheset_freq_flag == 0) return ENABLE;  //K All freq 
        if (p->frequency == u2DFSGetHighestFreq(p)) return ENABLE;      // K highest freq
        if (vGet_Current_ShuLevel(p)==SRAM_SHU4) return ENABLE;         // K unterm highest freq
    }    

#endif    
#endif    

    return DISABLE;
}

#define CKGEN_FMETER 0x0
#define ABIST_FMETER 0x1

/*
1. Select meter clock input: CLK_DBG_CFG[1:0] = 0x0
2. Select clock source from below table: CLK_DBG_CFG[21:16] = 0x39
3. Setup meter div: CLK_MISC_CFG_0[31:24] = 0x0
4. Enable frequency meter: CLK26CALI_0[12] = 0x1
5. Trigger frequency meter: CLK26CALI_0[4] = 0x1
6. Wait until CLK26CALI_0[4] = 0x0
7. Read meter count: CLK26CALI_1[15:0]
8. Calculate measured frequency: freq. = (26 * cal_cnt) / 1024

DE: HK Wean
*/
static U32 FMeter(U8 u1CLKMeterSel, U8 u1CLKMuxSel)
{
#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
    U32 tmp, u4CalCnt;

    // enable fmeter
	DRV_WriteReg32(CLK26CALI_0, (0x1 << 12)); // [12]=1: [8]=0: switch in_clk/clk27m
	tmp = DRV_Reg32(CLK_DBG_CFG);
	tmp &= ~0x3;

	if (u1CLKMeterSel == CKGEN_FMETER)
		tmp |= 0x1;

	DRV_WriteReg32(CLK_DBG_CFG, tmp);


    // select fmeter's input clock
	tmp = DRV_Reg32(CLK_DBG_CFG);

	if (u1CLKMeterSel == CKGEN_FMETER) {
		tmp &= ~(0x3F << 8);
		tmp |=  u1CLKMuxSel << 8;
	} else {
		tmp &= ~(0x3F << 16);
		tmp |=  u1CLKMuxSel << 16;
	}

	DRV_WriteReg32(CLK_DBG_CFG, tmp);

	// setup fmeter div
	tmp = DRV_Reg32(CLK_MISC_CFG_0);
	tmp &= ~(0xFF << 24);
	DRV_WriteReg32(CLK_MISC_CFG_0, tmp);

	// trigger fmeter
	tmp = DRV_Reg32(CLK26CALI_0);
	tmp |= (0x1 << 4);
	DRV_WriteReg32(CLK26CALI_0, tmp);

	// wait
	while (DRV_Reg32(CLK26CALI_0) & (0x1 << 4)) {
		mcDELAY_US(1);
	}

	// read
	u4CalCnt = DRV_Reg32(CLK26CALI_1) & 0xFFFF;

    // disable fmeter
	tmp = DRV_Reg32(CLK26CALI_0);
	tmp &= ~(0x1 << 12);
	DRV_WriteReg32(CLK26CALI_0, tmp);

    return ((u4CalCnt * 26) / 1024);
#endif
}

U16 gddrphyfmeter_value=0;
void DDRPhyFMeter_Init(DRAMC_CTX_T *p)
{
#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
#if (fcFOR_CHIP_ID == fcKrug)
    U32 reg;

    // enable ck_bfe_dcm_en for freqmeter measure ddrphy clock, not needed for normal use
    reg = DRV_Reg32(Channel_A_PHY_AO_BASE_ADDRESS + 0x2a0);
    DRV_WriteReg32(Channel_A_PHY_AO_BASE_ADDRESS + 0x2a0, reg | (1 << 11));

    // ckgen_clk60: fmem_ck_aft_dcm_ch0 (DRAMC CHA's clock after idle mask, the one really be used)
    gddrphyfmeter_value = FMeter(CKGEN_FMETER, 60) << 2;

    if (vGet_Div_Mode(p) == DIV4_MODE)
        gddrphyfmeter_value >>= 1;

    DRV_WriteReg32(Channel_A_PHY_AO_BASE_ADDRESS + 0x2a0, reg);

#else
    #error Need check of the DDRPhyFMeter for your chip !!!
#endif
#endif
}

U16 DDRPhyFMeter()
{
    return gddrphyfmeter_value;
}


#if ENABLE_DDRPHY_FREQ_METER
U32 DDRPhyFreqMeter()
{
#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
    U32 reg;
    U32 before_value, after_value;

    mcSHOW_DBG_MSG3(("\n[PhyFreqMeter]\n"));

    // abist_clk50: AD_MPLL_208M_CK
    mcSHOW_DBG_MSG(("AD_MPLL_208M_CK FREQ=%d\n", FMeter(ABIST_FMETER, 50)));

    // abist_clk57: DA_MPLL_52M_DIV_CK
    mcSHOW_DBG_MSG(("DA_MPLL_52M_DIV_CK FREQ=%d\n", FMeter(ABIST_FMETER, 57)));

	// enable ck_bfe_dcm_en for freqmeter measure ddrphy clock, not needed for normal use
    reg = DRV_Reg32(Channel_A_PHY_AO_BASE_ADDRESS + 0x2a0);
    DRV_WriteReg32(Channel_A_PHY_AO_BASE_ADDRESS + 0x2a0, reg | (1 << 11));

    // ckgen_clk59: fmem_ck_bfe_dcm_ch0 (DRAMC CHA's clock before idle mask)
    before_value = FMeter(CKGEN_FMETER, 59);
    mcSHOW_DBG_MSG(("fmem_ck_bfe_dcm_ch0 FREQ=%d\n", FMeter(CKGEN_FMETER, 59)));

    // ckgen_clk60: fmem_ck_aft_dcm_ch0 (DRAMC CHA's clock after idle mask, the one really be used)
    after_value = FMeter(CKGEN_FMETER, 60);
    mcSHOW_DBG_MSG(("fmem_ck_aft_dcm_ch0 FREQ=%d\n", FMeter(CKGEN_FMETER, 60)));

    DRV_WriteReg32(Channel_A_PHY_AO_BASE_ADDRESS + 0x2a0, reg);

    return ((before_value<<16) | after_value);
#else
    return 0;
#endif
}
#endif

#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
#if __ETT__
void GetPhyPllFrequency(DRAMC_CTX_T *p)
{
#if 1
    //U8 u1ShuLevel = u4IO32ReadFldAlign(DRAMC_REG_SHUSTATUS, SHUSTATUS_SHUFFLE_LEVEL);
    U8 u1ShuLevel = u4IO32ReadFldAlign(SPM_SPM_POWER_ON_VAL0, SPM_POWER_ON_VAL0_SC_DR_SHU_LEVEL0);
    U32 u4PLL5_ADDR = DDRPHY_SHU_PLL5 + SHU_GRP_DDRPHY_OFFSET * u1ShuLevel;
    U32 u4PLL8_ADDR = DDRPHY_SHU_PLL8 + SHU_GRP_DDRPHY_OFFSET * u1ShuLevel;
    U32 u4CA_CMD6 = DDRPHY_SHU_CA_CMD6 + SHU_GRP_DDRPHY_OFFSET * u1ShuLevel;
    U32 u4PLL4 = DDRPHY_SHU_PLL4 + SHU_GRP_DDRPHY_OFFSET * u1ShuLevel;

    /* VCOFreq = FREQ_XTAL x ((RG_RCLRPLL_SDM_PCW) / 2^(RG_*_RCLRPLL_PREDIV)) / 2^(RG_*_RCLRPLL_POSDIV) */
    U32 u4SDM_PCW = u4IO32ReadFldAlign(u4PLL5_ADDR, SHU_PLL5_RG_RPHYPLL_SDM_PCW);
    U32 u4PREDIV = u4IO32ReadFldAlign(u4PLL8_ADDR, SHU_PLL8_RG_RPHYPLL_PREDIV);
    U32 u4POSDIV = u4IO32ReadFldAlign(u4PLL8_ADDR, SHU_PLL8_RG_RPHYPLL_POSDIV);
    U32 u4CKDIV4 = u4IO32ReadFldAlign(u4CA_CMD6, SHU_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA);
    U16 u2CKMUL2 = u4IO32ReadFldAlign(u4PLL4, SHU_PLL4_RG_RPHYPLL_RESERVED);

    U32 u4VCOFreq = ((52>>u4PREDIV)*(u4SDM_PCW>>8))>>u4POSDIV;
    U32 u4DataRate = u4VCOFreq>>u4CKDIV4;
    if (p->frequency == 400) // for Eig_er 1:4 mode DDR800
        u4DataRate >>= 1;
    if (u2CKMUL2 & 0x2000)
        u4DataRate <<= 1;
#else
    U8 u1ShuLevel = (*((volatile unsigned int *)(0x10006474))&0x000000c0)>>6;
    U32 u4PLL5_ADDR = 0x10228d94 + SHU_GRP_DDRPHY_OFFSET * u1ShuLevel;
    U32 u4PLL8_ADDR = 0x10228da0 + SHU_GRP_DDRPHY_OFFSET * u1ShuLevel;
    U32 u4CA_CMD6   = 0x10228d18 + SHU_GRP_DDRPHY_OFFSET * u1ShuLevel;

    U32 u4SDM_PCW = *((volatile unsigned int *)(u4PLL5_ADDR))>>16;
    U32 u4PREDIV = (*((volatile unsigned int *)(u4PLL8_ADDR))&0x000c0000)>>18;
    U32 u4POSDIV = (*((volatile unsigned int *)(u4PLL8_ADDR))&0x00000007);
    U32 u4CKDIV4 = (*((volatile unsigned int *)(u4CA_CMD6))&0x08000000)>>27;


    U32 u4VCOFreq = ((52>>u4PREDIV)*(u4SDM_PCW>>8))>>u4POSDIV;
    U32 u4DataRate = u4VCOFreq>>u4CKDIV4;
#endif
    //mcSHOW_DBG_MSG(("PCW=0x%X, u4PREDIV=%d, u4POSDIV=%d, CKDIV4=%d, DataRate=%d\n", u4SDM_PCW, u4PREDIV, u4POSDIV, u4CKDIV4, u4DataRate));
    mcSHOW_DBG_MSG(("[F] DataRate=%d\n", u4DataRate));
}
#endif
#endif

DRAM_PLL_FREQ_SEL_T vGet_PLL_FreqSel(DRAMC_CTX_T *p)
{
    return p->pDFSTable->freq_sel;
}

void vSet_PLL_FreqSel(DRAMC_CTX_T *p, DRAM_PLL_FREQ_SEL_T sel)
{
    p->pDFSTable->freq_sel = sel;
}

DDR800_MODE_T vGet_DDR800_Mode(DRAMC_CTX_T *p)
{
    return p->pDFSTable->ddr800mode;
}

void vSet_Div_Mode(DRAMC_CTX_T *p, DIV_MODE_T eMode)
{
    p->pDFSTable->divmode = eMode;
}

DIV_MODE_T vGet_Div_Mode(DRAMC_CTX_T *p)
{
    return p->pDFSTable->divmode;
}

void vSet_Current_ShuLevel(DRAMC_CTX_T *p, DRAM_DFS_SRAM_SHU_T u1ShuIndex)
{
    p->pDFSTable->shuffleIdx = u1ShuIndex;
}

DRAM_DFS_SRAM_SHU_T vGet_Current_ShuLevel(DRAMC_CTX_T *p)
{
    return p->pDFSTable->shuffleIdx;
}


#if 0
void vSet_Duty_Calibration_Mode(DRAMC_CTX_T *p, U8 kMode)
{
    p->pDFSTable->duty_calibration_mode = kMode;
}
#endif

DUTY_CALIBRATION_T vGet_Duty_Calibration_Mode(DRAMC_CTX_T *p)
{
    return p->pDFSTable->duty_calibration_mode;
}

VREF_CALIBRATION_ENABLE_T Get_Vref_Calibration_OnOff(DRAMC_CTX_T *p)
{
    return p->pDFSTable->vref_calibartion_enable;
}

/* vGet_Dram_CBT_Mode
 * Due to current HW design (both ranks share the same set of ACTiming regs), mixed
 * mode LP4 now uses byte mode ACTiming settings. This means most calibration steps
 * should use byte mode code flow.
 * Note: The below items must have per-rank settings (Don't use this function)
 * 1. CBT training 2. TX tracking
 */
DRAM_CBT_MODE_T vGet_Dram_CBT_Mode(DRAMC_CTX_T *p)
{
    if(p->dram_cbt_mode[RANK_0] == CBT_NORMAL_MODE && p->dram_cbt_mode[RANK_1] == CBT_NORMAL_MODE)
    {
        return CBT_NORMAL_MODE;
    }
    else // For Mixed mode & Byte mode LP4
    {
        return CBT_BYTE_MODE1;
    }
}


void vPrintCalibrationBasicInfo(DRAMC_CTX_T *p)
{
#if __ETT__
    mcSHOW_DBG_MSG(("===============================================================================\n"
                    "Dram Type= %d, Freq= %u, FreqGroup= %u, CH_%d, rank %d\n"
                    "fsp= %d, odt_onoff= %d, Byte mode= %d, DivMode= %d\n"
                    "===============================================================================\n",
                        p->dram_type, DDRPhyFMeter(), p->freqGroup, p->channel, p->rank,
                        p->dram_fsp, p->odt_onoff, p->dram_cbt_mode[p->rank], vGet_Div_Mode(p)));
    mcDUMP_REG_MSG(("===============================================================================\n"
                    "Dram Type= %d, Freq= %u, FreqGroup= %u, CH_%d, rank %d\n"
                    "fsp= %d, odt_onoff= %d, Byte mode= %d, DivMode= %d\n"
                    "===============================================================================\n",
                        p->dram_type, DDRPhyFMeter(), p->freqGroup, p->channel, p->rank,
                        p->dram_fsp, p->odt_onoff, p->dram_cbt_mode[p->rank], vGet_Div_Mode(p)));
#else
    mcSHOW_DBG_MSG(("===============================================================================\n"
                    "Dram Type= %d, Freq= %u, CH_%d, rank %d\n"
                    "fsp= %d, odt_onoff= %d, Byte mode= %d, DivMode= %d\n"
                    "===============================================================================\n",
                        p->dram_type, DDRPhyFMeter(), p->channel, p->rank,
                        p->dram_fsp, p->odt_onoff, p->dram_cbt_mode[p->rank], vGet_Div_Mode(p)));
#endif
}

#if VENDER_JV_LOG
void vPrintCalibrationBasicInfo_ForJV(DRAMC_CTX_T *p)
{
    mcSHOW_JV_LOG_MSG(("\n\nDram type:"));

    switch(p->dram_type)
    {
        case TYPE_LPDDR4:
            mcSHOW_JV_LOG_MSG(("LPDDR4\t"));
            break;

        case TYPE_LPDDR4X:
            mcSHOW_JV_LOG_MSG(("LPDDR4X\t"));
            break;

        case TYPE_LPDDR4P:
            mcSHOW_JV_LOG_MSG(("LPDDR4P\t"));
            break;
    }

    mcSHOW_JV_LOG_MSG(("Freq: %d, FreqGroup %u, channel %d, rank %d\n"
                     "dram_fsp= %d, odt_onoff= %d, Byte mode= %d, DivMode= %d\n\n",
                                        p->frequency, p->freqGroup, p->channel, p->rank,
                                        p->dram_fsp, p->odt_onoff, p->dram_cbt_mode[p->rank], vGet_Div_Mode(p)));

    return;
}
#endif

void DramcWriteDBIOnOff(DRAMC_CTX_T *p, U8 onoff)
{
    // DRAMC Write-DBI On/Off
     vIO32WriteFldAlign_All(DRAMC_REG_SHU_WODT, onoff, SHU_WODT_DBIWR);
    mcSHOW_DBG_MSG(("DramC Write-DBI %s\n", ((onoff == DBI_ON) ? "on" : "off")));
    mcDUMP_REG_MSG(("DramC Write-DBI %s\n", ((onoff == DBI_ON) ? "on" : "off")));
 }

void DramcReadDBIOnOff(DRAMC_CTX_T *p, U8 onoff)
{
    // DRAMC Read-DBI On/Off
    vIO32WriteFldAlign_All(DDRPHY_SHU_B0_DQ7, onoff, SHU_B0_DQ7_R_DMDQMDBI_SHU_B0);
    vIO32WriteFldAlign_All(DDRPHY_SHU_B1_DQ7, onoff, SHU_B1_DQ7_R_DMDQMDBI_SHU_B1);
    
    mcSHOW_DBG_MSG(("DramC Read-DBI %s\n", ((onoff == DBI_ON) ? "on" : "off")));
 }
#if ENABLE_READ_DBI
void SetDramModeRegForReadDBIOnOff(DRAMC_CTX_T *p, U8 u1fsp, U8 onoff)
{
    #if MRW_CHECK_ONLY
    mcSHOW_MRW_MSG(("\n==[MR Dump] %s==\n", __func__));
    #endif        
    //mcSHOW_DBG_MSG(("--Fsp%d --\n", p->dram_fsp));
    
    //DRAM MR3[6] read-DBI On/Off
    u1MR03Value[u1fsp] = ((u1MR03Value[u1fsp] & 0xbf) | (onoff<<6));
    DramcModeRegWriteByRank(p, p->rank, 3, u1MR03Value[u1fsp]);
 }
#endif

#if ENABLE_WRITE_DBI
void SetDramModeRegForWriteDBIOnOff(DRAMC_CTX_T *p, U8 u1fsp, U8 onoff)
{
 #if MRW_CHECK_ONLY
    mcSHOW_MRW_MSG(("\n==[MR Dump] %s==\n", __func__));
#endif        
    //DRAM MR3[7] write-DBI On/Off
    u1MR03Value[u1fsp] = ((u1MR03Value[u1fsp] & 0x7F) | (onoff<<7));
    DramcModeRegWriteByRank(p, p->rank, 3, u1MR03Value[u1fsp]);
 }
#endif


void AutoRefreshCKEOff(DRAMC_CTX_T *p)
{
    U32 u4backup_broadcast;
#if ENABLE_TMRRI_NEW_MODE
    U8 u1RankIdx;
#endif
    //backup broadcast on/off
    u4backup_broadcast = GetDramcBroadcast();

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);

    //if(p->vendor_id==VENDOR_SAMSUNG && p->dram_type==TYPE_LPDDR3)
    {
        mcSHOW_DBG_MSG(("AutoRefreshCKEOff AutoREF OFF\n"));
        vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL0, 0x1, REFCTRL0_REFDIS);
        mcDELAY_US(3);
        mcSHOW_DBG_MSG(("DDRPhyPLLSetting-CKEOFF\n"));

        CKEFixOnOff(p, CKE_WRITE_TO_ALL_RANK, CKE_FIXOFF, CKE_WRITE_TO_ALL_CHANNEL);
			
        mcDELAY_US(1);
    }
    //restore broadcast on/off
    DramcBroadcastOnOff(u4backup_broadcast);
}

void CKEFixOnOff(DRAMC_CTX_T *p, U8 u1RankIdx, CKE_FIX_OPTION option, CKE_FIX_CHANNEL WriteChannelNUM)
{
    U8 u1CKEOn, u1CKEOff, u1setChannel,u1BackupChannel;

    if (option == CKE_DYNAMIC) //if CKE is dynamic, set both CKE fix On and Off as 0
    {                          //After CKE FIX on/off, CKE should be returned to dynamic (control by HW)
        u1CKEOn = u1CKEOff = 0;
    }
    else //if CKE fix on is set as 1, CKE fix off should also be set as 0; vice versa
    {
        u1CKEOn = option;
        u1CKEOff = (1 - option);
    }

    if (WriteChannelNUM == CKE_WRITE_TO_ALL_CHANNEL) //write register to all channel
    {
    #if ENABLE_TMRRI_NEW_MODE
        if (u1RankIdx == RANK_0)
        {
            vIO32WriteFldMulti_All(DRAMC_REG_CKECTRL, P_Fld(u1CKEOff, CKECTRL_CKEFIXOFF)|P_Fld(u1CKEOn, CKECTRL_CKEFIXON));
        }
        else if (u1RankIdx == RANK_1)
        {
            vIO32WriteFldMulti_All(DRAMC_REG_CKECTRL, P_Fld(u1CKEOff, CKECTRL_CKE1FIXOFF)|P_Fld(u1CKEOn, CKECTRL_CKE1FIXON));
        }
        else if (u1RankIdx == CKE_WRITE_TO_ALL_RANK)
        {
            vIO32WriteFldMulti_All(DRAMC_REG_CKECTRL, P_Fld(u1CKEOff, CKECTRL_CKEFIXOFF)|P_Fld(u1CKEOn, CKECTRL_CKEFIXON));
            if (p->support_rank_num == RANK_DUAL)
            {
                vIO32WriteFldMulti_All(DRAMC_REG_CKECTRL, P_Fld(u1CKEOff, CKECTRL_CKE1FIXOFF)|P_Fld(u1CKEOn, CKECTRL_CKE1FIXON));
            }
        }
    #else
        vIO32WriteFldMulti_All(DRAMC_REG_CKECTRL, P_Fld(u1CKEOff, CKECTRL_CKEFIXOFF)|P_Fld(u1CKEOn, CKECTRL_CKEFIXON));
        if ((u1RankIdx == CKE_WRITE_TO_ALL_RANK) && (p->support_rank_num == RANK_DUAL))
        {
            vIO32WriteFldMulti_All(DRAMC_REG_CKECTRL, P_Fld(u1CKEOff, CKECTRL_CKE1FIXOFF)|P_Fld(u1CKEOn, CKECTRL_CKE1FIXON));
        }
    #endif
    }
    else
    {
    #if ENABLE_TMRRI_NEW_MODE
        if (u1RankIdx == RANK_0)
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), P_Fld(u1CKEOff, CKECTRL_CKEFIXOFF)|P_Fld(u1CKEOn, CKECTRL_CKEFIXON));
        }
        else if (u1RankIdx == RANK_1)
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), P_Fld(u1CKEOff, CKECTRL_CKE1FIXOFF)|P_Fld(u1CKEOn, CKECTRL_CKE1FIXON));
        }
        else if (u1RankIdx == CKE_WRITE_TO_ALL_RANK)
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), P_Fld(u1CKEOff, CKECTRL_CKEFIXOFF)|P_Fld(u1CKEOn, CKECTRL_CKEFIXON));
            if (p->support_rank_num == RANK_DUAL)
            {
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), P_Fld(u1CKEOff, CKECTRL_CKE1FIXOFF)|P_Fld(u1CKEOn, CKECTRL_CKE1FIXON));
            }
        }
    #else
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), P_Fld(u1CKEOff, CKECTRL_CKEFIXOFF)|P_Fld(u1CKEOn, CKECTRL_CKEFIXON));
        if ((u1RankIdx == CKE_WRITE_TO_ALL_RANK) && (p->support_rank_num == RANK_DUAL))
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), P_Fld(u1CKEOff, CKECTRL_CKE1FIXOFF)|P_Fld(u1CKEOn, CKECTRL_CKE1FIXON));
        }
    #endif
    }
}

void vAutoRefreshSwitch(DRAMC_CTX_T *p, U8 option)
{
    if (option == ENABLE)
    {
        //enable autorefresh
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0), 0, REFCTRL0_REFDIS);      //REFDIS=0, enable auto refresh        
    }
    else
    {
        //disable autorefresh
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0), 1, REFCTRL0_REFDIS);      //REFDIS=1, disable auto refresh    

        //because HW will actually disable autorefresh after refresh_queue empty, so we need to wait quene empty.
        mcDELAY_US(u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MISC_STATUSA), MISC_STATUSA_REFRESH_QUEUE_CNT)*4);   //wait refresh_queue_cnt * 3.9us
    }
}


//-------------------------------------------------------------------------
/** vCKERankCtrl
 *  Control CKE toggle mode (toggle both ranks 1. at the same time (CKE_RANK_DEPENDENT) 2. individually (CKE_RANK_INDEPENDENT))
 *  Note: Sets CKE toggle mode for all channels
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param CKECtrlMode      Indicates 
 */
//-------------------------------------------------------------------------
void vCKERankCtrl(DRAMC_CTX_T *p, CKE_CTRL_MODE_T CKECtrlMode)
{
    /* Struct indicating all register fields mentioned in "multi rank CKE control" */
    typedef struct
    {
        U8 u1CKE2Rank :Fld_wid(RKCFG_CKE2RANK);
        U8 u1CKE2Rank_Opt :Fld_wid(RKCFG_CKE2RANK_OPT);
        U8 u1CKE2Rank_Opt2 :Fld_wid(RKCFG_CKE2RANK_OPT2);
        U8 u1CKE2Rank_Opt3 :Fld_wid(CKECTRL_CKE2RANK_OPT3);
        U8 u1CKE2Rank_Opt5 :Fld_wid(CKECTRL_CKE2RANK_OPT5);
        U8 u1CKE2Rank_Opt6 :Fld_wid(CKECTRL_CKE2RANK_OPT6);
        U8 u1CKE2Rank_Opt7 :Fld_wid(CKECTRL_CKE2RANK_OPT7);
        U8 u1CKE2Rank_Opt8 :Fld_wid(CKECTRL_CKE2RANK_OPT8);
        U8 u1CKETimer_Sel :Fld_wid(CKECTRL_CKETIMER_SEL);
        U8 u1FASTWake    :Fld_wid(SHU_CONF2_FASTWAKE);
        U8 u1FASTWake2    :Fld_wid(SHU_CONF2_FASTWAKE2);
        U8 u1FastWake_Sel :Fld_wid(CKECTRL_FASTWAKE_SEL);
        U8 u1CKEWake_Sel :Fld_wid(CKECTRL_CKEWAKE_SEL);
        U8 u1ClkWiTrfc    :Fld_wid(DRAMCTRL_CLKWITRFC);
    } CKE_CTRL_T;

    /* CKE_Rank dependent/independent mode register setting values */
    CKE_CTRL_T CKE_Mode, CKE_Rank_Independent = { .u1CKE2Rank = 0, .u1CKE2Rank_Opt = 0, .u1CKE2Rank_Opt2 = 1, .u1CKE2Rank_Opt3 = 0,
                                                  .u1CKE2Rank_Opt5 = 0, .u1CKE2Rank_Opt6 = 0, .u1CKE2Rank_Opt7 = 1, .u1CKE2Rank_Opt8 = 0,
                                                  .u1CKETimer_Sel = 0, .u1FASTWake = 1, .u1FASTWake2 = 1, .u1FastWake_Sel = 1, .u1CKEWake_Sel = 0, .u1ClkWiTrfc = 0
                                                },
                         CKE_Rank_Dependent = { .u1CKE2Rank = 1, .u1CKE2Rank_Opt = 0, .u1CKE2Rank_Opt2 = 1, .u1CKE2Rank_Opt3 = 0,
                                                .u1CKE2Rank_Opt5 = 0, .u1CKE2Rank_Opt6 = 0, .u1CKE2Rank_Opt7 = 0, .u1CKE2Rank_Opt8 = 0, .u1CKETimer_Sel = 1,
                                                .u1FASTWake = 1, .u1FASTWake2 = 0, .u1FastWake_Sel = 0, .u1CKEWake_Sel = 0, .u1ClkWiTrfc = 0
                                              };
    //Select CKE control mode
    CKE_Mode = (CKECtrlMode == CKE_RANK_INDEPENDENT) ? CKE_Rank_Independent : CKE_Rank_Dependent;

    //Apply CKE control mode register settings
    vIO32WriteFldMulti_All(DRAMC_REG_RKCFG, P_Fld(CKE_Mode.u1CKE2Rank, RKCFG_CKE2RANK)
                    | P_Fld(CKE_Mode.u1CKE2Rank_Opt, RKCFG_CKE2RANK_OPT)
                    | P_Fld(CKE_Mode.u1CKE2Rank_Opt2, RKCFG_CKE2RANK_OPT2));

    vIO32WriteFldMulti_All(DRAMC_REG_CKECTRL, P_Fld(CKE_Mode.u1CKE2Rank_Opt3, CKECTRL_CKE2RANK_OPT3)
                    | P_Fld(CKE_Mode.u1CKE2Rank_Opt5, CKECTRL_CKE2RANK_OPT5)
                    | P_Fld(CKE_Mode.u1CKE2Rank_Opt6, CKECTRL_CKE2RANK_OPT6)
                    | P_Fld(CKE_Mode.u1CKE2Rank_Opt7, CKECTRL_CKE2RANK_OPT7)
                    | P_Fld(CKE_Mode.u1CKE2Rank_Opt8, CKECTRL_CKE2RANK_OPT8)
                    | P_Fld(CKE_Mode.u1CKETimer_Sel, CKECTRL_CKETIMER_SEL)
                    | P_Fld(CKE_Mode.u1FastWake_Sel, CKECTRL_FASTWAKE_SEL)
                    | P_Fld(CKE_Mode.u1CKEWake_Sel, CKECTRL_CKEWAKE_SEL));
	
    vIO32WriteFldMulti_All(DRAMC_REG_SHU_CONF2, P_Fld(CKE_Mode.u1FASTWake, SHU_CONF2_FASTWAKE) | P_Fld(CKE_Mode.u1FASTWake2, SHU_CONF2_FASTWAKE2));

    vIO32WriteFldAlign_All(DRAMC_REG_DRAMCTRL, CKE_Mode.u1ClkWiTrfc, DRAMCTRL_CLKWITRFC);
}

#define MAX_CMP_CPT_WAIT_LOOP 100000   // max loop
void DramcSetRWOFOEN(DRAMC_CTX_T *p, U8 u1onoff)
{
    U32 u4loop_count = 0;
    static U8 u1onoff_flag = 1;

    if(u1onoff_flag != u1onoff)
    {
        while(u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MISC_STATUSA), MISC_STATUSA_REQQ_EMPTY) != 1)
        {
            mcDELAY_US(1);
            u4loop_count ++;

            if(u4loop_count > MAX_CMP_CPT_WAIT_LOOP)
            {
                mcSHOW_ERR_MSG(("RWOFOEN timout! queue is not empty\n"));
            #if __ETT__
                while(1);
            #else
                break;
            #endif
            }
        }
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PERFCTL0), u1onoff, PERFCTL0_RWOFOEN);
    }
    u1onoff_flag = u1onoff;
}

void DramcSetRankEngine2(DRAMC_CTX_T *p, U8 u1RankSel)
{
    //LPDDR2_3_ADRDECEN_TARKMODE =0, always rank0
    /* ADRDECEN_TARKMODE: rank input selection
     *   1'b1 select CTO_AGENT1_RANK, 1'b0 rank by address decode
     */
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMCTRL), 1, DRAMCTRL_ADRDECEN_TARKMODE);

    // DUMMY_TESTAGENTRKSEL =0, select rank according to CATRAIN_TESTAGENTRK
    /* TESTAGENTRKSEL: Test agent access rank mode selection
     * 2'b00: rank selection by TESTAGENTRK,           2'b01: rank selection by CTO_AGENT_1_BK_ADR[0]
     * 2'b10: rank selection by CTO_AGENT1_COL_ADR[3], 2'b11: rank selection by CTO_AGENT1_COL_ADR[4]
     */
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), 0, TEST2_4_TESTAGENTRKSEL);

    //CATRAIN_TESTAGENTRK = u1RankSel
    /* TESTAGENTRK: Specify test agent rank
     * 2'b00 rank 0, 2'b01 rank 1, 2'b10 rank 2
     */
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), u1RankSel, TEST2_4_TESTAGENTRK);
}

void DramcEngine2SetPat(DRAMC_CTX_T *p, U8 testaudpat, U8 log2loopcount, U8 Use_Len1_Flag)
{
    if (testaudpat == TEST_XTALK_PATTERN)   // xtalk
    {
        //TEST_REQ_LEN1=1 is new feature, hope to make dq bus continously.
        //but DV simulation will got problem of compare err
        //so go back to use old way
        //TEST_REQ_LEN1=0, R_DMRWOFOEN=1
        if (Use_Len1_Flag != 0)
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), P_Fld(1, TEST2_4_TEST_REQ_LEN1));   //test agent 2 with cmd length = 0, LEN1 of 256bits data  
            DramcSetRWOFOEN(p, DISABLE); //The per bank refresh will cause TA2 command reorder and compare error  
        }
        else
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), P_Fld(0, TEST2_4_TEST_REQ_LEN1));   //test agent 2 with cmd length = 0
            DramcSetRWOFOEN(p, ENABLE);
        }
        //vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_PERFCTL0), P_Fld(1, PERFCTL0_RWOFOEN)); //@IPM will fix for LEN1=1 issue

        // select XTALK pattern
        // set addr 0x044 [7] to 0
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(0, TEST2_3_TESTAUDPAT)|P_Fld(log2loopcount,TEST2_3_TESTCNT)); //dont use audio pattern

        // set addr 0x48[16] to 1, TESTXTALKPAT = 1
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), P_Fld(0, TEST2_4_TESTDMITGLPAT)|P_Fld(1, TEST2_4_TESTXTALKPAT)|P_Fld(0,TEST2_4_TESTAUDMODE)|P_Fld(0,TEST2_4_TESTAUDBITINV));  //use XTALK pattern, dont use audio pattern

        // R_DMTESTSSOPAT=0, R_DMTESTSSOXTALKPAT=0
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), P_Fld(0, TEST2_4_TESTSSOPAT)|P_Fld(0,TEST2_4_TESTSSOXTALKPAT));   //dont use sso, sso+xtalk pattern
    }
    else if (testaudpat == TEST_AUDIO_PATTERN)   // audio
    {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), P_Fld(0, TEST2_4_TEST_REQ_LEN1));   //test agent 2 with cmd length = 0
        // set AUDINIT=0x11 AUDINC=0x0d AUDBITINV=1 AUDMODE=1(1:read only(address fix), 0: write/read address change)
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), \
            P_Fld(0x00000011, TEST2_4_TESTAUDINIT)|P_Fld(0x0000000d, TEST2_4_TESTAUDINC)| \
            P_Fld(0, TEST2_4_TESTDMITGLPAT)|P_Fld(0, TEST2_4_TESTXTALKPAT)|P_Fld(0,TEST2_4_TESTAUDMODE)|P_Fld(1,TEST2_4_TESTAUDBITINV));

        // set addr 0x044 [7] to 1 ,select audio pattern
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(1, TEST2_3_TESTAUDPAT)|P_Fld(log2loopcount,TEST2_3_TESTCNT));
    }
    else if (testaudpat == TEST_DMITGL_PATTERN) //DQM toggle every cycle
    {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), P_Fld(1, TEST2_4_TEST_REQ_LEN1));   //test agent 2 with cmd length = 0, LEN1 of 256bits data  
        DramcSetRWOFOEN(p, DISABLE); //The per bank refresh will cause TA2 command reorder and compare error

        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(0, TEST2_3_TESTAUDPAT)|P_Fld(log2loopcount,TEST2_3_TESTCNT)); //dont use audio pattern
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), P_Fld(1, TEST2_4_TESTDMITGLPAT)|P_Fld(0, TEST2_4_TESTXTALKPAT)|P_Fld(0,TEST2_4_TESTAUDMODE)|P_Fld(0,TEST2_4_TESTAUDBITINV));  //use DMITGL pattern, don't use XTALK pattern, dont use audio pattern        
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), P_Fld(0, TEST2_4_TESTSSOPAT)|P_Fld(0,TEST2_4_TESTSSOXTALKPAT));   //dont use sso, sso+xtalk pattern        
    }
    else   // ISI
    {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), P_Fld(0, TEST2_4_TEST_REQ_LEN1));   //test agent 2 with cmd length = 0
        // select ISI pattern
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(0, TEST2_3_TESTAUDPAT)|P_Fld(log2loopcount,TEST2_3_TESTCNT));
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), 0, TEST2_4_TESTXTALKPAT);
    }
}

#define CMP_CPT_POLLING_PERIOD 1       // timeout for TE2: (CMP_CPT_POLLING_PERIOD X MAX_CMP_CPT_WAIT_LOOP) 
void DramcEngine2CheckComplete(DRAMC_CTX_T *p, U8 u1status)
{
    U32 u4loop_count = 0;

    while((u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_TESTRPT)) & u1status) != u1status)
    {
        mcDELAY_US(CMP_CPT_POLLING_PERIOD);
        u4loop_count++;
        if ((u4loop_count > 3) && (u4loop_count <= MAX_CMP_CPT_WAIT_LOOP))
        {
            //mcSHOW_ERR_MSG(("TESTRPT_DM_CMP_CPT: %d\n", u4loop_count));
        }
        else if(u4loop_count > MAX_CMP_CPT_WAIT_LOOP)
        {
            /*TINFO="fcWAVEFORM_MEASURE_A %d: time out\n", u4loop_count*/
            mcSHOW_DBG_MSG(("fcWAVEFORM_MEASURE_A %d :time out, [22:20]=0x%x\n", u4loop_count, u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TESTRPT), TESTRPT_TESTSTAT)));
            mcFPRINTF((fp_A60501, "fcWAVEFORM_MEASURE_A %d: time out\n", u4loop_count));
            break;
        }
    }        
}

U32 DramcEngine2Compare(DRAMC_CTX_T *p, DRAM_TE_OP_T wr)
{
    U32 u4result = 0xffffffff;
    U32 u4loopcount;
    U8  u1status = 1; //RK0

    u4loopcount = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), TEST2_3_TESTCNT);
    if (u4loopcount==1)
        u1status = 3; //RK0/1
    
    if (wr == TE_OP_WRITE_READ_CHECK)
    {
        // read data compare ready check
        DramcEngine2CheckComplete(p, u1status);
    
        // disable write
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(0, TEST2_3_TEST2W) | P_Fld(0, TEST2_3_TEST2R) | P_Fld(0, TEST2_3_TEST1));
    
        mcDELAY_US(1);
    
        // enable read
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(0, TEST2_3_TEST2W) | P_Fld(1, TEST2_3_TEST2R) | P_Fld(0, TEST2_3_TEST1));
    }
    
    // 5
    // read data compare ready check
    DramcEngine2CheckComplete(p, u1status);
    
    // delay 10ns after ready check from DE suggestion (1ms here)
    //mcDELAY_US(1);
    
    u4result = (u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_TESTRPT))>>4) & u1status; //CMP_ERR_RK0/1

    return u4result;
}

//-------------------------------------------------------------------------
/** DramcEngine2
 *  start the self test engine 2 inside dramc to test dram w/r.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param  wr              (DRAM_TE_OP_T): TE operation
 *  @param  test2_1         (U32): 28bits,base address[27:0].
 *  @param  test2_2         (U32): 28bits,offset address[27:0]. (unit is 16-byte, i.e: 0x100 is 0x1000).
 *  @param  loopforever     (S16): 0    read\write one time ,then exit
 *                                >0 enable eingie2, after "loopforever" second ,write log and exit
 *                                -1 loop forever to read\write, every "period" seconds ,check result ,only when we find error,write log and exit
 *                                -2 loop forever to read\write, every "period" seconds ,write log ,only when we find error,write log and exit
 *                                -3 just enable loop forever ,then exit
 *  @param period           (U8):  it is valid only when loopforever <0; period should greater than 0
 *  @param log2loopcount    (U8): test loop number of test agent2 loop number =2^(log2loopcount) ,0 one time
 *  @retval status          (U32): return the value of DM_CMP_ERR  ,0  is ok ,others mean  error
 */
//-------------------------------------------------------------------------
static U32 uiReg0D0h;
DRAM_STATUS_T DramcEngine2Init(DRAMC_CTX_T *p, U32 test2_1, U32 test2_2, U8 testaudpat, U8 log2loopcount)
{
    U8 Use_Len1_Flag;

    // error handling
    if (!p)
    {
        mcSHOW_ERR_MSG(("context is NULL\n"));
        return DRAM_FAIL;
    }

    // check loop number validness
//    if ((log2loopcount > 15) || (log2loopcount < 0))		// U8 >=0 always.
    if (log2loopcount > 15)
    {
        mcSHOW_ERR_MSG(("wrong param: log2loopcount > 15\n"));
        return DRAM_FAIL;
    }

    Use_Len1_Flag = (testaudpat & 0x80) >> 7;
    testaudpat = testaudpat & 0x7f;

    DramcSetRankEngine2(p, p->rank);

    uiReg0D0h=u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD), P_Fld(0, DUMMY_RD_DQSG_DMYRD_EN) | P_Fld(0, DUMMY_RD_DQSG_DMYWR_EN) | P_Fld(0, DUMMY_RD_DUMMY_RD_EN) | P_Fld(0, DUMMY_RD_SREF_DMYRD_EN) | P_Fld(0, DUMMY_RD_DMY_RD_DBG) | P_Fld(0, DUMMY_RD_DMY_WR_DBG));  //must close dummy read when do test agent

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TESTCHIP_DMA1), 0, TESTCHIP_DMA1_DMA_LP4MATAB_OPT);

    // disable self test engine1 and self test engine2
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(0, TEST2_3_TEST2W) | P_Fld(0, TEST2_3_TEST2R) | P_Fld(0, TEST2_3_TEST1));


    // 1.set pattern ,base address ,offset address
    // 2.select  ISI pattern or audio pattern or xtalk pattern
    // 3.set loop number
    // 4.enable read or write
    // 5.loop to check DM_CMP_CPT
    // 6.return CMP_ERR
    // currently only implement ucengine_status = 1, others are left for future extension

    // 1
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_0), P_Fld(test2_1>>24,TEST2_0_TEST2_PAT0)|P_Fld(test2_2>>24,TEST2_0_TEST2_PAT1));

#if (FOR_DV_SIMULATION_USED==1 || SW_CHANGE_FOR_SIMULATION==1)
    //DV sim memory 0~0x100 has values, can't used
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_1), (test2_1+0x10000) & 0x00ffffff, TEST2_1_TEST2_BASE);
#else
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_1), test2_1 & 0x00ffffff, TEST2_1_TEST2_BASE);
#endif
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_2), test2_2 & 0x00ffffff, TEST2_2_TEST2_OFF);

    // 2 & 3
    // (TESTXTALKPAT, TESTAUDPAT) = 00 (ISI), 01 (AUD), 10 (XTALK), 11 (UNKNOWN)
    DramcEngine2SetPat(p, testaudpat, log2loopcount, Use_Len1_Flag);
    return DRAM_OK;
}


U32 DramcEngine2Run(DRAMC_CTX_T *p, DRAM_TE_OP_T wr, U8 testaudpat)
{
    U32 u4result = 0xffffffff;

    // 4
    if (wr == TE_OP_READ_CHECK)
    {
        if ((testaudpat == 1) || (testaudpat == 2))
        {
            //if audio pattern, enable read only (disable write after read), AUDMODE=0x48[15]=0
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), 0, TEST2_4_TESTAUDMODE);
        }

        // enable read, 0x008[31:29]
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(0, TEST2_3_TEST2W) | P_Fld(1, TEST2_3_TEST2R) | P_Fld(0, TEST2_3_TEST1));
    }
    else if (wr == TE_OP_WRITE_READ_CHECK)
    {
        // enable write
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(1, TEST2_3_TEST2W) | P_Fld(0, TEST2_3_TEST2R) | P_Fld(0, TEST2_3_TEST1));
    }
    DramcEngine2Compare(p, wr);

    // delay 10ns after ready check from DE suggestion (1ms here)
    mcDELAY_US(1);

    // 6
    // return CMP_ERR, 0 is ok ,others are fail,diable test2w or test2r
    // get result
    // or all result
    u4result = (u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CMP_ERR)));
    // disable read
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(0, TEST2_3_TEST2W) | P_Fld(0, TEST2_3_TEST2R) | P_Fld(0, TEST2_3_TEST1));

    return u4result;
}

void DramcEngine2End(DRAMC_CTX_T *p)
{
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), P_Fld(0, TEST2_4_TEST_REQ_LEN1));   //test agent 2 with cmd length = 0
    DramcSetRWOFOEN(p, ENABLE);

    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD), uiReg0D0h);
}

// Full set of usage test engine 2, including of DramcEngine2Init->DramcEngine2Run->DramcEngine2End
// if you don't care the performance, and just for convinent, you may use this API (TestEngineCompare)
U32 TestEngineCompare(DRAMC_CTX_T *p)
{
    U8 jj;
    U32 u4err_value;

    if(p->test_pattern <= TEST_XTALK_PATTERN)
    {
        DramcEngine2Init(p, p->test2_1, p->test2_2, p->test_pattern, 0);
        u4err_value = DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, p->test_pattern);
        DramcEngine2End(p);
    }
    else if (p->test_pattern == TEST_MIX_PATTERN)
    {
        DramcEngine2Init(p, p->test2_1, p->test2_2, TEST_AUDIO_PATTERN, 0);
        u4err_value = DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, TEST_AUDIO_PATTERN);
        DramcEngine2End(p);

        DramcEngine2Init(p, p->test2_1, p->test2_2, TEST_XTALK_PATTERN, 0);
        u4err_value |= DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, TEST_XTALK_PATTERN);
        DramcEngine2End(p);
    }
    else
    {
        mcSHOW_ERR_MSG(("Test pattern error! Using default xtalk pattern\n"));
        DramcEngine2Init(p, p->test2_1, p->test2_2, TEST_XTALK_PATTERN, 0);
        u4err_value = DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, TEST_XTALK_PATTERN);
        DramcEngine2End(p);
    }
    return u4err_value;
}


#if (fcFOR_CHIP_ID == fcKrug)
#define EMI_APB_BASE    0x10219000
#endif
U8 u1MaType = 0x2; // for DV sim
void TA2_Test_Run_Time_HW_Set_Column_Num(DRAMC_CTX_T * p)
{
    U8 u1ChannelIdx = 0, shu_index;
    U32 u4matypeR0 = 0, u4matypeR1 = 0;
    U32 u4matype = 0;
    DRAM_CHANNEL_T eOriChannel = p->channel;

    for(u1ChannelIdx = 0; u1ChannelIdx<p->support_channel_num; u1ChannelIdx++)
    {  
        vSetPHY2ChannelMapping(p, u1ChannelIdx);  
        u4matype = u4IO32Read4B(EMI_APB_BASE);
        u4matypeR0 = ((u4matype>>(4+u1ChannelIdx*16))&0x3)+1; //refer to init_ta2_single_channel()
        u4matypeR1 = ((u4matype>>(6+u1ChannelIdx*16))&0x3)+1; //refer to init_ta2_single_channel()
        if(p->support_rank_num==RANK_SINGLE)
        {
            u4matype = u4matypeR0;
        }
        else
        {
            u4matype = (u4matypeR0 > u4matypeR1) ? u4matypeR1 : u4matypeR0; //get min value
        }
        //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_CONF0), u4matype, SHU_CONF0_MATYPE);
    }
    vSetPHY2ChannelMapping(p, eOriChannel);  
    u1MaType = u4matype;

    return;
}

/* ----------------------------------------------------------------------
 * LP4 RG Address
 * bit:   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 9 8 7 6 5 4 3 2 1 0
 *        1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 *        ---------------------------------------------------------------
 * RG:    - - R R R R R R R R R R R R R R R R R|B B B|C C C C C C - - - -
 *            1 1 1 1 1 1 1 9 8 7 6 5 4 3 2 1 0|A A A|9 8 7 6 5 4
 *            6 5 4 3 2 1 0                    |2 1 0|
 * AXI    ---------------------------------------------------------------
 * Addr:  - R R R R R R R R R R R R R R R R|B B B|C C C|C|C C C C C C C -
 *          1 1 1 1 1 1 9 8 7 6 5 4 3 2 1 0|A A A|9 8 7|H|6 5 4 3 2 1 0
 *          5 4 3 2 1 0                    |2 1 0|     |A|
 * ----------------------------------------------------------------------
 * LP3 RG Address of mix mode(R0 X16, R1 X32)
 * bit:   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 9 8 7 6 5 4 3 2 1 0
 *        1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 *        ---------------------------------------------------------------
 * RG:    - R R R R R R R R R R R R R R R R|B B B|C C C C C C C C - - - -
 *          1 1 1 1 1 1 9 8 7 6 5 4 3 2 1 0|A A A|9 8 7 6 5 4 3 2
 *          5 4 3 2 1 0                    |2 1 0|
 * AXI    ---------------------------------------------------------------
 * Addr:  R R R R R R R R R R R R R R R R|B B B|C C C C C C C C C C C - -
 * R0     1 1 1 1 1 1 9 8 7 6 5 4 3 2 1 0|A A A|1 9 8 7 6 5 4 3 2 1 0
 *        5 4 3 2 1 0                    |2 1 0|0  
 * AXI    ---------------------------------------------------------------
 * Addr:  - R R R R R R R R R R R R R R R R|B B B|C C C C C C C C C C - -
 * R1       1 1 1 1 1 1 9 8 7 6 5 4 3 2 1 0|A A A|9 8 7 6 5 4 3 2 1 0
 *          5 4 3 2 1 0                    |2 1 0|
 * ----------------------------------------------------------------------
 */
#define TA2_RANK0_ADDRESS   (0x40200000)
void TA2_Test_Run_Time_HW_Presetting(DRAMC_CTX_T * p, U32 len, TA2_RKSEL_TYPE_T rksel_mode)
{
    DRAM_CHANNEL_T eOriChannel = p->channel;
    U32 u4BaseR0, u4BaseR1, u4Offset, u4Addr;
    U32 u4matypeR0, u4matypeR1;
    U8 u1ChannelIdx, uiRWOFOEN;

    u4Addr = TA2_RANK0_ADDRESS & 0x1fffffff;
    u4BaseR0 = (((u4Addr&~0x1ff)>>2) | (u4Addr&0xff)>>1) >> 4;// drop bit8, bit0
    u4Offset = len>>(1+5);//1:ch_a/b field, 5:0x20 bytes coverage per pattern
    u4BaseR1 = u4BaseR0;

#if !FOR_DV_SIMULATION_USED
    u4matypeR0 = ((u4IO32Read4B(EMI_APB_BASE)>>4)&0x3)+1; 
    u4matypeR1 = ((u4IO32Read4B(EMI_APB_BASE)>>6)&0x3)+1; 
#else
    u4matypeR0 = u4matypeR1 = u1MaType;
#endif

    if(u4matypeR0 != u4matypeR1)//R0 R1 mix mode
    {
        (u4matypeR0>u4matypeR1) ? (u4BaseR0>>=1):(u4BaseR1>>=1);//set the correct start address, refer to mapping table
        u4Offset>>=1;//set min value 
    }

    u4Offset = (u4Offset==0) ? 1 : u4Offset;  //halt if u4Offset = 0
    
#if ENABLE_EMI_LPBK_TEST && EMI_USE_TA2
    if (gEmiLpbkTest)
    {
        u4matypeR0 = 2;
        u4matypeR1 = 2;
        u4Offset = 3;
    }
#endif

    if (TA2_RKSEL_XRT == rksel_mode)
    {
        // In order to enhance XRT R2R/W2W probability, use TEST2_4_TESTAGENTRKSEL=3, PERFCTL0_RWOFOEN=0 mode
        uiRWOFOEN = 0;
        mcSHOW_DBG_MSG(("=== TA2 XRT R2R/W2W\n"));
    }
    else
    {
        uiRWOFOEN = 1;
#if !ENABLE_EMI_LPBK_TEST
        mcSHOW_DBG_MSG(("=== TA2 HW\n"));
#endif
    }
	
    //u4Offset = 0x400;
    for(u1ChannelIdx = 0; u1ChannelIdx<p->support_channel_num; u1ChannelIdx++)
    {  
        p->channel = u1ChannelIdx;
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_1), u4BaseR0, TEST2_1_TEST2_BASE);//fill based on RG table for Rank 0
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_5), u4BaseR1, TEST2_5_TEST2_BASE_2);//fill based on RG table for Rank 1
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_2), u4Offset, TEST2_2_TEST2_OFF);//128 bits data length per offest
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), rksel_mode, TEST2_4_TESTAGENTRKSEL);
        DramcSetRWOFOEN(p, uiRWOFOEN);
    }
    p->channel = eOriChannel;

    //TA2_Test_Run_Time_HW_Set_Column_Num(p);

    return;
}

#define TA2_PAT TEST_XTALK_PATTERN
void TA2_Test_Run_Time_Pat_Setting(DRAMC_CTX_T *p, U8 PatSwitch)
{
    static U8 u1Pat = TA2_PAT, u1loop = 1;
    U8 u1ChannelIdx = 0, u1RankNum=0;
    DRAM_CHANNEL_T eOriChannel = p->channel;
    

    if (u1loop||PatSwitch==TA2_PAT_SWITCH_ON)
    {
        mcSHOW_DBG_MSG(("TA2 PAT: %s\n",(u1Pat==TEST_XTALK_PATTERN)?"XTALK":(u1Pat==TEST_AUDIO_PATTERN)?"AUDIO":"ISI"));
        for (u1ChannelIdx=CHANNEL_A; u1ChannelIdx<p->support_channel_num; u1ChannelIdx++)
        {
            p->channel = u1ChannelIdx;
            
            u1RankNum = p->support_rank_num - 1;
#if ENABLE_EMI_LPBK_TEST && EMI_USE_TA2
            if (gEmiLpbkTest)
            {
                u1RankNum = EMI_LPBK_K_RK1==1?1:0; //RK0 test only, speed up test
            }
#endif
            DramcEngine2SetPat(p, u1Pat, u1RankNum, 0);


        }
        p->channel = eOriChannel;
        if (PatSwitch)
            u1Pat = (u1Pat+1)%3;
        else
            u1loop = 0;
    }
    return;
}

void TA2_Test_Run_Time_HW_Write(DRAMC_CTX_T * p, U8 u1Enable)
{
    DRAM_CHANNEL_T eOriChannel = p->channel;
    U8 u1ChannelIdx;

    for(u1ChannelIdx =0; u1ChannelIdx<p->support_channel_num; u1ChannelIdx++)
    {
        p->channel = u1ChannelIdx;
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), u1Enable, TEST2_3_TEST2W);
    }
    p->channel = eOriChannel;
    return;
}

U32 TA2_Test_Run_Time_HW_Status(DRAMC_CTX_T * p)
{
    U8 u1ChannelIdx = 0;
    U8 u1RankIdx = 0;
    U32 u4loop_count = 0;
    U32 u4ErrRegField = 0;
    U32 u4ErrorValue = 0;
    U32 bit_error = 0;
    static U32 err_count = 0;
    static U32 pass_count = 0;
    DRAM_CHANNEL_T eOriChannel = p->channel;

    for(u1ChannelIdx =0; u1ChannelIdx<p->support_channel_num; u1ChannelIdx++)
    {  
#if ENABLE_EMI_LPBK_TEST && EMI_USE_TA2
        if (gEmiLpbkTest && (u1ChannelIdx != p->channel))
        {
            continue;
        }
#endif

        p->channel = u1ChannelIdx;
        u4ErrorValue = DramcEngine2Compare(p, TE_OP_WRITE_READ_CHECK);
        if (u4ErrorValue&0x3) //RK0 or RK1 test fail
        {
#if ENABLE_EMI_LPBK_TEST
            if (gEmiLpbkTest!=1)
#endif
            {
                mcSHOW_DBG_MSG(("=== HW channel(%d) u4ErrorValue: 0x%x, bit error: 0x%x\n",u1ChannelIdx,u4ErrorValue,u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CMP_ERR))));
            }

#if defined(SLT)
#if ENABLE_EMI_LPBK_TEST
            if (gEmiLpbkTest==0)
#endif
            while(1);
#endif            
        }
        
        for(u1RankIdx =0 ; u1RankIdx<p->support_rank_num; u1RankIdx++)
        {
            if(u4ErrorValue & (1<<u1RankIdx))
            {
                err_count++;
                
#if ENABLE_EMI_LPBK_TEST
                if (gEmiLpbkTest!=1)
#endif
                {
                    mcSHOW_TIME_MSG(("HW channel(%d) Rank(%d), TA2 fail, pass_cnt:%d, err_cnt:%d\n", u1ChannelIdx, u1RankIdx, pass_count, err_count));
                    mcSHOW_DBG_MSG(("HW channel(%d) Rank(%d), TA2 fail, pass_cnt:%d, err_cnt:%d\n", u1ChannelIdx, u1RankIdx, pass_count, err_count));
                }
            }
            else
            {
                pass_count++;
                
#if ENABLE_EMI_LPBK_TEST
                if (gEmiLpbkTest!=1)
#endif
                {
                    mcSHOW_TIME_MSG(("HW channel(%d) Rank(%d), TA2 pass, pass_cnt:%d, err_cnt:%d\n", u1ChannelIdx, u1RankIdx, pass_count, err_count));
                    mcSHOW_DBG_MSG(("HW channel(%d) Rank(%d), TA2 pass, pass_cnt:%d, err_cnt:%d\n", u1ChannelIdx, u1RankIdx, pass_count, err_count));
                }
            }

            
        }
#if EMI_USE_TA2
        bit_error = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CMP_ERR));
#endif
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(0, TEST2_3_TEST2W) | P_Fld(0, TEST2_3_TEST2R) | P_Fld(0, TEST2_3_TEST1));
    }

    p->channel = eOriChannel;

    return bit_error;
}

void TA2_Test_Run_Time_HW(DRAMC_CTX_T * p)
{
    DRAM_CHANNEL_T channel_bak = p->channel;
    DRAM_RANK_T rank_bak = p->rank;

    TA2_Test_Run_Time_HW_Presetting(p, 0x10000, TA2_RKSEL_HW);  //TEST2_2_TEST2_OFF = 0x400
    TA2_Test_Run_Time_Pat_Setting(p, TA2_PAT_SWITCH_OFF);
    TA2_Test_Run_Time_HW_Write(p, ENABLE);
    //mcDELAY_MS(1);
    TA2_Test_Run_Time_HW_Status(p);

    p->channel = channel_bak;
    p->rank = rank_bak;
    return;
}

U8 *DramcFetchGlobalMR(U8 mr_idx)
{
    U8 *pMRGlobalValue = NULL;
    switch (mr_idx)
    {
        case 13:    pMRGlobalValue = &u1MR13Value;    break;
        default:    pMRGlobalValue = NULL;            break;
    }

    return pMRGlobalValue;
}

void DramcMRWriteFldMsk(DRAMC_CTX_T *p, U8 mr_idx, U8 listVal8, U8 msk8, U8 UpdateMode) 
{                                                                           
    U8 *pMRGlobalValue = DramcFetchGlobalMR(mr_idx);                     
                                                                            
    *pMRGlobalValue = ((*pMRGlobalValue & ~msk8) | listVal8);               

    if (UpdateMode == TO_MR)
        DramcModeRegWriteByRank(p, p->rank, mr_idx, *pMRGlobalValue);
}

void DramcMRWriteFldAlign(DRAMC_CTX_T *p, U8 mr_idx, U8 value, U32 mr_fld, U8 UpdateMode)
{
    U8 *pMRGlobalValue = DramcFetchGlobalMR(mr_idx);

    *pMRGlobalValue &= ~(Fld2Msk32(mr_fld));
    *pMRGlobalValue |= (value << Fld_shft(mr_fld));

    if (UpdateMode == TO_MR)
        DramcModeRegWriteByRank(p, p->rank, mr_idx, *pMRGlobalValue);
}

void DramcModeRegRead(DRAMC_CTX_T *p, U8 u1MRIdx, U16 *u2pValue)
{
    U32 u4MRValue;
#ifdef DUMP_INIT_RG_LOG_TO_DE
    gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag=0;
#endif
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), p->rank, MRS_MRRRK);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1MRIdx, MRS_MRSMA);

    // MRR command will be fired when MRREN 0->1
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1, SPCMD_MRREN);

    // wait MRR command fired.
    while(u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_MRR_RESPONSE) ==0)
    {
        mcDELAY_US(1);
    }

    // Since LP3 does not support CG condition, LP3 can not use MRR_STATUS_MRR_SW_REG to do sw mrr.
    // After fix HW CG condition, LP3 will use MRR_STATUS_MRR_SW_REG to do sw mrr.
    U32 u4MRRReg;
    u4MRRReg = MRR_STATUS_MRR_SW_REG;

    // Read out mode register value
    u4MRValue = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRR_STATUS), u4MRRReg);
    *u2pValue = (U16)u4MRValue;

    // Set MRREN =0 for next time MRR.
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_MRREN);
    
#ifdef DUMP_INIT_RG_LOG_TO_DE
    gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag=1;
#endif

    mcSHOW_DBG_MSG3(("Read MR%d =0x%x\n", u1MRIdx, u4MRValue));
}


void DramcModeRegReadByRank(DRAMC_CTX_T *p, U8 u1Rank, U8 u1MRIdx, U16 *u2pValue)
{
    U16 u2Value = 0;
    U8 u1RankBak;
        
#ifdef DUMP_INIT_RG_LOG_TO_DE
    gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag=0;
#endif
    
    /* Since Cann_on/Sylv_ia, TMRRI design changed (2 kinds of modes depending on value of R_DMRK_SCINPUT_OPT) 
     * DE: Jouling, Berson
     * To specify SW_MRR rank -> new mode(scinput_opt == 0): MRSRK
     *                           old mode(scinput_opt == 1): MRRRK
     * Note: MPCRK is not used by SW to control rank anymore
     */
#if ENABLE_TMRRI_NEW_MODE
    //Backup & set rank
    u1RankBak = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), MRS_MRSRK); //backup rank
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1Rank, MRS_MRSRK); //set rank

    //Mode reg read
    DramcModeRegRead(p, u1MRIdx, &u2Value);
    *u2pValue = u2Value;
#ifdef DUMP_INIT_RG_LOG_TO_DE
    gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag=0;
#endif

    //Restore rank
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1RankBak, MRS_MRSRK);
#else
    //Backup & set rank
    u1RankBak = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), MRS_MRRRK); //backup rank
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1Rank, MRS_MRRRK); //set rank

    //Mode reg read
    DramcModeRegRead(p, u1MRIdx, &u2Value);
    *u2pValue = u2Value;
#ifdef DUMP_INIT_RG_LOG_TO_DE
    gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag=0;
#endif

    //Restore rank
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1RankBak, MRS_MRRRK);
#endif
        
#ifdef DUMP_INIT_RG_LOG_TO_DE
    gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag=1;
#endif
}


void DramcModeRegWrite(DRAMC_CTX_T *p, U8 u1MRIdx, U8 u1Value)
{
    U32 counter=0;
    U8 u1Rank = 0;
    U32 u4register_024;
        
#ifdef DUMP_INIT_RG_LOG_TO_DE
    gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag=0;
#endif

    //backup register of CKE fix on/off
    u4register_024 = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL));

    u1Rank = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), MRS_MRSRK);

    //CKE must be fix on when doing MRW
    CKEFixOnOff(p, u1Rank, CKE_FIXON, CKE_WRITE_TO_ONE_CHANNEL);
    
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), p->rank, MRS_MRSRK);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1MRIdx, MRS_MRSMA);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1Value, MRS_MRSOP);

    // MRW command will be fired when MRWEN 0->1
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1, SPCMD_MRWEN);

    // wait MRW command fired.
    while(u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_MRW_RESPONSE) ==0)
    {
        counter++;
        mcSHOW_DBG_MSG2(("wait MRW command Rank%d MR%d =0x%x fired (%d)\n", u1Rank, u1MRIdx, u1Value, counter));
        mcDELAY_US(1);
    }

    // Set MRWEN =0 for next time MRW.
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_MRWEN);
    
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), u4register_024); //restore CKEFIXON value 
    
    #if MRW_CHECK_ONLY
    u1PrintModeRegWrite = 1;
    if (u1MRIdx==13)
    {
        u2MRRecord[p->channel][u1Rank][FSP_0][13] =u1Value;
        u2MRRecord[p->channel][u1Rank][FSP_1][13] =u1Value;
    }
    else
        u2MRRecord[p->channel][u1Rank][p->dram_fsp][u1MRIdx] =u1Value;
    #endif 
    
    if(u1PrintModeRegWrite)
    {
        #if VENDER_JV_LOG
        mcSHOW_JV_LOG_MSG(("Write Rank%d MR%d =0x%x\n", u1Rank, u1MRIdx, u1Value));
        #endif
        #if MRW_CHECK_ONLY
        mcSHOW_MRW_MSG(("MRW CH%d Rank%d FSP%d MR%d =0x%x\n", p->channel, u1Rank, p->dram_fsp, u1MRIdx, u1Value));
        #endif
        mcSHOW_DBG_MSG2(("Write Rank%d MR%d =0x%x\n", u1Rank, u1MRIdx, u1Value));
        mcDUMP_REG_MSG(("Write Rank%d MR%d =0x%x\n", u1Rank, u1MRIdx, u1Value));
        mcFPRINTF((fp_A60501, "Write Rank%d MR%d =0x%x\n", u1Rank, u1MRIdx, u1Value));
    }
        
#ifdef DUMP_INIT_RG_LOG_TO_DE
    gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag=1;
#endif
}

void DramcModeRegWriteByRank(DRAMC_CTX_T *p, U8 u1Rank, U8 u1MRIdx, U8 u1Value)
{
    U32 u4RabnkBackup;
        
#ifdef DUMP_INIT_RG_LOG_TO_DE
    gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag=0;
#endif

    // backup rank
    u4RabnkBackup = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), MRS_MRSRK);

    // set rank
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1Rank, MRS_MRSRK);

    // write mode register 
    DramcModeRegWrite(p,u1MRIdx, u1Value);
#ifdef DUMP_INIT_RG_LOG_TO_DE
    gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag=0;
#endif

    // restore rank
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u4RabnkBackup, MRS_MRSRK);
        
#ifdef DUMP_INIT_RG_LOG_TO_DE
    gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag=1;
#endif
}

U32 u4gpRegBackupVlaue[64];
void DramcBackupRegisters(DRAMC_CTX_T *p, U32 *backup_addr, U32 backup_num)
{
    U32 u4RegIdx;

#if __ETT__
    if (backup_num>64)
    {
        mcSHOW_ERR_MSG(("[DramcBackupRegisters] backup number over 64!!!\n"));
        while(1);
    }
#endif

    for(u4RegIdx=0; u4RegIdx < backup_num; u4RegIdx++)
    {
        u4gpRegBackupVlaue[u4RegIdx] = u4IO32Read4B(backup_addr[u4RegIdx]);
        //mcSHOW_DBG_MSG(("Backup Reg(0x%X) = 0x%X\n", backup_addr[u4RegIdx], u4gpRegBackupVlaue[u4RegIdx]));
    }
}

void DramcRestoreRegisters(DRAMC_CTX_T *p, U32 *restore_addr, U32 restore_num)
{
    U32 u4RegIdx;

    for(u4RegIdx=0; u4RegIdx < restore_num; u4RegIdx++)
    {
        vIO32Write4B(restore_addr[u4RegIdx], u4gpRegBackupVlaue[u4RegIdx]);
        //mcSHOW_DBG_MSG(("Restore Reg(0x%X) = 0x%X\n", restore_addr[u4RegIdx], u4gpRegBackupVlaue[u4RegIdx]));
    }
}


U8 u1GetMR4RefreshRate(DRAMC_CTX_T *p, DRAM_CHANNEL_T channel)
{
    U8 u1Dummy, u1RefreshRate;

    //vSetPHY2ChannelMapping(p, channel);

    //DramcModeRegRead(p, 4, &u1Dummy);
    //mcSHOW_DBG_MSG(("[u2GetRefreshRate] MR4 0x%x,  u1RefreshRate= 0x%x\n", u1Dummy, u1RefreshRate));
    u1RefreshRate = (U8)u4IO32ReadFldAlign((DRAMC_REG_MISC_STATUSA)+(channel<<POS_BANK_NUM), MISC_STATUSA_REFRESH_RATE);
    //mcSHOW_DBG_MSG(("[u2GetRefreshRate] channel = %d, u1RefreshRate= 0x%x\n", channel, u1RefreshRate));

    return u1RefreshRate;
}


#if 1
//#ifdef DDR_INIT_TIME_PROFILING
void DramcConfInfraReset(DRAMC_CTX_T *p)
{
#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
#if (fcFOR_CHIP_ID == fcKrug)
        // 26M
        vIO32WriteFldMulti_All(DDRPHY_CKMUX_SEL, P_Fld(0x1, CKMUX_SEL_R_PHYCTRLMUX)  //move CKMUX_SEL_R_PHYCTRLMUX to here (it was originally between MISC_CG_CTRL0_CLK_MEM_SEL and MISC_CTRL0_R_DMRDSEL_DIV2_OPT)
                                    | P_Fld(0x1, CKMUX_SEL_R_PHYCTRLDCM)); // PHYCTRLDCM 1: follow DDRPHY_conf DCM settings, 0: follow infra DCM settings
        vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL0, P_Fld(0, MISC_CG_CTRL0_W_CHG_MEM)
                    | P_Fld(0, MISC_CG_CTRL0_CLK_MEM_SEL));//[5:4] mem_ck mux: 2'b00: 26MHz, [0]: change memory clock
        vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_W_CHG_MEM);//change clock freq
        mcDELAY_US(1);
        vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_W_CHG_MEM);//disable memory clock change

        // dramc conf reset
        //mcSHOW_TIME_MSG(("Before infra reset, 0x10001148:%x\n", *(volatile unsigned *)(0x10001148)));
        *(volatile unsigned *)(0x10001140) = (0x1 << 15);
        //mcSHOW_TIME_MSG(("After infra reset, 0x10001148:%x\n",  *(volatile unsigned *)(0x10001148)));
        __asm__ __volatile__ ("dsb" : : : "memory");
        mcDELAY_US(200);
        //mcSHOW_TIME_MSG(("Before infra clear, 0x10001148:%x\n",  *(volatile unsigned *)(0x10001148)));
        *(volatile unsigned *)(0x10001144) = (0x1 << 15); 
        //mcSHOW_TIME_MSG(("After infra clear, 0x10001148:%x\n",  *(volatile unsigned *)(0x10001148)));

        #if 0
        mcDELAY_US(200);
        *(volatile unsigned *)(0x10007018) = 0x88000040;
        mcDELAY_US(200);
        *(volatile unsigned *)(0x10007018) = 0x88000000;
        mcDELAY_US(200);
        #endif      

        //DDRPHY Reset
        vIO32WriteFldAlign_All(DDRPHY_B0_DQ3, 0x0, B0_DQ3_RG_ARDQ_RESETB_B0);
        vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI0, 0x0, B0_DLL_ARPI0_RG_ARPI_RESETB_B0);
        vIO32WriteFldAlign_All(DDRPHY_B1_DQ3, 0x0, B1_DQ3_RG_ARDQ_RESETB_B1);
        vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI0, 0x0, B1_DLL_ARPI0_RG_ARPI_RESETB_B1);
        vIO32WriteFldAlign_All(DDRPHY_CA_CMD3, 0x0, CA_CMD3_RG_ARCMD_RESETB);
        vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI0, 0x0, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);
        vIO32WriteFldAlign(DDRPHY_PLL4, 0x0, PLL4_RG_RPHYPLL_RESETB);//Since there is only 1 PLL, only control CHA
        mcDELAY_US(200);
        vIO32WriteFldAlign_All(DDRPHY_B0_DQ3, 0x1, B0_DQ3_RG_ARDQ_RESETB_B0);
        vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI0, 0x1, B0_DLL_ARPI0_RG_ARPI_RESETB_B0);
        vIO32WriteFldAlign_All(DDRPHY_B1_DQ3, 0x1, B1_DQ3_RG_ARDQ_RESETB_B1);
        vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI0, 0x1, B1_DLL_ARPI0_RG_ARPI_RESETB_B1);
        vIO32WriteFldAlign_All(DDRPHY_CA_CMD3, 0x1, CA_CMD3_RG_ARCMD_RESETB);
        vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI0, 0x1, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);
        vIO32WriteFldAlign(DDRPHY_PLL4, 0x1, PLL4_RG_RPHYPLL_RESETB);//Since there is only 1 PLL, only control CHA

        //Disable SPM control
        vIO32WriteFldMulti(SPM_POWERON_CONFIG_EN, P_Fld(0xB16, POWERON_CONFIG_EN_PROJECT_CODE) | P_Fld(0, POWERON_CONFIG_EN_BCLK_CG_EN));

        //For FMeter after dcm enable
        vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL2,
                P_Fld(0x0, MISC_CG_CTRL2_RG_MEM_DCM_DCM_EN) |
                P_Fld(0x1, MISC_CG_CTRL2_RG_MEM_DCM_FORCE_ON) |
                P_Fld(0x0, MISC_CG_CTRL2_RG_MEM_DCM_APB_TOG)); //toggle MEM_DCM_APB_TOG 0->1->0 to let RG setting take effect
        vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL2, 0x1, MISC_CG_CTRL2_RG_MEM_DCM_APB_TOG);
        vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL2, 0x0, MISC_CG_CTRL2_RG_MEM_DCM_APB_TOG);
#endif
#endif
}
#endif

#if defined(DDR_INIT_TIME_PROFILING) || ENABLE_APB_MASK_WRITE
U32 l_low_tick0, l_high_tick0, l_low_tick1, l_high_tick1;
void TimeProfileBegin(void)
{
#if __ETT__
    l_low_tick0 = GPT_GetTickCount(&l_high_tick0);
#else
    l_low_tick0 = get_timer(0);
#endif
}

U32 TimeProfileEnd(void)
{
#if __ETT__
    l_low_tick1 = GPT_GetTickCount(&l_high_tick1);

    //mcSHOW_TIME_MSG(("Time0 %u %u\n", l_high_tick0, l_low_tick0));
    //mcSHOW_TIME_MSG(("Time1 %u %u\n", l_high_tick1, l_low_tick1));
    return ((l_low_tick1-l_low_tick0)*76)/1000;
#else
    l_low_tick1 = get_timer(l_low_tick0);
    return l_low_tick1 * 1000;
#endif
}
#endif




