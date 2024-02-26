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

#include "platform.h"
#include "sec_devinfo.h"
#include "mtk_defeem.h"
#include "mtk_eem.h"

#include "mtk_eem_config.h"
#include "mtk_eem_internal_ap.h"

#include "mtk_eem_internal.h"
#include <regulator/mtk_regulator.h>
#include <mt6359.h>

/* for setting pmic pwm mode and auto mode */
static struct mtk_regulator eem_regulator_vcore;

static unsigned int detcount[4];
static unsigned int arrtype[4] = {0, DCCONFIG_VAL, 0xaaaaaa, 0xffffff};

void base_ops_switch_bank(struct eem_det *det, enum eem_phase phase)
{
	unsigned int coresel;

	coresel = (eem_read(EEMCORESEL) & ~BITMASK(2:0))
		| BITS(2:0, det->realptpid);

	/* 803f0000 + det->realptpid = enable ctrl's swcg clock */
	/* 003f0000 + det->realptpid = disable ctrl's swcg clock */
	/* bug: when system resume, need to restore coresel value */
	if (phase == EEM_PHASE_INIT01) {
		coresel |= CORESEL_VAL;
	} else {
		coresel |= CORESEL_INIT2_VAL;
#if defined(CFG_THERM_LVTS) && CFG_LVTS_DOMINATOR
		coresel &= 0x0fffffff;
#else
		coresel &= 0x0ffffeff;  //get temp from AUXADC
#endif
	}
	eem_write(EEMCORESEL, coresel);
#if 0
	eem_debug("[%s] 0x1100bf00=0x%x\n",
			((char *)(det->name) + 8), eem_read(EEMCORESEL));
#endif
}

int base_ops_init01(struct eem_det *det)
{
	det->ops->set_phase(det, EEM_PHASE_INIT01);

	return 0;
}

void base_ops_set_phase(struct eem_det *det, enum eem_phase phase)
{
	unsigned int i, filter, val;
	/* unsigned long flags; */

	det->ops->switch_bank(det, phase);
#if 0
	/* config EEM register */
	eem_write(EEM_DESCHAR,
		  ((det->BDES << 8) & 0xff00) | (det->MDES & 0xff));
	eem_write(EEM_TEMPCHAR,
		  (((det->VCO << 16) & 0xff0000) |
		   ((det->MTDES << 8) & 0xff00) | (det->DVTFIXED & 0xff)));
	eem_write(EEM_DETCHAR,
		  ((det->DCBDET << 8) & 0xff00) | (det->DCMDET & 0xff));
#endif
	eem_write(EEM_DCCONFIG, det->DCCONFIG);
	eem_write(EEM_AGECONFIG, det->AGECONFIG);

	if (det->AGEM == 0x0)
		eem_write(EEM_RUNCONFIG, 0x80000000);
	else {
		val = 0x0;

		for (i = 0; i < 24; i += 2) {
			filter = 0x3 << i;

			if (((det->AGECONFIG) & filter) == 0x0)
				val |= (0x1 << i);
			else
				val |= ((det->AGECONFIG) & filter);
		}
		eem_write(EEM_RUNCONFIG, val);
	}

#if 0
	eem_write(EEM_FREQPCT30,
	((det->freq_tbl[3 * ((det->num_freq_tbl + 7) / 8)] << 24)
		& 0xff000000) |
	((det->freq_tbl[2 * ((det->num_freq_tbl + 7) / 8)] << 16)
		& 0xff0000) |
	((det->freq_tbl[1 * ((det->num_freq_tbl + 7) / 8)] << 8)
		& 0xff00) |
	(det->freq_tbl[0]
		& 0xff));
	eem_write(EEM_FREQPCT74,
	((det->freq_tbl[7 * ((det->num_freq_tbl + 7) / 8)] << 24)
		& 0xff000000)	|
	((det->freq_tbl[6 * ((det->num_freq_tbl + 7) / 8)] << 16)
		& 0xff0000)	|
	((det->freq_tbl[5 * ((det->num_freq_tbl + 7) / 8)] << 8)
		& 0xff00)	|
	((det->freq_tbl[4 * ((det->num_freq_tbl + 7) / 8)])
		& 0xff));

	eem_write(EEM_LIMITVALS,
		  ((det->VMAX << 24) & 0xff000000)	|
		  ((det->VMIN << 16) & 0xff0000)	|
		  ((det->DTHI << 8) & 0xff00)		|
		  (det->DTLO & 0xff));
#endif
	/* eem_write(EEM_LIMITVALS, 0xFF0001FE); */
	eem_write(EEM_VBOOT, (((det->VBOOT) & 0xff)));
	eem_write(EEM_DETWINDOW, (((det->DETWINDOW) & 0xffff)));
	eem_write(EEMCONFIG, (((det->DETMAX) & 0xffff)));
	/* for two line */
	eem_write(EEM_CHKSHIFT, (0x77 & 0xff));

	/* eem ctrl choose thermal sensors */
	eem_write(EEM_CTL0, det->EEMCTL0);
	/* clear all pending EEM interrupt & config EEMINTEN */
	eem_write(EEMINTSTS, 0xffffffff);

	/* work around set thermal register
	 * eem_write(EEM_THERMAL, 0x200);
	 */

	/* eem_debug(" %s set phase = %d\n", ((char *)(det->name) + 8), phase); */
	switch (phase) {
	case EEM_PHASE_INIT01:
		eem_write(EEMINTEN, 0x00005f01);
		/* enable EEM INIT measurement */
		eem_write(EEMEN, 0x00000001 | SEC_MOD_SEL);
#if 0
		eem_debug("EEMEN = 0x%x, EEMINTEN = 0x%x\n",
				eem_read(EEMEN), eem_read(EEMINTEN));
		eem_debug("[base_ops_set_phase 1]EEMODINTST:0x%x, EEMINTSTS:0x%x\n",
		eem_read(EEMODINTST), eem_read(EEMINTSTS));
#endif
		udelay(140); /* all banks' phase cannot be set without delay */
#if 0
		eem_debug("[base_ops_set_phase 2]EEMODINTST:0x%x, EEMINTSTS:0x%x\n",
		eem_read(EEMODINTST), eem_read(EEMINTSTS));
#endif
		break;

	default:
		break;
	}
	/* mt_ptp_unlock(&flags); */
}

