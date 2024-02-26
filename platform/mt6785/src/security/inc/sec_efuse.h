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

#ifndef SEC_EFUSE_H
#define SEC_EFUSE_H

#define EFUSE_BLOW_KEY1			0x90b1e2f6
#define EFUSE_BLOW_KEY2			0x73d5a8c9

struct efuse_param {
	U32 magic_key1;
	U32 magic_key2;
	U32 reserved;
	bool  clear_part_dis;
	bool  enable_self_blow;
	bool  reserved2;
	bool  reserved3;
};

typedef struct efuse_part_info {
	U64 part_base;
	U32 part_id;
	blkdev_t *bootdev;
	bool  initialized;
	bool  reserved1;
	bool  reserved2;
	bool  reserved3;
} EFUSE_PART_INFO;

/**************************************************************************
 * EXPORTED FUNCTIONS
 **************************************************************************/
/* Partition & Storage */
extern int efuse_storage_init(void);
extern int efuse_storage_read(U64 efuse_part_start_offset, U32 size, U8 *dst);
extern int efuse_storage_write(U64 efuse_part_start_offset, U32 size, U8 *src);

/* WDT */
extern void efuse_wdt_init(void);
extern void efuse_wdt_restart(void);
extern void efuse_wdt_sw_reset(void);

/* DDR reserved mode */
extern int efuse_dram_reserved(int enable);

/* PLL */
extern void efuse_pll_set(void);

/* Vbat */
extern int efuse_check_lowbat(void);

/* Fsource */
extern U32 efuse_fsource_set(void);
extern U32 efuse_fsource_is_enabled(void);
extern U32 efuse_fsource_close(void);

/* Vcore */
extern U32 efuse_vcore_blow(void);
extern U32 efuse_vcore_high(void);
extern U32 efuse_vcore_low(void);

/* Others */
extern int efuse_module_reinit(void);

#endif /* SEC_EFUSE_H */

