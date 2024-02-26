/* Copyright Statement:
*
* This software/firmware and related documentation ("MediaTek Software") are
* protected under relevant copyright laws. The information contained herein
* is confidential and proprietary to MediaTek Inc. and/or its licensors.
* Without the prior written permission of MediaTek inc. and/or its licensors,
* any reproduction, modification, use or disclosure of MediaTek Software,
* and information contained herein, in whole or in part, shall be strictly prohibited.
*/
/* MediaTek Inc. (C) 2018. All rights reserved.
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
*
* The following software/firmware and/or related documentation ("MediaTek Software")
* have been modified by MediaTek Inc. All revisions are subject to any receiver\'s
* applicable license agreements with MediaTek Inc.
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

#define MSDC0_IO_CFG_BASE       (IOCFG_LT_BASE)
#define MSDC1_IO_CFG_BASE       (IOCFG_LB_BASE)

/*--------------------------------------------------------------------------*/
/* MSDC GPIO Related Register                                               */
/*--------------------------------------------------------------------------*/
/* MSDC0 */
#define MSDC0_GPIO_MODE4        (MSDC_GPIO_BASE + 0x340)
#define MSDC0_GPIO_MODE5        (MSDC_GPIO_BASE + 0x350)
#define MSDC0_GPIO_MODE6        (MSDC_GPIO_BASE + 0x360)
#define MSDC0_GPIO_DRV0_ADDR    (MSDC0_IO_CFG_BASE + 0x0)
#define MSDC0_GPIO_DRV1_ADDR    (MSDC0_IO_CFG_BASE + 0x10)
#define MSDC0_GPIO_IES_ADDR     (MSDC0_IO_CFG_BASE + 0x30)
#define MSDC0_GPIO_PUPD_ADDR    (MSDC0_IO_CFG_BASE + 0x40)
#define MSDC0_GPIO_R0_ADDR      (MSDC0_IO_CFG_BASE + 0x50)
#define MSDC0_GPIO_R1_ADDR      (MSDC0_IO_CFG_BASE + 0x60)
#define MSDC0_GPIO_RDSEL0_ADDR  (MSDC0_IO_CFG_BASE + 0x70)
#define MSDC0_GPIO_RDSEL1_ADDR  (MSDC0_IO_CFG_BASE + 0x80)
#define MSDC0_GPIO_SMT_ADDR     (MSDC0_IO_CFG_BASE + 0x90)
#define MSDC0_GPIO_SR_ADDR      (MSDC1_IO_CFG_BASE + 0xA0)
#define MSDC0_GPIO_TDSEL_ADDR   (MSDC0_IO_CFG_BASE + 0xB0)

/* MSDC1 */
#define MSDC1_GPIO_MODE8        (MSDC_GPIO_BASE + 0x380)
#define MSDC1_GPIO_MODE9        (MSDC_GPIO_BASE + 0x390)
#define MSDC1_GPIO_DRV_ADDR     (MSDC1_IO_CFG_BASE + 0x0)
#define MSDC1_GPIO_IES_ADDR     (MSDC1_IO_CFG_BASE + 0x20)
#define MSDC1_GPIO_PUPD_ADDR    (MSDC1_IO_CFG_BASE + 0x30)
#define MSDC1_GPIO_R0_ADDR      (MSDC1_IO_CFG_BASE + 0x50)
#define MSDC1_GPIO_R1_ADDR      (MSDC1_IO_CFG_BASE + 0x60)
#define MSDC1_GPIO_RDSEL_ADDR   (MSDC1_IO_CFG_BASE + 0x70)
#define MSDC1_GPIO_SMT_ADDR     (MSDC1_IO_CFG_BASE + 0x90)
#define MSDC1_GPIO_SR_ADDR      (MSDC1_IO_CFG_BASE + 0xA0)
#define MSDC1_GPIO_TDSEL_ADDR   (MSDC1_IO_CFG_BASE + 0xB0)


/*
 * MSDC0 GPIO and PAD register and bitfields definition
 */
