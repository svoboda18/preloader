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

#include <typedefs.h>
#include <rtc.h>
#include <timer.h>
#include <pmic_wrap_init.h>
#include <platform.h>
#include <cust_rtc.h>
#include <pmic.h>
#include <mt6580.h>
#include "mt_rtc_hw.h"

#define RTC_RELPWR_WHEN_XRST	1   /* BBPU = 0 when xreset_rstb goes low */

#define RTC_GPIO_USER_MASK	  (((1U << 13) - 1) & 0xff00)

//#define RTC_LEAKAGE_TEST

static bool recovery_flag = false;

static bool rtc_busy_wait(void);
static bool Write_trigger(void);
static U16 eosc_cali(void);
static bool rtc_first_boot_init(void);
static U16 get_frequency_meter(U16 val, U16 measureSrc, U16 window_size);
static bool rtc_frequency_meter_check(void);
static void rtc_recovery_flow(void);
static bool rtc_recovery_mode_check(void);
static bool rtc_init_after_recovery(void);
static bool rtc_get_recovery_mode_stat(void);
static bool rtc_gpio_init(void);
static bool rtc_android_init(void);
static bool rtc_lpd_init(void);
static bool Writeif_unlock(void);
#if RTC_2SEC_REBOOT_ENABLE
static bool rtc_2sec_stat_clear(void);
void rtc_enable_2sec_reboot(void);
void rtc_save_2sec_stat(void);
#endif

#ifdef RTC_LEAKAGE_TEST
void rtc_mode_test(void);
#endif

static U16 RTC_Read(U16 addr)
{
	U32 rdata=0;
	pwrap_read((U32)addr, &rdata);
	return (U16)rdata;
}

static void RTC_Write(U16 addr, U16 data)
{
	pwrap_write((U32)addr, (U32)data);
}

static bool rtc_busy_wait(void)
{
	ulong begin = get_timer(0);
	do {
		while (RTC_Read(RTC_BBPU) & RTC_BBPU_CBUSY)
		{
			/////>    Time > 1sec,  time out and set recovery mode enable.  </////
			if (get_timer(begin) > 1000)
			{
				print("[RTC] rtc cbusy time out!!!!!\n");
				return false;
			}
		}
	} while (0);

	return true;
}

static void rtc_call_exception(void)
{
	ASSERT(0);
}

static bool rtc_xosc_check_clock(U16 *result)
{
	///// fix me  loose range for frequency meter result////
	if ((result[0] >= 3  &&result[0] <= 7 ) &&
			(result[1] > 1500 && result[1] < 6000) &&
			(result[2] == 0) &&
			(result[3] == 0))
		return true;
	else
		return false;
}

static bool rtc_eosc_check_clock(U16 *result)
{
	if ((result[0] >= 3  &&result[0] <= 7 )&&
			(result[1] < 500) &&
			(result[2] > 2 && result[2] < 9) &&
			(result[3] > 300 && result[3] < 10400))
		return true;
	else
		return false;
}


static void rtc_xosc_write(U16 val)
{
	U16 bbpu;

	RTC_Write(RTC_OSC32CON, RTC_OSC32CON_UNLOCK1);
	mdelay(1);
	RTC_Write(RTC_OSC32CON, RTC_OSC32CON_UNLOCK2);
	mdelay(1);
	RTC_Write(RTC_OSC32CON, val);
	mdelay(1);
}

static U16 get_frequency_meter(U16 val, U16 measureSrc, U16 window_size)
{
	U16 ret;
	int i;
	ulong begin = get_timer(0);

	if(val!=0) {
		U16 osc32con;
		osc32con = RTC_Read(RTC_OSC32CON) & ~RTC_XOSCCALI_MASK & ~RTC_REG_XOSC32_ENB; //clear [0:4] [9]
		osc32con |= ((osc32con & RTC_XOSC32_ENB) >> RTC_XOSC32_ENB_SHIFT) << RTC_REG_XOSC32_ENB_SHIFT;
		print("[RTC] get_frequency_meter: val=0x%x osc32con=0x%x\n", val, osc32con | (val & 0x1f));
		rtc_xosc_write(osc32con | (val & 0x1f));
		print("[RTC] get_frequency_meter: osc32con=0x%x\n", RTC_Read(RTC_OSC32CON));
	}

	RTC_Write(MT_FQMTR_CON0, 0x0000); //disable freq. meter, gated src clock
	RTC_Write(MT_TOP_CKPDN1, RTC_Read(MT_TOP_CKPDN1)|0x20); //TOP_CKPDN1[5]=1, gated fixed clock

	RTC_Write(MT_TOP_RST_CON, 0x0100);			//FQMTR reset
	while( !(RTC_Read(MT_FQMTR_CON2)==0) && (0x8&RTC_Read(MT_FQMTR_CON0))==0x8);
	RTC_Write(MT_TOP_RST_CON, 0x0000);			//FQMTR normal

	RTC_Write(MT_TOP_CKPDN1, RTC_Read(MT_TOP_CKPDN1)&0xffdf); //TOP_CKPDN1[5]=0, turn on fixed clock

	RTC_Write(MT_FQMTR_CON1, window_size); //set freq. meter window value (0=1X32K(fix clock))
	RTC_Write(MT_FQMTR_CON0, 0x8000 | measureSrc); //enable freq. meter, set measure clock to 26Mhz

	mdelay(1);
	while( (0x8&RTC_Read(MT_FQMTR_CON0))==0x8 )
	{
		if (get_timer(begin) > 50)
		{
			print("get frequency time out\n");
			RTC_Write(MT_FQMTR_CON0, RTC_Read(MT_FQMTR_CON0) & ~0x8000); //disable freq. meter
			break;
		}
	};		// FQMTR read until ready

	ret = RTC_Read(MT_FQMTR_CON2);	//read data should be closed to 26M/32k = 812.5
	RTC_Write(MT_FQMTR_CON0, RTC_Read(MT_FQMTR_CON0) & ~0x8000); //disable freq. meter
	print("[RTC] get_frequency_meter: input=0x%x, ouput=%d\n",val, ret);

	return ret;
}

