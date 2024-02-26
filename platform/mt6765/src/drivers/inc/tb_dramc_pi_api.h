#ifndef _PI_API_H
#define _PI_API_H

/***********************************************************************/
/*              Includes                                               */
/***********************************************************************/

/***********************************************************************/
/*              Constant Define                                        */
/***********************************************************************/

#ifndef __FLASH_TOOL_DA__
#define __FLASH_TOOL_DA__   0
#else
#undef __FLASH_TOOL_DA__ 
#define __FLASH_TOOL_DA__   1
#endif
#define SW_CHANGE_FOR_SIMULATION 0  //calibration funciton for whole chip simulation. Code changed due to different compiler
#define FOR_DV_SIMULATION_USED  0 ////calibration funciton for DV simulation. Code changed due to different compiler
#define FT_DSIM_USED 0


#if(!SW_CHANGE_FOR_SIMULATION)
#if __FLASH_TOOL_DA__
  #include "sw_types.h"
#else
  #include "typedefs.h"
#endif
#endif

//Bring Up Selection : Do Not open it when normal operation
//#define FIRST_BRING_UP
#define BIANCO_TO_BE_PORTING
#define CERVINO_TO_BE_PORTING

//DRAMC Chip
#define fcA60501    1
#define fcElbrus        3
#define fcOlympus       4
#define fcA60838        5
#define fcWhitney       6
#define fcAlaska        7
#define fcBianco        8
#define fcSylvia        9
#define fcCannon        10
#define fcFOR_CHIP_ID   fcSylvia

#define VENDOR_SAMSUNG 1
#define VENDOR_HYNIX 6

#define CHANNEL_NUM      2   // 1 single channel, 2 dual channel, 4 4 channel
#define DUAL_RANK_ENABLE 1


//Feature option
#define ENABLE_LP4_ZQ_CAL  1
#define ENABLE_CA_TRAINING  1
#define ENABLE_WRITE_LEVELING 1
#define ENABLE_PHY_RX_INPUT_OFFSET  0
#define ENABLE_LOW_POWER_BOOT 0
#define ENABLE_DUTY_SCAN_V2   1
#define DUTY_SCAN_V2_ONLY_K_HIGHEST_FREQ    0   // 0: K all Freq 1: K highest Freq
//#define YH_SWEEP_IC 


//#define SOLUTION_FOR_SS_B422_TX_WIN_TOO_SMALL 1   //Only for Bianco, Only for Samsung normal mode (B422), Only for DRVP >=15 ==> Set DRAM ODT to RZQ/3 (80ohm)

#define TX_DQ_PRE_EMPHASIS 0 //pre-emphasis shoulde be adjusted by different frequency and IMP 

//SW option
#define DUAL_FREQ_K     1        // If enable, need to define DDR_xxxx the same as DUAL_FREQ_HIGH
#define EVEREST_PORTING_TODO 0
#define ENABLE_LP3_SW 1// for LP4 only project, can set to 0 to save code size.
#define ENABLE_RANK_NUMBER_AUTO_DETECTION 0  //only can enable when DUAL_RANK_ENABLE is 1
#define EYESCAN_LOG __ETT__   //draw eye diagram after calibration, if enable, need to fix code size problem (maybe disable ENABLE_LP3_SW first for temp workaround)
/*
#if EYESCAN_LOG  // TODO : temp workaround, need to fix code size problem
#undef ENABLE_LP3_SW
#define ENABLE_LP3_SW 0
#endif
*/
#define REDUCE_LOG_FOR_PRELOADER 1
#define APPLY_LP4_POWER_INIT_SEQUENCE   1
#define CBT_K_RANK1_USE_METHOD  1   // 1: Method 1, use mpcrk and txrank 2: Method 2, use rankswap
#define CA_TRAINING_K_RANK1_ENABLE 1
#define ENABLE_READ_DBI 0 
#define ENABLE_WRITE_DBI 1
#define ENABLE_WRITE_DBI_Protect 0
#define ENABLE_TX_WDQS 1
#define ENABLE_DRS 0

/* If defined: Use LP4_DDR3733 as highest freq
 * If not defined: Use LP4_DDR3200 as highest freq
 */
#define LP4_HIGHEST_DDR3733 0
#if LP4_HIGHEST_DDR3733
    #define LP4_HIGHEST_FREQ      (1866)
    #define LP4_MIDDLE_FREQ        (1600)
    #define LP4_HIGHEST_FREQSEL   (LP4_DDR3733)
    #define LP4_MIDDLE_FREQSEL     (LP4_DDR3200)
#else //LP4_HIGHEST_DDR3200
    #define LP4_HIGHEST_FREQ      (1600)
    #define LP4_MIDDLE_FREQ        (1200)
    #define LP4_HIGHEST_FREQSEL   (LP4_DDR3200)
    #define LP4_MIDDLE_FREQSEL     (LP4_DDR2400)
#endif

#if !__ETT__
// Preloader: using config CFG_DRAM_CALIB_OPTIMIZATION to identify
#if (FOR_DV_SIMULATION_USED==0)
// Preloader: using config CFG_DRAM_CALIB_OPTIMIZATION to identify
#define SUPPORT_SAVE_TIME_FOR_CALIBRATION		CFG_DRAM_CALIB_OPTIMIZATION
#else
// DV simulation, use full calibration flow
#define SUPPORT_SAVE_TIME_FOR_CALIBRATION		0
#endif
#define EMMC_READY CFG_DRAM_CALIB_OPTIMIZATION
#define FORLOOP_TEST_SAVE_TIME_FOR_CALIBRATION_ON_ETT	0
#define BYPASS_VREF_CAL		(SUPPORT_SAVE_TIME_FOR_CALIBRATION & EMMC_READY)
#define BYPASS_CBT		(SUPPORT_SAVE_TIME_FOR_CALIBRATION & EMMC_READY)
#define BYPASS_DATLAT		(SUPPORT_SAVE_TIME_FOR_CALIBRATION & EMMC_READY)
#define BYPASS_WRITELEVELING	(SUPPORT_SAVE_TIME_FOR_CALIBRATION & EMMC_READY)
#define BYPASS_RDDQC		(SUPPORT_SAVE_TIME_FOR_CALIBRATION & EMMC_READY)
#define BYPASS_RXWINDOW		(SUPPORT_SAVE_TIME_FOR_CALIBRATION & EMMC_READY)
#define BYPASS_TXWINDOW		(SUPPORT_SAVE_TIME_FOR_CALIBRATION & EMMC_READY)
#define BYPASS_GatingCal	(SUPPORT_SAVE_TIME_FOR_CALIBRATION & EMMC_READY)
#define BYPASS_CA_PER_BIT_DELAY_CELL (SUPPORT_SAVE_TIME_FOR_CALIBRATION & EMMC_READY)
//#define BYPASS_TX_PER_BIT_DELAY_CELL (SUPPORT_SAVE_TIME_FOR_CALIBRATION & EMMC_READY)
#else
// ETT
#define SUPPORT_SAVE_TIME_FOR_CALIBRATION 0
#define EMMC_READY 0
#define FORLOOP_TEST_SAVE_TIME_FOR_CALIBRATION_ON_ETT 0
#define BYPASS_VREF_CAL 0
#define BYPASS_CBT 0
#define BYPASS_DATLAT 0
#define BYPASS_WRITELEVELING 0
#define BYPASS_RDDQC 0
#define BYPASS_RXWINDOW 0
#define BYPASS_TXWINDOW 0
#define BYPASS_GatingCal 0
#define BYPASS_CA_PER_BIT_DELAY_CELL 0
//#define BYPASS_TX_PER_BIT_DELAY_CELL 0
//for DRAM calibration data stored in emmc trial-run: disable LP3 to reduce code size for msdc driver
#if EMMC_READY
#undef ENABLE_LP3_SW
#define ENABLE_LP3_SW 0
#endif
#endif


// Sw work around options.
#define WRITE_LEVELING_MOVE_DQS_INSTEAD_OF_CLK 1 // work around for clock multi phase problem(cannot move clk or the clk will be bad)
#define CA_TRAIN_RESULT_DO_NOT_MOVE_CLK 1   // work around for clock multi phase problem(cannot move clk or the clk will be bad)
#define TX_PERBIT_INIT_FLOW_CONTROL 1 // work around for clock multi phase problem(cannot move clk or the clk will be bad)
#define DramcHWDQSGatingTracking_DVT_JADE_TRACKING_MODE 0
#define DramcHWDQSGatingTracking_DVT_FIFO_MODE 1
#define DramcHWDQSGatingTracking_DVT_LP3_FIFO_MODE 1
#define DONT_MOVE_CLK_DELAY // don't move clk delay

//if SAMSUNG_TEST_MODE_MRS_FOR_PRELOADER == 0, Disable
//if SAMSUNG_TEST_MODE_MRS_FOR_PRELOADER == 1, Buffer sensitivity decrease1, test 2. TMRS enter -> 000 -> 390 -> 120 -> 8A7
//if SAMSUNG_TEST_MODE_MRS_FOR_PRELOADER == 2, Buffer sensitivity decrease2, test 3. TMRS enter -> 000 -> 390 -> 120 -> 803
//if SAMSUNG_TEST_MODE_MRS_FOR_PRELOADER == 3, xxxxxxxxxxxxxxxxxxxxxxxxxxxx, test 4. TMRS enter -> 000 -> 390 -> 120 -> 014 -> 863
//if SAMSUNG_TEST_MODE_MRS_FOR_PRELOADER == 4, Separate Rd fail/write fail , test 5. TMRS enter -> 000 -> 390 -> 120 -> 52A
#define SAMSUNG_TEST_MODE_MRS_FOR_PRELOADER 0

