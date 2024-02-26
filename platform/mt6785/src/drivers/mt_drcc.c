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
 * MediaTek Inc. (C) 2010. All rights reserved.
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

/*
	VER 1.0: 20170915 build by Ichang
*/


//xxx, #include <stdio.h>
//xxx, #include <stdlib.h>
//xxx, #include <ostimer.h>

#include "typedefs.h" //xxx, addb by UE
#include "platform.h" //xxx, addb by UE
#include "mt_drcc.h"

unsigned int drcc_base;
unsigned int drcc_config[8][5];

struct drcc_class drcc[8] = {
	[0] = {
		/* 0x0C53B000 */
		.drcc_enable = 0,
		.drcc_code = DRCC_CODE,
		.drcc_hwgatepct = DRCC_HWGATEPCT,
		.drcc_verffilt = DRCC_VERFFILT,
		.drcc_autocalibdelay = DRCC_AUTOCALIBDELAY,

		/* DRCC_CFG_REG0 0x0C530280 */
		.drcc_trigen = DRCC_TRIGEN,
		.drcc_trigsel = DRCC_TRIGSEL,
		.drcc_counten = DRCC_COUNTEN,
		.drcc_countsel = DRCC_COUNTSEL,
		.drcc_cpustandbywfi_mask = DRCC_STANDBYWFI_MASK,
		.drcc_cpustandbywfe_mask = DRCC_STANDBYWFE_MASK,
		.drcc_mode = DRCC_MODE,

		/* DRCC_CFG_REG1 0x0C530284 */
		.drcc_prgclk = 0,
		.drcc_prgwren = 0,
		.drcc_prgaddr = 0,
		.drcc_prgdatawr = 0,
		.drcc_prgdatard = 0,

		/* DRCC_CFG_REG1 0x0C530288 */
		.drcc_forcetrim = DRCC_FORCETRIM,
		.drcc_forcetrimen = DRCC_FORCETRIMEN,
		.drcc_disableautoprtdurcalib = DRCC_DISABLEAUTOPRTDURCALIB,
		.drcc_autocalibdone = 0,
		.drcc_autocaliberror = 0,
		.drcc_autocalibtrim = 0,

		/* DRCC_CFG_REG1 0x0C53028C */
		.drcc_eventcount = 0,
	},

	[1] = {
		/* 0x0C53B200 */
		.drcc_enable = 0,
		.drcc_code = DRCC_CODE,
		.drcc_hwgatepct = DRCC_HWGATEPCT,
		.drcc_verffilt = DRCC_VERFFILT,
		.drcc_autocalibdelay = DRCC_AUTOCALIBDELAY,

		/* DRCC_CFG_REG0 0x0C530A80 */
		.drcc_trigen = DRCC_TRIGEN,
		.drcc_trigsel = DRCC_TRIGSEL,
		.drcc_counten = DRCC_COUNTEN,
		.drcc_countsel = DRCC_COUNTSEL,
		.drcc_cpustandbywfi_mask = DRCC_STANDBYWFI_MASK,
		.drcc_cpustandbywfe_mask = DRCC_STANDBYWFE_MASK,
		.drcc_mode = DRCC_MODE,

		/* DRCC_CFG_REG1 0x0C530A84 */
		.drcc_prgclk = 0,
		.drcc_prgwren = 0,
		.drcc_prgaddr = 0,
		.drcc_prgdatawr = 0,
		.drcc_prgdatard = 0,

		/* DRCC_CFG_REG1 0x0C530A88 */
		.drcc_forcetrim = DRCC_FORCETRIM,
		.drcc_forcetrimen = DRCC_FORCETRIMEN,
		.drcc_disableautoprtdurcalib = DRCC_DISABLEAUTOPRTDURCALIB,
		.drcc_autocalibdone = 0,
		.drcc_autocaliberror = 0,
		.drcc_autocalibtrim = 0,

		/* DRCC_CFG_REG1 0x0C530A8C */
		.drcc_eventcount = 0,
	},

