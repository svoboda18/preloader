/* Copyright Statement:
*
* This software/firmware and related documentation ("MediaTek Software") are
* protected under relevant copyright laws. The information contained herein
* is confidential and proprietary to MediaTek Inc. and/or its licensors.
* Without the prior written permission of MediaTek inc. and/or its licensors,
* any reproduction, modification, use or disclosure of MediaTek Software,
* and information contained herein, in whole or in part, shall be strictly prohibited.
*/
/* MediaTek Inc. (C) 2016. All rights reserved.
*
* BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
* THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
* RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
* AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
* NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
* SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
* SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
* THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
* THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
* CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
* SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
* STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
* CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
* AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
* OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
* MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*/

/*
 * @file    clk_buf_ctl.c
 * @brief   Driver for clock buffer control
 */

#define __CLK_BUF_CTL_C__

/*
 * Include files
 */
#include <platform.h>
#include <timer.h>
#include <typedefs.h>
#include <pmic.h>
#include <upmu_hw.h>

#include <clkbuf_ctl.h>

#define clk_buf_warn(fmt, args...)		print(fmt, ##args)

#define clkbuf_readl(addr)			DRV_Reg32(addr)
#define clkbuf_writel(addr, val)		DRV_WriteReg32(addr, val)

#define late_initcall(a)

#define PWRAP_REG(ofs)		(PWRAP_BASE + ofs)
/* PMICWRAP Reg */
#define DCXO_ENABLE		PWRAP_REG(0x190)
#define DCXO_CONN_ADR0		PWRAP_REG(0x194)
#define DCXO_CONN_WDATA0	PWRAP_REG(0x198)
#define DCXO_CONN_ADR1		PWRAP_REG(0x19C)
#define DCXO_CONN_WDATA1	PWRAP_REG(0x1A0)
#define DCXO_NFC_ADR0		PWRAP_REG(0x1A4)
#define DCXO_NFC_WDATA0		PWRAP_REG(0x1A8)
#define DCXO_NFC_ADR1		PWRAP_REG(0x1AC)
#define DCXO_NFC_WDATA1		PWRAP_REG(0x1B0)

#define	DCXO_CONN_ENABLE	(0x1 << 1)
#define	DCXO_NFC_ENABLE		(0x1 << 0)

#define PMIC_REG_MASK				0xFFFF
#define PMIC_REG_SHIFT				0

#define PMIC_CW00_INIT_VAL			0x4E1D
#define PMIC_CW11_INIT_VAL			0x8000
#define PMIC_CW15_INIT_VAL			0xA2AA
#define XO_RESERVED4_INIT_VAL			0x3 /* suggest: [7]=1'b1,[6]=1'b1 */

/* TODO: marked this after driver is ready */
/* #define CLKBUF_BRINGUP */

/* #define CLKBUF_CONN_SUPPORT_CTRL_FROM_I1 */

static u8 drv_curr_vals[XO_NUMBER] = {CLK_BUF_DRIVING_CURR_2,
	CLK_BUF_DRIVING_CURR_2, CLK_BUF_DRIVING_CURR_2,
	CLK_BUF_DRIVING_CURR_2, CLK_BUF_DRIVING_CURR_2,
	CLK_BUF_DRIVING_CURR_2, CLK_BUF_DRIVING_CURR_2};

static int clk_buf_fs_init(void)
{
	return 0;
}

static int clk_buf_dts_map(void)
{
	return 0;
}

bool is_clk_buf_from_pmic(void)
{
	return true;
}

static void clk_buf_dump_dts_log(void)
{
#ifndef CLKBUF_BRINGUP
	clk_buf_warn("%s: PMIC_CLK_BUF?_STATUS=%d %d %d %d %d %d %d\n", __func__,
		     CLK_BUF1_STATUS_PMIC, CLK_BUF2_STATUS_PMIC,
		     CLK_BUF3_STATUS_PMIC, CLK_BUF4_STATUS_PMIC,
		     CLK_BUF5_STATUS_PMIC, CLK_BUF6_STATUS_PMIC,
		     CLK_BUF7_STATUS_PMIC);
	clk_buf_warn("%s: PMIC_CLK_BUF?_DRV_CURR=%d %d %d %d %d %d %d\n", __func__,
		     PMIC_CLK_BUF1_DRIVING_CURR,
		     PMIC_CLK_BUF2_DRIVING_CURR,
		     PMIC_CLK_BUF3_DRIVING_CURR,
		     PMIC_CLK_BUF4_DRIVING_CURR,
		     PMIC_CLK_BUF5_DRIVING_CURR,
		     PMIC_CLK_BUF6_DRIVING_CURR,
		     PMIC_CLK_BUF7_DRIVING_CURR);
#endif
}

