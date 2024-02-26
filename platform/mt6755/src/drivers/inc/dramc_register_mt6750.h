//
//  REGISTER.H  Register map include file
//

#ifndef _MT6755_REGISTER_MT6750_H_
#define _MT6755_REGISTER_MT6750_H_

//Bring Up Selection : Do Not open it when normal operation
//#define FIRST_BRING_UP

//DRAMC Chip 
#define fcA60808 	0
#define fcMT6595 	1
#define fcK2		2
#define fcJADE	 3
#define fcFOR_CHIP_ID fcJADE

//#define JADE_FLOW_RUN_ON_K2_EVB //test JADE flow on K2 EVB used
//#define ONE_PLL_MODE_INIT

//Defined for DRAM FT
//#define DDR_FT_LOAD_BOARD


//Feature option
#define DUAL_RANKS
#define SINGLE_CHANNEL_ENABLE	// Boot-up debugging.
//#define RX_DUTY_CALIBRATION	// Clock Duty Calibration
#define HW_GATING				// DQS strobe calibration enable
#define FINETUNE_CENTER			// Find the center of fine tune most close to the DQS_GW_FINE_END/2
#define CKE_CS_DLY_SETTING		// set CKE delay and CS delay in CATraining & Write Leveling
#define DRAMC_DRAMPHY_DCM_ENABLE	//Enable Dramc/DramPhy DCM

#define TEMP_SENSOR_ENABLE		// K2?? Disable in bring-up and enable thereafter.
#define ZQCS_ENABLE				// K2?? Disable in bring-up and enable thereafter.
#define EMI_OPTIMIZATION			// K2?? Disable in bring-up and enable thereafter.
#define SPM_CONTROL_AFTERK		// K2?? Disable in bring-up and enable thereafter.
#define MEMPLL_NEW_POWERON
#define MAX_DCMDLYREF		// SY comment need to set this.
//#define FRCLK_26M_DIV8
#define MAX_SREF_CK_DLY		// SY comment need to set this.		
#define DQM_SHIFT	
//#define ENABLE_DQSI_PER_RANK


#define TWPST_ENLARGE


//#define ENABLE_REDUCE_CLKTOG_REF	// This feature has bug.
#define RWSPLIT_ENABLE
//#define MATYPE_ADAPTATION
//#define LPDDR_MAX_DRIVE
//#define LPDDR_MIN_DRIVE
//#define tREFI_DIV4_MANUAL
//#define tREFI_DIV2_MANUAL

// CLKTDN=1. Extend clk low duty.
//#define CLKTDN_ENABLE				// K2?? Need to check.

//#define MAX_DRIVING
#define CATRAINING_STEP	0
#define MAX_CLKO_DELAY  15
#define CATRAINING_NUM  10





#define LOCK_ON_1PLL_3PLL			// mempll settings of 1pll/3pll are the same, for switch frequence quickly because no need to lock clk.
#define DUAL_FREQ_HIGH	833
#define DUAL_FREQ_LOW   650

//#define DUAL_FREQ_TEST




//#define Vcore_ADJ_FOR_DVT        // Enable : 1pll 0.9V and 3pll 1.0V    Disable : 1pll 1.0V and 3pll 1.0V

// Test option
//#define SEQ_FREERUN
//#define DDRCLK_NOGATING
//#define CKE_FIXON
#define DISABLE_FASTWAKE	//JADE open for DerPing's suggestion
//#define DQM_CALIBRATION
//#define TDP_TDN_CAL

//#define CPUTEST
//#define TA2_STRESS
#define XTALK_SSO_STRESS
#define NEW_WORST_PAT_64
#define IDD4_PATTERN_STRESS

//#define XTALK_SSO_K
//#define WAVEFORM_MEASURE
//#define FIX_DLE_19
#define DMA_BURST_LEN 	7		// 7:7x8. 3:4x8
//#define DDR_INIT_TIME_PROFILING
//#define EYE_SCAN
//#define ACDIO_TEST
//#define SUSPEND_TEST
#define READ_COMPARE_TEST

