/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2019. All rights reserved.
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

#ifndef SEC_ROMINFO_H
#define SEC_ROMINFO_H

#include "sec_boot.h"
#include "sec_key.h"
#include "sec_ctrl.h"


/**************************************************************************
*  INIT VARIABLES
**************************************************************************/
#define ROM_INFO_NAME                       "AND_ROMINFO_v"
/* VER1 - only ROM INFO region is provided */
/* VER2 - SECRO image anti-clone feature is supported */
#define ROM_INFO_VER                        0x2
#define ROM_INFO_SEC_RO_EXIST               0x1
#define ROM_INFO_ANTI_CLONE_OFFSET          0x54
#define ROM_INFO_ANTI_CLONE_LENGTH          0xE0

/**************************************************************************
*  ANDRIOD ROM INFO FORMAT
**************************************************************************/
/* this structure should always sync with FlashLib
   becuase FlashLib will search storage to find ROM_INFO */
#define AND_ROM_INFO_SIZE                  (120)
typedef struct {

    unsigned char                   m_identifier[16];   /* MTK */
    unsigned int                    m_rom_info_ver;     /* MTK */
    unsigned char                   m_platform_id[16];  /* CUSTOMER */
    unsigned char                   m_project_id[16];

    AND_SECCTRL_T                   m_SEC_CTRL;         /* CUSTOMER :
                                                            secure feature
                                                            control */

    AND_SECKEY_T                    m_SEC_KEY;          /* CUSTOMER :
                                                            key */

} AND_ROMINFO_T;

#endif /* SEC_ROMINFO_H */

