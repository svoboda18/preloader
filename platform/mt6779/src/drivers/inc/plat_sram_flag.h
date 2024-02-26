#ifndef __PLAT_SRAM_FLAG_H__
#define __PLAT_SRAM_FLAG_H__

#define PLAT_SRAM_FLAG
#define PLAT_SRAM_FLGA_MAGIC_PATTERN 0x5566520

typedef struct {
	unsigned int decs_magic;
	unsigned long plat_sram_flag0;
	unsigned long plat_sram_flag1;
	unsigned long plat_sram_flag2;
} PLAT_SRAM_FLAG_T;
#define DEF_PLAT_SRAM_FLAG PLAT_SRAM_FLAG_T

#endif
