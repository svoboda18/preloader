/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2015. All rights reserved.
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
 */
#include "platform.h"
#include "rpmb_cust_key.h"
#include "key_derive.h"
#include "rpmb_key.h"
#include "tz_mem.h"
#if (CFG_BOOT_DEV == BOOTDEV_UFS)
#include "ufs_aio_rpmb.h"
#endif

#define RPMB_KEY_SIZE (32)

/* Get chip key if RPMB_PER_DEVICE_KEY is defined, common key otherwise. */
int rpmb_get_key(unsigned char *rpmbkey, int key_size)
{
	//get common key
	const unsigned char rpmb_common_key[RPMB_KEY_SIZE] = {RPMB_CUST_KEY};
	static int is_initialized = 0;
	static unsigned char backup_rpmb_key[RPMB_KEY_SIZE] = {0};

	if (key_size != RPMB_KEY_SIZE) {
		return RPMB_KEY_LEN_ERROR;
	}

	/* Copy backup_rpmb_key to rpmbkey if is_initialized */
	if (is_initialized) {
		memcpy(rpmbkey, backup_rpmb_key, RPMB_KEY_SIZE);
		return RPMB_NO_ERROR;
	}

#if defined(RPMB_PER_DEVICE_KEY)
#ifdef MTK_SECURITY_SW_SUPPORT
	unsigned int ret = 0;
	ret = seclib_key_derive(KEY_TYPE_RPMB, rpmbkey, RPMB_KEY_SIZE);
	if (ret)
		return RPMB_KEY_DRIVE_ERROR;
#else
	return RPMB_SECURITY_SW_NOT_SUPPORT;
#endif
#else //!defined(RPMB_PER_DEVICE_KEY), use common key
	memcpy(rpmbkey, rpmb_common_key, RPMB_KEY_SIZE);
#endif //defined(RPMB_PER_DEVICE_KEY)

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
	mmc_rpmb_set_key(rpmb_key);
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
	ufs_rpmb_authen_key_program(rpmb_key, NULL);
#endif

#endif //(CFG_BOOT_DEV == BOOTDEV_UFS)
	return 0;
}
