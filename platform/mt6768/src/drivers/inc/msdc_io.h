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

/*--------------------------------------------------------------------------*/
/* MSDC GPIO Related Register                                               */
/*--------------------------------------------------------------------------*/
#define MSDC0_GPIO_MODE0       (MSDC_GPIO_BASE + 0x3f0)
#define MSDC0_GPIO_MODE1       (MSDC_GPIO_BASE + 0x400)
#define MSDC0_GPIO_IES     (MSDC0_IO_PAD_BASE + 0x10)
#define MSDC0_GPIO_SMT     (MSDC0_IO_PAD_BASE + 0x60)
#define MSDC0_GPIO_TDSEL   (MSDC0_IO_PAD_BASE + 0x70)
#define MSDC0_GPIO_RDSEL   (MSDC0_IO_PAD_BASE + 0x50)
#define MSDC0_GPIO_DRV     (MSDC0_IO_PAD_BASE + 0)
#define MSDC0_GPIO_PUPD   (MSDC0_IO_PAD_BASE + 0x20)
#define MSDC0_GPIO_R0   (MSDC0_IO_PAD_BASE + 0x30)
#define MSDC0_GPIO_R1   (MSDC0_IO_PAD_BASE + 0x40)

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

#ifndef FPGA_PLATFORM
void msdc_set_driving_by_id(u32 id, struct msdc_cust *msdc_cap);
void msdc_get_driving_by_id(u32 id, struct msdc_cust *msdc_cap);
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
#define msdc_set_driving_by_id(id, msdc_cap)
#define msdc_get_driving_by_id(id, msdc_cap)
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

#define msdc_get_driving(host, msdc_cap) \
	msdc_get_driving_by_id(host->id, msdc_cap)

#define msdc_set_driving(host, msdc_cap) \
	msdc_set_driving_by_id(host->id, msdc_cap)

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
	MSDC0_CLKSRC_MAX
};

enum {
	MSDC1_CLKSRC_26MHZ   = 0,
	MSDC1_CLKSRC_208MHZ,
	MSDC1_CLKSRC_200MHZ,
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

#define MSDC0_SRC_0             260000
#define MSDC0_SRC_1             (MSDCPLL_FREQ/2)

#define MSDC1_SRC_0             260000
#define MSDC1_SRC_1             208000000
#define MSDC1_SRC_2             (MSDCPLL_FREQ/4)

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

#define REG_VEMC_VOSEL          PMIC_RG_VEMC_VOSEL_ADDR
#define REG_VEMC_EN             PMIC_RG_LDO_VEMC_EN_ADDR

#define MASK_VEMC_VOSEL         PMIC_RG_VEMC_VOSEL_MASK
#define SHIFT_VEMC_VOSEL        PMIC_RG_VEMC_VOSEL_SHIFT
#define FIELD_VEMC_VOSEL        (MASK_VEMC_VOSEL << SHIFT_VEMC_VOSEL)

#define MASK_VEMC_EN            PMIC_RG_LDO_VEMC_EN_MASK
#define SHIFT_VEMC_EN           PMIC_RG_LDO_VEMC_EN_SHIFT
#define FIELD_VEMC_EN           (MASK_VEMC_EN << SHIFT_VEMC_EN)

#define EMMC_VOL_ACTUAL         VOL_3000
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

