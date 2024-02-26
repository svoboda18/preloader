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

#include "msdc.h"
#include "msdc_io.h"

#if defined(MMC_MSDC_DRV_CTP)
#include <common.h>
#include "api.h"    //For invocation cache_clean_invalidate()
#include "cache_api.h"  //For invocation cache_clean_invalidate()
#endif

#if defined(MMC_MSDC_DRV_LK)
#include <kernel/event.h>
#include <platform/mt_irq.h>
#endif

#if defined(MMC_MSDC_DRV_CTP)
#include "gpio.h"

#if defined(MSDC_USE_DCM)
#include "dcm.h"
#endif

#if !defined(FPGA_PLATFORM)
#include "pmic.h"
#include "clock_manager.h"
#endif
#endif

////////////////////////////////////////////////////////////////////////////////
//
// Section 1. Power Control -- Common for ASIC and FPGA
//
////////////////////////////////////////////////////////////////////////////////
//Note: MTKDRV_XXX are used by CTP
#if defined(MTKDRV_PMIC) && defined(MTKDRV_PMIC_WRAP) && defined(MTKDRV_I2C)
#include <upmu_common.h>
#else
#define pmic_config_interface(RegNum, val, MASK, SHIFT)
#define pmic_read_interface(RegNum, val, MASK, SHIFT)
#endif

u32 g_msdc0_io;
u32 g_msdc1_io;
u32 g_msdc2_io;
u32 g_msdc3_io;

u32 g_msdc0_flash;
u32 g_msdc1_flash;
u32 g_msdc2_flash;
u32 g_msdc3_flash;

#if defined(FPGA_PLATFORM)
#define PWR_GPIO        (0x10001E84) //Select Power On/Off
#define PWR_GPIO_EO     (0x10001E88) //Select Output/Input Direction

#define PWR_MASK_CARD       (0x1 << 8)   //Vcc
#define PWR_MASK_VOL_18     (0x1 << 9)   //CLK/CMD/DAT for eMMC/SD +  Vccq for eMMC
#define PWR_MASK_VOL_33     (0x1 << 10)  //CLK/CMD/DAT for eMMC/SD +  Vccq for eMMC
#define PWR_MASK_L4     (0x1 << 11)
#define PWR_MSDC        (PWR_MASK_CARD | PWR_MASK_VOL_18 | PWR_MASK_VOL_33 | PWR_MASK_L4)

//#define FPGA_GPIO_DEBUG
static void msdc_clr_gpio(u32 bits)
{
	u32 l_val = 0;
	char *name;

	switch (bits) {
		case PWR_MASK_CARD:
			name="card pwr";
			break;
		case PWR_MASK_VOL_18:
			name="SD/eMMC bus pwer 1.8V";
			break;
		case PWR_MASK_VOL_33:
			name="SD/eMMC bus pwer 3.3V";
			break;
		case PWR_MASK_L4:
			name="L4";
			break;
		default:
			msdc_printf("[%s:%d]invalid value: 0x%x\n", __FILE__, __func__, bits);
			return;
	}

	MSDC_GET_FIELD(PWR_GPIO_EO, bits, l_val);
	//msdc_printf("====%s====%d\n", name, l_val);
	if (0 == l_val) {
		msdc_printf("check me! gpio for %s is input\n", name);
		MSDC_SET_FIELD(PWR_GPIO_EO, bits, 1);
	}
	/* check for set before */
	if (bits &  MSDC_READ32(PWR_GPIO)) {
		msdc_printf("clear %s\n", name);
		MSDC_SET_FIELD(PWR_GPIO, bits, 0);
		//l_val = MSDC_READ32(PWR_GPIO);
	}

#ifdef FPGA_GPIO_DEBUG
	{
		u32 val = 0;
		val = MSDC_READ32(PWR_GPIO);
		msdc_printf("[clr]PWR_GPIO[8-11]:0x%x\n", val);
		val = MSDC_READ32(PWR_GPIO_EO);
		msdc_printf("[clr]GPIO_DIR[8-11]	   :0x%x\n", val);
	}
#endif
}

static void msdc_set_gpio(u32 bits)
{
	u32 l_val = 0;
	char *name;

	switch (bits) {
		case PWR_MASK_CARD:
			name="card pwr";
			break;
		case PWR_MASK_VOL_18:
			name="SD/eMMC bus pwer 1.8V";
			break;
		case PWR_MASK_VOL_33:
			name="SD/eMMC bus pwer 3.3V";
			break;
		case PWR_MASK_L4:
			name="L4";
			break;
		default:
			msdc_printf("[%s:%d]invalid value: 0x%x\n", __FILE__, __func__, bits);
			return;
	}

	MSDC_GET_FIELD(PWR_GPIO_EO, bits, l_val);
	//msdc_printf("====%s====%d\n", name, l_val);
	if (0 == l_val) {
		msdc_printf("check me! gpio for %s is input\n", name);
		MSDC_SET_FIELD(PWR_GPIO_EO, bits, 1);
	}
	/* check for set before */
	if ((bits &  MSDC_READ32(PWR_GPIO))==0) {
		msdc_printf("Set %s\n", name);
		MSDC_SET_FIELD(PWR_GPIO, bits, 1);
		//l_val = MSDC_READ32(PWR_GPIO);
	}

#ifdef FPGA_GPIO_DEBUG
	{
		u32 val = 0;
		val = MSDC_READ32(PWR_GPIO);
		msdc_printf("[set]PWR_GPIO[8-11]:0x%x\n", val);
		val = MSDC_READ32(PWR_GPIO_EO);
		msdc_printf("[set]GPIO_DIR[8-11]	   :0x%x\n", val);
	}
#endif
}
#endif

