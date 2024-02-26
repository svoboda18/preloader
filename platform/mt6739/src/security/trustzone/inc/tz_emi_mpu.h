#ifndef _EMI_MPU_H_
#define _EMI_MPU_H_

/* EMI memory protection align 64K */
#define EMI_MPU_ALIGNMENT   0x10000

#define NO_PROTECTION       0
#define SEC_RW              1
#define SEC_RW_NSEC_R       2
#define SEC_RW_NSEC_W       3
#define SEC_R_NSEC_R        4
#define FORBIDDEN           5
#define SEC_R_NSEC_RW       6

#define SECURE_OS_MPU_REGION_ID    0
#define ATF_MPU_REGION_ID          1

#define LOCK                1
#define UNLOCK              0
#define SET_ACCESS_PERMISSON(lock, d7, d6, d5, d4, d3, d2, d1, d0) \
((((unsigned int) d7) << 21) | (((unsigned int) d6) << 18) | (((unsigned int) d5) << 15) | \
 (((unsigned int) d4) << 12) | (((unsigned int) d3) <<  9) | (((unsigned int) d2) <<  6) | \
 (((unsigned int) d1) <<  3) | ((unsigned int) d0) | ((unsigned int) lock << 31))

extern int emi_mpu_set_region_protection(unsigned long long, unsigned long long, int, unsigned int);

#endif
