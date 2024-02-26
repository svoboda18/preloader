/**
* @file    mt_clk_buf_ctl.c
* @brief   Driver for RF clock buffer control
*
*/

#define __MT_CLK_BUF_CTL_C__

/*=============================================================*/
// Include files
/*=============================================================*/
#include <platform.h>
#include <spm.h>
#include <spm_mtcmos.h>
#include <spm_mtcmos_internal.h>
#include <timer.h>  //udelay
#include <clkbuf_ctl.h>
#include <pmic.h>
#include "typedefs.h"

#define PMIC_REG_MASK				0xFFFF
#define PMIC_REG_SHIFT				0

#define PMIC_CW00_ADDR				0x7000

#define PMIC_CW15_ADDR				0x701E
#define PMIC_CW15_DCXO_STATIC_AUXOUT_EN_MASK	0x1
#define PMIC_CW15_DCXO_STATIC_AUXOUT_EN_SHIFT	0
#define PMIC_CW15_DCXO_STATIC_AUXOUT_SEL_MASK	0xF
#define PMIC_CW15_DCXO_STATIC_AUXOUT_SEL_SHIFT	1

static unsigned short clkbuf_initiated;

static CLK_BUF_SWCTRL_STATUS_T  clk_buf_swctrl[CLKBUF_NUM]={
	CLK_BUF_SW_ENABLE,
	CLK_BUF_SW_ENABLE,
	CLK_BUF_SW_ENABLE,
	CLK_BUF_SW_ENABLE
};

bool is_clk_buf_from_pmic(void)
{
	unsigned int reg = 0;

#if 1
#ifdef CLKBUF_COVCTCXO_MODE
		return false;
#else
		return true;
#endif
#else
	/* switch to debug mode */
	pmic_config_interface(PMIC_CW15_ADDR, 0x1,
			      PMIC_CW15_DCXO_STATIC_AUXOUT_EN_MASK,
			      PMIC_CW15_DCXO_STATIC_AUXOUT_EN_SHIFT);
	pmic_config_interface(PMIC_CW15_ADDR, 0x3,
			      PMIC_CW15_DCXO_STATIC_AUXOUT_SEL_MASK,
			      PMIC_CW15_DCXO_STATIC_AUXOUT_SEL_SHIFT);
	/* bit 6, 7, 8, 9 => 32K Less Mode, Buffer Mode, RTC Mode, Off Mode */
	pmic_read_interface(PMIC_CW00_ADDR, &reg,
			    PMIC_REG_MASK, PMIC_REG_SHIFT);
	/* switch back from debug mode */
	pmic_config_interface(PMIC_CW15_ADDR, 0x0,
			      PMIC_CW15_DCXO_STATIC_AUXOUT_EN_MASK,
			      PMIC_CW15_DCXO_STATIC_AUXOUT_EN_SHIFT);
	if ((reg & 0x200) == 0x200) {
		print("clkbuf is from RF, PMIC_CW00_ADDR=0x%x\n", reg);
		return false;
	} else {
		print("clkbuf is from PMIC, PMIC_CW00_ADDR=0x%x\n", reg);
		return true;
	}
#endif
};

/*
 * BSI_TOP init setting:
 * RFIC BSITOP_PORT_PARAM:	HW_WRITE(0x10221004, 0x8000A824)
 * RFIC BSITOP_IC0_PARAM1:	HW_WRITE(0x10221010, 0x20001201)
 * RFIC BSITOP_IC0_PARAM2:	HW_WRITE(0x10221014, 0x150B0000)
 * RFIC BSITOP_IC1_PARAM1:	HW_WRITE(0x10221020, 0xE001841)
 * RFIC BSITOP_IC1_PARAM2:	HW_WRITE(0x10221024, 0x150B0000)
 * RFIC BSITOP_LOGGER_PARAM:	HW_WRITE(0x10221030, 0x1)
 */
void bsi_top_init(void)
{
	if (clkbuf_initiated)
		return;

	DRV_WriteReg32(0x10221004, 0x8000A824);
	DRV_WriteReg32(0x10221010, 0x20001201);
	DRV_WriteReg32(0x10221014, 0x150B0000);
	DRV_WriteReg32(0x10221020, 0xE001841);
	DRV_WriteReg32(0x10221024, 0x150B0000);
	DRV_WriteReg32(0x10221030, 0x1);

	clkbuf_initiated = 1;
}

/*
 * Config Clock buffer 4 enable
 * BSIMM_IMM_CONF:		HW_WRITE(0x1021F004, 0x0)
 * BSIMM_IMM_WDATA_1:		HW_WRITE(0x1021F008, 0x1859557)
 * BSIMM_IMM_CONF:		HW_WRITE(0x1021F004, 0x1)
 */
void clk_buf_enable_clkbuf24(void)
{
	bsi_top_init();

	DRV_WriteReg32(0x1021F004, 0x0);
	DRV_WriteReg32(0x1021F008, 0x1859557); /* old:0x1867775 */
	DRV_WriteReg32(0x1021F004, 0x1);
	print("clk_buf_enable_clkbuf24: 0x10221100 = 0x%x\n", DRV_Reg32(0x10221100));
}

