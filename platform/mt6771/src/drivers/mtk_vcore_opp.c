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
#include "dramc_pi_api.h"

#define TAG	"Vcore_OPP"
#define pr_err(str, ...) do {             \
	print("[%s]ERR " str, TAG, ##__VA_ARGS__); \
} while(0)

#define pr_info(str, ...) do {             \
	print("[%s]INFO " str, TAG, ##__VA_ARGS__); \
} while(0)

#define VCORE_OPP_EFUSE_NUM     (2)
#define VCORE_OPP_NUM           (4)
//#define max(a, b)               (a > b ? a : b)

/* SOC v1 Voltage (10uv)*/
#ifdef MTK_LP4_DDR3200
static unsigned int vcore_opp_L4_2CH[VCORE_OPP_NUM][VCORE_OPP_EFUSE_NUM] = {
#ifdef MTK_QEA_VCORE_SUPPORT
	{ 756250, 756250 },
	{ 687500, 687500 },
	{ 687500, 687500 },
	{ 687500, 687500 },
#else
	{ 800000, 800000 },
	{ 725000, 700000 },
	{ 725000, 700000 },
	{ 725000, 700000 },
#endif
};
#else
static unsigned int vcore_opp_L4_2CH[VCORE_OPP_NUM][VCORE_OPP_EFUSE_NUM] = {
#ifdef MTK_QEA_VCORE_SUPPORT
	{ 756250, 756250 },
	{ 756250, 756250 },
	{ 687500, 687500 },
	{ 687500, 687500 },
#else
	{ 800000, 800000 },
	{ 800000, 800000 },
	{ 725000, 700000 },
	{ 725000, 700000 },
#endif
};
#endif

static unsigned int vcore_opp_L3_1CH[VCORE_OPP_NUM][VCORE_OPP_EFUSE_NUM] = {
#ifdef MTK_QEA_VCORE_SUPPORT
	{ 756250, 756250 },
	{ 756250, 756250 },
	{ 687500, 687500 },
	{ 687500, 687500 },
#else
	{ 800000, 800000 },
	{ 800000, 800000 },
	{ 725000, 700000 },
	{ 725000, 700000 },
#endif
};

static opp_table_init = 0;

/* ptr that points to v1 or v2 opp table */
unsigned int (*vcore_opp)[VCORE_OPP_EFUSE_NUM];

/* final vcore opp table */
unsigned int vcore_opp_table[VCORE_OPP_NUM];

/* record index for vcore opp table from efuse */
unsigned int vcore_opp_efuse_idx[VCORE_OPP_NUM] = { 0 };

static int get_soc_efuse(void)
{
	return 0;
	//return ((seclib_get_devinfo_with_index(40) >> 12) & 0x3);
}

void build_vcore_opp_table(unsigned int ddr_type, unsigned int soc_efuse)
{
	int i, mask = 0x3;

	if (soc_efuse > 1) {
		pr_err("WRONG VCORE EFUSE(%d)\n", soc_efuse);
		for (i = 0; i < VCORE_OPP_NUM; i++)
			vcore_opp_table[i] = *(vcore_opp[i]); /* set to default table */
		return;
	}

	if (ddr_type == TYPE_LPDDR4X) {
		if (seclib_get_devinfo_with_index(30) & 0x20) {
			/* treat as MTK_LP4_DDR3200 by efuse */
			vcore_opp_L4_2CH[1][0] = 725000;
			vcore_opp_L4_2CH[1][1] = 700000;
		}
		vcore_opp = &vcore_opp_L4_2CH[0];
		vcore_opp_efuse_idx[0] = 0; /* 0.8V, no corner tightening*/
#ifdef MTK_LP4_DDR3200
		vcore_opp_efuse_idx[1] = soc_efuse & mask; /* 0.7V */
#else
		vcore_opp_efuse_idx[1] = 0; /* 0.8V */
#endif
		vcore_opp_efuse_idx[2] = soc_efuse & mask; /* 0.7V */
		vcore_opp_efuse_idx[3] = soc_efuse & mask; /* 0.7V */
	} else if (ddr_type == TYPE_LPDDR3) {
		vcore_opp = &vcore_opp_L3_1CH[0];
		vcore_opp_efuse_idx[0] = 0; /* 0.8V, no corner tightening*/
		vcore_opp_efuse_idx[1] = 0; /* 0.8V, no corner tightening*/
		vcore_opp_efuse_idx[2] = soc_efuse & mask; /* 0.7V */
		vcore_opp_efuse_idx[3] = soc_efuse & mask; /* 0.7V */
	} else {
		pr_err("gregre: %d\n", ddr_type);
		return;
	}

	for (i = 0; i < VCORE_OPP_NUM; i++)
		vcore_opp_table[i] = *(vcore_opp[i] + vcore_opp_efuse_idx[i]);

	for (i = VCORE_OPP_NUM - 2; i >= 0; i--)
		vcore_opp_table[i] = max(vcore_opp_table[i], vcore_opp_table[i + 1]);

	pr_info("table: %d, %d, %d, %d\n",
		vcore_opp_table[0], vcore_opp_table[1], vcore_opp_table[2], vcore_opp_table[3]);
}

unsigned int get_vcore_opp_volt(unsigned int ddr_type, unsigned int opp)
{
	if (opp >= VCORE_OPP_NUM) {
		pr_err("WRONG OPP: %u\n", opp);
		return 0;
	}

	if (!opp_table_init) {
		build_vcore_opp_table(ddr_type, get_soc_efuse());
		opp_table_init = 1;
	}

	pr_info("return vcore: %d (ddr=%d, opp=%d, efsule=%d)\n",
		vcore_opp_table[opp], ddr_type, opp, get_soc_efuse());

	return vcore_opp_table[opp];
}

