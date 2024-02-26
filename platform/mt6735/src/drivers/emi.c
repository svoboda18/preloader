/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

#include <typedefs.h>
#include <platform.h>
#include <emi.h>
#include <platform.h>
#include <dramc.h>
#include <pll.h>
#include "wdt.h"
#include <emi_hw.h>
#include <stdlib.h>
#include <upmu_hw.h>

#ifdef COMBO_MCP
#include "custom_emi.h"
#else
static int num_of_emi_records;
static EMI_SETTINGS emi_settings[];
#endif

#ifdef DERATING_ENABLE
void derating_enable(void);
#endif


#ifdef REXTDN_ENABLE
void ett_rextdn_sw_calibration(void);
unsigned int drvp, drvn;
#endif

#ifdef DRAMC_DEBUG
void print_DQS_DQ_tx_delay(void);
#endif

//---------------------------------------------
#ifdef REPAIR_SRAM
extern int repair_sram(void);
#endif

#ifdef ENABLE_DFS
extern char *opt_gw_coarse_value0, *opt_gw_fine_value0;
extern char *opt_gw_coarse_value1, *opt_gw_fine_value1;
#endif

/* select the corresponding memory devices */
extern int dramc_calib(EMI_SETTINGS *emi_setting);

extern u32 g_ddr_reserve_enable;
extern u32 g_ddr_reserve_success;

//---------------------------------------------
static int enable_combo_dis = 0;
static int DDR_type;
static char id[22];
static int emmc_nand_id_len = 16;
static int fw_id_len;


#ifdef HARDCODE_DRAM_SETTING
#if defined(MACH_TYPE_MT6735)
#define NUM_EMI_RECORD_HARD_CODE (3)
#elif defined(MACH_TYPE_MT6735M)
#define NUM_EMI_RECORD_HARD_CODE (1)
#elif defined(MACH_TYPE_MT6753)
#define NUM_EMI_RECORD_HARD_CODE (2)
#endif

#define emi_settings emi_settings_hard_code
#if defined(MACH_TYPE_MT6735)
EMI_SETTINGS emi_settings_hard_code[] =
{
	//H9TQ64A8GTMCUR_KUM
	{
		0x0,		/* sub_version */
		0x0203,		/* TYPE */
		9,		/* EMMC ID/FW ID checking length */
		0,		/* FW length */
		{0x90,0x01,0x4A,0x48,0x38,0x47,0x31,0x65,0x05,0x0,0x0,0x0,0x0,0x0,0x0,0x0},		/* NAND_EMMC_ID */
		{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},		/* FW_ID */
		0x00002012,		/* EMI_CONA_VAL */
		0xAA00AA00,		/* DRAMC_DRVCTL0_VAL */
		0xAA00AA00,		/* DRAMC_DRVCTL1_VAL */
		0x559A45D6,		/* DRAMC_ACTIM_VAL */
		0x01000000,		/* DRAMC_GDDR3CTL1_VAL */
		0xF0048683,		/* DRAMC_CONF1_VAL */
		0xC00642D1,		/* DRAMC_DDR2CTL_VAL */
		0xBF0B0401,		/* DRAMC_TEST2_3_VAL */
		0x0180AE50,		/* DRAMC_CONF2_VAL */
		0xD1644742,		/* DRAMC_PD_CTRL_VAL */
		0x00008888,		/* DRAMC_PADCTL3_VAL */
		0x88888888,		/* DRAMC_DQODLY_VAL */
		0x00000000,		/* DRAMC_ADDR_OUTPUT_DLY */
		0x00000000,		/* DRAMC_CLK_OUTPUT_DLY */
		0x11001330,		/* DRAMC_ACTIM1_VAL*/
		0x17800000,		/* DRAMC_MISCTL0_VAL*/
		0x00001420,		/* DRAMC_ACTIM05T_VAL*/
		{0x40000000,0,0,0},		/* DRAM RANK SIZE */
		{0,0,0,0,0,0,0,0,0,0},		/* reserved 10 */
		0x00C30001,		/* LPDDR3_MODE_REG1 */
		0x00080002,		/* LPDDR3_MODE_REG2 */
		0x00020003,		/* LPDDR3_MODE_REG3 */
		0x00000006,		/* LPDDR3_MODE_REG5 */
		0x00FF000A,		/* LPDDR3_MODE_REG10 */
		0x0000003F,		/* LPDDR3_MODE_REG63 */
	} ,
	//KMQ8X000SA_B414
	{
		0x0,		/* sub_version */
		0x0203,		/* TYPE */
		9,		/* EMMC ID/FW ID checking length */
		0,		/* FW length */
		{0x15,0x01,0x00,0x51,0x38,0x58,0x53,0x41,0x42,0x0,0x0,0x0,0x0,0x0,0x0,0x0},		/* NAND_EMMC_ID */
		{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},		/* FW_ID */
		0x00025052,		/* EMI_CONA_VAL */
		0xAA00AA00,		/* DRAMC_DRVCTL0_VAL */
		0xAA00AA00,		/* DRAMC_DRVCTL1_VAL */
		0x559A45D6,		/* DRAMC_ACTIM_VAL */
		0x01000000,		/* DRAMC_GDDR3CTL1_VAL */
		0xF0048683,		/* DRAMC_CONF1_VAL */
		0xC00642D1,		/* DRAMC_DDR2CTL_VAL */
		0xBF000401,		/* DRAMC_TEST2_3_VAL */
		0x01806C50,		/* DRAMC_CONF2_VAL */
		0xD1642C42,		/* DRAMC_PD_CTRL_VAL */
		0x00008888,		/* DRAMC_PADCTL3_VAL */
		0x88888888,		/* DRAMC_DQODLY_VAL */
		0x00000000,		/* DRAMC_ADDR_OUTPUT_DLY */
		0x00000000,		/* DRAMC_CLK_OUTPUT_DLY */
		0x11000920,		/* DRAMC_ACTIM1_VAL*/
		0x17800000,		/* DRAMC_MISCTL0_VAL*/
		0x00001424,		/* DRAMC_ACTIM05T_VAL*/
		{0x20000000,0x20000000,0,0},		/* DRAM RANK SIZE */
		{0,0,0,0,0,0,0,0,0,0},		/* reserved 10 */
		0x00C30001,		/* LPDDR3_MODE_REG1 */
		0x00080002,		/* LPDDR3_MODE_REG2 */
		0x00020003,		/* LPDDR3_MODE_REG3 */
		0x00000001,		/* LPDDR3_MODE_REG5 */
		0x00FF000A,		/* LPDDR3_MODE_REG10 */
		0x0000003F,		/* LPDDR3_MODE_REG63 */
	} ,
	//KMQ7X000SA_B315 (Alpha Phone)
	{
		0x0,		/* sub_version */
		0x0203,		/* TYPE */
		9,		/* EMMC ID/FW ID checking length */
		0,		/* FW length */
		{0x15,0x01,0x00,0x51,0x37,0x58,0x53,0x41,0x42,0x0,0x0,0x0,0x0,0x0,0x0,0x0},		/* NAND_EMMC_ID */
		{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},		/* FW_ID */
		0x00025052,		/* EMI_CONA_VAL */
		0xAA00AA00,		/* DRAMC_DRVCTL0_VAL */
		0xAA00AA00,		/* DRAMC_DRVCTL1_VAL */
		0x559A45D6,		/* DRAMC_ACTIM_VAL */
		0x01000000,		/* DRAMC_GDDR3CTL1_VAL */
		0xF0048683,		/* DRAMC_CONF1_VAL */
		0xC00642D1,		/* DRAMC_DDR2CTL_VAL */
		0xBF000401,		/* DRAMC_TEST2_3_VAL */
		0x01806C50,		/* DRAMC_CONF2_VAL */
		0xD1642C42,		/* DRAMC_PD_CTRL_VAL */
		0x00008888,		/* DRAMC_PADCTL3_VAL */
		0x88888888,		/* DRAMC_DQODLY_VAL */
		0x00000000,		/* DRAMC_ADDR_OUTPUT_DLY */
		0x00000000,		/* DRAMC_CLK_OUTPUT_DLY */
		0x11000920,		/* DRAMC_ACTIM1_VAL*/
		0x17800000,		/* DRAMC_MISCTL0_VAL*/
		0x00001424,		/* DRAMC_ACTIM05T_VAL*/
		{0x20000000,0x20000000,0,0},		/* DRAM RANK SIZE */
		{0,0,0,0,0,0,0,0,0,0},		/* reserved 10 */
		0x00C30001,		/* LPDDR3_MODE_REG1 */
		0x00080002,		/* LPDDR3_MODE_REG2 */
		0x00020003,		/* LPDDR3_MODE_REG3 */
		0x00000001,		/* LPDDR3_MODE_REG5 */
		0x00FF000A,		/* LPDDR3_MODE_REG10 */
		0x0000003F,		/* LPDDR3_MODE_REG63 */
	} ,
};
#elif defined(MACH_TYPE_MT6735M)
EMI_SETTINGS emi_settings_hard_code[] =
{
	//H9TQ64A8GTMCUR_KUM
	{
		0x0,		/* sub_version */
		0x0203,		/* TYPE */
		9,		/* EMMC ID/FW ID checking length */
		0,		/* FW length */
		{0x90,0x01,0x4A,0x48,0x38,0x47,0x31,0x65,0x05,0x0,0x0,0x0,0x0,0x0,0x0,0x0},		/* NAND_EMMC_ID */
		{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},		/* FW_ID */
		0x00002012,		/* EMI_CONA_VAL */
		0xAA00AA00,		/* DRAMC_DRVCTL0_VAL */
		0xAA00AA00,		/* DRAMC_DRVCTL1_VAL */
		0x44584493,		/* DRAMC_ACTIM_VAL */
		0x01000000,		/* DRAMC_GDDR3CTL1_VAL */
		0xF0048683,		/* DRAMC_CONF1_VAL */
		0xA00631D1,		/* DRAMC_DDR2CTL_VAL */
		0xBF0D0401,		/* DRAMC_TEST2_3_VAL */
		0x0180AE3F,		/* DRAMC_CONF2_VAL */
		0xD1643842,		/* DRAMC_PD_CTRL_VAL */
		0x00008888,		/* DRAMC_PADCTL3_VAL */
		0x88888888,		/* DRAMC_DQODLY_VAL */
		0x00000000,		/* DRAMC_ADDR_OUTPUT_DLY */
		0x00000000,		/* DRAMC_CLK_OUTPUT_DLY */
		0x11000D20,		/* DRAMC_ACTIM1_VAL*/
		0x07800000,		/* DRAMC_MISCTL0_VAL*/
		0x04002600,		/* DRAMC_ACTIM05T_VAL*/
		{0x40000000,0,0,0},		/* DRAM RANK SIZE */
		{0,0,0,0,0,0,0,0,0,0},		/* reserved 10 */
		0x00C30001,		/* LPDDR3_MODE_REG1 */
		0x00060002,		/* LPDDR3_MODE_REG2 */
		0x00020003,		/* LPDDR3_MODE_REG3 */
		0x00000006,		/* LPDDR3_MODE_REG5 */
		0x00FF000A,		/* LPDDR3_MODE_REG10 */
		0x0000003F,		/* LPDDR3_MODE_REG63 */
	} ,
};
#elif defined(MACH_TYPE_MT6753)
EMI_SETTINGS emi_settings_hard_code[] =
{
	//H9TQ64A8GTMCUR_KUM
	{
		0x0,		/* sub_version */
		0x0203,		/* TYPE */
		9,		/* EMMC ID/FW ID checking length */
		0,		/* FW length */
		{0x90,0x01,0x4A,0x48,0x38,0x47,0x31,0x65,0x05,0x0,0x0,0x0,0x0,0x0,0x0,0x0},		/* NAND_EMMC_ID */
		{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},		/* FW_ID */
		0x00002012,		/* EMI_CONA_VAL */
		0xAA00AA00,		/* DRAMC_DRVCTL0_VAL */
		0xAA00AA00,		/* DRAMC_DRVCTL1_VAL */
		0x66AA46F7,		/* DRAMC_ACTIM_VAL */
		0x01000000,		/* DRAMC_GDDR3CTL1_VAL */
		0xF00486C3,		/* DRAMC_CONF1_VAL */
		0xC00652D1,		/* DRAMC_DDR2CTL_VAL */
		0xBF020401,		/* DRAMC_TEST2_3_VAL */
		0x0180AE58,		/* DRAMC_CONF2_VAL */
		0xD1644E42,		/* DRAMC_PD_CTRL_VAL */
		0x00008888,		/* DRAMC_PADCTL3_VAL */
		0x88888888,		/* DRAMC_DQODLY_VAL */
		0x00000000,		/* DRAMC_ADDR_OUTPUT_DLY */
		0x00000000,		/* DRAMC_CLK_OUTPUT_DLY */
		0x11001640,		/* DRAMC_ACTIM1_VAL*/
		0x17800000,		/* DRAMC_MISCTL0_VAL*/
		0x04002201,		/* DRAMC_ACTIM05T_VAL*/
		{0x40000000,0,0,0},		/* DRAM RANK SIZE */
		{0,0,0,0,0,0,0,0,0,0},		/* reserved 10 */
		0x00C30001,		/* LPDDR3_MODE_REG1 */
		0x00090002,		/* LPDDR3_MODE_REG2 */
		0x00020003,		/* LPDDR3_MODE_REG3 */
		0x00000006,		/* LPDDR3_MODE_REG5 */
		0x00FF000A,		/* LPDDR3_MODE_REG10 */
		0x0000003F,		/* LPDDR3_MODE_REG63 */
	} ,
	//H9TQ17ABJTMCUR_KUM
	{
		0x0,		/* sub_version */
		0x0203,		/* TYPE */
		9,		/* EMMC ID/FW ID checking length */
		0,		/* FW length */
		{0x90,0x01,0x4A,0x48,0x41,0x47,0x32,0x65,0x05,0x0,0x0,0x0,0x0,0x0,0x0,0x0},		/* NAND_EMMC_ID */
		{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},		/* FW_ID */
		0x0002A052,		/* EMI_CONA_VAL */
		0xAA00AA00,		/* DRAMC_DRVCTL0_VAL */
		0xAA00AA00,		/* DRAMC_DRVCTL1_VAL */
		0x66AA46F7,		/* DRAMC_ACTIM_VAL */
		0x01000000,		/* DRAMC_GDDR3CTL1_VAL */
		0xF00486C3,		/* DRAMC_CONF1_VAL */
		0xC00652D1,		/* DRAMC_DDR2CTL_VAL */
		0xBF020401,		/* DRAMC_TEST2_3_VAL */
		0x0180AE58,		/* DRAMC_CONF2_VAL */
		0xD1644E42,		/* DRAMC_PD_CTRL_VAL */
		0x00008888,		/* DRAMC_PADCTL3_VAL */
		0x88888888,		/* DRAMC_DQODLY_VAL */
		0x00000000,		/* DRAMC_ADDR_OUTPUT_DLY */
		0x00000000,		/* DRAMC_CLK_OUTPUT_DLY */
		0x11001640,		/* DRAMC_ACTIM1_VAL*/
		0x17800000,		/* DRAMC_MISCTL0_VAL*/
		0x04002201,		/* DRAMC_ACTIM05T_VAL*/
		{0x40000000,0x40000000,0,0},		/* DRAM RANK SIZE */
		{0,0,0,0,0,0,0,0,0,0},		/* reserved 10 */
		0x00C30001,		/* LPDDR3_MODE_REG1 */
		0x00090002,		/* LPDDR3_MODE_REG2 */
		0x00020003,		/* LPDDR3_MODE_REG3 */
		0x00000006,		/* LPDDR3_MODE_REG5 */
		0x00FF000A,		/* LPDDR3_MODE_REG10 */
		0x0000003F,		/* LPDDR3_MODE_REG63 */
	} ,
};
#endif

#endif	//end #ifdef HARDCODE_DRAM_SETTING

EMI_SETTINGS emi_setting_default_lpddr2 =
{

        //default
                0x0,            /* sub_version */
                0x0002,         /* TYPE */
                0,              /* EMMC ID/FW ID checking length */
                0,              /* FW length */
                {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},              /* NAND_EMMC_ID */
                {0x00,0x0,0x0,0x0,0x0,0x0,0x0,0x0},             /* FW_ID */
                0x0000212E,             /* EMI_CONA_VAL */
                0xAA00AA00,             /* DRAMC_DRVCTL0_VAL */
                0xAA00AA00,             /* DRAMC_DRVCTL1_VAL */
                0x44584493,             /* DRAMC_ACTIM_VAL */
                0x01000000,             /* DRAMC_GDDR3CTL1_VAL */
                0xF0048683,             /* DRAMC_CONF1_VAL */
                0xA00632D1,             /* DRAMC_DDR2CTL_VAL */
                0xBF080401,             /* DRAMC_TEST2_3_VAL */
                0x0340633F,             /* DRAMC_CONF2_VAL */
                0x51642342,             /* DRAMC_PD_CTRL_VAL */
                0x00008888,             /* DRAMC_PADCTL3_VAL */
                0xEEEEEEEE,             /* DRAMC_DQODLY_VAL */
                0x00000000,             /* DRAMC_ADDR_OUTPUT_DLY */
                0x00000000,             /* DRAMC_CLK_OUTPUT_DLY */
                0x01000510,             /* DRAMC_ACTIM1_VAL*/
                0x07800000,             /* DRAMC_MISCTL0_VAL*/
                0x04002600,             /* DRAMC_ACTIM05T_VAL*/
                {0x10000000,0,0,0},                 /* DRAM RANK SIZE */
                {0,0,0,0,0,0,0,0,0,0},              /* reserved 10*4 bytes */
                0x00C30001,             /* LPDDR2_MODE_REG1 */
                0x00060002,             /* LPDDR2_MODE_REG2 */
                0x00020003,             /* LPDDR2_MODE_REG3 */
                0x00000000,             /* LPDDR2_MODE_REG5 */
                0x00FF000A,             /* LPDDR2_MODE_REG10 */
                0x0000003F,             /* LPDDR2_MODE_REG63 */
};

EMI_SETTINGS emi_setting_default_lpddr3 =
{

        //default
                0x0,            /* sub_version */
                0x0003,         /* TYPE */
                0,              /* EMMC ID/FW ID checking length */
                0,              /* FW length */
                {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},              /* NAND_EMMC_ID */
                {0x00,0x0,0x0,0x0,0x0,0x0,0x0,0x0},             /* FW_ID */
                0x0000212E,             /* EMI_CONA_VAL */
                0xAA00AA00,             /* DRAMC_DRVCTL0_VAL */
                0xAA00AA00,             /* DRAMC_DRVCTL1_VAL */
                0x44584493,             /* DRAMC_ACTIM_VAL */
                0x01000000,             /* DRAMC_GDDR3CTL1_VAL */
                0xF0048683,             /* DRAMC_CONF1_VAL */
                0xA00632F1,             /* DRAMC_DDR2CTL_VAL */
                0xBF080401,             /* DRAMC_TEST2_3_VAL */
                0x0340633F,             /* DRAMC_CONF2_VAL */
                0x51642342,             /* DRAMC_PD_CTRL_VAL */
                0x00008888,             /* DRAMC_PADCTL3_VAL */
                0xEEEEEEEE,             /* DRAMC_DQODLY_VAL */
                0x00000000,             /* DRAMC_ADDR_OUTPUT_DLY */
                0x00000000,             /* DRAMC_CLK_OUTPUT_DLY */
                0x11000510,             /* DRAMC_ACTIM1_VAL*/
                0x07800000,             /* DRAMC_MISCTL0_VAL*/
                0x04002600,             /* DRAMC_ACTIM05T_VAL*/
                {0x10000000,0,0,0},                 /* DRAM RANK SIZE */
                {0,0,0,0,0,0,0,0,0,0},              /* reserved 10*4 bytes */
                0x00C30001,             /* LPDDR3_MODE_REG1 */
                0x00060002,             /* LPDDR3_MODE_REG2 */
                0x00020003,             /* LPDDR3_MODE_REG3 */
                0x00000000,             /* LPDDR3_MODE_REG5 */
                0x00FF000A,             /* LPDDR3_MODE_REG10 */
                0x0000003F,             /* LPDDR3_MODE_REG63 */
};

static int mt_get_dram_type_for_dis(void)
{
    int i;
    int type = 2;

    type = (emi_settings[0].type & 0xF);

    for (i = 0 ; i < num_of_emi_records; i++)
    {
      //print("[EMI][%d] type%d\n",i,type);
      if (type != (emi_settings[i].type & 0xF))
      {
          printf("It's not allow to combine two type dram when combo discrete dram enable\n");
          ASSERT(0);
          break;
      }
    }

    return type;
}

unsigned int DRAM_MRR(int MRR_num)
{
	unsigned int MRR_value = 0x0;
	unsigned int bak_1e8, bak_1e4, bak_1dc, bak_008;
	unsigned int MRR_DQ_map[3];
	unsigned int dram_type;

	bak_1e8 = DRAMC_READ_REG(0x01E8);
	bak_1e4 = DRAMC_READ_REG(0x01E4);
	bak_1dc = DRAMC_READ_REG(0x01DC);
	bak_008 = DRAMC_READ_REG(0x0008);
	
	DRAMC_WRITE_REG(bak_1e4 & ~(1<<1), 0x01E4);
	DRAMC_WRITE_REG(bak_1dc | (1<<26), 0x01DC);
	DRAMC_WRITE_REG(bak_008 | (1<<28), 0x0008);
	gpt_busy_wait_us(1000);

	dram_type = mt_get_dram_type_for_dis();

	// For Discrete Tablet LPDDR2
	if ((emi_settings[0].type & 0x0F00) == 0x0F00)
		dram_type |= 0x0F00;

	if (TYPE_LPDDR2 == dram_type)
	{
		MRR_DQ_map[0] = 0x00090C0B;
		MRR_DQ_map[1] = 0x000F0A08;
		MRR_DQ_map[2] = 0x00000D0E;
	} 
	else if (TYPE_LPDDR3 == dram_type)
	{
		/*if((*((volatile unsigned int *)(0x10206174))& 0x00000800)!=0)
		{
			MRR_DQ_map[0] = 0x000E0B0D;
			MRR_DQ_map[1] = 0x00080F0C;
			MRR_DQ_map[2] = 0x0000090A;

		}
		else*/
		{
			MRR_DQ_map[0] = 0x000D0B08;
			MRR_DQ_map[1] = 0x000A0C09;
			MRR_DQ_map[2] = 0x00000F0E;
		}
	}
	else if (0x0F02 == dram_type) // Discrete Tablet LPDDR2
	{
		MRR_DQ_map[0] = 0x000D0B08;
		MRR_DQ_map[1] = 0x000A0C09;
		MRR_DQ_map[2] = 0x00000F0E;
	}

	//DRAMC_WRITE_CLEAR(0x00320000,0x1E8);
	//Setup DQ swap for bit[2:0]
	DRAMC_WRITE_REG(MRR_DQ_map[0],0x1F4);

	//Set MRSMA
	DRAMC_WRITE_REG(MRR_num,0x088);

	/*Enable MRR read for reading LPDDR2 refresh rate*/
	//DRAMC_WRITE_SET(0x00010000,0x1E8);
	//printf("Enable MRR read: 0x1E8:%x\n",DRAMC_READ_REG(0x01E8));
	DRAMC_WRITE_REG(bak_1e4 | (1<<1), 0x01E4);
	printf("Enable MRR read: 0x1E4:%x\n",DRAMC_READ_REG(0x01E4));

	//gpt_busy_wait_us(1000);//Wait > 1000000us
	while(!(DRAMC_READ_REG(0x03b8) & (1<<1)));
	
	MRR_value = (DRAMC_READ_REG(0x03B8) & 0x000000700) >> 8;
	printf("0x1f4:%x,refresh_rate[10:8]:%d\n",DRAMC_READ_REG(0x01F4),(DRAMC_READ_REG(0x03B8) & 0x000000700) >> 8);

	DRAMC_WRITE_REG(bak_1e4 & ~(1<<1), 0x01E4);
	printf("Disable MRR read: 0x1E4:%x\n", DRAMC_READ_REG(0x01E4));

	//Setup DQ swap for bit[5:3]
	DRAMC_WRITE_REG(MRR_DQ_map[1],0x1F4);
	
	DRAMC_WRITE_REG(bak_1e4 | (1<<1), 0x01E4);
	printf("Enable MRR read: 0x1E4:%x\n",DRAMC_READ_REG(0x01E4));

	//gpt_busy_wait_us(1000);//Wait > 1000000us
	while(!(DRAMC_READ_REG(0x03b8) & (1<<1)));

	MRR_value |= ((DRAMC_READ_REG(0x03B8) & 0x000000700) >> 8) << 0x3;
	printf("0x1f4:%x,refresh_rate[10:8]:%d\n",DRAMC_READ_REG(0x01F4),(DRAMC_READ_REG(0x03B8) & 0x000000700) >> 8);

	DRAMC_WRITE_REG(bak_1e4 & ~(1<<1), 0x01E4);
	printf("Disable MRR read: 0x1E4:%x\n", DRAMC_READ_REG(0x01E4));

	//Setup DQ swap for bit[9:6]
	DRAMC_WRITE_REG(MRR_DQ_map[2],0x1F4);

	DRAMC_WRITE_REG(bak_1e4 | (1<<1), 0x01E4);
	printf("Enable MRR read: 0x1E4:%x\n",DRAMC_READ_REG(0x01E4));

	//gpt_busy_wait_us(1000);//Wait > 1000000us
	while(!(DRAMC_READ_REG(0x03b8) & (1<<1)));
		
	MRR_value |= (((DRAMC_READ_REG(0x03B8) & 0x000000300) >> 8) << 0x6);
	printf("0x1f4:%x,refresh_rate[10:8]:%d\n",DRAMC_READ_REG(0x01F4),(DRAMC_READ_REG(0x03B8) & 0x000000700) >> 8);
	//print("MRR_value:%x\n",MRR_value);
	/*Disable MRR read for reading LPDDR2 refresh rate*/
	//DRAMC_WRITE_REG(bak_1e8,0x1E8);
	//printf("Disable MRR read: 0x1E8:%x\n",DRAMC_READ_REG(0x01E8));
	DRAMC_WRITE_REG(bak_1e4 & ~(1<<1), 0x01E4);
	printf("Disable MRR read: 0x1E4:%x\n", DRAMC_READ_REG(0x01E4));

	DRAMC_WRITE_REG(bak_1dc, 0x01DC);
	DRAMC_WRITE_REG(bak_008, 0x0008);

	/* reset infra*/
	printf("Before infra reset, 0x3B8:%x,0x10000034:%x\n", DRAMC_READ_REG(0x03B8), *(volatile unsigned *)(0x10000034));
	*(volatile unsigned *)(0x10000030) |= (1 << 0x12);
	__asm__ __volatile__ ("dsb" : : : "memory");
	gpt_busy_wait_us(200);//Wait > 200us
	*(volatile unsigned *)(0x10000034) &= (1 << 0x12);	
	printf("After infra reset, 0x3B8:%x,0x10000034:%x\n", DRAMC_READ_REG(0x03B8), *(volatile unsigned *)(0x10000034));

	printf("reset ddrphy\n");
	DRAMC_WRITE_REG((DRAMC_READ_REG(DRAMC_PHYCTL1)) | (1 << 28), DRAMC_PHYCTL1); 
	delay_a_while(1000); 
	DRAMC_WRITE_REG((DRAMC_READ_REG(DRAMC_PHYCTL1)) & (~(1 << 28)), DRAMC_PHYCTL1); 

	return MRR_value;
}

/*
 * init_lpddr2: Do initialization for LPDDR2.
 */
static void init_lpddr2(EMI_SETTINGS *emi_setting)
{
#if COMBO_LPDDR2 > 0
	unsigned int i, tmp = 0, conh = 0;
        // The following settings are from Yi-Chih.
        /* TINFO="Star mm_cosim_emi_config" */
        *EMI_CONA = emi_setting->EMI_CONA_VAL; //1.
        //*((V_UINT32P)(EMI_BASE+0x00000000))= 0x00001012; // EMI_CONA, 4Gb, x32, single rank, row14, col10
        //*((V_UINT32P)(EMI_BASE+0x00000000))= 0x00025052; // EMI_CONA, 4Gb, x32, dual rank, row14, col10
        
// Support CONH for 6Gb or 12Gb for start
        for(i = 0; i < 2; i++) {
		tmp = emi_setting->DRAM_RANK_SIZE[i];
		if(tmp == 0)
			break;

		switch (i) {
			case 0:
			case 1:
				if (tmp == 0x30000000)
					conh |= (0x00030000 << i*4);
				else if (tmp == 0x60000000)
					conh |= (0x00060000 << i*4);
				else
					conh |= 0;
				break;
			default:
				conh = 0;
				break;
				
		}
	}
        *EMI_CONH = conh;
// Support CONH for 6Gb or 12Gb for end
#if defined(MACH_TYPE_MT6753) || defined(MACH_TYPE_MT6737T)
			  *((V_UINT32P)(EMI_BASE+0x00000100))= 0x7f207c46;
        *((V_UINT32P)(EMI_BASE+0x00000108))= 0xa0a070da;
        *((V_UINT32P)(EMI_BASE+0x00000110))= 0xa0a07042;
        *((V_UINT32P)(EMI_BASE+0x00000118))= 0x07007046;
        *((V_UINT32P)(EMI_BASE+0x00000120))= 0x18306049;
        *((V_UINT32P)(EMI_BASE+0x00000128))= 0xa0a0704f;
        *((V_UINT32P)(EMI_BASE+0x00000134))= 0xa0a0704f; // Please set 0x134=0x128

        *((V_UINT32P)(EMI_BASE+0x00000008))=0x17283544;
        *((V_UINT32P)(EMI_BASE+0x00000010))=0x0a1a0b1a;
        *((V_UINT32P)(EMI_BASE+0x00000030))=0x2b2b2a38;


        *((V_UINT32P)(EMI_BASE+0x00000018))=0x3657587a; //SMI threthold
        *((V_UINT32P)(EMI_BASE+0x00000020))=0xffff0848;
        //*((V_UINT32P)(EMI_BASE+0x00000038))=0x00000000;
        *((V_UINT32P)(EMI_BASE+0x00000040))=0x80608846;
        *((V_UINT32P)(EMI_BASE+0x00000048))=0x1fc70801;
        *((V_UINT32P)(EMI_BASE+0x00000050))=0x1fe70aa1;

        *((V_UINT32P)(EMI_BASE+0x00000078))=0x34220e17;// defer ultra excpt MDMCU   //lower MDMCU perfer setting A


        *((V_UINT32P)(EMI_BASE+0x000000d0))=0xcccccccc;//R/8 W/8 outstanding
        *((V_UINT32P)(EMI_BASE+0x000000d8))=0xcccccccc;//R/8 W/8 outstanding

        *((V_UINT32P)(EMI_BASE+0x000000e8))=0x000601a7;// LPDDR3 //lower MDMCU perfer setting A

        *((V_UINT32P)(EMI_BASE+0x000000f0))=0x38460000; //[16] 0:disable urgent read

        *((V_UINT32P)(EMI_BASE+0x000000f8))=0x00000000;

        *((V_UINT32P)(EMI_BASE+0x00000140))=0x20406188;//0x12202488;   // 83 for low latency
        *((V_UINT32P)(EMI_BASE+0x00000144))=0x20406188;//0x12202488; //new add
        *((V_UINT32P)(EMI_BASE+0x00000148))=0x9719575e;//0323 chg, ori :0x00462f2f
        *((V_UINT32P)(EMI_BASE+0x0000014c))=0x9719575e; // new add
        *((V_UINT32P)(EMI_BASE+0x00000150))=0x64f3fc79;
        *((V_UINT32P)(EMI_BASE+0x00000154))=0x64f3fc79;

        *((V_UINT32P)(EMI_BASE+0x00000158))=0xff03ff00;// ???????????????????????0x08090800;

        //==============Scramble address==========================
        *((V_UINT32P)(EMI_BASE+0x00000028))=0x04210000;
        *((V_UINT32P)(EMI_BASE+0x00000060))=0x0000061a;
        //===========END===========================================
#else
        *((V_UINT32P)(EMI_BASE+0x00000100))= 0x7f07704a;
        *((V_UINT32P)(EMI_BASE+0x00000108))= 0xa0a070db;
        *((V_UINT32P)(EMI_BASE+0x00000110))= 0xa0a07042;
        *((V_UINT32P)(EMI_BASE+0x00000118))= 0x07007047;
        *((V_UINT32P)(EMI_BASE+0x00000120))= 0x2030604b;
        *((V_UINT32P)(EMI_BASE+0x00000128))= 0xa0a07046;
        *((V_UINT32P)(EMI_BASE+0x00000134))= 0xa0a07046; // Please set 0x134=0x128

        *((V_UINT32P)(EMI_BASE+0x00000008))=0x0d1e293a;
        *((V_UINT32P)(EMI_BASE+0x00000010))=0x09190819;
        *((V_UINT32P)(EMI_BASE+0x00000030))=0x2b2b282e;


        *((V_UINT32P)(EMI_BASE+0x00000018))=0x3657587a; //SMI threthold
        *((V_UINT32P)(EMI_BASE+0x00000020))=0xFFFF0848;
        //*((V_UINT32P)(EMI_BASE+0x00000038))=0x00000000;

        *((V_UINT32P)(EMI_BASE+0x00000078))=0x34220e17;// defer ultra excpt MDMCU   //lower MDMCU perfer setting A


        *((V_UINT32P)(EMI_BASE+0x000000d0))=0xCCCCCCCC;//R/8 W/8 outstanding
        *((V_UINT32P)(EMI_BASE+0x000000d8))=0xcccccccc;//R/8 W/8 outstanding

        *((V_UINT32P)(EMI_BASE+0x000000e8))=0x00020027;// LPDDR3 //lower MDMCU perfer setting A

        *((V_UINT32P)(EMI_BASE+0x000000f0))=0x38460000; //[16] 0:disable urgent read

        *((V_UINT32P)(EMI_BASE+0x000000f8))=0x00000000;

        *((V_UINT32P)(EMI_BASE+0x00000140))=0x20406188;//0x12202488;   // 83 for low latency
        *((V_UINT32P)(EMI_BASE+0x00000144))=0x20406188;//0x12202488; //new add
        *((V_UINT32P)(EMI_BASE+0x00000148))=0x9719595e;//0323 chg, ori :0x00462f2f
        *((V_UINT32P)(EMI_BASE+0x0000014c))=0x9719595e; // new add
        *((V_UINT32P)(EMI_BASE+0x00000150))=0x64f3fc79;
        *((V_UINT32P)(EMI_BASE+0x00000154))=0x64f3fc79;

        *((V_UINT32P)(EMI_BASE+0x00000158))=0xff01ff00;// ???????????????????????0x08090800;

        //==============Scramble address==========================
        *((V_UINT32P)(EMI_BASE+0x00000028))=0x00421000;
        *((V_UINT32P)(EMI_BASE+0x00000060))=0x000006ff;
        //===========END===========================================
#endif
        //---- add for real chip--------------------------//
        // TX driving 0x0a
#ifdef ENABLE_REXTDN_APPLY
        *((V_UINT32P)(DRAMC0_BASE + 0x00b4)) = 0x00000000 | ((drvp<<28) + (drvn<<24) + (drvp<<12) + (drvn<<8)); // DQC (@ DRAMC)
        *((V_UINT32P)(DDRPHY_BASE + 0x00b4)) = 0x00000000 | ((drvp<<28) + (drvn<<24) + (drvp<<12) + (drvn<<8));
        *((V_UINT32P)(DRAMC0_BASE + 0x00b8)) = 0x00000000 | ((drvp<<28) + (drvn<<24) + (drvp<<12) + (drvn<<8)); // DQ (@ DRAMC)
        *((V_UINT32P)(DDRPHY_BASE + 0x00b8)) = 0x00000000 | ((drvp<<28) + (drvn<<24) + (drvp<<12) + (drvn<<8));
        *((V_UINT32P)(DRAMC0_BASE + 0x00bc)) = 0x00000000 | ((drvp<<28) + (drvn<<24) + (drvp<<12) + (drvn<<8));  // CMD (@ DDRPHY)
        *((V_UINT32P)(DDRPHY_BASE + 0x00bc)) = 0x00000000 | ((drvp<<28) + (drvn<<24) + (drvp<<12) + (drvn<<8));
#else
        // default 0xa
        *((V_UINT32P)(DRAMC0_BASE + 0x00b4)) = 0xAA00AA00;   //[15:12]DRVP/[11:8]DRVN, default: 0xaa22aa22
        *((V_UINT32P)(DDRPHY_BASE + 0x00b4)) = 0xAA00AA00;   //[15:12]DRVP/[11:8]DRVN, default: 0xaa22aa22
        *((V_UINT32P)(DRAMC0_BASE + 0x00b8)) = 0xAA00AA00;   //[31:28]DQS DRVP/[27:24]DQS DRVN;[15:12]DQ DRVP/[11:8]DQ DRVN, default: 0xaa22aa22
        *((V_UINT32P)(DDRPHY_BASE + 0x00b8)) = 0xAA00AA00;   //[31:28]DQS DRVP/[27:24]DQS DRVN;[15:12]DQ DRVP/[11:8]DQ DRVN, default: 0xaa22aa22
        *((V_UINT32P)(DRAMC0_BASE + 0x00bc)) = 0xAA00AA00;    // NOTE: CLK / CA driving @ DDRPHY
        *((V_UINT32P)(DDRPHY_BASE + 0x00bc)) = 0xAA00AA00;    // NOTE: CLK / CA driving @ DDRPHY
/*
        *((V_UINT32P)(DRAMC0_BASE + 0x00b4)) = DRAMC_DRVCTL00_VAL;   //[15:12]DRVP/[11:8]DRVN, default: 0xaa22aa22
        *((V_UINT32P)(DDRPHY_BASE + 0x00b4)) = DRAMC_DRVCTL00_VAL;
        *((V_UINT32P)(DRAMC0_BASE + 0x00b8)) = DRAMC_DRVCTL0_VAL;   //[31:28]DQS DRVP/[27:24]DQS DRVN;[15:12]DQ DRVP/[11:8]DQ DRVN, default: 0xaa22aa22
        *((V_UINT32P)(DDRPHY_BASE + 0x00b8)) = DRAMC_DRVCTL0_VAL;
        *((V_UINT32P)(DRAMC0_BASE + 0x00bc)) = DRAMC_DRVCTL1_VAL;   //[31:28]CLK DRVP/[27:24]CLK DRVN;[15:12]CMD DRVP/[11:8]CMD DRVN;, default: 0xaa22aa22
        *((V_UINT32P)(DDRPHY_BASE + 0x00bc)) = DRAMC_DRVCTL1_VAL;
*/
#endif

        // Pre-emphasis ON. Should be set after MEMPLL init.
        // No Pre-emphasis for D123
        //*((V_UINT32P)(DRAMC0_BASE + 0x0640)) |= 0x00003f00;
        //*((V_UINT32P)(DDRPHY_BASE + 0x0640)) |= 0x00003f00;

#ifdef fcENABLE_INTVREF
        *((V_UINT32P)(DRAMC0_BASE + 0x0644)) |= 0x00000001;
        *((V_UINT32P)(DDRPHY_BASE + 0x0644)) |= 0x00000001;
#else
        // Default set to external Vref, 0x644[0]=0
        *((V_UINT32P)(DRAMC0_BASE + 0x0644)) &= 0xfffffffe;
        *((V_UINT32P)(DDRPHY_BASE + 0x0644)) &= 0xfffffffe;
#endif

/*
#ifdef DRAMC_ASYNC
        *((V_UINT32P)(DRAMC0_BASE + 0x00fc)) = 0x07900000 ;  //ASYNC // Edward: [23] PBC_ARB_E1T=1 [16]MODE18V=0in 6582
        *((V_UINT32P)(DDRPHY_BASE + 0x00fc)) = 0x07900000 ;  //ASYNC
#else
        *((V_UINT32P)(DRAMC0_BASE + 0x00fc)) = DRAMC_MISCTL0_VAL ;
        *((V_UINT32P)(DDRPHY_BASE + 0x00fc)) = DRAMC_MISCTL0_VAL ;
#endif
*/
        // Darren
        //*((V_UINT32P)(DRAMC0_BASE + 0x00fc)) = 0x07800000;
        //*((V_UINT32P)(DDRPHY_BASE + 0x00fc)) = 0x07800000;
        *((V_UINT32P)(DRAMC0_BASE + 0x00fc)) = emi_setting->DRAMC_MISCTL0_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x00fc)) = emi_setting->DRAMC_MISCTL0_VAL;

        *((V_UINT32P)(DRAMC0_BASE + 0x0048)) = 0x0001110d;
        *((V_UINT32P)(DDRPHY_BASE + 0x0048)) = 0x0001110d;

        *((V_UINT32P)(DRAMC0_BASE + 0x00d8)) = 0x00500900;    // [31:30] pinmux no use for D123, only PINMUX2 (0xf0[30])
        *((V_UINT32P)(DDRPHY_BASE + 0x00d8)) = 0x00500900;

        *((V_UINT32P)(DRAMC0_BASE + 0x00e4)) = 0x00000000;
        *((V_UINT32P)(DDRPHY_BASE + 0x00e4)) = 0x00000000;

        *((V_UINT32P)(DRAMC0_BASE + 0x008c)) = 0x00000001;
        *((V_UINT32P)(DDRPHY_BASE + 0x008c)) = 0x00000001;

        *((V_UINT32P)(DRAMC0_BASE + 0x0090)) = 0x00000000;
        *((V_UINT32P)(DDRPHY_BASE + 0x0090)) = 0x00000000;

        *((V_UINT32P)(DRAMC0_BASE + 0x0094)) = 0x80000000;
        *((V_UINT32P)(DDRPHY_BASE + 0x0094)) = 0x80000000;

        *((V_UINT32P)(DRAMC0_BASE + 0x00dc)) = 0x83004004;
        *((V_UINT32P)(DDRPHY_BASE + 0x00dc)) = 0x83004004;

        //*((V_UINT32P)(DRAMC0_BASE + 0x00e0)) = 0x19004004;
        //*((V_UINT32P)(DDRPHY_BASE + 0x00e0)) = 0x19004004;
