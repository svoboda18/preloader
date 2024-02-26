/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein is
 * confidential and proprietary to MediaTek Inc. and/or its licensors. Without
 * the prior written permission of MediaTek inc. and/or its licensors, any
 * reproduction, modification, use or disclosure of MediaTek Software, and
 * information contained herein, in whole or in part, shall be strictly
 * prohibited.
 * 
 * MediaTek Inc. (C) 2010. All rights reserved.
 * 
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER
 * ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 * INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES
 * TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO
 * OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN MEDIATEK
 * SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE
 * RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S
 * ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE
 * RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE
 * MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE
 * CHARGE PAID BY RECEIVER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek
 * Software") have been modified by MediaTek Inc. All revisions are subject to
 * any receiver's applicable license agreements with MediaTek Inc.
 */

#ifndef _DRAMC1_H
#define _DRAMC1_H

//========================
// Compile Option To Enable Functions
//========================
#define DRAM_BASE 0x40000000ULL
#define INFRA_MEM_DCM_CTRL       (INFRACFG_AO_BASE + 0x78)
//#define DDR_1333 1 //Science LPDDR3 Run in 1333MHz.

#define COMBO_MCP
//#define COMBO_LPDDR3 1
//#define COMBO_LPDDR2 1
//#define COMBO_PCDDR3 0

//#define GATING_CLOCK_CONTROL_DVT
// Macro definition.
/* These PLL config is defined in pll.h */
//#define DDRPHY_3PLL_MODE
//#define REAL_MEMPLL_MDL //no use

#define fcSWITCH_SPM_CONTROL
//#define fcENABLE_INTVREF
#define fcENABLE_FIX_REFCNT_ASYN
#define PHYSYNC_MODE

//tDQSCK related settings.
//#define BYPASS_DQSDUMMYPAD
#define TXDELAY_ENABLE
#define fcFIX_GATING_PRE_FAKEWIN
#define FINETUNE_CENTER
#define fcNEW_GATING_FINETUNE_LIMIT
#define fcGATING_SAME_MCK
//#define fcDATLAT_SAME_CHECK

//#define ENABLE_TX_CMD_CLK_SHIFT
//#define ENABLE_MRR

#define GW_TA2
#define REXTDN_ENABLE
#define ENABLE_REXTDN_APPLY
#define DQ_DQS_DQM_REMAPPING
#define TX_PERBIT_WIN

#define fcRESET_DDR_AFTER_CA_WR
#define WRITE_LEVELING
#define fcCA_TRAINING

// Run time config here
#define fcENABLE_RUNTIME_CONFIG // need to enable this first
    #define fcENABLE_HW_GATING
    #define fcENABLE_DISPBREFOVERTHD
    #define fcENABLE_EMIPERFORMANCE
    #define fcENABLE_DDRPHY_DCM_FUNC
    #define fcENABLE_DRAMC_DCM_FUNC
    #define fcENABLE_REF_BLOCK_EMI_ARBITRATION
    #define fcENABLE_GATING_COMPARE_FLAG
    #define fcENHANCE_VALID_DLY_FOR_DUAL_RANKS_TRACKING
    #define fcENHANCE_DDRPHY_DCMCTRL_EFFICIENCY
    #define ENHANCE_SREF_1DOT2US
    #define ENABLE_FIX_TXP_TMRRI_VIOLATION
    #define ENABLE_MR4
    #define fcENABLE_INFRA_MEM_DCM_CTRL

// only for test
#define READ_COMPARE_TEST
//#define FOR_TX_DELAY_CELL_MEASURE

/* To define DDRTYPE for bring up */
//#define DDRTYPE_LPDDR2	
//#define DDRTYPE_LPDDR3
//#define DDRTYPE_DDR3

//#define DERATING_ENABLE
#define ENABLE_DFS
//#define REPAIR_SRAM
//#define TX_DELAY_OVERCLOCK
//#define DRAMC_DEBUG

//#define HARDCODE_DRAM_SETTING
//#define FLASH_TOOL_PRELOADER
//#define DUMP_DRAMC_REGS

//#define H9TQ64A8GTMCUR_KUM
//#define KMQ8X000SA_B414
//#define KMQ7X000SA_B315

//#define VmFix_VcHV
//#define VmFix_VcNV
//#define VmFix_VcLV

