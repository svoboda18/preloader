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
#endif
#if CFG_TRUSTKERNEL_TEE_SUPPORT
#include "tz_tkcore.h"
#endif
#include "sec_devinfo.h"
#include "tz_emi_mpu.h"
#include "key_derive.h"
#include "log_store_pl.h"

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
#if CFG_TRUSTONIC_TEE_SUPPORT
#define TEE_MEM_ALIGNMENT (0x40000)   //4KB Alignment
#define ATF_MEM_ALIGNMENT (0x200000)  //2MB Alignment
#define ROUNDUP(a,b)      (((a) + ((b)-1)) & ~((b)-1))
#elif CFG_TRUSTKERNEL_TEE_SUPPORT
#define TEE_MEM_ALIGNMENT (0x100000)  //1MB Alignment
#else
#define TEE_MEM_ALIGNMENT (0x200000)  //2MB Alignment
#endif

#if CFG_BOOT_ARGUMENT
#define bootarg g_dram_buf->bootarg
#endif

#define RPMB_KEY_SIZE     (32)
#define FDE_KEY_SIZE      (16)

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
static u8 g_hwuid[16];
//for init.s
u32 bl31_base_addr = BL31_BASE;
u32 rst_vector_base_addr = RVBADDRESS_CPU0;
static u8 g_hwuid_initialized = 0;

/**************************************************************************
 *  INTERNAL FUNCTIONS
 **************************************************************************/
static u32 trustzone_get_atf_boot_param_addr(void)
{
    return ATF_BOOT_ARG_ADDR;
}
static void atf_dram_reserve_mem(void){
    u64 atf_dram_res;
    u64 limit_addr = BL31_DRAM_BASE+BL31_DRAM_SIZE;
    u64 align_addr = (u64)0x10000;

    atf_dram_res = (u32)mblock_reserve_ext(&bootarg.mblock_info,
            (u64) BL31_DRAM_SIZE, (u64)align_addr, limit_addr, 0, "atf-reserved");
	if (atf_dram_res != BL31_DRAM_BASE) {
		print("atf_dram_res:0x%x, should:0x%x, assert!\n", atf_dram_res, BL31_DRAM_BASE);
		ASSERT(0);
	}
}
static u32 tee_secmem_size = 0;
static u32 tee_secmem_start = 0;
static u64 atf_log_buf_start = 0;
static u32 tee_extra_mem_size = 0;

void tee_get_secmem_start(u64 *addr)
{
    *addr = atf_log_buf_start;
}

