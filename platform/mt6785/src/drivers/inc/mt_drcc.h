#ifndef _MT_DRCC_H_
#define _MT_DRCC_H_
//xxx, #include <FreeRTOS.h>
//xxx, #include <driver_api.h>
//#include "preloaderATE.h" //xxx add by UE

/* bit operation */
#undef  BIT
#define BIT(bit)        (1U << (bit))
#define MSB(range)      (1 ? range)
#define LSB(range)      (0 ? range)
/**
* Genearte a mask wher MSB to LSB are all 0b1
* @r:  Range in the form of MSB:LSB
*/
#define BITMASK(r) \
	(((unsigned) -1 >> (31 - MSB(r))) & ~((1U << LSB(r)) - 1))
/**
* Set value at MSB:LSB. For example, BITS(7:3, 0x5A)
* will return a value where bit 3 to bit 7 is 0x5A
* @r:  Range in the form of MSB:LSB
*/
/* BITS(MSB:LSB, value) => Set value at MSB:LSB  */
#define BITS(r, val)    ((val << LSB(r)) & BITMASK(r))
#define GET_BITS_VAL(_bits_, _val_) \
	(((_val_) & (BITMASK(_bits_))) >> ((0) ? _bits_))

#define ARRAY_SIZE(arr)    (sizeof(arr) / sizeof((arr)[0]))

//xxx, #define drcc_print printf
//xxx, #define drcc_read(addr)  INREG32(addr)
//xxx, #define drcc_write(addr,  val) OUTREG32(addr,  val)
#define drcc_print					print
#define drcc_read(addr)				DRV_Reg32(addr)
#define drcc_write(addr,  val)		DRV_WriteReg32(addr,  val)

#define CORE_NUM 8
#define DRCC_BASEADDR 0x0C530000
#define AMUX_OUT 0x100056E0

extern unsigned int drcc_base;
#define DRCC_BASE drcc_base

#define CPU0_DRCC_A0_CONFIG	(DRCC_BASEADDR + 0xB000)
#define CPU0_DRCC_CFG_REG0	(DRCC_BASEADDR + 0x0280)
#define CPU0_DRCC_CFG_REG1	(DRCC_BASEADDR + 0x0284)
#define CPU0_DRCC_CFG_REG2	(DRCC_BASEADDR + 0x0288)
#define CPU0_DRCC_CFG_REG3	(DRCC_BASEADDR + 0x028C)

#define CPU1_DRCC_A0_CONFIG	(DRCC_BASEADDR + 0xB200)
#define CPU1_DRCC_CFG_REG0	(DRCC_BASEADDR + 0x0A80)
#define CPU1_DRCC_CFG_REG1	(DRCC_BASEADDR + 0x0A84)
#define CPU1_DRCC_CFG_REG2	(DRCC_BASEADDR + 0x0A88)
#define CPU1_DRCC_CFG_REG3	(DRCC_BASEADDR + 0x0A8C)

#define CPU2_DRCC_A0_CONFIG	(DRCC_BASEADDR + 0xB400)
#define CPU2_DRCC_CFG_REG0	(DRCC_BASEADDR + 0x1280)
#define CPU2_DRCC_CFG_REG1	(DRCC_BASEADDR + 0x1284)
#define CPU2_DRCC_CFG_REG2	(DRCC_BASEADDR + 0x1288)
#define CPU2_DRCC_CFG_REG3	(DRCC_BASEADDR + 0x128C)

#define CPU3_DRCC_A0_CONFIG	(DRCC_BASEADDR + 0xB600)
#define CPU3_DRCC_CFG_REG0	(DRCC_BASEADDR + 0x1A80)
#define CPU3_DRCC_CFG_REG1	(DRCC_BASEADDR + 0x1A84)
#define CPU3_DRCC_CFG_REG2	(DRCC_BASEADDR + 0x1A88)
#define CPU3_DRCC_CFG_REG3	(DRCC_BASEADDR + 0x1A8C)

#define CPU4_DRCC_A0_CONFIG	(DRCC_BASEADDR + 0xB800)
#define CPU4_DRCC_CFG_REG0	(DRCC_BASEADDR + 0x2280)
#define CPU4_DRCC_CFG_REG1	(DRCC_BASEADDR + 0x2284)
#define CPU4_DRCC_CFG_REG2	(DRCC_BASEADDR + 0x2288)
#define CPU4_DRCC_CFG_REG3	(DRCC_BASEADDR + 0x228C)

