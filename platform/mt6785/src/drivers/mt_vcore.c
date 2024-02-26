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

#define TAG	"xxxxEEM"
#define EEM_INFO(str, ...) do {             \
	print("[%s] " str, TAG, ##__VA_ARGS__); \
} while(0)

#define VCORE_NR_FREQ_EFUSE     (16) 
#define VCORE_NR_FREQ           (4)

/* SOC v1 Voltage (10uv)*/
static unsigned int vcore_opp_L4_2CH[VCORE_NR_FREQ][VCORE_NR_FREQ_EFUSE] = {
	{79375, 78750, 78125, 77500,
	 76875, 76250, 75625, 75000,
	 74375, 73750, 73125, 72500,
	 71875, 71250, 70625, 70000},

	{71875, 71250, 70625, 70000,
	 69375, 71875, 71875, 71875,
	 71875, 71875, 71875, 71875,
	 71875, 71875, 71875, 71875},

	{71875, 71250, 70625, 70000,
	 69375, 71875, 71875, 71875,
	 71875, 71875, 71875, 71875,
	 71875, 71875, 71875, 71875},

	{71875, 71250, 70625, 70000,
	 69375, 71875, 71875, 71875,
	 71875, 71875, 71875, 71875,
	 71875, 71875, 71875, 71875},
};

static unsigned int vcore_opp_L3_1CH[VCORE_NR_FREQ][VCORE_NR_FREQ_EFUSE] = {
	{79375, 78750, 78125, 77500,
	 76875, 76250, 75625, 75000,
	 74375, 73750, 73125, 72500,
	 71875, 71250, 70625, 70000},

	{79375, 78750, 78125, 77500,
	 76875, 76250, 75625, 75000,
	 74375, 73750, 73125, 72500,
	 71875, 71250, 70625, 70000},

	{71875, 71250, 70625, 70000,
	 69375, 71875, 71875, 71875,
	 71875, 71875, 71875, 71875,
	 71875, 71875, 71875, 71875},

	{71875, 71250, 70625, 70000,
	 69375, 71875, 71875, 71875,
	 71875, 71875, 71875, 71875,
	 71875, 71875, 71875, 71875},
};

/* ptr that points to v1 or v2 opp table */
unsigned int (*vcore_opp)[VCORE_NR_FREQ_EFUSE];

/* final vcore opp table */
unsigned int eem_vcore[VCORE_NR_FREQ];

/* record index for vcore opp table from efuse */
unsigned int eem_vcore_index[VCORE_NR_FREQ] = {0};

unsigned int get_vcore_ptp_volt(unsigned int ddr_type, unsigned int emi_ch_num, unsigned int vcore_oppidx) {
	unsigned int soc_efuse = 0, mask_f = 0xF, mask_7 = 0x7;
	int i = 0;

	if (vcore_oppidx >= VCORE_NR_FREQ)
		return 0;

	if (ddr_type == TYPE_LPDDR4X && emi_ch_num == 2)
		vcore_opp = &vcore_opp_L4_2CH[0];
	else if (ddr_type == TYPE_LPDDR3 && emi_ch_num == 1)
		vcore_opp = &vcore_opp_L3_1CH[0];
	else {
		EEM_INFO("@%s():%d, ddr_type = %d, emi_ch_num = %d\n",
			__func__, __LINE__, ddr_type, emi_ch_num);
	}

	EEM_INFO("@%s():%d, ddr_type = %d, emi_ch_num = %d\n",
		  __func__, __LINE__, ddr_type, emi_ch_num);

	soc_efuse = seclib_get_devinfo_with_index(17);
	if (ddr_type == TYPE_LPDDR4X && emi_ch_num == 2) {
		eem_vcore_index[0] = soc_efuse & mask_f;
		eem_vcore_index[1] = (soc_efuse >> 4) & mask_7;
		eem_vcore_index[2] = (soc_efuse >> 4) & mask_7;
		eem_vcore_index[3] = (soc_efuse >> 4) & mask_7;
	} else {
		eem_vcore_index[0] = soc_efuse & mask_f;
		eem_vcore_index[1] = soc_efuse & mask_f;
		eem_vcore_index[2] = (soc_efuse >> 4) & mask_7;
		eem_vcore_index[3] = (soc_efuse >> 4) & mask_7;
	}
	EEM_INFO("eem_vcore_index:%d, %d, %d, %d\n", eem_vcore_index[0], eem_vcore_index[1],
				eem_vcore_index[2], eem_vcore_index[3]);

	for (i = 0; i < VCORE_NR_FREQ; i++) {
		eem_vcore[i] = *(vcore_opp[i]+eem_vcore_index[i]);
		EEM_INFO("eem_vcore[%d] = [%d] ----- 1\n", i, eem_vcore[i]);
	}
	
	for (i = VCORE_NR_FREQ - 2; i >= 0; i--) {
		eem_vcore[i] = (eem_vcore[i] < eem_vcore[i+1]) ? eem_vcore[i+1] : eem_vcore[i];
		EEM_INFO("eem_vcore[%d] = [%d] ----- 2\n", i, eem_vcore[i]);
	}

	return eem_vcore[vcore_oppidx];
}