void clk_buf_disable_clkbuf24(void)
{
	bsi_top_init();

	DRV_WriteReg32(0x1021F004, 0x0);
	DRV_WriteReg32(0x1021F008, 0x1869556); /* old: 0x1866775 */
	DRV_WriteReg32(0x1021F004, 0x1);
	printf("clk_buf_disable_clkbuf24: 0x10221100 = 0x%x\n", DRV_Reg32(0x10221100));
}

void clk_buf_dump_log(void)
{
	unsigned int start_addr = 0, i = 0, idx = 0;
	unsigned int bsilogger_cw[20], bsilogger_ctrl[20];
	unsigned int cw[20], auxout_cw[20], auxout_dcxo_cw[12];

	if (!is_clk_buf_from_pmic()) {
		/* RF clkbuf: VCTCXO */

		/* Debug code */
		start_addr = (DRV_Reg32(0x10221030) >> 16) & 0x7F;

		/* Debug code ??BSI command for 20 cw*/
		for(i=1; i<=20; i++) {
			idx = (start_addr+128-i)&0x7F;
			bsilogger_cw[20-i] = DRV_Reg32(0x10221100 + (idx * 8));
			bsilogger_ctrl[20-i] = DRV_Reg32(0x10221104 + (idx * 8));
			printf("clkbuf: bsilogger_cw[%d]=0x%x\n", 20-i, bsilogger_cw[20-i]);
			printf("clkbuf: bsilogger_ctrl[%d]=0x%x\n", 20-i, bsilogger_ctrl[20-i]);
		}

		// Sheet ??wire default??? Set CW6[10]=0x0 and CW27[14] =0x0
		// Read CW24~CW43.
		DRV_WriteReg32( 0x1021F004, 0x0 );
		DRV_WriteReg32( 0x1021F008, 0x600384 );
		DRV_WriteReg32( 0x1021F004, 0x1 );
		DRV_WriteReg32( 0x1021F004, 0x0 );
		DRV_WriteReg32( 0x1021F008, 0x1B00001 );
		DRV_WriteReg32( 0x1021F004, 0x1 );
		/* Readback DCXO cw register check - cw24~43 */
		for(i = 24; i < 44; i++) {
			DRV_WriteReg32(0x1021F004, 0x2);
			DRV_WriteReg32(0x1021F008, (0x400 + i));
			DRV_WriteReg32(0x1021F004, 0x3);
			udelay(1);
			cw[i - 24] = DRV_Reg32(0x1021F010);
			DRV_WriteReg32(0x1021F620, 0x1);
			printf("clkbuf: cw%d=0x%x\n", i, cw[i - 24]);
		}

		// Sheet ??wire Auxout??? Set CW6[10]=0x1 and CW27[14] =0x1
		// Read CW24~CW43
		DRV_WriteReg32( 0x1021F004, 0x0);
		DRV_WriteReg32( 0x1021F008, 0x600784 );
		DRV_WriteReg32( 0x1021F004, 0x1 );
		DRV_WriteReg32( 0x1021F004, 0x0 );
		DRV_WriteReg32( 0x1021F008, 0x1B04001 );
		DRV_WriteReg32( 0x1021F004, 0x1 );
		for( i=24; i<44; i++) {
			DRV_WriteReg32( 0x1021F004, 0x2 );
			DRV_WriteReg32( 0x1021F008, (0x400+i) );
			DRV_WriteReg32( 0x1021F004, 0x3 );
			udelay(1);
			auxout_cw[i-24] = DRV_Reg32( 0x1021F010 );
			DRV_WriteReg32( 0x1021F620, 0x1 );
			printf("clkbuf: auxout_cw%d=0x%x\n", i, auxout_cw[i - 24]);
		}

		// Sheet ??wire Auxout-dcxo??? Set CW6[10]=0x1 and CW27[14] =0x0
		// Read CW24~CW35.
		DRV_WriteReg32( 0x1021F004, 0x0 );
		DRV_WriteReg32( 0x1021F008, 0x600784 );
		DRV_WriteReg32( 0x1021F004, 0x1 );
		DRV_WriteReg32( 0x1021F004, 0x0 );
		DRV_WriteReg32( 0x1021F008, 0x1B00001 );
		DRV_WriteReg32( 0x1021F004, 0x1 );
		for( i=24; i<36; i++) {
			DRV_WriteReg32( 0x1021F004, 0x2 );
			DRV_WriteReg32( 0x1021F008, (0x400+i) );
			DRV_WriteReg32( 0x1021F004, 0x3 );
			udelay(1);
			auxout_dcxo_cw[i-24] = DRV_Reg32( 0x1021F010 );
			DRV_WriteReg32( 0x1021F620, 0x1 );
			printf("clkbuf: auxout_dcxo_cw%d=0x%x\n", i, auxout_dcxo_cw[i - 24]);
		}

	}
}

/* enable clkbuf4 for RTC calibration on MT6755 MP1/2 */
void clk_buf_all_on(void)
{
	if (!is_clk_buf_from_pmic()) {
		/* RF clkbuf: VCTCXO */
		print("before clk_buf_enable_clkbuf24: 0x10221100 = 0x%x\n",
		      DRV_Reg32(0x10221100));

		clk_buf_enable_clkbuf24();

		/* clk_buf_dump_log(); */
	}
}

