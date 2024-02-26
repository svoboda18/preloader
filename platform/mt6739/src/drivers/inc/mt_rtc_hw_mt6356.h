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

#ifndef __MT_RTC_HW_MT6356_H__
#define __MT_RTC_HW_MT6356_H__

#undef RTC_BASE
#define RTC_BASE			(0x0980)

/* RTC registers */
#define RTC_BBPU			(RTC_BASE + 0x0088)
#define RTC_BBPU_PWREN			(1U << 0)	/* BBPU = 1 when alarm occurs */
#define RTC_BBPU_BBPU			(1U << 2)	/* 1: power on, 0: power down */
#define RTC_BBPU_AUTO			(1U << 3)	/* BBPU = 0 when xreset_rstb goes low */
#define RTC_BBPU_CLRPKY		(1U << 4)
#define RTC_BBPU_RELOAD		(1U << 5)
#define RTC_BBPU_CBUSY			(1U << 6)
#define RTC_BBPU_KEY			(0x43 << 8)

#define RTC_IRQ_STA			(RTC_BASE + 0x008A)
#define RTC_IRQ_STA_AL			(1U << 0)
#define RTC_IRQ_STA_TC			(1U << 1)
#define RTC_IRQ_STA_LP			(1U << 3)

#define RTC_IRQ_EN			(RTC_BASE + 0x008C)
#define RTC_IRQ_EN_AL			(1U << 0)
#define RTC_IRQ_EN_TC			(1U << 1)
#define RTC_IRQ_EN_ONESHOT		(1U << 2)
#define RTC_IRQ_EN_LP			(1U << 3)
#define RTC_IRQ_EN_ONESHOT_AL		(RTC_IRQ_EN_ONESHOT | RTC_IRQ_EN_AL)

#define RTC_CII_EN			(RTC_BASE + 0x008E)
#define RTC_CII_SEC 			(1U << 0)
#define RTC_CII_MIN 			(1U << 1)
#define RTC_CII_HOU 			(1U << 2)
#define RTC_CII_DOM 			(1U << 3)
#define RTC_CII_DOW 			(1U << 4)
#define RTC_CII_MTH 			(1U << 5)
#define RTC_CII_YEA 			(1U << 6)
#define RTC_CII_1_2_SEC			(1U << 7)
#define RTC_CII_1_4_SEC			(1U << 8)
#define RTC_CII_1_8_SEC			(1U << 9)

#define RTC_AL_MASK			(RTC_BASE + 0x0090)
#define RTC_AL_MASK_SEC		(1U << 0)
#define RTC_AL_MASK_MIN		(1U << 1)
#define RTC_AL_MASK_HOU		(1U << 2)
#define RTC_AL_MASK_DOM		(1U << 3)
#define RTC_AL_MASK_DOW		(1U << 4)
#define RTC_AL_MASK_MTH		(1U << 5)
#define RTC_AL_MASK_YEA		(1U << 6)

#define RTC_TC_SEC			(RTC_BASE + 0x0092)
#define RTC_TC_SEC_MASK 0x003f

#define RTC_TC_MIN			(RTC_BASE + 0x0094)
#define RTC_TC_MIN_MASK 0x003f

#define RTC_TC_HOU			(RTC_BASE + 0x0096)
#define RTC_TC_HOU_MASK 0x001f

#define RTC_TC_DOM			(RTC_BASE + 0x0098)
#define RTC_TC_DOM_MASK 0x001f

#define RTC_TC_DOW			(RTC_BASE + 0x009A)
#define RTC_TC_DOW_MASK 0x0007

#define RTC_TC_MTH			(RTC_BASE + 0x009C)
#define RTC_TC_MTH_MASK 0x000f

#define RTC_TC_YEA			(RTC_BASE + 0x009E)
#define RTC_TC_YEA_MASK 0x007f

