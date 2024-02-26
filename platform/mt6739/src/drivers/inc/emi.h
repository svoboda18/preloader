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

#ifndef MT_EMI_H
#define MT_EMI_H

#define DDR_RESERVE_MODE

#define MAX_RK	2

/* Enable to support Combo Discrete LP2/3 */
//#define DRAM_TYPE_DECIDED_BY_GPIO

#ifdef DRAM_TYPE_DECIDED_BY_GPIO
#define DRAM_TYPE_GPIO_NUM 10       // judge by which GPIO
#define GPIO_VALUE_FOR_LPDDR2	1   // define GPIO value and corresponding DRAM type
#define GPIO_VALUE_FOR_LPDDR3   0
#endif

#define LAST_DRAMC
//#define DRAM_CALIB_LOG
typedef volatile unsigned int *V_UINT32P;
extern void mt6516_set_emi (void);
extern void mt6516_256M_mem_setting (void);
int get_dram_channel_nr(void);
int get_dram_rank_nr (void);
void get_dram_rank_size (u64 dram_rank_size[]);
void emi_reserve_4k_workaround(void);
enum reserve_rank {
        RANK0,  /* first rank */
        RANKMAX,        /* as high rank as possible */
};
#define mcp_dram 0x0200;
#define DDR1  1
#define LPDDR2  2
#define LPDDR3  3
#define PCDDR3  4
typedef struct
{
    int   sub_version;            // sub_version: 0x1 for new version
    int  type;                /* 0x0000 : Invalid
                                 0x0001 : Discrete DDR1
                                 0x0002 : Discrete LPDDR2
                                 0x0003 : Discrete LPDDR3
                                 0x0004 : Discrete PCDDR3
                                 0x0101 : MCP(NAND+DDR1)
                                 0x0102 : MCP(NAND+LPDDR2)
                                 0x0103 : MCP(NAND+LPDDR3)
                                 0x0104 : MCP(NAND+PCDDR3)
                                 0x0201 : MCP(eMMC+DDR1)
                                 0x0202 : MCP(eMMC+LPDDR2)
                                 0x0203 : MCP(eMMC+LPDDR3)
                                 0x0204 : MCP(eMMC+PCDDR3)
                              */
    int   id_length;              // EMMC and NAND ID checking length
    int   fw_id_length;              // FW ID checking length
    char  ID[16];
    char  fw_id[8];               // To save fw id
    int   EMI_CONA_VAL;           //@0x3000
    int   DRAMC_DRVCTL0_VAL;      //@0x40B8               -> customized TX I/O driving
    int   DRAMC_DRVCTL1_VAL;      //@0x40BC               -> customized TX I/O driving
    int   DRAMC_ACTIM_VAL;        //@0x4000
    int   DRAMC_GDDR3CTL1_VAL;    //@0x40F4
    int   DRAMC_CONF1_VAL;        //@0x4004
    int   DRAMC_DDR2CTL_VAL;      //@0x407C 
    int   DRAMC_TEST2_3_VAL;      //@0x4044
    int   DRAMC_CONF2_VAL;        //@0x4008
    int   DRAMC_PD_CTRL_VAL;      //@0x41DC
    int   DRAMC_PADCTL3_VAL;      //@0x4014               -> customized TX DQS delay
    int   DRAMC_DQODLY_VAL;       //@0x4200~0x420C        -> customized TX DQ delay
    int   DRAMC_ADDR_OUTPUT_DLY;  // for E1 DDR2 only
    int   DRAMC_CLK_OUTPUT_DLY;   // for E1 DDR2 only
    int   DRAMC_ACTIM1_VAL;       //@0x41E8
    int   DRAMC_MISCTL0_VAL;      //@0x40FC
    int   DRAMC_ACTIM05T_VAL;     //@0x41F8
    u32   DRAM_RANK_SIZE[4];
    int   reserved[10];

    union
    {
        struct
        {
            int   LPDDR2_MODE_REG_1;
            int   LPDDR2_MODE_REG_2;
            int   LPDDR2_MODE_REG_3;
            int   LPDDR2_MODE_REG_5;
            int   LPDDR2_MODE_REG_10;
            int   LPDDR2_MODE_REG_63;
        };
        struct
        {
            int   DDR1_MODE_REG;
            int   DDR1_EXT_MODE_REG;
        };
        struct
        {
            int   PCDDR3_MODE_REG0;
            int   PCDDR3_MODE_REG1;
            int   PCDDR3_MODE_REG2;
            int   PCDDR3_MODE_REG3;
            int   PCDDR3_MODE_REG4;
            int   PCDDR3_MODE_REG5;			
        };
        struct
        {
            int   LPDDR3_MODE_REG_1;
            int   LPDDR3_MODE_REG_2;
            int   LPDDR3_MODE_REG_3;
            int   LPDDR3_MODE_REG_5;
            int   LPDDR3_MODE_REG_10;
            int   LPDDR3_MODE_REG_63;
        };
    };
} EMI_SETTINGS;

