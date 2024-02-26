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

/* Include header files */
#include "typedefs.h"
#include "platform.h"
#include "dram_buffer.h"
#include "tz_init.h"
#include "tz_init_v7.h"
#include "tz_sec_reg.h"
//#include "device_apc.h"
#include "tz_mem.h"
#include "sec_devinfo.h"
#include "cust_sec_ctrl.h"
#include "sec.h"
#if CFG_TRUSTONIC_TEE_SUPPORT
#include "tz_tbase.h"
#endif
#if CFG_GOOGLE_TRUSTY_SUPPORT
#include "tz_trusty.h"
#endif
#if CFG_MICROTRUST_TEE_SUPPORT
#include "tz_teei.h"
#include "tz_platform_teei.h"
#endif

#if CFG_MICROTRUST_TEE_LITE_SUPPORT
#include "tz_teei.h"
#include "tz_platform_teei.h"
#endif

#include "sec_devinfo.h"
#include "emi_drv.h"

/**************************************************************************
 *  DEBUG FUNCTIONS
 **************************************************************************/
#define MOD "[TZ_INIT]"

#define TEE_DEBUG
#ifdef TEE_DEBUG
#define DBG_MSG(str, ...) do {print(str, ##__VA_ARGS__);} while(0)
#define DBG_INFO(str, ...) do {print(str, ##__VA_ARGS__);} while(0)
#else
#define DBG_MSG(str, ...) do {} while(0)
#define DBG_INFO(str, ...) do {print(str, ##__VA_ARGS__);} while(0)
#endif

/**************************************************************************
 *  MACROS
 **************************************************************************/
#define TEE_MEM_ALIGNMENT (0x1000)  //4K Alignment
#define TEE_ENABLE_VERIFY (1)

#if CFG_BOOT_ARGUMENT
#define bootarg g_dram_buf->bootarg
#endif

/**************************************************************************
 *  EXTERNAL FUNCTIONS
 **************************************************************************/
extern void tz_sram_sec_init(u32 start);
extern void tz_sec_mem_init(u32 start, u32 end, u32 mpu_region);
extern void tz_dapc_sec_init(void);
extern void tz_dapc_sec_postinit(void);
//extern void tz_set_module_apc(unsigned int module, E_MASK_DOM domain_num , APC_ATTR permission_control);

/**************************************************************************
 *  INTERNAL VARIABLES
 **************************************************************************/
static u32 tee_entry_addr = 0;
static const u8 tee_img_vfy_pubk[MTEE_IMG_VFY_PUBK_SZ] = {MTEE_IMG_VFY_PUBK};

static u8 g_hwuid[16];
static u8 g_hwuid_initialized = 0;
/**************************************************************************
 *  INTERNAL FUNCTIONS
 **************************************************************************/
static u32 trustzone_get_atf_boot_param_addr(void)
{
    return ATF_BOOT_ARG_ADDR;
}

static u32 trustzone_get_tee_boot_param_addr(void)
{
    return TEE_BOOT_ARG_ADDR;
}

static u32 tee_secmem_size = 0;
static u32 tee_secmem_start = 0;
static u32 atf_log_buf_start = 0;
static u32 tee_extra_mem_size = 0;

void tee_get_secmem_start(u32 *addr)
{
#if CFG_TEE_SUPPORT
    *addr = tee_secmem_start - ATF_LOG_BUFFER_SIZE;
#else
    *addr = atf_log_buf_start;
#endif
}

void tee_get_secmem_size(u32 *size)
{
#if CFG_TEE_SUPPORT
    *size = tee_secmem_size + ATF_LOG_BUFFER_SIZE;
#else
    *size = ATF_LOG_BUFFER_SIZE;
#endif
}

void tee_set_entry(u32 addr)
{
    tee_entry_addr = addr;

    DBG_MSG("%s TEE start entry : 0x%x\n", MOD, tee_entry_addr);
}

void tee_set_hwuid(u8 *id, u32 size)
{
    atf_arg_t_ptr teearg = (atf_arg_t_ptr)trustzone_get_atf_boot_param_addr();

    memcpy(teearg->hwuid, id, size);
    memcpy(g_hwuid, id, size);
    g_hwuid_initialized = 1;

    //DBG_MSG("%s MEID : 0x%x, 0x%x, 0x%x, 0x%x\n", MOD, id[0], id[1], id[2], id[3]);
    //DBG_MSG("%s MEID : 0x%x, 0x%x, 0x%x, 0x%x\n", MOD, id[4], id[5], id[6], id[7]);
    //DBG_MSG("%s MEID : 0x%x, 0x%x, 0x%x, 0x%x\n", MOD, id[8], id[9], id[10], id[11]);
    //DBG_MSG("%s MEID : 0x%x, 0x%x, 0x%x, 0x%x\n", MOD, id[12], id[13], id[14], id[15]);
}

