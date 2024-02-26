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
#ifdef MTK_SRCLKEN_RC_SUPPORT
#include <srclken_rc.h>
#endif

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

/* TODO: marked this after driver is ready */
/*  #define CLKBUF_BRINGUP */

/* #define CLKBUF_CONN_SUPPORT_CTRL_FROM_I1 */

#if MTK_SRCLKEN_RC_FULL_SET
#define PMIC_CW00_INIT_VAL			0x4A4D
#define PMIC_CW09_INIT_VAL			0x51F0
#else
#define PMIC_CW00_INIT_VAL			0x4E1D
#define PMIC_CW09_INIT_VAL			0x41F0
#endif

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
	clk_buf_warn("%s: PMIC_CLK_BUF?_STATUS 1/2/3/4/7=%d %d %d %d %d\n", __func__,
		     CLK_BUF1_STATUS_PMIC, CLK_BUF2_STATUS_PMIC,
		     CLK_BUF3_STATUS_PMIC, CLK_BUF4_STATUS_PMIC,
		     CLK_BUF7_STATUS_PMIC);
	clk_buf_warn("%s: PMIC_CLK_BUF?_OUTPUT_IMPEDANCE 1/2/3/4/7=%d %d %d %d %d\n", __func__,
		     PMIC_CLK_BUF1_OUTPUT_IMPEDANCE, PMIC_CLK_BUF2_OUTPUT_IMPEDANCE,
		     PMIC_CLK_BUF3_OUTPUT_IMPEDANCE, PMIC_CLK_BUF4_OUTPUT_IMPEDANCE,
		     PMIC_CLK_BUF7_OUTPUT_IMPEDANCE);
	clk_buf_warn("%s: PMIC_CLK_BUF2_CONTROLS_FOR_DESENSE 2/3/4=%d %d %d\n", __func__,
		     PMIC_CLK_BUF2_CONTROLS_FOR_DESENSE,
		     PMIC_CLK_BUF3_CONTROLS_FOR_DESENSE,
		     PMIC_CLK_BUF4_CONTROLS_FOR_DESENSE);
#endif
}

static void clk_buf_dump_clkbuf_log(void)
{
#ifndef CLKBUF_BRINGUP
	u32 pmic_cw00 = 0, pmic_cw09 = 0, pmic_cw12 = 0, pmic_cw13 = 0,
	    pmic_cw15 = 0, pmic_cw19 = 0, top_spi_con1 = 0,
	    ldo_vrfck_op_en = 0, ldo_vbbck_op_en = 0, ldo_vrfck_en = 0,
	    ldo_vbbck_en = 0;

	pmic_read_interface(PMIC_XO_EXTBUF1_MODE_ADDR, &pmic_cw00,
			    PMIC_REG_MASK, PMIC_REG_SHIFT);
	pmic_read_interface(PMIC_XO_EXTBUF7_MODE_ADDR, &pmic_cw09,
			    PMIC_REG_MASK, PMIC_REG_SHIFT);
	pmic_read_interface(PMIC_XO_EXTBUF2_CLKSEL_MAN_ADDR, &pmic_cw12,
			    PMIC_REG_MASK, PMIC_REG_SHIFT);
	pmic_read_interface(PMIC_RG_XO_EXTBUF2_SRSEL_ADDR, &pmic_cw13,
			    PMIC_REG_MASK, PMIC_REG_SHIFT);
	pmic_read_interface(PMIC_RG_XO_RESERVED1_ADDR, &pmic_cw15,
			    PMIC_REG_MASK, PMIC_REG_SHIFT);
	pmic_read_interface(PMIC_RG_XO_EXTBUF2_RSEL_ADDR, &pmic_cw19,
			    PMIC_REG_MASK, PMIC_REG_SHIFT);
	pmic_read_interface(PMIC_RG_SRCLKEN_IN3_EN_ADDR, &top_spi_con1,
			    PMIC_RG_SRCLKEN_IN3_EN_MASK, PMIC_RG_SRCLKEN_IN3_EN_SHIFT);
	pmic_read_interface(PMIC_RG_LDO_VRFCK_HW14_OP_EN_ADDR, &ldo_vrfck_op_en,
			    PMIC_RG_LDO_VRFCK_HW14_OP_EN_MASK, PMIC_RG_LDO_VRFCK_HW14_OP_EN_SHIFT);
	pmic_read_interface(PMIC_RG_LDO_VBBCK_HW14_OP_EN_ADDR, &ldo_vbbck_op_en,
			    PMIC_RG_LDO_VBBCK_HW14_OP_EN_MASK, PMIC_RG_LDO_VBBCK_HW14_OP_EN_SHIFT);
	pmic_read_interface(PMIC_RG_LDO_VRFCK_EN_ADDR, &ldo_vrfck_en,
			    PMIC_RG_LDO_VRFCK_EN_MASK, PMIC_RG_LDO_VRFCK_EN_SHIFT);
	pmic_read_interface(PMIC_RG_LDO_VBBCK_EN_ADDR, &ldo_vbbck_en,
			    PMIC_RG_LDO_VBBCK_EN_MASK, PMIC_RG_LDO_VBBCK_EN_SHIFT);
	clk_buf_warn("%s DCXO_CW00/09/12/13/15/19=0x%x %x %x %x %x %x\n",
		     __func__, pmic_cw00, pmic_cw09, pmic_cw12, pmic_cw13, pmic_cw15, pmic_cw19);
	clk_buf_warn("%s spi_con1/ldo_rf_op/ldo_bb_op/ldo_rf_en/ldo_bb_en=0x%x %x %x %x %x\n",
		     __func__, top_spi_con1, ldo_vrfck_op_en, ldo_vbbck_op_en, ldo_vrfck_en, ldo_vbbck_en);
#endif
}