static void rtc_measure_four_clock(U16 *result)
{
	U16 window_size;

	RTC_Write(MT_TOP_CKCON1, (RTC_Read(MT_TOP_CKCON1) & ~0x00C0) ); //select 26M as fixed clock (frequency meter clock source)
	window_size = 4;
	mdelay(1);
	result[0] = get_frequency_meter(0, 0x0004, window_size); 		//select 26M as target clock

	//select XOSC_DET as fixed clock
	RTC_Write(MT_TOP_CKCON1, (RTC_Read(MT_TOP_CKCON1) | 0x0080));
	window_size = 4;
	mdelay(1);
	result[1] = get_frequency_meter(0, 0x0004, window_size); 		//select 26M as target clock

	//select 26M as fixed clock
	RTC_Write(MT_TOP_CKCON1, (RTC_Read(MT_TOP_CKCON1) & ~0x00C0));
	window_size = 3970;  // (26M / 32K) * 5
	mdelay(1);
	result[2] = get_frequency_meter(0, 0x0000, window_size); 								//select DCXO_32 as target clock
	result[2] = get_frequency_meter(0, 0x0001, window_size); 								//select DCXO_32 as target clock

	//select EOSC_32 as fixed clock
	RTC_Write(MT_TOP_CKCON1, (RTC_Read(MT_TOP_CKCON1) | 0x00C0));
	window_size = 4;
	mdelay(1);
	result[3] = get_frequency_meter(0, 0x0004, window_size); 			//select 26M as target clock
}

static void rtc_switch_mode(bool XOSC, bool recovery)
{
	if (XOSC)
	{
		if (recovery)
		{
			/* HW bypass switch mode control and set to XOSC */
			RTC_Write(MT_CHRSTATUS, (RTC_Read(MT_CHRSTATUS) | 0x0800 & ~0x0200));
		}
		rtc_xosc_write(OSC32CON_ANALOG_SETTING | 0x0003);  /* assume crystal exist mode + XOSCCALI = 0x3 */
		if (recovery)
			mdelay(1000);
	}
	else
	{
		if (recovery)
		{
			/* HW bypass switch mode control and set to DCXO */
			RTC_Write(MT_CHRSTATUS, (RTC_Read(MT_CHRSTATUS) | 0x0A00));
		}
		rtc_xosc_write(0x240F); /*crystal not exist + eosc cali = 0xF*/
		mdelay(10);
	}
}

static void rtc_switch_to_xosc_mode(void)
{
	rtc_switch_mode(true, false);
}

static void rtc_switch_to_dcxo_mode(void)
{
	rtc_switch_mode(false, false);
}

static void rtc_switch_to_xosc_recv_mode(void)
{
	rtc_switch_mode(true, true);
}

static void rtc_switch_to_dcxo_recv_mode(void)
{
	rtc_switch_mode(false, true);
}

static bool rtc_get_xosc_mode(void)
{
	U16 con, xosc_mode;;

	con = RTC_Read(RTC_OSC32CON);

	if((con & 0x0020) == 0)
	{
		xosc_mode = 1;
	}
	else
		xosc_mode = 0;
	return xosc_mode;
}

static bool rtc_frequency_meter_check(void)
{
	U16  result[4], osc32con;
	int  cali;

	if (rtc_get_recovery_mode_stat())
		rtc_switch_to_xosc_recv_mode();

	rtc_measure_four_clock(result);

	if (rtc_xosc_check_clock(result))
	{
		print("OSC32CON_ANALOG_SETTING = 0x%x\n", OSC32CON_ANALOG_SETTING);
		osc32con = OSC32CON_ANALOG_SETTING;
		rtc_xosc_write(osc32con);	/* crystal exist mode + XOSCCALI = 0 */
		return true;
	}
	else
	{
		if (!rtc_get_recovery_mode_stat())
			rtc_switch_to_dcxo_mode();
		else
			rtc_switch_to_dcxo_recv_mode();
	}

	rtc_measure_four_clock(result);

	if (rtc_eosc_check_clock(result))
	{
		U16 val = 0;

		val = eosc_cali();
		print("[RTC] EOSC cali val = 0x%x\n", val);
		//EMB_HW_Mode
		osc32con = OSC32CON_ANALOG_SETTING | RTC_REG_XOSC32_ENB;
		val = (val & RTC_XOSCCALI_MASK) | osc32con;
		print("[RTC] EOSC cali val = 0x%x\n", val);
		rtc_xosc_write(val);
		return true;
	}
	else
	{
		return false;
	}
}

static void rtc_set_recovery_mode_stat(bool enable)
{
	recovery_flag = enable;
}

static bool rtc_get_recovery_mode_stat(void)
{
	return recovery_flag;
}