	[2] = {
		/* 0x0C53B400 */
		.drcc_enable = 0,
		.drcc_code = DRCC_CODE,
		.drcc_hwgatepct = DRCC_HWGATEPCT,
		.drcc_verffilt = DRCC_VERFFILT,
		.drcc_autocalibdelay = DRCC_AUTOCALIBDELAY,

		/* DRCC_CFG_REG0 0x0C531280 */
		.drcc_trigen = DRCC_TRIGEN,
		.drcc_trigsel = DRCC_TRIGSEL,
		.drcc_counten = DRCC_COUNTEN,
		.drcc_countsel = DRCC_COUNTSEL,
		.drcc_cpustandbywfi_mask = DRCC_STANDBYWFI_MASK,
		.drcc_cpustandbywfe_mask = DRCC_STANDBYWFE_MASK,
		.drcc_mode = DRCC_MODE,

		/* DRCC_CFG_REG1 0x0C531284 */
		.drcc_prgclk = 0,
		.drcc_prgwren = 0,
		.drcc_prgaddr = 0,
		.drcc_prgdatawr = 0,
		.drcc_prgdatard = 0,

		/* DRCC_CFG_REG1 0x0C531288 */
		.drcc_forcetrim = DRCC_FORCETRIM,
		.drcc_forcetrimen = DRCC_FORCETRIMEN,
		.drcc_disableautoprtdurcalib = DRCC_DISABLEAUTOPRTDURCALIB,
		.drcc_autocalibdone = 0,
		.drcc_autocaliberror = 0,
		.drcc_autocalibtrim = 0,

		/* DRCC_CFG_REG1 0x0C53128C */
		.drcc_eventcount = 0,
	},

	[3] = {
		/* 0x0C53B600 */
		.drcc_enable = 0,
		.drcc_code = DRCC_CODE,
		.drcc_hwgatepct = DRCC_HWGATEPCT,
		.drcc_verffilt = DRCC_VERFFILT,
		.drcc_autocalibdelay = DRCC_AUTOCALIBDELAY,

		/* DRCC_CFG_REG0 0x0C531A80 */
		.drcc_trigen = DRCC_TRIGEN,
		.drcc_trigsel = DRCC_TRIGSEL,
		.drcc_counten = DRCC_COUNTEN,
		.drcc_countsel = DRCC_COUNTSEL,
		.drcc_cpustandbywfi_mask = DRCC_STANDBYWFI_MASK,
		.drcc_cpustandbywfe_mask = DRCC_STANDBYWFE_MASK,
		.drcc_mode = DRCC_MODE,

		/* DRCC_CFG_REG1 0x0C531A84 */
		.drcc_prgclk = 0,
		.drcc_prgwren = 0,
		.drcc_prgaddr = 0,
		.drcc_prgdatawr = 0,
		.drcc_prgdatard = 0,

		/* DRCC_CFG_REG1 0x0C531A88 */
		.drcc_forcetrim = DRCC_FORCETRIM,
		.drcc_forcetrimen = DRCC_FORCETRIMEN,
		.drcc_disableautoprtdurcalib = DRCC_DISABLEAUTOPRTDURCALIB,
		.drcc_autocalibdone = 0,
		.drcc_autocaliberror = 0,
		.drcc_autocalibtrim = 0,

		/* DRCC_CFG_REG1 0x0C531A8C */
		.drcc_eventcount = 0,
	},

	[4] = {
		/* 0x0C53B800 */
		.drcc_enable = 0,
		.drcc_code = DRCC_CODE,
		.drcc_hwgatepct = DRCC_HWGATEPCT,
		.drcc_verffilt = DRCC_VERFFILT,
		.drcc_autocalibdelay = DRCC_AUTOCALIBDELAY,

		/* DRCC_CFG_REG0 0x0C532280 */
		.drcc_trigen = DRCC_TRIGEN,
		.drcc_trigsel = DRCC_TRIGSEL,
		.drcc_counten = DRCC_COUNTEN,
		.drcc_countsel = DRCC_COUNTSEL,
		.drcc_cpustandbywfi_mask = DRCC_STANDBYWFI_MASK,
		.drcc_cpustandbywfe_mask = DRCC_STANDBYWFE_MASK,
		.drcc_mode = DRCC_MODE,

		/* DRCC_CFG_REG1 0x0C532284 */
		.drcc_prgclk = 0,
		.drcc_prgwren = 0,
		.drcc_prgaddr = 0,
		.drcc_prgdatawr = 0,
		.drcc_prgdatard = 0,

		/* DRCC_CFG_REG1 0x0C532288 */
		.drcc_forcetrim = DRCC_FORCETRIM,
		.drcc_forcetrimen = DRCC_FORCETRIMEN,
		.drcc_disableautoprtdurcalib = DRCC_DISABLEAUTOPRTDURCALIB,
		.drcc_autocalibdone = 0,
		.drcc_autocaliberror = 0,
		.drcc_autocalibtrim = 0,

		/* DRCC_CFG_REG1 0x0C53228C */
		.drcc_eventcount = 0,
	},

