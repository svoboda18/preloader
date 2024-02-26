#include <mtk_charger.h>
#include <i2c.h>
#include "pal_log.h"

#ifndef BIT
#define BIT(nr) (1 << (nr))
#endif

#define MT6360_PMU_CHG_CTRL4		(0x14)
#define MT6360_PMU_DEVICE_TYPE		(0x22)
#define MT6360_PMU_USB_STATUS1		(0x27)
#define MT6360_PMU_ADC_CONFIG		(0x56)
#define MT6360_PMU_ADC_RPT_1		(0x5A)
#define MT6360_PMU_CHG_IRQ6		(0xD5)
#define MT6360_PMU_CHG_MASK6		(0xF5)

#define MT6360_MASK_VOREG		(0xFE)
#define MT6360_SHFT_VOREG		(1)
#define MT6360_MASK_USBCHGEN		BIT(7)
#define MT6360_SHFT_USBCHGEN		(7)
#define MT6360_MASK_USB_STATUS		(0x70)
#define MT6360_SHFT_USB_STATUS		(4)
#define MT6360_MASK_ADC_DONE		BIT(4)

static struct mt_i2c_t i2c = {
	.id = I2C5,
	.addr = 0x34,
	.mode = FS_MODE,
	.speed = 400,
};

enum mt6360_pmu_chg_type {
	MT6360_CHG_TYPE_NOVBUS = 0,
	MT6360_CHG_TYPE_UNDER_GOING,
	MT6360_CHG_TYPE_SDP,
	MT6360_CHG_TYPE_SDPNSTD,
	MT6360_CHG_TYPE_DCP,
	MT6360_CHG_TYPE_CDP,
	MT6360_CHG_TYPE_MAX,
};

static int mt6360_i2c_write_byte(unsigned char cmd, unsigned char data)
{
	unsigned char write_buf[2] = { cmd, data };

	return i2c_write(&i2c, write_buf, 2);
}

static int mt6360_i2c_read_byte(unsigned char cmd, unsigned char *data)
{
	int ret = I2C_OK;
	unsigned char reg_data = cmd;

	ret = i2c_write_read(&i2c, &reg_data, 1, 1);
	if (ret != I2C_OK)
		return ret;
	*data = reg_data;
	return ret;
}

static int mt6360_i2c_block_read(unsigned char cmd, unsigned char *data,
				 int len)
{
	data[0] = cmd;
	return i2c_write_read(&i2c, data, 1, len);
}

static int mt6360_i2c_block_write(unsigned char cmd, unsigned char *data,
				  int len)
{
	unsigned char write_buf[len + 1];

	write_buf[0] = cmd;
	memcpy(&write_buf[1], data, len);
	return i2c_write(&i2c, write_buf, len + 1);
}

static int mt6360_i2c_update_bits(unsigned char cmd, unsigned char mask, unsigned char data)
{
	int ret = 0;
	unsigned char reg_data = 0;

	ret = mt6360_i2c_read_byte(cmd, &reg_data);
	if (ret != I2C_OK)
		return ret;
	reg_data = reg_data & 0xFF;
	reg_data &= ~mask;
	reg_data |= (data & mask);
	return mt6360_i2c_write_byte(cmd, reg_data);
}

static inline int mt6360_i2c_set_bits(unsigned char reg, unsigned char mask)
{
	return mt6360_i2c_update_bits(reg, mask, mask);
}

static inline int mt6360_i2c_clr_bits(unsigned char reg, unsigned char mask)
{
	return mt6360_i2c_update_bits(reg, mask, 0x00);
}

static int mt6360_set_cv(unsigned int uV)
{
	int ret = 0;
	unsigned char reg_cv = 0;

	if (uV >= 3900000)
		reg_cv = (uV - 3900000) / 10000;
	if (reg_cv > 0x51)
		reg_cv = 0x51;
	pal_log_info("%s: cv = %d (0x%X)\n", __func__, uV, reg_cv);
	return mt6360_i2c_update_bits(MT6360_PMU_CHG_CTRL4,
				     MT6360_MASK_VOREG,
				     reg_cv << MT6360_SHFT_VOREG);
}

static int mt6360_enable_usbchgen(bool en)
{
	pal_log_info("%s: en = %d\n", __func__, en);
	return (en ? mt6360_i2c_set_bits : mt6360_i2c_clr_bits)
	    (MT6360_PMU_DEVICE_TYPE, MT6360_MASK_USBCHGEN);
}

