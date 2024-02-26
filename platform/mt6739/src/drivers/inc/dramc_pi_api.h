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

#if(!SW_CHANGE_FOR_SIMULATION)
#if __FLASH_TOOL_DA__
  #include "sw_types.h"
#else
  #include "typedefs.h"
#endif
#endif

//Bring Up Selection : Do Not open it when normal operation
//#define FIRST_BRING_UP

//DRAMC Chip
#define fcA60501    1
#define fcElbrus        3
#define fcOlympus       4
#define fcA60838        5
#define fcWhitney       6
#define fcFOR_CHIP_ID fcWhitney

#define VENDOR_HYNIX 6



#define CHANNEL_NUMBER      4   // 1 single channek, 2 dual channel, 4 4 channel
#define DUAL_RANK_ENABLE 1

#define ORIGINAL_PLL_INIT 0

//Feature option
#define ENABLE_LP4_ZQ_CAL  1
#define ENABLE_CA_TRAINING  1
#define ENABLE_WRITE_LEVELING 1
#define ENABLE_PHY_RX_INPUT_OFFSET  0
#define ENABLE_LOW_POWER_BOOT 0

//SW option
#define DUAL_FREQ_K     1        // If enable, need to define DDR_xxxx the same as DUAL_FREQ_HIGH
#define EVEREST_PORTING_TODO 0
#define ENABLE_LP3_SW 0  // for LP4 only project, can set to 0 to save code size.
#define ENABLE_MRR_AFTER_FIRST_K 1
#define ENABLE_RANK_NUMBER_AUTO_DETECTION 1  //only can enable when DUAL_RANK_ENABLE is 1
#define VREF_SPEED_UP_LP4 1   //if eye scan, disable this option. for Chaowei
#define REDUCE_LOG_FOR_PRELOADER VREF_SPEED_UP_LP4
#define SIMPLIFY_VREF_SCAN 0//DUAL_FREQ_K
#define APPLY_LP4_POWER_INIT_SEQUENCE   1
#define SUPPORT_CBT_K_RANK1 1   // 0: not support 1: support
#define CBT_K_RANK1_USE_METHOD  2   // 1: Method 1, use mpcrk and txrank 2: Method 2, use rankswap
#define MOVE_TX_OE_LONGER 0
#define SUPPORT_HYNIX_RX_DQS_WEAK_PULL 1
#define SUPPORT_PASS_WINDOW_SPEED_UP 0
#define SUPPORT_SAVE_OFF_LINE_CALIBRATION   0
#define SWIMP_LP4X_LP4P_CALI_METHOD   1 //0: old method with fomula  1: new method without fomula

#if !__ETT__
// Preloader: using config CFG_DRAM_CALIB_OPTIMIZATION to identify
#define SUPPORT_SAVE_TIME_FOR_CALIBRATION		1
#define EMMC_READY		CFG_DRAM_CALIB_OPTIMIZATION
#define FORLOOP_TEST_SAVE_TIME_FOR_CALIBRATION_ON_ETT	0
#define BYPASS_VREF_CAL		(SUPPORT_SAVE_TIME_FOR_CALIBRATION & EMMC_READY)
#define BYPASS_CBT		(SUPPORT_SAVE_TIME_FOR_CALIBRATION & EMMC_READY & 0)
#define BYPASS_DATLAT		(SUPPORT_SAVE_TIME_FOR_CALIBRATION & EMMC_READY)
#define BYPASS_WRITELEVELING	(SUPPORT_SAVE_TIME_FOR_CALIBRATION & EMMC_READY)
#define BYPASS_RDDQC		(SUPPORT_SAVE_TIME_FOR_CALIBRATION & EMMC_READY)
#define BYPASS_RXWINDOW		(SUPPORT_SAVE_TIME_FOR_CALIBRATION & EMMC_READY)
#define BYPASS_TXWINDOW		(SUPPORT_SAVE_TIME_FOR_CALIBRATION & EMMC_READY)
#define BYPASS_GatingCal	(SUPPORT_SAVE_TIME_FOR_CALIBRATION & EMMC_READY)
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

#endif


