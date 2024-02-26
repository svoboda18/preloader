#include <platform.h>
#include <i2c.h>
#include <gpio.h>
#include <rt5738.h>

#if CFG_FPGA_PLATFORM
#else
#include "cust_i2c.h"
#include "cust_gpio_usage.h"
#endif

/* rt5738 */
/**********************************************************
  *   I2C Slave Setting
  *********************************************************/
#define RT5738_SLAVE_ADDR_WRITE   0xA0
#define RT5738_SLAVE_ADDR_READ    0xA1

/**********************************************************
  *   Global Variable
  *********************************************************/
/* #ifdef I2C_EXT_BUCK_CHANNEL */
/* #define RT5738_I2C_ID I2C_EXT_BUCK_CHANNEL */
/* #else */
#define RT5738_I2C_ID I2C0

#define RT5738_BUCK_CTRL_REG	(RT5738_VSEL1)
#define RT5738_BUCK_EN_SHIFT	(1)
#define RT5738_MODE_SHIFT	(1)
/* #endif */

static struct mt_i2c_t rt5738_i2c;

int g_rt5738_driver_ready = 0;
int g_rt5738_hw_exist = 0;

#define rt5738_print(fmt, args...)   \
do {									\
    printf(fmt, ##args); \
} while (0)

kal_uint32 g_rt5738_cid = 0;

/**********************************************************
  *
  *   [I2C Function For Read/Write rt5738]
  *
  *********************************************************/
kal_uint32 rt5738_write_byte(kal_uint8 addr, kal_uint8 value)
{
	int ret_code = I2C_OK;
	kal_uint8 write_data[2];
	kal_uint16 len;

	write_data[0] = addr;
	write_data[1] = value;

	rt5738_i2c.id = RT5738_I2C_ID;
	/* Since i2c will left shift 1 bit, we need to set rt5738 I2C address to >>1 */
	rt5738_i2c.addr = (RT5738_SLAVE_ADDR_WRITE >> 1);
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

kal_uint32 rt5738_read_byte(kal_uint8 addr, kal_uint8 *dataBuffer)
{
	int ret_code = I2C_OK;
	kal_uint16 len;
	*dataBuffer = addr;

	rt5738_i2c.id = RT5738_I2C_ID;
	/* Since i2c will left shift 1 bit, we need to set rt5738 I2C address to >>1 */
	rt5738_i2c.addr = (RT5738_SLAVE_ADDR_WRITE >> 1);
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
kal_uint32 rt5738_read_interface(kal_uint8 RegNum, kal_uint8 *val, kal_uint8 MASK,
				   kal_uint8 SHIFT)
{
	kal_uint8 rt5738_reg = 0;
	kal_uint32 ret = 0;

	/* rt5738_print("--------------------------------------------------PL\n"); */

	ret = rt5738_read_byte(RegNum, &rt5738_reg);
	/* rt5738_print("[rt5738_read_interface] Reg[%x]=0x%x\n", RegNum, rt5738_reg); */

	rt5738_reg &= (MASK << SHIFT);
	*val = (rt5738_reg >> SHIFT);
	/* rt5738_print("[rt5738_read_interface] val=0x%x\n", *val); */

	return ret;
}

kal_uint32 rt5738_config_interface(kal_uint8 RegNum, kal_uint8 val, kal_uint8 MASK,
				     kal_uint8 SHIFT)
{
	kal_uint8 rt5738_reg = 0;
	kal_uint32 ret = 0;

	/* rt5738_print("--------------------------------------------------PL\n"); */

	ret = rt5738_read_byte(RegNum, &rt5738_reg);
	/* rt5738_print("[rt5738_config_interface] Reg[%x]=0x%x\n", RegNum, rt5738_reg); */

	rt5738_reg &= ~(MASK << SHIFT);
	rt5738_reg |= (val << SHIFT);

	ret = rt5738_write_byte(RegNum, rt5738_reg);
	/* rt5738_print("[rt5738_config_interface] write Reg[%x]=0x%x\n", RegNum, rt5738_reg); */

	/* Check */
	/* rt5738_read_byte(RegNum, &rt5738_reg); */
	/* rt5738_print("[rt5738_config_interface] Check Reg[%x]=0x%x\n", RegNum, rt5738_reg); */

	return ret;
}

kal_uint32 rt5738_get_reg_value(kal_uint32 reg)
{
	kal_uint32 ret = 0;
	kal_uint8 reg_val = 0;

	ret = rt5738_read_interface((kal_uint8) reg, &reg_val, 0xFF, 0x0);

	if (ret == 0)
		rt5738_print("%d", ret);
	return reg_val;
}

void rt5738_dump_register(void)
{
#if 1
	kal_uint8 i = 0x0;
	kal_uint8 i_max = 0x5;	/* 0x05 */

	for (i = 0x0; i <= i_max; i++) {
		rt5738_print("[0x%x]=0x%x ", i, rt5738_get_reg_value(i));
	}
#endif
}

int get_rt5738_i2c_ch_num(void)
{
	return RT5738_I2C_ID;
}

void rt5738_hw_init(void)
{
	rt5738_print("rt5738_hw_init\n");
}

kal_uint8 rt5738_get_id1(void)
{
	kal_uint8 val;

	rt5738_read_interface(RT5738_ID1, &val, 0xff, 0);
	return val;
}

kal_uint8 rt5738_get_id2(void)
{
	kal_uint8 val;
	rt5738_read_interface(RT5738_ID2, &val, 0xff, 0);
	return val;
}

kal_uint32 update_rt5738_chip_id(void)
{
	kal_uint32 id = 0;
	kal_uint32 id_l = 0;
	kal_uint32 id_r = 0;

	id_l = rt5738_get_id1();
	id_r = rt5738_get_id2();
	id = ((id_l << 8) | (id_r));

	g_rt5738_cid = id;
	return id;
}

void rt5738_hw_component_detect(void)
{
	int ret = 0;
	unsigned char PGOOD = 0;

	update_rt5738_chip_id();
	ret = rt5738_read_interface(0x05, &PGOOD, 0x1, 7);
	if (g_rt5738_cid == RT5738_ID && PGOOD) {
		g_rt5738_hw_exist = 1;
	} else {
		g_rt5738_hw_exist = 0;
	}
	rt5738_print("[rt5738_hw_component_detect] exist = %d, Chip ID = %x\n", g_rt5738_hw_exist, g_rt5738_cid);
}

int is_rt5738_exist(void)
{
	rt5738_print("g_rt5738_hw_exist=%d\n", g_rt5738_hw_exist);

	return g_rt5738_hw_exist;
}

int rt5738_enable(unsigned char en)
{
	int ret = 1;

	ret = rt5738_config_interface(RT5738_CTRL2, en, 0x1, RT5738_BUCK_EN_SHIFT);

	return ret;
}

int rt5738_is_enabled(void)
{
	int ret = 1;
	unsigned char en;

	ret = rt5738_read_interface(RT5738_BUCK_CTRL_REG, &en, 0x1, RT5738_BUCK_EN_SHIFT);
	if (ret < 0)
		return -1;
	return en;
}

int rt5738_set_voltage(unsigned long val)
{
	int ret = 1;
	unsigned long reg_val = 0;

	/*  0.3 ~ 1.85V*/

	if (val < 1300000)
		reg_val = (val - 300000)/5000;
	else
		reg_val = (val - 1300000)/10000;

	if (reg_val > 255)
		reg_val = 255;

	ret = rt5738_config_interface(RT5738_BUCK_CTRL_REG, reg_val, 0xFF, 0);

	return ret;
}

unsigned long rt5738_get_voltage(void)
{
	int ret = 1;
	unsigned char reg_val = 0;

	ret = rt5738_read_interface(RT5738_BUCK_CTRL_REG, &reg_val, 0xFF, 0);
	if (ret < 0)
		return 0;

	if (reg_val > 200)
		return 1300000 + reg_val * 10000;
	return 300000 + reg_val * 5000; 
}

int rt5738_set_mode(unsigned char mode)
{
	int ret;

	ret = rt5738_config_interface(RT5738_CONTROL, mode, 0x1, RT5738_MODE_SHIFT);
	return ret;
}

void rt5738_driver_probe(void)
{
	rt5738_hw_component_detect();
	rt5738_print("[%s]\n", __func__);
	if (g_rt5738_hw_exist == 1) {
		rt5738_hw_init();
		rt5738_dump_register();
	} else {
		rt5738_print("[%s] PL rt5738 is not exist\n", __func__);
	}
	g_rt5738_driver_ready = 1;

	rt5738_print
	    ("[%s] PL g_rt5738_hw_exist=%d, g_rt5738_driver_ready=%d\n",
	     __func__, g_rt5738_hw_exist, g_rt5738_driver_ready);

	/* -------------------------------------------------------- */

#ifdef I2C_EXT_BUCK_CHANNEL
	rt5738_print("[%s] PL I2C_EXT_BUCK_CHANNEL=%d.\n",
		       __func__, I2C_EXT_BUCK_CHANNEL);
#else
	rt5738_print("[%s] PL No I2C_EXT_BUCK_CHANNEL (%d)\n",
		       __func__, RT5738_I2C_ID);
#endif

}
