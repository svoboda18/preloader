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

#include "platform.h"
#include "ram_console.h"

#if CFG_FUNCTION_TRACER_SUPPORT

static U32 cur_ptr = 0;
static U32 is_disable_trace = 0;

extern bool b_is_ram_console_ready;
extern struct ram_console_buffer *ram_console;
extern struct reboot_reason_pl *rr_pl;

extern U32 pmic_config_interface (U32 RegNum, U32 val, U32 MASK, U32 SHIFT);
extern U32 pmic_read_interface (U32 RegNum, U32 *val, U32 MASK, U32 SHIFT);

void __cyg_profile_func_enter (void *this_fn, void *call_site)
    __attribute__ ((no_instrument_function));
void __cyg_profile_func_exit (void *this_fn, void *call_site)
    __attribute__ ((no_instrument_function));

void
__cyg_profile_func_enter (void *this_fn, void *call_site)
{
    if((this_fn == &print) || (this_fn == &pmic_config_interface) || (this_fn == &pmic_read_interface) || (this_fn == &platform_error_handler))
		is_disable_trace += 1;
	
	if(!is_disable_trace && b_is_ram_console_ready) {
        if (ram_console) {
            rr_pl->last_func[cur_ptr%RAM_CONSOLE_PL_SIZE] = this_fn;
            rr_pl->last_func[RAM_CONSOLE_PL_SIZE-1] = cur_ptr%RAM_CONSOLE_PL_SIZE;
			cur_ptr++;
        }
    }
}

void
__cyg_profile_func_exit (void *this_fn, void *call_site)
{
    if((this_fn == &print) || (this_fn == &pmic_config_interface) || (this_fn == &pmic_read_interface) || (this_fn == &platform_error_handler)) {
        if(is_disable_trace)
            is_disable_trace -= 1;
    }
}
#endif