static bool rtc_init_after_recovery(void)
{
	if (!Writeif_unlock())
		return false;
	/* write powerkeys */
	RTC_Write(RTC_POWERKEY1, RTC_POWERKEY1_KEY);
	RTC_Write(RTC_POWERKEY2, RTC_POWERKEY2_KEY);
	if (!Write_trigger())
		return false;

	RTC_Write(MT_CHRSTATUS, (RTC_Read(MT_CHRSTATUS) & ~0x0800));

	if (!rtc_gpio_init())
		return false;
	if (!rtc_android_init())
		return false;
	if (!rtc_lpd_init())
		return false;

	return true;
}
static bool rtc_recovery_mode_check(void)
{
	/////// fix me add return ret for recovery mode check fail
	if (!rtc_frequency_meter_check())
	{
		rtc_call_exception();
		return false;
	}
	return true;
}

static void rtc_recovery_flow(void)
{
	U8 count = 0;
	print("rtc_recovery_flow\n");
	rtc_set_recovery_mode_stat(true);
	while (count < 3)
	{
		if(rtc_recovery_mode_check())
		{
			if (rtc_init_after_recovery())
				break;
		}
		count++;
	}
	rtc_set_recovery_mode_stat(false);
	if (count == 3)
		rtc_call_exception();

}
#if defined (MTK_KERNEL_POWER_OFF_CHARGING)
extern kal_bool kpoc_flag ;
#endif

static unsigned long rtc_mktime(int yea, int mth, int dom, int hou, int min, int sec)
{
	unsigned long d1, d2, d3;

	mth -= 2;
	if (mth <= 0) {
		mth += 12;
		yea -= 1;
	}

	d1 = (yea - 1) * 365 + (yea / 4 - yea / 100 + yea / 400);
	d2 = (367 * mth / 12 - 30) + 59;
	d3 = d1 + d2 + (dom - 1) - 719162;

	return ((d3 * 24 + hou) * 60 + min) * 60 + sec;
}

static bool Write_trigger(void)
{
	RTC_Write(RTC_WRTGR, 1);
	if (rtc_busy_wait())
		return true;
	else
		return false;
}

static bool Writeif_unlock(void)
{
	RTC_Write(RTC_PROT, RTC_PROT_UNLOCK1);
	if (!Write_trigger())
		return false;
	RTC_Write(RTC_PROT, RTC_PROT_UNLOCK2);
	if (!Write_trigger())
		return false;

	return true;
}

static bool rtc_android_init(void)
{
	U16 irqsta;

	RTC_Write(RTC_IRQ_EN, 0);
	RTC_Write(RTC_CII_EN, 0);
	RTC_Write(RTC_AL_MASK, 0);

	RTC_Write(RTC_AL_YEA, 1970 - RTC_MIN_YEAR);
	RTC_Write(RTC_AL_MTH, 1);
	RTC_Write(RTC_AL_DOM, 1);
	RTC_Write(RTC_AL_DOW, 1);
	RTC_Write(RTC_AL_HOU, 0);
	RTC_Write(RTC_AL_MIN, 0);
	RTC_Write(RTC_AL_SEC, 0);

	RTC_Write(RTC_PDN1, RTC_PDN1_DEBUG);   /* set Debug bit */
	RTC_Write(RTC_PDN2, ((1970 - RTC_MIN_YEAR) << RTC_PDN2_PWRON_YEA_SHIFT) | 1);
	RTC_Write(RTC_SPAR0, 0);
	RTC_Write(RTC_SPAR1, (1 << RTC_SPAR1_PWRON_DOM_SHIFT));

	RTC_Write(RTC_DIFF, 0);
	RTC_Write(RTC_CALI, 0);
#if RTC_2SEC_REBOOT_ENABLE
	if (!rtc_2sec_stat_clear())
		return false;
#endif
	if (!Write_trigger())
		return false;

	irqsta = RTC_Read(RTC_IRQ_STA);	/* read clear */

	/* init time counters after resetting RTC_DIFF and RTC_CALI */
	RTC_Write(RTC_TC_YEA, RTC_DEFAULT_YEA - RTC_MIN_YEAR);
	RTC_Write(RTC_TC_MTH, RTC_DEFAULT_MTH);
	RTC_Write(RTC_TC_DOM, RTC_DEFAULT_DOM);
	RTC_Write(RTC_TC_DOW, 1);
	RTC_Write(RTC_TC_HOU, 0);
	RTC_Write(RTC_TC_MIN, 0);
	RTC_Write(RTC_TC_SEC, 0);
	if(!Write_trigger())
		return false;

	return true;
}

static bool rtc_gpio_init(void)
{
	U16 con;

	/* GPI mode and pull enable + pull down */
	con = RTC_Read(RTC_CON) & (RTC_CON_LPSTA_RAW | RTC_CON_LPRST | RTC_XOSC32_LPEN);
	con &= ~RTC_CON_GPU;
	con |= RTC_CON_GPEN | RTC_CON_GOE | RTC_CON_F32KOB;
	RTC_Write(RTC_CON, con);
	if (Write_trigger())
		return true;
	else
		return false;
}

