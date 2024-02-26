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

#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include "typedefs.h"
#include "platform.h"

#define CMD_GET_HW_SW_VER          0xfc
#define CMD_GET_HW_CODE            0xfd
#define CMD_GET_BL_VER             0xfe

#define CMD_LEGACY_WRITE           0xa1
#define CMD_LEGACY_READ            0xa2

#define CMD_I2C_INIT               0xB0
#define CMD_I2C_DEINIT             0xB1
#define CMD_I2C_WRITE8             0xB2
#define CMD_I2C_READ8              0xB3
#define CMD_I2C_SET_SPEED          0xB4

#define CMD_PWR_INIT               0xC4
#define CMD_PWR_DEINIT             0xC5
#define CMD_PWR_READ16             0xC6
#define CMD_PWR_WRITE16            0xC7

#define CMD_READ16                 0xD0
#define CMD_READ32                 0xD1
#define CMD_WRITE16                0xD2
#define CMD_WRITE16_NO_ECHO        0xD3
#define CMD_WRITE32                0xD4
#define CMD_JUMP_DA                0xD5
#define CMD_JUMP_BL                0xD6
#define CMD_SEND_DA                0xD7
#define CMD_GET_TARGET_CONFIG      0xD8
#define CMD_UART1_LOG_EN           0xDB

#define TGT_CFG_SBC_EN             0x00000001
#define TGT_CFG_SLA_EN             0x00000002
#define TGT_CFG_DAA_EN             0x00000004

#define DA_ARG_MAGIC               0x58885168
#define DA_ARG_VER                 1

#define DA_FLAG_SKIP_PLL_INIT      0x00000001
#define DA_FLAG_SKIP_EMI_INIT      0x00000002

/* error code */
#define READ_REGION_CHK_FAIL       (0x1000)
#define WRITE_REGION_CHK_FAIL      (0x1001)

typedef struct {
    u32 magic;
    u32 ver;
    u32 flags;
} DownloadArg;

#if CFG_USB_DOWNLOAD 
extern int usbdl_handler(struct bldr_comport *comport, u32 hshk_tmo_ms);
#endif

#endif /* DOWNLOAD_H */
