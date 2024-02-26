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


static inline int config_ddr_hyp_en(bool is_enable)
{
    uint32_t reg_sec_hyp = __raw_readl(INFRA_AO_SEC_HYP);

    GZREMAP_DBG("is_enable=%d\n", is_enable);

    if (is_enable)
        reg_sec_hyp |= RG_DDR_HYP_REMAP_EN;
    else
        reg_sec_hyp &= ~(RG_DDR_HYP_REMAP_EN);

    __raw_writel(reg_sec_hyp, INFRA_AO_SEC_HYP);

    return GZ_REMAP_SUCCESS;
}

static inline int config_io_hyp_en(bool is_enable)
{
    volatile uint32_t reg_sec_hyp = __raw_readl(INFRA_AO_SEC_HYP);

    GZREMAP_DBG("is_enable=%d\n", is_enable);

    if (is_enable)
        reg_sec_hyp |= RG_IO_HYP_REMAP_EN;
    else
        reg_sec_hyp &= ~(RG_IO_HYP_REMAP_EN);

    __raw_writel(reg_sec_hyp, INFRA_AO_SEC_HYP);

    return GZ_REMAP_SUCCESS;
}

static inline int config_ddr_hyp(uint32_t mask, uint32_t match)
{
    volatile uint32_t reg_sec_hyp = __raw_readl(INFRA_AO_SEC_HYP);

    GZREMAP_DBG("mask=0x%x, match=0x%x\n", mask, match);

    mask <<= RG_DDR_HYP_ADDR_MASK_OFFSET;
    mask &= RG_DDR_HYP_ADDR_MASK_MASK;

    match <<= RG_DDR_HYP_ADDR_MATCH_OFFSET;
    match &= RG_DDR_HYP_ADDR_MATCH_MASK;

    reg_sec_hyp &= ~(RG_DDR_HYP_ADDR_MASK_MASK | RG_DDR_HYP_ADDR_MATCH_MASK);
    reg_sec_hyp |= (mask | match);

    __raw_writel(reg_sec_hyp, INFRA_AO_SEC_HYP);

    return GZ_REMAP_SUCCESS;
}


int gz_remap_ddr_enable(void)
{
    GZREMAP_DBG("\n");
    return config_ddr_hyp_en(true);
}

int gz_remap_ddr_disable(void)
{
    GZREMAP_DBG("\n");
    return config_ddr_hyp_en(false);
}

uint64_t gz_remap_ddr_get_offset(uint32_t vm_id)
{
    uint64_t offset;

    GZREMAP_DBG("vm_id=%u\n", vm_id);

    switch (vm_id) {
        case GZ_REMAP_VMID_AP:
            offset = 0;
            break;
        case GZ_REMAP_VMID_GZ:
            offset = GZ_REMAP_DDR_OFFSET;
            break;
        default:
            GZREMAP_ERR("Unknown VM ID=%u\n", vm_id);
            return -GZ_REMAP_ERR_INVALID_ARGUMENT;
    }

    return offset;
}

int gz_remap_io_enable(void)
{
    GZREMAP_DBG("\n");
    return config_io_hyp_en(true);
}

int gz_remap_io_disable(void)
{
    GZREMAP_DBG("\n");
    return config_io_hyp_en(false);
}

uint64_t gz_remap_io_get_offset(uint32_t vm_id)
{
    uint64_t offset;

    GZREMAP_DBG("vm_id=%u\n", vm_id);

    switch (vm_id) {
        case GZ_REMAP_VMID_AP:
            offset = 0;
            break;
        case GZ_REMAP_VMID_GZ:
            offset = GZ_REMAP_IO_OFFSET;
            break;
        default:
            GZREMAP_ERR("Unknown VM ID=%u\n", vm_id);
            return -GZ_REMAP_ERR_INVALID_ARGUMENT;
    }

    return offset;
}

int gz_remap_dump_config(void)
{
    uint32_t i;
    uint32_t sec_hyp = __raw_readl(INFRA_AO_SEC_HYP);

    /* register raw value */
    GZREMAP_DBG("INFRA_AO_SEC_HYP=0x%x\n", INFRA_AO_SEC_HYP);

    /* remap enable status */
    GZREMAP_INFO("DDR remap: %s\n",
            (sec_hyp & RG_DDR_HYP_REMAP_EN) ? "enabled" : "disabled");
    GZREMAP_INFO("IO remap: %s\n",
            (sec_hyp & RG_IO_HYP_REMAP_EN) ? "enabled" : "disabled");

    /* per-VM remap offset */
    GZREMAP_INFO("DDR remap offset:\n");
    for (i = 0; i < GZ_REMAP_VM_NUM; ++i)
        GZREMAP_INFO("    VMID%d: 0x%llx\n", i, gz_remap_ddr_get_offset(i));
    GZREMAP_INFO("IO remap offset:\n");
    for (i = 0; i < GZ_REMAP_VM_NUM; ++i)
        GZREMAP_INFO("    VMID%d: 0x%llx\n", i, gz_remap_io_get_offset(i));

    return GZ_REMAP_SUCCESS;
}

int gz_remap_init(void)
{
    int rc;

    rc = config_ddr_hyp(GZ_REMAP_DDR_HYP_ADDR_MASK, GZ_REMAP_DDR_HYP_ADDR_MATCH);
    if (rc != GZ_REMAP_SUCCESS) {
        GZREMAP_ERR("failed to configure remap mask and match, rc=%d\n", rc);
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
