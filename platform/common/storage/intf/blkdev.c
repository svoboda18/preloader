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

#include "typedefs.h"
#include "platform.h"
#include "blkdev.h"
#include "dram_buffer.h"

static blkdev_t *blkdev_list = NULL;

#if CFG_DRAM_CALIB_OPTIMIZATION || MTK_EFUSE_WRITER_SUPPORT

/*
 * DRAM calibration optimization
 *
 * This feature requires storage device being initialized before DRAM initialization.
 * Therefore DRAM driver can access calibration data on storage devices.
 *
 * In such case, we shall make sure all buffers touched by storage driver are located
 * in SRAM.
 *
 * Blkdev APIs will use a local buffer to access non-block-size-aligned part.
 * Since preloader linker will always allocate local buffer in SRAM, we declare
 * it here to make sure it is surely in SRAM.
 */
#ifndef BLKDEV_FORCE_USE_SRAM_FOR_UNALIGNED_ACCESS
#define BLKDEV_FORCE_USE_SRAM_FOR_UNALIGNED_ACCESS
#endif

#endif

#ifdef BLKDEV_FORCE_USE_SRAM_FOR_UNALIGNED_ACCESS

#if defined(BOOTDEV_SDMMC_UFS_COMBO)
/* Use UFS, which is large then eMMC */
#define BOOT_DEV_BLOCK_SIZE	(UFS_BLOCK_SIZE)
#else
#if (CFG_BOOT_DEV == BOOTDEV_UFS)
#define BOOT_DEV_BLOCK_SIZE	(UFS_BLOCK_SIZE)
#elif (CFG_BOOT_DEV == BOOTDEV_SDMMC)
#define BOOT_DEV_BLOCK_SIZE	(MMC_BLOCK_SIZE)
#else
/*
 * NAND projects
 *
 * 1. NAND driver already uses local buffer (SRAM buffer).
 *
 * 2. We cannot use any macros to define NAND block size because page size
 *    may be different in differenct devices.
 *
 * Therefore a dummy SRAM buffer is declared here (not used anymore).
 */
#define BOOT_DEV_BLOCK_SIZE	4096
#endif
#endif

static u32 blkdev_buf_in_sram[BOOT_DEV_BLOCK_SIZE / sizeof(u32)];
#endif /* BLKDEV_FORCE_USE_SRAM_FOR_UNALIGNED_ACCESS */

int blkdev_register(blkdev_t *bdev)
{
    blkdev_t *tail = blkdev_list;

    bdev->next = NULL;

    while (tail && tail->next) {
        tail = tail->next;
    }

    if (tail) {
        tail->next = bdev;
    } else {
        blkdev_list = bdev;
    }

    return 0;
}

blkdev_t *blkdev_get(u32 type)
{
    blkdev_t *bdev = blkdev_list;

    while (bdev) {
        if (bdev->type == type)
            break;
#if defined(BOOTDEV_SDMMC_UFS_COMBO)
        /*
         * CFG_BOOT_DEV is fixed at build time, which is BOOTDEV_SDMMC or BOOTDEV_UFS
         * Regard bdev found if bdev->type == BOOTDEV_UFS(run time get) &&
         * CFG_BOOT_DEV == BOOTDEV_SDMMC(build time), and vise versa...
         */
        if ((bdev->type == BOOTDEV_UFS) && (type == BOOTDEV_SDMMC))
            break;
        if ((bdev->type == BOOTDEV_SDMMC) && (type == BOOTDEV_UFS))
            break;
#endif
        bdev = bdev->next;
    }
    return bdev;
}


int blkdev_bread(blkdev_t *bdev, u32 blknr, u32 blks, u8 *buf, u32 part_id)
{
    return bdev->bread(bdev, blknr, blks, buf, part_id);
}

int blkdev_bwrite(blkdev_t *bdev, u32 blknr, u32 blks, u8 *buf, u32 part_id)
{
    return bdev->bwrite(bdev, blknr, blks, buf, part_id);
}

int blkdev_read(blkdev_t *bdev, u64 src, u32 size, u8 *dst, u32 part_id)
{
    u8 *buf;
    u32 blksz;
    u64 end, part_start, part_end, part_len, aligned_start, aligned_end;
    u32 blknr, blks;

    if (!bdev) {
        return -1;
    }

    blksz = bdev->blksz;

#ifdef BLKDEV_FORCE_USE_SRAM_FOR_UNALIGNED_ACCESS
    buf = (u8 *)bdev->blkbuf ? bdev->blkbuf : (u8 *)blkdev_buf_in_sram;
#else
    buf = (u8 *)bdev->blkbuf ? bdev->blkbuf : g_dram_buf->storage_buffer;
#endif

    if (blksz == 0)
        return -1;

    if (size == 0)
        return 0;

    end = src + size;

    part_start    = src &  ((u64)blksz - 1);
    part_end      = end &  ((u64)blksz - 1);
    aligned_start = src & ~((u64)blksz - 1);
    aligned_end   = end & ~((u64)blksz - 1);

    if (part_start) {
        blknr = aligned_start / blksz;
        part_len = part_start + size > blksz ? blksz - part_start : size;
        if ((bdev->bread(bdev, blknr, 1, buf, part_id)) != 0)
            return -1;
        memcpy(dst, buf + part_start, part_len);
        dst  += part_len;
        src  += part_len;
        size -= part_len;
    }

    if (size >= blksz) {
        aligned_start = src & ~((u64)blksz - 1);
        blknr  = aligned_start / blksz;
        blks = (aligned_end - aligned_start) / blksz;

        if (blks && 0 != bdev->bread(bdev, blknr, blks, dst, part_id))
            return -1;

        src  += (blks * blksz);
        dst  += (blks * blksz);
        size -= (blks * blksz);
    }
    if (size && part_end && src < end) {
        blknr = aligned_end / blksz;
        if ((bdev->bread(bdev, blknr, 1, buf, part_id)) != 0)
            return -1;
        memcpy(dst, buf, part_end);
    }
    return 0;
}