#if defined(MMC_MSDC_DRV_CTP) && !defined(FPGA_PLATFORM)
u32 hwPowerOn(MSDC_POWER powerId, MSDC_POWER_VOLTAGE powerVolt)
{
	switch (powerId) {
		case MSDC_VEMC:
			if (powerVolt == VOL_3000) {
				pmic_config_interface(REG_VEMC_VOSEL, VEMC_VOSEL_3V, MASK_VEMC_VOSEL, SHIFT_VEMC_VOSEL);
				if (EMMC_VOL_ACTUAL != VOL_3000) {
					pmic_config_interface(REG_VEMC_VOSEL_CAL,
					                      VEMC_VOSEL_CAL_mV(EMMC_VOL_ACTUAL - VOL_3000),
					                      MASK_VEMC_VOSEL_CAL,
					                      SHIFT_VEMC_VOSEL_CAL);
				}
			} else if (powerVolt == VOL_3300) {
				pmic_config_interface(REG_VEMC_VOSEL, VEMC_VOSEL_3V3, MASK_VEMC_VOSEL, SHIFT_VEMC_VOSEL);
			} else {
				msdc_printf("Not support to Set VEMC_3V3 power to %d\n", powerVolt);
			}
			pmic_config_interface(REG_VEMC_EN, 1, MASK_VEMC_EN, SHIFT_VEMC_EN);
			break;

		case MSDC_VMC:
			if (powerVolt == VOL_3300 || powerVolt == VOL_3000)
				pmic_config_interface(REG_VMC_VOSEL, VMC_VOSEL_3V, MASK_VMC_VOSEL, SHIFT_VMC_VOSEL);
			else if (powerVolt == VOL_1800)
				pmic_config_interface(REG_VMC_VOSEL, VMC_VOSEL_1V8, MASK_VMC_VOSEL, SHIFT_VMC_VOSEL);
			else
				msdc_printf("Not support to Set VMC power to %d\n", powerVolt);
			pmic_config_interface(REG_VMC_EN, 1, MASK_VMC_EN, SHIFT_VMC_EN);
			break;

		case MSDC_VMCH:
			if (powerVolt == VOL_3000) {
				pmic_config_interface(REG_VMCH_VOSEL, VMCH_VOSEL_3V, MASK_VMCH_VOSEL, SHIFT_VMCH_VOSEL);
				if (SD_VOL_ACTUAL != VOL_3000) {
					pmic_config_interface(REG_VMCH_VOSEL_CAL,
					                      VMCH_VOSEL_CAL_mV(SD_VOL_ACTUAL - VOL_3000),
					                      MASK_VMCH_VOSEL_CAL,
					                      SHIFT_VMCH_VOSEL_CAL);
				}
			} else if (powerVolt == VOL_3300) {
				pmic_config_interface(REG_VMCH_VOSEL, VMCH_VOSEL_3V3, MASK_VMCH_VOSEL, SHIFT_VMCH_VOSEL);
			} else {
				msdc_printf("Not support to Set VMCH power to %d\n", powerVolt);
			}
			pmic_config_interface(REG_VMCH_EN, 1, MASK_VMCH_EN, SHIFT_VMCH_EN);
			break;

		default:
			msdc_printf("Not support to Set %d power on\n", powerId);
			break;
	}

	mdelay(100); /* requires before voltage stable */

	return 0;
}

u32 hwPowerDown(MSDC_POWER powerId)
{
	switch (powerId) {
		case MSDC_VEMC:
			pmic_config_interface(REG_VEMC_EN, 0, MASK_VEMC_EN, SHIFT_VEMC_EN);
			break;
		case MSDC_VMC:
			pmic_config_interface(REG_VMC_EN, 0, MASK_VMC_EN, SHIFT_VMC_EN);
			break;
		case MSDC_VMCH:
			pmic_config_interface(REG_VMCH_EN, 0, MASK_VMCH_EN, SHIFT_VMCH_EN);
			break;
		default:
			msdc_printf("Not support to Set %d power down\n", powerId);
			break;
	}
	return 0;
}

static u32 msdc_ldo_power(u32 on, MSDC_POWER powerId, MSDC_POWER_VOLTAGE powerVolt, u32 *status)
{
	if (on) { // want to power on
		if (*status == 0) {  // can power on
			msdc_printf("msdc LDO<%d> power on<%d>\n", powerId, powerVolt);
			hwPowerOn(powerId, powerVolt);
			*status = powerVolt;
		} else if (*status == powerVolt) {
			msdc_printf("msdc LDO<%d><%d> power on again!\n", powerId, powerVolt);
		} else { // for sd3.0 later
			msdc_printf("msdc LDO<%d> change<%d>	to <%d>\n", powerId, *status, powerVolt);
			hwPowerDown(powerId);
			hwPowerOn(powerId, powerVolt);
			*status = powerVolt;
		}
	} else {  // want to power off
		if (*status != 0) {  // has been powerred on
			msdc_printf("msdc LDO<%d> power off\n", powerId);
			hwPowerDown(powerId);
			*status = 0;
		} else {
			msdc_printf("LDO<%d>	not power on\n", powerId);
		}
	}

	return 0;
}
#endif /* defined(MMC_MSDC_DRV_CTP) && !defined(FPGA_PLATFORM) */

#if defined(FPGA_PLATFORM)
void msdc_card_power(struct mmc_host *host, u32 on)
{
	MSG(CFG, "[SD%d] Turn %s card power \n", host->id, on ? "on" : "off");
	switch (host->id) {
		case 0:
			if (on) {
				//On V6/V7 FPGA, card power need to be on/off
				//On HAPS FPGA, card power maybe always on.
				//Left this code for safety prevent from card power not always not.
				msdc_set_gpio(PWR_MASK_CARD);
			} else {
				msdc_clr_gpio(PWR_MASK_CARD);
			}
			mdelay(10);
			break;
		default:
			//No MSDC1 in FPGA
			break;
	}

	//add for fpga debug
	msdc_set_gpio(PWR_MASK_L4);
}

