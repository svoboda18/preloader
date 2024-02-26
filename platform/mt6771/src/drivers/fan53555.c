#include "platform.h"
#include "i2c.h"
#include "gpio.h"
#include "fan53555.h"

#if CFG_FPGA_PLATFORM
#else
#include "cust_i2c.h"
/* #include "cust_gpio_usage.h" */
#endif

/* FAN53555UC04X */
/**********************************************************
  *   I2C Slave Setting
  *********************************************************/
#define FAN53555_SLAVE_ADDR_WRITE   0xC0
#define FAN53555_SLAVE_ADDR_READ    0xC1

/**********************************************************
  *   Global Variable
  *********************************************************/
/* #ifdef I2C_EXT_BUCK_CHANNEL */
/* #define fan53555_I2C_ID I2C_EXT_BUCK_CHANNEL */
/* #else */
#define fan53555_I2C_ID I2C7
/* #endif */

#ifdef GPIO_EXT_BUCK_VSEL_PIN
unsigned int g_vgpu_vsel_gpio_number = GPIO_EXT_BUCK_VSEL_PIN;
#else
unsigned int g_vgpu_vsel_gpio_number = 0;
#endif

static struct mt_i2c_t fan53555_i2c;

int g_fan53555_driver_ready = 0;
int g_fan53555_hw_exist = 0;