#if 0 //def SYSTEM_26M //Edwin
        *((V_UINT32P)(DRAMC0_BASE + 0x00e0)) = 0x1c004004;
        *((V_UINT32P)(DDRPHY_BASE + 0x00e0)) = 0x1c004004;
        *((V_UINT32P)(DRAMC0_BASE + 0x0124)) = 0xaa080022;
        *((V_UINT32P)(DDRPHY_BASE + 0x0124)) = 0xaa080022;
#else
        #ifdef DDRPHY_3PLL_MODE
            *((V_UINT32P)(DRAMC0_BASE + 0x00e0)) = 0x1c004004;
            *((V_UINT32P)(DDRPHY_BASE + 0x00e0)) = 0x1c004004;
        #else
            *((V_UINT32P)(DRAMC0_BASE + 0x00e0)) = 0x1b004004;
            *((V_UINT32P)(DDRPHY_BASE + 0x00e0)) = 0x1b004004;
        #endif

        #ifdef DDRPHY_3PLL_MODE
            *((V_UINT32P)(DRAMC0_BASE + 0x0124)) = 0xaa080011;
            *((V_UINT32P)(DDRPHY_BASE + 0x0124)) = 0xaa080011;
        #else
            *((V_UINT32P)(DRAMC0_BASE + 0x0124)) = 0xaa080033;
            *((V_UINT32P)(DDRPHY_BASE + 0x0124)) = 0xaa080033;
        #endif
#endif

	if ((emi_settings[0].type & 0x0F00) == 0x0F00) { //for D123 Tablet POPLPDDR2
	    *((V_UINT32P)(DRAMC0_BASE + 0x00f0)) = 0xC0000000;  // PINMUX2 = 0 for LPDDR2 (1 for LPDDR3). [31] Enable 4-bit MUX
    	*((V_UINT32P)(DDRPHY_BASE + 0x00f0)) = 0xC0000000;
	}
	else {
    	*((V_UINT32P)(DRAMC0_BASE + 0x00f0)) = 0x80000000;  // PINMUX2 = 0 for LPDDR2 (1 for LPDDR3). [31] Enable 4-bit MUX
    	*((V_UINT32P)(DDRPHY_BASE + 0x00f0)) = 0x80000000;		
	}

/*
    #ifdef DRAMC_ASYNC
        //*((V_UINT32P)(DRAMC0_BASE + 0x00f4)) = 0x11100000;
        //*((V_UINT32P)(DDRPHY_BASE + 0x00f4)) = 0x11100000;
        *((V_UINT32P)(DRAMC0_BASE + 0x00f4)) = DRAMC_GDDR3CTL1_VAL;      // PHYSYNCM, default to 0
        *((V_UINT32P)(DDRPHY_BASE + 0x00f4)) = DRAMC_GDDR3CTL1_VAL;
    #else
        //*((V_UINT32P)(DRAMC0_BASE + 0x00f4)) = 0x01100000;
        //*((V_UINT32P)(DDRPHY_BASE + 0x00f4)) = 0x01100000;
        *((V_UINT32P)(DRAMC0_BASE + 0x00f4)) = DRAMC_GDDR3CTL1_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x00f4)) = DRAMC_GDDR3CTL1_VAL;
    #endif
*/
    // Darren
    //*((V_UINT32P)(DRAMC0_BASE + 0x00f4)) = 0x01000000;
    //*((V_UINT32P)(DDRPHY_BASE + 0x00f4)) = 0x01000000;
    *((V_UINT32P)(DRAMC0_BASE + 0x00f4)) = emi_setting->DRAMC_GDDR3CTL1_VAL;
    *((V_UINT32P)(DDRPHY_BASE + 0x00f4)) = emi_setting->DRAMC_GDDR3CTL1_VAL;
#ifdef PHYSYNC_MODE
    *((V_UINT32P)(DRAMC0_BASE + 0x00f4)) |= 0x10000000;
    *((V_UINT32P)(DDRPHY_BASE + 0x00f4)) |= 0x10000000;
#endif

#ifdef ENABLE_SYNC_MASK
    *((V_UINT32P)(DRAMC0_BASE + 0x00f4)) &= 0xff0fffff;
    *((V_UINT32P)(DDRPHY_BASE + 0x00f4)) &= 0xff0fffff;
#else
    *((V_UINT32P)(DRAMC0_BASE + 0x00f4)) |= 0x00f00000;
    *((V_UINT32P)(DDRPHY_BASE + 0x00f4)) |= 0x00f00000;
#endif

    *((V_UINT32P)(DRAMC0_BASE + 0x0168)) = 0x00000080;
    *((V_UINT32P)(DDRPHY_BASE + 0x0168)) = 0x00000080;


    *((V_UINT32P)(DRAMC0_BASE + 0x00d8)) = 0x00700900;
    *((V_UINT32P)(DDRPHY_BASE + 0x00d8)) = 0x00700900;

    // Darren
    //*((V_UINT32P)(DRAMC0_BASE + 0x0004)) = 0xf0048483;  // Edward: MATYPE[9:8] is for test engine use, set to 0 is safe.
    //*((V_UINT32P)(DDRPHY_BASE + 0x0004)) = 0xf0048483;
    *((V_UINT32P)(DRAMC0_BASE + 0x0004)) = emi_setting->DRAMC_CONF1_VAL;  // Edward: MATYPE[9:8] is for test engine use, set to 0 is safe.
    *((V_UINT32P)(DDRPHY_BASE + 0x0004)) = emi_setting->DRAMC_CONF1_VAL;

    //*((V_UINT32P)(DRAMC0_BASE + 0x007c)) = 0xc00646d1;
    //*((V_UINT32P)(DDRPHY_BASE + 0x007c)) = 0xc00646d1;
/*
    #ifdef DRAMC_ASYNC
        *((V_UINT32P)(DRAMC0_BASE + 0x007c)) = 0xa00632f1;    //DLE //Edward:TR2W[15:12]=3h TRTP[10:8]=2h, DATLAT[6:4]=7h
        *((V_UINT32P)(DDRPHY_BASE + 0x007c)) = 0xa00632f1;
    #else
        *((V_UINT32P)(DRAMC0_BASE + 0x007c)) = DRAMC_DDR2CTL_VAL ;
        *((V_UINT32P)(DDRPHY_BASE + 0x007c)) = DRAMC_DDR2CTL_VAL ;
    #endif
*/

    // Darren
    *((V_UINT32P)(DRAMC0_BASE + 0x007c)) = emi_setting->DRAMC_DDR2CTL_VAL ;
    *((V_UINT32P)(DDRPHY_BASE + 0x007c)) = emi_setting->DRAMC_DDR2CTL_VAL ;
/*
#if 1 //ndef DDR_POSTSIM
    *((V_UINT32P)(DRAMC0_BASE + 0x007c)) = 0xa00632d1;
    *((V_UINT32P)(DDRPHY_BASE + 0x007c)) = 0xa00632d1;
#else
    *((V_UINT32P)(DRAMC0_BASE + 0x007c)) = 0xa00632e1;    //Post-Sim, Tuning DLE
    *((V_UINT32P)(DDRPHY_BASE + 0x007c)) = 0xa00632e1;    //Post-Sim, Tuning DLE
#endif
*/

    *((V_UINT32P)(DRAMC0_BASE + 0x0028)) = 0xf1200f01;
    *((V_UINT32P)(DDRPHY_BASE + 0x0028)) = 0xf1200f01;

    *((V_UINT32P)(DRAMC0_BASE + 0x01e0)) = 0x3001ebff;
    *((V_UINT32P)(DDRPHY_BASE + 0x01e0)) = 0x3001ebff;

    *((V_UINT32P)(DRAMC0_BASE + 0x0158)) = 0xf0f0f0f0;  // Edward: 4 bit swap enable 0xf0f0f0f0. disable 0xff00ff00.
    *((V_UINT32P)(DDRPHY_BASE + 0x0158)) = 0xf0f0f0f0;

    *((V_UINT32P)(DRAMC0_BASE + 0x0400)) = 0x00111100;
    *((V_UINT32P)(DDRPHY_BASE + 0x0400)) = 0x00111100;

    *((V_UINT32P)(DRAMC0_BASE + 0x0404)) = 0x00000002;
    *((V_UINT32P)(DDRPHY_BASE + 0x0404)) = 0x00000002;

    *((V_UINT32P)(DRAMC0_BASE + 0x0408)) = 0x00222222;
    *((V_UINT32P)(DDRPHY_BASE + 0x0408)) = 0x00222222;

    *((V_UINT32P)(DRAMC0_BASE + 0x040c)) = 0x33330000;
    *((V_UINT32P)(DDRPHY_BASE + 0x040c)) = 0x33330000;

    *((V_UINT32P)(DRAMC0_BASE + 0x0410)) = 0x33330000;
    *((V_UINT32P)(DDRPHY_BASE + 0x0410)) = 0x33330000;

    *((V_UINT32P)(DRAMC0_BASE + 0x0110)) = 0x0b052311;          // Edward: Modify XRTR2W[14:12] to 2 & XRTR2R[10:8] to 3
    *((V_UINT32P)(DDRPHY_BASE + 0x0110)) = 0x0b052311;

    //*((V_UINT32P)(DRAMC0_BASE + 0x00e4)) = 0x00000004;
    //*((V_UINT32P)(DDRPHY_BASE + 0x00e4)) = 0x00000004;
    *((V_UINT32P)(DRAMC0_BASE + 0x00e4)) = 0x00000005;   //CKEBYCTL
    *((V_UINT32P)(DDRPHY_BASE + 0x00e4)) = 0x00000005;   //CKEBYCTL

        __asm__ __volatile__ ("dsb" : : : "memory");    // Edward : add this according to 6589.
        gpt_busy_wait_us(200);//Wait > 200us              // Edward : add this according to 6589.

    *((V_UINT32P)(DRAMC0_BASE + 0x0088)) = emi_setting->LPDDR2_MODE_REG_63;
    *((V_UINT32P)(DDRPHY_BASE + 0x0088)) = emi_setting->LPDDR2_MODE_REG_63;
    //*((V_UINT32P)(DRAMC0_BASE + 0x0088)) = 0x0000003f;
    //*((V_UINT32P)(DDRPHY_BASE + 0x0088)) = 0x0000003f;

    *((V_UINT32P)(DRAMC0_BASE + 0x01e4)) = 0x00000001;
    *((V_UINT32P)(DDRPHY_BASE + 0x01e4)) = 0x00000001;

        __asm__ __volatile__ ("dsb" : : : "memory");
        gpt_busy_wait_us(10);//Wait > 10us                              // Edward : add this according to DRAM spec, should wait at least 10us if not checking DAI.

    *((V_UINT32P)(DRAMC0_BASE + 0x01e4)) = 0x00000000;
    *((V_UINT32P)(DDRPHY_BASE + 0x01e4)) = 0x00000000;


    *((V_UINT32P)(DRAMC0_BASE + 0x0110)) &= (~0x7);   // Edward : Add this to disable two rank support for ZQ calibration tempority.
    *((V_UINT32P)(DDRPHY_BASE + 0x0110)) &= (~0x7);

    *((V_UINT32P)(DRAMC0_BASE + 0x0088)) = emi_setting->LPDDR2_MODE_REG_10;
    *((V_UINT32P)(DDRPHY_BASE + 0x0088)) = emi_setting->LPDDR2_MODE_REG_10;
    //*((V_UINT32P)(DRAMC0_BASE + 0x0088)) = 0x00ff000a;
    //*((V_UINT32P)(DDRPHY_BASE + 0x0088)) = 0x00ff000a;

    *((V_UINT32P)(DRAMC0_BASE + 0x01e4)) = 0x00000001;
    *((V_UINT32P)(DDRPHY_BASE + 0x01e4)) = 0x00000001;

         __asm__ __volatile__ ("dsb" : : : "memory");
        gpt_busy_wait_us(1);            //Wait > 1us. Edward : Add this because tZQINIT min value is 1us.

    *((V_UINT32P)(DRAMC0_BASE + 0x01e4)) = 0x00000000;
    *((V_UINT32P)(DDRPHY_BASE + 0x01e4)) = 0x00000000;

        // Edward : Add this for dual ranks support.
        if ( *(V_UINT32P)(EMI_CONA)& 0x20000)  {
                //for chip select 1: ZQ calibration
                *((V_UINT32P)(DRAMC0_BASE + 0x0110)) |= (0x8);
                *((V_UINT32P)(DDRPHY_BASE + 0x0110)) |= (0x8);

                *((V_UINT32P)(DRAMC0_BASE + 0x0088)) = emi_setting->LPDDR2_MODE_REG_10;
                *((V_UINT32P)(DDRPHY_BASE + 0x0088)) = emi_setting->LPDDR2_MODE_REG_10;
                //*((V_UINT32P)(DRAMC0_BASE + 0x0088)) = 0x00ff000a;
                //*((V_UINT32P)(DDRPHY_BASE + 0x0088)) = 0x00ff000a;

                *((V_UINT32P)(DRAMC0_BASE + 0x01e4)) = 0x00000001;
                *((V_UINT32P)(DDRPHY_BASE + 0x01e4)) = 0x00000001;

                __asm__ __volatile__ ("dsb" : : : "memory");
                gpt_busy_wait_us(1);//Wait > 100us

                *((V_UINT32P)(DRAMC0_BASE + 0x01e4)) = 0x00000000;
                *((V_UINT32P)(DDRPHY_BASE + 0x01e4)) = 0x00000000;

                //swap back
                *((V_UINT32P)(DRAMC0_BASE + 0x0110)) &= (~0x8);
                *((V_UINT32P)(DDRPHY_BASE + 0x0110)) &= (~0x8);
                *((V_UINT32P)(DRAMC0_BASE + 0x0110)) |= (0x1);
                *((V_UINT32P)(DDRPHY_BASE + 0x0110)) |= (0x1);
        }

        *((V_UINT32P)(DRAMC0_BASE + 0x0088)) = emi_setting->LPDDR2_MODE_REG_1;
        *((V_UINT32P)(DDRPHY_BASE + 0x0088)) = emi_setting->LPDDR2_MODE_REG_1;
        //*((V_UINT32P)(DRAMC0_BASE + 0x0088)) = 0x00c30001;
        //*((V_UINT32P)(DDRPHY_BASE + 0x0088)) = 0x00c30001;

        *((V_UINT32P)(DRAMC0_BASE + 0x01e4)) = 0x00000001;
        *((V_UINT32P)(DDRPHY_BASE + 0x01e4)) = 0x00000001;

         __asm__ __volatile__ ("dsb" : : : "memory");
        gpt_busy_wait_us(1);//Wait > 1us                        // Edward : 6589 has this delay. Seems no need.

        *((V_UINT32P)(DRAMC0_BASE + 0x01e4)) = 0x00000000;
        *((V_UINT32P)(DDRPHY_BASE + 0x01e4)) = 0x00000000;

        *((V_UINT32P)(DRAMC0_BASE + 0x0088)) = emi_setting->LPDDR2_MODE_REG_2;
        *((V_UINT32P)(DDRPHY_BASE + 0x0088)) = emi_setting->LPDDR2_MODE_REG_2;
        //*((V_UINT32P)(DRAMC0_BASE + 0x0088)) = 0x00060002;
        //*((V_UINT32P)(DDRPHY_BASE + 0x0088)) = 0x00060002;

        *((V_UINT32P)(DRAMC0_BASE + 0x01e4)) = 0x00000001;
        *((V_UINT32P)(DDRPHY_BASE + 0x01e4)) = 0x00000001;

         __asm__ __volatile__ ("dsb" : : : "memory");
        gpt_busy_wait_us(1);//Wait > 1us                        // Edward : 6589 has this delay. Seems no need.

        *((V_UINT32P)(DRAMC0_BASE + 0x01e4)) = 0x00000000;
        *((V_UINT32P)(DDRPHY_BASE + 0x01e4)) = 0x00000000;

        *((V_UINT32P)(DRAMC0_BASE + 0x0088)) = emi_setting->LPDDR2_MODE_REG_3;    //Dram Driving : 34.3-ohm  //Edward: modify default driving to 40ohm (1->2)?
        *((V_UINT32P)(DDRPHY_BASE + 0x0088)) = emi_setting->LPDDR2_MODE_REG_3;    //Dram Driving : 34.3-ohm
        //*((V_UINT32P)(DRAMC0_BASE + 0x0088)) = 0x00020003;    //Dram Driving : 34.3-ohm  //Edward: modify default driving to 40ohm (1->2)?
        //*((V_UINT32P)(DDRPHY_BASE + 0x0088)) = 0x00020003;    //Dram Driving : 34.3-ohm

        *((V_UINT32P)(DRAMC0_BASE + 0x01e4)) = 0x00000001;
        *((V_UINT32P)(DDRPHY_BASE + 0x01e4)) = 0x00000001;

         __asm__ __volatile__ ("dsb" : : : "memory");
        gpt_busy_wait_us(1);//Wait > 1us                // Edward : 6589 has this delay. Seems no need.

        *((V_UINT32P)(DRAMC0_BASE + 0x01e4)) = 0x00001100;
        *((V_UINT32P)(DDRPHY_BASE + 0x01e4)) = 0x00001100;

       // Edward : add two rank enable here.
        if ( *(V_UINT32P)(EMI_CONA)& 0x20000) {
                *((V_UINT32P)(DRAMC0_BASE + 0x0110)) = 0x00112391;      // Edward: Modify XRTR2W & XRTR2R.
                *((V_UINT32P)(DDRPHY_BASE + 0x0110)) = 0x00112391;
        } else {
                *((V_UINT32P)(DRAMC0_BASE + 0x0110)) = 0x00112390;
                *((V_UINT32P)(DDRPHY_BASE + 0x0110)) = 0x00112390;
        }

        *((V_UINT32P)(DRAMC0_BASE + 0x00e4)) = 0x00000001;  //CKEBYCTL
        *((V_UINT32P)(DDRPHY_BASE + 0x00e4)) = 0x00000001;  //CKEBYCTL

        *((V_UINT32P)(DRAMC0_BASE + 0x01ec)) = 0x00000001;    // Edward : Add this to enable dual scheduler. Should enable this for all DDR type.
        *((V_UINT32P)(DDRPHY_BASE + 0x01ec)) = 0x00000001;

        *((V_UINT32P)(DRAMC0_BASE + 0x0084)) = 0x00000a56;
        *((V_UINT32P)(DDRPHY_BASE + 0x0084)) = 0x00000a56;

        *((V_UINT32P)(DRAMC0_BASE + 0x000c)) = 0x00000000;
        *((V_UINT32P)(DDRPHY_BASE + 0x000c)) = 0x00000000;

        *((V_UINT32P)(DRAMC0_BASE + 0x0000)) = emi_setting->DRAMC_ACTIM_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x0000)) = emi_setting->DRAMC_ACTIM_VAL;
        //*((V_UINT32P)(DRAMC0_BASE + 0x0000)) = 0x44584493;
        //*((V_UINT32P)(DDRPHY_BASE + 0x0000)) = 0x44584493;

        // Edward : AC_TIME_0.5T control (new for 6582)
        *((V_UINT32P)(DRAMC0_BASE + 0x01f8)) = emi_setting->DRAMC_ACTIM05T_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x01f8)) = emi_setting->DRAMC_ACTIM05T_VAL;   //TWTR_M[26]=1,TFAW[13]=1,TWR_M[10]=1,TRAS[9]=1,TRP[7]=1,TRCD[6]=1,TRC[0]=1, TXP_05T =1
        //*((V_UINT32P)(DRAMC0_BASE + 0x01f8)) = 0x04002600; // OE problem (tRTW_05T=0)
        //*((V_UINT32P)(DDRPHY_BASE + 0x01f8)) = 0x04002600; // OE problem (tRTW_05T=0)

        *((V_UINT32P)(DRAMC0_BASE + 0x0044)) = emi_setting->DRAMC_TEST2_3_VAL;            // Edward : Modify DMPGTIM to 3f.
        *((V_UINT32P)(DDRPHY_BASE + 0x0044)) = emi_setting->DRAMC_TEST2_3_VAL;            // Edward : TRFC=18h, TRFC[19:16]=8h
        //*((V_UINT32P)(DRAMC0_BASE + 0x0044)) = 0xbf080401;            // Edward : Modify DMPGTIM to 3f.
        //*((V_UINT32P)(DDRPHY_BASE + 0x0044)) = 0xbf080401;            // Edward : TRFC=18h, TRFC[19:16]=8h

        *((V_UINT32P)(DRAMC0_BASE + 0x01e8)) = emi_setting->DRAMC_ACTIM1_VAL;    //82M need to enable R_DMREFRDIS // Edward : Why need to set [26]R_DMREFRDIS = 1 here? Should not enable according to SY.
        *((V_UINT32P)(DDRPHY_BASE + 0x01e8)) = emi_setting->DRAMC_ACTIM1_VAL;    //82M need to enable R_DMREFRDIS
        //*((V_UINT32P)(DRAMC0_BASE + 0x01e8)) = 0x01000510;    //82M need to enable R_DMREFRDIS // Edward : Why need to set [26]R_DMREFRDIS = 1 here? Should not enable according to SY.
        //*((V_UINT32P)(DDRPHY_BASE + 0x01e8)) = 0x01000510;    //82M need to enable R_DMREFRDIS

        //333Mhz
        //*((V_UINT32P)(DRAMC0_BASE + 0x0004)) = 0xf00487c3;
        //*((V_UINT32P)(DRAMC0_BASE + 0x0000)) = 0x55d84608;
        //*((V_UINT32P)(DRAMC0_BASE + 0x0044)) = 0x28000401;
        //*((V_UINT32P)(DRAMC0_BASE + 0x01e8)) = 0x00001010;

    #ifndef fcENABLE_FIX_REFCNT_ASYN
        *((V_UINT32P)(DRAMC0_BASE + 0x0008)) = emi_setting->DRAMC_CONF2_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x0008)) = emi_setting->DRAMC_CONF2_VAL;
        // move to below
        // Derping, [26:24] REFTHD=1
        // [28] REFDIS, to disable refresh here for set REFCNT_FR_CLK to avoid async issue
        //*((V_UINT32P)(DRAMC0_BASE + 0x0008)) = 0x01806c3f;
        //*((V_UINT32P)(DDRPHY_BASE + 0x0008)) = 0x01806c3f;
    #endif

        *((V_UINT32P)(DRAMC0_BASE + 0x0010)) = 0x00000000;
        *((V_UINT32P)(DDRPHY_BASE + 0x0010)) = 0x00000000;

        *((V_UINT32P)(DRAMC0_BASE + 0x00f8)) = 0xedcb000f;
        *((V_UINT32P)(DDRPHY_BASE + 0x00f8)) = 0xedcb000f;

        *((V_UINT32P)(DRAMC0_BASE + 0x0020)) = 0x00000000;
        *((V_UINT32P)(DDRPHY_BASE + 0x0020)) = 0x00000000;

/*
#ifdef DRAMC_ASYNC
        #ifdef DDRPHY_3PLL_MODE
            *((V_UINT32P)(DRAMC0_BASE + 0x0640)) |= (0x8c6e8008<<0);      //enable DDRPHY dynamic clk gating, // disable C_PHY_M_CK dynamic gating
            *((V_UINT32P)(DDRPHY_BASE + 0x0640)) |= (0x8c6e8008<<0);
        #else
            *((V_UINT32P)(DRAMC0_BASE + 0x0640)) |= (0xfc6f8008<<0);      //enable DDRPHY dynamic clk gating, // disable C_PHY_M_CK dynamic gating
             *((V_UINT32P)(DDRPHY_BASE + 0x0640)) |= (0xfc6f8008<<0);
        #endif
#else
        #ifdef DDRPHY_3PLL_MODE
           // Edward: Add 2PLL.
            #ifdef DDRPHY_2PLL
                  *((V_UINT32P)(DRAMC0_BASE + 0x0640)) |= (0xfc7e8008<<0);
                   *((V_UINT32P)(DDRPHY_BASE + 0x0640)) |= (0xfc7e8008<<0);
            #else
                *((V_UINT32P)(DRAMC0_BASE + 0x0640)) |= (0x8c7e8008<<0);      //enable DDRPHY dynamic clk gating, // disable C_PHY_M_CK dynamic gating // Edward: [3] is to save unused DDR3 I/O.
                *((V_UINT32P)(DDRPHY_BASE + 0x0640)) |= (0x8c7e8008<<0);
            #endif
        #else
            *((V_UINT32P)(DRAMC0_BASE + 0x0640)) |= (0xfc7f8008<<0);      //enable DDRPHY dynamic clk gating, // disable C_PHY_M_CK dynamic gating
            *((V_UINT32P)(DDRPHY_BASE + 0x0640)) |= (0xfc7f8008<<0);
        #endif
#endif
*/

// CG_EN set to 0 for bring up
// 0x640[3] NA for D123 (for DDR3)
//#ifdef fcENABLE_DDRPHY_DCM_FUNC
//  #ifdef DDRPHY_3PLL_MODE
//    #ifdef DDRPHY_2PLL
//        *((V_UINT32P)(DRAMC0_BASE + 0x0640)) |= (0xffe98000<<0);
//        *((V_UINT32P)(DDRPHY_BASE + 0x0640)) |= (0xffe98000<<0);
//    #else
//        *((V_UINT32P)(DRAMC0_BASE + 0x0640)) |= (0xffe98000<<0);      //enable DDRPHY dynamic clk gating, // disable C_PHY_M_CK dynamic gating
//        *((V_UINT32P)(DDRPHY_BASE + 0x0640)) |= (0xffe98000<<0);      //enable DDRPHY dynamic clk gating, // disable C_PHY_M_CK dynamic gating
//    #endif
//  #else
//        *((V_UINT32P)(DRAMC0_BASE + 0x0640)) |= (0xffe88000<<0);      //enable DDRPHY dynamic clk gating, // disable C_PHY_M_CK dynamic gating
//        *((V_UINT32P)(DDRPHY_BASE + 0x0640)) |= (0xffe88000<<0);      //enable DDRPHY dynamic clk gating, // disable C_PHY_M_CK dynamic gating
//  #endif
//#else
  #ifdef DDRPHY_3PLL_MODE
    #ifdef DDRPHY_2PLL
        *((V_UINT32P)(DRAMC0_BASE + 0x0640)) |= (0x00010000<<0);
        *((V_UINT32P)(DDRPHY_BASE + 0x0640)) |= (0x00010000<<0);
    #else
        *((V_UINT32P)(DRAMC0_BASE + 0x0640)) |= (0x00010000<<0);      //enable DDRPHY dynamic clk gating, // disable C_PHY_M_CK dynamic gating
        *((V_UINT32P)(DDRPHY_BASE + 0x0640)) |= (0x00010000<<0);      //enable DDRPHY dynamic clk gating, // disable C_PHY_M_CK dynamic gating
    #endif
  #else
    *((V_UINT32P)(DRAMC0_BASE + 0x0640)) |= (0x00010000<<0);      //enable DDRPHY dynamic clk gating, // disable C_PHY_M_CK dynamic gating
    *((V_UINT32P)(DDRPHY_BASE + 0x0640)) |= (0x00010000<<0);      //enable DDRPHY dynamic clk gating, // disable C_PHY_M_CK dynamic gating
  #endif
//#endif

    #ifdef fcENABLE_FIX_REFCNT_ASYN
        *((V_UINT32P)(DRAMC0_BASE + 0x0008)) = emi_setting->DRAMC_CONF2_VAL | 0x10000000;
        *((V_UINT32P)(DDRPHY_BASE + 0x0008)) = emi_setting->DRAMC_CONF2_VAL | 0x10000000;
        // Derping, [26:24] REFTHD=1
        // [28] REFDIS, to disable refresh here for set REFCNT_FR_CLK to avoid async issue
        //*((V_UINT32P)(DRAMC0_BASE + 0x0008)) = 0x11806c3f;
        //*((V_UINT32P)(DDRPHY_BASE + 0x0008)) = 0x11806c3f;
    #endif

        //*((V_UINT32P)(DRAMC0_BASE + 0x01dc)) = 0x51622842;  //enable DDRPHY dynamic clk gating
        //*((V_UINT32P)(DDRPHY_BASE + 0x01dc)) = 0x51622842;  //enable DDRPHY dynamic clk gating
        *((V_UINT32P)(DRAMC0_BASE + 0x01dc)) = emi_setting->DRAMC_PD_CTRL_VAL;    //enable DDRPHY dynamic clk gating & comb // Edward : [23:16]REFCNT_FR_CLK =64h, TXREFCNT[15:8]=23h
        *((V_UINT32P)(DDRPHY_BASE + 0x01dc)) = emi_setting->DRAMC_PD_CTRL_VAL;    //enable DDRPHY dynamic clk gating & comb
        //*((V_UINT32P)(DRAMC0_BASE + 0x01dc)) = 0xd1642342;    //enable DDRPHY dynamic clk gating & comb // Edward : [23:16]REFCNT_FR_CLK =64h, TXREFCNT[15:8]=23h
        //*((V_UINT32P)(DDRPHY_BASE + 0x01dc)) = 0xd1642342;    //enable DDRPHY dynamic clk gating & comb

    #ifdef fcENABLE_FIX_REFCNT_ASYN
        *((V_UINT32P)(DRAMC0_BASE + 0x0008)) = emi_setting->DRAMC_CONF2_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x0008)) = emi_setting->DRAMC_CONF2_VAL;
        // [28] REFDIS, to enable refresh here after REFCNT_FR_CLK to avoid async issue
        //*((V_UINT32P)(DRAMC0_BASE + 0x0008)) = 0x01806c3f;
        //*((V_UINT32P)(DDRPHY_BASE + 0x0008)) = 0x01806c3f;
    #endif

        if (emi_setting->EMI_CONA_VAL & 0x20000)  
        {
            *((V_UINT32P)(DRAMC0_BASE + 0x0110)) = 0x00112381;      //dsiable R_DMMRS2RK, this bit enable will impact MRR result // Edward : Modify [14:12] XRTR2W 1->2 [10:8] XRTR2R 1->3
            *((V_UINT32P)(DDRPHY_BASE + 0x0110)) = 0x00112381;      //dsiable R_DMMRS2RK, this bit enable will impact MRR result
        }
        else
        {
            *((V_UINT32P)(DRAMC0_BASE + 0x0110)) = 0x00112380;      //dsiable R_DMMRS2RK, this bit enable will impact MRR result // Edward : Modify [14:12] XRTR2W 1->2 [10:8] XRTR2R 1->3
            *((V_UINT32P)(DDRPHY_BASE + 0x0110)) = 0x00112380;      //dsiable R_DMMRS2RK, this bit enable will impact MRR result
        }

        // Before calibration, set default settings to avoid DLE calibration  fail. The following setting is for 1066Mbps.
        // After bring-up or HQA, please set optimized default here.
