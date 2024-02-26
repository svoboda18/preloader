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
 * MediaTek Inc. (C) 2017. All rights reserved.
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

#include "sec_platform.h"

/* import rom_info variable */
#include "sec_rom_info.h"

/* import partition table */
//#include "partition_define.h"

/* import custom info */
#include "cust_bldr.h"
#include "cust_sec_ctrl.h"
//MingTsang #include "KEY_SML_AUTH.h"
//MingTsang #include "KEY_SML_ENCODE.h"
#include "PL_CFG.h"
#include "pal_log.h"

/**************************************************************************
*  MACRO
**************************************************************************/
#define MOD                             "ROM_INFO"

#define yes                             (1)
#define no                              (0)

#define SMSG(f, ...)                    pal_log_info(f, ##__VA_ARGS__)

/**************************************************************************
 *  GLOBAL VARIABLES
 **************************************************************************/

/**************************************************************************
 *  LOCAL VARIABLES
 **************************************************************************/

/**************************************************************************
 *  EXTERNAL VARIABLES
 **************************************************************************/

/**************************************************************************
 *  EXTERNAL FUNCTIONS
 **************************************************************************/

/**************************************************************************
 *  DO NOT MODIFY THIS !
 **************************************************************************/
__attribute__((section(".data.rom_info")))
AND_ROMINFO_T  g_ROM_INFO =
{
    /* ================================================================ */
    /* ROM_INFO                                                         */
    /* ================================================================ */

                                                            /* MTK */
    .m_identifier                       = ROM_INFO_NAME

                                                            /* MTK */
    ,.m_rom_info_ver                    = PRELOADER_SEC_VERSION

                                                            /* CUSTOMER */
    ,.m_platform_id                     = PLATFORM_NAME

                                                            /* CUSTOMER */
    ,.m_project_id                      = PROJECT_NAME


    /* ================================================================ */
    /* AND_SECCTRL_T                                                    */
    /* ================================================================ */

                                                            /* MTK */
    ,.m_SEC_CTRL.m_identifier           = ROM_INFO_SEC_CTRL_ID

                                                            /* MTK */
    ,.m_SEC_CTRL.m_sec_cfg_ver          = ROM_INFO_SEC_CTRL_VER

                                                            /* CUSTOMER */
    ,.m_SEC_CTRL.m_sec_usb_dl           = SEC_USBDL_CFG

                                                            /* CUSTOMER */
    ,.m_SEC_CTRL.m_sec_boot             = SEC_BOOT_CFG

                                                            /* CUSTOMER */
#ifdef SECCFG_ANTICLONE_DIS
    ,.m_SEC_CTRL.m_seccfg_ac_en         = 0x0
#else
    ,.m_SEC_CTRL.m_seccfg_ac_en         = 0x1
#endif

    ,.m_SEC_CTRL.reserve                = {0x0}

    /* ================================================================ */
    /* AND_SECKEY_T                                                     */
    /* ================================================================ */

                                                            /* MTK */
    ,.m_SEC_KEY.m_identifier            = ROM_INFO_SEC_KEY_ID

                                                            /* MTK */
    ,.m_SEC_KEY.m_sec_key_ver           = ROM_INFO_SEC_KEY_VER

    ,.m_SEC_KEY.crypto_seed             = CUSTOM_CRYPTO_SEED


};

void sec_rom_info_init (void)
{
    COMPILE_ASSERT(AND_ROM_INFO_SIZE == sizeof(AND_ROMINFO_T));
    SMSG("[%s] 'v%d'\n", MOD, g_ROM_INFO.m_rom_info_ver);
}
