#include "typedefs.h"
#include "tz_init.h"
#include "emi_reg.h"
#include "emi_drv.h"

#define MOD "[TZ_EMI_MPU]"

void tz_emi_mpu_init(u32 start_add, u32 end_addr, u32 mpu_region)
{
    int ret = 0;
    unsigned int sec_mem_mpu_attr;
    unsigned int sec_mem_phy_start, sec_mem_phy_end;

    /* Caculate start/end address */
    sec_mem_phy_start = start_add;
    sec_mem_phy_end = end_addr;

    // For MT6580
    //====================================================================
    //            | Region |  D0(AP)  |  D1(MD0)  |  D2(Conn) |  D3(MM)
    //------------+-------------------------------------------------------
    // Secure OS  |    0   |RW(S)     |Forbidden  |Forbidden  |RW(S)
    //------------+-------------------------------------------------------
    //====================================================================

    sec_mem_mpu_attr = SET_ACCESS_PERMISSON(LOCK, SEC_RW, FORBIDDEN, FORBIDDEN, SEC_RW);

    //print("%s MPU [0x%x-0x%x]\n", MOD, sec_mem_phy_start, sec_mem_phy_end);

    ret = emi_mpu_set_region_protection(sec_mem_phy_start,  /*START_ADDR*/
                                        sec_mem_phy_end,    /*END_ADDR*/
                                        mpu_region,         /*region*/
                                        sec_mem_mpu_attr);
}