#define fan53555_print(fmt, args...)   \
do {									\
    printf(fmt, ##args); \
} while (0)

kal_uint32 g_fan53555_cid = 0;

/**********************************************************
  *
  *   [I2C Function For Read/Write fan53555]
  *
  *********************************************************/
kal_uint32 fan53555_write_byte(kal_uint8 addr, kal_uint8 value)
{
	int ret_code = I2C_OK;
	kal_uint8 write_data[2];
	kal_uint16 len;

	write_data[0] = addr;
	write_data[1] = value;

	fan53555_i2c.id = fan53555_I2C_ID;
	/* Since i2c will left shift 1 bit, we need to set fan53555 I2C address to >>1 */
	fan53555_i2c.addr = (FAN53555_SLAVE_ADDR_WRITE >> 1);
	fan53555_i2c.mode = ST_MODE;
	fan53555_i2c.speed = 100;
	fan53555_i2c.pushpull = 1;
	len = 2;

	ret_code = i2c_write(&fan53555_i2c, write_data, len);
	/* fan53555_print("%s: i2c_write: ret_code: %d\n", __func__, ret_code); */

	if (ret_code == 0)
		return 1;	/* ok */
	else
		return 0;	/* fail */
}

kal_uint32 fan53555_read_byte(kal_uint8 addr, kal_uint8 *dataBuffer)
{
	int ret_code = I2C_OK;
	kal_uint16 len;
	*dataBuffer = addr;

	fan53555_i2c.id = fan53555_I2C_ID;
	/* Since i2c will left shift 1 bit, we need to set fan53555 I2C address to >>1 */
	fan53555_i2c.addr = (FAN53555_SLAVE_ADDR_WRITE >> 1);
	fan53555_i2c.mode = ST_MODE;
	fan53555_i2c.speed = 100;
	fan53555_i2c.pushpull = 1;
	len = 1;

	ret_code = i2c_write_read(&fan53555_i2c, dataBuffer, len, len);
	/* fan53555_print("%s: i2c_read: ret_code: %d\n", __func__, ret_code); */

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
kal_uint32 fan53555_read_interface(kal_uint8 RegNum, kal_uint8 *val, kal_uint8 MASK,
				   kal_uint8 SHIFT)
{
	kal_uint8 fan53555_reg = 0;
	kal_uint32 ret = 0;

	/* fan53555_print("--------------------------------------------------PL\n"); */

	ret = fan53555_read_byte(RegNum, &fan53555_reg);
	/* fan53555_print("[fan53555_read_interface] Reg[%x]=0x%x\n", RegNum, fan53555_reg); */

	fan53555_reg &= (MASK << SHIFT);
	*val = (fan53555_reg >> SHIFT);
	/* fan53555_print("[fan53555_read_interface] val=0x%x\n", *val); */

	return ret;
}

kal_uint32 fan53555_config_interface(kal_uint8 RegNum, kal_uint8 val, kal_uint8 MASK,
				     kal_uint8 SHIFT)
{
	kal_uint8 fan53555_reg = 0;
	kal_uint32 ret = 0;

	/* fan53555_print("--------------------------------------------------PL\n"); */

	ret = fan53555_read_byte(RegNum, &fan53555_reg);
	/* fan53555_print("[fan53555_config_interface] Reg[%x]=0x%x\n", RegNum, fan53555_reg); */

	fan53555_reg &= ~(MASK << SHIFT);
	fan53555_reg |= (val << SHIFT);

	ret = fan53555_write_byte(RegNum, fan53555_reg);
	/* fan53555_print("[fan53555_config_interface] write Reg[%x]=0x%x\n", RegNum, fan53555_reg); */

	/* Check */
	/* fan53555_read_byte(RegNum, &fan53555_reg); */
	/* fan53555_print("[fan53555_config_interface] Check Reg[%x]=0x%x\n", RegNum, fan53555_reg); */

	return ret;
}

kal_uint32 fan53555_get_reg_value(kal_uint32 reg)
{
	kal_uint32 ret = 0;
	kal_uint8 reg_val = 0;

	ret = fan53555_read_interface((kal_uint8) reg, &reg_val, 0xFF, 0x0);

	if (ret == 0)
		fan53555_print("%d", ret);
	return reg_val;
}

void fan53555_dump_register(void)
{
#if 1
	kal_uint8 i = 0x0;
	kal_uint8 i_max = 0x5;	/* 0xD5 */

	for (i = 0x0; i <= i_max; i++) {
		fan53555_print("[0x%x]=0x%x ", i, fan53555_get_reg_value(i));
	}
#endif
}

int get_fan53555_i2c_ch_num(void)
{
	return fan53555_I2C_ID;
}

void fan53555_hw_init(void)
{
	fan53555_print("fan53555_hw_init\n");
	/* Disable FAN53555 */
	fan53555_config_interface(FAN53555_VSEL0, 0x0, 0x1, 7);
	/* Disable FAN53555 */
	fan53555_config_interface(FAN53555_VSEL1, 0x0, 0x1, 7);
}

kal_uint8 fan53555_get_id1(void)
{
	kal_uint8 val;

	fan53555_read_interface(FAN53555_ID1, &val, 0xff, 0);
	return val;
}

kal_uint8 fan53555_get_id2(void)
{
	kal_uint8 val;
	fan53555_read_interface(FAN53555_ID2, &val, 0xff, 0);
	return val;
}

kal_uint32 update_fan53555_chip_id(void)
{
	kal_uint32 id = 0;
	kal_uint32 id_l = 0;
	kal_uint32 id_r = 0;

	id_l = fan53555_get_id1();
	id_r = fan53555_get_id2();
	id = ((id_l << 8) | (id_r));

	g_fan53555_cid = id;

	/* fan53555_print("[update_fan53555_chip_id] id_l=0x%x, id_r=0x%x, id=0x%x\n", id_l, id_r, id); */

	return id;
}

void fan53555_hw_component_detect(void)
{
	update_fan53555_chip_id();

	if (g_fan53555_cid == FAN53555UC04X_ID) {
		g_fan53555_hw_exist = 1;
	} else {
		g_fan53555_hw_exist = 1;
	}
	fan53555_print("[fan53555_hw_component_detect] exist = %d, Chip ID = %x\n", g_fan53555_hw_exist, g_fan53555_cid);
}

#if 0
int is_fan53555_exist(void)
{
	fan53555_print("g_fan53555_hw_exist=%d\n", g_fan53555_hw_exist);

	return g_fan53555_hw_exist;
}

int fan53555_vosel(unsigned long val)
{
	int ret = 1;
	unsigned int reg_val = 0;

	/* 0.603~1.411V (step 12.826mv)*/
	reg_val = (((val * 1000) - 603000) + 12825) / 12826;

	if (reg_val > 63)
		reg_val = 63;

	reg_val = reg_val | 0x80;
	ret = fan53555_write_byte(0x00, reg_val);

	pr_notice("[fan53555_vosel] val=%ld, reg_val=%ld\n", val, reg_val);

	return ret;	
}
#endif

void fan53555_driver_probe(void)
{
	fan53555_hw_component_detect();
	fan53555_print("[fan53555_driver_probe]\n");
	if (g_fan53555_hw_exist == 1) {
		fan53555_hw_init();
		fan53555_dump_register();
	} else {
		fan53555_print("[fan53555_driver_probe] PL fan53555 is not exist\n");
	}
	g_fan53555_driver_ready = 1;

	fan53555_print
	    ("[fan53555_driver_probe] PL g_fan53555_hw_exist=%d, g_fan53555_driver_ready=%d\n",
	     g_fan53555_hw_exist, g_fan53555_driver_ready);

	/* -------------------------------------------------------- */

#ifdef I2C_EXT_BUCK_CHANNEL
	fan53555_print("[fan53555_driver_probe] PL I2C_EXT_BUCK_CHANNEL=%d.\n",
		       I2C_EXT_BUCK_CHANNEL);
#else
	fan53555_print("[fan53555_driver_probe] PL No I2C_EXT_BUCK_CHANNEL (%d)\n",
		       fan53555_I2C_ID);
#endif

#ifdef GPIO_EXT_BUCK_VSEL_PIN
	fan53555_print("[fan53555_driver_probe] PL GPIO_EXT_BUCK_VSEL_PIN=0x%x.\n",
		       GPIO_EXT_BUCK_VSEL_PIN);
#else
	fan53555_print("[fan53555_driver_probe] PL No GPIO_EXT_BUCK_VSEL_PIN (0x%x)\n",
		       g_vgpu_vsel_gpio_number);
#endif
}
