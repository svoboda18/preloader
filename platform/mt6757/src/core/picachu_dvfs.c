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
 * MediaTek Inc. (C) 2016. All rights reserved.
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

#if defined (PMIC_CHIP_MT6355)
#include "pmic.h"
#else
#include "mt6311.h"
#endif

#define PMIC_CMD_DELAY_TIME     5
#define MIN_PMIC_SETTLE_TIME    25
#define PMIC_VOLT_UP_SETTLE_TIME(old_volt, new_volt) \
	(((((new_volt) - (old_volt)) + 1250 - 1) / 1250) + PMIC_CMD_DELAY_TIME)
#define PMIC_VOLT_DOWN_SETTLE_TIME(old_volt, new_volt) \
	(((((old_volt) - (new_volt)) * 2)  / 625) + PMIC_CMD_DELAY_TIME)

#define cpu_dvfs_is_extbuck_valid() (pmic_exist == 1)

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

#define DVTFIXED	7
#define DVTFIXED_KBP	8

static int pmic_exist = 0;

static unsigned int get_cur_phy_freq(struct mt_armpll_addr *armpll);
static unsigned int get_cur_volt_extbuck(void);
static unsigned int get_cur_vsram(void);
static void set_cur_volt_extbuck(struct cpu_info *cpu_info, unsigned int volt);
static struct cpu_info *get_cpu_info(void);

extern struct cpu_margin oe_cpu_margin;
extern struct cpu_margin kb_cpu_margin;
extern struct cpu_margin kbp_cpu_margin;

#if defined (PMIC_CHIP_MT6355)
struct regulator *reg_vproc11, *reg_vsram_proc;
#endif

static struct mt_cpu_dvfs_ops picachu_dvfs_ops = {
	get_cur_phy_freq,
	get_cur_volt_extbuck,
	get_cur_vsram,
	set_cur_volt_extbuck,
	get_cpu_info
};

/* Olympus E1/E2 chip */
static struct cpu_info cpu_info_oe = {
	"OE",
	{
	  /* Freq, Clock Divider, Post Divider */
	  /* Little-Little */
	  {1638, 0x8, 0x0, 0, 0},
	  /* Little */
	  {2340, 0x8, 0x0, 0, 0},
	  /* CCI */
	  {988, 0x8, 0x1, 0, 0},
	},
	&oe_cpu_margin,
	DVTFIXED,
	MAX_VPROC_PMIC,
	MIN_VPROC_PMIC,
	MAX_VSRAM_PMIC,
	MIN_VSRAM_PMIC,
	0,
};

static struct cpu_info cpu_info_kb = {
	"KB",
	{
	  /* Freq, Clock Divider, Post Divider */
	  /* Little-Little */
	  { 1638, 0x8, 0x0, 0, 0},
	  /* Little */
	  { 2340, 0x8, 0x0, 0, 0},
	  /* CCI */
	  { 1014, 0x8, 0x0, 0, 0},
	},
	&kb_cpu_margin,
	DVTFIXED,
	KB_MAX_VPROC_PMIC,
	MIN_VPROC_PMIC,
	KB_MAX_VSRAM_PMIC,
	MIN_VSRAM_PMIC,
	0,
};

static struct cpu_info cpu_info_kbp = {
	"KBP",
	{
	 /* Freq, Clock Divider, Post Divider */
	 /* Little-Little */
	 { 1690, 0x8, 0x0, 0, 0},
	 /* Little */
	 { 2392, 0x8, 0x0, 0, 0},
	 /* CCI */
	 { 1040, 0x8, 0x0, 0, 0},
	},
	&kbp_cpu_margin,
	DVTFIXED_KBP,
	KBP_MAX_VPROC_PMIC,
	MIN_VPROC_PMIC,
	KBP_MAX_VSRAM_PMIC,
	MIN_VSRAM_PMIC,
	0,
};

static struct cpu_info *get_cpu_info(void)
{
	unsigned int segment;