#define ADJUST_CROSS_RANK_TIMING	// make cross rank timing better  // SY comment need to set this.
#define INCREASE_ZQCSCNT_AND_REFRCNT_RANGE	//ZQCSCNT[7:0] -> ZQCSCNT[8:0], REFRCNT[7:0] -> REFRCNT[8:0]
#define DDRPHY_BYPASS_DMYPAD					//DQSIEN bypass Dummy Pad
#define DDRPHY_BYPASS_NEW_DMYPAD				//bypadd New Dummy Pad
//#define AP_SLEEP_WORK_AROUND					//Auto_save+Auto-restore
#define ENABLE_MEMPLL_CG_TO_SAVE_POWER
#define ENABLE_MENGRU_SUGGEST_SETTING			//MengRu's setting of JADE
#define MRW_WAIT_RESPONSE						//MRW checks write command finishing by response
//#define USE_COARSE_TUNE_ALIGN_UPDATE
//#define SPM_WORKAROUND_CATRAINING
#define CA_TRAINING_NEW_FLOW
//#define NORMAL_CMD_SELPH                // cmd_selph = simulation's value
//#define CUSTOMIZE_COARSE_TUNE
//#define SINGLE_RANK_DMA
//#define RANK1_TEST
//#define DR_QUICKENDTEST
#define MEM_27_GRID
#if __ETT__
#define VOLTAGE_1600
//#define VOLTAGE_1866
#else
#define VIO18_ADJUST
#endif
//#define FTTEST_ZQONLY
#define DMA_ERROR_DISPLAY
#define DMA_ERROR_STOP
//#define DUMMY_READ
//#define TUNE_RX_TX_FACTORS
//#define WL_TEST
//#define LOOPBACK_TEST
//#define TEST_DFS_DMABUSY	2	// 1 : before shuffle enable. 2 : after shuffle enable.
//#define fcMEMPLL_DBG_MONITOR
//#define MEMPLL_BAND_INFO




/**************************  *********************/
#define CHA_DRAMCAO_BASE	0x10004000
#define CHA_DDRPHY_BASE		0x1000F000
#define CHA_DRAMCNAO_BASE	0x1020E000
#define DDR_BASE			0x40000000
#define EMI_APB_BASE 		0x10203000
#define INFRA_BASE			0x10001000
#define MEM_DCM_CTRL		(INFRA_BASE + 0x078)
#define DFS_MEM_DCM_CTRL	(INFRA_BASE + 0x07c)
#define CLK_CFG_0			(0x10000040)
#define CLK_CFG_0_CLR		(0x10000048)
#define CLK_CFG_0_SET		(0x10000044)
#define CLK_CFG_UPDATE		(0x10000004)
#define SPM_POWER_ON_VAL0 	(0x10006004)
#define PCM_INI_PWRON0_REG	(0x10006010)
#if fcFOR_CHIP_ID == fcK2
#define DLE_DLEDSEL_DIFF	6
#endif
#if fcFOR_CHIP_ID == fcJADE
#define DLE_DLEDSEL_DIFF	4
#endif



#if fcFOR_CHIP_ID == fcK2
#define	LPDDR3_ACTIM_000_LOW			0x22164340
#define 	LPDDR3_GDDR3CTL1_0F4_LOW	0x11000000
#define 	LPDDR3_CONF1_004_LOW		0x00048443
#define 	LPDDR3_DDR2CTL_07C_LOW		0x000031B1
#define 	LPDDR3_MODE_REG_1_LOW		0x00030001		// nWR = 10
#define 	LPDDR3_MODE_REG_2_LOW		0x00180002		// MR2_OP6=0, OP4=1, RL=10,WL=6
#define	LPDDR3_TEST2_3_044_LOW			0xBFCC0401
#ifdef FRCLK_26M_DIV8
#define 	LPDDR3_CONF2_008_LOW		0x0080172C
#define	LPDDR3_PD_CTRL_1DC_LOW			0xD10B2742
#else
#define 	LPDDR3_CONF2_008_LOW		0x0080B02C
#define	LPDDR3_PD_CTRL_1DC_LOW			0xD1642742
#endif
#define	LPDDR3_ACTIM1_1E8_LOW			0x90000610
#define	LPDDR3_AC_TIME_05T_1f8_LOW		0x000021C4
#ifdef ENABLE_REDUCE_CLKTOG_REF
#define	LPDDR3_RKCFG_110_LOW			0x00212541
#else
#define	LPDDR3_RKCFG_110_LOW			0x00612541
#endif
#define	LPDDR3_TEST2_4_048_LOW			0x1C01110D
#define	LPDDR3_MISCTL0_VAL_0fc_LOW		0x07000000
#endif