#define RTC_AL_SEC			(RTC_BASE + 0x00A0)
#define RTC_K_EOSC32_VTCXO_ON_SEL	(1U << 15)
#define RTC_LPD_OPT_SHIFT		13
#define RTC_LPD_OPT			(1U << RTC_LPD_OPT_SHIFT)
#define RTC_LPD_OPT_MASK			        (3U << RTC_LPD_OPT_SHIFT)
#define RTC_LPD_OPT_XOSC_AND_EOSC_LPD (0U << RTC_LPD_OPT_SHIFT)
#define RTC_LPD_OPT_EOSC_LPD          (1U << RTC_LPD_OPT_SHIFT)
#define RTC_LPD_OPT_XOSC_LPD          (2U << RTC_LPD_OPT_SHIFT)
#define RTC_LPD_OPT_F32K_CK_ALIVE     (3U << RTC_LPD_OPT_SHIFT)
#define RTC_BBPU_2SEC_STAT_CLEAR	(1U << 11)
#define RTC_BBPU_2SEC_MODE		(3U << 9)
#define RTC_BBPU_2SEC_EN		(1U << 8)
#define RTC_BBPU_2SEC_CK_SEL		(1U << 7)
#define RTC_BBPU_AUTO_PDN_SEL		(1U << 6)
#define RTC_AL_SEC_MASK 0x003f

#define RTC_AL_MIN			(RTC_BASE + 0x00A2)
#define RTC_AL_MIN_MASK 0x003f

/*
 * RTC_NEW_SPARE0: RTC_AL_HOU bit0~4
 * bit 8 ~ 15 : For design used, can't be overwrited.
 */
#define RTC_AL_HOU			(RTC_BASE + 0x00A4)
#define RTC_NEW_SPARE0		0xff00
#define RTC_AL_HOU_MASK 		0x001f
#define RTC_AL_HOU_FG_SHIFT		8
#define RTC_AL_HOU_FG_MASK		0xff00

/*
 * RTC_NEW_SPARE1: RTC_AL_DOM bit0~4
 * bit 8 ~ 15 : for 2 second reboot desing used,
 *              can't be overwrited.
 */
#define RTC_AL_DOM			(RTC_BASE + 0x00A6)
#define RTC_NEW_SPARE1	0xff00
#define RTC_AL_DOM_MASK 0x001f

/*
 * RTC_NEW_SPARE2: RTC_AL_DOW bit0~2
 * 	   bit 8 ~ 15 : reserved bits
 */
#define RTC_AL_DOW			(RTC_BASE + 0x00A8)
#define RTC_NEW_SPARE2	0xff00
#define RTC_AL_DOW_MASK 0x0007

/*
 * RTC_NEW_SPARE3: RTC_AL_MTH bit0~3
 * bit 8 ~ 15 : Fuel Gauge
 */
#define RTC_AL_MTH			(RTC_BASE + 0x00AA)
#define RTC_NEW_SPARE3	0xff00
#define RTC_AL_MTH_MASK 0x000f
#define RTC_AL_MTH_FG_SHIFT     8
#define RTC_AL_MTH_FG_MASK      0xff00

#define RTC_AL_YEA			(RTC_BASE + 0x00AC)
#define RTC_K_EOSC_RSV_7		(1U << 15)
#define RTC_K_EOSC_RSV_6		(1U << 14)
#define RTC_K_EOSC_RSV_5		(1U << 13)
#define RTC_K_EOSC_RSV_4		(1U << 12)
#define RTC_K_EOSC_RSV_3		(1U << 11)
#define RTC_K_EOSC_RSV_2		(1U << 10)
#define RTC_K_EOSC_RSV_1		(1U << 9)
#define RTC_K_EOSC_RSV_0		(1U << 8)
#define RTC_AL_YEA_MASK 0x007f

#define RTC_OSC32CON			(RTC_BASE + 0x00AE)
#define RTC_OSC32CON_UNLOCK1		0x1a57
#define RTC_OSC32CON_UNLOCK2		0x2b68
#define RTC_REG_XOSC32_ENB		(1U << 15)
#define RTC_GP_OSC32_CON		(2U << 13)
#define RTC_EOSC32_CHOP_EN 		(1U << 12)
#define RTC_EOSC32_VCT_EN_SHIFT	11
#define RTC_EOSC32_VCT_EN		(1U << RTC_EOSC32_VCT_EN_SHIFT)
#define RTC_EOSC32_OPT			(1U << 10)	/* 0: embedded clock switch back to dcxo decided by (eosc32_ck_alive & powerkey_match)
							   1: embedded clock switch back to dcxo decided by (powerkey_match)
 							*/