/* Samsung's 1xnm LP4(X) uses nWR & RL settings to determine the freq range it operates at
 * Frequency info determined by nWR & RL is used to adjust write-related internal timing(Write Pulse Width = WCSL) other than auto-precharge
 * For all samsung LP4 dram 1. Fix nWR to 30    2. tWTR@800Mhz: "original tWTR" + 2tCK
 */
#define SAMSUNG_LP4_NWR_WORKAROUND 1

//Definitions indicating DRAMC, DDRPHY register shuffle offset
#define SHU_GRP_DRAMC_OFFSET      0x600
#define SHU_GRP_DDRPHY_OFFSET     0x500

//init for test engine
#define DEFAULT_TEST2_1_CAL 0x55000000   // pattern0 and base address for test engine when we do calibration
#if (FOR_DV_SIMULATION_USED!=0)
//because cmd_len=1 has bug with byte mode, so need to set cmd_len=0, then it will cost more time to do a pattern test
//workaround: reduce TEST2_OFF to make less test agent cmd. make lpddr4-1600 can finish in 60us
#define DEFAULT_TEST2_2_CAL 0xaa000020 // pattern1 and offset address for test engine when we do calibraion
#else
#define DEFAULT_TEST2_2_CAL 0xaa000400   // pattern1 and offset address for test engine when we  do calibraion
#endif
#define CMP_CPT_POLLING_PERIOD 1       // timeout for TE2: (CMP_CPT_POLLING_PERIOD X MAX_CMP_CPT_WAIT_LOOP)
#define MAX_CMP_CPT_WAIT_LOOP 10000   // max loop

// common
#define DQS_NUMBER   4
#define DQS_BIT_NUMBER  8
#define DQ_DATA_WIDTH   32   // define max support bus width in the system (to allocate array size)
#define DQ_DATA_WIDTH_LP4 16
#define TIME_OUT_CNT    100 //100us
#define HW_REG_SHUFFLE_MAX  4

// Gating window
#define DQS_GW_COARSE_STEP    1//8 // for testing// 1
#define DQS_GW_FINE_START 0
#define DQS_GW_FINE_END 32
#define DQS_GW_FINE_STEP 4
#define GATING_ADJUST_TXDLY_FOR_TRACKING  1
/* When DQS_GW_7UI is
 * defined: 7UI burst length gating mode
 * not defined: 8UI burst length gating mode
 */
#define DQS_GW_7UI

// DATLAT
#define DATLAT_TAP_NUMBER 32   // DATLAT[3:0] = {0x80[20:4]}

// RX DQ/DQS
#define MAX_RX_DQSDLY_TAPS 127       // 0x018, May set back to 64 if no need.
#define MAX_RX_DQDLY_TAPS 63      // 0x210~0x22c, 0~15 delay tap

// TX DQ/DQS
#define MAX_TX_DQDLY_TAPS 31   // max DQ TAP number
#define MAX_TX_DQSDLY_TAPS 31   // max DQS TAP number
#define TX_OE_EXTEND 0
#if TX_OE_EXTEND
#define TX_DQ_OE_SHIFT_LP4 4
#else
#define TX_DQ_OE_SHIFT_LP4 3
#endif
#define TX_DQ_OE_SHIFT_LP3 2
#define TX_K_DQM_WITH_WDBI  1
#define TX_OE_CALIBATION (!TX_OE_EXTEND)

// DVFS
// need to review #if & #ifdef, Jeremy
#define TDQSCK_PRECALCULATION_FOR_DVFS  1//DQS pre-calculation
#define ENABLE_DLL_ALL_SLAVE_MODE 1 //set DLL to all slave mode to reduce 70mck during DVFS (data rate <= DDR1866)
#define DLL_ASYNC_MODE 0 //0:Sync mode(old),CHB CA will be slave; 1:Async mode(new),CHB CA will be master
#define ENABLE_DVS 0
#define DRAMC_DFS_MODE 1 //0:RG/SPM, 1:SPM, 2:RG

//DVT test
#define DVT_TEST_DUMMY_RD_SIDEBAND_FROM_SPM 0
#define DVT_TEST_DUMMY_READ_FOR_DQS_GATING_TRACKING 0

//Run time config
#define HW_GATING //DQS strobe calibration enable
//#define GATING_7UI_FOR_TRACKING /* Use DQS_GW_7UI instead */
#define DUMMY_READ_FOR_TRACKING
#define ENABLE_SW_RUN_TIME_ZQ_WA 1
#if !ENABLE_SW_RUN_TIME_ZQ_WA
#define ZQCS_ENABLE_LP4   //Whitney E1 can't enable HQZQ,  enable SW ZQ @ kernel.
#endif
#define ZQCS_ENABLE_LP3 //Enable LP3 ZQCS run time because of DRS de-feature.
#define TEMP_SENSOR_ENABLE //After enable rumtime MR4 read, Get DramC SPCMDRESP_REFRESH_RATE.
#if(!SW_CHANGE_FOR_SIMULATION)
#define APPLY_LOWPOWER_GOLDEN_SETTINGS 1 //Low pwer settings
#define SPM_CONTROL_AFTERK //Disable in bring-up and enable thereafter.
#else
#define APPLY_LOWPOWER_GOLDEN_SETTINGS 0 //Low pwer settings
#endif
#define IMPEDANCE_TRACKING_ENABLE //Impendence tracking
//#define IMPEDANCE_HW_SAVING   //mask because function fail, it lets clk swing change larger before DVFS occurs
#define MR13_RRO 1 //MR4 refresh rate option, 0:Disable codes 001 and 010 in MR4 OP[2:0]   1:Enable all codes in MR4 OP[2:0]

//Lewis@20170508: Undefine DUMMY_READ_FOR_DQS_GATING_RETRY since SPM not use it.
//#define DUMMY_READ_FOR_DQS_GATING_RETRY //LP4, LP4X: DQS gating retry controlled by SPM side band signal, dummy read rank selection controlled by test agent for DQS gating retry
                                        //LP3: Disable DQS gating retry
#define HW_SAVE_FOR_SR
#define CLK_FREE_FUN_FOR_DRAMC_PSEL //If Psel is 1, clock will be free run at the periof of 2T to let conf be applied. 
                                      //If Psel is 0, Clock will be gated
#define PA_IMPROVEMENT_FOR_DRAMC_ACTIVE_POWER 1 //DE PIC: Jouling, PowerArtist enables RTL-to-GDS design for power methodology by providing early RTL power estimation and analysis-driven power reduction capabilities
//#define MPC_SW_FIFO                           //LPDDR4 WRFIFO and RDFIFO

#define ENABLE_TX_TRACKING 1
#if ENABLE_TX_TRACKING
    #if (__ETT__)
    #define ENABLE_SW_TX_TRACKING 0
    #else
    #define ENABLE_SW_TX_TRACKING 1
    #endif
#endif
#define ENABLE_RX_TRACKING_LP4 1
#define ENABLE_PER_BANK_REFRESH 1
#define SET_CKE_2_RANK_INDEPENDENT 0
#define SET_CKE_2_RANK_INDEPENDENT_RUN_TIME 1 //Temporary solution: After we could calibation with CKE individual, we could remove this flag

#define ENABLE_RODT_TRACKING 1
#define ENABLE_RODT_TRACKING_SAVE_MCK 0 //Maybe turn on behind Vinson project

//#if defined(SLT)
//#define ENABLE_DRAM_SLT
//#endif

//Debug option
#define GATING_ONLY_FOR_DEBUG __ETT__
#define RX_DLY_TRACK_ONLY_FOR_DEBUG 0 // LP4 only, LP3 not support
#define CPU_RW_TEST_AFTER_K 1// need to enable GATING_ONLY_FOR_DEBUG at the same time for gating debug log
#define TA2_RW_TEST_AFTER_K 1

#if (FOR_DV_SIMULATION_USED==0)
#define ETT_PRINT_FORMAT  // Apply for both preloader and ETT
#endif
#define LJPLL_FREQ_DEBUG_LOG 0
#define ENABLE_CLAIBTAION_WINDOW_LOG_FOR_FT 0  //FT log for finding out IC with small window size
#if (__ETT__)
#define ENABLE_DDRPHY_FREQ_METER 1
#endif
#define SUPPORT_LP3_800 1  // cross rank parameters are over spec.  cannot support
#define CLOCKDUTY_SCAN_FOR_SLT_ONLY 0   // 1: for SLT test used
#define PRINT_CALIBRATION_SUMMARY (!SW_CHANGE_FOR_SIMULATION)
//... add new feature compile option here.
//#define RX_DLY_TRACKING_VERIFY 0 // not implemented

//misc feature
#define ENABLE_DQ3200_UNTERM 0  // 0: not support, 1: enable unterm swimp cal setting with 30 ohm @ DDR3200
#if (ENABLE_DQ3200_UNTERM == 1)
#undef IMPEDANCE_TRACKING_ENABLE
#endif
//#define ENABLE_HW_IMPCAL

#define XRTW2W_PERFORM_ENHANCE_TX
#define XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
//#define XRTR2W_PERFORM_ENHANCE_RODTEN    //conflict with ENABLE_RODT_TRACKING, LP4 support only

#define APPLY_SIGNAL_WAVEFORM_SETTINGS_ADJUST 1     //adjust IMP DRVP and DQ ODT RZQ(MR11) in the test menu
#define DRAMC_MODEREG_CHECK 1


/* LEGACY_DELAY_CELL: Initial DQ delay cell settings (originated from DE's mem.c)
 * Preserved here for 1. "DV_SIM" + "TX calibration bypassed"  2. May be required for high freq initial ModeRegWrite?
 */
