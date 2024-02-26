#include <typedefs.h>
#include <msdc.h>
#include <partition.h>
#include <pll.h>
#include <sec_efuse.h>
#include <pmic.h>
#include <regulator/mtk_regulator.h>
#include <mt6357.h>

/**************************************************************
 * Constant
 **************************************************************/

#define EFUSE_PMIC_CONTROL_ENABLE   1

#define EFUSE_VBAT_LIMIT            3700

#define EFUSE_BLOW_UVOL             1050000 /* 1.05V */
#define EFUSE_READ_UVOL_HIGH        1050000 /* 1.05V */
#define EFUSE_READ_UVOL_LOW         1050000 /* 1.05V */

#define EFUSE_VSRAM_BLOW_UVOL       1150000 /* 1.15V (require 0.1V higher than VCORE) */
#define EFUSE_VSRAM_READ_UVOL_HIGH  1150000 /* 1.15V (require 0.1V higher than VCORE) */
#define EFUSE_VSRAM_READ_UVOL_LOW   1150000 /* 1.15V (require 0.1V higher than VCORE) */


static EFUSE_PART_INFO g_efuse_part_info;
static const struct mtk_regulator empty_regulator;

/**************************************************************
 * Partition
 **************************************************************/
int efuse_storage_init(void)
{
	part_t *part_ptr;

	/* bldr_pre_process has already done the part_init(). Thus, no need to do it again. */

	part_ptr = (part_t *)part_get("efuse");

	if (!part_ptr) {

		print("[%s] Error: Can't get partition data\n", "EFUSE");

		return -1;

	} else {

		if (NULL == (g_efuse_part_info.bootdev = blkdev_get(CFG_BOOT_DEV)))
		{
			print("[%s] Error: Can't find boot device(%d)\n", "EFUSE", CFG_BOOT_DEV);

			return -1;
		}

		g_efuse_part_info.part_id = part_ptr->part_id;

#if ((CFG_BOOT_DEV == BOOTDEV_SDMMC) || (CFG_BOOT_DEV == BOOTDEV_UFS))
		g_efuse_part_info.part_base = (U64)(part_ptr->start_sect) * (U64)(g_efuse_part_info.bootdev->blksz);
#else
		g_efuse_part_info.part_base = (U64)(mt_part_get_start_addr(part_ptr));
#endif

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

	struct mtk_regulator reg_vcore = empty_regulator;
	struct mtk_regulator reg_vsram_others = empty_regulator;

	/* ---------- Control the vcore ---------- */
	ret = mtk_regulator_get("vcore", &reg_vcore);
	if (ret)
		print("[EFUSE] regulator_get reg_vcore failed\n");

	/*
	 * The function mtk_regulator_set_voltage selects the lowest matching VCORE
	 * from the given min_uV to max_uV. It is suggested to set max to vcore_max_uV.
	 */
	ret |= mtk_regulator_set_voltage(&reg_vcore, EFUSE_BLOW_UVOL, vcore_max_uV);
	if (ret)
		print("[EFUSE] regulator_set_voltage reg_vcore failed\n");

	print("[EFUSE] Scale vcore to %d [enabled=%d]\n",
		mtk_regulator_get_voltage(&reg_vcore), mtk_regulator_is_enabled(&reg_vcore));

	/* ---------- Control the vsram_others ---------- */
	ret = mtk_regulator_get("vsram_others", &reg_vsram_others);
	if (ret)
		print("[EFUSE] regulator_get reg_vsram_others failed\n");

	/*
	 * The function mtk_regulator_set_voltage selects the lowest matching vsram_others
	 * from the given min_uV to max_uV. It is suggested to set max to vsram_others_max_uV.
	 */
	ret |= mtk_regulator_set_voltage(&reg_vsram_others, EFUSE_VSRAM_BLOW_UVOL, vsram_others_max_uV);
	if (ret)
		print("[EFUSE] regulator_set_voltage reg_vsram_others failed\n");

	print("[EFUSE] Scale vsram_others to %d [enabled=%d]\n",
		mtk_regulator_get_voltage(&reg_vsram_others), mtk_regulator_is_enabled(&reg_vsram_others));

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
	int ret = 0;

#if EFUSE_PMIC_CONTROL_ENABLE

	struct mtk_regulator reg_vcore = empty_regulator;
	struct mtk_regulator reg_vsram_others = empty_regulator;

	/* ---------- Control the vcore ---------- */
	ret = mtk_regulator_get("vcore", &reg_vcore);
	if (ret)
		print("[EFUSE] regulator_get reg_vcore failed\n");

	/*
	 * The function mtk_regulator_set_voltage selects the lowest matching VCORE
	 * from the given min_uV to max_uV. It is suggested to set max to vcore_max_uV.
	 */
	ret |= mtk_regulator_set_voltage(&reg_vcore, EFUSE_READ_UVOL_HIGH, vcore_max_uV);
	if (ret)
		print("[EFUSE] regulator_set_voltage reg_vcore failed\n");

	print("[EFUSE] Scale vcore to %d [enabled=%d]\n",
		mtk_regulator_get_voltage(&reg_vcore), mtk_regulator_is_enabled(&reg_vcore));

	/* ---------- Control the vsram_others ---------- */
	ret = mtk_regulator_get("vsram_others", &reg_vsram_others);
	if (ret)
		print("[EFUSE] regulator_get reg_vsram_others failed\n");

	/*
	 * The function mtk_regulator_set_voltage selects the lowest matching vsram_others
	 * from the given min_uV to max_uV. It is suggested to set max to vsram_others_max_uV.
	 */
	ret |= mtk_regulator_set_voltage(&reg_vsram_others, EFUSE_VSRAM_READ_UVOL_HIGH, vsram_others_max_uV);
	if (ret)
		print("[EFUSE] regulator_set_voltage reg_vsram_others failed\n");

	print("[EFUSE] Scale vsram_others to %d [enabled=%d]\n",
		mtk_regulator_get_voltage(&reg_vsram_others), mtk_regulator_is_enabled(&reg_vsram_others));

	/*
	 * VCORE and VSRAM are enabled by default.
	 * No need to enable VCORE and VSRAM again.
	 * The system cannot work with VCORE or VSRAM disabled.
	 */

	mdelay(10);

#endif

	return ret;
}

U32 efuse_vcore_low(void)
{
	int ret = 0;

#if EFUSE_PMIC_CONTROL_ENABLE

	struct mtk_regulator reg_vcore = empty_regulator;
	struct mtk_regulator reg_vsram_others = empty_regulator;

	/* ---------- Control the vcore ---------- */
	ret = mtk_regulator_get("vcore", &reg_vcore);
	if (ret)
		print("[EFUSE] regulator_get reg_vcore failed\n");

	/*
	 * The function mtk_regulator_set_voltage selects the lowest matching VCORE
	 * from the given min_uV to max_uV. It is suggested to set max to vcore_max_uV.
	 */
	ret |= mtk_regulator_set_voltage(&reg_vcore, EFUSE_READ_UVOL_LOW, vcore_max_uV);
	if (ret)
		print("[EFUSE] regulator_set_voltage reg_vcore failed\n");

	print("[EFUSE] Scale vcore to %d [enabled=%d]\n",
		mtk_regulator_get_voltage(&reg_vcore), mtk_regulator_is_enabled(&reg_vcore));

	/* ---------- Control the vsram_others ---------- */
	ret = mtk_regulator_get("vsram_others", &reg_vsram_others);
	if (ret)
		print("[EFUSE] regulator_get reg_vsram_others failed\n");

	/*
	 * The function mtk_regulator_set_voltage selects the lowest matching vsram_others
	 * from the given min_uV to max_uV. It is suggested to set max to vsram_others_max_uV.
	 */
	ret |= mtk_regulator_set_voltage(&reg_vsram_others, EFUSE_VSRAM_READ_UVOL_LOW, vsram_others_max_uV);
	if (ret)
		print("[EFUSE] regulator_set_voltage reg_vsram_others failed\n");

	print("[EFUSE] Scale vsram_others to %d [enabled=%d]\n",
		mtk_regulator_get_voltage(&reg_vsram_others), mtk_regulator_is_enabled(&reg_vsram_others));

	/*
	 * VCORE and VSRAM are enabled by default.
	 * No need to enable VCORE and VSRAM again.
	 * The system cannot work with VCORE or VSRAM disabled.
	 */

	mdelay(10);

#endif

	return ret;
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