#if 1
        // GW coarse tune 14 (fh -> 1111b -> 11b, 10b)
        //*((V_UINT32P)(DRAMC0_BASE + 0x00e0)) = (*((V_UINT32P)(DRAMC0_BASE + 0x00e0)) & 0xf8ffffff) | (0x03<<24);
        //*((V_UINT32P)(DDRPHY_BASE + 0x00e0)) = (*((V_UINT32P)(DDRPHY_BASE + 0x00e0)) & 0xf8ffffff) | (0x03<<24);

        //*((V_UINT32P)(DRAMC0_BASE + 0x0124)) = (*((V_UINT32P)(DRAMC0_BASE + 0x0124)) & 0xffffff00) | 0xaa;
        //*((V_UINT32P)(DDRPHY_BASE + 0x0124)) = (*((V_UINT32P)(DDRPHY_BASE + 0x0124)) & 0xffffff00) | 0xaa;

        // GW fine tune 56 (38h)
        //*((V_UINT32P)(DRAMC0_BASE + 0x0094)) = 0x38383838;
        //*((V_UINT32P)(DDRPHY_BASE + 0x0094)) = 0x38383838;

        //*((V_UINT32P)(DRAMC0_BASE + 0x0098)) = 0x38383838;
        //*((V_UINT32P)(DDRPHY_BASE + 0x0098)) = 0x38383838;

        //  DLE 7
        *((V_UINT32P)(DRAMC0_BASE + 0x007c)) = (*((V_UINT32P)(DRAMC0_BASE + 0x007c)) & 0xFFFFFF8F) | ((7 & 0x07) <<4);
        *((V_UINT32P)(DDRPHY_BASE + 0x007c)) = (*((V_UINT32P)(DDRPHY_BASE + 0x007c)) & 0xFFFFFF8F) |  ((7 & 0x07) <<4);

        *((V_UINT32P)(DRAMC0_BASE + 0x00e4)) = (*((V_UINT32P)(DRAMC0_BASE + 0x00e4)) & 0xFFFFFFEF) | (((7 >> 3) & 0x01) << 4);
        *((V_UINT32P)(DDRPHY_BASE + 0x00e4)) =  (*((V_UINT32P)(DDRPHY_BASE + 0x00e4)) & 0xFFFFFFEF) | (((7 >> 3) & 0x01) << 4);

        // RX DQ, DQS input delay
        *((V_UINT32P)(DRAMC0_BASE + 0x0210)) = 0x05050101;
        *((V_UINT32P)(DDRPHY_BASE + 0x0210)) = 0x05050101;
        *((V_UINT32P)(DRAMC0_BASE + 0x0214)) = 0x00040307;
        *((V_UINT32P)(DDRPHY_BASE + 0x0214)) = 0x00040307;
        *((V_UINT32P)(DRAMC0_BASE + 0x0218)) = 0x01030503;
        *((V_UINT32P)(DDRPHY_BASE + 0x0218)) = 0x01030503;
        *((V_UINT32P)(DRAMC0_BASE + 0x021c)) = 0x03020100;
        *((V_UINT32P)(DDRPHY_BASE + 0x021c)) = 0x03020100;
        *((V_UINT32P)(DRAMC0_BASE + 0x0220)) = 0x05070001;
        *((V_UINT32P)(DDRPHY_BASE + 0x0220)) = 0x05070001;
        *((V_UINT32P)(DRAMC0_BASE + 0x0224)) = 0x03040602;
        *((V_UINT32P)(DDRPHY_BASE + 0x0224)) = 0x03040602;
        *((V_UINT32P)(DRAMC0_BASE + 0x0228)) = 0x03030405;
        *((V_UINT32P)(DDRPHY_BASE + 0x0228)) = 0x03030405;
        *((V_UINT32P)(DRAMC0_BASE + 0x022c)) = 0x04040004;
        *((V_UINT32P)(DDRPHY_BASE + 0x022c)) = 0x04040004;

        *((V_UINT32P)(DRAMC0_BASE + 0x0018)) = 0x1F221E22;
        *((V_UINT32P)(DDRPHY_BASE + 0x0018)) = 0x1F221E22;
        *((V_UINT32P)(DRAMC0_BASE + 0x001c)) = 0x1F221E22;
        *((V_UINT32P)(DDRPHY_BASE + 0x001c)) = 0x1F221E22;

#if 0
        // TX DQ, DQS output delay
        *((V_UINT32P)(DRAMC0_BASE + 0x0014)) = 0x00008888;   // DQS output delay
        *((V_UINT32P)(DDRPHY_BASE + 0x0014)) = 0x00008888;

        *((V_UINT32P)(DRAMC0_BASE + 0x0010)) = 0x00008888;   // DQM output delay
        *((V_UINT32P)(DDRPHY_BASE + 0x0010)) = 0x00008888;

        *((V_UINT32P)(DRAMC0_BASE + 0x0200)) = 0x88888888;   // DQ
        *((V_UINT32P)(DDRPHY_BASE + 0x0200)) = 0x88888888;
        *((V_UINT32P)(DRAMC0_BASE + 0x0204)) = 0x88888888;
        *((V_UINT32P)(DDRPHY_BASE + 0x0204)) = 0x88888888;
        *((V_UINT32P)(DRAMC0_BASE + 0x0208)) = 0x88888888;
        *((V_UINT32P)(DDRPHY_BASE + 0x0208)) = 0x88888888;
        *((V_UINT32P)(DRAMC0_BASE + 0x020c)) = 0x88888888;
        *((V_UINT32P)(DDRPHY_BASE + 0x020c)) = 0x88888888;
#endif

#endif
#endif //!COMBO_LPDDR2
}

#define DRAM_IS_2_RANK_MODE()	(*((volatile unsigned int*)(EMI_CONA)) & 0x20000)
static void init_ddr3(EMI_SETTINGS *emi_setting)
{
#if COMBO_PCDDR3 > 0
#ifdef DDR3_16B
	printf("mt6592 ddr3 x16 init : \n");
#else
	printf("mt6592 ddr3 x32 init : \n");
#endif

	// The following settings are from Yi-Chih.
	/* TINFO="Star mm_cosim_emi_config" */
	//----------------EMI Setting--------------------
	*((volatile unsigned int *)(EMI_BASE + 0x0060))=0x0000051f;
	*((volatile unsigned int *)(EMI_BASE + 0x0140))=0x20406188;//0x12202488;   // 83 for low latency
	*((volatile unsigned int *)(EMI_BASE + 0x0144))=0x20406188;//0x12202488; //new add
	*((volatile unsigned int *)(EMI_BASE + 0x0100))=0x40105906;//0x40105808; //m0 cpu ori:0x8020783f
	*((volatile unsigned int *)(EMI_BASE + 0x0108))=0x808050ea;//0xa0a05028; //m1 ori:0x80200000 // ultra can over limit
	*((volatile unsigned int *)(EMI_BASE + 0x0110))=0xffff50c4;//0xa0a05001; //m2 arm9
	*((volatile unsigned int *)(EMI_BASE + 0x0118))=0x0810d84a;//0x030fd80d; // ???????????????????????????????????????????????????????????//m3 mdmcu ori:0x80807809 ori:0x07007010 bit[12]:enable bw limiter
	*((volatile unsigned int *)(EMI_BASE + 0x0120))=0x40405042; //m4 fcore ori:0x8080781a
	// *((volatile unsigned int *)(EMI_BASE +0x0128))=0xa0a05001; //m5 peri
	// *((volatile unsigned int *)(EMI_BASE +0x0130))=0xa0a05028; //m6 vcodec ori:0x8080381a
	*((volatile unsigned int *)(EMI_BASE + 0x0148))=0x9719595e;//0323 chg, ori :0x00462f2f
	*((volatile unsigned int *)(EMI_BASE + 0x014c))=0x9719595e; // new add
	//-----------------------------------
	//-- modified by liya for EMI address mapping setting
#ifdef DDR3_16B
	*(volatile unsigned int*)(EMI_CONA) = 0x00003120;			// Edward 4Gb(256bx16) settings (may use  Hynix H5TC4G64AFR), 8 banks/15 rows/10 columns.
#else
	*(volatile unsigned int*)(EMI_CONA) = emi_setting->EMI_CONA_VAL;//0x00003122;			// Edward: 4Gb(256bx16)x2 settings (may use  Hynix H5TC4G64AFR), 8 banks/15 rows/10 columns.
#endif
	//-----------------------------------
	*((volatile unsigned int *)(EMI_BASE + 0x00f8))=0x00000000;
	*((volatile unsigned int *)(EMI_BASE + 0x0400))=0x007f0001;

	*((volatile unsigned int *)(EMI_BASE   +0x0008))=0x0b1b2737;// Chuan:0x181e1e2b Edwin:0x12161620
	*((volatile unsigned int *)(EMI_BASE   +0x0010))=0x08170717;// Chuan:0x18181818 Edwin:0x12121212
	*((volatile unsigned int *)(EMI_BASE   +0x0018))=0x0b1b2737;// Chuan:0x1e262b33 Edwin:0x161c2026
	*((volatile unsigned int *)(EMI_BASE   +0x0020))=0x08170717;// Chuan:0x1818181b Edwin:0x12121214
	*((volatile unsigned int *)(EMI_BASE   +0x0030))=0x2828272b;
	*((volatile unsigned int *)(EMI_BASE   +0x0038))=0x2828272b;

	*((volatile unsigned int *)(EMI_BASE + 0x0158))=0x00010800;// ???????????????????????0x08090800;
	*((volatile unsigned int *)(EMI_BASE + 0x0150))=0x6470fc79;
	*((volatile unsigned int *)(EMI_BASE + 0x0154))=0x6470fc79;
	//============== Defer WR threthold
	*((volatile unsigned int *)(EMI_BASE + 0x00f0))=0x38470000;
	//============== Reserve bufer
	//
	//MDMCU don't always ULTRA, but small age
	*((volatile unsigned int *)(EMI_BASE   +0x0078))=0x23110c03;// ???????????0x00030F4d;
	// Turn on M1 Ultra and all port DRAMC hi enable
	*((volatile unsigned int *)(EMI_BASE + 0x0158))=0x1f011f00;// ???????????????????????0x08090800;
	// RFF)_PBC_MASK
#ifdef DDR3_16B
	*((volatile unsigned int *)(EMI_BASE + 0x00e8))=0x00000004;//0x00070714; // EMI_TESTB[4]=1 -> pre-ultra= 1/2 starvation
#else
	*((volatile unsigned int *)(EMI_BASE + 0x00e8))= emi_setting->PCDDR3_MODE_REG4;//0x00000024; //Edward : 92 PCDDR3O32 bit ,n}32byte_wrap mode(0xe8[5])
#endif
	// Page hit is high
	*((volatile unsigned int *)(EMI_BASE + 0x0060))=0x0000051f;
	*((volatile unsigned int *)(EMI_BASE + 0x00d0))=0x84848c84;//R/8 W/8 outstanding
	*((volatile unsigned int *)(EMI_BASE + 0x00d8))=0x00480084;//R/8 W/8 outstanding
	//===========END===========================================
#ifdef DRAMC_ASYNC
	*((volatile unsigned int *)(DRAMC0_BASE + 0x00FC)) = emi_setting->DRAMC_MISCTL0_VAL;//0x07900000;   //ASYNC  //Edward: TXP[30:28]=0, PBC_ARB_E1T[23]=1 & MODE18V[16]=0.
	*((volatile unsigned int *)(DDRPHY_BASE + 0x00FC)) = emi_setting->DRAMC_MISCTL0_VAL;//0x07900000;   //ASYNC
#else
	*((volatile unsigned int *)(DRAMC0_BASE + 0x00FC)) = emi_setting->DRAMC_MISCTL0_VAL;//0x07800000;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x00FC)) = emi_setting->DRAMC_MISCTL0_VAL;//0x07800000;
#endif
    //default cross-talk pattern
	*(volatile unsigned int *)(DRAMC0_BASE + 0x0048) = 0x1e01110d;    // Edward: tZQCS[31:24]=0x1e
	*(volatile unsigned int *)(DDRPHY_BASE + 0x0048) = 0x1e01110d;      // cross-talk pattern

	*(volatile unsigned int *)(DRAMC0_BASE + 0x00F0) = 0xC0000000;   //PINMUX2 ==> 1 // Edward: DQ4BMUX[31]=1  [30]S2=1,
	*(volatile unsigned int *)(DDRPHY_BASE + 0x00F0) = 0xC0000000;
	*(volatile unsigned int *)(DRAMC0_BASE + 0x00D8) = 0x80100900;   //PINMUX(1,0) ==> (0,1) // Edward : 101b for DDR3x8x4, 110b for DDR3x16x2, 111b for DDR3x32
	*(volatile unsigned int *)(DDRPHY_BASE + 0x00D8) = 0x80100900;

	*(volatile unsigned int *)(DRAMC0_BASE + 0x00E4) = 0x000000a3;	//CKEBYCTL
	*(volatile unsigned int *)(DDRPHY_BASE + 0x00E4) = 0x000000a3;
	// Edward : After reset enable, need 500us before CKE high. (Reset low needs 200us.)
	 __asm__ __volatile__ ("dsb" : : : "memory");
	gpt_busy_wait_us(500);

	*(volatile unsigned int *)(DRAMC0_BASE + 0x008C) = 0x00000000;    //[20]DQ16COM1=0, [3]DQCMD=0, WTLATC=000, only DDR3 use
	*(volatile unsigned int *)(DDRPHY_BASE + 0x008C) = 0x00000000;    //[20]DQ16COM1=0, [3]DQCMD=0, WTLATC=000, only DDR3 use

	*(volatile unsigned int *)(DRAMC0_BASE + 0x0090) = 0x00000000;
	*(volatile unsigned int *)(DDRPHY_BASE + 0x0090) = 0x00000000;

	*(volatile unsigned int *)(DRAMC0_BASE + 0x0094) = 0x80000000;
	*(volatile unsigned int *)(DDRPHY_BASE + 0x0094) = 0x80000000;

	*(volatile unsigned int *)(DRAMC0_BASE + 0x00DC) = 0x83080080;
	*(volatile unsigned int *)(DDRPHY_BASE + 0x00DC) = 0x83080080;

	*(volatile unsigned int *)(DRAMC0_BASE + 0x00E0) = 0x13080080;
	*(volatile unsigned int *)(DDRPHY_BASE + 0x00E0) = 0x13080080;

#ifdef DDR3_16B
	*(volatile unsigned int *)(DRAMC0_BASE + 0x00F0) = 0xC0000000;   //pinmux2=1   // Edward: DQ4BMUX[31]=1  [30]S2=1,
	*(volatile unsigned int *)(DDRPHY_BASE + 0x00F0) = 0xC0000000;   //pinmux2=1
#else
	*(volatile unsigned int *)(DRAMC0_BASE + 0x00F0) = 0xC0000000;
	*(volatile unsigned int *)(DDRPHY_BASE + 0x00F0) = 0xC0000000;
#endif //DDR3_16B

	*(volatile unsigned int *)(DRAMC0_BASE + 0x00F4) = emi_setting->DRAMC_GDDR3CTL1_VAL;//0x01000000;
	*(volatile unsigned int *)(DDRPHY_BASE + 0x00F4) = emi_setting->DRAMC_GDDR3CTL1_VAL;//0x01000000;

    #ifdef PHYSYNC_MODE
	*((volatile unsigned int *)(DRAMC0_BASE + 0x00F4)) |= 0x10000000;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x00F4)) |= 0x10000000;
    #endif

	*(volatile unsigned int *)(DRAMC0_BASE + 0x0168) = 0x00000080;
	*(volatile unsigned int *)(DDRPHY_BASE + 0x0168) = 0x00000080;

	*(volatile unsigned int *)(DRAMC0_BASE + 0x0130) = 0x30000000;	 //dram_clk_en0 & 1
	*(volatile unsigned int *)(DDRPHY_BASE + 0x0130) = 0x30000000;	 //dram_clk_en0 & 1
	 __asm__ __volatile__ ("dsb" : : : "memory");
	delay_a_while(1000);									// Edward : add this because 6589 has this.

	*(volatile unsigned int *)(DRAMC0_BASE + 0x00D8) = 0x80300900;   //PINMUX (1,0) ==> (0,1)  // Edward : 101b for DDR3x8x4, 110b for DDR3x16x2, 111b for DDR3x32
	*(volatile unsigned int *)(DDRPHY_BASE + 0x00D8) = 0x80300900;

#ifdef DDR3_16B
	*(volatile unsigned int *)(DRAMC0_BASE + 0x0004) = 0xf0748662;
	*(volatile unsigned int *)(DDRPHY_BASE + 0x0004) = 0xf0748662;	 // Edward : MATYPE[9:8] set to 0 for safe.
#else
	*(volatile unsigned int *)(DRAMC0_BASE + 0x0004) = emi_setting->DRAMC_CONF1_VAL;//0xf0748663;	//DM64BIT=1
	*(volatile unsigned int *)(DDRPHY_BASE + 0x0004) = emi_setting->DRAMC_CONF1_VAL;//0xf0748663;
#endif

	*(volatile unsigned int *)(DRAMC0_BASE + 0x0124) = 0x80000011;
	*(volatile unsigned int *)(DDRPHY_BASE + 0x0124) = 0x80000011;

	*(volatile unsigned int *)(DRAMC0_BASE + 0x0094) = 0x40404040;
	*(volatile unsigned int *)(DDRPHY_BASE + 0x0094) = 0x40404040;

	*(volatile unsigned int *)(DRAMC0_BASE + 0x01C0) = 0x00000000;    // Edward: Should not enable HW gating & GW limit here.
	*(volatile unsigned int *)(DDRPHY_BASE + 0x01C0) = 0x00000000;

	*(volatile unsigned int *)(DRAMC0_BASE + 0x007C) = emi_setting->DRAMC_DDR2CTL_VAL;//0xa18e21c1;	  // Edward: TR2W[15:12]=3->1. Disable WOEN and ROEN.
	*(volatile unsigned int *)(DDRPHY_BASE + 0x007C) = emi_setting->DRAMC_DDR2CTL_VAL;//0xa18e21c1;

#ifdef DDR3_16B
	*(volatile unsigned int *)(DRAMC0_BASE + 0x008C) = 0x00100008;    //[20]DQ16COM1=1, [3]dqcmd=1, WTLATC=000, only pinmux1 use  // Edward : Only in 6582 case. In 6592, DDR3 does not use this setting.
	*(volatile unsigned int *)(DDRPHY_BASE + 0x008C) = 0x00100008;	  //[20]DQ16COM1=1, [3]dqcmd=1, WTLATC=000, only pinmux1 use
#endif

	*(volatile unsigned int *)(DRAMC0_BASE + 0x0028) = 0xF1200f01;
	*(volatile unsigned int *)(DDRPHY_BASE + 0x0028) = 0xF1200f01;

	*(volatile unsigned int *)(DRAMC0_BASE + 0x0158) = 0xf0f0f0f0;	//Edward: 4 bits swap 0xf0f0f0f0, disable 0xff00ff00.
	*(volatile unsigned int *)(DDRPHY_BASE + 0x0158) = 0xf0f0f0f0;

	*(volatile unsigned int *)(DRAMC0_BASE + 0x01E0) = 0x08000000;
	*(volatile unsigned int *)(DDRPHY_BASE + 0x01E0) = 0x08000000;

	*((volatile unsigned int *)(DRAMC0_BASE + 0x00E4)) = 0x000000a7;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x00E4)) = 0x000000a7;
	//-------------------------------------------------------------
#ifdef ENABLE_REXTDN_APPLY
	if(drvp < 0x06) drvp = 0x0A;
	if(drvn < 0x06) drvn = 0x0B;
	drvp = drvp + 2;
	drvn = drvn + 2;
	if(drvp > 0x0F) drvp = 0x0F;
	if(drvn > 0x0F) drvn = 0x0F;

	if(DRAM_IS_2_RANK_MODE())
	{//dual rank mode
		*((volatile unsigned int *)(DRAMC0_BASE + 0x00BC)) = 0x00000000 | ((drvp<<28) + (drvn<<24) + ((drvp)<<12) + ((drvn)<<8));
		*((volatile unsigned int *)(DDRPHY_BASE + 0x00BC)) = 0x00000000 | ((drvp<<28) + (drvn<<24) + ((drvp)<<12) + ((drvn)<<8));
	}
	else
	{//single rank mode
	*((volatile unsigned int *)(DRAMC0_BASE + 0x00BC)) = 0x00000000 | ((drvp<<28) + (drvn<<24) + ((drvp - 2)<<12) + ((drvn - 2)<<8));
	*((volatile unsigned int *)(DDRPHY_BASE + 0x00BC)) = 0x00000000 | ((drvp<<28) + (drvn<<24) + ((drvp - 2)<<12) + ((drvn - 2)<<8));
	}
	*((volatile unsigned int *)(DRAMC0_BASE + 0x00B8)) = 0x00000000 | ((drvp<<28) + (drvn<<24) + (drvp<<12) + (drvn<<8));
	*((volatile unsigned int *)(DDRPHY_BASE + 0x00B8)) = 0x00000000 | ((drvp<<28) + (drvn<<24) + (drvp<<12) + (drvn<<8));
	*((volatile unsigned int *)(DRAMC0_BASE + 0x00B4)) = 0x00000000 | ((drvp<<28) + (drvn<<24) + (drvp<<12) + (drvn<<8));
	*((volatile unsigned int *)(DDRPHY_BASE + 0x00B4)) = 0x00000000 | ((drvp<<28) + (drvn<<24) + (drvp<<12) + (drvn<<8));
#else
	//CLK, CMD driving
	*((volatile unsigned int *)(DRAMC0_BASE + 0x00BC)) = emi_setting->DRAMC_DRVCTL1_VAL;//0xaa00aa00;//0xaa00aa00;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x00BC)) = emi_setting->DRAMC_DRVCTL1_VAL;//0xaa00aa00;//0xaa00aa00;

	//DQS driving
	*((volatile unsigned int *)(DRAMC0_BASE + 0x00B8)) = emi_setting->DRAMC_DRVCTL0_VAL;//0xaa00aa00;//0xaa00aa00;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x00B8)) = emi_setting->DRAMC_DRVCTL0_VAL;//0xaa00aa00;//0xaa00aa00;

	//DQ driving
	*((volatile unsigned int *)(DRAMC0_BASE + 0x00B4)) = emi_setting->DRAMC_DRVCTL0_VAL;//0xaa00aa00;//0xaa00aa00;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x00B4)) = emi_setting->DRAMC_DRVCTL0_VAL;//0xaa00aa00;//0xaa00aa00;
#endif

	//Pre-emphassis enable
	//*((volatile unsigned int *)(DRAMC0_BASE + 0x0640)) |= 0x3F00;
	//*((volatile unsigned int *)(DDRPHY_BASE + 0x0640)) |= 0x3F00;

	#if 0
	//CLK delay
	*((volatile unsigned int *)(DRAMC0_BASE + 0x000C)) = 0x00000000;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x000C)) = 0x00000000;
	#else
	//CLK delay + 4step for 8LMMD
	*((volatile unsigned int *)(DRAMC0_BASE + 0x000C)) = 0x04000000;//CLK0
	*((volatile unsigned int *)(DDRPHY_BASE + 0x000C)) = 0x04000000;//CLK0

	*((volatile unsigned int *)(DRAMC0_BASE + 0x008C)) = 0x00040000;//CLK1
	*((volatile unsigned int *)(DDRPHY_BASE + 0x008C)) = 0x00040000;//CLK1
	#endif

	//CMD/ADDR delay
	*((volatile unsigned int *)(DRAMC0_BASE + 0x01A8)) = 0x00000000;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x01A8)) = 0x00000000;
	*((volatile unsigned int *)(DRAMC0_BASE + 0x01AC)) = 0x00000000;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x01AC)) = 0x00000000;
	*((volatile unsigned int *)(DRAMC0_BASE + 0x01B0)) = 0x00000000;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x01B0)) = 0x00000000;
	*((volatile unsigned int *)(DRAMC0_BASE + 0x01B4)) = 0x00000000;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x01B4)) = 0x00000000;
	*((volatile unsigned int *)(DRAMC0_BASE + 0x01B8)) = 0x00000000;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x01B8)) = 0x00000000;
	*((volatile unsigned int *)(DRAMC0_BASE + 0x01BC)) = 0x00000000;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x01BC)) = 0x00000000;
	*((volatile unsigned int *)(DRAMC0_BASE + 0x01C0)) &= 0xE0E0FFFF;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x01C0)) &= 0xE0E0FFFF;
	delay_a_while(1000);

	if(!DRAM_IS_2_RANK_MODE())
	{//single rank mode
		*((volatile unsigned int *)(DRAMC0_BASE + 0x0110)) = 0x0b051100;
		*((volatile unsigned int *)(DDRPHY_BASE + 0x0110)) = 0x0b051100;
	}
	else
	{//dual rank mode
		*((volatile unsigned int *)(DRAMC0_BASE + 0x0110)) = 0x0b051211;
		*((volatile unsigned int *)(DDRPHY_BASE + 0x0110)) = 0x0b051211;
	}
	//-------------------------------------------------------------
	//MRS2 -> MRS3 -> MRS1 - > MRS0
	 __asm__ __volatile__ ("dsb" : : : "memory");
	delay_a_while(2000);				// Edward : add this because 6589 has this.

	*((volatile unsigned int *)(DRAMC0_BASE + 0x0088)) = emi_setting->PCDDR3_MODE_REG2;//0x00004010;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x0088)) = emi_setting->PCDDR3_MODE_REG2;//0x00004010;

	*((volatile unsigned int *)(DRAMC0_BASE + 0x01e4)) = 0x00000001;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x01e4)) = 0x00000001;

	 __asm__ __volatile__ ("dsb" : : : "memory");
	delay_a_while(1000);

	*((volatile unsigned int *)(DRAMC0_BASE + 0x01e4)) = 0x00000000;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x01e4)) = 0x00000000;

	*((volatile unsigned int *)(DRAMC0_BASE + 0x0088)) = emi_setting->PCDDR3_MODE_REG3;//0x00006000;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x0088)) = emi_setting->PCDDR3_MODE_REG3;//0x00006000;

	*((volatile unsigned int *)(DRAMC0_BASE + 0x01e4)) = 0x00000001;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x01e4)) = 0x00000001;

	 __asm__ __volatile__ ("dsb" : : : "memory");
	delay_a_while(1000);

	*((volatile unsigned int *)(DRAMC0_BASE + 0x01e4)) = 0x00000000;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x01e4)) = 0x00000000;

	*((volatile unsigned int *)(DRAMC0_BASE + 0x0088)) = emi_setting->PCDDR3_MODE_REG1;//0x00002000;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x0088)) = emi_setting->PCDDR3_MODE_REG1;//0x00002000;

	*((volatile unsigned int *)(DRAMC0_BASE + 0x01e4)) = 0x00000001;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x01e4)) = 0x00000001;

	 __asm__ __volatile__ ("dsb" : : : "memory");
	delay_a_while(1000);

	*((volatile unsigned int *)(DRAMC0_BASE + 0x01e4)) = 0x00000000;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x01e4)) = 0x00000000;

	*((volatile unsigned int *)(DRAMC0_BASE + 0x0088)) = emi_setting->PCDDR3_MODE_REG0;//0x00001B51;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x0088)) = emi_setting->PCDDR3_MODE_REG0;//0x00001B51;

	*((volatile unsigned int *)(DRAMC0_BASE + 0x01e4)) = 0x00000001;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x01e4)) = 0x00000001;

	 __asm__ __volatile__ ("dsb" : : : "memory");
	delay_a_while(1000);

	*((volatile unsigned int *)(DRAMC0_BASE + 0x01e4)) = 0x00000000;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x01e4)) = 0x00000000;
	//-------------------------------------------------------------
	//ZQ calibration
	#if 0
	*((volatile unsigned int *)(DRAMC0_BASE + 0x0088)) = 0x00000400;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x0088)) = 0x00000400;

	*((volatile unsigned int *)(DRAMC0_BASE + 0x01e4)) = 0x00000010;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x01e4)) = 0x00000010;

	 __asm__ __volatile__ ("dsb" : : : "memory");
	delay_a_while(1000);

	*((volatile unsigned int *)(DRAMC0_BASE + 0x01e4)) = 0x00000000;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x01e4)) = 0x00000000;

	#else
	//for chip select 0: ZQ calibration
	*((volatile unsigned *)(DRAMC0_BASE + 0x0110)) &= (~0x7);
	*((volatile unsigned *)(DDRPHY_BASE + 0x0110)) &= (~0x7);

	*((volatile unsigned *)(DRAMC0_BASE + 0x0088)) = 0x00000400;
	*((volatile unsigned *)(DDRPHY_BASE + 0x0088)) = 0x00000400;

	*((volatile unsigned *)(DRAMC0_BASE + 0x01e4)) = 0x00000010;
	*((volatile unsigned *)(DDRPHY_BASE + 0x01e4)) = 0x00000010;

	__asm__ __volatile__ ("dsb" : : : "memory");
	delay_a_while(1000);

	*((volatile unsigned *)(DRAMC0_BASE + 0x01e4)) = 0x00000000;
	*((volatile unsigned *)(DDRPHY_BASE + 0x01e4)) = 0x00000000;
	if (DRAM_IS_2_RANK_MODE())
	{
		//for chip select 1: ZQ calibration
		*((volatile unsigned *)(DRAMC0_BASE + 0x0110)) |= (0x8);
		*((volatile unsigned *)(DDRPHY_BASE + 0x0110)) |= (0x8);

		*((volatile unsigned *)(DRAMC0_BASE + 0x0088)) = 0x00000400;
		*((volatile unsigned *)(DDRPHY_BASE + 0x0088)) = 0x00000400;

		*((volatile unsigned *)(DRAMC0_BASE + 0x01e4)) = 0x00000010;
		*((volatile unsigned *)(DDRPHY_BASE + 0x01e4)) = 0x00000010;

		__asm__ __volatile__ ("dsb" : : : "memory");
		delay_a_while(1000);
		*((volatile unsigned *)(DRAMC0_BASE + 0x01e4)) = 0x00000000;
		*((volatile unsigned *)(DDRPHY_BASE + 0x01e4)) = 0x00000000;
		//swap back
		*((volatile unsigned *)(DRAMC0_BASE + 0x0110)) &= (~0x8);
		*((volatile unsigned *)(DDRPHY_BASE + 0x0110)) &= (~0x8);
		*((volatile unsigned *)(DRAMC0_BASE + 0x0110)) |= (0x1);
		*((volatile unsigned *)(DDRPHY_BASE + 0x0110)) |= (0x1);
	}

	#endif
	__asm__ __volatile__ ("dsb" : : : "memory");
	 delay_a_while(1000);		// Edward : Add this for ZQinit=Max(512nCK, 640ns) .
	//-------------------------------------------------------------
	*((volatile unsigned int *)(DRAMC0_BASE + 0x01e4)) = 0x00001100;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x01e4)) = 0x00001100;

	*((volatile unsigned int *)(DRAMC0_BASE + 0x00E4)) = 0x000007a3;  //CKEBYCTL
	*((volatile unsigned int *)(DDRPHY_BASE + 0x00E4)) = 0x000007a3;

	*((volatile unsigned int *)(DRAMC0_BASE + 0x01ec)) = 0x00000001;  // Edward : Add this to enable dual scheduler according to Cl and SY.
	*((volatile unsigned int *)(DDRPHY_BASE + 0x01ec)) = 0x00000001;

	*((volatile unsigned int *)(DRAMC0_BASE + 0x01E0)) = 0x08000000;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x01E0)) = 0x08000000;

	*((volatile unsigned int *)(DRAMC0_BASE + 0x0088)) = 0x0000ffff;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x0088)) = 0x0000ffff;

	*((volatile unsigned int *)(DRAMC0_BASE + 0x01E4)) = 0x00000020;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x01E4)) = 0x00000020;

	*((volatile unsigned int *)(DRAMC0_BASE + 0x01E4)) = 0x00000000;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x01E4)) = 0x00000000;

	*((volatile unsigned int *)(DRAMC0_BASE + 0x0000)) = emi_setting->DRAMC_ACTIM_VAL;//0x337A4684;   // Edward: TFAW[23:20]=6->5h
	*((volatile unsigned int *)(DDRPHY_BASE + 0x0000)) = emi_setting->DRAMC_ACTIM_VAL;//0x337A4684;

	// Edward : AC_TIME_0.5T control (new for 6582)
	*((volatile unsigned int *)(DRAMC0_BASE + 0x01f8)) = emi_setting->DRAMC_ACTIM05T_VAL;//0x040014E1;  //TWTR_M[26]=1, TRTW[24]=1, TFAW[13]=1
	*((volatile unsigned int *)(DDRPHY_BASE + 0x01f8)) = emi_setting->DRAMC_ACTIM05T_VAL;//0x040014E1;

	*((volatile unsigned int *)(DRAMC0_BASE + 0x0044)) = emi_setting->DRAMC_TEST2_3_VAL;//0xbf890401;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x0044)) = emi_setting->DRAMC_TEST2_3_VAL;//0xbf890401;  // Edward: : TRFC=45h, TRFC[19:16]=5h

	*((volatile unsigned int *)(DRAMC0_BASE + 0x01E8)) = emi_setting->DRAMC_ACTIM1_VAL;//0x00000650;  // Edward: TRFC_BIT7_4[7:4]=4h
	*((volatile unsigned int *)(DDRPHY_BASE + 0x01E8)) = emi_setting->DRAMC_ACTIM1_VAL;//0x00000650;

	*((volatile unsigned int *)(DRAMC0_BASE + 0x0008)) = emi_setting->DRAMC_CONF2_VAL;//0x03046978;  // Edward: REDFCNT=41h. But not use.
	*((volatile unsigned int *)(DDRPHY_BASE + 0x0008)) = emi_setting->DRAMC_CONF2_VAL;//0x03046978;

	*((volatile unsigned int *)(DRAMC0_BASE + 0x0010)) = 0x00000000;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x0010)) = 0x00000000;

	*((volatile unsigned int *)(DRAMC0_BASE + 0x00F8)) = 0xedcb000f;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x00F8)) = 0xedcb000f;

	//*((volatile unsigned int *)(DRAMC0_BASE + 0x00FC)) = 0x27010000;
	//*((volatile unsigned int *)(DDRPHY_BASE + 0x00FC)) = 0x27010000;

	*((volatile unsigned int *)(DRAMC0_BASE + 0x01D8)) = 0x00c80008;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x01D8)) = 0x00c80008;

    // Edward : Added according to Cl comment for power saving.
    #ifdef DDRPHY_3PLL_MODE
        #ifdef DDRPHY_2PLL
              *((volatile unsigned int *)(DRAMC0_BASE + 0x0640)) |= (0xfc7e8000<<0);
        *((volatile unsigned int *)(DDRPHY_BASE + 0x0640)) |= (0xfc7e8000<<0);
        #else
         	*((volatile unsigned int *)(DRAMC0_BASE + 0x0640)) |= (0x8c7e8000<<0);      //enable DDRPHY dynamic clk gating, // disable C_PHY_M_CK dynamic gating // Edward: [3] is to save unused DDR3 I/O. how about other bits?
		*((volatile unsigned int *)(DDRPHY_BASE + 0x0640)) |= (0x8c7e8000<<0);		//enable DDRPHY dynamic clk gating, // disable C_PHY_M_CK dynamic gating // Edward: [3] is to save unused DDR3 I/O. how about other bits?
        #endif
    #else
        *((volatile unsigned int *)(DRAMC0_BASE + 0x0640)) |= (0xfc7f8000<<0);      //enable DDRPHY dynamic clk gating, // disable C_PHY_M_CK dynamic gating
        *((volatile unsigned int *)(DDRPHY_BASE + 0x0640)) |= (0xfc7f8000<<0);      //enable DDRPHY dynamic clk gating, // disable C_PHY_M_CK dynamic gating
    #endif

	*((volatile unsigned int *)(DRAMC0_BASE + 0x01dc)) = emi_setting->DRAMC_PD_CTRL_VAL;//0xd5972842;    //Edward : need to enable DDRPHY dynamic clk gating and FR.
	*((volatile unsigned int *)(DDRPHY_BASE + 0x01dc)) = emi_setting->DRAMC_PD_CTRL_VAL;//0xd5972842;    // REFFRERUN[24]=1, REFCNT_FR_CLK[23:16]=64h, TXREFCNT[15:8]=NA (Fix by HW)

	#if 1
	// Before calibration, set default settings to avoid some problems happened during calibration.
	// GW coarse tune 17 (11h -> 10001b -> 100b, 01b) ()
	*((volatile unsigned int *)(DRAMC0_BASE + 0x00e0)) = (*((volatile unsigned int *)(DRAMC0_BASE + 0x00e0)) & 0xf8ffffff) | (0x04<<24);
	*((volatile unsigned int *)(DDRPHY_BASE + 0x00e0)) = (*((volatile unsigned int *)(DDRPHY_BASE + 0x00e0)) & 0xf8ffffff) | (0x04<<24);

	*((volatile unsigned int *)(DRAMC0_BASE + 0x0124)) = (*((volatile unsigned int *)(DRAMC0_BASE + 0x0124)) & 0xffffff00) | 0x55;//rank0, rank1
	*((volatile unsigned int *)(DDRPHY_BASE + 0x0124)) = (*((volatile unsigned int *)(DDRPHY_BASE + 0x0124)) & 0xffffff00) | 0x55;//rank0, rank1

	// GW fine tune 64 (40h)
	//rank0 fine tune
	*((volatile unsigned int *)(DRAMC0_BASE + 0x0094)) = 0x40404040;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x0094)) = 0x40404040;
	//rank1 fine tune
	*((volatile unsigned int *)(DRAMC0_BASE + 0x0098)) = 0x40404040;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x0098)) = 0x40404040;

	//DLE 7
	*((volatile unsigned int *)(DRAMC0_BASE + 0x007c)) = (*((volatile unsigned int *)(DRAMC0_BASE + 0x007c)) & 0xFFFFFF8F) | ((7 & 0x07) <<4);
	*((volatile unsigned int *)(DDRPHY_BASE + 0x007c)) = (*((volatile unsigned int *)(DDRPHY_BASE + 0x007c)) & 0xFFFFFF8F) | ((7 & 0x07) <<4);

	*((volatile unsigned int *)(DRAMC0_BASE + 0x00e4)) = (*((volatile unsigned int *)(DRAMC0_BASE + 0x00e4)) & 0xFFFFFFEF) | (((7 >> 3) & 0x01) << 4);
	*((volatile unsigned int *)(DDRPHY_BASE + 0x00e4)) = (*((volatile unsigned int *)(DDRPHY_BASE + 0x00e4)) & 0xFFFFFFEF) | (((7 >> 3) & 0x01) << 4);

	// RX DQ, DQS input delay
	*((volatile unsigned int *)(DRAMC0_BASE + 0x0018)) = 0x1B1B1B1B;//rank0
	*((volatile unsigned int *)(DDRPHY_BASE + 0x0018)) = 0x1B1B1B1B;
	*((volatile unsigned int *)(DRAMC0_BASE + 0x001c)) = 0x1B1B1B1B;//rank1
	*((volatile unsigned int *)(DDRPHY_BASE + 0x001c)) = 0x1B1B1B1B;

	*((volatile unsigned int *)(DRAMC0_BASE + 0x0210)) = 0x05030002;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x0210)) = 0x05030002;
	*((volatile unsigned int *)(DRAMC0_BASE + 0x0214)) = 0x00010104;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x0214)) = 0x00010104;
	*((volatile unsigned int *)(DRAMC0_BASE + 0x0218)) = 0x00020202;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x0218)) = 0x00020202;
	*((volatile unsigned int *)(DRAMC0_BASE + 0x021c)) = 0x03030003;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x021c)) = 0x03030003;
	*((volatile unsigned int *)(DRAMC0_BASE + 0x0220)) = 0x02020000;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x0220)) = 0x02020000;
	*((volatile unsigned int *)(DRAMC0_BASE + 0x0224)) = 0x01030202;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x0224)) = 0x01030202;
	*((volatile unsigned int *)(DRAMC0_BASE + 0x0228)) = 0x01030502;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x0228)) = 0x01030502;
	*((volatile unsigned int *)(DRAMC0_BASE + 0x022c)) = 0x04030002;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x022c)) = 0x04030002;

	// TX DQ, DQS output delay
	*((volatile unsigned int *)(DRAMC0_BASE + 0x0014)) = emi_setting->DRAMC_PADCTL3_VAL;//0x00008888;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x0014)) = emi_setting->DRAMC_PADCTL3_VAL;//0x00008888;

	*((volatile unsigned int *)(DRAMC0_BASE + 0x0010)) = emi_setting->DRAMC_PADCTL3_VAL;//0x00008888;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x0010)) = emi_setting->DRAMC_PADCTL3_VAL;//0x00008888;

	*((volatile unsigned int *)(DRAMC0_BASE + 0x0200)) = emi_setting->DRAMC_DQODLY_VAL;//0x88888888;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x0200)) = emi_setting->DRAMC_DQODLY_VAL;//0x88888888;
	*((volatile unsigned int *)(DRAMC0_BASE + 0x0204)) = emi_setting->DRAMC_DQODLY_VAL;//0x88888888;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x0204)) = emi_setting->DRAMC_DQODLY_VAL;//0x88888888;
	*((volatile unsigned int *)(DRAMC0_BASE + 0x0208)) = emi_setting->DRAMC_DQODLY_VAL;//0x88888888;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x0208)) = emi_setting->DRAMC_DQODLY_VAL;//0x88888888;
	*((volatile unsigned int *)(DRAMC0_BASE + 0x020c)) = emi_setting->DRAMC_DQODLY_VAL;//0x88888888;
	*((volatile unsigned int *)(DDRPHY_BASE + 0x020c)) = emi_setting->DRAMC_DQODLY_VAL;//0x88888888;

	#endif

	printf("mt6592 ddr3 init finish!\n");
