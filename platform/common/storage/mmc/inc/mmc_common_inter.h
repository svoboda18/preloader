/* Copyright Statement:
*
* This software/firmware and related documentation ("MediaTek Software") are
* protected under relevant copyright laws. The information contained herein
* is confidential and proprietary to MediaTek Inc. and/or its licensors.
* Without the prior written permission of MediaTek inc. and/or its licensors,
* any reproduction, modification, use or disclosure of MediaTek Software,
* and information contained herein, in whole or in part, shall be strictly prohibited.
*/
/* MediaTek Inc. (C) 2018. All rights reserved.
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

#ifndef __MMC_COMMON_INTER_H__
#define __MMC_COMMON_INTER_H__

#if defined(MMC_MSDC_DRV_PRELOADER)
#include "typedefs.h"
#endif
#if defined(MMC_MSDC_DRV_PRELOADER) || defined(MMC_MSDC_DRV_LK)
#include "platform.h"
#endif

extern u32 mmc_init_device (void);
extern int mmc_switch_part(u32 part_id);
extern void msdc_apply_timing(unsigned int id);
extern int mmc_get_boot_part(u32 *bootpart);
extern int mmc_set_boot_part(u32 bootpart);

#ifdef MTK_EMMC_SUPPORT_OTP
typedef enum {
	EMMC_OTP_UNLOCKED = 0,
	EMMC_OTP_LOCKED,
	EMMC_OTP_OTHER_STATUS
} OTP_STATUS;

typedef enum {
	EMMC_OTP_GP0,
	EMMC_OTP_GP1,
	EMMC_OTP_GP2,
	EMMC_OTP_GP3
} OTP_WP_GROUP;

u32 mmc_wrap_otp_get_size(u32 group_id);
u32 mmc_wrap_otp_lock(u32 group_id);
u32 mmc_wrap_otp_status(u32 group_id, u32 *status);
int mmc_wrap_otp_write(u32 group_id, u64 offset, uchar *buf, u64 size);
int mmc_wrap_otp_read(u32 group_id, u64 offset, uchar *buf, u64 size);
#endif /* MTK_EMMC_SUPPORT_OTP */
#endif /* __MMC_COMMON_INTER_H__ */
