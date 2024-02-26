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

/******************************************************************************
 * gpio_init.c - MT67xx Preloader GPIO Initialization
 *
 * Copyright 2017 MediaTek Co.,Ltd.
 *
 * DESCRIPTION:
 *     default GPIO init
 *
 ******************************************************************************/

#include <gpio.h>

#if !defined(FPGA_PLATFORM)
#include <cust_power.h>
#include <cust_gpio_boot.h>
#endif

#include <platform.h>

//#include <debug.h>
#define GPIO_INIT_DEBUG
/*----------------------------------------------------------------------------*/
#define GPIOTAG "[GPIO] "
#define GPIODBG(fmt, arg...)    printf(GPIOTAG "%s: " fmt, __FUNCTION__ ,##arg)
#define GPIOERR(fmt, arg...)    printf(GPIOTAG "%s: " fmt, __FUNCTION__ ,##arg)
#define GPIOVER(fmt, arg...)    printf(GPIOTAG "%s: " fmt, __FUNCTION__ ,##arg)

#define GPIO_WR32(addr, data)   DRV_WriteReg32(addr,data)
#define GPIO_RD32(addr)         DRV_Reg32(addr)
/*----------------------------------------------------------------------------*/
#ifdef FPGA_PLATFORM
void mt_gpio_set_default(void)
{
	return;
}
#else

#include <gpio_init.h>

void mt_gpio_set_default_chip(void)
{
	unsigned pin = 0;
	for (pin = 0; pin < MT_GPIO_BASE_MAX; pin++) {
		/* set GPIOx_MODE*/
		mt_set_gpio_mode(0x80000000 + pin , gpio_array[pin].mode);

		/* set GPIOx_DIR*/
		if (gpio_array[pin].dir != 0xFF)
			mt_set_gpio_dir(0x80000000 + pin, gpio_array[pin].dir);

		/* set GPIOx_PULL*/
		if (gpio_array[pin].pull != 0xFF)
			mt_set_gpio_pull_select(0x80000000 + pin, gpio_array[pin].pull);

		/* set GPIOx_PULLEN*/
		if (gpio_array[pin].pullen != 0xFF)
			mt_set_gpio_pull_enable(0x80000000 + pin , gpio_array[pin].pullen);

		/* set GPIOx_DATAOUT*/
		if (gpio_array[pin].dataout != 0xFF)
			mt_set_gpio_out(0x80000000 + pin, gpio_array[pin].dataout);

		/* set GPIO0_SMT */
		if (gpio_array[pin].smt != 0xFF)
			mt_set_gpio_smt(0x80000000 + pin, gpio_array[pin].smt);
	}
}

void mt_gpio_set_default(void)
{
	mt_gpio_set_default_chip();

	return;
}
/*----------------------------------------------------------------------------*/
#if CHECK_POINT_TEST
#if defined(GPIO_INIT_DEBUG)
static GPIO_REGS saved;
#endif