#endif //!COMBO_PCDDR3
}

#ifdef fcRESET_DDR_AFTER_CA_WR
// reset DRAM and MRS
void init_lpddr3_only(EMI_SETTINGS *emi_setting)
{
        // disable auto refresh
        *((V_UINT32P)(DRAMC0_BASE + 0x0008)) |= 0x10000000;
        *((V_UINT32P)(DDRPHY_BASE + 0x0008)) |= 0x10000000;

        // set MRS2RK
        *((V_UINT32P)(DRAMC0_BASE + 0x0110)) |= (0x10);
        *((V_UINT32P)(DDRPHY_BASE + 0x0110)) |= (0x10);

        // enable clock here for tINIT2
        *((V_UINT32P)(DRAMC0_BASE + 0x01dc)) |= 0x04000000;
        *((V_UINT32P)(DDRPHY_BASE + 0x01dc)) |= 0x04000000;
        gpt_busy_wait_us(1);    

        // CKEFIXON
        *((V_UINT32P)(DRAMC0_BASE + 0x00e4)) |= (0x4);
        *((V_UINT32P)(DDRPHY_BASE + 0x00e4)) |= (0x4);

        __asm__ __volatile__ ("dsb" : : : "memory");    // Edward : add this according to 6589.
        gpt_busy_wait_us(200);//Wait > 200us              // Edward : add this according to 6589.

        //Darren
        *((V_UINT32P)(DRAMC0_BASE + 0x0088)) = emi_setting->LPDDR3_MODE_REG_63;
        *((V_UINT32P)(DDRPHY_BASE + 0x0088)) = emi_setting->LPDDR3_MODE_REG_63;
        //*((V_UINT32P)(DRAMC0_BASE + 0x0088)) = 0x0000003F;
        //*((V_UINT32P)(DDRPHY_BASE + 0x0088)) = 0x0000003F;

        *((V_UINT32P)(DRAMC0_BASE + 0x01e4)) = 0x00000001;
        *((V_UINT32P)(DDRPHY_BASE + 0x01e4)) = 0x00000001;

        __asm__ __volatile__ ("dsb" : : : "memory");
        gpt_busy_wait_us(10);//Wait > 10us                              // Edward : add this according to DRAM spec, should wait at least 10us if not checking DAI.

        *((V_UINT32P)(DRAMC0_BASE + 0x01e4)) = 0x00000000;
        *((V_UINT32P)(DDRPHY_BASE + 0x01e4)) = 0x00000000;

        *((V_UINT32P)(DRAMC0_BASE + 0x0110)) &= (~0x7); // Edward : Add this to disable  two ranks support for ZQ calibration tempority.
        *((V_UINT32P)(DDRPHY_BASE + 0x0110)) &= (~0x7);

        //Darren
        *((V_UINT32P)(DRAMC0_BASE + 0x0088)) = emi_setting->LPDDR3_MODE_REG_10;
        *((V_UINT32P)(DDRPHY_BASE + 0x0088)) = emi_setting->LPDDR3_MODE_REG_10;
        //*((V_UINT32P)(DRAMC0_BASE + 0x0088)) = 0x00FF000A;
        //*((V_UINT32P)(DDRPHY_BASE + 0x0088)) = 0x00FF000A;

        *((V_UINT32P)(DRAMC0_BASE + 0x01e4)) = 0x00000001;
        *((V_UINT32P)(DDRPHY_BASE + 0x01e4)) = 0x00000001;

         __asm__ __volatile__ ("dsb" : : : "memory");
        gpt_busy_wait_us(1);            //Wait > 1us. Edward : Add this because tZQINIT min value is 1us.

        *((V_UINT32P)(DRAMC0_BASE + 0x01e4)) = 0x00000000;
        *((V_UINT32P)(DDRPHY_BASE + 0x01e4)) = 0x00000000;

        // Edward : Add this for dual ranks support.
        if (emi_setting->EMI_CONA_VAL & 0x20000)  {
                //for chip select 1: ZQ calibration
                // CS swap
                *((V_UINT32P)(DRAMC0_BASE + 0x0110)) |= (0x8);
                *((V_UINT32P)(DDRPHY_BASE + 0x0110)) |= (0x8);

                //Darren
                *((V_UINT32P)(DRAMC0_BASE + 0x0088)) = emi_setting->LPDDR3_MODE_REG_10;
                *((V_UINT32P)(DDRPHY_BASE + 0x0088)) = emi_setting->LPDDR3_MODE_REG_10;
                //*((V_UINT32P)(DRAMC0_BASE + 0x0088)) = 0x00FF000A;
                //*((V_UINT32P)(DDRPHY_BASE + 0x0088)) = 0x00FF000A;

                *((V_UINT32P)(DRAMC0_BASE + 0x01e4)) = 0x00000001;
                *((V_UINT32P)(DDRPHY_BASE + 0x01e4)) = 0x00000001;

                __asm__ __volatile__ ("dsb" : : : "memory");
              gpt_busy_wait_us(1);//Wait > 1us

                *((V_UINT32P)(DRAMC0_BASE + 0x01e4)) = 0x00000000;
                *((V_UINT32P)(DDRPHY_BASE + 0x01e4)) = 0x00000000;

                //swap back
                *((V_UINT32P)(DRAMC0_BASE + 0x0110)) &= (~0x8);
                *((V_UINT32P)(DDRPHY_BASE + 0x0110)) &= (~0x8);
                *((V_UINT32P)(DRAMC0_BASE + 0x0110)) |= (0x1);
                *((V_UINT32P)(DDRPHY_BASE + 0x0110)) |= (0x1);
        }

        //Darren
        *((V_UINT32P)(DRAMC0_BASE + 0x0088)) = emi_setting->LPDDR3_MODE_REG_1;
        *((V_UINT32P)(DDRPHY_BASE + 0x0088)) = emi_setting->LPDDR3_MODE_REG_1;
        //*((V_UINT32P)(DRAMC0_BASE + 0x0088)) = 0x00C30001;
        //*((V_UINT32P)(DDRPHY_BASE + 0x0088)) = 0x00C30001;
        

        *((V_UINT32P)(DRAMC0_BASE + 0x01e4)) = 0x00000001;
        *((V_UINT32P)(DDRPHY_BASE + 0x01e4)) = 0x00000001;

         __asm__ __volatile__ ("dsb" : : : "memory");
        gpt_busy_wait_us(1);            //Wait > 1us. Edward : Add this because tZQINIT min value is 1us.

        *((V_UINT32P)(DRAMC0_BASE + 0x01e4)) = 0x00000000;
        *((V_UINT32P)(DDRPHY_BASE + 0x01e4)) = 0x00000000;

        //Darren
        *((V_UINT32P)(DRAMC0_BASE + 0x0088)) = emi_setting->LPDDR3_MODE_REG_2;
        *((V_UINT32P)(DDRPHY_BASE + 0x0088)) = emi_setting->LPDDR3_MODE_REG_2;
        
/*
    #ifdef DDR_1333
        *((V_UINT32P)(DRAMC0_BASE + 0x0088)) = 0x00080002;
        *((V_UINT32P)(DDRPHY_BASE + 0x0088)) = 0x00080002;
    #else
      #ifdef DDR_1466
        *((V_UINT32P)(DRAMC0_BASE + 0x0088)) = 0x00090002;
        *((V_UINT32P)(DDRPHY_BASE + 0x0088)) = 0x00090002;
      #else
        *((V_UINT32P)(DRAMC0_BASE + 0x0088)) = 0x00060002;
        *((V_UINT32P)(DDRPHY_BASE + 0x0088)) = 0x00060002;
      #endif
    #endif
*/
        *((V_UINT32P)(DRAMC0_BASE + 0x01e4)) = 0x00000001;
        *((V_UINT32P)(DDRPHY_BASE + 0x01e4)) = 0x00000001;

         __asm__ __volatile__ ("dsb" : : : "memory");
        gpt_busy_wait_us(1);//Wait > 1us                        // Edward : 6589 has this delay. Seems no need.

        *((V_UINT32P)(DRAMC0_BASE + 0x01e4)) = 0x00001100;
        *((V_UINT32P)(DDRPHY_BASE + 0x01e4)) = 0x00001100;

        // clear MRS2RK
        *((V_UINT32P)(DRAMC0_BASE + 0x0110)) &= (~0x10);
        *((V_UINT32P)(DDRPHY_BASE + 0x0110)) &= (~0x10);     

        // CKEFIXON=0
        *((V_UINT32P)(DRAMC0_BASE + 0x00e4)) &= (~0x4);
        *((V_UINT32P)(DDRPHY_BASE + 0x00e4)) &= (~0x4);

        gpt_busy_wait_us(1);
        // disable clock here for tCPDED
        *((V_UINT32P)(DRAMC0_BASE + 0x01dc)) &= (~0x04000000);
        *((V_UINT32P)(DDRPHY_BASE + 0x01dc)) &= (~0x04000000);

        // enable auto refresh
        *((V_UINT32P)(DRAMC0_BASE + 0x0008)) &= (~0x10000000);
        *((V_UINT32P)(DDRPHY_BASE + 0x0008)) &= (~0x10000000);
}
#endif

static void init_lpddr3(EMI_SETTINGS *emi_setting) {
#if COMBO_LPDDR3 > 0
	unsigned int i, tmp = 0, conh = 0;
        // The following settings are from Yi-Chih.
        //C:---------------BEGIN EMI Setting from Yi-Chih----------
        *EMI_CONA = emi_setting->EMI_CONA_VAL; //1.
        //*((V_UINT32P)(EMI_BASE+0x00000000))= 0x00002012; // EMI_CONA, 8Gb, x32, single rank, row15, col10
        //*((V_UINT32P)(EMI_BASE+0x00000000))= 0x0002A052; // EMI_CONA, 8Gb+8Gb, x32, dual rank, row15, col10

// Support CONH for 6Gb or 12Gb for start
        for(i = 0; i < 2; i++) {
		tmp = emi_setting->DRAM_RANK_SIZE[i];
		if(tmp == 0)
			break;

		switch (i) {
			case 0:
			case 1:
				if (tmp == 0x30000000)
					conh |= (0x00030000 << i*4);
				else if (tmp == 0x60000000)
					conh |= (0x00060000 << i*4);
				else
					conh |= 0;
				break;
			default:
				conh = 0;
				break;
				
		}
	}
        *EMI_CONH = conh;
// Support CONH for 6Gb or 12Gb for end

#if defined(MACH_TYPE_MT6753) || defined(MACH_TYPE_MT6737T)
			  *((V_UINT32P)(EMI_BASE+0x00000100))= 0x7f207c46;
        *((V_UINT32P)(EMI_BASE+0x00000108))= 0xa0a070da;
        *((V_UINT32P)(EMI_BASE+0x00000110))= 0xa0a07042;
        *((V_UINT32P)(EMI_BASE+0x00000118))= 0x07007046;
        *((V_UINT32P)(EMI_BASE+0x00000120))= 0x18306049;
        *((V_UINT32P)(EMI_BASE+0x00000128))= 0xa0a0704f;
        *((V_UINT32P)(EMI_BASE+0x00000134))= 0xa0a0704f; // Please set 0x134=0x128

        *((V_UINT32P)(EMI_BASE+0x00000008))=0x17283544;
        *((V_UINT32P)(EMI_BASE+0x00000010))=0x0a1a0b1a;
        *((V_UINT32P)(EMI_BASE+0x00000030))=0x2b2b2a38;


        *((V_UINT32P)(EMI_BASE+0x00000018))=0x3657587a; //SMI threthold
        *((V_UINT32P)(EMI_BASE+0x00000020))=0xffff0848;
        //*((V_UINT32P)(EMI_BASE+0x00000038))=0x00000000;
        *((V_UINT32P)(EMI_BASE+0x00000040))=0x80608846;
        *((V_UINT32P)(EMI_BASE+0x00000048))=0x1fc70801;
        *((V_UINT32P)(EMI_BASE+0x00000050))=0x1fe70aa1;

        *((V_UINT32P)(EMI_BASE+0x00000078))=0x34220e17;// defer ultra excpt MDMCU   //lower MDMCU perfer setting A


        *((V_UINT32P)(EMI_BASE+0x000000d0))=0xcccccccc;//R/8 W/8 outstanding
        *((V_UINT32P)(EMI_BASE+0x000000d8))=0xcccccccc;//R/8 W/8 outstanding

        *((V_UINT32P)(EMI_BASE+0x000000e8))=0x000601a7;// LPDDR3 //lower MDMCU perfer setting A

        *((V_UINT32P)(EMI_BASE+0x000000f0))=0x38460000; //[16] 0:disable urgent read

        *((V_UINT32P)(EMI_BASE+0x000000f8))=0x00000000;

        *((V_UINT32P)(EMI_BASE+0x00000140))=0x20406188;//0x12202488;   // 83 for low latency
        *((V_UINT32P)(EMI_BASE+0x00000144))=0x20406188;//0x12202488; //new add
        *((V_UINT32P)(EMI_BASE+0x00000148))=0x9719575e;//0323 chg, ori :0x00462f2f
        *((V_UINT32P)(EMI_BASE+0x0000014c))=0x9719575e; // new add
        *((V_UINT32P)(EMI_BASE+0x00000150))=0x64f3fc79;
        *((V_UINT32P)(EMI_BASE+0x00000154))=0x64f3fc79;

        *((V_UINT32P)(EMI_BASE+0x00000158))=0xff03ff00;// ???????????????????????0x08090800;

        //==============Scramble address==========================
        *((V_UINT32P)(EMI_BASE+0x00000028))=0x04210000;
        *((V_UINT32P)(EMI_BASE+0x00000060))=0x0000061a;
        //===========END===========================================
#else
        *((V_UINT32P)(EMI_BASE+0x00000100))= 0x7f07704a;
        *((V_UINT32P)(EMI_BASE+0x00000108))= 0xa0a070db;
        *((V_UINT32P)(EMI_BASE+0x00000110))= 0xa0a07042;
        *((V_UINT32P)(EMI_BASE+0x00000118))= 0x07007047;
        *((V_UINT32P)(EMI_BASE+0x00000120))= 0x2030604b;
        *((V_UINT32P)(EMI_BASE+0x00000128))= 0xa0a07046;
        *((V_UINT32P)(EMI_BASE+0x00000134))= 0xa0a07046; // Please set 0x134=0x128

        *((V_UINT32P)(EMI_BASE+0x00000008))=0x0d1e293a;
        *((V_UINT32P)(EMI_BASE+0x00000010))=0x09190819;
        *((V_UINT32P)(EMI_BASE+0x00000030))=0x2b2b282e;


        *((V_UINT32P)(EMI_BASE+0x00000018))=0x3657587a; //SMI threthold
        *((V_UINT32P)(EMI_BASE+0x00000020))=0xFFFF0848;
        //*((V_UINT32P)(EMI_BASE+0x00000038))=0x00000000;

        *((V_UINT32P)(EMI_BASE+0x00000078))=0x34220e17;// defer ultra excpt MDMCU   //lower MDMCU perfer setting A


        *((V_UINT32P)(EMI_BASE+0x000000d0))=0xCCCCCCCC;//R/8 W/8 outstanding
        *((V_UINT32P)(EMI_BASE+0x000000d8))=0xcccccccc;//R/8 W/8 outstanding

        *((V_UINT32P)(EMI_BASE+0x000000e8))=0x00020027;// LPDDR3 //lower MDMCU perfer setting A

        *((V_UINT32P)(EMI_BASE+0x000000f0))=0x38460000; //[16] 0:disable urgent read

        *((V_UINT32P)(EMI_BASE+0x000000f8))=0x00000000;

        *((V_UINT32P)(EMI_BASE+0x00000140))=0x20406188;//0x12202488;   // 83 for low latency
        *((V_UINT32P)(EMI_BASE+0x00000144))=0x20406188;//0x12202488; //new add
        *((V_UINT32P)(EMI_BASE+0x00000148))=0x9719595e;//0323 chg, ori :0x00462f2f
        *((V_UINT32P)(EMI_BASE+0x0000014c))=0x9719595e; // new add
        *((V_UINT32P)(EMI_BASE+0x00000150))=0x64f3fc79;
        *((V_UINT32P)(EMI_BASE+0x00000154))=0x64f3fc79;

        *((V_UINT32P)(EMI_BASE+0x00000158))=0xff01ff00;// ???????????????????????0x08090800;

        //==============Scramble address==========================
        *((V_UINT32P)(EMI_BASE+0x00000028))=0x00421000;
        *((V_UINT32P)(EMI_BASE+0x00000060))=0x000006ff;
        //===========END===========================================
#endif
/*
if((*((volatile unsigned int *)(0x10206174))& 0x00000800)!=0)
{   //0x10206174[1]=1 means Argo-BI
      //CLK delay
	    *((volatile unsigned int *)(DRAMC0_BASE + 0x000C)) = 0x00000000;
	    *((volatile unsigned int *)(DDRPHY_BASE + 0x000C)) = 0x00000000;
	    //CMD/ADDR delay
	    *((volatile unsigned int *)(DRAMC0_BASE + 0x01A8)) = 0x03030303;
	    *((volatile unsigned int *)(DDRPHY_BASE + 0x01A8)) = 0x00000000;
	    *((volatile unsigned int *)(DRAMC0_BASE + 0x01AC)) = 0x00000000;
	    *((volatile unsigned int *)(DDRPHY_BASE + 0x01AC)) = 0x00000000;
	    *((volatile unsigned int *)(DRAMC0_BASE + 0x01B0)) = 0x00000000;
	    *((volatile unsigned int *)(DDRPHY_BASE + 0x01B0)) = 0x00000000;
	    *((volatile unsigned int *)(DRAMC0_BASE + 0x01B4)) = 0x00000302;
	    *((volatile unsigned int *)(DDRPHY_BASE + 0x01B4)) = 0x00000000;
	    *((volatile unsigned int *)(DRAMC0_BASE + 0x01B8)) = 0x00000000;
	    *((volatile unsigned int *)(DDRPHY_BASE + 0x01B8)) = 0x00000000;
	    *((volatile unsigned int *)(DRAMC0_BASE + 0x01BC)) = 0x00000000;
	    *((volatile unsigned int *)(DDRPHY_BASE + 0x01BC)) = 0x00000000;
	    *((volatile unsigned int *)(DRAMC0_BASE + 0x01C0)) &= 0xE0E0FFFF;
	    *((volatile unsigned int *)(DDRPHY_BASE + 0x01C0)) &= 0xE0E0FFFF;
      *((volatile unsigned int *)(DRAMC0_BASE + 0x01C0)) |= 0x00000000;
	    *((volatile unsigned int *)(DDRPHY_BASE + 0x01C0)) |= 0x00000000;
  }
*/        

        // CLK ([27:24]/CS1 ([31:28]) delay
        *((V_UINT32P)(DRAMC0_BASE + 0x000c)) = 0x00000000;
        *((V_UINT32P)(DDRPHY_BASE + 0x000c)) = 0x00000000;
    #if 0
        //CMD/ADDR delay
        *((volatile unsigned int *)(DRAMC0_BASE + 0x01A8)) = 0x03030303;
        *((volatile unsigned int *)(DDRPHY_BASE + 0x01A8)) = 0x00000000;
        *((volatile unsigned int *)(DRAMC0_BASE + 0x01AC)) = 0x00000000;
        *((volatile unsigned int *)(DDRPHY_BASE + 0x01AC)) = 0x00000000;	
        *((volatile unsigned int *)(DRAMC0_BASE + 0x01B0)) = 0x00000000;
        *((volatile unsigned int *)(DDRPHY_BASE + 0x01B0)) = 0x00000000;
        *((volatile unsigned int *)(DRAMC0_BASE + 0x01B4)) = 0x00000302;
        *((volatile unsigned int *)(DDRPHY_BASE + 0x01B4)) = 0x00000000;
        *((volatile unsigned int *)(DRAMC0_BASE + 0x01B8)) = 0x00000000;
        *((volatile unsigned int *)(DDRPHY_BASE + 0x01B8)) = 0x00000000;
        *((volatile unsigned int *)(DRAMC0_BASE + 0x01BC)) = 0x00000000;
        *((volatile unsigned int *)(DDRPHY_BASE + 0x01BC)) = 0x00000000;
        *((volatile unsigned int *)(DRAMC0_BASE + 0x01C0)) &= 0xE0E0FFFF;
        *((volatile unsigned int *)(DDRPHY_BASE + 0x01C0)) &= 0xE0E0FFFF;
        *((volatile unsigned int *)(DRAMC0_BASE + 0x01C0)) |= 0x00000000;
        *((volatile unsigned int *)(DDRPHY_BASE + 0x01C0)) |= 0x00000000;
    #endif
        
        // TX driving 0x0a
#ifdef ENABLE_REXTDN_APPLY
#ifdef MAX_DRAMC_DRIVING
        *((V_UINT32P)(DRAMC0_BASE + 0x00b4)) = 0xFF00FF00;
        *((V_UINT32P)(DDRPHY_BASE + 0x00b4)) = 0xFF00FF00;
        *((V_UINT32P)(DRAMC0_BASE + 0x00b8)) = 0xFF00FF00;
        *((V_UINT32P)(DDRPHY_BASE + 0x00b8)) = 0xFF00FF00;
        *((V_UINT32P)(DRAMC0_BASE + 0x00bc)) = 0xFF00FF00;
        *((V_UINT32P)(DDRPHY_BASE + 0x00bc)) = 0xFF00FF00;
#else
        *((V_UINT32P)(DRAMC0_BASE + 0x00b4)) = 0x00000000 | ((drvp<<28) + (drvn<<24) + (drvp<<12) + (drvn<<8)); // DQC (@ DRAMC)
        *((V_UINT32P)(DDRPHY_BASE + 0x00b4)) = 0x00000000 | ((drvp<<28) + (drvn<<24) + (drvp<<12) + (drvn<<8));
        *((V_UINT32P)(DRAMC0_BASE + 0x00b8)) = 0x00000000 | ((drvp<<28) + (drvn<<24) + (drvp<<12) + (drvn<<8)); // DQ (@ DRAMC)
        *((V_UINT32P)(DDRPHY_BASE + 0x00b8)) = 0x00000000 | ((drvp<<28) + (drvn<<24) + (drvp<<12) + (drvn<<8));
#if defined(MACH_TYPE_MT6735) || defined(MACH_TYPE_MT6737T)
        if ((drvp>1) && (drvn>1)) // drv -1 for large jitter. ONLY for D-1
        {
            *((V_UINT32P)(DRAMC0_BASE + 0x00bc)) = 0x00000000 | ((drvp<<28) + (drvn<<24) + ((drvp-1)<<12) + ((drvn-1)<<8));  // CMD (@ DDRPHY)
            *((V_UINT32P)(DDRPHY_BASE + 0x00bc)) = 0x00000000 | ((drvp<<28) + (drvn<<24) + ((drvp-1)<<12) + ((drvn-1)<<8));
        }
        else
        {
            *((V_UINT32P)(DRAMC0_BASE + 0x00bc)) = 0x00000000 | ((drvp<<28) + (drvn<<24) + (drvp<<12) + (drvn<<8));  // CMD (@ DDRPHY)
            *((V_UINT32P)(DDRPHY_BASE + 0x00bc)) = 0x00000000 | ((drvp<<28) + (drvn<<24) + (drvp<<12) + (drvn<<8));
        }
#else
        *((V_UINT32P)(DRAMC0_BASE + 0x00bc)) = 0x00000000 | ((drvp<<28) + (drvn<<24) + (drvp<<12) + (drvn<<8));  // CMD (@ DDRPHY)
        *((V_UINT32P)(DDRPHY_BASE + 0x00bc)) = 0x00000000 | ((drvp<<28) + (drvn<<24) + (drvp<<12) + (drvn<<8));
#endif
#endif
#else
        // default 0xa
        *((V_UINT32P)(DRAMC0_BASE + 0x00b4)) = 0xAA00AA00;   //[15:12]DRVP/[11:8]DRVN, default: 0xaa22aa22
        *((V_UINT32P)(DDRPHY_BASE + 0x00b4)) = 0xAA00AA00;   //[15:12]DRVP/[11:8]DRVN, default: 0xaa22aa22
        *((V_UINT32P)(DRAMC0_BASE + 0x00b8)) = 0xAA00AA00;   //[31:28]DQS DRVP/[27:24]DQS DRVN;[15:12]DQ DRVP/[11:8]DQ DRVN, default: 0xaa22aa22
        *((V_UINT32P)(DDRPHY_BASE + 0x00b8)) = 0xAA00AA00;   //[31:28]DQS DRVP/[27:24]DQS DRVN;[15:12]DQ DRVP/[11:8]DQ DRVN, default: 0xaa22aa22
        *((V_UINT32P)(DRAMC0_BASE + 0x00bc)) = 0xAA00AA00;    // NOTE: CLK / CA driving @ DDRPHY
        *((V_UINT32P)(DDRPHY_BASE + 0x00bc)) = 0xAA00AA00;    // NOTE: CLK / CA driving @ DDRPHY
/*
        *((V_UINT32P)(DRAMC0_BASE + 0x00b4)) = DRAMC_DRVCTL00_VAL;   //[15:12]DRVP/[11:8]DRVN, default: 0xaa22aa22
        *((V_UINT32P)(DDRPHY_BASE + 0x00b4)) = DRAMC_DRVCTL00_VAL;
        *((V_UINT32P)(DRAMC0_BASE + 0x00b8)) = emi_setting->DRAMC_DRVCTL0_VAL;   //[31:28]DQS DRVP/[27:24]DQS DRVN;[15:12]DQ DRVP/[11:8]DQ DRVN, default: 0xaa22aa22
        *((V_UINT32P)(DDRPHY_BASE + 0x00b8)) = emi_setting->DRAMC_DRVCTL0_VAL;
        *((V_UINT32P)(DRAMC0_BASE + 0x00bc)) = emi_setting->DRAMC_DRVCTL1_VAL;   //[31:28]CLK DRVP/[27:24]CLK DRVN;[15:12]CMD DRVP/[11:8]CMD DRVN;, default: 0xaa22aa22
        *((V_UINT32P)(DDRPHY_BASE + 0x00bc)) = emi_setting->DRAMC_DRVCTL1_VAL;
*/
#endif

#if defined(MACH_TYPE_MT6737T) || defined(MACH_TYPE_MT6737M)
        //DRAMC CLK duty
        *((V_UINT32P)(DRAMC0_BASE + 0x00cc)) &= 0xFFFFFF00;
        *((V_UINT32P)(DDRPHY_BASE + 0x00cc)) &= 0xFFFFFF00;
        *((V_UINT32P)(DRAMC0_BASE + 0x00cc)) |= 0x000000C3;
        *((V_UINT32P)(DDRPHY_BASE + 0x00cc)) |= 0x000000C3;
#endif

        // Pre-emphasis ON. Should be set after MEMPLL init.
        // No Pre-emphasis for D123
        //*((V_UINT32P)(DRAMC0_BASE + 0x0640)) |= 0x00003f00;
        //*((V_UINT32P)(DDRPHY_BASE + 0x0640)) |= 0x00003f00;

#ifdef fcENABLE_INTVREF
        *((V_UINT32P)(DRAMC0_BASE + 0x0644)) |= 0x00000001;
        *((V_UINT32P)(DDRPHY_BASE + 0x0644)) |= 0x00000001;
#else
        // Default set to external Vref, 0x644[0]=0
        *((V_UINT32P)(DRAMC0_BASE + 0x0644)) &= 0xfffffffe;
        *((V_UINT32P)(DDRPHY_BASE + 0x0644)) &= 0xfffffffe;
#endif

/*
#ifdef DRAMC_ASYNC
        *((V_UINT32P)(DRAMC0_BASE + 0x00fc)) =   emi_setting->DRAMC_MISCTL0_VAL | (1<<20);  //ASYNC // Edward: TXP[30:28]=2->0 MODE18V[16] 1->0(1.2V) PBC_ARB_E1T[23]=1??
        *((V_UINT32P)(DDRPHY_BASE + 0x00fc)) =   emi_setting->DRAMC_MISCTL0_VAL | (1<<20);  //ASYNC
#else
        *((V_UINT32P)(DRAMC0_BASE + 0x00fc)) =  emi_setting->DRAMC_MISCTL0_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x00fc)) =  emi_setting->DRAMC_MISCTL0_VAL;
#endif
*/

        *((V_UINT32P)(DRAMC0_BASE + 0x0048)) = 0x0001110d; // [16] XTALK
        *((V_UINT32P)(DDRPHY_BASE + 0x0048)) = 0x0001110d;
        //if((*((volatile unsigned int *)(0x10206174))& 0x00000800)!=0)
        //{
        //*((V_UINT32P)(DRAMC0_BASE + 0x00d8)) = 0xC0500900; //Edward: 100b for LPDDR2/3, 110b for DDR3x16x2
        //*((V_UINT32P)(DDRPHY_BASE + 0x00d8)) = 0xC0500900;
        //}
        //else
        //{
        *((V_UINT32P)(DRAMC0_BASE + 0x00d8)) = 0x00500900; //Edward: 100b for LPDDR2/3, 110b for DDR3x16x2
        *((V_UINT32P)(DDRPHY_BASE + 0x00d8)) = 0x00500900;
        //}
        *((V_UINT32P)(DRAMC0_BASE + 0x00e4)) = 0x00000000;
        *((V_UINT32P)(DDRPHY_BASE + 0x00e4)) = 0x00000000;

        *((V_UINT32P)(DRAMC0_BASE + 0x008c)) = 0x00000001;
        *((V_UINT32P)(DDRPHY_BASE + 0x008c)) = 0x00000001;

        *((V_UINT32P)(DRAMC0_BASE + 0x0090)) = 0x00000000;
        *((V_UINT32P)(DDRPHY_BASE + 0x0090)) = 0x00000000;

        *((V_UINT32P)(DRAMC0_BASE + 0x0094)) = 0x80000000;
        *((V_UINT32P)(DDRPHY_BASE + 0x0094)) = 0x80000000;

        *((V_UINT32P)(DRAMC0_BASE + 0x00dc)) = 0x83004004;
        *((V_UINT32P)(DDRPHY_BASE + 0x00dc)) = 0x83004004;

#if 0 //def SYSTEM_26M
    *((V_UINT32P)(DRAMC0_BASE + 0x00e0)) = 0x1b004004;
    *((V_UINT32P)(DDRPHY_BASE + 0x00e0)) = 0x1b004004;
    *((V_UINT32P)(DRAMC0_BASE + 0x0124)) = 0xaa080022;
    *((V_UINT32P)(DDRPHY_BASE + 0x0124)) = 0xaa080022;
#else
    #ifdef DDR_1333
        *((V_UINT32P)(DRAMC0_BASE + 0x00e0)) = 0x1c004004; // gating coarse
        *((V_UINT32P)(DDRPHY_BASE + 0x00e0)) = 0x1c004004;
         #ifdef DENALI_POSIM_MEM_MAX
            *((V_UINT32P)(DRAMC0_BASE + 0x0124)) = 0xaa080000; // gating coarse
            *((V_UINT32P)(DDRPHY_BASE + 0x0124)) = 0xaa080000;
            *((V_UINT32P)(DRAMC0_BASE + 0x0018)) = 0x10101010; // rx dqs delay
            *((V_UINT32P)(DDRPHY_BASE + 0x0018)) = 0x10101010;
        #else
            *((V_UINT32P)(DRAMC0_BASE + 0x0124)) = 0xaa080033;
            *((V_UINT32P)(DDRPHY_BASE + 0x0124)) = 0xaa080033;
        #endif
    #else
        #ifdef DDR_1466
        *((V_UINT32P)(DRAMC0_BASE + 0x00e0)) = 0x1d004004;
        *((V_UINT32P)(DDRPHY_BASE + 0x00e0)) = 0x1d004004;
        *((V_UINT32P)(DRAMC0_BASE + 0x0124)) = 0xaa080022;
        *((V_UINT32P)(DDRPHY_BASE + 0x0124)) = 0xaa080022;
        #else
        *((V_UINT32P)(DRAMC0_BASE + 0x00e0)) = 0x1b004004;
        *((V_UINT32P)(DDRPHY_BASE + 0x00e0)) = 0x1b004004;
        *((V_UINT32P)(DRAMC0_BASE + 0x0124)) = 0xaa080033;
        *((V_UINT32P)(DDRPHY_BASE + 0x0124)) = 0xaa080033;
        #endif
    #endif
#endif

        *((V_UINT32P)(DRAMC0_BASE + 0x00f0)) = 0xc0000000; // [30] pinmux, 1:LPDDR3, 0: LPDDR2. [31] Enable 4-bit MUX
        *((V_UINT32P)(DDRPHY_BASE + 0x00f0)) = 0xc0000000; // [30] pinmux, 1:LPDDR3, 0: LPDDR2. [31] Enable 4-bit MUX

        //Darren
        //*((V_UINT32P)(DRAMC0_BASE + 0x00f4)) = 0x01000000;
        //*((V_UINT32P)(DDRPHY_BASE + 0x00f4)) = 0x01000000;
        *((V_UINT32P)(DRAMC0_BASE + 0x00f4)) = emi_setting->DRAMC_GDDR3CTL1_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x00f4)) = emi_setting->DRAMC_GDDR3CTL1_VAL;
    #ifdef PHYSYNC_MODE
        *((V_UINT32P)(DRAMC0_BASE + 0x00f4)) |= 0x10000000;
        *((V_UINT32P)(DDRPHY_BASE + 0x00f4)) |= 0x10000000;
    #endif

    #ifdef ENABLE_SYNC_MASK
        *((V_UINT32P)(DRAMC0_BASE + 0x00f4)) &= 0xff0fffff;
        *((V_UINT32P)(DDRPHY_BASE + 0x00f4)) &= 0xff0fffff;
    #else
        *((V_UINT32P)(DRAMC0_BASE + 0x00f4)) |= 0x00f00000;
        *((V_UINT32P)(DDRPHY_BASE + 0x00f4)) |= 0x00f00000;
    #endif
      
    #if 0 //def PALLADIUM
        *((V_UINT32P)(DRAMC0_BASE + 0x0168)) = 0x00000000;
        *((V_UINT32P)(DDRPHY_BASE + 0x0168)) = 0x00000000;
    #else
        *((V_UINT32P)(DRAMC0_BASE + 0x0168)) = 0x00000080;
        *((V_UINT32P)(DDRPHY_BASE + 0x0168)) = 0x00000080;
    #endif

        //if((*((volatile unsigned int *)(0x10206174))& 0x00000800)!=0)
        //{
        //*((V_UINT32P)(DRAMC0_BASE + 0x00d8)) = 0xC0700900;   // Edward : 100b for LPDDR2/3, 110b for DDR3x16x2
        //*((V_UINT32P)(DDRPHY_BASE + 0x00d8)) = 0xC0700900;
        //}
        //else
        //{
        *((V_UINT32P)(DRAMC0_BASE + 0x00d8)) = 0x00700900;   // Edward : 100b for LPDDR2/3, 110b for DDR3x16x2
        *((V_UINT32P)(DDRPHY_BASE + 0x00d8)) = 0x00700900;
        //}

        // Set later
        // Darren
        //*((V_UINT32P)(DRAMC0_BASE + 0x0004)) = emi_setting->DRAMC_CONF1_VAL; // Edward: MATYPE[8:9]=0 for safe.
        //*((V_UINT32P)(DDRPHY_BASE + 0x0004)) = emi_setting->DRAMC_CONF1_VAL;

    //#ifdef DRAMC_ASYNC
        //*((V_UINT32P)(DRAMC0_BASE + 0x007c)) =  emi_setting->DRAMC_DDR2CTL_VAL | 0x1<<5;    //DLE
        //*((V_UINT32P)(DDRPHY_BASE + 0x007c)) =  emi_setting->DRAMC_DDR2CTL_VAL | 0x1<<5;
    //#else
        //*((V_UINT32P)(DRAMC0_BASE + 0x007c)) = emi_setting->DRAMC_DDR2CTL_VAL;
        //*((V_UINT32P)(DDRPHY_BASE + 0x007c)) = emi_setting->DRAMC_DDR2CTL_VAL;
    //#endif

        *((V_UINT32P)(DRAMC0_BASE + 0x0028)) = 0xf1200f01;
        *((V_UINT32P)(DDRPHY_BASE + 0x0028)) = 0xf1200f01;

        //if((*((volatile unsigned int *)(0x10206174))& 0x00000800)!=0)
        //{
        //    *((V_UINT32P)(DRAMC0_BASE + 0x01e0)) = 0x2011e9ff;    //LPDDR2EN set to 0
        //    *((V_UINT32P)(DDRPHY_BASE + 0x01e0)) = 0x2011e9ff;    //LPDDR2EN set to 0
        //}
        //else
        //{
        *((V_UINT32P)(DRAMC0_BASE + 0x01e0)) = 0x2001ebff;    //LPDDR2EN set to 0
        *((V_UINT32P)(DDRPHY_BASE + 0x01e0)) = 0x2001ebff;    //LPDDR2EN set to 0
        //}

        // Darren
        *((V_UINT32P)(DRAMC0_BASE + 0x01e8)) = emi_setting->DRAMC_ACTIM1_VAL;    //LPDDR3EN set to 1
        *((V_UINT32P)(DDRPHY_BASE + 0x01e8)) = emi_setting->DRAMC_ACTIM1_VAL;    //LPDDR3EN set to 1
        //*((V_UINT32P)(DRAMC0_BASE + 0x01e8)) = 0x11001330;    //LPDDR3EN set to 1
        //*((V_UINT32P)(DDRPHY_BASE + 0x01e8)) = 0x11001330;    //LPDDR3EN set to 1

        // reg map N/A (D123 has no this option. Force 4-bit swap)
        *((V_UINT32P)(DRAMC0_BASE + 0x0158)) = 0xf0f0f0f0;      // Edward: 4 bit swap enable 0xf0f0f0f0, disable 0xff00ff00
        *((V_UINT32P)(DDRPHY_BASE + 0x0158)) = 0xf0f0f0f0;

    #if 0 //def PALLADIUM
        *((V_UINT32P)(DRAMC0_BASE + 0x0400)) = 0x00111100;
        *((V_UINT32P)(DDRPHY_BASE + 0x0400)) = 0x00111100;

        *((V_UINT32P)(DRAMC0_BASE + 0x0404)) = 0x00000003;
        *((V_UINT32P)(DDRPHY_BASE + 0x0404)) = 0x00000003;

        *((V_UINT32P)(DRAMC0_BASE + 0x0408)) = 0x00222222;
        *((V_UINT32P)(DDRPHY_BASE + 0x0408)) = 0x00222222;

        *((V_UINT32P)(DRAMC0_BASE + 0x040c)) = 0x03330000;
        *((V_UINT32P)(DDRPHY_BASE + 0x040c)) = 0x03330000;

        *((V_UINT32P)(DRAMC0_BASE + 0x0410)) = 0x03330000;
        *((V_UINT32P)(DDRPHY_BASE + 0x0410)) = 0x03330000;
    #else
        *((V_UINT32P)(DRAMC0_BASE + 0x0400)) = 0x00111100;
        *((V_UINT32P)(DDRPHY_BASE + 0x0400)) = 0x00111100;

        *((V_UINT32P)(DRAMC0_BASE + 0x0404)) = 0x00000002;
        *((V_UINT32P)(DDRPHY_BASE + 0x0404)) = 0x00000002;

        *((V_UINT32P)(DRAMC0_BASE + 0x0408)) = 0x00222222;
        *((V_UINT32P)(DDRPHY_BASE + 0x0408)) = 0x00222222;

        *((V_UINT32P)(DRAMC0_BASE + 0x040c)) = 0x33330000;
        *((V_UINT32P)(DDRPHY_BASE + 0x040c)) = 0x33330000;

        *((V_UINT32P)(DRAMC0_BASE + 0x0410)) = 0x33330000;
        *((V_UINT32P)(DDRPHY_BASE + 0x0410)) = 0x33330000;
    #endif

        *((V_UINT32P)(DRAMC0_BASE + 0x0110)) = 0x0b052311;              //Edward: cross rank timing.
        *((V_UINT32P)(DDRPHY_BASE + 0x0110)) = 0x0b052311;

        // enable clock here for tINIT2
        *((V_UINT32P)(DRAMC0_BASE + 0x01dc)) |= 0x04000000;
        *((V_UINT32P)(DDRPHY_BASE + 0x01dc)) |= 0x04000000;
        gpt_busy_wait_us(1);

        *((V_UINT32P)(DRAMC0_BASE + 0x00e4)) = 0x00000005;   //CKEBYCTL
        *((V_UINT32P)(DDRPHY_BASE + 0x00e4)) = 0x00000005;   //CKEBYCTL

        __asm__ __volatile__ ("dsb" : : : "memory");    // Edward : add this according to 6589.
        gpt_busy_wait_us(200);//Wait > 200us              // Edward : add this according to 6589.

        // Darren
        *((V_UINT32P)(DRAMC0_BASE + 0x0088)) = emi_setting->LPDDR3_MODE_REG_63;
        *((V_UINT32P)(DDRPHY_BASE + 0x0088)) = emi_setting->LPDDR3_MODE_REG_63;
        //*((V_UINT32P)(DRAMC0_BASE + 0x0088)) = 0x0000003F;
        //*((V_UINT32P)(DDRPHY_BASE + 0x0088)) = 0x0000003F;

        *((V_UINT32P)(DRAMC0_BASE + 0x01e4)) = 0x00000001;
        *((V_UINT32P)(DDRPHY_BASE + 0x01e4)) = 0x00000001;

        __asm__ __volatile__ ("dsb" : : : "memory");
        gpt_busy_wait_us(10);//Wait > 10us                              // Edward : add this according to DRAM spec, should wait at least 10us if not checking DAI.

        *((V_UINT32P)(DRAMC0_BASE + 0x01e4)) = 0x00000000;
        *((V_UINT32P)(DDRPHY_BASE + 0x01e4)) = 0x00000000;

        *((V_UINT32P)(DRAMC0_BASE + 0x0110)) &= (~0x7); // Edward : Add this to disable  two ranks support for ZQ calibration tempority.
        *((V_UINT32P)(DDRPHY_BASE + 0x0110)) &= (~0x7);

        // Darren
        *((V_UINT32P)(DRAMC0_BASE + 0x0088)) = emi_setting->LPDDR3_MODE_REG_10;
        *((V_UINT32P)(DDRPHY_BASE + 0x0088)) = emi_setting->LPDDR3_MODE_REG_10;
        //*((V_UINT32P)(DRAMC0_BASE + 0x0088)) = 0x00FF000A;
        //*((V_UINT32P)(DDRPHY_BASE + 0x0088)) = 0x00FF000A;

        *((V_UINT32P)(DRAMC0_BASE + 0x01e4)) = 0x00000001;
        *((V_UINT32P)(DDRPHY_BASE + 0x01e4)) = 0x00000001;

         __asm__ __volatile__ ("dsb" : : : "memory");
        gpt_busy_wait_us(1);            //Wait > 1us. Edward : Add this because tZQINIT min value is 1us.

        *((V_UINT32P)(DRAMC0_BASE + 0x01e4)) = 0x00000000;
        *((V_UINT32P)(DDRPHY_BASE + 0x01e4)) = 0x00000000;

        // Edward : Add this for dual ranks support.
        if ( *(V_UINT32P)(EMI_CONA)& 0x20000)  {
                //for chip select 1: ZQ calibration
                *((V_UINT32P)(DRAMC0_BASE + 0x0110)) |= (0x8);
                *((V_UINT32P)(DDRPHY_BASE + 0x0110)) |= (0x8);

                // Darren
                *((V_UINT32P)(DRAMC0_BASE + 0x0088)) = emi_setting->LPDDR3_MODE_REG_10;
                *((V_UINT32P)(DDRPHY_BASE + 0x0088)) = emi_setting->LPDDR3_MODE_REG_10;
                //*((V_UINT32P)(DRAMC0_BASE + 0x0088)) = 0x00FF000A;
                //*((V_UINT32P)(DDRPHY_BASE + 0x0088)) = 0x00FF000A;

                *((V_UINT32P)(DRAMC0_BASE + 0x01e4)) = 0x00000001;
                *((V_UINT32P)(DDRPHY_BASE + 0x01e4)) = 0x00000001;

                __asm__ __volatile__ ("dsb" : : : "memory");
              gpt_busy_wait_us(1);//Wait > 1us

                *((V_UINT32P)(DRAMC0_BASE + 0x01e4)) = 0x00000000;
                *((V_UINT32P)(DDRPHY_BASE + 0x01e4)) = 0x00000000;

                //swap back
                *((V_UINT32P)(DRAMC0_BASE + 0x0110)) &= (~0x8);
                *((V_UINT32P)(DDRPHY_BASE + 0x0110)) &= (~0x8);
                *((V_UINT32P)(DRAMC0_BASE + 0x0110)) |= (0x1);
                *((V_UINT32P)(DDRPHY_BASE + 0x0110)) |= (0x1);
        }

        // Darren
        *((V_UINT32P)(DRAMC0_BASE + 0x0088)) = emi_setting->LPDDR3_MODE_REG_1;
        *((V_UINT32P)(DDRPHY_BASE + 0x0088)) = emi_setting->LPDDR3_MODE_REG_1;
        //*((V_UINT32P)(DRAMC0_BASE + 0x0088)) = 0x00C30001;
        //*((V_UINT32P)(DDRPHY_BASE + 0x0088)) = 0x00C30001;


        *((V_UINT32P)(DRAMC0_BASE + 0x01e4)) = 0x00000001;
        *((V_UINT32P)(DDRPHY_BASE + 0x01e4)) = 0x00000001;

         __asm__ __volatile__ ("dsb" : : : "memory");
        gpt_busy_wait_us(1);//Wait > 1us                        // Edward : 6589 has this delay. Seems no need.

        *((V_UINT32P)(DRAMC0_BASE + 0x01e4)) = 0x00000000;
        *((V_UINT32P)(DDRPHY_BASE + 0x01e4)) = 0x00000000;

