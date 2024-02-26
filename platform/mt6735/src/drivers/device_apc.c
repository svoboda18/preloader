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

 //#define DEV_APC_DEBUG
 
/*=======================================================================*/
/* HEADER FILES                                                          */
/*=======================================================================*/
#include "platform.h"
#include "device_apc.h"

#define _DEBUG_
#define DBG_PRELOADER

/* Debug message event */
#define DBG_EVT_NONE        0x00000000      /* No event */
#define DBG_EVT_ERR         0x00000001      /* ERR related event */
#define DBG_EVT_DOM         0x00000002      /* DOM related event */

#define DBG_EVT_ALL         0xffffffff

#define DBG_EVT_MASK       (DBG_EVT_DOM )

#ifdef _DEBUG_
#define MSG(evt, fmt, args...) \
    do {    \
    if ((DBG_EVT_##evt) & DBG_EVT_MASK) { \
    printf(fmt, ##args); \
    } \
    } while(0)

#define MSG_FUNC_ENTRY(f)   MSG(FUC, "<FUN_ENT>: %s\n", __FUNCTION__)
#else
#define MSG(evt, fmt, args...) do{}while(0)
#define MSG_FUNC_ENTRY(f)      do{}while(0)
#endif

void tz_dapc_set_master_transaction(unsigned int master_index, E_TRANSACTION permisssion_control)
{
    reg_set_field(DEVAPC0_MAS_SEC, (0x1 << master_index), permisssion_control);
}

void tz_dapc_sec_init(void)
{
    tz_dapc_set_master_transaction(MASTER_MSDC0, SECURE_TRAN);

#if defined(CFG_TRUSTONIC_TEE_SUPPORT)
    tz_dapc_set_master_transaction(MASTER_SPI0, SECURE_TRAN);
#endif
}

void tz_dapc_sec_postinit(void)
{
    tz_dapc_set_master_transaction(MASTER_MSDC0, NON_SECURE_TRAN);

    /* DO NOT SET SPI0 back to Non-secure transaction. It should always keep in secure. */
}

void device_APC_dom_setup(void)
{
    MSG(DOM, "\nDevice APC domain init setup:\n\n");
    reg_write32(DEVAPC0_APC_CON, 0x0);
#ifdef DBG_PRELOADER
    MSG(DOM, "Domain Setup (0x%x)\n", reg_read32(DEVAPC0_MAS_DOM_0));
    MSG(DOM, "Domain Setup (0x%x)\n", reg_read32(DEVAPC0_MAS_DOM_1));
    MSG(DOM, "Domain Setup (0x%x)\n", reg_read32(DEVAPC0_MAS_DOM_2));
#endif


#if defined(MACH_TYPE_MT6735) || defined(MACH_TYPE_MT6737T)

    /*Set modem0 master to DOMAIN1*/
    reg_set_field(DEVAPC0_MAS_DOM_1 , MD1_DOMAIN, DOMAIN_1);
    /*Set connsys master to DOMAIN2*/
    reg_set_field(DEVAPC0_MAS_DOM_1 , CONN2AP,    DOMAIN_2);
    /*Set modem1 master to DOMAIN5*/
    reg_set_field(DEVAPC0_MAS_DOM_2 , MD3_DOMAIN, DOMAIN_5);
    /*Set gpu master to DOMAIN6*/
    reg_set_field(DEVAPC0_MAS_DOM_2 , GPU,        DOMAIN_6);

#elif defined(MACH_TYPE_MT6735M) || defined(MACH_TYPE_MT6737M)

    // Notice: Compare to Denali-2: (1)Denali-2 does not have Master "MD3"  (2)Denali-2 does not have Domain 4, 5, 6, 7.

    /*Set modem0 master to DOMAIN1*/
    reg_set_field(DEVAPC0_MAS_DOM_0 , MD1_DOMAIN, DOMAIN_1);
    /*Set connsys master to DOMAIN2*/
    reg_set_field(DEVAPC0_MAS_DOM_0 , CONN2AP,    DOMAIN_2);

#elif defined(MACH_TYPE_MT6753)

    /*Set modem0 master to DOMAIN1*/
    reg_set_field(DEVAPC0_MAS_DOM_1 , MD1_DOMAIN, DOMAIN_1);
    /*Set connsys master to DOMAIN2*/
    reg_set_field(DEVAPC0_MAS_DOM_1 , CONN2AP,    DOMAIN_2);
    /*Set modem1 master to DOMAIN5*/
    reg_set_field(DEVAPC0_MAS_DOM_2 , MD3_DOMAIN, DOMAIN_5);
    /*Set gpu master to DOMAIN6*/
    reg_set_field(DEVAPC0_MAS_DOM_2 , GPU,        DOMAIN_6);

#else

#error "Wrong MACH type"
	
#endif


#ifdef DBG_PRELOADER
    MSG(DOM, "Device APC domain after setup:\n");
    MSG(DOM, "Domain Setup (0x%x)\n", reg_read32(DEVAPC0_MAS_DOM_0));
    MSG(DOM, "Domain Setup (0x%x)\n", reg_read32(DEVAPC0_MAS_DOM_1));
    MSG(DOM, "Domain Setup (0x%x)\n", reg_read32(DEVAPC0_MAS_DOM_2));
#endif
}