static void clk_buf_dump_clkbuf_log(void)
{
#ifndef CLKBUF_BRINGUP
	u32 pmic_cw00 = 0, pmic_cw02 = 0, pmic_cw11 = 0,
	    pmic_cw13 = 0, pmic_cw14 = 0, pmic_cw15 = 0,
	    pmic_cw16 = 0, pmic_cw20 = 0, top_spi_con1 = 0;

	pmic_read_interface(PMIC_XO_EXTBUF1_MODE_ADDR, &pmic_cw00,
			    PMIC_REG_MASK, PMIC_REG_SHIFT);
	pmic_read_interface(PMIC_XO_BUFLDOK_EN_ADDR, &pmic_cw02,
			    PMIC_REG_MASK, PMIC_REG_SHIFT);
	pmic_read_interface(PMIC_XO_EXTBUF6_MODE_ADDR, &pmic_cw11,
			    PMIC_REG_MASK, PMIC_REG_SHIFT);
	pmic_read_interface(PMIC_RG_XO_RESERVED4_ADDR, &pmic_cw13,
			    PMIC_REG_MASK, PMIC_REG_SHIFT);
	pmic_read_interface(PMIC_XO_EXTBUF2_CLKSEL_MAN_ADDR, &pmic_cw14,
			    PMIC_REG_MASK, PMIC_REG_SHIFT);
	pmic_read_interface(PMIC_RG_XO_EXTBUF1_HD_ADDR, &pmic_cw15,
			    PMIC_REG_MASK, PMIC_REG_SHIFT);
	pmic_read_interface(PMIC_XO_EXTBUF1_ISET_M_ADDR, &pmic_cw16,
			    PMIC_REG_MASK, PMIC_REG_SHIFT);
	pmic_read_interface(PMIC_RG_SRCLKEN_IN3_EN_ADDR, &top_spi_con1,
			    PMIC_REG_MASK, PMIC_REG_SHIFT);
	clk_buf_warn("%s DCXO_CW00/02/11/13/14/15/16/top_spi_con1=0x%x %x %x %x %x %x %x %x\n",
		     __func__, pmic_cw00, pmic_cw02, pmic_cw11, pmic_cw13,
		     pmic_cw14, pmic_cw15, pmic_cw16, top_spi_con1);
#endif
}

static void clk_buf_ctrl_vcn18(u8 onoff)
{
#ifndef CLKBUF_BRINGUP
	if (onoff) {
		pmic_config_interface(PMIC_RG_LDO_VCN18_EN_ADDR, 1,
				      PMIC_RG_LDO_VCN18_EN_MASK,
				      PMIC_RG_LDO_VCN18_EN_SHIFT);
		pmic_config_interface(PMIC_RG_LDO_VCN18_SW_OP_EN_ADDR, 1,
				      PMIC_RG_LDO_VCN18_SW_OP_EN_MASK,
				      PMIC_RG_LDO_VCN18_SW_OP_EN_SHIFT);
	} else {
		pmic_config_interface(PMIC_RG_LDO_VCN18_EN_ADDR, 0,
				      PMIC_RG_LDO_VCN18_EN_MASK,
				      PMIC_RG_LDO_VCN18_EN_SHIFT);
	}
#endif
}

