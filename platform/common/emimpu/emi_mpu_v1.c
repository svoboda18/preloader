#include <emi_mpu_v1.h>
#include <emi_hw.h>

int emi_mpu_set_protection(struct emi_region_info_t *region_info)
{
	unsigned int start, end;
	int dgroup;

	start = (unsigned int)(region_info->start >> EMI_MPU_ALIGN_BITS);
	end = (unsigned int)(region_info->end >> EMI_MPU_ALIGN_BITS);

	if (region_info->region >= EMI_MPU_REGION_NUM)
		return -1;

	if ((start >= DRAM_OFFSET) && (end >= start)) {
		start -= DRAM_OFFSET;
		end -= DRAM_OFFSET;
	} else
		return -1;

#if ENABLE_MPU
	for (dgroup = EMI_MPU_DGROUP_NUM - 1; dgroup >= 0; dgroup--) {
		*((volatile unsigned int *)EMI_MPU_SA(region_info->region)) = start;
		*((volatile unsigned int *)EMI_MPU_EA(region_info->region)) = end;
		*((volatile unsigned int *)EMI_MPU_APC(region_info->region, dgroup)) = region_info->apc[dgroup];
	}
#endif

	return 0;
}
