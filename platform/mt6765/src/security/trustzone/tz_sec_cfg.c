#include "tz_sec_reg.h"
#include <emi_mpu_mt.h>

#define MOD "[TZ_SEC_CFG]"

#define TEE_DEBUG
#ifdef TEE_DEBUG
#define DBG_MSG(str, ...) do {print(str, ##__VA_ARGS__);} while(0)
#else
#define DBG_MSG(str, ...) do {} while(0)
#endif

static void tz_set_field(volatile u32 *reg, u32 field, u32 val)
{
    u32 tv = (u32)*reg;
    tv &= ~(field);
    tv |= val;
    *reg = tv;
}

#define set_field(r,f,v)                tz_set_field((volatile u32*)r,f,v)
#define TZ_SET_FIELD(reg,field,val)     set_field(reg,field,val)

void tz_sram_enable_region_protection(int region, bool enable)
{
/*
0: disable Region n protection
1: Enable Region n protection and based on Region 1's security setting
*/
    if (region == 0) {
        if(!enable) /*Disable*/
            DRV_ClrReg32(SRAMROM_SEC_ADDR, (0x1<<SRAMROM_SEC_ADDR_SEC0_SEC_EN));
        else if(enable) /*Enable*/
            DRV_SetReg32(SRAMROM_SEC_ADDR, (0x1<<SRAMROM_SEC_ADDR_SEC0_SEC_EN));
    } else if (region == 1) {
        if(!enable) /*Disable*/
            DRV_ClrReg32(SRAMROM_SEC_ADDR, (0x1<<SRAMROM_SEC_ADDR_SEC1_SEC_EN));
        else if(enable) /*Enable*/
            DRV_SetReg32(SRAMROM_SEC_ADDR, (0x1<<SRAMROM_SEC_ADDR_SEC1_SEC_EN));
    } else if (region == 2) {
        if(!enable) /*Disable*/
            DRV_ClrReg32(SRAMROM_SEC_ADDR, (0x1<<SRAMROM_SEC_ADDR_SEC2_SEC_EN));
        else if(enable) /*Enable*/
            DRV_SetReg32(SRAMROM_SEC_ADDR, (0x1<<SRAMROM_SEC_ADDR_SEC2_SEC_EN));
    } else if (region == 3) {
        if(!enable) /*Disable*/
            DRV_ClrReg32(SRAMROM_SEC_ADDR, (0x1<<SRAMROM_SEC_ADDR_SEC3_SEC_EN));
        else if(enable) /*Enable*/
            DRV_SetReg32(SRAMROM_SEC_ADDR, (0x1<<SRAMROM_SEC_ADDR_SEC3_SEC_EN));
    }
}

void tz_sram_set_region_addr(unsigned int sram_split_register, unsigned int addr)
{
    if (sram_split_register == 0) {
        WRITE_REGISTER_UINT32(SRAMROM_SEC_ADDR, (addr & SRAMROM_SEC_ADDR_MASK));
    } else if (sram_split_register == 1) {
        WRITE_REGISTER_UINT32(SRAMROM_SEC_ADDR1, (addr & SRAMROM_SEC_ADDR_MASK));
    } else if (sram_split_register == 2) {
        WRITE_REGISTER_UINT32(SRAMROM_SEC_ADDR2, (addr & SRAMROM_SEC_ADDR_MASK));
    }
}

