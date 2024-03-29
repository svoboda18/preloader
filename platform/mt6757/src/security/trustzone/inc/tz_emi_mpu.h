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

#define SECURE_OS_MPU_REGION_ID    0
#define ATF_MPU_REGION_ID          1

#define LOCK                1
#define UNLOCK              0
#define SET_ACCESS_PERMISSON(lock, d7, d6, d5, d4, d3, d2, d1, d0) (((d7) << 21) | ((d6) << 18) | ((d5) << 15) | ((d4) << 12) | ((d3) << 9) | ((d2) << 6) | ((d1) << 3) | (d0) | (lock << 31))

#endif
