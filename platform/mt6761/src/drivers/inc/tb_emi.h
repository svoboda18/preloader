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
#include "tb_dramc_common.h"
#if !__ETT__ && (FOR_DV_SIMULATION_USED == 0)
#include "boot_device.h"
#endif

#if !__ETT__ 
#if (FOR_DV_SIMULATION_USED==0)
#define DDR_RESERVE_MODE
#define COMBO_MCP
#define DRAM_BASE 0x40000000ULL
#define DDR_BASE DRAM_BASE
//#define DRAM_CALIB_LOG
#define LAST_DRAMC
#define LAST_EMI
#endif
#endif

#if __ETT__
#define dramc_crit		printf
#define dramc_debug		printf
#elif __FLASH_TOOL_DA__
#define dramc_crit		LOGD
#define dramc_debug		LOGD
#else
#define dramc_crit		print
#define dramc_debug		printf
#endif

#if (FOR_DV_SIMULATION_USED==1 || SW_CHANGE_FOR_SIMULATION==1)
typedef unsigned long long u64;
#endif

#define MAX_CH	2
#define MAX_RK	2

int get_dram_channel_nr(void);
int get_dram_rank_nr(void);
void get_dram_rank_size(u64 dram_rank_size[]);
void reserve_dramc_dummy_read(void);

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
    unsigned int  sub_version;            // sub_version: 0x1 for new version
    unsigned int  type;                /* 0x0000 : Invalid
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
    unsigned int   id_length;              // EMMC and NAND ID checking length
    unsigned int   fw_id_length;              // FW ID checking length
    unsigned char  ID[16];
    unsigned char  fw_id[8];               // To save fw id
    unsigned int   EMI_CONA_VAL;           //@0x3000
    unsigned int   EMI_CONH_VAL;
    
    union   {        
        unsigned int DRAMC_ACTIME_UNION[8];
    AC_TIMING_EXTERNAL_T AcTimeEMI;
    };       

    u64   DRAM_RANK_SIZE[4];
    unsigned int   EMI_CONF_VAL;
    unsigned int   CHN0_EMI_CONA_VAL;
    unsigned int   CHN1_EMI_CONA_VAL;
    /* Single field to store LP4 dram type (normal, byte, mixed) */
    unsigned int   dram_cbt_mode_extern;
    unsigned int   reserved[6];

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
    unsigned int   iLPDDR3_MODE_REG_5;
#endif    
    unsigned int   PIN_MUX_TYPE;   
} EMI_SETTINGS;

//typedef  EMI_SETTINGS_v15 EMI_SETTINGS;
#if (FOR_DV_SIMULATION_USED==0)
void setup_dramc_voltage_by_pmic(void);
void switch_dramc_voltage_to_auto_mode(void);
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

#include "x_hal_io.h"

void init_ta2_single_channel(unsigned int);
#ifdef LAST_DRAMC
#define LAST_DRAMC_MAGIC_PATTERN 0x19870611
static void update_last_dramc_info(void);
void init_ta2_all_channel(void);
typedef struct {
    unsigned int ta2_result_magic;
    unsigned int ta2_result_last;
    unsigned int ta2_result_past;
    unsigned int ta2_result_checksum;
    unsigned int reboot_count;
    volatile unsigned int last_fatal_err_flag;
    volatile unsigned int fatal_err_flag;
    volatile unsigned int storage_api_err_flag;
    volatile unsigned int last_gating_err[2][2]; // [channel][rank]
    volatile unsigned int gating_err[2][2]; // [channel][rank]
} LAST_DRAMC_INFO_T;
#define DEF_LAST_DRAMC LAST_DRAMC_INFO_T

#define OFFSET_DRAM_FATAL_ERR		(31)
#define OFFSET_DRAM_TA2_ERR		(23)
#define OFFSET_DRAM_GATING_ERR		(7)
#define OFFSET_CPU_RW_ERR		(5)
#define OFFSET_DDR_RSV_MODE_FLOW	(4)
#define OFFSET_DDR_RSV_MODE_ERR		(3)
#define OFFSET_EMI_DCS_ERR		(2)
#define OFFSET_DVFSRC_ERR		(1)
#define OFFSET_DRS_ERR			(0)

#define ERR_DRAM_TA2_RK0		(1 << 0)
#define ERR_DRAM_TA2_RK1		(1 << 1)

#define ERR_DRAM_GATING_RK0_R		(1 << 0)
#define ERR_DRAM_GATING_RK0_F		(1 << 1)
#define ERR_DRAM_GATING_RK1_R		(1 << 2)
#define ERR_DRAM_GATING_RK1_F		(1 << 3)

#define ERR_CPU_RW_RK0			(1 << 0)
#define ERR_CPU_RW_RK1			(1 << 1)

/* 0x1f -> bit[4:0] is for DDR reserve mode */
#define DDR_RSV_MODE_ERR_MASK		(0x1f)