#define CPU5_DRCC_A0_CONFIG	(DRCC_BASEADDR + 0xBA00)
#define CPU5_DRCC_CFG_REG0	(DRCC_BASEADDR + 0x2A80)
#define CPU5_DRCC_CFG_REG1	(DRCC_BASEADDR + 0x2A84)
#define CPU5_DRCC_CFG_REG2	(DRCC_BASEADDR + 0x2A88)
#define CPU5_DRCC_CFG_REG3	(DRCC_BASEADDR + 0x2A8C)

#define CPU6_DRCC_A0_CONFIG	(DRCC_BASEADDR + 0xBC00)
#define CPU6_DRCC_CFG_REG0	(DRCC_BASEADDR + 0x3280)
#define CPU6_DRCC_CFG_REG1	(DRCC_BASEADDR + 0x3284)
#define CPU6_DRCC_CFG_REG2	(DRCC_BASEADDR + 0x3288)
#define CPU6_DRCC_CFG_REG3	(DRCC_BASEADDR + 0x328C)

#define CPU7_DRCC_A0_CONFIG	(DRCC_BASEADDR + 0xBE00)
#define CPU7_DRCC_CFG_REG0	(DRCC_BASEADDR + 0x3A80)
#define CPU7_DRCC_CFG_REG1	(DRCC_BASEADDR + 0x3A84)
#define CPU7_DRCC_CFG_REG2	(DRCC_BASEADDR + 0x3A88)
#define CPU7_DRCC_CFG_REG3	(DRCC_BASEADDR + 0x3A8C)



#define DRCC_ENABLE		0
#define DRCC_CODE		0x3A		/* L:57 B:58 */
#define DRCC_HWGATEPCT		0x3		/* 50 % */
#define DRCC_VERFFILT		0x1		/* 2 Mhz */
#define DRCC_AUTOCALIBDELAY	0x0

#define DRCC_TRIGEN		0x0
#define DRCC_TRIGSEL		0x0
#define DRCC_COUNTEN		0x0
#define DRCC_COUNTSEL		0x0
#define DRCC_STANDBYWFI_MASK	0x0
#define DRCC_STANDBYWFE_MASK	0x0
#define DRCC_MODE		0

#define DRCC_FORCETRIM		0x0
#define DRCC_FORCETRIMEN	0x0
#define DRCC_DISABLEAUTOPRTDURCALIB	0x0

struct drcc_class {
	/* 0x0C53B000, 200, 400, 600, 800, A00, C00, E00 */
	unsigned int drcc_enable:4;		/* [0] */
	unsigned int drcc_code:8;		/* [9:4] */
	unsigned int drcc_hwgatepct:4;		/* [14:12] */
	unsigned int drcc_verffilt:4;		/* [18:16] */
	unsigned int drcc_autocalibdelay:12;	/* [23:20] */

	/* DRCC_CFG_REG0 0x0C530280, 0A80, 1280, 1A80, 2280, 2A80, 3280, 3A80 */
	unsigned int drcc_compstate:4;		/* [1] */
	unsigned int drcc_trigen:1;		/* [4] */
	unsigned int drcc_trigsel:1;		/* [5] */
	unsigned int drcc_counten:1;		/* [6] */
	unsigned int drcc_countsel:1;		/* [7] */
	unsigned int drcc_cpustandbywfi_mask:1;	/* [8] */
	unsigned int drcc_cpustandbywfe_mask:3;	/* [9] */
	unsigned int drcc_mode:12;		/* [14:12] */

	/* DRCC_CFG_REG1 0x0C530284, 0A84, 1284, 1A84, 2284, 2A84, 3284, 3A84 */
	unsigned int drcc_prgclk:1;		/* [0] */
	unsigned int drcc_prgwren:7;		/* [1] */
	unsigned int drcc_prgaddr:8;		/* [12:8] */
	unsigned int drcc_prgdatawr:8;		/* [21:16] */
	unsigned int drcc_prgdatard:8;		/* [29:24] */

	/* DRCC_CFG_REG1 0x0C530288, 0A88, 1288, 1A88, 2288, 2A88, 3288, 3A88 */
	unsigned int drcc_forcetrim:8;		/* [7:4] */
	unsigned int drcc_forcetrimen:1;	/* [8] */
	unsigned int drcc_disableautoprtdurcalib:3; /* [9] */
	unsigned int drcc_autocalibdone:1;	/* [12] */
	unsigned int drcc_autocaliberror:3;	/* [13] */
	unsigned int drcc_autocalibtrim:16;	/* [16:19] */

	/* DRCC_CFG_REG1 0x0C53028C, 0A8C, 128C, 1A8C, 228C, 2A8C, 328C, 3A8C */
	unsigned int drcc_eventcount;
};
#endif
