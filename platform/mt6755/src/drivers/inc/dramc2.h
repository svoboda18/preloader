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

#ifndef _DRAMC2_H
#define _DRAMC2_H

//========================
// Compile Option To Enable Functions
//========================
#define DRAM_BASE 0x40000000ULL
//#define DDR_1333 1 //Science LPDDR3 Run in 1333MHz.

#define COMBO_MCP
//#define COMBO_LPDDR3 1
//#define COMBO_LPDDR2 1
//#define COMBO_PCDDR3 0

//#define GATING_CLOCK_CONTROL_DVT
// Macro definition.
/* These PLL config is defined in pll.h */
//#define DDRPHY_3PLL_MODE
#define REAL_MEMPLL_MDL

#define fcSWITCH_SPM_CONTROL
//#define fcENABLE_INTVREF
#define fcENABLE_FIX_REFCNT_ASYN

//tDQSCK related settings.
#define BYPASS_DQSDUMMYPAD
//#define TXDELAY_ENABLE

//#define ENABLE_TX_CMD_CLK_SHIFT
//#define ENABLE_MRR

#define GW_TA2
#define REXTDN_ENABLE
#define ENABLE_REXTDN_APPLY
//#define WRITE_LEVELING
#define DQ_DQS_DQM_REMAPPING
#define TX_PERBIT_WIN

// Run time config here
#define fcENABLE_RUNTIME_CONFIG // need to enable this first
    #define fcENABLE_HW_GATING
    #define fcENABLE_DISPBREFOVERTHD
    #define fcENABLE_EMIPERFORMANCE
    #define fcENABLE_DDRPHY_DCM_FUNC
    #define fcENABLE_DRAMC_DCM_FUNC
    #define fcENABLE_REF_BLOCK_EMI_ARBITRATION

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

//#define pmic_HQA_TCs
//#define WAVEFORM_MEASURE
//#define DRAM_INIT_CYCLES

// End of Compile Option
//========================

#define TOPRGU_BASE	     RGU_BASE
#define TIMEOUT 3
#define CQ_DMA_BASE	0x10217c00

// EMI address definition.
#define EMI_CONA 		((volatile unsigned int *)(EMI_BASE+0x000))
#define EMI_CONB		((volatile unsigned int *)(EMI_BASE+0x008))
#define EMI_CONC		((volatile unsigned int *)(EMI_BASE+0x010))
#define EMI_COND		((volatile unsigned int *)(EMI_BASE+0x018))
#define EMI_CONE		((volatile unsigned int *)(EMI_BASE+0x020))
#define EMI_CONF                ((volatile unsigned int *)(EMI_BASE+0x028))
#define EMI_CONG		((volatile unsigned int *)(EMI_BASE+0x030))
#define EMI_CONH		((volatile unsigned int *)(EMI_BASE+0x038))
#define EMI_CONI		((volatile unsigned int *)(EMI_BASE+0x040))
#define EMI_CONM		((volatile unsigned int *)(EMI_BASE+0x060))
#define EMI_DFTB		((volatile unsigned int *)(EMI_BASE+0x0E8))		// TESTB
#define EMI_DFTD		((volatile unsigned int *)(EMI_BASE+0x0F8))		// TESTD
#define EMI_SLCT		((volatile unsigned int *)(EMI_BASE+0x158))
#define EMI_ARBA		((volatile unsigned int *)(EMI_BASE+0x100))
#define EMI_ARBB		((volatile unsigned int *)(EMI_BASE+0x108))
#define EMI_ARBC		((volatile unsigned int *)(EMI_BASE+0x110))
#define EMI_ARBD		((volatile unsigned int *)(EMI_BASE+0x118))
#define EMI_ARBE		((volatile unsigned int *)(EMI_BASE+0x120))
#define EMI_ARBF		((volatile unsigned int *)(EMI_BASE+0x128))
#define EMI_ARBG		((volatile unsigned int *)(EMI_BASE+0x130))
#define EMI_ARBI		((volatile unsigned int *)(EMI_BASE+0x140))
#define EMI_BMEN		((volatile unsigned int *)(EMI_BASE+0x400))

//=======================

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
#if defined(MT6735)
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
};

extern void pmic_voltage_read(unsigned int nAdjust);
extern void pmic_Vcore_adjust(int nAdjust);
extern void pmic_Vmem_adjust(int nAdjust);
extern void pmic_Vmem_Cal_adjust(int nAdjust);
extern void pmic_HQA_NoSSC_Voltage_adjust(int nAdjust);
extern void pmic_HQA_Voltage_adjust(int nAdjust);
extern void pmic_force_PWM_Mode(void);

#endif  /* !_DRAMC2_H */