static U16 eosc_cali(void)
{
	U16 val, diff1, diff2;
	int middle;
	int i, j;
	int left = RTC_XOSCCALI_START, right = RTC_XOSCCALI_END;

	RTC_Write(MT_TOP_CKCON1, (RTC_Read(MT_TOP_CKCON1)|0x00C0) );//fix clock = eosc 32K
	print("[RTC] EOSC_Cali: TOP_CKCON1=0x%x\n", RTC_Read(MT_TOP_CKCON1));
	while( left<=(right) )
	{
		middle = (right + left) / 2;
		if(middle == left)
			break;

		val = get_frequency_meter(middle, FQMTR_FQM26M_CK, 0);
		print("[RTC] EOSC_Cali: val=0x%x\n", val);
		if ((val>=RTC_FQMTR_LOW_BASE) && (val<=RTC_FQMTR_HIGH_BASE))
			break;
		if (val > RTC_FQMTR_HIGH_BASE)
			//right = middle - 1;
			right = middle;
		else
			//left = middle + 1;
			left = middle;
	}

	if ((val>=RTC_FQMTR_LOW_BASE) && (val<=RTC_FQMTR_HIGH_BASE))
		return middle;

	val=get_frequency_meter(left, FQMTR_FQM26M_CK, 0);
	if (val > RTC_FQMTR_LOW_BASE)
		diff1=val - RTC_FQMTR_LOW_BASE;
	else
		diff1=RTC_FQMTR_LOW_BASE - val;
	val=get_frequency_meter(right, FQMTR_FQM26M_CK, 0);
	if (val > RTC_FQMTR_LOW_BASE)
		diff2=val - RTC_FQMTR_LOW_BASE;
	else
		diff2=RTC_FQMTR_LOW_BASE - val;
	if(diff1<diff2)
		return left;
	else
		return right;
}

static void rtc_lpd_state_clr(void)
{
	U16 spar0;

	spar0 = RTC_Read(RTC_SPAR0);

	RTC_Write(RTC_SPAR0, spar0 & (~0x0080) ); //bit 7 for low power detected in preloader
	Write_trigger();
	spar0 = RTC_Read(RTC_SPAR0);
	print("[RTC] RTC_SPAR0=0x%x \n", spar0);
}

static void rtc_osc_init(void)
{
	U16 con, osc32con;
	/* disable 32K export if there are no RTC_GPIO users */
	if (!(RTC_Read(RTC_PDN1) & RTC_GPIO_USER_MASK))
		rtc_gpio_init();

	if(rtc_get_xosc_mode())
	{
		con = RTC_Read(RTC_OSC32CON);
		if ((con & RTC_XOSCCALI_MASK) != 0x0) {	/* check XOSCCALI */
			osc32con = OSC32CON_ANALOG_SETTING;
			rtc_xosc_write(osc32con | 0x0003);  /* crystal exist mode + XOSCCALI = 0x3 */
			gpt_busy_wait_us(200);
		}
		osc32con = OSC32CON_ANALOG_SETTING;
		rtc_xosc_write(osc32con);  /* crystal exist mode + XOSCCALI = 0x0 */
	}
	else
	{
		U16 val;
		val = eosc_cali();
		osc32con = OSC32CON_ANALOG_SETTING | RTC_REG_XOSC32_ENB;
		val = (val & 0x001f) | osc32con;
		print("[RTC] EOSC cali val = 0x%x\n", val);
		//SW_EOSC_Mode
		rtc_xosc_write(val);
	}
	rtc_lpd_state_clr();
}

static bool rtc_lpd_init(void)
{
	U16 con;

	con = RTC_Read(RTC_CON) | RTC_XOSC32_LPEN;
	con &= ~RTC_CON_LPRST;
	RTC_Write(RTC_CON, con);
	if (!Write_trigger())
		return false;

	con |= RTC_CON_LPRST;
	RTC_Write(RTC_CON, con);
	if (!Write_trigger())
		return false;

	con &= ~RTC_CON_LPRST;
	RTC_Write(RTC_CON, con);
	if (!Write_trigger())
		return false;

	con |= RTC_CON_LPRST;
	RTC_Write(RTC_CON, con);
	if (!Write_trigger())
		return false;

	con &= ~RTC_CON_LPRST;
	RTC_Write(RTC_CON, con);
	if (!Write_trigger())
		return false;

	RTC_Write(RTC_SPAR0, RTC_Read(RTC_SPAR0) | 0x0080 ); //bit 7 for low power detected in preloader
	if (!Write_trigger())
		return false;
	return true;
}

static bool rtc_first_boot_init(void)
{
	print("rtc_first_boot_init\n");

	if (!Writeif_unlock())
		return false;

	if (!rtc_gpio_init())
		return false;
	rtc_switch_to_xosc_mode();
	/* write powerkeys */
	RTC_Write(RTC_POWERKEY1, RTC_POWERKEY1_KEY);
	RTC_Write(RTC_POWERKEY2, RTC_POWERKEY2_KEY);
	if (!Write_trigger())
		return false;
	mdelay(1000);

	if (!rtc_frequency_meter_check())
		return false;
	if (!rtc_android_init())
		return false;
	if (!rtc_lpd_init())
		return false;

	return true;
}

static void rtc_bbpu_power_down(void)
{
	U16 bbpu;

	/* pull PWRBB low */
	bbpu = RTC_BBPU_KEY | RTC_BBPU_AUTO | RTC_BBPU_PWREN;
	Writeif_unlock();
	RTC_Write(RTC_BBPU, bbpu);
	Write_trigger();
}

void rtc_bbpu_power_on(void)
{
	U16 bbpu, pdn2;

	/* pull PWRBB high */
#if RTC_RELPWR_WHEN_XRST
	bbpu = RTC_BBPU_KEY | RTC_BBPU_AUTO | RTC_BBPU_BBPU | RTC_BBPU_PWREN;
#else
	bbpu = RTC_BBPU_KEY | RTC_BBPU_BBPU | RTC_BBPU_PWREN;
#endif
	RTC_Write(RTC_BBPU, bbpu);
	Write_trigger();
	print("[RTC] rtc_bbpu_power_on done\n");
#if RTC_2SEC_REBOOT_ENABLE
	rtc_enable_2sec_reboot();
	print("[RTC] rtc_enable_2sec_reboot done\n");
	//clear IPO shutdown block auto reboot pdn
	//if IPO shutdown pdn set. rtc_2sec_reboot_check() return false,
	//which means must be other boot reason, cause preloader call rtc_bbpu_power_on()
	pdn2 = RTC_Read(RTC_PDN2) & ~RTC_PDN2_AUTOBOOT;
	RTC_Write(RTC_PDN2, pdn2);
	Write_trigger();
#else
	RTC_Write(RTC_CALI, RTC_Read(RTC_CALI) & ~RTC_CALI_BBPU_2SEC_EN);
#endif
}