#if (FOR_DV_SIMULATION_USED==0)
#define LEGACY_DELAY_CELL 1
#define LEGACY_TX_TRACK 0      //Legacy TX tracking redundant initial settings (actual correct values are set during calibration)
#define LEGACY_TDQSCK_PRECAL 0 //Legacy tDQSCK precal redundant initial settings (actual correct values are set during calibration)
#define LEGACY_GATING_DLY 0 //Legacy settings which correct values are calibrated/set during gating
#define LEGACY_RX_DLY 0 //Legacy settings which correct values are calibrated/set during DramcRxWindowPerbitCal()
#else
#define LEGACY_DELAY_CELL 1
#define LEGACY_TX_TRACK 1      //Legacy TX tracking redundant initial settings (actual correct values are set during calibration)
#define LEGACY_TDQSCK_PRECAL 1 //Legacy tDQSCK precal redundant initial settings (actual correct values are set during calibration)
#define LEGACY_GATING_DLY 1 //Legacy settings which correct values are calibrated/set during gating
#define LEGACY_RX_DLY 1 //Legacy settings which correct values are calibrated/set during DramcRxWindowPerbitCal()
#endif

#define CA_PER_BIT_DELAY_CELL 0 
#define ENABLE_MIOCK_JMETER   // for TX_PER_BIT_DELAY_CELL

/* HQA debug option - Calibration results summary
 * 1. Enabled in ETT
 * 2. Disabled in preloader
 *    (May be enabled manually)
 */
#if (__ETT__)
#define FOR_HQA_TEST_USED   // HQA test used, to print result for easy report
#define FOR_HQA_REPORT_USED // print out special format result for HQA make report used, from Arnold
#endif

//#define ENABLE_POST_PACKAGE_REPAIR

#define RUNTIME_SHMOO_RELEATED_FUNCTION     CFG_DRAM_SAVE_FOR_RUNTIME_SHMOO
#define RUNTIME_SHMOO_RG_BACKUP_NUM         (200)

#if (__ETT__)
/* 1. Add test file (ex: ft_scan_test.c or fpc_lpbk_mem.c) in the ett\makefile (C_SRC_FILES = ft_scan_test.c ...)
 * 2. Enable LOOPBACK_TEST define and others(below)
 * 3. Enable VCORE_SHMOO
 */
/*
#define LOOPBACK_TEST 
//#define DDR1600_LPBK
//#define DDR2667_LPBK
//#define DDR3200_LPBK
#define DDR3733_LPBK
#define LPBK_INTERNAL_EN
#define DDRPHY_LPBK_CAL_EN
#define EN_DDR3733
*/
//#define VCORE_SHMOO
#ifdef VCORE_SHMOO
#define VCORE_SHMOO_RX_TX_SCAN 0    //for Rx_Tx_scan pattern
#endif
#endif

#define ETT_NO_DRAM 0 //For ETT power measure, test with NO DRAM

//======================== FIRST_BRING_UP Init Definition =====================
#ifdef FIRST_BRING_UP

//#define USE_CLK26M

#undef DUAL_FREQ_K
#define DUAL_FREQ_K 0

#undef TDQSCK_PRECALCULATION_FOR_DVFS
#define TDQSCK_PRECALCULATION_FOR_DVFS  0//DQS pre-calculation

#undef CHANNEL_NUM
#define CHANNEL_NUM 2
#undef DUAL_RANK_ENABLE
#define DUAL_RANK_ENABLE 1

#undef ENABLE_DUTY_SCAN_V2
#define ENABLE_DUTY_SCAN_V2 0

#undef ENABLE_DRS
#define ENABLE_DRS 0

#undef ENABLE_CA_TRAINING
#define ENABLE_CA_TRAINING  1
#undef ENABLE_WRITE_LEVELING
#define ENABLE_WRITE_LEVELING 1
#undef ENABLE_PHY_RX_INPUT_OFFSET
#define ENABLE_PHY_RX_INPUT_OFFSET  0

//#undef REDUCE_LOG_FOR_PRELOADER
//#define REDUCE_LOG_FOR_PRELOADER 0

#undef REDUCE_CALIBRATION_OLYMPUS_ONLY
#define REDUCE_CALIBRATION_OLYMPUS_ONLY 0

#undef APPLY_LOWPOWER_GOLDEN_SETTINGS
#define APPLY_LOWPOWER_GOLDEN_SETTINGS 1

#undef TX_K_DQM_WITH_WDBI
#define TX_K_DQM_WITH_WDBI 0

#undef EYESCAN_LOG
#define EYESCAN_LOG 0

#undef PA_IMPROVEMENT_FOR_DRAMC_ACTIVE_POWER
#define PA_IMPROVEMENT_FOR_DRAMC_ACTIVE_POWER 0

#undef ENABLE_TX_TRACKING
#undef ENABLE_SW_TX_TRACKING
#define ENABLE_TX_TRACKING  0
#define ENABLE_SW_TX_TRACKING 0

#undef ENABLE_RX_TRACKING_LP4
#define ENABLE_RX_TRACKING_LP4  0

#undef ENABLE_PER_BANK_REFRESH
#define ENABLE_PER_BANK_REFRESH 1

#undef HW_GATING
#undef DUMMY_READ_FOR_TRACKING
//#undef ZQCS_ENABLE_LP3
//#undef ZQCS_ENABLE_LP4
//#undef SPM_CONTROL_AFTERK
#undef TEMP_SENSOR_ENABLE
#undef IMPEDANCE_TRACKING_ENABLE
#undef ENABLE_SW_RUN_TIME_ZQ_WA
#define ENABLE_SW_RUN_TIME_ZQ_WA 0

#undef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY

#if 0
#undef XRTW2W_PERFORM_ENHANCE_TX
#undef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
#ifdef XRTR2W_PERFORM_ENHANCE_RODTEN
#undef XRTR2W_PERFORM_ENHANCE_RODTEN     //conflict with ENABLE_RODT_TRACKING, LP4 support only
#endif
#endif
#endif //FIRST_BRING_UP

//=============================================================================
// for D Sim sumulation used
//=============================================================================
#if SW_CHANGE_FOR_SIMULATION
#define SIMULATION_LP4_ZQ 0
#define SIMULATION_RX_INPUT_BUF  0          //only LP4
#define SIMULATION_LP3_CA_TRAINING 0
#define SIMUILATION_LP4_CBT 0
#define SIMULATION_WRITE_LEVELING  0
#define SIMULATION_GATING 0
#define SIMUILATION_LP4_RDDQC 0
#define SIMULATION_DATLAT 0
#define SIMULATION_SW_IMPED 0
#define SIMULATION_RX_PERBIT    0
#define SIMULATION_TX_PERBIT    0  // Please enable with write leveling
#else
#define SIMULATION_LP4_ZQ 1
#define SIMULATION_RX_INPUT_BUF  0
#define SIMULATION_LP3_CA_TRAINING 1
#define SIMUILATION_LP4_CBT 1
#define SIMULATION_WRITE_LEVELING  1
#define SIMULATION_GATING 1
#define SIMUILATION_LP4_RDDQC 1
#define SIMULATION_DATLAT 1
#define SIMULATION_SW_IMPED 1
#define SIMULATION_RX_PERBIT    1
#define SIMULATION_TX_PERBIT    1  // Please enable with write leveling
#endif

#if (!PRINT_CALIBRATION_SUMMARY)
#define vSetCalibrationResult(x,y,z)
#endif

//#define DDR_INIT_TIME_PROFILING

//======================== ETT Definition =====================================
#if __ETT__
//#define TEST_MODE_MRS
#define VENDER_JV_LOG 0
#if DUAL_RANK_ENABLE
#define LP3_DUAL_RANK_RX_K
#define LP3_DUAL_RANK_TX_K
#endif
#define ENABLE_MPLL_SSC 1 // only for Everest ETT
#else // not ETT
#define ENABLE_MPLL_SSC 0 // not for android. never enable.
#endif

#if SW_CHANGE_FOR_SIMULATION
#define VENDER_JV_LOG 0

#undef ENABLE_SW_TX_TRACKING
#define ENABLE_TX_TRACKING  0

#undef HW_GATING
#undef DUMMY_READ_FOR_TRACKING
//#undef ZQCS_ENABLE_LP3
//#undef ZQCS_ENABLE_LP4
#undef SPM_CONTROL_AFTERK
#undef TEMP_SENSOR_ENABLE
#undef IMPEDANCE_TRACKING_ENABLE
#endif
//=============================================================================

//======================== FLASH TOOL DA Definition =====================================
#if __FLASH_TOOL_DA__

#undef DUAL_FREQ_K
#define DUAL_FREQ_K   0        // If enable, need to define DDR_xxxx the same as DUAL_FREQ_HIGH

#undef SUPPORT_SAVE_TIME_FOR_CALIBRATION
#define SUPPORT_SAVE_TIME_FOR_CALIBRATION 0

#endif
//=============================================================================

//======================== RSHMOO Definition =====================================
#if RUNTIME_SHMOO_RELEATED_FUNCTION    //if enable rshmoo, close TX OE calibration
#undef TX_OE_EXTEND
#define TX_OE_EXTEND 1
#undef TX_DQ_OE_SHIFT_LP4
#define TX_DQ_OE_SHIFT_LP4 4
#undef TX_OE_CALIBATION
#define TX_OE_CALIBATION (!TX_OE_EXTEND)

