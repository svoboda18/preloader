#ifndef __EMI_MPU_MT_H__
#define __EMI_MPU_MT_H__

#define ENABLE_MPU	1

#define EMI_MPU_ALIGN_BITS	16
#define EMI_MPU_DOMAIN_NUM	16
#define EMI_MPU_REGION_NUM	32
#define DRAM_OFFSET		(0x40000000 >> EMI_MPU_ALIGN_BITS)

#define SECURE_OS_MPU_REGION_ID	0
#define ATF_MPU_REGION_ID	1
#define SSPM_MPU_REGION_ID	3

#include <emi_mpu_v1.h>

#endif /* __EMI_MPU_MT_H__ */