void rtc_mark_bypass_pwrkey(void)
{
	U16 pdn1;

	pdn1 = RTC_Read(RTC_PDN1) | RTC_PDN1_BYPASS_PWR;
	RTC_Write(RTC_PDN1, pdn1);
	Write_trigger();
}

static void rtc_clean_mark(void)
{
	U16 pdn1, pdn2;

	pdn1 = RTC_Read(RTC_PDN1) & ~(RTC_PDN1_DEBUG | RTC_PDN1_BYPASS_PWR);   /* also clear Debug bit */
	pdn2 = RTC_Read(RTC_PDN2) & ~RTC_PDN1_FAC_RESET;
	RTC_Write(RTC_PDN1, pdn1);
	RTC_Write(RTC_PDN2, pdn2);
	Write_trigger();
}

U16 rtc_rdwr_uart_bits(U16 *val)
{
	U16 pdn2;

	if (RTC_Read(RTC_CON) & RTC_CON_LPSTA_RAW)
		return 3;   /* UART bits are invalid due to RTC uninit */

	if (val) {
		pdn2 = RTC_Read(RTC_PDN2) & ~RTC_PDN2_UART_MASK;
		pdn2 |= (*val & (RTC_PDN2_UART_MASK >> RTC_PDN2_UART_SHIFT)) << RTC_PDN2_UART_SHIFT;
		RTC_Write(RTC_PDN2, pdn2);
		Write_trigger();
	}

	return (RTC_Read(RTC_PDN2) & RTC_PDN2_UART_MASK) >> RTC_PDN2_UART_SHIFT;
}