#define RUNTIME_SHMOO_TEST_CHANNEL    0   // 0: CHA, 1: CHB
#define RUNTIME_SHMOO_TEST_RANK       0   // 0: RK0, 1: RK1
#define RUNTIME_SHMOO_TEST_BYTE       0   // 0: Byte0, 1: Byte1

#define RUNTIME_SHMOO_TEST_PI_DELAY_START 0   // 0~63
#define RUNTIME_SHMOO_TEST_PI_DELAY_END   63  // 0~63
#define RUNTIME_SHMOO_TEST_PI_DELAY_STEP  1

#define RUNTIME_SHMOO_TEST_VREF_START     0   // 0~81  : 0~50 is range 0, 51~81 is range 1
#define RUNTIME_SHMOO_TEST_VREF_END       81  // 0~81  : 0~50 is range 0, 51~81 is range 1
#define RUNTIME_SHMOO_TEST_VREF_STEP      1
#endif
//=============================================================================

//============================ For Future Definition =================================

#define ENABLE_DVFS_CDC_SYNCHRONIZER_OPTION 0
#define ENABLE_DVFS_BYPASS_MR13_FSP 0
#define FIX_CROSSRK_XRT_05T_OPT 0

//=============================================================================

#if SW_CHANGE_FOR_SIMULATION
typedef signed char     INT8;
typedef signed short    INT16;
typedef signed int      INT32;
typedef signed char     S8;
typedef signed short    S16;
typedef signed int      S32;
typedef unsigned char       U8;
typedef unsigned short      U16;
typedef unsigned int        U32;
typedef unsigned char       BOOL;
#define FALSE   0
#define TRUE    1

#define NULL 0
#define DDR_BASE 0x40000000
#endif
#if FOR_DV_SIMULATION_USED==1
#define DDR_BASE 0x40000000
#endif
/***********************************************************************/
/*              Defines                                                */
/***********************************************************************/
#define ENABLE  1
#define DISABLE 0

#define CBT_LOW_FREQ   0
#define CBT_HIGH_FREQ   1

typedef enum
{
    DRAM_OK = 0, // OK
    DRAM_FAIL,    // FAIL
} DRAM_STATUS_T; // DRAM status type

typedef enum
{
    IC_VERSION_E1 =0,
    IC_VERSION_E2,  
    IC_VERSION_E3
} DRAM_IC_VERSION_T;

typedef enum
{
    CKE_FIXOFF = 0,
    CKE_FIXON,
    CKE_DYNAMIC //After CKE FIX on/off, CKE should be returned to dynamic (control by HW)
} CKE_FIX_OPTION;

typedef enum
{
    CKE_WRITE_TO_ONE_CHANNEL = 0, //just need to write CKE FIX register to current channel
    CKE_WRITE_TO_ALL_CHANNEL //need to write CKE FIX register to all channel    
} CKE_FIX_CHANNEL;
    
typedef enum
{
    AD_MPLL_208M_CK=0,
    DA_MPLL_52M_DIV_CK,
    FMEM_CK_BFE_DCM_CH0,
}CLOCK_SRC_T;

typedef enum
{
    DLL_MASTER = 0,
    DLL_SLAVE,
} DRAM_DLL_MODE_T;


typedef enum
{
    LP4_DDR3733=0,
    LP4_DDR3200,
    LP4_DDR2667,
    LP4_DDR2400,
    LP4_DDR2280,
    LP4_DDR1600,
    LP4_DDR800,

    // scy: reduce code size by removing unused LPDDR3 opp
    LP3_DDR1866,
    LP3_DDR1600,
    LP3_DDR1333,
    LP3_DDR1200,
    LP3_DDR933,
    LP3_DDR800,
#if __ETT__
    LJ_DDR1866,
    LJ_DDR1700,
    LJ_DDR1600,
    LJ_DDR933,  //mpdiv2
    LJ_DDR850,  //mpdiv2
    LJ_DDR800,  //mpdiv2
    LJ_MAX_SEL,
    LC_DDR1600,
    LC_DDR1270,
    LC_DDR1066,
    LC_DDR800,  //mpdiv2
    LC_DDR635,  //mpdiv2
    LC_DDR533,  //mpdiv2
#endif
    PLL_FREQ_SEL_MAX
} DRAM_PLL_FREQ_SEL_T; // DRAM DFS type

typedef enum
{
    DRAM_DFS_SHUFFLE_1 = 0,
    DRAM_DFS_SHUFFLE_2,
    DRAM_DFS_SHUFFLE_3,
    DRAM_DFS_SHUFFLE_MAX
} DRAM_DFS_SHUFFLE_TYPE_T; // DRAM SHUFFLE RG type
#define DRAM_LP3_DFS_SHUFFLE_MAX 3


typedef struct _DRAM_DFS_FREQUENCY_TABLE_T
{
    DRAM_PLL_FREQ_SEL_T freq_sel;
    U16 frequency;
    DRAM_DFS_SHUFFLE_TYPE_T shuffleIdx;
} DRAM_DFS_FREQUENCY_TABLE_T;



typedef enum
{
    DRAM_CALIBRATION_ZQ = 0,
    DRAM_CALIBRATION_SW_IMPEDANCE ,
    DRAM_CALIBRATION_CA_TRAIN ,
    DRAM_CALIBRATION_WRITE_LEVEL,
    DRAM_CALIBRATION_GATING,
    DRAM_CALIBRATION_DATLAT,
    DRAM_CALIBRATION_RX_RDDQC,
    DRAM_CALIBRATION_RX_PERBIT,
    DRAM_CALIBRATION_TX_PERBIT,
    DRAM_CALIBRATION_MAX
} DRAM_CALIBRATION_STATUS_T;


typedef enum
{
    DDRPHY_CONF_A = 0,
    DDRPHY_CONF_B,
    DDRPHY_CONF_C,
    DDRPHY_CONF_D,
    DDRPHY_CONF_MAX
} DDRPHY_CONF_T;


typedef enum
{
    CHANNEL_A = 0,
    CHANNEL_B,
    CHANNEL_C,
    CHANNEL_D,
} DRAM_CHANNEL_T;

typedef enum
{
    CHANNEL_SINGLE = 1,
    CHANNEL_DUAL,
    CHANNEL_THIRD,
    CHANNEL_FOURTH
} DRAM_CHANNEL_NUMBER_T;

typedef enum
{
    RANK_0= 0,
    RANK_1,
    RANK_MAX
} DRAM_RANK_T;

typedef enum
{
    RANK_SINGLE = 1,
    RANK_DUAL
} DRAM_RANK_NUMBER_T;


typedef enum
{
    TYPE_DDR1 = 1,
    TYPE_LPDDR2,
    TYPE_LPDDR3,
    TYPE_PCDDR3,
    TYPE_LPDDR4,
    TYPE_LPDDR4X,
    TYPE_LPDDR4P
} DRAM_DRAM_TYPE_T;

/* For faster switching between term and un-term operation
 * FSP_0: For un-terminated freq.
 * FSP_1: For terminated freq.
 */
typedef enum
{
    FSP_0 = 0,
    FSP_1,
    FSP_MAX
} DRAM_FAST_SWITH_POINT_T;

/*
 * Internal CBT mode enum
 * 1. Calibration flow uses vGet_Dram_CBT_Mode to
 *    differentiate between mixed vs non-mixed LP4
 * 2. Declared as dram_cbt_mode[RANK_MAX] internally to
 *    store each rank's CBT mode type
 */
typedef enum
{
    CBT_NORMAL_MODE = 0,
    CBT_BYTE_MODE1
} DRAM_CBT_MODE_T;

/*
 * External CBT mode enum
 * Due to MDL structure compatibility (single field for dram CBT mode),
 * the below enum is used in preloader to differentiate between dram cbt modes
 */
typedef enum
{
    CBT_R0_R1_NORMAL = 0,   // Normal mode
    CBT_R0_R1_BYTE,         // Byte mode
    CBT_R0_NORMAL_R1_BYTE,  // Mixed mode R0: Normal R1: Byte
    CBT_R0_BYTE_R1_NORMAL   // Mixed mode R0: Byte R1: Normal
} DRAM_CBT_MODE_EXTERN_T;

typedef enum
{
    ODT_OFF = 0,
    ODT_ON
} DRAM_ODT_MODE_T;


typedef enum
{
    DBI_OFF = 0,
    DBI_ON
} DRAM_DBI_MODE_T;

typedef enum
{
    DATA_WIDTH_16BIT = 16,
    DATA_WIDTH_32BIT = 32
} DRAM_DATA_WIDTH_T;

typedef enum
{
    GET_MDL_USED = 0,
    NORMAL_USED
} DRAM_INIT_USED_T;

// for A60501 DDR3
typedef enum
{
    PCB_LOC_ASIDE = 0,
    PCB_LOC_BSIDE
} DRAM_PCB_LOC_T;

typedef enum
{
    MODE_1X = 0,
    MODE_2X
} DRAM_DRAM_MODE_T;

typedef enum
{
    PACKAGE_SBS = 0,
    PACKAGE_POP
} DRAM_PACKAGE_T;

typedef enum
{
    TE_OP_WRITE_READ_CHECK = 0,
    TE_OP_READ_CHECK
} DRAM_TE_OP_T;

typedef enum
{
    TEST_ISI_PATTERN = 0, //don't change
    TEST_AUDIO_PATTERN =1, //don't change
    TEST_XTALK_PATTERN =2, //don't change
    TEST_TA1_SIMPLE,
    TEST_TESTPAT4,
    TEST_TESTPAT4_3,
    TEST_MIX_PATTERN,
    TEST_DMA,
} DRAM_TEST_PATTERN_T;

typedef enum
{
    BL_TYPE_4 = 0,
    BL_TYPE_8
} DRAM_BL_TYPE_T;

