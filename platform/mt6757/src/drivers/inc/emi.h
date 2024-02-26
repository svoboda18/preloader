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
 * The following software/firmware and/or related documentation ("Media Tek
 * Software") have been modified by MediaTek Inc. All revisions are subject to
 * any receiver's applicable license agreements with MediaTek Inc.
 */

#ifndef MT_EMI_H
#define MT_EMI_H
#ifdef FLASH_TOOL_DA
  #include "sw_types.h"
#else
  #include "typedefs.h"
#endif
#include "dramc_common.h"

#if !__ETT__ 
#if (FOR_DV_SIMULATION_USED==0)
#define DDR_RESERVE_MODE
#define COMBO_MCP
#define DRAM_BASE 0x40000000ULL
#define DDR_BASE DRAM_BASE
#define DRAM_CALIB_LOG
#define LAST_DRAMC
#endif
#endif

int get_dram_rank_nr (void);
void get_dram_rank_size(u64 dram_rank_size[]);

typedef struct _AC_TIMING_EXTERNAL_T
{
    // U 00
    U32 AC_TIME_EMI_FREQUENCY      :16;
    U32 AC_TIME_EMI_TRAS           :8;
    U32 AC_TIME_EMI_TRP            :8;
    
    // U 01
    U32 AC_TIME_EMI_TRPAB          :8;
    U32 AC_TIME_EMI_TRC            :8;
    U32 AC_TIME_EMI_TRFC           :8;
    U32 AC_TIME_EMI_TRFCPB         :8;
    
    // U 02
    U32 AC_TIME_EMI_TXP            :8;
    U32 AC_TIME_EMI_TRTP           :8;
    U32 AC_TIME_EMI_TRCD           :8;
    U32 AC_TIME_EMI_TWR            :8;

    // U 03
    U32 AC_TIME_EMI_TWTR           :8;
    U32 AC_TIME_EMI_TRRD           :8;
    U32 AC_TIME_EMI_TFAW           :8;
    U32 AC_TIME_EMI_TRTW_ODT_OFF   :4;
    U32 AC_TIME_EMI_TRTW_ODT_ON    :4;

    // U 04    
    U32 AC_TIME_EMI_REFCNT         :8; //(REFFRERUN = 0)
    U32 AC_TIME_EMI_REFCNT_FR_CLK  :8; //(REFFRERUN = 1)
    U32 AC_TIME_EMI_TXREFCNT       :8;
    U32 AC_TIME_EMI_TZQCS          :8;

    // U 05 
    U32 AC_TIME_EMI_TRTPD            :8; // LP4/LP3, // Olympus new 
    U32 AC_TIME_EMI_TWTPD            :8; // LP4/LP3, // Olympus new 
    U32 AC_TIME_EMI_TMRR2W_ODT_OFF   :8; // LP4      // Olympus new 
    U32 AC_TIME_EMI_TMRR2W_ODT_ON    :8; // LP4      // Olympus new 

    // U 06
    // Byte0
    U32 AC_TIME_EMI_TRAS_05T          :2;
    U32 AC_TIME_EMI_TRP_05T           :2;
    U32 AC_TIME_EMI_TRPAB_05T         :2;
    U32 AC_TIME_EMI_TRC_05T           :2;
    // Byte1	
    U32 AC_TIME_EMI_TRFC_05T          :2;
    U32 AC_TIME_EMI_TRFCPB_05T        :2;
    U32 AC_TIME_EMI_TXP_05T           :2;
    U32 AC_TIME_EMI_TRTP_05T          :2;
    // Byte2	
    U32 AC_TIME_EMI_TRCD_05T          :2;
    U32 AC_TIME_EMI_TWR_05T           :2;
    U32 AC_TIME_EMI_TWTR_05T          :2; // Olympus modified
    U32 AC_TIME_EMI_TRRD_05T          :2;
    // Byte3	
    U32 AC_TIME_EMI_TFAW_05T          :2;
    U32 AC_TIME_EMI_TRTW_ODT_OFF_05T  :2;
    U32 AC_TIME_EMI_TRTW_ODT_ON_05T   :2;
    U32 AC_TIME_EMI_TRTPD_05T         :2; // LP4/LP3 // Olympus new 

    // U 07
    // Byte0
    U32 AC_TIME_EMI_TWTPD_05T           :2; // LP4/LP3 // Olympus new
    U32 AC_TIME_EMI_TMRR2W_ODT_OFF_05T  :2; // Useless, no 0.5T in Olympus and Elbrus
    U32 AC_TIME_EMI_TMRR2W_ODT_ON_05T   :2; // Useless, no 0.5T in Olympus and Elbrus

   
}AC_TIMING_EXTERNAL_T;


typedef struct
{
    int   sub_version;            // sub_version: 0x1 for new version
    int  type;                /* 0x0000 : Invalid
                                 0x0001 : Discrete DDR1
                                 0x0002 : Discrete LPDDR2
                                 0x0003 : Discrete LPDDR3
                                 0x0004 : Discrete PCDDR3
                                 0x0005 : Discrete LPDDR4
                                 0x0006 : Discrete LPDR4X
                                 0x0101 : MCP(NAND+DDR1)
                                 0x0102 : MCP(NAND+LPDDR2)
                                 0x0103 : MCP(NAND+LPDDR3)
                                 0x0104 : MCP(NAND+PCDDR3)
                                 0x0201 : MCP(eMMC+DDR1)
                                 0x0202 : MCP(eMMC+LPDDR2)
                                 0x0203 : MCP(eMMC+LPDDR3)
                                 0x0204 : MCP(eMMC+PCDDR3)
                                 0x0205 : MCP(eMMC+LPDDR4)
                                 0x0206 : MCP(eMMC+LPDR4X)
                              */
    int   id_length;              // EMMC and NAND ID checking length
    int   fw_id_length;              // FW ID checking length
    char  ID[16];
    char  fw_id[8];               // To save fw id
    int   EMI_CONA_VAL;           //@0x3000
    int   EMI_CONH_VAL;
    
    union   {        
        int DRAMC_ACTIME_UNION[8];
    AC_TIMING_EXTERNAL_T AcTimeEMI;
    };       

    u64   DRAM_RANK_SIZE[4];
    int   EMI_CONF_VAL;
    int   CHN0_EMI_CONA_VAL;
    int   CHN1_EMI_CONA_VAL;
    /* Single field to store LP4 dram type (normal, byte, mixed) */
    int   dram_cbt_mode_extern;
    int   reserved[6];

#if 0
    union
    {
        struct
        {
            int   iLPDDR2_MODE_REG_1;
            int   iLPDDR2_MODE_REG_2;
            int   iLPDDR2_MODE_REG_3;
            int   iLPDDR2_MODE_REG_5;
            int   iLPDDR2_MODE_REG_10;
            int   iLPDDR2_MODE_REG_63;
        };
        struct
        {
            int   iDDR1_MODE_REG;
            int   iDDR1_EXT_MODE_REG;
        };
        struct
        {
            int   iPCDDR3_MODE_REG0;
            int   iPCDDR3_MODE_REG1;
            int   iPCDDR3_MODE_REG2;
            int   iPCDDR3_MODE_REG3;
        };
        struct
        {
            int   iLPDDR3_MODE_REG_1;
            int   iLPDDR3_MODE_REG_2;
            int   iLPDDR3_MODE_REG_3;
            int   iLPDDR3_MODE_REG_5;
            int   iLPDDR3_MODE_REG_10;
            int   iLPDDR3_MODE_REG_63;
        };
    };
#else
    int   iLPDDR3_MODE_REG_5;
#endif    
    int   PIN_MUX_TYPE;   
} EMI_SETTINGS;

//typedef  EMI_SETTINGS_v15 EMI_SETTINGS;
#if (FOR_DV_SIMULATION_USED==0)
#if ! __ETT__
uint32 mt_set_emis(uint8* emi, uint32 len, bool use_default); //array of emi setting.
#endif
#endif

extern int num_of_emi_records;
extern int emi_setting_index;
extern EMI_SETTINGS emi_settings[];
extern EMI_SETTINGS default_emi_setting;
extern EMI_SETTINGS emi_setting_default_lpddr3;  
extern EMI_SETTINGS emi_setting_default_lpddr4;  

int mt_get_dram_type (void); 

#ifdef LAST_DRAMC
#include "x_hal_io.h"
#define DRAM_DEBUG_SRAM_BASE 0x11D800
#define DRAM_OPT_DEBUG_SRAM_BASE 0x11D840
#define LAST_DRAMC_SRAM_SIZE 20
//#define DRAM_DEBUG_SRAM_SIZE 1024
#define LAST_DRAMC_MAGIC_PATTERN 0x19870611
static void update_last_dramc_info(void);
void init_ta2_all_channel(void);
void init_ta2_single_channel(unsigned int);
typedef struct {
    unsigned int ta2_result_magic;
    unsigned int ta2_result_last;
    unsigned int ta2_result_past;
    unsigned int ta2_result_checksum;
    unsigned int reboot_count;
} LAST_DRAMC_INFO_T;

#define LAST_DRAM_FATAL_ERR_FLAG		(DRAM_OPT_DEBUG_SRAM_BASE)
#define DRAM_FATAL_ERR_FLAG				(DRAM_OPT_DEBUG_SRAM_BASE + 4)
#define STORAGE_API_ERR_FLAG			(DRAM_OPT_DEBUG_SRAM_BASE + 8)
#define LAST_DRAM_GATING_ERR_CHA_RK0	(DRAM_OPT_DEBUG_SRAM_BASE + 12)
#define LAST_DRAM_GATING_ERR_CHA_RK1	(DRAM_OPT_DEBUG_SRAM_BASE + 16)
#define LAST_DRAM_GATING_ERR_CHB_RK0	(DRAM_OPT_DEBUG_SRAM_BASE + 20)
#define LAST_DRAM_GATING_ERR_CHB_RK1	(DRAM_OPT_DEBUG_SRAM_BASE + 24)
#define DRAM_GATING_ERR_CHA_RK0			(DRAM_OPT_DEBUG_SRAM_BASE + 28)
#define DRAM_GATING_ERR_CHA_RK1			(DRAM_OPT_DEBUG_SRAM_BASE + 32)
#define DRAM_GATING_ERR_CHB_RK0			(DRAM_OPT_DEBUG_SRAM_BASE + 36)
#define DRAM_GATING_ERR_CHB_RK1			(DRAM_OPT_DEBUG_SRAM_BASE + 40)


#define OFFSET_DRAM_FATAL_ERR		(31)
#define OFFSET_DRAM_TA2_ERR		(23)
#define OFFSET_DRAM_GATING_ERR		(7)
#define OFFSET_CPU_RW_ERR		(5)

#define OFFSET_DDR_RSV_MODE_FLOW	(4)
#define OFFSET_DDR_RSV_MODE_ERR		(3)
#define OFFSET_KIBO_PLUS_ONLY		(2)

#define ERR_DRAM_TA2_RK0		(1 << 0)
#define ERR_DRAM_TA2_RK1		(1 << 1)

#define ERR_DRAM_GATING_RK0_R		(1 << 0)
#define ERR_DRAM_GATING_RK0_F		(1 << 1)
#define ERR_DRAM_GATING_RK1_R		(1 << 2)
#define ERR_DRAM_GATING_RK1_F		(1 << 3)

#define ERR_CPU_RW_RK0			(1 << 0)
#define ERR_CPU_RW_RK1			(1 << 1)

/* 0x1f -> bit[4:0] is for DDR reserve mode */
#define DDR_RSV_MODE_ERR_MASK          (0x1f)

unsigned int check_last_dram_fatal_exception(void);
unsigned int check_dram_fatal_exception(void);
void set_err_code_for_storage_api(void);
void dram_fatal_set_ta2_err(unsigned int chn, unsigned int err_code);
void dram_fatal_set_gating_err(unsigned int chn, unsigned int err_code);
void dram_fatal_set_cpu_rw_err(unsigned int err_code);
void dram_fatal_set_stberr(unsigned int chn, unsigned int rk, unsigned int err_code);

void dram_fatal_backup_stberr(void);
void dram_fatal_init_stberr(void);

#define dram_fatal_set_ddr_rsv_mode_err()\
	do {\
		dram_fatal_set_err(0x1, 0x1, OFFSET_DDR_RSV_MODE_ERR);\
	} while(0)

#define dram_fatal_set_ddr_rsv_mode_flow()\
	do {\
		dram_fatal_set_err(0x1, 0x1, OFFSET_DDR_RSV_MODE_FLOW);\
	} while(0)

#define dram_fatal_set_kibo_plus_only()\
	do {\
		dram_fatal_set_err(0x1, 0x1, OFFSET_KIBO_PLUS_ONLY);\
	} while(0)

#endif //LAST_DRAMC

//================================================
//=============pmic related api for ETT HQA test ==============
//================================================
#define MAX_VDRAM2 1190625 //Vddq
#define MAX_VDRAM1 1309375 //Vdram
#define MAX_VCORE  1196875

//#define DRAM_HQA
#ifdef DRAM_HQA

//#define HQA_LPDDR3
//#define HQA_LPDDR4
#define HQA_LPDDR4X

//#define HVCORE_HVDRAM
#define NVCORE_NVDRAM
//#define LVCORE_LVDRAM
//#define HVCORE_LVDRAM
//#define LVCORE_HVDRAM

//===========Vcore=====================================
/*OFFSET: MT6351_BUCK_VCORE_CON4(0x0608) [6:0]
1. set to SW control ->0x0600  bit1 set to 0(default value=0 , initial setting=1) (0:SW control ,1:HW control)
2. set output voltage by OFFSET :0x0608  bit0~bit6(Voltage = 0.6 + 0.00625*code) (default:7'b1000000)
*/
#if defined(PMIC_CHIP_MT6355)
#define HQA_VCORE_LV_LPM 656000	// 0.656V
#define HQA_VCORE_NV_LPM 700000	// 0.700V
#define HQA_VCORE_HV_LPM 744000	// 0.744V

#define HQA_VCORE_LV_HPM 756000 // 0.756V
#define HQA_VCORE_NV_HPM 800000 // 0.800V
#define HQA_VCORE_HV_HPM 844000 // 0.844V

#define HQA_VCORE_KLV_ULPM 656000 // 0.656V
#define HQA_VCORE_KNV_ULPM 700000 // 0.700V
#define HQA_VCORE_KHV_ULPM 744000 // 0.744V

#define HQA_VCORE_KLV_LPM 706000 // 0.706V
#define HQA_VCORE_KNV_LPM 750000 // 0.750V
#define HQA_VCORE_KHV_LPM 794000 // 0.794V

#define HQA_VCORE_KLV_HPM 756000 // 0.756V
#define HQA_VCORE_KNV_HPM 800000 // 0.800V
#define HQA_VCORE_KHV_HPM 844000 // 0.844V
#else
#define HQA_VCORE_LV_LPM 0x09 // 0.6+0.00625*09=0.656
#define HQA_VCORE_NV_LPM 0x10 // 0.6+0.00625*16=0.700
#define HQA_VCORE_HV_LPM 0x17 // 0.6+0.00625*23=0.744

#define HQA_VCORE_LV_HPM 0x19 // 0.6+0.00625*25=0.756
#define HQA_VCORE_NV_HPM 0x20 // 0.6+0.00625*32=0.800
#define HQA_VCORE_HV_HPM 0x27 // 0.6+0.00625*39=0.844

#define HQA_VCORE_KLV_ULPM 0x09 // 0.6+0.00625*09=0.656
#define HQA_VCORE_KNV_ULPM 0x10 // 0.6+0.00625*16=0.700
#define HQA_VCORE_KHV_ULPM 0x17 // 0.6+0.00625*23=0.744

#define HQA_VCORE_KLV_LPM 0x11 // 0.6+0.00625*17=0.706
#define HQA_VCORE_KNV_LPM 0x18 // 0.6+0.00625*24=0.750
#define HQA_VCORE_KHV_LPM 0x1F // 0.6+0.00625*31=0.794

#define HQA_VCORE_KLV_HPM 0x19 // 0.6+0.00625*25=0.756
#define HQA_VCORE_KNV_HPM 0x20 // 0.6+0.00625*32=0.800
#define HQA_VCORE_KHV_HPM 0x27 // 0.6+0.00625*39=0.844
#endif

#if defined(HQA_LPDDR3)
#if defined(PMIC_CHIP_MT6355)
#define VDRAM_LV 1160000 // 1.16V
#define VDRAM_NV 1220000 // 1.22V
#define VDRAM_HV 1300000 // 1.30V
#else
#define VDRAM_LV 0x504 // 1.20-0.04=1.16
#define VDRAM_NV 0x51E // 1.20+0.02=1.22
#define VDRAM_HV 0x617 // 1.21+0.9=1.30
#endif
#elif defined(HQA_LPDDR4) || defined(HQA_LPDDR4X)
#define VDRAM_LV 1060000 // 1.06V
#define VDRAM_NV 1100000 // 1.10V
#define VDRAM_HV 1170000 // 1.17V
#endif

#define VDDQ_LV 570000
#define VDDQ_NV 600000
#define VDDQ_HV 650000

#define REDUCE_3_STEPS 18750 //Kibo Plus Only

#if defined(HVCORE_HVDRAM)
#define HQA_VCORE_LPM	HQA_VCORE_HV_LPM
#define HQA_VCORE_HPM	HQA_VCORE_HV_HPM
#define HQA_VCORE_KULPM	HQA_VCORE_KHV_ULPM
#define HQA_VCORE_KLPM	HQA_VCORE_KHV_LPM
#define HQA_VCORE_KHPM	HQA_VCORE_KHV_HPM
#define HQA_VDRAM	VDRAM_HV
#define HQA_VDDQ	VDDQ_HV
#elif defined(NVCORE_NVDRAM)
#define HQA_VCORE_LPM	HQA_VCORE_NV_LPM
#define HQA_VCORE_HPM	HQA_VCORE_NV_HPM
#define HQA_VCORE_KULPM	HQA_VCORE_KNV_ULPM
#define HQA_VCORE_KLPM	HQA_VCORE_KNV_LPM
#define HQA_VCORE_KHPM	HQA_VCORE_KNV_HPM
#define HQA_VDRAM	VDRAM_NV
#define HQA_VDDQ	VDDQ_NV
#elif defined(LVCORE_LVDRAM)
#define HQA_VCORE_LPM	HQA_VCORE_LV_LPM
#define HQA_VCORE_HPM	HQA_VCORE_LV_HPM
#define HQA_VCORE_KULPM	HQA_VCORE_KLV_ULPM
#define HQA_VCORE_KLPM	HQA_VCORE_KLV_LPM
#define HQA_VCORE_KHPM	HQA_VCORE_KLV_HPM
#define HQA_VDRAM	VDRAM_LV
#define HQA_VDDQ	VDDQ_LV
#elif defined(HVCORE_LVDRAM)
#define HQA_VCORE_LPM	HQA_VCORE_HV_LPM
#define HQA_VCORE_HPM	HQA_VCORE_HV_HPM
#define HQA_VCORE_KULPM	HQA_VCORE_KHV_ULPM
#define HQA_VCORE_KLPM	HQA_VCORE_KHV_LPM
#define HQA_VCORE_KHPM	HQA_VCORE_KHV_HPM
#define HQA_VDRAM	VDRAM_LV
#define HQA_VDDQ	VDDQ_LV
#elif defined(LVCORE_HVDRAM)
#define HQA_VCORE_LPM	HQA_VCORE_LV_LPM
#define HQA_VCORE_HPM	HQA_VCORE_LV_HPM
#define HQA_VCORE_KULPM	HQA_VCORE_KLV_ULPM
#define HQA_VCORE_KLPM	HQA_VCORE_KLV_LPM
#define HQA_VCORE_KHPM	HQA_VCORE_KLV_HPM
#define HQA_VDRAM	VDRAM_HV
#define HQA_VDDQ	VDDQ_HV
#endif

#if defined(HQA_LPDDR4)
#include <fan53526.h>
#elif defined(HQA_LPDDR4X)
#include <fan53526.h>
#include <fan53528buc08x.h>
#endif
#endif //end #ifdef DRAM_HQA

#ifdef DRAM_CALIB_LOG
#include <mt_rtc_hw.h>
#include <rtc.h>
#include <pmic_wrap_init.h>
#define CALIB_LOG_BASE 0x0011E000
#define CALIB_LOG_SIZE 8192
#define CALIB_LOG_MAGIC 0x19870611
void dram_klog_clean(void);
void dram_klog_init(void);
int i4WriteSramLog(unsigned int u4Offset, unsigned int *pu4Src, unsigned int u4WordCnt);
typedef struct {
    unsigned int guard;
    unsigned int rtc_dom_hou;
    unsigned int rtc_min_sec;
    unsigned int data_count;
    unsigned int rtc_yea_mth;
} DRAM_KLOG_HEAD;
typedef struct {
    unsigned int dram_type;
    unsigned int check;
    unsigned int guard;
} DRAM_KLOG_TAIL;
#endif //end #ifdef DRAM_CALIB_LOG

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION

#define PART_DRAM_DATA_ADDR	0x8000
#define PART_DRAM_DATA_SIZE	0x100000

#if (CFG_BOOT_DEV == BOOTDEV_SDMMC)
#define	PART_ID_DRAM_DATA	EMMC_PART_USER
#endif

#define	DRAM_CALIBRATION_DATA_MAGIC	0x9502

typedef struct _DRAM_CALIBRATION_DATA_T
{
	u32	pl_version;
	u16	magic_number;
	u16	checksum;
	u32	calib_err_code;
	SAVE_TIME_FOR_CALIBRATION_T	calibration_data;
} DRAM_CALIBRATION_DATA_T;

/*
 * g_dram_storage_api_err_code:
 * 	bit[0:3] -> read api
 * 	bit[4:7] -> write api
 * 	bit[8:11] -> clean api
 * 	bit[12:12] -> data formatted due to fatal exception
 */
#define ERR_NULL_POINTER	(0x1)
#define ERR_MAGIC_NUMBER	(0x2)
#define ERR_CHECKSUM		(0x3)
#define ERR_PL_UPDATED		(0x4)
#define ERR_BLKDEV_NOT_FOUND	(0x5)
#define ERR_BLKDEV_READ_FAIL	(0x6)
#define ERR_BLKDEV_WRITE_FAIL	(0x7)

#define ERR_DATA_FORMATTED_OFFSET	(12)

typedef enum {
	DRAM_STORAGE_API_READ = 0,
	DRAM_STORAGE_API_WRITE,
	DRAM_STORAGE_API_CLEAN,
} DRAM_STORAGE_API_TPYE;

extern u32 g_dram_storage_api_err_code;
#define SET_DRAM_STORAGE_API_ERR(err_type, api_type) \
do {\
	g_dram_storage_api_err_code |= (err_type << (api_type * 4));\
} while(0)

#define SET_DATA_FORMATTED_STORAGE_API_ERR() \
do {\
	g_dram_storage_api_err_code |= (1 << ERR_DATA_FORMATTED_OFFSET);\
} while(0)

int read_offline_dram_calibration_data(SAVE_TIME_FOR_CALIBRATION_T *offLine_SaveData);
int write_offline_dram_calibration_data(SAVE_TIME_FOR_CALIBRATION_T *offLine_SaveData);
int clean_dram_calibration_data(void);

void dram_fatal_exception_detection_start(void);
void dram_fatal_exception_detection_end(void);

#define CBT_VREF_OFFSET			2
#define WRITE_LEVELING_OFFSET		5
#define GATING_START_OFFSET		0
#define GATING_PASS_WIN_OFFSET		3
#define RX_WIN_PERBIT_OFFSET		5
#define RX_WIN_PERBIT_VREF_OFFSET	4
#define TX_WIN_PERBIT_OFFSET		5
#define TX_WIN_PERBIT_VREF_OFFSET	4
#define RX_DATLAT_OFFSET		1
#define RX_WIN_HIGH_SPEED_TH		10
#define RX_WIN_LOW_SPEED_TH		100
#define TX_WIN_TH			12

#endif

#endif