#ifdef MAX_DRAM_DRIVING
        // Darren - MR3
        *((V_UINT32P)(DRAMC0_BASE + 0x0088)) = 0x00010003;
        *((V_UINT32P)(DDRPHY_BASE + 0x0088)) = 0x00010003;


        *((V_UINT32P)(DRAMC0_BASE + 0x01e4)) = 0x00000001;
        *((V_UINT32P)(DDRPHY_BASE + 0x01e4)) = 0x00000001;

         __asm__ __volatile__ ("dsb" : : : "memory");
        gpt_busy_wait_us(1);//Wait > 1us                        // Edward : 6589 has this delay. Seems no need.

        *((V_UINT32P)(DRAMC0_BASE + 0x01e4)) = 0x00000000;
        *((V_UINT32P)(DDRPHY_BASE + 0x01e4)) = 0x00000000;
#endif

        // Darren
        *((V_UINT32P)(DRAMC0_BASE + 0x0088)) = emi_setting->LPDDR3_MODE_REG_2;
        *((V_UINT32P)(DDRPHY_BASE + 0x0088)) = emi_setting->LPDDR3_MODE_REG_2;
/*
    #ifdef DDR_1333
        *((V_UINT32P)(DRAMC0_BASE + 0x0088)) = 0x00080002;
        *((V_UINT32P)(DDRPHY_BASE + 0x0088)) = 0x00080002;
    #else
      #ifdef DDR_1466
        *((V_UINT32P)(DRAMC0_BASE + 0x0088)) = 0x00090002;
        *((V_UINT32P)(DDRPHY_BASE + 0x0088)) = 0x00090002;
      #elif defined(DDR_1600)
        *((V_UINT32P)(DRAMC0_BASE + 0x0088)) = 0x000A0002;
        *((V_UINT32P)(DDRPHY_BASE + 0x0088)) = 0x000A0002;
      #else
        *((V_UINT32P)(DRAMC0_BASE + 0x0088)) = 0x00060002;
        *((V_UINT32P)(DDRPHY_BASE + 0x0088)) = 0x00060002;
      #endif
    #endif
*/
        *((V_UINT32P)(DRAMC0_BASE + 0x01e4)) = 0x00000001;
        *((V_UINT32P)(DDRPHY_BASE + 0x01e4)) = 0x00000001;

         __asm__ __volatile__ ("dsb" : : : "memory");
        gpt_busy_wait_us(1);//Wait > 1us                        // Edward : 6589 has this delay. Seems no need.

        *((V_UINT32P)(DRAMC0_BASE + 0x01e4)) = 0x00001100;
        *((V_UINT32P)(DDRPHY_BASE + 0x01e4)) = 0x00001100;

        // Edward : add two rank enable here.
        if ( *(V_UINT32P)(EMI_CONA)& 0x20000)  {
            *((V_UINT32P)(DRAMC0_BASE + 0x0110)) = 0x00112391;
            *((V_UINT32P)(DDRPHY_BASE + 0x0110)) = 0x00112391;
        } else {
            *((V_UINT32P)(DRAMC0_BASE + 0x0110)) = 0x00112390;
            *((V_UINT32P)(DDRPHY_BASE + 0x0110)) = 0x00112390;
        }

        *((V_UINT32P)(DRAMC0_BASE + 0x00e4)) = 0x00000001;  //CKEBYCTL
        *((V_UINT32P)(DDRPHY_BASE + 0x00e4)) = 0x00000001;  //CKEBYCTL

        *((V_UINT32P)(DRAMC0_BASE + 0x01ec)) = 0x00000001;    // Edward : Add this to enable dual scheduler according to CC Wen. Should enable this for all DDR type.
        *((V_UINT32P)(DDRPHY_BASE + 0x01ec)) = 0x00000001;

        *((V_UINT32P)(DRAMC0_BASE + 0x0084)) = 0x00000a56;
        *((V_UINT32P)(DDRPHY_BASE + 0x0084)) = 0x00000a56;

        // [27:24] CLK delay. Better to move above
        // [31:28] CS1 delay
		// by JC
        //*((V_UINT32P)(DRAMC0_BASE + 0x000c)) = 0x00000000;
        //*((V_UINT32P)(DDRPHY_BASE + 0x000c)) = 0x00000000;

        // AC timing, by Chiahsien
    #ifdef DDR_1333
      //#ifdef REAL_MEMPLL_MDL
        // Darren
        //*((V_UINT32P)(DRAMC0_BASE + 0x0000)) = 0x559A45d6;
        //*((V_UINT32P)(DDRPHY_BASE + 0x0000)) = 0x559A45d6;
        *((V_UINT32P)(DRAMC0_BASE + 0x0000)) = emi_setting->DRAMC_ACTIM_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x0000)) = emi_setting->DRAMC_ACTIM_VAL;

        //*((V_UINT32P)(DRAMC0_BASE + 0x0004)) = 0xf0048683;
        //*((V_UINT32P)(DDRPHY_BASE + 0x0004)) = 0xf0048683;
        *((V_UINT32P)(DRAMC0_BASE + 0x0004)) = emi_setting->DRAMC_CONF1_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x0004)) = emi_setting->DRAMC_CONF1_VAL;

      #ifndef fcENABLE_FIX_REFCNT_ASYN
        // move to below
        // Derping: R_DMREFTHD(0x08[26:24])='h1
        // Darren
        //*((V_UINT32P)(DRAMC0_BASE + 0x0008)) = 0x0180ae50; // [23] NEW for D123, FREBW_FREN, [17:8] NEW for D123, FREBW_FR
        //*((V_UINT32P)(DDRPHY_BASE + 0x0008)) = 0x0180ae50; // [23] NEW for D123, FREBW_FREN, [17:8] NEW for D123, FREBW_FR
        *((V_UINT32P)(DRAMC0_BASE + 0x0008)) = emi_setting->DRAMC_CONF2_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x0008)) = emi_setting->DRAMC_CONF2_VAL;
      #endif

        // Darren
        //*((V_UINT32P)(DRAMC0_BASE + 0x0044)) = 0xBF0B0401;
        //*((V_UINT32P)(DDRPHY_BASE + 0x0044)) = 0xBF0B0401;
        *((V_UINT32P)(DRAMC0_BASE + 0x0044)) = emi_setting->DRAMC_TEST2_3_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x0044)) = emi_setting->DRAMC_TEST2_3_VAL;

        //*((V_UINT32P)(DRAMC0_BASE + 0x007c)) = 0xc00642d1; // RTP = 2 is for MEMPLL_1333
        //*((V_UINT32P)(DDRPHY_BASE + 0x007c)) = 0xc00642d1; // RTP = 2 is for MEMPLL_1333
        *((V_UINT32P)(DRAMC0_BASE + 0x007c)) = emi_setting->DRAMC_DDR2CTL_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x007c)) = emi_setting->DRAMC_DDR2CTL_VAL;

        //*((V_UINT32P)(DRAMC0_BASE + 0x00fc)) = 0x17800000; // MRW problem by DV (tXP_05T=0)
        //*((V_UINT32P)(DDRPHY_BASE + 0x00fc)) = 0x17800000; // MRW problem by DV (tXP_05T=0)
        *((V_UINT32P)(DRAMC0_BASE + 0x00fc)) = emi_setting->DRAMC_MISCTL0_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x00fc)) = emi_setting->DRAMC_MISCTL0_VAL;

        // Darren
        //*((V_UINT32P)(DRAMC0_BASE + 0x01e8)) = 0x11001330;
        //*((V_UINT32P)(DDRPHY_BASE + 0x01e8)) = 0x11001330;
        *((V_UINT32P)(DRAMC0_BASE + 0x01e8)) = emi_setting->DRAMC_ACTIM1_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x01e8)) = emi_setting->DRAMC_ACTIM1_VAL;

        // Darren
        //*((V_UINT32P)(DRAMC0_BASE + 0x01f8)) = 0x00001420; // MRW problem by DV (tXP_05T=0), OE problem (tRTW_05T=0)
        //*((V_UINT32P)(DDRPHY_BASE + 0x01f8)) = 0x00001420; // MRW problem by DV (tXP_05T=0), OE problem (tRTW_05T=0)
        *((V_UINT32P)(DRAMC0_BASE + 0x01f8)) = emi_setting->DRAMC_ACTIM05T_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x01f8)) = emi_setting->DRAMC_ACTIM05T_VAL;

      #ifdef fcENABLE_FIX_REFCNT_ASYN
        // Derping: R_DMREFTHD(0x08[26:24])='h1
        // Fix refresh enable asynchronous issue
        //*((V_UINT32P)(DRAMC0_BASE + 0x0008)) = 0x1180ae50; // [23] NEW for D123, FREBW_FREN, [17:8] NEW for D123, FREBW_FR
        //*((V_UINT32P)(DDRPHY_BASE + 0x0008)) = 0x1180ae50; // [23] NEW for D123, FREBW_FREN, [17:8] NEW for D123, FREBW_FR
        *((V_UINT32P)(DRAMC0_BASE + 0x0008)) = emi_setting->DRAMC_CONF2_VAL | 0x10000000;
        *((V_UINT32P)(DDRPHY_BASE + 0x0008)) = emi_setting->DRAMC_CONF2_VAL | 0x10000000;
      #endif

        // Darren
        //*((V_UINT32P)(DRAMC0_BASE + 0x01dc)) = 0xd1644742; // [25] DCMEN=0 @ bring up
        //*((V_UINT32P)(DDRPHY_BASE + 0x01dc)) = 0xd1644742; // [25] DCMEN=0 @ bring up
        *((V_UINT32P)(DRAMC0_BASE + 0x01dc)) = emi_setting->DRAMC_PD_CTRL_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x01dc)) = emi_setting->DRAMC_PD_CTRL_VAL;

      #ifdef fcENABLE_FIX_REFCNT_ASYN
        //*((V_UINT32P)(DRAMC0_BASE + 0x0008)) = 0x0180ae50; // [23] NEW for D123, FREBW_FREN, [17:8] NEW for D123, FREBW_FR
        //*((V_UINT32P)(DDRPHY_BASE + 0x0008)) = 0x0180ae50; // [23] NEW for D123, FREBW_FREN, [17:8] NEW for D123, FREBW_FR
        *((V_UINT32P)(DRAMC0_BASE + 0x0008)) = emi_setting->DRAMC_CONF2_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x0008)) = emi_setting->DRAMC_CONF2_VAL;
      #endif

      //#else
      //  *((V_UINT32P)(DRAMC0_BASE + 0x0000)) = 0x779a45f6;
      //  *((V_UINT32P)(DRAMC0_BASE + 0x0004)) = 0xf0048483;
      //  *((V_UINT32P)(DRAMC0_BASE + 0x0008)) = 0x0380ae4f;
      //  *((V_UINT32P)(DRAMC0_BASE + 0x0044)) = 0xbf000401;
      //  #ifdef DENALI_POSIM_MEM_MAX
      //  *((V_UINT32P)(DRAMC0_BASE + 0x007c)) = 0xc00631f1;
      //  *((V_UINT32P)(DDRPHY_BASE + 0x007c)) = 0xc00631f1;
      //  #else
      //  *((V_UINT32P)(DRAMC0_BASE + 0x007c)) = 0xc00631d1;
      //  *((V_UINT32P)(DDRPHY_BASE + 0x007c)) = 0xc00631d1;
      //  #endif
      //  *((V_UINT32P)(DRAMC0_BASE + 0x00fc)) = 0x17800000;
      //  *((V_UINT32P)(DRAMC0_BASE + 0x01e8)) = 0x11001320;
      //  *((V_UINT32P)(DRAMC0_BASE + 0x01f8)) = 0x01001424;
      //  *((V_UINT32P)(DRAMC0_BASE + 0x01dc)) = 0xd3642E42;        //Set TREFCNT = 0x2e    [15:8]
      //#endif
    #else
      #ifdef DDR_1466
        //*((V_UINT32P)(DRAMC0_BASE + 0x0000)) = 0x66aa46f7;
        //*((V_UINT32P)(DDRPHY_BASE + 0x0000)) = 0x66aa46f7;
        *((V_UINT32P)(DRAMC0_BASE + 0x0000)) = emi_setting->DRAMC_ACTIM_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x0000)) = emi_setting->DRAMC_ACTIM_VAL;

        //*((V_UINT32P)(DRAMC0_BASE + 0x0004)) = 0xf00486c3;
        //*((V_UINT32P)(DDRPHY_BASE + 0x0004)) = 0xf00486c3;
        *((V_UINT32P)(DRAMC0_BASE + 0x0004)) = emi_setting->DRAMC_CONF1_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x0004)) = emi_setting->DRAMC_CONF1_VAL;

        #ifndef fcENABLE_FIX_REFCNT_ASYN
        // Derping: R_DMREFTHD(0x08[26:24])='h1
        // move to below
        //*((V_UINT32P)(DRAMC0_BASE + 0x0008)) = 0x0180ae58;
        //*((V_UINT32P)(DDRPHY_BASE + 0x0008)) = 0x0180ae58;
        *((V_UINT32P)(DRAMC0_BASE + 0x0008)) = emi_setting->DRAMC_CONF2_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x0008)) = emi_setting->DRAMC_CONF2_VAL;
        #endif

        //*((V_UINT32P)(DRAMC0_BASE + 0x0044)) = 0xbf020401;
        //*((V_UINT32P)(DDRPHY_BASE + 0x0044)) = 0xbf020401;
        *((V_UINT32P)(DRAMC0_BASE + 0x0044)) = emi_setting->DRAMC_TEST2_3_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x0044)) = emi_setting->DRAMC_TEST2_3_VAL;

        //*((V_UINT32P)(DRAMC0_BASE + 0x007c)) = 0xc00652d1;
        //*((V_UINT32P)(DDRPHY_BASE + 0x007c)) = 0xc00652d1;
        *((V_UINT32P)(DRAMC0_BASE + 0x007c)) = emi_setting->DRAMC_DDR2CTL_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x007c)) = emi_setting->DRAMC_DDR2CTL_VAL;
        
        //*((V_UINT32P)(DRAMC0_BASE + 0x00fc)) = 0x17800000;
        //*((V_UINT32P)(DDRPHY_BASE + 0x00fc)) = 0x17800000;
        *((V_UINT32P)(DRAMC0_BASE + 0x00fc)) = emi_setting->DRAMC_MISCTL0_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x00fc)) = emi_setting->DRAMC_MISCTL0_VAL;

        //*((V_UINT32P)(DRAMC0_BASE + 0x01e8)) = 0x11001640;
        //*((V_UINT32P)(DDRPHY_BASE + 0x01e8)) = 0x11001640;
        *((V_UINT32P)(DRAMC0_BASE + 0x01e8)) = emi_setting->DRAMC_ACTIM1_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x01e8)) = emi_setting->DRAMC_ACTIM1_VAL;

        //*((V_UINT32P)(DRAMC0_BASE + 0x01f8)) = 0x04002201; // OE problem (tRTW_05T=0)
        //*((V_UINT32P)(DDRPHY_BASE + 0x01f8)) = 0x04002201; // OE problem (tRTW_05T=0)
        *((V_UINT32P)(DRAMC0_BASE + 0x01f8)) = emi_setting->DRAMC_ACTIM05T_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x01f8)) = emi_setting->DRAMC_ACTIM05T_VAL;

        #ifdef fcENABLE_FIX_REFCNT_ASYN
        // Derping: R_DMREFTHD(0x08[26:24])='h1
        // Fix refresh enable asynchronous issue
        //*((V_UINT32P)(DRAMC0_BASE + 0x0008)) = 0x1180ae58;
        //*((V_UINT32P)(DDRPHY_BASE + 0x0008)) = 0x1180ae58;
        *((V_UINT32P)(DRAMC0_BASE + 0x0008)) = emi_setting->DRAMC_CONF2_VAL | 0x10000000;
        *((V_UINT32P)(DDRPHY_BASE + 0x0008)) = emi_setting->DRAMC_CONF2_VAL | 0x10000000;
        #endif

        //*((V_UINT32P)(DRAMC0_BASE + 0x01dc)) = 0xd1644e42; // [25] DCMEN=0 @ bring up
        //*((V_UINT32P)(DDRPHY_BASE + 0x01dc)) = 0xd1644e42; // [25] DCMEN=0 @ bring up
        *((V_UINT32P)(DRAMC0_BASE + 0x01dc)) = emi_setting->DRAMC_PD_CTRL_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x01dc)) = emi_setting->DRAMC_PD_CTRL_VAL;

        #ifdef fcENABLE_FIX_REFCNT_ASYN
        //*((V_UINT32P)(DRAMC0_BASE + 0x0008)) = 0x0180ae58;
        //*((V_UINT32P)(DDRPHY_BASE + 0x0008)) = 0x0180ae58;
        *((V_UINT32P)(DRAMC0_BASE + 0x0008)) = emi_setting->DRAMC_CONF2_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x0008)) = emi_setting->DRAMC_CONF2_VAL;
        #endif

      #else // DDR_1066
        //*((V_UINT32P)(DRAMC0_BASE + 0x0000)) = 0x44584493;
        //*((V_UINT32P)(DDRPHY_BASE + 0x0000)) = 0x44584493;
        *((V_UINT32P)(DRAMC0_BASE + 0x0000)) = emi_setting->DRAMC_ACTIM_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x0000)) = emi_setting->DRAMC_ACTIM_VAL;

        //*((V_UINT32P)(DRAMC0_BASE + 0x0004)) = 0xF0048683;
        //*((V_UINT32P)(DDRPHY_BASE + 0x0004)) = 0xF0048683;
        *((V_UINT32P)(DRAMC0_BASE + 0x0004)) = emi_setting->DRAMC_CONF1_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x0004)) = emi_setting->DRAMC_CONF1_VAL;

        #ifndef fcENABLE_FIX_REFCNT_ASYN
        // Derping: R_DMREFTHD(0x08[26:24])='h1
        // move to below
        //*((V_UINT32P)(DRAMC0_BASE + 0x0008)) = 0x0180AE3F;
        //*((V_UINT32P)(DDRPHY_BASE + 0x0008)) = 0x0180AE3F;
        *((V_UINT32P)(DRAMC0_BASE + 0x0008)) = emi_setting->DRAMC_CONF2_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x0008)) = emi_setting->DRAMC_CONF2_VAL;
        #endif

        //*((V_UINT32P)(DRAMC0_BASE + 0x0044)) = 0xBF0D0401;
        //*((V_UINT32P)(DDRPHY_BASE + 0x0044)) = 0xBF0D0401;
        *((V_UINT32P)(DRAMC0_BASE + 0x0044)) = emi_setting->DRAMC_TEST2_3_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x0044)) = emi_setting->DRAMC_TEST2_3_VAL;

        //*((V_UINT32P)(DRAMC0_BASE + 0x007c)) = 0xA00631D1;
        //*((V_UINT32P)(DDRPHY_BASE + 0x007c)) = 0xA00631D1;
        *((V_UINT32P)(DRAMC0_BASE + 0x007c)) = emi_setting->DRAMC_DDR2CTL_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x007c)) = emi_setting->DRAMC_DDR2CTL_VAL;   
        
        //*((V_UINT32P)(DRAMC0_BASE + 0x00fc)) = 0x07800000;
        //*((V_UINT32P)(DDRPHY_BASE + 0x00fc)) = 0x07800000;
        *((V_UINT32P)(DRAMC0_BASE + 0x00fc)) = emi_setting->DRAMC_MISCTL0_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x00fc)) = emi_setting->DRAMC_MISCTL0_VAL;

        //*((V_UINT32P)(DRAMC0_BASE + 0x01e8)) = 0x11000D20;
        //*((V_UINT32P)(DDRPHY_BASE + 0x01e8)) = 0x11000D20;
        *((V_UINT32P)(DRAMC0_BASE + 0x01e8)) = emi_setting->DRAMC_ACTIM1_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x01e8)) = emi_setting->DRAMC_ACTIM1_VAL;

        //*((V_UINT32P)(DRAMC0_BASE + 0x01f8)) = 0x04002600; //OE problem (tRTW_05T=0)
        //*((V_UINT32P)(DDRPHY_BASE + 0x01f8)) = 0x04002600; //OE problem (tRTW_05T=0)
        *((V_UINT32P)(DRAMC0_BASE + 0x01f8)) = emi_setting->DRAMC_ACTIM05T_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x01f8)) = emi_setting->DRAMC_ACTIM05T_VAL;

        #ifdef fcENABLE_FIX_REFCNT_ASYN
        // Derping: R_DMREFTHD(0x08[26:24])='h1
        // Fix refresh enable asynchronous issue
        //*((V_UINT32P)(DRAMC0_BASE + 0x0008)) = 0x1180AE3F;
        //*((V_UINT32P)(DDRPHY_BASE + 0x0008)) = 0x1180AE3F;
        *((V_UINT32P)(DRAMC0_BASE + 0x0008)) = emi_setting->DRAMC_CONF2_VAL | 0x10000000;
        *((V_UINT32P)(DDRPHY_BASE + 0x0008)) = emi_setting->DRAMC_CONF2_VAL | 0x10000000;
        #endif

        //*((V_UINT32P)(DRAMC0_BASE + 0x01dc)) = 0xD1643842; // [25] DCMEN=0 @ bring up
        //*((V_UINT32P)(DDRPHY_BASE + 0x01dc)) = 0xD1643842; // [25] DCMEN=0 @ bring up
        *((V_UINT32P)(DRAMC0_BASE + 0x01dc)) = emi_setting->DRAMC_PD_CTRL_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x01dc)) = emi_setting->DRAMC_PD_CTRL_VAL;

        #ifdef fcENABLE_FIX_REFCNT_ASYN
        //*((V_UINT32P)(DRAMC0_BASE + 0x0008)) = 0x0180AE3F;
        //*((V_UINT32P)(DDRPHY_BASE + 0x0008)) = 0x0180AE3F;
        *((V_UINT32P)(DRAMC0_BASE + 0x0008)) = emi_setting->DRAMC_CONF2_VAL;
        *((V_UINT32P)(DDRPHY_BASE + 0x0008)) = emi_setting->DRAMC_CONF2_VAL;
        #endif
      #endif
    #endif

        //*((V_UINT32P)(DRAMC0_BASE + 0x0000)) = DRAMC_ACTIM_VAL; // Edward : tRAS[3:0] 3h, TRC[7:4] ah, TWTR[11:8] 4h,  TWR[19:16] 8h,
        //*((V_UINT32P)(DDRPHY_BASE + 0x0000)) = DRAMC_ACTIM_VAL;  // Edward : TFAW[23:20] 5h, TRP[27:24] 5h, TRCD[31:28] 5h

        // Edward : AC_TIME_0.5T control (new for 6582)
        //*((V_UINT32P)(DRAMC0_BASE + 0x01f8)) = DRAMC_ACTIM05T_VAL;
        //*((V_UINT32P)(DDRPHY_BASE + 0x01f8)) = DRAMC_ACTIM05T_VAL;   //TWTR_M[26]=1,TFAW[13]=1,TWR_M[10]=1,TRAS[9]=1,TRP[7]=1,TRCD[6]=1,TRC[0]=1

        //*((V_UINT32P)(DRAMC0_BASE + 0x0044)) = DRAMC_TEST2_3_VAL;
        //*((V_UINT32P)(DDRPHY_BASE + 0x0044)) = DRAMC_TEST2_3_VAL;

        //*((V_UINT32P)(DRAMC0_BASE + 0x01e8)) = DRAMC_ACTIM1_VAL;    //LPDDR3EN set to 1
        //*((V_UINT32P)(DDRPHY_BASE + 0x01e8)) = DRAMC_ACTIM1_VAL;    //LPDDR3EN set to 1

        //*((V_UINT32P)(DRAMC0_BASE + 0x0008)) = DRAMC_CONF2_VAL;   // REFCNT[7:0]=41h although not use this. Instead, use FR clock.
        //*((V_UINT32P)(DDRPHY_BASE + 0x0008)) = DRAMC_CONF2_VAL;

        *((V_UINT32P)(DRAMC0_BASE + 0x0010)) = 0x00000000;
        *((V_UINT32P)(DDRPHY_BASE + 0x0010)) = 0x00000000;

        *((V_UINT32P)(DRAMC0_BASE + 0x00f8)) = 0xedcb000f;
        *((V_UINT32P)(DDRPHY_BASE + 0x00f8)) = 0xedcb000f;

        *((V_UINT32P)(DRAMC0_BASE + 0x0020)) = 0x00000000;
        *((V_UINT32P)(DDRPHY_BASE + 0x0020)) = 0x00000000;

        // Edward: The following code will overwrite PLL code setting?
        //*((V_UINT32P)(DRAMC0_BASE + 0x0640)) = 0xffc00033;    //enable DDRPHY dynamic clk gating
        //*((V_UINT32P)(DDRPHY_BASE + 0x0640)) = 0xffc00033;    //enable DDRPHY dynamic clk gating
        // *((V_UINT32P)(DRAMC0_BASE + 0x0640)) |= 0xffc00008;    //enable DDRPHY dynamic clk gating
        //*((V_UINT32P)(DDRPHY_BASE + 0x0640)) |= 0xffc00008;    //enable DDRPHY dynamic clk gating

