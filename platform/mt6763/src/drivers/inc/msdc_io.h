/* Copyright Statement:
*
* This software/firmware and related documentation ("MediaTek Software") are
* protected under relevant copyright laws. The information contained herein
* is confidential and proprietary to MediaTek Inc. and/or its licensors.
* Without the prior written permission of MediaTek inc. and/or its licensors,
* any reproduction, modification, use or disclosure of MediaTek Software,
* and information contained herein, in whole or in part, shall be strictly prohibited.
*/
/* MediaTek Inc. (C) 2015. All rights reserved.
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

#ifndef _MSDC_IO_H_
#define _MSDC_IO_H_

#include "msdc.h"
#include "mmc_types.h"

#if defined(MMC_MSDC_DRV_PRELOADER)
#include "platform.h"
#include "pll.h"
#include "upmu_hw.h"
#endif

#if defined(MMC_MSDC_DRV_LK)
#include "mt_reg_base.h"
#include "upmu_hw.h"
#endif

#if defined(MMC_MSDC_DRV_CTP)
#include "reg_base.H"
#include "intrCtrl.h"
#ifndef FPGA_PLATFORM
#include "pmic.h"
#endif
#endif

/*******************************************************************************
 *PINMUX and GPIO definition
 ******************************************************************************/

#define MSDC_PIN_PULL_NONE      (0)
#define MSDC_PIN_PULL_DOWN      (1)
#define MSDC_PIN_PULL_UP        (2)
#define MSDC_PIN_KEEP           (3)

/* add pull down/up mode define */
#define MSDC_GPIO_PULL_UP       (0)
#define MSDC_GPIO_PULL_DOWN     (1)

#define MSDC_TDRDSEL_SLEEP      (0)
#define MSDC_TDRDSEL_3V         (1)
#define MSDC_TDRDSEL_1V8        (2)
#define MSDC_TDRDSEL_CUST       (3)

/*--------------------------------------------------------------------------*/
/* MSDC0~3 GPIO and IO Pad Configuration Base                               */
/*--------------------------------------------------------------------------*/
#ifndef GPIO_BASE
#error definition of GPIO_BASE does not exist
#endif
#define MSDC_GPIO_BASE          GPIO_BASE

#define MSDC0_IO_PAD_BASE       (IOCFG_TL_BASE)
#define MSDC1_IO_PAD_BASE       (IOCFG_LM_BASE)
#define MSDC3_IO_PAD_BASE       (IOCFG_LB_BASE)

/*--------------------------------------------------------------------------*/
/* MSDC GPIO Related Register                                               */
/*--------------------------------------------------------------------------*/
#define MSDC0_GPIO_MODE16       (MSDC_GPIO_BASE + 0x3F0)
#define MSDC0_GPIO_MODE17       (MSDC_GPIO_BASE + 0x400)
#define MSDC0_GPIO_IES_ADDR     (MSDC0_IO_PAD_BASE + 0x0)
#define MSDC0_GPIO_SMT_ADDR     (MSDC0_IO_PAD_BASE + 0x10)
#define MSDC0_GPIO_TDSEL0_ADDR  (MSDC0_IO_PAD_BASE + 0x20)
#define MSDC0_GPIO_RDSEL0_ADDR  (MSDC0_IO_PAD_BASE + 0x40)
#define MSDC0_GPIO_DRV0_ADDR    (MSDC0_IO_PAD_BASE + 0xa0)
#define MSDC0_GPIO_PUPD0_ADDR   (MSDC0_IO_PAD_BASE + 0xc0)
#define MSDC0_GPIO_PUPD1_ADDR   (MSDC0_IO_PAD_BASE + 0xd0)

/* MSDC1 */
#define MSDC1_GPIO_MODE4        (MSDC_GPIO_BASE + 0x330)
#define MSDC1_GPIO_MODE5        (MSDC_GPIO_BASE + 0x340)
#define MSDC1_GPIO_IES_ADDR     (MSDC1_IO_PAD_BASE + 0x0)
#define MSDC1_GPIO_SMT_ADDR     (MSDC1_IO_PAD_BASE + 0x10)
#define MSDC1_GPIO_TDSEL0_ADDR  (MSDC1_IO_PAD_BASE + 0x20)
#define MSDC1_GPIO_RDSEL0_ADDR  (MSDC1_IO_PAD_BASE + 0x40)
#define MSDC1_GPIO_DRV0_ADDR    (MSDC1_IO_PAD_BASE + 0xa0)
#define MSDC1_GPIO_PUPD0_ADDR   (MSDC1_IO_PAD_BASE + 0xc0)