#define RTC_EMBCK_SEL_OPTION		(1U << 9)
#define RTC_EMBCK_SRC_SEL		(1U << 8)
#define RTC_EMBCK_SEL_MODE		(3U << 6)

#define RTC_EMBCK_SEL_DCXO 		(0 << 6)	/*	0:dcxo_ck	1:eosc32_ck	2:emb_k_eosc_32	3: emb_hw	*/
#define RTC_EMBCK_SEL_EOSC 		(1U << 6)
#define RTC_EMBCK_SEL_K_EOSC	(2U << 6)
#define RTC_EMBCK_SEL_HW 		(3U << 6)

#define RTC_XOSC32_ENB			(1U << 5)	/* 0 (32k crystal exist)	1 (32k crystal doesn't exist)*/
#define RTC_XOSCCALI_MASK		0x001f		/*Default 4'b0111, 2nd step suggest to set to 4'b0000 EOSC_CALI = charging cap calibration*/

#define OSC32CON_ANALOG_SETTING	RTC_GP_OSC32_CON | RTC_EOSC32_VCT_EN | RTC_EOSC32_CHOP_EN  & (~RTC_REG_XOSC32_ENB) | RTC_EMBCK_SEL_MODE | RTC_EMBCK_SEL_OPTION | RTC_EMBCK_SRC_SEL

#define RTC_XOSCCALI_START		0x0000
#define RTC_XOSCCALI_END		0x001f

#define RTC_POWERKEY1			(RTC_BASE + 0x00B0)
#define RTC_POWERKEY2			(RTC_BASE + 0x00B2)
#define RTC_POWERKEY1_KEY		0xa357
#define RTC_POWERKEY2_KEY		0x67d2
/*
 * RTC_PDN1:
 *     bit 0 - 3  : Android bits
 *     bit 4 - 5  : Recovery bits (0x10: factory data reset)
 *     bit 6      : Bypass PWRKEY bit
 *     bit 7      : Power-On Time bit
 *     bit 8      : RTC_GPIO_USER_WIFI bit
 *     bit 9      : RTC_GPIO_USER_GPS bit
 *     bit 10     : RTC_GPIO_USER_BT bit
 *     bit 11     : RTC_GPIO_USER_FM bit
 *     bit 12     : RTC_GPIO_USER_PMIC bit
 *     bit 13     : Fast Boot
 *     bit 14	  : Kernel Power Off Charging
 *     bit 15     : Debug bit
 */
#define RTC_PDN1			(RTC_BASE + 0x00B4)
#define RTC_PDN1_ANDROID_MASK		0x000f
#define RTC_PDN1_RECOVERY_MASK		0x0030
#define RTC_PDN1_FAC_RESET		(1U << 4)
#define RTC_PDN1_BYPASS_PWR		(1U << 6)
#define RTC_PDN1_PWRON_TIME		(1U << 7)
#define RTC_PDN1_GPIO_WIFI		(1U << 8)
#define RTC_PDN1_GPIO_GPS		(1U << 9)
#define RTC_PDN1_GPIO_BT		(1U << 10)
#define RTC_PDN1_GPIO_FM		(1U << 11)
#define RTC_PDN1_GPIO_PMIC		(1U << 12)
#define RTC_PDN1_FAST_BOOT		(1U << 13)
#define RTC_PDN1_KPOC			(1U << 14)
#define RTC_PDN1_DEBUG			(1U << 15)

/*
 * RTC_PDN2:
 *     bit 0 - 3 : MTH in power-on time
 *     bit 4     : Power-On Alarm bit
 *     bit 5 - 6 : UART bits
 *     bit 7     : reserved bit
 *     bit 8 - 14: YEA in power-on time
 *     bit 15    : Power-On Logo bit
 */
