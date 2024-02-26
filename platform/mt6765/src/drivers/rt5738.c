#ifdef CFG_PRELOADER
#include <platform.h>
#include <gpio.h>
#endif
#include <i2c.h>
#include <rt5738.h>

#ifdef CFG_PRELOADER
#if CFG_FPGA_PLATFORM
#else
#include "cust_i2c.h"
#include "cust_gpio_usage.h"
#endif
#endif

/* for reduce code size */
/*#define	EXTBUCK_RT5738	1*/
/**********************************************************
  *   I2C Slave Setting
  *********************************************************/

/* VDD2 */
#define RT5738_CHIP_NAME_0		"rt5738_vdd2"
#define RT5738_I2C_CHANNEL_REG0		I2C3
#define RT5738_SLAVE_ADDR_REG0		0x57
#define RT5738_BUCK_CTRL_REG0		(RT5738_VSEL0)
#define RT5738_MODE_SHIFT_REG0		(0)
#define RT5738_EN_SHIFT_REG0		(0)
/* VDDQ */
#define RT5738_CHIP_NAME_1		"rt5738_vddq"
#define RT5738_I2C_CHANNEL_REG1		I2C3
#define RT5738_SLAVE_ADDR_REG1		0x50
#define RT5738_BUCK_CTRL_REG1		(RT5738_VSEL1)
#define RT5738_MODE_SHIFT_REG1		(1)
#define RT5738_EN_SHIFT_REG1		(1)

/**********************************************************
  *   Global Variable
  *********************************************************/
static struct mt_i2c_t rt5738_i2c;

int g_rt5738_driver_ready[RT5738_MAX] = {0};
int g_rt5738_hw_exist[RT5738_MAX] = {0};