void tee_get_secmem_size(u32 *size)
{
    *size = ATF_LOG_BUFFER_SIZE;
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

int tee_verify_image(u32 *addr)
{
    int ret;

    /* verify tee image and addr is updated to pointer entry point */
    ret = trustonic_teehdr_sanity(addr);
    if (ret)
        return ret;

    return ret;
}

static inline void save_atf_log_buf_to_log_store(void)
{
    /* for preloader log store */
    ((struct sram_log_header *)SRAM_LOG_ADDR)->dram_buf.atf_log_addr = (u32) atf_log_buf_start;
    ((struct sram_log_header *)SRAM_LOG_ADDR)->dram_buf.atf_log_len = ATF_LOG_BUFFER_SIZE;
}

u32 tee_get_load_addr(u32 maddr)
{
    u32 ret_addr = 0;
    u64 limit_addr = 0xC0000000;
    u64 align_addr = (u64)TEE_MEM_ALIGNMENT;

#if CFG_TEE_SUPPORT
    if (tee_secmem_start != 0)
        goto allocated;

    tee_extra_mem_size = maddr % TEE_MEM_ALIGNMENT;
    tee_secmem_size = maddr - tee_extra_mem_size;

    DBG_MSG("%s maddr: 0x%x, tee_extra_mem_size: 0x%x, tee_secmem_size: 0x%x\n", MOD, maddr, tee_extra_mem_size, tee_secmem_size);

#if CFG_GOOGLE_TRUSTY_SUPPORT
    //tee_secmem_start = TRUSTY_MEM_LOAD_ADDR;
    align_addr = 0x10000000;
    limit_addr = TRUSTY_MEM_LOAD_ADDR + 0x10000000;
    DBG_MSG("%s align_addr: 0x%llx, limit_addr: 0x%llx\n", MOD, align_addr, limit_addr);
    tee_secmem_start = (u32)mblock_reserve_ext(&bootarg.mblock_info,
            (u64)(tee_secmem_size), (u64)align_addr, limit_addr, 0, "trusty-reserved");
    DBG_MSG("%s reserved_addr: 0x%x, reserved_size: 0x%x\n", MOD, tee_secmem_start, tee_secmem_size);

    if (tee_secmem_start != (limit_addr - align_addr)) {
        print("%s Fail to allocate secure memory at fixed address: 0x%x\n", MOD,
            (u32)(limit_addr - align_addr));
        BUG_ON(1);
    }
#elif CFG_MICROTRUST_TEE_SUPPORT
    /* To fix at the location : (limit_addr - align_addr) */
    /* first allocate for tee region */
    align_addr = 0x10000000;
    limit_addr = 0x80000000;
    DBG_MSG("%s align_addr: 0x%llx, limit_addr: 0x%llx\n", MOD, align_addr, limit_addr);
    tee_secmem_start = (u32)mblock_reserve_ext(&bootarg.mblock_info,
            (u64)(tee_secmem_size), (u64)align_addr, limit_addr, 0, "teei-reserved");
    DBG_MSG("%s reserved_addr: 0x%x, reserved_size: 0x%x\n", MOD, tee_secmem_start, tee_secmem_size);

    if (tee_secmem_start != (limit_addr - align_addr)) {
        print("%s Fail to allocate secure memory at fixed address: 0x%x\n", MOD,
            (u32)(limit_addr - align_addr));
        BUG_ON(1);
    }

    DBG_MSG("%s reserved_addr: 0x%x, reserved_size: 0x%x\n", MOD, tee_secmem_start, tee_secmem_size);
#elif CFG_TRUSTKERNEL_TEE_SUPPORT
    limit_addr = 0xF0000000;
    DBG_MSG("%s align_addr: 0x%llx, limit_addr: 0x%llx\n", MOD, align_addr, limit_addr);
    tee_secmem_start = (u32)mblock_reserve_ext(&bootarg.mblock_info,
        (u64)(tee_secmem_size), align_addr, limit_addr, 0, "tee-reserved");
    DBG_MSG("%s TEE reserved_addr: 0x%x, reserved_size: 0x%x\n", MOD, tee_secmem_start, tee_secmem_size);
#else /* CFG_TRUSTONIC_TEE_SUPPORT */
    limit_addr = 0x78000000;
    DBG_MSG("%s align_addr: 0x%llx, limit_addr: 0x%llx\n", MOD, align_addr, limit_addr);
    tee_secmem_start = (u32)mblock_reserve_ext(&bootarg.mblock_info,
        (u64)(tee_secmem_size), (u64)ATF_MEM_ALIGNMENT,
        limit_addr, 0, "tee-reserved");
    DBG_MSG("%s TEE reserved_addr: 0x%x, reserved_size: 0x%x\n", MOD, tee_secmem_start, tee_secmem_size);
#endif // !CFG_GOOGLE_TRUSTY_SUPPORT

    if(!tee_secmem_start){
        printf("%s Fail to allocate secure memory: 0x%x, 0x%x\n", MOD,
            (tee_secmem_size), TEE_MEM_ALIGNMENT);
        return 0;
    }

    save_atf_log_buf_to_log_store();

    DBG_MSG("%s atf_log_buf_start: 0x%lx, tee_secmem_start: 0x%x\n", MOD, atf_log_buf_start, tee_secmem_start);

allocated:
    ret_addr = tee_secmem_start - tee_extra_mem_size;
#endif /* end of CFG_TEE_SUPPORT */

    return ret_addr;
}

static void tee_sec_config(void)
{
    tz_sram_sec_init(CFG_NON_SECURE_SRAM_ADDR);

#if CFG_TEE_SUPPORT
#if CFG_TRUSTKERNEL_TEE_SUPPORT
    tz_sec_mem_init(tee_entry_addr, tee_entry_addr + tee_secmem_size - TKCORE_SHM_SIZE_LIMIT - 1,
        SECURE_OS_MPU_REGION_ID);
	DBG_MSG("%s set secure memory protection : 0x%x, 0x%x (%d)\n", MOD, tee_entry_addr,
        tee_entry_addr + tee_secmem_size - TKCORE_SHM_SIZE_LIMIT - 1, SECURE_OS_MPU_REGION_ID);
#else
    tz_sec_mem_init(tee_entry_addr, tee_entry_addr + tee_secmem_size - 1, SECURE_OS_MPU_REGION_ID);
    DBG_MSG("%s set secure memory protection : 0x%x, 0x%x (%d)\n", MOD, tee_entry_addr,
        tee_entry_addr + tee_secmem_size - 1, SECURE_OS_MPU_REGION_ID);
#endif
#endif
#if 1

    {
    u32 atf_dram_addr = BL31_DRAM_BASE;
    atf_dram_addr = atf_dram_addr & ~(EMI_MPU_ALIGNMENT - 1);

    DBG_MSG("%s ATF entry addr, dram addr: 0x%x, 0x%x\n", MOD, BL31_BASE, atf_dram_addr);

    tz_sec_mem_init(atf_dram_addr, atf_dram_addr + BL31_DRAM_SIZE - 1, ATF_MPU_REGION_ID);
    DBG_MSG("%s set secure memory protection : 0x%x, 0x%x (%d)\n", MOD, atf_dram_addr,
        atf_dram_addr + BL31_DRAM_SIZE - 1, ATF_MPU_REGION_ID);
    }
#endif
}

void trustzone_pre_init(void)
{
    sec_malloc_buf_reset();
    atf_dram_reserve_mem();
    tz_dapc_sec_init();

#if CFG_ATF_LOG_SUPPORT
    {
        u64 limit_addr = 0xC0000000;
        atf_arg_t_ptr teearg = (atf_arg_t_ptr)trustzone_get_atf_boot_param_addr();

        atf_log_buf_start = mblock_reserve_ext(&bootarg.mblock_info,
            (u64)ATF_LOG_BUFFER_SIZE, (u64)TEE_MEM_ALIGNMENT,
            limit_addr, 0, "atf-log-reserved");

        if(!atf_log_buf_start){
            printf("%s Fail to allocate atf log buffer: 0x%x, 0x%x\n", MOD,
                ATF_LOG_BUFFER_SIZE, TEE_MEM_ALIGNMENT);
            teearg->atf_log_buf_size = 0;
        }
        save_atf_log_buf_to_log_store();
    }
#endif
}

void trustzone_post_init(void)
{
    atf_arg_t_ptr teearg = (atf_arg_t_ptr)trustzone_get_atf_boot_param_addr();
    u32 i;
    u8 rpmb_key[RPMB_KEY_SIZE] = {0};
    u8 fde_key[FDE_KEY_SIZE] = {0};

    teearg->atf_magic = ATF_BOOTCFG_MAGIC;
    teearg->tee_entry = tee_entry_addr;
    teearg->tee_boot_arg_addr = TEE_BOOT_ARG_ADDR;

    teearg->atf_hrid_size = HRID_SIZE;
    for (i = 0; i < HRID_SIZE; i++) {
        teearg->HRID[i] = seclib_get_devinfo_with_index(12 + i);
    }
    for (i = HRID_SIZE; i < (sizeof(teearg->HRID) / sizeof(u32)); i++) {
        teearg->HRID[i] = 0;
    }

    teearg->atf_log_port = CFG_UART_LOG;
    teearg->atf_log_baudrate = CFG_LOG_BAUDRATE;
    teearg->atf_irq_num = (32 + 293);
    /* reserve SPI ID 293 for ATF log, which is ID 325 */
    teearg->devinfo[0] = seclib_get_devinfo_with_index(E_AREA3);
    teearg->devinfo[1] = seclib_get_devinfo_with_index(E_AREA4);
    teearg->devinfo[2] = seclib_get_devinfo_with_index(E_AREA28);

    //DBG_MSG("%s hwuid[0] : 0x%x\n", MOD, teearg->hwuid[0]);
    //DBG_MSG("%s hwuid[1] : 0x%x\n", MOD, teearg->hwuid[1]);
    //DBG_MSG("%s hwuid[2] : 0x%x\n", MOD, teearg->hwuid[2]);
    //DBG_MSG("%s hwuid[3] : 0x%x\n", MOD, teearg->hwuid[3]);
    //DBG_MSG("%s HRID[0] : 0x%x\n", MOD, teearg->HRID[0]);
    //DBG_MSG("%s HRID[1] : 0x%x\n", MOD, teearg->HRID[1]);
    DBG_MSG("%s atf_log_port : 0x%x\n", MOD, teearg->atf_log_port);
    DBG_MSG("%s atf_log_baudrate : 0x%x\n", MOD, teearg->atf_log_baudrate);
    DBG_MSG("%s atf_irq_num : %d\n", MOD, teearg->atf_irq_num);

#ifdef MTK_SECURITY_SW_SUPPORT
    seclib_key_derive(KEY_TYPE_RPMB, rpmb_key, RPMB_KEY_SIZE);
    seclib_key_derive(KEY_TYPE_FDE, fde_key, FDE_KEY_SIZE);
#endif

	memcpy(teearg->msg_fde_key, fde_key, FDE_KEY_SIZE);
#if 0 /* FDE KEY DUMP */
    for (i = 0; i < FDE_KEY_SIZE; i+=8) {
        DBG_MSG("%s FDEKEY[%d:%d] : 0x%x 0x%x 0x%x 0x%x  0x%x 0x%x 0x%x 0x%x\n", MOD, i, i+7,
                fde_key[i + 0], fde_key[i + 1], fde_key[i + 2], fde_key[i + 3],
                fde_key[i + 4], fde_key[i + 5], fde_key[i + 6], fde_key[i + 7]);
    }
#endif /* FDE KEY DUMP */

    /* Config RPMB key for UFS w/ POWP, used in LK thru SMC to ATF */
    sec_mem_arg_t sec_mem_arg = {0};
    sec_mem_arg.magic = SEC_MEM_MAGIC;
    sec_mem_arg.version = SEC_MEM_VERSION;
    memcpy(sec_mem_arg.msg_auth_key, rpmb_key, RPMB_KEY_SIZE);
    memcpy((void*)TEE_PARAMETER_ADDR, &sec_mem_arg, sizeof(sec_mem_arg_t));
#if 0 /* RPMB KEY DUMP */
    for (i = 0; i < RPMB_KEY_SIZE; i+=8) {
        DBG_MSG("%s RPMBKEY[%d:%d] : 0x%x 0x%x 0x%x 0x%x  0x%x 0x%x 0x%x 0x%x\n", MOD, i, i+7,
                rpmb_key[i + 0], rpmb_key[i + 1], rpmb_key[i + 2], rpmb_key[i + 3],
                rpmb_key[i + 4], rpmb_key[i + 5], rpmb_key[i + 6], rpmb_key[i + 7]);
    }
#endif /* RPMB KEY DUMP */

#if CFG_TRUSTONIC_TEE_SUPPORT
    tbase_secmem_param_prepare(TEE_PARAMETER_ADDR, tee_entry_addr, CFG_TEE_CORE_SIZE,
        tee_secmem_size);
    tbase_boot_param_prepare(TEE_BOOT_ARG_ADDR, tee_entry_addr, CFG_TEE_CORE_SIZE,
        CFG_DRAM_ADDR, platform_memory_size());
    teearg->tee_support = 1;
#elif CFG_MICROTRUST_TEE_SUPPORT
	DBG_MSG("%s come to    teei ***\n", MOD);
    teei_boot_param_prepare(TEE_BOOT_ARG_ADDR, tee_entry_addr, tee_secmem_size, CFG_DRAM_ADDR,
                            platform_memory_size(), platform_chip_hw_code());
    //prepare the parameter for secure driver here
    teearg->tee_support = 1;
#elif CFG_GOOGLE_TRUSTY_SUPPORT
    trusty_boot_param_prepare((u32)TEE_BOOT_ARG_ADDR, (u32)tee_entry_addr, (u64)tee_secmem_size,
        (u64)CFG_DRAM_ADDR, (u64)platform_memory_size(), rpmb_key, RPMB_KEY_SIZE);
    teearg->tee_support = 1;
#elif CFG_TRUSTKERNEL_TEE_SUPPORT
	DBG_MSG("%s come to    tkcore ***  \n", MOD);
    tkcore_boot_param_prepare(TEE_BOOT_ARG_ADDR, tee_entry_addr, tee_secmem_size,
        CFG_DRAM_ADDR, platform_memory_size(),
        g_boot_mode == META_BOOT && g_meta_com_type == META_UART_COM);
    //prepare the parameter for secure driver here
    teearg->tee_support = 1;
#else //CFG_ATF_SUPPORT
    teearg->tee_support = 0;
#endif

#if CFG_ATF_LOG_SUPPORT
    teearg->atf_log_buf_start = atf_log_buf_start;
    teearg->atf_log_buf_size = ATF_LOG_BUFFER_SIZE;
    teearg->atf_aee_debug_buf_start = (atf_log_buf_start + ATF_LOG_BUFFER_SIZE - ATF_AEE_BUFFER_SIZE);
    teearg->atf_aee_debug_buf_size = ATF_AEE_BUFFER_SIZE;
#else
    teearg->atf_log_buf_start = 0;
    teearg->atf_log_buf_size = 0;
    teearg->atf_aee_debug_buf_start = 0;
    teearg->atf_aee_debug_buf_size = 0;
#endif
    DBG_MSG("%s ATF log buffer start : 0x%lx\n", MOD, teearg->atf_log_buf_start);
    DBG_MSG("%s ATF log buffer size : 0x%x\n", MOD, teearg->atf_log_buf_size);
    DBG_MSG("%s ATF aee buffer start : 0x%lx\n", MOD, teearg->atf_aee_debug_buf_start);
    DBG_MSG("%s ATF aee buffer size : 0x%x\n", MOD, teearg->atf_aee_debug_buf_size);


#if CFG_TEE_SUPPORT

#if CFG_MICROTRUST_TEE_SUPPORT
    teei_key_param_prepare(TEE_PARAMETER_KEY, (u8 *)teearg->hwuid, sizeof(teearg->hwuid), (u8 *)teearg->HRID,
					sizeof(u32) * teearg->atf_hrid_size, rpmb_key);
#endif
#if CFG_TRUSTKERNEL_TEE_SUPPORT
    tkcore_boot_param_prepare_rpmbkey(TEE_BOOT_ARG_ADDR, rpmb_key, RPMB_KEY_SIZE);
    /* only useful for non-ATF platform */
    tkcore_boot_param_prepare_nwbootargs(TEE_BOOT_ARG_ADDR, 0U, 0U);

    tkcore_boot_param_prepare_rtctime(TEE_BOOT_ARG_ADDR);

    tkcore_boot_param_prepare_soc_id(TEE_BOOT_ARG_ADDR);

    tkcore_dump_param(TEE_BOOT_ARG_ADDR);
#endif

#if ((CFG_BOOT_DEV == BOOTDEV_SDMMC) && CFG_RPMB_SET_KEY) // For eMMC project with TEEs
    mmc_rpmb_set_key(rpmb_key);
    teearg->tee_rpmb_size = mmc_rpmb_get_size();
    DBG_MSG("%s TEE RPMB Size : 0x%x\n", MOD, teearg->tee_rpmb_size);
#endif
#endif /* CFG_TEE_SUPPORT */

#if (CFG_BOOT_DEV == BOOTDEV_UFS)
	ufs_rpmb_authen_key_program(rpmb_key);
#endif

#if CFG_MICROTRUST_TEE_SUPPORT
    teei_rtctime_param_prepare(TEE_BOOT_ARG_ADDR);
#endif
    tz_dapc_sec_postinit();
}

void trustzone_jump(u32 addr, u32 arg1, u32 arg2)
{
    u32 bl31_reserve = 0;
    tee_sec_config();

#if CFG_TEE_SUPPORT
    DBG_MSG("%s Jump to ATF, then 0x%x and 0x%x\n", MOD, tee_entry_addr, addr);
#else
    DBG_MSG("%s Jump to ATF, then 0x%x\n", MOD, addr);
#endif

    g_dram_buf->bl31_info.bootarg_loc = arg1;
    g_dram_buf->bl31_info.bootarg_size = arg2;
    g_dram_buf->bl31_info.bl33_start_addr = addr;
    g_dram_buf->bl31_info.tee_info_addr = trustzone_get_atf_boot_param_addr();
    jumparch64(&(g_dram_buf->bl31_info), bl31_reserve);
}

