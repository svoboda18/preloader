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
#ifndef __PMIC_WRAP_SSPM_REGS_HMAC__
#define __PMIC_WRAP_SSPM_REGS_HMAC__

#define GET_WACS_SSPM_WDATA(x)       ((x>>0)  & 0x0000ffff)
#define GET_WACS_SSPM_ADR(x)         ((x>>16) & 0x00007fff)
#define GET_WACS_SSPM_WRITE(x)       ((x>>31) & 0x00000001)
#define GET_WACS_SSPM_RDATA(x)       ((x>>0)  & 0x0000ffff)
#define GET_WACS_SSPM_FSM(x)         ((x>>16) & 0x00000007)
#define GET_WACS_SSPM_REQ(x)         ((x>>19) & 0x00000001)
#define GET_WACS_SYNC_IDLE_SSPM(x)   ((x>>20) & 0x00000001)
#define GET_WACS_SSPM_EN(x)          ((x>>21) & 0x00000001)
#define GET_WACS_INIT_DONE_SSPM(x)   ((x>>22) & 0x00000001)
#define GET_SYS_IDLE_SSPM(x)         ((x>>23) & 0x00000001)
#define GET_WACS_SSPM_FIFO_FILLCNT(x) ((x>>24) & 0x0000000f)
#define GET_WACS_SSPM_FIFO_FREECNT(x) ((x>>28) & 0x0000000f)
#define GET_WACS_SSPM_VLDCLR(x)      ((x>>0)  & 0x00000001)
#define GET_PWRAP_PWRMCU_RDATA(x)    ((x>>0)  & 0x0000ffff)

#endif //__PMIC_WRAP_SSPM_REGS_HMAC__