/* MSDC0_GPIO_MODE4, 001b is msdc mode */
#define MSDC0_MODE_CMD_MASK     (0x7 << 24)
#define MSDC0_MODE_DSL_MASK     (0x7 << 28)
/* MSDC0_GPIO_MODE5, 001b is msdc mode*/
#define MSDC0_MODE_CLK_MASK     (0x7 << 0)
#define MSDC0_MODE_DAT0_MASK    (0x7 << 4)
#define MSDC0_MODE_DAT1_MASK    (0x7 << 8)
#define MSDC0_MODE_DAT2_MASK    (0x7 << 12)
#define MSDC0_MODE_DAT3_MASK    (0x7 << 16)
#define MSDC0_MODE_DAT4_MASK    (0x7 << 20)
#define MSDC0_MODE_DAT5_MASK    (0x7 << 24)
#define MSDC0_MODE_DAT6_MASK    (0x7 << 28)
/* MSDC0_GPIO_MODE6, 001b is msdc mode */
#define MSDC0_MODE_DAT7_MASK    (0x7 << 0)
#define MSDC0_MODE_RSTB_MASK    (0x7 << 4)

/* MSDC0 IES mask*/
#define MSDC0_IES_CLK_MASK      (0x1  <<  6)
#define MSDC0_IES_CMD_MASK      (0x1  <<  7)
#define MSDC0_IES_DAT_MASK      (0xff <<  8)
#define MSDC0_IES_DAT0_MASK     (0x1  <<  8)
#define MSDC0_IES_DAT1_MASK     (0x1  <<  9)
#define MSDC0_IES_DAT2_MASK     (0x1  << 10)
#define MSDC0_IES_DAT3_MASK     (0x1  << 11)
#define MSDC0_IES_DAT4_MASK     (0x1  << 12)
#define MSDC0_IES_DAT5_MASK     (0x1  << 13)
#define MSDC0_IES_DAT6_MASK     (0x1  << 14)
#define MSDC0_IES_DAT7_MASK     (0x1  << 15)
#define MSDC0_IES_DSL_MASK      (0x1  <<  16)
#define MSDC0_IES_RSTB_MASK     (0x1  <<  17)
#define MSDC0_IES_ALL_MASK      (0xfff <<  6)
/* MSDC0 SMT mask*/
#define MSDC0_SMT_CLK_MASK      (0x1 <<   1)
#define MSDC0_SMT_CMD_MASK      (0x1 <<   2)
#define MSDC0_SMT_DAT_MASK      (0x1 <<   3)
#define MSDC0_SMT_DSL_MASK      (0x1 <<   4)
#define MSDC0_SMT_RSTB_MASK     (0x1 <<   5)
#define MSDC0_SMT_ALL_MASK      (0x1f <<  1)
/* MSDC0 TDSEL mask*/
#define MSDC0_TDSEL_CLK_MASK    (0xf  <<   4)
#define MSDC0_TDSEL_CMD_MASK    (0xf  <<   8)
#define MSDC0_TDSEL_DAT_MASK    (0xf  <<  12)
#define MSDC0_TDSEL_DSL_MASK    (0xf  <<  16)
#define MSDC0_TDSEL_RSTB_MASK   (0xf  <<  20)
#define MSDC0_TDSEL_ALL_MASK    (0xfffff << 4)
/* MSDC0 RDSEL mask*/
#define MSDC0_RDSEL_CLK_MASK    (0x3f <<  6)
#define MSDC0_RDSEL_CMD_MASK    (0x3f << 12)
#define MSDC0_RDSEL_DAT_MASK    (0x3f << 18)
#define MSDC0_RDSEL_DSL_MASK    (0x3f << 24)
#define MSDC0_RDSEL0_ALL_MASK   (0xffffff << 6)
#define MSDC0_RDSEL_RSTB_MASK   (0x3f <<  0)
#define MSDC0_RDSEL1_ALL_MASK   (0x3f <<  0)
/* MSDC0 SR mask*/
#define MSDC0_SR_CLK_MASK       (0x1  <<  6)
#define MSDC0_SR_CMD_MASK       (0x1  <<  7)
#define MSDC0_SR_DAT_MASK       (0xff <<  8)
#define MSDC0_SR_DAT0_MASK      (0x1  <<  8)
#define MSDC0_SR_DAT1_MASK      (0x1  <<  9)
#define MSDC0_SR_DAT2_MASK      (0x1  << 10)
#define MSDC0_SR_DAT3_MASK      (0x1  << 11)
#define MSDC0_SR_DAT4_MASK      (0x1  << 12)
#define MSDC0_SR_DAT5_MASK      (0x1  << 13)
#define MSDC0_SR_DAT6_MASK      (0x1  << 14)
#define MSDC0_SR_DAT7_MASK      (0x1  << 15)
#define MSDC0_SR_DSL_MASK       (0x1  <<  16)
#define MSDC0_SR_RSTB_MASK      (0x1  <<  17)
#define MSDC0_SR_ALL_MASK       (0xfff <<  6)
/* MSDC0 DRV mask*/
#define MSDC0_DRV_CMD_MASK      (0x7  << 18)
#define MSDC0_DRV_CLK_MASK      (0x7  << 21)
#define MSDC0_DRV_DAT0_MASK     (0x7  << 24)
#define MSDC0_DRV_DAT1_MASK     (0x7  << 27)
#define MSDC0_DRV0_ALL_MASK     (0xfff << 18)
#define MSDC0_DRV_DAT2_MASK     (0x7  <<  0)
#define MSDC0_DRV_DAT3_MASK     (0x7  <<  3)
#define MSDC0_DRV_DAT4_MASK     (0x7  <<  6)
#define MSDC0_DRV_DAT5_MASK     (0x7  <<  9)
#define MSDC0_DRV_DAT6_MASK     (0x7  << 12)
#define MSDC0_DRV_DAT7_MASK     (0x7  << 15)
#define MSDC0_DRV_DSL_MASK      (0x7  << 18)
#define MSDC0_DRV_RSTB_MASK     (0x7  <<  21)
#define MSDC0_DRV1_ALL_MASK     (0xffffff << 0)
/* MSDC0 PUPD mask */
#define MSDC0_PUPD_CLK_MASK     (0x1  <<  6)
#define MSDC0_PUPD_CMD_MASK     (0x1  <<  7)
#define MSDC0_PUPD_DAT_MASK     (0xff <<  8)
#define MSDC0_PUPD_DAT0_MASK    (0x1  <<  8)
#define MSDC0_PUPD_DAT1_MASK    (0x1  <<  9)
#define MSDC0_PUPD_DAT2_MASK    (0x1  << 10)
#define MSDC0_PUPD_DAT3_MASK    (0x1  << 11)
#define MSDC0_PUPD_DAT4_MASK    (0x1  << 12)
#define MSDC0_PUPD_DAT5_MASK    (0x1  << 13)
#define MSDC0_PUPD_DAT6_MASK    (0x1  << 14)
#define MSDC0_PUPD_DAT7_MASK    (0x1  << 15)
#define MSDC0_PUPD_DSL_MASK     (0x7  << 16)
#define MSDC0_PUPD_RSTB_MASK    (0x7  << 17)
#define MSDC0_PUPD_MASK_WITH_RSTB       (0xFFF << 6)
#define MSDC0_PUPD_ALL_MASK     (0x7FF << 6)
/* MSDC0 R0 mask */
#define MSDC0_R0_CLK_MASK       (0x1  <<  6)
#define MSDC0_R0_CMD_MASK       (0x1  <<  7)
#define MSDC0_R0_DAT_MASK       (0xff <<  8)
#define MSDC0_R0_DAT0_MASK      (0x1  <<  8)
#define MSDC0_R0_DAT1_MASK      (0x1  <<  9)
#define MSDC0_R0_DAT2_MASK      (0x1  << 10)
#define MSDC0_R0_DAT3_MASK      (0x1  << 11)
#define MSDC0_R0_DAT4_MASK      (0x1  << 12)
#define MSDC0_R0_DAT5_MASK      (0x1  << 13)
#define MSDC0_R0_DAT6_MASK      (0x1  << 14)
#define MSDC0_R0_DAT7_MASK      (0x1  << 15)
#define MSDC0_R0_DSL_MASK       (0x7  << 16)
#define MSDC0_R0_RSTB_MASK      (0x7  << 17)
#define MSDC0_R0_MASK_WITH_RSTB (0xFFF << 6)
#define MSDC0_R0_ALL_MASK       (0x7FF << 6)
/* MSDC0 R1 mask */
#define MSDC0_R1_CLK_MASK       (0x1  <<  6)
#define MSDC0_R1_CMD_MASK       (0x1  <<  7)
#define MSDC0_R1_DAT_MASK       (0xff <<  8)
#define MSDC0_R1_DAT0_MASK      (0x1  <<  8)
#define MSDC0_R1_DAT1_MASK      (0x1  <<  9)
#define MSDC0_R1_DAT2_MASK      (0x1  << 10)
#define MSDC0_R1_DAT3_MASK      (0x1  << 11)
#define MSDC0_R1_DAT4_MASK      (0x1  << 12)
#define MSDC0_R1_DAT5_MASK      (0x1  << 13)
#define MSDC0_R1_DAT6_MASK      (0x1  << 14)
#define MSDC0_R1_DAT7_MASK      (0x1  << 15)
#define MSDC0_R1_DSL_MASK       (0x7  << 16)
#define MSDC0_R1_RSTB_MASK      (0x7  << 17)
#define MSDC0_R1_MASK_WITH_RSTB (0xFFF << 6)
#define MSDC0_R1_ALL_MASK       (0x7FF << 6)