#if fcFOR_CHIP_ID == fcJADE
#define	LPDDR3_ACTIM_000_LOW			0x558a45c6 
#define 	LPDDR3_GDDR3CTL1_0F4_LOW	0x11000000
#define 	LPDDR3_CONF1_004_LOW		0x000484a3 
#define 	LPDDR3_DDR2CTL_07C_LOW		0x000052a1 
#define 	LPDDR3_MODE_REG_1_LOW		0x00830001 //0x00030001		// nWR = 10
#define 	LPDDR3_MODE_REG_2_LOW		0x00180002		// MR2_OP6=0, OP4=1, RL=10,WL=6
#define	LPDDR3_TEST2_3_044_LOW			0xbfc90480 
#define 	LPDDR3_CONF2_008_LOW		0x11c0b04e 
#define	LPDDR3_PD_CTRL_1DC_LOW			0xd3644542 
#define	LPDDR3_ACTIM1_1E8_LOW			0x95001230 
#define	LPDDR3_AC_TIME_05T_1f8_LOW		0x0000343f 
#define	LPDDR3_RKCFG_110_LOW			0x04262681
#define	LPDDR3_TEST2_4_048_LOW			0x1c00110d 
#define	LPDDR3_MISCTL0_VAL_0fc_LOW		0x81190000 
#endif




#define XTALK_SSO_LEN	 	3396

// PLL registers

// DRAMC registers
#define DRAMC_REG_ACTIM0 			0x000
    #define POS_ACTIM0_BL2 			15
#define DRAMC_REG_CONF1 			0x004
    #define POS_CONF1_BL4 			10
    #define POS_CONF1_SELFREF 		26
    #define POS_CONF1_TESTLP 		27    
#define DRAMC_REG_CONF2 			0x008
    #define POS_CONF2_TEST1 		29
    #define MASK_CONF2_TE12_ENABLE 	0xe0000000
#define DRAMC_REG_PADCTL1 			0x00c
#define DRAMC_REG_PADCTL2 			0x010
#define DRAMC_REG_PADCTL3 			0x014
#define DRAMC_REG_DELDLY1 			0x018
#define DRAMC_REG_01C 				0x01c
#define DRAMC_REG_DLLCONF 			0x028
#define DRAMC_REG_TEST2_0 			0x038   // new on MT5399
#define DRAMC_REG_TEST2_1 			0x03C
#define DRAMC_REG_TEST2_2 			0x040
#define DRAMC_REG_TEST2_3 			0x044
    #define POS_TEST2_3_ADVREFEN 	30
    #define POS_TEST2_3_TESTAUDPAT 	7
    // bit0~3
    #define MASK_TEST2_3_TESTCNT 	0x0000000f
    #define POS_TEST2_3_TESTCNT 	0
#define DRAMC_REG_TEST2_4 			0x048
    // bit0~4
    #define MASK_TEST2_4_TESTAUDINC 	0x0000001f
    #define POS_TEST2_4_TESTAUDINC 		0
    // bit8~12
    #define MASK_TEST2_4_TESTAUDINIT 	0x00001f00
    #define POS_TEST2_4_TESTAUDINIT 	8
    // bit 14, 15
    #define POS_TEST2_4_TESTAUDBITINV 	14
    #define POS_TEST2_4_TESTAUDMODE 	15
    //bit 16
    #define POS_TEST2_4_TESTXTALKPAT 	16
#define DRAMC_REG_DDR2CTL 			0x07c
    #define MASK_DDR2CTL_DATLAT 	0x00000070
    #define POS_DDR2CTL_DTALAT 		4
    #define POS_DDR2CTL_WOEN 		3
#define DRAMC_REG_084 				0x084
#define DRAMC_REG_MRS 				0x088
#define DRAMC_REG_CLK1DELAY 		0x08c
    #define POS_CLK1DELAY_DQCMD 	3
