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
 * MediaTek Inc. (C) 2019. All rights reserved.
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


/*=======================================================================*/
/* HEADER FILES                                                       */
/*=======================================================================*/
#include "platform.h"
#include "device_apc.h"
#include <pal_log.h>

#if CFG_GZ_SUPPORT
#include <gz_init.h>
#endif

/* Debug message event */
#define DBG_EVT_NONE        0x00000000    /* No event */
#define DBG_EVT_ERR         0x00000001    /* ERR related event */
#define DBG_EVT_DOM         0x00000002    /* DOM related event */
#define DBG_EVT_ALL         0xffffffff

#define DBG_EVT_MASK       (DBG_EVT_ALL)
#define MTAG	"[DEVAPC]"
#define _DEBUG_

#ifdef _DEBUG_
#define MSG(evt, fmt, args...) \
	do {    \
		if ((DBG_EVT_##evt) & DBG_EVT_MASK) { \
			pal_log_info(MTAG fmt, ##args); \
		} \
	} while(0)

#define MSG_FUNC_ENTRY(f)   MSG(FUC, "<FUN_ENT>: %s\n", __FUNCTION__)
#else
#define MSG(evt, fmt, args...) do{}while(0)
#define MSG_FUNC_ENTRY(f)     do{}while(0)
#endif

static uint32_t tz_dapc_set_master_transaction(E_MASTER_TYPE master_type,
		uint32_t master_index, E_TRANSACTION permisssion_control)
{
	volatile uint32_t *base = 0;
	uint32_t master_register_index = 0;
	uint32_t master_set_index = 0;

	master_register_index = master_index / (MOD_NO_IN_1_DEVAPC * 2);
	master_set_index = master_index % (MOD_NO_IN_1_DEVAPC * 2);

	if (master_type == MASTER_TYPE_INFRA &&
		master_index < MASTER_NUM_INFRA) {
		base = (volatile uint32_t *)((size_t)DEVAPC_AO_INFRA_MAS_SEC_0 +
			master_register_index * 4);
	} else {
		MSG(DOM, "[ERROR] out of boundary, %s=0x%x, %s=0x%x\n",
			"master_type", master_type,
			"master_index", master_index);

		return DEVAPC_ERR_OUT_OF_BOUNDARY;
	}

	if (base != 0) {
		reg_set_field(base, 0x1 << master_set_index,
				permisssion_control);

		return DEVAPC_OK;
	}

	return DEVAPC_ERR_GENERIC;
}

void tz_dapc_sec_init(void)
{
	/* No need?
	tz_dapc_set_master_transaction(MASTER_TYPE_INFRA, MASTER_MSDC0, SECURE_TRAN);
	tz_dapc_set_master_transaction(MASTER_TYPE_INFRA, MASTER_UFS, SECURE_TRAN);
	MSG(DOM, "sec_init Infra MAS_SEC_0=0x%x\n", reg_read32(DEVAPC_AO_INFRA_MAS_SEC_0));
	*/
}

void tz_dapc_sec_postinit(void)
{
	/* No need?
	tz_dapc_set_master_transaction(MASTER_TYPE_INFRA, MASTER_MSDC0, NON_SECURE_TRAN);
	tz_dapc_set_master_transaction(MASTER_TYPE_INFRA, MASTER_UFS, NON_SECURE_TRAN);
	MSG(DOM, "sec_postinit Infra MAS_SEC_0=0x%x\n", reg_read32(DEVAPC_AO_INFRA_MAS_SEC_0));
	*/
}

void device_APC_dom_setup(void)
{
	MSG(DOM, "Device APC domain init setup:\n");

	/* Set DEVAPC_AO reg to Swd access only */
	reg_write32(DEVAPC_AO_INFRA_APC_CON, 0x80000001);
	reg_write32(DEVAPC_AO_MD_APC_CON, 0x80000001);
	reg_write32(DEVAPC_AO_MM_APC_CON, 0x80000001);

	MSG(DOM, "APC_CON: INFRA(0x%x), MD(0x%x), MM(0x%x)\n",
		reg_read32(DEVAPC_AO_INFRA_APC_CON),
		reg_read32(DEVAPC_AO_MD_APC_CON),
		reg_read32(DEVAPC_AO_MM_APC_CON));

	/* SSPM will access SSPM memory region, but SSPM region is protected as
	 * secure access only by EMI MPU. So this setting is necessary.
	 */
	tz_dapc_set_master_transaction(MASTER_TYPE_INFRA, MASTER_SSPM,
			SECURE_TRAN);

	MSG(DOM, "Infra MAS_SEC_0=0x%x, MAS_SEC_1=0x%x\n",
		reg_read32(DEVAPC_AO_INFRA_MAS_SEC_0),
		reg_read32(DEVAPC_AO_INFRA_MAS_SEC_1));

/* ---------------------------------------------------------------------------------------- */
// INFRA MASTER DOMAIN settings

	/* Set MD1 to DOMAIN1(MD1) */
	reg_set_field(DEVAPC_AO_INFRA_MAS_DOM_2, MD1, DOMAIN_1);

	/* Set CONN2AP to DOMAIN2(CONN) */
	reg_set_field(DEVAPC_AO_INFRA_MAS_DOM_2, CONN2AP, DOMAIN_2);

	/* Set SSHUB(SSHUB=SCP=Sensor Hub) to DOMAIN3(SCP) */
	reg_set_field(DEVAPC_AO_INFRA_MAS_DOM_0, SCP, DOMAIN_3);

	/* Set MFG(GPU) to DOMAIN6(MFG) */
	reg_set_field(DEVAPC_AO_INFRA_MAS_DOM_3, MFG, DOMAIN_6);

	/* Set SSPM to DOMAIN8(SSPM) */
	reg_set_field(DEVAPC_AO_INFRA_MAS_DOM_3, SSPM, DOMAIN_8);

	/* Set SPM to DOMAIN9(SPM) */
	reg_set_field(DEVAPC_AO_INFRA_MAS_DOM_1, SPM, DOMAIN_9);

	/* Set ADSP(Audio DSP, HIFI3) to DOMAIN10(ADSP) */
	reg_set_field(DEVAPC_AO_INFRA_MAS_DOM_2, ADSP, DOMAIN_10);

#if (CFG_GZ_SUPPORT && CFG_GZ_REMAP)
	/* Set Remapped GZ to DOMAIN11(APMCU_HYPER) */
	if (is_booting_el2())
		reg_set_field(DEVAPC_AO_INFRA_MAS_DOM_0, APMCU_HYPER, DOMAIN_11);
#endif

	/* MD HW domain(7) and MM domain(4) are not set by DEVAPC */

	MSG(DOM, "Domain Setup Infra (0x%x), (0x%x), (0x%x), (0x%x), (0x%x)\n",
		reg_read32(DEVAPC_AO_INFRA_MAS_DOM_0),
		reg_read32(DEVAPC_AO_INFRA_MAS_DOM_1),
		reg_read32(DEVAPC_AO_INFRA_MAS_DOM_2),
		reg_read32(DEVAPC_AO_INFRA_MAS_DOM_3),
		reg_read32(DEVAPC_AO_INFRA_MAS_DOM_4));

/* ---------------------------------------------------------------------------------------- */
/* INFRA DOMAIN REMAP settings */
/* Remove Infra domain remapping */


/* ---------------------------------------------------------------------------------------- */
/* MD DOMAIN REMAP settings */

	/* Remap domain 0~16 (4-bit domain) to domain 3 (2-bit domain) by default */
	reg_write32(DEVAPC_AO_MD_DOM_RMP_0, MD_DOM_RMP_INIT);

	/* remap Infra domain 0 to MD view domain 0 */
	reg_set_field(DEVAPC_AO_MD_DOM_RMP_0, MD_INFRA_DOMAIN_0, DOMAIN_0);

	MSG(DOM, "MD DOMAIN REMAP 0:(0x%x)\n",
		reg_read32(DEVAPC_AO_MD_DOM_RMP_0));

/* ---------------------------------------------------------------------------------------- */
/* MM DOMAIN REMAP settings */

	/* Remap domain 0~16 (4-bit domain) to domain 3 (2-bit domain) by default */
	reg_write32(DEVAPC_AO_MM_DOM_RMP_0, MM_DOM_RMP_INIT);

	/* remap AP (Infra domain 0) to domain 0 */
	reg_set_field(DEVAPC_AO_MM_DOM_RMP_0, MM_AP, DOMAIN_0);

	/* remap SSPM  (Infra domain 8) to domain 2 */
	reg_set_field(DEVAPC_AO_MM_DOM_RMP_0, MM_SSPM, DOMAIN_2);

	MSG(DOM, "MM DOMAIN REMAP 0:(0x%x)\n",
		reg_read32(DEVAPC_AO_MM_DOM_RMP_0));
}