//#define CUSTOM_CONFIG_MAX_DRAM_SIZE 0x3F000000
//#define ENABLE_SYNC_MASK

//#define WAVEFORM_MEASURE
//#define DRAM_INIT_CYCLES

//#define MAX_DRAM_DRIVING
//#define MAX_DRAMC_DRIVING

// End of Compile Option
//========================

#define TOPRGU_BASE	     RGU_BASE
#define TIMEOUT 3
#define CQ_DMA_BASE	0x10217c00

//=======================
/**********************/
/****mempll related****/
enum {
    PLL_MODE_1  = 1,
    PLL_MODE_2  = 2,
    PLL_MODE_3  = 3,
};
enum {
     DDR533   = 533,
     DDR800   = 800,
     DDR900   = 900,
     DDR938   = 938,
     DDR1066  = 1066,
     DDR1280  = 1280,
     DDR1313  = 1313,
     DDR1333  = 1333,
     DDR1466  = 1466,
     DDR1600  = 1600,
};

#if defined(MACH_TYPE_MT6737T)

#define DDRPHY_3PLL_MODE

#ifdef DDRPHY_3PLL_MODE
#define PHYSYNC_MODE
#define DDRPHY_2PLL
#endif

#define DDR_1466

#else

#if 0
#define DRAMC_ASYNC
#endif

#if 0
#define DDRPHY_3PLL_MODE
#endif


#ifdef DDRPHY_3PLL_MODE
#define PHYSYNC_MODE
#if 1
#define DDRPHY_2PLL
#endif
#endif

#if 0
#define DDR_533
#endif
#if 0
#define DDR_800
#endif
#if 1
#define DDR_1333
#endif
#endif	// End of #if defined(MACH_TYPE_MT6737)
/**** end mempll related****/
/****************************/

typedef struct {
    char *name;
    char **factor_tbl;
    char *curr_val;
    char *opt_val;
    void (*factor_handler) (char *);
} tuning_factor;

typedef struct {
    void (*ett_print_banner) (unsigned int);
    void (*ett_print_before_start_loop_zero) (void);
    void (*ett_print_before_each_round_of_loop_zero) (void);
    unsigned int (*ett_print_result) (void);
    void (*ett_print_after_each_round_of_loop_zero) (void);
    void (*ett_calc_opt_value) (unsigned int, unsigned int *, unsigned int *);
    void (*ett_print_after_finish_loop_n) (int);
} print_callbacks;

#define ETT_TUNING_FACTOR_NUMS(x)	(sizeof(x)/sizeof(tuning_factor))

typedef struct {
    int (*test_case) (unsigned int, unsigned int, void *);
    unsigned int start;
    unsigned int range;
    void *ext_arg;
} test_case;
#if defined(MT6739)
#define DRAMC_WRITE_REG(val,offset)  do{ \
                                      (*(volatile unsigned int *)(DRAMC0_BASE + (offset))) = (unsigned int)(val); \
                                      (*(volatile unsigned int *)(DDRPHY_BASE + (offset))) = (unsigned int)(val); \
                                      (*(volatile unsigned int *)(DRAMC_NAO_BASE + (offset))) = (unsigned int)(val); \
                                      }while(0)

#define DRAMC_WRITE_REG_W(val,offset)     do{ \
                                      (*(volatile unsigned int *)(DRAMC0_BASE + (offset))) = (unsigned int)(val); \
                                      (*(volatile unsigned int *)(DDRPHY_BASE + (offset))) = (unsigned int)(val); \
                                      (*(volatile unsigned int *)(DRAMC_NAO_BASE + (offset))) = (unsigned int)(val); \
                                      }while(0)

#define DRAMC_WRITE_REG_H(val,offset)     do{ \
                                      (*(volatile unsigned short *)(DRAMC0_BASE + (offset))) = (unsigned short)(val); \
                                      (*(volatile unsigned short *)(DDRPHY_BASE + (offset))) = (unsigned short)(val); \
                                      (*(volatile unsigned short *)(DDRPHY_BASE + (offset))) = (unsigned short)(val); \
                                      }while(0)
#define DRAMC_WRITE_REG_B(val,offset)     do{ \
                                      (*(volatile unsigned char *)(DRAMC0_BASE + (offset))) = (unsigned char)(val); \
                                      (*(volatile unsigned char *)(DDRPHY_BASE + (offset))) = (unsigned char)(val); \
                                      (*(volatile unsigned char *)(DDRPHY_BASE + (offset))) = (unsigned char)(val); \
                                      }while(0)
