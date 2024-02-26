#ifndef _ACTIMING_H
#define _ACTIMING_H

/***********************************************************************/
/*              Includes                                               */
/***********************************************************************/
#include "dramc_register.h"


//Definitions to enable specific freq's LP4 ACTiming support (To save code size)
#define SUPPORT_LP4_DDR4266_ACTIM 1
#define SUPPORT_LP4_DDR3733_ACTIM 1
#define SUPPORT_LP4_DDR3200_ACTIM 1
#define SUPPORT_LP4_DDR2667_ACTIM 0
#define SUPPORT_LP4_DDR2400_ACTIM 1
#define SUPPORT_LP4_DDR1600_ACTIM 1
#define SUPPORT_LP4_DDR1200_ACTIM 1
#define SUPPORT_LP4_DDR800_ACTIM 1

/* Used to keep track the total number of LP4 ACTimings */
/* Since READ_DBI is enable/disabled using preprocessor C define
 * -> Save code size by excluding unneeded ACTimingTable entries
 * Note 1: READ_DBI on/off is for (LP4 data rate >= DDR2667 (FSP1))
 * Must make sure DDR3733 is the 1st entry (DMCATRAIN_INTV is used)
 */
typedef enum
{
#if SUPPORT_LP4_DDR4266_ACTIM
#if ENABLE_READ_DBI
    AC_TIME_LP4_BYTE_DDR4266_RDBI_ON = 0,
    AC_TIME_LP4_NORM_DDR4266_RDBI_ON,
#else //(ENABLE_READ_DBI == 0)
    AC_TIME_LP4_BYTE_DDR4266_RDBI_OFF,
    AC_TIME_LP4_NORM_DDR4266_RDBI_OFF,
#endif //ENABLE_READ_DBI
#endif

#if SUPPORT_LP4_DDR3733_ACTIM
#if ENABLE_READ_DBI
    AC_TIME_LP4_BYTE_DDR3733_RDBI_ON,
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
    AC_TIME_LP4_BYTE_DDR1600_DIV4_RDBI_OFF,
    AC_TIME_LP4_NORM_DDR1600_DIV4_RDBI_OFF,
#endif

#if SUPPORT_LP4_DDR1200_ACTIM
    AC_TIME_LP4_BYTE_DDR1200_RDBI_OFF,
    AC_TIME_LP4_NORM_DDR1200_RDBI_OFF,
#endif

#if SUPPORT_LP4_DDR800_ACTIM
    AC_TIME_LP4_BYTE_DDR800_RDBI_OFF,
    AC_TIME_LP4_NORM_DDR800_RDBI_OFF,
#endif

    AC_TIMING_NUMBER_LP4
} AC_TIMING_LP4_COUNT_TYPE_T;

/* ACTiming struct declaration (declared here due Fld_wid for each register type)
 * Should include all fields from ACTiming excel file (And update the correct values in UpdateACTimingReg()
 * Note: DQSINCTL, DATLAT aren't in ACTiming excel file (internal delay parameters)
 */
