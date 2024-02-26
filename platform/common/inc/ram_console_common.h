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
 * MediaTek Inc. (C) 2010. All rights reserved.
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

#ifndef __RAM_CONSOLE_COMMON_H__
#define __RAM_CONSOLE_COMMON_H__
#include <typedefs.h>

#ifndef __RAM_CONSOLE_H__
#error "Please DON'T include this internal common file directly and use #include <ram_console.h> instead"
#endif

enum RAM_CONSOLE_DEF_TYPE {
    RAM_CONSOLE_DEF_UNKNOWN = 0,
    RAM_CONSOLE_DEF_SRAM,
    RAM_CONSOLE_DEF_DRAM,
};

#define MEM_MAGIC1 0x61646472 // "addr"
#define MEM_MAGIC2 0x73697a65 // "size"
/*
 * ram console sram layout description:
 * |ram console|plt_dbg_info|reserved|memory_info|log_store|mrdump(i.e. fulldump)|
 * ram console dram layout description:
 * |ram console dram memory|pstore|mrdump mini header|
 */
struct ram_console_memory_info {
	u32 magic1;
	u32 sram_plat_dbg_info_addr;
	u32 sram_plat_dbg_info_size;
	u32 sram_log_store_addr;
	u32 sram_log_store_size;
	u32 mrdump_addr;
	u32 mrdump_size;
	u32 dram_addr;
	u32 dram_size;
	u32 pstore_addr;
	u32 pstore_size;
	u32 pstore_console_size;
	u32 pstore_pmsg_size;
	u32 mrdump_mini_header_addr;
	u32 mrdump_mini_header_size;
	u32 magic2;
};

#if (RAM_CONSOLE_SRAM_SIZE_OVERALL == 0xC00) || (RAM_CONSOLE_SRAM_SIZE_OVERALL == 0xE00)
	#define RAM_CONSOLE_PLAT_DBG_INFO_SIZE (0x200)
#elif (RAM_CONSOLE_SRAM_SIZE_OVERALL == 0x1000)
	#define RAM_CONSOLE_PLAT_DBG_INFO_SIZE (0x400)
#else
	#error "Illegal ram_console overall size defined, need check ..."
#endif

#define RAM_CONSOLE_PLAT_DBG_INFO_OFFSET (RAM_CONSOLE_SRAM_SIZE)
#define RAM_CONSOLE_PLAT_DBG_INFO_ADDR (RAM_CONSOLE_SRAM_ADDR + RAM_CONSOLE_PLAT_DBG_INFO_OFFSET)
#define RAM_CONSOLE_SRAM_RESERVED_OFFSET (RAM_CONSOLE_SRAM_SIZE + RAM_CONSOLE_PLAT_DBG_INFO_SIZE)

#define RAM_CONSOLE_LOG_STORE_SIZE (0x100)
#define RAM_CONSOLE_LOG_STORE_OFFSET (RAM_CONSOLE_SRAM_SIZE_OVERALL - RAM_CONSOLE_LOG_STORE_SIZE)
#define RAM_CONSOLE_LOG_STORE_ADDR (RAM_CONSOLE_SRAM_ADDR + RAM_CONSOLE_LOG_STORE_OFFSET)
#define RAM_CONSOLE_MEMORY_INFO_OFFSET (RAM_CONSOLE_LOG_STORE_OFFSET - sizeof(struct ram_console_memory_info))

#ifdef RAM_CONSOLE_MRDUMP_SIZE_SPECIAL
#define RAM_CONSOLE_MRDUMP_SIZE RAM_CONSOLE_MRDUMP_SIZE_SPECIAL
#else
#define RAM_CONSOLE_MRDUMP_SIZE (RAM_CONSOLE_MRDUMP_SRAM_SIZE_OVERALL - RAM_CONSOLE_SRAM_SIZE_OVERALL)
#endif
#define RAM_CONSOLE_MRDUMP_ADDR (RAM_CONSOLE_SRAM_ADDR + RAM_CONSOLE_SRAM_SIZE_OVERALL)

/* tricky code here for the reason that sizeof can't be used for macro comparing */
#define RAM_CONSOLE_DUMMY_CHECK(e) enum { e_dummy = 1/(!(e)) }
RAM_CONSOLE_DUMMY_CHECK(RAM_CONSOLE_SRAM_RESERVED_OFFSET > RAM_CONSOLE_MEMORY_INFO_OFFSET);

#define PSTORE_ADDR PL_PSTORE_BASE
#define PSTORE_SIZE PL_PSTORE_MAX_SIZE
#define PSTORE_CONSOLE_SIZE (0x40000)
#define PSTORE_PMSG_SIZE (0x10000)
#define MRDUMP_MINI_HEADER_ADDR PL_MRDUMP_MINI_HEADER_BASE
#define MRDUMP_MINI_HEADER_SIZE PL_MRDUMP_MINI_HEADER_MAX_SIZE

#if CFG_RAM_CONSOLE
extern void ram_console_mblock_reserve(void);
extern void ram_console_init(void);
extern void ram_console_reboot_reason_save(u32 rgu_status);
extern void ram_console_pl_save(unsigned int val, int index);
extern bool ram_console_is_abnormal_boot(void);
extern void ram_console_sram_addr_size(u32 *addr, int *size);
extern u32 ram_console_def_memory(void);
extern u32 ram_console_memory_info_offset(void);
#else
static void ram_console_mblock_reserve(void)
{
}

static void ram_console_init(void)
{
    print("ram_console disabled\n");
}

static void ram_console_reboot_reason_save(u32 rgu_status)
{
}

static void ram_console_pl_save(unsigned int val, int index)
{
}

static bool ram_console_is_abnormal_boot(void)
{
	return false;
}

static void ram_console_sram_addr_size(u32 *addr, int *size)
{
}

static u32 ram_console_def_memory(void)
{
	return RAM_CONSOLE_DEF_UNKNOWN;
}

static u32 ram_console_memory_info_offset(void)
{
	return 0;
}
#endif

#endif // #ifndef __RAM_CONSOLE_COMMON_H__
