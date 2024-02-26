#include <i2c.h>
#include <mt6360_pmic.h>
#include <mt6359.h>
#include <timer.h>

#define CRC8_TABLE_SIZE	(256)
static u8 crc8_table[CRC8_TABLE_SIZE];

struct regulator_desc {
	const char *name;
	u8 vsel_reg;
	u8 vsel_mask;
	u8 vsel_shift;
	u8 en_reg;
	u8 en_mask;
	u8 en_shift;
	u8 mode_reg;
	u8 mode_mask;
	u8 mode_shift;
	u32 min_uV;
	u32 uV_step;
	u32 max_uV;
};

struct mt6360_pmic_info {
	struct regulator_desc *reg_desc;
	struct mtk_regulator mreg;
};

/* pmu i2c */
struct mt_i2c_t mt6360_pmu_i2c = {
	.id = 0x05,
	.addr = 0x34,
	.mode = ST_MODE,
	.speed = 400,
	.pushpull = 0,
};

struct mt_i2c_t mt6360_pmic_i2c = {
	.id = 0x05,
	.addr = 0x1a,
	.mode = ST_MODE,
	.speed = 400,
	.pushpull = 0,
};

static void crc8_populate_msb(u8 table[CRC8_TABLE_SIZE], u8 polynomial)
{
	int i, j;
	const u8 msbit = 0x80;
	u8 t = msbit;

	table[0] = 0;

	for (i = 1; i < CRC8_TABLE_SIZE; i *= 2) {
		t = (t << 1) ^ (t & msbit ? polynomial : 0);
		for (j = 0; j < i; j++)
			table[i+j] = table[j]^t;
	}
}

static u8 crc8(const u8 table[CRC8_TABLE_SIZE], u8 *pdata, size_t nbytes, u8 crc)
{
	while (nbytes-- > 0)
		crc = table[(crc ^ *pdata++) & 0xff];

	return crc;
}

static int mt6360_write_byte(u8 addr, u8 data)
{
#if 1
	int len = 1;
	u8 chunk[8] = {0};
#else
	int ret = I2C_OK;
	u8 buf[2] = {0};
#endif

#if 1
	if ((addr & 0xc0) != 0) {
		dbg_print("%s: not support addr [%x]\n", __func__, addr);
		return -EINVAL;
	}
	chunk[0] = (mt6360_pmic_i2c.addr & 0x7f) << 1;
	chunk[1] = (addr & 0x3f)| ((len - 1) << 6);
	memcpy(chunk + 2, &data, len);
	chunk[2+len] = crc8(crc8_table, chunk, 2+len, 0);
	return i2c_write(&mt6360_pmic_i2c, chunk+1, len+3);
#else
	buf[0] = addr;
	buf[1] = data;
	ret = i2c_write(&mt6360_pmic_i2c, buf, 2);
	if (ret < 0)
		dbg_print("%s: fail(%d), addr = 0x%x\n", __func__, ret, addr);
	return ret;
#endif
}

static int mt6360_read_byte(u8 addr, u8 *data)
{
	int ret = I2C_OK;
#if 1
	int len = 1;
	u8 chunk[8] = {0};
#endif
	u8 temp[2] = {0};

#if 1
	chunk[0] = ((mt6360_pmic_i2c.addr & 0x7f) << 1) + 1;
	chunk[1] = (addr & 0x3f) | ((len - 1) << 6);
	temp[0] = chunk[1];
	ret = i2c_write_read(&mt6360_pmic_i2c, &temp[0], 1, len + 1);
	if (ret < 0)
		return ret;
	chunk[2] = temp[0];
	chunk[3] = temp[1];
	chunk[7] = crc8(crc8_table, chunk, 2 + len, 0);
	if (chunk[2+len] != chunk[7])
		return -EINVAL;
	memcpy(data, chunk+2, len);
	return ret;
#else
	temp[0] = addr;
	ret = i2c_write_read(&mt6360_pmic_i2c, temp, 1, 1);
	if (ret < 0) {
		dbg_print("%s: fail(%d), addr = 0x%x\n", __func__, ret, addr);
		return ret;
	}
	*data = temp[0];
	return ret;
#endif
}

static int mt6360_update_bits(u8 addr, u8 mask, u8 data)
{
	int ret = I2C_OK;
	u8 org = 0;

	ret = mt6360_read_byte(addr, &org);
	if (ret < 0)
		return ret;
	org &= ~mask;
	org |= (data & mask);
	return mt6360_write_byte(addr, org);
}

