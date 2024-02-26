#include "i2c.h"
#include "flashlight.h"

/**********************************************************
  *   I2C Slave Setting
  *********************************************************/
#define FLASHLIGHT_SLAVE_ADDR	0x63 
#define FLASHLIGHT_I2C_ID	I2C1
/**********************************************************
  *   Global Variable
  *********************************************************/
static struct mt_i2c_t flashlight_i2c;
/**********************************************************
  *
  *   [I2C Function For Read/Write flashlight]
  *
  *********************************************************/
kal_uint32 flashlight_write_byte(kal_uint8 addr, kal_uint8 value)
{
    int ret_code = I2C_OK;
    kal_uint8 write_data[2];
    kal_uint16 len;

    write_data[0]= addr;
    write_data[1] = value;

    flashlight_i2c.id = FLASHLIGHT_I2C_ID;
    flashlight_i2c.addr = FLASHLIGHT_SLAVE_ADDR;
    flashlight_i2c.mode = ST_MODE;
    flashlight_i2c.speed = MAX_ST_MODE_SPEED;
    flashlight_i2c.pushpull = 1;
    len = 2;

    ret_code = i2c_write(&flashlight_i2c, write_data, len);

    if(ret_code == 0)
        return 1; // ok
    else
        return 0; // fail
}

kal_uint32 flashlight_read_byte(kal_uint8 addr, kal_uint8 *dataBuffer)
{
    int ret_code = I2C_OK;
    kal_uint16 len;
    *dataBuffer = addr;

    flashlight_i2c.id = FLASHLIGHT_I2C_ID;
    flashlight_i2c.addr = FLASHLIGHT_SLAVE_ADDR;
    flashlight_i2c.mode = ST_MODE;
    flashlight_i2c.speed = MAX_ST_MODE_SPEED;
    flashlight_i2c.pushpull = 1;
    len = 1;

    ret_code = i2c_write_read(&flashlight_i2c, dataBuffer, len, len);

    if(ret_code == 0)
        return 1; // ok
    else
        return 0; // fail
}

int flashlight_init(void)
{
    kal_uint8 read_reg = 0;
    kal_uint32 ret = 0;
    ret = flashlight_read_byte(0x00, &read_reg);
    ret = flashlight_write_byte(0x00, (0x80 | read_reg));
    return ret;
}
