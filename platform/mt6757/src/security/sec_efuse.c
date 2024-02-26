#include "typedefs.h"
#include "pmic.h"
#include "msdc.h"
#include "partition.h"
#include "pll.h"
#include "sec_efuse.h"

#define EFUSE_VBAT_LIMIT            3700
#define EFUSE_PART                  "efuse"

#if defined(PMIC_CHIP_MT6355)

#define EFUSE_BLOW_UVOL             800000 /* 0.8V */
#define EFUSE_READ_UVOL_HIGH        800000 /* 0.8V */
#define EFUSE_READ_UVOL_LOW         725000 /* 0.725V */

#else
/* PMIC: MT6351 */

#define EFUSE_BLOW_VOL              PMIC_VOLT_00_800000_V /* 0.8V */
#define EFUSE_READ_VOL_HIGH         PMIC_VOLT_00_800000_V /* 0.8V */
#define EFUSE_READ_VOL_LOW          PMIC_VOLT_00_725000_V /* 0.725V */
#define EFUSE_BLOW_VOL_UPMU         0x20 /* 0.8V */
#define EFUSE_READ_VOL_HIGH_UPMU    0x20 /* 0.8V */
#define EFUSE_READ_VOL_LOW_UPMU     0x14 /* 0.725V */

#endif

#define EFUSE_STORAGE_NUM           0

/**************************************************************
 * Partition
 **************************************************************/
int efuse_part_get_base(unsigned long *base)
{
	part_t *part_ptr;

	part_init();

	part_ptr = part_get(EFUSE_PART);
	if (!part_ptr) {
		print("[%s] Error: Can't get partition base address\n", "EFUSE");
		return -1;
	} else {
		*base = part_ptr->start_sect;
		return 0;
	}
}

/**************************************************************
 * Storage
 **************************************************************/
int efuse_storage_init(void)
{
	int ret;

	ret = mmc_init(EFUSE_STORAGE_NUM, MSDC_MODE_DEFAULT);
	return ret;
}

int efuse_storage_read(unsigned long blknr, U32 blkcnt, unsigned long *dst)
{
	int ret;

	ret = mmc_block_read(EFUSE_STORAGE_NUM, blknr, blkcnt, dst);
	return ret;
}

int efuse_storage_write(unsigned long blknr, U32 blkcnt, unsigned long *src)
{
	int ret;

	ret = mmc_block_write(EFUSE_STORAGE_NUM, blknr, blkcnt, src);
	return ret;
}

/**************************************************************
 * WDT
 **************************************************************/
void efuse_wdt_init(void)
{
	mtk_wdt_init();
}

void efuse_wdt_disable(void)
{
	mtk_wdt_disable();
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

#if defined(PMIC_CHIP_MT6355)

	/* 1.8V */
	ret_val |= pmic_config_interface((kal_uint32)(PMIC_RG_VGP_VOSEL_ADDR),
			(kal_uint32)(0x4),
			(kal_uint32)(PMIC_RG_VGP_VOSEL_MASK),
			(kal_uint32)(PMIC_RG_VGP_VOSEL_SHIFT)
			);

	/* +60mV */
	ret_val |= pmic_config_interface((kal_uint32)(PMIC_RG_VGP_VOCAL_ADDR),
			(kal_uint32)(0x6),
			(kal_uint32)(PMIC_RG_VGP_VOCAL_MASK),
			(kal_uint32)(PMIC_RG_VGP_VOCAL_SHIFT)
			);

	/* Fsource enable */
	ret_val |= pmic_config_interface((kal_uint32)(PMIC_RG_LDO_VGP_EN_ADDR),
			(kal_uint32)(0x1),
			(kal_uint32)(PMIC_RG_LDO_VGP_EN_MASK),
			(kal_uint32)(PMIC_RG_LDO_VGP_EN_SHIFT)
			);

#else
	/* PMIC: MT6351 */

	/* RG_EFUSE_MODE_1 = VEFUSE */
	ret_val |= pmic_config_interface((kal_uint32)(MT6351_PMIC_RG_EFUSE_MODE_1_ADDR),
			(kal_uint32)(0x1),
			(kal_uint32)(MT6351_PMIC_RG_EFUSE_MODE_1_MASK),
			(kal_uint32)(MT6351_PMIC_RG_EFUSE_MODE_1_SHIFT)
			);

	/* 1.8V */
	ret_val |= pmic_config_interface((kal_uint32)(MT6351_PMIC_RG_VEFUSE_VOSEL_ADDR),
			(kal_uint32)(0x3),
			(kal_uint32)(MT6351_PMIC_RG_VEFUSE_VOSEL_MASK),
			(kal_uint32)(MT6351_PMIC_RG_VEFUSE_VOSEL_SHIFT)
			);

	/* +60mV */
	ret_val |= pmic_config_interface((kal_uint32)(MT6351_PMIC_RG_VEFUSE_CAL_ADDR),
			(kal_uint32)(0xD),
			(kal_uint32)(MT6351_PMIC_RG_VEFUSE_CAL_MASK),
			(kal_uint32)(MT6351_PMIC_RG_VEFUSE_CAL_SHIFT)
			);

	/* Fsource enable */
	ret_val |= pmic_config_interface((kal_uint32)(MT6351_PMIC_RG_VEFUSE_EN_ADDR),
			(kal_uint32)(0x1),
			(kal_uint32)(MT6351_PMIC_RG_VEFUSE_EN_MASK),
			(kal_uint32)(MT6351_PMIC_RG_VEFUSE_EN_SHIFT)
			);
#endif

	mdelay(10);

	return ret_val;
}