// Sw work around options.
#define WRITE_LEVELING_MOVE_DQS_INSTEAD_OF_CLK 1 // work around for clock multi phase problem(cannot move clk or the clk will be bad)
#define CA_TRAIN_RESULT_DO_NOT_MOVE_CLK 1   // work around for clock multi phase problem(cannot move clk or the clk will be bad)
#define TX_PERBIT_INIT_FLOW_CONTROL 1 // work around for clock multi phase problem(cannot move clk or the clk will be bad)
#define DramcHWDQSGatingTracking_DVT_JADE_TRACKING_MODE 1
#define DramcHWDQSGatingTracking_DVT_FIFO_MODE 0
#define TX_WIDNOW_DELAY_CELL_DQM 0

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
#define TIME_OUT_CNT    100 //100us

// Gating window
#define DQS_GW_COARSE_STEP    1//8 // for testing// 1
#define DQS_GW_FINE_START 0
#define DQS_GW_FINE_END 32
#define DQS_GW_FINE_STEP 4
#define GATING_ADJUST_TXDLY_FOR_TRACKING  1

// DATLAT
#define DATLAT_TAP_NUMBER 32   // DATLAT[3:0] = {0x80[20:4]}

// RX DQ/DQS
#define MAX_RX_DQSDLY_TAPS 127       // 0x018, May set back to 64 if no need.
#define MAX_RX_DQDLY_TAPS 63      // 0x210~0x22c, 0~15 delay tap

// TX DQ/DQS
#define TX_DQ_DELAY_BEGIN_LP4 0//192   // first step to DQ delay
#define TX_DQ_DELAY_BEGIN_LP3 0   // first step to DQ delay
#define TX_DQ_DELAY_END_LP4 308//500   // first step to DQ delay
#define TX_DQ_DELAY_END_LP3 31   // first step to DQ delay
#define MAX_TX_DQDLY_TAPS 31   // max DQ TAP number
#define MAX_TX_DQSDLY_TAPS 31   // max DQS TAP number

// DVFS
// need to review #if & #ifdef, Jeremy
#define DFS_HW_SYNC_GATING_TRACKING 0   //Only for EVEREST
//#define TDQSCK_PRECALCULATION_FOR_DVFS  //DQS pre-calculation
#define APHY_DDR800_FB_MCK_CG_ON 0 //FB_MCK_CG for DDR800 E2

//Run time config
#define HW_GATING //DQS strobe calibration enable
#define GATING_7UI_FOR_TRACKING
#define DUMMY_READ_FOR_TRACKING
#define ENABLE_SW_RUN_TIME_ZQ_WA 0
#if !ENABLE_SW_RUN_TIME_ZQ_WA
//#define ZQCS_ENABLE_LP4   //Whitney E1 can't enable HQZQ,  enable SW ZQ @ kernel.
#endif
#define ZQCS_ENABLE_LP3 //Lewis@20160606: Enable LP3 ZQCS run time since there is no problem.
#define TEMP_SENSOR_ENABLE //After enable rumtime MR4 read, Get DramC SPCMDRESP_REFRESH_RATE.
#if(!SW_CHANGE_FOR_SIMULATION)
#define APPLY_LOWPOWER_GOLDEN_SETTINGS 1 //Low pwer settings
#define SPM_CONTROL_AFTERK //Disable in bring-up and enable thereafter.
#else
#define APPLY_LOWPOWER_GOLDEN_SETTINGS 0 //Low pwer settings
#endif
#define IMPEDENCE_TRACKING_ENABLE //Impendence tracking
//#define IMPEDENCE_HW_SAVING
#define MR13_RRO 1 //MR4 refresh rate option, 0:Disable codes 001 and 010 in MR4 OP[2:0]   1:Enable all codes in MR4 OP[2:0]
#define DUMMY_READ_FOR_DQS_GATING_RETRY //LP4, LP4X: DQS gating retry controlled by SPM side band signal, dummy read rank selection controlled by test agent for DQS gating retry
                                        //LP3: Disable DQS gating retry
#define HW_SAVE_FOR_SR
#define CLK_FREE_FUN_FOR_DRAMC_PSEL //If Psel is 1, clock will be free run at the periof of 2T to let conf be applied. 
                                      //If Psel is 0, Clock will be gated
