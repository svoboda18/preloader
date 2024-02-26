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
#include <upmu_hw.h>
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

#define PWR_MASK_CARD   (0x1 << 8)   //Vcc
#define PWR_MASK_VOL_18 (0x1 << 9)   //CLK/CMD/DAT for eMMC/SD +  Vccq for eMMC
#define PWR_MASK_VOL_33 (0x1 << 10)  //CLK/CMD/DAT for eMMC/SD +  Vccq for eMMC
#define PWR_MASK_L4     (0x1 << 11)
#define PWR_MSDC        (PWR_MASK_CARD | PWR_MASK_VOL_18 | PWR_MASK_VOL_33 | PWR_MASK_L4)

//#define FPGA_GPIO_DEBUG
static void msdc_clr_gpio(u32 bits)
{
	u32 l_val = 0;
	char *name;

	switch (bits) {
		case PWR_MASK_CARD:
			name = "card pwr";
			break;
		case PWR_MASK_VOL_18:
			name = "SD/eMMC bus pwer 1.8V";
			break;
		case PWR_MASK_VOL_33:
			name = "SD/eMMC bus pwer 3.3V";
			break;
		case PWR_MASK_L4:
			name = "L4";
			break;
		default:
			msdc_pr_err("[%s:%s]invalid value: 0x%x\n", __FILE__, __func__, bits);
			return;
	}

	MSDC_GET_FIELD(PWR_GPIO_EO, bits, l_val);
	//msdc_pr_info("====%s====%d\n", name, l_val);
	if (0 == l_val) {
		msdc_pr_info("check me! gpio for %s is input\n", name);
		MSDC_SET_FIELD(PWR_GPIO_EO, bits, 1);
	}
	/* check for set before */
	if (bits &  MSDC_READ32(PWR_GPIO)) {
		msdc_pr_info("clear %s\n", name);
		MSDC_SET_FIELD(PWR_GPIO, bits, 0);
		//l_val = MSDC_READ32(PWR_GPIO);
	}

#ifdef FPGA_GPIO_DEBUG
	{
		u32 val = 0;
		val = MSDC_READ32(PWR_GPIO);
		msdc_pr_err("[clr]PWR_GPIO[8-11]:0x%x\n", val);
		val = MSDC_READ32(PWR_GPIO_EO);
		msdc_pr_err("[clr]GPIO_DIR[8-11]	   :0x%x\n", val);
	}
#endif
}

static void msdc_set_gpio(u32 bits)
{
	u32 l_val = 0;
	char *name;

	switch (bits) {
		case PWR_MASK_CARD:
			name = "card pwr";
			break;
		case PWR_MASK_VOL_18:
			name = "SD/eMMC bus pwer 1.8V";
			break;
		case PWR_MASK_VOL_33:
			name = "SD/eMMC bus pwer 3.3V";
			break;
		case PWR_MASK_L4:
			name = "L4";
			break;
		default:
			msdc_pr_err("[%s:%d]invalid value: 0x%x\n", __FILE__, __func__, bits);
			return;
	}

	MSDC_GET_FIELD(PWR_GPIO_EO, bits, l_val);
	//msdc_pr_info("====%s====%d\n", name, l_val);
	if (0 == l_val) {
		msdc_pr_info("check me! gpio for %s is input\n", name);
		MSDC_SET_FIELD(PWR_GPIO_EO, bits, 1);
	}
	/* check for set before */
	if ((bits &  MSDC_READ32(PWR_GPIO))==0) {
		msdc_pr_info("Set %s\n", name);
		MSDC_SET_FIELD(PWR_GPIO, bits, 1);
		//l_val = MSDC_READ32(PWR_GPIO);
	}

#ifdef FPGA_GPIO_DEBUG
	{
		u32 val = 0;
		val = MSDC_READ32(PWR_GPIO);
		msdc_pr_err("[set]PWR_GPIO[8-11]:0x%x\n", val);
		val = MSDC_READ32(PWR_GPIO_EO);
		msdc_pr_err("[set]GPIO_DIR[8-11]	   :0x%x\n", val);
	}
#endif
}
#endif

#if defined(MMC_MSDC_DRV_CTP) && !defined(FPGA_PLATFORM)
u32 hwPowerOn(MSDC_POWER powerId, MSDC_POWER_VOLTAGE powerVolt)
{
	u32 ret;

	switch (powerId) {
		case MSDC_VEMC:
			if (powerVolt == VOL_3000) {
				pmic_config_interface(REG_VEMC_VOSEL, VEMC_VOSEL_3V, MASK_VEMC_VOSEL, SHIFT_VEMC_VOSEL);
				if (EMMC_VOL_ACTUAL != VOL_3000) {
					pmic_config_interface(REG_VEMC_VOSEL_CAL,
						VEMC_VOSEL_CAL_mV(EMMC_VOL_ACTUAL - VOL_3000),
						MASK_VEMC_VOSEL_CAL, SHIFT_VEMC_VOSEL_CAL);
				}
			} else if (powerVolt == VOL_3300) {
				pmic_config_interface(REG_VEMC_VOSEL, VEMC_VOSEL_3V3, MASK_VEMC_VOSEL, SHIFT_VEMC_VOSEL);
			} else {
				msdc_pr_err("Not support to Set VEMC_3V3 power to %d\n", powerVolt);
			}
			pmic_config_interface(REG_VEMC_EN, 1, MASK_VEMC_EN, SHIFT_VEMC_EN);
			break;

		case MSDC_VMC:
			if (powerVolt == VOL_3300 || powerVolt == VOL_3000)
				pmic_config_interface(REG_VMC_VOSEL, VMC_VOSEL_3V, MASK_VMC_VOSEL, SHIFT_VMC_VOSEL);
			else if (powerVolt == VOL_1800)
				pmic_config_interface(REG_VMC_VOSEL, VMC_VOSEL_1V8, MASK_VMC_VOSEL, SHIFT_VMC_VOSEL);
			else
				msdc_pr_err("Not support to Set VMC power to %d\n", powerVolt);

			pmic_config_interface(REG_VMC_EN, 1, MASK_VMC_EN, SHIFT_VMC_EN);
			break;

		case MSDC_VMCH:
			if (powerVolt == VOL_3000) {
				pmic_config_interface(REG_VMCH_VOSEL, VMCH_VOSEL_3V, MASK_VMCH_VOSEL, SHIFT_VMCH_VOSEL);
				if (SD_VOL_ACTUAL != VOL_3000) {
					pmic_config_interface(REG_VMCH_VOSEL_CAL,
						VMCH_VOSEL_CAL_mV(SD_VOL_ACTUAL - VOL_3000),
						MASK_VMCH_VOSEL_CAL, SHIFT_VMCH_VOSEL_CAL);
				}
			} else if (powerVolt == VOL_3300) {
				pmic_config_interface(REG_VMCH_VOSEL, VMCH_VOSEL_3V3, MASK_VMCH_VOSEL, SHIFT_VMCH_VOSEL);
			} else {
				msdc_pr_err("Not support to Set VMCH power to %d\n", powerVolt);
			}
			pmic_config_interface(REG_VMCH_EN, 1, MASK_VMCH_EN, SHIFT_VMCH_EN);
			break;

		default:
			msdc_pr_err("Not support to Set %d power on\n", powerId);
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
			msdc_pr_err("Not support to Set %d power down\n", powerId);
			break;
	}
	return 0;
}