/* MSDC3 */
#define MSDC3_GPIO_MODE7        (MSDC_GPIO_BASE + 0x360)
#define MSDC3_GPIO_MODE8        (MSDC_GPIO_BASE + 0x370)
#define MSDC3_GPIO_IES_ADDR     (MSDC3_IO_PAD_BASE + 0x0)
#define MSDC3_GPIO_SMT_ADDR     (MSDC3_IO_PAD_BASE + 0x10)
#define MSDC3_GPIO_TDSEL_ADDR   (MSDC3_IO_PAD_BASE + 0x20)
#define MSDC3_GPIO_RDSEL_ADDR   (MSDC3_IO_PAD_BASE + 0x40)
#define MSDC3_GPIO_DRV_ADDR     (MSDC3_IO_PAD_BASE + 0xA0)
#define MSDC3_GPIO_PUPD_ADDR    (MSDC3_IO_PAD_BASE + 0xC0)

/*
 * MSDC0 GPIO and PAD register and bitfields definition
 */
/* MSDC0_GPIO_MODE16, 001b is msdc mode*/
#define MSDC0_MODE_DAT6_AUX1_MASK    (0x7 << 28)
#define MSDC0_MODE_DAT4_AUX1_MASK    (0x7 << 24)
#define MSDC0_MODE_DAT2_AUX1_MASK    (0x7 << 20)
#define MSDC0_MODE_CLK_AUX1_MASK     (0x7 << 16)
#define MSDC0_MODE_DAT0_AUX1_MASK    (0x7 << 12)
#define MSDC0_MODE_CMD_AUX1_MASK     (0x7 << 8)

#define MSDC0_MODE_DAT3_AUX2_MASK MSDC0_MODE_DAT6_AUX1_MASK
#define MSDC0_MODE_DAT2_AUX2_MASK MSDC0_MODE_DAT4_AUX1_MASK
#define MSDC0_MODE_DAT5_AUX2_MASK MSDC0_MODE_DAT2_AUX1_MASK
#define MSDC0_MODE_DAT4_AUX2_MASK MSDC0_MODE_DAT0_AUX1_MASK

/* MSDC0_GPIO_MODE17, 001b is msdc mode */
#define MSDC0_MODE_RSTB_AUX1_MASK    (0x7 << 20)
#define MSDC0_MODE_DAT3_AUX1_MASK    (0x7 << 16)
#define MSDC0_MODE_DSL_AUX1_MASK     (0x7 << 12)
#define MSDC0_MODE_DAT7_AUX1_MASK    (0x7 << 8)
#define MSDC0_MODE_DAT5_AUX1_MASK    (0x7 << 4)
#define MSDC0_MODE_DAT1_AUX1_MASK    (0x7 << 0)

#define MSDC0_MODE_DAT1_AUX2_MASK MSDC0_MODE_DAT3_AUX1_MASK
#define MSDC0_MODE_DAT0_AUX2_MASK MSDC0_MODE_DAT5_AUX1_MASK
#define MSDC0_MODE_DAT6_AUX2_MASK MSDC0_MODE_DAT1_AUX1_MASK

