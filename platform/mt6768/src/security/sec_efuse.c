/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2019. All rights reserved.
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
 * have been modified by MediaTek Inc. All revisions are subject to any receiver\'s
 * applicable license agreements with MediaTek Inc.
 */

#include <typedefs.h>
#include <msdc.h>
#include <partition.h>
#include <pll.h>
#include <sec_efuse.h>
#include <pmic.h>
#include <regulator/mtk_regulator.h>
#include <mt6358.h>
#include <pal_log.h>

#define EFUSE_PMIC_CONTROL_ENABLE   1

#define EFUSE_VBAT_LIMIT            3700

#define EFUSE_BLOW_UVOL             800000 /* 0.8V */
#define EFUSE_READ_UVOL_HIGH        800000 /* 0.8V */
#define EFUSE_READ_UVOL_LOW         800000 /* 0.8V */
#define EFUSE_VSRAM_UVOL            900000 /* 0.9V (require 0.1V higher than VCORE) */

static EFUSE_PART_INFO g_efuse_part_info;

/**************************************************************
 * Partition
 **************************************************************/
int efuse_storage_init(void)
{
	part_t *part_ptr;

	part_init();

	part_ptr = (part_t *)part_get("efuse");

	if (!part_ptr) {

		pal_log_info("[%s] Error: Can't get partition data\n", "EFUSE");

		return -1;

	} else {

		if (NULL == (g_efuse_part_info.bootdev = blkdev_get(CFG_BOOT_DEV)))
		{
			pal_log_info("[%s] Error: Can't find boot device(%d)\n", "EFUSE", CFG_BOOT_DEV);

			return -1;
		}

		g_efuse_part_info.part_id = part_ptr->part_id;

		g_efuse_part_info.part_base = (U64)(part_ptr->start_sect) * (U64)(g_efuse_part_info.bootdev->blksz);

		g_efuse_part_info.initialized = TRUE;

		return 0;

	}
}

/**************************************************************
 * Storage
 **************************************************************/
int efuse_storage_read(U64 efuse_part_start_offset, U32 size, U8 *dst)
{
	int ret;

	if (!g_efuse_part_info.initialized) {
		ret = efuse_storage_init();
		if (ret) {
			return ret;
		}
	}

	ret = blkdev_read(g_efuse_part_info.bootdev,
			(g_efuse_part_info.part_base + efuse_part_start_offset),
			size,
			dst,
			g_efuse_part_info.part_id);

	return ret;
}

int efuse_storage_write(U64 efuse_part_start_offset, U32 size, U8 *src)
{
	int ret;

	if (!g_efuse_part_info.initialized) {
		ret = efuse_storage_init();
		if (ret) {
			return ret;
		}
	}

	ret = blkdev_write(g_efuse_part_info.bootdev,
			(g_efuse_part_info.part_base + efuse_part_start_offset),
			size,
			src,
			g_efuse_part_info.part_id);

	return ret;
}

/**************************************************************
 * WDT
 **************************************************************/
void efuse_wdt_init(void)
{
	mtk_wdt_init();
}

void efuse_wdt_restart(void)
{
	mtk_wdt_restart();
}

void efuse_wdt_sw_reset(void)
{
	platform_safe_mode(0,0);
	mtk_wdt_sw_reset();
}

/**************************************************************
 * DDR reserved mode
 **************************************************************/
int efuse_dram_reserved(int enable)
{
	/* return 0: success, -1: fail */
	return rgu_dram_reserved(enable);
}

/**************************************************************
 * PLL
 **************************************************************/
void efuse_pll_set(void)
{
	mt_set_topck_default();
}


/**************************************************************
 * Vbat
 **************************************************************/
int efuse_check_lowbat(void)
{
	int volt;

	volt = get_bat_sense_volt(5);
	if (volt < EFUSE_VBAT_LIMIT)
		return 1;
	else
		return 0;
}

/****************************************************
 * Fsource
 * return 0 : success
 ****************************************************/