/*
#ifdef DRAMC_ASYNC
        #ifdef DDRPHY_3PLL_MODE
            //if((*((volatile unsigned int *)(0x10206174))& 0x00000800)!=0)
            //{
            //*((V_UINT32P)(DRAMC0_BASE + 0x0640)) |= (0x8c6e8000<<0);      //enable DDRPHY dynamic clk gating, // disable C_PHY_M_CK dynamic gating
            //*((V_UINT32P)(DDRPHY_BASE + 0x0640)) |= (0x8c6e8000<<0);
            //}
            //else
            //{
            *((V_UINT32P)(DRAMC0_BASE + 0x0640)) |= (0x8c6e8008<<0);      //enable DDRPHY dynamic clk gating, // disable C_PHY_M_CK dynamic gating
            *((V_UINT32P)(DDRPHY_BASE + 0x0640)) |= (0x8c6e8008<<0);
            //}
        #else
            //if((*((volatile unsigned int *)(0x10206174))& 0x00000800)!=0)
            //{
            //*((V_UINT32P)(DRAMC0_BASE + 0x0640)) |= (0xfc6f8000<<0);      //enable DDRPHY dynamic clk gating, // disable C_PHY_M_CK dynamic gating
            //*((V_UINT32P)(DDRPHY_BASE + 0x0640)) |= (0xfc6f8000<<0);
            //}
            //else
            //{
            *((V_UINT32P)(DDRPHY_BASE + 0x0640)) |= (0xfc6f8008<<0);      //enable DDRPHY dynamic clk gating, // disable C_PHY_M_CK dynamic gating
            *((V_UINT32P)(DRAMC0_BASE + 0x0640)) |= (0xfc6f8008<<0);
            //}
        #endif
#else
        #ifdef DDRPHY_3PLL_MODE
            #ifdef DDRPHY_2PLL
                //if((*((volatile unsigned int *)(0x10206174))& 0x00000800)!=0)
                //{
                //  *((V_UINT32P)(DRAMC0_BASE + 0x0640)) |= (0xfc7e8000<<0);
                //  *((V_UINT32P)(DDRPHY_BASE + 0x0640)) |= (0xfc7e8000<<0);
                //}
                //else
                //{
                   *((V_UINT32P)(DRAMC0_BASE + 0x0640)) |= (0xfc7e8008<<0);
                   *((V_UINT32P)(DDRPHY_BASE + 0x0640)) |= (0xfc7e8008<<0);
                //}
            #else
                //if((*((volatile unsigned int *)(0x10206174))& 0x00000800)!=0)
                //{
                //*((V_UINT32P)(DRAMC0_BASE + 0x0640)) |= (0x8c7e8000<<0);      //enable DDRPHY dynamic clk gating, // disable C_PHY_M_CK dynamic gating
                //*((V_UINT32P)(DDRPHY_BASE + 0x0640)) |= (0x8c7e8000<<0);
                //}
                //else
                //{
                *((V_UINT32P)(DRAMC0_BASE + 0x0640)) |= (0x8c7e8008<<0);      //enable DDRPHY dynamic clk gating, // disable C_PHY_M_CK dynamic gating
                *((V_UINT32P)(DDRPHY_BASE + 0x0640)) |= (0x8c7e8008<<0);
                //}
            #endif
        #else
            //if((*((volatile unsigned int *)(0x10206174))& 0x00000800)!=0)
            //	{
            //   *((V_UINT32P)(DRAMC0_BASE + 0x0640)) |= (0xfc7f8000<<0);      //enable DDRPHY dynamic clk gating, // disable C_PHY_M_CK dynamic gating
            //   *((V_UINT32P)(DDRPHY_BASE + 0x0640)) |= (0xfc7f8000<<0);
            //	}
            //	else
            //	{
            *((V_UINT32P)(DRAMC0_BASE + 0x0640)) |= (0xfc7f8008<<0);      //enable DDRPHY dynamic clk gating, // disable C_PHY_M_CK dynamic gating
            *((V_UINT32P)(DDRPHY_BASE + 0x0640)) |= (0xfc7f8008<<0);
            //	}


        #endif
#endif
*/

// CG_EN set to 0 for bring up
// 0x640[3] NA for D123 (for DDR3)
//#ifdef fcENABLE_DDRPHY_DCM_FUNC
//  #ifdef DDRPHY_3PLL_MODE
//    #ifdef DDRPHY_2PLL
//          *((V_UINT32P)(DDRPHY_BASE + 0x0640)) |= (0xffe98000<<0);
//    #else
//        *((V_UINT32P)(DDRPHY_BASE + 0x0640)) |= (0xffe98000<<0);      //enable DDRPHY dynamic clk gating, // disable C_PHY_M_CK dynamic gating
//    #endif
//  #else
//    *((V_UINT32P)(DDRPHY_BASE + 0x0640)) |= (0xffe88000<<0);      //enable DDRPHY dynamic clk gating, // disable C_PHY_M_CK dynamic gating
//  #endif
//#else
  #ifdef DDRPHY_3PLL_MODE
    #ifdef DDRPHY_2PLL
        *((V_UINT32P)(DRAMC0_BASE + 0x0640)) |= (0x00010000<<0);
        *((V_UINT32P)(DDRPHY_BASE + 0x0640)) |= (0x00010000<<0);
    #else
        *((V_UINT32P)(DRAMC0_BASE + 0x0640)) |= (0x00010000<<0);      //enable DDRPHY dynamic clk gating, // disable C_PHY_M_CK dynamic gating
        *((V_UINT32P)(DDRPHY_BASE + 0x0640)) |= (0x00010000<<0);      //enable DDRPHY dynamic clk gating, // disable C_PHY_M_CK dynamic gating
    #endif
  #else
    // [16] R_MEMPLL2_FB_M_CK_CG_EN -> no need for 1PLL mode (feedback clock)
    //*((V_UINT32P)(DRAMC0_BASE + 0x0640)) |= (0x00010000<<0);      //enable DDRPHY dynamic clk gating, // disable C_PHY_M_CK dynamic gating
    //*((V_UINT32P)(DDRPHY_BASE + 0x0640)) |= (0x00010000<<0);      //enable DDRPHY dynamic clk gating, // disable C_PHY_M_CK dynamic gating
  #endif
//#endif

        //*((V_UINT32P)(DRAMC0_BASE + 0x01dc)) = DRAMC_PD_CTRL_VAL;    //enable DDRPHY dynamic clk gating
        //*((V_UINT32P)(DDRPHY_BASE + 0x01dc)) = DRAMC_PD_CTRL_VAL;    //enable DDRPHY dynamic clk gating

         if (emi_setting->EMI_CONA_VAL & 0x20000)  
         {
             *((V_UINT32P)(DRAMC0_BASE + 0x0110)) = 0x00114381;      //dsiable R_DMMRS2RK, this bit enable will impact MRR result // Edward : Modify [14:12] XRTR2W 1->2 [10:8] XRTR2R 1->3
             *((V_UINT32P)(DDRPHY_BASE + 0x0110)) = 0x00114381;      //dsiable R_DMMRS2RK, this bit enable will impact MRR result
         }
         else
         {
             *((V_UINT32P)(DRAMC0_BASE + 0x0110)) = 0x00114380;      //dsiable R_DMMRS2RK, this bit enable will impact MRR result // Edward : Modify [14:12] XRTR2W 1->2 [10:8] XRTR2R 1->3
             *((V_UINT32P)(DDRPHY_BASE + 0x0110)) = 0x00114380;      //dsiable R_DMMRS2RK, this bit enable will impact MRR result
         }

        // Before calibration, set default settings to avoid DLE calibration  fail. The following setting is for 1066Mbps.
        // After bring-up or HQA, please set optimized default here.

        // GW coarse tune 18/12h (fh -> 10010b -> 100b, 10b)
        //*((V_UINT32P)(DRAMC0_BASE + 0x00e0)) = (*((V_UINT32P)(DRAMC0_BASE + 0x00e0)) & 0xf8ffffff) | (0x04<<24);
        //*((V_UINT32P)(DDRPHY_BASE + 0x00e0)) = (*((V_UINT32P)(DDRPHY_BASE + 0x00e0)) & 0xf8ffffff) | (0x04<<24);

        //*((V_UINT32P)(DRAMC0_BASE + 0x0124)) = (*((V_UINT32P)(DRAMC0_BASE + 0x0124)) & 0xffffff00) | 0xaa;
        //*((V_UINT32P)(DDRPHY_BASE + 0x0124)) = (*((V_UINT32P)(DDRPHY_BASE + 0x0124)) & 0xffffff00) | 0xaa;

        // GW fine tune 56 (38h)
        //*((V_UINT32P)(DRAMC0_BASE + 0x0094)) = 0x28282828;
        //*((V_UINT32P)(DDRPHY_BASE + 0x0094)) = 0x28282828;

        //*((V_UINT32P)(DRAMC0_BASE + 0x0098)) = 0x28282828;
        //*((V_UINT32P)(DDRPHY_BASE + 0x0098)) = 0x28282828;

#if defined(MACH_TYPE_MT6735) || defined(MACH_TYPE_MT6737M)
        //  DLE 8
        *((V_UINT32P)(DRAMC0_BASE + 0x007c)) = (*((V_UINT32P)(DRAMC0_BASE + 0x007c)) & 0xFFFFFF8F) | ((8 & 0x07) <<4);
        *((V_UINT32P)(DDRPHY_BASE + 0x007c)) = (*((V_UINT32P)(DDRPHY_BASE + 0x007c)) & 0xFFFFFF8F) |  ((8 & 0x07) <<4);

        *((V_UINT32P)(DRAMC0_BASE + 0x00e4)) = (*((V_UINT32P)(DRAMC0_BASE + 0x00e4)) & 0xFFFFFFEF) | (((8 >> 3) & 0x01) << 4);
        *((V_UINT32P)(DDRPHY_BASE + 0x00e4)) =  (*((V_UINT32P)(DDRPHY_BASE + 0x00e4)) & 0xFFFFFFEF) | (((8 >> 3) & 0x01) << 4);
#elif defined(MACH_TYPE_MT6735M)
        //  DLE 6
        *((V_UINT32P)(DRAMC0_BASE + 0x007c)) = (*((V_UINT32P)(DRAMC0_BASE + 0x007c)) & 0xFFFFFF8F) | ((6 & 0x07) <<4);
        *((V_UINT32P)(DDRPHY_BASE + 0x007c)) = (*((V_UINT32P)(DDRPHY_BASE + 0x007c)) & 0xFFFFFF8F) |  ((6 & 0x07) <<4);

        *((V_UINT32P)(DRAMC0_BASE + 0x00e4)) = (*((V_UINT32P)(DRAMC0_BASE + 0x00e4)) & 0xFFFFFFEF) | (((6 >> 3) & 0x01) << 4);
        *((V_UINT32P)(DDRPHY_BASE + 0x00e4)) =  (*((V_UINT32P)(DDRPHY_BASE + 0x00e4)) & 0xFFFFFFEF) | (((6 >> 3) & 0x01) << 4);
#elif defined(MACH_TYPE_MT6753) || defined(MACH_TYPE_MT6737T)
        //  DLE 10
        *((V_UINT32P)(DRAMC0_BASE + 0x007c)) = (*((V_UINT32P)(DRAMC0_BASE + 0x007c)) & 0xFFFFFF8F) | ((10 & 0x07) <<4);
        *((V_UINT32P)(DDRPHY_BASE + 0x007c)) = (*((V_UINT32P)(DDRPHY_BASE + 0x007c)) & 0xFFFFFF8F) |  ((10 & 0x07) <<4);

        *((V_UINT32P)(DRAMC0_BASE + 0x00e4)) = (*((V_UINT32P)(DRAMC0_BASE + 0x00e4)) & 0xFFFFFFEF) | (((10 >> 3) & 0x01) << 4);
        *((V_UINT32P)(DDRPHY_BASE + 0x00e4)) =  (*((V_UINT32P)(DDRPHY_BASE + 0x00e4)) & 0xFFFFFFEF) | (((10 >> 3) & 0x01) << 4);
#endif

#if 1	// Plus need checks
#if defined(MACH_TYPE_MT6753)
        // RX DQ, DQS input delay
        *((V_UINT32P)(DRAMC0_BASE + 0x0210)) = 0x04060806;
        *((V_UINT32P)(DDRPHY_BASE + 0x0210)) = 0x04060806;
        *((V_UINT32P)(DRAMC0_BASE + 0x0214)) = 0x0A0D0809;
        *((V_UINT32P)(DDRPHY_BASE + 0x0214)) = 0x0A0D0809;
        *((V_UINT32P)(DRAMC0_BASE + 0x0218)) = 0x00050306;
        *((V_UINT32P)(DDRPHY_BASE + 0x0218)) = 0x00050306;
        *((V_UINT32P)(DRAMC0_BASE + 0x021c)) = 0x07090706;
        *((V_UINT32P)(DDRPHY_BASE + 0x021c)) = 0x07090706;
        *((V_UINT32P)(DRAMC0_BASE + 0x0220)) = 0x08070703;
        *((V_UINT32P)(DDRPHY_BASE + 0x0220)) = 0x08070703;
        *((V_UINT32P)(DRAMC0_BASE + 0x0224)) = 0x0B0B0A0C;
        *((V_UINT32P)(DDRPHY_BASE + 0x0224)) = 0x0B0B0A0C;
        *((V_UINT32P)(DRAMC0_BASE + 0x0228)) = 0x01020202;
        *((V_UINT32P)(DDRPHY_BASE + 0x0228)) = 0x01020202;
        *((V_UINT32P)(DRAMC0_BASE + 0x022c)) = 0x04030303;
        *((V_UINT32P)(DDRPHY_BASE + 0x022c)) = 0x04030303;

        *((V_UINT32P)(DRAMC0_BASE + 0x0018)) = 0x13131813;
        *((V_UINT32P)(DDRPHY_BASE + 0x0018)) = 0x13131813;
        *((V_UINT32P)(DRAMC0_BASE + 0x001c)) = 0x13131813;
        *((V_UINT32P)(DDRPHY_BASE + 0x001c)) = 0x13131813;
#elif defined(MACH_TYPE_MT6737T) || defined(MACH_TYPE_MT6737M)
        // RX DQ, DQS input delay
        *((V_UINT32P)(DRAMC0_BASE + 0x0210)) = 0x080B0B07;
        *((V_UINT32P)(DDRPHY_BASE + 0x0210)) = 0x080B0B07;
        *((V_UINT32P)(DRAMC0_BASE + 0x0214)) = 0x0C0F0B0D;
        *((V_UINT32P)(DDRPHY_BASE + 0x0214)) = 0x0C0F0B0D;
        *((V_UINT32P)(DRAMC0_BASE + 0x0218)) = 0x00060405;
        *((V_UINT32P)(DDRPHY_BASE + 0x0218)) = 0x00060405;
        *((V_UINT32P)(DRAMC0_BASE + 0x021c)) = 0x07090709;
        *((V_UINT32P)(DDRPHY_BASE + 0x021c)) = 0x07090709;
        *((V_UINT32P)(DRAMC0_BASE + 0x0220)) = 0x0A0D0A07;
        *((V_UINT32P)(DDRPHY_BASE + 0x0220)) = 0x0A0D0A07;
        *((V_UINT32P)(DRAMC0_BASE + 0x0224)) = 0x0D0F0B0F;
        *((V_UINT32P)(DDRPHY_BASE + 0x0224)) = 0x0D0F0B0F;
        *((V_UINT32P)(DRAMC0_BASE + 0x0228)) = 0x00020101;
        *((V_UINT32P)(DDRPHY_BASE + 0x0228)) = 0x00020101;
        *((V_UINT32P)(DRAMC0_BASE + 0x022c)) = 0x03020206;
        *((V_UINT32P)(DDRPHY_BASE + 0x022c)) = 0x03020206;

        *((V_UINT32P)(DRAMC0_BASE + 0x0018)) = 0x15191B18;
        *((V_UINT32P)(DDRPHY_BASE + 0x0018)) = 0x15191B18;
        *((V_UINT32P)(DRAMC0_BASE + 0x001c)) = 0x15191B18;
        *((V_UINT32P)(DDRPHY_BASE + 0x001c)) = 0x15191B18;
#else
        // RX DQ, DQS input delay
        *((V_UINT32P)(DRAMC0_BASE + 0x0210)) = 0x80B0B07;
        *((V_UINT32P)(DDRPHY_BASE + 0x0210)) = 0x80B0B07;
        *((V_UINT32P)(DRAMC0_BASE + 0x0214)) = 0xC0F0B0D;
        *((V_UINT32P)(DDRPHY_BASE + 0x0214)) = 0xC0F0B0D;
        *((V_UINT32P)(DRAMC0_BASE + 0x0218)) = 0x60405;
        *((V_UINT32P)(DDRPHY_BASE + 0x0218)) = 0x60405;
        *((V_UINT32P)(DRAMC0_BASE + 0x021c)) = 0x7090709;
        *((V_UINT32P)(DDRPHY_BASE + 0x021c)) = 0x7090709;
        *((V_UINT32P)(DRAMC0_BASE + 0x0220)) = 0xA0D0A07;
        *((V_UINT32P)(DDRPHY_BASE + 0x0220)) = 0xA0D0A07;
        *((V_UINT32P)(DRAMC0_BASE + 0x0224)) = 0xD0F0B0F;
        *((V_UINT32P)(DDRPHY_BASE + 0x0224)) = 0xD0F0B0F;
        *((V_UINT32P)(DRAMC0_BASE + 0x0228)) = 0x20101;
        *((V_UINT32P)(DDRPHY_BASE + 0x0228)) = 0x20101;
        *((V_UINT32P)(DRAMC0_BASE + 0x022c)) = 0x3020206;
        *((V_UINT32P)(DDRPHY_BASE + 0x022c)) = 0x3020206;

        *((V_UINT32P)(DRAMC0_BASE + 0x0018)) = 0x15191B18;
        *((V_UINT32P)(DDRPHY_BASE + 0x0018)) = 0x15191B18;
        *((V_UINT32P)(DRAMC0_BASE + 0x001c)) = 0x15191B18;
        *((V_UINT32P)(DDRPHY_BASE + 0x001c)) = 0x15191B18;

#endif

        // TX DQ, DQS output delay
        
        //*((V_UINT32P)(DRAMC0_BASE + 0x0014)) = 0x8888;   // DQS output delay
        //*((V_UINT32P)(DDRPHY_BASE + 0x0014)) = 0x8888;

        // for D-3 test
    #ifdef fcD3_TX_DQ_CHANGE_DEFAULT
        *((V_UINT32P)(DRAMC0_BASE + 0x0010)) = 0x8888;   // DQM output delay
        *((V_UINT32P)(DDRPHY_BASE + 0x0010)) = 0x8888;

        *((V_UINT32P)(DRAMC0_BASE + 0x0200)) = 0x88888888;   // DQ
        *((V_UINT32P)(DDRPHY_BASE + 0x0200)) = 0x88888888;
        *((V_UINT32P)(DRAMC0_BASE + 0x0204)) = 0x88888888;
        *((V_UINT32P)(DDRPHY_BASE + 0x0204)) = 0x88888888;
        *((V_UINT32P)(DRAMC0_BASE + 0x0208)) = 0x99998888;
        *((V_UINT32P)(DDRPHY_BASE + 0x0208)) = 0x99998888;
        *((V_UINT32P)(DRAMC0_BASE + 0x020c)) = 0x99998888;
        *((V_UINT32P)(DDRPHY_BASE + 0x020c)) = 0x99998888;
    #endif
        
#endif

#ifdef MEMPLL_CLK_733
  // cross rank timing, TBD
  DRAMC_WRITE_REG(0x00114281,0x110); //1466
#endif

//#ifdef MEMPLL_CLK_733
//  DRAMC_WRITE_REG(0x58ea0916<<1,0x624);
//#endif

// DQSGATED fine tune for dummy pad input
// to let gating coarse tune be advanced for DVFS more margin
#ifdef TX_DELAY_OVERCLOCK
   DRAMC_WRITE_REG(DRAMC_READ_REG(0x124)|0xf<<8,0x124);
#endif

  // Ungate dram transaction blockage mechanism after Dram Init.
  // If bit 10 of EMI_CONM is not 1, any transaction to EMI will be ignored.
  // TBD, double check EMI initial settings (EMI_BASE+0x060)
  //*EMI_CONM |= (1<<10);

//         DRAMC_WRITE_REG(0x54E94731<<1,0x624); //1400
//          DRAMC_WRITE_REG(0x58ea0916<<1,0x624); //1466
//           DRAMC_WRITE_REG(0x5cfa51cc<<1,0x624); //1533
//           DRAMC_WRITE_REG(0x610a9a82<<1,0x624); //1600
  #endif //!COMBO_LPDDR3
}

static int mt_get_mdl_number (void)
{
    static int found = 0;
    static int mdl_number = -1;
    int i;
    int j;
    int has_emmc_nand = 0;
    int discrete_dram_num = 0;
    int mcp_dram_num = 0;

    unsigned int mode_reg_5 = 0;
    unsigned int dram_type;
    unsigned int index = 0;
#ifdef HARDCODE_DRAM_SETTING
#ifdef FLASH_TOOL_PRELOADER
   //print("[Check] Flash tool EMI Setting determination\n");
   num_of_emi_records = NUM_EMI_RECORD_HARD_CODE ;
   platform_get_mcp_id (id, emmc_nand_id_len, &fw_id_len);
   if ((memcmp(id, emi_settings[1].ID, emi_settings[1].id_length) == 0)) {
       index = 1;
   } else if ((memcmp(id, emi_settings[2].ID, emi_settings[2].id_length) == 0)) {
       index = 2;
   } else {
       index = 0;
   }

   printf("[Check] Flash tool mt_get_mdl_number 0x%x\n", index);
   return index;
#else

   num_of_emi_records = NUM_EMI_RECORD_HARD_CODE ;

#ifdef H9TQ64A8GTMCUR_KUM
index = 0;
#endif

#if defined(KMQ8X000SA_B414) || defined(H9TQ17ABJTMCUR_KUM)
index = 1;
#endif

#ifdef KMQ7X000SA_B315
index = 2;
#endif

//platform_get_mcp_id (id, emmc_nand_id_len,&fw_id_len);
printf("[Check] Hardcode mt_get_mdl_number 0x%x\n", index);
return index;
#endif	// end #ifdef FLASH_TOOL_PRELOADER

#else // from #ifdef HARDCODE_DRAM_SETTING

    if (!(found))
    {
        int result=0;
        platform_get_mcp_id (id, emmc_nand_id_len,&fw_id_len);
        for (i = 0 ; i < num_of_emi_records; i++)
        {
            if (((emi_settings[i].type & 0x0F00) == 0x0000) || ((emi_settings[i].type & 0x0F00) == 0x0F00))
            {
                discrete_dram_num ++;
            }
            else
            {
                mcp_dram_num ++;
            }
        }

        /*If the number >=2  &&
         * one of them is discrete DRAM
         * enable combo discrete dram parse flow
         * */
        if ((discrete_dram_num > 0) && (num_of_emi_records >= 2))
        {
            /* if we enable combo discrete dram
             * check all dram are all same type and not DDR3
             * */
            enable_combo_dis = 1;
            dram_type = emi_settings[0].type & 0x000F;
            for (i = 0 ; i < num_of_emi_records; i++)
            {
                if (dram_type != (emi_settings[i].type & 0x000F))
                {
                    printf("[EMI] Combo discrete dram only support when combo lists are all same dram type.");
                    ASSERT(0);
                }
                if ((emi_settings[i].type & 0x000F) == TYPE_PCDDR3)
                {
                    // has PCDDR3, disable combo discrete drame, no need to check others setting
                    enable_combo_dis = 0;
                    break;
                }
                dram_type = emi_settings[i].type & 0x000F;
            }
        }
        printf("[EMI] mcp_dram_num:%d,discrete_dram_num:%d,enable_combo_dis:%d\r\n",mcp_dram_num,discrete_dram_num,enable_combo_dis);
        /*
         *
         * 0. if there is only one discrete dram, use index=0 emi setting and boot it.
         * */
        if ((0 == mcp_dram_num) && (1 == discrete_dram_num))
        {
            mdl_number = 0;
            found = 1;
            return mdl_number;
        }


        /* 1.
         * if there is MCP dram in the list, we try to find emi setting by emmc ID
         * */
        if (mcp_dram_num > 0)
        {
        	result = platform_get_mcp_id (id, emmc_nand_id_len,&fw_id_len);

	        for (i = 0; i < num_of_emi_records; i++)
	        {
	            if (emi_settings[i].type != 0)
	            {
	                if ((emi_settings[i].type & 0x0F00) != 0x0000)
	                {
	                    if (result == 0)
	                    {   /* valid ID */
	
	                        if ((emi_settings[i].type & 0x0F00) == 0x100)
	                        {
	                            /* NAND */
	                            if (memcmp(id, emi_settings[i].ID, emi_settings[i].id_length) == 0){
	                                memset(id + emi_settings[i].id_length, 0, sizeof(id) - emi_settings[i].id_length);
	                                mdl_number = i;
	                                found = 1;
	                                break; /* found */
	                            }
	                        }
	                        else
	                        {
	
	                            /* eMMC */
	                            if (memcmp(id, emi_settings[i].ID, emi_settings[i].id_length) == 0)
	                            {
#if 0
	                                printf("fw id len:%d\n",emi_settings[i].fw_id_length);
	                                if (emi_settings[i].fw_id_length > 0)
	                                {
	                                    char fw_id[6];
	                                    memset(fw_id, 0, sizeof(fw_id));
	                                    memcpy(fw_id,id+emmc_nand_id_len,fw_id_len);
	                                    for (j = 0; j < fw_id_len;j ++){
	                                        printf("0x%x, 0x%x ",fw_id[j],emi_settings[i].fw_id[j]);
	                                    }
	                                    if(memcmp(fw_id,emi_settings[i].fw_id,fw_id_len) == 0)
	                                    {
	                                        mdl_number = i;
	                                        found = 1;
	                                        break; /* found */
	                                    }
	                                    else
	                                    {
	                                        printf("[EMI] fw id match failed\n");
	                                    }
	                                }
	                                else
	                                {
	                                    mdl_number = i;
	                                    found = 1;
	                                    break; /* found */
	                                }
#else
	                                    mdl_number = i;
	                                    found = 1;
	                                    break; /* found */
#endif
	                            }
	                            else{
	                                  printf("[EMI] index(%d) emmc id match failed\n",i);
	                            }
	
	                        }
	                    }
	                }
	            }
	        }
	}
#if 1
        /* 2. find emi setting by MODE register 5
         * */
        // if we have found the index from by eMMC ID checking, we can boot android by the setting
        // if not, we try by vendor ID
        if ((0 == found) && (1 == enable_combo_dis))
        {
            unsigned int manu_id = 0;
            /* DDR reserve mode no need to enable memory & test */
            if((g_ddr_reserve_enable==1) && (g_ddr_reserve_success==1))
            {
                unsigned int emi_cona;
                emi_cona = *(volatile unsigned *)(EMI_CONA);
                printf("[DDR Reserve mode] EMI dummy read CONA = 0x%x in mt_get_mdl_number()\n", emi_cona);

                /*
                 * NOTE:
                 * in DDR reserve mode, the DRAM access abnormal after DRAM_MRR(), so
                 * we use (DRAMC0_BASE + 0x100)[7:0] to store DRAM's vendor ID, skip DRAM_MRR() after reboot;
                 * (DRAMC0_BASE + 0x100)[7:0] is the dummy register.
                 * */
                printf("[EMI] DRAMC0_BASE + 0x100 : %x\n\r", *((V_UINT32P)(DRAMC0_BASE + 0x0100)));
                manu_id = *((V_UINT32P)(DRAMC0_BASE + 0x0100)) & 0xff;
	    }
	    else {
	        EMI_SETTINGS *emi_set;
	        //print_DBG_info();
	        //print("-->%x,%x,%x\n",emi_set->DRAMC_ACTIM_VAL,emi_set->sub_version,emi_set->fw_id_length);
	        //print("-->%x,%x,%x\n",emi_setting_default.DRAMC_ACTIM_VAL,emi_setting_default.sub_version,emi_setting_default.fw_id_length);
	        dram_type = mt_get_dram_type_for_dis();
	        if (TYPE_LPDDR2 == dram_type)
	        {
	            printf("[EMI] LPDDR2 discrete dram init\r\n");
	            emi_set = &emi_setting_default_lpddr2;
	            #ifdef REXTDN_ENABLE
	            ett_rextdn_sw_calibration();
	            #endif
	            init_lpddr2(emi_set);
	        }
	        else if (TYPE_LPDDR3 == dram_type)
	        {
	            printf("[EMI] LPDDR3 discrete dram init\r\n");
	            emi_set = &emi_setting_default_lpddr3;
	            #ifdef REXTDN_ENABLE
	            ett_rextdn_sw_calibration();
	            #endif
	            init_lpddr3(emi_set);
	        }
#ifdef DUMP_DRAMC_REGS
	        printf("[EMI] Discrete dram first init timing\n");
	        print_DBG_info();
#endif
	        if (dramc_calib(emi_set) < 0) {
		    printf("[EMI] Default EMI setting DRAMC calibration failed\n\r");
                } else {
	            printf("[EMI] Default EMI setting DRAMC calibration passed\n\r");
	        }	
                manu_id = DRAM_MRR(0x5);
                /*
                 * NOTE: 
                 * in DDR reserve mode, the DRAM access abnormal after DRAM_MRR(), so
                 * we use (DRAMC0_BASE + 0x100)[7:0] to store DRAM's vendor ID, skip DRAM_MRR() after reboot;
                 * (DRAMC0_BASE + 0x100)[7:0] is the dummy register.
                 * */
                *((V_UINT32P)(DRAMC0_BASE + 0x0100)) &= (~0xff);
                *((V_UINT32P)(DRAMC0_BASE + 0x0100)) |= (manu_id & 0xff);
                printf("[EMI] DRAMC0_BASE + 0x100 : %x\n\r", *((V_UINT32P)(DRAMC0_BASE + 0x0100)));
	    }
            printf("[EMI]MR5:%x\n",manu_id);
            //try to find discrete dram by DDR2_MODE_REG5(vendor ID)
            for (i = 0; i < num_of_emi_records; i++)
            {
                if (TYPE_LPDDR2 == dram_type)
                    mode_reg_5 = emi_settings[i].LPDDR2_MODE_REG_5;
                else if (TYPE_LPDDR3 == dram_type)
                    mode_reg_5 = emi_settings[i].LPDDR3_MODE_REG_5;
                printf("emi_settings[i].MODE_REG_5:%x,emi_settings[i].type:%x\n",mode_reg_5,emi_settings[i].type);
                //only check discrete dram type
                if (((emi_settings[i].type & 0x0F00) == 0x0000) || ((emi_settings[i].type & 0x0F00) == 0x0F00))
                {
                    //support for compol discrete dram
                    if ((mode_reg_5 == manu_id) )
                    {
                        mdl_number = i;
                        found = 1;
                        break;
                    }
                }
            }
        } // end if ((0 == found) && (1 == enable_combo_dis))

#endif	// end #if 1
        printf("found:%d,i:%d,mdl_number:%d\n",found,i,mdl_number);
    }

    return mdl_number;
#endif
}