static inline int mt6360_set_bits(u8 addr, u8 mask)
{
	return mt6360_update_bits(addr, mask, mask);
}

static inline int mt6360_clr_bits(u8 addr, u8 mask)
{
	return mt6360_update_bits(addr, mask, 0);
}

static int mt6360_enable_hidden_mode(bool en)
{
	u8 tmp[2];

	tmp[0] = 0x07;
	tmp[1] = (en ? 0x69 : 0);
	return i2c_write(&mt6360_pmu_i2c, tmp, 2);
}

int mt6360_pmic_read_interface(u8 addr, u8 *data, u8 mask, u8 shift)
{
	int ret = 0;
	u8 rdata = 0;

	ret = mt6360_read_byte(addr, &rdata);
	if (ret < 0) {
		dbg_print("%s: fail, addr = 0x%x, ret = %d\n",
			   __func__, addr, ret);
		return ret;
	}
	rdata &= (mask << shift);
	*data = (rdata >> shift);
	return 0;
}

int mt6360_pmic_config_interface(u8 addr, u8 data, u8 mask, u8 shift)
{
	int ret = 0;
	u8 org = 0;

	ret = mt6360_read_byte(addr, &org);
	if (ret < 0) {
		dbg_print("%s: fail, addr = 0x%x, ret = %d\n",
			   __func__, addr, ret);
		return ret;
	}
	org &= ~(mask << shift);
	org |= (data << shift);
	return mt6360_write_byte(addr, org);
}

static struct regulator_desc mt6360_desc[] = {
	{
		.name = "BUCK1",
		.vsel_reg = 0x10,
		.vsel_mask = 0xff,
		.vsel_shift = 0,
		.en_reg = 0x17,
		.en_mask = 0x40,
		.en_shift = 6,
		.mode_reg = 0x1a,
		.mode_mask = 0x02,
		.mode_shift = 1,
		.min_uV =  300000,
		.uV_step = 5000,
		.max_uV = 1300000,
	},
	{
		.name = "BUCK2",
		.vsel_reg = 0x20,
		.vsel_mask = 0xff,
		.vsel_shift = 0,
		.en_reg = 0x27,
		.en_mask = 0x40,
		.en_shift = 6,
		.mode_reg = 0x2a,
		.mode_mask = 0x02,
		.mode_shift = 1,
		.min_uV =  300000,
		.uV_step = 5000,
		.max_uV = 1300000,
	},
	{
		.name = "LDO6",
		.vsel_reg = 0x3b,
		.vsel_mask = 0xff,
		.vsel_shift = 0,
		.en_reg = 0x37,
		.en_mask = 0x40,
		.en_shift = 6,
		/* w/o FPWM mode */
		.mode_reg = 0xff,
		.mode_mask = 0x30,
		.mode_shift = 4,
		.min_uV =  500000,
		.uV_step = 10000,
		.max_uV = 2100000,
	},
	{
		.name = "LDO7",
		.vsel_reg = 0x35,
		.vsel_mask = 0xff,
		.vsel_shift = 0,
		.en_reg = 0x31,
		.en_mask = 0x40,
		.en_shift = 6,
		/* w/o FPWM mode */
		.mode_reg = 0xff,
		.mode_mask = 0x30,
		.mode_shift = 4,
		.min_uV =  500000,
		.uV_step = 10000,
		.max_uV = 2100000,
	},
};

static struct regulator_ctrl mt6360_regulator_ctrl;

static struct mt6360_pmic_info g_mpi[] = {
	{
		.reg_desc = &mt6360_desc[0],
		.mreg = {
			.name = "VMDLA",
			.id = MTK_REGULATOR_VMDLA,
			.reg_ops = &mt6360_regulator_ctrl,
		},
	},
	{
		.reg_desc = &mt6360_desc[1],
		.mreg = {
			.name = "VDRAM1",
			.id = MTK_REGULATOR_VDRAM1,
			.reg_ops = &mt6360_regulator_ctrl,
		},
	},
	{
		.reg_desc = &mt6360_desc[2],
		.mreg = {
			.name = "VMDDR",
			.id = MTK_REGULATOR_VMDDR,
			.reg_ops = &mt6360_regulator_ctrl,
		},
	},
	{
		.reg_desc = &mt6360_desc[3],
		.mreg = {
			.name = "VDRAM2",
			.id = MTK_REGULATOR_VDRAM2,
			.reg_ops = &mt6360_regulator_ctrl,
		},
	},
};