/* MSDC0 IES mask*/
#define MSDC0_IES_RSTB_MASK     (0x1  <<  4)
#define MSDC0_IES_DSL_MASK      (0x1  <<  3)
#define MSDC0_IES_CLK_MASK      (0x1  <<  2)
#define MSDC0_IES_DAT_MASK      (0x1  <<  1)
#define MSDC0_IES_CMD_MASK      (0x1  <<  0)
#define MSDC0_IES_ALL_MASK      (0x1f <<  0)
/* MSDC0 SMT mask*/
#define MSDC0_SMT_RSTB_MASK     (0x1 <<   4)
#define MSDC0_SMT_DSL_MASK      (0x1 <<   3)
#define MSDC0_SMT_CLK_MASK      (0x1 <<   2)
#define MSDC0_SMT_DAT_MASK      (0x1 <<   1)
#define MSDC0_SMT_CMD_MASK      (0x1 <<   0)
#define MSDC0_SMT_ALL_MASK      (0x1f <<  0)
/* MSDC0 TDSEL mask*/
#define MSDC0_TDSEL0_RSTB_MASK  (0xf  << 16)
#define MSDC0_TDSEL0_DSL_MASK   (0xf  << 12)
#define MSDC0_TDSEL0_CLK_MASK   (0xf  <<  8)
#define MSDC0_TDSEL0_DAT_MASK   (0xf  <<  4)
#define MSDC0_TDSEL0_CMD_MASK   (0xf  <<  0)
#define MSDC0_TDSEL0_ALL_MASK   (0xfffff << 0)
/* MSDC0 RDSEL mask*/
#define MSDC0_RDSEL0_RSTB_MASK  (0x3f << 24)
#define MSDC0_RDSEL0_DSL_MASK   (0x3f << 18)
#define MSDC0_RDSEL0_CLK_MASK   (0x3f << 12)
#define MSDC0_RDSEL0_DAT_MASK   (0x3f <<  6)
#define MSDC0_RDSEL0_CMD_MASK   (0x3f <<  0)
#define MSDC0_RDSEL0_ALL_MASK   (0x3fffffff << 0)
/* MSDC0 SR mask*/
#define MSDC0_SR_DAT_MASK       (0x1  << 19)
#define MSDC0_SR_CMD_MASK       (0x1  << 15)
#define MSDC0_SR_CLK_MASK       (0x1  << 11)
#define MSDC0_SR_DSL_MASK       (0x1  <<  7)
#define MSDC0_SR_RSTB_MASK      (0x1  <<  3)
/* MSDC0 DRV mask*/
#define MSDC0_DRV0_RSTB_MASK     (0x7  << 16)
#define MSDC0_DRV0_DSL_MASK      (0x7  << 12)
#define MSDC0_DRV0_CLK_MASK      (0x7  <<  8)
#define MSDC0_DRV0_DAT_MASK      (0x7  <<  4)
#define MSDC0_DRV0_CMD_MASK      (0x7  <<  0)
#define MSDC0_DRV0_ALL_MASK      (0xfffff << 0)
/* MSDC0 PUPD mask */
#define MSDC0_PUPD0_DAT5_MASK    (0x7  << 28)
#define MSDC0_PUPD0_DAT1_MASK    (0x7  << 24)
#define MSDC0_PUPD0_DAT6_MASK    (0x7  << 20)
#define MSDC0_PUPD0_DAT4_MASK    (0x7  << 16)
#define MSDC0_PUPD0_DAT2_MASK    (0x7  << 12)
#define MSDC0_PUPD0_CLK_MASK     (0x7  <<  8)
#define MSDC0_PUPD0_DAT0_MASK    (0x7  <<  4)
#define MSDC0_PUPD0_CMD_MASK     (0x7  <<  0)
#define MSDC0_PUPD0_MASK         (0xffffffff << 0)

#define MSDC0_PUPD1_RSTB_MASK    (0x7  <<  12)
#define MSDC0_PUPD1_DAT3_MASK    (0x7  <<   8)
#define MSDC0_PUPD1_DSL_MASK     (0x7  <<   4)
#define MSDC0_PUPD1_DAT7_MASK    (0x7  <<   0)
#define MSDC0_PUPD1_MASK         (0x7fff << 0)

#define MSDC0_PU_10K            (1)
#define MSDC0_PU_50K            (2)
#define MSDC0_PU_8K             (3)
#define MSDC0_PD_10K            (5)
#define MSDC0_PD_50K            (6)
#define MSDC0_PD_8K             (7)

/*
 * MSDC1 GPIO and PAD register and bitfields definition
 */
/* MSDC1_GPIO_MODE4, 0001b is msdc mode */
#define MSDC1_MODE_CMD_MASK     (0x7 << 28)
#define MSDC1_MODE_DAT3_MASK    (0x7 << 24)
#define MSDC1_MODE_CLK_MASK     (0x7 << 20)
/* MSDC1_GPIO_MODE5, 0001b is msdc mode */
#define MSDC1_MODE_DAT1_MASK    (0x7 << 8)
#define MSDC1_MODE_DAT2_MASK    (0x7 << 4)
#define MSDC1_MODE_DAT0_MASK    (0x7 << 0)