int tee_get_hwuid(u8 *id, u32 size)
{
    int ret = 0;

    if (!g_hwuid_initialized)
        return -1;

    memcpy(id, g_hwuid, size);

    return ret;
}

int tee_verify_image(u32 *addr, u32 size)
{
    u32 haddr = *addr; /* tee header address */
    int ret;

    ret = 0;

#if TEE_ENABLE_VERIFY
    /* verify tee image and addr is updated to pointer entry point */
    ret = trustonic_tee_verify(addr, size, tee_img_vfy_pubk);
    if (ret)
        goto verify_end;

    ret = trustonic_tee_decrypt(haddr, size);
#else
    DBG_MSG("%s tee_verify_image : 0x%x, 0x%x (before)\n", MOD, *addr, size);

    *addr = *addr + 0x240;

    DBG_MSG("%s tee_verify_image : 0x%x, 0x%x (after)\n", MOD, *addr, size);
#endif

verify_end:
    if(ret){
        DBG_MSG("%s tee_verify_image : failed (%d)\n", MOD, ret);
        DBG_MSG("%s DUMP 0x%x: 0x%x, 0x%x, 0x%x, 0x%x\n", MOD, *addr,
            *(u32*)(*addr), *(u32*)(*addr+4), *(u32*)(*addr+8), *(u32*)(*addr+12));
        DBG_MSG("%s DUMP 0x%x: 0x%x, 0x%x, 0x%x, 0x%x\n", MOD, *addr+0x240,
            *(u32*)(*addr+0x240), *(u32*)(*addr+0x244), *(u32*)(*addr+0x248),
            *(u32*)(*addr+0x24C));
    }else{
        DBG_MSG("%s tee_verify_image : passed\n", MOD);
    }

    return ret;
}

u32 tee_get_load_addr(u32 maddr)
{
    u32 ret_addr = 0;

#if CFG_TEE_SUPPORT
    if (tee_secmem_start != 0)
        goto allocated;

    tee_extra_mem_size = maddr % TEE_MEM_ALIGNMENT;
    tee_secmem_size = maddr - tee_extra_mem_size;

#if CFG_GOOGLE_TRUSTY_SUPPORT
    tee_secmem_start = TRUSTY_MEM_LOAD_ADDR;
#elif CFG_MICROTRUST_TEE_SUPPORT
	u64 limit_addr = 0xC0000000;
	u64 align_addr = (u64)TEE_MEM_ALIGNMENT;
    /* To fix at the location : (limit_addr - align_addr) */
    /* first allocate for tee region */
	align_addr = 0x10000000;
	limit_addr = 0xA0000000;
	tee_secmem_start = (u32)mblock_reserve(&bootarg.mblock_info,
		(u64)(tee_secmem_size), (u64)align_addr, limit_addr, RANKMAX);
	print("%s microtrust reserved_addr: 0x%x, reserved_size: 0x%x\n",
		MOD, tee_secmem_start, tee_secmem_size);

	if (tee_secmem_start != (limit_addr - align_addr)) {
	print("%s Fail to allocate secure memory at fixed address: 0x%x\n",
		MOD, (u32)(limit_addr - align_addr));
		WARN_ON(1);
	}

    /* second, allocate for atf log buffer */
	limit_addr = limit_addr - align_addr;
	align_addr = (u64)(ATF_LOG_BUFFER_SIZE);
	tee_secmem_start = (u32)mblock_reserve(&bootarg.mblock_info,
	(u64)(ATF_LOG_BUFFER_SIZE), (u64)align_addr, limit_addr, RANKMAX);

	if (tee_secmem_start != (limit_addr - align_addr)) {
	print("%s Fail to allocate atf log buffer at fixed address: 0x%x\n",
		MOD, (u32)(limit_addr - align_addr));
		WARN_ON(1);
	}
#elif CFG_MICROTRUST_TEE_LITE_SUPPORT
        u64 limit_addr = 0xC0000000;
        u64 align_addr = (u64)TEE_MEM_ALIGNMENT;
    /* To fix at the location : (limit_addr - align_addr) */
    /* first allocate for tee region */
        align_addr = 0x10000000;
        limit_addr = 0xA0000000;
        tee_secmem_start = (u32)mblock_reserve(&bootarg.mblock_info,
                (u64)(tee_secmem_size), (u64)align_addr, limit_addr, RANKMAX);
        print("%s microtrust reserved_addr: 0x%x, reserved_size: 0x%x\n",
                MOD, tee_secmem_start, tee_secmem_size);

        if (tee_secmem_start != (limit_addr - align_addr)) {
        print("%s Fail to allocate secure memory at fixed address: 0x%x\n",
                MOD, (u32)(limit_addr - align_addr));
                WARN_ON(1);
        }
#else
	tee_secmem_start = (u32)mblock_reserve(&bootarg.mblock_info,
	(u64)(tee_secmem_size + ATF_LOG_BUFFER_SIZE), (u64)TEE_MEM_ALIGNMENT,
	0x100000000, RANKMAX);
#endif

    if(!tee_secmem_start){
        printf("%s Fail to allocate secure memory: 0x%x, 0x%x\n", MOD,
            (tee_secmem_size + ATF_LOG_BUFFER_SIZE), TEE_MEM_ALIGNMENT);
        return 0;
    }

    atf_log_buf_start = tee_secmem_start;
    tee_secmem_start = tee_secmem_start + ATF_LOG_BUFFER_SIZE;

allocated:
    ret_addr = tee_secmem_start - tee_extra_mem_size;
#endif /* end of CFG_TEE_SUPPORT */

    return ret_addr;
}

