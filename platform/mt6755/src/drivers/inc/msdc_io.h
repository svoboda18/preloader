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
#if defined(PMIC_CHIP_MT6353)
#include "mt6353_hw.h"
#define MTK_PMIC_CHIP_MT6353
#else
#include "upmu_hw.h"
#endif
#endif

#if defined(MMC_MSDC_DRV_LK)
#include "mt_reg_base.h"
#if defined(PMIC_CHIP_MT6353)
#include "mt6353_hw.h"
#define MTK_PMIC_CHIP_MT6353
#else
#include "upmu_hw.h"
#endif
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

/*--------------------------------------------------------------------------*/
/* MSDC0~3 GPIO and IO Pad Configuration Base                               */
/*--------------------------------------------------------------------------*/
#define GPIO_base               0x10005000
#ifndef GPIO_BASE
#define GPIO_BASE               GPIO_base
#endif
#define MSDC_GPIO_BASE          GPIO_BASE

#define IO_0_CFG_BASE           (0x10002000)    /* IO Config group0 base   */
#define IO_1_CFG_BASE           (0x10002200)    /* IO Config group1 base   */
#define IO_2_CFG_BASE           (0x10002400)    /* IO Config group2 base   */
#define IO_3_CFG_BASE           (0x10002600)    /* IO Config group3 base   */
#define IO_4_CFG_BASE           (0x10002800)    /* IO Config group4 base   */
#define IO_5_CFG_BASE           (0x10002A00)    /* IO Config group5 base   */

#define MSDC0_IO_PAD_BASE       (IO_5_CFG_BASE)
#define MSDC1_IO_PAD_BASE       (IO_0_CFG_BASE)
#define MSDC2_IO_PAD_BASE       (IO_4_CFG_BASE)

/*--------------------------------------------------------------------------*/
/* MSDC GPIO Related Register                                               */
/*--------------------------------------------------------------------------*/
#define MSDC0_GPIO_MODE18       (MSDC_GPIO_BASE + 0x410)
#define MSDC0_GPIO_MODE19       (MSDC_GPIO_BASE + 0x420)
#define MSDC0_GPIO_IES_ADDR     (MSDC0_IO_PAD_BASE + 0x000)
#define MSDC0_GPIO_SMT_ADDR     (MSDC0_IO_PAD_BASE + 0x010)
#define MSDC0_GPIO_TDSEL_ADDR   (MSDC0_IO_PAD_BASE + 0x020)
#define MSDC0_GPIO_RDSEL_ADDR   (MSDC0_IO_PAD_BASE + 0x040)
#define MSDC0_GPIO_SR_ADDR      (MSDC0_IO_PAD_BASE + 0x0a0)
#define MSDC0_GPIO_DRV_ADDR     (MSDC0_IO_PAD_BASE + 0x0a0)
#define MSDC0_GPIO_PUPD0_ADDR   (MSDC0_IO_PAD_BASE + 0x0c0)
#define MSDC0_GPIO_PUPD1_ADDR   (MSDC0_IO_PAD_BASE + 0x0d0)

#define MSDC1_GPIO_MODE4        (MSDC_GPIO_BASE + 0x330)
#define MSDC1_GPIO_IES_ADDR     (MSDC1_IO_PAD_BASE + 0x000)
#define MSDC1_GPIO_SMT_ADDR     (MSDC1_IO_PAD_BASE + 0x010)
#define MSDC1_GPIO_TDSEL_ADDR   (MSDC1_IO_PAD_BASE + 0x030)
#define MSDC1_GPIO_RDSEL0_ADDR  (MSDC1_IO_PAD_BASE + 0x040)
#define MSDC1_GPIO_RDSEL1_ADDR  (MSDC1_IO_PAD_BASE + 0x050)
#define MSDC1_GPIO_SR_ADDR      (MSDC1_IO_PAD_BASE + 0x0b0)
#define MSDC1_GPIO_DRV_ADDR     (MSDC1_IO_PAD_BASE + 0x0b0)
#define MSDC1_GPIO_PUPD_ADDR    (MSDC1_IO_PAD_BASE + 0x0c0)

#define MSDC2_GPIO_MODE14       (MSDC_GPIO_BASE + 0x3D0)
#define MSDC2_GPIO_IES_ADDR     (MSDC2_IO_PAD_BASE + 0x000)
#define MSDC2_GPIO_SMT_ADDR     (MSDC2_IO_PAD_BASE + 0x010)
#define MSDC2_GPIO_TDSEL_ADDR   (MSDC2_IO_PAD_BASE + 0x020)
#define MSDC2_GPIO_RDSEL_ADDR   (MSDC2_IO_PAD_BASE + 0x040)
#define MSDC2_GPIO_SR_ADDR      (MSDC2_IO_PAD_BASE + 0x0a0)
#define MSDC2_GPIO_DRV_ADDR     (MSDC2_IO_PAD_BASE + 0x0a0)
#define MSDC2_GPIO_PUPD_ADDR    (MSDC2_IO_PAD_BASE + 0x0c0)

