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
 * MediaTek Inc. (C) 2016. All rights reserved.
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
#include "bootctrl.h"
#include "platform.h"
#include "avb_util.h"
#include "storage_api.h"
#include <partition_api.h>
#include <partition_active.h>
#include <pal_log.h>

#define BOOTCTR_MISC_PARTITION "misc"
#define BOOTCTR_PARA_PARTITION "para"
#define MOD "bootctrl"
/******************************************************************************
 * DEBUG
 ******************************************************************************/

static const char* suffix[2] = {BOOTCTRL_SUFFIX_A, BOOTCTRL_SUFFIX_B};
static AvbABData metadata_saved;
static int metadata_read = 0;
static boot_ctrl_t zero_metadata;

int check_suffix_with_slot(const char *suffix)
{
    int slot = -1;

    if(suffix == NULL) {
        pal_log_err("suffix NULL\n");
        return -1;
    }

    if(strcmp(suffix, BOOTCTRL_SUFFIX_A) == 0) {
        slot = 0;
    }
    else if(strcmp(suffix, BOOTCTRL_SUFFIX_B) == 0) {
        slot = 1;
    }

   return slot;
}

static int read_write_partition_info_backup_restore_bootctrlv1(boot_ctrl_t *bctrl ,int mode,int offset)
{
    blkdev_t *bootdev   = NULL;
    part_t *part        = NULL;
    u64 src = 0;
    u32 part_id = 0;


    int ret = -1, storage_type = 0;

    storage_type = ab_get_storage_type();
    if(storage_type == -1) {
        pal_log_err("unknown device\n");
        return -1;
    }

    if (NULL == (bootdev = blkdev_get(CFG_BOOT_DEV))) {
        pal_log_err("[%s] can't find boot device(%d)\n", MOD, CFG_BOOT_DEV);
        return -1;
    }

    part = part_get(BOOTCTR_MISC_PARTITION);
    if(part == NULL)
        part = part_get(BOOTCTR_PARA_PARTITION);

    if(part == NULL) {
#if (CFG_DRAM_CALIB_OPTIMIZATION || EARLY_PARTITION_ACCESS)
            pal_log_debug("[%s] Try to query by sram\n", MOD);
            /* The fail might caused by dram not init yet, using sram query then */

            src = get_part_addr(BOOTCTR_MISC_PARTITION);
            if (src == 0) {
                pal_log_err("[%s] get part misc fail try para\n", MOD);
                src = get_part_addr(BOOTCTR_PARA_PARTITION);
                if (src == 0)
                    pal_log_err("[%s] get part para fail\n", MOD);
            }

            part_id = storage_get_part_id(STORAGE_PHYS_PART_USER);
#else
            pal_log_err("[%s] part_get fail\n", MOD);
#endif
    } else {
        src = (u64)part->start_sect * bootdev->blksz;
        part_id = part->part_id;
    }
    src += OFFSETOF_SLOT_SUFFIX + offset;

    if(bctrl == NULL) {
        pal_log_err("bctrl is NULL\n");
        return ret;
    }

    if(mode == READ_PARTITION) {

        if (-1 == blkdev_read(bootdev, src, (u32)sizeof(boot_ctrl_t), (char *)bctrl, part_id)) {
            pal_log_err("[%s] part_load fail\n", MOD);
            return ret;
        }
    }
    else if(mode == WRITE_PARTITION) {

        if (-1 == blkdev_write(bootdev, src, (u32)sizeof(boot_ctrl_t), (char *)bctrl, part_id)) {
            pal_log_err("[%s] part_load fail\n", MOD);
            return ret;
        }
    }
    else {
        pal_log_err("unknown mode\n");
        return ret;
    }
    ret = 0;
    return ret;
}