static bool clk_buf_is_auto_calc(void)
{
	if (CLK_BUF1_STATUS_PMIC != CLOCK_BUFFER_DISABLE)
		if (PMIC_CLK_BUF1_DRIVING_CURR == CLK_BUF_DRIVING_CURR_AUTO_K)
			return true; /* choose auto mode */
		else
			drv_curr_vals[XO_SOC] = PMIC_CLK_BUF1_DRIVING_CURR;
	if (CLK_BUF2_STATUS_PMIC != CLOCK_BUFFER_DISABLE)
		if (PMIC_CLK_BUF2_DRIVING_CURR == CLK_BUF_DRIVING_CURR_AUTO_K)
			return true; /* choose auto mode */
		else
			drv_curr_vals[XO_WCN] = PMIC_CLK_BUF2_DRIVING_CURR;
	if (CLK_BUF3_STATUS_PMIC != CLOCK_BUFFER_DISABLE)
		if (PMIC_CLK_BUF3_DRIVING_CURR == CLK_BUF_DRIVING_CURR_AUTO_K)
			return true; /* choose auto mode */
		else
			drv_curr_vals[XO_NFC] = PMIC_CLK_BUF3_DRIVING_CURR;
	if (CLK_BUF4_STATUS_PMIC != CLOCK_BUFFER_DISABLE)
		if (PMIC_CLK_BUF4_DRIVING_CURR == CLK_BUF_DRIVING_CURR_AUTO_K)
			return true; /* choose auto mode */
		else
			drv_curr_vals[XO_CEL] = PMIC_CLK_BUF4_DRIVING_CURR;
	if (CLK_BUF5_STATUS_PMIC != CLOCK_BUFFER_DISABLE)
		if (PMIC_CLK_BUF5_DRIVING_CURR == CLK_BUF_DRIVING_CURR_AUTO_K)
			return true; /* choose auto mode */
		else
			drv_curr_vals[XO_AUD] = PMIC_CLK_BUF5_DRIVING_CURR;
	if (CLK_BUF6_STATUS_PMIC != CLOCK_BUFFER_DISABLE)
		if (PMIC_CLK_BUF6_DRIVING_CURR == CLK_BUF_DRIVING_CURR_AUTO_K)
			return true; /* choose auto mode */
		else
			drv_curr_vals[XO_PD] = PMIC_CLK_BUF6_DRIVING_CURR;
	if (CLK_BUF7_STATUS_PMIC != CLOCK_BUFFER_DISABLE)
		if (PMIC_CLK_BUF7_DRIVING_CURR == CLK_BUF_DRIVING_CURR_AUTO_K)
			return true; /* choose auto mode */
		else
			drv_curr_vals[XO_EXT] = PMIC_CLK_BUF7_DRIVING_CURR;

	return false;
}

static void clk_buf_set_auto_calc(u8 onoff)
{
	if (onoff) {
		pmic_config_interface(PMIC_XO_BUFLDOK_EN_ADDR, 0,
				    PMIC_XO_BUFLDOK_EN_MASK, PMIC_XO_BUFLDOK_EN_SHIFT);
		udelay(100);
		pmic_config_interface(PMIC_XO_BUFLDOK_EN_ADDR, 1,
				    PMIC_XO_BUFLDOK_EN_MASK, PMIC_XO_BUFLDOK_EN_SHIFT);
		mdelay(1);
	} else
		pmic_config_interface(PMIC_XO_BUFLDOK_EN_ADDR, 0,
				    PMIC_XO_BUFLDOK_EN_MASK, PMIC_XO_BUFLDOK_EN_SHIFT);
}

