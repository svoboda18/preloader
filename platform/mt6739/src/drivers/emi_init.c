/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
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
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

#include <typedefs.h>
#include <platform.h>
#include <stdlib.h>
#include <emi.h>
#include <dram_buffer.h>

#define EMI_APB_BASE    0x10219000
#define EMI_CHANNEL_APB_BASE 0x1021A000
#define MD_BANK_PRE_MASK  1

void emi_init(void)
{
//C:-------------------BEGIN EMI Setting--------------------
// Row = 15-bit
//write 0x00000000 0x0002a0a2
//write 0x00000000 0x0000a0a2
//single rank
//write 0x00000000 0x0000a052 
//dual rank
 *(volatile unsigned int *)(EMI_APB_BASE+0x00000000)=0x0002a0a2;
 *(volatile unsigned int *)(EMI_APB_BASE+0x00000008)=0x0d1e293a;
 *(volatile unsigned int *)(EMI_APB_BASE+0x00000010)=0x09190819;
 *(volatile unsigned int *)(EMI_APB_BASE+0x00000018)=0x3657587a;
 *(volatile unsigned int *)(EMI_APB_BASE+0x00000020)=0xffff0848;
 *(volatile unsigned int *)(EMI_APB_BASE+0x00000028)=0x00000000;
 *(volatile unsigned int *)(EMI_APB_BASE+0x00000030)=0x2b2b2a38;
//write 0x00000038 0x00ff0000
 *(volatile unsigned int *)(EMI_APB_BASE+0x00000038)=0x00400000; // rank1: 1GB
 *(volatile unsigned int *)(EMI_APB_BASE+0x00000040)=0x00008806;
//C: turn-off EMI DCM
//write 0x00000060 0x002806b0
#ifdef SW_PDE
 *(volatile unsigned int *)(EMI_APB_BASE+0x00000060)=0x008806b0;
#else
 *(volatile unsigned int *)(EMI_APB_BASE+0x00000060)=0x002806b0;
#endif
 *(volatile unsigned int *)(EMI_APB_BASE+0x00000068)=0x00000000;
 *(volatile unsigned int *)(EMI_APB_BASE+0x00000078)=0x11118c37;
 *(volatile unsigned int *)(EMI_APB_BASE+0x0000007c)=0x00001111;
 *(volatile unsigned int *)(EMI_APB_BASE+0x000000d0)=0xa8a8a8a8;
 *(volatile unsigned int *)(EMI_APB_BASE+0x000000d4)=0x25252525;
 *(volatile unsigned int *)(EMI_APB_BASE+0x000000d8)=0xa8a8a8a8;
 *(volatile unsigned int *)(EMI_APB_BASE+0x000000dc)=0x25252525;
 *(volatile unsigned int *)(EMI_APB_BASE+0x000000e8)=0x00060027;
 *(volatile unsigned int *)(EMI_APB_BASE+0x000000f0)=0x00000000;
 *(volatile unsigned int *)(EMI_APB_BASE+0x000000f8)=0x80000000; // ex_mode=1
/////  EMI  bandwidth limit  ////////// default UI
//  UI
 *(volatile unsigned int *)(EMI_APB_BASE+0x00000100)=0x4f0f704a;
 *(volatile unsigned int *)(EMI_APB_BASE+0x00000108)=0x7f7f5050;
 *(volatile unsigned int *)(EMI_APB_BASE+0x00000110)=0xffff7041;
 *(volatile unsigned int *)(EMI_APB_BASE+0x00000118)=0x0f0f7057;
 *(volatile unsigned int *)(EMI_APB_BASE+0x00000120)=0x80806048; // adjust MDHW starvation to 0x80
 *(volatile unsigned int *)(EMI_APB_BASE+0x00000128)=0xffff7046;
 *(volatile unsigned int *)(EMI_APB_BASE+0x00000130)=0x0;
 *(volatile unsigned int *)(EMI_APB_BASE+0x00000138)=0x0;
#ifdef SW_PDE
 *(volatile unsigned int *)(EMI_APB_BASE+0x00000100)=0xffff7045;
 *(volatile unsigned int *)(EMI_APB_BASE+0x00000108)=0x9f6f505C; // adjust MM1 read starvation to 0x6f
 *(volatile unsigned int *)(EMI_APB_BASE+0x00000110)=0xffff7041;
 *(volatile unsigned int *)(EMI_APB_BASE+0x00000118)=0x20207056; // adjust MDMCH starvation to 0x20
 *(volatile unsigned int *)(EMI_APB_BASE+0x00000120)=0x80806047; // adjust MDHW starvation to 0x80
 *(volatile unsigned int *)(EMI_APB_BASE+0x00000128)=0xffff7041;
 *(volatile unsigned int *)(EMI_APB_BASE+0x00000130)=0x0;
 *(volatile unsigned int *)(EMI_APB_BASE+0x00000138)=0x0;
#endif

////////////////////////////////////////////////////
 *(volatile unsigned int *)(EMI_APB_BASE+0x00000140)=0x00006108;
 *(volatile unsigned int *)(EMI_APB_BASE+0x00000158)=0xff01ff00;
// *(volatile unsigned int *)(EMI_APB_BASE+0x00000400)=0x00000001;
// *(volatile unsigned int *)(EMI_APB_BASE+0x00000800)=0x55775775;
//C:-------------------END EMI Setting--------------------
//C:-------------------BEGIN CHANNEL EMI Setting--------------------
//write 0x00015000 0x0400a0a3
//write 0x00015000 0x0400a0a2
//single rank
//write 0x00015000 0x0000a052
//dual rank
// *(volatile unsigned int *)(EMI_CHANNEL_APB_BASE+0x00000000)=0x0000a0a3;
 *(volatile unsigned int *)(EMI_CHANNEL_APB_BASE+0x00000008)=0x02000040;
 *(volatile unsigned int *)(EMI_CHANNEL_APB_BASE+0x00000010)=0x00000001;
 *(volatile unsigned int *)(EMI_CHANNEL_APB_BASE+0x00000018)=0x88008c00; // [31]/[27] sync M17
 *(volatile unsigned int *)(EMI_CHANNEL_APB_BASE+0x00000048)=0x00028027; // [15]=1, WRITE_BIG_AGE=1
 *(volatile unsigned int *)(EMI_CHANNEL_APB_BASE+0x00000050)=0x38460002; // [1]: MD_RD_AFT_WR_EN
 *(volatile unsigned int *)(EMI_CHANNEL_APB_BASE+0x00000058)=0x00000000;
 *(volatile unsigned int *)(EMI_CHANNEL_APB_BASE+0x00000140)=0x20406188;
 *(volatile unsigned int *)(EMI_CHANNEL_APB_BASE+0x00000144)=0x20406188;
 *(volatile unsigned int *)(EMI_CHANNEL_APB_BASE+0x00000148)=0x9819595e;
 *(volatile unsigned int *)(EMI_CHANNEL_APB_BASE+0x0000014c)=0x9819595e;
 *(volatile unsigned int *)(EMI_CHANNEL_APB_BASE+0x00000150)=0x64f3fc79;
 *(volatile unsigned int *)(EMI_CHANNEL_APB_BASE+0x00000154)=0x64f3fc79;
//write 0x0001515c 0x00010223
 *(volatile unsigned int *)(EMI_CHANNEL_APB_BASE+0x00000158)=0x00000078; // "no" reserve 1 dramc queue for MDMCU
 *(volatile unsigned int *)(EMI_CHANNEL_APB_BASE+0x0000015c)=0x0002032f;

 *(volatile unsigned int *)(EMI_CHANNEL_APB_BASE+0x000001b0)=0x0006002f; // Rank-Aware arbitration
 *(volatile unsigned int *)(EMI_CHANNEL_APB_BASE+0x000001b4)=0x01010101; // Rank-Aware arbitration
 *(volatile unsigned int *)(EMI_CHANNEL_APB_BASE+0x000001b8)=0x20201840; // Rank-Aware arbitration
#ifdef MD_BANK_PRE_MASK
	#ifdef BANK_MASK_ULTRA_OFF
		 *(volatile unsigned int *)(EMI_CHANNEL_APB_BASE+0x00000080)=0xb00;
	#else
		 *(volatile unsigned int *)(EMI_CHANNEL_APB_BASE+0x00000080)=0xf00;
	#endif
 *(volatile unsigned int *)(EMI_CHANNEL_APB_BASE+0x00000088)=0xb00;
#endif 
//C:-------------------END CHANNEL EMI Setting--------------------
}

/* Workaround for Zion only */
#define bootarg g_dram_buf->bootarg
void emi_reserve_4k_workaround(void)
{
	unsigned long long rank_size[4] = {0};
	unsigned long long reserve_start, reserve_limit;

#ifndef SLT
	*((volatile unsigned int *)0x10226804) |= 0x10;
	*((volatile unsigned int *)0x1022681C) |= 0x10;
#endif

	if (get_dram_rank_nr() < 2)
		return;

	get_dram_rank_size(rank_size);
	reserve_limit = (unsigned long long) 0x40000000 + rank_size[0];

#ifdef CUSTOM_CONFIG_MAX_DRAM_SIZE
	if (CUSTOM_CONFIG_MAX_DRAM_SIZE < rank_size[0])
		return;
#endif

	reserve_start = mblock_reserve_ext(&bootarg.mblock_info, 0x1000, 0x1000, reserve_limit, 0, "emi-reserved");
	if (reserve_start != (reserve_limit - 0x1000)) {
		print("[EMI] workaround fail (0x%llx)\n", reserve_start);
		ASSERT(0);
	}
}