/*
 * MSDC0 GPIO and PAD register and bitfields definition
 */
/*MSDC0_GPIO_MODE19, Set as pinmux value as 1*/
#define MSDC0_MODE_DAT7_MASK            (7 << 25)
#define MSDC0_MODE_DAT6_MASK            (7 <<  9)
#define MSDC0_MODE_DAT5_MASK            (7 <<  6)
#define MSDC0_MODE_DAT4_MASK            (7 << 12)
#define MSDC0_MODE_DAT3_MASK            (7 << 22)
#define MSDC0_MODE_DAT2_MASK            (7 << 19)
#define MSDC0_MODE_DAT1_MASK            (7 <<  3)
#define MSDC0_MODE_CMD_MASK             (7 <<  0)
#define MSDC0_MODE_DSL_MASK             (7 << 28)
#define MSDC0_MODE_RST_MASK             (7 << 16)

/*MSDC0_GPIO_MODE18, Set as pinmux value as 1*/
#define MSDC0_MODE_DAT0_MASK            (7 << 25)
#define MSDC0_MODE_CLK_MASK             (7 << 28)

/* MSDC0 IES mask*/
#define MSDC0_IES_DAT_MASK              (0x1  <<  0)
#define MSDC0_IES_CLK_MASK              (0x1  <<  1)
#define MSDC0_IES_CMD_MASK              (0x1  <<  2)
#define MSDC0_IES_RSTB_MASK             (0x1  <<  3)
#define MSDC0_IES_DSL_MASK              (0x1  <<  4)
#define MSDC0_IES_ALL_MASK              (0x1F <<  0)

/* MSDC0 SMT mask*/
#define MSDC0_SMT_DAT_MASK              (0x1  <<  0)
#define MSDC0_SMT_CLK_MASK              (0x1  <<  1)
#define MSDC0_SMT_CMD_MASK              (0x1  <<  2)
#define MSDC0_SMT_RSTB_MASK             (0x1  <<  3)
#define MSDC0_SMT_DSL_MASK              (0x1  <<  4)
#define MSDC0_SMT_ALL_MASK              (0x1F <<  0)

/* MSDC0 TDSEL mask*/
#define MSDC0_TDSEL_DAT_MASK            (0xF  <<  0)
#define MSDC0_TDSEL_CLK_MASK            (0xF  <<  4)
#define MSDC0_TDSEL_CMD_MASK            (0xF  <<  8)
#define MSDC0_TDSEL_RSTB_MASK           (0xF  << 12)
#define MSDC0_TDSEL_DSL_MASK            (0xF  << 16)
#define MSDC0_TDSEL_ALL_MASK            (0xFFFFF << 0)

/* MSDC0 RDSEL mask*/
#define MSDC0_RDSEL_DAT_MASK            (0x3F <<  0)
#define MSDC0_RDSEL_CLK_MASK            (0x3F <<  6)
#define MSDC0_RDSEL_CMD_MASK            (0x3F << 12)
#define MSDC0_RDSEL_RSTB_MASK           (0x3F << 18)
#define MSDC0_RDSEL_DSL_MASK            (0x3F << 24)
#define MSDC0_RDSEL_ALL_MASK            (0x3FFFFFFF << 0)

/* MSDC0 SR mask*/
#define MSDC0_SR_DAT_MASK               (0x1  <<  3)
#define MSDC0_SR_CLK_MASK               (0x1  <<  7)
#define MSDC0_SR_CMD_MASK               (0x1  << 11)
#define MSDC0_SR_RSTB_MASK              (0x1  << 15)
#define MSDC0_SR_DSL_MASK               (0x1  << 19)
/*Attention: bits are not continuous, shall not define MSDC0_SR_ALL_MASK*/

/* MSDC0 DRV mask*/
#define MSDC0_DRV_DAT_MASK              (0x7  <<  0)
#define MSDC0_DRV_CLK_MASK              (0x7  <<  4)
#define MSDC0_DRV_CMD_MASK              (0x7  <<  8)
#define MSDC0_DRV_RSTB_MASK             (0x7  << 12)
#define MSDC0_DRV_DSL_MASK              (0x7  << 16)
/*Attention: bits are not continuous, shall not define MSDC0_DRV_ALL_MASK*/

/* MSDC0 PUPD mask*/
#define MSDC0_PUPD_DAT0_MASK            (0x7  <<  0)
#define MSDC0_PUPD_CLK_MASK             (0x7  <<  4)
#define MSDC0_PUPD_CMD_MASK             (0x7  <<  8)
#define MSDC0_PUPD_DAT1_MASK            (0x7  << 12)
#define MSDC0_PUPD_DAT5_MASK            (0x7  << 16)
#define MSDC0_PUPD_DAT6_MASK            (0x7  << 20)
#define MSDC0_PUPD_DAT4_MASK            (0x7  << 24)
#define MSDC0_PUPD_RSTB_MASK            (0x7  << 28)
#define MSDC0_PUPD0_MASK_WITH_RSTB      (0x77777777 << 0)
#define MSDC0_PUPD0_MASK                (0x7777777 << 0)

