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
 * MediaTek Inc. (C) 2018. All rights reserved.
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

#include <pal_log.h>
#include <gz_remap.h>
#include <gz_remap_mt.h>
#include <typedefs.h>

#define LOG_TAG "GZ_REMAP"

#define GZ_REMAP_DEBUG  (0)

#if GZ_REMAP_DEBUG
#define GZREMAP_ERR(fmt, ...) \
    pal_log_err("[%s][ERROR] %s:%d: " fmt, LOG_TAG, __func__, __LINE__, ##__VA_ARGS__)
#define GZREMAP_INFO(fmt, ...) \
    pal_log_info("[%s] " fmt, LOG_TAG, ##__VA_ARGS__)
#define GZREMAP_DBG(fmt, ...) \
    pal_log_debug("[%s][DEBUG] %s:%d: " fmt, LOG_TAG, __func__, __LINE__, ##__VA_ARGS__)
#else
#define GZREMAP_ERR(fmt, ...) \
    pal_log_err("[%s][ERROR] %s: " fmt, LOG_TAG, __func__, __LINE__, ##__VA_ARGS__)
#define GZREMAP_INFO(fmt, ...) \
    pal_log_info("[%s] " fmt, LOG_TAG, ##__VA_ARGS__)
#define GZREMAP_DBG(fmt, ...) \
    do {} while (0)
#endif

#define get_bit_field(x, msk, off)  ((x & (msk << off)) >> off)


static int config_ddr_hyp_remap_en(bool is_enable)
{
    uint32_t sec_hyp = __raw_readl(INFRA_AO_SEC_HYP);

    GZREMAP_DBG("is_enable=%d\n", is_enable);

    if (is_enable)
        sec_hyp |= RG_DDR_HYP_REMAP_EN;
    else
        sec_hyp &= ~(RG_DDR_HYP_REMAP_EN);

    __raw_writel(sec_hyp, INFRA_AO_SEC_HYP);

    return GZ_REMAP_SUCCESS;
}

static int config_io_hyp_remap_en(bool is_enable)
{
    uint32_t sec_hyp = __raw_readl(INFRA_AO_SEC_HYP);

    GZREMAP_DBG("is_enable=%d\n", is_enable);

    if (is_enable)
        sec_hyp |= (RG_IO_HYP_REMAP_EN | RG_IO_VMID_SEC_REMAP_EN);
    else
        sec_hyp &= ~(RG_IO_HYP_REMAP_EN | RG_IO_VMID_SEC_REMAP_EN);

    __raw_writel(sec_hyp, INFRA_AO_SEC_HYP);

    return GZ_REMAP_SUCCESS;
}

static int config_vm_sec_io_hyp_remap_en(uint32_t vm_id, bool is_enable)
{
    uint32_t sec_hyp;
    uint32_t vm_sec_io_bitmask;

    GZREMAP_DBG("vm_id=%u, is_enable=%d\n", vm_id, is_enable);

    switch (vm_id) {
        case GZ_REMAP_VMID_AP:
            vm_sec_io_bitmask = RG_VMID0_SECURE_EN;
            break;
        case GZ_REMAP_VMID_GZ:
            vm_sec_io_bitmask = RG_VMID1_SECURE_EN;
            break;
        default:
            GZREMAP_ERR("Unknown VM ID=%u\n", vm_id);
            return -GZ_REMAP_ERR_INVALID_ARGUMENT;
    }

    sec_hyp = __raw_readl(INFRA_AO_SEC_HYP);
    if (is_enable)
        sec_hyp |= vm_sec_io_bitmask;
    else
        sec_hyp &= ~(vm_sec_io_bitmask);

    return GZ_REMAP_SUCCESS;
}

static uint64_t get_vm_offset(uint32_t vm_id)
{
    GZREMAP_DBG("vm_id=%u\n", vm_id);

    switch (vm_id) {
        case GZ_REMAP_VMID_AP:
        case GZ_REMAP_VMID_GZ:
            return GZ_REMAP_VM_OFFSET * vm_id;
        default:
            GZREMAP_ERR("Unknown VM ID=%u\n", vm_id);
            return -GZ_REMAP_ERR_INVALID_ARGUMENT;
    }

    /* should not reach */
    GZREMAP_ERR("Unhandled VM ID=%u\n", vm_id);
    ASSERT(0);
}


int gz_remap_ddr_enable(void)
{
    GZREMAP_DBG("\n");
    return config_ddr_hyp_remap_en(true);
}

int gz_remap_ddr_disable(void)
{
    GZREMAP_DBG("\n");
    return config_ddr_hyp_remap_en(false);
}

uint64_t gz_remap_ddr_get_offset(uint32_t vm_id)
{
    GZREMAP_DBG("vm_id=%u\n");
    return get_vm_offset(vm_id);
}

int gz_remap_io_enable(void)
{
    GZREMAP_DBG("\n");
    return config_io_hyp_remap_en(true);
}

int gz_remap_io_disable(void)
{
    GZREMAP_DBG("\n");
    return config_io_hyp_remap_en(false);
}

uint64_t gz_remap_io_get_offset(uint32_t vm_id)
{
    GZREMAP_DBG("vm_id=%u\n", vm_id);
    return get_vm_offset(vm_id);
}

int gz_remap_sec_io_enable(uint32_t vm_id)
{
    GZREMAP_DBG("vm_id=%u\n", vm_id);
    return config_vm_sec_io_hyp_remap_en(vm_id, true);
}

int gz_remap_sec_io_disable(uint32_t vm_id)
{
    GZREMAP_DBG("vm_id=%u\n", vm_id);
    return config_vm_sec_io_hyp_remap_en(vm_id, false);
}

uint64_t gz_remap_sec_io_get_offset(uint32_t vm_id)
{
    uint64_t vm_offset;

    GZREMAP_DBG("vm_id=%u\n", vm_id);

    vm_offset = get_vm_offset(vm_id);
    if (vm_offset == -GZ_REMAP_ERR_INVALID_ARGUMENT)
        return vm_offset;

    return vm_offset + GZ_REMAP_VM_SEC_IO_OFFSET;
}

int gz_remap_vm_domain_set(uint32_t vm_id, uint32_t map_domain)
{
    uint32_t domain_num;

    uint32_t bit_mask = ((0x1 << GZ_REMAP_VMDOMAIN_MAP_BITS) - 0x1);
    uint32_t bit_offset;

    GZREMAP_DBG("vm_id=%u, map_domain=%u\n", vm_id, map_domain);

    switch (vm_id) {
        case GZ_REMAP_VMID_AP:
        case GZ_REMAP_VMID_GZ:
            bit_offset = GZ_REMAP_VMDOMAIN_MAP_BITS * vm_id;
            break;
        default:
            GZREMAP_ERR("Unknown VM id=%u\n", vm_id);
            return -GZ_REMAP_ERR_INVALID_ARGUMENT;
    }

    if (map_domain & ~(bit_mask)) {
        GZREMAP_ERR("Invalid domain=%u\n", map_domain);
        return -GZ_REMAP_ERR_INVALID_ARGUMENT;
    }

    domain_num = __raw_readl(VPU_IOMMU_DOMAIN_NUM);

    domain_num &= ~(bit_mask << bit_offset);
    domain_num |= ((map_domain & bit_mask) << bit_offset);

    __raw_writel(domain_num, VPU_IOMMU_DOMAIN_NUM);

    return GZ_REMAP_SUCCESS;
}

int gz_remap_vm_domain_get(uint32_t vm_id)
{
    uint32_t domain_num;

    uint32_t bit_mask = ((0x1 << GZ_REMAP_VMDOMAIN_MAP_BITS) - 0x1);
    uint32_t bit_offset;

    GZREMAP_DBG("vm_id=%u\n", vm_id);

    switch (vm_id) {
        case GZ_REMAP_VMID_AP:
        case GZ_REMAP_VMID_GZ:
            bit_offset = GZ_REMAP_VMDOMAIN_MAP_BITS * vm_id;
            break;
        default:
            GZREMAP_ERR("Unknown VM ID=%u\n", vm_id);
            return -GZ_REMAP_ERR_INVALID_ARGUMENT;
    }

    domain_num = __raw_readl(VPU_IOMMU_DOMAIN_NUM);

    return get_bit_field(domain_num, bit_mask, bit_offset);
}

int gz_remap_dump_config(void)
{
    uint32_t i;

    uint32_t sec_hyp = __raw_readl(INFRA_AO_SEC_HYP);
    uint32_t domain_num = __raw_readl(VPU_IOMMU_DOMAIN_NUM);

    /* register raw value */
    GZREMAP_DBG("INFRA_AO_SEC_HYP=0x%x\n", sec_hyp);
    GZREMAP_DBG("VPU_IOMMU_DOMAIN_NUM=0x%x\n", domain_num);

    /* ddr, io, sec_io remap enable status */
    GZREMAP_INFO("DDR remap: %s\n",
            (sec_hyp & RG_DDR_HYP_REMAP_EN) ? "enabled" : "disabled");
    GZREMAP_INFO("IO remap: %s\n",
            (sec_hyp & RG_IO_HYP_REMAP_EN) ? "enabled" : "disabled");
    GZREMAP_INFO("IO_VMID_SEC remap: %s\n",
            (sec_hyp & RG_IO_VMID_SEC_REMAP_EN) ? "enabled" : "disabled");
    for (i = 0; i < GZ_REMAP_VM_NUM; ++i) {
        GZREMAP_INFO("    VMID%d_SECURE_EN: %s\n", i,
            (sec_hyp & RG_VMID0_SECURE_EN) ? "enabled" : "disabled");
    }

    /* ddr, io, sec_io remap offset */
    GZREMAP_INFO("DDR remap offset:\n");
    for (i = 0; i < GZ_REMAP_VM_NUM; ++i )
        GZREMAP_INFO("    VMID%d: 0x%llx\n", i, gz_remap_ddr_get_offset(i));

    GZREMAP_INFO("IO remap offset:\n");
    for (i = 0; i < GZ_REMAP_VM_NUM; ++i )
        GZREMAP_INFO("    VMID%d: 0x%llx\n", i, gz_remap_io_get_offset(i));

    GZREMAP_INFO("IO_VMID_SEC remap offset: \n");
    for (i = 0; i < GZ_REMAP_VM_NUM; ++i )
        GZREMAP_INFO("    VMID%d: 0x%llx\n", i, gz_remap_sec_io_get_offset(i));

    /* domain mapping */
    GZREMAP_INFO("Domain remap:\n");
    for (i = 0; i < GZ_REMAP_VM_NUM; ++i )
        GZREMAP_INFO("    VMID%d -> D%u\n", i, gz_remap_vm_domain_get(i));

    return GZ_REMAP_SUCCESS;
}

int gz_remap_init(void)
{
    int rc;

    rc = gz_remap_vm_domain_set(GZ_REMAP_VMID_AP, GZ_REMAP_VMDOMAIN_AP);
    if (rc != GZ_REMAP_SUCCESS) {
        GZREMAP_ERR("failed to map VM %u to Domain %u\n, rc=%d",
                GZ_REMAP_VMID_AP, GZ_REMAP_VMDOMAIN_AP, rc);
        return rc;
    }

    rc = gz_remap_vm_domain_set(GZ_REMAP_VMID_GZ, GZ_REMAP_VMDOMAIN_GZ);
    if (rc != GZ_REMAP_SUCCESS) {
        GZREMAP_ERR("failed to map VM %u to Domain %u\n, rc=%d",
                GZ_REMAP_VMID_GZ, GZ_REMAP_VMDOMAIN_GZ, rc);
        return rc;
    }

    rc = gz_remap_ddr_enable();
    if (rc != GZ_REMAP_SUCCESS) {
        GZREMAP_ERR("failed to enable DDR remap, rc=%d\n", rc);
        return rc;
    }

    rc = gz_remap_io_enable();
    if (rc != GZ_REMAP_SUCCESS) {
        GZREMAP_ERR("failed to enable IO remap, rc=%d\n", rc);
        return rc;
    }

    return GZ_REMAP_SUCCESS;
}
