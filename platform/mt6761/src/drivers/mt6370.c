#include <i2c.h>
#include <mtk_subpmic.h>
#include "pal_log.h"

#define MT6370_PMU_PL_DRV_VERSION	"1.0.4_MTK"

#undef FAST_UNKNOWN_TA_DECT
#undef DISABLE_VLGC

#ifndef BIT
#define BIT(nr) (1 << (nr))
#endif

/* uV */
#define MT6370_BAT_VOREG_MIN		3900000
#define MT6370_BAT_VOREG_MAX		4710000
#define MT6370_BAT_VOREG_STEP		10000

#define MT6370_SLAVE_ADDR		(0x34)
#define MT6370_PMU_REG_CORECTRL1	(0x01)
#define MT6370_PMU_REG_CHGCTRL4		(0x14)
#define MT6370_PMU_REG_DEVICETYPE	(0x22)
#define MT6370_PMU_REG_USBSTATUS1	(0x27)
#define MT6370_PMU_REG_QCSTATUS1	(0x28)
#define MT6370_PMU_REG_DBCTRL2		(0xB1)
/* ========== CORECTRL1 0x01 ============ */
#define MT6370_SHIFT_MREN		4
#define MT6370_MASK_MREN		BIT(MT6370_SHIFT_MREN)
/* ========== CHGCTRL4 0x14 ============ */
#define MT6370_SHIFT_BAT_VOREG		1
#define MT6370_MASK_BAT_VOREG		0xFE
/* ========== DEVICETYPE 0x22 ============ */
#define MT6370_SHIFT_USBCHGEN		7
#define MT6370_MASK_USBCHGEN		BIT(MT6370_SHIFT_USBCHGEN)
/* ========== USBSTATUS1 0x27 ============ */
#define MT6370_SHIFT_USB_STATUS_OPT	7
#define MT6370_MASK_USB_STATUS_OPT	BIT(MT6370_SHIFT_USB_STATUS_OPT)
#define MT6370_SHIFT_USB_STATUS		4
#define MT6370_MASK_USB_STATUS		0x70
#define MT6370_SHIFT_DCDT		2
#define MT6370_MASK_DCDT		BIT(MT6370_SHIFT_DCDT)
/* ========== QCSTATUS1 0x28 ============ */
#define MT6370_SHIFT_VLGC		7
#define MT6370_MASK_VLGC		BIT(MT6370_SHIFT_VLGC)
/* ========== DBCTRL2 0xB1 ============ */
#define MT6370_SHIFT_DB_VPOS_GND	5
#define MT6370_MASK_DB_VPOS_GND		BIT(MT6370_SHIFT_DB_VPOS_GND)

enum mt6370_pmu_chg_type {
	MT6370_CHG_TYPE_NOVBUS = 0,
	MT6370_CHG_TYPE_UNDER_GOING,
	MT6370_CHG_TYPE_SDP,
	MT6370_CHG_TYPE_SDPNSTD,
	MT6370_CHG_TYPE_DCP,
	MT6370_CHG_TYPE_CDP,
	MT6370_CHG_TYPE_MAX,
};

static struct mt_i2c_t i2c = {
	.id = I2C5,
	.addr = MT6370_SLAVE_ADDR,
	.mode = FS_MODE,
	.speed = 400,
	.pushpull = false,
};

static int mt6370_i2c_read_byte(u8 cmd, u8 *data)
{
	int ret = 0;
	u8 reg_data = cmd;

	ret = i2c_write_read(&i2c, &reg_data, 1, 1);
	if (ret != I2C_OK)
		pal_log_err("%s: I2CR[0x%X] fail(%d)\n", __func__, cmd, ret);
	else {
		pal_log_info("%s: I2CR[0x%X] = 0x%X\n",
			     __func__, cmd, reg_data);
		*data = reg_data;
	}

	return ret;
}