#define DRAMC_REG_IOCTL 			0x090
#define DRAMC_REG_DQSIEN 			0x094
    #define MASK_DQSIEN_R0DQS0IEN 	0x0000007f
    #define MASK_DQSIEN_R0DQS1IEN 	0x00007f00
    #define MASK_DQSIEN_R0DQS2IEN 	0x007f0000
    #define MASK_DQSIEN_R0DQS3IEN 	0x7f000000
    #define POS_DQSIEN_R0DQS0IEN 	0
    #define POS_DQSIEN_R0DQS1IEN 	8
    #define POS_DQSIEN_R0DQS2IEN 	16
    #define POS_DQSIEN_R0DQS3IEN 	24
#define DRAMC_REG_R1DQSIEN 			0x098
    #define MASK_DQSIEN_R1DQS0IEN 	0x0000007f
    #define MASK_DQSIEN_R1DQS1IEN 	0x00007f00
    #define MASK_DQSIEN_R1DQS2IEN 	0x007f0000
    #define MASK_DQSIEN_R1DQS3IEN 	0x7f000000
    #define POS_DQSIEN_R1DQS0IEN 	0
    #define POS_DQSIEN_R1DQS1IEN 	8
    #define POS_DQSIEN_R1DQS2IEN 	16
    #define POS_DQSIEN_R1DQS3IEN 	24    
#define DRAMC_REG_IODRV4 			0x0b0
    #define MASK_IODRV4_DQDRVN_2 	0x00000f00
    #define MASK_IODRV4_DQDRVP_2 	0x0000f000
    #define MASK_IODRV4_CMDDRVN_2 	0x0f000000
    #define MASK_IODRV4_CMDDRVP_2 	0xf0000000
    #define POS_IODRV4_DQDRVN_2 	8
    #define POS_IODRV4_DQDRVP_2 	12
    #define POS_IODRV4_CMDDRVN_2 	24
    #define POS_IODRV4_CMDDRVP_2 	28
#define DRAMC_REG_IODRV5 			0x0b4
    #define MASK_IODRV5_DQCDRVN 	0x00000f00
    #define MASK_IODRV5_DQCDRVP 	0x0000f000
    #define MASK_IODRV5_DQCDRVN_2 	0x0f000000
    #define MASK_IODRV5_DQCDRVP_2 	0xf0000000
    #define POS_IODRV5_DQCDRVN 		8
    #define POS_IODRV5_DQCDRVP 		12
    #define POS_IODRV5_DQCDRVN_2 	24
    #define POS_IODRV5_DQCDRVP_2 	28
#define DRAMC_REG_DRVCTL0 			0x0b8
    #define MASK_DRVCTL0_DQDRVN 	0x00000f00
    #define MASK_DRVCTL0_DQDRVP 	0x0000f000
    #define MASK_DRVCTL0_DQSDRVN 	0x0f000000
    #define MASK_DRVCTL0_DQSDRVP 	0xf0000000
    #define POS_DRVCTL0_DQDRVN 		8
    #define POS_DRVCTL0_DQDRVP 		12
    #define POS_DRVCTL0_DQSDRVN 	24
    #define POS_DRVCTL0_DQSDRVP 	28
#define DRAMC_REG_DRVCTL1 			0x0bc
    #define MASK_DRVCTL1_CMDDRVN 	0x00000f00
    #define MASK_DRVCTL1_CMDDRVP 	0x0000f000
    #define MASK_DRVCTL1_CLKDRVN 	0x0f000000
    #define MASK_DRVCTL1_CLKDRVP 	0xf0000000
    #define POS_DRVCTL1_CMDDRVN 	8
    #define POS_DRVCTL1_CMDDRVP 	12
    #define POS_DRVCTL1_CLKDRVN 	24
    #define POS_DRVCTL1_CLKDRVP 	28
#define DRAMC_REG_MCKDLY 			0x0d8
    //bit 23
    #define POS_MCKDLY_FIXODT 		23
    //bit 12~15
    #define POS_MCKDLY_FIXDQIEN 	12
    #define MASK_MCKDLY_FIXDQIEN 	0x0000f000
    // bit 10,11
    #define POS_MCKDLY_DQIENQKEND 	10
    #define MASK_MCKDLY_DQIENQKEND 	0x00000c00
