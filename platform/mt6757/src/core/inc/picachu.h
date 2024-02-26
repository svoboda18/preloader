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

#ifndef PICACHU_H
#define PICACHU_H

#include "platform.h"
#include "print.h"
#include "spm_mtcmos.h"
#include "typedefs.h"
#include "wdt.h"
#include "ram_console.h"
#include "rtc.h"
#include "mt_rtc_hw.h"
#include "upmu_hw.h"

#define DO_ISOLATION	1

#define NR_OPPS	1
#define NR_CPUS	8

/*
 * PMIC_WRAP
 */
/* for Vsram */
#if defined (PMIC_CHIP_MT6355)  /* Kibo+ (MT6355) */
#define VOLT_TO_PMIC_VAL(volt)          (((volt) - 51875 + 625 - 1) / 625)
#define PMIC_VAL_TO_VOLT(val)           ((val) * 625 + 51875)
#else /* Kibo/Olympus */
#define VOLT_TO_PMIC_VAL(volt)          (((volt) - 60000 + 625 - 1) / 625)
#define PMIC_VAL_TO_VOLT(val)           ((val) * 625 + 60000)
#endif

/* for Vproc */
#if defined (PMIC_CHIP_MT6355)  /* Kibo+ (MT6355) */
#define VOLT_TO_EXTBUCK_VAL(volt)       (((volt) - 40625 + 625 - 1) / 625)
#define EXTBUCK_VAL_TO_VOLT(val)        ((val) * 625 + 40625)
#else /* Kibo/Olympus */
#define VOLT_TO_EXTBUCK_VAL(volt)       (((volt) - 60000 + 625 - 1) / 625)
#define EXTBUCK_VAL_TO_VOLT(val)        ((val) * 625 + 60000)
#endif

#define MAX(a, b) ((a) >= (b) ? (a) : (b))
#define MIN(a, b) ((a) >= (b) ? (b) : (a))

/*
 * BIT Operation
 */
#define _BITMASK_(_bits_)               (((unsigned) -1 >> (31 - ((1) ? _bits_))) & ~((1U << ((0) ? _bits_)) - 1))
#define _GET_BITS_VAL_(_bits_, _val_)   (((_val_) & (_BITMASK_(_bits_))) >> ((0) ? _bits_))

#define NORMAL_DIFF_VRSAM_VPROC	10000	/* 100mv * 100 */
#define MAX_DIFF_VSRAM_VPROC	30000	/* 300mv * 100 */
#define MIN_VSRAM_VOLT		85000	/* 800mv * 100 */
#define MAX_VSRAM_VOLT		105000	/* 1050mv * 100 */
#define MIN_VPROC_VOLT		60000
#define MAX_VPROC_VOLT		102500
#define MAX_VPROC_PMIC		VOLT_TO_EXTBUCK_VAL(MAX_VPROC_VOLT)
#define MIN_VPROC_PMIC		VOLT_TO_EXTBUCK_VAL(MIN_VPROC_VOLT)
#define MAX_VSRAM_PMIC		VOLT_TO_PMIC_VAL(MAX_VSRAM_VOLT)
#define MIN_VSRAM_PMIC		VOLT_TO_PMIC_VAL(MIN_VSRAM_VOLT)

#define KB_MAX_VPROC_VOLT	107500
#define KB_MAX_VSRAM_VOLT	111875
#define KB_MAX_VPROC_PMIC	VOLT_TO_EXTBUCK_VAL(KB_MAX_VPROC_VOLT)
#define KB_MAX_VSRAM_PMIC	VOLT_TO_PMIC_VAL(KB_MAX_VSRAM_VOLT)

#define KBP_MAX_VPROC_VOLT	111875
#define KBP_MAX_VSRAM_VOLT	111875
#define KBP_MAX_VPROC_PMIC \
		VOLT_TO_EXTBUCK_VAL(KBP_MAX_VPROC_VOLT)
#define KBP_MAX_VSRAM_PMIC \
		VOLT_TO_PMIC_VAL(KBP_MAX_VSRAM_VOLT)

#define BUG()								\
	do {								\
		print("BUG: %s line %d\n", __FILE__, __LINE__);	\
		while (1)						\
			;						\
	} while (0)

#define TAG 			"[PICACHU] "

//#define DEBUG_LOG
#ifdef DEBUG_LOG
#define PICACHU_DEBUG(str, ...) do { print(TAG str,  ##__VA_ARGS__); } while(0)
#else
#define PICACHU_DEBUG(str, ...) do {} while(0)
#endif

#define PICACHU_INFO(str, ...) do { print(TAG str,  ##__VA_ARGS__); } while(0)

/* Check core1-7 in WFE mode. */
#define SLAVE_CORES_IN_WFE      0xFE

#define CORE_POWER_ON	(1 << 0)
#define CORE_POWER_DOWN	(1 << 1)

enum mt_cluster_id {
        MT_CLUSTER_LL,
        MT_CLUSTER_L,
	MT_CLUSTER_CCI,

        NR_CLUSTERS,
};

enum mt_picachu_ops {
	PICACHU_DVFS_OPS,
	PICACHU_CLUSTER_OPS,

	NR_PICACHU_OPS
};

enum mt_udi_mbist_result {
	UDI_MBIST_RESULT_NO_NOTHING,
	UDI_MBIST_RESULT_L1_FAIL,
	UDI_MBIST_RESULT_L2_FAIL,
	UDI_MBIST_RESULT_ALL_FAIL,
	UDI_MBIST_RESULT_PASS
};

typedef enum {
        CHIP_HW_VER_OE1 = 0xCA00, 	/* Olympus E1 */
        CHIP_HW_VER_OE2 = 0xCA01, 	/* Olympus E2 */
        CHIP_HW_VER_KIBO = 0xCB00, 	/* Kibo */
} CHIP_HW_VER;

struct cpu_opp_info {
	unsigned int freq;
	unsigned int ckdiv;
	unsigned int postdiv;
	unsigned int stored_armpll_con1_val;
	unsigned int stored_ckdiv_sel;
};

struct cpu_margin {
	unsigned int customer;
	unsigned int temp;
	unsigned int slt;
	unsigned int aging;
	unsigned int test;
	unsigned int theory_gap;
};

struct cpu_info {
	const char *name;

	/*
	 * Index 0: OPP table for little-little cluster.
	 * Index 1: OPP table for little cluster.
	 */
	struct cpu_opp_info cpu_opp_info[NR_OPPS * NR_CLUSTERS];
	struct cpu_margin *margin;
	unsigned int dvtfixed;
	unsigned int max_vproc_pmic;
	unsigned int min_vproc_pmic;
	unsigned int max_vsram_pmic;
	unsigned int min_vsram_pmic;
	unsigned int stored_volt;
};

struct mt_armpll_addr {
	unsigned int armpll_con0;
	unsigned int armpll_con1;
	unsigned int clkdiv;
};

struct mt_cpu_dvfs_ops {
        unsigned int (*get_cur_freq) (struct mt_armpll_addr *);

        unsigned int (*get_cur_volt) (void);
        unsigned int (*get_cur_vsram) (void);

        void (*set_cur_volt) (struct cpu_info *cpu_info, unsigned int);

	struct cpu_info * (*get_cpu_info) (void);
};

struct mt_cluster_ops {
	int (*ctrl_slave_cpus) (unsigned int);
	void (*cfg_boot_addr) (unsigned int);
	int (*check_spm_pwr_status) (void);

#if DO_ISOLATION
	void (*cluster_ctrl_iso) (int);
	void (*mp1_cluster_iso_prepare) (void);
#endif
};

#endif /* PICACHU_H */