typedef struct _ACTime_T
{
    U8 dramType, cbtMode, readDBI;
    U8 DivMode;
    U16 freq;
    U8 readLat, writeLat;
    U8 dqsinctl, datlat; //DQSINCTL, DATLAT aren't in ACTiming excel file

    //DRAMC_REG_SHU_ACTIM0 ===================================
    U16 trcd :Fld_wid(SHU_ACTIM0_TRCD);
    U16 trrd :Fld_wid(SHU_ACTIM0_TRRD);
    U16 twr :Fld_wid(SHU_ACTIM0_TWR);
    U16 twtr :Fld_wid(SHU_ACTIM0_TWTR);

    //DRAMC_REG_SHU_ACTIM1 ===================================
    U16 trc :Fld_wid(SHU_ACTIM1_TRC);
    U16 tras :Fld_wid(SHU_ACTIM1_TRAS);
    U16 trp :Fld_wid(SHU_ACTIM1_TRP);
    U16 trpab :Fld_wid(SHU_ACTIM1_TRPAB);

    //DRAMC_REG_SHU_ACTIM2 ===================================
    U16 tfaw :Fld_wid(SHU_ACTIM2_TFAW);

    U16 trtw_ODT_off :Fld_wid(SHU_ACTIM2_TR2W);
    U16 trtw_ODT_on :Fld_wid(SHU_ACTIM2_TR2W);

    U16 trtp :Fld_wid(SHU_ACTIM2_TRTP);
    U16 txp :Fld_wid(SHU_ACTIM2_TXP);

    //DRAMC_REG_SHU_ACTIM3 ===================================
    U16 refcnt :Fld_wid(SHU_ACTIM3_REFCNT);
    U16 trfc :Fld_wid(SHU_ACTIM3_TRFC);
    U16 trfcpb :Fld_wid(SHU_ACTIM3_TRFCPB);

    //DRAMC_REG_SHU_ACTIM4 ===================================
    U16 tzqcs :Fld_wid(SHU_ACTIM4_TZQCS);
    U16 refcnt_fr_clk :Fld_wid(SHU_ACTIM4_REFCNT_FR_CLK);
    U16 txrefcnt :Fld_wid(SHU_ACTIM4_TXREFCNT);

    //DRAMC_REG_SHU_ACTIM5 ===================================
    U16 tmrr2w_ODT_off :Fld_wid(SHU_ACTIM5_TMRR2W);
    U16 tmrr2w_ODT_on :Fld_wid(SHU_ACTIM5_TMRR2W);

    U16 twtpd :Fld_wid(SHU_ACTIM5_TWTPD);
    U16 trtpd:Fld_wid(SHU_ACTIM5_TR2PD);

    #if 0 //Currently not used
    //DRAMC_REG_SHU_ACTIM6
    U16 :Fld_wid(SHU_ACTIM6_BGTRRD);
    U16 :Fld_wid(SHU_ACTIM6_TWRMPR);
    U16 :Fld_wid(SHU_ACTIM6_BGTWTR);
    U16 :Fld_wid(SHU_ACTIM6_BGTCCD);
    #endif
    //DRAMC_REG_SHU_ACTIM_XRT ===================================
    U16 xrtw2w :Fld_wid(SHU_ACTIM_XRT_XRTW2W);
    U16 xrtw2r :Fld_wid(SHU_ACTIM_XRT_XRTW2R);
    U16 xrtr2w :Fld_wid(SHU_ACTIM_XRT_XRTR2W);
    U16 xrtr2r :Fld_wid(SHU_ACTIM_XRT_XRTR2R);

    //DRAMC_REG_SHU_AC_TIME_05T ===================================
    #if 0 //Currently not used
    U16 :Fld_wid(SHU_AC_TIME_05T_XRTW2R_M05T);
    U16 :Fld_wid(SHU_AC_TIME_05T_XRTR2W_05T);
    #endif
    U16 twtr_05T :Fld_wid(SHU_AC_TIME_05T_TWTR_M05T);
    U16 trtw_ODT_off_05T :Fld_wid(SHU_AC_TIME_05T_TR2W_05T);
    U16 trtw_ODT_on_05T :Fld_wid(SHU_AC_TIME_05T_TR2W_05T);
    #if 0 //Currently not used
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

    //Other ACTiming reg fields ===================================
    U16 r_dmcatrain_intv :Fld_wid(CATRAINING1_CATRAIN_INTV);
    U16 r_dmmrw_intv :Fld_wid(SHU_SCINTV_MRW_INTV);
    U16 r_dmfspchg_prdcnt :Fld_wid(SHU_CONF2_FSPCHG_PRDCNT);
    U16 ckeprd :Fld_wid(SHU_CKECTRL_CKEPRD);
    U16 ckelckcnt :Fld_wid(CKECTRL_CKELCKCNT);
    U16 earlyckecnt :Fld_wid(SHU_ACTIM2_EARLYCKECNT);
    U16 zqlat2 :Fld_wid(SHU_SCINTV_TZQLAT2);
    //K_rug
    U16 tpbr2pbr :Fld_wid(SHU_ACTIM5_TPBR2PBR);
    U16 tpbr2pbr_05T :Fld_wid(SHU_ACTIM5_TPBR2PBR_05T);

} ACTime_T;


//ACTimingTbl[] forward declaration
extern const ACTime_T ACTimingTbl[TOTAL_AC_TIMING_NUMBER];

extern U8 vDramcACTimingGetDatLat(DRAMC_CTX_T *p);

#endif // _PI_API_H