bool rtc_boot_check(void)
{
#if 1
	U16 irqsta, pdn1, pdn2, spar0, spar1, Rdata;
	U32 osc32con;
	U16 result[4];
	U16 XOSC = 0;
	U16 cali;
	bool check_mode_flag = false;

	//Disable RTC CLK gating
	RTC_Write(MT_TOP_CKPDN1, 0x3df);

	// check clock source are match with 32K exist
	rtc_measure_four_clock(result);
	if (!rtc_xosc_check_clock(result) && !rtc_eosc_check_clock(result))
	{
		print("[RTC] RTC 32K mode setting wrong. Enter first boot/recovery. \n");
		check_mode_flag = true;
	}

	print("[RTC] bbpu = 0x%x, con = 0x%x, osc32con = 0x%x, sec = 0x%x, yea = 0x%x\n", RTC_Read(RTC_BBPU), RTC_Read(RTC_CON), RTC_Read(RTC_OSC32CON), RTC_Read(RTC_AL_SEC), RTC_Read(RTC_AL_YEA));
	if ((RTC_Read(RTC_CON) & RTC_CON_LPSTA_RAW || check_mode_flag))
	{
		if (!rtc_first_boot_init()) {
			rtc_recovery_flow();
		}
		RTC_Write(RTC_BBPU, RTC_Read(RTC_BBPU) | RTC_BBPU_KEY | RTC_BBPU_RELOAD);
		Write_trigger();
	}
	else
	{
	/* normally HW reload is done in BROM but check again here */
		print("[RTC] powerkey1 = 0x%x, powerkey2 = 0x%x\n",
		RTC_Read(RTC_POWERKEY1), RTC_Read(RTC_POWERKEY2));
		RTC_Write(RTC_BBPU, RTC_Read(RTC_BBPU) | RTC_BBPU_KEY | RTC_BBPU_RELOAD);
		if (!Write_trigger())
		{
			rtc_recovery_flow();
		}else
		{
			if (!Writeif_unlock())
			{
				rtc_recovery_flow();
			}else
			{
				print("Writeif_unlock\n");
				if (RTC_Read(RTC_POWERKEY1) != RTC_POWERKEY1_KEY ||
					RTC_Read(RTC_POWERKEY2) != RTC_POWERKEY2_KEY)
				{
					print("[RTC] powerkey1 = 0x%x, powerkey2 = 0x%x\n",
						RTC_Read(RTC_POWERKEY1), RTC_Read(RTC_POWERKEY2));
					if (!rtc_first_boot_init()) {
						rtc_recovery_flow();
					}
				} else
				{
					rtc_osc_init();
				}
			}
		}
		// make sure RTC get the latest register info. //
		RTC_Write(RTC_BBPU, RTC_Read(RTC_BBPU) | RTC_BBPU_KEY | RTC_BBPU_RELOAD);
		Write_trigger();
	}

	RTC_Write(MT_TOP_CKTST2, RTC_Read(MT_TOP_CKTST2) | 0x0100);
	//Disable RTC CLK gating
	RTC_Write(MT_TOP_CKPDN1, 0x3ff);

	#ifdef RTC_LEAKAGE_TEST
	rtc_mode_test();
	#endif

	rtc_clean_mark();
	//set register to let MD know 32k status
	spar0 = RTC_Read(RTC_SPAR0);
	if(rtc_get_xosc_mode())
	{
		RTC_Write(RTC_SPAR0, (spar0 | RTC_SPAR0_32K_LESS) );
	}
	else
	{
		RTC_Write(RTC_SPAR0, (spar0 & ~RTC_SPAR0_32K_LESS) );
	}
	Write_trigger();

#if RTC_2SEC_REBOOT_ENABLE
	//after unlock, save the 2sec stat and clear(need unlock)
	//save and clear the stat before alarm IRQ check,
	//otherwise platform.c return directly, not clear the 2sec stat
	rtc_save_2sec_stat();
#endif //#ifdef RTC_2SEC_REBOOT_ENABLE
	RTC_Write(RTC_BBPU, RTC_Read(RTC_BBPU) | RTC_BBPU_KEY | RTC_BBPU_RELOAD);
	Write_trigger();
	irqsta = RTC_Read(RTC_IRQ_STA);	/* Read clear */
	pdn1 = RTC_Read(RTC_PDN1);
	pdn2 = RTC_Read(RTC_PDN2);
	spar0 = RTC_Read(RTC_SPAR0);
	spar1 = RTC_Read(RTC_SPAR1);
	print("[RTC] irqsta = 0x%x, pdn1 = 0x%x, pdn2 = 0x%x, spar0 = 0x%x, spar1 = 0x%x\n",
		  irqsta, pdn1, pdn2, spar0, spar1);
	print("[RTC] new_spare0 = 0x%x, new_spare1 = 0x%x, new_spare2 = 0x%x, new_spare3 = 0x%x\n",
		  RTC_Read(RTC_AL_HOU), RTC_Read(RTC_AL_DOM), RTC_Read(RTC_AL_DOW), RTC_Read(RTC_AL_MTH));
	print("[RTC] bbpu = 0x%x, con = 0x%x, cali = 0x%x\n", RTC_Read(RTC_BBPU), RTC_Read(RTC_CON), RTC_Read(RTC_AL_SEC));

	if (irqsta & RTC_IRQ_STA_AL) {
#if RTC_RELPWR_WHEN_XRST
		/* set AUTO bit because AUTO = 0 when PWREN = 1 and alarm occurs */
		U16 bbpu = RTC_Read(RTC_BBPU) | RTC_BBPU_KEY | RTC_BBPU_AUTO;
		RTC_Write(RTC_BBPU, bbpu);
		Write_trigger();
#endif

		if (pdn1 & RTC_PDN1_PWRON_TIME) {	/* power-on time is available */
			U16 now_sec, now_min, now_hou, now_dom, now_mth, now_yea;
			U16 irqen, sec, min, hou, dom, mth, yea;
			unsigned long now_time, time;
			unsigned long time_upper, time_lower;

			now_sec = RTC_Read(RTC_TC_SEC);
			now_min = RTC_Read(RTC_TC_MIN);
			now_hou = RTC_Read(RTC_TC_HOU);
			now_dom = RTC_Read(RTC_TC_DOM);
			now_mth = RTC_Read(RTC_TC_MTH);
			now_yea = RTC_Read(RTC_TC_YEA) + RTC_MIN_YEAR;
			if (RTC_Read(RTC_TC_SEC) < now_sec) {  /* SEC has carried */
				now_sec = RTC_Read(RTC_TC_SEC);
				now_min = RTC_Read(RTC_TC_MIN);
				now_hou = RTC_Read(RTC_TC_HOU);
				now_dom = RTC_Read(RTC_TC_DOM);
				now_mth = RTC_Read(RTC_TC_MTH);
				now_yea = RTC_Read(RTC_TC_YEA) + RTC_MIN_YEAR;
			}

			sec = ((spar0 & RTC_SPAR0_PWRON_SEC_MASK) >> RTC_SPAR0_PWRON_SEC_SHIFT);
			min = ((spar1 & RTC_SPAR1_PWRON_MIN_MASK) >> RTC_SPAR1_PWRON_MIN_SHIFT);
			hou = ((spar1 & RTC_SPAR1_PWRON_HOU_MASK) >> RTC_SPAR1_PWRON_HOU_SHIFT);
			dom = ((spar1 & RTC_SPAR1_PWRON_DOM_MASK) >> RTC_SPAR1_PWRON_DOM_SHIFT);
			mth = ((pdn2  & RTC_PDN2_PWRON_MTH_MASK) >> RTC_PDN2_PWRON_MTH_SHIFT);
			yea = ((pdn2  & RTC_PDN2_PWRON_YEA_MASK) >> RTC_PDN2_PWRON_YEA_SHIFT) + RTC_MIN_YEAR;

			now_time = rtc_mktime(now_yea, now_mth, now_dom, now_hou, now_min, now_sec);
			time = rtc_mktime(yea, mth, dom, hou, min, sec);

			print("[RTC] now = %d/%d/%d %d:%d:%d (%u)\n",
				  now_yea, now_mth, now_dom, now_hou, now_min, now_sec, now_time);
			print("[RTC] power-on = %d/%d/%d %d:%d:%d (%u)\n",
				  yea, mth, dom, hou, min, sec, time);

#if defined (MTK_KERNEL_POWER_OFF_CHARGING)
			if (kpoc_flag == true) {
				time_upper = time + 5;
				time_lower = time - 2;
			} else
 #endif
			{
				time_upper = time + 4;
				time_lower = time - 1;
			}
			//print("[RTC] now =%u time_upper = %u time_lower = %u\n", now_time, time_upper, time_lower);
			if (now_time >= time_lower && now_time <= time_upper) {	 /* power on */
				pdn1 = (pdn1 & ~RTC_PDN1_PWRON_TIME) | RTC_PDN1_BYPASS_PWR;
				RTC_Write(RTC_PDN1, pdn1);
				RTC_Write(RTC_PDN2, pdn2 | RTC_PDN2_PWRON_ALARM);
				Write_trigger();
				if (!(pdn2 & RTC_PDN2_PWRON_LOGO))   /* no logo means ALARM_BOOT */
					g_boot_mode = ALARM_BOOT;
#if defined (MTK_KERNEL_POWER_OFF_CHARGING)
			if ((pdn1 & RTC_PDN1_KPOC) == RTC_PDN1_KPOC) {
				kpoc_flag = false;
				RTC_Write(RTC_PDN1, pdn1 & ~RTC_PDN1_KPOC);
				Write_trigger();
			}
#endif
				return true;
			} else if (now_time < time) {   /* set power-on alarm */
				RTC_Write(RTC_AL_YEA, yea - RTC_MIN_YEAR);
				RTC_Write(RTC_AL_MTH, (RTC_Read(RTC_AL_MTH)&RTC_NEW_SPARE3)|mth);
				RTC_Write(RTC_AL_DOM, (RTC_Read(RTC_AL_DOM)&RTC_NEW_SPARE1)|dom);
				RTC_Write(RTC_AL_HOU, (RTC_Read(RTC_AL_HOU)&RTC_NEW_SPARE_FG_MASK)|hou);
				RTC_Write(RTC_AL_MIN, min);
				RTC_Write(RTC_AL_SEC, (RTC_Read(RTC_AL_SEC) & (~RTC_AL_SEC_MASK)) | (sec & RTC_AL_SEC_MASK));
				RTC_Write(RTC_AL_MASK, RTC_AL_MASK_DOW);	/* mask DOW */
				Write_trigger();
				irqen = RTC_Read(RTC_IRQ_EN) | RTC_IRQ_EN_ONESHOT_AL;
				RTC_Write(RTC_IRQ_EN, irqen);
				Write_trigger();
			}
		}
	}

	if ((pdn1 & RTC_PDN1_RECOVERY_MASK) == RTC_PDN1_FAC_RESET) {	/* factory data reset */
		/* keep bit 4 set until rtc_boot_check() in U-Boot */
		return true;
	}
#if defined (MTK_KERNEL_POWER_OFF_CHARGING)
	if ((pdn1 & RTC_PDN1_KPOC) == RTC_PDN1_KPOC) {
		kpoc_flag = true;
		return false;
	}
#endif
	return false;
#else
	return false;
#endif
}

