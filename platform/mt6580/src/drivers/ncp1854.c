#include "platform.h"
#include "i2c.h"
#include "pmic.h"
#include "ncp1854.h"

/**********************************************************
  *
  *   [I2C Slave Setting]
  *
  *********************************************************/
#define NCP1854_SLAVE_ADDR_WRITE 0x6C
#define NCP1854_SLAVE_ADDR_READ 0x6D

/**********************************************************
  *
  *   [Global Variable]
  *
  *********************************************************/
#define NCP1854_REG_NUM 18

#ifdef I2C_SWITCHING_CHARGER_CHANNEL
#define NCP1854_BUSNUM I2C_SWITCHING_CHARGER_CHANNEL
#else
#define NCP1854_BUSNUM 3
#endif

#define PRECC_BATVOL 2800 /*preCC 2.8V*/

/* #ifndef GPIO_SWCHARGER_EN_PIN
#define GPIO_SWCHARGER_EN_PIN 65
#endif  */
kal_uint8 ncp1854_reg[NCP1854_REG_NUM] = {0};


/**********************************************************
  *
  *   [I2C Function For Read/Write ncp1854]
  *
  *********************************************************/
static struct mt_i2c_t ncp1854_i2c;

kal_uint32 ncp1854_write_byte(kal_uint8 addr, kal_uint8 value)
{
	kal_uint32 ret_code = I2C_OK;
	kal_uint8 write_data[2];
	kal_uint16 len;

	write_data[0]= addr;
	write_data[1] = value;

	ncp1854_i2c.id = NCP1854_BUSNUM;
	/* Since i2c will left shift 1 bit, we need to set NCP1854 I2C address to >>1 */
	ncp1854_i2c.addr = (NCP1854_SLAVE_ADDR_WRITE >> 1);
	ncp1854_i2c.mode = ST_MODE;
	ncp1854_i2c.speed = 100;
	len = 2;

	ret_code = i2c_write(&ncp1854_i2c, write_data, len);

	if(I2C_OK != ret_code)
	print("%s: i2c_write: ret_code: %d\n", __func__, ret_code);

	return ret_code;
}

kal_uint32 ncp1854_read_byte (kal_uint8 addr, kal_uint8 *dataBuffer)
{
	kal_uint32 ret_code = I2C_OK;
	kal_uint16 len;
	*dataBuffer = addr;

	ncp1854_i2c.id = NCP1854_BUSNUM;
	/* Since i2c will left shift 1 bit, we need to set NCP1854 I2C address to >>1 */
	ncp1854_i2c.addr = (NCP1854_SLAVE_ADDR_READ >> 1);
	ncp1854_i2c.mode = ST_MODE;
	ncp1854_i2c.speed = 100;
	len = 1;

	ret_code = i2c_write_read(&ncp1854_i2c, dataBuffer, len, len);

	if(I2C_OK != ret_code)
	print("%s: i2c_read: ret_code: %d\n", __func__, ret_code);

	return ret_code;
}


/**********************************************************
  *
  *   [Read / Write Function]
  *
  *********************************************************/
kal_uint32 ncp1854_read_interface (kal_uint8 RegNum, kal_uint8 *val, kal_uint8 MASK, kal_uint8 SHIFT)
{
	kal_uint8 ncp1854_reg = 0;
	int ret = 0;

	print("--------------------------------------------------\n");

	ret = ncp1854_read_byte(RegNum, &ncp1854_reg);
	print("[ncp1854_read_interface] Reg[%x]=0x%x\n", RegNum, ncp1854_reg);

	ncp1854_reg &= (MASK << SHIFT);
	*val = (ncp1854_reg >> SHIFT);
	print("[ncp1854_read_interface] Val=0x%x\n", *val);

	return ret;

}

kal_uint32 ncp1854_config_interface (kal_uint8 RegNum, kal_uint8 val, kal_uint8 MASK, kal_uint8 SHIFT)
{
	kal_uint8 ncp1854_reg = 0;
	int ret = 0;

	print("--------------------------------------------------\n");

	ret = ncp1854_read_byte(RegNum, &ncp1854_reg);
	/*print("[ncp1854_config_interface] Reg[%x]=0x%x\n", RegNum, ncp1854_reg);*/

	ncp1854_reg &= ~(MASK << SHIFT);
	ncp1854_reg |= (val << SHIFT);
	
	if(RegNum == NCP1854_CON1 && val == 1 && MASK ==CON1_REG_RST_MASK && SHIFT == CON1_REG_RST_SHIFT)
	{
		/*RESET bit*/
	}
	else if(RegNum == NCP1854_CON1)
	{
		ncp1854_reg &= ~0x80;	/*RESET bit read returs 1, so clear it*/
	}

	ret = ncp1854_write_byte(RegNum, ncp1854_reg);
	/*print("[ncp18546_config_interface] Write Reg[%x]=0x%x\n", RegNum, ncp1854_reg);*/

	/* Check */
	/*ncp1854_read_byte(RegNum, &ncp1854_reg);*/
	/*print("[ncp1854_config_interface] Check Reg[%x]=0x%x\n", RegNum, ncp1854_reg);*/

	return ret;

}

/**********************************************************
  *
  *   [ncp1854 Function]
  *
  *********************************************************/
void ncp1854_set_otg_en(kal_uint32 val)
{
    kal_uint32 ret=0;

    ret=ncp1854_config_interface((kal_uint8)(NCP1854_CON1),
                                                   (kal_uint8)(val),
                                                   (kal_uint8)(CON1_OTG_EN_MASK),
                                                   (kal_uint8)(CON1_OTG_EN_SHIFT)
                                                   );
    return val;
}

kal_uint32 ncp1854_get_otg_en(void)
{
    kal_uint32 ret=0;
    kal_uint8 val=0;

    ret=ncp1854_read_interface((kal_uint8)(NCP1854_CON1),
	        					  (&val),
							      (kal_uint8)(CON1_OTG_EN_MASK),
							      (kal_uint8)(CON1_OTG_EN_SHIFT)
							      );
    return val;
}

/**********************************************************
  *
  *   [Internal Function]
  *
  *********************************************************/
void ncp1854_check_otg_status(void)
{
	kal_uint32 is_otg_boost_mode=0;
	is_otg_boost_mode = ncp1854_get_otg_en();
	if(is_otg_boost_mode==1)
	{
		ncp1854_set_otg_en(0);
		mdelay(100); /*wait for vbus drop down*/
	}
}