static u32 msdc_ldo_power(u32 on, MSDC_POWER powerId, MSDC_POWER_VOLTAGE powerVolt, u32 *status)
{
	if (on) { // want to power on
		if (*status == 0) {  // can power on
			msdc_pr_info("msdc LDO<%d> power on<%d>\n", powerId, powerVolt);
			hwPowerOn(powerId, powerVolt);
			*status = powerVolt;
		} else if (*status == powerVolt) {
			msdc_pr_info("msdc LDO<%d><%d> power on again!\n", powerId, powerVolt);
		} else { // for sd3.0 later
			msdc_pr_info("msdc LDO<%d> change<%d>	to <%d>\n", powerId, *status, powerVolt);
			hwPowerDown(powerId);
			hwPowerOn(powerId, powerVolt);
			*status = powerVolt;
		}
	} else {  // want to power off
		if (*status != 0) {  // has been powerred on
			msdc_pr_info("msdc LDO<%d> power off\n", powerId);
			hwPowerDown(powerId);
			*status = 0;
		} else {
			msdc_pr_info("LDO<%d>	not power on\n", powerId);
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
				if (level == VOL_1800)
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
			msdc_ldo_power(on, MSDC_VEMC, VOL_3000, &g_msdc0_flash);
			mdelay(10);
			break;
		case 1:
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
	u32 card_on = on;
	if (host->id != 0) {
		host->cur_pwr = level;
	}
	msdc_set_tdsel_wrap(host);
	msdc_set_rdsel_wrap(host);
	msdc_set_driving(host, &msdc_cap[host->id]);

	MSG(CFG, "[SD%d] Turn %s host power \n", host->id, on ? "on" : "off");

	switch (host->id) {
		case 0:
			//no need change;
			break;
		case 1:
			msdc_ldo_power(on, MSDC_VMC, level, &g_msdc1_io);
			mdelay(10);
			break;
		case 3:
			//For MSDC with SD card, not for SDIO
			msdc_ldo_power(on, MSDC_VMC, level, &g_msdc2_io);
			mdelay(10);
			break;
		default:
			break;
	}
#endif
}

void msdc_dump_ldo_sts(struct mmc_host *host)
{
#if defined(MTK_MSDC_BRINGUP_DEBUG)
	u32 ldo_en = 0, ldo_vol = 0;

	pmic_read_interface(REG_VEMC_EN, &ldo_en, MASK_VEMC_EN,
	                    SHIFT_VEMC_EN);
	pmic_read_interface(REG_VEMC_VOSEL, &ldo_vol, MASK_VEMC_VOSEL,
	                    SHIFT_VEMC_VOSEL);
	msdc_pr_err(" VEMC_EN=0x%x, should:1b'1, "
	       "VEMC_VOL=0x%x, should:3b'011(3.0V)\n",
	       ldo_en, ldo_vol);
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


/**************************************************************/
/* Section 2: Clock                    */
/**************************************************************/
#if !defined(FPGA_PLATFORM)
u32 hclks_msdc0[] = { MSDC0_SRC_0, MSDC0_SRC_1};

/* msdc1/2 clock source reference value is 200M */
u32 hclks_msdc1[] = { MSDC1_SRC_0, MSDC1_SRC_1, MSDC1_SRC_2};

u32 hclks_msdc3[] = { MSDC3_SRC_0, MSDC3_SRC_1, MSDC3_SRC_2, MSDC3_SRC_3,
		      MSDC3_SRC_4, MSDC3_SRC_5, MSDC3_SRC_6, MSDC3_SRC_7};

u32 *hclks_msdc_all[] = {
	hclks_msdc0,
	hclks_msdc1,
	hclks_msdc3,
};

void msdc_dump_clock_sts(struct mmc_host *host)
{
#if defined(MTK_MSDC_BRINGUP_DEBUG)
#define MSDC_TOPCKGEN_REG_BASE 0x10000000
#define MSDC_INFRACFG_AO_REG_BASE 0x10001000
	msdc_pr_err(
	" topckgen [0x%x]=0x%x(should bit[1:0]=01b, bit[7]=0, bit[10:8]=001b, bit[15]=0)\n",
		MSDC_TOPCKGEN_REG_BASE + 0x70, MSDC_READ32(MSDC_TOPCKGEN_REG_BASE + 0x70));

	msdc_pr_err(
	" infracfg [0x%x]=0x%x(should bit[2]=0b)\n",
		MSDC_INFRACFG_AO_REG_BASE + 0x94, MSDC_READ32(MSDC_INFRACFG_AO_REG_BASE + 0x94));

	msdc_pr_err(
	" infracfg [0x%x]=0x%x(should bit[9]=0b)\n",
	MSDC_INFRACFG_AO_REG_BASE + 0xc8, MSDC_READ32(MSDC_INFRACFG_AO_REG_BASE + 0xc8));

#endif
}
#endif

void msdc_clock(struct mmc_host *host, int on)
{
	//Only CTP need enable/disable clock
	//Preloader will turn on clock with predefined clock source and module need not to touch clock setting.
	//LK will use preloader's setting and need not to touch clock setting.
#if !defined(FPGA_PLATFORM)
#if defined(MMC_MSDC_DRV_CTP)
	//int clk_id[MSDC_MAX_NUM] = {MT_CG_INFRA_MSDC_0, MT_CG_INFRA_MSDC_1, MT_CG_INFRA_MSDC_2, MT_CG_INFRA_MSDC_3};
	MSG(CFG, "[SD%d] Turn %s %s clock \n", host->id, on ? "on" : "off", "host");

	if (on) {
		//INFRA_enable_clock(clk_id[host->id]);
		if (host->id == 0) {
			MSDC_SET_BIT32(MSDC0_CLOCK_UNGATE_REG, MSDC0_CLOCK_CG_SECURE);
			MSDC_SET_BIT32(MSDC0_CLOCK_UNGATE_REG, MSDC0_CLOCK_CG_NORMAL);
			MSDC_SET_BIT32(MSDC0_CLOCK_UNGATE_REG, MSDC0_CLOCK_CG_MD);
		} else if (host->id == 1)
			MSDC_SET_BIT32(MSDC1_CLOCK_UNGATE_REG, MSDC1_CLOCK_CG);
		else if (host->id == 3)
			MSDC_SET_BIT32(MSDC3_CLOCK_UNGATE_REG, MSDC3_CLOCK_CG);
	} else {
		//INFRA_disable_clock(clk_id[host->id]);
		if (host->id == 0) {
			MSDC_SET_BIT32(MSDC0_CLOCK_GATE_REG, MSDC0_CLOCK_CG_SECURE);
			MSDC_SET_BIT32(MSDC0_CLOCK_GATE_REG, MSDC0_CLOCK_CG_NORMAL);
			MSDC_SET_BIT32(MSDC0_CLOCK_GATE_REG, MSDC0_CLOCK_CG_MD);
		} else if (host->id == 1)
			MSDC_SET_BIT32(MSDC1_CLOCK_GATE_REG, MSDC1_CLOCK_CG);
		else if (host->id == 3)
			MSDC_SET_BIT32(MSDC3_CLOCK_GATE_REG, MSDC3_CLOCK_CG);
	}
#endif

	msdc_dump_clock_sts(host);

#endif
}

/* perloader will pre-set msdc pll and the mux channel of msdc pll */
/* note: pll will not changed */
void msdc_config_clksrc(struct mmc_host *host, int clksrc)
{
#if !defined(FPGA_PLATFORM)
	#if defined(MMC_MSDC_DRV_PRELOADER) || defined(MMC_MSDC_DRV_LK)
	host->pll_mux_clk = MSDC0_CLKSRC_DEFAULT;
	host->src_clk = msdc_get_hclk(host->id, MSDC0_CLKSRC_DEFAULT);
	#endif

	#if defined(MMC_MSDC_DRV_CTP)
	if (host->card && mmc_card_hs400(host->card)) {
		/* if card is in hs400 mode, force clk source to be 400MHz */
		clksrc = MSDC0_CLKSRC_400MHZ;
	}
	host->pll_mux_clk = clksrc;
	host->src_clk = msdc_get_hclk(host->id, clksrc);

	/* The following section shall be confirmed by clock owner during porting */
	if (host->id == 0) {
		MSDC_SET_FIELD((TOPCKGEN_BASE + 0x0140), 0x7 << 16, host->pll_mux_clk);
	} else if (host->id == 1) {
		MSDC_SET_FIELD((TOPCKGEN_BASE + 0x0140), 0x7 << 24, host->pll_mux_clk);
	} else if (host->id == 3) {
		MSDC_SET_FIELD((TOPCKGEN_BASE + 0x0150), 0x7 << 8, host->pll_mux_clk);
	}
	MSDC_WRITE32(TOPCKGEN_BASE+0x04, 0x07FFFFFF);
	#endif
#else
	host->src_clk = msdc_get_hclk(host->id, clksrc);
#endif

}



/**************************************************************/
/* Section 3: GPIO and Pad                    */
/**************************************************************/
#if !defined(FPGA_PLATFORM)
void msdc_dump_padctl_by_id(u32 id)
{
#if defined(MTK_MSDC_BRINGUP_DEBUG)
	if (id == 0) {
		msdc_pr_err(
		"MSDC0_GPIO_MODE_TRAP[0x%x] =0x%x\t\n",
			MSDC0_GPIO_MODE_TRAP, MSDC_READ32(MSDC0_GPIO_MODE_TRAP));

		if (MSDC_READ32(MSDC0_GPIO_MODE_TRAP) & (0x3 << 13)) {
			msdc_pr_err(
			"MSDC0 GPIO0[0x%x] =0x%x\tshould:32'b.001.001 .001.001 .001.001 ........\n",
				MSDC0_GPIO_MODE0, MSDC_READ32(MSDC0_GPIO_MODE0));
			msdc_pr_err(
			"MSDC0 GPIO1 [0x%x] =0x%x\tshould:32'b........ .001.001 .001.001 .001.001\n",
				MSDC0_GPIO_MODE1, MSDC_READ32(MSDC0_GPIO_MODE1));
		} else {
			msdc_pr_err(
			"MSDC0 GPIO0[0x%x] =0x%x\tshould:32'b.010.010 .010.010 .010.010 ........\n",
				MSDC0_GPIO_MODE0, MSDC_READ32(MSDC0_GPIO_MODE0));
			msdc_pr_err(
			"MSDC0 GPIO1 [0x%x] =0x%x\tshould:32'b........ .010.010 .010.010 .010.010\n",
				MSDC0_GPIO_MODE1, MSDC_READ32(MSDC0_GPIO_MODE1));
		}
		msdc_pr_err(
		"MSDC0 SMT   [0x%x] =0x%x\tshould:32'b........ ........ ...11111 1111111.\n",
			MSDC0_GPIO_SMT, MSDC_READ32(MSDC0_GPIO_SMT));
		msdc_pr_err(
		"MSDC0 IES   [0x%x] =0x%x\tshould:32'b........ ....1111 11111111 ........\n",
			MSDC0_GPIO_IES, MSDC_READ32(MSDC0_GPIO_IES));

		msdc_pr_err(
		"MSDC0 PUPD [0x%x] =0x%x\tshould:32'b........ ........ ....0100 00000001\n",
			MSDC0_GPIO_PUPD, MSDC_READ32(MSDC0_GPIO_PUPD));
		msdc_pr_err(
		"MSDC0 R0 [0x%x] =0x%x\tshould:32'b........ ........ ....1011 11111110\n",
			MSDC0_GPIO_R0, MSDC_READ32(MSDC0_GPIO_R0));
		msdc_pr_err(
		"MSDC0 R1 [0x%x] =0x%x\tshould:32'b........ ........ ....0100 00000001\n",
			MSDC0_GPIO_R1, MSDC_READ32(MSDC0_GPIO_R1));

		msdc_pr_err(
		"MSDC0 TDSEL [0x%x] =0x%x\tshould:32'b........ 00000000 00000000 0000....\n",
			MSDC0_GPIO_TDSEL, MSDC_READ32(MSDC0_GPIO_TDSEL));
		msdc_pr_err(
		"MSDC0 RDSEL [0x%x] =0x%x\tshould:32'b....0000 00000000 00000000 0000....\n",
			MSDC0_GPIO_RDSEL, MSDC_READ32(MSDC0_GPIO_RDSEL));

		msdc_pr_err(
		"MSDC0 DRV   [0x%x] =0x%x\tshould: 32'b........ ...00100 10010010 01......\n",
			MSDC0_GPIO_DRV, MSDC_READ32(MSDC0_GPIO_DRV));
	}
	else if (id == 1) {
	} else if (id == 3) {
	}

#endif
}

void msdc_set_pin_mode(struct mmc_host *host)
{
	if (host->id == 0) {
		if ((MSDC_READ32(MSDC0_GPIO_MODE_TRAP) & (0x3 << 13)) == 0) {
			MSDC_SET_FIELD(MSDC0_GPIO_MODE0, 0x777777 << 8, 0x222222);
			MSDC_SET_FIELD(MSDC0_GPIO_MODE1, 0x777777 << 0, 0x222222);
		} else {
			MSDC_SET_FIELD(MSDC0_GPIO_MODE0, 0x777777 << 8, 0x111111);
			MSDC_SET_FIELD(MSDC0_GPIO_MODE1, 0x777777 << 0, 0x111111);
		}

	} else if (host->id == 1) {
	} else if (host->id == 3) {
	}
}

void msdc_set_ies_by_id(u32 id, int set_ies)
{
	if (id == 0) {
	} else if (id == 1) {
	} else if (id == 3) {
	}
}

void msdc_set_smt_by_id(u32 id, int set_smt)
{
	if (id == 0) {
		/*
		* 1. enable SMT
		*/
	} else if (id == 1) {
	} else if (id == 3) {
	}
}

void msdc_set_tdsel_by_id(u32 id, u32 flag, u32 value)
{
	if (id == 0) {
	} else if (id == 1) {
	} else if (id == 3) {
	}
}


void msdc_set_rdsel_by_id(u32 id, u32 flag, u32 value)
{
	if (id == 0) {
	} else if (id == 1) {
	} else if (id == 3) {
	}
}


void msdc_set_sr_by_id(u32 id, int clk, int cmd, int dat, int rst, int ds)
{
	if (id == 0) {
	} else if (id == 1) {
	} else if (id == 3) {
	}
}

void msdc_set_driving_by_id(u32 id, struct msdc_cust *msdc_cap)
{
	switch (id) {
		case 0:
			MSDC_SET_FIELD(MSDC0_GPIO_DRV, 0x7 << 15,
			msdc_cap->ds_drv);
			MSDC_SET_FIELD(MSDC0_GPIO_DRV, 0x7 << 18,
				msdc_cap->rst_drv);
			MSDC_SET_FIELD(MSDC0_GPIO_DRV, 0x7 << 9,
				msdc_cap->cmd_drv);
			MSDC_SET_FIELD(MSDC0_GPIO_DRV, 0x7 << 6,
				msdc_cap->clk_drv);
			MSDC_SET_FIELD(MSDC0_GPIO_DRV, 0x7 << 12,
				msdc_cap->dat_drv);
			break;
		case 1:
		default:
			break;
	}
}

/* msdc pin config
 * MSDC0
 * PUPD/RO/R1
 * 0/0/0: High-Z
 * 0/0/1: Pull-up with 50Kohm
 * 0/1/0: Pull-up with 10Kohm
 * 0/1/1: Pull-up with 10Kohm//50Kohm
 * 1/0/0: High-Z
 * 1/0/1: Pull-down with 50Kohm
 * 1/1/0: Pull-down with 10Kohm
 * 1/1/1: Pull-down with 10Kohm//50Kohm
 */
void msdc_pin_config_by_id(u32 id, u32 mode)
{
	if (id == 0) {
	} else if (id == 1) {
	} else if (id == 3) {
	}
}


/**************************************************************/
/* Section 4: MISC                        */
/**************************************************************/
/* make sure the pad is msdc mode */
#if !defined(FPGA_PLATFORM)
void msdc_set_tdsel_wrap(struct mmc_host *host)
{
	if (host->cur_pwr == VOL_1800)
		msdc_set_tdsel_by_id(host->id, MSDC_TDRDSEL_1V8, 0);
	else
		msdc_set_tdsel_by_id(host->id, MSDC_TDRDSEL_3V, 0);
}

void msdc_set_rdsel_wrap(struct mmc_host *host)
{
	if (host->cur_pwr == VOL_1800)
		msdc_set_rdsel_by_id(host->id, MSDC_TDRDSEL_1V8, 0);
	else
		msdc_set_rdsel_by_id(host->id, MSDC_TDRDSEL_3V, 0);
}

void msdc_gpio_and_pad_init(struct mmc_host *host)
{
	/* set smt enable */
	msdc_set_smt(host, 1);

	/* set pull enable */
	msdc_pin_config(host, MSDC_PIN_PULL_UP);

	/* set gpio to msdc mode */
	msdc_set_pin_mode(host);

	/* set driving */
	msdc_set_driving(host, &msdc_cap[host->id]);

	/* set tdsel and rdsel */
	msdc_set_tdsel_wrap(host);
	msdc_set_rdsel_wrap(host);

	msdc_dump_padctl_by_id(host->id);
}
#endif
#endif
#if defined(SLT) || defined(MMC_MSDC_DRV_CTP)
/* copy from autok.c */
#define AUTOK_CKGEN_VALUE                       (0)
#define AUTOK_CMD_LATCH_EN_HS400_PORT0_VALUE    (2)
#define AUTOK_CRC_LATCH_EN_HS400_PORT0_VALUE    (2)
#define AUTOK_CMD_LATCH_EN_DDR208_PORT3_VALUE   (2)
#define AUTOK_CRC_LATCH_EN_DDR208_PORT3_VALUE   (2)
#define AUTOK_CMD_LATCH_EN_HS200_PORT0_VALUE    (1)
#define AUTOK_CRC_LATCH_EN_HS200_PORT0_VALUE    (1)
#define AUTOK_CMD_LATCH_EN_SDR104_PORT1_VALUE   (1)
#define AUTOK_CRC_LATCH_EN_SDR104_PORT1_VALUE   (1)
#define AUTOK_CMD_LATCH_EN_HS_VALUE             (1)
#define AUTOK_CRC_LATCH_EN_HS_VALUE             (1)

void msdc_apply_timing(unsigned int id)
{
	struct mmc_host *host;
	msdc_priv_t *priv;
	int mode;
	u32 base;
	u32 hz, div;

	host = mmc_get_host(id);
	priv = host->priv;
	base = host->base;
	hz = host->cur_bus_clk;

	MSDC_GET_FIELD(MSDC_CFG, MSDC_CFG_CKMOD, mode);
	MSDC_GET_FIELD(MSDC_CFG, MSDC_CFG_CKDIV, div);

	if ((id == 0) && (mode == 3)) {
		MSDC_SET_FIELD(host->base + 0x4, (0x1 << 1), 0);	/* MSDC_IOCON[MSDC_IOCON_RSPL] */
		MSDC_SET_FIELD(host->base + 0x4, (0x1 << 2), 0);        /* MSDC_IOCON[MSDC_IOCON_R_D_SMPL] */

		MSDC_SET_FIELD(host->base + 0xb4, (0xf << 8), 3);       /* MSDC_PB1_STOP_DLY_SEL */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 20), 1);      /* MSDC_PB1_POP_MARK_WATER */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 19), 1);      /* MSDC_PB1_STATE_CLR */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 18), 1);      /* Reserved bit */

		MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 29), 0);      /* MSDC_PB2_CRCSTSENSEL*/
		MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 16), 0);      /* MSDC_PB2_CFG_CRCSTS_SEL*/
		//MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 29), AUTOK_CRC_LATCH_EN_HS400_PORT0_VALUE);
		//MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 16), AUTOK_CMD_LATCH_EN_HS400_PORT0_VALUE);

		MSDC_SET_FIELD(host->base + 0x208, (0x1 << 3), 0);      /* MSDC_PB2_CFGCRCSTSEDGE, write crc stage edge */
		MSDC_SET_FIELD(host->base + 0x208, (0x7 << 21), 1);     /* MSDC_EMMC50_CFG_READ_DAT_CNT */
		MSDC_SET_FIELD(host->base + 0x228, (0x1 << 24), 0);     /* SDC_FIFO_CFG_WR_VALID_SEL */
		MSDC_SET_FIELD(host->base + 0x228, (0x1 << 25), 0);     /* SDC_FIFO_CFG_RD_VALID_SEL */


		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x0, (0x1 << 13), 0x0);	/* EMMC_TOP_CONTROL[PAD_DAT_RD_RXDLY_SEL] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x0, (0x1f << 7), 0x0);	/* EMMC_TOP_CONTROL[PAD_DAT_RD_RXDLY] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x0, (0x1 << 12), 0x0);	/* EMMC_TOP_CONTROL[PAD_DAT_RD_RXDLY2_SEL] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x0, (0x1f << 2), 0x0);	/* EMMC_TOP_CONTROL[PAD_DAT_RD_RXDLY2] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x0, (0x1 << 15), 0x1); /* EMMC_TOP_CONTROL[SDC_RX_ENH_EN] */

		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x4, (0x1 << 11), 0x1);	/* EMMC_TOP_CMD[PAD_CMD_RXDLY] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x4, (0x1f << 5), 0x9);	/* EMMC_TOP_CMD[PAD_CMD_RXDLY_SEL] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x4, (0x1 << 10), 0x0);	/* EMMC_TOP_CMD[PAD_CMD_RXDLY2] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x4, (0x1f << 0), 0x0);	/* EMMC_TOP_CMD[PAD_CMD_RXDLY2_SEL] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x4, (0x1f << 12), 0x0);	/* EMMC_TOP_CMD_LEVEL1[PAD_CMD_TX_DLY] */

		MSDC_WRITE32(MSDC0_TOP_BASE + 0x8, 0x3);	        /* EMMC50_PAD_CTL0 */

		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0xC, (0x1 << 16), 0x1);	/* EMMC50_PAD_DS_TUNE[PAD_DS_DLY_SEL] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0xC, (0x1f << 10), 0xA);	/* EMMC50_PAD_DS_TUNE[PAD_DS_DLY1] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0xC, (0x1 << 15), 0x0);	/* EMMC50_PAD_DS_TUNE[PAD_DS_DLY2_SEL] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0xC, (0x1 << 5), 0x0);	/* EMMC50_PAD_DS_TUNE[PAD_DS_DLY2] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0xC, (0x1f << 0), 0xC);	/* EMMC50_PAD_DS_TUNE[PAD_DS_DLY3] */

	} else if ((id == 0) && (div == 0)) {
		MSDC_SET_FIELD(host->base + 0x4, (0x1 << 1), 0x0);	/* MSDC_IOCON[MSDC_IOCON_RSPL] */
		MSDC_SET_FIELD(host->base + 0x4, (0x1 << 2), 0x0);      /* MSDC_IOCON[MSDC_IOCON_R_D_SMPL] */

		MSDC_SET_FIELD(host->base + 0xb0, (0x1 << 3), 0);       /* MSDC_PB1_STOP_DLY_SEL */

		MSDC_SET_FIELD(host->base + 0xb4, (0xf << 8), 3);       /* MSDC_PB1_STOP_DLY_SEL */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 20), 1);      /* MSDC_PB1_POP_MARK_WATER */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 19), 1);      /* MSDC_PB1_STATE_CLR */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 18), 1);      /* Reserved bit */

		MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 29), 0);
		MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 16), 0);
		//MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 29), AUTOK_CRC_LATCH_EN_HS200_PORT0_VALUE);
		//MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 16), AUTOK_CMD_LATCH_EN_HS200_PORT0_VALUE);
		MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 28), 1);
		MSDC_SET_FIELD(host->base + 0xb8, (0x1 << 25), 0);      /* MSDC_PB2_CFGCRCSTSEDGE, write crc stage edge */

		MSDC_SET_FIELD(host->base + 0x208, (0x1 << 3), 0);      /* MSDC_PB2_CFGCRCSTSEDGE, write crc stage edge */
		MSDC_SET_FIELD(host->base + 0x208, (0x7 << 21), 7);     /* MSDC_EMMC50_CFG_READ_DAT_CNT */

		MSDC_SET_FIELD(host->base + 0x228, (0x1 << 24), 0);     /* SDC_FIFO_CFG_WR_VALID_SEL */
		MSDC_SET_FIELD(host->base + 0x228, (0x1 << 25), 0);     /* SDC_FIFO_CFG_RD_VALID_SEL */

		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x0, (0x1 << 13), 0x1);	/* EMMC_TOP_CONTROL[PAD_DAT_RD_RXDLY_SEL] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x0, (0x1f << 7), 0x4);	/* EMMC_TOP_CONTROL[PAD_DAT_RD_RXDLY] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x0, (0x1 << 12), 0x0);	/* EMMC_TOP_CONTROL[PAD_DAT_RD_RXDLY2_SEL] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x0, (0x1f << 2), 0x0);	/* EMMC_TOP_CONTROL[PAD_DAT_RD_RXDLY2] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x0, (0x1 << 15), 0x1); /* EMMC_TOP_CONTROL[SDC_RX_ENH_EN] */

		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x4, (0x1 << 11), 0x1);	/* EMMC_TOP_CMD[PAD_CMD_RXDLY] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x4, (0x1f << 5), 0x7);	/* EMMC_TOP_CMD[PAD_CMD_RXDLY_SEL] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x4, (0x1 << 10), 0x0);	/* EMMC_TOP_CMD[PAD_CMD_RXDLY2] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x4, (0x1f << 0), 0x0);	/* EMMC_TOP_CMD[PAD_CMD_RXDLY2_SEL] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x4, (0x1f << 12), 0x0);	/* EMMC_TOP_CMD_LEVEL1[PAD_CMD_TX_DLY] */

		MSDC_WRITE32(MSDC0_TOP_BASE + 0x8, 0x3);	        /* EMMC50_PAD_CTL0 */

		MSDC_WRITE32(MSDC0_TOP_BASE + 0xC, 0x0);	        /* EMMC50_PAD_DS_TUNE */
	} else if ((id == 0) && (mode == 2)) {
		MSDC_SET_FIELD(host->base + 0x4, (0x1 << 4), 0x1);	/* MSDC_IOCON[MSDC_IOCON_RSPL] */

		MSDC_SET_FIELD(host->base + 0xb4, (0xf << 8), 3);       /* MSDC_PB1_STOP_DLY_SEL */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 20), 1);      /* MSDC_PB1_POP_MARK_WATER */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 19), 1);      /* MSDC_PB1_STATE_CLR */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 18), 1);      /* Reserved bit */

		MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 29), AUTOK_CRC_LATCH_EN_HS_VALUE);
		MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 16), AUTOK_CMD_LATCH_EN_HS_VALUE);
		MSDC_SET_FIELD(host->base + 0xb8, (0x1 << 28), 1);
		MSDC_SET_FIELD(host->base + 0xb8, (0x1 << 25), 0);      /* MSDC_PB2_CFGCRCSTSEDGE, write crc stage edge */

		MSDC_SET_FIELD(host->base + 0x208, (0x1 << 3), 0);      /* MSDC_PB2_CFGCRCSTSEDGE, write crc stage edge */
		MSDC_SET_FIELD(host->base + 0x208, (0x7 << 21), 7);     /* MSDC_EMMC50_CFG_READ_DAT_CNT */

		MSDC_SET_FIELD(host->base + 0x228, (0x1 << 24), 0);     /* SDC_FIFO_CFG_WR_VALID_SEL */
		MSDC_SET_FIELD(host->base + 0x228, (0x1 << 25), 0);     /* SDC_FIFO_CFG_RD_VALID_SEL */

		MSDC_WRITE32(MSDC0_TOP_BASE + 0x0, 0x0);	        /* EMMC50_PAD_DS_TUNE */
		MSDC_WRITE32(MSDC0_TOP_BASE + 0x4, 0x0);	        /* EMMC50_PAD_DS_TUNE */
		MSDC_WRITE32(MSDC0_TOP_BASE + 0x8, 0x0);	        /* EMMC50_PAD_DS_TUNE */
		MSDC_WRITE32(MSDC0_TOP_BASE + 0xC, 0x0);	        /* EMMC50_PAD_DS_TUNE */
	} else if (id == 0) {
		MSDC_SET_FIELD(host->base + 0x4, (0x1 << 4), 0x0);	/* MSDC_IOCON[MSDC_IOCON_RSPL] */

		MSDC_SET_FIELD(host->base + 0x228, (0x1 << 24), 0);     /* SDC_FIFO_CFG_WR_VALID_SEL */
		MSDC_SET_FIELD(host->base + 0x228, (0x1 << 25), 0);     /* SDC_FIFO_CFG_RD_VALID_SEL */

		MSDC_SET_FIELD(host->base + 0xb4, (0xf << 8), 3);       /* MSDC_PB1_STOP_DLY_SEL */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 20), 1);      /* MSDC_PB1_POP_MARK_WATER */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 19), 1);      /* MSDC_PB1_STATE_CLR */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 18), 1);      /* Reserved bit */

		MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 29), AUTOK_CRC_LATCH_EN_HS_VALUE);
		MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 16), AUTOK_CMD_LATCH_EN_HS_VALUE);
		MSDC_SET_FIELD(host->base + 0xb8, (0x1 << 28), 1);
		MSDC_SET_FIELD(host->base + 0xb8, (0x1 << 25), 0);      /* MSDC_PB2_CFGCRCSTSEDGE, write crc stage edge */

		MSDC_WRITE32(MSDC0_TOP_BASE + 0x0, 0x0);	        /* EMMC50_PAD_DS_TUNE */
		MSDC_WRITE32(MSDC0_TOP_BASE + 0x4, 0x0);	        /* EMMC50_PAD_DS_TUNE */
		MSDC_WRITE32(MSDC0_TOP_BASE + 0x8, 0x0);	        /* EMMC50_PAD_DS_TUNE */
		MSDC_WRITE32(MSDC0_TOP_BASE + 0xC, 0x0);	        /* EMMC50_PAD_DS_TUNE */
	} else if ((id == 1) && (mode == 1)) {
		MSDC_SET_FIELD(host->base + 0x4, (0x1 << 1), 0x0);	/*MSDC_IOCON[MSDC_IOCON_RSPL] */

		MSDC_SET_FIELD(host->base + 0xb0, (0x1 << 3), 0x0);

		MSDC_SET_FIELD(host->base + 0xb4, (0xf << 8), 3);       /* MSDC_PB1_STOP_DLY_SEL */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 20), 1);      /* MSDC_PB1_POP_MARK_WATER */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 19), 1);      /* MSDC_PB1_STATE_CLR */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 18), 1);      /* Reserved bit */

		MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 29), 0);
		MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 16), 0);
		MSDC_SET_FIELD(host->base + 0xb8, (0x1 << 28), 1);
		MSDC_SET_FIELD(host->base + 0xb8, (0x1 << 25), 0x0);

		MSDC_SET_FIELD(host->base + 0x228, (0x1 << 24), 0);     /* SDC_FIFO_CFG_WR_VALID_SEL */
		MSDC_SET_FIELD(host->base + 0x228, (0x1 << 25), 0);     /* SDC_FIFO_CFG_RD_VALID_SEL */

		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x0, (0x1 << 13), 0x1);	/* EMMC_TOP_CONTROL[PAD_DAT_RD_RXDLY_SEL] */
		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x0, (0x1f << 7), 0x6);	/* EMMC_TOP_CONTROL[PAD_DAT_RD_RXDLY] */
		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x0, (0x1 << 12), 0x0);	/* EMMC_TOP_CONTROL[PAD_DAT_RD_RXDLY2_SEL] */
		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x0, (0x1f << 2), 0x0);	/* EMMC_TOP_CONTROL[PAD_DAT_RD_RXDLY2] */
		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x0, (0x1 << 15), 0x1); /* EMMC_TOP_CONTROL[SDC_RX_ENH_EN] */

		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x4, (0x1 << 11), 0x1);	/* EMMC_TOP_CMD[PAD_CMD_RXDLY] */
		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x4, (0x1f << 5), 0x5);	/* EMMC_TOP_CMD[PAD_CMD_RXDLY_SEL] */
		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x4, (0x1 << 10), 0x0);	/* EMMC_TOP_CMD[PAD_CMD_RXDLY2] */
		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x4, (0x1f << 0), 0x0);	/* EMMC_TOP_CMD[PAD_CMD_RXDLY2_SEL] */
		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x4, (0x1f << 12), 0x0);	/* EMMC_TOP_CMD_LEVEL1[PAD_CMD_TX_DLY] */

		MSDC_WRITE32(MSDC0_TOP_BASE + 0x8, 0x3);	        /* EMMC50_PAD_CTL0 */
		MSDC_WRITE32(MSDC0_TOP_BASE + 0xC, 0x0);	        /* EMMC50_PAD_CTL0 */
	} else if ((id == 1) && (mode == 0) && (div == 0)) {
		MSDC_SET_FIELD(host->base + 0x4, (0x1 << 1), 0x1);	/*MSDC_IOCON[MSDC_IOCON_RSPL] */

		MSDC_SET_FIELD(host->base + 0xb0, (0x1 << 3), 0x1);

		MSDC_SET_FIELD(host->base + 0xb4, (0xf << 8), 3);       /* MSDC_PB1_STOP_DLY_SEL */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 20), 1);      /* MSDC_PB1_POP_MARK_WATER */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 19), 1);      /* MSDC_PB1_STATE_CLR */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 18), 1);      /* Reserved bit */

		MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 29), 0);
		MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 16), 0);
		MSDC_SET_FIELD(host->base + 0xb8, (0x1 << 28), 1);
		MSDC_SET_FIELD(host->base + 0xb8, (0x1 << 25), 0x1);

		MSDC_SET_FIELD(host->base + 0x228, (0x1 << 24), 0);     /* SDC_FIFO_CFG_WR_VALID_SEL */
		MSDC_SET_FIELD(host->base + 0x228, (0x1 << 25), 0);     /* SDC_FIFO_CFG_RD_VALID_SEL */

		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x0, (0x1 << 13), 0x1);	/* EMMC_TOP_CONTROL[PAD_DAT_RD_RXDLY_SEL] */
		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x0, (0x1f << 7), 0x16);	/* EMMC_TOP_CONTROL[PAD_DAT_RD_RXDLY] */
		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x0, (0x1 << 12), 0x0);	/* EMMC_TOP_CONTROL[PAD_DAT_RD_RXDLY2_SEL] */
		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x0, (0x1f << 2), 0x0);	/* EMMC_TOP_CONTROL[PAD_DAT_RD_RXDLY2] */
		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x0, (0x1 << 15), 0x1); /* EMMC_TOP_CONTROL[SDC_RX_ENH_EN] */

		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x4, (0x1 << 11), 0x1);	/* EMMC_TOP_CMD[PAD_CMD_RXDLY] */
		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x4, (0x1f << 5), 0x16);	/* EMMC_TOP_CMD[PAD_CMD_RXDLY_SEL] */
		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x4, (0x1 << 10), 0x0);	/* EMMC_TOP_CMD[PAD_CMD_RXDLY2] */
		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x4, (0x1f << 0), 0x0);	/* EMMC_TOP_CMD[PAD_CMD_RXDLY2_SEL] */
		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x4, (0x1f << 12), 0x0);	/* EMMC_TOP_CMD_LEVEL1[PAD_CMD_TX_DLY] */

		MSDC_WRITE32(MSDC0_TOP_BASE + 0x8, 0x3);	        /* EMMC50_PAD_CTL0 */
		MSDC_WRITE32(MSDC0_TOP_BASE + 0xC, 0x0);	        /* EMMC50_PAD_CTL0 */
	} else if ((id == 1) && (mode == 0)) {
		MSDC_SET_FIELD(host->base + 0x4, (0x1 << 1), 0x0);	/*MSDC_IOCON[MSDC_IOCON_RSPL] */

		MSDC_SET_FIELD(host->base + 0xb0, (0x1 << 3), 0x0);

		MSDC_SET_FIELD(host->base + 0xb4, (0xf << 8), 3);       /* MSDC_PB1_STOP_DLY_SEL */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 20), 1);      /* MSDC_PB1_POP_MARK_WATER */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 19), 1);      /* MSDC_PB1_STATE_CLR */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 18), 1);      /* Reserved bit */

		MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 29), 0);
		MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 16), 0);
		MSDC_SET_FIELD(host->base + 0xb8, (0x1 << 28), 1);
		MSDC_SET_FIELD(host->base + 0xb8, (0x1 << 25), 0x0);

		MSDC_SET_FIELD(host->base + 0x228, (0x1 << 24), 0);     /* SDC_FIFO_CFG_WR_VALID_SEL */
		MSDC_SET_FIELD(host->base + 0x228, (0x1 << 25), 0);     /* SDC_FIFO_CFG_RD_VALID_SEL */

		MSDC_WRITE32(MSDC1_TOP_BASE + 0x0, 0x0);
		MSDC_WRITE32(MSDC1_TOP_BASE + 0x4, 0x0);

		MSDC_WRITE32(MSDC1_TOP_BASE + 0x8, 0x3);	        /* EMMC50_PAD_CTL0 */
		MSDC_WRITE32(MSDC1_TOP_BASE + 0xC, 0x18000);	        /* EMMC50_PAD_CTL0 */
	} else if ((id == 1) && (mode == 2)) {
		MSDC_SET_FIELD(host->base + 0x4, (0x1 << 1), 0x0);	/*MSDC_IOCON[MSDC_IOCON_RSPL] */

		MSDC_SET_FIELD(host->base + 0xb0, (0x1 << 3), 0x0);

		MSDC_SET_FIELD(host->base + 0xb4, (0xf << 8), 3);       /* MSDC_PB1_STOP_DLY_SEL */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 20), 1);      /* MSDC_PB1_POP_MARK_WATER */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 19), 1);      /* MSDC_PB1_STATE_CLR */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 18), 1);      /* Reserved bit */

		MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 29), 1);
		MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 16), 1);
		MSDC_SET_FIELD(host->base + 0xb8, (0x1 << 28), 1);
		MSDC_SET_FIELD(host->base + 0xb8, (0x1 << 25), 0x0);

		MSDC_SET_FIELD(host->base + 0x228, (0x1 << 24), 0);     /* SDC_FIFO_CFG_WR_VALID_SEL */
		MSDC_SET_FIELD(host->base + 0x228, (0x1 << 25), 0);     /* SDC_FIFO_CFG_RD_VALID_SEL */

		MSDC_WRITE32(MSDC1_TOP_BASE + 0x0, 0x2000);	/* EMMC_TOP_CONTROL[PAD_DAT_RD_RXDLY_SEL] */
		MSDC_WRITE32(MSDC1_TOP_BASE + 0x4, 0);	/* EMMC_TOP_CONTROL[PAD_DAT_RD_RXDLY_SEL] */

		MSDC_WRITE32(MSDC1_TOP_BASE + 0x8, 0x3);	        /* EMMC50_PAD_CTL0 */
		MSDC_WRITE32(MSDC1_TOP_BASE + 0xC, 0x18000);	        /* EMMC50_PAD_CTL0 */
	}
}
#endif