unsigned int check_last_dram_fatal_exception(void);
unsigned int check_dram_fatal_exception(void);
void set_err_code_for_storage_api(void);
void dram_fatal_set_ta2_err(unsigned int chn, unsigned int err_code);
void dram_fatal_set_gating_err(unsigned int chn, unsigned int err_code);
void dram_fatal_set_cpu_rw_err(unsigned int err_code);
void dram_fatal_set_stberr(unsigned int chn, unsigned int rk, unsigned int err_code);

void dram_fatal_backup_stberr(void);
void dram_fatal_init_stberr(void);
void dram_fatal_set_err(unsigned int err_code, unsigned int mask, unsigned int offset);

#define dram_fatal_set_cpu_rw_err(err_code)\
	do {\
		dram_fatal_set_err(err_code, 0x3, OFFSET_CPU_RW_ERR);\
	} while(0)

#define dram_fatal_set_ddr_rsv_mode_err()\
	do {\
		dram_fatal_set_err(0x1, 0x1, OFFSET_DDR_RSV_MODE_ERR);\
	} while(0)

#define dram_fatal_set_emi_dcs_err()\
	do {\
		dram_fatal_set_err(0x1, 0x1, OFFSET_EMI_DCS_ERR);\
	} while(0)

#define dram_fatal_set_dvfsrc_err()\
	do {\
		dram_fatal_set_err(0x1, 0x1, OFFSET_DVFSRC_ERR);\
	} while(0)

#define dram_fatal_set_drs_err()\
	do {\
		dram_fatal_set_err(0x1, 0x1, OFFSET_DRS_ERR);\
	} while(0)

#define dram_fatal_set_ddr_rsv_mode_flow()\
	do {\
		dram_fatal_set_err(0x1, 0x1, OFFSET_DDR_RSV_MODE_FLOW);\
	} while(0)

#endif //LAST_DRAMC

//================================================
//=============pmic related api for ETT HQA test ==============
//================================================
#if __ETT__
#define DRAM_HQA
#endif

#define MAX_VCORE	1196875
#define MAX_VDRAM	1309375
#define MAX_VDDQ	1190625

#define UNIT_VIO18  10000
#define UNIT_VCORE	6250
#define UNIT_VDRAM	10000
#define UNIT_VDDQ	10000

#define HQA_VIO18_HV			1950000
#define HQA_VCORE_HV_LP4_KOPP0	843750
#define HQA_VCORE_HV_LP4_KOPP1	737500
#define HQA_VCORE_HV_LP4_KOPP2	737500
#define HQA_VCORE_HV_LP4_KOPP3	737500
#define HQA_VCORE_HV_LP4_KOPP4	682500
#define HQA_VDRAM_HV_LP4		1170000
#define HQA_VDDQ_HV_LP4			650000

#define HQA_VIO18_NV			1800000
#define HQA_VCORE_NV_LP4_KOPP0	800000
#define HQA_VCORE_NV_LP4_KOPP1	700000
#define HQA_VCORE_NV_LP4_KOPP2	700000
#define HQA_VCORE_NV_LP4_KOPP3	700000
#define HQA_VCORE_NV_LP4_KOPP4	650000
#define HQA_VDRAM_NV_LP4		1125000
#define HQA_VDDQ_NV_LP4			600000

#define HQA_VIO18_LV			1730000
#define HQA_VCORE_LV_LP4_KOPP0	756250
#define HQA_VCORE_LV_LP4_KOPP1	662500
#define HQA_VCORE_LV_LP4_KOPP2	662500
#define HQA_VCORE_LV_LP4_KOPP3	662500
#define HQA_VCORE_LV_LP4_KOPP4	612500
#define HQA_VDRAM_LV_LP4		1090000
#define HQA_VDDQ_LV_LP4			580000

#define HQA_VCORE_HV_LP3_KOPP0	843750
#define HQA_VCORE_HV_LP3_KOPP1	843750
#define HQA_VCORE_HV_LP3_KOPP2	737500
#define HQA_VCORE_HV_LP3_KOPP3	737500
#define HQA_VCORE_HV_LP3_KOPP4	682500
#define HQA_VDRAM_HV_LP3		1300000

#define HQA_VCORE_NV_LP3_KOPP0	800000
#define HQA_VCORE_NV_LP3_KOPP1	800000
#define HQA_VCORE_NV_LP3_KOPP2	700000
#define HQA_VCORE_NV_LP3_KOPP3	700000
#define HQA_VCORE_NV_LP3_KOPP4	650000
#define HQA_VDRAM_NV_LP3		1220000

#define HQA_VCORE_LV_LP3_KOPP0	756250
#define HQA_VCORE_LV_LP3_KOPP1	756250
#define HQA_VCORE_LV_LP3_KOPP2	662500
#define HQA_VCORE_LV_LP3_KOPP3	662500
#define HQA_VCORE_LV_LP3_KOPP4	612500
#define HQA_VDRAM_LV_LP3		1160000

