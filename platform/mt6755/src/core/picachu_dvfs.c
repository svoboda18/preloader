/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein is
 * confidential and proprietary to MediaTek Inc. and/or its licensors. Without
 * the prior written permission of MediaTek inc. and/or its licensors, any
 * reproduction, modification, use or disclosure of MediaTek Software, and
 * information contained herein, in whole or in part, shall be strictly
 * prohibited.
 *
 * MediaTek Inc. (C) 2015. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER
 * ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 * INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES
 * TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO
 * OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN MEDIATEK
 * SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE
 * RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S
 * ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE
 * RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE
 * MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE
 * CHARGE PAID BY RECEIVER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek
 * Software") have been modified by MediaTek Inc. All revisions are subject to
 * any receiver's applicable license agreements with MediaTek Inc.
 */

#include "picachu.h"
#ifndef PMIC_CHIP_MT6353
#include "mt6311.h"
#endif

#define PMIC_CMD_DELAY_TIME     5
#define MIN_PMIC_SETTLE_TIME    25
#define PMIC_VOLT_UP_SETTLE_TIME(old_volt, new_volt) \
	(((((new_volt) - (old_volt)) + 1250 - 1) / 1250) + PMIC_CMD_DELAY_TIME)
#define PMIC_VOLT_DOWN_SETTLE_TIME(old_volt, new_volt) \
	(((((old_volt) - (new_volt)) * 2)  / 625) + PMIC_CMD_DELAY_TIME)

#define cpufreq_err                     print
#define cpufreq_ver                     print

#ifndef PMIC_CHIP_MT6353
static int pmic_exist = 0;

#define cpu_dvfs_is_extbuck_valid() (pmic_exist == 1)

#endif

/* for freq change (PLL/MUX) */
#define PLL_FREQ_STEP		(13000)		/* KHz */

#define PLL_MIN_FREQ		(130000)	/* KHz */
#define PLL_DIV1_FREQ		(1001000)	/* KHz */
#define PLL_DIV2_FREQ		(520000)	/* KHz */
#define PLL_DIV4_FREQ		(260000)	/* KHz */
#define PLL_DIV8_FREQ		(PLL_MIN_FREQ)	/* KHz */

#define DDS_DIV1_FREQ		(0x0009A000)	/* 1001MHz */
#define DDS_DIV2_FREQ		(0x010A0000)	/* 520MHz  */
#define DDS_DIV4_FREQ		(0x020A0000)	/* 260MHz  */
#define DDS_DIV8_FREQ		(0x030A0000)	/* 130MHz  */

/* The number of power loop for stress pattern. */
#if defined (PMIC_CHIP_MT6353)
int power_loop_list[] = {100};
#else
int power_loop_list[] = {5};
#endif

static unsigned int get_cur_phy_freq(struct mt_armpll_addr *armpll);
static unsigned int get_cur_volt_extbuck(void);
static unsigned int get_cur_vsram(void);
static void set_cur_volt_extbuck(unsigned int volt);

static struct mt_cpu_dvfs_ops picachu_dvfs_ops = {
	get_cur_phy_freq,
	get_cur_volt_extbuck,
	get_cur_vsram,
	set_cur_volt_extbuck
};

#if defined (PMIC_CHIP_MT6353)
struct cpu_info cpu_info_fy = {
	"FY",
	{
	  /* Little-little */
	  { 1001, 0x80134000, 0x8, 0x1 },
	  /* Little */
	  { 1508, 0x800E8000, 0x8, 0x0 },
	},
	VMAX_CPU_VOLT,
	VMIN_CPU_VOLT,
};
#else
struct cpu_info cpu_info_fy = {
	"FY",
	{
	  /* Little-Little */
	  { 1001, 0x80134000, 0x8, 0x1 },
	  /* Little */
	  { 1807, 0x80116000, 0x8, 0x0 },
	},
	VMAX_CPU_VOLT,
	VMIN_CPU_VOLT,
};

struct cpu_info cpu_info_sb = {
	"SB",
	{
	  /* Little-Little */
	  { 1144, 0x80160000, 0x8, 0x1 },
	  /* Little */
	  { 1950, 0x8012C000, 0x8, 0x0 },
	},
	VMAX_CPU_VOLT,
	VMIN_CPU_VOLT,
};
#endif

#ifndef PMIC_CHIP_MT6353 /* MT6755 */
struct cpu_info *get_cpu_info(void)
{
	unsigned int bin_level, bin_level_eng;
	struct cpu_info *cpu_info;

	bin_level = DRV_Reg32(0x1020627C) & 0xff;
	bin_level_eng = DRV_Reg32(0x10206278) & 0xffff;