#define MSDC0_PUPD_DAT2_MASK            (0x7  <<  0)
#define MSDC0_PUPD_DAT3_MASK            (0x7  <<  4)
#define MSDC0_PUPD_DAT7_MASK            (0x7  <<  8)
#define MSDC0_PUPD_DSL_MASK             (0x7  << 12)
#define MSDC0_PUPD1_MASK                (0x7777 << 0)

/*
 * MSDC1 GPIO and PAD register and bitfields definition
 */
/*MSDC0_GPIO_MODE4, Set as pinmux value as 1*/
#define MSDC1_MODE_CLK_MASK             (7 <<  0)
#define MSDC1_MODE_CMD_MASK             (7 <<  6)
#define MSDC1_MODE_DAT0_MASK            (7 <<  9)
#define MSDC1_MODE_DAT1_MASK            (7 << 16)
#define MSDC1_MODE_DAT2_MASK            (7 << 12)
#define MSDC1_MODE_DAT3_MASK            (7 <<  3)

/* MSDC1 IES mask*/
#define MSDC1_IES_CLK_MASK              (0x1 <<  8)
#define MSDC1_IES_CMD_MASK              (0x1 <<  9)
#define MSDC1_IES_DAT_MASK              (0x1 << 10)
#define MSDC1_IES_ALL_MASK              (0x7 <<  8)

/* MSDC1 SMT mask*/
#define MSDC1_SMT_CLK_MASK              (0x1 <<  8)
#define MSDC1_SMT_CMD_MASK              (0x1 <<  9)
#define MSDC1_SMT_DAT_MASK              (0x1 << 10)
#define MSDC1_SMT_ALL_MASK              (0x7 <<  8)

/* MSDC1 TDSEL mask*/
#define MSDC1_TDSEL_CLK_MASK            (0xF <<  0)
#define MSDC1_TDSEL_CMD_MASK            (0xF <<  4)
#define MSDC1_TDSEL_DAT_MASK            (0xF <<  8)
#define MSDC1_TDSEL_ALL_MASK            (0xFFF << 0)

/* MSDC1 RDSEL mask*/
/*MSDC1_GPIO_RDSEL0*/
#define MSDC1_RDSEL_CLK_MASK            (0x3F << 16)
#define MSDC1_RDSEL_CMD_MASK            (0x3F << 22)
/*MSDC1_GPIO_RDSEL1*/
#define MSDC1_RDSEL_DAT_MASK            (0x3F <<  0)
#define MSDC1_RDSEL0_ALL_MASK           (0xFFF << 16)
#define MSDC1_RDSEL1_ALL_MASK           (0x3F <<  0)
/*Attention: not the same address, shall not define MSDC1_RDSEL_ALL_MASK*/

/* MSDC1 SR mask*/
#define MSDC1_SR_CLK_MASK               (0x1 <<  3)
#define MSDC1_SR_CMD_MASK               (0x1 <<  7)
#define MSDC1_SR_DAT_MASK               (0x1 << 11)
/*Attention: bits are not continuous, shall not define MSDC1_SR_ALL_MASK*/

/* MSDC1 DRV mask*/
#define MSDC1_DRV_CLK_MASK              (0x7 <<  0)
#define MSDC1_DRV_CMD_MASK              (0x7 <<  4)
#define MSDC1_DRV_DAT_MASK              (0x7 <<  8)
/*Attention: bits are not continuous, shall not define MSDC1_DRV_ALL_MASK*/

/* MSDC1 PUPD mask*/
#define MSDC1_PUPD_CMD_MASK             (0x7  <<  0)
#define MSDC1_PUPD_CLK_MASK             (0x7  <<  4)
#define MSDC1_PUPD_DAT0_MASK            (0x7  <<  8)
#define MSDC1_PUPD_DAT1_MASK            (0x7  << 12)
#define MSDC1_PUPD_DAT2_MASK            (0x7  << 16)
#define MSDC1_PUPD_DAT3_MASK            (0x7  << 20)
#define MSDC1_PUPD_MASK                 (0x777777 << 0)

/*
 * MSDC2 GPIO and PAD register and bitfields definition
 */
/*MSDC2_GPIO_MODE14, Set as pinmux value as 2*/
#define MSDC2_MODE_CLK_MASK             (7 <<  9)
#define MSDC2_MODE_CMD_MASK             (7 <<  6)
#define MSDC2_MODE_DAT0_MASK            (7 << 16)
#define MSDC2_MODE_DAT1_MASK            (7 <<  3)
#define MSDC2_MODE_DAT2_MASK            (7 << 19)
#define MSDC2_MODE_DAT3_MASK            (7 << 12)