void msdc_host_power(struct mmc_host *host, u32 on, u32 level)
{
	MSG(CFG, "[SD%d] Turn %s host power \n", host->id, on ? "on" : "off");

	// GPO[3:2] = {LVL_PWR33, LVL_PWR18};
	msdc_clr_gpio(PWR_MASK_VOL_18);
	msdc_clr_gpio(PWR_MASK_VOL_33);

	switch (host->id) {
		case 0:
			if (on) {
				if (level==VOL_1800)
					msdc_set_gpio(PWR_MASK_VOL_18);
				else
					msdc_set_gpio(PWR_MASK_VOL_33);
			}
			mdelay(10);
			break;
		default:
			//No MSDC1 in FPGA
			break;
	}

	//add for fpga debug
	msdc_set_gpio(PWR_MASK_L4);
}
#else
void msdc_card_power(struct mmc_host *host, u32 on)
{
	//Only CTP support power on/off for verification purose.
	//Preload and LK need not touch power since it is default on
	MSG(CFG, "[SD%d] Turn %s card power \n", host->id, on ? "on" : "off");

#if defined(MMC_MSDC_DRV_CTP)
	switch (host->id) {
		case 0:
#if defined(MTK_PMIC_CHIP_MT6353)
			pmic_config_interface(REG_VEMC_DL_EN, on, MASK_VEMC_DL_EN,
			                      SHIFT_VEMC_DL_EN);
			/*
			pmic_config_interface(REG_VEMC_CL_EN, on, MASK_VEMC_CL_EN,
			    SHIFT_VEMC_CL_EN);*/
#endif
			msdc_ldo_power(on, MSDC_VEMC, VOL_3000, &g_msdc0_flash);
			mdelay(10);
			break;
		case 1:
#if defined(MTK_PMIC_CHIP_MT6353)
			pmic_config_interface(REG_VMCH_RAMPUP_SEL, on, MASK_VMCH_RAMPUP_SEL,
			                      SHIFT_VMCH_RAMPUP_SEL);
			pmic_config_interface(REG_VMCH_DL_EN, on, MASK_VMCH_DL_EN,
			                      SHIFT_VMCH_DL_EN);
			/*
			pmic_config_interface(REG_VMCH_CL_EN, on, MASK_VMCH_CL_EN,
			    SHIFT_VMCH_CL_EN);*/
#endif
			msdc_ldo_power(on, MSDC_VMCH, VOL_3000, &g_msdc1_flash);
			mdelay(10);
			break;
		case 2:
			if (on && (host->cur_pwr < VOL_2000) ) {
				//For MTK SDIO device initialized at 1.8V, use external source of 1.8V
				//So we just turn VMCH off
				on = 0;
			}
			msdc_ldo_power(on, MSDC_VMCH, VOL_3000, &g_msdc2_flash);
			mdelay(10);
			break;
		default:
			break;
	}
#endif
}

void msdc_host_power(struct mmc_host *host, u32 on, u32 level)
{
	//Only CTP support power on/off for verification purose.
	//Preload and LK need not touch power since it is default on
#if defined(MMC_MSDC_DRV_CTP)
	u32 card_on=on;
	if (host->id==1 || host->id==2) {
		host->cur_pwr=level;
	}
	msdc_set_tdsel(host, 0, (host->cur_pwr == VOL_1800));
	msdc_set_rdsel(host, 0, (host->cur_pwr == VOL_1800));
	msdc_set_driving(host, &msdc_cap[host->id], (level == VOL_1800));

	MSG(CFG, "[SD%d] Turn %s host power \n", host->id, on ? "on" : "off");

	switch (host->id) {
		case 0:
			//no need change;
			break;
		case 1:
#if defined(MTK_PMIC_CHIP_MT6353)
			pmic_config_interface(REG_VMC_DL_EN, on, MASK_VMC_DL_EN,
			                      SHIFT_VMC_DL_EN);
			/*pmic_config_interface(REG_VMC_CL_EN, on, MASK_VMC_CL_EN,
			    SHIFT_VMC_CL_EN);*/
#endif
			msdc_ldo_power(on, MSDC_VMC, level, &g_msdc1_io);
			mdelay(10);
			break;
		case 2:
			//For MSDC with SD card, not for SDIO
			msdc_ldo_power(on, MSDC_VMC, level, &g_msdc2_io);
			mdelay(10);
			break;
		default:
			break;
	}
#endif
}
#endif

void msdc_power(struct mmc_host *host, u8 mode)
{
	if (mode == MMC_POWER_ON || mode == MMC_POWER_UP) {
		msdc_pin_config(host, MSDC_PIN_PULL_UP);
		msdc_host_power(host, 1, host->cur_pwr);
		msdc_card_power(host, 1);
		msdc_clock(host, 1);
	} else {
		msdc_clock(host, 0);
		msdc_host_power(host, 0, host->cur_pwr);
		msdc_card_power(host, 0);
		msdc_pin_config(host, MSDC_PIN_PULL_DOWN);
	}
}

void msdc_dump_ldo_sts(struct mmc_host *host)
{
#ifdef MTK_MSDC_BRINGUP_DEBUG
	u32 ldo_en = 0, ldo_vol = 0;

	pmic_read_interface(REG_VEMC_EN, &ldo_en, MASK_VEMC_EN,
	                    SHIFT_VEMC_EN);
	pmic_read_interface(REG_VEMC_VOSEL, &ldo_vol, MASK_VEMC_VOSEL,
	                    SHIFT_VEMC_VOSEL);
	msdc_printf(" VEMC_EN=0x%x, VEMC_VOL=0x%x [2b'00(2.9V),2b'01(3V),2b'10(3.3V)]\n",
	            ldo_en, ldo_vol);
#endif
}


/**************************************************************/
/* Section 2: Clock                    */
/**************************************************************/
#if !defined(FPGA_PLATFORM)
u32 hclks_msdc50[] = {26000000,  400000000, 200000000, 156000000,
                      182000000, 156000000, 100000000, 624000000,
                      312000000
                     };