#define DRAMC_REG_DQSCTL0 			0x0dc
#define DRAMC_REG_DQSCTL1 			0x0e0
    #define POS_DQSCTL1_DQSIENMODE 	28
    #define MASK_DQSCTL1_DQSINCTL 	0x0f000000   // for A60808, 4 bits totally
    #define POS_DQSCTL1_DQSINCTL 	24
    #define POS_DQSCTL1_DQS2CTL 	0   // [3:0] A60808: HW run-time DATLAT
    #define MASK_DQSCTL1_DQS2CTL 	0x0000000f
#define DRAMC_REG_PADCTL4 			0x0e4
    #define MASK_PADCTL4_DATLAT3 	0x00000010
    #define POS_PADCTL4_DATLAT3 	4
    #define POS_PADCTL4_CKEFIXON 	2
#define DRAMC_REG_PHYCTL1 			0x0f0
    #define MASK_PHYCTL1_DATLAT4 	0x02000000
    #define POS_PHYCTL1_DATLAT4 	25
    #define POS_PHYCTL1_PHYRST 		28
#define DRAMC_REG_GDDR3CTL1 		0x0f4
    #define POS_GDDR3CTL1_DQMSWAP 	31
    #define POS_GDDR3CTL1_RDATRST 	25
    #define POS_GDDR3CTL1_BKSWAP 	20
#define DRAMC_REG_PADCTL7 			0x0f8
#define DRAMC_REG_MISCTL0 			0x0fc
#define DRAMC_REG_OCDK 				0x100
    #define POS_OCDK_DRVREF 		24
#define DRAMC_REG_RKCFG 			0x110
    #define POS_RKCFG_PBREF_DISBYRATE 6
    #define POS_RKCFG_WDATKEY64 	29   //32-bit channel it is 0, for 16-bit or asym DRAM it is 1
#define DRAMC_REG_CKPHDET 			0x114
#define DRAMC_REG_DQSCTL2 			0x118
    #define MASK_DQSCTL2_DQSINCTL 	0x0000000f
    #define POS_DQSCTL2_DQSINCTL 	0
#define DRAMC_REG_DQ_DQS01_SEL 		0x120
#define DRAMC_REG_DQSGCTL 			0x124
    #define MASK_DQSGCTL_R0DQSG_COARSE_DLY_COM0 0x00000003
    #define MASK_DQSGCTL_R1DQSG_COARSE_DLY_COM0 0x0000000C
    #define MASK_DQSGCTL_R0DQSG_COARSE_DLY_COM1 0x00000030
    #define MASK_DQSGCTL_R1DQSG_COARSE_DLY_COM1 0x000000C0
    #define POS_DQSGCTL_R0DQSG_COARSE_DLY_COM0 	0
    #define POS_DQSGCTL_R1DQSG_COARSE_DLY_COM0 	2
    #define POS_DQSGCTL_R0DQSG_COARSE_DLY_COM1 	4
    #define POS_DQSGCTL_R1DQSG_COARSE_DLY_COM1 	6
    #define POS_DQSGCTL_DQSGDUALP 	30
#define DRAMC_REG_CLKENCTL 			0x130
#define DRAMC_REG_WRLEV 			0x13c
    //bit 12
    #define POS_WRLEV_DQS_SEL 		12
    #define MASK_WRLEV_DQS_SEL 		0x0000f000
    //bit 8
    #define POS_WRLEV_DQS_WLEV 		8
    //bit 1~4
    #define POS_WRLEV_DQS_Bx_G 		1
    #define MASK_WRLEV_DQS_Bx_G 	0x0000001e
    //bit 0
    #define POS_WRLEV_WRITE_LEVEL_EN 0
#define DRAMC_REG_PHYCLKDUTY		0x148
    #define POS_PHYCLKDUTY_CMDCLKP0DUTYSEL	28
    #define POS_PHYCLKDUTY_CMDCLKP0DUTYP 	18
    #define MASK_PHYCLKDUTY_CMDCLKP0DUTYP 	0x000c0000
    #define POS_PHYCLKDUTY_CMDCLKP0DUTYN 	16
    #define MASK_PHYCLKDUTY_CMDCLKP0DUTYN 	0x00030000