/* MSDC1 IES mask*/
#define MSDC1_IES_CMD_MASK      (0x1 <<  6)
#define MSDC1_IES_DAT_MASK      (0x1 <<  5)
#define MSDC1_IES_CLK_MASK      (0x1 <<  4)
#define MSDC1_IES_ALL_MASK      (0x7 <<  4)
/* MSDC1 SMT mask*/
#define MSDC1_SMT_CMD_MASK      (0x1 <<  6)
#define MSDC1_SMT_DAT_MASK      (0x1 <<  5)
#define MSDC1_SMT_CLK_MASK      (0x1 <<  4)
#define MSDC1_SMT_ALL_MASK      (0x7 <<  4)
/* MSDC1 TDSEL mask*/
#define MSDC1_TDSEL0_CMD_MASK   (0xf << 24)
#define MSDC1_TDSEL0_DAT_MASK   (0xf << 20)
#define MSDC1_TDSEL0_CLK_MASK   (0xf << 16)
#define MSDC1_TDSEL0_ALL_MASK   (0xfff << 16)
/* MSDC1 RDSEL mask*/
#define MSDC1_RDSEL0_CMD_MASK   (0x3f << 20)
#define MSDC1_RDSEL0_DAT_MASK   (0x3f << 14)
#define MSDC1_RDSEL0_CLK_MASK   (0x3f << 8)
#define MSDC1_RDSEL0_ALL_MASK   (0x3ffff << 8)
/* MSDC1 SR mask*/
#define MSDC1_SR_CMD_MASK       (0x1 << 27)
#define MSDC1_SR_DAT_MASK       (0x1 << 23)
#define MSDC1_SR_CLK_MASK       (0x1 << 19)
/* MSDC1 DRV mask*/
#define MSDC1_DRV0_CMD_MASK     (0x7 << 24)
#define MSDC1_DRV0_DAT_MASK     (0x7 << 20)
#define MSDC1_DRV0_CLK_MASK     (0x7 << 16)
#define MSDC1_DRV0_ALL_MASK     (0xfff << 16)
/* MSDC1 PUPD mask */
#define MSDC1_PUPD0_DAT1_MASK   (0x7 << 20)
#define MSDC1_PUPD0_DAT2_MASK   (0x7 << 16)
#define MSDC1_PUPD0_DAT0_MASK   (0x7 << 12)
#define MSDC1_PUPD0_CMD_MASK    (0x7 << 8)
#define MSDC1_PUPD0_DAT3_MASK   (0x7 << 4)
#define MSDC1_PUPD0_CLK_MASK    (0x7 << 0)
#define MSDC1_PUPD0_MASK        (0xffffff << 0)

#define MSDC1_PU_10K            (1)
#define MSDC1_PU_50K            (2)
#define MSDC1_PU_8K             (3)
#define MSDC1_PD_10K            (5)
#define MSDC1_PD_50K            (6)
#define MSDC1_PD_8K             (7)

/*
 * MSDC3 GPIO and PAD register and bitfields definition
 */
/* MSDC3_GPIO_MODE7, 0001b is msdc mode */
#define MSDC3_MODE_CMD_MASK     (0x7 << 28)
#define MSDC3_MODE_DSL_MASK     (0x7 << 24)
/* MSDC3_GPIO_MODE8, 0001b is msdc mode */
#define MSDC3_MODE_DAT0_MASK    (0x7 << 16)
#define MSDC3_MODE_DAT1_MASK    (0x7 << 12)
#define MSDC3_MODE_DAT2_MASK    (0x7 << 8)
#define MSDC3_MODE_DAT3_MASK    (0x7 << 4)
#define MSDC3_MODE_CLK_MASK     (0x7 << 0)