static struct mt6360_pmic_info *mt6360_find_pmic_info(unsigned char id)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(g_mpi); i++) {
		if(g_mpi[i].mreg.id == id)
			return &g_mpi[i];
	}
	return NULL;
}

static int mt6360_set_voltage(unsigned char id,
			      unsigned int min_vol, unsigned int max_vol)
{
	struct mt6360_pmic_info *mpi = mt6360_find_pmic_info(id);
	struct regulator_desc *desc;
	u8 sel = 0;

	if (!mpi)
		return -ENODEV;
	desc = mpi->reg_desc;
	if (min_vol < desc->min_uV || max_vol > desc->max_uV)
		return -EINVAL;
	sel = DIV_ROUND_UP(min_vol - desc->min_uV, desc->uV_step);
	if (id == MTK_REGULATOR_VMDDR || id == MTK_REGULATOR_VDRAM2)
		sel = ((sel / 10) << 4) + sel % 10;
	return mt6360_write_byte(desc->vsel_reg, sel << desc->vsel_shift);
}

static int mt6360_get_voltage(unsigned char id)
{
	struct mt6360_pmic_info *mpi = mt6360_find_pmic_info(id);
	struct regulator_desc *desc;
	int ret = 0;
	u8 data = 0;
	U32 vol = 0;

	if (!mpi)
		return -ENODEV;
	desc = mpi->reg_desc;
	ret = mt6360_read_byte(desc->vsel_reg, &data);
	if (ret < 0)
		return ret;
	data = (data & desc->vsel_mask) >> desc->vsel_shift;
	if (id == MTK_REGULATOR_VMDDR || id == MTK_REGULATOR_VDRAM2) {
		if ((data & 0x0f) > 0x0a)
			data = (data & 0xf0) | 0x0a;
		data = ((data & 0xf0) >> 4) * 10 + (data & 0x0f);
	}
	vol = desc->min_uV + (data * desc->uV_step);
	if (vol > desc->max_uV)
		vol = desc->max_uV;
	return vol;
}

static int mt6360_enable(unsigned char id, unsigned char en)
{
	struct mt6360_pmic_info *mpi = mt6360_find_pmic_info(id);
	struct regulator_desc *desc;

	if (!mpi)
		return -ENODEV;
	desc = mpi->reg_desc;
	return mt6360_update_bits(desc->en_reg, desc->en_mask, en ? 0xff: 0);
}

static int mt6360_is_enabled(unsigned char id)
{
	struct mt6360_pmic_info *mpi = mt6360_find_pmic_info(id);
	struct regulator_desc *desc;
	int ret = 0;
	u8 data = 0;

	if (!mpi)
		return -ENODEV;
	desc = mpi->reg_desc;
	ret = mt6360_read_byte(desc->en_reg, &data);
	if (ret < 0)
		return ret;
	return (data & desc->en_mask) ? true : false;
}

static int mt6360_set_mode(unsigned char id, unsigned char mode)
{
	struct mt6360_pmic_info *mpi = mt6360_find_pmic_info(id);
	struct regulator_desc *desc;
	int ret = 0;

	if (!mpi)
		return -ENODEV;
	if (id != MTK_REGULATOR_VMDLA && id != MTK_REGULATOR_VDRAM1)
		return -EPERM;
	desc = mpi->reg_desc;
	ret = mt6360_enable_hidden_mode(true);
	if (ret < 0) {
		dbg_print("%s: enter hidden mode fail\n", __func__);
		return ret;
	}
	ret = mt6360_update_bits(desc->mode_reg, desc->mode_mask,
				 mode << desc->mode_shift);
	if (ret < 0)
		dbg_print("%s: set mode fail, mode = %d\n", __func__, mode);
	mt6360_enable_hidden_mode(false);
	return ret;
}

static int mt6360_get_mode(unsigned char id)
{
	struct mt6360_pmic_info *mpi = mt6360_find_pmic_info(id);
	struct regulator_desc *desc;
	int ret = 0;
	u8 data = 0;

	if (!mpi)
		return -ENODEV;
	if (id != MTK_REGULATOR_VMDLA && id != MTK_REGULATOR_VDRAM1)
		return REGULATOR_MODE_NORMAL;
	desc = mpi->reg_desc;
	ret = mt6360_enable_hidden_mode(true);
	if (ret < 0) {
		dbg_print("%s: enter hidden mode fail\n", __func__);
		return ret;
	}
	ret = mt6360_read_byte(desc->mode_reg, &data);
	if (ret < 0)
		goto out;
	data = (data & desc->mode_mask) >> desc->mode_shift;
	switch (data) {
	case 0:
		ret = REGULATOR_MODE_NORMAL;
		break;
	case 1:
		ret = REGULATOR_MODE_FAST;
		break;
	default:
		ret = -EINVAL;
		break;
	}
out:
	mt6360_enable_hidden_mode(false);
	return ret;
}