static int mt6370_i2c_write_byte(u8 cmd, u8 data)
{
	int ret = 0;
	u8 write_buf[2] = {cmd, data};

	ret = i2c_write(&i2c, write_buf, 2);
	if (ret != I2C_OK)
		pal_log_err("%s: I2CW[0x%X] = 0x%X fail(%d)\n",
			    __func__, cmd, data, ret);
	else
		pal_log_info("%s: I2CW[0x%X] = 0x%X\n", __func__, cmd, data);

	return ret;
}

static int mt6370_i2c_update_bits(u8 cmd, u8 mask, u8 data)
{
	int ret = 0;
	u8 reg_data = 0;

	ret = mt6370_i2c_read_byte(cmd, &reg_data);
	if (ret != I2C_OK)
		return ret;

	reg_data &= ~mask;
	reg_data |= (data & mask);

	return mt6370_i2c_write_byte(cmd, reg_data);
}

static inline int mt6370_set_bit(u8 cmd, u8 mask)
{
	return mt6370_i2c_update_bits(cmd, mask, mask);
}

static inline int mt6370_clr_bit(u8 cmd, u8 mask)
{
	return mt6370_i2c_update_bits(cmd, mask, 0x00);
}

#ifdef MTK_UNLOCK_BAT_SUPPORT
static u8 mt6370_find_closest_reg_value(u32 min, u32 max, u32 step, u32 target)
{
	if (target < min)
		return 0;

	if (target > max)
		target = max;

	return (target - min) / step;
}

static int mt6370_set_cv(u32 cv)
{
	u8 reg_data = 0;

	reg_data = mt6370_find_closest_reg_value(MT6370_BAT_VOREG_MIN,
						 MT6370_BAT_VOREG_MAX,
						 MT6370_BAT_VOREG_STEP, cv);

	pal_log_info("%s: cv = %d(0x%X)", __func__, cv, reg_data);

	return mt6370_i2c_update_bits(MT6370_PMU_REG_CHGCTRL4,
				      MT6370_MASK_BAT_VOREG,
				      reg_data << MT6370_SHIFT_BAT_VOREG);
}

int mtk_try_unlock_bat(u32 uV)
{
	return mt6370_set_cv(uV);
}
#endif

#ifdef MTK_EXT_CHGDET_SUPPORT
static int mt6370_enable_chgdet_flow(bool en)
{
	pal_log_info("%s: en = %d\n", __func__, en);
	return (en ? mt6370_set_bit : mt6370_clr_bit)
		(MT6370_PMU_REG_DEVICETYPE, MT6370_MASK_USBCHGEN);
}

static int chgdet_pre_inited = 0;

static int mtk_ext_chgdet_pre_init(void)
{
	int ret = 0;

	/* MT6372 start */
#ifdef FAST_UNKNOWN_TA_DECT
	ret = mt6370_clr_bit(MT6370_PMU_REG_USBSTATUS1,
			     MT6370_MASK_USB_STATUS_OPT);
#else
	ret = mt6370_set_bit(MT6370_PMU_REG_USBSTATUS1,
			     MT6370_MASK_USB_STATUS_OPT);
#endif
	if (ret != I2C_OK)
		goto out;
#ifdef DISABLE_VLGC
	ret = mt6370_set_bit(MT6370_PMU_REG_QCSTATUS1, MT6370_MASK_VLGC);
#else
	ret = mt6370_clr_bit(MT6370_PMU_REG_QCSTATUS1, MT6370_MASK_VLGC);
#endif
	if (ret != I2C_OK)
		goto out;
	/* MT6372 end */

	Charger_Detect_Init();

	/* Toggle chgdet flow */
	ret = mt6370_enable_chgdet_flow(false);
	if (ret != I2C_OK)
		goto out;

	ret = mt6370_enable_chgdet_flow(true);
	if (ret != I2C_OK)
		goto out;

	chgdet_pre_inited = 1;
out:
	return ret;
}