/* MSDC3 IES mask*/
#define MSDC3_IES_ALL_MASK      (0x1 <<  3)
/* MSDC3 SMT mask*/
#define MSDC3_SMT_ALL_MASK      (0x1 <<  3)
/* MSDC3 TDSEL mask */
#define MSDC3_TDSEL_ALL_MASK    (0xf << 12)
/* MSDC3 TDSEL mask */
#define MSDC3_RDSEL_ALL_MASK    (0x3f << 6)
/* MSDC3 SR mask */
#define MSDC3_SR_ALL_MASK       (0x1 << 15)
/* MSDC3 DRV mask */
#define MSDC3_DRV_ALL_MASK      (0x7 << 12)
/* MSDC3 PUPD mask */
#define MSDC3_PUPD_DAT0_MASK    (0x7 << 24)
#define MSDC3_PUPD_DAT1_MASK    (0x7 << 20)
#define MSDC3_PUPD_DAT2_MASK    (0x7 << 16)
#define MSDC3_PUPD_DAT3_MASK    (0x7 << 12)
#define MSDC3_PUPD_CLK_MASK     (0x7 <<  8)
#define MSDC3_PUPD_CMD_MASK     (0x7 <<  4)
#define MSDC3_PUPD_DSL_MASK     (0x7 <<  0)
#define MSDC3_PUPD_MASK         (0xFFFFFFF << 0)

/*
   MSDC Driving Strength Definition: specify as gear instead of driving
*/
#define MSDC_DRVN_GEAR0         0x0
#define MSDC_DRVN_GEAR1         0x1
#define MSDC_DRVN_GEAR2         0x2
#define MSDC_DRVN_GEAR3         0x3
#define MSDC_DRVN_GEAR4         0x4
#define MSDC_DRVN_GEAR5         0x5
#define MSDC_DRVN_GEAR6         0x6
#define MSDC_DRVN_GEAR7         0x7
#define MSDC_DRVN_DONT_CARE     0x0

#define MSDC_HW_TRAPPING_ADDR   (MSDC_GPIO_BASE + 0x6E0)
#define MSDC_HW_TRAPPING_MASK   (0x3 << 25) /* 0: Aux2, other:Aux1 */

#ifndef FPGA_PLATFORM
void msdc_set_driving_by_id(u32 id, struct msdc_cust *msdc_cap, bool sd_18);
void msdc_get_driving_by_id(u32 id, struct msdc_cust *msdc_cap, bool sd_18);
void msdc_set_ies_by_id(u32 id, int set_ies);
void msdc_set_sr_by_id(u32 id, int clk, int cmd, int dat, int rst, int ds);
void msdc_set_smt_by_id(u32 id, int set_smt);
void msdc_set_tdsel_by_id(u32 id, u32 flag, u32 value);
void msdc_set_rdsel_by_id(u32 id, u32 flag, u32 value);
void msdc_get_tdsel_by_id(u32 id, u32 *value);
void msdc_get_rdsel_by_id(u32 id, u32 *value);
void msdc_dump_clock_sts(struct mmc_host *host);
void msdc_dump_ldo_sts(struct mmc_host *host);
void msdc_dump_padctl_by_id(u32 id);
void msdc_pin_config_by_id(u32 id, u32 mode);
void msdc_set_pin_mode(struct mmc_host *host);
void msdc_set_tdsel_wrap(struct mmc_host *host);
void msdc_set_rdsel_wrap(struct mmc_host *host);
void msdc_gpio_and_pad_init(struct mmc_host *host);
#endif

#ifdef FPGA_PLATFORM
#define msdc_set_driving_by_id(id, msdc_cap, sd_18)
#define msdc_get_driving_by_id(id, msdc_cap, sd_18)
#define msdc_set_ies_by_id(id, set_ies)
#define msdc_set_sr_by_id(id, clk, cmd, dat, rst, ds)
#define msdc_set_smt_by_id(id, set_smt)
#define msdc_set_tdsel_by_id(id, flag, value)
#define msdc_set_rdsel_by_id(id, flag, value)
#define msdc_get_tdsel_by_id(id, value)
#define msdc_get_rdsel_by_id(id, value)
#define msdc_dump_clock_sts(host)
#define msdc_dump_ldo_sts(host)
#define msdc_dump_padctl_by_id(id)
#define msdc_pin_config_by_id(id, mode)
#define msdc_set_pin_mode(host)
#endif

#define msdc_get_driving(host, msdc_cap, sd_18) \
	msdc_get_driving_by_id(host->id, msdc_cap, sd_18)

#define msdc_set_driving(host, msdc_cap, sd_18) \
	msdc_set_driving_by_id(host->id, msdc_cap, sd_18)