int mt_get_dram_type (void)
{

    int n;
    unsigned int dram_type;

    // pll not ready for discrete DRAM
    if (((emi_settings[0].type & 0x0F00) == 0x0000) || ((emi_settings[0].type & 0x0F00) == 0x0F00)) {
        dram_type = mt_get_dram_type_for_dis();
        return dram_type;
    } else {
        /* if combo discrete is enabled, the dram_type is LPDDR2 or LPDDR4, depend on the emi_setting list*/
        if (1 == enable_combo_dis)
            return mt_get_dram_type_for_dis();

        n = mt_get_mdl_number ();

        if (n < 0  || n >= num_of_emi_records)
        {
            return 0; /* invalid */
        }

        return (emi_settings[n].type & 0xF);
    }
}

int get_dram_rank_nr (void)
{

    int index;
    int emi_cona;
    index = mt_get_mdl_number ();
    if (index < 0 || index >=  num_of_emi_records)
    {
        return -1;
    }

    emi_cona = emi_settings[index].EMI_CONA_VAL;

#if CFG_FPGA_PLATFORM
    return 1;
#endif

    return (emi_cona & 0x20000) ? 2 : 1;

}

/*
 * setup block correctly, we should hander both 4GB mode and
 * non-4GB mode.
 */
void get_orig_dram_rank_info(dram_info_t *orig_dram_info)
{
	int i, j;
	u64 base = DRAM_BASE;
	unsigned int rank_size[4] = {0};

	i = get_dram_rank_nr();
	orig_dram_info->rank_num = (i > 0) ? i : 0;
	get_dram_rank_size(rank_size);
	orig_dram_info->rank_info[0].start = base;
	for (i = 0; i < orig_dram_info->rank_num; i++) {

		orig_dram_info->rank_info[i].size = (u64)rank_size[i];

		if (i > 0) {
			orig_dram_info->rank_info[i].start =
				orig_dram_info->rank_info[i - 1].start +
				orig_dram_info->rank_info[i - 1].size;
		}
		printf("orig_dram_info[%d] start: 0x%llx, size: 0x%llx\n",
				i, orig_dram_info->rank_info[i].start,
				orig_dram_info->rank_info[i].size);
	}
	
	for(j=i; j<4; j++)
	{
	  		orig_dram_info->rank_info[j].start = 0;
	  		orig_dram_info->rank_info[j].size = 0;	
	}
}

/* This function returns available dram size for normal world. *
 * If it's called before TEE has been loaded, it returns real  *
 * total dram size. Use it with care!!                         */
int get_dram_rank_size (int dram_rank_size[])
{
 int index,/* bits,*/ rank_nr, i;
    //int emi_cona;
#if 1
    index = mt_get_mdl_number ();

    if (index < 0 || index >=  num_of_emi_records)
    {
        return;
    }

    rank_nr = get_dram_rank_nr();

    for(i = 0; i < rank_nr; i++){
        dram_rank_size[i] = emi_settings[index].DRAM_RANK_SIZE[i];

        printf("%d:dram_rank_size:%x\n",i,dram_rank_size[i]);
    }

    return;
#endif
}

int check_dram_CONA_value (int idx)
{
    int rank_nr, i, emi_cona;

    if (idx < 0 || idx >=  num_of_emi_records)
    {
        printf("check_dram_CONA_value index incorrext\n");
        return -1;
    }

    emi_cona = emi_settings[idx].EMI_CONA_VAL;
    rank_nr = (emi_cona & 0x20000) ? 2 : 1;

    if (rank_nr == 1)
        return;

    if ((emi_settings[idx].DRAM_RANK_SIZE[0] == 0x80000000) &&(emi_settings[idx].DRAM_RANK_SIZE[1] == 0x40000000)) { //3GB
        if (emi_settings[idx].EMI_CONA_VAL != 0x2A062) {
            printf("[DRAMC] 3GB CONA value incorrect != 0x2A062\n");
    	    ASSERT(0);
        }
    } else if ((emi_settings[idx].DRAM_RANK_SIZE[0] == 0x40000000) &&(emi_settings[idx].DRAM_RANK_SIZE[1] == 0x20000000)) { //1.5GB
        if (emi_settings[idx].EMI_CONA_VAL != 0x26052) {
            printf("[DRAMC] 1.5GB CONA value incorrect != 0x26052\n");
    	    ASSERT(0);
        }
    }

    return;
}

#ifdef DUMP_DRAMC_REGS
void print_DBG_info(){
    unsigned int addr = 0x0;
        printf("=====================DBG=====================\n");
    for(addr = 0x0; addr < 0x650; addr +=4){
    printf("addr:%x, value:%x\n",addr, DRAMC_READ_REG(addr));
}
    printf("=============================================\n");
}
#endif

