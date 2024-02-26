/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2018
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

#include <tz_mem.h>
#include <tz_tags.h>

#include "typedefs.h"
#include "pal_log.h"
#include "rpmb_key.h"
#include "mmc_rpmb.h"
#include "ufs_aio_interface.h"
#include "key_derive.h"

#define MOD "TZ_TAGS"

#if CFG_GZ_REMAP_OFFSET_ENABLE
#if (CFG_GZ_SUPPORT && CFG_GZ_REMAP)
#include <gz_remap.h>
#endif
#endif

#if defined(TARGET_BUILD_VARIANT_ENG)
#define TAG_AUTO_VERIFY_ENABLE (1)
#else
#define TAG_AUTO_VERIFY_ENABLE (0)
#endif

#define IS_ZERO(val) (val == 0)
#define ASSERT_NONNULL(val) \
    do { \
        if (val == NULL) { \
            pal_log_err("[%s] %s:%d param is null!\n", MOD, \
                __func__, __LINE__); \
            ASSERT(0); \
        } \
    } while(0)

#define ASSERT_LT(val1, val2) \
    do { \
        if (val1 > val2) { \
            pal_log_err("[%s] %s:%d 0x%x exceeds 0x%x\n", MOD, \
                __func__, __LINE__, (u32)val1, (u32)val2); \
            ASSERT(0); \
        } \
    } while(0)

static u8* tz_update_sec_mem_cfg(struct tz_tag *tag,
    u32 tz_sec_mem_start)
{
    tag = (struct tz_tag *) NEXT_TAG(tag);
    tag->hdr.size = sizeof(struct tz_sec_mem_cfg);
    tag->hdr.tag = TZ_TAG_SEC_MEM_CFG;
#if CFG_TEE_SECURE_MEM_PROTECTED
    tag->u.sec_mem_cfg.secmem_obfuscation = 1;
#else
    tag->u.sec_mem_cfg.secmem_obfuscation = 0;
#endif
#if CFG_TEE_SECURE_MEM_SHARED
    tag->u.sec_mem_cfg.shared_secmem = 1;
#else
    tag->u.sec_mem_cfg.shared_secmem = 0;
#endif
    tag->u.sec_mem_cfg.svp_mem_start = tz_sec_mem_start;

    return (u8*)tag;
}

static void tz_clean_flash_info(struct tz_sec_flash_dev_cfg *cfg)
{
    cfg->rpmb_size = 0;
}

static void tz_update_sdmmc_info(struct tz_sec_flash_dev_cfg *cfg)
{
    cfg->rpmb_size = mmc_rpmb_get_size();
#if CFG_RPMB_KEY_PROGRAMED_IN_KERNEL
    cfg->emmc_setkey_flag = rpmb_set_flag;
#endif
    cfg->emmc_rel_wr_sec_c = mmc_rpmb_get_rel_wr_sec_c();
}

static void tz_update_ufs_info(struct tz_sec_flash_dev_cfg *cfg)
{
    cfg->rpmb_size = (int)ufs_rpmb_get_lu_size();

#if CFG_RPMB_KEY_PROGRAMED_IN_KERNEL
    cfg->emmc_setkey_flag = rpmb_set_flag;
#endif
    /*
     * We still use legacy eMMC name "rel_wr_sec" here.
     *
     * Since eMMC's REL_WR_SEC shall be 1 (512 bytes) which means
     * 2 RPMB frams and TEE will multiply this value by 2, we divide
     * it by 2 here for UFS.
     */
    cfg->emmc_rel_wr_sec_c = ufs_rpmb_get_rw_size() / 2;
}

