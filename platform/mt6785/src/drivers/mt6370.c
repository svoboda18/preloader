#include <mtk_subpmic.h>
#include <i2c.h>
#include "pal_log.h"

/* uV */
#define MT6370_BAT_VOREG_NUM	128
#define MT6370_BAT_VOREG_MIN	3900000
#define MT6370_BAT_VOREG_MAX	4710000
#define MT6370_BAT_VOREG_STEP	10000

#define MT6370_SLAVE_ADDR		(0x34)
#define MT6370_PMU_REG_CHGCTRL4		(0x14)
#define MT6370_PMU_REG_DEVICETYPE	(0x22)
#define MT6370_PMU_REG_USBSTATUS1	(0x27)
#define MT6370_SHIFT_BAT_VOREG		1
#define MT6370_MASK_BAT_VOREG		0xFE
#define MT6370_SHIFT_USBCHGEN	7
#define MT6370_MASK_USBCHGEN	(1 << MT6370_SHIFT_USBCHGEN)
#define MT6370_SHIFT_USB_STATUS	4
#define MT6370_MASK_USB_STATUS	0x70

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
	.mode = HS_MODE,
	.speed = 3400,
	.pushpull = true,
};

static int mt6370_i2c_write_byte(unsigned char cmd, unsigned char data)
{
	int ret = I2C_OK;
	unsigned char write_buf[2] = { cmd, data };

	return i2c_write(&i2c, write_buf, 2);
}

static int mt6370_i2c_read_byte(unsigned char cmd, unsigned char *data)
{
	int ret = I2C_OK;
	unsigned char ret_data = cmd;

	ret = i2c_write_read(&i2c, &ret_data, 1, 1);

	if (ret != I2C_OK)
		return ret;

	*data = ret_data;
	return ret;
}

static int mt6370_i2c_update_bits(unsigned char cmd, unsigned char mask, unsigned char data)
{
	int ret = 0;
	unsigned char reg_data = 0;

	ret = mt6370_i2c_read_byte(cmd, &reg_data);
	if (ret != I2C_OK)
		return ret;

	reg_data = reg_data & 0xFF;
	reg_data &= ~mask;
	reg_data |= (data & mask);

	return mt6370_i2c_write_byte(cmd, reg_data);
}

static inline int mt6370_set_bit(unsigned char reg, unsigned char mask)
{
	return mt6370_i2c_update_bits(reg, mask, mask);
}

static inline int mt6370_clr_bit(unsigned char reg, unsigned char mask)
{
	return mt6370_i2c_update_bits(reg, mask, 0x00);
}

static unsigned char mt6370_find_closest_reg_value(unsigned int min,
						   unsigned int max, unsigned int step,
						   unsigned int num, unsigned int target)
{
	unsigned int i = 0, cur_val = 0, next_val = 0;

	/* Smaller than minimum supported value, use minimum one */
	if (target < min)
		return 0;

	for (i = 0; i < num - 1; i++) {
		cur_val = min + i * step;
		next_val = cur_val + step;

		if (cur_val > max)
			cur_val = max;

		if (next_val > max)
			next_val = max;

		if (target >= cur_val && target < next_val)
			return i;
	}

	/* Greater than maximum supported value, use maximum one */
	return num - 1;
}

static int mt6370_set_cv(unsigned int uV)
{
	int ret = 0;
	unsigned char reg_cv = 0;

	reg_cv = mt6370_find_closest_reg_value(MT6370_BAT_VOREG_MIN,
					       MT6370_BAT_VOREG_MAX,
					       MT6370_BAT_VOREG_STEP, MT6370_BAT_VOREG_NUM, uV);

	print("%s: cv = %d (0x%X), reg = 0x%02x\n",
	      __func__, uV, reg_cv, MT6370_PMU_REG_CHGCTRL4);

	ret = mt6370_i2c_update_bits(MT6370_PMU_REG_CHGCTRL4,
				     MT6370_MASK_BAT_VOREG, reg_cv << MT6370_SHIFT_BAT_VOREG);

	return ret;
}

static int mt6370_enable_chgdet_flow(bool en)
{
	int ret = 0, i = 0;

	pal_log_info("%s: en = %d\n", __func__, en);
	ret = (en ? mt6370_set_bit : mt6370_clr_bit)
	    (MT6370_PMU_REG_DEVICETYPE, MT6370_MASK_USBCHGEN);

	return ret;
}


int mtk_try_unlock_bat(unsigned int uV)
{
	return mt6370_set_cv(uV);
}

#ifdef MTK_EXT_CHGDET_SUPPORT
int mtk_ext_chgdet(CHARGER_TYPE *chg_type)
{
	int ret = 0, i = 0;
	const int max_retry_cnt = 5;
	unsigned char usb_status = 0;

	Charger_Detect_Init();

	*chg_type = CHARGER_UNKNOWN;

	/* Toggle chgdet flow */
	ret = mt6370_enable_chgdet_flow(false);
	if (ret != I2C_OK)
		goto out;

	ret = mt6370_enable_chgdet_flow(true);
	if (ret != I2C_OK)
		goto out;

	for (i = 0; i < max_retry_cnt; i++) {
		mdelay(500);
		ret = mt6370_i2c_read_byte(MT6370_PMU_REG_USBSTATUS1, &usb_status);
		if (ret != I2C_OK) {
			pal_log_info("%s: read charger type failed\n", __func__);
			continue;
		}
		pal_log_info("%s: usb_stats = 0x%X\n", __func__, usb_status);
		usb_status = (usb_status & MT6370_MASK_USB_STATUS) >> MT6370_SHIFT_USB_STATUS;
		if (usb_status != MT6370_CHG_TYPE_UNDER_GOING)
			break;
	}
	if (i == max_retry_cnt) {
		pal_log_info("%s: bc12 failed\n", __func__);
		goto out;
	}

	switch (usb_status) {
	case MT6370_CHG_TYPE_SDP:
		*chg_type = STANDARD_HOST;
		break;
	case MT6370_CHG_TYPE_SDPNSTD:
		*chg_type = NONSTANDARD_CHARGER;
		break;
	case MT6370_CHG_TYPE_CDP:
		*chg_type = CHARGING_HOST;
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

	pal_log_info("%s\n", __func__);
	/* Enable mrstb reset */
	ret = mt6370_set_bit(0x01, 0x10);
	if (ret < 0)
		pal_log_info("%s: en mrstb fail\n", __func__);
	return ret;
}
