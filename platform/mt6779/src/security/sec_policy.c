/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2018. All rights reserved.
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
#include "sec_policy.h"
#include "lock_state.h"
#include "pal_log.h"
#include <string.h>
#include "verified_boot.h"

unsigned char g_binding_hash[SHA256_HASH_SZ] = {0};

struct policy_part_map g_policy_map[] = {
	{0,                 "default",     0, 0, 0, DEFAULT_SEC_POLICY_1, DEFAULT_SEC_POLICY_2, DEFAULT_SEC_POLICY_3, DEFAULT_SEC_POLICY_4, 0},
	{PL_SW_ID,          "preloader",   0, 0, 0, PL_SEC_POLICY_1, PL_SEC_POLICY_2, PL_SEC_POLICY_3, PL_SEC_POLICY_4, PL_BINDING_HASH},
	{LK_SW_ID,          "lk",          0, 0, 0, LK_SEC_POLICY_1, LK_SEC_POLICY_2, LK_SEC_POLICY_3, LK_SEC_POLICY_4, LK_BINDING_HASH},
	{LOGO_SW_ID,        "logo",        0, 0, 0, LOGO_SEC_POLICY_1, LOGO_SEC_POLICY_2, LOGO_SEC_POLICY_3, LOGO_SEC_POLICY_4, LOGO_BINDING_HASH},
	{BOOT_SW_ID,        "boot",        0, 0, 0, BOOT_SEC_POLICY_1, BOOT_SEC_POLICY_2, BOOT_SEC_POLICY_3, BOOT_SEC_POLICY_4, BOOT_BINDING_HASH},
	{SYSTEM_SW_ID,      "system",      0, 0, 0, SYSTEM_SEC_POLICY_1, SYSTEM_SEC_POLICY_2, SYSTEM_SEC_POLICY_3, SYSTEM_SEC_POLICY_4, SYSTEM_BINDING_HASH},
	{TEE1_SW_ID,        "tee1",        0, 0, 0, TEE1_SEC_POLICY_1, TEE1_SEC_POLICY_2, TEE1_SEC_POLICY_3, TEE1_SEC_POLICY_4, TEE1_BINDING_HASH},
	{TEE2_SW_ID,        "tee2",        0, 0, 0, TEE2_SEC_POLICY_1, TEE2_SEC_POLICY_2, TEE2_SEC_POLICY_3, TEE2_SEC_POLICY_4, TEE2_BINDING_HASH},
	{OEMKEYSTORE_SW_ID, "oemkeystore", 0, 0, 0, OEMKEYSTORE_SEC_POLICY_1, OEMKEYSTORE_SEC_POLICY_2, OEMKEYSTORE_SEC_POLICY_3, OEMKEYSTORE_SEC_POLICY_4, OEMKEYSTORE_BINDING_HASH},
	{KEYSTORE_SW_ID,    "keystore",    0, 0, 0, KEYSTORE_SEC_POLICY_1, KEYSTORE_SEC_POLICY_2, KEYSTORE_SEC_POLICY_3, KEYSTORE_SEC_POLICY_4, KEYSTORE_BINDING_HASH},
	{USERDATA_SW_ID,    "userdata",    0, 0, 0, USERDATA_SEC_POLICY_1, USERDATA_SEC_POLICY_2, USERDATA_SEC_POLICY_3, USERDATA_SEC_POLICY_4, USERDATA_BINDING_HASH},
	{MD1IMG_SW_ID,      "md1img",      0, 0, 0, MD1IMG_SEC_POLICY_1, MD1IMG_SEC_POLICY_2, MD1IMG_SEC_POLICY_3, MD1IMG_SEC_POLICY_4, MD1IMG_BINDING_HASH},
	{MD1DSP_SW_ID,      "md1dsp",      0, 0, 0, MD1DSP_SEC_POLICY_1, MD1DSP_SEC_POLICY_2, MD1DSP_SEC_POLICY_3, MD1DSP_SEC_POLICY_4, MD1DSP_BINDING_HASH},
	{MD1ARM7_SW_ID,     "md1arm7",     0, 0, 0, MD1ARM7_SEC_POLICY_1, MD1ARM7_SEC_POLICY_2, MD1ARM7_SEC_POLICY_3, MD1ARM7_SEC_POLICY_4, MD1ARM7_BINDING_HASH},
	{MD3IMG_SW_ID,      "md3img",      0, 0, 0, MD3IMG_SEC_POLICY_1, MD3IMG_SEC_POLICY_2, MD3IMG_SEC_POLICY_3, MD3IMG_SEC_POLICY_4, MD3IMG_BINDING_HASH},

	/*Custom img*/
	{0, CUST1_IMG_NAME, 0, 0, 0, CUST1_SEC_POLICY_1, CUST1_SEC_POLICY_2, CUST1_SEC_POLICY_3, CUST1_SEC_POLICY_4 , CUST1_BINDING_HASH},
	{0, CUST2_IMG_NAME, 0, 0, 0, CUST2_SEC_POLICY_1, CUST2_SEC_POLICY_2, CUST2_SEC_POLICY_3, CUST2_SEC_POLICY_4 , CUST2_BINDING_HASH},
	/*End of array*/
	{0, "NULL", "NULL", "NULL", "NULL", 0 , 0, 0, 0, 0},
};

uint32_t get_policy_entry_idx(char *part_name)
{
	uint32_t i = 0;
    uint32_t num_of_entries = sizeof(g_policy_map)/sizeof(struct policy_part_map);

	while (i < num_of_entries) {
        if (0 == strcmp(part_name, g_policy_map[i].part_name1))
			break;
		else if (0 == strcmp(part_name, g_policy_map[i].part_name2))
			break;
		else if (0 == strcmp(part_name, g_policy_map[i].part_name3))
			break;
		else if (0 == strcmp(part_name, g_policy_map[i].part_name4))
			break;
		i++;
	}

    if (i == num_of_entries) {
        pal_log_info("[SEC_POLICY] reached the end, use default policy\n");
        i = 0;
    }

	return i;
}