#define rt5738_print(fmt, args...)	\
do {					\
    print(fmt, ##args);			\
} while (0)

#define DECL_CHIP(_id)			\
{					\
	.id = _id,			\
	.name = RT5738_CHIP_NAME_##_id,	\
	.i2c_channel = RT5738_I2C_CHANNEL_REG##_id,	\
	.slave_addr = RT5738_SLAVE_ADDR_REG##_id,	\
	.buck_ctrl = RT5738_BUCK_CTRL_REG##_id,		\
	.mode_shift = RT5738_MODE_SHIFT_REG##_id,	\
	.en_shift = RT5738_EN_SHIFT_REG##_id,		\
	.chip_id = 0,					\
}

static struct rt5738_chip rt5738_chip[RT5738_MAX] = {
	DECL_CHIP(0),
	DECL_CHIP(1),
};


/**********************************************************
  *
  *   [I2C Function For Read/Write rt5738]
  *
  *********************************************************/
kal_uint32 rt5738_write_byte(struct rt5738_chip *chip,
			     kal_uint8 addr, kal_uint8 value)
{
	int ret_code = I2C_OK;
	kal_uint8 write_data[2];
	kal_uint16 len;

	if (chip == NULL) {
		rt5738_print("%s: chip not exist\n",__func__);
		return 0;
	}

	write_data[0] = addr;
	write_data[1] = value;

	rt5738_i2c.id = chip->i2c_channel;

	/* Since i2c will left shift 1 bit, we need to set rt5738 I2C address to >>1 */
	rt5738_i2c.addr = (chip->slave_addr);
	rt5738_i2c.mode = ST_MODE;
	rt5738_i2c.speed = 100;
	rt5738_i2c.pushpull = 0;
	len = 2;

#if defined(GPIO_VMBUCK_I2C_SDA) & defined(GPIO_VMBUCK_I2C_SCL)
	/* switch to I2C mode */
	mt_set_gpio_mode(GPIO_VMBUCK_I2C_SDA, GPIO_VMBUCK_I2C_SDA_M_SDA1_);
	mt_set_gpio_mode(GPIO_VMBUCK_I2C_SCL, GPIO_VMBUCK_I2C_SCL_M_SCL1_);
#endif

	ret_code = i2c_write(&rt5738_i2c, write_data, len);
	/* rt5738_print("%s: i2c_write: ret_code: %d\n", __func__, ret_code); */

#if defined(GPIO_VMBUCK_I2C_SDA) & defined(GPIO_VMBUCK_I2C_SCL)
	/* Switch to GPIO mode */
	mt_set_gpio_mode(GPIO_VMBUCK_I2C_SDA, GPIO_VMBUCK_I2C_SDA_M_GPIO);
	mt_set_gpio_mode(GPIO_VMBUCK_I2C_SCL, GPIO_VMBUCK_I2C_SCL_M_GPIO);
#endif

	if (ret_code == 0)
		return 1;	/* ok */
	else
		return 0;	/* fail */
}

kal_uint32 rt5738_read_byte(struct rt5738_chip *chip, kal_uint8 addr,
			    kal_uint8 *dataBuffer)
{
	int ret_code = I2C_OK;
	kal_uint16 len;
	*dataBuffer = addr;

	if (chip == NULL) {
		rt5738_print("%s: chip not exist\n",__func__);
		return 0;
	}

	rt5738_i2c.id = chip->i2c_channel;
	/* Since i2c will left shift 1 bit, we need to set rt5738 I2C address to >>1 */
	rt5738_i2c.addr = (chip->slave_addr);
	rt5738_i2c.mode = ST_MODE;
	rt5738_i2c.speed = 100;
	rt5738_i2c.pushpull = 0;
	len = 1;

#if defined(GPIO_VMBUCK_I2C_SDA) & defined(GPIO_VMBUCK_I2C_SCL)
	/* switch to I2C mode */
	mt_set_gpio_mode(GPIO_VMBUCK_I2C_SDA, GPIO_VMBUCK_I2C_SDA_M_SDA1_);
	mt_set_gpio_mode(GPIO_VMBUCK_I2C_SCL, GPIO_VMBUCK_I2C_SCL_M_SCL1_);
#endif

	ret_code = i2c_write_read(&rt5738_i2c, dataBuffer, len, len);

#if defined(GPIO_VMBUCK_I2C_SDA) & defined(GPIO_VMBUCK_I2C_SCL)
	/* switch to GPIO mode */
	mt_set_gpio_mode(GPIO_VMBUCK_I2C_SDA, GPIO_VMBUCK_I2C_SDA_M_GPIO);
	mt_set_gpio_mode(GPIO_VMBUCK_I2C_SCL, GPIO_VMBUCK_I2C_SCL_M_GPIO);
#endif

	/* rt5738_print("%s: i2c_read: ret_code: %d\n", __func__, ret_code); */

	if (ret_code == 0)
		return 1;	/* ok */
	else
		return 0;	/* fail */
}

/**********************************************************
  *
  *   [Read / Write Function]
  *
  *********************************************************/
kal_uint32 rt5738_read_interface(struct rt5738_chip *chip, kal_uint8 RegNum,
			 kal_uint8 *val, kal_uint8 MASK, kal_uint8 SHIFT)
{
	kal_uint8 rt5738_reg = 0;
	kal_uint32 ret = 0;

	/* rt5738_print("--------------------------------------------------PL\n"); */

	ret = rt5738_read_byte(chip, RegNum, &rt5738_reg);
	/* rt5738_print("[rt5738_read_interface] Reg[%x]=0x%x\n", RegNum, rt5738_reg); */

	rt5738_reg &= (MASK << SHIFT);
	*val = (rt5738_reg >> SHIFT);
	/* rt5738_print("[rt5738_read_interface] val=0x%x\n", *val); */

	return ret;
}

kal_uint32 rt5738_config_interface(struct rt5738_chip *chip, kal_uint8 RegNum,
			   kal_uint8 val, kal_uint8 MASK, kal_uint8 SHIFT)
{
	kal_uint8 rt5738_reg = 0;
	kal_uint32 ret = 0;

	/* rt5738_print("--------------------------------------------------PL\n"); */

	ret = rt5738_read_byte(chip, RegNum, &rt5738_reg);
	/* rt5738_print("[rt5738_config_interface] Reg[%x]=0x%x\n", RegNum, rt5738_reg); */

	rt5738_reg &= ~(MASK << SHIFT);
	rt5738_reg |= (val << SHIFT);

	ret = rt5738_write_byte(chip, RegNum, rt5738_reg);
	/* rt5738_print("[rt5738_config_interface] write Reg[%x]=0x%x\n", RegNum, rt5738_reg); */

	/* Check */
	/* rt5738_read_byte(RegNum, &rt5738_reg); */
	/* rt5738_print("[rt5738_config_interface] Check Reg[%x]=0x%x\n", RegNum, rt5738_reg); */

	return ret;
}

#if defined(EXTBUCK_RT5738)
kal_uint32 rt5738_get_reg_value(struct rt5738_chip *chip, kal_uint32 reg)
{
	kal_uint32 ret = 0;
	kal_uint8 reg_val = 0;

	ret = rt5738_read_interface(chip, (kal_uint8) reg, &reg_val, 0xFF, 0x0);

	if (ret == 0)
		rt5738_print("ret=%d\n", ret);
	return reg_val;
}

void rt5738_hw_init(struct rt5738_chip *chip)
{
	rt5738_print("%s_hw_init\n", chip->name);
}

kal_uint8 rt5738_get_id1(struct rt5738_chip *chip)
{
	kal_uint8 val;

	rt5738_read_interface(chip, RT5738_ID1, &val, 0xff, 0);
	return val;
}

kal_uint8 rt5738_get_id2(struct rt5738_chip *chip)
{
	kal_uint8 val;
	rt5738_read_interface(chip, RT5738_ID2, &val, 0xff, 0);
	return val;
}

void update_rt5738_chip_id(struct rt5738_chip *chip)
{
	kal_uint32 id = 0;
	kal_uint32 id_l = 0;
	kal_uint32 id_r = 0;

	id_l = rt5738_get_id1(chip);
	id_r = rt5738_get_id2(chip);
	id = ((id_l << 8) | (id_r));

	chip->chip_id = id;
}

void rt5738_hw_component_detect(struct rt5738_chip *chip)
{
	int ret = 0;
	unsigned char PGOOD = 0;
	int chip_id = 0;

	update_rt5738_chip_id(chip);
	ret = rt5738_read_interface(chip, 0x05, &PGOOD, 0x1, 7);

	if (chip->chip_id == RT5738_VDD2)
		chip_id = RT5738_VDD2_ID;
	else if (chip->chip_id == RT5738_VDDQ)
		chip_id = RT5738_VDDQ_ID;

	if (chip->chip_id == chip_id && PGOOD) {
		g_rt5738_hw_exist[chip->id] = 1;
	} else {
		g_rt5738_hw_exist[chip->id] = 0;
	}
	rt5738_print("[%s] exist = %d, Chip ID = %x\n"
		     , __func__
		     , g_rt5738_hw_exist[chip->id], chip->chip_id);
}
#endif

static struct rt5738_chip *rt5738_find_chip(int id)
{
	struct rt5738_chip *chip;
	int i;

	for (i = 0; i<RT5738_MAX; i++) {
		chip = &rt5738_chip[i];
		if (chip->id == id)
			return chip;
	}
	return NULL;
}

#if defined(EXTBUCK_RT5738)
void rt5738_dump_register(int id)
{
	kal_uint8 i = 0x0;
	kal_uint8 i_max = 0x6;	/* 0x06 */
	struct rt5738_chip *chip = rt5738_find_chip(id);

	for (i = 0x0; i <= i_max; i++) {
		rt5738_print("[0x%x]=0x%x ", i, rt5738_get_reg_value(chip, i));
	}
}

int is_rt5738_exist(int id)
{
	rt5738_print("g_rt5738_hw_exist=%d\n", g_rt5738_hw_exist[id]);

	return g_rt5738_hw_exist[id];
}
#endif

int rt5738_enable(int id, unsigned char en)
{
	int ret = 1;
	struct rt5738_chip *chip = rt5738_find_chip(id);

	ret = rt5738_config_interface(chip, RT5738_CTRL2, en
				      , 0x1, chip->en_shift);

	return ret;
}

#if defined(EXTBUCK_RT5738)
int rt5738_is_enabled(int id)
{
	int ret = 1;
	unsigned char en;
	struct rt5738_chip *chip = rt5738_find_chip(id);

	ret = rt5738_read_interface(chip, RT5738_CTRL2, &en
				    , 0x1, chip->en_shift);
	if (ret == 0)
		return -1;
	return en;
}
#endif

int rt5738_set_voltage(int id, unsigned long val)
{
	int ret = 1;
	unsigned long reg_val = 0;
	struct rt5738_chip *chip = rt5738_find_chip(id);

	if (val < 1300000)
		reg_val = (val - 300000)/5000;
	else
		reg_val = (val - 1300000)/10000 + 200;

	if (reg_val > 255)
		reg_val = 255;

	ret = rt5738_config_interface(chip, chip->buck_ctrl, reg_val
				      , 0xFF, 0);

	return ret;
}

unsigned long rt5738_get_voltage(int id)
{
	int ret = 1;
	unsigned char reg_val = 0;
	struct rt5738_chip *chip = rt5738_find_chip(id);

	ret = rt5738_read_interface(chip, chip->buck_ctrl, &reg_val
				    , 0xFF, 0);
	if (ret == 0)
		return 0;

	if (reg_val > 200)
		return 1300000 + (reg_val - 200) * 10000;
	return 300000 + reg_val * 5000;
}

#if 0
unsigned char rt5738_get_vsel0(int id)
{
	int ret = 1;
	unsigned char reg_val = 0;
	struct rt5738_chip *chip = rt5738_find_chip(id);

	ret = rt5738_read_byte(chip, RT5738_VSEL0, &reg_val);
	if (ret == 0)
		return 0;
	return reg_val;
}
#endif

int rt5738_set_mode(int id, unsigned char mode)
{
	int ret;
	struct rt5738_chip *chip = rt5738_find_chip(id);

	ret = rt5738_config_interface(chip, RT5738_CONTROL, mode
				      , 0x1, chip->mode_shift);
	return ret;
}

#if defined(EXTBUCK_RT5738)
unsigned char rt5738_get_mode(int id)
{
	int ret;
	unsigned char reg_val = 0;
	struct rt5738_chip *chip = rt5738_find_chip(id);

	ret = rt5738_read_interface(chip, RT5738_CONTROL, &reg_val
				    , 0x01, chip->mode_shift);
	if (ret < 0)
		return 0;

	return reg_val;
}

void rt5738_driver_probe(void)
{
	int id = 0;
	struct rt5738_chip *chip;

	rt5738_print("[%s]\n", __func__);
	for (id = 0; id < RT5738_MAX; id++) {
		chip = &rt5738_chip[id];
		rt5738_hw_component_detect(chip);
		if (g_rt5738_hw_exist[id] == 1) {
			rt5738_hw_init(chip);
			rt5738_dump_register(id);
			g_rt5738_driver_ready[id] = 1;

			rt5738_print("[%s] PL g_rt5738_%d_hw_exist=%d, g_rt5738_driver_ready=%d\n"
				     , __func__
				     , id
				     , g_rt5738_hw_exist[id]
				     , g_rt5738_driver_ready[id]);

		} else {
			rt5738_print("[%s] PL %s is not exist\n"
				     , __func__
				     , chip->name);
		}
	}
}
#endif