#define DRAMC_READ_REG(offset)         ( \
                                        (*(volatile unsigned int *)(DRAMC0_BASE + (offset))) |\
                                        (*(volatile unsigned int *)(DDRPHY_BASE + (offset))) |\
                                        (*(volatile unsigned int *)(DRAMC_NAO_BASE + (offset))) \
                                       )
#define DRAMC_WRITE_SET(val,offset)     do{ \
                                      (*(volatile unsigned int *)(DRAMC0_BASE + (offset))) |= (unsigned int)(val); \
                                      (*(volatile unsigned int *)(DDRPHY_BASE + (offset))) |= (unsigned int)(val); \
                                      (*(volatile unsigned int *)(DRAMC_NAO_BASE + (offset))) |= (unsigned int)(val); \
                                      }while(0)

#define DRAMC_WRITE_CLEAR(val,offset)     do{ \
                                      (*(volatile unsigned int *)(DRAMC0_BASE + (offset))) &= ~(unsigned int)(val); \
                                      (*(volatile unsigned int *)(DDRPHY_BASE + (offset))) &= ~(unsigned int)(val); \
                                      (*(volatile unsigned int *)(DRAMC_NAO_BASE + (offset))) &= ~(unsigned int)(val); \
                                      }while(0)

#define DDRPHY_WRITE_REG(val,offset)    __raw_writel(val, (DDRPHY_BASE + (offset)))
#define DRAMC0_WRITE_REG(val,offset)    __raw_writel(val, (DRAMC0_BASE + (offset)))
#define DRAMC_NAO_WRITE_REG(val,offset) __raw_writel(val, (DRAMC_NAO_BASE + (offset)))
#define MCUSYS_CFGREG_WRITE_REG(val,offset) __raw_writel(val, (MCUSYS_CFGREG_BASE + (offset)))
#else

#endif

#if 1//#if defined(WRITE_LEVELING) || defined(fcCA_TRAINING)
// DRAMC registers
#define DRAMC_REG_ACTIM0 0x000
    #define POS_ACTIM0_BL2 15
#define DRAMC_REG_CONF1 0x004
    #define POS_CONF1_BL4 10
    #define POS_CONF1_SELFREF 26
    #define POS_CONF1_TESTLP 27    
#define DRAMC_REG_CONF2 0x008
    #define POS_CONF2_TEST1 29
    #define MASK_CONF2_TE12_ENABLE 0xe0000000
#define DRAMC_REG_PADCTL1 0x00c
#define DRAMC_REG_PADCTL2 0x010
#define DRAMC_REG_PADCTL3 0x014
#define DRAMC_REG_DELDLY1 0x018
#define DRAMC_REG_01C 0x01c
#define DRAMC_REG_DLLCONF 0x028
#define DRAMC_REG_TEST2_0 0x038   // new on MT5399
#define DRAMC_REG_TEST2_1 0x03C
#define DRAMC_REG_TEST2_2 0x040
#define DRAMC_REG_TEST2_3 0x044
    #define POS_TEST2_3_ADVREFEN 30
    #define POS_TEST2_3_TESTAUDPAT 7
    // bit0~3
    #define MASK_TEST2_3_TESTCNT 0x0000000f
    #define POS_TEST2_3_TESTCNT 0
#define DRAMC_REG_TEST2_4 0x048
    // bit0~4
    #define MASK_TEST2_4_TESTAUDINC 0x0000001f
    #define POS_TEST2_4_TESTAUDINC 0
    // bit8~12
    #define MASK_TEST2_4_TESTAUDINIT 0x00001f00
    #define POS_TEST2_4_TESTAUDINIT 8
    // bit 14, 15
    #define POS_TEST2_4_TESTAUDBITINV 14
    #define POS_TEST2_4_TESTAUDMODE 15
    //bit 16
    #define POS_TEST2_4_TESTXTALKPAT 16
#define DRAMC_REG_DDR2CTL 0x07c
    #define MASK_DDR2CTL_DATLAT 0x00000070
    #define POS_DDR2CTL_DTALAT 4
    #define POS_DDR2CTL_WOEN 3