void mt_gpio_checkpoint_save(void)
{
#if defined(GPIO_INIT_DEBUG)
	GPIO_REGS *pReg = (GPIO_REGS*)(GPIO_BASE);
	GPIO_REGS *cur = &saved;
	int idx;

	memset(cur, 0x00, sizeof(*cur));
	for (idx = 0; idx < sizeof(pReg->dir)/sizeof(pReg->dir[0]); idx++)
		cur->dir[idx].val = GPIO_RD32(&pReg->dir[idx]);
	#if 0
	for (idx = 0; idx < sizeof(pReg->pullen)/sizeof(pReg->pullen[0]); idx++)
		cur->pullen[idx].val = GPIO_RD32(&pReg->pullen[idx]);
	for (idx = 0; idx < sizeof(pReg->pullsel)/sizeof(pReg->pullsel[0]); idx++)
		cur->pullsel[idx].val =GPIO_RD32(&pReg->pullsel[idx]);
	#endif
	for (idx = 0; idx < sizeof(pReg->dout)/sizeof(pReg->dout[0]); idx++)
		cur->dout[idx].val = GPIO_RD32(&pReg->dout[idx]);
	for (idx = 0; idx < sizeof(pReg->mode)/sizeof(pReg->mode[0]); idx++)
		cur->mode[idx].val = GPIO_RD32(&pReg->mode[idx]);
#endif
}
/*----------------------------------------------------------------------------*/
EXPORT_SYMBOL(mt_gpio_checkpoint_save);
/*----------------------------------------------------------------------------*/
void mt_gpio_dump_diff(GPIO_REGS* pre, GPIO_REGS* cur)
{
#if defined(GPIO_INIT_DEBUG)
	GPIO_REGS *pReg = (GPIO_REGS*)(GPIO_BASE);
	int idx;
	unsigned char* p = (unsigned char*)pre;
	unsigned char* q = (unsigned char*)cur;

	GPIOVER("------ dumping difference between %p and %p ------\n", pre, cur);
	for (idx = 0; idx < sizeof(pReg->dir)/sizeof(pReg->dir[0]); idx++) {
		if (pre->dir[idx].val != cur->dir[idx].val)
			GPIOVER("diff: dir[%2d]    : 0x%08X <=> 0x%08X\n", idx, pre->dir[idx].val, cur->dir[idx].val);
	}
	#if 0
	for (idx = 0; idx < sizeof(pReg->pullen)/sizeof(pReg->pullen[0]); idx++) {
		if (pre->pullen[idx].val != cur->pullen[idx].val)
			GPIOVER("diff: pullen[%2d] : 0x%08X <=> 0x%08X\n", idx, pre->pullen[idx].val, cur->pullen[idx].val);
	}
	for (idx = 0; idx < sizeof(pReg->pullsel)/sizeof(pReg->pullsel[0]); idx++) {
		if (pre->pullsel[idx].val != cur->pullsel[idx].val)
			GPIOVER("diff: pullsel[%2d]: 0x%08X <=> 0x%08X\n", idx, pre->pullsel[idx].val, cur->pullsel[idx].val);
	}
	#endif
	for (idx = 0; idx < sizeof(pReg->dout)/sizeof(pReg->dout[0]); idx++) {
		if (pre->dout[idx].val != cur->dout[idx].val)
			GPIOVER("diff: dout[%2d]   : 0x%08X <=> 0x%08X\n", idx, pre->dout[idx].val, cur->dout[idx].val);
	}
	for (idx = 0; idx < sizeof(pReg->mode)/sizeof(pReg->mode[0]); idx++) {
		if (pre->mode[idx].val != cur->mode[idx].val)
			GPIOVER("diff: mode[%2d]   : 0x%08X <=> 0x%08X\n", idx, pre->mode[idx].val, cur->mode[idx].val);
	}

	for (idx = 0; idx < sizeof(*pre); idx++) {
		if (p[idx] != q[idx])
			GPIOVER("diff: raw[%2d]: 0x%02X <=> 0x%02X\n", idx, p[idx], q[idx]);
	}
	GPIOVER("memcmp(%p, %p, %d) = %d\n", p, q, sizeof(*pre), memcmp(p, q, sizeof(*pre)));
	GPIOVER("------ dumping difference end --------------------------------\n");
#endif
}
/*----------------------------------------------------------------------------*/
void mt_gpio_checkpoint_compare(void)
{
#if defined(GPIO_INIT_DEBUG)
	GPIO_REGS *pReg = (GPIO_REGS*)(GPIO_BASE);
	GPIO_REGS latest;
	GPIO_REGS *cur = &latest;
	int idx;

	memset(cur, 0x00, sizeof(*cur));
	for (idx = 0; idx < sizeof(pReg->dir)/sizeof(pReg->dir[0]); idx++)
		cur->dir[idx].val = GPIO_RD32(&pReg->dir[idx]);
	#if 0
	for (idx = 0; idx < sizeof(pReg->pullen)/sizeof(pReg->pullen[0]); idx++)
		cur->pullen[idx].val = GPIO_RD32(&pReg->pullen[idx]);
	for (idx = 0; idx < sizeof(pReg->pullsel)/sizeof(pReg->pullsel[0]); idx++)
		cur->pullsel[idx].val =GPIO_RD32(&pReg->pullsel[idx]);
	#endif
	for (idx = 0; idx < sizeof(pReg->dout)/sizeof(pReg->dout[0]); idx++)
		cur->dout[idx].val = GPIO_RD32(&pReg->dout[idx]);
	for (idx = 0; idx < sizeof(pReg->mode)/sizeof(pReg->mode[0]); idx++)
		cur->mode[idx].val = GPIO_RD32(&pReg->mode[idx]);

	//mt_gpio_dump_diff(&latest, &saved);
	//GPIODBG("memcmp(%p, %p, %d) = %d\n", &latest, &saved, sizeof(GPIO_REGS), memcmp(&latest, &saved, sizeof(GPIO_REGS)));
	if (memcmp(&latest, &saved, sizeof(GPIO_REGS))) {
		GPIODBG("checkpoint compare fail!!\n");
		GPIODBG("dump checkpoint....\n");
		//mt_gpio_dump(&saved);
		GPIODBG("\n\n");
		GPIODBG("dump current state\n");
		//mt_gpio_dump(&latest);
		GPIODBG("\n\n");
		mt_gpio_dump_diff(&saved, &latest);
		//WARN_ON(1);
	} else {
		GPIODBG("checkpoint compare success!!\n");
	}
#endif
}
/*----------------------------------------------------------------------------*/
EXPORT_SYMBOL(mt_gpio_checkpoint_compare);
/*----------------------------------------------------------------------------*/
#endif
#endif