#define PA_IMPROVEMENT_FOR_DRAMC_ACTIVE_POWER //ACTIVE power defined is smaller than power undefined
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
#define SET_CKE_2_RANK_INDEPENDENT 1
#define ENABLE_RODT_TRACKING    0

//Debug option
#define GATING_ONLY_FOR_DEBUG __ETT__
#define CPU_RW_TEST_AFTER_K 1// need to enable GATING_ONLY_FOR_DEBUG at the same time for gating debug log
#if (FOR_DV_SIMULATION_USED==0)
#define ETT_PRINT_FORMAT  // Apply for both preloader and ETT
#endif
#define LJPLL_FREQ_DEBUG_LOG 0
#define ENABLE_CLAIBTAION_WINDOW_LOG_FOR_FT 0  //FT log for finding out IC with small window size
#if (__ETT__)
#define ENABLE_DDRPHY_FREQ_METER 1
#endif
#define SUPPORT_LP3_800 1  // cross rank parameters are over spec.  cannot support
#define PRINT_CBT_VREFCA_VALUE 0    //print vrefca after calibration

//... add new feature compile option here.
#define RX_DLY_TRACKING_VERIFY 0

//misc feature
//#define CBT_VREFCA_EYE_SCAN
//#define RX_EYE_SCAN
//#define TX_EYE_SCAN
//#define TX_VREFDQ_EYE_SCAN

#define TX_PER_BIT_DELAY_CELL 1
#define CA_PER_BIT_DELAY_CELL 1
//#if (__ETT__ || (TX_PER_BIT_DELAY_CELL==1))
#if (__ETT__)
#define ENABLE_MIOCK_JMETER_HQA
#define ENABLE_MIOCK_JMETER
#endif

#if (__ETT__)
#define DRAM_HQA_USED          // print minium windows percentage and judgement PASS/FAIL
#endif

//#define ENABLE_POST_PACKAGE_REPAIR

#define LP4_CBT_NORMAL_MODE_DQ_PIN_MUX_WORKAROUND 0 //Normal mode can't use DQ to write VREFCA because pin mux over 0~5, so instead of MR12
#define ETT_NO_DRAM 0 //For ETT power measure, test with NO DRAM

//HQA debug option
#define FOR_HQA_TEST_USED    // HQA test used, to print result for easy report

//======================== FIRST_BRING_UP Init Definition =====================
#ifdef FIRST_BRING_UP

//#define USE_CLK26M

#undef DUAL_FREQ_K
#define DUAL_FREQ_K   1        // If enable, need to define DDR_xxxx the same as DUAL_FREQ_HIGH

#undef ORIGINAL_PLL_INIT
#define ORIGINAL_PLL_INIT 0

#undef CHANNEL_NUMBER
#define CHANNEL_NUMBER 4 //work only for Olympus LP4, LP3 is only single channel
#undef DUAL_RANK_ENABLE
#define DUAL_RANK_ENABLE 1

#undef ENABLE_CA_TRAINING
#define ENABLE_CA_TRAINING  1
#undef ENABLE_WRITE_LEVELING
#define ENABLE_WRITE_LEVELING 1
#undef ENABLE_PHY_RX_INPUT_OFFSET
#define ENABLE_PHY_RX_INPUT_OFFSET  0

#undef VREF_SPEED_UP_LP4
#define VREF_SPEED_UP_LP4 0

#undef REDUCE_LOG_FOR_PRELOADER
#define REDUCE_LOG_FOR_PRELOADER 0

#undef REDUCE_CALIBRATION_OLYMPUS_ONLY
#define REDUCE_CALIBRATION_OLYMPUS_ONLY 0

#undef APPLY_LOWPOWER_GOLDEN_SETTINGS
#define APPLY_LOWPOWER_GOLDEN_SETTINGS 1

#undef SUPPORT_HYNIX_RX_DQS_WEAK_PULL
#define SUPPORT_HYNIX_RX_DQS_WEAK_PULL 0

#undef ENABLE_TX_TRACKING
#undef ENABLE_SW_TX_TRACKING
#define ENABLE_TX_TRACKING  0
#define ENABLE_SW_TX_TRACKING 0