#ifndef RPMB_KEY_SIZE
#define RPMB_KEY_SIZE (32)
#endif
static u8* tz_update_flash_dev_info(struct tz_tag *tag)
{
    int ret = 0;

    tag = (struct tz_tag *) NEXT_TAG(tag);
    tag->hdr.size = sizeof(struct tz_sec_flash_dev_cfg);
    tag->hdr.tag = TZ_TAG_FLASH_DEV_INFO;
#ifdef MTK_SECURITY_SW_SUPPORT
    ret = rpmb_get_key((char *)&tag->u.flash_dev_cfg.rpmb_msg_auth_key,
        RPMB_KEY_SIZE);
    if (ret)
        pal_log_err("[%s] rpmb_get_key failed, ret = %d\n", MOD, ret);
#endif

#if defined(BOOTDEV_SDMMC_UFS_COMBO) && \
    ((CFG_BOOT_DEV == BOOTDEV_SDMMC) || (CFG_BOOT_DEV == BOOTDEV_UFS))
    blkdev_t *bootdev = blkdev_get(CFG_BOOT_DEV);

    if (bootdev == NULL) {
        tz_clean_flash_info(&tag->u.flash_dev_cfg);
    } else if (bootdev->type == BOOTDEV_SDMMC) {
        tz_update_sdmmc_info(&tag->u.flash_dev_cfg);
    } else if (bootdev->type == BOOTDEV_UFS) {
        tz_update_ufs_info(&tag->u.flash_dev_cfg);
    }
#elif (CFG_BOOT_DEV == BOOTDEV_UFS)
    tz_update_ufs_info(&tag->u.flash_dev_cfg);
#elif (CFG_BOOT_DEV == BOOTDEV_SDMMC)
    tz_update_sdmmc_info(&tag->u.flash_dev_cfg);
#endif

    return (u8*)tag;
}

#define UPDATE_RESERVE_MEM_TAG(mem_size, tag_magic) \
    pre_reserve_size = mem_size; \
    pre_reserve_start -= mem_size; \
    tag = (struct tz_tag *) NEXT_TAG(tag); \
    tag->hdr.size = sizeof(struct tz_sec_reserved_mem); \
    tag->hdr.tag = tag_magic; \
    tag->u.res_mem_cfg.size = pre_reserve_size; \
    tag->u.res_mem_cfg.start = pre_reserve_start;

static u8* tz_update_reserved_mem(struct tz_tag *tag,
    u32 tz_sec_mem_end)
{
    struct tz_tag *tz_tag_sec_mem_cfg = tag;
    u32 pre_reserve_start = tz_sec_mem_end;
    u32 pre_reserve_size = 0;

#if CFG_TEE_SECURE_MEM_SHARED
    UPDATE_RESERVE_MEM_TAG(SEC_MEM_RESERVED_M4U_SIZE,
        TZ_TAG_RESERVED_MEM_M4U_BUF);
    UPDATE_RESERVE_MEM_TAG(SEC_MEM_RESERVED_CMDQ_SIZE,
        TZ_TAG_RESERVED_MEM_CMDQ_BUF);
    UPDATE_RESERVE_MEM_TAG(SEC_MEM_RESERVED_SPI_SIZE,
        TZ_TAG_RESERVED_MEM_SPI_BUF);
#if CFG_MEM_RESERVED_I2C_ENABLE
    UPDATE_RESERVE_MEM_TAG(SEC_MEM_RESERVED_I2C_SIZE,
        TZ_TAG_RESERVED_MEM_I2C_BUF);
#endif
#endif

    /* Sanity check for boundary */
    tz_tag_sec_mem_cfg->u.sec_mem_cfg.svp_mem_end = pre_reserve_start;
    ASSERT_LT(tz_tag_sec_mem_cfg->u.sec_mem_cfg.svp_mem_start,
        tz_tag_sec_mem_cfg->u.sec_mem_cfg.svp_mem_end);

    return (u8*)tag;
}

static u8* tz_update_sec_hardware_cfg(struct tz_tag *tag)
{
    tag = (struct tz_tag *) NEXT_TAG(tag);
    tag->hdr.size = sizeof(struct tz_sec_device_cfg);
    tag->hdr.tag = TZ_TAG_SEC_DEVICE_CFG;
#if CFG_GZ_REMAP_OFFSET_ENABLE
#if (CFG_GZ_SUPPORT && CFG_GZ_REMAP)
    tag->u.sec_hw_cfg.remap_offset = gz_remap_ddr_get_offset(GZ_REMAP_VMID_GZ);
#else
    tag->u.sec_hw_cfg.remap_offset = 0x0LL;
#endif
#endif

    return (u8*)tag;
}