static int read_write_partition_info(AvbABData *bctrl ,int mode)
{
    blkdev_t *bootdev   = NULL;
    part_t *part        = NULL;
    u64 src = 0;
    u32 part_id = 0;


    int ret = -1, storage_type = 0;

    storage_type = ab_get_storage_type();
    if(storage_type == -1) {
        pal_log_err("unknown device\n");
        return -1;
    }

    if (NULL == (bootdev = blkdev_get(CFG_BOOT_DEV))) {
        pal_log_err("[%s] can't find boot device(%d)\n", MOD, CFG_BOOT_DEV);
        return -1;
    }

    part = part_get(BOOTCTR_MISC_PARTITION);
    if(part == NULL)
        part = part_get(BOOTCTR_PARA_PARTITION);

    if(part == NULL) {
#if (CFG_DRAM_CALIB_OPTIMIZATION || EARLY_PARTITION_ACCESS)
            pal_log_debug("[%s] Try to query by sram\n", MOD);
            /* The fail might caused by dram not init yet, using sram query then */

            src = get_part_addr(BOOTCTR_MISC_PARTITION);
            if (src == 0) {
                pal_log_err("[%s] get part misc fail try para\n", MOD);
                src = get_part_addr(BOOTCTR_PARA_PARTITION);
                if (src == 0)
                    pal_log_err("[%s] get part para fail\n", MOD);
            }

            part_id = storage_get_part_id(STORAGE_PHYS_PART_USER);
#else
            pal_log_err("[%s] part_get fail\n", MOD);
#endif
    } else {
        src = (u64)part->start_sect * bootdev->blksz;
        part_id = part->part_id;
    }
    src += OFFSETOF_SLOT_SUFFIX;

    if(bctrl == NULL) {
        pal_log_err("bctrl is NULL\n");
        return ret;
    }

    if(mode == READ_PARTITION) {
      if ((metadata_read)  && (!memcmp(metadata_saved.magic ,AVB_AB_MAGIC, AVB_AB_MAGIC_LEN)))  {
        memcpy(bctrl, &metadata_saved, sizeof(AvbABData));
      }
      else {
        if (-1 == blkdev_read(bootdev, src, (u32)sizeof(AvbABData), (char *)bctrl, part_id)) {
            pal_log_err("[%s] part_load fail\n", MOD);
            return ret;
        }
        else {
          memcpy(&metadata_saved, bctrl, sizeof(AvbABData));
          metadata_read = 1;
        }
      }
    }
    else if(mode == WRITE_PARTITION) {
        bctrl->crc32 = avb_htobe32(
             avb_crc32((const uint8_t*)bctrl, sizeof(AvbABData) - sizeof(uint32_t)));

        if (-1 == blkdev_write(bootdev, src, (u32)sizeof(AvbABData), (char *)bctrl, part_id)) {
            pal_log_err("[%s] part_load fail\n", MOD);
            return ret;
        }
        else {
            metadata_read = 0;  //force to read from partition after successful blkdev_write()
        }
    }
    else {
        pal_log_err("unknown mode\n");
        return ret;
    }
    ret = 0;
    return ret;
}

static int ab_metadata_init(const char *suffix) {
    int slot = 0 ,slot1 = 0;
    int ret = -1 ,storage_type = -1;
    AvbABSlotData *slotp;

    AvbABData metadata;

    slot = check_suffix_with_slot(suffix);
    storage_type = ab_get_storage_type();
    if(storage_type == -1) {
        pal_log_err("unknown device\n");
        return -1;
    }
    if(slot == -1) {
        pal_log_err("ab init fail, slot: 0x%x\n", slot);
        return -1;
    }

    if(suffix == NULL) {
        pal_log_err("suffix is NULL\n");
        return -1;
    }

    ret = read_write_partition_info(&metadata, READ_PARTITION);
    if(ret < 0) {
        pal_log_err("partition_read fail, ret: 0x%x\n", ret);
        return -1;
    }

    memcpy(metadata.magic ,AVB_AB_MAGIC, AVB_AB_MAGIC_LEN);

    /* Set highest priority and reset retry count */
    slotp = &metadata.slots[slot];
    slotp->successful_boot = 0;
    slotp->priority = AVB_AB_MAX_PRIORITY;
    slotp->tries_remaining = AVB_AB_MAX_TRIES_REMAINING;

    /* Re-set arg to another slot */
    slot1 = (slot == 0) ? 1 : 0;
    slotp = &metadata.slots[slot1];
    slotp->successful_boot = 0;
    slotp->priority = AVB_AB_MAX_PRIORITY - 1;
    slotp->tries_remaining = AVB_AB_MAX_TRIES_REMAINING;

    ret = read_write_partition_info(&metadata, WRITE_PARTITION);
    if (ret < 0) {
        pal_log_err("partition_write fail, ret: 0x%x\n", ret);
        return -1;
    }

    return 0;
}