/*
 * MSDC1 GPIO and PAD register and bitfields definition
 */
/* MSDC1_GPIO_MODE8, 0001b is msdc mode */
#define MSDC1_MODE_CMD_MASK     (0x7 << 28)
/* MSDC1_GPIO_MODE9, 0001b is msdc mode */
#define MSDC1_MODE_CLK_MASK     (0x7 << 0)
#define MSDC1_MODE_DAT0_MASK    (0x7 << 4)
#define MSDC1_MODE_DAT1_MASK    (0x7 << 8)
#define MSDC1_MODE_DAT2_MASK    (0x7 << 12)
#define MSDC1_MODE_DAT3_MASK    (0x7 << 16)

/* MSDC1 IES mask*/
#define MSDC1_IES_CLK_MASK      (0x1 <<  2)
#define MSDC1_IES_CMD_MASK      (0x1 <<  3)
#define MSDC1_IES_DAT_MASK      (0xf <<  4)
#define MSDC1_IES_DAT0_MASK     (0x1 <<  4)
#define MSDC1_IES_DAT1_MASK     (0x1 <<  5)
#define MSDC1_IES_DAT2_MASK     (0x1 <<  6)
#define MSDC1_IES_DAT3_MASK     (0x1 <<  7)
#define MSDC1_IES_ALL_MASK      (0x3f <<  2)
/* MSDC1 SMT mask*/
#define MSDC1_SMT_CLK_MASK      (0x1 <<  2)
#define MSDC1_SMT_CMD_MASK      (0x1 <<  3)
#define MSDC1_SMT_DAT_MASK      (0x1 <<  4)
#define MSDC1_SMT_DAT0_MASK     (0x1 <<  4)
#define MSDC1_SMT_DAT1_MASK     (0x1 <<  5)
#define MSDC1_SMT_DAT2_MASK     (0x1 <<  6)
#define MSDC1_SMT_DAT3_MASK     (0x1 <<  7)
#define MSDC1_SMT_ALL_MASK      (0x3f <<  2)
/* MSDC1 TDSEL mask*/
#define MSDC1_TDSEL_DAT_MASK    (0xf <<  4)
#define MSDC1_TDSEL_CMD_MASK    (0xf <<  8)
#define MSDC1_TDSEL_CLK_MASK    (0xf << 12)
#define MSDC1_TDSEL_ALL_MASK    (0xfff << 4)
/* MSDC1 RDSEL mask*/
#define MSDC1_RDSEL_CLK_MASK    (0x3f <<  6)
#define MSDC1_RDSEL_CMD_MASK    (0x3f << 12)
#define MSDC1_RDSEL_DAT_MASK    (0x3f << 18)
#define MSDC1_RDSEL_ALL_MASK    (0x3ffff << 6)
/* MSDC1 SR mask*/
#define MSDC1_SR_CLK_MASK       (0x1 << 2)
#define MSDC1_SR_CMD_MASK       (0x1 << 3)
#define MSDC1_SR_DAT_MASK       (0xf << 4)
#define MSDC1_SR_DAT0_MASK      (0xf << 4)
#define MSDC1_SR_DAT1_MASK      (0xf << 4)
#define MSDC1_SR_DAT2_MASK      (0xf << 4)
#define MSDC1_SR_DAT3_MASK      (0xf << 4)
#define MSDC1_SR_ALL_MASK       (0x3f << 2)
/* MSDC1 DRV mask*/
#define MSDC1_DRV_CLK_MASK      (0x7 <<  3)
#define MSDC1_DRV_CMD_MASK      (0x7 <<  6)
#define MSDC1_DRV_DAT_MASK      (0x7 <<  9)
#define MSDC1_DRV_ALL_MASK      (0x1ff << 3)
/* MSDC1 PUPD mask */
#define MSDC1_PUPD_CLK_MASK     (0x7 << 2)
#define MSDC1_PUPD_CMD_MASK     (0x7 << 3)
#define MSDC1_PUPD_DAT0_MASK    (0x7 << 4)
#define MSDC1_PUPD_DAT1_MASK    (0x7 << 5)
#define MSDC1_PUPD_DAT2_MASK    (0x7 << 6)
#define MSDC1_PUPD_DAT3_MASK    (0x7 << 7)
#define MSDC1_PUPD_ALL_MASK     (0x3F << 2)
/* MSDC1 R0 mask */
#define MSDC1_R0_CLK_MASK       (0x7 << 2)
#define MSDC1_R0_CMD_MASK       (0x7 << 3)
#define MSDC1_R0_DAT0_MASK      (0x7 << 4)
#define MSDC1_R0_DAT1_MASK      (0x7 << 5)
#define MSDC1_R0_DAT2_MASK      (0x7 << 6)
#define MSDC1_R0_DAT3_MASK      (0x7 << 7)
#define MSDC1_R0_ALL_MASK       (0x3F << 2)
/* MSDC1 R1 mask */
#define MSDC1_R1_CLK_MASK       (0x7 << 2)
#define MSDC1_R1_CMD_MASK       (0x7 << 3)
#define MSDC1_R1_DAT0_MASK      (0x7 << 4)
#define MSDC1_R1_DAT1_MASK      (0x7 << 5)
#define MSDC1_R1_DAT2_MASK      (0x7 << 6)
#define MSDC1_R1_DAT3_MASK      (0x7 << 7)
#define MSDC1_R1_ALL_MASK       (0x3F << 2)


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

