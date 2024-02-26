#include <i2c.h>
#include <mt_drcc.h>
#include <mtk_subpmic.h>
#include <pal_log.h>
#include <timer.h>

#define RT9471_SLAVE_ADDR	(0x53)
#define RT9470_DEVID		0x09
#define RT9470D_DEVID		0x0A
#define RT9471_DEVID		0x0D
#define RT9471D_DEVID		0x0E
/* uV */
#define RT9471_CV_MIN		3900000
#define RT9471_CV_MAX		4700000
#define RT9471_CV_STEP		10000
#define RT9471_REG_VCHG		(0x07)
#define RT9471_CV_SHIFT		0
#define RT9471_CV_MASK		0x7F
#define RT9471_REG_INFO		(0x0B)
#define RT9471_DEVID_SHIFT	3
#define RT9471_DEVID_MASK	0x78
#define RT9471_REG_DPDMDET	(0x0E)
#define RT9471_BC12_EN_MASK	BIT(7)
#define RT9471_REG_STATUS	(0x0F)
#define RT9471_PORTSTAT_SHIFT	4
#define RT9471_PORTSTAT_MASK	0xF0

#define RT9471_PL_DRV_VERSION	"1.0.1_MTK"

enum rt9471_port_stat {
	RT9471_PORTSTAT_NOINFO = 0,
	RT9471_PORTSTAT_APPLE_10W = 8,
	RT9471_PORTSTAT_SAMSUNG_10W,
	RT9471_PORTSTAT_APPLE_5W,
	RT9471_PORTSTAT_APPLE_12W,
	RT9471_PORTSTAT_NSDP,
	RT9471_PORTSTAT_SDP,
	RT9471_PORTSTAT_CDP,
	RT9471_PORTSTAT_DCP,
	RT9471_PORTSTAT_MAX,
};

#if defined(MTK_UNLOCK_BAT_SUPPORT) || defined(MTK_EXT_CHGDET_SUPPORT)
static struct mt_i2c_t i2c = {
	.id = I2C7,
	.addr = RT9471_SLAVE_ADDR,
	.mode = FS_MODE,
	.speed = 400,
};

static int rt9471_i2c_write_byte(unsigned char cmd, unsigned char data)
{
	int ret = I2C_OK;
	unsigned char write_buf[2] = {cmd, data};

	ret = i2c_write(&i2c, write_buf, 2);
	if (ret != I2C_OK)
		pal_log_err("%s I2CW[0x%X] = 0x%X fail, code = %d\n",
			    __func__, cmd, data, ret);
	else
		pal_log_info("%s I2CW[0x%X] = 0x%X\n", __func__, cmd, data);

	return ret;
}

static int rt9471_i2c_read_byte(unsigned char cmd, unsigned char *data)
{
	int ret = I2C_OK;
	unsigned char regval = cmd;

	ret = i2c_write_read(&i2c, &regval, 1, 1);
	if (ret != I2C_OK)
		pal_log_err("%s I2CR[0x%X] fail, code = %d\n",
			    __func__, cmd, ret);
	else {
		pal_log_info("%s I2CR[0x%X] = 0x%X\n",
			     __func__, cmd, regval);
		*data = regval;
	}

	return ret;
}

static int rt9471_i2c_update_bits(unsigned char cmd, unsigned char data,
				  unsigned char mask)
{
	int ret = I2C_OK;
	unsigned char regval = 0;

	ret = rt9471_i2c_read_byte(cmd, &regval);
	if (ret != I2C_OK)
		return ret;

	regval &= ~mask;
	regval |= (data & mask);

	return rt9471_i2c_write_byte(cmd, regval);
}
#endif

#ifdef MTK_UNLOCK_BAT_SUPPORT
static inline unsigned char rt9471_closest_reg(unsigned int min,
					       unsigned int max,
					       unsigned int step,
					       unsigned int target)
{
	if (target < min)
		return 0;

	if (target >= max)
		return (max - min) / step;

	return (target - min) / step;
}

static int rt9471_set_cv(unsigned int cv)
{
	unsigned char regval = 0;

	regval = rt9471_closest_reg(RT9471_CV_MIN, RT9471_CV_MAX,
				    RT9471_CV_STEP, cv);

	pal_log_info("%s cv = %d(0x%X)\n", __func__, cv, regval);

	return rt9471_i2c_update_bits(RT9471_REG_VCHG,
				      regval << RT9471_CV_SHIFT,
				      RT9471_CV_MASK);
}