u32 hclks_msdc30[] = {26000000,  208000000, 100000000, 156000000,
                      182000000, 156000000, 178000000, 200000000
                     };

u32 *msdc_src_clks = hclks_msdc30;
#else
u32 msdc_src_clks[] = {12000000, 12000000, 12000000, 12000000, 12000000,
                       12000000, 12000000, 12000000, 12000000
                      };
#endif

void msdc_clock(struct mmc_host *host, int on)
{
	//Only CTP need enable/disable clock
	//Preloader will turn on clock with predefined clock source and module need not to touch clock setting.
	//LK will use preloader's setting and need not to touch clock setting.
#if !defined(FPGA_PLATFORM)
#if defined(MMC_MSDC_DRV_CTP) && defined(MTKDRV_CLOCK_MANAGER)
	int cg_address[MSDC_MAX_NUM] = {
		MSDC0_CLOCK_GATE_REG, MSDC1_CLOCK_GATE_REG,
		MSDC2_CLOCK_GATE_REG, MSDC3_CLOCK_GATE_REG
	};
	int uncg_address[MSDC_MAX_NUM] = {
		MSDC0_CLOCK_UNGATE_REG, MSDC1_CLOCK_UNGATE_REG,
		MSDC2_CLOCK_UNGATE_REG, MSDC3_CLOCK_UNGATE_REG
	};
	int cg_bit[MSDC_MAX_NUM] = {
		MSDC0_CLOCK_CG, MSDC1_CLOCK_CG,
		MSDC2_CLOCK_CG, MSDC3_CLOCK_CG
	};
	MSG(CFG, "[SD%d] Turn %s %s clock \n", host->id, on ? "on" : "off", "host");

	if (on)
		MSDC_SET_FIELD(uncg_address[host->id], cg_bit[host->id], 1);
	else
		MSDC_SET_FIELD(cg_address[host->id], cg_bit[host->id], 1);
#endif
#endif /* end of FPGA_PLATFORM */
}

/* perloader will pre-set msdc pll and the mux channel of msdc pll */
/* note: pll will not changed */
void msdc_config_clksrc(struct mmc_host *host, u8 clksrc)
{
	// modify the clock
#if !defined(FPGA_PLATFORM)
	if (host->id == 0) {
		msdc_src_clks = hclks_msdc50;
	} else {
		msdc_src_clks = hclks_msdc30;
	}

#if defined(MMC_MSDC_DRV_PRELOADER) || defined(MMC_MSDC_DRV_LK)
	/* Perloader and LK use 200 is ok, no need change source */
	host->pll_mux_clk = MSDC50_CLKSRC_DEFAULT;
	host->src_clk = msdc_src_clks[MSDC50_CLKSRC_DEFAULT];
#endif

#if defined(MMC_MSDC_DRV_CTP)
	if (host->card && mmc_card_hs400(host->card)) {
		/* if card is in hs400 mode, force clk source to be 400MHz */
		clksrc = MSDC50_CLKSRC_400MHZ;
	}
	host->pll_mux_clk = clksrc;
	host->src_clk    = msdc_src_clks[clksrc];

	/* The following section shall be confirmed by clock owner during porting */
	if (host->id == 0) {
		MSDC_SET_FIELD((TOPCKGEN_BASE + 0x0070), 0xf << 16, host->pll_mux_clk);
	} else if (host->id == 1) {
		MSDC_SET_FIELD((TOPCKGEN_BASE + 0x0070), 0xf << 24, host->pll_mux_clk);
	} else if (host->id == 2) {
		MSDC_SET_FIELD((TOPCKGEN_BASE + 0x0080), 0xf << 0, host->pll_mux_clk);
	} else if (host->id == 3) {
		MSDC_SET_FIELD((TOPCKGEN_BASE + 0x0080), 0xf << 8, host->pll_mux_clk);
	}
	MSDC_WRITE32(TOPCKGEN_BASE+0x04, 0x07FFFFFF);
#endif
#else
	host->src_clk    = msdc_src_clks[clksrc];
#endif
	msdc_printf("[info][%s] input clock is %dkHz\n", __func__, host->src_clk/1000);
}

#if !defined(FPGA_PLATFORM)
void msdc_dump_clock_sts(struct mmc_host *host)
{
	//FIX ME
#ifdef MTK_MSDC_BRINGUP_DEBUG
	msdc_printf(" msdcpll en[0x1000C250][bit0 should be 0x1]=0x%x\n", MSDC_READ32(0x1000C250));
	msdc_printf(" msdcpll on[0x1000C25C][bit0~1 should be 0x1]=0x%x\n", MSDC_READ32(0x1000C25C));

	msdc_printf(" pdn, mux[0x10000070][bit23, 16~19 for msdc0, bit31, 16~19 for msdc1]=0x%x\n", MSDC_READ32(0x10000070));
#endif
}
#endif

