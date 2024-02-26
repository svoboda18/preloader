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

#include <pal_typedefs.h>
#include <pal_log.h>
#include <pal_assert.h>

#include <partition_api.h>
#include <partition_error.h>
#include <partition_active.h>
#include <storage_api.h>
#include <bootctrl.h>
#include <platform.h>

#ifdef MTK_PARTITION_COMMON
#include "dram_buffer.h"
#include "partition.h"
#define part_num (g_dram_buf->part_num)
#define part_info (g_dram_buf->part_info)
#define partition_info  (g_dram_buf->partition_info)
#define meta_info       (g_dram_buf->meta_info)
#endif

#define AB_TAG_SZ     (2)

#ifdef MTK_PARTITION_COMMON
static u8 g_cur_part_name[32] = {0};
static u8 g_part_ready = 0;

struct active_part_entry g_active_part_tbl[] = {
	{"lk", "lk", "lk2"},
	{"tee", "tee1", "tee2"},
	{"gz", "gz1", "gz2"},
	{"sspm", "sspm_1", "sspm_2"},
	{"scp", "scp1", "scp2"},
	{"NULL", "NULL", "NULL"},
};
#endif

static uint32_t name_compare(char *name1, char *name2)
{
	uint32_t name1_sz = strlen(name1);
	uint32_t name2_sz = strlen(name2);

	if (name1_sz != name2_sz)
		return 1;

	return memcmp(name1, name2, sizeof(char) * name1_sz);
}

uint32_t partition_exists(char *part_name)
{
	uint32_t exist = 0;
	part_t *part = NULL;

	if (NULL == part_name) {
		set_last_error(ERR_PART_GENERAL_INVALID_NAME);
		return -1;
	}

	if (NULL == (part = part_get(part_name)))
		exist = 0;
	else
		exist = 1;

	return exist;
}

uint32_t partition_get_gpt_active(char *part_name, uint32_t part_name_buf_sz,
				  uint32_t op)
{
	uint32_t ret = STATUS_OK;
	uint32_t active_1 = 0, active_2 = 0;
	uint32_t found = 0;
	uint32_t choice = 0;
	part_t *part = NULL;
	struct active_part_entry *iter = &g_active_part_tbl[0];

	if (NULL == part_name)
		return ERR_PART_GENERAL_INVALID_NAME;

	if (op != GET_ACTIVE && op != GET_INACTIVE)
		return ERR_PART_GENERAL_UNKNOWN_OP;

	/* look up base name in table, if not found, the partition does not
	   support active mechanism, generally this table is pretty short so
	   performance penalty should be low */
	while (name_compare("NULL", iter->base_name)) {
		if (!name_compare(part_name, iter->base_name)) {
			found = 1;

			part = part_get(iter->name1);
			if (NULL == part)
				return ERR_PART_GENERAL_NOT_EXIST;

			active_1 = mt_part_get_active_bit(part);
			if (GET_ACTIVE == op) {
				if (active_1) {
					choice = 1;
					break;
				}
			} else if (GET_INACTIVE == op) {
				if (!active_1) {
					choice = 1;
					break;
				}
			}

			part = part_get(iter->name2);
			if (NULL == part)
				//return ERR_PART_GENERAL_NOT_EXIST;
				break;
			active_2 = mt_part_get_active_bit(part);
			if (GET_ACTIVE == op) {
				if (active_2) {
					choice = 2;
					break;
				}
			} else if (GET_INACTIVE == op) {
				if (!active_2) {
					choice = 2;
					break;
				}
			}

			break;
		}
		iter++;
	}

	/* if this partition does not exist in base name in table, then
	 * it does not support active bit mechanism so we keep its name
	 * as is.
	 */
	if (0 == found)
		return ret;

	/* active bit is not set before OTA. In this case, we boot
	 * with set 1.
	 */
	if (0 == choice || (active_1 == 0 && active_2 == 0)) {
		if (GET_ACTIVE == op) {
			choice = 1;
		} else if (GET_INACTIVE == op) {
			choice = 2;
		}
	}

	if (1 == choice) {
		uint32_t output_name_sz = strlen(iter->name1);
		if (output_name_sz > (part_name_buf_sz - 1))
			return ERR_PART_GENERAL_INVALID_NAME_SZ;

		memcpy(part_name, iter->name1, output_name_sz);
		*(part_name + output_name_sz) = 0;
	} else if (2 == choice) {
		uint32_t output_name_sz = strlen(iter->name2);
		if (output_name_sz > (part_name_buf_sz - 1))
			return ERR_PART_GENERAL_INVALID_NAME_SZ;

		memcpy(part_name, iter->name2, output_name_sz);
		*(part_name + output_name_sz) = 0;
	}

	return ret;
}