#define DRAMC_REG_158 				0x158
#define DRAMC_REG_ARBCTL0 			0x168
#define DRAMC_REG_CMDDLY0 			0x1a8
#define DRAMC_REG_CMDDLY1 			0x1ac
#define DRAMC_REG_CMDDLY2 			0x1b0
#define DRAMC_REG_CMDDLY3 			0x1b4
#define DRAMC_REG_CMDDLY4 			0x1b8
#define DRAMC_REG_DQSCAL0 			0x1c0
    #define POS_DQSCAL0_STBCALEN 	31
#define DRAMC_REG_IMPCAL 			0x1c8
    #define POS_IMPCAL_IMPCALI_EN 	0
    #define POS_IMPCAL_IMPCAL_HW 	1
    #define POS_IMPCAL_IMP_CALI_ENN 4
    #define POS_IMPCAL_IMP_CALI_ENP 5
    #define POS_IMPCAL_IMPPDN 		6
    #define POS_IMPCAL_IMPPDP 		7    
    #define POS_IMPCAL_IMPDRVP 		8
    #define MASK_IMPCAL_IMPDRVP 	0x00000f00
    #define POS_IMPCAL_IMPDRVN 		12
    #define MASK_IMPCAL_IMPDRVN 	0x0000f000
#define DRAMC_REG_DMMonitor 		0x1d8
#define DRAMC_REG_DRAMC_PD_CTRL 	0x1dc
    #define MASK_DRAMC_PD_CTRL_REFCNT_FR_CLK 	0x00ff0000
    #define POS_DRAMC_PD_CTRL_REFCNT_FR_CLK 	16
    #define POS_DRAMC_PD_CTRL_MIOCKCTRLOFF 		26
#define DRAMC_REG_LPDDR2 			0x1e0
    #define POS_LPDDR2_ADRDECEN 	31
    #define POS_LPDDR2_SELO1ASO 	30
#define DRAMC_REG_SPCMD 			0x1e4
    #define POS_SPCMD_MRWEN			0
    #define POS_SPCMD_DQSGCNTEN 	8
    #define POS_SPCMD_DQSGCNTRST 	9
#define DRAMC_REG_ACTIM1 			0x1e8
    #define MASK_ACTIM1_REFRCNT 	0x00ff0000
    #define POS_ACTIM1_REFRCNT 		16
#define DRAMC_REG_PERFCTL0 			0x1ec
#define DRAMC_REG_RRRATE_CTL 		0x1f4
    #define MASK_RRRATE_CTL_BIT0_SEL 0x0000001f
    #define MASK_RRRATE_CTL_BIT1_SEL 0x00001f00
    #define MASK_RRRATE_CTL_BIT2_SEL 0x001f0000
    #define MASK_RRRATE_CTL_BIT3_SEL 0x1f000000
    #define POS_RRRATE_CTL_BIT0_SEL 0
    #define POS_RRRATE_CTL_BIT1_SEL 8
    #define POS_RRRATE_CTL_BIT2_SEL 16
    #define POS_RRRATE_CTL_BIT3_SEL 24
#define DRAMC_REG_LPDDR2_4 			0x1f8
#define DRAMC_REG_MRR_CTL 			0x1fc
    #define MASK_MRR_CTL_BIT4_SEL 	0x0000001f
    #define MASK_MRR_CTL_BIT5_SEL 	0x00001f00
    #define MASK_MRR_CTL_BIT6_SEL 	0x001f0000
    #define MASK_MRR_CTL_BIT7_SEL 	0x1f000000
    #define POS_MRR_CTL_BIT4_SEL 	0
    #define POS_MRR_CTL_BIT5_SEL 	8
    #define POS_MRR_CTL_BIT6_SEL 	16
    #define POS_MRR_CTL_BIT7_SEL 	24