U32 efuse_fsource_set(void)
{
	U32 ret_val = 0;

#if EFUSE_PMIC_CONTROL_ENABLE

	/* 1.8V */
	ret_val |= pmic_config_interface((kal_uint32)(PMIC_RG_VEFUSE_VOSEL_ADDR),
			(kal_uint32)(0xC),
			(kal_uint32)(PMIC_RG_VEFUSE_VOSEL_MASK),
			(kal_uint32)(PMIC_RG_VEFUSE_VOSEL_SHIFT)
			);

	/* +0mV */
	ret_val |= pmic_config_interface((kal_uint32)(PMIC_RG_VEFUSE_VOCAL_ADDR),
			(kal_uint32)(0x0),
			(kal_uint32)(PMIC_RG_VEFUSE_VOCAL_MASK),
			(kal_uint32)(PMIC_RG_VEFUSE_VOCAL_SHIFT)
			);

	/* Fsource(VEFUSE) enabled */
	ret_val |= pmic_config_interface((kal_uint32)(PMIC_RG_LDO_VEFUSE_EN_ADDR),
			(kal_uint32)(0x1),
			(kal_uint32)(PMIC_RG_LDO_VEFUSE_EN_MASK),
			(kal_uint32)(PMIC_RG_LDO_VEFUSE_EN_SHIFT)
			);

	mdelay(10);

#endif

	return ret_val;
}

U32 efuse_fsource_close(void)
{
	U32 ret_val = 0;

#if EFUSE_PMIC_CONTROL_ENABLE

	/* Fsource(VEFUSE) disable */
	ret_val |= pmic_config_interface((kal_uint32)(PMIC_RG_LDO_VEFUSE_EN_ADDR),
			(kal_uint32)(0x0),
			(kal_uint32)(PMIC_RG_LDO_VEFUSE_EN_MASK),
			(kal_uint32)(PMIC_RG_LDO_VEFUSE_EN_SHIFT)
			);

	mdelay(10);

#endif

	return ret_val;
}

U32 efuse_fsource_is_enabled(void)
{
	U32 regVal = 0;

#if EFUSE_PMIC_CONTROL_ENABLE

	/*  Check Fsource(VEFUSE) Status */
	pmic_read_interface((kal_uint32)(PMIC_RG_LDO_VEFUSE_EN_ADDR),
			&regVal,
			(kal_uint32)(PMIC_RG_LDO_VEFUSE_EN_MASK),
			(kal_uint32)(PMIC_RG_LDO_VEFUSE_EN_SHIFT)
			);

	/* return 1 : fsource enabled
	 * return 0 : fsource disabled */

#endif

	return regVal;
}

/**************************************************************
 * Vcore
 **************************************************************/
U32 efuse_vcore_blow(void)
{
	int ret = 0;

#if EFUSE_PMIC_CONTROL_ENABLE

	struct mtk_regulator reg_vcore;
	struct mtk_regulator reg_vsram_others;

	/* ---------- Control the vsram_others ---------- */
	/* Required to set VSRAM to 0.85V in MT6768 because VSRAM must be 0.1V higher than VCORE */
	ret = mtk_regulator_get("vsram_others", &reg_vsram_others);
	if (ret)
		pal_log_info("Err: vg1\n");
	else {
		/*
		 * The function mtk_regulator_set_voltage selects the lowest matching vsram_others
		 * from the given min_uV to max_uV. It is suggested to set max to vsram_others_max_uV.
		 */
		ret = mtk_regulator_set_voltage(&reg_vsram_others, EFUSE_VSRAM_UVOL, vsram_others_max_uV);
		if (ret)
			pal_log_info("Err: vs1\n");

		pal_log_info("vc1 %d, %d\n",
			mtk_regulator_get_voltage(&reg_vsram_others), mtk_regulator_is_enabled(&reg_vsram_others));
	}


	/* ---------- Control the vcore ---------- */
	ret = mtk_regulator_get("vcore", &reg_vcore);
	if (ret)
		pal_log_info("Err: vg2\n");
	else {
		/*
		 * The function mtk_regulator_set_voltage selects the lowest matching VCORE
		 * from the given min_uV to max_uV. It is suggested to set max to vcore_max_uV.
		 */
		ret = mtk_regulator_set_voltage(&reg_vcore, EFUSE_BLOW_UVOL, vcore_max_uV);
		if (ret)
			pal_log_info("Err: vs2\n");

		pal_log_info("vc2 %d, %d\n",
			mtk_regulator_get_voltage(&reg_vcore), mtk_regulator_is_enabled(&reg_vcore));
	}

	/*
	 * VCORE and VSRAM are enabled by default.
	 * No need to enable VCORE and VSRAM again.
	 * The system cannot work with VCORE or VSRAM disabled.
	 */

	mdelay(10);

#endif

	return ret;
}

U32 efuse_vcore_high(void)
{
	return efuse_vcore_blow();
}

U32 efuse_vcore_low(void)
{
	return efuse_vcore_blow();
}

/**************************************************************
 * Others
 **************************************************************/
/* re-initial modules after declinie clock */
int efuse_module_reinit(void)
{

	/*
	 * No need to do pwrap_init again because platform_pre_init -> pwrap_init_preloader -> pwrap_init
	 * has already do that!
	 */

	return 0;
}


