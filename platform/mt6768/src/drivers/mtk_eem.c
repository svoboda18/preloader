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

#define TAG	"EEM"
#define EEM_INFO(str, ...) do {             \
	print("[%s] " str, TAG, ##__VA_ARGS__); \
} while(0)

unsigned int get_final_vcore_opp(unsigned int index)
{
	/* original vcore opp */
	/* 84000, 79000, 74000, 69000 are original vcore voltages, others are voltage bin voltages */

	unsigned int soc_efuse, ret;
	unsigned int vcore_opp_2[4][4] = {
		{84375, 81875, 79375, 76875},
		{79375, 76875, 74375, 71875},
		{74375, 73750, 71250, 68750},
		{69375, 66875, 64375, 61875},
	};

	soc_efuse = seclib_get_devinfo_with_index(38);
	/* EEM_INFO("soc_efuse = 0x%x\n", soc_efuse); */
	switch (index) {
		case 0:
			ret = vcore_opp_2[0][(soc_efuse >> 6) & 0x3];
			break;
		case 1:
			ret = vcore_opp_2[1][(soc_efuse >> 4) & 0x3];
			break;
		case 2:
			ret = vcore_opp_2[2][(soc_efuse >> 2) & 0x3];
			break;
		case 3:
			ret = vcore_opp_2[3][(soc_efuse >> 0) & 0x3];
			break;
		default:
			ret = 0;
			break;
	}
	/* EEM_INFO("get final_vcore_opp(%x): %d\n", index, ret); */

	return ret;
}