	PICACHU_DEBUG("bin_level = 0x%x, bin_level_eng: 0x%x\n",
			bin_level, bin_level_eng);

	if (bin_level > 0 && bin_level <= 4) {
		/* Modulus 2 */
		cpu_info = (bin_level & 1) ? &cpu_info_fy : &cpu_info_sb;
	} else {
		if (((bin_level_eng >> 4) & 0x07) == 2 ||
			((bin_level_eng >> 10) & 0x7) == 2)
			cpu_info = &cpu_info_fy;
		else
			cpu_info = &cpu_info_sb;
	}

	PICACHU_DEBUG("The CPU is '%s'\n", cpu_info->name);

	return cpu_info;
}
#else /* MT6750 */
struct cpu_info *get_cpu_info(void) { return &cpu_info_fy; }
#endif

static unsigned int _cpu_freq_calc(unsigned int con1, unsigned int ckdiv1)
{
	unsigned int freq = 0;

	con1 &= _BITMASK_(26:0);

	if (con1 >= DDS_DIV8_FREQ) {
		freq = DDS_DIV8_FREQ;
		freq = PLL_DIV8_FREQ + (((con1 - freq) / 0x2000) * PLL_FREQ_STEP / 8);
	} else if (con1 >= DDS_DIV4_FREQ) {
		freq = DDS_DIV4_FREQ;
		freq = PLL_DIV4_FREQ + (((con1 - freq) / 0x2000) * PLL_FREQ_STEP / 4);
	} else if (con1 >= DDS_DIV2_FREQ) {
		freq = DDS_DIV2_FREQ;
		freq = PLL_DIV2_FREQ + (((con1 - freq) / 0x2000) * PLL_FREQ_STEP / 2);
	} else if (con1 >= DDS_DIV1_FREQ) {
		freq = DDS_DIV1_FREQ;
		freq = PLL_DIV1_FREQ + (((con1 - freq) / 0x2000) * PLL_FREQ_STEP);
	} else
		return 0;

	switch (ckdiv1) {
	case 9:
		freq = freq * 3 / 4;
		break;

	case 10:
		freq = freq * 2 / 4;
		break;

	case 11:
		freq = freq * 1 / 4;
		break;

	case 17:
		freq = freq * 4 / 5;
		break;

	case 18:
		freq = freq * 3 / 5;
		break;

	case 19:
		freq = freq * 2 / 5;
		break;

	case 20:
		freq = freq * 1 / 5;
		break;

	case 25:
		freq = freq * 5 / 6;
		break;

	case 26:
		freq = freq * 4 / 6;
		break;

	case 27:
		freq = freq * 3 / 6;
		break;

	case 28:
		freq = freq * 2 / 6;
		break;

	case 29:
		freq = freq * 1 / 6;
		break;

	case 8:
	case 16:
	case 24:
	default:
		break;
	}

	return freq;
}

static unsigned int get_cur_phy_freq(struct mt_armpll_addr *armpll)
{
	unsigned int con1;
	unsigned int ckdiv1;
	unsigned int cur_khz;

	con1 = DRV_Reg32(armpll->armpll_con1);
	ckdiv1 = DRV_Reg32(armpll->clkdiv);
	ckdiv1 = _GET_BITS_VAL_(4:0, ckdiv1);

	cur_khz = _cpu_freq_calc(con1, ckdiv1);

	return cur_khz;
}

#if defined(PMIC_CHIP_MT6353)
static unsigned int get_cur_vsram(void)
{
        unsigned int rdata = 0;

	pmic_read_interface((unsigned int)(MT6353_LDO_VSRAM_PROC_VOL_CON0),
                            &rdata,
                            (unsigned int)(PMIC_DA_NI_VSRAM_PROC_VOSEL_MASK),
                            (unsigned int)(PMIC_DA_NI_VSRAM_PROC_VOSEL_SHIFT));

	/* vproc: mv*100 */
        return PMIC_VAL_TO_VOLT(rdata);
}

static unsigned int get_cur_volt_extbuck(void)
{
	unsigned int rdata = 0;

	pmic_read_interface((unsigned int)(MT6353_BUCK_VPROC_VOL_CON0),
                            &rdata,
                            (unsigned int)(PMIC_DA_NI_VPROC_VOSEL_MASK),
                            (unsigned int)(PMIC_DA_NI_VPROC_VOSEL_SHIFT));

        return PMIC_VAL_TO_VOLT(rdata);
}

