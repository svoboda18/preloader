#include <mtk_charger.h>
#include <i2c.h>

/* uV */
#define RT5081_BAT_VOREG_NUM	128
#define RT5081_BAT_VOREG_MIN	3900000
#define RT5081_BAT_VOREG_MAX	4710000
#define RT5081_BAT_VOREG_STEP	10000

#define RT5081_SLAVE_ADDR		(0x34)
#define RT5081_PMU_REG_CHGCTRL4		(0x14)
#define RT5081_PMU_REG_DEVICETYPE	(0x22)
#define RT5081_PMU_REG_USBSTATUS1	(0x27)
#define RT5081_SHIFT_BAT_VOREG		1
#define RT5081_MASK_BAT_VOREG		0xFE
#define RT5081_SHIFT_USBCHGEN	7
#define RT5081_MASK_USBCHGEN	(1 << RT5081_SHIFT_USBCHGEN)
#define RT5081_SHIFT_USB_STATUS	4
#define RT5081_MASK_USB_STATUS	0x70

enum rt5081_pmu_chg_type {
	RT5081_CHG_TYPE_NOVBUS = 0,
	RT5081_CHG_TYPE_UNDER_GOING,
	RT5081_CHG_TYPE_SDP,
	RT5081_CHG_TYPE_SDPNSTD,
	RT5081_CHG_TYPE_DCP,
	RT5081_CHG_TYPE_CDP,
	RT5081_CHG_TYPE_MAX,
};

static struct mt_i2c_t i2c = {
	.id = I2C_APPM,
	.addr = RT5081_SLAVE_ADDR,
	.mode = HS_MODE,
	.speed = 3400,
};

static int rt5081_i2c_write_byte(unsigned char cmd, unsigned char data)
{
	int ret = I2C_OK;
	unsigned char write_buf[2] = {cmd, data};

	return i2c_write(&i2c, write_buf, 2);
}

static int rt5081_i2c_read_byte(unsigned char cmd, unsigned char *data)
{
	int ret = I2C_OK;
	unsigned char ret_data = cmd;

	ret = i2c_write_read(&i2c, &ret_data, 1, 1);

	if (ret != I2C_OK)
		return ret;

	*data = ret_data;
	return ret;
}

static int rt5081_i2c_update_bits(unsigned char cmd, unsigned char mask,
	unsigned char data)
{
	int ret = 0;
	unsigned char reg_data = 0;

	ret = rt5081_i2c_read_byte(cmd, &reg_data);
	if (ret != I2C_OK)
		return ret;

	reg_data = reg_data & 0xFF;
	reg_data &= ~mask;
	reg_data |= (data & mask);

	return rt5081_i2c_write_byte(cmd, reg_data);
}

static inline int rt5081_set_bit(unsigned char reg, unsigned char mask)
{
    return rt5081_i2c_update_bits(reg, mask, mask);
}

static inline int rt5081_clr_bit(unsigned char reg, unsigned char mask)
{
    return rt5081_i2c_update_bits(reg, mask, 0x00);
}

static unsigned char rt5081_find_closest_reg_value(unsigned int min,
	unsigned int max, unsigned int step, unsigned int num,
	unsigned int target)
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

static int rt5081_set_cv(unsigned int uV)
{
	int ret = 0;
	unsigned char reg_cv = 0;

	reg_cv = rt5081_find_closest_reg_value(
		RT5081_BAT_VOREG_MIN,
		RT5081_BAT_VOREG_MAX,
		RT5081_BAT_VOREG_STEP,
		RT5081_BAT_VOREG_NUM,
		uV
	);

	print("%s: cv = %d (0x%X), reg = 0x%02x\n",
	      __func__, uV, reg_cv, RT5081_PMU_REG_CHGCTRL4);

	ret = rt5081_i2c_update_bits(
		RT5081_PMU_REG_CHGCTRL4,
		RT5081_MASK_BAT_VOREG,
		reg_cv << RT5081_SHIFT_BAT_VOREG
	);

	return ret;
}

static int rt5081_enable_chgdet_flow(bool en)
{
	int ret = 0, i = 0;

	print("%s: en = %d\n", __func__, en);
	ret = (en ? rt5081_set_bit : rt5081_clr_bit)
		(RT5081_PMU_REG_DEVICETYPE, RT5081_MASK_USBCHGEN);

	return ret;
}


int mtk_try_unlock_bat(unsigned int uV)
{
	return rt5081_set_cv(uV);
}

int mtk_ext_chgdet(CHARGER_TYPE *chg_type)
{
	int ret = 0, i = 0;
	const int max_retry_cnt = 5;
	unsigned char usb_status = 0;

	Charger_Detect_Init();

	*chg_type = CHARGER_UNKNOWN;

	/* Toggle chgdet flow */
	ret = rt5081_enable_chgdet_flow(false);
	if (ret != I2C_OK)
		goto out;

	ret = rt5081_enable_chgdet_flow(true);
	if (ret != I2C_OK)
		goto out;

	for (i = 0; i < max_retry_cnt; i++) {
		mdelay(500);
		ret = rt5081_i2c_read_byte(RT5081_PMU_REG_USBSTATUS1,
			&usb_status);
		if (ret != I2C_OK) {
			print("%s: read charger type failed\n", __func__);
			continue;
		}
		print("%s: usb_stats = 0x%X\n", __func__, usb_status);
		usb_status = (usb_status & RT5081_MASK_USB_STATUS) >> RT5081_SHIFT_USB_STATUS;
		if (usb_status != RT5081_CHG_TYPE_UNDER_GOING)
			break;
	}
	if (i == max_retry_cnt) {
		print("%s: bc12 failed\n", __func__);
		goto out;
	}

	switch (usb_status) {
	case RT5081_CHG_TYPE_SDP:
		*chg_type = STANDARD_HOST;
		break;
	case RT5081_CHG_TYPE_SDPNSTD:
		*chg_type = NONSTANDARD_CHARGER;
		break;
	case RT5081_CHG_TYPE_CDP:
		*chg_type = CHARGING_HOST;
		break;
	case RT5081_CHG_TYPE_DCP:
		*chg_type = STANDARD_CHARGER;
		break;
	default:
		*chg_type = CHARGER_UNKNOWN;
		break;
	}

out:
	print("%s: chg type = %d\n", __func__, *chg_type);
	Charger_Detect_Release();
	return ret;
}