const char *get_suffix(void)
{
    int slot = 0, ret = -1;

    AvbABData metadata;
	boot_ctrl_t metadatabootctrlv1;    
	boot_ctrl_t metadatabootctrlv2;    
	boot_part_t bootpart =BOOT_PART_NONE;    
	int slot_bootctrlv1 = 0;

    ret = read_write_partition_info(&metadata, READ_PARTITION);
    if (ret < 0) {
        pal_log_err("get_suffix fail, ret: 0x%x\n", ret);
        return NULL;
    }

    if(memcmp(metadata.magic , AVB_AB_MAGIC, AVB_AB_MAGIC_LEN) != 0) {
		pal_log_err("booctrl magic not match init default value\n");
		read_write_partition_info_backup_restore_bootctrlv1(&metadatabootctrlv1,READ_PARTITION,0);
        if(metadatabootctrlv1.magic == BOOTCTRL_MAGIC){
			if(metadatabootctrlv1.slot_info[0].priority > metadatabootctrlv1.slot_info[1].priority){
				//bootctrlv1 in B part, so we need to backup bootctrv1
				read_write_partition_info_backup_restore_bootctrlv1(&metadatabootctrlv2,READ_PARTITION,32);
				//if not backup  need  backup (mark)
				if(metadatabootctrlv2.magic != BOOTCTRL_MAGIC)
					read_write_partition_info_backup_restore_bootctrlv1(&metadatabootctrlv1,WRITE_PARTITION,32);
				slot = 0;
				pal_log_err("reinit to BOOTCTRL_SUFFIX_A \n");
				ab_metadata_init(BOOTCTRL_SUFFIX_A);	
			}else{
				//bootctrv1 in A part,no need deal, if hasbackup,need wipe
				read_write_partition_info_backup_restore_bootctrlv1(&metadatabootctrlv2,READ_PARTITION,32);
				if(metadatabootctrlv2.magic == BOOTCTRL_MAGIC){
					//wipe out
					memcpy(&metadatabootctrlv2,&zero_metadata,sizeof(boot_ctrl_t));
					read_write_partition_info_backup_restore_bootctrlv1(&metadatabootctrlv2,WRITE_PARTITION,32);
				}
                slot = 1;
				pal_log_err("reinit to BOOTCTRL_SUFFIX_B \n");
				ab_metadata_init(BOOTCTRL_SUFFIX_B);	
					
			}
		}else{
			slot = 0;
			pal_log_err("reinit to BOOTCTRL_SUFFIX_A \n");
			ab_metadata_init(BOOTCTRL_SUFFIX_A);
		}
    }   
    else {

        if(metadata.slots[0].priority >= metadata.slots[1].priority)
           slot = 0;
        else if (metadata.slots[0].priority < metadata.slots[1].priority)
           slot = 1;
    }

    return suffix[slot];
}


int rollback_slot(const char *suffix) {
    int slot = 0 ,slot1 = 0;
    int ret = -1;
    AvbABSlotData *slotp;

    AvbABData metadata;

    slot = check_suffix_with_slot(suffix);
    if(slot == -1) {
        pal_log_err("rollback_slot fail, slot: 0x%x\n", slot);
        return -1;
    }

    if(suffix == NULL) {
        pal_log_err("suffix NULL\n");
        return -1;
    }

    ret = read_write_partition_info(&metadata, READ_PARTITION);
    if(ret < 0) {
        pal_log_err("rollback_slot fail, ret: 0x%x\n", ret);
        return -1;
    }

    memcpy(metadata.magic ,AVB_AB_MAGIC, AVB_AB_MAGIC_LEN);

    /* Set highest priority and reset retry count */
    slotp = &metadata.slots[slot];
    slotp->priority = AVB_AB_MAX_PRIORITY;

    /* Ensure other slot doesn't have as high a priority. */
    slot1 = (slot == 0) ? 1 : 0;
    slotp = &metadata.slots[slot1];
    if(slotp->priority == AVB_AB_MAX_PRIORITY)
        slotp->priority = AVB_AB_MAX_PRIORITY - 1;

    ret = read_write_partition_info(&metadata, WRITE_PARTITION);
    if (ret < 0) {
        pal_log_err("partition_write fail, ret: 0x%x\n", ret);
        return -1;
    }

    return 0;
}

