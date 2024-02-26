/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */

/* MediaTek Inc. (C) 2017. All rights reserved.
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

#ifndef __BOOTIMG_H__
#define __BOOTIMG_H__

#include <pal_typedefs.h>

#define BOOTIMG_MAGIC    "ANDROID!"
#define BOOTIMG_MAGIC_SZ (8)
#define BOOTIMG_NAME_SZ  (16)
#define BOOTIMG_ARGS_SZ  (512)

#define BOOTIMG_HDR_SZ   (0x800)

/*
 ** +-----------------+
 ** | boot header     | 1 page
 ** +-----------------+
 ** | kernel          | n pages
 ** +-----------------+
 ** | ramdisk         | m pages
 ** +-----------------+
 ** | second stage    | o pages
 ** +-----------------+
 **
 ** n = (kernel_size + page_size - 1) / page_size
 ** m = (ramdisk_size + page_size - 1) / page_size
 ** o = (second_size + page_size - 1) / page_size
 **
 ** 0. all entities are page_size aligned in flash
 ** 1. kernel and ramdisk are required (size != 0)
 ** 2. second is optional (second_size == 0 -> no second)
 ** 3. load each element (kernel, ramdisk, second) at
 **    the specified physical address (kernel_addr, etc)
 ** 4. prepare tags at tag_addr.  kernel_args[] is
 **    appended to the kernel commandline in the tags.
 ** 5. r0 = 0, r1 = MACHINE_TYPE, r2 = tags_addr
 ** 6. if second_size != 0: jump to second_addr
 **    else: jump to kernel_addr
 */

struct bootimg_hdr
{
	uint8_t  magic[BOOTIMG_MAGIC_SZ];
	uint32_t kernel_sz;  /* size in bytes */
	uint32_t kernel_addr;  /* physical load addr */
	uint32_t ramdisk_sz; /* size in bytes */
	uint32_t ramdisk_addr; /* physical load addr */
	uint32_t second_sz;  /* size in bytes */
	uint32_t second_addr;  /* physical load addr */
	uint32_t tags_addr;    /* physical addr for kernel tags */
	uint32_t page_sz;    /* flash page size we assume */
	uint32_t unused;       /* future expansion: should be 0 */
	uint32_t os_version;
	char name[BOOTIMG_NAME_SZ]; /* asciiz product name */
	char cmdline[BOOTIMG_ARGS_SZ];
	uint32_t id[8]; /* timestamp / checksum / sha1 / etc */
};

#endif /* __BOOTIMG_H__ */