/* MSDC2 IES mask*/
#define MSDC2_IES_CLK_MASK              (0x1 << 5)
#define MSDC2_IES_CMD_MASK              (0x1 << 4)
#define MSDC2_IES_DAT_MASK              (0x1 << 3)
#define MSDC2_IES_ALL_MASK              (0x7 << 3)

/* MSDC2 SMT mask*/
#define MSDC2_SMT_CLK_MASK              (0x1 << 5)
#define MSDC2_SMT_CMD_MASK              (0x1 << 4)
#define MSDC2_SMT_DAT_MASK              (0x1 << 3)
#define MSDC2_SMT_ALL_MASK              (0x7 << 3)

/* MSDC2 TDSEL mask*/
#define MSDC2_TDSEL_CLK_MASK            (0xF << 20)
#define MSDC2_TDSEL_CMD_MASK            (0xF << 16)
#define MSDC2_TDSEL_DAT_MASK            (0xF << 12)
#define MSDC2_TDSEL_ALL_MASK            (0xFFF << 12)

/* MSDC2 RDSEL mask*/
#define MSDC2_RDSEL_CLK_MASK            (0x3F << 22)
#define MSDC2_RDSEL_CMD_MASK            (0x3F << 16)
#define MSDC2_RDSEL_DAT_MASK            (0x3F << 10)
#define MSDC2_RDSEL_ALL_MASK            (0x3FFFF << 10)

/* MSDC2 SR mask*/
#define MSDC2_SR_CLK_MASK               (0x1 << 23)
#define MSDC2_SR_CMD_MASK               (0x1 << 19)
#define MSDC2_SR_DAT_MASK               (0x1 << 15)

/* MSDC2 DRV mask*/
#define MSDC2_DRV_CLK_MASK              (0x7 << 20)
#define MSDC2_DRV_CMD_MASK              (0x7 << 16)
#define MSDC2_DRV_DAT_MASK              (0x7 << 12)

/* MSDC2 PUPD mask*/
#define MSDC2_PUPD_CLK_MASK             (0x7 << 20)
#define MSDC2_PUPD_CMD_MASK             (0x7 << 16)
#define MSDC2_PUPD_DAT_MASK             (0x7 << 12)
#define MSDC2_PUPD_MASK                 (0x777 << 0)

/*
   MSDC Driving Strength Definition: specify as gear instead of driving
*/
#define MSDC_DRVN_GEAR0                 0x0
#define MSDC_DRVN_GEAR1                 0x1
#define MSDC_DRVN_GEAR2                 0x2
#define MSDC_DRVN_GEAR3                 0x3
#define MSDC_DRVN_GEAR4                 0x4
#define MSDC_DRVN_GEAR5                 0x5
#define MSDC_DRVN_GEAR6                 0x6
#define MSDC_DRVN_GEAR7                 0x7
#define MSDC_DRVN_DONT_CARE             0x0

#ifndef FPGA_PLATFORM
void msdc_set_driving_by_id(u32 id, struct msdc_cust *msdc_cap, bool sd_18);
void msdc_get_driving_by_id(u32 id, struct msdc_cust *msdc_cap, bool sd_18);
void msdc_set_ies_by_id(u32 id, int set_ies);
void msdc_set_sr_by_id(u32 id, int clk, int cmd, int dat, int rst, int ds);
void msdc_set_smt_by_id(u32 id, int set_smt);
void msdc_set_tdsel_by_id(u32 id, bool sleep, bool sd_18);
void msdc_set_rdsel_by_id(u32 id, bool sleep, bool sd_18);
void msdc_dump_padctl_by_id(u32 id);
void msdc_pin_config_by_id(u32 id, u32 mode);
void msdc_set_pin_mode(struct mmc_host *host);
#endif

#ifdef FPGA_PLATFORM
#define msdc_set_driving_by_id(id, msdc_cap, sd_18)
#define msdc_get_driving_by_id(id, msdc_cap, sd_18)
#define msdc_set_ies_by_id(id, set_ies)
#define msdc_set_sr_by_id(id, clk, cmd, dat, rst, ds)
#define msdc_set_smt_by_id(id, set_smt)
#define msdc_set_tdsel_by_id(id, sleep, sd_18)
#define msdc_set_rdsel_by_id(id, sleep, sd_18)
#define msdc_dump_padctl_by_id(id)
#define msdc_pin_config_by_id(id, mode)
#define msdc_set_pin_mode(host)
#endif

void msdc_gpio_and_pad_init(struct mmc_host *host);

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

#define msdc_set_tdsel(host, sleep, sd_18) \
    msdc_set_tdsel_by_id(host->id, sleep, sd_18)