void msdc_card_power(struct mmc_host *host, u32 on);
void msdc_power(struct mmc_host *host, u8 mode);

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
void msdc_apply_timing(unsigned int id);
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
	MSDC0_CLKSRC_26MHZ  = 0,
	MSDC0_CLKSRC_400MHZ,
	MSDC0_CLKSRC_182MHZ,
	MSDC0_CLKSRC_78MHZ,
	MSDC0_CLKSRC_312MHZ,
	MSDC0_CLKSRC_273MHZ,
	MSDC0_CLKSRC_250MHZ,
	MSDC0_CLKSRC_156MHZ,
	MSDC0_CLKSRC_MAX
};

enum {
	MSDC1_CLKSRC_26MHZ   = 0,
	MSDC1_CLKSRC_200MHZ,
	MSDC1_CLKSRC_208MHZ,
	MSDC1_CLKSRC_182MHZ,
	MSDC1_CLKSRC_137MHZ,
	MSDC1_CLKSRC_156MHZ,
	MSDC1_CLKSRC_48MHZ,
	MSDC1_CLKSRC_91MHZ,
	MSDC1_CLKSRC_MAX
};

#define MSDC0_CLKSRC_DEFAULT    MSDC0_CLKSRC_400MHZ
#define MSDC1_CLKSRC_DEFAULT    MSDC1_CLKSRC_200MHZ
#define MSDC_CLKSRC_DEFAULT(ID) MSDC##ID_CLKSRC_DEFAULT