#define DRAMC_REG_DQODLY1 			0x200
#define DRAMC_REG_DQODLY2 			0x204
#define DRAMC_REG_DQODLY3 			0x208
#define DRAMC_REG_DQODLY4 			0x20c
#define DRAMC_REG_DQIDLY1 			0x210
#define DRAMC_REG_DQIDLY2 			0x214
#define DRAMC_REG_DQIDLY3 			0x218
#define DRAMC_REG_DQIDLY4 			0x21c
#define DRAMC_REG_DQIDLY5 			0x220
#define DRAMC_REG_DQIDLY6 			0x224
#define DRAMC_REG_DQIDLY7 			0x228
#define DRAMC_REG_DQIDLY8 			0x22c
#define DRAMC_REG_DMACON 			0x234
    #define POS_DMACON_DMAEN 			0
    #define POS_DMACON_DMAPUREWRITEEN 	1
    #define POS_DMACON_DMAPUREREADEN 	2
    #define POS_DMACON_DMABURSTLEN 		4
    #define MASK_DMACON_DMABURSTLEN 	0x00000070
#define DRAMC_REG_DMASRCADDR 		0x238
#define DRAMC_REG_DMADSTADDR 		0x23c
#define DRAMC_REG_DMATRANSLEN 		0x240
    #define POS_DMATRANSLEN_DMATRANSLEN 0
    #define MASK_DMATRANSLEN_DMATRANSLEN 0x000fffff
#define DRAMC_REG_DMAPAT 			0x244
#define DRAMC_REG_DMAMON 			0x254
    #define POS_DMAMON_DMASTATUS 	0
#define DRAMC_REG_DMACMPERR 		0x258
#define DRAMC_REG_R2R_page_hit_counter 0x280
#define DRAMC_REG_STBENERR_R 		0x320
#define DRAMC_REG_STBENERR_F 		0x324
#define DRAMC_REG_TOGGLE_CNT 		0x32c
#define DRAMC_REG_DQS_ERR_CNT 		0x330
#define DRAMC_REG_DQ_ERR_CNT 		0x334
#define DRAMC_REG_SELPH 			0x344
#define DRAMC_REG_DCBLN 			0x348
    #define POS_DCBLN_RSTBCNT_LATCH_EN 	11
    #define POS_DCBLN_RX_MIOCK_JIT_EN 	2
    #define POS_DCBLN_RX_EYE_SCAN_EN 	1
    #define POS_DCBLN_REG_SW_RST 		0
#define DRAMC_REG_TOGGLE_CNT_2 		0x360
#define DRAMC_REG_DQS_ERR_CNT_2 	0x364
#define DRAMC_REG_DQ_ERR_CNT_2 		0x368
#define DRAMC_REG_CMP_ERR 			0x370
#define DRAMC_REG_DQSDLY0 			0x3a0
#define DRAMC_REG_SPCMDRESP 		0x3b8
    #define POS_SPCMDRESP_SREF_STATE 16
#define DRAMC_REG_DQSGNWCNT0 		0x3c0
#define DRAMC_REG_DQSGNWCNT1 		0x3c4
#define DRAMC_REG_DLLSTATUS0 		0x3dc
    #define POS_DLLSTATUS0_CMPOT 	31
    #define POS_DLLSTATUS0_CMPOUTN 	30
#define DRAMC_REG_TESTRPT 			0x3fc
    #define POS_TESTRPT_DM_CMP_CPT 	10
    #define POS_TESTRPT_DM_CMP_ERR 	14
#define DRAMC_REG_SELPH1 			0x400
#define DRAMC_REG_SELPH2 			0x404
    #define MASK_SELPH2_TXDLY_DQSGATE 		0x00007000
    #define POS_SELPH2_TXDLY_DQSGATE 		12
    #define MASK_SELPH2_TXDLY_DQSGATE_P1 	0x00700000
    #define POS_SELPH2_TXDLY_DQSGATE_P1 	20
#define DRAMC_REG_SELPH3 			0x408
#define DRAMC_REG_SELPH4 			0x40C    
#define DRAMC_REG_SELPH5 			0x410
    #define MASK_SELPH5_dly_DQSGATE 0x00c00000
    #define POS_SELPH5_dly_DQSGATE 	22
    #define MASK_SELPH5_dly_DQSGATE_P1 		0x03000000
    #define POS_SELPH5_dly_DQSGATE_P1 		24