U32 efuse_fsource_close(void)
{
	U32 ret_val = 0;

#if defined(PMIC_CHIP_MT6355)
	ret_val |= pmic_config_interface((kal_uint32)(PMIC_RG_LDO_VGP_EN_ADDR),
			(kal_uint32)(0x0),
			(kal_uint32)(PMIC_RG_LDO_VGP_EN_MASK),
			(kal_uint32)(PMIC_RG_LDO_VGP_EN_SHIFT)
			);
#else
	/* PMIC: MT6351 */
	ret_val |= pmic_config_interface((kal_uint32)(MT6351_PMIC_RG_VEFUSE_EN_ADDR),
			(kal_uint32)(0x0),
			(kal_uint32)(MT6351_PMIC_RG_VEFUSE_EN_MASK),
			(kal_uint32)(MT6351_PMIC_RG_VEFUSE_EN_SHIFT)
			);
#endif

	mdelay(10);

	return ret_val;
}

U32 efuse_fsource_is_enabled(void)
{
	U32 regVal = 0;

#if defined(PMIC_CHIP_MT6355)
	pmic_read_interface((kal_uint32)(PMIC_RG_LDO_VGP_EN_ADDR),
			&regVal,
			(kal_uint32)(PMIC_RG_LDO_VGP_EN_MASK),
			(kal_uint32)(PMIC_RG_LDO_VGP_EN_SHIFT)
			);
#else
	/* PMIC: MT6351 */
	pmic_read_interface((kal_uint32)(MT6351_PMIC_RG_VEFUSE_EN_ADDR),
			&regVal,
			(kal_uint32)(MT6351_PMIC_RG_VEFUSE_EN_MASK),
			(kal_uint32)(MT6351_PMIC_RG_VEFUSE_EN_SHIFT)
			);
#endif

	/* return 1 : fsource enabled
	 * return 0 : fsource disabled */

	return regVal;
}

/**************************************************************
 * Vcore
 **************************************************************/
U32 efuse_vcore_blow(void)
{
	U32 ret = 0;

#if defined(PMIC_CHIP_MT6355)

	struct regulator *reg_vcore;
	reg_vcore = (struct regulator *)regulator_get(NULL, "vcore");
	if (!reg_vcore) {
		print("[EFUSE] regulator_get reg_vcore failed\n");
		return 1;
	}

	/*
	 * The function regulator_set_voltage selects the lowest matching VCORE
	 * from the given min_uV to max_uV. It is suggested to set max to vcore_max_uV.
	 */
	ret |= regulator_set_voltage(reg_vcore, EFUSE_BLOW_UVOL, vcore_max_uV);
	if (ret)
		print("[EFUSE] regulator_set_voltage reg_vcore failed\n");

	/*
	 * VCORE is enabled by default. Enable again to make sure it is not being disabled.
	 * Enable VCORE multiple times will not cause any side-effect.
	 */
	ret |= regulator_enable(reg_vcore);
	if (ret)
		print("[EFUSE] regulator_enable reg_vcore failed\n");

	regulator_put(reg_vcore);

#else
	/* PMIC: MT6351 */

	/*
	 * ret |= upmu_set_reg_value(MT6351_PMIC_BUCK_VCORE_VOSEL_ADDR , EFUSE_BLOW_VOL_UPMU);
	 * ret |= upmu_set_reg_value(MT6351_PMIC_BUCK_VCORE_VOSEL_ON_ADDR , EFUSE_BLOW_VOL_UPMU);
	 *
	 * The "upmu_set_reg_value" function has the same feature as buck_adjust_voltage,
	 * but it provides more fine-grained VCORE setting.
	 * VCORE = 0.6 + (0.00625 * value)
	 */

	ret = buck_adjust_voltage(VCORE, EFUSE_BLOW_VOL);
#endif

	mdelay(10);

	return ret;
}

