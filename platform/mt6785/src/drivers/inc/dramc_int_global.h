#ifndef _INT_GLOBAL_H
#define _INT_GLOBAL_H

extern DRAMC_CTX_T *psCurrDramCtx;
extern DRAMC_CTX_T gTimeProfilingDramCtx;
extern DRAMC_CTX_T DramCtx_LPDDR4;
extern U8 gfirst_init_flag;

//DVT test
#define DVT_TEST_DUMMY_RD_SIDEBAND_FROM_SPM 0
#define DVT_TEST_DUMMY_READ_FOR_DQS_GATING_TRACKING 0
#define DVT_TEST_RX_DLY_HW_TRACKING 0


extern U8 gu1MR23[CHANNEL_NUM][RANK_MAX];
extern U16 gu2MR0_Value[RANK_MAX];  //read only mode register

extern U32 gDramcSwImpedanceResult[IMP_VREF_MAX][IMP_DRV_MAX];//ODT_ON/OFF x DRVP/DRVN/ODTP/ODTN

extern U8 ucg_num_dlycell_perT_all[DRAM_DFS_SHUFFLE_MAX][CHANNEL_NUM];///TODO: to be removed by Francis
extern U16 u2gdelay_cell_ps_all[DRAM_DFS_SHUFFLE_MAX][CHANNEL_NUM];///TODO: to be removed by Francis
extern U16 u2gdelay_cell_ps;


extern U8 gFinalCBTVrefDQ[CHANNEL_NUM][RANK_MAX];
extern U8 gFinalRXVrefDQ[CHANNEL_NUM][RANK_MAX][2];
extern U8 gFinalTXVrefDQ[CHANNEL_NUM][RANK_MAX];

#ifdef FOR_HQA_REPORT_USED
extern U8 gHQALog_flag;
extern U16 gHQALOG_RX_delay_cell_ps_075V;
#endif

#if __ETT__
extern U8 gETT_WHILE_1_flag;
#endif

#if (TDQSCK_PRECALCULATION_FOR_DVFS || defined(HW_GATING))
extern U8 gIsddr800DisGating;
#endif

// --- Eye scan variables -----
extern U8 gEye_Scan_color_flag;
extern U8 gCBT_EYE_Scan_flag;
extern U8 gCBT_EYE_Scan_only_higheset_freq_flag;
extern U8 gRX_EYE_Scan_flag;
extern U8 gRX_EYE_Scan_only_higheset_freq_flag;
extern U8 gTX_EYE_Scan_flag;
extern U8 gTX_EYE_Scan_only_higheset_freq_flag;

#if ENABLE_EYESCAN_GRAPH
#define VREF_TOTAL_NUM_WITH_RANGE ((51+30)+1)/EYESCAN_GRAPH_SAVE_SPACE_VREF_DIV //range0 0~50 + range1 21~50
#define EYESCAN_BROKEN_NUM 3
#define EYESCAN_DATA_INVALID 0x7f
extern S8  gEyeScan_Min[VREF_TOTAL_NUM_WITH_RANGE][DQ_DATA_WIDTH_LP4][EYESCAN_BROKEN_NUM];
extern S8  gEyeScan_Max[VREF_TOTAL_NUM_WITH_RANGE][DQ_DATA_WIDTH_LP4][EYESCAN_BROKEN_NUM];
extern S16 gEyeScan_MinMax_store_delay[DQS_NUMBER];
extern U16  gEyeScan_CaliDelay[DQS_NUMBER];
extern U8  gEyeScan_WinSize[VREF_TOTAL_NUM_WITH_RANGE][DQ_DATA_WIDTH_LP4];
extern S8  gEyeScan_DelayCellPI[DQ_DATA_WIDTH_LP4];
extern U16  gEyeScan_ContinueVrefHeight[DQ_DATA_WIDTH_LP4];
extern U16  gEyeScan_TotalPassCount[DQ_DATA_WIDTH_LP4];
#endif

extern bool gAndroid_DVFS_en;
extern bool gUpdateHighestFreq;


// --- TA2 -----
extern U8 u1MaType;
void TA2_Test_Run_Time_HW_Set_Column_Num(DRAMC_CTX_T * p);
void TA2_Test_Run_Time_HW_Presetting(DRAMC_CTX_T * p, U32 len, TA2_RKSEL_TYPE_T rksel_mode);
void TA2_Test_Run_Time_Pat_Setting(DRAMC_CTX_T *p, U8 PatSwitch);
void TA2_Test_Run_Time_HW_Write(DRAMC_CTX_T * p, U8 u1Enable);
U32 TA2_Test_Run_Time_HW_Status(DRAMC_CTX_T * p);
void TA2_Test_Run_Time_HW(DRAMC_CTX_T * p);

#if ENABLE_EMI_LPBK_TEST
extern U8 gEmiLpbkTest;
#endif

#endif //_INT_GLOBAL_H