/**************************************************************/
/* Section 3: GPIO and Pad                    */
/**************************************************************/
#if !defined(FPGA_PLATFORM)
void msdc_dump_padctl_by_id(u32 id)
{
	switch (id) {
		case 0:
			msdc_printf("MSDC0 MODE18[0x%x] =0x%x\tshould:0x12--	----\n",
			            MSDC0_GPIO_MODE18, MSDC_READ32(MSDC0_GPIO_MODE18));
			msdc_printf("MSDC0 MODE19[0x%x] =0x%x\tshould:0x1249	1249\n",
			            MSDC0_GPIO_MODE19, MSDC_READ32(MSDC0_GPIO_MODE19));
			msdc_printf("MSDC0 IES   [0x%x] =0x%x\tshould:0x----	--1f\n",
			            MSDC0_GPIO_IES_ADDR, MSDC_READ32(MSDC0_GPIO_IES_ADDR));
			msdc_printf("MSDC0 SMT   [0x%x] =0x%x\tshould:0x----	--1f\n",
			            MSDC0_GPIO_SMT_ADDR, MSDC_READ32(MSDC0_GPIO_SMT_ADDR));
			msdc_printf("MSDC0 TDSEL [0x%x] =0x%x\tshould:0x---0	0000\n",
			            MSDC0_GPIO_TDSEL_ADDR,
			            MSDC_READ32(MSDC0_GPIO_TDSEL_ADDR));
			msdc_printf("MSDC0 RDSEL [0x%x] =0x%x\tshould:0x0000	0000\n",
			            MSDC0_GPIO_RDSEL_ADDR,
			            MSDC_READ32(MSDC0_GPIO_RDSEL_ADDR));
			msdc_printf("MSDC0 SR	   [0x%x] =0x%x\n",
			            MSDC0_GPIO_SR_ADDR,
			            MSDC_READ32(MSDC0_GPIO_SR_ADDR));
			msdc_printf("MSDC0 DRV   [0x%x] =0x%x\n",
			            MSDC0_GPIO_DRV_ADDR,
			            MSDC_READ32(MSDC0_GPIO_DRV_ADDR));
			msdc_printf("MSDC0 PULL0 [0x%x] =0x%x\n",
			            MSDC0_GPIO_PUPD0_ADDR,
			            MSDC_READ32(MSDC0_GPIO_PUPD0_ADDR));
			msdc_printf("P-NONE: 0x4444 4444, PU:0x2111 1161, PD:0x6666 6666\n");
			msdc_printf("MSDC0 PULL1 [0x%x] =0x%x\n",
			            MSDC0_GPIO_PUPD1_ADDR,
			            MSDC_READ32(MSDC0_GPIO_PUPD1_ADDR));
			msdc_printf("P-NONE: 0x---- 4444, PU:0x---- 6111, PD:0x---- 6666\n");
			break;
#if (MSDC_MAX_NUM>=1)
		case 1:
			msdc_printf("MSDC1 MODE4 [0x%x] =0x%x\tshould:0x---1 1249\n",
			            MSDC1_GPIO_MODE4, MSDC_READ32(MSDC1_GPIO_MODE4));
			msdc_printf("MSDC1 IES   [0x%x] =0x%x\tshould:0x----   -7--\n",
			            MSDC1_GPIO_IES_ADDR, MSDC_READ32(MSDC1_GPIO_IES_ADDR));
			msdc_printf("MSDC1 SMT   [0x%x] =0x%x\tshould:0x----   -7--\n",
			            MSDC1_GPIO_SMT_ADDR, MSDC_READ32(MSDC1_GPIO_SMT_ADDR));
			msdc_printf("MSDC1 TDSEL [0x%x] =0x%x\n",
			            MSDC1_GPIO_TDSEL_ADDR,
			            MSDC_READ32(MSDC1_GPIO_TDSEL_ADDR));
			//FIX ME: Check why sleep shall set as F
			msdc_printf("should 1.8v: sleep:0x---- -FFF,	awake:0x---- -AAA\n");
			msdc_printf("MSDC1 RDSEL0[0x%x] =0x%x\n",
			            MSDC1_GPIO_RDSEL0_ADDR,
			            MSDC_READ32(MSDC1_GPIO_RDSEL0_ADDR));
			msdc_printf("1.8V: 0x-000 ----, 3.3v: 0x-30C	----\n");
			msdc_printf("MSDC1 RDSEL1[0x%x] =0x%x\n",
			            MSDC1_GPIO_RDSEL1_ADDR,
			            MSDC_READ32(MSDC1_GPIO_RDSEL1_ADDR));
			msdc_printf("should 1.8V: 0x-000 ----, 3.3v:	0x---- ---C\n");
			msdc_printf("MSDC1 SR    [0x%x] =0x%x\n",
			            MSDC1_GPIO_SR_ADDR, MSDC_READ32(MSDC1_GPIO_SR_ADDR));
			msdc_printf("MSDC1 DRV   [0x%x] =0x%x\n",
			            MSDC1_GPIO_DRV_ADDR, MSDC_READ32(MSDC1_GPIO_DRV_ADDR));
			msdc_printf("MSDC1 PULL  [0x%x] =0x%x\n",
			            MSDC1_GPIO_PUPD_ADDR,
			            MSDC_READ32(MSDC1_GPIO_PUPD_ADDR));
			msdc_printf("P-NONE:	0x--44 4444, PU:0x--22 2262, PD:0x--66 6666\n");
			break;
#endif
#if (HOST_MAX_NUM>=2)
		case 2:
			msdc_printf("MSDC2 MODE14[0x%x] =0x%x\tshould:0x--12 2492\n",
			            MSDC2_GPIO_MODE14, MSDC_READ32(MSDC2_GPIO_MODE14));
			msdc_printf("MSDC2 IES   [0x%x] =0x%x\tshould:0x----   -7--\n",
			            MSDC2_GPIO_IES_ADDR, MSDC_READ32(MSDC2_GPIO_IES_ADDR));
			msdc_printf("MSDC2 SMT   [0x%x] =0x%x\tshould:0x----   -7--\n",
			            MSDC2_GPIO_SMT_ADDR, MSDC_READ32(MSDC2_GPIO_SMT_ADDR));
			msdc_printf("MSDC2 TDSEL [0x%x] =0x%x\n",
			            MSDC2_GPIO_TDSEL_ADDR,
			            MSDC_READ32(MSDC2_GPIO_TDSEL_ADDR));
			/* FIX ME: Check why sleep shall set as F */
			msdc_printf("should 1.8v: sleep:0x---- -FFF, awake:0x---- -AAA\n");
			msdc_printf("MSDC2 RDSEL0[0x%x] =0x%x\n",
			            MSDC2_GPIO_RDSEL_ADDR,
			            MSDC_READ32(MSDC2_GPIO_RDSEL_ADDR));
			msdc_printf("1.8V: 0x-000 ----, 3.3v: 0x-30C ----\n");
			msdc_printf("MSDC2 RDSEL1[0x%x] =0x%x\n",
			            MSDC2_GPIO_RDSEL_ADDR,
			            MSDC_READ32(MSDC2_GPIO_RDSEL_ADDR));
			msdc_printf("should 1.8V: 0x-000 ----, 3.3v: 0x---- ---C\n");
			msdc_printf("MSDC2 SR    [0x%x] =0x%x\n",
			            MSDC2_GPIO_SR_ADDR, MSDC_READ32(MSDC2_GPIO_SR_ADDR));
			msdc_printf("MSDC2 DRV   [0x%x] =0x%x\n",
			            MSDC2_GPIO_DRV_ADDR, MSDC_READ32(MSDC2_GPIO_DRV_ADDR));
			msdc_printf("MSDC2 PULL  [0x%x] =0x%x\n",
			            MSDC2_GPIO_PUPD_ADDR,
			            MSDC_READ32(MSDC2_GPIO_PUPD_ADDR));
			msdc_printf("P-NONE: 0x--444, PU:0x--622, PD:0x--666\n");
			break;
#endif
	}
}