#define msdc_set_rdsel(host, sleep, sd_18) \
    msdc_set_rdsel_by_id(host->id, sleep, sd_18)

#define msdc_dump_padctl(host) \
    msdc_dump_padctl_by_id(host->id)

#define msdc_pin_config(host, mode) \
    msdc_pin_config_by_id(host->id, mode)


/*******************************************************************************
 * CLOCK definition
 ******************************************************************************/
#if defined(FPGA_PLATFORM)
#define MSDC_OP_SCLK            (12000000)
#define MSDC_SRC_CLK            (12000000)
#define MSDC_MAX_SCLK           (MSDC_SRC_CLK>>1)
#else
#define MSDC_OP_SCLK            (200000000)
#define MSDC_MAX_SCLK           (200000000)
#endif

#define MSDC_MIN_SCLK           (260000)

/* each PLL have different gears for select
 * software can used mux interface from clock management module to select */
enum {
	MSDC50_CLKSRC_26MHZ  = 0,
	MSDC50_CLKSRC_400MHZ,
	MSDC50_CLKSRC_200MHZ,
	MSDC50_CLKSRC_156MHZ,
	MSDC50_CLKSRC_182MHZ,
	MSDC50_CLKSRC_NONE,
	MSDC50_CLKSRC_100MHZ,
	MSDC50_CLKSRC_624MHZ,
	MSDC50_CLKSRC_312MHZ,
	MSDC50_CLKSRC_MAX
};

enum {
	MSDC30_CLKSRC_26MHZ   = 0,
	MSDC30_CLKSRC_208MHZ,
	MSDC30_CLKSRC_100MHZ,
	MSDC30_CLKSRC_156MHZ,
	MSDC30_CLKSRC_182MHZ,
	MSDC30_CLKSRC_NONE,
	MSDC30_CLKSRC_178MHZ,
	MSDC30_CLKSRC_200MHZ,
	MSDC30_CLKSRC_MAX
};

#define MSDC50_CLKSRC_DEFAULT   MSDC50_CLKSRC_400MHZ
#define MSDC30_CLKSRC_DEFAULT   MSDC30_CLKSRC_200MHZ
#define MSDC_CLKSRC_DEFAULT(ID) ((id == 0) ? MSDC50_CLKSRC_DEFAULT : MSDC30_CLKSRC_200MHZ)

void msdc_clock(struct mmc_host *host, int on);
void msdc_config_clksrc(struct mmc_host *host, u8 clksrc);

extern u32 hclks_msdc50[];
extern u32 hclks_msdc30[];
#if !defined(FPGA_PLATFORM)
extern u32 *msdc_src_clks;
#else
extern u32 msdc_src_clks[];
#endif

#define MSDC0_CLOCK_GATE_REG    (INFRACFG_AO_BASE + 0x0088)
#define MSDC1_CLOCK_GATE_REG    (INFRACFG_AO_BASE + 0x0088)
#define MSDC2_CLOCK_GATE_REG    (INFRACFG_AO_BASE + 0x0088)
#define MSDC3_CLOCK_GATE_REG    (INFRACFG_AO_BASE + 0x0088)
#define MSDC0_CLOCK_UNGATE_REG  (INFRACFG_AO_BASE + 0x008C)
#define MSDC1_CLOCK_UNGATE_REG  (INFRACFG_AO_BASE + 0x008C)
#define MSDC2_CLOCK_UNGATE_REG  (INFRACFG_AO_BASE + 0x008C)
#define MSDC3_CLOCK_UNGATE_REG  (INFRACFG_AO_BASE + 0x008C)
#define MSDC0_CLOCK_CG      (1 << 2)
#define MSDC1_CLOCK_CG      (1 << 4)
#define MSDC2_CLOCK_CG      (1 << 5)
#define MSDC3_CLOCK_CG      (1 << 6)


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

#if defined(MTK_PMIC_CHIP_MT6353)
#define REG_VEMC_VOSEL_CAL              PMIC_RG_VEMC33_CAL_ADDR
#define REG_VEMC_VOSEL                  PMIC_RG_VEMC33_VOSEL_ADDR
#define REG_VEMC_EN                     PMIC_LDO_VEMC33_EN_ADDR
#define REG_VEMC_DL_EN                  PMIC_RG_VEMC33_DL_EN_ADDR
#define REG_VEMC_CL_EN                  PMIC_RG_VEMC33_CL_EN_ADDR

