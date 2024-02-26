/******************************************************************************
  Copyright Statement:*  --------------------*  This software is protected by
Copyright and the information contained*  herein is confidential. The
software may not be copied and the information*  contained herein may not be
used or disclosed except with the written*  permission of MediaTek Inc. (C)
2011**  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND
AGREES*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE
")*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER
ON*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
WARRANTIES,*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
WARRANTIES OF*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
NONINFRINGEMENT.*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
RESPECT TO THE*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
INCORPORATED IN, OR*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES
TO LOOK ONLY TO SUCH*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
MEDIATEK SHALL ALSO*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES
MADE TO BUYER'S*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR
OPEN FORUM.**  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
CUMULATIVE*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED
HEREUNDER WILL BE,*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK
SOFTWARE AT ISSUE,*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE
PAID BY BUYER TO*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. **  THE
TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE*  WITH
THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF*  LAWS
PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND*
RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER*
THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC)
.*****************************************************************************/
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
    E_AREA_MAX
} E_INDEX;


/******************************************************************************
 * EXPORT FUNCTION
 ******************************************************************************/
extern u32 seclib_get_devinfo_with_index(u32 index);
extern void show_devinfo_debug_info(void);
extern void enable_devinfo_dcm(void);

#endif /* DEVINFO_H*/

