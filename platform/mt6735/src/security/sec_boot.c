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

/* for storage operations */
#include "sec_platform.h"

/* for crypto operations */
#include "cust_sec_ctrl.h"
#include "sec_boot.h"
#include "sec_error.h"
#include "sec.h"
#include "KEY_IMAGE_AUTH.h"
#include "sec_secroimg.h"
#if defined(MTK_AB_OTA_UPDATER)
#include "bootctrl.h"
#endif

/* to get current boot device type */
//#include "cust.h"

/**************************************************************************
*  MACRO
**************************************************************************/
#define MOD                             "SBC"

/******************************************************************************
 * DEBUG
 ******************************************************************************/
#define SEC_DEBUG                       (FALSE)
#define SMSG                            printf
#if SEC_DEBUG
#define DMSG                            printf
#else
#define DMSG
#endif

/**************************************************************************
 *  GLOBAL VARIABLES
 **************************************************************************/
u8 g_oemkey[OEM_PUBK_SZ] = {OEM_PUBK};

/**************************************************************************
 *  LOCAL VARIABLES
 **************************************************************************/

/**************************************************************************
 *  EXTERNAL VARIABLES
 **************************************************************************/
extern BOOL bDumpPartInfo;
extern AND_SECROIMG_T               *secroimg;
extern unsigned int                 g_sec_cfg_exists;

/**************************************************************************
 *  EXTERNAL FUNCTIONS
 **************************************************************************/

/**************************************************************************
 *  [SECURE BOOT CHECK]
 **************************************************************************/
U32 sec_boot_img_check(BOOL sw_sboot_support, unsigned int boot_chk_img, U8 *img_name)
{
    bDumpPartInfo = TRUE;
    U32 ret = SEC_OK;

    if (TRUE == sw_sboot_support) {
        if (SW_SEC_BOOT_CHECK_IMAGE == boot_chk_img) {
            if (SEC_OK != (ret = seclib_image_check(img_name, TRUE)))
                return ret;
        }
    } else {
        if (SEC_OK != (ret = seclib_image_check(img_name, TRUE)))
            return ret;
    }

    return ret;
}



void sec_boot_check(void)
{
    char *lk_part_name = NULL;
#ifdef MTK_SECURITY_SW_SUPPORT
#if defined(MTK_AB_OTA_UPDATER)
    const char *ab_suffix = get_suffix();

    if (!memcmp(ab_suffix, BOOTCTRL_SUFFIX_A, 2))
        lk_part_name = SBOOT_PART_UBOOT_A;
    else
        lk_part_name = SBOOT_PART_UBOOT_B;
#else
    lk_part_name = SBOOT_PART_UBOOT;
#endif // MTK_AB_OTA_UPDATER

    U32 g_total_verify_time = get_timer(0);
    U32 g_single_verify_time = 0;
    U32 ret = 0;
    BOOL sw_secure_boot_support = FALSE;

    if (FALSE == seclib_sec_boot_enabled(TRUE)) {
        SMSG("[%s] Don't check\n", MOD);
        return ;
    }

    if (0 != seclib_set_pubk(g_oemkey, OEM_PUBK_SZ)) {
        SMSG("[%s] public key initialization failed, cannot do secure boot!\n", MOD);
        ASSERT(0);
    }

#define SW_SECURITY_DISABLE
/*from Android P, sw security is not supported any longer*/
/*so preloader should not verify and read secro*/
#ifndef SW_SECURITY_DISABLE
    /* verification of secro image is enforced */
    /* secro must be verified first before using its content */
    g_single_verify_time = get_timer(0);
    if (SEC_OK != seclib_image_check(SBOOT_PART_SECSTATIC, TRUE))
        goto fail;
    SMSG("[%s] Consume (%d) ms\n", MOD, get_timer(g_single_verify_time));

    sec_lib_read_secro();
    if ((secroimg != NULL) && (SW_SUPPORT_ENABLE ==
                               secroimg->m_andro.sw_sec_boot.flashtool_unlock_support))
        sw_secure_boot_support = TRUE;

    /* verify 2nd bootloader */
    g_single_verify_time = get_timer(0);
    if (SEC_OK != sec_boot_img_check(sw_secure_boot_support,
                                     secroimg->m_andro.sw_sec_boot.boot_chk_2nd_loader,
                                     lk_part_name))
#else
    g_single_verify_time = get_timer(0);
    if (SEC_OK != sec_boot_img_check(sw_secure_boot_support,
                                     1,//force assign 1 for secroimg is not valid
                                     lk_part_name))
#endif
        goto fail;
    SMSG("[%s] Consume (%d) ms\n", MOD, get_timer(g_single_verify_time));

#if VERIFY_PART_CUST
    /* verify partition added by odm */
    g_single_verify_time = get_timer(0);;
    if (SEC_OK != sec_boot_img_check(TRUE, SW_SEC_BOOT_CHECK_IMAGE, VERIFY_PART_CUST_NAME))
        goto fail;
    SMSG("[%s] Consume (%d) ms\n", MOD, get_timer(g_single_verify_time));
#endif

    seclib_cfg_print_status();

    SMSG("\n[%s] Total Consume (%d) ms\n", MOD, get_timer(g_total_verify_time));
    return;

fail:
    SMSG("[%s] Fail (0x%x)\n", MOD, ret);
    ASSERT(0);

#endif // MTK_SECURITY_SW_SUPPORT
    return;
}