static void tee_sec_config(void)
{
    tz_sram_sec_init(CFG_NON_SECURE_SRAM_ADDR);

#if CFG_TEE_SUPPORT
#if CFG_TEE_SECURE_MEM_PROTECTED
    tz_sec_mem_init(tee_entry_addr, tee_entry_addr + tee_secmem_size - 1, SECURE_OS_MPU_REGION_ID);
    DBG_MSG("%s set secure memory protection : 0x%x, 0x%x (%d)\n", MOD, tee_entry_addr,
        tee_entry_addr + tee_secmem_size - 1, SECURE_OS_MPU_REGION_ID);
#endif
#endif
}

void trustzone_pre_init(void)
{
    sec_malloc_buf_reset();

#if CFG_ATF_LOG_SUPPORT
#if !CFG_TEE_SUPPORT
    {
        atf_arg_t_ptr teearg = (atf_arg_t_ptr)trustzone_get_atf_boot_param_addr();
        atf_log_buf_start = (u32)mblock_reserve(&bootarg.mblock_info,
            (u64)ATF_LOG_BUFFER_SIZE, (u64)TEE_MEM_ALIGNMENT,
            0x100000000, RANKMAX);
        if(!atf_log_buf_start){
            printf("%s Fail to allocate atf log buffer: 0x%x, 0x%x\n", MOD,
                ATF_LOG_BUFFER_SIZE, TEE_MEM_ALIGNMENT);
            teearg->atf_log_buf_size = 0;
        }
    }
#endif
#endif
}

#if CFG_TRUSTONIC_TEE_SUPPORT || CFG_GOOGLE_TRUSTY_SUPPORT || \
	CFG_MICROTRUST_TEE_SUPPORT || CFG_MICROTRUST_TEE_LITE_SUPPORT
void trustzone_pre_init_v7(void)
{
    sec_malloc_buf_reset();
    tz_dapc_sec_init();
}

void trustzone_post_init_v7(void)
{
    tz_dapc_sec_postinit();
}

#if CFG_MICROTRUST_TEE_SUPPORT || CFG_MICROTRUST_TEE_LITE_SUPPORT
extern void teei_rtctime_param_prepare(u32 param_addr);
#endif