#if EEM_CHECK_VOLT
int base_ops_get_volt(struct eem_det *det)
{
	eem_debug("base_ops_get_volt\n");

	return 0;
}

int base_ops_volt_2_eem(struct eem_det *det, int volt)
{
	eem_debug("base_ops_volt_2_eem\n");

	return (((volt) - det->eem_v_base + det->eem_step - 1) / det->eem_step);
}


static int get_volt_soc(struct eem_det *det)
{
	int value = 80000;

	eem_debug("get_volt_soc\n");

	return value;
}
#endif

unsigned int eem_get_soc_det_cnt(int typeidx)
{
	return detcount[typeidx];
}


#define BASE_OP(fn)	.fn = base_ops_ ## fn
static struct eem_det_ops eem_det_base_ops = {
	BASE_OP(switch_bank),
	BASE_OP(init01),
	BASE_OP(set_phase),
#if EEM_CHECK_VOLT
	BASE_OP(get_volt),
	BASE_OP(volt_2_eem),
#endif
};

static struct eem_det_ops soc_det_ops = {
#if EEM_CHECK_VOLT
	.get_volt		= get_volt_soc,
#endif
};

struct eem_det eem_detectors[NR_EEM_DET] = {
	[0] = {
		.name		= "EEM_DET_SOC",
		.ops		= &soc_det_ops,
		.realptpid	= 6,//EEM_CTRL_SOC,
		.features	= FEA_INIT01,
		.VBOOT		= VBOOT_VAL, /* 10uV */
		.DETWINDOW	= DETWINDOW_VAL,
		.DTHI		= DTHI_VAL,
		.DTLO		= DTLO_VAL,
		.DETMAX		= DETMAX_VAL,
		.AGECONFIG	= AGECONFIG_VAL,
		.AGEM		= AGEM_VAL,
		.VMAX		= VMAX_VAL,
		.VMIN		= VMIN_VAL,
#if EEM_CHECK_VOLT
		.eem_v_base = EEM_V_BASE,
		.eem_step	= EEM_STEP,
		.pmic_base	= CPU_PMIC_BASE_6359,
		.pmic_step	= CPU_PMIC_STEP,
#endif
		.DCCONFIG	= DCCONFIG_VAL,
	},
};

static inline void handle_init_err_isr(struct eem_det *det)
{

	eem_debug("====================================================\n");
	eem_notice("EEM init err: EEMEN(%p) = 0x%X, EEMINTSTS(%p) = 0x%X\n",
			 EEMEN, eem_read(EEMEN),
			 EEMINTSTS, eem_read(EEMINTSTS));
	eem_debug("EEM_SMSTATE0 (%p) = 0x%X\n",
			 EEM_SMSTATE0, eem_read(EEM_SMSTATE0));
	eem_debug("EEM_SMSTATE1 (%p) = 0x%X\n",
			 EEM_SMSTATE1, eem_read(EEM_SMSTATE1));
	eem_debug("====================================================\n");


}