static void clk_buf_set_manual_drv_curr(u8 is_force)
{
#ifndef CLKBUF_BRINGUP
	u32 drv_curr_val = 0, drv_curr_mask = 0, drv_curr_shift = 0;

	if (is_force) {
		drv_curr_val =
			(CLK_BUF_DRIVING_CURR_1 << PMIC_XO_EXTBUF1_ISET_M_SHIFT) |
			(CLK_BUF_DRIVING_CURR_1 << PMIC_XO_EXTBUF2_ISET_M_SHIFT) |
			(CLK_BUF_DRIVING_CURR_1 << PMIC_XO_EXTBUF3_ISET_M_SHIFT) |
			(CLK_BUF_DRIVING_CURR_1 << PMIC_XO_EXTBUF4_ISET_M_SHIFT) |
			/* (CLK_BUF_DRIVING_CURR_1 << PMIC_XO_EXTBUF5_ISET_M_SHIFT) | */
			(CLK_BUF_DRIVING_CURR_1 << PMIC_XO_EXTBUF6_ISET_M_SHIFT) |
			(CLK_BUF_DRIVING_CURR_1 << PMIC_XO_EXTBUF7_ISET_M_SHIFT);
	} else {
		drv_curr_vals[XO_SOC] = PMIC_CLK_BUF1_DRIVING_CURR;
		drv_curr_vals[XO_WCN] = PMIC_CLK_BUF2_DRIVING_CURR;
		drv_curr_vals[XO_NFC] = PMIC_CLK_BUF3_DRIVING_CURR;
		drv_curr_vals[XO_CEL] = PMIC_CLK_BUF4_DRIVING_CURR;
		drv_curr_vals[XO_PD] = PMIC_CLK_BUF6_DRIVING_CURR;
		drv_curr_vals[XO_EXT] = PMIC_CLK_BUF7_DRIVING_CURR;
		drv_curr_val =
			(drv_curr_vals[XO_SOC] << PMIC_XO_EXTBUF1_ISET_M_SHIFT) |
			(drv_curr_vals[XO_WCN] << PMIC_XO_EXTBUF2_ISET_M_SHIFT) |
			(drv_curr_vals[XO_NFC] << PMIC_XO_EXTBUF3_ISET_M_SHIFT) |
			(drv_curr_vals[XO_CEL] << PMIC_XO_EXTBUF4_ISET_M_SHIFT) |
			/* (drv_curr_vals[XO_AUD] << PMIC_XO_EXTBUF5_ISET_M_SHIFT) | */
			(drv_curr_vals[XO_PD] << PMIC_XO_EXTBUF6_ISET_M_SHIFT) |
			(drv_curr_vals[XO_EXT] << PMIC_XO_EXTBUF7_ISET_M_SHIFT);
	}

	drv_curr_mask =
		(PMIC_XO_EXTBUF1_ISET_M_MASK << PMIC_XO_EXTBUF1_ISET_M_SHIFT) |
		(PMIC_XO_EXTBUF2_ISET_M_MASK << PMIC_XO_EXTBUF2_ISET_M_SHIFT) |
		(PMIC_XO_EXTBUF3_ISET_M_MASK << PMIC_XO_EXTBUF3_ISET_M_SHIFT) |
		(PMIC_XO_EXTBUF4_ISET_M_MASK << PMIC_XO_EXTBUF4_ISET_M_SHIFT) |
		/* (PMIC_XO_EXTBUF5_ISET_M_MASK << PMIC_XO_EXTBUF5_ISET_M_SHIFT) | */
		(PMIC_XO_EXTBUF6_ISET_M_MASK << PMIC_XO_EXTBUF6_ISET_M_SHIFT) |
		(PMIC_XO_EXTBUF7_ISET_M_MASK << PMIC_XO_EXTBUF7_ISET_M_SHIFT);
	drv_curr_shift = PMIC_XO_EXTBUF1_ISET_M_SHIFT;

	pmic_config_interface(PMIC_XO_EXTBUF1_ISET_M_ADDR, drv_curr_val, drv_curr_mask,
			      drv_curr_shift);
#endif
}