void trustzone_jump_v7(u32 addr, u32 arg1, u32 arg2)
{
	typedef void (*jump_func_type)(u32 addr, u32 arg1, u32 arg2, u32 arg3)
					__attribute__ ((__noreturn__));
	jump_func_type jump_func;
	u32 full_memory_size = 0;
	u32 HRID[2] = {0};
	sec_mem_arg_t sec_mem_arg;
	tee_v7_arg_t_ptr teearg =
		(tee_v7_arg_t_ptr)trustzone_get_tee_boot_param_addr();

    /* Configure platform's security settings */
	tee_sec_config();

    /* prepare trustonic's TEE arguments */
	HRID[0] = seclib_get_devinfo_with_index(E_AREA12);
	HRID[1] = seclib_get_devinfo_with_index(E_AREA13);
	memcpy(teearg->hwuid, g_hwuid, 16);
	teearg->magic        = TEE_ARGUMENT_MAGIC;
	/* Trustonic's TEE magic number */
	teearg->version      = TEE_ARGUMENT_VERSION;
	/* Trustonic's TEE argument block version */
	teearg->NWEntry      = addr;
	/* NW Entry point after t-base */
	teearg->NWBootArgs   = arg1;
	/* NW boot args (propagated by t-base in r4 before jump) */
	teearg->NWBootArgsSize = arg2;
	/* NW boot args size (propagated by t-base in r5 before jump) */
	teearg->dRamBase     = CFG_DRAM_ADDR;
	/* DRAM base address */
	teearg->dRamSize     = platform_memory_size();
	/* Full DRAM size */
	teearg->secDRamBase  = tee_entry_addr;/* Secure DRAM base address */
	teearg->secDRamSize  = CFG_TEE_CORE_SIZE;/* Secure DRAM size */
	teearg->sRamBase     = 0x00000000;/* SRAM base address */
	teearg->sRamSize     = 0x00000000;/* SRAM size */
	teearg->secSRamBase  = 0x00000000;/* Secure SRAM base address */
	teearg->secSRamSize  = 0x00000000;/* Secure SRAM size */
	teearg->log_port     = CFG_UART_LOG;
/* UART logging : UART base address. Can be same as preloader's one or not */
	if (g_boot_mode == META_BOOT && g_meta_com_type == META_UART_COM)
		teearg->log_port     = UART2;
/* UART logging : UART base address. Can be same as preloader's one or not */
		teearg->log_baudrate = CFG_LOG_BAUDRATE;
/* UART logging : UART baud rate */
#if CFG_GOOGLE_TRUSTY_SUPPORT
	teearg->gicd_base    = GICD_BASE;
/* ARM GIC Distributor Interface Base Address */
	teearg->gicc_base    = GICC_BASE;
/* ARM GIC CPU Interface Base Address */

#if (CFG_BOOT_DEV == BOOTDEV_SDMMC)
    u8 rpmb_key[32];
    seclib_get_msg_auth_key(teearg->hwuid, 16, rpmb_key, 32);
    mmc_rpmb_set_key(rpmb_key);
    DBG_MSG("%s TEE RPMB Size : 0x%x\n", MOD, mmc_rpmb_get_size());
#endif
#endif

    DBG_MSG("%s CFG_TEE_CORE_SIZE : 0x%x\n", MOD, CFG_TEE_CORE_SIZE);
    DBG_MSG("%s tee_secmem_size : 0x%x\n", MOD, tee_secmem_size);

#if CFG_TRUSTONIC_TEE_SUPPORT
    tbase_secmem_param_prepare(TEE_PARAMETER_ADDR, tee_entry_addr, CFG_TEE_CORE_SIZE,
        tee_secmem_size);
#endif

#if CFG_MICROTRUST_TEE_SUPPORT || CFG_MICROTRUST_TEE_LITE_SUPPORT
	DBG_MSG("%s come to teei *** \n", MOD);
	DBG_MSG("%s tee_arg_t size = %d\n", MOD, sizeof(tee_arg_t));
	teei_boot_param_prepare(TEE_BOOT_ARG_ADDR,
				tee_entry_addr,
				tee_secmem_size,
				CFG_DRAM_ADDR,
				platform_memory_size(),
				platform_chip_hw_code());

	u8 rpmb_key[32];

	seclib_get_msg_auth_key(teearg->hwuid, 16, rpmb_key, 32);
    //teei_key_param_prepare(TEE_PARAMETER_ADDR,(u8 *)teearg->hwuid,rpmb_key);
	teei_key_param_prepare(TEE_PARAMETER_ADDR, (u8 *)teearg->hwuid,
	sizeof(teearg->hwuid), (u8 *)HRID, sizeof(HRID), rpmb_key);

	teei_rtctime_param_prepare(TEE_BOOT_ARG_ADDR);
#endif

    /* Jump to TEE */
    print("%s Mode(%d) Start world switch from secure to non-secure world.\n",MOD, g_boot_mode);
    print("%s SW jump addr is 0x%x.\n",MOD, tee_entry_addr);
    jump_func = (jump_func_type)tee_entry_addr;

#if CFG_GOOGLE_TRUSTY_SUPPORT
    print("%s NWd jump addr is 0x%x, args addr is 0x%x\n",MOD, teearg->NWEntry, teearg->NWBootArgs);
    print("%s arg0(0x%x), arg1(0x%x), arg2(0x%x)\n",MOD, tee_secmem_size, (u32)teearg, sizeof(tee_v7_arg_t));
    print("%s log_port(0x%x), log_baudrate(0x%x)\n",MOD, CFG_UART_LOG, CFG_LOG_BAUDRATE);
    (*jump_func)(tee_secmem_size, (u32)teearg, sizeof(tee_v7_arg_t), 0);
#elif CFG_MICROTRUST_TEE_SUPPORT || CFG_MICROTRUST_TEE_LITE_SUPPORT
	(*jump_func)(addr, (u32)teearg, arg1, arg2);
#else
    (*jump_func)(0, (u32)teearg, 0, 0);
#endif
    // Never return.
}
#endif

