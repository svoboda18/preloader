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

#ifndef _SEC_POLICY_H_
#define _SEC_POLICY_H_

#include "sec_policy_config_common.h"
#include "sec_policy_config1.h"
#include "sec_policy_config2.h"
#include "sec_policy_config3.h"
#include "sec_policy_config4.h"

struct policy_part_map{
    unsigned int sw_id;
	char *part_name1;
	char *part_name2;
	char *part_name3;
	char *part_name4;
	/*secure policy*/
	unsigned char sec_sbcdis_lock_policy;   /*SBC :Disable, LOCK_STATE: Lock*/
	unsigned char sec_sbcdis_unlock_policy; /*SBC :Disable, LOCK_STATE: Unlock*/
	unsigned char sec_sbcen_lock_policy;    /*SBC :Enable, LOCK_STATE: Lock*/
	unsigned char sec_sbcen_unlock_policy;  /*SBC :ENable, LOCK_STATE: Unlock*/
	unsigned char *hash;
};

/*Custom Secure Policy*/
#define  CUST1_SEC_POLICY_1  CREATE_POLICY_ENTRY(BIND_CUST1_POLICY_1, VERIFY_CUST1_POLICY_1, DL_CUST1_POLICY_1)
#define  CUST1_SEC_POLICY_2  CREATE_POLICY_ENTRY(BIND_CUST1_POLICY_2, VERIFY_CUST1_POLICY_2, DL_CUST1_POLICY_2)
#define  CUST1_SEC_POLICY_3  CREATE_POLICY_ENTRY(BIND_CUST1_POLICY_3, VERIFY_CUST1_POLICY_3, DL_CUST1_POLICY_3)
#define  CUST1_SEC_POLICY_4  CREATE_POLICY_ENTRY(BIND_CUST1_POLICY_4, VERIFY_CUST1_POLICY_4, DL_CUST1_POLICY_4)

#define  CUST2_SEC_POLICY_1  CREATE_POLICY_ENTRY(BIND_CUST2_POLICY_1, VERIFY_CUST2_POLICY_1, DL_CUST2_POLICY_1)
#define  CUST2_SEC_POLICY_2  CREATE_POLICY_ENTRY(BIND_CUST2_POLICY_2, VERIFY_CUST2_POLICY_2, DL_CUST2_POLICY_2)
#define  CUST2_SEC_POLICY_3  CREATE_POLICY_ENTRY(BIND_CUST2_POLICY_3, VERIFY_CUST2_POLICY_3, DL_CUST2_POLICY_3)
#define  CUST2_SEC_POLICY_4  CREATE_POLICY_ENTRY(BIND_CUST2_POLICY_4, VERIFY_CUST2_POLICY_4, DL_CUST2_POLICY_4)

#endif