int mtk_try_unlock_bat(unsigned int uV)
{
	return rt9471_set_cv(uV);
}
#endif

#ifdef MTK_EXT_CHGDET_SUPPORT
static inline int rt9471_set_bit(unsigned char cmd, unsigned char mask)
{
	return rt9471_i2c_update_bits(cmd, mask, mask);
}

static inline int rt9471_clr_bit(unsigned char cmd, unsigned char mask)
{
	return rt9471_i2c_update_bits(cmd, 0x00, mask);
}

static int rt9471_enable_bc12(bool en)
{
	pal_log_info("%s en = %d\n", __func__, en);
	return (en ? rt9471_set_bit : rt9471_clr_bit)
		(RT9471_REG_DPDMDET, RT9471_BC12_EN_MASK);
}

void Charger_Detect_Init(void);
void Charger_Detect_Release(void);

int mtk_ext_chgdet(CHARGER_TYPE *chg_type)
{
	int ret = I2C_OK, i = 0;
	const int max_retry_cnt = 100;
	unsigned char port_stat = RT9471_PORTSTAT_NOINFO;
	unsigned char dev_id = 0;

	pal_log_info("%s (%s)\n", __func__, RT9471_PL_DRV_VERSION);

	ret = rt9471_i2c_read_byte(RT9471_REG_INFO, &dev_id);
	if (ret != I2C_OK)
		return ret;
	dev_id = (dev_id & RT9471_DEVID_MASK) >> RT9471_DEVID_SHIFT;
	if (dev_id != RT9470D_DEVID && dev_id != RT9471D_DEVID) {
		pal_log_err("%s incorrect devid 0x%X\n", __func__, dev_id);
		return ret;
	}

	Charger_Detect_Init();

	*chg_type = CHARGER_UNKNOWN;

	/* Toggle chgdet flow */
	ret = rt9471_enable_bc12(false);
	if (ret != I2C_OK)
		goto out;

	ret = rt9471_enable_bc12(true);
	if (ret != I2C_OK)
		goto out;

	for (i = 0; i < max_retry_cnt; i++) {
		mdelay(20);
		ret = rt9471_i2c_read_byte(RT9471_REG_STATUS, &port_stat);
		if (ret != I2C_OK)
			continue;
		port_stat = (port_stat & RT9471_PORTSTAT_MASK) >>
					 RT9471_PORTSTAT_SHIFT;
		pal_log_info("%s port_stat = 0x%X\n", __func__, port_stat);
		if (port_stat != RT9471_PORTSTAT_NOINFO)
			break;
	}
	if (i == max_retry_cnt) {
		pal_log_err("%s bc12 failed\n", __func__);
		goto out;
	}

	switch (port_stat) {
	case RT9471_PORTSTAT_NOINFO:
		*chg_type = CHARGER_UNKNOWN;
		break;
	case RT9471_PORTSTAT_SDP:
		*chg_type = STANDARD_HOST;
		break;
	case RT9471_PORTSTAT_CDP:
		*chg_type = CHARGING_HOST;
		break;
	case RT9471_PORTSTAT_SAMSUNG_10W:
	case RT9471_PORTSTAT_APPLE_12W:
	case RT9471_PORTSTAT_DCP:
		*chg_type = STANDARD_CHARGER;
		break;
	case RT9471_PORTSTAT_APPLE_10W:
		*chg_type = APPLE_2_1A_CHARGER;
		break;
	case RT9471_PORTSTAT_APPLE_5W:
		*chg_type = APPLE_1_0A_CHARGER;
		break;
	case RT9471_PORTSTAT_NSDP:
	default:
		*chg_type = NONSTANDARD_CHARGER;
		break;
	}

out:
	pal_log_info("%s chg type = %d\n", __func__, *chg_type);
	Charger_Detect_Release();

	return ret;
}
#endif

/*
 * Revision Note
 * 1.0.1
 * (1) Rearrange the #if blocks
 * (2) Add support for RT9470/RT9470D
 * (3) Sync with LK Driver
 * (4) Shorten the delay of mtk_ext_chgdet()
 *
 * 1.0.0
 * (1) Initial release
 */