int mt_get_dram_type (void); 
/* 0: invalid */
/* 1: mDDR1 */
/* 2: LPDDR2 */
/* 3: LPDDR3 */
/* 4: PCDDR3 */

#ifdef LAST_DRAMC
#include "x_hal_io.h"
#define DRAM_DEBUG_SRAM_BASE 0x0010E400
#define LAST_DRAMC_SRAM_SIZE 1024
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
#endif //LAST_DRAMC
#ifdef DRAM_CALIB_LOG
#include <mt_rtc_hw.h>
#include <rtc.h>
#include <pmic_wrap_init.h>
#define CALIB_LOG_BASE 0x0010EA00
#define CALIB_LOG_SIZE 4608
int i4WriteSramLog(unsigned int u4Offset, unsigned int *pu4Src, unsigned int u4WordCnt);
typedef struct {
    unsigned int rtc_yea_mth;
    unsigned int rtc_dom_hou;
    unsigned int rtc_min_sec;
    unsigned int data_count;
} DRAM_KLOG_HEAD;
typedef struct {
    unsigned int check;
    unsigned int guard;
} DRAM_KLOG_TAIL;
#endif //end #ifdef DRAM_CALIB_LOG

#define LP2_VCORE 1050000

//#define DRAM_HQA
#ifdef DRAM_HQA
//#define HV_HQA
//#define NV_HQA
#define LV_HQA
//#define HV_VDRAM_HQA
//#define LV_VDRAM_HQA
//#define k_LP3_1066
#else
/* set to NV by default */
#define NV_HQA
#endif

#ifndef k_LP3_1066
//1333 voltage table
#ifdef HV_HQA
#define HQA_VCORE 1231250
#define HQA_VDRAM 1300000
#endif
#ifdef NV_HQA
#define HQA_VCORE 1150000 
#define HQA_VDRAM 1220000
#endif
#ifdef LV_HQA
#define HQA_VCORE 1068750
#define HQA_VDRAM 1160000
#endif
#else
//1066 voltage table
#ifdef HV_HQA
#define HQA_VCORE 1125000
#define HQA_VDRAM 1300000
#endif
#ifdef NV_HQA
#define HQA_VCORE 1050000 
#define HQA_VDRAM 1220000
#endif
#ifdef LV_HQA
#define HQA_VCORE 975000
#define HQA_VDRAM 1160000
#endif
#endif

/* optional, set VDRAM to HV/LV */
#ifdef HV_VDRAM_HQA
#undef HQA_VDRAM
#define HQA_VDRAM 1300000
#endif

#ifdef LV_VDRAM_HQA
#undef HQA_VDRAM
#define HQA_VDRAM 1160000
#endif
#endif

#define VDRAM_VAL HQA_VDRAM