typedef enum
{
    DLINE_0 = 0,
    DLINE_1,
    DLINE_TOGGLE
} PLL_PHASE_CAL_STATUS_T;

typedef enum
{
    TA43_OP_STOP,
    TA43_OP_CLEAR,
    TA43_OP_RUN,
    TA43_OP_RUNQUIET,
    TA43_OP_UNKNOWN,
} DRAM_TA43_OP_TYPE_T;

// used for record last test pattern in TA
typedef enum
{
    TA_PATTERN_IDLE,
    TA_PATTERN_TA43,
    TA_PATTERN_TA4,
    TA_PATTERN_UNKNOWM,
} DRAM_TA_PATTERN_T;


typedef enum
{
    DMA_PREPARE_DATA_ONLY,
    DMA_CHECK_DATA_ACCESS_ONLY_AND_NO_WAIT,
    DMA_CHECK_COMAPRE_RESULT_ONLY,
    DMA_CHECK_DATA_ACCESS_AND_COMPARE,
} DRAM_DMA_CHECK_RESULT_T;


typedef enum
{
    fcDATLAT_USE_DEFAULT = 0,
    fcDATLAT_USE_RX_SCAN,
    //fcDATLAT_USE_TXRX_SCAN
}DRAM_DATLAT_CALIBRATION_TYTE_T;


typedef enum
{
    TX_DQ_DQS_MOVE_DQ_ONLY = 0,
    TX_DQ_DQS_MOVE_DQM_ONLY,
    TX_DQ_DQS_MOVE_DQ_DQM
} DRAM_TX_PER_BIT_CALIBRATION_TYTE_T;

typedef enum
{
    TX_DQM_WINDOW_SPEC_IN = 0xfe,
    TX_DQM_WINDOW_SPEC_OUT = 0xff
} DRAM_TX_PER_BIT_DQM_WINDOW_RESULT_TYPE_T;

typedef enum
{
    TX_TRIANGLE_EYE_STATE_START = 0,
    TX_TRIANGLE_EYE_STATE_ADD_MARGIN,
    TX_TRIANGLE_EYE_STATE_RESTART,
    TX_TRIANGLE_EYE_STATE_END
} TX_TRIANGLe_EYE_TYPE_T;

typedef enum
{
    HQA_REPORT_FORMAT0 = 0,
    HQA_REPORT_FORMAT1,
    HQA_REPORT_FORMAT2,    
    HQA_REPORT_FORMAT3,   
    HQA_REPORT_FORMAT4,    
    HQA_REPORT_FORMAT5,    
    HQA_REPORT_FORMAT6
}
HQA_REPORT_FORMAT_T;

typedef enum
{
    VREF_RANGE_0= 0,
    VREF_RANGE_1,
    VREF_RANGE_MAX
}DRAM_VREF_RANGE_T;
#define VREF_VOLTAGE_TABLE_NUM 51
#define VREF_VOLTAGE_TABLE_TOTAL_NUM 81

// enum for CKE toggle mode (toggle both ranks 1. at the same time (CKE_RANK_DEPENDENT) 2. individually (CKE_RANK_INDEPENDENT))
typedef enum
{
    CKE_RANK_INDEPENDENT = 0,
    CKE_RANK_DEPENDENT
} CKE_CTRL_MODE_T;

typedef enum
{
    DutyScan_Calibration_K_CLK= 0,
    DutyScan_Calibration_K_DQS,
    DutyScan_Calibration_K_DQ,
    DutyScan_Calibration_K_DQM
}DUTYSCAN_CALIBRATION_FLOW_K_T;

//Definitions to enable specific freq's LP4 ACTiming support (To save code size)
#define SUPPORT_LP4_DDR3733_ACTIM 1
#define SUPPORT_LP4_DDR3200_ACTIM 1
#define SUPPORT_LP4_DDR2667_ACTIM 0
#define SUPPORT_LP4_DDR2400_ACTIM 1
#define SUPPORT_LP4_DDR1600_ACTIM 1

/* Used to keep track the total number of LP4 ACTimings */
/* Since READ_DBI is enable/disabled using preprocessor C define
 * -> Save code size by excluding unneeded ACTimingTable entries
 * Note 1: READ_DBI on/off is for (LP4 data rate >= DDR2667 (FSP1))
 * Must make sure DDR3733 is the 1st entry (DMCATRAIN_INTV is used)
 */
typedef enum
{
#if SUPPORT_LP4_DDR3733_ACTIM
#if ENABLE_READ_DBI
    AC_TIME_LP4_BYTE_DDR3733_RDBI_ON = 0,
    AC_TIME_LP4_NORM_DDR3733_RDBI_ON,
#else //(ENABLE_READ_DBI == 0)
    AC_TIME_LP4_BYTE_DDR3733_RDBI_OFF,
    AC_TIME_LP4_NORM_DDR3733_RDBI_OFF,
#endif //ENABLE_READ_DBI
#endif

#if SUPPORT_LP4_DDR3200_ACTIM
#if ENABLE_READ_DBI
    AC_TIME_LP4_BYTE_DDR3200_RDBI_ON,
    AC_TIME_LP4_NORM_DDR3200_RDBI_ON,
#else //(ENABLE_READ_DBI == 0)
    AC_TIME_LP4_BYTE_DDR3200_RDBI_OFF,
    AC_TIME_LP4_NORM_DDR3200_RDBI_OFF,
#endif //ENABLE_READ_DBI
#endif

#if SUPPORT_LP4_DDR2667_ACTIM
#if ENABLE_READ_DBI
    AC_TIME_LP4_BYTE_DDR2667_RDBI_ON,
    AC_TIME_LP4_NORM_DDR2667_RDBI_ON,
#else //(ENABLE_READ_DBI == 0)
    AC_TIME_LP4_BYTE_DDR2667_RDBI_OFF,
    AC_TIME_LP4_NORM_DDR2667_RDBI_OFF,
#endif //ENABLE_READ_DBI
#endif

#if SUPPORT_LP4_DDR2400_ACTIM
    AC_TIME_LP4_BYTE_DDR2400_RDBI_OFF,
    AC_TIME_LP4_NORM_DDR2400_RDBI_OFF,
#endif

#if SUPPORT_LP4_DDR1600_ACTIM
    AC_TIME_LP4_BYTE_DDR1600_RDBI_OFF,
    AC_TIME_LP4_NORM_DDR1600_RDBI_OFF,
#endif
    AC_TIMING_NUMBER_LP4
} AC_TIMING_LP4_COUNT_TYPE_T;

#if ENABLE_LP3_SW
/* Used to keep track the total number of LP3 ACTimings */
typedef enum
{
    AC_TIME_LP3_DDR1866 = 0,
    AC_TIME_LP3_DDR1600,
    AC_TIME_LP3_DDR1333,
    AC_TIME_LP3_DDR1200,

    AC_TIMING_NUMBER_LP3
} AC_TIMING_LP3_COUNT_TYPE_T;
#else
#define AC_TIMING_NUMBER_LP3    0
#endif

typedef enum
{
    TA2_RKSEL_XRT = 3,
    TA2_RKSEL_HW = 4,
} TA2_RKSEL_TYPE_T;

typedef enum
{
    TA2_PAT_SWITCH_OFF = 0,
    TA2_PAT_SWITCH_ON,
} TA2_PAT_SWITCH_TYPE_T;

//Combine LP3 & LP4 ACTiming count
#define TOTAL_AC_TIMING_NUMBER  (AC_TIMING_NUMBER_LP3 + AC_TIMING_NUMBER_LP4)

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
#if RUNTIME_SHMOO_RELEATED_FUNCTION
typedef struct _RUNTIME_SHMOO_SAVE_PARAMETER_T
{
    U8 flag;
    U16 TX_PI_delay;
    U16 TX_Original_PI_delay;
    U16 TX_DQM_PI_delay;
    U16 TX_Original_DQM_PI_delay;    
    U8 TX_Vref_Range;
    U8 TX_Vref_Value;
    U8 TX_Channel;
    U8 TX_Rank;
    U8 TX_Byte;
} RUNTIME_SHMOO_SAVE_PARAMETER_T;
#endif