int mtk_get_vbus_adc(u32 *vbus)
{
	unsigned char tmp[3], rpt[3];
	unsigned char data = 0;
	int i, ret = 0, max_retry_cnt = 20;

	pal_log_info("%s: ++\n", __func__);
	/* first reset all channels and adc en */
	memset(tmp, 0, sizeof(tmp));
	ret = mt6360_i2c_block_write(MT6360_PMU_ADC_CONFIG, tmp, 3);
	if (ret < 0) {
		pal_log_info("%s: reset all channels fail\n", __func__);
		goto err_adc_init;
	}
	/* enable adc_donei irq */
	ret = mt6360_i2c_clr_bits(MT6360_PMU_CHG_MASK6, MT6360_MASK_ADC_DONE);
	if (ret < 0) {
		pal_log_info("%s: unmask adc_donei fail\n", __func__);
		goto err_adc_conv;
	}
	/* select preferred channel : vbus */
	ret = mt6360_i2c_update_bits(MT6360_PMU_ADC_RPT_1, 0xf0, 0x01 << 4);
	if (ret < 0) {
		pal_log_info("%s: select prefer channel fail\n", __func__);
		goto err_adc_conv;
	}
	/* enable vbus adc channel and adc_en */
	tmp[0] |= (1 << 7);
	tmp[1] |= (1 << 1);
	ret = mt6360_i2c_block_write(MT6360_PMU_ADC_CONFIG, tmp, 2);
	if (ret < 0) {
		pal_log_info("%s: enable vbus adc and adc en fail\n", __func__);
		goto err_adc_conv;
	}
	/* wait adc conversion done */
	for (i = 0; i < max_retry_cnt; i++) {
		mdelay(30);
		/* read adc conversion donei event */
		ret = mt6360_i2c_read_byte(MT6360_PMU_CHG_IRQ6, &data);
		if (ret < 0)
			goto err_adc_conv;
		if (!(data & MT6360_MASK_ADC_DONE))
			continue;
		ret = mt6360_i2c_set_bits(MT6360_PMU_CHG_IRQ6, MT6360_MASK_ADC_DONE);
		if (ret < 0) {
			pal_log_info("%s: clear adc donei irq fail\n", __func__);
			goto err_adc_conv;
		}
		memset(rpt, 0, sizeof(rpt));
		ret = mt6360_i2c_block_read(MT6360_PMU_ADC_RPT_1, rpt, 3);
		if (ret < 0)
			goto err_adc_conv;
		if ((rpt[0] & 0x0f) != 0x01) {
			pal_log_info("%s: not want channel report(%d)\n",
				__func__, rpt[0]);
			continue;
		}
		*vbus = ((rpt[1] << 8) | rpt[2]) * 6250;
		break;
	}
	if (i == max_retry_cnt) {
		pal_log_info("%s: reach adc retry cnt\n", __func__);
		ret = -EBUSY;
		goto err_adc_conv;
	}
err_adc_conv:
	tmp[0] &= ~(0x7);
	tmp[1] = 0;
	mt6360_i2c_block_write(MT6360_PMU_ADC_CONFIG, tmp, 2);
	mt6360_i2c_set_bits(MT6360_PMU_CHG_MASK6, MT6360_MASK_ADC_DONE);
err_adc_init:
	pal_log_info("%s: --\n", __func__);
	return ret;
}

int mtk_try_unlock_bat(unsigned int uV)
{
	return mt6360_set_cv(uV);
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
	ret = mt6360_enable_usbchgen(false);
	if (ret != I2C_OK)
		goto out;

	ret = mt6360_enable_usbchgen(true);
	if (ret != I2C_OK)
		goto out;

	for (i = 0; i < max_retry_cnt; i++) {
		mdelay(500);
		ret = mt6360_i2c_read_byte(MT6360_PMU_USB_STATUS1, &usb_status);
		if (ret != I2C_OK) {
			pal_log_info("%s: read charger type failed\n", __func__);
			continue;
		}
		pal_log_info("%s: usb_stats = 0x%x\n", __func__, usb_status);
		usb_status = (usb_status & MT6360_MASK_USB_STATUS) >> MT6360_SHFT_USB_STATUS;
		if (usb_status != MT6360_CHG_TYPE_UNDER_GOING)
			break;
	}
	if (i == max_retry_cnt) {
		pal_log_info("%s: bc12 failed\n", __func__);
		goto out;
	}

	switch (usb_status) {
	case MT6360_CHG_TYPE_SDP:
		*chg_type = STANDARD_HOST;
		break;
	case MT6360_CHG_TYPE_SDPNSTD:
		*chg_type = NONSTANDARD_CHARGER;
		break;
	case MT6360_CHG_TYPE_CDP:
		*chg_type = CHARGING_HOST;
		break;
	case MT6360_CHG_TYPE_DCP:
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