static void clk_buf_init_pmic_clkbuf(void)
{
#ifndef CLKBUF_BRINGUP
	/* Dump registers before setting */
	clk_buf_dump_clkbuf_log();

	/* Setup initial PMIC clock buffer setting */
	/* de-sense setting */
	pmic_config_interface(PMIC_RG_XO_EXTBUF1_HD_ADDR,
			      PMIC_CW15_INIT_VAL,
			      PMIC_REG_MASK, PMIC_REG_SHIFT);
	pmic_config_interface(PMIC_RG_XO_RESERVED4_ADDR, XO_RESERVED4_INIT_VAL,
			      PMIC_RG_XO_RESERVED4_MASK,
			      PMIC_RG_XO_RESERVED4_SHIFT);

#ifdef CLKBUF_DETECT_AUTO_CALC
	if (clk_buf_is_auto_calc()) {
		/* Adjust auto-K parameter */
		pmic_config_interface(PMIC_RG_XO_RESRVED10_ADDR, 0x4, 0x7, 9);

		clk_buf_ctrl_vcn18(1);
		clk_buf_set_auto_calc(1);
		clk_buf_ctrl_vcn18(0);
	} else {
		clk_buf_set_auto_calc(0);
		clk_buf_set_manual_drv_curr(0);
	}
#else
	clk_buf_set_auto_calc(0);
	clk_buf_set_manual_drv_curr(0);
#endif

	/* XO_WCN */
#ifdef CLKBUF_CONN_SUPPORT_CTRL_FROM_I1
	pmic_config_interface(PMIC_XO_EXTBUF2_CLKSEL_MAN_ADDR, 0x1,
			    PMIC_XO_EXTBUF2_CLKSEL_MAN_MASK,
			    PMIC_XO_EXTBUF2_CLKSEL_MAN_SHIFT);
#else
	pmic_config_interface(PMIC_RG_SRCLKEN_IN3_EN_ADDR, 0,
			    PMIC_RG_SRCLKEN_IN3_EN_MASK, PMIC_RG_SRCLKEN_IN3_EN_SHIFT);
#endif
	/* clock buffer settting */
	pmic_config_interface(PMIC_XO_EXTBUF1_MODE_ADDR, PMIC_CW00_INIT_VAL,
			    PMIC_REG_MASK, PMIC_REG_SHIFT);
	pmic_config_interface(PMIC_XO_EXTBUF7_MODE_ADDR, PMIC_CW11_INIT_VAL,
			    PMIC_REG_MASK, PMIC_REG_SHIFT);

	/* disable clock buffer by DCT setting */
	if (CLK_BUF2_STATUS_PMIC == CLOCK_BUFFER_DISABLE) {
		pmic_config_interface(PMIC_DCXO_CW00_CLR_ADDR,
				      PMIC_XO_EXTBUF2_MODE_MASK,
				      PMIC_XO_EXTBUF2_MODE_MASK,
				      PMIC_XO_EXTBUF2_MODE_SHIFT);
		pmic_config_interface(PMIC_DCXO_CW00_CLR_ADDR,
				      PMIC_XO_EXTBUF2_EN_M_MASK,
				      PMIC_XO_EXTBUF2_EN_M_MASK,
				      PMIC_XO_EXTBUF2_EN_M_SHIFT);
	}
	if (CLK_BUF3_STATUS_PMIC == CLOCK_BUFFER_DISABLE) {
		pmic_config_interface(PMIC_DCXO_CW00_CLR_ADDR,
				      PMIC_XO_EXTBUF3_MODE_MASK,
				      PMIC_XO_EXTBUF3_MODE_MASK,
				      PMIC_XO_EXTBUF3_MODE_SHIFT);
		pmic_config_interface(PMIC_DCXO_CW00_CLR_ADDR,
				      PMIC_XO_EXTBUF3_EN_M_MASK,
				      PMIC_XO_EXTBUF3_EN_M_MASK,
				      PMIC_XO_EXTBUF3_EN_M_SHIFT);
	}
	if (CLK_BUF4_STATUS_PMIC == CLOCK_BUFFER_DISABLE) {
		pmic_config_interface(PMIC_DCXO_CW00_CLR_ADDR,
				      PMIC_XO_EXTBUF4_MODE_MASK,
				      PMIC_XO_EXTBUF4_MODE_MASK,
				      PMIC_XO_EXTBUF4_MODE_SHIFT);
		pmic_config_interface(PMIC_DCXO_CW00_CLR_ADDR,
				      PMIC_XO_EXTBUF4_EN_M_MASK,
				      PMIC_XO_EXTBUF4_EN_M_MASK,
				      PMIC_XO_EXTBUF4_EN_M_SHIFT);
	}
	if (CLK_BUF6_STATUS_PMIC == CLOCK_BUFFER_DISABLE) {
		pmic_config_interface(PMIC_DCXO_CW11_CLR_ADDR,
				      PMIC_XO_EXTBUF6_MODE_MASK,
				      PMIC_XO_EXTBUF6_MODE_MASK,
				      PMIC_XO_EXTBUF6_MODE_SHIFT);
		pmic_config_interface(PMIC_DCXO_CW11_CLR_ADDR,
				      PMIC_XO_EXTBUF6_EN_M_MASK,
				      PMIC_XO_EXTBUF6_EN_M_MASK,
				      PMIC_XO_EXTBUF6_EN_M_SHIFT);
	}
	if (CLK_BUF7_STATUS_PMIC == CLOCK_BUFFER_DISABLE) {
		pmic_config_interface(PMIC_DCXO_CW11_CLR_ADDR,
				      PMIC_XO_EXTBUF7_MODE_MASK,
				      PMIC_XO_EXTBUF7_MODE_MASK,
				      PMIC_XO_EXTBUF7_MODE_SHIFT);
		pmic_config_interface(PMIC_DCXO_CW11_CLR_ADDR,
				      PMIC_XO_EXTBUF7_EN_M_MASK,
				      PMIC_XO_EXTBUF7_EN_M_MASK,
				      PMIC_XO_EXTBUF7_EN_M_SHIFT);
	}

	/* Check if the setting is ok */
	clk_buf_dump_clkbuf_log();
#endif
}