typedef struct _SAVE_TIME_FOR_CALIBRATION_T
{
    U8 femmc_Ready;

    // Calibration or not
    U8 Bypass_TXWINDOW[DRAM_DFS_SHUFFLE_MAX];
    U8 Bypass_RXWINDOW[DRAM_DFS_SHUFFLE_MAX];
    U8 Bypass_RDDQC[DRAM_DFS_SHUFFLE_MAX];

    // delay cell time
    U8 ucg_num_dlycell_perT_all[DRAM_DFS_SHUFFLE_MAX][CHANNEL_NUM];
    U16 u2gdelay_cell_ps_all[DRAM_DFS_SHUFFLE_MAX][CHANNEL_NUM];    
    U8 ucnum_dlycell_perT;
    U16 u2DelayCellTimex100;

    // CLK & DQS duty
    S8 s1ClockDuty_clk_delay_cell[CHANNEL_NUM][DRAM_DFS_SHUFFLE_MAX][RANK_MAX];
    S8 s1DQSDuty_clk_delay_cell[CHANNEL_NUM][DRAM_DFS_SHUFFLE_MAX][DQS_NUMBER];

    // CBT
    U8 u1CBTVref_Save[CHANNEL_NUM][RANK_MAX][DRAM_DFS_SHUFFLE_MAX];
    U8 u1CBTClkDelay_Save[CHANNEL_NUM][RANK_MAX][DRAM_DFS_SHUFFLE_MAX];
    U8 u1CBTCmdDelay_Save[CHANNEL_NUM][RANK_MAX][DRAM_DFS_SHUFFLE_MAX];
    U8 u1CBTCsDelay_Save[CHANNEL_NUM][RANK_MAX][DRAM_DFS_SHUFFLE_MAX];
    U8 u1CBTCA_PerBit_DelayLine_Save[CHANNEL_NUM][RANK_MAX][DRAM_DFS_SHUFFLE_MAX][DQS_BIT_NUMBER];

    // Write leveling
    U8 u1WriteLeveling_bypass_Save[CHANNEL_NUM][RANK_MAX][DRAM_DFS_SHUFFLE_MAX][DQS_NUMBER];  //for bypass writeleveling

    // Gating
    U8 u1Gating2T_Save[CHANNEL_NUM][RANK_MAX][DRAM_DFS_SHUFFLE_MAX][DQS_NUMBER];
    U8 u1Gating05T_Save[CHANNEL_NUM][RANK_MAX][DRAM_DFS_SHUFFLE_MAX][DQS_NUMBER];
    U8 u1Gatingfine_tune_Save[CHANNEL_NUM][RANK_MAX][DRAM_DFS_SHUFFLE_MAX][DQS_NUMBER];
    U8 u1Gatingucpass_count_Save[CHANNEL_NUM][RANK_MAX][DRAM_DFS_SHUFFLE_MAX][DQS_NUMBER];

    // TX perbit
    U8 u1TxWindowPerbitVref_Save[CHANNEL_NUM][RANK_MAX][DRAM_DFS_SHUFFLE_MAX];
    U16 u1TxCenter_min_Save[CHANNEL_NUM][RANK_MAX][DRAM_DFS_SHUFFLE_MAX][DQS_NUMBER];
    U16 u1TxCenter_max_Save[CHANNEL_NUM][RANK_MAX][DRAM_DFS_SHUFFLE_MAX][DQS_NUMBER];
    U16 u1Txwin_center_Save[CHANNEL_NUM][RANK_MAX][DRAM_DFS_SHUFFLE_MAX][DQ_DATA_WIDTH];
    U16 u1Txfirst_pass_Save[CHANNEL_NUM][RANK_MAX][DRAM_DFS_SHUFFLE_MAX][DQ_DATA_WIDTH];
    U16 u1Txlast_pass_Save[CHANNEL_NUM][RANK_MAX][DRAM_DFS_SHUFFLE_MAX][DQ_DATA_WIDTH];
    U8 u1TX_PerBit_DelayLine_Save[CHANNEL_NUM][RANK_MAX][DRAM_DFS_SHUFFLE_MAX][DQS_BIT_NUMBER];  

    // Datlat
    U8 u1RxDatlat_Save[CHANNEL_NUM][RANK_MAX][DRAM_DFS_SHUFFLE_MAX];

    // RX perbit
    U8 u1RxWinPerbitVref_Save[CHANNEL_NUM][DRAM_DFS_SHUFFLE_MAX];
    S16 u1RxWinPerbitDQ_firsbypass_Save[CHANNEL_NUM][RANK_MAX][DRAM_DFS_SHUFFLE_MAX][DQ_DATA_WIDTH];  //for bypass rxwindow
    U8 u1RxWinPerbitDQ_lastbypass_Save[CHANNEL_NUM][RANK_MAX][DRAM_DFS_SHUFFLE_MAX][DQ_DATA_WIDTH];  //for bypass rxwindow
#if RUNTIME_SHMOO_RELEATED_FUNCTION
    U8 u1SwImpedanceResule[2][4];
    U32 u4TXRG_Backup[CHANNEL_NUM][RUNTIME_SHMOO_RG_BACKUP_NUM];

    RUNTIME_SHMOO_SAVE_PARAMETER_T Runtime_Shmoo_para;
#endif
} SAVE_TIME_FOR_CALIBRATION_T;

extern int write_offline_dram_calibration_data(SAVE_TIME_FOR_CALIBRATION_T *offLine_SaveData);
extern int read_offline_dram_calibration_data(SAVE_TIME_FOR_CALIBRATION_T *offLine_SaveData);
//3733, 3200, 2667, 2400, 2280, 1600, 800
extern U8 _MappingFreqArray[];
#endif


////////////////////////////
typedef struct _DRAMC_CTX_T
{
    DRAM_CHANNEL_NUMBER_T support_channel_num;
    DRAM_CHANNEL_T channel;
    DRAM_RANK_NUMBER_T support_rank_num;
    DRAM_RANK_T rank;
    DRAM_PLL_FREQ_SEL_T freq_sel;
    DRAM_DFS_SHUFFLE_TYPE_T shu_type;
    DRAM_DRAM_TYPE_T dram_type;
    DRAM_FAST_SWITH_POINT_T dram_fsp;  // only for LP4, uesless in LP3
    DRAM_ODT_MODE_T odt_onoff;/// only for LP4, uesless in LP3
    DRAM_CBT_MODE_T dram_cbt_mode[RANK_MAX]; //only for LP4, useless in LP3
    DRAM_DBI_MODE_T DBI_R_onoff[FSP_MAX];   // only for LP4, uesless in LP3
    DRAM_DBI_MODE_T DBI_W_onoff[FSP_MAX];   // only for LP4, uesless in LP3
    DRAM_DATA_WIDTH_T data_width;
    U32 test2_1;
    U32 test2_2;
    DRAM_TEST_PATTERN_T test_pattern;
    U16 frequency;
    U16 freqGroup; /* Used to support freq's that are not in ACTimingTable */
    U16 shuffle_frequency[DRAM_DFS_SHUFFLE_MAX];
    U16 vendor_id;
    U16 revision_id;
    U16 density;
    U64 ranksize[RANK_MAX];
    U8 ucnum_dlycell_perT;
    U16 u2DelayCellTimex100;
    U8 enable_cbt_scan_vref;
    U8 enable_rx_scan_vref;
    U8 enable_tx_scan_vref;
#if PRINT_CALIBRATION_SUMMARY
    U32 aru4CalResultFlag[CHANNEL_NUM][RANK_MAX];// record the calibration is fail or success,  0:success, 1: fail
    U32 aru4CalExecuteFlag[CHANNEL_NUM][RANK_MAX]; // record the calibration is execute or not,  0:no operate, 1: done
#endif
    #if WRITE_LEVELING_MOVE_DQS_INSTEAD_OF_CLK
    BOOL arfgWriteLevelingInitShif[CHANNEL_NUM][RANK_MAX];
    #endif
    #if TX_PERBIT_INIT_FLOW_CONTROL
    BOOL fgTXPerbifInit[CHANNEL_NUM][RANK_MAX];
    #endif
    #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    SAVE_TIME_FOR_CALIBRATION_T *pSavetimeData;
    #endif
    BOOL bDLP3;
} DRAMC_CTX_T;

typedef struct _PASS_WIN_DATA_T
{
    S16 first_pass;
    S16 last_pass;
    S16 win_center;
    U16 win_size;
    U16 best_dqdly;
} PASS_WIN_DATA_T;

#if 0
typedef struct _RXDQS_PERBIT_DLY_T
{
    S8 first_dqdly_pass;
    S8 last_dqdly_pass;
    S8 first_dqsdly_pass;
    S8 last_dqsdly_pass;
    U8 best_dqdly;
    U8 best_dqsdly;
} RXDQS_PERBIT_DLY_T;
#endif
typedef struct _TXDQS_PERBIT_DLY_T
{
    S8 first_dqdly_pass;
    S8 last_dqdly_pass;
    S8 first_dqsdly_pass;
    S8 last_dqsdly_pass;
    U8 best_dqdly;
    U8 best_dqsdly;
} TXDQS_PERBIT_DLY_T;

typedef struct _DRAM_INFO_BY_MRR_T
{
    U16 u2MR5VendorID;
    U16 u2MR6RevisionID;
    U64 u8MR8Density[CHANNEL_NUM][RANK_MAX];
} DRAM_INFO_BY_MRR_T;

typedef struct _VCORE_DELAYCELL_T
{
    U16 u2Vcore;
    U16 u2DelayCell;
} VCORE_DELAYCELL_T;

/////////////////////////////////// SAVE TO SRAM FORMAT ////////////////////////////////////////////
typedef struct _SAVE_TO_SRAM_FORMAT_SwImpedanceCal_T
{
    U8  DRVP;
    U8  DRVN;
    U8  ODTN;
    U8  Reserved[5];
} SAVE_TO_SRAM_FORMAT_SwImpedanceCal_T;

typedef struct _SAVE_TO_SRAM_FORMAT_CmdBusTraining_T
{
    U8  VrefCA;
    U8  CA_delay;
    U8  Clk_delay;
    U8  CS_delay;
    U8  Reserved[4];

} SAVE_TO_SRAM_FORMAT_CmdBusTraining_T;

typedef struct _SAVE_TO_SRAM_FORMAT_Write_Leveling_T
{
    U8  Clk_delay;
    U8  CA_delay;
    U8  Final_Clk_delay;
    U8  DQS0_delay;
    U8  DQS1_delay;
    U8  DQS2_delay;
    U8  DQS3_delay;
    U8  Reserved[1];
} SAVE_TO_SRAM_FORMAT_Write_Leveling_T;