#undef ENABLE_RX_TRACKING_LP4
#define ENABLE_RX_TRACKING_LP4  0

#undef ENABLE_PER_BANK_REFRESH
#define ENABLE_PER_BANK_REFRESH 1

//#undef ENABLE_MRR_AFTER_FIRST_K // first calibration for MR5 vender ID and MR8 dram density .
//#define ENABLE_MRR_AFTER_FIRST_K 0

//#undef ENABLE_RANK_NUMBER_AUTO_DETECTION
//#define ENABLE_RANK_NUMBER_AUTO_DETECTION 0

#undef HW_GATING
#undef DUMMY_READ_FOR_TRACKING
//#undef ZQCS_ENABLE_LP3
//#undef ZQCS_ENABLE_LP4
#undef SPM_CONTROL_AFTERK
#undef TEMP_SENSOR_ENABLE
#undef IMPEDENCE_TRACKING_ENABLE
#undef DUMMY_READ_FOR_DQS_GATING_RETRY

#if __ETT__
#define ENABLE_MIOCK_JMETER  // debug purpose, don't enable normally
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

#define vSetCalibrationResult(x,y,z)
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

//======================== ETT Definition =====================================
#if __ETT__ || FOR_DV_SIMULATION_USED==1
#define VER_BASE            (0x08000000)
#define APHW_VER            (VER_BASE + 0x08)
#define APSW_VER            (VER_BASE + 0x0C)
typedef enum {
    CHIP_SW_VER_01 = 0x0000,
    CHIP_SW_VER_02 = 0x0001
} CHIP_SW_VER;

unsigned int mt_get_chip_hw_ver(void);
CHIP_SW_VER mt_get_chip_sw_ver(void);
#endif

#if __ETT__
//#define DDR_INIT_TIME_PROFILING
#define VENDER_JV_LOG 0
#if DUAL_RANK_ENABLE
#define DUAL_RANK_RX_K
#define DUAL_RANK_TX_K
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
#undef IMPEDENCE_TRACKING_ENABLE
#undef DUMMY_READ_FOR_DQS_GATING_RETRY

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
#if SUPPORT_SAVE_OFF_LINE_CALIBRATION
    DRAM_K_VREF
#endif
} DRAM_STATUS_T; // DRAM status type

typedef enum
{
    AD_MPLL_208M_CK=0,
    DA_MPLL_52M_DIV_CK,
    FMEM_CK_BFE_DCM_CH0,
}CLOCK_SRC_T;

typedef enum
{
    LP4_DDR3733=0,
    LP4_DDR3200,
    LP4_DDR2667,
    LP4_DDR2400,
    LP4_DDR1600,
    LP4_DDR800,
#if __ETT__
    // scy: reduce code size by removing unused LPDDR3 opp
    LP3_DDR1866,
    LP3_DDR1600,
    LP3_DDR1333,
    LP3_DDR1200,
    LP3_DDR933,
    LP3_DDR800,
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
    DRAM_DFS_SHUFFLE_4,
    DRAM_DFS_SHUFFLE_MAX
} DRAM_DFS_SHUFFLE_TYPE_T; // DRAM SHUFFLE RG type


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
    CHANNEL_MAX
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

typedef enum
{
    FSP_0 = 0,
    FSP_1,
    FSP_MAX
} DRAM_FAST_SWITH_POINT_T;

typedef enum
{
    CBT_NORMAL_MODE = 0,
    CBT_BYTE_MODE1
} DRAM_CBT_MODE_T;

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
    FIRST_USED = 0,
    GET_MDL_USED,
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
    DMA_OP_PURE_READ,
    DMA_OP_PURE_WRITE,
    DMA_OP_READ_WRITE,
} DRAM_DMA_OP_T;

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