#else
static unsigned int get_cur_vsram(void)
{
        unsigned int rdata = 0;
        unsigned int retry_cnt = 5;

        rdata = 1;

        if (rdata) {
                do {
                        rdata = pmic_get_register_value(MT6351_PMIC_BUCK_VSRAM_PROC_VOSEL_ON_ADDR);
                } while ((rdata == 0 || rdata > 0x7F) && retry_cnt--);

                rdata = PMIC_VAL_TO_VOLT(rdata);
                /* cpufreq_ver("@%s: volt = %d\n", __func__, rdata); */
        } else
                PICACHU_INFO("@%s: read VSRAM_EN failed, rdata = 0x%x\n", __func__, rdata);

        return rdata;           /* vproc: mv*100 */
}

static unsigned int get_cur_volt_extbuck(void)
{
        unsigned char ret_val = 0;
        unsigned int ret_volt = 0;      /* volt: mv * 100 */
        unsigned int retry_cnt = 5;

        if (cpu_dvfs_is_extbuck_valid()) {
                do {
                        if (!mt6311_read_byte(MT6311_VDVFS11_CON13, &ret_val)) {
                                cpufreq_err("%s(), fail to read ext buck volt\n", __func__);
                                ret_volt = 0;
                        } else {
                                ret_volt = EXTBUCK_VAL_TO_VOLT(ret_val);
                                /* cpufreq_ver("@%s: volt = %d\n", __func__, ret_volt); */
                        }
                } while (ret_volt == EXTBUCK_VAL_TO_VOLT(0) && retry_cnt--);
        } else
                PICACHU_INFO("%s(), can not use ext buck!\n", __func__);

        return ret_volt;
}
#endif

static unsigned int _calc_pmic_settle_time(unsigned int old_vproc, unsigned int old_vsram,
					   unsigned int new_vproc, unsigned int new_vsram)
{
	unsigned delay = 100;

	if (new_vproc == old_vproc && new_vsram == old_vsram)
		return 0;

	/* VPROC is UP */
	if (new_vproc >= old_vproc) {
		/* VSRAM is UP too, choose larger one to calculate settle time */
		if (new_vsram >= old_vsram)
			delay = MAX(PMIC_VOLT_UP_SETTLE_TIME(old_vsram, new_vsram),
				    PMIC_VOLT_UP_SETTLE_TIME(old_vproc, new_vproc)
			    );
		/* VSRAM is DOWN, it may happen at bootup stage */
		else
			delay = MAX(PMIC_VOLT_DOWN_SETTLE_TIME(old_vsram, new_vsram),
				    PMIC_VOLT_UP_SETTLE_TIME(old_vproc, new_vproc)
			    );
	}
	/* VPROC is DOWN */
	else {
		/* VSRAM is DOWN too, choose larger one to calculate settle time */
		if (old_vsram >= new_vsram)
			delay = MAX(PMIC_VOLT_DOWN_SETTLE_TIME(old_vsram, new_vsram),
				    PMIC_VOLT_DOWN_SETTLE_TIME(old_vproc, new_vproc)
			    );
		/* VSRAM is UP, it may happen at bootup stage */
		else
			delay = MAX(PMIC_VOLT_UP_SETTLE_TIME(old_vsram, new_vsram),
				    PMIC_VOLT_DOWN_SETTLE_TIME(old_vproc, new_vproc)
			    );
	}

	if (delay < MIN_PMIC_SETTLE_TIME)
		delay = MIN_PMIC_SETTLE_TIME;

	return delay;
}