#define msdc_set_ies(host, set_ies) \
	msdc_set_ies_by_id(host->id, set_ies)

#define msdc_set_sr(host, clk, cmd, dat, rst, ds) \
	msdc_set_sr_by_id(host->id, clk, cmd, dat, rst, ds)

#define msdc_set_smt(host, set_smt) \
	msdc_set_smt_by_id(host->id, set_smt)

#define msdc_set_tdsel(host, flag, value) \
	msdc_set_tdsel_by_id(host->id, flag, value)

#define msdc_set_rdsel(host, flag, value) \
	msdc_set_rdsel_by_id(host->id, flag, value)

#define msdc_get_tdsel(host, value) \
	msdc_get_tdsel_by_id(host->id, value)

#define msdc_get_rdsel(host, value) \
	msdc_get_rdsel_by_id(host->id, value)

#define msdc_dump_padctl(host) \
	msdc_dump_padctl_by_id(host->id)

#define msdc_pin_config(host, mode) \
	msdc_pin_config_by_id(host->id, mode)


/*******************************************************************************
 * CLOCK definition
 ******************************************************************************/
#if defined(FPGA_PLATFORM)
#define MSDC_OP_SCLK            (6000000)
#define MSDC_SRC_CLK            (6000000)
#define MSDC_MAX_SCLK           (MSDC_SRC_CLK>>1)
#else
#define MSDC_OP_SCLK            (200000000)
#define MSDC_MAX_SCLK           (200000000)
#endif

#define MSDC_MIN_SCLK           (260000)

/* each PLL have different gears for select
 * software can used mux interface from clock management module to select */
enum {
	MSDC0_CLKSRC_26MHZ  = 0,
	MSDC0_CLKSRC_400MHZ,
	MSDC0_CLKSRC_800MHZ,
	MSDC0_CLKSRC_156MHZ,
	MSDC0_CLKSRC_182MHZ,
	MSDC0_CLKSRC_364MHZ,
	MSDC0_CLKSRC_200MHZ,
	MSDC0_CLKSRC_312MHZ,
	MSDC0_CLKSRC_MAX
};

enum {
	MSDC1_CLKSRC_26MHZ   = 0,
	MSDC1_CLKSRC_208MHZ,
	MSDC1_CLKSRC_200MHZ,
	MSDC1_CLKSRC_156MHZ,
	MSDC1_CLKSRC_182MHZ,
	MSDC1_CLKSRC_NONE,
	MSDC1_CLKSRC_178MHZ,
	MSDC1_CLKSRC_400MHZ,
	MSDC1_CLKSRC_MAX
};

enum {
	MSDC3_CLKSRC_26MHZ   = 0,
	MSDC3_CLKSRC_208MHZ,
	MSDC3_CLKSRC_400MHZ,
	MSDC3_CLKSRC_156MHZ,
	MSDC3_CLKSRC_182MHZ,
	MSDC3_CLKSRC_312MHZ,
	MSDC3_CLKSRC_364MHZ,
	MSDC3_CLKSRC_200MHZ,
	MSDC3_CLKSRC_MAX
};

#define MSDC0_CLKSRC_DEFAULT    MSDC0_CLKSRC_400MHZ
#define MSDC1_CLKSRC_DEFAULT    MSDC1_CLKSRC_200MHZ
#define MSDC3_CLKSRC_DEFAULT    MSDC3_CLKSRC_400MHZ
#define MSDC_CLKSRC_DEFAULT(ID) MSDC##ID_CLKSRC_DEFAULT

#define MSDCPLL_FREQ            800000000
#define MSDC0_E1_CLKSRC_FREQ    364000000
#define MSDC1_E1_CLKSRC_FREQ    182000000

#define MSDC0_SRC_0             260000
#define MSDC0_SRC_1             (MSDCPLL_FREQ/2)
#define MSDC0_SRC_2             MSDCPLL_FREQ
#define MSDC0_SRC_3             156000000
#define MSDC0_SRC_4             182000000
#define MSDC0_SRC_5             364000000
#define MSDC0_SRC_6             (MSDCPLL_FREQ/4)
#define MSDC0_SRC_7             312000000