uint8_t get_retry_count(const char *suffix)
{
    int slot = 0;
    int ret = -1;
    AvbABSlotData *slotp;
    AvbABData metadata;

    slot = check_suffix_with_slot(suffix);
    if(slot == -1) {
        pal_log_err("check fail, slot: 0x%x\n", slot);
        return -1;
    }

    ret = read_write_partition_info(&metadata, READ_PARTITION);
    if (ret < 0) {
        pal_log_err("partition_read fail, ret: 0x%x\n", ret);
        return -1;
    }

    slotp = &metadata.slots[slot];
    return slotp->tries_remaining;
}

int reduce_retry_count(const char *suffix)
{
    int slot = 0, ret = -1;
    AvbABSlotData *slotp;
    AvbABData metadata;

    slot = check_suffix_with_slot(suffix);
    if(slot == -1) {
        pal_log_err("check fail, slot: 0x%x\n", slot);
        return -1;
    }

    ret = read_write_partition_info(&metadata, READ_PARTITION);
    if(ret < 0) {
        pal_log_err("partition_read fail, ret: 0x%x\n", ret);
        return -1;
    }

    slotp = &metadata.slots[slot];
    if(slotp->tries_remaining > 0)
        slotp->tries_remaining--;

    ret = read_write_partition_info(&metadata, WRITE_PARTITION);
    if(ret < 0) {
        pal_log_err("partition_write fail, ret: 0x%x\n", ret);
        return -1;
    }

    return 0;
}

int check_valid_slot(void)
{
    int slot = 0, ret = -1;
    AvbABData metadata;

    ret = read_write_partition_info(&metadata, READ_PARTITION);

    if (ret < 0) {
        pal_log_err("write partition fail, ret: 0x%x\n", ret);
        return -1;
    }

    if(metadata.slots[0].priority > 0)
           return 0;
    else if (metadata.slots[1].priority > 0)
           return 0;

    return -1;
}

int mark_slot_invalid(const char *suffix)
{
    int slot = 0, slot2 = 0, ret = -1;
    AvbABSlotData *slotp;
    AvbABData metadata;

    slot = check_suffix_with_slot(suffix);
    if(slot == -1) {
        pal_log_err("invliad slot , slot: 0x%x\n", slot);
        return -1;
    }

    ret = read_write_partition_info(&metadata, READ_PARTITION);
    if(ret < 0) {
        pal_log_err("part read fail, ret: 0x%x\n", ret);
        return -1;
    }

    slotp = &metadata.slots[slot];
    slotp->successful_boot = 0;
    slotp->priority = 0;

    ret = read_write_partition_info(&metadata, WRITE_PARTITION);
    if(ret < 0) {
        pal_log_err("part write fail, ret: 0x%x\n", ret);
        return -1;
    }

    return 0;
}

int get_bootup_status(const char *suffix)
{
    int slot = 0, ret = -1;
    AvbABSlotData *slotp;
    AvbABData metadata;

    slot = check_suffix_with_slot(suffix);
    if(slot == -1) {
        pal_log_err("invliad slot, slot: 0x%x\n", slot);
        return -1;
    }

    ret = read_write_partition_info(&metadata, READ_PARTITION);
    if(ret < 0) {
        pal_log_err("part read fail, ret: 0x%x\n", ret);
        return -1;
    }

    slotp = &metadata.slots[slot];
    return slotp->successful_boot;

    return 0;
}