static struct regulator_ctrl mt6360_regulator_ctrl = {
	.set_voltage = mt6360_set_voltage,
	.get_voltage = mt6360_get_voltage,
	.enable = mt6360_enable,
	.is_enabled = mt6360_is_enabled,
	.set_mode = mt6360_set_mode,
	.get_mode = mt6360_get_mode,
};

int mt6360_enable_test_mode(bool en)
{
	u8 tmp[5] = {0};
	u8 tm_val[4] = {0x69, 0x96, 0x63, 0x72};

	if (en) {
		tmp[0] = 0x07;
		memcpy(tmp + 1, tm_val, 4);
		return i2c_write(&mt6360_pmu_i2c, tmp, 5);
	}
	else {
		tmp[0] = 0x07;
		return i2c_write(&mt6360_pmu_i2c, tmp, 2);
	}
}

int mt6360_pmic_enable_crc_check(bool en)
{
	/* test mode i2c */
	struct mt_i2c_t mt6360_tm_i2c = {
		.id = 0x05,
		.addr = 0x3f,
		.mode = ST_MODE,
		.speed = 400,
		.pushpull = 0,
	};
	u8 tmp[2] = {0};
	int ret = 0;

	/* check crc_en */
	tmp[0] = 0x03;
	ret = i2c_write_read(&mt6360_pmu_i2c, tmp, 1, 1);
	if (ret < 0) {
		dbg_print("%s: read pmu crc_en fail\n", __func__);
		return ret;
	} else
		dbg_print("%s: crc_en = 0x%x\n", __func__, tmp[0]);
	if ((tmp[0] & 0xc0) == 0xc0) {
		dbg_print("%s: no need\n", __func__);
		return ret;
	}
	/* enter test mode */
	ret = mt6360_enable_test_mode(true);
	if (ret < 0) {
		dbg_print("%s: enable test mode fail\n", __func__);
		return ret;
	}
	/* check is in test mode */
	tmp[0] = 0x0f;
	ret = i2c_write_read(&mt6360_pmu_i2c, tmp, 1, 1);
	if (ret < 0) {
		dbg_print("%s: read test mode reg fail\n", __func__);
		goto out;
	}
	if (tmp[0] & 0x80)
		dbg_print("%s: enter test mode success\n", __func__);
	else {
		dbg_print("%s: enter test mode fail(0x%x)\n", __func__,tmp[0]);
		return ret;
	}
	/* enable pmic/ldo crc in test mode reg0x5c[5:4] */
	tmp[0] = 0x5c;
	ret = i2c_write_read(&mt6360_tm_i2c, tmp, 1, 1);
	if (ret < 0) {
		dbg_print("%s: read pmic/ldo crc fail\n", __func__);
		goto out;
	}
	tmp[1] = (tmp[0] | 0x30);
	tmp[0] = 0x5c;
	ret = i2c_write(&mt6360_tm_i2c, tmp, 2);
	if (ret < 0) {
		dbg_print("%s: write crc_en fail\n", __func__);
		goto out;
	}
	/* reload efuse */
	tmp[0] = 0x80;
	tmp[1] = 0x08;
	ret = i2c_write(&mt6360_tm_i2c, tmp, 2);
	if (ret < 0) {
		dbg_print("%s: reload efuse fail\n", __func__);
		goto out;
	}
#if 1
	tmp[0] = 0x5c;
	ret = i2c_write_read(&mt6360_tm_i2c, tmp, 1, 1);
	if (ret < 0) {
		dbg_print("%s: read tm crc_en fail\n", __func__);
		goto out;
	}
	dbg_print("%s: read tm reg0x5c = 0x%x\n", __func__, tmp[0]);
#endif
out:
	/* disable test mode */
	mt6360_enable_test_mode(false);
	return ret;
}

/* init value mask */
static const u8 sysuv_ctrl_mask[] = { 0xff, 0xc0, 0xff };
static const u8 buck_ctrl_mask[] = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 0xff, 0xff,
};
static const u8 ldo_ctrl_mask[] = { 0xff, 0x8f, 0x3f, 0xfe, 0xff };