#define DRAMC_REG_084 0x084
#define DRAMC_REG_MRS 0x088
#define DRAMC_REG_CLK1DELAY 0x08c
    #define POS_CLK1DELAY_DQCMD 3
#define DRAMC_REG_IOCTL 0x090
#define DRAMC_REG_DQSIEN 0x094
    #define MASK_DQSIEN_R0DQS0IEN 0x0000007f
    #define MASK_DQSIEN_R0DQS1IEN 0x00007f00
    #define MASK_DQSIEN_R0DQS2IEN 0x007f0000
    #define MASK_DQSIEN_R0DQS3IEN 0x7f000000
    #define POS_DQSIEN_R0DQS0IEN 0
    #define POS_DQSIEN_R0DQS1IEN 8
    #define POS_DQSIEN_R0DQS2IEN 16
    #define POS_DQSIEN_R0DQS3IEN 24
#define DRAMC_REG_IODRV4 0x0b0
    #define MASK_IODRV4_DQDRVN_2 0x00000f00
    #define MASK_IODRV4_DQDRVP_2 0x0000f000
    #define MASK_IODRV4_CMDDRVN_2 0x0f000000
    #define MASK_IODRV4_CMDDRVP_2 0xf0000000
    #define POS_IODRV4_DQDRVN_2 8
    #define POS_IODRV4_DQDRVP_2 12
    #define POS_IODRV4_CMDDRVN_2 24
    #define POS_IODRV4_CMDDRVP_2 28
#define DRAMC_REG_IODRV5 0x0b4
    #define MASK_IODRV5_DQCDRVN 0x00000f00
    #define MASK_IODRV5_DQCDRVP 0x0000f000
    #define MASK_IODRV5_DQCDRVN_2 0x0f000000
    #define MASK_IODRV5_DQCDRVP_2 0xf0000000
    #define POS_IODRV5_DQCDRVN 8
    #define POS_IODRV5_DQCDRVP 12
    #define POS_IODRV5_DQCDRVN_2 24
    #define POS_IODRV5_DQCDRVP_2 28
#define DRAMC_REG_DRVCTL0 0x0b8
    #define MASK_DRVCTL0_DQDRVN 0x00000f00
    #define MASK_DRVCTL0_DQDRVP 0x0000f000
    #define MASK_DRVCTL0_DQSDRVN 0x0f000000
    #define MASK_DRVCTL0_DQSDRVP 0xf0000000
    #define POS_DRVCTL0_DQDRVN 8
    #define POS_DRVCTL0_DQDRVP 12
    #define POS_DRVCTL0_DQSDRVN 24
    #define POS_DRVCTL0_DQSDRVP 28
#define DRAMC_REG_DRVCTL1 0x0bc
    #define MASK_DRVCTL1_CMDDRVN 0x00000f00
    #define MASK_DRVCTL1_CMDDRVP 0x0000f000
    #define MASK_DRVCTL1_CLKDRVN 0x0f000000
    #define MASK_DRVCTL1_CLKDRVP 0xf0000000
    #define POS_DRVCTL1_CMDDRVN 8
    #define POS_DRVCTL1_CMDDRVP 12
    #define POS_DRVCTL1_CLKDRVN 24
    #define POS_DRVCTL1_CLKDRVP 28
#define DRAMC_REG_MCKDLY 0x0d8
    //bit 23
    #define POS_MCKDLY_FIXODT 23
    //bit 12~15
    #define POS_MCKDLY_FIXDQIEN 12
    #define MASK_MCKDLY_FIXDQIEN 0x0000f000
#define DRAMC_REG_DQSCTL0 0x0dc
#define DRAMC_REG_DQSCTL1 0x0e0
    #define POS_DQSCTL1_DQSIENMODE 28
    #define MASK_DQSCTL1_DQSINCTL 0x0f000000   // for A60808, 4 bits totally
    #define POS_DQSCTL1_DQSINCTL 24
    #define POS_DQSCTL1_DQS2CTL 0   // [3:0] A60808: HW run-time DATLAT
    #define MASK_DQSCTL1_DQS2CTL 0x0000000f
#define DRAMC_REG_PADCTL4 0x0e4
    #define MASK_PADCTL4_DATLAT3 0x00000010
    #define POS_PADCTL4_DATLAT3 4
    #define POS_PADCTL4_CKEFIXON 2
#define DRAMC_REG_PHYCTL1 0x0f0
    #define POS_PHYCTL1_PHYRST 28