U32 efuse_vcore_high(void)
{
	U32 ret = 0;

#if defined(PMIC_CHIP_MT6355)

	struct regulator *reg_vcore;
	reg_vcore = (struct regulator *)regulator_get(NULL, "vcore");
	if (!reg_vcore) {
		print("[EFUSE] regulator_get reg_vcore failed\n");
		return 1;
	}

	/*
	 * The function regulator_set_voltage selects the lowest matching VCORE
	 * from the given min_uV to max_uV. It is suggested to set max to vcore_max_uV.
	 */
	ret |= regulator_set_voltage(reg_vcore, EFUSE_READ_UVOL_HIGH, vcore_max_uV);
	if (ret)
		print("[EFUSE] regulator_set_voltage reg_vcore failed\n");

	/*
	 * VCORE is enabled by default. Enable again to make sure it is not being disabled.
	 * Enable VCORE multiple times will not cause any side-effect.
	 */
	ret |= regulator_enable(reg_vcore);
	if (ret)
		print("[EFUSE] regulator_enable reg_vcore failed\n");

	regulator_put(reg_vcore);

#else
	/* PMIC: MT6351 */

	/*
	 * ret |= upmu_set_reg_value(MT6351_PMIC_BUCK_VCORE_VOSEL_ADDR , EFUSE_READ_VOL_HIGH_UPMU);
	 * ret |= upmu_set_reg_value(MT6351_PMIC_BUCK_VCORE_VOSEL_ON_ADDR , EFUSE_READ_VOL_HIGH_UPMU);
	 *
	 * The "upmu_set_reg_value" function has the same feature as buck_adjust_voltage,
	 * but it provides more fine-grained VCORE setting.
	 * VCORE = 0.6 + (0.00625 * value)
	 */

	ret = buck_adjust_voltage(VCORE, EFUSE_READ_VOL_HIGH);
#endif

	mdelay(10);

	return ret;
}

U32 efuse_vcore_low(void)
{
	U32 ret = 0;

#if defined(PMIC_CHIP_MT6355)

	struct regulator *reg_vcore;
	reg_vcore = (struct regulator *)regulator_get(NULL, "vcore");
	if (!reg_vcore) {
		print("[EFUSE] regulator_get reg_vcore failed\n");
		return 1;
	}

	/*
	 * The function regulator_set_voltage selects the lowest matching VCORE
	 * from the given min_uV to max_uV. It is suggested to set max to vcore_max_uV.
	 */
	ret |= regulator_set_voltage(reg_vcore, EFUSE_READ_UVOL_LOW, vcore_max_uV);
	if (ret)
		print("[EFUSE] regulator_set_voltage reg_vcore failed\n");

	/*
	 * VCORE is enabled by default. Enable again to make sure it is not being disabled.
	 * Enable VCORE multiple times will not cause any side-effect.
	 */
	ret |= regulator_enable(reg_vcore);
	if (ret)
		print("[EFUSE] regulator_enable reg_vcore failed\n");

	regulator_put(reg_vcore);

#else
	/* PMIC: MT6351 */

	/*
	 * ret |= upmu_set_reg_value(MT6351_PMIC_BUCK_VCORE_VOSEL_ADDR , EFUSE_READ_VOL_LOW_UPMU);
	 * ret |= upmu_set_reg_value(MT6351_PMIC_BUCK_VCORE_VOSEL_ON_ADDR , EFUSE_READ_VOL_LOW_UPMU);
	 *
	 * The "upmu_set_reg_value" function has the same feature as buck_adjust_voltage,
	 * but it provides more fine-grained VCORE setting.
	 * VCORE = 0.6 + (0.00625 * value)
	 */

	ret = buck_adjust_voltage(VCORE, EFUSE_READ_VOL_LOW);
#endif

	mdelay(10);

	return ret;
}

/**************************************************************
 * Others
 **************************************************************/
/* re-initial modules after declinie clock */
int efuse_module_reinit(void)
{
	int ret;

	ret = pwrap_init();
	return ret;
}


