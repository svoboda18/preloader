/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2019. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver\'s
 * applicable license agreements with MediaTek Inc.
 */

#include "platform.h"
#include "rpmb_cust_key.h"
#include "key_derive.h"
#include "rpmb_key.h"
#include "tz_mem.h"
#if (CFG_BOOT_DEV == BOOTDEV_UFS) || defined(BOOTDEV_SDMMC_UFS_COMBO)
#include "ufs_aio_rpmb.h"
#endif

#define RPMB_KEY_SIZE (32)
#if CFG_RPMB_KEY_RETRY_IN_PL
char g_rpmb_key[RPMB_KEY_SIZE];
#endif

#if CFG_RPMB_KEY_RETRY_IN_PL
int rpmb_get_wc(u32 *wc, int *rpmb_result)
{
#if (CFG_BOOT_DEV == BOOTDEV_SDMMC) || (CFG_BOOT_DEV == BOOTDEV_UFS)
	u8 *nonce_buf = NULL;
	blkdev_t *bootdev = blkdev_get(CFG_BOOT_DEV);
	if (bootdev == NULL) {
		print("Can't find boot device(%d)\n", CFG_BOOT_DEV);
		return -1;
	}

	if (bootdev->type == BOOTDEV_SDMMC) {
		return mmc_rpmb_get_wc(wc,rpmb_result);
	}
#if (CFG_BOOT_DEV == BOOTDEV_UFS) || defined(BOOTDEV_SDMMC_UFS_COMBO)
	else if (bootdev->type == BOOTDEV_UFS) {
		nonce_buf = (u8 *)malloc(RPMB_FRM_NONCE_LEN);
		memset(nonce_buf, 0, RPMB_FRM_NONCE_LEN);
		return rpmb_authen_read_counter(0, nonce_buf, RPMB_FRM_NONCE_LEN,
	                               wc);
	}
#endif
#endif
	return 0;
}
#endif //CFG_RPMB_KEY_RETRY_IN_PL

/* Return per-device key unless CFG_RPMB_KEY_RETRY_IN_PL define and have program common key */
int rpmb_get_key(unsigned char *rpmbkey, int key_size)
{
	//get common key
	const unsigned char rpmb_common_key[RPMB_KEY_SIZE] = {RPMB_CUST_KEY};
	static int is_initialized = 0;
	static unsigned char backup_rpmb_key[RPMB_KEY_SIZE] = {0};
	int ret;
#if CFG_RPMB_KEY_RETRY_IN_PL
	u32 wc;
	int rpmb_result;
#endif //CFG_RPMB_KEY_RETRY_IN_PL

	if (key_size != RPMB_KEY_SIZE) {
		return RPMB_KEY_LEN_ERROR;
	}

	/* Copy backup_rpmb_key to rpmbkey if is_initialized */
	if (is_initialized) {
		memcpy(rpmbkey, backup_rpmb_key, RPMB_KEY_SIZE);
		return RPMB_NO_ERROR;
	}

#ifdef MTK_SECURITY_SW_SUPPORT
	ret = (int)seclib_key_derive(KEY_TYPE_RPMB, rpmbkey, RPMB_KEY_SIZE);
	if (ret)
		return RPMB_KEY_DRIVE_ERROR;
	/* try per_dev and common key */
#if CFG_RPMB_KEY_RETRY_IN_PL
	/* g_rpmb_key is for storage dirver to get rpmb key */
	/* Copy per_dev key to g_rpmb_key */
	memcpy(g_rpmb_key, rpmbkey, RPMB_KEY_SIZE);
	ret = rpmb_get_wc(&wc,&rpmb_result);
	/*
	 * If per_dev key get rpmb wc fail, try common key.
	 * Otherwise, use per_dev key.
	*/
	if (ret) {
		print("\n RPMB try per_dev key fail, retry common key. ret = %d\n", ret);
		/* g_rpmb_key is for storage dirver to get rpmb key */
		/* Copy common key to g_rpmb_key */
		memcpy(g_rpmb_key, rpmb_common_key, RPMB_KEY_SIZE);
		ret = rpmb_get_wc(&wc,&rpmb_result);
		/* Use common key if common key read rpmb ws success */
		if (!ret) {
			print("\n RPMB retry common key ok!!!\n");
			memcpy(rpmbkey, rpmb_common_key, RPMB_KEY_SIZE);
		}
	}
#endif //CFG_RPMB_KEY_RETRY_IN_PL

#else
	return RPMB_SECURITY_SW_NOT_SUPPORT;
#endif

	/* Copy rpmbkey to backup_rpmb_key, return backup_rpmb_key for next rpmb_get_key() */
	memcpy(backup_rpmb_key, rpmbkey, RPMB_KEY_SIZE);
	is_initialized = 1;
	return RPMB_NO_ERROR;
}

#if (CFG_BOOT_DEV == BOOTDEV_SDMMC)
#if (CFG_TEE_SUPPORT && CFG_RPMB_SET_KEY && CFG_RPMB_KEY_PROGRAMED_IN_KERNEL)
int rpmb_set_flag;
#endif
#endif

int rpmb_program_key(void)
{
#if (CFG_BOOT_DEV == BOOTDEV_SDMMC) // For eMMC project with TEEs
#if (CFG_TEE_SUPPORT && CFG_RPMB_SET_KEY)
	unsigned char rpmb_key[RPMB_KEY_SIZE] = {0};
	int ret = 0;

	ret = rpmb_get_key(&rpmb_key[0], RPMB_KEY_SIZE);
	if (ret) {
		print("\n RPMB storage get key fail ret = %d\n", ret);
		return ret;
	}

#if CFG_RPMB_KEY_PROGRAMED_IN_KERNEL
	int result;

	mmc_rpmb_set_key(rpmb_key, &result);
	rpmb_set_flag = (result == 7) ? 1 : 0;
	//flag = 1 means permitting key program in kernel
	//tbase_secmem_param_rpmb_set_flag(TEE_PARAMETER_ADDR, flag);
#else
#if CFG_RPMB_PROGRAM_KEY
	mmc_rpmb_set_key(rpmb_key);
#else
	print("RPMB not program key\n");
#endif
#endif
#endif


#elif (CFG_BOOT_DEV == BOOTDEV_UFS)
#if defined(MTK_UFS_OTP_SUPPORT) || defined(MTK_UFS_POWP_SUPPORT) || (CFG_TEE_SUPPORT && CFG_RPMB_SET_KEY)
	char rpmb_key[RPMB_KEY_SIZE] = {0};
	int ret = 0;

	ret = rpmb_get_key(&rpmb_key[0], RPMB_KEY_SIZE);
	if (ret) {
		print("\n RPMB storage get key fail ret = %d\n", ret);
		return ret;
	}
#if CFG_RPMB_PROGRAM_KEY
	ufs_rpmb_authen_key_program(rpmb_key);
#else
	print("RPMB not program key\n");
#endif
#endif

#endif //(CFG_BOOT_DEV == BOOTDEV_UFS)
	return 0;
}
