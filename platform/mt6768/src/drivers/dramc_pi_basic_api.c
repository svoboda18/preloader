/* ----------------------------------------------------------------------------*
 * Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software")
 * are protected under international and related jurisdictions'copyright laws
 * as unpublished works. The information contained herein is confidential and
 * proprietary to MediaTek Inc. Without the prior written permission of
 * MediaTek Inc., any reproduction, modification, use or disclosure of
 * MediaTek Software, and information contained herein, in whole or in part,
 * shall be strictly prohibited.
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND
 * AGREES TO THE FOLLOWING:
 *
 * 1)Any and all intellectual property rights (including without
 * limitation, patent, copyright, and trade secrets) in and to this
 * Software/firmware and related documentation ("MediaTek Software") shall
 * remain the exclusive property of MediaTek Inc. Any and all intellectual
 * property rights (including without limitation, patent, copyright, and
 * trade secrets) in and to any modifications and derivatives to MediaTek
 * Software, whoever made, shall also remain the exclusive property of
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any
 * title to any intellectual property right in MediaTek Software to Receiver.
 *
 * 2)This MediaTek Software Receiver received from MediaTek Inc. and/or its
 * representatives is provided to Receiver on an "AS IS" basis only.
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,
 * including but not limited to any implied warranties of merchantability,
 * non-infringement and fitness for a particular purpose and any warranties
 * arising out of course of performance, course of dealing or usage of trade.
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the
 * software of any third party which may be used by, incorporated in, or
 * supplied with the MediaTek Software, and Receiver agrees to look only to
 * such third parties for any warranty claim relating thereto.  Receiver
 * expressly acknowledges that it is Receiver's sole responsibility to obtain
 * from any third party all proper licenses contained in or delivered with
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software
 * releases made to Receiver's specifications or to conform to a particular
 * standard or open forum.
 *
 * 3)Receiver further acknowledge that Receiver may, either presently
 * and/or in the future, instruct MediaTek Inc. to assist it in the
 * development and the implementation, in accordance with Receiver's designs,
 * of certain softwares relating to Receiver's product(s) (the "Services").
 * Except as may be otherwise agreed to in writing, no warranties of any
 * kind, whether express or implied, are given by MediaTek Inc. with respect
 * to the Services provided, and the Services are provided on an "AS IS"
 * basis. Receiver further acknowledges that the Services may contain errors
 * that testing is important and it is solely responsible for fully testing
 * the Services and/or derivatives thereof before they are used, sublicensed
 * or distributed. Should there be any third party action brought against
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties
 * mutually agree to enter into or continue a business relationship or other
 * arrangement, the terms and conditions set forth herein shall remain
 * effective and, unless explicitly stated otherwise, shall prevail in the
 * event of a conflict in the terms in any agreements entered into between
 * the parties.
 *
 * 4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and
 * cumulative liability with respect to MediaTek Software released hereunder
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the
 * MediaTek Software at issue.
 *
 * 5)The transaction contemplated hereunder shall be construed in
 * accordance with the laws of Singapore, excluding its conflict of laws
 * principles.  Any disputes, controversies or claims arising thereof and
 * related thereto shall be settled via arbitration in Singapore, under the
 * then current rules of the International Chamber of Commerce (ICC).  The
 * arbitration shall be conducted in English. The awards of the arbitration
 * shall be final and binding upon both parties and shall be entered and
 * enforceable in any court of competent jurisdiction.
*---------------------------------------------------------------------------
 */
/* -----------------------------------------------------------------------------
 *
 * $Author: jc.wu $
 * $Date: 2012/6/5 $
 * $RCSfile: pi_basic_api.c,v $
 * $Revision: #5 $
 *
*---------------------------------------------------------------------------
 */

/* * @file pi_basic_api.c
 * Basic DRAMC API implementation
 */

/*
 * -----------------------------------------------------------------------------
 * Include files
 * -----------------------------------------------------------------------------
 * #include "..\Common\pd_common.h"
 * #include "Register.h"
 */
#if __ETT__
#include "processor.h"
#endif
#include "dramc_common.h"
#include "x_hal_io.h"
#if (FOR_DV_SIMULATION_USED == 0)
#include "pll.h"
#endif

#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
#ifndef MT6768_FPGA
#include <pmic.h>
#endif
#endif

/*
 * #include "DramC_reg.h"
 * #include "System_reg.h"
 * #include "string.h"
 * #define mcFPRINTF(_x_)            do{}while(0)
 */

/*
 * -----------------------------------------------------------------------------
 * Global variables
 * -----------------------------------------------------------------------------
 */
#if REG_ACCESS_PORTING_DGB
extern U8 RegLogEnable;
#endif
U8 u1PrintModeRegWrite = 0;

#if FOR_DV_SIMULATION_USED
U8 u1BroadcastOnOff = 0;
extern U8 gu1BroadcastIsLP4;
#endif

#if TX_DQ_PRE_EMPHASIS
extern U32 gDramcSwImpedanceResule[2][4]; /* ODT_ON / OFF x DRVP / DRVN / ODTP / ODTN */
#endif

#if (fcFOR_PINMUX == fcTalbot)
//CA APHY->DRAM
#if (CA_PER_BIT_DELAY_CELL || PINMUX_AUTO_TEST_PER_BIT_CA)
const U8 uiLPDDR4_CA_Mapping_POP[CHANNEL_NUM][6] =
{
	/* CH-A */
	{
		0, 3, 4, 2, 1, 5
	},
	/* CH-B */
	{
		0, 4, 2, 3, 1, 5
	}
};

#if ENABLE_LP3_SW
const U8 uiLPDDR3_CA_Mapping_POP_eMCP[10] = {
	0, 1, 5, 8, 4, 9, 6, 2, 7, 3,
};

const U8 uiLPDDR3_CA_Mapping_POP_DSC[10] = {
	0, 1, 9, 5, 8, 7, 6, 2, 3, 4,
};

#endif
#endif

/* MRR DRAM->DRAMC */
#if ENABLE_LP3_SW
const U8 uiLPDDR3_MRR_Mapping_POP[8] =
{
	0, 1, 2, 3, 4, 5, 6, 7
};

const U8 uiLPDDR3_MRR_Mapping_POP_DLP3[8] =
{
	5, 1, 0, 2, 6, 3, 7, 4
};
#endif
const U8 uiLPDDR4_MRR_Mapping_POP[CHANNEL_NUM][16] =
{
	/* CH-A */
	{
		0, 1, 7, 6, 3, 5, 4, 2,
		9, 8, 13, 12, 11, 15, 14, 10
	},
	/* CH-B */
	{
		0, 1, 5, 6, 3, 7, 4, 2,
		9, 8, 12, 13, 11, 15, 14, 10
	}
};

/* O1 DRAM->DRAMC */
#if ENABLE_LP3_SW
const U8 uiLPDDR3_O1_Mapping_POP[32] =
{
	0, 1, 2, 3, 4, 5, 6, 7,
	8, 10, 9, 11, 12, 13, 14, 15,
	16, 17, 18, 23, 19, 21, 22, 20,
	24, 25, 26, 27, 28, 29, 30, 31
};

const U8 uiLPDDR3_O1_Mapping_POP_DLP3[32] =
{
	5, 1, 0, 2, 6, 3, 7, 4,
	11, 8, 9, 10, 14, 15, 12, 13,
	16, 18, 17, 22, 20, 23, 21, 19,
	24, 25, 28, 30, 27, 26, 31, 29
};
#endif /* ENABLE_LP3_SW */
const U8 uiLPDDR4_O1_Mapping_POP[CHANNEL_NUM][16] =
{
	/* CH-A */
	{
		0, 1, 7, 6, 3, 5, 4, 2,
		9, 8, 13, 12, 11, 15, 14, 10
	},
	/* CH-B */
	{
		0, 1, 5, 6, 3, 7, 4, 2,
		9, 8, 12, 13, 11, 15, 14, 10
	}
};

#if 0/* (FOR_DV_SIMULATION_USED != 0) */
#if ENABLE_LP3_SW
const U8 uiLPDDR3_O1_Mapping_POP[32] =
{
	20, 22, 21, 23, 19, 17, 16, 18,
	26, 24, 29, 30, 31, 27, 28, 25,
	10, 8, 15, 14, 11, 9, 12, 13,
	0, 2, 5, 6, 4, 1, 3, 7
};
#endif /* ENABLE_LP3_SW */

const U8 uiLPDDR4_O1_Mapping_POP[CHANNEL_NUM][16] =
{
	/* CH-A */
	{
		0, 1, 7, 6, 3, 5, 4, 2,
		9, 8, 13, 12, 11, 15, 14, 10
	},
	/* CH-B */
	{
		0, 1, 5, 6, 3, 7, 4, 2,
		9, 8, 12, 13, 11, 15, 14, 10
	}
};
#endif
#endif /* fcFOR_PINMUX */

U32 gu4TermFreq = 0;

#if 0
/* {runtime, MR23, MR1819} */
const U16 TX3200_Runtime_tDQS2DQ[3][31] =
{
	{ 40, 4, 100},
	{ 50, 5, 114},
	{ 60, 6, 125},
	{ 70, 7, 135},
	{ 80, 8, 143},
	{ 90, 9, 150},
	{100, 10, 156},
	{120, 12, 176},
	{130, 13, 181},
	{150, 15, 197},
	{160, 16, 200},
	{180, 18, 214},
	{200, 20, 227},
	{220, 22, 239},
	{240, 24, 250},
	{260, 26, 260},
	{280, 28, 269},
	{300, 30, 278},
	{320, 32, 286},
	{340, 34, 293},
	{370, 37, 308},
	{390, 39, 315},
	{420, 42, 328},
	{450, 45, 341},
	{470, 47, 346},
	{500, 50, 357},
	{530, 53, 368},
	{560, 56, 378},
	{590, 59, 388},
	{620, 62, 397},
	{660, 66, 413}
};

const U16 TX2667_Runtime_tDQS2DQ[3][31] =
{
	{ 36, 3, 90},
	{ 36, 3, 82},
	{ 48, 4, 100},
	{ 60, 5, 115},
	{ 72, 6, 129},
	{ 72, 6, 120},
	{ 84, 7, 131},
	{ 96, 8, 141},
	{108, 9, 150},
	{120, 10, 158},
	{132, 11, 165},
	{156, 13, 186},
	{168, 14, 191},
	{180, 15, 196},
	{192, 16, 200},
	{216, 18, 216},
	{228, 19, 219},
	{252, 21, 233},
	{264, 22, 236},
	{288, 24, 248},
	{312, 26, 260},
	{324, 27, 261},
	{348, 29, 272},
	{372, 31, 282},
	{396, 33, 291},
	{420, 35, 300},
	{444, 37, 308},
	{468, 39, 316},
	{492, 41, 324},
	{516, 43, 331},
	{552, 46, 345}
};

const U16 TX1600_Runtime_tDQS2DQ[3][31] =
{
	{ 20, 1, 50},
	{ 20, 1, 45},
	{ 20, 1, 42},
	{ 40, 2, 77},
	{ 40, 2, 71},
	{ 40, 2, 67},
	{ 60, 3, 94},
	{ 60, 3, 88},
	{ 60, 3, 83},
	{ 80, 4, 105},
	{ 80, 4, 100},
	{100, 5, 119},
	{100, 5, 114},
	{100, 5, 109},
	{120, 6, 125},
	{120, 6, 120},
	{140, 7, 135},
	{140, 7, 130},
	{160, 8, 143},
	{180, 9, 155},
	{180, 9, 150},
	{200, 10, 161},
	{200, 10, 156},
	{220, 11, 167},
	{240, 12, 176},
	{260, 13, 186},
	{260, 13, 181},
	{280, 14, 189},
	{300, 15, 197},
	{320, 16, 205},
	{320, 16, 200}
};
#endif

#if ENABLE_RODT_TRACKING_SAVE_MCK
/* global variables for RODT tracking & ROEN */
U8 u1ODT_ON;        /* infor of p->odt_onoff */
U8 u1WDQS_ON = 0;   /* infor of WDQS on(ROEN = 1) */
U8 u1RODT_TRACK = 0;  /* infor of rodt tracking enable */
U8 u1ROEN, u1ModeSel; /* status of ROEN, MODESEL setting */
#endif
U8 _MappingFreqArray[LP3_DDR800 + 1] = {0};



/*
 * ACTiming struct declaration (declared here due Fld_wid for each register type)
 * Should include all fields from ACTiming excel file (And update the correct values in UpdateACTimingReg()
 * Note: DQSINCTL, DATLAT aren't in ACTiming excel file (internal delay parameters)
 */
typedef struct _ACTime_T
{
	U8 dramType, cbtMode, readDBI;
	U16 freq;
	U8 readLat, writeLat;
	U8 dqsinctl, datlat; /* DQSINCTL, DATLAT aren't in ACTiming excel file */

	/* DRAMC_REG_SHU_ACTIM0 =================================== */
	U16 trcd :Fld_wid(SHU_ACTIM0_TRCD);
	U16 trrd :Fld_wid(SHU_ACTIM0_TRRD);
	U16 twr :Fld_wid(SHU_ACTIM0_TWR);
	U16 twtr :Fld_wid(SHU_ACTIM0_TWTR);

	/* DRAMC_REG_SHU_ACTIM1 =================================== */
	U16 trc :Fld_wid(SHU_ACTIM1_TRC);
	U16 tras :Fld_wid(SHU_ACTIM1_TRAS);
	U16 trp :Fld_wid(SHU_ACTIM1_TRP);
	U16 trpab :Fld_wid(SHU_ACTIM1_TRPAB);

	/* DRAMC_REG_SHU_ACTIM2 =================================== */
	U16 tfaw :Fld_wid(SHU_ACTIM2_TFAW);

	U16 trtw_ODT_off :Fld_wid(SHU_ACTIM2_TR2W);
	U16 trtw_ODT_on :Fld_wid(SHU_ACTIM2_TR2W);

	U16 trtp :Fld_wid(SHU_ACTIM2_TRTP);
	U16 txp :Fld_wid(SHU_ACTIM2_TXP);

	/* DRAMC_REG_SHU_ACTIM3 =================================== */
	U16 refcnt :Fld_wid(SHU_ACTIM3_REFCNT);
	U16 trfc :Fld_wid(SHU_ACTIM3_TRFC);
	U16 trfcpb :Fld_wid(SHU_ACTIM3_TRFCPB);

	/* DRAMC_REG_SHU_ACTIM4 =================================== */
	U16 tzqcs :Fld_wid(SHU_ACTIM4_TZQCS);
	U16 refcnt_fr_clk :Fld_wid(SHU_ACTIM4_REFCNT_FR_CLK);
	U16 txrefcnt :Fld_wid(SHU_ACTIM4_TXREFCNT);

	/* DRAMC_REG_SHU_ACTIM5 =================================== */
	U16 tmrr2w_ODT_off :Fld_wid(SHU_ACTIM5_TMRR2W);
	U16 tmrr2w_ODT_on :Fld_wid(SHU_ACTIM5_TMRR2W);

	U16 twtpd :Fld_wid(SHU_ACTIM5_TWTPD);
	U16 trtpd:Fld_wid(SHU_ACTIM5_TR2PD);

#if 0 /* Currently not used */
	/* DRAMC_REG_SHU_ACTIM6 */
	U16 :Fld_wid(SHU_ACTIM6_BGTRRD);
	U16 :Fld_wid(SHU_ACTIM6_TWRMPR);
	U16 :Fld_wid(SHU_ACTIM6_BGTWTR);
	U16 :Fld_wid(SHU_ACTIM6_BGTCCD);
#endif
	/* DRAMC_REG_SHU_ACTIM_XRT =================================== */
	U16 xrtw2w :Fld_wid(SHU_ACTIM_XRT_XRTW2W);
	U16 xrtw2r :Fld_wid(SHU_ACTIM_XRT_XRTW2R);
	U16 xrtr2w :Fld_wid(SHU_ACTIM_XRT_XRTR2W);
	U16 xrtr2r :Fld_wid(SHU_ACTIM_XRT_XRTR2R);

	/* DRAMC_REG_SHU_AC_TIME_05T =================================== */
#if 0 /* Currently not used */
	U16 :Fld_wid(SHU_AC_TIME_05T_XRTW2R_M05T);
	U16 :Fld_wid(SHU_AC_TIME_05T_XRTR2W_05T);
#endif
	U16 twtr_05T :Fld_wid(SHU_AC_TIME_05T_TWTR_M05T);
	U16 trtw_ODT_off_05T :Fld_wid(SHU_AC_TIME_05T_TR2W_05T);
	U16 trtw_ODT_on_05T :Fld_wid(SHU_AC_TIME_05T_TR2W_05T);
#if 0 /* Currently not used */
	U16 :Fld_wid(SHU_AC_TIME_05T_BGTWTR_05T);
	U16 :Fld_wid(SHU_AC_TIME_05T_BGTCCD_05T);
	U16 :Fld_wid(SHU_AC_TIME_05T_BGTRRD_05T);
#endif

	U16 twtpd_05T :Fld_wid(SHU_AC_TIME_05T_TWTPD_M05T);
	U16 trtpd_05T :Fld_wid(SHU_AC_TIME_05T_TR2PD_05T);
	U16 tfaw_05T :Fld_wid(SHU_AC_TIME_05T_TFAW_05T);
	U16 trrd_05T :Fld_wid(SHU_AC_TIME_05T_TRRD_05T);
	U16 twr_05T :Fld_wid(SHU_AC_TIME_05T_TWR_M05T);
	U16 tras_05T :Fld_wid(SHU_AC_TIME_05T_TRAS_05T);
	U16 trpab_05T :Fld_wid(SHU_AC_TIME_05T_TRPAB_05T);
	U16 trp_05T :Fld_wid(SHU_AC_TIME_05T_TRP_05T);
	U16 trcd_05T :Fld_wid(SHU_AC_TIME_05T_TRCD_05T);
	U16 trtp_05T :Fld_wid(SHU_AC_TIME_05T_TRTP_05T);
	U16 txp_05T :Fld_wid(SHU_AC_TIME_05T_TXP_05T);
	U16 trfc_05T :Fld_wid(SHU_AC_TIME_05T_TRFC_05T);
	U16 trfcpb_05T :Fld_wid(SHU_AC_TIME_05T_TRFCPB_05T);
	U16 trc_05T :Fld_wid(SHU_AC_TIME_05T_TRC_05T);

	/* Other ACTiming reg fields =================================== */
	U16 r_dmcatrain_intv :Fld_wid(CATRAINING1_CATRAIN_INTV);
	U16 r_dmmrw_intv :Fld_wid(SHU_SCINTV_MRW_INTV);
	U16 r_dmfspchg_prdcnt :Fld_wid(SHU_CONF2_FSPCHG_PRDCNT);
	U16 ckeprd :Fld_wid(SHU_CKECTRL_CKEPRD);
	U16 ckelckcnt :Fld_wid(CKECTRL_CKELCKCNT);
	U16 zqlat2 :Fld_wid(SHU_SCINTV_TZQLAT2);

} ACTime_T;

/* ACTimingTbl[] forward declaration */
const ACTime_T ACTimingTbl[TOTAL_AC_TIMING_NUMBER];

void DramcPrintRXDQDQSStatus(DRAMC_CTX_T *p, U8 u1Channel);
void DramcPrintIMPTrackingStatus(DRAMC_CTX_T *p, U8 u1Channel);

static void TxCalibrationMoveDQSByDifferentWL(DRAMC_CTX_T *p);

void vSetShuffleFrequency(DRAMC_CTX_T *p, DRAM_DFS_SHUFFLE_TYPE_T whitch_shuffle, U16 frequency);
void Hynix_Test_Mode(DRAMC_CTX_T *p);
void DramcHWGatingDebugOnOff(DRAMC_CTX_T *p, U8 u1OnOff);
void DramcModeRegWriteByRank(DRAMC_CTX_T *p, U8 u1Rank, U8 u1MRIdx, U8 u1Value);
void DramcBackupRegisters(DRAMC_CTX_T *p, U32 *backup_addr, U32 backup_num);
void DramcRestoreRegisters(DRAMC_CTX_T *p, U32 *restore_addr, U32 restore_num);
void DDRPhyFMeter_Init();

extern U8 gFinalRXVrefDQ[CHANNEL_NUM][RANK_MAX];

#if (FOR_DV_SIMULATION_USED == 1)
extern void broadcast_on(void);
extern void broadcast_off(void);
#endif

#if (FOR_DV_SIMULATION_USED == 0 && SW_CHANGE_FOR_SIMULATION == 0)
extern EMI_SETTINGS *get_emi_setting(void);
#endif
#ifdef FOR_HQA_REPORT_USED
extern U8 gHQALog_flag;
#endif

void DramcBroadcastOnOff(U32 bOnOff)
{
#if (FOR_DV_SIMULATION_USED == 0 && SW_CHANGE_FOR_SIMULATION == 0)
	*((volatile unsigned int *)(DRAMC_WBR)) = bOnOff;
#endif
#if (FOR_DV_SIMULATION_USED == 1)
	if (gu1BroadcastIsLP4 == TRUE)
	{
		if (bOnOff)
		{
			broadcast_on(); mcSHOW_DBG_MSG(("Broadcast ON\n"));
			u1BroadcastOnOff = 1;
		}
		else
		{
			broadcast_off(); mcSHOW_DBG_MSG(("Broadcast OFF\n"));
			u1BroadcastOnOff = 0;
		}
	}
	else /* LP3 */
	{
		{
			broadcast_off(); mcSHOW_DBG_MSG(("Broadcast OFF\n"));
			u1BroadcastOnOff = 0;
		}
	}
#endif

#ifdef DUMP_INIT_RG_LOG_TO_DE
	if (gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag)
	{
		mcSHOW_DUMP_INIT_RG_MSG(("*((UINT32P)(0x%x)) = 0x%x; \n", DRAMC_WBR, bOnOff));
		/* mcDELAY_MS(1); */
		GPT_Delay_ms(1);

	}
#endif

}

U32 GetDramcBroadcast(void)
{
#if (FOR_DV_SIMULATION_USED == 0)
	return *((volatile unsigned int *)(DRAMC_WBR));
#endif
#if (FOR_DV_SIMULATION_USED == 1)
	return u1BroadcastOnOff;
#endif
}

U8 u1IsLP4Family(DRAM_DRAM_TYPE_T dram_type)
{
	if (dram_type == TYPE_LPDDR4 || dram_type == TYPE_LPDDR4X || dram_type == TYPE_LPDDR4P)
		return 1;
	else
		return 0;
}

/*
 * To support frequencies not on the ACTimingTable, we use read/write latency
 * settings as the next highest freq listed in ACTimingTable. Use this API
 * to set which freq group the target freq belongs to.
 * (Set's DRAMC_CTX_T's p->freqGroup)
 * Currently is designed to be called from DDRPhyFreqSel()
 */
void setFreqGroup(DRAMC_CTX_T *p)
{

	/*
	 * Below listed conditions represent freqs that exist in ACTimingTable
	 * -> Should cover freqGroup settings for all real freq values
	 */
	if (u1IsLP4Family(p->dram_type))
	{
		if (p->frequency <= 800) /* DDR1600, DDR800 */
		{
			p->freqGroup = 800;
		}
		else if (p->frequency <= 1200) /* DDR2400, DDR2280 */
		{
			p->freqGroup = 1200;
		}
		else if (p->frequency <= 1333) /* DDR2667 */
		{
			p->freqGroup = 1333;
		}
		else if (p->frequency <= 1600) /* DDR3200 */
		{
			p->freqGroup = 1600;
		}
		else  /* DDR3733 */
		{
			p->freqGroup = 1866;
		}
	}
	else
	{
		if (p->frequency <= 600) /* DDR1200 */
		{
			p->freqGroup = 600;
		}
		else if (p->frequency <= 667) /* DDR1333 */
		{
			p->freqGroup = 667;
		}
		else if (p->frequency <= 800) /* DDR1600 */
		{
			p->freqGroup = 800;
		}
		else /* DDR1866 */
		{
			p->freqGroup = 933;
		}
	}

	mcSHOW_DBG_MSG3(("[setFreqGroup] p-> frequency %u, freqGroup: %u\n", p->frequency, p->freqGroup));
	return;
}

void Set_MRR_Pinmux_Mapping(DRAMC_CTX_T *p)
{
	U8 *uiLPDDR_MRR_Mapping = NULL;
	U8 backup_broadcast, backup_channel;
	DRAM_CHANNEL_T chIdx = CHANNEL_A;

	/* Backup channel & broadcast */
	backup_channel = vGetPHY2ChannelMapping(p);
	backup_broadcast = GetDramcBroadcast();

	DramcBroadcastOnOff(DRAMC_BROADCAST_OFF); /* Disable broadcast */

	/* LP4: Set each channel's pinmux individually, LP3: Only has 1 channel (support_channel_num == 1) */
	for (chIdx = CHANNEL_A; chIdx < p->support_channel_num; chIdx++)
	{
		vSetPHY2ChannelMapping(p, chIdx);

		if (u1IsLP4Family(p->dram_type))
		{
			uiLPDDR_MRR_Mapping = (U8 *)uiLPDDR4_MRR_Mapping_POP[chIdx];
		}
	#if ENABLE_LP3_SW
		else
		{
		#if (fcFOR_CHIP_ID == fcTalbot)
			if (p->bDLP3)
			{
				uiLPDDR_MRR_Mapping = (U8 *)uiLPDDR3_MRR_Mapping_POP_DLP3;
			}
			else
		#endif
			{
				uiLPDDR_MRR_Mapping = (U8 *)uiLPDDR3_MRR_Mapping_POP;
			}
		}
	#endif

		/* Set MRR pin mux */
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_MRR_BIT_MUX1), P_Fld(uiLPDDR_MRR_Mapping[0], MRR_BIT_MUX1_MRR_BIT0_SEL) | P_Fld(uiLPDDR_MRR_Mapping[1], MRR_BIT_MUX1_MRR_BIT1_SEL) |
			P_Fld(uiLPDDR_MRR_Mapping[2], MRR_BIT_MUX1_MRR_BIT2_SEL) | P_Fld(uiLPDDR_MRR_Mapping[3], MRR_BIT_MUX1_MRR_BIT3_SEL));
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_MRR_BIT_MUX2), P_Fld(uiLPDDR_MRR_Mapping[4], MRR_BIT_MUX2_MRR_BIT4_SEL) | P_Fld(uiLPDDR_MRR_Mapping[5], MRR_BIT_MUX2_MRR_BIT5_SEL) |
			P_Fld(uiLPDDR_MRR_Mapping[6], MRR_BIT_MUX2_MRR_BIT6_SEL) | P_Fld(uiLPDDR_MRR_Mapping[7], MRR_BIT_MUX2_MRR_BIT7_SEL));
		if (u1IsLP4Family(p->dram_type))
		{
			vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_MRR_BIT_MUX3), P_Fld(uiLPDDR_MRR_Mapping[8], MRR_BIT_MUX3_MRR_BIT8_SEL) | P_Fld(uiLPDDR_MRR_Mapping[9], MRR_BIT_MUX3_MRR_BIT9_SEL) |
				P_Fld(uiLPDDR_MRR_Mapping[10], MRR_BIT_MUX3_MRR_BIT10_SEL) | P_Fld(uiLPDDR_MRR_Mapping[11], MRR_BIT_MUX3_MRR_BIT11_SEL));
			vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_MRR_BIT_MUX4), P_Fld(uiLPDDR_MRR_Mapping[12], MRR_BIT_MUX4_MRR_BIT12_SEL) | P_Fld(uiLPDDR_MRR_Mapping[13], MRR_BIT_MUX4_MRR_BIT13_SEL) |
				P_Fld(uiLPDDR_MRR_Mapping[14], MRR_BIT_MUX4_MRR_BIT14_SEL) | P_Fld(uiLPDDR_MRR_Mapping[15], MRR_BIT_MUX4_MRR_BIT15_SEL));
		}
	}

	/* Recover channel & broadcast */
	vSetPHY2ChannelMapping(p, backup_channel);
	DramcBroadcastOnOff(backup_broadcast);
}

static void SetRankInfoToConf(DRAMC_CTX_T *p, U32 u4RankIdx)
{
#if (FOR_DV_SIMULATION_USED == 0 && SW_CHANGE_FOR_SIMULATION == 0)
	vIO32WriteFldAlign(DRAMC_REG_RSTMASK, u4RankIdx, RSTMASK_RSV_DRAM_SUPPORT_RANK_NUM); /* 0: dual rank 1: single rank */
#endif
}

U8 get_shuffleIndex_by_Freq(DRAMC_CTX_T *p)
{
	u8 u1ShuffleIdx = 0;
	DRAM_DFS_FREQUENCY_TABLE_T *pFreqTbl;

	if (u1IsLP4Family(p->dram_type))
	{
		pFreqTbl = gFreqTbl;
	}
	else
	{
		pFreqTbl = gFreqTbl_LP3;
	}

	for (u1ShuffleIdx = DRAM_DFS_SHUFFLE_1; u1ShuffleIdx < DRAM_DFS_SHUFFLE_MAX; u1ShuffleIdx++)
	{
		if (pFreqTbl[u1ShuffleIdx].frequency == p->frequency)
		{
			return _MappingFreqArray[pFreqTbl[u1ShuffleIdx].freq_sel];
		}
	}

	return 0;
}

U16 get_Freq_by_shuffleIndex(DRAMC_CTX_T *p, U8 index)
{
	u8 u1ShuffleIdx = 0;
	DRAM_DFS_FREQUENCY_TABLE_T *pFreqTbl;

	if (u1IsLP4Family(p->dram_type))
	{
		pFreqTbl = gFreqTbl;
	}
	else
	{
		pFreqTbl = gFreqTbl_LP3;
	}

	/* cc add after MP review. This function is currently used by tDQSCK Precalculation.
	 * Here returns read Frequency to improve the accuracy of precalculation.
	 */
#if (fcFOR_CHIP_ID == fcTalbot)
	if (u1IsLP4Family(p->dram_type) && (pFreqTbl[index].frequency == LP4_HIGHEST_FREQ))
		return 1800;
#endif
	return pFreqTbl[index].frequency;
}


void vInitMappingFreqArray(DRAMC_CTX_T *p)
{
	u8 u1ShuffleIdx = 0;
	DRAM_DFS_FREQUENCY_TABLE_T *pFreqTbl;

	if (u1IsLP4Family(p->dram_type))
	{
		pFreqTbl = gFreqTbl;
	}
	else
	{
		pFreqTbl = gFreqTbl_LP3;
	}

	for (u1ShuffleIdx = DRAM_DFS_SHUFFLE_1; u1ShuffleIdx < DRAM_DFS_SHUFFLE_MAX; u1ShuffleIdx++)
	{
		_MappingFreqArray[pFreqTbl[u1ShuffleIdx].freq_sel] = u1ShuffleIdx;
	}
}

void Global_Option_Init(DRAMC_CTX_T *p)
{
	U8 u1ShuffleIdx;

	/* SaveCurrDramCtx(p); */
	vSetChannelNumber(p);
	vInitGlobalVariablesByCondition(p->dram_type);
	vInitMappingFreqArray(p);

#ifdef FOR_HQA_TEST_USED
	HQA_measure_message_reset_all_data(p);
#endif
}

void Global_Option_Init2(DRAMC_CTX_T *p)
{
	U8 u1rank_num = 0;

#if (FOR_DV_SIMULATION_USED == 0 && SW_CHANGE_FOR_SIMULATION == 0)
	EMI_SETTINGS *emi_set;

	emi_set = get_emi_setting();

	mcSHOW_DBG_MSG(("Rank info CONA[0x%x]\n", emi_set->EMI_CONA_VAL));

#if (fcFOR_CHIP_ID == fcTalbot)
	if (emi_set->PIN_MUX_TYPE) {
		p->bDLP3 = 1;
		mcSHOW_DBG_MSG(("\n\nusing DLP3\n\n"));
	}
#endif

	p->vendor_id = emi_set->iLPDDR3_MODE_REG_5;

	u1rank_num = ((emi_set->EMI_CONA_VAL >> 17) & 0x1) == 1 ? 0 : 1;
#else
	u1rank_num = p->support_rank_num; /* cc add for SIM */
#endif
	SetRankInfoToConf(p, u1rank_num);
	vSetRankNumber(p);
	Set_MRR_Pinmux_Mapping(p);
}

void RISCWriteDRAM(unsigned int UI_offset_address, unsigned int UI_content_value)
{
	*((volatile unsigned int *)(Channel_A_DRAMC_AO_BASE_ADDRESS + UI_offset_address)) = UI_content_value;
	*((volatile unsigned int *)(Channel_A_DRAMC_NAO_BASE_ADDRESS + UI_offset_address)) = UI_content_value;
} /* end of RISCWriteDRAM */

void RISCWriteDDRPHY(unsigned int UI_offset_address, unsigned int UI_content_value)
{
	*((volatile unsigned int *)(Channel_A_PHY_AO_BASE_VIRTUAL + UI_offset_address)) = UI_content_value;
} /* end of RISCWriteDDRPHY */

#if (FOR_DV_SIMULATION_USED == 0)
void RISCWrite(unsigned int UI_offset_address, unsigned int UI_content_value)
{
#if (FOR_DV_SIMULATION_USED == 1)   /* DV */
	register_write_c(UI_offset_address, UI_content_value);
#else
	*((volatile unsigned int *)(UI_offset_address)) = UI_content_value;
#endif
} /* end of RISCWrite */

unsigned int RISCRead(unsigned int UI_offset_address)
{
	return *((volatile unsigned int *)(UI_offset_address));
} /* end of RISCWrite */
#endif

#if 0/* __ETT__ */
void RISCReadAll(void)
{
	unsigned int address = DRAMC0_BASE;
	unsigned int value = 0;

	/* TINFO=">>>>>DRAMC0<<<<<\n" */
	mcSHOW_DBG_MSG((" >> > >> DRAMC0 <<  <  << \n"));
	for (address = DRAMC0_BASE; address <= DRAMC0_BASE + 0xD54; address += 4)
	{
		value = *((volatile unsigned int *)(address));
	#ifdef ETT_PRINT_FORMAT
		mcSHOW_DBG_MSG(("Addr. 0x%X = 0x%X\n", address, value));
	#else
		mcSHOW_DBG_MSG(("Addr. 0x%8x = 0x%8x\n", address, value));
	#endif
		/* TINFO="[REG_ACCESS_PORTING_DBG] Register_Dump Reg(%h) = %h\n", address, value */
	}

	/* TINFO=">>>>>DRAMC1<<<<<\n" */
	mcSHOW_DBG_MSG((" >> > >> DRAMC1 <<  <  << \n"));
	for (address = DRAMC1_BASE; address <= DRAMC1_BASE + 0xD54; address += 4)
	{
		value = *((volatile unsigned int *)(address));
	#ifdef ETT_PRINT_FORMAT
		mcSHOW_DBG_MSG(("Addr. 0x%X = 0x%X\n", address, value));
	#else
		mcSHOW_DBG_MSG(("Addr. 0x%8x = 0x%8x\n", address, value));
	#endif
		/* TINFO="[REG_ACCESS_PORTING_DBG] Register_Dump Reg(%h) = %h\n", address, value */
	}

	/* TINFO=">>>>>DRAMC0_NAO<<<<<\n" */
	mcSHOW_DBG_MSG((" >> > >> DRAMC0_NAO <<  <  << \n"));
	for (address = DRAMC0_NAO_BASE; address <= DRAMC0_NAO_BASE + 0x59C; address += 4)
	{
		value = *((volatile unsigned int *)(address));
	#ifdef ETT_PRINT_FORMAT
		mcSHOW_DBG_MSG(("Addr. 0x%X = 0x%X\n", address, value));
	#else
		mcSHOW_DBG_MSG(("Addr. 0x%8x = 0x%8x\n", address, value));
	#endif
		/* TINFO="[REG_ACCESS_PORTING_DBG] Register_Dump Reg(%h) = %h\n", address, value */
	}

	/* TINFO=">>>>>DRAMC1_NAO<<<<<\n" */
	mcSHOW_DBG_MSG((" >> > >> DRAMC1_NAO <<  <  << \n"));
	for (address = DRAMC1_NAO_BASE; address <= DRAMC1_NAO_BASE + 0x59C; address += 4)
	{
		value = *((volatile unsigned int *)(address));
	#ifdef ETT_PRINT_FORMAT
		mcSHOW_DBG_MSG(("Addr. 0x%X = 0x%X\n", address, value));
	#else
		mcSHOW_DBG_MSG(("Addr. 0x%8x = 0x%8x\n", address, value));
	#endif
		/* TINFO="[REG_ACCESS_PORTING_DBG] Register_Dump Reg(%h) = %h\n", address, value */
	}

	/* TINFO=">>>>>DDRPHY_A<<<<<\n" */
	mcSHOW_DBG_MSG((" >> > >> DDRPHY_A <<  <  << \n"));
	for (address = DDRPHY_BASE; address <= DDRPHY_BASE + 0xFD4; address += 4)
	{
		value = *((volatile unsigned int *)(address));
	#ifdef ETT_PRINT_FORMAT
		mcSHOW_DBG_MSG(("Addr. 0x%X = 0x%X\n", address, value));
	#else
		mcSHOW_DBG_MSG(("Addr. 0x%8x = 0x%8x\n", address, value));
	#endif
		/* TINFO="[REG_ACCESS_PORTING_DBG] Register_Dump Reg(%h) = %h\n", address, value */
	}

	/* TINFO=">>>>>DDRPHY_B<<<<<\n" */
	mcSHOW_DBG_MSG((" >> > >> DDRPHY_B <<  <  << \n"));
	for (address = DDRPHY1_BASE; address <= DDRPHY1_BASE + 0xFD4; address += 4)
	{
		value = *((volatile unsigned int *)(address));
	#ifdef ETT_PRINT_FORMAT
		mcSHOW_DBG_MSG(("Addr. 0x%X = 0x%X\n", address, value));
	#else
		mcSHOW_DBG_MSG(("Addr. 0x%8x = 0x%8x\n", address, value));
	#endif
		/* TINFO="[REG_ACCESS_PORTING_DBG] Register_Dump Reg(%h) = %h\n", address, value */
	}

	/* TINFO=">>>>>DDRPHY_C<<<<<\n" */
	mcSHOW_DBG_MSG((" >> > >> DDRPHY_C <<  <  << \n"));
	for (address = DDRPHY2_BASE; address <= DDRPHY2_BASE + 0xFD4; address += 4)
	{
		value = *((volatile unsigned int *)(address));
	#ifdef ETT_PRINT_FORMAT
		mcSHOW_DBG_MSG(("Addr. 0x%X = 0x%X\n", address, value));
	#else
		mcSHOW_DBG_MSG(("Addr. 0x%8x = 0x%8x\n", address, value));
	#endif
		/* TINFO="[REG_ACCESS_PORTING_DBG] Register_Dump Reg(%h) = %h\n", address, value */
	}
	/* TINFO=">>>>>DDRPHY_D<<<<<\n" */
	mcSHOW_DBG_MSG((" >> > >> DDRPHY_D <<  <  << \n"));
	for (address = DDRPHY3_BASE; address <= DDRPHY3_BASE + 0xFD4; address += 4)
	{
		value = *((volatile unsigned int *)(address));
	#ifdef ETT_PRINT_FORMAT
		mcSHOW_DBG_MSG(("Addr. 0x%X = 0x%X\n", address, value));
	#else
		mcSHOW_DBG_MSG(("Addr. 0x%8x = 0x%8x\n", address, value));
	#endif
		/* TINFO="[REG_ACCESS_PORTING_DBG] Register_Dump Reg(%h) = %h\n", address, value */
	}
}
#endif

void AutoRefreshCKEOff(DRAMC_CTX_T *p)
{
	U32 u4backup_broadcast;
#if ENABLE_TMRRI_NEW_MODE
	U8 u1RankIdx;
#endif
	/* backup broadcast on/off */
	u4backup_broadcast = GetDramcBroadcast();

	DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);

	/* if(p->vendor_id==VENDOR_SAMSUNG && p->dram_type==TYPE_LPDDR3) */
	{
		mcSHOW_DBG_MSG(("AutoRefreshCKEOff AutoREF OFF\n"));
		vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL0, 0x1, REFCTRL0_REFDIS);
		mcDELAY_US(3);
		mcSHOW_DBG_MSG(("DDRPhyPLLSetting - CKEOFF\n"));

		CKEFixOnOff(p, CKE_WRITE_TO_ALL_RANK, CKE_FIXOFF, CKE_WRITE_TO_ALL_CHANNEL);

		mcDELAY_US(1);
	}
	/* restore broadcast on/off */
	DramcBroadcastOnOff(u4backup_broadcast);
}

void CKEFixOnOff(DRAMC_CTX_T *p, U8 u1RankIdx, CKE_FIX_OPTION option, CKE_FIX_CHANNEL WriteChannelNUM)
{
	U8 u1CKEOn, u1CKEOff, u1setChannel, u1BackupChannel;

	if (option == CKE_DYNAMIC) /* if CKE is dynamic, set both CKE fix On and Off as 0 */
	{                          /* After CKE FIX on / off, CKE should be returned to dynamic (control by HW) */
		u1CKEOn = u1CKEOff = 0;
	}
	else /* if CKE fix on is set as 1, CKE fix off should also be set as 0; vice versa */
	{
		u1CKEOn = option;
		u1CKEOff = (1 - option);
	}

	if (WriteChannelNUM == CKE_WRITE_TO_ALL_CHANNEL) /* write register to all channel */
	{
	#if ENABLE_TMRRI_NEW_MODE
		if (u1RankIdx == RANK_0)
		{
			vIO32WriteFldMulti_All(DRAMC_REG_CKECTRL, P_Fld(u1CKEOff, CKECTRL_CKEFIXOFF) | P_Fld(u1CKEOn, CKECTRL_CKEFIXON));
		}
		else if (u1RankIdx == RANK_1)
		{
			vIO32WriteFldMulti_All(DRAMC_REG_CKECTRL, P_Fld(u1CKEOff, CKECTRL_CKE1FIXOFF) | P_Fld(u1CKEOn, CKECTRL_CKE1FIXON));
		}
		else if (u1RankIdx == CKE_WRITE_TO_ALL_RANK)
		{
			vIO32WriteFldMulti_All(DRAMC_REG_CKECTRL, P_Fld(u1CKEOff, CKECTRL_CKEFIXOFF) | P_Fld(u1CKEOn, CKECTRL_CKEFIXON));
			if (p->support_rank_num == RANK_DUAL)
			{
				vIO32WriteFldMulti_All(DRAMC_REG_CKECTRL, P_Fld(u1CKEOff, CKECTRL_CKE1FIXOFF) | P_Fld(u1CKEOn, CKECTRL_CKE1FIXON));
			}
		}
	#else
		vIO32WriteFldMulti_All(DRAMC_REG_CKECTRL, P_Fld(u1CKEOff, CKECTRL_CKEFIXOFF) | P_Fld(u1CKEOn, CKECTRL_CKEFIXON));
		if ((u1RankIdx == CKE_WRITE_TO_ALL_RANK) && (p->support_rank_num == RANK_DUAL))
		{
			vIO32WriteFldMulti_All(DRAMC_REG_CKECTRL, P_Fld(u1CKEOff, CKECTRL_CKE1FIXOFF) | P_Fld(u1CKEOn, CKECTRL_CKE1FIXON));
		}
	#endif
	}
	else
	{
	#if ENABLE_TMRRI_NEW_MODE
		if (u1RankIdx == RANK_0)
		{
			vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), P_Fld(u1CKEOff, CKECTRL_CKEFIXOFF) | P_Fld(u1CKEOn, CKECTRL_CKEFIXON));
		}
		else if (u1RankIdx == RANK_1)
		{
			vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), P_Fld(u1CKEOff, CKECTRL_CKE1FIXOFF) | P_Fld(u1CKEOn, CKECTRL_CKE1FIXON));
		}
		else if (u1RankIdx == CKE_WRITE_TO_ALL_RANK)
		{
			vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), P_Fld(u1CKEOff, CKECTRL_CKEFIXOFF) | P_Fld(u1CKEOn, CKECTRL_CKEFIXON));
			if (p->support_rank_num == RANK_DUAL)
			{
				vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), P_Fld(u1CKEOff, CKECTRL_CKE1FIXOFF) | P_Fld(u1CKEOn, CKECTRL_CKE1FIXON));
			}
		}
	#else
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), P_Fld(u1CKEOff, CKECTRL_CKEFIXOFF) | P_Fld(u1CKEOn, CKECTRL_CKEFIXON));
		if ((u1RankIdx == CKE_WRITE_TO_ALL_RANK) && (p->support_rank_num == RANK_DUAL))
		{
			vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), P_Fld(u1CKEOff, CKECTRL_CKE1FIXOFF) | P_Fld(u1CKEOn, CKECTRL_CKE1FIXON));
		}
	#endif
	}
}

/*
 * RxDQSIsiPulseCG() - API for "RX DQS ISI pulse CG function" 0: disable, 1: enable
 * 1. RG_*_RPRE_TOG_EN (16nm APHY): B0_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B0, B1_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B1
 * 2. RG_RX_*RDQS*_DQSSTB_CG_EN*(10nm APHY): B0_DQ8_RG_RX_ARDQS_DQSSTB_CG_EN_B0, B1_DQ8_RG_RX_ARDQS_DQSSTB_CG_EN_B1
 * Supports setting current channel only, add function to set "all channels" in the future
 */
void RxDQSIsiPulseCG(DRAMC_CTX_T *p, U8 u1OnOff)
{
	mcSHOW_DBG_MSG3(("CH%u RX DQS ISI pulse CG: %u (0:disable, 1:enable)\n", u1OnOff));

	/*
	 * LP4: Disable(set to 0) "RX DQS ISI pulse CG function" during the below senarios (must enable(set to 1) when done)
	 * 1. Gating window calibration
	 * 2. Duty related calibration (Justin: prevents DQSI from being kept high after READ burst)
	 * LP3: should always be set to 1
	 */
#if (fcFOR_CHIP_ID == fcTalbot)
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), u1OnOff, B0_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B0);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), u1OnOff, B1_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B1);
#else
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ8), u1OnOff, B0_DQ8_RG_RX_ARDQS_DQSSTB_CG_EN_B0);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ8), u1OnOff, B1_DQ8_RG_RX_ARDQS_DQSSTB_CG_EN_B1);
#endif

	return;
}

void DVFSSettings(DRAMC_CTX_T *p)
{
	U8 u1DVFS_52M_104M_SEL = 1; /* DVFS_SM freq: 0: 52Mhz 1:104Mhz */
	U8 u1DLL_Idle = 0x30;   /* DDR800 */

#if (fcFOR_CHIP_ID == fcTalbot)
	/*
	 * DVFS_SM LP4: dll_idle 90MCK/19.5ns, LP3: 70MCK   (DE: Lynx)
	 * Below values are pre-calculated for each freq and dram type specifically
	 * for 52M DVFS_SM mode (104M should multiply values by 2)
	 * Since this register value is related to dram's operating freq
	 * -> Each freq must use specific pre-calculated value
	 * (Couldn't use formula to calculate this value because floating point numbers are
	 * not supported via preloader)
	 */
	if (u1IsLP4Family(p->dram_type))
	{
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
			case 400:
				u1DLL_Idle = 0x30;
				break;
			default:
				mcSHOW_ERR_MSG(("u1DLL_Idle err!\n"));
				u1DLL_Idle = 0x30; /* Set to a large value, but should check with designer for actual settings */
			#if __ETT__
				while (1);
			#endif
			}
	}
	else
	{
		switch (p->frequency)
		{
			case 933:
				u1DLL_Idle = 0x8;
				break;
			case 800:
				u1DLL_Idle = 0x9;
				break;
			case 600:
				u1DLL_Idle = 0xC;
				break;
			default:
				mcSHOW_ERR_MSG(("u1DLL_Idle err!\n"));
				u1DLL_Idle = 0xC; /* Set to a large value, but should check with designer for actual settings */
			#if __ETT__
				while (1);
			#endif
			}
	}
#endif
	/*
	 * 52M_104M_SEL
	*((volatile unsigned int*)(0x10000104)) |= (0x1 << 14);//104M clock
	 */
#if (fcFOR_CHIP_ID == fcCannon)
	if (u1DVFS_52M_104M_SEL == 1)
		u1DLL_Idle = (u1DLL_Idle*109 / 52); /* If DVFS_SM uses 104M mode, u1DLL_Idle value should multiply by 2 */
#elif (fcFOR_CHIP_ID == fcTalbot)
		u1DLL_Idle = (u1DLL_Idle << u1DVFS_52M_104M_SEL); /* If DVFS_SM uses 104M mode, u1DLL_Idle value should multiply by 2 */
#else
#error No defined u1DLL_Idle for your chip !!!
#endif

	/* design change for LP4 one channel (Ch-B only) */
	vIO32WriteFldAlign_All(DRAMC_REG_SHUCTRL, 0x1, SHUCTRL_DVFS_CHB_SEL_B);

	/* DVFS debug enable - MRR_STATUS2_DVFS_STATE */
	vIO32WriteFldAlign_All(DRAMC_REG_DVFSDLL, 1, DVFSDLL_R_DDRPHY_SHUFFLE_DEBUG_ENABLE);

	vIO32WriteFldAlign_All(DDRPHY_DVFS_EMI_CLK, u1DVFS_52M_104M_SEL, DVFS_EMI_CLK_RG_52M_104M_SEL); /* Set DVFS_SM's clk */
	vIO32WriteFldAlign_All(DRAMC_REG_SHUCTRL2, u1DLL_Idle, SHUCTRL2_R_DLL_IDLE);

#if ENABLE_DVFS_CDC_SYNCHRONIZER_OPTION
	/* CDC option */
	vIO32WriteFldAlign_All(DRAMC_REG_SHUCTRL2, 1, SHUCTRL2_R_DVFS_CDC_OPTION); /* Lewis@20170331: Not set SHUCTRL2_R_DVFS_CDC_OPTION to 1 since it will lead DDR reserve mode fail in DDR2400 and DDR1600 */
	vIO32WriteFldAlign_All(DRAMC_REG_DVFSDLL, 1, DVFSDLL_R_DVFS_SYNC_MODULE_RST_SEL);
	vIO32WriteFldAlign_All(DRAMC_REG_SHUCTRL2, 1, SHUCTRL2_R_CDC_MUX_SEL_OPTION);
#endif

	/* Can_non CDC options */
	vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL0), P_Fld(1, MISC_CTRL0_IMPCAL_CDC_ECO_OPT) | P_Fld(1, MISC_CTRL0_IDLE_DCM_CHB_CDC_ECO_OPT));
	/* DLL_SHUFFLE should be set enable before switch frequency */
	if (u1IsLP4Family(p->dram_type)) { /* for ENABLE_DLL_ALL_SLAVE_MODE */
		vIO32WriteFldAlign_All(DDRPHY_DVFS_EMI_CLK, 1, DVFS_EMI_CLK_RG_DLL_SHUFFLE);
		vIO32WriteFldAlign_All(DRAMC_REG_DVFSDLL, 1, DVFSDLL_RG_DLL_SHUFFLE);
	}
}

#if ENABLE_SSC
void DDRPhyApplySSCSetting(DRAMC_CTX_T * p)
{
	U32 u4Period, u4Delta, u4Delta1;

	u4Period = 0x1b1;

	if (p->frequency == 1866)
	{
		u4Delta = 0x1a3;
		u4Delta1 = 0x1a3;
		//u4DELTA1 = 0xE14;
	}
	else if (p->frequency == 1600)
	{
		u4Delta = 0x174;
		u4Delta1 = 0x174;
		//u4DELTA1 = 0xC1C;
	}
	else if (p->frequency == 1200)
	{
		u4Delta = 0x117;
		u4Delta1 = 0x117;
		//u4DELTA1 = 0x90F;
	}
	else
	{
		return;
	}

	vIO32WriteFldAlign_All(DDRPHY_SHU1_PLL5, 0x1, SHU1_PLL5_RG_RPHYPLL_SDM_FRA_EN);
	vIO32WriteFldAlign_All(DDRPHY_SHU1_PLL7, 0x1, SHU1_PLL5_RG_RPHYPLL_SDM_FRA_EN);

	vIO32WriteFldAlign_All(DDRPHY_SHU1_PLL14, 0x1, SHU1_PLL14_RG_RPHYPLL_SDM_SSC_PH_INIT);
	vIO32WriteFldAlign_All(DDRPHY_SHU1_PLL20, 0x1, SHU1_PLL20_RG_RCLRPLL_SDM_SSC_PH_INIT);

	vIO32WriteFldAlign_All(DDRPHY_SHU1_PLL14, 0x0208, SHU1_PLL14_RG_RPHYPLL_SDM_SSC_PRD);
	vIO32WriteFldAlign_All(DDRPHY_SHU1_PLL20, 0x0208, SHU1_PLL20_RG_RCLRPLL_SDM_SSC_PRD);

	vIO32WriteFldAlign_All(DDRPHY_SHU1_PLL15, u4Delta, SHU1_PLL15_RG_RPHYPLL_SDM_SSC_DELTA);
	vIO32WriteFldAlign_All(DDRPHY_SHU1_PLL21, u4Delta, SHU1_PLL21_RG_RCLRPLL_SDM_SSC_DELTA);

	vIO32WriteFldAlign_All(DDRPHY_SHU1_PLL15, u4Delta1, SHU1_PLL15_RG_RPHYPLL_SDM_SSC_DELTA1);
	vIO32WriteFldAlign_All(DDRPHY_SHU1_PLL21, u4Delta1, SHU1_PLL21_RG_RCLRPLL_SDM_SSC_DELTA1);
}

#endif

void DDRPhyPLLSetting(DRAMC_CTX_T *p)
{
	U8 u1CAP_SEL;
	U8 u1MIDPICAP_SEL;
	U8 u1VTH_SEL;
	U16 u2SDM_PCW = 0;
	U8 u1CA_DLL_Mode[2];
	U8 iChannel = CHANNEL_A;
	U8 u1BRPI_MCTL_EN_CA = 0;
#if ENABLE_TMRRI_NEW_MODE
	U8 u1RankIdx;
#endif

	u1VTH_SEL = 0x2; /* RG_*RPI_MIDPI_VTH_SEL[1:0] is 2 for all freqs */

#if (fcFOR_CHIP_ID == fcTalbot)
	if (p->frequency > 933)
	{
		u1CAP_SEL = 0x0;
	}
	else
	{
		u1CAP_SEL = 0x3;
	}
#endif

	if (p->frequency <= 933)/* Lewis@20161129: Fix DDR1600 S - idle Vcore power is too big problem. */
	{
		u1MIDPICAP_SEL = 0;
	}
	else if (p->frequency <= 1200)
	{
		u1MIDPICAP_SEL = 0x3;
	}
	else if (p->frequency <= 1333)
	{
		u1MIDPICAP_SEL = 0x3;
	}
	else if (p->frequency <= 1600)
	{
		u1MIDPICAP_SEL = 0x2;
	}
	else if (p->frequency <= 1866)
	{
		u1MIDPICAP_SEL = 0x1;
	}
	else/* 4266 */
	{
		u1MIDPICAP_SEL = 0x0;
	}


	vIO32WriteFldAlign_All(DDRPHY_SHU1_PLL4, 0x0, SHU1_PLL4_RG_RPHYPLL_RESERVED);
	vIO32WriteFldAlign_All(DDRPHY_SHU1_PLL6, 0x0, SHU1_PLL6_RG_RCLRPLL_RESERVED);

#if (fcFOR_CHIP_ID == fcTalbot)
	if (u1IsLP4Family(p->dram_type))
	{
	#if DLL_ASYNC_MODE
		u1BRPI_MCTL_EN_CA = 1;
		u1CA_DLL_Mode[CHANNEL_A] = u1CA_DLL_Mode[CHANNEL_B] = DLL_MASTER;
		vIO32WriteFldAlign(DDRPHY_MISC_SHU_OPT + ((U32)CHANNEL_A << POS_BANK_NUM), 1, MISC_SHU_OPT_R_CA_SHU_PHDET_SPM_EN);
		vIO32WriteFldAlign(DDRPHY_MISC_SHU_OPT + SHIFT_TO_CHB_ADDR, 1, MISC_SHU_OPT_R_CA_SHU_PHDET_SPM_EN);
		vIO32WriteFldMulti(DDRPHY_CKMUX_SEL + ((U32)CHANNEL_A << POS_BANK_NUM), P_Fld(0, CKMUX_SEL_FMEM_CK_MUX) | P_Fld(0, CKMUX_SEL_FB_CK_MUX));
		vIO32WriteFldMulti(DDRPHY_CKMUX_SEL + SHIFT_TO_CHB_ADDR, P_Fld(2, CKMUX_SEL_FMEM_CK_MUX) | P_Fld(2, CKMUX_SEL_FB_CK_MUX));
		vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_CMD0, 0x0, SHU1_CA_CMD0_RG_FB_CK_MUX);
	#else
		u1CA_DLL_Mode[CHANNEL_A] = DLL_MASTER;
		u1CA_DLL_Mode[CHANNEL_B] = DLL_SLAVE;
		vIO32WriteFldAlign(DDRPHY_MISC_SHU_OPT + ((U32)CHANNEL_A << POS_BANK_NUM), 1, MISC_SHU_OPT_R_CA_SHU_PHDET_SPM_EN);
		vIO32WriteFldAlign(DDRPHY_MISC_SHU_OPT + SHIFT_TO_CHB_ADDR, 2, MISC_SHU_OPT_R_CA_SHU_PHDET_SPM_EN);
		vIO32WriteFldMulti(DDRPHY_CKMUX_SEL + ((U32)CHANNEL_A << POS_BANK_NUM), P_Fld(1, CKMUX_SEL_FMEM_CK_MUX) | P_Fld(1, CKMUX_SEL_FB_CK_MUX));
		vIO32WriteFldMulti(DDRPHY_CKMUX_SEL + SHIFT_TO_CHB_ADDR, P_Fld(1, CKMUX_SEL_FMEM_CK_MUX) | P_Fld(1, CKMUX_SEL_FB_CK_MUX));
		vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_CMD0, 0x1, SHU1_CA_CMD0_RG_FB_CK_MUX);
	#endif
	}
	else/* LP3 sync mode only */
	{
		u1CA_DLL_Mode[CHANNEL_A] = DLL_MASTER;
		u1CA_DLL_Mode[CHANNEL_B] = DLL_SLAVE;
		vIO32WriteFldAlign(DDRPHY_MISC_SHU_OPT + ((U32)CHANNEL_A << POS_BANK_NUM), 1, MISC_SHU_OPT_R_CA_SHU_PHDET_SPM_EN);
		vIO32WriteFldAlign(DDRPHY_MISC_SHU_OPT + SHIFT_TO_CHB_ADDR, 2, MISC_SHU_OPT_R_CA_SHU_PHDET_SPM_EN);
		vIO32WriteFldMulti(DDRPHY_CKMUX_SEL + ((U32)CHANNEL_A << POS_BANK_NUM), P_Fld(1, CKMUX_SEL_FMEM_CK_MUX) | P_Fld(1, CKMUX_SEL_FB_CK_MUX));
		vIO32WriteFldMulti(DDRPHY_CKMUX_SEL + SHIFT_TO_CHB_ADDR, P_Fld(1, CKMUX_SEL_FMEM_CK_MUX) | P_Fld(1, CKMUX_SEL_FB_CK_MUX));
		vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_CMD0, 0x1, SHU1_CA_CMD0_RG_FB_CK_MUX);
	}
#if ENABLE_DLL_ALL_SLAVE_MODE
	if (p->frequency <= 933)
	{
		u1CA_DLL_Mode[CHANNEL_A] = u1CA_DLL_Mode[CHANNEL_B] = DLL_SLAVE;
	}
#endif

	if (u1CA_DLL_Mode[CHANNEL_A] == DLL_SLAVE)/* All slave mode */
	{
		vIO32WriteFldAlign_All(DRAMC_REG_DVFSDLL, 1, DVFSDLL_R_BYPASS_1ST_DLL_SHU1);
	}
	else
	{
		vIO32WriteFldAlign_All(DRAMC_REG_DVFSDLL, 0, DVFSDLL_R_BYPASS_1ST_DLL_SHU1);
	}

	for (iChannel = CHANNEL_A; iChannel <= CHANNEL_B; iChannel++)
	{
		if (u1CA_DLL_Mode[iChannel] == DLL_MASTER)
		{
			vIO32WriteFldMulti(DDRPHY_SHU1_CA_DLL0 + ((U32)iChannel << POS_BANK_NUM), P_Fld(0x0, SHU1_CA_DLL0_RG_ARDLL_PHDET_OUT_SEL_CA)
				| P_Fld(0x0, SHU1_CA_DLL0_RG_ARDLL_PHDET_IN_SWAP_CA)
				| P_Fld(0x6, SHU1_CA_DLL0_RG_ARDLL_GAIN_CA)
				| P_Fld(0x9, SHU1_CA_DLL0_RG_ARDLL_IDLECNT_CA)
				| P_Fld(0x8, SHU1_CA_DLL0_RG_ARDLL_P_GAIN_CA)
				| P_Fld(0x1, SHU1_CA_DLL0_RG_ARDLL_PHJUMP_EN_CA)
				| P_Fld(0x1, SHU1_CA_DLL0_RG_ARDLL_PHDIV_CA)
				| P_Fld(0x1, SHU1_CA_DLL0_RG_ARDLL_FAST_PSJP_CA));
			vIO32WriteFldMulti(DDRPHY_SHU1_CA_DLL1 + ((U32)iChannel << POS_BANK_NUM), P_Fld(0x1, SHU1_CA_DLL1_RG_ARDLL_PD_CK_SEL_CA) | P_Fld(0x0, SHU1_CA_DLL1_RG_ARDLL_FASTPJ_CK_SEL_CA));
			vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD6 + ((U32)iChannel << POS_BANK_NUM), 1, RG_ARPI_RESERVE_BIT_01_DLL_FAST_PSJP); /* RG_*RPI_RESERVE_CA[1] 1'b1 tracking leaf(slave) */
		}
		else
		{
			vIO32WriteFldMulti(DDRPHY_SHU1_CA_DLL0 + ((U32)iChannel << POS_BANK_NUM), P_Fld(0x1, SHU1_CA_DLL0_RG_ARDLL_PHDET_OUT_SEL_CA)
				| P_Fld(0x1, SHU1_CA_DLL0_RG_ARDLL_PHDET_IN_SWAP_CA)
				| P_Fld(0x7, SHU1_CA_DLL0_RG_ARDLL_GAIN_CA)
				| P_Fld(0x7, SHU1_CA_DLL0_RG_ARDLL_IDLECNT_CA)
				| P_Fld(0x8, SHU1_CA_DLL0_RG_ARDLL_P_GAIN_CA)
				| P_Fld(0x1, SHU1_CA_DLL0_RG_ARDLL_PHJUMP_EN_CA)
				| P_Fld(0x1, SHU1_CA_DLL0_RG_ARDLL_PHDIV_CA)
				| P_Fld(0x0, SHU1_CA_DLL0_RG_ARDLL_FAST_PSJP_CA));
			vIO32WriteFldMulti(DDRPHY_SHU1_CA_DLL1 + ((U32)iChannel << POS_BANK_NUM), P_Fld(0x0, SHU1_CA_DLL1_RG_ARDLL_PD_CK_SEL_CA) | P_Fld(0x1, SHU1_CA_DLL1_RG_ARDLL_FASTPJ_CK_SEL_CA));
			vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD6 + ((U32)iChannel << POS_BANK_NUM), 0, RG_ARPI_RESERVE_BIT_01_DLL_FAST_PSJP); /* RG_*RPI_RESERVE_CA[1] 1'b1 tracking leaf(slave) */
		}
	}
#endif

#if 0/* ENABLE_DLL_ALL_SLAVE_MODE */
	if (p->frequency <= 933) /* Only set DLL to all slave mode if DDR speed <= 1866 (for both LP3 / LP4) */
	{
		vIO32WriteFldAlign_All(DRAMC_REG_DVFSDLL, 1, DVFSDLL_R_BYPASS_1ST_DLL_SHU1);

		vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_DLL1, 0, SHU1_CA_DLL1_RG_ARDLL_PD_CK_SEL_CA);
		vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_DLL0, 1, SHU1_CA_DLL0_RG_ARDLL_PHDET_OUT_SEL_CA);
		vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_DLL0, 1, SHU1_CA_DLL0_RG_ARDLL_PHDET_IN_SWAP_CA);
		vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_DLL1, 1, SHU1_CA_DLL1_RG_ARDLL_FASTPJ_CK_SEL_CA);
		vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_DLL0, 0x7, SHU1_CA_DLL0_RG_ARDLL_GAIN_CA);
		vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_DLL0, 0x7, SHU1_CA_DLL0_RG_ARDLL_IDLECNT_CA);
	#if (fcFOR_CHIP_ID == fcCervino)
		/* 6763 need to set RG_ARPI_RESERVE_CA[1], and not RG_*RDLL_FAST_PSJP_CA */
		vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_CMD6, 0, RG_ARPI_RESERVE_BIT_01_DLL_FAST_PSJP); /* RG_*RPI_RESERVE_CA[1] 1'b1 tracking leaf(slave) */
	#else
		vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_DLL0, 0, SHU1_CA_DLL0_RG_ARDLL_FAST_PSJP_CA);
	#endif
	}
	else
	{
		vIO32WriteFldAlign_All(DRAMC_REG_DVFSDLL, 0, DVFSDLL_R_BYPASS_1ST_DLL_SHU1);
	#if 0 /* master setting already init at dram init, no need to set here for saving code size. */
		vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_DLL1, 1, SHU1_CA_DLL1_RG_ARDLL_PD_CK_SEL_CA);
		vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_DLL0, 0, SHU1_CA_DLL0_RG_ARDLL_PHDET_OUT_SEL_CA);
		vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_DLL0, 0, SHU1_CA_DLL0_RG_ARDLL_PHDET_IN_SWAP_CA);
		vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_DLL1, 0, SHU1_CA_DLL1_RG_ARDLL_FASTPJ_CK_SEL_CA);
		vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_DLL0, 0x6, SHU1_CA_DLL0_RG_ARDLL_GAIN_CA);
		vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_DLL0, 0x9, SHU1_CA_DLL0_RG_ARDLL_IDLECNT_CA);
		/*
		 * 6763 need to set RG_ARPI_RESERVE_CA[1], and not RG_*RDLL_FAST_PSJP_CA
		 * vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD6, 1, SHU1_CA_CMD6_RG_ARPI_RESERVE_CA_BIT_01);
		 */
		vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_DLL0, 1, SHU1_CA_DLL0_RG_ARDLL_FAST_PSJP_CA);
	#endif
	}
#endif /* ENABLE_DLL_ALL_SLAVE_MODE */

	U32 u4RegBackupAddress[] =
	{
		(DDRPHY_B0_DQ7),
		(DDRPHY_B1_DQ7),
		(DDRPHY_CA_CMD7),
		(DDRPHY_B0_DQ7 + SHIFT_TO_CHB_ADDR),
		(DDRPHY_B1_DQ7 + SHIFT_TO_CHB_ADDR),
		(DDRPHY_CA_CMD7 + SHIFT_TO_CHB_ADDR),
	};

	/* if(p->vendor_id==VENDOR_SAMSUNG && p->dram_type==TYPE_LPDDR3) */
	{
	#if 0
		mcSHOW_DBG_MSG(("DDRPhyPLLSetting - DMSUS\n"));
		vIO32WriteFldAlign_All(DDRPHY_MISC_SPM_CTRL0, 0x0, MISC_SPM_CTRL0_PHY_SPM_CTL0);
		vIO32WriteFldAlign_All(DDRPHY_MISC_SPM_CTRL2, 0x0, MISC_SPM_CTRL2_PHY_SPM_CTL2);
		vIO32WriteFldMulti_All(DDRPHY_MISC_SPM_CTRL1, P_Fld(0x1, MISC_SPM_CTRL1_RG_ARDMSUS_10) | P_Fld(0x1, MISC_SPM_CTRL1_RG_ARDMSUS_10_B0)
			| P_Fld(0x1, MISC_SPM_CTRL1_RG_ARDMSUS_10_B1) | P_Fld(0x1, MISC_SPM_CTRL1_RG_ARDMSUS_10_CA));
	#else
		DramcBackupRegisters(p, u4RegBackupAddress, sizeof (u4RegBackupAddress) / sizeof (U32));
		vIO32WriteFldMulti_All(DDRPHY_B0_DQ7, P_Fld(0x1, B0_DQ7_RG_TX_ARDQ_PULL_DN_B0) | P_Fld(0x1, B0_DQ7_RG_TX_ARDQM0_PULL_DN_B0)
			| P_Fld(0x1, B0_DQ7_RG_TX_ARDQS0_PULL_DN_B0) | P_Fld(0x1, B0_DQ7_RG_TX_ARDQS0B_PULL_DN_B0));
		vIO32WriteFldMulti_All(DDRPHY_B1_DQ7, P_Fld(0x1, B1_DQ7_RG_TX_ARDQ_PULL_DN_B1) | P_Fld(0x1, B1_DQ7_RG_TX_ARDQM0_PULL_DN_B1)
			| P_Fld(0x1, B1_DQ7_RG_TX_ARDQS0_PULL_DN_B1) | P_Fld(0x1, B1_DQ7_RG_TX_ARDQS0B_PULL_DN_B1));
		vIO32WriteFldMulti_All(DDRPHY_CA_CMD7, P_Fld(0x1, CA_CMD7_RG_TX_ARCMD_PULL_DN) | P_Fld(0x1, CA_CMD7_RG_TX_ARCS_PULL_DN)
			| P_Fld(0x1, CA_CMD7_RG_TX_ARCLK_PULL_DN) | P_Fld(0x1, CA_CMD7_RG_TX_ARCLKB_PULL_DN));

		/*
		 * DMSUS replaced by CA_CMD2_RG_TX_ARCMD_OE_DIS, CMD_OE_DIS(1) will prevent illegal command ouput
		 * And DRAM 1st reset_n pulse will disappear if use CA_CMD2_RG_TX_ARCMD_OE_DIS
		 */
		vIO32WriteFldAlign_All(DDRPHY_CA_CMD2, 1, CA_CMD2_RG_TX_ARCMD_OE_DIS);
	#endif
	}

	/* 26M */
	vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0x0, MISC_CG_CTRL0_CLK_MEM_SEL);

#ifdef USE_CLK26M
	vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0x1, MISC_CG_CTRL0_RG_DA_RREF_CK_SEL);
#endif


	/* MIDPI */
	vIO32WriteFldMulti_All(DDRPHY_SHU1_B0_DQ6, P_Fld(0x0, SHU1_B0_DQ6_RG_ARPI_MIDPI_EN_B0)
		| P_Fld(0x0, SHU1_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0));
	vIO32WriteFldMulti_All(DDRPHY_SHU1_B1_DQ6, P_Fld(0x0, SHU1_B1_DQ6_RG_ARPI_MIDPI_EN_B1)
		| P_Fld(0x0, SHU1_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1));
	vIO32WriteFldMulti_All(DDRPHY_SHU1_CA_CMD6, P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA)
		| P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA));

	vIO32WriteFldMulti_All(DDRPHY_PLL4, P_Fld(0x0, PLL4_RG_RPHYPLL_ADA_MCK8X_EN)
		| P_Fld(0x0, PLL4_RG_RPHYPLL_RESETB));

	/* PLL */
	vIO32WriteFldAlign_All(DDRPHY_PLL1, 0x0, PLL1_RG_RPHYPLL_EN);
	vIO32WriteFldAlign_All(DDRPHY_PLL2, 0x0, PLL2_RG_RCLRPLL_EN);

	/* DLL */
	vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI2, 0x0, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);
	vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI2, 0x0, B0_DLL_ARPI2_RG_ARDLL_PHDET_EN_B0);
	vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI2, 0x0, B1_DLL_ARPI2_RG_ARDLL_PHDET_EN_B1);

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

	/* RESETB */
	vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI0, 0x0, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);
	vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI0, 0x0, B0_DLL_ARPI0_RG_ARPI_RESETB_B0);
	vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI0, 0x0, B1_DLL_ARPI0_RG_ARPI_RESETB_B1);

	mcDELAY_US(1);

#if ENABLE_SSC
	DDRPhyApplySSCSetting(p);
#endif
	/*
	 * TODO: PLL/MIDPI Settings
	 * Ref clock should be 20M~30M, if MPLL=52M, Pre-divider should be set to 1
	 */
#ifdef USE_CLK26M
	vIO32WriteFldMulti_All(DDRPHY_SHU1_PLL8, P_Fld(0x0, SHU1_PLL8_RG_RPHYPLL_POSDIV) | P_Fld(0x0, SHU1_PLL8_RG_RPHYPLL_PREDIV));
	vIO32WriteFldMulti_All(DDRPHY_SHU1_PLL10, P_Fld(0x0, SHU1_PLL10_RG_RCLRPLL_POSDIV) | P_Fld(0x0, SHU1_PLL10_RG_RCLRPLL_PREDIV));
#else /* MPLL 52M */
	vIO32WriteFldMulti_All(DDRPHY_SHU1_PLL8, P_Fld(0x0, SHU1_PLL8_RG_RPHYPLL_POSDIV) | P_Fld(0x1, SHU1_PLL8_RG_RPHYPLL_PREDIV));
	vIO32WriteFldMulti_All(DDRPHY_SHU1_PLL10, P_Fld(0x0, SHU1_PLL10_RG_RCLRPLL_POSDIV) | P_Fld(0x1, SHU1_PLL10_RG_RCLRPLL_PREDIV));
#endif
	if (p->frequency == 1866)
	{   /* 3733 / 26 = 143(0x8f) */
	#ifdef EN_DDR3733
		u2SDM_PCW = 0x8f00;
	#else
		u2SDM_PCW = 0x8a00; /* 3600 FREQ. Reuse 3733 ACTiming */
	#endif
	}
	else if (p->frequency == 1600 || p->frequency == 800 || p->frequency == 400)
	{
		if (p->frequency == 800)
			u2SDM_PCW = 0x7600;
		else
			u2SDM_PCW = 0x7b00;
	#if EMI_LPBK_DRAM_USED == 0
	#if EMI_LPBK_USE_DDR_800
		if (p->frequency == 800)
		{
			vIO32WriteFldAlign_All(DDRPHY_SHU1_PLL8, 0x1, SHU1_PLL8_RG_RPHYPLL_POSDIV);
			vIO32WriteFldAlign_All(DDRPHY_SHU1_PLL10, 0x1, SHU1_PLL10_RG_RCLRPLL_POSDIV);
		}
	#endif
	#endif

		if (p->frequency == 400)
		{
			vIO32WriteFldAlign_All(DDRPHY_SHU1_PLL8, 0x1, SHU1_PLL8_RG_RPHYPLL_POSDIV);
			vIO32WriteFldAlign_All(DDRPHY_SHU1_PLL10, 0x1, SHU1_PLL10_RG_RCLRPLL_POSDIV);
		}
	}
	else if (p->frequency == 1333 || p->frequency == 667)
	{
		u2SDM_PCW = 0x6600;
	}
	else if (p->frequency == 1200)
	{
		u2SDM_PCW = 0x5c00;
	}
	else if (p->frequency == 1140)
	{
		u2SDM_PCW = 0x5700;
	}
#if ENABLE_LP3_SW
	/* LP3 */
	else if (p->frequency == 933)
	{
		u2SDM_PCW = 0x8f00;
	}
	else if (p->frequency == 600)
	{
		u2SDM_PCW = 0x5c00;
	}
	else if (p->frequency == 467)
	{
		u2SDM_PCW = 0x4700;
	}
#endif

	/*
	 * SDM_PCW: Feedback divide ratio (8-bit integer + 8-bit fraction)
	 * PLL_SDM_FRA_EN: SDMPLL fractional mode enable (0:Integer mode, 1:Fractional mode)
	 */
	vIO32WriteFldMulti_All(DDRPHY_SHU1_PLL5, P_Fld(u2SDM_PCW, SHU1_PLL5_RG_RPHYPLL_SDM_PCW)
		| P_Fld(0x0, SHU1_PLL5_RG_RPHYPLL_SDM_FRA_EN)); /* Disable fractional mode */
	vIO32WriteFldMulti_All(DDRPHY_SHU1_PLL7, P_Fld(u2SDM_PCW, SHU1_PLL7_RG_RCLRPLL_SDM_PCW)
		| P_Fld(0x0, SHU1_PLL7_RG_RCLRPLL_SDM_FRA_EN)); /* Disable fractional mode */


	vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI0, 1, CA_DLL_ARPI0_RG_ARMCTLPLL_CK_SEL_CA);
	vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI0, 1, B0_DLL_ARPI0_RG_ARMCTLPLL_CK_SEL_B0);
	vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI0, 1, B1_DLL_ARPI0_RG_ARMCTLPLL_CK_SEL_B1);
	vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI1, 0, CA_DLL_ARPI1_RG_ARPI_CLKIEN_JUMP_EN);
	vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI1, 0, B0_DLL_ARPI1_RG_ARPI_MCTL_JUMP_EN_B0);
	vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI1, 0, B1_DLL_ARPI1_RG_ARPI_MCTL_JUMP_EN_B1);


	vIO32WriteFldMulti_All(DDRPHY_SHU1_B0_DQ6, P_Fld(u1VTH_SEL, SHU1_B0_DQ6_RG_ARPI_MIDPI_VTH_SEL_B0)
		| P_Fld(u1CAP_SEL, SHU1_B0_DQ6_RG_ARPI_CAP_SEL_B0)
		| P_Fld(u1MIDPICAP_SEL, SHU1_B0_DQ6_RG_ARPI_MIDPI_CAP_SEL_B0));
	vIO32WriteFldMulti_All(DDRPHY_SHU1_B1_DQ6, P_Fld(u1VTH_SEL, SHU1_B1_DQ6_RG_ARPI_MIDPI_VTH_SEL_B1)
		| P_Fld(u1CAP_SEL, SHU1_B1_DQ6_RG_ARPI_CAP_SEL_B1)
		| P_Fld(u1MIDPICAP_SEL, SHU1_B1_DQ6_RG_ARPI_MIDPI_CAP_SEL_B1));
	vIO32WriteFldMulti_All(DDRPHY_SHU1_CA_CMD6, P_Fld(u1VTH_SEL, SHU1_CA_CMD6_RG_ARPI_MIDPI_VTH_SEL_CA)
		| P_Fld(u1CAP_SEL, SHU1_CA_CMD6_RG_ARPI_CAP_SEL_CA)
		| P_Fld(u1MIDPICAP_SEL, SHU1_CA_CMD6_RG_ARPI_MIDPI_CAP_SEL_CA));


	/* TODO: RESETB */
	vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI0, 0x1, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);
	vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI0, 0x1, B0_DLL_ARPI0_RG_ARPI_RESETB_B0);
	vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI0, 0x1, B1_DLL_ARPI0_RG_ARPI_RESETB_B1);
	mcDELAY_US(1);

	/* TODO: PLL EN */
	vIO32WriteFldAlign_All(DDRPHY_PLL1, 0x1, PLL1_RG_RPHYPLL_EN);
	vIO32WriteFldAlign_All(DDRPHY_PLL2, 0x1, PLL2_RG_RCLRPLL_EN);
	mcDELAY_US(100);

	/* TODO: MIDPI Init 1 */
	vIO32WriteFldMulti_All(DDRPHY_PLL4, P_Fld(0x1, PLL4_RG_RPHYPLL_ADA_MCK8X_EN)
		| P_Fld(0x1, PLL4_RG_RPHYPLL_RESETB));

	mcDELAY_US(1);


	/* TODO: MIDPI Init 2 */
	/*
	 * MIDPI Settings (Oly_mpus): DA_*RPI_MIDPI_EN, DA_*RPI_MIDPI_CKDIV4_EN
	 * Justin suggests use frequency > 933 as boundary
	 */
	if (p->frequency > 933)
	{
		vIO32WriteFldMulti_All(DDRPHY_SHU1_B0_DQ6, P_Fld(0x1, SHU1_B0_DQ6_RG_ARPI_MIDPI_EN_B0)
			| P_Fld(0x0, SHU1_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0));
		vIO32WriteFldMulti_All(DDRPHY_SHU1_B1_DQ6, P_Fld(0x1, SHU1_B1_DQ6_RG_ARPI_MIDPI_EN_B1)
			| P_Fld(0x0, SHU1_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1));
		vIO32WriteFldMulti_All(DDRPHY_SHU1_CA_CMD6, P_Fld(0x1, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA)
			| P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA));
	}
	else
	{
		vIO32WriteFldMulti_All(DDRPHY_SHU1_B0_DQ6, P_Fld(0x0, SHU1_B0_DQ6_RG_ARPI_MIDPI_EN_B0)
			| P_Fld(0x1, SHU1_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0));
		vIO32WriteFldMulti_All(DDRPHY_SHU1_B1_DQ6, P_Fld(0x0, SHU1_B1_DQ6_RG_ARPI_MIDPI_EN_B1)
			| P_Fld(0x1, SHU1_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1));
		vIO32WriteFldMulti_All(DDRPHY_SHU1_CA_CMD6, P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA)
			| P_Fld(0x1, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA));
	}
	mcDELAY_US(1);

	if (u1IsLP4Family(p->dram_type))
	{
	#if (fcFOR_CHIP_ID == fcTalbot)
		vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI3, P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_MCTL_EN_CA)
			| P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_FB_EN_CA)
			| P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CS_EN)
			| P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CLK_EN)
			| P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CMD_EN));
		vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI3 + SHIFT_TO_CHB_ADDR, P_Fld(u1BRPI_MCTL_EN_CA, CA_DLL_ARPI3_RG_ARPI_MCTL_EN_CA) /* CH_B CA slave */
			| P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_FB_EN_CA)
			| P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CS_EN)
			| P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CLK_EN)
			| P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CMD_EN));
	#endif
		vIO32WriteFldMulti_All(DDRPHY_B0_DLL_ARPI3, P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_FB_EN_B0)
			| P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQS_EN_B0)
			| P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQM_EN_B0)
			| P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQ_EN_B0)
			| P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B0));
		vIO32WriteFldMulti_All(DDRPHY_B1_DLL_ARPI3, P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_FB_EN_B1)
			| P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQS_EN_B1)
			| P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQM_EN_B1)
			| P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQ_EN_B1)
			| P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B1));
	}
	else
	{
		vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI3, P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_MCTL_EN_CA)
			| P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_FB_EN_CA)
			| P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CS_EN)
			| P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CLK_EN)
			| P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CMD_EN)
			| P_Fld(0x0, CA_DLL_ARPI3_RG_ARPI_CLKIEN_EN));
		vIO32WriteFldMulti(DDRPHY_B0_DLL_ARPI3, P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_FB_EN_B0)
			| P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQS_EN_B0)
			| P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQM_EN_B0)
			| P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQ_EN_B0)
			| P_Fld(0x0, B0_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B0)); /* Shih - shiu: Vinson LP3 doesn't use CHA DQS0 pad, set to 0 to save power */
		vIO32WriteFldMulti(DDRPHY_B1_DLL_ARPI3, P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_FB_EN_B1)
			| P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQS_EN_B1)
			| P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQM_EN_B1)
			| P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQ_EN_B1)
			| P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B1));
		vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI3 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_FB_EN_CA)
			| P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CS_EN)
			| P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CLK_EN)
			| P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CMD_EN)
			| P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CLKIEN_EN));
		vIO32WriteFldMulti(DDRPHY_B0_DLL_ARPI3 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_FB_EN_B0)
			| P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQS_EN_B0)
			| P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQM_EN_B0)
			| P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQ_EN_B0)
			| P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B0));
		vIO32WriteFldMulti(DDRPHY_B1_DLL_ARPI3 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_FB_EN_B1)
			| P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQS_EN_B1)
			| P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQM_EN_B1)
			| P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQ_EN_B1)
			| P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B1));
	}
	vIO32WriteFldMulti_All(DDRPHY_CA_DLL_ARPI2, P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCK_CA)
		| P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_CA)
		| P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCTL_CA)
		| P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_FB_CA)
		| P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CS)
		| P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CLK)
		| P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CMD)
		| P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA));
	vIO32WriteFldMulti_All(DDRPHY_B0_DLL_ARPI2, P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCK_B0)
		| P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B0)
		| P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCTL_B0)
		| P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_FB_B0)
		| P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQS_B0)
		| P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQM_B0)
		| P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQ_B0)
		| P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0));
	vIO32WriteFldMulti_All(DDRPHY_B1_DLL_ARPI2, P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_MCK_B1)
		| P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B1)
		| P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_MCTL_B1)
		| P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_FB_B1)
		| P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQS_B1)
		| P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQM_B1)
		| P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQ_B1)
		| P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1));
#if (fcFOR_CHIP_ID == fcTalbot)
	if (u1IsLP4Family(p->dram_type))
	{
		vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI2, 1, CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN);
		vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI2, 0, B0_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B0);
		vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI2, 0, B1_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B1);
	}
	else
	{
		vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI2, 1, CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN);
		vIO32WriteFldAlign(DDRPHY_B0_DLL_ARPI2, 1, B0_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B0);
		vIO32WriteFldAlign(DDRPHY_B1_DLL_ARPI2, 0, B1_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B1);
		vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI2 + SHIFT_TO_CHB_ADDR, 0, CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN);
		vIO32WriteFldAlign(DDRPHY_B0_DLL_ARPI2 + SHIFT_TO_CHB_ADDR, 0, B0_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B0);
		vIO32WriteFldAlign(DDRPHY_B1_DLL_ARPI2 + SHIFT_TO_CHB_ADDR, 0, B1_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B1);
	}
#endif

	mcDELAY_US(2);

	vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0x1, MISC_CG_CTRL0_CLK_MEM_SEL);
	mcDELAY_US(1);

	/* DLL */
	vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI2, 0x1, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);
	mcDELAY_US(1);
	vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI2, 0x1, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);
	mcDELAY_US(1);
	vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI2, 0x1, B0_DLL_ARPI2_RG_ARDLL_PHDET_EN_B0);
	mcDELAY_US(1);
	vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI2, 0x1, B1_DLL_ARPI2_RG_ARDLL_PHDET_EN_B1);
	mcDELAY_US(1);

	/* if(p->vendor_id==VENDOR_SAMSUNG && p->dram_type==TYPE_LPDDR3) */
	{
	#if 0
		mcSHOW_DBG_MSG(("DDRPhyPLLSetting - DMSUS\n\n"));
		vIO32WriteFldMulti_All(DDRPHY_MISC_SPM_CTRL1, P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10) | P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_B0)
			| P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_B1) | P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_CA));
		vIO32WriteFldAlign_All(DDRPHY_MISC_SPM_CTRL0, 0xffffffff, MISC_SPM_CTRL0_PHY_SPM_CTL0);
		vIO32WriteFldAlign_All(DDRPHY_MISC_SPM_CTRL2, 0xffffffff, MISC_SPM_CTRL2_PHY_SPM_CTL2);
	#else
		/*
		 * DMSUS replaced by CA_CMD2_RG_TX_ARCMD_OE_DIS, CMD_OE_DIS(1) will prevent illegal command ouput
		 * And DRAM 1st reset_n pulse will disappear if use CA_CMD2_RG_TX_ARCMD_OE_DIS
		 */
		vIO32WriteFldAlign_All(DDRPHY_CA_CMD2, 0, CA_CMD2_RG_TX_ARCMD_OE_DIS);
		DramcRestoreRegisters(p, u4RegBackupAddress, sizeof (u4RegBackupAddress) / sizeof (U32));
	#endif

		mcSHOW_DBG_MSG(("DDRPhyPLLSetting - CKEON\n\n"));

		CKEFixOnOff(p, CKE_WRITE_TO_ALL_RANK, CKE_DYNAMIC, CKE_WRITE_TO_ALL_CHANNEL);
	}

#if ENABLE_SSC
	if (p->u1PhyPLLEn)
	{
		vIO32WriteFldAlign_All(DDRPHY_PLL1, 0x1, PLL1_RG_RPHYPLL_SDM_SSC_EN);
	}
	else
	{
		vIO32WriteFldAlign_All(DDRPHY_PLL2, 0x1, PLL2_RG_RCLRPLL_SDM_SSC_EN);
	}
#endif

	DDRPhyFreqMeter();
}

#if 1
void DramcPowerOnSequence(DRAMC_CTX_T *p)
{
#if ENABLE_TMRRI_NEW_MODE
	U8 u1RankIdx;
#endif
#if APPLY_LP4_POWER_INIT_SEQUENCE
	/*
	 * static U8 u1PowerOn=0;
	 * if(u1PowerOn ==0)
	 */
	{
		/* reset dram = low */
		vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 0x0, MISC_CTRL1_R_DMDA_RRESETB_I);

		/* vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0, RKCFG_CKE2RANK_OPT2); */

		/* CKE low */
		CKEFixOnOff(p, CKE_WRITE_TO_ALL_RANK, CKE_FIXOFF, CKE_WRITE_TO_ALL_CHANNEL);

		/* delay tINIT1=200us(min) & tINIT2=10ns(min) */
		mcDELAY_US(200);

		/* reset dram = low */
		vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 0x1, MISC_CTRL1_R_DMDA_RRESETB_I);

		/* Disable HW MIOCK control to make CLK always on */
		vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 1, DRAMC_PD_CTRL_MIOCKCTRLOFF);

		/* tINIT3=2ms(min) */
		mcDELAY_MS(2);

		/* CKE high */
		CKEFixOnOff(p, CKE_WRITE_TO_ALL_RANK, CKE_FIXON, CKE_WRITE_TO_ALL_CHANNEL);

		/* tINIT5=2us(min) */
		mcDELAY_US(2);
		/* u1PowerOn=1; */
		mcSHOW_DBG_MSG3(("APPLY_LP4_POWER_INIT_SEQUENCE\n"));
	}
#endif
}


U8 gCATermWA = 0;
DRAM_STATUS_T DramcModeRegInit_CATerm(DRAMC_CTX_T *p, U8 bWorkAround)
{
    U32 u4RankIdx;//, u4CKE0Bak, u4CKE1Bak, u4MIOCKBak, u4AutoRefreshBak;
    U16 u2MR3Value;
    U8 u1MRFsp= FSP_0;
    U8 u1ChannelIdx, support_channel_num;
    U8 backup_channel, backup_rank;
    U8 operating_fsp = p->dram_fsp;
    U32 backup_broadcast;
    U8 u1MR11Value;
    U8 u1MR22Value;

    if(gCATermWA==bWorkAround)
        return DRAM_OK;

    gCATermWA=bWorkAround;

    backup_broadcast = GetDramcBroadcast();

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);

    u1ChannelIdx = p->channel;
    backup_rank = p->rank;

    u1PrintModeRegWrite=1;
#if VENDER_JV_LOG
    vPrintCalibrationBasicInfo_ForJV(p);
#endif

    //for(u1ChannelIdx=0; u1ChannelIdx<(p->support_channel_num); u1ChannelIdx++)
    //{
    //    vSetPHY2ChannelMapping(p, u1ChannelIdx);

        for(u4RankIdx =0; u4RankIdx < (U32)(p->support_rank_num); u4RankIdx++)
        {
            vSetRank(p, u4RankIdx);

            mcSHOW_DBG_MSG(("[DramcModeRegInit_CATerm] CH%u RK%u bWorkAround=%d\n", u1ChannelIdx, u4RankIdx, bWorkAround));
            mcFPRINTF((fp_A60501, "[DramcModeRegInit_CATerm] CH%u RK%d\n", u1ChannelIdx, u4RankIdx));
            #if VENDER_JV_LOG
            mcSHOW_DBG_MSG5(("\n[DramcModeRegInit_CATerm] CH%u RK%d\n", u1ChannelIdx, u4RankIdx));
            #endif
            #if MRW_CHECK_ONLY
            mcSHOW_MRW_MSG(("\n==[MR Dump] %s==\n", __func__));
            #endif

            //else
            {
                /* FSP_1: 1. For term freqs   2. Assumes "data rate >= DDR2667" are terminated */
                u1MRFsp = FSP_1;
                mcSHOW_DBG_MSG3(("\tFsp%d\n", u1MRFsp));
                mcFPRINTF((fp_A60501, "\tFsp%d\n", u1MRFsp));
                #if VENDER_JV_LOG
                mcSHOW_DBG_MSG5(("\tFsp%d\n", u1MRFsp));
                #endif

                #if MRW_CHECK_ONLY
                p->dram_fsp = FSP_1;
                #endif

                DramcMRWriteFldAlign(p, 13, 1, MR13_FSP_WR, TO_MR);

                //MR12 use previous value
                //DramcModeRegWriteByRank(p, u4RankIdx, 12, u1MR12Value[u1ChannelIdx][u4RankIdx][u1MRFsp]); //MR12 VREF-CA

                if(p->dram_type == TYPE_LPDDR4P)
                    u1MR11Value = 0x0;                  //ODT disable
                else
                {
                    u1MR11Value = 0x3;              //DQ ODT:80ohm
		#if FSP1_CLKCA_TERM
			/* For LPDDR4, keep CA ODT = 0 to disable */
			if (p->dram_type == TYPE_LPDDR4X) {
				if (p->dram_cbt_mode[u4RankIdx] == CBT_NORMAL_MODE)
					u1MR11Value |= 0x40; /* 60ohm for Normal mode */
				else
					u1MR11Value |= 0x20; /* 120ohm for Byte mode */
			}
		#endif
                }
                #if APPLY_SIGNAL_WAVEFORM_SETTINGS_ADJUST
                    if (gDramcDqOdtRZQAdjust>=0)
                        u1MR11Value = gDramcDqOdtRZQAdjust;
                #endif
                    DramcModeRegWriteByRank(p, u4RankIdx, 11, u1MR11Value); //ODT

                if(p->dram_type == TYPE_LPDDR4)
                {
                    u1MR22Value = 0x24;     //SOC-ODT, ODTE-CK, ODTE-CS, Disable ODTD-CA
                }
                else  //TYPE_LPDDR4x, LP4P
                {
                    u1MR22Value = 0x3c;     //Disable CA-CS-CLK ODT, SOC ODT=RZQ/4
                    #if FSP1_CLKCA_TERM
                    if(bWorkAround)
                    {
                        u1MR22Value = 0x4;
                    }
                    else
                    {

                        if(u4RankIdx==RANK_0)
                        {
                            u1MR22Value = 0x4;     //Enable CA-CS-CLK ODT, SOC ODT=RZQ/4
                        }
                        else
                        {
                            u1MR22Value = 0x2c;     //Enable CS ODT, SOC ODT=RZQ/4
                        }
                    }
                    #endif
                }
                #if APPLY_SIGNAL_WAVEFORM_SETTINGS_ADJUST
                if (gDramcMR22SoCODTAdjust[u1MRFsp]>=0)
                {
                    u1MR22Value = (u1MR22Value&~(0x7))|gDramcMR22SoCODTAdjust[u1MRFsp];
                }
                #endif
                DramcModeRegWriteByRank(p, u4RankIdx, 22, u1MR22Value);

            }
        }
    //}

    //vSetPHY2ChannelMapping(p, backup_channel);
    vSetRank(p, backup_rank);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_MRS), RANK_0, MRS_MRSRK);
#if MRW_CHECK_ONLY
    p->dram_fsp = operating_fsp;
#endif

    DramcBroadcastOnOff(backup_broadcast);
    u1PrintModeRegWrite=0;
    return DRAM_OK;
}
/*============ cc porting end ================*/

#if SAMSUNG_TEST_MODE_MRS_FOR_PRELOADER
void vApplyProgramSequence(DRAMC_CTX_T *p);
#endif
DRAM_STATUS_T DramcModeRegInit_LP4(DRAMC_CTX_T *p)
{
	U32 u4RankIdx; /*, u4CKE0Bak, u4CKE1Bak, u4MIOCKBak, u4AutoRefreshBak; */
	U16 u2MR3Value;
	U8 u1MRFsp = FSP_0;
	U8 u1ChannelIdx, support_channel_num;
	U8 backup_channel, backup_rank;
	U8 operating_fsp = p->dram_fsp;
	U32 backup_broadcast;
	U8 u1MR11Value;
	U8 u1MR22Value;
	U8 backup_rkcfg;
	backup_broadcast = GetDramcBroadcast();

	DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);

	DramcPowerOnSequence(p);

	backup_channel = p->channel;
	backup_rank = p->rank;


#if VENDER_JV_LOG
	vPrintCalibrationBasicInfo_ForJV(p);
#endif

	/*
	 * Fix nWR value to 30 (MR01[6:4] = 101B) for DDR3200
	 * Fix nWR value to 34 (MR01[6:4] = 110B) for DDR3733
	 * Other vendors: Use default MR01 for each FSP (Set in vInitGlobalVariablesByCondition() )
	 */
	{
		/* Clear MR01 OP[6:4] */
		u1MR01Value[FSP_0] &= 0x8F;
		u1MR01Value[FSP_1] &= 0x8F;
	#if LP4_HIGHEST_DDR3733
		/* Set MR01 OP[6:4] to 110B = 6 */
		u1MR01Value[FSP_0] |= (0x6 << 4);
		u1MR01Value[FSP_1] |= (0x6 << 4);
	#else
		/* Set MR01 OP[6:4] to 101B = 5 */
		u1MR01Value[FSP_0] |= (0x5 << 4);
		u1MR01Value[FSP_1] |= (0x5 << 4);
	#endif

		mcSHOW_DBG_MSG3(("nWR fixed to %d\n", (LP4_HIGHEST_FREQ == 1600) ? 30:34));
	#if __ETT__
		if (u2DFSGetHighestFreq(p) != LP4_HIGHEST_FREQ)
		{
			mcSHOW_ERR_MSG(("Workaround only suitable for projects with %s as highest freq!\n", (LP4_HIGHEST_FREQ == 1600) ? "LP4_DDR3200":"LP4_DDR3733"));
			while (1);
		}
	#endif
	}
#if (SW_CHANGE_FOR_SIMULATION == 0 && !defined(DUMP_INIT_RG_LOG_TO_DE))
    if (Get_PRE_MIOCK_JMETER_HQA_USED_flag() == 0)
    {
       //switch to low freq
       (p->dram_fsp == FSP_1)? CBT_Switch_Freq(p, CBT_LOW_FREQ): NULL;
    }
#endif

	for (u1ChannelIdx = 0; u1ChannelIdx < (p->support_channel_num); u1ChannelIdx++)
	{
		vSetPHY2ChannelMapping(p, u1ChannelIdx);

		for (u4RankIdx = 0; u4RankIdx < (U32)(p->support_rank_num); u4RankIdx++)
		{
			vSetRank(p, u4RankIdx);

			mcSHOW_DBG_MSG(("[ModeRegInit_LP4] CH%u RK%u\n", u1ChannelIdx, u4RankIdx));
			mcFPRINTF((fp_A60501, "[ModeRegInit_LP4] CH%u RK%d\n", u1ChannelIdx, u4RankIdx));
		#if VENDER_JV_LOG
			mcSHOW_JV_LOG_MSG(("\n[ModeRegInit_LP4] CH%u RK%d\n", u1ChannelIdx, u4RankIdx));
		#endif
		#if MRW_CHECK_ONLY
			mcSHOW_MRW_MSG(("\n == [MR Dump] %s == \n", __func__));
		#endif

			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u4RankIdx, MRS_MRSRK);

			/*
			 * Note : MR37 for LP4P should be set before any Mode register.
			 * MR37 is not shadow register, just need to set by channel and rank. No need to set by FSP
			 */
			if (p->dram_type == TYPE_LPDDR4P)
			{
				/* temp solution, need remove later */
			#ifndef MT6768_FPGA
			#if (FOR_DV_SIMULATION_USED == 0 && SW_CHANGE_FOR_SIMULATION == 0)
				dramc_set_vddq_voltage(p->dram_type, 600000);
			#endif
			#endif

				DramcModeRegWrite(p, 37, 0x1);

				/* temp solution, need remove later */
			#ifndef MT6768_FPGA
			#if (FOR_DV_SIMULATION_USED == 0 && SW_CHANGE_FOR_SIMULATION == 0)
				dramc_set_vddq_voltage(p->dram_type, 400000);
			#endif
			#endif
			}

		#if ENABLE_LP4_ZQ_CAL
			DramcZQCalibration(p); /* ZQ calobration should be done before CBT calibration by switching to low frequency */
		#endif

			/* if(p->frequency<=1200) */
			{
				/* FSP_0: 1. For un-term freqs   2. Assumes "data rate < DDR2667" are un-term */
				u1MRFsp = FSP_0;
				mcSHOW_DBG_MSG3(("\tFsp%d\n", u1MRFsp));
				mcFPRINTF((fp_A60501, "\tFsp%d\n", u1MRFsp));
			#if VENDER_JV_LOG
				mcSHOW_JV_LOG_MSG(("\tFsp%d\n", u1MRFsp));
			#endif

			#if MRW_CHECK_ONLY
				p->dram_fsp = FSP_0;
			#endif

				u1MR13Value = (MR13_RRO << 4) | (1 << 3);
				DramcModeRegWrite(p, 13, u1MR13Value); /* FSP - 0 */
				/* MR12 use previous value */
				DramcModeRegWrite(p, 12, u1MR12Value[u1ChannelIdx][u4RankIdx][u1MRFsp]); /* MR12 VREF - CA */
				DramcModeRegWrite(p, 1, u1MR01Value[u1MRFsp]);

				/* MR2 set Read/Write Latency */
				if (p->freqGroup == 800) /* DDR1600, DDR800 */
				{
					u1MR02Value[u1MRFsp] = 0x12;
				}
				else if (p->freqGroup == 1200) /* DDR2280, DDR2400 (DDR2667 uses FSP_1) */
				{
					u1MR02Value[u1MRFsp] = 0x24;
				}

				DramcModeRegWrite(p, 2, u1MR02Value[u1MRFsp]);
				/* if(p->odt_onoff) */
				DramcModeRegWrite(p, 11, 0x0);     /* ODT disable */

				if (p->dram_type == TYPE_LPDDR4)
				{
					u1MR22Value = 0x20;     /* SOC - ODT, ODTE - CK, ODTE - CS, Disable ODTD - CA */
				}
				else  /* TYPE_LPDDR4x, LP4P */
				{
					u1MR22Value = 0x38;     /* SOC - ODT, ODTE - CK, ODTE - CS, Disable ODTD - CA */
				}
			#if APPLY_SIGNAL_WAVEFORM_SETTINGS_ADJUST
				if (gDramcMR22SoCODTAdjust[u1MRFsp] >= 0)
				{
					u1MR22Value = (u1MR22Value & ~(0x7)) | gDramcMR22SoCODTAdjust[u1MRFsp];
				}
			#endif
				DramcModeRegWrite(p, 22, u1MR22Value);

				/* MR14 use previous value */
				DramcModeRegWrite(p, 14, u1MR14Value[u1ChannelIdx][u4RankIdx][u1MRFsp]);    /* MR14 VREF - DQ */

				/* MR3 set write-DBI and read-DBI (Disabled during calibration, enabled after K) */
				u1MR03Value[u1MRFsp] = (u1MR03Value[u1MRFsp] & 0x3F);

				if (p->dram_type == TYPE_LPDDR4X || p->dram_type == TYPE_LPDDR4P)
				{
					u1MR03Value[u1MRFsp] &= 0xfe;
				}
			#if APPLY_SIGNAL_WAVEFORM_SETTINGS_ADJUST
				if (gDramcMR03PDDSAdjust[u1MRFsp] >= 0)
				{
					u1MR03Value[u1MRFsp] = (u1MR03Value[u1MRFsp] & ~(0x7 << 3)) | (gDramcMR03PDDSAdjust[u1MRFsp] << 3);
				}
			#endif
				DramcModeRegWrite(p, 3, u1MR03Value[u1MRFsp]);
			}
			/* else */
			{
				/* FSP_1: 1. For term freqs   2. Assumes "data rate >= DDR2667" are terminated */
				u1MRFsp = FSP_1;
				mcSHOW_DBG_MSG3(("\tFsp%d\n", u1MRFsp));
				mcFPRINTF((fp_A60501, "\tFsp%d\n", u1MRFsp));
			#if VENDER_JV_LOG
				mcSHOW_JV_LOG_MSG(("\tFsp%d\n", u1MRFsp));
			#endif

			#if MRW_CHECK_ONLY
				p->dram_fsp = FSP_1;
			#endif

				DramcMRWriteFldAlign(p, 13, 1, MR13_FSP_WR, TO_MR);

				/* MR12 use previous value */
				DramcModeRegWrite(p, 12, u1MR12Value[u1ChannelIdx][u4RankIdx][u1MRFsp]); /* MR12 VREF - CA */
				DramcModeRegWrite(p, 1, u1MR01Value[u1MRFsp]);

				/* MR2 set Read/Write Latency */
				if (p->freqGroup == 1866)
				{
					u1MR02Value[u1MRFsp] = 0x36;
				}
				else if (p->freqGroup == 1600)
				{
					u1MR02Value[u1MRFsp] = 0x2d;
				}
				else if (p->freqGroup == 1333)
				{
					u1MR02Value[u1MRFsp] = 0x24;
				}
				else if (p->freqGroup == 1200) /* DDR2280, DDR2400 (DDR2667 uses FSP_1) */
				{
					u1MR02Value[u1MRFsp] = 0x24;
				}

				DramcModeRegWrite(p, 2, u1MR02Value[u1MRFsp]);

				if (p->dram_type == TYPE_LPDDR4P)
					u1MR11Value = 0x0;                  /* ODT disable */
				else
				{
					u1MR11Value = 0x3;              /* ODT:80ohm */
					/* u1MR11Value = 0x4;              //ODT:60ohm */

				#if FSP1_CLKCA_TERM
					/* For LPDDR4, keep CA ODT = 0 to disable */
					if (p->dram_type == TYPE_LPDDR4X) {
						if (p->dram_cbt_mode[u4RankIdx] == CBT_NORMAL_MODE)
							u1MR11Value |= 0x40; /* 60ohm for Normal mode */
						else
							u1MR11Value |= 0x20; /* 120ohm for Byte mode */
					}
				#endif
				}
			#if APPLY_SIGNAL_WAVEFORM_SETTINGS_ADJUST
				if (gDramcDqOdtRZQAdjust >= 0)
					u1MR11Value = gDramcDqOdtRZQAdjust;
			#endif
				DramcModeRegWrite(p, 11, u1MR11Value);     /* ODT */

				if (p->dram_type == TYPE_LPDDR4)
				{
					u1MR22Value = 0x24;     /* SOC - ODT, ODTE - CK, ODTE - CS, Disable ODTD - CA */
				}
				else  /* TYPE_LPDDR4x, LP4P */
				{
					u1MR22Value = 0x3c;     /* SOC - ODT, ODTE - CK, ODTE - CS, Disable ODTD - CA */

				#if FSP1_CLKCA_TERM
					if(u4RankIdx==RANK_0)
					{
						u1MR22Value = 0x4;     //Enable CA-CS-CLK ODT, SOC ODT=RZQ/4
					}
					else
					{
						u1MR22Value = 0x2c;      //Enable CS ODT, SOC ODT=RZQ/4
					}
				#endif
				}
			#if APPLY_SIGNAL_WAVEFORM_SETTINGS_ADJUST
				if (gDramcMR22SoCODTAdjust[u1MRFsp] >= 0)
				{
					u1MR22Value = (u1MR22Value & ~(0x7)) | gDramcMR22SoCODTAdjust[u1MRFsp];
				}
			#endif
				DramcModeRegWrite(p, 22, u1MR22Value);

				/* MR14 use previous value */
				DramcModeRegWrite(p, 14, u1MR14Value[u1ChannelIdx][u4RankIdx][u1MRFsp]);    /* MR14 VREF - DQ */

				/* MR3 set write-DBI and read-DBI (Disabled during calibration, enabled after K) */
				u1MR03Value[u1MRFsp] = (u1MR03Value[u1MRFsp] & 0x3F);

				if (p->dram_type == TYPE_LPDDR4X || p->dram_type == TYPE_LPDDR4P)
				{
					u1MR03Value[u1MRFsp] &= 0xfe;
				}
			#if APPLY_SIGNAL_WAVEFORM_SETTINGS_ADJUST
				if (gDramcMR03PDDSAdjust[u1MRFsp] >= 0)
				{
					u1MR03Value[u1MRFsp] = (u1MR03Value[u1MRFsp] & ~(0x7 << 3)) | (gDramcMR03PDDSAdjust[u1MRFsp] << 3);
				}
			#endif
				DramcModeRegWrite(p, 3, u1MR03Value[u1MRFsp]);
			}

		}

		/* freq < 1333 is assumed to be odt_off -> uses FSP_0 */
		//if (p->frequency < MRFSP_TERM_FREQ)
		if(operating_fsp == FSP_0)
		{
		    DramcMRWriteFldAlign(p, 13, 0, MR13_FSP_OP, JUST_TO_GLOBAL_VALUE);
		    DramcMRWriteFldAlign(p, 13, 0, MR13_FSP_WR, JUST_TO_GLOBAL_VALUE);
		}
		else
		{
		    DramcMRWriteFldAlign(p, 13, 1, MR13_FSP_OP, JUST_TO_GLOBAL_VALUE);
		    DramcMRWriteFldAlign(p, 13, 1, MR13_FSP_WR, JUST_TO_GLOBAL_VALUE);
		}

#if 0
		for(u4RankIdx =0; u4RankIdx < (U32)(p->support_rank_num); u4RankIdx++)
		{
		    DramcModeRegWriteByRank(p, u4RankIdx, 13, u1MR13Value);
		}
#endif

		/* MRS two ranks simutaniously */
		backup_rkcfg = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), RKCFG_CS2RANK);
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), 0x1, RKCFG_CS2RANK);
		DramcModeRegWriteByRank(p, RANK_0, 13, u1MR13Value);
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), backup_rkcfg, RKCFG_CS2RANK);
		/*
		 * Auto-MRW related register write (Used during HW DVFS frequency switch flow)
		 * VRCG seems to be enabled/disabled even when switching to same FSP(but different freq) to simplify HW DVFS flow
		 */
		/* 1. MR13 OP[3] = 1 : Enable "high current mode" to reduce the settling time when changing FSP(freq) during operation */
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_MR13), P_Fld(u1MR13Value | (0x1 << 3), SHU_HWSET_MR13_HWSET_MR13_OP)
			| P_Fld(13, SHU_HWSET_MR13_HWSET_MR13_MRSMA));
		/* 2. MR13 OP[3] = 1 : Enable "high current mode" after FSP(freq) switch operation for calibration */
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_VRCG), P_Fld(u1MR13Value | (0x1 << 3), SHU_HWSET_VRCG_HWSET_VRCG_OP)
			| P_Fld(13, SHU_HWSET_VRCG_HWSET_VRCG_MRSMA));
		/* 3. MR2 : Set RL/WL after FSP(freq) switch */
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_MR2), P_Fld(u1MR02Value[operating_fsp], SHU_HWSET_MR2_HWSET_MR2_OP)
			| P_Fld(2, SHU_HWSET_MR2_HWSET_MR2_MRSMA));
	}

#if (SW_CHANGE_FOR_SIMULATION == 0)
	    if (Get_PRE_MIOCK_JMETER_HQA_USED_flag() == 0)
	    {
	        //switch to low freq
	        (p->dram_fsp == FSP_1)? CBT_Switch_Freq(p, CBT_HIGH_FREQ): NULL;
	    }
#endif

	vSetPHY2ChannelMapping(p, backup_channel);
	vSetRank(p, backup_rank);
	vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_MRS), RANK_0, MRS_MRSRK);
#if MRW_CHECK_ONLY
	p->dram_fsp = operating_fsp;
#endif

	DramcBroadcastOnOff(backup_broadcast);

#if SAMSUNG_TEST_MODE_MRS_FOR_PRELOADER
	vApplyProgramSequence(p);
#endif

	return DRAM_OK;
}
#else

DRAM_STATUS_T DramcModeRegInit_LP4(DRAMC_CTX_T *p, U8 init_type)
{
	U32 u4RankIdx; /*, u4CKE0Bak, u4CKE1Bak, u4MIOCKBak, u4AutoRefreshBak; */
	U16 u2MR3Value;

	vPrintCalibrationBasicInfo(p);

#if APPLY_LP4_POWER_INIT_SEQUENCE
	static U8 u1PowerOn = 0;
	if (u1PowerOn == 0)
	{
		/* reset dram = low */
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DDRCONF0), 0, DDRCONF0_GDDR3RST);

		/* CKE low */
		CKEFixOnOff(p, CKE_FIXOFF, CKE_WRITE_TO_ONE_CHANNEL);
		/* delay tINIT1=200us(min) & tINIT2=10ns(min) */
		mcDELAY_US(200);

		/* reset dram = low */
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DDRCONF0), 1, DDRCONF0_GDDR3RST);

		/* Disable HW MIOCK control to make CLK always on */
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 1, DRAMC_PD_CTRL_MIOCKCTRLOFF);

		/* tINIT3=2ms(min) */
		mcDELAY_MS(2);

		/* CKE high */
		CKEFixOnOff(p, CKE_FIXON, CKE_WRITE_TO_ONE_CHANNEL);
		/* tINIT5=2us(min) */
		mcDELAY_US(2);
		u1PowerOn = 1;
		mcSHOW_DBG_MSG3(("APPLY_LP4_POWER_INIT_SEQUENCE\n"));
	}
#endif

	for (u4RankIdx = 0; u4RankIdx < (U32)(p->support_rank_num); u4RankIdx++)
	{
		mcSHOW_DBG_MSG(("ModeRegInit_LP4 RK%d dram_fsp %d\n", u4RankIdx, p->dram_fsp));
		mcFPRINTF((fp_A60501, "ModeRegInit_LP4 RK%d dram_fsp %d\n", u4RankIdx, p->dram_fsp));

		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u4RankIdx, MRS_MRSRK);

		/*
		 * Note : MR37 for LP4P should be set before any Mode register.
		 * MR37 is not shadow register, just need to set by channel and rank. No need to set by FSP
		 */
		if (p->dram_type == TYPE_LPDDR4P)
		{
			/* temp solution, need remove later */
		#ifndef MT6765_FPGA
			fan53528buc08x_set_voltage((unsigned long)600000);
		#endif

			DramcModeRegWrite(p, 37, 0x1);

			/* temp solution, need remove later */
		#ifndef MT6765_FPGA
			fan53528buc08x_set_voltage((unsigned long)400000);
		#endif
		}

		/* if(p->frequency<=1200) */
		if (p->odt_onoff == ODT_OFF)
		{
			u1MR13Value[p->dram_fsp] = 0 | (MR13_RRO << 4) | (1 << 3);
			DramcModeRegWrite(p, 0xd, u1MR13Value[p->dram_fsp]); /* FSP - 0 */
			/* u1MR12Value[p->channel][p->dram_fsp] = 0x4d; */
			DramcModeRegWrite(p, 0xc, u1MR12Value[p->channel][p->dram_fsp]); /* MR12 VREF - CA */
			u1MR01Value[p->dram_fsp] = 0x56;
			DramcModeRegWrite(p, 0x1, u1MR01Value[p->dram_fsp]);

			if (p->frequency == 1600)
			{
				u1MR02Value[p->dram_fsp] = 0x2d;
			}
			else if (p->frequency == 1333)
			{
				u1MR02Value[p->dram_fsp] = 0x24;
			}
			else if (p->frequency == 1200)
			{
			}
			else if (p->frequency == 800)
			{
				u1MR02Value[p->dram_fsp] = 0x12;
			}
			DramcModeRegWrite(p, 0x2, u1MR02Value[p->dram_fsp]);
			DramcModeRegWrite(p, 0xb, 0x0);     /* ODT */
			if (p->dram_type == TYPE_LPDDR4)
			{
				DramcModeRegWrite(p, 0x16, 0x20);    /* SOC - ODT, ODTE - CK, ODTE - CS, disable ODTD - CA */
			}
			else  /* TYPE_LPDDR4x & TYPE_LPDDR4P */
			{
				DramcModeRegWrite(p, 0x16, 0x38);    /* SOC - ODT, ODTE - CK, ODTE - CS, disable ODTD - CA */
			}
			/* u1MR14Value[p->channel][p->dram_fsp] = 0x4d; */
			DramcModeRegWrite(p, 0xe, u1MR14Value[p->channel][p->dram_fsp]);    /* MR14 VREF - DQ */
		}
		else
		{
		#if 0/* CA - Term */
			u1MR13Value[p->dram_fsp] = 0xc0 | (MR13_RRO << 4);
			DramcModeRegWrite(p, 0xd, u1MR13Value[p->dram_fsp]); /* FSP - 1 */
			u1MR12Value[p->dram_fsp] = 0x10;
			DramcModeRegWrite(p, 0xc, u1MR12Value[p->dram_fsp]); /* MR12 VREF - CA */
			u1MR01Value[p->dram_fsp] = 0x56;
			DramcModeRegWrite(p, 0x1, u1MR01Value[p->dram_fsp]);

			if (p->frequency == 1600)
			{
				u1MR02Value[p->dram_fsp] = 0x2d;
			}
			else if (p->frequency == 1333)
			{
				u1MR02Value[p->dram_fsp] = 0x24;
			}
			DramcModeRegWrite(p, 0x2, u1MR02Value[p->dram_fsp]);
			DramcModeRegWrite(p, 0xb, 0x44);     /* ODT */
			DramcModeRegWrite(p, 0x16, 0xC4);    /* SOC - ODT, ODTE - CK, ODTE - CS, ODTD - CA */
			u1MR14Value[p->dram_fsp] = 0x10;
			DramcModeRegWrite(p, 0xe, u1MR14Value);    /* MR14 VREF - DQ */
		#else
			u1MR13Value[p->dram_fsp] = 0xc0 | (MR13_RRO << 4) | (1 << 3);
			DramcModeRegWrite(p, 0xd, u1MR13Value[p->dram_fsp]); /* FSP - 1 */
			/* u1MR12Value[p->channel][p->dram_fsp] = 0x4d; */
			DramcModeRegWrite(p, 0xc, u1MR12Value[p->channel][p->dram_fsp]); /* MR12 VREF - CA */
			DramcModeRegWrite(p, 0x1, 0x56);
			if (p->frequency == 1600)
			{
				u1MR02Value[p->dram_fsp] = 0x2d;
			}
			else if (p->frequency == 1333)
			{
				u1MR02Value[p->dram_fsp] = 0x24;
			}
			DramcModeRegWrite(p, 0x2, u1MR02Value[p->dram_fsp]);
			DramcModeRegWrite(p, 0xb, 0x04);     /* ODT */
			if (p->dram_type == TYPE_LPDDR4)
			{
				DramcModeRegWrite(p, 0x16, 0x24);    /* SOC - ODT, ODTE - CK, ODTE - CS, disable ODTD - CA */
			}
			else  /* TYPE_LPDDR4x */
			{
				DramcModeRegWrite(p, 0x16, 0x3c);    /* SOC - ODT, ODTE - CK, ODTE - CS, disable ODTD - CA */
			}

			/* u1MR14Value[p->channel][p->dram_fsp] = 0x10; */
			DramcModeRegWrite(p, 0xe, u1MR14Value[p->channel][p->dram_fsp]);    /* MR14 VREF - DQ */
		#endif
		}

		u2MR3Value = 0x31;
		u2MR3Value |= ((p->DBI_W_onoff) << 7 | (p->DBI_R_onoff) << 6);
		if (p->dram_type == TYPE_LPDDR4X || p->dram_type == TYPE_LPDDR4P)
			u2MR3Value &= 0xfe;  /* MR3[0] = 0, PU - Cal = VEED / 2.5 */

		DramcModeRegWrite(p, 0x3, u2MR3Value); /* Enable DBI */
	}


	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), RANK_0, MRS_MRSRK);

	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_MR2), P_Fld(u1MR02Value[p->dram_fsp], SHU_HWSET_MR2_HWSET_MR2_OP)
		| P_Fld(2, SHU_HWSET_MR2_HWSET_MR2_MRSMA));
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_MR13), P_Fld(u1MR13Value[p->dram_fsp] | (1 << 3), SHU_HWSET_MR13_HWSET_MR13_OP)
		| P_Fld(13, SHU_HWSET_MR13_HWSET_MR13_MRSMA));
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_VRCG), P_Fld(u1MR13Value[p->dram_fsp] & 0xf7, SHU_HWSET_VRCG_HWSET_VRCG_OP)
		| P_Fld(13, SHU_HWSET_VRCG_HWSET_VRCG_MRSMA));

	return DRAM_OK;
}

#endif

#if (__ETT__ || ENABLE_LP3_SW) /* remove unused function to save space of log string */
#if LP3_MR_INIT_AFTER_CA_TRAIN
DRAM_STATUS_T DramcModeRegInit_LP3(DRAMC_CTX_T *p, U8 u1ResetDram)
#else
DRAM_STATUS_T DramcModeRegInit_LP3(DRAMC_CTX_T *p)
#endif
{
	U8 u1RankIdx = 0;
	U32 u4AutoRefreshBak;

#if 0
	U32 u4CKEFIXBak, u4MIOCKBak;
	u4MIOCKBak = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), DRAMC_PD_CTRL_MIOCKCTRLOFF);
	u4CKEFIXBak = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL));
#endif
	u4AutoRefreshBak = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0), REFCTRL0_REFDIS);

	/* Disable HW MIOCK control to make CLK always on */
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 1, DRAMC_PD_CTRL_MIOCKCTRLOFF);
	mcDELAY_US(1);

	CKEFixOnOff(p, CKE_WRITE_TO_ALL_RANK, CKE_FIXON, CKE_WRITE_TO_ONE_CHANNEL);

	/* disable auto refresh command during Dram reset (MR63) */
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0), 1, REFCTRL0_REFDIS);

	/*
	 * wait cke to reset timing constraint 'tinit3'
	 * for (i=0; i < 10 ; ++i);
	 */
	mcDELAY_US(200);

	for (u1RankIdx = 0; u1RankIdx < (U8)(p->support_rank_num); u1RankIdx++)
	{
	#if MRW_CHECK_ONLY
		mcSHOW_MRW_MSG(("\n == [MR Dump] %s == \n", __func__));
	#endif

		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1RankIdx, MRS_MRSRK);

	#if LP3_MR_INIT_AFTER_CA_TRAIN
		if (u1ResetDram)
	#endif
		{
			DramcModeRegWrite(p, 63, 0xfc);       /* MR63 = 0   -> Reset */
			mcDELAY_US(10);  /* wait 10us for dram reset */
			mcSHOW_DBG_MSG2(("MR63 Reset Dram RK%d\n", u1RankIdx));
		}
	#if LP3_MR_INIT_AFTER_CA_TRAIN
		else
	#endif
		{
			mcSHOW_DBG_MSG2(("ModeRegInit_LP3 for RK%d\n", u1RankIdx));
			mcFPRINTF((fp_A60501, "ModeRegInit_LP3 for RK%d\n", u1RankIdx));

			if (p->vendor_id == VENDOR_HYNIX && p->revision_id == 4) /* MR6 is the die ID, if u read as 4, that's 21nm die, and if u read as 3, that's 25nm die. */
			{
				Hynix_Test_Mode(p);
			}

			DramcModeRegWrite(p, 10, 0xff);   /* MR10 = 0xff  -> ZQ Init */

			DramcModeRegWrite(p, 1, 0x83);      /* MR1 = 0x83 nWR: OP[7:5] (14 -> LP3 - 1866), A60817 is 0x43  nWR: OP[7:5] (12 -> LP3 - 1600) */

			/* MR2 */
			/*
			 * if (p->freqGroup <= 533)
			 * {
			 * u1MR02Value[p->dram_fsp] = 0x16;
			 * }
			 * else
			 */
			/* LP3_DDR1200's RL/WL can only be set to 10/6 due to design constraints (Unique, YH) */
			if (p->freqGroup == 600 || p->freqGroup == 667)
			{
				u1MR02Value[p->dram_fsp] = 0x18;
			}
			else if (p->freqGroup == 800)
			{
				u1MR02Value[p->dram_fsp] = 0x1a;
			}
			else
			{
				u1MR02Value[p->dram_fsp] = 0x1c;
			}
			DramcModeRegWrite(p, 2, u1MR02Value[p->dram_fsp]);

			DramcModeRegWrite(p, 11, 0x0);      /* MR11 = 0x0, ODT disable. */
		}
	}

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), 0, MRS_MRSRK);

	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_MR2), P_Fld(0, SHU_HWSET_MR2_HWSET_MR2_OP)
		| P_Fld(0, SHU_HWSET_MR2_HWSET_MR2_MRSMA));
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_MR13), P_Fld(u1MR02Value[p->dram_fsp], SHU_HWSET_MR13_HWSET_MR13_OP)
		| P_Fld(2, SHU_HWSET_MR13_HWSET_MR13_MRSMA));
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_VRCG), P_Fld(0, SHU_HWSET_VRCG_HWSET_VRCG_OP)
		| P_Fld(0, SHU_HWSET_VRCG_HWSET_VRCG_MRSMA));

	/* Restore auto refresh command after Dram reset (MR63) */
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0), u4AutoRefreshBak, REFCTRL0_REFDIS);

#if 0/* Set to dynamic in ApplyAfterCalibration(); */
	/* Restore CKE fix on setting */
	vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), u4CKEFIXBak);

	/* Restore HW MIOCK control setting */
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), u4MIOCKBak, DRAMC_PD_CTRL_MIOCKCTRLOFF);
#endif

	/* TINFO="===  dram initial end (DramcModeRegInit_LP3)===" */
	return DRAM_OK;
}
#endif /* remove unused function to save space of log string */

void MPLLInit()
{
#if (FOR_DV_SIMULATION_USED == 0)
	unsigned int tmp;

	/*
	 * DRV_WriteReg32(AP_PLL_CON0, 0x85); // CLKSQ Enable
	 * mcDELAY_US(100);
	 * DRV_WriteReg32(AP_PLL_CON0, 0x87); // CLKSQ LPF Enable
	 * mcDELAY_MS(1);
	 */
	DRV_WriteReg32(MPLL_CON3, 0x3); /* power on MPLL */

	mcDELAY_US(30);

	tmp = DRV_Reg32(MPLL_CON3);
	DRV_WriteReg32(MPLL_CON3, tmp & 0xFFFFFFFD); /* turn off ISO of  MPLL */

	mcDELAY_US(1);

	DRV_WriteReg32(MPLL_CON1, 0x83100000); /* Config MPLL freq */

	tmp = DRV_Reg32(MPLL_CON0);
	DRV_WriteReg32(MPLL_CON0, tmp | 0x1); /* enable MPLL */

	mcDELAY_US(20);
#endif
}

void DDRPhyFreqSel(DRAMC_CTX_T *p, DRAM_PLL_FREQ_SEL_T sel)
{
	p->freq_sel = sel;

	switch (p->freq_sel)
	{
		case LP4_DDR3733:
			p->frequency = 1866;
			break;
		case LP4_DDR3200:
			p->frequency = 1600;
			break;
		case LP4_DDR2667:
			p->frequency = 1333;
			break;
		case LP4_DDR2400:
			p->frequency = 1200;
			break;
		case LP4_DDR1600:
			p->frequency = 800;
			break;
	#if (__ETT__ || ENABLE_LP3_SW)
		/* scy: reduce code size by removing unused LPDDR3 opp */
		case LP3_DDR1866:
			p->frequency = 933;
			break;
		case LP3_DDR1600:
			p->frequency = 800;
			break;
		case LP3_DDR1333:
			p->frequency = 667;
			break;
		case LP3_DDR1200:
			p->frequency = 600;
			break;
		case LP3_DDR933:
			p->frequency = 467;
			break;
	#endif
		default:
			p->frequency = 800;
			break;
	}

	gu4TermFreq = MRFSP_TERM_FREQ;

	p->dram_fsp = (p->frequency < gu4TermFreq) ? FSP_0 : FSP_1;
	p->odt_onoff = (p->frequency < gu4TermFreq) ? ODT_OFF : ODT_ON;
#if (ENABLE_DQ3200_UNTERM == 1)
	if ((p->frequency == 1333) || (p->frequency == 1600))
		p->odt_onoff = ODT_OFF;
#endif

	if (p->dram_type == TYPE_LPDDR4P)
		p->odt_onoff = ODT_OFF;

	p->shu_type = get_shuffleIndex_by_Freq(p);
	setFreqGroup(p); /* Set p->freqGroup to support freqs not in ACTimingTable */

	/*
	 * TODO: add DBI_onoff by condition
	 * p->DBI_onoff = p->odt_onoff;
	 */
}

#if ENABLE_RODT_TRACKING_SAVE_MCK
void SetTxWDQSStatusOnOff(U8 u1OnOff)
{
	u1WDQS_ON = u1OnOff;
}
#endif

void DDRPhyReservedRGSetting(DRAMC_CTX_T *p)
{
	U8 u1HYST_SEL = 0;
	U8 u1MIDPI_CAP_SEL2 = 0;
	U8 u1LP3_SEL = 0;
	U8 u1SER_RST_MODE = 1;
	U8 u1TX_READ_BASE_EN = 1;
#if TX_DQ_PRE_EMPHASIS
	U8 u1DRVING = 0;
#endif

	if (p->frequency <= 1333)
		u1HYST_SEL = 1;

	if (u1IsLP4Family(p->dram_type) && p->frequency < 1333)
		u1MIDPI_CAP_SEL2 = 1;
	else
		u1MIDPI_CAP_SEL2 = 0;

	if (p->frequency <= 933)
		u1LP3_SEL = 1;

	if (p->dram_type == TYPE_LPDDR3)
	{
		u1TX_READ_BASE_EN = 0; /* this Rev.bit9 should be set as 0 in LP3, or the Gating will k fail (TX / RX signals will collide) */
	#if DramcHWDQSGatingTracking_DVT_JADE_TRACKING_MODE
		u1SER_RST_MODE = 0;
	#endif
	}

	/* PI */
	vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD6, P_Fld(0x1, RG_ARPI_RESERVE_BIT_00_TX_CG_EN) /* RG_*RPI_RESERVE_B0[0] 1'b1 prevent leakage */
		| P_Fld(1, RG_ARPI_RESERVE_BIT_01_DLL_FAST_PSJP)
		| P_Fld(u1HYST_SEL, RG_ARPI_RESERVE_BIT_02_HYST_SEL)
		| P_Fld(u1MIDPI_CAP_SEL2, RG_ARPI_RESERVE_BIT_03_MIDPI_CAP_SEL)
		| P_Fld(1, RG_ARPI_RESERVE_BIT_04_8PHASE_XLATCH_FORCE)
		| P_Fld(0, RG_ARPI_RESERVE_BIT_05_PSMUX_XLATCH_FORCE)
		| P_Fld(0, RG_ARPI_RESERVE_BIT_06_PSMUX_XLATCH_FORCEDQS)
		| P_Fld(0, RG_ARPI_RESERVE_BIT_07_SMT_XLATCH_FORCE)
		| P_Fld(0, RG_ARPI_RESERVE_BIT_08_SMT_XLATCH_FORCE_DQS)
		| P_Fld(0, RG_ARPI_RESERVE_BIT_09_BUFGP_XLATCH_FORCE)
		| P_Fld(0, RG_ARPI_RESERVE_BIT_10_BUFGP_XLATCH_FORCE_DQS)
		| P_Fld(1, RG_ARPI_RESERVE_BIT_11_BYPASS_SR)
		| P_Fld(1, RG_ARPI_RESERVE_BIT_12_BYPASS_SR_DQS)
		| P_Fld(0, RG_ARPI_RESERVE_BIT_13_CG_SYNC_ENB)
		| P_Fld(u1LP3_SEL, RG_ARPI_RESERVE_BIT_14_LP3_SEL)
		| P_Fld(0, RG_ARPI_RESERVE_BIT_15));

	vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD6 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, RG_ARPI_RESERVE_BIT_00_TX_CG_EN) /* RG_*RPI_RESERVE_B0[0] 1'b1 prevent leakage */
		| P_Fld(0, RG_ARPI_RESERVE_BIT_01_DLL_FAST_PSJP)
		| P_Fld(u1HYST_SEL, RG_ARPI_RESERVE_BIT_02_HYST_SEL)
		| P_Fld(u1MIDPI_CAP_SEL2, RG_ARPI_RESERVE_BIT_03_MIDPI_CAP_SEL)
		| P_Fld(1, RG_ARPI_RESERVE_BIT_04_8PHASE_XLATCH_FORCE)
		| P_Fld(0, RG_ARPI_RESERVE_BIT_05_PSMUX_XLATCH_FORCE)
		| P_Fld(0, RG_ARPI_RESERVE_BIT_06_PSMUX_XLATCH_FORCEDQS)
		| P_Fld(0, RG_ARPI_RESERVE_BIT_07_SMT_XLATCH_FORCE)
		| P_Fld(0, RG_ARPI_RESERVE_BIT_08_SMT_XLATCH_FORCE_DQS)
		| P_Fld(0, RG_ARPI_RESERVE_BIT_09_BUFGP_XLATCH_FORCE)
		| P_Fld(0, RG_ARPI_RESERVE_BIT_10_BUFGP_XLATCH_FORCE_DQS)
		| P_Fld(1, RG_ARPI_RESERVE_BIT_11_BYPASS_SR)
		| P_Fld(1, RG_ARPI_RESERVE_BIT_12_BYPASS_SR_DQS)
		| P_Fld(0, RG_ARPI_RESERVE_BIT_13_CG_SYNC_ENB)
		| P_Fld(u1LP3_SEL, RG_ARPI_RESERVE_BIT_14_LP3_SEL)
		| P_Fld(0, RG_ARPI_RESERVE_BIT_15));

	vIO32WriteFldMulti_All(DDRPHY_SHU1_CA_DLL1, P_Fld(0x1, RG_ARCMD_REV_BIT_00_TX_LSH_DQ_CG_EN)
		| P_Fld(0x0, RG_ARCMD_REV_BIT_01_TX_LSH_DQS_CG_EN)
		| P_Fld(0x0, RG_ARCMD_REV_BIT_02_TX_LSH_DQM_CG_EN)
		| P_Fld(0x0, RG_ARCMD_REV_BIT_03_RX_DQS_GATE_EN_MODE)
		| P_Fld(0x0, RG_ARCMD_REV_BIT_04_RX_DQSIEN_RB_DLY)
		| P_Fld(u1SER_RST_MODE, RG_ARCMD_REV_BIT_05_RX_SER_RST_MODE)
		| P_Fld(0x1, RG_ARCMD_REV_BIT_06_MCK4X_SEL_CKE0)
		| P_Fld(0x1, RG_ARCMD_REV_BIT_07_MCK4X_SEL_CKE1)
		| P_Fld(0x4, RG_ARCMD_REV_BIT_1208_TX_CKE_DRVN)
		| P_Fld(0x0, RG_ARCMD_REV_BIT_13_TX_DDR3_CKE_SEL)
		| P_Fld(0x0, RG_ARCMD_REV_BIT_14_TX_DDR4_CKE_SEL)
		| P_Fld(0x0, RG_ARCMD_REV_BIT_15_TX_DDR4P_CKE_SEL)
		| P_Fld(0x0, RG_ARCMD_REV_BIT_1716_TX_LP4Y_SEL)
		| P_Fld(0x0, RG_ARCMD_REV_BIT_18_RX_LP4Y_EN)
		| P_Fld(0x0, RG_ARCMD_REV_BIT_19_RX_DQSIEN_FORCE_ON_EN)
		| P_Fld(0x0, RG_ARCMD_REV_BIT_20_DATA_SWAP_EN)
		| P_Fld(0x0, RG_ARCMD_REV_BIT_2221_DATA_SWAP)
		| P_Fld(0x0, RG_ARCMD_REV_BIT_23_NA));

	vIO32WriteFldMulti_All(DDRPHY_SHU1_B0_DQ6, P_Fld(0x1, RG_ARPI_RESERVE_BIT_00_TX_CG_EN) /* RG_*RPI_RESERVE_B0[0] 1'b1 prevent leakage */
		| P_Fld(0, RG_ARPI_RESERVE_BIT_01_DLL_FAST_PSJP)
		| P_Fld(u1HYST_SEL, RG_ARPI_RESERVE_BIT_02_HYST_SEL)
		| P_Fld(u1MIDPI_CAP_SEL2, RG_ARPI_RESERVE_BIT_03_MIDPI_CAP_SEL)
		| P_Fld(1, RG_ARPI_RESERVE_BIT_04_8PHASE_XLATCH_FORCE)
		| P_Fld(0, RG_ARPI_RESERVE_BIT_05_PSMUX_XLATCH_FORCE)
		| P_Fld(0, RG_ARPI_RESERVE_BIT_06_PSMUX_XLATCH_FORCEDQS)
		| P_Fld(0, RG_ARPI_RESERVE_BIT_07_SMT_XLATCH_FORCE)
		| P_Fld(0, RG_ARPI_RESERVE_BIT_08_SMT_XLATCH_FORCE_DQS)
		| P_Fld(0, RG_ARPI_RESERVE_BIT_09_BUFGP_XLATCH_FORCE)
		| P_Fld(0, RG_ARPI_RESERVE_BIT_10_BUFGP_XLATCH_FORCE_DQS)
		| P_Fld(1, RG_ARPI_RESERVE_BIT_11_BYPASS_SR)
		| P_Fld(1, RG_ARPI_RESERVE_BIT_12_BYPASS_SR_DQS)
		| P_Fld(0, RG_ARPI_RESERVE_BIT_13_CG_SYNC_ENB)
		| P_Fld(u1LP3_SEL, RG_ARPI_RESERVE_BIT_14_LP3_SEL)
		| P_Fld(0, RG_ARPI_RESERVE_BIT_15));


	vIO32WriteFldMulti_All(DDRPHY_SHU1_B1_DQ6, P_Fld(0x1, RG_ARPI_RESERVE_BIT_00_TX_CG_EN) /* RG_*RPI_RESERVE_B0[0] 1'b1 prevent leakage */
		| P_Fld(0, RG_ARPI_RESERVE_BIT_01_DLL_FAST_PSJP)
		| P_Fld(u1HYST_SEL, RG_ARPI_RESERVE_BIT_02_HYST_SEL)
		| P_Fld(u1MIDPI_CAP_SEL2, RG_ARPI_RESERVE_BIT_03_MIDPI_CAP_SEL)
		| P_Fld(1, RG_ARPI_RESERVE_BIT_04_8PHASE_XLATCH_FORCE)
		| P_Fld(0, RG_ARPI_RESERVE_BIT_05_PSMUX_XLATCH_FORCE)
		| P_Fld(0, RG_ARPI_RESERVE_BIT_06_PSMUX_XLATCH_FORCEDQS)
		| P_Fld(0, RG_ARPI_RESERVE_BIT_07_SMT_XLATCH_FORCE)
		| P_Fld(0, RG_ARPI_RESERVE_BIT_08_SMT_XLATCH_FORCE_DQS)
		| P_Fld(0, RG_ARPI_RESERVE_BIT_09_BUFGP_XLATCH_FORCE)
		| P_Fld(0, RG_ARPI_RESERVE_BIT_10_BUFGP_XLATCH_FORCE_DQS)
		| P_Fld(1, RG_ARPI_RESERVE_BIT_11_BYPASS_SR)
		| P_Fld(1, RG_ARPI_RESERVE_BIT_12_BYPASS_SR_DQS)
		| P_Fld(0, RG_ARPI_RESERVE_BIT_13_CG_SYNC_ENB)
		| P_Fld(u1LP3_SEL, RG_ARPI_RESERVE_BIT_14_LP3_SEL)
		| P_Fld(0, RG_ARPI_RESERVE_BIT_15));

	/* TX */
	vIO32WriteFldMulti_All(DDRPHY_SHU1_B0_DLL1, P_Fld(0x0, RG_ARDQ_REV_BIT_00_DQS_MCK4X_DLY_EN)
		| P_Fld(0x0, RG_ARDQ_REV_BIT_01_DQS_MCK4XB_DLY_EN)
		| P_Fld(u1TX_READ_BASE_EN, RG_ARDQ_REV_BIT_02_TX_READ_BASE_EN_DQSB)
		| P_Fld(0x0, RG_ARDQ_REV_BIT_03_RX_DQS_GATE_EN_MODE)
		| P_Fld(0x0, RG_ARDQ_REV_BIT_04_RX_DQSIEN_RB_DLY)
		| P_Fld(u1SER_RST_MODE, RG_ARDQ_REV_BIT_05_RX_SER_RST_MODE)
		| P_Fld(0x0, RG_ARDQ_REV_BIT_06_MCK4X_SEL_DQ1)
		| P_Fld(0x0, RG_ARDQ_REV_BIT_07_MCK4X_SEL_DQ5)
		| P_Fld(0x0, RG_ARDQ_REV_BIT_08_TX_ODT_DISABLE)
		| P_Fld(u1TX_READ_BASE_EN, RG_ARDQ_REV_BIT_09_TX_READ_BASE_EN)
		| P_Fld(0x0, RG_ARDQ_REV_BIT_1110_DRVN_PRE)
		| P_Fld(0x0, RG_ARDQ_REV_BIT_1312_DRVP_PRE)
		| P_Fld(0x0, RG_ARDQ_REV_BIT_14_TX_PRE_DATA_SEL)
		| P_Fld(0x0, RG_ARDQ_REV_BIT_15_TX_PRE_EN)
		| P_Fld(0x0, RG_ARDQ_REV_BIT_1716_TX_LP4Y_SEL)
		| P_Fld(0x0, RG_ARDQ_REV_BIT_18_RX_LP4Y_EN)
		| P_Fld(0x0, RG_ARDQ_REV_BIT_19_RX_DQSIEN_FORCE_ON_EN)
		| P_Fld(0x0, RG_ARDQ_REV_BIT_20_DATA_SWAP_EN)
		| P_Fld(0x0, RG_ARDQ_REV_BIT_2221_DATA_SWAP)
		| P_Fld(0x0, RG_ARDQ_REV_BIT_23_NA));

	vIO32WriteFldMulti_All(DDRPHY_SHU1_B1_DLL1, P_Fld(0x0, RG_ARDQ_REV_BIT_00_DQS_MCK4X_DLY_EN)
		| P_Fld(0x0, RG_ARDQ_REV_BIT_01_DQS_MCK4XB_DLY_EN)
		| P_Fld(u1TX_READ_BASE_EN, RG_ARDQ_REV_BIT_02_TX_READ_BASE_EN_DQSB)
		| P_Fld(0x0, RG_ARDQ_REV_BIT_03_RX_DQS_GATE_EN_MODE)
		| P_Fld(0x0, RG_ARDQ_REV_BIT_04_RX_DQSIEN_RB_DLY)
		| P_Fld(u1SER_RST_MODE, RG_ARDQ_REV_BIT_05_RX_SER_RST_MODE)
		| P_Fld(0x0, RG_ARDQ_REV_BIT_06_MCK4X_SEL_DQ1)
		| P_Fld(0x0, RG_ARDQ_REV_BIT_07_MCK4X_SEL_DQ5)
		| P_Fld(0x0, RG_ARDQ_REV_BIT_08_TX_ODT_DISABLE)
		| P_Fld(u1TX_READ_BASE_EN, RG_ARDQ_REV_BIT_09_TX_READ_BASE_EN)
		| P_Fld(0x0, RG_ARDQ_REV_BIT_1110_DRVN_PRE)
		| P_Fld(0x0, RG_ARDQ_REV_BIT_1312_DRVP_PRE)
		| P_Fld(0x0, RG_ARDQ_REV_BIT_14_TX_PRE_DATA_SEL)
		| P_Fld(0x0, RG_ARDQ_REV_BIT_15_TX_PRE_EN)
		| P_Fld(0x0, RG_ARDQ_REV_BIT_1716_TX_LP4Y_SEL)
		| P_Fld(0x0, RG_ARDQ_REV_BIT_18_RX_LP4Y_EN)
		| P_Fld(0x0, RG_ARDQ_REV_BIT_19_RX_DQSIEN_FORCE_ON_EN)
		| P_Fld(0x0, RG_ARDQ_REV_BIT_20_DATA_SWAP_EN)
		| P_Fld(0x0, RG_ARDQ_REV_BIT_2221_DATA_SWAP)
		| P_Fld(0x0, RG_ARDQ_REV_BIT_23_NA));
#if TX_DQ_PRE_EMPHASIS
	if ((p->frequency > 1600) && (gDramcSwImpedanceResule[ODT_ON][DRVP] > 0))
	{
		if (gDramcSwImpedanceResule[ODT_ON][DRVP] <= 7)
		{
			u1DRVING = 1;
		}
		else if (gDramcSwImpedanceResule[ODT_ON][DRVP] <= 15)
		{
			u1DRVING = 2;
		}
		else /* DRVP > 15 */
		{
			u1DRVING = 3;
		}

		vIO32WriteFldMulti_All(DDRPHY_SHU1_B0_DLL1, P_Fld(u1DRVING, RG_ARDQ_REV_BIT_1110_DRVN_PRE)
			| P_Fld(u1DRVING, RG_ARDQ_REV_BIT_1312_DRVP_PRE)
			| P_Fld(0x0, RG_ARDQ_REV_BIT_14_TX_PRE_DATA_SEL)
			| P_Fld(0x1, RG_ARDQ_REV_BIT_15_TX_PRE_EN));
		vIO32WriteFldMulti_All(DDRPHY_SHU1_B1_DLL1, P_Fld(u1DRVING, RG_ARDQ_REV_BIT_1110_DRVN_PRE)
			| P_Fld(u1DRVING, RG_ARDQ_REV_BIT_1312_DRVP_PRE)
			| P_Fld(0x0, RG_ARDQ_REV_BIT_14_TX_PRE_DATA_SEL)
			| P_Fld(0x1, RG_ARDQ_REV_BIT_15_TX_PRE_EN));

		mcSHOW_DBG_MSG(("[TX_DQ_PRE_EMPHASIS] Set pre - emphasis driving at %d\n", u1DRVING));
	}
#endif


#if 0
	/* LP4-Init */
	vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ3, 0x0, SHU1_B0_DQ3_RG_ARDQ_REV_B0);
	vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ3, 0x0, SHU1_B1_DQ3_RG_ARDQ_REV_B1);

	/* gating */
	vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ3, 0x20, SHU1_B0_DQ3_RG_ARDQ_REV_B0);
	vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ3, 0x20, SHU1_B1_DQ3_RG_ARDQ_REV_B1);
	vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD3, 0x09E0, SHU1_CA_CMD3_RG_ARCMD_REV);


	/* LP3-INIT */
	vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD3, 0x7e2, SHU1_CA_CMD3_RG_ARCMD_REV);
	vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD3 + SHIFT_TO_CHB_ADDR, 0x720, SHU1_CA_CMD3_RG_ARCMD_REV);
#endif
}

#if TX_OE_EXTEND
static void UpdateTxOEN(DRAMC_CTX_T *p)
{
	U8 u1ByteIdx, backup_rank, ii;
	U8 u1DQ_OE_CNT;

	/*
	 * For LP4
	 * 1. R_DMDQOE_OPT (dramc_conf 0x8C0[11])
	 * set 1'b1: adjust DQSOE/DQOE length with R_DMDQOE_CNT
	 * 2. R_DMDQOE_CNT (dramc_conf 0x8C0[10:8])
	 * set 3'h3
	 * 3. Initial TX setting OE/DATA
	 * OE = DATA - 4 UI
	 */

	/*
	 * For LP3
	 * 1. R_DMDQOE_OPT (dramc_conf 0x8C0[11])
	 * set 1'b1: adjust DQSOE/DQOE length with R_DMDQOE_CNT
	 * 2. R_DMDQOE_CNT (dramc_conf 0x8C0[10:8])
	 * set 3'h2
	 * 3. Initial TX setting OE/DATA
	 * OE = DATA - 2 UI
	 */


	if (u1IsLP4Family(p->dram_type))
		u1DQ_OE_CNT = 3;
	else
		u1DQ_OE_CNT = 2;

	vIO32WriteFldMulti(DRAMC_REG_SHU1_WODT, P_Fld(1, SHU1_WODT_DQOE_OPT) | P_Fld(u1DQ_OE_CNT, SHU1_WODT_DQOE_CNT));

	backup_rank = u1GetRank(p);
	for (u1ByteIdx = 0 ; u1ByteIdx < (p->data_width / DQS_BIT_NUMBER); u1ByteIdx++)
	{
		MoveDramC_TX_DQS_OEN(p, u1ByteIdx, - 1);

		for (ii = RANK_0; ii < RANK_MAX; ii++)
		{
			vSetRank(p, ii);
			MoveDramC_TX_DQ_OEN(p, u1ByteIdx, - 1);
		}
		vSetRank(p, backup_rank);
	}
}
#endif

#if CMD_PICG_NEW_MODE
void SetCmdPicgNewMode(DRAMC_CTX_T *p)
{
    U16 u2Clk_Dyn_Gating_Sel = 0x4, u2CG_CK_SEL = 0xb;

    if(!u1IsLP4Family(p->dram_type))//(vGet_Div_Mode(p)== DIV4_MODE) DIV4_MODE==>LP3
    {
        u2Clk_Dyn_Gating_Sel = 0x6;
        u2CG_CK_SEL = 0xd;
    }
    else //DIV8_MODE ==>LP4
    {
        u2Clk_Dyn_Gating_Sel = 0x5;
        u2CG_CK_SEL = 0xc;
    }

    vIO32WriteFldMulti(DRAMC_REG_SHU_APHY_TX_PICG_CTRL, P_Fld(1, SHU_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_OPT) | P_Fld(u2Clk_Dyn_Gating_Sel, SHU_APHY_TX_PICG_CTRL_DDRPHY_CLK_DYN_GATING_SEL));
    vIO32WriteFldMulti(DRAMC_REG_SHU_APHY_TX_PICG_CTRL, P_Fld(1, SHU_APHY_TX_PICG_CTRL_APHYPI_CG_CK_OPT) | P_Fld(u2CG_CK_SEL, SHU_APHY_TX_PICG_CTRL_APHYPI_CG_CK_SEL));
}
#endif

static DRAM_STATUS_T UpdateInitialSettings_LP4(DRAMC_CTX_T *p)
{
	U16 u2RXVrefDefault = 0x8;
	U16 u1ChannelIdx, u1RankIdx;
	U32 u4ImpcalChkCycle;

	if (p->odt_onoff == ODT_ON)
	{
		vIO32WriteFldAlign(DRAMC_REG_SHU_ODTCTRL, 1, SHU_ODTCTRL_ROEN);
		vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ7, 0x1, SHU1_B0_DQ7_R_DMRODTEN_B0);
		vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ7, 0x1, SHU1_B1_DQ7_R_DMRODTEN_B1);
		vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD0, P_Fld(0x0, SHU1_CA_CMD0_RG_TX_ARCMD_PRE_EN) /* OE Suspend EN */
			| P_Fld(0x1, SHU1_CA_CMD0_RG_TX_ARCLK_PRE_EN)); /* ODT Suspend EN */
	}
	else
	{
		vIO32WriteFldAlign(DRAMC_REG_SHU_ODTCTRL, 0, SHU_ODTCTRL_ROEN);
		vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ7, 0x0, SHU1_B0_DQ7_R_DMRODTEN_B0);
		vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ7, 0x0, SHU1_B1_DQ7_R_DMRODTEN_B1);
		vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD0, P_Fld(0x0, SHU1_CA_CMD0_RG_TX_ARCMD_PRE_EN) /* OE Suspend EN */
			| P_Fld(0x0, SHU1_CA_CMD0_RG_TX_ARCLK_PRE_EN)); /* ODT Suspend EN */
	}

	/* close RX DQ/DQS tracking to save power */
	vIO32WriteFldMulti(DDRPHY_R0_B0_RXDVS2, P_Fld(0x0, R0_B0_RXDVS2_R_RK0_DVS_MODE_B0)
		| P_Fld(0x0, R0_B0_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B0)
		| P_Fld(0x0, R0_B0_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B0));
	vIO32WriteFldMulti(DDRPHY_R1_B0_RXDVS2, P_Fld(0x0, R1_B0_RXDVS2_R_RK1_DVS_MODE_B0)
		| P_Fld(0x0, R1_B0_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_B0)
		| P_Fld(0x0, R1_B0_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_B0));
	vIO32WriteFldMulti(DDRPHY_R0_B1_RXDVS2, P_Fld(0x0, R0_B1_RXDVS2_R_RK0_DVS_MODE_B1)
		| P_Fld(0x0, R0_B1_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B1)
		| P_Fld(0x0, R0_B1_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B1));
	vIO32WriteFldMulti(DDRPHY_R1_B1_RXDVS2, P_Fld(0x0, R1_B1_RXDVS2_R_RK1_DVS_MODE_B1)
		| P_Fld(0x0, R1_B1_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_B1)
		| P_Fld(0x0, R1_B1_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_B1));
	/* wei-jen: RX rank_sel for CA is not used(6763), set it's dly to 0 to save power */
	vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD7, 0, SHU1_CA_CMD7_R_DMRANKRXDVS_CA);


	/* DDRPhyTxRxInitialSettings_LP4 */
	vIO32WriteFldAlign(DDRPHY_CA_CMD3, 0x1, CA_CMD3_RG_RX_ARCMD_STBENCMP_EN);

	vIO32WriteFldAlign(DDRPHY_CA_CMD10, 0x1, CA_CMD10_RG_RX_ARCLK_DQSIENMODE);

	vIO32WriteFldAlign(DDRPHY_CA_CMD6, 0x1, CA_CMD6_RG_RX_ARCMD_BIAS_VREF_SEL);

	vIO32WriteFldMulti(DDRPHY_B0_DQ3, P_Fld(0x1, B0_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B0)
		| P_Fld(0x1, B0_DQ3_RG_RX_ARDQM0_IN_BUFF_EN_B0)
		| P_Fld(0x1, B0_DQ3_RG_RX_ARDQS0_IN_BUFF_EN_B0));
	vIO32WriteFldMulti(DDRPHY_B1_DQ3, P_Fld(0x1, B1_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B1)
		| P_Fld(0x1, B1_DQ3_RG_RX_ARDQM0_IN_BUFF_EN_B1)
		| P_Fld(0x1, B1_DQ3_RG_RX_ARDQS0_IN_BUFF_EN_B1));
	vIO32WriteFldMulti(DDRPHY_CA_CMD3, P_Fld(0x1, CA_CMD3_RG_RX_ARCMD_IN_BUFF_EN)
		| P_Fld(0x1, CA_CMD3_RG_RX_ARCLK_IN_BUFF_EN));

	vIO32WriteFldAlign(DDRPHY_B0_DQ3, 0x0, B0_DQ3_RG_RX_ARDQ_SMT_EN_B0);
	vIO32WriteFldAlign(DDRPHY_B1_DQ3, 0x0, B1_DQ3_RG_RX_ARDQ_SMT_EN_B1);

	vIO32WriteFldAlign(DDRPHY_B0_DQ5, 0x1, B0_DQ5_RG_RX_ARDQS0_DVS_EN_B0);
	vIO32WriteFldAlign(DDRPHY_B1_DQ5, 0x1, B1_DQ5_RG_RX_ARDQS0_DVS_EN_B1);
	vIO32WriteFldAlign(DDRPHY_CA_CMD5, 0x1, CA_CMD5_RG_RX_ARCLK_DVS_EN);

	/*
	 * LP4 no need, follow LP3 first.
	 * vIO32WriteFldAlign(DDRPHY_MISC_VREF_CTRL, P_Fld(0x1, MISC_VREF_CTRL_RG_RVREF_DDR3_SEL)
	 * | P_Fld(0x0, MISC_VREF_CTRL_RG_RVREF_DDR4_SEL));
	 */


	vIO32WriteFldMulti(DDRPHY_CA_CMD6, P_Fld(0x0, CA_CMD6_RG_TX_ARCMD_DDR3_SEL)
		| P_Fld(0x1, CA_CMD6_RG_TX_ARCMD_DDR4_SEL)
		| P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_DDR3_SEL)
		| P_Fld(0x1, CA_CMD6_RG_RX_ARCMD_DDR4_SEL));
	vIO32WriteFldMulti(DDRPHY_CA_CMD6, P_Fld(0x0, CA_CMD6_RG_TX_ARCMD_DDR3_SEL)
		| P_Fld(0x1, CA_CMD6_RG_TX_ARCMD_DDR4_SEL)
		| P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_DDR3_SEL)
		| P_Fld(0x1, CA_CMD6_RG_RX_ARCMD_DDR4_SEL));
	vIO32WriteFldMulti(DDRPHY_MISC_IMP_CTRL0, P_Fld(0x0, MISC_IMP_CTRL0_RG_RIMP_DDR3_SEL)
		| P_Fld(0x1, MISC_IMP_CTRL0_RG_RIMP_DDR4_SEL));


	vIO32WriteFldAlign(DDRPHY_B0_DQ6, 0x1, B0_DQ6_RG_RX_ARDQ_O1_SEL_B0);
	vIO32WriteFldAlign(DDRPHY_B1_DQ6, 0x1, B1_DQ6_RG_RX_ARDQ_O1_SEL_B1);
	vIO32WriteFldAlign(DDRPHY_CA_CMD6, 0x1, CA_CMD6_RG_RX_ARCMD_O1_SEL);

	vIO32WriteFldAlign(DDRPHY_B0_DQ6, 0x1, B0_DQ6_RG_RX_ARDQ_BIAS_PS_B0);
	vIO32WriteFldAlign(DDRPHY_B1_DQ6, 0x1, B1_DQ6_RG_RX_ARDQ_BIAS_PS_B1);
	vIO32WriteFldAlign(DDRPHY_CA_CMD6, 0x1, CA_CMD6_RG_RX_ARCMD_BIAS_PS);

	vIO32WriteFldAlign(DDRPHY_CA_CMD6, 0x1, CA_CMD6_RG_RX_ARCMD_RES_BIAS_EN);

	vIO32WriteFldAlign(DDRPHY_B0_DQ6, 0x0, B0_DQ6_RG_TX_ARDQ_ODTEN_EXT_DIS_B0);
	vIO32WriteFldAlign(DDRPHY_B1_DQ6, 0x0, B1_DQ6_RG_TX_ARDQ_ODTEN_EXT_DIS_B1);
	vIO32WriteFldAlign(DDRPHY_CA_CMD6, 0x0, CA_CMD6_RG_TX_ARCMD_ODTEN_EXT_DIS);

	vIO32WriteFldAlign(DDRPHY_B0_DQ6, 0x1, B0_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B0);
	vIO32WriteFldAlign(DDRPHY_B1_DQ6, 0x1, B1_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B1);
	vIO32WriteFldAlign(DDRPHY_CA_CMD6, 0x1, CA_CMD6_RG_RX_ARCMD_RPRE_TOG_EN);


	if (p->dram_type == TYPE_LPDDR4)
	{
		if (p->odt_onoff == ODT_ON)
		{
			u2RXVrefDefault = 0xE;
		}
		else
		{
			u2RXVrefDefault = 0x16;
		}
	}
	else if (p->dram_type == TYPE_LPDDR4X)
	{
		if (p->odt_onoff == ODT_ON)
		{
			u2RXVrefDefault = 0xb;
		}
		else
		{
			u2RXVrefDefault = 0x16;
		}
	}
	else /* if (p->dram_type == TYPE_LPDDR4P) */
	{
		u2RXVrefDefault = 0x10;
	}

	vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ5, u2RXVrefDefault, SHU1_B0_DQ5_RG_RX_ARDQ_VREF_SEL_B0);
	vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5, u2RXVrefDefault, SHU1_B1_DQ5_RG_RX_ARDQ_VREF_SEL_B1);
	vIO32WriteFldAlign(DDRPHY_B0_DQ5, u2RXVrefDefault, B0_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B0);
	vIO32WriteFldAlign(DDRPHY_B1_DQ5, u2RXVrefDefault, B1_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B1);

	for (u1ChannelIdx = CHANNEL_A; u1ChannelIdx < CHANNEL_NUM; u1ChannelIdx++)
	{
		for (u1RankIdx = RANK_0; u1RankIdx < RANK_MAX; u1RankIdx++)
		{
			gFinalRXVrefDQ[u1ChannelIdx][u1RankIdx] = u2RXVrefDefault;
			gFinalRXVrefDQ[u1ChannelIdx][u1RankIdx] = u2RXVrefDefault;
		}
	}

	if ((p->dram_type == TYPE_LPDDR4X) || (p->dram_type == TYPE_LPDDR4P))
	{
		/* LP4x eye fine-tune */
		vIO32WriteFldMulti(DDRPHY_B0_DQ8, P_Fld(0x1, B0_DQ8_RG_TX_ARDQ_EN_LP4P_B0)
			| P_Fld(0x1, B0_DQ8_RG_TX_ARDQ_EN_CAP_LP4P_B0)
			| P_Fld(0x1, B0_DQ8_RG_TX_ARDQ_CAP_DET_B0));
		/*
		 * | P_Fld(0x1, B0_DQ8_RG_RX_ARDQS_DQSSTB_CG_EN_B0) // Field only exists for 10nm APHY
		 * corresponds to B0_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B0 for 16nm APHY
		 */
		vIO32WriteFldMulti(DDRPHY_B1_DQ8, P_Fld(0x1, B1_DQ8_RG_TX_ARDQ_EN_LP4P_B1)
			| P_Fld(0x1, B1_DQ8_RG_TX_ARDQ_EN_CAP_LP4P_B1)
			| P_Fld(0x1, B1_DQ8_RG_TX_ARDQ_CAP_DET_B1));
		/*
		 * | P_Fld(0x1, B1_DQ8_RG_RX_ARDQS_DQSSTB_CG_EN_B1) // Field only exists for 10nm APHY
		 * corresponds to B1_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B1 for 16nm APHY
		 */
		vIO32WriteFldMulti(DDRPHY_CA_CMD9, P_Fld(0x1, CA_CMD9_RG_TX_ARCMD_EN_LP4P)
			| P_Fld(0x1, CA_CMD9_RG_TX_ARCMD_EN_CAP_LP4P)
			| P_Fld(0x1, CA_CMD9_RG_TX_ARCMD_CAP_DET));
	}
	else
	{
		vIO32WriteFldMulti(DDRPHY_B0_DQ8, P_Fld(0x0, B0_DQ8_RG_TX_ARDQ_EN_LP4P_B0)
			| P_Fld(0x0, B0_DQ8_RG_TX_ARDQ_EN_CAP_LP4P_B0)
			| P_Fld(0x0, B0_DQ8_RG_TX_ARDQ_CAP_DET_B0));
		/*
		 * | P_Fld(0x1, B0_DQ8_RG_RX_ARDQS_DQSSTB_CG_EN_B0) // Field only exists for 10nm APHY
		 * corresponds to B0_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B0 for 16nm APHY
		 */
		vIO32WriteFldMulti(DDRPHY_B1_DQ8, P_Fld(0x0, B1_DQ8_RG_TX_ARDQ_EN_LP4P_B1)
			| P_Fld(0x0, B1_DQ8_RG_TX_ARDQ_EN_CAP_LP4P_B1)
			| P_Fld(0x0, B1_DQ8_RG_TX_ARDQ_CAP_DET_B1));
		/*
		 * | P_Fld(0x1, B1_DQ8_RG_RX_ARDQS_DQSSTB_CG_EN_B1) // Field only exists for 10nm APHY
		 * corresponds to B1_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B1 for 16nm APHY
		 */
		vIO32WriteFldMulti(DDRPHY_CA_CMD9, P_Fld(0x0, CA_CMD9_RG_TX_ARCMD_EN_LP4P)
			| P_Fld(0x0, CA_CMD9_RG_TX_ARCMD_EN_CAP_LP4P)
			| P_Fld(0x0, CA_CMD9_RG_TX_ARCMD_CAP_DET));
	}


	/* Set initial default mode to "new burst mode (7UI or new 8UI)" */
	DramcGatingMode(p, 1);

	vIO32WriteFldAlign(DDRPHY_CA_CMD8, 0x1, CA_CMD8_RG_TX_RRESETB_DDR3_SEL);
	vIO32WriteFldAlign(DDRPHY_CA_CMD8, 0x0, CA_CMD8_RG_TX_RRESETB_DDR4_SEL); /* TODO: Remove if register default value is 0 */
	/* End of DDRPhyTxRxInitialSettings_LP4 */

	/* DFS workaround */
	vIO32WriteFldAlign(DRAMC_REG_SHU_MISC, 0x2, SHU_MISC_REQQUE_MAXCNT);

	/* should set 0x2a, otherwise AC-timing violation from Berson */
	vIO32WriteFldMulti(DRAMC_REG_SHU1_DQSG, P_Fld(0x2a, SHU1_DQSG_SCINTV) | P_Fld(0x1, SHU1_DQSG_DQSINCTL_PRE_SEL));


	/* Update setting for 6763 */
	vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ5, 0x0, SHU1_B0_DQ5_RG_ARPI_FB_B0);
	vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5, 0x0, SHU1_B1_DQ5_RG_ARPI_FB_B1);
	vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD5, 0x0, SHU1_CA_CMD5_RG_ARPI_FB_CA);


	/* Reserved bits usage, check with PHY owners */
	DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
	vIO32WriteFldAlign_All(DDRPHY_SHU1_B0_DQ6, 0x0, SHU1_B0_DQ6_RG_ARPI_OFFSET_DQSIEN_B0);
	vIO32WriteFldAlign_All(DDRPHY_SHU1_B1_DQ6, 0x0, SHU1_B1_DQ6_RG_ARPI_OFFSET_DQSIEN_B1);
	vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_CMD6, 0x0, SHU1_CA_CMD6_RG_ARPI_OFFSET_CLKIEN);
	DramcBroadcastOnOff(DRAMC_BROADCAST_ON);

	/*
	 * IMP Tracking Init Settings
	 * Write (DRAMC _BASE+ 0x219) [31:0] = 32'h80080020//DDR3200 default
	 * SHU_IMPCAL1_IMPCAL_CHKCYCLE should > 12.5/MCK, 1:4 mode will disable imp tracking -> don't care
	 */
#if (fcFOR_CHIP_ID == fcTalbot) /* cc change after MP review */
	if (p->frequency >= 1600)
		u4ImpcalChkCycle = 0x7;
	else if (p->frequency >= 1200)
		u4ImpcalChkCycle = 0x4;
	else if (p->frequency >= 800)
		u4ImpcalChkCycle = 0x3;
	else
		u4ImpcalChkCycle = 0x0;

	vIO32WriteFldMulti(DRAMC_REG_SHU_IMPCAL1, P_Fld(8, SHU_IMPCAL1_IMPCAL_CALICNT) | P_Fld(0x10, SHU_IMPCAL1_IMPCALCNT) | P_Fld(4, SHU_IMPCAL1_IMPCAL_CALEN_CYCLE) | P_Fld(u4ImpcalChkCycle, SHU_IMPCAL1_IMPCAL_CHKCYCLE));
#endif

	/* for _K_ */
	vIO32WriteFldMulti(DRAMC_REG_SREFCTRL, P_Fld(0x1, SREFCTRL_SCSM_CGAR)
		| P_Fld(0x1, SREFCTRL_SCARB_SM_CGAR)
		| P_Fld(0x1, SREFCTRL_RDDQSOSC_CGAR)
		| P_Fld(0x1, SREFCTRL_HMRRSEL_CGAR));
	vIO32WriteFldAlign(DRAMC_REG_PRE_TDQSCK1, 0x1, PRE_TDQSCK1_TXUIPI_CAL_CGAR);
	/* DVFS related, PREA interval counter (After DVFS DVT, set to 0xf (originally was 0x1f)) */
	vIO32WriteFldAlign(DRAMC_REG_SHU_MISC, 0xf, SHU_MISC_PREA_INTV);
	vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ8, P_Fld(0x1, SHU1_B0_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B0)
		| P_Fld(0x1, SHU1_B0_DQ8_R_DMRANK_PIPE_CG_IG_B0)
		| P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B0)
		| P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B0)
		| P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B0)
		| P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B0)
		| P_Fld(0x1, SHU1_B0_DQ8_R_DMSTBEN_SYNC_CG_IG_B0)
		| P_Fld(0x1, SHU1_B0_DQ8_R_DMRXDLY_CG_IG_B0)
		| P_Fld(0x1, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B0)
		| P_Fld(0x1, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B0)
		| P_Fld(0x0, SHU1_B0_DQ8_R_DMRXDVS_UPD_FORCE_EN_B0)
		| P_Fld(0x7fff, SHU1_B0_DQ8_R_DMRXDVS_UPD_FORCE_CYC_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ8, P_Fld(0x1, SHU1_B1_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B1)
		| P_Fld(0x1, SHU1_B1_DQ8_R_DMRANK_PIPE_CG_IG_B1)
		| P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B1)
		| P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B1)
		| P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B1)
		| P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B1)
		| P_Fld(0x1, SHU1_B1_DQ8_R_DMSTBEN_SYNC_CG_IG_B1)
		| P_Fld(0x1, SHU1_B1_DQ8_R_DMRXDLY_CG_IG_B1)
		| P_Fld(0x1, SHU1_B1_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B1)
		| P_Fld(0x1, SHU1_B1_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B1)
		| P_Fld(0x0, SHU1_B1_DQ8_R_DMRXDVS_UPD_FORCE_EN_B1)
		| P_Fld(0x7fff, SHU1_B1_DQ8_R_DMRXDVS_UPD_FORCE_CYC_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD8, P_Fld(0x1, SHU1_CA_CMD8_R_DMRANK_CHG_PIPE_CG_IG_CA)
		| P_Fld(0x1, SHU1_CA_CMD8_R_DMRANK_PIPE_CG_IG_CA)
		| P_Fld(0x1, SHU1_CA_CMD8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_CA)
		| P_Fld(0x1, SHU1_CA_CMD8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_CA)
		| P_Fld(0x1, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_PIPE_CG_IG_CA)
		| P_Fld(0x1, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_SYNC_CG_IG_CA)
		| P_Fld(0x1, SHU1_CA_CMD8_R_DMSTBEN_SYNC_CG_IG_CA)
		| P_Fld(0x1, SHU1_CA_CMD8_R_DMRXDLY_CG_IG_CA)
		| P_Fld(0x1, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_CA)
		| P_Fld(0x1, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_PIPE_CG_IG_CA)
		| P_Fld(0x0, SHU1_CA_CMD8_R_DMRXDVS_UPD_FORCE_EN_CA)
		| P_Fld(0x7fff, SHU1_CA_CMD8_R_DMRXDVS_UPD_FORCE_CYC_CA));
	vIO32WriteFldAlign(DDRPHY_MISC_CTRL3, 0x1, MISC_CTRL3_R_DDRPHY_COMB_CG_IG);
	/* 6763 HW design issue: run-time PBYTE (B0, B1) flags will lose it's function and become per-bit -> set to 0 */
	vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7, P_Fld(0x0, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0)
		| P_Fld(0x0, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0)
		| P_Fld(0x0, SHU1_B0_DQ7_R_DMRXDVS_DQM_FLAGSEL_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ7, P_Fld(0x0, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1)
		| P_Fld(0x0, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1)
		| P_Fld(0x0, SHU1_B1_DQ7_R_DMRXDVS_DQM_FLAGSEL_B1));

	vIO32WriteFldMulti(DRAMC_REG_CLKAR, P_Fld(0x1, CLKAR_SELPH_CMD_CG_DIS) | P_Fld(0x7FFF, CLKAR_REQQUE_PACG_DIS));

	vIO32WriteFldAlign(DRAMC_REG_SHU_DQSG_RETRY, 0x0, SHU_DQSG_RETRY_R_RETRY_PA_DSIABLE); /* SH: Set to 0 -> save power */
	vIO32WriteFldAlign(DRAMC_REG_WRITE_LEV, 0x0, WRITE_LEV_DDRPHY_COMB_CG_SEL);
	vIO32WriteFldAlign(DRAMC_REG_DUMMY_RD, 0x1, DUMMY_RD_DUMMY_RD_PA_OPT);
	vIO32WriteFldMulti(DRAMC_REG_STBCAL2, P_Fld(0x0, STBCAL2_STB_UIDLYCG_IG)
		| P_Fld(0x0, STBCAL2_STB_PIDLYCG_IG));
	vIO32WriteFldMulti(DRAMC_REG_EYESCAN, P_Fld(0x1, EYESCAN_EYESCAN_DQS_SYNC_EN)
		| P_Fld(0x1, EYESCAN_EYESCAN_NEW_DQ_SYNC_EN)
		| P_Fld(0x1, EYESCAN_EYESCAN_DQ_SYNC_EN));
	vIO32WriteFldMulti(DRAMC_REG_SHU_ODTCTRL, P_Fld(0x1, SHU_ODTCTRL_RODTENSTB_SELPH_CG_IG)
		| P_Fld(0x1, SHU_ODTCTRL_RODTEN_SELPH_CG_IG));
#if 1/* #ifndef BIANCO_TO_BE_PORTING */
	vIO32WriteFldAlign(DDRPHY_SHU1_B0_DLL0, 0x1, SHU1_B0_DLL0_RG_ARPISM_MCK_SEL_B0_SHU);
	vIO32WriteFldAlign(DDRPHY_SHU1_B1_DLL0, 0x1, SHU1_B1_DLL0_RG_ARPISM_MCK_SEL_B1_SHU);
	/* vIO32WriteFldAlign(DDRPHY_SHU1_CA_DLL0, 0x1, SHU1_CA_DLL0_RG_ARPISM_MCK_SEL_CA_SHU); move to DramcSetting_Olympus_LP4_ByteMode() */

	vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI1, 0x1, CA_DLL_ARPI1_RG_ARPISM_MCK_SEL_CA);
#endif
	/* end _K_ */

	/* DE_UPDATE */
#if (fcFOR_CHIP_ID == fcTalbot)
	/* Must check with EMI owners -> Asynchronous EMI: Can't turn on RWSPLIT, Synchronous EMI: Can enable RWSPLIT (DE: JL Wu) */
	vIO32WriteFldMulti(DRAMC_REG_PERFCTL0, P_Fld(0x1, PERFCTL0_WRFIFO_OPT)
		| P_Fld(0x0, PERFCTL0_REORDEREN)
		| P_Fld(0x1, PERFCTL0_RWSPLIT)); /* synchronous EMI -> can turn on RWSPLIT */
#endif
#if FIX_CROSSRK_XRT_05T_OPT
	vIO32WriteFldAlign(DRAMC_REG_PERFCTL0, 0x0, PERFCTL0_XRT_05T_OPT);
#else
	vIO32WriteFldAlign(DRAMC_REG_PERFCTL0, 0x1, PERFCTL0_XRT_05T_OPT);
#endif

	vIO32WriteFldAlign(DRAMC_REG_SREFCTRL, 0x1, SREFCTRL_SREF2_OPTION);
	vIO32WriteFldAlign(DRAMC_REG_SHUCTRL1, 0x1a, SHUCTRL1_FC_PRDCNT);

#ifdef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
	vIO32WriteFldMulti(DDRPHY_B0_DQ6, P_Fld(0x1, B0_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B0)
		| P_Fld(0x1, B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0));
	vIO32WriteFldMulti(DDRPHY_B1_DQ6, P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B1)
		| P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1));
	vIO32WriteFldMulti(DDRPHY_CA_CMD6, P_Fld(0x1, CA_CMD6_RG_RX_ARCMD_OP_BIAS_SW_EN)
		| P_Fld(0x1, CA_CMD6_RG_RX_ARCMD_BIAS_EN));
	vIO32WriteFldAlign(DRAMC_REG_STBCAL2, 0x1, STBCAL2_STB_PICG_EARLY_1T_EN);
	vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7, P_Fld(0x0, SHU1_B0_DQ7_R_DMRXRANK_DQS_LAT_B0)
		| P_Fld(0x1, SHU1_B0_DQ7_R_DMRXRANK_DQS_EN_B0)
		| P_Fld(0x1, SHU1_B0_DQ7_R_DMRXRANK_DQ_LAT_B0)
		| P_Fld(0x1, SHU1_B0_DQ7_R_DMRXRANK_DQ_EN_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ7, P_Fld(0x0, SHU1_B1_DQ7_R_DMRXRANK_DQS_LAT_B1)
		| P_Fld(0x1, SHU1_B1_DQ7_R_DMRXRANK_DQS_EN_B1)
		| P_Fld(0x1, SHU1_B1_DQ7_R_DMRXRANK_DQ_LAT_B1)
		| P_Fld(0x1, SHU1_B1_DQ7_R_DMRXRANK_DQ_EN_B1));
#else
	vIO32WriteFldMulti(DDRPHY_B0_DQ6, P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B0)
		| P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0));
	vIO32WriteFldMulti(DDRPHY_B1_DQ6, P_Fld(0x0, B1_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B1)
		| P_Fld(0x0, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1));
	vIO32WriteFldMulti(DDRPHY_CA_CMD6, P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_OP_BIAS_SW_EN)
		| P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_BIAS_EN));
#endif

	/* end DE UPDATE */

	/* Disable RODT tracking */
	vIO32WriteFldAlign(DRAMC_REG_SHU_RODTENSTB, 0, SHU_RODTENSTB_RODTEN_MCK_MODESEL);

	/* Rx Gating tracking settings */
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DQSG), \
		P_Fld(9, SHU1_DQSG_STB_UPDMASKCYC) | \
		P_Fld(1, SHU1_DQSG_STB_UPDMASK_EN));
	vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQSCAL, P_Fld(0, SHURK0_DQSCAL_R0DQSIENLLMTEN) | P_Fld(0, SHURK0_DQSCAL_R0DQSIENHLMTEN));
	vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQSCAL, P_Fld(0, SHURK1_DQSCAL_R1DQSIENLLMTEN) | P_Fld(0, SHURK1_DQSCAL_R1DQSIENHLMTEN));
	vIO32WriteFldMulti(DRAMC_REG_SHU_STBCAL, P_Fld(1, SHU_STBCAL_DQSG_MODE) | P_Fld(1, SHU_STBCAL_PICGLAT));

#ifdef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
	vIO32WriteFldAlign(DDRPHY_B0_DQ9, 0x4, B0_DQ9_R_IN_GATE_EN_LOW_OPT_B0);
	vIO32WriteFldAlign(DDRPHY_B1_DQ9, 0x4, B1_DQ9_R_IN_GATE_EN_LOW_OPT_B1);
#else
	/* Modify for corner IC failed at HQA test XTLV */
	vIO32WriteFldAlign(DDRPHY_B0_DQ9, 0x7, B0_DQ9_R_IN_GATE_EN_LOW_OPT_B0);
	vIO32WriteFldAlign(DDRPHY_B1_DQ9, 0x7, B1_DQ9_R_IN_GATE_EN_LOW_OPT_B1);
#endif
	vIO32WriteFldAlign(DDRPHY_CA_CMD10, 0x0, CA_CMD10_R_IN_GATE_EN_LOW_OPT_CA);

	vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ8, 0x1, SHU1_B0_DQ8_R_DMRXDLY_CG_IG_B0);
	vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ8, 0x1, SHU1_B1_DQ8_R_DMRXDLY_CG_IG_B1);

#ifdef DUMMY_READ_FOR_DQS_GATING_RETRY
	if (p->support_rank_num == RANK_SINGLE)
	{
		vIO32WriteFldAlign(DRAMC_REG_SHU_DQSG_RETRY, 1, SHU_DQSG_RETRY_R_RETRY_1RANK);
	}
#endif


#if ENABLE_TX_WDQS
	mcSHOW_DBG_MSG(("Enable WDQS\n"));
	/* Check reserved bits with PHY integrator */
	vIO32WriteFldMulti(DDRPHY_SHU1_B0_DLL1, P_Fld(1, RG_ARDQ_REV_BIT_09_TX_READ_BASE_EN) | P_Fld(1, RG_ARDQ_REV_BIT_02_TX_READ_BASE_EN_DQSB)
		| P_Fld(!p->odt_onoff, RG_ARDQ_REV_BIT_08_TX_ODT_DISABLE));
	vIO32WriteFldMulti(DDRPHY_SHU1_B1_DLL1, P_Fld(1, RG_ARDQ_REV_BIT_09_TX_READ_BASE_EN) | P_Fld(1, RG_ARDQ_REV_BIT_02_TX_READ_BASE_EN_DQSB)
		| P_Fld(!p->odt_onoff, RG_ARDQ_REV_BIT_08_TX_ODT_DISABLE));
	vIO32WriteFldMulti(DRAMC_REG_SHU_ODTCTRL, P_Fld(0x1, SHU_ODTCTRL_RODTE)
		| P_Fld(0x1, SHU_ODTCTRL_RODTE2)
		| P_Fld(0x1, SHU_ODTCTRL_ROEN));

	vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ7, 0x1, SHU1_B0_DQ7_R_DMRODTEN_B0);
	vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ7, 0x1, SHU1_B1_DQ7_R_DMRODTEN_B1);
#if ENABLE_RODT_TRACKING_SAVE_MCK
	SetTxWDQSStatusOnOff(1);
#endif

#else /* WDQS and reak pull are disable */
	/* Check reserved bits with PHY integrator */
	vIO32WriteFldMulti(DDRPHY_SHU1_B0_DLL1, P_Fld(0, RG_ARDQ_REV_BIT_09_TX_READ_BASE_EN) | P_Fld(0, RG_ARDQ_REV_BIT_02_TX_READ_BASE_EN_DQSB)
		| P_Fld(0, RG_ARDQ_REV_BIT_08_TX_ODT_DISABLE));
	vIO32WriteFldMulti(DDRPHY_SHU1_B1_DLL1, P_Fld(0, RG_ARDQ_REV_BIT_09_TX_READ_BASE_EN) | P_Fld(0, RG_ARDQ_REV_BIT_02_TX_READ_BASE_EN_DQSB)
		| P_Fld(0, RG_ARDQ_REV_BIT_08_TX_ODT_DISABLE));
#endif


	/* DE review WhitneyE2 */
	vIO32WriteFldAlign(DRAMC_REG_DRSCTRL, 0x1, DRSCTRL_DRS_SELFWAKE_DMYRD_DIS);
	vIO32WriteFldAlign(DRAMC_REG_REFCTRL0, 0x1, REFCTRL0_REFNA_OPT);
	vIO32WriteFldAlign(DRAMC_REG_ZQCS, 0x1, ZQCS_ZQCS_MASK_SEL_CGAR);
	vIO32WriteFldMulti(DRAMC_REG_DUMMY_RD, P_Fld(0x1, DUMMY_RD_DMYRD_REORDER_DIS) | P_Fld(0x0, DUMMY_RD_DMYRD_HPRI_DIS));
	vIO32WriteFldAlign(DRAMC_REG_SHUCTRL2, 0x1, SHUCTRL2_R_DVFS_SREF_OPT);
	vIO32WriteFldAlign(DRAMC_REG_SHUCTRL3, 0xb, SHUCTRL3_VRCGDIS_PRDCNT);
	vIO32WriteFldAlign(DDRPHY_MISC_CTRL3, 0x1, MISC_CTRL3_R_DDRPHY_RX_PIPE_CG_IG);
	/* End */

	/* DE review 6763 */
	/*
	 * ARPISM_MCK_SEL_B0, B1 set to 1 (Joe): "Due to TX_PICG modify register is set to 1,
	 * ARPISM_MCK_SEL_Bx should be 1 to fulfill APHY TX OE spec for low freq (Ex: DDR1600)"
	 */
	vIO32WriteFldMulti(DDRPHY_B0_DLL_ARPI1, P_Fld(0x1, B0_DLL_ARPI1_RG_ARPISM_MCK_SEL_B0_REG_OPT)
		| P_Fld(0x1, B0_DLL_ARPI1_RG_ARPISM_MCK_SEL_B0));
	vIO32WriteFldMulti(DDRPHY_B1_DLL_ARPI1, P_Fld(0x1, B1_DLL_ARPI1_RG_ARPISM_MCK_SEL_B1_REG_OPT)
		| P_Fld(0x1, B1_DLL_ARPI1_RG_ARPISM_MCK_SEL_B1));
	vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI1, 0x1, CA_DLL_ARPI1_RG_ARPISM_MCK_SEL_CA_REG_OPT);
	vIO32WriteFldAlign(DDRPHY_MISC_CTRL0, 0, MISC_CTRL0_R_DMSHU_PHYDCM_FORCEOFF);
	vIO32WriteFldAlign(DDRPHY_MISC_RXDVS2, 1, MISC_RXDVS2_R_DMRXDVS_SHUFFLE_CTRL_CG_IG);
	vIO32WriteFldAlign(DRAMC_REG_CLKCTRL, 0x1, CLKCTRL_SEQCLKRUN3);
	vIO32WriteFldAlign(DRAMC_REG_REFCTRL1, 1, REFCTRL1_SREF_CG_OPT);
	vIO32WriteFldMulti(DRAMC_REG_SHUCTRL, P_Fld(0x0, SHUCTRL_DVFS_CG_OPT) | P_Fld(0x3, SHUCTRL_R_DVFS_PICG_MARGIN2) | P_Fld(0x3, SHUCTRL_R_DVFS_PICG_MARGIN3));
	vIO32WriteFldMulti(DRAMC_REG_SHUCTRL2, P_Fld(0x1, SHUCTRL2_SHORTQ_OPT) | P_Fld(0x3, SHUCTRL2_R_DVFS_PICG_MARGIN));
	vIO32WriteFldAlign(DRAMC_REG_STBCAL2, 0x0, STBCAL2_STB_DBG_EN);
	vIO32WriteFldMulti(DRAMC_REG_PRE_TDQSCK1, P_Fld(0x0, PRE_TDQSCK1_APHY_CG_OPT1) | P_Fld(0x0, PRE_TDQSCK1_SHU_PRELOAD_TX_HW));

#ifndef FIRST_BRING_UP
	if (u2DFSGetHighestFreq(p) >= 1866)
#endif
	{
		/* if product supports 3733, CLKAR_SELPH_4LCG_DIS always 1 else o, but if 1, comsume more power */
		vIO32WriteFldAlign(DRAMC_REG_CLKAR, 1, CLKAR_SELPH_4LCG_DIS);
	}

#if TX_OE_EXTEND
	UpdateTxOEN(p);
#endif

	vIO32WriteFldAlign(DRAMC_REG_CKECTRL, 0x1, CKECTRL_CKEPBDIS);

	vIO32WriteFldMulti(DDRPHY_CA_TX_MCK, P_Fld(0x1, CA_TX_MCK_R_DMRESET_FRPHY_OPT) | P_Fld(0xa, CA_TX_MCK_R_DMRESETB_DRVP_FRPHY) | P_Fld(0xa, CA_TX_MCK_R_DMRESETB_DRVN_FRPHY));

	/* 6765 MP setting should set CKECTRL_CKELCKFIX = 0 as default, after DVT certification pass */
	vIO32WriteFldAlign(DRAMC_REG_CKECTRL, 0x0, CKECTRL_CKELCKFIX);

	/* Gating error problem happened in M17 has been solved by setting this RG as 0 (when RODT tracking on (6763), TX DLY of byte2,3 must not be zero) */
	vIO32WriteFldAlign(DRAMC_REG_SHU_RODTENSTB, 0, SHU_RODTENSTB_RODTENSTB_4BYTE_EN);

#if ENABLE_RODT_TRACKING /* The SHU_RODTENSTB_RODTENSTB_TRACK_EN will depend on SHU_ODTCTRL_ROEN setting */
	U8 u1ReadROEN;
	u1ReadROEN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_ODTCTRL), SHU_ODTCTRL_ROEN);
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_RODTENSTB), \
		P_Fld(0xff, SHU_RODTENSTB_RODTENSTB_EXT) | \
		P_Fld(9, SHU_RODTENSTB_RODTENSTB_OFFSET) | \
		P_Fld(u1ReadROEN, SHU_RODTENSTB_RODTEN_MCK_MODESEL));
#endif

#if ENABLE_TMRRI_NEW_MODE
	/* [6765 DVT](1)dram auto refersh rate by hardware mr4 for rank0 and rank1; (2)After suspend resume, HW MR4 will be fire autoly (Berson) */
	vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, (p->support_rank_num == RANK_DUAL) ? (1) : (0), SPCMDCTRL_HMR4_TOG_OPT);
#else
	vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x0, SPCMDCTRL_HMR4_TOG_OPT);
#endif

#if CMD_PICG_NEW_MODE
    SetCmdPicgNewMode(p);
#endif

	/* [6765 DVT]RX FIFO debug feature, MP setting should enable debug function */
	vIO32WriteFldAlign(DDRPHY_B0_DQ9, 0x1, B0_DQ9_R_DMRXFIFO_STBENCMP_EN_B0);
	vIO32WriteFldAlign(DDRPHY_B1_DQ9, 0x1, B1_DQ9_R_DMRXFIFO_STBENCMP_EN_B1);

#if CBT_MOVE_CA_INSTEAD_OF_CLK
	/* CA shift -1*UI and + 32 PI delay */
	vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_CA7, P_Fld(0x0, SHU_SELPH_CA7_DLY_RA0) |
		P_Fld(0x0, SHU_SELPH_CA7_DLY_RA1) |
		P_Fld(0x0, SHU_SELPH_CA7_DLY_RA2) |
		P_Fld(0x0, SHU_SELPH_CA7_DLY_RA3) |
		P_Fld(0x0, SHU_SELPH_CA7_DLY_RA4) |
		P_Fld(0x0, SHU_SELPH_CA7_DLY_RA5));

	// Note: CKE UI must sync CA UI (CA and CKE delay circuit are same) @Lin-Yi
	// To avoid tXP timing margin issue
	vIO32WriteFldAlign(DRAMC_REG_SHU_SELPH_CA5, 0x0, SHU_SELPH_CA5_DLY_CKE);
	vIO32WriteFldAlign(DRAMC_REG_SHU_SELPH_CA6, 0x0, SHU_SELPH_CA6_DLY_CKE1);

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), 0x20, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CMD);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R1_CA_CMD9), 0x20, SHU1_R1_CA_CMD9_RG_RK1_ARPI_CMD);
#endif

	/* [MT6768 DVT] Add new settings for DVT required */
#ifdef BYPASS_TXPIPE_EN
	if (p->frequency >= 1600) {
		vIO32WriteFldMulti(DDRPHY_SHU1_MISC0,
			P_Fld(0x1, SHU1_MISC0_RG_CK_TXPIPE_BYPASS_EN) |
			P_Fld(0x1, SHU1_MISC0_RG_CMD_TXPIPE_BYPASS_EN));
	}
#endif

#ifdef BYPASS_RXPIPE_EN
	vIO32WriteFldAlign(DDRPHY_SHU1_MISC0, 0x1, SHU1_MISC0_R_RX_PIPE_BYPASS_EN);
#endif

	/* MT6768 add to fix short pulse */
	vIO32WriteFldMulti(DRAMC_REG_DVFSDLL,
		P_Fld(0x1, DVFSDLL_R_BYPASS_1ST_DLL_SHU4) |
		P_Fld(0x1, DVFSDLL_R_DMSHUFFLE_CHANGE_FREQ_OPT) |
		P_Fld(0x2, DVFSDLL_R_DVFS_DLL_MARGIN));


	return DRAM_OK;
}


#if ENABLE_LP3_SW
static DRAM_STATUS_T UpdateInitialSettings_LP3(DRAMC_CTX_T *p)
{

	/* close RX DQ/DQS tracking to save power */
	vIO32WriteFldMulti_All(DDRPHY_R0_B1_RXDVS2, P_Fld(0x0, R0_B1_RXDVS2_R_RK0_DVS_MODE_B1)
		| P_Fld(0x0, R0_B1_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B1)
		| P_Fld(0x0, R0_B1_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B1));
	vIO32WriteFldMulti_All(DDRPHY_R1_B1_RXDVS2, P_Fld(0x0, R1_B1_RXDVS2_R_RK1_DVS_MODE_B1)
		| P_Fld(0x0, R1_B1_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_B1)
		| P_Fld(0x0, R1_B1_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_B1));
	vIO32WriteFldMulti(DDRPHY_R0_B0_RXDVS2 + SHIFT_TO_CHB_ADDR, P_Fld(0x0, R0_B0_RXDVS2_R_RK0_DVS_MODE_B0)
		| P_Fld(0x0, R0_B0_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B0)
		| P_Fld(0x0, R0_B0_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B0));
	vIO32WriteFldMulti(DDRPHY_R1_B0_RXDVS2 + SHIFT_TO_CHB_ADDR, P_Fld(0x0, R1_B0_RXDVS2_R_RK1_DVS_MODE_B0)
		| P_Fld(0x0, R1_B0_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_B0)
		| P_Fld(0x0, R1_B0_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_B0));
	vIO32WriteFldMulti(DDRPHY_R0_CA_RXDVS2 + SHIFT_TO_CHB_ADDR, P_Fld(0x0, R0_CA_RXDVS2_R_RK0_DVS_MODE_CA)
		| P_Fld(0x0, R0_CA_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_CA)
		| P_Fld(0x0, R0_CA_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_CA));
	vIO32WriteFldMulti(DDRPHY_R1_CA_RXDVS2 + SHIFT_TO_CHB_ADDR, P_Fld(0x0, R1_CA_RXDVS2_R_RK1_DVS_MODE_CA)
		| P_Fld(0x0, R1_CA_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_CA)
		| P_Fld(0x0, R1_CA_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_CA));
#if 0 /* Use _All() instead */
	vIO32WriteFldMulti(DDRPHY_R0_B1_RXDVS2 + SHIFT_TO_CHB_ADDR, P_Fld(0x0, R0_B1_RXDVS2_R_RK0_DVS_MODE_B1)
		| P_Fld(0x0, R0_B1_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B1)
		| P_Fld(0x0, R0_B1_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B1));
	vIO32WriteFldMulti(DDRPHY_R1_B1_RXDVS2 + SHIFT_TO_CHB_ADDR, P_Fld(0x0, R1_B1_RXDVS2_R_RK1_DVS_MODE_B1)
		| P_Fld(0x0, R1_B1_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_B1)
		| P_Fld(0x0, R1_B1_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_B1));
#endif


	/* DDRPhyTxRxInitialSettings_LP3 */
	vIO32WriteFldMulti_All(DDRPHY_B0_DQ6, P_Fld(0x1, B0_DQ6_RG_TX_ARDQ_DDR3_SEL_B0)
		| P_Fld(0x1, B0_DQ6_RG_RX_ARDQ_DDR3_SEL_B0)
		| P_Fld(0x0, B0_DQ6_RG_TX_ARDQ_DDR4_SEL_B0)
		| P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_DDR4_SEL_B0));
	vIO32WriteFldMulti_All(DDRPHY_B1_DQ6, P_Fld(0x1, B1_DQ6_RG_TX_ARDQ_DDR3_SEL_B1)
		| P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_DDR3_SEL_B1)
		| P_Fld(0x0, B1_DQ6_RG_TX_ARDQ_DDR4_SEL_B1)
		| P_Fld(0x0, B1_DQ6_RG_RX_ARDQ_DDR4_SEL_B1));
	vIO32WriteFldMulti_All(DDRPHY_CA_CMD6, P_Fld(0x1, CA_CMD6_RG_TX_ARCMD_DDR3_SEL)
		| P_Fld(0x1, CA_CMD6_RG_RX_ARCMD_DDR3_SEL)
		| P_Fld(0x0, CA_CMD6_RG_TX_ARCMD_DDR4_SEL)
		| P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_DDR4_SEL));

	vIO32WriteFldAlign_All(DDRPHY_B0_DQ8, 0x0, B0_DQ8_RG_TX_ARDQ_EN_LP4P_B0);
	vIO32WriteFldAlign_All(DDRPHY_B1_DQ8, 0x0, B1_DQ8_RG_TX_ARDQ_EN_LP4P_B1);
	vIO32WriteFldAlign_All(DDRPHY_CA_CMD9, 0x0, CA_CMD9_RG_TX_ARCMD_EN_LP4P);

	vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ5), 0, B0_DQ5_RG_RX_ARDQ_EYE_VREF_EN_B0);
	vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ5), 0, B1_DQ5_RG_RX_ARDQ_EYE_VREF_EN_B1);
	vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_CA_CMD5), 0, CA_CMD5_RG_RX_ARCMD_EYE_VREF_EN);

	vIO32WriteFldMulti_All(DDRPHY_CA_CMD8, P_Fld(0x1, CA_CMD8_RG_TX_RRESETB_DDR3_SEL)
		| P_Fld(0x0, CA_CMD8_RG_TX_RRESETB_DDR4_SEL)); /* TODO: Remove if register default value is 0 */

	vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ5), 0xb, B0_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B0);
	vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ5), 0xb, B1_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B1);
	vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_CA_CMD5), 0xb, CA_CMD5_RG_RX_ARCMD_EYE_VREF_SEL);

	vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ5), 0xb, SHU1_B0_DQ5_RG_RX_ARDQ_VREF_SEL_B0);
	vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DQ5), 0xb, SHU1_B1_DQ5_RG_RX_ARDQ_VREF_SEL_B1);
	vIO32WriteFldAlign_Phy_All(DRAMC_REG_ADDR(DDRPHY_SHU1_CA_CMD5), 0xb, SHU1_CA_CMD5_RG_RX_ARCMD_VREF_SEL);

#if (fcFOR_CHIP_ID == fcTalbot) /* ChA - B1, ChB - B0 / B1 / CA are used as RX */
	vIO32WriteFldAlign(DDRPHY_B0_DQ6, 0x0, B0_DQ6_RG_RX_ARDQ_RES_BIAS_EN_B0);
	vIO32WriteFldAlign(DDRPHY_B0_DQ6 + SHIFT_TO_CHB_ADDR, 0x1, B0_DQ6_RG_RX_ARDQ_RES_BIAS_EN_B0);
	vIO32WriteFldAlign_All(DDRPHY_B1_DQ6, 0x1, B1_DQ6_RG_RX_ARDQ_RES_BIAS_EN_B1);
	vIO32WriteFldAlign(DDRPHY_CA_CMD6, 0x0, CA_CMD6_RG_RX_ARCMD_RES_BIAS_EN);
	vIO32WriteFldAlign(DDRPHY_CA_CMD6 + SHIFT_TO_CHB_ADDR, 0x1, CA_CMD6_RG_RX_ARCMD_RES_BIAS_EN);
#endif

	vIO32WriteFldAlign_All(DDRPHY_CA_CMD6, 0x1, CA_CMD6_RG_RX_ARCMD_RES_BIAS_EN);

	vIO32WriteFldAlign_Phy_All(DDRPHY_B0_DQ6, 0x0, B0_DQ6_RG_RX_ARDQ_O1_SEL_B0);
	vIO32WriteFldAlign_Phy_All(DDRPHY_B1_DQ6, 0x0, B1_DQ6_RG_RX_ARDQ_O1_SEL_B1);
	vIO32WriteFldAlign_Phy_All(DDRPHY_CA_CMD6, 0x0, CA_CMD6_RG_RX_ARCMD_O1_SEL);

	vIO32WriteFldAlign_Phy_All(DDRPHY_B0_DQ6, 0x0, B0_DQ6_RG_TX_ARDQ_ODTEN_EXT_DIS_B0);
	vIO32WriteFldAlign_Phy_All(DDRPHY_B1_DQ6, 0x0, B1_DQ6_RG_TX_ARDQ_ODTEN_EXT_DIS_B1);
	vIO32WriteFldAlign_Phy_All(DDRPHY_CA_CMD6, 0x0, CA_CMD6_RG_TX_ARCMD_ODTEN_EXT_DIS);

	/* CH_A B0/CA = 0 */
	vIO32WriteFldMulti(DDRPHY_B0_DQ3, P_Fld(0x0, B0_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B0)
		| P_Fld(0x0, B0_DQ3_RG_RX_ARDQM0_IN_BUFF_EN_B0)
		| P_Fld(0x0, B0_DQ3_RG_RX_ARDQS0_IN_BUFF_EN_B0));
	vIO32WriteFldMulti(DDRPHY_B0_DQ3 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, B0_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B0)
		| P_Fld(0x1, B0_DQ3_RG_RX_ARDQM0_IN_BUFF_EN_B0)
		| P_Fld(0x1, B0_DQ3_RG_RX_ARDQS0_IN_BUFF_EN_B0));

	vIO32WriteFldMulti_All(DDRPHY_B1_DQ3, P_Fld(0x1, B1_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B1)
		| P_Fld(0x1, B1_DQ3_RG_RX_ARDQM0_IN_BUFF_EN_B1)
		| P_Fld(0x1, B1_DQ3_RG_RX_ARDQS0_IN_BUFF_EN_B1));

	vIO32WriteFldMulti(DDRPHY_CA_CMD3, P_Fld(0x0, CA_CMD3_RG_RX_ARCMD_IN_BUFF_EN)
		| P_Fld(0x0, CA_CMD3_RG_RX_ARCLK_IN_BUFF_EN));
	vIO32WriteFldMulti(DDRPHY_CA_CMD3 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, CA_CMD3_RG_RX_ARCMD_IN_BUFF_EN)
		| P_Fld(0x1, CA_CMD3_RG_RX_ARCLK_IN_BUFF_EN));

	vIO32WriteFldAlign(DDRPHY_B0_DQ9, 0, B0_DQ9_R_IN_GATE_EN_LOW_OPT_B0);
	vIO32WriteFldAlign(DDRPHY_CA_CMD10, 0, CA_CMD10_R_IN_GATE_EN_LOW_OPT_CA);
#ifdef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
	vIO32WriteFldAlign(DDRPHY_B1_DQ9, 0x04, B1_DQ9_R_IN_GATE_EN_LOW_OPT_B1);
	vIO32WriteFldAlign(DDRPHY_B0_DQ9 + SHIFT_TO_CHB_ADDR, 0x04, B0_DQ9_R_IN_GATE_EN_LOW_OPT_B0);
	vIO32WriteFldAlign(DDRPHY_B1_DQ9 + SHIFT_TO_CHB_ADDR, 0x04, B1_DQ9_R_IN_GATE_EN_LOW_OPT_B1);
	vIO32WriteFldAlign(DDRPHY_CA_CMD10 + SHIFT_TO_CHB_ADDR, 0x04, CA_CMD10_R_IN_GATE_EN_LOW_OPT_CA);
#else
	vIO32WriteFldAlign(DDRPHY_B1_DQ9, 0x7, B1_DQ9_R_IN_GATE_EN_LOW_OPT_B1);
	vIO32WriteFldAlign(DDRPHY_B0_DQ9 + SHIFT_TO_CHB_ADDR, 0x7, B0_DQ9_R_IN_GATE_EN_LOW_OPT_B0);
	vIO32WriteFldAlign(DDRPHY_B1_DQ9 + SHIFT_TO_CHB_ADDR, 0x7, B1_DQ9_R_IN_GATE_EN_LOW_OPT_B1);
	vIO32WriteFldAlign(DDRPHY_CA_CMD10 + SHIFT_TO_CHB_ADDR, 0x7, CA_CMD10_R_IN_GATE_EN_LOW_OPT_CA);
#endif
	/* End of DDRPhyTxRxInitialSettings_LP3 */

	/* DFS workaround */
	vIO32WriteFldAlign_All(DRAMC_REG_SHU_MISC, 0x2, SHU_MISC_REQQUE_MAXCNT);

	/* should set 0x2a, otherwise AC-timing violation from Berson */
	vIO32WriteFldAlign_All(DRAMC_REG_SHU1_DQSG, 0x2a, SHU1_DQSG_SCINTV);

	/* for _K_ */
	vIO32WriteFldMulti(DRAMC_REG_SREFCTRL, P_Fld(0x1, SREFCTRL_SCSM_CGAR)
		| P_Fld(0x1, SREFCTRL_SCARB_SM_CGAR)
		| P_Fld(0x1, SREFCTRL_RDDQSOSC_CGAR)
		| P_Fld(0x1, SREFCTRL_HMRRSEL_CGAR));

	vIO32WriteFldAlign(DRAMC_REG_PRE_TDQSCK1, 0x1, PRE_TDQSCK1_TXUIPI_CAL_CGAR);
	/* DVFS related, PREA interval counter (After DVFS DVT, set to 0xf (originally was 0x1f)) */
	vIO32WriteFldAlign(DRAMC_REG_SHU_MISC, 0xf, SHU_MISC_PREA_INTV);
#if 0 /* Correct value is set in PllSettings() */
	vIO32WriteFldAlign(DRAMC_REG_DVFSDLL, 0x0, DVFSDLL_R_BYPASS_1ST_DLL_SHU1);
	vIO32WriteFldAlign(DRAMC_REG_DVFSDLL, 0x0, DVFSDLL_R_BYPASS_1ST_DLL_SHU2);
	vIO32WriteFldAlign(DRAMC_REG_DVFSDLL, 0x0, DVFSDLL_R_BYPASS_1ST_DLL_SHU3);
#endif
	vIO32WriteFldMulti_All(DDRPHY_SHU1_B0_DQ8, P_Fld(0x1, SHU1_B0_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B0)
		| P_Fld(0x1, SHU1_B0_DQ8_R_DMRANK_PIPE_CG_IG_B0)
		| P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B0)
		| P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B0)
		| P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B0)
		| P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B0)
		| P_Fld(0x1, SHU1_B0_DQ8_R_DMSTBEN_SYNC_CG_IG_B0)
		| P_Fld(0x1, SHU1_B0_DQ8_R_DMRXDLY_CG_IG_B0)
		| P_Fld(0x1, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B0)
		| P_Fld(0x1, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B0)
		| P_Fld(0x0, SHU1_B0_DQ8_R_DMRXDVS_UPD_FORCE_EN_B0)
		| P_Fld(0x7fff, SHU1_B0_DQ8_R_DMRXDVS_UPD_FORCE_CYC_B0));
#if 0 /* Use _All() instead */
	vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ8 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, SHU1_B0_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B0)
		| P_Fld(0x1, SHU1_B0_DQ8_R_DMRANK_PIPE_CG_IG_B0)
		| P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B0)
		| P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B0)
		| P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B0)
		| P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B0)
		| P_Fld(0x1, SHU1_B0_DQ8_R_DMSTBEN_SYNC_CG_IG_B0)
		| P_Fld(0x1, SHU1_B0_DQ8_R_DMRXDLY_CG_IG_B0)
		| P_Fld(0x1, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B0)
		| P_Fld(0x1, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B0)
		| P_Fld(0x0, SHU1_B0_DQ8_R_DMRXDVS_UPD_FORCE_EN_B0)
		| P_Fld(0x7fff, SHU1_B0_DQ8_R_DMRXDVS_UPD_FORCE_CYC_B0));
#endif
	vIO32WriteFldMulti_All(DDRPHY_SHU1_B1_DQ8, P_Fld(0x1, SHU1_B1_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B1)
		| P_Fld(0x1, SHU1_B1_DQ8_R_DMRANK_PIPE_CG_IG_B1)
		| P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B1)
		| P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B1)
		| P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B1)
		| P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B1)
		| P_Fld(0x1, SHU1_B1_DQ8_R_DMSTBEN_SYNC_CG_IG_B1)
		| P_Fld(0x1, SHU1_B1_DQ8_R_DMRXDLY_CG_IG_B1)
		| P_Fld(0x1, SHU1_B1_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B1)
		| P_Fld(0x1, SHU1_B1_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B1)
		| P_Fld(0x0, SHU1_B1_DQ8_R_DMRXDVS_UPD_FORCE_EN_B1)
		| P_Fld(0x7fff, SHU1_B1_DQ8_R_DMRXDVS_UPD_FORCE_CYC_B1));
#if 0 /* Use _All() instead */
	vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ8 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, SHU1_B1_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B1)
		| P_Fld(0x1, SHU1_B1_DQ8_R_DMRANK_PIPE_CG_IG_B1)
		| P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B1)
		| P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B1)
		| P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B1)
		| P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B1)
		| P_Fld(0x1, SHU1_B1_DQ8_R_DMSTBEN_SYNC_CG_IG_B1)
		| P_Fld(0x1, SHU1_B1_DQ8_R_DMRXDLY_CG_IG_B1)
		| P_Fld(0x1, SHU1_B1_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B1)
		| P_Fld(0x1, SHU1_B1_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B1)
		| P_Fld(0x0, SHU1_B1_DQ8_R_DMRXDVS_UPD_FORCE_EN_B1)
		| P_Fld(0x7fff, SHU1_B1_DQ8_R_DMRXDVS_UPD_FORCE_CYC_B1));
#endif
	vIO32WriteFldMulti_All(DDRPHY_SHU1_CA_CMD8, P_Fld(0x1, SHU1_CA_CMD8_R_DMRANK_CHG_PIPE_CG_IG_CA)
		| P_Fld(0x1, SHU1_CA_CMD8_R_DMRANK_PIPE_CG_IG_CA)
		| P_Fld(0x1, SHU1_CA_CMD8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_CA)
		| P_Fld(0x1, SHU1_CA_CMD8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_CA)
		| P_Fld(0x1, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_PIPE_CG_IG_CA)
		| P_Fld(0x1, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_SYNC_CG_IG_CA)
		| P_Fld(0x1, SHU1_CA_CMD8_R_DMSTBEN_SYNC_CG_IG_CA)
		| P_Fld(0x1, SHU1_CA_CMD8_R_DMRXDLY_CG_IG_CA)
		| P_Fld(0x1, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_CA)
		| P_Fld(0x1, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_PIPE_CG_IG_CA)
		| P_Fld(0x0, SHU1_CA_CMD8_R_DMRXDVS_UPD_FORCE_EN_CA)
		| P_Fld(0x7fff, SHU1_CA_CMD8_R_DMRXDVS_UPD_FORCE_CYC_CA));
#if 0 /* Use _ALl() instead */
	vIO32WriteFldMulti_All(DDRPHY_SHU1_CA_CMD8 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, SHU1_CA_CMD8_R_DMRANK_CHG_PIPE_CG_IG_CA)
		| P_Fld(0x1, SHU1_CA_CMD8_R_DMRANK_PIPE_CG_IG_CA)
		| P_Fld(0x1, SHU1_CA_CMD8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_CA)
		| P_Fld(0x1, SHU1_CA_CMD8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_CA)
		| P_Fld(0x1, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_PIPE_CG_IG_CA)
		| P_Fld(0x1, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_SYNC_CG_IG_CA)
		| P_Fld(0x1, SHU1_CA_CMD8_R_DMSTBEN_SYNC_CG_IG_CA)
		| P_Fld(0x1, SHU1_CA_CMD8_R_DMRXDLY_CG_IG_CA)
		| P_Fld(0x1, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_CA)
		| P_Fld(0x1, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_PIPE_CG_IG_CA)
		| P_Fld(0x0, SHU1_CA_CMD8_R_DMRXDVS_UPD_FORCE_EN_CA)
		| P_Fld(0x7fff, SHU1_CA_CMD8_R_DMRXDVS_UPD_FORCE_CYC_CA));
#endif
	vIO32WriteFldAlign_All(DDRPHY_MISC_CTRL3, 0x1, MISC_CTRL3_R_DDRPHY_COMB_CG_IG);
	/* vIO32WriteFldAlign(DDRPHY_MISC_CTRL3+SHIFT_TO_CHB_ADDR, 0x1, MISC_CTRL3_R_DDRPHY_COMB_CG_IG); //Use _All() instead */
	vIO32WriteFldAlign_All(DDRPHY_MISC_CTRL0, 0x0, MISC_CTRL0_R_DMSHU_PHYDCM_FORCEOFF);
	/* vIO32WriteFldAlign(DDRPHY_MISC_CTRL0+SHIFT_TO_CHB_ADDR, 0x0, MISC_CTRL0_R_DMSHU_PHYDCM_FORCEOFF); //Use _All() instead */
	vIO32WriteFldAlign_All(DDRPHY_MISC_RXDVS2, 0x1, MISC_RXDVS2_R_DMRXDVS_SHUFFLE_CTRL_CG_IG);
	/* vIO32WriteFldAlign(DDRPHY_MISC_RXDVS2+SHIFT_TO_CHB_ADDR, 0x1, MISC_RXDVS2_R_DMRXDVS_SHUFFLE_CTRL_CG_IG); //Use _All() instead */

	vIO32WriteFldMulti_All(DDRPHY_SHU1_B0_DQ7, P_Fld(0x0, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0)
		| P_Fld(0x0, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0)
		| P_Fld(0x0, SHU1_B0_DQ7_R_DMRXDVS_DQM_FLAGSEL_B0));
#if 0 /* Use _All() instead */
	vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7 + SHIFT_TO_CHB_ADDR, P_Fld(0x0, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0)
		| P_Fld(0x0, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0)
		| P_Fld(0x0, SHU1_B0_DQ7_R_DMRXDVS_DQM_FLAGSEL_B0));
#endif
	vIO32WriteFldMulti_All(DDRPHY_SHU1_B1_DQ7, P_Fld(0x0, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1)
		| P_Fld(0x0, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1)
		| P_Fld(0x0, SHU1_B1_DQ7_R_DMRXDVS_DQM_FLAGSEL_B1));
#if 0 /* Use _All() instead */
	vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ7 + SHIFT_TO_CHB_ADDR, P_Fld(0x0, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1)
		| P_Fld(0x0, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1)
		| P_Fld(0x0, SHU1_B1_DQ7_R_DMRXDVS_DQM_FLAGSEL_B1));

#endif
	if (1) /* (mt_get_chip_sw_ver() == CHIP_SW_VER_02) */
	{
		vIO32WriteFldMulti(DRAMC_REG_CLKAR, P_Fld(0x1, CLKAR_SELPH_CMD_CG_DIS) | P_Fld(0x7FFF, CLKAR_REQQUE_PACG_DIS));
	}
	else
	{
		vIO32WriteFldAlign(DRAMC_REG_CLKAR, 0x0, CLKAR_SEQCLKRUN);
	}

	vIO32WriteFldAlign(DRAMC_REG_SHU_DQSG_RETRY, 0x0, SHU_DQSG_RETRY_R_RETRY_PA_DSIABLE); /* SH: Set to 0 -> save power */
	vIO32WriteFldAlign(DRAMC_REG_WRITE_LEV, 0x0, WRITE_LEV_DDRPHY_COMB_CG_SEL);

	/* Dummy read setting review by Chris Li */
	vIO32WriteFldMulti(DRAMC_REG_DUMMY_RD, P_Fld(0x1, DUMMY_RD_DUMMY_RD_PA_OPT)
		| P_Fld(0x1, DUMMY_RD_DMYRD_REORDER_DIS) /* YH: dummy read with reorder high priority 1:disable 0:enable */
		| P_Fld(0x0, DUMMY_RD_DMYRD_HPRI_DIS) /* YH: dummy read request with high priority 1:disable 0:enable */
		| P_Fld(0x1, DUMMY_RD_DUMMY_RD_CNT6)
		| P_Fld(0x1, DUMMY_RD_DUMMY_RD_CNT5)
		| P_Fld(0x1, DUMMY_RD_DUMMY_RD_CNT3)
		| P_Fld(0x1, DUMMY_RD_DUMMY_RD_SW));

	vIO32WriteFldMulti(DRAMC_REG_STBCAL2, P_Fld(0x1, STBCAL2_STB_UIDLYCG_IG)
		| P_Fld(0x1, STBCAL2_STB_PIDLYCG_IG));
	vIO32WriteFldMulti(DRAMC_REG_EYESCAN, P_Fld(0x1, EYESCAN_EYESCAN_DQS_SYNC_EN)
		| P_Fld(0x1, EYESCAN_EYESCAN_NEW_DQ_SYNC_EN)
		| P_Fld(0x1, EYESCAN_EYESCAN_DQ_SYNC_EN));
	vIO32WriteFldMulti(DRAMC_REG_SHU_ODTCTRL, P_Fld(0x1, SHU_ODTCTRL_RODTENSTB_SELPH_CG_IG)
		| P_Fld(0x1, SHU_ODTCTRL_RODTEN_SELPH_CG_IG));
#if 1/* #ifndef BIANCO_TO_BE_PORTING */
	vIO32WriteFldAlign_All(DDRPHY_SHU1_B0_DLL0, 0x1, SHU1_B0_DLL0_RG_ARPISM_MCK_SEL_B0_SHU);
	vIO32WriteFldAlign_All(DDRPHY_SHU1_B1_DLL0, 0x1, SHU1_B1_DLL0_RG_ARPISM_MCK_SEL_B1_SHU);
	vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_DLL0, 0x1, SHU1_CA_DLL0_RG_ARPISM_MCK_SEL_CA_SHU);
#if 0 /* Use _All() instead */
	vIO32WriteFldAlign(DDRPHY_SHU1_B0_DLL0 + SHIFT_TO_CHB_ADDR, 0x1, SHU1_B0_DLL0_RG_ARPISM_MCK_SEL_B0_SHU);
	vIO32WriteFldAlign(DDRPHY_SHU1_B1_DLL0 + SHIFT_TO_CHB_ADDR, 0x1, SHU1_B1_DLL0_RG_ARPISM_MCK_SEL_B1_SHU);
	vIO32WriteFldAlign(DDRPHY_SHU1_CA_DLL0 + SHIFT_TO_CHB_ADDR, 0x1, SHU1_CA_DLL0_RG_ARPISM_MCK_SEL_CA_SHU);
#endif /* if 0 */
#endif
	/* end _K_ */

	/* Rx Gating tracking settings */
	vIO32WriteFldMulti_All(DRAMC_REG_SHURK0_DQSCAL, P_Fld(0, SHURK0_DQSCAL_R0DQSIENLLMTEN) | P_Fld(0, SHURK0_DQSCAL_R0DQSIENHLMTEN));
	vIO32WriteFldMulti_All(DRAMC_REG_SHURK1_DQSCAL, P_Fld(0, SHURK1_DQSCAL_R1DQSIENLLMTEN) | P_Fld(0, SHURK1_DQSCAL_R1DQSIENHLMTEN));
	vIO32WriteFldMulti_All(DRAMC_REG_SHU_STBCAL, P_Fld(1, SHU_STBCAL_DQSG_MODE) | P_Fld(1, SHU_STBCAL_PICGLAT));

	/* 6755-like ACTiming */
#if DramcHWDQSGatingTracking_DVT_JADE_TRACKING_MODE

	vIO32WriteFldAlign_All(DRAMC_REG_SHU_PIPE, 0xFC000000, PHY_FLD_FULL);
	vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ6, 0x38, SHU1_B1_DQ6_RG_ARPI_OFFSET_DQSIEN_B1);
	vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ6 + SHIFT_TO_CHB_ADDR, 0x38, SHU1_B0_DQ6_RG_ARPI_OFFSET_DQSIEN_B0);
	vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD6 + SHIFT_TO_CHB_ADDR, 0x38, SHU1_CA_CMD6_RG_ARPI_OFFSET_CLKIEN);
	vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ6 + SHIFT_TO_CHB_ADDR, 0x38, SHU1_B1_DQ6_RG_ARPI_OFFSET_DQSIEN_B1);
	/* if(p->frequency==933)//test code */
	{
		/* For 6755-like mode, reserve bit5 should be set as "0" before calibration (by Justin) */
		vIO32WriteFldAlign(DDRPHY_SHU1_B0_DLL1, 0, RG_ARDQ_REV_BIT_05_RX_SER_RST_MODE);
		vIO32WriteFldAlign(DDRPHY_SHU1_B1_DLL1, 0, RG_ARDQ_REV_BIT_05_RX_SER_RST_MODE);
		vIO32WriteFldAlign(DDRPHY_SHU1_CA_DLL1, 0, RG_ARCMD_REV_BIT_05_RX_SER_RST_MODE);
		vIO32WriteFldAlign(DDRPHY_SHU1_B0_DLL1 + SHIFT_TO_CHB_ADDR, 0, RG_ARDQ_REV_BIT_05_RX_SER_RST_MODE);
		vIO32WriteFldAlign(DDRPHY_SHU1_B1_DLL1 + SHIFT_TO_CHB_ADDR, 0, RG_ARDQ_REV_BIT_05_RX_SER_RST_MODE);
		vIO32WriteFldAlign(DDRPHY_SHU1_CA_DLL1 + SHIFT_TO_CHB_ADDR, 0, RG_ARCMD_REV_BIT_05_RX_SER_RST_MODE);
	}

	vIO32WriteFldAlign_All(DRAMC_REG_SHU1_DQSG, 0x0, SHU1_DQSG_STB_UPDMASKCYC);
	vIO32WriteFldAlign_All(DRAMC_REG_SHU1_DQSG, 0x0, SHU1_DQSG_STB_UPDMASK_EN);
	/* DMSTBLAT should set as 3 for 6755-like mode */
	if (p->frequency >= 800)
	{
		vIO32WriteFldAlign_All(DRAMC_REG_SHU_STBCAL, 3, SHU_STBCAL_DMSTBLAT);
		/*
		 * vIO32WriteFldAlign_All(DRAMC_REG_SHU_ACTIM2, 0x7, SHU_ACTIM2_TR2W);
		 * vIO32WriteFldAlign_All(DRAMC_REG_SHU_AC_TIME_05T, 0, SHU_AC_TIME_05T_TR2W_05T);
		 * vIO32WriteFldAlign_All(DRAMC_REG_SHU_ACTIM_XRT, 0x5, SHU_ACTIM_XRT_XRTW2R);
		 * vIO32WriteFldAlign_All(DRAMC_REG_SHU_ACTIM_XRT, 0x6, SHU_ACTIM_XRT_XRTR2W);
		 */
	}
	else if (p->frequency >= 600)
	{
		vIO32WriteFldAlign_All(DRAMC_REG_SHU_STBCAL, 2, SHU_STBCAL_DMSTBLAT);
		/*
		 * vIO32WriteFldAlign_All(DRAMC_REG_SHU_ACTIM2, 0x5, SHU_ACTIM2_TR2W);
		 * vIO32WriteFldAlign_All(DRAMC_REG_SHU_AC_TIME_05T, 0, SHU_AC_TIME_05T_TR2W_05T);
		 * vIO32WriteFldAlign_All(DRAMC_REG_SHU_ACTIM_XRT, 0x5, SHU_ACTIM_XRT_XRTW2R);
		 * vIO32WriteFldAlign_All(DRAMC_REG_SHU_ACTIM_XRT, 0x6, SHU_ACTIM_XRT_XRTR2W);
		 */
	}
	else
	{
		vIO32WriteFldAlign_All(DRAMC_REG_SHU_STBCAL, 1, SHU_STBCAL_DMSTBLAT);
		/*
		 * vIO32WriteFldAlign_All(DRAMC_REG_SHU_ACTIM2, 0x4, SHU_ACTIM2_TR2W);
		 * vIO32WriteFldAlign_All(DRAMC_REG_SHU_AC_TIME_05T, 0, SHU_AC_TIME_05T_TR2W_05T);
		 * vIO32WriteFldAlign_All(DRAMC_REG_SHU_ACTIM_XRT, 0x5, SHU_ACTIM_XRT_XRTW2R);
		 * vIO32WriteFldAlign_All(DRAMC_REG_SHU_ACTIM_XRT, 0x6, SHU_ACTIM_XRT_XRTR2W);
		 */
	}

	/*
	 * For LP3 FFFF corner IC pass LTLV test
	 * Need to check with
	 */
	vIO32WriteFldAlign_All(DDRPHY_SHU1_B0_DQ5, 4, SHU1_B0_DQ5_RG_RX_ARDQS0_DQSIEN_DLY_B0);
	vIO32WriteFldAlign_All(DDRPHY_SHU1_B1_DQ5, 4, SHU1_B1_DQ5_RG_RX_ARDQS0_DQSIEN_DLY_B1);
	vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_CMD5, 4, SHU1_CA_CMD5_RG_RX_ARCLK_DQSIEN_DLY);


#else
	/*
	 * Valid delay mode debug : monitor window
	 * vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ5+SHIFT_TO_CHB_ADDR, 7, SHU1_B0_DQ5_RG_RX_ARDQS0_DQSIEN_DLY_B0);
	 * vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5+SHIFT_TO_CHB_ADDR, 7, SHU1_B1_DQ5_RG_RX_ARDQS0_DQSIEN_DLY_B1);
	 * vIO32WriteFldAlign_All(DRAMC_REG_SHU_PIPE, 0xF0000000,PHY_FLD_FULL);
	 */
	vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DQSG), \
		P_Fld(9, SHU1_DQSG_STB_UPDMASKCYC) | \
		P_Fld(1, SHU1_DQSG_STB_UPDMASK_EN));
#endif

	vIO32WriteFldAlign_All(DDRPHY_SHU1_B0_DQ5, 0x0, SHU1_B0_DQ5_RG_ARPI_FB_B0);
	vIO32WriteFldAlign_All(DDRPHY_SHU1_B1_DQ5, 0x0, SHU1_B1_DQ5_RG_ARPI_FB_B1);
	vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_CMD5, 0x0, SHU1_CA_CMD5_RG_ARPI_FB_CA);
#if 0 /* Use _All() instead */
	vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ5 + SHIFT_TO_CHB_ADDR, 0x4, SHU1_B0_DQ5_RG_ARPI_FB_B0);
	vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5 + SHIFT_TO_CHB_ADDR, 0x4, SHU1_B1_DQ5_RG_ARPI_FB_B1);
	vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD5 + SHIFT_TO_CHB_ADDR, 0x4, SHU1_CA_CMD5_RG_ARPI_FB_CA);
#endif

	/* DE_UPDATE */
	vIO32WriteFldMulti(DRAMC_REG_DRAMCTRL, P_Fld(0, DRAMCTRL_FW2R)
		| P_Fld(0x0, DRAMCTRL_DYNMWREN)
		| P_Fld(0x0, DRAMCTRL_CLKWITRFC)
		| P_Fld(0x1, DRAMCTRL_TMRR2WDIS)
		| P_Fld(0x0, DRAMCTRL_ADRBIT3DEC)
		| P_Fld(0x0, DRAMCTRL_CTOREQ_HPRI_OPT));
	vIO32WriteFldAlign(DRAMC_REG_MISCTL0, 0x1, MISCTL0_PBC_ARB_EN); /* Darren + for LPDDR3 */
	vIO32WriteFldAlign(DRAMC_REG_PERFCTL0, 0x0, PERFCTL0_WRFIFO_OPT);
	vIO32WriteFldMulti(DRAMC_REG_PERFCTL0, P_Fld(0x0, PERFCTL0_REORDEREN)
		| P_Fld(0x0, PERFCTL0_RWHPRICTL)
		| P_Fld(0x1, PERFCTL0_EMILLATEN)
		| P_Fld(0x1, PERFCTL0_RWAGEEN)
		| P_Fld(0x1, PERFCTL0_RWHPRIEN)); /* Darren + by Derping confirm */
	vIO32WriteFldAlign(DRAMC_REG_SREFCTRL, 0x8, SREFCTRL_SREFDLY);
	/* LP3 use blocking mode not run time mode (review by YH) */
	vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x1, SPCMDCTRL_REFR_BLOCKEN);
	vIO32WriteFldAlign(DRAMC_REG_HW_MRR_FUN, 0x0, HW_MRR_FUN_TMRR_ENA);

	vIO32WriteFldMulti(DRAMC_REG_SHUCTRL, P_Fld(0x0, SHUCTRL_VRCG_EN)
		| P_Fld(0x3, SHUCTRL_R_DVFS_PICG_MARGIN2)
		| P_Fld(0x3, SHUCTRL_R_DVFS_PICG_MARGIN3));
	vIO32WriteFldMulti(DRAMC_REG_SHUCTRL2, P_Fld(0x1, SHUCTRL2_SHORTQ_OPT) /* YH: short mode selection 0:original (worst case 4 commands in queue before DVFS), 1: new (worst case 2 commands) */
		| P_Fld(0x3, SHUCTRL2_R_DVFS_PICG_MARGIN)
		| P_Fld(0x1, SHUCTRL2_R_DVFS_SREF_OPT)); /* sw to enable dvfs + refresh */

	vIO32WriteFldMulti(DRAMC_REG_REFCTRL0, P_Fld(0x1, REFCTRL0_REFNA_OPT)
		| P_Fld(0x3, REFCTRL0_DISBYREFNUM)
		| P_Fld(0x0, REFCTRL0_UPDBYWR)); /* LP3 is fifo mode, so have to set as 0 (review by YH) / /Darren + for LPDDR3 */
	vIO32WriteFldMulti(DDRPHY_B0_DQ6, P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B0)
		| P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0));
	vIO32WriteFldMulti(DDRPHY_CA_CMD6, P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_OP_BIAS_SW_EN)
		| P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_BIAS_EN));

#ifdef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
	vIO32WriteFldMulti(DDRPHY_B0_DQ6 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, B0_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B0)
		| P_Fld(0x1, B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0));
	vIO32WriteFldMulti(DDRPHY_B1_DQ6, P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B1)
		| P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1));
	vIO32WriteFldMulti(DDRPHY_B1_DQ6 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B1)
		| P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1));
	vIO32WriteFldMulti(DDRPHY_CA_CMD6 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, CA_CMD6_RG_RX_ARCMD_OP_BIAS_SW_EN)
		| P_Fld(0x1, CA_CMD6_RG_RX_ARCMD_BIAS_EN));
	vIO32WriteFldAlign(DRAMC_REG_STBCAL2, 0x1, STBCAL2_STB_PICG_EARLY_1T_EN);

	vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ7, P_Fld(0x0, SHU1_B1_DQ7_R_DMRXRANK_DQS_LAT_B1)
		| P_Fld(0x1, SHU1_B1_DQ7_R_DMRXRANK_DQS_EN_B1)
		| P_Fld(0x1, SHU1_B1_DQ7_R_DMRXRANK_DQ_LAT_B1)
		| P_Fld(0x1, SHU1_B1_DQ7_R_DMRXRANK_DQ_EN_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7 + SHIFT_TO_CHB_ADDR, P_Fld(0x0, SHU1_B0_DQ7_R_DMRXRANK_DQS_LAT_B0)
		| P_Fld(0x1, SHU1_B0_DQ7_R_DMRXRANK_DQS_EN_B0)
		| P_Fld(0x1, SHU1_B0_DQ7_R_DMRXRANK_DQ_LAT_B0)
		| P_Fld(0x1, SHU1_B0_DQ7_R_DMRXRANK_DQ_EN_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ7 + SHIFT_TO_CHB_ADDR, P_Fld(0x0, SHU1_B1_DQ7_R_DMRXRANK_DQS_LAT_B1)
		| P_Fld(0x1, SHU1_B1_DQ7_R_DMRXRANK_DQS_EN_B1)
		| P_Fld(0x1, SHU1_B1_DQ7_R_DMRXRANK_DQ_LAT_B1)
		| P_Fld(0x1, SHU1_B1_DQ7_R_DMRXRANK_DQ_EN_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD7 + SHIFT_TO_CHB_ADDR, P_Fld(0x0, SHU1_CA_CMD7_R_DMRXRANK_CLK_LAT)
		| P_Fld(0x1, SHU1_CA_CMD7_R_DMRXRANK_CLK_EN)
		| P_Fld(0x1, SHU1_CA_CMD7_R_DMRXRANK_CMD_LAT)
		| P_Fld(0x1, SHU1_CA_CMD7_R_DMRXRANK_CMD_EN));

#else

	vIO32WriteFldMulti(DDRPHY_B0_DQ6 + SHIFT_TO_CHB_ADDR, P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B0)
		| P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0));
	vIO32WriteFldMulti(DDRPHY_B1_DQ6, P_Fld(0x0, B1_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B1)
		| P_Fld(0x0, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1));
	vIO32WriteFldMulti(DDRPHY_B1_DQ6 + SHIFT_TO_CHB_ADDR, P_Fld(0x0, B1_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B1)
		| P_Fld(0x0, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1));
	vIO32WriteFldMulti(DDRPHY_CA_CMD6 + SHIFT_TO_CHB_ADDR, P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_OP_BIAS_SW_EN)
		| P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_BIAS_EN));
#endif
	/* end DE UPDATE */


	/* review */
	vIO32WriteFldMulti_All(DDRPHY_CA_DLL_ARPI1, P_Fld(0x0, CA_DLL_ARPI1_RG_ARPI_MCTL_JUMP_EN_CA)
		| P_Fld(0x0, CA_DLL_ARPI1_RG_ARPI_FB_JUMP_EN_CA)
		| P_Fld(0x0, CA_DLL_ARPI1_RG_ARPI_CS_JUMP_EN)
		| P_Fld(0x0, CA_DLL_ARPI1_RG_ARPI_CLK_JUMP_EN)
		| P_Fld(0x0, CA_DLL_ARPI1_RG_ARPI_CMD_JUMP_EN)
		| P_Fld(0x0, CA_DLL_ARPI1_RG_ARPI_CLKIEN_JUMP_EN)
		| P_Fld(0x1, CA_DLL_ARPI1_RG_ARPISM_MCK_SEL_CA)
		| P_Fld(0x1, CA_DLL_ARPI1_RG_ARPISM_MCK_SEL_CA_REG_OPT));
	vIO32WriteFldMulti_All(DDRPHY_B0_DLL_ARPI1, P_Fld(0x0, B0_DLL_ARPI1_RG_ARPI_MCTL_JUMP_EN_B0)
		| P_Fld(0x0, B0_DLL_ARPI1_RG_ARPI_FB_JUMP_EN_B0)
		| P_Fld(0x0, B0_DLL_ARPI1_RG_ARPI_DQS_JUMP_EN_B0)
		| P_Fld(0x0, B0_DLL_ARPI1_RG_ARPI_DQM_JUMP_EN_B0)
		| P_Fld(0x0, B0_DLL_ARPI1_RG_ARPI_DQ_JUMP_EN_B0)
		| P_Fld(0x0, B0_DLL_ARPI1_RG_ARPI_DQSIEN_JUMP_EN_B0)
		| P_Fld(0x1, B0_DLL_ARPI1_RG_ARPISM_MCK_SEL_B0) /* Joe: ARPISM_MCK_SEL_Bx, ARPISM_MCK_SEL_Bx_REG_OPT should be 1 to fulfill APHY TX OE spec for low freq (Ex: DDR1600) */
		| P_Fld(0x1, B0_DLL_ARPI1_RG_ARPISM_MCK_SEL_B0_REG_OPT));
	vIO32WriteFldMulti_All(DDRPHY_B1_DLL_ARPI1, P_Fld(0x0, B1_DLL_ARPI1_RG_ARPI_MCTL_JUMP_EN_B1)
		| P_Fld(0x0, B1_DLL_ARPI1_RG_ARPI_FB_JUMP_EN_B1)
		| P_Fld(0x0, B1_DLL_ARPI1_RG_ARPI_DQS_JUMP_EN_B1)
		| P_Fld(0x0, B1_DLL_ARPI1_RG_ARPI_DQM_JUMP_EN_B1)
		| P_Fld(0x0, B1_DLL_ARPI1_RG_ARPI_DQ_JUMP_EN_B1)
		| P_Fld(0x0, B1_DLL_ARPI1_RG_ARPI_DQSIEN_JUMP_EN_B1)
		| P_Fld(0x1, B1_DLL_ARPI1_RG_ARPISM_MCK_SEL_B1) /* Joe: ARPISM_MCK_SEL_Bx, ARPISM_MCK_SEL_Bx_REG_OPT should be 1 to fulfill APHY TX OE spec for low freq (Ex: DDR1600) */
		| P_Fld(0x1, B1_DLL_ARPI1_RG_ARPISM_MCK_SEL_B1_REG_OPT));

	vIO32WriteFldAlign_All(DDRPHY_B0_DQ5, 0x0, B0_DQ5_RG_RX_ARDQS0_DVS_EN_B0);
	vIO32WriteFldAlign_All(DDRPHY_B1_DQ5, 0x0, B1_DQ5_RG_RX_ARDQS0_DVS_EN_B1);
	vIO32WriteFldAlign_All(DDRPHY_CA_CMD5, 0x0, CA_CMD5_RG_RX_ARCLK_DVS_EN);

	vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL4, 0x13300000, MISC_CG_CTRL4_R_PHY_MCK_CG_CTRL);
	vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL4 + SHIFT_TO_CHB_ADDR, 0x22200a00, MISC_CG_CTRL4_R_PHY_MCK_CG_CTRL);

#if (fcFOR_CHIP_ID == fcTalbot)
	/* Must check with EMI owners -> Asynchronous EMI: Can't turn on RWSPLIT, Synchronous EMI: Can enable RWSPLIT (DE: JL Wu) */
	vIO32WriteFldMulti_All(DRAMC_REG_PERFCTL0, P_Fld(0x1, PERFCTL0_RWSPLIT) /* synchronous EMI -> can turn on RWSPLIT */
		| P_Fld(0x0, PERFCTL0_REORDEREN)); /* Lewis@20170623: Set REORDEREN to 1 in LP3 In order to be consistent with kibo + . SBR is off in EMI. */
#endif
#if FIX_CROSSRK_XRT_05T_OPT
	vIO32WriteFldAlign_All(DRAMC_REG_PERFCTL0, 0x0, PERFCTL0_XRT_05T_OPT);
#else
	vIO32WriteFldAlign_All(DRAMC_REG_PERFCTL0, 0x1, PERFCTL0_XRT_05T_OPT);
#endif

	vIO32WriteFldAlign_All(DRAMC_REG_CLKCTRL, 0x1, CLKCTRL_SEQCLKRUN3);
	vIO32WriteFldAlign_All(DRAMC_REG_STBCAL1, 0x0, STBCAL1_DLLFRZ_MON_PBREF_OPT);
	vIO32WriteFldAlign_All(DRAMC_REG_ARBCTL, 0x0, ARBCTL_DBIWR_IMP_EN );

	/* wei-jen: RX rank_sel for CHA CA is not used(6763), set it's dly to 0 to save power */
	vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD7, 0, SHU1_CA_CMD7_R_DMRANKRXDVS_CA);

#if TX_OE_EXTEND
	UpdateTxOEN(p);
#endif

	vIO32WriteFldAlign_All(DRAMC_REG_CKECTRL, 0x1, CKECTRL_CKEPBDIS);

#if (fcFOR_CHIP_ID == fcTalbot)
	{
		vIO32WriteFldAlign_All(DDRPHY_SHU1_B0_DQ3, 0x3, SHU1_B0_DQ3_RG_TX_ARDQ_PU_PRE_B0);
		vIO32WriteFldAlign_All(DDRPHY_SHU1_B1_DQ3, 0x3, SHU1_B1_DQ3_RG_TX_ARDQ_PU_PRE_B1);
		/* vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_CMD3, 0x3, SHU1_CA_CMD3_RG_TX_ARCMD_PU_PRE); //move to SW impedance */
	}
#endif
#if ENABLE_TMRRI_NEW_MODE
	/* [6765 DVT]dram auto refersh rate by hardware mr4 for rank0 and rank1 (Berson) */
	vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, (p->support_rank_num == RANK_DUAL) ? (1) : (0), SPCMDCTRL_HMR4_TOG_OPT);
#endif

#if CMD_PICG_NEW_MODE
    SetCmdPicgNewMode(p);
#endif

	/* [6765 DVT]RX FIFO debug feature, MP setting should enable debug function */
	vIO32WriteFldAlign(DDRPHY_B0_DQ9, 0x1, B0_DQ9_R_DMRXFIFO_STBENCMP_EN_B0);
	vIO32WriteFldAlign(DDRPHY_B1_DQ9, 0x1, B1_DQ9_R_DMRXFIFO_STBENCMP_EN_B1);
	vIO32WriteFldAlign(DDRPHY_CA_CMD10, 0x1, CA_CMD10_R_DMRXFIFO_STBENCMP_EN_CA);

#if (DramcHWDQSGatingTracking_DVT_LP3_FIFO_MODE == 1)
	vIO32WriteFldAlign(DDRPHY_B1_DQ9, 0x1, B1_DQ9_R_DMDQSIEN_RDSEL_LAT_B1);
	vIO32WriteFldAlign(DDRPHY_B0_DQ9 + SHIFT_TO_CHB_ADDR, 0x1, B0_DQ9_R_DMDQSIEN_RDSEL_LAT_B0);
	vIO32WriteFldAlign(DDRPHY_B1_DQ9 + SHIFT_TO_CHB_ADDR, 0x1, B1_DQ9_R_DMDQSIEN_RDSEL_LAT_B1);
	vIO32WriteFldAlign(DDRPHY_CA_CMD10 + SHIFT_TO_CHB_ADDR, 0x1, CA_CMD10_R_DMDQSIEN_RDSEL_LAT_CA);
#endif

	/* [MT6768 DVT] Add new settings for DVT required */
#ifdef BYPASS_TXPIPE_EN
	if ((p->frequency >= 800) && !p->bDLP3)
	{
		/* Note shall only set CHA */
		vIO32WriteFldMulti(DDRPHY_SHU1_MISC0,
			P_Fld(0x1, SHU1_MISC0_RG_CK_TXPIPE_BYPASS_EN) |
			P_Fld(0x1, SHU1_MISC0_RG_CMD_TXPIPE_BYPASS_EN));
	}
#endif

#ifdef BYPASS_RXPIPE_EN
	vIO32WriteFldAlign_All(DDRPHY_SHU1_MISC0, 0x1, SHU1_MISC0_R_RX_PIPE_BYPASS_EN);
#endif

	/* MT6768 add to fix short pulse */
	vIO32WriteFldMulti(DRAMC_REG_DVFSDLL,
		P_Fld(0x1, DVFSDLL_R_BYPASS_1ST_DLL_SHU4) |
		P_Fld(0x1, DVFSDLL_R_DMSHUFFLE_CHANGE_FREQ_OPT) |
		P_Fld(0x2, DVFSDLL_R_DVFS_DLL_MARGIN));

	return DRAM_OK;
}
#endif

#if LEGACY_DELAY_CELL
/*
 * Legacy CA, TX DQ, TX DQM delay cell initial settings
 * RK#_TX_ARCA#_DLY (RK0~2, CA0~5), RK#_TX_ARDQ#_DLY_B# (RK0~2, DQ0~8, B0~1), RK#_TX_ARDQM0_DLY_B# (RK0~2, B0~1)
 */
#if ENABLE_LP3_SW
static void LegacyDlyCellInitLP3(DRAMC_CTX_T *p)
{
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ0, P_Fld(0x6, SHU1_R1_B1_DQ0_RK1_TX_ARDQ7_DLY_B1)
		| P_Fld(0x6, SHU1_R1_B1_DQ0_RK1_TX_ARDQ6_DLY_B1)
		| P_Fld(0x6, SHU1_R1_B1_DQ0_RK1_TX_ARDQ5_DLY_B1)
		| P_Fld(0x6, SHU1_R1_B1_DQ0_RK1_TX_ARDQ4_DLY_B1)
		| P_Fld(0x6, SHU1_R1_B1_DQ0_RK1_TX_ARDQ3_DLY_B1)
		| P_Fld(0x6, SHU1_R1_B1_DQ0_RK1_TX_ARDQ2_DLY_B1)
		| P_Fld(0x6, SHU1_R1_B1_DQ0_RK1_TX_ARDQ1_DLY_B1)
		| P_Fld(0x6, SHU1_R1_B1_DQ0_RK1_TX_ARDQ0_DLY_B1));
	vIO32WriteFldAlign(DDRPHY_SHU1_R1_B1_DQ1, 0x6, SHU1_R1_B1_DQ1_RK1_TX_ARDQM0_DLY_B1);
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD0 + SHIFT_TO_CHB_ADDR, P_Fld(0x6, SHU1_R1_CA_CMD0_RK1_TX_ARCA5_DLY)
		| P_Fld(0x6, SHU1_R1_CA_CMD0_RK1_TX_ARCA4_DLY)
		| P_Fld(0x6, SHU1_R1_CA_CMD0_RK1_TX_ARCA3_DLY)
		| P_Fld(0x6, SHU1_R1_CA_CMD0_RK1_TX_ARCA2_DLY)
		| P_Fld(0x6, SHU1_R1_CA_CMD0_RK1_TX_ARCA1_DLY)
		| P_Fld(0x6, SHU1_R1_CA_CMD0_RK1_TX_ARCA0_DLY));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ0 + SHIFT_TO_CHB_ADDR, P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ7_DLY_B0)
		| P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ6_DLY_B0)
		| P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ5_DLY_B0)
		| P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ4_DLY_B0)
		| P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ3_DLY_B0)
		| P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ2_DLY_B0)
		| P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ1_DLY_B0)
		| P_Fld(0xb, SHU1_R1_B0_DQ0_RK1_TX_ARDQ0_DLY_B0));
	vIO32WriteFldAlign(DDRPHY_SHU1_R1_B0_DQ1 + SHIFT_TO_CHB_ADDR, 0x6, SHU1_R1_B0_DQ1_RK1_TX_ARDQM0_DLY_B0);
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ0 + SHIFT_TO_CHB_ADDR, P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ7_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ6_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ5_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ4_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ3_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ2_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ1_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ0_RK1_TX_ARDQ0_DLY_B1));
	vIO32WriteFldAlign(DDRPHY_SHU1_R1_B1_DQ1 + SHIFT_TO_CHB_ADDR, 0x6, SHU1_R1_B1_DQ1_RK1_TX_ARDQM0_DLY_B1);
}
#endif

static void LegacyDlyCellInitLP4_DDR2667(DRAMC_CTX_T *p)
{
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ0, P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ7_DLY_B0)
		| P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ6_DLY_B0)
		| P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ5_DLY_B0)
		| P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ4_DLY_B0)
		| P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ3_DLY_B0)
		| P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ2_DLY_B0)
		| P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ1_DLY_B0)
		| P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ0_DLY_B0));
	vIO32WriteFldAlign(DDRPHY_SHU1_R1_B0_DQ1, 0xa, SHU1_R1_B0_DQ1_RK1_TX_ARDQM0_DLY_B0);
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ0, P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ7_DLY_B1)
		| P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ6_DLY_B1)
		| P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ5_DLY_B1)
		| P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ4_DLY_B1)
		| P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ3_DLY_B1)
		| P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ2_DLY_B1)
		| P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ1_DLY_B1)
		| P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ0_DLY_B1));
	vIO32WriteFldAlign(DDRPHY_SHU1_R1_B1_DQ1, 0xa, SHU1_R1_B1_DQ1_RK1_TX_ARDQM0_DLY_B1);
}

static void LegacyDlyCellInitLP4_DDR1600(DRAMC_CTX_T *p)
{
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ0, P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ7_DLY_B0)
		| P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ6_DLY_B0)
		| P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ5_DLY_B0)
		| P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ4_DLY_B0)
		| P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ3_DLY_B0)
		| P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ2_DLY_B0)
		| P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ1_DLY_B0)
		| P_Fld(0xd, SHU1_R1_B0_DQ0_RK1_TX_ARDQ0_DLY_B0));
	vIO32WriteFldAlign(DDRPHY_SHU1_R1_B0_DQ1, 0xd, SHU1_R1_B0_DQ1_RK1_TX_ARDQM0_DLY_B0);
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ0, P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ7_DLY_B1)
		| P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ6_DLY_B1)
		| P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ5_DLY_B1)
		| P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ4_DLY_B1)
		| P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ3_DLY_B1)
		| P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ2_DLY_B1)
		| P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ1_DLY_B1)
		| P_Fld(0xd, SHU1_R1_B1_DQ0_RK1_TX_ARDQ0_DLY_B1));
	vIO32WriteFldAlign(DDRPHY_SHU1_R1_B1_DQ1, 0xd, SHU1_R1_B1_DQ1_RK1_TX_ARDQM0_DLY_B1);
}

static void LegacyDlyCellInitLP4_DDR3200(DRAMC_CTX_T *p)
{
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ0, P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ7_DLY_B0)
		| P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ6_DLY_B0)
		| P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ5_DLY_B0)
		| P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ4_DLY_B0)
		| P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ3_DLY_B0)
		| P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ2_DLY_B0)
		| P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ1_DLY_B0)
		| P_Fld(0xa, SHU1_R1_B0_DQ0_RK1_TX_ARDQ0_DLY_B0));
	vIO32WriteFldAlign(DDRPHY_SHU1_R1_B0_DQ1, 0xa, SHU1_R1_B0_DQ1_RK1_TX_ARDQM0_DLY_B0);
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ0, P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ7_DLY_B1)
		| P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ6_DLY_B1)
		| P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ5_DLY_B1)
		| P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ4_DLY_B1)
		| P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ3_DLY_B1)
		| P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ2_DLY_B1)
		| P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ1_DLY_B1)
		| P_Fld(0xa, SHU1_R1_B1_DQ0_RK1_TX_ARDQ0_DLY_B1));
	vIO32WriteFldAlign(DDRPHY_SHU1_R1_B1_DQ1, 0xa, SHU1_R1_B1_DQ1_RK1_TX_ARDQM0_DLY_B1);
}
#endif

#if LEGACY_TX_TRACK
/*
 * Legacy tx tracking related initial settings (actual correct values are set during calibration steps)
 * BOOT_ORIG_UI_RK#_DQ#, BOOT_TARG_UI_RK#_DQ#, BOOT_TARG_UI_RK#_DQM#, BOOT_TARG_UI_RK#_OEN_DQ#, BOOT_TARG_UI_RK#_OEN_DQM#
 * DQSOSCTHRD_DEC, DQSOSCTHRD_INC, DQSOSC_PRDCNT
 * DQSOSC_BASE_RK#, DQSOSCR_RK#_BYTE_MODE, DQSOSCR_DQSOSCRCNT
 */
static void LegacyTxTrackLP4_DDR2667(DRAMC_CTX_T *p)
{
	vIO32WriteFldMulti(DRAMC_REG_SHU_DQSOSCTHRD, P_Fld(0xc, SHU_DQSOSCTHRD_DQSOSCTHRD_DEC_RK0)
		| P_Fld(0xc, SHU_DQSOSCTHRD_DQSOSCTHRD_INC_RK0)
		| P_Fld(0xc, SHU_DQSOSCTHRD_DQSOSCTHRD_INC_RK1_7TO0));
	vIO32WriteFldMulti(DRAMC_REG_SHU1_DQSOSC_PRD, P_Fld(0xc, SHU1_DQSOSC_PRD_DQSOSCTHRD_DEC_RK1)
		| P_Fld(0x0, SHU1_DQSOSC_PRD_DQSOSCTHRD_INC_RK1_11TO8)
		| P_Fld(0x10, SHU1_DQSOSC_PRD_DQSOSC_PRDCNT));
	vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQSOSC, P_Fld(0x1ae, SHU1RK0_DQSOSC_DQSOSC_BASE_RK0_B1)
		| P_Fld(0x1ae, SHU1RK0_DQSOSC_DQSOSC_BASE_RK0));
	if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
	{
		vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL1, P_Fld(0x354, SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ1)
			| P_Fld(0x354, SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ0));
		vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL2, P_Fld(0x354, SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ1)
			| P_Fld(0x354, SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ0));
	}
	else
	{
		vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL1, P_Fld(0x254, SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ1)
			| P_Fld(0x254, SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ0));
		vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL2, P_Fld(0x254, SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ1)
			| P_Fld(0x254, SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ0));
	}
	vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL3, P_Fld(0x14, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1_B4TO0)
		| P_Fld(0x14, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0_B4TO0)
		| P_Fld(0x18, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1)
		| P_Fld(0x18, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0));
	vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL4, P_Fld(0x14, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1_B4TO0)
		| P_Fld(0x14, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0_B4TO0)
		| P_Fld(0x18, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1)
		| P_Fld(0x18, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0));
	if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
	{
		vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL5, P_Fld(0x354, SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM1)
			| P_Fld(0x354, SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM0));
	}
	else
	{
		vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL5, P_Fld(0x254, SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM1)
			| P_Fld(0x254, SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM0));
	}
	vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQSOSC, P_Fld(0x160, SHU1RK1_DQSOSC_DQSOSC_BASE_RK1_B1)
		| P_Fld(0x160, SHU1RK1_DQSOSC_DQSOSC_BASE_RK1));
	if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
	{
		vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL1, P_Fld(0x354, SHU1RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ1)
			| P_Fld(0x354, SHU1RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ0));
		vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL2, P_Fld(0x354, SHU1RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ1)
			| P_Fld(0x354, SHU1RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ0));
	}
	else
	{
		vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL1, P_Fld(0x254, SHU1RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ1)
			| P_Fld(0x254, SHU1RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ0));
		vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL2, P_Fld(0x254, SHU1RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ1)
			| P_Fld(0x254, SHU1RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ0));
	}
	vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL3, P_Fld(0x14, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ1_B4TO0)
		| P_Fld(0x14, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ0_B4TO0)
		| P_Fld(0x18, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ1)
		| P_Fld(0x18, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ0));
	vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL4, P_Fld(0x14, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM1_B4TO0)
		| P_Fld(0x14, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM0_B4TO0)
		| P_Fld(0x18, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM1)
		| P_Fld(0x18, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM0));
	if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
	{
		vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL5, P_Fld(0x354, SHU1RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM1)
			| P_Fld(0x354, SHU1RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM0));
	}
	else
	{
		vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL5, P_Fld(0x254, SHU1RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM1)
			| P_Fld(0x254, SHU1RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM0));
	}
}

static void LegacyTxTrackLP4_DDR1600(DRAMC_CTX_T *p)
{
	vIO32WriteFldMulti(DRAMC_REG_SHU_DQSOSCTHRD, P_Fld(0x14, SHU_DQSOSCTHRD_DQSOSCTHRD_DEC_RK0)
		| P_Fld(0x14, SHU_DQSOSCTHRD_DQSOSCTHRD_INC_RK0)
		| P_Fld(0x14, SHU_DQSOSCTHRD_DQSOSCTHRD_INC_RK1_7TO0));
	vIO32WriteFldMulti(DRAMC_REG_SHU1_DQSOSC_PRD, P_Fld(0x14, SHU1_DQSOSC_PRD_DQSOSCTHRD_DEC_RK1)
		| P_Fld(0x0, SHU1_DQSOSC_PRD_DQSOSCTHRD_INC_RK1_11TO8)
		| P_Fld(0xf, SHU1_DQSOSC_PRD_DQSOSC_PRDCNT));
	vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQSOSC, P_Fld(0x2d0, SHU1RK0_DQSOSC_DQSOSC_BASE_RK0_B1)
		| P_Fld(0x2d0, SHU1RK0_DQSOSC_DQSOSC_BASE_RK0));
	vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL1, P_Fld(0x23a, SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ1)
		| P_Fld(0x23a, SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ0));
	vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL2, P_Fld(0x23a, SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ1)
		| P_Fld(0x23a, SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ0));
	vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL3, P_Fld(0x1a, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1_B4TO0)
		| P_Fld(0x1a, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0_B4TO0)
		| P_Fld(0xf, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1)
		| P_Fld(0xf, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0));
	vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL4, P_Fld(0x1a, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1_B4TO0)
		| P_Fld(0x1a, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0_B4TO0)
		| P_Fld(0xf, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1)
		| P_Fld(0xf, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0));
	vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL5, P_Fld(0x23e, SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM1)
		| P_Fld(0x23e, SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM0));
	vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQSOSC, P_Fld(0x24e, SHU1RK1_DQSOSC_DQSOSC_BASE_RK1_B1)
		| P_Fld(0x24e, SHU1RK1_DQSOSC_DQSOSC_BASE_RK1));
	vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL1, P_Fld(0x23e, SHU1RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ1)
		| P_Fld(0x23e, SHU1RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ0));
	vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL2, P_Fld(0x23e, SHU1RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ1)
		| P_Fld(0x23e, SHU1RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ0));
	vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL3, P_Fld(0x1e, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ1_B4TO0)
		| P_Fld(0x1e, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ0_B4TO0)
		| P_Fld(0xf, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ1)
		| P_Fld(0xf, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ0));
	vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL4, P_Fld(0x1e, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM1_B4TO0)
		| P_Fld(0x1e, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM0_B4TO0)
		| P_Fld(0xf, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM1)
		| P_Fld(0xf, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM0));
	vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL5, P_Fld(0x23e, SHU1RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM1)
		| P_Fld(0x23e, SHU1RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM0));
}

static void LegacyTxTrackLP4_DDR3200(DRAMC_CTX_T *p)
{
	vIO32WriteFldMulti(DRAMC_REG_SHU_DQSOSCTHRD, P_Fld(0xa, SHU_DQSOSCTHRD_DQSOSCTHRD_DEC_RK0)
		| P_Fld(0xa, SHU_DQSOSCTHRD_DQSOSCTHRD_INC_RK0)
		| P_Fld(0xa, SHU_DQSOSCTHRD_DQSOSCTHRD_INC_RK1_7TO0));
	vIO32WriteFldMulti(DRAMC_REG_SHU1_DQSOSC_PRD, P_Fld(0xa, SHU1_DQSOSC_PRD_DQSOSCTHRD_DEC_RK1)
		| P_Fld(0x0, SHU1_DQSOSC_PRD_DQSOSCTHRD_INC_RK1_11TO8)
		| P_Fld(0x10, SHU1_DQSOSC_PRD_DQSOSC_PRDCNT));
	vIO32WriteFldAlign(DRAMC_REG_SHU_DQSOSCR, 0x10, SHU_DQSOSCR_DQSOSCRCNT);
	vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQSOSC, P_Fld(0x168, SHU1RK0_DQSOSC_DQSOSC_BASE_RK0_B1)
		| P_Fld(0x168, SHU1RK0_DQSOSC_DQSOSC_BASE_RK0));
	if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
	{
		vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL1, P_Fld(0x3da, SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ1)
			| P_Fld(0x3da, SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ0));
		vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL2, P_Fld(0x3da, SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ1)
			| P_Fld(0x3da, SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ0));
	}
	else
	{
		vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL1, P_Fld(0x2da, SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ1)
			| P_Fld(0x2da, SHU1RK0_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ0));
		vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL2, P_Fld(0x2da, SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ1)
			| P_Fld(0x2da, SHU1RK0_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ0));
	}
	vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL3, P_Fld(0x1a, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1_B4TO0)
		| P_Fld(0x1a, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0_B4TO0)
		| P_Fld(0x1c, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ1)
		| P_Fld(0x1c, SHU1RK0_DQS2DQ_CAL3_BOOT_TARG_UI_RK0_OEN_DQ0));
	vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL4, P_Fld(0x1a, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1_B4TO0)
		| P_Fld(0x1a, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0_B4TO0)
		| P_Fld(0x1c, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM1)
		| P_Fld(0x1c, SHU1RK0_DQS2DQ_CAL4_BOOT_TARG_UI_RK0_OEN_DQM0));
	if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
	{
		vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL5, P_Fld(0x3da, SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM1)
			| P_Fld(0x3da, SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM0));
	}
	else
	{
		vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_DQS2DQ_CAL5, P_Fld(0x2da, SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM1)
			| P_Fld(0x2da, SHU1RK0_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM0));
	}
	vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQSOSC, P_Fld(0x127, SHU1RK1_DQSOSC_DQSOSC_BASE_RK1_B1)
		| P_Fld(0x127, SHU1RK1_DQSOSC_DQSOSC_BASE_RK1));
	if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
	{
		vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL1, P_Fld(0x3d4, SHU1RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ1)
			| P_Fld(0x3d4, SHU1RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ0));
		vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL2, P_Fld(0x3d4, SHU1RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ1)
			| P_Fld(0x3d4, SHU1RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ0));
	}
	else
	{
		vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL1, P_Fld(0x2d4, SHU1RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ1)
			| P_Fld(0x2d4, SHU1RK1_DQS2DQ_CAL1_BOOT_ORIG_UI_RK1_DQ0));
		vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL2, P_Fld(0x2d4, SHU1RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ1)
			| P_Fld(0x2d4, SHU1RK1_DQS2DQ_CAL2_BOOT_TARG_UI_RK1_DQ0));
	}
	vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL3, P_Fld(0x14, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ1_B4TO0)
		| P_Fld(0x14, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ0_B4TO0)
		| P_Fld(0x1c, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ1)
		| P_Fld(0x1c, SHU1RK1_DQS2DQ_CAL3_BOOT_TARG_UI_RK1_OEN_DQ0));
	vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL4, P_Fld(0x14, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM1_B4TO0)
		| P_Fld(0x14, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM0_B4TO0)
		| P_Fld(0x1c, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM1)
		| P_Fld(0x1c, SHU1RK1_DQS2DQ_CAL4_BOOT_TARG_UI_RK1_OEN_DQM0));
	if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
	{
		vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL5, P_Fld(0x3d4, SHU1RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM1)
			| P_Fld(0x3d4, SHU1RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM0));
	}
	else
	{
		vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_DQS2DQ_CAL5, P_Fld(0x2d4, SHU1RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM1)
			| P_Fld(0x2d4, SHU1RK1_DQS2DQ_CAL5_BOOT_TARG_UI_RK1_DQM0));
	}
	if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
	{
		vIO32WriteFldAlign(DRAMC_REG_DQSOSCR, 0x1, DQSOSCR_RK0_BYTE_MODE);
		vIO32WriteFldAlign(DRAMC_REG_DQSOSCR, 0x1, DQSOSCR_RK1_BYTE_MODE);
	}
}
#endif

#if LEGACY_TDQSCK_PRECAL
/*
 * Legacy tDQSCK precal related initial settings (actual correct values are set during calibration)
 * Ex: TDQSCK_JUMP_RATIO, TDQSCK_UIFREQ#, TDQSCK_PIFREQ#
 */
static void LegacyPreCalLP4_DDR2667(DRAMC_CTX_T *p)
{
	vIO32WriteFldMulti(DRAMC_REG_PRE_TDQSCK2, P_Fld(0x1a, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO0)
		| P_Fld(0x10, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO1)
		| P_Fld(0x0, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO2)
		| P_Fld(0x26, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO3));
	vIO32WriteFldMulti(DRAMC_REG_PRE_TDQSCK3, P_Fld(0x13, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO4)
		| P_Fld(0x0, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO5)
		| P_Fld(0x40, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO6)
		| P_Fld(0x35, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO7));
	vIO32WriteFldMulti(DRAMC_REG_PRE_TDQSCK4, P_Fld(0x0, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO8)
		| P_Fld(0x0, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO9)
		| P_Fld(0x0, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO10)
		| P_Fld(0x0, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO11));
	vIO32WriteFldAlign(DRAMC_REG_RK0_PRE_TDQSCK1, 0xa, RK0_PRE_TDQSCK1_TDQSCK_UIFREQ2_B0R0);
	vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK2, P_Fld(0x0, RK0_PRE_TDQSCK2_TDQSCK_PIFREQ4_B0R0)
		| P_Fld(0x0, RK0_PRE_TDQSCK2_TDQSCK_UIFREQ4_B0R0)
		| P_Fld(0xb, RK0_PRE_TDQSCK2_TDQSCK_PIFREQ3_B0R0)
		| P_Fld(0xd, RK0_PRE_TDQSCK2_TDQSCK_UIFREQ3_B0R0));
	vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK3, P_Fld(0x0, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ4_P1_B0R0)
		| P_Fld(0x11, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ3_P1_B0R0)
		| P_Fld(0xe, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ2_P1_B0R0));
	vIO32WriteFldAlign(DRAMC_REG_RK0_PRE_TDQSCK4, 0xa, RK0_PRE_TDQSCK4_TDQSCK_UIFREQ2_B1R0);
	vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK5, P_Fld(0x0, RK0_PRE_TDQSCK5_TDQSCK_PIFREQ4_B1R0)
		| P_Fld(0x0, RK0_PRE_TDQSCK5_TDQSCK_UIFREQ4_B1R0)
		| P_Fld(0xb, RK0_PRE_TDQSCK5_TDQSCK_PIFREQ3_B1R0)
		| P_Fld(0xd, RK0_PRE_TDQSCK5_TDQSCK_UIFREQ3_B1R0));
	vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK6, P_Fld(0x0, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ4_P1_B1R0)
		| P_Fld(0x11, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ3_P1_B1R0)
		| P_Fld(0xe, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ2_P1_B1R0));
	vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK1, P_Fld(0x8, RK1_PRE_TDQSCK1_TDQSCK_PIFREQ2_B0R1)
		| P_Fld(0x10, RK1_PRE_TDQSCK1_TDQSCK_UIFREQ2_B0R1));
	vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK2, P_Fld(0x0, RK1_PRE_TDQSCK2_TDQSCK_PIFREQ4_B0R1)
		| P_Fld(0x0, RK1_PRE_TDQSCK2_TDQSCK_UIFREQ4_B0R1)
		| P_Fld(0xe, RK1_PRE_TDQSCK2_TDQSCK_PIFREQ3_B0R1)
		| P_Fld(0x10, RK1_PRE_TDQSCK2_TDQSCK_UIFREQ3_B0R1));
	vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK3, P_Fld(0x0, RK1_PRE_TDQSCK3_TDQSCK_UIFREQ4_P1_B0R1)
		| P_Fld(0x14, RK1_PRE_TDQSCK3_TDQSCK_UIFREQ3_P1_B0R1)
		| P_Fld(0x14, RK1_PRE_TDQSCK3_TDQSCK_UIFREQ2_P1_B0R1));
	vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK4, P_Fld(0x8, RK1_PRE_TDQSCK4_TDQSCK_PIFREQ2_B1R1)
		| P_Fld(0x10, RK1_PRE_TDQSCK4_TDQSCK_UIFREQ2_B1R1));
	vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK5, P_Fld(0x0, RK1_PRE_TDQSCK5_TDQSCK_PIFREQ4_B1R1)
		| P_Fld(0x0, RK1_PRE_TDQSCK5_TDQSCK_UIFREQ4_B1R1)
		| P_Fld(0xe, RK1_PRE_TDQSCK5_TDQSCK_PIFREQ3_B1R1)
		| P_Fld(0x10, RK1_PRE_TDQSCK5_TDQSCK_UIFREQ3_B1R1));
	vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK6, P_Fld(0x0, RK1_PRE_TDQSCK6_TDQSCK_UIFREQ4_P1_B1R1)
		| P_Fld(0x14, RK1_PRE_TDQSCK6_TDQSCK_UIFREQ3_P1_B1R1)
		| P_Fld(0x14, RK1_PRE_TDQSCK6_TDQSCK_UIFREQ2_P1_B1R1));
}

static void LegacyPreCalLP4_DDR1600(DRAMC_CTX_T *p)
{
	vIO32WriteFldMulti(DRAMC_REG_PRE_TDQSCK2, P_Fld(0x1a, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO0)
		| P_Fld(0x10, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO1)
		| P_Fld(0x0, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO2)
		| P_Fld(0x26, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO3));
	vIO32WriteFldMulti(DRAMC_REG_PRE_TDQSCK3, P_Fld(0x13, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO4)
		| P_Fld(0x0, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO5)
		| P_Fld(0x40, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO6)
		| P_Fld(0x35, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO7));
	vIO32WriteFldMulti(DRAMC_REG_PRE_TDQSCK4, P_Fld(0x0, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO8)
		| P_Fld(0x0, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO9)
		| P_Fld(0x0, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO10)
		| P_Fld(0x0, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO11));
	vIO32WriteFldAlign(DRAMC_REG_RK0_PRE_TDQSCK1, 0xa, RK0_PRE_TDQSCK1_TDQSCK_UIFREQ2_B0R0);
	vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK2, P_Fld(0x0, RK0_PRE_TDQSCK2_TDQSCK_PIFREQ4_B0R0)
		| P_Fld(0x0, RK0_PRE_TDQSCK2_TDQSCK_UIFREQ4_B0R0)
		| P_Fld(0xb, RK0_PRE_TDQSCK2_TDQSCK_PIFREQ3_B0R0)
		| P_Fld(0xd, RK0_PRE_TDQSCK2_TDQSCK_UIFREQ3_B0R0));
	vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK3, P_Fld(0x0, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ4_P1_B0R0)
		| P_Fld(0x11, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ3_P1_B0R0)
		| P_Fld(0xe, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ2_P1_B0R0));
	vIO32WriteFldAlign(DRAMC_REG_RK0_PRE_TDQSCK4, 0xa, RK0_PRE_TDQSCK4_TDQSCK_UIFREQ2_B1R0);
	vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK5, P_Fld(0x0, RK0_PRE_TDQSCK5_TDQSCK_PIFREQ4_B1R0)
		| P_Fld(0x0, RK0_PRE_TDQSCK5_TDQSCK_UIFREQ4_B1R0)
		| P_Fld(0xb, RK0_PRE_TDQSCK5_TDQSCK_PIFREQ3_B1R0)
		| P_Fld(0xd, RK0_PRE_TDQSCK5_TDQSCK_UIFREQ3_B1R0));
	vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK6, P_Fld(0x0, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ4_P1_B1R0)
		| P_Fld(0x11, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ3_P1_B1R0)
		| P_Fld(0xe, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ2_P1_B1R0));
	vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK1, P_Fld(0x8, RK1_PRE_TDQSCK1_TDQSCK_PIFREQ2_B0R1)
		| P_Fld(0x10, RK1_PRE_TDQSCK1_TDQSCK_UIFREQ2_B0R1));
	vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK2, P_Fld(0x0, RK1_PRE_TDQSCK2_TDQSCK_PIFREQ4_B0R1)
		| P_Fld(0x0, RK1_PRE_TDQSCK2_TDQSCK_UIFREQ4_B0R1)
		| P_Fld(0xe, RK1_PRE_TDQSCK2_TDQSCK_PIFREQ3_B0R1)
		| P_Fld(0x10, RK1_PRE_TDQSCK2_TDQSCK_UIFREQ3_B0R1));
	vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK3, P_Fld(0x0, RK1_PRE_TDQSCK3_TDQSCK_UIFREQ4_P1_B0R1)
		| P_Fld(0x14, RK1_PRE_TDQSCK3_TDQSCK_UIFREQ3_P1_B0R1)
		| P_Fld(0x14, RK1_PRE_TDQSCK3_TDQSCK_UIFREQ2_P1_B0R1));
	vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK4, P_Fld(0x8, RK1_PRE_TDQSCK4_TDQSCK_PIFREQ2_B1R1)
		| P_Fld(0x10, RK1_PRE_TDQSCK4_TDQSCK_UIFREQ2_B1R1));
	vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK5, P_Fld(0x0, RK1_PRE_TDQSCK5_TDQSCK_PIFREQ4_B1R1)
		| P_Fld(0x0, RK1_PRE_TDQSCK5_TDQSCK_UIFREQ4_B1R1)
		| P_Fld(0xe, RK1_PRE_TDQSCK5_TDQSCK_PIFREQ3_B1R1)
		| P_Fld(0x10, RK1_PRE_TDQSCK5_TDQSCK_UIFREQ3_B1R1));
	vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK6, P_Fld(0x0, RK1_PRE_TDQSCK6_TDQSCK_UIFREQ4_P1_B1R1)
		| P_Fld(0x14, RK1_PRE_TDQSCK6_TDQSCK_UIFREQ3_P1_B1R1)
		| P_Fld(0x14, RK1_PRE_TDQSCK6_TDQSCK_UIFREQ2_P1_B1R1));
}

static void LegacyPreCalLP4_DDR3200(DRAMC_CTX_T *p)
{
	vIO32WriteFldMulti(DRAMC_REG_PRE_TDQSCK2, P_Fld(0x25, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO0)
		| P_Fld(0x18, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO1)
		| P_Fld(0x10, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO2)
		| P_Fld(0x1b, PRE_TDQSCK2_TDDQSCK_JUMP_RATIO3));
	vIO32WriteFldMulti(DRAMC_REG_PRE_TDQSCK3, P_Fld(0x14, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO4)
		| P_Fld(0xd, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO5)
		| P_Fld(0x2a, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO6)
		| P_Fld(0x31, PRE_TDQSCK3_TDDQSCK_JUMP_RATIO7));
	vIO32WriteFldMulti(DRAMC_REG_PRE_TDQSCK4, P_Fld(0x15, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO8)
		| P_Fld(0x40, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO9)
		| P_Fld(0x4a, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO10)
		| P_Fld(0x30, PRE_TDQSCK4_TDDQSCK_JUMP_RATIO11));
	vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK1, P_Fld(0x1a, RK0_PRE_TDQSCK1_TDQSCK_PIFREQ2_B0R0)
		| P_Fld(0x1c, RK0_PRE_TDQSCK1_TDQSCK_UIFREQ2_B0R0)
		| P_Fld(0x14, RK0_PRE_TDQSCK1_TDQSCK_UIFREQ1_B0R0));
	vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK2, P_Fld(0xb, RK0_PRE_TDQSCK2_TDQSCK_PIFREQ4_B0R0)
		| P_Fld(0xd, RK0_PRE_TDQSCK2_TDQSCK_UIFREQ4_B0R0)
		| P_Fld(0x10, RK0_PRE_TDQSCK2_TDQSCK_PIFREQ3_B0R0)
		| P_Fld(0xa, RK0_PRE_TDQSCK2_TDQSCK_UIFREQ3_B0R0));
	vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK3, P_Fld(0x11, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ4_P1_B0R0)
		| P_Fld(0xe, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ3_P1_B0R0)
		| P_Fld(0x20, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ2_P1_B0R0)
		| P_Fld(0x18, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ1_P1_B0R0));
	vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK4, P_Fld(0x1a, RK0_PRE_TDQSCK4_TDQSCK_PIFREQ2_B1R0)
		| P_Fld(0x1c, RK0_PRE_TDQSCK4_TDQSCK_UIFREQ2_B1R0)
		| P_Fld(0x14, RK0_PRE_TDQSCK4_TDQSCK_UIFREQ1_B1R0));
	vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK5, P_Fld(0xb, RK0_PRE_TDQSCK5_TDQSCK_PIFREQ4_B1R0)
		| P_Fld(0xd, RK0_PRE_TDQSCK5_TDQSCK_UIFREQ4_B1R0)
		| P_Fld(0x10, RK0_PRE_TDQSCK5_TDQSCK_PIFREQ3_B1R0)
		| P_Fld(0xa, RK0_PRE_TDQSCK5_TDQSCK_UIFREQ3_B1R0));
	vIO32WriteFldMulti(DRAMC_REG_RK0_PRE_TDQSCK6, P_Fld(0x11, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ4_P1_B1R0)
		| P_Fld(0xe, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ3_P1_B1R0)
		| P_Fld(0x20, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ2_P1_B1R0)
		| P_Fld(0x18, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ1_P1_B1R0));
	vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK1, P_Fld(0xb, RK1_PRE_TDQSCK1_TDQSCK_PIFREQ2_B0R1)
		| P_Fld(0x23, RK1_PRE_TDQSCK1_TDQSCK_UIFREQ2_B0R1)
		| P_Fld(0xf, RK1_PRE_TDQSCK1_TDQSCK_PIFREQ1_B0R1)
		| P_Fld(0x19, RK1_PRE_TDQSCK1_TDQSCK_UIFREQ1_B0R1));
	vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK2, P_Fld(0xe, RK1_PRE_TDQSCK2_TDQSCK_PIFREQ4_B0R1)
		| P_Fld(0x10, RK1_PRE_TDQSCK2_TDQSCK_UIFREQ4_B0R1)
		| P_Fld(0x1f, RK1_PRE_TDQSCK2_TDQSCK_PIFREQ3_B0R1)
		| P_Fld(0xe, RK1_PRE_TDQSCK2_TDQSCK_UIFREQ3_B0R1));
	vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK3, P_Fld(0x14, RK1_PRE_TDQSCK3_TDQSCK_UIFREQ4_P1_B0R1)
		| P_Fld(0x12, RK1_PRE_TDQSCK3_TDQSCK_UIFREQ3_P1_B0R1)
		| P_Fld(0x27, RK1_PRE_TDQSCK3_TDQSCK_UIFREQ2_P1_B0R1)
		| P_Fld(0x1d, RK1_PRE_TDQSCK3_TDQSCK_UIFREQ1_P1_B0R1));
	vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK4, P_Fld(0xb, RK1_PRE_TDQSCK4_TDQSCK_PIFREQ2_B1R1)
		| P_Fld(0x23, RK1_PRE_TDQSCK4_TDQSCK_UIFREQ2_B1R1)
		| P_Fld(0xf, RK1_PRE_TDQSCK4_TDQSCK_PIFREQ1_B1R1)
		| P_Fld(0x19, RK1_PRE_TDQSCK4_TDQSCK_UIFREQ1_B1R1));
	vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK5, P_Fld(0xe, RK1_PRE_TDQSCK5_TDQSCK_PIFREQ4_B1R1)
		| P_Fld(0x10, RK1_PRE_TDQSCK5_TDQSCK_UIFREQ4_B1R1)
		| P_Fld(0x1f, RK1_PRE_TDQSCK5_TDQSCK_PIFREQ3_B1R1)
		| P_Fld(0xe, RK1_PRE_TDQSCK5_TDQSCK_UIFREQ3_B1R1));
	vIO32WriteFldMulti(DRAMC_REG_RK1_PRE_TDQSCK6, P_Fld(0x14, RK1_PRE_TDQSCK6_TDQSCK_UIFREQ4_P1_B1R1)
		| P_Fld(0x12, RK1_PRE_TDQSCK6_TDQSCK_UIFREQ3_P1_B1R1)
		| P_Fld(0x27, RK1_PRE_TDQSCK6_TDQSCK_UIFREQ2_P1_B1R1)
		| P_Fld(0x1d, RK1_PRE_TDQSCK6_TDQSCK_UIFREQ1_P1_B1R1));
}
#endif

#if LEGACY_GATING_DLY
/*
 * Legacy initial settings (actual correct values are set during gating calibration)
 * Ex: TX_DLY_DQS#_GATED, TX_DLY_DQS#_GATED_P1, REG_DLY_DQS#_GATED, REG_DLY_DQS#_GATED_P1
 * TXDLY_B#_RODTEN, TXDLY_B#_RODTEN_P1, DLY_B#_RODTEN, DLY_B#_RODTEN_P1
 */
static void LegacyGatingDlyLP3(DRAMC_CTX_T *p)
{
	vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN0, P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B3_RODTEN_P1)
		| P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B3_RODTEN)
		| P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B2_RODTEN_P1)
		| P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B2_RODTEN)
		| P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN_P1)
		| P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN)
		| P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN_P1)
		| P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN));
	vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN1, P_Fld(0x4, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN_P1)
		| P_Fld(0x4, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN)
		| P_Fld(0x4, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN_P1)
		| P_Fld(0x4, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN)
		| P_Fld(0x4, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN_P1)
		| P_Fld(0x4, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN)
		| P_Fld(0x4, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN_P1)
		| P_Fld(0x4, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN));
	vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN0, P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN_P1)
		| P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN)
		| P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN_P1)
		| P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN)
		| P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN_P1)
		| P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN)
		| P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN_P1)
		| P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN));
	vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN1, P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B3_R1RODTEN_P1)
		| P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B3_R1RODTEN)
		| P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B2_R1RODTEN_P1)
		| P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B2_R1RODTEN)
		| P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN_P1)
		| P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN)
		| P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN_P1)
		| P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN));
	vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0, P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED_P1)
		| P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED)
		| P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED_P1)
		| P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED)
		| P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
		| P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
		| P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
		| P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
	vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(0x4, SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED_P1)
		| P_Fld(0x2, SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED)
		| P_Fld(0x4, SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED_P1)
		| P_Fld(0x2, SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED)
		| P_Fld(0x4, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
		| P_Fld(0x2, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
		| P_Fld(0x4, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
		| P_Fld(0x2, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
	vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0, P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS3_GATED_P1)
		| P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS3_GATED)
		| P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS2_GATED_P1)
		| P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS2_GATED)
		| P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
		| P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
		| P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
		| P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
	vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1, P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS3_GATED_P1)
		| P_Fld(0x7, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS3_GATED)
		| P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS2_GATED_P1)
		| P_Fld(0x7, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS2_GATED)
		| P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
		| P_Fld(0x7, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED)
		| P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1)
		| P_Fld(0x7, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED));
	vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED_P1)
		| P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED)
		| P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED_P1)
		| P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED)
		| P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
		| P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
		| P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
		| P_Fld(0x1, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
	vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1 + SHIFT_TO_CHB_ADDR, P_Fld(0x3, SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED_P1)
		| P_Fld(0x1, SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED)
		| P_Fld(0x3, SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED_P1)
		| P_Fld(0x1, SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED)
		| P_Fld(0x3, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
		| P_Fld(0x1, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
		| P_Fld(0x3, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
		| P_Fld(0x1, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
	vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0 + SHIFT_TO_CHB_ADDR, P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS3_GATED_P1)
		| P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS3_GATED)
		| P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS2_GATED_P1)
		| P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS2_GATED)
		| P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
		| P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
		| P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
		| P_Fld(0x1, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
	vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS3_GATED_P1)
		| P_Fld(0x7, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS3_GATED)
		| P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS2_GATED_P1)
		| P_Fld(0x7, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS2_GATED)
		| P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
		| P_Fld(0x7, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED)
		| P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1)
		| P_Fld(0x7, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED));
}

static void LegacyGatingDlyLP4_DDR2667(DRAMC_CTX_T *p)
{
	if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
	{
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN0, P_Fld(0x1, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN_P1)
			| P_Fld(0x1, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN)
			| P_Fld(0x1, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN_P1)
			| P_Fld(0x1, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN));
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN1, P_Fld(0x1, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN_P1)
			| P_Fld(0x1, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN)
			| P_Fld(0x1, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN_P1)
			| P_Fld(0x1, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN));
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0, P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
			| P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
			| P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
			| P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
			| P_Fld(0x2, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
			| P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
			| P_Fld(0x2, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
	}
	else
	{
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN0, P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN_P1)
			| P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN)
			| P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN_P1)
			| P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN));
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN1, P_Fld(0x1, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN_P1)
			| P_Fld(0x1, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN)
			| P_Fld(0x1, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN_P1)
			| P_Fld(0x1, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN));

		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0, P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
			| P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
			| P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
			| P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
			| P_Fld(0x2, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
			| P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
			| P_Fld(0x2, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
	}
	if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
	{
		vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN0, P_Fld(0x1, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN_P1)
			| P_Fld(0x1, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN)
			| P_Fld(0x1, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN_P1)
			| P_Fld(0x1, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN));
		vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN1, P_Fld(0x6, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN_P1)
			| P_Fld(0x6, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN)
			| P_Fld(0x6, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN_P1)
			| P_Fld(0x6, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN));
		vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0, P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
			| P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
			| P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
			| P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
	}
	else
	{
		vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN0, P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN_P1)
			| P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN)
			| P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN_P1)
			| P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN));
		vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN1, P_Fld(0x6, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN_P1)
			| P_Fld(0x6, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN)
			| P_Fld(0x6, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN_P1)
			| P_Fld(0x6, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN));
		vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0, P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
			| P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
			| P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
			| P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
	}
	vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1, P_Fld(0x4, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
		| P_Fld(0x0, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED)
		| P_Fld(0x4, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1)
		| P_Fld(0x0, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED));
}

static void LegacyGatingDlyLP4_DDR1600(DRAMC_CTX_T *p)
{
	vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN0, P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN_P1)
		| P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN)
		| P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN_P1)
		| P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN));
	vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN1, P_Fld(0x5, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN_P1)
		| P_Fld(0x5, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN)
		| P_Fld(0x5, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN_P1)
		| P_Fld(0x5, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN));
	if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
	{
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0, P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
			| P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
			| P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
			| P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
			| P_Fld(0x2, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
			| P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
			| P_Fld(0x2, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
	}
	else
	{
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0, P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
			| P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
			| P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
			| P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(0x0, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
			| P_Fld(0x4, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
			| P_Fld(0x0, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
			| P_Fld(0x4, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
	}
	vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN0, P_Fld(0x1, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN_P1)
		| P_Fld(0x1, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN)
		| P_Fld(0x1, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN_P1)
		| P_Fld(0x1, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN));
	vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN1, P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN_P1)
		| P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN)
		| P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN_P1)
		| P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN));
	if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
	{
		vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0, P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
			| P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
			| P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
			| P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
		vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1, P_Fld(0x0, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
			| P_Fld(0x4, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED)
			| P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1)
			| P_Fld(0x5, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED));
	}
	else
	{
		vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0, P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
			| P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
			| P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
			| P_Fld(0x2, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
		vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1, P_Fld(0x3, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
			| P_Fld(0x7, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED)
			| P_Fld(0x3, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1)
			| P_Fld(0x7, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED));
	}
}

static void LegacyGatingDlyLP4_DDR3200(DRAMC_CTX_T *p)
{
	if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
	{
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN1, P_Fld(0x2, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN_P1)
			| P_Fld(0x2, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN)
			| P_Fld(0x2, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN_P1)
			| P_Fld(0x2, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN)
			| P_Fld(0x6, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN_P1)
			| P_Fld(0x6, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN)
			| P_Fld(0x6, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN_P1)
			| P_Fld(0x6, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN));
		vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN0, P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN_P1)
			| P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN)
			| P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN_P1)
			| P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN)
			| P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN_P1)
			| P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN)
			| P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN_P1)
			| P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN));
		vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN1, P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B3_R1RODTEN_P1)
			| P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B3_R1RODTEN)
			| P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B2_R1RODTEN_P1)
			| P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B2_R1RODTEN)
			| P_Fld(0x2, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN_P1)
			| P_Fld(0x2, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN)
			| P_Fld(0x2, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN_P1)
			| P_Fld(0x2, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN));
	}
	else
	{
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN1, P_Fld(0x2, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN_P1)
			| P_Fld(0x2, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN)
			| P_Fld(0x2, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN_P1)
			| P_Fld(0x2, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN));
		vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN0, P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN_P1)
			| P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN)
			| P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN_P1)
			| P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN));
		vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN1, P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B3_R1RODTEN_P1)
			| P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B3_R1RODTEN)
			| P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B2_R1RODTEN_P1)
			| P_Fld(0x4, SHURK1_SELPH_ODTEN1_DLY_B2_R1RODTEN)
			| P_Fld(0x7, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN_P1)
			| P_Fld(0x7, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN)
			| P_Fld(0x7, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN_P1)
			| P_Fld(0x7, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN));
	}
	if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
	{
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0, P_Fld(0x4, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
			| P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
			| P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
			| P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(0x4, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
			| P_Fld(0x7, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
			| P_Fld(0x3, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
		vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0, P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
			| P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
			| P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
			| P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
		vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1, P_Fld(0x3, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
			| P_Fld(0x7, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED)
			| P_Fld(0x5, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1)
			| P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED));
	}
	else
	{
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0, P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
			| P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
			| P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
			| P_Fld(0x2, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
			| P_Fld(0x2, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
			| P_Fld(0x6, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
			| P_Fld(0x2, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
		vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0, P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
			| P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
			| P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
			| P_Fld(0x3, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
		vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1, P_Fld(0x4, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
			| P_Fld(0x4, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1));
	}
	if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
	{
	}
	else
	{
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG0, P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1)
			| P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED)
			| P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1)
			| P_Fld(0x3, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED));
		vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG0, P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED_P1)
			| P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED)
			| P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED_P1)
			| P_Fld(0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS0_GATED));
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN0, P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN)
			| P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN));
		vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN0, P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN)
			| P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN));
	}
	if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
	{
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN0, P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B3_RODTEN_P1)
			| P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B3_RODTEN)
			| P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B2_RODTEN_P1)
			| P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B2_RODTEN)
			| P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN_P1)
			| P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN)
			| P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN_P1)
			| P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN));
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN1, P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN_P1)
			| P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN)
			| P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN_P1)
			| P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN)
			| P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN_P1)
			| P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN)
			| P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN_P1)
			| P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN));
		vIO32WriteFldAlign(DRAMC_REG_SHURK0_SELPH_DQSG0, 0x4, SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1);
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(0x1, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
			| P_Fld(0x5, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
			| P_Fld(0x0, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
			| P_Fld(0x4, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
	}
	else
	{
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN0, P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B3_RODTEN_P1)
			| P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B3_RODTEN)
			| P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B2_RODTEN_P1)
			| P_Fld(0x0, SHURK0_SELPH_ODTEN0_TXDLY_B2_RODTEN)
			| P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B1_RODTEN_P1)
			| P_Fld(0x2, SHURK0_SELPH_ODTEN0_TXDLY_B0_RODTEN_P1));
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_ODTEN1, P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN_P1)
			| P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B3_RODTEN)
			| P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN_P1)
			| P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B2_RODTEN)
			| P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN_P1)
			| P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B1_RODTEN)
			| P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN_P1)
			| P_Fld(0x0, SHURK0_SELPH_ODTEN1_DLY_B0_RODTEN));
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQSG1, P_Fld(0x7, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1)
			| P_Fld(0x3, SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED)
			| P_Fld(0x7, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1)
			| P_Fld(0x3, SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED));
	}
	if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
	{
		vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN0, P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN_P1)
			| P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN)
			| P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN_P1)
			| P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN));
	}
	else
	{
		vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN0, P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN_P1)
			| P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B3_R1RODTEN)
			| P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN_P1)
			| P_Fld(0x0, SHURK1_SELPH_ODTEN0_TXDLY_B2_R1RODTEN)
			| P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B1_R1RODTEN_P1)
			| P_Fld(0x2, SHURK1_SELPH_ODTEN0_TXDLY_B0_R1RODTEN_P1));
	}
	vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_ODTEN1, P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B3_R1RODTEN_P1)
		| P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B3_R1RODTEN)
		| P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B2_R1RODTEN_P1)
		| P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B2_R1RODTEN)
		| P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN_P1)
		| P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B1_R1RODTEN)
		| P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN_P1)
		| P_Fld(0x0, SHURK1_SELPH_ODTEN1_DLY_B0_R1RODTEN));
	if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
	{
		vIO32WriteFldAlign(DRAMC_REG_SHURK1_SELPH_DQSG0, 0x4, SHURK1_SELPH_DQSG0_TX_DLY_R1DQS1_GATED);
		vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1, P_Fld(0x4, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
			| P_Fld(0x0, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED)
			| P_Fld(0x6, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1)
			| P_Fld(0x2, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED));
	}
	else
	{
		vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQSG1, P_Fld(0x5, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED_P1)
			| P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS1_GATED)
			| P_Fld(0x5, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED_P1)
			| P_Fld(0x1, SHURK1_SELPH_DQSG1_REG_DLY_R1DQS0_GATED));
	}
}
#endif /* LEGACY_GATING_DLY */

#if LEGACY_RX_DLY
/*
 * Legacy RX related delay initial settings:
 * RK#_RX_ARDQ#_F_DLY_B#, RK#_RX_ARDQ#_R_DLY_B# (DQ Rx per bit falling/rising edge delay line control)
 * RK#_RX_ARDQS#_F_DLY_B#, RK#_RX_ARDQS#_R_DLY_B# (DQS RX per bit falling/rising edge delay line control)
 * RK#_RX_ARDQM#_F_DLY_B#, RK#_RX_ARDQM#_R_DLY_B# (DQM RX per bit falling/rising edge delay line control)
 * TODO: Channel B's PHY, seems to be all used for LP3's DQ, DQS, DQM -> LP3's Channel B RX CA, CKE etc.. delay should be regarded as RX_DLY?
 */
#if ENABLE_LP3_SW
static void LegacyRxDly_LP3(DRAMC_CTX_T *p)
{
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ2, P_Fld(0x12, SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_R_DLY_B1)
		| P_Fld(0x12, SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ3, P_Fld(0x12, SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_R_DLY_B1)
		| P_Fld(0x12, SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ4, P_Fld(0x12, SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_R_DLY_B1)
		| P_Fld(0x12, SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ5, P_Fld(0x12, SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_R_DLY_B1)
		| P_Fld(0x12, SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ6, P_Fld(0x12, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1)
		| P_Fld(0x12, SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ2, P_Fld(0x13, SHU1_R1_B1_DQ2_RK1_RX_ARDQ1_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ2_RK1_RX_ARDQ1_R_DLY_B1)
		| P_Fld(0x13, SHU1_R1_B1_DQ2_RK1_RX_ARDQ0_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ2_RK1_RX_ARDQ0_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ3, P_Fld(0x13, SHU1_R1_B1_DQ3_RK1_RX_ARDQ3_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ3_RK1_RX_ARDQ3_R_DLY_B1)
		| P_Fld(0x13, SHU1_R1_B1_DQ3_RK1_RX_ARDQ2_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ3_RK1_RX_ARDQ2_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ4, P_Fld(0x13, SHU1_R1_B1_DQ4_RK1_RX_ARDQ5_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ4_RK1_RX_ARDQ5_R_DLY_B1)
		| P_Fld(0x13, SHU1_R1_B1_DQ4_RK1_RX_ARDQ4_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ4_RK1_RX_ARDQ4_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ5, P_Fld(0x13, SHU1_R1_B1_DQ5_RK1_RX_ARDQ7_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ5_RK1_RX_ARDQ7_R_DLY_B1)
		| P_Fld(0x13, SHU1_R1_B1_DQ5_RK1_RX_ARDQ6_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ5_RK1_RX_ARDQ6_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ6, P_Fld(0x13, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_R_DLY_B1)
		| P_Fld(0x13, SHU1_R1_B1_DQ6_RK1_RX_ARDQM0_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ6_RK1_RX_ARDQM0_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ2 + SHIFT_TO_CHB_ADDR, P_Fld(0x12, SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_F_DLY_B0)
		| P_Fld(0xa, SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_R_DLY_B0)
		| P_Fld(0x12, SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_F_DLY_B0)
		| P_Fld(0xa, SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ3 + SHIFT_TO_CHB_ADDR, P_Fld(0x12, SHU1_R0_B0_DQ3_RK0_RX_ARDQ3_F_DLY_B0)
		| P_Fld(0xa, SHU1_R0_B0_DQ3_RK0_RX_ARDQ3_R_DLY_B0)
		| P_Fld(0x12, SHU1_R0_B0_DQ3_RK0_RX_ARDQ2_F_DLY_B0)
		| P_Fld(0xa, SHU1_R0_B0_DQ3_RK0_RX_ARDQ2_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ4 + SHIFT_TO_CHB_ADDR, P_Fld(0x12, SHU1_R0_B0_DQ4_RK0_RX_ARDQ5_F_DLY_B0)
		| P_Fld(0xa, SHU1_R0_B0_DQ4_RK0_RX_ARDQ5_R_DLY_B0)
		| P_Fld(0x12, SHU1_R0_B0_DQ4_RK0_RX_ARDQ4_F_DLY_B0)
		| P_Fld(0xa, SHU1_R0_B0_DQ4_RK0_RX_ARDQ4_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ5 + SHIFT_TO_CHB_ADDR, P_Fld(0x12, SHU1_R0_B0_DQ5_RK0_RX_ARDQ7_F_DLY_B0)
		| P_Fld(0xa, SHU1_R0_B0_DQ5_RK0_RX_ARDQ7_R_DLY_B0)
		| P_Fld(0x12, SHU1_R0_B0_DQ5_RK0_RX_ARDQ6_F_DLY_B0)
		| P_Fld(0xa, SHU1_R0_B0_DQ5_RK0_RX_ARDQ6_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ6 + SHIFT_TO_CHB_ADDR, P_Fld(0x12, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_F_DLY_B0)
		| P_Fld(0xa, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0)
		| P_Fld(0x12, SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_F_DLY_B0)
		| P_Fld(0xa, SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ2 + SHIFT_TO_CHB_ADDR, P_Fld(0x13, SHU1_R1_B0_DQ2_RK1_RX_ARDQ1_F_DLY_B0)
		| P_Fld(0xb, SHU1_R1_B0_DQ2_RK1_RX_ARDQ1_R_DLY_B0)
		| P_Fld(0x13, SHU1_R1_B0_DQ2_RK1_RX_ARDQ0_F_DLY_B0)
		| P_Fld(0xb, SHU1_R1_B0_DQ2_RK1_RX_ARDQ0_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ3 + SHIFT_TO_CHB_ADDR, P_Fld(0x13, SHU1_R1_B0_DQ3_RK1_RX_ARDQ3_F_DLY_B0)
		| P_Fld(0xb, SHU1_R1_B0_DQ3_RK1_RX_ARDQ3_R_DLY_B0)
		| P_Fld(0x13, SHU1_R1_B0_DQ3_RK1_RX_ARDQ2_F_DLY_B0)
		| P_Fld(0xb, SHU1_R1_B0_DQ3_RK1_RX_ARDQ2_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ4 + SHIFT_TO_CHB_ADDR, P_Fld(0x13, SHU1_R1_B0_DQ4_RK1_RX_ARDQ5_F_DLY_B0)
		| P_Fld(0xb, SHU1_R1_B0_DQ4_RK1_RX_ARDQ5_R_DLY_B0)
		| P_Fld(0x13, SHU1_R1_B0_DQ4_RK1_RX_ARDQ4_F_DLY_B0)
		| P_Fld(0xb, SHU1_R1_B0_DQ4_RK1_RX_ARDQ4_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ5 + SHIFT_TO_CHB_ADDR, P_Fld(0x13, SHU1_R1_B0_DQ5_RK1_RX_ARDQ7_F_DLY_B0)
		| P_Fld(0xb, SHU1_R1_B0_DQ5_RK1_RX_ARDQ7_R_DLY_B0)
		| P_Fld(0x13, SHU1_R1_B0_DQ5_RK1_RX_ARDQ6_F_DLY_B0)
		| P_Fld(0xb, SHU1_R1_B0_DQ5_RK1_RX_ARDQ6_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ6 + SHIFT_TO_CHB_ADDR, P_Fld(0x13, SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_F_DLY_B0)
		| P_Fld(0xb, SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_R_DLY_B0)
		| P_Fld(0x13, SHU1_R1_B0_DQ6_RK1_RX_ARDQM0_F_DLY_B0)
		| P_Fld(0xb, SHU1_R1_B0_DQ6_RK1_RX_ARDQM0_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ2 + SHIFT_TO_CHB_ADDR, P_Fld(0x12, SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_R_DLY_B1)
		| P_Fld(0x12, SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ3 + SHIFT_TO_CHB_ADDR, P_Fld(0x12, SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_R_DLY_B1)
		| P_Fld(0x12, SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ4 + SHIFT_TO_CHB_ADDR, P_Fld(0x12, SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_R_DLY_B1)
		| P_Fld(0x12, SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ5 + SHIFT_TO_CHB_ADDR, P_Fld(0x12, SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_R_DLY_B1)
		| P_Fld(0x12, SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ6 + SHIFT_TO_CHB_ADDR, P_Fld(0x12, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1)
		| P_Fld(0x12, SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ2 + SHIFT_TO_CHB_ADDR, P_Fld(0x13, SHU1_R1_B1_DQ2_RK1_RX_ARDQ1_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ2_RK1_RX_ARDQ1_R_DLY_B1)
		| P_Fld(0x13, SHU1_R1_B1_DQ2_RK1_RX_ARDQ0_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ2_RK1_RX_ARDQ0_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ3 + SHIFT_TO_CHB_ADDR, P_Fld(0x13, SHU1_R1_B1_DQ3_RK1_RX_ARDQ3_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ3_RK1_RX_ARDQ3_R_DLY_B1)
		| P_Fld(0x13, SHU1_R1_B1_DQ3_RK1_RX_ARDQ2_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ3_RK1_RX_ARDQ2_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ4 + SHIFT_TO_CHB_ADDR, P_Fld(0x13, SHU1_R1_B1_DQ4_RK1_RX_ARDQ5_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ4_RK1_RX_ARDQ5_R_DLY_B1)
		| P_Fld(0x13, SHU1_R1_B1_DQ4_RK1_RX_ARDQ4_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ4_RK1_RX_ARDQ4_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ5 + SHIFT_TO_CHB_ADDR, P_Fld(0x13, SHU1_R1_B1_DQ5_RK1_RX_ARDQ7_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ5_RK1_RX_ARDQ7_R_DLY_B1)
		| P_Fld(0x13, SHU1_R1_B1_DQ5_RK1_RX_ARDQ6_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ5_RK1_RX_ARDQ6_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ6 + SHIFT_TO_CHB_ADDR, P_Fld(0x13, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_R_DLY_B1)
		| P_Fld(0x13, SHU1_R1_B1_DQ6_RK1_RX_ARDQM0_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ6_RK1_RX_ARDQM0_R_DLY_B1));
}
#endif /* ENABLE_LP3_SW */

static void LegacyRxDly_LP4_DDR2667(DRAMC_CTX_T *p)
{
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ2, P_Fld(0x12, SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_F_DLY_B0)
		| P_Fld(0xa, SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_R_DLY_B0)
		| P_Fld(0x12, SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_F_DLY_B0)
		| P_Fld(0xa, SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ3, P_Fld(0x12, SHU1_R0_B0_DQ3_RK0_RX_ARDQ3_F_DLY_B0)
		| P_Fld(0xa, SHU1_R0_B0_DQ3_RK0_RX_ARDQ3_R_DLY_B0)
		| P_Fld(0x12, SHU1_R0_B0_DQ3_RK0_RX_ARDQ2_F_DLY_B0)
		| P_Fld(0xa, SHU1_R0_B0_DQ3_RK0_RX_ARDQ2_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ4, P_Fld(0x12, SHU1_R0_B0_DQ4_RK0_RX_ARDQ5_F_DLY_B0)
		| P_Fld(0xa, SHU1_R0_B0_DQ4_RK0_RX_ARDQ5_R_DLY_B0)
		| P_Fld(0x12, SHU1_R0_B0_DQ4_RK0_RX_ARDQ4_F_DLY_B0)
		| P_Fld(0xa, SHU1_R0_B0_DQ4_RK0_RX_ARDQ4_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ5, P_Fld(0x12, SHU1_R0_B0_DQ5_RK0_RX_ARDQ7_F_DLY_B0)
		| P_Fld(0xa, SHU1_R0_B0_DQ5_RK0_RX_ARDQ7_R_DLY_B0)
		| P_Fld(0x12, SHU1_R0_B0_DQ5_RK0_RX_ARDQ6_F_DLY_B0)
		| P_Fld(0xa, SHU1_R0_B0_DQ5_RK0_RX_ARDQ6_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ6, P_Fld(0x1d, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_F_DLY_B0)
		| P_Fld(0x15, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0)
		| P_Fld(0x12, SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_F_DLY_B0)
		| P_Fld(0xa, SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ2, P_Fld(0x12, SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_R_DLY_B1)
		| P_Fld(0x12, SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ3, P_Fld(0x12, SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_R_DLY_B1)
		| P_Fld(0x12, SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ4, P_Fld(0x12, SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_R_DLY_B1)
		| P_Fld(0x12, SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ5, P_Fld(0x12, SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_R_DLY_B1)
		| P_Fld(0x12, SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ6, P_Fld(0x1d, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_F_DLY_B1)
		| P_Fld(0x15, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1)
		| P_Fld(0x12, SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ2, P_Fld(0x13, SHU1_R1_B0_DQ2_RK1_RX_ARDQ1_F_DLY_B0)
		| P_Fld(0xb, SHU1_R1_B0_DQ2_RK1_RX_ARDQ1_R_DLY_B0)
		| P_Fld(0x13, SHU1_R1_B0_DQ2_RK1_RX_ARDQ0_F_DLY_B0)
		| P_Fld(0xb, SHU1_R1_B0_DQ2_RK1_RX_ARDQ0_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ3, P_Fld(0x13, SHU1_R1_B0_DQ3_RK1_RX_ARDQ3_F_DLY_B0)
		| P_Fld(0xb, SHU1_R1_B0_DQ3_RK1_RX_ARDQ3_R_DLY_B0)
		| P_Fld(0x13, SHU1_R1_B0_DQ3_RK1_RX_ARDQ2_F_DLY_B0)
		| P_Fld(0xb, SHU1_R1_B0_DQ3_RK1_RX_ARDQ2_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ4, P_Fld(0x13, SHU1_R1_B0_DQ4_RK1_RX_ARDQ5_F_DLY_B0)
		| P_Fld(0xb, SHU1_R1_B0_DQ4_RK1_RX_ARDQ5_R_DLY_B0)
		| P_Fld(0x13, SHU1_R1_B0_DQ4_RK1_RX_ARDQ4_F_DLY_B0)
		| P_Fld(0xb, SHU1_R1_B0_DQ4_RK1_RX_ARDQ4_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ5, P_Fld(0x13, SHU1_R1_B0_DQ5_RK1_RX_ARDQ7_F_DLY_B0)
		| P_Fld(0xb, SHU1_R1_B0_DQ5_RK1_RX_ARDQ7_R_DLY_B0)
		| P_Fld(0x13, SHU1_R1_B0_DQ5_RK1_RX_ARDQ6_F_DLY_B0)
		| P_Fld(0xb, SHU1_R1_B0_DQ5_RK1_RX_ARDQ6_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ6, P_Fld(0x1e, SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_F_DLY_B0)
		| P_Fld(0x16, SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_R_DLY_B0)
		| P_Fld(0x13, SHU1_R1_B0_DQ6_RK1_RX_ARDQM0_F_DLY_B0)
		| P_Fld(0xb, SHU1_R1_B0_DQ6_RK1_RX_ARDQM0_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ2, P_Fld(0x13, SHU1_R1_B1_DQ2_RK1_RX_ARDQ1_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ2_RK1_RX_ARDQ1_R_DLY_B1)
		| P_Fld(0x13, SHU1_R1_B1_DQ2_RK1_RX_ARDQ0_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ2_RK1_RX_ARDQ0_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ3, P_Fld(0x13, SHU1_R1_B1_DQ3_RK1_RX_ARDQ3_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ3_RK1_RX_ARDQ3_R_DLY_B1)
		| P_Fld(0x13, SHU1_R1_B1_DQ3_RK1_RX_ARDQ2_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ3_RK1_RX_ARDQ2_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ4, P_Fld(0x13, SHU1_R1_B1_DQ4_RK1_RX_ARDQ5_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ4_RK1_RX_ARDQ5_R_DLY_B1)
		| P_Fld(0x13, SHU1_R1_B1_DQ4_RK1_RX_ARDQ4_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ4_RK1_RX_ARDQ4_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ5, P_Fld(0x13, SHU1_R1_B1_DQ5_RK1_RX_ARDQ7_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ5_RK1_RX_ARDQ7_R_DLY_B1)
		| P_Fld(0x13, SHU1_R1_B1_DQ5_RK1_RX_ARDQ6_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ5_RK1_RX_ARDQ6_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ6, P_Fld(0x1e, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_F_DLY_B1)
		| P_Fld(0x16, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_R_DLY_B1)
		| P_Fld(0x13, SHU1_R1_B1_DQ6_RK1_RX_ARDQM0_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ6_RK1_RX_ARDQM0_R_DLY_B1));
}

static void LegacyRxDly_LP4_DDR1600(DRAMC_CTX_T *p)
{
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ2, P_Fld(0xc, SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_F_DLY_B0)
		| P_Fld(0x4, SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_R_DLY_B0)
		| P_Fld(0xc, SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_F_DLY_B0)
		| P_Fld(0x4, SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ3, P_Fld(0xc, SHU1_R0_B0_DQ3_RK0_RX_ARDQ3_F_DLY_B0)
		| P_Fld(0x4, SHU1_R0_B0_DQ3_RK0_RX_ARDQ3_R_DLY_B0)
		| P_Fld(0xc, SHU1_R0_B0_DQ3_RK0_RX_ARDQ2_F_DLY_B0)
		| P_Fld(0x4, SHU1_R0_B0_DQ3_RK0_RX_ARDQ2_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ4, P_Fld(0xc, SHU1_R0_B0_DQ4_RK0_RX_ARDQ5_F_DLY_B0)
		| P_Fld(0x4, SHU1_R0_B0_DQ4_RK0_RX_ARDQ5_R_DLY_B0)
		| P_Fld(0xc, SHU1_R0_B0_DQ4_RK0_RX_ARDQ4_F_DLY_B0)
		| P_Fld(0x4, SHU1_R0_B0_DQ4_RK0_RX_ARDQ4_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ5, P_Fld(0xc, SHU1_R0_B0_DQ5_RK0_RX_ARDQ7_F_DLY_B0)
		| P_Fld(0x4, SHU1_R0_B0_DQ5_RK0_RX_ARDQ7_R_DLY_B0)
		| P_Fld(0xc, SHU1_R0_B0_DQ5_RK0_RX_ARDQ6_F_DLY_B0)
		| P_Fld(0x4, SHU1_R0_B0_DQ5_RK0_RX_ARDQ6_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ6, P_Fld(0x1d, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_F_DLY_B0)
		| P_Fld(0x15, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0)
		| P_Fld(0xc, SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_F_DLY_B0)
		| P_Fld(0x4, SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ2, P_Fld(0xc, SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_F_DLY_B1)
		| P_Fld(0x4, SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_R_DLY_B1)
		| P_Fld(0xc, SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_F_DLY_B1)
		| P_Fld(0x4, SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ3, P_Fld(0xc, SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_F_DLY_B1)
		| P_Fld(0x4, SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_R_DLY_B1)
		| P_Fld(0xc, SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_F_DLY_B1)
		| P_Fld(0x4, SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ4, P_Fld(0xc, SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_F_DLY_B1)
		| P_Fld(0x4, SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_R_DLY_B1)
		| P_Fld(0xc, SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_F_DLY_B1)
		| P_Fld(0x4, SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ5, P_Fld(0xc, SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_F_DLY_B1)
		| P_Fld(0x4, SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_R_DLY_B1)
		| P_Fld(0xc, SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_F_DLY_B1)
		| P_Fld(0x4, SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ6, P_Fld(0x1d, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_F_DLY_B1)
		| P_Fld(0x15, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1)
		| P_Fld(0xc, SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_F_DLY_B1)
		| P_Fld(0x4, SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ2, P_Fld(0xd, SHU1_R1_B0_DQ2_RK1_RX_ARDQ1_F_DLY_B0)
		| P_Fld(0x5, SHU1_R1_B0_DQ2_RK1_RX_ARDQ1_R_DLY_B0)
		| P_Fld(0xd, SHU1_R1_B0_DQ2_RK1_RX_ARDQ0_F_DLY_B0)
		| P_Fld(0x5, SHU1_R1_B0_DQ2_RK1_RX_ARDQ0_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ3, P_Fld(0xd, SHU1_R1_B0_DQ3_RK1_RX_ARDQ3_F_DLY_B0)
		| P_Fld(0x5, SHU1_R1_B0_DQ3_RK1_RX_ARDQ3_R_DLY_B0)
		| P_Fld(0xd, SHU1_R1_B0_DQ3_RK1_RX_ARDQ2_F_DLY_B0)
		| P_Fld(0x5, SHU1_R1_B0_DQ3_RK1_RX_ARDQ2_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ4, P_Fld(0xd, SHU1_R1_B0_DQ4_RK1_RX_ARDQ5_F_DLY_B0)
		| P_Fld(0x5, SHU1_R1_B0_DQ4_RK1_RX_ARDQ5_R_DLY_B0)
		| P_Fld(0xd, SHU1_R1_B0_DQ4_RK1_RX_ARDQ4_F_DLY_B0)
		| P_Fld(0x5, SHU1_R1_B0_DQ4_RK1_RX_ARDQ4_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ5, P_Fld(0xd, SHU1_R1_B0_DQ5_RK1_RX_ARDQ7_F_DLY_B0)
		| P_Fld(0x5, SHU1_R1_B0_DQ5_RK1_RX_ARDQ7_R_DLY_B0)
		| P_Fld(0xd, SHU1_R1_B0_DQ5_RK1_RX_ARDQ6_F_DLY_B0)
		| P_Fld(0x5, SHU1_R1_B0_DQ5_RK1_RX_ARDQ6_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ6, P_Fld(0x1e, SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_F_DLY_B0)
		| P_Fld(0x16, SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_R_DLY_B0)
		| P_Fld(0xd, SHU1_R1_B0_DQ6_RK1_RX_ARDQM0_F_DLY_B0)
		| P_Fld(0x5, SHU1_R1_B0_DQ6_RK1_RX_ARDQM0_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ2, P_Fld(0xd, SHU1_R1_B1_DQ2_RK1_RX_ARDQ1_F_DLY_B1)
		| P_Fld(0x5, SHU1_R1_B1_DQ2_RK1_RX_ARDQ1_R_DLY_B1)
		| P_Fld(0xd, SHU1_R1_B1_DQ2_RK1_RX_ARDQ0_F_DLY_B1)
		| P_Fld(0x5, SHU1_R1_B1_DQ2_RK1_RX_ARDQ0_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ3, P_Fld(0xd, SHU1_R1_B1_DQ3_RK1_RX_ARDQ3_F_DLY_B1)
		| P_Fld(0x5, SHU1_R1_B1_DQ3_RK1_RX_ARDQ3_R_DLY_B1)
		| P_Fld(0xd, SHU1_R1_B1_DQ3_RK1_RX_ARDQ2_F_DLY_B1)
		| P_Fld(0x5, SHU1_R1_B1_DQ3_RK1_RX_ARDQ2_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ4, P_Fld(0xd, SHU1_R1_B1_DQ4_RK1_RX_ARDQ5_F_DLY_B1)
		| P_Fld(0x5, SHU1_R1_B1_DQ4_RK1_RX_ARDQ5_R_DLY_B1)
		| P_Fld(0xd, SHU1_R1_B1_DQ4_RK1_RX_ARDQ4_F_DLY_B1)
		| P_Fld(0x5, SHU1_R1_B1_DQ4_RK1_RX_ARDQ4_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ5, P_Fld(0xd, SHU1_R1_B1_DQ5_RK1_RX_ARDQ7_F_DLY_B1)
		| P_Fld(0x5, SHU1_R1_B1_DQ5_RK1_RX_ARDQ7_R_DLY_B1)
		| P_Fld(0xd, SHU1_R1_B1_DQ5_RK1_RX_ARDQ6_F_DLY_B1)
		| P_Fld(0x5, SHU1_R1_B1_DQ5_RK1_RX_ARDQ6_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ6, P_Fld(0x1e, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_F_DLY_B1)
		| P_Fld(0x16, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_R_DLY_B1)
		| P_Fld(0xd, SHU1_R1_B1_DQ6_RK1_RX_ARDQM0_F_DLY_B1)
		| P_Fld(0x5, SHU1_R1_B1_DQ6_RK1_RX_ARDQM0_R_DLY_B1));
}

static void LegacyRxDly_LP4_DDR3200(DRAMC_CTX_T *p)
{
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ2, P_Fld(0x12, SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_F_DLY_B0)
		| P_Fld(0xa, SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_R_DLY_B0)
		| P_Fld(0x12, SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_F_DLY_B0)
		| P_Fld(0xa, SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ3, P_Fld(0x12, SHU1_R0_B0_DQ3_RK0_RX_ARDQ3_F_DLY_B0)
		| P_Fld(0xa, SHU1_R0_B0_DQ3_RK0_RX_ARDQ3_R_DLY_B0)
		| P_Fld(0x12, SHU1_R0_B0_DQ3_RK0_RX_ARDQ2_F_DLY_B0)
		| P_Fld(0xa, SHU1_R0_B0_DQ3_RK0_RX_ARDQ2_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ4, P_Fld(0x12, SHU1_R0_B0_DQ4_RK0_RX_ARDQ5_F_DLY_B0)
		| P_Fld(0xa, SHU1_R0_B0_DQ4_RK0_RX_ARDQ5_R_DLY_B0)
		| P_Fld(0x12, SHU1_R0_B0_DQ4_RK0_RX_ARDQ4_F_DLY_B0)
		| P_Fld(0xa, SHU1_R0_B0_DQ4_RK0_RX_ARDQ4_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ5, P_Fld(0x12, SHU1_R0_B0_DQ5_RK0_RX_ARDQ7_F_DLY_B0)
		| P_Fld(0xa, SHU1_R0_B0_DQ5_RK0_RX_ARDQ7_R_DLY_B0)
		| P_Fld(0x12, SHU1_R0_B0_DQ5_RK0_RX_ARDQ6_F_DLY_B0)
		| P_Fld(0xa, SHU1_R0_B0_DQ5_RK0_RX_ARDQ6_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ6, P_Fld(0x14, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_F_DLY_B0)
		| P_Fld(0xc, SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0)
		| P_Fld(0x12, SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_F_DLY_B0)
		| P_Fld(0xa, SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ2, P_Fld(0x13, SHU1_R1_B0_DQ2_RK1_RX_ARDQ1_F_DLY_B0)
		| P_Fld(0xb, SHU1_R1_B0_DQ2_RK1_RX_ARDQ1_R_DLY_B0)
		| P_Fld(0x13, SHU1_R1_B0_DQ2_RK1_RX_ARDQ0_F_DLY_B0)
		| P_Fld(0xb, SHU1_R1_B0_DQ2_RK1_RX_ARDQ0_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ3, P_Fld(0x13, SHU1_R1_B0_DQ3_RK1_RX_ARDQ3_F_DLY_B0)
		| P_Fld(0xb, SHU1_R1_B0_DQ3_RK1_RX_ARDQ3_R_DLY_B0)
		| P_Fld(0x13, SHU1_R1_B0_DQ3_RK1_RX_ARDQ2_F_DLY_B0)
		| P_Fld(0xb, SHU1_R1_B0_DQ3_RK1_RX_ARDQ2_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ4, P_Fld(0x13, SHU1_R1_B0_DQ4_RK1_RX_ARDQ5_F_DLY_B0)
		| P_Fld(0xb, SHU1_R1_B0_DQ4_RK1_RX_ARDQ5_R_DLY_B0)
		| P_Fld(0x13, SHU1_R1_B0_DQ4_RK1_RX_ARDQ4_F_DLY_B0)
		| P_Fld(0xb, SHU1_R1_B0_DQ4_RK1_RX_ARDQ4_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ5, P_Fld(0x13, SHU1_R1_B0_DQ5_RK1_RX_ARDQ7_F_DLY_B0)
		| P_Fld(0xb, SHU1_R1_B0_DQ5_RK1_RX_ARDQ7_R_DLY_B0)
		| P_Fld(0x13, SHU1_R1_B0_DQ5_RK1_RX_ARDQ6_F_DLY_B0)
		| P_Fld(0xb, SHU1_R1_B0_DQ5_RK1_RX_ARDQ6_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ6, P_Fld(0x15, SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_F_DLY_B0)
		| P_Fld(0xd, SHU1_R1_B0_DQ6_RK1_RX_ARDQS0_R_DLY_B0)
		| P_Fld(0x13, SHU1_R1_B0_DQ6_RK1_RX_ARDQM0_F_DLY_B0)
		| P_Fld(0xb, SHU1_R1_B0_DQ6_RK1_RX_ARDQM0_R_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ2, P_Fld(0x12, SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ2_RK0_RX_ARDQ1_R_DLY_B1)
		| P_Fld(0x12, SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ2_RK0_RX_ARDQ0_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ3, P_Fld(0x12, SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ3_RK0_RX_ARDQ3_R_DLY_B1)
		| P_Fld(0x12, SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ3_RK0_RX_ARDQ2_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ4, P_Fld(0x12, SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ4_RK0_RX_ARDQ5_R_DLY_B1)
		| P_Fld(0x12, SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ4_RK0_RX_ARDQ4_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ5, P_Fld(0x12, SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ5_RK0_RX_ARDQ7_R_DLY_B1)
		| P_Fld(0x12, SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ5_RK0_RX_ARDQ6_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ6, P_Fld(0x14, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_F_DLY_B1)
		| P_Fld(0xc, SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1)
		| P_Fld(0x12, SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_F_DLY_B1)
		| P_Fld(0xa, SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ2, P_Fld(0x13, SHU1_R1_B1_DQ2_RK1_RX_ARDQ1_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ2_RK1_RX_ARDQ1_R_DLY_B1)
		| P_Fld(0x13, SHU1_R1_B1_DQ2_RK1_RX_ARDQ0_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ2_RK1_RX_ARDQ0_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ3, P_Fld(0x13, SHU1_R1_B1_DQ3_RK1_RX_ARDQ3_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ3_RK1_RX_ARDQ3_R_DLY_B1)
		| P_Fld(0x13, SHU1_R1_B1_DQ3_RK1_RX_ARDQ2_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ3_RK1_RX_ARDQ2_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ4, P_Fld(0x13, SHU1_R1_B1_DQ4_RK1_RX_ARDQ5_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ4_RK1_RX_ARDQ5_R_DLY_B1)
		| P_Fld(0x13, SHU1_R1_B1_DQ4_RK1_RX_ARDQ4_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ4_RK1_RX_ARDQ4_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ5, P_Fld(0x13, SHU1_R1_B1_DQ5_RK1_RX_ARDQ7_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ5_RK1_RX_ARDQ7_R_DLY_B1)
		| P_Fld(0x13, SHU1_R1_B1_DQ5_RK1_RX_ARDQ6_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ5_RK1_RX_ARDQ6_R_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ6, P_Fld(0x15, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_F_DLY_B1)
		| P_Fld(0xd, SHU1_R1_B1_DQ6_RK1_RX_ARDQS0_R_DLY_B1)
		| P_Fld(0x13, SHU1_R1_B1_DQ6_RK1_RX_ARDQM0_F_DLY_B1)
		| P_Fld(0xb, SHU1_R1_B1_DQ6_RK1_RX_ARDQM0_R_DLY_B1));
}
#endif /* LEGACY_RX_DLY */

#if ENABLE_LP3_SW
static void DramcSetting_Olympus_LP3(DRAMC_CTX_T *p)
{
	AutoRefreshCKEOff(p);

#if (fcFOR_CHIP_ID == fcTalbot)                                        //before switch clock from 26M to PHY, need to init PHY clock first
	vIO32WriteFldMulti(DDRPHY_CKMUX_SEL, P_Fld(0x1, CKMUX_SEL_R_PHYCTRLMUX)  /* move CKMUX_SEL_R_PHYCTRLMUX to here (it was originally between MISC_CG_CTRL0_CLK_MEM_SEL and MISC_CTRL0_R_DMRDSEL_DIV2_OPT) */
		| P_Fld(0x1, CKMUX_SEL_R_PHYCTRLDCM)); /* PHYCTRLDCM 1: follow DDRPHY_conf DCM settings, 0: follow infra DCM settings */
#endif

	/* chg_mem_en = 1 */
	vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 0x1, MISC_CG_CTRL0_W_CHG_MEM);

	/* 26M */
	vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 0x0, MISC_CG_CTRL0_CLK_MEM_SEL);


	vIO32WriteFldAlign_All(DDRPHY_MISC_CTRL0, 0x1, MISC_CTRL0_R_DMRDSEL_DIV2_OPT);
	/* vIO32WriteFldAlign(DDRPHY_MISC_CTRL0+SHIFT_TO_CHB_ADDR, 0x1, MISC_CTRL0_R_DMRDSEL_DIV2_OPT); //Use _All() instead */

#if (fcFOR_CHIP_ID == fcTalbot)
	if (p->bDLP3)
	{
		vIO32WriteFldAlign_All(DDRPHY_MISC_CTRL1, 0x1, MISC_CTRL1_R_DMMUXCA_SEC);
	}
#endif

	vIO32WriteFldMulti(DDRPHY_MISC_SPM_CTRL1, P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10) | P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_B0)
		| P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_B1) | P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_CA));
	vIO32WriteFldMulti(DDRPHY_MISC_SPM_CTRL1 + SHIFT_TO_CHB_ADDR, P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10) | P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_B0)
		| P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_B1) | P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_CA));

	vIO32WriteFldAlign_All(DDRPHY_MISC_SPM_CTRL0, 0xffffffff, MISC_SPM_CTRL0_PHY_SPM_CTL0);
	/* vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL0+SHIFT_TO_CHB_ADDR, 0xffffffff, MISC_SPM_CTRL0_PHY_SPM_CTL0); //Use _All() instead */
	vIO32WriteFldAlign_All(DDRPHY_MISC_SPM_CTRL2, 0xffffffff, MISC_SPM_CTRL2_PHY_SPM_CTL2);
	/* vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL2+SHIFT_TO_CHB_ADDR, 0xffffffff, MISC_SPM_CTRL2_PHY_SPM_CTL2); //Use _All() instead */
	vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL2, 0x6003a1, MISC_CG_CTRL2_RG_MEM_DCM_CTL);
	vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL4, 0x13300000, MISC_CG_CTRL4_R_PHY_MCK_CG_CTRL);
	vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL4 + SHIFT_TO_CHB_ADDR, 0x222a2a00, MISC_CG_CTRL4_R_PHY_MCK_CG_CTRL);
	vIO32WriteFldMulti(DDRPHY_SHU1_PLL1, P_Fld(0x1, SHU1_PLL1_R_SHU_AUTO_PLL_MUX)
		| P_Fld(0x7, SHU1_PLL1_SHU1_PLL1_RFU));
	vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7, P_Fld(0x1, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0)
		| P_Fld(0x1, SHU1_B0_DQ7_MIDPI_DIV4_ENABLE)
		| P_Fld(0x0, SHU1_B0_DQ7_R_DMRANKRXDVS_B0));
	vIO32WriteFldMulti_All(DDRPHY_SHU1_B1_DQ7, P_Fld(0x1, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1)
		| P_Fld(0x1, SHU1_B1_DQ7_R_DMRANKRXDVS_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0)
		| P_Fld(0x1, SHU1_B0_DQ7_MIDPI_DIV4_ENABLE)
		| P_Fld(0x1, SHU1_B0_DQ7_R_DMRANKRXDVS_B0));

#if 0 /* Use _All() instead */
	vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ7 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1) /* Use _All() instead */
		| P_Fld(0x1, SHU1_B1_DQ7_R_DMRANKRXDVS_B1));
#endif
	vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD7 + SHIFT_TO_CHB_ADDR, P_Fld(0x0, SHU1_CA_CMD7_R_DMRXDVS_PBYTE_FLAG_OPT_CA) /* Wei - jen: no rx_dly_track in LP3 */
		| P_Fld(0x1, SHU1_CA_CMD7_R_DMRANKRXDVS_CA));
	/*
	 * vIO32WriteFldAlign(DDRPHY_SHU1_PLL4, 0xfe, SHU1_PLL4_RG_RPHYPLL_RESERVED);
	 * vIO32WriteFldAlign(DDRPHY_SHU1_PLL6, 0xfe, SHU1_PLL6_RG_RCLRPLL_RESERVED);
	 */
	vIO32WriteFldMulti_All(DDRPHY_CA_CMD2, P_Fld(0x0, CA_CMD2_RG_TX_ARCMD_OE_DIS)
		| P_Fld(0x0, CA_CMD2_RG_TX_ARCMD_ODTEN_DIS)
		| P_Fld(0x0, CA_CMD2_RG_TX_ARCLK_OE_DIS)
		| P_Fld(0x0, CA_CMD2_RG_TX_ARCLK_ODTEN_DIS));
	vIO32WriteFldMulti_All(DDRPHY_B0_DQ2, P_Fld(0x0, B0_DQ2_RG_TX_ARDQ_OE_DIS_B0)
		| P_Fld(0x0, B0_DQ2_RG_TX_ARDQ_ODTEN_DIS_B0)
		| P_Fld(0x0, B0_DQ2_RG_TX_ARDQS0_OE_DIS_B0)
		| P_Fld(0x0, B0_DQ2_RG_TX_ARDQS0_ODTEN_DIS_B0));
	vIO32WriteFldMulti_All(DDRPHY_B1_DQ2, P_Fld(0x0, B1_DQ2_RG_TX_ARDQ_OE_DIS_B1)
		| P_Fld(0x0, B1_DQ2_RG_TX_ARDQ_ODTEN_DIS_B1)
		| P_Fld(0x0, B1_DQ2_RG_TX_ARDQS0_OE_DIS_B1)
		| P_Fld(0x0, B1_DQ2_RG_TX_ARDQS0_ODTEN_DIS_B1));
#if 0 /* Correct settings are set in UpdateInitialSettings_LP3() */
	vIO32WriteFldAlign(DDRPHY_B0_DQ9, 0x0, B0_DQ9_R_IN_GATE_EN_LOW_OPT_B0);
	vIO32WriteFldAlign(DDRPHY_B1_DQ9, 0x7, B1_DQ9_R_IN_GATE_EN_LOW_OPT_B1);
	vIO32WriteFldAlign(DDRPHY_CA_CMD10, 0x0, CA_CMD10_R_IN_GATE_EN_LOW_OPT_CA);
#endif
#if 0 /* Use _All() instead */
	vIO32WriteFldMulti(DDRPHY_CA_CMD2 + SHIFT_TO_CHB_ADDR, P_Fld(0x0, CA_CMD2_RG_TX_ARCMD_OE_DIS)
		| P_Fld(0x0, CA_CMD2_RG_TX_ARCMD_ODTEN_DIS)
		| P_Fld(0x0, CA_CMD2_RG_TX_ARCLK_OE_DIS)
		| P_Fld(0x0, CA_CMD2_RG_TX_ARCLK_ODTEN_DIS));
	vIO32WriteFldMulti(DDRPHY_B0_DQ2 + SHIFT_TO_CHB_ADDR, P_Fld(0x0, B0_DQ2_RG_TX_ARDQ_OE_DIS_B0)
		| P_Fld(0x0, B0_DQ2_RG_TX_ARDQ_ODTEN_DIS_B0)
		| P_Fld(0x0, B0_DQ2_RG_TX_ARDQS_OE_DIS_B0)
		| P_Fld(0x0, B0_DQ2_RG_TX_ARDQS_ODTEN_DIS_B0));
	vIO32WriteFldMulti(DDRPHY_B1_DQ2 + SHIFT_TO_CHB_ADDR, P_Fld(0x0, B1_DQ2_RG_TX_ARDQ_OE_DIS_B1)
		| P_Fld(0x0, B1_DQ2_RG_TX_ARDQ_ODTEN_DIS_B1)
		| P_Fld(0x0, B1_DQ2_RG_TX_ARDQS_OE_DIS_B1)
		| P_Fld(0x0, B1_DQ2_RG_TX_ARDQS_ODTEN_DIS_B1));
#endif
#if 0 /* Correct settings are set in UpdateInitialSettings_LP3() */
	vIO32WriteFldAlign(DDRPHY_B0_DQ9 + SHIFT_TO_CHB_ADDR, 0x7, B0_DQ9_R_IN_GATE_EN_LOW_OPT_B0);
	vIO32WriteFldAlign(DDRPHY_B1_DQ9 + SHIFT_TO_CHB_ADDR, 0x7, B1_DQ9_R_IN_GATE_EN_LOW_OPT_B1);
	vIO32WriteFldAlign(DDRPHY_CA_CMD10 + SHIFT_TO_CHB_ADDR, 0x7, CA_CMD10_R_IN_GATE_EN_LOW_OPT_CA);
#endif
	vIO32WriteFldMulti_All(DDRPHY_MISC_CTRL1, P_Fld(0x1, MISC_CTRL1_R_DMPINMUX)
		| P_Fld(0x0, MISC_CTRL1_R_DMARPIDQ_SW)
		| P_Fld(0x1, MISC_CTRL1_R_DMMCTLPLL_CKSEL));
#if 0 /* Use _All() instead */
	vIO32WriteFldMulti(DDRPHY_MISC_CTRL1 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, MISC_CTRL1_R_DMPINMUX)
		| P_Fld(0x0, MISC_CTRL1_R_DMARPIDQ_SW)
		| P_Fld(0x1, MISC_CTRL1_R_DMMCTLPLL_CKSEL));
#endif
	vIO32WriteFldAlign(DDRPHY_PLL3, 0x0, PLL3_RG_RPHYPLL_TSTOP_EN);

	vIO32WriteFldAlign_All(DDRPHY_MISC_VREF_CTRL, 0x1, MISC_VREF_CTRL_RG_RVREF_VREF_EN);
	vIO32WriteFldMulti_All(DDRPHY_SHU1_MISC0, P_Fld(0x0, SHU1_MISC0_RG_RVREF_SEL_CMD)
		| P_Fld(0x1, SHU1_MISC0_RG_RVREF_DDR3_SEL)
		| P_Fld(0x0, SHU1_MISC0_RG_RVREF_DDR4_SEL)
		| P_Fld(0xb, SHU1_MISC0_RG_RVREF_SEL_DQ));
	vIO32WriteFldAlign_All(DDRPHY_MISC_IMP_CTRL0, 0x1, MISC_IMP_CTRL0_RG_RIMP_DDR3_SEL);
#if 0 /* Use _All() instead */
	vIO32WriteFldAlign(DDRPHY_MISC_VREF_CTRL + SHIFT_TO_CHB_ADDR, 0x1, MISC_VREF_CTRL_RG_RVREF_VREF_EN);
	vIO32WriteFldMulti(DDRPHY_SHU1_MISC0 + SHIFT_TO_CHB_ADDR, P_Fld(0x0, SHU1_MISC0_RG_RVREF_SEL_CMD)
		| P_Fld(0x1, SHU1_MISC0_RG_RVREF_DDR3_SEL)
		| P_Fld(0x0, SHU1_MISC0_RG_RVREF_DDR4_SEL)
		| P_Fld(0xb, SHU1_MISC0_RG_RVREF_SEL_DQ));
	vIO32WriteFldAlign(DDRPHY_MISC_IMP_CTRL0 + SHIFT_TO_CHB_ADDR, 0x1, MISC_IMP_CTRL0_RG_RIMP_DDR3_SEL);
#endif
	vIO32WriteFldAlign(DDRPHY_B1_RXDVS0, 0x1, B1_RXDVS0_R_RX_DLY_TRACK_CG_EN_B1);
	vIO32WriteFldMulti(DDRPHY_B0_RXDVS0 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, B0_RXDVS0_R_RX_DLY_TRACK_CG_EN_B0)
		| P_Fld(0x0, B0_RXDVS0_R_RX_RANKINCTL_B0));
	vIO32WriteFldMulti(DDRPHY_CA_RXDVS0 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, CA_RXDVS0_R_RX_DLY_TRACK_CG_EN_CA)
		| P_Fld(0x0, CA_RXDVS0_R_RX_RANKINCTL_CA));
	vIO32WriteFldMulti(DDRPHY_B1_RXDVS0 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, B1_RXDVS0_R_RX_DLY_TRACK_CG_EN_B1)
		| P_Fld(0x0, B1_RXDVS0_R_RX_RANKINCTL_B1));
	vIO32WriteFldMulti(DDRPHY_B1_RXDVS0, P_Fld(0x1, B1_RXDVS0_R_DMRXDVS_CNTCMP_OPT_B1)
		| P_Fld(0x1, B1_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_B1));
	vIO32WriteFldMulti(DDRPHY_B0_RXDVS0 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, B0_RXDVS0_R_DMRXDVS_CNTCMP_OPT_B0)
		| P_Fld(0x1, B0_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_B0));
	vIO32WriteFldMulti(DDRPHY_B1_RXDVS0 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, B1_RXDVS0_R_DMRXDVS_CNTCMP_OPT_B1)
		| P_Fld(0x1, B1_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_B1));
	vIO32WriteFldMulti(DDRPHY_CA_RXDVS0 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, CA_RXDVS0_R_DMRXDVS_CNTCMP_OPT_CA)
		| P_Fld(0x1, CA_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_CA));

#if 0 /* Wei - jen: no rx_dly_track in LP3 */
	vIO32WriteFldAlign(DDRPHY_R0_B1_RXDVS2, 0x1, R0_B1_RXDVS2_R_RK0_DVS_FDLY_MODE_B1);
	vIO32WriteFldAlign(DDRPHY_R1_B1_RXDVS2, 0x1, R1_B1_RXDVS2_R_RK1_DVS_FDLY_MODE_B1);
	vIO32WriteFldAlign(DDRPHY_R0_B0_RXDVS2 + SHIFT_TO_CHB_ADDR, 0x1, R0_B0_RXDVS2_R_RK0_DVS_FDLY_MODE_B0);
	vIO32WriteFldAlign(DDRPHY_R1_B0_RXDVS2 + SHIFT_TO_CHB_ADDR, 0x1, R1_B0_RXDVS2_R_RK1_DVS_FDLY_MODE_B0);

	vIO32WriteFldAlign(DDRPHY_R0_CA_RXDVS2 + SHIFT_TO_CHB_ADDR, 0x1, R0_CA_RXDVS2_R_RK0_DVS_FDLY_MODE_CA);
	vIO32WriteFldAlign(DDRPHY_R1_CA_RXDVS2 + SHIFT_TO_CHB_ADDR, 0x1, R1_CA_RXDVS2_R_RK1_DVS_FDLY_MODE_CA);
	vIO32WriteFldAlign(DDRPHY_R0_B1_RXDVS2 + SHIFT_TO_CHB_ADDR, 0x1, R0_B1_RXDVS2_R_RK0_DVS_FDLY_MODE_B1);
	vIO32WriteFldAlign(DDRPHY_R1_B1_RXDVS2 + SHIFT_TO_CHB_ADDR, 0x1, R1_B1_RXDVS2_R_RK1_DVS_FDLY_MODE_B1);
#endif
	vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5, 0x0, SHU1_B1_DQ5_RG_RX_ARDQS0_DVS_DLY_B1);
	vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ5 + SHIFT_TO_CHB_ADDR, P_Fld(0x0, SHU1_B0_DQ5_RG_RX_ARDQS0_DVS_DLY_B0)
		| P_Fld(0x0, SHU1_B0_DQ5_RG_RX_ARDQ_VREF_SEL_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD5 + SHIFT_TO_CHB_ADDR, P_Fld(0x0, SHU1_CA_CMD5_RG_RX_ARCLK_DVS_DLY)
		| P_Fld(0x0, SHU1_CA_CMD5_RG_RX_ARCMD_VREF_SEL));
	vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ5 + SHIFT_TO_CHB_ADDR, P_Fld(0x0, SHU1_B1_DQ5_RG_RX_ARDQS0_DVS_DLY_B1)
		| P_Fld(0x0, SHU1_B1_DQ5_RG_RX_ARDQ_VREF_SEL_B1));

#if LEGACY_RX_DLY
	LegacyRxDly_LP3(p);
#endif
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD2 + SHIFT_TO_CHB_ADDR, P_Fld(0x12, SHU1_R0_CA_CMD2_RG_RK0_RX_ARCA1_F_DLY)
		| P_Fld(0xa, SHU1_R0_CA_CMD2_RG_RK0_RX_ARCA1_R_DLY)
		| P_Fld(0x12, SHU1_R0_CA_CMD2_RG_RK0_RX_ARCA0_F_DLY)
		| P_Fld(0xa, SHU1_R0_CA_CMD2_RG_RK0_RX_ARCA0_R_DLY));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD3 + SHIFT_TO_CHB_ADDR, P_Fld(0x12, SHU1_R0_CA_CMD3_RG_RK0_RX_ARCA3_F_DLY)
		| P_Fld(0xa, SHU1_R0_CA_CMD3_RG_RK0_RX_ARCA3_R_DLY)
		| P_Fld(0x12, SHU1_R0_CA_CMD3_RG_RK0_RX_ARCA2_F_DLY)
		| P_Fld(0xa, SHU1_R0_CA_CMD3_RG_RK0_RX_ARCA2_R_DLY));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD4 + SHIFT_TO_CHB_ADDR, P_Fld(0x12, SHU1_R0_CA_CMD4_RG_RK0_RX_ARCA5_F_DLY)
		| P_Fld(0xa, SHU1_R0_CA_CMD4_RG_RK0_RX_ARCA5_R_DLY)
		| P_Fld(0x12, SHU1_R0_CA_CMD4_RG_RK0_RX_ARCA4_F_DLY)
		| P_Fld(0xa, SHU1_R0_CA_CMD4_RG_RK0_RX_ARCA4_R_DLY));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD5 + SHIFT_TO_CHB_ADDR, P_Fld(0x12, SHU1_R0_CA_CMD5_RG_RK0_RX_ARCKE1_F_DLY)
		| P_Fld(0xa, SHU1_R0_CA_CMD5_RG_RK0_RX_ARCKE1_R_DLY)
		| P_Fld(0x12, SHU1_R0_CA_CMD5_RG_RK0_RX_ARCKE0_F_DLY)
		| P_Fld(0xa, SHU1_R0_CA_CMD5_RG_RK0_RX_ARCKE0_R_DLY));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD6 + SHIFT_TO_CHB_ADDR, P_Fld(0x12, SHU1_R0_CA_CMD6_RG_RK0_RX_ARCS0_F_DLY)
		| P_Fld(0xa, SHU1_R0_CA_CMD6_RG_RK0_RX_ARCS0_R_DLY)
		| P_Fld(0x12, SHU1_R0_CA_CMD6_RG_RK0_RX_ARCKE2_F_DLY)
		| P_Fld(0xa, SHU1_R0_CA_CMD6_RG_RK0_RX_ARCKE2_R_DLY));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD7 + SHIFT_TO_CHB_ADDR, P_Fld(0x12, SHU1_R0_CA_CMD7_RG_RK0_RX_ARCS2_F_DLY)
		| P_Fld(0xa, SHU1_R0_CA_CMD7_RG_RK0_RX_ARCS2_R_DLY)
		| P_Fld(0x12, SHU1_R0_CA_CMD7_RG_RK0_RX_ARCS1_F_DLY)
		| P_Fld(0xa, SHU1_R0_CA_CMD7_RG_RK0_RX_ARCS1_R_DLY));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD8 + SHIFT_TO_CHB_ADDR, P_Fld(0x12, SHU1_R0_CA_CMD8_RG_RK0_RX_ARCLK_F_DLY)
		| P_Fld(0xa, SHU1_R0_CA_CMD8_RG_RK0_RX_ARCLK_R_DLY));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD2 + SHIFT_TO_CHB_ADDR, P_Fld(0x13, SHU1_R1_CA_CMD2_RG_RK1_RX_ARCA1_F_DLY)
		| P_Fld(0xb, SHU1_R1_CA_CMD2_RG_RK1_RX_ARCA1_R_DLY)
		| P_Fld(0x13, SHU1_R1_CA_CMD2_RG_RK1_RX_ARCA0_F_DLY)
		| P_Fld(0xb, SHU1_R1_CA_CMD2_RG_RK1_RX_ARCA0_R_DLY));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD3 + SHIFT_TO_CHB_ADDR, P_Fld(0x13, SHU1_R1_CA_CMD3_RG_RK1_RX_ARCA3_F_DLY)
		| P_Fld(0xb, SHU1_R1_CA_CMD3_RG_RK1_RX_ARCA3_R_DLY)
		| P_Fld(0x13, SHU1_R1_CA_CMD3_RG_RK1_RX_ARCA2_F_DLY)
		| P_Fld(0xb, SHU1_R1_CA_CMD3_RG_RK1_RX_ARCA2_R_DLY));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD4 + SHIFT_TO_CHB_ADDR, P_Fld(0x13, SHU1_R1_CA_CMD4_RG_RK1_RX_ARCA5_F_DLY)
		| P_Fld(0xb, SHU1_R1_CA_CMD4_RG_RK1_RX_ARCA5_R_DLY)
		| P_Fld(0x13, SHU1_R1_CA_CMD4_RG_RK1_RX_ARCA4_F_DLY)
		| P_Fld(0xb, SHU1_R1_CA_CMD4_RG_RK1_RX_ARCA4_R_DLY));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD5 + SHIFT_TO_CHB_ADDR, P_Fld(0x13, SHU1_R1_CA_CMD5_RG_RK1_RX_ARCKE1_F_DLY)
		| P_Fld(0xb, SHU1_R1_CA_CMD5_RG_RK1_RX_ARCKE1_R_DLY)
		| P_Fld(0x13, SHU1_R1_CA_CMD5_RG_RK1_RX_ARCKE0_F_DLY)
		| P_Fld(0xb, SHU1_R1_CA_CMD5_RG_RK1_RX_ARCKE0_R_DLY));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD6 + SHIFT_TO_CHB_ADDR, P_Fld(0x13, SHU1_R1_CA_CMD6_RG_RK1_RX_ARCS0_F_DLY)
		| P_Fld(0xb, SHU1_R1_CA_CMD6_RG_RK1_RX_ARCS0_R_DLY)
		| P_Fld(0x13, SHU1_R1_CA_CMD6_RG_RK1_RX_ARCKE2_F_DLY)
		| P_Fld(0xb, SHU1_R1_CA_CMD6_RG_RK1_RX_ARCKE2_R_DLY));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD7 + SHIFT_TO_CHB_ADDR, P_Fld(0x13, SHU1_R1_CA_CMD7_RG_RK1_RX_ARCS2_F_DLY)
		| P_Fld(0xb, SHU1_R1_CA_CMD7_RG_RK1_RX_ARCS2_R_DLY)
		| P_Fld(0x13, SHU1_R1_CA_CMD7_RG_RK1_RX_ARCS1_F_DLY)
		| P_Fld(0xb, SHU1_R1_CA_CMD7_RG_RK1_RX_ARCS1_R_DLY));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD8 + SHIFT_TO_CHB_ADDR, P_Fld(0x13, SHU1_R1_CA_CMD8_RG_RK1_RX_ARCLK_F_DLY)
		| P_Fld(0xb, SHU1_R1_CA_CMD8_RG_RK1_RX_ARCLK_R_DLY));

#if 0 /* Wei - jen: LP3 no rx_dly_track */
	vIO32WriteFldMulti(DDRPHY_R0_B1_RXDVS1, P_Fld(0x4, R0_B1_RXDVS1_R_RK0_B1_DVS_TH_LEAD)
		| P_Fld(0x4, R0_B1_RXDVS1_R_RK0_B1_DVS_TH_LAG));
	vIO32WriteFldMulti(DDRPHY_R1_B1_RXDVS1, P_Fld(0x4, R1_B1_RXDVS1_R_RK1_B1_DVS_TH_LEAD)
		| P_Fld(0x4, R1_B1_RXDVS1_R_RK1_B1_DVS_TH_LAG));
	vIO32WriteFldMulti(DDRPHY_R0_B0_RXDVS1 + SHIFT_TO_CHB_ADDR, P_Fld(0x4, R0_B0_RXDVS1_R_RK0_B0_DVS_TH_LEAD)
		| P_Fld(0x4, R0_B0_RXDVS1_R_RK0_B0_DVS_TH_LAG));
	vIO32WriteFldMulti(DDRPHY_R1_B0_RXDVS1 + SHIFT_TO_CHB_ADDR, P_Fld(0x4, R1_B0_RXDVS1_R_RK1_B0_DVS_TH_LEAD)
		| P_Fld(0x4, R1_B0_RXDVS1_R_RK1_B0_DVS_TH_LAG));
	vIO32WriteFldMulti(DDRPHY_R0_CA_RXDVS1 + SHIFT_TO_CHB_ADDR, P_Fld(0x4, R0_CA_RXDVS1_R_RK0_CA_DVS_TH_LEAD)
		| P_Fld(0x4, R0_CA_RXDVS1_R_RK0_CA_DVS_TH_LAG));
	vIO32WriteFldMulti(DDRPHY_R1_CA_RXDVS1 + SHIFT_TO_CHB_ADDR, P_Fld(0x4, R1_CA_RXDVS1_R_RK1_CA_DVS_TH_LEAD)
		| P_Fld(0x4, R1_CA_RXDVS1_R_RK1_CA_DVS_TH_LAG));
	vIO32WriteFldMulti(DDRPHY_R0_B1_RXDVS1 + SHIFT_TO_CHB_ADDR, P_Fld(0x4, R0_B1_RXDVS1_R_RK0_B1_DVS_TH_LEAD)
		| P_Fld(0x4, R0_B1_RXDVS1_R_RK0_B1_DVS_TH_LAG));
	vIO32WriteFldMulti(DDRPHY_R1_B1_RXDVS1 + SHIFT_TO_CHB_ADDR, P_Fld(0x4, R1_B1_RXDVS1_R_RK1_B1_DVS_TH_LEAD)
		| P_Fld(0x4, R1_B1_RXDVS1_R_RK1_B1_DVS_TH_LAG));
#endif

	vIO32WriteFldMulti_All(DDRPHY_R0_B1_RXDVS2, P_Fld(0x2, R0_B1_RXDVS2_R_RK0_DVS_MODE_B1)
		| P_Fld(0x1, R0_B1_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B1)
		| P_Fld(0x1, R0_B1_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B1));
	vIO32WriteFldMulti_All(DDRPHY_R1_B1_RXDVS2, P_Fld(0x2, R1_B1_RXDVS2_R_RK1_DVS_MODE_B1)
		| P_Fld(0x1, R1_B1_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_B1)
		| P_Fld(0x1, R1_B1_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_B1));
	vIO32WriteFldMulti(DDRPHY_R0_B0_RXDVS2 + SHIFT_TO_CHB_ADDR, P_Fld(0x2, R0_B0_RXDVS2_R_RK0_DVS_MODE_B0)
		| P_Fld(0x1, R0_B0_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B0)
		| P_Fld(0x1, R0_B0_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B0));
	vIO32WriteFldMulti(DDRPHY_R1_B0_RXDVS2 + SHIFT_TO_CHB_ADDR, P_Fld(0x2, R1_B0_RXDVS2_R_RK1_DVS_MODE_B0)
		| P_Fld(0x1, R1_B0_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_B0)
		| P_Fld(0x1, R1_B0_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_B0));
	vIO32WriteFldMulti(DDRPHY_R0_CA_RXDVS2 + SHIFT_TO_CHB_ADDR, P_Fld(0x2, R0_CA_RXDVS2_R_RK0_DVS_MODE_CA)
		| P_Fld(0x1, R0_CA_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_CA)
		| P_Fld(0x1, R0_CA_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_CA));
	vIO32WriteFldMulti(DDRPHY_R1_CA_RXDVS2 + SHIFT_TO_CHB_ADDR, P_Fld(0x2, R1_CA_RXDVS2_R_RK1_DVS_MODE_CA)
		| P_Fld(0x1, R1_CA_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_CA)
		| P_Fld(0x1, R1_CA_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_CA));
#if 0 /* Use _All instead() */
	vIO32WriteFldMulti(DDRPHY_R0_B1_RXDVS2 + SHIFT_TO_CHB_ADDR, P_Fld(0x2, R0_B1_RXDVS2_R_RK0_DVS_MODE_B1)
		| P_Fld(0x1, R0_B1_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B1)
		| P_Fld(0x1, R0_B1_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B1));
	vIO32WriteFldMulti(DDRPHY_R1_B1_RXDVS2 + SHIFT_TO_CHB_ADDR, P_Fld(0x2, R1_B1_RXDVS2_R_RK1_DVS_MODE_B1)
		| P_Fld(0x1, R1_B1_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_B1)
		| P_Fld(0x1, R1_B1_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_B1));
#endif

	vIO32WriteFldAlign_All(DDRPHY_B1_RXDVS0, 0x0, B1_RXDVS0_R_RX_DLY_TRACK_CG_EN_B1);
	vIO32WriteFldAlign(DDRPHY_B0_RXDVS0 + SHIFT_TO_CHB_ADDR, 0x0, B0_RXDVS0_R_RX_DLY_TRACK_CG_EN_B0);
	vIO32WriteFldAlign(DDRPHY_CA_RXDVS0 + SHIFT_TO_CHB_ADDR, 0x0, CA_RXDVS0_R_RX_DLY_TRACK_CG_EN_CA);
	/* vIO32WriteFldAlign(DDRPHY_B1_RXDVS0+SHIFT_TO_CHB_ADDR, 0x0, B1_RXDVS0_R_RX_DLY_TRACK_CG_EN_B1); //Use _All() instead */
	vIO32WriteFldAlign(DDRPHY_B1_DQ9, 0x1, B1_DQ9_RG_RX_ARDQ_STBEN_RESETB_B1);
	vIO32WriteFldAlign(DDRPHY_B0_DQ9 + SHIFT_TO_CHB_ADDR, 0x1, B0_DQ9_RG_RX_ARDQ_STBEN_RESETB_B0);
	vIO32WriteFldAlign(DDRPHY_CA_CMD10 + SHIFT_TO_CHB_ADDR, 0x1, CA_CMD10_RG_RX_ARCMD_STBEN_RESETB);
	vIO32WriteFldAlign(DDRPHY_B1_DQ9 + SHIFT_TO_CHB_ADDR, 0x1, B1_DQ9_RG_RX_ARDQ_STBEN_RESETB_B1);
	vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL1, 0, MISC_CG_CTRL1_R_DVS_DIV4_CG_CTRL); /* Wei - jen: no rx_dly_track in LP3 */
	/* vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL1+SHIFT_TO_CHB_ADDR, 0, MISC_CG_CTRL1_R_DVS_DIV4_CG_CTRL); //Wei-jen: no rx_dly_track in LP3 (Use _All() instead) */

	vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD9, P_Fld(0xc, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CMD)
		| P_Fld(0, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CLK)
		| P_Fld(0, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CS));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD9, P_Fld(0xc, SHU1_R1_CA_CMD9_RG_RK1_ARPI_CMD)
		| P_Fld(0, SHU1_R1_CA_CMD9_RG_RK1_ARPI_CLK)
		| P_Fld(0, SHU1_R1_CA_CMD9_RG_RK1_ARPI_CS));

#if LEGACY_DELAY_CELL
	LegacyDlyCellInitLP3(p);
#endif
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD1 + SHIFT_TO_CHB_ADDR, P_Fld(0x6, SHU1_R1_CA_CMD1_RK1_TX_ARCKE2_DLY)
		| P_Fld(0x6, SHU1_R1_CA_CMD1_RK1_TX_ARCKE1_DLY)
		| P_Fld(0x6, SHU1_R1_CA_CMD1_RK1_TX_ARCKE0_DLY));

	vIO32WriteFldAlign(DDRPHY_SHU1_R0_B0_DQ7, 0xc, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0); /* Macro 1 CA delay */
	vIO32WriteFldAlign(DDRPHY_SHU1_R1_B0_DQ7, 0xc, SHU1_R1_B0_DQ7_RK1_ARPI_DQ_B0); /* Macro 1 CA delay */

	vIO32WriteFldMulti_All(DDRPHY_SHU1_R0_B1_DQ7, P_Fld(0xf, SHU1_R0_B1_DQ7_RK0_ARPI_DQM_B1)
		| P_Fld(0xf, SHU1_R0_B1_DQ7_RK0_ARPI_DQ_B1));
	vIO32WriteFldMulti_All(DDRPHY_SHU1_R1_B1_DQ7, P_Fld(0xf, SHU1_R1_B1_DQ7_RK1_ARPI_DQM_B1)
		| P_Fld(0xf, SHU1_R1_B1_DQ7_RK1_ARPI_DQ_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD9 + SHIFT_TO_CHB_ADDR, P_Fld(0xf, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CMD)
		| P_Fld(0xf, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CS));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_CA_CMD9 + SHIFT_TO_CHB_ADDR, P_Fld(0xf, SHU1_R1_CA_CMD9_RG_RK1_ARPI_CMD)
		| P_Fld(0xf, SHU1_R1_CA_CMD9_RG_RK1_ARPI_CS));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ7 + SHIFT_TO_CHB_ADDR, P_Fld(0xf, SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0)
		| P_Fld(0xf, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ7 + SHIFT_TO_CHB_ADDR, P_Fld(0xf, SHU1_R1_B0_DQ7_RK1_ARPI_DQM_B0)
		| P_Fld(0xf, SHU1_R1_B0_DQ7_RK1_ARPI_DQ_B0));
#if 0 /* Use _All instead */
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ7 + SHIFT_TO_CHB_ADDR, P_Fld(0xf, SHU1_R0_B1_DQ7_RK0_ARPI_DQM_B1)
		| P_Fld(0xf, SHU1_R0_B1_DQ7_RK0_ARPI_DQ_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ7 + SHIFT_TO_CHB_ADDR, P_Fld(0xf, SHU1_R1_B1_DQ7_RK1_ARPI_DQM_B1)
		| P_Fld(0xf, SHU1_R1_B1_DQ7_RK1_ARPI_DQ_B1));
#endif
	vIO32WriteFldMulti(DDRPHY_B1_DQ4, P_Fld(0x10, B1_DQ4_RG_RX_ARDQS_EYE_R_DLY_B1)
		| P_Fld(0x10, B1_DQ4_RG_RX_ARDQS_EYE_F_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_B1_DQ5, P_Fld(0x1, B1_DQ5_RG_RX_ARDQ_EYE_EN_B1)
		| P_Fld(0x1, B1_DQ5_RG_RX_ARDQ_EYE_SEL_B1)
		| P_Fld(0x1, B1_DQ5_RG_RX_ARDQ_VREF_EN_B1)
		| P_Fld(0x10, B1_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B1)
		| P_Fld(0x10, B1_DQ5_B1_DQ5_RFU));
	vIO32WriteFldMulti(DDRPHY_B1_DQ6, P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_EYE_DLY_DQS_BYPASS_B1)
		| P_Fld(0x0, B1_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B1)
		| P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1)
		| P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B1));
	vIO32WriteFldMulti(DDRPHY_B1_DQ5, P_Fld(0x1, B1_DQ5_RG_RX_ARDQ_EYE_STBEN_RESETB_B1)
		| P_Fld(0x0, B1_DQ5_B1_DQ5_RFU));
	vIO32WriteFldMulti(DDRPHY_CA_CMD3, P_Fld(0x1, CA_CMD3_RG_RX_ARCMD_IN_BUFF_EN)
		| P_Fld(0x1, CA_CMD3_RG_ARCMD_RESETB)
		| P_Fld(0x1, CA_CMD3_RG_TX_ARCMD_EN));
	vIO32WriteFldMulti(DDRPHY_CA_CMD6, P_Fld(0x1, CA_CMD6_RG_TX_ARCMD_DDR3_SEL)
		| P_Fld(0x0, CA_CMD6_RG_TX_ARCMD_DDR4_SEL)
		| P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_DDR4_SEL)
		| P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_BIAS_VREF_SEL)
		| P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_RES_BIAS_EN)
		| P_Fld(0x1, CA_CMD6_RG_TX_ARCMD_SER_MODE));
	/*
	 * ARCMD_DRVP, DRVN , ARCLK_DRVP, DRVN not used anymore
	 * vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD1, P_Fld(0x1, SHU1_CA_CMD1_RG_TX_ARCMD_DRVN)
	 * | P_Fld(0x1, SHU1_CA_CMD1_RG_TX_ARCMD_DRVP));
	 * vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD2, P_Fld(0x1, SHU1_CA_CMD2_RG_TX_ARCLK_DRVN)
	 * | P_Fld(0x1, SHU1_CA_CMD2_RG_TX_ARCLK_DRVP));
	 */
	/*
	 * vIO32WriteFldMulti(DDRPHY_SHU2_CA_CMD1, P_Fld(0x1, SHU2_CA_CMD1_RG_TX_ARCMD_DRVN)
	 * | P_Fld(0x1, SHU2_CA_CMD1_RG_TX_ARCMD_DRVP));
	 * vIO32WriteFldMulti(DDRPHY_SHU2_CA_CMD2, P_Fld(0x1, SHU2_CA_CMD2_RG_TX_ARCLK_DRVN)
	 * | P_Fld(0x1, SHU2_CA_CMD2_RG_TX_ARCLK_DRVP));
	 */
	vIO32WriteFldMulti(DDRPHY_B0_DQ3, P_Fld(0x1, B0_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B0)
		| P_Fld(0x1, B0_DQ3_RG_ARDQ_RESETB_B0)
		| P_Fld(0x1, B0_DQ3_RG_TX_ARDQ_EN_B0));
	vIO32WriteFldMulti(DDRPHY_B0_DQ6, P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_DDR3_SEL_B0)
		| P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B0)
		| P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_RES_BIAS_EN_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ1, P_Fld(0x1, SHU1_B0_DQ1_RG_TX_ARDQ_DRVN_B0)
		| P_Fld(0x1, SHU1_B0_DQ1_RG_TX_ARDQ_DRVP_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ2, P_Fld(0x1, SHU1_B0_DQ2_RG_TX_ARDQS0_DRVN_B0)
		| P_Fld(0x1, SHU1_B0_DQ2_RG_TX_ARDQS0_DRVP_B0));
	/*
	 * vIO32WriteFldMulti(DDRPHY_SHU2_B0_DQ1, P_Fld(0x1, SHU2_B0_DQ1_RG_TX_ARDQ_DRVN_B0)
	 * | P_Fld(0x1, SHU2_B0_DQ1_RG_TX_ARDQ_DRVP_B0));
	 * vIO32WriteFldMulti(DDRPHY_SHU2_B0_DQ2, P_Fld(0x1, SHU2_B0_DQ2_RG_TX_ARDQS0_DRVN_B0)
	 * | P_Fld(0x1, SHU2_B0_DQ2_RG_TX_ARDQS0_DRVP_B0));
	 */
	vIO32WriteFldMulti(DDRPHY_CA_CMD4 + SHIFT_TO_CHB_ADDR, P_Fld(0x10, CA_CMD4_RG_RX_ARCLK_EYE_R_DLY)
		| P_Fld(0x10, CA_CMD4_RG_RX_ARCLK_EYE_F_DLY));
	vIO32WriteFldMulti(DDRPHY_CA_CMD5 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, CA_CMD5_RG_RX_ARCMD_EYE_EN)
		| P_Fld(0x1, CA_CMD5_RG_RX_ARCMD_EYE_SEL)
		| P_Fld(0x1, CA_CMD5_RG_RX_ARCMD_VREF_EN)
		| P_Fld(0x10, CA_CMD5_RG_RX_ARCMD_EYE_VREF_SEL)
		| P_Fld(0x10, CA_CMD5_CA_CMD5_RFU));
	vIO32WriteFldMulti(DDRPHY_CA_CMD6 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, CA_CMD6_RG_RX_ARCMD_EYE_DLY_DQS_BYPASS)
		| P_Fld(0x1, CA_CMD6_RG_TX_ARCMD_DDR3_SEL)
		| P_Fld(0x1, CA_CMD6_RG_RX_ARCMD_DDR3_SEL)
		| P_Fld(0x0, CA_CMD6_RG_TX_ARCMD_DDR4_SEL)
		| P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_DDR4_SEL)
		| P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_BIAS_VREF_SEL)
		| P_Fld(0x1, CA_CMD6_RG_RX_ARCMD_BIAS_EN)
		| P_Fld(0x1, CA_CMD6_RG_RX_ARCMD_OP_BIAS_SW_EN)
		| P_Fld(0x1, CA_CMD6_RG_TX_ARCMD_SER_MODE));
	vIO32WriteFldMulti(DDRPHY_CA_CMD5 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, CA_CMD5_RG_RX_ARCMD_EYE_STBEN_RESETB)
		| P_Fld(0x0, CA_CMD5_CA_CMD5_RFU));
	vIO32WriteFldMulti(DDRPHY_B0_DQ4 + SHIFT_TO_CHB_ADDR, P_Fld(0x10, B0_DQ4_RG_RX_ARDQS_EYE_R_DLY_B0)
		| P_Fld(0x10, B0_DQ4_RG_RX_ARDQS_EYE_F_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_B0_DQ5 + SHIFT_TO_CHB_ADDR, P_Fld(0x0, B0_DQ5_RG_RX_ARDQ_EYE_EN_B0)
		| P_Fld(0x1, B0_DQ5_RG_RX_ARDQ_EYE_SEL_B0)
		| P_Fld(0x1, B0_DQ5_RG_RX_ARDQ_VREF_EN_B0)
		| P_Fld(0x10, B0_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B0)
		| P_Fld(0x10, B0_DQ5_B0_DQ5_RFU));
	vIO32WriteFldMulti(DDRPHY_B0_DQ6 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, B0_DQ6_RG_RX_ARDQ_EYE_DLY_DQS_BYPASS_B0)
		| P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B0)
		| P_Fld(0x1, B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0)
		| P_Fld(0x1, B0_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B0));
	vIO32WriteFldMulti(DDRPHY_B0_DQ5 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, B0_DQ5_RG_RX_ARDQ_EYE_STBEN_RESETB_B0)
		| P_Fld(0x0, B0_DQ5_B0_DQ5_RFU));
	vIO32WriteFldMulti(DDRPHY_B1_DQ4 + SHIFT_TO_CHB_ADDR, P_Fld(0x10, B1_DQ4_RG_RX_ARDQS_EYE_R_DLY_B1)
		| P_Fld(0x10, B1_DQ4_RG_RX_ARDQS_EYE_F_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_B1_DQ5 + SHIFT_TO_CHB_ADDR, P_Fld(0x0, B1_DQ5_RG_RX_ARDQ_EYE_EN_B1)
		| P_Fld(0x1, B1_DQ5_RG_RX_ARDQ_EYE_SEL_B1)
		| P_Fld(0x1, B1_DQ5_RG_RX_ARDQ_VREF_EN_B1)
		| P_Fld(0x10, B1_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B1)
		| P_Fld(0x10, B1_DQ5_B1_DQ5_RFU));
	vIO32WriteFldMulti(DDRPHY_B1_DQ6 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_EYE_DLY_DQS_BYPASS_B1)
		| P_Fld(0x0, B1_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B1)
		| P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1)
		| P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B1));
	vIO32WriteFldMulti(DDRPHY_B1_DQ5 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, B1_DQ5_RG_RX_ARDQ_EYE_STBEN_RESETB_B1)
		| P_Fld(0x0, B1_DQ5_B1_DQ5_RFU));


	vIO32WriteFldMulti(DDRPHY_PLL4, P_Fld(0x0, PLL4_RG_RPHYPLL_AD_MCK8X_EN)
		| P_Fld(0x1, PLL4_PLL4_RFU)
		| P_Fld(0x1, PLL4_RG_RPHYPLL_MCK8X_SEL));


	vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI1, P_Fld(0x1, CA_DLL_ARPI1_RG_ARPISM_MCK_SEL_CA)
		| P_Fld(0x1, CA_DLL_ARPI1_RG_ARPI_CS_JUMP_EN)
		| P_Fld(0x1, CA_DLL_ARPI1_RG_ARPI_CLK_JUMP_EN));
	vIO32WriteFldMulti(DDRPHY_B0_DLL_ARPI1, P_Fld(0x1, B0_DLL_ARPI1_RG_ARPISM_MCK_SEL_B0)
		| P_Fld(0x1, B0_DLL_ARPI1_RG_ARPI_DQ_JUMP_EN_B0));
	vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI1, 0x1, B1_DLL_ARPI1_RG_ARPI_DQ_JUMP_EN_B1);


	vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI1 + SHIFT_TO_CHB_ADDR, 0x1, CA_DLL_ARPI1_RG_ARPI_CMD_JUMP_EN);
	vIO32WriteFldAlign(DDRPHY_B0_DLL_ARPI1 + SHIFT_TO_CHB_ADDR, 0x1, B0_DLL_ARPI1_RG_ARPI_DQ_JUMP_EN_B0);
#if 0 /* Use _All instead() */
	vIO32WriteFldAlign(DDRPHY_B1_DLL_ARPI1 + SHIFT_TO_CHB_ADDR, 0x1, B1_DLL_ARPI1_RG_ARPI_DQ_JUMP_EN_B1);
	vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI2 + SHIFT_TO_CHB_ADDR, P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_FB_CA)
		| P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA));
	vIO32WriteFldMulti(DDRPHY_B0_DLL_ARPI2 + SHIFT_TO_CHB_ADDR, P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_FB_B0)
		| P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0));
	vIO32WriteFldMulti(DDRPHY_B1_DLL_ARPI2 + SHIFT_TO_CHB_ADDR, P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_FB_B1)
		| P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1));
#endif

	vIO32WriteFldMulti_All(DDRPHY_CA_CMD3, P_Fld(0x1, CA_CMD3_RG_RX_ARCMD_STBENCMP_EN)
		| P_Fld(0x1, CA_CMD3_RG_ARCMD_RESETB)
		| P_Fld(0x1, CA_CMD3_RG_TX_ARCMD_EN)
		| P_Fld(0x0, CA_CMD3_RG_RX_ARCMD_SMT_EN));
	vIO32WriteFldMulti_All(DDRPHY_B0_DQ3, P_Fld(0x1, B0_DQ3_RG_RX_ARDQ_STBENCMP_EN_B0)
		| P_Fld(0x1, B0_DQ3_RG_ARDQ_RESETB_B0)
		| P_Fld(0x1, B0_DQ3_RG_TX_ARDQ_EN_B0)
		| P_Fld(0x0, B0_DQ3_RG_RX_ARDQ_SMT_EN_B0));

	vIO32WriteFldMulti_All(DDRPHY_B1_DQ3, P_Fld(0x1, B1_DQ3_RG_RX_ARDQ_STBENCMP_EN_B1)
		| P_Fld(0x1, B1_DQ3_RG_ARDQ_RESETB_B1)
		| P_Fld(0x1, B1_DQ3_RG_TX_ARDQ_EN_B1)
		| P_Fld(0x0, B1_DQ3_RG_RX_ARDQ_SMT_EN_B1));
#if 0
	vIO32WriteFldMulti(DDRPHY_CA_CMD3 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, CA_CMD3_RG_RX_ARCMD_STBENCMP_EN)
		| P_Fld(0x1, CA_CMD3_RG_ARCMD_RESETB)
		| P_Fld(0x1, CA_CMD3_RG_TX_ARCMD_EN)
		| P_Fld(0x1, CA_CMD3_RG_RX_ARCMD_SMT_EN));
	vIO32WriteFldMulti(DDRPHY_B0_DQ3 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, B0_DQ3_RG_RX_ARDQ_STBENCMP_EN_B0)
		| P_Fld(0x1, B0_DQ3_RG_ARDQ_RESETB_B0)
		| P_Fld(0x1, B0_DQ3_RG_TX_ARDQ_EN_B0)
		| P_Fld(0x1, B0_DQ3_RG_RX_ARDQ_SMT_EN_B0));
	vIO32WriteFldMulti(DDRPHY_B1_DQ3 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, B1_DQ3_RG_RX_ARDQ_STBENCMP_EN_B1)
		| P_Fld(0x1, B1_DQ3_RG_ARDQ_RESETB_B1)
		| P_Fld(0x1, B1_DQ3_RG_TX_ARDQ_EN_B1)
		| P_Fld(0x1, B1_DQ3_RG_RX_ARDQ_SMT_EN_B1));
#endif
	vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI5, 0x0, CA_DLL_ARPI5_RG_ARDLL_DIV_MCTL_CA);
	vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI5, 0x0, B0_DLL_ARPI5_RG_ARDLL_DIV_MCTL_B0);
	vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI5, 0x0, B1_DLL_ARPI5_RG_ARDLL_DIV_MCTL_B1);
#if 0 /* Use _All() instead */
	vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI5 + SHIFT_TO_CHB_ADDR, 0x0, CA_DLL_ARPI5_RG_ARDLL_DIV_MCTL_CA);
	vIO32WriteFldAlign(DDRPHY_B0_DLL_ARPI5 + SHIFT_TO_CHB_ADDR, 0x0, B0_DLL_ARPI5_RG_ARDLL_DIV_MCTL_B0);
	vIO32WriteFldAlign(DDRPHY_B1_DLL_ARPI5 + SHIFT_TO_CHB_ADDR, 0x0, B1_DLL_ARPI5_RG_ARDLL_DIV_MCTL_B1);
#endif

	vIO32WriteFldMulti(DDRPHY_SHU1_CA_DLL0, P_Fld(0x9, SHU1_CA_DLL0_RG_ARDLL_IDLECNT_CA)
		| P_Fld(0x8, SHU1_CA_DLL0_RG_ARDLL_P_GAIN_CA)
		| P_Fld(0x1, SHU1_CA_DLL0_RG_ARDLL_PHJUMP_EN_CA)
		| P_Fld(0x1, SHU1_CA_DLL0_RG_ARDLL_PHDIV_CA));
	vIO32WriteFldMulti_All(DDRPHY_SHU1_B0_DLL0, P_Fld(0x1, SHU1_B0_DLL0_RG_ARDLL_PHDET_IN_SWAP_B0)
		| P_Fld(0x7, SHU1_B0_DLL0_RG_ARDLL_GAIN_B0)
		| P_Fld(0x7, SHU1_B0_DLL0_RG_ARDLL_IDLECNT_B0)
		| P_Fld(0x8, SHU1_B0_DLL0_RG_ARDLL_P_GAIN_B0)
		| P_Fld(0x1, SHU1_B0_DLL0_RG_ARDLL_PHJUMP_EN_B0)
		| P_Fld(0x1, SHU1_B0_DLL0_RG_ARDLL_PHDIV_B0));
	vIO32WriteFldMulti_All(DDRPHY_SHU1_B1_DLL0, P_Fld(0x1, SHU1_B1_DLL0_RG_ARDLL_PHDET_IN_SWAP_B1)
		| P_Fld(0x7, SHU1_B1_DLL0_RG_ARDLL_GAIN_B1)
		| P_Fld(0x7, SHU1_B1_DLL0_RG_ARDLL_IDLECNT_B1)
		| P_Fld(0x8, SHU1_B1_DLL0_RG_ARDLL_P_GAIN_B1)
		| P_Fld(0x1, SHU1_B1_DLL0_RG_ARDLL_PHJUMP_EN_B1)
		| P_Fld(0x1, SHU1_B1_DLL0_RG_ARDLL_PHDIV_B1));

	vIO32WriteFldMulti(DDRPHY_SHU1_CA_DLL0 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, SHU1_CA_DLL0_RG_ARDLL_PHDET_OUT_SEL_CA)
		| P_Fld(0x1, SHU1_CA_DLL0_RG_ARDLL_PHDET_IN_SWAP_CA)
		| P_Fld(0x7, SHU1_CA_DLL0_RG_ARDLL_GAIN_CA)
		| P_Fld(0x7, SHU1_CA_DLL0_RG_ARDLL_IDLECNT_CA)
		| P_Fld(0x8, SHU1_CA_DLL0_RG_ARDLL_P_GAIN_CA)
		| P_Fld(0x1, SHU1_CA_DLL0_RG_ARDLL_PHJUMP_EN_CA)
		| P_Fld(0x1, SHU1_CA_DLL0_RG_ARDLL_PHDIV_CA));
#if 0 /* Use _All() instead */
	vIO32WriteFldMulti(DDRPHY_SHU1_B0_DLL0 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, SHU1_B0_DLL0_RG_ARDLL_PHDET_IN_SWAP_B0)
		| P_Fld(0x7, SHU1_B0_DLL0_RG_ARDLL_GAIN_B0)
		| P_Fld(0x7, SHU1_B0_DLL0_RG_ARDLL_IDLECNT_B0)
		| P_Fld(0x8, SHU1_B0_DLL0_RG_ARDLL_P_GAIN_B0)
		| P_Fld(0x1, SHU1_B0_DLL0_RG_ARDLL_PHJUMP_EN_B0)
		| P_Fld(0x1, SHU1_B0_DLL0_RG_ARDLL_PHDIV_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_B1_DLL0 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, SHU1_B1_DLL0_RG_ARDLL_PHDET_IN_SWAP_B1)
		| P_Fld(0x7, SHU1_B1_DLL0_RG_ARDLL_GAIN_B1)
		| P_Fld(0x7, SHU1_B1_DLL0_RG_ARDLL_IDLECNT_B1)
		| P_Fld(0x8, SHU1_B1_DLL0_RG_ARDLL_P_GAIN_B1)
		| P_Fld(0x1, SHU1_B1_DLL0_RG_ARDLL_PHJUMP_EN_B1)
		| P_Fld(0x1, SHU1_B1_DLL0_RG_ARDLL_PHDIV_B1));
#endif

	vIO32WriteFldMulti_All(DDRPHY_CA_CMD8, P_Fld(0x1, CA_CMD8_RG_TX_RRESETB_DDR3_SEL)
		| P_Fld(0x1, CA_CMD8_RG_RRESETB_DRVN)
		| P_Fld(0x1, CA_CMD8_RG_RRESETB_DRVP));
#if 0 /* Use _All() instead */
	vIO32WriteFldMulti(DDRPHY_CA_CMD8 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, CA_CMD8_RG_TX_RRESETB_DDR3_SEL)
		| P_Fld(0x1, CA_CMD8_RG_RRESETB_DRVN)
		| P_Fld(0x1, CA_CMD8_RG_RRESETB_DRVP));
#endif
	mcDELAY_US(1);

	vIO32WriteFldAlign_All(DDRPHY_SHU1_PLL0, 0x3, SHU1_PLL0_RG_RPHYPLL_TOP_REV);
	/*
	 * vIO32WriteFldAlign(DDRPHY_SHU2_PLL0, 0x3, SHU2_PLL0_RG_RPHYPLL_TOP_REV);
	 * vIO32WriteFldAlign(DDRPHY_SHU3_PLL0, 0x3, SHU3_PLL0_RG_RPHYPLL_TOP_REV);
	 * vIO32WriteFldAlign(DDRPHY_SHU4_PLL0, 0x3, SHU4_PLL0_RG_RPHYPLL_TOP_REV);
	 * vIO32WriteFldAlign(DDRPHY_SHU1_PLL0+SHIFT_TO_CHB_ADDR, 0x3, SHU1_PLL0_RG_RPHYPLL_TOP_REV); //Use _All() instead
	 * vIO32WriteFldAlign(DDRPHY_SHU2_PLL0+SHIFT_TO_CHB_ADDR, 0x3, SHU2_PLL0_RG_RPHYPLL_TOP_REV);
	 * vIO32WriteFldAlign(DDRPHY_SHU3_PLL0+SHIFT_TO_CHB_ADDR, 0x3, SHU3_PLL0_RG_RPHYPLL_TOP_REV);
	 * vIO32WriteFldAlign(DDRPHY_SHU4_PLL0+SHIFT_TO_CHB_ADDR, 0x3, SHU4_PLL0_RG_RPHYPLL_TOP_REV);
	 */
	vIO32WriteFldMulti_All(DDRPHY_SHU1_CA_CMD0, P_Fld(0x1, SHU1_CA_CMD0_RG_TX_ARCMD_PRE_EN)
		| P_Fld(0x7, SHU1_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
		| P_Fld(0x1, SHU1_CA_CMD0_RG_TX_ARCLK_PRE_EN));
	/*
	 * vIO32WriteFldMulti(DDRPHY_SHU2_CA_CMD0, P_Fld(0x1, SHU2_CA_CMD0_RG_TX_ARCMD_PRE_EN)
	 * | P_Fld(0x7, SHU2_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
	 * | P_Fld(0x1, SHU2_CA_CMD0_RG_TX_ARCLK_PRE_EN));
	 * vIO32WriteFldMulti(DDRPHY_SHU3_CA_CMD0, P_Fld(0x1, SHU3_CA_CMD0_RG_TX_ARCMD_PRE_EN)
	 * | P_Fld(0x7, SHU3_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
	 * | P_Fld(0x1, SHU3_CA_CMD0_RG_TX_ARCLK_PRE_EN));
	 * vIO32WriteFldMulti(DDRPHY_SHU4_CA_CMD0, P_Fld(0x1, SHU4_CA_CMD0_RG_TX_ARCMD_PRE_EN)
	 * | P_Fld(0x7, SHU4_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
	 * | P_Fld(0x1, SHU4_CA_CMD0_RG_TX_ARCLK_PRE_EN));
	 */
#if 0 /* Use _ALl() instead */
	vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD0 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, SHU1_CA_CMD0_RG_TX_ARCMD_PRE_EN)
		| P_Fld(0x7, SHU1_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
		| P_Fld(0x1, SHU1_CA_CMD0_RG_TX_ARCLK_PRE_EN));
#endif
	/*
	 * vIO32WriteFldMulti(DDRPHY_SHU2_CA_CMD0+SHIFT_TO_CHB_ADDR, P_Fld(0x1, SHU2_CA_CMD0_RG_TX_ARCMD_PRE_EN)
	 * | P_Fld(0x7, SHU2_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
	 * | P_Fld(0x1, SHU2_CA_CMD0_RG_TX_ARCLK_PRE_EN));
	 * vIO32WriteFldMulti(DDRPHY_SHU3_CA_CMD0+SHIFT_TO_CHB_ADDR, P_Fld(0x1, SHU3_CA_CMD0_RG_TX_ARCMD_PRE_EN)
	 * | P_Fld(0x7, SHU3_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
	 * | P_Fld(0x1, SHU3_CA_CMD0_RG_TX_ARCLK_PRE_EN));
	 * vIO32WriteFldMulti(DDRPHY_SHU4_CA_CMD0+SHIFT_TO_CHB_ADDR, P_Fld(0x1, SHU4_CA_CMD0_RG_TX_ARCMD_PRE_EN)
	 * | P_Fld(0x7, SHU4_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
	 * | P_Fld(0x1, SHU4_CA_CMD0_RG_TX_ARCLK_PRE_EN));
	 */
	vIO32WriteFldMulti_All(DDRPHY_MISC_SHU_OPT, P_Fld(0x1, MISC_SHU_OPT_R_CA_SHU_PHDET_SPM_EN)
		| P_Fld(0x1, MISC_SHU_OPT_R_CA_SHU_PHY_GATING_RESETB_SPM_EN)
		| P_Fld(0x2, MISC_SHU_OPT_R_DQB1_SHU_PHDET_SPM_EN)
		| P_Fld(0x1, MISC_SHU_OPT_R_DQB1_SHU_PHY_GATING_RESETB_SPM_EN)
		| P_Fld(0x2, MISC_SHU_OPT_R_DQB0_SHU_PHDET_SPM_EN)
		| P_Fld(0x1, MISC_SHU_OPT_R_DQB0_SHU_PHY_GATING_RESETB_SPM_EN));
#if 0 /* Use _All() instead */
	vIO32WriteFldMulti(DDRPHY_MISC_SHU_OPT + SHIFT_TO_CHB_ADDR, P_Fld(0x2, MISC_SHU_OPT_R_CA_SHU_PHDET_SPM_EN)
		| P_Fld(0x1, MISC_SHU_OPT_R_CA_SHU_PHY_GATING_RESETB_SPM_EN)
		| P_Fld(0x2, MISC_SHU_OPT_R_DQB1_SHU_PHDET_SPM_EN)
		| P_Fld(0x1, MISC_SHU_OPT_R_DQB1_SHU_PHY_GATING_RESETB_SPM_EN)
		| P_Fld(0x2, MISC_SHU_OPT_R_DQB0_SHU_PHDET_SPM_EN)
		| P_Fld(0x1, MISC_SHU_OPT_R_DQB0_SHU_PHY_GATING_RESETB_SPM_EN));
#endif

	mcDELAY_US(1);

	vIO32WriteFldAlign_All(DDRPHY_PLL3, 0x0, PLL3_RG_RPHYPLL_TSTOP_EN);
	/* vIO32WriteFldAlign(DDRPHY_PLL3+SHIFT_TO_CHB_ADDR, 0x0, PLL3_RG_RPHYPLL_TSTOP_EN); //Use _All() instead */

	vIO32WriteFldMulti_All(DDRPHY_SHU1_PLL9, P_Fld(0x0, SHU1_PLL9_RG_RPHYPLL_MONCK_EN)
		| P_Fld(0x0, SHU1_PLL9_RG_RPHYPLL_MONVC_EN)
		| P_Fld(0x0, SHU1_PLL9_RG_RPHYPLL_LVROD_EN)
		| P_Fld(0x0, SHU1_PLL9_RG_RPHYPLL_RST_DLY));
	vIO32WriteFldMulti_All(DDRPHY_SHU1_PLL11, P_Fld(0x0, SHU1_PLL11_RG_RCLRPLL_MONCK_EN)
		| P_Fld(0x0, SHU1_PLL11_RG_RCLRPLL_MONVC_EN)
		| P_Fld(0x0, SHU1_PLL11_RG_RCLRPLL_LVROD_EN)
		| P_Fld(0x0, SHU1_PLL11_RG_RCLRPLL_RST_DLY));
#if 0 /* Use _All() instead */
	vIO32WriteFldMulti(DDRPHY_SHU1_PLL9 + SHIFT_TO_CHB_ADDR, P_Fld(0x0, SHU1_PLL9_RG_RPHYPLL_MONCK_EN)
		| P_Fld(0x0, SHU1_PLL9_RG_RPHYPLL_MONVC_EN)
		| P_Fld(0x0, SHU1_PLL9_RG_RPHYPLL_LVROD_EN)
		| P_Fld(0x0, SHU1_PLL9_RG_RPHYPLL_RST_DLY));
#endif
#if 0 /* Correct setting moved into PllSetting (Disable SDMPLL fractional mode) */
	vIO32WriteFldAlign(DDRPHY_SHU1_PLL5, 0x0, SHU1_PLL5_RG_RPHYPLL_SDM_FRA_EN);
	vIO32WriteFldAlign(DDRPHY_SHU1_PLL7, 0x0, SHU1_PLL7_RG_RCLRPLL_SDM_FRA_EN);
	vIO32WriteFldAlign(DDRPHY_SHU1_PLL5 + SHIFT_TO_CHB_ADDR, 0x0, SHU1_PLL5_RG_RPHYPLL_SDM_FRA_EN);
	vIO32WriteFldAlign(DDRPHY_SHU1_PLL7 + SHIFT_TO_CHB_ADDR, 0x0, SHU1_PLL7_RG_RCLRPLL_SDM_FRA_EN);
#endif
	/*
	 * vIO32WriteFldAlign_All(DDRPHY_SHU1_PLL4, 0xfe, SHU1_PLL4_RG_RPHYPLL_RESERVED);
	 * vIO32WriteFldAlign_All(DDRPHY_SHU1_PLL6, 0xfe, SHU1_PLL6_RG_RCLRPLL_RESERVED);
	 */
#if 0 /* Use _All() instead */
	vIO32WriteFldAlign(DDRPHY_SHU1_PLL4 + SHIFT_TO_CHB_ADDR, 0xfe, SHU1_PLL4_RG_RPHYPLL_RESERVED);
	vIO32WriteFldAlign(DDRPHY_SHU1_PLL6 + SHIFT_TO_CHB_ADDR, 0xfe, SHU1_PLL6_RG_RCLRPLL_RESERVED);
#endif

	mcDELAY_US(1);


	/* vIO32WriteFldAlign(DDRPHY_SHU1_CA_DLL1, 0x1, SHU1_CA_DLL1_RG_ARDLL_PD_CK_SEL_CA); */
	vIO32WriteFldAlign_All(DDRPHY_SHU1_B0_DLL1, 0x1, SHU1_B0_DLL1_RG_ARDLL_FASTPJ_CK_SEL_B0);
	vIO32WriteFldAlign_All(DDRPHY_SHU1_B1_DLL1, 0x1, SHU1_B1_DLL1_RG_ARDLL_FASTPJ_CK_SEL_B1);
	vIO32WriteFldAlign(DDRPHY_SHU1_CA_DLL1 + SHIFT_TO_CHB_ADDR, 0x1, SHU1_CA_DLL1_RG_ARDLL_FASTPJ_CK_SEL_CA);
#if 0 /* Use _All() instead */
	vIO32WriteFldAlign(DDRPHY_SHU1_B0_DLL1 + SHIFT_TO_CHB_ADDR, 0x1, SHU1_B0_DLL1_RG_ARDLL_FASTPJ_CK_SEL_B0);
	vIO32WriteFldAlign(DDRPHY_SHU1_B1_DLL1 + SHIFT_TO_CHB_ADDR, 0x1, SHU1_B1_DLL1_RG_ARDLL_FASTPJ_CK_SEL_B1);
#endif
	mcDELAY_US(1);


	/*
	 * vIO32WriteFldAlign(DDRPHY_PLL2, 0x0, PLL2_RG_RCLRPLL_EN);
	 * vIO32WriteFldAlign(DDRPHY_SHU1_PLL4, 0xff, SHU1_PLL4_RG_RPHYPLL_RESERVED);
	 * vIO32WriteFldAlign(DDRPHY_SHU1_PLL6, 0xff, SHU1_PLL6_RG_RCLRPLL_RESERVED);
	 */
	vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0 + SHIFT_TO_CHB_ADDR, 0x100000, MISC_CG_CTRL0_CLK_MEM_DFS_CFG);
	vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 0xf, MISC_CG_CTRL0_CLK_MEM_DFS_CFG);

	mcDELAY_US(1);

	/* vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI2, 0x1, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA); */

	mcDELAY_US(1);

	DDRPhyReservedRGSetting(p);

	DDRPhyPLLSetting(p);

	/* rollback tMRRI design change */
#if ENABLE_TMRRI_NEW_MODE
	/* fix rank at 0 to trigger new TMRRI setting */
	vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0, RKCFG_TXRANK);
	vIO32WriteFldAlign(DRAMC_REG_RKCFG, 1, RKCFG_TXRANKFIX);
	vIO32WriteFldAlign(DRAMC_REG_DRSCTRL, 0x0, DRSCTRL_RK_SCINPUT_OPT); /* new mode, HW_MRR: R_DMMRRRK, SW_MRR: R_DMMRSRK */
	vIO32WriteFldMulti(DRAMC_REG_DRAMCTRL, P_Fld(0x0, DRAMCTRL_MRRIOPT) | P_Fld(0x0, DRAMCTRL_TMRRIBYRK_DIS) | P_Fld(0x1, DRAMCTRL_TMRRICHKDIS));
	vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x1, SPCMDCTRL_SC_PG_UPD_OPT);
	vIO32WriteFldMulti(DRAMC_REG_SPCMDCTRL, P_Fld(0x0, SPCMDCTRL_SC_PG_MPRW_DIS) | P_Fld(0x0, SPCMDCTRL_SC_PG_STCMD_AREF_DIS)
		| P_Fld(0x0, SPCMDCTRL_SC_PG_OPT2_DIS) | P_Fld(0x0, SPCMDCTRL_SC_PG_MAN_DIS));
	vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0, RKCFG_TXRANK);
	vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0, RKCFG_TXRANKFIX);
#else
	vIO32WriteFldAlign(DRAMC_REG_DRSCTRL, 0x1, DRSCTRL_RK_SCINPUT_OPT); /* old mode, HW / SW MRR: R_DMMRRRK */
	vIO32WriteFldMulti(DRAMC_REG_DRAMCTRL, P_Fld(0x1, DRAMCTRL_MRRIOPT) | P_Fld(0x1, DRAMCTRL_TMRRIBYRK_DIS) | P_Fld(0x0, DRAMCTRL_TMRRICHKDIS));
	vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x0, SPCMDCTRL_SC_PG_UPD_OPT);
	vIO32WriteFldMulti(DRAMC_REG_SPCMDCTRL, P_Fld(0x1, SPCMDCTRL_SC_PG_MPRW_DIS) | P_Fld(0x1, SPCMDCTRL_SC_PG_STCMD_AREF_DIS)
		| P_Fld(0x1, SPCMDCTRL_SC_PG_OPT2_DIS) | P_Fld(0x1, SPCMDCTRL_SC_PG_MAN_DIS));
#endif
	/* LP3 don't have MPC block ALE mode (review by YH) */
	vIO32WriteFldMulti(DRAMC_REG_MPC_OPTION, P_Fld(0x0, MPC_OPTION_ZQ_BLOCKALE_OPT) | P_Fld(0x0, MPC_OPTION_MPC_BLOCKALE_OPT2) |
		P_Fld(0x0, MPC_OPTION_MPC_BLOCKALE_OPT1) | P_Fld(0x0, MPC_OPTION_MPC_BLOCKALE_OPT));

	vIO32WriteFldAlign(DRAMC_REG_CKECTRL, 0x1, CKECTRL_RUNTIMEMRRCKEFIX); /* Set Run time MRR CKE fix to 1 in tMRRI old mode to avoid no ACK from precharge all */
	vIO32WriteFldAlign(DRAMC_REG_CKECTRL, 0x0, CKECTRL_RUNTIMEMRRMIODIS);

	vIO32WriteFldAlign(DDRPHY_B1_DQ9, 0x1, B1_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B1);
	vIO32WriteFldAlign(DDRPHY_CA_CMD10 + SHIFT_TO_CHB_ADDR, 0x1, CA_CMD10_RG_RX_ARCLK_STBEN_RESETB);
	vIO32WriteFldAlign(DDRPHY_B0_DQ9 + SHIFT_TO_CHB_ADDR, 0x1, B0_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B0);
	vIO32WriteFldAlign(DDRPHY_B1_DQ9 + SHIFT_TO_CHB_ADDR, 0x1, B1_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B1);
	vIO32WriteFldMulti(DDRPHY_MISC_CTRL1, P_Fld(0x1, MISC_CTRL1_R_DMDA_RRESETB_E)
		| P_Fld(0x1, MISC_CTRL1_R_DMDQSIENCG_EN)
		| P_Fld(0x1, MISC_CTRL1_R_DMARPIDQ_SW)
		| P_Fld(0x1, MISC_CTRL1_R_DM_TX_ARCMD_OE)
		| P_Fld(0x1, MISC_CTRL1_R_DM_TX_ARCLK_OE));
	vIO32WriteFldAlign_All(DDRPHY_SHU1_B0_DQ7, 0, SHU1_B0_DQ7_R_DMRXTRACK_DQM_EN_B0); /* no DQM RX in LP3 */
	vIO32WriteFldAlign_All(DDRPHY_SHU1_B1_DQ7, 0, SHU1_B1_DQ7_R_DMRXTRACK_DQM_EN_B1); /* no DQM RX in LP3 */
	vIO32WriteFldAlign_All(DDRPHY_B0_RXDVS0, 1, B0_RXDVS0_R_HWSAVE_MODE_ENA_B0);
	vIO32WriteFldAlign_All(DDRPHY_B1_RXDVS0, 1, B1_RXDVS0_R_HWSAVE_MODE_ENA_B1);
	vIO32WriteFldAlign_All(DDRPHY_CA_RXDVS0, 1, CA_RXDVS0_R_HWSAVE_MODE_ENA_CA);
	vIO32WriteFldMulti(DDRPHY_MISC_CTRL1 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, MISC_CTRL1_R_DMDA_RRESETB_E)
		| P_Fld(0x1, MISC_CTRL1_R_DMDQSIENCG_EN)
		| P_Fld(0x1, MISC_CTRL1_R_DMARPIDQ_SW)
		| P_Fld(0x1, MISC_CTRL1_R_DM_TX_ARCMD_OE)
		| P_Fld(0x1, MISC_CTRL1_R_DM_TX_ARCLK_OE));
#if 0 /* Use _ALl() instead */
	vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ7 + SHIFT_TO_CHB_ADDR, 0, SHU1_B0_DQ7_R_DMRXTRACK_DQM_EN_B0); /* no DQM RX in LP3 */
	vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ7 + SHIFT_TO_CHB_ADDR, 0, SHU1_B1_DQ7_R_DMRXTRACK_DQM_EN_B1); /* no DQM RX in LP3 */
	vIO32WriteFldAlign(DDRPHY_B0_RXDVS0 + SHIFT_TO_CHB_ADDR, 1, B0_RXDVS0_R_HWSAVE_MODE_ENA_B0);
	vIO32WriteFldAlign(DDRPHY_B1_RXDVS0 + SHIFT_TO_CHB_ADDR, 1, B1_RXDVS0_R_HWSAVE_MODE_ENA_B1);
	vIO32WriteFldAlign(DDRPHY_CA_RXDVS0 + SHIFT_TO_CHB_ADDR, 1, CA_RXDVS0_R_HWSAVE_MODE_ENA_CA);
#endif
	vIO32WriteFldMulti(DDRPHY_CA_CMD8, P_Fld(0x0, CA_CMD8_RG_TX_RRESETB_PULL_DN)
		| P_Fld(0x0, CA_CMD8_RG_TX_RRESETB_DDR3_SEL));
	/* | P_Fld(0x1, CA_CMD8_RG_TX_RRESETB_DDR4_SEL)); Don't toggle RESETB_DDR4_SEL, Justin's feedback on RESETB issue */
	vIO32WriteFldAlign(DDRPHY_CA_CMD7, 0x0, CA_CMD7_RG_TX_ARCMD_PULL_DN);
	vIO32WriteFldAlign(DDRPHY_B0_DQ7, 0x0, B0_DQ7_RG_TX_ARDQ_PULL_DN_B0);
	vIO32WriteFldAlign(DDRPHY_B1_DQ7, 0x0, B1_DQ7_RG_TX_ARDQ_PULL_DN_B1);
	vIO32WriteFldAlign(DDRPHY_CA_CMD7 + SHIFT_TO_CHB_ADDR, 0x0, CA_CMD7_RG_TX_ARCMD_PULL_DN);
	vIO32WriteFldAlign(DDRPHY_B0_DQ7 + SHIFT_TO_CHB_ADDR, 0x0, B0_DQ7_RG_TX_ARDQ_PULL_DN_B0);
	vIO32WriteFldAlign(DDRPHY_B1_DQ7 + SHIFT_TO_CHB_ADDR, 0x0, B1_DQ7_RG_TX_ARDQ_PULL_DN_B1);
	vIO32WriteFldMulti(DRAMC_REG_SHU_DQSG_RETRY, P_Fld(0x1, SHU_DQSG_RETRY_R_RDY_SEL_DLE)
		| P_Fld(0x6, SHU_DQSG_RETRY_R_DQSIENLAT)
		| P_Fld(0x1, SHU_DQSG_RETRY_R_RETRY_ONCE));
	vIO32WriteFldMulti(DRAMC_REG_SHUCTRL2, P_Fld(0x0, SHUCTRL2_HWSET_WLRL)
		| P_Fld(0x1, SHUCTRL2_SHU_PERIOD_GO_ZERO_CNT)
		| P_Fld(0x1, SHUCTRL2_R_DVFS_OPTION)
		| P_Fld(0x1, SHUCTRL2_R_DVFS_PARK_N)
		| P_Fld(0x1, SHUCTRL2_R_DVFS_DLL_CHA)
		| P_Fld(0xa, SHUCTRL2_R_DLL_IDLE));
	vIO32WriteFldMulti(DRAMC_REG_SHUCTRL2 + SHIFT_TO_CHB_ADDR, P_Fld(0x0, SHUCTRL2_HWSET_WLRL)
		| P_Fld(0x1, SHUCTRL2_SHU_PERIOD_GO_ZERO_CNT)
		| P_Fld(0x1, SHUCTRL2_R_DVFS_OPTION)
		| P_Fld(0x1, SHUCTRL2_R_DVFS_PARK_N)
		| P_Fld(0xa, SHUCTRL2_R_DLL_IDLE));
	vIO32WriteFldAlign(DRAMC_REG_DVFSDLL, 0x1, DVFSDLL_DLL_LOCK_SHU_EN);
	vIO32WriteFldMulti(DRAMC_REG_DDRCONF0, P_Fld(0x1, DDRCONF0_LPDDR3EN)
		| P_Fld(0x1, DDRCONF0_DM64BITEN)
		| P_Fld(0x1, DDRCONF0_BK8EN));
	vIO32WriteFldMulti(DRAMC_REG_STBCAL2, P_Fld(0x1, STBCAL2_STB_GERR_B23) /* STBCAL gating error by byte2 / 3 (LP3) */
		| P_Fld(0x1, STBCAL2_STB_GERR_B01)
		| P_Fld(0x1, STBCAL2_STB_GERRSTOP));
	vIO32WriteFldAlign(DRAMC_REG_STBCAL2, 0x1, STBCAL2_STB_GERR_RST);
	vIO32WriteFldAlign(DRAMC_REG_STBCAL2, 0x0, STBCAL2_STB_GERR_RST);
	vIO32WriteFldAlign(DRAMC_REG_SHU1_WODT, 0x0, SHU1_WODT_WPST2T);
	vIO32WriteFldAlign(DRAMC_REG_CLKAR, 0x1, CLKAR_PSELAR);
	vIO32WriteFldMulti(DDRPHY_MISC_CTRL0, P_Fld(0x0, MISC_CTRL0_R_DQS0IEN_DIV4_CK_CG_CTRL)
		| P_Fld(0x1, MISC_CTRL0_R_DMDQSIEN_FIFO_EN)
		| P_Fld(0x1, MISC_CTRL0_R_DMSTBEN_OUTSEL)
		| P_Fld(0xf, MISC_CTRL0_R_DMDQSIEN_SYNCOPT));
	vIO32WriteFldMulti(DDRPHY_MISC_CTRL0 + SHIFT_TO_CHB_ADDR, P_Fld(0x1, MISC_CTRL0_R_CLKIEN_DIV4_CK_CG_CTRL)
		| P_Fld(0x1, MISC_CTRL0_R_DMDQSIEN_FIFO_EN)
		| P_Fld(0x1, MISC_CTRL0_R_DMSTBEN_OUTSEL)
		| P_Fld(0xf, MISC_CTRL0_R_DMDQSIEN_SYNCOPT));
	vIO32WriteFldMulti(DRAMC_REG_SHU_CONF0, P_Fld(0x2, SHU_CONF0_MATYPE)
		| P_Fld(0x1, SHU_CONF0_BL4)
		| P_Fld(0x1, SHU_CONF0_FDIV2)
		| P_Fld(0x1, SHU_CONF0_REFTHD)
		| P_Fld(0x1, SHU_CONF0_ADVPREEN)
		| P_Fld(0x3f, SHU_CONF0_DMPGTIM));
	vIO32WriteFldMulti(DRAMC_REG_SHU_ODTCTRL, P_Fld(0x3, SHU_ODTCTRL_TWODT)
	/* | P_Fld(0x5, SHU_ODTCTRL_RODT) //Set in UpdateACTimingReg() */
		| P_Fld(0x1, SHU_ODTCTRL_WOEN));
	vIO32WriteFldAlign(DRAMC_REG_REFCTRL0, 0x5, REFCTRL0_REF_PREGATE_CNT);
	vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_CA1, P_Fld(0x0, SHU_SELPH_CA1_TXDLY_CS1)
		| P_Fld(0x0, SHU_SELPH_CA1_TXDLY_RAS)
		| P_Fld(0x0, SHU_SELPH_CA1_TXDLY_CAS)
		| P_Fld(0x0, SHU_SELPH_CA1_TXDLY_WE)
		| P_Fld(0x0, SHU_SELPH_CA1_TXDLY_RESET)
		| P_Fld(0x0, SHU_SELPH_CA1_TXDLY_ODT)
		| P_Fld(0x0, SHU_SELPH_CA1_TXDLY_CKE)
		| P_Fld(0x0, SHU_SELPH_CA1_TXDLY_CS));
	vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_CA2, P_Fld(0x0, SHU_SELPH_CA2_TXDLY_CKE1)
		| P_Fld(0x0, SHU_SELPH_CA2_TXDLY_CMD)
		| P_Fld(0x0, SHU_SELPH_CA2_TXDLY_BA2)
		| P_Fld(0x0, SHU_SELPH_CA2_TXDLY_BA1)
		| P_Fld(0x0, SHU_SELPH_CA2_TXDLY_BA0));
	vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_CA3, P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA7)
		| P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA6)
		| P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA5)
		| P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA4)
		| P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA3)
		| P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA2)
		| P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA1)
		| P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA0));
	vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_CA4, P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA15)
		| P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA14)
		| P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA13)
		| P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA12)
		| P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA11)
		| P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA10)
		| P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA9)
		| P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA8));
	vIO32WriteFldAlign(DRAMC_REG_SHU_SELPH_CA5, 0x0, SHU_SELPH_CA5_DLY_ODT);
	vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS0, P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS3)
		| P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS2)
		| P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS1)
		| P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS0)
		| P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_DQS3)
		| P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_DQS2)
		| P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_DQS1)
		| P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_DQS0));
	vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS1, P_Fld(0x1, SHU_SELPH_DQS1_DLY_OEN_DQS3)
		| P_Fld(0x1, SHU_SELPH_DQS1_DLY_OEN_DQS2)
		| P_Fld(0x1, SHU_SELPH_DQS1_DLY_OEN_DQS1)
		| P_Fld(0x1, SHU_SELPH_DQS1_DLY_OEN_DQS0)
		| P_Fld(0x3, SHU_SELPH_DQS1_DLY_DQS3)
		| P_Fld(0x3, SHU_SELPH_DQS1_DLY_DQS2)
		| P_Fld(0x3, SHU_SELPH_DQS1_DLY_DQS1)
		| P_Fld(0x3, SHU_SELPH_DQS1_DLY_DQS0));
	vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3)
		| P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2)
		| P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1)
		| P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0)
		| P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ3)
		| P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ2)
		| P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ1)
		| P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ0));
	vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3)
		| P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2)
		| P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1)
		| P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0)
		| P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM3)
		| P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM2)
		| P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM1)
		| P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM0));
	vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ2, P_Fld(0x1, SHURK0_SELPH_DQ2_DLY_OEN_DQ3)
		| P_Fld(0x1, SHURK0_SELPH_DQ2_DLY_OEN_DQ2)
		| P_Fld(0x1, SHURK0_SELPH_DQ2_DLY_OEN_DQ1)
		| P_Fld(0x1, SHURK0_SELPH_DQ2_DLY_OEN_DQ0)
		| P_Fld(0x3, SHURK0_SELPH_DQ2_DLY_DQ3)
		| P_Fld(0x3, SHURK0_SELPH_DQ2_DLY_DQ2)
		| P_Fld(0x3, SHURK0_SELPH_DQ2_DLY_DQ1)
		| P_Fld(0x3, SHURK0_SELPH_DQ2_DLY_DQ0));
	vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ3, P_Fld(0x1, SHURK0_SELPH_DQ3_DLY_OEN_DQM3)
		| P_Fld(0x1, SHURK0_SELPH_DQ3_DLY_OEN_DQM2)
		| P_Fld(0x1, SHURK0_SELPH_DQ3_DLY_OEN_DQM1)
		| P_Fld(0x1, SHURK0_SELPH_DQ3_DLY_OEN_DQM0)
		| P_Fld(0x3, SHURK0_SELPH_DQ3_DLY_DQM3)
		| P_Fld(0x3, SHURK0_SELPH_DQ3_DLY_DQM2)
		| P_Fld(0x3, SHURK0_SELPH_DQ3_DLY_DQM1)
		| P_Fld(0x3, SHURK0_SELPH_DQ3_DLY_DQM0));
	vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3)
		| P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2)
		| P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1)
		| P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0)
		| P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ3)
		| P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ2)
		| P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ1)
		| P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ0));
	vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3)
		| P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2)
		| P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1)
		| P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0)
		| P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM3)
		| P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM2)
		| P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM1)
		| P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM0));
	vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ2, P_Fld(0x1, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ3)
		| P_Fld(0x1, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ2)
		| P_Fld(0x1, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ1)
		| P_Fld(0x1, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ0)
		| P_Fld(0x3, SHURK1_SELPH_DQ2_DLY_R1DQ3)
		| P_Fld(0x3, SHURK1_SELPH_DQ2_DLY_R1DQ2)
		| P_Fld(0x3, SHURK1_SELPH_DQ2_DLY_R1DQ1)
		| P_Fld(0x3, SHURK1_SELPH_DQ2_DLY_R1DQ0));
	vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ3, P_Fld(0x1, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM3)
		| P_Fld(0x1, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM2)
		| P_Fld(0x1, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM1)
		| P_Fld(0x1, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM0)
		| P_Fld(0x3, SHURK1_SELPH_DQ3_DLY_R1DQM3)
		| P_Fld(0x3, SHURK1_SELPH_DQ3_DLY_R1DQM2)
		| P_Fld(0x3, SHURK1_SELPH_DQ3_DLY_R1DQM1)
		| P_Fld(0x3, SHURK1_SELPH_DQ3_DLY_R1DQM0));
	mcDELAY_US(1);

	vIO32WriteFldMulti(DRAMC_REG_STBCAL, P_Fld(0x0, STBCAL_DQSIENMODE_SELPH)
		| P_Fld(0x1, STBCAL_STB_DQIEN_IG)
		| P_Fld(0x1, STBCAL_PICHGBLOCK_NORD)
		| P_Fld(0x1, STBCAL_PIMASK_RKCHG_OPT));
	vIO32WriteFldMulti(DRAMC_REG_SHU1_DQSG, P_Fld(0x9, SHU1_DQSG_STB_UPDMASKCYC)
		| P_Fld(0x1, SHU1_DQSG_STB_UPDMASK_EN));
	vIO32WriteFldAlign(DRAMC_REG_STBCAL, 0x1, STBCAL_DQSIENMODE);
	vIO32WriteFldMulti(DRAMC_REG_SREFCTRL, P_Fld(0x0, SREFCTRL_SREF_HW_EN)
		| P_Fld(0x8, SREFCTRL_SREFDLY)
		| P_Fld(0x0, SREFCTRL_SREF2_OPTION));

#ifdef FIRST_BRING_UP /* Add after DE setting review */
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_PIPE), P_Fld(0x1, SHU_PIPE_READ_START_EXTEND1)
			| P_Fld(0x1, SHU_PIPE_DLE_LAST_EXTEND1)
			| P_Fld(0x1, SHU_PIPE_READ_START_EXTEND2)
			| P_Fld(0x1, SHU_PIPE_DLE_LAST_EXTEND2)
			| P_Fld(0x1, SHU_PIPE_READ_START_EXTEND3)
			| P_Fld(0x1, SHU_PIPE_DLE_LAST_EXTEND3));
#else
		vIO32WriteFldMulti(DRAMC_REG_SHU_PIPE, P_Fld(0x1, SHU_PIPE_READ_START_EXTEND1)
			| P_Fld(0x1, SHU_PIPE_DLE_LAST_EXTEND1));
#endif
	vIO32WriteFldMulti(DRAMC_REG_CKECTRL, P_Fld(0x1, CKECTRL_CKEON)
		| P_Fld(0x1, CKECTRL_CKETIMER_SEL));
	vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0x1, RKCFG_CKE2RANK_OPT2);
	vIO32WriteFldAlign(DRAMC_REG_SHU_CONF2, 0x7, SHU_CONF2_DCMDLYREF);
	/* | P_Fld(0x32, SHU_CONF2_FSPCHG_PRDCNT)); //ACTiming related -> set in UpdateACTiming_Reg() */
	vIO32WriteFldMulti(DRAMC_REG_SHU_SCINTV, P_Fld(0x0, SHU_SCINTV_DQS2DQ_SHU_PITHRD)
	/* | P_Fld(0xF, SHU_SCINTV_MRW_INTV) //YH Tsai: DVFS MRW counter(Set in UpdateACTimingReg()) */
		| P_Fld(0x0, SHU_SCINTV_RDDQC_INTV)
		| P_Fld(0x0, SHU_SCINTV_TZQLAT));
#if 0 /* Set in UpdateACTimingReg() */
	vIO32WriteFldMulti(DRAMC_REG_SHU_CONF1, P_Fld(0xc, SHU_CONF1_DATLAT_DSEL_PHY)
		| P_Fld(0xc, SHU_CONF1_DATLAT_DSEL)
		| P_Fld(0xe, SHU_CONF1_DATLAT));
#endif
	vIO32WriteFldAlign(DRAMC_REG_SHUCTRL, 0x1, SHUCTRL_LPSM_BYPASS_B);
	vIO32WriteFldAlign(DRAMC_REG_REFCTRL1, 0x0, REFCTRL1_SREF_PRD_OPT);
	/*
	 * vIO32WriteFldAlign(DDRPHY_SHU1_PLL4, 0xfe, SHU1_PLL4_RG_RPHYPLL_RESERVED);
	 * vIO32WriteFldAlign(DDRPHY_SHU1_PLL6, 0xfe, SHU1_PLL6_RG_RCLRPLL_RESERVED);
	 */
	vIO32WriteFldAlign(DRAMC_REG_REFRATRE_FILTER, 0x1, REFRATRE_FILTER_PB2AB_OPT);
	vIO32WriteFldAlign(DDRPHY_MISC_CTRL1, 0x1, MISC_CTRL1_R_DMDA_RRESETB_I);
	vIO32WriteFldAlign(DRAMC_REG_DRAMCTRL, 0x0, DRAMCTRL_CLKWITRFC);
	vIO32WriteFldMulti(DRAMC_REG_MISCTL0, P_Fld(0x1, MISCTL0_REFP_ARB_EN2)
		| P_Fld(0x1, MISCTL0_PBC_ARB_EN)
		| P_Fld(0x1, MISCTL0_REFA_ARB_EN2));
	vIO32WriteFldMulti(DRAMC_REG_PERFCTL0, P_Fld(0x1, PERFCTL0_MWHPRIEN)
	/* | P_Fld(0x1, PERFCTL0_RWSPLIT) //Set in UpdateInitialSettings_LP3() */
		| P_Fld(0x1, PERFCTL0_WFLUSHEN)
		| P_Fld(0x1, PERFCTL0_EMILLATEN)
		| P_Fld(0x1, PERFCTL0_RWAGEEN)
		| P_Fld(0x1, PERFCTL0_RWLLATEN)
		| P_Fld(0x1, PERFCTL0_RWHPRIEN)
		| P_Fld(0x1, PERFCTL0_RWOFOEN)
		| P_Fld(0x1, PERFCTL0_DISRDPHASE1)
		| P_Fld(0x1, PERFCTL0_DUALSCHEN));
	vIO32WriteFldAlign(DRAMC_REG_ARBCTL, 0x80, ARBCTL_MAXPENDCNT);
	vIO32WriteFldMulti(DRAMC_REG_PADCTRL, P_Fld(0x1, PADCTRL_DQIENLATEBEGIN)
		| P_Fld(0x1, PADCTRL_DQIENQKEND));
	vIO32WriteFldAlign(DRAMC_REG_DRAMC_PD_CTRL, 0x1, DRAMC_PD_CTRL_DCMREF_OPT);
	vIO32WriteFldAlign(DRAMC_REG_CLKCTRL, 0x1, CLKCTRL_CLK_EN_1);
	vIO32WriteFldMulti(DRAMC_REG_REFCTRL0, P_Fld(0x4, REFCTRL0_DISBYREFNUM)
		| P_Fld(0x1, REFCTRL0_DLLFRZ));
	vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x1, SPCMDCTRL_CLR_EN);
#if 0 /* Berson: LP3 doesn't have function for CATRAIN_INTV */
	vIO32WriteFldMulti(DRAMC_REG_CATRAINING1, P_Fld(0x4, CATRAINING1_CATRAIN_INTV)
		| P_Fld(0x3, CATRAINING1_CATRAINLAT));
#endif
	vIO32WriteFldAlign(DRAMC_REG_STBCAL, 0x1, STBCAL_REFUICHG);
	vIO32WriteFldMulti(DRAMC_REG_SHU_RANKCTL, P_Fld(0x5, SHU_RANKCTL_RANKINCTL_PHY)
		| P_Fld(0x2, SHU_RANKCTL_RANKINCTL_ROOT1)
		| P_Fld(0x2, SHU_RANKCTL_RANKINCTL));
	vIO32WriteFldAlign(DRAMC_REG_SHU_STBCAL, 0x1, SHU_STBCAL_DMSTBLAT);
	/*
	 * vIO32WriteFldAlign(DRAMC_REG_SHURK0_DQSCTL, 0x4, SHURK0_DQSCTL_DQSINCTL); //DQSINCTL: set in UpdateACTimingReg()
	 * vIO32WriteFldAlign(DRAMC_REG_SHURK1_DQSCTL, 0x4, SHURK1_DQSCTL_R1DQSINCTL); //DQSINCTL: set in UpdateACTimingReg()
	 */

	mcDELAY_US(2);

#if LEGACY_GATING_DLY
	LegacyGatingDlyLP3(p);
#endif

	/* TODO: overwrite 1866 setting START */
	vIO32WriteFldMulti(DRAMC_REG_SHU1_WODT, P_Fld(0x1, SHU1_WODT_DISWODTE)
		| P_Fld(0x1, SHU1_WODT_WODTFIXOFF)
		| P_Fld(0x4, SHU1_WODT_DISWODT));

	/*
	 * vIO32WriteFldAlign(DRAMC_REG_SHU_CONF3, 0x5, SHU_CONF3_ZQCSCNT);
	 * vIO32WriteFldAlign(DRAMC_REG_SHU_CONF3, 0xff, SHU_CONF3_REFRCNT);
	 * vIO32WriteFldAlign(DRAMC_REG_SHU_CONF1, 0xb0, SHU_CONF1_REFBW_FR);
	 */

	vIO32WriteFldAlign(DRAMC_REG_SHU_STBCAL, 0x1, SHU_STBCAL_DQSG_MODE);
	vIO32WriteFldMulti(DRAMC_REG_SHU_CONF0, P_Fld(0x5, SHU_CONF0_REFTHD)
		| P_Fld(0x15, SHU_CONF0_DMPGTIM));
	/*
	 * vIO32WriteFldAlign(DRAMC_REG_SHU_CONF1, 0xae, SHU_CONF1_REFBW_FR);
	 * vIO32WriteFldMulti(DRAMC_REG_SHU_CONF3, P_Fld(0x8d, SHU_CONF3_REFRCNT)
	 * | P_Fld(0x1f8, SHU_CONF3_ZQCSCNT));
	 */
	vIO32WriteFldAlign(DRAMC_REG_SHU_STBCAL, 0x1, SHU_STBCAL_PICGLAT);
	/* vIO32WriteFldAlign(DRAMC_REG_SHU1_WODT, 0x1, SHU1_WODT_TWPSTEXT); */

	/* if(p->frequency==933) */
	{
		/* 91423 ===dramc_shu1_lp3_1866 begin=== */
		vIO32WriteFldMulti(DRAMC_REG_SHU_CONF0, P_Fld(0x1, SHU_CONF0_REFTHD)
			| P_Fld(0x3f, SHU_CONF0_DMPGTIM));
		vIO32WriteFldMulti(DRAMC_REG_SHU_CONF1, P_Fld(0x1, SHU_CONF1_TREFBWIG) | P_Fld(0x0, SHU_CONF1_REFBW_FR)); /* tREFBW is 0xb0 = 176 for all LP3 freqs (ACTiming excel file) */
		vIO32WriteFldMulti(DRAMC_REG_SHU_CONF3, P_Fld(0xff, SHU_CONF3_REFRCNT)
			| P_Fld(0x5, SHU_CONF3_ZQCSCNT));
		vIO32WriteFldAlign(DRAMC_REG_SHU_RANKCTL, 0x3, SHU_RANKCTL_RANKINCTL_PHY);
		/* vIO32WriteFldAlign(DRAMC_REG_SHU_CKECTRL, 0x3, SHU_CKECTRL_CKEPRD); CKEPRD's correct value is set in UpdateACTImingReg( ) */
		vIO32WriteFldAlign(DRAMC_REG_SHU1_WODT, 0x0, SHU1_WODT_TWPSTEXT);
		vIO32WriteFldAlign(DRAMC_REG_SHU1_DQSG, 0xf, SHU1_DQSG_SCINTV);
		/*
		 * vIO32WriteFldMulti(DRAMC_REG_SHU_HWSET_MR2, P_Fld(0x0, SHU_HWSET_MR2_HWSET_MR2_OP)
		 * | P_Fld(0x0, SHU_HWSET_MR2_HWSET_MR2_MRSMA));
		 * vIO32WriteFldMulti(DRAMC_REG_SHU_HWSET_MR13, P_Fld(0x1c, SHU_HWSET_MR13_HWSET_MR13_OP)
		 * | P_Fld(0x2, SHU_HWSET_MR13_HWSET_MR13_MRSMA));
		 * vIO32WriteFldMulti(DRAMC_REG_SHU_HWSET_VRCG, P_Fld(0x0, SHU_HWSET_VRCG_HWSET_VRCG_OP)
		 * | P_Fld(0x0, SHU_HWSET_VRCG_HWSET_VRCG_MRSMA));
		 */
		vIO32WriteFldMulti(DRAMC_REG_SHU_DQSG_RETRY, P_Fld(0x1, SHU_DQSG_RETRY_R_RETRY_USE_BURST_MDOE)
			| P_Fld(0x1, SHU_DQSG_RETRY_R_RETRY_ROUND_NUM)
			| P_Fld(0x0, SHU_DQSG_RETRY_R_XSR_RETRY_SPM_MODE) /* cc change from 1->0 after setting review */
			| P_Fld(0x3, SHU_DQSG_RETRY_R_DQSIENLAT)
			| P_Fld(0x1, SHU_DQSG_RETRY_R_DM4BYTE));
		/* 92288 ===dramc_shu1_lp3_1866 end=== */

		/* 93153 ===ddrphy_shu1_lp3_1866_CHA begin=== */
		vIO32WriteFldAlign_All(DDRPHY_SHU1_B0_DQ5, 0x0, SHU1_B0_DQ5_RG_ARPI_FB_B0);
		vIO32WriteFldAlign_All(DDRPHY_SHU1_B1_DQ5, 0x0, SHU1_B1_DQ5_RG_ARPI_FB_B1);
		vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_CMD5, 0x0, SHU1_CA_CMD5_RG_ARPI_FB_CA);
		/*
		 * 93918 ===ddrphy_shu1_lp3_1866_CHA end===
		 * 93918 ===ddrphy_shu1_lp3_1866_CHB begin===
		 */
	#if 0 /* Use _All() instead */
		vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ5 + SHIFT_TO_CHB_ADDR, 0x4, SHU1_B0_DQ5_RG_ARPI_FB_B0);
		vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5 + SHIFT_TO_CHB_ADDR, 0x4, SHU1_B1_DQ5_RG_ARPI_FB_B1);
		vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD5 + SHIFT_TO_CHB_ADDR, 0x4, SHU1_CA_CMD5_RG_ARPI_FB_CA);
	#endif
		/* vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD6+SHIFT_TO_CHB_ADDR, 0x3, SHU1_CA_CMD6_RG_ARPI_RESERVE_CA); */
		vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ2 + SHIFT_TO_CHB_ADDR, P_Fld(0xa, SHU1_R0_B0_DQ2_RK0_RX_ARDQ1_F_DLY_B0)
			| P_Fld(0xa, SHU1_R0_B0_DQ2_RK0_RX_ARDQ0_F_DLY_B0));
		/* 94591 ===ddrphy_shu1_lp3_1866_CHB end=== */
	}
	/*
	 * else
	 * TODO: overwrite 1866 setting END
	 */

	/*
	 * Since the above initial settings are for LP3_DDR1866, TX related initial delay settings needs to be adjusted
	 * according it's Write Latency difference
	 */
	if (p->freqGroup != 933)
	{
		TxCalibrationMoveDQSByDifferentWL(p); /* TX DQS, DQS_OEN, DQ delay settings should be adjusted to prevent TX calibration K fail */
	}

	/* TODO: */
	vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0x1, RKCFG_RKMODE);

	UpdateInitialSettings_LP3(p);

#if SIMULATION_SW_IMPED
	DramcSwImpedanceSaveRegister(p, ODT_OFF, ODT_OFF, DRAM_DFS_SHUFFLE_1);
#endif

#ifndef LOOPBACK_TEST
	DDRPhyFMeter_Init();
	DDRPhyFreqMeter();
#endif

#if LP3_MR_INIT_AFTER_CA_TRAIN
    DramcModeRegInit_LP3(p, TRUE);
#else
	DramcModeRegInit_LP3(p);
#endif

#if 0
	vIO32WriteFldMulti(DRAMC_REG_MRS, P_Fld(0x0, MRS_MRSRK)
		| P_Fld(0x4, MRS_MRSMA)
		| P_Fld(0x0, MRS_MRSOP));

	mcDELAY_US(1);

	vIO32WriteFldAlign(DRAMC_REG_SPCMD, 0x1, SPCMD_MRREN);

	mcDELAY_US(1);
#endif

	vIO32WriteFldMulti(DRAMC_REG_REFCTRL0, P_Fld(0x0, REFCTRL0_PBREFEN)
		| P_Fld(0x1, REFCTRL0_PBREF_DISBYRATE));
	vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0x0, RKCFG_DQSOSC2RK);
	/*
	 * vIO32WriteFldAlign(DRAMC_REG_CKECTRL, 0x0, CKECTRL_CKEFIXON);
	 * vIO32WriteFldAlign(DRAMC_REG_HW_MRR_FUN, 0x1, HW_MRR_FUN_TMRR_ENA);
	 */
	vIO32WriteFldAlign(DRAMC_REG_DRAMCTRL, 0x1, DRAMCTRL_PREALL_OPTION);
	vIO32WriteFldAlign(DRAMC_REG_ZQCS, 0x56, ZQCS_ZQCSOP);

	mcDELAY_US(1);


	/* vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x0, SPCMDCTRL_REFRDIS);//Lewis@20160613: Fix refresh rate is wrong while diable MR4 */
	vIO32WriteFldMulti(DRAMC_REG_REFCTRL0, P_Fld(0x1, REFCTRL0_REFFRERUN)
		| P_Fld(0x1, REFCTRL0_REFDIS));
	vIO32WriteFldAlign(DRAMC_REG_SREFCTRL, 0x1, SREFCTRL_SREF_HW_EN);
	vIO32WriteFldAlign(DRAMC_REG_MPC_OPTION, 0x1, MPC_OPTION_MPCRKEN);
	vIO32WriteFldMulti(DRAMC_REG_DRAMC_PD_CTRL, P_Fld(0x1, DRAMC_PD_CTRL_PHYCLKDYNGEN)
		| P_Fld(0x1, DRAMC_PD_CTRL_DCMEN));
	vIO32WriteFldMulti(DRAMC_REG_DRAMC_PD_CTRL + SHIFT_TO_CHB_ADDR, P_Fld(0x0, DRAMC_PD_CTRL_COMBCLKCTRL)
		| P_Fld(0x0, DRAMC_PD_CTRL_DCMENNOTRFC)
		| P_Fld(0x1, DRAMC_PD_CTRL_DCMEN));
	vIO32WriteFldMulti(DRAMC_REG_EYESCAN, P_Fld(0x0, EYESCAN_RX_DQ_EYE_SEL)
		| P_Fld(0x0, EYESCAN_RG_RX_EYE_SCAN_EN));
	vIO32WriteFldMulti(DRAMC_REG_STBCAL1, P_Fld(0x1, STBCAL1_STBCNT_LATCH_EN)
		| P_Fld(0x1, STBCAL1_STBENCMPEN));
	vIO32WriteFldAlign(DRAMC_REG_TEST2_1, 0x10000, TEST2_1_TEST2_BASE);
	vIO32WriteFldAlign(DRAMC_REG_TEST2_2, 0x400, TEST2_2_TEST2_OFF);
	vIO32WriteFldMulti(DRAMC_REG_TEST2_3, P_Fld(0x1, TEST2_3_TEST2WREN2_HW_EN)
		| P_Fld(0x4, TEST2_3_DQSICALSTP)
		| P_Fld(0x1, TEST2_3_TESTAUDPAT)
		| P_Fld(0x0, TEST2_3_TESTADR_SHIFT)); /* test agent's address shift 1: for LPDDR4, 0: others(LPDDR3) */
	vIO32WriteFldAlign(DRAMC_REG_SHUCTRL2, 0x1, SHUCTRL2_MR13_SHU_EN);
	vIO32WriteFldAlign(DRAMC_REG_DRAMCTRL, 0x1, DRAMCTRL_REQQUE_THD_EN);
	/* | P_Fld(0x1, DRAMCTRL_DPDRK_OPT)); DPDRK_OPT doesn't exist on 6771 */
	vIO32WriteFldAlign(DRAMC_REG_SHU_CKECTRL, 0x3, SHU_CKECTRL_SREF_CK_DLY);

	vIO32WriteFldAlign(DRAMC_REG_DUMMY_RD, p->support_rank_num, DUMMY_RD_RANK_NUM);

	/* cc change from 4->0 after DE setting review */
	vIO32WriteFldAlign(DRAMC_REG_TEST2_4, 0x0, TEST2_4_TESTAGENTRKSEL);
	vIO32WriteFldMulti(DRAMC_REG_REFCTRL1, P_Fld(0x1, REFCTRL1_REF_QUE_AUTOSAVE_EN)
		| P_Fld(0x1, REFCTRL1_SLEFREF_AUTOSAVE_EN));
	vIO32WriteFldMulti(DRAMC_REG_RSTMASK, P_Fld(0x0, RSTMASK_GT_SYNC_MASK)
		| P_Fld(0x0, RSTMASK_DVFS_SYNC_MASK)
		| P_Fld(0x0, RSTMASK_GT_SYNC_MASK_FOR_PHY)
		| P_Fld(0x0, RSTMASK_DVFS_SYNC_MASK_FOR_PHY));
	/*
	 * vIO32WriteFldAlign(DRAMC_REG_DRAMCTRL, 0x0, DRAMCTRL_CTOREQ_HPRI_OPT);
	 * 91074 === DE initial sequence done ===
	 * #ifdef DVFS_Enable
	 */
	vIO32WriteFldAlign(DRAMC_REG_STBCAL1, 0x8, STBCAL1_STBCAL_FILTER);
	vIO32WriteFldMulti(DRAMC_REG_STBCAL, P_Fld(0x0, STBCAL_STBCALEN)
		| P_Fld(0x0, STBCAL_STB_SELPHYCALEN)
		| P_Fld(0x0, STBCAL_REFUICHG)
		| P_Fld(0x1, STBCAL_SREF_DQSGUPD));

	mcDELAY_US(1);

	/* *((UINT32P)(DDRPHY0AO_BASE + 0x0000)) = 0x00000000; // trun off when SPM need */
	vIO32WriteFldAlign(DRAMC_REG_DDRCONF0, 0x1, DDRCONF0_AG0MWR);
	/*
	 * vIO32WriteFldMulti(DRAMC_REG_DRAMCTRL, P_Fld(0x1, DRAMCTRL_FW2R)
	 * | P_Fld(0x0, DRAMCTRL_DYNMWREN)
	 * | P_Fld(0x1, DRAMCTRL_CLKWITRFC)
	 * | P_Fld(0x1, DRAMCTRL_ADRBIT3DEC)
	 * | P_Fld(0x1, DRAMCTRL_CTOREQ_HPRI_OPT));
	 */
#if 0 /* Darren - by Jeremy confirm */
	vIO32WriteFldAlign(DRAMC_REG_MISCTL0, 0x0, MISCTL0_PBC_ARB_EN);
	vIO32WriteFldMulti(DRAMC_REG_PERFCTL0, P_Fld(0x1, PERFCTL0_REORDEREN)
		| P_Fld(0x1, PERFCTL0_RWHPRICTL)
		| P_Fld(0x0, PERFCTL0_EMILLATEN)
		| P_Fld(0x0, PERFCTL0_RWAGEEN)
		| P_Fld(0x0, PERFCTL0_RWHPRIEN));
#endif
	vIO32WriteFldMulti(DRAMC_REG_RSTMASK, P_Fld(0x0, RSTMASK_PHY_SYNC_MASK)
		| P_Fld(0x0, RSTMASK_DAT_SYNC_MASK));
	/* vIO32WriteFldAlign(DRAMC_REG_SREFCTRL, 0xa, SREFCTRL_SREFDLY); */
#if 0 /* Darren - by Jeremy confirm */
	vIO32WriteFldMulti(DRAMC_REG_REFCTRL0, P_Fld(0x1, REFCTRL0_REFNA_OPT)
		| P_Fld(0x2, REFCTRL0_DISBYREFNUM)
		| P_Fld(0x1, REFCTRL0_UPDBYWR));
#endif
	vIO32WriteFldMulti(DRAMC_REG_REFRATRE_FILTER, P_Fld(0x0, REFRATRE_FILTER_REFRATE_FIL7)
		| P_Fld(0x1, REFRATRE_FILTER_REFRATE_FIL6)
		| P_Fld(0x7, REFRATRE_FILTER_REFRATE_FIL5)
		| P_Fld(0x5, REFRATRE_FILTER_REFRATE_FIL4)
		| P_Fld(0x6, REFRATRE_FILTER_REFRATE_FIL2)
		| P_Fld(0x4, REFRATRE_FILTER_REFRATE_FIL1)
		| P_Fld(0x6, REFRATRE_FILTER_REFRATE_FIL0));
	vIO32WriteFldMulti(DRAMC_REG_ZQCS, P_Fld(0x0, ZQCS_ZQCSDUAL)
		| P_Fld(0x0, ZQCS_ZQCSMASK));
	vIO32WriteFldAlign(DRAMC_REG_HW_MRR_FUN, 0x1, HW_MRR_FUN_MRR_HW_HIPRI);
	vIO32WriteFldAlign(DRAMC_REG_DUMMY_RD, 0x1, DUMMY_RD_SREF_DMYRD_EN);
	vIO32WriteFldAlign(DRAMC_REG_STBCAL1, 0x2, STBCAL1_STBCAL_FILTER);
	/* 91423 ===LP3_1866_intial_setting_shu1 end=== */

	DVFSSettings(p);
}
#endif


static void DramcSetting_Olympus_LP4_ByteMode_DDR3733(DRAMC_CTX_T *p)
{
	vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS0, P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS3)
		| P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS2)
		| P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS1)
		| P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS0)
		| P_Fld(0x4, SHU_SELPH_DQS0_TXDLY_DQS3)
		| P_Fld(0x4, SHU_SELPH_DQS0_TXDLY_DQS2)
		| P_Fld(0x4, SHU_SELPH_DQS0_TXDLY_DQS1)
		| P_Fld(0x4, SHU_SELPH_DQS0_TXDLY_DQS0));
	vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS1, P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS3)
		| P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS2)
		| P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS1)
		| P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS0)
		| P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS3)
		| P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS2)
		| P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS1)
		| P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS0));
}


static void DramcSetting_Olympus_LP4_ByteMode_DDR2667(DRAMC_CTX_T *p)
{
	/* DRAMC0-SHU2 */
#if LEGACY_TDQSCK_PRECAL
	LegacyPreCalLP4_DDR2667(p);
#endif
	/* vIO32WriteFldAlign(DRAMC_REG_SHU_CONF2, 0x54, SHU_CONF2_FSPCHG_PRDCNT);//ACTiming related -> set in UpdateACTiming_Reg() */
	vIO32WriteFldMulti(DRAMC_REG_SHU_RANKCTL, P_Fld(0x4, SHU_RANKCTL_RANKINCTL_PHY)
		| P_Fld(0x2, SHU_RANKCTL_RANKINCTL_ROOT1)
		| P_Fld(0x2, SHU_RANKCTL_RANKINCTL));
	vIO32WriteFldMulti(DRAMC_REG_SHU_CKECTRL, P_Fld(0x3, SHU_CKECTRL_TCKESRX));
	/* | P_Fld(0x3, SHU_CKECTRL_CKEPRD)); */
	vIO32WriteFldMulti(DRAMC_REG_SHU_ODTCTRL, P_Fld(0x1, SHU_ODTCTRL_RODTE)
		| P_Fld(0x1, SHU_ODTCTRL_RODTE2)
	/* | P_Fld(0x4, SHU_ODTCTRL_RODT) //Set in UpdateACTimingReg() */
		| P_Fld(0x1, SHU_ODTCTRL_ROEN));
	vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ7, 0x1, SHU1_B0_DQ7_R_DMRODTEN_B0);
	vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ7, 0x1, SHU1_B1_DQ7_R_DMRODTEN_B1);
#if LEGACY_TX_TRACK
	LegacyTxTrackLP4_DDR2667(p);
#endif
	vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS0, P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_OEN_DQS3)
		| P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_OEN_DQS2)
		| P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_OEN_DQS1)
		| P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_OEN_DQS0)
		| P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_DQS3)
		| P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_DQS2)
		| P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_DQS1)
		| P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_DQS0));
	vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS1, P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS3)
		| P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS2)
		| P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS1)
		| P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS0)
		| P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS3)
		| P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS2)
		| P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS1)
		| P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS0));
	if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
	{
		vIO32WriteFldAlign(DRAMC_REG_SHU1_WODT, 0x0, SHU1_WODT_WPST2T);
	}
	else
	{
		vIO32WriteFldMulti(DRAMC_REG_SHU1_WODT, P_Fld(0x0, SHU1_WODT_WPST2T)
			| P_Fld(0x1, SHU1_WODT_DBIWR));
	}
	vIO32WriteFldAlign(DRAMC_REG_SHU_DQS2DQ_TX, 0x0, SHU_DQS2DQ_TX_OE2DQ_OFFSET);
	/*
	 * vIO32WriteFldAlign(DRAMC_REG_SHU_HWSET_MR2, 0x24, SHU_HWSET_MR2_HWSET_MR2_OP);
	 * vIO32WriteFldAlign(DRAMC_REG_SHU_HWSET_MR13, 0xc8, SHU_HWSET_MR13_HWSET_MR13_OP);
	 * vIO32WriteFldAlign(DRAMC_REG_SHU_HWSET_VRCG, 0xc0, SHU_HWSET_VRCG_HWSET_VRCG_OP);
	 */
	vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQSIEN, P_Fld(0x19, SHURK0_DQSIEN_R0DQS1IEN)
		| P_Fld(0x19, SHURK0_DQSIEN_R0DQS0IEN));
	vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_PI, P_Fld(0x14, SHU1RK0_PI_RK0_ARPI_DQM_B1)
		| P_Fld(0x14, SHU1RK0_PI_RK0_ARPI_DQM_B0)
		| P_Fld(0x14, SHU1RK0_PI_RK0_ARPI_DQ_B1)
		| P_Fld(0x14, SHU1RK0_PI_RK0_ARPI_DQ_B0));

#if LEGACY_GATING_DLY
	LegacyGatingDlyLP4_DDR2667(p);
#endif

	if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
	{
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3)
			| P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2)
			| P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1)
			| P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0)
			| P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ3)
			| P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ2)
			| P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ1)
			| P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ0));
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3)
			| P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2)
			| P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1)
			| P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0)
			| P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM3)
			| P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM2)
			| P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM1)
			| P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM0));
	}
	else
	{
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3)
			| P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2)
			| P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1)
			| P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0)
			| P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ3)
			| P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ2));
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3)
			| P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2)
			| P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1)
			| P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0)
			| P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM3)
			| P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM2));
	}
	vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ2, P_Fld(0x0, SHURK0_SELPH_DQ2_DLY_OEN_DQ3)
		| P_Fld(0x0, SHURK0_SELPH_DQ2_DLY_OEN_DQ2)
		| P_Fld(0x0, SHURK0_SELPH_DQ2_DLY_OEN_DQ1)
		| P_Fld(0x0, SHURK0_SELPH_DQ2_DLY_OEN_DQ0)
		| P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_DQ3)
		| P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_DQ2)
		| P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_DQ1)
		| P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_DQ0));
	vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ3, P_Fld(0x0, SHURK0_SELPH_DQ3_DLY_OEN_DQM3)
		| P_Fld(0x0, SHURK0_SELPH_DQ3_DLY_OEN_DQM2)
		| P_Fld(0x0, SHURK0_SELPH_DQ3_DLY_OEN_DQM1)
		| P_Fld(0x0, SHURK0_SELPH_DQ3_DLY_OEN_DQM0)
		| P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_DQM3)
		| P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_DQM2)
		| P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_DQM1)
		| P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_DQM0));
	/* vIO32WriteFldAlign(DRAMC_REG_SHURK1_DQSCTL, 0x4, SHURK1_DQSCTL_R1DQSINCTL); //Set in UpdateACTimingReg() */
	vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQSIEN, P_Fld(0x1f, SHURK1_DQSIEN_R1DQS1IEN)
		| P_Fld(0x1f, SHURK1_DQSIEN_R1DQS0IEN));
	vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_PI, P_Fld(0x14, SHU1RK1_PI_RK1_ARPI_DQM_B1)
		| P_Fld(0x14, SHU1RK1_PI_RK1_ARPI_DQM_B0)
		| P_Fld(0x14, SHU1RK1_PI_RK1_ARPI_DQ_B1)
		| P_Fld(0x14, SHU1RK1_PI_RK1_ARPI_DQ_B0));
	if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
	{
		vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3)
			| P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2)
			| P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1)
			| P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0)
			| P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ3)
			| P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ2)
			| P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ1)
			| P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ0));
		vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3)
			| P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2)
			| P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1)
			| P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0)
			| P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM3)
			| P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM2)
			| P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM1)
			| P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM0));
	}
	else
	{
		vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3)
			| P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2)
			| P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1)
			| P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0)
			| P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ3)
			| P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ2));
		vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3)
			| P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2)
			| P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1)
			| P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0)
			| P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM3)
			| P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM2));
	}
	vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ2, P_Fld(0x0, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ3)
		| P_Fld(0x0, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ2)
		| P_Fld(0x0, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ1)
		| P_Fld(0x0, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ0)
		| P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1DQ3)
		| P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1DQ2)
		| P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1DQ1)
		| P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1DQ0));
	vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ3, P_Fld(0x0, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM3)
		| P_Fld(0x0, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM2)
		| P_Fld(0x0, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM1)
		| P_Fld(0x0, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM0)
		| P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1DQM3)
		| P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1DQM2)
		| P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1DQM1)
		| P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1DQM0));
	vIO32WriteFldMulti(DRAMC_REG_SHU_DQSG_RETRY, P_Fld(0x0, SHU_DQSG_RETRY_R_DQSIENLAT)
		| P_Fld(0x0, SHU_DQSG_RETRY_R_DDR1866_PLUS));

	/* DDRPHY0-SHU2 */
	vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ5, 0x3, SHU1_B0_DQ5_RG_RX_ARDQS0_DVS_DLY_B0);
	/*
	 * vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ6, P_Fld(0x0, SHU1_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0)
	 * | P_Fld(0x1, SHU1_B0_DQ6_RG_ARPI_MIDPI_EN_B0));
	 */
	if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
	{
		vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7, P_Fld(0x0, SHU1_B0_DQ7_MIDPI_DIV4_ENABLE)
			| P_Fld(0x1, SHU1_B0_DQ7_MIDPI_ENABLE));
	}
	else
	{
		vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7, P_Fld(0x1, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0)
			| P_Fld(0x1, SHU1_B0_DQ7_R_DMDQMDBI_SHU_B0)
			| P_Fld(0x0, SHU1_B0_DQ7_MIDPI_DIV4_ENABLE)
			| P_Fld(0x1, SHU1_B0_DQ7_MIDPI_ENABLE));
	}
	vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5, 0x3, SHU1_B1_DQ5_RG_RX_ARDQS0_DVS_DLY_B1);
	/*
	 * vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ6, P_Fld(0x0, SHU1_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1)
	 * | P_Fld(0x1, SHU1_B1_DQ6_RG_ARPI_MIDPI_EN_B1));
	 */
	if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
	{
	}
	else
	{
		vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ7, P_Fld(0x1, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1)
			| P_Fld(0x1, SHU1_B1_DQ7_R_DMDQMDBI_SHU_B1));
	}
	/*
	 * DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
	 * vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD6, P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA)
	 * | P_Fld(0x1, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA));
	 * vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD6+SHIFT_TO_CHB_ADDR, P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA)
	 * | P_Fld(0x1, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA));
	 * DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
	 * vIO32WriteFldAlign(DDRPHY_SHU1_PLL5, 0x3300, SHU1_PLL5_RG_RPHYPLL_SDM_PCW);
	 * vIO32WriteFldAlign(DDRPHY_SHU1_PLL7, 0x3300, SHU1_PLL7_RG_RCLRPLL_SDM_PCW);
	 */
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ7, P_Fld(0x14, SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0)
		| P_Fld(0x14, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ7, P_Fld(0x14, SHU1_R0_B1_DQ7_RK0_ARPI_DQM_B1)
		| P_Fld(0x14, SHU1_R0_B1_DQ7_RK0_ARPI_DQ_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ7, P_Fld(0x14, SHU1_R1_B0_DQ7_RK1_ARPI_DQM_B0)
		| P_Fld(0x14, SHU1_R1_B0_DQ7_RK1_ARPI_DQ_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ7, P_Fld(0x14, SHU1_R1_B1_DQ7_RK1_ARPI_DQM_B1)
		| P_Fld(0x14, SHU1_R1_B1_DQ7_RK1_ARPI_DQ_B1));
#if LEGACY_RX_DLY
	LegacyRxDly_LP4_DDR2667(p);
#endif
#if LEGACY_DELAY_CELL
	LegacyDlyCellInitLP4_DDR2667(p);
#endif

}


static void DramcSetting_Olympus_LP4_ByteMode_DDR1600(DRAMC_CTX_T *p)
{
	/* DRAMC0-SHU3 */
#if LEGACY_TDQSCK_PRECAL
	LegacyPreCalLP4_DDR1600(p);
#endif
#if LEGACY_TX_TRACK
	LegacyTxTrackLP4_DDR1600(p);
#endif
	/* vIO32WriteFldAlign(DRAMC_REG_SHU_CONF2, 0x32, SHU_CONF2_FSPCHG_PRDCNT); //ACTiming related -> set in UpdateACTiming_Reg() */
	vIO32WriteFldMulti(DRAMC_REG_SHU_RANKCTL, P_Fld(0x2, SHU_RANKCTL_RANKINCTL_PHY)
		| P_Fld(0x0, SHU_RANKCTL_RANKINCTL_ROOT1)
		| P_Fld(0x0, SHU_RANKCTL_RANKINCTL));
	vIO32WriteFldMulti(DRAMC_REG_SHU_CKECTRL, P_Fld(0x3, SHU_CKECTRL_TCKESRX));
	/* | P_Fld(0x2, SHU_CKECTRL_CKEPRD)); */
	vIO32WriteFldMulti(DRAMC_REG_SHU_ODTCTRL, P_Fld(0x0, SHU_ODTCTRL_RODTE)
		| P_Fld(0x0, SHU_ODTCTRL_RODTE2)
	/* | P_Fld(0x2, SHU_ODTCTRL_RODT) //Set in UpdateACTimingReg() */
#ifdef LOOPBACK_TEST
		| P_Fld(0x1, SHU_ODTCTRL_ROEN));
#else
		| P_Fld(0x0, SHU_ODTCTRL_ROEN));
#endif
	vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ7, 0x0, SHU1_B0_DQ7_R_DMRODTEN_B0);
	vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ7, 0x0, SHU1_B1_DQ7_R_DMRODTEN_B1);
	vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS0, P_Fld(0x1, SHU_SELPH_DQS0_TXDLY_OEN_DQS3)
		| P_Fld(0x1, SHU_SELPH_DQS0_TXDLY_OEN_DQS2)
		| P_Fld(0x1, SHU_SELPH_DQS0_TXDLY_OEN_DQS1)
		| P_Fld(0x1, SHU_SELPH_DQS0_TXDLY_OEN_DQS0)
		| P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_DQS3)
		| P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_DQS2)
		| P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_DQS1)
		| P_Fld(0x2, SHU_SELPH_DQS0_TXDLY_DQS0));
	vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS1, P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS3)
		| P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS2)
		| P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS1)
		| P_Fld(0x6, SHU_SELPH_DQS1_DLY_OEN_DQS0)
		| P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS3)
		| P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS2)
		| P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS1)
		| P_Fld(0x1, SHU_SELPH_DQS1_DLY_DQS0));
	if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
	{
		vIO32WriteFldAlign(DRAMC_REG_SHU1_WODT, 0x1, SHU1_WODT_WPST2T);
	}
	else
	{
		vIO32WriteFldMulti(DRAMC_REG_SHU1_WODT, P_Fld(0x1, SHU1_WODT_WPST2T)
			| P_Fld(0x0, SHU1_WODT_DBIWR));
	}
	vIO32WriteFldAlign(DRAMC_REG_SHU_DQS2DQ_TX, 0x0, SHU_DQS2DQ_TX_OE2DQ_OFFSET);
	/*
	 * vIO32WriteFldAlign(DRAMC_REG_SHU_HWSET_MR2, 0x12, SHU_HWSET_MR2_HWSET_MR2_OP);
	 * vIO32WriteFldAlign(DRAMC_REG_SHU_HWSET_MR13, 0x8, SHU_HWSET_MR13_HWSET_MR13_OP);
	 * vIO32WriteFldAlign(DRAMC_REG_SHU_HWSET_VRCG, 0x0, SHU_HWSET_VRCG_HWSET_VRCG_OP);
	 */
	vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQSIEN, P_Fld(0x0, SHURK0_DQSIEN_R0DQS1IEN)
		| P_Fld(0x0, SHURK0_DQSIEN_R0DQS0IEN));
	vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_PI, P_Fld(0x1a, SHU1RK0_PI_RK0_ARPI_DQM_B1)
		| P_Fld(0x1a, SHU1RK0_PI_RK0_ARPI_DQM_B0)
		| P_Fld(0x1a, SHU1RK0_PI_RK0_ARPI_DQ_B1)
		| P_Fld(0x1a, SHU1RK0_PI_RK0_ARPI_DQ_B0));
	if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
	{
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3)
			| P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2)
			| P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1)
			| P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0)
			| P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ3)
			| P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ2)
			| P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ1)
			| P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ0));
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3)
			| P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2)
			| P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1)
			| P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0)
			| P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM3)
			| P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM2)
			| P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM1)
			| P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM0));
	}
	else
	{
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3)
			| P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2)
			| P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1)
			| P_Fld(0x1, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0)
			| P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ3)
			| P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ2));
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3)
			| P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2)
			| P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1)
			| P_Fld(0x1, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0)
			| P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM3)
			| P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM2));
	}
	vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ2, P_Fld(0x7, SHURK0_SELPH_DQ2_DLY_OEN_DQ3)
		| P_Fld(0x7, SHURK0_SELPH_DQ2_DLY_OEN_DQ2)
		| P_Fld(0x7, SHURK0_SELPH_DQ2_DLY_OEN_DQ1)
		| P_Fld(0x7, SHURK0_SELPH_DQ2_DLY_OEN_DQ0)
		| P_Fld(0x1, SHURK0_SELPH_DQ2_DLY_DQ3)
		| P_Fld(0x1, SHURK0_SELPH_DQ2_DLY_DQ2)
		| P_Fld(0x1, SHURK0_SELPH_DQ2_DLY_DQ1)
		| P_Fld(0x1, SHURK0_SELPH_DQ2_DLY_DQ0));
	vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ3, P_Fld(0x7, SHURK0_SELPH_DQ3_DLY_OEN_DQM3)
		| P_Fld(0x7, SHURK0_SELPH_DQ3_DLY_OEN_DQM2)
		| P_Fld(0x7, SHURK0_SELPH_DQ3_DLY_OEN_DQM1)
		| P_Fld(0x7, SHURK0_SELPH_DQ3_DLY_OEN_DQM0)
		| P_Fld(0x1, SHURK0_SELPH_DQ3_DLY_DQM3)
		| P_Fld(0x1, SHURK0_SELPH_DQ3_DLY_DQM2)
		| P_Fld(0x1, SHURK0_SELPH_DQ3_DLY_DQM1)
		| P_Fld(0x1, SHURK0_SELPH_DQ3_DLY_DQM0));
	/* vIO32WriteFldAlign(DRAMC_REG_SHURK1_DQSCTL, 0x2, SHURK1_DQSCTL_R1DQSINCTL); //Set in UpdateACTimingReg() */
	vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQSIEN, P_Fld(0x0, SHURK1_DQSIEN_R1DQS1IEN)
		| P_Fld(0x0, SHURK1_DQSIEN_R1DQS0IEN));
	vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_PI, P_Fld(0x1e, SHU1RK1_PI_RK1_ARPI_DQM_B1)
		| P_Fld(0x1e, SHU1RK1_PI_RK1_ARPI_DQM_B0)
		| P_Fld(0x1e, SHU1RK1_PI_RK1_ARPI_DQ_B1)
		| P_Fld(0x1e, SHU1RK1_PI_RK1_ARPI_DQ_B0));
#if LEGACY_GATING_DLY
	LegacyGatingDlyLP4_DDR1600(p);
#endif
	if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
	{
		vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3)
			| P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2)
			| P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1)
			| P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0)
			| P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ3)
			| P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ2)
			| P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ1)
			| P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ0));
		vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3)
			| P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2)
			| P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1)
			| P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0)
			| P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM3)
			| P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM2)
			| P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM1)
			| P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM0));
	}
	else
	{
		vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3)
			| P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2)
			| P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1)
			| P_Fld(0x1, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0)
			| P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ3)
			| P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ2));
		vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3)
			| P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2)
			| P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1)
			| P_Fld(0x1, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0)
			| P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM3)
			| P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM2));
	}
	vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ2, P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ3)
		| P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ2)
		| P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ1)
		| P_Fld(0x7, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ0)
		| P_Fld(0x1, SHURK1_SELPH_DQ2_DLY_R1DQ3)
		| P_Fld(0x1, SHURK1_SELPH_DQ2_DLY_R1DQ2)
		| P_Fld(0x1, SHURK1_SELPH_DQ2_DLY_R1DQ1)
		| P_Fld(0x1, SHURK1_SELPH_DQ2_DLY_R1DQ0));
	vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ3, P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM3)
		| P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM2)
		| P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM1)
		| P_Fld(0x7, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM0)
		| P_Fld(0x1, SHURK1_SELPH_DQ3_DLY_R1DQM3)
		| P_Fld(0x1, SHURK1_SELPH_DQ3_DLY_R1DQM2)
		| P_Fld(0x1, SHURK1_SELPH_DQ3_DLY_R1DQM1)
		| P_Fld(0x1, SHURK1_SELPH_DQ3_DLY_R1DQM0));
	vIO32WriteFldMulti(DRAMC_REG_SHU_DQSG_RETRY, P_Fld(0x0, SHU_DQSG_RETRY_R_DQSIENLAT)
		| P_Fld(0x0, SHU_DQSG_RETRY_R_DDR1866_PLUS));

	/* DDRPHY0-SHU3 */
	vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ5, 0x4, SHU1_B0_DQ5_RG_RX_ARDQS0_DVS_DLY_B0);
	/*
	 * vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ6, P_Fld(0x1, SHU1_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0)
	 * | P_Fld(0x0, SHU1_B0_DQ6_RG_ARPI_MIDPI_EN_B0));
	 */
	if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
	{
		vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7, P_Fld(0x1, SHU1_B0_DQ7_MIDPI_DIV4_ENABLE)
			| P_Fld(0x0, SHU1_B0_DQ7_MIDPI_ENABLE));
	}
	else
	{
		vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7, P_Fld(0x0, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0)
			| P_Fld(0x0, SHU1_B0_DQ7_R_DMDQMDBI_SHU_B0)
			| P_Fld(0x1, SHU1_B0_DQ7_MIDPI_DIV4_ENABLE)
			| P_Fld(0x0, SHU1_B0_DQ7_MIDPI_ENABLE));
	}
	vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5, 0x4, SHU1_B1_DQ5_RG_RX_ARDQS0_DVS_DLY_B1);
	/*
	 * vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ6, P_Fld(0x1, SHU1_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1)
	 * | P_Fld(0x0, SHU1_B1_DQ6_RG_ARPI_MIDPI_EN_B1));
	 */
	if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
	{}
	else
	{
		vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ7, P_Fld(0x0, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1)
			| P_Fld(0x0, SHU1_B1_DQ7_R_DMDQMDBI_SHU_B1));
	}
	/*
	 * DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
	 * vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD6, P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA)
	 * | P_Fld(0x1, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA));
	 * vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD6+SHIFT_TO_CHB_ADDR, P_Fld(0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA)
	 * | P_Fld(0x1, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA));
	 * DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
	 * vIO32WriteFldAlign(DDRPHY_SHU1_PLL5, 0x3d00, SHU1_PLL5_RG_RPHYPLL_SDM_PCW);
	 * vIO32WriteFldAlign(DDRPHY_SHU1_PLL7, 0x3d00, SHU1_PLL7_RG_RCLRPLL_SDM_PCW);
	 */
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ7, P_Fld(0x1a, SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0)
		| P_Fld(0x1a, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ7, P_Fld(0x1a, SHU1_R0_B1_DQ7_RK0_ARPI_DQM_B1)
		| P_Fld(0x1a, SHU1_R0_B1_DQ7_RK0_ARPI_DQ_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ7, P_Fld(0x26, SHU1_R1_B0_DQ7_RK1_ARPI_DQM_B0)
		| P_Fld(0x26, SHU1_R1_B0_DQ7_RK1_ARPI_DQ_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ7, P_Fld(0x26, SHU1_R1_B1_DQ7_RK1_ARPI_DQM_B1)
		| P_Fld(0x26, SHU1_R1_B1_DQ7_RK1_ARPI_DQ_B1));
#if LEGACY_RX_DLY
	LegacyRxDly_LP4_DDR1600(p);
#endif
#if LEGACY_DELAY_CELL
	LegacyDlyCellInitLP4_DDR1600(p);
#endif
}

/*
 * CANNOT use DRAMC_WBR :
 * DDRPHY_CA_DLL_ARPI0 -> DDRPHY_SHU1_CA_DLL1
 * DDRPHY_CA_DLL_ARPI3
 * DDRPHY_CA_DLL_ARPI5 -> DDRPHY_SHU1_CA_DLL0
 * DDRPHY_SHU1_CA_CMD6
 */

#if ENABLE_DUTY_SCAN_V2
#ifdef DDR_INIT_TIME_PROFILING
U32 gu4DutyCalibrationTime;
#endif
#endif

static void DramcSetting_Olympus_LP4_ByteMode(DRAMC_CTX_T *p)
{

	U8 u1CAP_SEL;
	U8 u1MIDPICAP_SEL;
	/* U16 u2SDM_PCW = 0; // SDM_PCW are set in DDRPhyPLLSetting() */
	U8 u1TXDLY_CMD;

	AutoRefreshCKEOff(p);

	DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
	/* before switch clock from 26M to PHY, need to init PHY clock first */
	vIO32WriteFldMulti_All(DDRPHY_CKMUX_SEL, P_Fld(0x1, CKMUX_SEL_R_PHYCTRLMUX)  /* move CKMUX_SEL_R_PHYCTRLMUX to here (it was originally between MISC_CG_CTRL0_CLK_MEM_SEL and MISC_CTRL0_R_DMRDSEL_DIV2_OPT) */
		| P_Fld(0x1, CKMUX_SEL_R_PHYCTRLDCM)); /* PHYCTRLDCM 1: follow DDRPHY_conf DCM settings, 0: follow infra DCM settings */
	DramcBroadcastOnOff(DRAMC_BROADCAST_ON);

	/* chg_mem_en = 1 */
	vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 0x1, MISC_CG_CTRL0_W_CHG_MEM);
	/* 26M */
	vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 0x0, MISC_CG_CTRL0_CLK_MEM_SEL);

	vIO32WriteFldAlign(DDRPHY_MISC_CTRL0, 0x0, MISC_CTRL0_R_DMRDSEL_DIV2_OPT);

	/*
	 * 0 ===LP4_3200_intial_setting_shu1 begin===
	 * Francis : pin mux issue, need to set CHD
	 * TODO: ARDMSUS_10 already set to 0 in SwimpedanceCal(), may be removed here?
	 */
	vIO32WriteFldMulti(DDRPHY_MISC_SPM_CTRL1, P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10) | P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_B0)
		| P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_B1) | P_Fld(0x0, MISC_SPM_CTRL1_RG_ARDMSUS_10_CA));

	vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL2, 0x0, MISC_SPM_CTRL2_PHY_SPM_CTL2);
	vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL0, 0x0, MISC_SPM_CTRL0_PHY_SPM_CTL0);
	vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL2, 0x6003af, MISC_CG_CTRL2_RG_MEM_DCM_CTL);
	vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL4, 0x333f3f00, MISC_CG_CTRL4_R_PHY_MCK_CG_CTRL);
	vIO32WriteFldMulti(DDRPHY_SHU1_PLL1, P_Fld(0x1, SHU1_PLL1_R_SHU_AUTO_PLL_MUX)
		| P_Fld(0x7, SHU1_PLL1_SHU1_PLL1_RFU));
	vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7, P_Fld(0x1, SHU1_B0_DQ7_MIDPI_ENABLE)
		| P_Fld(0x0, SHU1_B0_DQ7_MIDPI_DIV4_ENABLE)
		| P_Fld(0, SHU1_B0_DQ7_R_DMRANKRXDVS_B0));
	vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ7, 0, SHU1_B1_DQ7_R_DMRANKRXDVS_B1);
	/* vIO32WriteFldAlign(DDRPHY_SHU1_PLL4, 0xfe, SHU1_PLL4_RG_RPHYPLL_RESERVED); */
	vIO32WriteFldMulti(DDRPHY_SHU1_PLL4, P_Fld(0x1, SHU1_PLL4_RG_RPHYPLL_IBIAS) | P_Fld(0x1, SHU1_PLL4_RG_RPHYPLL_ICHP) | P_Fld(0x2, SHU1_PLL4_RG_RPHYPLL_FS));
	/* vIO32WriteFldAlign(DDRPHY_SHU1_PLL6, 0xfe, SHU1_PLL6_RG_RCLRPLL_RESERVED); */
	vIO32WriteFldMulti(DDRPHY_SHU1_PLL6, P_Fld(0x1, SHU1_PLL6_RG_RCLRPLL_IBIAS) | P_Fld(0x1, SHU1_PLL6_RG_RCLRPLL_ICHP) | P_Fld(0x2, SHU1_PLL6_RG_RCLRPLL_FS));
	vIO32WriteFldAlign(DDRPHY_SHU1_PLL14, 0x0, SHU1_PLL14_RG_RPHYPLL_SDM_SSC_PH_INIT);
	vIO32WriteFldAlign(DDRPHY_SHU1_PLL20, 0x0, SHU1_PLL20_RG_RCLRPLL_SDM_SSC_PH_INIT);
	vIO32WriteFldMulti(DDRPHY_CA_CMD2, P_Fld(0x0, CA_CMD2_RG_TX_ARCMD_OE_DIS)
		| P_Fld(0x0, CA_CMD2_RG_TX_ARCMD_ODTEN_DIS)
		| P_Fld(0x0, CA_CMD2_RG_TX_ARCLK_OE_DIS)
		| P_Fld(0x0, CA_CMD2_RG_TX_ARCLK_ODTEN_DIS));
	vIO32WriteFldMulti(DDRPHY_B0_DQ2, P_Fld(0x0, B0_DQ2_RG_TX_ARDQ_OE_DIS_B0)
		| P_Fld(0x0, B0_DQ2_RG_TX_ARDQ_ODTEN_DIS_B0)
		| P_Fld(0x0, B0_DQ2_RG_TX_ARDQS0_OE_DIS_B0)
		| P_Fld(0x0, B0_DQ2_RG_TX_ARDQS0_ODTEN_DIS_B0));
	vIO32WriteFldMulti(DDRPHY_B1_DQ2, P_Fld(0x0, B1_DQ2_RG_TX_ARDQ_OE_DIS_B1)
		| P_Fld(0x0, B1_DQ2_RG_TX_ARDQ_ODTEN_DIS_B1)
		| P_Fld(0x0, B1_DQ2_RG_TX_ARDQS0_OE_DIS_B1)
		| P_Fld(0x0, B1_DQ2_RG_TX_ARDQS0_ODTEN_DIS_B1));
#if 0 /* Correct settings are set in UpdateInitialSettings_LP4() */
	vIO32WriteFldAlign(DDRPHY_B0_DQ9, 0x0, B0_DQ9_R_IN_GATE_EN_LOW_OPT_B0);
	vIO32WriteFldAlign(DDRPHY_B1_DQ9, 0x7, B1_DQ9_R_IN_GATE_EN_LOW_OPT_B1);
	vIO32WriteFldAlign(DDRPHY_CA_CMD10, 0x0, CA_CMD10_R_IN_GATE_EN_LOW_OPT_CA);
#endif
	vIO32WriteFldAlign(DDRPHY_B0_DQ9, 0x1, B0_DQ9_R_DMRXDVS_RDSEL_LAT_B0);
	vIO32WriteFldAlign(DDRPHY_B1_DQ9, 0x1, B1_DQ9_R_DMRXDVS_RDSEL_LAT_B1);
	vIO32WriteFldAlign(DDRPHY_CA_CMD10, 0x0, CA_CMD10_R_DMRXDVS_RDSEL_LAT_CA);

	vIO32WriteFldAlign(DDRPHY_B0_RXDVS0, 0x1, B0_RXDVS0_R_RX_DLY_TRACK_CG_EN_B0);
	vIO32WriteFldAlign(DDRPHY_B1_RXDVS0, 0x1, B1_RXDVS0_R_RX_DLY_TRACK_CG_EN_B1);
	vIO32WriteFldAlign(DDRPHY_B0_RXDVS0, 0x1, B0_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_B0);
	vIO32WriteFldAlign(DDRPHY_B1_RXDVS0, 0x1, B1_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_B1);
	vIO32WriteFldAlign(DDRPHY_R0_B0_RXDVS2, 0x1, R0_B0_RXDVS2_R_RK0_DVS_FDLY_MODE_B0);
	vIO32WriteFldAlign(DDRPHY_R1_B0_RXDVS2, 0x1, R1_B0_RXDVS2_R_RK1_DVS_FDLY_MODE_B0);
	vIO32WriteFldAlign(DDRPHY_R0_B1_RXDVS2, 0x1, R0_B1_RXDVS2_R_RK0_DVS_FDLY_MODE_B1);
	vIO32WriteFldAlign(DDRPHY_R1_B1_RXDVS2, 0x1, R1_B1_RXDVS2_R_RK1_DVS_FDLY_MODE_B1);
	vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ5, 0x3, SHU1_B0_DQ5_RG_RX_ARDQS0_DVS_DLY_B0);
	vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ5, 0x3, SHU1_B1_DQ5_RG_RX_ARDQS0_DVS_DLY_B1);
#if LEGACY_RX_DLY
	LegacyRxDly_LP4_DDR3200(p);
#endif
	vIO32WriteFldMulti(DDRPHY_R0_B0_RXDVS1, P_Fld(0x2, R0_B0_RXDVS1_R_RK0_B0_DVS_TH_LEAD)
		| P_Fld(0x2, R0_B0_RXDVS1_R_RK0_B0_DVS_TH_LAG));
	vIO32WriteFldMulti(DDRPHY_R1_B0_RXDVS1, P_Fld(0x2, R1_B0_RXDVS1_R_RK1_B0_DVS_TH_LEAD)
		| P_Fld(0x2, R1_B0_RXDVS1_R_RK1_B0_DVS_TH_LAG));
	vIO32WriteFldMulti(DDRPHY_R0_B1_RXDVS1, P_Fld(0x2, R0_B1_RXDVS1_R_RK0_B1_DVS_TH_LEAD)
		| P_Fld(0x2, R0_B1_RXDVS1_R_RK0_B1_DVS_TH_LAG));
	vIO32WriteFldMulti(DDRPHY_R1_B1_RXDVS1, P_Fld(0x2, R1_B1_RXDVS1_R_RK1_B1_DVS_TH_LEAD)
		| P_Fld(0x2, R1_B1_RXDVS1_R_RK1_B1_DVS_TH_LAG));

	vIO32WriteFldMulti(DDRPHY_R0_B0_RXDVS2, P_Fld(0x2, R0_B0_RXDVS2_R_RK0_DVS_MODE_B0)
		| P_Fld(0x1, R0_B0_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B0)
		| P_Fld(0x1, R0_B0_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B0));
	vIO32WriteFldMulti(DDRPHY_R1_B0_RXDVS2, P_Fld(0x2, R1_B0_RXDVS2_R_RK1_DVS_MODE_B0)
		| P_Fld(0x1, R1_B0_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_B0)
		| P_Fld(0x1, R1_B0_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_B0));
	vIO32WriteFldMulti(DDRPHY_R0_B1_RXDVS2, P_Fld(0x2, R0_B1_RXDVS2_R_RK0_DVS_MODE_B1)
		| P_Fld(0x1, R0_B1_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B1)
		| P_Fld(0x1, R0_B1_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B1));
	vIO32WriteFldMulti(DDRPHY_R1_B1_RXDVS2, P_Fld(0x2, R1_B1_RXDVS2_R_RK1_DVS_MODE_B1)
		| P_Fld(0x1, R1_B1_RXDVS2_R_RK1_RX_DLY_RIS_TRACK_GATE_ENA_B1)
		| P_Fld(0x1, R1_B1_RXDVS2_R_RK1_RX_DLY_FAL_TRACK_GATE_ENA_B1));

	vIO32WriteFldAlign(DDRPHY_B0_RXDVS0, 0x0, B0_RXDVS0_R_RX_DLY_TRACK_CG_EN_B0);
	vIO32WriteFldAlign(DDRPHY_B1_RXDVS0, 0x0, B1_RXDVS0_R_RX_DLY_TRACK_CG_EN_B1);
	vIO32WriteFldAlign(DDRPHY_B0_DQ9, 0x1, B0_DQ9_RG_RX_ARDQ_STBEN_RESETB_B0);
	vIO32WriteFldAlign(DDRPHY_B1_DQ9, 0x1, B1_DQ9_RG_RX_ARDQ_STBEN_RESETB_B1);
#if LEGACY_DELAY_CELL
	LegacyDlyCellInitLP4_DDR3200(p);
#endif
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ7, P_Fld(0x1f, SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0)
		| P_Fld(0x1f, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ7, P_Fld(0x1f, SHU1_R0_B1_DQ7_RK0_ARPI_DQM_B1)
		| P_Fld(0x1f, SHU1_R0_B1_DQ7_RK0_ARPI_DQ_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ7, P_Fld(0x1f, SHU1_R1_B0_DQ7_RK1_ARPI_DQM_B0)
		| P_Fld(0x1f, SHU1_R1_B0_DQ7_RK1_ARPI_DQ_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ7, P_Fld(0x1f, SHU1_R1_B1_DQ7_RK1_ARPI_DQM_B1)
		| P_Fld(0x1f, SHU1_R1_B1_DQ7_RK1_ARPI_DQ_B1));
	vIO32WriteFldMulti(DDRPHY_B0_DQ4, P_Fld(0x10, B0_DQ4_RG_RX_ARDQS_EYE_R_DLY_B0)
		| P_Fld(0x10, B0_DQ4_RG_RX_ARDQS_EYE_F_DLY_B0));
	vIO32WriteFldMulti(DDRPHY_B0_DQ5, P_Fld(0x0, B0_DQ5_RG_RX_ARDQ_EYE_EN_B0)
		| P_Fld(0x1, B0_DQ5_RG_RX_ARDQ_EYE_SEL_B0)
		| P_Fld(0x1, B0_DQ5_RG_RX_ARDQ_VREF_EN_B0)
		| P_Fld(0xe, B0_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B0)
		| P_Fld(0x10, B0_DQ5_B0_DQ5_RFU));
	vIO32WriteFldMulti(DDRPHY_B0_DQ6, P_Fld(0x1, B0_DQ6_RG_RX_ARDQ_EYE_DLY_DQS_BYPASS_B0)
		| P_Fld(0x0, B0_DQ6_RG_TX_ARDQ_DDR3_SEL_B0)
		| P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_DDR3_SEL_B0)
		| P_Fld(0x1, B0_DQ6_RG_TX_ARDQ_DDR4_SEL_B0)
		| P_Fld(0x1, B0_DQ6_RG_RX_ARDQ_DDR4_SEL_B0)
		| P_Fld(0x0, B0_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B0)
		| P_Fld(0x1, B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0)
		| P_Fld(0x1, B0_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B0)
		| P_Fld(0x0, B0_DQ6_RG_TX_ARDQ_SER_MODE_B0));
	vIO32WriteFldMulti(DDRPHY_B0_DQ5, P_Fld(0x1, B0_DQ5_RG_RX_ARDQ_EYE_STBEN_RESETB_B0)
		| P_Fld(0x0, B0_DQ5_B0_DQ5_RFU));
	vIO32WriteFldMulti(DDRPHY_B1_DQ4, P_Fld(0x10, B1_DQ4_RG_RX_ARDQS_EYE_R_DLY_B1)
		| P_Fld(0x10, B1_DQ4_RG_RX_ARDQS_EYE_F_DLY_B1));
	vIO32WriteFldMulti(DDRPHY_B1_DQ5, P_Fld(0x0, B1_DQ5_RG_RX_ARDQ_EYE_EN_B1)
		| P_Fld(0x1, B1_DQ5_RG_RX_ARDQ_EYE_SEL_B1)
		| P_Fld(0x1, B1_DQ5_RG_RX_ARDQ_VREF_EN_B1)
		| P_Fld(0xe, B1_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B1)
		| P_Fld(0x10, B1_DQ5_B1_DQ5_RFU));
	vIO32WriteFldMulti(DDRPHY_B1_DQ6, P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_EYE_DLY_DQS_BYPASS_B1)
		| P_Fld(0x0, B1_DQ6_RG_TX_ARDQ_DDR3_SEL_B1)
		| P_Fld(0x0, B1_DQ6_RG_RX_ARDQ_DDR3_SEL_B1)
		| P_Fld(0x1, B1_DQ6_RG_TX_ARDQ_DDR4_SEL_B1)
		| P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_DDR4_SEL_B1)
		| P_Fld(0x0, B1_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B1)
		| P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1)
		| P_Fld(0x1, B1_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B1)
		| P_Fld(0x0, B1_DQ6_RG_TX_ARDQ_SER_MODE_B1));
	vIO32WriteFldMulti(DDRPHY_B1_DQ5, P_Fld(0x1, B1_DQ5_RG_RX_ARDQ_EYE_STBEN_RESETB_B1)
		| P_Fld(0x0, B1_DQ5_B1_DQ5_RFU));
	vIO32WriteFldMulti(DDRPHY_CA_CMD3, P_Fld(0x1, CA_CMD3_RG_RX_ARCMD_IN_BUFF_EN)
		| P_Fld(0x1, CA_CMD3_RG_ARCMD_RESETB)
		| P_Fld(0x1, CA_CMD3_RG_TX_ARCMD_EN));
	vIO32WriteFldMulti(DDRPHY_CA_CMD6, P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_DDR4_SEL)
		| P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_BIAS_VREF_SEL)
		| P_Fld(0x0, CA_CMD6_RG_RX_ARCMD_RES_BIAS_EN));
	/*
	 * ARCMD_DRVP, DRVN , ARCLK_DRVP, DRVN not used anymore
	 * vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD1, P_Fld(0x1, SHU1_CA_CMD1_RG_TX_ARCMD_DRVN)
	 * | P_Fld(0x1, SHU1_CA_CMD1_RG_TX_ARCMD_DRVP));
	 * vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD2, P_Fld(0x1, SHU1_CA_CMD2_RG_TX_ARCLK_DRVN)
	 * | P_Fld(0x1, SHU1_CA_CMD2_RG_TX_ARCLK_DRVP));
	 */
	/*
	 * vIO32WriteFldMulti(DDRPHY_SHU2_CA_CMD1, P_Fld(0x1, SHU2_CA_CMD1_RG_TX_ARCMD_DRVN)
	 * | P_Fld(0x1, SHU2_CA_CMD1_RG_TX_ARCMD_DRVP));
	 * vIO32WriteFldMulti(DDRPHY_SHU2_CA_CMD2, P_Fld(0x1, SHU2_CA_CMD2_RG_TX_ARCLK_DRVN)
	 * | P_Fld(0x1, SHU2_CA_CMD2_RG_TX_ARCLK_DRVP));
	 */
	vIO32WriteFldMulti(DDRPHY_PLL3, P_Fld(0x0, PLL3_RG_RPHYPLL_TSTOP_EN) | P_Fld(0x0, PLL3_RG_RPHYPLL_TST_EN));
	vIO32WriteFldAlign(DDRPHY_MISC_VREF_CTRL, 0x0, MISC_VREF_CTRL_RG_RVREF_VREF_EN); /* LP3 VREF */

	vIO32WriteFldAlign(DDRPHY_B0_DQ3, 0x1, B0_DQ3_RG_ARDQ_RESETB_B0);
	vIO32WriteFldAlign(DDRPHY_B1_DQ3, 0x1, B1_DQ3_RG_ARDQ_RESETB_B1);

	mcDELAY_US(1);

	/* Ref clock should be 20M~30M, if MPLL=52M, Pre-divider should be set to 1 */
	vIO32WriteFldMulti(DDRPHY_SHU1_PLL8, P_Fld(0x0, SHU1_PLL8_RG_RPHYPLL_POSDIV) | P_Fld(0x1, SHU1_PLL8_RG_RPHYPLL_PREDIV));
	/*
	 * vIO32WriteFldAlign(DDRPHY_SHU2_PLL8, 0x0, SHU2_PLL8_RG_RPHYPLL_POSDIV);
	 * vIO32WriteFldAlign(DDRPHY_SHU3_PLL8, 0x0, SHU3_PLL8_RG_RPHYPLL_POSDIV);
	 * vIO32WriteFldAlign(DDRPHY_SHU4_PLL8, 0x0, SHU4_PLL8_RG_RPHYPLL_POSDIV);
	 */

	mcDELAY_US(1);

	vIO32WriteFldMulti(DDRPHY_SHU1_PLL9, P_Fld(0x0, SHU1_PLL9_RG_RPHYPLL_MONCK_EN)
		| P_Fld(0x0, SHU1_PLL9_RG_RPHYPLL_MONVC_EN)
		| P_Fld(0x0, SHU1_PLL9_RG_RPHYPLL_LVROD_EN)
		| P_Fld(0x0, SHU1_PLL9_RG_RPHYPLL_RST_DLY));
	vIO32WriteFldMulti(DDRPHY_SHU1_PLL11, P_Fld(0x0, SHU1_PLL11_RG_RCLRPLL_MONCK_EN)
		| P_Fld(0x0, SHU1_PLL11_RG_RCLRPLL_MONVC_EN)
		| P_Fld(0x0, SHU1_PLL11_RG_RCLRPLL_LVROD_EN)
		| P_Fld(0x0, SHU1_PLL11_RG_RCLRPLL_RST_DLY));

	mcDELAY_US(1);

	/* Ref clock should be 20M~30M, if MPLL=52M, Pre-divider should be set to 1 */
	vIO32WriteFldMulti(DDRPHY_SHU1_PLL10, P_Fld(0x0, SHU1_PLL10_RG_RCLRPLL_POSDIV) | P_Fld(0x1, SHU1_PLL10_RG_RCLRPLL_PREDIV));
	/*
	 * vIO32WriteFldAlign(DDRPHY_SHU2_PLL10, 0x0, SHU2_PLL10_RG_RCLRPLL_POSDIV);
	 * vIO32WriteFldAlign(DDRPHY_SHU3_PLL10, 0x0, SHU3_PLL10_RG_RCLRPLL_POSDIV);
	 * vIO32WriteFldAlign(DDRPHY_SHU4_PLL10, 0x0, SHU4_PLL10_RG_RCLRPLL_POSDIV);
	 */

	mcDELAY_US(1);


	/* TODO: MIDPI Init 1 */
	vIO32WriteFldMulti(DDRPHY_PLL4, P_Fld(0x0, PLL4_RG_RPHYPLL_AD_MCK8X_EN)
		| P_Fld(0x1, PLL4_PLL4_RFU)
		| P_Fld(0x1, PLL4_RG_RPHYPLL_MCK8X_SEL));


	mcDELAY_US(1);

	vIO32WriteFldAlign(DDRPHY_SHU1_PLL0, 0x3, SHU1_PLL0_RG_RPHYPLL_TOP_REV); /* debug1111, org:3 -> mdf:0 */
	/*
	 * vIO32WriteFldAlign(DDRPHY_SHU2_PLL0, 0x3, SHU2_PLL0_RG_RPHYPLL_TOP_REV);
	 * vIO32WriteFldAlign(DDRPHY_SHU3_PLL0, 0x3, SHU3_PLL0_RG_RPHYPLL_TOP_REV);
	 * vIO32WriteFldAlign(DDRPHY_SHU4_PLL0, 0x3, SHU4_PLL0_RG_RPHYPLL_TOP_REV);
	 */

	mcDELAY_US(1);


	vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI1, 0x1, CA_DLL_ARPI1_RG_ARPISM_MCK_SEL_CA);
	vIO32WriteFldMulti(DDRPHY_B0_DQ3, P_Fld(0x1, B0_DQ3_RG_RX_ARDQ_STBENCMP_EN_B0)
		| P_Fld(0x1, B0_DQ3_RG_TX_ARDQ_EN_B0)
		| P_Fld(0x1, B0_DQ3_RG_RX_ARDQ_SMT_EN_B0));
	vIO32WriteFldMulti(DDRPHY_B1_DQ3, P_Fld(0x1, B1_DQ3_RG_RX_ARDQ_STBENCMP_EN_B1)
		| P_Fld(0x1, B1_DQ3_RG_TX_ARDQ_EN_B1)
		| P_Fld(0x1, B1_DQ3_RG_RX_ARDQ_SMT_EN_B1));



#if (fcFOR_CHIP_ID == fcTalbot)
	DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
#if 1/* #ifndef BIANCO_TO_BE_PORTING */
	vIO32WriteFldAlign(DDRPHY_SHU1_CA_DLL0, 0x1, SHU1_CA_DLL0_RG_ARPISM_MCK_SEL_CA_SHU);
	vIO32WriteFldAlign(DDRPHY_SHU1_CA_DLL0 + SHIFT_TO_CHB_ADDR, 0x1, SHU1_CA_DLL0_RG_ARPISM_MCK_SEL_CA_SHU);
#endif
	DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
#endif

	vIO32WriteFldMulti(DDRPHY_SHU1_B0_DLL0, P_Fld(0x1, SHU1_B0_DLL0_RG_ARDLL_PHDET_IN_SWAP_B0)
		| P_Fld(0x7, SHU1_B0_DLL0_RG_ARDLL_GAIN_B0)
		| P_Fld(0x7, SHU1_B0_DLL0_RG_ARDLL_IDLECNT_B0)
		| P_Fld(0x8, SHU1_B0_DLL0_RG_ARDLL_P_GAIN_B0)
		| P_Fld(0x1, SHU1_B0_DLL0_RG_ARDLL_PHJUMP_EN_B0)
		| P_Fld(0x1, SHU1_B0_DLL0_RG_ARDLL_PHDIV_B0)
		| P_Fld(0x0, SHU1_B0_DLL0_RG_ARDLL_FAST_PSJP_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_B1_DLL0, P_Fld(0x1, SHU1_B1_DLL0_RG_ARDLL_PHDET_IN_SWAP_B1)
		| P_Fld(0x7, SHU1_B1_DLL0_RG_ARDLL_GAIN_B1)
		| P_Fld(0x7, SHU1_B1_DLL0_RG_ARDLL_IDLECNT_B1)
		| P_Fld(0x8, SHU1_B1_DLL0_RG_ARDLL_P_GAIN_B1)
		| P_Fld(0x1, SHU1_B1_DLL0_RG_ARDLL_PHJUMP_EN_B1)
		| P_Fld(0x1, SHU1_B1_DLL0_RG_ARDLL_PHDIV_B1)
		| P_Fld(0x0, SHU1_B1_DLL0_RG_ARDLL_FAST_PSJP_B1));

	vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD5, 0x0, SHU1_CA_CMD5_RG_RX_ARCMD_VREF_SEL);
	/*
	 * vIO32WriteFldAlign(DDRPHY_SHU2_CA_CMD5, 0x0, SHU2_CA_CMD5_RG_RX_ARCMD_VREF_SEL);
	 * vIO32WriteFldAlign(DDRPHY_SHU3_CA_CMD5, 0x0, SHU3_CA_CMD5_RG_RX_ARCMD_VREF_SEL);
	 * vIO32WriteFldAlign(DDRPHY_SHU4_CA_CMD5, 0x0, SHU4_CA_CMD5_RG_RX_ARCMD_VREF_SEL);
	 */
	vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD0, P_Fld(0x1, SHU1_CA_CMD0_RG_TX_ARCMD_PRE_EN)
		| P_Fld(0x4, SHU1_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
		| P_Fld(0x1, SHU1_CA_CMD0_RG_TX_ARCLK_PRE_EN));
	/*
	 * vIO32WriteFldMulti(DDRPHY_SHU2_CA_CMD0, P_Fld(0x1, SHU2_CA_CMD0_RG_TX_ARCMD_PRE_EN)
	 * | P_Fld(0x4, SHU2_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
	 * | P_Fld(0x1, SHU2_CA_CMD0_RG_TX_ARCLK_PRE_EN));
	 * vIO32WriteFldMulti(DDRPHY_SHU3_CA_CMD0, P_Fld(0x1, SHU3_CA_CMD0_RG_TX_ARCMD_PRE_EN)
	 * | P_Fld(0x4, SHU3_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
	 * | P_Fld(0x1, SHU3_CA_CMD0_RG_TX_ARCLK_PRE_EN));
	 * vIO32WriteFldMulti(DDRPHY_SHU4_CA_CMD0, P_Fld(0x1, SHU4_CA_CMD0_RG_TX_ARCMD_PRE_EN)
	 * | P_Fld(0x4, SHU4_CA_CMD0_RG_TX_ARCLK_DRVN_PRE)
	 * | P_Fld(0x1, SHU4_CA_CMD0_RG_TX_ARCLK_PRE_EN));
	 */
#if (fcFOR_CHIP_ID == fcTalbot)
	DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
	vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD6, 0x3, SHU1_CA_CMD6_RG_ARPI_RESERVE_CA);
	vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD6 + SHIFT_TO_CHB_ADDR, 0x1, SHU1_CA_CMD6_RG_ARPI_RESERVE_CA);
	DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
#else
	vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD6, 0x3, SHU1_CA_CMD6_RG_ARPI_RESERVE_CA);
#endif
	/*
	 * vIO32WriteFldAlign(DDRPHY_SHU2_CA_CMD6, 0x3, SHU2_CA_CMD6_RG_ARPI_RESERVE_CA);
	 * vIO32WriteFldAlign(DDRPHY_SHU3_CA_CMD6, 0x3, SHU3_CA_CMD6_RG_ARPI_RESERVE_CA);
	 * vIO32WriteFldAlign(DDRPHY_SHU4_CA_CMD6, 0x3, SHU4_CA_CMD6_RG_ARPI_RESERVE_CA);
	 * vIO32WriteFldAlign(DDRPHY_SHU1_CA_CMD3, 0x4e1, SHU1_CA_CMD3_RG_ARCMD_REV);
	 * vIO32WriteFldAlign(DDRPHY_SHU2_CA_CMD7, 0x4e1, SHU2_CA_CMD7_RG_ARCMD_REV);
	 * vIO32WriteFldAlign(DDRPHY_SHU3_CA_CMD7, 0x4e1, SHU3_CA_CMD7_RG_ARCMD_REV);
	 * vIO32WriteFldAlign(DDRPHY_SHU4_CA_CMD7, 0x4e1, SHU4_CA_CMD7_RG_ARCMD_REV);
	 * vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7, P_Fld(0x00, SHU1_B0_DQ7_RG_ARDQ_REV_B0)
	 * | P_Fld(0x0, SHU1_B0_DQ7_DQ_REV_B0_BIT_05));
	 * vIO32WriteFldAlign(DDRPHY_SHU2_B0_DQ7, 0x20, SHU2_B0_DQ7_RG_ARDQ_REV_B0);
	 * vIO32WriteFldAlign(DDRPHY_SHU3_B0_DQ7, 0x20, SHU3_B0_DQ7_RG_ARDQ_REV_B0);
	 * vIO32WriteFldAlign(DDRPHY_SHU4_B0_DQ7, 0x20, SHU4_B0_DQ7_RG_ARDQ_REV_B0);
	 * vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ7, P_Fld(0x00, SHU1_B1_DQ7_RG_ARDQ_REV_B1)
	 * | P_Fld(0x0, SHU1_B1_DQ7_DQ_REV_B1_BIT_05));
	 */

	/*
	 * lynx added
	 * vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ7, 0x0, SHU1_B0_DQ7_RG_ARDQ_REV_B0);
	 * vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ7, 0x0, SHU1_B1_DQ7_RG_ARDQ_REV_B1);
	 *
	 * vIO32WriteFldAlign(DDRPHY_SHU2_B1_DQ7, 0x20, SHU2_B1_DQ7_RG_ARDQ_REV_B1);
	 * vIO32WriteFldAlign(DDRPHY_SHU3_B1_DQ7, 0x20, SHU3_B1_DQ7_RG_ARDQ_REV_B1);
	 * vIO32WriteFldAlign(DDRPHY_SHU4_B1_DQ7, 0x20, SHU4_B1_DQ7_RG_ARDQ_REV_B1);
	 */
	vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ6, 0x1, SHU1_B0_DQ6_RG_ARPI_RESERVE_B0);
	/*
	 * vIO32WriteFldAlign(DDRPHY_SHU2_B0_DQ6, 0x1, SHU2_B0_DQ6_RG_ARPI_RESERVE_B0);
	 * vIO32WriteFldAlign(DDRPHY_SHU3_B0_DQ6, 0x1, SHU3_B0_DQ6_RG_ARPI_RESERVE_B0);
	 * vIO32WriteFldAlign(DDRPHY_SHU4_B0_DQ6, 0x1, SHU4_B0_DQ6_RG_ARPI_RESERVE_B0);
	 */
	vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ6, 0x1, SHU1_B1_DQ6_RG_ARPI_RESERVE_B1);
	/*
	 * vIO32WriteFldAlign(DDRPHY_SHU2_B1_DQ6, 0x1, SHU2_B1_DQ6_RG_ARPI_RESERVE_B1);
	 * vIO32WriteFldAlign(DDRPHY_SHU3_B1_DQ6, 0x1, SHU3_B1_DQ6_RG_ARPI_RESERVE_B1);
	 * vIO32WriteFldAlign(DDRPHY_SHU4_B1_DQ6, 0x1, SHU4_B1_DQ6_RG_ARPI_RESERVE_B1);
	 */
	DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
	vIO32WriteFldMulti(DDRPHY_MISC_SHU_OPT, P_Fld(0x1, MISC_SHU_OPT_R_CA_SHU_PHDET_SPM_EN)
		| P_Fld(0x1, MISC_SHU_OPT_R_CA_SHU_PHY_GATING_RESETB_SPM_EN)
		| P_Fld(0x2, MISC_SHU_OPT_R_DQB1_SHU_PHDET_SPM_EN)
		| P_Fld(0x1, MISC_SHU_OPT_R_DQB1_SHU_PHY_GATING_RESETB_SPM_EN)
		| P_Fld(0x2, MISC_SHU_OPT_R_DQB0_SHU_PHDET_SPM_EN)
		| P_Fld(0x1, MISC_SHU_OPT_R_DQB0_SHU_PHY_GATING_RESETB_SPM_EN));
	vIO32WriteFldMulti(DDRPHY_MISC_SHU_OPT + SHIFT_TO_CHB_ADDR, P_Fld(0x2, MISC_SHU_OPT_R_CA_SHU_PHDET_SPM_EN)
		| P_Fld(0x1, MISC_SHU_OPT_R_CA_SHU_PHY_GATING_RESETB_SPM_EN)
		| P_Fld(0x2, MISC_SHU_OPT_R_DQB1_SHU_PHDET_SPM_EN)
		| P_Fld(0x1, MISC_SHU_OPT_R_DQB1_SHU_PHY_GATING_RESETB_SPM_EN)
		| P_Fld(0x2, MISC_SHU_OPT_R_DQB0_SHU_PHDET_SPM_EN)
		| P_Fld(0x1, MISC_SHU_OPT_R_DQB0_SHU_PHY_GATING_RESETB_SPM_EN));
	DramcBroadcastOnOff(DRAMC_BROADCAST_ON);

	mcDELAY_US(9);

#if (fcFOR_CHIP_ID == fcTalbot)
	DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
	vIO32WriteFldMulti(DDRPHY_SHU1_CA_DLL1, P_Fld(0x1, SHU1_CA_DLL1_RG_ARDLL_PD_CK_SEL_CA) | P_Fld(0x0, SHU1_CA_DLL1_RG_ARDLL_FASTPJ_CK_SEL_CA));
	vIO32WriteFldMulti(DDRPHY_SHU1_CA_DLL1 + SHIFT_TO_CHB_ADDR, P_Fld(0x0, SHU1_CA_DLL1_RG_ARDLL_PD_CK_SEL_CA)
		| P_Fld(0x1, SHU1_CA_DLL1_RG_ARDLL_FASTPJ_CK_SEL_CA)); /* CH_B CA slave mode */
	DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
#endif
	vIO32WriteFldMulti(DDRPHY_SHU1_B0_DLL1, P_Fld(0x0, SHU1_B0_DLL1_RG_ARDLL_PD_CK_SEL_B0) | P_Fld(0x1, SHU1_B0_DLL1_RG_ARDLL_FASTPJ_CK_SEL_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_B1_DLL1, P_Fld(0x0, SHU1_B1_DLL1_RG_ARDLL_PD_CK_SEL_B1) | P_Fld(0x1, SHU1_B1_DLL1_RG_ARDLL_FASTPJ_CK_SEL_B1));

	mcDELAY_US(1);

	vIO32WriteFldAlign(DDRPHY_PLL2, 0x0, PLL2_RG_RCLRPLL_EN);
	/*
	 * vIO32WriteFldAlign(DDRPHY_SHU1_PLL4, 0xff, SHU1_PLL4_RG_RPHYPLL_RESERVED);
	 * vIO32WriteFldAlign(DDRPHY_SHU1_PLL6, 0xff, SHU1_PLL6_RG_RCLRPLL_RESERVED);
	 */
	vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 0xf, MISC_CG_CTRL0_CLK_MEM_DFS_CFG);

	mcDELAY_US(1);

	DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
	DDRPhyReservedRGSetting(p);
	DDRPhyPLLSetting(p);
	DramcBroadcastOnOff(DRAMC_BROADCAST_ON);

	/* rollback tMRRI design change */
#if ENABLE_TMRRI_NEW_MODE
	/* fix rank at 0 to trigger new TMRRI setting */
	vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0, RKCFG_TXRANK);
	vIO32WriteFldAlign(DRAMC_REG_RKCFG, 1, RKCFG_TXRANKFIX);
	vIO32WriteFldAlign(DRAMC_REG_DRSCTRL, 0x0, DRSCTRL_RK_SCINPUT_OPT); /* new mode, HW_MRR: R_DMMRRRK, SW_MRR: R_DMMRSRK */
	vIO32WriteFldMulti(DRAMC_REG_DRAMCTRL, P_Fld(0x0, DRAMCTRL_MRRIOPT) | P_Fld(0x0, DRAMCTRL_TMRRIBYRK_DIS) | P_Fld(0x1, DRAMCTRL_TMRRICHKDIS));
	vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x1, SPCMDCTRL_SC_PG_UPD_OPT);
	vIO32WriteFldMulti(DRAMC_REG_SPCMDCTRL, P_Fld(0x0, SPCMDCTRL_SC_PG_MPRW_DIS) | P_Fld(0x0, SPCMDCTRL_SC_PG_STCMD_AREF_DIS)
		| P_Fld(0x0, SPCMDCTRL_SC_PG_OPT2_DIS) | P_Fld(0x0, SPCMDCTRL_SC_PG_MAN_DIS));
	vIO32WriteFldMulti(DRAMC_REG_MPC_OPTION, P_Fld(0x1, MPC_OPTION_ZQ_BLOCKALE_OPT) | P_Fld(0x1, MPC_OPTION_MPC_BLOCKALE_OPT2) |
		P_Fld(0x1, MPC_OPTION_MPC_BLOCKALE_OPT1) | P_Fld(0x1, MPC_OPTION_MPC_BLOCKALE_OPT));
	/* fix rank at 0 to trigger new TMRRI setting */
	vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0, RKCFG_TXRANK);
	vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0, RKCFG_TXRANKFIX);
#else
	vIO32WriteFldAlign(DRAMC_REG_DRSCTRL, 0x1, DRSCTRL_RK_SCINPUT_OPT); /* old mode, HW / SW MRR: R_DMMRRRK */
	vIO32WriteFldMulti(DRAMC_REG_DRAMCTRL, P_Fld(0x1, DRAMCTRL_MRRIOPT) | P_Fld(0x1, DRAMCTRL_TMRRIBYRK_DIS) | P_Fld(0x0, DRAMCTRL_TMRRICHKDIS));
	vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x0, SPCMDCTRL_SC_PG_UPD_OPT);
	vIO32WriteFldMulti(DRAMC_REG_SPCMDCTRL, P_Fld(0x1, SPCMDCTRL_SC_PG_MPRW_DIS) | P_Fld(0x1, SPCMDCTRL_SC_PG_STCMD_AREF_DIS)
		| P_Fld(0x1, SPCMDCTRL_SC_PG_OPT2_DIS) | P_Fld(0x1, SPCMDCTRL_SC_PG_MAN_DIS));
#endif
	vIO32WriteFldAlign(DRAMC_REG_CKECTRL, 0x1, CKECTRL_RUNTIMEMRRCKEFIX); /* Set Run time MRR CKE fix to 1 in tMRRI old mode to avoid no ACK from precharge all */
	vIO32WriteFldAlign(DRAMC_REG_CKECTRL, 0x0, CKECTRL_RUNTIMEMRRMIODIS);

	vIO32WriteFldAlign(DDRPHY_B0_DQ9, 0x1, B0_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B0);
	vIO32WriteFldAlign(DDRPHY_B1_DQ9, 0x1, B1_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B1);
	vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQSIEN, P_Fld(0xf, SHURK1_DQSIEN_R1DQS3IEN)
		| P_Fld(0xf, SHURK1_DQSIEN_R1DQS2IEN)
		| P_Fld(0xf, SHURK1_DQSIEN_R1DQS1IEN)
		| P_Fld(0xf, SHURK1_DQSIEN_R1DQS0IEN));
	vIO32WriteFldMulti(DRAMC_REG_STBCAL1, P_Fld(0x0, STBCAL1_DLLFRZ_MON_PBREF_OPT)
		| P_Fld(0x1, STBCAL1_STB_FLAGCLR)
		| P_Fld(0x1, STBCAL1_STBCNT_MODESEL));
	vIO32WriteFldMulti(DRAMC_REG_SHU_DQSG_RETRY, P_Fld(0x0, SHU_DQSG_RETRY_R_RETRY_USE_BURST_MDOE)
		| P_Fld(0x0, SHU_DQSG_RETRY_R_RDY_SEL_DLE)
		| P_Fld(0x0, SHU_DQSG_RETRY_R_DQSIENLAT)
		| P_Fld(0x0, SHU_DQSG_RETRY_R_RETRY_ONCE));
	vIO32WriteFldMulti(DRAMC_REG_SHU1_DRVING1, P_Fld(0xa, SHU1_DRVING1_DQSDRVP2) | P_Fld(0xa, SHU1_DRVING1_DQSDRVN2)
		| P_Fld(0xa, SHU1_DRVING1_DQSDRVP1) | P_Fld(0xa, SHU1_DRVING1_DQSDRVN1)
		| P_Fld(0xa, SHU1_DRVING1_DQDRVP2) | P_Fld(0xa, SHU1_DRVING1_DQDRVN2));
	vIO32WriteFldMulti(DRAMC_REG_SHU1_DRVING2, P_Fld(0xa, SHU1_DRVING2_DQDRVP1) | P_Fld(0xa, SHU1_DRVING2_DQDRVN1)
		| P_Fld(0xa, SHU1_DRVING2_CMDDRVP2) | P_Fld(0xa, SHU1_DRVING2_CMDDRVN2)
		| P_Fld(0xa, SHU1_DRVING2_CMDDRVP1) | P_Fld(0xa, SHU1_DRVING2_CMDDRVN1));
	vIO32WriteFldMulti(DRAMC_REG_SHU1_DRVING3, P_Fld(0xa, SHU1_DRVING3_DQSODTP2) | P_Fld(0xa, SHU1_DRVING3_DQSODTN2)
		| P_Fld(0xa, SHU1_DRVING3_DQSODTP) | P_Fld(0xa, SHU1_DRVING3_DQSODTN)
		| P_Fld(0xa, SHU1_DRVING3_DQODTP2) | P_Fld(0xa, SHU1_DRVING3_DQODTN2));
	vIO32WriteFldMulti(DRAMC_REG_SHU1_DRVING4, P_Fld(0xa, SHU1_DRVING4_DQODTP1) | P_Fld(0xa, SHU1_DRVING4_DQODTN1)
		| P_Fld(0xa, SHU1_DRVING4_CMDODTP2) | P_Fld(0xa, SHU1_DRVING4_CMDODTN2)
		| P_Fld(0xa, SHU1_DRVING4_CMDODTP1) | P_Fld(0xa, SHU1_DRVING4_CMDODTN1));
	/*
	 * vIO32WriteFldMulti(DRAMC_REG_SHU2_DRVING1, P_Fld(0x14a, SHU2_DRVING1_DQSDRV2)
	 * | P_Fld(0x14a, SHU2_DRVING1_DQSDRV1)
	 * | P_Fld(0x14a, SHU2_DRVING1_DQDRV2));
	 * vIO32WriteFldMulti(DRAMC_REG_SHU2_DRVING2, P_Fld(0x14a, SHU2_DRVING2_DQDRV1)
	 * | P_Fld(0x14a, SHU2_DRVING2_CMDDRV2)
	 * | P_Fld(0x14a, SHU2_DRVING2_CMDDRV1));
	 * vIO32WriteFldMulti(DRAMC_REG_SHU2_DRVING3, P_Fld(0x14a, SHU2_DRVING3_DQSODT2)
	 * | P_Fld(0x14a, SHU2_DRVING3_DQSODT1)
	 * | P_Fld(0x14a, SHU2_DRVING3_DQODT2));
	 * vIO32WriteFldMulti(DRAMC_REG_SHU2_DRVING4, P_Fld(0x14a, SHU2_DRVING4_DQODT1)
	 * | P_Fld(0x14a, SHU2_DRVING4_CMDODT2)
	 * | P_Fld(0x14a, SHU2_DRVING4_CMDODT1));
	 * vIO32WriteFldMulti(DRAMC_REG_SHU3_DRVING1, P_Fld(0x14a, SHU3_DRVING1_DQSDRV2)
	 * | P_Fld(0x14a, SHU3_DRVING1_DQSDRV1)
	 * | P_Fld(0x14a, SHU3_DRVING1_DQDRV2));
	 * vIO32WriteFldMulti(DRAMC_REG_SHU3_DRVING2, P_Fld(0x14a, SHU3_DRVING2_DQDRV1)
	 * | P_Fld(0x14a, SHU3_DRVING2_CMDDRV2)
	 * | P_Fld(0x14a, SHU3_DRVING2_CMDDRV1));
	 * vIO32WriteFldMulti(DRAMC_REG_SHU3_DRVING3, P_Fld(0x14a, SHU3_DRVING3_DQSODT2)
	 * | P_Fld(0x14a, SHU3_DRVING3_DQSODT1)
	 * | P_Fld(0x14a, SHU3_DRVING3_DQODT2));
	 * vIO32WriteFldMulti(DRAMC_REG_SHU3_DRVING4, P_Fld(0x14a, SHU3_DRVING4_DQODT1)
	 * | P_Fld(0x14a, SHU3_DRVING4_CMDODT2)
	 * | P_Fld(0x14a, SHU3_DRVING4_CMDODT1));
	 * vIO32WriteFldMulti(DRAMC_REG_SHU4_DRVING1, P_Fld(0x14a, SHU4_DRVING1_DQSDRV2)
	 * | P_Fld(0x14a, SHU4_DRVING1_DQSDRV1)
	 * | P_Fld(0x14a, SHU4_DRVING1_DQDRV2));
	 * vIO32WriteFldMulti(DRAMC_REG_SHU4_DRVING2, P_Fld(0x14a, SHU4_DRVING2_DQDRV1)
	 * | P_Fld(0x14a, SHU4_DRVING2_CMDDRV2)
	 * | P_Fld(0x14a, SHU4_DRVING2_CMDDRV1));
	 */
	/*
	 * *((UINT32P)(DRAMC1_AO_BASE + 0x08a8)) = 0x14a5294a;
	 * *((UINT32P)(DRAMC1_AO_BASE + 0x08ac)) = 0x14a5294a;
	 * *((UINT32P)(DRAMC1_AO_BASE + 0x08b0)) = 0x14a5294a;
	 * *((UINT32P)(DRAMC1_AO_BASE + 0x08b4)) = 0x14a5294a;
	 * *((UINT32P)(DRAMC1_AO_BASE + 0x0ea8)) = 0x14a5294a;
	 * *((UINT32P)(DRAMC1_AO_BASE + 0x0eac)) = 0x14a5294a;
	 * *((UINT32P)(DRAMC1_AO_BASE + 0x0eb0)) = 0x14a5294a;
	 * *((UINT32P)(DRAMC1_AO_BASE + 0x0eb4)) = 0x14a5294a;
	 * *((UINT32P)(DRAMC1_AO_BASE + 0x14a8)) = 0x14a5294a;
	 * *((UINT32P)(DRAMC1_AO_BASE + 0x14ac)) = 0x14a5294a;
	 * *((UINT32P)(DRAMC1_AO_BASE + 0x14b0)) = 0x14a5294a;
	 * *((UINT32P)(DRAMC1_AO_BASE + 0x14b4)) = 0x14a5294a;
	 * *((UINT32P)(DRAMC1_AO_BASE + 0x1aa8)) = 0x14a5294a;
	 * *((UINT32P)(DRAMC1_AO_BASE + 0x1aac)) = 0x14a5294a;
	 */
	vIO32WriteFldMulti(DRAMC_REG_SHUCTRL2, P_Fld(0x0, SHUCTRL2_HWSET_WLRL)
		| P_Fld(0x1, SHUCTRL2_SHU_PERIOD_GO_ZERO_CNT)
		| P_Fld(0x1, SHUCTRL2_R_DVFS_OPTION)
		| P_Fld(0x1, SHUCTRL2_R_DVFS_PARK_N)
		| P_Fld(0x1, SHUCTRL2_R_DVFS_DLL_CHA)
		| P_Fld(0xa, SHUCTRL2_R_DLL_IDLE));
	vIO32WriteFldAlign(DRAMC_REG_DVFSDLL, 0x1, DVFSDLL_DLL_LOCK_SHU_EN);
	vIO32WriteFldMulti(DRAMC_REG_DDRCONF0, P_Fld(0x1, DDRCONF0_LPDDR4EN)
		| P_Fld(0x1, DDRCONF0_DM64BITEN)
		| P_Fld(0x1, DDRCONF0_BC4OTF)
		| P_Fld(0x1, DDRCONF0_BK8EN));
	vIO32WriteFldMulti(DRAMC_REG_STBCAL2, P_Fld(0x1, STBCAL2_STB_GERR_B01)
		| P_Fld(0x1, STBCAL2_STB_GERRSTOP)
		| P_Fld(0x1, EYESCAN_EYESCAN_RD_SEL_OPT));
	vIO32WriteFldAlign(DRAMC_REG_STBCAL2, 0x1, STBCAL2_STB_GERR_RST);
	vIO32WriteFldAlign(DRAMC_REG_STBCAL2, 0x0, STBCAL2_STB_GERR_RST);
	vIO32WriteFldAlign(DRAMC_REG_CLKAR, 0x1, CLKAR_PSELAR);
	vIO32WriteFldAlign(DDRPHY_B0_DQ9, 0x1, B0_DQ9_R_DMDQSIEN_RDSEL_LAT_B0);
	vIO32WriteFldAlign(DDRPHY_B1_DQ9, 0x1, B1_DQ9_R_DMDQSIEN_RDSEL_LAT_B1);
	vIO32WriteFldAlign(DDRPHY_CA_CMD10, 0x0, CA_CMD10_R_DMDQSIEN_RDSEL_LAT_CA);
	if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
	{
		vIO32WriteFldMulti(DDRPHY_MISC_CTRL0, P_Fld(0x1, MISC_CTRL0_R_STBENCMP_DIV4CK_EN)
			| P_Fld(0x1, MISC_CTRL0_R_DMDQSIEN_FIFO_EN)
			| P_Fld(0x1, MISC_CTRL0_R_DMSTBEN_OUTSEL)
			| P_Fld(0xf, MISC_CTRL0_R_DMDQSIEN_SYNCOPT));
	}
	else
	{
		vIO32WriteFldMulti(DDRPHY_MISC_CTRL0, P_Fld(0x1, MISC_CTRL0_R_STBENCMP_DIV4CK_EN)
			| P_Fld(0x1, MISC_CTRL0_R_DMDQSIEN_FIFO_EN)
			| P_Fld(0x1, MISC_CTRL0_R_DMSTBEN_OUTSEL)
			| P_Fld(0xf, MISC_CTRL0_R_DMDQSIEN_SYNCOPT));
	}
	/* vIO32WriteFldMulti(DDRPHY_MISC_CTRL1, P_Fld(0x1, MISC_CTRL1_R_DMDA_RRESETB_E) //Already set in vDramcInit_PreSettings() */
	vIO32WriteFldMulti(DDRPHY_MISC_CTRL1, P_Fld(0x1, MISC_CTRL1_R_DMDQSIENCG_EN)
		| P_Fld(0x1, MISC_CTRL1_R_DM_TX_ARCMD_OE)
		| P_Fld(0x1, MISC_CTRL1_R_DM_TX_ARCLK_OE));
	vIO32WriteFldAlign(DDRPHY_B0_RXDVS0, 1, B0_RXDVS0_R_HWSAVE_MODE_ENA_B0);
	vIO32WriteFldAlign(DDRPHY_B1_RXDVS0, 1, B1_RXDVS0_R_HWSAVE_MODE_ENA_B1);
	vIO32WriteFldAlign(DDRPHY_CA_RXDVS0, 0, CA_RXDVS0_R_HWSAVE_MODE_ENA_CA);

	vIO32WriteFldAlign(DDRPHY_CA_CMD7, 0x0, CA_CMD7_RG_TX_ARCMD_PULL_DN);
	vIO32WriteFldAlign(DDRPHY_CA_CMD7, 0x0, CA_CMD7_RG_TX_ARCS_PULL_DN); /* Added by Lingyun.Wu, 11 - 15 */
	vIO32WriteFldAlign(DDRPHY_B0_DQ7, 0x0, B0_DQ7_RG_TX_ARDQ_PULL_DN_B0);
	vIO32WriteFldAlign(DDRPHY_B1_DQ7, 0x0, B1_DQ7_RG_TX_ARDQ_PULL_DN_B1);
	/* vIO32WriteFldAlign(DDRPHY_CA_CMD8, 0x0, CA_CMD8_RG_TX_RRESETB_PULL_DN); //Already set in vDramcInit_PreSettings() */

	vIO32WriteFldMulti(DRAMC_REG_SHU_CONF0, P_Fld(0x2, SHU_CONF0_MATYPE)
		| P_Fld(0x1, SHU_CONF0_BL4)
		| P_Fld(0x1, SHU_CONF0_FREQDIV4)
		| P_Fld(0x1, SHU_CONF0_REFTHD)
		| P_Fld(0x1, SHU_CONF0_ADVPREEN)
		| P_Fld(0x3f, SHU_CONF0_DMPGTIM));
	vIO32WriteFldMulti(DRAMC_REG_SHU_ODTCTRL, P_Fld(0x1, SHU_ODTCTRL_RODTE)
		| P_Fld(0x1, SHU_ODTCTRL_RODTE2)
		| P_Fld(0x1, SHU_ODTCTRL_TWODT)
	/* | P_Fld(0x5, SHU_ODTCTRL_RODT) //Set in UpdateACTimingReg() */
		| P_Fld(0x1, SHU_ODTCTRL_WOEN)
		| P_Fld(0x1, SHU_ODTCTRL_ROEN));
	vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ7, 0x1, SHU1_B0_DQ7_R_DMRODTEN_B0);
	vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ7, 0x1, SHU1_B1_DQ7_R_DMRODTEN_B1);
	vIO32WriteFldAlign(DRAMC_REG_REFCTRL0, 0x5, REFCTRL0_REF_PREGATE_CNT);
	vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_CA1, P_Fld(0x0, SHU_SELPH_CA1_TXDLY_CS1)
		| P_Fld(0x0, SHU_SELPH_CA1_TXDLY_RAS)
		| P_Fld(0x0, SHU_SELPH_CA1_TXDLY_CAS)
		| P_Fld(0x0, SHU_SELPH_CA1_TXDLY_WE)
		| P_Fld(0x0, SHU_SELPH_CA1_TXDLY_RESET)
		| P_Fld(0x0, SHU_SELPH_CA1_TXDLY_ODT)
		| P_Fld(0x0, SHU_SELPH_CA1_TXDLY_CKE)
		| P_Fld(0x0, SHU_SELPH_CA1_TXDLY_CS));

	if (p->frequency < 1333)
		u1TXDLY_CMD = 0x7;
	else if (p->frequency < 1600)
		u1TXDLY_CMD = 0x8;
	else if (p->frequency < 1866)
		u1TXDLY_CMD = 0x9;
	else if (p->frequency < 2133)
		u1TXDLY_CMD = 0xA;
	else
		u1TXDLY_CMD = 0xB;

	vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_CA2, P_Fld(0x0, SHU_SELPH_CA2_TXDLY_CKE1)
		| P_Fld(u1TXDLY_CMD, SHU_SELPH_CA2_TXDLY_CMD)
		| P_Fld(0x0, SHU_SELPH_CA2_TXDLY_BA2)
		| P_Fld(0x0, SHU_SELPH_CA2_TXDLY_BA1)
		| P_Fld(0x0, SHU_SELPH_CA2_TXDLY_BA0));
	vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_CA3, P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA7)
		| P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA6)
		| P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA5)
		| P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA4)
		| P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA3)
		| P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA2)
		| P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA1)
		| P_Fld(0x0, SHU_SELPH_CA3_TXDLY_RA0));
	vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_CA4, P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA15)
		| P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA14)
		| P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA13)
		| P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA12)
		| P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA11)
		| P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA10)
		| P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA9)
		| P_Fld(0x0, SHU_SELPH_CA4_TXDLY_RA8));
	vIO32WriteFldAlign(DRAMC_REG_SHU_SELPH_CA5, 0x0, SHU_SELPH_CA5_DLY_ODT);
	vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS0, P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS3)
		| P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS2)
		| P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS1)
		| P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_OEN_DQS0)
		| P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_DQS3)
		| P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_DQS2)
		| P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_DQS1)
		| P_Fld(0x3, SHU_SELPH_DQS0_TXDLY_DQS0));
	vIO32WriteFldMulti(DRAMC_REG_SHU_SELPH_DQS1, P_Fld(0x2, SHU_SELPH_DQS1_DLY_OEN_DQS3)
		| P_Fld(0x2, SHU_SELPH_DQS1_DLY_OEN_DQS2)
		| P_Fld(0x2, SHU_SELPH_DQS1_DLY_OEN_DQS1)
		| P_Fld(0x2, SHU_SELPH_DQS1_DLY_OEN_DQS0)
		| P_Fld(0x5, SHU_SELPH_DQS1_DLY_DQS3)
		| P_Fld(0x5, SHU_SELPH_DQS1_DLY_DQS2)
		| P_Fld(0x5, SHU_SELPH_DQS1_DLY_DQS1)
		| P_Fld(0x5, SHU_SELPH_DQS1_DLY_DQS0));
	vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ3)
		| P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ2)
		| P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ1)
		| P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_OEN_DQ0)
		| P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ3)
		| P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ2)
		| P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ1)
		| P_Fld(0x3, SHURK0_SELPH_DQ0_TXDLY_DQ0));
	vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM3)
		| P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM2)
		| P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM1)
		| P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_OEN_DQM0)
		| P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM3)
		| P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM2)
		| P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM1)
		| P_Fld(0x3, SHURK0_SELPH_DQ1_TXDLY_DQM0));
	vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ2, P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_OEN_DQ3)
		| P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_OEN_DQ2)
		| P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_OEN_DQ1)
		| P_Fld(0x2, SHURK0_SELPH_DQ2_DLY_OEN_DQ0)
		| P_Fld(0x6, SHURK0_SELPH_DQ2_DLY_DQ3)
		| P_Fld(0x6, SHURK0_SELPH_DQ2_DLY_DQ2)
		| P_Fld(0x6, SHURK0_SELPH_DQ2_DLY_DQ1)
		| P_Fld(0x6, SHURK0_SELPH_DQ2_DLY_DQ0));
	vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ3, P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_OEN_DQM3)
		| P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_OEN_DQM2)
		| P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_OEN_DQM1)
		| P_Fld(0x2, SHURK0_SELPH_DQ3_DLY_OEN_DQM0)
		| P_Fld(0x6, SHURK0_SELPH_DQ3_DLY_DQM3)
		| P_Fld(0x6, SHURK0_SELPH_DQ3_DLY_DQM2)
		| P_Fld(0x6, SHURK0_SELPH_DQ3_DLY_DQM1)
		| P_Fld(0x6, SHURK0_SELPH_DQ3_DLY_DQM0));
	vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ3)
		| P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ2)
		| P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ1)
		| P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1OEN_DQ0)
		| P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ3)
		| P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ2)
		| P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ1)
		| P_Fld(0x3, SHURK1_SELPH_DQ0_TX_DLY_R1DQ0));
	vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM3)
		| P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM2)
		| P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM1)
		| P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1OEN_DQM0)
		| P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM3)
		| P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM2)
		| P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM1)
		| P_Fld(0x3, SHURK1_SELPH_DQ1_TX_DLY_R1DQM0));
	vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ2, P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ3)
		| P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ2)
		| P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ1)
		| P_Fld(0x2, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ0)
		| P_Fld(0x6, SHURK1_SELPH_DQ2_DLY_R1DQ3)
		| P_Fld(0x6, SHURK1_SELPH_DQ2_DLY_R1DQ2)
		| P_Fld(0x6, SHURK1_SELPH_DQ2_DLY_R1DQ1)
		| P_Fld(0x6, SHURK1_SELPH_DQ2_DLY_R1DQ0));
	vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ3, P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM3)
		| P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM2)
		| P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM1)
		| P_Fld(0x2, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM0)
		| P_Fld(0x6, SHURK1_SELPH_DQ3_DLY_R1DQM3)
		| P_Fld(0x6, SHURK1_SELPH_DQ3_DLY_R1DQM2)
		| P_Fld(0x6, SHURK1_SELPH_DQ3_DLY_R1DQM1)
		| P_Fld(0x6, SHURK1_SELPH_DQ3_DLY_R1DQM0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B1_DQ7, P_Fld(0x1a, SHU1_R0_B1_DQ7_RK0_ARPI_DQM_B1)
		| P_Fld(0x1a, SHU1_R0_B1_DQ7_RK0_ARPI_DQ_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R0_B0_DQ7, P_Fld(0x1a, SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0)
		| P_Fld(0x1a, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B1_DQ7, P_Fld(0x14, SHU1_R1_B1_DQ7_RK1_ARPI_DQM_B1)
		| P_Fld(0x14, SHU1_R1_B1_DQ7_RK1_ARPI_DQ_B1));
	vIO32WriteFldMulti(DDRPHY_SHU1_R1_B0_DQ7, P_Fld(0x14, SHU1_R1_B0_DQ7_RK1_ARPI_DQM_B0)
		| P_Fld(0x14, SHU1_R1_B0_DQ7_RK1_ARPI_DQ_B0));

	mcDELAY_US(1);

	vIO32WriteFldAlign(DDRPHY_B1_DQ9, 0x1, B1_DQ9_RG_RX_ARDQS0_DQSIENMODE_B1);
	vIO32WriteFldAlign(DDRPHY_B0_DQ9, 0x1, B0_DQ9_RG_RX_ARDQS0_DQSIENMODE_B0);
	vIO32WriteFldAlign(DDRPHY_B0_DQ6, 0x1, B0_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B0);
	vIO32WriteFldAlign(DDRPHY_B1_DQ6, 0x1, B1_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B1);
	vIO32WriteFldAlign(DRAMC_REG_STBCAL, 0x1, STBCAL_DQSIENMODE);
	vIO32WriteFldMulti(DRAMC_REG_SREFCTRL, P_Fld(0x0, SREFCTRL_SREF_HW_EN)
		| P_Fld(0x8, SREFCTRL_SREFDLY));
	vIO32WriteFldMulti(DRAMC_REG_SHU_CKECTRL, P_Fld(0x3, SHU_CKECTRL_SREF_CK_DLY)
		| P_Fld(0x3, SHU_CKECTRL_TCKESRX));
	/* | P_Fld(0x3, SHU_CKECTRL_CKEPRD)); */
#ifdef FIRST_BRING_UP
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_PIPE), P_Fld(0x1, SHU_PIPE_READ_START_EXTEND1)
		| P_Fld(0x1, SHU_PIPE_DLE_LAST_EXTEND1)
		| P_Fld(0x1, SHU_PIPE_READ_START_EXTEND2)
		| P_Fld(0x1, SHU_PIPE_DLE_LAST_EXTEND2)
		| P_Fld(0x1, SHU_PIPE_READ_START_EXTEND3)
		| P_Fld(0x1, SHU_PIPE_DLE_LAST_EXTEND3));
#else
	vIO32WriteFldMulti(DRAMC_REG_SHU_PIPE, P_Fld(0x1, SHU_PIPE_READ_START_EXTEND1)
		| P_Fld(0x1, SHU_PIPE_DLE_LAST_EXTEND1));
#endif
	vIO32WriteFldMulti(DRAMC_REG_CKECTRL, P_Fld(0x1, CKECTRL_CKEON)
		| P_Fld(0x1, CKECTRL_CKETIMER_SEL));
	vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0x1, RKCFG_CKE2RANK_OPT2);
	if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
	{
		vIO32WriteFldMulti(DRAMC_REG_SHU_CONF2, P_Fld(0x1, SHU_CONF2_WPRE2T)
			| P_Fld(0x7, SHU_CONF2_DCMDLYREF));
		/* | P_Fld(0x64, SHU_CONF2_FSPCHG_PRDCNT)); //ACTiming related -> set in UpdateACTiming_Reg() */
		vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x1, SPCMDCTRL_CLR_EN);
		/* vIO32WriteFldAlign(DRAMC_REG_SHU_SCINTV, 0xf, SHU_SCINTV_MRW_INTV); (Set in UpdateACTimingReg()) */
		vIO32WriteFldAlign(DRAMC_REG_SHUCTRL1, 0x40, SHUCTRL1_FC_PRDCNT);
	}
	else
	{
		vIO32WriteFldMulti(DRAMC_REG_SHU_CONF2, P_Fld(0x1, SHU_CONF2_WPRE2T)
			| P_Fld(0x7, SHU_CONF2_DCMDLYREF));
		/* | P_Fld(0x64, SHU_CONF2_FSPCHG_PRDCNT)); //ACTiming related -> set in UpdateACTiming_Reg() */
		vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x1, SPCMDCTRL_CLR_EN);
		/* vIO32WriteFldAlign(DRAMC_REG_SHU_SCINTV, 0xf, SHU_SCINTV_MRW_INTV); (Set in UpdateACTimingReg()) */
		vIO32WriteFldAlign(DRAMC_REG_SHUCTRL1, 0x40, SHUCTRL1_FC_PRDCNT);
	}
	vIO32WriteFldAlign(DRAMC_REG_SHUCTRL, 0x1, SHUCTRL_LPSM_BYPASS_B);
	vIO32WriteFldMulti(DRAMC_REG_REFCTRL1, P_Fld(0x0, REFCTRL1_SREF_PRD_OPT) | P_Fld(0x0, REFCTRL1_PSEL_OPT1) | P_Fld(0x0, REFCTRL1_PSEL_OPT2) | P_Fld(0x0, REFCTRL1_PSEL_OPT3));
	/*
	 * vIO32WriteFldAlign(DDRPHY_SHU1_PLL4, 0xfe, SHU1_PLL4_RG_RPHYPLL_RESERVED);
	 * vIO32WriteFldAlign(DDRPHY_SHU1_PLL6, 0xfe, SHU1_PLL6_RG_RCLRPLL_RESERVED);
	 */
	vIO32WriteFldMulti(DRAMC_REG_REFRATRE_FILTER, P_Fld(0x1, REFRATRE_FILTER_PB2AB_OPT) | P_Fld(0x0, REFRATRE_FILTER_PB2AB_OPT1));

#if !APPLY_LP4_POWER_INIT_SEQUENCE
	vIO32WriteFldAlign(DDRPHY_MISC_CTRL1, 0x1, MISC_CTRL1_R_DMDA_RRESETB_I);
#endif
	vIO32WriteFldAlign(DRAMC_REG_DRAMCTRL, 0x0, DRAMCTRL_CLKWITRFC);
	vIO32WriteFldMulti(DRAMC_REG_MISCTL0, P_Fld(0x1, MISCTL0_REFP_ARB_EN2)
		| P_Fld(0x1, MISCTL0_PBC_ARB_EN)
		| P_Fld(0x1, MISCTL0_REFA_ARB_EN2));
	vIO32WriteFldMulti(DRAMC_REG_PERFCTL0, P_Fld(0x1, PERFCTL0_MWHPRIEN)
	/* | P_Fld(0x1, PERFCTL0_RWSPLIT) //Set in UpdateInitialSettings_LP4() */
		| P_Fld(0x1, PERFCTL0_WFLUSHEN)
		| P_Fld(0x1, PERFCTL0_EMILLATEN)
		| P_Fld(0x1, PERFCTL0_RWAGEEN)
		| P_Fld(0x1, PERFCTL0_RWLLATEN)
		| P_Fld(0x1, PERFCTL0_RWHPRIEN)
		| P_Fld(0x1, PERFCTL0_RWOFOEN)
		| P_Fld(0x1, PERFCTL0_DISRDPHASE1)
		| P_Fld(0x1, PERFCTL0_DUALSCHEN));
	vIO32WriteFldAlign(DRAMC_REG_ARBCTL, 0x80, ARBCTL_MAXPENDCNT);
	vIO32WriteFldMulti(DRAMC_REG_PADCTRL, P_Fld(0x1, PADCTRL_DQIENLATEBEGIN)
		| P_Fld(0x1, PADCTRL_DQIENQKEND));
	vIO32WriteFldAlign(DRAMC_REG_DRAMC_PD_CTRL, 0x1, DRAMC_PD_CTRL_DCMREF_OPT);
	vIO32WriteFldAlign(DRAMC_REG_CLKCTRL, 0x1, CLKCTRL_CLK_EN_1);
	vIO32WriteFldMulti(DRAMC_REG_REFCTRL0, P_Fld(0x4, REFCTRL0_DISBYREFNUM)
		| P_Fld(0x1, REFCTRL0_DLLFRZ));
#if 0 /* CATRAIN_INTV, CATRAINLAT: Correct values are set in UpdateACTimingReg( ) */
	vIO32WriteFldMulti(DRAMC_REG_CATRAINING1, P_Fld(0xff, CATRAINING1_CATRAIN_INTV)
		| P_Fld(0x0, CATRAINING1_CATRAINLAT));
#endif
	vIO32WriteFldMulti(DRAMC_REG_SHU_RANKCTL, P_Fld(0x6, SHU_RANKCTL_RANKINCTL_PHY)
		| P_Fld(0x4, SHU_RANKCTL_RANKINCTL_ROOT1)
		| P_Fld(0x4, SHU_RANKCTL_RANKINCTL));

	/*
	 * vIO32WriteFldAlign(DRAMC_REG_SHU_STBCAL, 0x1, SHU_STBCAL_DMSTBLAT);
	 * vIO32WriteFldAlign(DRAMC_REG_SHURK0_DQSCTL, 0x5, SHURK0_DQSCTL_DQSINCTL); //DQSINCTL: set in UpdateACTimingReg()
	 * vIO32WriteFldAlign(DRAMC_REG_SHURK1_DQSCTL, 0x5, SHURK1_DQSCTL_R1DQSINCTL); //Set in UpdateACTimingReg()
	 */

	mcDELAY_US(2);
#if LEGACY_GATING_DLY
	LegacyGatingDlyLP4_DDR3200(p);
#endif
	if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
	{
	}
	else
	{
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQSIEN, P_Fld(0x19, SHURK0_DQSIEN_R0DQS1IEN)
			| P_Fld(0x19, SHURK0_DQSIEN_R0DQS0IEN));
		vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQSIEN, P_Fld(0x0, SHURK1_DQSIEN_R1DQS3IEN)
			| P_Fld(0x0, SHURK1_DQSIEN_R1DQS2IEN)
			| P_Fld(0x1b, SHURK1_DQSIEN_R1DQS1IEN)
			| P_Fld(0x1b, SHURK1_DQSIEN_R1DQS0IEN));
	}
	/*
	 * 41536 === over_write_setting_begin ===
	 * 41536 === over_write_setting_end ===
	 */
	vIO32WriteFldAlign(DRAMC_REG_DRAMCTRL, 0x1, DRAMCTRL_PREALL_OPTION);
	vIO32WriteFldAlign(DRAMC_REG_ZQCS, 0x56, ZQCS_ZQCSOP);

	mcDELAY_US(1);

	vIO32WriteFldAlign(DRAMC_REG_SHU_CONF1, 0x1, SHU_CONF1_TREFBWIG);
	vIO32WriteFldAlign(DRAMC_REG_SHU_CONF3, 0xff, SHU_CONF3_REFRCNT);
	vIO32WriteFldAlign(DRAMC_REG_REFCTRL0, 0x1, REFCTRL0_REFFRERUN);
	vIO32WriteFldAlign(DRAMC_REG_SREFCTRL, 0x1, SREFCTRL_SREF_HW_EN);
	vIO32WriteFldAlign(DRAMC_REG_MPC_OPTION, 0x1, MPC_OPTION_MPCRKEN);
	vIO32WriteFldAlign(DRAMC_REG_DRAMC_PD_CTRL, 0x1, DRAMC_PD_CTRL_PHYCLKDYNGEN);
	vIO32WriteFldAlign(DRAMC_REG_DRAMC_PD_CTRL, 0x1, DRAMC_PD_CTRL_DCMEN);
	vIO32WriteFldMulti(DRAMC_REG_EYESCAN, P_Fld(0x0, EYESCAN_RX_DQ_EYE_SEL)
		| P_Fld(0x0, EYESCAN_RG_RX_EYE_SCAN_EN));
	vIO32WriteFldMulti(DRAMC_REG_STBCAL1, P_Fld(0x1, STBCAL1_STBCNT_LATCH_EN)
		| P_Fld(0x1, STBCAL1_STBENCMPEN));
	vIO32WriteFldAlign(DRAMC_REG_TEST2_1, 0x10000, TEST2_1_TEST2_BASE);
#if (FOR_DV_SIMULATION_USED == 1 || SW_CHANGE_FOR_SIMULATION == 1)
	/*
	 * because cmd_len=1 has bug with byte mode, so need to set cmd_len=0, then it will cost more time to do a pattern test
	 * workaround: reduce TEST2_OFF to make less test agent cmd. make lpddr4-1600 can finish in 60us (Mengru)
	 */
	vIO32WriteFldAlign(DRAMC_REG_TEST2_2, 0x20, TEST2_2_TEST2_OFF);
#else
	vIO32WriteFldAlign(DRAMC_REG_TEST2_2, 0x400, TEST2_2_TEST2_OFF);
#endif
	vIO32WriteFldMulti(DRAMC_REG_TEST2_3, P_Fld(0x1, TEST2_3_TEST2WREN2_HW_EN)
		| P_Fld(0x4, TEST2_3_DQSICALSTP)
		| P_Fld(0x1, TEST2_3_TESTAUDPAT));
	vIO32WriteFldAlign(DRAMC_REG_RSTMASK, 0x0, RSTMASK_DAT_SYNC_MASK);
	vIO32WriteFldAlign(DRAMC_REG_RSTMASK, 0x0, RSTMASK_PHY_SYNC_MASK);

	mcDELAY_US(1);

	vIO32WriteFldMulti(DRAMC_REG_HW_MRR_FUN, P_Fld(0x0, HW_MRR_FUN_TRPMRR_EN)
		| P_Fld(0x0, HW_MRR_FUN_TRCDMRR_EN) | P_Fld(0x0, HW_MRR_FUN_TMRR_ENA)
		| P_Fld(0x6, HW_MRR_FUN_MANTMRR) | P_Fld(0x0, HW_MRR_FUN_MANTMRR_EN));
	if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
	{
		vIO32WriteFldAlign(DRAMC_REG_PERFCTL0, 0x1, PERFCTL0_WRFIO_MODE2);
		/* | P_Fld(0x0, PERFCTL0_RWSPLIT)); //Set in UpdateInitialSettings_LP4() */
		vIO32WriteFldMulti(DRAMC_REG_PERFCTL0, P_Fld(0x1, PERFCTL0_REORDEREN)
			| P_Fld(0x0, PERFCTL0_REORDER_MODE));
		vIO32WriteFldAlign(DRAMC_REG_RSTMASK, 0x0, RSTMASK_GT_SYNC_MASK);
		vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0x1, RKCFG_DQSOSC2RK);
		vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x1, SPCMDCTRL_REFR_BLOCKEN);
		vIO32WriteFldAlign(DRAMC_REG_EYESCAN, 0x0, EYESCAN_RG_RX_MIOCK_JIT_EN);
	}
	else
	{
		vIO32WriteFldAlign(DRAMC_REG_DRAMCTRL, 0x0, DRAMCTRL_CTOREQ_HPRI_OPT);
		vIO32WriteFldMulti(DRAMC_REG_PERFCTL0, P_Fld(0x1, PERFCTL0_REORDEREN)
			| P_Fld(0x0, PERFCTL0_REORDER_MODE));
		vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x1, SPCMDCTRL_REFR_BLOCKEN);
		vIO32WriteFldAlign(DRAMC_REG_RSTMASK, 0x0, RSTMASK_GT_SYNC_MASK);
		vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0x1, RKCFG_DQSOSC2RK);
		vIO32WriteFldAlign(DRAMC_REG_MPC_OPTION, 1, MPC_OPTION_MPCRKEN);
		vIO32WriteFldAlign(DRAMC_REG_EYESCAN, 0x0, EYESCAN_RG_RX_MIOCK_JIT_EN);
		vIO32WriteFldAlign(DRAMC_REG_SHU1_WODT, 0x1, SHU1_WODT_DBIWR);
		vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ7, 0x1, SHU1_B0_DQ7_R_DMDQMDBI_SHU_B0);
		vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ7, 0x1, SHU1_B1_DQ7_R_DMDQMDBI_SHU_B1);
		vIO32WriteFldAlign(DRAMC_REG_SHU_RANKCTL, 0x4, SHU_RANKCTL_RANKINCTL);
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ0, P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ1)
			| P_Fld(0x2, SHURK0_SELPH_DQ0_TXDLY_DQ0));
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ1, P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM1)
			| P_Fld(0x2, SHURK0_SELPH_DQ1_TXDLY_DQM0));
		vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ0, P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ1)
			| P_Fld(0x2, SHURK1_SELPH_DQ0_TX_DLY_R1DQ0));
		vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ1, P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM1)
			| P_Fld(0x2, SHURK1_SELPH_DQ1_TX_DLY_R1DQM0));
	}
#if ENABLE_TMRRI_NEW_MODE
	vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x0, SPCMDCTRL_REFR_BLOCKEN);
	vIO32WriteFldAlign(DRAMC_REG_HW_MRR_FUN, 0x1, HW_MRR_FUN_TMRR_ENA);
#endif
	mcDELAY_US(5);

	vIO32WriteFldAlign(DRAMC_REG_STBCAL1, 0x3, STBCAL1_STBCAL_FILTER);
	vIO32WriteFldAlign(DRAMC_REG_STBCAL1, 0x1, STBCAL1_STBCAL_FILTER);
	vIO32WriteFldMulti(DRAMC_REG_STBCAL, P_Fld(0x1, STBCAL_STB_DQIEN_IG)
		| P_Fld(0x1, STBCAL_PICHGBLOCK_NORD)
		| P_Fld(0x0, STBCAL_STBCALEN)
		| P_Fld(0x0, STBCAL_STB_SELPHYCALEN)
		| P_Fld(0x1, STBCAL_PIMASK_RKCHG_OPT));
	vIO32WriteFldAlign(DRAMC_REG_STBCAL1, 0x1, STBCAL1_STB_SHIFT_DTCOUT_IG);
	vIO32WriteFldMulti(DRAMC_REG_SHU1_DQSG, P_Fld(0x9, SHU1_DQSG_STB_UPDMASKCYC)
		| P_Fld(0x1, SHU1_DQSG_STB_UPDMASK_EN));
	vIO32WriteFldAlign(DDRPHY_MISC_CTRL0, 0x0, MISC_CTRL0_R_DMDQSIEN_SYNCOPT);
	vIO32WriteFldAlign(DRAMC_REG_SHU_STBCAL, 0x1, SHU_STBCAL_DQSG_MODE);
	vIO32WriteFldAlign(DRAMC_REG_STBCAL, 0x1, STBCAL_SREF_DQSGUPD);
	/* M17_Remap:vIO32WriteFldAlign(DDRPHY_MISC_CTRL1, 0x0, MISC_CTRL1_R_DMDQMDBI); */
	/* RX Tracking DQM SM enable (actual values are set in DramcRxInputDelayTrackingHW()) */
#if ENABLE_RX_TRACKING_LP4
	vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ7, p->DBI_R_onoff[p->dram_fsp], SHU1_B0_DQ7_R_DMRXTRACK_DQM_EN_B0);
	vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ7, p->DBI_R_onoff[p->dram_fsp], SHU1_B1_DQ7_R_DMRXTRACK_DQM_EN_B1);
#else
	vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ7, 0, SHU1_B0_DQ7_R_DMRXTRACK_DQM_EN_B0);
	vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ7, 0, SHU1_B1_DQ7_R_DMRXTRACK_DQM_EN_B1);
#endif

	vIO32WriteFldAlign(DRAMC_REG_SHU_STBCAL, 0x1, SHU_STBCAL_PICGLAT);
	/* 6771 MP setting should set SHU_STBCAL_DMSTBLAT as 0x2 (review by HJ Huang) */

	if (p->frequency <= 800)
	{
		vIO32WriteFldAlign(DRAMC_REG_SHU_STBCAL, 0, SHU_STBCAL_DMSTBLAT);
	}
	else
	{
		vIO32WriteFldAlign(DRAMC_REG_SHU_STBCAL, 2, SHU_STBCAL_DMSTBLAT);
	}

	vIO32WriteFldMulti(DRAMC_REG_REFCTRL1, P_Fld(0x1, REFCTRL1_REF_QUE_AUTOSAVE_EN)
		| P_Fld(0x1, REFCTRL1_SLEFREF_AUTOSAVE_EN));
	vIO32WriteFldMulti(DRAMC_REG_DQSOSCR, P_Fld(0x1, DQSOSCR_SREF_TXPI_RELOAD_OPT)
		| P_Fld(0x1, DQSOSCR_SREF_TXUI_RELOAD_OPT));
	vIO32WriteFldMulti(DRAMC_REG_RSTMASK, P_Fld(0x0, RSTMASK_DVFS_SYNC_MASK)
		| P_Fld(0x0, RSTMASK_GT_SYNC_MASK_FOR_PHY)
		| P_Fld(0x0, RSTMASK_DVFS_SYNC_MASK_FOR_PHY));
	vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0x1, RKCFG_RKMODE);

#if !APPLY_LP4_POWER_INIT_SEQUENCE
	vIO32WriteFldMulti(DRAMC_REG_CKECTRL, P_Fld(0x1, CKECTRL_CKEFIXON)
		| P_Fld(0x1, CKECTRL_CKE1FIXON));
#endif

	mcDELAY_US(12);

	/*
	 * TODO: DDR3200
	 * if(p->frequency==1600)
	 */
	{
		/* 60826 ===dramc_shu1_lp4_3200 begin=== */
		if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
		{
			vIO32WriteFldMulti(DRAMC_REG_SHU_RANKCTL, P_Fld(0x5, SHU_RANKCTL_RANKINCTL_PHY)
				| P_Fld(0x3, SHU_RANKCTL_RANKINCTL_ROOT1)
				| P_Fld(0x3, SHU_RANKCTL_RANKINCTL));
		}
		else
		{
			vIO32WriteFldMulti(DRAMC_REG_SHU_RANKCTL, P_Fld(0x6, SHU_RANKCTL_RANKINCTL_PHY)
				| P_Fld(0x4, SHU_RANKCTL_RANKINCTL_ROOT1));
		}
	#if LEGACY_TX_TRACK
		LegacyTxTrackLP4_DDR3200(p);
	#endif
		vIO32WriteFldAlign(DRAMC_REG_SHU1_WODT, 0x0, SHU1_WODT_WPST2T);
		/* vIO32WriteFldAlign(DRAMC_REG_SHU_HWSET_MR2, 0x2d, SHU_HWSET_MR2_HWSET_MR2_OP); */
		if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
		{
			vIO32WriteFldMulti(DRAMC_REG_SHURK0_DQSIEN, P_Fld(0x19, SHURK0_DQSIEN_R0DQS1IEN)
				| P_Fld(0x19, SHURK0_DQSIEN_R0DQS0IEN));
		}
		vIO32WriteFldMulti(DRAMC_REG_SHU1RK0_PI, P_Fld(0x1a, SHU1RK0_PI_RK0_ARPI_DQM_B1)
			| P_Fld(0x1a, SHU1RK0_PI_RK0_ARPI_DQM_B0)
			| P_Fld(0x1a, SHU1RK0_PI_RK0_ARPI_DQ_B1)
			| P_Fld(0x1a, SHU1RK0_PI_RK0_ARPI_DQ_B0));
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ2, P_Fld(0x4, SHURK0_SELPH_DQ2_DLY_OEN_DQ3)
			| P_Fld(0x4, SHURK0_SELPH_DQ2_DLY_OEN_DQ2)
			| P_Fld(0x4, SHURK0_SELPH_DQ2_DLY_OEN_DQ1)
			| P_Fld(0x4, SHURK0_SELPH_DQ2_DLY_OEN_DQ0));
		vIO32WriteFldMulti(DRAMC_REG_SHURK0_SELPH_DQ3, P_Fld(0x4, SHURK0_SELPH_DQ3_DLY_OEN_DQM3)
			| P_Fld(0x4, SHURK0_SELPH_DQ3_DLY_OEN_DQM2)
			| P_Fld(0x4, SHURK0_SELPH_DQ3_DLY_OEN_DQM1)
			| P_Fld(0x4, SHURK0_SELPH_DQ3_DLY_OEN_DQM0));
		if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
		{
			vIO32WriteFldMulti(DRAMC_REG_SHURK1_DQSIEN, P_Fld(0x0, SHURK1_DQSIEN_R1DQS3IEN)
				| P_Fld(0x0, SHURK1_DQSIEN_R1DQS2IEN)
				| P_Fld(0x1b, SHURK1_DQSIEN_R1DQS1IEN)
				| P_Fld(0x1b, SHURK1_DQSIEN_R1DQS0IEN));
		}
		vIO32WriteFldMulti(DRAMC_REG_SHU1RK1_PI, P_Fld(0x14, SHU1RK1_PI_RK1_ARPI_DQM_B1)
			| P_Fld(0x14, SHU1RK1_PI_RK1_ARPI_DQM_B0)
			| P_Fld(0x14, SHU1RK1_PI_RK1_ARPI_DQ_B1)
			| P_Fld(0x14, SHU1RK1_PI_RK1_ARPI_DQ_B0));
		vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ2, P_Fld(0x4, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ3)
			| P_Fld(0x4, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ2)
			| P_Fld(0x4, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ1)
			| P_Fld(0x4, SHURK1_SELPH_DQ2_DLY_R1OEN_DQ0));
		vIO32WriteFldMulti(DRAMC_REG_SHURK1_SELPH_DQ3, P_Fld(0x4, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM3)
			| P_Fld(0x4, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM2)
			| P_Fld(0x4, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM1)
			| P_Fld(0x4, SHURK1_SELPH_DQ3_DLY_R1OEN_DQM0));
		vIO32WriteFldMulti(DRAMC_REG_SHU_DQSG_RETRY, P_Fld(0x0, SHU_DQSG_RETRY_R_RETRY_ROUND_NUM)
			| P_Fld(0x0, SHU_DQSG_RETRY_R_XSR_RETRY_SPM_MODE)
			| P_Fld(0x0, SHU_DQSG_RETRY_R_DQSIENLAT)
			| P_Fld(0x0, SHU_DQSG_RETRY_R_DDR1866_PLUS));
	#if LEGACY_TDQSCK_PRECAL
		LegacyPreCalLP4_DDR3200(p);
	#endif
		/* 61832 ===dramc_shu1_lp4_3200 end=== */


		/* 66870 ===ddrphy_shu1_lp4_3200_CHA begin=== */
		if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
		{
			vIO32WriteFldAlign(DDRPHY_SHU1_B0_DQ7, 0x1, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0);
			vIO32WriteFldAlign(DDRPHY_SHU1_B1_DQ7, 0x1, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1);
		}
		else
		{
			vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ7, P_Fld(0x1, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0)
				| P_Fld(0x1, SHU1_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0));
			vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ7, P_Fld(0x1, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1)
				| P_Fld(0x1, SHU1_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1));
			/*
			 * francis remove : it will make CLRPLL frequency wrong!
			 * francis remove                vIO32WriteFldMulti(DDRPHY_SHU1_PLL7, P_Fld(0x3d00, SHU1_PLL7_RG_RCLRPLL_SDM_PCW)
			 * francis remove                            | P_Fld(0x1, SHU1_PLL7_RG_RCLRPLL_SDM_PCW_CHG));
			 */
		}
		/* 67761 ===ddrphy_shu1_lp4_3200_CHA end=== */

		/* NOT included in parsing tool */
		vIO32WriteFldAlign(DRAMC_REG_SHU_DQS2DQ_TX, 0x0, SHU_DQS2DQ_TX_OE2DQ_OFFSET);
	}
	/* TODO: DDR3733 */
	if (p->freqGroup == 1866)
	{
		DramcSetting_Olympus_LP4_ByteMode_DDR3733(p);
	}
	/* TODO: DDR2667 */
	else if (p->freqGroup == 1333 || p->freqGroup == 1200) /* TODO: Initial settings for DDR2400 ? */
	{
		DramcSetting_Olympus_LP4_ByteMode_DDR2667(p);
	}
	else if (p->freqGroup == 800)
	{
		DramcSetting_Olympus_LP4_ByteMode_DDR1600(p);
	}

	UpdateInitialSettings_LP4(p);

#if SIMULATION_SW_IMPED
	if (p->dram_type == TYPE_LPDDR4) {
		DramcSwImpedanceSaveRegister(p, ODT_ON, ODT_ON, DRAM_DFS_SHUFFLE_1);
	} else if (p->dram_type == TYPE_LPDDR4X) {
	#if FSP1_CLKCA_TERM
		DramcSwImpedanceSaveRegister(p, (p->frequency >= MRFSP_TERM_FREQ) ? ODT_ON : ODT_OFF,
			p->odt_onoff, DRAM_DFS_SHUFFLE_1);
	#else
		DramcSwImpedanceSaveRegister(p, ODT_OFF, p->odt_onoff, DRAM_DFS_SHUFFLE_1);
	#endif
	} else if (p->dram_type == TYPE_LPDDR4P) {
		DramcSwImpedanceSaveRegister(p, ODT_OFF, ODT_OFF, DRAM_DFS_SHUFFLE_1);
	}
#endif

#ifndef LOOPBACK_TEST
	DDRPhyFreqMeter();
#endif

#if 0
	vIO32WriteFldMulti(DRAMC_REG_MRS, P_Fld(0x0, MRS_MRSRK)
		| P_Fld(0x4, MRS_MRSMA)
		| P_Fld(0x0, MRS_MRSOP));
	mcDELAY_US(1);
	vIO32WriteFldAlign(DRAMC_REG_SPCMD, 0x1, SPCMD_MRREN);
	vIO32WriteFldAlign(DRAMC_REG_SPCMD, 0x0, SPCMD_MRREN);
#endif

	vIO32WriteFldAlign(DRAMC_REG_TEST2_4, 0x0, TEST2_4_TEST_REQ_LEN1);

	vIO32WriteFldAlign(DRAMC_REG_SHU_CONF3, 0x5, SHU_CONF3_ZQCSCNT);

	mcDELAY_US(1);

#if !APPLY_LP4_POWER_INIT_SEQUENCE
	vIO32WriteFldMulti(DRAMC_REG_CKECTRL, P_Fld(0x0, CKECTRL_CKEFIXON)
		| P_Fld(0x0, CKECTRL_CKE1FIXON));
#endif
	vIO32WriteFldMulti(DRAMC_REG_REFCTRL0, P_Fld(0x1, REFCTRL0_PBREFEN)
		| P_Fld(0x1, REFCTRL0_PBREF_DISBYRATE));
	vIO32WriteFldMulti(DRAMC_REG_SHUCTRL2, P_Fld(0x1, SHUCTRL2_MR13_SHU_EN)
		| P_Fld(0x1, SHUCTRL2_HWSET_WLRL));
	vIO32WriteFldAlign(DRAMC_REG_REFCTRL0, 0x1, REFCTRL0_REFDIS);
	/* vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0x0, SPCMDCTRL_REFRDIS);//Lewis@20160613: Fix refresh rate is wrong while diable MR4 */
	vIO32WriteFldAlign(DRAMC_REG_DRAMCTRL, 0x1, DRAMCTRL_REQQUE_THD_EN);
	/* | P_Fld(0x1, DRAMCTRL_DPDRK_OPT)); //DPDRK_OPT doesn't exit for 6771 */
	vIO32WriteFldMulti(DRAMC_REG_DUMMY_RD, P_Fld(0x1, DUMMY_RD_DQSG_DMYRD_EN)
		| P_Fld(p->support_rank_num, DUMMY_RD_RANK_NUM)
		| P_Fld(0x1, DUMMY_RD_DUMMY_RD_CNT6)
		| P_Fld(0x1, DUMMY_RD_DUMMY_RD_CNT5)
		| P_Fld(0x1, DUMMY_RD_DUMMY_RD_CNT3)
		| P_Fld(0x1, DUMMY_RD_DUMMY_RD_SW));

	/* cc change from 4->0 after DE setting review */
	vIO32WriteFldAlign(DRAMC_REG_TEST2_4, 0x0, TEST2_4_TESTAGENTRKSEL);
	vIO32WriteFldAlign(DRAMC_REG_DRAMCTRL, 0x0, DRAMCTRL_CTOREQ_HPRI_OPT);
	/* 60759 === DE initial sequence done === */

	/* TODO: DVFS_Enable */

	mcDELAY_US(1);

	/* *((UINT32P)(DDRPHY0AO_BASE + 0x0000)) = 0x00000000; */

	mcDELAY_US(1);

	DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
	/* CH-A */
	vIO32WriteFldMulti(DRAMC_REG_SHUCTRL, P_Fld(0x1, SHUCTRL_R_DRAMC_CHA) | P_Fld(0x0, SHUCTRL_SHU_PHYRST_SEL));
	vIO32WriteFldAlign(DRAMC_REG_SHUCTRL2, 0x1, SHUCTRL2_R_DVFS_DLL_CHA);
	/* CH-B */
	vIO32WriteFldMulti(DRAMC_REG_SHUCTRL + SHIFT_TO_CHB_ADDR, P_Fld(0x0, SHUCTRL_R_DRAMC_CHA) | P_Fld(0x1, SHUCTRL_SHU_PHYRST_SEL));
	vIO32WriteFldAlign(DRAMC_REG_SHUCTRL2 + SHIFT_TO_CHB_ADDR, 0x0, SHUCTRL2_R_DVFS_DLL_CHA);
	/* 60826 ===LP4_3200_intial_setting_shu1 end=== */

#ifndef SYLVIA_TO_BE_PORTING
	/* Does 6771 still require this code snippet? */
#if (fcFOR_CHIP_ID == fcTalbot)
	{
		U8 u1CrossLatch;
		U8 u1Hysteresis;

		u1Hysteresis = (p->frequency > 933) ? 0 : 3;
		vIO32WriteFldAlign_All(DDRPHY_SHU1_B0_DQ3, u1Hysteresis, SHU1_B0_DQ3_RG_TX_ARDQ_PU_PRE_B0);
		vIO32WriteFldAlign_All(DDRPHY_SHU1_B1_DQ3, u1Hysteresis, SHU1_B1_DQ3_RG_TX_ARDQ_PU_PRE_B1);
		/* vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_CMD3, u1Hysteresis, SHU1_CA_CMD3_RG_TX_ARCMD_PU_PRE);  //move to SW impedance */

		u1CrossLatch = (p->frequency > 933) ? 3 : 0;
		vIO32WriteFldAlign_All(DDRPHY_SHU1_B0_DQ6, u1CrossLatch, SHU1_B0_DQ6_RG_ARPI_RESERVE_B0_BIT23);
		vIO32WriteFldAlign_All(DDRPHY_SHU1_B1_DQ6, u1CrossLatch, SHU1_B1_DQ6_RG_ARPI_RESERVE_B1_BIT23);
		vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_CMD6, u1CrossLatch, SHU1_CA_CMD6_RG_ARPI_RESERVE_CA_BIT23); /* DDRPHY_SHU1_CA_CMD6 cannot be broadcast */
	}
#endif
#endif /* SYLVIA_TO_BE_PORTING */

#ifndef LOOPBACK_TEST
	DDRPhyFMeter_Init();
#endif

#if ENABLE_DUTY_SCAN_V2
#ifdef DDR_INIT_TIME_PROFILING
	U32 u4low_tick0, u4high_tick0, u4low_tick1, u4high_tick1;
#if __ETT__
	u4low_tick0 = GPT_GetTickCount( & u4high_tick0);
#else
	u4low_tick0 = get_timer(0);
#endif
#endif

#ifndef DUMP_INIT_RG_LOG_TO_DE
	if (Get_MDL_Used_Flag() == NORMAL_USED)
	{
		DramcNewDutyCalibration(p);
	}
#endif

#ifdef DDR_INIT_TIME_PROFILING
#if __ETT__
	u4low_tick1 = GPT_GetTickCount( & u4high_tick1);
	gu4DutyCalibrationTime = ((u4low_tick1 - u4low_tick0)*76) / 1000000;
#else
	u4low_tick1 = get_timer(u4low_tick0);
	gu4DutyCalibrationTime = u4low_tick1;
#endif
#endif
#endif

	DVFSSettings(p);

	DramcModeRegInit_LP4(p);

}



/* Optimize all-bank refresh parameters (by density) for LP4 */
void vDramcACTimingOptimize(DRAMC_CTX_T *p)
{
	/*
	 * TRFC: tRFCab
	 * Refresh Cycle Time (All Banks)
	 * TXREFCNT: tXSR max((tRFCab + 7.5ns), 2nCK)
	 * Min self refresh time (Entry to Exit)
	 * u1ExecuteOptimize: Indicate if ACTimings are updated at the end of this function
	 */
	U8 u1RFCabGrpIdx = 0, u1FreqGrpIdx = 0, u1ExecuteOptimize = ENABLE;
	U8 u1TRFC = 101, u1TRFC_05T = 0, u1TRFCpb = 44, u1TRFCpb_05T = 0, u1TXREFCNT = 118;
	typedef struct
	{   /* Bitfield sizes set to current project register field's size */
		U8 u1TRFC      : 8;
		U8 u1TRFRC_05T : 1;
		U8 u1TRFCpb      : 8;
		U8 u1TRFRCpb_05T : 1;
		U16 u2TXREFCNT : 10;
	} optimizeACTime;
	/*
	 * JESD209-4B: tRFCab has 4 settings for 7 density settings (130, 180, 280, 380)
	 * tRFCAB_NUM: Used to indicate tRFCab group (since some densities share the same tRFCab)
	 */
	enum tRFCABIdx{tRFCAB_130 = 0, tRFCAB_180, tRFCAB_280, tRFCAB_380, tRFCAB_NUM};
	enum ACTimeIdx{GRP_DDR1600_ACTIM = 0, GRP_DDR2400_ACTIM, GRP_DDR2667_ACTIM, GRP_DDR3200_ACTIM, GRP_DDR3733_ACTIM, GRP_ACTIM_NUM};
	/* Values retrieved from 1. Alaska ACTiming excel file 2. JESD209-4B Refresh requirement table */
	optimizeACTime tRFCab_Opt [GRP_ACTIM_NUM][tRFCAB_NUM] =
	{
		/* For freqGroup DDR1600 */
		{{.u1TRFC = 14, .u1TRFRC_05T = 0, .u1TRFCpb = 0, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 32}, /* tRFCab = 130, tRFCpb = 60 */
		{.u1TRFC = 24, .u1TRFRC_05T = 0, .u1TRFCpb = 6, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 42}, /* tRFCab = 180, tRFCpb = 90 */
		{.u1TRFC = 44, .u1TRFRC_05T = 0, .u1TRFCpb = 16, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 62}, /* tRFCab = 280, tRFCpb = 140 */
		{.u1TRFC = 64, .u1TRFRC_05T = 0, .u1TRFCpb = 26, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 82}}, /* tRFCab = 380, tRFCpb = 190 */
		/* For freqGroup DDR2400 */
		{{.u1TRFC = 27, .u1TRFRC_05T = 0, .u1TRFCpb = 6, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 46}, /* tRFCab = 130, tRFCpb = 60 */
		{.u1TRFC = 42, .u1TRFRC_05T = 0, .u1TRFCpb = 15, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 61}, /* tRFCab = 180, tRFCpb = 90 */
		{.u1TRFC = 72, .u1TRFRC_05T = 0, .u1TRFCpb = 30, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 91}, /* tRFCab = 280, tRFCpb = 140 */
		{.u1TRFC = 102, .u1TRFRC_05T = 0, .u1TRFCpb = 45, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 121}}, /* tRFCab = 380, tRFCpb = 190 */
		/* For freqGroup DDR2667 */
		{{.u1TRFC = 31, .u1TRFRC_05T = 1, .u1TRFCpb = 8, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 50}, /* tRFCab = 130, tRFCpb = 60 */
		{.u1TRFC = 48, .u1TRFRC_05T = 0, .u1TRFCpb = 18, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 67}, /* tRFCab = 180, tRFCpb = 90 */
		{.u1TRFC = 81, .u1TRFRC_05T = 1, .u1TRFCpb = 35, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 100}, /* tRFCab = 280, tRFCpb = 140 */
		{.u1TRFC = 115, .u1TRFRC_05T = 0, .u1TRFCpb = 51, .u1TRFRCpb_05T = 1, .u2TXREFCNT = 134}}, /* tRFCab = 380, tRFCpb = 190 */
		/* For freqGroup DDR3200 */
		{{.u1TRFC = 40, .u1TRFRC_05T = 0, .u1TRFCpb = 12, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 59}, /* tRFCab = 130, tRFCpb = 60 */
		{.u1TRFC = 60, .u1TRFRC_05T = 0, .u1TRFCpb = 24, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 79}, /* tRFCab = 180, tRFCpb = 90 */
		{.u1TRFC = 100, .u1TRFRC_05T = 0, .u1TRFCpb = 44, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 119}, /* tRFCab = 280, tRFCpb = 140 */
		{.u1TRFC = 140, .u1TRFRC_05T = 0, .u1TRFCpb = 64, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 159}}, /* tRFCab = 380, tRFCpb = 190 */
		/* For freqGroup DDR3733 */
		{{.u1TRFC = 48, .u1TRFRC_05T = 1, .u1TRFCpb = 16, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 68}, /* tRFCab = 130, tRFCpb = 60 */
		{.u1TRFC = 72, .u1TRFRC_05T = 0, .u1TRFCpb = 30, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 92}, /* tRFCab = 180, tRFCpb = 90 */
		{.u1TRFC = 118, .u1TRFRC_05T = 1, .u1TRFCpb = 53, .u1TRFRCpb_05T = 1, .u2TXREFCNT = 138}, /* tRFCab = 280, tRFCpb = 140 */
		{.u1TRFC = 165, .u1TRFRC_05T = 0, .u1TRFCpb = 76, .u1TRFRCpb_05T = 1, .u2TXREFCNT = 185}} /* tRFCab = 380, tRFCpb = 190 */
	};

	enum tRFCABIdx_LP3{tRFCAB_130_LP3 = 0, tRFCAB_210_LP3, tRFCAB_NUM_LP3};
	enum ACTimeIdx_LP3{GRP_DDR1200_ACTIM_LP3 = 0, GRP_DDR1333_ACTIM_LP3, GRP_DDR1600_ACTIM_LP3, GRP_DDR1866_ACTIM_LP3, GRP_ACTIM_NUM_LP3};
	/* Values retrieved from 1. Alaska ACTiming excel file 2. JESD209-4B Refresh requirement table */
	optimizeACTime tRFCab_Opt_LP3 [GRP_ACTIM_NUM_LP3][tRFCAB_NUM_LP3] =
	{
		/* For freqGroup DDR1200 */
		{{.u1TRFC = 27, .u1TRFRC_05T = 0, .u1TRFCpb = 6, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 40}, /* tRFCab = 130, tRFCpb = 60 */
		{.u1TRFC = 51, .u1TRFRC_05T = 0, .u1TRFCpb = 15, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 64}}, /* tRFCab = 210, tRFCpb = 90 */
		/* For freqGroup DDR1333 */
		{{.u1TRFC = 31, .u1TRFRC_05T = 0, .u1TRFCpb = 8, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 45}, /* tRFCab = 130, tRFCpb = 60 */
		{.u1TRFC = 58, .u1TRFRC_05T = 0, .u1TRFCpb = 18, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 72}}, /* tRFCab = 210, tRFCpb = 90 */
		/* For freqGroup DDR1600 */
		{{.u1TRFC = 40, .u1TRFRC_05T = 0, .u1TRFCpb = 12, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 54}, /* tRFCab = 130, tRFCpb = 60 */
		{.u1TRFC = 72, .u1TRFRC_05T = 0, .u1TRFCpb = 24, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 86}}, /* tRFCab = 210, tRFCpb = 90 */
		/* For freqGroup DDR1866 */
		{{.u1TRFC = 49, .u1TRFRC_05T = 0, .u1TRFCpb = 16, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 64}, /* tRFCab = 130, tRFCpb = 60 */
		{.u1TRFC = 86, .u1TRFRC_05T = 0, .u1TRFCpb = 30, .u1TRFRCpb_05T = 0, .u2TXREFCNT = 101}}, /* tRFCab = 210, tRFCpb = 90 */
	};


	mcSHOW_DBG_MSG(("[ACTimingOptimize]"));

#if __ETT__
	if (p->density == 0xff)
	{
		mcSHOW_ERR_MSG(("Error : No call MR8 to get density!!\n"));
		while (1);
	}
#endif

	if (u1IsLP4Family(p->dram_type))  /* already read MR8 for density update */
	{
		/* Set tRFCab group idx p->density = MR8 OP[5:2] */
		switch (p->density)
		{
			case 0x0:   /* 4Gb per die  (2Gb per channel), tRFCab = 130 */
				u1RFCabGrpIdx = tRFCAB_130;
				break;
			case 0x1:   /* 6Gb per die  (3Gb per channel), tRFCab = 180 */
				case 0x2:   /* 8Gb per die  (4Gb per channel), tRFCab = 180 */
				u1RFCabGrpIdx = tRFCAB_180;
				break;
			case 0x3:   /* 12Gb per die (6Gb per channel), tRFCab = 280 */
				case 0x4:   /* 16Gb per die (8Gb per channel), tRFCab = 280 */
				u1RFCabGrpIdx = tRFCAB_280;
				break;
			case 0x5:   /* 24Gb per die (12Gb per channel), tRFCab = 380 */
				case 0x6:   /* 32Gb per die (16Gb per channel), tRFCab = 380 */
				u1RFCabGrpIdx = tRFCAB_380;
				break;
			default:
				u1ExecuteOptimize = DISABLE;
				mcSHOW_ERR_MSG(("MR8 density err!\n"));
			}
		/* Set freqGroup Idx */
		switch (p->freqGroup)
		{
			case 800:
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
			default:
				u1ExecuteOptimize = DISABLE;
				mcSHOW_ERR_MSG(("freqGroup err!\n"));
			}

		u1TRFC = tRFCab_Opt[u1FreqGrpIdx][u1RFCabGrpIdx].u1TRFC;
		u1TRFC_05T = tRFCab_Opt[u1FreqGrpIdx][u1RFCabGrpIdx].u1TRFRC_05T;
		u1TRFCpb = tRFCab_Opt[u1FreqGrpIdx][u1RFCabGrpIdx].u1TRFCpb;
		u1TRFCpb_05T = tRFCab_Opt[u1FreqGrpIdx][u1RFCabGrpIdx].u1TRFRCpb_05T;
		u1TXREFCNT = tRFCab_Opt[u1FreqGrpIdx][u1RFCabGrpIdx].u2TXREFCNT;
	}
	else
	{
		/* Set tRFCab group idx p->density = MR8 OP[5:2] */
		switch (p->density)
		{
			case 0x4:  /* 1Gb per die, tRFCab = 130 */
				case 0x5:  /* 2Gb per die, tRFCab = 130 */
				case 0x6:  /* 4Gb per die, tRFCab = 130 */
				u1RFCabGrpIdx = tRFCAB_130_LP3;
				break;
			case 0xE:  /* 6Gb per die, tRFCab = 210 */
				case 0x7:  /* 8Gb per die, tRFCab = 210 */
				u1RFCabGrpIdx = tRFCAB_210_LP3;
				break;
			default:
				u1ExecuteOptimize = DISABLE;
				mcSHOW_ERR_MSG(("MR8 density err!\n"));
			}
		/* Set freqGroup Idx */
		switch (p->freqGroup)
		{
			case 600:
				u1FreqGrpIdx = GRP_DDR1200_ACTIM_LP3;
				break;
			case 667:
				u1FreqGrpIdx = GRP_DDR1333_ACTIM_LP3;
				break;
			case 800:
				u1FreqGrpIdx = GRP_DDR1600_ACTIM_LP3;
				break;
			case 933:
				u1FreqGrpIdx = GRP_DDR1866_ACTIM_LP3;
				break;
			default:
				u1ExecuteOptimize = DISABLE;
				mcSHOW_ERR_MSG(("LP3 freqGroup err!\n"));
			}

		u1TRFC = tRFCab_Opt_LP3[u1FreqGrpIdx][u1RFCabGrpIdx].u1TRFC;
		u1TRFC_05T = tRFCab_Opt_LP3[u1FreqGrpIdx][u1RFCabGrpIdx].u1TRFRC_05T;
		u1TRFCpb = tRFCab_Opt_LP3[u1FreqGrpIdx][u1RFCabGrpIdx].u1TRFCpb;
		u1TRFCpb_05T = tRFCab_Opt_LP3[u1FreqGrpIdx][u1RFCabGrpIdx].u1TRFRCpb_05T;
		u1TXREFCNT = tRFCab_Opt_LP3[u1FreqGrpIdx][u1RFCabGrpIdx].u2TXREFCNT;
	}

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

	return DRAM_OK;
}

/* ------------------------------------------------------------------------- */
/* * UpdateACTimingReg()
 * ACTiming related register field update
 * @param p                Pointer of context created by DramcCtxCreate.
 * @param  ACTbl           Pointer to correct ACTiming table struct
 * @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL
 */
/* ------------------------------------------------------------------------- */
DRAM_STATUS_T DdrUpdateACTimingReg(DRAMC_CTX_T *p, const ACTime_T *ACTbl)
{
	ACTime_T ACTblFinal; /* ACTblFinal: Use to set correct ACTiming values and write into registers */
	DRAM_ODT_MODE_T r2w_odt_onoff = p->odt_onoff; /* Variable used in step 1 (decide to use odt on or off ACTiming) */
	/*
	 * ACTiming regs that have ODT on/off values -> declare variables to save the wanted value
	 * -> Used to retrieve correct SHU_ACTIM2_TR2W value and write into final register field
	 */
	U8 u1TRTW, u1TRTW_05T, u1TMRR2W;

#ifdef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
	U8 u1RANKINCTL = 0;
#endif
	U8 RODT_TRACKING_SAVEING_MCK = 0, u1ROOT = 0, u1TXRANKINCTL = 0, u1TXDLY = 0, u1DATLAT_DSEL = 0; /* Used to store tmp ACTiming values */

#if SAMSUNG_LP4_NWR_WORKAROUND
	U8 u1TWTR = 0, u1TWTR_05T = 0, u1TWTR_TMP = 0;
#endif
	/* ACTiming regs that aren't currently in ACTime_T struct */
	U8 u1TREFBW = 0; /* REFBW_FR (tREFBW) for LP3, REFBW_FR = 0 & TREFBWIG = 1 (by CF) */

	U16 u2TXREFCNT = 0, u2tRFCabTiming = 380, u2tRFCpbTiming = 190;
	U8 u1DivMode = 3, u1MCKtoTck = 4;
	U8 u1tRFCab = 0, u1tRFCab_05T = 0, u1tRFCpb = 0, u1tRFCpb_05T = 0;

	if (ACTbl == NULL)
		return DRAM_FAIL;
	ACTblFinal = *ACTbl;

	/* ----Step 1: Perform ACTiming table adjustments according to different usage/scenarios-------------------------- */
#if ENABLE_TX_WDQS
	r2w_odt_onoff = ODT_ON;
#else
	r2w_odt_onoff = p->odt_onoff;
#endif

	/* ACTimings that have different values for odt on/off, retrieve the correct one and store in local variable */
	if (r2w_odt_onoff == ODT_ON) /* odt_on */
	{
		u1TRTW = ACTblFinal.trtw_ODT_on;
		u1TRTW_05T = ACTblFinal.trtw_ODT_on_05T;
		u1TMRR2W = ACTblFinal.tmrr2w_ODT_on;

	}
	else /* odt_off */
	{
		u1TRTW = ACTblFinal.trtw_ODT_off;
		u1TRTW_05T = ACTblFinal.trtw_ODT_off_05T;
		u1TMRR2W = ACTblFinal.tmrr2w_ODT_off;
	}

	/* Override the above tRTW & tRTW_05T selection for Hynix LPDDR4P dram (always use odt_on's value for tRTW) */
	if ((p->dram_type == TYPE_LPDDR4P) && (p->vendor_id == VENDOR_HYNIX)) /* !SUPPORT_HYNIX_RX_DQS_WEAK_PULL (temp solution, need to discuss with SY) */
	{
		u1TRTW = ACTblFinal.trtw_ODT_on;
		u1TRTW_05T = ACTblFinal.trtw_ODT_on_05T;
	}

#if ENABLE_RODT_TRACKING_SAVE_MCK
	/* for rodt tracking save 1 MCK and rodt tracking enable or not(RODTEN_MCK_MODESEL) */
	u1ODT_ON = p->odt_onoff;
	u1RODT_TRACK = ENABLE_RODT_TRACKING;
	u1ROEN = u1WDQS_ON | u1ODT_ON;
	u1ModeSel = u1RODT_TRACK & u1ROEN;

	/*
	 * when WDQS on and RODT Track define open and un-term, RODT_TRACKING_SAVEING_MCK = 1 for the future setting
	 * Maybe "Save 1 MCK" will be set after Vinson project, but 6763 & Vin_son can not.(different with performance team)
	 * if (u1RODT_TRACK && (u1ROEN==1))
	 * RODT_TRACKING_SAVEING_MCK = 1;
	 */
#endif

#if (ENABLE_RODT_TRACKING || defined(XRTR2W_PERFORM_ENHANCE_RODTEN))
	/*
	 * yr: same code
	 * // set to 0, let TRTW & XRTR2W setting values are the smae with DV-sim's value that DE provided
	 * if (r2w_odt_onoff == ODT_ON) RODT_TRACKING_SAVEING_MCK = 0; //RODT_TRACKING eanble can save r2w 1 MCK
	 * else RODT_TRACKING_SAVEING_MCK = 0;
	 */
	RODT_TRACKING_SAVEING_MCK = 0;
#endif

	/* Update values that are used by RODT_TRACKING_SAVEING_MCK */
	u1TRTW = u1TRTW - RODT_TRACKING_SAVEING_MCK;
	ACTblFinal.xrtr2w = ACTblFinal.xrtr2w - RODT_TRACKING_SAVEING_MCK;

#if SAMSUNG_LP4_NWR_WORKAROUND
	/* If nWR is fixed to 30 for all freqs, tWTR@800Mhz should add 2tCK gap, allowing sufficient Samsung DRAM internal IO precharge time */
	if (u1IsLP4Family(p->dram_type) && (p->vendor_id == VENDOR_SAMSUNG) && (p->frequency == 800)) /* LP4X, Samsung, DDR1600 */
	{
		u1TWTR_TMP = (ACTblFinal.twtr * 4 - ACTblFinal.twtr_05T * 2) + 2; /* Convert TWTR to tCK, and add 2tCK */
		if ((u1TWTR_TMP % 4) == 0) /* TWTR can be transferred to TWTR directly */
		{
			u1TWTR = u1TWTR_TMP >> 2;
			u1TWTR_05T = 0;
		}
		else /* Can't be transfered to TWTR directly */
		{
			u1TWTR = (u1TWTR_TMP + 2) >> 2; /* Add 2 tCK and set TWTR value (Then minus 2tCK using 05T) */
			u1TWTR_05T = 1;  /* 05T means minus 2tCK */
		}

		ACTblFinal.twtr = u1TWTR;
		ACTblFinal.twtr_05T = u1TWTR_05T;
	}
#endif

#ifdef BYPASS_RXPIPE_EN
	ACTblFinal.datlat--;
	u1DATLAT_DSEL = ACTblFinal.datlat - 1;
#else
	/* DATLAT related */
	u1DATLAT_DSEL = ACTblFinal.datlat - 2;
#endif

	/* XRT performance enhancement */
#ifndef XRTW2W_PERFORM_ENHANCE_TX
	if (u1IsLP4Family(p->dram_type))
	{
		ACTblFinal.xrtw2w = 6;
	}
	else
	{
		ACTblFinal.xrtw2w = 7;
	}
#endif

#ifndef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
	if (u1IsLP4Family(p->dram_type))
	{
		ACTblFinal.xrtr2r = 12;
	}
#endif

#if FIX_CROSSRK_XRT_05T_OPT
	ACTblFinal.xrtr2r -= 1;
#endif

#if DramcHWDQSGatingTracking_DVT_JADE_TRACKING_MODE
	if (!u1IsLP4Family(p->dram_type))
	{
		u1TRTW += 1;
		ACTblFinal.xrtw2r += 3;
	}
#endif

#if TX_OE_EXTEND
	if (u1IsLP4Family(p->dram_type))
	{
		ACTblFinal.xrtw2w += 1;
		ACTblFinal.xrtw2r += 1;
	}
#endif

	// tRFC formula start============================================================
	if (u1IsLP4Family(p->dram_type))
	{
		// tRFCab = 380ns
		UpdatetRFCMaxTiming(p, u2tRFCabTiming, u1DivMode, u1MCKtoTck, &u1tRFCab, &u1tRFCab_05T);

		// TXREFCNT
		u2TXREFCNT = ((u2tRFCabTiming*10+75)*1000)/(1000000000/(((p->frequency*2)*100) >> u1DivMode)); //tRFCab max is 380ns for 12Gb up
		if (((u2tRFCabTiming*10+75)*1000)%(1000000000/(((p->frequency*2)*100) >> u1DivMode)) != 0) // Round up
			u2TXREFCNT++;
		u2TXREFCNT = u2TXREFCNT - 3 + 7; // -3 + 7 by formula

		// tRFCpb = 190ns
		UpdatetRFCMaxTiming(p, u2tRFCpbTiming, u1DivMode, u1MCKtoTck, &u1tRFCpb, &u1tRFCpb_05T);

		mcSHOW_DBG_MSG(("[DdrUpdateACTimingReg] Freq = %d, tRFCab = %d, tRFCab_05T = %d, TXREFCNT = %d, tRFCpb = %d, tRFCpb_05T = %d\n", p->frequency, u1tRFCab, u1tRFCab_05T, u2TXREFCNT, u1tRFCpb, u1tRFCpb_05T));
	}
	else // LPDDR3
	{
		u1tRFCab = ACTblFinal.trfc;
		u1tRFCab_05T = ACTblFinal.trfc_05T;
		u2TXREFCNT = ACTblFinal.txrefcnt;
		u1tRFCpb = ACTblFinal.trfcpb;
		u1tRFCpb_05T = ACTblFinal.trfcpb_05T;
	}
	// =========================================================== tRFC formula end

	/* ----Step 2: Perform register writes for entries in ACTblFinal struct & ACTiming excel file (all actiming adjustments should be done in Step 1)------- */
	vIO32WriteFldMulti_All(DRAMC_REG_SHU_ACTIM0, P_Fld(ACTblFinal.trcd, SHU_ACTIM0_TRCD)
		| P_Fld(ACTblFinal.trrd, SHU_ACTIM0_TRRD)
		| P_Fld(ACTblFinal.twr, SHU_ACTIM0_TWR)
		| P_Fld(ACTblFinal.twtr, SHU_ACTIM0_TWTR));

	vIO32WriteFldMulti_All(DRAMC_REG_SHU_ACTIM1, P_Fld(ACTblFinal.trc, SHU_ACTIM1_TRC)
		| P_Fld(ACTblFinal.tras, SHU_ACTIM1_TRAS)
		| P_Fld(ACTblFinal.trp, SHU_ACTIM1_TRP)
		| P_Fld(ACTblFinal.trpab, SHU_ACTIM1_TRPAB));

#ifdef BYPASS_TXPIPE_EN
	if ((u1IsLP4Family(p->dram_type) && (p->frequency >= 1600)) ||
		((p->dram_type == TYPE_LPDDR3) && (p->frequency >= 800) && !p->bDLP3))
	{
		if (ACTblFinal.txp > 0)
			ACTblFinal.txp--;
		ACTblFinal.ckeprd++;
	}
#endif
	vIO32WriteFldMulti_All(DRAMC_REG_SHU_ACTIM2, P_Fld(ACTblFinal.tfaw, SHU_ACTIM2_TFAW)
		| P_Fld(u1TRTW, SHU_ACTIM2_TR2W) /* Value has odt_on / off difference, use local variable u1TRTW */
		| P_Fld(ACTblFinal.trtp, SHU_ACTIM2_TRTP)
		| P_Fld(ACTblFinal.txp, SHU_ACTIM2_TXP));

	vIO32WriteFldMulti_All(DRAMC_REG_SHU_ACTIM3, P_Fld(u1tRFCab, SHU_ACTIM3_TRFC)
		| P_Fld(ACTblFinal.refcnt, SHU_ACTIM3_REFCNT)
		| P_Fld(u1tRFCpb, SHU_ACTIM3_TRFCPB));

	vIO32WriteFldMulti_All(DRAMC_REG_SHU_ACTIM4, P_Fld(ACTblFinal.tzqcs, SHU_ACTIM4_TZQCS)
		| P_Fld(ACTblFinal.refcnt_fr_clk, SHU_ACTIM4_REFCNT_FR_CLK)
		| P_Fld(u2TXREFCNT, SHU_ACTIM4_TXREFCNT));

	vIO32WriteFldMulti_All(DRAMC_REG_SHU_ACTIM5, P_Fld(u1TMRR2W, SHU_ACTIM5_TMRR2W)  /* Value has odt_on / off difference, use local variable u1TMRR2W */
		| P_Fld(ACTblFinal.twtpd, SHU_ACTIM5_TWTPD)
		| P_Fld(ACTblFinal.trtpd, SHU_ACTIM5_TR2PD));

	vIO32WriteFldMulti_All(DRAMC_REG_SHU_ACTIM_XRT, P_Fld(ACTblFinal.xrtw2w, SHU_ACTIM_XRT_XRTW2W)
		| P_Fld(ACTblFinal.xrtw2r, SHU_ACTIM_XRT_XRTW2R)
		| P_Fld(ACTblFinal.xrtr2w, SHU_ACTIM_XRT_XRTR2W)
		| P_Fld(ACTblFinal.xrtr2r, SHU_ACTIM_XRT_XRTR2R));

	/* AC timing 0.5T */
	vIO32WriteFldMulti_All(DRAMC_REG_SHU_AC_TIME_05T, P_Fld(ACTblFinal.twtr_05T, SHU_AC_TIME_05T_TWTR_M05T)
		| P_Fld(u1TRTW_05T, SHU_AC_TIME_05T_TR2W_05T) /* Value has odt_on / off difference, use local variable u1TRTW */
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

	/*
	 * CATRAIN_INTV isn't a shuffle register, but only affects LP4 CBT timing intv. during calibration
	 * Berson: CATRAIN_INTV is calculated based on CATRAINLAT = 0
	 */
	vIO32WriteFldMulti_All(DRAMC_REG_CATRAINING1, P_Fld(ACTblFinal.r_dmcatrain_intv, CATRAINING1_CATRAIN_INTV)
		| P_Fld(0x0, CATRAINING1_CATRAINLAT));

	/* DQSINCTL related */
	vIO32WriteFldAlign_All(DRAMC_REG_SHURK0_DQSCTL, ACTblFinal.dqsinctl, SHURK0_DQSCTL_DQSINCTL); /* Rank 0 DQSINCTL */
	vIO32WriteFldAlign_All(DRAMC_REG_SHURK1_DQSCTL, ACTblFinal.dqsinctl, SHURK1_DQSCTL_R1DQSINCTL); /* Rank 1 DQSINCTL */

	vIO32WriteFldAlign_All(DRAMC_REG_SHU_ODTCTRL, ACTblFinal.dqsinctl, SHU_ODTCTRL_RODT);

	/* DATLAT related, tREFBW */
	vIO32WriteFldMulti_All(DRAMC_REG_SHU_CONF1, P_Fld(ACTblFinal.datlat, SHU_CONF1_DATLAT)
		| P_Fld(u1DATLAT_DSEL, SHU_CONF1_DATLAT_DSEL)
		| P_Fld(u1DATLAT_DSEL, SHU_CONF1_DATLAT_DSEL_PHY)
		| P_Fld(u1TREFBW, SHU_CONF1_REFBW_FR));

	/* FSPCHG_PRDCNT: LPDDR4 tFC constraint (LP3 unused, set to default value 0) */
	vIO32WriteFldAlign_All(DRAMC_REG_SHU_CONF2, ACTblFinal.r_dmfspchg_prdcnt, SHU_CONF2_FSPCHG_PRDCNT);

	/*
	 * TODO: MRW_INTV can be set to different values for each freq, request new forumula/values from Berson
	 * (Currently set to 0xF for both LP3/LP4 due to YH Tsai's M17 golden settings review)
	 */
	vIO32WriteFldMulti_All(DRAMC_REG_SHU_SCINTV, P_Fld(ACTblFinal.r_dmmrw_intv, SHU_SCINTV_MRW_INTV)
		| P_Fld(ACTblFinal.zqlat2, SHU_SCINTV_TZQLAT2));

	/* CKEPRD - CKE pulse width */

	vIO32WriteFldAlign_All(DRAMC_REG_SHU_CKECTRL, ACTblFinal.ckeprd, SHU_CKECTRL_CKEPRD);

	/* CKELCKCNT: Valid clock requirement after CKE input low */
	vIO32WriteFldAlign_All(DRAMC_REG_CKECTRL, ACTblFinal.ckelckcnt, CKECTRL_CKELCKCNT);

	/* ----Step 3: Perform register writes/calculation for other regs (That aren't in ACTblFinal struct)------------------------------------------------ */
#ifdef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
	/*
	 * Wei-Jen: Ininital setting values are the same, RANKINCTL_RXDLY = RANKINCTL = RANKINCTL_ROOT1
	 * XRTR2R setting will be updated in RxdqsGatingPostProcess
	 */
	u1RANKINCTL = u4IO32ReadFldAlign(DRAMC_REG_SHU_RANKCTL, SHU_RANKCTL_RANKINCTL);
	vIO32WriteFldAlign_All(DRAMC_REG_SHU_RANKCTL, u1RANKINCTL, SHU_RANKCTL_RANKINCTL_RXDLY);
#endif

	/* Update releated RG of XRTW2W */
#ifdef XRTW2W_PERFORM_ENHANCE_TX
	if (u1IsLP4Family(p->dram_type))
	{
		if (p->frequency <= 800)
		{
			u1ROOT = 0; u1TXRANKINCTL = 0; u1TXDLY = 1;
		}
		else
		{
			u1ROOT = (p->frequency == 1866) ? 1:0;
			u1TXRANKINCTL = 1; u1TXDLY = 2;
		}
	}
	else
	{
		if (p->frequency <= 667)    /* LP3 - 1333 */
		{
			u1ROOT = 0; u1TXRANKINCTL = 0; u1TXDLY = 0;
		}
		else
		{
			u1ROOT = 0; u1TXRANKINCTL = 1; u1TXDLY = 2;
		}
	}
#else
	if (u1IsLP4Family(p->dram_type))
	{
		if (p->frequency <= 800)
		{
			u1ROOT = 0; u1TXRANKINCTL = 0; u1TXDLY = 0;
		}
		else
		{
			u1ROOT = 0; u1TXRANKINCTL = 1; u1TXDLY = 1;
		}
	}
	else
	{
		if (p->frequency <= 667)    /* LP3 - 1333 */
		{
			u1ROOT = 0; u1TXRANKINCTL = 0; u1TXDLY = 0;
		}
		else
		{
			u1ROOT = 0; u1TXRANKINCTL = 1; u1TXDLY = 1;
		}
	}
#endif
	vIO32WriteFldMulti_All(DRAMC_REG_SHU_RANKCTL, P_Fld(u1ROOT, SHU_RANKCTL_TXRANKINCTL_ROOT)
		| P_Fld(u1TXRANKINCTL, SHU_RANKCTL_TXRANKINCTL)
		| P_Fld(u1TXDLY, SHU_RANKCTL_TXRANKINCTL_TXDLY));

	return DRAM_OK;
}


/* ------------------------------------------------------------------------- */
/* * u1GetACTimingIdx()
 * Retrieve internal ACTimingTbl's index according to dram type, freqGroup, Read DBI status
 * @param p                Pointer of context created by DramcCtxCreate.
 * @retval u1TimingIdx     Return ACTimingTbl entry's index
 */
/* ------------------------------------------------------------------------- */
static U8 u1GetACTimingIdx(DRAMC_CTX_T *p)
{
	U8 u1TimingIdx = 0xff, u1TmpIdx;
	U8 u1TmpDramType = p->dram_type;

	/* LP4/LP4P/LP4X use same table */
	if (u1TmpDramType == TYPE_LPDDR4X || u1TmpDramType == TYPE_LPDDR4P)
		u1TmpDramType = TYPE_LPDDR4;

	for (u1TmpIdx = 0; u1TmpIdx < TOTAL_AC_TIMING_NUMBER; u1TmpIdx++)
	{
		if ((ACTimingTbl[u1TmpIdx].dramType == u1TmpDramType) &&
			/* p->frequency may not be in ACTimingTable, use p->freqGroup */
			(ACTimingTbl[u1TmpIdx].freq == p->freqGroup) &&
			(ACTimingTbl[u1TmpIdx].readDBI == p->DBI_R_onoff[p->dram_fsp]) &&
			((ACTimingTbl[u1TmpIdx].cbtMode == vGet_Dram_CBT_Mode(p)) || !(u1IsLP4Family(p->dram_type)))
			/* LP4 byte/mixed mode dram both use byte mode ACTiming, LP3 skip CBT mode check */
			)
			{
				u1TimingIdx = u1TmpIdx;
			mcSHOW_DBG_MSG(("match AC timing %d\n", u1TimingIdx));
			mcFPRINTF((fp_A60501, "match AC timing %d\n", u1TimingIdx));
			break;
		}
	}

	return u1TimingIdx;
}

DRAM_STATUS_T DdrUpdateACTiming(DRAMC_CTX_T *p)
{
	U8 u1TimingIdx = 0;

	mcSHOW_DBG_MSG3(("[UpdateACTiming]\n"));

	/* Retrieve ACTimingTable's corresponding index */
	u1TimingIdx = u1GetACTimingIdx(p);

	if (u1TimingIdx == 0xff)
	{
	#if 0
		if (u1TmpDramType = TYPE_LPDDR4)
			u1TimingIdx = 0;
		else /* LPDDR3 */
			u1TimingIdx = 6;
		mcSHOW_ERR_MSG(("Error, no match AC timing, use default timing %d\n", u1TimingIdx));
		mcFPRINTF((fp_A60501, "Error, no match AC timing, use default timing %d\n", u1TimingIdx));
	#else
		mcSHOW_ERR_MSG(("Error, no match AC timing, not apply table\n"));
		mcFPRINTF((fp_A60501, "Error, no match AC timing, not apply table\n"));
		return DRAM_FAIL;
	#endif
	}

	/* Set ACTiming registers */
	DdrUpdateACTimingReg(p, & ACTimingTbl[u1TimingIdx]);

	return DRAM_OK;
}

#if ((!SW_CHANGE_FOR_SIMULATION) && (!FOR_DV_SIMULATION_USED && SW_CHANGE_FOR_SIMULATION == 0) && (!__ETT__))
DRAM_STATUS_T DdrUpdateACTiming_EMI(DRAMC_CTX_T *p, AC_TIMING_EXTERNAL_T *ACRegFromEmi)
{
	U8 u1TimingIdx = 0;
	ACTime_T ACTime;
	mcSHOW_DBG_MSG3(("[DdrUpdateACTiming_EMI]\n"));

	if (ACRegFromEmi == NULL)
		return DRAM_FAIL;

	/* Retrieve ACTimingTable's corresponding index */
	u1TimingIdx = u1GetACTimingIdx(p);
	ACTime = ACTimingTbl[u1TimingIdx];

	/* Overwrite AC timing from emi settings */
	ACTime.dramType = p->dram_type;
#if 1 /* Will use MDL ac timing, Others from internal ac timing */
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

	ACTime.refcnt = ACRegFromEmi->AC_TIME_EMI_REFCNT; /* (REFFRERUN = 0) */
	ACTime.refcnt_fr_clk = ACRegFromEmi->AC_TIME_EMI_REFCNT_FR_CLK; /* (REFFRERUN = 1) */
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

	/* Set ACTiming registers */
	DdrUpdateACTimingReg(p, & ACTime);

	return DRAM_OK;
}
#endif


/* vDramcInit_PreSettings(): Initial register settings(which are required to be set before all calibration flow) */
void vDramcInit_PreSettings(DRAMC_CTX_T *p)
{
	if (u1IsLP4Family(p->dram_type))
	{   /* PAD_RRESETB control sequence */
		/* remove twice dram reset pin pulse before dram power on sequence flow */
		vIO32WriteFldMulti(DDRPHY_CA_CMD8, P_Fld(0x0, CA_CMD8_RG_TX_RRESETB_PULL_UP) | P_Fld(0x0, CA_CMD8_RG_TX_RRESETB_PULL_DN)
			| P_Fld(0x1, CA_CMD8_RG_TX_RRESETB_DDR3_SEL) | P_Fld(0x0, CA_CMD8_RG_TX_RRESETB_DDR4_SEL)
			| P_Fld(0xa, CA_CMD8_RG_RRESETB_DRVP) | P_Fld(0xa, CA_CMD8_RG_RRESETB_DRVN));
		vIO32WriteFldAlign(DDRPHY_MISC_CTRL1, 0x1, MISC_CTRL1_R_DMRRESETB_I_OPT); /* Change to glitch - free path */
		/* replace DDRCONF0_GDDR3RST with MISC_CTRL1_R_DMDA_RRESETB_I */
		vIO32WriteFldAlign(DDRPHY_MISC_CTRL1, 0x0, MISC_CTRL1_R_DMDA_RRESETB_I);
		vIO32WriteFldAlign(DDRPHY_MISC_CTRL1, 0x1, MISC_CTRL1_R_DMDA_RRESETB_E);
	}

#if __ETT__
	if (u1IsLP4Family(p->dram_type) && (GetDramcBroadcast() == DRAMC_BROADCAST_OFF))
	{   /* In this function, broadcast is assumed to be ON(LP4) -> Show error if broadcast is OFF */
		mcSHOW_ERR_MSG(("Err! Broadcast is OFF!\n"));
	}
#endif
	return;
}

DRAM_STATUS_T DramcInit(DRAMC_CTX_T *p)
{
	U32 save_ch, dram_t, highest_freq = 0; /* Darren */
#if (!__ETT__ && !FOR_DV_SIMULATION_USED && SW_CHANGE_FOR_SIMULATION == 0)
	EMI_SETTINGS *emi_set; /* Darren */
#endif
	U8 dram_cbt_mode;

	show_msg_with_timestamp(("[TimeStamp] Start Dramc & DDRPHY init\n"));

#ifdef FOR_HQA_REPORT_USED
	if (gHQALog_flag == 1)
	{
		mcSHOW_DBG_MSG(("[HQA] Log parsing, "));
		mcSHOW_DBG_MSG(("\tDram Data rate = %d\n", p->frequency*2));
	}
#endif

	mcSHOW_DBG_MSG(("\n[DramcInit]\n"));
	mcFPRINTF((fp_A60501, "\n[DramcInit] Begin\n"));

	save_ch = vGetPHY2ChannelMapping(p);
	vSetPHY2ChannelMapping(p, CHANNEL_A);

#if WRITE_LEVELING_MOVE_DQS_INSTEAD_OF_CLK
	memset(p->arfgWriteLevelingInitShif, FALSE, sizeof (p->arfgWriteLevelingInitShif));
#endif
#if TX_PERBIT_INIT_FLOW_CONTROL
	memset(p->fgTXPerbifInit, FALSE, sizeof (p->fgTXPerbifInit));
#endif

	if (u1IsLP4Family(p->dram_type))
	{
		highest_freq = 1600; /* Darren */
		DramcSetting_Olympus_LP4_ByteMode(p);
	}
#if ENABLE_LP3_SW
	else if (p->dram_type == TYPE_LPDDR3)
	{
		highest_freq = 933; /* Darren */
		DramcSetting_Olympus_LP3(p);
	}
#endif

#if (!__ETT__ && !FOR_DV_SIMULATION_USED && SW_CHANGE_FOR_SIMULATION == 0)
	if ((p->dram_type == TYPE_LPDDR3) && (p->frequency == highest_freq)) /* For LP3 Only */
	{
		emi_set = get_emi_setting();

		if (emi_set->AcTimeEMI.AC_TIME_EMI_FREQUENCY == p->frequency)
		{
			DdrUpdateACTiming_EMI(p, & (emi_set->AcTimeEMI));
		}
		else
		{
			mcSHOW_DBG_MSG(("[DdrUpdateACTiming_EMI] frequency not match (MDL = %d, target = %d), use inner table setting\n", emi_set->AcTimeEMI.AC_TIME_EMI_FREQUENCY, p->frequency));
			DdrUpdateACTiming(p);
		}
	}
	else
#endif
	{
		/* if(u1IsLP4Family(p->dram_type) && (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1))  // tmp solution, need to take care normal mode and DBI */
		DdrUpdateACTiming(p);
	}


#if 0  /* update refresh rate */
	/* for free-run clk 26MHz, 0x62 * (1/26) = 3.8ns */
	vIO32WriteFldAlign_All(DRAMC_REG_DRAMC_PD_CTRL, 0x62, DRAMC_PD_CTRL_REFCNT_FR_CLK);
	/* for non-fre-run clk,  reg = 3.8 ns * f / 4 / 16; */
	u4RefreshRate = 38*p->frequency / 640;
	vIO32WriteFldAlign_All(DRAMC_REG_CONF2, u4RefreshRate, CONF2_REFCNT);
#endif

#if (fcFOR_CHIP_ID == fcTalbot)
	/*
	 * For kernel api for check LPDDR3/4/4X (Darren), only for fcOlympus and fcElbrus.
	 * For Other chip, please confirm the register is free for SW use.
	 */
	vSetPHY2ChannelMapping(p, CHANNEL_A);

	switch (p->dram_type)
	{
	#if ENABLE_LP3_SW
		case TYPE_LPDDR3:
			dram_t = 1;
			break;
	#endif /* ENABLE_LP3_SW */
		case TYPE_LPDDR4:
			dram_t = 2;
			break;
		case TYPE_LPDDR4X:
			dram_t = 3;
			break;
		case TYPE_LPDDR4P:
			dram_t = 4;
			break;
		default:
			dram_t = 0;
			mcSHOW_ERR_MSG(("Incorrect DRAM Type!\n"));
			break;
	}
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_ARBCTL), dram_t, ARBCTL_RSV_DRAM_TYPE);

	/* For DRAM normal, byte and mixed mode */
	if ((p->dram_cbt_mode[RANK_0] == CBT_NORMAL_MODE) && (p->dram_cbt_mode[RANK_1] == CBT_NORMAL_MODE))
		dram_cbt_mode = CBT_R0_R1_NORMAL;
	else if ((p->dram_cbt_mode[RANK_0] == CBT_BYTE_MODE1) && (p->dram_cbt_mode[RANK_1] == CBT_BYTE_MODE1))
		dram_cbt_mode = CBT_R0_R1_BYTE;
	else if ((p->dram_cbt_mode[RANK_0] == CBT_NORMAL_MODE) && (p->dram_cbt_mode[RANK_1] == CBT_BYTE_MODE1))
		dram_cbt_mode = CBT_R0_NORMAL_R1_BYTE;
	else if ((p->dram_cbt_mode[RANK_0] == CBT_BYTE_MODE1) && (p->dram_cbt_mode[RANK_1] == CBT_NORMAL_MODE))
		dram_cbt_mode = CBT_R0_BYTE_R1_NORMAL;
	else
		dram_cbt_mode = CBT_R0_R1_NORMAL;

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RSTMASK), dram_cbt_mode, RSTMASK_RSV_DRAM_CBT_MIXED);

	/* Sagy: Keep original setting till OS kernel ready, if ready, remove it */
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_ARBCTL), (p->dram_cbt_mode[RANK_0] | p->dram_cbt_mode[RANK_1]), ARBCTL_RSV_DRAM_CBT);

	vSetPHY2ChannelMapping(p, save_ch);
#endif

	mcSHOW_DBG_MSG3(("[DramcInit] Done\n"));
	mcFPRINTF((fp_A60501, "[DramcInit] Done\n\n"));

	return DRAM_OK;
}



#if 1 /* Comment out unused code */
#if (FOR_DV_SIMULATION_USED == 0 && SW_CHANGE_FOR_SIMULATION == 0)
void DramcEnterSelfRefresh(DRAMC_CTX_T *p, U8 op)
{
	U8 ucstatus = 0;
	U32 uiTemp;
	U32 u4TimeCnt;

	u4TimeCnt = TIME_OUT_CNT;

	mcSHOW_DBG_MSG(("[EnterSelfRefresh] %s\n", ((op == 1) ? "enter" : "exit")));
	mcFPRINTF((fp_A60501, "[EnterSelfRefresh] %s\n", ((op == 1) ? "enter" : "exit")));

	if (op == 1) /* enter self refresh */
	{
		if (u1IsLP4Family(p->dram_type))
		{
			/*
			 * ONLY work for LP4, not LP3
			 * MISCA_SRFPD_DIS =1, self-refresh
			 * MISCA_SRFPD_DIS =0, self-refresh power down
			 */
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SREFCTRL), 1, SREFCTRL_SRFPD_DIS);
		}

		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SREFCTRL), 1, SREFCTRL_SELFREF);
		mcDELAY_US(2);
		uiTemp = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MISC_STATUSA), MISC_STATUSA_SREF_STATE);
		while ((uiTemp == 0) && (u4TimeCnt > 0))
		{
			mcSHOW_DBG_MSG2(("Still not enter self refresh(%d)\n", u4TimeCnt));
			mcDELAY_US(1);
			uiTemp = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MISC_STATUSA), MISC_STATUSA_SREF_STATE);
			u4TimeCnt --;
		}
	}
	else /* exit self refresh */
	{
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SREFCTRL), 0, SREFCTRL_SELFREF);

		mcDELAY_US(2);
		uiTemp = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MISC_STATUSA), MISC_STATUSA_SREF_STATE);
		while ((uiTemp != 0) && (u4TimeCnt > 0))
		{
			mcSHOW_DBG_MSG2(("Still not exit self refresh(%d)\n", u4TimeCnt));
			mcDELAY_US(1);
			uiTemp = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MISC_STATUSA), MISC_STATUSA_SREF_STATE);
			u4TimeCnt--;
		}
	}

	if (u4TimeCnt == 0)
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
#endif /* Comment out unused code */


#ifdef DUMMY_READ_FOR_TRACKING
void DramcDummyReadAddressSetting(DRAMC_CTX_T *p)
{
#if 1/* ndef CERVINO_TO_BE_PORTING */
	U8 backup_channel = p->channel, backup_rank = p->rank;
	U8 channelIdx, rankIdx;
	dram_addr_t dram_addr;

	for (channelIdx = CHANNEL_A; channelIdx < CHANNEL_NUM; channelIdx++)
	{
		vSetPHY2ChannelMapping(p, channelIdx);
		for (rankIdx = RANK_0; rankIdx < RANK_MAX; rankIdx++)
		{
			vSetRank(p, rankIdx);

			dram_addr.ch = channelIdx;
			dram_addr.rk = rankIdx;

			get_dummy_read_addr( & dram_addr);
			mcSHOW_DBG_MSG3(("=== dummy read address: CH_%d, RK%d, row: 0x%x, bk: %d, col: 0x%x\n\n",
				channelIdx, rankIdx, dram_addr.row, dram_addr.bk, dram_addr.col));

			vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RK0_DUMMY_RD_ADR), P_Fld(dram_addr.row, RK0_DUMMY_RD_ADR_DMY_RD_RK0_ROW_ADR)
				| P_Fld(dram_addr.col, RK0_DUMMY_RD_ADR_DMY_RD_RK0_COL_ADR)
				| P_Fld(0, RK0_DUMMY_RD_ADR_DMY_RD_RK0_LEN));
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_DUMMY_RD_BK), dram_addr.bk, RK0_DUMMY_RD_BK_DMY_RD_RK0_BK);
		}
	}

	vSetPHY2ChannelMapping(p, backup_channel);
	vSetRank(p, backup_rank);
#endif
}

void DramcDummyReadForTrackingEnable(DRAMC_CTX_T *p)
{
	vIO32WriteFldAlign_All(DRAMC_REG_DUMMY_RD, p->support_rank_num, DUMMY_RD_RANK_NUM);
	/* Dummy read pattern (Better efficiency during rx dly tracking) DE: YH Tsai, Wei-jen */
	vIO32Write4B_All(DRAMC_REG_RK0_DUMMY_RD_WDATA0, 0xAAAA5555); /* Field RK0_DUMMY_RD_WDATA0_DMY_RD_RK0_WDATA0 */
	vIO32Write4B_All(DRAMC_REG_RK0_DUMMY_RD_WDATA1, 0xAAAA5555); /* Field RK0_DUMMY_RD_WDATA1_DMY_RD_RK0_WDATA1 */
	vIO32Write4B_All(DRAMC_REG_RK0_DUMMY_RD_WDATA2, 0xAAAA5555); /* Field RK0_DUMMY_RD_WDATA2_DMY_RD_RK0_WDATA2 */
	vIO32Write4B_All(DRAMC_REG_RK0_DUMMY_RD_WDATA3, 0xAAAA5555); /* Field RK0_DUMMY_RD_WDATA3_DMY_RD_RK0_WDATA3 */

	vIO32Write4B_All(DRAMC_REG_RK1_DUMMY_RD_WDATA0, 0xAAAA5555); /* Field RK1_DUMMY_RD_WDATA0_DMY_RD_RK1_WDATA0 */
	vIO32Write4B_All(DRAMC_REG_RK1_DUMMY_RD_WDATA1, 0xAAAA5555); /* Field RK1_DUMMY_RD_WDATA1_DMY_RD_RK1_WDATA1 */
	vIO32Write4B_All(DRAMC_REG_RK1_DUMMY_RD_WDATA2, 0xAAAA5555); /* Field RK1_DUMMY_RD_WDATA2_DMY_RD_RK1_WDATA2 */
	vIO32Write4B_All(DRAMC_REG_RK1_DUMMY_RD_WDATA3, 0xAAAA5555); /* Field RK1_DUMMY_RD_WDATA3_DMY_RD_RK1_WDATA3 */

	vIO32WriteFldAlign_All(DRAMC_REG_TEST2_4, 4, TEST2_4_TESTAGENTRKSEL); /* Dummy Read rank selection is controlled by Test Agent */

#if 0/* __ETT__ */
	/*
	 * indicate ROW_ADR = 2 for Dummy Write pattern address, in order to avoid pattern will be overwrited by MEM_TEST(test range 0xffff)
	* Pattern locates: 0x40010000, 0x40010100, 0x80010000, 0x80010100
	 */
	dram_addr_t dram_addr;

	dram_addr.ch = 0;
	dram_addr.rk = 0;
	get_dummy_read_addr( & dram_addr);

	vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_RK0_DUMMY_RD_ADR), P_Fld(dram_addr.row, RK0_DUMMY_RD_ADR_DMY_RD_RK0_ROW_ADR)
		| P_Fld(dram_addr.col, RK0_DUMMY_RD_ADR_DMY_RD_RK0_COL_ADR)
		| P_Fld(0, RK0_DUMMY_RD_ADR_DMY_RD_RK0_LEN));
	vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_RK0_DUMMY_RD_BK), dram_addr.bk, RK0_DUMMY_RD_BK_DMY_RD_RK0_BK);

	dram_addr.rk = 1;
	get_dummy_read_addr( & dram_addr);

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

	/*
	 * DUMMY_RD_RX_TRACK = 1:
	 * During "RX input delay tracking enable" and "DUMMY_RD_EN=1" Dummy read will force a read command to a certain rank,
	 * ignoring whether or not EMI has executed a read command to that certain rank in the past 4us.
	 */
	if (p->frequency >= 1600)
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
	vIO32WriteFldAlign_All(DRAMC_REG_SHU_IMPCAL1, 0x10, SHU_IMPCAL1_IMPCALCNT);
}
#endif
#ifdef IMPEDANCE_TRACKING_ENABLE
void DramcImpedanceTrackingEnable(DRAMC_CTX_T *p)
{
#if 0  /* Impedance tracking offset for DRVP + 2 */
	vIO32WriteFldMulti_All(DRAMC_REG_IMPEDAMCE_CTRL1, P_Fld(2, IMPEDAMCE_CTRL1_DQS1_OFF) | P_Fld(2, IMPEDAMCE_CTRL1_DOS2_OFF));
	vIO32WriteFldMulti_All(DRAMC_REG_IMPEDAMCE_CTRL2, P_Fld(2, IMPEDAMCE_CTRL2_DQ1_OFF) | P_Fld(2, IMPEDAMCE_CTRL2_DQ2_OFF));
#endif

	/* Write (DRAMC _BASE+ 0x8B) [31:0] = 32'he4000000//enable impedance tracking */
	vIO32WriteFldAlign(DDRPHY_MISC_CTRL0, 0x1, MISC_CTRL0_IMPCAL_CHAB_EN); /* Set CHA this bit to enable dual channel tracking */

	/*
	 * During shuffle, after CH_A IMP update done, CH_B has no enough time to update (IMPCAL_IMPCAL_DRVUPDOPT=1)
	 * enable ECO function for impedance load last tracking result of previous shuffle level (IMPCAL_IMPCAL_CHGDRV_ECO_OPT=1)
	 * enable ECO function for impcal_sm hange when DRVP>=0x1D (IMPCAL_IMPCAL_SM_ECO_OPT=1)
	 */
	vIO32WriteFldMulti_All(DRAMC_REG_IMPCAL, P_Fld(1, IMPCAL_IMPCAL_HW) | P_Fld(0, IMPCAL_IMPCAL_EN) | P_Fld(1, IMPCAL_IMPCAL_SWVALUE_EN) |
		P_Fld(1, IMPCAL_IMPCAL_NEW_OLD_SL) | P_Fld(1, IMPCAL_IMPCAL_DRVUPDOPT) |
		P_Fld(1, IMPCAL_IMPCAL_CHGDRV_ECO_OPT) | P_Fld(1, IMPCAL_IMPCAL_SM_ECO_OPT) |
		P_Fld(1, IMPCAL_IMPCAL_ECO_OPT) | P_Fld(1, IMPCAL_DRV_ECO_OPT));

	/* dual channel continuously tracking @ system busy, self-refresh, Hhbrid-S1 */
	vIO32WriteFldAlign_All(DDRPHY_MISC_CTRL0, 0x1, MISC_CTRL0_IMPCAL_LP_ECO_OPT);

	/* no update imp CA, because CA is unterm now */
	vIO32WriteFldAlign_All(DRAMC_REG_IMPCAL, 1, IMPCAL_IMPCAL_BYPASS_UP_CA_DRV);

	/* CH_A set 1, CH_B set 0 (mp setting) */
	vIO32WriteFldMulti(DRAMC_REG_IMPCAL, P_Fld(0, IMPCAL_DIS_SUS_CH0_DRV) | P_Fld(1, IMPCAL_DIS_SUS_CH1_DRV));
	vIO32WriteFldMulti(DRAMC_REG_IMPCAL + SHIFT_TO_CHB_ADDR, P_Fld(1, IMPCAL_DIS_SUS_CH0_DRV) | P_Fld(0, IMPCAL_DIS_SUS_CH1_DRV));
}
#endif
#if TDQSCK_PRECALCULATION_FOR_DVFS
/*
 * divRoundClosest() - to round up to the nearest integer
 * discard four, but treat five as whole (of decimal points)
 */
int divRoundClosest(const int n, const int d)
{
	return ((n < 0) ^ (d < 0)) ? ((n - d / 2) / d) : ((n + d / 2) / d);
}


void DramcDQSPrecalculation_preset(DRAMC_CTX_T *p)/* Test tDQSCK_temp Pre - calculation */
{
	DRAM_CHANNEL_T eOriChannel = p->channel;
	DRAM_RANK_T eOriRank = p->rank;
	U8 irank_num = 0, ichannel_num = 0;
	U32 u4value = 0;
	U32 u4value1 = 0;
	U32 shuffle_src_index, shuffle_dst_index, jump_ratio_index;
	U16 u2JumpRatio[12] = {0}; /* Used to record "jump ratio" calulation results */

	/* Calculate jump ratios and save to u2JumpRatio array */
	jump_ratio_index = 0;

	for (shuffle_src_index = DRAM_DFS_SHUFFLE_1; shuffle_src_index < HW_REG_SHUFFLE_MAX; shuffle_src_index++)
	{
		for (shuffle_dst_index = DRAM_DFS_SHUFFLE_1; shuffle_dst_index < HW_REG_SHUFFLE_MAX; shuffle_dst_index++)
		{
			if (shuffle_src_index == shuffle_dst_index)
			{
				continue;
			}
			if (shuffle_src_index >= DRAM_DFS_SHUFFLE_MAX || shuffle_dst_index >= DRAM_DFS_SHUFFLE_MAX)
			{
				jump_ratio_index++;
				continue;
			}

			u2JumpRatio[jump_ratio_index] = divRoundClosest((get_Freq_by_shuffleIndex(p, shuffle_dst_index) * 32), get_Freq_by_shuffleIndex(p, shuffle_src_index));
			/*
			 * mcSHOW_DBG_MSG3(("shuffle_%d=DDR%d / shuffle_%d=DDR%d \n", shuffle_dst_index, get_Freq_by_shuffleIndex(p,shuffle_dst_index)<<1,
			 * shuffle_src_index, get_Freq_by_shuffleIndex(p,shuffle_src_index)<<1));
			 * mcSHOW_DBG_MSG3(("Jump_RATIO_%d : 0x%x\n", jump_ratio_index, u2JumpRatio[jump_ratio_index],
			 * get_Freq_by_shuffleIndex(p,shuffle_src_index)));
			 */

			mcSHOW_DBG_MSG3(("Jump_RATIO [%d]: %x\tFreq %d -> %d\tDDR%d -> DDR%d\n", jump_ratio_index, u2JumpRatio[jump_ratio_index], shuffle_src_index + 1, shuffle_dst_index + 1, get_Freq_by_shuffleIndex(p, shuffle_src_index) << 1, get_Freq_by_shuffleIndex(p, shuffle_dst_index) << 1));
			jump_ratio_index++;
		}
	}

	for (ichannel_num = 0; ichannel_num < p->support_channel_num; ichannel_num++)
	{
		vSetPHY2ChannelMapping(p, ichannel_num);

		/*
		 * mcSHOW_DBG_MSG(("Step1: Set DVFS HW enable\n"));
		 * Step1: Set DVFS HW enable
		 */
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PRE_TDQSCK1), 1, PRE_TDQSCK1_TDQSCK_PRECAL_HW);

		/*
		 * mcSHOW_DBG_MSG(("Step2: Set jump ratio\n"));
		 * Step2: Set jump ratio
		 */
		/*
		 * F1:3200
		 * F2:2667
		 * F3:2667
		 * F4:800
		 *
		 * Name            DVFS setting        Location    Value
		 * Jump_RATIO_0    00 to 01(F1 to F2)  0x87[31:24] 01(Freq)/00(freq)  * 0x20---1A
		 * Jump_RATIO_1    00 to 10(F1 to F3)  0x87[23:16] 10(Freq)/00(freq)  * 0x20---1A
		 * Jump_RATIO_2    00 to 11(F1 to F4)  0x87[15:8]  11(Freq)/00(freq)  * 0x20---8
		 * Jump_RATIO_3    01 to 00(F2 to F1)  0x87[7:0]   00(Freq)/01(freq)  * 0x20---26
		 * Jump_RATIO_4    01 to 10(F2 to F3)  0x88[31:24] 10(Freq)/01(freq)  * 0x20---20
		 * Jump_RATIO_5    01 to 11(F2 to F4)  0x88[23:16] 11(Freq)/01(freq)  * 0x20---9
		 * Jump_RATIO_6    10 to 00(F3 to F1)  0x88[15:8]  00(Freq)/10(freq)  * 0x20---26
		 * Jump_RATIO_7    10 to 01(F3 to F2)  0x88[7:0]   01(Freq)/10(freq)  * 0x20---20
		 * Jump_RATIO_8    10 to 11(F3 to F4)  0x89[31:24] 11(Freq)/10(freq)  * 0x20---9
		 * Jump_RATIO_9    11 to 00(F4 to F1)  0x89[23:16] 00(Freq)/11(freq)  * 0x20---80
		 * Jump_RATIO_10   11 to 01(F4 to F2)  0x89[15:8]  01(Freq)/11(freq)  * 0x20---6A
		 * Jump_RATIO_11   11 to 10(F4 to F3)  0x89[7:0]   10(Freq)/11(freq)  * 0x20---6A
		 */
		/* Save jumpRatios into corresponding register fields */
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_PRE_TDQSCK2), P_Fld(u2JumpRatio[0], PRE_TDQSCK2_TDDQSCK_JUMP_RATIO0)
			| P_Fld(u2JumpRatio[1], PRE_TDQSCK2_TDDQSCK_JUMP_RATIO1)
			| P_Fld(u2JumpRatio[2], PRE_TDQSCK2_TDDQSCK_JUMP_RATIO2)
			| P_Fld(u2JumpRatio[3], PRE_TDQSCK2_TDDQSCK_JUMP_RATIO3));
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_PRE_TDQSCK3), P_Fld(u2JumpRatio[4], PRE_TDQSCK3_TDDQSCK_JUMP_RATIO4)
			| P_Fld(u2JumpRatio[5], PRE_TDQSCK3_TDDQSCK_JUMP_RATIO5)
			| P_Fld(u2JumpRatio[6], PRE_TDQSCK3_TDDQSCK_JUMP_RATIO6)
			| P_Fld(u2JumpRatio[7], PRE_TDQSCK3_TDDQSCK_JUMP_RATIO7));
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_PRE_TDQSCK4), P_Fld(u2JumpRatio[8], PRE_TDQSCK4_TDDQSCK_JUMP_RATIO8)
			| P_Fld(u2JumpRatio[9], PRE_TDQSCK4_TDDQSCK_JUMP_RATIO9)
			| P_Fld(u2JumpRatio[10], PRE_TDQSCK4_TDDQSCK_JUMP_RATIO10)
			| P_Fld(u2JumpRatio[11], PRE_TDQSCK4_TDDQSCK_JUMP_RATIO11));

		for (irank_num = 0; irank_num < 2; irank_num++)
		{
			vSetRank(p, irank_num);

			/*
			 * mcSHOW_DBG_MSG(("Step3: Write UI/PI info to RG\n"));
			 * Step3: Write UI/PI info to RG
			 * B0 ==========================
			 * Shuffle 0 =========================================
			 */
			u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED);
			u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK1), (u4value << 3) | u4value1, RK0_PRE_TDQSCK1_TDQSCK_UIFREQ1_B0R0); /* 0xcc[5:0] */

			u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQSIEN), SHURK0_DQSIEN_R0DQS0IEN);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK1), u4value, RK0_PRE_TDQSCK1_TDQSCK_PIFREQ1_B0R0); /* 0xcc[12:6] */

			u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1);
			u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK3), (u4value << 3) | u4value1, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ1_P1_B0R0); /* 0xce[5:0] */

			/* Shuffle 1 ========================================= */
			u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED);
			u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK1), (u4value << 3) | u4value1, RK0_PRE_TDQSCK1_TDQSCK_UIFREQ2_B0R0); /* 0xcc[18:13] */

			u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_DQSIEN), SHURK0_DQSIEN_R0DQS0IEN);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK1), u4value, RK0_PRE_TDQSCK1_TDQSCK_PIFREQ2_B0R0); /* 0xcc[25:19] */

			u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1);
			u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK3), (u4value << 3) | u4value1, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ2_P1_B0R0); /* 0xce[11:6] */

			/* Shuffle 2 ========================================= */
			u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED);
			u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK2), (u4value << 3) | u4value1, RK0_PRE_TDQSCK2_TDQSCK_UIFREQ3_B0R0); /* 0xcd[5:0] */

			u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_DQSIEN), SHURK0_DQSIEN_R0DQS0IEN);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK2), u4value, RK0_PRE_TDQSCK2_TDQSCK_PIFREQ3_B0R0); /* 0xcd[12:6] */

			u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1);
			u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK3), (u4value << 3) | u4value1, RK0_PRE_TDQSCK3_TDQSCK_UIFREQ3_P1_B0R0); /* 0xce[17:12] */

			/*
			 * B1 ==========================
			 * Shuffle 0 =========================================
			 */
			u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED);
			u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK4), (u4value << 3) | u4value1, RK0_PRE_TDQSCK4_TDQSCK_UIFREQ1_B1R0); /* 0xcf[5:0] */

			u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQSIEN), SHURK0_DQSIEN_R0DQS1IEN);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK4), u4value, RK0_PRE_TDQSCK4_TDQSCK_PIFREQ1_B1R0); /* 0xcf[12:6] */

			u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1);
			u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK6), (u4value << 3) | u4value1, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ1_P1_B1R0); /* 0xd1[5:0] */

			/* Shuffle 1 ========================================= */
			u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED);
			u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK4), (u4value << 3) | u4value1, RK0_PRE_TDQSCK4_TDQSCK_UIFREQ2_B1R0); /* 0xcf[18:13] */

			u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_DQSIEN), SHURK0_DQSIEN_R0DQS1IEN);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK4), u4value, RK0_PRE_TDQSCK4_TDQSCK_PIFREQ2_B1R0); /* 0xcf[25:19] */

			u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1);
			u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK6), (u4value << 3) | u4value1, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ2_P1_B1R0); /* 0xd1[11:6] */

			/* Shuffle 2 ========================================= */
			u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED);
			u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK5), (u4value << 3) | u4value1, RK0_PRE_TDQSCK5_TDQSCK_UIFREQ3_B1R0); /* 0xd0[5:0] */

			u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_DQSIEN), SHURK0_DQSIEN_R0DQS1IEN);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK5), u4value, RK0_PRE_TDQSCK5_TDQSCK_PIFREQ3_B1R0); /* 0xd0[12:6] */

			u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1);
			u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK6), (u4value << 3) | u4value1, RK0_PRE_TDQSCK6_TDQSCK_UIFREQ3_P1_B1R0); /* 0xd1[17:12] */

			/*
			 * if Project is combo LP3+LP4, then needs to set B2 and B3
			 * B2 ==========================
			 * Shuffle 0 =========================================
			 */
			u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED);
			u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK7), (u4value << 3) | u4value1, RK0_PRE_TDQSCK7_TDQSCK_UIFREQ1_B2R0); /* 0xcf[5:0] */

			u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQSIEN), SHURK0_DQSIEN_R0DQS2IEN);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK7), u4value, RK0_PRE_TDQSCK7_TDQSCK_PIFREQ1_B2R0); /* 0xcf[12:6] */

			u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED_P1);
			u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED_P1);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK9), (u4value << 3) | u4value1, RK0_PRE_TDQSCK9_TDQSCK_UIFREQ1_P1_B2R0); /* 0xd1[5:0] */

			/* Shuffle 1 ========================================= */
			u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED);
			u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK7), (u4value << 3) | u4value1, RK0_PRE_TDQSCK7_TDQSCK_UIFREQ2_B2R0); /* 0xcf[18:13] */

			u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_DQSIEN), SHURK0_DQSIEN_R0DQS2IEN);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK7), u4value, RK0_PRE_TDQSCK7_TDQSCK_PIFREQ2_B2R0); /* 0xcf[25:19] */

			u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED_P1);
			u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED_P1);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK9), (u4value << 3) | u4value1, RK0_PRE_TDQSCK9_TDQSCK_UIFREQ2_P1_B2R0); /* 0xd1[11:6] */

			/* Shuffle 2 ========================================= */
			u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED);
			u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK8), (u4value << 3) | u4value1, RK0_PRE_TDQSCK8_TDQSCK_UIFREQ3_B2R0); /* 0xd0[5:0] */

			u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_DQSIEN), SHURK0_DQSIEN_R0DQS2IEN);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK8), u4value, RK0_PRE_TDQSCK8_TDQSCK_PIFREQ3_B2R0); /* 0xd0[12:6] */

			u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS2_GATED_P1);
			u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS2_GATED_P1);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK9), (u4value << 3) | u4value1, RK0_PRE_TDQSCK9_TDQSCK_UIFREQ3_P1_B2R0); /* 0xd1[17:12] */

			/*
			 * B3 ==========================
			 * Shuffle 0 =========================================
			 */
			u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED);
			u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK10), (u4value << 3) | u4value1, RK0_PRE_TDQSCK10_TDQSCK_UIFREQ1_B3R0); /* 0xcf[5:0] */

			u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQSIEN), SHURK0_DQSIEN_R0DQS3IEN);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK10), u4value, RK0_PRE_TDQSCK10_TDQSCK_PIFREQ1_B3R0); /* 0xcf[12:6] */

			u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED_P1);
			u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED_P1);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK12), (u4value << 3) | u4value1, RK0_PRE_TDQSCK12_TDQSCK_UIFREQ1_P1_B3R0); /* 0xd1[5:0] */

			/* Shuffle 1 ========================================= */
			u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED);
			u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK10), (u4value << 3) | u4value1, RK0_PRE_TDQSCK10_TDQSCK_UIFREQ2_B3R0); /* 0xcf[18:13] */

			u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_DQSIEN), SHURK0_DQSIEN_R0DQS3IEN);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK10), u4value, RK0_PRE_TDQSCK10_TDQSCK_PIFREQ2_B3R0); /* 0xcf[25:19] */

			u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED_P1);
			u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED_P1);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK12), (u4value << 3) | u4value1, RK0_PRE_TDQSCK12_TDQSCK_UIFREQ2_P1_B3R0); /* 0xd1[11:6] */

			/* Shuffle 2 ========================================= */
			u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED);
			u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK11), (u4value << 3) | u4value1, RK0_PRE_TDQSCK11_TDQSCK_UIFREQ3_B3R0); /* 0xd0[5:0] */

			u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_DQSIEN), SHURK0_DQSIEN_R0DQS3IEN);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK11), u4value, RK0_PRE_TDQSCK11_TDQSCK_PIFREQ3_B3R0); /* 0xd0[12:6] */

			u4value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS3_GATED_P1);
			u4value1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS3_GATED_P1);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK12), (u4value << 3) | u4value1, RK0_PRE_TDQSCK12_TDQSCK_UIFREQ3_P1_B3R0); /* 0xd1[17:12] */
		}
		/* mcSHOW_DBG_MSG((" Select HW flow\n")); */
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_PRE_TDQSCK1), P_Fld(0x1, PRE_TDQSCK1_TDQSCK_REG_DVFS));
		/*
		 * Step4: Set Auto save to RG
		 * mcSHOW_DBG_MSG(("Step 4: Select HW flow\n"));
		 */
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PRE_TDQSCK1), 1, PRE_TDQSCK1_TDQSCK_HW_SW_UP_SEL);

	}
	vSetPHY2ChannelMapping(p, eOriChannel);
	vSetRank(p, eOriRank);
	return;
}
#endif

#if GATING_ONLY_FOR_DEBUG
void DramcGatingDebugRankSel(DRAMC_CTX_T *p, U8 u1Rank)
{
	if (p->support_rank_num == RANK_SINGLE)
	{
		u1Rank = 0;
	}

	vIO32WriteFldAlign_All(DDRPHY_MISC_CTRL1, u1Rank, MISC_CTRL1_R_DMSTBENCMP_RK_OPT);
}

void DramcGatingDebugInit(DRAMC_CTX_T *p)
{
	U32 backupReg0x64[CHANNEL_NUM], backupReg0xC8[CHANNEL_NUM], backupReg0xD0[CHANNEL_NUM];
	U32 backupRegShu_SCINTV[DRAM_DFS_SHUFFLE_MAX][CHANNEL_NUM];
	U32 channel_idx, backup_channel, shu_index;

	backup_channel = vGetPHY2ChannelMapping(p);

	for (channel_idx = CHANNEL_A; channel_idx < p->support_channel_num; channel_idx++)
	{
		vSetPHY2ChannelMapping(p, channel_idx);
		backupReg0x64[channel_idx] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL));
		backupReg0xC8[channel_idx] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR));
		backupReg0xD0[channel_idx] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD));
		for (shu_index = DRAM_DFS_SHUFFLE_1; shu_index < DRAM_DFS_SHUFFLE_MAX; shu_index++)
			backupRegShu_SCINTV[shu_index][channel_idx] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SHU_SCINTV) + SHU_GRP_DRAMC_OFFSET*shu_index);
	}
	vSetPHY2ChannelMapping(p, backup_channel);

	/* Disable MR4 MR18/MR19, TxHWTracking, Dummy RD before reset */
	vIO32WriteFldAlign_All(DRAMC_REG_SPCMDCTRL, 0x1, SPCMDCTRL_REFRDIS);    /* MR4 Disable */
	vIO32WriteFldAlign_All(DRAMC_REG_DQSOSCR, 0x1, DQSOSCR_DQSOSCRDIS);  /* MR18, MR19 Disable */
	for (shu_index = DRAM_DFS_SHUFFLE_1; shu_index < DRAM_DFS_SHUFFLE_MAX; shu_index++)
		vIO32WriteFldAlign_All(DRAMC_REG_SHU_SCINTV + SHU_GRP_DRAMC_OFFSET*shu_index, 0x1, SHU_SCINTV_DQSOSCENDIS);
	vIO32WriteFldMulti_All(DRAMC_REG_DUMMY_RD, P_Fld(0x0, DUMMY_RD_DUMMY_RD_EN)
		| P_Fld(0x0, DUMMY_RD_SREF_DMYRD_EN)
		| P_Fld(0x0, DUMMY_RD_DQSG_DMYRD_EN)
		| P_Fld(0x0, DUMMY_RD_DMY_RD_DBG));
	mcDELAY_US(4);

	DramPhyReset(p);

	/* Restore backup regs */
	for (channel_idx = CHANNEL_A; channel_idx < p->support_channel_num; channel_idx++)
	{
		vSetPHY2ChannelMapping(p, channel_idx);
		vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), backupReg0x64[channel_idx]);
		vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR), backupReg0xC8[channel_idx]);
		vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD), backupReg0xD0[channel_idx]);
		for (shu_index = DRAM_DFS_SHUFFLE_1; shu_index < DRAM_DFS_SHUFFLE_MAX; shu_index++)
			vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU_SCINTV) + SHU_GRP_DRAMC_OFFSET*shu_index, backupRegShu_SCINTV[shu_index][channel_idx]);
	}
	vSetPHY2ChannelMapping(p, backup_channel);


	/* enable &reset DQS counter */
	vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1, SPCMD_DQSGCNTEN);
	mcDELAY_US(4); /* wait 1 auto refresh after DQS Counter enable */

	vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1, SPCMD_DQSGCNTRST);
	mcDELAY_US(1); /* delay 2T */
	vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_DQSGCNTRST);
	/* mcSHOW_DBG_MSG(("DramcGatingDebugInit done\n" )); */
}

void DramcGatingDebugExit(DRAMC_CTX_T *p)
{
	/* enable &reset DQS counter */
	vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_DQSGCNTEN);
	vIO32WriteFldAlign_All(DDRPHY_MISC_CTRL1, 0, MISC_CTRL1_R_DMSTBENCMP_RK_OPT);
}

void DramcGatingDebug(DRAMC_CTX_T *p, U8 u1Channel)
{
	U32 LP3_DataPerByte[DQS_NUMBER];
	U32 u4DebugCnt[DQS_NUMBER];
	U16 u2DebugCntPerByte;

	U32 u4value, u4all_result_R, u4all_result_F, u4err_value;
	U8 backup_channel;

	backup_channel = p->channel;
	vSetPHY2ChannelMapping(p, u1Channel);

	mcDELAY_MS(10);

	LP3_DataPerByte[0] = (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_STBERR_RK0_R), MISC_STBERR_RK0_R_STBERR_RK0_R));
	LP3_DataPerByte[2] = (LP3_DataPerByte[0] >> 8) & 0xff;
	LP3_DataPerByte[0] &= 0xff;

	u4all_result_R = LP3_DataPerByte[0] | (LP3_DataPerByte[2] << 8);

	/* falling */
	LP3_DataPerByte[0] = (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_STBERR_RK0_F), MISC_STBERR_RK0_F_STBERR_RK0_F));
	LP3_DataPerByte[2] = (LP3_DataPerByte[0] >> 8) & 0xff;
	LP3_DataPerByte[0] &= 0xff;

	u4all_result_F = LP3_DataPerByte[0] | (LP3_DataPerByte[2] << 8);

	/* read DQS counter */
	u4DebugCnt[0] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DQSGNWCNT0));
	u4DebugCnt[1] = (u4DebugCnt[0] >> 16) & 0xffff;
	u4DebugCnt[0] &= 0xffff;

	u4DebugCnt[2] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DQSGNWCNT1));
	u4DebugCnt[3] = (u4DebugCnt[2] >> 16) & 0xffff;
	u4DebugCnt[2] &= 0xffff;

	mcSHOW_DBG_MSG(("\n[GatingDebug] CH %d, DQS count (B3->B0) 0x%H, 0x%H, 0x%H, 0x%H \nError flag Rank0 (B3->B0) %B %B  %B %B  %B %B  %B %B\n", \
		u1Channel, u4DebugCnt[3], u4DebugCnt[2], u4DebugCnt[1], u4DebugCnt[0], \
		(u4all_result_F >> 24) & 0xff, (u4all_result_R >> 24) & 0xff, \
		(u4all_result_F >> 16) & 0xff, (u4all_result_R >> 16) & 0xff, \
		(u4all_result_F >> 8) & 0xff, (u4all_result_R >> 8) & 0xff, \
		(u4all_result_F) & 0xff, (u4all_result_R) & 0xff));

	if (p->support_rank_num == RANK_DUAL)
	{
		LP3_DataPerByte[0] = (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_STBERR_RK1_R), MISC_STBERR_RK1_R_STBERR_RK1_R)); /* PHY_B */
		LP3_DataPerByte[2] = (LP3_DataPerByte[0] >> 8) & 0xff;
		LP3_DataPerByte[0] &= 0xff;

		u4all_result_R = LP3_DataPerByte[0] | (LP3_DataPerByte[2] << 8);

		/* falling */
		LP3_DataPerByte[0] = (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_STBERR_RK1_F), MISC_STBERR_RK1_F_STBERR_RK1_F)); /* PHY_B */
		LP3_DataPerByte[2] = (LP3_DataPerByte[0] >> 8) & 0xff;
		LP3_DataPerByte[0] &= 0xff;

		u4all_result_F = LP3_DataPerByte[0];

		mcSHOW_DBG_MSG(("Error flag Rank1 (B3->B0) %B %B  %B %B  %B %B  %B %B\n", \
			(u4all_result_F >> 24) & 0xff, (u4all_result_R >> 24) & 0xff, \
			(u4all_result_F >> 16) & 0xff, (u4all_result_R >> 16) & 0xff, \
			(u4all_result_F >> 8) & 0xff, (u4all_result_R >> 8) & 0xff, \
			(u4all_result_F) & 0xff, (u4all_result_R) & 0xff));
	}
	else
	{
		mcSHOW_DBG_MSG(("\n"));
	}

	/*
	 * vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1, SPCMD_DQSGCNTRST);
	 * mcDELAY_US(1);//delay 2T
	 * vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_DQSGCNTRST);
	 */

	vSetPHY2ChannelMapping(p, backup_channel);
}
#endif

#if (FOR_DV_SIMULATION_USED == 0 && SW_CHANGE_FOR_SIMULATION == 0)
#if (__ETT__ || CPU_RW_TEST_AFTER_K)
void DramcDumpDebugInfo(DRAMC_CTX_T *p)
{
	U8 mpdivInSel, cali_shu_sel, mpdiv_shu_sel, u1RefreshRate;
	DRAM_CHANNEL_T channelIdx;

	mpdivInSel = u4IO32ReadFldAlign(DDRPHY_MISC_CTRL2, MISC_CTRL2_PLL_SHU_GP);
	cali_shu_sel = u4IO32ReadFldAlign(DRAMC_REG_SHUCTRL, SHUCTRL_R_OTHER_SHU_GP);
	mpdiv_shu_sel = u4IO32ReadFldAlign(DRAMC_REG_SHUCTRL, SHUCTRL_R_MPDIV_SHU_GP);

	/* Read shuffle selection */
	mcSHOW_ERR_MSG(("\n\n[DumpDebugInfo]\n"
		"\tmpdivInSel %d, cali_shu_sel %d, mpdiv_shu_sel %d\n",
		mpdivInSel, cali_shu_sel, mpdiv_shu_sel));

#if GATING_ONLY_FOR_DEBUG
	/*
	 * Read gating error flag
	 * DramcGatingDebugInit(p);
	 */
	for (channelIdx = CHANNEL_A; channelIdx < p->support_channel_num; channelIdx++)
	{
		DramcGatingDebug(p, channelIdx);
	}
#endif

#if (FOR_DV_SIMULATION_USED == 0 && SW_CHANGE_FOR_SIMULATION == 0)
	/* Read HW gating tracking */
#ifdef HW_GATING
	for (channelIdx = CHANNEL_A; channelIdx < p->support_channel_num; channelIdx++)
	{
		DramcPrintHWGatingStatus(p, channelIdx);
	}
#endif

#if (ENABLE_RX_TRACKING_LP4 && RX_DLY_TRACK_ONLY_FOR_DEBUG && defined(DUMMY_READ_FOR_TRACKING))
	DramcPrintRxDlyTrackDebugStatus(p);
#endif

#if ENABLE_RX_TRACKING_LP4
	for (channelIdx = CHANNEL_A; channelIdx < p->support_channel_num; channelIdx++)
	{
		DramcPrintRXDQDQSStatus(p, channelIdx);
	}
#endif

#ifdef IMPEDANCE_TRACKING_ENABLE
	if (u1IsLP4Family(p->dram_type))
	{
		for (channelIdx = CHANNEL_A; channelIdx < p->support_channel_num; channelIdx++)
		{
			DramcPrintIMPTrackingStatus(p, channelIdx);
		}

	}
#endif

#ifdef TEMP_SENSOR_ENABLE
	for (channelIdx = CHANNEL_A; channelIdx < p->support_channel_num; channelIdx++)
	{
		u1RefreshRate = u1GetMR4RefreshRate(p, channelIdx);
		mcSHOW_ERR_MSG(("[CH%d] MRR(MR4) [10:8] = %x\n", channelIdx, u1RefreshRate));
	}
#endif
#endif/* (FOR_DV_SIMULATION_USED == 0 && SW_CHANGE_FOR_SIMULATION == 0) */
}
#endif
#endif

void vAutoRefreshSwitch(DRAMC_CTX_T *p, U8 option)
{
	if (option == ENABLE)
	{
		/* enable autorefresh */
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0), 0, REFCTRL0_REFDIS);      /* REFDIS = 0, enable auto refresh */
	}
	else
	{
		/* disable autorefresh */
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0), 1, REFCTRL0_REFDIS);      /* REFDIS = 1, disable auto refresh */

		/* because HW will actually disable autorefresh after refresh_queue empty, so we need to wait quene empty. */
		mcDELAY_US(u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MISC_STATUSA), MISC_STATUSA_REFRESH_QUEUE_CNT)*4);   /* wait refresh_queue_cnt * 3.9us */
	}
}

void DramcDebugTest(DRAMC_CTX_T *p)
{
#if 0
	U32 u4ErrorValue;

	/* TestEngine2 R/W test */
	p->test2_1 = DEFAULT_TEST2_1_CAL;
	u4ErrorValue = TestEngineCompare(p);
	mcSHOW_ERR_MSG(("[DramcDebugTest] First R / W test. Rank0: u4ErrorValue 0x%X\n", u4ErrorValue));
	if (p->support_rank_num == RANK_DUAL)
	{
		p->test2_1 = 0x55a00000;
		u4ErrorValue = TestEngineCompare(p);
		mcSHOW_ERR_MSG(("[DramcDebugTest] First R / W test. Rank1: u4ErrorValue 0x%X\n", u4ErrorValue));
	}

	/* Reset PHY */
	DramPhyReset(p);

	/* TestEngine2 R/W test */
	p->test2_1 = DEFAULT_TEST2_1_CAL;
	u4ErrorValue = TestEngineCompare(p);
	mcSHOW_ERR_MSG(("[DramcDebugTest] Reset PHY R / W test. Rank0: u4ErrorValue 0x%X\n", u4ErrorValue));

	if (p->support_rank_num == RANK_DUAL)
	{
		p->test2_1 = 0x55a00000;
		u4ErrorValue = TestEngineCompare(p);
		mcSHOW_ERR_MSG(("[DramcDebugTest] Reset PHY R / W test . Rank1:u4ErrorValue 0x%X\n", u4ErrorValue));
	}

	/* Reset Dram */
	Dram_Reset(p);

	/* TestEngine2 R/W test */
	p->test2_1 = DEFAULT_TEST2_1_CAL;
	u4ErrorValue = TestEngineCompare(p);
	mcSHOW_ERR_MSG(("[DramcDebugTest] Reset Dram R / W test. Rank0: u4ErrorValue 0x%X\n", u4ErrorValue));

	if (p->support_rank_num == RANK_DUAL)
	{
		p->test2_1 = 0x55a00000;
		u4ErrorValue = TestEngineCompare(p);
		mcSHOW_ERR_MSG(("[DramcDebugTest] Reset Dram R / W test . Rank1:u4ErrorValue 0x%X\n", u4ErrorValue));
	}

	p->test2_1 = DEFAULT_TEST2_1_CAL;
#endif
}


void TransferToSPMControl(DRAMC_CTX_T *p)
{
	if (u1IsLP4Family(p->dram_type))
	{
		vIO32WriteFldAlign_All(DDRPHY_MISC_SPM_CTRL0, 0xfbffefff, MISC_SPM_CTRL0_PHY_SPM_CTL0);
		vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL2, 0xffffffef, MISC_SPM_CTRL2_PHY_SPM_CTL2);
		vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL2 + SHIFT_TO_CHB_ADDR, 0x7fffffef, MISC_SPM_CTRL2_PHY_SPM_CTL2); /* Lewis@20170627: Set CHB CA DLL type to slave mode */
	}
	else /* LP3 */
	{
	#if ENABLE_LP3_SW
		vIO32WriteFldAlign_All(DDRPHY_MISC_SPM_CTRL0, 0xfbffefff, MISC_SPM_CTRL0_PHY_SPM_CTL0);
		vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL2, 0x7fffffef, MISC_SPM_CTRL2_PHY_SPM_CTL2); /* Different from 6771 PIC:Chen - Hsiang Set CHA CA DLL type to slave mode */
		vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL2 + SHIFT_TO_CHB_ADDR, 0x7fffffef, MISC_SPM_CTRL2_PHY_SPM_CTL2); /* Lewis@20170731: Set CHB CA DLL type to slave mode since CHB CA is data in LP3 */
	#endif
	}
	return;
}

void TransferPLLToSPMControl(DRAMC_CTX_T *p)
{
#if (FOR_DV_SIMULATION_USED == 0 && SW_CHANGE_FOR_SIMULATION == 0)
	U8 shu_level ;
	shu_level = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHUSTATUS), SHUSTATUS_SHUFFLE_LEVEL);

	/* TINFO="DRAM : enter SW DVFS" */

	/* TINFO="DRAM : SPM presetting for pinmux" */
	/* ! set SPM project code and enable clock enable */
	vIO32WriteFldMulti(SPM_POWERON_CONFIG_EN, P_Fld(0xB16, POWERON_CONFIG_EN_PROJECT_CODE) | P_Fld(1, POWERON_CONFIG_EN_BCLK_CG_EN));

	/* ! set SPM pinmux */
	vIO32WriteFldMulti(SPM_PCM_PWR_IO_EN, P_Fld(0, PCM_PWR_IO_EN_RG_PCM_PWR_IO_EN) | P_Fld(0, PCM_PWR_IO_EN_RG_RF_SYNC_EN));

	if (u1IsLP4Family(p->dram_type))
		vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON_SEL, 0x00ffffff, DRAMC_DPY_CLK_SW_CON_SEL_FULL);
	else
		vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON_SEL, 0xaaffffff, DRAMC_DPY_CLK_SW_CON_SEL_FULL);
	vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON_SEL2, 0xffffffff, DRAMC_DPY_CLK_SW_CON_SEL2_FULL);

	/* ! set  sc_dpy_2nd_dll_en, sc_dpy_dll_en, sc_dpy_dll_ck_en ,sc_dpy_vref_en , sc_phypll_en = 1 */
	vIO32WriteFldMulti(SPM_POWER_ON_VAL0, P_Fld(1, SPM_POWER_ON_VAL0_SC_DPY_2ND_DLL_EN_PCM)
		| P_Fld(1, SPM_POWER_ON_VAL0_SC_DPY_DLL_EN_PCM) | P_Fld(1, SPM_POWER_ON_VAL0_SC_DPY_DLL_CK_EN_PCM)
		| P_Fld(1, SPM_POWER_ON_VAL0_SC_DPY_VREF_EN_PCM) | P_Fld(1, SPM_POWER_ON_VAL0_SC_PHYPLL_EN_PCM));

	vIO32WriteFldAlign(SPM_S1_MODE_CH, 3, S1_MODE_CH_SPM_S1_MODE_CH);

	mcSHOW_DBG_MSG(("TransferPLLToSPMControl - MODE SW "));
	if (shu_level == 1)
	{
		mcSHOW_DBG_MSG(("CLRPLL\n"));
		vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_PHYPLL_MODE_SW_PCM);
		vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL2_MODE_SW_PCM);
		vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, 0, DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL_MODE_SW);
		vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, 1, DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL2_MODE_SW);
	}
	else
	{
		mcSHOW_DBG_MSG(("PHYPLL\n"));
		vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_PHYPLL2_MODE_SW_PCM);
		vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL_MODE_SW_PCM);
		vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, 0, DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL2_MODE_SW);
		vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, 1, DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL_MODE_SW);
	}
	mcDELAY_US(1);
	vIO32WriteFldAlign_All(DDRPHY_PLL1, 0x0, PLL1_RG_RPHYPLL_EN);
	vIO32WriteFldAlign_All(DDRPHY_PLL2, 0x0, PLL2_RG_RCLRPLL_EN);
#endif

	return;
}


void TransferToRegControl(void)
{
	/* Chen-Hsiang@20160323: After leave preloader and low power scenario, conf is controller by RG */
}

void Switch26MHzDisableDummyReadRefreshAllBank(DRAMC_CTX_T *p)
{
	vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_CLK_MEM_SEL); /* Switch clk to 26MHz */
	vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_W_CHG_MEM);
	vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_W_CHG_MEM);

	vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL0, 0, REFCTRL0_PBREFEN); /* Switch to all bank refresh */

	vIO32WriteFldMulti_All(DRAMC_REG_DUMMY_RD, P_Fld(0x0, DUMMY_RD_DQSG_DMYWR_EN)/* Disable Dummy Read */
		| P_Fld(0x0, DUMMY_RD_DQSG_DMYRD_EN) | P_Fld(0x0, DUMMY_RD_SREF_DMYRD_EN)
		| P_Fld(0x0, DUMMY_RD_DUMMY_RD_EN) | P_Fld(0x0, DUMMY_RD_DMY_RD_DBG)
		| P_Fld(0x0, DUMMY_RD_DMY_WR_DBG));
	return;
}

#define DUMP_GOLDEN_SETTING 0
#if DUMP_GOLDEN_SETTING
#define CHA_DRAMC(offset) (Channel_A_DRAMC_AO_BASE_ADDRESS + offset)
#define CHB_DRAMC(offset) (Channel_B_DRAMC_AO_BASE_ADDRESS + offset)
#define CHA_PHY(offset) (Channel_A_PHY_AO_BASE_ADDRESS + offset)
#define CHB_PHY(offset) (Channel_B_PHY_AO_BASE_ADDRESS + offset)

static U32 DramcAddr(U32 offset, DRAM_CHANNEL_T eChannel)
{
	U32 u4Addr = (eChannel == CHANNEL_A) ? (CHA_DRAMC(offset)) : (CHB_DRAMC(offset));
	return u4Addr;
}

static U32 PhyAddr(U32 offset, DRAM_CHANNEL_T eChannel)
{
	U32 u4Addr = (eChannel == CHANNEL_A) ? (CHA_PHY(offset)) : (CHB_PHY(offset));
	return u4Addr;
}

void DumpGoldenSetting(DRAMC_CTX_T *p)
{
	U32 u4Value = 0;
	U8 u1ChIdx = CHANNEL_A;

	for (u1ChIdx = CHANNEL_A; u1ChIdx <= CHANNEL_B; u1ChIdx++)
	{
		mcSHOW_DBG_MSG(("*** DRAMC related \n"))
		u4Value = (*(volatile unsigned int *)DramcAddr(0x38, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[31] = 0x%X \n", DramcAddr(0x38, u1ChIdx), (u4Value >> 31) & 0x1));
		mcSHOW_DBG_MSG(("0x%X[30] = 0x%X \n", DramcAddr(0x38, u1ChIdx), (u4Value >> 30) & 0x1));
		mcSHOW_DBG_MSG(("0x%X[5] = 0x%X \n", DramcAddr(0x38, u1ChIdx), (u4Value >> 5) & 0x1));
		mcSHOW_DBG_MSG(("0x%X[2] = 0x%X \n", DramcAddr(0x38, u1ChIdx), (u4Value >> 2) & 0x1));
		mcSHOW_DBG_MSG(("0x%X[1] = 0x%X \n", DramcAddr(0x38, u1ChIdx), (u4Value >> 1) & 0x1));
		mcSHOW_DBG_MSG(("0x%X[0] = 0x%X \n", DramcAddr(0x38, u1ChIdx), (u4Value >> 0) & 0x1));

		mcSHOW_DBG_MSG(("*** PHY related \n"));
		u4Value = (*(volatile unsigned int *)PhyAddr(0x84, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[20] = 0x%X\n", PhyAddr(0x84, u1ChIdx), (u4Value >> 20) & 0x1));

		u4Value = (*(volatile unsigned int *)PhyAddr(0x88, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[31] = 0x%X\n", PhyAddr(0x88, u1ChIdx), (u4Value >> 31) & 0x1));
		mcSHOW_DBG_MSG(("0x%X[27] = 0x%X\n", PhyAddr(0x88, u1ChIdx), (u4Value >> 27) & 0x1));
		mcSHOW_DBG_MSG(("0x%X[19] = 0x%X\n", PhyAddr(0x88, u1ChIdx), (u4Value >> 19) & 0x1));
		mcSHOW_DBG_MSG(("0x%X[17] = 0x%X\n", PhyAddr(0x88, u1ChIdx), (u4Value >> 17) & 0x1));
		mcSHOW_DBG_MSG(("0x%X[15:13] = 0x%X\n", PhyAddr(0x88, u1ChIdx), (u4Value >> 13) & 0x7));
		mcSHOW_DBG_MSG(("0x%X[11:10] = 0x%X\n", PhyAddr(0x88, u1ChIdx), (u4Value >> 10) & 0x3));

		u4Value = (*(volatile unsigned int *)PhyAddr(0x8C, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[19] = 0x%X\n", PhyAddr(0x8C, u1ChIdx), (u4Value >> 19) & 0x1));
		mcSHOW_DBG_MSG(("0x%X[17] = 0x%X\n", PhyAddr(0x8C, u1ChIdx), (u4Value >> 17) & 0x1));
		mcSHOW_DBG_MSG(("0x%X[15:13] = 0x%X\n", PhyAddr(0x8C, u1ChIdx), (u4Value >> 13) & 0x7));
		mcSHOW_DBG_MSG(("0x%X[11] = 0x%X\n", PhyAddr(0x8C, u1ChIdx), (u4Value >> 11) & 0x1));

		u4Value = (*(volatile unsigned int *)PhyAddr(0x104, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[20] = 0x%X\n", PhyAddr(0x104, u1ChIdx), (u4Value >> 20) & 0x1));

		u4Value = (*(volatile unsigned int *)PhyAddr(0x108, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[31] = 0x%X\n", PhyAddr(0x108, u1ChIdx), (u4Value >> 31) & 0x1));
		mcSHOW_DBG_MSG(("0x%X[27] = 0x%X\n", PhyAddr(0x108, u1ChIdx), (u4Value >> 27) & 0x1));
		mcSHOW_DBG_MSG(("0x%X[19] = 0x%X\n", PhyAddr(0x108, u1ChIdx), (u4Value >> 19) & 0x1));
		mcSHOW_DBG_MSG(("0x%X[17] = 0x%X\n", PhyAddr(0x108, u1ChIdx), (u4Value >> 17) & 0x1));
		mcSHOW_DBG_MSG(("0x%X[15:13] = 0x%X\n", PhyAddr(0x108, u1ChIdx), (u4Value >> 13) & 0x7));
		mcSHOW_DBG_MSG(("0x%X[11:10] = 0x%X\n", PhyAddr(0x108, u1ChIdx), (u4Value >> 10) & 0x3));

		u4Value = (*(volatile unsigned int *)PhyAddr(0x10C, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[19] = 0x%X\n", PhyAddr(0x10C, u1ChIdx), (u4Value >> 19) & 0x1));
		mcSHOW_DBG_MSG(("0x%X[17] = 0x%X\n", PhyAddr(0x10C, u1ChIdx), (u4Value >> 17) & 0x1));
		mcSHOW_DBG_MSG(("0x%X[15:13] = 0x%X\n", PhyAddr(0x10C, u1ChIdx), (u4Value >> 13) & 0x7));
		mcSHOW_DBG_MSG(("0x%X[11] = 0x%X\n", PhyAddr(0x10C, u1ChIdx), (u4Value >> 11) & 0x1));

		u4Value = (*(volatile unsigned int *)PhyAddr(0x184, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[20] = 0x%X\n", PhyAddr(0x184, u1ChIdx), (u4Value >> 20) & 0x1));

		u4Value = (*(volatile unsigned int *)PhyAddr(0x188, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[31] = 0x%X\n", PhyAddr(0x188, u1ChIdx), (u4Value >> 31) & 0x1));
		mcSHOW_DBG_MSG(("0x%X[27] = 0x%X\n", PhyAddr(0x188, u1ChIdx), (u4Value >> 27) & 0x1));
		mcSHOW_DBG_MSG(("0x%X[19] = 0x%X\n", PhyAddr(0x188, u1ChIdx), (u4Value >> 19) & 0x1));
		mcSHOW_DBG_MSG(("0x%X[17:15] = 0x%X\n", PhyAddr(0x188, u1ChIdx), (u4Value >> 15) & 0x7));
		mcSHOW_DBG_MSG(("0x%X[13] = 0x%X\n", PhyAddr(0x188, u1ChIdx), (u4Value >> 13) & 0x1));
		mcSHOW_DBG_MSG(("0x%X[11:10] = 0x%X\n", PhyAddr(0x188, u1ChIdx), (u4Value >> 10) & 0x3));

		u4Value = (*(volatile unsigned int *)PhyAddr(0x18C, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[19] = 0x%X\n", PhyAddr(0x18C, u1ChIdx), (u4Value >> 19) & 0x1));
		mcSHOW_DBG_MSG(("0x%X[17] = 0x%X\n", PhyAddr(0x18C, u1ChIdx), (u4Value >> 17) & 0x1));
		mcSHOW_DBG_MSG(("0x%X[16:15] = 0x%X\n", PhyAddr(0x18C, u1ChIdx), (u4Value >> 15) & 0x3));
		mcSHOW_DBG_MSG(("0x%X[13] = 0x%X\n", PhyAddr(0x18C, u1ChIdx), (u4Value >> 13) & 0x1));
		mcSHOW_DBG_MSG(("0x%X[11] = 0x%X\n", PhyAddr(0x18C, u1ChIdx), (u4Value >> 11) & 0x1));

		u4Value = (*(volatile unsigned int *)PhyAddr(0x274, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X = 0x%X\n", PhyAddr(0x274, u1ChIdx), u4Value));

		u4Value = (*(volatile unsigned int *)PhyAddr(0x27C, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X = 0x%X\n", PhyAddr(0x27C, u1ChIdx), u4Value));

		u4Value = (*(volatile unsigned int *)PhyAddr(0x284, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[20:19] = 0x%X\n", PhyAddr(0x284, u1ChIdx), (u4Value >> 19) & 0x3));
		mcSHOW_DBG_MSG(("0x%X[17:8] = 0x%X\n", PhyAddr(0x284, u1ChIdx), (u4Value >> 8) & 0x3FF));

		u4Value = (*(volatile unsigned int *)PhyAddr(0x28C, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X = 0x%X\n", PhyAddr(0x28C, u1ChIdx), u4Value));

		u4Value = (*(volatile unsigned int *)PhyAddr(0x298, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[22:20] = 0x%X\n", PhyAddr(0x298, u1ChIdx), (u4Value >> 20) & 0x7));
		mcSHOW_DBG_MSG(("0x%X[18:16] = 0x%X\n", PhyAddr(0x298, u1ChIdx), (u4Value >> 16) & 0x7));

		u4Value = (*(volatile unsigned int *)PhyAddr(0x29C, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[27] = 0x%X\n", PhyAddr(0x29C, u1ChIdx), (u4Value >> 27) & 0x1));

		u4Value = (*(volatile unsigned int *)PhyAddr(0x2A8, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[27:26] = 0x%X\n", PhyAddr(0x2A8, u1ChIdx), (u4Value >> 26) & 0x3));

		u4Value = (*(volatile unsigned int *)PhyAddr(0xC1C, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[19:17] = 0x%X\n", PhyAddr(0xC1C, u1ChIdx), (u4Value >> 17) & 0x7));

		u4Value = (*(volatile unsigned int *)PhyAddr(0xC20, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[31:19] = 0x%X\n", PhyAddr(0xc20, u1ChIdx), (u4Value >> 19) & 0x1fff));

		u4Value = (*(volatile unsigned int *)PhyAddr(0xC34, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[0] = 0x%X\n", PhyAddr(0xC34, u1ChIdx), (u4Value >> 0) & 0x1));

		u4Value = (*(volatile unsigned int *)PhyAddr(0xC9C, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[19:17] = 0x%X\n", PhyAddr(0xC9C, u1ChIdx), (u4Value >> 17) & 0x7));

		u4Value = (*(volatile unsigned int *)PhyAddr(0xCA0, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[31:19] = 0x%X\n", PhyAddr(0xcA0, u1ChIdx), (u4Value >> 19) & 0x1fff));

		u4Value = (*(volatile unsigned int *)PhyAddr(0xCB4, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[0] = 0x%X\n", PhyAddr(0xCB4, u1ChIdx), (u4Value >> 0) & 0x1));

		u4Value = (*(volatile unsigned int *)PhyAddr(0xD1C, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[19:17] = 0x%X\n", PhyAddr(0xD1C, u1ChIdx), (u4Value >> 17) & 0x7));

		u4Value = (*(volatile unsigned int *)PhyAddr(0xD20, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[31:19] = 0x%X\n", PhyAddr(0xD20, u1ChIdx), (u4Value >> 19) & 0x1fff));

		u4Value = (*(volatile unsigned int *)PhyAddr(0xD34, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[0] = 0x%X\n", PhyAddr(0xD34, u1ChIdx), (u4Value >> 0) & 0x1));

		u4Value = (*(volatile unsigned int *)PhyAddr(0x111C, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[19:17] = 0x%X\n", PhyAddr(0x111C, u1ChIdx), (u4Value >> 17) & 0x7));

		u4Value = (*(volatile unsigned int *)PhyAddr(0x1120, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[31:19] = 0x%X\n", PhyAddr(0x1120, u1ChIdx), (u4Value >> 19) & 0x1fff));


		u4Value = (*(volatile unsigned int *)PhyAddr(0x1134, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[0] = 0x%X\n", PhyAddr(0x1134, u1ChIdx), (u4Value >> 0) & 0x1));

		u4Value = (*(volatile unsigned int *)PhyAddr(0x119C, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[19:17] = 0x%X\n", PhyAddr(0x119C, u1ChIdx), (u4Value >> 17) & 0x7));

		u4Value = (*(volatile unsigned int *)PhyAddr(0x11A0, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[31:19] = 0x%X\n", PhyAddr(0x11A0, u1ChIdx), (u4Value >> 19) & 0x1fff));

		u4Value = (*(volatile unsigned int *)PhyAddr(0x11B4, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[0] = 0x%X\n", PhyAddr(0x11B4, u1ChIdx), (u4Value >> 0) & 0x1));

		u4Value = (*(volatile unsigned int *)PhyAddr(0x121C, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[19:17] = 0x%X\n", PhyAddr(0x121C, u1ChIdx), (u4Value >> 17) & 0x7));

		u4Value = (*(volatile unsigned int *)PhyAddr(0x1220, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[31:19] = 0x%X\n", PhyAddr(0x1220, u1ChIdx), (u4Value >> 19) & 0x1fff));

		u4Value = (*(volatile unsigned int *)PhyAddr(0x1234, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[0] = 0x%X\n", PhyAddr(0x1234, u1ChIdx), (u4Value >> 0) & 0x1));

		u4Value = (*(volatile unsigned int *)PhyAddr(0x161C, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[19:17] = 0x%X\n", PhyAddr(0x161C, u1ChIdx), (u4Value >> 17) & 0x7));

		u4Value = (*(volatile unsigned int *)PhyAddr(0x1620, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[31:19] = 0x%X\n", PhyAddr(0x1620, u1ChIdx), (u4Value >> 19) & 0x1fff));

		u4Value = (*(volatile unsigned int *)PhyAddr(0x1634, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[0] = 0x%X\n", PhyAddr(0x1634, u1ChIdx), (u4Value >> 0) & 0x1));

		u4Value = (*(volatile unsigned int *)PhyAddr(0x169C, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[19:17] = 0x%X\n", PhyAddr(0x169C, u1ChIdx), (u4Value >> 17) & 0x7));

		u4Value = (*(volatile unsigned int *)PhyAddr(0x16A0, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[31:19] = 0x%X\n", PhyAddr(0x16A0, u1ChIdx), (u4Value >> 19) & 0x1fff));

		u4Value = (*(volatile unsigned int *)PhyAddr(0x16B4, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[0] = 0x%X\n", PhyAddr(0x16B4, u1ChIdx), (u4Value >> 0) & 0x1));

		u4Value = (*(volatile unsigned int *)PhyAddr(0x171C, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[19:17] = 0x%X\n", PhyAddr(0x171C, u1ChIdx), (u4Value >> 17) & 0x7));

		u4Value = (*(volatile unsigned int *)PhyAddr(0x1720, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[31:19] = 0x%X\n", PhyAddr(0x1720, u1ChIdx), (u4Value >> 19) & 0x1fff));

		u4Value = (*(volatile unsigned int *)PhyAddr(0x1734, u1ChIdx));
		mcSHOW_DBG_MSG(("0x%X[0] = 0x%X\n", PhyAddr(0x1734, u1ChIdx), (u4Value >> 0) & 0x1));
	}
	return;
}
#endif

#if (SW_CHANGE_FOR_SIMULATION == 0)
/* LP4 2CH */
void LP4EnableDramcPhyDCM2Channel(DRAMC_CTX_T *p, BOOL bEn)/* Should refer to "vSetChannelNumber" */
{
	INT8 i1ShuIdx = 0;
	INT8 i1ShuffleMax = DRAM_DFS_SHUFFLE_MAX;

	if (bEn)/* DCM on */
	{
		vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL0, P_Fld(0x0, MISC_CG_CTRL0_RG_CG_INFRA_OFF_DISABLE)
			| P_Fld(0x0, MISC_CG_CTRL0_RG_CG_RX_COMB1_OFF_DISABLE) | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_RX_COMB0_OFF_DISABLE)
			| P_Fld(0x0, MISC_CG_CTRL0_RG_CG_RX_CMD_OFF_DISABLE) | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_COMB1_OFF_DISABLE)
			| P_Fld(0x0, MISC_CG_CTRL0_RG_CG_COMB0_OFF_DISABLE) | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_CMD_OFF_DISABLE)
			| P_Fld(0x0, MISC_CG_CTRL0_RG_CG_COMB_OFF_DISABLE) | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_PHY_OFF_DIABLE)
			| P_Fld(0x0, MISC_CG_CTRL0_RG_CG_DRAMC_OFF_DISABLE) | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_EMI_OFF_DISABLE)
			| P_Fld(0x0, MISC_CG_CTRL0_RG_CG_DRAMC_CHB_CK_OFF));
		for (i1ShuIdx = DRAM_DFS_SHUFFLE_1; i1ShuIdx < i1ShuffleMax; i1ShuIdx++)
		{
			vIO32WriteFldMulti_All(DDRPHY_SHU1_B0_DQ8 + SHU_GRP_DDRPHY_OFFSET * i1ShuIdx, P_Fld(0x0, SHU1_B0_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B0)
				| P_Fld(0x0, SHU1_B0_DQ8_R_DMRANK_PIPE_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B0)
				| P_Fld(0x0, SHU1_B0_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B0)
				| P_Fld(0x0, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_DMSTBEN_SYNC_CG_IG_B0)
				| P_Fld(0x0, SHU1_B0_DQ8_R_DMRXDLY_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B0)
				| P_Fld(0x0, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B0) | P_Fld(0x1, SHU1_B0_DQ8_R_RMRX_TOPHY_CG_IG_B0)
				| P_Fld(0x0, SHU1_B0_DQ8_R_RMRODTEN_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B0)); /* 19th */
			vIO32WriteFldMulti_All(DDRPHY_SHU1_B1_DQ8 + SHU_GRP_DDRPHY_OFFSET * i1ShuIdx, P_Fld(0x0, SHU1_B1_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B1)
				| P_Fld(0x0, SHU1_B1_DQ8_R_DMRANK_PIPE_CG_IG_B1) | P_Fld(0x0, SHU1_B1_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B1)
				| P_Fld(0x0, SHU1_B1_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B1) | P_Fld(0x0, SHU1_B1_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B1)
				| P_Fld(0x0, SHU1_B1_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B1) | P_Fld(0x0, SHU1_B1_DQ8_R_DMSTBEN_SYNC_CG_IG_B1)
				| P_Fld(0x0, SHU1_B1_DQ8_R_DMRXDLY_CG_IG_B1) | P_Fld(0x0, SHU1_B1_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B1)
				| P_Fld(0x0, SHU1_B1_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B1) | P_Fld(0x1, SHU1_B1_DQ8_R_RMRX_TOPHY_CG_IG_B1)
				| P_Fld(0x0, SHU1_B1_DQ8_R_RMRODTEN_CG_IG_B1) | P_Fld(0x0, SHU1_B1_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B1)); /* 19th */
			vIO32WriteFldMulti_All(DDRPHY_SHU1_CA_CMD8 + SHU_GRP_DDRPHY_OFFSET * i1ShuIdx, P_Fld(0x0, SHU1_CA_CMD8_R_DMRANK_CHG_PIPE_CG_IG_CA)
				| P_Fld(0x0, SHU1_CA_CMD8_R_DMRANK_PIPE_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_CA)
				| P_Fld(0x0, SHU1_CA_CMD8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_PIPE_CG_IG_CA)
				| P_Fld(0x0, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_SYNC_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMSTBEN_SYNC_CG_IG_CA)
				| P_Fld(0x0, SHU1_CA_CMD8_R_DMRXDLY_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_CA)
				| P_Fld(0x0, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_PIPE_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_RMRX_TOPHY_CG_IG_CA)
				| P_Fld(0x0, SHU1_CA_CMD8_R_RMRODTEN_CG_IG_CA)
				| P_Fld(0x0, SHU1_CA_CMD8_R_DMRANK_RXDLY_PIPE_CG_IG_CA)); /* 19th */
		}
		vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL5, P_Fld(0x1, MISC_CG_CTRL5_R_CA_DLY_DCM_EN)
			| P_Fld(0x1, MISC_CG_CTRL5_R_DQ0_DLY_DCM_EN)
			| P_Fld(0x1, MISC_CG_CTRL5_R_DQ1_DLY_DCM_EN)
			| P_Fld(0x1, MISC_CG_CTRL5_R_CA_PI_DCM_EN)
			| P_Fld(0x1, MISC_CG_CTRL5_R_DQ0_PI_DCM_EN)
			| P_Fld(0x1, MISC_CG_CTRL5_R_DQ1_PI_DCM_EN));

	}
	else/* DCM off */
	{
		vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL0, P_Fld(0x1, MISC_CG_CTRL0_RG_CG_INFRA_OFF_DISABLE)
			| P_Fld(0x1, MISC_CG_CTRL0_RG_CG_RX_COMB1_OFF_DISABLE) | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_RX_COMB0_OFF_DISABLE)
			| P_Fld(0x1, MISC_CG_CTRL0_RG_CG_RX_CMD_OFF_DISABLE) | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_COMB1_OFF_DISABLE)
			| P_Fld(0x1, MISC_CG_CTRL0_RG_CG_COMB0_OFF_DISABLE) | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_CMD_OFF_DISABLE)
			| P_Fld(0x1, MISC_CG_CTRL0_RG_CG_COMB_OFF_DISABLE) | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_PHY_OFF_DIABLE)
			| P_Fld(0x1, MISC_CG_CTRL0_RG_CG_DRAMC_OFF_DISABLE) | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_EMI_OFF_DISABLE)
			| P_Fld(0x0, MISC_CG_CTRL0_RG_CG_DRAMC_CHB_CK_OFF));

		for (i1ShuIdx = DRAM_DFS_SHUFFLE_1; i1ShuIdx < i1ShuffleMax; i1ShuIdx++)
		{
			vIO32WriteFldMulti_All(DDRPHY_SHU1_B0_DQ8 + SHU_GRP_DDRPHY_OFFSET * i1ShuIdx, P_Fld(0x1, SHU1_B0_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B0)
				| P_Fld(0x1, SHU1_B0_DQ8_R_DMRANK_PIPE_CG_IG_B0) | P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B0)
				| P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B0) | P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B0)
				| P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B0) | P_Fld(0x1, SHU1_B0_DQ8_R_DMSTBEN_SYNC_CG_IG_B0)
				| P_Fld(0x1, SHU1_B0_DQ8_R_DMRXDLY_CG_IG_B0) | P_Fld(0x1, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B0)
				| P_Fld(0x1, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B0) | P_Fld(0x1, SHU1_B0_DQ8_R_RMRX_TOPHY_CG_IG_B0)
				| P_Fld(0x1, SHU1_B0_DQ8_R_RMRODTEN_CG_IG_B0) | P_Fld(0x1, SHU1_B0_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B0)); /* 19th */
			vIO32WriteFldMulti_All(DDRPHY_SHU1_B1_DQ8 + SHU_GRP_DDRPHY_OFFSET * i1ShuIdx, P_Fld(0x1, SHU1_B1_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B1)
				| P_Fld(0x1, SHU1_B1_DQ8_R_DMRANK_PIPE_CG_IG_B1) | P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B1)
				| P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B1) | P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B1)
				| P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B1) | P_Fld(0x1, SHU1_B1_DQ8_R_DMSTBEN_SYNC_CG_IG_B1)
				| P_Fld(0x1, SHU1_B1_DQ8_R_DMRXDLY_CG_IG_B1) | P_Fld(0x1, SHU1_B1_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B1)
				| P_Fld(0x1, SHU1_B1_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B1) | P_Fld(0x1, SHU1_B1_DQ8_R_RMRX_TOPHY_CG_IG_B1)
				| P_Fld(0x1, SHU1_B1_DQ8_R_RMRODTEN_CG_IG_B1) | P_Fld(0x1, SHU1_B1_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B1)); /* 19th */
			vIO32WriteFldMulti_All(DDRPHY_SHU1_CA_CMD8 + SHU_GRP_DDRPHY_OFFSET * i1ShuIdx, P_Fld(0x0, SHU1_CA_CMD8_R_DMRANK_CHG_PIPE_CG_IG_CA)
				| P_Fld(0x0, SHU1_CA_CMD8_R_DMRANK_PIPE_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_CA)
				| P_Fld(0x0, SHU1_CA_CMD8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_PIPE_CG_IG_CA)
				| P_Fld(0x0, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_SYNC_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMSTBEN_SYNC_CG_IG_CA)
				| P_Fld(0x0, SHU1_CA_CMD8_R_DMRXDLY_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_CA)
				| P_Fld(0x0, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_PIPE_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_RMRX_TOPHY_CG_IG_CA)
				| P_Fld(0x0, SHU1_CA_CMD8_R_RMRODTEN_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMRANK_RXDLY_PIPE_CG_IG_CA)); /* 19th */
		}
		vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL5, P_Fld(0x0, MISC_CG_CTRL5_R_CA_DLY_DCM_EN)
			| P_Fld(0x0, MISC_CG_CTRL5_R_DQ0_DLY_DCM_EN)
			| P_Fld(0x0, MISC_CG_CTRL5_R_DQ1_DLY_DCM_EN)
			| P_Fld(0x0, MISC_CG_CTRL5_R_CA_PI_DCM_EN)
			| P_Fld(0x0, MISC_CG_CTRL5_R_DQ0_PI_DCM_EN)
			| P_Fld(0x0, MISC_CG_CTRL5_R_DQ1_PI_DCM_EN));
	}
	/* LP4 Low power related */
	vIO32WriteFldMulti_All(DDRPHY_B0_DLL_ARPI2, P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCK_B0)
		| P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B0) | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCTL_B0)
		| P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_FB_B0) | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQS_B0)
		| P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQM_B0) | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQ_B0)
		| P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B0) | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0));
	vIO32WriteFldMulti_All(DDRPHY_B0_DLL_ARPI3, P_Fld(0x0, B0_DLL_ARPI3_RG_ARPI_MCTL_EN_B0)
		| P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_FB_EN_B0) | P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQS_EN_B0)
		| P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQM_EN_B0) | P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQ_EN_B0)
		| P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B0));
	vIO32WriteFldMulti_All(DDRPHY_B1_DLL_ARPI2, P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_MCK_B1)
		| P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B1) | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_MCTL_B1)
		| P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_FB_B1) | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQS_B1)
		| P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQM_B1) | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQ_B1)
		| P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B1) | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1));
	vIO32WriteFldMulti_All(DDRPHY_B1_DLL_ARPI3, P_Fld(0x0, B1_DLL_ARPI3_RG_ARPI_MCTL_EN_B1)
		| P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_FB_EN_B1) | P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQS_EN_B1)
		| P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQM_EN_B1) | P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQ_EN_B1)
		| P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B1));
	vIO32WriteFldMulti_All(DDRPHY_CA_DLL_ARPI2, P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCK_CA)
		| P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_CA) | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCTL_CA)
		| P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_FB_CA) | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CS)
		| P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CLK) | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CMD)
		| P_Fld(0x1, CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN) | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA));
	vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI3, P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_MCTL_EN_CA)
		| P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_FB_EN_CA) | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CS_EN)
		| P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CLK_EN) | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CMD_EN)
		| P_Fld(0x0, CA_DLL_ARPI3_RG_ARPI_CLKIEN_EN));
	vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI3 + SHIFT_TO_CHB_ADDR, P_Fld(0x0, CA_DLL_ARPI3_RG_ARPI_MCTL_EN_CA)
		| P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_FB_EN_CA) | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CS_EN)
		| P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CLK_EN) | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CMD_EN)
		| P_Fld(0x0, CA_DLL_ARPI3_RG_ARPI_CLKIEN_EN));

	return;
}


#if ENABLE_LP3_SW
/* LP3 */
void LP3EnableDramcPhyDCM(DRAMC_CTX_T *p, BOOL bEn)/* Should refer to "vSetChannelNumber" */
{
	INT8 i1ShuIdx = 0;
	INT8 i1ShuffleMax = DRAM_DFS_SHUFFLE_MAX;

	if (bEn)/* DCM on */
	{
		vIO32WriteFldMulti(DDRPHY_MISC_CG_CTRL0, P_Fld(0x0, MISC_CG_CTRL0_RG_CG_INFRA_OFF_DISABLE)
			| P_Fld(0x0, MISC_CG_CTRL0_RG_CG_RX_COMB1_OFF_DISABLE) | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_RX_COMB0_OFF_DISABLE)
			| P_Fld(0x0, MISC_CG_CTRL0_RG_CG_RX_CMD_OFF_DISABLE) | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_COMB1_OFF_DISABLE)
			| P_Fld(0x0, MISC_CG_CTRL0_RG_CG_COMB0_OFF_DISABLE) | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_CMD_OFF_DISABLE)
			| P_Fld(0x0, MISC_CG_CTRL0_RG_CG_COMB_OFF_DISABLE) | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_PHY_OFF_DIABLE)
			| P_Fld(0x0, MISC_CG_CTRL0_RG_CG_DRAMC_OFF_DISABLE) | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_EMI_OFF_DISABLE)
			| P_Fld(0x0, MISC_CG_CTRL0_RG_CG_DRAMC_CHB_CK_OFF)); /* 20th */
		vIO32WriteFldMulti(SHIFT_TO_CHB_ADDR + DDRPHY_MISC_CG_CTRL0, P_Fld(0x0, MISC_CG_CTRL0_RG_CG_INFRA_OFF_DISABLE)
			| P_Fld(0x0, MISC_CG_CTRL0_RG_CG_RX_COMB1_OFF_DISABLE) | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_RX_COMB0_OFF_DISABLE)
			| P_Fld(0x0, MISC_CG_CTRL0_RG_CG_RX_CMD_OFF_DISABLE) | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_COMB1_OFF_DISABLE)
			| P_Fld(0x0, MISC_CG_CTRL0_RG_CG_COMB0_OFF_DISABLE) | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_CMD_OFF_DISABLE)
			| P_Fld(0x0, MISC_CG_CTRL0_RG_CG_COMB_OFF_DISABLE) | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_PHY_OFF_DIABLE)
			| P_Fld(0x0, MISC_CG_CTRL0_RG_CG_DRAMC_OFF_DISABLE) | P_Fld(0x0, MISC_CG_CTRL0_RG_CG_EMI_OFF_DISABLE)
			| P_Fld(0x1, MISC_CG_CTRL0_RG_CG_DRAMC_CHB_CK_OFF)); /* 20th */
		for (i1ShuIdx = DRAM_DFS_SHUFFLE_1; i1ShuIdx < i1ShuffleMax; i1ShuIdx++)
		{
			vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ8 + SHU_GRP_DDRPHY_OFFSET * i1ShuIdx, P_Fld(0x0, SHU1_B0_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B0)
				| P_Fld(0x0, SHU1_B0_DQ8_R_DMRANK_PIPE_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B0)
				| P_Fld(0x0, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_DMSTBEN_SYNC_CG_IG_B0)
				| P_Fld(0x0, SHU1_B0_DQ8_R_DMRXDLY_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B0)
				| P_Fld(0x0, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_RMRX_TOPHY_CG_IG_B0)
				| P_Fld(0x0, SHU1_B0_DQ8_R_RMRODTEN_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B0)); /* 19th */
			vIO32WriteFldMulti(SHIFT_TO_CHB_ADDR + DDRPHY_SHU1_B0_DQ8 + SHU_GRP_DDRPHY_OFFSET * i1ShuIdx, P_Fld(0x0, SHU1_B0_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B0)
				| P_Fld(0x0, SHU1_B0_DQ8_R_DMRANK_PIPE_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B0)
				| P_Fld(0x0, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_DMSTBEN_SYNC_CG_IG_B0)
				| P_Fld(0x0, SHU1_B0_DQ8_R_DMRXDLY_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B0)
				| P_Fld(0x0, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B0) | P_Fld(0x1, SHU1_B0_DQ8_R_RMRX_TOPHY_CG_IG_B0)
				| P_Fld(0x0, SHU1_B0_DQ8_R_RMRODTEN_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B0)); /* 19th */
			vIO32WriteFldMulti_All(DDRPHY_SHU1_B1_DQ8 + SHU_GRP_DDRPHY_OFFSET * i1ShuIdx, P_Fld(0x0, SHU1_B1_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B1)
				| P_Fld(0x0, SHU1_B1_DQ8_R_DMRANK_PIPE_CG_IG_B1) | P_Fld(0x0, SHU1_B1_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B1)
				| P_Fld(0x0, SHU1_B1_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B1) | P_Fld(0x0, SHU1_B1_DQ8_R_DMSTBEN_SYNC_CG_IG_B1)
				| P_Fld(0x0, SHU1_B1_DQ8_R_DMRXDLY_CG_IG_B1) | P_Fld(0x0, SHU1_B1_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B1)
				| P_Fld(0x0, SHU1_B1_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B1) | P_Fld(0x1, SHU1_B1_DQ8_R_RMRX_TOPHY_CG_IG_B1)
				| P_Fld(0x0, SHU1_B1_DQ8_R_RMRODTEN_CG_IG_B1) | P_Fld(0x0, SHU1_B1_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B1)); /* 19th */
			vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD8 + SHU_GRP_DDRPHY_OFFSET * i1ShuIdx, P_Fld(0x0, SHU1_CA_CMD8_R_DMRANK_CHG_PIPE_CG_IG_CA)
				| P_Fld(0x0, SHU1_CA_CMD8_R_DMRANK_PIPE_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_PIPE_CG_IG_CA)
				| P_Fld(0x0, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_SYNC_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMSTBEN_SYNC_CG_IG_CA)
				| P_Fld(0x0, SHU1_CA_CMD8_R_DMRXDLY_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_CA)
				| P_Fld(0x0, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_PIPE_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_RMRX_TOPHY_CG_IG_CA)
				| P_Fld(0x0, SHU1_CA_CMD8_R_RMRODTEN_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMRANK_RXDLY_PIPE_CG_IG_CA)); /* 19th */
			/* Lewis@20171218: Set CHB RX CA{LP3 B2} 24th bit to 1{free run} as the temp solution of bit flip in LP3 B2 */
			vIO32WriteFldMulti(SHIFT_TO_CHB_ADDR + DDRPHY_SHU1_CA_CMD8 + SHU_GRP_DDRPHY_OFFSET * i1ShuIdx, P_Fld(0x0, SHU1_CA_CMD8_R_DMRANK_CHG_PIPE_CG_IG_CA)
				| P_Fld(0x0, SHU1_CA_CMD8_R_DMRANK_PIPE_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_PIPE_CG_IG_CA)
				| P_Fld(0x0, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_SYNC_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMSTBEN_SYNC_CG_IG_CA)
				| P_Fld(0x1, SHU1_CA_CMD8_R_DMRXDLY_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_CA)
				| P_Fld(0x0, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_PIPE_CG_IG_CA) | P_Fld(0x1, SHU1_CA_CMD8_R_RMRX_TOPHY_CG_IG_CA)
				| P_Fld(0x0, SHU1_CA_CMD8_R_RMRODTEN_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMRANK_RXDLY_PIPE_CG_IG_CA)); /* 19th */
		}
		/* Lewis@20170421: LP3 should set DCM off(CHA:DQ0, CHB:DQ0 DQ1 CA) since DCM on will degrade TX DQ/DQS window to 4 PI while at the first write after DVFS(DE:Lynx.Lin) */
		vIO32WriteFldMulti(DDRPHY_MISC_CG_CTRL5, P_Fld(0x1, MISC_CG_CTRL5_R_CA_DLY_DCM_EN)
			| P_Fld(0x1, MISC_CG_CTRL5_R_DQ0_DLY_DCM_EN)
			| P_Fld(0x1, MISC_CG_CTRL5_R_DQ1_DLY_DCM_EN)
			| P_Fld(0x1, MISC_CG_CTRL5_R_CA_PI_DCM_EN)
			| P_Fld(0x0, MISC_CG_CTRL5_R_DQ0_PI_DCM_EN)
			| P_Fld(0x1, MISC_CG_CTRL5_R_DQ1_PI_DCM_EN));
		vIO32WriteFldMulti(SHIFT_TO_CHB_ADDR + DDRPHY_MISC_CG_CTRL5, P_Fld(0x1, MISC_CG_CTRL5_R_CA_DLY_DCM_EN)
			| P_Fld(0x1, MISC_CG_CTRL5_R_DQ0_DLY_DCM_EN)
			| P_Fld(0x1, MISC_CG_CTRL5_R_DQ1_DLY_DCM_EN)
			| P_Fld(0x1, MISC_CG_CTRL5_R_CA_PI_DCM_EN)/* Differnet from 6771, PIC: Lynx */
			| P_Fld(0x0, MISC_CG_CTRL5_R_DQ0_PI_DCM_EN)
			| P_Fld(0x1, MISC_CG_CTRL5_R_DQ1_PI_DCM_EN)); /* Different from 6771, PIC: Lynx */
	}
	else/* DCM off */
	{
		vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL0, P_Fld(0x1, MISC_CG_CTRL0_RG_CG_INFRA_OFF_DISABLE)
			| P_Fld(0x1, MISC_CG_CTRL0_RG_CG_RX_COMB1_OFF_DISABLE) | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_RX_COMB0_OFF_DISABLE)
			| P_Fld(0x1, MISC_CG_CTRL0_RG_CG_RX_CMD_OFF_DISABLE) | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_COMB1_OFF_DISABLE)
			| P_Fld(0x1, MISC_CG_CTRL0_RG_CG_COMB0_OFF_DISABLE) | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_CMD_OFF_DISABLE)
			| P_Fld(0x1, MISC_CG_CTRL0_RG_CG_COMB_OFF_DISABLE) | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_PHY_OFF_DIABLE)
			| P_Fld(0x1, MISC_CG_CTRL0_RG_CG_DRAMC_OFF_DISABLE) | P_Fld(0x1, MISC_CG_CTRL0_RG_CG_EMI_OFF_DISABLE)
			| P_Fld(0x0, MISC_CG_CTRL0_RG_CG_DRAMC_CHB_CK_OFF)); /* 20th */
		for (i1ShuIdx = DRAM_DFS_SHUFFLE_1; i1ShuIdx < i1ShuffleMax; i1ShuIdx++)
		{
			vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ8 + SHU_GRP_DDRPHY_OFFSET * i1ShuIdx, P_Fld(0x0, SHU1_B0_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B0)
				| P_Fld(0x0, SHU1_B0_DQ8_R_DMRANK_PIPE_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B0)
				| P_Fld(0x0, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_DMSTBEN_SYNC_CG_IG_B0)
				| P_Fld(0x0, SHU1_B0_DQ8_R_DMRXDLY_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B0)
				| P_Fld(0x0, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_RMRX_TOPHY_CG_IG_B0)
				| P_Fld(0x0, SHU1_B0_DQ8_R_RMRODTEN_CG_IG_B0) | P_Fld(0x0, SHU1_B0_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B0)); /* 19th */
			vIO32WriteFldMulti(SHIFT_TO_CHB_ADDR + DDRPHY_SHU1_B0_DQ8 + SHU_GRP_DDRPHY_OFFSET * i1ShuIdx, P_Fld(0x1, SHU1_B0_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B0)
				| P_Fld(0x1, SHU1_B0_DQ8_R_DMRANK_PIPE_CG_IG_B0) | P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B0)
				| P_Fld(0x1, SHU1_B0_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B0) | P_Fld(0x1, SHU1_B0_DQ8_R_DMSTBEN_SYNC_CG_IG_B0)
				| P_Fld(0x1, SHU1_B0_DQ8_R_DMRXDLY_CG_IG_B0) | P_Fld(0x1, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B0)
				| P_Fld(0x1, SHU1_B0_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B0) | P_Fld(0x1, SHU1_B0_DQ8_R_RMRX_TOPHY_CG_IG_B0)
				| P_Fld(0x1, SHU1_B0_DQ8_R_RMRODTEN_CG_IG_B0) | P_Fld(0x1, SHU1_B0_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B0)); /* 19th */
			vIO32WriteFldMulti_All(DDRPHY_SHU1_B1_DQ8 + SHU_GRP_DDRPHY_OFFSET * i1ShuIdx, P_Fld(0x1, SHU1_B1_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B1)
				| P_Fld(0x1, SHU1_B1_DQ8_R_DMRANK_PIPE_CG_IG_B1) | P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B1)
				| P_Fld(0x1, SHU1_B1_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B1) | P_Fld(0x1, SHU1_B1_DQ8_R_DMSTBEN_SYNC_CG_IG_B1)
				| P_Fld(0x1, SHU1_B1_DQ8_R_DMRXDLY_CG_IG_B1) | P_Fld(0x1, SHU1_B1_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B1)
				| P_Fld(0x1, SHU1_B1_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B1) | P_Fld(0x1, SHU1_B1_DQ8_R_RMRX_TOPHY_CG_IG_B1)
				| P_Fld(0x1, SHU1_B1_DQ8_R_RMRODTEN_CG_IG_B1) | P_Fld(0x1, SHU1_B1_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B1)); /* 19th */
			vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD8 + SHU_GRP_DDRPHY_OFFSET * i1ShuIdx, P_Fld(0x0, SHU1_CA_CMD8_R_DMRANK_CHG_PIPE_CG_IG_CA)
				| P_Fld(0x0, SHU1_CA_CMD8_R_DMRANK_PIPE_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_PIPE_CG_IG_CA)
				| P_Fld(0x0, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_SYNC_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMSTBEN_SYNC_CG_IG_CA)
				| P_Fld(0x0, SHU1_CA_CMD8_R_DMRXDLY_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_CA)
				| P_Fld(0x0, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_PIPE_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_RMRX_TOPHY_CG_IG_CA)
				| P_Fld(0x0, SHU1_CA_CMD8_R_RMRODTEN_CG_IG_CA) | P_Fld(0x0, SHU1_CA_CMD8_R_DMRANK_RXDLY_PIPE_CG_IG_CA)); /* 19th */
			vIO32WriteFldMulti(SHIFT_TO_CHB_ADDR + DDRPHY_SHU1_CA_CMD8 + SHU_GRP_DDRPHY_OFFSET * i1ShuIdx, P_Fld(0x1, SHU1_CA_CMD8_R_DMRANK_CHG_PIPE_CG_IG_CA)
				| P_Fld(0x1, SHU1_CA_CMD8_R_DMRANK_PIPE_CG_IG_CA) | P_Fld(0x1, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_PIPE_CG_IG_CA)
				| P_Fld(0x1, SHU1_CA_CMD8_R_DMDQSIEN_FLAG_SYNC_CG_IG_CA) | P_Fld(0x1, SHU1_CA_CMD8_R_DMSTBEN_SYNC_CG_IG_CA)
				| P_Fld(0x1, SHU1_CA_CMD8_R_DMRXDLY_CG_IG_CA) | P_Fld(0x1, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_CA)
				| P_Fld(0x1, SHU1_CA_CMD8_R_DMRXDVS_RDSEL_PIPE_CG_IG_CA) | P_Fld(0x1, SHU1_CA_CMD8_R_RMRX_TOPHY_CG_IG_CA)
				| P_Fld(0x1, SHU1_CA_CMD8_R_RMRODTEN_CG_IG_CA) | P_Fld(0x1, SHU1_CA_CMD8_R_DMRANK_RXDLY_PIPE_CG_IG_CA)); /* 19th */
		}
		vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL5, P_Fld(0x0, MISC_CG_CTRL5_R_CA_PI_DCM_EN)
			| P_Fld(0x0, MISC_CG_CTRL5_R_DQ0_PI_DCM_EN)
			| P_Fld(0x0, MISC_CG_CTRL5_R_DQ1_PI_DCM_EN)
			| P_Fld(0x0, MISC_CG_CTRL5_R_CA_DLY_DCM_EN)
			| P_Fld(0x0, MISC_CG_CTRL5_R_DQ0_DLY_DCM_EN)
			| P_Fld(0x0, MISC_CG_CTRL5_R_DQ1_DLY_DCM_EN));
	}

	/* LP3 Low power related */
	vIO32WriteFldMulti(DDRPHY_B0_DLL_ARPI2, P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCK_B0)
		| P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B0) | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCTL_B0)
		| P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_FB_B0) | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQS_B0)
		| P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQM_B0) | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQ_B0)
		| P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B0) | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0));
	vIO32WriteFldMulti(DDRPHY_B0_DLL_ARPI2 + SHIFT_TO_CHB_ADDR, P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCK_B0)
		| P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B0) | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCTL_B0)
		| P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_FB_B0) | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQS_B0)
		| P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQM_B0) | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQ_B0)
		| P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B0) | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0));
	vIO32WriteFldMulti(DDRPHY_B0_DLL_ARPI3, P_Fld(0x0, B0_DLL_ARPI3_RG_ARPI_MCTL_EN_B0)
		| P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_FB_EN_B0) | P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQS_EN_B0)
		| P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQM_EN_B0) | P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQ_EN_B0)
		| P_Fld(0x0, B0_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B0));
	vIO32WriteFldMulti(DDRPHY_B0_DLL_ARPI3 + SHIFT_TO_CHB_ADDR, P_Fld(0x0, B0_DLL_ARPI3_RG_ARPI_MCTL_EN_B0)
		| P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_FB_EN_B0) | P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQS_EN_B0)
		| P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQM_EN_B0) | P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQ_EN_B0)
		| P_Fld(0x1, B0_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B0));
	vIO32WriteFldMulti_All(DDRPHY_B1_DLL_ARPI2, P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_MCK_B1)
		| P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B1) | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_MCTL_B1)
		| P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_FB_B1) | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQS_B1)
		| P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQM_B1) | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQ_B1)
		| P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B1) | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1));
	vIO32WriteFldMulti_All(DDRPHY_B1_DLL_ARPI3, P_Fld(0x0, B1_DLL_ARPI3_RG_ARPI_MCTL_EN_B1)
		| P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_FB_EN_B1) | P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQS_EN_B1)
		| P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQM_EN_B1) | P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQ_EN_B1)
		| P_Fld(0x1, B1_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B1));
	vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI2, P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCK_CA)
		| P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_CA) | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCTL_CA)
		| P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_FB_CA) | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CS)
		| P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CLK) | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CMD)
		| P_Fld(0x1, CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN) | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA));
	vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI2 + SHIFT_TO_CHB_ADDR, P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCK_CA)
		| P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_CA) | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCTL_CA)
		| P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_FB_CA) | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CS)
		| P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CLK) | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CMD)
		| P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN) | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA));
	vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI3, P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_MCTL_EN_CA)
		| P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_FB_EN_CA) | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CS_EN)
		| P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CLK_EN) | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CMD_EN)
		| P_Fld(0x0, CA_DLL_ARPI3_RG_ARPI_CLKIEN_EN));
	vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI3 + SHIFT_TO_CHB_ADDR, P_Fld(0x0, CA_DLL_ARPI3_RG_ARPI_MCTL_EN_CA)
		| P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_FB_EN_CA) | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CS_EN)
		| P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CLK_EN) | P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CMD_EN)
		| P_Fld(0x1, CA_DLL_ARPI3_RG_ARPI_CLKIEN_EN));

	return;
}
#endif

/* Common part + different by LP4_2CH, LP3, LP4_1CH */
void EnableDramcPhyDCM(DRAMC_CTX_T *p, BOOL bEn)/* Should refer to "vSetChannelNumber" */
{
	U32 u4WbrBackup = GetDramcBroadcast(); /* Just for bring up */
	INT8 i1ShuIdx = 0;
	INT8 i1ShuffleMax = DRAM_DFS_SHUFFLE_MAX;

	DramcBroadcastOnOff(DRAMC_BROADCAST_OFF); /* Just for bring up */

	/* PIC:Lynx, different from 6771, related to PHY 0x298[22][20] */
	vIO32WriteFldAlign_All(DDRPHY_MISC_CTRL0, 0x1, MISC_CTRL0_R_DMSHU_PHYDCM_FORCEOFF);

	/* APHY PICG DQ & DQM PIC:JOE */
	vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI1, 0x0, B0_DLL_ARPI1_RG_ARPISM_MCK_SEL_B0_REG_OPT);
	vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI1, 0x0, B1_DLL_ARPI1_RG_ARPISM_MCK_SEL_B1_REG_OPT);
	vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI1, 0x0, CA_DLL_ARPI1_RG_ARPISM_MCK_SEL_CA_REG_OPT);

	for (i1ShuIdx = DRAM_DFS_SHUFFLE_1; i1ShuIdx < i1ShuffleMax; i1ShuIdx++)/* Joe@20171020: the group of 0xc1c[19:17] is 3'b1 then the value should be 1, else 0 or 1 is ok. */
	{
		vIO32WriteFldAlign_All(DDRPHY_SHU1_B0_DLL0 + SHU_GRP_DDRPHY_OFFSET * i1ShuIdx, 0x1, SHU1_B0_DLL0_RG_ARPISM_MCK_SEL_B0_SHU); /* PI_CLOCK_CG_IMPROVEMENT PIC:JOE */
		vIO32WriteFldAlign_All(DDRPHY_SHU1_B1_DLL0 + SHU_GRP_DDRPHY_OFFSET * i1ShuIdx, 0x1, SHU1_B1_DLL0_RG_ARPISM_MCK_SEL_B1_SHU); /* PI_CLOCK_CG_IMPROVEMENT PIC:JOE */
		vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_DLL0 + SHU_GRP_DDRPHY_OFFSET * i1ShuIdx, 0x1, SHU1_CA_DLL0_RG_ARPISM_MCK_SEL_CA_SHU); /* PI_CLOCK_CG_IMPROVEMENT PIC:JOE */
	}

	if (bEn)/* DCM on */
	{
		vIO32WriteFldMulti_All(DRAMC_REG_DRAMC_PD_CTRL, P_Fld(0x1, DRAMC_PD_CTRL_DCMENNOTRFC)
			| P_Fld(0x1, DRAMC_PD_CTRL_COMBCLKCTRL)
			| P_Fld(0x1, DRAMC_PD_CTRL_PHYCLKDYNGEN)
			| P_Fld(0x1, DRAMC_PD_CTRL_DCMEN) | P_Fld(0x0, DRAMC_PD_CTRL_COMBPHY_CLKENSAME)
			| P_Fld(0x1, DRAMC_PD_CTRL_DCMEN2));
		/* mem_dcm */
		if (u1IsLP4Family(p->dram_type)) {
			vIO32Write4B_All(DDRPHY_MISC_CG_CTRL2, 0x006003AE); /* divided freq change to 1 / 4 */
			vIO32Write4B_All(DDRPHY_MISC_CG_CTRL2, 0x006003AF); /* divided freq change to 1 / 4 */
			vIO32Write4B_All(DDRPHY_MISC_CG_CTRL2, 0x006003AE); /* divided freq change to 1 / 4 */
		} else  {
		#if ENABLE_LP3_SW
			vIO32Write4B_All(DDRPHY_MISC_CG_CTRL2, 0x006003A0); /* divided freq change to 1 / 4 */
			vIO32Write4B_All(DDRPHY_MISC_CG_CTRL2, 0x006003A0); /* divided freq change to 1 / 4 */
			vIO32Write4B_All(DDRPHY_MISC_CG_CTRL2, 0x006003A0); /* divided freq change to 1 / 4 */
		#endif
		}
		/* TX pipe/sync cell CG */
		vIO32WriteFldMulti_All(DDRPHY_MISC_CTRL3, P_Fld(0x0, MISC_CTRL3_R_DDRPHY_COMB_CG_IG)
			| P_Fld(0x0, MISC_CTRL3_R_DDRPHY_RX_PIPE_CG_IG));
		for (i1ShuIdx = DRAM_DFS_SHUFFLE_1; i1ShuIdx < i1ShuffleMax; i1ShuIdx++)
		{
			vIO32WriteFldMulti_All(DDRPHY_SHU1_B0_DQ7 + SHU_GRP_DDRPHY_OFFSET * i1ShuIdx, P_Fld(0x1, SHU1_B0_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B0)
				| P_Fld(0x1, SHU1_B0_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B0)
				| P_Fld(0x1, SHU1_B0_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B0)); /* PI_CLOCK_CG_IMPROVEMENT PIC:JOE */
			vIO32WriteFldMulti_All(DDRPHY_SHU1_B1_DQ7 + SHU_GRP_DDRPHY_OFFSET * i1ShuIdx, P_Fld(0x1, SHU1_B1_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B1)
				| P_Fld(0x1, SHU1_B1_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B1)
				| P_Fld(0x1, SHU1_B1_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B1)); /* PI_CLOCK_CG_IMPROVEMENT PIC:JOE */
			vIO32WriteFldMulti_All(DDRPHY_SHU1_CA_CMD7 + SHU_GRP_DDRPHY_OFFSET * i1ShuIdx, P_Fld(0x1, SHU1_CA_CMD7_R_DMTX_ARPI_CG_CS_NEW)
				| P_Fld(0x1, SHU1_CA_CMD7_R_DMTX_ARPI_CG_CLK_NEW)
				| P_Fld(0x1, SHU1_CA_CMD7_R_DMTX_ARPI_CG_CMD_NEW)); /* PI_CLOCK_CG_IMPROVEMENT PIC:JOE */
		}
	}
	else/* DCM off */
	{
		vIO32WriteFldMulti_All( DRAMC_REG_DRAMC_PD_CTRL, P_Fld(0x0, DRAMC_PD_CTRL_DCMENNOTRFC)
			| P_Fld(0x0, DRAMC_PD_CTRL_COMBCLKCTRL)
			| P_Fld(0x0, DRAMC_PD_CTRL_PHYCLKDYNGEN)
			| P_Fld(0x0, DRAMC_PD_CTRL_DCMEN) | P_Fld(0x1, DRAMC_PD_CTRL_COMBPHY_CLKENSAME)
			| P_Fld(0x0, DRAMC_PD_CTRL_DCMEN2));
		/* mem_dcm */
		if (u1IsLP4Family(p->dram_type)) {
			vIO32Write4B_All(DDRPHY_MISC_CG_CTRL2, 0x0060036E); /* divided freq change to 1 / 4 */
			vIO32Write4B_All(DDRPHY_MISC_CG_CTRL2, 0x0060036F); /* divided freq change to 1 / 4 */
			vIO32Write4B_All(DDRPHY_MISC_CG_CTRL2, 0x0060036E); /* divided freq change to 1 / 4 */
		} else  {
		#if ENABLE_LP3_SW
			vIO32Write4B_All(DDRPHY_MISC_CG_CTRL2, 0x00600360); /* divided freq change to 1 / 4 */
			vIO32Write4B_All(DDRPHY_MISC_CG_CTRL2, 0x00600360); /* divided freq change to 1 / 4 */
			vIO32Write4B_All(DDRPHY_MISC_CG_CTRL2, 0x00600360); /* divided freq change to 1 / 4 */
		#endif
		}
		/* TX pipe/sync cell CG */
		vIO32WriteFldMulti_All(DDRPHY_MISC_CTRL3, P_Fld(0x1, MISC_CTRL3_R_DDRPHY_COMB_CG_IG)
			| P_Fld(0x1, MISC_CTRL3_R_DDRPHY_RX_PIPE_CG_IG));
		for (i1ShuIdx = DRAM_DFS_SHUFFLE_1; i1ShuIdx < i1ShuffleMax; i1ShuIdx++)
		{
			vIO32WriteFldMulti_All(DDRPHY_SHU1_B0_DQ7 + SHU_GRP_DDRPHY_OFFSET * i1ShuIdx, P_Fld(0x0, SHU1_B0_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B0)
				| P_Fld(0x0, SHU1_B0_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B0)
				| P_Fld(0x0, SHU1_B0_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B0)); /* PI_CLOCK_CG_IMPROVEMENT PIC:JOE */
			vIO32WriteFldMulti_All(DDRPHY_SHU1_B1_DQ7 + SHU_GRP_DDRPHY_OFFSET * i1ShuIdx, P_Fld(0x0, SHU1_B1_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B1)
				| P_Fld(0x0, SHU1_B1_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B1)
				| P_Fld(0x0, SHU1_B1_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B1)); /* PI_CLOCK_CG_IMPROVEMENT PIC:JOE */
			vIO32WriteFldMulti_All(DDRPHY_SHU1_CA_CMD7 + SHU_GRP_DDRPHY_OFFSET * i1ShuIdx, P_Fld(0x0, SHU1_CA_CMD7_R_DMTX_ARPI_CG_CS_NEW)
				| P_Fld(0x0, SHU1_CA_CMD7_R_DMTX_ARPI_CG_CLK_NEW)
				| P_Fld(0x0, SHU1_CA_CMD7_R_DMTX_ARPI_CG_CMD_NEW)); /* PI_CLOCK_CG_IMPROVEMENT PIC:JOE */
		}
	}
	/* Different part */
	if (u1IsLP4Family(p->dram_type))
	{
		LP4EnableDramcPhyDCM2Channel(p, bEn);
	}
#if ENABLE_LP3_SW
	else/* LP3 */
	{
		LP3EnableDramcPhyDCM(p, bEn);
	}
#endif
	DramcBroadcastOnOff(u4WbrBackup); /* Just for bring up */
}
#endif

void DummyReadForDqsGatingRetry(DRAMC_CTX_T *p, bool bEn)
{
	U8 u1ShuIdx = 0, u1ShuCnt = DRAM_DFS_SHUFFLE_MAX; /* TODO: change u1ShuCnt to actual shuffle num define */
	U32 u4targetAddr = 0;

	if (bEn == 1)
	{
		for (u1ShuIdx = DRAM_DFS_SHUFFLE_1; u1ShuIdx < u1ShuCnt; u1ShuIdx++)
		{
			u4targetAddr = DRAMC_REG_SHU_DQSG_RETRY + SHU_GRP_DRAMC_OFFSET * u1ShuIdx;
			vIO32WriteFldMulti_All(u4targetAddr, P_Fld(0, SHU_DQSG_RETRY_R_RETRY_ROUND_NUM)/* Retry once */
				| P_Fld(1, SHU_DQSG_RETRY_R_XSR_RETRY_SPM_MODE)
				| P_Fld(0, SHU_DQSG_RETRY_R_XSR_DQSG_RETRY_EN)
				| P_Fld(0, SHU_DQSG_RETRY_R_DQSG_RETRY_SW_EN)
				| P_Fld(1, SHU_DQSG_RETRY_R_RETRY_USE_BURST_MDOE)
				| P_Fld(1, SHU_DQSG_RETRY_R_RDY_SEL_DLE)
				| P_Fld(1, SHU_DQSG_RETRY_R_DDR1866_PLUS));
		}
		vIO32WriteFldAlign_All(DRAMC_REG_TEST2_4, 4, TEST2_4_TESTAGENTRKSEL); /* Dummy Read rank selection is controlled by Test Agent */
		vIO32WriteFldMulti_All(DRAMC_REG_DUMMY_RD, P_Fld(1, DUMMY_RD_DQSG_DMYRD_EN)
			| P_Fld(p->support_rank_num, DUMMY_RD_RANK_NUM)
			| P_Fld(1, DUMMY_RD_DUMMY_RD_SW));
		vIO32WriteFldAlign_All(DRAMC_REG_RK1_DUMMY_RD_ADR, 0, RK1_DUMMY_RD_ADR_DMY_RD_RK1_LEN);
		vIO32WriteFldAlign_All(DRAMC_REG_RK0_DUMMY_RD_ADR, 0, RK0_DUMMY_RD_ADR_DMY_RD_RK0_LEN);
	}
	else
	{
		for (u1ShuIdx = DRAM_DFS_SHUFFLE_1; u1ShuIdx < u1ShuCnt; u1ShuIdx++)
		{
			u4targetAddr = DRAMC_REG_SHU_DQSG_RETRY + SHU_GRP_DRAMC_OFFSET * u1ShuIdx;
			vIO32WriteFldMulti_All(u4targetAddr, P_Fld(0, SHU_DQSG_RETRY_R_XSR_RETRY_SPM_MODE)
				| P_Fld(0, SHU_DQSG_RETRY_R_XSR_DQSG_RETRY_EN)
				| P_Fld(0, SHU_DQSG_RETRY_R_DQSG_RETRY_SW_EN));
		}
	}
}

void HwSaveForSR(DRAMC_CTX_T *p)
{
	vIO32WriteFldMulti_All(DRAMC_REG_RSTMASK, P_Fld(0, RSTMASK_GT_SYNC_MASK)
		| P_Fld(0, RSTMASK_GT_SYNC_MASK_FOR_PHY));
	vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL1, 1, REFCTRL1_SLEFREF_AUTOSAVE_EN);
	vIO32WriteFldMulti_All(DRAMC_REG_SREFCTRL, P_Fld(1, SREFCTRL_SREF2_OPTION)
		| P_Fld(0, SREFCTRL_SREF3_OPTION));
}


/* ------------------------------------------------------------------------- */
/* * vCKERankCtrl
 * Control CKE toggle mode (toggle both ranks 1. at the same time (CKE_RANK_DEPENDENT) 2. individually (CKE_RANK_INDEPENDENT))
 * Note: Sets CKE toggle mode for all channels
 * @param p                Pointer of context created by DramcCtxCreate.
 * @param CKECtrlMode      Indicates
 */
/* ------------------------------------------------------------------------- */
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
	/* Select CKE control mode */
	CKE_Mode = (CKECtrlMode == CKE_RANK_INDEPENDENT) ? CKE_Rank_Independent : CKE_Rank_Dependent;

	/* Apply CKE control mode register settings */
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


#if ENABLE_TMRRI_NEW_MODE
void SetCKE2RankIndependent(DRAMC_CTX_T *p)
{
#if ENABLE_TMRRI_NEW_MODE/* Newly added CKE control mode API */
	mcSHOW_DBG_MSG(("SET_CKE_2_RANK_INDEPENDENT_RUN_TIME: ON\n"));
	vCKERankCtrl(p, CKE_RANK_INDEPENDENT);
#else /* Legacy individual CKE control register settings */
	mcSHOW_DBG_MSG(("SET_CKE_2_RANK_INDEPENDENT_RUN_TIME: OFF\n"));
	vCKERankCtrl(p, CKE_RANK_DEPENDENT);
#endif
}
#endif

#ifdef CLK_FREE_FUN_FOR_DRAMC_PSEL
/*
 * If dramc enter SREF and power down, all configure need to sync 2T again after exit SREF.
 * If Psel is 1, clock will be free run at the periof of 2T to let conf be applied.
 * If Psel is 0, Clock will be gated
 */
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
	U8 u1ShuIdx = 0, u1ShuCnt = 3; /* TODO: change u1ShuCnt to actual shuffle num define */
	U32 u4targetAddr = 0; /* For SHU_ODTCTRL_RODTENSTB_SELPH_CG_IG, SHU_ODTCTRL_RODTEN_SELPH_CG_IG shuffle regs */

	vIO32WriteFldMulti_All(DRAMC_REG_CLKAR, P_Fld(0, CLKAR_REQQUE_PACG_DIS)
		| P_Fld(0, CLKAR_SELPH_CMD_CG_DIS));
	/*
	 * Dummy_RD_PA_OPT should be set to 1, or else some functions would fail (YH Tsai)
	 * Already set to 1 in in UpdateInitialSettings(), so comment out set to 0 here
	 */
	/* vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD), 0, DUMMY_RD_DUMMY_RD_PA_OPT); */
	vIO32WriteFldMulti_All(DRAMC_REG_SREFCTRL, P_Fld(0, SREFCTRL_SCSM_CGAR)
		| P_Fld(0, SREFCTRL_SCARB_SM_CGAR)
		| P_Fld(0, SREFCTRL_RDDQSOSC_CGAR)
		| P_Fld(0, SREFCTRL_HMRRSEL_CGAR));
	vIO32WriteFldAlign_All(DRAMC_REG_ZQCS, 0x0, ZQCS_ZQCS_MASK_SEL_CGAR);
	vIO32WriteFldAlign_All(DRAMC_REG_PRE_TDQSCK1, 0x0, PRE_TDQSCK1_TXUIPI_CAL_CGAR);

	/* Below loop sets SHU*_ODTCTRL_RODTENSTB_SELPH_CG_IG, SHU*_ODTCTRL_RODTEN_SELPH_CG_IG (wei-jen) */
	for (u1ShuIdx = DRAM_DFS_SHUFFLE_1; u1ShuIdx < u1ShuCnt; u1ShuIdx++)
	{
		u4targetAddr = DRAMC_REG_SHU_ODTCTRL + SHU_GRP_DRAMC_OFFSET * u1ShuIdx;
		vIO32WriteFldMulti_All(u4targetAddr, P_Fld(0x0, SHU_ODTCTRL_RODTENSTB_SELPH_CG_IG)
			| P_Fld(0x0, SHU_ODTCTRL_RODTEN_SELPH_CG_IG));
	}

}
#endif

#if ENABLE_DVFS_BYPASS_MR13_FSP
void DFSBypassMR13HwSet(DRAMC_CTX_T *p)
{
#if (fcFOR_CHIP_ID == fcTalbot)
	U8 u1ShuffleIdx, BFSP1 = 0, BFSP0 = 0, BFSP = 0;
	REG_TRANSFER_T TransferReg;

	if (u1IsLP4Family(p->dram_type))
	{
		for (u1ShuffleIdx = 0; u1ShuffleIdx < DRAM_DFS_SHUFFLE_MAX; u1ShuffleIdx++)
		{
			if (p->shuffle_frequency[u1ShuffleIdx] >= gu4TermFreq)
				BFSP1 |= 1 << u1ShuffleIdx;
			else
				BFSP0 |= 1 << u1ShuffleIdx;
		}

		TransferReg.u4Addr = DRAMC_REG_BYPASS_FSPOP;
		for (u1ShuffleIdx = 0; u1ShuffleIdx < DRAM_DFS_SHUFFLE_MAX; u1ShuffleIdx++)
		{
			switch (u1ShuffleIdx)
			{
				case 0:
					TransferReg.u4Fld = BYPASS_FSPOP_BPFSP_SHU0;
					break;
				case 1:
					TransferReg.u4Fld = BYPASS_FSPOP_BPFSP_SHU1;
					break;
				case 2:
					TransferReg.u4Fld = BYPASS_FSPOP_BPFSP_SHU2;
					break;
				default:
					break;
			}
			BFSP = (p->shuffle_frequency[u1ShuffleIdx] >= gu4TermFreq) ? BFSP1 : BFSP0;
			/* mcSHOW_DBG_MSG(("[DFSBypassMR13HwSet] BPFSP_SHU%d = 0x%x\n", u1ShuffleIdx, BFSP)); */
			vIO32WriteFldAlign_All(TransferReg.u4Addr, BFSP, TransferReg.u4Fld);
		}
		vIO32WriteFldAlign_All(DRAMC_REG_BYPASS_FSPOP, 0x1, BYPASS_FSPOP_BPFSP_OPT);
	}
#endif
}
#endif

#if ENABLE_WRITE_DBI
void EnableDRAMModeRegWriteDBIAfterCalibration(DRAMC_CTX_T *p)
{
	U8 channel_idx, rank_idx;
	U8 ch_backup, rank_backup, u1FSPIdx = 0;
	S8 u1ShuffleIdx;

	ch_backup = p->channel;
	rank_backup = p->rank;

	for (channel_idx = CHANNEL_A; channel_idx < p->support_channel_num; channel_idx++)
	{
		vSetPHY2ChannelMapping(p, channel_idx);
		for (rank_idx = RANK_0; rank_idx < RANK_MAX; rank_idx++)
		{
			vSetRank(p, rank_idx);
			for (u1FSPIdx = FSP_0; u1FSPIdx < FSP_MAX; u1FSPIdx++)
			{
				/* mcSHOW_DBG_MSG(("[EnableDRAMModeRegWriteDBIAfterCalibration] FSP_%d, MR13 = 0x%x\n", u1FSPIdx, u1MR13Value[u1FSPIdx])); */
				if (u1FSPIdx)
				{
				    DramcMRWriteFldAlign(p, 13, 1, MR13_FSP_WR, TO_MR);
				}
				else
				{
				    DramcMRWriteFldAlign(p, 13, 0, MR13_FSP_WR, TO_MR);
				}

				SetDramModeRegForWriteDBIOnOff(p, p->DBI_W_onoff[u1FSPIdx]);
			}
		}
	}

	vSetRank(p, rank_backup);
	vSetPHY2ChannelMapping(p, ch_backup);
}
#endif

#if ENABLE_READ_DBI
void EnableDRAMModeRegReadDBIAfterCalibration(DRAMC_CTX_T *p)
{
	U8 channel_idx, rank_idx;
	U8 ch_backup, rank_backup, u1FSPIdx = 0;
	S8 u1ShuffleIdx;

	ch_backup = p->channel;
	rank_backup = p->rank;

	for (channel_idx = CHANNEL_A; channel_idx < p->support_channel_num; channel_idx++)
	{
		vSetPHY2ChannelMapping(p, channel_idx);
		for (rank_idx = RANK_0; rank_idx < RANK_MAX; rank_idx++)
		{
			vSetRank(p, rank_idx);
			for (u1FSPIdx = FSP_0; u1FSPIdx < FSP_MAX; u1FSPIdx++)
			{
				/* mcSHOW_DBG_MSG(("[EnableDRAMModeRegReadDBIAfterCalibration] FSP_%d, MR13 = 0x%x\n", u1FSPIdx, u1MR13Value[u1FSPIdx])); */
				if (u1FSPIdx)
		                {
		                    DramcMRWriteFldAlign(p, 13, 1, MR13_FSP_WR, TO_MR);
		                }
		                else
		                {
		                    DramcMRWriteFldAlign(p, 13, 0, MR13_FSP_WR, TO_MR);
		                }

				SetDramModeRegForReadDBIOnOff(p, p->DBI_R_onoff[u1FSPIdx]);
			}
		}
	}

	vSetRank(p, rank_backup);
	vSetPHY2ChannelMapping(p, ch_backup);
}
#endif

void Hynix_Test_Mode(DRAMC_CTX_T *p)
{
#if MRW_CHECK_ONLY
	mcSHOW_MRW_MSG(("\n == [MR Dump] %s == \n", __func__));
#endif

	DramcModeRegWrite(p, 9, 0xb8);
	DramcModeRegWrite(p, 9, 0xe8);
	DramcModeRegWrite(p, 9, 0x98);
	DramcModeRegWrite(p, 9, 0xbf);
	DramcModeRegWrite(p, 9, 0xef);
	DramcModeRegWrite(p, 9, 0x9f);
	DramcModeRegWrite(p, 9, 0xb9);
	DramcModeRegWrite(p, 9, 0xe9);
	DramcModeRegWrite(p, 9, 0x99);
	DramcModeRegWrite(p, 9, 0xd8);
	DramcModeRegWrite(p, 9, 0x88);
	DramcModeRegWrite(p, 9, 0xa3);
	DramcModeRegWrite(p, 9, 0xe0);
}

#if 1
/* #ifdef DDR_INIT_TIME_PROFILING */
void DramcConfInfraReset(DRAMC_CTX_T *p)
{
#if (fcFOR_CHIP_ID == fcTalbot)
	/* 26M */
	vIO32WriteFldMulti_All(DDRPHY_CKMUX_SEL, P_Fld(0x1, CKMUX_SEL_R_PHYCTRLMUX)  /* move CKMUX_SEL_R_PHYCTRLMUX to here (it was originally between MISC_CG_CTRL0_CLK_MEM_SEL and MISC_CTRL0_R_DMRDSEL_DIV2_OPT) */
		| P_Fld(0x1, CKMUX_SEL_R_PHYCTRLDCM)); /* PHYCTRLDCM 1: follow DDRPHY_conf DCM settings, 0: follow infra DCM settings */
	vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL0, P_Fld(0, MISC_CG_CTRL0_W_CHG_MEM)
		| P_Fld(0, MISC_CG_CTRL0_CLK_MEM_SEL)); /* [5:4] mem_ck mux: 2'b00: 26MHz, [0]: change memory clock */
	vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_W_CHG_MEM); /* change clock freq */
	mcDELAY_US(1);
	vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_W_CHG_MEM); /* disable memory clock change */

	/*
	 * dramc conf reset
	 * mcSHOW_TIME_MSG(("Before infra reset, 0x10001148:%x\n", *(volatile unsigned *)(0x10001148)));
	 */
	*(volatile unsigned *)(0x10001140) = (0x1 << 15);
	/* mcSHOW_TIME_MSG(("After infra reset, 0x10001148:%x\n",  *(volatile unsigned *)(0x10001148))); */
#if (FOR_DV_SIMULATION_USED == 0)
	__asm__ __volatile__ ("dsb" : : : "memory");
#endif
	mcDELAY_US(200);
	/* mcSHOW_TIME_MSG(("Before infra clear, 0x10001148:%x\n",  *(volatile unsigned *)(0x10001148))); */
	*(volatile unsigned *)(0x10001144) = (0x1 << 15);
	/* mcSHOW_TIME_MSG(("After infra clear, 0x10001148:%x\n",  *(volatile unsigned *)(0x10001148))); */

#if 0
	mcDELAY_US(200);
	*(volatile unsigned *)(0x10007018) = 0x88000040;
	mcDELAY_US(200);
	*(volatile unsigned *)(0x10007018) = 0x88000000;
	mcDELAY_US(200);
#endif

	/* DDRPHY Reset */
	vIO32WriteFldAlign_All(DDRPHY_B0_DQ3, 0x0, B0_DQ3_RG_ARDQ_RESETB_B0);
	vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI0, 0x0, B0_DLL_ARPI0_RG_ARPI_RESETB_B0);
	vIO32WriteFldAlign_All(DDRPHY_B1_DQ3, 0x0, B1_DQ3_RG_ARDQ_RESETB_B1);
	vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI0, 0x0, B1_DLL_ARPI0_RG_ARPI_RESETB_B1);
	vIO32WriteFldAlign_All(DDRPHY_CA_CMD3, 0x0, CA_CMD3_RG_ARCMD_RESETB);
	vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI0, 0x0, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);
	vIO32WriteFldAlign(DDRPHY_PLL4, 0x0, PLL4_RG_RPHYPLL_RESETB); /* Since there is only 1 PLL, only control CHA */
	mcDELAY_US(200);
	vIO32WriteFldAlign_All(DDRPHY_B0_DQ3, 0x1, B0_DQ3_RG_ARDQ_RESETB_B0);
	vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI0, 0x1, B0_DLL_ARPI0_RG_ARPI_RESETB_B0);
	vIO32WriteFldAlign_All(DDRPHY_B1_DQ3, 0x1, B1_DQ3_RG_ARDQ_RESETB_B1);
	vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI0, 0x1, B1_DLL_ARPI0_RG_ARPI_RESETB_B1);
	vIO32WriteFldAlign_All(DDRPHY_CA_CMD3, 0x1, CA_CMD3_RG_ARCMD_RESETB);
	vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI0, 0x1, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);
	vIO32WriteFldAlign(DDRPHY_PLL4, 0x1, PLL4_RG_RPHYPLL_RESETB); /* Since there is only 1 PLL, only control CHA */

	/* Disable SPM control */
	vIO32WriteFldMulti(SPM_POWERON_CONFIG_EN, P_Fld(0xB16, POWERON_CONFIG_EN_PROJECT_CODE) | P_Fld(0, POWERON_CONFIG_EN_BCLK_CG_EN));

	/* For FMeter after dcm enable */
	vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL2, 0x0, MISC_CG_CTRL2_RG_MEM_DCM_DCM_EN);
	vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL2, 0x1, MISC_CG_CTRL2_RG_MEM_DCM_FORCE_ON);
#endif
}
#endif


void SetMr13VrcgToNormalOperation(DRAMC_CTX_T *p)
{
	DRAM_CHANNEL_T eOriChannel = vGetPHY2ChannelMapping(p);
	U8 u1ShuffleIdx = 0;
	U8 u1ChIdx = CHANNEL_A;
	U8 u1RankIdx = 0;
	U32 u4Value = 0;
#if MRW_CHECK_ONLY
	mcSHOW_MRW_MSG(("\n == [MR Dump] %s == \n", __func__));
#endif

	DramcMRWriteFldAlign(p, 13, 0, MR13_VRCG, JUST_TO_GLOBAL_VALUE);

	for (u1ChIdx = CHANNEL_A; u1ChIdx < p->support_channel_num; u1ChIdx++)
	{
		vSetPHY2ChannelMapping(p, u1ChIdx);
		/* To DRAM: MR13[3] = 0 */
		for (u1RankIdx = 0; u1RankIdx < p->support_rank_num; u1RankIdx++)
		{
			DramcModeRegWriteByRank(p, u1RankIdx, 13, u1MR13Value);
		}
		/* DVFS MRW */
		for (u1ShuffleIdx = 0; u1ShuffleIdx < DRAM_DFS_SHUFFLE_MAX; u1ShuffleIdx++)
		{
			u4Value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_VRCG) + SHU_GRP_DRAMC_OFFSET * u1ShuffleIdx,
				SHU_HWSET_VRCG_HWSET_VRCG_OP);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_VRCG) + SHU_GRP_DRAMC_OFFSET * u1ShuffleIdx, u4Value & ~(0x1 << 3),
				SHU_HWSET_VRCG_HWSET_VRCG_OP);
		}
	}
	vSetPHY2ChannelMapping(p, (U8)eOriChannel);
	return;
}

void DramcRunTimeConfig(DRAMC_CTX_T *p)
{
	vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL0, 0x0, REFCTRL0_REFDIS); /* after k, auto refresh should be enable */
	mcSHOW_DBG_MSG(("[DramcRunTimeConfig]\n"));

#ifndef FIRST_BRING_UP /* Chengchun add to disable all runtime settings for bringup */
#if (FOR_DV_SIMULATION_USED == 0 && SW_CHANGE_FOR_SIMULATION == 0)
	/* #ifdef SPM_CONTROL_AFTERK */
	TransferPLLToSPMControl(p);
	/* #endif */

	if (p->frequency >= 1600)
		vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_DPHY_RXDLY_TRACK_EN);
#endif

#if ENABLE_TX_TRACKING
	if (u1IsLP4Family(p->dram_type))
	{
		U8 backup_channel = p->channel;
		U8 channelIdx;

		for (channelIdx = CHANNEL_A; channelIdx < p->support_channel_num; channelIdx++)
		{
			vSetPHY2ChannelMapping(p, channelIdx);
			DramcHwDQSOSC(p);
		}

		vSetPHY2ChannelMapping(p, backup_channel);
		mcSHOW_DBG_MSG(("TX_TRACKING: ON\n"));
	}
	else
	{
		mcSHOW_DBG_MSG(("TX_TRACKING: OFF\n"));
	}
#else
	mcSHOW_DBG_MSG(("TX_TRACKING: OFF\n"));
#endif

#if ENABLE_RX_TRACKING_LP4
	if (u1IsLP4Family(p->dram_type))
	{
		DramcRxInputDelayTrackingInit_Common(p);
		DramcRxInputDelayTrackingHW(p);
		mcSHOW_DBG_MSG(("RX_TRACKING: ON\n"));
	}
	else
	{
		mcSHOW_DBG_MSG(("RX_TRACKING: OFF\n"));
	}
#else
	mcSHOW_DBG_MSG(("RX_TRACKING: OFF\n"));
#endif

#if (ENABLE_RX_TRACKING_LP4 && RX_DLY_TRACK_ONLY_FOR_DEBUG && defined(DUMMY_READ_FOR_TRACKING))
	mcSHOW_DBG_MSG(("RX_DLY_TRACK_DBG: ON\n"));
	DramcRxDlyTrackDebug(p);
#endif

#ifdef HW_GATING
	DramcHWGatingInit(p);     /* HW gating initial before RunTime config. */
	DramcHWGatingOnOff(p, 1); /* Enable HW gating tracking */
#else
	mcSHOW_DBG_MSG(("HW_GATING: OFF\n"));
#endif /* HW_GATING */

	/* HW gating - Disabled by default(in preloader) to save power (DE: HJ Huang) */
#if (defined(HW_GATING) && GATING_ONLY_FOR_DEBUG)
	mcSHOW_DBG_MSG(("HW_GATING DBG: ON\n"));
	DramcHWGatingDebugOnOff(p, ENABLE);
#else
	mcSHOW_DBG_MSG(("HW_GATING DBG: OFF\n"));
	DramcHWGatingDebugOnOff(p, DISABLE);
#endif

#ifdef DUMMY_READ_FOR_TRACKING
	if (u1IsLP4Family(p->dram_type))
	{
		DramcDummyReadForTrackingEnable(p);
	}
	else
	{
		mcSHOW_DBG_MSG(("DUMMY_READ_FOR_TRACKING: OFF\n"));
	}
#else
	mcSHOW_DBG_MSG(("DUMMY_READ_FOR_TRACKING: OFF\n"));
#endif

	if (u1IsLP4Family(p->dram_type))
	{
	#ifdef ZQCS_ENABLE_LP4
		/*
		 * ZQCSMASK setting: (Ch A, Ch B) = (1,0) or (0,1)
		 * if CHA.ZQCSMASK=1, and then set CHA.ZQCALDISB=1 first, else set CHB.ZQCALDISB=1 first
		 */
		vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), 1, SPCMDCTRL_ZQCALDISB); /* LP3 and LP4 are different, be careful. */
		mcSHOW_DBG_MSG(("ZQCS_ENABLE_LP4: ON\n"));
	#else
		vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), 0, SPCMDCTRL_ZQCALDISB); /* LP3 and LP4 are different, be careful. */
		mcSHOW_DBG_MSG(("ZQCS_ENABLE_LP4: OFF\n"));
	#endif
	}
	else
	{
	#ifdef ZQCS_ENABLE_LP3
		vIO32WriteFldAlign_All(DRAMC_REG_SPCMDCTRL, 1, SPCMDCTRL_ZQCSDISB);
		mcSHOW_DBG_MSG(("ZQCS_ENABLE_LP3: ON\n"));
	#else
		vIO32WriteFldAlign_All(DRAMC_REG_SPCMDCTRL, 0, SPCMDCTRL_ZQCSDISB);
		mcSHOW_DBG_MSG(("ZQCS_ENABLE_LP3: OFF\n"));
	#endif
	}
#if APPLY_LOWPOWER_GOLDEN_SETTINGS
#if (SW_CHANGE_FOR_SIMULATION == 0)
	int enable_dcm =  (doe_get_config("dramc_dcm"))?0:1;
	char *str = (enable_dcm == 1)? ("ON"):("OFF");
	EnableDramcPhyDCM(p, enable_dcm);
	mcSHOW_DBG_MSG(("LOWPOWER_GOLDEN_SETTINGS(DCM): %s\n",str));
#endif
#else
#if (SW_CHANGE_FOR_SIMULATION == 0)
	EnableDramcPhyDCM(p, 0);
	mcSHOW_DBG_MSG(("LOWPOWER_GOLDEN_SETTINGS(DCM): OFF\n"));
#endif
#endif

#ifdef DUMMY_READ_FOR_DQS_GATING_RETRY
	if (u1IsLP4Family(p->dram_type))
	{
		DummyReadForDqsGatingRetry(p, 1);
		mcSHOW_DBG_MSG(("DUMMY_READ_FOR_DQS_GATING_RETRY: ON\n"));
	}
	else
	{
		DummyReadForDqsGatingRetry(p, 0);
		mcSHOW_DBG_MSG(("DUMMY_READ_FOR_DQS_GATING_RETRY: OFF\n"));
	}
#else
	DummyReadForDqsGatingRetry(p, 0);
	mcSHOW_DBG_MSG(("DUMMY_READ_FOR_DQS_GATING_RETRY: OFF\n"));
#endif

#ifdef SPM_CONTROL_AFTERK
	TransferToSPMControl(p);  /* don't enable in ETT */
	mcSHOW_DBG_MSG(("SPM_CONTROL_AFTERK: ON\n"));
#else
	mcSHOW_DBG_MSG(("SPM_CONTROL_AFTERK: OFF\n"));
#endif

#ifdef IMPEDANCE_TRACKING_ENABLE
	if (p->dram_type == TYPE_LPDDR4 || p->dram_type == TYPE_LPDDR4X)
	{
		DramcImpedanceTrackingEnable(p);
		mcSHOW_DBG_MSG(("IMPEDANCE_TRACKING: ON\n"));

	#ifdef IMPEDANCE_HW_SAVING
		DramcImpedanceHWSaving(p);
	#endif
	}
#else
	mcSHOW_DBG_MSG(("IMPEDANCE_TRACKING: OFF\n"));
#endif

	/*
	 * 0x1c0[31]
	 * vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQSCAL0), 0, DQSCAL0_STBCALEN);
	 */

#ifdef TEMP_SENSOR_ENABLE
	if (u1IsLP4Family(p->dram_type))
	{
		/* Enable HW-MRR4 : SPCMDCTRL_REFRDIS=0 */
		vIO32WriteFldAlign_All(DRAMC_REG_SPCMDCTRL, 0, SPCMDCTRL_REFRDIS);

		/* HW-MRR4 don't block normal EMI request : SPCMDCTRL_REFR_BLOCKEN=0 */
		vIO32WriteFldAlign_All(DRAMC_REG_SPCMDCTRL, 0, SPCMDCTRL_REFR_BLOCKEN);

		/* DRAMC will consider tMRR ac-timing : HW_MRR_FUN_TMRR_ENA =1 */
		vIO32WriteFldAlign_All(DRAMC_REG_HW_MRR_FUN, 1, HW_MRR_FUN_TMRR_ENA);

		/* Set HW-MRR4 command in queue to high priority : R_DMMRR_HW_HIPRI = 1 */
		vIO32WriteFldAlign_All(DRAMC_REG_HW_MRR_FUN, 1, HW_MRR_FUN_MRR_HW_HIPRI);
	}
#if ENABLE_LP3_SW
	else
	{
		/* lp3 only has CHA */
		vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL, 0, SPCMDCTRL_REFRDIS);
	}
#endif /* ENABLE_LP3_SW */

	mcSHOW_DBG_MSG(("TEMP_SENSOR: ON\n"));
#else
	vIO32WriteFldAlign_All(DRAMC_REG_SPCMDCTRL, 1, SPCMDCTRL_REFRDIS);
	mcSHOW_DBG_MSG(("TEMP_SENSOR: OFF\n"));
#endif

#if ENABLE_PER_BANK_REFRESH
	vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL0, 1, REFCTRL0_PBREFEN);
	mcSHOW_DBG_MSG(("PER_BANK_REFRESH: ON\n"));
#else
	vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL0, 0, REFCTRL0_PBREFEN);
	mcSHOW_DBG_MSG(("PER_BANK_REFRESH: OFF\n"));
#endif

#if CMD_PICG_NEW_MODE
    mcSHOW_DBG_MSG(("CMD_PICG_NEW_MODE: ON\n"));
#else
    mcSHOW_DBG_MSG(("CMD_PICG_NEW_MODE: OFF\n"));
#endif

#if ENABLE_LP3_SW
	/* Lewis@20160506:[DDR reserve mode] LPDDR3 Let CHB enter SR, let SR ACK always 1 to avoid WDT conf could not enter SR */
	if (p->dram_type == TYPE_LPDDR3)
	{
		DRAM_CHANNEL_T backChannel = p->channel;
		p->channel = CHANNEL_B;
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SREFCTRL), 1, SREFCTRL_SELFREF); /* Let CHB enter SR */
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RKCFG), 1, RKCFG_RK0DPD); /* Set to let CHB SREF_STATE always 1, fix(a)LP3 WDT TO to let DRAM into SR fail at second time (b) LP3 could not enter SR in S0 */
		p->channel = backChannel;
	}
#endif /* ENABLE_LP3_SW */

#ifdef HW_SAVE_FOR_SR
	HwSaveForSR(p);
	mcSHOW_DBG_MSG(("HW_SAVE_FOR_SR: ON\n"));
#else
	mcSHOW_DBG_MSG(("HW_SAVE_FOR_SR: OFF\n"));
#endif

#ifdef CLK_FREE_FUN_FOR_DRAMC_PSEL
	ClkFreeRunForDramcPsel(p);
	mcSHOW_DBG_MSG(("CLK_FREE_FUN_FOR_DRAMC_PSEL: ON\n"));
#else
	mcSHOW_DBG_MSG(("CLK_FREE_FUN_FOR_DRAMC_PSEL: OFF\n"));
#endif

#if PA_IMPROVEMENT_FOR_DRAMC_ACTIVE_POWER
	DramcPAImprove(p);
	mcSHOW_DBG_MSG(("PA_IMPROVEMENT_FOR_DRAMC_ACTIVE_POWER: ON\n"));
#else
	mcSHOW_DBG_MSG(("PA_IMPROVEMENT_FOR_DRAMC_ACTIVE_POWER: OFF\n"));
#endif

#if ENABLE_RODT_TRACKING
	mcSHOW_DBG_MSG(("Read ODT Tracking: %s\n", u1IsLP4Family(p->dram_type) ? "ON":"OFF"));
#else
	mcSHOW_DBG_MSG(("Read ODT Tracking: OFF\n"));
#endif

#if TDQSCK_PRECALCULATION_FOR_DVFS
#if DUAL_FREQ_K
	mcSHOW_DBG_MSG(("DQS Precalculation for DVFS: "));
	/* Maoauo: Enable DQS precalculation for LP4, disable for LP3(same as Kibo) */
	if (u1IsLP4Family(p->dram_type))
	{
		DramcDQSPrecalculation_preset(p);
		mcSHOW_DBG_MSG(("ON\n"));
	}
	else
	{   /* Disable for LP3 (Maintain settings as Kibo, since Kibo doesn't have this function) */
		mcSHOW_DBG_MSG(("OFF\n"));
	}
#endif
#else
	mcSHOW_DBG_MSG(("DQS Precalculation for DVFS: OFF\n"));
#endif

	mcSHOW_DBG_MSG(("=========================\n"));

#if (ENABLE_DRS)
	if (p->support_rank_num == RANK_DUAL)
	{
		DramcDRS(p, 1);
	}
	else
	{
		DramcDRS(p, 0);
	}
#else
	DramcDRS(p, 0);
#endif

#if DUMP_GOLDEN_SETTING
	DumpGoldenSetting(p);
#endif

#if ENABLE_DVFS_BYPASS_MR13_FSP
	DFSBypassMR13HwSet(p);
#endif
#endif /* end of FIRST_BRING_UP */
}


void DramcSetRankEngine2(DRAMC_CTX_T *p, U8 u1RankSel)
{
	/* LPDDR2_3_ADRDECEN_TARKMODE =0, always rank0 */
	/*
	 * ADRDECEN_TARKMODE: rank input selection
	 * 1'b1 select CTO_AGENT1_RANK, 1'b0 rank by address decode
	 */
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMCTRL), 1, DRAMCTRL_ADRDECEN_TARKMODE);

	/* DUMMY_TESTAGENTRKSEL =0, select rank according to CATRAIN_TESTAGENTRK */
	/*
	 * TESTAGENTRKSEL: Test agent access rank mode selection
	 * 2'b00: rank selection by TESTAGENTRK,           2'b01: rank selection by CTO_AGENT_1_BK_ADR[0]
	 * 2'b10: rank selection by CTO_AGENT1_COL_ADR[3], 2'b11: rank selection by CTO_AGENT1_COL_ADR[4]
	 */
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), 0, TEST2_4_TESTAGENTRKSEL);

	/* CATRAIN_TESTAGENTRK = u1RankSel */
	/*
	 * TESTAGENTRK: Specify test agent rank
	 * 2'b00 rank 0, 2'b01 rank 1, 2'b10 rank 2
	 */
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), u1RankSel, TEST2_4_TESTAGENTRK);
}

void DramcEngine2SetPat(DRAMC_CTX_T *p, U8 testaudpat, U8 log2loopcount, U8 Use_Len1_Flag)
{
	if (testaudpat == TEST_XTALK_PATTERN)   /* xtalk */
	{
		/*
		 * TEST_REQ_LEN1=1 is new feature, hope to make dq bus continously.
		 * but DV simulation will got problem of compare err
		 * so go back to use old way
		 * TEST_REQ_LEN1=0, R_DMRWOFOEN=1
		 */
		if (Use_Len1_Flag != 0)
		{
			vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), P_Fld(1, TEST2_4_TEST_REQ_LEN1));   /* test agent 2 with cmd length = 0 */
		}
		else
		{
			vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), P_Fld(0, TEST2_4_TEST_REQ_LEN1));   /* test agent 2 with cmd length = 0 */
		}
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_PERFCTL0), P_Fld(1, PERFCTL0_RWOFOEN));

		/*
		 * select XTALK pattern
		 * set addr 0x044 [7] to 0
		 */
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(0, TEST2_3_TESTAUDPAT) | P_Fld(log2loopcount, TEST2_3_TESTCNT)); /* dont use audio pattern */

		/* set addr 0x48[16] to 1, TESTXTALKPAT = 1 */
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), P_Fld(1, TEST2_4_TESTXTALKPAT) | P_Fld(0, TEST2_4_TESTAUDMODE) | P_Fld(0, TEST2_4_TESTAUDBITINV));  /* use XTALK pattern, dont use audio pattern */

		/* R_DMTESTSSOPAT=0, R_DMTESTSSOXTALKPAT=0 */
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), P_Fld(0, TEST2_4_TESTSSOPAT) | P_Fld(0, TEST2_4_TESTSSOXTALKPAT));   /* dont use sso, sso + xtalk pattern */
	}
	else if (testaudpat == TEST_AUDIO_PATTERN)   /* audio */
	{
		/* set AUDINIT=0x11 AUDINC=0x0d AUDBITINV=1 AUDMODE=1(1:read only(address fix), 0: write/read address change) */
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), \
			P_Fld(0x00000011, TEST2_4_TESTAUDINIT) | P_Fld(0x0000000d, TEST2_4_TESTAUDINC) | \
			P_Fld(0, TEST2_4_TESTXTALKPAT) | P_Fld(0, TEST2_4_TESTAUDMODE) | P_Fld(1, TEST2_4_TESTAUDBITINV));

		/* set addr 0x044 [7] to 1 ,select audio pattern */
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(1, TEST2_3_TESTAUDPAT) | P_Fld(log2loopcount, TEST2_3_TESTCNT));
	}
	else   /* ISI */
	{
		/* select ISI pattern */
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(0, TEST2_3_TESTAUDPAT) | P_Fld(log2loopcount, TEST2_3_TESTCNT));
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), 0, TEST2_4_TESTXTALKPAT);
	}
}

void DramcEngine2CheckComplete(DRAMC_CTX_T *p, U8 u1status)
{
	U32 u4loop_count = 0;

	while ((u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_TESTRPT)) & u1status) != u1status)
	{
		mcDELAY_US(CMP_CPT_POLLING_PERIOD);
		u4loop_count++;
		if ((u4loop_count > 3) && (u4loop_count <= MAX_CMP_CPT_WAIT_LOOP))
		{
			/* mcSHOW_ERR_MSG(("TESTRPT_DM_CMP_CPT: %d\n", u4loop_count)); */
		}
		else if (u4loop_count > MAX_CMP_CPT_WAIT_LOOP)
		{
			/* TINFO="fcWAVEFORM_MEASURE_A %d: time out\n", u4loop_count */
			mcSHOW_DBG_MSG(("fcWAVEFORM_MEASURE_A %d :time out, [22:20] = 0x%x\n", u4loop_count, u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TESTRPT), TESTRPT_TESTSTAT)));
			mcFPRINTF((fp_A60501, "fcWAVEFORM_MEASURE_A %d: time out\n", u4loop_count));
			break;
		}
	}
}

U32 DramcEngine2Compare(DRAMC_CTX_T *p, DRAM_TE_OP_T wr)
{
	U32 u4result = 0xffffffff;
	U32 u4loopcount;
	U8  u1status = 1; /* RK0 */

	u4loopcount = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), TEST2_3_TESTCNT);
	if (u4loopcount == 1)
		u1status = 3; /* RK0 / 1 */

	if (wr == TE_OP_WRITE_READ_CHECK)
	{
		/* read data compare ready check */
		DramcEngine2CheckComplete(p, u1status);

		/* disable write */
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(0, TEST2_3_TEST2W) | P_Fld(0, TEST2_3_TEST2R) | P_Fld(0, TEST2_3_TEST1));

		mcDELAY_US(1);

		/* enable read */
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(0, TEST2_3_TEST2W) | P_Fld(1, TEST2_3_TEST2R) | P_Fld(0, TEST2_3_TEST1));
	}

	/*
	 * 5
	 * read data compare ready check
	 */
	DramcEngine2CheckComplete(p, u1status);

	/*
	 * delay 10ns after ready check from DE suggestion (1ms here)
	 * mcDELAY_US(1);
	 */

	u4result = (u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_TESTRPT)) >> 4) & u1status; /* CMP_ERR_RK0 / 1 */

	return u4result;
}

/* ------------------------------------------------------------------------- */
/* * DramcEngine2
 * start the self test engine 2 inside dramc to test dram w/r.
 * @param p                Pointer of context created by DramcCtxCreate.
 * @param  wr              (DRAM_TE_OP_T): TE operation
 * @param  test2_1         (U32): 28bits,base address[27:0].
 * @param  test2_2         (U32): 28bits,offset address[27:0]. (unit is 16-byte, i.e: 0x100 is 0x1000).
 * @param  loopforever     (S16): 0    read\write one time ,then exit
 * >0 enable eingie2, after "loopforever" second ,write log and exit
 * -1 loop forever to read\write, every "period" seconds ,check result ,only when we find error,write log and exit
 * -2 loop forever to read\write, every "period" seconds ,write log ,only when we find error,write log and exit
 * -3 just enable loop forever ,then exit
 * @param period           (U8):  it is valid only when loopforever <0; period should greater than 0
 * @param log2loopcount    (U8): test loop number of test agent2 loop number =2^(log2loopcount) ,0 one time
 * @retval status          (U32): return the value of DM_CMP_ERR  ,0  is ok ,others mean  error
 */
/* ------------------------------------------------------------------------- */
static U32 uiReg0D0h;
DRAM_STATUS_T DramcEngine2Init(DRAMC_CTX_T *p, U32 test2_1, U32 test2_2, U8 testaudpat, U8 log2loopcount)
{
	U8 Use_Len1_Flag;

	/* error handling */
	if (!p)
	{
		mcSHOW_ERR_MSG(("context is NULL\n"));
		return DRAM_FAIL;
	}

	/*
	 * check loop number validness
	 * if ((log2loopcount > 15) || (log2loopcount < 0))		// U8 >=0 always.
	 */
	if (log2loopcount > 15)
	{
		mcSHOW_ERR_MSG(("wrong param: log2loopcount > 15\n"));
		return DRAM_FAIL;
	}

	Use_Len1_Flag = (testaudpat & 0x80) >> 7;
	testaudpat = testaudpat & 0x7f;

	DramcSetRankEngine2(p, p->rank);

	uiReg0D0h = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD));
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD), P_Fld(0, DUMMY_RD_DQSG_DMYRD_EN) | P_Fld(0, DUMMY_RD_DQSG_DMYWR_EN) | P_Fld(0, DUMMY_RD_DUMMY_RD_EN) | P_Fld(0, DUMMY_RD_SREF_DMYRD_EN) | P_Fld(0, DUMMY_RD_DMY_RD_DBG) | P_Fld(0, DUMMY_RD_DMY_WR_DBG));  /* must close dummy read when do test agent */

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TESTCHIP_DMA1), 0, TESTCHIP_DMA1_DMA_LP4MATAB_OPT);

	/* disable self test engine1 and self test engine2 */
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(0, TEST2_3_TEST2W) | P_Fld(0, TEST2_3_TEST2R) | P_Fld(0, TEST2_3_TEST1));


	/*
	 * 1.set pattern ,base address ,offset address
	 * 2.select  ISI pattern or audio pattern or xtalk pattern
	 * 3.set loop number
	 * 4.enable read or write
	 * 5.loop to check DM_CMP_CPT
	 * 6.return CMP_ERR
	 * currently only implement ucengine_status = 1, others are left for future extension
	 */

	/* 1 */
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_0), P_Fld(test2_1 >> 24, TEST2_0_TEST2_PAT0) | P_Fld(test2_2 >> 24, TEST2_0_TEST2_PAT1));

#if (FOR_DV_SIMULATION_USED == 1 || SW_CHANGE_FOR_SIMULATION == 1)
	/* DV sim memory 0~0x100 has values, can't used */
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_1), (test2_1 + 0x10000) & 0x00ffffff, TEST2_1_TEST2_BASE);
#else
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_1), test2_1 & 0x00ffffff, TEST2_1_TEST2_BASE);
#endif
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_2), test2_2 & 0x00ffffff, TEST2_2_TEST2_OFF);

	/*
	 * 2 & 3
	 * (TESTXTALKPAT, TESTAUDPAT) = 00 (ISI), 01 (AUD), 10 (XTALK), 11 (UNKNOWN)
	 */
	DramcEngine2SetPat(p, testaudpat, log2loopcount, Use_Len1_Flag);
	return DRAM_OK;
}


U32 DramcEngine2Run(DRAMC_CTX_T *p, DRAM_TE_OP_T wr, U8 testaudpat)
{
	U32 u4result = 0xffffffff;

	/* 4 */
	if (wr == TE_OP_READ_CHECK)
	{
		if ((testaudpat == 1) || (testaudpat == 2))
		{
			/* if audio pattern, enable read only (disable write after read), AUDMODE=0x48[15]=0 */
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), 0, TEST2_4_TESTAUDMODE);
		}

		/* enable read, 0x008[31:29] */
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(0, TEST2_3_TEST2W) | P_Fld(1, TEST2_3_TEST2R) | P_Fld(0, TEST2_3_TEST1));
	}
	else if (wr == TE_OP_WRITE_READ_CHECK)
	{
		/* enable write */
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(1, TEST2_3_TEST2W) | P_Fld(0, TEST2_3_TEST2R) | P_Fld(0, TEST2_3_TEST1));
	}
	DramcEngine2Compare(p, wr);

	/* delay 10ns after ready check from DE suggestion (1ms here) */
	mcDELAY_US(1);

	/*
	 * 6
	 * return CMP_ERR, 0 is ok ,others are fail,diable test2w or test2r
	 * get result
	 * or all result
	 */
	u4result = (u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CMP_ERR)));
	/* disable read */
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(0, TEST2_3_TEST2W) | P_Fld(0, TEST2_3_TEST2R) | P_Fld(0, TEST2_3_TEST1));

	return u4result;
}

void DramcEngine2End(DRAMC_CTX_T *p)
{
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), P_Fld(0, TEST2_4_TEST_REQ_LEN1));   /* test agent 2 with cmd length = 0 */

	vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD), uiReg0D0h);
}

/*
 * Full set of usage test engine 2, including of DramcEngine2Init->DramcEngine2Run->DramcEngine2End
 * if you don't care the performance, and just for convinent, you may use this API (TestEngineCompare)
 */
U32 TestEngineCompare(DRAMC_CTX_T *p)
{
	U8 jj;
	U32 u4err_value;

	if (p->test_pattern <= TEST_XTALK_PATTERN)
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


#if (FOR_DV_SIMULATION_USED == 0 && SW_CHANGE_FOR_SIMULATION == 0)
#if __ETT__
/* ------------------------------------------------------------------------- */
/* * DramcRegDump
 * Dump all registers (DDRPHY and DRAMC)
 * @param p                Pointer of context created by DramcCtxCreate.
 * @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL
 */
/* ------------------------------------------------------------------------- */
void DramcRegDumpRange(U32 u4StartAddr, U32 u4EndAddr)
{
	U32 ii;

	for (ii = u4StartAddr; ii <= u4EndAddr; ii += 4)
	{
		mcSHOW_DBG_MSG(("Reg(0x%xh) Address 0x%X = 0x%X\n", (ii & 0xfff) >> 2, ii, (*(volatile unsigned int *)(ii))));
		mcDELAY_US(20000); /* Large delay to prevent UART overflow */
	}
}

/*
 * Calculate regs dump range for each type
 * Using last register definition of each group
 * Ex: Bianco DramC NAO last register definition
 * #define DRAMC_REG_RK2_B23_STB_DBG_INFO_15 (DRAMC_NAO_BASE_ADDRESS + 0x08FC)
 * Dump range end offset would be (DRAMC_REG_RK2_B23_STB_DBG_INFO_15 - DRAMC_NAO_BASE_ADDRESS)
 */
#if (fcFOR_CHIP_ID == fcTalbot)
#define DRAMC_NAO_DUMP_RANGE (DRAMC_REG_DVFS_DBG1 - DRAMC_NAO_BASE_ADDRESS)
#define DRAMC_AO_DUMP_RANGE (DRAMC_REG_SHU4_DQSG_RETRY - DRAMC_AO_BASE_ADDRESS)
#define DDRPHY_NAO_DUMP_RANGE (DDRPHY_MISC_PHY_RGS_STBEN_CMD - DDRPHY_NAO_BASE_ADDRESS)
#define DDRPHY_AO_DUMP_RANGE (DDRPHY_RFU_0X1FCC - DDRPHY_AO_BASE_ADDRESS)
#endif
DRAM_STATUS_T DramcRegDump(DRAMC_CTX_T *p)
{    /* Must modify Dump range end address accordingly for each project */
	/* Tip, add mcSHOW_DBG_MSGDUMP and set a large delay value for DramcRegDump related message output */
	mcSHOW_DBG_MSG(("\n\n\tCHA_DRAMC_NAO_BASE\n"));
	DramcRegDumpRange(Channel_A_DRAMC_NAO_BASE_ADDRESS, Channel_A_DRAMC_NAO_BASE_ADDRESS + DRAMC_NAO_DUMP_RANGE);
	mcSHOW_DBG_MSG(("\n\tCHB_DRAMC_NAO_BASE\n"));
	DramcRegDumpRange(Channel_B_DRAMC_NAO_BASE_ADDRESS, Channel_B_DRAMC_NAO_BASE_ADDRESS + DRAMC_NAO_DUMP_RANGE);

	mcSHOW_DBG_MSG(("\n\tCHA_DRAMC_AO_BASE\n"));
	DramcRegDumpRange(Channel_A_DRAMC_AO_BASE_ADDRESS, Channel_A_DRAMC_AO_BASE_ADDRESS + DRAMC_AO_DUMP_RANGE);
	mcSHOW_DBG_MSG(("\n\tCHB_DRAMC_AO_BASE\n"));
	DramcRegDumpRange(Channel_B_DRAMC_AO_BASE_ADDRESS, Channel_B_DRAMC_AO_BASE_ADDRESS + DRAMC_AO_DUMP_RANGE);

	mcSHOW_DBG_MSG(("\n\tCHA_PHY_NAO_BASE\n"));
	DramcRegDumpRange(Channel_A_PHY_NAO_BASE_ADDRESS, Channel_A_PHY_NAO_BASE_ADDRESS + DDRPHY_NAO_DUMP_RANGE);
	mcSHOW_DBG_MSG(("\n\tCHB_PHY_NAO_BASE\n"));
	DramcRegDumpRange(Channel_B_PHY_NAO_BASE_ADDRESS, Channel_B_PHY_NAO_BASE_ADDRESS + DDRPHY_NAO_DUMP_RANGE);

	mcSHOW_DBG_MSG(("\n\tCHA_PHY_AO_BASE\n"));
	DramcRegDumpRange(Channel_A_PHY_AO_BASE_ADDRESS, Channel_A_PHY_AO_BASE_ADDRESS + DDRPHY_AO_DUMP_RANGE);
	mcSHOW_DBG_MSG(("\n\tCHB_PHY_AO_BASE\n"));
	DramcRegDumpRange(Channel_B_PHY_AO_BASE_ADDRESS, Channel_B_PHY_AO_BASE_ADDRESS + DDRPHY_AO_DUMP_RANGE);

	return DRAM_OK;
}


#if ETT_NO_DRAM
void NoDramDramcRegDumpRange(U32 u4StartAddr, U32 u4EndAddr)
{
	U32 ii;

	for (ii = u4StartAddr; ii < u4EndAddr; ii += 4)
	{
		mcSHOW_DBG_MSG(("*(volatile unsigned int *)0x%X = 0x%X; \n", ii, (*(volatile unsigned int *)(ii))));
		/* mcSHOW_DBG_MSG(("mcSHOW_DBG_MSG((\"0x%X\"));\n", ii)); */
	}
}

DRAM_STATUS_T NoDramDramcRegDump(DRAMC_CTX_T *p)
{
	mcSHOW_DBG_MSG(("\n\n\tCHA_DRAMC_AO_BASE\n"));
	NoDramDramcRegDumpRange(Channel_A_DRAMC_AO_BASE_ADDRESS, Channel_A_DRAMC_AO_BASE_ADDRESS + 0x1E58);
	mcSHOW_DBG_MSG(("\n\tCHB_DRAMC_AO_BASE\n"));
	NoDramDramcRegDumpRange(Channel_B_DRAMC_AO_BASE_ADDRESS, Channel_B_DRAMC_AO_BASE_ADDRESS + 0x1E58);
	mcSHOW_DBG_MSG(("\n\tPHY_A_BASE\n"));
	NoDramDramcRegDumpRange(Channel_A_PHY_BASE_ADDRESS, Channel_A_PHY_BASE_ADDRESS + 0x1FC8);
	mcSHOW_DBG_MSG(("\n\tPHY_B_BASE\n"));
	NoDramDramcRegDumpRange(Channel_B_PHY_BASE_ADDRESS, Channel_B_PHY_BASE_ADDRESS + 0x1FC8);

	return DRAM_OK;
}

DRAM_STATUS_T NoDramRegFill(void)
{
}
#endif
#endif
#endif

#if DRAMC_MODEREG_CHECK
void DramcModeReg_Check(DRAMC_CTX_T *p)
{
	U8 backup_channel, backup_rank;
	U8 u1ChannelIdx, u1RankIdx;
	U8 u1MRFsp, operating_fsp = p->dram_fsp;
	U8 ii, u1MR[] = {5, 12, 14, 4, 18, 19}; /* MR5, MR12, MR14, MR18, MR19 */
	U16 u2MRValue = 0, u2Value = 0;
	U8 u1match = 0;
	U8 backup_u1MR13Value = 0;

	mcSHOW_DBG_MSG(("\n\n[DramcModeReg_Check] Freq_%d, FSP_%d\n", p->frequency, p->dram_fsp));

	backup_channel = vGetPHY2ChannelMapping(p);
	backup_rank = u1GetRank(p);

	if (u1IsLP4Family(p->dram_type))
	{
		backup_u1MR13Value = u1MR13Value;

	#if MRW_CHECK_ONLY
		mcSHOW_MRW_MSG(("\n == [MR Dump] %s == \n", __func__));
	#endif

		for (u1MRFsp = FSP_0; u1MRFsp < FSP_MAX; u1MRFsp++)
		{
			for (u1ChannelIdx = 0; u1ChannelIdx < (p->support_channel_num); u1ChannelIdx++)
			{
				vSetPHY2ChannelMapping(p, u1ChannelIdx);

				for (u1RankIdx = 0; u1RankIdx < (U32)(p->support_rank_num); u1RankIdx++)
				{
					mcSHOW_DBG_MSG(("FSP_%d, CH_%d, RK%d\n", u1MRFsp, u1ChannelIdx, u1RankIdx));

					/* Fixed Biwin and LongSys LP4 DRAM MRR incorrect between FSP0 and FSP1 */
					if ((p->vendor_id == VENDOR_MICRON) && (p->dram_type == TYPE_LPDDR4))
					{
						if (u1MRFsp == FSP_1)
							CBT_Switch_Freq(p, CBT_HIGH_FREQ);
						else
							CBT_Switch_Freq(p, CBT_LOW_FREQ);
					}
					else /* for Hynix and Samsung */
					{
						if (u1MRFsp == FSP_1)
							u1MR13Value |= 0x40;   /* Read / Write FSP */
						else
							u1MR13Value &= (~0x40);   /* Read / Write FSP */

						DramcModeRegWriteByRank(p, u1RankIdx, 13, u1MR13Value);
					}

					for (ii = 0; ii < sizeof (u1MR); ii++)
					{
						DramcModeRegReadByRank(p, u1RankIdx, u1MR[ii], & u2Value);
						u2Value &= 0xFF;

						if ((u1MR[ii] == 12) || (u1MR[ii] == 14)) /* need to compare final setting with global variants */
						{
							if (u1MR[ii] == 12)
							{
								u2MRValue = u1MR12Value[u1ChannelIdx][u1RankIdx][u1MRFsp];
							}
							else if (u1MR[ii] == 14)
							{
								u2MRValue = u1MR14Value[u1ChannelIdx][u1RankIdx][u1MRFsp];
							}
							u1match = (u2Value == u2MRValue) ? 1:0;

						#ifdef FOR_HQA_REPORT_USED
							if (gHQALog_flag == 1)
							{
								HQA_Log_Message_for_Report(p, u1ChannelIdx, u1RankIdx, HQA_REPORT_FORMAT1, u1MRFsp == 0 ? "DramcModeReg_Read_FSP_0_MR":"DramcModeReg_Read_FSP_1_MR", u1MR[ii], u2Value, NULL);
							}
							else
						#endif
							{
								mcSHOW_DBG_MSG(("\t\tMR%d = 0x%x (global = 0x%x)\t%smatch\n", u1MR[ii], u2Value, u2MRValue, (u1match == 1) ? "":"mis"));
							}
						}
						else
						{
						#ifdef FOR_HQA_REPORT_USED
							if (gHQALog_flag == 1)
							{
								HQA_Log_Message_for_Report(p, u1ChannelIdx, u1RankIdx, HQA_REPORT_FORMAT1, u1MRFsp == 0 ? "DramcModeReg_Read_FSP_0_MR":"DramcModeReg_Read_FSP_1_MR", u1MR[ii], u2Value, NULL);
							}
							else
						#endif
							{
								char *str_vender = "";
								if (u1MR[ii] == 5)
								{
									/* Vendor ID 1: Samsung, 6: Hynix */
									str_vender = (u2Value == 1) ? "Samsung":(u2Value == 6) ? "Hynix":"mismatch";
								}
								mcSHOW_DBG_MSG(("\t\tMR%d = 0x%x %s\n", u1MR[ii], u2Value, str_vender));
							}
						}

					}
				}
			}
		}

		/* resotre MR13 settings */
		u1MR13Value = backup_u1MR13Value;
		for (u1ChannelIdx = 0; u1ChannelIdx < (p->support_channel_num); u1ChannelIdx++)
		{
			vSetPHY2ChannelMapping(p, u1ChannelIdx);
			for (u1RankIdx = 0; u1RankIdx < (U32)(p->support_rank_num); u1RankIdx++)
				DramcModeRegWriteByRank(p, u1RankIdx, 13, backup_u1MR13Value);
		}
	}

	vSetPHY2ChannelMapping(p, backup_channel);
	vSetRank(p, backup_rank);
	return;
}
#endif

void DramcTest_DualSch_stress(DRAMC_CTX_T *p)
{
	U32 count = 0;
	U16 u2Value = 0;

#if MRW_CHECK_ONLY
	mcSHOW_MRW_MSG(("\n == [MR Dump] %s == \n", __func__));
#endif

	vIO32WriteFldAlign_All(DRAMC_REG_PERFCTL0, 1, PERFCTL0_DUALSCHEN);
	while (count < 10)
	{
		count++;

		u1MR12Value[p->channel][p->rank][p->dram_fsp] = 0x14;
		DramcModeRegWrite(p, 12, u1MR12Value[p->channel][p->rank][p->dram_fsp]);
		DramcModeRegRead(p, 12, & u2Value);
		/* mcSHOW_DBG_MSG(("MR12 = 0x%0X\n", u1Value)); */
	}
}

/*=============== cc porting to support CA term workaround ==============*/
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
	/* vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), p->rank, MRS_MRRRK); */
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1MRIdx, MRS_MRSMA);

	/* MRR command will be fired when MRREN 0->1 */
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1, SPCMD_MRREN);

	/* wait MRR command fired. */
	while (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_MRR_RESPONSE) == 0)
	{
		mcDELAY_US(1);
	}

	/*
	 * Since LP3 does not support CG condition, LP3 can not use MRR_STATUS_MRR_SW_REG to do sw mrr.
	 * After fix HW CG condition, LP3 will use MRR_STATUS_MRR_SW_REG to do sw mrr.
	 */
	U32 u4MRRReg;
	if (u1IsLP4Family(p->dram_type))
		u4MRRReg = MRR_STATUS_MRR_SW_REG;
	else
		u4MRRReg = MRR_STATUS_MRR_REG;

	/* Read out mode register value */
	u4MRValue = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRR_STATUS), u4MRRReg);
	*u2pValue = (U16)u4MRValue;

	/* Set MRREN =0 for next time MRR. */
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_MRREN);

#ifdef DUMP_INIT_RG_LOG_TO_DE
    gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag=1;
#endif

	mcSHOW_DBG_MSG3(("Read MR%d = 0x%x\n", u1MRIdx, u4MRValue));
}


void DramcModeRegReadByRank(DRAMC_CTX_T *p, U8 u1Rank, U8 u1MRIdx, U16 *u2pValue)
{
	U16 u2Value = 0;
	U8 u1RankBak;

#ifdef DUMP_INIT_RG_LOG_TO_DE
    gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag=0;
#endif

	/*
	 * Since Can_non/6771, TMRRI design changed (2 kinds of modes depending on value of R_DMRK_SCINPUT_OPT)
	 * DE: Jouling, Berson
	 * To specify SW_MRR rank -> new mode(scinput_opt == 0): MRSRK
	 * old mode(scinput_opt == 1): MRRRK
	 * Note: MPCRK is not used by SW to control rank anymore
	 */
#if ENABLE_TMRRI_NEW_MODE
	/* Backup & set rank */
	u1RankBak = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), MRS_MRSRK); /* backup rank */
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1Rank, MRS_MRSRK); /* set rank */

	/* Mode reg read */
	DramcModeRegRead(p, u1MRIdx, & u2Value);
	*u2pValue = u2Value;
#ifdef DUMP_INIT_RG_LOG_TO_DE
    gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag=0;
#endif

	/* Restore rank */
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1RankBak, MRS_MRSRK);
#else
	/* Backup & set rank */
	u1RankBak = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), MRS_MRRRK); /* backup rank */
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1Rank, MRS_MRRRK); /* set rank */

	/* Mode reg read */
	DramcModeRegRead(p, u1MRIdx, & u2Value);
	*u2pValue = u2Value;
#ifdef DUMP_INIT_RG_LOG_TO_DE
    gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag=0;
#endif

	/* Restore rank */
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1RankBak, MRS_MRRRK);
#endif

#ifdef DUMP_INIT_RG_LOG_TO_DE
    gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag=1;
#endif
}


void DramcModeRegWrite(DRAMC_CTX_T *p, U8 u1MRIdx, U8 u1Value)
{
	U32 counter = 0;
	U8 u1Rank = 0;
	U32 u4register_024;

#ifdef DUMP_INIT_RG_LOG_TO_DE
    gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag=0;
#endif

	/* backup register of CKE fix on/off */
	u4register_024 = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL));

	u1Rank = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), MRS_MRSRK);

	/* CKE must be fix on when doing MRW */
	CKEFixOnOff(p, u1Rank, CKE_FIXON, CKE_WRITE_TO_ONE_CHANNEL);

	/* vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), p->rank, MRS_MRSRK); */
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1MRIdx, MRS_MRSMA);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1Value, MRS_MRSOP);

	/* MRW command will be fired when MRWEN 0->1 */
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1, SPCMD_MRWEN);

	/* wait MRW command fired. */
	while (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_MRW_RESPONSE) == 0)
	{
		counter++;
		mcSHOW_DBG_MSG2(("wait MRW command Rank%d MR%d = 0x%x fired (%d)\n", u1Rank, u1MRIdx, u1Value, counter));
		mcDELAY_US(1);
	}

	/* Set MRWEN =0 for next time MRW. */
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0, SPCMD_MRWEN);

	vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), u4register_024); /* restore CKEFIXON value */

	u1Rank = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), MRS_MRSRK);

#if MRW_CHECK_ONLY
	u1PrintModeRegWrite = 1;
	if (u1IsLP4Family(p->dram_type) && u1MRIdx == 13)
	{
		u2MRRecord[p->channel][u1Rank][FSP_0][13] = u1Value;
		u2MRRecord[p->channel][u1Rank][FSP_1][13] = u1Value;
	}
	else
		u2MRRecord[p->channel][u1Rank][p->dram_fsp][u1MRIdx] = u1Value;
#endif

	if (u1PrintModeRegWrite)
	{
	#if VENDER_JV_LOG
		mcSHOW_JV_LOG_MSG(("Write Rank%d MR%d = 0x%x\n", u1Rank, u1MRIdx, u1Value));
	#endif
	#if MRW_CHECK_ONLY
		mcSHOW_MRW_MSG(("MRW CH%d Rank%d FSP%d MR%d = 0x%x\n", p->channel, u1Rank, p->dram_fsp, u1MRIdx, u1Value));
	#endif
		mcSHOW_DBG_MSG2(("Write Rank%d MR%d = 0x%x\n", u1Rank, u1MRIdx, u1Value));
		mcFPRINTF((fp_A60501, "Write Rank%d MR%d = 0x%x\n", u1Rank, u1MRIdx, u1Value));
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

	/* backup rank */
	u4RabnkBackup = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), MRS_MRSRK);

	/* set rank */
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1Rank, MRS_MRSRK);

	/* write mode register */
	DramcModeRegWrite(p, u1MRIdx, u1Value);
#ifdef DUMP_INIT_RG_LOG_TO_DE
    gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag=0;
#endif

	/* restore rank */
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u4RabnkBackup, MRS_MRSRK);

#ifdef DUMP_INIT_RG_LOG_TO_DE
    gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag=1;
#endif
}

#define SHUFFLE_GROUP   4               /* SHU1~4 */

#if DUAL_FREQ_K
#define SHUFFLE_ADDR_NUM_DRAMC          4
#define SHUFFLE_ADDR_NUM_DDRPHY         14

typedef struct _SHUFFLE_REG_ADDR
{
	U32 u4StartAddr;
	U32 u4EndAddr;
} SHUFFLE_REG_ADDR;

const SHUFFLE_REG_ADDR ShuffleRegTableDramc[SHUFFLE_ADDR_NUM_DRAMC] =
{
	{DRAMC_AO_BASE_ADDRESS + (0x0800), DRAMC_AO_BASE_ADDRESS + (0x08E4) }, /* Lewis add for CMD new PICG */
	{DRAMC_AO_BASE_ADDRESS + (0x0A00), DRAMC_AO_BASE_ADDRESS + (0x0A50) },
	{DRAMC_AO_BASE_ADDRESS + (0x0B00), DRAMC_AO_BASE_ADDRESS + (0x0B50) },
	{DRAMC_AO_BASE_ADDRESS + (0x0C00), DRAMC_AO_BASE_ADDRESS + (0x0C54) },
};
const SHUFFLE_REG_ADDR ShuffleRegTableDDRPhy[SHUFFLE_ADDR_NUM_DDRPHY] =
{
	{DDRPHY_AO_BASE_ADDRESS + (0x0C00), DDRPHY_AO_BASE_ADDRESS + (0x0C38) },
	{DDRPHY_AO_BASE_ADDRESS + (0x0C80), DDRPHY_AO_BASE_ADDRESS + (0x0CB8) },
	{DDRPHY_AO_BASE_ADDRESS + (0x0D00), DDRPHY_AO_BASE_ADDRESS + (0x0D38) },
	{DDRPHY_AO_BASE_ADDRESS + (0x0D80), DDRPHY_AO_BASE_ADDRESS + (0x0DBC) }, /* PLL */
	{DDRPHY_AO_BASE_ADDRESS + (0x0DD0), DDRPHY_AO_BASE_ADDRESS + (0x0DF0) }, /* PLL */
	{DDRPHY_AO_BASE_ADDRESS + (0x0E00), DDRPHY_AO_BASE_ADDRESS + (0x0E2C) }, /* R0 */
	{DDRPHY_AO_BASE_ADDRESS + (0x0E50), DDRPHY_AO_BASE_ADDRESS + (0x0E7C) },
	{DDRPHY_AO_BASE_ADDRESS + (0x0EA0), DDRPHY_AO_BASE_ADDRESS + (0x0ECC) },
	{DDRPHY_AO_BASE_ADDRESS + (0x0F00), DDRPHY_AO_BASE_ADDRESS + (0x0F2C) }, /* R1 */
	{DDRPHY_AO_BASE_ADDRESS + (0x0F50), DDRPHY_AO_BASE_ADDRESS + (0x0F7C) },
	{DDRPHY_AO_BASE_ADDRESS + (0x0FA0), DDRPHY_AO_BASE_ADDRESS + (0x0FCC) },
	{DDRPHY_AO_BASE_ADDRESS + (0x1000), DDRPHY_AO_BASE_ADDRESS + (0x102C) }, /* R2 */
	{DDRPHY_AO_BASE_ADDRESS + (0x1050), DDRPHY_AO_BASE_ADDRESS + (0x107C) },
	{DDRPHY_AO_BASE_ADDRESS + (0x10A0), DDRPHY_AO_BASE_ADDRESS + (0x10CC) },
};

#if REG_SHUFFLE_REG_CHECK
void ShuffleRegCheckProgram(U32 u4Addr)
{
#ifndef OLYMPUS_TO_BE_PORTING

	U32 Offset, TmpAddr;

	if ((u4Addr >= DRAMC_AO_BASE_ADDRESS) && (u4Addr < DDRPHY_BASE_ADDR))
	{
		TmpAddr = (DRAMC_AO_BASE_ADDRESS | (u4Addr & 0xffff));
	}
	else
	{
		TmpAddr = (DDRPHY_BASE_ADDR | (u4Addr & 0xffff));
	}

	for (Offset = 0; Offset < FREQREG_SIZE; Offset++)
	{
		if (TmpAddr == LowFreq_ShuffleReg[Offset].uiSourceAddr)
		{
			mcSHOW_DBG_MSG(("\n[ShuffleRegCheck] OK 0x%x \n", u4Addr));
			mcFPRINTF((fp_A60501, "\n[ShuffleRegCheck] OK 0x%x\n", u4Addr));
			break;
		}
	}

	if (Offset == FREQREG_SIZE)
	{
		mcSHOW_DBG_MSG(("\n[ShuffleRegCheck] Not in shuffle 0x%x \n", u4Addr));
		mcFPRINTF((fp_A60501, "\n[ShuffleRegCheck] Not in shuffle 0x%x\n", u4Addr));
	}
#endif
}
#endif


#if 1// __ETT__, define __ETT__, SLT compile error
void DramcDumpFreqSetting(DRAMC_CTX_T *p)
{
	U32 Offset;
	U32 u4RegValue;
	U32 i, j;
	U32 tbIdx;

	mcSHOW_DBG_MSG(("Dump shuffle regs\n"));
	for (i = 0; i < CHANNEL_NUM; i++)
	{
		mcSHOW_DBG_MSG(("DRAMC CH_%d\n", i));
		for (tbIdx = 0; tbIdx < SHUFFLE_ADDR_NUM_DRAMC; tbIdx++)
		{
			U32 normalAddr = ShuffleRegTableDramc[tbIdx].u4StartAddr + ((U32)i << POS_BANK_NUM);
			while (normalAddr <= ShuffleRegTableDramc[tbIdx].u4EndAddr + ((U32)i << POS_BANK_NUM))
			{
				for (j = 0; j < SHUFFLE_GROUP; j++)
				{
					U32 shuffleAddr = normalAddr + SHU_GRP_DRAMC_OFFSET * j;
					U32 regValue = u4IO32Read4B(shuffleAddr);

				#ifdef ETT_PRINT_FORMAT
					mcSHOW_DBG_MSG(("[SHU%d]Addr 0x%X = 0x%X ", j, shuffleAddr, regValue));
				#else
					mcSHOW_DBG_MSG(("[SHU%2d] Addr 0x%8xh = 0x%8xh  ", j, shuffleAddr, regValue));
				#endif
				}
				mcSHOW_DBG_MSG(("\n"));
				normalAddr += 4;
			}
		}
	}

	/* PHY */
	for (i = 0; i < CHANNEL_NUM; i++)
	{
		mcSHOW_DBG_MSG(("DDRPHY CH_%d\n", i));
		for (tbIdx = 0; tbIdx < SHUFFLE_ADDR_NUM_DDRPHY; tbIdx++)
		{
			U32 normalAddr = ShuffleRegTableDDRPhy[tbIdx].u4StartAddr + ((U32)i << POS_BANK_NUM);
			while (normalAddr <= ShuffleRegTableDDRPhy[tbIdx].u4EndAddr + ((U32)i << POS_BANK_NUM))
			{
				for (j = 0; j < SHUFFLE_GROUP; j++)
				{
					U32 shuffleAddr = normalAddr + SHU_GRP_DDRPHY_OFFSET * j;
					U32 regValue = u4IO32Read4B(shuffleAddr);

				#ifdef ETT_PRINT_FORMAT
					mcSHOW_DBG_MSG(("[SHU%d]Addr 0x%X = 0x%X ", j, shuffleAddr, regValue));
				#else
					mcSHOW_DBG_MSG(("[SHU%2d]Addr 0x%8xh = 0x%8xh  ", j, shuffleAddr, regValue));
				#endif
				}
				mcSHOW_DBG_MSG(("\n"));
				normalAddr += 4;
			}
		}
	}
}

void DramcDFSTestCode(DRAMC_CTX_T *p)
{
#ifdef HW_GATING
	if (p->dram_type == TYPE_LPDDR3 || (u1IsLP4Family(p->dram_type)))
	{
		vIO32WriteFldMulti_All(DRAMC_REG_SHUCTRL2, P_Fld(0x1, SHUCTRL2_R_DVFS_OPTION)
			| P_Fld(0x1, SHUCTRL2_R_DVFS_PARK_N));
	}
	else
	{
		vIO32WriteFldMulti_All(DRAMC_REG_SHUCTRL2, P_Fld(0x0, SHUCTRL2_R_DVFS_OPTION)
			| P_Fld(0x0, SHUCTRL2_R_DVFS_PARK_N));
	}
#else
	vIO32WriteFldMulti_All(DRAMC_REG_SHUCTRL2, P_Fld(0x0, SHUCTRL2_R_DVFS_OPTION)
		| P_Fld(0x0, SHUCTRL2_R_DVFS_PARK_N));
#endif
	/*
	 * vIO32WriteFldMulti_All(DRAMC_REG_SHUCTRL2, P_Fld(0x3F, SHUCTRL2_R_DLL_IDLE));
	 * vIO32WriteFldAlign_All(DRAMC_REG_DVFSDLL, 0xFFFFFFFF, Fld(32,0,AC_FULLDW));
	 * vIO32WriteFldAlign_All(DRAMC_REG_SHU_MISC, 0x0, SHU_MISC_REQQUE_MAXCNT);
	 * vIO32WriteFldMulti_All(DRAMC_REG_SHUCTRL2,  P_Fld(0xFF, SHUCTRL2_SHU_PERIOD_GO_ZERO_CNT));
	 */
}

#endif  /* __ETT__ */

void DramcSaveToShuffleReg(DRAMC_CTX_T *p, DRAM_DFS_SHUFFLE_TYPE_T srcRG, DRAM_DFS_SHUFFLE_TYPE_T dstRG)
{
	U32 Offset;
	U32 u4RegValue;
	U32 i;
	U32 tbIdx;

	mcSHOW_DBG_MSG(("Save freq reg settings into shuffle regs\n"));

	/* DRAMC */
	for (i = 0; i < CHANNEL_NUM; i++)
	{
		for (tbIdx = 0; tbIdx < SHUFFLE_ADDR_NUM_DRAMC; tbIdx++)
		{
			U32 normalAddr = ShuffleRegTableDramc[tbIdx].u4StartAddr + ((U32)i << POS_BANK_NUM);
			while (normalAddr <= ShuffleRegTableDramc[tbIdx].u4EndAddr + ((U32)i << POS_BANK_NUM))
			{
				U32 srcAddr = normalAddr + SHU_GRP_DRAMC_OFFSET * srcRG;
				U32 shuffleAddr = normalAddr + SHU_GRP_DRAMC_OFFSET * dstRG;
				U32 regValue = u4IO32Read4B(srcAddr);
				vIO32Write4B(shuffleAddr, regValue);
				/* mcSHOW_DBG_MSG(("0x%X -> 0x%X\n", normalAddr, shuffleAddr)); */
				normalAddr += 4;
			}
		}
	}

	/* DRAMC-exception-1 */
	for (i = 0; i < CHANNEL_NUM; i++)
	{
		U32 srcAddr = DRAMC_REG_SHUCTRL2 + ((U32)i << POS_BANK_NUM);
		U32 shuffleAddr = DRAMC_REG_DVFSDLL + ((U32)i << POS_BANK_NUM);
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
		/* mcSHOW_DBG_MSG(("0x%X -> 0x%X\n", normalAddr, shuffleAddr)); */
	}

	/* DRAMC-exception-2 */
	for (i = 0; i < CHANNEL_NUM; i++)
	{
		U32 srcAddr = DRAMC_REG_DVFSDLL + ((U32)i << POS_BANK_NUM);
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

	/* PHY */
	for (i = 0; i < CHANNEL_NUM; i++)
	{
		for (tbIdx = 0; tbIdx < SHUFFLE_ADDR_NUM_DDRPHY; tbIdx++)
		{
			U32 normalAddr = ShuffleRegTableDDRPhy[tbIdx].u4StartAddr + ((U32)i << POS_BANK_NUM);
			while (normalAddr <= ShuffleRegTableDDRPhy[tbIdx].u4EndAddr + ((U32)i << POS_BANK_NUM))
			{
				U32 srcAddr = normalAddr + SHU_GRP_DDRPHY_OFFSET * srcRG;
				U32 shuffleAddr = normalAddr + SHU_GRP_DDRPHY_OFFSET * dstRG;
				U32 regValue = u4IO32Read4B(srcAddr);
				vIO32Write4B(shuffleAddr, regValue);
				/* mcSHOW_DBG_MSG(("0x%X -> 0x%X\n", normalAddr, shuffleAddr)); */
				normalAddr += 4;
			}
		}
	}

	/* PHY-exception */
#ifndef BIANCO_TO_BE_PORTING
	for (i = 0; i < CHANNEL_NUM; i++)
	{
		U32 normalAddr = DDRPHY_SSC_SHU1_PLL5 + ((U32)i << POS_BANK_NUM);
		while (normalAddr <= DDRPHY_SSC_SHU1_PLL8 + ((U32)i << POS_BANK_NUM))
		{
			U32 srcOffset = (srcRG == 0) ? 0 : 0x20;
			U32 dstOffset = (dstRG == 0) ? 0 : 0x20;
			U32 srcAddr = normalAddr + 0x10*srcRG + srcOffset;
			U32 shuffleAddr = normalAddr + 0x10*dstRG + dstOffset;
			U32 regValue = u4IO32Read4B(srcAddr);
			vIO32Write4B(shuffleAddr, regValue);
			/* mcSHOW_DBG_MSG(("0x%X -> 0x%X\n", normalAddr, shuffleAddr)); */
			normalAddr += 4;
		}
	}
#endif
}

void SPM_Pinmux_Setting(DRAMC_CTX_T * p)
{
#if (FOR_DV_SIMULATION_USED == 0 && SW_CHANGE_FOR_SIMULATION == 0)
	/* ! set SPM project code and enable clock enable */
	vIO32WriteFldMulti(SPM_POWERON_CONFIG_EN, P_Fld(0xB16, POWERON_CONFIG_EN_PROJECT_CODE) | P_Fld(1, POWERON_CONFIG_EN_BCLK_CG_EN));

	/* ! set SPM pinmux */
	vIO32WriteFldMulti(SPM_PCM_PWR_IO_EN, P_Fld(0, PCM_PWR_IO_EN_RG_PCM_PWR_IO_EN) | P_Fld(0, PCM_PWR_IO_EN_RG_RF_SYNC_EN));

	if (u1IsLP4Family(p->dram_type))
		vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON_SEL, 0x00ffffff, DRAMC_DPY_CLK_SW_CON_SEL_FULL);
	else
		vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON_SEL, 0xaaffffff, DRAMC_DPY_CLK_SW_CON_SEL_FULL);
	vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON_SEL2, 0xffffffff, DRAMC_DPY_CLK_SW_CON_SEL2_FULL);
#endif
}


#if __ETT__ || defined(SLT)
U8 gDVFSCtrlSel = 0;
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

	if (gDVFSCtrlSel == 0)/* SPM Mode */
	{
		if (!p->u1PhyPLLEn)
		{
			mcSHOW_DBG_MSG(("SPM DramcDFSDirectJump_SwitchCtrlMode(%s) current = CLRPLL\n", gDVFSCtrlSel ? "RG" : "SPM"));
			vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_PHYPLL_MODE_SW_PCM);
			vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL2_MODE_SW_PCM);
			vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, 0, DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL_MODE_SW);
			vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, 1, DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL2_MODE_SW);
		}
		else
		{
			mcSHOW_DBG_MSG(("SPM DramcDFSDirectJump_SwitchCtrlMode(%s) current = PHYPLL\n", gDVFSCtrlSel ? "RG" : "SPM"));
			vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_PHYPLL2_MODE_SW_PCM);
			vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL_MODE_SW_PCM);
			vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, 0, DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL2_MODE_SW);
			vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, 1, DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL_MODE_SW);
		}
	#if (DRAMC_DFS_MODE == 0)
		shu_level = u4IO32ReadFldAlign(DDRPHY_MISC_SPM_CTRL1, MISC_SPM_CTRL1_RG_DR_SHU_LEVEL);
		vIO32WriteFldAlign(SPM_POWER_ON_VAL0, shu_level, SPM_POWER_ON_VAL0_SC_DR_SHU_LEVEL_PCM);
	#endif
	}
	else/* RG Mode */
	{
		if (!p->u1PhyPLLEn)
		{
			mcSHOW_DBG_MSG(("RG DramcDFSDirectJump_SwitchCtrlMode(%s) current = CLRPLL\n", gDVFSCtrlSel ? "RG" : "SPM"));
			vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_PHYPLL_SHU_EN);
			vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_PHYPLL_MODE_SW);
			vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_PHYPLL2_SHU_EN);
			vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_PHYPLL2_MODE_SW);
		}
		else
		{
			mcSHOW_DBG_MSG(("RG DramcDFSDirectJump_SwitchCtrlMode(%s) current = PHYPLL\n", gDVFSCtrlSel ? "RG" : "SPM"));
			vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_PHYPLL_SHU_EN);
			vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_PHYPLL_MODE_SW);
			vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_PHYPLL2_SHU_EN);
			vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_PHYPLL2_MODE_SW);
		}
	#if (DRAMC_DFS_MODE == 0)
		shu_level = u4IO32ReadFldAlign(SPM_POWER_ON_VAL0, SPM_POWER_ON_VAL0_SC_DR_SHU_LEVEL_PCM);
		vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, shu_level, MISC_SPM_CTRL1_RG_DR_SHU_LEVEL);
	#endif
	}
	vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, gDVFSCtrlSel, MISC_SPM_CTRL1_SPM_DVFS_CONTROL_SEL);
}

void DramcDFSDirectJump_SPMMode(DRAMC_CTX_T *p, U8 shu_level)
{
	U8 u1ShuAck = 0;
	U8 i = 0;
	for (i = 0; i < p->support_channel_num; i++)
	{
		u1ShuAck |= (0x1 << i);
	}

	if (p->u1PhyPLLEn)
	{
		mcSHOW_DBG_MSG3(("DFSDirectJump to CLRPLL, SHU_LEVEL = %d, ACK = %x\n", shu_level, u1ShuAck));
	}
	else
	{
		mcSHOW_DBG_MSG3(("DFSDirectJump to PHYPLL, SHU_LEVEL = %d, ACK = %x\n", shu_level, u1ShuAck));
	}


	vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_PHYPLL_SHU_EN_PCM);
	vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_PHYPLL2_SHU_EN_PCM);

	vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DR_SHU_LEVEL_PCM);
	vIO32WriteFldAlign(SPM_POWER_ON_VAL0, shu_level, SPM_POWER_ON_VAL0_SC_DR_SHU_LEVEL_PCM);

	if (p->u1PhyPLLEn)
	{
		vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL2_SHU_EN_PCM);
		mcDELAY_US(1);
		vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL2_MODE_SW_PCM);
		mcSHOW_DBG_MSG3(("Enable CLRPLL\n"));
	}
	else
	{
		vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL_SHU_EN_PCM);
		mcDELAY_US(1);
		vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL_MODE_SW_PCM);
		mcSHOW_DBG_MSG3(("Enable PHYPLL\n"));
	}
	/* TINFO="DRAM : set tx tracking disable  = 1" */
	vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_TX_TRACKING_DIS);

	mcDELAY_US(20);

#if 0
	mcSHOW_DBG_MSG3(("Enable SHORT - QUEUE\n"));
	vIO32WriteFldAlign(SPM_POWER_ON_VAL1, 1, SPM_POWER_ON_VAL1_SC_DR_SHORT_QUEUE_PCM);
	while ((u4IO32ReadFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, DRAMC_DPY_CLK_SW_CON2_SC_DR_SHORT_QUEUE_ACK) & u1ShuAck ) != u1ShuAck)
	{
		mcSHOW_DBG_MSG3(("\twait short queue ack.\n"));
	}
#endif


	/* TINFO="DRAM : set ddrphy_fb_ck_en=1" */
	vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_DDRPHY_FB_CK_EN_PCM);


	mcSHOW_DBG_MSG3(("Disable RX - Tracking\n"));
	vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DPHY_RXDLY_TRACK_EN);

	mcSHOW_DBG_MSG3(("SHUFFLE Start\n"));
	vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_DR_SHU_EN_PCM);
	while ((u4IO32ReadFldAlign(SPM_DRAMC_DPY_CLK_SW_CON, DRAMC_DPY_CLK_SW_CON_SC_DMDRAMCSHU_ACK) & u1ShuAck ) != u1ShuAck)
	{
		mcSHOW_DBG_MSG3(("\twait shu_en ack.\n"));
	}

	/* vIO32WriteFldAlign(SPM_POWER_ON_VAL1, 0, SPM_POWER_ON_VAL1_SC_DR_SHORT_QUEUE_PCM); */
	vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DR_SHU_EN_PCM);
	mcSHOW_DBG_MSG3(("SHUFFLE End\n"));

	if (shu_level == 0)/* LP4 - 2CH */
	{
		mcSHOW_DBG_MSG3(("Enable RX - Tracking for shuffle - 0\n"));
		vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_DPHY_RXDLY_TRACK_EN);
	}

#ifdef ENABLE_TX_TRACKING
	/* TINFO="DRAM : clear tx tracking disable  = 0" */
	vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_TX_TRACKING_DIS);
#endif

	/* TINFO="DRAM : set ddrphy_fb_ck_en=0" */
	vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DDRPHY_FB_CK_EN_PCM);

	if (p->u1PhyPLLEn)
	{
		vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_PHYPLL_MODE_SW_PCM);
	}
	else
	{
		/* TINFO="DRAM : set sc_phypll_mode_sw=0" */
		vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_PHYPLL2_MODE_SW_PCM);
	}

	mcSHOW_DBG_MSG3(("Shuffle flow complete\n"));

	p->u1PhyPLLEn = !p->u1PhyPLLEn;
	return;
}

void DramcDFSDirectJump_RGMode(DRAMC_CTX_T *p, U8 shu_level)
{
	U8 u1ShuAck = 0;
	U8 i = 0;

	for (i = 0; i < p->support_channel_num; i++)
	{
		u1ShuAck |= (0x1 << i);
	}

	if (p->u1PhyPLLEn)
	{
		mcSHOW_DBG_MSG3(("DFSDirectJump to CLRPLL, SHU_LEVEL = %d, ACK = %x\n", shu_level, u1ShuAck));
	}
	else
	{
		mcSHOW_DBG_MSG3(("DFSDirectJump to PHYPLL, SHU_LEVEL = %d, ACK = %x\n", shu_level, u1ShuAck));
	}


	vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_PHYPLL_SHU_EN);
	vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_PHYPLL2_SHU_EN);

	vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_DR_SHU_LEVEL);
	vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, shu_level, MISC_SPM_CTRL1_RG_DR_SHU_LEVEL);

	if (p->u1PhyPLLEn)
	{
		vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_PHYPLL2_SHU_EN);
		mcDELAY_US(1);
		vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_PHYPLL2_MODE_SW);
		mcSHOW_DBG_MSG3(("Enable CLRPLL\n"));
	}
	else
	{
		vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_PHYPLL_SHU_EN);
		mcDELAY_US(1);
		vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_PHYPLL_MODE_SW);
		mcSHOW_DBG_MSG3(("Enable PHYPLL\n"));
	}
	mcDELAY_US(20);

#if 0
	mcSHOW_DBG_MSG3(("Enable SHORT - QUEUE\n"));
	vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_DR_SHORT_QUEUE);

	mcSHOW_DBG_MSG3(("\twait 5us for short queue ack.\n"));
	mcDELAY_US(5);
#endif

	/* TINFO="DRAM : set ddrphy_fb_ck_en=1" */
	vIO32WriteFldMulti(DDRPHY_MISC_SPM_CTRL1, P_Fld(1, MISC_SPM_CTRL1_RG_DDRPHY_DB_CK_CH1_EN) | P_Fld(1, MISC_SPM_CTRL1_RG_DDRPHY_DB_CK_CH0_EN));

	/*
	 * mcSHOW_DBG_MSG(("Disable RX-Tracking\n"));
	 * vIO32WriteFldAlign(SPM_SW_RSV_8, 0, SW_RSV_8_RX_TRACKING_EN);
	 */


	mcSHOW_DBG_MSG3(("SHUFFLE Start\n"));
	vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_DR_SHU_EN);

	mcSHOW_DBG_MSG3(("\twait 5us for shu_en ack.\n"));
	mcDELAY_US(5);

	/* vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_DR_SHORT_QUEUE); */
	vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_DR_SHU_EN);
	mcSHOW_DBG_MSG3(("SHUFFLE End\n"));

	if (shu_level == 0)/* LP4 - 2CH */
	{
		/*
		 * mcSHOW_DBG_MSG(("Enable RX-Tracking for shuffle-0\n"));
		 * vIO32WriteFldAlign(SPM_SW_RSV_8, 3, SW_RSV_8_RX_TRACKING_EN);
		 */
	}

	/* TINFO="DRAM : set ddrphy_fb_ck_en=0" */
	vIO32WriteFldMulti(DDRPHY_MISC_SPM_CTRL1, P_Fld(0, MISC_SPM_CTRL1_RG_DDRPHY_DB_CK_CH1_EN) | P_Fld(0, MISC_SPM_CTRL1_RG_DDRPHY_DB_CK_CH0_EN));

	if (p->u1PhyPLLEn)
	{
		vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_PHYPLL_MODE_SW);
	}
	else
	{
		/* TINFO="DRAM : set sc_phypll_mode_sw=0" */
		vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_PHYPLL2_MODE_SW);
	}

	mcSHOW_DBG_MSG3(("Shuffle flow complete\n"));

	p->u1PhyPLLEn = !p->u1PhyPLLEn;
	return;
}

void DramcDFSDirectJump(DRAMC_CTX_T *p, U8 shu_level)
{
#if defined(SLT) || (FT_DSIM_USED)
	/* use SPM dfs to test */
	DramcDFSDirectJump_SPMMode(p, shu_level);
	p->shuffle_index = shu_level;
#else
	if (gDVFSCtrlSel == 0)
	{
		DramcDFSDirectJump_SPMMode(p, shu_level);
	}
	else
	{
		DramcDFSDirectJump_RGMode(p, shu_level);
	}
#endif
}

#endif

void CBT_DramcDFSDirectJump(DRAMC_CTX_T *p, U8 shu_level)
{
	U8 u1ShuAck = 0;
	U8 i = 0;
	/* enable another PLL */
	if (p->u1PhyPLLEn)
	{
		mcSHOW_DBG_MSG3(("Disable CLRPLL\n"));
		/* vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, 1, DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL2_MODE_SW); */
		vIO32WriteFldAlign(DDRPHY_PLL2, 0, PLL2_RG_RCLRPLL_EN);
	}
	else
	{
		mcSHOW_DBG_MSG3(("Disable PHYPLL\n"));
		/* vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, 1, DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL_MODE_SW); */
		vIO32WriteFldAlign(DDRPHY_PLL1, 0, PLL1_RG_RPHYPLL_EN);
	}

	for (i = 0; i < p->support_channel_num; i++)
	{
		u1ShuAck |= (0x1 << i);
	}

	if (p->u1PhyPLLEn)
	{
		mcSHOW_DBG_MSG3(("DFSDirectJump to CLRPLL, SHU_LEVEL = %d, ACK = %x\n", shu_level, u1ShuAck));
	}
	else
	{
		mcSHOW_DBG_MSG3(("DFSDirectJump to PHYPLL, SHU_LEVEL = %d, ACK = %x\n", shu_level, u1ShuAck));
	}

	vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_PHYPLL_SHU_EN_PCM);
	vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_PHYPLL2_SHU_EN_PCM);

	vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DR_SHU_LEVEL_PCM);
	vIO32WriteFldAlign(SPM_POWER_ON_VAL0, shu_level, SPM_POWER_ON_VAL0_SC_DR_SHU_LEVEL_PCM);

	if (p->u1PhyPLLEn)
	{
		vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL2_SHU_EN_PCM);
		mcDELAY_US(1);
		/* vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL2_MODE_SW_PCM); */
		vIO32WriteFldAlign(DDRPHY_PLL2, 1, PLL2_RG_RCLRPLL_EN);
		mcSHOW_DBG_MSG3(("Enable CLRPLL\n"));
	}
	else
	{
		vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL_SHU_EN_PCM);
		mcDELAY_US(1);
		/* vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL_MODE_SW_PCM); */
		vIO32WriteFldAlign(DDRPHY_PLL1, 1, PLL1_RG_RPHYPLL_EN);
		mcSHOW_DBG_MSG3(("Enable PHYPLL\n"));
	}
	/* TINFO="DRAM : set tx tracking disable  = 1" */
	vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_TX_TRACKING_DIS);

	mcDELAY_US(20);

#if 0
	mcSHOW_DBG_MSG3(("Enable SHORT - QUEUE\n"));
	vIO32WriteFldAlign(SPM_POWER_ON_VAL1, 1, SPM_POWER_ON_VAL1_SC_DR_SHORT_QUEUE_PCM);
	while ((u4IO32ReadFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, DRAMC_DPY_CLK_SW_CON2_SC_DR_SHORT_QUEUE_ACK) & u1ShuAck ) != u1ShuAck)
	{
		mcSHOW_DBG_MSG3(("\twait short queue ack.\n"));
	}
#endif

	/* TINFO="DRAM : set ddrphy_fb_ck_en=1" */
	vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_DDRPHY_FB_CK_EN_PCM);


	mcSHOW_DBG_MSG3(("Disable RX - Tracking\n"));
	vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DPHY_RXDLY_TRACK_EN);


	mcSHOW_DBG_MSG3(("SHUFFLE Start\n"));
	vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_DR_SHU_EN_PCM);
	while ((u4IO32ReadFldAlign(SPM_DRAMC_DPY_CLK_SW_CON, DRAMC_DPY_CLK_SW_CON_SC_DMDRAMCSHU_ACK) & u1ShuAck ) != u1ShuAck)
	{
		mcSHOW_DBG_MSG3(("\twait shu_en ack.\n"));
	}

	/* vIO32WriteFldAlign(SPM_POWER_ON_VAL1, 0, SPM_POWER_ON_VAL1_SC_DR_SHORT_QUEUE_PCM); */
	vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DR_SHU_EN_PCM);
	mcSHOW_DBG_MSG3(("SHUFFLE End\n"));

	if (shu_level == 0)/* LP4 - 2CH */
	{
		mcSHOW_DBG_MSG3(("Enable RX - Tracking for shuffle - 0\n"));
		vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_DPHY_RXDLY_TRACK_EN);
	}

#ifdef ENABLE_TX_TRACKING
	/* TINFO="DRAM : clear tx tracking disable  = 0" */
	vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_TX_TRACKING_DIS);
#endif

	/* TINFO="DRAM : set ddrphy_fb_ck_en=0" */
	vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DDRPHY_FB_CK_EN_PCM);

	if (p->u1PhyPLLEn)
	{
		/* vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_PHYPLL_MODE_SW_PCM); */
		vIO32WriteFldAlign(DDRPHY_PLL1, 0, PLL1_RG_RPHYPLL_EN);
	}
	else
	{
		/* TINFO="DRAM : set sc_phypll_mode_sw=0" */
		/* vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_PHYPLL2_MODE_SW_PCM); */
		vIO32WriteFldAlign(DDRPHY_PLL2, 0, PLL2_RG_RCLRPLL_EN);
	}

	mcSHOW_DBG_MSG3(("Shuffle flow complete\n"));

	p->u1PhyPLLEn = !p->u1PhyPLLEn;
	return;
}

void No_Parking_On_CLRPLL(DRAMC_CTX_T *p)
{
	U8 shu_level;

	if (p->u1PhyPLLEn) return; /* already parking on PHYPLL */

	shu_level = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHUSTATUS), SHUSTATUS_SHUFFLE_LEVEL);

	CBT_DramcDFSDirectJump(p, shu_level); /* parking on PHYPLL */
}

#endif  /* DUAL_FREQ_K */


U16 u2DFSGetHighestFreq(DRAMC_CTX_T * p)
{
	if (u1IsLP4Family(p->dram_type))
	{
		return LP4_HIGHEST_FREQ;
	}
#if ENABLE_LP3_SW
	else
	{
		return 933;
	}
#endif /* ENABLE_LP3_SW */
}


/* both "ENABLE_RANK_NUMBER_AUTO_DETECTION" and "DUAL_FREQ_K" use this API */
void DFSInitForCalibration(DRAMC_CTX_T *p)
{
	U8 u1JMeterK = 0;
#ifdef DDR_INIT_TIME_PROFILING
	U32 CPU_Cycle;
	mcSHOW_TIME_MSG(("*** Data rate %d ***\n\n", p->frequency << 1));

	TimeProfileBegin();
#endif

	u1PrintModeRegWrite = 1;
	DramcInit(p);
	u1PrintModeRegWrite = 0;
	vApplyConfigBeforeCalibration(p);

#ifdef DUMP_INIT_RG_LOG_TO_DE
	while (1);
#endif

#ifdef DDR_INIT_TIME_PROFILING
	CPU_Cycle = TimeProfileEnd();
	mcSHOW_TIME_MSG(("  (1) DFSInitForCalibration() take %d ms\n\n", CPU_Cycle / 1000 - gu4DutyCalibrationTime));
	mcSHOW_TIME_MSG(("  (2) DramcNewDutyCalibration take %d ms\n\r", gu4DutyCalibrationTime));
#endif

#ifdef ENABLE_MIOCK_JMETER
#ifdef DDR_INIT_TIME_PROFILING
	TimeProfileBegin();
#endif

	if (u1IsLP4Family(p->dram_type))
	{
	#if (__ETT__ || CA_PER_BIT_DELAY_CELL)
		if (Get_MDL_Used_Flag() == NORMAL_USED)
			u1JMeterK = 1;

	#else /* preloader */

		if ((p->frequency >= gu4TermFreq) && (Get_MDL_Used_Flag() == NORMAL_USED))
			u1JMeterK = 1;
	#endif
	}
	else /* LP3 */
	{
	#if (__ETT__ || LP3_CA_PER_BIT)
		u1JMeterK = 1;
	#endif
	}

	if (u1JMeterK)
		PRE_MIOCK_JMETER_HQA_USED(p);

#ifdef DDR_INIT_TIME_PROFILING
	CPU_Cycle = TimeProfileEnd();
	mcSHOW_TIME_MSG(("  (3) JMeter takes %d ms\n\r", CPU_Cycle / 1000));
#endif
#endif

}

#if RX_DLY_TRACK_ONLY_FOR_DEBUG
void DramcRxDlyTrackDebug(DRAMC_CTX_T *p)
{
	/*
	 * indicate ROW_ADR = 2 for dummy write & read for Rx dly track debug feature, avoid pattern overwrite by MEM_TEST
	* pattern(0xAAAA5555) locates: 0x40010000, 0x40010100, 0x80010000, 0x80010100
	 */

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

	/* disable Rx dly track debug and clear status lock */
	vIO32WriteFldMulti_All((DDRPHY_MISC_RXDVS2), P_Fld(0, MISC_RXDVS2_R_DMRXDVS_DBG_MON_EN)
		| P_Fld(1, MISC_RXDVS2_R_DMRXDVS_DBG_MON_CLR)
		| P_Fld(0, MISC_RXDVS2_R_DMRXDVS_DBG_PAUSE_EN));

	/* trigger dummy write pattern 0xAAAA5555 */
	vIO32WriteFldAlign_All(DRAMC_REG_DUMMY_RD, 0x1, DUMMY_RD_DMY_WR_DBG);
	vIO32WriteFldAlign_All(DRAMC_REG_DUMMY_RD, 0x0, DUMMY_RD_DMY_WR_DBG);

	/* enable Rx dly track debug feature */
	vIO32WriteFldMulti_All((DDRPHY_MISC_RXDVS2), P_Fld(1, MISC_RXDVS2_R_DMRXDVS_DBG_MON_EN)
		| P_Fld(0, MISC_RXDVS2_R_DMRXDVS_DBG_MON_CLR)
		| P_Fld(1, MISC_RXDVS2_R_DMRXDVS_DBG_PAUSE_EN));
}

void DramcPrintRxDlyTrackDebugStatus(DRAMC_CTX_T *p)
{
	U32 backup_rank, u1ChannelBak, u4value;
	U8 u1ChannelIdx, u1ChannelMax = p->support_channel_num; /* channel A / B ... */

	u1ChannelBak = p->channel;
	backup_rank = u1GetRank(p);

	for (u1ChannelIdx = CHANNEL_A; u1ChannelIdx < p->support_channel_num; u1ChannelIdx++)
	{
		p->channel = u1ChannelIdx;

		u4value = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_DQ_RXDLY_TRRO18));
		/* mcSHOW_DBG_MSG(("\nCH_%d DQ_RXDLY_TRRO18 = 0x\033[1;36m%x\033[m\n",u1ChannelIdx,u4value)); */
		if (u4value & 1)
		{
			mcSHOW_DBG_MSG(("=== CH_%d DQ_RXDLY_TRRO18 = 0x\033[1; 36m%x\033[m, %s %s shu: %d\n", u1ChannelIdx, u4value,
				u4value & 0x2 ? "RK0: fail":"", u4value & 0x4 ? "RK1: fail":"", (u4value >> 4) & 0x3));
		}
	}
}
#endif

void DramcHWDQSGatingTracking_DVT(DRAMC_CTX_T *p)
{
#ifdef HW_GATING
	if (u1IsLP4Family(p->dram_type))
	{
	#if DramcHWDQSGatingTracking_DVT_FIFO_MODE
		/* REFUICHG=0, STB_SHIFT_DTCOUT_IG=0, DQSG_MODE=1, NARROW_IG=0 */
		vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), P_Fld(1, STBCAL_STB_DQIEN_IG) | P_Fld(1, STBCAL_PICHGBLOCK_NORD) | \
			P_Fld(0, STBCAL_REFUICHG) | P_Fld(0, STBCAL_PHYVALID_IG) | \
			P_Fld(0, STBCAL_STBSTATE_OPT) | P_Fld(0, STBCAL_STBDLELAST_FILTER) | \
			P_Fld(0, STBCAL_STBDLELAST_PULSE) | \
			P_Fld(0, STBCAL_STBDLELAST_OPT) | \
			P_Fld(1, STBCAL_PIMASK_RKCHG_OPT));

		vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL1), \
			P_Fld(1, STBCAL1_STBCAL_FILTER) | \
			P_Fld(1, STBCAL1_STB_FLAGCLR) | \
			P_Fld(1, STBCAL1_STB_SHIFT_DTCOUT_IG));

		vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL0), \
			P_Fld(1, MISC_CTRL0_R_DMDQSIEN_FIFO_EN) | \
			P_Fld(0, MISC_CTRL0_R_DMVALID_DLY) | \
			P_Fld(0, MISC_CTRL0_R_DMVALID_DLY_OPT) | \
			P_Fld(0, MISC_CTRL0_R_DMVALID_NARROW_IG) | \
			P_Fld(0, MISC_CTRL0_R_DMDQSIEN_SYNCOPT));

		vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), 0, B0_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B0);
		vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), 0, B1_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B1);
		vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_CA_CMD6), 0, CA_CMD6_RG_RX_ARCMD_DMRANK_OUTSEL);

	#else
		/* REFUICHG=0, STB_SHIFT_DTCOUT_IG=0, DQSG_MODE=1, NARROW_IG=0 */
		vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), P_Fld(1, STBCAL_STB_DQIEN_IG) | P_Fld(1, STBCAL_PICHGBLOCK_NORD) | \
			P_Fld(0, STBCAL_REFUICHG) | P_Fld(0, STBCAL_PHYVALID_IG) | \
			P_Fld(0, STBCAL_STBSTATE_OPT) | P_Fld(0, STBCAL_STBDLELAST_FILTER) | \
			P_Fld(0, STBCAL_STBDLELAST_PULSE) | \
			P_Fld(0, STBCAL_STBDLELAST_OPT) | \
			P_Fld(1, STBCAL_PIMASK_RKCHG_OPT));

		vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL1), \
			P_Fld(1, STBCAL1_STBCAL_FILTER) | \
			P_Fld(1, STBCAL1_STB_FLAGCLR) | \
			P_Fld(0, STBCAL1_STB_SHIFT_DTCOUT_IG));


		vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL0), \
			P_Fld(0, MISC_CTRL0_R_DMDQSIEN_FIFO_EN) | \
			P_Fld(3, MISC_CTRL0_R_DMVALID_DLY) | \
			P_Fld(1, MISC_CTRL0_R_DMVALID_DLY_OPT) | \
			P_Fld(0, MISC_CTRL0_R_DMVALID_NARROW_IG) | \
			P_Fld(0xf, MISC_CTRL0_R_DMDQSIEN_SYNCOPT));

		vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), 1, B0_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B0);
		vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), 1, B1_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B1);
		vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_CA_CMD6), 1, CA_CMD6_RG_RX_ARCMD_DMRANK_OUTSEL);
	#endif

	}
#if ENABLE_LP3_SW
	else
	{
		/* Set LP3 as FIFO mode to gain AC timing margin (review by HJ Huang) */
	#if DramcHWDQSGatingTracking_DVT_LP3_FIFO_MODE
		mcSHOW_DBG_MSG(("Gating Tracking in FIFO mode\n"));
		/* REFUICHG=0, STB_SHIFT_DTCOUT_IG=0, DQSG_MODE=1, NARROW_IG=0 */
		vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), P_Fld(1, STBCAL_STB_DQIEN_IG) | P_Fld(1, STBCAL_PICHGBLOCK_NORD) | \
			P_Fld(0, STBCAL_REFUICHG) | P_Fld(0, STBCAL_PHYVALID_IG) | \
			P_Fld(0, STBCAL_STBSTATE_OPT) | P_Fld(0, STBCAL_STBDLELAST_FILTER) | \
			P_Fld(0, STBCAL_STBDLELAST_PULSE) | \
			P_Fld(0, STBCAL_STBDLELAST_OPT) | \
			P_Fld(1, STBCAL_PIMASK_RKCHG_OPT));

		vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL1), \
			P_Fld(1, STBCAL1_STBCAL_FILTER) | \
			P_Fld(1, STBCAL1_STB_FLAGCLR) | \
			P_Fld(1, STBCAL1_STB_SHIFT_DTCOUT_IG));

		vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL0), \
			P_Fld(1, MISC_CTRL0_R_DMDQSIEN_FIFO_EN) | \
			P_Fld(0, MISC_CTRL0_R_DMVALID_DLY) | \
			P_Fld(0, MISC_CTRL0_R_DMVALID_DLY_OPT) | \
			P_Fld(0, MISC_CTRL0_R_DMVALID_NARROW_IG) | \
			P_Fld(0, MISC_CTRL0_R_DMDQSIEN_SYNCOPT));

		vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL1), 1, STBCAL1_STBCNT_MODESEL);

		vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), 0, B0_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B0);
		vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), 0, B1_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B1);
		vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_CA_CMD6), 0, CA_CMD6_RG_RX_ARCMD_DMRANK_OUTSEL);

	#elif DramcHWDQSGatingTracking_DVT_JADE_TRACKING_MODE
		mcSHOW_DBG_MSG(("Gating Tracking in Jadetracking mode\n"));
		/* UPDBYWR=0, REFUICHG=1, STB_SHIFT_DTCOUT_IG=0, DQSG_MODE=1, NARROW_IG=1 */
		vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0), 0, REFCTRL0_UPDBYWR);
		vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), P_Fld(0, STBCAL_STB_DQIEN_IG) | P_Fld(0, STBCAL_PICHGBLOCK_NORD) | \
			P_Fld(1, STBCAL_REFUICHG) | P_Fld(0, STBCAL_PHYVALID_IG) | \
			P_Fld(1, STBCAL_STBSTATE_OPT) | P_Fld(0, STBCAL_STBDLELAST_FILTER) | \
			P_Fld(0, STBCAL_STBDLELAST_PULSE) | \
			P_Fld(0, STBCAL_STBDLELAST_OPT) | \
			P_Fld(0, STBCAL_PIMASK_RKCHG_OPT));

		vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL1), \
			P_Fld(0, STBCAL1_STBCAL_FILTER) | \
			P_Fld(1, STBCAL1_STB_FLAGCLR) | \
			P_Fld(0, STBCAL1_STB_SHIFT_DTCOUT_IG));

		vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL0), \
			P_Fld(0, MISC_CTRL0_R_DMDQSIEN_FIFO_EN) | \
			P_Fld(2, MISC_CTRL0_R_DMVALID_DLY) | \
			P_Fld(1, MISC_CTRL0_R_DMVALID_DLY_OPT) | \
			P_Fld(1, MISC_CTRL0_R_DMVALID_NARROW_IG) | \
			P_Fld(0xf, MISC_CTRL0_R_DMDQSIEN_SYNCOPT));

		vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), 1, B0_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B0);
		vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), 1, B1_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B1);
		vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_CA_CMD6), 1, CA_CMD6_RG_RX_ARCMD_DMRANK_OUTSEL);

	#else
		/* REFUICHG=0, STB_SHIFT_DTCOUT_IG=0, DQSG_MODE=1, NARROW_IG=0 */
		vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), P_Fld(1, STBCAL_STB_DQIEN_IG) | P_Fld(1, STBCAL_PICHGBLOCK_NORD) | \
			P_Fld(0, STBCAL_REFUICHG) | P_Fld(0, STBCAL_PHYVALID_IG) | \
			P_Fld(0, STBCAL_STBSTATE_OPT) | P_Fld(0, STBCAL_STBDLELAST_FILTER) | \
			P_Fld(0, STBCAL_STBDLELAST_PULSE) | \
			P_Fld(0, STBCAL_STBDLELAST_OPT) | \
			P_Fld(1, STBCAL_PIMASK_RKCHG_OPT));

		vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL1), \
			P_Fld(1, STBCAL1_STBCAL_FILTER) | \
			P_Fld(1, STBCAL1_STB_FLAGCLR) | \
			P_Fld(0, STBCAL1_STB_SHIFT_DTCOUT_IG));

		vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL0), \
			P_Fld(0, MISC_CTRL0_R_DMDQSIEN_FIFO_EN) | \
			P_Fld(5, MISC_CTRL0_R_DMVALID_DLY) | \
			P_Fld(1, MISC_CTRL0_R_DMVALID_DLY_OPT) | \
			P_Fld(0, MISC_CTRL0_R_DMVALID_NARROW_IG) | \
			P_Fld(0xf, MISC_CTRL0_R_DMDQSIEN_SYNCOPT));

		vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), 1, B0_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B0);
		vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), 1, B1_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B1);
		vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_CA_CMD6), 1, CA_CMD6_RG_RX_ARCMD_DMRANK_OUTSEL);

	#endif

	}
#endif /* ENABLE_LP3_SW */

#endif
}


void DramcHWGatingInit(DRAMC_CTX_T *p)
{
#ifdef HW_GATING
	vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), P_Fld(0, STBCAL_STBCALEN) | \
		P_Fld(0, STBCAL_STBCAL2R) | P_Fld(0, STBCAL_STB_SELPHYCALEN) | \
		P_Fld(0, STBCAL_STBSTATE_OPT) | \
		P_Fld(0, STBCAL_RKCHGMASKDIS) | P_Fld(0, STBCAL_REFUICHG) | \
		P_Fld(1, STBCAL_PICGEN));

	vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), P_Fld(1, STBCAL_DQSIENCG_CHG_EN));
	vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), 0, STBCAL_CG_RKEN);
	vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), 1, STBCAL_DQSIENCG_NORMAL_EN);
	vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 1, MISC_CTRL1_R_DMDQSIENCG_EN);

	DramcHWDQSGatingTracking_DVT(p);
#endif
#if 0 /* Moved into DramcGatingMode() */
#ifdef GATING_7UI_FOR_TRACKING
	vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL1), 0x1, STBCAL1_DQSIEN_7UI_EN);
	vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), 0x2, B0_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B0);
	vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), 0x2, B1_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B1);

	mcSHOW_DBG_MSG(("GATING_7UI_FOR_TRACKING: ON\n"));
#else
	mcSHOW_DBG_MSG(("GATING_7UI_FOR_TRACKING: OFF\n"));
#endif
#endif
}


void DramcHWGatingOnOff(DRAMC_CTX_T *p, U8 u1OnOff)
{
#ifdef HW_GATING
	if (u1OnOff)
		u1OnOff = (U8) (doe_get_config("dram_hw_gating"))? 0: 1;
	char *str = (u1OnOff == 1)? ("ON"):("OFF");
	mcSHOW_DBG_MSG(("HW_GATING: %s\n",str));


	vIO32WriteFldMulti_All(DRAMC_REG_SHUCTRL2, P_Fld(u1OnOff, SHUCTRL2_R_DVFS_OPTION)
		| P_Fld(u1OnOff, SHUCTRL2_R_DVFS_PARK_N));
	vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL2), u1OnOff, STBCAL2_STB_GERRSTOP);
	vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), u1OnOff, STBCAL_STBCALEN);
	vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), u1OnOff, STBCAL_STB_SELPHYCALEN);
#else
	vIO32WriteFldMulti_All(DRAMC_REG_SHUCTRL2, P_Fld(0x0, SHUCTRL2_R_DVFS_OPTION)
		| P_Fld(0x0, SHUCTRL2_R_DVFS_PARK_N));
	vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL2), 0, STBCAL2_STB_GERRSTOP);
	vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), 0, STBCAL_STBCALEN);   /* PI tracking off = HW gating tracking off */
	vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), 0, STBCAL_STB_SELPHYCALEN);
#endif
}

void DramcHWGatingDebugOnOff(DRAMC_CTX_T *p, U8 u1OnOff)
{
#ifdef HW_GATING
	vIO32WriteFldMulti_All(DRAMC_REG_STBCAL2, P_Fld(u1OnOff, STBCAL2_STB_DBG_EN_B1) |
		P_Fld(u1OnOff, STBCAL2_STB_DBG_EN_B0) |
		P_Fld(u1OnOff, STBCAL2_STB_PIDLYCG_IG) |
		P_Fld(u1OnOff, STBCAL2_STB_UIDLYCG_IG) |
		P_Fld(u1OnOff, STBCAL2_STB_GERRSTOP) |
		P_Fld(0, STBCAL2_STB_DBG_CG_AO) |
		P_Fld(0, STBCAL2_STB_DBG_UIPI_UPD_OPT));
#endif
}

#if (FOR_DV_SIMULATION_USED == 0 && SW_CHANGE_FOR_SIMULATION == 0)
#if (__ETT__ || CPU_RW_TEST_AFTER_K)
#if 0 /* Please use memeset to initail value, due to different CHANNEL_NUM */
U16 u2MaxGatingPos[CHANNEL_NUM][RANK_MAX][DQS_NUMBER] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
U16 u2MinGatingPos[CHANNEL_NUM][RANK_MAX][DQS_NUMBER] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
#endif
void DramcPrintHWGatingStatus(DRAMC_CTX_T *p, U8 u1Channel)
{
#ifdef HW_GATING
	U8 u1RankIdx, u1RankMax, u1ChannelBak, u1ByteIdx, ii;
	U32 u4ResultDQS_PI, u4ResultDQS_UI, u4ResultDQS_UI_P1;
	U8 u1Dqs_pi[DQS_BIT_NUMBER], u1Dqs_ui[DQS_BIT_NUMBER], u1Dqs_ui_P1[DQS_BIT_NUMBER];
	U16 u2TmpValue, u2TmpUI[DQS_NUMBER], u2TmpPI[DQS_NUMBER];
	U32 MANUDLLFRZ_bak, STBSTATE_OPT_bak;
	U32 backup_rank;

	u1ChannelBak = p->channel;
	vSetPHY2ChannelMapping(p, u1Channel);
	backup_rank = u1GetRank(p);

	if (p->support_rank_num == RANK_DUAL)
		u1RankMax = RANK_MAX;
	else
		u1RankMax = RANK_1;

	MANUDLLFRZ_bak = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), TEST2_3_MANUDLLFRZ);
	STBSTATE_OPT_bak = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), STBCAL_STBSTATE_OPT);

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), 1, TEST2_3_MANUDLLFRZ);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), 1, STBCAL_STBSTATE_OPT);

	for (u1RankIdx = 0; u1RankIdx < u1RankMax; u1RankIdx++)
	{
		vSetRank(p, u1RankIdx);
		mcSHOW_DBG_MSG(("[DramcHWGatingStatus] Channel = %d, Rank = %d\n", p->channel, u1RankIdx));
		u4ResultDQS_PI = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_RK0_DQSIENDLY));
		u1Dqs_pi[0] = u4ResultDQS_PI & 0xff;
		u1Dqs_pi[1] = (u4ResultDQS_PI >> 8) & 0xff;
		u1Dqs_pi[2] = (u4ResultDQS_PI >> 16) & 0xff;
		u1Dqs_pi[3] = (u4ResultDQS_PI >> 24) & 0xff;
		/* mcSHOW_DBG_MSG(("[DramcHWGatingStatus] PI status (DQS0, DQS1, DQS2, DQS3) =(%d, %d, %d, %d)\n", u1Dqs_pi[0], u1Dqs_pi[1], u1Dqs_pi[2], u1Dqs_pi[3])); */

		u4ResultDQS_UI = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_RK0_DQSIENUIDLY));
		u1Dqs_ui[0] = u4ResultDQS_UI & 0xff;
		u1Dqs_ui[1] = (u4ResultDQS_UI >> 8) & 0xff;
		u1Dqs_ui[2] = (u4ResultDQS_UI >> 16) & 0xff;
		u1Dqs_ui[3] = (u4ResultDQS_UI >> 24) & 0xff;
		/* mcSHOW_DBG_MSG(("[DramcHWGatingStatus] UI status (DQS0, DQS1, DQS2, DQS3) =(%d, %d, %d, %d)\n", u1Dqs_ui[0], u1Dqs_ui[1], u1Dqs_ui[2], u1Dqs_ui[3])); */
		mcSHOW_DBG_MSG(("Byte0(2T, 0.5T, PI) = (%d, %d, %d)\n Byte1(2T, 0.5T, PI) = (%d, %d, %d)\n Byte2(2T, 0.5T, PI) = (%d, %d, %d)\n Byte3(2T, 0.5T, PI) = (%d, %d, %d)\n", \
			u1Dqs_ui[0] / 8, u1Dqs_ui[0]%8, u1Dqs_pi[0], u1Dqs_ui[1] / 8, u1Dqs_ui[1]%8, u1Dqs_pi[1], \
			u1Dqs_ui[2] / 8, u1Dqs_ui[2]%8, u1Dqs_pi[2], u1Dqs_ui[3] / 8, u1Dqs_ui[3]%8, u1Dqs_pi[3]));

		u4ResultDQS_UI_P1 = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_RK0_DQSIENUIDLY_P1));
		u1Dqs_ui_P1[0] = u4ResultDQS_UI_P1 & 0xff;
		u1Dqs_ui_P1[1] = (u4ResultDQS_UI_P1 >> 8) & 0xff;
		u1Dqs_ui_P1[2] = (u4ResultDQS_UI_P1 >> 16) & 0xff;
		u1Dqs_ui_P1[3] = (u4ResultDQS_UI_P1 >> 24) & 0xff;
		mcSHOW_DBG_MSG(("UI_Phase1 (DQS0~3) = (%d, %d, %d, %d)\n\n", u1Dqs_ui_P1[0], u1Dqs_ui_P1[1], u1Dqs_ui_P1[2], u1Dqs_ui_P1[3]));
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
	U8 u1RankIdx, u1RankMax, u1Info_NUM, u1Info_Max_MUM = 16;
	U8 u1ChannelBak, u1RankBak;

	U8 u1DBG_Dqs0_DFS, u1DBG_Dqs0_Lead, u1DBG_Dqs0_Lag, u1DBG_Dqs0_UI, u1DBG_Dqs0_PI;
	U8 u1DBG_Dqs1_DFS, u1DBG_Dqs1_Lead, u1DBG_Dqs1_Lag, u1DBG_Dqs1_UI, u1DBG_Dqs1_PI;
	U16 u4DBG_Dqs0_Info, u4DBG_Dqs1_Info;
	U32 u4DBG_Dqs01_Info;

	U32 u4Dqs0_MAX_MIN_DLY, u4Dqs1_MAX_MIN_DLY;
	U16 u2Dqs0_UI_MAX_DLY, u2Dqs0_PI_MAX_DLY, u2Dqs0_UI_MIN_DLY, u2Dqs0_PI_MIN_DLY;
	U16 u2Dqs1_UI_MAX_DLY, u2Dqs1_PI_MAX_DLY, u2Dqs1_UI_MIN_DLY, u2Dqs1_PI_MIN_DLY;
	U8 u1ShuffleLevel;

	u1ChannelBak = p->channel;
	vSetPHY2ChannelMapping(p, u1Channel);
	u1RankBak = u1GetRank(p);

	if (p->support_rank_num == RANK_DUAL)
		u1RankMax = RANK_MAX;
	else
		u1RankMax = RANK_1;

	/*
	 * Run Time HW Gating Debug Information
	 * for(u1RankIdx=0; u1RankIdx<u1RankMax; u1RankIdx++)
	 */
	for (u1RankIdx = 0; u1RankIdx < u1RankMax; u1RankIdx++)
	{
		vSetRank(p, u1RankIdx);

		u1ShuffleLevel = u4IO32ReadFldAlign(DRAMC_REG_SHUSTATUS, SHUSTATUS_SHUFFLE_LEVEL);

		mcSHOW_DBG_MSG3(("\n[HWGatingTrackingRecord] Channel = %d, Rank = %d, SHU_LEVEL = %d\n", p->channel, u1RankIdx, u1ShuffleLevel));

		mcSHOW_DBG_MSG3(("Run Time HW Gating Debug Information :\n"));
		mcSHOW_DBG_MSG3(("        B0 = (DFS, Lead, Lag, 4T, UI, PI), B1 = (DFS, Lead, Lag, 4T, UI, PI)\n"));

		for (u1Info_NUM = 0; u1Info_NUM < u1Info_Max_MUM; u1Info_NUM++)
		{
			/*
			 * DFS_ST(Shuffle Level): bit[15:14]
			 * Shift_R(Lead): bit[13]
			 * Shift_L(Lag) : bit[12]
			 * UI_DLY : bit[11:06]
			 * PI_DLY : bit[05:00]
			 */
			u4DBG_Dqs01_Info = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_RK0_B01_STB_DBG_INFO_00 + 0x4*u1Info_NUM));
			u4DBG_Dqs0_Info = (u4DBG_Dqs01_Info >> 0) & 0xffff;
			u4DBG_Dqs1_Info = (u4DBG_Dqs01_Info >> 16) & 0xffff;

			u1DBG_Dqs0_DFS = (u4DBG_Dqs0_Info >> 14) & 0x03;
			u1DBG_Dqs0_Lead = (u4DBG_Dqs0_Info >> 13) & 0x01;
			u1DBG_Dqs0_Lag = (u4DBG_Dqs0_Info >> 12) & 0x01;
			u1DBG_Dqs0_UI = (u4DBG_Dqs0_Info >> 6) & 0x3f;
			u1DBG_Dqs0_PI = (u4DBG_Dqs0_Info >> 0) & 0x3f;

			u1DBG_Dqs1_DFS = (u4DBG_Dqs1_Info >> 14) & 0x03;
			u1DBG_Dqs1_Lead = (u4DBG_Dqs1_Info >> 13) & 0x01;
			u1DBG_Dqs1_Lag = (u4DBG_Dqs1_Info >> 12) & 0x01;
			u1DBG_Dqs1_UI = (u4DBG_Dqs1_Info >> 6) & 0x3f;
			u1DBG_Dqs1_PI = (u4DBG_Dqs1_Info >> 0) & 0x3f;

			if (u1Info_NUM < 10)
			{
				mcSHOW_DBG_MSG3(("Info = %d ", u1Info_NUM));
			}
			else
			{
				mcSHOW_DBG_MSG3(("Info = %d ", u1Info_NUM));
			}

			mcSHOW_DBG_MSG3(("B0 = (  %d, %d, %d, %d, %d, %d), B1 = (  %d, %d, %d, %d, %d, %d)\n",
				u1DBG_Dqs0_DFS, u1DBG_Dqs0_Lead, u1DBG_Dqs0_Lag, u1DBG_Dqs0_UI / 8, u1DBG_Dqs0_UI%8, u1DBG_Dqs0_PI,
				u1DBG_Dqs1_DFS, u1DBG_Dqs1_Lead, u1DBG_Dqs1_Lag, u1DBG_Dqs1_UI / 8, u1DBG_Dqs1_UI%8, u1DBG_Dqs1_PI));
		}

		/*
		 * Run Time HW Gating Max and Min Value Record
		 * Run Time HW Gating MAX_DLY UI : bit[27:22]
		 * Run Time HW Gating MAX_DLY PI : bit[21:16]
		 * Run Time HW Gating MIN_DLY UI : bit[11:06]
		 * Run Time HW Gating MIN_DLY PI : bit[05:00]
		 */
		u4Dqs0_MAX_MIN_DLY = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_RK0_B0_STB_MAX_MIN_DLY));
		u2Dqs0_UI_MAX_DLY = (u4Dqs0_MAX_MIN_DLY >> 22) & 0x3f;
		u2Dqs0_PI_MAX_DLY = (u4Dqs0_MAX_MIN_DLY >> 16) & 0x3f;
		u2Dqs0_UI_MIN_DLY = (u4Dqs0_MAX_MIN_DLY >> 6) & 0x3f;
		u2Dqs0_PI_MIN_DLY = (u4Dqs0_MAX_MIN_DLY >> 0) & 0x3f;

		u4Dqs1_MAX_MIN_DLY = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_RK0_B1_STB_MAX_MIN_DLY));
		u2Dqs1_UI_MAX_DLY = (u4Dqs1_MAX_MIN_DLY >> 22) & 0x3f;
		u2Dqs1_PI_MAX_DLY = (u4Dqs1_MAX_MIN_DLY >> 16) & 0x3f;
		u2Dqs1_UI_MIN_DLY = (u4Dqs1_MAX_MIN_DLY >> 6) & 0x3f;
		u2Dqs1_PI_MIN_DLY = (u4Dqs1_MAX_MIN_DLY >> 0) & 0x3f;

		mcSHOW_DBG_MSG3(("B0 = MAX(4T, UI, PI) MIN(4T, UI, PI), B1 = MAX(4T, UI, PI) MIN(4T, UI, PI)\n"));
		mcSHOW_DBG_MSG3(("B0 = MAX( %d, %d, %d) MIN( %d, %d, %d), B1 = MAX( %d, %d, %d) MIN( %d, %d, %d)\n",
			u2Dqs0_UI_MAX_DLY / 8, u2Dqs0_UI_MAX_DLY%8, u2Dqs0_PI_MAX_DLY,
			u2Dqs0_UI_MIN_DLY / 8, u2Dqs0_UI_MIN_DLY%8, u2Dqs0_PI_MIN_DLY,
			u2Dqs1_UI_MAX_DLY / 8, u2Dqs1_UI_MAX_DLY%8, u2Dqs1_PI_MAX_DLY,
			u2Dqs1_UI_MIN_DLY / 8, u2Dqs1_UI_MIN_DLY%8, u2Dqs1_PI_MIN_DLY));
	}
	vSetRank(p, u1RankBak);
	p->channel = u1ChannelBak;
	vSetPHY2ChannelMapping(p, u1ChannelBak);
#endif
}

void DramcPrintRXDQDQSStatus(DRAMC_CTX_T *p, U8 u1Channel)
{
	U8 u1RankIdx, u1RankMax, u1ChannelBak, u1ByteIdx, ii;
	U32 u4ResultDQS_PI, u4ResultDQS_UI, u4ResultDQS_UI_P1;
	U8 u1Dqs_pi[DQS_BIT_NUMBER], u1Dqs_ui[DQS_BIT_NUMBER], u1Dqs_ui_P1[DQS_BIT_NUMBER];
	U16 u2TmpValue, u2TmpUI[DQS_NUMBER], u2TmpPI[DQS_NUMBER];
	U32 MANUDLLFRZ_bak, STBSTATE_OPT_bak;
	U32 backup_rank;
	U8 u1DQX_B0, u1DQS0, u1DQX_B1, u1DQS1;

	u1ChannelBak = p->channel;
	vSetPHY2ChannelMapping(p, u1Channel);
	backup_rank = u1GetRank(p);

	if (p->support_rank_num == RANK_DUAL)
		u1RankMax = RANK_MAX;
	else
		u1RankMax = RANK_1;

	for (u1RankIdx = 0; u1RankIdx < u1RankMax; u1RankIdx++)
	{
		vSetRank(p, u1RankIdx);
		mcSHOW_DBG_MSG(("[RXDQDQSStatus] CH%d, RK%d\n", p->channel, u1RankIdx));
		if (u1RankIdx == 0)
			u4ResultDQS_PI = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_DQ_RXDLY_TRRO22));
		if (u1RankIdx == 1)
			u4ResultDQS_PI = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_DQ_RXDLY_TRRO23));

		u1DQX_B0 = u4ResultDQS_PI & 0xff;
		u1DQS0 = (u4ResultDQS_PI >> 8) & 0xff;
		u1DQX_B1 = (u4ResultDQS_PI >> 16) & 0xff;
		u1DQS1 = (u4ResultDQS_PI >> 24) & 0xff;

		mcSHOW_DBG_MSG(("DQX_B0, DQS0, DQX_B1, DQS1 = (%d, %d, %d, %d)\n\n", u1DQX_B0, u1DQS0, u1DQX_B1, u1DQS1));

	}
	vSetRank(p, backup_rank);

	p->channel = u1ChannelBak;
	vSetPHY2ChannelMapping(p, u1ChannelBak);
}


void DramcPrintIMPTrackingStatus(DRAMC_CTX_T *p, U8 u1Channel)
{
#ifdef IMPEDANCE_TRACKING_ENABLE

	U8 u1RankIdx, u1RankMax, u1ChannelBak, u1ByteIdx, ii;
	U8 DQS_DRVN_2, DQS_DRVP_2, DQS_ODTN_2, DQS_DRVN, DQS_DRVP, DQS_ODTN;
	U8 DQ_DRVN_2, DQ_DRVP_2, DQ_ODTN_2, DQ_DRVN, DQ_DRVP, DQ_ODTN;
	U8 CMD_DRVN_2, CMD_DRVP_2, CMD_ODTN_2, CMD_DRVN, CMD_DRVP, CMD_ODTN;

	u1ChannelBak = p->channel;
	vSetPHY2ChannelMapping(p, u1Channel);

	mcSHOW_DBG_MSG(("[IMPTrackingStatus] CH = %d\n", p->channel));

	/* if (u1Channel == CHANNEL_A) */
	{/* 460 464: ODTN DRVP, 468 46C: DRVN DRVP */

		/* DQS */
		DQS_DRVN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQDRV_STATUS), DQDRV_STATUS_DRVNDQS_2);
		DQS_DRVP_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS1), IMPCAL_STATUS1_DRVPDQS_SAVE2);
		DQS_ODTN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS1), IMPCAL_STATUS1_DRVNDQS_SAVE2);
		DQS_DRVN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQDRV_STATUS), DQDRV_STATUS_DRVNDQS_1);
		DQS_DRVP = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS1), IMPCAL_STATUS1_DRVPDQS_SAVE1);
		DQS_ODTN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS1), IMPCAL_STATUS1_DRVNDQS_SAVE1);

		/* DQ */
		DQ_DRVN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQDRV_STATUS), DQDRV_STATUS_DRVNDQ_2);
		DQ_DRVP_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS1), IMPCAL_STATUS1_DRVPDQ_SAVE2);
		DQ_ODTN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS1), IMPCAL_STATUS1_DRVNDQ_SAVE2);
		DQ_DRVN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CMDDRV_STATUS), CMDDRV_STATUS_DRVNDQ_1);
		DQ_DRVP = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS2), IMPCAL_STATUS2_DRVPDQ_SAVE1);
		DQ_ODTN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_IMPCAL_STATUS2), IMPCAL_STATUS2_DRVNDQ_SAVE1);

		/* CMD */
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

		/* Channel B is workaround */
		shu_level = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHUSTATUS), SHUSTATUS_SHUFFLE_LEVEL);
		mcSHOW_DBG_MSG(("shu_level = %d\n", shu_level));

		/* DQ */
		DQ_DRVP_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING1 + shu_level * SHU_GRP_DRAMC_OFFSET), SHU1_DRVING1_DQDRV2_DRVP);
		DQ_DRVP = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING2 + shu_level * SHU_GRP_DRAMC_OFFSET), SHU1_DRVING2_DQDRV1_DRVP);
		DQ_ODTN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING3 + shu_level * SHU_GRP_DRAMC_OFFSET), SHU1_DRVING3_DQODT2_ODTN);
		DQ_ODTN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING4 + shu_level * SHU_GRP_DRAMC_OFFSET), SHU1_DRVING4_DQODT1_ODTN);

		/* DQS */
		DQS_DRVP_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING1 + shu_level * SHU_GRP_DRAMC_OFFSET), SHU1_DRVING1_DQSDRV2_DRVP);
		DQS_DRVP = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING1 + shu_level * SHU_GRP_DRAMC_OFFSET), SHU1_DRVING1_DQSDRV1_DRVP);
		DQS_ODTN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING3 + shu_level * SHU_GRP_DRAMC_OFFSET), SHU1_DRVING3_DQSODT2_ODTN);
		DQS_ODTN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING3 + shu_level * SHU_GRP_DRAMC_OFFSET), SHU1_DRVING3_DQSODT1_ODTN);

		/* CMD */
		CMD_DRVP_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING2 + shu_level * SHU_GRP_DRAMC_OFFSET), SHU1_DRVING2_CMDDRV2_DRVP);
		CMD_DRVP = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING2 + shu_level * SHU_GRP_DRAMC_OFFSET), SHU1_DRVING2_CMDDRV1_DRVP);
		CMD_ODTN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING4 + shu_level * SHU_GRP_DRAMC_OFFSET), SHU1_DRVING4_CMDODT2_ODTN);
		CMD_ODTN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING4 + shu_level * SHU_GRP_DRAMC_OFFSET), SHU1_DRVING4_CMDODT1_ODTN);
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
#endif  /* #if __ETT__ */
#endif


U8 u1GetMR4RefreshRate(DRAMC_CTX_T *p, DRAM_CHANNEL_T channel)
{
	U8 u1Dummy, u1RefreshRate;

	/* vSetPHY2ChannelMapping(p, channel); */

	/*
	 * DramcModeRegRead(p, 4, &u1Dummy);
	 * mcSHOW_DBG_MSG(("[u2GetRefreshRate] MR4 0x%x,  u1RefreshRate= 0x%x\n", u1Dummy, u1RefreshRate));
	 */
	u1RefreshRate = (U8)u4IO32ReadFldAlign((DRAMC_REG_MISC_STATUSA) + (channel << POS_BANK_NUM), MISC_STATUSA_REFRESH_RATE);
	/* mcSHOW_DBG_MSG(("[u2GetRefreshRate] channel = %d, u1RefreshRate= 0x%x\n", channel, u1RefreshRate)); */

	return u1RefreshRate;
}


U8 gEnableSelfWakeup = 0;
void DramcDRS(DRAMC_CTX_T *p, U8 bEnable)
{
	if (u1IsLP4Family(p->dram_type))
	{
		/* R_DMDRS_CNTX[6:0](DVT set 0, HQA set 4 or 5) */
		vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_DRSCTRL), P_Fld(0, DRSCTRL_DRSPB2AB_OPT)
			| P_Fld(0, DRSCTRL_DRSMON_CLR)
			| P_Fld(8, DRSCTRL_DRSDLY)
			| P_Fld(0, DRSCTRL_DRSACKWAITREF)
			| P_Fld(!bEnable, DRSCTRL_DRSDIS)
			| P_Fld(1, DRSCTRL_DRSCLR_EN)
			| P_Fld(3, DRSCTRL_DRS_CNTX)
			| P_Fld(!gEnableSelfWakeup, DRSCTRL_DRS_SELFWAKE_DMYRD_DIS)
			| P_Fld(0, DRSCTRL_DRSOPT2));

		vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD), P_Fld(1, DUMMY_RD_DMYRD_REORDER_DIS)
			| P_Fld(0, DUMMY_RD_DMYRD_HPRI_DIS));
	}
}


#define CKGEN_FMETER 0x0
#define ABIST_FMETER 0x1

/*
 * 1. Select meter clock input: CLK_DBG_CFG[1:0] = 0x0
 * 2. Select clock source from below table: CLK_DBG_CFG[21:16] = 0x39
 * 3. Setup meter div: CLK_MISC_CFG_0[31:24] = 0x0
 * 4. Enable frequency meter: CLK26CALI_0[12] = 0x1
 * 5. Trigger frequency meter: CLK26CALI_0[4] = 0x1
 * 6. Wait until CLK26CALI_0[4] = 0x0
 * 7. Read meter count: CLK26CALI_1[15:0]
 * 8. Calculate measured frequency: freq. = (26 * cal_cnt) / 1024
 *
 * DE: Mas Lin
 */
static U32 FMeter(U8 u1CLKMeterSel, U8 u1CLKMuxSel)
{
#if (FOR_DV_SIMULATION_USED == 0 && SW_CHANGE_FOR_SIMULATION == 0)
	U32 tmp, u4CalCnt;

	/* enable fmeter */
	DRV_WriteReg32(CLK26CALI_0, (0x1 << 12)); /* [12] = 1: [8] = 0: switch in_clk / clk27m */

	/* select meter */
	tmp = DRV_Reg32(CLK_DBG_CFG);
	tmp &= ~0x3;

	if (u1CLKMeterSel == CKGEN_FMETER)
		tmp |= 0x1;

	DRV_WriteReg32(CLK_DBG_CFG, tmp);

	/* select fmeter's input clock */
	tmp = DRV_Reg32(CLK_DBG_CFG);

	if (u1CLKMeterSel == CKGEN_FMETER) {
		tmp &= ~(0x3F << 8);
		tmp |= u1CLKMuxSel << 8;
	} else {
		tmp &= ~(0x3F << 16);
		tmp |= u1CLKMuxSel << 16;
	}

	DRV_WriteReg32(CLK_DBG_CFG, tmp);

	/* setup fmeter div */
	tmp = DRV_Reg32(CLK_MISC_CFG_0);
	tmp &= ~(0xFF << 24);
	DRV_WriteReg32(CLK_MISC_CFG_0, tmp);

	/* set load_cnt to 0x3FF (1024 - 1) */
	tmp = DRV_Reg32(CLK26CALI_1);
	tmp &= ~(0x3FF << 16);
	tmp |= 0x3FF << 16;
	DRV_WriteReg32(CLK26CALI_1, tmp);

	/* trigger fmeter */
	tmp = DRV_Reg32(CLK26CALI_0);
	tmp |= (0x1 << 4);
	DRV_WriteReg32(CLK26CALI_0, tmp);

	/* wait */
	while (DRV_Reg32(CLK26CALI_0) & (0x1 << 4)) {
		mcDELAY_US(1);
	}

	/* read */
	u4CalCnt = DRV_Reg32(CLK26CALI_1) & 0xFFFF;

	/* disable fmeter */
	tmp = DRV_Reg32(CLK26CALI_0);
	tmp &= ~(0x1 << 12);
	DRV_WriteReg32(CLK26CALI_0, tmp);

	return ((u4CalCnt * 26) / 1024);
#endif
}

U16 gddrphyfmeter_value = 0;
void DDRPhyFMeter_Init()
{
#if ((FOR_DV_SIMULATION_USED == 0 && SW_CHANGE_FOR_SIMULATION == 0) || FT_DSIM_USED)
#if (fcFOR_CHIP_ID == fcTalbot)
	U32 reg;
	/* enable ck_bfe_dcm_en for freqmeter measure ddrphy clock, not needed for normal use */
	reg = DRV_Reg32(0x10228000 + 0x2a0);
	DRV_WriteReg32(0x10228000 + 0x2a0, reg | (1 << 11));

	/* ckgen_clk56: fmem_ck_aft_dcm_ch0 (DRAMC CHA's clock after idle mask, the one really be used) */
#if FT_DSIM_USED == 0
	gddrphyfmeter_value = FMeter(CKGEN_FMETER, 55) << 2;
#endif

	DRV_WriteReg32(0x10228000 + 0x2a0, reg);
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
#if ((FOR_DV_SIMULATION_USED == 0 && SW_CHANGE_FOR_SIMULATION == 0) || FT_DSIM_USED)
	U32 reg;
	U32 before_value, after_value;

	mcSHOW_DBG_MSG3(("\n[PhyFreqMeter]\n"));

	/* abist_clk50: AD_MPLL_208M_CK */
	mcSHOW_DBG_MSG(("AD_MPLL_208M_CK FREQ = %d\n", FMeter(ABIST_FMETER, 50)));

	/* abist_clk57: DA_MPLL_52M_DIV_CK */
	mcSHOW_DBG_MSG(("DA_MPLL_52M_DIV_CK FREQ = %d\n", FMeter(ABIST_FMETER, 57)));

	/* enable ck_bfe_dcm_en for freqmeter measure ddrphy clock, not needed for normal use */
	reg = DRV_Reg32(DRAMC_CH0_TOP0_BASE + 0x2a0);
	DRV_WriteReg32(DRAMC_CH0_TOP0_BASE + 0x2a0, reg | (1 << 11));

	/* ckgen_clk55: fmem_ck_bfe_dcm_ch0 (DRAMC CHA's clock before idle mask) */
	before_value = FMeter(CKGEN_FMETER, 55);
	mcSHOW_DBG_MSG(("fmem_ck_bfe_dcm_ch0 FREQ = %d\n", FMeter(CKGEN_FMETER, 55)));

	/* ckgen_clk56: fmem_ck_aft_dcm_ch0 (DRAMC CHA's clock after idle mask, the one really be used) */
	after_value = FMeter(CKGEN_FMETER, 56);
	mcSHOW_DBG_MSG(("fmem_ck_aft_dcm_ch0 FREQ = %d\n", FMeter(CKGEN_FMETER, 56)));

	DRV_WriteReg32(DRAMC_CH0_TOP0_BASE + 0x2a0, reg);

	return ((before_value << 16) | after_value);
#endif
}
#else
U32 DDRPhyFreqMeter()
{
	return 0;
}
#endif


#if ((FOR_DV_SIMULATION_USED == 0 && SW_CHANGE_FOR_SIMULATION == 0) || FT_DSIM_USED)
#if __ETT__
void GetPhyPllFrequency(DRAMC_CTX_T *p)
{
#if 1
	U8 u1ShuLevel = u4IO32ReadFldAlign(DRAMC_REG_SHUSTATUS, SHUSTATUS_SHUFFLE_LEVEL);
	U32 u4PLL5_ADDR = DDRPHY_SHU1_PLL5 + SHU_GRP_DDRPHY_OFFSET * u1ShuLevel;
	U32 u4PLL8_ADDR = DDRPHY_SHU1_PLL8 + SHU_GRP_DDRPHY_OFFSET * u1ShuLevel;
	U32 u4CA_CMD6 = DDRPHY_SHU1_CA_CMD6 + SHU_GRP_DDRPHY_OFFSET * u1ShuLevel;

	/* VCOFreq = FREQ_XTAL x ((RG_RCLRPLL_SDM_PCW) / 2^(RG_*_RCLRPLL_PREDIV)) / 2^(RG_*_RCLRPLL_POSDIV) */
	U32 u4SDM_PCW = u4IO32ReadFldAlign(u4PLL5_ADDR, SHU1_PLL5_RG_RPHYPLL_SDM_PCW);
	U32 u4PREDIV = u4IO32ReadFldAlign(u4PLL8_ADDR, SHU1_PLL8_RG_RPHYPLL_PREDIV);
	U32 u4POSDIV = u4IO32ReadFldAlign(u4PLL8_ADDR, SHU1_PLL8_RG_RPHYPLL_POSDIV);
	U32 u4CKDIV4 = u4IO32ReadFldAlign(u4CA_CMD6, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA);

	U32 u4VCOFreq = ((52 >> u4PREDIV)*(u4SDM_PCW >> 8)) >> u4POSDIV;
	U32 u4DataRate = u4VCOFreq >> u4CKDIV4;
#else
	U8 u1ShuLevel = (*((volatile unsigned int *)(0x10006474)) & 0x000000c0) >> 6;
	U32 u4PLL5_ADDR = 0x10228d94 + SHU_GRP_DDRPHY_OFFSET * u1ShuLevel;
	U32 u4PLL8_ADDR = 0x10228da0 + SHU_GRP_DDRPHY_OFFSET * u1ShuLevel;
	U32 u4CA_CMD6 = 0x10228d18 + SHU_GRP_DDRPHY_OFFSET * u1ShuLevel;

	U32 u4SDM_PCW = *((volatile unsigned int *)(u4PLL5_ADDR)) >> 16;
	U32 u4PREDIV = (*((volatile unsigned int *)(u4PLL8_ADDR)) & 0x000c0000) >> 18;
	U32 u4POSDIV = (*((volatile unsigned int *)(u4PLL8_ADDR)) & 0x00000007);
	U32 u4CKDIV4 = (*((volatile unsigned int *)(u4CA_CMD6)) & 0x08000000) >> 27;


	U32 u4VCOFreq = ((52 >> u4PREDIV)*(u4SDM_PCW >> 8)) >> u4POSDIV;
	U32 u4DataRate = u4VCOFreq >> u4CKDIV4;
#endif
	mcSHOW_DBG_MSG(("PCW = 0x%X, u4PREDIV = %d, u4POSDIV = %d, CKDIV4 = %d, DataRate = %d\n", u4SDM_PCW, u4PREDIV, u4POSDIV, u4CKDIV4, u4DataRate));
}
#else
void GetPhyPllFrequency(DRAMC_CTX_T *p)
{
}
#endif
#endif

#if __ETT__
const U32 u4Cannot_Use_Dramc_WBR_Reg[] =
{
	DDRPHY_SHU1_CA_CMD6,
	DDRPHY_SHU1_CA_DLL0,
	DDRPHY_SHU1_CA_DLL1,
	DDRPHY_CA_DLL_ARPI3,
	DDRPHY_MISC_SHU_OPT,
	DDRPHY_CKMUX_SEL
};
#define CANNOT_USE_WBR_SIZE ((sizeof (u4Cannot_Use_Dramc_WBR_Reg)) / (sizeof (U32)))
void CheckDramcWBR(U32 u4address)
{
	U32 i, channel_and_value;
	if (GetDramcBroadcast() == DRAMC_BROADCAST_ON)
	{
	#if ((CHANNEL_NUM == 1) || (CHANNEL_NUM == 2))
		channel_and_value = 0x1;
	#else /* for channel number = 3 or 4 */
		channel_and_value = 0x3;
	#endif
		if ((((u4address - Channel_A_DRAMC_NAO_BASE_VIRTUAL) >> POS_BANK_NUM) & channel_and_value) != CHANNEL_A)
		{
			mcSHOW_ERR_MSG(("Error! virtual address 0x%x is not CHA and cannot use Dramc WBR\n", u4address));
			while (1);
		}
		for (i = 0; i < CANNOT_USE_WBR_SIZE; i++)
		{
			if (u4Cannot_Use_Dramc_WBR_Reg[i] == u4address)
			{
				mcSHOW_ERR_MSG(("Error! virtual address 0x%x cannot use Dramc WBR\n", u4address));
				while (1);
			}
		}
	}
}
#endif

/*
 * Since initial register settings are for LP3_DDR1866, TX DQS, DQS_OEN (output enable), DQ delay
 * need to be adjusted according to the difference between Write Latencies for target frequency group
 */
static void TxCalibrationMoveDQSByDifferentWL(DRAMC_CTX_T *p)
{
	U8 u1ByteIdx, u1ACTableIdx = 0;
	U8 backup_rank, u1RankIdx;
	S8 s1WriteLatencyDiff = 0; /* Difference of write latency compared with LP3_DDR1866 */
	S8 s1TargetFreqGroupWL = 0, s1FreqGroup933WL = 0;

	/* Find target freqGroup, and LP3_DDR1866's Write latency ACTimings */
	for (u1ACTableIdx = 0; u1ACTableIdx < TOTAL_AC_TIMING_NUMBER; u1ACTableIdx++)
	{
		if (ACTimingTbl[u1ACTableIdx].dramType != TYPE_LPDDR3)  /* This function is for LP3 only, skip other dram type ACTimingTable entries */
		{
			continue;
		}

		if (ACTimingTbl[u1ACTableIdx].freq == p->freqGroup) /* Retrieve target ACTiming FreqGroup write latency */
		{
			s1TargetFreqGroupWL = ACTimingTbl[u1ACTableIdx].writeLat;
		}

		if (ACTimingTbl[u1ACTableIdx].freq == 933) /* Retrieve LP3_DDR1866 (933Mhz) write latency */
		{
			s1FreqGroup933WL = ACTimingTbl[u1ACTableIdx].writeLat;
		}
	}

	/* Check if either source (LP3_DDR1866) or target FreqGroup's Write latency wasn't found */
	if ((s1FreqGroup933WL == 0) || (s1TargetFreqGroupWL == 0))
	{
		mcSHOW_ERR_MSG(("Write latency search err!\n"));
	#if __ETT__
		while (1);
	#endif
	}

	s1WriteLatencyDiff = ((s1FreqGroup933WL - s1TargetFreqGroupWL) << 1); /* Calculate Write Latency Difference (compared with LP3_DDR1866) */
	mcSHOW_DBG_MSG3(("u1WriteLatencyDiff x2 = %d\n", s1WriteLatencyDiff));

	backup_rank = u1GetRank(p);

	/* Set DQS, DQS_OEN and DQ delay according to Write Latency difference with LP3_DDR1866 */
	for (u1ByteIdx = 0 ; u1ByteIdx < (p->data_width / DQS_BIT_NUMBER); u1ByteIdx++)
	{
		MoveDramC_TX_DQS(p, u1ByteIdx, - s1WriteLatencyDiff);
		MoveDramC_TX_DQS_OEN(p, u1ByteIdx, - s1WriteLatencyDiff);

		for (u1RankIdx = RANK_0; u1RankIdx < p->support_rank_num; u1RankIdx++)
		{
			vSetRank(p, u1RankIdx);
			MoveDramC_TX_DQ(p, u1ByteIdx, - s1WriteLatencyDiff);
			MoveDramC_TX_DQ_OEN(p, u1ByteIdx, - s1WriteLatencyDiff);
		}
		vSetRank(p, backup_rank);
	}

	return;
}


#ifdef DDR_INIT_TIME_PROFILING
U32 l_low_tick0, l_high_tick0, l_low_tick1, l_high_tick1;
void TimeProfileBegin(void)
{
#if __ETT__
	l_low_tick0 = GPT_GetTickCount( & l_high_tick0);
#else
	l_low_tick0 = get_timer(0);
#endif
}

U32 TimeProfileEnd(void)
{
#if __ETT__
	l_low_tick1 = GPT_GetTickCount( & l_high_tick1);

	/*
	 * mcSHOW_TIME_MSG(("Time0 %u %u\n", l_high_tick0, l_low_tick0));
	 * mcSHOW_TIME_MSG(("Time1 %u %u\n", l_high_tick1, l_low_tick1));
	 */
	return ((l_low_tick1 - l_low_tick0)*76) / 1000;
#else
	l_low_tick1 = get_timer(l_low_tick0);
	return l_low_tick1 * 1000;
#endif
}
#endif

U32 u4gpRegBackupVlaue[64];
void DramcBackupRegisters(DRAMC_CTX_T *p, U32 *backup_addr, U32 backup_num)
{
	U32 u4RegIdx;

#if __ETT__
	if (backup_num > 64)
	{
		mcSHOW_ERR_MSG(("[DramcBackupRegisters] backup number over 64!!!\n"));
		while (1);
	}
#endif

	for (u4RegIdx = 0; u4RegIdx < backup_num; u4RegIdx++)
	{
		u4gpRegBackupVlaue[u4RegIdx] = u4IO32Read4B(backup_addr[u4RegIdx]);
		/* mcSHOW_DBG_MSG(("Backup Reg(0x%X) = 0x%X\n", backup_addr[u4RegIdx], u4gpRegBackupVlaue[u4RegIdx])); */
	}
}

void DramcRestoreRegisters(DRAMC_CTX_T *p, U32 *restore_addr, U32 restore_num)
{
	U32 u4RegIdx;

	for (u4RegIdx = 0; u4RegIdx < restore_num; u4RegIdx++)
	{
		vIO32Write4B(restore_addr[u4RegIdx], u4gpRegBackupVlaue[u4RegIdx]);
		/* mcSHOW_DBG_MSG(("Restore Reg(0x%X) = 0x%X\n", restore_addr[u4RegIdx], u4gpRegBackupVlaue[u4RegIdx])); */
	}
}


#if defined(TEST_MODE_MRS) || SAMSUNG_TEST_MODE_MRS_FOR_PRELOADER
const U8 PME_Key[4][6] =
{
	{ 2, 5, 5, 2, 4, 8},
	{ 2, 5, 5, 2, 4, 8},
	{ 2, 5, 5, 2, 4, 8},
	{ 10, 13, 13, 10, 12, 13}
};


static U32 u4TestModeV0[2] = {};
static U32 u4TestModeV1[2] = {};
static U32 u4TestModeV2[2] = {};
static U32 u4TestModeV3[2] = {};
static U32 u4TestModeV4[2] = {};
static U32 u4TestModeV5[2] = {};
static U32 u4TestModeV6[2] = {};

void BackupRGBeforeTestMode(DRAMC_CTX_T *p)
{
	DRAM_CHANNEL_T eOriChannel = p->channel;
	int i = 0;
	p->channel = CHANNEL_A;
	for (i = 0; i < 2; i++)
	{
		if (i == 0)
		{
			mcSHOW_DBG_MSG(("***CHA\n"));
		}
		else
		{
			mcSHOW_DBG_MSG(("***CHB\n"));
		}

		u4TestModeV0[i] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_MRS));
		mcSHOW_DBG_MSG4(("DRAMC_REG_MRS[0x%x]\n", u4TestModeV0[i]));
		u4TestModeV1[i] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL));
		mcSHOW_DBG_MSG4(("DRAMC_REG_DRAMC_PD_CTRL[0x%x]\n", u4TestModeV1[i]));
		u4TestModeV2[i] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_STBCAL)); /* STBCAL_DQSIENCG_NORMAL_EN */
		mcSHOW_DBG_MSG4(("DRAMC_REG_STBCAL[0x%x]\n", u4TestModeV2[i]));
		u4TestModeV3[i] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0)); /* Auto refresh */
		mcSHOW_DBG_MSG4(("DRAMC_REG_REFCTRL0[0x%x]\n", u4TestModeV3[i]));
		u4TestModeV4[i] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL));
		mcSHOW_DBG_MSG4(("DRAMC_REG_SPCMDCTRL[0x%x]\n", u4TestModeV4[i]));
		u4TestModeV5[i] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL));
		mcSHOW_DBG_MSG4(("DRAMC_REG_CKECTRL[0x%x]\n", u4TestModeV5[i]));
		u4TestModeV6[i] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE));
		mcSHOW_DBG_MSG4(("DRAMC_REG_SLP4_TESTMODE[0x%x]\n", u4TestModeV6[i]));
		p->channel = CHANNEL_B;
	}
	p->channel = eOriChannel;
	return;

}

void RestoreRGBeforeTestMode(DRAMC_CTX_T *p)
{
	DRAM_CHANNEL_T eOriChannel = p->channel;
	int i = 0;
	p->channel = CHANNEL_A;
	for (i = 0; i < 2; i++)
	{
		vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_MRS), u4TestModeV0[i]);
		vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), u4TestModeV1[i]);
		vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), u4TestModeV2[i]);
		vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0), u4TestModeV3[i]);
		vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), u4TestModeV4[i]);
		vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), u4TestModeV5[i]);
		vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), u4TestModeV6[i]);

		p->channel = CHANNEL_B;
	}
	p->channel = eOriChannel;
	return;
}

void ProgramModeEnter(DRAMC_CTX_T *p)
{
	DRAM_CHANNEL_T eOriChannel = p->channel;
	int i = 0;
	int j = 0;
	int k = 0;
	U8 u1Rank = 0;

	BackupRGBeforeTestMode(p);
	p->channel = CHANNEL_A;
	mcSHOW_DBG_MSG(("ProgramModeEnter [Begin]\n"));

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 6; j++)
		{
			mcSHOW_DBG_MSG(("Key[%d] CA[%d] = > [%d]\n", i, j, PME_Key[i][j]));
		}
		mcSHOW_DBG_MSG(("\n"));
	}

	for (i = 0; i < 2; i++)
	{
		if (i == 0)
		{
			mcSHOW_DBG_MSG(("***CHA\n"));
		}
		else
		{
			mcSHOW_DBG_MSG(("***CHB\n"));
		}
		for (j = 0; j < 2; j++)
		{
			if (j == 0)
			{
				u1Rank = 0;
				mcSHOW_DBG_MSG(("***R0\n"));
			}
			else
			{
				u1Rank = 1;
				mcSHOW_DBG_MSG(("***R1\n"));
			}
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1Rank, MRS_MRSRK);

			/* Disable dramc and DDRPHY clock gated, let clock freerun */
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 0, DRAMC_PD_CTRL_DCMEN);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 0, DRAMC_PD_CTRL_PHYCLKDYNGEN);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), 0, STBCAL_DQSIENCG_NORMAL_EN);

			/* Disable MR4, refresh */
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0), 1, REFCTRL0_REFDIS);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), 1, SPCMDCTRL_REFRDIS);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), 1, CKECTRL_CKEFIXON);

			for (k = 0; k < 4; k++)
			{
				vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), PME_Key[k][0], SLP4_TESTMODE_CA0_TEST);
				vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), PME_Key[k][1], SLP4_TESTMODE_CA1_TEST);
				vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), PME_Key[k][2], SLP4_TESTMODE_CA2_TEST);
				vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), PME_Key[k][3], SLP4_TESTMODE_CA3_TEST);
				vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), PME_Key[k][4], SLP4_TESTMODE_CA4_TEST);
				vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), PME_Key[k][5], SLP4_TESTMODE_CA5_TEST);

				vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), 1, SLP4_TESTMODE_STESTEN);
				while (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_STEST_RESPONSE))
				{
					mcSHOW_DBG_MSG(("status[%d]: %d\n", k, u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_STEST_RESPONSE)));
					break;
				}
				vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), 0, SLP4_TESTMODE_STESTEN);
			}
		}
		p->channel = CHANNEL_B;
	}
	mcSHOW_DBG_MSG(("ProgramModeEnter [Stop]\n"));
	p->channel = eOriChannel;
	RestoreRGBeforeTestMode(p);
	return;
}

void ProgramCodeInput(DRAMC_CTX_T *p, U16 u2A_value, U16 u2B_value, U16 u2C_value)
{
	DRAM_CHANNEL_T eOriChannel = p->channel;
	int i = 0;
	int j = 0;
	int k = 0;
	int keyNumber = 0;
	U16 u2Value = 0;
	U8 u1Rank = 0;

	BackupRGBeforeTestMode(p);
	p->channel = CHANNEL_A;
	/* CA0, CA1, CA2, CA3, CA4, CA5 */
	U8 PCI_Key[5][6] =
	{
		{ 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0},
		{ 2, 5, 1, 2, 0, 0},
		{ 2, 5, 1, 2, 0, 0}
	};

	mcSHOW_DBG_MSG(("ProgramCodeInput [Begin]\n"));

	for (keyNumber = 0; keyNumber < 3; keyNumber++)
	{
		if (keyNumber == 0)
			u2Value = u2A_value;
		else if (keyNumber == 1)
			u2Value = u2B_value;
		else
			u2Value = u2C_value;

		PCI_Key[keyNumber][0] = (u2Value == 0) ? (10) : (2);
		PCI_Key[keyNumber][1] = (u2Value == 1) ? (13) : (5);
		PCI_Key[keyNumber][2] = (u2Value == 10) ? (5) : ((u2Value == 2) ? (9):(1));
		PCI_Key[keyNumber][3] = (u2Value == 9) ? (6) : ((u2Value == 3) ? (10):(2));
		PCI_Key[keyNumber][4] = (u2Value == 8) ? (4) : ((u2Value == 4) ? (8):(0));
		PCI_Key[keyNumber][5] = (u2Value == 7) ? (1) : ((u2Value == 6) ? (4):((u2Value == 5) ? (8):(0)));
	}

	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 6; j++)
		{
			mcSHOW_DBG_MSG(("Key[%d] CA[%d] = > [%d]\n", i, j, PCI_Key[i][j]));
		}
		mcSHOW_DBG_MSG(("\n"));
	}

	for (i = 0; i < 2; i++)
	{
		if (i == 0)
		{
			mcSHOW_DBG_MSG(("***CHA\n"));
		}
		else
		{
			mcSHOW_DBG_MSG(("***CHB\n"));
		}
		for (j = 0; j < 2; j++)
		{
			if (j == 0)
			{
				u1Rank = 0;
				mcSHOW_DBG_MSG(("***R0\n"));
			}
			else
			{
				u1Rank = 1;
				mcSHOW_DBG_MSG(("***R1\n"));
			}
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1Rank, MRS_MRSRK);

			/* Disable dramc and DDRPHY clock gated, let clock freerun */
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 0, DRAMC_PD_CTRL_DCMEN);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 0, DRAMC_PD_CTRL_PHYCLKDYNGEN);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_STBCAL), 0, STBCAL_DQSIENCG_NORMAL_EN);

			/* Disable MR4, refresh */
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0), 1, REFCTRL0_REFDIS);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), 1, SPCMDCTRL_REFRDIS);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), 1, CKECTRL_CKEFIXON);

			for (k = 0; k < 5; k++)
			{
				vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), PCI_Key[k][0], SLP4_TESTMODE_CA0_TEST);
				vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), PCI_Key[k][1], SLP4_TESTMODE_CA1_TEST);
				vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), PCI_Key[k][2], SLP4_TESTMODE_CA2_TEST);
				vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), PCI_Key[k][3], SLP4_TESTMODE_CA3_TEST);
				vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), PCI_Key[k][4], SLP4_TESTMODE_CA4_TEST);
				vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), PCI_Key[k][5], SLP4_TESTMODE_CA5_TEST);

				vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), 1, SLP4_TESTMODE_STESTEN);
				while (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_STEST_RESPONSE))
				{
					mcSHOW_DBG_MSG(("status[%d]: %d\n", k, u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_STEST_RESPONSE)));
					break;
				}
				vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SLP4_TESTMODE), 0, SLP4_TESTMODE_STESTEN);
			}
		}
		p->channel = CHANNEL_B;
	}
	mcSHOW_DBG_MSG(("ProgramCodeInput [Stop]\n"));
	p->channel = eOriChannel;
	RestoreRGBeforeTestMode(p);
	return;
}

void vApplyProgramSequence(DRAMC_CTX_T *p)
{
#if SAMSUNG_TEST_MODE_MRS_FOR_PRELOADER == 1
	/*
	 * Buffer sensitivity decrease1
	 * test 2. TMRS enter -> 000 -> 390 -> 120 -> 8A7
	 */
	ProgramModeEnter(p);
	ProgramCodeInput(p, 0, 0, 0);
	ProgramCodeInput(p, 3, 9, 0);
	ProgramCodeInput(p, 1, 2, 0);
	ProgramCodeInput(p, 8, 0xa, 7);
	ProgramCodeInput(p, 2, 5, 8);
	mcSHOW_DBG_MSG(("Buffer sensitivity decrease1: TMRS enter -> 000 -> 390 -> 120 -> 8A7 -> 258\n"));

#elif SAMSUNG_TEST_MODE_MRS_FOR_PRELOADER == 2
	/*
	 * Buffer sensitivity decrease2
	 * test 3. TMRS enter -> 000 -> 390 -> 120 -> 803
	 */
	ProgramModeEnter(p);
	ProgramCodeInput(p, 0, 0, 0);
	ProgramCodeInput(p, 3, 9, 0);
	ProgramCodeInput(p, 1, 2, 0);
	ProgramCodeInput(p, 8, 0, 3);
	ProgramCodeInput(p, 2, 5, 8);
	mcSHOW_DBG_MSG(("Buffer sensitivity decrease1: TMRS enter -> 000 -> 390 -> 120 -> 803 -> 258\n"));

#elif SAMSUNG_TEST_MODE_MRS_FOR_PRELOADER == 3
	/*
	 * 2014 + 2863
	 * test 4.TMRS enter -> 000 -> 390 -> 120 -> 014 -> 863
	 */
	ProgramModeEnter(p);
	ProgramCodeInput(p, 0, 0, 0);
	ProgramCodeInput(p, 3, 9, 0);
	ProgramCodeInput(p, 1, 2, 0);
	ProgramCodeInput(p, 0, 1, 4);
	ProgramCodeInput(p, 8, 6, 3);
	ProgramCodeInput(p, 2, 5, 8);
	mcSHOW_DBG_MSG(("2014 + 2863: test 4.TMRS enter -> 000 -> 390 -> 120 -> 014 -> 863 -> 258\n"));

#elif SAMSUNG_TEST_MODE_MRS_FOR_PRELOADER == 4
	/*
	 * 252A
	 * test 5. TMRS enter -> 000 -> 390 -> 120 -> 52A
	 */
	ProgramModeEnter(p);
	ProgramCodeInput(p, 0, 0, 0);
	ProgramCodeInput(p, 3, 9, 0);
	ProgramCodeInput(p, 1, 2, 0);
	ProgramCodeInput(p, 5, 2, 0xa);
	ProgramCodeInput(p, 2, 5, 8);
	mcSHOW_DBG_MSG(("252A: test 5. TMRS enter -> 000 -> 390 -> 120 -> 52A -> 258\n"));

#else
	/* test 1. TMRS enter -> 000 -> 390 -> 021 */
	ProgramModeEnter(p);
	ProgramCodeInput(p, 0, 0, 0);
	ProgramCodeInput(p, 3, 9, 0);
	ProgramCodeInput(p, 0, 2, 1);
	ProgramCodeInput(p, 2, 5, 8);
	mcSHOW_DBG_MSG(("Buffer sensitivity decrease1: TMRS enter -> 000 -> 390 -> 021\n"));
#endif
}
#endif


/*
 * ACTimingTbl: All freq's ACTiming from ACTiming excel file
 * (Some fields don't exist for LP3 -> set to 0)
 * Note: !!All ACTiming adjustments should not be set in-table should be moved into UpdateACTimingReg()!!
 * Or else preloader's highest freq ACTimings may be set to different values than expected.
 */
const ACTime_T ACTimingTbl[TOTAL_AC_TIMING_NUMBER] =
{
	/* ----------LPDDR4--------------------------- */
#if SUPPORT_LP4_DDR3733_ACTIM
	/* LP4_DDR3733 ACTiming--------------------------------- */
#if (ENABLE_READ_DBI == 1)
	/*
	 * AC_TIME_LP4_BYTE_DDR3733_DBI_ON
	 * LP4-3733, 1866MHz, RDBI_ON, byte mode
	 */
	{
		.dramType = TYPE_LPDDR4, .freq = 1866, .cbtMode = CBT_BYTE_MODE1, .readDBI = DBI_ON,
		.readLat = 40, .writeLat = 16,

		.tras = 11, .tras_05T = 1,
		.trp = 6, .trp_05T = 1,
		.trpab = 1, .trpab_05T = 1,
		.trc = 20, .trc_05T = 1,
		.trfc = 118, .trfc_05T = 1,
		.trfcpb = 53, .trfcpb_05T = 1,
		.txp = 1, .txp_05T = 1,
		.trtp = 2, .trtp_05T = 0,
		.trcd = 7, .trcd_05T = 1,
		.twr = 15, .twr_05T = 1,
		.twtr = 9, .twtr_05T = 0,
		.trrd = 3, .trrd_05T = 0,
		.tfaw = 10, .tfaw_05T = 0,
		.trtw_ODT_off = 8, .trtw_ODT_off_05T = 0,
		.trtw_ODT_on = 10, .trtw_ODT_on_05T = 0,
		.refcnt = 113,
		.refcnt_fr_clk = 101,
		.txrefcnt = 138,
		.tzqcs = 40,
		.xrtw2w = 5,
		.xrtw2r = 3,
		.xrtr2w = 9,
		.xrtr2r = 9,
		.r_dmcatrain_intv = 13,
		.r_dmmrw_intv = 0xf, /* Berson: LP3 / 4 both use this field -> Formula may change, set to 0xF for now */
		.r_dmfspchg_prdcnt = 117,
		.trtpd = 14, .trtpd_05T = 0,
		.twtpd = 14, .twtpd_05T = 0,
		.tmrr2w_ODT_off = 11,
		.tmrr2w_ODT_on = 13,
		.ckeprd = 3,
		.ckelckcnt = 3,
		.zqlat2 = 14,

		/* DQSINCTL, DATLAT aren't in ACTiming excel file */
		.dqsinctl = 7, .datlat = 18
	},

	/*
	 * AC_TIME_LP4_NORM_DDR3733_DBI_ON
	 * LP4-3733, 1866MHz, RDBI_ON, normal mode
	 */
	{
		.dramType = TYPE_LPDDR4, .freq = 1866, .cbtMode = CBT_NORMAL_MODE, .readDBI = DBI_ON,
		.readLat = 36, .writeLat = 16,

		.tras = 11, .tras_05T = 1,
		.trp = 6, .trp_05T = 1,
		.trpab = 1, .trpab_05T = 1,
		.trc = 20, .trc_05T = 1,
		.trfc = 118, .trfc_05T = 1,
		.trfcpb = 53, .trfcpb_05T = 1,
		.txp = 1, .txp_05T = 1,
		.trtp = 2, .trtp_05T = 0,
		.trcd = 7, .trcd_05T = 1,
		.twr = 14, .twr_05T = 1,
		.twtr = 8, .twtr_05T = 0,
		.trrd = 3, .trrd_05T = 0,
		.tfaw = 10, .tfaw_05T = 0,
		.trtw_ODT_off = 7, .trtw_ODT_off_05T = 0,
		.trtw_ODT_on = 9, .trtw_ODT_on_05T = 0,
		.refcnt = 113,
		.refcnt_fr_clk = 101,
		.txrefcnt = 138,
		.tzqcs = 40,
		.xrtw2w = 5,
		.xrtw2r = 3,
		.xrtr2w = 8,
		.xrtr2r = 9,
		.r_dmcatrain_intv = 13,
		.r_dmmrw_intv = 0xf, /* Berson: LP3 / 4 both use this field -> Formula may change, set to 0xF for now */
		.r_dmfspchg_prdcnt = 117,
		.trtpd = 13, .trtpd_05T = 0,
		.twtpd = 13, .twtpd_05T = 0,
		.tmrr2w_ODT_off = 10,
		.tmrr2w_ODT_on = 12,
		.ckeprd = 3,
		.ckelckcnt = 3,
		.zqlat2 = 14,

		/* DQSINCTL, DATLAT aren't in ACTiming excel file */
		.dqsinctl = 5, .datlat = 18
	},
#else /* ENABLE_READ_DBI == 0) */
	/*
	 * AC_TIME_LP4_BYTE_DDR3733_DBI_OFF
	 * LP4-3733, 1866MHz, RDBI_OFF, byte mode
	 */
	{
		.dramType = TYPE_LPDDR4, .freq = 1866, .cbtMode = CBT_BYTE_MODE1, .readDBI = DBI_OFF,
		.readLat = 36, .writeLat = 16,

		.tras = 11, .tras_05T = 1,
		.trp = 6, .trp_05T = 1,
		.trpab = 1, .trpab_05T = 1,
		.trc = 20, .trc_05T = 1,
		.trfc = 118, .trfc_05T = 1,
		.trfcpb = 53, .trfcpb_05T = 1,
		.txp = 1, .txp_05T = 1,
		.trtp = 2, .trtp_05T = 0,
		.trcd = 7, .trcd_05T = 1,
		.twr = 15, .twr_05T = 1,
		.twtr = 9, .twtr_05T = 0,
		.trrd = 3, .trrd_05T = 0,
		.tfaw = 10, .tfaw_05T = 0,
		.trtw_ODT_off = 7, .trtw_ODT_off_05T = 0,
		.trtw_ODT_on = 9, .trtw_ODT_on_05T = 0,
		.refcnt = 113,
		.refcnt_fr_clk = 101,
		.txrefcnt = 138,
		.tzqcs = 40,
		.xrtw2w = 5,
		.xrtw2r = 3,
		.xrtr2w = 8,
		.xrtr2r = 9,
		.r_dmcatrain_intv = 13,
		.r_dmmrw_intv = 0xf, /* Berson: LP3 / 4 both use this field -> Formula may change, set to 0xF for now */
		.r_dmfspchg_prdcnt = 117,
		.trtpd = 13, .trtpd_05T = 0,
		.twtpd = 14, .twtpd_05T = 0,
		.tmrr2w_ODT_off = 10,
		.tmrr2w_ODT_on = 12,
		.ckeprd = 3,
		.ckelckcnt = 3,
		.zqlat2 = 14,

		/* DQSINCTL, DATLAT aren't in ACTiming excel file */
		.dqsinctl = 7, .datlat = 17
	},

	/*
	 * AC_TIME_LP4_NORM_DDR3733_DBI_OFF
	 * LP4-3733, 1866MHz, RDBI_OFF, normal mode
	 */
	{
		.dramType = TYPE_LPDDR4, .freq = 1866, .cbtMode = CBT_NORMAL_MODE, .readDBI = DBI_OFF,
		.readLat = 32, .writeLat = 16,

		.tras = 11, .tras_05T = 1,
		.trp = 6, .trp_05T = 1,
		.trpab = 1, .trpab_05T = 1,
		.trc = 20, .trc_05T = 1,
		.trfc = 118, .trfc_05T = 1,
		.trfcpb = 53, .trfcpb_05T = 1,
		.txp = 1, .txp_05T = 1,
		.trtp = 2, .trtp_05T = 0,
		.trcd = 7, .trcd_05T = 1,
		.twr = 14, .twr_05T = 1,
		.twtr = 8, .twtr_05T = 0,
		.trrd = 3, .trrd_05T = 0,
		.tfaw = 10, .tfaw_05T = 0,
		.trtw_ODT_off = 6, .trtw_ODT_off_05T = 0,
		.trtw_ODT_on = 8, .trtw_ODT_on_05T = 0,
		.refcnt = 113,
		.refcnt_fr_clk = 101,
		.txrefcnt = 138,
		.tzqcs = 40,
		.xrtw2w = 5,
		.xrtw2r = 3,
		.xrtr2w = 7,
		.xrtr2r = 9,
		.r_dmcatrain_intv = 13,
		.r_dmmrw_intv = 0xf, /* Berson: LP3 / 4 both use this field -> Formula may change, set to 0xF for now */
		.r_dmfspchg_prdcnt = 117,
		.trtpd = 12, .trtpd_05T = 0,
		.twtpd = 13, .twtpd_05T = 0,
		.tmrr2w_ODT_off = 9,
		.tmrr2w_ODT_on = 11,
		.ckeprd = 3,
		.ckelckcnt = 3,
		.zqlat2 = 14,

		/* DQSINCTL, DATLAT aren't in ACTiming excel file */
		.dqsinctl = 6, .datlat = 18
	},
#endif /* (ENABLE_READ_DBI) */
#endif /* SUPPORT_LP4_DDR3733_ACTIM */

#if SUPPORT_LP4_DDR3200_ACTIM
	/* LP4_DDR3200 ACTiming--------------------------------- */
#if (ENABLE_READ_DBI == 1)
	/*
	 * AC_TIME_LP4_BYTE_DDR3200_DBI_ON
	 * LP4-3200, 1600MHz, RDBI_ON, byte mode
	 */
	{
		.dramType = TYPE_LPDDR4, .freq = 1600, .cbtMode = CBT_BYTE_MODE1, .readDBI = DBI_ON,
		.readLat = 36, .writeLat = 14,

		.tras = 8, .tras_05T = 1,
		.trp = 5, .trp_05T = 1,
		.trpab = 1, .trpab_05T = 0,
		.trc = 16, .trc_05T = 1,
		.trfc = 100, .trfc_05T = 0,
		.trfcpb = 44, .trfcpb_05T = 0,
		.txp = 1, .txp_05T = 0,
		.trtp = 2, .trtp_05T = 1,
		.trcd = 6, .trcd_05T = 1,
		.twr = 13, .twr_05T = 1,
		.twtr = 8, .twtr_05T = 0,
		.trrd = 2, .trrd_05T = 0,
		.tfaw = 7, .tfaw_05T = 0,
		.trtw_ODT_off = 7, .trtw_ODT_off_05T = 0,
		.trtw_ODT_on = 9, .trtw_ODT_on_05T = 0,
		.refcnt = 97,
		.refcnt_fr_clk = 101,
		.txrefcnt = 119,
		.tzqcs = 34,
		.xrtw2w = 5,
		.xrtw2r = 3,
		.xrtr2w = 8,
		.xrtr2r = 9,
		.r_dmcatrain_intv = 11,
		.r_dmmrw_intv = 0xf, /* Berson: LP3 / 4 both use this field -> Formula may change, set to 0xF for now */
		.r_dmfspchg_prdcnt = 100,
		.trtpd = 13, .trtpd_05T = 0,
		.twtpd = 12, .twtpd_05T = 0,
		.tmrr2w_ODT_off = 10,
		.tmrr2w_ODT_on = 12,
		.ckeprd = 2,
		.ckelckcnt = 3,
		.zqlat2 = 12,

		/* DQSINCTL, DATLAT aren't in ACTiming excel file */
		.dqsinctl = 5, .datlat = 16
	},

	/*
	 * AC_TIME_LP4_NORM_DDR3200_DBI_ON
	 * LP4-3200, 1600MHz, RDBI_ON, normal mode
	 */
	{
		.dramType = TYPE_LPDDR4, .freq = 1600, .cbtMode = CBT_NORMAL_MODE, .readDBI = DBI_ON,
		.readLat = 32, .writeLat = 14,

		.tras = 8, .tras_05T = 1,
		.trp = 5, .trp_05T = 1,
		.trpab = 1, .trpab_05T = 0,
		.trc = 16, .trc_05T = 1,
		.trfc = 100, .trfc_05T = 0,
		.trfcpb = 44, .trfcpb_05T = 0,
		.txp = 1, .txp_05T = 0,
		.trtp = 2, .trtp_05T = 1,
		.trcd = 6, .trcd_05T = 1,
		.twr = 12, .twr_05T = 1,
		.twtr = 7, .twtr_05T = 0,
		.trrd = 2, .trrd_05T = 0,
		.tfaw = 7, .tfaw_05T = 0,
		.trtw_ODT_off = 6, .trtw_ODT_off_05T = 0,
		.trtw_ODT_on = 8, .trtw_ODT_on_05T = 0,
		.refcnt = 97,
		.refcnt_fr_clk = 101,
		.txrefcnt = 119,
		.tzqcs = 34,
		.xrtw2w = 5,
		.xrtw2r = 3,
		.xrtr2w = 7,
		.xrtr2r = 9,
		.r_dmcatrain_intv = 11,
		.r_dmmrw_intv = 0xf, /* Berson: LP3 / 4 both use this field -> Formula may change, set to 0xF for now */
		.r_dmfspchg_prdcnt = 100,
		.trtpd = 12, .trtpd_05T = 0,
		.twtpd = 12, .twtpd_05T = 1,
		.tmrr2w_ODT_off = 9,
		.tmrr2w_ODT_on = 11,
		.ckeprd = 2,
		.ckelckcnt = 3,
		.zqlat2 = 12,

		/* DQSINCTL, DATLAT aren't in ACTiming excel file */
		.dqsinctl = 4, .datlat = 15
	},
#else /* ENABLE_READ_DBI == 0) */
	/*
	 * AC_TIME_LP4_BYTE_DDR3200_DBI_OFF
	 * LP4-3200, 1600MHz, RDBI_OFF, byte mode
	 */
	{
		.dramType = TYPE_LPDDR4, .freq = 1600, .cbtMode = CBT_BYTE_MODE1, .readDBI = DBI_OFF,
		.readLat = 32, .writeLat = 14,

		.tras = 8, .tras_05T = 1,
		.trp = 5, .trp_05T = 1,
		.trpab = 1, .trpab_05T = 0,
		.trc = 16, .trc_05T = 1,
		.trfc = 100, .trfc_05T = 0,
		.trfcpb = 44, .trfcpb_05T = 0,
		.txp = 1, .txp_05T = 0,
		.trtp = 2, .trtp_05T = 1,
		.trcd = 6, .trcd_05T = 1,
		.twr = 13, .twr_05T = 1,
		.twtr = 8, .twtr_05T = 0,
		.trrd = 2, .trrd_05T = 0,
		.tfaw = 7, .tfaw_05T = 0,
		.trtw_ODT_off = 6, .trtw_ODT_off_05T = 0,
		.trtw_ODT_on = 8, .trtw_ODT_on_05T = 0,
		.refcnt = 97,
		.refcnt_fr_clk = 101,
		.txrefcnt = 119,
		.tzqcs = 34,
		.xrtw2w = 5,
		.xrtw2r = 3,
		.xrtr2w = 7,
		.xrtr2r = 9,
		.r_dmcatrain_intv = 11,
		.r_dmmrw_intv = 0xf, /* Berson: LP3 / 4 both use this field -> Formula may change, set to 0xF for now */
		.r_dmfspchg_prdcnt = 100,
		.trtpd = 12, .trtpd_05T = 0,
		.twtpd = 12, .twtpd_05T = 0,
		.tmrr2w_ODT_off = 9,
		.tmrr2w_ODT_on = 11,
		.ckeprd = 2,
		.ckelckcnt = 3,
		.zqlat2 = 12,

		/* DQSINCTL, DATLAT aren't in ACTiming excel file */
		.dqsinctl = 5, .datlat = 16
	},

	/*
	 * AC_TIME_LP4_NORM_DDR3200_DBI_OFF
	 * LP4-3200, 1600MHz, RDBI_OFF, normal mode
	 */
	{
		.dramType = TYPE_LPDDR4, .freq = 1600, .cbtMode = CBT_NORMAL_MODE, .readDBI = DBI_OFF,
		.readLat = 28, .writeLat = 14,

		.tras = 8, .tras_05T = 1,
		.trp = 5, .trp_05T = 1,
		.trpab = 1, .trpab_05T = 0,
		.trc = 16, .trc_05T = 1,
		.trfc = 100, .trfc_05T = 0,
		.trfcpb = 44, .trfcpb_05T = 0,
		.txp = 1, .txp_05T = 0,
		.trtp = 2, .trtp_05T = 1,
		.trcd = 6, .trcd_05T = 1,
		.twr = 12, .twr_05T = 1,
		.twtr = 7, .twtr_05T = 0,
		.trrd = 2, .trrd_05T = 0,
		.tfaw = 7, .tfaw_05T = 0,
		.trtw_ODT_off = 5, .trtw_ODT_off_05T = 0,
		.trtw_ODT_on = 7, .trtw_ODT_on_05T = 0,
		.refcnt = 97,
		.refcnt_fr_clk = 101,
		.txrefcnt = 119,
		.tzqcs = 34,
		.xrtw2w = 5,
		.xrtw2r = 3,
		.xrtr2w = 6,
		.xrtr2r = 9,
		.r_dmcatrain_intv = 11,
		.r_dmmrw_intv = 0xf, /* Berson: LP3 / 4 both use this field -> Formula may change, set to 0xF for now */
		.r_dmfspchg_prdcnt = 100,
		.trtpd = 11, .trtpd_05T = 0,
		.twtpd = 12, .twtpd_05T = 1,
		.tmrr2w_ODT_off = 8,
		.tmrr2w_ODT_on = 10,
		.ckeprd = 2,
		.ckelckcnt = 3,
		.zqlat2 = 12,

		/* DQSINCTL, DATLAT aren't in ACTiming excel file */
		.dqsinctl = 4, .datlat = 15
	},
#endif /* (ENABLE_READ_DBI) */
#endif /* SUPPORT_LP4_DDR3200_ACTIM */

#if SUPPORT_LP4_DDR2667_ACTIM
	/* LP4_DDR2667 ACTiming--------------------------------- */
#if (ENABLE_READ_DBI == 1)
	/*
	 * AC_TIME_LP4_BYTE_DDR2667_DBI_ON
	 * LP4-2667, 1333MHz, RDBI_ON, byte mode
	 */
	{
		.dramType = TYPE_LPDDR4, .freq = 1333, .cbtMode = CBT_BYTE_MODE1, .readDBI = DBI_ON,
		.readLat = 30, .writeLat = 12,

		.tras = 5, .tras_05T = 1,
		.trp = 4, .trp_05T = 0,
		.trpab = 1, .trpab_05T = 0,
		.trc = 12, .trc_05T = 0,
		.trfc = 81, .trfc_05T = 1,
		.trfcpb = 35, .trfcpb_05T = 0,
		.txp = 0, .txp_05T = 1,
		.trtp = 1, .trtp_05T = 0,
		.trcd = 5, .trcd_05T = 0,
		.twr = 11, .twr_05T = 1,
		.twtr = 7, .twtr_05T = 1,
		.trrd = 1, .trrd_05T = 1,
		.tfaw = 4, .tfaw_05T = 1,
		.trtw_ODT_off = 6, .trtw_ODT_off_05T = 0,
		.trtw_ODT_on = 8, .trtw_ODT_on_05T = 0,
		.refcnt = 81,
		.refcnt_fr_clk = 101,
		.txrefcnt = 100,
		.tzqcs = 28,
		.xrtw2w = 5,
		.xrtw2r = 3,
		.xrtr2w = 7,
		.xrtr2r = 8,
		.r_dmcatrain_intv = 10,
		.r_dmmrw_intv = 0xf, /* Berson: LP3 / 4 both use this field -> Formula may change, set to 0xF for now */
		.r_dmfspchg_prdcnt = 84,
		.trtpd = 11, .trtpd_05T = 0,
		.twtpd = 11, .twtpd_05T = 1,
		.tmrr2w_ODT_off = 8,
		.tmrr2w_ODT_on = 10,
		.ckeprd = 2,
		.ckelckcnt = 3,
		.zqlat2 = 10,

		/* DQSINCTL, DATLAT aren't in ACTiming excel file */
		.dqsinctl = 4, .datlat = 15
	},

	/*
	 * AC_TIME_LP4_NORM_DDR2667_DBI_ON
	 * LP4-2667, 1333MHz, RDBI_ON, normal mode
	 */
	{
		.dramType = TYPE_LPDDR4, .freq = 1333, .cbtMode = CBT_NORMAL_MODE, .readDBI = DBI_ON,
		.readLat = 28, .writeLat = 12,

		.tras = 5, .tras_05T = 1,
		.trp = 4, .trp_05T = 0,
		.trpab = 1, .trpab_05T = 0,
		.trc = 12, .trc_05T = 0,
		.trfc = 81, .trfc_05T = 1,
		.trfcpb = 35, .trfcpb_05T = 0,
		.txp = 0, .txp_05T = 1,
		.trtp = 1, .trtp_05T = 0,
		.trcd = 5, .trcd_05T = 0,
		.twr = 10, .twr_05T = 0,
		.twtr = 6, .twtr_05T = 0,
		.trrd = 1, .trrd_05T = 1,
		.tfaw = 4, .tfaw_05T = 1,
		.trtw_ODT_off = 6, .trtw_ODT_off_05T = 0,
		.trtw_ODT_on = 8, .trtw_ODT_on_05T = 0,
		.refcnt = 81,
		.refcnt_fr_clk = 101,
		.txrefcnt = 100,
		.tzqcs = 28,
		.xrtw2w = 5,
		.xrtw2r = 3,
		.xrtr2w = 7,
		.xrtr2r = 8,
		.r_dmcatrain_intv = 10,
		.r_dmmrw_intv = 0xf, /* Berson: LP3 / 4 both use this field -> Formula may change, set to 0xF for now */
		.r_dmfspchg_prdcnt = 84,
		.trtpd = 10, .trtpd_05T = 0,
		.twtpd = 10, .twtpd_05T = 1,
		.tmrr2w_ODT_off = 7,
		.tmrr2w_ODT_on = 9,
		.ckeprd = 2,
		.ckelckcnt = 3,
		.zqlat2 = 10,

		/* DQSINCTL, DATLAT aren't in ACTiming excel file */
		.dqsinctl = 3, .datlat = 13
	},
#else /* ENABLE_READ_DBI == 0) */
	/*
	 * AC_TIME_LP4_BYTE_DDR2667_DBI_OFF
	 * LP4-2667, 1333MHz, RDBI_OFF, byte mode
	 */
	{
		.dramType = TYPE_LPDDR4, .freq = 1333, .cbtMode = CBT_BYTE_MODE1, .readDBI = DBI_OFF,
		.readLat = 26, .writeLat = 12,

		.tras = 5, .tras_05T = 1,
		.trp = 4, .trp_05T = 0,
		.trpab = 1, .trpab_05T = 0,
		.trc = 12, .trc_05T = 0,
		.trfc = 81, .trfc_05T = 1,
		.trfcpb = 35, .trfcpb_05T = 0,
		.txp = 0, .txp_05T = 1,
		.trtp = 1, .trtp_05T = 0,
		.trcd = 5, .trcd_05T = 0,
		.twr = 11, .twr_05T = 1,
		.twtr = 7, .twtr_05T = 1,
		.trrd = 1, .trrd_05T = 1,
		.tfaw = 4, .tfaw_05T = 1,
		.trtw_ODT_off = 5, .trtw_ODT_off_05T = 0,
		.trtw_ODT_on = 7, .trtw_ODT_on_05T = 0,
		.refcnt = 81,
		.refcnt_fr_clk = 101,
		.txrefcnt = 100,
		.tzqcs = 28,
		.xrtw2w = 5,
		.xrtw2r = 3,
		.xrtr2w = 6,
		.xrtr2r = 8,
		.r_dmcatrain_intv = 10,
		.r_dmmrw_intv = 0xf, /* Berson: LP3 / 4 both use this field -> Formula may change, set to 0xF for now */
		.r_dmfspchg_prdcnt = 84,
		.trtpd = 10, .trtpd_05T = 0,
		.twtpd = 11, .twtpd_05T = 1,
		.tmrr2w_ODT_off = 7,
		.tmrr2w_ODT_on = 9,
		.ckeprd = 2,
		.ckelckcnt = 3,
		.zqlat2 = 10,

		/* DQSINCTL, DATLAT aren't in ACTiming excel file */
		.dqsinctl = 4, .datlat = 14
	},

	/*
	 * AC_TIME_LP4_NORM_DDR2667_DBI_OFF
	 * LP4-2667, 1333MHz, RDBI_OFF, normal mode
	 */
	{
		.dramType = TYPE_LPDDR4, .freq = 1333, .cbtMode = CBT_NORMAL_MODE, .readDBI = DBI_OFF,
		.readLat = 24, .writeLat = 12,

		.tras = 5, .tras_05T = 1,
		.trp = 4, .trp_05T = 0,
		.trpab = 1, .trpab_05T = 0,
		.trc = 12, .trc_05T = 0,
		.trfc = 81, .trfc_05T = 1,
		.trfcpb = 35, .trfcpb_05T = 0,
		.txp = 0, .txp_05T = 1,
		.trtp = 1, .trtp_05T = 0,
		.trcd = 5, .trcd_05T = 0,
		.twr = 10, .twr_05T = 0,
		.twtr = 6, .twtr_05T = 0,
		.trrd = 1, .trrd_05T = 1,
		.tfaw = 4, .tfaw_05T = 1,
		.trtw_ODT_off = 5, .trtw_ODT_off_05T = 0,
		.trtw_ODT_on = 7, .trtw_ODT_on_05T = 0,
		.refcnt = 81,
		.refcnt_fr_clk = 101,
		.txrefcnt = 100,
		.tzqcs = 28,
		.xrtw2w = 5,
		.xrtw2r = 3,
		.xrtr2w = 6,
		.xrtr2r = 8,
		.r_dmcatrain_intv = 10,
		.r_dmmrw_intv = 0xf, /* Berson: LP3 / 4 both use this field -> Formula may change, set to 0xF for now */
		.r_dmfspchg_prdcnt = 84,
		.trtpd = 9, .trtpd_05T = 0,
		.twtpd = 10, .twtpd_05T = 1,
		.tmrr2w_ODT_off = 6,
		.tmrr2w_ODT_on = 8,
		.ckeprd = 2,
		.ckelckcnt = 3,
		.zqlat2 = 10,

		/* DQSINCTL, DATLAT aren't in ACTiming excel file */
		.dqsinctl = 3, .datlat = 13
	},
#endif /* (ENABLE_READ_DBI) */
#endif /* SUPPORT_LP4_DDR2667_ACTIM */

#if SUPPORT_LP4_DDR2400_ACTIM
	/*
	 * LP4_DDR2400 ACTiming---------------------------------
	 * AC_TIME_LP4_BYTE_DDR2400_DBI_OFF
	 * LP4-2400, 1200MHz, RDBI_OFF, byte mode
	 */
	{
		.dramType = TYPE_LPDDR4, .freq = 1200, .cbtMode = CBT_BYTE_MODE1, .readDBI = DBI_OFF,
		.readLat = 26, .writeLat = 12,

		.tras = 4, .tras_05T = 1,
		.trp = 3, .trp_05T = 1,
		.trpab = 1, .trpab_05T = 0,
		.trc = 10, .trc_05T = 0,
		.trfc = 72, .trfc_05T = 0,
		.trfcpb = 30, .trfcpb_05T = 0,
		.txp = 0, .txp_05T = 1,
		.trtp = 1, .trtp_05T = 0,
		.trcd = 4, .trcd_05T = 1,
		.twr = 10, .twr_05T = 0,
		.twtr = 6, .twtr_05T = 0,
		.trrd = 1, .trrd_05T = 0,
		.tfaw = 3, .tfaw_05T = 0,
		.trtw_ODT_off = 5, .trtw_ODT_off_05T = 0,
		.trtw_ODT_on = 7, .trtw_ODT_on_05T = 0,
		.refcnt = 73,
		.refcnt_fr_clk = 101,
		.txrefcnt = 91,
		.tzqcs = 25,
		.xrtw2w = 5,
		.xrtw2r = 3,
		.xrtr2w = 6,
		.xrtr2r = 8,
		.r_dmcatrain_intv = 9,
		.r_dmmrw_intv = 0xf, /* Berson: LP3 / 4 both use this field -> Formula may change, set to 0xF for now */
		.r_dmfspchg_prdcnt = 75,
		.trtpd = 10, .trtpd_05T = 0,
		.twtpd = 10, .twtpd_05T = 1,
		.tmrr2w_ODT_off = 7,
		.tmrr2w_ODT_on = 9,
		.ckeprd = 2,
		.ckelckcnt = 3,
		.zqlat2 = 9,

		/* DQSINCTL, DATLAT aren't in ACTiming excel file */
		.dqsinctl = 4, .datlat = 14
	},

	/*
	 * AC_TIME_LP4_NORM_DDR2400_DBI_OFF
	 * LP4-2400, 1200MHz, RDBI_OFF, normal mode
	 */
	{
		.dramType = TYPE_LPDDR4, .freq = 1200, .cbtMode = CBT_NORMAL_MODE, .readDBI = DBI_OFF,
		.readLat = 24, .writeLat = 12,

		.tras = 4, .tras_05T = 1,
		.trp = 3, .trp_05T = 1,
		.trpab = 1, .trpab_05T = 0,
		.trc = 10, .trc_05T = 0,
		.trfc = 72, .trfc_05T = 0,
		.trfcpb = 30, .trfcpb_05T = 0,
		.txp = 0, .txp_05T = 1,
		.trtp = 1, .trtp_05T = 0,
		.trcd = 4, .trcd_05T = 1,
		.twr = 10, .twr_05T = 1,
		.twtr = 6, .twtr_05T = 1,
		.trrd = 1, .trrd_05T = 0,
		.tfaw = 3, .tfaw_05T = 0,
		.trtw_ODT_off = 5, .trtw_ODT_off_05T = 0,
		.trtw_ODT_on = 7, .trtw_ODT_on_05T = 0,
		.refcnt = 73,
		.refcnt_fr_clk = 101,
		.txrefcnt = 91,
		.tzqcs = 25,
		.xrtw2w = 5,
		.xrtw2r = 3,
		.xrtr2w = 6,
		.xrtr2r = 8,
		.r_dmcatrain_intv = 9,
		.r_dmmrw_intv = 0xf, /* Berson: LP3 / 4 both use this field -> Formula may change, set to 0xF for now */
		.r_dmfspchg_prdcnt = 75,
		.trtpd = 9, .trtpd_05T = 0,
		.twtpd = 9, .twtpd_05T = 0,
		.tmrr2w_ODT_off = 6,
		.tmrr2w_ODT_on = 8,
		.ckeprd = 2,
		.ckelckcnt = 3,
		.zqlat2 = 9,

		/* DQSINCTL, DATLAT aren't in ACTiming excel file */
		.dqsinctl = 3, .datlat = 13
	},
#endif

#if SUPPORT_LP4_DDR1600_ACTIM
	/*
	 * LP4_DDR1600 ACTiming---------------------------------
	 * AC_TIME_LP4_BYTE_DDR1600_DBI_OFF
	 * LP4-1600, 800MHz, RDBI_OFF, byte mode
	 */
	{
		.dramType = TYPE_LPDDR4, .freq = 800, .cbtMode = CBT_BYTE_MODE1, .readDBI = DBI_OFF,
		.readLat = 16, .writeLat = 8,

		.tras = 0, .tras_05T = 0,
		.trp = 2, .trp_05T = 0,
		.trpab = 0, .trpab_05T = 1,
		.trc = 4, .trc_05T = 0,
		.trfc = 44, .trfc_05T = 0,
		.trfcpb = 16, .trfcpb_05T = 0,
		.txp = 0, .txp_05T = 0,
		.trtp = 1, .trtp_05T = 1,
		.trcd = 3, .trcd_05T = 0,
		.twr = 7, .twr_05T = 0,
		.twtr = 4, .twtr_05T = 0,
		.trrd = 0, .trrd_05T = 0,
		.tfaw = 0, .tfaw_05T = 0,
		.trtw_ODT_off = 3, .trtw_ODT_off_05T = 0,
		.trtw_ODT_on = 4, .trtw_ODT_on_05T = 0,
		.refcnt = 48,
		.refcnt_fr_clk = 101,
		.txrefcnt = 62,
		.tzqcs = 16,
		.xrtw2w = 5,
		.xrtw2r = 3,
		.xrtr2w = 3,
		.xrtr2r = 8,
		.r_dmcatrain_intv = 8,
		.r_dmmrw_intv = 0xf, /* Berson: LP3 / 4 both use this field -> Formula may change, set to 0xF for now */
		.r_dmfspchg_prdcnt = 50,
		.trtpd = 7, .trtpd_05T = 0,
		.twtpd = 7, .twtpd_05T = 1,
		.tmrr2w_ODT_off = 4,
		.tmrr2w_ODT_on = 6,
		.ckeprd = 1,
		.ckelckcnt = 3,
		.zqlat2 = 6,

		/* DQSINCTL, DATLAT aren't in ACTiming excel file */
		.dqsinctl = 2, .datlat = 11
	},

	/*
	 * AC_TIME_LP4_NORM_DDR1600_DBI_OFF
	 * LP4-1600, 800MHz, RDBI_OFF, normal mode
	 */
	{
		.dramType = TYPE_LPDDR4, .freq = 800, .cbtMode = CBT_NORMAL_MODE, .readDBI = DBI_OFF,
		.readLat = 14, .writeLat = 8,

		.tras = 0, .tras_05T = 0,
		.trp = 2, .trp_05T = 0,
		.trpab = 0, .trpab_05T = 1,
		.trc = 4, .trc_05T = 0,
		.trfc = 44, .trfc_05T = 0,
		.trfcpb = 16, .trfcpb_05T = 0,
		.txp = 0, .txp_05T = 0,
		.trtp = 1, .trtp_05T = 1,
		.trcd = 3, .trcd_05T = 0,
		.twr = 7, .twr_05T = 1,
		.twtr = 4, .twtr_05T = 1,
		.trrd = 0, .trrd_05T = 0,
		.tfaw = 0, .tfaw_05T = 0,
		.trtw_ODT_off = 3, .trtw_ODT_off_05T = 0,
		.trtw_ODT_on = 4, .trtw_ODT_on_05T = 0,
		.refcnt = 48,
		.refcnt_fr_clk = 101,
		.txrefcnt = 62,
		.tzqcs = 16,
		.xrtw2w = 5,
		.xrtw2r = 3,
		.xrtr2w = 3,
		.xrtr2r = 8,
		.r_dmcatrain_intv = 8,
		.r_dmmrw_intv = 0xf, /* Berson: LP3 / 4 both use this field -> Formula may change, set to 0xF for now */
		.r_dmfspchg_prdcnt = 50,
		.trtpd = 6, .trtpd_05T = 0,
		.twtpd = 6, .twtpd_05T = 0,
		.tmrr2w_ODT_off = 3,
		.tmrr2w_ODT_on = 5,
		.ckeprd = 1,
		.ckelckcnt = 3, /* LP3 doesn't use this field */
		.zqlat2 = 6, /* LP3 doesn't use this field */

		/* DQSINCTL, DATLAT aren't in ACTiming excel file */
		.dqsinctl = 1, .datlat = 10
	},
#endif /* SUPPORT_LP4_DDR1600_ACTIM */

#if ENABLE_LP3_SW
	/*
	 * ----------LPDDR3---------------------------
	 * LP3-1866, 933MHz
	 */
	{
		.dramType = TYPE_LPDDR3, .freq = 933, .cbtMode = CBT_NORMAL_MODE, .readDBI = DBI_OFF,
		.readLat = 14, .writeLat = 8,

		.tras = 11, .tras_05T = 0,
		.trp = 9, .trp_05T = 1,
		.trpab = 1, .trpab_05T = 1,
		.trc = 23, .trc_05T = 1,
		.trfc = 86, .trfc_05T = 0,
		.trfcpb = 30, .trfcpb_05T = 0,
		.txp = 0, .txp_05T = 1,
		.trtp = 2, .trtp_05T = 1,
		.trcd = 10, .trcd_05T = 1,
		.twr = 12, .twr_05T = 1,
		.twtr = 7, .twtr_05T = 0,
		.trrd = 3, .trrd_05T = 0,
		.tfaw = 14, .tfaw_05T = 1,
		.trtw_ODT_off = 6, .trtw_ODT_off_05T = 1,
		.trtw_ODT_on = 6, .trtw_ODT_on_05T = 1, /* LP3 doesn't have odt on / off option (copy ODT_off's value) */
		.refcnt = 112,
		.refcnt_fr_clk = 101,
		.txrefcnt = 101,
		.tzqcs = 41,
		.xrtw2w = 6,
		.xrtw2r = 2,
		.xrtr2w = 6,
		.xrtr2r = 10,
		.r_dmcatrain_intv = 0, /* LP3 doesn't use this field */
		.r_dmmrw_intv = 0xf, /* Berson: LP3 / 4 both use this field -> Formula may change, set to 0xF for now */
		.r_dmfspchg_prdcnt = 0, /* LP3 doesn't use this field */
		.trtpd = 10, .trtpd_05T = 0,
		.twtpd = 11, .twtpd_05T = 0,
		.tmrr2w_ODT_off = 0, /* LP3 doesn't use this field */
		.tmrr2w_ODT_on = 0, /* LP3 doesn't use this field */
		.ckeprd = 3,
		.ckelckcnt = 3, /* LP3 doesn't use this field */
		.zqlat2 = 0, /* LP3 doesn't use this field */

		/* DQSINCTL, DATLAT aren't in ACTiming excel file */
		.dqsinctl = 5, .datlat = 17
	},

	/* LP3-1600, 800MHz */
	{
		.dramType = TYPE_LPDDR3, .freq = 800, .cbtMode = CBT_NORMAL_MODE, .readDBI = DBI_OFF,
		.readLat = 12, .writeLat = 6,

		.tras = 8, .tras_05T = 0,
		.trp = 8, .trp_05T = 0,
		.trpab = 1, .trpab_05T = 0,
		.trc = 19, .trc_05T = 0,
		.trfc = 72, .trfc_05T = 0,
		.trfcpb = 24, .trfcpb_05T = 0,
		.txp = 0, .txp_05T = 0,
		.trtp = 1, .trtp_05T = 0,
		.trcd = 9, .trcd_05T = 0,
		.twr = 10, .twr_05T = 1,
		.twtr = 6, .twtr_05T = 1,
		.trrd = 2, .trrd_05T = 0,
		.tfaw = 11, .tfaw_05T = 0,
		.trtw_ODT_off = 5, .trtw_ODT_off_05T = 1,
		.trtw_ODT_on = 5, .trtw_ODT_on_05T = 1, /* LP3 doesn't have odt on / off option (copy ODT_off's value) */
		.refcnt = 96,
		.refcnt_fr_clk = 101,
		.txrefcnt = 86,
		.tzqcs = 35,
		.xrtw2w = 6,
		.xrtw2r = 2,
		.xrtr2w = 6,
		.xrtr2r = 10,
		.r_dmcatrain_intv = 0, /* LP3 doesn't use this field */
		.r_dmmrw_intv = 0xf, /* Berson: LP3 / 4 both use this field -> Formula may change, set to 0xF for now */
		.r_dmfspchg_prdcnt = 0, /* LP3 doesn't use this field */
		.trtpd = 8, .trtpd_05T = 0,
		.twtpd = 9, .twtpd_05T = 0,
		.tmrr2w_ODT_off = 0, /* LP3 doesn't use this field */
		.tmrr2w_ODT_on = 0, /* LP3 doesn't use this field */
		.ckeprd = 2,
		.ckelckcnt = 3, /* LP3 doesn't use this field */
		.zqlat2 = 0, /* LP3 doesn't use this field */

		/* DQSINCTL, DATLAT aren't in ACTiming excel file */
		.dqsinctl = 4, .datlat = 15
	},

	/* LP3-1333, 667MHz */
	{
		.dramType = TYPE_LPDDR3, .freq = 667, .cbtMode = CBT_NORMAL_MODE, .readDBI = DBI_OFF,
		.readLat = 10, .writeLat = 6,

		.tras = 5, .tras_05T = 0,
		.trp = 6, .trp_05T = 0,
		.trpab = 1, .trpab_05T = 0,
		.trc = 14, .trc_05T = 0,
		.trfc = 58, .trfc_05T = 0,
		.trfcpb = 18, .trfcpb_05T = 0,
		.txp = 0, .txp_05T = 1,
		.trtp = 1, .trtp_05T = 1,
		.trcd = 7, .trcd_05T = 0,
		.twr = 9, .twr_05T = 1,
		.twtr = 5, .twtr_05T = 0,
		.trrd = 1, .trrd_05T = 1,
		.tfaw = 8, .tfaw_05T = 0,
		.trtw_ODT_off = 4, .trtw_ODT_off_05T = 1,
		.trtw_ODT_on = 4, .trtw_ODT_on_05T = 1, /* LP3 doesn't have odt on / off option (copy ODT_off's value) */
		.refcnt = 80,
		.refcnt_fr_clk = 101,
		.txrefcnt = 72,
		.tzqcs = 29,
		.xrtw2w = 6,
		.xrtw2r = 2,
		.xrtr2w = 6,
		.xrtr2r = 9,
		.r_dmcatrain_intv = 0, /* LP3 doesn't use this field */
		.r_dmmrw_intv = 0xf, /* Berson: LP3 / 4 both use this field -> Formula may change, set to 0xF for now */
		.r_dmfspchg_prdcnt = 0, /* LP3 doesn't use this field */
		.trtpd = 7, .trtpd_05T = 0,
		.twtpd = 8, .twtpd_05T = 0,
		.tmrr2w_ODT_off = 0, /* LP3 doesn't use this field */
		.tmrr2w_ODT_on = 0, /* LP3 doesn't use this field */
		.ckeprd = 2,
		.ckelckcnt = 3, /* LP3 doesn't use this field */
		.zqlat2 = 0, /* LP3 doesn't use this field */

		/* DQSINCTL, DATLAT aren't in ACTiming excel file */
		.dqsinctl = 3, .datlat = 14
	},

	/* LP3-1200, 600MHz */
	{
		.dramType = TYPE_LPDDR3, .freq = 600, .cbtMode = CBT_NORMAL_MODE, .readDBI = DBI_OFF,
		.readLat = 10, .writeLat = 6,

		.tras = 4, .tras_05T = 0,
		.trp = 5, .trp_05T = 1,
		.trpab = 1, .trpab_05T = 0,
		.trc = 12, .trc_05T = 0,
		.trfc = 51, .trfc_05T = 0,
		.trfcpb = 15, .trfcpb_05T = 0,
		.txp = 0, .txp_05T = 1,
		.trtp = 1, .trtp_05T = 1,
		.trcd = 6, .trcd_05T = 1,
		.twr = 8, .twr_05T = 0,
		.twtr = 5, .twtr_05T = 0,
		.trrd = 1, .trrd_05T = 0,
		.tfaw = 6, .tfaw_05T = 0,
		.trtw_ODT_off = 4, .trtw_ODT_off_05T = 1,
		.trtw_ODT_on = 4, .trtw_ODT_on_05T = 1, /* LP3 doesn't have odt on / off option (copy ODT_off's value) */
		.refcnt = 72,
		.refcnt_fr_clk = 101,
		.txrefcnt = 64,
		.tzqcs = 26,
		.xrtw2w = 6,
		.xrtw2r = 2,
		.xrtr2w = 6,
		.xrtr2r = 9,
		.r_dmcatrain_intv = 0, /* LP3 doesn't use this field */
		.r_dmmrw_intv = 0xf, /* Berson: LP3 / 4 both use this field -> Formula may change, set to 0xF for now */
		.r_dmfspchg_prdcnt = 0, /* LP3 doesn't use this field */
		.trtpd = 7, .trtpd_05T = 0,
		.twtpd = 7, .twtpd_05T = 0,
		.tmrr2w_ODT_off = 0, /* LP3 doesn't use this field */
		.tmrr2w_ODT_on = 0, /* LP3 doesn't use this field */
		.ckeprd = 2,
		.ckelckcnt = 3, /* LP3 doesn't use this field */
		.zqlat2 = 0, /* LP3 doesn't use this field */

		/* DQSINCTL, DATLAT aren't in ACTiming excel file */
		.dqsinctl = 3, .datlat = 14
	},
#endif
};

#if RUNTIME_SHMOO_RELEATED_FUNCTION && SUPPORT_SAVE_TIME_FOR_CALIBRATION
void DramcRunTimeShmooRG_BackupRestore(DRAMC_CTX_T *p)
{
    U8 ii, channel_idx, channel_backup;
    channel_backup = vGetPHY2ChannelMapping(p);
    for(channel_idx=CHANNEL_A; channel_idx<p->support_channel_num; channel_idx++)
    {
        vSetPHY2ChannelMapping(p, channel_idx);
        U32 u4RegBackupAddress[RUNTIME_SHMOO_RG_BACKUP_NUM] =
        {
            (DRAMC_REG_ADDR(DRAMC_REG_SHU1RK0_PI)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU1RK1_PI)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_PI)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU2RK1_PI)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_PI)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU3RK1_PI)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU4RK0_PI)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU4RK1_PI)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHURK1_SELPH_DQ0)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQ0)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU2RK1_SELPH_DQ0)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQ0)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU3RK1_SELPH_DQ0)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU4RK0_SELPH_DQ0)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU4RK1_SELPH_DQ0)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ1)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHURK1_SELPH_DQ1)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQ1)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU2RK1_SELPH_DQ1)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQ1)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU3RK1_SELPH_DQ1)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU4RK0_SELPH_DQ1)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU4RK1_SELPH_DQ1)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ2)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHURK1_SELPH_DQ2)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQ2)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU2RK1_SELPH_DQ2)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQ2)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU3RK1_SELPH_DQ2)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU4RK0_SELPH_DQ2)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU4RK1_SELPH_DQ2)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ3)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHURK1_SELPH_DQ3)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_SELPH_DQ3)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU2RK1_SELPH_DQ3)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_SELPH_DQ3)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU3RK1_SELPH_DQ3)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU4RK0_SELPH_DQ3)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU4RK1_SELPH_DQ3)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU1RK0_DQS2DQ_CAL1)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU1RK1_DQS2DQ_CAL1)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_DQS2DQ_CAL1)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU2RK1_DQS2DQ_CAL1)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_DQS2DQ_CAL1)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU3RK1_DQS2DQ_CAL1)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU4RK0_DQS2DQ_CAL1)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU4RK1_DQS2DQ_CAL1)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU1RK0_DQS2DQ_CAL2)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU1RK1_DQS2DQ_CAL2)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_DQS2DQ_CAL2)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU2RK1_DQS2DQ_CAL2)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_DQS2DQ_CAL2)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU3RK1_DQS2DQ_CAL2)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU4RK0_DQS2DQ_CAL2)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU4RK1_DQS2DQ_CAL2)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU1RK0_DQS2DQ_CAL5)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU1RK1_DQS2DQ_CAL5)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU2RK0_DQS2DQ_CAL5)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU2RK1_DQS2DQ_CAL5)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU3RK0_DQS2DQ_CAL5)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU3RK1_DQS2DQ_CAL5)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU4RK0_DQS2DQ_CAL5)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU4RK1_DQS2DQ_CAL5)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ0)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R1_B0_DQ0)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ0)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R1_B1_DQ0)),
            (DRAMC_REG_ADDR(DDRPHY_SHU2_R0_B0_DQ0)),
            (DRAMC_REG_ADDR(DDRPHY_SHU2_R1_B0_DQ0)),
            (DRAMC_REG_ADDR(DDRPHY_SHU2_R0_B1_DQ0)),
            (DRAMC_REG_ADDR(DDRPHY_SHU2_R1_B1_DQ0)),
            (DRAMC_REG_ADDR(DDRPHY_SHU3_R0_B0_DQ0)),
            (DRAMC_REG_ADDR(DDRPHY_SHU3_R1_B0_DQ0)),
            (DRAMC_REG_ADDR(DDRPHY_SHU3_R0_B1_DQ0)),
            (DRAMC_REG_ADDR(DDRPHY_SHU3_R1_B1_DQ0)),
            (DRAMC_REG_ADDR(DDRPHY_SHU4_R0_B0_DQ0)),
            (DRAMC_REG_ADDR(DDRPHY_SHU4_R1_B0_DQ0)),
            (DRAMC_REG_ADDR(DDRPHY_SHU4_R0_B1_DQ0)),
            (DRAMC_REG_ADDR(DDRPHY_SHU4_R1_B1_DQ0)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ1)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R1_B0_DQ1)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ1)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R1_B1_DQ1)),
            (DRAMC_REG_ADDR(DDRPHY_SHU2_R0_B0_DQ1)),
            (DRAMC_REG_ADDR(DDRPHY_SHU2_R1_B0_DQ1)),
            (DRAMC_REG_ADDR(DDRPHY_SHU2_R0_B1_DQ1)),
            (DRAMC_REG_ADDR(DDRPHY_SHU2_R1_B1_DQ1)),
            (DRAMC_REG_ADDR(DDRPHY_SHU3_R0_B0_DQ1)),
            (DRAMC_REG_ADDR(DDRPHY_SHU3_R1_B0_DQ1)),
            (DRAMC_REG_ADDR(DDRPHY_SHU3_R0_B1_DQ1)),
            (DRAMC_REG_ADDR(DDRPHY_SHU3_R1_B1_DQ1)),
            (DRAMC_REG_ADDR(DDRPHY_SHU4_R0_B0_DQ1)),
            (DRAMC_REG_ADDR(DDRPHY_SHU4_R1_B0_DQ1)),
            (DRAMC_REG_ADDR(DDRPHY_SHU4_R0_B1_DQ1)),
            (DRAMC_REG_ADDR(DDRPHY_SHU4_R1_B1_DQ1)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ2)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R1_B0_DQ2)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ2)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R1_B1_DQ2)),
            (DRAMC_REG_ADDR(DDRPHY_SHU2_R0_B0_DQ2)),
            (DRAMC_REG_ADDR(DDRPHY_SHU2_R1_B0_DQ2)),
            (DRAMC_REG_ADDR(DDRPHY_SHU2_R0_B1_DQ2)),
            (DRAMC_REG_ADDR(DDRPHY_SHU2_R1_B1_DQ2)),
            (DRAMC_REG_ADDR(DDRPHY_SHU3_R0_B0_DQ2)),
            (DRAMC_REG_ADDR(DDRPHY_SHU3_R1_B0_DQ2)),
            (DRAMC_REG_ADDR(DDRPHY_SHU3_R0_B1_DQ2)),
            (DRAMC_REG_ADDR(DDRPHY_SHU3_R1_B1_DQ2)),
            (DRAMC_REG_ADDR(DDRPHY_SHU4_R0_B0_DQ2)),
            (DRAMC_REG_ADDR(DDRPHY_SHU4_R1_B0_DQ2)),
            (DRAMC_REG_ADDR(DDRPHY_SHU4_R0_B1_DQ2)),
            (DRAMC_REG_ADDR(DDRPHY_SHU4_R1_B1_DQ2)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ3)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R1_B0_DQ3)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ3)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R1_B1_DQ3)),
            (DRAMC_REG_ADDR(DDRPHY_SHU2_R0_B0_DQ3)),
            (DRAMC_REG_ADDR(DDRPHY_SHU2_R1_B0_DQ3)),
            (DRAMC_REG_ADDR(DDRPHY_SHU2_R0_B1_DQ3)),
            (DRAMC_REG_ADDR(DDRPHY_SHU2_R1_B1_DQ3)),
            (DRAMC_REG_ADDR(DDRPHY_SHU3_R0_B0_DQ3)),
            (DRAMC_REG_ADDR(DDRPHY_SHU3_R1_B0_DQ3)),
            (DRAMC_REG_ADDR(DDRPHY_SHU3_R0_B1_DQ3)),
            (DRAMC_REG_ADDR(DDRPHY_SHU3_R1_B1_DQ3)),
            (DRAMC_REG_ADDR(DDRPHY_SHU4_R0_B0_DQ3)),
            (DRAMC_REG_ADDR(DDRPHY_SHU4_R1_B0_DQ3)),
            (DRAMC_REG_ADDR(DDRPHY_SHU4_R0_B1_DQ3)),
            (DRAMC_REG_ADDR(DDRPHY_SHU4_R1_B1_DQ3)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ4)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R1_B0_DQ4)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ4)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R1_B1_DQ4)),
            (DRAMC_REG_ADDR(DDRPHY_SHU2_R0_B0_DQ4)),
            (DRAMC_REG_ADDR(DDRPHY_SHU2_R1_B0_DQ4)),
            (DRAMC_REG_ADDR(DDRPHY_SHU2_R0_B1_DQ4)),
            (DRAMC_REG_ADDR(DDRPHY_SHU2_R1_B1_DQ4)),
            (DRAMC_REG_ADDR(DDRPHY_SHU3_R0_B0_DQ4)),
            (DRAMC_REG_ADDR(DDRPHY_SHU3_R1_B0_DQ4)),
            (DRAMC_REG_ADDR(DDRPHY_SHU3_R0_B1_DQ4)),
            (DRAMC_REG_ADDR(DDRPHY_SHU3_R1_B1_DQ4)),
            (DRAMC_REG_ADDR(DDRPHY_SHU4_R0_B0_DQ4)),
            (DRAMC_REG_ADDR(DDRPHY_SHU4_R1_B0_DQ4)),
            (DRAMC_REG_ADDR(DDRPHY_SHU4_R0_B1_DQ4)),
            (DRAMC_REG_ADDR(DDRPHY_SHU4_R1_B1_DQ4)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ5)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R1_B0_DQ5)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ5)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R1_B1_DQ5)),
            (DRAMC_REG_ADDR(DDRPHY_SHU2_R0_B0_DQ5)),
            (DRAMC_REG_ADDR(DDRPHY_SHU2_R1_B0_DQ5)),
            (DRAMC_REG_ADDR(DDRPHY_SHU2_R0_B1_DQ5)),
            (DRAMC_REG_ADDR(DDRPHY_SHU2_R1_B1_DQ5)),
            (DRAMC_REG_ADDR(DDRPHY_SHU3_R0_B0_DQ5)),
            (DRAMC_REG_ADDR(DDRPHY_SHU3_R1_B0_DQ5)),
            (DRAMC_REG_ADDR(DDRPHY_SHU3_R0_B1_DQ5)),
            (DRAMC_REG_ADDR(DDRPHY_SHU3_R1_B1_DQ5)),
            (DRAMC_REG_ADDR(DDRPHY_SHU4_R0_B0_DQ5)),
            (DRAMC_REG_ADDR(DDRPHY_SHU4_R1_B0_DQ5)),
            (DRAMC_REG_ADDR(DDRPHY_SHU4_R0_B1_DQ5)),
            (DRAMC_REG_ADDR(DDRPHY_SHU4_R1_B1_DQ5)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ6)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R1_B0_DQ6)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ6)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R1_B1_DQ6)),
            (DRAMC_REG_ADDR(DDRPHY_SHU2_R0_B0_DQ6)),
            (DRAMC_REG_ADDR(DDRPHY_SHU2_R1_B0_DQ6)),
            (DRAMC_REG_ADDR(DDRPHY_SHU2_R0_B1_DQ6)),
            (DRAMC_REG_ADDR(DDRPHY_SHU2_R1_B1_DQ6)),
            (DRAMC_REG_ADDR(DDRPHY_SHU3_R0_B0_DQ6)),
            (DRAMC_REG_ADDR(DDRPHY_SHU3_R1_B0_DQ6)),
            (DRAMC_REG_ADDR(DDRPHY_SHU3_R0_B1_DQ6)),
            (DRAMC_REG_ADDR(DDRPHY_SHU3_R1_B1_DQ6)),
            (DRAMC_REG_ADDR(DDRPHY_SHU4_R0_B0_DQ6)),
            (DRAMC_REG_ADDR(DDRPHY_SHU4_R1_B0_DQ6)),
            (DRAMC_REG_ADDR(DDRPHY_SHU4_R0_B1_DQ6)),
            (DRAMC_REG_ADDR(DDRPHY_SHU4_R1_B1_DQ6)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R1_B0_DQ7)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ7)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R1_B1_DQ7)),
            (DRAMC_REG_ADDR(DDRPHY_SHU2_R0_B0_DQ7)),
            (DRAMC_REG_ADDR(DDRPHY_SHU2_R1_B0_DQ7)),
            (DRAMC_REG_ADDR(DDRPHY_SHU2_R0_B1_DQ7)),
            (DRAMC_REG_ADDR(DDRPHY_SHU2_R1_B1_DQ7)),
            (DRAMC_REG_ADDR(DDRPHY_SHU3_R0_B0_DQ7)),
            (DRAMC_REG_ADDR(DDRPHY_SHU3_R1_B0_DQ7)),
            (DRAMC_REG_ADDR(DDRPHY_SHU3_R0_B1_DQ7)),
            (DRAMC_REG_ADDR(DDRPHY_SHU3_R1_B1_DQ7)),
            (DRAMC_REG_ADDR(DDRPHY_SHU4_R0_B0_DQ7)),
            (DRAMC_REG_ADDR(DDRPHY_SHU4_R1_B0_DQ7)),
            (DRAMC_REG_ADDR(DDRPHY_SHU4_R0_B1_DQ7)),
            (DRAMC_REG_ADDR(DDRPHY_SHU4_R1_B1_DQ7)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R1_CA_CMD9)),
            (DRAMC_REG_ADDR(DDRPHY_SHU2_R0_CA_CMD9)),
            (DRAMC_REG_ADDR(DDRPHY_SHU2_R1_CA_CMD9)),
            (DRAMC_REG_ADDR(DDRPHY_SHU3_R0_CA_CMD9)),
            (DRAMC_REG_ADDR(DDRPHY_SHU3_R1_CA_CMD9)),
            (DRAMC_REG_ADDR(DDRPHY_SHU4_R0_CA_CMD9)),
            (DRAMC_REG_ADDR(DDRPHY_SHU4_R1_CA_CMD9)),
        };
        for (ii=0; ii<RUNTIME_SHMOO_RG_BACKUP_NUM; ii++)
        {
            if(u1IsLP4Family(p->dram_type) && (p->femmc_Ready==0))
            {
                p->pSavetimeData->u4TXRG_Backup[channel_idx][ii] = u4IO32Read4B(u4RegBackupAddress[ii]);
                mcSHOW_DBG_MSG3(("=== run time shmoo backup : *(U32*)(0x%x) = 0x%x\n",u4RegBackupAddress[ii],p->pSavetimeData->u4TXRG_Backup[channel_idx][ii]));
            }
            else
            {
                vIO32Write4B(u4RegBackupAddress[ii], p->pSavetimeData->u4TXRG_Backup[channel_idx][ii]);
                mcSHOW_DBG_MSG3(("=== run time shmoo restore: *(U32*)(0x%x) = 0x%x\n",u4RegBackupAddress[ii],p->pSavetimeData->u4TXRG_Backup[channel_idx][ii]));
            }
        }
    }
    vSetPHY2ChannelMapping(p, channel_backup);
    //DramcRegDump(p);//for run time Tx eye scan RG check
}
#endif
#if (fcFOR_CHIP_ID == fcTalbot)
#define EMI_APB_BASE    0x10219000
#endif
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
        else //dual rank
        {
            u4matype = (u4matypeR0 > u4matypeR1) ? u4matypeR1 : u4matypeR0; //get min value
        }

        for(shu_index = DRAM_DFS_SHUFFLE_1; shu_index < DRAM_DFS_SHUFFLE_MAX; shu_index++)
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_CONF0) + SHU_GRP_DRAMC_OFFSET*shu_index, u4matype, SHU_CONF0_MATYPE);
    }
    vSetPHY2ChannelMapping(p, eOriChannel);
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
    if(u1IsLP4Family(p->dram_type))
    {
        u4BaseR0 = (((u4Addr&~0x1ff)>>2) | (u4Addr&0xff)>>1) >> 4;// drop bit8, bit0
        u4Offset = len>>(1+5);//1:ch_a/b field, 5:0x20 bytes coverage per pattern
    }
    else
    {
        u4BaseR0 = u4Addr>>4;
        if(rksel_mode == TA2_RKSEL_XRT)
        {
            u4Offset = len>>4;//16B per pattern
        }
        else
        {
            u4Offset = (len>>4)>>1;//16B per pattern //len should be >>2 or test engine will time out
        }
    }
    u4BaseR1 = u4BaseR0;
    u4matypeR0 = ((u4IO32Read4B(EMI_APB_BASE)>>4)&0x3)+1;
    u4matypeR1 = ((u4IO32Read4B(EMI_APB_BASE)>>6)&0x3)+1;
    if(u4matypeR0 != u4matypeR1)//R0 R1 mix mode
    {
        (u4matypeR0>u4matypeR1) ? (u4BaseR0>>=1):(u4BaseR1>>=1);//set the correct start address, refer to mapping table
        u4Offset>>=1;//set min value
    }
    u4Offset = (u4Offset==0) ? 1 : u4Offset;  //halt if u4Offset = 0
    if (TA2_RKSEL_XRT == rksel_mode)
    {
        // In order to enhance XRT R2R/W2W probability, use TEST2_4_TESTAGENTRKSEL=3, PERFCTL0_RWOFOEN=0 mode
        uiRWOFOEN = 0;
        mcSHOW_DBG_MSG(("=== TA2 XRT R2R/W2W\n"));
    }
    else
    {
        uiRWOFOEN = 1;
        mcSHOW_DBG_MSG(("=== TA2 HW\n"));
    }
    //u4Offset = 0x400;
    for(u1ChannelIdx = 0; u1ChannelIdx<p->support_channel_num; u1ChannelIdx++)
    {
        p->channel = u1ChannelIdx;
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_1), u4BaseR0, TEST2_1_TEST2_BASE);//fill based on RG table for Rank 0
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_5), u4BaseR1, TEST2_5_TEST2_BASE_2);//fill based on RG table for Rank 1
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_2), u4Offset, TEST2_2_TEST2_OFF);//128 bits data length per offest
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), rksel_mode, TEST2_4_TESTAGENTRKSEL);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PERFCTL0), uiRWOFOEN, PERFCTL0_RWOFOEN);
    }
    p->channel = eOriChannel;
    TA2_Test_Run_Time_HW_Set_Column_Num(p);
    return;
}
#define TA2_PAT TEST_XTALK_PATTERN
void TA2_Test_Run_Time_Pat_Setting(DRAMC_CTX_T *p, U8 PatSwitch)
{
    static U8 u1Pat = TA2_PAT, u1loop = 1;
    U8 u1ChannelIdx = 0;
    DRAM_CHANNEL_T eOriChannel = p->channel;
    if (u1loop||PatSwitch==TA2_PAT_SWITCH_ON)
    {
        mcSHOW_DBG_MSG(("TA2 PAT: %s\n",(u1Pat==TEST_XTALK_PATTERN)?"XTALK":(u1Pat==TEST_AUDIO_PATTERN)?"AUDIO":"ISI"));
        for (u1ChannelIdx=CHANNEL_A; u1ChannelIdx<p->support_channel_num; u1ChannelIdx++)
        {
            p->channel = u1ChannelIdx;
            DramcEngine2SetPat(p, u1Pat, p->support_rank_num - 1, 0);
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
void TA2_Test_Run_Time_HW_Status(DRAMC_CTX_T * p)
{
    U8 u1ChannelIdx = 0;
    U8 u1RankIdx = 0;
    U32 u4loop_count = 0;
    U32 u4ErrRegField = 0;
    U32 u4ErrorValue = 0;
    static U32 err_count = 0;
    static U32 pass_count = 0;
    DRAM_CHANNEL_T eOriChannel = p->channel;
    for(u1ChannelIdx =0; u1ChannelIdx<p->support_channel_num; u1ChannelIdx++)
    {
        p->channel = u1ChannelIdx;
        u4ErrorValue = DramcEngine2Compare(p, TE_OP_WRITE_READ_CHECK);
        if (u4ErrorValue&0x3) //RK0 or RK1 test fail
        {
            mcSHOW_DBG_MSG(("=== HW channel(%d) u4ErrorValue: 0x%x, bit error: 0x%x\n",u1ChannelIdx,u4ErrorValue,u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CMP_ERR))));
#if defined(SLT)
            while(1);
#endif
        }
        for(u1RankIdx =0 ; u1RankIdx<p->support_rank_num; u1RankIdx++)
        {
            if(u4ErrorValue & (1<<u1RankIdx))
            {
                err_count++;
                mcSHOW_DBG_MSG(("HW channel(%d) Rank(%d), TA2 failed, pass_cnt:%d, err_cnt:%d\n", u1ChannelIdx, u1RankIdx, pass_count, err_count));
            }
            else
            {
                pass_count++;
                mcSHOW_DBG_MSG(("HW channel(%d) Rank(%d), TA2 pass, pass_cnt:%d, err_cnt:%d\n", u1ChannelIdx, u1RankIdx, pass_count, err_count));
            }
        }
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(0, TEST2_3_TEST2W) | P_Fld(0, TEST2_3_TEST2R) | P_Fld(0, TEST2_3_TEST1));
    }
    p->channel = eOriChannel;


    return;
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
#if MRW_CHECK_ONLY
void vPrintFinalModeRegisterSetting(DRAMC_CTX_T * p)
{
    U8 u1CHIdx, u1RankIdx, u1FSPIdx, u1MRIdx;
    U16 u2MRValue;
    mcSHOW_MRW_MSG(("\n\n\n==vPrintFinalModeRegisterSetting====\n"));
    for(u1CHIdx=0; u1CHIdx<CHANNEL_NUM; u1CHIdx++)
        for(u1RankIdx=0; u1RankIdx< RANK_MAX; u1RankIdx++)
            for(u1FSPIdx=0; u1FSPIdx<FSP_MAX; u1FSPIdx++)
            {
                for(u1MRIdx=0; u1MRIdx<MR_NUM; u1MRIdx++)
                {
                    u2MRValue = u2MRRecord[u1CHIdx][u1RankIdx][u1FSPIdx][u1MRIdx];
                    if(u2MRValue != 0xffff) //value changed
                    {
                        mcSHOW_MRW_MSG(("[MR Dump] CH%d Rank%d Fsp%d MR%d =0x%x\n", u1CHIdx, u1RankIdx, u1FSPIdx, u1MRIdx, u2MRValue));
                    }
                }
                mcSHOW_MRW_MSG(("\n"));
            }
}
#endif
#if defined(SLT) || (FT_DSIM_USED) || (__ETT__)
#if DUAL_FREQ_K
DRAM_PLL_FREQ_SEL_T currFreqSel=PLL_FREQ_SEL_MAX;
void DramcDFS(DRAMC_CTX_T *p, int iDoDMA)
{
    DRAM_PLL_FREQ_SEL_T targetFreqSel = p->freq_sel;
    //if(currFreqSel==p->freq_sel)
    //    return;
    mcSHOW_ERR_MSG(("[DramcDFS] currFreqSel=%d, target=%d\n", currFreqSel, targetFreqSel));
#ifdef IMPEDANCE_TRACKING_ENABLE
    vIO32WriteFldAlign_All(DRAMC_REG_IMPCAL, 0, IMPCAL_IMPCAL_HW);
#endif

#if ENABLE_DRS
    EnableDRS_EMI(0);
#endif

    ///TODO: DFS scenario
    if(targetFreqSel==LP4_HIGHEST_FREQSEL)
    {
        DramcDFSDirectJump(p, 0);
    }
    else if(targetFreqSel==LP4_MIDDLE_FREQSEL)
    {
        DramcDFSDirectJump(p, 1);
    }
    else if(targetFreqSel==LP4_DDR1600)
    {
        DramcDFSDirectJump(p, 2);
    }

    if(targetFreqSel==LP3_DDR1866)
    {
        DramcDFSDirectJump(p, 0);
    }
    else if(targetFreqSel==LP3_DDR1600)
    {
        DramcDFSDirectJump(p, 1);
    }
    else if(targetFreqSel==LP3_DDR1200)
    {
        DramcDFSDirectJump(p, 2);
    }
#if ENABLE_DRS
    EnableDRS_EMI(1);
#endif
#ifdef IMPEDANCE_TRACKING_ENABLE
    vIO32WriteFldAlign_All(DRAMC_REG_IMPCAL, 1, IMPCAL_IMPCAL_HW);
#endif
    currFreqSel= p->freq_sel;
}
#endif //DUAL_FREQ_K
static unsigned int su4DfsCnt=0;
static unsigned int su4DfsRWCnt=0;
#if DUAL_FREQ_K
void DFSTestProgram(DRAMC_CTX_T *p, int iDoDMA)
{
    UINT8 updown = FALSE; //0:down, 1:up

    DramcDFSTestCode(p);
    su4DfsRWCnt++;
    ///TODO: DVFS scenario
    if(u1IsLP4Family(p->dram_type))
    {
        switch(su4DfsCnt%4)
        {
            //highestFreq->2400->1600->2400->highestFreq
            case 0:
                updown = FALSE;
                DDRPhyFreqSel(p, LP4_MIDDLE_FREQSEL);
                break;
            case 1:
                updown = FALSE;
                DDRPhyFreqSel(p, LP4_LOWEST_FREQSEL);
                break;
            case 2:
                updown = TRUE;
                DDRPhyFreqSel(p, LP4_MIDDLE_FREQSEL);
                break;
            case 3:
                updown = TRUE;
                DDRPhyFreqSel(p, LP4_HIGHEST_FREQSEL);
                break;
            default:
                break;
        }
    }
    else
    {
        switch(su4DfsCnt%4)
        {   case 0:
                updown = FALSE;
                 DDRPhyFreqSel(p, gFreqTbl_LP3[DRAM_DFS_SHUFFLE_2].freq_sel/*LP3_DDR1600*/);
                break;
            case 1:
                updown = FALSE;
                DDRPhyFreqSel(p, gFreqTbl_LP3[DRAM_DFS_SHUFFLE_3].freq_sel);
                break;
            case 2:
                updown = TRUE;
                DDRPhyFreqSel(p, gFreqTbl_LP3[DRAM_DFS_SHUFFLE_2].freq_sel);
                break;
            case 3:
                updown = TRUE;
                DDRPhyFreqSel(p, gFreqTbl_LP3[DRAM_DFS_SHUFFLE_1].freq_sel);
                break;
        }
    }
    if(su4DfsCnt%10==0)
    {
        DramcDFSDirectJump_SwitchCtrlMode(p);
    }
#ifndef MT6768_FPGA
#if ENABLE_DVS
    if(updown)
    {
        must_print("Start Voltage ----------------------------------------------> Freq\n");
        SetDVSProgram(p);
    }
    else
        must_print("Start Freq ----------------------------------------------> Voltage\n");
#endif
    DramcDFS(p, iDoDMA);
    GetPhyPllFrequency(p);
#if ENABLE_DVS
    if(!updown)
    {
        SetDVSProgram(p);
        must_print("End Freq -----------------------------------------------> Voltage\n");
    }
    else
        must_print("End Voltage -----------------------------------------------> Freq\n");
#endif
#endif
    su4DfsCnt++;
}
#endif //DUAL_FREQ_K
#endif
U32 DFS_Freq_Meter_Check(DRAMC_CTX_T *p, U32 ret_value)
{
    static U8 first_time=1;
    U8 shu;
    U32 before_value, after_value;
    U32 value, value_5_percent, test_result=1;

    U32 LP3_freqmeter[3]={LP3_1866_freq_meter, LP3_1600_freq_meter, LP3_1200_freq_meter};  //Shuffle  0,1,2 1866/1600/1200
#if LP4_HIGHEST_DDR3733
    U32 LP4_freqmeter[3]={LP4_3733_freq_meter, LP4_2400_freq_meter, LP4_1600_freq_meter};   //Shuffle 0,1,2 3200/2400/1600
#else
    U32 LP4_freqmeter[3]={LP4_3200_freq_meter, LP4_2400_freq_meter, LP4_1600_freq_meter};   //Shuffle 0,1,2 3200/2400/1600
#endif
    shu = u4IO32ReadFldAlign(DRAMC_REG_SHUSTATUS, SHUSTATUS_SHUFFLE_LEVEL);
    if (first_time)
    {
        DDRPhyFMeter_Init();
        ret_value = DDRPhyFreqMeter();
        first_time = 0;
    }
    before_value = (ret_value>>16) & 0xffff;
    after_value = ret_value & 0xffff;
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
            mcSHOW_ERR_MSG(("Fail!! fmem_ck_bfe_dcm_ch0 FREQ=%d!=%d\n", before_value, value));
            mcSHOW_ERR_MSG(("Fail!! fmem_ck_aft_dcm_ch0 FREQ=%d!=%d\n", after_value, value));
            test_result = 0; /* fail */
        }
#endif
    }
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
    if (test_result==1)
    {
        if(!p->u1PhyPLLEn)
        {
            mcSHOW_ERR_MSG(("now is [CLRPLL]\n"));
        }
        else
        {
            mcSHOW_ERR_MSG(("now is [PHYPLL]\n"));
        }

        GetPhyPllFrequency(p);
        mcSHOW_ERR_MSG(("DFS and Freq Meter test Pass!!\n"));
    }
    else
    {
        mcSHOW_ERR_MSG(("DFS and Freq Meter test Fail!!\n"));
#if defined(SLT)
        while(1);
#endif
    }
    return test_result;
}
#if defined(SLT)
void SLT_Test_DFS_Freq_Meter_Memory_Test(DRAMC_CTX_T *p)
{   U8 ii;
    U32 test_result;
    for(ii=0; ii<3; ii++)
    {
        vDramCPUReadWriteTestAfterCalibration(p);
        EnableDramcPhyDCM(p, 0);
        test_result = DFS_Freq_Meter_Check(p, DDRPhyFreqMeter());
        EnableDramcPhyDCM(p, 1);
        if (test_result==0) break;
#if DUAL_FREQ_K
        DFSTestProgram(p, 0);
#endif
    }
    if (test_result==1)
    {
        mcSHOW_ERR_MSG(("DFS and Freq Meter test Pass!!\n"));
    }
    else
    {
        mcSHOW_ERR_MSG(("DFS and Freq Meter test Fail!!\n"));
    }
    DDRPhyFreqSel(p, gFreqTbl[DRAM_DFS_SHUFFLE_2].freq_sel);
    vSetVcoreByFreq(p);
#if DUAL_FREQ_K
    DramcDFS(p, 0);
#endif
}
#endif
//======================== Low_Power_Scenarios_Test Begin =====================================
#if (ENABLE_EMI_LPBK_TEST)
#if (fcFOR_CHIP_ID == fcTalbot)
//-- SPM side band RG
#define PCM_PWR_IO_EN              (0x1000602C)
#define DRAMC_DPY_CLK_SW_CON_SEL   (0x10006460)
#define DRAMC_DPY_CLK_SW_CON_SEL2  (0x10006470)
#define DRAMC_DPY_CLK_SW_CON2      (0x10006474)
#define SB_SPM_S1_MODE_CH          (0x10006468)
#define PDEF_SPM_POWER_ON_VAL0     (0x10006004)
#define PDEF_SPM_POWER_ON_VAL1     (0x10006008)
#define DVT_PCM_REG13_DATA         (0x10006134)
#define PDEF_DPY_PWR_CON           (0x1000631C)
#define PDEF_SPM_MAS_PAUSE_MASK_B  (0x10006408)
#define PDEF_SPM_MAS_PAUSE2_MASK_B (0x1000640C)
#define PDEF_SPM_PASR_DPD_0        (0x10006630)
#define PDEF_SPM_PASR_DPD_3        (0x1000663C)
#define PDEF_SPM_MAS_PAUSE_MASK_B  (0x10006408)
#define PDEF_SUBSYS_IDLE_STA       (0x10006170)
#define DPYA_PWR_CON               (0x10006320)
#define PDEF_ARMPLL_CLK_CON        (0x100062BC)
#define PDEF_SPM_PLL_CON           (0x1000644C)
#define PDEF_CLK_CFG_0_SET         (0x10000044)
#define PDEF_CLK_CFG_0_CLR         (0x10000048)
#define PDEF_CLK_CFG_UPDATE        (0x10000004)
#define PDEF_EMI_CONN              (0x10230068)
#define PDEF_CLK_CFG_10_SET        (0x102101A4)
//-- SPM RG Internal Defined
//-- VAL0
#define PDEF_SC_26M_CK_OFF_LSB       0
#define PDEF_SC_ARM_CK_OFF_LSB       1
#define PDEF_SC_MEM_CK_OFF_LSB       2
#define PDEF_SC_AXI_CK_OFF_LSB       3
#define PDEF_SC_DR_GATE_RETRY_EN_LSB 4
#define PDEF_SC_MD26M_CK_OFF_LSB     5
#define PDEF_SC_DPY_MODE_SW          6
#define PDEF_SC_DMSUS_OFF            7
#define PDEF_SC_DPY_2ND_DLL_EN       8
#define PDEF_SC_ARMPLLOUT_OFF_LSB    9
#define PDEF_SC_MAINPOUT_OFF_LSB     10
#define PDEF_SC_ARMBPLLOUT_OFF_LSB   11
#define PDEF_SC_DPY_DLL_EN           12
#define PDEF_SC_DPY_DLL_CK_EN        13
#define PDEF_SC_DPY_VREF_EN          14
#define PDEF_SC_PHYPLL_EN            15
#define PDEF_SC_DDRPHY_FB_CK_EN      16
#define PDEF_SC_ARMPLL_OFF_LSB       17
#define PDEF_SC_MAINPLL_OFF_LSB      18
#define PDEF_SC_ARMBPLL_OFF_LSB      19
#define PDEF_SC_CKSQ0_OFF_LSB        20
#define PDEF_SC_CKSQ1_OFF_LSB        21
#define PDEF_SC_DR_SHU_EN            22
#define PDEF_SC_ARMPLL_S_OFF_LSB     23
#define PDEF_SC_MAINPLL_S_OFF_LSB    24
#define PDEF_SC_ARMBPLL_S_OFF_LSB    25
#define PDEF_SC_PHYPLL_SHU_EN        26
#define PDEF_SC_PHYPLL2_SHU_EN       27
#define PDEF_SC_PHYPLL_MODE_SW       28
#define PDEF_SC_PHYPLL2_MODE_SW      29
#define PDEF_SC_DR_SHU_LEVEL0        30
#define PDEF_SC_DR_SHU_LEVEL1        31
//-- VAL1
#define PDEF_PWRAP_SLEEP_REQ_LSB         0
#define PDEF_EMI_CLK_OFF_REQ_LSB         1
#define PDEF_TOP_MAS_PAU_REQ_LSB         2
#define PDEF_SPM2CKSYS_MEM_CK_MUX_UPDATE 3
#define PDEF_PCM_CK_SEL0_LSB             4
#define PDEF_PCM_CK_SEL1_LSB             5
#define PDEF_MD32_WAKEUP_LSB             6
#define PDEF_FREQH_PAUSE_ARM_LSB         7
#define PDEF_SC_26M_CK_SEL_LSB           8
#define PDEF_PCM_TIMER_SET_LSB           9
#define PDEF_PCM_TIMER_CLR_LSB           10
#define PDEF_SRCVOLTEN_LSB               11
#define PDEF_CSYSPWRUPACK_LSB            12
#define PDEF_IM_SLEEP_ENABLE_LSB         13
#define PDEF_SRCCLKENO_LSB               14
#define PDEF_SYSRST_LSB                  15
#define PDEF_MD_APSRC_ACK_LSB            16
#define PDEF_CPU_SYS_TIMER_CLK_SEL       17
#define PDEF_SC_AXI_DCM_DIS_LSB          18
#define PDEF_FREQH_PAUSE_MAIN_LSB        19
#define PDEF_FREQH_PAUSE_MEM_LSB         20
#define PDEF_SRCCLKENO_1_LSB             21
#define PDEF_WDT_KICK_P_LSB              22
#define PDEF_TWAM_EVENT_CLR_LSB          23
#define PDEF_WAKEUP_EXT_W_SEL            24
#define PDEF_WAKEUP_EXT_R_SEL            25
#define PDEF_PMIC_IRQ_REQ_EN_LSB         26
#define PDEF_R7_FORCE_VTCXO_ON           27
#define PDEF_R7_FORCE_APSRC_ON           28
#define PDEF_R7_FORCE_INFRA_ON           29
#define PDEF_R7_FORCE_VRF18_ON           30
#define PDEF_SC_DR_SHORT_QUEUE           31
//-- EMI monitor BW related
#define EMI_MSEL     0x10219440
#define EMI_MSEL2    0x10219468
#define EMI_BMEN2    0x102194E8
#define EMI_BMRW0    0x102194F8
#define EMI_WSCT     0x10219428
#define EMI_BCNT     0x10219408
#define EMI_WACT     0x10219420
#define EMI_CONM     0x10219060
#define EMI_APB_BASE 0x10219000
//-- S0 related
//#define  INFRA_TOPAXI_PROTECTEN_1_SET     (0x100002A8)
#define  DPY_CH0_PROT_BIT_MASK            (0x1 << 0)
#define  SPM_SLEEP_BASE                   (0X10006000)
#define  DPY_CH0_PWR_CON                  (SPM_SLEEP_BASE + 0x31C)
#define  MOCMOS_PWR_RST_B                 (0x1 << 0)
#define  MOCMOS_PWR_ISO                   (0x1 << 1)
#define  MOCMOS_PWR_ON                    (0x1 << 2)
#define  MOCMOS_PWR_ON_2ND                (0x1 << 3)
#define  MOCMOS_DPY_CH0_SRAM_PDN          (0xF << 8)
#define  MOCMOS_DPY_CH0_SRAM_PDN_ACK      (0xF << 12)
#define  MOCMOS_PWR_CLK_DIS               (0x1 << 4)
#define  MOCMOS_DPY_CH0_SRAM_PDN_ACK_BIT0 (0x1 << 12)
#define  MOCMOS_DPY_CH0_SRAM_PDN_ACK_BIT1 (0x1 << 13)
#define  MOCMOS_DPY_CH0_SRAM_PDN_ACK_BIT2 (0x1 << 14)
#define  MOCMOS_DPY_CH0_SRAM_PDN_ACK_BIT3 (0x1 << 15)
#define  MOCMOS_DPY_CH0_PWR_STA_MASK      (0x1 << 2)
#define DRAMC_DPY_CLK_SW_CON (0x10006464)
#define POWERON_CONFIG_EN        (SPM_SLEEP_BASE + 0x000)
#define DIS_PWR_CON              (SPM_SLEEP_BASE + 0x30C)
#define PWR_STATUS               (SPM_SLEEP_BASE + 0x180)
#define PWR_STATUS_2ND           (SPM_SLEEP_BASE + 0x184)
#define DIS_SRAM_PDN_ACK_BIT0    (0x1 << 12)
#define DIS_PROT_BIT_MASK        ((0x1 << 1) | (0x1 << 2))
#define DIS_PROT_BIT_ACK_MASK    ((0x1 << 1) | (0x1 << 2))
//#define INFRA_TOPAXI_PROTECTEN   (INFRACFG_AO_BASE + 0x220)
#define INFRA_TOPAXI_PROTECTSTA1 (INFRACFG_AO_BASE + 0x228)
#define DIS_PWR_STA_MASK         (0x1 << 3)
#define SLT_LS0 1
#define SLT_LS1 0
int gSLT_LS = SLT_LS1; // test @ LS1
void EMI_LPBK_memory_test_start(DRAMC_CTX_T * p, U8 K_win);
void SLT_Low_Power_Scenarios_Test(DRAMC_CTX_T *p, U8 dfs_idx);

#if (!__ETT__)
static int Reg_Sync_Writel(U32 addr, unsigned int val)
{
	(*(volatile unsigned int *)(addr)) = val;
	//mcSHOW_DBG_MSG(("@@@ Write Reg:0x%X Value:0x%X\n", addr, val));
	__asm__ __volatile__ ("dsb" : : : "memory");
	return 0;
}

static unsigned int Reg_Readl(U32 addr)
{
	return (*(volatile unsigned int *)(addr));
}
#endif

void SLT_Func_Emi_Off(DRAMC_CTX_T *p)
{
    int i = 0;
    U32 u4value = 0;
    U32 u4SrAck = (u1IsLP4Family(p->dram_type)) ? 3 : 1;//LP4: CHA CHB; LP3: CHA
    Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL1, Reg_Readl(PDEF_SPM_POWER_ON_VAL1) | (1U << PDEF_EMI_CLK_OFF_REQ_LSB));
    for(i=2; i>0; i--)
    {
        mcDELAY_US(1);
        u4value = Reg_Readl(DRAMC_DPY_CLK_SW_CON);
        if(u4SrAck & (u4value >> 26))
        {
            break;
        }
    }
    if(i==0) //no issue Ack =>status = 1
    {
        mcSHOW_DBG_MSG(("!Wait no ack(Enter SR)[0x%X]\n", u4value));
        u4value = Reg_Readl(PDEF_SPM_POWER_ON_VAL1);
        Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL1, u4value & ~(1U << PDEF_EMI_CLK_OFF_REQ_LSB));//Set to 0
    }
    return;
}
void SLT_Func_Emi_On(DRAMC_CTX_T *p)
{
    int i = 0;
    U32 u4value = 0;
    U32 u4SrAck = (u1IsLP4Family(p->dram_type)) ? 3 : 1;//LP4: CHA CHB; LP3: CHA
    Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL1, Reg_Readl(PDEF_SPM_POWER_ON_VAL1) & ~(1U << PDEF_EMI_CLK_OFF_REQ_LSB));
    for(i=2; i>0; i--)
    {
        mcDELAY_US(1);
        u4value = Reg_Readl(DRAMC_DPY_CLK_SW_CON);
        if((u4SrAck & (u4value >> 26)) == 0)
        {
            break;
        }
    }
    if(i==0) //no issue Ack =>status = 1
    {
        mcSHOW_DBG_MSG(("!Wait no ack(Leave SR)[0x%X]\n", u4value));
    }
}
void SLT_FUNC_DDRPHY_DLL_OFF_INFRA_CG_EN_ENTER(DRAMC_CTX_T *p)
{
    U32 u4value = 0;
    //mcSHOW_DBG_MSG(("DVFS DEBUG CHA PHY 278:[%X]\n", *(volatile unsigned int *)(0x10228278)));
    //mcSHOW_DBG_MSG(("DVFS DEBUG CHC PHY 000:[%X]\n", *(volatile unsigned int *)(0x10228000)));
    //mcSHOW_DBG_MSG(("DVFS DEBUG CHC PHY 004:[%X]\n", *(volatile unsigned int *)(0x10228004)));
    /*TINFO="DRAM : pause freq hopping = 1"*/
    ///TODO: (Step 6) Pause frequency hopping; {no freq hopping}
    Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL1,  Reg_Readl(PDEF_SPM_POWER_ON_VAL1) | (1U << PDEF_FREQH_PAUSE_ARM_LSB));//Sync SPM code, close MPLL freq hopping
    //! mpll_pause
    Reg_Sync_Writel(PDEF_ARMPLL_CLK_CON, Reg_Readl(PDEF_ARMPLL_CLK_CON) | (1U << 23));
    ///TODO: {Lewis: switch to SPM low power mode}
    ///TODO: (Step 7) DDRPHY switch to SPM control
    /*TINFO="DRAM : set sc_dpy_mode_sw = 1"*/
    Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL0, Reg_Readl(PDEF_SPM_POWER_ON_VAL0) | (1U << PDEF_SC_DPY_MODE_SW));
    ///TODO: (Step 8) IO enter suspend{tie to specific value, SR state}
    /*TINFO="DRAM : set sc_dmsus_off = 1"*/
    Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL0, Reg_Readl(PDEF_SPM_POWER_ON_VAL0) | (1U << PDEF_SC_DMSUS_OFF));
#ifdef CHECK_CONF_FOR_DEBUG
    mcSHOW_DBG_MSG(("[Enter]CHECK_CONF_FOR_DEBUG\n"));
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_CLK_MEM_SEL);//Switch clk to 26MHz
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_W_CHG_MEM);
    mcDELAY_US(1);
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_W_CHG_MEM);
    u4value = Reg_Readl(PDEF_SPM_POWER_ON_VAL0);
    Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL0, u4value | (1U << PDEF_SC_DDRPHY_FB_CK_EN));//Lewis@20160418: Open FB clk before mem_ck is off for debugging
#endif
    ///TODO: (Step 9) CG enable of mem_ck{dcm clk off}
    /*TINFO="DRAM : set sc_mem_ck_off = 1"*/
    Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL0, Reg_Readl(PDEF_SPM_POWER_ON_VAL0) | (1U << PDEF_SC_MEM_CK_OFF_LSB));
    ///TODO: (Step 10) DLL disable
    /*TINFO="DRAM : set sc_dpy_2nd_dll_en = 0"*/
    Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL0, Reg_Readl(PDEF_SPM_POWER_ON_VAL0) & ~(1U << PDEF_SC_DPY_2ND_DLL_EN));
    /*TINFO="DRAM : set sc_dpy_dll_en = 0"*/
    Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL0, Reg_Readl(PDEF_SPM_POWER_ON_VAL0) & ~(1U << PDEF_SC_DPY_DLL_EN));
    ///TODO: (Step 11) All PI disable & CG enable{Aphy cg enable, Not do PI delay Calculation}
    /*TINFO="DRAM : set sc_dpy_dll_ck_en = 0"*/
    Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL0, Reg_Readl(PDEF_SPM_POWER_ON_VAL0) & ~(1U << PDEF_SC_DPY_DLL_CK_EN));
    ///TODO: (Step 12) VREF disable, MCK8X CG enable and  MIDPI disable{MIDPI: PI of DLL }
    /*TINFO="DRAM : set sc_dpy_vref_en = 0"*/
    Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL0,  Reg_Readl(PDEF_SPM_POWER_ON_VAL0) & ~(1U << PDEF_SC_DPY_VREF_EN));
    return;
}
void SLT_FUNC_DDRPHY_DLL_OFF_INFRA_CG_EN_EXIT(DRAMC_CTX_T *p)
{
    //VREF enable, MCK8X CG disable and  MIDPI enable
    ///TODO: (Step 1)  VREF enable MCK8X CG disable MIDPI enable
    Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL0, Reg_Readl(PDEF_SPM_POWER_ON_VAL0) | (1U << PDEF_SC_DPY_VREF_EN));
    //All PI enable & CG disable
    ///TODO: (Step 2)  All PI enable & CG disable
    Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL0, Reg_Readl(PDEF_SPM_POWER_ON_VAL0) | (1U << PDEF_SC_DPY_DLL_CK_EN));
#ifdef CHECK_CONF_FOR_DEBUG
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_CLK_MEM_SEL);//Switch clk to mem_ck
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_W_CHG_MEM);
    mcDELAY_US(1);
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_W_CHG_MEM);
#endif
    //set TOP feedback MCK to full frequency
    ///TODO: (Step 3)  set TOP feedback MCK to full frequency
    Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL0, Reg_Readl(PDEF_SPM_POWER_ON_VAL0) | (1U << PDEF_SC_DDRPHY_FB_CK_EN));
    //Master DLL enable
    ///TODO: (Step 4) 1st DLL enable
    Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL0, Reg_Readl(PDEF_SPM_POWER_ON_VAL0) | (1U << PDEF_SC_DPY_DLL_EN));
    //wait 70 MCK
    ///TODO: (Step 5) wait 150ns
    mcDELAY_US(1);
    //Slave DLL enable
    ///TODO: (Step 6)  2nd DLL enable
    Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL0, Reg_Readl(PDEF_SPM_POWER_ON_VAL0) | (1U << PDEF_SC_DPY_2ND_DLL_EN));
    //wait 90 MCK
    ///TODO: (Step 7)  wait 150ns
    mcDELAY_US(1);
    //DDRPHY switch to SW control
    ///TODO: {Lewis: switch to SPM normal mode}
    ///TODO: (Step 8)  DDRPHY switch to DRAMC control
    Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL0, Reg_Readl(PDEF_SPM_POWER_ON_VAL0) & ~(1U << PDEF_SC_DPY_MODE_SW));
    //mem_ck CG disable
    ///TODO: (Step 9)  mem_ck CG disable
    Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL0, Reg_Readl(PDEF_SPM_POWER_ON_VAL0) & ~(1U << PDEF_SC_MEM_CK_OFF_LSB));
    //set TOP feedback MCK to divided frequency
    ///TODO: (Step 10)  set TOP feedback MCK to divided frequency
    //Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL0, Reg_Readl(PDEF_SPM_POWER_ON_VAL0) & ~(1U << PDEF_SC_DDRPHY_FB_CK_EN));
    //IO exit suspend
    ///TODO: (Step 11)  wait 150ns
    mcDELAY_US(1);
    ///TODO: (Step 12)  IO exit suspend
    Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL0, Reg_Readl(PDEF_SPM_POWER_ON_VAL0) & ~(1U << PDEF_SC_DMSUS_OFF));
    Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL1, Reg_Readl(PDEF_SPM_POWER_ON_VAL1) & ~(1U << PDEF_FREQH_PAUSE_MEM_LSB));
    ///TODO: (Step 13)  release frequency hopping
    Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL1, Reg_Readl(PDEF_SPM_POWER_ON_VAL1) & ~(1U << PDEF_FREQH_PAUSE_ARM_LSB));//Sync SPM code, open MPLL freq hopping
    //enable MPLL freq hopping
    //Reg_Sync_Writel(PDEF_ARMPLL_CLK_CON, Reg_Readl(PDEF_ARMPLL_CLK_CON) & ~(1U << 23));
#ifdef TEST_DQS_RETRY_CAPABILITY
    Set_DQS_Gating_Value(p);//Minus 2 UI
#endif
    ///TODO: (Step 14)  release self-refresh request
    SLT_Func_Emi_On(p);
#if (fcFOR_CHIP_ID == fcTalbot)/* This WA only need to apply to M17 */
    //Set m17_toggle_mask to fix CQDMA bus hang since there is no DRAMC power down reset signal connected with INFRA
    //Refer to M17_toogle_mode_issue.pptx
    Reg_Sync_Writel(DRAMC_CH0_TOP3_BASE + 0x3FC, Reg_Readl(DRAMC_CH0_TOP3_BASE + 0x3FC) & ~0x1);
    Reg_Sync_Writel(DRAMC_CH1_TOP3_BASE + 0x3FC, Reg_Readl(DRAMC_CH1_TOP3_BASE + 0x3FC) & ~0x1);
#endif
    if(!u1IsLP4Family(p->dram_type))//LPDDR3
    {
        Reg_Sync_Writel(Channel_B_PHY_AO_BASE_ADDRESS + 0x284, Reg_Readl(Channel_B_PHY_AO_BASE_ADDRESS + 0x284) | (1U << 20));
    }
    if(u1IsLP4Family(p->dram_type))
    {
        //r0 = r0 & ~r1 << PDEF_SC_DR_GATE_RETRY_EN_LSB
        ///TODO: (Step 19)  set DQS gating retry disable
        //Lewis@20160408: Remove set DQS gating retry setting in low power scenario, since it has chance to retry all the time after SR without dummy read
        Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL0, Reg_Readl(PDEF_SPM_POWER_ON_VAL0) & ~(1U << PDEF_SC_DR_GATE_RETRY_EN_LSB));
    }
    //Set TOP feedback MCK to divided frequency
    Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL0, Reg_Readl(PDEF_SPM_POWER_ON_VAL0) & ~(1U << PDEF_SC_DDRPHY_FB_CK_EN));
    ///TODO: (Step 20 21 22)
#if TDQSCK_PRECALCULATION_FOR_DVFS
    //Joe@20171030: In 6771, If use SPM_DRAMC_DPY_CLK_SW_CON2, you need to set SPM_DRAMC_DPY_CLK_SW_CON_SEL2 to 0.So we use VAL0
    Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL0, Reg_Readl(PDEF_SPM_POWER_ON_VAL0) | (1 << 23));
    //wait 80ns
    mcDELAY_US(1);
    Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL0, Reg_Readl(PDEF_SPM_POWER_ON_VAL0) & ~(1 << 23));
#endif
    return;
}
void SLT_Set_Spm_Poweron_Config_En()
{
    Reg_Sync_Writel(POWERON_CONFIG_EN, 0x0B160001);//PROJECT CODE should be equal to 0xB16
}
void SLT_Set_Spm_Side_Band_Pinmux(DRAMC_CTX_T *p)
{
    U32 u4value = 0;
    Reg_Sync_Writel(PCM_PWR_IO_EN, 0);
    u4value = (u1IsLP4Family(p->dram_type)) ? (0x00ffffff) : (0xaaffffff);
    Reg_Sync_Writel(DRAMC_DPY_CLK_SW_CON_SEL, u4value);
    Reg_Sync_Writel(DRAMC_DPY_CLK_SW_CON_SEL2, 0xffffffff);
    u4value = Reg_Readl(PDEF_SPM_POWER_ON_VAL0);
    u4value |= ((0x1 << 8) | (0x1 << 12) | (0x1 << 13) | (0x1 << 14) | (0x1 << 15));
    Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL0, u4value);
    /*TINFO="DRAM : set tx tracking disable  = 1"*/
    u4value |= (0x1 << 11) ;
    Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL0, u4value);
    mcDELAY_US(12);
    //u4value = Reg_Readl(SB_SPM_S1_MODE_CH);
    //u4value |= 0x3;
    //Reg_Sync_Writel(SB_SPM_S1_MODE_CH, u4value);
}
void SLT_Low_Power_Scenarios_Init(DRAMC_CTX_T *p)
{
    SLT_Set_Spm_Poweron_Config_En();
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
//#define IGNORE_MTCMOS_CHECK
int SLT_spm_mtcmos_ctrl_dpy_ch0_apmcu(int state)
{
    int err = 0;

    if (state == 0)
    {   //! Power Down
        /* TINFO="Set bus protect" */
        //!mazar   spm_write_apmcu( INFRA_TOPAXI_PROTECTEN_1_SET,  DPY_CH0_PROT_BIT_MASK);
        //Reg_Sync_Writel(INFRA_TOPAXI_PROTECTEN_1_SET,  DPY_CH0_PROT_BIT_MASK);
#ifndef IGNORE_MTCMOS_CHECK
        //!     while ((spm_read_apmcu( INFRA_TOPAXI_PROTECTSTA1_1) &  DPY_CH0_PROT_BIT_ACK_MASK) !=  DPY_CH0_PROT_BIT_ACK_MASK) {
        //!     }
#endif
        /* TINFO="Set SRAM_PDN = 1" */
        Reg_Sync_Writel(DPY_CH0_PWR_CON, Reg_Readl(DPY_CH0_PWR_CON) | (MOCMOS_DPY_CH0_SRAM_PDN));
#ifndef IGNORE_MTCMOS_CHECK
        /* TINFO="Wait until  DPY_CH0_SRAM_PDN_ACK = 1" */
        //mcSHOW_DBG_MSG(("Wait 0X1000631C[12] until  DPY_CH0_SRAM_PDN_ACK = 1\n"));
        while ((Reg_Readl(DPY_CH0_PWR_CON) & MOCMOS_DPY_CH0_SRAM_PDN_ACK) !=  MOCMOS_DPY_CH0_SRAM_PDN_ACK)
        {
        }
#endif
#if !ENABLE_EMI_LPBK_TEST
        /* TINFO="Set  PWR_ISO = 1" */
        Reg_Sync_Writel(DPY_CH0_PWR_CON, Reg_Readl(DPY_CH0_PWR_CON) | MOCMOS_PWR_ISO);
        /* TINFO="Set  PWR_CLK_DIS = 1" */
        Reg_Sync_Writel(DPY_CH0_PWR_CON, Reg_Readl(DPY_CH0_PWR_CON) | MOCMOS_PWR_CLK_DIS);
        /* TINFO="Set  PWR_RST_B = 0" */
        Reg_Sync_Writel(DPY_CH0_PWR_CON, Reg_Readl(DPY_CH0_PWR_CON) & ~MOCMOS_PWR_RST_B);
		/* TINFO="Set  PWR_ON = 0" */
	    Reg_Sync_Writel(DPY_CH0_PWR_CON, Reg_Readl(DPY_CH0_PWR_CON) & ~MOCMOS_PWR_ON);
        /* TINFO="Set  PWR_ON_2ND = 0" */
        Reg_Sync_Writel(DPY_CH0_PWR_CON, Reg_Readl(DPY_CH0_PWR_CON) & ~MOCMOS_PWR_ON_2ND);
        //mcSHOW_DBG_MSG(("Wait until 0X10006180[2] PWR_STATUS = 0 and  0X10006184[2] PWR_STATUS_2ND = 0\n"));
        while((Reg_Readl(PWR_STATUS) & MOCMOS_DPY_CH0_PWR_STA_MASK) || (Reg_Readl(PWR_STATUS_2ND) & MOCMOS_DPY_CH0_PWR_STA_MASK))
        {
        }
#endif
    }
    else
    {   /* STA_POWER_ON  1 */
        /* TINFO="Set  PWR_ON = 1" */
        Reg_Sync_Writel(DPY_CH0_PWR_CON, Reg_Readl(DPY_CH0_PWR_CON) | MOCMOS_PWR_ON);
        /* TINFO="Set  PWR_ON_2ND = 1" */
        Reg_Sync_Writel( DPY_CH0_PWR_CON, Reg_Readl(DPY_CH0_PWR_CON) | MOCMOS_PWR_ON_2ND);
#ifndef IGNORE_MTCMOS_CHECK
        /* TINFO="Wait until  PWR_STATUS = 1 and  PWR_STATUS_2ND = 1" */
        //mcSHOW_DBG_MSG(("Wait until 0X10006180[2] PWR_STATUS = 1 and  0X10006184[2] PWR_STATUS_2ND = 1\n"));
        while (((Reg_Readl(PWR_STATUS) &  MOCMOS_DPY_CH0_PWR_STA_MASK) !=  MOCMOS_DPY_CH0_PWR_STA_MASK)
                || ((Reg_Readl(PWR_STATUS_2ND) &  MOCMOS_DPY_CH0_PWR_STA_MASK) !=  MOCMOS_DPY_CH0_PWR_STA_MASK))
        {
        }
#endif
        /* TINFO="Set  PWR_CLK_DIS = 0" */
        Reg_Sync_Writel(DPY_CH0_PWR_CON, Reg_Readl(DPY_CH0_PWR_CON) & ~MOCMOS_PWR_CLK_DIS);
        /* TINFO="Set  PWR_ISO = 0" */
        Reg_Sync_Writel(DPY_CH0_PWR_CON, Reg_Readl(DPY_CH0_PWR_CON) & ~MOCMOS_PWR_ISO);
        /* TINFO="Set  PWR_RST_B = 1" */
        Reg_Sync_Writel(DPY_CH0_PWR_CON, Reg_Readl(DPY_CH0_PWR_CON) | MOCMOS_PWR_RST_B);
        /* TINFO="Set SRAM_PDN = 0" */
        Reg_Sync_Writel(DPY_CH0_PWR_CON, Reg_Readl(DPY_CH0_PWR_CON) & ~(0x1 << 8));
#ifndef IGNORE_MTCMOS_CHECK
        /* TINFO="Wait until  DPY_CH0_SRAM_PDN_ACK_BIT0 = 0" */
        //mcSHOW_DBG_MSG(("Wait until 0X1000631C[12] DPY_CH0_SRAM_PDN_ACK_BIT0 = 0\n"));
        while (Reg_Readl(DPY_CH0_PWR_CON) & MOCMOS_DPY_CH0_SRAM_PDN_ACK_BIT0)
        {
        }
#endif
        Reg_Sync_Writel(DPY_CH0_PWR_CON, Reg_Readl(DPY_CH0_PWR_CON) & ~(0x1 << 9));
#ifndef IGNORE_MTCMOS_CHECK
        /* TINFO="Wait until  DPY_CH0_SRAM_PDN_ACK_BIT0 = 0" */
        //mcSHOW_DBG_MSG(("Wait until 0X1000631C[13] DPY_CH0_SRAM_PDN_ACK_BIT0 = 0\n"));
        while (Reg_Readl(DPY_CH0_PWR_CON) & MOCMOS_DPY_CH0_SRAM_PDN_ACK_BIT1)
        {
        }
#endif
        Reg_Sync_Writel(DPY_CH0_PWR_CON, Reg_Readl(DPY_CH0_PWR_CON) & ~(0x1 << 10));
#ifndef IGNORE_MTCMOS_CHECK
        /* TINFO="Wait until  DPY_CH0_SRAM_PDN_ACK_BIT0 = 0" */
        //mcSHOW_DBG_MSG(("Wait until 0X1000631C[14] DPY_CH0_SRAM_PDN_ACK_BIT0 = 0\n"));
        while (Reg_Readl(DPY_CH0_PWR_CON) & MOCMOS_DPY_CH0_SRAM_PDN_ACK_BIT2)
        {
        }
#endif
        Reg_Sync_Writel(DPY_CH0_PWR_CON, Reg_Readl(DPY_CH0_PWR_CON) & ~(0x1 << 11));
#ifndef IGNORE_MTCMOS_CHECK
        /* TINFO="Wait until  DPY_CH0_SRAM_PDN_ACK_BIT0 = 0" */
        //mcSHOW_DBG_MSG(("Wait until 0X1000631C[15] DPY_CH0_SRAM_PDN_ACK_BIT0 = 0\n"));
        while (Reg_Readl(DPY_CH0_PWR_CON) & MOCMOS_DPY_CH0_SRAM_PDN_ACK_BIT3)
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
    ///TODO: (Step 2) PHY PLL off {PHYPLL && CLRPLL}
    /* HW will use this bit AND normal path signal(PHYPLL2_MODE_SW_PCM, PHYPLL_MODE_SW_PCM) to decide which PLL is on/off */
    Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL0, Reg_Readl(PDEF_SPM_POWER_ON_VAL0) & ~(1U << PDEF_SC_PHYPLL_EN));
    /*TINFO="DRAM : set sc_mpll_off = 1"*/
    Reg_Sync_Writel(PDEF_SPM_PLL_CON, Reg_Readl(PDEF_SPM_POWER_ON_VAL0) | (0x1 << 24));
    /*TINFO="DRAM : set sc_mpll_s_off = 1"*/
    Reg_Sync_Writel(PDEF_SPM_PLL_CON, Reg_Readl(PDEF_SPM_POWER_ON_VAL0) | (0x1 << 18));
    /*TINFO="DRAM : DPY power down, DRAMC_DPY_CLK_SW_CON_SEL2[30:28] = 3'b111, means turn off CHA, BCD will also turn off  "*/
    SLT_spm_mtcmos_ctrl_dpy_ch0_apmcu(0);
    return;
}
void SLT_FUNC_DDRPHY_INFRA_MTCMOS_OFF_EXIT(DRAMC_CTX_T *p)
{
    SLT_spm_mtcmos_ctrl_dpy_ch0_apmcu(1);
    /*TINFO="DRAM : set sc_mpll_off = 0"*/
    Reg_Sync_Writel(PDEF_SPM_PLL_CON, Reg_Readl(PDEF_SPM_PLL_CON) & ~(1U << 18));
    /*TINFO="DRAM : set sc_mpll_s_off = 0"*/
    Reg_Sync_Writel(PDEF_SPM_PLL_CON, Reg_Readl(PDEF_SPM_PLL_CON) & ~(1U << 24));
    ///TODO: (Step 8) wait 20 us
    mcDELAY_US(20);
    ///TODO: (Step 9-1 ~ 9-7)
    //Joe@20171030: In 6771, If use SPM_DRAMC_DPY_CLK_SW_CON2, you need to set SPM_DRAMC_DPY_CLK_SW_CON_SEL2 to 0.So we use VAL0
    /*TINFO="DRAM : set sc_dpy_bclk_en = 1"*/
    Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL0, Reg_Readl(PDEF_SPM_POWER_ON_VAL0) | (0x1 << 17));
    mcDELAY_US(1);
    /*TINFO="DRAM : set sc_shu_restore_en = 1"*/
    Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL0, Reg_Readl(PDEF_SPM_POWER_ON_VAL0) | (0x1 << 19));
    //wait 80ns
    mcDELAY_US(1);
    /*TINFO="DRAM : set sc_shu_restore_en = 0"*/
    Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL0, Reg_Readl(PDEF_SPM_POWER_ON_VAL0) & ~(0x1 << 19));
    //wait 195ns
    mcDELAY_US(1);
    /*TINFO="DRAM : set sc_dpy_bclk_en = 0"*/
    Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL0, Reg_Readl(PDEF_SPM_POWER_ON_VAL0) & ~(0x1 << 17));
    //PHYPLL on
    ///TODO: (Step 10) PHYPLL on{PHYPLL  || CLRPLL}
    /* HW will use this bit AND normal path signal(PHYPLL2_MODE_SW_PCM, PHYPLL_MODE_SW_PCM) to decide which PLL is on/off */
    //Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL0, Reg_Readl(PDEF_SPM_POWER_ON_VAL0) | (0x1 << 28));
    //Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL0, Reg_Readl(PDEF_SPM_POWER_ON_VAL0) | (0x1 << 29));
    Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL0, Reg_Readl(PDEF_SPM_POWER_ON_VAL0) | (0x1 << PDEF_SC_PHYPLL_EN));
    //wait 20us
    ///TODO: (Step 11) wait 20us
    mcDELAY_US(20);

    return;
}
void SLT_Func_Apsrc_Sleep (DRAMC_CTX_T *p, int intoS0)
{
    mcSHOW_DBG_MSG(("[Start]Func_Apsrc_Sleep\n"));
    ///TODO: [CPU off](Step 0 -1) Disable DQSOSC calibration
    //Disable_DQSOSC();
    ///TODO: [CPU off](Step 0 -2) wait 12us
    mcDELAY_US(12);
    if(!u1IsLP4Family(p->dram_type))//LPDDR3
    {
        Reg_Sync_Writel(Channel_B_PHY_AO_BASE_ADDRESS + 0x284, Reg_Readl(Channel_B_PHY_AO_BASE_ADDRESS + 0x284) & ~(1U << 20));
    }
#if (fcFOR_CHIP_ID == fcTalbot)/* This WA only need to apply to M17 */
    //Set m17_toggle_mask to fix CQDMA bus hang since there is no DRAMC power down reset signal connected with INFRA
    //Refer to M17_toogle_mode_issue.pptx
    Reg_Sync_Writel(DRAMC_CH0_TOP3_BASE + 0x3FC, Reg_Readl(DRAMC_CH0_TOP3_BASE + 0x3FC) | 0x1);
    Reg_Sync_Writel(DRAMC_CH1_TOP3_BASE + 0x3FC, Reg_Readl(DRAMC_CH1_TOP3_BASE + 0x3FC) | 0x1);
#endif
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
//Update MR4 value to NAO while exit S0
void SLT_SW_MR4(DRAMC_CTX_T *p)
{
    U32 u4value_24 = 0;
    U32 u4value_64 = 0;
    U32 u4value_38 = 0;
    U8 u1ChannelIdx = CHANNEL_A;
    for (u1ChannelIdx = CHANNEL_A; u1ChannelIdx< p->support_channel_num; u1ChannelIdx++)
    {
        u4value_24 = u4IO32Read4B(DRAMC_REG_CKECTRL + (u1ChannelIdx<<POS_BANK_NUM));//CKE control
        u4value_64 = u4IO32Read4B(DRAMC_REG_SPCMDCTRL + (u1ChannelIdx<<POS_BANK_NUM));//MR4 ZQCS
        u4value_38 = u4IO32Read4B(DRAMC_REG_DRAMC_PD_CTRL + (u1ChannelIdx<<POS_BANK_NUM));//MIOCKCTRLOFF
        vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL + (u1ChannelIdx<<POS_BANK_NUM), 1, SPCMDCTRL_MRRSWUPD);//enable MRS SW update
        vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL + (u1ChannelIdx<<POS_BANK_NUM), 1, SPCMDCTRL_REFRDIS);/* Disable MR4 => 0x64[29] = 1 */
        vIO32WriteFldMulti(DRAMC_REG_SPCMDCTRL + (u1ChannelIdx<<POS_BANK_NUM), P_Fld(0, SPCMDCTRL_ZQCSDISB)| P_Fld(0, SPCMDCTRL_ZQCALDISB));/* Disable ZQCS => LPDDR4: 0x64[30] = 0 LPDDR3: 0x64[31] = 0 */
        vIO32WriteFldMulti(DRAMC_REG_CKECTRL + (u1ChannelIdx<<POS_BANK_NUM), P_Fld(0, CKECTRL_CKEFIXOFF)|
            P_Fld(0, CKECTRL_CKE1FIXOFF) | P_Fld(1, CKECTRL_CKEFIXON) |
            P_Fld(1, CKECTRL_CKE1FIXON));/* CKE0 CKE1 fix on no matter the setting of CKE2RANK*/
        vIO32WriteFldMulti(DRAMC_REG_MRS + (u1ChannelIdx<<POS_BANK_NUM), P_Fld(0, MRS_MRRRK)| P_Fld(0, MRS_MRSMA));
        vIO32WriteFldAlign(DRAMC_REG_MRS + (u1ChannelIdx<<POS_BANK_NUM), 4, MRS_MRSMA);//Set MR4
        vIO32WriteFldAlign(DRAMC_REG_SPCMD + (u1ChannelIdx<<POS_BANK_NUM), 1, SPCMD_MRREN);//Issue MRR
        while(u4IO32ReadFldAlign(DRAMC_REG_SPCMDRESP + (u1ChannelIdx<<POS_BANK_NUM), SPCMDRESP_MRR_RESPONSE) == 0);//wait DRAMC_CONF_NAO + 0x88 [1] = 1
        vIO32WriteFldAlign(DRAMC_REG_SPCMD + (u1ChannelIdx<<POS_BANK_NUM), 0, SPCMD_MRREN);//Disable Issue MRR
        vIO32Write4B(DRAMC_REG_CKECTRL + (u1ChannelIdx<<POS_BANK_NUM), u4value_24);//restore CKE control
        vIO32Write4B(DRAMC_REG_SPCMDCTRL + (u1ChannelIdx<<POS_BANK_NUM), u4value_64);//restore MR4 ZQCS
        vIO32Write4B(DRAMC_REG_DRAMC_PD_CTRL + (u1ChannelIdx<<POS_BANK_NUM), u4value_38);//restore MIOCKCTRLOFF
        vIO32WriteFldAlign(DRAMC_REG_SPCMDCTRL + (u1ChannelIdx<<POS_BANK_NUM), 0, SPCMDCTRL_MRRSWUPD);//enable MRS SW update
    }
    return;
}
void SLT_Func_Apsrc_Wakeup(DRAMC_CTX_T *p, int fromS0)
{
    U32 u4Value = 0;
    mcSHOW_DBG_MSG(("[Start]Func_Apsrc_Wakeup\n"));
    if(fromS0 == 1)
    {
        SLT_FUNC_DDRPHY_INFRA_MTCMOS_OFF_EXIT(p);
        mcDELAY_US(1);
    }
    SLT_FUNC_DDRPHY_DLL_OFF_INFRA_CG_EN_EXIT(p);
    mcDELAY_US(1);
    u4Value = u4IO32ReadFldAlign(DRAMC_REG_SPCMDCTRL, SPCMDCTRL_REFRDIS);//Read MR4 setting of CHA, expect 4 channel setting is the same
    if((fromS0 == 1) && (u4Value == 0))//u4value is 0: MR4 is on
    {
        SLT_SW_MR4(p);
    }
    //-- Signal MD/MD32/MD2/CONN resource ready
    //Reg_Sync_Writel(PDEF_SPM_POWER_ON_VAL1, Reg_Readl(PDEF_SPM_POWER_ON_VAL1) | (1U << PDEF_MD_APSRC_ACK_LSB));
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
    SLT_Low_Power_Scenarios_Init(p);

    //gVcore = pmic_vcore_voltage_read();//Lewis@20151110: Use Vcore and (Vcore-100) mV to do DVS
    testCnt = 6;
    for(i=0; i<testCnt;i++)
    {
        mcSHOW_DBG_MSG(("**** Start Low Power scenarios DDR[%d]****\n", psCurrDramCtx->frequency * 2));
        mcSHOW_DBG_MSG(("**** Test[%d] ****\n", i+1));
        EMI_LPBK_memory_test_start(p, 0); //Emi lpbk test
        //SLT_Access_Memory_Test(p);
        /* Suspend */
        mcSHOW_DBG_MSG(("**** Suspend\n"));
        SLT_Suspend_Low_Power_Scenario(p);
        /* Resume */
        mcSHOW_DBG_MSG(("**** Resume\n"));
        SLT_Resume_Low_Power_Scenario(p);


#if DUAL_FREQ_K//EXECUTE_DFS_TEST_PROGRAM
        //DFSTestProgramForLowPower(p);
        DFSTestProgram(p, 0);//Since S0 + DVFS will lead CQDMA could not finished(only in M17 project).{Refer to M17_toogle_mode_issue.pptx}
#endif

        EMI_LPBK_memory_test_start(p, 0); //Emi lpbk test

        mcSHOW_DBG_MSG(("**** Stop Power scenarios test End ****\n"));
        mcSHOW_DBG_MSG(("\n\n\n\n\n\n"));

        gSLT_LS = (gSLT_LS+1)&1;
    }
    //Dma_Test(p);
}
#endif //#if (fcFOR_CHIP_ID == fcCervino)
#endif //#if (ENABLE_EMI_LPBK_TEST)
//======================== Low_Power_Scenarios_Test End =====================================
//======================== EMI LPBK TEST Definition Begin ===================================
#if defined(SLT) || (FT_DSIM_USED)
#define EMI_LPBK_USE_THROUGH_IO_O1PATH_TEST_USED 1
U32 O1Path_test_start(DRAMC_CTX_T * p)
{
    U32 channel_idx, backup_channel;
    U32 Tx_dqs_dly=0;
    U32 addr_w;
    U32 u4dq_o1_ff=0, u4ca_o1_ff=0;
    U32 u4dq_o1_00=0, u4ca_o1_00=0;
    U32 u4fail[2]={0,0};

#if RUN_AT_ETT || __ETT__
    GetPhyPllFrequency(p);
    //mcDELAY_US(2);
    mcSHOW_ERR_MSG(("[EMI Internal LPBK] DDR\033[1;32m%d\033[m\n",p->frequency<<1));
    //mcSHOW_ERR_MSG(("Vcore: %d\n",(int)dramc_get_vcore_voltage()));
#endif
    backup_channel = vGetPHY2ChannelMapping(p);

    O1PathOnOff(p, 1);

    mcSHOW_ERR_MSG(("O1Path_Test\n"));

    for(channel_idx=CHANNEL_A; channel_idx<2; channel_idx++)
    {
#if __ETT__ || defined(SLT)
        U32 u4RegBackupAddress[] =
        {
            (DRAMC_REG_ADDR(DDRPHY_CA_CMD5)),
            (DRAMC_REG_ADDR(DRAMC_REG_PADCTRL)),
            (DRAMC_REG_ADDR(DDRPHY_B0_DQ6)),
            (DRAMC_REG_ADDR(DDRPHY_B1_DQ6)),
            (DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_CA_CMD11)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING1)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING2)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING3)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING4)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DLL1)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DLL1)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_CA_DLL1)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9)),
            (DDRPHY_MISC_CG_CTRL0),
            (DDRPHY_MISC_CG_CTRL0+SHIFT_TO_CHB_ADDR),
            (DDRPHY_B0_DQ7),
            (DDRPHY_B1_DQ7),
            (DDRPHY_CA_CMD7),
            (DDRPHY_B0_DQ7+SHIFT_TO_CHB_ADDR),
            (DDRPHY_B1_DQ7+SHIFT_TO_CHB_ADDR),
            (DDRPHY_CA_CMD7+SHIFT_TO_CHB_ADDR),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_DQS0)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_DQS1)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ1)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ2)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ3)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHURK1_SELPH_DQ0)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHURK1_SELPH_DQ1)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHURK1_SELPH_DQ2)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHURK1_SELPH_DQ3)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU_ODTCTRL)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU1_WODT)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ7)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DQ7)),
            (DRAMC_REG_ADDR(DRAMC_REG_DDRCONF0)),
            (DRAMC_REG_ADDR(DDRPHY_B0_DQ9)),
            (DRAMC_REG_ADDR(DDRPHY_B1_DQ9)),

            (DRAMC_REG_ADDR(DDRPHY_B0_DQ3)),
            (DRAMC_REG_ADDR(DDRPHY_B1_DQ3)),
            (DRAMC_REG_ADDR(DDRPHY_B0_DQ5)),
            (DRAMC_REG_ADDR(DDRPHY_B1_DQ5)),
            (DRAMC_REG_ADDR(DDRPHY_CA_CMD3)),
            (DRAMC_REG_ADDR(DDRPHY_CA_CMD5)),
            (DRAMC_REG_ADDR(DDRPHY_B0_DQ2)),
            (DRAMC_REG_ADDR(DDRPHY_B1_DQ2)),
            (DRAMC_REG_ADDR(DDRPHY_B0_DQ7)),
            (DRAMC_REG_ADDR(DDRPHY_B1_DQ7)),
            (DRAMC_REG_ADDR(DDRPHY_CA_CMD2)),
            (DRAMC_REG_ADDR(DDRPHY_CA_CMD7)),
            (DRAMC_REG_ADDR(DDRPHY_B0_DQ0)),
            (DRAMC_REG_ADDR(DDRPHY_B0_DQ1)),
            (DRAMC_REG_ADDR(DDRPHY_B1_DQ0)),
            (DRAMC_REG_ADDR(DDRPHY_B1_DQ1)),
            (DRAMC_REG_ADDR(DDRPHY_CA_CMD0)),
            (DRAMC_REG_ADDR(DDRPHY_CA_CMD1)),
            (DRAMC_REG_ADDR(DDRPHY_CA_CMD6)),
            (DRAMC_REG_ADDR(DDRPHY_MISC_EXTLB18)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ8)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DQ8)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_CA_CMD8)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_CA_CMD5)),
        };
        DramcBackupRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));
#endif

        //mcSHOW_ERR_MSG(("=== CH_%c\n",'A'+channel_idx));
        vSetPHY2ChannelMapping(p, channel_idx);

        // add new RG setting for 6771/Can_non O1Path pull dn/up setting
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_CMD5), 1, CA_CMD5_RG_RX_ARCMD_VREF_EN);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_CA_CMD5), 0xb, SHU1_CA_CMD5_RG_RX_ARCMD_VREF_SEL);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MISC_EXTLB18), P_Fld(0x1, MISC_EXTLB18_R_LPBK_DQ_RX_MODE)|P_Fld(0x1, MISC_EXTLB18_R_LPBK_CA_RX_MODE));
        //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_CMD6), 0, CA_CMD6_RG_TX_ARCMD_DDR4_SEL);//for sim
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ8), 1, SHU1_B0_DQ8_R_RMRX_TOPHY_CG_IG_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DQ8), 1, SHU1_B1_DQ8_R_RMRX_TOPHY_CG_IG_B1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_CA_CMD8), 1, SHU1_CA_CMD8_R_RMRX_TOPHY_CG_IG_CA);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_CMD3), 1, CA_CMD3_RG_RX_ARCMD_SMT_EN);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_CMD5), 1, CA_CMD5_RG_RX_ARCMD_EYE_VREF_EN);
        // DQ, CMD OE_DIS
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B0_DQ2), P_Fld(0x1, B0_DQ2_RG_TX_ARDQ_OE_DIS_B0)|P_Fld(0x1, B0_DQ2_RG_TX_ARDQM0_OE_DIS_B0));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B1_DQ2), P_Fld(0x1, B1_DQ2_RG_TX_ARDQ_OE_DIS_B1)|P_Fld(0x1, B1_DQ2_RG_TX_ARDQM0_OE_DIS_B1));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_CA_CMD2), P_Fld(0x1, CA_CMD2_RG_TX_ARCMD_OE_DIS)|P_Fld(0x1, CA_CMD2_RG_TX_ARCLK_OE_DIS));
        // pull down DQ, DQM, CMD, CS, CLK
//fra            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B0_DQ7), P_Fld(0x1, B0_DQ7_RG_TX_ARDQ_PULL_DN_B0)|P_Fld(0x1, B0_DQ7_RG_TX_ARDQM0_PULL_DN_B0));
//fra            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B1_DQ7), P_Fld(0x1, B1_DQ7_RG_TX_ARDQ_PULL_DN_B1)|P_Fld(0x1, B1_DQ7_RG_TX_ARDQM0_PULL_DN_B1));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_CA_CMD7), P_Fld(0x1, CA_CMD7_RG_TX_ARCMD_PULL_DN)|P_Fld(0x1, CA_CMD7_RG_TX_ARCS_PULL_DN)|P_Fld(0x1, CA_CMD7_RG_TX_ARCLK_PULL_DN));
        mcDELAY_US(1);
        u4ca_o1_00 = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_AD_RX_CMD_O1));

        // Justin: remove original pull up driver, use new design for Can_non/6771
        // pull up DQ, DQM, CMD, CS, CLK
#if 0
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ3), 1, B0_DQ3_RG_RX_ARDQ_OFFC_EN_B0);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B0_DQ0), P_Fld(0xF, B0_DQ0_RG_RX_ARDQ7_OFFC_B0)|
                                                          P_Fld(0xF, B0_DQ0_RG_RX_ARDQ6_OFFC_B0)|
                                                          P_Fld(0xF, B0_DQ0_RG_RX_ARDQ5_OFFC_B0)|
                                                          P_Fld(0xF, B0_DQ0_RG_RX_ARDQ4_OFFC_B0)|
                                                          P_Fld(0xF, B0_DQ0_RG_RX_ARDQ3_OFFC_B0)|
                                                          P_Fld(0xF, B0_DQ0_RG_RX_ARDQ2_OFFC_B0)|
                                                          P_Fld(0xF, B0_DQ0_RG_RX_ARDQ1_OFFC_B0)|
                                                          P_Fld(0xF, B0_DQ0_RG_RX_ARDQ0_OFFC_B0));
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ1), 0xF, B0_DQ1_RG_RX_ARDQM0_OFFC_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ3), 1, B1_DQ3_RG_RX_ARDQ_OFFC_EN_B1);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_B1_DQ0), P_Fld(0xF, B1_DQ0_RG_RX_ARDQ7_OFFC_B1)|
                                                          P_Fld(0xF, B1_DQ0_RG_RX_ARDQ6_OFFC_B1)|
                                                          P_Fld(0xF, B1_DQ0_RG_RX_ARDQ5_OFFC_B1)|
                                                          P_Fld(0xF, B1_DQ0_RG_RX_ARDQ4_OFFC_B1)|
                                                          P_Fld(0xF, B1_DQ0_RG_RX_ARDQ3_OFFC_B1)|
                                                          P_Fld(0xF, B1_DQ0_RG_RX_ARDQ2_OFFC_B1)|
                                                          P_Fld(0xF, B1_DQ0_RG_RX_ARDQ1_OFFC_B1)|
                                                          P_Fld(0xF, B1_DQ0_RG_RX_ARDQ0_OFFC_B1));
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ1), 0xF, B1_DQ1_RG_RX_ARDQM0_OFFC_B1);
#endif
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_CA_CMD7),P_Fld(0x0, CA_CMD7_RG_TX_ARCMD_PULL_DN)|
                                                          P_Fld(0x0, CA_CMD7_RG_TX_ARCS_PULL_DN)|
                                                          P_Fld(0x0, CA_CMD7_RG_TX_ARCLK_PULL_DN)|
                                                          P_Fld(0x1, CA_CMD7_RG_TX_ARCMD_PULL_UP)|
                                                          P_Fld(0x1, CA_CMD7_RG_TX_ARCS_PULL_UP)|
                                                          P_Fld(0x1, CA_CMD7_RG_TX_ARCLK_PULL_UP));

        mcDELAY_US(1);
        u4ca_o1_ff = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_AD_RX_CMD_O1));

        // disable pull up CMD, CS, CLK
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_CA_CMD7), P_Fld(0x0, CA_CMD7_RG_TX_ARCMD_PULL_UP)|P_Fld(0x0, CA_CMD7_RG_TX_ARCS_PULL_UP)|P_Fld(0x0, CA_CMD7_RG_TX_ARCLK_PULL_UP));





        // EMI internal lpbk initial
        mcSHOW_ERR_MSG(("=== EMI Intlpbk Init, DDR%d, CH_%c\n",p->frequency<<1,'A'+channel_idx));
        //mcSHOW_ERR_MSG(("=== CH_%c, O1 Path initial setting\n",'A'+channel_idx));
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_PADCTRL), 1, PADCTRL_DRAMOEN);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ2), 0x1, B0_DQ2_RG_TX_ARDQ_OE_DIS_B0);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ2), 0x1, B1_DQ2_RG_TX_ARDQ_OE_DIS_B1);
#if EMI_LPBK_USE_LP3_PINMUX
        vIO32WriteFldAlign_All(DDRPHY_CA_CMD2, 0x1, CA_CMD2_RG_TX_ARCMD_OE_DIS);
#endif
        // add new RG setting for 6771/Can_non O1Path pull dn/up setting
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_CA_CMD5), 1, CA_CMD5_RG_RX_ARCMD_VREF_EN);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_SHU1_CA_CMD5), 0xb, SHU1_CA_CMD5_RG_RX_ARCMD_VREF_SEL);
        //vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MISC_EXTLB18), P_Fld(0x1, MISC_EXTLB18_R_LPBK_DQ_RX_MODE)|P_Fld(0x1, MISC_EXTLB18_R_LPBK_CA_RX_MODE));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_MISC_EXTLB18), P_Fld(0x1, MISC_EXTLB18_R_LPBK_DQ_RX_MODE)|P_Fld(0x0, MISC_EXTLB18_R_LPBK_CA_RX_MODE));

        //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_CMD6), 0, CA_CMD6_RG_TX_ARCMD_DDR4_SEL);//for sim

        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ8), 1, SHU1_B0_DQ8_R_RMRX_TOPHY_CG_IG_B0);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DQ8), 1, SHU1_B1_DQ8_R_RMRX_TOPHY_CG_IG_B1);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_SHU1_CA_CMD8), 1, SHU1_CA_CMD8_R_RMRX_TOPHY_CG_IG_CA);

        //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_CMD3), 1, CA_CMD3_RG_RX_ARCMD_SMT_EN);
        //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_CMD5), 1, CA_CMD5_RG_RX_ARCMD_EYE_VREF_EN);

        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ2), P_Fld(0x0, B0_DQ2_RG_TX_ARDQ_OE_DIS_B0)|P_Fld(0x1, B0_DQ2_RG_TX_ARDQM0_OE_DIS_B0));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ2), P_Fld(0x0, B1_DQ2_RG_TX_ARDQ_OE_DIS_B1)|P_Fld(0x1, B1_DQ2_RG_TX_ARDQM0_OE_DIS_B1));
#if EMI_LPBK_USE_LP3_PINMUX
        vIO32WriteFldAlign_All(DDRPHY_CA_CMD2, 0x0, CA_CMD2_RG_TX_ARCMD_OE_DIS);
#endif
        //mcDELAY_MS(1);
        // 5. Set lpbk mode, force RX_DQSIEN free run
        // Set LPBK mode,
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_MISC_EXTLB18), 1, MISC_EXTLB18_R_LPBK_DQ_RX_MODE);
#if EMI_LPBK_USE_THROUGH_IO_O1PATH_TEST_USED
        // Set LPBK_DQ_RX_MODE=0
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), 0, B0_DQ6_RG_RX_ARDQ_LPBK_EN_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), 0, B1_DQ6_RG_RX_ARDQ_LPBK_EN_B1);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), P_Fld(0x1, MISC_CTRL1_R_DMRRESETB_I_OPT)|P_Fld(0x0, MISC_CTRL1_R_DMDA_RRESETB_I));

        /* Set IMP_VREF_SEL register field's value */
        //term: 0x1b, unterm: 0x1a
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_CA_CMD11), 0x1a, SHU1_CA_CMD11_RG_RIMP_VREF_SEL);

        // Set IMP driving: drvp, drvn, odtn
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING1), 0xffffffff);
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING2), 0xffffffff);
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING3), 0xffffffff);
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING4), 0xffffffff);
#else
        // Set LPBK_DQ_RX_MODE=1
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), 1, B0_DQ6_RG_RX_ARDQ_LPBK_EN_B0);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), 1, B1_DQ6_RG_RX_ARDQ_LPBK_EN_B1);
#if EMI_LPBK_USE_LP3_PINMUX
        vIO32WriteFldAlign_All(DDRPHY_CA_CMD6, 1, CA_CMD6_RG_RX_ARCMD_LPBK_EN);
#endif
#if (!EMI_LPBK_DRAM_USED)
        dq_term_option = (p->frequency>=1600)?1:0;
        if (dq_term_option)
        {   ddrphy_drvp = 10; ddrphy_drvn = 9; ddrphy_odtn = 15;
        }
        else
        {   ddrphy_drvp = 8; ddrphy_drvn = 7; ddrphy_odtn = 15;
        }

        DisImpHw = (p->frequency >= 1333)?0:1;

        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING1), 0);
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING2), 0);
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING3), 0);
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING4), 0);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING1), P_Fld(DisImpHw, SHU1_DRVING1_DIS_IMPCAL_HW)|
                                                                   P_Fld(0, SHU1_DRVING1_DIS_IMP_ODTN_TRACK)|
                                                                   P_Fld(ddrphy_drvp, SHU1_DRVING1_DQSDRVP2)|
                                                                   P_Fld(ddrphy_drvn, SHU1_DRVING1_DQSDRVN2)|
                                                                   P_Fld(ddrphy_drvp, SHU1_DRVING1_DQSDRVP1)|
                                                                   P_Fld(ddrphy_drvn, SHU1_DRVING1_DQSDRVN1)|
                                                                   P_Fld(ddrphy_drvp, SHU1_DRVING1_DQDRVP2)|
                                                                   P_Fld(ddrphy_drvn, SHU1_DRVING1_DQDRVN2));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING2), P_Fld((!dq_term_option), SHU1_DRVING2_DIS_IMPCAL_ODT_EN)|
                                                                   P_Fld(ddrphy_drvp, SHU1_DRVING2_DQDRVP1)|
                                                                   P_Fld(ddrphy_drvn, SHU1_DRVING2_DQDRVN1)|
                                                                   P_Fld(ddrphy_drvp, SHU1_DRVING2_CMDDRVP2)|
                                                                   P_Fld(ddrphy_drvn, SHU1_DRVING2_CMDDRVN2)|
                                                                   P_Fld(ddrphy_drvp, SHU1_DRVING2_CMDDRVP1)|
                                                                   P_Fld(ddrphy_drvn, SHU1_DRVING2_CMDDRVN1));

        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING3), P_Fld(0, SHU1_DRVING3_DQSODTP2)|
                                                                   P_Fld(ddrphy_odtn, SHU1_DRVING3_DQSODTN2)|
                                                                   P_Fld(0, SHU1_DRVING3_DQSODTP)|
                                                                   P_Fld(ddrphy_odtn, SHU1_DRVING3_DQSODTN)|
                                                                   P_Fld(0, SHU1_DRVING3_DQODTP2)|
                                                                   P_Fld(ddrphy_odtn, SHU1_DRVING3_DQODTN2));

        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING4), P_Fld(0, SHU1_DRVING4_DQODTP1)|
                                                                   P_Fld(ddrphy_odtn, SHU1_DRVING4_DQODTN1)|
                                                                   P_Fld(0, SHU1_DRVING4_CMDODTP2)|
                                                                   P_Fld(ddrphy_odtn, SHU1_DRVING4_CMDODTN2)|
                                                                   P_Fld(0, SHU1_DRVING4_CMDODTP1)|
                                                                   P_Fld(ddrphy_odtn, SHU1_DRVING4_CMDODTN1));
#endif
#endif  //EMI_LPBK_USE_THROUGH_IO_O1PATH_TEST_USED
        // RX_DQSIEN_FORCE_ON_EN=1 free run
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DLL1), 1, RG_ARDQ_REV_BIT_19_RX_DQSIEN_FORCE_ON_EN);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DLL1), 1, RG_ARDQ_REV_BIT_19_RX_DQSIEN_FORCE_ON_EN);
#if EMI_LPBK_USE_LP3_PINMUX
        vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_DLL1, 1, RG_ARCMD_REV_BIT_19_RX_DQSIEN_FORCE_ON_EN);
#endif
        // 6. Set tx dqs/dq delay (now set 0)
        if (p->frequency>=1600)
            Tx_dqs_dly = 32;
        else if (p->frequency>=1200)
            Tx_dqs_dly = 20;
        else
            Tx_dqs_dly = 0;

        //mcSHOW_ERR_MSG(("=== Tx_dqs_dly = %d\n",Tx_dqs_dly));
#if EMI_LPBK_USE_LP3_PINMUX==0
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS2), 0x0);
        vIO32Write4B((DDRPHY_R0_B0_RXDVS2)+(1<<16), 0x0);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), Tx_dqs_dly, SHU1_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2), 0x0);
        vIO32Write4B((DDRPHY_R0_B1_RXDVS2)+(1<<16), 0x0);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ7), Tx_dqs_dly, SHU1_R0_B1_DQ7_RK0_ARPI_PBYTE_B1);
#else
        //emi still is lp4 setting, so only can see B0/B1, can't see B2/B3
        vIO32WriteFldAlign_Phy_Byte(0, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), 0, SHU1_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);
        vIO32WriteFldAlign_Phy_Byte(0, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), 0, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);
        vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), 0, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CLK);
        vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), 0, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CMD);
        vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), 0, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CS);
#endif

        // 7. Enable CG
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_MISC_CG_CTRL0), 0x3fc1f);
        vIO32Write4B((DDRPHY_MISC_CG_CTRL0)+(1<<16), 0x3fc1f);
        //releated golden setting, if enable,
        //vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_CA_DLL_ARPI2), 0x0);
        //vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL3), 0x15001500);
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B0_DQ7), 0x0);
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B1_DQ7), 0x0);
        vIO32Write4B((DDRPHY_B0_DQ7)+(1<<16), 0x0);
        vIO32Write4B((DDRPHY_B1_DQ7)+(1<<16), 0x0);
#if EMI_LPBK_USE_LP3_PINMUX
        vIO32Write4B((DDRPHY_CA_CMD7), 0x0);
        vIO32Write4B((DDRPHY_CA_CMD7)+(1<<16), 0x0);
#endif
        // Jouling add "adjust TX DQ SELPH", + disable R/W DBI define + remove MR3 bit[7:6]
        if (p->frequency>800)
        {
            //36, 32, 24 pass
            vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_DQS0), 0x33333333);
            vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_DQS1), 0x22225555);
            vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0), 0x33333333);
            vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ1), 0x33333333);
            vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ2), 0x33336666);
            vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ3), 0x33336666);
            vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK1_SELPH_DQ0), 0x33333333);
            vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK1_SELPH_DQ1), 0x33333333);
            vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK1_SELPH_DQ2), 0x33336666);
            vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK1_SELPH_DQ3), 0x33336666);
        }
        else
        {
            //32, 24, 16 pass
            vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_DQS0), 0x11112222);
            vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_DQS1), 0x66661111);
            vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0), 0x11112222);
            vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ1), 0x11112222);
            vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ2), 0x77771111);
            vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ3), 0x77771111);
            vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK1_SELPH_DQ0), 0x11112222);
            vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK1_SELPH_DQ1), 0x11112222);
            vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK1_SELPH_DQ2), 0x77771111);
            vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK1_SELPH_DQ3), 0x77771111);
        }
        // Joe Disable RODT
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SHU_ODTCTRL), 0, SHU_ODTCTRL_ROEN);
        // Joe Disable write dbi
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SHU1_WODT), 0, SHU1_WODT_DBIWR);

        // Joe Disable read dbi
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ7), 0, SHU1_B0_DQ7_R_DMDQMDBI_SHU_B0);
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DQ7), 0, SHU1_B1_DQ7_R_DMDQMDBI_SHU_B1);

        //Francis test , parameter same with LP4 calibration
        addr_w = (0x40000024 & (~0x100)) | 0x100*channel_idx;
        mcSHOW_ERR_MSG(("addr = 0x%x\n",addr_w));

        // When write data, please follow the flow
        // 1.   Reset rxfifo and dramc fifo
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

        mcDELAY_US(2);
        *((UINT32P)(addr_w)) = 0x00000000;
        *((UINT32P)(addr_w+4)) = 0x00000000;
        *((UINT32P)(addr_w+8)) = 0x00000000;
        *((UINT32P)(addr_w+12)) = 0x00000000;
        *((UINT32P)(addr_w+16)) = 0x00000000;
        *((UINT32P)(addr_w+20)) = 0x00000000;
        *((UINT32P)(addr_w+24)) = 0x00000000;
        *((UINT32P)(addr_w+28)) = 0x00000000;
        u4dq_o1_00 = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_DQO1));



        // When write data, please follow the flow
        // 1.   Reset rxfifo and dramc fifo
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

        mcDELAY_US(2);
        *((UINT32P)(addr_w)) = 0xffffffff;
        *((UINT32P)(addr_w+4)) = 0xffffffff;
        *((UINT32P)(addr_w+8)) = 0xffffffff;
        *((UINT32P)(addr_w+12)) = 0xffffffff;
        *((UINT32P)(addr_w+16)) = 0xffffffff;
        *((UINT32P)(addr_w+20)) = 0xffffffff;
        *((UINT32P)(addr_w+24)) = 0xffffffff;
        *((UINT32P)(addr_w+28)) = 0xffffffff;
        u4dq_o1_ff = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_MISC_DQO1));

        if (u4dq_o1_00!=0 || u4ca_o1_00!=0)
            u4fail[channel_idx] = 1;
        if (u4dq_o1_ff!=0xFFFF || u4ca_o1_ff!=0x6C3F) //refer to RG coda field
            u4fail[channel_idx] = 1;

        mcSHOW_ERR_MSG(("Fra CH_%d DQ O1=0x%x, CS-CKE-CA O1=0x%x\n",channel_idx,u4dq_o1_00,u4ca_o1_00));
        mcSHOW_ERR_MSG(("Fra CH_%d DQ O1=0x%x, CS-CKE-CA O1=0x%x\n",channel_idx,u4dq_o1_ff,u4ca_o1_ff));

        mcSHOW_ERR_MSG(("=== CH_%c, O1Path test %s\n\n\n",'A'+channel_idx, (u4fail[channel_idx]==0)?"pass":"fail"));

#if __ETT__ || defined(SLT)
        DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));
#endif
    }

    mcSHOW_ERR_MSG(("\n=== All Channel O1Path test done, test %s\n", ((u4fail[0]|u4fail[1])==0)?"pass":"fail"));
    O1PathOnOff(p, 0);
    vSetPHY2ChannelMapping(p, backup_channel);

#if __ETT__ || defined(SLT)
    if (u4fail[0] || u4fail[1])
        while(1);
#endif

    return (u4fail[0]|u4fail[1]);
}
#endif

#if ENABLE_EMI_LPBK_TEST || (FT_DSIM_USED)
U8  gEmiLpbkTest = 0;
void Set_Emi_Lpbk_Test_Mode(U8 u1OnOff)
{
    gEmiLpbkTest = u1OnOff?1:0;
}
void SetRxDqDqsDelay_FT(DRAMC_CTX_T *p, U16 dqs_Delay, U16 dq_Delay)
{
    U8 ii, u1ByteIdx;
    U32 u4value;
    U8 dl_value[8];
    // Set DQS delay
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ6), \
                            P_Fld(dqs_Delay,SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0) |P_Fld(dqs_Delay,SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_F_DLY_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ6), \
                            P_Fld(dqs_Delay,SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_R_DLY_B1) |P_Fld(dqs_Delay,SHU1_R0_B1_DQ6_RK0_RX_ARDQS0_F_DLY_B1));
    // Adjust DQM output delay.
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ6), \
                            P_Fld(dq_Delay,SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0) |P_Fld(dq_Delay,SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_F_DLY_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ6), \
                            P_Fld(dq_Delay,SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_R_DLY_B1) |P_Fld(dq_Delay,SHU1_R0_B1_DQ6_RK0_RX_ARDQM0_F_DLY_B1));
/*
    // Adjust CA output delay
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD2), \
                            P_Fld(dq_Delay,SHU1_R0_CA_CMD2_RG_RK0_RX_ARCA1_F_DLY) |P_Fld(dq_Delay,SHU1_R0_CA_CMD2_RG_RK0_RX_ARCA1_R_DLY) |
                            P_Fld(dq_Delay,SHU1_R0_CA_CMD2_RG_RK0_RX_ARCA0_F_DLY) |P_Fld(dq_Delay,SHU1_R0_CA_CMD2_RG_RK0_RX_ARCA0_R_DLY));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD3), \
                            P_Fld(dq_Delay,SHU1_R0_CA_CMD3_RG_RK0_RX_ARCA3_F_DLY) |P_Fld(dq_Delay,SHU1_R0_CA_CMD3_RG_RK0_RX_ARCA3_R_DLY) |
                            P_Fld(dq_Delay,SHU1_R0_CA_CMD3_RG_RK0_RX_ARCA2_F_DLY) |P_Fld(dq_Delay,SHU1_R0_CA_CMD3_RG_RK0_RX_ARCA2_R_DLY));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD4), \
                            P_Fld(dq_Delay,SHU1_R0_CA_CMD4_RG_RK0_RX_ARCA5_F_DLY) |P_Fld(dq_Delay,SHU1_R0_CA_CMD4_RG_RK0_RX_ARCA5_R_DLY) |
                            P_Fld(dq_Delay,SHU1_R0_CA_CMD4_RG_RK0_RX_ARCA4_F_DLY) |P_Fld(dq_Delay,SHU1_R0_CA_CMD4_RG_RK0_RX_ARCA4_R_DLY));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD5), \
                            P_Fld(dq_Delay,SHU1_R0_CA_CMD5_RG_RK0_RX_ARCKE1_F_DLY) |P_Fld(dq_Delay,SHU1_R0_CA_CMD5_RG_RK0_RX_ARCKE1_R_DLY) |
                            P_Fld(dq_Delay,SHU1_R0_CA_CMD5_RG_RK0_RX_ARCKE0_F_DLY) |P_Fld(dq_Delay,SHU1_R0_CA_CMD5_RG_RK0_RX_ARCKE0_R_DLY));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD6), \
                            P_Fld(dq_Delay,SHU1_R0_CA_CMD6_RG_RK0_RX_ARCS0_F_DLY) |P_Fld(dq_Delay,SHU1_R0_CA_CMD6_RG_RK0_RX_ARCS0_R_DLY) |
                            P_Fld(dq_Delay,SHU1_R0_CA_CMD6_RG_RK0_RX_ARCKE2_F_DLY) |P_Fld(dq_Delay,SHU1_R0_CA_CMD6_RG_RK0_RX_ARCKE2_R_DLY));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD7), \
                            P_Fld(dq_Delay,SHU1_R0_CA_CMD7_RG_RK0_RX_ARCS2_F_DLY) |P_Fld(dq_Delay,SHU1_R0_CA_CMD7_RG_RK0_RX_ARCS2_R_DLY) |
                            P_Fld(dq_Delay,SHU1_R0_CA_CMD7_RG_RK0_RX_ARCS1_F_DLY) |P_Fld(dq_Delay,SHU1_R0_CA_CMD7_RG_RK0_RX_ARCS1_R_DLY));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD8), \
                            P_Fld(dqs_Delay,SHU1_R0_CA_CMD8_RG_RK0_RX_ARCLK_F_DLY) |P_Fld(dqs_Delay,SHU1_R0_CA_CMD8_RG_RK0_RX_ARCLK_R_DLY));
*/

    DramPhyReset(p);

    // Adjust DQ output delay.
    u4value = ((U32) dq_Delay) | (((U32)dq_Delay)<<8) | (((U32)dq_Delay)<<16) | (((U32)dq_Delay)<<24);
    for (ii=0; ii<4; ii++)
    {
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ2+ ii*4), u4value);//DQ0~DQ7
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ2+ ii*4), u4value);//DQ8~DQ15
    }
}
void EMI_LPBK_memory_test_start(DRAMC_CTX_T * p, U8 K_win)
{
    static U32 shu_failcnt[2][3] = {{0,0,0}, {0,0,0}};
    U8 channel_idx, rank_idx, backup_channel, backup_rank, K_Rank_Num;
    U32 shu, u4dq_o1, u4ca_o1, u4fail_up=0, u4fail_dn=0, loop=0, fail_cnt=0, retry_dn_cnt=0, retry_up_cnt=0;
    U32 pattern_mask;
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

#if EMI_INT_LPBK_WL_DQS_RINGCNT==0
    GetPhyPllFrequency(p);
    mcDELAY_US(2);
    mcSHOW_ERR_MSG(("[EMI Internal LPBK] Shu: %d, DDR\033[1;32m%d\033[m\n",shu,p->frequency<<1));
    mcSHOW_ERR_MSG(("Vcore: %d\n",(int)dramc_get_vcore_voltage()));
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
    //for(channel_idx=CHANNEL_A; channel_idx<p->support_channel_num; channel_idx++)
#if EMI_LPBK_USE_LP3_PINMUX
    for(channel_idx=CHANNEL_A; channel_idx<1; channel_idx++)
#else
    for(channel_idx=CHANNEL_A; channel_idx<2; channel_idx++)
#endif
    {
        //mcSHOW_ERR_MSG(("=== CH_%c\n",'A'+channel_idx));
        vSetPHY2ChannelMapping(p, channel_idx);
#if FT_DSIM_USED==0
        U32 u4RegBackupAddress[] =
        {
            (DRAMC_REG_ADDR(DDRPHY_CA_CMD5)),
            (DRAMC_REG_ADDR(DDRPHY_B0_DQ2)),
            (DRAMC_REG_ADDR(DDRPHY_B1_DQ2)),
            (DRAMC_REG_ADDR(DDRPHY_MISC_EXTLB18)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ8)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DQ8)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_CA_CMD8)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_CA_CMD5)),

            (DRAMC_REG_ADDR(DRAMC_REG_PADCTRL)),
            (DRAMC_REG_ADDR(DDRPHY_B0_DQ6)),
            (DRAMC_REG_ADDR(DDRPHY_B1_DQ6)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DLL1)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DLL1)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7)),
            (DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ7)),
            (DRAMC_REG_ADDR(DDRPHY_MISC_CG_CTRL0)),
            (DRAMC_REG_ADDR(DDRPHY_CA_DLL_ARPI2)),//0x188   related to golden setting
            (DRAMC_REG_ADDR(DDRPHY_MISC_CTRL3)),//0x2a8 related to golden setting
            (DRAMC_REG_ADDR(DDRPHY_B0_DQ7)),
            (DRAMC_REG_ADDR(DDRPHY_B1_DQ7)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ1)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHURK1_SELPH_DQ0)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHURK1_SELPH_DQ1)),
            (DRAMC_REG_ADDR(DRAMC_REG_SHU_ODTCTRL)),
            (DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1)),
#if 0
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ6)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ6)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD2)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD3)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD4)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD5)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD6)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD7)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD8)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ2)),
            (DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ2)),
#endif
        };
#endif
        //mcSHOW_ERR_MSG(("=== CH_%c, backup RG\n",'A'+channel_idx));
#if FT_DSIM_USED==0
        DramcBackupRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));
#endif
//francis added for test
#if EMI_LPBK_USE_LP3_PINMUX
        //PHY digital
        vIO32WriteFldAlign((DDRPHY_MISC_CTRL1), 1, MISC_CTRL1_R_DMPINMUX);
        vIO32WriteFldAlign((DDRPHY_MISC_EXTLB18)+(1<<16), 1, MISC_EXTLB18_R_LPBK_CA_RX_MODE);
        //PHY analog
        vIO32WriteFldAlign((DDRPHY_CA_CMD6)+(1<<16), 1, CA_CMD6_RG_RX_ARCMD_LPBK_EN);
        vIO32WriteFldAlign((DDRPHY_SHU1_CA_DLL1)+(1<<16), 1, RG_ARCMD_REV_BIT_19_RX_DQSIEN_FORCE_ON_EN);
        vIO32WriteFldAlign((DDRPHY_CA_CMD6)+(1<<16), 0x1, CA_CMD6_RG_RX_ARCMD_BIAS_EN);
        vIO32WriteFldAlign((DDRPHY_CA_CMD6)+(1<<16), 0x1, CA_CMD6_RG_RX_ARCMD_DDR4_SEL);
        vIO32WriteFldAlign((DDRPHY_CA_CMD6)+(1<<16), 0x0, CA_CMD6_RG_RX_ARCMD_DDR3_SEL);
        vIO32WriteFldAlign((DDRPHY_CA_CMD6)+(1<<16), 0x1, CA_CMD6_RG_RX_ARCMD_OP_BIAS_SW_EN);
        vIO32WriteFldAlign((DDRPHY_CA_CMD6)+(1<<16), 0x1, CA_CMD6_RG_RX_ARCMD_RES_BIAS_EN);
        vIO32WriteFldAlign((DDRPHY_CA_CMD6)+(1<<16), 0x1, CA_CMD6_RG_RX_ARCMD_RPRE_TOG_EN);
        vIO32WriteFldAlign((DDRPHY_CA_CMD6)+(1<<16), 0x1, CA_CMD6_RG_RX_ARCMD_BIAS_PS);
        vIO32WriteFldAlign((DDRPHY_CA_CMD6)+(1<<16), 0x2, CA_CMD6_RG_RX_ARCMD_BIAS_VREF_SEL);
        vIO32WriteFldAlign((DDRPHY_CA_CMD5)+(1<<16), 0x1, CA_CMD5_RG_RX_ARCMD_VREF_EN);
        vIO32WriteFldAlign((DDRPHY_SHU1_CA_CMD5)+(1<<16), 0xb, SHU1_CA_CMD5_RG_RX_ARCMD_VREF_SEL);
        //(disable TX PI DCM)
        vIO32Write4B((DDRPHY_MISC_CG_CTRL5), 0);
        vIO32Write4B((DDRPHY_MISC_CG_CTRL5)+(1<<16), 0);

        //add for A-Q
        vIO32WriteFldAlign_All(DDRPHY_SHU1_B0_DQ6, 0x5815, SHU1_B0_DQ6_RG_ARPI_RESERVE_B0);
        vIO32WriteFldAlign_All(DDRPHY_SHU1_B1_DQ6, 0x5815, SHU1_B1_DQ6_RG_ARPI_RESERVE_B1);
        vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_CMD6, 0x5815, SHU1_CA_CMD6_RG_ARPI_RESERVE_CA);    //0x5817 for master/slave, 0x5815 for all slave
        vIO32WriteFldAlign_All(DDRPHY_SHU1_B0_DQ6, 3, SHU1_B0_DQ6_RG_ARPI_CAP_SEL_B0);
        vIO32WriteFldAlign_All(DDRPHY_SHU1_B1_DQ6, 3, SHU1_B1_DQ6_RG_ARPI_CAP_SEL_B1);
        vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_CMD6, 3, SHU1_CA_CMD6_RG_ARPI_CAP_SEL_CA);
        //For 6755-like mode, reserve bit5 should be set as "0" before calibration (by Justin)
        vIO32WriteFldAlign(DDRPHY_SHU1_B0_DLL1, 0, RG_ARDQ_REV_BIT_05_RX_SER_RST_MODE);
        vIO32WriteFldAlign(DDRPHY_SHU1_B1_DLL1, 0, RG_ARDQ_REV_BIT_05_RX_SER_RST_MODE);
        vIO32WriteFldAlign(DDRPHY_SHU1_CA_DLL1, 0, RG_ARCMD_REV_BIT_05_RX_SER_RST_MODE);
        vIO32WriteFldAlign(DDRPHY_SHU1_B0_DLL1+SHIFT_TO_CHB_ADDR, 0, RG_ARDQ_REV_BIT_05_RX_SER_RST_MODE);
        vIO32WriteFldAlign(DDRPHY_SHU1_B1_DLL1+SHIFT_TO_CHB_ADDR, 0, RG_ARDQ_REV_BIT_05_RX_SER_RST_MODE);
        vIO32WriteFldAlign(DDRPHY_SHU1_CA_DLL1+SHIFT_TO_CHB_ADDR, 0, RG_ARCMD_REV_BIT_05_RX_SER_RST_MODE);

#endif
        if (u1IsLP4Family(p->dram_type))
        {
            if (K_win)
            {
                // EMI internal lpbk initial
                mcSHOW_ERR_MSG(("=== EMI Intlpbk Init, DDR%d, CH_%c\n",DDRPhyFMeter()<<1,'A'+channel_idx));
                //mcSHOW_ERR_MSG(("=== CH_%c, O1 Path initial setting\n",'A'+channel_idx));
                vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_PADCTRL), 1, PADCTRL_DRAMOEN);
                vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ2), 0x1, B0_DQ2_RG_TX_ARDQ_OE_DIS_B0);
                vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ2), 0x1, B1_DQ2_RG_TX_ARDQ_OE_DIS_B1);
#if EMI_LPBK_USE_LP3_PINMUX
                vIO32WriteFldAlign_All(DDRPHY_CA_CMD2, 0x1, CA_CMD2_RG_TX_ARCMD_OE_DIS);
#endif
                // add new RG setting for 6771/Can_non O1Path pull dn/up setting
                vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_CA_CMD5), 1, CA_CMD5_RG_RX_ARCMD_VREF_EN);
                vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_SHU1_CA_CMD5), 0xb, SHU1_CA_CMD5_RG_RX_ARCMD_VREF_SEL);
                //vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MISC_EXTLB18), P_Fld(0x1, MISC_EXTLB18_R_LPBK_DQ_RX_MODE)|P_Fld(0x1, MISC_EXTLB18_R_LPBK_CA_RX_MODE));
                vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_MISC_EXTLB18), P_Fld(0x1, MISC_EXTLB18_R_LPBK_DQ_RX_MODE)|P_Fld(0x0, MISC_EXTLB18_R_LPBK_CA_RX_MODE));

                //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_CMD6), 0, CA_CMD6_RG_TX_ARCMD_DDR4_SEL);//for sim

                vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ8), 1, SHU1_B0_DQ8_R_RMRX_TOPHY_CG_IG_B0);
                vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DQ8), 1, SHU1_B1_DQ8_R_RMRX_TOPHY_CG_IG_B1);
                vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_SHU1_CA_CMD8), 1, SHU1_CA_CMD8_R_RMRX_TOPHY_CG_IG_CA);

                //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_CMD3), 1, CA_CMD3_RG_RX_ARCMD_SMT_EN);
                //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_CMD5), 1, CA_CMD5_RG_RX_ARCMD_EYE_VREF_EN);

                vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ2), P_Fld(0x0, B0_DQ2_RG_TX_ARDQ_OE_DIS_B0)|P_Fld(0x1, B0_DQ2_RG_TX_ARDQM0_OE_DIS_B0));
                vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ2), P_Fld(0x0, B1_DQ2_RG_TX_ARDQ_OE_DIS_B1)|P_Fld(0x1, B1_DQ2_RG_TX_ARDQM0_OE_DIS_B1));
#if EMI_LPBK_USE_LP3_PINMUX
                vIO32WriteFldAlign_All(DDRPHY_CA_CMD2, 0x0, CA_CMD2_RG_TX_ARCMD_OE_DIS);
#endif
                //mcDELAY_MS(1);
                // 5. Set lpbk mode, force RX_DQSIEN free run
                // Set LPBK mode,
                vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_MISC_EXTLB18), 1, MISC_EXTLB18_R_LPBK_DQ_RX_MODE);
#if EMI_LPBK_USE_THROUGH_IO
                // Set LPBK_DQ_RX_MODE=0
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), 0, B0_DQ6_RG_RX_ARDQ_LPBK_EN_B0);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), 0, B1_DQ6_RG_RX_ARDQ_LPBK_EN_B1);
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), P_Fld(0x1, MISC_CTRL1_R_DMRRESETB_I_OPT)|P_Fld(0x0, MISC_CTRL1_R_DMDA_RRESETB_I));

                /* Set IMP_VREF_SEL register field's value */
                //term: 0x1b, unterm: 0x1a
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_CA_CMD11), 0x1a, SHU1_CA_CMD11_RG_RIMP_VREF_SEL);

                // Set IMP driving: drvp, drvn, odtn
                vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING1), 0xffffffff);
                vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING2), 0xffffffff);
                vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING3), 0xffffffff);
                vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING4), 0xffffffff);
#else
                // Set LPBK_DQ_RX_MODE=1
                vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ6), 1, B0_DQ6_RG_RX_ARDQ_LPBK_EN_B0);
                vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ6), 1, B1_DQ6_RG_RX_ARDQ_LPBK_EN_B1);
#if EMI_LPBK_USE_LP3_PINMUX
                vIO32WriteFldAlign_All(DDRPHY_CA_CMD6, 1, CA_CMD6_RG_RX_ARCMD_LPBK_EN);
#endif
#if (!EMI_LPBK_DRAM_USED)
                dq_term_option = (p->frequency>=1600)?1:0;
                if (dq_term_option)
                {   ddrphy_drvp = 10; ddrphy_drvn = 9; ddrphy_odtn = 15;}
                else
                {   ddrphy_drvp = 8; ddrphy_drvn = 7; ddrphy_odtn = 15;}

                DisImpHw = (p->frequency >= 1333)?0:1;

                vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING1), 0);
                vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING2), 0);
                vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING3), 0);
                vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING4), 0);
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING1), P_Fld(DisImpHw, SHU1_DRVING1_DIS_IMPCAL_HW)|
                                                                           P_Fld(0, SHU1_DRVING1_DIS_IMP_ODTN_TRACK)|
                                                                           P_Fld(ddrphy_drvp, SHU1_DRVING1_DQSDRVP2)|
                                                                           P_Fld(ddrphy_drvn, SHU1_DRVING1_DQSDRVN2)|
                                                                           P_Fld(ddrphy_drvp, SHU1_DRVING1_DQSDRVP1)|
                                                                           P_Fld(ddrphy_drvn, SHU1_DRVING1_DQSDRVN1)|
                                                                           P_Fld(ddrphy_drvp, SHU1_DRVING1_DQDRVP2)|
                                                                           P_Fld(ddrphy_drvn, SHU1_DRVING1_DQDRVN2));
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING2), P_Fld((!dq_term_option), SHU1_DRVING2_DIS_IMPCAL_ODT_EN)|
                                                                           P_Fld(ddrphy_drvp, SHU1_DRVING2_DQDRVP1)|
                                                                           P_Fld(ddrphy_drvn, SHU1_DRVING2_DQDRVN1)|
                                                                           P_Fld(ddrphy_drvp, SHU1_DRVING2_CMDDRVP2)|
                                                                           P_Fld(ddrphy_drvn, SHU1_DRVING2_CMDDRVN2)|
                                                                           P_Fld(ddrphy_drvp, SHU1_DRVING2_CMDDRVP1)|
                                                                           P_Fld(ddrphy_drvn, SHU1_DRVING2_CMDDRVN1));

                vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING3), P_Fld(0, SHU1_DRVING3_DQSODTP2)|
                                                                           P_Fld(ddrphy_odtn, SHU1_DRVING3_DQSODTN2)|
                                                                           P_Fld(0, SHU1_DRVING3_DQSODTP)|
                                                                           P_Fld(ddrphy_odtn, SHU1_DRVING3_DQSODTN)|
                                                                           P_Fld(0, SHU1_DRVING3_DQODTP2)|
                                                                           P_Fld(ddrphy_odtn, SHU1_DRVING3_DQODTN2));

                vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU1_DRVING4), P_Fld(0, SHU1_DRVING4_DQODTP1)|
                                                                           P_Fld(ddrphy_odtn, SHU1_DRVING4_DQODTN1)|
                                                                           P_Fld(0, SHU1_DRVING4_CMDODTP2)|
                                                                           P_Fld(ddrphy_odtn, SHU1_DRVING4_CMDODTN2)|
                                                                           P_Fld(0, SHU1_DRVING4_CMDODTP1)|
                                                                           P_Fld(ddrphy_odtn, SHU1_DRVING4_CMDODTN1));
#endif
#endif  //EMI_LPBK_USE_THROUGH_IO
                // RX_DQSIEN_FORCE_ON_EN=1 free run
                vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DLL1), 1, RG_ARDQ_REV_BIT_19_RX_DQSIEN_FORCE_ON_EN);
                vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DLL1), 1, RG_ARDQ_REV_BIT_19_RX_DQSIEN_FORCE_ON_EN);
#if EMI_LPBK_USE_LP3_PINMUX
                vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_DLL1, 1, RG_ARCMD_REV_BIT_19_RX_DQSIEN_FORCE_ON_EN);
#endif
                // 6. Set tx dqs/dq delay (now set 0)
                if (p->frequency>=1600)
                    Tx_dqs_dly = 32;
                else if (p->frequency>=1200)
                    Tx_dqs_dly = 20;
                else
                    Tx_dqs_dly = 0;

                //mcSHOW_ERR_MSG(("=== Tx_dqs_dly = %d\n",Tx_dqs_dly));
#if EMI_LPBK_USE_LP3_PINMUX==0
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS2), 0x0);
                vIO32Write4B((DDRPHY_R0_B0_RXDVS2)+(1<<16), 0x0);
                vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), Tx_dqs_dly, SHU1_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2), 0x0);
                vIO32Write4B((DDRPHY_R0_B1_RXDVS2)+(1<<16), 0x0);
                vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ7), Tx_dqs_dly, SHU1_R0_B1_DQ7_RK0_ARPI_PBYTE_B1);
#else
                //emi still is lp4 setting, so only can see B0/B1, can't see B2/B3
                vIO32WriteFldAlign_Phy_Byte(0, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), 0, SHU1_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);
                vIO32WriteFldAlign_Phy_Byte(0, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), 0, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);
                vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), 0, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CLK);
                vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), 0, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CMD);
                vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), 0, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CS);
#endif

                // 7. Enable CG
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_MISC_CG_CTRL0), 0x3fc1f);
                vIO32Write4B((DDRPHY_MISC_CG_CTRL0)+(1<<16), 0x3fc1f);
                //releated golden setting, if enable,
                //vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_CA_DLL_ARPI2), 0x0);
                //vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL3), 0x15001500);
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B0_DQ7), 0x0);
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_B1_DQ7), 0x0);
                vIO32Write4B((DDRPHY_B0_DQ7)+(1<<16), 0x0);
                vIO32Write4B((DDRPHY_B1_DQ7)+(1<<16), 0x0);
#if EMI_LPBK_USE_LP3_PINMUX
                vIO32Write4B((DDRPHY_CA_CMD7), 0x0);
                vIO32Write4B((DDRPHY_CA_CMD7)+(1<<16), 0x0);
#endif
                // Jouling add "adjust TX DQ SELPH", + disable R/W DBI define + remove MR3 bit[7:6]
                if (p->frequency>800)
                {
                    //36, 32, 24 pass
                    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_DQS0), 0x33333333);
                    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_DQS1), 0x22225555);
                    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0), 0x33333333);
                    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ1), 0x33333333);
                    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ2), 0x33336666);
                    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ3), 0x33336666);
                    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK1_SELPH_DQ0), 0x33333333);
                    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK1_SELPH_DQ1), 0x33333333);
                    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK1_SELPH_DQ2), 0x33336666);
                    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK1_SELPH_DQ3), 0x33336666);
                }
                else
                {
                    //32, 24, 16 pass
                    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_DQS0), 0x11112222);
                    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_DQS1), 0x66661111);
                    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ0), 0x11112222);
                    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ1), 0x11112222);
                    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ2), 0x77771111);
                    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQ3), 0x77771111);
                    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK1_SELPH_DQ0), 0x11112222);
                    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK1_SELPH_DQ1), 0x11112222);
                    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK1_SELPH_DQ2), 0x77771111);
                    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK1_SELPH_DQ3), 0x77771111);
                }
                // Joe Disable RODT
                vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SHU_ODTCTRL), 0, SHU_ODTCTRL_ROEN);
                // Joe Disable write dbi
                vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SHU1_WODT), 0, SHU1_WODT_DBIWR);

                // Joe Disable read dbi
                vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_SHU1_B0_DQ7), 0, SHU1_B0_DQ7_R_DMDQMDBI_SHU_B0);
                vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_SHU1_B1_DQ7), 0, SHU1_B1_DQ7_R_DMDQMDBI_SHU_B1);

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
#if 1
if (rx_enable)
{
            if (K_win>0)
            {
                dq_center=0,dqs_center=0;
                SetRxDqDqsDelay_FT(p, dqs_center, dq_center);//saint
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
                        len = 0x100; //0x20000;//0x40;
#endif
                        step = 0x4;

#if 1//FOR_DV_SIMULATION_USED
                        pi_dly_step = 4;
#endif
                        if (k_type==3)
                        {
#if 1//FOR_DV_SIMULATION_USED
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
                            if (k_type==3)
                            {
                                //adjust DQS dly
                                SetRxDqDqsDelay_FT(p, pi_dly, dq_center);
                                pattern_mask=0x00ff00ff;
                            }
                            else if (k_type==2)
                            {
                                //adjust DQ dly
                                SetRxDqDqsDelay_FT(p, dqs_center, pi_dly);
                                pattern_mask=0x00ff00ff;
                            }
                            else if (k_type==0)
                            {
                                //adjust clk dly
                                pattern_mask=0xff00ff00;
                            }
                            else
                            {
                                //adjust cmd dly
                                pattern_mask=0xff00ff00;
                            }

                            //mcSHOW_ERR_MSG(("\nCH_%c, O%d pattern 0x%X: from 0x%x ~ 0x%x\n",'A'+channel_idx,pi_dly,pattern,addr_s,addr_s+len));
                            //mcSHOW_ERR_MSG(("\nCH_%c, from 0x%x ~ 0x%x\n",'A'+channel_idx,addr_s,addr_s+len));

                            len = len;
                            for (addr_offset=0;addr_offset<len;addr_offset+=step)
                            {
                                addr_w = addr_s + addr_offset;
                                addr_w = (addr_w & (~0x100)) | 0x100*channel_idx;
                                //if ((channel_idx==0)&&(addr_w&0x100)||(channel_idx==1)&&((addr_w&0x100)==0))
                                //    continue;

                                pattern = (randSeed = 69069 * randSeed + 362437);
                                //mcSHOW_ERR_MSG(("[%d]pattern = 0x%x\n",addr_offset,pattern));
                                //pattern = 0x55aa55aa;

                            #if 1
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
                            #endif

                                mcDELAY_US(1);
                                {
                                    dly = 1;    //saint
                                    w_addr = addr_w & ~0x1f;
                                    w_offset = addr_w & 0x1f;
                                    addr_r = addr_w;
                                    if (w_offset < 0x10)
                                    {
                                        for (loop_w=0;loop_w<8;loop_w++)
                                        {
                                        #if !FOR_DV_SIMULATION_USED
                                            mcDELAY_US(dly);
                                        #endif
                                            *((UINT32P)(addr_w)) = pattern;
                                        }
                                        #if !FOR_DV_SIMULATION_USED
                                            mcDELAY_US(2);
                                        #endif
                                        u4value = *((UINT32P)(addr_r));
                                    }
                                    else if (w_offset>=0x10 && w_offset<0x20)
                                    {
                                        for (loop_w=0;loop_w<10;loop_w++)
                                        {
                                        #if !FOR_DV_SIMULATION_USED
                                            mcDELAY_US(dly);
                                        #endif
                                            *((UINT32P)(addr_w)) = pattern;
                                        }
                                        #if !FOR_DV_SIMULATION_USED
                                            mcDELAY_US(2);
                                        #endif
                                        u4value = *((UINT32P)(addr_r));
                                    }
                                }
        //    mcSHOW_DBG_MSG(("Fra rx (0x%x, 0x%x), ii=%d\n",u4value, pattern, ii));
                                if (u4value!=pattern)
                                    pass = 0;
                                else
                                    pass = 1;

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
                            SetRxDqDqsDelay_FT(p, dqs_center, dq_center);

                            if (dqs_center==0&&k_again_dqs==0)
                            {
                                dqs_center = 32;
                                mcSHOW_ERR_MSG(("\n=== saint Rx DQS K fail, adjust DQ dly = 32\n"));
                                SetRxDqDqsDelay_FT(p, dqs_center, 0);
                                k_again_dqs = 1;
                                k_type = 0;
                            }
                        }
                        else if (k_type==2)
                        {
                            //adjust DQ dly
                            dq_center = win_center;
                            SetRxDqDqsDelay_FT(p, dqs_center, dq_center);

                            if (dq_center==0&&k_again_dq==0)
                            {
                                dqs_center = 32;
                                mcSHOW_ERR_MSG(("\n=== saint Rx DQ K fail, adjust DQS dly = 32\n"));
                                SetRxDqDqsDelay_FT(p, dqs_center, dq_center);
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
                dqs_Delay = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ6), SHU1_R0_B0_DQ6_RK0_RX_ARDQS0_R_DLY_B0);
                // Get Rx DQ/DQM delay
                dq_Delay = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ6), SHU1_R0_B0_DQ6_RK0_RX_ARDQM0_R_DLY_B0);
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
if (tx_enable)
{
            if (K_win>0)
            {
                dq_center=0,dqs_center=0;
#if EMI_LPBK_USE_LP3_PINMUX
                for (k_type=0;k_type<4;k_type++) //2: Adjust DQ dly, 3: Adjust DQS dly
#else
                for (k_type=2;k_type<4;k_type++) //2: Adjust DQ dly, 3: Adjust DQS dly
#endif
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
                        len = 0x100; //0x20000;//0x40;
#endif
                        step = 0x4;

                        max_idx = 63;
#if 1//FOR_DV_SIMULATION_USED
                        pi_dly_step = 4;
#endif

                        mcSHOW_ERR_MSG(("\n=== K Tx %s dly window, address: 0x%x, len= 0x%x, pi_dly_step= %d\n",(k_type==2)?"DQ":(k_type==3)?"DQS":(k_type==0)?"CLK":"CMD",(addr_s&~0x100)|(0x100*channel_idx),len,pi_dly_step));
                        for (pi_dly=0;pi_dly<=max_idx;pi_dly+=pi_dly_step) //adjust pi_dly
                        {
                        if (k_type==3)
                        {
#if EMI_LPBK_USE_LP3_PINMUX==0
                            //adjust DQS dly
                            vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS2), 0x0);
                            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ii, SHU1_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);//DQS dly
                            vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2), 0x0);
                            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ7), ii, SHU1_R0_B1_DQ7_RK0_ARPI_PBYTE_B1);
#else
                            //emi still is lp4 setting, so only can see B0/B1, can't see B2/B3
                            vIO32WriteFldAlign_Phy_Byte(0, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ii, SHU1_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);
                            vIO32WriteFldAlign_Phy_Byte(0, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), 0, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);
            //mm==0/1 k       vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ii, SHU1_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);
            //                vIO32WriteFldAlign_Phy_Byte(2, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ii, SHU1_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);
            //                vIO32WriteFldAlign_Phy_Byte(3, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ii, SHU1_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);
#endif
                            pattern_mask=0x00ff00ff;
                        }
                        else if (k_type==2)
                        {
#if EMI_LPBK_USE_LP3_PINMUX==0
                            //adjust DQ dly
                            vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS2), 0x0);
                            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), P_Fld(ii, SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0)|P_Fld(ii, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0));//DQ dly
                            vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2), 0x0);
                            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ7), P_Fld(ii, SHU1_R0_B1_DQ7_RK0_ARPI_DQM_B1)|P_Fld(ii, SHU1_R0_B1_DQ7_RK0_ARPI_DQ_B1));//DQ dly
#else
                            //emi still is lp4 setting, so only can see B0/B1, can't see B2/B3
                            vIO32WriteFldAlign_Phy_Byte(0, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), 0, SHU1_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);
                            vIO32WriteFldAlign_Phy_Byte(0, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ii, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);
            //mm==0/1 k     vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ii, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);
            //              vIO32WriteFldAlign_Phy_Byte(2, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ii, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);
            //              vIO32WriteFldAlign_Phy_Byte(3, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ii, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);
#endif
                            pattern_mask=0x00ff00ff;
                        }
                        else if (k_type==0)
                        {
#if EMI_LPBK_USE_LP3_PINMUX
                            //adjust CLK(dq) dly
                            //vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD9 + (1<<16), P_Fld(ii, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CLK)|P_Fld(0, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CMD));
                            vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), ii, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CLK);
                            vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), 0, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CMD);
                            vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), 0, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CS);
                            //vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ii, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);
#endif
                            pattern_mask=0xff00ff00;
                        }
                        else
                        {
#if EMI_LPBK_USE_LP3_PINMUX
                            //adjust CMD(dqs) dly
                            //vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD9 + (1<<16), P_Fld(0, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CLK)|P_Fld(ii, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CMD));
                            vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), 0, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CLK);
                            vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), ii, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CMD);
                            vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_CA_CMD9), ii, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CS);
                            //vIO32WriteFldAlign_Phy_Byte(1, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), ii, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);
#endif
                            pattern_mask=0xff00ff00;
                        }

                            //mcSHOW_ERR_MSG(("\nCH_%c, O%d pattern 0x%X: from 0x%x ~ 0x%x\n",'A'+channel_idx,pi_dly,pattern,addr_s,addr_s+len));
                            //mcSHOW_ERR_MSG(("\nCH_%c, from 0x%x ~ 0x%x\n",'A'+channel_idx,addr_s,addr_s+len));

                            len = len;
                            for (addr_offset=0;addr_offset<len;addr_offset+=step)
                            {
                                addr_w = addr_s + addr_offset;
                                addr_w = (addr_w & (~0x100)) | 0x100*channel_idx;
                                //if ((channel_idx==0)&&(addr_w&0x100)||(channel_idx==1)&&((addr_w&0x100)==0))
                                //    continue;

                                pattern = (randSeed = 69069 * randSeed + 362437);
                                //mcSHOW_ERR_MSG(("[%d]pattern = 0x%x\n",addr_offset,pattern));
                                //pattern = 0x55aa55aa;
                            #if 1
                                // When write data, please follow the flow
                                // 1.	Reset rxfifo and dramc fifo
                                vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 1, MISC_CTRL1_R_DMPHYRST);
                                vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_DDRCONF0), 1, DDRCONF0_RDATRST);
                                vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 0, MISC_CTRL1_R_DMPHYRST);
                                vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_DDRCONF0), 0, DDRCONF0_RDATRST);

                                //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ3), 0, B0_DQ3_RG_ARDQ_RESETB_B0);
                                vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ9), P_Fld(0, B0_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B0)|P_Fld(0, B0_DQ9_RG_RX_ARDQ_STBEN_RESETB_B0));
                                //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ3), 0, B1_DQ3_RG_ARDQ_RESETB_B1);
                                vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ9), P_Fld(0, B1_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B1)|P_Fld(0, B1_DQ9_RG_RX_ARDQ_STBEN_RESETB_B1));

                                //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ3), 1, B0_DQ3_RG_ARDQ_RESETB_B0);
                                vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_B0_DQ9), P_Fld(1, B0_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B0)|P_Fld(1, B0_DQ9_RG_RX_ARDQ_STBEN_RESETB_B0));
                                //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ3), 1, B1_DQ3_RG_ARDQ_RESETB_B1);
                                vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_B1_DQ9), P_Fld(1, B1_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B1)|P_Fld(1, B1_DQ9_RG_RX_ARDQ_STBEN_RESETB_B1));
                            #endif
                                mcDELAY_US(1);
                                {
                                    dly = 1;    //saint
                                    w_addr = addr_w & ~0x1f;
                                    w_offset = addr_w & 0x1f;
                                    addr_r = addr_w;
                                    if (w_offset < 0x10)
                                    {
                                        for (loop_w=0;loop_w<8;loop_w++)
                                        {
                                            #if !FOR_DV_SIMULATION_USED
                                            mcDELAY_US(dly);
                                            #endif
                                            *((UINT32P)(addr_w)) = pattern;
                                        }
                                        #if !FOR_DV_SIMULATION_USED
                                            mcDELAY_US(2);
                                        #endif
                                        u4value = *((UINT32P)(addr_r));
                                    }
                                    else if (w_offset>=0x10 && w_offset<0x20)
                                    {
                                        for (loop_w=0;loop_w<10;loop_w++)
                                        {
                                            #if !FOR_DV_SIMULATION_USED
                                            mcDELAY_US(dly);
                                            #endif
                                            *((UINT32P)(addr_w)) = pattern;
                                        }
                                        #if !FOR_DV_SIMULATION_USED
                                            mcDELAY_US(2);
                                        #endif
                                        u4value = *((UINT32P)(addr_r));
                                    }
                                }


                                //if (k_type==0 || k_type==1) mcSHOW_DBG_MSG(("Fra tx (0x%x, 0x%x), pi_dly=%d\n",u4value, pattern, pi_dly));
                                //if (pi_dly==25) mcSHOW_DBG_MSG(("Fra tx (0x%x, 0x%x), pi_dly=%d\n",u4value, pattern, pi_dly));
#if EMI_LPBK_USE_LP3_PINMUX
                                if ((u4value&pattern_mask)!=(pattern&pattern_mask))
#else
                                if (u4value!=pattern)
#endif
                                    pass = 0;
                                else
                                    pass = 1;

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
                    win_center = ((win[1]+win[0])>>1);
                    #if FOR_DV_SIMULATION_USED
                    mcSHOW_ERR_MSG(("\n=== saint Tx %s %d ~ %d, size= %d\n",(k_type==2)?"DQ":(k_type==3)?"DQS":(k_type==0)?"CLK":"CMD",win[0],win[1],win[1]-win[0]+1));
                    #else
                    mcSHOW_ERR_MSG(("\n=== saint Tx %s %d ~ %d, size= \033[1;36m%d\033[m\n",(k_type==2)?"DQ":(k_type==3)?"DQS":(k_type==0)?"CLK":"CMD",win[0],win[1],win[1]-win[0]+1));
                    #endif
            if (k_type==3)
            {
                    dqs_center = win_center;
#if EMI_LPBK_USE_LP3_PINMUX==0
                    //adjust DQS dly
                    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS2), 0x0);
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), win_center, SHU1_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);
                    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2), 0x0);
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ7), win_center, SHU1_R0_B1_DQ7_RK0_ARPI_PBYTE_B1);

                    if (win_center==0&&k_again_dqs==0)
                    {
                        win_center = 32;
                        mcSHOW_ERR_MSG(("\n=== saint DQS K fail, adjust DQ dly = 32\n"));
                        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS2), 0x0);
                        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), P_Fld(win_center, SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0)|P_Fld(win_center, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0));//DQ dly
                        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2), 0x0);
                        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ7), P_Fld(win_center, SHU1_R0_B1_DQ7_RK0_ARPI_DQM_B1)|P_Fld(win_center, SHU1_R0_B1_DQ7_RK0_ARPI_DQ_B1));//DQ dly
                        k_again_dqs = 1;
                        mm = 2;
                        goto _label_1;
                    }
#else
                    //emi still is lp4 setting, so only can see B0/B1, can't see B2/B3
                    vIO32WriteFldAlign_Phy_Byte(0, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), win_center, SHU1_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);
                    if (dqs_center==0&&k_again_dqs==0)
                    {
                        dq_center = 32;
                        mcSHOW_ERR_MSG(("\n=== saint Tx DQS K fail, adjust DQ dly = 32\n"));
                        vIO32WriteFldAlign_Phy_Byte(0, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), win_center, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);
                        k_again_dqs = 1;
                        k_type = 2;
                        goto _label_1;
                    }
#endif
            }
            else if (k_type==2)
            {
#if EMI_LPBK_USE_LP3_PINMUX==0
                    //adjust DQ dly
                    dq_center = win_center;
                    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS2), 0x0);
                    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), P_Fld(dq_center, SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0)|P_Fld(win_center, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0));//DQ dly
                    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2), 0x0);
                    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ7), P_Fld(dq_center, SHU1_R0_B1_DQ7_RK0_ARPI_DQM_B1)|P_Fld(win_center, SHU1_R0_B1_DQ7_RK0_ARPI_DQ_B1));//DQ dly

                    if (dq_center==0&&k_again_dq==0)
                    {
                        dqs_center = 32;
                        mcSHOW_ERR_MSG(("\n=== saint Tx DQ K fail, adjust DQS dly = 32\n"));
                        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_R0_B0_RXDVS2), 0x0);
                        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), dqs_center, SHU1_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);
                        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_R0_B1_RXDVS2), 0x0);
                        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B1_DQ7), dqs_center, SHU1_R0_B1_DQ7_RK0_ARPI_PBYTE_B1);
                        k_again_dq = 1;
                    }
                    if ((dq_center!=0||dqs_center!=0)&&k_again_dq!=1)
                    {
                        first = last = 1;
                        win[0] = win[1] = 0;
                        continue;//break;
                    }
#else
                    //emi still is lp4 setting, so only can see B0/B1, can't see B2/B3
                    vIO32WriteFldAlign_Phy_Byte(0, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), win_center, SHU1_R0_B0_DQ7_RK0_ARPI_DQ_B0);
                    if (dq_center==0&&k_again_dq==0)
                    {
                        dqs_center = 32;
                        mcSHOW_ERR_MSG(("\n=== saint DQ K fail, adjust DQS dly = 32\n"));
                        vIO32WriteFldAlign_Phy_Byte(0, DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), dqs_center, SHU1_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);
                        k_again_dq = 1;
                    }
#endif
            }
            else if (k_type==0)
            {
                    //adjust CLK dly
                    vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD9 + (1<<16), P_Fld(win_center, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CLK)|P_Fld(0, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CMD));
            }
            else
            {
                    //adjust CMD dly
                    vIO32WriteFldMulti(DDRPHY_SHU1_R0_CA_CMD9 + (1<<16), P_Fld(0, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CLK)|P_Fld(win_center, SHU1_R0_CA_CMD9_RG_RK0_ARPI_CMD));
            }
    _label_1:
                    first = last = 1;
                    win[0] = win[1] = 0;
                }
                }
            }
            if (K_win)
            {
                // Get Tx DQS per byte delay
                dqs_Delay = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), SHU1_R0_B0_DQ7_RK0_ARPI_PBYTE_B0);
                // Get Tx DQ/DQM per byte delay
                dq_Delay = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU1_R0_B0_DQ7), SHU1_R0_B0_DQ7_RK0_ARPI_DQM_B0);
                #if FOR_DV_SIMULATION_USED
                mcSHOW_ERR_MSG(("\n=== Tx dly, DQ/DQM center: %d, DQS center: %d\n",dq_Delay,dqs_Delay));
                #else
                mcSHOW_ERR_MSG(("\n=== Tx dly, DQ/DQM center: \033[1;32m%d\033[m, DQS center: \033[1;32m%d\033[m\n",dq_Delay,dqs_Delay));
                #endif
            }
            //while(1);
}
#endif
#if EMI_INT_LPBK_WL_DQS_RINGCNT
            {
                // When write data, please follow the flow
                // 1.   Reset rxfifo and dramc fifo
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
                //saint 710
                //go_ahead(0,0,"SLT 710 check DQS ? WL-1UI DDR800 EMI intlpbk thr io + 1CPU write");
                B0 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_PHY_RGS_STBEN_B0), MISC_PHY_RGS_STBEN_B0_AD_RX_ARDQ0_STBEN_B0);
                B1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_PHY_RGS_STBEN_B1), MISC_PHY_RGS_STBEN_B1_AD_RX_ARDQ0_STBEN_B1);

                pattern = 0xffffffff;
                mcSHOW_DBG_MSG(("\nbefore === CH_%d, pattern: 0x%x, B0: 0x%x, B1: 0x%x\n",channel_idx,pattern,B0,B1));
                *((volatile UINT32P)(0x40000000|(0x100*channel_idx))) = pattern;
                B0 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_PHY_RGS_STBEN_B0), MISC_PHY_RGS_STBEN_B0_AD_RX_ARDQ0_STBEN_B0);
                B1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_PHY_RGS_STBEN_B1), MISC_PHY_RGS_STBEN_B1_AD_RX_ARDQ0_STBEN_B1);
                mcSHOW_DBG_MSG(("\nafter  === CH_%d, pattern: 0x%x, B0: 0x%x, B1: 0x%x\n",channel_idx,pattern,B0,B1));
                //2400 through io, SLT 710(B1 DQS1 fail) or good ic
                //WL-1UI, good ic : 0x30/0x30(5 times), 710: 0x30/0xC(5 times)
                continue; // for next channel
                while(1);
            }
#endif


            //EMI Internal LPBK test
            if (K_win==0)
            {
                mcSHOW_ERR_MSG(("\n===== Start EMI LPBK Test .... =====, %s\n","CPU Write"));
                addr_s = test_addr[0];
                //len = 0x44000;  //0x10;
#if FT_DSIM_USED || FOR_DV_SIMULATION_USED
                len = 0x20;//0x40;
#else
                len = 0x100;//0x40;
#endif
                step = 0x4;

#if FT_DSIM_USED || FOR_DV_SIMULATION_USED
                addr_num = 1;
#else
                addr_num = 4;
#endif
                for (addr_idx=0;addr_idx<addr_num;addr_idx++)
                {
                    addr_s = test_addr[addr_idx];
                for (pa_idx=4;pa_idx<5;pa_idx++) // 0:O0 pattern, 1:O1 pattern
                {
                    //pattern = (randSeed = 69069 * randSeed + 362437);
                    //pattern = (ii==0)?randSeed:(ii==1)?0xffffffff:(ii==2)?0:(ii==3)?0xAAAAAAAA:0x55555555;
                    pattern = (pa_idx==0)?0:(pa_idx==1)?0xffffffff:(pa_idx==2)?0xAAAAAAAA:(pa_idx==3)?0x55555555:(randSeed = 69069 * randSeed + 362437);

                    len = len;
                    //mcSHOW_ERR_MSG(("\nCH_%c, O%d pattern 0x%X: from 0x%x ~ 0x%x\n",'A'+channel_idx,ii,pattern,addr_s,addr_s+len));
                    mcSHOW_ERR_MSG(("\nCH_%c, from 0x%x ~ 0x%x, len= 0x%x, pattern: %d\n",'A'+channel_idx,addr_s,addr_s+len,len,pa_idx));

                    for (addr_offset=0;addr_offset<len;addr_offset+=step) // for address 0xXXXXXXX0 only
                    {
                        addr_w = addr_s + addr_offset;
                        addr_w = (addr_w & (~0x100)) | 0x100*channel_idx;
                        //if ((channel_idx==0)&&(addr_w&0x100)||(channel_idx==1)&&((addr_w&0x100)==0))
                        //    continue;
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

        #if 1
                        // When write data, please follow the flow
                        // 1.   Reset rxfifo and dramc fifo
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
        #endif
                        mcDELAY_US(1);
    //                    mcSHOW_ERR_MSG(("Reset rxfifo and dramc fifo\n"));
#if EMI_LPBK_ADDRESS_DEFECT
#define EMI_APB_BASE   0x10219000
#define CHN0_EMI_BASE  0x1022D000
#define CHN1_EMI_BASE  0x10235000
#endif
                        {
                            dly = 1;    //saint
                            w_addr = addr_w & ~0x1f;
                            w_offset = addr_w & 0x1f;
                            addr_r = addr_w;
                            if (w_offset < 0x10)
                            {
                                for (loop_w=0;loop_w<8;loop_w++)
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
                                    else
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
                                for (loop_w=0;loop_w<10;loop_w++)
                                {
                                #if !FOR_DV_SIMULATION_USED
                                    mcDELAY_US(dly);
                                #endif
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
                                        *((UINT32P)(addr_w)) = pattern;
                                    }
                                }

                                #if !FOR_DV_SIMULATION_USED
                                mcDELAY_US(2);
                                #endif
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

                        //if (1)//(!pass)
                        if (!pass)
                        {
                            mcSHOW_ERR_MSG(("CH_%c, cmp: 0x\033[1;36m%x\033[m, [%d]pattern: 0x%x, write addr: 0x%x, read addr: 0x%x, value: 0x%x (%s)\n",'A'+channel_idx,u4value^pattern,addr_offset>>2,pattern,addr_w,addr_r,u4value,pass?"Pass":"\033[1;36mFail\033[m"));
                            //mcSHOW_ERR_MSG(("\tO%d pattern : CH_%d, address: 0x%x\t= 0x%x (%s)\n",ii,channel_idx,address,u4value,pass?"Pass":"\033[1;36mFail\033[m"));
                            //while(1);
                            shu_failcnt[channel_idx][shu]++;
                            emi_test_fail[channel_idx]++;
                            break;
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
    {
        //go_ahead(p,0,0);
        //while(1);
    }
    #endif
    //DramcRegDump(p); //fra

    //mcSHOW_ERR_MSG(("=== CH_%c, restore RG\n",'A'+channel_idx));
#if FT_DSIM_USED==0
    //saint DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));
#endif
#if EMI_INT_LPBK_WL_DQS_RINGCNT==0
    mcSHOW_ERR_MSG(("=== Total err cnt:\t\t\t[0]:0x%x/0x%x, [1]:0x%x/0x%x, [2]:0x%x/0x%x\n\n\n",shu_failcnt[0][0],shu_failcnt[1][0],shu_failcnt[0][1],shu_failcnt[1][1],shu_failcnt[0][2],shu_failcnt[1][2]));
#endif
#if defined(SLT)
    if (shu_failcnt[0][0]||shu_failcnt[1][0]||shu_failcnt[0][1]||shu_failcnt[1][1]||shu_failcnt[0][2]||shu_failcnt[1][2])
        while(1);
#endif
    //mcSHOW_ERR_MSG(("=== All Channel test done\n"));
    vSetPHY2ChannelMapping(p, backup_channel);
    //vIO32WriteFldAlign_All(DRAMC_REG_PADCTRL, 0, PADCTRL_DRAMOEN);
    //while(1);
}

void EMI_LPBK_memory_test(DRAMC_CTX_T * p)
{
        U32 ii;
        //Enable golden setting
        EnableDramcPhyDCM(p, 1);
        mcSHOW_ERR_MSG(("\nline: %d, DDR%d\n",__LINE__,p->frequency<<1));
//test SER_MODE=1
//vIO32WriteFldAlign_All(DDRPHY_CA_CMD6, 0x1, CA_CMD6_RG_TX_ARCMD_SER_MODE);
//vIO32WriteFldAlign_All((DDRPHY_B0_DQ6), 1, B0_DQ6_RG_TX_ARDQ_SER_MODE_B0);
//vIO32WriteFldAlign_All((DDRPHY_B1_DQ6), 1, B1_DQ6_RG_TX_ARDQ_SER_MODE_B1);
#if 0
        DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
        vIO32WriteFldAlign(DDRPHY_B0_DQ6, 0x0, B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0);
        vIO32WriteFldAlign(DDRPHY_B1_DQ6, 0x0, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1);
        vIO32WriteFldAlign(DDRPHY_CA_CMD6, 0x0, CA_CMD6_RG_RX_ARCMD_BIAS_EN);
        DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);

        vIO32WriteFldMulti_All(DDRPHY_SHU1_B0_DQ6, P_Fld(1, SHU1_B0_DQ6_RG_ARPI_CAP_SEL_B0)
                    | P_Fld(2, SHU1_B0_DQ6_RG_ARPI_MIDPI_CAP_SEL_B0));
        vIO32WriteFldMulti_All(DDRPHY_SHU1_B1_DQ6, P_Fld(1, SHU1_B1_DQ6_RG_ARPI_CAP_SEL_B1)
                    | P_Fld(2, SHU1_B1_DQ6_RG_ARPI_MIDPI_CAP_SEL_B1));
        vIO32WriteFldMulti_All(DDRPHY_SHU1_CA_CMD6, P_Fld(1, SHU1_CA_CMD6_RG_ARPI_CAP_SEL_CA)
                    | P_Fld(2, SHU1_CA_CMD6_RG_ARPI_MIDPI_CAP_SEL_CA));
        vIO32WriteFldAlign(DDRPHY_CA_CMD10+SHIFT_TO_CHB_ADDR, 0x1, CA_CMD10_RG_RX_ARCMD_STBEN_RESETB);
        vIO32WriteFldAlign(DDRPHY_CA_CMD10+SHIFT_TO_CHB_ADDR, 0x1, CA_CMD10_RG_RX_ARCLK_STBEN_RESETB);
#endif
#if FT_DSIM_USED==0 // FT will load SHU1/SHU2/SHU3 init into RG before EMI_LPBK test
#if DUAL_FREQ_K
        if(u1IsLP4Family(p->dram_type))
        {
            //init emi lpbk
            //EMI_LPBK_memory_test_start(p, 1);
#if EMI_LPBK_DFS_16
            EMI_LPBK_memory_test_start(p, 1);
            DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_1, DRAM_DFS_SHUFFLE_2);
            DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_1, DRAM_DFS_SHUFFLE_3);
#elif EMI_LPBK_DFS_24
            DDRPhyFreqSel(p, gFreqTbl[DRAM_DFS_SHUFFLE_2].freq_sel);
            vSetVcoreByFreq(p);
            u1PrintModeRegWrite=1;
            DramcInit(p);
            u1PrintModeRegWrite=0;
            vApplyConfigBeforeCalibration(p);
            //init emi lpbk
            EMI_LPBK_memory_test_start(p, 1);
            DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_1, DRAM_DFS_SHUFFLE_2);
            DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_1, DRAM_DFS_SHUFFLE_3);
#elif EMI_LPBK_DFS_32
            DDRPhyFreqSel(p, gFreqTbl[DRAM_DFS_SHUFFLE_1].freq_sel);
            vSetVcoreByFreq(p);
            u1PrintModeRegWrite=1;
            DramcInit(p);
            u1PrintModeRegWrite=0;
            vApplyConfigBeforeCalibration(p);
            //init emi lpbk
            EMI_LPBK_memory_test_start(p, 1);
            DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_1, DRAM_DFS_SHUFFLE_2);
            DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_1, DRAM_DFS_SHUFFLE_3);
#else
            EnableDramcPhyDCM(p, 1);
            EMI_LPBK_memory_test_start(p, 1);
            DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_1, DRAM_DFS_SHUFFLE_3); //Darren NOTE: Please take care of gFreqTbl table when you update SHUFFLE_X

            //===  Second frequency ======
            DDRPhyFreqSel(p, gFreqTbl[DRAM_DFS_SHUFFLE_2].freq_sel);
            vSetVcoreByFreq(p);
            DramcInit(p);
            vApplyConfigBeforeCalibration(p);

            //init emi lpbk
            EnableDramcPhyDCM(p, 1);
            EMI_LPBK_memory_test_start(p, 1);
            DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_1, DRAM_DFS_SHUFFLE_2); //Darren NOTE: Please take care of gFreqTbl table when you update SHUFFLE_X
            //===  Third frequency ======
            DDRPhyFreqSel(p, gFreqTbl[DRAM_DFS_SHUFFLE_1].freq_sel);
            vSetVcoreByFreq(p);
            DramcInit(p);
            vApplyConfigBeforeCalibration(p);

            //init emi lpbk
            EnableDramcPhyDCM(p, 1);
            EMI_LPBK_memory_test_start(p, 1);
#endif
        }
        else
        {
            DDRPhyFreqSel(p, LP3_DDR1600);
            vSetVcoreByFreq(p);
            u1PrintModeRegWrite=1;
            DramcInit(p);
            u1PrintModeRegWrite=0;
            vApplyConfigBeforeCalibration(p);
            //init emi lpbk
            EMI_LPBK_memory_test_start(p, 1);
            DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_1, DRAM_DFS_SHUFFLE_2);   //save 1333 0.74V shuffle_3
            DDRPhyFreqSel(p, LP3_DDR1866);
            vSetVcoreByFreq(p);
            u1PrintModeRegWrite=1;
            DramcInit(p);
            u1PrintModeRegWrite=0;
            vApplyConfigBeforeCalibration(p);
            //init emi lpbk
            EMI_LPBK_memory_test_start(p, 1);
        }
        TransferPLLToSPMControl(p);  //switch to SPM control
        TransferToSPMControl(p);
#endif
#endif
        for (ii=0;ii<5;ii++)  // Do 3 times EMI LPBK test
        {
            EMI_LPBK_memory_test_start(p, 0);
#if DUAL_FREQ_K
            DFSTestProgram(p, 0);
#endif
            DFS_Freq_Meter_Check(p, DDRPhyFreqMeter());
        }
        // EMI LPBK + LS0/1
        SLT_Low_Power_Scenarios_Test(p, 0);

#if 0
#if EMI_INT_LPBK_WL_DQS_RINGCNT //Dramc_init+WL(adjust selPH, -1UI)+ LP4-2400 EMI loop-bak pattern + check waveform

        U8 ii;

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
#endif


#if FT_DSIM_USED==0
        //while(1);
#endif
}
#endif
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
    if(u1IsLP4Family(dram_type))
    {
        psCurrDramCtx = &DramCtx_LPDDR4;
    }
    else
    {
        return;
    }
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

		if (slt_data.test_result[0]) {
			mcSHOW_DBG_MSG(("[SLT] Fail in previous test\n"));
			slt_data.header.stage_status = 1;
		} else if (slt_data.header.stage_status == -1) {
			slt_data.header.stage_status = 1;
		} else {
			slt_data.header.stage_status = 0;
		}

		mcSHOW_DBG_MSG(("[SLT] stage:%d\n", slt_data.header.stage_status));

		slt_data.test_result[0] = 1;

		result = write_slt_data(&slt_data);
		if (result < 0) {
			mcSHOW_DBG_MSG(("[SLT] write fail. %d\n", result));
			while(1);
		}

		Set_Emi_Lpbk_Test_Mode(slt_data.header.stage_status);

#else
		Set_Emi_Lpbk_Test_Mode(1);
#endif
		if (!gEmiLpbkTest) {
			#if (!__ETT__)
			clean_slt_data(); // clear data in storage for next round
			read_slt_data(&slt_data);
			#endif
			return;
		}
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
#if DUAL_FREQ_K
    SPM_Pinmux_Setting(p);
    if(u1IsLP4Family(p->dram_type))
        p->freq_sel = gFreqTbl[DRAM_DFS_SHUFFLE_3].freq_sel;
#endif
    // DramC & PHY init for all channels
    //===  First frequency ======
    DDRPhyFreqSel(p, p->freq_sel);
    vSetVcoreByFreq(p);
    DFSInitForCalibration(p);

    #if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
    U8 backup_broadcast;
    backup_broadcast = GetDramcBroadcast();
    EMI_Init2();
    DramcBroadcastOnOff(backup_broadcast);
    #endif
    O1Path_test_start(p);
#if (ENABLE_EMI_LPBK_TEST)
    if (gEmiLpbkTest)
    {
        mcSHOW_DBG_MSG(("\n\n\n=== saint SLT_Test_Main_Flow\n"));
        EMI_LPBK_memory_test(p);
		#if (!__ETT__)
		read_slt_data(&slt_data);
		slt_data.test_result[0] = 0;
		write_slt_data(&slt_data);
		#endif
        mcSHOW_DBG_MSG(("\nCold Reboot...\n"));
		pmic_config_interface(PMIC_RG_CRST_ADDR, 1, PMIC_RG_CRST_MASK, PMIC_RG_CRST_SHIFT);  //trigger full pmic reset
    }
#endif
}
#endif
//======================== SLT TEST Main Flow End ===========================================