static void handle_init01_isr(struct eem_det *det)
{
#if 0
	eem_debug("[handle_init01_isr 1]EEMODINTST:0x%x, EEMINTSTS:0x%x\n",
		eem_read(EEMODINTST), eem_read(EEMINTSTS));
#endif
	/* det->dcvalues[EEM_PHASE_INIT01]		= eem_read(EEM_DCVALUES); */
	det->eem_eemEn[EEM_PHASE_INIT01]	= eem_read(EEMEN);
	det->DCVOFFSETIN = ((eem_read(EEM_DCVALUES) >> 16) & 0xffff);

#if 0
	if (det->DCVOFFSETIN == 0)
		eem_debug("handle_init01_isr incorrect det count 0\n");
	eem_debug("mode = init1 %s-isr, DC:0x%x\n", ((char *)(det->name) + 8),
		(eem_read(EEM_DCVALUES) >> 16) & 0xffff);
#endif
	/*
	 * Set EEMEN.EEMINITEN/EEMEN.EEMINIT2EN = 0x0 &
	 * Clear EEM INIT interrupt EEMINTSTS = 0x00000001
	 */
	eem_write(EEMEN, 0x0 | SEC_MOD_SEL);
	eem_write(EEMINTSTS, 0x1);
#if 0
	eem_debug("[handle_init01_isr 2]EEMODINTST:0x%x, EEMINTSTS:0x%x\n",
		eem_read(EEMODINTST), eem_read(EEMINTSTS));
#endif
}

static void eem_isr_handler(struct eem_det *det)
{
	unsigned int eemintsts, eemen;

	eemintsts = eem_read(EEMINTSTS);
	eemen = eem_read(EEMEN);
#if 0
	eem_debug("[eem_isr_handler 1]EEMODINTST:0x%x, EEMINTSTS:0x%x\n",
		eem_read(EEMODINTST), eem_read(EEMINTSTS));
	eem_debug("Bk_# = %d %s-isr, 0x%X, 0x%X\n",
		det->realptpid, ((char *)(det->name) + 8), eemintsts, eemen);
#endif

	if (eemintsts == 0x1) { /* EEM init1 or init2 */
		if ((eemen & 0x7) == 0x1) /* EEM init1 */
			handle_init01_isr(det);
		else if ((eemen & 0x7) == 0x5) {/* EEM init2 */
			eem_debug("Should not receive init2 isr\n");
		} else {
			/* error : init1 or init2 */
			handle_init_err_isr(det);
		}
	} else if ((eemintsts & 0x00ff0000) != 0x0) {
		eem_debug("Should not receive mon isr\n");
	} else { /* EEM error handler */
		/* init 1  || init 2 error handler */
		if (((eemen & 0x7) == 0x1) || ((eemen & 0x7) == 0x5))
			handle_init_err_isr(det);
		else {/* EEM Monitor mode error handler */
			eem_debug("Should not receive mon isr\n");
		}
	}

}


static void eem_isr(void)
{
	unsigned long flags;
	struct eem_det *det = NULL;
	int i;

	for (i = 0; i < NR_EEM_CTRL; i++) {
		det = &eem_detectors[i];
#if 0
		eem_debug("[eem_isr 1]EEMODINTST:0x%x, EEMINTSTS:0x%x\n",
			eem_read(EEMODINTST), eem_read(EEMINTSTS));
#endif
		if ((BIT(det->realptpid) & eem_read(EEMODINTST)))
			continue;

		det->ops->switch_bank(det, NR_EEM_PHASE);
		eem_isr_handler(det);
	}
#if 0
	eem_debug("[eem_isr 2]EEMODINTST:0x%x, EEMINTSTS:0x%x\n",
		eem_read(EEMODINTST), eem_read(EEMINTSTS));
#endif
}

static void inherit_base_det(struct eem_det *det)
{
	/*
	 * Inherit ops from eem_det_base_ops if ops in det is NULL
	 */

	#define INIT_OP(ops, func)					\
		do {							\
			if (ops->func == NULL)				\
				ops->func = eem_det_base_ops.func;	\
		} while (0)


	INIT_OP(det->ops, switch_bank);
	INIT_OP(det->ops, init01);
	INIT_OP(det->ops, set_phase);
#if EEM_CHECK_VOLT
	INIT_OP(det->ops, get_volt);
	INIT_OP(det->ops, volt_2_eem);
#endif
}

