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
#include <profiling.h>

#define PROFILING_DEFAULT_EN  (0)
#define MOD "PROFILING"

#if PROFILING_DEFAULT_EN
static uint32_t g_profiling_enable = 1;
#else
static uint32_t g_profiling_enable = 0;
#endif

void set_profiling_enable(uint32_t enable)
{
	g_profiling_enable = enable;
	return;
}

void profiling_reset(struct profile_info *ctx)
{
	uint32_t i;
	ctx->start_time = 0;
	ctx->running = 0;
	ctx->cur_time_slot_idx = 0;
	for (i = 0; i < NUM_TIME_SLOT; i++) {
		ctx->time[i].name = NULL;
		ctx->time[i].time = 0;
	}

	return;
}

void profiling_start(struct profile_info *ctx, char *prompt_str, char *time_slot_name)
{
	if (0 == g_profiling_enable)
		return;

	if (NULL == prompt_str)
		prompt_str = "NULL";

	pal_log_debug("[%s] %s start\n", MOD, prompt_str);

	if (NULL != time_slot_name && (ctx->cur_time_slot_idx < NUM_TIME_SLOT))
		ctx->time[ctx->cur_time_slot_idx].name = time_slot_name;

	ctx->running = 1;
	ctx->start_time = pal_get_timer(0);

	return;
}

void profiling_stop(struct profile_info *ctx, char *prompt_str, uint32_t advance_time_slot)
{
	if (0 == g_profiling_enable) {
		return;
	}

	if (0 == ctx->running)
		return;

	/* behavior similar to we stop then start timer immediately, but we don't stop timer actually */
	if (ctx->cur_time_slot_idx < NUM_TIME_SLOT) {
		ctx->time[ctx->cur_time_slot_idx].time += pal_get_timer(ctx->start_time);
		if (advance_time_slot)
			ctx->cur_time_slot_idx++;
	} else
		pal_log_err("profiling time slot depleted\n");

	if (NULL == prompt_str)
		prompt_str = "NULL";

	pal_log_debug("[%s] %s stop\n", MOD, prompt_str);

	ctx->running = 0;

	return;
}

void profiling_end(struct profile_info *ctx, char *prompt_str)
{
	uint32_t i = 0;
	uint32_t total_time = 0;

	if (0 == g_profiling_enable)
		return;

	profiling_stop(ctx, prompt_str, 0);

	if (NULL == prompt_str)
		prompt_str = "NULL";

	pal_log_debug("[%s] %s end\n", MOD, prompt_str);

	for (i = 0; i < NUM_TIME_SLOT; i++) {
		char *time_slot_name = NULL;
		if (0 == ctx->time[i].time)
			break;
		time_slot_name = ctx->time[i].name != NULL ? ctx->time[i].name : "NULL";
		pal_log_info("[%s][%s][%s][0x%x] time = %dms\n", MOD, prompt_str, time_slot_name, i, ctx->time[i].time);
		total_time += ctx->time[i].time;
	}
	pal_log_info("[%s][%s] total time = %dms\n", MOD, prompt_str, total_time);

	return;
}