#if (CFG_GZ_SUPPORT && (CFG_GZ_SECURE_DSP || CFG_GZ_SAPU_MTEE_SHM || CFG_GZ_TEE_STATIC_SHM))
static u8* tz_update_gz_cfg(struct tz_tag *tag)
{
    tag = (struct tz_tag *) NEXT_TAG(tag);
    tag->hdr.size = sizeof(struct tz_sec_gz_cfg);
    tag->hdr.tag = TZ_TAG_SEC_GZ_CFG;
#if CFG_GZ_SECURE_DSP
    gz_get_sdsp_mem_info(&tag->u.gz_cfg.sdsp_fw_pa,
        &tag->u.gz_cfg.sdsp_fw_size);
#endif
#if CFG_GZ_SAPU_MTEE_SHM
    gz_get_sapu_shm_info(&tag->u.gz_cfg.sdsp_shared_mem_pa,
        &tag->u.gz_cfg.sdsp_shared_mem_size);
#endif
#if CFG_GZ_TEE_STATIC_SHM
    get_gz_tee_static_shm_info(&tag->u.gz_cfg.gz_tee_shared_mem_pa,
        &tag->u.gz_cfg.gz_tee_shared_mem_size);
#endif

    return (u8*)tag;
}
#endif

#define DUMP_HEX(name) \
    pal_log_info("[%s] %s: 0x%x\n", MOD, #name, tag->u.name)
#define DUMP_HEX_WITH_TAG(name) \
    pal_log_info("[%s] %s(0x%x): 0x%x\n", MOD, #name, tag->hdr.tag, tag->u.name)
#define DUMP_HEX_64(name) \
    pal_log_info("[%s] %s: 0x%llx\n", MOD, #name, tag->u.name)
#define DUMP_INT(name) \
    pal_log_info("[%s] %s: %d\n", MOD, #name, tag->u.name)

static void tz_secmem_tags_dump(unsigned long param_addr)
{
    struct tz_tag *tag = (struct tz_tag *) param_addr;

    ASSERT_NONNULL(tag);
    if (!IS_TAG_FMT(tag)) {
        pal_log_err("[%s] secmem params are not tag format!\n", MOD);
        ASSERT(0);
    }

    do {
        switch (tag->hdr.tag) {
        case SEC_MEM_TAG_FMT_MAGIC:
            pal_log_info("[%s] secmem is tag format\n", MOD);
            break;
        case TZ_TAG_SEC_MEM_CFG:
            DUMP_HEX_64(sec_mem_cfg.svp_mem_start);
            DUMP_HEX_64(sec_mem_cfg.svp_mem_end);
            DUMP_INT(sec_mem_cfg.secmem_obfuscation);
            DUMP_INT(sec_mem_cfg.shared_secmem);
            break;
        case TZ_TAG_RESERVED_MEM_TPLAY_TABLE:
        case TZ_TAG_RESERVED_MEM_TPLAY_BUF:
        case TZ_TAG_RESERVED_MEM_M4U_BUF:
        case TZ_TAG_RESERVED_MEM_CMDQ_BUF:
        case TZ_TAG_RESERVED_MEM_SPI_BUF:
        case TZ_TAG_RESERVED_MEM_I2C_BUF:
            DUMP_HEX_WITH_TAG(res_mem_cfg.start);
            DUMP_HEX_WITH_TAG(res_mem_cfg.size);
            break;
        case TZ_TAG_FLASH_DEV_INFO:
            DUMP_HEX(flash_dev_cfg.rpmb_size);
            DUMP_HEX(flash_dev_cfg.emmc_rel_wr_sec_c);
            DUMP_INT(flash_dev_cfg.emmc_setkey_flag);
            break;
        case TZ_TAG_SEC_DEVICE_CFG:
            DUMP_HEX_64(sec_hw_cfg.remap_offset);
            break;
#if (CFG_GZ_SUPPORT && (CFG_GZ_SECURE_DSP || CFG_GZ_SAPU_MTEE_SHM || CFG_GZ_TEE_STATIC_SHM))
        case TZ_TAG_SEC_GZ_CFG:
            DUMP_HEX(gz_cfg.sdsp_fw_pa);
            DUMP_HEX(gz_cfg.sdsp_fw_size);
            DUMP_HEX(gz_cfg.sdsp_shared_mem_pa);
            DUMP_HEX(gz_cfg.sdsp_shared_mem_size);
            DUMP_HEX(gz_cfg.gz_tee_shared_mem_pa);
            DUMP_HEX(gz_cfg.gz_tee_shared_mem_size);
            break;
#endif
        default:
            pal_log_warn("[%s] unknown tags: 0x%x\n", MOD, tag->hdr.tag);
            break;
        }
        tag = (struct tz_tag*)NEXT_TAG(tag);

        if (OUT_OF_TAG_SEARCH_RANGE(tag, param_addr))
            break;
    } while (!IS_TAG_END(tag));

    pal_log_debug("[%s] remained tag len: 0x%x\n", MOD,
        PTR_DIFF_BYTES(param_addr + TAG_STRUCT_MAX_SIZE, NEXT_TAG(tag)));
}

#if TAG_AUTO_VERIFY_ENABLE
static void tz_secmem_tags_auto_verify(unsigned long param_addr)
{
    ASSERT_NONNULL(find_tag(param_addr, SEC_MEM_TAG_FMT_MAGIC));
    ASSERT_NONNULL(find_tag(param_addr, TZ_TAG_SEC_MEM_CFG));
#if CFG_TEE_SECURE_MEM_SHARED
    ASSERT_NONNULL(find_tag(param_addr, TZ_TAG_RESERVED_MEM_M4U_BUF));
    ASSERT_NONNULL(find_tag(param_addr, TZ_TAG_RESERVED_MEM_CMDQ_BUF));
    ASSERT_NONNULL(find_tag(param_addr, TZ_TAG_RESERVED_MEM_SPI_BUF));
#if CFG_MEM_RESERVED_I2C_ENABLE
    ASSERT_NONNULL(find_tag(param_addr, TZ_TAG_RESERVED_MEM_I2C_BUF));
#endif
#endif
    ASSERT_NONNULL(find_tag(param_addr, TZ_TAG_FLASH_DEV_INFO));
    ASSERT_NONNULL(find_tag(param_addr, TZ_TAG_SEC_DEVICE_CFG));
#if (CFG_GZ_SUPPORT && (CFG_GZ_SECURE_DSP || CFG_GZ_SAPU_MTEE_SHM || CFG_GZ_TEE_STATIC_SHM))
    ASSERT_NONNULL(find_tag(param_addr, TZ_TAG_SEC_GZ_CFG));
#endif
    ASSERT_NONNULL(find_tag(param_addr, SEC_MEM_TAG_ENG_MAGIC));
}
#endif

#define INIT_SEC_MEM_TAG_STRUCT(start_addr) \
    memset((u8*)start_addr, 0x0, TAG_STRUCT_MAX_SIZE); \
    tag = (struct tz_tag *) start_addr; \
    tag->hdr.tag = SEC_MEM_TAG_FMT_MAGIC; \
    tag->hdr.size = 0

#define UPDATE_SEC_MEM_END_MAGIC(tag) \
    tag = (struct tz_tag *) NEXT_TAG(tag); \
    tag->hdr.tag = SEC_MEM_TAG_ENG_MAGIC; \
    tag->hdr.size = 0

void tz_secmem_tags_prepare(unsigned long param_addr,
    unsigned int tz_sec_mem_start, unsigned int tz_sec_mem_end)
{
    u8 *tag_start_ptr = (u8*) param_addr;
    u8 *tag_end_ptr;
    struct tz_tag *tag = NULL;

    pal_log_info("[%s] 0x%x-0x%x\n", MOD, tz_sec_mem_start, tz_sec_mem_end);

    if (IS_ZERO(tz_sec_mem_start)) {
        pal_log_warn("[%s] secmem tags are not prepared!\n", MOD);
        return;
    }

    if (IS_ZERO(tz_sec_mem_end - tz_sec_mem_start)) {
        pal_log_warn("[%s] no space for secmem tags!\n", MOD);
        return;
    }

    ASSERT_NONNULL(tag_start_ptr);

    INIT_SEC_MEM_TAG_STRUCT(tag_start_ptr);
    tag = (struct tz_tag*)tz_update_sec_mem_cfg(tag, tz_sec_mem_start);
    tag = (struct tz_tag*)tz_update_reserved_mem(tag, tz_sec_mem_end);
    tag = (struct tz_tag*)tz_update_flash_dev_info(tag);
    tag = (struct tz_tag*)tz_update_sec_hardware_cfg(tag);
#if (CFG_GZ_SUPPORT && (CFG_GZ_SECURE_DSP || CFG_GZ_SAPU_MTEE_SHM || CFG_GZ_TEE_STATIC_SHM))
    tag = (struct tz_tag*)tz_update_gz_cfg(tag);
#endif
    UPDATE_SEC_MEM_END_MAGIC(tag);

    /* Sanity check for boundary */
    tag_end_ptr = (u8*) NEXT_TAG(tag);
    ASSERT_LT(tag_end_ptr, (tag_start_ptr + TAG_STRUCT_MAX_SIZE));

    /* Verify/dump tag contents */
#if TAG_AUTO_VERIFY_ENABLE
    tz_secmem_tags_auto_verify(param_addr);
#endif
    tz_secmem_tags_dump(param_addr);
}