void pl_power_off(void)
{
	ulong begin = get_timer(0);

	print("[RTC] pl_power_off\n");

	rtc_bbpu_power_down();

	while (1) {
		if (get_timer(begin) > 1000) {
			print("[RTC] pl_power_off timeout!!!!!\n");
			break;
		}
	}

	mtk_arch_reset(0);
}

static bool g_rtc_2sec_stat;
bool rtc_2sec_reboot_check(void)
{
#if RTC_2SEC_REBOOT_ENABLE
   print("rtc_2sec_reboot_check = %d\n", g_rtc_2sec_stat);
	return g_rtc_2sec_stat;
#else
	return false;
#endif
}

#if RTC_2SEC_REBOOT_ENABLE

static bool rtc_2sec_stat_clear(void)
{
	print("rtc_2sec_stat_clear\n");
	RTC_Write(RTC_CALI, RTC_Read(RTC_CALI) & ~RTC_CALI_BBPU_2SEC_STAT);
	if (!Write_trigger())
		return false;
	RTC_Write(RTC_CALI, RTC_Read(RTC_CALI) | RTC_CALI_BBPU_2SEC_STAT);
	if(!Write_trigger())
		return false;
	RTC_Write(RTC_CALI, RTC_Read(RTC_CALI) & ~RTC_CALI_BBPU_2SEC_STAT);
	if(!Write_trigger())
		return false;

	return true;
}

void rtc_save_2sec_stat(void)
{
	U16 cali, pdn2;
	static bool save_stat=false;

	if(save_stat==true)
		return;
	else
		save_stat = true;

	cali = RTC_Read(RTC_CALI);
	print("rtc_2sec_reboot_check cali=%d\n", cali);
	if (cali & RTC_CALI_BBPU_2SEC_EN) {
		switch((cali & RTC_CALI_BBPU_2SEC_MODE_MSK) >> RTC_CALI_BBPU_2SEC_MODE_SHIFT) {
			case 0:
			case 1:
			case 2:
				if(cali & RTC_CALI_BBPU_2SEC_STAT || pmic_IsVbatDrop() == PMIC_VBAT_DROP) {
					rtc_2sec_stat_clear();
					pdn2 = RTC_Read(RTC_PDN2);
					if(pdn2 & RTC_PDN2_AUTOBOOT) //IPO set shutdown
					{
						print("rtc IPO shutdown disable auto reboot\n");
						g_rtc_2sec_stat = false;
					}
					else
						{
						g_rtc_2sec_stat = true;
						print("g_rtc_2sec_stat = true\n");
					}
				} else {
					rtc_2sec_stat_clear();
					g_rtc_2sec_stat = false;
					print("g_rtc_2sec_stat = false\n");
				}
				break;
			case 3:
				rtc_2sec_stat_clear();
				print("g_rtc_2sec_stat = true, case3\n");
				g_rtc_2sec_stat = true;
			default:
				break;
		}
	}
}