static void eem_init_det(struct eem_det *det)
{
	enum eem_det_id det_id = det_to_id(det);

	inherit_base_det(det);

	/* get DVFS frequency table */
#if 0
	if (det->ops->get_freq_table)
		det->ops->get_freq_table(det);
#endif
}

void eem_init01(int testcnt)
{
	struct eem_det *det;
	//struct eem_ctrl *ctrl;
	unsigned int out = 0, timeout = 0;
	int ret;

	for_each_det(det) {
		if (HAS_FEATURE(det, FEA_INIT01)) {
#if 0
			if (det->ops->get_volt != NULL) {
				det->ops->get_volt(det);
				det->real_vboot = det->ops->volt_2_eem
					(det, det->ops->get_volt(det));
			}
			while (det->real_vboot != det->VBOOT) {
				det->real_vboot = det->ops->volt_2_eem(det,
						det->ops->get_volt(det));
				/* eem_debug("@%s():%d, get_volt(%s) = 0x%08X,"
				 * "VBOOT = 0x%08X\n",
				 *__func__, __LINE__,
				 *det->name, det->real_vboot, det->VBOOT);
				 */
			}
			/* BUG_ON(det->real_vboot != det->VBOOT); */
			WARN_ON(det->real_vboot != det->VBOOT);
#endif
			det->eem_eemEn[EEM_PHASE_INIT01] = 0;
			det->DCVOFFSETIN = 0;
			det->DCCONFIG = arrtype[testcnt];

			det->ops->init01(det);
		}
	}


	/* This patch is waiting for whole bank finish the init01 then go
	 * next. Due to LL/L use same bulk PMIC, LL voltage table change
	 * will impact L to process init01 stage, because L require a
	 * stable 1V for init01.
	 */
	while (1) {
		for_each_det(det) {
			eem_isr();
			if (((out & BIT(det->realptpid)) == 0) &&
					(det->eem_eemEn[EEM_PHASE_INIT01] ==
					(1 | SEC_MOD_SEL))) {
				out |= BIT(det->realptpid);

				if (det->DCVOFFSETIN != 0)
					detcount[testcnt] = det->DCVOFFSETIN;
				else {
					eem_debug("eem_init01 incorrect det count [%d]\n", testcnt);
					break;
				}
			}
		}

		if (out == EEM_INIT01_FLAG)
			break;

		udelay(20);
		timeout++;

		if (timeout % 300 == 0)
			eem_notice
			("init01 wait time is %d, bankmask:0x%x\n",
			timeout, out);
	}

}

int eem_probe(void)
{
	int ret, i, socbin, status = 0;
	struct eem_det *det;

	eem_notice("HRID: 0x%x, 0x%x, 0x%x, 0x%x\n", eem_read(HRID0_ADDR),
		eem_read(HRID1_ADDR), eem_read(HRID2_ADDR), eem_read(HRID3_ADDR));

	socbin = eem_read(SOC_VB_ADDR);
	eem_notice("SOC_VB 0p65/0p725/0p825: 0x%x, 0x%x, 0x%x\n", (socbin & 0x7),
		((socbin >> 3) & 0x7), ((socbin >> 6) & 0x7));

	ret = mtk_regulator_get("vcore", &eem_regulator_vcore); /* instantiate vproc */
	if (ret)
		eem_debug("Get regulotor_vcore error\n");
	else {
		if (!mtk_regulator_is_enabled(&eem_regulator_vcore)) /* vproc not enabled */
			mtk_regulator_enable(&eem_regulator_vcore, 1); /* enable vproc */
		mtk_regulator_set_mode(&eem_regulator_vcore, 1); /* vproc PWM mode */
	}

	for_each_det(det)
		eem_init_det(det);

	for (i = 0; i < 4; i++) {
		eem_init01(i);
		if (detcount[i] == 0)
			status = -1;
	}

	/* SOC post-process */
	mtk_regulator_set_mode(&eem_regulator_vcore, 0);

	/* Check Det value */
	if (status == -1) {
		for (i = 0; i < 4; i++)
			detcount[i] = 0;
	}

	eem_notice("Det count: 0x%x, 0x%x, 0x%x, 0x%x\n", detcount[0],
		detcount[1], detcount[2], detcount[3]);

	return 0;
}