static void clk_buf_init_pmic_clkbuf(void)
{
#ifndef CLKBUF_BRINGUP
	/* Dump registers before setting */
	clk_buf_dump_clkbuf_log();

	/* enable XO LDO */
	pmic_config_interface(PMIC_RG_LDO_VRFCK_OP_EN_SET_ADDR, 0x1,
			      PMIC_RG_LDO_VRFCK_HW14_OP_EN_MASK, PMIC_RG_LDO_VRFCK_HW14_OP_EN_SHIFT);
	pmic_config_interface(PMIC_RG_LDO_VBBCK_OP_EN_SET_ADDR, 0x1,
			      PMIC_RG_LDO_VBBCK_HW14_OP_EN_MASK, PMIC_RG_LDO_VBBCK_HW14_OP_EN_SHIFT);
	pmic_config_interface(PMIC_RG_LDO_VRFCK_EN_ADDR, 0x0,
			      PMIC_RG_LDO_VRFCK_EN_MASK, PMIC_RG_LDO_VRFCK_EN_SHIFT);
	pmic_config_interface(PMIC_RG_LDO_VBBCK_EN_ADDR, 0x0,
			      PMIC_RG_LDO_VBBCK_EN_MASK, PMIC_RG_LDO_VBBCK_EN_SHIFT);

	/* Setup initial PMIC clock buffer setting */
	/* 1.1 Buffer de-sense setting */
	/* FIXME: read dts and set to SRSEL and HD */
	pmic_config_interface(PMIC_RG_XO_EXTBUF2_SRSEL_ADDR, PMIC_CLK_BUF2_CONTROLS_FOR_DESENSE,
			      PMIC_RG_XO_EXTBUF2_SRSEL_MASK, PMIC_RG_XO_EXTBUF2_SRSEL_SHIFT);
	pmic_config_interface(PMIC_RG_XO_EXTBUF3_HD_ADDR, PMIC_CLK_BUF3_CONTROLS_FOR_DESENSE,
			      PMIC_RG_XO_EXTBUF3_HD_MASK, PMIC_RG_XO_EXTBUF3_HD_SHIFT);
	pmic_config_interface(PMIC_RG_XO_EXTBUF4_SRSEL_ADDR, PMIC_CLK_BUF4_CONTROLS_FOR_DESENSE,
			      PMIC_RG_XO_EXTBUF4_SRSEL_MASK, PMIC_RG_XO_EXTBUF4_SRSEL_SHIFT);

	/* 1.2 Buffer setting for trace impedance */
	/* FIXME: read dts and set to RSEL */
	pmic_config_interface(PMIC_RG_XO_EXTBUF1_RSEL_ADDR, PMIC_CLK_BUF1_OUTPUT_IMPEDANCE,
			      PMIC_RG_XO_EXTBUF1_RSEL_MASK, PMIC_RG_XO_EXTBUF1_RSEL_SHIFT);
	pmic_config_interface(PMIC_RG_XO_EXTBUF2_RSEL_ADDR, PMIC_CLK_BUF2_OUTPUT_IMPEDANCE,
			      PMIC_RG_XO_EXTBUF2_RSEL_MASK, PMIC_RG_XO_EXTBUF2_RSEL_SHIFT);
	pmic_config_interface(PMIC_RG_XO_EXTBUF3_RSEL_ADDR, PMIC_CLK_BUF3_OUTPUT_IMPEDANCE,
			      PMIC_RG_XO_EXTBUF3_RSEL_MASK, PMIC_RG_XO_EXTBUF3_RSEL_SHIFT);
	pmic_config_interface(PMIC_RG_XO_EXTBUF4_RSEL_ADDR, PMIC_CLK_BUF4_OUTPUT_IMPEDANCE,
			      PMIC_RG_XO_EXTBUF4_RSEL_MASK, PMIC_RG_XO_EXTBUF4_RSEL_SHIFT);
	pmic_config_interface(PMIC_RG_XO_EXTBUF7_RSEL_ADDR, PMIC_CLK_BUF7_OUTPUT_IMPEDANCE,
			      PMIC_RG_XO_EXTBUF7_RSEL_MASK, PMIC_RG_XO_EXTBUF7_RSEL_SHIFT);

	/* 1.3 XO_WCN/XO_CEL phase delay */
	pmic_config_interface(PMIC_RG_XO_RESERVED1_ADDR, 0xF,
			      PMIC_RG_XO_RESERVED1_MASK, PMIC_RG_XO_RESERVED1_SHIFT);

	/* 1.4 26M enable control */
#ifndef MTK_SRCLKEN_RC_SUPPORT

	/*Legacy co-clock mode */
#ifdef CLKBUF_CONN_SUPPORT_CTRL_FROM_I1
	pmic_config_interface(PMIC_XO_EXTBUF2_CLKSEL_MAN_ADDR, 0x1,
			    PMIC_XO_EXTBUF2_CLKSEL_MAN_MASK, PMIC_XO_EXTBUF2_CLKSEL_MAN_SHIFT);
#else
	pmic_config_interface(PMIC_RG_SRCLKEN_IN3_EN_ADDR, 0,
			    PMIC_RG_SRCLKEN_IN3_EN_MASK, PMIC_RG_SRCLKEN_IN3_EN_SHIFT);
#endif
	pmic_config_interface(PMIC_XO_EXTBUF1_MODE_ADDR, PMIC_CW00_INIT_VAL,
			    PMIC_REG_MASK, PMIC_REG_SHIFT);
	pmic_config_interface(PMIC_XO_EXTBUF7_MODE_ADDR, PMIC_CW09_INIT_VAL,
			    PMIC_REG_MASK, PMIC_REG_SHIFT);

#else /* MTK_SRCLKEN_RC_SUPPORT */

#if MTK_SRCLKEN_RC_BT_ONLY || MTK_SRCLKEN_RC_VFE28_ONLY

	/* only PMRC_EN[5] to control MODE and XO */
#ifdef CLKBUF_CONN_SUPPORT_CTRL_FROM_I1
	pmic_config_interface(PMIC_XO_EXTBUF2_CLKSEL_MAN_ADDR, 0x1,
			    PMIC_XO_EXTBUF2_CLKSEL_MAN_MASK, PMIC_XO_EXTBUF2_CLKSEL_MAN_SHIFT);
#else
	pmic_config_interface(PMIC_RG_SRCLKEN_IN3_EN_ADDR, 0,
			    PMIC_RG_SRCLKEN_IN3_EN_MASK, PMIC_RG_SRCLKEN_IN3_EN_SHIFT);
#endif
	pmic_config_interface(PMIC_XO_EXTBUF1_MODE_ADDR, PMIC_CW00_INIT_VAL,
			    PMIC_REG_MASK, PMIC_REG_SHIFT);
	pmic_config_interface(PMIC_XO_EXTBUF7_MODE_ADDR, PMIC_CW09_INIT_VAL,
			    PMIC_REG_MASK, PMIC_REG_SHIFT);

	/*
	* XO_PMIC_TOP_DIG_SW=1
	* XO_MODE_CONN_BT_MASK=1
	* XO_BUF_CONN_BT_MASK=1
	*/
	pmic_config_interface(PMIC_XO_PMIC_TOP_DIG_SW_ADDR, 0x1,
			    PMIC_XO_PMIC_TOP_DIG_SW_MASK, PMIC_XO_PMIC_TOP_DIG_SW_SHIFT);
	pmic_config_interface(PMIC_XO_MODE_CONN_BT_MASK_ADDR, 0x0,
			    PMIC_XO_MODE_CONN_BT_MASK_MASK, PMIC_XO_MODE_CONN_BT_MASK_SHIFT);
	pmic_config_interface(PMIC_XO_BUF_CONN_BT_MASK_ADDR, 0x0,
			    PMIC_XO_BUF_CONN_BT_MASK_MASK, PMIC_XO_BUF_CONN_BT_MASK_SHIFT);

#elif MTK_SRCLKEN_RC_FULL_SET

	/* fully new co-clock mode */

	/* All XO mode should set to 2'b01 */
	pmic_config_interface(PMIC_XO_EXTBUF1_MODE_ADDR, PMIC_CW00_INIT_VAL,
				    PMIC_REG_MASK, PMIC_REG_SHIFT);
	pmic_config_interface(PMIC_XO_EXTBUF7_MODE_ADDR, PMIC_CW09_INIT_VAL,
				    PMIC_REG_MASK, PMIC_REG_SHIFT);

	/* 1.switch to new control mode */
	/*
	* XO_PMIC_TOP_DIG_SW=0
	* XO_MODE_CONN_BT_MASK=0
	* XO_BUF_CONN_BT_MASK=0
	*/
	pmic_config_interface(PMIC_XO_PMIC_TOP_DIG_SW_ADDR, 0x0,
			    PMIC_XO_PMIC_TOP_DIG_SW_MASK, PMIC_XO_PMIC_TOP_DIG_SW_SHIFT);
	pmic_config_interface(PMIC_XO_MODE_CONN_BT_MASK_ADDR, 0x0,
			    PMIC_XO_MODE_CONN_BT_MASK_MASK, PMIC_XO_MODE_CONN_BT_MASK_SHIFT);
	pmic_config_interface(PMIC_XO_BUF_CONN_BT_MASK_ADDR, 0x0,
			    PMIC_XO_BUF_CONN_BT_MASK_MASK, PMIC_XO_BUF_CONN_BT_MASK_SHIFT);

	/* 2.update control mapping table */
	/*
	* XO_SOC_VOTE=11'h006(default) --> too old, need modify
	* XO_WCN_VOTE=11'h078(default) --> too old, need modify
	* XO_NFC_VOTE=11'h100(default) --> too old, need modify
	* XO_CEL_VOTE=11'h001(default) --> too old, need modify
	* XO_EXT_VOTE=11'h200(default) --> too old, need modify
	*/
	pmic_config_interface(PMIC_XO_SOC_VOTE_ADDR, 0x00d,
			    PMIC_XO_SOC_VOTE_MASK, PMIC_XO_SOC_VOTE_SHIFT);
	pmic_config_interface(PMIC_XO_WCN_VOTE_ADDR, 0x0f0,
			    PMIC_XO_WCN_VOTE_MASK, PMIC_XO_WCN_VOTE_SHIFT);
	pmic_config_interface(PMIC_XO_NFC_VOTE_ADDR, 0x200,
			    PMIC_XO_NFC_VOTE_MASK, PMIC_XO_NFC_VOTE_SHIFT);
	pmic_config_interface(PMIC_XO_CEL_VOTE_ADDR, 0x002,
			    PMIC_XO_CEL_VOTE_MASK, PMIC_XO_CEL_VOTE_SHIFT);
	pmic_config_interface(PMIC_XO_EXT_VOTE_ADDR, 0x400,
			    PMIC_XO_EXT_VOTE_MASK, PMIC_XO_EXT_VOTE_SHIFT);
#endif
#endif /* MTK_SRCLKEN_RC_SUPPORT */

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

	if (CLK_BUF7_STATUS_PMIC == CLOCK_BUFFER_DISABLE) {
		pmic_config_interface(PMIC_DCXO_CW09_CLR_ADDR,
				      PMIC_XO_EXTBUF7_MODE_MASK,
				      PMIC_XO_EXTBUF7_MODE_MASK,
				      PMIC_XO_EXTBUF7_MODE_SHIFT);
		pmic_config_interface(PMIC_DCXO_CW09_CLR_ADDR,
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
#if MTK_SRCLKEN_RC_BT_ONLY || MTK_SRCLKEN_RC_VFE28_ONLY \
	|| !defined(MTK_SRCLKEN_RC_SUPPORT)
		clk_buf_init_pmic_wrap();
#endif
		/* Switch to SW control */
#ifdef MTK_SRCLKEN_RC_SUPPORT
		rc_ctrl_mode_switch_sw(CHN_BT);
#endif
	}

	return 0;
}
late_initcall(clk_buf_init);