void msdc_set_ies_by_id(u32 id, int set_ies)
{
	switch (id) {
		case 0:
			MSDC_SET_FIELD(MSDC0_GPIO_IES_ADDR, MSDC0_IES_ALL_MASK,
			               (set_ies ? 0x1F : 0) );
			break;

		case 1:
			MSDC_SET_FIELD(MSDC1_GPIO_IES_ADDR, MSDC1_IES_ALL_MASK,
			               (set_ies ? 0x7 : 0) );
			break;

#if (HOST_MAX_NUM>=2)
		case 2:
			MSDC_SET_FIELD(MSDC2_GPIO_IES_ADDR, MSDC2_IES_ALL_MASK,
			               (set_ies ? 0x7 : 0));
			break;
#endif

		default:
			msdc_printf("[%s] invlalid host->id!\n", __func__);
			break;
	}
}

void msdc_set_smt_by_id(u32 id, int set_smt)
{
	switch (id) {
		case 0:
			MSDC_SET_FIELD(MSDC0_GPIO_SMT_ADDR, MSDC0_SMT_ALL_MASK,
			               (set_smt ? 0x1F : 0) );
			break;
		case 1:
			MSDC_SET_FIELD(MSDC1_GPIO_SMT_ADDR, MSDC1_SMT_ALL_MASK,
			               (set_smt ? 0x7 : 0) );
			break;

#if (HOST_MAX_NUM>=2)
		case 2:
			MSDC_SET_FIELD(MSDC2_GPIO_SMT_ADDR, MSDC2_SMT_ALL_MASK,
			               (set_smt ? 0x7 : 0) );
			break;
#endif

		default:
			msdc_printf("[%s] invlalid host->id!\n", __func__);
			break;
	}
}

void msdc_set_sr_by_id(u32 id, int clk, int cmd, int dat, int rst, int ds)
{
	switch (id) {
		case 0:
			MSDC_SET_FIELD(MSDC0_GPIO_SR_ADDR, MSDC0_SR_CMD_MASK,
			               (cmd != 0));
			MSDC_SET_FIELD(MSDC0_GPIO_SR_ADDR, MSDC0_SR_DSL_MASK,
			               (ds != 0));
			MSDC_SET_FIELD(MSDC0_GPIO_SR_ADDR, MSDC0_SR_CLK_MASK,
			               (clk != 0));
			MSDC_SET_FIELD(MSDC0_GPIO_SR_ADDR, MSDC0_SR_DAT_MASK,
			               (dat != 0));
			MSDC_SET_FIELD(MSDC0_GPIO_SR_ADDR, MSDC0_SR_RSTB_MASK,
			               (rst != 0));
			break;
		case 1:
			MSDC_SET_FIELD(MSDC1_GPIO_SR_ADDR, MSDC1_SR_CMD_MASK,
			               (cmd != 0));
			MSDC_SET_FIELD(MSDC1_GPIO_SR_ADDR, MSDC1_SR_CLK_MASK,
			               (clk != 0));
			MSDC_SET_FIELD(MSDC1_GPIO_SR_ADDR, MSDC1_SR_DAT_MASK,
			               (dat != 0));
			break;

#if (HOST_MAX_NUM>=2)
		case 2:
			MSDC_SET_FIELD(MSDC2_GPIO_SR_ADDR, MSDC2_SR_CMD_MASK,
			               (cmd != 0));
			MSDC_SET_FIELD(MSDC2_GPIO_SR_ADDR, MSDC2_SR_CLK_MASK,
			               (clk != 0));
			MSDC_SET_FIELD(MSDC2_GPIO_SR_ADDR, MSDC2_SR_DAT_MASK,
			               (dat != 0));
			break;
#endif

		default:
			msdc_printf("[%s] invlalid host->id!\n", __func__);
			break;
	}

}

void msdc_set_tdsel_by_id(u32 id, bool sleep, bool sd_18)
{
	switch (id) {
		case 0:
			MSDC_SET_FIELD(MSDC0_GPIO_TDSEL_ADDR, MSDC0_TDSEL_ALL_MASK ,0);
			break;
		case 1:
			MSDC_SET_FIELD(MSDC1_GPIO_TDSEL_ADDR, MSDC1_TDSEL_ALL_MASK,
			               (sleep ? 0xFFF : 0xAAA ) );
			break;

#if (HOST_MAX_NUM>=2)
		case 2:
			MSDC_SET_FIELD(MSDC2_GPIO_TDSEL_ADDR, MSDC2_TDSEL_ALL_MASK ,0);
			break;
#endif

		default:
			msdc_printf("[%s] invlalid host->id!\n", __func__);
			break;
	}
}

