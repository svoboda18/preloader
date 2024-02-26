/* Copyright Statement:
*
* This software/firmware and related documentation ("MediaTek Software") are
* protected under relevant copyright laws. The information contained herein
* is confidential and proprietary to MediaTek Inc. and/or its licensors.
* Without the prior written permission of MediaTek inc. and/or its licensors,
* any reproduction, modification, use or disclosure of MediaTek Software,
* and information contained herein, in whole or in part, shall be strictly prohibited.
*/
/* MediaTek Inc. (C) 2018. All rights reserved.
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

#include <plat_debug.h>
#include <typedefs.h>
#include <sec_devinfo.h>

#include <sec_debugport.h>

/* L3 ram delsel */
#define RAM_DELSEL0 47
/* type0 ram delsel */
#define RAM_DELSEL1 48
/* type1 ram delsel */
#define RAM_DELSEL2 49

#define MP0_L3_DATA_RAM_DELSEL		*(unsigned int *)(0x0c53c840)
#define MP0_L3_TAG_RAM_DELSEL           *(unsigned int *)(0x0c53c844)
#define MP0_L3_VICTIM_RAM_DELSEL        *(unsigned int *)(0x0c53c848)
#define MP0_L3_DATA_RAM_DELSEL          *(unsigned int *)(0x0c53c84c)

#define CPU_TYPE0_RAM_DELSEL0_CFG	*(unsigned int *)(0x0c53cc20)
#define CPU_TYPE0_RAM_DELSEL1_CFG	*(unsigned int *)(0x0c53cc24)
#define CPU_TYPE0_RAM_DELSEL2_CFG	*(unsigned int *)(0x0c53cc28)

#define CPU_TYPE1_RAM_DELSEL0_CFG	*(unsigned int *)(0x0c53cd20)
#define CPU_TYPE1_RAM_DELSEL1_CFG	*(unsigned int *)(0x0c53cd24)
#define CPU_TYPE1_RAM_DELSEL2_CFG	*(unsigned int *)(0x0c53cd28)

void ram_delsel_setup(void)
{
	unsigned int ram_delsel0;
	unsigned int ram_delsel1;
	unsigned int ram_delsel2;

	print("ram_delsel init\n");
#if 1
	print("disable ram delsel by efuse\n");
#else
	ram_delsel0 = seclib_get_devinfo_with_index(RAM_DELSEL0);
	ram_delsel1 = seclib_get_devinfo_with_index(RAM_DELSEL1);
	ram_delsel2 = seclib_get_devinfo_with_index(RAM_DELSEL2);

	/*
	 * ram_delsel1[24] type1- 1: E-fuse programmed.
	 * ram_delsel1[25] type0- 1: E-fuse programmed.
	 * ram_delsel1[26] L3	- 1: E-fuse programmed.
	 */
	if ((ram_delsel1 >> 24) && 0x7 == 0x7) {
		MP0_L3_DATA_RAM_DELSEL =
			(unsigned int)((ram_delsel0) && 0xfff);
		MP0_L3_TAG_RAM_DELSEL =
			(unsigned int)((ram_delsel0 >> 12) && 0x3f);
		MP0_L3_VICTIM_RAM_DELSEL =
			(unsigned int)((ram_delsel0 >> 18) && 0x3f);
		MP0_L3_DATA_RAM_DELSEL =
			(unsigned int)((ram_delsel0 >> 24) && 0x3f);

		CPU_TYPE0_RAM_DELSEL0_CFG =
			(unsigned int)((ram_delsel1) && 0x3f);
		CPU_TYPE0_RAM_DELSEL1_CFG =
			(unsigned int)((ram_delsel1 >> 6) && 0xfff);
		CPU_TYPE0_RAM_DELSEL2_CFG =
			(unsigned int)((ram_delsel1 >> 18) && 0x3f);

		CPU_TYPE1_RAM_DELSEL0_CFG =
			(unsigned int)((ram_delsel2) && 0x3f);
		CPU_TYPE1_RAM_DELSEL1_CFG =
			(unsigned int)((ram_delsel2 >> 6) && 0xfff);
		CPU_TYPE1_RAM_DELSEL2_CFG =
			(unsigned int)((ram_delsel2 >> 18) && 0x3f);
	}
#endif
	print("MP0_L3_DATA_RAM_DELSEL: 0x%x\n", (unsigned int)MP0_L3_DATA_RAM_DELSEL);
	print("MP0_L3_TAG_RAM_DELSEL : 0x%x\n", (unsigned int)MP0_L3_TAG_RAM_DELSEL);
	print("MP0_L3_VICTIM_RAM_DELSEL: 0x%x\n", (unsigned int)MP0_L3_VICTIM_RAM_DELSEL);
	print("MP0_L3_DATA_RAM_DELSEL: 0x%x\n", (unsigned int)MP0_L3_DATA_RAM_DELSEL);
	print("CPU_TYPE0_RAM_DELSEL0_CFG: 0x%x\n", (unsigned int)CPU_TYPE0_RAM_DELSEL0_CFG);
	print("CPU_TYPE0_RAM_DELSEL1_CFG: 0x%x\n", (unsigned int)CPU_TYPE0_RAM_DELSEL1_CFG);
	print("CPU_TYPE0_RAM_DELSEL2_CFG: 0x%x\n", (unsigned int)CPU_TYPE0_RAM_DELSEL2_CFG);
	print("CPU_TYPE1_RAM_DELSEL0_CFG: 0x%x\n", (unsigned int)CPU_TYPE1_RAM_DELSEL0_CFG);
	print("CPU_TYPE1_RAM_DELSEL1_CFG: 0x%x\n", (unsigned int)CPU_TYPE1_RAM_DELSEL1_CFG);
	print("CPU_TYPE1_RAM_DELSEL2_CFG: 0x%x\n", (unsigned int)CPU_TYPE1_RAM_DELSEL2_CFG);

	return;
}

void mcu_dfd_debug_en(void)
{
	/* enable mcu dfd debug */
	seclib_dbgport_config(DCU_EN_0, MCU_DFD, MCU_DFD);
}