#define DRAMC_REG_GDDR3CTL1 0x0f4
    #define POS_GDDR3CTL1_DQMSWAP 31
    #define POS_GDDR3CTL1_RDATRST 25
    #define POS_GDDR3CTL1_BKSWAP 20
#define DRAMC_REG_PADCTL7 0x0f8
#define DRAMC_REG_MISCTL0 0x0fc
#define DRAMC_REG_OCDK 0x100
    #define POS_OCDK_DRVREF 24
#define DRAMC_REG_RKCFG 0x110
    #define POS_RKCFG_WDATKEY64 29   //32-bit channel it is 0, for 16-bit or asym DRAM it is 1
#define DRAMC_REG_CKPHDET 0x114
#define DRAMC_REG_DQ_DQS01_SEL 0x120
#define DRAMC_REG_DQSGCTL 0x124
    #define MASK_DQSGCTL_R0DQSG_COARSE_DLY_COM0 0x00000003
    #define MASK_DQSGCTL_R0DQSG_COARSE_DLY_COM1 0x00000030
    #define POS_DQSGCTL_R0DQSG_COARSE_DLY_COM0 0
    #define POS_DQSGCTL_R0DQSG_COARSE_DLY_COM1 4
    #define POS_DQSGCTL_DQSGDUALP 30
#define DRAMC_REG_CLKENCTL 0x130
#define DRAMC_REG_WRLEV 0x13c
    //bit 12
    #define POS_WRLEV_DQS_SEL 12
    #define MASK_WRLEV_DQS_SEL 0x0000f000
    //bit 8
    #define POS_WRLEV_DQS_WLEV 8
    //bit 1~4
    #define POS_WRLEV_DQS_Bx_G 1
    #define MASK_WRLEV_DQS_Bx_G 0x0000001e
    //bit 0
    #define POS_WRLEV_WRITE_LEVEL_EN 0
#define DRAMC_REG_158 0x158
#define DRAMC_REG_ARBCTL0 0x168
#define DRAMC_REG_CMDDLY0 0x1a8
#define DRAMC_REG_DQSCAL0 0x1c0
#define DRAMC_REG_IMPCAL 0x1c8
    #define POS_IMPCAL_IMPCALI_EN 0
    #define POS_IMPCAL_IMPCAL_HW 1
    #define POS_IMPCAL_IMP_CALI_ENN 4
    #define POS_IMPCAL_IMP_CALI_ENP 5
    #define POS_IMPCAL_IMPPDN 6
    #define POS_IMPCAL_IMPPDP 7    
    #define POS_IMPCAL_IMPDRVP 8
    #define MASK_IMPCAL_IMPDRVP 0x00000f00
    #define POS_IMPCAL_IMPDRVN 12
    #define MASK_IMPCAL_IMPDRVN 0x0000f000
#define DRAMC_REG_DMMonitor 0x1d8
#define DRAMC_REG_DRAMC_PD_CTRL 0x1dc
    #define MASK_DRAMC_PD_CTRL_REFCNT_FR_CLK 0x00ff0000
    #define POS_DRAMC_PD_CTRL_REFCNT_FR_CLK 16
    #define POS_DRAMC_PD_CTRL_MIOCKCTRLOFF 26
#define DRAMC_REG_LPDDR2 0x1e0
    #define POS_LPDDR2_ADRDECEN 31
    #define POS_LPDDR2_SELO1ASO 30
#define DRAMC_REG_SPCMD 0x1e4
    #define POS_SPCMD_MRWEN 0
    #define POS_SPCMD_DQSGCNTEN 8
    #define POS_SPCMD_DQSGCNTRST 9
#define DRAMC_REG_ACTIM1 0x1e8
#define DRAMC_REG_PERFCTL0 0x1ec
#define DRAMC_REG_LPDDR2_4 0x1f8
#define DRAMC_REG_DQODLY1 0x200
#define DRAMC_REG_DQODLY2 0x204
#define DRAMC_REG_DQODLY3 0x208
#define DRAMC_REG_DQODLY4 0x20c
#define DRAMC_REG_DQIDLY1 0x210
#define DRAMC_REG_DQIDLY2 0x214
#define DRAMC_REG_DQIDLY3 0x218
#define DRAMC_REG_DQIDLY4 0x21c
#define DRAMC_REG_DQIDLY5 0x220
#define DRAMC_REG_DQIDLY6 0x224
#define DRAMC_REG_DQIDLY7 0x228
#define DRAMC_REG_DQIDLY8 0x22c
#define DRAMC_REG_DMACON 0x234
    #define POS_DMACON_DMAEN 0
    #define POS_DMACON_DMAPUREWRITEEN 1
    #define POS_DMACON_DMAPUREREADEN 2
    #define POS_DMACON_DMABURSTLEN 4
    #define MASK_DMACON_DMABURSTLEN 0x00000070