#define REG_VMC_VOSEL_CAL               PMIC_RG_VMC_CAL_ADDR
#define REG_VMC_VOSEL                   PMIC_RG_VMC_VOSEL_ADDR
#define REG_VMC_EN                      PMIC_LDO_VMC_EN_ADDR
#define REG_VMC_DL_EN                   PMIC_RG_VMC_DL_EN_ADDR
#define REG_VMC_CL_EN                   PMIC_RG_VMC_CL_EN_ADDR
#define REG_VMCH_VOSEL_CAL              PMIC_RG_VMCH_CAL_ADDR
#define REG_VMCH_VOSEL                  PMIC_RG_VMCH_VOSEL_ADDR
#define REG_VMCH_EN                     PMIC_LDO_VMCH_EN_ADDR
#define REG_VMCH_DL_EN                  PMIC_RG_VMCH_DL_EN_ADDR
#define REG_VMCH_CL_EN                  PMIC_RG_VMCH_CL_EN_ADDR
#define REG_VMCH_OC_STATUS              PMIC_OC_STATUS_VMCH_ADDR
#define REG_VMCH_RAMPUP_SEL             PMIC_LDO_VMCH_STBTD_ADDR

#define MASK_VEMC_VOSEL_CAL             PMIC_RG_VEMC33_CAL_MASK
#define SHIFT_VEMC_VOSEL_CAL            PMIC_RG_VEMC33_CAL_SHIFT
#define FIELD_VEMC_VOSEL_CAL            (MASK_VEMC_VOSEL_CAL \
                        << SHIFT_VEMC_VOSEL_CAL)
#define MASK_VEMC_VOSEL                 PMIC_RG_VEMC33_VOSEL_MASK
#define SHIFT_VEMC_VOSEL                PMIC_RG_VEMC33_VOSEL_SHIFT
#define FIELD_VEMC_VOSEL                (MASK_VEMC_VOSEL << SHIFT_VEMC_VOSEL)
#define MASK_VEMC_EN                    PMIC_LDO_VEMC33_EN_MASK
#define SHIFT_VEMC_EN                   PMIC_LDO_VEMC33_EN_SHIFT
#define FIELD_VEMC_EN                   (MASK_VEMC_EN << SHIFT_VEMC_EN)
#define MASK_VEMC_DL_EN                 PMIC_RG_VEMC33_DL_EN_MASK
#define SHIFT_VEMC_DL_EN                PMIC_RG_VEMC33_DL_EN_SHIFT
#define FIELD_VEMC_DL_EN                (MASK_VEMC_DL_EN << SHIFT_VEMC_DL_EN)
#define MASK_VEMC_CL_EN                 PMIC_RG_VEMC33_CL_EN_MASK
#define SHIFT_VEMC_CL_EN                PMIC_RG_VEMC33_CL_EN_SHIFT
#define FIELD_VEMC_CL_EN                (MASK_VEMC_CL_EN << SHIFT_VEMC_CL_EN)

#define MASK_VMC_VOSEL_CAL              PMIC_RG_VMC_CAL_MASK
#define SHIFT_VMC_VOSEL_CAL             PMIC_RG_VMC_CAL_SHIFT
#define FIELD_VMC_VOSEL_CAL             (MASK_VMC_VOSEL_CAL \
                        << SHIFT_VMC_VOSEL_CAL)
#define MASK_VMC_VOSEL                  PMIC_RG_VMC_VOSEL_MASK
#define SHIFT_VMC_VOSEL                 PMIC_RG_VMC_VOSEL_SHIFT
#define FIELD_VMC_VOSEL                 (MASK_VMC_VOSEL << SHIFT_VMC_VOSEL)
#define MASK_VMC_EN                     PMIC_LDO_VMC_EN_MASK
#define SHIFT_VMC_EN                    PMIC_LDO_VMC_EN_SHIFT
#define FIELD_VMC_EN                    (MASK_VMC_EN << SHIFT_VMC_EN)
#define MASK_VMC_DL_EN                  PMIC_RG_VMC_DL_EN_MASK
#define SHIFT_VMC_DL_EN                 PMIC_RG_VMC_DL_EN_SHIFT
#define FIELD_VMC_DL_EN                 (MASK_VMC_DL_EN << SHIFT_VMC_DL_EN)
#define MASK_VMC_CL_EN                  PMIC_RG_VMC_CL_EN_MASK
#define SHIFT_VMC_CL_EN                 PMIC_RG_VMC_CL_EN_SHIFT
#define FIELD_VMC_CL_EN                 (MASK_VMC_CL_EN << SHIFT_VMC_CL_EN)

#define MASK_VMCH_VOSEL_CAL             PMIC_RG_VMCH_CAL_MASK
#define SHIFT_VMCH_VOSEL_CAL            PMIC_RG_VMCH_CAL_SHIFT
#define FIELD_VMCH_VOSEL_CAL            (MASK_VMCH_VOSEL_CAL \
                        << SHIFT_VMCH_VOSEL_CAL)
