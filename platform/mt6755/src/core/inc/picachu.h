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

#define NR_OPPS	1
#define NR_CPUS	8

#define VOLT_TO_PMIC_VAL(volt)  (((volt) - 60000 + 625 - 1) / 625)	/* ((((volt) - 700 * 100 + 625 - 1) / 625) */
#define PMIC_VAL_TO_VOLT(pmic)  (((pmic) * 625) + 60000)	/* (((pmic) * 625) / 100 + 700) */

#define VOLT_TO_EXTBUCK_VAL(volt)   VOLT_TO_PMIC_VAL(volt)	/* (((((volt) - 300) + 9) / 10) & 0x7F) */
#define EXTBUCK_VAL_TO_VOLT(val)    PMIC_VAL_TO_VOLT(val)	/* (300 + ((val) & 0x7F) * 10) */

#define MAX(a, b) ((a) >= (b) ? (a) : (b))
#define MIN(a, b) ((a) >= (b) ? (b) : (a))

/*
 * BIT Operation
 */
#define _BITMASK_(_bits_)               (((unsigned) -1 >> (31 - ((1) ? _bits_))) & ~((1U << ((0) ? _bits_)) - 1))
#define _GET_BITS_VAL_(_bits_, _val_)   (((_val_) & (_BITMASK_(_bits_))) >> ((0) ? _bits_))

#define NORMAL_DIFF_VRSAM_VPROC    10000   /* 100mv * 100 */
#define MAX_DIFF_VSRAM_VPROC       20000  /* 200mv * 100 */
#define MIN_VSRAM_VOLT             93000  /* 931.25mv * 100 */
#define MAX_VSRAM_VOLT             115000  /* 1150mv * 100 */
#define MIN_VPROC_VOLT             80000
#define MAX_VPROC_VOLT             115000
#define VMAX_CPU_VOLT              VOLT_TO_PMIC_VAL(MAX_VPROC_VOLT)
#define VMIN_CPU_VOLT              VOLT_TO_PMIC_VAL(MIN_VPROC_VOLT)

#define BUG()								\
	do {								\
		print("BUG: %s line %d\n", __FILE__, __LINE__);	\
		while (1)						\
			;						\
	} while (0)

#define TAG 			"PICACHU"

//#define DEBUG_LOG
#ifdef DEBUG_LOG
#define PICACHU_DEBUG(str, ...) do { print("[%s] " str, TAG, ##__VA_ARGS__); } while(0)
#else
#define PICACHU_DEBUG(str, ...) do {} while(0)
#endif

#define PICACHU_INFO(str, ...) do { print("[%s] " str, TAG, ##__VA_ARGS__); } while(0)

enum picachu_cal_status {
        PICACHU_DO_CAL = 1,
        PICACHU_SKIP_CAL,
};

enum mt_cluster_id {
        MT_CLUSTER_LL,
        MT_CLUSTER_L,

        NR_CLUSTERS,
};

struct cpu_opp_info {
	int freq;
	int pll;
	int ckdiv;
	int postdiv;
};

struct cpu_info {
	const char *name;

	/*
	 * Index 0: OPP table for little-little cluster.
	 * Index 1: OPP table for little cluster.
	 */
	struct cpu_opp_info cpu_opp_info[NR_OPPS * NR_CLUSTERS];
	int vmin_hi;
	int vmin_lo;
};

struct mt_armpll_addr {
	unsigned int armpll_con0;
	unsigned int armpll_con1;
	unsigned int clkdiv;
};

struct mt_cpu_dvfs_ops {
        unsigned int (*get_cur_freq)(struct mt_armpll_addr *);

        unsigned int (*get_cur_volt)(void);
        unsigned int (*get_cur_vsram)(void);

        void (*set_cur_volt)(unsigned int);
};

#endif /* PICACHU_H */