	segment = (seclib_get_devinfo_with_index(30) & 0x000000E0) >> 5;
	//segment = (DRV_Reg32(0x10206054) & 0x000000E0) >> 5;

	PICACHU_DEBUG("segment: %d\n", segment);

	switch (segment) {
	case 0:
		return &cpu_info_oe;
	case 1:
		return &cpu_info_kb;
	case 3:
	case 7:

#if defined (PMIC_CHIP_MT6355)  /* Kibo+ (MT6355) */
		return &cpu_info_kbp;
#else
		return &cpu_info_kb;
#endif

	}

	return &cpu_info_oe;
}

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

#if defined (PMIC_CHIP_MT6355)  /* Kibo+ (MT6355) */
static unsigned int get_cur_vsram(void)
{
	return regulator_get_voltage(reg_vsram_proc) / 10;
}

static void set_vsram(unsigned int volt)
{
	if (regulator_set_voltage(reg_vsram_proc, volt * 10, 1306250))
		PICACHU_INFO("set vsram failed\n");
}

static void set_vproc(unsigned int volt)
{
	if(regulator_set_voltage(reg_vproc11, volt * 10, 1193750))
		PICACHU_INFO("set vproc failed\n");
}

static unsigned int get_cur_volt_extbuck(void)
{
	return regulator_get_voltage(reg_vproc11) / 10;
}

#else /* Olympus and kibo (MT6351) */
static unsigned int get_cur_vsram(void)
{
        unsigned int rdata = 0;
        unsigned int retry_cnt = 5;

	 do {
                rdata = pmic_get_register_value(MT6351_PMIC_BUCK_VSRAM_PROC_VOSEL_ON_ADDR, 0x7F, 0);
        } while ((rdata == 0 || rdata > 0x7F) && retry_cnt--);

        rdata = PMIC_VAL_TO_VOLT(rdata);

        return rdata;   /* mv * 100 */
}

static void set_vsram(unsigned int volt)
{
	upmu_set_reg_value(MT6351_PMIC_BUCK_VSRAM_PROC_VOSEL_ADDR,
				VOLT_TO_PMIC_VAL(volt));
	upmu_set_reg_value(MT6351_PMIC_BUCK_VSRAM_PROC_VOSEL_ON_ADDR,
				VOLT_TO_PMIC_VAL(volt));
}

static void set_vproc(unsigned int volt)
{
	mt6311_config_interface((unsigned char)(MT6311_VDVFS11_CON12),
                        VOLT_TO_EXTBUCK_VAL(volt), 0x7F, 0);
	mt6311_config_interface((unsigned char)(MT6311_VDVFS11_CON13),
			VOLT_TO_EXTBUCK_VAL(volt), 0x7F, 0);
}

static unsigned int get_cur_volt_extbuck(void)
{
        unsigned char ret_val = 0;
        unsigned int ret_volt = 0;      /* volt: mv * 100 */
        unsigned int retry_cnt = 5;

        if (cpu_dvfs_is_extbuck_valid()) {
                do {
                        if (!mt6311_read_byte(MT6311_VDVFS11_CON12, &ret_val)) {
                                PICACHU_INFO(" fail to read ext buck volt\n");
                                ret_volt = 0;
                        } else {
                                ret_volt = EXTBUCK_VAL_TO_VOLT(ret_val);
                        }
                } while (ret_volt == EXTBUCK_VAL_TO_VOLT(0) && retry_cnt--);
        } else
                PICACHU_INFO("%s(), can not use ext buck!\n", __func__);

        return ret_volt;
}
#endif

