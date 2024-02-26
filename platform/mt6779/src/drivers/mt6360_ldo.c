#include <i2c.h>
#include <mt6360_ldo.h>
#include <mt6359.h>

#define CRC8_TABLE_SIZE	(256)
static u8 crc8_table[CRC8_TABLE_SIZE];

struct mt_i2c_t mt6360_ldo_i2c = {
	.id = 0x05,
	.addr = 0x64,
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
	chunk[0] = (mt6360_ldo_i2c.addr & 0x7f) << 1;
	chunk[1] = (addr & 0x3f)| ((len - 1) << 6);
	memcpy(chunk + 2, &data, len);
	chunk[2+len] = crc8(crc8_table, chunk, 2+len, 0);
	return i2c_write(&mt6360_ldo_i2c, chunk+1, len+3);
#else
	buf[0] = addr;
	buf[1] = data;
	ret = i2c_write(&mt6360_ldo_i2c, buf, 2);
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
	chunk[0] = ((mt6360_ldo_i2c.addr & 0x7f) << 1) + 1;
	chunk[1] = (addr & 0x3f) | ((len - 1) << 6);
	temp[0] = chunk[1];
	ret = i2c_write_read(&mt6360_ldo_i2c, &temp[0], 1, len + 1);
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
	ret = i2c_write_read(&mt6360_ldo_i2c, temp, 1, 1);
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

int mt6360_ldo_read_interface(u8 addr, u8 *data, u8 mask, u8 shift)
{
	int ret = 0;

	ret = mt6360_read_byte(addr, data);
	if (ret < 0) {
		dbg_print("%s: fail, addr = 0x%X, ret = %d\n",
			   __func__, addr, ret);
		return ret;
	}
	*data = (*data & mask) >> shift;
	return 0;
}

int mt6360_ldo_config_interface(u8 addr, u8 data, u8 mask, u8 shift)
{
	int ret = 0;
	u8 org = 0;

	ret = mt6360_read_byte(addr, &org);
	if (ret < 0) {
		dbg_print("%s: fail, addr = 0x%X, ret = %d\n",
			   __func__, addr, ret);
		return ret;
	}
	org &= ~(mask << shift);
	org |= (data << shift);
	return mt6360_write_byte(addr, org);
}

/* init value mask */
static const u8 ldo_ctrl_mask[] = { 0xff, 0x8f, 0xff, 0xff, 0xff };

/* ldo init value */
static const u8 ldo1_ctrl_init[] = { 0x00, 0x80, 0x01, 0x2d, 0x44 };
static const u8 ldo2_ctrl_init[] = { 0x00, 0x80, 0x01, 0x2d, 0x44 };
static const u8 ldo3_ctrl_init[] = { 0x00, 0x80, 0x01, 0x2d, 0xa4 };
static const u8 ldo5_ctrl_init[] = { 0x00, 0x80, 0x81, 0x2d, 0xa4 };

static int mt6360_ldo_init(void)
{
	unsigned int i = 0;
	int ret = 0;

	/* ldo init */
	for (i = 0; i < ARRAY_SIZE(ldo_ctrl_mask); i++) {
		ret = mt6360_update_bits(0x04 + i, ldo_ctrl_mask[i],
					 ldo3_ctrl_init[i]);
		if (ret < 0) {
			dbg_print("%s: reg 0x%x set fail\n",
				  __func__, (0x04 + i));
			return ret;
		}
		ret = mt6360_update_bits(0x0a + i, ldo_ctrl_mask[i],
					 ldo5_ctrl_init[i]);
		if (ret < 0) {
			dbg_print("%s: reg 0x%x set fail\n",
				  __func__, (0x0a + i));
			return ret;
		}
		ret = mt6360_update_bits(0x10 + i, ldo_ctrl_mask[i],
					 ldo2_ctrl_init[i]);
		if (ret < 0) {
			dbg_print("%s: reg 0x%x set fail\n",
				  __func__, (0x10 + i));
			return ret;
		}
		ret = mt6360_update_bits(0x16 + i, ldo_ctrl_mask[i],
					 ldo1_ctrl_init[i]);
		if (ret < 0) {
			dbg_print("%s: reg 0x%x set fail\n",
				  __func__, (0x16 + i));
			return ret;
		}
	}
	return 0;
}

static int mt6360_ldo_is_hw_exist(void)
{
	/* pmu i2c */
	struct mt_i2c_t mt6360_pmu_i2c = {
		.id = 0x05,
		.addr = 0x34,
		.mode = ST_MODE,
		.speed = 400,
		.pushpull = 0,
	};
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

int mt6360_ldo_i2c_probe(void)
{
	int ret;

	dbg_print("%s: ++\n", __func__);
	ret = mt6360_ldo_is_hw_exist();
	if (ret < 0) {
		dbg_print("%s: is not exist\n", __func__);
		return -1;
	}

	crc8_populate_msb(crc8_table, 0x7);

	ret = mt6360_ldo_init();
	if (ret < 0)
		dbg_print("%s probe fail!\n", __func__);

	dbg_print("%s: --\n", __func__);
	return ret;
}
