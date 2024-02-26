#include "platform.h"
#include "i2c.h"
#include "gpio.h"
#include "da9214.h"

#if CFG_FPGA_PLATFORM
#else
#include "cust_i2c.h"
/* #include "cust_gpio_usage.h" */
#endif

/* #define DA9214_DEBUG */

/* DA9214UC04X */
/**********************************************************
  *   I2C Slave Setting
  *********************************************************/
#define DA9214_SLAVE_ADDR_WRITE   0xD0
#define DA9214_SLAVE_ADDR_READ    0xD1

/**********************************************************
  *   Global Variable
  *********************************************************/
#define DA9214_I2C_ID I2C6

#ifdef GPIO_EXT_BUCK_VSEL_PIN
unsigned int g_vproc_vsel_gpio_number = GPIO_EXT_BUCK_VSEL_PIN;
#else
unsigned int g_vproc_vsel_gpio_number = 0;
#endif

static struct mt_i2c_t da9214_i2c;

int g_da9214_driver_ready = 0;
int g_da9214_hw_exist = 0;

#define da9214_print(fmt, args...)   \
do {									\
    printf(fmt, ##args); \
} while (0)

kal_uint32 g_da9214_cid = 0;

/**********************************************************
  *
  *   [I2C Function For Read/Write da9214]
  *
  *********************************************************/
kal_uint32 da9214_write_byte(kal_uint8 addr, kal_uint8 value)
{
	int ret_code = I2C_OK;
	kal_uint8 write_data[2];
	kal_uint16 len;

	write_data[0] = addr;
	write_data[1] = value;

	da9214_i2c.id = DA9214_I2C_ID;
	/* Since i2c will left shift 1 bit, we need to set da9214 I2C address to >>1 */
	da9214_i2c.addr = (DA9214_SLAVE_ADDR_WRITE >> 1);
	da9214_i2c.mode = ST_MODE;
	da9214_i2c.speed = 100;
	da9214_i2c.pushpull = 1;
	len = 2;

	ret_code = i2c_write(&da9214_i2c, write_data, len);
	/* da9214_print("%s: i2c_write: ret_code: %d\n", __func__, ret_code); */

	if (ret_code == 0)
		return 1;	/* ok */
	else
		return 0;	/* fail */
}

kal_uint32 da9214_read_byte(kal_uint8 addr, kal_uint8 *dataBuffer)
{
	int ret_code = I2C_OK;
	kal_uint16 len;
	*dataBuffer = addr;

	da9214_i2c.id = DA9214_I2C_ID;
	/* Since i2c will left shift 1 bit, we need to set da9214 I2C address to >>1 */
	da9214_i2c.addr = (DA9214_SLAVE_ADDR_WRITE >> 1);
	da9214_i2c.mode = ST_MODE;
	da9214_i2c.speed = 100;
	da9214_i2c.pushpull = 1;
	len = 1;

	ret_code = i2c_write_read(&da9214_i2c, dataBuffer, len, len);
	/* da9214_print("%s: i2c_read: ret_code: %d\n", __func__, ret_code); */

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
kal_uint32 da9214_read_interface(kal_uint8 RegNum, kal_uint8 *val, kal_uint8 MASK,
				   kal_uint8 SHIFT)
{
	kal_uint8 da9214_reg = 0;
	kal_uint32 ret = 0;

	/* da9214_print("--------------------------------------------------PL\n"); */

	ret = da9214_read_byte(RegNum, &da9214_reg);
	/* da9214_print("[da9214_read_interface] Reg[%x]=0x%x\n", RegNum, da9214_reg); */

	da9214_reg &= (MASK << SHIFT);
	*val = (da9214_reg >> SHIFT);
	/* da9214_print("[da9214_read_interface] val=0x%x\n", *val); */

	return ret;
}

kal_uint32 da9214_config_interface(kal_uint8 RegNum, kal_uint8 val, kal_uint8 MASK,
				     kal_uint8 SHIFT)
{
	kal_uint8 da9214_reg = 0;
	kal_uint32 ret = 0;

	/* da9214_print("--------------------------------------------------PL\n"); */

	ret = da9214_read_byte(RegNum, &da9214_reg);
	/* da9214_print("[da9214_config_interface] Reg[%x]=0x%x\n", RegNum, da9214_reg); */

	da9214_reg &= ~(MASK << SHIFT);
	da9214_reg |= (val << SHIFT);

	ret = da9214_write_byte(RegNum, da9214_reg);
	/* da9214_print("[da9214_config_interface] write Reg[%x]=0x%x\n", RegNum, da9214_reg); */

	/* Check */
	/* da9214_read_byte(RegNum, &da9214_reg); */
	/* da9214_print("[da9214_config_interface] Check Reg[%x]=0x%x\n", RegNum, da9214_reg); */

	return ret;
}

kal_uint32 da9214_get_reg_value(kal_uint32 reg)
{
	kal_uint32 ret = 0;
	kal_uint8 reg_val = 0;

	ret = da9214_read_interface((kal_uint8) reg, &reg_val, 0xFF, 0x0);

	if (ret == 0)
		da9214_print("%d", ret);
	return reg_val;
}

#if DA9214_DEBUG
void da9214_dump_register(void)
{
	kal_uint8 i = 0;
	/* within 0x50 to 0x67, 0xD0 to DF, 0x140 to 0x14F and (read only) 0x200 to 0x27F. */
	/*
	   000: Selects Register 0x01 to 0x3F
	   001: Selects Register 0x81 to 0xCF
	   010: Selects Register 0x101 to 0x1CF
	 */
	/* ---------------------------------------------------------------- */
	da9214_print("[da9214] page 0,1: \n");
	da9214_print("[0x%x]=0x%x ", 0x0, da9214_get_reg_value(0x0));
	for (i = 0x50; i <= 0x5E; i++) {
		da9214_print("Dump: [0x%x]=0x%x \n", i, da9214_get_reg_value(i));
	}
	for (i = 0xD0; i <= 0xDA; i++) {
		da9214_config_interface(0x0, 0x1, 0xF, 0);	/* select to page 1 */
		da9214_print("Dump: [0x%x]=0x%x \n", i, da9214_get_reg_value(i));
	}
	da9214_print("\n");
	/* ---------------------------------------------------------------- */
	da9214_print("[da9214] page 2,3: \n");
	for (i = 0x01; i <= 0x06; i++) {
		da9214_config_interface(0x0, 0x2, 0xF, 0);	/* select to page 2,3 */
		da9214_print("Dump: [0x%x]=0x%x \n", i, da9214_get_reg_value(i));
	}
	for (i = 0x43; i <= 0x48; i++) {
		da9214_config_interface(0x0, 0x2, 0xF, 0);	/* select to page 2,3 */
		da9214_print("Dump: [0x%x]=0x%x \n", i, da9214_get_reg_value(i));
	}
	da9214_print("\n");
	/* ---------------------------------------------------------------- */
	da9214_config_interface(0x0, 0x0, 0xF, 0);	/* select to page 0,1 */
	/* ---------------------------------------------------------------- */
}
#endif

int get_da9214_i2c_ch_num(void)
{
	return DA9214_I2C_ID;
}

void da9214_hw_init(void)
{
	kal_uint32 ret = 0;

	da9214_print("da9214_hw_init\n");
	//ret = da9214_config_interface(0x0, 0x1, 0x1, 7);	/* page reverts to 0 after one access */
	ret = da9214_config_interface(0x5D, 0x1, 0x1, 0);	/* BUCKA_EN = 1b'1 */
	ret = da9214_config_interface(0x5E, 0x1, 0x1, 0);	/* BUCKB_EN = 1b'1 */
	/* add by UE, set Vproc = 880mv */
	/* ret = da9214_config_interface(0xD9, 0xBA, 0xFF, 0); */
	ret = da9214_config_interface(0x0, 0x2, 0xF, 0);	/* select to page 2,3 */
	ret = da9214_config_interface(0x44, 0x1, 0x1, 0);	/* RELOAD_FUNC_EN = 1b'1 */
	/* Set DA9214 VBUCKA_B = 0.6V auto mode */
	ret = da9214_config_interface(0x0, 0x0, 0xF, 0);	/* select to page 0,1 */
	ret = da9214_config_interface(0xD8, 0x9E, 0x7F, 0);

}

void da9214_hw_component_detect(void)
{
	kal_uint32 ret = 0;
	kal_uint8 val = 0;

	//ret = da9214_config_interface(0x0, 0x1, 0x1, 7);	/* page reverts to 0 after one access */
	ret = da9214_config_interface(0x0, 0x2, 0xF, 0);	/* select to page 2,3 */

	ret = da9214_read_interface(0x5, &val, 0xF, 4);	/* Read IF_BASE_ADDR1 */

	/* check default SPEC. value */
	if (val == 0xD) {
		g_da9214_hw_exist = 1;
	} else {
		g_da9214_hw_exist = 0;
	}
	#if DA9214_DEBUG
	da9214_dump_register();
	#endif
	da9214_print("[da9214_hw_component_detect] exist=%d, Reg[0x105][7:4]=0x%x\n",
		  g_da9214_hw_exist, val);
	da9214_print("Enable continuous high speed mode Start\n");
	ret = da9214_config_interface(0x0, 0x2, 0xF, 0);	/* select to page 2,3 */
	ret = da9214_config_interface(0x6, 0x1, 0x1, 6);	/* Enables continuous high speed mode */
	/* i2c_hs_enable = 1; */
	da9214_print("Enable continuous high speed mode End\n");
}

int is_da9214_exist(void)
{
	da9214_print("g_da9214_hw_exist=%d\n", g_da9214_hw_exist);

	return g_da9214_hw_exist;
}

int da9214_vosel_buck_a(unsigned long val)
{
	int ret = 1;
	unsigned long reg_val = 0;

	/* reg_val = ( (val) - 30000 ) / 1000; //300mV~1570mV, step=10mV */
	reg_val = ((((val * 10) - 300000) / 1000) + 9) / 10;

	if (reg_val > 127)
		reg_val = 127;

	ret = da9214_write_byte(0xD7, reg_val);

	da9214_print("[da9214_vosel_buck_a] val=%d, reg_val=%d, Reg[0xD7]=0x%x\n",
		  val, reg_val, da9214_get_reg_value(0xD7));

	return ret;
}

int da9214_vosel_buck_b(unsigned long val)
{
	int ret = 1;
	unsigned long reg_val = 0;

	/* reg_val = ( (val) - 30000 ) / 1000; //300mV~1570mV, step=10mV */
	reg_val = ((((val * 10) - 300000) / 1000) + 9) / 10;

	if (reg_val > 127)
		reg_val = 127;

	ret = da9214_write_byte(0xD9, reg_val);

	da9214_print("[da9214_vosel_buck_b] val=%d, reg_val=%d, Reg[0xD9]=0x%x\n",
		  val, reg_val, da9214_get_reg_value(0xD9));

	return ret;
}

void da9214_driver_probe(void)
{
	da9214_hw_component_detect();
	da9214_print("[da9214_driver_probe]\n");
	if (g_da9214_hw_exist == 1) {
		da9214_hw_init();
		#if DA9214_DEBUG
		da9214_dump_register();
		#endif
	} else {
		da9214_print("[da9214_driver_probe] PL da9214 is not exist\n");
	}
	g_da9214_driver_ready = 1;

	da9214_print
	    ("[da9214_driver_probe] PL g_da9214_hw_exist=%d, g_da9214_driver_ready=%d\n",
	     g_da9214_hw_exist, g_da9214_driver_ready);

	/* -------------------------------------------------------- */

#ifdef I2C_EXT_BUCK_CHANNEL
	da9214_print("[da9214_driver_probe] PL I2C_EXT_BUCK_CHANNEL=%d.\n",
		       I2C_EXT_BUCK_CHANNEL);
#else
	da9214_print("[da9214_driver_probe] PL No I2C_EXT_BUCK_CHANNEL (%d)\n",
		       DA9214_I2C_ID);
#endif

#ifdef GPIO_EXT_BUCK_VSEL_PIN
	da9214_print("[da9214_driver_probe] PL GPIO_EXT_BUCK_VSEL_PIN=0x%x.\n",
		       GPIO_EXT_BUCK_VSEL_PIN);
#else
	da9214_print("[da9214_driver_probe] PL No GPIO_EXT_BUCK_VSEL_PIN (0x%x)\n",
		       g_vproc_vsel_gpio_number);
#endif
}