void tz_sram_sec_init(u32 start)
{
    DBG_MSG("%s [B]SRAMROM SEC_ADDR:0x%x, SEC_ADDR1:0x%x, SEC_ADDR2:0x%x\n",
        MOD, READ_REGISTER_UINT32(SRAMROM_SEC_ADDR), READ_REGISTER_UINT32(SRAMROM_SEC_ADDR1), READ_REGISTER_UINT32(SRAMROM_SEC_ADDR2));
    DBG_MSG("%s [B]SRAMROM SEC_CTRL:0x%x, SEC_CTRL2:0x%x, SEC_CTRL5:0x%x, SEC_CTRL6:0x%x\n",
        MOD, READ_REGISTER_UINT32(SRAMROM_SEC_CTRL), READ_REGISTER_UINT32(SRAMROM_SEC_CTRL2),
        READ_REGISTER_UINT32(SRAMROM_SEC_CTRL5), READ_REGISTER_UINT32(SRAMROM_SEC_CTRL6));

    /* Set Region Address Split Info */

    /* Split to 2 regions (region 0 and region 3) */
    tz_sram_set_region_addr(0, start);
    tz_sram_set_region_addr(1, start);
    tz_sram_set_region_addr(2, start);

    /* Enable region 0 and region 3 protection */
    tz_sram_enable_region_protection(0, TRUE);
    tz_sram_enable_region_protection(3, TRUE);

    /* Set permission for Region 0 */
    TZ_SET_FIELD(SRAMROM_SEC_CTRL, SRAMROM_SEC_CTRL_SEC0_DOM0_MASK, PERMIT_S_RW_NS_BLOCK    << SRAMROM_SEC_CTRL_SEC0_DOM0_SHIFT);
    TZ_SET_FIELD(SRAMROM_SEC_CTRL, SRAMROM_SEC_CTRL_SEC0_DOM1_MASK, PERMIT_S_BLOCK_NS_BLOCK << SRAMROM_SEC_CTRL_SEC0_DOM1_SHIFT);
    TZ_SET_FIELD(SRAMROM_SEC_CTRL, SRAMROM_SEC_CTRL_SEC0_DOM2_MASK, PERMIT_S_BLOCK_NS_BLOCK << SRAMROM_SEC_CTRL_SEC0_DOM2_SHIFT);
    TZ_SET_FIELD(SRAMROM_SEC_CTRL, SRAMROM_SEC_CTRL_SEC0_DOM3_MASK, PERMIT_S_BLOCK_NS_BLOCK << SRAMROM_SEC_CTRL_SEC0_DOM3_SHIFT);

    /* Set permission for Region 3 */
    TZ_SET_FIELD(SRAMROM_SEC_CTRL5, SRAMROM_SEC_CTRL5_SEC3_DOM0_MASK, PERMIT_S_RW_NS_RW       << SRAMROM_SEC_CTRL5_SEC3_DOM0_SHIFT);
    TZ_SET_FIELD(SRAMROM_SEC_CTRL5, SRAMROM_SEC_CTRL5_SEC3_DOM1_MASK, PERMIT_S_BLOCK_NS_BLOCK << SRAMROM_SEC_CTRL5_SEC3_DOM1_SHIFT);
    TZ_SET_FIELD(SRAMROM_SEC_CTRL5, SRAMROM_SEC_CTRL5_SEC3_DOM2_MASK, PERMIT_S_BLOCK_NS_BLOCK << SRAMROM_SEC_CTRL5_SEC3_DOM2_SHIFT);
    TZ_SET_FIELD(SRAMROM_SEC_CTRL5, SRAMROM_SEC_CTRL5_SEC3_DOM3_MASK, PERMIT_S_RW_NS_RW       << SRAMROM_SEC_CTRL5_SEC3_DOM3_SHIFT);

    DBG_MSG("%s [A]SRAMROM SEC_ADDR:0x%x, SEC_ADDR1:0x%x, SEC_ADDR2:0x%x\n",
        MOD, READ_REGISTER_UINT32(SRAMROM_SEC_ADDR), READ_REGISTER_UINT32(SRAMROM_SEC_ADDR1), READ_REGISTER_UINT32(SRAMROM_SEC_ADDR2));
    DBG_MSG("%s [A]SRAMROM SEC_CTRL:0x%x, SEC_CTRL2:0x%x, SEC_CTRL5:0x%x, SEC_CTRL6:0x%x\n",
        MOD, READ_REGISTER_UINT32(SRAMROM_SEC_CTRL), READ_REGISTER_UINT32(SRAMROM_SEC_CTRL2),
        READ_REGISTER_UINT32(SRAMROM_SEC_CTRL5), READ_REGISTER_UINT32(SRAMROM_SEC_CTRL6));
}

