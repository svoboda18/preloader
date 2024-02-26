#include "typedefs.h"
#include "pmic.h"
#include "msdc.h"
#include "partition.h"
#include "pll.h"
#include "sec_efuse.h"
#include "gpio.h"

#define EFUSE_VBAT_LIMIT		3700
#define EFUSE_PART				"efuse"

#if defined (PMIC_CHIP_MT6353)
#define EFUSE_BLOW_VOL			PMIC_VOLT_00_900000_V
#define EFUSE_READ_VOL_HIGH		PMIC_VOLT_01_000000_V
#define EFUSE_READ_VOL_LOW		PMIC_VOLT_00_850000_V
#define EFUSE_STORAGE_NUM		0
#define EFUSE_VEFUSE_EN_DELAY	10
#define EFUSE_VEFUSE_DIS_DELAY	80
#else
#define EFUSE_BLOW_VOL			PMIC_VOLT_01_000000_V
#define EFUSE_READ_VOL_HIGH		PMIC_VOLT_01_000000_V
#define EFUSE_READ_VOL_LOW		PMIC_VOLT_00_900000_V
#define EFUSE_STORAGE_NUM		0
#define EFUSE_VEFUSE_EN_DELAY	10
#define EFUSE_VEFUSE_DIS_DELAY	10
#endif
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

void efuse_wdt_restart(void)
{
	mtk_wdt_restart();
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

#if defined (PMIC_CHIP_MT6353)
	mt_set_gpio_mode(GPIO_VEFUSE_LDO_EN, GPIO_MODE_GPIO);
	mt_set_gpio_dir(GPIO_VEFUSE_LDO_EN, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_VEFUSE_LDO_EN, GPIO_OUT_ONE);
#else
	ret_val |= pmic_config_interface((kal_uint32)(MT6351_PMIC_RG_EFUSE_MODE_1_ADDR),
			(kal_uint32)(0x1),
			(kal_uint32)(MT6351_PMIC_RG_EFUSE_MODE_1_MASK),
			(kal_uint32)(MT6351_PMIC_RG_EFUSE_MODE_1_SHIFT)
			);

	ret_val |= pmic_config_interface((kal_uint32)(MT6351_PMIC_RG_VEFUSE_VOSEL_ADDR),
			(kal_uint32)(0x3),
			(kal_uint32)(MT6351_PMIC_RG_VEFUSE_VOSEL_MASK),
			(kal_uint32)(MT6351_PMIC_RG_VEFUSE_VOSEL_SHIFT)
			);

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

	mdelay(EFUSE_VEFUSE_EN_DELAY);

	return ret_val;
}

U32 efuse_fsource_close(void)
{
	U32 ret_val = 0;

	/*  Fsource disable */
#if defined (PMIC_CHIP_MT6353)
	mt_set_gpio_out(GPIO_VEFUSE_LDO_EN, GPIO_OUT_ZERO);
#else
	ret_val |= pmic_config_interface((kal_uint32)(MT6351_PMIC_RG_VEFUSE_EN_ADDR),
			(kal_uint32)(0x0),
			(kal_uint32)(MT6351_PMIC_RG_VEFUSE_EN_MASK),
			(kal_uint32)(MT6351_PMIC_RG_VEFUSE_EN_SHIFT)
			);
#endif

	mdelay(EFUSE_VEFUSE_DIS_DELAY);

	return ret_val;
}

/**************************************************************
 * Vcore
 **************************************************************/
U32 efuse_vcore_blow(void)
{
	U32 ret = 0;

	ret = buck_adjust_voltage(VCORE, EFUSE_BLOW_VOL);

	mdelay(10);

	return ret;
}

U32 efuse_vcore_high(void)
{
	U32 ret = 0;

	ret = buck_adjust_voltage(VCORE, EFUSE_READ_VOL_HIGH);

	mdelay(10);

	return ret;
}

U32 efuse_vcore_low(void)
{
	U32 ret = 0;

	ret = buck_adjust_voltage(VCORE, EFUSE_READ_VOL_LOW);

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


