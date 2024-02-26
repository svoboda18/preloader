#ifndef _EMI_DRV_H_
#define _EMI_DRV_H_

/* EMI memory protection align 64K */
#define EMI_MPU_ALIGNMENT   0x10000
#define EMI_PHY_OFFSET      0x80000000
#define SEC_PHY_SIZE        0x06000000

#define NO_PROTECTION       0
#define SEC_RW              1
#define SEC_RW_NSEC_R       2
#define SEC_RW_NSEC_W       3
#define SEC_R_NSEC_R        4
#define FORBIDDEN           5

#define SECURE_OS_MPU_REGION_ID    0
#define AP_MPU_REGION_ID           7

#define LOCK                1
#define UNLOCK              0
#define SET_ACCESS_PERMISSON(lock, d3, d2, d1, d0) ((((d3) << 9) | ((d2) << 6) | ((d1) << 3) | (d0)) | (lock << 15))


extern int emi_mpu_set_region_protection(unsigned int start, unsigned int end, int region, unsigned int access_permission);
extern int emi_mpu_init(void);

#endif