	[5] = {
		/* 0x0C53BA00 */
		.drcc_enable = 0,
		.drcc_code = DRCC_CODE,
		.drcc_hwgatepct = DRCC_HWGATEPCT,
		.drcc_verffilt = DRCC_VERFFILT,
		.drcc_autocalibdelay = DRCC_AUTOCALIBDELAY,

		/* DRCC_CFG_REG0 0x0C532A80 */
		.drcc_trigen = DRCC_TRIGEN,
		.drcc_trigsel = DRCC_TRIGSEL,
		.drcc_counten = DRCC_COUNTEN,
		.drcc_countsel = DRCC_COUNTSEL,
		.drcc_cpustandbywfi_mask = DRCC_STANDBYWFI_MASK,
		.drcc_cpustandbywfe_mask = DRCC_STANDBYWFE_MASK,
		.drcc_mode = DRCC_MODE,

		/* DRCC_CFG_REG1 0x0C532A84 */
		.drcc_prgclk = 0,
		.drcc_prgwren = 0,
		.drcc_prgaddr = 0,
		.drcc_prgdatawr = 0,
		.drcc_prgdatard = 0,

		/* DRCC_CFG_REG1 0x0C532A88 */
		.drcc_forcetrim = DRCC_FORCETRIM,
		.drcc_forcetrimen = DRCC_FORCETRIMEN,
		.drcc_disableautoprtdurcalib = DRCC_DISABLEAUTOPRTDURCALIB,
		.drcc_autocalibdone = 0,
		.drcc_autocaliberror = 0,
		.drcc_autocalibtrim = 0,

		/* DRCC_CFG_REG1 0x0C532A8C */
		.drcc_eventcount = 0,
	},

	[6] = {
		/* 0x0C53BC00 */
		.drcc_enable = 0,
		.drcc_code = DRCC_CODE,
		.drcc_hwgatepct = DRCC_HWGATEPCT,
		.drcc_verffilt = DRCC_VERFFILT,
		.drcc_autocalibdelay = DRCC_AUTOCALIBDELAY,

		/* DRCC_CFG_REG0 0x0C533280 */
		.drcc_trigen = DRCC_TRIGEN,
		.drcc_trigsel = DRCC_TRIGSEL,
		.drcc_counten = DRCC_COUNTEN,
		.drcc_countsel = DRCC_COUNTSEL,
		.drcc_cpustandbywfi_mask = DRCC_STANDBYWFI_MASK,
		.drcc_cpustandbywfe_mask = DRCC_STANDBYWFE_MASK,
		.drcc_mode = DRCC_MODE,

		/* DRCC_CFG_REG1 0x0C533284 */
		.drcc_prgclk = 0,
		.drcc_prgwren = 0,
		.drcc_prgaddr = 0,
		.drcc_prgdatawr = 0,
		.drcc_prgdatard = 0,

		/* DRCC_CFG_REG1 0x0C533288 */
		.drcc_forcetrim = DRCC_FORCETRIM,
		.drcc_forcetrimen = DRCC_FORCETRIMEN,
		.drcc_disableautoprtdurcalib = DRCC_DISABLEAUTOPRTDURCALIB,
		.drcc_autocalibdone = 0,
		.drcc_autocaliberror = 0,
		.drcc_autocalibtrim = 0,

		/* DRCC_CFG_REG1 0x0C53328C */
		.drcc_eventcount = 0,
	},

	[7] = {
		/* 0x0C53BE00 */
		.drcc_enable = 0,
		.drcc_code = DRCC_CODE,
		.drcc_hwgatepct = DRCC_HWGATEPCT,
		.drcc_verffilt = DRCC_VERFFILT,
		.drcc_autocalibdelay = DRCC_AUTOCALIBDELAY,

		/* DRCC_CFG_REG0 0x0C533A80 */
		.drcc_trigen = DRCC_TRIGEN,
		.drcc_trigsel = DRCC_TRIGSEL,
		.drcc_counten = DRCC_COUNTEN,
		.drcc_countsel = DRCC_COUNTSEL,
		.drcc_cpustandbywfi_mask = DRCC_STANDBYWFI_MASK,
		.drcc_cpustandbywfe_mask = DRCC_STANDBYWFE_MASK,
		.drcc_mode = DRCC_MODE,

		/* DRCC_CFG_REG1 0x0C533A84 */
		.drcc_prgclk = 0,
		.drcc_prgwren = 0,
		.drcc_prgaddr = 0,
		.drcc_prgdatawr = 0,
		.drcc_prgdatard = 0,

		/* DRCC_CFG_REG1 0x0C533A88 */
		.drcc_forcetrim = DRCC_FORCETRIM,
		.drcc_forcetrimen = DRCC_FORCETRIMEN,
		.drcc_disableautoprtdurcalib = DRCC_DISABLEAUTOPRTDURCALIB,
		.drcc_autocalibdone = 0,
		.drcc_autocaliberror = 0,
		.drcc_autocalibtrim = 0,

		/* DRCC_CFG_REG1 0x0C533A8C */
		.drcc_eventcount = 0,
	}
};