typedef struct _SAVE_TO_SRAM_FORMAT_GatingWindow_T
{
    U8  DQS0_2T;
    U8  DQS0_05T;
    U8  DQS0_PI;
    U8  DQS1_2T;
    U8  DQS1_05T;
    U8  DQS1_PI;
    U8  Reserved0[2];

    U8  DQS2_2T;
    U8  DQS2_05T;
    U8  DQS2_PI;
    U8  DQS3_2T;
    U8  DQS3_05T;
    U8  DQS3_PI;
    U8  Reserved1[2];

    U8  DQS0_RODT_2T;
    U8  DQS0_RODT_05T;
    U8  DQS1_RODT_2T;
    U8  DQS1_RODT_05T;
    U8  DQS2_RODT_2T;
    U8  DQS2_RODT_05T;
    U8  DQS3_RODT_2T;
    U8  DQS3_RODT_05T;
} SAVE_TO_SRAM_FORMAT_GatingWindow_T;

typedef struct _SAVE_TO_SRAM_FORMAT_RXWinDowPerbitCal_T
{
    U8  DQS0_delay;
    U8  DQS1_delay;
    U8  DQS2_delay;
    U8  DQS3_delay;
    U8  DQM0_delay;
    U8  DQM1_delay;
    U8  DQM2_delay;
    U8  DQM3_delay;

    U8  DQ_delay[32];
} SAVE_TO_SRAM_FORMAT_RXWinDowPerbitCal_T;

typedef struct _SAVE_TO_SRAM_FORMAT_TxWindowPerbitCal_T
{
    U8  PI_DQ_elay[4];
    U8  Large_UI[4];
    U8  Small_UI[4];
    U8  PI[4];
}SAVE_TO_SRAM_FORMAT_TxWindowPerbitCal_T;

typedef struct _SAVE_TO_SRAM_FORMAT_DATLAT_T
{
    U8  best_step;
    U8  Reserved[7];
} SAVE_TO_SRAM_FORMAT_DATLAT_T;

typedef struct _SAVE_TO_SRAM_FORMAT_RANK_T
{
    SAVE_TO_SRAM_FORMAT_SwImpedanceCal_T    SwImpedanceCal;

    SAVE_TO_SRAM_FORMAT_CmdBusTraining_T    CmdBusTraining;

    SAVE_TO_SRAM_FORMAT_Write_Leveling_T    Write_Leveling;

    SAVE_TO_SRAM_FORMAT_GatingWindow_T  GatingWindow;

    SAVE_TO_SRAM_FORMAT_RXWinDowPerbitCal_T    RDDQC_Cali;

    SAVE_TO_SRAM_FORMAT_TxWindowPerbitCal_T TxWindowPerbitCal;

    SAVE_TO_SRAM_FORMAT_DATLAT_T    DATLAT;

    SAVE_TO_SRAM_FORMAT_RXWinDowPerbitCal_T RxWindowPerbitCal;
} SAVE_TO_SRAM_FORMAT_RANK_T;

typedef struct _SAVE_TO_SRAM_FORMAT_RxdqsGatingPostProcess_T
{
    U8  s1ChangeDQSINCTL;
    U8  reg_TX_dly_DQSgated_min;
    U8  u1TXDLY_Cal_min;
    U8  TX_dly_DQSgated_check_min;
    U8  TX_dly_DQSgated_check_max;
    U8  DQSINCTL;
    U8  RANKINCTL;
    U8  u4XRTR2R;
} SAVE_TO_SRAM_FORMAT_RxdqsGatingPostProcess_T;

typedef struct _SAVE_TO_SRAM_FORMAT_DualRankRxdatlatCal_T
{
    U8  RANK0;
    U8  RANK1;
    U8  Final_Datlat;
    U8  Reserved[5];
} SAVE_TO_SRAM_FORMAT_DualRankRxdatlatCal_T;

typedef struct _SAVE_TO_SRAM_FORMAT_CHANNEL_T
{
    SAVE_TO_SRAM_FORMAT_RANK_T                      RANK[RANK_MAX];
    SAVE_TO_SRAM_FORMAT_RxdqsGatingPostProcess_T    RxdqsGatingPostProcess;
    SAVE_TO_SRAM_FORMAT_DualRankRxdatlatCal_T       DualRankRxdatlatCal;
} SAVE_TO_SRAM_FORMAT_CHANNEL_T;

typedef struct _SAVE_TO_SRAM_FORMAT_T
{
    SAVE_TO_SRAM_FORMAT_CHANNEL_T   CHANNEL[CHANNEL_NUM];
} SAVE_TO_SRAM_FORMAT_T;


//For new register access
#define DRAMC_REG_ADDR(offset)    ((p->channel << POS_BANK_NUM)+ (offset))
#define SYS_REG_ADDR(offset)    (offset)

// Different from Pi_calibration.c due to Base address
//#define mcSET_DRAMC_REG_ADDR(offset)    (DRAMC_BASE_ADDRESS | (p->channel << POS_BANK_NUM) | (offset))
#define mcSET_SYS_REG_ADDR(offset)    (DRAMC_BASE_ADDRESS | (offset))
#define mcSET_DRAMC_NAO_REG_ADDR(offset)   (DRAMC_NAO_BASE_ADDRESS | (offset))
#define mcSET_DRAMC_AO_REG_ADDR(offset)   (DRAMC_AO_BASE_ADDRESS | (offset))
//#define mcSET_DRAMC_AO_REG_ADDR_CHC(offset)   ((DRAMC_AO_BASE_ADDRESS + ((U32)CHANNEL_C << POS_BANK_NUM)) | (offset))
#define mcSET_DDRPHY_REG_ADDR(offset) (DDRPHY_BASE_ADDR | (offset))
#define mcSET_DDRPHY_REG_ADDR_CHA(offset) ((DDRPHY_BASE_ADDR + ((U32) CHANNEL_A << POS_BANK_NUM)) | (offset))
#define mcSET_DDRPHY_REG_ADDR_CHB(offset) ((DDRPHY_BASE_ADDR + ((U32) CHANNEL_B << POS_BANK_NUM)) | (offset))
//#define mcSET_DDRPHY_REG_ADDR_CHC(offset) ((DDRPHY_BASE_ADDR + ((U32) CHANNEL_C << POS_BANK_NUM)) | (offset))
//#define mcSET_DDRPHY_REG_ADDR_CHD(offset) ((DDRPHY_BASE_ADDR + ((U32) CHANNEL_D << POS_BANK_NUM)) | (offset))

/***********************************************************************/
/*              External declarations                                  */
/***********************************************************************/
extern U8 RXPERBIT_LOG_PRINT;

/***********************************************************************/
/*              Public Functions                                       */
/***********************************************************************/
// basic function
//DRAM_IC_VERSION_T u1GetICVersion_Bianco(void);
U8 u1IsLP4Family(DRAM_DRAM_TYPE_T dram_type);
void setFreqGroup(DRAMC_CTX_T *p); /* Used to support freq's not in ACTimingTable */
void vDramcInit_PreSettings(DRAMC_CTX_T *p);
DRAM_STATUS_T DramcInit(DRAMC_CTX_T *p);

void DramcSetRankEngine2(DRAMC_CTX_T *p, U8 u1RankSel);
DRAM_STATUS_T DramcEngine2Init(DRAMC_CTX_T *p, U32 test2_1, U32 test2_2, U8 testaudpat, U8 log2loopcount);
U32 DramcEngine2Run(DRAMC_CTX_T *p, DRAM_TE_OP_T wr, U8 testaudpat);
void DramcEngine2End(DRAMC_CTX_T *p);
U32 TestEngineCompare(DRAMC_CTX_T *p);
void DramcEnterSelfRefresh(DRAMC_CTX_T *p, U8 op);
void DramcRunTimeConfig(DRAMC_CTX_T *p);
void TransferToSPMControl(DRAMC_CTX_T *p);
#if (SW_CHANGE_FOR_SIMULATION==0)
void EnableDramcPhyDCM(DRAMC_CTX_T *p, BOOL bEn);
#endif

void vSetDramMRWriteLevelingOnOff(DRAMC_CTX_T *p, U8 u1OnOff);

void MoveDramC_TX_DQS(DRAMC_CTX_T *p, U8 u1ByteIdx, S8 iShiftUI);
void MoveDramC_TX_DQS_OEN(DRAMC_CTX_T *p, U8 u1ByteIdx, S8 iShiftUI);
void MoveDramC_TX_DQ(DRAMC_CTX_T *p, U8 u1ByteIdx, S8 iShiftUI);

void CKEFixOnOff(DRAMC_CTX_T *p, CKE_FIX_OPTION option, CKE_FIX_CHANNEL WriteChannelNUM);
// Control CKE toggle mode (toggle both ranks 1. at the same time (CKE_RANK_DEPENDENT) 2. individually (CKE_RANK_INDEPENDENT))
void vCKERankCtrl(DRAMC_CTX_T *p, CKE_CTRL_MODE_T RankCtrlMode);

/* RxDQSIsiPulseCG() - API for "RX DQS ISI pulse CG function" 0: disable, 1: enable */
void RxDQSIsiPulseCG(DRAMC_CTX_T *p, U8 u1OnOff);

DRAM_STATUS_T DramcRegDump(DRAMC_CTX_T *p);

void DramcModeRegReadByRank(DRAMC_CTX_T *p, U8 u1Rank, U8 u1MRIdx, U16 *u2pValue);
void DramcModeRegRead(DRAMC_CTX_T *p, U8 u1MRIdx, U16 *u1pValue);
void DramcModeRegWrite(DRAMC_CTX_T *p, U8 u1MRIdx, U8 u1Value);

void DramPhyReset(DRAMC_CTX_T *p);
U8 u1GetMR4RefreshRate(DRAMC_CTX_T *p, DRAM_CHANNEL_T channel);

