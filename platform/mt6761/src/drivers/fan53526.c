#include <platform.h>
#include <i2c.h>
#include <gpio.h>
#include <fan53526.h>

#if CFG_FPGA_PLATFORM
#else
#include "cust_i2c.h"
#include "cust_gpio_usage.h"
#endif

/* FAN53526 */
/**********************************************************
  *   I2C Slave Setting
  *********************************************************/
#define FAN53526_SLAVE_ADDR_WRITE   0xC0
#define FAN53526_SLAVE_ADDR_READ    0xC1

/**********************************************************
  *   Global Variable
  *********************************************************/
/* #ifdef I2C_EXT_BUCK_CHANNEL */
/* #define fan53526_I2C_ID I2C_EXT_BUCK_CHANNEL */
/* #else */
#define fan53526_I2C_ID I2C1
/* #endif */

static struct mt_i2c_t fan53526_i2c;

int g_fan53526_driver_ready = 0;
int g_fan53526_hw_exist = 0;

#define fan53526_print(fmt, args...)   \
do {									\
    printf(fmt, ##args); \
} while (0)

kal_uint32 g_fan53526_cid = 0;

/**********************************************************
  *
  *   [I2C Function For Read/Write fan53526]
  *
  *********************************************************/
kal_uint32 fan53526_write_byte(kal_uint8 addr, kal_uint8 value)
{
	int ret_code = I2C_OK;
	kal_uint8 write_data[2];
	kal_uint16 len;

	write_data[0] = addr;
	write_data[1] = value;

	fan53526_i2c.id = fan53526_I2C_ID;
	/* Since i2c will left shift 1 bit, we need to set fan53526 I2C address to >>1 */
	fan53526_i2c.addr = (FAN53526_SLAVE_ADDR_WRITE >> 1);
	fan53526_i2c.mode = ST_MODE;
	fan53526_i2c.speed = 100;
	fan53526_i2c.pushpull = 0;
	len = 2;

#if defined(GPIO_VMBUCK_I2C_SDA) & defined(GPIO_VMBUCK_I2C_SCL)
	/* switch to I2C mode */
	mt_set_gpio_mode(GPIO_VMBUCK_I2C_SDA, GPIO_VMBUCK_I2C_SDA_M_SDA1_);
	mt_set_gpio_mode(GPIO_VMBUCK_I2C_SCL, GPIO_VMBUCK_I2C_SCL_M_SCL1_);
#endif

	ret_code = i2c_write(&fan53526_i2c, write_data, len);
	/* fan53526_print("%s: i2c_write: ret_code: %d\n", __func__, ret_code); */

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

kal_uint32 fan53526_read_byte(kal_uint8 addr, kal_uint8 *dataBuffer)
{
	int ret_code = I2C_OK;
	kal_uint16 len;
	*dataBuffer = addr;

	fan53526_i2c.id = fan53526_I2C_ID;
	/* Since i2c will left shift 1 bit, we need to set fan53526 I2C address to >>1 */
	fan53526_i2c.addr = (FAN53526_SLAVE_ADDR_WRITE >> 1);
	fan53526_i2c.mode = ST_MODE;
	fan53526_i2c.speed = 100;
	fan53526_i2c.pushpull = 0;
	len = 1;

#if defined(GPIO_VMBUCK_I2C_SDA) & defined(GPIO_VMBUCK_I2C_SCL)
	/* switch to I2C mode */
	mt_set_gpio_mode(GPIO_VMBUCK_I2C_SDA, GPIO_VMBUCK_I2C_SDA_M_SDA1_);
	mt_set_gpio_mode(GPIO_VMBUCK_I2C_SCL, GPIO_VMBUCK_I2C_SCL_M_SCL1_);
#endif

	ret_code = i2c_write_read(&fan53526_i2c, dataBuffer, len, len);

#if defined(GPIO_VMBUCK_I2C_SDA) & defined(GPIO_VMBUCK_I2C_SCL)
	/* switch to GPIO mode */
	mt_set_gpio_mode(GPIO_VMBUCK_I2C_SDA, GPIO_VMBUCK_I2C_SDA_M_GPIO);
	mt_set_gpio_mode(GPIO_VMBUCK_I2C_SCL, GPIO_VMBUCK_I2C_SCL_M_GPIO);
#endif

	/* fan53526_print("%s: i2c_read: ret_code: %d\n", __func__, ret_code); */

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
kal_uint32 fan53526_read_interface(kal_uint8 RegNum, kal_uint8 *val, kal_uint8 MASK,
				   kal_uint8 SHIFT)
{
	kal_uint8 fan53526_reg = 0;
	kal_uint32 ret = 0;

	/* fan53526_print("--------------------------------------------------PL\n"); */

	ret = fan53526_read_byte(RegNum, &fan53526_reg);
	/* fan53526_print("[fan53526_read_interface] Reg[%x]=0x%x\n", RegNum, fan53526_reg); */

	fan53526_reg &= (MASK << SHIFT);
	*val = (fan53526_reg >> SHIFT);
	/* fan53526_print("[fan53526_read_interface] val=0x%x\n", *val); */

	return ret;
}

kal_uint32 fan53526_config_interface(kal_uint8 RegNum, kal_uint8 val, kal_uint8 MASK,
				     kal_uint8 SHIFT)
{
	kal_uint8 fan53526_reg = 0;
	kal_uint32 ret = 0;

	/* fan53526_print("--------------------------------------------------PL\n"); */

	ret = fan53526_read_byte(RegNum, &fan53526_reg);
	/* fan53526_print("[fan53526_config_interface] Reg[%x]=0x%x\n", RegNum, fan53526_reg); */

	fan53526_reg &= ~(MASK << SHIFT);
	fan53526_reg |= (val << SHIFT);

	ret = fan53526_write_byte(RegNum, fan53526_reg);
	/* fan53526_print("[fan53526_config_interface] write Reg[%x]=0x%x\n", RegNum, fan53526_reg); */

	/* Check */
	/* fan53526_read_byte(RegNum, &fan53526_reg); */
	/* fan53526_print("[fan53526_config_interface] Check Reg[%x]=0x%x\n", RegNum, fan53526_reg); */

	return ret;
}

kal_uint32 fan53526_get_reg_value(kal_uint32 reg)
{
	kal_uint32 ret = 0;
	kal_uint8 reg_val = 0;

	ret = fan53526_read_interface((kal_uint8) reg, &reg_val, 0xFF, 0x0);

	if (ret == 0)
		fan53526_print("%d", ret);
	return reg_val;
}

void fan53526_dump_register(void)
{
#if 1
	kal_uint8 i = 0x0;
	kal_uint8 i_max = 0x5;	/* 0xD5 */

	for (i = 0x0; i <= i_max; i++) {
		fan53526_print("[0x%x]=0x%x ", i, fan53526_get_reg_value(i));
	}
#endif
}

int get_fan53526_i2c_ch_num(void)
{
	return fan53526_I2C_ID;
}

void fan53526_hw_init(void)
{
	fan53526_print("fan53526_hw_init\n");
}

kal_uint8 fan53526_get_id1(void)
{
	kal_uint8 val;

	fan53526_read_interface(FAN53526_ID1, &val, 0xff, 0);
	return val;
}

kal_uint8 fan53526_get_id2(void)
{
	kal_uint8 val;
	fan53526_read_interface(FAN53526_ID2, &val, 0xff, 0);
	return val;
}

kal_uint32 update_fan53526_chip_id(void)
{
	kal_uint32 id = 0;
	kal_uint32 id_l = 0;
	kal_uint32 id_r = 0;

	id_l = fan53526_get_id1();
	id_r = fan53526_get_id2();
	id = ((id_l << 8) | (id_r));

	g_fan53526_cid = id;

	/* fan53526_print("[update_fan53526_chip_id] id_l=0x%x, id_r=0x%x, id=0x%x\n", id_l, id_r, id); */

	return id;
}

void fan53526_hw_component_detect(void)
{
	int ret = 0;
	unsigned char PGOOD = 0;

	update_fan53526_chip_id();

	ret = fan53526_read_interface(0x05, &PGOOD, 0x1, 7);


	if (g_fan53526_cid == FAN53526_ID && PGOOD) {
		g_fan53526_hw_exist = 1;
	} else {
		g_fan53526_hw_exist = 0;
	}
	fan53526_print("[fan53526_hw_component_detect] exist = %d PGOOD = %d, Chip ID = %x\n", g_fan53526_hw_exist, PGOOD, g_fan53526_cid);
}

#if 0
int is_fan53526_exist(void)
{
	fan53526_print("g_fan53526_hw_exist=%d\n", g_fan53526_hw_exist);

	return g_fan53526_hw_exist;
}

int fan53526_vosel(unsigned long val)
{
	int ret = 1;
	unsigned int reg_val = 0;


	reg_val = (((val * 1000) - 603000) + 12825) / 12826;
	if (reg_val > 63)
		reg_val = 63;

	reg_val = reg_val | 0x80;
	ret = fan53526_write_byte(0x00, reg_val);

	pr_notice("[fan53526_vosel] val=%ld, reg_val=%ld\n", val, reg_val);

	return ret;
}
#endif

int is_fan53526_exist(void)
{
	fan53526_print("g_fan53526_hw_exist=%d\n", g_fan53526_hw_exist);

	return g_fan53526_hw_exist;
}

int fan53526_is_enabled(void)
{
    int ret=1;
    unsigned char en=0;

    ret = fan53526_read_interface(0x00, &en, 0x1, 7);

    fan53526_print("[fan53526_is_enabled] en = %d\n", en);

    return en;
}

/* en = 1 enable */
/* en = 0 disable */
int fan53526_enable(unsigned char en)
{
	int ret = 1;

	if (en > 1) {
		fan53526_print("[fan53526_enable] error en = %d only 0 or 1\n", en);
		return -1;
	}

	ret = fan53526_config_interface(0x00, en, 0x1, 7);

	fan53526_print("[fan53526_enable]=%d\n", en);

	return ret;
}

int fan53526_set_voltage(unsigned long val)
{
	int ret = 1;
	unsigned long reg_val = 0;

	/* 0.603~1.411V (step 12.826mv) */
	/*reg_val = (((val * 1000) - 603000) + 6413) / 12826;*/
	/* 0.6~1.39375V (step 6.25mv) */
	if (val < 600000) {
		fan53526_print("[fan53526_set_voltage] val = %ld < 600000uV \n", val);
		return -1;
	}

	reg_val = (val - 600000) / 6250;

	if (reg_val > 127)
		reg_val = 127;

	ret = fan53526_config_interface(0x00, reg_val, 0x7F, 0);
	/* ret = fan53526_config_interface(0x00, 0x1, 0x1, 7); */

	/* pr_notice("[fan53526_vosel] val=%ld, reg_val=%ld\n", val, reg_val); */

	return ret;
}

unsigned int fan53526_get_voltage(void)
{
    unsigned int ret = 0, vol = 0;
    unsigned char step = 0;
    
    ret = fan53526_read_interface(0x00, &step, 0x7F, 0);

    vol = ((step * 6250 ) + 600000);

    return vol;
}

/* mode = 3 force PWM mode no matter vsel H/L */
/* mode = 0 auto mode      */
int fan53526_set_mode(unsigned char mode)
{
	int ret = 1;

	if (mode != 0 && mode != 3 ) {
		fan53526_print("[fan53526_set_mode] error mode = %d only 0 or 1\n", mode);
		return -1;
	}

	ret = fan53526_config_interface(0x02, mode, 0x3, 0);
	/* ret = fan53526_config_interface(0x00, 0x1, 0x1, 7); */

	/* pr_notice("[fan53526_vosel] val=%ld, reg_val=%ld\n", val, reg_val); */
	return ret;
}

void fan53526_driver_probe(void)
{
	fan53526_hw_component_detect();
	fan53526_print("[fan53526_driver_probe]\n");
	if (g_fan53526_hw_exist == 1) {
		fan53526_hw_init();
		fan53526_dump_register();
	} else {
		fan53526_print("[fan53526_driver_probe] PL fan53526 is not exist\n");
	}
	g_fan53526_driver_ready = 1;

	fan53526_print
	    ("[fan53526_driver_probe] PL g_fan53526_hw_exist=%d, g_fan53526_driver_ready=%d\n",
	     g_fan53526_hw_exist, g_fan53526_driver_ready);

	/* -------------------------------------------------------- */

#ifdef I2C_EXT_BUCK_CHANNEL
	fan53526_print("[fan53526_driver_probe] PL I2C_EXT_BUCK_CHANNEL=%d.\n",
		       I2C_EXT_BUCK_CHANNEL);
#else
	fan53526_print("[fan53526_driver_probe] PL No I2C_EXT_BUCK_CHANNEL (%d)\n",
		       fan53526_I2C_ID);
#endif

}