void drccOnOff(int on_off, int core)
{
	drcc[core].drcc_enable = (on_off)? 1: 0;
	drcc_write(CPU0_DRCC_A0_CONFIG + (0x200 * core),
		(drcc_read(CPU0_DRCC_A0_CONFIG + (0x200 * core)) &~ 0x01 | drcc[core].drcc_enable));
}

void drccAMUXOUT(void)
{
	unsigned int val;

	val = drcc_read(0x11E70050);
	drcc_write(0x11E70050, val & (0x3)); // set B GPIO analog mode
	drcc_print("0x11E70050 = 0x%x\n", drcc_read(0x11E70050));

	drcc_write(0x11D10040, 0x0); // set L GPIO analog mode
	drcc_print("0x11D10040 = 0x%x\n", drcc_read(0x11D10040));

}

void drcc_init(void)
{
	int i = 0, core = 0;
	for (core = 0; core < CORE_NUM; core++) {
		drcc_print("-----CPU(%d)_DRCC_AO_CONFIG default, reg=0x%x,\t value=0x%x\n",
			core,
			CPU0_DRCC_A0_CONFIG + (0x200 * core),
			drcc_read(CPU0_DRCC_A0_CONFIG + (0x200 * core)));
		drcc_config[core][0] = 0;

		for (i = 1; i < ARRAY_SIZE(drcc_config[core]); i++) {
			/* clear variable */
			drcc_config[core][i] = 0;
			/* read default value */
			#if 0
			drcc_print("-----CPU(%d)_DRCC_CFG_REG(%d) default, reg=0x%x,\t value=0x%x\n",
				core, (i - 1),
				CPU0_DRCC_CFG_REG0 + (0x800 * core)  + ((i - 1) * 4),
				drcc_read(CPU0_DRCC_CFG_REG0 + (0x800 * core)  + ((i - 1) * 4)));
			#endif
		}

		drccOnOff(0, core); /* Disable DRCC */

		drcc_print("CPU (%d) DRCC initial start.\n", core);
		/* Config the init value to DRCC */
		for (i = 0; i < ARRAY_SIZE(drcc_config[core]); i++)
			drcc_config[core][i] = 0;

		drcc_config[core][0] = \
			drcc[core].drcc_code << 4 |\
			drcc[core].drcc_hwgatepct << 12 |\
			drcc[core].drcc_verffilt << 16 |\
			drcc[core].drcc_autocalibdelay << 20;

		drcc_config[core][1] = \
			drcc[core].drcc_trigen << 4 |\
			drcc[core].drcc_trigsel << 5 |\
			drcc[core].drcc_counten << 6 |\
			drcc[core].drcc_countsel << 7 |\
			drcc[core].drcc_cpustandbywfi_mask << 8 |\
			drcc[core].drcc_cpustandbywfe_mask << 9 |\
			drcc[core].drcc_mode << 12;

		drcc_config[core][3] = \
			drcc[core].drcc_forcetrim << 4 |\
			drcc[core].drcc_forcetrimen << 8 |\
			drcc[core].drcc_disableautoprtdurcalib << 9;

		drcc_write(CPU0_DRCC_A0_CONFIG + (0x200 * core),
			drcc_config[core][0]);

		drcc_write(CPU0_DRCC_CFG_REG0 + (0x800 * core),
			drcc_config[core][1]);

		drcc_write(CPU0_DRCC_CFG_REG2 + (0x800 * core),
			drcc_config[core][3]);

		#if defined(SLT)
		drccOnOff(0, core); /* Disable DRCC */
		#else
		drccOnOff(1, core); /* Enable DRCC */
		#endif
		drcc_print("[Inspect Read] CPU(%d) reg=0x%x,\t read=0x%x,\t set=0x%x\n",
			core,
			CPU0_DRCC_A0_CONFIG + (0x200 * core),
			drcc_read(CPU0_DRCC_A0_CONFIG + (0x200 * core)),
			drcc_config[core][0]);

		#if 0
		for (i = 1; i < ARRAY_SIZE(drcc_config[core]); i++) {
			drcc_print("[Inspect Read] CPU(%d) reg=0x%x,\t read=0x%x,\t set=0x%x\n",
				core,
				CPU0_DRCC_CFG_REG0 + (0x800 * core) + ((i-1) * 4),
				drcc_read(CPU0_DRCC_CFG_REG0 + (0x800 * core) + ((i-1) * 4)),
				drcc_config[core][i]);
		}
		#endif
		drcc_print("CPU (%d) DRCC initial finish.\n", core);
	}
}