#define MSDC1_SRC_0             260000
#define MSDC1_SRC_1             208000000
#define MSDC1_SRC_2             (MSDCPLL_FREQ/4)
#define MSDC1_SRC_3             156000000
#define MSDC1_SRC_4             182000000
#define MSDC1_SRC_5             156000000
#define MSDC1_SRC_6             178000000
#define MSDC1_SRC_7             480000000

#define MSDC3_SRC_0             260000
#define MSDC3_SRC_1             208000000
#define MSDC3_SRC_2             (MSDCPLL_FREQ/2)
#define MSDC3_SRC_3             156000000
#define MSDC3_SRC_4             182000000
#define MSDC3_SRC_5             312000000
#define MSDC3_SRC_6             364000000
#define MSDC3_SRC_7             (MSDCPLL_FREQ/4)

void msdc_clock(struct mmc_host *host, int on);
void msdc_config_clksrc(struct mmc_host *host, int clksrc);

#if defined(FPGA_PLATFORM)
#define msdc_get_hclk(host, src)        MSDC_SRC_CLK
#else
extern u32 hclks_msdc0[];
extern u32 hclks_msdc1[];
extern u32 hclks_msdc3[];
extern u32 *hclks_msdc_all[];
#define msdc_get_hclk(id, src)          hclks_msdc_all[id][src]
#endif

#define MSDC0_CLOCK_GATE_REG    (PERICFG_BASE + 0x0290)
#define MSDC1_CLOCK_GATE_REG    (PERICFG_BASE + 0x0290)
#define MSDC3_CLOCK_GATE_REG    (PERICFG_BASE + 0x0290)
#define MSDC0_CLOCK_UNGATE_REG  (PERICFG_BASE + 0x0294)
#define MSDC1_CLOCK_UNGATE_REG  (PERICFG_BASE + 0x0294)
#define MSDC3_CLOCK_UNGATE_REG  (PERICFG_BASE + 0x0294)
#define MSDC0_CLOCK_CG_MD       (1 << 25)
#define MSDC0_CLOCK_CG_SECURE   (1 << 24)
#define MSDC0_CLOCK_CG_NORMAL   (1 << 0)
#define MSDC0_CLOCK_CG          MSDC0_CLOCK_CG_SECURE
#define MSDC1_CLOCK_CG          (1 << 1)
#define MSDC3_CLOCK_CG          (1 << 3)


/*******************************************************************************
 * Power Definition
 ******************************************************************************/
typedef enum MSDC_POWER_VOL_TAG {
	VOL_DEFAULT,
	VOL_0900 = 900,
	VOL_1000 = 1000,
	VOL_1100 = 1100,
	VOL_1200 = 1200,
	VOL_1300 = 1300,
	VOL_1350 = 1350,
	VOL_1500 = 1500,
	VOL_1800 = 1800,
	VOL_2000 = 2000,
	VOL_2100 = 2100,
	VOL_2500 = 2500,
	VOL_2800 = 2800,
	VOL_2900 = 2900,
	VOL_3000 = 3000,
	VOL_3300 = 3300,
	VOL_3400 = 3400,
	VOL_3500 = 3500,
	VOL_3600 = 3600
} MSDC_POWER_VOLTAGE;

#if defined(MMC_MSDC_DRV_CTP) && !defined(FPGA_PLATFORM)
typedef enum MSDC_POWER_TAG {
	MSDC_VMC,
	MSDC_VMCH,
	MSDC_VEMC,
} MSDC_POWER;
#endif

#define REG_VEMC_VOSEL_CAL      PMIC_RG_VEMC_VOCAL_ADDR
#define MASK_VEMC_VOSEL_CAL     PMIC_RG_VEMC_VOCAL_MASK
#define SHIFT_VEMC_VOSEL_CAL    PMIC_RG_VEMC_VOCAL_SHIFT
#define FIELD_VEMC_VOSEL_CAL    (MASK_VEMC_VOSEL_CAL \
					<< SHIFT_VEMC_VOSEL_CAL)

#define REG_VEMC_VOSEL          PMIC_RG_VEMC_VOSEL_ADDR
#define MASK_VEMC_VOSEL         PMIC_RG_VEMC_VOSEL_MASK
#define SHIFT_VEMC_VOSEL        PMIC_RG_VEMC_VOSEL_SHIFT
#define FIELD_VEMC_VOSEL        (MASK_VEMC_VOSEL << SHIFT_VEMC_VOSEL)