#define MASK_VMCH_VOSEL                 PMIC_RG_VMCH_VOSEL_MASK
#define SHIFT_VMCH_VOSEL                PMIC_RG_VMCH_VOSEL_SHIFT
#define FIELD_VMCH_VOSEL                (MASK_VMCH_VOSEL << SHIFT_VMCH_VOSEL)
#define MASK_VMCH_EN                    PMIC_LDO_VMCH_EN_MASK
#define SHIFT_VMCH_EN                   PMIC_LDO_VMCH_EN_SHIFT
#define FIELD_VMCH_EN                   (MASK_VMCH_EN << SHIFT_VMCH_EN)
#define MASK_VMCH_DL_EN                 PMIC_RG_VMCH_DL_EN_MASK
#define SHIFT_VMCH_DL_EN                PMIC_RG_VMCH_DL_EN_SHIFT
#define FIELD_VMCH_DL_EN                (MASK_VMCH_DL_EN << SHIFT_VMCH_DL_EN)
#define MASK_VMCH_CL_EN                 PMIC_RG_VMCH_CL_EN_MASK
#define SHIFT_VMCH_CL_EN                PMIC_RG_VMCH_CL_EN_SHIFT
#define FIELD_VMCH_CL_EN                (MASK_VMCH_CL_EN << SHIFT_VMCH_CL_EN)
#define MASK_VMCH_OC_STATUS             PMIC_OC_STATUS_VMCH_MASK
#define SHIFT_VMCH_OC_STATUS            PMIC_OC_STATUS_VMCH_SHIFT
#define FIELD_VMCH_OC_STATUS            (MASK_VMCH_OC_STATUS \
                        << SHIFT_VMCH_OC_STATUS)
#define MASK_VMCH_RAMPUP_SEL            PMIC_LDO_VMCH_STBTD_MASK
#define SHIFT_VMCH_RAMPUP_SEL           PMIC_LDO_VMCH_STBTD_SHIFT
#define FIELD_VMCH_RAMPUP_SEL           (PMIC_LDO_VMCH_STBTD_MASK \
                        << PMIC_LDO_VMCH_STBTD_SHIFT)

#define VEMC_VOSEL_CAL_mV(cal)          ((cal <= 0) ? \
                        ((0-(cal))/20) : (16-(cal)/20))
#define VEMC_VOSEL_2V9                  (0)
#define VEMC_VOSEL_3V                   (1)
#define VEMC_VOSEL_3V3                  (2)
#define VMC_VOSEL_1V8                   (3)
#define VMC_VOSEL_2V9                   (5)
#define VMC_VOSEL_3V                    (6)
#define VMC_VOSEL_3V3                   (7)
#define VMC_VOSEL_CAL_mV(cal)           ((cal <= 0) ? \
                        ((0-(cal))/20) : (16-(cal)/20))
#define VMCH_VOSEL_CAL_mV(cal)          ((cal <= 0) ? \
                        ((0-(cal))/20) : (16-(cal)/20))
#define VMCH_VOSEL_2V9                  (0)
#define VMCH_VOSEL_3V                   (1)
#define VMCH_VOSEL_3V3                  (2)

#define REG_VCORE_VOSEL_SW              PMIC_BUCK_VCORE_VOSEL_ADDR
#define VCORE_VOSEL_SW_MASK             PMIC_BUCK_VCORE_VOSEL_MASK
#define VCORE_VOSEL_SW_SHIFT            PMIC_BUCK_VCORE_VOSEL_SHIFT
#define REG_VCORE_VOSEL_HW              PMIC_BUCK_VCORE_VOSEL_ON_ADDR
#define VCORE_VOSEL_HW_MASK             PMIC_BUCK_VCORE_VOSEL_ON_MASK
#define VCORE_VOSEL_HW_SHIFT            PMIC_BUCK_VCORE_VOSEL_ON_SHIFT

#define REG_VIO18_CAL                   PMIC_RG_VIO18_CAL_ADDR
#define VIO18_CAL_MASK                  PMIC_RG_VIO18_CAL_MASK
#define VIO18_CAL_SHIFT                 PMIC_RG_VIO18_CAL_SHIFT

#define EMMC_VOL_ACTUAL                 VOL_3000
#define SD_VOL_ACTUAL                   VOL_3000

#else
#define REG_VEMC_VOSEL_CAL              MT6351_PMIC_RG_VEMC_CAL_ADDR
#define REG_VEMC_VOSEL                  MT6351_PMIC_RG_VEMC_VOSEL_ADDR
#define REG_VEMC_EN                     MT6351_PMIC_RG_VEMC_EN_ADDR
#define REG_VMC_VOSEL                   MT6351_PMIC_RG_VMC_VOSEL_ADDR
#define REG_VMC_EN                      MT6351_PMIC_RG_VMC_EN_ADDR
#define REG_VMCH_VOSEL_CAL              MT6351_PMIC_RG_VMCH_CAL_ADDR
#define REG_VMCH_VOSEL                  MT6351_PMIC_RG_VMCH_VOSEL_ADDR
#define REG_VMCH_EN                     MT6351_PMIC_RG_VMCH_EN_ADDR

