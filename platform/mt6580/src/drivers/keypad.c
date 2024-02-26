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

#include "typedefs.h"
#include "platform.h"
#include "keypad.h"
#include "pmic.h"
#include <gpio.h>

void mtk_kpd_gpio_set(void);
extern U32 pmic_read_interface(U32 RegNum, U32 *val, U32 MASK, U32 SHIFT);


void mtk_kpd_gpios_get(unsigned int ROW_REG[], unsigned int COL_REG[], unsigned int GPIO_MODE[])
{
	int i;
	for (i = 0; i < 3; i++)
	{
		ROW_REG[i] = 0;
		COL_REG[i] = 0;
		GPIO_MODE[i] = 0;
	}
	#ifdef GPIO_KPD_KROW0_PIN
		ROW_REG[0] = GPIO_KPD_KROW0_PIN;
		GPIO_MODE[0] = GPIO_KPD_KROW0_PIN_M_KPROW;
	#endif

	#ifdef GPIO_KPD_KROW1_PIN
		ROW_REG[1] = GPIO_KPD_KROW1_PIN;
		GPIO_MODE[1] = GPIO_KPD_KROW1_PIN_M_KPROW;
	#endif

	#ifdef GPIO_KPD_KROW2_PIN
		ROW_REG[2] = GPIO_KPD_KROW2_PIN;
		GPIO_MODE[2] = GPIO_KPD_KROW2_PIN_M_KPROW;
	#endif

	#ifdef GPIO_KPD_KCOL0_PIN
		COL_REG[0] = GPIO_KPD_KCOL0_PIN;
		GPIO_MODE[0] |= (GPIO_KPD_KCOL0_PIN_M_KPCOL << 4);
	#endif

	#ifdef GPIO_KPD_KCOL1_PIN
		COL_REG[1] = GPIO_KPD_KCOL1_PIN;
		GPIO_MODE[1] |= (GPIO_KPD_KCOL1_PIN_M_KPCOL << 4);
	#endif

	#ifdef GPIO_KPD_KCOL2_PIN
		COL_REG[2] = GPIO_KPD_KCOL2_PIN;
		GPIO_MODE[2] |= (GPIO_KPD_KCOL2_PIN_M_KPCOL << 4);
	#endif
}

void mtk_kpd_gpio_set(void)
{
	unsigned int ROW_REG[3];
	unsigned int COL_REG[3];
	unsigned int GPIO_MODE[3];
	int i;

	print("Enter mtk_kpd_gpio_set! \n");
	mtk_kpd_gpios_get(ROW_REG, COL_REG, GPIO_MODE);

	//print("kpd debug column : %d, %d, %d, %d, %d, %d, %d, %d\n",COL_REG[0],COL_REG[1],COL_REG[2],COL_REG[3],COL_REG[4],COL_REG[5],COL_REG[6],COL_REG[7]);
	//print("kpd debug row : %d, %d, %d, %d, %d, %d, %d, %d\n",ROW_REG[0],ROW_REG[1],ROW_REG[2],ROW_REG[3],ROW_REG[4],ROW_REG[5],ROW_REG[6],ROW_REG[7]);

	for(i = 0; i < 3; i++)
	{
		if (COL_REG[i] != 0)
		{
			/* KCOL: GPIO INPUT + PULL ENABLE + PULL UP */
			mt_set_gpio_mode(COL_REG[i], ((GPIO_MODE[i] >> 4) & 0x0f));
			mt_set_gpio_dir(COL_REG[i], 0);
			mt_set_gpio_pull_enable(COL_REG[i], 1);
			mt_set_gpio_pull_select(COL_REG[i], 1);
		}

		if (ROW_REG[i] != 0)
		{
			/* KROW: GPIO output + pull disable + pull down */
			mt_set_gpio_mode(ROW_REG[i], (GPIO_MODE[i] & 0x0f));
			mt_set_gpio_dir(ROW_REG[i], 1);
			mt_set_gpio_pull_enable(ROW_REG[i], 0);
			mt_set_gpio_pull_select(ROW_REG[i], 0);
		}
	}

	mdelay(33);
}