#define DRAMC_REG_SELPH6 			0x414
#define DRAMC_REG_SELPH6_1 			0x418
    #define MASK_SELPH6_1_TXDLY_R1DQSGATE 	0x00000070
    #define POS_SELPH6_1_TXDLY_R1DQSGATE 	4
    #define MASK_SELPH6_1_TXDLY_R1DQSGATE_P1 0x00000700
    #define POS_SELPH6_1_TXDLY_R1DQSGATE_P1 8
    #define MASK_SELPH6_1_dly_R1DQSGATE 	0x00000003
    #define POS_SELPH6_1_dly_R1DQSGATE 		0
    #define MASK_SELPH6_1_dly_R1DQSGATE_P1 	0x0000000c
    #define POS_SELPH6_1_dly_R1DQSGATE_P1 	2
#define DRAMC_REG_SELPH7 			0x41c
#define DRAMC_REG_SELPH8 			0x420
#define DRAMC_REG_SELPH9 			0x424
#define DRAMC_REG_SELPH10 			0x428
#define DRAMC_REG_SELPH11 			0x42c
#define DRAMC_REG_SELPH12 			0x430
#define DRAMC_REG_SELPH13 			0x434
#define DRAMC_REG_SELPH14 			0x438
#define DRAMC_REG_SELPH15 			0x43c
#define DRAMC_REG_SELPH16 			0x440
#define DRAMC_REG_COM0_DLY_SEL_DGTED 0x478
#define DRAMC_REG_COM0_DLY_SEL_DQS0 0x47C
#define DRAMC_REG_COM0_DLY_SEL_DQS1 0x480
#define DRAMC_REG_COM0_DLY_SEL_DQM0 0x484
#define DRAMC_REG_COM0_DLY_SEL_DQ2 	0x494
#define DRAMC_REG_COM0_DLY_SEL_DQ3 	0x498
#define DRAMC_REG_COM0_DLY_SEL_DQ4 	0x49c
#define DRAMC_REG_COM0_DLY_SEL_DQ5 	0x4a0
#define DRAMC_REG_COM0_DLY_SEL_DQ6 	0x4a4
#define DRAMC_REG_COM0_DLY_SEL_DQ7 	0x4a8
#define DRAMC_REG_COM1_DLY_SEL_DGTED 0x4CC
#define DRAMC_REG_COM1_DLY_SEL_DQS0 0x4D0
#define DRAMC_REG_COM1_DLY_SEL_DQS1 0x4D4
#define DRAMC_REG_COM1_DLY_SEL_DQM0 0x4D8
#define DRAMC_REG_COM1_DLY_SEL_DQ0 	0x4e0
#define DRAMC_REG_COM1_DLY_SEL_DQ2 	0x4e8
#define DRAMC_REG_COM1_DLY_SEL_DQ3 	0x4ec
#define DRAMC_REG_COM1_DLY_SEL_DQ4 	0x4f0
#define DRAMC_REG_COM1_DLY_SEL_DQ5 	0x4f4
#define DRAMC_REG_COM1_DLY_SEL_DQ6 	0x4f8
#define DRAMC_REG_COM1_DLY_SEL_DQ7 	0x4fc
#define DRAMC_REG_PH_EN 			0x520
#define DRAMC_REG_DLY_SEL_MCK_ANA 	0x524
#define DRAMC_REG_DLY_SEL_CLKGEN 	0x528
#define DRAMC_REG_DLY_SEL_CLKGEN2 	0x52C

//======================== ETT Definition =====================================
//#if __ETT__
//#undef SPM_CONTROL_AFTERK		//Lewis@20150820: ETT will not use TransferToSPMControl() except low power scenario test code
//#endif
//=============================================================================



//======================== FIRST_BRING_UP Init Definition =====================
#ifdef FIRST_BRING_UP
#define DDRTYPE_LPDDR3

//#undef DUAL_RANKS

#undef HW_GATING

#undef DRAMC_DRAMPHY_DCM_ENABLE

#undef TEMP_SENSOR_ENABLE		
#undef ZQCS_ENABLE				
#undef EMI_OPTIMIZATION			
//#undef SPM_CONTROL_AFTERK		

#undef DUAL_FREQ_HIGH
#undef DUAL_FREQ_LOW
#define DUAL_FREQ_HIGH	533
#define DUAL_FREQ_LOW	533		

#undef DUAL_FREQ_TEST
#undef ONEPLL_TEST
#undef DUAL_FREQ_TEST_DIFFV

#endif //FIRST_BRING_UP
//=============================================================================





#endif // _MT6755_REGISTER_H_