#define DRAMC_REG_DMASRCADDR 0x238
#define DRAMC_REG_DMADSTADDR 0x23c
#define DRAMC_REG_DMATRANSLEN 0x240
    #define POS_DMATRANSLEN_DMATRANSLEN 0
    #define MASK_DMATRANSLEN_DMATRANSLEN 0x000fffff
#define DRAMC_REG_DMAPAT 0x244
#define DRAMC_REG_DMAMON 0x254
    #define POS_DMAMON_DMASTATUS 0
#define DRAMC_REG_DMACMPERR 0x258
#define DRAMC_REG_R2R_page_hit_counter 0x280
#define DRAMC_REG_STBENERR_R 0x320
#define DRAMC_REG_STBENERR_F 0x324
#define DRAMC_REG_TOGGLE_CNT 0x32c
#define DRAMC_REG_DQS_ERR_CNT 0x330
#define DRAMC_REG_DQ_ERR_CNT 0x334
#define DRAMC_REG_SELPH 0x344
#define DRAMC_REG_DCBLN 0x348
    #define POS_DCBLN_RSTBCNT_LATCH_EN 11
    #define POS_DCBLN_RX_MIOCK_JIT_EN 2
    #define POS_DCBLN_RX_EYE_SCAN_EN 1
    #define POS_DCBLN_REG_SW_RST 0
#define DRAMC_REG_TOGGLE_CNT_2 0x360
#define DRAMC_REG_DQS_ERR_CNT_2 0x364
#define DRAMC_REG_DQ_ERR_CNT_2 0x368
#define DRAMC_REG_CMP_ERR 0x370
#define DRAMC_REG_DQSDLY0 0x3a0
#define DRAMC_REG_SPCMDRESP 0x3b8
    #define POS_SPCMDRESP_SREF_STATE 16
#define DRAMC_REG_DQSGNWCNT0 0x3c0
#define DRAMC_REG_DQSGNWCNT1 0x3c4
#define DRAMC_REG_DLLSTATUS0 0x3dc
    #define POS_DLLSTATUS0_CMPOT 31
    #define POS_DLLSTATUS0_CMPOUTN 30
#define DRAMC_REG_TESTRPT 0x3fc
    #define POS_TESTRPT_DM_CMP_CPT 10
    #define POS_TESTRPT_DM_CMP_ERR 14
#define DRAMC_REG_SELPH1 0x400
#define DRAMC_REG_SELPH2 0x404
    #define MASK_SELPH2_TXDLY_DQSGATE 0x00007000
    #define POS_SELPH2_TXDLY_DQSGATE 12
    #define MASK_SELPH2_TXDLY_DQSGATE_P1 0x00700000
    #define POS_SELPH2_TXDLY_DQSGATE_P1 20
#define DRAMC_REG_SELPH3 0x408
#define DRAMC_REG_SELPH4 0x40C    
#define DRAMC_REG_SELPH5 0x410
    #define MASK_SELPH5_dly_DQSGATE 0x00c00000
    #define POS_SELPH5_dly_DQSGATE 22
    #define MASK_SELPH5_dly_DQSGATE_P1 0x03000000
    #define POS_SELPH5_dly_DQSGATE_P1 24