#define MSDCPLL_FREQ            400000000

#define MSDC0_SRC_0             260000
#define MSDC0_SRC_1             MSDCPLL_FREQ
#define MSDC0_SRC_2             182000000
#define MSDC0_SRC_3             78000000
#define MSDC0_SRC_4             312000000
#define MSDC0_SRC_5             273000000
#define MSDC0_SRC_6             249600000
#define MSDC0_SRC_7             156000000

#define MSDC1_SRC_0             260000
#define MSDC1_SRC_1             (MSDCPLL_FREQ/2)
#define MSDC1_SRC_2             208000000
#define MSDC1_SRC_3             182000000
#define MSDC1_SRC_4             136500000
#define MSDC1_SRC_5             156000000
#define MSDC1_SRC_6             48000000
#define MSDC1_SRC_7             91000000

void msdc_clock(struct mmc_host *host, int on);
void msdc_config_clksrc(struct mmc_host *host, int clksrc);

#if defined(FPGA_PLATFORM)
#define msdc_get_hclk(host, src)        MSDC_SRC_CLK
#else
extern u32 hclks_msdc0[];
extern u32 hclks_msdc1[];
extern u32 *hclks_msdc_all[];
#define msdc_get_hclk(id, src)          hclks_msdc_all[id][src]
#endif