#define REG_VEMC_EN             PMIC_RG_LDO_VEMC_EN_ADDR
#define MASK_VEMC_EN            PMIC_RG_LDO_VEMC_EN_MASK
#define SHIFT_VEMC_EN           PMIC_RG_LDO_VEMC_EN_SHIFT
#define FIELD_VEMC_EN           (MASK_VEMC_EN << SHIFT_VEMC_EN)

#define REG_VMC_VOSEL_CAL       PMIC_RG_VMC_VOCAL_ADDR
#define MASK_VMC_VOSEL_CAL      PMIC_RG_VMC_VOCAL_MASK
#define SHIFT_VMC_VOSEL_CAL     PMIC_RG_VMC_VOCAL_SHIFT
#define FIELD_VMC_VOSEL_CAL     (MASK_VMC_VOSEL_CAL \
					<< SHIFT_VMC_VOSEL_CAL)

#define REG_VMC_VOSEL           PMIC_RG_VMC_VOSEL_ADDR
#define MASK_VMC_VOSEL          PMIC_RG_VMC_VOSEL_MASK
#define SHIFT_VMC_VOSEL         PMIC_RG_VMC_VOSEL_SHIFT
#define FIELD_VMC_VOSEL         (MASK_VMC_VOSEL << SHIFT_VMC_VOSEL)

#define REG_VMC_EN              PMIC_RG_LDO_VMC_EN_ADDR
#define MASK_VMC_EN             PMIC_RG_LDO_VMC_EN_MASK
#define SHIFT_VMC_EN            PMIC_RG_LDO_VMC_EN_SHIFT
#define FIELD_VMC_EN            (MASK_VMC_EN << SHIFT_VMC_EN)

#define REG_VMCH_VOSEL_CAL      PMIC_RG_VMCH_VOCAL_ADDR
#define MASK_VMCH_VOSEL_CAL     PMIC_RG_VMCH_VOCAL_MASK
#define SHIFT_VMCH_VOSEL_CAL    PMIC_RG_VMCH_VOCAL_SHIFT
#define FIELD_VMCH_VOSEL_CAL    (MASK_VMCH_VOSEL_CAL \
					<< SHIFT_VMCH_VOSEL_CAL)

#define REG_VMCH_VOSEL          PMIC_RG_VMCH_VOSEL_ADDR
#define MASK_VMCH_VOSEL         PMIC_RG_VMCH_VOSEL_MASK
#define SHIFT_VMCH_VOSEL        PMIC_RG_VMCH_VOSEL_SHIFT
#define FIELD_VMCH_VOSEL        (MASK_VMCH_VOSEL << SHIFT_VMCH_VOSEL)

#define REG_VMCH_EN             PMIC_RG_LDO_VMCH_EN_ADDR
#define MASK_VMCH_EN            PMIC_RG_LDO_VMCH_EN_MASK
#define SHIFT_VMCH_EN           PMIC_RG_LDO_VMCH_EN_SHIFT
#define FIELD_VMCH_EN           (MASK_VMCH_EN << SHIFT_VMCH_EN)

#define VEMC_VOSEL_CAL_mV(cal)  ((cal <= 0) ? (0) : (cal/10))
#define VEMC_VOSEL_2V9          (0xa)
#define VEMC_VOSEL_3V           (0xb)
#define VEMC_VOSEL_3V3          (0xd)
#define VMC_VOSEL_CAL_mV(cal)   ((cal <= 0) ? (0) : (cal/10))
#define VMC_VOSEL_1V8           (0x4)
#define VMC_VOSEL_2V9           (0xa)
#define VMC_VOSEL_3V            (0xb)
#define VMC_VOSEL_3V3           (0xd)
#define VMCH_VOSEL_CAL_mV(cal)  ((cal <= 0) ? (0) : (cal/10))
#define VMCH_VOSEL_2V9          (0xa)
#define VMCH_VOSEL_3V           (0xb)
#define VMCH_VOSEL_3V3          (0xd)

#define EMMC_VOL_ACTUAL         VOL_3000
#define SD_VOL_ACTUAL           VOL_3000

#endif /* end of _MSDC_IO_H_ */

