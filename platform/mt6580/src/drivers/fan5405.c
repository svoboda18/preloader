#include "platform.h"
#include "i2c.h"
#include "fan5405.h"

int g_fan5405_log_en=0;

/**********************************************************
  *
  *   [I2C Slave Setting] 
  *
  *********************************************************/
#define fan5405_SLAVE_ADDR_WRITE   0xD4
#define fan5405_SLAVE_ADDR_Read    0xD5

/**********************************************************
  *
  *   [Global Variable] 
  *
  *********************************************************/
#define fan5405_REG_NUM 7  
kal_uint8 fan5405_reg[fan5405_REG_NUM] = {0};

#define FAN5405_I2C_ID	I2C1
static struct mt_i2c_t fan5405_i2c;

/**********************************************************
  *
  *   [I2C Function For Read/Write fan5405] 
  *
  *********************************************************/
kal_uint32 fan5405_write_byte(kal_uint8 addr, kal_uint8 value)
{
    kal_uint32 ret_code = I2C_OK;
    kal_uint8 write_data[2];
    kal_uint16 len;

    write_data[0]= addr;
    write_data[1] = value;

    fan5405_i2c.id = FAN5405_I2C_ID;
    /* Since i2c will left shift 1 bit, we need to set FAN5405 I2C address to >>1 */
    fan5405_i2c.addr = (fan5405_SLAVE_ADDR_WRITE >> 1);
    fan5405_i2c.mode = ST_MODE;
    fan5405_i2c.speed = 100;
    len = 2;

    ret_code = i2c_write(&fan5405_i2c, write_data, len);
    printf("%s: i2c_write: ret_code: %d\n", __func__, ret_code);

    return ret_code;
}

kal_uint32 fan5405_read_byte (kal_uint8 addr, kal_uint8 *dataBuffer) 
{
    kal_uint32 ret_code = I2C_OK;
    kal_uint16 len;
    *dataBuffer = addr;

    fan5405_i2c.id = FAN5405_I2C_ID;
    /* Since i2c will left shift 1 bit, we need to set FAN5405 I2C address to >>1 */
    fan5405_i2c.addr = (fan5405_SLAVE_ADDR_WRITE >> 1);
    fan5405_i2c.mode = ST_MODE;
    fan5405_i2c.speed = 100;
    len = 1;

    ret_code = i2c_write_read(&fan5405_i2c, dataBuffer, len, len);
    printf("%s: i2c_read: ret_code: %d\n", __func__, ret_code);

    return ret_code;
}

/**********************************************************
  *
  *   [Read / Write Function] 
  *
  *********************************************************/
kal_uint32 fan5405_read_interface (kal_uint8 RegNum, kal_uint8 *val, kal_uint8 MASK, kal_uint8 SHIFT)
{
    kal_uint8 fan5405_reg = 0;
    int ret = 0;
    
    printf("--------------------------------------------------PL\n");

    ret = fan5405_read_byte(RegNum, &fan5405_reg);
    printf("[fan5405_read_interface] Reg[%x]=0x%x\n", RegNum, fan5405_reg);
    
    fan5405_reg &= (MASK << SHIFT);
    *val = (fan5405_reg >> SHIFT);    
    printf("[fan5405_read_interface] val=0x%x\n", *val);

    return ret;
}

kal_uint32 fan5405_config_interface (kal_uint8 RegNum, kal_uint8 val, kal_uint8 MASK, kal_uint8 SHIFT)
{
    kal_uint8 fan5405_reg = 0;
    int ret = 0;

    printf("--------------------------------------------------PL\n");

    ret = fan5405_read_byte(RegNum, &fan5405_reg);
    printf("[fan5405_config_interface] Reg[%x]=0x%x\n", RegNum, fan5405_reg);
    
    fan5405_reg &= ~(MASK << SHIFT);
    fan5405_reg |= (val << SHIFT);

    ret = fan5405_write_byte(RegNum, fan5405_reg);
    printf("[fan5405_config_interface] write Reg[%x]=0x%x\n", RegNum, fan5405_reg);

    // Check
    //fan5405_read_byte(RegNum, &fan5405_reg);
    //printf("[fan5405_config_interface] Check Reg[%x]=0x%x\n", RegNum, fan5405_reg);

    return ret;
}

kal_uint32 fan5405_get_boost_status(void)
{
    kal_uint32 ret=0;
    kal_uint8 val=0;

    ret=fan5405_read_interface(     (kal_uint8)(fan5405_CON0), 
                                    (&val),
                                    (kal_uint8)(CON0_BOOST_MASK),
                                    (kal_uint8)(CON0_BOOST_SHIFT)
                                    );
    if(g_fan5405_log_en>1)        
        printf("%d\n", ret);
    
    return val;
}

void fan5405_set_opa_mode(kal_uint32 val)
{
    kal_uint32 ret=0;    

    ret=fan5405_config_interface(   (kal_uint8)(fan5405_CON1), 
                                    (kal_uint8)(val),
                                    (kal_uint8)(CON1_OPA_MODE_MASK),
                                    (kal_uint8)(CON1_OPA_MODE_SHIFT)
                                    );
    if(g_fan5405_log_en>1)        
        printf("%d\n", ret);
}

void fan5405_set_otg_en(kal_uint32 val)
{
    kal_uint32 ret=0;    

    ret=fan5405_config_interface(   (kal_uint8)(fan5405_CON2), 
                                    (kal_uint8)(val),
                                    (kal_uint8)(CON2_OTG_EN_MASK),
                                    (kal_uint8)(CON2_OTG_EN_SHIFT)
                                    );
    if(g_fan5405_log_en>1)        
        printf("%d\n", ret);
}