#define MSDC0_HCLOCK_MUX_REG    (TOPCKGEN_BASE    + 0x0070)
#define MSDC0_CLOCK_MUX_REG     (TOPCKGEN_BASE    + 0x0070)
#define MSDC1_CLOCK_MUX_REG     (TOPCKGEN_BASE    + 0x0070)
#define MSDC0_HCLOCK_MUX        (3 <<  8)
#define MSDC0_HCLOCK_PDN        (1 << 15)
#define MSDC0_CLOCK_MUX         (7 << 16)
#define MSDC0_CLOCK_PDN         (1 << 23)
#define MSDC1_CLOCK_MUX         (7 << 24)
#define MSDC1_CLOCK_PDN         (1 << 31)

#define MSDC0_HCLOCK_GATE_REG   (INFRACFG_AO_BASE + 0x0088)
#define MSDC0_HCLOCK_UNGATE_REG (INFRACFG_AO_BASE + 0x008C)
#define MSDC0_HCLOCK_CG         (1 << 2)
#define MSDC1_HCLOCK_CG         (1 << 4)

#define MSDC0_CLOCK_GATE_REG    (INFRACFG_AO_BASE + 0x00C0)
#define MSDC1_CLOCK_GATE_REG    (INFRACFG_AO_BASE + 0x00C0)
#define MSDC0_CLOCK_UNGATE_REG  (INFRACFG_AO_BASE + 0x00C4)
#define MSDC1_CLOCK_UNGATE_REG  (INFRACFG_AO_BASE + 0x00C4)
#define MSDC0_CLOCK_CG_AP       (1 << 7)
#define MSDC0_CLOCK_CG_MD       (1 << 8)
#define MSDC0_CLOCK_CG_SRC      (1 << 9)
#define MSDC1_CLOCK_CG          (1 << 10)


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

typedef enum MSDC_POWER_TAG {
	MSDC_VMC,
	MSDC_VMCH,
	MSDC_VEMC,
} MSDC_POWER;

#define REG_VEMC_VOSEL_CAL      PMIC_RG_VEMC_VOCAL_ADDR
#define REG_VEMC_VOSEL          PMIC_RG_VEMC_VOSEL_ADDR
#define REG_VEMC_EN             PMIC_RG_LDO_VEMC_EN_ADDR /* TBC */

#define REG_VMC_VOSEL_CAL       PMIC_RG_VMC_VOCAL_ADDR
#define REG_VMC_VOSEL           PMIC_RG_VMC_VOSEL_ADDR
#define REG_VMC_EN              PMIC_RG_LDO_VMC_EN_ADDR /* TBC */
#define REG_VMCH_VOSEL_CAL      PMIC_RG_VMCH_VOCAL_ADDR
#define REG_VMCH_VOSEL          PMIC_RG_VMCH_VOSEL_ADDR
#define REG_VMCH_EN             PMIC_RG_LDO_VMCH_EN_ADDR /* TBC */

#define MASK_VEMC_VOSEL_CAL     PMIC_RG_VEMC_VOCAL_MASK
#define SHIFT_VEMC_VOSEL_CAL    PMIC_RG_VEMC_VOCAL_SHIFT
#define FIELD_VEMC_VOSEL_CAL    (MASK_VEMC_VOSEL_CAL \
					<< SHIFT_VEMC_VOSEL_CAL)

#define MASK_VEMC_VOSEL         PMIC_RG_VEMC_VOSEL_MASK
#define SHIFT_VEMC_VOSEL        PMIC_RG_VEMC_VOSEL_SHIFT
#define FIELD_VEMC_VOSEL        (MASK_VEMC_VOSEL << SHIFT_VEMC_VOSEL)

#define MASK_VEMC_EN            PMIC_RG_LDO_VEMC_EN_MASK
#define SHIFT_VEMC_EN           PMIC_RG_LDO_VEMC_EN_SHIFT
#define FIELD_VEMC_EN           (MASK_VEMC_EN << SHIFT_VEMC_EN)