static void set_cur_volt_extbuck(unsigned int volt)
{
	/* volt: vproc (mv*100) */
	unsigned int cur_vsram = get_cur_vsram();
	unsigned int cur_vproc = get_cur_volt_extbuck();
	unsigned int delay_us = 0;
	int ret = 0;

	if (cur_vproc == 0) {
		cpufreq_err("@%s():%d, cur_vproc is 0!\n", __func__, __LINE__);
		return;
	}

	/* UP */
	if (volt > cur_vproc) {
		unsigned int target_vsram = volt + NORMAL_DIFF_VRSAM_VPROC;
		unsigned int next_vsram;

		do {
			unsigned int old_vproc = cur_vproc;
			unsigned int old_vsram = cur_vsram;

			next_vsram = MIN(((MAX_DIFF_VSRAM_VPROC - 2500) + cur_vproc), target_vsram);

			/* update vsram */
			cur_vsram = MAX(next_vsram, MIN_VSRAM_VOLT);

			if (cur_vsram > MAX_VSRAM_VOLT) {
				cur_vsram = MAX_VSRAM_VOLT;
				target_vsram = MAX_VSRAM_VOLT;	/* to end the loop */
			}
#if defined (PMIC_CHIP_MT6353)
			upmu_set_reg_value(PMIC_LDO_VSRAM_PROC_VOSEL_ADDR, VOLT_TO_PMIC_VAL(cur_vsram));
			upmu_set_reg_value(PMIC_LDO_VSRAM_PROC_VOSEL_ON_ADDR, VOLT_TO_PMIC_VAL(cur_vsram));
#else
			upmu_set_reg_value(MT6351_PMIC_BUCK_VSRAM_PROC_VOSEL_ADDR, VOLT_TO_PMIC_VAL(cur_vsram));
			upmu_set_reg_value(MT6351_PMIC_BUCK_VSRAM_PROC_VOSEL_ON_ADDR, VOLT_TO_PMIC_VAL(cur_vsram));
#endif

			/* update vproc */
			if (next_vsram > MAX_VSRAM_VOLT)
				cur_vproc = volt;	/* Vsram was limited, set to target vproc directly */
			else
				cur_vproc = next_vsram - NORMAL_DIFF_VRSAM_VPROC;

#if defined (PMIC_CHIP_MT6353)
			pmic_config_interface(PMIC_BUCK_VPROC_VOSEL_ADDR, VOLT_TO_PMIC_VAL(cur_vproc),
			PMIC_BUCK_VPROC_VOSEL_MASK, PMIC_BUCK_VPROC_VOSEL_SHIFT);
#else
			mt6311_config_interface((unsigned char)(MT6311_VDVFS11_CON13), VOLT_TO_EXTBUCK_VAL(cur_vproc), 0x7F, 0);
			mt6311_config_interface((unsigned char)(MT6311_VDVFS11_CON12), VOLT_TO_EXTBUCK_VAL(cur_vproc), 0x7F, 0);
#endif

			delay_us =
			    _calc_pmic_settle_time(old_vproc, old_vsram, cur_vproc, cur_vsram);
			udelay(delay_us);
		} while (target_vsram > cur_vsram);
	}
	/* DOWN */
	else if (volt < cur_vproc) {
		unsigned int next_vproc;
		unsigned int next_vsram = cur_vproc + NORMAL_DIFF_VRSAM_VPROC;

		do {
			unsigned int old_vproc = cur_vproc;
			unsigned int old_vsram = cur_vsram;

			next_vproc = MAX((next_vsram - (MAX_DIFF_VSRAM_VPROC - 2500)), volt);

			/* update vproc */
			cur_vproc = next_vproc;

#if defined (PMIC_CHIP_MT6353)
			pmic_config_interface(PMIC_BUCK_VPROC_VOSEL_ADDR, VOLT_TO_PMIC_VAL(cur_vproc),
			PMIC_BUCK_VPROC_VOSEL_MASK, PMIC_BUCK_VPROC_VOSEL_SHIFT);
#else
			mt6311_config_interface((unsigned char)(MT6311_VDVFS11_CON13), VOLT_TO_EXTBUCK_VAL(cur_vproc), 0x7F, 0);
			mt6311_config_interface((unsigned char)(MT6311_VDVFS11_CON12), VOLT_TO_EXTBUCK_VAL(cur_vproc), 0x7F, 0);
#endif

			/* update vsram */
			next_vsram = cur_vproc + NORMAL_DIFF_VRSAM_VPROC;
			cur_vsram = MAX(next_vsram, MIN_VSRAM_VOLT);
			cur_vsram = MIN(cur_vsram, MAX_VSRAM_VOLT);

#if defined (PMIC_CHIP_MT6353)
			upmu_set_reg_value(PMIC_LDO_VSRAM_PROC_VOSEL_ADDR, VOLT_TO_PMIC_VAL(cur_vsram));
			upmu_set_reg_value(PMIC_LDO_VSRAM_PROC_VOSEL_ON_ADDR, VOLT_TO_PMIC_VAL(cur_vsram));
#else
			upmu_set_reg_value(MT6351_PMIC_BUCK_VSRAM_PROC_VOSEL_ADDR, VOLT_TO_PMIC_VAL(cur_vsram));
			upmu_set_reg_value(MT6351_PMIC_BUCK_VSRAM_PROC_VOSEL_ON_ADDR, VOLT_TO_PMIC_VAL(cur_vsram));
#endif

			delay_us =
			    _calc_pmic_settle_time(old_vproc, old_vsram, cur_vproc, cur_vsram);
			udelay(delay_us);
		} while (cur_vproc > volt);
	}
}

void picachu_check_pmic_exist(void)
{
#ifndef PMIC_CHIP_MT6353
        pmic_exist = is_mt6311_exist();
#endif
}

struct mt_cpu_dvfs_ops *picachu_cpu_dvfs_ops(void)
{
	return &picachu_dvfs_ops;
}