void msdc_set_rdsel_by_id(u32 id, bool sleep, bool sd_18)
{
	switch (id) {
		case 0:
			MSDC_SET_FIELD(MSDC0_GPIO_RDSEL_ADDR, MSDC0_RDSEL_ALL_MASK, 0);
			break;
		case 1:
			MSDC_SET_FIELD(MSDC1_GPIO_RDSEL0_ADDR, MSDC1_RDSEL0_ALL_MASK,
			               (sd_18 ? 0 : 0x30C) );
			MSDC_SET_FIELD(MSDC1_GPIO_RDSEL1_ADDR, MSDC1_RDSEL1_ALL_MASK,
			               (sd_18 ? 0 : 0xC) );
			break;

#if (HOST_MAX_NUM>=2)
		case 2:
			MSDC_SET_FIELD(MSDC2_GPIO_RDSEL_ADDR,MSDC2_RDSEL_ALL_MASK, 0);
			break;
#endif

		default:
			msdc_printf("[%s] invlalid host->id!\n", __func__);
			break;
	}
}

/* host can modify from 0-7 */
void msdc_set_driving_by_id(u32 id, struct msdc_cust *msdc_cap, bool sd_18)
{
	switch (id) {
		case 0:
			MSDC_SET_FIELD(MSDC0_GPIO_DRV_ADDR, MSDC0_DRV_CMD_MASK,
			               msdc_cap->cmd_drv);
			MSDC_SET_FIELD(MSDC0_GPIO_DRV_ADDR, MSDC0_DRV_DSL_MASK,
			               msdc_cap->ds_drv);
			MSDC_SET_FIELD(MSDC0_GPIO_DRV_ADDR, MSDC0_DRV_CLK_MASK,
			               msdc_cap->clk_drv);
			MSDC_SET_FIELD(MSDC0_GPIO_DRV_ADDR, MSDC0_DRV_DAT_MASK,
			               msdc_cap->dat_drv);
			MSDC_SET_FIELD(MSDC0_GPIO_DRV_ADDR, MSDC0_DRV_RSTB_MASK,
			               msdc_cap->rst_drv);
			break;
		case 1:
			if (sd_18) {
				MSDC_SET_FIELD(MSDC1_GPIO_DRV_ADDR, MSDC1_DRV_CMD_MASK,
				               msdc_cap->cmd_18v_drv);
				MSDC_SET_FIELD(MSDC1_GPIO_DRV_ADDR, MSDC1_DRV_CLK_MASK,
				               msdc_cap->clk_18v_drv);
				MSDC_SET_FIELD(MSDC1_GPIO_DRV_ADDR, MSDC1_DRV_DAT_MASK,
				               msdc_cap->dat_18v_drv);
			} else {
				MSDC_SET_FIELD(MSDC1_GPIO_DRV_ADDR, MSDC1_DRV_CMD_MASK,
				               msdc_cap->cmd_drv);
				MSDC_SET_FIELD(MSDC1_GPIO_DRV_ADDR, MSDC1_DRV_CLK_MASK,
				               msdc_cap->clk_drv);
				MSDC_SET_FIELD(MSDC1_GPIO_DRV_ADDR, MSDC1_DRV_DAT_MASK,
				               msdc_cap->dat_drv);
			}
			break;

#ifdef CFG_DEV_MSDC2
		case 2:
			MSDC_SET_FIELD(MSDC2_GPIO_DRV_ADDR, MSDC2_DRV_CMD_MASK,
			               hw->cmd_drv);
			MSDC_SET_FIELD(MSDC2_GPIO_DRV_ADDR, MSDC2_DRV_CLK_MASK,
			               hw->clk_drv);
			MSDC_SET_FIELD(MSDC2_GPIO_DRV_ADDR, MSDC2_DRV_DAT_MASK,
			               hw->dat_drv);
			break;
#endif

		default:
			msdc_printf("[%s] invlalid host->id!\n", __func__);
			break;
	}
}

#if defined(MMC_MSDC_DRV_CTP)
void msdc_get_driving_by_id(u32 id, struct msdc_cust *msdc_cap, bool sd_18)
{
	u32 addr;
	u32 bitsfield_clk, bitsfield_cmd, bitsfield_dat;
	switch (id) {
		case 0:
			MSDC_GET_FIELD(MSDC0_GPIO_DRV_ADDR, MSDC0_DRV_CLK_MASK,
			               msdc_cap->clk_drv);
			MSDC_GET_FIELD(MSDC0_GPIO_DRV_ADDR, MSDC0_DRV_CMD_MASK,
			               msdc_cap->cmd_drv);
			MSDC_GET_FIELD(MSDC0_GPIO_DRV_ADDR, MSDC0_DRV_DAT_MASK,
			               msdc_cap->dat_drv);
			break;

		case 1:
			if (sd_18) {
				MSDC_GET_FIELD(MSDC1_GPIO_DRV_ADDR, MSDC1_DRV_CLK_MASK,
				               msdc_cap->clk_18v_drv);
				MSDC_GET_FIELD(MSDC1_GPIO_DRV_ADDR, MSDC1_DRV_CMD_MASK,
				               msdc_cap->cmd_18v_drv);
				MSDC_GET_FIELD(MSDC1_GPIO_DRV_ADDR, MSDC1_DRV_DAT_MASK,
				               msdc_cap->dat_18v_drv);
			} else {
				MSDC_GET_FIELD(MSDC1_GPIO_DRV_ADDR, MSDC1_DRV_CLK_MASK,
				               msdc_cap->clk_drv);
				MSDC_GET_FIELD(MSDC1_GPIO_DRV_ADDR, MSDC1_DRV_CMD_MASK,
				               msdc_cap->cmd_drv);
				MSDC_GET_FIELD(MSDC1_GPIO_DRV_ADDR, MSDC1_DRV_DAT_MASK,
				               msdc_cap->dat_drv);
			}
			break;

		case 2:
			MSDC_GET_FIELD(MSDC2_GPIO_DRV_ADDR, MSDC2_DRV_CLK_MASK,
			               msdc_cap->clk_drv);
			MSDC_GET_FIELD(MSDC2_GPIO_DRV_ADDR, MSDC2_DRV_CMD_MASK,
			               msdc_cap->cmd_drv);
			MSDC_GET_FIELD(MSDC2_GPIO_DRV_ADDR, MSDC2_DRV_DAT_MASK,
			               msdc_cap->dat_drv);
			break;

		default:
			return;
	}
}
#endif

