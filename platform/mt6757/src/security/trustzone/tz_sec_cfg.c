#include "tz_sec_reg.h"

#define MOD "[TZ_SEC_CFG]"

#define TEE_DEBUG
#ifdef TEE_DEBUG
#define DBG_MSG(str, ...) do {print(str, ##__VA_ARGS__);} while(0)
#else
#define DBG_MSG(str, ...) do {} while(0)
#endif

extern void tz_emi_mpu_init(u32 start, u32 end, u32 mpu_region);

void tz_sec_mem_init(u32 start, u32 end, u32 mpu_region)
{    
    tz_emi_mpu_init(start, end, mpu_region);
}

static void tz_set_field(volatile u32 *reg, u32 field, u32 val)
{
    u32 tv = (u32)*reg;
    tv &= ~(field);
    tv |= val;
    *reg = tv;
}

#define set_field(r,f,v)                tz_set_field((volatile u32*)r,f,v)
#define TZ_SET_FIELD(reg,field,val)     set_field(reg,field,val)

void tz_sram_sec_init(u32 start)
{
    /* Set Region Address Info */
    WRITE_REGISTER_UINT32(SRAMROM_SEC_ADDR, (start & SRAMROM_SEC_ADDR_MASK));

    DBG_MSG("%s SRAMROM Secure Addr 0x%x\n", MOD, READ_REGISTER_UINT32(SRAMROM_SEC_ADDR));
    DBG_MSG("%s SRAMROM Secure Control 0x%x\n", MOD, READ_REGISTER_UINT32(BOOTROM_PWR_CTRL));

    /* Set permission for Region 0 */
    TZ_SET_FIELD(BOOTROM_PWR_CTRL, SRAMROM_SEC_SEC0_DOM0_MASK, PERMIT_S_RW_NS_BLOCK << SRAMROM_SEC_SEC0_DOM0_SHIFT);
    TZ_SET_FIELD(BOOTROM_PWR_CTRL, SRAMROM_SEC_SEC0_DOM1_MASK, PERMIT_S_BLOCK_NS_BLOCK << SRAMROM_SEC_SEC0_DOM1_SHIFT);
    TZ_SET_FIELD(BOOTROM_PWR_CTRL, SRAMROM_SEC_SEC0_DOM2_MASK, PERMIT_S_BLOCK_NS_BLOCK << SRAMROM_SEC_SEC0_DOM2_SHIFT);
    TZ_SET_FIELD(BOOTROM_PWR_CTRL, SRAMROM_SEC_SEC0_DOM3_MASK, PERMIT_S_BLOCK_NS_BLOCK << SRAMROM_SEC_SEC0_DOM3_SHIFT);
    
    DBG_MSG("%s SRAMROM Secure Control 0x%x\n", MOD, READ_REGISTER_UINT32(BOOTROM_PWR_CTRL));
    
    /* Set permission for Region 1 */
    TZ_SET_FIELD(BOOTROM_PWR_CTRL, SRAMROM_SEC_SEC1_DOM0_MASK, PERMIT_S_RW_NS_RW << SRAMROM_SEC_SEC1_DOM0_SHIFT);
    TZ_SET_FIELD(BOOTROM_PWR_CTRL, SRAMROM_SEC_SEC1_DOM1_MASK, PERMIT_S_BLOCK_NS_BLOCK << SRAMROM_SEC_SEC1_DOM1_SHIFT);
    TZ_SET_FIELD(BOOTROM_PWR_CTRL, SRAMROM_SEC_SEC1_DOM2_MASK, PERMIT_S_BLOCK_NS_BLOCK << SRAMROM_SEC_SEC1_DOM2_SHIFT);
    TZ_SET_FIELD(BOOTROM_PWR_CTRL, SRAMROM_SEC_SEC1_DOM3_MASK, PERMIT_S_BLOCK_NS_BLOCK << SRAMROM_SEC_SEC1_DOM3_SHIFT);
    TZ_SET_FIELD(BOOTROM_PWR_CTRL, SRAMROM_SEC_SEC1_EN_MASK, ENABLE_SEC_SEC1_PROTECTION << SRAMROM_SEC_SEC1_EN_SHIFT);
    
    DBG_MSG("%s SRAMROM Secure Control 0x%x\n", MOD, READ_REGISTER_UINT32(BOOTROM_PWR_CTRL));
}