void trustzone_post_init(void)
{
    atf_arg_t_ptr teearg = (atf_arg_t_ptr)trustzone_get_atf_boot_param_addr();

    teearg->atf_magic = ATF_BOOTCFG_MAGIC;
    teearg->tee_entry = tee_entry_addr;
    teearg->tee_boot_arg_addr = TEE_BOOT_ARG_ADDR;
    teearg->HRID[0] = seclib_get_devinfo_with_index(E_AREA12);
    teearg->HRID[1] = seclib_get_devinfo_with_index(E_AREA13);
    teearg->atf_log_port = CFG_UART_LOG;
    teearg->atf_log_baudrate = CFG_LOG_BAUDRATE;
    teearg->atf_irq_num = (32 + 249); /* K2 reserve SPI ID 249 for ATF log, which is ID 281 */
    teearg->devinfo[0] = seclib_get_devinfo_with_index(E_AREA3);
    teearg->devinfo[1] = seclib_get_devinfo_with_index(E_AREA4);

    //DBG_MSG("%s hwuid[0] : 0x%x\n", MOD, teearg->hwuid[0]);
    //DBG_MSG("%s hwuid[1] : 0x%x\n", MOD, teearg->hwuid[1]);
    //DBG_MSG("%s hwuid[2] : 0x%x\n", MOD, teearg->hwuid[2]);
    //DBG_MSG("%s hwuid[3] : 0x%x\n", MOD, teearg->hwuid[3]);
    //DBG_MSG("%s HRID[0] : 0x%x\n", MOD, teearg->HRID[0]);
    //DBG_MSG("%s HRID[1] : 0x%x\n", MOD, teearg->HRID[1]);
    DBG_MSG("%s atf_log_port : 0x%x\n", MOD, teearg->atf_log_port);
    DBG_MSG("%s atf_log_baudrate : 0x%x\n", MOD, teearg->atf_log_baudrate);
    DBG_MSG("%s atf_irq_num : %d\n", MOD, teearg->atf_irq_num);


#if CFG_TRUSTONIC_TEE_SUPPORT
    tbase_secmem_param_prepare(TEE_PARAMETER_ADDR, tee_entry_addr, CFG_TEE_CORE_SIZE,
        tee_secmem_size);
    tbase_boot_param_prepare(TEE_BOOT_ARG_ADDR, tee_entry_addr, CFG_TEE_CORE_SIZE,
        CFG_DRAM_ADDR, platform_memory_size());
    teearg->tee_support = 1;
#else //CFG_ATF_SUPPORT
    teearg->tee_support = 0;
#endif

#if CFG_ATF_LOG_SUPPORT
    teearg->atf_log_buf_start = atf_log_buf_start;
    teearg->atf_log_buf_size = ATF_LOG_BUFFER_SIZE;
#else
    teearg->atf_log_buf_start = 0;
    teearg->atf_log_buf_size = 0;
#endif
    DBG_MSG("%s ATF log buffer start : 0x%x\n", MOD, teearg->atf_log_buf_start);
    DBG_MSG("%s ATF log buffer size : 0x%x\n", MOD, teearg->atf_log_buf_size);

#if CFG_TEE_SUPPORT
    u8 rpmb_key[32];
    seclib_get_msg_auth_key(teearg->hwuid, 16, rpmb_key, 32);
    mmc_rpmb_set_key(rpmb_key);
    teearg->tee_rpmb_size = mmc_rpmb_get_size();
    DBG_MSG("%s TEE RPMB Size : 0x%x\n", MOD, teearg->tee_rpmb_size);
#endif
}

void trustzone_jump(u32 addr, u32 arg1, u32 arg2)
{
    tee_sec_config();

#if CFG_TEE_SUPPORT
    DBG_MSG("%s Jump to ATF, then 0x%x and 0x%x\n", MOD, tee_entry_addr, addr);
#else
    DBG_MSG("%s Jump to ATF, then 0x%x\n", MOD, addr);
#endif
    jumparch64(addr, arg1, arg2, trustzone_get_atf_boot_param_addr());
}