#define MASK_VMC_VOSEL_CAL      PMIC_RG_VMC_VOCAL_MASK
#define SHIFT_VMC_VOSEL_CAL     PMIC_RG_VMC_VOCAL_SHIFT
#define FIELD_VMC_VOSEL_CAL     (MASK_VMC_VOSEL_CAL \
					<< SHIFT_VMC_VOSEL_CAL)

#define MASK_VMC_VOSEL          PMIC_RG_VMC_VOSEL_MASK
#define SHIFT_VMC_VOSEL         PMIC_RG_VMC_VOSEL_SHIFT
#define FIELD_VMC_VOSEL         (MASK_VMC_VOSEL << SHIFT_VMC_VOSEL)

#define MASK_VMC_EN             PMIC_RG_LDO_VMC_EN_MASK
#define SHIFT_VMC_EN            PMIC_RG_LDO_VMC_EN_SHIFT
#define FIELD_VMC_EN            (MASK_VMC_EN << SHIFT_VMC_EN)

#define MASK_VMCH_VOSEL_CAL     PMIC_RG_VMCH_VOCAL_MASK
#define SHIFT_VMCH_VOSEL_CAL    PMIC_RG_VMCH_VOCAL_SHIFT
#define FIELD_VMCH_VOSEL_CAL    (MASK_VMCH_VOSEL_CAL \
					<< SHIFT_VMCH_VOSEL_CAL)

#define MASK_VMCH_VOSEL         PMIC_RG_VMCH_VOSEL_MASK
#define SHIFT_VMCH_VOSEL        PMIC_RG_VMCH_VOSEL_SHIFT
#define FIELD_VMCH_VOSEL        (MASK_VMCH_VOSEL << SHIFT_VMCH_VOSEL)

#define MASK_VMCH_EN            PMIC_RG_LDO_VMCH_EN_MASK
#define SHIFT_VMCH_EN           PMIC_RG_LDO_VMCH_EN_SHIFT
#define FIELD_VMCH_EN           (MASK_VMCH_EN << SHIFT_VMCH_EN)

#define VEMC_VOSEL_CAL_mV(cal)  ((cal >= 0) ? ((cal)/10) : 0)
#define VEMC_VOSEL_2V9          (0x2)
#define VEMC_VOSEL_3V           (0x3)
#define VEMC_VOSEL_3V3          (0x5)
#define VMC_VOSEL_CAL_mV(cal)   ((cal >= 0) ? ((cal)/10) : 0)
#define VMC_VOSEL_1V8           (0x4)
#define VMC_VOSEL_2V9           (0xa)
#define VMC_VOSEL_3V            (0xb)
#define VMC_VOSEL_3V3           (0xd)
#define VMCH_VOSEL_CAL_mV(cal)  ((cal >= 0) ? ((cal)/10) : 0)
#define VMCH_VOSEL_2V9          (0x2)
#define VMCH_VOSEL_3V           (0x3)
#define VMCH_VOSEL_3V3          (0x5)

#define REG_VCORE_VOCAL         0x1522
#define REG_VCORE_MIN_UV        518750
#define REG_VCORE_STEP_UV       6250

#define REG_VIO18_VOCAL         0x1C4C
#define MASK_VIO18_VOCAL        0xF
#define SHIFT_VIO18_VOCAL       0
#define VIO18_VOCAL_mV(cal)     ((cal >= 0) ? ((cal)/10) : 0)
#define VIO18_VOSEL_1V8         (0xc)
/* Note: 6357 does not support 1.7V VIO18 */

#define EMMC_VOL_ACTUAL         VOL_3000
#define SD_VOL_ACTUAL           VOL_3000


/*******************************************************************************
 * Power Definition
 ******************************************************************************/
#define MSDC_PB0_DEFAULT        0x403C0007
#define MSDC_PB1_DEFAULT        0xFFE20349
#define MSDC_PB2_DEFAULT        0x14801803

#define CMD_RSP_TA_CNTR_DEFAULT         0
#define WRDAT_CRCS_TA_CNTR_DEFAULT      0
#define BUSY_MA_DEFAULT                 1

#define CRCSTSENSEL_HS400_DEFAULT       3
#define RESPSTENSEL_HS400_DEFAULT       3
#define CRCSTSENSEL_HS_DEFAULT          1
#define RESPSTENSEL_HS_DEFAULT          1
#define CRCSTSENSEL_FPGA_DEFAULT        0

#endif /* end of _MSDC_IO_H_ */
