/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2010
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

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
#define pmic_config_interface(RegNum, val, MASK, SHIFT);
#define pmic_read_interface(RegNum, val, MASK, SHIFT);
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
				if (CARD_VOL_ACTUAL != VOL_3000) {
					pmic_config_interface(REG_VEMC_VOSEL_CAL,
					                      VEMC_VOSEL_CAL_mV(CARD_VOL_ACTUAL - VOL_3000),
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
				if (CARD_VOL_ACTUAL != VOL_3000) {
					pmic_config_interface(REG_VMCH_VOSEL_CAL,
					                      VMCH_VOSEL_CAL_mV(CARD_VOL_ACTUAL - VOL_3000),
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

//FIX ME
//To do: check if PMIC can provide unified interface for PL/LK/CTP and independent of PMIC
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
	u32 card_on=on;
	if (host->id == 1 || host->id == 2) {
		host->cur_pwr = level;
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
	msdc_printf(" VEMC_EN=0x%x, should:1b'1, "
	       "VEMC_VOL=0x%x, should:1b'0(3.0V),1b'1(3.3V)\n",
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

#if !defined(FPGA_PLATFORM)
void msdc_dump_clock_sts(struct mmc_host *host)
{
	msdc_printf(" msdcpll en[0x1000C250][bit0 should be 0x1]=0x%x\n", MSDC_READ32(0x1000C250));
	msdc_printf(" msdcpll on[0x1000C25C][bit0~1 should be 0x1]=0x%x\n", MSDC_READ32(0x1000C25C));

	msdc_printf(" pdn, mux[0x10000070][bit23, 16~19 for msdc0, bit31, 24~26 for msdc1]=0x%x\n", MSDC_READ32(0x10000070));
}
#endif

void msdc_clock(struct mmc_host *host, int on)
{
	//Only CTP need enable/disable clock
	//Preloader will turn on clock with predefined clock source and module need not to touch clock setting.
	//LK will use preloader's setting and need not to touch clock setting.
#if !defined(FPGA_PLATFORM)
#if defined(MMC_MSDC_DRV_CTP) && defined(MTKDRV_CLOCK_MANAGER)
	int clk_id[MSDC_MAX_NUM] = {MT_CG_INFRA_MSDC_0, MT_CG_INFRA_MSDC_1, MT_CG_INFRA_MSDC_2, MT_CG_INFRA_MSDC_3};
	MSG(CFG, "[SD%d] Turn %s %s clock \n", host->id, on ? "on" : "off", "host");

	if (on) {
		INFRA_enable_clock(clk_id[host->id]);
	} else {
		INFRA_disable_clock(clk_id[host->id]);
	}
#endif
#endif /* end of FPGA_PLATFORM */
#if !defined(FPGA_PLATFORM)
#ifdef MTK_MSDC_BRINGUP_DEBUG
	msdc_dump_clock_sts(host);
#endif
#endif
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
	host->pll_mux_clk = MSDC_CLKSRC_DEFAULT(host->id);
	host->src_clk = msdc_src_clks[host->pll_mux_clk];
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
	}
	MSDC_WRITE32(TOPCKGEN_BASE + 0x04, 0x07FFFFFF);
	#endif
#else
	host->src_clk    = msdc_src_clks[clksrc];
#endif
	msdc_printf("[info][%s] input clock is %dkHz\n", __func__, host->src_clk/1000);
}

/**************************************************************/
/* Section 3: GPIO and Pad                    */
/**************************************************************/
#if !defined(FPGA_PLATFORM)
void msdc_dump_padctl_by_id(u32 id)
{
	switch (id) {
		case 0:
			msdc_printf("MSDC0 MODE23[0x%x] =0x%8x\tshould:0x111111??\n",
				MSDC0_GPIO_MODE23, MSDC_READ32(MSDC0_GPIO_MODE23));
			msdc_printf("MSDC0 MODE24[0x%x] =0x%8x\tshould:0x??111111\n",
				MSDC0_GPIO_MODE24, MSDC_READ32(MSDC0_GPIO_MODE24));
			msdc_printf("MSDC0 IES	[0x%x] =0x%8x\tshould:0x??????1f\n",
				MSDC0_GPIO_IES_ADDR, MSDC_READ32(MSDC0_GPIO_IES_ADDR));
			msdc_printf("MSDC0 SMT	[0x%x] =0x%8x\tshould:0x???????f\n",
				MSDC0_GPIO_SMT_ADDR, MSDC_READ32(MSDC0_GPIO_SMT_ADDR));
			msdc_printf("MSDC0 TDSEL [0x%x] =0x%8x\tshould:0x????0000\n",
				MSDC0_GPIO_TDSEL_ADDR,
				MSDC_READ32(MSDC0_GPIO_TDSEL_ADDR));
			msdc_printf("MSDC0 RDSEL [0x%x] =0x%8x\tshould:0x???00000\n",
				MSDC0_GPIO_RDSEL_ADDR,
				MSDC_READ32(MSDC0_GPIO_RDSEL_ADDR));
			msdc_printf("MSDC0 DRV	[0x%x] =0x%8x\tshould: 1001001001b\n",
				MSDC0_GPIO_DRV_ADDR,
				MSDC_READ32(MSDC0_GPIO_DRV_ADDR));
			msdc_printf("PUPD/R1/R0: dat/cmd:0/0/1, clk/dst: 1/1/0\n");
			msdc_printf("PUPD/R1/R0: rstb: 0/1/0\n");
			msdc_printf("MSDC0 PUPD0 [0x%x] =0x%8x\tshould: 0x21111161 ??????b\n",
				MSDC0_GPIO_PUPD0_ADDR,
				MSDC_READ32(MSDC0_GPIO_PUPD0_ADDR));
			msdc_printf("MSDC0 PUPD1 [0x%x] =0x%8x\tshould: 0x6111 ??????b\n",
				MSDC0_GPIO_PUPD1_ADDR,
				MSDC_READ32(MSDC0_GPIO_PUPD1_ADDR));
			break;
		case 1:
			msdc_printf("MSDC1 MODE4 [0x%x] =0x%8x\n",
				MSDC1_GPIO_MODE4, MSDC_READ32(MSDC1_GPIO_MODE4));
			msdc_printf("MSDC1 MODE5 [0x%x] =0x%8x\n",
				MSDC1_GPIO_MODE5, MSDC_READ32(MSDC1_GPIO_MODE5));
			msdc_printf("MSDC1 IES	 [0x%x] =0x%8x\n",
				MSDC1_GPIO_IES_ADDR, MSDC_READ32(MSDC1_GPIO_IES_ADDR));
			msdc_printf("MSDC1 SMT	 [0x%x] =0x%8x\n",
				MSDC1_GPIO_SMT_ADDR, MSDC_READ32(MSDC1_GPIO_SMT_ADDR));

			msdc_printf("MSDC1 TDSEL  [0x%x] =0x%8x\n",
				MSDC1_GPIO_TDSEL_ADDR,
				MSDC_READ32(MSDC1_GPIO_TDSEL_ADDR));
			/* Note1=> For Vcore=0.7V sleep mode
			 * if TDSEL0~3 don't set to [1111]
			 * the digital output function will fail
			 */
			msdc_printf("should 1.8v: sleep:0x?fff????, awake:0x?aaa????\n");
			msdc_printf("MSDC1 RDSEL0  [0x%x] =0x%8x\n",
				MSDC1_GPIO_RDSEL0_ADDR,
				MSDC_READ32(MSDC1_GPIO_RDSEL0_ADDR));
			msdc_printf("MSDC1 RDSEL1  [0x%x] =0x%8x\n",
				MSDC1_GPIO_RDSEL1_ADDR,
				MSDC_READ32(MSDC1_GPIO_RDSEL1_ADDR));
			msdc_printf("1.8V: ??0000??, 2.9v: 0x??c30c??\n");

			msdc_printf("MSDC1 DRV1	  [0x%x] =0x%8x\n",
				MSDC1_GPIO_DRV1_ADDR, MSDC_READ32(MSDC1_GPIO_DRV1_ADDR));

			msdc_printf("MSDC1 PUPD	 [0x%x] =0x%8x\n",
				MSDC1_GPIO_PUPD_ADDR,
				MSDC_READ32(MSDC1_GPIO_PUPD_ADDR));
			break;
		default:
			msdc_printf("[%s] invalid host->id!\n", __func__);
			break;
	}
}

void msdc_set_pin_mode(struct mmc_host *host)
{
#if defined(MMC_MSDC_DRV_CTP)
	switch (host->id) {
		case 0:
			MSDC_SET_FIELD(MSDC0_GPIO_MODE23, MSDC0_MODE_DAT6_MASK, 0x1);
			MSDC_SET_FIELD(MSDC0_GPIO_MODE23, MSDC0_MODE_DAT5_MASK, 0x1);
			MSDC_SET_FIELD(MSDC0_GPIO_MODE23, MSDC0_MODE_DAT1_MASK, 0x1);
			MSDC_SET_FIELD(MSDC0_GPIO_MODE23, MSDC0_MODE_CMD_MASK,	0x1);
			MSDC_SET_FIELD(MSDC0_GPIO_MODE23, MSDC0_MODE_CLK_MASK,	0x1);
			MSDC_SET_FIELD(MSDC0_GPIO_MODE23, MSDC0_MODE_DAT0_MASK, 0x1);

			MSDC_SET_FIELD(MSDC0_GPIO_MODE24, MSDC0_MODE_DSL_MASK,	0x1);
			MSDC_SET_FIELD(MSDC0_GPIO_MODE24, MSDC0_MODE_DAT7_MASK, 0x1);
			MSDC_SET_FIELD(MSDC0_GPIO_MODE24, MSDC0_MODE_DAT3_MASK, 0x1);
			MSDC_SET_FIELD(MSDC0_GPIO_MODE24, MSDC0_MODE_DAT2_MASK, 0x1);
			MSDC_SET_FIELD(MSDC0_GPIO_MODE24, MSDC0_MODE_RSTB_MASK, 0x1);
			MSDC_SET_FIELD(MSDC0_GPIO_MODE24, MSDC0_MODE_DAT4_MASK, 0x1);
			break;
		case 1:
			MSDC_SET_FIELD(MSDC1_GPIO_MODE4, MSDC1_MODE_DAT3_MASK, 0x1);
			MSDC_SET_FIELD(MSDC1_GPIO_MODE4, MSDC1_MODE_CLK_MASK,  0x1);

			MSDC_SET_FIELD(MSDC1_GPIO_MODE5, MSDC1_MODE_DAT1_MASK, 0x1);
			MSDC_SET_FIELD(MSDC1_GPIO_MODE5, MSDC1_MODE_DAT2_MASK, 0x1);
			MSDC_SET_FIELD(MSDC1_GPIO_MODE5, MSDC1_MODE_DAT0_MASK, 0x1);
			MSDC_SET_FIELD(MSDC1_GPIO_MODE5, MSDC1_MODE_CMD_MASK,  0x1);
			break;
		default:
			msdc_printf("[%s] invalid host->id!\n", __func__);
			break;
	}
#endif
}

/* set pin ies:
 * 0: Disable
 * 1: Enable
 */
void msdc_set_ies_by_id(u32 id, int set_ies)
{
	switch (id) {
		case 0:
			MSDC_SET_FIELD(MSDC0_GPIO_IES_ADDR, MSDC0_IES_ALL_MASK,
				(set_ies ? 0x1f : 0));
			break;
		case 1:
			MSDC_SET_FIELD(MSDC1_GPIO_IES_ADDR, MSDC1_IES_ALL_MASK,
				(set_ies ? 0x7 : 0));
			break;
		default:
			msdc_printf("[%s] invalid host->id!\n", __func__);
			break;
	}
}

void msdc_set_smt_by_id(u32 id, int set_smt)
{
	switch (id) {
		case 0:
			MSDC_SET_FIELD(MSDC0_GPIO_SMT_ADDR, MSDC0_SMT_ALL_MASK,
				(set_smt ? 0x1f : 0));
			break;
		case 1:
			MSDC_SET_FIELD(MSDC1_GPIO_SMT_ADDR, MSDC1_SMT_ALL_MASK,
				(set_smt ? 0x7 : 0));
			break;
		default:
			msdc_printf("[%s] invalid host->id!\n", __func__);
			break;
	}
}

void msdc_set_tdsel_by_id(u32 id, bool sleep, bool sd_18)
{
	switch (id) {
		case 0:
			MSDC_SET_FIELD(MSDC0_GPIO_TDSEL_ADDR, MSDC0_TDSEL_ALL_MASK, 0);
			break;
		case 1:
			MSDC_SET_FIELD(MSDC1_GPIO_TDSEL_ADDR, MSDC1_TDSEL_ALL_MASK, 0);
			break;
		default:
			msdc_printf("[%s] invalid host->id!\n", __func__);
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
			MSDC_SET_FIELD(MSDC1_GPIO_RDSEL0_ADDR, MSDC1_RDSEL0_ALL_MASK, 0);
			MSDC_SET_FIELD(MSDC1_GPIO_RDSEL1_ADDR, MSDC1_RDSEL1_ALL_MASK, 0);
			break;
		default:
			msdc_printf("[%s] invalid host->id!\n", __func__);
			break;
	}
}

void msdc_set_sr_by_id(u32 id, int clk, int cmd, int dat)
{
	switch (id) {
		case 0:
			/* no SR to set */
			break;
		case 1:
			MSDC_SET_FIELD(MSDC1_GPIO_DRV1_ADDR, MSDC1_SR_CMD_MASK,
				(cmd != 0));
			MSDC_SET_FIELD(MSDC1_GPIO_DRV1_ADDR, MSDC1_SR_CLK_MASK,
				(clk != 0));
			MSDC_SET_FIELD(MSDC1_GPIO_DRV1_ADDR, MSDC1_SR_DAT_MASK,
				(dat != 0));
			break;
		default:
			msdc_printf("[%s] invalid host->id!\n", __func__);
			break;
	}
}

/*
 * Set pin driving:
 * MSDC0,MSDC1
 * 000: 2mA
 * 001: 4mA
 * 010: 6mA
 * 011: 8mA
 * 100: 10mA
 * 101: 12mA
 * 110: 14mA
 * 111: 16mA
 */
void msdc_set_driving_by_id(u32 id, struct msdc_cust *hw, bool sd_18)
{
	switch (id) {
		case 0:
			MSDC_SET_FIELD(MSDC0_GPIO_DRV_ADDR, MSDC0_DRV_DSL_MASK,
				hw->ds_drv);
			MSDC_SET_FIELD(MSDC0_GPIO_DRV_ADDR, MSDC0_DRV_RSTB_MASK,
				hw->rst_drv);
			MSDC_SET_FIELD(MSDC0_GPIO_DRV_ADDR, MSDC0_DRV_CMD_MASK,
				hw->cmd_drv);
			MSDC_SET_FIELD(MSDC0_GPIO_DRV_ADDR, MSDC0_DRV_CLK_MASK,
				hw->clk_drv);
			MSDC_SET_FIELD(MSDC0_GPIO_DRV_ADDR, MSDC0_DRV_DAT_MASK,
				hw->dat_drv);
			break;
		case 1:
			if (sd_18) {
				MSDC_SET_FIELD(MSDC1_GPIO_DRV1_ADDR, MSDC1_DRV_CMD_MASK,
					hw->cmd_18v_drv);
				MSDC_SET_FIELD(MSDC1_GPIO_DRV1_ADDR, MSDC1_DRV_CLK_MASK,
					hw->clk_18v_drv);
				MSDC_SET_FIELD(MSDC1_GPIO_DRV1_ADDR, MSDC1_DRV_DAT_MASK,
					hw->dat_18v_drv);
			} else {
				MSDC_SET_FIELD(MSDC1_GPIO_DRV1_ADDR, MSDC1_DRV_CMD_MASK,
					hw->cmd_drv);
				MSDC_SET_FIELD(MSDC1_GPIO_DRV1_ADDR, MSDC1_DRV_CLK_MASK,
					hw->clk_drv);
				MSDC_SET_FIELD(MSDC1_GPIO_DRV1_ADDR, MSDC1_DRV_DAT_MASK,
					hw->dat_drv);
			}
			break;
		default:
			msdc_printf("[%s] invalid host->id!\n", __func__);
			break;
	}
}

void msdc_get_driving_by_id(u32 id, struct msdc_cust *hw)
{
	switch (id) {
		case 0:
			MSDC_GET_FIELD(MSDC0_GPIO_DRV_ADDR, MSDC0_DRV_DSL_MASK,
				hw->ds_drv);
			MSDC_GET_FIELD(MSDC0_GPIO_DRV_ADDR, MSDC0_DRV_RSTB_MASK,
				hw->rst_drv);
			MSDC_GET_FIELD(MSDC0_GPIO_DRV_ADDR, MSDC0_DRV_CMD_MASK,
				hw->cmd_drv);
			MSDC_GET_FIELD(MSDC0_GPIO_DRV_ADDR, MSDC0_DRV_CLK_MASK,
				hw->clk_drv);
			MSDC_GET_FIELD(MSDC0_GPIO_DRV_ADDR, MSDC0_DRV_DAT_MASK,
				hw->dat_drv);
			break;
		case 1:
			MSDC_GET_FIELD(MSDC1_GPIO_DRV1_ADDR, MSDC1_DRV_CMD_MASK,
				hw->cmd_drv);
			MSDC_GET_FIELD(MSDC1_GPIO_DRV1_ADDR, MSDC1_DRV_CLK_MASK,
				hw->clk_drv);
			MSDC_GET_FIELD(MSDC1_GPIO_DRV1_ADDR, MSDC1_DRV_DAT_MASK,
				hw->dat_drv);
			break;
		default:
			msdc_printf("[%s] invalid host->id!\n", __func__);
			break;
	}
}

/* msdc pin config
 * MSDC0
 * PUPD/R1/R0
 * 0/0/0: High-Z
 * 0/1/0: Pull-up with 50Kohm
 * 0/0/1: Pull-up with 10Kohm
 * 0/1/1: Pull-up with 50Kohm//10Kohm
 * 1/0/0: High-Z
 * 1/1/0: Pull-down with 50Kohm
 * 1/0/1: Pull-down with 10Kohm
 * 1/1/1: Pull-down with 50Kohm//10Kohm
 */
void msdc_pin_config_by_id(u32 id, u32 mode)
{
	switch (id) {
		case 0:
			/*Attention: don't pull CLK high; Don't toggle RST to prevent
			  from entering boot mode */
			if (MSDC_PIN_PULL_NONE == mode) {
				/* Switch MSDC0_* to no ohm PU, MSDC0_RSTB to 50K ohm PU
				* 0x10002AC0[32:0] = 0x20000000
				* 0x10002AD0[15:0] = 0
				*/
				MSDC_SET_FIELD(MSDC0_GPIO_PUPD0_ADDR, 0xFFFFFFFF, 0x20000000);
				MSDC_SET_FIELD(MSDC0_GPIO_PUPD1_ADDR, 0x7FFF, 0);
			} else if (MSDC_PIN_PULL_DOWN == mode) {
				/* Switch MSDC0_* to 50K ohm PD, MSDC0_RSTB to 50K ohm PU
				* 0x10002AC0[32:0] = 0x26666666
				* 0x10002AD0[15:0] = 0x6666
				*/
				MSDC_SET_FIELD(MSDC0_GPIO_PUPD0_ADDR, 0xFFFFFFFF, 0x26666666);
				MSDC_SET_FIELD(MSDC0_GPIO_PUPD1_ADDR, 0x7FFF, 0x6666);
			} else if (MSDC_PIN_PULL_UP == mode) {
				/* Switch MSDC0_CLK to 50K ohm PD, MSDC0_CMD/MSDC0_DAT*
				* to 10K ohm PU, MSDC0_RSTB to 50K ohm PU, MSDC0_DSL to 50K ohm PD
				* 0x10002AC0[31:0] = 0x21111161
				* 0x10002AD0[14:0] = 0x6111
				*/
				MSDC_SET_FIELD(MSDC0_GPIO_PUPD0_ADDR, 0xFFFFFFFF, 0x21111161);
				MSDC_SET_FIELD(MSDC0_GPIO_PUPD1_ADDR, 0x7FFF, 0x6111);
			}
			break;
		case 1:
			if (MSDC_PIN_PULL_NONE == mode) {
				/* Switch MSDC1_* to no ohm PU
				* 0x100020C0[22:0] = 0
				*/
				MSDC_SET_FIELD(MSDC1_GPIO_PUPD_ADDR, 0x7FFFFF, 0);
			} else if (MSDC_PIN_PULL_DOWN == mode) {
				/* Switch MSDC1_* to 50K ohm PD
				* 0x100020C0[22:0] = 0x666666
				*/
				MSDC_SET_FIELD(MSDC1_GPIO_PUPD_ADDR, 0x7FFFFF, 0x666666);
			} else if (MSDC_PIN_PULL_UP == mode) {
				/* Switch MSDC1_CLK to 50K ohm PD,
				* MSDC1_CMD/MSDC1_DAT* to 10K ohm PU
				* 0x100020C0[22:0] = 0x111161
				*/
				MSDC_SET_FIELD(MSDC1_GPIO_PUPD_ADDR, 0x7FFFFF, 0x111161);
			}
			break;
		default:
			msdc_printf("[%s] invalid host->id!\n", __func__);
			break;
	}
}

/**************************************************************/
/* Section 4: MISC                        */
/**************************************************************/
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
#ifdef MTK_MSDC_BRINGUP_DEBUG
	msdc_dump_padctl_by_id(host->id);
#endif
}
#endif