int blkdev_boot_read(blkdev_t *bdev, u64 src, u32 size, u8 *dst)
{
    u8 *buf;
    u32 blksz;
    u64 end, part_start, part_end, part_len, aligned_start, aligned_end;
    u32 blknr, blks;

    if (!bdev) {
        return -1;
    }

    blksz = bdev->blksz;

#ifdef BLKDEV_FORCE_USE_SRAM_FOR_UNALIGNED_ACCESS
    buf = (u8 *)bdev->blkbuf ? bdev->blkbuf : (u8 *)blkdev_buf_in_sram;
#else
    buf = (u8 *)bdev->blkbuf ? bdev->blkbuf : g_dram_buf->storage_buffer;
#endif

    if (blksz == 0)
        return -1;

    if (size == 0)
        return 0;

    end = src + size;

    part_start    = src &  ((u64)blksz - 1);
    part_end      = end &  ((u64)blksz - 1);
    aligned_start = src & ~((u64)blksz - 1);
    aligned_end   = end & ~((u64)blksz - 1);

    if (part_start) {
        blknr = aligned_start / blksz;
        part_len = part_start + size > blksz ? blksz - part_start : size;
        if (mmc_bread_boot(bdev, (unsigned long)blknr, 1, buf) != 0)
            return -1;
        memcpy(dst, buf + part_start, part_len);
        dst  += part_len;
        src  += part_len;
        size -= part_len;
    }

    if (size >= blksz) {
        aligned_start = src & ~((u64)blksz - 1);
        blknr  = aligned_start / blksz;
        blks = (aligned_end - aligned_start) / blksz;

        if (blks && 0 != mmc_bread_boot(bdev, (unsigned long)blknr, blks, dst))
            return -1;

        src  += (blks * blksz);
        dst  += (blks * blksz);
        size -= (blks * blksz);
    }
    if (size && part_end && src < end) {
        blknr = aligned_end / blksz;
        if (mmc_bread_boot(bdev, (unsigned long)blknr, 1, buf))
            return -1;
        memcpy(dst, buf, part_end);
    }
    return 0;
}

int blkdev_write(blkdev_t *bdev, u64 dst, u32 size, u8 *src, u32 part_id)
{
    u8 *buf;
    u32 blksz;
    u64 end, part_start, part_end, part_len, aligned_start, aligned_end;
    u32 blknr, blks;

    if (!bdev) {
        return -1;
    }

    blksz = bdev->blksz;

#ifdef BLKDEV_FORCE_USE_SRAM_FOR_UNALIGNED_ACCESS
    buf = (u8 *)bdev->blkbuf ? bdev->blkbuf : (u8 *)blkdev_buf_in_sram;
#else
    buf = (u8 *)bdev->blkbuf ? bdev->blkbuf : g_dram_buf->storage_buffer;
#endif

    if (blksz == 0)
        return -1;

    if (size == 0)
        return 0;

    end = dst + size;

    part_start    = dst &  ((u64)blksz - 1);
    part_end      = end &  ((u64)blksz - 1);
    aligned_start = dst & ~((u64)blksz - 1);
    aligned_end   = end & ~((u64)blksz - 1);

    if (part_start) {
        blknr = aligned_start / blksz;
        part_len = part_start + size > blksz ? blksz - part_start : size;
        if ((bdev->bread(bdev, blknr, 1, buf, part_id)) != 0)
            return -1;
        memcpy(buf + part_start, src, part_len);
        if ((bdev->bwrite(bdev, blknr, 1, buf, part_id)) != 0)
            return -1;
        dst  += part_len;
        src  += part_len;
        size -= part_len;
    }

    if (size >= blksz) {
        aligned_start = dst & ~((u64)blksz - 1);
        blknr  = aligned_start / blksz;
        blks = (aligned_end - aligned_start) / blksz;

        if (blks && 0 != bdev->bwrite(bdev, blknr, blks, src, part_id))
            return -1;

        src  += (blks * blksz);
        dst  += (blks * blksz);
        size -= (blks * blksz);
    }

    if (size && part_end && dst < end) {
        blknr = aligned_end / blksz;
        if ((bdev->bread(bdev, blknr, 1, buf, part_id)) != 0)
            return -1;
        memcpy(buf, src, part_end);
        if ((bdev->bwrite(bdev, blknr, 1, buf, part_id)) != 0)
            return -1;
    }
    return 0;
}

