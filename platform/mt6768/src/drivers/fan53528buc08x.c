#include <platform.h>
#include <i2c.h>
#include <gpio.h>
#include <fan53528buc08x.h>

#if CFG_FPGA_PLATFORM
#else
#include "cust_i2c.h"
#include "cust_gpio_usage.h"
#endif

/* fan53528BUC08X */
/**********************************************************
  *   I2C Slave Setting
  *********************************************************/
#define fan53528BUC08X_SLAVE_ADDR_WRITE   0xA0
#define fan53528BUC08X_SLAVE_ADDR_READ    0xA1

/**********************************************************
  *   Global Variable
  *********************************************************/
/* #ifdef I2C_EXT_BUCK_CHANNEL */
/* #define fan53528buc08x_I2C_ID I2C_EXT_BUCK_CHANNEL */
/* #else */
#define fan53528buc08x_I2C_ID I2C1

#define FAN53528_BUCK_CTRL_REG	(0x01)
/* #endif */

static struct mt_i2c_t fan53528buc08x_i2c;

int g_fan53528buc08x_driver_ready = 0;
int g_fan53528buc08x_hw_exist = 0;

#define fan53528buc08x_print(fmt, args...)   \
do {									\
    printf(fmt, ##args); \
} while (0)

kal_uint32 g_fan53528buc08x_cid = 0;

/**********************************************************
  *
  *   [I2C Function For Read/Write fan53528buc08x]
  *
  *********************************************************/
kal_uint32 fan53528buc08x_write_byte(kal_uint8 addr, kal_uint8 value)
{
	int ret_code = I2C_OK;
	kal_uint8 write_data[2];
	kal_uint16 len;

	write_data[0] = addr;
	write_data[1] = value;

	fan53528buc08x_i2c.id = fan53528buc08x_I2C_ID;
	/* Since i2c will left shift 1 bit, we need to set fan53528buc08x I2C address to >>1 */
	fan53528buc08x_i2c.addr = (fan53528BUC08X_SLAVE_ADDR_WRITE >> 1);
	fan53528buc08x_i2c.mode = ST_MODE;
	fan53528buc08x_i2c.speed = 100;
	fan53528buc08x_i2c.pushpull = 0;
	len = 2;

#if defined(GPIO_VMBUCK_I2C_SDA) & defined(GPIO_VMBUCK_I2C_SCL)
	/* switch to I2C mode */
	mt_set_gpio_mode(GPIO_VMBUCK_I2C_SDA, GPIO_VMBUCK_I2C_SDA_M_SDA1_);
	mt_set_gpio_mode(GPIO_VMBUCK_I2C_SCL, GPIO_VMBUCK_I2C_SCL_M_SCL1_);
#endif
	
	ret_code = i2c_write(&fan53528buc08x_i2c, write_data, len);
	/* fan53528buc08x_print("%s: i2c_write: ret_code: %d\n", __func__, ret_code); */
	
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

kal_uint32 fan53528buc08x_read_byte(kal_uint8 addr, kal_uint8 *dataBuffer)
{
	int ret_code = I2C_OK;
	kal_uint16 len;
	*dataBuffer = addr;

	fan53528buc08x_i2c.id = fan53528buc08x_I2C_ID;
	/* Since i2c will left shift 1 bit, we need to set fan53528buc08x I2C address to >>1 */
	fan53528buc08x_i2c.addr = (fan53528BUC08X_SLAVE_ADDR_WRITE >> 1);
	fan53528buc08x_i2c.mode = ST_MODE;
	fan53528buc08x_i2c.speed = 100;
	fan53528buc08x_i2c.pushpull = 0;
	len = 1;

#if defined(GPIO_VMBUCK_I2C_SDA) & defined(GPIO_VMBUCK_I2C_SCL)
	/* switch to I2C mode */
	mt_set_gpio_mode(GPIO_VMBUCK_I2C_SDA, GPIO_VMBUCK_I2C_SDA_M_SDA1_);
	mt_set_gpio_mode(GPIO_VMBUCK_I2C_SCL, GPIO_VMBUCK_I2C_SCL_M_SCL1_);
#endif
	        
	ret_code = i2c_write_read(&fan53528buc08x_i2c, dataBuffer, len, len);
		
#if defined(GPIO_VMBUCK_I2C_SDA) & defined(GPIO_VMBUCK_I2C_SCL)
	/* switch to GPIO mode */
	mt_set_gpio_mode(GPIO_VMBUCK_I2C_SDA, GPIO_VMBUCK_I2C_SDA_M_GPIO);
	mt_set_gpio_mode(GPIO_VMBUCK_I2C_SCL, GPIO_VMBUCK_I2C_SCL_M_GPIO);
#endif

	/* fan53528buc08x_print("%s: i2c_read: ret_code: %d\n", __func__, ret_code); */
	
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
kal_uint32 fan53528buc08x_read_interface(kal_uint8 RegNum, kal_uint8 *val, kal_uint8 MASK,
				   kal_uint8 SHIFT)
{
	kal_uint8 fan53528buc08x_reg = 0;
	kal_uint32 ret = 0;

	/* fan53528buc08x_print("--------------------------------------------------PL\n"); */

	ret = fan53528buc08x_read_byte(RegNum, &fan53528buc08x_reg);
	/* fan53528buc08x_print("[fan53528_read_interface] Reg[%x]=0x%x\n", RegNum, fan53528_reg); */

	fan53528buc08x_reg &= (MASK << SHIFT);
	*val = (fan53528buc08x_reg >> SHIFT);
	/* fan53528buc08x_print("[fan53528_read_interface] val=0x%x\n", *val); */

	return ret;
}

kal_uint32 fan53528buc08x_config_interface(kal_uint8 RegNum, kal_uint8 val, kal_uint8 MASK,
				     kal_uint8 SHIFT)
{
	kal_uint8 fan53528buc08x_reg = 0;
	kal_uint32 ret = 0;

	/* fan53528buc08x_print("--------------------------------------------------PL\n"); */

	ret = fan53528buc08x_read_byte(RegNum, &fan53528buc08x_reg);
	/* fan53528buc08x_print("[fan53528_config_interface] Reg[%x]=0x%x\n", RegNum, fan53528_reg); */

	fan53528buc08x_reg &= ~(MASK << SHIFT);
	fan53528buc08x_reg |= (val << SHIFT);

	ret = fan53528buc08x_write_byte(RegNum, fan53528buc08x_reg);
	/* fan53528buc08x_print("[fan53528_config_interface] write Reg[%x]=0x%x\n", RegNum, fan53528_reg); */

	/* Check */
	/* fan53528buc08x_read_byte(RegNum, &fan53528_reg); */
	/* fan53528buc08x_print("[fan53528_config_interface] Check Reg[%x]=0x%x\n", RegNum, fan53528_reg); */

	return ret;
}

kal_uint32 fan53528buc08x_get_reg_value(kal_uint32 reg)
{
	kal_uint32 ret = 0;
	kal_uint8 reg_val = 0;

	ret = fan53528buc08x_read_interface((kal_uint8) reg, &reg_val, 0xFF, 0x0);

	if (ret == 0)
		fan53528buc08x_print("%d", ret);
	return reg_val;
}

void fan53528buc08x_dump_register(void)
{
#if 1
	kal_uint8 i = 0x0;
	kal_uint8 i_max = 0x5;	/* 0x05 */

	for (i = 0x0; i <= i_max; i++) {
		fan53528buc08x_print("[0x%x]=0x%x ", i, fan53528buc08x_get_reg_value(i));
	}
#endif
}

int get_fan53528buc08x_i2c_ch_num(void)
{
	return fan53528buc08x_I2C_ID;
}

void fan53528buc08x_hw_init(void)
{
	fan53528buc08x_print("fan53528_hw_init\n");
}

kal_uint8 fan53528buc08x_get_id1(void)
{
	kal_uint8 val;

	fan53528buc08x_read_interface(fan53528BUC08X_ID1, &val, 0xff, 0);
	return val;
}

kal_uint8 fan53528buc08x_get_id2(void)
{
	kal_uint8 val;
	fan53528buc08x_read_interface(fan53528BUC08X_ID2, &val, 0xff, 0);
	return val;
}

kal_uint32 update_fan53528buc08x_chip_id(void)
{
	kal_uint32 id = 0;
	kal_uint32 id_l = 0;
	kal_uint32 id_r = 0;

	id_l = fan53528buc08x_get_id1();
	id_r = fan53528buc08x_get_id2();
	id = ((id_l << 8) | (id_r));

	g_fan53528buc08x_cid = id;

	/* fan53528buc08x_print("[update_fan53528_chip_id] id_l=0x%x, id_r=0x%x, id=0x%x\n", id_l, id_r, id); */

	return id;
}

void fan53528buc08x_hw_component_detect(void)
{
	int ret = 0;
	unsigned char PGOOD = 0;

	update_fan53528buc08x_chip_id();
	ret = fan53528buc08x_read_interface(0x05, &PGOOD, 0x1, 7);
	if (g_fan53528buc08x_cid == fan53528BUC08X_ID && PGOOD) {
		g_fan53528buc08x_hw_exist = 1;
	} else {
		g_fan53528buc08x_hw_exist = 0;
	}
	fan53528buc08x_print("[fan53528_hw_component_detect] exist = %d, Chip ID = %x\n", g_fan53528buc08x_hw_exist, g_fan53528buc08x_cid);
}

#if 0
int is_fan53528buc08x_exist(void)
{
	fan53528buc08x_print("g_fan53528_hw_exist=%d\n", g_fan53528_hw_exist);

	return g_fan53528buc08x_hw_exist;
}

int fan53528buc08x_vosel(unsigned long val)
{
	int ret = 1;
	unsigned int reg_val = 0;


	reg_val = (((val * 1000) - 603000) + 12825) / 12826;
	if (reg_val > 63)
		reg_val = 63;

	reg_val = reg_val | 0x80;
	ret = fan53528buc08x_write_byte(0x00, reg_val);

	pr_notice("[fan53528buc08x_vosel] val=%ld, reg_val=%ld\n", val, reg_val);

	return ret;	
}
#endif

int is_fan53528buc08x_exist(void)
{
	fan53528buc08x_print("g_fan53528_hw_exist=%d\n", g_fan53528buc08x_hw_exist);

	return g_fan53528buc08x_hw_exist;
}

int fan53528buc08x_enable(unsigned char en)
{
	int ret = 1;

	ret = fan53528buc08x_config_interface(FAN53528_BUCK_CTRL_REG, en, 0x1, 7);

	return ret;
}

int fan53528buc08x_is_enabled(void)
{
	int ret = 1;
	unsigned char en;

	ret = fan53528buc08x_read_interface(FAN53528_BUCK_CTRL_REG, &en, 0x1, 7);
	if (ret < 0)
		return -1;
	return en;
}

int fan53528buc08x_set_voltage(unsigned long val)
{
	int ret = 1;
	unsigned long reg_val = 0;

	/* 0.350 ~ 1.14375V (step = 0.6250)*/

	reg_val = (val - 350000)/6250;

	if (reg_val > 127)
		reg_val = 127;

	ret = fan53528buc08x_config_interface(FAN53528_BUCK_CTRL_REG, reg_val, 0x7F, 0);

	return ret;
}

unsigned long fan53528buc08x_get_voltage(void)
{
	int ret = 1;
	unsigned char reg_val = 0;

	ret = fan53528buc08x_read_interface(FAN53528_BUCK_CTRL_REG, &reg_val, 0x7F, 0);
	if (ret < 0)
		return 0;
	return (reg_val * 6250) + 350000;
}

int fan53528buc08x_set_mode(unsigned char mode)
{
	int ret;

	ret = fan53528buc08x_config_interface(0x02, mode, 0x3, 0);
	return ret;
}


void fan53528buc08x_driver_probe(void)
{
	fan53528buc08x_hw_component_detect();
	fan53528buc08x_print("[%s]\n", __func__);
	if (g_fan53528buc08x_hw_exist == 1) {
		fan53528buc08x_hw_init();
		fan53528buc08x_dump_register();
	} else {
		fan53528buc08x_print("[%s] PL fan53528 is not exist\n", __func__);
	}
	g_fan53528buc08x_driver_ready = 1;

	fan53528buc08x_print
	    ("[%s] PL g_fan53528_hw_exist=%d, g_fan53528buc08x_driver_ready=%d\n",
	     __func__, g_fan53528buc08x_hw_exist, g_fan53528buc08x_driver_ready);

	/* -------------------------------------------------------- */

#ifdef I2C_EXT_BUCK_CHANNEL
	fan53528buc08x_print("[%s] PL I2C_EXT_BUCK_CHANNEL=%d.\n",
		       __func__, I2C_EXT_BUCK_CHANNEL);
#else
	fan53528buc08x_print("[%s] PL No I2C_EXT_BUCK_CHANNEL (%d)\n",
		       __func__, fan53528buc08x_I2C_ID);
#endif

}