#define DRAMC_REG_SELPH6 0x414
#define DRAMC_REG_SELPH7 0x41c
#define DRAMC_REG_SELPH8 0x420
#define DRAMC_REG_SELPH9 0x424
#define DRAMC_REG_SELPH10 0x428
#define DRAMC_REG_SELPH11 0x42c
#define DRAMC_REG_SELPH12 0x430
#define DRAMC_REG_SELPH13 0x434
#define DRAMC_REG_SELPH14 0x438
#define DRAMC_REG_SELPH15 0x43c
#define DRAMC_REG_SELPH16 0x440
#define DRAMC_REG_COM0_DLY_SEL_DGTED 0x478
#define DRAMC_REG_COM0_DLY_SEL_DQS0 0x47C
#define DRAMC_REG_COM0_DLY_SEL_DQS1 0x480
#define DRAMC_REG_COM0_DLY_SEL_DQM0 0x484
#define DRAMC_REG_COM0_DLY_SEL_DQ2 0x494
#define DRAMC_REG_COM0_DLY_SEL_DQ3 0x498
#define DRAMC_REG_COM0_DLY_SEL_DQ4 0x49c
#define DRAMC_REG_COM0_DLY_SEL_DQ5 0x4a0
#define DRAMC_REG_COM0_DLY_SEL_DQ6 0x4a4
#define DRAMC_REG_COM0_DLY_SEL_DQ7 0x4a8
#define DRAMC_REG_COM1_DLY_SEL_DGTED 0x4CC
#define DRAMC_REG_COM1_DLY_SEL_DQS0 0x4D0
#define DRAMC_REG_COM1_DLY_SEL_DQS1 0x4D4
#define DRAMC_REG_COM1_DLY_SEL_DQM0 0x4D8
#define DRAMC_REG_COM1_DLY_SEL_DQ0 0x4e0
#define DRAMC_REG_COM1_DLY_SEL_DQ2 0x4e8
#define DRAMC_REG_COM1_DLY_SEL_DQ3 0x4ec
#define DRAMC_REG_COM1_DLY_SEL_DQ4 0x4f0
#define DRAMC_REG_COM1_DLY_SEL_DQ5 0x4f4
#define DRAMC_REG_COM1_DLY_SEL_DQ6 0x4f8
#define DRAMC_REG_COM1_DLY_SEL_DQ7 0x4fc
#define DRAMC_REG_PH_EN 0x520
#define DRAMC_REG_DLY_SEL_MCK_ANA 0x524
#define DRAMC_REG_DLY_SEL_CLKGEN 0x528
#define DRAMC_REG_DLY_SEL_CLKGEN2 0x52C
#endif

#define ETT_TEST_CASE_NUMS(x)	(sizeof(x)/sizeof(test_case))

#define GRAY_ENCODED(a) (a)

#ifndef NULL
#define NULL    0
#endif

#define delay_a_while(count) \
        do {    \
           register unsigned int delay;        \
           asm volatile ("dsb":::"memory");    \
           asm volatile ("mov %0, %1\n\t"      \
                         "1:\n\t"              \
                         "subs %0, %0, #1\n\t" \
                         "bne 1b\n\t"          \
                         : "+r" (delay)        \
                         : "r" (count)         \
                         : "cc"); \
        } while (0)

#define DDR_PHY_RESET() do { \
} while(0)
#define DDR_PHY_RESET_NEW() do { \
    DRAMC_WRITE_REG((DRAMC_READ_REG(DRAMC_PHYCTL1)) \
		| (1 << 28), \
		DRAMC_PHYCTL1); \
    DRAMC_WRITE_REG((DRAMC_READ_REG(DRAMC_GDDR3CTL1)) \
		| (1 << 25),	\
		DRAMC_GDDR3CTL1); \
    delay_a_while(1000); \
    DRAMC_WRITE_REG((DRAMC_READ_REG(DRAMC_PHYCTL1)) \
		& (~(1 << 28)),	\
		DRAMC_PHYCTL1); \
    DRAMC_WRITE_REG((DRAMC_READ_REG(DRAMC_GDDR3CTL1)) \
		& (~(1 << 25)),	\
		DRAMC_GDDR3CTL1); \
} while(0)

/* define supported DRAM types */
enum
{
  TYPE_mDDR = 1,
  TYPE_LPDDR2,
  TYPE_LPDDR3,
  TYPE_PCDDR3,
  TYPE_LPDDR4,
};

extern void pmic_voltage_read(unsigned int nAdjust);
extern void pmic_Vcore_adjust(int nAdjust);
extern void pmic_Vmem_adjust(int nAdjust);
extern void pmic_Vmem_Cal_adjust(int nAdjust);
extern void pmic_HQA_NoSSC_Voltage_adjust(int nAdjust);
extern void pmic_HQA_Voltage_adjust(int nAdjust);
extern void pmic_force_PWM_Mode(void);

#endif  /* !_DRAMC1_H */