int mtk_ext_chgdet(CHARGER_TYPE *chg_type)
{
	int ret = 0, i = 0;
	const int max_retry_cnt = 120;
	u8 usb_status = 0;

	*chg_type = CHARGER_UNKNOWN;

	if (!chgdet_pre_inited) {
		ret = mtk_ext_chgdet_pre_init();
		if (ret != I2C_OK) {
			pal_log_err("%s: chgdet pre init fail(%d)\n",
				    __func__, ret);
			goto out;
		}
		mdelay(100);
	}

	for (i = 0; i < max_retry_cnt; i++) {
		ret = mt6370_i2c_read_byte(MT6370_PMU_REG_USBSTATUS1,
					   &usb_status);
		if (ret != I2C_OK)
			continue;
		pal_log_info("%s: usb_status = 0x%X\n", __func__, usb_status);
#ifdef FAST_UNKNOWN_TA_DECT
		if (usb_status & MT6370_MASK_DCDT) {
			*chg_type = NONSTANDARD_CHARGER;
			goto out;
		}
#endif
		usb_status = (usb_status & MT6370_MASK_USB_STATUS)
					>> MT6370_SHIFT_USB_STATUS;
		if (usb_status != MT6370_CHG_TYPE_UNDER_GOING)
			break;
		mdelay(20);
	}
	if (i == max_retry_cnt) {
		pal_log_err("%s: chgdet fail\n", __func__);
		goto out;
	}

	switch (usb_status) {
	case MT6370_CHG_TYPE_SDP:
		*chg_type = STANDARD_HOST;
		break;
	case MT6370_CHG_TYPE_CDP:
		*chg_type = CHARGING_HOST;
		break;
	case MT6370_CHG_TYPE_SDPNSTD:
		*chg_type = NONSTANDARD_CHARGER;
		break;
	case MT6370_CHG_TYPE_DCP:
		*chg_type = STANDARD_CHARGER;
		break;
	default:
		*chg_type = CHARGER_UNKNOWN;
		break;
	}

out:
	pal_log_info("%s: chg type = %d\n", __func__, *chg_type);
	Charger_Detect_Release();
	return ret;
}
#endif

int mtk_subpmic_init(void)
{
	int ret = 0;
	u8 reg_data = 0;

	pal_log_info("%s: (%s)\n", __func__, MT6370_PMU_PL_DRV_VERSION);
	ret = mt6370_set_bit(MT6370_PMU_REG_CORECTRL1, MT6370_MASK_MREN);
	if (ret != I2C_OK)
		pal_log_err("%s: en mrstb fail(%d)\n", __func__, ret);
	ret = mt6370_set_bit(MT6370_PMU_REG_DBCTRL2, MT6370_MASK_DB_VPOS_GND);
	if (ret != I2C_OK)
		pal_log_err("%s: en db_vpos_gnd fail(%d)\n", __func__, ret);
	mt6370_i2c_read_byte(MT6370_PMU_REG_CORECTRL1, &reg_data);
	mt6370_i2c_read_byte(MT6370_PMU_REG_DBCTRL2, &reg_data);
#ifdef MTK_EXT_CHGDET_SUPPORT
	ret = mtk_ext_chgdet_pre_init();
	if (ret != I2C_OK)
		pal_log_err("%s: chgdet pre init fail(%d)\n", __func__, ret);
#endif
	return ret;
}

/*
 * Release Note
 * 1.0.4
 * (1) Decrease I2C speed from 3400Hz to 400Hz
 * (2) Replace unsigned int/char with u32/u8
 * (3) Add an option for VLGC checking
 *
 * 1.0.3
 * (1) Add an option for fast unknown TA detection
 *
 * 1.0.2
 * (1) Add log for I2C read/write byte
 *
 * 1.0.1
 * (1) Rearrange the #if blocks
 * (2) Shorten the delay of mtk_ext_chgdet()
 * (3) Add support for MT6372
 * (4) Enable MRSTB
 *
 * 1.0.0
 * (1) Initial release
 */