/* buck/ldo init value */
static const u8 sysuv_ctrl_init[] = { 0x25, 0xc0, 0x10 };
static const u8 buck1_ctrl_init[] = {
	0xa9, 0x80, 0x82, 0x6a, 0x00, 0x80, 0x02, 0x70,
};
static const u8 buck2_ctrl_init[] = {
	0xa9, 0x80, 0x81, 0x6a, 0x00, 0x80, 0x02, 0x70,
};

static const u8 ldo6_ctrl_init[] = { 0x30, 0x80, 0x02, 0xec, 0x44 };
static const u8 ldo7_ctrl_init[] = { 0x30, 0x80, 0x02, 0xec, 0x44 };
#if 0
static int mt6360_enable_fpwm_usm(bool en)
{
	int ret = 0;

	dbg_print("%s: en = %d\n", __func__, en);
	/* Enable ultra sonic mode */
	ret = mt6360_update_bits(0x19, 0x08, en ? 0xff : 0);
	if (ret < 0) {
		dbg_print("%s: enable ultra sonic mode fail\n", __func__);
		return ret;
	}

	ret = mt6360_enable_hidden_mode(true);
	if (ret < 0) {
		dbg_print("%s: enter hidden mode fail\n", __func__);
		return ret;
	}
	/* Enable FPWM mode */
	ret = mt6360_update_bits(0x1a, 0x02, en ? 0xff : 0);
	if (ret < 0) {
		dbg_print("%s: enable FPWM fail\n",  __func__);
		goto out;
	}
out:
	ret = mt6360_enable_hidden_mode(false);
	if (ret < 0)
		dbg_print("%s: exit hidden mode fail\n", __func__);
	return ret;
}
#endif
static int mt6360_check_buck1_vosel(void)
{
	int ret = 0;
	u8 data = 0;

	dbg_print("%s\n", __func__);
	/* check buck1 voltage already trim or not */
	ret = mt6360_read_byte(0x10, &data);
	if (ret < 0)
		dbg_print("%s: read buck1 vosel fail\n", __func__);

	if (data == 0x69) {
		dbg_print("%s: already trim\n", __func__);
		return ret;
	} else {
		dbg_print("%s: need trim buck1 to 0.825v, data = 0x%x\n",
			  __func__, data);
		dbg_print("%s: fail, while loop\n", __func__);
		dbg_print("%s: Please return phone to Alex Chchang\n", __func__);
		while (1)
			mdelay(200);
		return -EINVAL;
	}
}