#define MASK_VEMC_VOSEL_CAL             MT6351_PMIC_RG_VEMC_CAL_MASK
#define SHIFT_VEMC_VOSEL_CAL            MT6351_PMIC_RG_VEMC_CAL_SHIFT
#define FIELD_VEMC_VOSEL_CAL            (MASK_VEMC_VOSEL_CAL << \
                        SHIFT_VEMC_VOSEL_CAL)
#define MASK_VEMC_VOSEL                 MT6351_PMIC_RG_VEMC_VOSEL_MASK
#define SHIFT_VEMC_VOSEL                MT6351_PMIC_RG_VEMC_VOSEL_SHIFT
#define FIELD_VEMC_VOSEL                (MASK_VEMC_VOSEL << SHIFT_VEMC_VOSEL)
#define MASK_VEMC_EN                    MT6351_PMIC_RG_VEMC_EN_MASK
#define SHIFT_VEMC_EN                   MT6351_PMIC_RG_VEMC_EN_SHIFT
#define FIELD_VEMC_EN                   (MASK_VEMC_EN << SHIFT_VEMC_EN)
#define MASK_VMC_VOSEL                  MT6351_PMIC_RG_VMC_VOSEL_MASK
#define SHIFT_VMC_VOSEL                 MT6351_PMIC_RG_VMC_VOSEL_SHIFT
#define FIELD_VMC_VOSEL                 (MASK_VMC_VOSEL << SHIFT_VMC_VOSEL)
#define MASK_VMC_EN                     MT6351_PMIC_RG_VMC_EN_MASK
#define SHIFT_VMC_EN                    MT6351_PMIC_RG_VMC_EN_SHIFT
#define FIELD_VMC_EN                    (MASK_VMC_EN << SHIFT_VMC_EN)
#define MASK_VMCH_VOSEL_CAL             MT6351_PMIC_RG_VMCH_CAL_MASK
#define SHIFT_VMCH_VOSEL_CAL            MT6351_PMIC_RG_VMCH_CAL_SHIFT
#define FIELD_VMCH_VOSEL_CAL            (MASK_VMCH_VOSEL_CAL << \
                        SHIFT_VMCH_VOSEL_CAL)
#define MASK_VMCH_VOSEL                 MT6351_PMIC_RG_VMCH_VOSEL_MASK
#define SHIFT_VMCH_VOSEL                MT6351_PMIC_RG_VMCH_VOSEL_SHIFT
#define FIELD_VMCH_VOSEL                (MASK_VMCH_VOSEL << SHIFT_VMCH_VOSEL)
#define MASK_VMCH_EN                    MT6351_PMIC_RG_VMCH_EN_MASK
#define SHIFT_VMCH_EN                   MT6351_PMIC_RG_VMCH_EN_SHIFT
#define FIELD_VMCH_EN                   (MASK_VMCH_EN << SHIFT_VMCH_EN)

#define VEMC_VOSEL_CAL_mV(cal)          ((cal <= 0) ? ((0-(cal))/20): (32-(cal)/20))
#define VEMC_VOSEL_3V                   (0)
#define VEMC_VOSEL_3V3                  (1)
#define VMC_VOSEL_1V8                   (3)
#define VMC_VOSEL_2V8                   (5)
#define VMC_VOSEL_3V                    (6)
#define VMCH_VOSEL_CAL_mV(cal)          ((cal <= 0) ? ((0-(cal))/20): (32-(cal)/20))
#define VMCH_VOSEL_3V                   (0)
#define VMCH_VOSEL_3V3                  (1)

#define REG_VCORE_VOSEL_SW      MT6351_PMIC_BUCK_VCORE_VOSEL_ADDR
#define VCORE_VOSEL_SW_MASK     MT6351_PMIC_BUCK_VCORE_VOSEL_MASK
#define VCORE_VOSEL_SW_SHIFT        MT6351_PMIC_BUCK_VCORE_VOSEL_SHIFT
#define REG_VCORE_VOSEL_HW      MT6351_PMIC_BUCK_VCORE_VOSEL_ON_ADDR
#define VCORE_VOSEL_HW_MASK     MT6351_PMIC_BUCK_VCORE_VOSEL_ON_MASK
#define VCORE_VOSEL_HW_SHIFT        MT6351_PMIC_BUCK_VCORE_VOSEL_ON_SHIFT
#define REG_VIO18_CAL           MT6351_PMIC_RG_VIO18_CAL_ADDR
#define VIO18_CAL_MASK          MT6351_PMIC_RG_VIO18_CAL_MASK
#define VIO18_CAL_SHIFT         MT6351_PMIC_RG_VIO18_CAL_SHIFT

#define EMMC_VOL_ACTUAL                 VOL_3000
#define SD_VOL_ACTUAL                   VOL_3000
#endif

#endif /* end of _MSDC_IO_H_ */