int ab_get_storage_type(void)
{
    blkdev_t *bootdev = NULL;
    bootdev = blkdev_get(CFG_BOOT_DEV);

    if (bootdev == NULL) {
        pal_log_debug("%s can't find boot device(%d)\n", MOD, CFG_BOOT_DEV);
        return -1;
    }

    if (bootdev->type == BOOTDEV_SDMMC) {
        pal_log_debug("device is EMMC\n");
        return BOOTDEV_SDMMC;
    } else if (bootdev->type == BOOTDEV_UFS) {
        pal_log_debug("device is UFS\n");
        return BOOTDEV_UFS;
    }

    pal_log_debug("unknown device\n");
    return -1;
}

int ab_get_boot_part(u32 *bootpart)
{
    int ret = 0, storage_type = 0;
    u32 boot_part = 0;

    storage_type = ab_get_storage_type();
    if(storage_type == -1) {
        pal_log_debug("unknown device\n");
        return -1;
    }

    if (storage_type == BOOTDEV_SDMMC) {
        ret = mmc_get_boot_part(&boot_part);

        if(boot_part == EMMC_PART_BOOT1)
            *bootpart = BOOT_PART_A;
        else if(boot_part == EMMC_PART_BOOT2)
            *bootpart = BOOT_PART_B;
    } else if (storage_type == BOOTDEV_UFS) {
        ret = ufs_get_boot_part(&boot_part);

        if(boot_part == STORAGE_PHYS_PART_BOOT1)
            *bootpart = BOOT_PART_A;
        else if(boot_part == STORAGE_PHYS_PART_BOOT2)
            *bootpart = BOOT_PART_B;
    }
    return ret;
}

int ab_set_boot_part(u32 bootpart)
{
    int storage_type = 0;
	int slot = 0;    
	boot_ctrl_t metadatabootctrlv1;	
	if (bootpart ==BOOT_PART_B)		
		restore_bootctrv1_slotB();

    storage_type = ab_get_storage_type();
    if(storage_type == -1) {
        pal_log_debug("unknown device\n");
        return -1;
    }

    if (storage_type == BOOTDEV_SDMMC) {
        return mmc_set_boot_part(bootpart);
    } else if (storage_type == BOOTDEV_UFS) {

	if (bootpart == BOOT_PART_A)
		bootpart = STORAGE_PHYS_PART_BOOT1;
	else if (bootpart == BOOT_PART_B)
		bootpart = STORAGE_PHYS_PART_BOOT2;

        return ufs_set_boot_part(bootpart);
    }
}

int restore_bootctrv1_slotB()
{
	boot_ctrl_t metadatabootctrlv1;
	read_write_partition_info_backup_restore_bootctrlv1(&metadatabootctrlv1,READ_PARTITION,32);
	if(metadatabootctrlv1.magic == BOOTCTRL_MAGIC){
		set_active_slot(BOOTCTRL_SUFFIX_B);
	}
	return 0;
}

int set_active_slot(const char *suffix) {
    int slot = 0 ,slot1 = 0;
    int ret = -1;
    slot_metadata_t *slotp;

    boot_ctrl_t metadata;

    slot = check_suffix_with_slot(suffix);
    if(slot == -1) {
        pal_log_err("set_active_slot failed, slot: 0x%x\n", slot);
        return -1;
    }

    if(suffix == NULL) {
        pal_log_err("input suffix is NULL\n");
        return -1;
    }

    ret = read_write_partition_info_backup_restore_bootctrlv1(&metadata,READ_PARTITION,0);
    if(ret < 0) {
        pal_log_err("partition_read failed, ret: 0x%x\n", ret);
        return -1;
    }

    metadata.magic = BOOTCTRL_MAGIC;

    /* Set highest priority and reset retry count */
    slotp = &metadata.slot_info[slot];
    slotp->successful_boot = 0;
    slotp->priority = 7;
    slotp->retry_count = 3;
    slotp->normal_boot = 1;

    /* Re-set arg to another slot */
    slot1 = (slot == 0) ? 1 : 0;
    slotp = &metadata.slot_info[slot1];
    slotp->successful_boot = 0;
    slotp->priority = 0;
    slotp->retry_count = 0;
    slotp->normal_boot = 0;

    ret = read_write_partition_info_backup_restore_bootctrlv1(&metadata,WRITE_PARTITION,0);
    if (ret < 0) {
        pal_log_err("partition_write failed, ret: 0x%x\n", ret);
        return -1;
    }

    return 0;
}