static int mt6360_pmic_init(void)
{
	unsigned int i = 0;
	int ret = 0;
	u8 tmp[2];
	u8 data = 0;

	/* enable mrstb reset */
	tmp[0] = 0x01;
	tmp[1] = 0x18;
	ret = i2c_write(&mt6360_pmu_i2c, tmp, 2);
	if (ret < 0) {
		dbg_print("%s: enable mrstb reset fail\n", __func__);
		return ret;
	}
	/* enable pmic/ldo reset by mrstb reset */
	tmp[0] = 0x06;
	tmp[1] = 0x82;
	ret = i2c_write(&mt6360_pmu_i2c, tmp, 2);
	if (ret < 0) {
		dbg_print("%s: enable i2c reset fail\n", __func__);
		return ret;
	}

	/* mt6360 enable pmic/ldo crc check */
	ret = mt6360_pmic_enable_crc_check(true);
	if (ret < 0) {
		dbg_print("%s: enable crc check fail\n", __func__);
		return ret;
	}

	/*  disable buck&ldo sequence off delay */
	ret = mt6360_pmic_enable_poweroff_sequence(false);
	if (ret < 0) {
		dbg_print("%s: disable power off sequence fail\n", __func__);
		return ret;
	}

	ret = mt6360_check_buck1_vosel();
	if (ret < 0) {
		dbg_print("%s: check buck1 voltage fail\n", __func__);
		return ret;
	}

#if 0
	/* workaround for PGB EVT false alarm trigger by dvfs buck to lower voltage */
	ret = mt6360_enable_fpwm_usm(true);
	if (ret < 0) {
		dbg_print("%s: fail(%d)\n", __func__);
		return ret;
	}
#endif

	/* sysuv init */
	for (i = 0; i < ARRAY_SIZE(sysuv_ctrl_mask); i++) {
		ret = mt6360_update_bits(0x04 + i, sysuv_ctrl_mask[i],
					 sysuv_ctrl_init[i]);
		if (ret < 0) {
			dbg_print("%s: reg 0x%x set fail\n",
				  __func__, (0x04 + i));
			return ret;
		}
	}
	/* buck init */
	for (i = 0; i < ARRAY_SIZE(buck_ctrl_mask); i++) {
		ret = mt6360_update_bits(0x12 + i, buck_ctrl_mask[i],
					 buck1_ctrl_init[i]);
		if (ret < 0) {
			dbg_print("%s: reg 0x%x set fail\n",
				  __func__, (0x12 + i));
			return ret;
		}
		ret = mt6360_update_bits(0x22 + i, buck_ctrl_mask[i],
					 buck2_ctrl_init[i]);
		if (ret < 0) {
			dbg_print("%s: reg 0x%x set fail\n",
				  __func__, (0x22 + i));
			return ret;
		}
	}
	/* ldo init */
	for (i = 0; i < ARRAY_SIZE(ldo_ctrl_mask); i++) {
		ret = mt6360_update_bits(0x30 + i, ldo_ctrl_mask[i],
					 ldo7_ctrl_init[i]);
		if (ret < 0) {
			dbg_print("%s: reg 0x%x set fail\n",
				  __func__, (0x30 + i));
			return ret;
		}
		ret = mt6360_update_bits(0x36 + i, ldo_ctrl_mask[i],
					 ldo6_ctrl_init[i]);
		if (ret < 0) {
			dbg_print("%s: reg 0x%x set fail\n",
				  __func__, (0x36 + i));
			return ret;
		}
	}
	/* mtk regulator register */
	for (i = 0; i < ARRAY_SIZE(g_mpi); i++) {
		ret |= mtk_simple_regulator_register(&g_mpi[i].mreg);
		if (ret) {
			dbg_print("%s: regulator register %s fail\n",
				   __func__, g_mpi[i].reg_desc->name);
			return -1;
		}
	}
#if 1
	ret = mt6360_read_byte(0x26, &data);
	dbg_print("%s: read 0x26 = 0x%x\n", __func__, data);
#endif
	return 0;
}

static int mt6360_pmic_is_hw_exist(void)
{
	u8 tmp;
	int ret;

	/* addr 0x00 is DEV_INFO */
	tmp = 0x00;
	ret = i2c_write_read(&mt6360_pmu_i2c, &tmp, 1, 1);
	/* mt6360 pmu DEV_ID bit[7:4] must be 0x50 */
	if (ret < 0 || (tmp & 0xf0) != 0x50)
		return -1;
	return 0;
}

int mt6360_pmic_enable_poweroff_sequence(bool en)
{
	int i, ret = 0;
	u8 seq_delay[4] = { 0x06, 0x04, 0x00, 0x02 };

	dbg_print("%s: en = %d\n", __func__, en);
	/* Set sub-pmic buck&ldo power off seuqence on */
	for (i = 0; i < 4; i++) {
		ret = mt6360_pmic_config_interface(
				   0x07 + i, en ? seq_delay[i] : 0x00, 0xff, 0);
		if (ret < 0) {
			pal_log_info("%s: set pmic(%d) fail\n", __func__, i);
			return ret;
		}
	}
	return ret;
}

int mt6360_get_hw_trapping_type(void)
{
       int ret = 0;
       u8 data = 0;

       ret = mt6360_read_byte(0x06, &data);
       if (ret < 0) {
               print("%s: fail(%d)\n", __func__, ret);
               return ret;
       }
       return (data & 0x07);
}

int mt6360_pmic_i2c_probe(void)
{
	int i, ret = 0;
	u8 data;

	dbg_print("%s\n", __func__);
	ret = mt6360_pmic_is_hw_exist();
	if (ret < 0) {
		dbg_print("%s: is not exist\n", __func__);
		return -1;
	}

	crc8_populate_msb(crc8_table, 0x7);

	ret = mt6360_pmic_init();
	if (ret < 0)
		dbg_print("%s probe fail!\n", __func__);


	/* Dump PGB&OC evt for debug */
	for (i = 0; i < 4; i++) {
		data = 0xdc + i;
		ret = i2c_write_read(&mt6360_pmu_i2c, &data, 1, 1);
		if (ret < 0)
			return ret;
		dbg_print("%s: reg[0x%x] = 0x%x\n",
			  __func__, 0xdc + i, data);
	}
	return ret;
}