#if SUPPORT_SAVE_OFF_LINE_CALIBRATION
typedef struct _SAVE_OFF_LINE_CALIBRATION_T
{
    U8 u1CAVref_Save[CHANNEL_MAX][RANK_MAX][FSP_MAX];
    U8 u1RXVref_Save[CHANNEL_MAX][RANK_MAX][FSP_MAX];
    U8 u1TXVref_Save[CHANNEL_MAX][RANK_MAX][FSP_MAX];

    U16 u2CAWinSum_Save[CHANNEL_MAX][RANK_MAX][FSP_MAX];
    U16 u2RXWinSum_Save[CHANNEL_MAX][RANK_MAX][FSP_MAX];
    U16 u2TXWinSum_Save[CHANNEL_MAX][RANK_MAX][FSP_MAX];
} SAVE_OFF_LINE_CALIBRATION_T;
#endif

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
typedef struct _SAVE_TIME_FOR_CALIBRATION_T
{
    U8 femmc_Ready;
    U16 fcalfail[DRAM_DFS_SHUFFLE_MAX];
  //  U8 u1WriteLeveling_Save[CHANNEL_MAX][RANK_MAX][PLL_FREQ_SEL_MAX];
  //  U8 u1WriteLevelingoffset;
  //  U8 u1CBTVrefoffset;

    U8 u1Gating2T_Save[CHANNEL_MAX][RANK_MAX][DRAM_DFS_SHUFFLE_MAX][DQS_NUMBER];
    U8 u1Gating05T_Save[CHANNEL_MAX][RANK_MAX][DRAM_DFS_SHUFFLE_MAX][DQS_NUMBER];
    U8 u1Gatingfine_tune_Save[CHANNEL_MAX][RANK_MAX][DRAM_DFS_SHUFFLE_MAX][DQS_NUMBER];
    U8 u1Gatingucpass_count_Save[CHANNEL_MAX][RANK_MAX][DRAM_DFS_SHUFFLE_MAX][DQS_NUMBER];
//    U8 u1GatingPass_Win[CHANNEL_MAX][RANK_MAX][PLL_FREQ_SEL_MAX];
//    U8 u1GatingPass_Winoffset;
 //   U8 u1GatingStartoffset;

    U8 u1CBTVref_Save[CHANNEL_MAX][RANK_MAX][DRAM_DFS_SHUFFLE_MAX];


    U8 u1RxDatlat_Save[CHANNEL_MAX][RANK_MAX][DRAM_DFS_SHUFFLE_MAX];
//    U8 u1RxDatlatoffset;

//    S16 u1RxWinPerbit_Min_FirstPass_Save[CHANNEL_MAX][RANK_MAX][PLL_FREQ_SEL_MAX];
//    U8 u1RxWinPerbit_Min_LastPass_Save[CHANNEL_MAX][RANK_MAX][PLL_FREQ_SEL_MAX];
    U8 u1RxWinPerbitVref_Save[CHANNEL_MAX][DRAM_DFS_SHUFFLE_MAX];
//    U8 u1RxWinPerbitoffset;
//    U8 u1RxWinPerbitVrefoffset;
//    U8 RXWindowTHHighSpeed;
//    U8 RXWindowTHLowSpeed;

//    U8 u1TxWindowPerbit_Min_FirstPass_Save[CHANNEL_MAX][RANK_MAX][PLL_FREQ_SEL_MAX];
    U8 u1TxWindowPerbitVref_Save[CHANNEL_MAX][RANK_MAX][DRAM_DFS_SHUFFLE_MAX];
//    U8 TxWindowTH;

//     U8 u1TxWindowPerbitoffset;
//    U8 u1TxWindowPerbititVrefoffset;


       U8 u1WriteLeveling_bypass_Save[CHANNEL_MAX][RANK_MAX][DRAM_DFS_SHUFFLE_MAX][DQS_NUMBER];  //for bypass writeleveling

       S16 u1RxWinPerbitDQ_firsbypass_Save[CHANNEL_MAX][RANK_MAX][DRAM_DFS_SHUFFLE_MAX][DQ_DATA_WIDTH];  //for bypass rxwindow
       U8 u1RxWinPerbitDQ_lastbypass_Save[CHANNEL_MAX][RANK_MAX][DRAM_DFS_SHUFFLE_MAX][DQ_DATA_WIDTH];  //for bypass rxwindow

       
//       U8 u1RxWinPerbitDQS_bypass_Save[CHANNEL_MAX][RANK_MAX][PLL_FREQ_SEL_MAX][DQS_NUMBER];  //for bypass rxwindow
//       U8 u1RxWinPerbitDQM_bypass_Save[CHANNEL_MAX][RANK_MAX][PLL_FREQ_SEL_MAX][DQS_NUMBER];  //for bypass rxwindow
	U8 u1TxCenter_min_Save[CHANNEL_MAX][RANK_MAX][DRAM_DFS_SHUFFLE_MAX][DQS_NUMBER];
	U8 u1TxCenter_max_Save[CHANNEL_MAX][RANK_MAX][DRAM_DFS_SHUFFLE_MAX][DQS_NUMBER];
	U8 u1Txwin_center_Save[CHANNEL_MAX][RANK_MAX][DRAM_DFS_SHUFFLE_MAX][DQ_DATA_WIDTH];
	U8 u1Txfirst_pass_Save[CHANNEL_MAX][RANK_MAX][DRAM_DFS_SHUFFLE_MAX][DQ_DATA_WIDTH];
	U8 u1Txlast_pass_Save[CHANNEL_MAX][RANK_MAX][DRAM_DFS_SHUFFLE_MAX][DQ_DATA_WIDTH];
  	U8 Bypass_TXWINDOW[DRAM_DFS_SHUFFLE_MAX];
  	U8 Bypass_RXWINDOW[DRAM_DFS_SHUFFLE_MAX];
  	U8 Bypass_RDDQC[DRAM_DFS_SHUFFLE_MAX];

    U8 u1CBTClkDelay_Save[CHANNEL_MAX][RANK_MAX][DRAM_DFS_SHUFFLE_MAX];
    U8 u1CBTCmdDelay_Save[CHANNEL_MAX][RANK_MAX][DRAM_DFS_SHUFFLE_MAX];
    U8 u1CBTFinalDelay_Save[CHANNEL_MAX][RANK_MAX][DRAM_DFS_SHUFFLE_MAX];
    U8 u1CBTCA_PerBit_DelayLine_Save[CHANNEL_MAX][RANK_MAX][DRAM_DFS_SHUFFLE_MAX][DQS_BIT_NUMBER];    
} SAVE_TIME_FOR_CALIBRATION_T;

