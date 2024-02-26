#include "typedefs.h"
#include "dram_buffer.h"
#include "platform.h"
#include "emi_reg.h"
#include "emi_drv.h"

/*============================================================================*/
/* CONSTAND DEFINITIONS                                                       */
/*============================================================================*/
#define MOD "[EMI_DRV]"

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
int emi_mpu_set_region_protection(unsigned int start, unsigned int end, int region, unsigned int access_permission)
{
	int ret = 0;
	unsigned int tmp, tmp2;
	unsigned int ax_pm, ax_pm2;
	print("%smpu protect region %d 0x%x to 0x %x",
	      MOD, region, start, end);

	if((end != 0) || (start !=0))
	{
		/*Address 64KB alignment*/
		start -= EMI_PHY_OFFSET;
		end -= EMI_PHY_OFFSET;
		start = start >> 16;
		end = end >> 16;

		if (end <= start)
		{
			ret = -1;
			goto out;
		}
	}

	ax_pm  = (access_permission << 16) >> 16;
	ax_pm2 = (access_permission >> 16);

	switch (region) {
	case 0:
		tmp = readl(IOMEM(EMI_MPUI)) & 0xFFFF0000;
		writel(0, EMI_MPUI);
		writel((start << 16) | end, EMI_MPUA);
		writel(tmp | ax_pm, EMI_MPUI);

		//print("EMI_MPUA(0x%x)=0x%x\n",EMI_MPUA, readl(IOMEM(EMI_MPUA)));
		//print("EMI_MPUI(0x%x)=0x%x\n",EMI_MPUI, readl(IOMEM(EMI_MPUI)));
		break;

	case 1:
		tmp = readl(IOMEM(EMI_MPUI)) & 0x0000FFFF;
		writel(0, EMI_MPUI);
		writel((start << 16) | end, EMI_MPUB);
		writel(tmp | (ax_pm << 16), EMI_MPUI);
		break;

	case 2:
		tmp = readl(IOMEM(EMI_MPUJ)) & 0xFFFF0000;
		writel(0, EMI_MPUJ);
		writel((start << 16) | end, EMI_MPUC);
		writel(tmp | ax_pm, EMI_MPUJ);
		break;

	case 3:
		tmp = readl(IOMEM(EMI_MPUJ)) & 0x0000FFFF;
		writel(0, EMI_MPUJ);
		writel((start << 16) | end, EMI_MPUD);
		writel(tmp | (ax_pm << 16), EMI_MPUJ);
		break;

	case 4:
		tmp = readl(IOMEM(EMI_MPUK)) & 0xFFFF0000;
		writel(0, EMI_MPUK);
		writel((start << 16) | end, EMI_MPUE);
		writel(tmp | ax_pm, EMI_MPUK);
		break;

	case 5:
		tmp = readl(IOMEM(EMI_MPUK)) & 0x0000FFFF;
		writel(0, EMI_MPUK);
		writel((start << 16) | end, EMI_MPUF);
		writel(tmp | (ax_pm << 16), EMI_MPUK);
		break;

	case 6:
		tmp = readl(IOMEM(EMI_MPUL)) & 0xFFFF0000;
		writel(0, EMI_MPUL);
		writel((start << 16) | end, EMI_MPUG);
		writel(tmp | ax_pm, EMI_MPUL);
		break;

	case 7:
		tmp = readl(IOMEM(EMI_MPUL)) & 0x0000FFFF;
		writel(0, EMI_MPUL);
		writel((start << 16) | end, EMI_MPUH);
		writel(tmp | (ax_pm << 16), EMI_MPUL);
		break;
	default:
		ret = -1;
		break;
	}

out:


	print("[%s]\n", ret ? "FAIL" : "OK");

	return ret;
}


int emi_mpu_init(void)
{
	mblock_info_t *mblock_info = &g_dram_buf->bootarg.mblock_info;
	unsigned int start, end;
	unsigned int permission = SET_ACCESS_PERMISSON(UNLOCK, NO_PROTECTION, FORBIDDEN, FORBIDDEN, NO_PROTECTION);
	int i;

	start = mblock_info->mblock[0].start;
	end = start;
	for(i = 0; i < mblock_info->mblock_num; i++) {
		end += mblock_info->mblock[i].size;
		//print("block[%d] size = 0x%llx\n", i, mblock_info->mblock[i].size);
	}

	/*clear violation */
	writel((0x1 << 31) , EMI_MPUS);
	return emi_mpu_set_region_protection(start, end, AP_MPU_REGION_ID, permission);
}


