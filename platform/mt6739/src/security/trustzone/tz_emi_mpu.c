#include "typedefs.h"
#include "tz_init.h"
#include "tz_emi_reg.h"
#include "tz_emi_mpu.h"

#define MOD "[TZ_EMI_MPU]"

#define readl(addr) (__raw_readl(addr))
#define writel(b,addr) __raw_writel(b,addr)
#define IOMEM(reg) (reg)

/*
 * emi_mpu_set_region_protection: protect a region.
 * @start: start address of the region
 * @end: end address of the region
 * @region: EMI MPU region id
 * @access_permission: EMI MPU access permission
 * Return 0 for success, otherwise negative status code.
 */
int emi_mpu_set_region_protection(unsigned long long start, unsigned long long end, int region, unsigned int access_permission)
{
    int ret = 0;

    if((end != 0) || (start !=0))
    {
    	/* DRAM space is from 0x40000000 */
	start -= 0x40000000;
	end -= 0x40000000;

        /*Address 64KB alignment*/
        start = (start >> 16) & 0x1FFFF;
        end = (end >> 16) & 0x1FFFF;

        if (end < start)
        {
            return -1;
        }
    }

    writel(start, EMI_MPU_SA(region));
    writel(end, EMI_MPU_EA(region));
    writel(access_permission, EMI_MPU_APC(region, 0));

    return ret;
}

void tz_emi_mpu_init(u32 start_add, u32 end_addr, u32 mpu_region)
{
    int ret = 0;
    unsigned long long sec_mem_mpu_attr;
    unsigned long long sec_mem_phy_start, sec_mem_phy_end;

    /* Caculate start/end address */
    sec_mem_phy_start = (unsigned long long) start_add;
    sec_mem_phy_end = (unsigned long long) end_addr;

    switch(mpu_region)
    {
        case SECURE_OS_MPU_REGION_ID:
            sec_mem_mpu_attr = SET_ACCESS_PERMISSON(LOCK, FORBIDDEN, FORBIDDEN, FORBIDDEN, SEC_RW, FORBIDDEN, FORBIDDEN, FORBIDDEN, SEC_RW);
            break;

        case ATF_MPU_REGION_ID:
            sec_mem_mpu_attr = SET_ACCESS_PERMISSON(LOCK, FORBIDDEN, FORBIDDEN, FORBIDDEN, FORBIDDEN, FORBIDDEN, FORBIDDEN, FORBIDDEN, SEC_RW);
            break;

        default:
            print("%s Warning - MPU region '%d' is not supported in pre-loader!\n", MOD, mpu_region);
            return;
    }

    print("%s MPU [0x%llx-0x%llx]\n", MOD, sec_mem_phy_start, sec_mem_phy_end);

    ret = emi_mpu_set_region_protection(sec_mem_phy_start, sec_mem_phy_end, mpu_region, sec_mem_mpu_attr);

    if(ret) {
        print("%s MPU error!!\n", MOD);
    }
}

