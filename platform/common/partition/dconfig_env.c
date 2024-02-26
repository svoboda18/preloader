/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2015. All rights reserved.
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
 */

#include <pal_typedefs.h>
#include <pal_log.h>
#include <pal_assert.h>

#include <partition_api.h>
#include <partition_error.h>
#include <partition_active.h>
#include <storage_api.h>
#include <platform.h>
#include <dconfig_env.h>

#define MODULE_NAME "DOE_ENV"

#define DCONFIG_PART "boot_para"
#define DCONFIG_IMG_NAME "dconfig"
#define DCONFIG_IMG_SIZE 2048

#if MTK_DOE_CONFIG_ENV_SUPPORT

char g_dconfig_img[DCONFIG_IMG_SIZE];

static dconfig_env_t g_env;
static int g_env_valid = 0;
static int g_env_init_done = 0;
static char env_get_char(int index);
static char *env_get_addr(int index);
static int envmatch(uchar *s1, int i2);
static int read_env_area(void);

#ifndef min
#define min(x, y)   (x < y ? x : y)
#endif

void dconfig_initenv(void)
{
	int ret, i, checksum;
	char * const env_buffer = g_dconfig_img + CFG_ENV_OFFSET;

	if (g_env_init_done == 1) {
		pal_log_warn("[%s] env is already inited\n", MODULE_NAME);
		return;
	}
	checksum = 0;
	g_env_valid = 0;
	ret = read_env_area();
	if (ret < 0) {
		pal_log_err("[%s]read_env_area fail, ret = %d\n", MODULE_NAME, ret);
		g_env_init_done = 1;
		g_env_valid = 0;
		return;
	}

	g_env.env_data = env_buffer + CFG_ENV_DATA_OFFSET;
	memcpy(g_env.sig, env_buffer, sizeof(g_env.sig));
	memcpy(g_env.sig_1, env_buffer + CFG_ENV_SIG_1_OFFSET, sizeof(g_env.sig_1));

	if (!strcmp(g_env.sig, ENV_SIG) && !strcmp(g_env.sig_1, ENV_SIG)) {
		g_env.checksum = *((int *)env_buffer + CFG_ENV_CHECKSUM_OFFSET / 4);
		for (i = 0; i < (int)(CFG_ENV_DATA_SIZE); i++)
			checksum += g_env.env_data[i];
		if(checksum != g_env.checksum){
			pal_log_err("[%s]checksum mismatch s %d d %d!\n",MODULE_NAME, g_env.checksum,checksum);
			g_env_valid = 0;
		} else {
			pal_log_info("[%s]ENV initialize success\n", MODULE_NAME);
			g_env_valid = 1;
		}
	} else {
		pal_log_err("[%s]ENV SIG is wrong\n",MODULE_NAME);
		g_env_valid = 0;
	}
	if (!g_env_valid)
		memset(env_buffer, 0x00, CFG_ENV_SIZE);
	g_env_init_done = 1;
}

char *dconfig_getenv(char *name)
{
	int i, nxt;
	pal_log_info("[%s]get_env %s\n",MODULE_NAME,name);

	if (!g_env_valid)
		return NULL;

	for (i=0; env_get_char(i) != '\0'; i=nxt+1) {
		int val;

		for (nxt=i; env_get_char(nxt) != '\0'; ++nxt) {
			if (nxt >= CFG_ENV_SIZE) {
				return (NULL);
			}
		}
		if ((val=envmatch((uchar *)name, i)) < 0)
			continue;
		return ((char *)env_get_addr(val));
	}

	return (NULL);
}

static char env_get_char(int index)
{
	return *(g_env.env_data+index);
}

static char *env_get_addr(int index)
{
	return (g_env.env_data+index);

}
static int envmatch(uchar *s1, int i2)
{
	if (s1 == NULL)
		return -1;
	while (*s1 == env_get_char(i2++))
		if (*s1++ == '=')
			return(i2);
	if (*s1 == '\0' && env_get_char(i2-1) == '=')
		return(i2);
	return(-1);
}

void dconfig_printenv(void)
{
	int i,nxt,exit = 0;
	char *env;

	pal_log_info("[%s] print env data\n", MODULE_NAME);
	if (g_env_init_done == 0) {
		pal_log_info("[%s] no init yet\n", MODULE_NAME);
		return;
	}

	if (!g_env_valid) {
		pal_log_info("[%s]no valid env\n", MODULE_NAME);
		return;
	}

	env = g_env.env_data;
	for (i=0; env_get_char(i) != '\0'; i=nxt+1) {
		for (nxt=i; env_get_char(nxt) != '\0'; ++nxt) {
			if (nxt >= (int)(CFG_ENV_DATA_SIZE)) {
				exit = 1;
				break;
			}
		}
		if (exit == 1)
			break;
		pal_log_info("[%s]%s\n", MODULE_NAME, env+i);
	}
}

static int read_env_area(void)
{
	long len;
#ifdef MTK_SECURITY_SW_SUPPORT
	unsigned int sec_policy_idx = 0;
	unsigned int img_auth_required = 0;
#endif
	int size = CFG_ENV_SIZE;
	const part_hdr_t *part = (const part_hdr_t *)g_dconfig_img;

	memset(g_dconfig_img, 0x00, DCONFIG_IMG_SIZE);

	len = partition_read(ENV_PART, 0, (uchar*)g_dconfig_img, DCONFIG_IMG_SIZE);
	if ((len != DCONFIG_IMG_SIZE) || (part->info.magic != PART_MAGIC)) {
		pal_log_info("[%s] No doconfig setting\n", MODULE_NAME);
		goto failed;
	}

#ifdef MTK_SECURITY_SW_SUPPORT
	/* get security policy of current partition */
	sec_policy_idx = get_policy_entry_idx(DCONFIG_PART);
	img_auth_required = get_vfy_policy(sec_policy_idx);
	if (img_auth_required) {
		int ret = 0;
		sec_malloc_buf_reset();
		ret = sec_img_auth_init(DCONFIG_PART, DCONFIG_IMG_NAME);
		if (ret) {
			goto failed;
		}
		ret = sec_img_auth(g_dconfig_img + sizeof(part_hdr_t), part->info.dsize);
		if (ret) {
			goto failed;
		}
	}
#endif
	return 0;
failed:
	memset(g_dconfig_img, 0x00, DCONFIG_IMG_SIZE);
	return -1;
}

#else // MTK_DOE_CONFIG_ENV_SUPPORT

void dconfig_initenv(void) {
}

void dconfig_printenv(void) {
}

char *dconfig_getenv(char *name) {
	pal_log_err("[%s]dconfig is disabled\n", MODULE_NAME);
	return NULL;
}

#endif // MTK_DOE_CONFIG_ENV_SUPPORT