static uint32_t get_default_lock_state(uint32_t *lock_state)
{
    uint32_t ret = 0;

    *lock_state = LKS_LOCK;

    return ret;
}

static uint32_t get_sec_state(uint32_t *sboot_state, uint32_t *lock_state)
{
    uint32_t ret = 0;
    uint32_t default_lock_state = 0;

    ret = get_sboot_state(sboot_state);
    if (ret)
        *sboot_state = 1;

    ret = get_lock_state(lock_state);
    if (ret)
        *lock_state = LKS_DEFAULT;

    if ((LKS_DEFAULT == *lock_state) || (LKS_MP_DEFAULT == *lock_state)) {
        ret = get_default_lock_state(&default_lock_state);
        if (ret)
            *lock_state = LKS_LOCK;
        *lock_state = default_lock_state;
    } else if ((LKS_UNLOCK != *lock_state) && (LKS_LOCK != *lock_state))
        *lock_state = LKS_LOCK;

    pal_log_info("[SEC_POLICY] sboot_state = 0x%x\n", *sboot_state);
    if (!default_lock_state)
        pal_log_info("[SEC_POLICY] lock_state = 0x%x\n", *lock_state);
    else
        pal_log_info("[SEC_POLICY] lock_state(default) = 0x%x\n", *lock_state);

    return 0;
}

static unsigned char get_sec_policy(uint32_t policy_entry_idx)
{
    uint32_t sboot_state = 0;
    uint32_t lock_state = 0;

    unsigned char sec_policy = 0;
    uint32_t ret = 0;

    ret = get_sec_state(&sboot_state, &lock_state);
    if (ret) {
        pal_log_err("fail to get sec state.\n");
        return 0xFF;
    }

	if (sboot_state == 0 && lock_state == LKS_UNLOCK)
		sec_policy = g_policy_map[policy_entry_idx].sec_sbcdis_unlock_policy;
	else if (sboot_state == 0 && lock_state != LKS_UNLOCK)
		sec_policy = g_policy_map[policy_entry_idx].sec_sbcdis_lock_policy;
	else if (sboot_state == 1 && lock_state == LKS_UNLOCK)
		sec_policy = g_policy_map[policy_entry_idx].sec_sbcen_unlock_policy;
	else if (sboot_state == 1 && lock_state != LKS_UNLOCK)
		sec_policy = g_policy_map[policy_entry_idx].sec_sbcen_lock_policy;

	return sec_policy;
}

static unsigned char chr_to_hex(unsigned char input)
{
    unsigned char output = 0;
    if (input >= '0' && input <= '9')
        output = input - '0' + 0x0;
    if (input >= 'A' && input <= 'F')
        output = input - 'A' + 0xa;
    if (input >= 'a' && input <= 'f')
        output = input - 'a' + 0xa;

    return output;
}

unsigned char *get_binding_hash(uint32_t policy_entry_idx)
{
    uint32_t i = 0;

    if (!g_policy_map[policy_entry_idx].hash)
        return 0;

    for (i = 0; i < SHA256_HASH_SZ; i++) {
        unsigned char chr0, chr1 = 0;
        chr0 = g_policy_map[policy_entry_idx].hash[i * 2];
        chr1 = g_policy_map[policy_entry_idx].hash[i * 2 + 1];
        g_binding_hash[i] = (chr_to_hex(chr0) << 4) | (chr_to_hex(chr1));
    }

    return g_binding_hash;
}

/* bypass hashbind: return 0, need hashbind: return 1 */
uint32_t get_hash_binding_policy(uint32_t policy_entry_idx)
{
  	unsigned char sec_policy = 0;
    sec_policy = get_sec_policy(policy_entry_idx);

	if (HASH_BIND_BIT_SET(sec_policy))
		/* need hash bind */
		return 1;
	else
		/* skip hash bind */
		return 0;
}

/* bypass auth check: return 0, need auth check: return 1 */
uint32_t get_vfy_policy(uint32_t policy_entry_idx)
{
  	unsigned char sec_policy = 0;
    sec_policy = get_sec_policy(policy_entry_idx);

	if (VFY_BIT_SET(sec_policy))
		/* need verify */
		return 1;
	else
		/* skip verify */
		return 0;
}

/* bypass dl check: return 0, need dl check: return 1 */
uint32_t get_dl_policy(uint32_t policy_entry_idx)
{
  	unsigned char sec_policy = 0;
    sec_policy = get_sec_policy(policy_entry_idx);

	if (DL_FORBIDDEN_BIT_SET(sec_policy))
		/* dl is forbidden */
		return 1;
	else
		/* dl is permitted */
		return 0;
}

uint32_t get_sw_id(uint32_t policy_entry_idx)
{
    return g_policy_map[policy_entry_idx].sw_id;
}

uint32_t is_all_partition_writable(uint32_t *write_all_perm)
{
    uint32_t ret = 0;
    uint32_t i = 0;
    uint32_t num_of_entries = sizeof(g_policy_map)/sizeof(struct policy_part_map);

    *write_all_perm = 1;

    for (i = 0; i < num_of_entries; i++) {
        /* if any partition in the table is not writeable, then write/format
        without specifying partion name is not allowed */
        if (get_dl_policy(i)) {
            *write_all_perm = 0;
            break;
        }
    }

    return ret;
}