void msdc_set_pin_mode(struct mmc_host *host)
{
#if defined(MMC_MSDC_DRV_CTP)
	switch (host->id) {
		case 0:
			// Switch MSDC1_* to Aux.1 mode
			// MSDC0 178:DAT0, 179:CLK
			/* MSDC0 180:CMD,  181:DAT1, 182:DAT5, 183:DAT6, 184:DAT4,
			     185:RSTB, 186:DAT2, 187:DAT3, 188:DAT7, 189:DSL */
			MSDC_SET_FIELD(MSDC0_GPIO_MODE18, (0x3F<<25),   0x9);
			MSDC_SET_FIELD(MSDC0_GPIO_MODE19, 0xFFFFFFFF,   0x12491249);
			break;

		case 1:
			/* MSDC1 30:CLK,  31:DAT3, 32:CMD, 33:DAT0, 34:DAT2, 35:DAT1 */
			MSDC_SET_FIELD(MSDC1_GPIO_MODE4, 0x0007FFFF, 0x0011249);
			break;

		case 2:
			MSDC_SET_FIELD(MSDC2_GPIO_MODE14, 0x003FFFF8, 0x0022492);
			break;

		default:
			break;
	}
#endif
}

void msdc_pin_config_by_id(u32 id, u32 mode)
{
	switch (id) {
		case 0:
			/*Attention: don't pull CLK high; Don't toggle RST to prevent
			  from entering boot mode */
			if (MSDC_PIN_PULL_NONE == mode) {
				/* high-Z */
				MSDC_SET_FIELD(MSDC0_GPIO_PUPD0_ADDR,
				               MSDC0_PUPD0_MASK, 0x4444444);
				MSDC_SET_FIELD(MSDC0_GPIO_PUPD1_ADDR,
				               MSDC0_PUPD1_MASK,0x4444);
			} else if (MSDC_PIN_PULL_DOWN == mode) {
				/* cmd/clk/dat/(rstb)/dsl:pd-50k*/
				MSDC_SET_FIELD(MSDC0_GPIO_PUPD0_ADDR,
				               MSDC0_PUPD0_MASK, 0x6666666);
				MSDC_SET_FIELD(MSDC0_GPIO_PUPD1_ADDR,
				               MSDC0_PUPD1_MASK, 0x6666);
			} else if (MSDC_PIN_PULL_UP == mode) {
				/* clk/dsl:pd-50k, cmd/dat:pu-10k, (rstb:pu-50k)*/
				MSDC_SET_FIELD(MSDC0_GPIO_PUPD0_ADDR,
				               MSDC0_PUPD0_MASK, 0x1111161);
				MSDC_SET_FIELD(MSDC0_GPIO_PUPD1_ADDR,
				               MSDC0_PUPD1_MASK, 0x6111);
			}
			break;
		case 1:
			if (MSDC_PIN_PULL_NONE == mode) {
				/* high-Z */
				MSDC_SET_FIELD(MSDC1_GPIO_PUPD_ADDR,
				               MSDC1_PUPD_MASK, 0x444444);
			} else if (MSDC_PIN_PULL_DOWN == mode) {
				/* cmd/clk/dat:pd-50k */
				MSDC_SET_FIELD(MSDC1_GPIO_PUPD_ADDR,
				               MSDC1_PUPD_MASK, 0x666666);
			} else if (MSDC_PIN_PULL_UP == mode) {
				/* cmd/dat:pu-50k, clk:pd-50k */
				MSDC_SET_FIELD(MSDC1_GPIO_PUPD_ADDR,
				               MSDC1_PUPD_MASK, 0x222262);
			}
			break;
		case 2:
			if (MSDC_PIN_PULL_NONE == mode) {
				/* high-Z */
				MSDC_SET_FIELD(MSDC2_GPIO_PUPD_ADDR,
				               MSDC2_PUPD_MASK, 0x444);
			} else if (MSDC_PIN_PULL_DOWN == mode) {
				/* cmd/clk/dat:pd-50k */
				MSDC_SET_FIELD(MSDC2_GPIO_PUPD_ADDR,
				               MSDC2_PUPD_MASK, 0x666);
			} else if (MSDC_PIN_PULL_UP == mode) {
				/* cmd/dat:pu-10k, clk:pd-50k, */
				MSDC_SET_FIELD(MSDC2_GPIO_PUPD_ADDR,
				               MSDC2_PUPD_MASK, 0x611);
			}
			break;


		default:
			break;
	}
}
#endif //for #if !defined(FPGA_PLATFORM)


/**************************************************************/
/* Section 4: MISC                        */
/**************************************************************/
#if !defined(FPGA_PLATFORM)
/* make sure the pad is msdc mode */
void msdc_gpio_and_pad_init(struct mmc_host *host)
{
	/* set smt enable */
	msdc_set_smt(host, 1);

	/* set pull enable */
	msdc_pin_config(host, MSDC_PIN_PULL_UP);

	/* set gpio to msdc mode */
	msdc_set_pin_mode(host);

	/* set driving */
	msdc_set_driving(host, &msdc_cap[host->id], (host->cur_pwr == VOL_1800));

	/* set tdsel and rdsel */
	msdc_set_tdsel(host, 0, (host->cur_pwr == VOL_1800));
	msdc_set_rdsel(host, 0, (host->cur_pwr == VOL_1800));

}
#endif