#ifdef ENABLE_DFS
int DFS_ability_detection(void)
{
    int rank0_coarse, rank0_fine;
    int rank1_coarse, rank1_fine;
    int n;
    int DDR_TYPE, rank_nr;
#if defined(MACH_TYPE_MT6753) || defined(MACH_TYPE_MT6737T)
    unsigned int spbin = ((seclib_get_devinfo_with_index(5) & (1<<20)) && \
			(seclib_get_devinfo_with_index(5) & (1<<21)) && \
			(seclib_get_devinfo_with_index(5) & (1<<22))) ? 1 : 0; //MT6753 or MT6753T
    unsigned int ddr1466 = (seclib_get_devinfo_with_index(15) & (1<<8)) ? 1 : 0; //DDR1466 or DDR1600
#endif

    DDR_TYPE = DDR_type;
    rank0_coarse = atoi(opt_gw_coarse_value0);
    rank0_fine = atoi(opt_gw_fine_value0);
    rank1_coarse = atoi(opt_gw_coarse_value1);
    rank1_fine = atoi(opt_gw_fine_value1);
    rank_nr = get_dram_rank_nr();

    if (rank_nr == 1) {
        printf("[DRAM] DFS rank0 coarse fine : %d %d\n", rank0_coarse, rank0_fine);
    } else {
        printf("[DRAM] DFS rank0 coarse fine : %d %d, rank1 coarse fine :%d %d\n", rank0_coarse, rank0_fine, rank1_coarse, rank1_fine);
    }

if (DDR_TYPE == TYPE_LPDDR3) {
    if (rank_nr == 1) {
#if defined(MACH_TYPE_MT6753) || defined(MACH_TYPE_MT6737T)
          if ((spbin == 1) && (ddr1466 == 0)) {// DDR1600
	    if (rank0_coarse <= 24) {
	        printf("D3T DFS could be enable\n");
	        *((V_UINT32P)(DRAMC0_BASE + 0xf4)) = *((V_UINT32P)(DRAMC0_BASE + 0xf4)) | (0x1 << 15);
	        *((V_UINT32P)(DDRPHY_BASE + 0xf4)) = *((V_UINT32P)(DDRPHY_BASE + 0xf4)) | (0x1 << 15);
	        return 1;
            } else {
		printf("D3T DFS could not be enable\n");
		*((V_UINT32P)(DRAMC0_BASE + 0xf4)) = *((V_UINT32P)(DRAMC0_BASE + 0xf4)) & ~(0x1 << 15);
		*((V_UINT32P)(DDRPHY_BASE + 0xf4)) = *((V_UINT32P)(DDRPHY_BASE + 0xf4)) & ~(0x1 << 15);
		return 0;
            }
          } else {
	    if (((rank0_coarse == 25) && (rank0_fine >= 56)) \
               || (rank0_coarse <= 24)) {
	        printf("D3 DFS could be enable\n");
	        *((V_UINT32P)(DRAMC0_BASE + 0xf4)) = *((V_UINT32P)(DRAMC0_BASE + 0xf4)) | (0x1 << 15);
	        *((V_UINT32P)(DDRPHY_BASE + 0xf4)) = *((V_UINT32P)(DDRPHY_BASE + 0xf4)) | (0x1 << 15);
	        return 1;
            } else {
		printf("D3 DFS could not be enable\n");
		*((V_UINT32P)(DRAMC0_BASE + 0xf4)) = *((V_UINT32P)(DRAMC0_BASE + 0xf4)) & ~(0x1 << 15);
		*((V_UINT32P)(DDRPHY_BASE + 0xf4)) = *((V_UINT32P)(DDRPHY_BASE + 0xf4)) & ~(0x1 << 15);
		return 0;
            }
          }
#else
#if defined(MACH_TYPE_MT6735M)
	    if (rank0_coarse <= 15) {
#else //MT6735 and MT6737M
	    if (((rank0_coarse == 20) && (rank0_fine >= 56)) || (rank0_coarse < 20)) {
#endif
	        printf("DFS could be enable\n");
	        *((V_UINT32P)(DRAMC0_BASE + 0xf4)) = *((V_UINT32P)(DRAMC0_BASE + 0xf4)) | (0x1 << 15);
	        *((V_UINT32P)(DDRPHY_BASE + 0xf4)) = *((V_UINT32P)(DDRPHY_BASE + 0xf4)) | (0x1 << 15);
	        return 1;
            } else {
		printf("DFS could not be enable\n");
		*((V_UINT32P)(DRAMC0_BASE + 0xf4)) = *((V_UINT32P)(DRAMC0_BASE + 0xf4)) & ~(0x1 << 15);
		*((V_UINT32P)(DDRPHY_BASE + 0xf4)) = *((V_UINT32P)(DDRPHY_BASE + 0xf4)) & ~(0x1 << 15);
		return 0;
            }
#endif
    } else { // dual rank
#if defined(MACH_TYPE_MT6753) || defined(MACH_TYPE_MT6737T)
          if ((spbin == 1) && (ddr1466 == 0)) {// DDR1600
	    if (((rank0_coarse <= 24) && (rank1_coarse <= 24))) {
	        printf("D3T DFS could be enable\n");
	        *((V_UINT32P)(DRAMC0_BASE + 0xf4)) = *((V_UINT32P)(DRAMC0_BASE + 0xf4)) | (0x1 << 15);
	        *((V_UINT32P)(DDRPHY_BASE + 0xf4)) = *((V_UINT32P)(DDRPHY_BASE + 0xf4)) | (0x1 << 15);
	        return 1;
            } else {
		printf("D3T DFS could not be enable\n");
		*((V_UINT32P)(DRAMC0_BASE + 0xf4)) = *((V_UINT32P)(DRAMC0_BASE + 0xf4)) & ~(0x1 << 15);
		*((V_UINT32P)(DDRPHY_BASE + 0xf4)) = *((V_UINT32P)(DDRPHY_BASE + 0xf4)) & ~(0x1 << 15);
		return 0;
            }
          } else {
	    if (((rank0_coarse == 25) && (rank1_coarse == 25) && (rank0_fine >= 56) && (rank1_fine >= 56)) \
               || ((rank0_coarse <= 24) && (rank1_coarse <= 24))) {
	        printf("D3 DFS could be enable\n");
	        *((V_UINT32P)(DRAMC0_BASE + 0xf4)) = *((V_UINT32P)(DRAMC0_BASE + 0xf4)) | (0x1 << 15);
	        *((V_UINT32P)(DDRPHY_BASE + 0xf4)) = *((V_UINT32P)(DDRPHY_BASE + 0xf4)) | (0x1 << 15);
	        return 1;
            } else {
		printf("D3 DFS could not be enable\n");
		*((V_UINT32P)(DRAMC0_BASE + 0xf4)) = *((V_UINT32P)(DRAMC0_BASE + 0xf4)) & ~(0x1 << 15);
		*((V_UINT32P)(DDRPHY_BASE + 0xf4)) = *((V_UINT32P)(DDRPHY_BASE + 0xf4)) & ~(0x1 << 15);
		return 0;
            }
          }
#else
#if defined(MACH_TYPE_MT6735M)
	    if ((rank0_coarse <= 15) && (rank1_coarse <= 15)) {
#else // MT6735 and MT6737M
	    if (((rank0_coarse == 20) && (rank1_coarse == 20) && (rank0_fine >= 56) && (rank1_fine >= 56)) \
               || ((rank0_coarse < 20) && (rank1_coarse < 20))) {
#endif
	        printf("DFS could be enable\n");
	        *((V_UINT32P)(DRAMC0_BASE + 0xf4)) = *((V_UINT32P)(DRAMC0_BASE + 0xf4)) | (0x1 << 15);
	        *((V_UINT32P)(DDRPHY_BASE + 0xf4)) = *((V_UINT32P)(DDRPHY_BASE + 0xf4)) | (0x1 << 15);
	        return 1;
            } else {
		printf("DFS could not be enable\n");
		*((V_UINT32P)(DRAMC0_BASE + 0xf4)) = *((V_UINT32P)(DRAMC0_BASE + 0xf4)) & ~(0x1 << 15);
		*((V_UINT32P)(DDRPHY_BASE + 0xf4)) = *((V_UINT32P)(DDRPHY_BASE + 0xf4)) & ~(0x1 << 15);
		return 0;
            }
#endif
    } // end if (rank_nr == 1)
} else if (DDR_TYPE == TYPE_LPDDR2) {
#if defined(MACH_TYPE_MT6753)
	printf("LPDDR2 DFS always disable\n");
	*((V_UINT32P)(DRAMC0_BASE + 0xf4)) = *((V_UINT32P)(DRAMC0_BASE + 0xf4)) & ~(0x1 << 15);
	*((V_UINT32P)(DDRPHY_BASE + 0xf4)) = *((V_UINT32P)(DDRPHY_BASE + 0xf4)) & ~(0x1 << 15);
#elif defined(MACH_TYPE_MT6735M)
        if (rank_nr == 1) {
	    if (rank0_coarse <= 15) {
		printf("DFS could be enable\n");
		*((V_UINT32P)(DRAMC0_BASE + 0xf4)) = *((V_UINT32P)(DRAMC0_BASE + 0xf4)) | (0x1 << 15);
		*((V_UINT32P)(DDRPHY_BASE + 0xf4)) = *((V_UINT32P)(DDRPHY_BASE + 0xf4)) | (0x1 << 15);
		return 1;
	    } else {
		printf("DFS could not be enable\n");
		*((V_UINT32P)(DRAMC0_BASE + 0xf4)) = *((V_UINT32P)(DRAMC0_BASE + 0xf4)) & ~(0x1 << 15);
		*((V_UINT32P)(DDRPHY_BASE + 0xf4)) = *((V_UINT32P)(DDRPHY_BASE + 0xf4)) & ~(0x1 << 15);
		return 0;
	    }
	} else {
	    if ((rank0_coarse <= 15) && (rank1_coarse <= 15)) {
		printf("DFS could be enable\n");
		*((V_UINT32P)(DRAMC0_BASE + 0xf4)) = *((V_UINT32P)(DRAMC0_BASE + 0xf4)) | (0x1 << 15);
		*((V_UINT32P)(DDRPHY_BASE + 0xf4)) = *((V_UINT32P)(DDRPHY_BASE + 0xf4)) | (0x1 << 15);
		return 1;
	    } else {
		printf("DFS could not be enable\n");
		*((V_UINT32P)(DRAMC0_BASE + 0xf4)) = *((V_UINT32P)(DRAMC0_BASE + 0xf4)) & ~(0x1 << 15);
		*((V_UINT32P)(DDRPHY_BASE + 0xf4)) = *((V_UINT32P)(DDRPHY_BASE + 0xf4)) & ~(0x1 << 15);
		return 0;
	    }
	}
#else
	printf("LPDDR2 DFS always enable\n");
	*((V_UINT32P)(DRAMC0_BASE + 0xf4)) = *((V_UINT32P)(DRAMC0_BASE + 0xf4)) | (0x1 << 15);
	*((V_UINT32P)(DDRPHY_BASE + 0xf4)) = *((V_UINT32P)(DDRPHY_BASE + 0xf4)) | (0x1 << 15);
#endif
} else {
	printf("Unknown DRAM Type\n");
	return 0;
}

    return 0;
}
#endif

#ifdef pmic_HQA_TCs
void pmic_voltage_read(unsigned int nAdjust)
{
    int ret_val = 0;
    unsigned int OldVcore1 = 0;
    unsigned int OldVmem = 0, OldVmem1 = 0;

    printf("[PMIC]pmic_voltage_read : \r\n");

    ret_val = pmic_read_interface(MT6328_VCORE1_CON11, &OldVcore1, 0x7F, 0);
    ret_val = pmic_read_interface(MT6328_SLDO_ANA_CON0, &OldVmem, 0x7F, 0);
    ret_val = pmic_read_interface(MT6328_SLDO_ANA_CON1, &OldVmem1, 0x0F, 8);

    printf("[Vcore] MT6328_VCORE1_CON11=0x%x,\r\n[Vmem] MT6328_SLDO_ANA_CON0/1=0x%x 0x%x\r\n", OldVcore1, OldVmem, OldVmem1);
}

void pmic_force_PWM_Mode(void)
{
    unsigned int vcore1_pwm = 0;

    pmic_config_interface(MT6328_VCORE_ANA_CON1, 0x01, 0x01, 11);
    pmic_read_interface(MT6328_VCORE_ANA_CON1, &vcore1_pwm, 0x1FFF, 0);
    printf("[Vcore] MT6328_VCORE_ANA_CON1 = 0x%x\n", vcore1_pwm);
}

void pmic_Vcore_adjust(int nAdjust)
{
    unsigned int vcore_idx = 0;

    switch (nAdjust) {
        case 0:	// HV 1280MHz 
        	//pmic_config_interface(MT6328_VCORE1_CON11, 0x6B, 0x7F, 0);	// 1.265V
        	pmic_config_interface(MT6328_VCORE1_CON11, 0x65, 0x7F, 0);	// 1.230V
        	break;
        case 1: // HV 938MHz
        	//pmic_config_interface(MT6328_VCORE1_CON11, 0x59, 0x7F, 0);	// 1.155V
        	pmic_config_interface(MT6328_VCORE1_CON11, 0x54, 0x7F, 0);	// 1.124V
        	break;
        case 2:	// NV 1280MHz 
        	pmic_config_interface(MT6328_VCORE1_CON11, 0x58, 0x7F, 0);	// 1.15V
        	break;
        case 3: // NV 938MHz
        	pmic_config_interface(MT6328_VCORE1_CON11, 0x48, 0x7F, 0);	// 1.05V
        	break;
        case 4:	// LV 1280MHz 
        	//pmic_config_interface(MT6328_VCORE1_CON11, 0x46, 0x7F, 0);	// 1.035V
        	pmic_config_interface(MT6328_VCORE1_CON11, 0x4B, 0x7F, 0);	// 1.070V
        	break;
        case 5: // LV 938MHz
        	//pmic_config_interface(MT6328_VCORE1_CON11, 0x38, 0x7F, 0);	// 0.945V
        	pmic_config_interface(MT6328_VCORE1_CON11, 0x3C, 0x7F, 0);	// 0.976V
        	break;
        default:
        	printf("Enter incorrect config number !!! \r\n");
        	break;
    }

}

void pmic_Vmem_adjust(int nAdjust)
{
    switch (nAdjust) {
        case 0:
        	pmic_config_interface(MT6328_SLDO_ANA_CON0, 0, 0x3, 0);	// 1.24V
        	break;
        case 1:
        	pmic_config_interface(MT6328_SLDO_ANA_CON0, 1, 0x3, 0);	// 1.39V
        	break;
        case 2:
        	pmic_config_interface(MT6328_SLDO_ANA_CON0, 2, 0x3, 0);	// 1.54V
        	break;
        default:
        	pmic_config_interface(MT6328_SLDO_ANA_CON0, 0, 0x3, 0);	// 1.24V
        	break;
    }
}

void pmic_Vmem_Cal_adjust(int nAdjust)
{
    switch (nAdjust) {
        case 0:
        	pmic_config_interface(MT6328_SLDO_ANA_CON1, 0x0D, 0xF, 8);	// +0.6V
        	break;
        case 1:
        	pmic_config_interface(MT6328_SLDO_ANA_CON1, 0x01, 0xF, 8);	// -0.2V
        	break;
        case 2:
        	//pmic_config_interface(MT6328_SLDO_ANA_CON1, 0x04, 0xF, 8);	// -0.8V
        	pmic_config_interface(MT6328_SLDO_ANA_CON1, 0x05, 0xF, 8);	// -1.0V
        	break;
        default:
        	pmic_config_interface(MT6328_SLDO_ANA_CON1, 0x00, 0xF, 8);	// 1.24V
        	break;
    }
}

void pmic_HQA_NoSSC_Voltage_adjust(int nAdjust)
{
    switch (nAdjust) {
        case 0: // Vm = 1.18V & Vc = 1.15V
        	pmic_config_interface(MT6328_VCORE1_CON11, 0x58, 0x7F, 0);	// 1.15V
        	pmic_config_interface(MT6328_SLDO_ANA_CON1, 0x03, 0xF, 8);	// -0.6V (1.18V)
        	printf("========== Vm = 1.18V & Vc = 1.15V ==========\r\n");
        	break;
        case 1: // Vm = 1.26V & Vc = 1.15V
        	pmic_config_interface(MT6328_VCORE1_CON11, 0x58, 0x7F, 0);	// 1.15V
        	pmic_config_interface(MT6328_SLDO_ANA_CON1, 0x0F, 0xF, 8);	// +0.2V (1.26V)
        	printf("========== Vm = 1.26V & Vc = 1.15V ==========\r\n");
        	break;
        case 2: // Vm = 1.22V & Vc = 1.11V
        	pmic_config_interface(MT6328_VCORE1_CON11, 0x52, 0x7F, 0);	// 1.11V
        	pmic_config_interface(MT6328_SLDO_ANA_CON1, 0x01, 0xF, 8);	// -0.2V (1.22V)
        	printf("========== Vm = 1.22V & Vc = 1.11V ==========\r\n");
        	break;
        case 3: // Vm = 1.22V & Vc = 1.19V
        	pmic_config_interface(MT6328_VCORE1_CON11, 0x5F, 0x7F, 0);	// 1.19V
        	pmic_config_interface(MT6328_SLDO_ANA_CON1, 0x01, 0xF, 8);	// -0.2V (1.22V)
        	printf("========== Vm = 1.22V & Vc = 1.19V ==========\r\n");
        	break;
        case 4: //NV
        	pmic_config_interface(MT6328_VCORE1_CON11, 0x58, 0x7F, 0);	// 1.15V
        	pmic_config_interface(MT6328_SLDO_ANA_CON1, 0x01, 0xF, 8);	// -0.2V (1.22V)
        	printf("========== NV ==========\r\n");
        	break;
        default: //NV
        	pmic_config_interface(MT6328_VCORE1_CON11, 0x58, 0x7F, 0);	// 1.15V
        	pmic_config_interface(MT6328_SLDO_ANA_CON1, 0x01, 0xF, 8);	// -0.2V (1.22V)
        	printf("========== NV ==========\r\n");
        	break;
    }
}

void pmic_HQA_Voltage_adjust(int nAdjust)
{
    switch (nAdjust) {
        case 0: //HVcHVm
        	//pmic_config_interface(MT6328_VCORE1_CON11, 0x6B, 0x7F, 0);	// 1.265V
#if defined(MACH_TYPE_MT6753)
        	pmic_config_interface(MT6328_VCORE1_CON11, 0x71, 0x7F, 0);	// 1.310V
#else
        	pmic_config_interface(MT6328_VCORE1_CON11, 0x65, 0x7F, 0);	// 1.230V
#endif
        	pmic_config_interface(MT6328_SLDO_ANA_CON1, 0x0D, 0xF, 8);	// +0.6V (1.3V)
        	printf("========== HVcHVm ==========\r\n");
        	break;
        case 1: //HVcLVm
        	//pmic_config_interface(MT6328_VCORE1_CON11, 0x6B, 0x7F, 0);	// 1.265V
#if defined(MACH_TYPE_MT6753)
        	pmic_config_interface(MT6328_VCORE1_CON11, 0x71, 0x7F, 0);	// 1.310V
        	pmic_config_interface(MT6328_SLDO_ANA_CON1, 0x04, 0xF, 8);	// -0.8V (1.16V)
#else
        	pmic_config_interface(MT6328_VCORE1_CON11, 0x65, 0x7F, 0);	// 1.230V
        	pmic_config_interface(MT6328_SLDO_ANA_CON1, 0x05, 0xF, 8);	// -1.0V (1.14V)
#endif
        	printf("========== HVcLVm ==========\r\n");
        	break;
        case 2: //LVcHVm
        	//pmic_config_interface(MT6328_VCORE1_CON11, 0x46, 0x7F, 0);	// 1.035V
#if defined(MACH_TYPE_MT6753)
        	pmic_config_interface(MT6328_VCORE1_CON11, 0x54, 0x7F, 0);	// 1.125V
#else
        	pmic_config_interface(MT6328_VCORE1_CON11, 0x4B, 0x7F, 0);	// 1.070V
#endif
        	pmic_config_interface(MT6328_SLDO_ANA_CON1, 0x0D, 0xF, 8);	// +0.6V (1.3V)
        	printf("========== LVcHVm ==========\r\n");
        	break;
        case 3: //LVcLVm
        	//pmic_config_interface(MT6328_VCORE1_CON11, 0x46, 0x7F, 0);	// 1.035V
#if defined(MACH_TYPE_MT6753)
        	pmic_config_interface(MT6328_VCORE1_CON11, 0x54, 0x7F, 0);	// 1.125V
        	pmic_config_interface(MT6328_SLDO_ANA_CON1, 0x04, 0xF, 8);	// -0.8V (1.16V)
#else
        	pmic_config_interface(MT6328_VCORE1_CON11, 0x4B, 0x7F, 0);	// 1.070V
        	pmic_config_interface(MT6328_SLDO_ANA_CON1, 0x05, 0xF, 8);	// -1.0V (1.14V)
#endif
        	printf("========== LVcLVm ==========\r\n");
        	break;
        case 4: //NV
#if defined(MACH_TYPE_MT6753)
        	pmic_config_interface(MT6328_VCORE1_CON11, 0x68, 0x7F, 0);	// 1.25V
#else
        	pmic_config_interface(MT6328_VCORE1_CON11, 0x58, 0x7F, 0);	// 1.15V
#endif
        	pmic_config_interface(MT6328_SLDO_ANA_CON1, 0x01, 0xF, 8);	// -0.2V (1.22V)
        	printf("========== NV ==========\r\n");
        	break;
        default: //NV
#if defined(MACH_TYPE_MT6753)
        	pmic_config_interface(MT6328_VCORE1_CON11, 0x68, 0x7F, 0);	// 1.25V
#else
        	pmic_config_interface(MT6328_VCORE1_CON11, 0x58, 0x7F, 0);	// 1.15V
#endif
        	pmic_config_interface(MT6328_SLDO_ANA_CON1, 0x01, 0xF, 8);	// -0.2V (1.22V)
        	printf("========== NV ==========\r\n");
        	break;
    }
}
#endif

void mt_set_DCM_clock (void)
{
#ifdef fcENABLE_DDRPHY_DCM_FUNC
     // 0x640[3] NA for D123 (for DDR3)
    #ifdef DDRPHY_3PLL_MODE
        #ifdef DDRPHY_2PLL
            //*((V_UINT32P)(DDRPHY_BASE + 0x0640)) |= (0xfea98000<<0);
            //==========================================
            // issue resolve: [21] = 0
            *((V_UINT32P)(DDRPHY_BASE + 0x0640)) |= (0xfe898000<<0);
            //==========================================
        #else
            //*((V_UINT32P)(DDRPHY_BASE + 0x0640)) |= (0xfea98000<<0);      //enable DDRPHY dynamic clk gating, // disable C_PHY_M_CK dynamic gating
            //==========================================
            // issue resolve: [21] = 0
            *((V_UINT32P)(DDRPHY_BASE + 0x0640)) |= (0xfe898000<<0);
            //==========================================
        #endif
    #else
        // [16] R_MEMPLL2_FB_M_CK_CG_EN -> no need for 1PLL mode (feedback clock)
        //*((V_UINT32P)(DDRPHY_BASE + 0x0640)) |= (0xfea88000<<0);      //enable DDRPHY dynamic clk gating, // disable C_PHY_M_CK dynamic gating
        //==========================================
        // issue resolve: [21] = 0
        *((V_UINT32P)(DDRPHY_BASE + 0x0640)) |= (0xfe888000<<0);
        //==========================================
    #endif
#endif // fcENABLE_DDRPHY_DCM_FUNC
}

#define r_dmpll2_clk_en               (0x00000001 << 5)   //190[5]
#define r_dmall_ck_en                 (0x00000001 << 4)   //190[4]
void pll_switch_to_RG(void)
{
    *((UINT32P)(DDRPHY_BASE + (0x0172 <<2))) = 0x1111FF11;
    *((UINT32P)(DDRPHY_BASE + (0x0173 <<2))) = 0x11511111;
    return;
}

void pll_switch_to_SPM(int pll_mode)
{
    /**********************************
    * (4) MEMPLL control switch to SPM
    ***********************************/
#ifdef fcSWITCH_SPM_CONTROL
    if ((pll_mode == PLL_MODE_3) || (pll_mode == PLL_MODE_2))
    {
        *((UINT32P)(DDRPHY_BASE + (0x0173 <<2))) = 0x40101000;   //[0]ISO_EN_SRC=0,[22]DIV_EN_SC_SRC=0 (pll2off),[16]DIV_EN_SRC=0,[8]PLL2_CK_EN_SRC=1(1pll),[8]PLL2_CK_EN_SRC=0(3pll)
    }
    else // 1-PLL mode
    {
        *((UINT32P)(DDRPHY_BASE + (0x0173 <<2))) = 0x40101000;   //[0]ISO_EN_SRC=0,[22]DIV_EN_SC_SRC=0 (pll2off),[16]DIV_EN_SRC=0,[8]PLL2_CK_EN_SRC=1(1pll),[8]PLL2_CK_EN_SRC=0(3pll)
    }

    if (pll_mode == PLL_MODE_3)
    {
        *((UINT32P)(DDRPHY_BASE + (0x0172 <<2))) = 0x0000F010;   //[24]BIAS_EN_SRC=0,[16]BIAS_LPF_EN_SRC=0,[8]MEMPLL_EN,[9][10][11]MEMPLL2,3,4_EN_SRC,[0]ALL_CK_EN_SRC=0
    }
    else if (pll_mode == PLL_MODE_2)
    {
        *((UINT32P)(DDRPHY_BASE + (0x0172 <<2))) = 0x0000F410;   //PLL3 switch to SW control
    }
    else // 1-PLL mode
    {
        *((UINT32P)(DDRPHY_BASE + (0x0172 <<2))) = 0x0000FC10;   //1PLL mode, MEMPLL3,4_EN no change to spm controller.eep to 1'b0 for power saving.
        //*((UINT32P)(DDRPHY_BASE + (0x0172 <<2))) = 0x0000F010; // sim ok
    }

    //*((UINT32P)(DDRPHY_BASE + (0x0170 <<2))) = 0x003C1B96;   //setting for delay time
    *((UINT32P)(DDRPHY_BASE + (0x0170 <<2))) = 0x063C0000;   //setting for delay time
#endif
    return;
}

#define pllc1_mempll_bias_en          (0x00000001 <<14)   //181[14]
#define pllc1_mempll_bias_lpf_en      (0x00000001 <<15)   //181[15]
#define pllc1_mempll_en               (0x00000001 << 2)   //180[2]
#define pllc1_mempll_div_en           (0x00000001 <<24)   //181[24]
#define mempll2_en                    (0x00000001 <<18)   //183[18]
#define mempll3_en                    (0x00000001 <<18)   //185[18]
#define mempll4_en                    (0x00000001 <<18)   //187[18]
void prepare_mempll_power_on(int pll_mode)
{
    /***********************************
    * (3) Setup MEMPLL power on sequence
    ************************************/

    //gpt_busy_wait_us(2);

    *((UINT32P)(DDRPHY_BASE + (0x0181 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0181 <<2))) & (~pllc1_mempll_bias_en)); //RG_MEMPLL_BIAS_EN = 1'b1;

    //gpt_busy_wait_us(2);

    *((UINT32P)(DDRPHY_BASE + (0x0181 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0181 <<2))) & (~pllc1_mempll_bias_lpf_en)); //RG_MEMPLL_BIAS_LPF_EN = 1'b1;

    //gpt_busy_wait_us(1000);

    *((UINT32P)(DDRPHY_BASE + (0x0180 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0180 <<2))) & (~pllc1_mempll_en)); //RG_MEMPLL_EN = 1'b1;

    //gpt_busy_wait_us(20);

    *((UINT32P)(DDRPHY_BASE + (0x0181 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0181 <<2))) & (~pllc1_mempll_div_en)); //RG_MEMPLL_DIV_EN = 1'b1;

    //gpt_busy_wait_us(1);

    if (pll_mode == PLL_MODE_3)
    {
        *((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) & (~mempll2_en)); //RG_MEMPLL2_EN = 1'b1;
        *((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) & (~mempll3_en)); //RG_MEMPLL3_EN = 1'b1;
        *((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) & (~mempll4_en)); //RG_MEMPLL4_EN = 1'b1;
    }
    else if (pll_mode == PLL_MODE_2)
    {
        *((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) & (~mempll2_en)); //RG_MEMPLL2_EN = 1'b1;
        *((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) & (~mempll3_en)); //RG_MEMPLL3_EN = 1'b0;
        *((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) & (~mempll4_en)); //RG_MEMPLL4_EN = 1'b1;
    }
    else
    {
        *((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) & (~mempll2_en)); //RG_MEMPLL2_EN = 1'b1;
        *((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) & (~mempll3_en)); //RG_MEMPLL3_EN = 1'b0;
        *((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) & (~mempll4_en)); //RG_MEMPLL4_EN = 1'b0;
    }

    //gpt_busy_wait_us(23);

    if (pll_mode == PLL_MODE_2)
    {
#ifndef DDRPHY_2PLL_LONG_CKTREE
        *((UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00010022;
#else
        *((UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00010026;
#endif
    }
    else if (pll_mode == PLL_MODE_3)
    {
        *((UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00010020;    //3PLL sync mode
    }
    else // 1-PLL mode
    {
        *((UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00000007;    //1PLL sync mode
    }
    return;
}

void mempll_power_on_sequence(int pll_mode)
{
    /***********************************
    * (3) Setup MEMPLL power on sequence
    ************************************/

    gpt_busy_wait_us(2);

    *((UINT32P)(DDRPHY_BASE + (0x0181 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0181 <<2))) & (~pllc1_mempll_bias_en)) | (0x00000001 << 14); //RG_MEMPLL_BIAS_EN = 1'b1;

    gpt_busy_wait_us(2);

    *((UINT32P)(DDRPHY_BASE + (0x0181 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0181 <<2))) & (~pllc1_mempll_bias_lpf_en)) | (0x00000001 << 15); //RG_MEMPLL_BIAS_LPF_EN = 1'b1;

    gpt_busy_wait_us(1000);

    *((UINT32P)(DDRPHY_BASE + (0x0180 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0180 <<2))) & (~pllc1_mempll_en)) | (0x00000001 << 2); //RG_MEMPLL_EN = 1'b1;

    gpt_busy_wait_us(20);

    *((UINT32P)(DDRPHY_BASE + (0x0181 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0181 <<2))) & (~pllc1_mempll_div_en)) | (0x00000001 << 24); //RG_MEMPLL_DIV_EN = 1'b1;

    gpt_busy_wait_us(1);

    if (pll_mode == PLL_MODE_3)
    {
        *((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) & (~mempll2_en)) | (0x00000001 << 18); //RG_MEMPLL2_EN = 1'b1;
        *((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) & (~mempll3_en)) | (0x00000001 << 18); //RG_MEMPLL3_EN = 1'b1;
        *((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) & (~mempll4_en)) | (0x00000001 << 18); //RG_MEMPLL4_EN = 1'b1;
    }
    else if (pll_mode == PLL_MODE_2)
    {
        *((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) & (~mempll2_en)) | (0x00000001 << 18); //RG_MEMPLL2_EN = 1'b1;
        *((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) & (~mempll3_en)) | (0x00000000 << 18); //RG_MEMPLL3_EN = 1'b0;
        *((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) & (~mempll4_en)) | (0x00000001 << 18); //RG_MEMPLL4_EN = 1'b1;
    }
    else
    {
        *((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0183 <<2))) & (~mempll2_en)) | (0x00000001 << 18); //RG_MEMPLL2_EN = 1'b1;
        *((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0185 <<2))) & (~mempll3_en)) | (0x00000000 << 18); //RG_MEMPLL3_EN = 1'b0;
        *((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) = (*((UINT32P)(DDRPHY_BASE + (0x0187 <<2))) & (~mempll4_en)) | (0x00000000 << 18); //RG_MEMPLL4_EN = 1'b0;
    }

    gpt_busy_wait_us(23);

    if (pll_mode == PLL_MODE_2)
    {
#ifndef DDRPHY_2PLL_LONG_CKTREE
        *((UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00010022  | r_dmpll2_clk_en | r_dmall_ck_en;
#else
        *((UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00010026  | r_dmpll2_clk_en | r_dmall_ck_en;
#endif
    }
    else if (pll_mode == PLL_MODE_3)
    {
        *((UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00010020 | r_dmpll2_clk_en | r_dmall_ck_en;    //3PLL sync mode
    }
    else // 1-PLL mode
    {
        *((UINT32P)(DDRPHY_BASE + (0x0190 <<2))) = 0x00000007 | r_dmpll2_clk_en | r_dmall_ck_en;    //1PLL sync mode
    }
    return;
}

void restore_DDR_regs(void)
{
#ifndef DDRPHY_3PLL_MODE
    pll_switch_to_RG(); //PLL Switch to RG control
    prepare_mempll_power_on(PLL_MODE_1); //PLL RGs back to default
    mempll_power_on_sequence(PLL_MODE_1); //Rerun mempll power on
    pll_switch_to_SPM(PLL_MODE_1); //PLL Switch to SPM control  
#endif    
    mt_set_DCM_clock(); //Restore DCM clock from DDR reserved mode

#ifdef DDRPHY_3PLL_MODE
    /* The mempll calib is valid after conf iso */
    if((g_ddr_reserve_enable==1) && (g_ddr_reserve_success==1)) {
        printf("[DDR Reserved Mode] mempll calib for 2/3-PLL\n");
        mt_mempll_cali();
    }
#endif
}

/*
* mt_set_emi: Set up EMI/DRAMC.
*/
#if CFG_FPGA_PLATFORM
void mt_set_emi (void)
{
    MCUSYS_CFGREG_WRITE_REG(0x00025352, 0x3000); // 2 ranks - 1GB
    //MCUSYS_CFGREG_WRITE_REG(0x00025302, 0x3000); // 2 ranks - 512MB
    //MCUSYS_CFGREG_WRITE_REG(0x00002112, 0x3000); // per ranks
    MCUSYS_CFGREG_WRITE_REG(0x00000400, 0x3060); //EMI_CONA
    //DDRPHY Settings
    DDRPHY_WRITE_REG(0x03051192, 0x110);
    DDRPHY_WRITE_REG(0x0000110d, 0x048);
    DDRPHY_WRITE_REG(0x00500900, 0x0D8);
    DDRPHY_WRITE_REG(0x00000001, 0x0E4);
    DDRPHY_WRITE_REG(0x00000001, 0x08C);
    DDRPHY_WRITE_REG(0x00000000, 0x090);
    DDRPHY_WRITE_REG(0x80000000, 0x094);
    DDRPHY_WRITE_REG(0x83004004, 0x0DC);
    DDRPHY_WRITE_REG(0x01004004, 0x0E0);
    DDRPHY_WRITE_REG(0x00000000, 0x0F0);
    DDRPHY_WRITE_REG(0x01000000, 0x0F4);
    DDRPHY_WRITE_REG(0x00000080, 0x168);
    DDRPHY_WRITE_REG(0x00700900, 0x0D8);
    DDRPHY_WRITE_REG(0xF00487A2, 0x004);
    DDRPHY_WRITE_REG(0x900641C0, 0x07C);
    DDRPHY_WRITE_REG(0xF1200F01, 0x028);
    DDRPHY_WRITE_REG(0x3001EBFF, 0x1E0);
    DDRPHY_WRITE_REG(0x00000000, 0x158);
    DDRPHY_WRITE_REG(0x00000005, 0x0E4);
    gpt_busy_wait_ms(1);
    DDRPHY_WRITE_REG(0x0000003F, 0x088);
    gpt_busy_wait_ms(1);
    DDRPHY_WRITE_REG(0x00FF000A, 0x088);
    gpt_busy_wait_ms(1);
    DDRPHY_WRITE_REG(0x00320001, 0x088);
    gpt_busy_wait_ms(1);
    DDRPHY_WRITE_REG(0x00020002, 0x088);
    gpt_busy_wait_ms(1);
    DDRPHY_WRITE_REG(0x00000001, 0x0E4);
    DDRPHY_WRITE_REG(0x00000A56, 0x084);
    DDRPHY_WRITE_REG(0x00000000, 0x00C);
    DDRPHY_WRITE_REG(0x45D84408, 0x000);
    DDRPHY_WRITE_REG(0xA8080401, 0x044);
    DDRPHY_WRITE_REG(0x00000510, 0x1E8);
    DDRPHY_WRITE_REG(0x03406340, 0x008);
    DDRPHY_WRITE_REG(0x00000000, 0x010);
    DDRPHY_WRITE_REG(0xEDCB000F, 0x0F8);
    DDRPHY_WRITE_REG(0x27010000, 0x0FC);
    // DRAMC Settings
    //DRAMC0_WRITE_REG(0x03051192, 0x110);	// WS1622 - 512MB
    DRAMC0_WRITE_REG(0x04051192, 0x110);	// WS2666 - 1GB
    DRAMC0_WRITE_REG(0xAA080000, 0x124);
    DRAMC0_WRITE_REG(0x00000001, 0x118);
    DRAMC0_WRITE_REG(0x0000110d, 0x048);
    DRAMC0_WRITE_REG(0x00500900, 0x0D8);
    DRAMC0_WRITE_REG(0x00000001, 0x0E4);
    DRAMC0_WRITE_REG(0x00000001, 0x08C);
    DRAMC0_WRITE_REG(0x00000000, 0x090);
    DRAMC0_WRITE_REG(0x80000000, 0x094);
    DRAMC0_WRITE_REG(0x83004004, 0x0DC);
    DRAMC0_WRITE_REG(0x01004004, 0x0E0);
    DRAMC0_WRITE_REG(0x00000000, 0x0F0);
    DRAMC0_WRITE_REG(0x01000000, 0x0F4);
    DRAMC0_WRITE_REG(0x00000080, 0x168);
    DRAMC0_WRITE_REG(0x00700900, 0x0D8);
    DRAMC0_WRITE_REG(0xF00487A2, 0x004);
    DRAMC0_WRITE_REG(0x900641C0, 0x07C);
    DRAMC0_WRITE_REG(0xF1200F01, 0x028);
    DRAMC0_WRITE_REG(0x3001EBFF, 0x1E0);
    DRAMC0_WRITE_REG(0x00000000, 0x158);
    DRAMC0_WRITE_REG(0x00000005, 0x0E4);
    gpt_busy_wait_ms(1);
    DRAMC0_WRITE_REG(0x0000003F, 0x088);
    DRAMC0_WRITE_REG(0x00000001, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    gpt_busy_wait_ms(1);
    DRAMC0_WRITE_REG(0x00FF000A, 0x088);
    DRAMC0_WRITE_REG(0x00000001, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    gpt_busy_wait_ms(1);
    DRAMC0_WRITE_REG(0x00320001, 0x088);
    DRAMC0_WRITE_REG(0x00000001, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    gpt_busy_wait_ms(1);
    DRAMC0_WRITE_REG(0x00020002, 0x088);
    DRAMC0_WRITE_REG(0x00000001, 0x1E4);
    DRAMC0_WRITE_REG(0x00001100, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    DRAMC0_WRITE_REG(0x00000000, 0x1E4);
    gpt_busy_wait_ms(1);
    DRAMC0_WRITE_REG(0x00000001, 0x0E4);
    DRAMC0_WRITE_REG(0x00000A56, 0x084);
    DRAMC0_WRITE_REG(0x00000000, 0x00C);
    DRAMC0_WRITE_REG(0x45D84408, 0x000);
    DRAMC0_WRITE_REG(0xA8080401, 0x044);
    DRAMC0_WRITE_REG(0x00000510, 0x1E8);
    DRAMC0_WRITE_REG(0x03406340, 0x008);
    DRAMC0_WRITE_REG(0x00000000, 0x010);
    DRAMC0_WRITE_REG(0xEDCB000F, 0x0F8);
    DRAMC0_WRITE_REG(0x27010000, 0x0FC);
}
#else
void mt_set_emi (void)
{
    int index = 0;
    unsigned int val1,val2/*, temp1, temp2*/;
    EMI_SETTINGS *emi_set;
    unsigned int manu_id;
    *EMI_CONI |= 0x0000CC00; // CWM500 4G Max throughput test phone hang issue

#ifdef DRAM_INIT_CYCLES
    int temp;

    /* enable ARM CPU PMU */
    asm volatile(
        "MRC p15, 0, %0, c9, c12, 0\n"
        "BIC %0, %0, #1 << 0\n"   /* disable */
        "ORR %0, %0, #1 << 2\n"   /* reset cycle count */
        "BIC %0, %0, #1 << 3\n"   /* count every clock cycle */
        "MCR p15, 0, %0, c9, c12, 0\n"
        : "+r"(temp)
        :
        : "cc"
    );
    asm volatile(
        "MRC p15, 0, %0, c9, c12, 0\n"
        "ORR %0, %0, #1 << 0\n"   /* enable */
        "MCR p15, 0, %0, c9, c12, 0\n"
        "MRC p15, 0, %0, c9, c12, 1\n"
        "ORR %0, %0, #1 << 31\n"
        "MCR p15, 0, %0, c9, c12, 1\n"
        : "+r"(temp)
        :
        : "cc"
    );
#endif

#ifdef COMBO_MCP
	DDR_type = mt_get_dram_type();
	printf ("mt_get_dram_type() 0x%x\n",DDR_type);
	switch (DDR_type)
	{
    		case TYPE_mDDR:
        		printf("[EMI] DDR1\r\n");
        		break;
    		case TYPE_LPDDR2:
        		printf("[EMI] LPDDR2\r\n");
        		break;
    		case TYPE_LPDDR3:
        		printf("[EMI] LPDDR3\r\n");
        		break;
    		case TYPE_PCDDR3:
        		printf("[EMI] PCDDR3\r\n");
        		break;
    		default:
        		printf("[EMI] unknown dram type:%d\r\n",mt_get_dram_type());
        		break;
	}

	index = mt_get_mdl_number ();
	printf("[Check]mt_get_mdl_number 0x%x\n",index);
	printf("[EMI] eMMC/NAND ID = %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x\r\n", id[0], id[1], id[2], id[3], id[4], id[5], id[6], id[7], id[8],id[9],id[10],id[11],id[12],id[13],id[14],id[15]);
	if (index < 0 || index >=  num_of_emi_records)
	{
    		print("[EMI] setting failed 0x%x\r\n", index);
    		return;
	}

	printf("[EMI] MDL number = %d\r\n", index);
	emi_set = &emi_settings[index];
	printf("[EMI] emi_set eMMC/NAND ID = %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x\r\n", emi_set->ID[0], emi_set->ID[1], emi_set->ID[2], emi_set->ID[3], emi_set->ID[4], emi_set->ID[5], emi_set->ID[6], emi_set->ID[7], emi_set->ID[8],emi_set->ID[9],emi_set->ID[10],emi_set->ID[11],emi_set->ID[12],emi_set->ID[13],emi_set->ID[14],emi_set->ID[15]);

	if ((emi_set->type & 0xF) == TYPE_LPDDR2)
	{
    		//mt_mempll_init(DDR1066, PLL_MODE_1);

    		#ifdef REXTDN_ENABLE
    		    ett_rextdn_sw_calibration();
    		#endif

    		init_lpddr2(emi_set);
	}
	else if ((emi_set->type & 0xF) == TYPE_LPDDR3)
	{
    		//emi_set = &emi_setting_default_lpddr3;

    		//mt_mempll_init(DDR1333, PLL_MODE_1);

    		#ifdef REXTDN_ENABLE
        	    ett_rextdn_sw_calibration();
    		#endif

    		init_lpddr3(emi_set);

	}
	else if ((emi_set->type & 0xF) == TYPE_PCDDR3)
	{//fy for 92 bring up
		//1.mempll 1066MHz --> 1466MHz
		//int dram_clock = 938;
		//print("[EMI] PCDDR3-%d, DRAM Clock = %d MHz\r\n", dram_clock, dram_clock/2);
		//mt_mempll_init(dram_clock, 1);

		//2.REXTDN Calibration
		#ifdef REXTDN_ENABLE
	    	    printf("[EMI] PCDDR3 RXTDN Calibration:\r\n");
            	    ett_rextdn_sw_calibration();
		#endif
		//----------------------------------------
		//3.DDR3 init
		init_ddr3(emi_set);
		//----------------------------------------
	} else {
    		printf("The DRAM type is not supported");
    		ASSERT(0);
	}
//    print_DBG_info();
#else

#ifdef HARDCODE_DRAM_SETTING
    index = mt_get_mdl_number ();
    emi_set = &emi_settings[index];	//LPDDR3
#endif

#ifdef REXTDN_ENABLE
    ett_rextdn_sw_calibration();
#endif

    #ifdef DDRTYPE_LPDDR2
    init_lpddr2(emi_set);
    #endif

    #ifdef DDRTYPE_DDR3
    init_ddr3(emi_set);
    #endif

    #ifdef DDRTYPE_LPDDR3
    init_lpddr3(emi_set);
    #endif

#endif	// end #ifdef COMBO_MCP

#if 0
	/* save original PLL setting */
	temp1 = DRV_Reg32(AP_PLL_CON1);
	temp2 = DRV_Reg32(AP_PLL_CON2);
	temp1 = temp1 & 0xFF00FC0C;
	temp2 = temp2 & 0xFFFFFFF8;

	//SWITCH FROM SW TO HW SPM SLEEP CONTROL
	DRV_WriteReg32(AP_PLL_CON1, 0x0);
	DRV_WriteReg32(AP_PLL_CON2, 0x0);
	//*AP_PLL_CON1 = 0x0;
	//*AP_PLL_CON2 = 0x0;
#endif

	// Ungate dram transaction blockage mechanism after Dram Init.
	// If bit 10 of EMI_CONM is not 1, any transaction to EMI will be ignored.
	*EMI_CONM |= (1<<10);

	//DRAM Translate Off
	// Edward : After discussing with CC Wen, need to test different patterns like following  during calibration to see which one is worse in the future. If possible, do two is good.
	// 					TESTXTALKPAT(REG.48[16])		TESTAUDPAT (REG.44[7])
	// Audio pattern				0							1
	// Cross-talk					1							0

// NA for D123
#if 0
#ifndef PHYSYNC_MODE
        // not PHYSYNC mode.
        *((V_UINT32P)(DRAMC0_BASE + 0x015c)) |= 0x80000000;    //15c.31 set to 1 in sync mode, 0 in PHYsync mode
        *((V_UINT32P)(DDRPHY_BASE + 0x015c)) |= 0x80000000;    //15c.31 set to 1 in sync mode, 0 in PHYsync mode
#endif
#endif

#ifdef DRAM_INIT_CYCLES
    /* get CPU cycle count from the ARM CPU PMU */
    asm volatile(
        "MRC p15, 0, %0, c9, c12, 0\n"
        "BIC %0, %0, #1 << 0\n"   /* disable */
        "MCR p15, 0, %0, c9, c12, 0\n"
        "MRC p15, 0, %0, c9, c13, 0\n"
        : "+r"(temp)
        :
        : "cc"
    );
    printf("DRAMC COMBO_MCP calibration takes %d CPU cycles\n\r", temp);
#endif

#ifdef DRAM_INIT_CYCLES
    /* enable ARM CPU PMU */
    asm volatile(
        "MRC p15, 0, %0, c9, c12, 0\n"
        "BIC %0, %0, #1 << 0\n"   /* disable */
        "ORR %0, %0, #1 << 2\n"   /* reset cycle count */
        "BIC %0, %0, #1 << 3\n"   /* count every clock cycle */
        "MCR p15, 0, %0, c9, c12, 0\n"
        : "+r"(temp)
        :
        : "cc"
    );
    asm volatile(
        "MRC p15, 0, %0, c9, c12, 0\n"
        "ORR %0, %0, #1 << 0\n"   /* enable */
        "MCR p15, 0, %0, c9, c12, 0\n"
        "MRC p15, 0, %0, c9, c12, 1\n"
        "ORR %0, %0, #1 << 31\n"
        "MCR p15, 0, %0, c9, c12, 1\n"
        : "+r"(temp)
        :
        : "cc"
    );
#endif

    if (dramc_calib(emi_set) < 0) {
        print("[EMI] DRAMC calibration failed\n\r");
    } else {
        print("[EMI] DRAMC calibration passed\n\r");
    }

#ifdef DUMP_DRAMC_REGS
    print_DBG_info();
#endif

#if 0
    /* revert PLL setting */
    DRV_WriteReg32(AP_PLL_CON1, temp1);
    DRV_WriteReg32(AP_PLL_CON2, temp2);
#endif

    //pmic_voltage_read(1);//check the pmic setting
    //print("[MEM]mt_get_dram_type:%d\n",mt_get_dram_type());
    /* FIXME: modem exception occurs if set the max pending count */
    /* To Repair SRAM */
    #if defined(CTP) || defined(SLT)
    	#ifdef REPAIR_SRAM
    		int repair_ret;
    		repair_ret = repair_sram();
    		if(repair_ret != 0){
        		printf("Sram repair failed %d\n", repair_ret);
        		while(1);
    		}
    		printf("Sram repaired ok\n");
    	#endif
    #endif

#ifdef GATING_CLOCK_CONTROL_DVT
    *((V_UINT32P)(DRAMC0_BASE + 0x01dc)) &= ~(0x1 << 31);       // Edward : Need to make sure bit 13 is 0.
    *((V_UINT32P)(DDRPHY_BASE + 0x01dc)) &= ~(0x1 << 30);
#endif

#ifdef DERATING_ENABLE
    derating_enable();
#endif


// Run time config
#ifdef fcENABLE_RUNTIME_CONFIG
    // Edward : The following setting is after calibration.
    // Edward : power-down CMP. (After REXTDN)
    // NA for D123
    //*((V_UINT32P)(DRAMC0_BASE + 0x0630)) |= (1<<20);
    //*((V_UINT32P)(DDRPHY_BASE + 0x0630)) |= (1<<20);

    // Need to be confirmed by STA
    if ( *(volatile unsigned *)(EMI_CONA)& 0x20000) /* check dual rank support */
    {
                unsigned int val1, val2;
                val1 = *((V_UINT32P)(DRAMC0_BASE + 0xe0)) & 0x07000000;
#ifdef PHYSYNC_MODE
                // 2PLL, 3PLL
                //val1 = ((val1 >> 24)+2) <<16;
                //==========================================
                //issue resolve
                val1 = ((val1 >> 24)+1) <<16;
                //==========================================
#else
                // 1PLL
                //val1 = ((val1 >> 24)+1) <<16;
                //==========================================
                //issue resolve
                val1 = ((val1 >> 24)+0) <<16;
                //==========================================
#endif
                val2 = *((V_UINT32P)(DRAMC0_BASE + 0x1c4)) & 0xFFF0FFFF | val1;
                *((V_UINT32P)(DRAMC0_BASE + 0x1c4)) = val2;
    }

#ifdef fcENABLE_HW_GATING // temp marked @ bring up, enable it later...
    // Edward : enable DQS GW HW enable[31].  (Tracking value apply to 2 ranks[30] need to be set to 1 when if DFS is applied.)
    *((V_UINT32P)(DRAMC0_BASE + 0x01c0)) |= (0x80000000);     // Edward : Need to make sure bit 13 is 0.
    *((V_UINT32P)(DDRPHY_BASE + 0x01c0)) |= (0x80000000);
#endif // fcENABLE_HW_GATING

#ifdef fcENABLE_DISPBREFOVERTHD // new function for D123, enable after DVT
    // [14:12] R_DMDISBYREFNUM : adjust the threshold, D13 is fixed to 3. D2 can use this.
    *((V_UINT32P)(DRAMC0_BASE + 0x00e4)) = (*((V_UINT32P)(DRAMC0_BASE + 0x00e4)) & 0xFFFF8FFF) | ((3 & 0x07) <<12);
    *((V_UINT32P)(DDRPHY_BASE + 0x00e4)) = (*((V_UINT32P)(DDRPHY_BASE + 0x00e4)) & 0xFFFF8FFF) | ((3 & 0x07) <<12);

    // [9] PBREF_DISBYREFNUM : enable
    *((V_UINT32P)(DRAMC0_BASE + 0x00e4)) |= (0x00000200);
    *((V_UINT32P)(DDRPHY_BASE + 0x00e4)) |= (0x00000200);
#endif // fcENABLE_DISPBREFOVERTHD

#ifdef fcENABLE_EMIPERFORMANCE
    // Edward : Add following based on CC Wen request.
    //      * WFLUSHEN (REG.1EC[14]): 1
    //      * R_DMRWHPRICTL (REG.1EC[13]): 0
    //      * R_DMEMILLATEN (REG.1EC[11]): 1
    //      R/W aging (REG.1EC[10]) : 1
    //    R/W low latency (REG.1EC[9]) : 1
    //    R/W high priority (REG.1EC[8]) : 1
    //      R/W out of order enable (REG.1EC[4]) : 1
    *((V_UINT32P)(DRAMC0_BASE + 0x01ec)) |= (0x4f10);         // Edward : Need to make sure bit 13 is 0.
    *((V_UINT32P)(DDRPHY_BASE + 0x01ec)) |= (0x4f10);
#endif // fcENABLE_EMIPERFORMANCE

#ifdef fcENABLE_REF_BLOCK_EMI_ARBITRATION
    // For D123, please set dramc AO 0xfc[26:25]=2'b00
    *((V_UINT32P)(DRAMC0_BASE + 0x00fc)) &= (0xf9ffffff);         // Edward : Need to make sure bit 13 is 0. 
    *((V_UINT32P)(DDRPHY_BASE + 0x00fc)) &= (0xf9ffffff);
#endif //fcENABLE_REF_BLOCK_EMI_ARBITRATION

    mt_set_DCM_clock();

#ifdef fcENABLE_DRAMC_DCM_FUNC
    *((V_UINT32P)(DRAMC0_BASE + 0x01dc)) |= 0x02000000; // [25] DCMEN=1
    *((V_UINT32P)(DDRPHY_BASE + 0x01dc)) |= 0x02000000; // [25] DCMEN=1
#endif // fcENABLE_DRAMC_DCM_FUNC

#ifdef WAVEFORM_MEASURE
    // 1dc[26] MIOCKCTRLOFF = 1, miock always on
    *((V_UINT32P)(DRAMC0_BASE + 0x01dc)) |= 0x04000000; 
    *((V_UINT32P)(DDRPHY_BASE + 0x01dc)) |= 0x04000000;         

    // 1ec[16] DISDMOEDIS = 1, self refresh not disable IO
    *((V_UINT32P)(DRAMC0_BASE + 0x01ec)) |= 0x00010000; 
    *((V_UINT32P)(DDRPHY_BASE + 0x01ec)) |= 0x00010000;            
#endif

#endif // fcENABLE_RUNTIME_CONFIG

//====================================================================================
// issue resolve
// enlarge timings
// RTW=8
*((V_UINT32P)(DRAMC0_BASE + 0x007c)) = (*((V_UINT32P)(DRAMC0_BASE + 0x007c)) & 0xFFFF0FFF) | ((8 & 0x0f) <<12);
*((V_UINT32P)(DDRPHY_BASE + 0x007c)) = (*((V_UINT32P)(DDRPHY_BASE + 0x007c)) & 0xFFFF0FFF) | ((8 & 0x0f) <<12);

//WTR+=2
val1 = ((DRAMC_READ_REG(0x000) & 0x00000f00)) >> 8;
val1 += 2;
if (val1 > 0xe)
    val1 = 0xe;
*((V_UINT32P)(DRAMC0_BASE + 0x0000)) = (*((V_UINT32P)(DRAMC0_BASE + 0x0000)) & 0xFFFFF0FF) | ((val1 & 0x0f) <<8);
*((V_UINT32P)(DDRPHY_BASE + 0x0000)) = (*((V_UINT32P)(DDRPHY_BASE + 0x0000)) & 0xFFFFF0FF) | ((val1 & 0x0f) <<8);

// XRTW2W=3, XRTW2R=3, XRTR2W=6, XRTR2R=6
*((V_UINT32P)(DRAMC0_BASE + 0x0110)) = (*((V_UINT32P)(DRAMC0_BASE + 0x0110)) & 0xFFF088FF) | (3 <<18) | (3<<16) | (6<<12) | (6<<8);
*((V_UINT32P)(DDRPHY_BASE + 0x0110)) = (*((V_UINT32P)(DDRPHY_BASE + 0x0110)) & 0xFFF088FF) | (3 <<18) | (3<<16) | (6<<12) | (6<<8);

// XRTW2R_M05T=1
*((V_UINT32P)(DRAMC0_BASE + 0x01f8)) |= (0x08000000); 
*((V_UINT32P)(DDRPHY_BASE + 0x01f8)) |= (0x08000000);

//DCMDLYREF (0x1dc[6:4]=7, original is 4)
*((V_UINT32P)(DRAMC0_BASE + 0x01dc)) = (*((V_UINT32P)(DRAMC0_BASE + 0x01dc)) & 0xFFFFFF8F) | (7<<4);
*((V_UINT32P)(DDRPHY_BASE + 0x01dc)) = (*((V_UINT32P)(DDRPHY_BASE + 0x01dc)) & 0xFFFFFF8F)  | (7<<4);

//COMBPHY_CLKENSAME=1 (0x1dc[3])
*((V_UINT32P)(DRAMC0_BASE + 0x01dc)) |= 0x00000008; 
*((V_UINT32P)(DDRPHY_BASE + 0x01dc)) |= 0x00000008; 
//====================================================================================

    check_dram_CONA_value(index);	// For DRAM size are 3GB/1.5GB

#ifdef DUMP_DRAMC_REGS
    print_DBG_info();
#endif

#if defined(MACH_TYPE_MT6735) || defined(MACH_TYPE_MT6735M) || defined(MACH_TYPE_MT6753) || defined(MACH_TYPE_MT6737T) || defined(MACH_TYPE_MT6737M)
#ifdef ENABLE_DFS
    DFS_ability_detection();
#endif
#endif

#ifdef DRAM_INIT_CYCLES
    /* get CPU cycle count from the ARM CPU PMU */
    asm volatile(
        "MRC p15, 0, %0, c9, c12, 0\n"
        "BIC %0, %0, #1 << 0\n"   /* disable */
        "MCR p15, 0, %0, c9, c12, 0\n"
        "MRC p15, 0, %0, c9, c13, 0\n"
        : "+r"(temp)
        :
        : "cc"
    );
    printf("DRAMC EXIT calibration takes %d CPU cycles\n\r", temp);
#endif
    //print("MR5:%x MR4:%x\n",DRAM_MRR(0x5),DRAM_MRR(0x4));

}
#endif //end #ifdef CFG_FPGA_PLATFORM

#if 0
void in_sref()
{
    volatile unsigned int tmp;
    tmp = DRAMC_READ_REG(DRAMC0_BASE+0x4);
    if(tmp & (0x1 << 26))
        print("Enable sref %x\n", tmp);
    else
        print("Disable sref %x\n", tmp);
    tmp = DRAMC_READ_REG(DRAMC0_BASE+0x3B8);
    if(tmp & (0x1 << 16))
        print("In sref %x\n", tmp);
    else
        print("Not in sref %x\n", tmp);
    tmp = READ_REG(TOPRGU_BASE+0x40);
    if(tmp & 0x10000)
        print("success finish ddr reserved flow. %x\n", tmp);
    else
        print("failed finish ddr reserved flow. %x\n", tmp);
}
#endif

#define	CHAN_DRAMC_NAO_MISC_STATUSA(base)	(base + 0x3B8)
#define SREF_STATE				(1 << 16)

static unsigned int is_dramc_exit_slf(void)
{
	unsigned int ret;

	ret = *(volatile unsigned *)CHAN_DRAMC_NAO_MISC_STATUSA(DRAMC_NAO_BASE);
	if ((ret & SREF_STATE) != 0) {
		print("DRAM CHAN-A is in self-refresh (MISC_STATUSA = 0x%x)\n", ret);
		return 0;
	}

	print("ALL DRAM CHAN is not in self-refresh\n");
	return 1;
}

void release_dram(void)
{
#ifdef DDR_RESERVE_MODE
    int counter = TIMEOUT;
    rgu_release_rg_dramc_conf_iso();
    rgu_release_rg_dramc_iso();
    restore_DDR_regs(); // for DDR reserved mode restore DCM clock & mempll_calib regs
    rgu_release_rg_dramc_sref();

    while(counter)
    {
      if(is_dramc_exit_slf() == 1) /* expect to exit dram-self-refresh */
        break;
      counter--;
    }
    if(counter == 0)
    {
      if(g_ddr_reserve_enable==1 && g_ddr_reserve_success==1)
      {
        print("[DDR Reserve] release dram from self-refresh FAIL!\n");
        g_ddr_reserve_success = 0;
      }
    }
#endif
}

void check_ddr_reserve_status(void)
{
#ifdef DDR_RESERVE_MODE
    int counter = TIMEOUT;
    if(rgu_is_reserve_ddr_enabled())
    {
      g_ddr_reserve_enable = 1;
      if(rgu_is_reserve_ddr_mode_success())
      {
        while(counter)
        {
          if(rgu_is_dram_slf())
          {
            g_ddr_reserve_success = 1;
            break;
          }
          counter--;
        }
        if(counter == 0)
        {
          print("[DDR Reserve] ddr reserve mode success but DRAM not in self-refresh!\n");
          g_ddr_reserve_success = 0;
        }
     }
     else
     {
        print("[DDR Reserve] ddr reserve mode FAIL!\n");
        g_ddr_reserve_success = 0;
     }

      /* release dram, no matter success or failed */
      release_dram();
    }
    else
    {
      print("[DDR Reserve] ddr reserve mode not be enabled yet\n");
      g_ddr_reserve_enable = 0;
    }
#endif
}

#ifdef REXTDN_ENABLE
void ett_rextdn_sw_calibration()
{
    unsigned int tmp;
    unsigned int sel;
    printf("Start REXTDN SW calibration...\n");

    /* 1.initialization */

    /* disable power down mode */
// NA for D123
#if 0
    tmp = (DRAMC_READ_REG(0x01e4)&0xffffdfff);
    DRAMC_WRITE_REG(tmp,0x01e4) ;
#ifndef RELEASE
    print("PD 0x01e4[13]:%xh\n",DRAMC_READ_REG(0x01e4));
#endif
#endif

    /* 2.DRVP calibration */

    /*enable P drive calibration*/
    // CAL_ENP = 1
    // 0x0644[9] (maoauo, reg map not corrent)
    tmp = (DRAMC_READ_REG(0x0644) |0x00000200);
    DRAMC_WRITE_REG(tmp,0x0644);

#ifndef RELEASE
    printf("enable P drive (initial settings), 0x0644:%xh\n", DRAMC_READ_REG(0x0644));
#endif

    // delay 1us
    gpt_busy_wait_us(1);

    /*Start P drive calibration*/
    for(drvp = 0 ; drvp <=15; drvp ++)
    {
        tmp = (DRAMC_READ_REG(0x00c0)&0xffff0fff)|(drvp << 12);
        DRAMC_WRITE_REG(tmp,0x00c0);
#ifndef RELEASE
        printf("2.1. DRVP 0x00c0[15:12]:%xh\n", tmp);
#endif

        // wait at least 100ns
        gpt_busy_wait_us(1);

        /* check the 3dc[31] from 0 to 1 */
#ifndef RELEASE
        printf("2.2. CMPOP 0x03dc[31]:%xh\n", DRAMC_READ_REG(0x03dc));
#endif

        if ((DRAMC_READ_REG(0x3dc) >> 31)  == 1)
        {
#ifndef RELEASE
            print("P drive:%d\n",drvp);
#endif
            break;
        }
    }

    // CAL_ENP = 0
    tmp = (DRAMC_READ_REG(0x0644) & 0xfffffdff);
    DRAMC_WRITE_REG(tmp,0x0644);

    if (drvp == 16)
    {
        drvp = 10; // back to HW defult
#ifndef RELEASE
        printf("No valid P drive\n");
#endif
    }

    /* 3.DRVN calibration */
    /*enable N drive calibration*/

    /* Enable N drive calibration */
    // CAL_ENN = 1
    // 0x0644[8] (maoauo, reg map not corrent)
    tmp = (DRAMC_READ_REG(0x0644) |0x00000100);
    DRAMC_WRITE_REG(tmp,0x0644);

#ifndef RELEASE
    printf("enable N drive (initial settings), 0x0644:%xh\n", DRAMC_READ_REG(0x0644));
#endif

    // delay 1us
    gpt_busy_wait_us(1);

    /*Start N drive calibration*/
    for(drvn = 0 ; drvn <=15; drvn ++)
    {
        tmp = (DRAMC_READ_REG(0x00c0)&0xfffff0ff)|(drvn<<8);
        DRAMC_WRITE_REG(tmp,0x00c0);

#ifndef RELEASE
        printf("4.1. DRVN 0x00c0[11:8]:%xh\n", tmp);
#endif

        // wait at least 100ns
        gpt_busy_wait_us(1);

        /* check the 3dc[30] from 0 to 1 */
#ifndef RELEASE
        printf("4.2.CMPON 0x3dc[30]:%xh\n", DRAMC_READ_REG(0x3dc));
#endif
        if ((DRAMC_READ_REG(0x3dc) >> 30)  == 1)
        {
            /* fixup the drvn by minus 1 */
            if (drvn > 0)
                drvn--;
#ifndef RELEASE
            printf("N drive:%d\n",drvn);
#endif
            break;
        }
    }

    // CAL_ENN = 0
    tmp = (DRAMC_READ_REG(0x0644) & 0xfffffeff);
    DRAMC_WRITE_REG(tmp,0x0644);

    if (drvn == 16)
    {
        drvn = 10; // back to default
#ifndef RELEASE
        printf("No valid N drive\n");
#endif
    }

#ifndef RELEASE
    printf("drvp=%d,drvn=%d\n",drvp,drvn);
#else
    printf("p %d,n %d\n",drvp,drvn);
#endif
    return;
}
#endif

#ifdef DRAMC_DEBUG
void print_DQS_DQ_tx_delay(void)
{
  unsigned int DQSbit;
  unsigned int DQbit;
  for(DQSbit=0;DQSbit<4;DQSbit++)
  {
    printf("\n");
    printf("[MTK DEBUG] DQS%d_OUTDLY:0x%x\n",DQSbit,((DRAMC_READ_REG(DRAMC_PADCTL3) & (0xf<<(DQSbit<<2)))>>(DQSbit<<2)));
    printf("[MTK DEBUG] DQM%d_OUTODLY:0x%x\n",DQSbit,((DRAMC_READ_REG(DRAMC_PADCTL2) & (0xf<<(DQSbit<<2)))>>(DQSbit<<2)));
    for(DQbit=0;DQbit<8;DQbit++)
    {
      if(DQbit%4==0)
      {
        printf("[MTK DEBUG]");
      }
      printf(" DQ%d_OUTDLY:0x%x",(DQSbit<<3)+DQbit,(DRAMC_READ_REG(DRAMC_DQODLY1+(DQSbit<<2))& (0xf<<(DQbit<<2))) >> (DQbit<<2));
      if(DQbit%4==3)
      {
        printf("\n");
      }
    }
  }
}
#endif