#define  _SEL_PREFIX_OPP(type, vol, dtype, opp)   HQA_##type##_##vol##_##dtype##_##opp
#define  _SEL_PREFIX(type, vol, dtype)   HQA_##type##_##vol##_##dtype
#define  _SEL_VIO18(vol)   HQA_VIO18_##vol

#define STD_VIO18				_SEL_VIO18(NV)
#define STD_VCORE(dtype, opp)	_SEL_PREFIX_OPP(VCORE, NV, dtype, opp)
#define STD_VDRAM(dtype)		_SEL_PREFIX(VDRAM, NV, dtype)
#define STD_VDDQ				_SEL_PREFIX(VDDQ, NV, LP4)

#ifdef DRAM_HQA

//#define HVCORE_HVDRAM
#define NVCORE_NVDRAM
//#define LVCORE_LVDRAM
//#define HVCORE_LVDRAM
//#define LVCORE_HVDRAM

#if defined(HVCORE_HVDRAM)
	#define HQA_VCORE(dtype, opp)	_SEL_PREFIX_OPP(VCORE, HV, dtype, opp)
	#define HQA_VDRAM(dtype)		_SEL_PREFIX(VDRAM, HV, dtype)
	#define HQA_VDDQ				_SEL_PREFIX(VDDQ, HV, LP4)
#elif defined(NVCORE_NVDRAM)
	#define HQA_VCORE(dtype, opp)	_SEL_PREFIX_OPP(VCORE, NV, dtype, opp)
	#define HQA_VDRAM(dtype)		_SEL_PREFIX(VDRAM, NV, dtype)
	#define HQA_VDDQ				_SEL_PREFIX(VDDQ, NV, LP4)
#elif defined(LVCORE_LVDRAM)
	#define HQA_VCORE(dtype, opp)	_SEL_PREFIX_OPP(VCORE, LV, dtype, opp)
	#define HQA_VDRAM(dtype)		_SEL_PREFIX(VDRAM, LV, dtype)
	#define HQA_VDDQ				_SEL_PREFIX(VDDQ, LV, LP4)
#elif defined(HVCORE_LVDRAM)
	#define HQA_VCORE(dtype, opp)	_SEL_PREFIX_OPP(VCORE, HV, dtype, opp)
	#define HQA_VDRAM(dtype)		_SEL_PREFIX(VDRAM, LV, dtype)
	#define HQA_VDDQ				_SEL_PREFIX(VDDQ, LV, LP4)
#elif defined(LVCORE_HVDRAM)
	#define HQA_VCORE(dtype, opp)	_SEL_PREFIX_OPP(VCORE, LV, dtype, opp)
	#define HQA_VDRAM(dtype)		_SEL_PREFIX(VDRAM, HV, dtype)
	#define HQA_VDDQ				_SEL_PREFIX(VDDQ, HV, LP4)
#else
	#error "Please set HQA voltage type"
#endif

#define SEL_PREFIX_VCORE(dtype, opp)	HQA_VCORE(dtype, opp)
#define SEL_PREFIX_VDRAM(dtype)				HQA_VDRAM(dtype)
#define SEL_PREFIX_VDDQ					HQA_VDDQ
#else
//#if !__ETT__
//#define VCORE_BIN
//#endif
#define SEL_PREFIX_VCORE(dtype, opp)	STD_VCORE(dtype, opp)
#define SEL_PREFIX_VDRAM(dtype)				STD_VDRAM(dtype)
#define SEL_PREFIX_VDDQ					STD_VDDQ
#endif

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION

#define PART_DRAM_DATA_SIZE	0x100000

#if !__ETT__
#if !defined(BOOTDEV_UFS) && !defined(BOOTDEV_SDMMC)
#error "BOOTDEV_UFS & BOOTDEV_SDMMC not defined"
#endif
#endif

#if (CFG_BOOT_DEV == BOOTDEV_UFS)
#define	PART_ID_DRAM_DATA	UFS_LU_USER
#elif (CFG_BOOT_DEV == BOOTDEV_SDMMC)
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
#define ERR_BLKDEV_NO_PART	(0x8)

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

unsigned long long get_dram_size(void);

typedef struct {
	unsigned long long full_sys_addr;
	unsigned int addr;
	unsigned int row;
	unsigned int col;
	unsigned char ch;
	unsigned char rk;
	unsigned char bk;
	unsigned char dummy;
} dram_addr_t;

unsigned int get_dummy_read_addr(dram_addr_t *dram_addr);

#ifdef LAST_EMI
#define LAST_EMI_MAGIC_PATTERN 0x19870611
typedef struct {
	unsigned int decs_magic;
	unsigned int decs_ctrl;
	unsigned int decs_dram_type;
	unsigned int decs_diff_us;
	unsigned int mbw_buf_l;
	unsigned int mbw_buf_h;
} LAST_EMI_INFO_T;
#define DEF_LAST_EMI LAST_EMI_INFO_T

void reserve_emi_mbw_buf(void);
#endif

#endif