// mandatory calibration function
DRAM_STATUS_T DramcDQSOSCAuto(DRAMC_CTX_T *p);
DRAM_STATUS_T DramcStartDQSOSC(DRAMC_CTX_T *p);
DRAM_STATUS_T DramcStopDQSOSC(DRAMC_CTX_T *p);
DRAM_STATUS_T DramcZQCalibration(DRAMC_CTX_T *p);
DRAM_STATUS_T DramcRXInputBufferOffsetCal(DRAMC_CTX_T *p);
DRAM_STATUS_T DramcSwImpedanceCal(DRAMC_CTX_T *p, U8 u1Para, U8 term_option);
DRAM_STATUS_T DramcWriteLevelingLP3(DRAMC_CTX_T *p);
#if GATING_ONLY_FOR_DEBUG
void DramcGatingDebugInit(DRAMC_CTX_T *p);
void DramcGatingDebugRankSel(DRAMC_CTX_T *p, U8 u1Rank);
void DramcGatingDebug(DRAMC_CTX_T *p, U8 u1Channel);
void DramcGatingDebugExit(DRAMC_CTX_T *p);
#endif

void vApplyConfigBeforeCalibration(DRAMC_CTX_T *p);
void vApplyConfigAfterCalibration(DRAMC_CTX_T *p);
void DramcRxdqsGatingPreProcess(DRAMC_CTX_T *p);
void DramcRxdqsGatingPostProcess(DRAMC_CTX_T *p);
DRAM_STATUS_T DramcRxWindowPerbitCal(DRAMC_CTX_T *p, U8 u1UseTestEngine);
void DramcRxdatlatCal(DRAMC_CTX_T *p);
DRAM_STATUS_T DramcTxWindowPerbitCal(DRAMC_CTX_T *p, DRAM_TX_PER_BIT_CALIBRATION_TYTE_T calType, U8 u1VrefScanEnable);
void Dram_Reset(DRAMC_CTX_T *p);

U16 u2DFSGetHighestFreq(DRAMC_CTX_T * p);

#if ENABLE_RX_TRACKING_LP4
void DramcRxInputDelayTrackingInit_byFreq(DRAMC_CTX_T *p);
void DramcRxInputDelayTrackingInit_Common(DRAMC_CTX_T *p);
void DramcRxInputDelayTrackingHW(DRAMC_CTX_T *p);
#endif

#if DUAL_FREQ_K
void DramcDFS(DRAMC_CTX_T *p, int iDoDMA);
void DFSSwitchFreq(DRAMC_CTX_T *p);
void DramcSaveFreqSetting(DRAMC_CTX_T *p);
void DramcDumpFreqSetting(DRAMC_CTX_T *p);
void DramcRestorePLLSetting(DRAMC_CTX_T *p);
void DramcRestoreFreqSetting(DRAMC_CTX_T *p);
void DramcExchangeFreqSetting(DRAMC_CTX_T *p);
DRAM_STATUS_T DramcRestoreGatingTrackingToRG(DRAMC_CTX_T *p);
#endif

void DramcHWGatingInit(DRAMC_CTX_T *p);
void DramcHWGatingOnOff(DRAMC_CTX_T *p, U8 u1OnOff);
void DramcPrintHWGatingStatus(DRAMC_CTX_T *p, U8 u1Channel);

// reference function
DRAM_STATUS_T DramcRankSwap(DRAMC_CTX_T *p, U8 u1Rank);

// DDR reserved mode function
void Dramc_DDR_Reserved_Mode_setting(void);

// dump all reg for debug
extern void RISCReadAll(void);
#if ENABLE_DDRPHY_FREQ_METER
extern void DDRPhyFreqMeter();
#else
#define DDRPhyFreqMeter(_x_)
#endif
extern void GetPhyPllFrequency(DRAMC_CTX_T *p);
extern void vSetPHY2ChannelMapping(DRAMC_CTX_T *p, U8 u1Channel);
#if (SW_CHANGE_FOR_SIMULATION || FOR_DV_SIMULATION_USED)
extern void vMR2InitForSimulationTest(DRAMC_CTX_T *p);
#endif
DRAM_STATUS_T CmdBusTrainingLP4(DRAMC_CTX_T *p);

void MPLLInit(void);

void vSetChannelNumber(DRAMC_CTX_T *p);
void vSetRankNumber(DRAMC_CTX_T *p);
void vSetPHY2ChannelMapping(DRAMC_CTX_T *p, U8 u1Channel);
U8 vGetPHY2ChannelMapping(DRAMC_CTX_T *p);
void vSetRank(DRAMC_CTX_T *p, U8 ucRank);
U8 u1GetRank(DRAMC_CTX_T *p);

void vIO32WriteFldAlign_Phy_All2(DRAMC_CTX_T *p, U32 reg32, U32 val32, U32 fld);
void vIO32WriteFldAlign_Phy_Byte2(DRAMC_CTX_T *p, U8 ucByteIdx, U32 reg32, U32 val32, U32 fld);
U32 u4IO32ReadFldAlign_Phy_Byte2(DRAMC_CTX_T *p, U8 ucByteIdx, U32 reg32, U32 fld);
#define vIO32WriteFldAlign_Phy_All(reg32, val32, fld) vIO32WriteFldAlign_Phy_All2(p, reg32, val32, fld)
#define vIO32WriteFldAlign_Phy_Byte(ucByteIdx, reg32, val32, fld) vIO32WriteFldAlign_Phy_Byte2(p, ucByteIdx, reg32, val32, fld)
#define u4IO32ReadFldAlign_Phy_Byte(ucByteIdx, reg32, fld) u4IO32ReadFldAlign_Phy_Byte2(p, ucByteIdx, reg32, fld)


void DramcHwDQSOSC(DRAMC_CTX_T *p);

#ifdef MPC_SW_FIFO
void DramcMPC_FIFO(DRAMC_CTX_T *p);
#endif

#ifdef ENABLE_POST_PACKAGE_REPAIR
void DramcPostPackageRepair(DRAMC_CTX_T *p);
#endif

void print_HQA_measure_message(DRAMC_CTX_T * p);

void DramcWriteDBIOnOff(DRAMC_CTX_T *p, U8 onoff);
void DramcReadDBIOnOff(DRAMC_CTX_T *p, U8 onoff);
void DramcDRS(DRAMC_CTX_T *p, U8 bEnable);

void SPM_Pinmux_Setting(DRAMC_CTX_T * p);

/* Macro that implements ceil function for unsigned integers (Test before using!) */
#define CEILING(n, v) (((n)%(v)) ? (((n)/(v)) + 1) : ((n)/(v)))

// Global variables
#ifdef _WIN32
extern FILE *fp_A60501;
#endif

#ifdef DDR_INIT_TIME_PROFILING
void TimeProfileBegin(void);
UINT32 TimeProfileEnd(void);
#endif


extern U8 u1DMAtest;
extern U8 u1MR02Value[FSP_MAX];
extern U8 u1MR12Value[CHANNEL_NUM][RANK_MAX][FSP_MAX];
extern U8 u1MR13Value[FSP_MAX];
extern U8 u1MR14Value[CHANNEL_NUM][RANK_MAX][FSP_MAX];
extern U8 u1MR01Value[FSP_MAX];
extern U8 u1MR03Value[FSP_MAX];
extern DRAM_DFS_FREQUENCY_TABLE_T gFreqTbl[DRAM_DFS_SHUFFLE_MAX];
extern U8 u1PrintModeRegWrite;

#if APPLY_SIGNAL_WAVEFORM_SETTINGS_ADJUST
extern S8 gDramcSwImpedanceAdjust[CHANNEL_NUM][4];
extern S8 gDramcDqOdtRZQAdjust;
extern S8 gDramcMR03PDDSAdjust[FSP_MAX];
extern S8 gDramcMR22SoCODTAdjust[FSP_MAX];
#endif

extern U8 gEye_Scan_color_flag;
extern U8 gCBT_EYE_Scan_flag;
extern U8 gCBT_EYE_Scan_only_higheset_freq_flag;
extern U8 gRX_EYE_Scan_flag;
extern U8 gRX_EYE_Scan_only_higheset_freq_flag;
extern U8 gTX_EYE_Scan_flag;
extern U8 gTX_EYE_Scan_only_higheset_freq_flag;

#ifdef FOR_HQA_REPORT_USED
void HQA_Log_Message_for_Report(DRAMC_CTX_T *p, U8 u1ChannelIdx, U8 u1RankIdx, U32 who_am_I, U8 *main_str, U8 byte_bit_idx, S32 value1, U8 *ans_str);
#endif

#if defined(ENABLE_DRAM_SLT)
void O1Path_Test(DRAMC_CTX_T * p);
#endif
#if DRAMC_MODEREG_CHECK
void DramcModeReg_Check(DRAMC_CTX_T *p);
#endif
#if RUNTIME_SHMOO_RELEATED_FUNCTION && SUPPORT_SAVE_TIME_FOR_CALIBRATION
void DramcRunTimeShmooRG_BackupRestore(DRAMC_CTX_T *p);
#endif

void TA2_Test_Run_Time_HW_Set_Column_Num(DRAMC_CTX_T * p);
void TA2_Test_Run_Time_HW_Presetting(DRAMC_CTX_T * p, U32 len, TA2_RKSEL_TYPE_T rksel_mode);
void TA2_Test_Run_Time_Pat_Setting(DRAMC_CTX_T *p, U8 PatSwitch);
void TA2_Test_Run_Time_HW_Write(DRAMC_CTX_T * p, U8 u1Enable);
void TA2_Test_Run_Time_HW_Status(DRAMC_CTX_T * p);
void TA2_Test_Run_Time_HW(DRAMC_CTX_T * p);

#endif // _PI_API_H
