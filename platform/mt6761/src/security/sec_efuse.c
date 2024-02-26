#include <typedefs.h>
#include <msdc.h>
#include <partition.h>
#include <pll.h>
#include <sec_efuse.h>
#include <pmic.h>
#include <regulator/mtk_regulator.h>
#include <mt6356.h>

/**************************************************************
 * Constant
 **************************************************************/

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

		print("Err: get part\n");

		return -1;

	} else {

		if (NULL == (g_efuse_part_info.bootdev = blkdev_get(CFG_BOOT_DEV)))
		{
			print("Err: find bdev\n");

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
void efuse_wdt_restart(void)
{
	mtk_wdt_restart();
}

void efuse_wdt_sw_reset(void)
{
       platform_safe_mode(0, 0);
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
			(kal_uint32)(0x4),
			(kal_uint32)(PMIC_RG_VEFUSE_VOSEL_MASK),
			(kal_uint32)(PMIC_RG_VEFUSE_VOSEL_SHIFT)
			);

	/* +0mV */
	ret_val |= pmic_config_interface((kal_uint32)(PMIC_RG_VEFUSE_VOCAL_ADDR),
			(kal_uint32)(0x0),
			(kal_uint32)(PMIC_RG_VEFUSE_VOCAL_MASK),
			(kal_uint32)(PMIC_RG_VEFUSE_VOCAL_SHIFT)
			);

	/* Fsource(VEFUSE or VMIPI) enabled */
	ret_val |= pmic_config_interface((kal_uint32)(PMIC_RG_LDO_VEFUSE_EN_ADDR),
			(kal_uint32)(1),
			(kal_uint32)(PMIC_RG_LDO_VEFUSE_EN_MASK),
			(kal_uint32)(PMIC_RG_LDO_VEFUSE_EN_SHIFT));

	mdelay(10);

#endif

	return ret_val;
}

U32 efuse_fsource_close(void)
{
	U32 ret_val = 0;

#if EFUSE_PMIC_CONTROL_ENABLE

	/* Fsource(VEFUSE or VMIPI) disabled */
	ret_val |= pmic_config_interface((kal_uint32)(PMIC_RG_LDO_VEFUSE_EN_ADDR),
			(kal_uint32)(0),
			(kal_uint32)(PMIC_RG_LDO_VEFUSE_EN_MASK),
			(kal_uint32)(PMIC_RG_LDO_VEFUSE_EN_SHIFT));

	mdelay(10);

#endif

	return ret_val;
}

U32 efuse_fsource_is_enabled(void)
{
	U32 regVal = 0;

#if EFUSE_PMIC_CONTROL_ENABLE

	/*  Check Fsource(VEFUSE or VMIPI) Status */
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

	struct mtk_regulator reg_vsram_others;
	struct mtk_regulator reg_vcore;


	if (mtk_regulator_get("vsram_others", &reg_vsram_others)) {
		return -1;
	}

	if (mtk_regulator_get("vcore", &reg_vcore)) {
		return -1;
	}

	if (mtk_regulator_get_voltage(&reg_vcore) == EFUSE_BLOW_UVOL &&
		mtk_regulator_get_voltage(&reg_vsram_others) == EFUSE_VSRAM_UVOL) {
		print("EFUSE VCORE already in blow range\n");
		return 0;
	}

	if (mtk_regulator_set_voltage(&reg_vsram_others, EFUSE_VSRAM_UVOL, vsram_others_max_uV)) {
		return -2;
	}

	if (mtk_regulator_set_voltage(&reg_vcore, EFUSE_BLOW_UVOL, vcore_max_uV)) {
		return -2;
	}

        mdelay(10);

        /* return 1 if PMIC config is fail because the function returns unsigned int */
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