uint32_t partition_get_ab(char *part_name, uint32_t part_name_buf_sz,
			  uint32_t op)
{
	uint32_t ret = STATUS_OK;
	const char *ab_suffix = get_suffix();
	uint32_t part_name_sz = 0;
	uint32_t part_exist = 0;

	if (NULL == part_name)
		return ERR_PART_GENERAL_INVALID_NAME;

	if (op != GET_ACTIVE && op != GET_INACTIVE)
		return ERR_PART_GENERAL_UNKNOWN_OP;

	part_exist = partition_exists(part_name);

	/* use original name */
	if (1 == part_exist)
		return ret;

	/* something wrong with partition_exists api, return error code */
	if (-1 == part_exist)
		return get_last_error();

	if (ab_suffix == NULL)
		return ERR_PART_GENERAL_INVALID_SLOT;

	part_name_sz = strlen(part_name);
	if (part_name_sz > (part_name_buf_sz - 1 - AB_TAG_SZ))
		return ERR_PART_GENERAL_INVALID_NAME_SZ;

	if (!memcmp(ab_suffix, BOOTCTRL_SUFFIX_A, AB_TAG_SZ))
		memcpy(part_name + part_name_sz,
		       GET_ACTIVE == op ? BOOTCTRL_SUFFIX_A : BOOTCTRL_SUFFIX_B, AB_TAG_SZ);
	else
		memcpy(part_name + part_name_sz,
		       GET_ACTIVE == op ? BOOTCTRL_SUFFIX_B : BOOTCTRL_SUFFIX_A, AB_TAG_SZ);
	*(part_name + part_name_sz + AB_TAG_SZ) = 0;

	return ret;
}

uint32_t partition_get_active(char *part_name, uint32_t part_name_buf_sz,
			      uint32_t op)
{
#if defined(MTK_AB_OTA_UPDATER)
	return partition_get_ab(part_name, part_name_buf_sz, op);
#else
	return partition_get_gpt_active(part_name, part_name_buf_sz, op);
#endif
}

#ifdef MTK_PARTITION_COMMON
extern int read_gpt(part_t *part);

static int (*read_partition[])(part_t *) = {
    read_gpt,
    NULL
};

int is_partition_init(void)
{
    return g_part_ready;
}

int part_init(void)
{
    int err, i;
    part_num = 0;
    memset(part_info, 0x00, sizeof(part_info));

    memset(partition_info, 0x0, sizeof(partition_info));
    memset(meta_info, 0x0, sizeof(meta_info));

    err = 0;
    i = 0;
    g_part_ready = 1; // ready means gpt structure is read to dram
    while (read_partition[i]) {
        err = read_partition[i](partition_info);
        if (!err)
            break;
        i++;
    }

    return 0;
}


static part_t tempart;
static struct part_meta_info temmeta;

part_t *get_part(char *name)
{
    part_t *part;
    part_t *ret = NULL;
    if (g_part_ready == 1) {
        part = partition_info;
        while (part->nr_sects) {
            if (part->info) {
                if (!strcmp(name, part->info->name)) {
                    memcpy(&tempart, part, sizeof(part_t));
                    memcpy(&temmeta, part->info, sizeof(struct part_meta_info));
                    tempart.info = &temmeta;
                    ret = &tempart;
                    break;
                }
            }
            part++;
        }
    } else {
#ifdef EARLY_PARTITION_ACCESS
        part = mt_get_part_sram(name);
        if (part != NULL) {
            memcpy(&tempart, part, sizeof(part_t));
            ret = &tempart;
        }
#else
        pal_log_err("[%s] cannot get_part before dram init\n", __func__);
#endif
    }

    return ret;
}

part_t *part_get(char *name)
{
    /* this api corrupts if length of name is > 32 */
    memcpy(g_cur_part_name, name, 32);
    return get_part(name);
}

u8 *partition_getCurPartName(void)
{
    return g_cur_part_name;
}

/* retun > 0 if active bit exists, return = 0 if active bit is not existed */
unsigned long mt_part_get_active_bit(part_t *part)
{
    return (part->part_attr & PART_ATTR_LEGACY_BIOS_BOOTABLE);
}

#ifdef MTK_PART_DUMP_DISABLE
void part_dump(void)
{
}
#else
void part_dump(void)
{
    blkdev_t *bdev;
    part_t *part;
    u32 blksz;
    u64 start, end;

    bdev = blkdev_get(CFG_BOOT_DEV);
    if (!bdev)
        return;

    part = partition_info;
    blksz = bdev->blksz;

    pal_log_debug("\n[%s] blksz: %dB\n", __func__, blksz);
    while (part->nr_sects) {
        start = (u64)part->start_sect * blksz;
        end = (u64)(part->start_sect + part->nr_sects) * blksz - 1;
        pal_log_debug("[%s] [0x%llx-0x%llx] \"%s\" (%d blocks) \n", __func__, start, end,
            (part->info) ? (char *)part->info->name : "unknown", part->nr_sects);
        part++;
    }
}
#endif // MTK_PART_DUMP_DISABLE
#endif // MTK_PARTITION_COMMON