void rtc_enable_2sec_reboot(void)
{
	U16 cali;
	U16 bbpu;

	print("rtc_enable_2sec_reboot original cali:0x%x\n", RTC_Read(RTC_CALI));
	cali = RTC_Read(RTC_CALI) | RTC_CALI_BBPU_2SEC_EN;
	cali = (cali & ~(RTC_CALI_BBPU_2SEC_MODE_MSK)) | (RTC_2SEC_MODE << RTC_CALI_BBPU_2SEC_MODE_SHIFT);
	RTC_Write(RTC_CALI, cali);
	Write_trigger();
	print("rtc_enable_2sec_reboot cali:0x%x\n", RTC_Read(RTC_CALI));
}
#endif //#ifdef RTC_2SEC_REBOOT_ENABLE

#ifdef RTC_LEAKAGE_TEST
void rtc_k_eosc_cali_td_set(unsigned int i)
{
	if(i==0)	// 1sec
		RTC_Write(RTC_K_EOSC_CALI_TD, 0x0061);
	else if(i==1)	// 8sec
		RTC_Write(RTC_K_EOSC_CALI_TD, 0x00C1);
	Write_trigger();
}


void rtc_mode_test(void)
{
	U32 sw_mode_select=4;
	U32 xosc_cali_val = 0x10;
	U32 val;

	if(sw_mode_select==0) {
		rtc_xosc_write(0x2a07);  /* assume crystal exist mode + XOSCCALI = 0x7 */
		rtc_xosc_write(0x2a00);	/* LP mode + XOSCCALI = 0x0 */
		print("XOSC mode \n");

	} if (sw_mode_select == 1) {
		rtc_xosc_write(0x910f); //set EM_HW_mode
		print("EMB_HW mode \n");
		mdelay(1);
		val = RTC_Read(RTC_OSC32CON);
		print("RTC_OSC32CON=0x%x\n", val);
	} if (sw_mode_select == 2) {
		rtc_xosc_write(0x918f); //set SW_DCXO_mode
		print("SW_DCXO mode \n");
		mdelay(1);
		val = RTC_Read(RTC_OSC32CON);
		print("RTC_OSC32CON=0x%x\n", val);
	} else if (sw_mode_select == 3) {
		rtc_xosc_write(0x91c0 | xosc_cali_val); //set SW_EOSC32_mode
		print("SW_EOSC32 mode \n");
		mdelay(1);
		val = RTC_Read(RTC_OSC32CON);
	} else if (sw_mode_select == 4) {
		RTC_Write(RTC_DIFF, 8000);
		Write_trigger();
		RTC_Write(RTC_CALI, 4200);
		Write_trigger();
		RTC_Write(RTC_AL_YEA, RTC_Read(RTC_AL_YEA) & (0x00ff) | 0x8100); //RTC_K_EOSC_RSV_7~0=0x81;
		Write_trigger();
		rtc_xosc_write(0x9160 | xosc_cali_val); //set SW_K_EOSC32_mode with VTCXO on/off


		print("SW_K_EOSC32 mode with VTCXO on/off\n");
		mdelay(1);
		val = RTC_Read(RTC_OSC32CON);
		print("RTC_OSC32CON=0x%x\n", val);

		//rtc_k_eosc_cali_td_set(0);	// 1 sec
		rtc_k_eosc_cali_td_set(1);	// 8 sec

	} else if (sw_mode_select == 5) {
		//rtc_xosc_write(0x914f); //set SW_K_EOSC32_mode with eosc clk on
		rtc_xosc_write(0x9160 | xosc_cali_val); //set SW_K_EOSC32_mode with VTCXO on
		RTC_Write(RTC_AL_YEA, RTC_Read(RTC_AL_YEA) & (0x00ff) | 0x8100); //RTC_K_EOSC_RSV_7~0=0x81;
		Write_trigger();
		print("SW_K_EOSC32 mode with VTCXO on/off\n");
		mdelay(1);
		val = RTC_Read(RTC_OSC32CON);
		print("RTC_OSC32CON=0x%x\n", val);

		rtc_k_eosc_cali_td_set(0);	// 1 sec
		rtc_k_eosc_cali_td_set(1);	// 8 sec
	}

}
#endif

static void rtc_get_tick(struct rtc_time *tm)
{
	tm->tm_sec = RTC_Read(RTC_TC_SEC);
	tm->tm_min = RTC_Read(RTC_TC_MIN);
	tm->tm_hour = RTC_Read(RTC_TC_HOU);
	tm->tm_mday = RTC_Read(RTC_TC_DOM);
	tm->tm_mon = RTC_Read(RTC_TC_MTH);
	tm->tm_year = RTC_Read(RTC_TC_YEA);
}

void rtc_get_time(struct rtc_time *tm) {
	rtc_get_tick(tm);
	if (RTC_Read(RTC_TC_SEC) < tm->tm_sec) {	/* SEC has carried */
		rtc_get_tick(tm);
	}
	tm->tm_year += RTC_MIN_YEAR;
}

#if (defined CFG_MICROTRUST_TEE_SUPPORT || defined CFG_MICROTRUST_TEE_LITE_SUPPORT)
unsigned long rtcgettime_teei(void)
{
	U16 now_sec, now_min, now_hou, now_dom, now_mth, now_yea;
	unsigned long now_time;

	now_sec = RTC_Read(RTC_TC_SEC);
	now_min = RTC_Read(RTC_TC_MIN);
	now_hou = RTC_Read(RTC_TC_HOU);
	now_dom = RTC_Read(RTC_TC_DOM);
	now_mth = RTC_Read(RTC_TC_MTH);
	now_yea = RTC_Read(RTC_TC_YEA) + RTC_MIN_YEAR;
	now_time = rtc_mktime(now_yea, now_mth, now_dom, now_hou, now_min,
								now_sec);
	return now_time;
}
#endif