#define RTC_PDN2			(RTC_BASE + 0x00B6)
#define RTC_PDN2_PWRON_MTH_MASK 	0x000f
#define RTC_PDN2_PWRON_MTH_SHIFT 	0
#define RTC_PDN2_PWRON_ALARM 		(1U << 4)
#define RTC_PDN2_UART_MASK		0x0060
#define RTC_PDN2_UART_SHIFT		5
#define RTC_PDN2_PWRON_YEA_MASK 	0x7f00
#define RTC_PDN2_AUTOBOOT         (1U << 7)
#define RTC_PDN2_PWRON_YEA_SHIFT 	8
#define RTC_PDN2_PWRON_LOGO		(1U << 15)

/*
 * RTC_SPAR0:
 *     bit 0 - 5 : SEC in power-on time
 *     bit 6 	 : 32K less bit. True:with 32K, False:Without 32K
 *     bit 7 - 15: reserved bits
 */
#define RTC_SPAR0			(RTC_BASE + 0x00B8)
#define RTC_SPAR0_PWRON_SEC_MASK 	0x003f
#define RTC_SPAR0_PWRON_SEC_SHIFT 	0
#define RTC_SPAR0_32K_LESS 		(1U << 6)
#define RTC_SPAR0_LP_DET		(1U << 7)

/*
 * RTC_SPAR1:
 *     bit 0 - 5  : MIN in power-on time
 *     bit 6 - 10 : HOU in power-on time
 *     bit 11 - 15: DOM in power-on time
 */
#define RTC_SPAR1			(RTC_BASE + 0x00BA)
#define RTC_SPAR1_PWRON_MIN_MASK 	0x003f
#define RTC_SPAR1_PWRON_MIN_SHIFT 	0
#define RTC_SPAR1_PWRON_HOU_MASK 	0x07c0
#define RTC_SPAR1_PWRON_HOU_SHIFT 	6
#define RTC_SPAR1_PWRON_DOM_MASK 	0xf800
#define RTC_SPAR1_PWRON_DOM_SHIFT 	11

#define RTC_PROT			(RTC_BASE + 0x00BC)
#define RTC_PROT_UNLOCK1 		0x586a
#define RTC_PROT_UNLOCK2 		0x9136

#define RTC_DIFF			(RTC_BASE + 0x00BE)
#define RTC_CALI_RD_SEL_SHIFT		15
#define RTC_CALI_RD_SEL 		(1U << RTC_CALI_RD_SEL_SHIFT)
#define RTC_K_EOSC32_RSV		(1U << 14)
#define RTC_POWER_DETECTED_SHIFT		12
#define RTC_POWER_DETECTED 		(1U << RTC_POWER_DETECTED_SHIFT)
#define RTC_DIFF_MASK			0xFFF
#define RTC_DIFF_SHIFT			0

#define RTC_CALI			(RTC_BASE + 0x00C0)
#define RTC_K_EOSC32_OVERFLOW		(1U << 15)
#define RTC_CALI_WR_SEL_SHIFT		14
#define RTC_CALI_WR_SEL_MASK		1
#define RTC_CALI_XOSC			0
#define RTC_CALI_K_EOSC_32		1
#define RTC_CALI_MASK			0x3FFF
#define RTC_CALI_SHIFT			0

#define RTC_WRTGR			(RTC_BASE + 0x00C2)

#define RTC_CON				(RTC_BASE + 0x00C4)
#define RTC_VBAT_LPSTA_RAW		(1U << 0)
#define RTC_EOSC32_LPEN			(1U << 1)
#define RTC_XOSC32_LPEN			(1U << 2)
#define RTC_CON_LPRST			(1U << 3)
#define RTC_CON_CDBO			(1U << 4)
#define RTC_CON_F32KOB			(1U << 5)	/* 0: RTC_GPIO exports 32K */
#define RTC_CON_GPO			(1U << 6)
#define RTC_CON_GOE			(1U << 7)	/* 1: GPO mode, 0: GPI mode */
#define RTC_CON_GSR			(1U << 8)
#define RTC_CON_GSMT			(1U << 9)
#define RTC_CON_GPEN			(1U << 10)
#define RTC_CON_GPU			(1U << 11)
#define RTC_CON_GE4			(1U << 12)
#define RTC_CON_GE8			(1U << 13)
#define RTC_CON_GPI			(1U << 14)
#define RTC_CON_LPSTA_RAW		(1U << 15)	/* 32K was stopped */

#endif /* __MT_RTC_HW_MT6356_H__ */