static void clk_buf_init_pmic_wrap(void)
{
#ifndef CLKBUF_BRINGUP
	u32 dcxo_en_flag = 0;

	/* Setup PMIC_WRAP setting for XO2 & XO3 */
	if (CLK_BUF2_STATUS_PMIC != CLOCK_BUFFER_DISABLE) {
#ifdef CLKBUF_CONN_SUPPORT_CTRL_FROM_I1
		clkbuf_writel(DCXO_CONN_ADR0, PMIC_DCXO_CW00_CLR_ADDR);
		clkbuf_writel(DCXO_CONN_WDATA0,
		      	PMIC_XO_EXTBUF2_EN_M_MASK << PMIC_XO_EXTBUF2_EN_M_SHIFT);	/* bit5 = 0 */
		clkbuf_writel(DCXO_CONN_ADR1, PMIC_DCXO_CW00_SET_ADDR);
		clkbuf_writel(DCXO_CONN_WDATA1,
		     	 PMIC_XO_EXTBUF2_EN_M_MASK << PMIC_XO_EXTBUF2_EN_M_SHIFT);	/* bit5 = 1 */
#else
		clkbuf_writel(DCXO_CONN_ADR0, PMIC_RG_SRCLKEN_IN3_EN_ADDR);
		clkbuf_writel(DCXO_CONN_WDATA0,
			      0 << PMIC_RG_SRCLKEN_IN3_EN_SHIFT); /* bit0 = 0 */
		clkbuf_writel(DCXO_CONN_ADR1, PMIC_RG_SRCLKEN_IN3_EN_ADDR);
		clkbuf_writel(DCXO_CONN_WDATA1,
			      1 << PMIC_RG_SRCLKEN_IN3_EN_SHIFT); /* bit0 = 1 */
#endif
		dcxo_en_flag |= DCXO_CONN_ENABLE;
	}

	if (CLK_BUF3_STATUS_PMIC != CLOCK_BUFFER_DISABLE) {
		clkbuf_writel(DCXO_NFC_ADR0, PMIC_DCXO_CW00_CLR_ADDR);
		clkbuf_writel(DCXO_NFC_WDATA0,
				PMIC_XO_EXTBUF3_EN_M_MASK << PMIC_XO_EXTBUF3_EN_M_SHIFT);	/* bit8 = 0 */
		clkbuf_writel(DCXO_NFC_ADR1, PMIC_DCXO_CW00_SET_ADDR);
		clkbuf_writel(DCXO_NFC_WDATA1,
				PMIC_XO_EXTBUF3_EN_M_MASK << PMIC_XO_EXTBUF3_EN_M_SHIFT);	/* bit8 = 1 */
		dcxo_en_flag |= DCXO_NFC_ENABLE;
	}

	clkbuf_writel(DCXO_ENABLE, dcxo_en_flag);

	clk_buf_warn("%s: DCXO_CONN_ADR0/WDATA0/ADR1/WDATA1=0x%x/%x/%x/%x\n",
		     __func__, clkbuf_readl(DCXO_CONN_ADR0),
		     clkbuf_readl(DCXO_CONN_WDATA0),
		     clkbuf_readl(DCXO_CONN_ADR1),
		     clkbuf_readl(DCXO_CONN_WDATA1));
	clk_buf_warn("%s: DCXO_NFC_ADR0/WDATA0/ADR1/WDATA1/EN=0x%x/%x/%x/%x/%x\n",
		     __func__, clkbuf_readl(DCXO_NFC_ADR0),
		     clkbuf_readl(DCXO_NFC_WDATA0),
		     clkbuf_readl(DCXO_NFC_ADR1),
		     clkbuf_readl(DCXO_NFC_WDATA1),
		     clkbuf_readl(DCXO_ENABLE));
#endif /* #ifndef CLKBUF_BRINGUP */
}

int clk_buf_init(void)
{
#ifdef CLKBUF_BRINGUP
	clk_buf_warn("%s: is skipped for bring-up\n", __func__);

	return 0;
#endif /* CLKBUF_BRINGUP */

	if (clk_buf_dts_map())
		return -1;

	clk_buf_dump_dts_log();

	if (clk_buf_fs_init())
		return -1;

	/* Co-TSX @PMIC */
	if (is_clk_buf_from_pmic()) {
		clk_buf_init_pmic_clkbuf();
		clk_buf_init_pmic_wrap();
	}

	return 0;
}
late_initcall(clk_buf_init);

