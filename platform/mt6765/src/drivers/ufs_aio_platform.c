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

#include "ufs_aio_cfg.h"
#include "ufs_aio_platform.h"
#include "ufs_aio_hcd.h"
#include "ufs_aio_utils.h"

#ifdef UFS_CFG_HQA_MODE
void ufs_aio_hqa_mode(void)
{
	unsigned int reg;

	print("[UFS HQA] Configure environment\n");

	/* enable MPHY VA09 */

	print("[UFS HQA] Enable MPHY VA09\n");
	reg = readl(0x10003448);
	reg = reg & ~(0x1 << 1);
	reg = reg | (0x1 << 1);
	writel(reg, 0x10003448);
	print("[UFS HQA] - 0x10003448: 0x%x\n", readl(0x10003448));

	/* GPIO for I2C */

	print("[UFS HQA] Configure GPIO for I2C\n");

	/*
	 * GPIO126: UFS_MPHY_SCL (Mode 6)
	 * GPIO127: UFS_MPHY_SDA (Mode 6)
	 */
	writel(0x66111111, 0x100053F0);
	print("[UFS HQA] - 0x100053F0: 0x%x\n", readl(0x100053F0));

	/*
	 * GPIO128: UFS_UNIPRO_SDA (Mode 6)
	 * GPIO129: UFS_UNIPRO_SCL (Mode 6)
	 */
	writel(0x11111166, 0x10005400);
	print("[UFS HQA] - 0x10005400: 0x%x\n", readl(0x10005400));

	/* MPHY I2C Mux */

	print("[UFS HQA] Configure MPHY I2C Mux\n");

	reg = readl(0x11C500F0);
	reg = reg & ~(0x3 << 0);
	reg = reg | (0x3 << 0);
	writel(reg, 0x11C500F0);
	print("[UFS HQA] - 0x11C500F0: 0x%x\n", readl(0x11C500F0));

	/* Enable Debug Monitor B */

	print("[UFS HQA] Configure Debug Monitor for UFS\n");

	reg = readl(0x100056D0);
	reg = reg & ~(0x1F << 8);
	reg = reg | (0x06 << 8);
	writel(reg, 0x100056D0);
	print("[UFS HQA] - 0x100056D0: 0x%x\n", readl(0x100056D0));
	writel(0x07000111, 0x10005340);
	print("[UFS HQA] - 0x10005340: 0x%x\n", readl(0x10005340));
	writel(0x00007770, 0x10005350);
	print("[UFS HQA] - 0x10005350: 0x%x\n", readl(0x10005350));
	writel(0x77777711, 0x10005370);
	print("[UFS HQA] - 0x10005370: 0x%x\n", readl(0x10005370));
	writel(0x11117777, 0x10005380);
	print("[UFS HQA] - 0x10005380: 0x%x\n", readl(0x10005380));
	writel(0x77111111, 0x10005420);
	print("[UFS HQA] - 0x10005420: 0x%x\n", readl(0x10005420));

	/* Disable watchdog */

	print("[UFS HQA] Disable watchdog\n");

	writel(0x22000000, 0x10007000);
	print("[UFS HQA] - 0x10007000: 0x%x\n", readl(0x10007000));

	print("[UFS HQA] Configure environment done\n");
	print("[UFS HQA] Infinite loop, happy testing ...\n");

	while (1);
}
#endif

int ufs_aio_init_mphy(struct ufs_hba *hba)
{
	return 0;
}