extern int write_offline_dram_calibration_data(SAVE_TIME_FOR_CALIBRATION_T *offLine_SaveData);
extern int read_offline_dram_calibration_data(SAVE_TIME_FOR_CALIBRATION_T *offLine_SaveData);
static const U8 _MappingFreqArray[LP4_DDR800+1]=
{
{0,LP4_DDR3733},
{0,LP4_DDR3200},
{1,LP4_DDR2667},
{0,LP4_DDR2400},
{0,LP4_DDR1600},
{2,LP4_DDR800},
};

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
    DRAM_CBT_MODE_T dram_cbt_mode; //only for LP4, useless in LP3
    DRAM_DBI_MODE_T DBI_R_onoff;   // only for LP4, uesless in LP3
    DRAM_DBI_MODE_T DBI_W_onoff;   // only for LP4, uesless in LP3
    DRAM_PACKAGE_T package;
    DRAM_DATA_WIDTH_T data_width;
    U32 test2_1;
    U32 test2_2;
    DRAM_TEST_PATTERN_T test_pattern;
    U16 frequency;
    U8 vendor_id;
    U16 density;
    U32 ranksize[RANK_MAX];
    U16 u2DelayCellTimex100;
    U8 fglow_freq_write_en;
    U8 ssc_en;
    U8 en_4bitMux;
    U8 enable_cbt_scan_vref;
    U8 enable_rx_scan_vref;
    U8 enable_tx_scan_vref;
#if SUPPORT_SAVE_OFF_LINE_CALIBRATION
    SAVE_OFF_LINE_CALIBRATION_T *pOffLineSaveData;
#endif
    U32 aru4CalResultFlag[CHANNEL_MAX][RANK_MAX][DRAM_DFS_SHUFFLE_MAX];// record the calibration is fail or success,  0:success, 1: fail
    U32 aru4CalExecuteFlag[CHANNEL_MAX][RANK_MAX][DRAM_DFS_SHUFFLE_MAX]; // record the calibration is execute or not,  0:no operate, 1: done

    #if WRITE_LEVELING_MOVE_DQS_INSTEAD_OF_CLK
    BOOL arfgWriteLevelingInitShif[CHANNEL_MAX][RANK_MAX];
    #endif
    #if TX_PERBIT_INIT_FLOW_CONTROL
    BOOL fgTXPerbifInit[CHANNEL_MAX][RANK_MAX];
    #endif
    #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
          SAVE_TIME_FOR_CALIBRATION_T *pSavetimeData;
    #endif
    
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