void set_kpd_pmic_mode(void)
{
	unsigned int temp_reg = 0;

	mtk_kpd_gpio_set();

	temp_reg = DRV_Reg16(KP_SEL);

#if KPD_USE_EXTEND_TYPE		//double keypad
	/* select specific cols for double keypad */
	temp_reg &= KP_SEL_MASK; //clear HW default COL2 and ROW2 bit
	
	#ifndef GPIO_KPD_KCOL0_PIN
		temp_reg &= ~(KP_COL0_SEL);
	#endif

	#ifndef GPIO_KPD_KCOL1_PIN
		temp_reg &= ~(KP_COL1_SEL);
	#endif

	#ifndef GPIO_KPD_KCOL2_PIN
		temp_reg &= ~(KP_COL2_SEL);
	#endif

	temp_reg |= 0x1;

#else			//single keypad
	temp_reg &= (~0x1);
#endif

	DRV_WriteReg16(KP_SEL, temp_reg);
	DRV_WriteReg16(KP_EN, 0x1);
	print("after set KP enable: KP_SEL = 0x%x !\n", DRV_Reg16(KP_SEL));

	return;
}

void disable_PMIC_kpd_clock(void)
{

}
void enable_PMIC_kpd_clock(void)
{

}

bool mtk_detect_key(unsigned short key)  /* key: HW keycode */
{
	unsigned short idx, bit, din;
	U32 just_rst;

	if (key >= KPD_NUM_KEYS)
		return false;
#if 0
	if (key % 9 == 8)
		key = 8;
#endif

	if (key == MTK_PMIC_PWR_KEY)
	{	/* Power key */
		#if 0 // for long press reboot, not boot up from a reset
		pmic_read_interface(MT6325_STRUP_CON9, &just_rst, MT6325_PMIC_JUST_PWRKEY_RST_MASK, MT6325_PMIC_JUST_PWRKEY_RST_SHIFT);
		if(just_rst)
		{
			pmic_config_interface(MT6325_STRUP_CON9, 0x01, MT6325_PMIC_CLR_JUST_RST_MASK,  MT6325_PMIC_CLR_JUST_RST_SHIFT);
			print("Just recover from a reset\n");
			return false;
		}
		#endif
		if (1 == pmic_detect_powerkey())
		{
			print ("power key is pressed\n");
			return true;
		}
		return false;
	}


#ifdef MTK_PMIC_RST_KEY
	if (key == MTK_PMIC_RST_KEY)
	{
		print("mtk detect key function pmic_detect_homekey MTK_PMIC_RST_KEY = %d\n",MTK_PMIC_RST_KEY);
		if (1 == pmic_detect_homekey())
		{
			print("mtk detect key function pmic_detect_homekey pressed\n");
			return TRUE;
		}
		return FALSE;
	}
#endif

	idx = key / 16;
	bit = key % 16;

	din = DRV_Reg16(KP_MEM1 + (idx << 2)) & (1U << bit);
	if (!din) /* key is pressed */
	{
		print("key %d is pressed\n", key);
		return true;
	}
	return false;
}

bool mtk_detect_dl_keys(void)
{
	mtk_kpd_gpio_set();

#ifdef KPD_DL_KEY1
	if (!mtk_detect_key (KPD_DL_KEY1))
	return false;
#endif
#ifdef KPD_DL_KEY2
	if (!mtk_detect_key (KPD_DL_KEY2))
	return false;
#endif
#ifdef KPD_DL_KEY3
	if (!mtk_detect_key (KPD_DL_KEY3))
	return false;
#endif
#ifdef MTK_PMIC_RST_KEY
	if (!mtk_detect_key (MTK_PMIC_RST_KEY))
	return false;
#endif
	{
		print("download keys are pressed\n");
		return true;
	}
}