static unsigned int _calc_pmic_settle_time(unsigned int old_vproc,
					   unsigned int old_vsram,
					   unsigned int new_vproc,
					   unsigned int new_vsram)
{
	unsigned int delay;

	if (new_vproc == old_vproc && new_vsram == old_vsram)
		return 0;

	/* VPROC is UP */
	if (new_vproc >= old_vproc) {
		/* VSRAM is UP too, choose larger one for settle time */
		if (new_vsram >= old_vsram)
			delay = MAX(PMIC_VOLT_UP_SETTLE_TIME(old_vsram, new_vsram),
				    PMIC_VOLT_UP_SETTLE_TIME(old_vproc, new_vproc));
		else /* VSRAM is DOWN, it may happen at bootup stage */
			delay = MAX(PMIC_VOLT_DOWN_SETTLE_TIME(old_vsram, new_vsram),
				    PMIC_VOLT_UP_SETTLE_TIME(old_vproc, new_vproc));
	} else { /* VPROC is DOWN */

		/* VSRAM is DOWN too, choose larger one for settle time */
		if (old_vsram >= new_vsram)
			delay = MAX(PMIC_VOLT_DOWN_SETTLE_TIME(old_vsram, new_vsram),
				    PMIC_VOLT_DOWN_SETTLE_TIME(old_vproc, new_vproc));
		else /* VSRAM is UP, it may happen at bootup stage */
			delay = MAX(PMIC_VOLT_UP_SETTLE_TIME(old_vsram, new_vsram),
				    PMIC_VOLT_DOWN_SETTLE_TIME(old_vproc, new_vproc));
	}

	if (delay < MIN_PMIC_SETTLE_TIME)
		delay = MIN_PMIC_SETTLE_TIME;

	return delay;
}

static void set_cur_volt_extbuck(struct cpu_info *cpu_info, unsigned int volt)
{
	/* volt: vproc (mv*100) */
	unsigned int cur_vsram = get_cur_vsram();
	unsigned int cur_vproc = get_cur_volt_extbuck();
	unsigned int val;
	unsigned int delay_us = 0;
	int ret = 0;

	if (cur_vproc == 0) {
		PICACHU_INFO("cur_vproc is 0!\n");
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
			cur_vsram = MAX(next_vsram, PMIC_VAL_TO_VOLT(cpu_info->min_vsram_pmic));

			val = PMIC_VAL_TO_VOLT(cpu_info->max_vsram_pmic);
			if (cur_vsram > val) {
				cur_vsram = val;
				target_vsram = val;/* to end the loop */
			}

			/* Set voltage of VSRAM */
			set_vsram(cur_vsram);

			/* update vproc */
			if (next_vsram > PMIC_VAL_TO_VOLT(cpu_info->max_vsram_pmic))
				cur_vproc = volt;	/* Vsram was limited, set to target vproc directly */
			else
				cur_vproc = next_vsram - NORMAL_DIFF_VRSAM_VPROC;

			set_vproc(cur_vproc);

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

			set_vproc(cur_vproc);

			/* update vsram */
			next_vsram = cur_vproc + NORMAL_DIFF_VRSAM_VPROC;
			cur_vsram = MAX(next_vsram, PMIC_VAL_TO_VOLT(cpu_info->min_vsram_pmic));
			cur_vsram = MIN(cur_vsram, PMIC_VAL_TO_VOLT(cpu_info->max_vsram_pmic));

			/* Set voltage of VSRAM */
			set_vsram(cur_vsram);

			delay_us =
			    _calc_pmic_settle_time(old_vproc, old_vsram, cur_vproc, cur_vsram);
			udelay(delay_us);
		} while (cur_vproc > volt);
	}
}

int picachu_dvfs_init(void)
{

#if defined (PMIC_CHIP_MT6355)
	reg_vproc11 = regulator_get(NULL, "vproc11");
	if (!reg_vproc11)
		PICACHU_INFO("regulator_get reg_vproc11 failed\n");

	reg_vsram_proc = regulator_get(NULL, "vsram_proc");
	if (!reg_vsram_proc)
		PICACHU_INFO("regulator_get reg_vsram_proc failed\n");


	pmic_exist = 1;
#else
	pmic_exist = is_mt6311_exist();
#endif
	picachu_register_ops(PICACHU_DVFS_OPS, (void *) &picachu_dvfs_ops);

	return 0;
}