#if 1//ENABLE_MRR_AFTER_FIRST_K
typedef struct _DRAM_INFO_BY_MRR_T
{
    U16 u2MR5VendorID;
    U32 u4MR8Density[CHANNEL_MAX][RANK_MAX];
} DRAM_INFO_BY_MRR_T;
#endif

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
    SAVE_TO_SRAM_FORMAT_CHANNEL_T   CHANNEL[CHANNEL_MAX];
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
DRAMC_CTX_T *DramcCtxCreate(void);
void vDramcCtxDestroy(DRAMC_CTX_T *p);
void vDramcCtxInit(DRAMC_CTX_T *p);
U8 u1IsLP4Family(DRAM_DRAM_TYPE_T dram_type);
DRAM_STATUS_T MemPllInit(void);
DRAM_STATUS_T SscEnable(DRAMC_CTX_T *p);
DRAM_STATUS_T DramcInit(DRAMC_CTX_T *p);

void DramcSetRankEngine2(DRAMC_CTX_T *p, U8 u1RankSel);
DRAM_STATUS_T DramcEngine2Init(DRAMC_CTX_T *p, U32 test2_1, U32 test2_2, U8 testaudpat, U8 log2loopcount);
U32 DramcEngine2Run(DRAMC_CTX_T *p, DRAM_TE_OP_T wr, U8 testaudpat);
void DramcEngine2End(DRAMC_CTX_T *p);
U32 TestEngineCompare(DRAMC_CTX_T *p);

void DramcEnterSelfRefresh(DRAMC_CTX_T *p, U8 op);
void DramcLowFreqWrite(DRAMC_CTX_T *p);
void DramcRunTimeConfig(DRAMC_CTX_T *p);
void DDRPhyLowPowerEnable(DRAMC_CTX_T *p);
void TransferToSPMControl(DRAMC_CTX_T *p);
#if (SW_CHANGE_FOR_SIMULATION==0)
void EnableDramcPhyDCM(DRAMC_CTX_T *p, BOOL bEn);
#endif

DRAM_STATUS_T DramcRegDump(DRAMC_CTX_T *p);
void DramcTestPat4_3(DRAMC_CTX_T *p, DRAM_TA43_OP_TYPE_T type);
U32 DramcPat4_3(DRAMC_CTX_T *p);
//U32 DramcDmaEngineNoWait(DRAMC_CTX_T *p, DRAM_DMA_OP_T op, U32 src_addr, U32 dst_addr, U32 trans_len, U8 burst_len);
//U32 DramcDmaWaitCompare(DRAMC_CTX_T *p, U32 src_addr, U32 dst_addr, U32 trans_len, U8 burst_len);

void DramcModeRegReadByRank(DRAMC_CTX_T *p, U8 u1Rank, U8 u1MRIdx, U16 *u2pValue);
void DramcModeRegRead(DRAMC_CTX_T *p, U8 u1MRIdx, U16 *u1pValue);
void DramcModeRegWrite(DRAMC_CTX_T *p, U8 u1MRIdx, U8 u1Value);

void DramPhyReset(DRAMC_CTX_T *p);
U8 u1GetMR4RefreshRate(DRAMC_CTX_T *p, DRAM_CHANNEL_T channel);
void vGetImpedanceResult(DRAMC_CTX_T *p, U8 *drvp, U8* drvn);

