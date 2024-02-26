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

#ifndef __PARTITION_H_API__
#define __PARTITION_H_API__

#include <pal_typedefs.h>
#include "partition.h"

/* definitions */
#define AB_SYSTEM_DISABLE     (0)
#define AB_SYSTEM_A           (1)
#define AB_SYSTEM_B           (2)

#define BACKUP_SYSTEM_DISABLE (0)
#define BACKUP_SYSTEM_ACTIVE1 (1)
#define BACKUP_SYSTEM_ACTIVE2 (2)

#define PART_NAME_BUF_SZ      (64)

/* macros */
/* empty */

/* global variables */
/* empty */

/* functions */
uint32_t partition_exists(char *part_name);
ssize_t partition_read(char *part_name, uint64_t offset,
		       uint8_t *buf, uint32_t read_sz);
ssize_t partition_write(char *part_name, uint64_t offset,
			uint8_t *buf, uint32_t read_sz);
ssize_t partition_read_inactive(char *part_name, uint64_t offset,
				uint8_t *buf, uint32_t read_sz);
ssize_t partition_write_inactive(char *part_name, uint64_t offset,
				 uint8_t *buf, uint32_t read_sz);

/* the following functions should be used by partition api only */
uint32_t partition_get_gpt_active(char *part_name, uint32_t part_name_buf_sz,
				  uint32_t op);
uint32_t partition_get_ab(char *part_name, uint32_t part_name_buf_sz,
			  uint32_t op);
uint32_t partition_get_active(char *part_name, uint32_t part_name_buf_sz,
			      uint32_t op);

int is_partition_init(void);
int part_init(void);
part_t *part_get(char *name);
void part_dump(void);
part_t *get_part(char *name);
unsigned long mt_part_get_active_bit(part_t *part);
unsigned long mt_part_get_start_addr(part_t *part);
unsigned long mt_part_get_size(part_t *part);
unsigned int mt_part_get_part_id(part_t *part);
int mt_get_part_info_by_name(const char *name, struct part_info_t *part_info);
part_t *mt_get_part_sram(const char *name);

#endif /* __PARTITION_H_API__ */

