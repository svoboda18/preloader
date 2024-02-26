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
{800000, 800000, 800000, 800000, 800000, 800000, 800000, 800000, 800000, 768750, 768750, 768750, 768750, 768750, 768750, 768750}, /* (x + 625) * 10 */
{700000, 700000, 700000, 700000, 700000, 700000, 700000, 700000, 700000, 668750, 668750, 668750, 668750, 668750, 668750, 668750}, /* (x + 625) * 10 - 25000 */
{700000, 700000, 700000, 700000, 700000, 700000, 700000, 700000, 700000, 668750, 668750, 668750, 668750, 668750, 668750, 668750}, /* (x + 625) * 10 - 25000 */
{700000, 700000, 700000, 700000, 700000, 700000, 700000, 700000, 700000, 668750, 668750, 668750, 668750, 668750, 668750, 668750} /* (x + 625) * 10 - 25000 */
};

static unsigned int vcore_opp_L3_1CH[VCORE_NR_FREQ][VCORE_NR_FREQ_EFUSE] = {
{800000, 800000, 800000, 800000, 800000, 800000, 800000, 800000, 800000, 768750, 768750, 768750, 768750, 768750, 768750, 768750}, /* (x + 625) * 10 */
{800000, 800000, 800000, 800000, 800000, 800000, 800000, 800000, 800000, 768750, 768750, 768750, 768750, 768750, 768750, 768750}, /* (x + 625) * 10 */
{700000, 700000, 700000, 700000, 700000, 700000, 700000, 700000, 700000, 668750, 668750, 668750, 668750, 668750, 668750, 668750}, /* (x + 625) * 10 - 25000 */
{700000, 700000, 700000, 700000, 700000, 700000, 700000, 700000, 700000, 668750, 668750, 668750, 668750, 668750, 668750, 668750}, /* (x + 625) * 10 - 25000 */
};

/* ptr that points to v1 or v2 opp table */
unsigned int (*vcore_opp)[VCORE_NR_FREQ_EFUSE];

/* final vcore opp table */
unsigned int eem_vcore[VCORE_NR_FREQ];

/* record index for vcore opp table from efuse */
unsigned int eem_vcore_index[VCORE_NR_FREQ] = {0};

unsigned int get_vcore_ptp_volt(unsigned int ddr_type, unsigned int vcore_oppidx) {
	unsigned int soc_efuse = 0, mask = 0xF, backup_2 = 0, backup_3 = 0;
	int i = 0;

	if (vcore_oppidx >= VCORE_NR_FREQ)
		return 0;

	if (ddr_type == TYPE_LPDDR4X)
		vcore_opp = &vcore_opp_L4_2CH[0];
	else
		vcore_opp = &vcore_opp_L3_1CH[0];

	/* EEM_INFO("@%s():%d, ddr_type = %d\n",
		  __func__, __LINE__, ddr_type); */

	backup_2 = seclib_get_devinfo_with_index(38) & 0xf; /* for 0.7V */
	backup_3 = seclib_get_devinfo_with_index(39) & 0xf; /* for 0.8V */

	if (backup_3 != 0) {
		soc_efuse = (backup_2 << 4) | backup_3 ;
	} else {
		soc_efuse = 0; /* seclib_get_devinfo_with_index(17); */
	}

	if (ddr_type == TYPE_LPDDR4X) {
		eem_vcore_index[0] = soc_efuse & mask;
		eem_vcore_index[1] = (soc_efuse >> 4) & mask;
		eem_vcore_index[2] = (soc_efuse >> 4) & mask;
		eem_vcore_index[3] = (soc_efuse >> 4) & mask;
	} else {
		eem_vcore_index[0] = soc_efuse & mask;
		eem_vcore_index[1] = soc_efuse & mask;
		eem_vcore_index[2] = (soc_efuse >> 4) & mask;
		eem_vcore_index[3] = (soc_efuse >> 4) & mask;
	}
	/* EEM_INFO("eem_vcore_index:%d, %d, %d, %d\n", eem_vcore_index[0], eem_vcore_index[1],
				eem_vcore_index[2], eem_vcore_index[3]); */

	for (i = 0; i < VCORE_NR_FREQ; i++) {
		eem_vcore[i] = *(vcore_opp[i]+eem_vcore_index[i]);
		/* EEM_INFO("eem_vcore[%d] = [%d] ----- 1\n", i, eem_vcore[i]); */
	}

	/* read efuse (0x11F107C0[8]) to patch opp-1 */
	if ((seclib_get_devinfo_with_index(25) & 0x100) || ((seclib_get_devinfo_with_index(9) & 0x3) == 0x01))
		eem_vcore[1] = eem_vcore[0];

	for (i = VCORE_NR_FREQ - 2; i >= 0; i--) {
		eem_vcore[i] = (eem_vcore[i] < eem_vcore[i+1]) ? eem_vcore[i+1] : eem_vcore[i];
		/* EEM_INFO("eem_vcore[%d] = [%d] ----- 2\n", i, eem_vcore[i]); */
	}

	return eem_vcore[vcore_oppidx];
}