// mandatory calibration function
DRAM_STATUS_T DramcDQSOSCAuto(DRAMC_CTX_T *p);
DRAM_STATUS_T DramcWriteFifoTest(DRAMC_CTX_T *p);
DRAM_STATUS_T DramcReadFifoTest(DRAMC_CTX_T *p);
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
//DRAM_STATUS_T DramcRxdqsGatingCal_Manual(DRAMC_CTX_T *p, S16 iDelay);
void DramcRxdqsGatingPreProcess(DRAMC_CTX_T *p);
void DramcRxdqsGatingPostProcess(DRAMC_CTX_T *p);
DRAM_STATUS_T DramcRxWindowPerbitCal(DRAMC_CTX_T *p, U8 u1UseTestEngine);
//DRAM_STATUS_T DramcRxWindowPerbitCal_Manual(DRAMC_CTX_T *p, U8 u1UseTestEngine, S16 iDelay);
void DramcRxdatlatCal(DRAMC_CTX_T *p);
DRAM_STATUS_T DramcTxWindowPerbitCal(DRAMC_CTX_T *p, DRAM_TX_PER_BIT_CALIBRATION_TYTE_T calType);
DRAM_STATUS_T DramcCATraining(DRAMC_CTX_T *p);
void Dram_Reset(DRAMC_CTX_T *p);

void DramcDFSSetDMAConfig(U32 src, U32 dst, U32 len, U32 config);
void PMIC_VCORE_ADJUST(U16 targetV);
U16 u2DFSGetHighestFreq(DRAMC_CTX_T * p);

#if ENABLE_RX_TRACKING_LP4
void DramcRxInputDelayTrackingInit_byFreq(DRAMC_CTX_T *p);
void DramcRxInputDelayTrackingInit_Common(DRAMC_CTX_T *p);
void DramcRxInputDelayTrackingHW(DRAMC_CTX_T *p);
#endif

#if DUAL_FREQ_K
void DramcDFS(DRAMC_CTX_T *p);
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

#if 0  //no use now, disable for saving code size.
void vGetCalibrationResult(DRAMC_CTX_T *p, U8 ucCalType, U8 *ucCalExecute, U8 *ucCalResult); // get result of specific calibration
#endif
// get result of all calibration of specific channel/rank/freq.
void vGetCalibrationResult_All(DRAMC_CTX_T *p, U8 u1Channel, U8 u1Rank, U8 u1FreqType, U32 *u4CalExecute, U32 *u4CalResult);
void vPrintCalibrationResult(DRAMC_CTX_T *p);


// reference function
void DramcRxEyeScanInit(DRAMC_CTX_T *p);
DRAM_STATUS_T DramcRxEyeScan(DRAMC_CTX_T *p, U8 boot_or_run, U8 dq_no);
DRAM_STATUS_T DramcTxEyeScan(DRAMC_CTX_T *p);
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
void vIO32WriteFldAlign_Phy_All(U32 reg32, U32 val32, U32 fld);
void vIO32WriteFldAlign_Phy_Byte(U8 ucByteIdx, U32 reg32, U32 val32, U32 fld);
U32 u4IO32ReadFldAlign_Phy_Byte(U8 ucByteIdx, U32 reg32, U32 fld);

void DramcHwDQSOSC(DRAMC_CTX_T *p);
void DramcDFSUpdateFreqRatio(DRAMC_CTX_T *p);

void DramcEnableDBI(DRAMC_CTX_T *p, U8 u1EnableDBI);

#ifdef MPC_SW_FIFO
void DramcMPC_FIFO(DRAMC_CTX_T *p);
#endif

#ifdef ENABLE_POST_PACKAGE_REPAIR
void DramcPostPackageRepair(DRAMC_CTX_T *p);
#endif

U32 get_HQA_shuffle_number(DRAMC_CTX_T *p);
void print_HQA_measure_message(DRAMC_CTX_T * p);

// Global variables
#ifdef _WIN32
extern FILE *fp_A60501;
#endif

#ifdef DDR_INIT_TIME_PROFILING
void TimeProfileBegin(void);
UINT32 TimeProfileEnd(void);
#endif


extern U8 u1DMAtest;
extern U8 u1MR2Value[FSP_MAX];
extern U8 u1MR12Value[CHANNEL_MAX][RANK_MAX][FSP_MAX];
extern U8 u1MR13Value[FSP_MAX];
extern U8 u1MR14Value[CHANNEL_MAX][RANK_MAX][FSP_MAX];
extern U8 u1MR01Value[FSP_MAX];
extern DRAM_DFS_FREQUENCY_TABLE_T gFreqTbl[DRAM_DFS_SHUFFLE_MAX];
#endif // _PI_API_H
