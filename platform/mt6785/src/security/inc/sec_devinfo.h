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

#ifndef DEVINFO_H
#define DEVINFO_H

/******************************************************************************
 * CONSTANT DEFINITIONS
 ******************************************************************************/
#define INCORRECT_INDEX          0xFFFFFFFF    /* incorrect register index */

/* Each platform may support different HRID sizes:
 *   HRID_SIZE: 2 => HRID index 12, 13
 *   HRID_SIZE: 4 => HRID index 12, 13, 14, 15  */
#define HRID_SIZE 4

/******************************************************************************
 * TYPE DEFINITIONS
******************************************************************************/
typedef enum
{
    E_AREA0,
    E_AREA1,
    E_AREA2,
    E_AREA3,
    E_AREA4,
    E_AREA5,
    E_AREA6,
    E_AREA7,
    E_AREA8,
    E_AREA9,
    E_AREA10,
    E_AREA11,
    E_AREA12,
    E_AREA13,
    E_AREA14,
    E_AREA15,
    E_AREA16,
    E_AREA17,
    E_AREA18,
    E_AREA19,
    E_AREA20,
    E_AREA21,
    E_AREA22,
    E_AREA23,
    E_AREA24,
    E_AREA25,
    E_AREA26,
    E_AREA27,
    E_AREA28,
    E_AREA29,
    E_AREA30,
    E_AREA31,
    E_AREA32,
    E_AREA33,
    E_AREA34,
    E_AREA35,
    E_AREA36,
    E_AREA37,
    E_AREA38,
    E_AREA39,
    E_AREA40,
    E_AREA41,
    E_AREA42,
    E_AREA43,
    E_AREA_MAX
} E_INDEX;


/******************************************************************************
 * EXPORT FUNCTION
 ******************************************************************************/
extern u32 seclib_get_devinfo_with_index(u32 index);
extern u32 internal_seclib_get_devinfo_with_index(u32 index);
extern void show_devinfo_debug_info(void);
extern void enable_devinfo_dcm(void);

#endif /* DEVINFO_H*/

